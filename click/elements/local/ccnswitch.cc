#include <click/config.h>
#include "ccnswitch.hh"
#include <click/args.hh>
#include <click/error.hh>
#include <click/packet.hh>
#include <click/glue.hh>
#include <click/straccum.hh>
#include <click/packet_anno.hh>
#include <string>
#include <cstdio>
#include <map>


CLICK_DECLS

#define REFRESH_INTERVAL 60
#define CLIENT_REG 0
#define TYPE_WIDTH 12

CCNSwitch::CCNSwitch()
  : _timer(this) {}

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

  self_route.h_name = this->_hname;
  self_route.cost = 1;
  self_route.next_hop = this->_switch_id;
  
  return 0;
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

  uint8_t plen, hlen, num_attr;
  uint32_t uid;

  const char *p = reinterpret_cast<const char *>(buf);
  
  sscanf(p,"%c%c%c",&plen,&hlen,&num_attr);
  num_attr &= 0x0f; // mask first 4 bits
  click_chatter("num_attr :%c",num_attr);
  

  p+=3;

  sscanf(p,"%d",&uid);
  click_chatter("UID: %s",std::to_string(uid));
  
  p+=4;
  
  char typ[TYPE_WIDTH];
  memcpy(typ, p, TYPE_WIDTH);

  for(int i=0;i<12;i++)
	click_chatter("type: %x",typ[i]);

  p+=12;
  char k[32], v[32];
  std::string key, value;
  std::map<std::string,std::string> attrs;
  while(num_attr>0) {
	sscanf(p,"%s:%s",k,v);
	key = k;
	value = v;
	attrs.insert(std::pair<std::string,std::string>(key,value));
	num_attr--;
  }
  station_t station;
  
  station.id = uid;
  station.timestamp = Timestamp::now();
  station.type = typ;
  station.attributes = attrs;

		   

  return station;

}

void
CCNSwitch::push(int port, Packet *p) {

  switch(port) {
  case 0: // client registration
	register_station(p);
	p->kill();
	break;

  case 1: //management
	break;

  case 2: //data request
	break;

  case 3: //data response
	break;
	      
  }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CCNSwitch);
