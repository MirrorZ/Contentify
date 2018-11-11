/*
 * Broadcast client address 
 * Protocol identifier for this frame is 07 00
 */


/*
InfiniteSource(DATA \<
  // Ethernet header
  e4 a7 a0 58 bf 60 e0 2a 82 43 5b e3 90 00 45 00 01 81 c4 b1 00 00 40 11>, LIMIT 1, STOP true, ACTIVE true) 
//  ff ff ff ff ff ff e0 2a 82 43 5b e3 90 00 45 00 01 81 c4 b1 00 00 40 11>, LIMIT 1, STOP true, ACTIVE true) 
-> Print (Ping:)
-> Queue
-> ToDevice(wlan0);


*/


inf_src :: InfiniteSource(DATA <ffff>, LIMIT 1, ACTIVE true, STOP true) 
	-> EtherEncap(0x9000, e4:a7:a0:58:bf:60, ff:ff:ff:ff:ff:ff) 
        -> wifi_encap :: WifiEncap(0x00, 0:0:0:0:0:0) 
	-> set_rate :: SetTXRate(RATE 2) 
	-> ExtraEncap()
	-> Print(Ping: )
	-> ToDevice(wlan0);
