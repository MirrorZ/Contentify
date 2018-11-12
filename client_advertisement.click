/*
 * Broadcast client address 
 * Protocol identifier for this frame is 07 00
 */


/*
InfiniteSource(DATA \<
  // Ethernet header
//  e4 a7 a0 58 bf 60 e0 2a 82 43 5b e3 90 00 45 00 01 81 c4 b1 00 00 40 11>, LIMIT 1, STOP true, ACTIVE true) 
//  ff ff ff ff ff ff e0 2a 82 43 5b e3 90 00 45 00 01 81 c4 b1 00 00 40 11>, LIMIT 1, STOP true, ACTIVE true) 
//  ff ff ff ff ff ff e0 2a 82 43 5b e3 08 06 00 01 08 00 06 04 00 01 ac 5f>, LIMIT 1, STOP true, ACTIVE true)
80 00 00 00 ff ff ff ff ff ff b8 c7 5d 04 b6 f5 b8 c7 5d 04 b6 f5 50 c3 83 5c 3a 38 5b 00 00 00 64 00 31 14 00 05 62 74 6f 77 6e 01 08 82 84 8b 96 0c 12 18 24 03 01 01 05 04 00 03 00 00 07 06 55 53 20 01 0b 1e 2a 01 00 32 04 30 48 60 6c 30 14 01 00 00 0f ac 04 01 00 00 0f ac 04 01 00 00 0f ac 02 00 00 2d 1a ad 51 1b ff ff ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 3d 16 01 00 11 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 46 05 02 00 01 00 00 dd 18 00 50 f2 02 01 01 01 00 03 a4 00 00 27 a4 00 00 42 43 5e 00 62 32 2f 00 dd 07 00 03 93 01 74 03 20 dd 0e 00 17 f2 07 00 01 01 06 b8 c7 5d 04 b6 f5 79 34 3f dd>, LIMIT 1, STOP true, ACTIVE true)
-> Print (Ping:)
-> Queue
-> ToDevice(wlan0);
*/




inf_src :: InfiniteSource(DATA \< //<ffff>, LIMIT 1, ACTIVE true, STOP true)
        08 80 00 00 ff ff ff ff ff ff e0 2a 82 43 5b e3 e0 2a 82 43 5b e3 00 00 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
	-> SetTXRate(2)
	-> RadiotapEncap()
	-> Print(Ping,254)
	-> ToDevice(wlan0);


