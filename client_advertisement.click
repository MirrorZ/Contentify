/*
 * Broadcast client address 
 * Protocol identifier for this frame is 07 00
 */

inf_src :: InfiniteSource(DATA \< //<ffff>, LIMIT 1, ACTIVE true, STOP true)
        08 80 00 00 ff ff ff ff ff ff e0 2a 82 43 5b e3 e0 2a 82 43 5b e3 00 00 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
	-> SetTXRate(2)
	-> RadiotapEncap()
	-> Print(Ping,254)
	-> ToDevice(tap3);


