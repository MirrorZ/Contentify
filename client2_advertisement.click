/*
 * Broadcast client address 
 * Protocol identifier for this frame is 0001
 */

inf_src :: InfiniteSource(DATA \< //<ffff>, LIMIT 1, ACTIVE true, STOP true)
//        08 80 00 00 ff ff ff ff ff ff e0 2a 82 43 5b e3 e0 2a 82 43 5b e3 00 00 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
        08 80 00 00 ff ff ff ff ff ff e0 2a 82 43 5b e3 e0 2a 82 43 5b e3 00 00 00 00 00 00 00 00 18 18 10 32 34 31 33 68 6f 73 74 00 68 6f 73 74 00 00 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
	-> SetTXRate(2)
	-> RadiotapEncap()
	-> Print(Ping,254)
//	-> ToDevice(wlan0);
	-> ToDevice(tap24);
