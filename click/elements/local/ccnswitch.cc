#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/packet.hh>
#include <click/glue.hh>
#include <click/straccum.hh>
#include <click/packet_anno.hh>
#include <clicknet/wifi.h>
#include <click/etheraddress.hh>
#include <string>
#include <cstdio>
#include <map>

#include "ccnswitch.hh"


CCNSwitch::CCNSwitch() { 
}

CCNSwitch::register(Packet *p) {
  Station node;
  string station_id = getStationID(p);
  
  // update node info
  if(stations.find(node.id)) {
	stations[node.id].timestamp = CURRENT_TIME_STAMP;
  }
  else {
	node.id = station_id;
	node.attributes = getAttributes(p);
	node.timestamp = CURRENT_TIME_STAMP;
	map.insert(std::pair<string,Station>(station_id, node));
  }
}
