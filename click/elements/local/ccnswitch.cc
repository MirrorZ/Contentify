#include <click/config.h>
#include "ccnswitch.hh"
#include <click/args.hh>
#include <click/error.hh>
#include <click/packet.hh>
#include <click/glue.hh>
#include <click/straccum.hh>
#include <click/packet_anno.hh>
#include <algorithm>
#include <string>
#include <cstdio>
#include <map>

CLICK_DECLS

#define REFRESH_INTERVAL 60
#define CLIENT_REG 0
#define TYPE_WIDTH 12

CCNSwitch::CCNSwitch() : _timer(this) {}

CCNSwitch::~CCNSwitch() {}

int
CCNSwitch::configure(Vector<String> &conf, ErrorHandler* errh)
{
  int ret;
  String hname = String();
  String switch_id = String();
  
  ret = Args(conf, this, errh)
	.read_mp("Hname", AnyArg(), hname)
    .read_mp("SwitchID", AnyArg(),switch_id)
	.complete();

  if(ret < 0) {
	return -1;
  }
  
  _hname = hname.c_str();
  _switch_id = switch_id.c_str();

  return ret;
}

int
CCNSwitch::initialize(ErrorHandler *) {
  _timer.initialize(this);
  _timer.schedule_now();


  _fib_port = 32768;
  _parent_hname = generate_parent(_hname);
  
  self_route.h_name = this->_hname;
  self_route.cost = 1;
  self_route.next_hop = this->_switch_id;
  
  return 0;
}

std::vector<std::string>
CCNSwitch::generate_parent(std::string hname) {
  std::vector<std::string> parent;
  std::string name;
  
  int pos = 0;

  // Get all the possible parent hnames
  while(1) {
	pos = hname.find_last_of("/");
	if(pos == -1)
	  break;
	name = hname.substr(0,pos);
	hname = name;
	parent.push_back(name);
  }
  
  return parent;
}

void
CCNSwitch::register_station(Packet *p) {
  const unsigned char *buf = p->data();
  station_t new_station = parse_advertisement_packet(buf);
  std::string station_id = new_station.id;

  // station present, update timestamp
  if(stations.find(station_id)!=stations.end()) {
	new_station.timestamp = Timestamp::now();
	stations[new_station.id] = new_station;
	
	station_t st = stations[new_station.id];

	click_chatter("\n\n==============All Stations=======================");
	std::map<std::string, station_t>::iterator it;
	for(it = stations.begin(); it!= stations.end(); it++) {
	  station_t stn = it->second;
	  click_chatter("ID: %s Timestamp: %s Type: %s", stn.id.c_str(), stn.timestamp.unparse().c_str(), stn.type.c_str());
	}

	click_chatter("====================================================");
	
  }
  else  { // new registration
	stations.insert(std::pair<std::string,station_t>(station_id, new_station));
  }
}


void
CCNSwitch::advertise_route(route_t route) {
  Packet *p = generate_route_advertisement(route);
  output(1).push(p);
}

Packet *
CCNSwitch::generate_route_advertisement(route_t route) {
  Packet *p = (Packet *)malloc(sizeof(route_t));
  
  memcpy(p, (void *)&route, sizeof(route));
  //Write route to packet
  
  return p;
}

void
CCNSwitch::run_timer(Timer *) {
  // Advertise oneself
  advertise_route(self_route);
  _timer.reschedule_after_sec(REFRESH_INTERVAL);
}

station_t
CCNSwitch::parse_advertisement_packet(const unsigned char *buf) {

  struct adv_frame {
	uint8_t plen;
	uint8_t hlen;
	uint8_t num_attr;
	char uid[9];
	char typ[12];
	char attr[][20];
  };

  adv_frame *p = (adv_frame *)buf;
  // mask first 4 bits for num_attr
  p->num_attr &= 0x0f;
  
  station_t station;

  std::vector<std::string> attrs;
    
  for(int c=0; c<int(p->num_attr); c++) {
	attrs.push_back(p->attr[c]);
  }

  station.id = p->uid;

  station.timestamp = Timestamp::now();
  station.type = p->typ;
  station.attributes = attrs;
 
  return station;
}


Packet *
CCNSwitch::forward_data_request(Packet *p) {

  const unsigned char *buf = p->data();
  
  struct data_req {
	uint8_t plen;
	uint8_t hlen;
	uint8_t num_attr;
	uint8_t src_len;
	uint32_t port;
	char type[12];
  };

  
  data_req *meta = (data_req *)buf;
  meta->num_attr &= 0x0f; // mask first 4 bits

  meta->port = ntohl(meta->port);
  
  buf += 20;

  int sa_len = int(meta->src_len);
  char sa[sa_len];
  int da_len = int(meta->hlen) - sa_len - 20;
  char da[da_len];
 
  strncpy(sa, (const char *)buf, sa_len);
  buf += sa_len;

  strncpy(da, (const char *)buf, da_len);
  buf += da_len;

  int num_attrs = int(meta->num_attr);
  char attrs[20];
  std::vector<std::string> attributes;
  
  for(int i=0; i<num_attrs; i++) {
	strncpy(attrs, (const char *)buf, 20);
	buf+=20;
	attributes.push_back(attrs);
  }

  click_chatter("Got request with plen:%d hlen:%d num_attr:%d sa_len:%d port:%d type: %s sa: %s da:%s\n\n", int(meta->plen), int(meta->hlen), num_attrs, sa_len, int(meta->port), meta->type, sa, da);
  
  std::string destination = da;
  std::string source = sa;
  std::string next_hop = find_next_hop(destination, meta->type, attributes);
  uint32_t f_port;

  // Broadcast to other switches
  if(std::find(_parent_hname.begin(), _parent_hname.end(), destination) != _parent_hname.end()) {
	click_chatter("Broadcasting");
	output(1).push(p);
  }

  
  click_chatter("Finding next hop");
  if(!next_hop.empty()) {
	f_port = insert_fib(source, int(meta->port), next_hop);
	click_chatter("Next hop:%s, fport:%d", next_hop.c_str(),f_port);
	return update_packet(_hname, next_hop, f_port, int(meta->plen), int(meta->hlen), sa_len, da_len, p);
  }

  return p;
}

