
ccn::CCNSwitch(Hname /CMU/WEH/4F/S, SwitchID 90108khf)

fd::FromDevice(wlan1, SNIFFER true) 
//kernel_tap1 :: KernelTap(192.168.1.0/24)


pre_cl::Classifier(0/0880)
input_cl::Classifier(2/1?, 2/2?, 2/4?, 2/8?) 

//	kernel_tap1
	fd
	-> RadiotapDecap()
	-> pre_cl
	-> Strip(30)
	-> input_cl

	input_cl[0]
	//-> Print("Advertisement Packet", 50) 
	-> [0]ccn


	input_cl[1]
	-> [1]ccn

	input_cl[2]
	-> Print("\nReq: ", 70)
	-> [2]ccn

	input_cl[3]
	-> [3]ccn


ccn[0] //-> RadiotapEncap()
	   -> Print("\nSending", 70)
	   -> Discard
	   //-> ToDevice(wlan0)
	   

ccn[1] -> Discard
