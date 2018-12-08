/*
 * Broadcast client(sensor) address 
 * Protocol identifier for this frame is 0001
 */

inf_src :: InfiniteSource(DATA \<
//        08 80 00 00 ff ff ff ff ff ff e0 2a 82 43 5b e3 e0 2a 82 43 5b e3 00 00 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
//        08 80 00 00 ff ff ff ff ff ff e0 2a 82 43 5b e3 e0 2a 82 43 5b e3 00 00 00 00 00 00 00 00 40 18 12 61 30 68 38 74 65 6d 70 00 74 65 6d 70 00 00 00 00 00 00 00 00 75 6e 69 74 3a 63 65 6c 63 69 75 73 00 00 00 00 00 00 00 00 72 65 73 6f 6c 75 74 69 6f 6e 3a 30 2e 31 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
        //ff ff ff ff ff ff e0 2a 82 43 5b e3 e0 2a 82 43 5b e3 
	00 00 00 00 00 00 00 00 40 18 12 61 30 68 38 74 65 6d 70 00 74 65 6d 70 00 00 00 00 00 00 00 00 75 6e 69 74 3a 63 65 6c 63 69 75 73 00 00 00 00 00 00 00 00 72 65 73 6f 6c 75 74 69 6f 6e 3a 30 2e 31 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
//        01 00 5e 7f ff fa e0 2a 82 43 5b e3 e0 2a 82 43 5b e3 00 00 00 00 00 00 00 00 40 18 12 61 30 68 38 74 65 6d 70 00 74 65 6d 70 00 00 00 00 00 00 00 00 75 6e 69 74 3a 63 65 6c 63 69 75 73 00 00 00 00 00 00 00 00 72 65 73 6f 6c 75 74 69 6f 6e 3a 30 2e 31 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
	-> EtherEncap(0x0880, e0:2a:82:43:5b:e3, ff:ff:ff:ff:ff:ff)
//	-> WifiEncap(0, ff:ff:ff:ff:ff:ff)
	-> ExtraEncap()
	-> SetTXRate(3)
	-> RadiotapEncap()
	-> Print(Ping,254)
	-> ToDevice(wlan1);
//	-> ToDevice(tap0);


