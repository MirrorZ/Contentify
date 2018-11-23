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

typedef struct {
  std::string id;
  Timestamp timestamp;
  std::string type;
  std::map<std::string,std::string> attributes;
}station_t;

typedef struct {
  std::string h_name;

  // Next hop for local sensors is itself
  // Aggregation is supported by expoanding stations associated
  // with the requested type
  std::string next_hop;
  unsigned int cost;
}route_t;


class CCNSwitch : public Element {
public:
  CCNSwitch();
  ~CCNSwitch();

  const char *class_name() const { return "CCNSwitch"; }
  const char *port_count() const { return "4/2"; }
  const char *processing() const { return PUSH; }


  int configure(Vector<String> &, ErrorHandler *) CLICK_COLD;
  int initialize(ErrorHandler *errh) CLICK_COLD;
  void run_timer(Timer *);

  void push(int port, Packet *p);
  
  //private:

  Timer _timer;

  std::string _switch_id;
  std::string _hname;
  
  // map of hierarchical names to the route
  std::map<std::string,route_t> route_table;
  route_t self_route;

  // Maps every type supported to the coressponding set of sensors
  // The type is used for route advertisement
  std::map<std::string,station_t> stations;

  void register_station(Packet *);
  void advertise_route(route_t);
  Packet *generate_route_advertisement(route_t);
  station_t parse_advertisement_packet(const unsigned char *);
  
};

CLICK_ENDDECLS
#endif
 
