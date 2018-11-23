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

CCNSwitch::CCNSwitch()
  : _timer(this) {}

CCNSwitch::~CCNSwitch() {}

int
CCNSwitch::configure(Vector<String> &conf, ErrorHandler* errh)
{
  int ret;
  String hname = String();
  String switch_id = String();
  //int hname;

  
  
  ret = Args(conf, this, errh)
	.read_mp("Hname", AnyArg(), hname)
    .read_mp("SwitchID", AnyArg(),switch_id)
	.complete();

  _hname = hname.c_str();
  _switch_id = switch_id.c_str();
  
  click_chatter("Got %s, %s\n",hname.c_str(), switch_id.c_str());
  

  /*
  _hname = "home";
  _switch_id = "0";
  */

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
  station_t new_station = parse_advertisement_packet(p);

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
CCNSwitch::parse_advertisement_packet(Packet *p) {  //, station_t *station) {
  uint8_t plen, hlen;
  uint32_t uid; 
  sscanf((char *)p,"%c%c%d",&plen,&hlen,&uid);
  p+=5;
  uint8_t num_attr;
  char typ[12];
  sscanf((char *)p,"%c",&num_attr);
  num_attr &= 31; // mask first 4 bits
  p++;
  memcpy(typ, p, 12);
  p+=12;
  char k[32], v[32];
  std::string key, value;
  std::map<std::string,std::string> attrs;
  while(num_attr>0) {
	sscanf((char *)p,"%s:%s",k,v);
	key = k;
	value = v;
	attrs.insert(std::pair<std::string,std::string>(key,value));
	num_attr--;
  }
  station_t station;
  
  station.id = std::to_string(uid);
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
