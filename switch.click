
ccn::CCNSwitch(Hname home, SwitchID 90108khf)

//fd::FromDevice(wlan0, SNIFFER true) 
kernel_tap :: KernelTap(192.168.1.0/24)


in_cl::Classifier(00/0880, 00/0881, 00/0882, 00/0883) 

	kernel_tap
	-> RadiotapDecap()
//	-> WifiDecap()
	-> in_cl

	in_cl[0]
	-> Print("Advertisement Packet", 42) 
	-> [0]ccn


	in_cl[1]
	-> [1]ccn

	in_cl[2]
	-> [2]ccn

	in_cl[3]
	-> [3]ccn

ccn[0] -> Discard
ccn[1] -> Discard
