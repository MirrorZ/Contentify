
ccn::CCNSwitch(Hname /CMU/WEH/4F/S, SwitchID 90108khf)

fd::FromDevice(wlan1, SNIFFER true) 
//kernel_tap1 :: KernelTap(192.168.1.0/24)


cl::Classifier(0/0880)
//pre_cl::Classifier(0/ff)
pre_cl::Classifier(24/ffff)

input_cl::Classifier(2/1?, 2/2?, 2/4?, 2/8?) 

//	kernel_tap1
	fd
//	-> Print("Before", 125)
	-> RadiotapDecap()
	-> pre_cl
	-> Strip(36)
	-> cl
	-> Strip(10)
	-> input_cl

	input_cl[0]
	//-> Print("Advertisement Packet", 50) 
	-> [0]ccn


	input_cl[1]
	-> Print("Req:",100, TIMESTAMP true)
	-> [1]ccn

	input_cl[2]
	-> Print("Req: ", 70)
	-> [2]ccn

	input_cl[3]
	-> [3]ccn


ccn[0]	   -> Print("\nSending", 70)
	   -> EtherEncap(0x0880, b8:27:eb:a7:3b:c1, ff:ff:ff:ff:ff:ff)
           -> ExtraEncap()
	   -> Queue()
           -> SetTXRate(3)
           -> RadiotapEncap()
	   -> Print("Res:",100, TIMESTAMP true)
	   -> ToDevice(wlan1)
	   

ccn[1] -> Discard
