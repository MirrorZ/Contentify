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

  struct adv_frame{
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

  click_chatter("Hlen :%d Plen: %d Num_attr: %d, uid: %s, type:%s, attributes:%s, %s",p->hlen,p->plen,p->num_attr,station.id.c_str(),station.type.c_str(),attrs[0].c_str(),attrs[1].c_str());
  
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
