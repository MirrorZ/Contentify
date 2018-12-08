/*
 * Client request data
 * Protocol identifier for this frame is 0100
 */

inf_src :: RatedSource(\< 
        00 00 00 00 00 00 00 00 3f 2b 41 09 00 00 c3 50 74 65 6d 70 00 00 00 00 00 00 00 00 32 34 31 33 68 6f 73 74 00 2f 43 4d 55 2f 57 45 48 2f 34 46 2f 53 00 75 6e 69 74 3a 63 65 6c 63 69 75 73 00 00 00 00 00 00 00 00>, 1, 1)
	-> EtherEncap(0x0880, e0:2a:82:43:5b:e3, ff:ff:ff:ff:ff:ff)
	-> ExtraEncap()	
	-> SetTXRate(3)
	-> RadiotapEncap()
	-> Print(Ping,254, TIMESTAMP true)
	-> ToDevice(wlan1);
