/*
 * Broadcast client(sensor) address
 * Protocol identifier for this frame is 0001
 */

inf_src :: InfiniteSource(DATA \<
        00 00 00 00 00 00 00 00 18 18 10 32 34 31 33 68 6f 73 74 00 68 6f 73 74 00 00 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true)
        -> EtherEncap(0x0880, e0:2a:82:43:5b:e3, ff:ff:ff:ff:ff:ff)
        -> ExtraEncap()
        -> SetTXRate(3)
        -> RadiotapEncap()
        -> Print(Ping,254)
        -> ToDevice(wlan1);

