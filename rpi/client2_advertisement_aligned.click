# 6 "../../client2_advertisement.click"
inf_src :: InfiniteSource(DATA \<
        00 00 00 00 00 00 00 00 18 18 10 32 34 31 33 68 6f 73 74 00 68 6f 73 74 00 00 00 00 00 00 00 00>, LIMIT 1, STOP true, ACTIVE true);
# 8 "../../client2_advertisement.click"
EtherEncap@2 :: EtherEncap(0x0880, e0:2a:82:43:5b:e3, ff:ff:ff:ff:ff:ff);
# 9 "../../client2_advertisement.click"
ExtraEncap@3 :: ExtraEncap;
# 10 "../../client2_advertisement.click"
SetTXRate@4 :: SetTXRate(3);
# 11 "../../client2_advertisement.click"
RadiotapEncap@5 :: RadiotapEncap;
# 12 "../../client2_advertisement.click"
Print@6 :: Print(Ping,254);
# 13 "../../client2_advertisement.click"
ToDevice@7 :: ToDevice(wlan1);
# 17 ""
inf_src -> EtherEncap@2
    -> ExtraEncap@3
    -> SetTXRate@4
    -> RadiotapEncap@5
    -> Print@6
    -> ToDevice@7;