Packet *
CCNSwitch::forward_data_response(Packet *p) {

  const unsigned char *buf = p->data();
  
  struct data_req {
	uint8_t plen;
	uint8_t hlen;
	uint8_t num_attr;
	uint8_t src_len;
	uint32_t port;
	char type[12];
  };

  
  data_req *meta = (data_req *)buf;
  buf += 20; // move to next field

  int sa_len = int(meta->src_len);
  char sa[sa_len];
  int da_len = int(meta->hlen) - sa_len - 20;
  char da[da_len];
 
  strncpy(sa, (const char *)buf, sa_len);
  buf += sa_len;

  strncpy(da, (const char *)buf, da_len);
  buf += da_len;
  
  std::string destination = da;
  std::string source = sa;
  uint32_t portno;

  std::string dst, src;
  
  // Destination oneself
  if(destination == _hname) {
	lookup_fib(meta->port, &src, &dst, &portno);
  }
  
  // Handle other cases?
  
  return update_packet(src, dst, portno, meta->plen, meta->hlen, sa_len, da_len, p);
}

Packet 
CCNSwitch::update_packet(std::string src, std::string dst, uint32_t port, uint8_t old_plen, uint8_t old_hlen, uint8_t old_src_len, uint8_t old_dst_len, Packet *p) {
  
  WritablePacket *q = p->uniqueify();
  uint8_t src_len = src.length();
  uint8_t dst_len = dst.length();
  uint8_t hlen = old_hlen - (old_src_len + old_dst_len) + (src_len + dst_len);
  uint8_t plen = old_plen - old_hlen + hlen;

  unsigned char *buf = q->data();

  click_chatter("Before string %d %d %d %d", plen, hlen, port, src_len);
    
  memcpy(buf, (char *)&plen, 1);
  memcpy(buf+1, (char *)&hlen, 1);
  memcpy(buf+3, (char *)&src_len, 1);

  port = htonl(port);
  memcpy(buf+4, (char *)&(port), 4);
  
  memcpy(buf+20, src.c_str(), src_len);
  
  memcpy(buf+20+src_len, dst.c_str(), dst_len);
  
  return q;
}

std::string
CCNSwitch::find_next_hop(std::string dst, std::string type, std::vector<std::string>attrs) {
  std::string next_hop;

  // if requested for parent or oneself, find matching sensor
  if(std::find(_parent_hname.begin(),_parent_hname.end(), dst) != _parent_hname.end() ||  dst == _hname) {
	next_hop = filter_station(type,attrs);
  }
  else {
	// find the entry in routing table
	if(route_table.find(dst) != route_table.end()) {
	  next_hop = route_table[dst].next_hop;
	}
  }
  
  return next_hop;
}

std::string
CCNSwitch::filter_station(std::string type, std::vector<std::string>attrs) {
  std::map<std::string,station_t>::iterator it;
  std::string station;
  
  // look for matching type
  for(it=stations.begin(); it!=stations.end(); it++) {
	if((it->second).type == type) {
	  return it->first;
	}
	// compare attrs
  }

  return station;
}

uint32_t
CCNSwitch::insert_fib(std::string src, uint32_t port, std::string dst) {
  uint32_t portno;

  // next free port
  _fib_port = (_fib_port + 1) % 65536;

  addr_pair fib_pair;
  
  fib_pair.saddr = src;
  fib_pair.daddr = dst;

  // Make entry into the fib
  fib[_fib_port] = std::pair<struct addr_pair, uint32_t>(fib_pair, port);
  
  return _fib_port;
}

bool
CCNSwitch::lookup_fib(uint32_t port, std::string *da, std::string *sa, uint32_t *portno) {

  std::map<uint32_t, std::pair<struct addr_pair, uint32_t> >::iterator it;
  
  if((it = fib.find(port)) == fib.end()) {
	return false;
  }

  // get the fib entry
  struct addr_pair fib_pair = std::get<0>(it->second);
  *sa = fib_pair.saddr;
  *da = fib_pair.daddr;
  *portno = std::get<1>(it->second);
  
  return true;
}


void
CCNSwitch::push(int port, Packet *p) {

  Packet *fwd;
  
  switch(port) {
  case 0: // client registration
	register_station(p);
	p->kill();
	break;

  case 1: //management
	break;

  case 2: //data request
	if((fwd=forward_data_request(p))) {
	  output(0).push(fwd);
	}
	p->kill();
	break;

  case 3: //data response
	if((fwd=forward_data_response(p))) {
	  output(0).push(fwd);
	}
	p->kill();
	break;

  }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CCNSwitch);
