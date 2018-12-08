# 2 "switch.click"
ccn :: CCNSwitch(Hname /CMU/WEH/4F/S, SwitchID 90108khf);
# 4 "switch.click"
fd :: FromDevice(wlan1, SNIFFER true);
# 8 "switch.click"
cl :: Classifier(0/0880);
# 9 "switch.click"
pre_cl :: Classifier(24/ffff);
# 12 "switch.click"
input_cl :: Classifier(2/1?, 2/2?, 2/4?, 2/8?);
# 16 "switch.click"
RadiotapDecap@6 :: RadiotapDecap;
# 19 "switch.click"
Strip@7 :: Strip(36);
# 21 "switch.click"
Strip@8 :: Strip(10);
# 33 "switch.click"
Print@9 :: Print("\nReq: ", 70);
# 40 "switch.click"
Print@10 :: Print("\nSending", 70);
# 42 "switch.click"
Discard@11 :: Discard;
# 46 "switch.click"
Discard@12 :: Discard;
# 0 "<click-align>"
AlignmentInfo@click_align@13 :: AlignmentInfo(cl  4 2,
  pre_cl  4 2,
  input_cl  4 0);
# 30 ""
fd -> RadiotapDecap@6
    -> pre_cl
    -> Strip@7
    -> cl
    -> Strip@8
    -> input_cl
    -> ccn
    -> Print@10
    -> Discard@11;
input_cl [1] -> [1] ccn;
input_cl [2] -> Print@9
    -> [2] ccn;
input_cl [3] -> [3] ccn;
ccn [1] -> Discard@12;
