#ifndef CCN_SWITCH_HH
#define CCN_SWITCH_HH
#include <click/element.hh>
#include <click/string.hh>
#include <click/timer.hh>
#include <clicknet/ether.h>
#include <time.h>

#include <set>
#include <map>
#include <vector>
#include <string>

CLICK_DECLS

/*
 *
 * ccnswitch element
 * 
 * Input[0] - Register Req/Update
 * Input[1] - Management Beacons from Switches
 * Input[2] - Data Frames (Req/Response)
 * 
 * Output[0] - output
 */

class CCNSwitch : public Element {
public:
  CCNSwitch();
  ~CCNSwitch();

  const char *class_name() const { return "CCNSwitch"; }

private:

  std::string switch_name;
  
  struct Station {
	std::string id;
	uint64_t timestamp;
	std::map<string, string> attributes;
  };

  std::map<string, Station> stations;
  void register(Packet *p);
}

CLICK_ENDDECLS
#endif
