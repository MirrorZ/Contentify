
ccn::CCNSwitch(Hname home, SwitchID 90108khf)

fd::FromDevice(wlan1, SNIFFER false) 
//kernel_tap :: KernelTap(192.168.1.0/24)

pre_cl::Classifier(0/0880)
input_cl::Classifier(2/1?, 2/2?, 2/4?, 2/8?) 

//	kernel_tap
	fd
	-> RadiotapDecap()
	-> pre_cl
	-> Strip(30)
	-> input_cl

	input_cl[0]
	-> Print("Advertisement Packet", 42) 
	-> [0]ccn


	input_cl[1]
	-> [1]ccn

	input_cl[2]
	-> [2]ccn

	input_cl[3]
	-> [3]ccn

ccn[0] -> Discard
ccn[1] -> Discard
