# 2 "/home/pi/click-rpi/switch.click"
ccn :: CCNSwitch(Hname /CMU/WEH/4F/S, SwitchID 90108khf);
# 4 "/home/pi/click-rpi/switch.click"
fd :: FromDevice(wlan0, SNIFFER true);
# 8 "/home/pi/click-rpi/switch.click"
pre_cl :: Classifier(0/0880);
# 9 "/home/pi/click-rpi/switch.click"
input_cl :: Classifier(2/1?, 2/2?, 2/4?, 2/8?);
# 13 "/home/pi/click-rpi/switch.click"
RadiotapDecap@5 :: RadiotapDecap;
# 15 "/home/pi/click-rpi/switch.click"
Strip@6 :: Strip(30);
# 27 "/home/pi/click-rpi/switch.click"
Print@7 :: Print("\nReq: ", 70);
# 34 "/home/pi/click-rpi/switch.click"
Print@8 :: Print("\nSending", 70);
# 36 "/home/pi/click-rpi/switch.click"
Discard@9 :: Discard;
# 40 "/home/pi/click-rpi/switch.click"
Discard@10 :: Discard;
# 0 "<click-align>"
AlignmentInfo@click_align@11 :: AlignmentInfo(pre_cl  4 2,
  input_cl  4 0);
# 25 ""
fd -> RadiotapDecap@5
    -> pre_cl
    -> Strip@6
    -> input_cl
    -> ccn
    -> Print@8
    -> Discard@9;
input_cl [1] -> [1] ccn;
input_cl [2] -> Print@7
    -> [2] ccn;
input_cl [3] -> [3] ccn;
ccn [1] -> Discard@10;
