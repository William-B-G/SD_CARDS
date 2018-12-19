// external global variables Global.h

#define SLC386 0
#define Stop_At_Lobby 0
#define Security 0
#define PEAK 1
#define auto_elevator 0

#define CR 0x0D
#define LF 0x0A
#define ESC 27

extern int16 start_int;
extern const int8 version;
extern const int8 revision;
extern const int8 interim_revision;
extern const int8 Sandbox_revision;

extern uint32 cons_date_code;

// Debug data
extern int32 debug1;
extern int32 debug2;
extern int32 debug3;
extern int32 debug4;
extern int32 debug5;
extern int32 debug6;
extern int32 debug7;
extern int32 debug8;

extern uint8 chkrun_state;
extern uint8 chkstart_state;
extern uint8 chklevel_state;
extern uint8 chkdoor_state;

extern uint8 Timer_tic;

extern int16 int_flag;
extern int16 int_err;
extern int16 ir_mask_1;
extern int16 ir_mask_2;
extern int16 dpy_io_data;

extern int16 DIO1_in;
extern int16 DIO2_out;

extern uint32 inctim_time;
extern uint32 inctim_min;
extern uint32 inctim_max;
extern uint32 inctim_avg;
extern uint32 inctim_now;
extern uint32 proctim_min;
extern uint32 proctim_max;
extern uint32 proctim_avg;
extern uint32 proctim_now;

extern uint8 pwr_up_init;				// flag that control software has been initialized on power up

extern int16 remote_cc;
extern int16 remote_rcc;
extern unsigned char misc_io_data;


// Timers used throught the program
extern int16 cctim[fl_size];          // car call timers
extern int16 ccrtim[fl_size];          // car call timers

enum {
 tinc,          	// Time generated from the 1/100 sec interupt
 tten,          	// 1 tenth of a second buffer
 tsec,          	// 1 second buffer
 tbuf,          	// buffer used to count 1 second
 tvpat,         	// voltage pattern timer
 tcom_response, 	// comunication response timer 
 tcom_que,			// Comm que timer
 tiofault,			// I/O fault timer
 tpatdly,			// Pattern delay timer
 tbep,          	// handicap beep timer
 tcablnt,     		// cab riding lantern timer
 tdc,           	// door close timer
 tdcycle,      		// door cycle timer
 tdo,          		// door open timer
 tdwel,        		// door stay open timer
 tfault,       		// delay after fault timer
 tgen,         		// generator run timer
 tlant,        		// door lantern timer
 tclf,    			// time to delay turn off the cab light and fan
 tmode3,       		// time in mode 3
 tmode5,       		// time in mode 5
 tmotion,      		// motion timer
 tnudge,       		// door nudge timer
 tpatd,        		// pattern delay timer
 tpredo,       		// door preopening delay timer
 trdc,         		// rear door close timer
 trdo,         		// rear door open timer
 treset,       		// reset timer
 trlant,       		// rear door lantern timer
 trnudge,      		// rear door nudge timer
 ccct,         		// clear car call timer
 tfs2t,        		// clear fire service phase 2 timer
 tnit,        		// non interference timer
 tfs1t,        		// fire service phase 1 used if on independent
 tdisplay,     		// diag leds am i alive counter
 tfltled,      		// fault blinking timer
 tdoor_reopen, 		// delay timer for high speed reversals on the door
 tgroup_hcc,   		// group hall call clear timer
 tsafe,        		// time after the DCL breaks until SAF makes up
 trst,				// reset timer
 tdppflt,			// digital pulse falure timer
 tff,				// field force output timer
 trdwel,			// rear door dwel timer
 trdoor_reopen, 	// delay timer for high speed reversals on rear
 tuser_com1,		// User interface comm timer
 tuser_com2,		// User interface comm timer
 tdrive_com,		// Drive comm timer
 tgcom_car0,  		// group comunication timer for car 0
 tgcom_car1,  		// group comunication timer for car 1
 tgcom_car2,  		// group comunication timer for car 2
 tgcom_car3, 		// group comunication timer for car 3
 tgcom_car4,  		// group comunication timer for car 4
 tgcom_car5,  		// group comunication timer for car 5
 tgcom_car6,  		// group comunication timer for car 6
 tgcom_car7,  		// group comunication timer for car 7
 tgcom_car8,  		// group comunication timer for car 8
 tgcom_master, 		// group comm master select timer
 tgc_tran,			// com 1 transmit interupt timer
 tgc_recv,			// com 1 receive interupt timer
 tcom1_tran,		// com 1 transmit interupt timer
 tcom2_tran,		// com 2 transmit interupt timer
 tcom1_recv,		// com 1 receive interupt timer
 tcom2_recv,		// com 2 receive interupt timer
 thct,				// Hall call common timer
 tlct,				// Logic common timer
 tsst,				// Saftey String Timer
 tsecure,			// Security timer length of time to enter code
 tsecbep,			// Security beep timer
 tydel,				// Generator y-delta timer
 tunused1,			// Unused: Previously Debounce timer for Emergency Power Manual Select Switch
 tepcar1,			// Recall time-out timer for Emergency Power Return Car
 tepcar2,			// Recall time-out timer for Emergency Power Return Car
 tepcar3,			// Recall time-out timer for Emergency Power Return Car
 tepcar4,			// Recall time-out timer for Emergency Power Return Car
 tepcar5,			// Recall time-out timer for Emergency Power Return Car
 tepcar6,			// Recall time-out timer for Emergency Power Return Car
 tepcar7,			// Recall time-out timer for Emergency Power Return Car
 tepcar8,			// Recall time-out timer for Emergency Power Return Car
 tsoftstp,			// Soft Stop Timer
 tiofltrst,			// I/O fault reset timer  
 tins_flt,			// Inspection input fault timer
 tbyp_flt, 			// Gate or Lock bypass switch fault timer
 tgripper,			// Timer for rope gripper test
 tparking,			// Parking delay timer
 tcan,				// Can Bus timer
 tcantbs,			// Can Transmit buffer status timer
 tcanrbs,			// Can Receive buffer status timer
 tcantx1,			// Can device 1 output re-transmit timer
 tcantx2,			// Can device 2 output re-transmit timer
 tcantx3,			// Can device 3 output re-transmit timer
 tcantx4,			// Can device 4 output re-transmit timer
 tcantx5,			// Can device 5 output re-transmit timer
 tcantx6,			// Can device 6 output re-transmit timer
 tcantx7,			// Can device 7 output re-transmit timer
 tcantx8,			// Can device 8 output re-transmit timer
 tcantx9,			// Can device 9 output re-transmit timer
 tcantx10,			// Can device 10 output re-transmit timer
 tcantx11,			// Can device 11 output re-transmit timer
 tcantx12,			// Can device 12 output re-transmit timer
 tcantx13,			// Can device 13 output re-transmit timer
 tcantx14,			// Can device 14 output re-transmit timer
 tcantx15,			// Can device 15 output re-transmit timer
 tcantx16,			// Can device 16 output re-transmit timer
 tcantx17,			// Can device 17 output re-transmit timer
 tcantx18,			// Can device 18 output re-transmit timer
 tcantx19, 			// Can device 19 output re-transmit timer
 tcantx20, 			// Can device 20 output re-transmit timer
 tcantx21, 			// Can device 21 output re-transmit timer
 tcantx22, 			// Can device 22 output re-transmit timer
 tcantx23,			// Can device 23 output re-transmit timer
 tcantx24,			// Can device 24 output re-transmit timer
 tcantx25,			// Can device 25 output re-transmit timer
 tcantx26,			// Can device 26 output re-transmit timer
 tcantx27,			// Can device 27 output re-transmit timer
 tcantx28,			// Can device 28 output re-transmit timer
 tcantx29, 			// Can device 29 output re-transmit timer
 tcantx30, 			// Can device 30 output re-transmit timer
 tcantx31, 			// Can device 31 output re-transmit timer
 tcantx32, 			// Can device 32 output re-transmit timer
 tcantx33,			// Can device 33 output re-transmit timer
 tcantx34,			// Can device 34 output re-transmit timer
 tcantx35,			// Can device 35 output re-transmit timer
 tcantx36,			// Can device 36 output re-transmit timer
 tcantx37,			// Can device 37 output re-transmit timer
 tcantx38,			// Can device 38 output re-transmit timer
 tcantx39, 			// Can device 39 output re-transmit timer
 tcantx40, 			// Can device 40 output re-transmit timer
 tcanrx1,			// Can device 1 input timer
 tcanrx2,			// Can device 2 input timer
 tcanrx3,			// Can device 3 input timer
 tcanrx4,			// Can device 4 input timer
 tcanrx5,			// Can device 5 input timer
 tcanrx6,			// Can device 6 input timer
 tcanrx7,			// Can device 7 input timer
 tcanrx8,			// Can device 8 input timer
 tcanrx9,   		// Can device 9 input timer
 tcanrx10,			// Can device 10 input timer
 tcanrx11,			// Can device 11 input timer
 tcanrx12,			// Can device 12 input timer
 tcanrx13,			// Can device 13 input timer
 tcanrx14,			// Can device 14 input timer
 tcanrx15,			// Can device 15 input timer
 tcanrx16,			// Can device 16 input timer
 tcanrx17,			// Can device 17 input timer
 tcanrx18,  		// Can device 18 input timer
 tcanrx19,			// Can device 19 input timer
 tcanrx20, 			// Can device 20 input timer
 tcanrx21, 			// Can device 21 input timer
 tcanrx22, 			// Can device 22 input timer
 tcanrx23,			// Can device 23 input timer
 tcanrx24,			// Can device 24 input timer
 tcanrx25,			// Can device 25 input timer
 tcanrx26,			// Can device 26 input timer
 tcanrx27,			// Can device 27 input timer
 tcanrx28,  		// Can device 28 input timer
 tcanrx29,			// Can device 29 input timer
 tcanrx30, 			// Can device 30 input timer
 tcanrx31, 			// Can device 31 input timer
 tcanrx32, 			// Can device 32 input timer
 tcanrx33,			// Can device 33 input timer
 tcanrx34,			// Can device 34 input timer
 tcanrx35,			// Can device 35 input timer
 tcanrx36,			// Can device 36 input timer
 tcanrx37,			// Can device 37 input timer
 tcanrx38,  		// Can device 38 input timer
 tcanrx39,			// Can device 39 input timer
 tcanrx40, 			// Can device 40 input timer
 tpilant_ctcan,		// Can PI and lantern message timer
 tpilant_mrcan,		// Can PI and lantern message timer
 temhc1,			// Emergency dispatch hall call timer 1
 temhc2, 			// Emergency dispatch hall call timer 2
 temhc3,			// Emergency dispatch hall call timer 3
 temhc4,			// Emergency dispatch hall call timer 4
 temhc5, 			// Emergency dispatch hall call timer 5
 temhc6,			// Emergency dispatch hall call timer 6
 temrhc1,			// Emergency dispatch rear hall call timer 1
 temrhc2, 			// Emergency dispatch rear hall call timer 2
 temrhc3,			// Emergency dispatch rear hall call timer 3
 temrhc4,			// Emergency dispatch rear hall call timer 4
 temrhc5,			// Emergency dispatch rear hall call timer 5
 temrhc6,			// Emergency dispatch rear hall call timer 6
 tglkout,			// Group car call lockout update timers (send lockout data to cars)
 tncu,				// Next Car Up timer
 tdlt, 				// door lock top secondary i/o timer
 tdlm, 				// door lock middle secondary i/o timer
 tdlb, 				// door lock bottom secondary i/o timer
 tgs, 				// gate switch secondary i/o timer
 trlm, 				// rear door lock middle secondary i/o timer
 trgs, 				// rear gate switch secondary i/o timer
 tbrkon, 			// brake fail on i/o timer
 tbrkoff, 			// brake fail off i/o timer
 tdonon, 			// Drive ON fail on i/o timer
 tdonoff, 			// Drive ON fail off i/o timer
 tuppeak,			// Up peak duration timer
 tuptrig, 			// Up peak trigger timer
 tdnpeak,			// Down Peak duration timer
 tdptrig, 			// Down Peak trigger timer
 txasgn, 			// Hall Call cross assignment timer
 tmedf,				// Medical Emergency Service timer
 tfltcarcom, 		// Comm timer for fault to be received from car
 trhjack,			// reset hydro jack timer
 tvideo, 			// Video time out
 tloadw, 			// Load weigher timer
 theartbeat,		// ids Liftnet heartbeat timer
 trelev, 			// relevel delay timer
 tdpm, 				// Door protection module i/o timer
 trpm, 				// Rear door protection module i/o timer
 tgpio, 			// Timer to transfer group io to or from car
 tiract, 			// Inconspicious Riser active timer
 thbcc, 			// Car call handicap buzzer timer
 tattbz, 			// Attendant buzzer timer
 tpst, 				// power status timer
 tdodly, 			// door open delay
 tdcdly, 			// door close delay
 trdodly,			// rear door open delay
 trdcdly, 			// rear door close delay
 tdoorbz, 			// Manual door buzzer delay timer
 trcpkdly, 			// retiring cam pick delay timer
 tlimit, 			// Terminal limit debounce timer
 tredundancy, 			// pfc and sfc relay test timer
 tbkdrop, 			// brake drop debounce
 tpinflt, 			// p-input fault debounce timer
 tspbupd, 			// Safety processor	update timer
 tlwupd, 			// Load weigher update timer 
 tbrkupd, 			// Brake board update timer 
 tspbts,			// Safety processor board temperature sensor update timer
 tvsupd, 			// Voltage sensor board update timer
 tfltdelay, 		// Fault delay timer to send faults to user interface
 tsufsdfdly,		// Fault timer for UPF and DNF ON fault
 tinsbrk, 			// Lift brake on inspection max timer
 teeto, 			// Electric Eye Time-out timer
 treeto,			// Rear Electric Eye Time-out timer
 trcablnt, 			// Rear cab riding lantern timer
 tjackdly,			// Reset jack delay timer
 tdrvrst, 			// Drive reset timer
 tfsdo, 			// Front swing door open timer
 trsdo, 			// Rear swing door open timer
 tbksrun, 			// Brake switch run timer
 tdrvrdy, 			// Drive ready debounce timer
 tlevto,			// Leveling time-out timer (time targeting for the floor)
 tautolnhw,			// Auto Learn Hoistway timer.
 tintmot, 			// Intended Motiont timer
 tattbzcyc,			// Attendant buzzer cycle timer used to cycle buzzer intermittently
 // tbkpkrate,			// Brake pick rate time
 tshortfl, 			// Short floor slowdown timer
 tdoorupd, 			// Door can update timer
 trdoorupd, 		// Rear door can update timer
 tunused2, 			// Unused: Previously Emergency Power Recall Delay timer
 tgsecovr, 			// Group car call security override comm timer
 tuldloff,			// UL/DL off timer
 tebrkio, 			// Emergency brake contactor i/o debounce timer
 tebkpick, 			// Emergency brake lift time
 tebkdrop, 			// Emergency brake drop time
 tdonbrkflt, 		// DON and BRKi input fault timer
 tsabbath,			// Sabbath restart timer
 thcpl,				// Hall call power loss timer
 tpwrdo,			// Power door open timer
 tpwrdor,			// Power rear door open timer
 tpassword, 		// Password timer
 tgovrst, 			// Governor reset timer
 tdoorrq,			// Door request time-out timer
 tfdo_adv_de,		// Front door advance door enable non-simulatneous doors
 trdo_adv_de,		// Rear door advance door enable non-simulatneous doors
 tdl, 				// Dl down limit io check timer
 tul, 				// Ul Up limit io check timer 
 taltpi_on, 		// alternate between different ascii characters on digital pi's
 taltpi_off, 		// alternate bewteen different ascii characters on digital pi's
 tFlash, 			// LCD  flash timer
 tFbuff, 			// LCD  flash buffer timer
 theoff, 			// Heoff switch debounce timer 
 tbkdroprate, 		// Brake Voltage Drop Rate timer
 trcdrdly,			// Retiring cam drop delay timer
 tlcd_keb_dly, 		// Lcd timer for KEB drive
 tpwl_pwr_on, 		// Power Loss power on timer
 tpwl_pwr_off, 		// Power Loss power off timer
 tsx_txA,			// Serial Expansion A Transmit timer
 tsx_rxA,			// Serial Expansion A Receive timer
 tsx_txB,			// Serial Expansion B Transmit timer
 tsx_rxB,			// Serial Expansion B Receive timer
 tcen, 				// CEN debounce timer
 tattbzini, 		// Attendant timer to initialize buzzer
 tadvptrq,			// Advanced pre-torque safe timer
 tgovtrip,			// Governor reset timer	
 tirots,			// IR Out of Service Timer to cancel IR hall calls
 tdmo,				// Door motor overload timer
 tEP_npd,			// Emergency Power Normal Power Drive timer
 tEP_epd,			// Emergency Power EP Drive timer
 tEP_epo,			// Emergency Power EP Off timer
 tinst, 			// Inspection debounce timer
 tcol, 				// Collision switch debounce
 tospdbk, 			// Overspeed brake hold timer
 teprcvr1,			// Recover time-out timer for Emergency Power Return Car
 teprcvr2,			// Recover time-out timer for Emergency Power Return Car
 teprcvr3,			// Recover time-out timer for Emergency Power Return Car
 teprcvr4,			// Recover time-out timer for Emergency Power Return Car
 teprcvr5,			// Recover time-out timer for Emergency Power Return Car
 teprcvr6,			// Recover time-out timer for Emergency Power Return Car
 teprcvr7,			// Recover time-out timer for Emergency Power Return Car
 teprcvr8,			// Recover time-out timer for Emergency Power Return Car
 tdoorhold,			// max door hold timer
 tatthc,			// Attendant hall call annunciator timer
 tccsync, 			// Car Call security comm sync check timer
 tattsync,			// Attendant annunciator sync check timer
 trcmon,			// Retiring cam ON energized
 tebkpkrate, 		// Emergency Brake Pick Rate timer
 tebkdroprate, 		// Emergency Brake Drop rate timer
 tebrkupd, 			// Emergency Brake board update timer
 tvipseqto,			// VIP sequence time-out timer
 talrm_pi,			// Alarm display timer for PIs
 tcbovrind,			// Code Blue Over Independent Delay
 theofovrind, 		// Hall Elevator Off over ind delay
 tpwl_brk_on, 		// Power Loss brake on timer
 tpwl_brk_off, 		// Power Loss brake off timer
 tosernocall,		// timer to control oser option 0x01	
 toserbtwfl, 		// timer to control oser option 0x02
 toserssopen, 		// timer to control oser option 0x04
 tlocalgetdate, 	// tiemr to delay the date check for update of call time tables
 tkebrun,			// Keb run mode check timer
 trelevstdir,	 	// Relevel start direction debounce timer
 tepsw,				// Debounce timer for Emergency Power Manual Select Switch 
 teprcldly, 		// Emergency Power Recall Delay timer 
 thugs1,			// Timer for prealarm mode of hugs service type 3
 thugs2,			// Timer for Alarm status of hugs service type 3
 tlantchange, 		// lantern change delay timeer
 ttoutmandoor, 		// timeout manual door for hall calls
 ttoutmandoordbnc, 	//timeout manual door for hall calls debopunce
 tenc_inifail, 		// Encoder init fail timer
 tenc_hb,			// Encoder heart beat timer
 tenc_init,			// Encoder init timer
 tenc_pktreq, 		// Encoder packet request timer
 tprntcomto,		// print to com time-out
 tgts_ex, 			// gts emerency exit timer
 tfire_fdo,			// phase 1 fire front door open time for denver fire service
 tfire_rdo,			// phase 1 fire rear door open time for denver fire service
 tmrcantx1,			// Can device 1 output re-transmit timer
 tmrcantx2,			// Can device 2 output re-transmit timer
 tmrcantx3,			// Can device 3 output re-transmit timer
 tmrcantx4,			// Can device 4 output re-transmit timer
 tmrcantx5,			// Can device 5 output re-transmit timer
 tmrcantx6,			// Can device 6 output re-transmit timer
 tmrcantx7,			// Can device 7 output re-transmit timer
 tmrcantx8,			// Can device 8 output re-transmit timer
 tmrcantx9,			// Can device 9 output re-transmit timer
 tmrcantx10,		// Can device 10 output re-transmit timer
 tmrcantx11,		// Can device 11 output re-transmit timer
 tmrcantx12,		// Can device 12 output re-transmit timer
 tmrcantx13,		// Can device 13 output re-transmit timer
 tmrcantx14,		// Can device 14 output re-transmit timer
 tmrcantx15,		// Can device 15 output re-transmit timer
 tmrcantx16,		// Can device 16 output re-transmit timer
 tmrcantx17,		// Can device 17 output re-transmit timer
 tmrcantx18,		// Can device 18 output re-transmit timer
 tmrcantx19, 		// Can device 19 output re-transmit timer
 tmrcantx20, 		// Can device 20 output re-transmit timer
 tmrcantx21, 		// Can device 21 output re-transmit timer
 tmrcantx22, 		// Can device 22 output re-transmit timer
 tmrcantx23,		// Can device 23 output re-transmit timer
 tmrcantx24,		// Can device 24 output re-transmit timer
 tmrcantx25,		// Can device 25 output re-transmit timer
 tmrcantx26,		// Can device 26 output re-transmit timer
 tmrcantx27,		// Can device 27 output re-transmit timer
 tmrcantx28,		// Can device 28 output re-transmit timer
 tmrcantx29, 		// Can device 29 output re-transmit timer
 tmrcantx30, 		// Can device 30 output re-transmit timer
 tmrcantx31, 		// Can device 31 output re-transmit timer
 tmrcantx32, 		// Can device 32 output re-transmit timer
 tmrcantx33,		// Can device 33 output re-transmit timer
 tmrcantx34,		// Can device 34 output re-transmit timer
 tmrcantx35,		// Can device 35 output re-transmit timer
 tmrcantx36,		// Can device 36 output re-transmit timer
 tmrcantx37,		// Can device 37 output re-transmit timer
 tmrcantx38,		// Can device 38 output re-transmit timer
 tmrcantx39, 		// Can device 39 output re-transmit timer
 tmrcantx40, 		// Can device 40 output re-transmit timer
 tmrcanrx1,			// Can device 1 input timer
 tmrcanrx2,			// Can device 2 input timer
 tmrcanrx3,			// Can device 3 input timer
 tmrcanrx4,			// Can device 4 input timer
 tmrcanrx5,			// Can device 5 input timer
 tmrcanrx6,			// Can device 6 input timer
 tmrcanrx7,			// Can device 7 input timer
 tmrcanrx8,			// Can device 8 input timer
 tmrcanrx9,   		// Can device 9 input timer
 tmrcanrx10,		// Can device 10 input timer
 tmrcanrx11,		// Can device 11 input timer
 tmrcanrx12,		// Can device 12 input timer
 tmrcanrx13,		// Can device 13 input timer
 tmrcanrx14,		// Can device 14 input timer
 tmrcanrx15,		// Can device 15 input timer
 tmrcanrx16,		// Can device 16 input timer
 tmrcanrx17,		// Can device 17 input timer
 tmrcanrx18,  		// Can device 18 input timer
 tmrcanrx19,		// Can device 19 input timer
 tmrcanrx20, 		// Can device 20 input timer
 tmrcanrx21, 		// Can device 21 input timer
 tmrcanrx22, 		// Can device 22 input timer
 tmrcanrx23,		// Can device 23 input timer
 tmrcanrx24,		// Can device 24 input timer
 tmrcanrx25,		// Can device 25 input timer
 tmrcanrx26,		// Can device 26 input timer
 tmrcanrx27,		// Can device 27 input timer
 tmrcanrx28,  		// Can device 28 input timer
 tmrcanrx29,		// Can device 29 input timer
 tmrcanrx30, 		// Can device 30 input timer
 tmrcanrx31, 		// Can device 31 input timer
 tmrcanrx32, 		// Can device 32 input timer
 tmrcanrx33,		// Can device 33 input timer
 tmrcanrx34,		// Can device 34 input timer
 tmrcanrx35,		// Can device 35 input timer
 tmrcanrx36,		// Can device 36 input timer
 tmrcanrx37,		// Can device 37 input timer
 tmrcanrx38,  		// Can device 38 input timer
 tmrcanrx39,		// Can device 39 input timer
 tmrcanrx40, 		// Can device 40 input timer
 tgrcantx1,			// Can device 1 output re-transmit timer
 tgrcantx2,			// Can device 2 output re-transmit timer
 tgrcantx3,			// Can device 3 output re-transmit timer
 tgrcantx4,			// Can device 4 output re-transmit timer
 tgrcantx5,			// Can device 5 output re-transmit timer
 tgrcantx6,			// Can device 6 output re-transmit timer
 tgrcantx7,			// Can device 7 output re-transmit timer
 tgrcantx8,			// Can device 8 output re-transmit timer
 tgrcantx9,			// Can device 9 output re-transmit timer
 tgrcantx10,		// Can device 10 output re-transmit timer
 tgrcantx11,		// Can device 11 output re-transmit timer
 tgrcantx12,		// Can device 12 output re-transmit timer
 tgrcantx13,		// Can device 13 output re-transmit timer
 tgrcantx14,		// Can device 14 output re-transmit timer
 tgrcantx15,		// Can device 15 output re-transmit timer
 tgrcantx16,		// Can device 16 output re-transmit timer
 tgrcantx17,		// Can device 17 output re-transmit timer
 tgrcantx18,		// Can device 18 output re-transmit timer
 tgrcantx19, 		// Can device 19 output re-transmit timer
 tgrcantx20, 		// Can device 20 output re-transmit timer
 tgrcantx21, 		// Can device 21 output re-transmit timer
 tgrcantx22, 		// Can device 22 output re-transmit timer
 tgrcantx23,		// Can device 23 output re-transmit timer
 tgrcantx24,		// Can device 24 output re-transmit timer
 tgrcantx25,		// Can device 25 output re-transmit timer
 tgrcantx26,		// Can device 26 output re-transmit timer
 tgrcantx27,		// Can device 27 output re-transmit timer
 tgrcantx28,		// Can device 28 output re-transmit timer
 tgrcantx29, 		// Can device 29 output re-transmit timer
 tgrcantx30, 		// Can device 30 output re-transmit timer
 tgrcantx31, 		// Can device 31 output re-transmit timer
 tgrcantx32, 		// Can device 32 output re-transmit timer
 tgrcantx33,		// Can device 33 output re-transmit timer
 tgrcantx34,		// Can device 34 output re-transmit timer
 tgrcantx35,		// Can device 35 output re-transmit timer
 tgrcantx36,		// Can device 36 output re-transmit timer
 tgrcantx37,		// Can device 37 output re-transmit timer
 tgrcantx38,		// Can device 38 output re-transmit timer
 tgrcantx39, 		// Can device 39 output re-transmit timer
 tgrcantx40, 		// Can device 40 output re-transmit timer
 tgrcantx41, 		// Can device 41 output re-transmit timer
 tgrcantx42, 		// Can device 42 output re-transmit timer
 tgrcantx43,		// Can device 43 output re-transmit timer
 tgrcantx44,		// Can device 44 output re-transmit timer
 tgrcantx45,		// Can device 45 output re-transmit timer
 tgrcantx46,		// Can device 46 output re-transmit timer
 tgrcantx47,		// Can device 47 output re-transmit timer
 tgrcantx48,		// Can device 48 output re-transmit timer
 tgrcantx49, 		// Can device 49 output re-transmit timer
 tgrcantx50, 		// Can device 50 output re-transmit timer
 tgrcantx51, 		// Can device 51 output re-transmit timer
 tgrcantx52, 		// Can device 52 output re-transmit timer
 tgrcantx53,		// Can device 53 output re-transmit timer
 tgrcantx54,		// Can device 54 output re-transmit timer
 tgrcantx55,		// Can device 55 output re-transmit timer
 tgrcantx56,		// Can device 56 output re-transmit timer
 tgrcantx57,		// Can device 57 output re-transmit timer
 tgrcantx58,		// Can device 58 output re-transmit timer
 tgrcantx59, 		// Can device 59 output re-transmit timer
 tgrcantx60, 		// Can device 60 output re-transmit timer
 tgrcanrx1,			// Can device 1 input timer
 tgrcanrx2,			// Can device 2 input timer
 tgrcanrx3,			// Can device 3 input timer
 tgrcanrx4,			// Can device 4 input timer
 tgrcanrx5,			// Can device 5 input timer
 tgrcanrx6,			// Can device 6 input timer
 tgrcanrx7,			// Can device 7 input timer
 tgrcanrx8,			// Can device 8 input timer
 tgrcanrx9,   		// Can device 9 input timer
 tgrcanrx10,		// Can device 10 input timer
 tgrcanrx11,		// Can device 11 input timer
 tgrcanrx12,		// Can device 12 input timer
 tgrcanrx13,		// Can device 13 input timer
 tgrcanrx14,		// Can device 14 input timer
 tgrcanrx15,		// Can device 15 input timer
 tgrcanrx16,		// Can device 16 input timer
 tgrcanrx17,		// Can device 17 input timer
 tgrcanrx18,  		// Can device 18 input timer
 tgrcanrx19,		// Can device 19 input timer
 tgrcanrx20, 		// Can device 20 input timer
 tgrcanrx21, 		// Can device 21 input timer
 tgrcanrx22, 		// Can device 22 input timer
 tgrcanrx23,		// Can device 23 input timer
 tgrcanrx24,		// Can device 24 input timer
 tgrcanrx25,		// Can device 25 input timer
 tgrcanrx26,		// Can device 26 input timer
 tgrcanrx27,		// Can device 27 input timer
 tgrcanrx28,  		// Can device 28 input timer
 tgrcanrx29,		// Can device 29 input timer
 tgrcanrx30, 		// Can device 30 input timer
 tgrcanrx31, 		// Can device 31 input timer
 tgrcanrx32, 		// Can device 32 input timer
 tgrcanrx33,		// Can device 33 input timer
 tgrcanrx34,		// Can device 34 input timer
 tgrcanrx35,		// Can device 35 input timer
 tgrcanrx36,		// Can device 36 input timer
 tgrcanrx37,		// Can device 37 input timer
 tgrcanrx38,  		// Can device 38 input timer
 tgrcanrx39,		// Can device 39 input timer
 tgrcanrx40, 		// Can device 40 input timer
 tgrcanrx41, 		// Can device 41 input timer
 tgrcanrx42, 		// Can device 42 input timer
 tgrcanrx43,		// Can device 43 input timer
 tgrcanrx44,		// Can device 44 input timer
 tgrcanrx45,		// Can device 45 input timer
 tgrcanrx46,		// Can device 46 input timer
 tgrcanrx47,		// Can device 47 input timer
 tgrcanrx48,  		// Can device 48 input timer
 tgrcanrx49,		// Can device 49 input timer
 tgrcanrx50, 		// Can device 50 input timer
 tgrcanrx51, 		// Can device 51 input timer
 tgrcanrx52, 		// Can device 52 input timer
 tgrcanrx53,		// Can device 53 input timer
 tgrcanrx54,		// Can device 54 input timer
 tgrcanrx55,		// Can device 55 input timer
 tgrcanrx56,		// Can device 56 input timer
 tgrcanrx57,		// Can device 57 input timer
 tgrcanrx58,  		// Can device 58 input timer
 tgrcanrx59,		// Can device 59 input timer
 tgrcanrx60, 		// Can device 60 input timer
 tinitselcnt,		// init selector count via spb
 tfs1_grp,			// fire service debounce from group
 tindrcl, 			// independent recall back to lobby timer
 tenc_busy,			// encoder busy delay
 tspb_ui,			// spb user interface timer
 tgempf,	 		// group emergency power debounce
 tntsupd,			// NTS board comm update timer
 tnts_ui,			// NTS user interface timers
 tfept,				// Fire/Emergency Power timer
 tgrcan_master, 	// group can com master
 tgrcan_master_req, 	// group can comm master select request timer
 tgrcan_car1,  		// group comunication timer for car 1
 tgrcan_car2,  		// group comunication timer for car 2
 tgrcan_car3, 		// group comunication timer for car 3
 tgrcan_car4,  		// group comunication timer for car 4
 tgrcan_car5,  		// group comunication timer for car 5
 tgrcan_car6,  		// group comunication timer for car 6
 tgrcan_car7,  		// group comunication timer for car 7
 tgrcan_car8,  		// group comunication timer for car 8
 trx_DL20,			// receve timer for DL20 display
 ttphdbnc,			// timer to debounce input for tph
 thighprioritytm,	// High Priority Timer
 theofautorst,		// HEOF Auto Reset Timer
 tlimitsetup,		// Limit setup timer
 tspbcmdupd,		// spb command update timer
 tDZsel,				// DZsn timer
 tbktest,			// Brake Test timer
 tpicc_ctcan,		// PI CC ctcan timer
 tpicc_mrcan,		// PI CC mrcan timer
 tsecdis,			// security disable timer
 teeftest,			// peelle Electric Eye Front Test Timer
 teertest,			// peelle Electric Eye Rear Test Timer
 tceoff,			// CEOF switch debounce timer
 tpbdoort,			// Push button doors, time to wait at floor before moving
 tpwrdwel,			// Power door dwell timer
 tpwrdwelr,			// Power rear door dwell timer
 tcom1_pkreq,		// Auto Packet request time-out com1
 tcom2_pkreq,		// Auto Packet request time-out com2
 theofovratt,		// Hall Elevator Off over att delay
 tselcanrx,			// Selector can receive timer
 tccpbsgrptmr,		// CCPBS timer to clear codes and fl sending on grp comm
 taps_ui,			// APS selector user interface data time-out timer
 tautowifi,			// Timer for auto sending commands to set up the wifi card
 
   
 //GALX_1132_TEST_TIMER
 tSDDelay,          //27 SD  delay
 tRTCDelay,			//21 RTC delay
 t_Generic_Delay,
 t_Generic_Delay_2,
 t_Generic_Delay_3,
 t_Generic_Delay_4,
 t_Functional_Test_Delay,
 tRefreshInp,
 trsCOM1Delay,
 tGR_CAN_Tx,
 tGR_CAN_Rx,
 tCanB_Tx,          //10 Can B Tx
 tCanB_Rx,          //11 Can B Rx
 
 tnmb				// Must be last
 };

extern int16 timers[tnmb];
// extern int16 new_timers[tnmb];

extern uint16 tmr_10_msec;


extern int16 tuser_com;	// User interface comm timer
extern int16 tcom_recv;	// User interface comm timer
extern int16 tcom_tran;	// User interface comm timer
extern int16 tcom_pkreq;// User interface auot packete request time-out comm timer
extern int16 tgcom_car; // group comunication timer for car 1
extern int16 tgrcan_car; // group can comunication timer for car 1
extern int16 tepcar;	// Recall time-out timer for Emergency Power Return Car
extern int16 tcantx;	// Can device 1 output re-transmit timer
extern int16 tcanrx;	// Can device 1 input timer
extern int16 temhc;		// Emergency dispatch hall call timer 1
extern int16 temrhc; 	// Emergency dispatch rear hall call timer 1
extern int16 teprcvr;	// Recover time-out timer for Emergency Power Return Car
extern int16 tmrcantx;	// Can device 1 output re-transmit timer
extern int16 tmrcanrx;	// Can device 1 input timer
extern int16 tgrcantx;	// Can device 1 output re-transmit timer
extern int16 tgrcanrx;	// Can device 1 input timer

extern trace_tmr;

enum {
 ts_sec,			// General Purpose second timer
 ts_pwd,			// Password timer
 ts_datechk,		// Date chk sec timer
 ts_crtcsync,		// Car sync rtc with dispatcher fail timer
 ts_nmb				// Must be last
};

extern int16 timer_sec[ts_nmb];

extern int16 loop_cnt;
extern int16 loop_cnt_array[10];
extern int16 loop_cnt_ptr;
extern int16 avg_loop_cnt;

// Variables for limit test

extern int16 tmr_lim_dir;			// Timer for limit direction fault
extern int16 tmr_vel_diff;	// Timer for velocity difference fault
extern int16 tmr_spb_enc_dir; 	// Timer for SPB encoder direction fault
extern int16 tmr_uldl_dir;		// Timer for UL/DL direction fault check (prevent fault on power cycle)

#if (Traction == 1) || (Tract_OL == 1)

extern int16 limit_0;
extern int16 limit_0_tog;
extern int16 limit_1;
extern int16 limit_1_tog;
extern int16 limit_2;
extern int16 limit_2_tog;
extern int16 limit_3;
extern int16 limit_3_tog;
extern int16 limit_4;
extern int16 limit_4_tog;
extern int16 limit_5;
extern int16 limit_5_tog;
extern int16 limit_6;
extern int16 limit_6_tog;
extern int16 limit_ts;
extern int16 limit_ts_tog;

#endif

extern int16 uldl_seq;		// ul/dl sequence variable
extern int16 dlul_seq;	// dl/ul sequence variable

// hour timer
extern int16 rt_day;
extern int16 rt_hour;
extern int16 rt_minute;
extern int16 rt_second;

// Messages sent out to the user
extern unsigned char message[2][15];
// Flags used to direct the program
extern int16 codebf;      	// code blue flag
extern int16 ems_onecall;	//Allow  one call in hospital service after code blue recall
extern int16 in_code_blue; 	// set when in code blue
extern int16 ind_codeb_alarm;	// set to alarm ind car that code blue requested 
extern int16 cb_over_ind;		// Set CB over independent
extern int16 heof_over_ind;	// Set HEOF over IND
extern int16 heof_over_att;	// Set HEOF over ATT
extern int16 hsvf;			// hospital service flag
extern int16 vipf;    		// priority service (vip) flag
extern int16 in_vip;			// set when in vip service
extern int16 tugf; 			// TUG robot interface flag
extern int16 in_overload;		// set when car on overload
extern int16 in_att;			// set when car in attendant service
extern int16 dcalls;      	// direction of calls
extern int16 onward_calls;	// onward calls
extern int16 onward_cc;		// onward car calls
extern int16 onward_hc;		// onward hall calls
extern int16 nohcreopen;		// no hall call reopen
extern int16 top_cc;			// top most car call
extern int16 bot_cc;			// bottom most car call
extern int16 nmb_cc;			// number of car calls at different floors
extern int16 dirf;       	 	// direction flag
extern int16 prev_dirf;		// previous direction of travel
extern int16 doorf;       	// door flag
extern int16 swdorf;		// hall swing door flag
extern int16 rswdorf;		// rear hall swing door flag
extern int16 ins_doorf;		
extern int16 fdoor_en;		// front door enable
extern int16 fdoor_req;		// front door request to open
extern int16 fdob_req;		// front door open button request
extern int16 fcc_req;			// front car call door open request
extern int16 nudgst;			// front door nudging status
extern int16 do_seq;
extern int16 do_to;			// Set when door open limit times out.
extern int16 ee_to;			// Set when electric eye times out.
extern int16 manual_door;		// set for manual door (added to have manual rear staggered and automatic front)
extern int16 freight_fdoor_alarm;		// frieght front door alarm
extern int16 fsdo;			// Front swing door open
extern int16 fsdo_seq;		// front swing door open sequence
extern int16 dpref;       	// direction preference
extern int16 cc_above_lby;	// set to 1 if car calls above the lobby 
extern int16 cc_below_lby;	// set to 1 if car calls below the lobby  
extern int16 run_no_pass_cnt;	// Run with no passenger entering the car
extern int16 pass_det;			// Passenger detected in the car
extern int16 extended_door_time;	// set when extended door time is active
extern int16 rm_extd_doort;		// set when remote extended door time is active
extern int16 rcm_frght_out_en;				// Retiring Cam for freight output enable

extern int16 eqf;				// Earthquake flag
extern int16 old_eqf;			// Previous earthquake flag
extern int16 above_cwt;		// 1 = car is above the counterweight
extern int16 old_above_cwt;
extern int16 empf;        	// emergency power car flag
extern int16 in_emp_trying_to_reset;


extern int16 drv_has_power;				// used for emergency power lowering, 1 = normal power, 2 = emergency power

// Line voltage variables

extern int16 L1_Voltage;
extern int16 L2_Voltage;
extern int16 L3_Voltage;
extern int16 Door_Voltage;

extern int16 voltf;				// voltage status flag 
								// bit 0: L1
								// bit 1: L2
								// bit 2: L3
								// bit 4: Door


// Hugs Security Variables

extern int16 hugsf;	// hugsf = 0 no hugs security
					// hugsf = 1 front door security
					// hugsf = 2 rear door security
					// hugsf = 3 front and rear door security


extern int16 adv_pre_trq_run;		// Advanced pre-torque run enable flag
extern int16 adv_pre_trq_fault;	// Advanced pre-torque run fault flag

extern int16 run_time;			// Floor to floor run time from tmotion timer.
extern int16 pre_torque_time;	// Time to pretorque the motor to prepare to tun
extern int16 targetfl_time;		// Target floor tmotion time value when first entered
extern int16 preopen_time;		// Time car enables preopoening door on tmotion timer
extern int16 ep_recall_fl;		// emergency power recall floor set from setempf routine
extern int16 estop;		        // emergency stop
extern int16 estops_1_run;		// estops counter
extern int16 fault_run;			// fault during run
extern int16 fault_run_cnt;		// number of runs with faults
extern int16 em_sd_run;			// emergency slowdown during run
extern int16 shutdown;			// shutdown car due to too many runs with faults in a row
extern int16 pwl_ups_fault;		// power loss ups fault (shuts car down)
extern int16 limit_dir_fault;	// Fault from hitting limits from wrong direction
extern int16 spb_dir_fault;		// SPB direction opposite from car direction
extern int16 enc_dir_fault;		// SPB direction opposite from car direction
extern int16 uldl_dir_fault;	// UL and DL Direction fault 
extern int16 vel_decel_diff_fault;	// Velocity decel tracking fault
extern int16 spb_vel_diff_fault;		// SPB velocity difference fault
extern int16 spb_vel_diff;			// SPB velocity difference greater than parameter value
extern int16 lev_to;			// leveling time-out while targeting floor
extern int16 prev_fault;
extern int16 Current_Fault;    	// The Current Fault
extern int16 Display_Fault;
extern int16 Rotate_Fault;
extern int16 BRK_Fault;			// Brake lift switch fault
extern int16 BKS_Run_Fault;		// Brake lift switch fault during run
extern int16 BRK_Pick_Test;		// Brake lift switch Pick Test flag
extern int16 BRK_Drop_Test;		// Brake lift switch Drop Test flag

extern uint8 Door_Lock_GS_DPM_Fault;	// single boolean to identify any Door Lock, GS or DPM fault

extern uint8 Door_Lock_fault;			// Door lock fault if dli on on when doors open
extern uint8 Door_Lock_front_fault;		// Door lock fault if dli on on when doors open - front
extern uint8 Door_Lock_rear_fault;		// Door lock fault if dli on on when doors open - rear

extern uint8 Door_GS_fault;				// Door GS fault if gs is on when door are open
extern uint8 Door_GS_front_fault;		// Door GS fault if gs is on when door are open - front
extern uint8 Door_GS_rear_fault;			// Door GS fault if gs is on when door are open - rear

extern uint8 Door_DPM_fault;			// Door DPM fault if dpm is on when door are open
extern uint8 Door_DPM_front_fault;		// Door DPM fault if dpm is on when door are open - front
extern uint8 Door_DPM_rear_fault;		// Door DPM fault if dpm is on when door are open - rear

extern uint8 Door_DPM_seq_fault;		//  Door DPM fault if dpm sequence failed
extern uint8 Door_DPM_seq_front_fault;	//  Door DPM fault if dpm sequence failed - front
extern uint8 Door_DPM_seq_rear_fault;	//  Door DPM fault if dpm sequence failed - rear

extern int16 DZ_ON_Fault;
extern int16 DZA_ON_Fault;
extern int16 DZ_OFF_Fault;
extern int16 DZA_OFF_Fault;
extern int16 DZsn_OFF_Fault;		// Door zone sn (selector) off fault
extern int16 UL_ON_Fault;			// Detect if UL does not turn off during motion
extern int16 DL_ON_Fault;			// Detect if DL does not turn off during motion
extern int16 UL_DL_OFF_Fault;		// UL or DL did not turn on at level zone
extern int16 io_fault;			// Io device fault
extern int16 io_fault_cnt;		// Lock up after 3 io faults
extern int16 limit_fault;			// UT,UTS,DT or DTS limit fault
extern int16 ins_fault;			// inspection fault flag
extern int16 ins_fault_latch;		// inspection fault latch
extern int16 ins_sw_err;			// inspection switch error (up or down run)
extern int16 ins_status;			// inspection status
extern int16 byp_fault;			// gate or lock bypass fault
extern int16 byp_fault_latch;		// gate or lock bypass fault latch
extern int16 gripper_test;		// State flag for gripper test
extern int16 in_gripper_test;		// In gripper test routine
extern int16 gripper_fault;		// Rope gripper fault
extern int16 gripper_trip_enable; 	// gripper trip enable flag
extern int16 DBR_Temp_Fault;		// Dynamic Braking Temperature Fault
extern int16 fault_device;		// device that had the fault
extern int16 fault_device_2;		// device 2 that had the fault
extern int16 fault_prog_flag1;	// programmer multi-use flag 1
extern int16 fault_prog_flag2;	// programmer multi-use flag 2
extern int16 motion_fault;		// Motion fault occurred: must stop car
extern int16 redundancy_test;			// redundancy relay test
extern int16 in_redundancy_test;			// in redundancy relay test
extern int16 reset_courion;
extern int16 in_serv_lt;			// in service light control for single automatic pushbutton operation
extern int16 oserl_enable;		// Special Out of Service Light enable variable
extern int16 oser_lt;				// Out of service light
extern int16 oser_bz;				// Out of service buzzer
extern int16 oser_bz_pulse;		// Out of service buzzer flash
extern int16 hardware_init_fault;	// 1 if hardware failed on init
extern int16 drv_rst_cnt;			// Counter for drive ready reset
extern int16 rdy_flt_latch;		// fault latch for drive ready
extern bool ResetFaults;
extern int16 cen_fault;		// CEN input fault
extern int16 door_motor_ovl;		// Door motor overload flag
extern int16 safe_fault_latch;	// safe fault latch
extern int16 eef2_enabled;			// set when secondary electric eye input is enabled
extern int16 eer2_enabled;			// set when secondary rear electric eye input is enabled 
extern int16 fado_fault_cnt;		// front auto swing hall door open fault count
extern int16 rado_fault_cnt;		// rear auto swing hall door open fault count

extern int16 car_has_fault;

typedef struct Fault {
	uint16 code;			// 0,1
	uint8 count;			// 2
	uint8 day;				// 3
	uint8 mon;				// 4
	int16 year;				// 5,6
	uint8 min;				// 7
	uint8 hour;				// 8
	uint8 sec;				// 9
	uint8 pos;				// 10
	uint8 servf;			// 11	  
	uint8 procf;			// 12	  
	uint8 doorf;			// 13
	uint8 rdoorf;			// 14
	uint8 dpref;			// 15
	uint8 dirf;				// 16
	uint8 empf;				// 17
	uint8 medf;				// 18
	uint8 codebf;			// 19
	uint8 eqf;				// 20
	uint8 firef;			// 21	   
	uint8 rfiref;			// 22
	uint8 hsf;				// 23
	uint8 startf;		   	// 24
	uint8 dcalls;   		// 25
	uint8 estop;			// 26
	uint8 nstopf;			// 27
	uint8 relevel;			// 28
	uint8 stepf;			// 29
	uint8 door_seqf;		// 30
	uint8 next_stop;		// 31   
	uint8 ins_status;		// 32  
	uint8 ncu_door_seq;		// 33
	uint8 device;			// 34
	uint8 prog_flag1;		// 35
	uint8 prog_flag2;		// 36
	uint8 device_2;			// 37
	uint8 iodata [30];		// 38-67
	int32 statusf;			// 68-71
	int32 dpp_count;	 	// 72-75
	int32 sd_count;		// 76-79
	int16 vel_fpm;	 		// 80-81
	int16 dmd_vel_fpm;		// 82-83
	int16 drv_vel_fpm;		// 84-85
	int16 enc_vel_fpm;		// 86-87
	
	uint8 NTS_status1;		// 88
	uint8 NTS_status2;		// 89
	int16 NTS_vel;			// 90-91
	int32 NTS_poscnt;		// 92-95
	uint8 NTS_command1;		// 96
	uint8 NTS_command2;		// 97
	uint8 NTS_doorzone;		// 98
	uint8 NTS_limits;		// 99
	uint8 NTS_vel_dir;		// 100
	uint8 unused1;			// 101
	uint8 unused2;			// 102

	uint8 fault_bits[4];	// 103-106
	int32 statusf2;			// 107-110
	int16 ss_status;		// 111-112
	int16 power_status;		// 113-114
	
	int32 fl_dpp_count;		// 115-118
	uint8 unused7;			// 119
	uint8 unused8;			// 120
	uint8 unused9;			// 121
	uint8 unused10;			// 122
	uint8 unused11;			// 123
	uint8 unused12;			// 124
	uint8 unused13;			// 125
	
	uint32 run_statusf;		// 126-129
 	uint8 nudgst_flags;		// 130		Nudging_Flags = rnudgst, nudgst
	uint8 door_req_flags;	// 131 		Door Request = rdoor_en, rcc_req, rdob_req, rdoor_req, fdoor_en, fcc_req, fdob_req, fdoor_req
	uint8 call_flags;		// 132 		Call_Flags = onward_hc, onward_cc, onward_calls, dcalls 
	uint8 chkrun_st;		// 133
	uint8 chkstart_st;		// 134
	uint8 chklevel_st;		// 135
	uint8 chkdoor_st;		// 136
	uint16 front_sd;		// 137-138
	uint16 rear_sd;			// 139-140
	uint16 motion_tmr;		// 141-142
	int16 PAL_vel;			// 143-144
	uint8 PAL_stat;			// 145
	uint8 ins_servf; 		// 146
	uint8 enc_dir;			// 147
	uint16 drv_cmd;			// 148-149
	uint16 drv_status;		// 150-151
	uint16 drv_mtr_torque;	// 152-153
	uint16 drv_torque_cmd;	// 154-155
	uint16 percent_load;	// 156-157
	uint8 lev_startf;		// 158
	int32 statusf3;			// 159-162
	int32 statusf4;			// 163-166
	uint8 spare;			// 167		even number
} Fault_type;				// 168 * 50 = 8400 bytes

#define max_flt_ix 50
extern union fault_union {
	unsigned char fltb[max_flt_ix][sizeof(Fault_type)];
	uint16 fltw[max_flt_ix][sizeof(Fault_type)/2];
	struct Fault Flts[max_flt_ix];
}f;

extern union car_fault_union {
	unsigned char fltb[max_flt_ix][sizeof(Fault_type)];
	uint16 fltw[max_flt_ix][sizeof(Fault_type)/2];
	struct Fault Flts[max_flt_ix];
}cf;

extern int16 Flt_ix;
extern int16 Car_Flt_ix;

#define f_reset 1						// 001F - Reset failure
#define f_pos_error 2					// 002F - Elevator lost failure (position error)
#define f_binary 3						// 003F - Binary floor input failure
#define f_sstring 4						// 004F - Safety string failure or no P
#define f_dz_on 5						// 005F - Door zone on failure
#define f_stall_fault 6					// 006F - Stall failure
#define f_door_open 7					// 007F - Door open failure
#define f_door_close 8					// 008F - Door close failure
#define f_un_open 9						// 009F - Up directional open
#define f_dn_open 10					// 010F - Down directional open
#define f_hw_learn 11					// 011F - Hoistway not Learned
#define f_stop_sw 12					// 012F - Stop switch open
#define f_dpm_on 13						// 013F - Door lock Protection (DPM) on fault
#define f_S10_fuse 14					// 014F - LC fuse Blown
#define f_HC_fuse 15					// 015F - HC fuse Blown
#define f_drive_rdy 16	 				// 016F - Drive Ready off
#define f_P_off	17						// 017F - Potential input off
#define f_car_safe 18	  				// 018F - Car Safe Fault
#define f_ul_dl_off 19					// 019F - UL and DL off when expected on
#define f_delta_off 20	 				// 020F - Delta off when expected on
#define f_UT_count 21					// 021F - Up Terminal limit count error
#define f_UT1_count 22					// 022F - Up Terminal 1 limit count error
#define f_UT2_count 23					// 023F - Up Terminal 2 limit count error
#define f_UT3_count 24					// 024F - Up Terminal 3 limit count error
#define f_DT_count 25					// 025F - Down Terminal limit count error
#define f_DT1_count 26					// 026F - Down Terminal 1 limit count error
#define f_DT2_count 27					// 027F - Down Terminal 2 limit count error
#define f_DT3_count 28					// 028F - Down Terminal 3 limit count error
#define f_rdoor_open 29					// 029F - Door open failure
#define f_rdoor_close 30				// 030F - Door close failure
#define f_Group_Comm 31					// 031F - Group Comm loss 
#define f_grp_Car_Statm 31				// 031F - Car Comm loss  (Start of index + Car number)
#define f_Car1_Comm 32					// 032F - Car 1 Comm Loss (Cannot have a Car 1 comm loss)
#define f_Car2_Comm 33					// 033F - Car 2 Comm Loss
#define f_Car3_Comm 34					// 034F - Car 3 Comm Loss
#define f_Car4_Comm 35					// 035F - Car 4 Comm Loss
#define f_Car5_Comm 36					// 036F - Car 5 Comm Loss
#define f_Car6_Comm 37					// 037F - Car 6 Comm Loss
#define f_run_on 38						// 038F - RUN input or output failed on
#define f_runo_runai 39					// 039F - Run output or runa input failed
#define f_run_off 40					// 040F - RUN input or output failed off 
#define f_runa_on 41					// 041F - RUNA input or output failed on
#define f_runa_off 42 					// 042F - RUNA input or output failed off
#define f_up_on 43						// 043F - UP input or output failed on
#define f_up_off 44						// 044F - UP input or output failed off
#define f_dnr_on 45						// 045F - DNR input or output failed on
#define f_dnr_off 46					// 046F - DNR input or output failed off
#define f_upf_on 47						// 047F - UP Fast input or output failed on
#define f_upf_off 48	   				// 048F - UP Fast input or output failed off
#define f_df_on 49						// 049F - DN Fast input or output failed on
#define f_df_off 50						// 050F - DN Fast input or output failed off
#define f_MCC_on 51						// 051F - MCC input or output failed on
#define f_runo_runi 52					// 052F - Run output or run input failed
#define f_MCC_off 53					// 053F - MCC input or output failed off 
#define f_MCA_on 54						// 054F - MC input or output failed on
#define f_MCA_off 55					// 055F - MC input or output failed off
#define f_brk_on 56						// 056F - BRK input or output failed on
#define f_brk_off 57 					// 057F - BRK input or output failed off
#define f_don_on 58						// 058F - DON input failed on
#define f_don_off 59 					// 059F - DON input failed off
#define f_run_runa_up 60 				// 060F - RUN input/output or UP output failed off
#define f_run_runa_dn 61 				// 061F - RUN input/output or DNR output failed off
#define f_dlt_on 62						// 062F - Door Lock Top failed on
#define f_dlm_on 63						// 063F - Door Lock Middle failed on
#define f_dlb_on 64						// 064F - Door Lock Bottom failed on
#define f_gs_on	 65						// 065F - Gate Switch failed on	
#define f_rdlt_on 66					// 066F - Rear Door Lock Top failed on
#define f_rlm_on 67						// 067F - Rear Door Lock Middle failed on
#define f_rdlb_on 68					// 068F - Rear Door Lock Bottom failed on
#define f_rgs_on 69						// 069F - Rear Gate Switch failed on
#define f_P_on 70						// 070F - Potential input failed on
#define f_estop 71 						// 071F - Three estops in a row
#define f_ins_err 72					// 072F - Inspection input error
#define f_byp_err 73					// 073F - Gate or lock bypass error
#define f_grt1_on 74					// 074F - Gripper input 1 failed on
#define f_grt1_off 75					// 075F - Gripper input 1 failed off
#define f_grt2_on 76					// 076F - Gripper input 2 failed on
#define f_grt2_off 77					// 077F - Gripper input 2 failed off
#define f_gts_pick 78					// 078F - Gripper did not pick
#define f_grip_trip 79					// 079F - Gripper tripped from an un-safe condition
#define f_dlt_fail 80					// 080F - Door Lock Top DLT and DLT_1 failed opposite
#define f_dlm_fail 81					// 081F - Door Lock Middle DLM and DLM_1 failed opposite
#define f_dlb_fail 82					// 082F - Door Lock Bottom DLB and DLB_1 failed opposite
#define f_gs_fail 83					// 083F - Gate Switch GS and GS_1 failed opposite	
#define f_rlm_fail 84					// 084F - Rear Door Lock Middle RLM and RLM_1 failed opposite
#define f_rgs_fail 85					// 085F - Rear Gate Switch RGS and RGS_1 failed opposite
#define f_brk_pick 86					// 086F - Brake failed to pick 
#define f_brk_drop 87					// 087F - Brake failed to drop
#define f_grip_set 88					// 088F - Gripper set from brake failure
#define f_delta_on 89					// 089F - DEL input failed on when should be off
#define f_cws_dt 90						// 090F - Above cwt flag set incorrectly at DT limit
#define f_cws_ut 91						// 091F - Above cwt flag set incorrectly at UT limit
#define f_UT_Target 92					// 092F - Target error at UT slowdown limit
#define f_UT1_Target 93					// 093F - Target error at UT1 slowdown limit
#define f_UT2_Target 94					// 094F - Target error at UT2 slowdown limit
#define f_UT3_Target 95					// 095F - Target error at UT3 slowdown limit
#define f_DT_Target 96					// 096F - Target error at DT slowdown limit
#define f_DT1_Target 97					// 097F - Target error at DT1 slowdown limit
#define f_DT2_Target 98					// 098F - Target error at DT2 slowdown limit
#define f_DT3_Target 99					// 099F - Target error at DT3 slowdown limit
#define f_vel_overspeed 100				// 100F - Velocity Over speed
#define f_hc_dev_reset 101 				// 101F - Hall call device reset
#define f_car_dev_reset 102 			// 102F - Car comm device reset
#define f_toc_can_error 103 			// 103F - Top car device can comm error
#define f_drv_rcv_error 104 			// 104F - Drive comm receive error 
#define f_pwrup_reset 105				// 105F - Power Up Reset
#define f_DRV_SP_flt 106				// 106F - Drive or Safety Processor Fault
#define f_car_safe_preop 107	  		// 107F - Car Safe Fault during preopening
#define f_car_safe_start 108	  		// 108F - Car Safe Fault during start (door possibly not closed)
#define f_ins_up_dn_sw 109				// 109F - Up or down inspections switch error
#define f_drv_trm_error 110				// 110F - Drive Comm transmit error
#define f_drv_inten_error 111 			// 111F - Drive Comm interrupt enable error
#define f_drv_txempty_error 112 		// 112F - Drive comm transmit empty error
#define f_UTS_count 113					// 113F - Up ETS Terminal limit count error
#define f_DTS_count 114					// 114F - Down ETS Terminal limit count error
#define f_UTS_Target 115				// 115F - Target error at UTS slowdown limit
#define f_DTS_Target 116				// 116F - Target error at DTS slowdown limit
#define f_drv_8259_error 117 			// 117F - Drive comm 8256 error
#define f_drv_tx_stop_error 118			// 118F - Drive transmitter stopped error
#define f_spb_can_error 119				// 119F - Safety Processor board can communications error
#define f_dbr_temp_flt 120				// 120F - Dynamic Brake Resistors are too hot fault
#define f_seb_can_error 121				// 121F - Serial Expansion Board Can comm error
#define f_lw_calibrate 122				// 122F - Load weigher calibration error
#define f_can_init_error 123			// 123F - Can bus initialization error (bus or timer failure)
#define f_shutdown 124					// 124F - Too many consecutive runs with faults
#define f_drv_com_error 125				// 125F - Drive has a comm error
#define f_encoder 126					// 126F - Encoder Fault
#define f_can_bus_off 127				// 127F - Can Bus Off error
#define f_zero_vel_error 128			// 128F - Zero Velocity Error during roll into decel
#define f_bus_error 129					// 129F - bus error during encoder read
#define f_lowpress 130					// 130F - low pressure switch error
#define f_UTS_flt 131					// 131F - UTS failed on
#define f_DTS_flt 132					// 132F - DTS failed on
#define f_UT_flt 133					// 133F - UT failed on
#define f_DT_flt 134					// 134F - DT failed on
#define f_rpm_on 135					// 135F - Rear Door lock Protection (RPM) on fault
#define f_dpm_gl 136					// 136F - DPM off with gate or lock on
#define f_rpm_gl 137					// 137F - RPM off with rear gate or lock on 
#define f_pulse_75fpm_2in 138			// 138F - Pulse count error, car > 75 fpm travel < 2in
#define f_pulse_dir_up 139				// 139F - Pulse count wrong direction running up
#define f_pulse_dir_dn 140				// 140F - Pulse count wrong direction running down
#define f_pulse_75speed	141				// 141F - Pulse count error, 75% top speed not enough counts
#define f_brk_can_error 142				// 142F - Brake board can error
#define f_gov 143						// 143F - Governor switch
#define f_top_final 144					// 144F - Top final limit
#define f_bottom_final 145				// 145F - Bottom final limit
#define f_pit_sw 146					// 146F - Pit switch
#define f_pit_door_sw 147				// 147F - Pit door switch
#define f_buffer_sw 148					// 148F - Buffer switch
#define f_comp_sw 149					// 149F - Comp rope switch
#define f_top_em_exit 150				// 150F - Top emergency exit switch
#define f_side_em_exit 151				// 151F - Side Emergency exit switch
#define f_car_safety_sw 152				// 152F - Car safety switch
#define f_CC_fuse 153					// 153F - Car call common fuse
#define f_CCL_fuse 154					// 154F - Car call light common fuse
#define f_HCL_fuse 155					// 155F - Hall call light common fuse
#define f_FF_stop_sw 156				// 156F - Fire Fighters Stop Switch
#define f_PFC_failed_on 157				// 157F - PFC relay failed on 
#define f_PFC_failed_off 158			// 158F - PFC relay failed off
#define f_SFC_failed_on 159				// 159F - SFC relay failed on
#define f_SFC_failed_off 160			// 160F - SFC relay failed off
#define f_dza_on 161 					// 161F - Door zone A on fault
#define f_dz_off 162					// 162F - Door zone off fault
#define f_dza_off 163					// 163F - Door zone A off fault
#define f_PFC_SFC_DZ_lost 164			// 164F - Lost DZ during PFC SFC test
#define f_FST_failed_on 165				// 165F - FST output failed on or FSTI input failed on
#define f_FST_failed_off 166 			// 166F - FST output failed off or FSTI input failed off
#define f_FSTP_failed_on 167			// 167F - FSTP output failed on
#define f_FSTP_failed_off 168 			// 168F - FSTP output failed off
#define f_sel_can_error 169				// 169F - Selector board can error
#define f_runa_runi 170					// 170F - Runa or runi failed
#define f_eexit 171						// 171F - Emergency exit
#define f_ul_on 172						// 172F - UL failed on
#define f_dl_on 173						// 173F - DL failed on
#define f_ul_dl_dz_off 174				// 174F - UL DL and DZ off at floor
#define f_UT4_count 175					// 175F - Up Terminal 4 limit count error
#define f_UT5_count 176					// 176F - Up Terminal 5 limit count error
#define f_UT6_count 177					// 177F - Up Terminal 6 limit count error
#define f_DT4_count 178					// 178F - Down Terminal 4 limit count error
#define f_DT5_count 179					// 179F - Down Terminal 5 limit count error
#define f_DT6_count 180					// 180F - Down Terminal 6 limit count error
#define f_UT4_Target 181				// 181F - Target error at UT4 slowdown limit
#define f_UT5_Target 182				// 182F - Target error at UT5 slowdown limit
#define f_UT6_Target 183				// 183F - Target error at UT6 slowdown limit
#define f_DT4_Target 184 				// 184F - Target error at DT4 slowdown limit
#define f_DT5_Target 185 				// 185F - Target error at DT5 slowdown limit
#define f_DT6_Target 186 				// 186F - Target error at DT6 slowdown limit
#define f_fvars_reloc 187				// 187F - Field variables relocated
#define f_fvars_init 188				// 188F - Field variables initialized
#define f_hw_init 189					// 189F - Hoistway initialized
#define f_Car7_Comm 190					// 190F - Car 7 Comm Loss
#define f_Car8_Comm 191					// 191F - Car 8 Comm Loss
#define f_EE_fault 192					// 192F - EE Ram fault
#define f_NV_fault 193					// 193F - NV Ram Fault
#define f_hw_upd_init 194				// 194F - Hoistway update table initialized
#define f_lowoil_switch 195				// 195F - Low oil switch input fault
#define f_leveling_fault 196			// 196F - Leveling fault
#define f_SEL_Cnt_fault 197				// 197F - Selector Count fault
#define f_hw_inv_fl_cnt	198 			// 198F - Invalid floor count
#define f_hw_topf_cnt 199 				// 199F - Invalid top floor count
#define f_hw_dn_dt_cnt 200 				// 200F - Invalid DN or DT count
#define f_hw_dt_dt1_cnt	201 			// 201F - Invalid DT or DT1 count
#define f_hw_dt1_dt2_cnt 202			// 202F - Invalid DT1 or DT2 count
#define f_hw_dt2_dt3_cnt 203			// 203F - Invalid DT2 or DT3 count
#define f_hw_dt3_dt4_cnt 204			// 204F - Invalid DT3 or DT4 count
#define f_hw_dt4_dt5_cnt 205			// 205F - Invalid DT4 or DT5 count
#define f_hw_dt5_dt6_cnt 206			// 206F - Invalid DT5 or DT6 count
#define f_hw_ut6_ut5_cnt 207			// 207F - Invalid UT6 or UT5 count
#define f_hw_ut5_ut4_cnt 208			// 208F - Invalid UT5 or UT4 count
#define f_hw_ut4_ut3_cnt 209			// 209F - Invalid UT4 or UT3 count
#define f_hw_ut3_ut2_cnt 210			// 210F - Invalid UT3 or UT2 count
#define f_hw_ut2_ut1_cnt 211			// 211F - Invalid UT2 or UT1 count
#define f_hw_ut1_ut_cnt	212				// 212F - Invalid UT1 or UT  count
#define f_hw_ut_un_cnt 213				// 213F - Invalid UT or UN count
#define f_hw_sel_bot_cnt 214			// 214F - Invalid Selector Bottom Count
#define f_hw_sel_top_cnt 215			// 215F - Invalid Selector Top Count
#define f_hw_dzu_dzd_cnt 216			// 216F - Invalid DZU or DZD Count
#define f_hw_fl_off_cnt 217				// 217F - Invalid FL offset Count
#define f_asv_to_car 217				// 217F - Start of fault index for auto service time-out
#define f_asv_to_car_1 218				// 218F - Auto Service Time-out Car 1
#define f_asv_to_car_2 219				// 219F - Auto Service Time-out Car 2
#define f_asv_to_car_3 220				// 220F - Auto Service Time-out Car 3
#define f_asv_to_car_4 221				// 221F - Auto Service Time-out Car 4
#define f_asv_to_car_5 222				// 222F - Auto Service Time-out Car 5
#define f_asv_to_car_6 223				// 223F - Auto Service Time-out Car 6
#define f_asv_to_car_7 224				// 224F - Auto Service Time-out Car 7
#define f_asv_to_car_8 225				// 225F - Auto Service Time-out Car 8
#define f_bks_run 226					// 226F - Brake switch fail slowdown
#define f_levto 227						// 227F - Time-out leveling fault targeting floor
#define f_hw_safe_fault 228				// 228F - Auto learn hoistway safe fault
#define f_hw_stall_fault 229			// 229F - Auto learn hoistway stall fault
#define f_em_brk_off 230				// 230F - Emergency Brake failed off fault
#define f_em_brk_on 231					// 231F - Emergency Brake failed on fault
#define f_em_brk_sw_off 232				// 232F - Emergency Brake failed off fault
#define f_em_brk_sw_on 233				// 233F - Emergency Brake failed on fault
#define f_brk_over_current 234			// 234F - Brake board over current fault (brake error 2)
#define f_brk_over_volt 235				// 235F - Brake board over voltage fault (3)
#define f_brk_no_current 236			// 236F - Brake board no current fault with voltage applied (4)
#define f_brk_no_dc_bus_volt 237		// 237F - Brake board no dc bus voltage fault (5)
#define f_brk_no_output_volt 238		// 238F - Brake board not output voltage fault	(6)
#define f_brk_low_dc_bus_volt 239		// 239F - Brake board low dc bus voltage fault	 (7)
#define f_brk_dmd_gt_dc_bus 240			// 240F - Brake board dmd voltage greater than dc bus voltage fault (8)
#define f_com_config 241				// 241F - Group Comm configuration error (hall call i/o address configuration error)
#define f_hc_rx_top 242  				// 242F - Rx from top HC board fault	(nest 8 errors must be consecutive)
#define f_hc_tx_top 243					// 243F - Tx to top HC board fault
#define f_hc_too_few 244				// 244F - Too few stations on bus
#define f_hc_too_many 245				// 245F - Too many stations on bus
#define f_hc_rx_bot 246					// 246F - Rx from bottom HC board fault
#define f_hc_tx_bot 247					// 247F - Tx to bottom HC board fault
#define f_hc_reserved_248				// 248F - Reserved for hc board fault
#define f_hc_reserved_249				// 249F - Reserved for hc board fault	
#define f_lev_stop_cnt 250				// 250F - Leveling stop fault occured from incorrect count
#define f_GTS_failed_on 251				// 251F - GTS input failed on  
#define f_GTS_failed_off 252			// 252F - GTS input failed off
#define f_inv_fault 253					// 253F - Invalid fault code
#define f_spb_umotion 254				// 254F - SPB Unintended Motion
#define f_ids_last_error 255			// 255F - Largest error IDS can do
#define f_hcb_tx_dn 256					// 256F - HCB Tx fault down to below floor	  (next 14 errors must be consecutive)
#define f_hcb_tx_up 257					// 257F - HCB tx fault up to above floor
#define f_hcb_rx_dn	258					// 258F - HCB rx fault down from below floor
#define f_hcb_rx_up 259					// 259F - HCB rx fault up to above floor
#define f_hcb_stuck_dn 260				// 260F - HCB stuck button down
#define f_hcb_stuck_up 261				// 261F - HCB stuck button up
#define f_hcb_led_open_dn 262			// 262F - HCB led open down
#define f_hcb_led_open_up 263			// 263F - HCB led open up
#define f_hcb_led_short_dn 264			// 264F - HCB led short down
#define f_hcb_led_short_up 265			// 265F - HCB led short up
#define f_hcb_fet_open_dn 266			// 266F - HCB fet open down
#define f_hcb_fet_open_up 267			// 267F - HCB fet open up
#define f_hcb_fet_short_dn 268			// 268F - HCB fet short down
#define f_hcb_fet_short_up 269			// 269F - HCB fet short up
#define f_EBAi_on 270					// 270F - Emergency brake A1,A2 input failed on
#define f_EBAi_off 271					// 271F - Emergency brake A1,A2 input failed off
#define f_EBBi_on 272					// 272F - Emergency brake B1,B2 input failed on
#define f_EBBi_off 273					// 273F - Emergency brake B1,B2 input failed off
#define f_dl_fail 274					// 274F - DL and DL1 do not match
#define f_ul_fail 275					// 275F - UL and UL1 do not match
#define f_hcb_inv_fl 276				// 276F - HCB has invalid floor
#define f_hcb_no_com 277				// 277F - HCB no comm to individual hc board
#define f_hcb_dev_reset 278				// 278F - HCB device reset
#define f_hcb_dn_led_brd 279			// 279F - HCB No Down Led Board Detected
#define f_hcb_up_led_brd 280			// 280F - HCB No Up Led Board Detected
#define f_hcb_led_short_red_up	 281	// 281F - HCB led short red up
#define f_hcb_led_short_green_up 282	// 282F - HCB led short green up
#define f_hcb_led_short_blue_up	 283	// 283F - HCB led short blue up
#define f_hcb_led_short_blue_dn	 284	// 284F - HCB led short blue down
#define f_hcb_led_short_green_dn 285	// 285F - HCB led short green down
#define f_hcb_led_short_red_dn	 286	// 286F - HCB led short red down
#define f_hcb_led_open_red_up	 287	// 287F - HCB led short red up
#define f_hcb_led_open_green_up  288	// 288F - HCB led open green up
#define f_hcb_led_open_blue_up	 289	// 289F - HCB led open blue up
#define f_hcb_led_open_blue_dn	 290	// 290F - HCB led open blue down
#define f_hcb_led_open_green_dn  291	// 291F - HCB led open green down
#define f_hcb_led_open_red_dn	 292	// 292F - HCB led open red down
#define f_hcb_fet_short_red_up	 293	// 293F - HCB fet short red up
#define f_hcb_fet_short_green_up 294	// 294F - HCB fet short green up
#define f_hcb_fet_short_blue_up	 295	// 295F - HCB fet short blue up
#define f_hcb_fet_short_blue_dn	 296	// 296F - HCB fet short blue down
#define f_hcb_fet_short_green_dn 297	// 297F - HCB fet short green down
#define f_hcb_fet_short_red_dn	 298	// 298F - HCB fet short red down
#define f_hcb_fet_open_red_up	 299	// 299F - HCB fet short red up
#define f_hcb_fet_open_green_up  300	// 300F - HCB fet open green up
#define f_hcb_fet_open_blue_up	 301	// 301F - HCB fet open blue up
#define f_hcb_fet_open_blue_dn	 302	// 302F - HCB fet open blue down
#define f_hcb_fet_open_green_dn  303	// 303F - HCB fet open green down
#define f_hcb_fet_open_red_dn	 304	// 304F - HCB fet open red down
#define f_pwl_ups_low_bat		 305	// 305F - power loss ups battery fault
#define f_pwl_ups_on_bat_pwr	 306	// 306F - power loss ups on battery power
#define f_pwl_ups_turned_off	 307	// 307F - power loss ups turned off
#define f_pwl_ups_online		 308	// 308F - power loss ups online fault
#define f_pwl_ups_low_bat_volt	 309	// 309F - power loss ups battery fault
#define f_uldl_setup_count		 310	// 310F - UL,DL setup count fault
#define f_ep_to_car 310					// 310F - Start of fault index for Emergency Power Recall time-out
#define f_ep_to_car_1 311				// 311F - Emergency Power Recall Time-out Car 1
#define f_ep_to_car_2 312				// 312F - Emergency Power Recall Time-out Car 2
#define f_ep_to_car_3 313				// 313F - Emergency Power Recall Time-out Car 3
#define f_ep_to_car_4 314				// 314F - Emergency Power Recall Time-out Car 4
#define f_ep_to_car_5 315				// 315F - Emergency Power Recall Time-out Car 5
#define f_ep_to_car_6 316				// 316F - Emergency Power Recall Time-out Car 6
#define f_ep_to_car_7 317				// 317F - Emergency Power Recall Time-out Car 7
#define f_ep_to_car_8 318				// 318F - Emergency Power Recall Time-out Car 8
#define f_SPB_SFC_flt 319				// 319F - Safety Processor Board SFC fault
#define f_ep_ots_car 319				// 319F - Start of fault index for Emergency Power Recall Car Out of Service
#define f_ep_ots_car_1 320				// 320F - Emergency Power Recall Car Out of Service Car 1
#define f_ep_ots_car_2 321				// 321F - Emergency Power Recall Car Out of Service Car 2
#define f_ep_ots_car_3 322				// 322F - Emergency Power Recall Car Out of Service Car 3
#define f_ep_ots_car_4 323				// 323F - Emergency Power Recall Car Out of Service Car 4
#define f_ep_ots_car_5 324				// 324F - Emergency Power Recall Car Out of Service Car 5
#define f_ep_ots_car_6 325				// 325F - Emergency Power Recall Car Out of Service Car 6
#define f_ep_ots_car_7 326				// 326F - Emergency Power Recall Car Out of Service Car 7
#define f_ep_ots_car_8 327				// 327F - Emergency Power Recall Car Out of Service Car 8
#define f_dn_setup 328					// 328F - Down Normal must turn on before reaching bottom floor dead level
#define f_un_setup 329					// 329F - Up Normal must turn off after reaching the top floor dead level
#define f_advpt_start 330				// 330F - Did not get safe on advance pre-torque start
#define f_sel_preset 331				// 331F - Selector preset position fault
#define f_umotion_flt 332				// 332F - Unintended Motion fault
#define f_EMBK_trip 333					// 333F - Emergency Brake Trip fault
#define f_utm_gts_flt 334				// 334F - UTS contact/GTS input pick fault
#define f_EMBK_set 335					// 335F - Emergency brake set from brake fault
#define f_fdet_edg_to 336				// 336F - Front Detector Edge Time-out
#define f_rdet_edg_to 337				// 337F - Rear Detector Edge Time-out
#define f_L1_Voltage_Low 338			// 338F - L1 Line Voltage Low
#define f_L2_Voltage_Low 339			// 339F - L2 Line Voltage Low
#define f_L3_Voltage_Low 340			// 340F - L3 Line Voltage Low
#define f_Door_Voltage_Low 341			// 341F - Door Line Voltage Low
#define f_LHC_fuse 342					// 342F - Lobby Call common fuse
#define f_Door_Motor_OVL 343			// 343F - Door Motor Overload
#define f_Hoist_Motor_OVL 344			// 344F - Hoist Motor Overload
#define f_Car_Top_Stop_Sw 345			// 345F - Car top stop switch 
#define f_dl_safe_flt 346 				// 346F - Door lock safe fault
#define f_cg_safe_flt 347				// 347F - Car Gate safe fault
#define f_fdcc_safe_flt 348				// 348F - Door Close Contact safe fault
#define f_rdcc_safe_flt 349				// 349F - Rear Door Close Contact safe fault
#define f_pwl_ups_low_bat_cap 350		// 350F - power loss ups battery capacity low fault
#define f_ins_eexit 351					// 351F - emergency motion exit from inspection 
#define f_reset_run_inhibit 352			// 352F - Run inhibit from reset count
#define f_atfloor_shutdown 353			// 353F - At floor shutdown
#define f_hotoil 354					// 354F - Hot Oil Fault
#define f_epr_pwr_mov_flt 355			// 355F - EP Recovery power lost while moving
#define f_ccb_led_short_red 356			// 356F - Car Call Board LED short red 
#define f_ccb_led_short_green 357		// 357F - Car Call Board LED short green
#define f_ccb_led_short_blue 358 		// 358F - Car Call Board LED short blue
#define f_ccb_led_open_red 359	 		// 359F - Car Call Board LED open red
#define f_ccb_led_open_green 360 		// 360F - Car Call Board LED open green
#define f_ccb_led_open_blue 361	 		// 361F - Car Call Board LED open blue
#define f_ccb_fet_short_red 362	 		// 362F - Car Call Board FET short red 
#define f_ccb_fet_short_green 363		// 363F - Car Call Board FET short green 
#define f_ccb_fet_short_blue 364 		// 364F - Car Call Board FET short blue 
#define f_ccb_fet_open_red 365	 		// 365F - Car Call Board FET open red 
#define f_ccb_fet_open_green 366 		// 366F - Car Call Board FET open green 
#define f_ccb_fet_open_blue 367	 		// 367F - Car Call Board FET open blue
#define f_ccb_led_board 368		 		// 368F - Car Call LED board missing
#define f_ccb_stuck_button 369	 		// 369F - Car Call Board stuck button
#define f_ccb_comm_bd1 370		 		// 370F - Car Call Board local board 1 comm loss
#define f_ccb_comm_bd2 371		 		// 371F - Car Call Board local board 2 comm loss
#define f_ccb_comm_xbd1 372		 		// 372F - Car Call Board local aux board 1 comm loss
#define f_ccb_comm_xbd2 373		 		// 373F - Car Call Board local aux board 2 comm loss
#define f_hcb_low_supply_voltage 374 	// 374F - Hall call board has low supply voltage
#define f_hcb_up_input_overload 375	 	// 375F - HCB Up input overload
#define f_hcb_dn_input_overload 376		// 376F - HCB Down input overload
#define f_hcb_xu_input_overload 377		// 377F - HCB Aux Up input overload
#define f_hcb_xd_input_overload 378		// 378F - HCB Aux Down input overload
#define f_fvars_partial_init 379		// 379F - Field variables partial table initialized
#define f_rcm_lock_off	380				// 380F - Retiring Cam/Lock fault
#define f_ebrk_can_error 381			// 381F - Emergency Brake board can error
#define f_encoder_preset 382			// 382F - encoder Preset error
#define f_encoder_ppr_err 383			// 383F - encoder ppr error
#define f_encoder_com_err 384 			// 384F - encoder com error
#define f_encoder_init 385				// 385F - encoder init
#define f_vel_decel_diff 386			// 386F - Velocity decel difference fault
#define f_spb_dir_fault 387				// 387F - SPB encoder opposite direction fault
#define f_limit_dir_flt 388				// 388F - Car hit limits in wrong direction fault
#define f_dir_fault	389					// 389F - Encoder direction fault
#define f_keb_no_run_mode 390			// 390F - Keb drive not in run mode
#define f_uldl_dir_flt 391				// 391F - UL and DL Direction Fault
#define f_can_bus_idle 392				// 392F - can bus idle error
#define f_can_ack_err 393				// 393F - can bus acknowledge error
#define f_encoder_busy_flt 394 			// 394F - Encoder busy fault
#define f_spb_vel_diff 395				// 395F - SPB Velocity difference fault
#define f_user_init 396					// 396F - User variable init
#define f_lw_init 397					// 397F - Load weigher init
#define f_MRAM_fault 398				// 398F - Mram Fault
#define f_enc_can_bus_off 399			// 399F - Encoder can bus off
#define f_enc_can_bus_idle 400			// 400F - Encoder can bus idle
#define f_enc_can_ack_err 401			// 401F - Encoder can bus acknowledge error
#define f_enc_can_pkt_flt 402			// 402F - Encoder can bus packet fault
#define f_xbrk_over_current 403			// 403F - Aux Brake board over current fault (brake error 2)
#define f_xbrk_over_volt 404			// 404F - Aux Brake board over voltage fault (3)
#define f_xbrk_no_current 405			// 405F - Aux Brake board no current fault with voltage applied (4)
#define f_xbrk_no_dc_bus_volt 406		// 406F - Aux Brake board no dc bus voltage fault (5)
#define f_xbrk_no_output_volt 407		// 407F - Aux Brake board not output voltage fault	(6)
#define f_xbrk_low_dc_bus_volt 408		// 408F - Aux Brake board low dc bus voltage fault	 (7)
#define f_xbrk_dmd_gt_dc_bus 409		// 409F - Aux Brake board dmd voltage greater than dc bus voltage fault (8)
#define f_gts_eexit 410					// 410F - GTS Motion emergency exit
#define f_EBK1_off 411					// 411F - Emergency Brake EBK1 failed off fault
#define f_EBK1_on 412					// 412F - Emergency Brake EBK1 failed on fault
#define f_EBK2_off 413					// 413F - Emergency Brake EBK2 failed off fault
#define f_EBK2_on 414					// 414F - Emergency Brake EBK2 failed on fault
#define f_grip_levflt 415				// 415F - Gripper set from leveling fault
#define f_EMBK_levflt 416				// 416F - Emergency brake set from leveling fault
#define f_dzsl_off 417					// 417F - Selector Door zone off fault
#define f_Rail_Lock_Sw 418				// 418F - Rail Lock Switch
#define f_Machine_Room_Stop_Sw 419		// 419F - Machine Room Stop Switch
#define f_SPB_count_upd_err 420			// 420F - Safety Processor count update error
#define f_MRCAN_device_fault 421		// 421F - Device fault (must look up device code
#define f_CTCAN_device_fault 422		// 422F - Device fault (must look up device code
#define f_GRCAN_device_fault 423		// 423F - Device fault (must look up device code
#define f_MRCAN_device_reset 424 		// 424F - Car comm device reset
#define f_CTCAN_device_reset 425 		// 425F - Car comm device reset
#define f_GRCAN_device_reset 426 		// 426F - Car comm device reset
#define f_hatch_safety 427				// 427F - Hatch safety fault
#define f_cop_can_error 428 			// 428F - Top car device can comm error
#define f_mcx_off 429					// 429F - MCX off fault
#define f_mcx_on 430					// 430F - MCX on fault
#define f_FEP_fuse 431					// 431F - Fire/Emergency Fuse Blown
#define f_phone_test 432				// 432F - phone test failed
#define f_bad_SPB_umotion 433			// 433F - wrong umotion command being sent to spb
#define f_trace 434						// 434F - trace trigger occured
#define f_EE1_Failed_ON 435				// 435F - EE1 front input failed ON fault 
#define f_EE1_Failed_OFF 436			// 436F - EE1 front input failed OFF fault 
#define f_EE2_Failed_ON 437				// 437F - EE2 front input failed ON fault
#define f_EE2_Failed_OFF 438			// 438F - EE2 front input failed OFF fault
#define f_EER1_Failed_ON 439			// 439F - EER1 rear input failed ON fault 
#define f_EER1_Failed_OFF 440			// 440F - EER1 rear input failed OFF fault
#define f_EER2_Failed_ON 441			// 441F - EER2 rear input failed ON fault
#define f_EER2_Failed_OFF 442			// 442F - EER2 rear input failed OFF fault
#define f_FETST_ON_fault 443			// 443F - front edge test output ON fault
#define f_FETST_OFF_fault 444			// 444F - front edge test output OFF fault
#define f_RETST_ON_fault 445			// 445F - rear edge test output ON fault
#define f_RETST_OFF_fault 446			// 446F - rear edge test output OFF fault
#define f_bkt_stall_fault 447			// 447F - Brake test stall fault
#define f_spb_limit_vel_er 448			// 448F - Safety Processor Limit Velocity Error
#define f_nts_limit_vel_er 449			// 449F - NTS Processor Limit Velocity Error
#define f_bkt_un_dn 450		  			// 450F - Brake Test UN, DN error
#define f_bkt_ut_dt 451		  			// 451F - Brake Test UT, DT error
#define f_bkt_uts_dts 452		  		// 452F - Brake Test UTS, DTS error
#define f_bkt_time_out 453				// 453F - Brake Test Time-out before top speed
#define f_runx_on 454					// 454F - RUNX input or output failed on
#define f_runx_off 455					// 455F - RUNX input or output failed off 
#define f_motion_runx_off 456			// 456F - RUNX input or output failed off during motion 
#define f_mram_write_er 457				// 457F - Mram Write error
#define f_fvars_backup 458				// 458F - Field Variable backup table error
#define f_fvars_chksum_both 459			// 459F - Field Variable and backup table checksum error
#define f_fvars_chksum 460				// 460F - Field Variable checksum error
#define f_fvars_chksum_bkup 461			// 461F - Field Variable backup checksum error
#define f_fvars_backkup_init 462		// 462F - Field variable backup init
#define f_swg_fhdoor_open 463			// 463F - Swing front hall door open flt
#define f_swg_rhdoor_open 464			// 464F - Swing rear hall door open flt
#define f_NTS_UP 465					// 465F - NTS fault during run in the up direction
#define f_NTS_DN 466					// 466F - NTS fault during run in the down direction
#define f_uncontrolled_levflt 467		// 467F - Uncontrolled Leveling fault
#define f_vel_overspeed_125	468			// 468F - Car overspeed greater than 125 %
#define f_umotion_enc_vel 469			// 469F - Unintended motion detected from encoder velocity
#define f_umotion_spb_vel 470			// 470F - Unintended motion detected from Safety processor velocity
#define f_umotion_gov 471				// 471F - Governor overspeed trip
#define f_umotion_ins_do_ndz 472		// 472F - Unintended motion on inspection, door open and no door zone
#define f_umotion_auto_do_ndz 473		// 473F - Unintended motion on automatic, door open and not door zone
#define f_ETSU_flt 474					// 474F - ETS failed on at the top floor
#define f_ETSD_flt 475					// 475F - ETS failed on at the bottom floor
#define f_lw_load_table 476				// 476F - Load Weigher load table fault
#define f_SPB_setup_flt 477				// 477F - SPB configuration error - incorrect parameters loaded
#define f_shutdn_alarm 478				// 478F - Car out of service and need mechanic
#define f_GOVRi_on_fault 479			// 479F - Governor Reset input GOVRi on fault
#define f_CED_APS_can_error 480			// 480F - Cedes APS can comm fault
#define f_CED_APS_fault 481				// 481F - Cedes APS fault
#define f_CED_brd_sel_fault 482			// 482F - Cedes Selector Interface board Fault
#define f_CED_brd_can_error 483			// 483F - Cedes Selector Interface board CAN comm fault
#define f_CEN_failed_on	484				// 484F - CEN Relay input/output failed on 
#define f_CEN_failed_off 485			// 485F - CEN Relay input/output failed off
#define f_PAL_failed_on	486				// 486F - PAL fault input failed on 
#define f_PAL_failed_off 487			// 487F - PAL fault input failed off	
#define f_CFLT_failed_off 488			// 488F = CFLT/STE C Contactor/input/output failed off
#define f_CFLT_failed_on 489 			// 489F - CFLT/STE C Contactor/input/output failed on
#define f_Redundancy_DZ_lost 490		// 490F - DZ lost durning Redundancy Test
#define f_SUF_on 491					// 491F - SUF I/O on fault
#define f_SDF_on 492					// 492F - SDF I/O on fault
#define f_SPD_on 493					// 493F - SPD I/O on fault
#define f_SPD_off 494					// 494F - SPD I/O off fault
#define f_CEN_flt 495					// 495F - CEN I/O off fault
#define f_hw_cnt_read 496				// 496F - Hoistway Count Read Fault
#define f_hw_sd_cnt 497					// 497F - Hoistway Slowdown Count fault
#define f_CEN_TPH_failed_off 498		// 498F - CEN/TPH failed off
#define f_NTS_setup_flt	499				// 499F - NTS CPU parameter setup fault

#define max_nmbflts 499				// set to last error code number




extern int16 FS_DOB_DIS;       	// fire phase 1 door open button dissable
extern int16 fs2_to_fs1;       	// fire phase 2 changing modes to fire phase 1
extern int16 old_fs2_to_fs1;	// old fs2 to fs1
extern int16 fs2_on;			// fire phase 2 switch on
extern int16 fs2_hold;			// fire phase 2 switch hold
extern int16 fs2_start;			// fire phase 2 switch start
extern int16 fs_on;				// fire phase 1 switch input
extern int16 prev_fs_on;		// previous value fire phase 1 switch input
extern int16 fsx_on;			// fire phase 1 aux switch input
extern int16 mes_on;			// main egress input 
extern int16 alt_on;			// alternate egress input
extern int16 mrs_on;			// machine room sensor input
extern int16 hws_on;			// hoistway sensor input
extern int16 hws2_on;			// hoistway 2 sensor input
extern int16 fs1_reset_on;		// fs1 reset input
extern int16 fs1_effect;       	// fire service phase 1 is in effect
extern int16 fire_sensor;	   	// fire sensor has been activated
extern int16 fire_reset;		// fire reset enabled from reset switch
extern int16 fire_floor;		// fire recall floor
extern int16 old_firef;    		// old fire service phase flag
extern int16 old_rfiref;       	// old rear fire service phase flag
extern int16 rfiref;           	// rear door fire service phase flag
extern int16 Blink_FSL;			// Blinking FSL light fire in Machine room or hatch
extern int16 Extinguish_FSL;	// Extinguish Fire Service Output (Phase one before/after device actuation)
extern int16 firef;            	// door fire service phase flag
extern int16 fire_stopsw_hold;	// used in a17.1 2000 to hold car until car call entered after stop switch hit.
extern uint16 fire_status;		// Fire Status:
								// 	bit0: MES
								//	bit1: ALT
								// 	bit2: MRS
								//  bit3: HWS
								//  bit4: HWS2
								//  bit5: Unused
								//  bit6: Unused
								//  bit7: Unused
#define fs_MES 		0x0001
#define fs_ALT 		0x0002
#define fs_MRS 		0x0004
#define fs_HWS 		0x0008
#define fs_HWS2 	0x0010
#define fs_FS  		0x0020
#define fs_FSX 		0x0040
#define fs_ANY_FI  	0x0080						
#define fs_FS2		0x0100
#define fs_FS2H		0x0200
								
								
extern int16 fire_fdo_seq;		// denver fire service phase 1 front door open sequence flag
extern int16 fire_rdo_seq;		// denver fire service phase 1 rear door open sequence flag
extern int16 fs2_no_fdoor_op;	// Fire Phase 2 No Front Door Operation after reset flag
extern int16 fs2_no_rdoor_op;	// Fire Phase 2 No Rear Door Operation after reset flag

extern int16 momentary_dcb;		//momentary door close button
extern int16 momentary_rdcb;	//momentary rear door close button								
extern int16 hsf;            	// high speed flag
extern int16 startf;			// Start flag 1 = valid start, 0 = no start
extern int16 stallf;          	// low oil or stalled flag
extern int16 old_stallf;		// old stall flag
extern int16 lps_stall;			// 1 = low pressure switch stall
extern int16 nstopf;         	// need to stop flag
extern int16 next_stop;			// next stop floor
extern int16 short_fl_run;		// Short floor run flag
extern int16 no_high_speed;		// No high speed output
extern int16 front_slowdown;	// Front slowdown
extern int16 rear_slowdown;		// Rear slowdown
#define uc_sd 1					// up hall call slowdown
#define dc_sd 2					// down hall call slowdown
#define cc_sd 4					// car call slowdown
#define uc_do 0x10				// request up call door open
#define dc_do 0x20				// request down call door open
#define cc_do 0x40				// request car call door open
#define iruc_sd 0x100			// up hall call slowdown
#define irdc_sd 0x200			// down hall call slowdown

extern int16 predof;         		// preopen door flag
extern int16 prev_procf;
extern int16 procf;            // procedure flag
#define last_procf 23

extern int32 prev_statusf;
extern int32 statusf;		// Status flag
#define sf_S10 0x1			// Status: NO S10 power
#define sf_HC 0x2			// Status: NO HC power
#define sf_SS 0x4			// Status: NO SS input
#define sf_READY 0x8		// Status: Drive not ready
#define sf_GRIP 0x10		// Status: Gripper error
#define sf_IO 0x20 			// Status: I/O error during redundancy check
#define sf_INS 0x40			// Status: Inspection or lock bypass fault
#define sf_BPI 0x80			// Status: Binary Position Input Error
#define sf_POS 0x100 		// Status: Position Error
#define sf_AD 0x200			// Status: No automatic Doors
#define sf_STP 0x400		// Status: Stop switch open
#define sf_DZ 0x800			// Status: Door Zone fault
#define sf_GDL 0x1000		// Status: Gate or Door lock fault
#define sf_P 0x2000   		// Status: No Potential Input
#define sf_DCL 0x4000		// Status: No DCL
#define sf_DCC 0x8000		// Status: No Door Close Contact
#define sf_BKS 0x10000L	// Status: Brake lift switch error
#define sf_TOC 0x20000L		// Status: Top of Car Comm Error
#define sf_DRV 0x40000L		// Status: Drive Comm Error
#define sf_SPB 0x80000L		// Status: Safety Processor Board Comm Error
#define sf_DBR 0x100000L  	// Status: DB Resistor Temp. Error
#define sf_SHUTDN 0x200000L	// Status: Shutdown (too many fault runs)
#define sf_ASTST 0x400000L	// Status: Annual Safety Test             
#define sf_SAFE 0x800000L	// Status: Waiting for Car to be safe()
#define sf_TLimit 0x1000000L // Status: UT,UTS,DT or DTS limit error
#define sf_GTS 0x2000000L	// Status: GTS input off
#define sf_ULDLDZ 0x4000000L	// Status: UL, DL and DZ off at floor
#define sf_BKCAN 0x8000000L // Status: Brake Board Can Error
#define sf_FSTP 0x10000000L // Status: Fire Fighter Stop Switch
#define sf_SELCAN 0x20000000L // Status: Selector Can error
#define sf_ULDL 0x40000000L	// Status: UL or DL fault
#define sf_LEV 0x80000000L	// Status: Leveling fault

extern int32 prev_statusf2;
extern int32 statusf2;		// Status flag
#define sf_HWINIT 0x1L		// Status: Hardware Init fault
#define sf_FDC 0x2L			// Status: Front Door Closing Fault
#define sf_RDC 0x4L			// Status: Rear Door Closing Fault
#define sf_LVolt 0x8L 	  	// Status: Line Voltage Fault
#define sf_DVolt 0x10L 	  	// Status: Door Voltage Fault
#define sf_BKSR 0x20L		// Status: Brake lift switch run error 
#define sf_DMO 0x40L		// Status: Door motor overload
#define sf_LearnHW 0x80L	// Status: Learn Hoistway Fault
#define sf_PWLUPS 0x100L	// Status: Power Loss UPS Fault
#define sf_EBKCAN 0x200L	// Status: Emergency Brake Can error
#define sf_KEBRUN 0x400L	// Status: KEB Drive Not in Run Mode
#define sf_AF_SHUTDN 0x800L	// Status: At Floor Shutdown
#define sf_1036 0x1000L		// Status: 1036 board connected
#define sf_ResetRun 0x2000L	// Status: Reset run fault
#define sf_SEL_Count 	0x4000L 		// Status: Invalid SEL count
#define sf_COPCAN 		0x8000L			// Status: COP can comm error
#define sf_ENCCAN 		0x10000L		// Status: Encoder can comm error
#define sf_IO_Test		0x20000L		// Status: IO Test in progress
#define sf_GRIP_Test 	0x40000L		// Status: Gripper Test in progress
#define sf_DRV_Pwr 		0x80000L		// Status: No Power to Drive
#define sf_NIT 			0x100000L		// Status: Non Interference timer
#define sf_DOOR_Req 	0x200000L		// Status: Door open request
#define sf_DPM 			0x400000L		// Status: Waiting for DPM
#define sf_RPM 			0x800000L		// Status: Waiting for RPM
#define sf_VISC 		0x1000000L		// Status: Viscosity operation
#define sf_LEV_Req 		0x2000000L		// Status: Leveling request
#define sf_Term_Spd		0x4000000L		// Status: Terminal Limit Speed Clamp
#define sf_Spd_Rate		0x8000000L		// Status: Sped Rate Clamp
#define sf_EE_SHUTDN	0x10000000L 	// Status: Front EE Test failed fault
#define sf_EER_SHUTDN 	0x20000000L 	// Status: Rear EE Test failed fault
#define sf_Setup_Svc	0x40000000L		// Status: Setup Mode out of auto service
#define sf_Shutdown_Alarm 0x80000000L	// Status: Service Person Required

extern int32 prev_statusf3;
extern int32 statusf3;					// Status flag
#define sf_SPB_SAF_CAN 		0x1L		// Status: SPB safety CAN error
#define sf_APS_Vel_flt 		0x2L		// Status: APS Velocity Verificatin fault
#define sf_APS_SEL_Brd_Flt 	0x4L		// Status: APS Selector board fault
#define sf_APS_SEL_CAN_Flt	0x8L		// Status: APS Selector CAN fault
#define sf_NTS_SEL_CAN_Flt  0x10L		// Status: NTS APS Selector CAN fault

extern int32 prev_statusf4;
extern int32 statusf4;					// Status flag


extern int16 safety_string_status;
#define sss_GOV  0x0001		// Governor input open
#define sss_TF   0x0002		// Top Final Limit Open
#define sss_BF   0x0004		// Bottom Final Limit open
#define sss_PS   0x0008		// Pit Switch open
#define sss_HSS  0x0010		// Hoistway Safety
#define sss_CTS  0x0020		// Car Top Stop switch open
#define sss_CSS  0x0040		// Car Safety Switch open
#define sss_RLS  0x0080		// Rail Lock Switch
#define sss_FFS  0x0100		// Fire Fighter Stop Switch
#define sss_CST  0x0200		// Car Stop Switch
#define sss_MRSW 0x0400		// Machine Room Stop Switch
#define sss_GTSW 0x0800		// Gripper Trip Switch

/*
#define sss_GOV 0x0001		// Governor input open
#define sss_TF  0x0002		// Top Final Limit Open
#define sss_BF  0x0004		// Bottom Final Limit open
#define sss_PS  0x0008		// Pit Switch open
#define sss_PDS 0x0010		// Pit Door Switch open
#define sss_BS  0x0020		// Buffer Switch open
#define sss_CMP 0x0040		// Compensating Rope Switch open
#define sss_TEE	0x0080		// Top Emergency Exit Switch open
#define sss_SEE 0x0100		// Side Emergency Exit Switch open
#define sss_CSS 0x0200		// Car Safety Switch open
#define sss_SS  0x0400		// Car Top Stop Switch or Fire Fighter Stop Switch
#define sss_CTS 0x0800		// Car Top Stop switch open
#define sss_HMO 0x1000		// Hoist Motor Overload
#define sss_RLS 0x2000		// Rail Lock Switch
#define sss_MRSW 0x4000		// Machine Room Stop Switch
*/

extern uint32 run_statusf;			// Run status for each car see bits below:
#define rs_RUN	0x00000001
#define rs_DNR  0x00000002
#define rs_UP	0x00000004
#define rs_DL	0x00000008
#define rs_UL	0x00000010
#define rs_DZ	0x00000020
#define rs_DLT	0x00000040
#define rs_DLM	0x00000080
#define rs_DLB	0x00000100
#define rs_GS	0x00000200
#define rs_RLM	0x00000400
#define rs_RGS	0x00000800
#define rs_DOL  0x00001000
#define rs_DCL  0x00002000
#define rs_DOLR 0x00004000
#define rs_DCLR 0x00008000
#define rs_EE	0x00010000
#define rs_SE	0x00020000
#define rs_EER	0x00040000
#define rs_SER	0x00080000
#define rs_HSF	0x00100000		// High Speed Flag
#define rs_STF	0x00200000		// Start Flag
#define rs_LSTF 0x00400000		// Leveling Start Flag
#define rs_DZA	0x00800020		// Door Zone OR'd
#define rs_DO	0x01000000		// Door Open
#define rs_DC	0x02000000		// Door Close
#define rs_DOR	0x04000000		// Rear Door Open
#define rs_DCR	0x08000000		// Rear Door Close
#define rs_UN	0x10000000		// Up Normal Limit
#define rs_DN	0x20000000		// Down Normal Limit


extern int16 power_status;
#define ps_HC   0x0001		// Hall call power loss
#define ps_HCL	0x0002		// Hall call light power loss
#define ps_CC	0x0004		// Car call power loss
#define ps_CCL	0x0008		// Car call light power loss
#define ps_LHC  0x0010		// Lobby Hall common power loss
#define ps_FEP  0x0020		// Fire/Emergency Power Loss


extern int16 rdoorf;           // rear door flag
extern int16 ins_rdoorf;
extern int16 rdoor_en;		// rear door enable
extern int16 rdoor_req;		// rear door request to open
extern int16 rdob_req;		// rear door open button request
extern int16 rcc_req;			// rear car call door open request
extern int16 rnudgst;			// rear door nudging status
extern int16 rdo_to;			// Set when door open limit times out.
extern int16 ree_to;			// Set when rear electric eye times out.
extern int16 rdo_seq;
extern int16 manual_rdoor;	// set for manual rear door 
extern int16 freight_rdoor_alarm;	// freight rear door alarm
extern int16 rsdo;			// Rear swing door open
extern int16 rsdo_seq;		// Rear swing door open sequence
extern int16 level_en;		// boolean level enable output since LE and LE1 are turned off during gripper test
extern int16 relevel;         // relevel
extern int16 relevel_req;		// request to relevel 
							// 1 = car is not level and needs to relevel
							// 0 = car is level or in leveling
extern int16 stop_seq;		// Stop sequence
							// 0 = stopped and run and mc dropped
							// 1 = stopped with soft stop timming
							// 2 = stopped with soft stop timmed out but run and mc timming
extern int16 leveling_fault_cnt;	// Leveling fault counter (relevel called before the stop sequence went to zero
extern int16 leveling_fault;	// leveling fault flag
extern int16 lev_startf;		// leveling start flag

#define s_SSTRING 		0
#define s_INSP 			1
#define s_RESET			2
#define s_CONFIG		3
#define s_HWLEARN		4

#define s_FFSTOP		5
#define s_MRSTOP		6

#define s_SHUTDOWN		7
#define s_COM_FAIL		8
#define s_DC_FAIL		9
#define s_GATE_LOCK		10
#define s_STALLED 		11
#define s_LOW_OIL		12

#define s_EAQ 			13
#define s_EMP 			14
#define s_FIRE2 		15
#define s_F1MAIN 		16
#define s_F1ALT 		17
#define s_EPR			18
#define s_HSV 			19
#define s_MED_EMS 		20
#define s_CBLUE 		21

#define s_LOW_PRES	 	22
#define s_HOT_OIL 		23

#define s_ADOFF			24

#define s_RIOT 			25
#define s_ELEV_OFF_CS 	26
#define s_ELEV_OFF_HS 	27
#define s_CAR_OFF 		28
#define s_LBY_RCL 		29

#define s_IND 			30
#define s_VIP 			31

#define s_LW_CAL 		32
#define s_RST_JACK		33
#define s_LW_OVL 		34	
#define s_LWBP 			35
#define s_EXT_DOOR		36

#define s_RESET_UP 		37
#define s_RESET_DN 		38
#define s_SEC_RCL 		39
#define s_TUGS 			40
#define s_SABBATH 		41
#define s_ATT 			42
#define s_HOMING 		43
#define s_AUTO 			44

extern int16 prev_servf;      // service flag
extern int16 servf;           // service flag
			// servf = 0 Safty String
			// servf = 1 Inspection
			// servf = 2 Reset Mode
			// servf = 3 Configuration Mode
			// servf = 4 Hoistway Setup 
			// servf = 5 Fire Fighters Stop Switch
			// servf = 6 Machine Room Stop Switch
			// servf = 7 AT Floor Shutdown
			// servf = 8 Comm Fail
			// servf = 9 Door Close Fail
			// servf = 10 Gate and Lock 
			// servf = 11 Stalled
			// servf = 12 Low Oil
			// servf = 13 Earthquake
			// servf = 14 Emergency Power
			// servf = 15 Fire Phase 2
			// servf = 16 Fire Phase 1 Main
			// servf = 17 Fire Phase 2 Alt
			// servf = 18 Emergency Power Recall
			// servf = 19 Hospital Service
			// servf = 20 Medical emergency Service
			// servf = 21 Code Blue
			// servf = 22 Low Pressure
			// servf = 23 Hot Oil.
			// servf = 24 Auto Door Off
			// servf = 25 RIOT
			// servf = 26 Elevator Off Car Switch
			// servf = 27 Elevator Off Hall Switch
			// servf = 28 Car Off
			// servf = 29 Lobby Recall
			// servf = 30 Independent
			// servf = 31 VIP Service
			// servf = 32 Load Weigher Calibration
			// servf = 33 Reset Jack
			// servf = 34 Load Weigh Overload
			// servf = 35 Load Bypass
			// servf = 36 Extended Door Time
			// servf = 37 Reset Up
			// servf = 38 Reset Down
			// servf = 39 Security Recall
			// servf = 40 TUGS
			// servf = 41 Sabbath Service
			// servf = 42 Attendant
			// servf = 43 Homing
			// servf = 44 Auto 
			
#define last_servf 44

extern int16 rhjackf;			// Reset Hydro Jack Flag
extern int16 stepf;           // position stepping flag
extern int16 medf;			// Mass. Medical Emergency Service
extern int16 med_latch;		// Switch latch for medical emergency service
extern int16 hotoilf;	// hot oil temperature fault
extern int16 old_hotoilf;		// old hot oil temperature flag
extern int16 old_trace_auto_start;	// old variable to start trace automatically on power up
extern int16 govrstf;			// Governor reset flag			
extern int16 govtripf;		// Governor trip flag			
extern int16 pwl_brakef;		// Flag to pick brake with power loss

extern int16 keep_lants_on;   // keeps the cab riding lanterns on
extern int16 keep_rlants_on;  // keeps the rear cab riding lanterns on
extern int16 lant_up;			// logical boolean for lantern up
extern int16 lant_dn;			// logical boolean for lantern down
extern int16 rlant_up;		// logical boolean for rear lantern up
extern int16 rlant_dn;		// logical boolean for rear lantern down
extern int16 cab_lant_up;		// logical boolean for cab lantern up
extern int16 cab_lant_dn;		// logical boolean for cab lantern dn
extern int16 cab_rlant_up;	// logical boolean for cab rear lantern up
extern int16 cab_rlant_dn;	// logical boolean for cab rear lantern dn
extern int16 up_dir_arrow;	// logical boolean for up direction arrow
extern int16 dn_dir_arrow;	// logical boolean for down direction arrow

extern int16 IcMrStopSw;		// logical boolean for in car and motor room stop switch
extern int16 InCarStopSw;		// logical boolean for in car stop switch


extern int16 flight_times[fl_size][fl_size];
extern int16 Viscosity;
extern int16 bypass_hc;
extern int16 hb_en;			// handicap buzzer enable control flag
extern int16 handicap_buz;	// handicap buzzer flag
extern int16 hb_cc_buz;		// handicap car call buzzer flag
extern int16 cc_sec_buz;		// car call security buzzer flag
extern int16 nudg_buz;		// Nudging buzzer boolean
extern int16 rnudg_buz;		// Nudging buzzer boolean for rear door
extern int16 fire_buz;		// Fire service buzzer boolean
extern int16 med_buz;			// Medical Emergency service buzzer boolean
extern int16 att_buz;			// Attendant Buzzer
extern int16 att_buz_on;		// Attendant Buzzer On timer 
extern int16 att_nmb_hc2buz;	// Attendant Number of Hall Calls to Buzz
extern int16 att_nmb_hc;		// Attendant Number of Hall Calls 
extern int16 update_att_nmb_hc;	//Update the number of hall calls that the system needs to Buzz
extern int16 att_buz_hc;		// Attendant Buzz on hall call
extern int16 att_buz_hc_en;	// Attendant Buzz on hall call enable
extern int16 athcd_in_enable;
extern int16 MST_enable;	////allow operation of MST if 1
extern int16 door_buz;		// Manual door left open buzzer
extern int16 sabb_buz;		// Sabbath warning buzzer before doors close
extern int16 sabb_ee_en;		//sabbath electric eye enable
extern int16 sabbath_active;	// Sabbath active flag (when sabbath is running)
extern int16 sabbath_start;	// Wait until set to 1 to start sabbath operation
extern int16 timeout_mandoor; //	timeout manual door for hall calls
extern int16 stp_sw;			// debounced stp input
extern int16 ut_limit;		// Debounced ut input
extern int16 dt_limit;		// Debounced dt input
extern int16 uts_limit;		// Debounced uts input
extern int16 dts_limit;		// Debounced dts input
extern int16 ets_limit;		// Debounced ets input
extern int16 start_floor;
extern int16 no_fault;
extern int16 park_fl;			// parking floor
extern int16 lby_req_fl;		// lobby request floor
extern int16 zone_fl;			// floor to zone car
extern int16 latch_HEOF;		// Latch HEOF operation
extern int16 latch_CEOF;		// Latch EOF operation
extern int16 latch_JAIL;		// Latch for Jail break input

extern int16 galcom_ver;
extern int16 galcom_rev;



// interger variables used throughout the program
extern int16 nmb_carcalls;	   // number of latched car calls
extern int16 all_cars_ots;       // all cars are out of service
extern int16 nmb_cars_in_svc;			// number of cars in service
extern int16 stops[fl_size];          // keep an array of the sequence of stops
extern int16 nmbstops;           // number of stops to clear calls
extern int16 carcb[fl_size];          // car call buffer value = 1 there is a call
extern int16 rcarcb[fl_size];         // rear car call buffer value = 1 there is a rear call
extern int16 closetry;           // number of times the doors have tryed to close
extern int16 rclosetry; 	       // number of times the rear doors have tryed to close
extern int16 safe_closetry;  	   // number of times the doors have tryed to close to get safe
extern int16 safe_rclosetry; 	   // number of times the rear doors have tryed to close to get safe
extern int16 codebfl;            // code blue floor value is position of emergency
extern int16 doort;              // door dwell time
extern int16 rdoort;          // rear door dwell time
extern int16 dtmrf;				// door dwell timer flag
#define cc_dt 1					// car call door time was set
#define hc_dt 2					// hall call door time was set
extern int16 upcb[fl_size];           // up hall call buffer value = 1 there is a call
extern int16 rupcb[fl_size];          // rear up hall call buffer value = 1 there is a call
extern int16 downcb[fl_size];         // down hall call buffer value = 1 there is a call
extern int16 rdowncb[fl_size];        // rear down hall call buffer value = 1 there is a call
extern int16 nmbflts;            // number of faults that have occured
extern int16 position;           // the position of the elevator
extern int16 prev_position;	   // previous position of the elevator
extern int16 scnd_pos;		// temporary position between floors 
extern int16 prev_scnd_pos;
extern int16 preset;			   // set to 1 when binary preset is set
extern int16 preset_pos;			// set to 1 to update the preset the position
extern int16 Lev_Mode;           // Used for emergency slowdown traction's
extern int16 Hoistway_Learned;   // Hoistway Learned 1 to run automatic
extern int16 Field_Force;	       // Field Force
extern int16 rset;               // in reset and have not found a floor yet
extern int16 rset_run_cnt;
extern int16 rset_error_cnt;	// max times to log reset error looping in reset mode
extern int16 baud_default;
extern int16 ext_temperature;		// External temperature
extern int16 mr_temperature;		// Motor room temperature
extern int16 safe_override_seq;	// safe override sequence to lift brake on inspection
extern int16 eef_testtry;		// front electric eye test try count 
extern int16 eer_testtry;		// rear electric eye test try count 


extern int16 dol_fail_cnt;		// door open limit fail count
extern int16 rdol_fail_cnt;		// door open limit fail count
extern bool dol_fail;			// door open limit fail flag
extern bool rdol_fail;			// rear door open limit fail flag


#if (Traction == 1)
extern float Max_Lant_Time;
#endif

extern int16 speed_clamp[9];		// Software velocity clamp stored in non-volitile memory

extern int16 dpp_store_tmr;
extern int16 enc_vel_timer;		// velocity timer for encoder velocity calculation 
extern int16 enc_save_timer;
extern int16 dz_dbn_timer;
extern int16 drive_update_timer;
extern int16 dpyspi_timer;
extern int16 iospi_timer;
extern int16 can_sel_timer;
extern int16 can_sync_timer;
extern int16 Overspeed_Test;
extern int16 Buffer_Test;
extern int16 NTS_Test;
extern int16 ETS_Test;

extern int16 don_delay;			// drive on delay timer

// Status Bit flags
extern uint8 Door_Req_Flags;
extern uint8 Door_Seq_Flags;
extern uint8 Fault_Bits[4];
extern uint8 Nudgst_Flags;
extern uint8 Call_Flags;



//  Local variable for service timers
extern int16 c_tmr_svc[17];
extern int16 c_tmr_svc_en[17];
extern int16 c_tmr_svc_logic[17];


// variables for security
extern int16 Sec_Car[car_size][6][26];
extern int16 Sec_Hall[car_size][6][26];

extern int16 Security_Type[car_size];	// 0 = No Security, 1 = Car call lockouts	
extern int16 Car_Call_Lockouts; 		// Car call lockouts and car reader security
extern int16 Lockout_Override;			// Key input to override group floor or ids floor security (uses car call lockout inputs)
extern int16 Sec_Fl_Mask;				// Security Floor mask select switch
extern int16 ind_override;		  		// Independent overrides security when set
extern int16 ind_secf_override;			// Independent overrides security even from SECF tables
extern int16 med_override;				// Medical service overrides security when set
extern int16 secdis_override;			// Security disabling input overrides security when set
extern int16 sec_recall;				// Set to recall car on security.
extern int16 sec_recall_complete;		// Set when security recall is complete.
extern int16 sec_panic_recall;			// panic recall flag to cancel car calls and recall to security floor
extern int16 ind_sec_recall;			// Initiate Independent security recall

extern uint32 cc_sec_mask[2];			// Car call security mask
extern uint32 rcc_sec_mask[2];			// Rear Car call security mask
extern uint32 master_cc_sec[2];			// Car call security mask
extern uint32 master_rcc_sec[2];   		// Rear Car call security mask
extern uint32 master_cc_sec_prev[2];		// Car call security mask
extern uint32 master_rcc_sec_prev[2];	// Rear Car call security mask

extern int16 car_alrm;

// Group I/O's common to car and group ... Must be set in car z6 cpu

extern uint8 gin_EMP;
extern uint8 gin_EPT;
extern uint8 gin_ATTHC;
extern uint8 gin_SAB;
extern uint8 gin_SECFM;
extern uint8 gin_HCP;
extern uint8 gout_GSERV;


struct car_Grp_Stat_struct 
{
	int16 c_park_fl;
	int16 c_gempf;
	int16 c_grp_car;
	int16 c_ncu_door;
	int16 c_next_up;
	int16 c_ncu_que;
	int16 c_lby_req_fl;
	int16 c_local_up_peak;
	int16 c_alt_lobby;
	int16 c_grp_fire_ctrl;
	int16 c_car_hc_fire_door_open;
	int16 c_fvfiresw_loc;
	int16 c_asvto_pos;
	int16 c_ir_active;
	int16 c_ir_car;
	int16 c_any_ir_hall_call;
	int16 c_any_hall_call;
	int16 c_sr_cc_ctrl;
	int16 c_cb_fl;
	int16 c_rcb_fl;
	int16 c_grp_codebf;
	int16 c_in_grp_sabbath;
	int16 c_sec_fl_ctl;
	int16 c_car_asgn_sec;
	int16 c_vip_fl;
	int16 c_rvip_fl;
	int16 c_ids_allw_door;
	int16 c_ids_return_typ;
	int16 c_ids_return_fl;
	int16 c_ids_fsx;
	int16 c_ids_ind;
	int16 c_ids_rtl;
	int16 c_ids_heof;
	int16 c_ids_ad_off;
	int16 c_ui_ind;
	int16 c_ui_rtl;
	int16 c_ui_heof;
	int16 c_ui_ad_off;
	int16 c_grp_rst_doort;
	int16 c_grp_rst_rdoort;
};

extern struct car_Grp_Stat_struct car_Grp_Stat;



// long variables used throught the program
#define nmblvar 20
extern int32 lvars[nmblvar];
#define nmbcc 0         // number of car calls
#define nmbuc 1         // number of up calls
#define nmbdc 2         // number of down calls
#define uc_less15 3     // number of up calls answered in less than 15 sec
#define uc_less30 4     // number of up calls answered in less than 30 sec
#define uc_less45 5     // number of up calls answered in less than 45 sec
#define uc_less60 6     // number of up calls answered in less than 60 sec
#define uc_more60 7     // number of up calls answered in more than 60 sec
#define dc_less15 8     // number of down calls answered in less than 15 sec
#define dc_less30 9     // number of down calls answered in less than 30 sec
#define dc_less45 10    // number of down calls answered in less than 45 sec
#define dc_less60 11    // number of down calls answered in less than 60 sec
#define dc_more60 12    // number of down calls answered in more than 60 sec


extern int16 day_of_week;
extern int16 old_day_of_week;

extern int16 t_door_open;
extern int16 t_door_dwell;
extern int16 t_door_close;
extern int16 t_rdoor_open;
extern int16 t_rdoor_dwell;
extern int16 t_rdoor_close;

// float floating point variables used throught the program
#define nmbdvar 20
extern float dvars[nmbdvar];


#pragma section sdata_type

extern unsigned char *ccmsk[car_size];        	// car call mask pointer to screen unused calls
extern unsigned char *ucmsk[car_size];        	// up call mask pointer to screen unused calls
extern unsigned char *dcmsk[car_size];        	// down call mask pointer to screen unused calls
extern unsigned char *rccmsk[car_size];       	// rear car call mask pointer to screen unused calls
extern unsigned char *rucmsk[car_size];       	// rear up call mask pointer to screen unused calls
extern unsigned char *rdcmsk[car_size];       	// rear down call mask pointer to screen unused calls

#pragma section all_types

extern unsigned char ccsmsk[car_size][fl_size];    	// car call security mask to screen unused calls
extern unsigned char ucsmsk[car_size][fl_size];    	// up call security mask to screen unused calls
extern unsigned char dcsmsk[car_size][fl_size];    	// down call security mask to screen unused calls
extern unsigned char rccsmsk[car_size][fl_size];   	// rear car call security mask to screen unused calls
extern unsigned char rucsmsk[car_size][fl_size];   	// rear up call security mask to screen unused calls
extern unsigned char rdcsmsk[car_size][fl_size];   	// rear down call security mask to screen unused calls

extern unsigned char ccs2msk[car_size][fl_size];    	// car call security mask to screen unused calls
extern unsigned char ucs2msk[car_size][fl_size];    	// up call security mask to screen unused calls
extern unsigned char dcs2msk[car_size][fl_size];    	// down call security mask to screen unused calls
extern unsigned char rccs2msk[car_size][fl_size];   	// rear car call security mask to screen unused calls
extern unsigned char rucs2msk[car_size][fl_size];   	// rear up call security mask to screen unused calls
extern unsigned char rdcs2msk[car_size][fl_size];   	// rear down call security mask to screen unused calls

extern unsigned char ccs3msk[car_size][fl_size];    	// car call security mask to screen unused calls
extern unsigned char ucs3msk[car_size][fl_size];    	// up call security mask to screen unused calls
extern unsigned char dcs3msk[car_size][fl_size];    	// down call security mask to screen unused calls
extern unsigned char rccs3msk[car_size][fl_size];   	// rear car call security mask to screen unused calls
extern unsigned char rucs3msk[car_size][fl_size];   	// rear up call security mask to screen unused calls
extern unsigned char rdcs3msk[car_size][fl_size];   	// rear down call security mask to screen unused calls

extern unsigned char ccs4msk[car_size][fl_size];    	// car call security mask to screen unused calls
extern unsigned char ucs4msk[car_size][fl_size];    	// up call security mask to screen unused calls
extern unsigned char dcs4msk[car_size][fl_size];    	// down call security mask to screen unused calls
extern unsigned char rccs4msk[car_size][fl_size];   	// rear car call security mask to screen unused calls
extern unsigned char rucs4msk[car_size][fl_size];   	// rear up call security mask to screen unused calls
extern unsigned char rdcs4msk[car_size][fl_size];   	// rear down call security mask to screen unused calls

extern unsigned char ccnmsk[car_size][fl_size];    	// car call normal mask to screen unused calls
extern unsigned char ucnmsk[car_size][fl_size];    	// up call normal mask to screen unused calls
extern unsigned char dcnmsk[car_size][fl_size];    	// down call normal mask to screen unused calls
extern unsigned char rccnmsk[car_size][fl_size];   	// rear car call normal mask to screen unused calls
extern unsigned char rucnmsk[car_size][fl_size];   	// rear up call normal mask to screen unused calls
extern unsigned char rdcnmsk[car_size][fl_size];   	// rear down call normal mask to screen unused calls

extern unsigned char irccmsk[car_size][fl_size];    	// ir car call mask to screen unused calls
extern unsigned char irucmsk[car_size][fl_size];     	// ir up call mask pointer to screen unused calls
extern unsigned char irdcmsk[car_size][fl_size];     	// ir down call mask pointer to screen unused calls
extern unsigned char rirccmsk[car_size][fl_size];   	// rear ir car call mask to screen unused calls
extern unsigned char rirucmsk[car_size][fl_size];    	// rear ir up call mask pointer to screen unused calls
extern unsigned char rirdcmsk[car_size][fl_size];    	// rear ir down call mask pointer to screen unused calls

extern unsigned char irccmsk2[car_size][fl_size];    	// backup ir car call mask to screen unused calls
extern unsigned char irucmsk2[car_size][fl_size];     	// backup ir up call mask pointer to screen unused calls
extern unsigned char irdcmsk2[car_size][fl_size];     	// backup ir down call mask pointer to screen unused calls
extern unsigned char rirccmsk2[car_size][fl_size];   	// backup rear ir car call mask to screen unused calls
extern unsigned char rirucmsk2[car_size][fl_size];    	// backup rear ir up call mask pointer to screen unused calls
extern unsigned char rirdcmsk2[car_size][fl_size];    	// backup rear ir down call mask pointer to screen unused calls

extern unsigned char trccmsk[car_size][fl_size];    	// tr car call mask to screen unused calls
extern unsigned char trucmsk[car_size][fl_size];     	// tr up call mask pointer to screen unused calls
extern unsigned char trdcmsk[car_size][fl_size];     	// tr down call mask pointer to screen unused calls
extern unsigned char rtrccmsk[car_size][fl_size];   	// rear tr car call mask to screen unused calls
extern unsigned char rtrucmsk[car_size][fl_size];    	// rear tr up call mask pointer to screen unused calls
extern unsigned char rtrdcmsk[car_size][fl_size];    	// rear tr down call mask pointer to screen unused calls

extern unsigned char hpccmsk[car_size][fl_size];    // high priority floor car call mask to screen unused calls
extern unsigned char hpucmsk[car_size][fl_size];    // high priority floor up call security mask to screen unused calls
extern unsigned char hpdcmsk[car_size][fl_size];    // high priority floor down call security mask to screen unused calls
extern unsigned char rhpccmsk[car_size][fl_size];   // high priority floor rear car call security mask to screen unused calls
extern unsigned char rhpucmsk[car_size][fl_size];   // high priority floor rear up call security mask to screen unused calls
extern unsigned char rhpdcmsk[car_size][fl_size];   // high priority floor rear down call security mask to screen unused calls

extern unsigned char xa_uc_msk[fl_size];			// cross assignment up call mask to decide which group gets the floor hall call 1=gal,2=other,3=both 
extern unsigned char xa_dc_msk[fl_size];			// cross assignment down call mask to decide which group gets the floor hall call 1=gal,2=other,3=both

extern unsigned char stag_rmsk[fl_size];		// staggard rear floor mask
extern unsigned char heavy_door_fmsk[fl_size];	// heavy door front floor mask
extern unsigned char heavy_door_rmsk[fl_size];	// heavy door rear floor mask
extern unsigned char stag_ado_msk[fl_size];	// staggered auto door floor mask
extern unsigned char stag_ado_rmsk[fl_size];	// staggered auto door rear floor mask
extern unsigned char valid_fl[fl_size];  	// valid floors for this car
extern unsigned char pbdoor_fmsk[fl_size];		// heavy door front floor mask
extern unsigned char pbdoor_rmsk[fl_size];		// heavy door rear floor mask

extern unsigned char sab_ccmsk[fl_size];     	// Sabbath service car call mask pointer to screen unused calls
extern unsigned char sab_ucmsk[fl_size];     	// Sabbath service up call mask pointer to screen unused calls
extern unsigned char sab_dcmsk[fl_size];     	// Sabbath service down call mask pointer to screen unused calls
extern unsigned char sab_rccmsk[fl_size];    	// Sabbath service rear car call mask pointer to screen unused calls
extern unsigned char sab_rucmsk[fl_size];    	// Sabbath service rear up call mask pointer to screen unused calls
extern unsigned char sab_rdcmsk[fl_size];    	// Sabbath service rear down call mask pointer to screen unused calls

extern unsigned char flmrk[fl_size][2];     	// floor markings
extern unsigned char alt_flmrk[fl_size][2];     // alternate floor markings
extern unsigned char Blind_flmrk[2][2];     // Blind floor markings to display on digital PI indicator.
extern unsigned char xpi_pos[fl_size];
extern unsigned char jobnm[26];         // job name
extern unsigned char add1[26];         	// address line #1
extern unsigned char add2[26];         	// address line #2
extern unsigned char cmp[26];          	// company name
extern unsigned char pass[8][26];      	// password
extern unsigned char user[8][26];      	// user name
extern unsigned char vidmes[3][26];    	// video message three lines


// Car call color variables
																				
extern int16 ccOnBright;			// Call On Brightness
extern int16 ccOffBright;			// Call Off Brightness

extern int16 ccOnRed;				// Call On Red
extern int16 ccOnGreen;				// Call On Green
extern int16 ccOnBlue;				// Call On Blue

extern int16 ccOffRed;				// Call Off Red
extern int16 ccOffGreen;			// Call Off Green
extern int16 ccOffBlue;				// Call Off Blue

extern int16 ccSecOnRed;			// Call Security On Red
extern int16 ccSecOnGreen;			// Call Security On Green
extern int16 ccSecOnBlue;			// Call Security On Blue

extern int16 ccAttUpOnRed;			// Call Attendant Up On Red
extern int16 ccAttUpOnGreen;		// Call Attendant Up On Green
extern int16 ccAttUpOnBlue;			// Call Attendant Up On Blue

extern int16 ccAttDnOnRed;			// Call Attendant Down On Red
extern int16 ccAttDnOnGreen;		// Call Attendant Down On Green
extern int16 ccAttDnOnBlue;			// Call Attendant Down On Blue

extern int16 copFireOnRed;			// COP Fire On Red
extern int16 copFireOnGreen;		// COP Fire On Green
extern int16 copFireOnBlue;			// COP Fire On Blue

extern int16 copMedOnRed;			// COP Medical On Red
extern int16 copMedOnGreen;			// COP Medical On Green
extern int16 copMedOnBlue;			// COP Medical On Blue

extern int16 copEmOnRed;			// COP Emergency On Red
extern int16 copEmOnGreen;			// COP Emergency On Green
extern int16 copEmOnBlue;			// COP Emergency On Blue

extern int16 copOTSOnRed;			// COP OTS On Red
extern int16 copOTSOnGreen;			// COP OTS On Green
extern int16 copOTSOnBlue;			// COP OTS On Blue


extern int16 ccOnBright_prev;		
extern int16 ccOffBright_prev;

extern int16 ccOnRed_prev;
extern int16 ccOnGreen_prev;
extern int16 ccOnBlue_prev;

extern int16 ccOffRed_prev;
extern int16 ccOffGreen_prev;
extern int16 ccOffBlue_prev;

extern int16 ccSecOnRed_prev;
extern int16 ccSecOnGreen_prev;
extern int16 ccSecOnBlue_prev;

extern int16 ccAttUpOnRed_prev;		
extern int16 ccAttUpOnGreen_prev;	
extern int16 ccAttUpOnBlue_prev;		

extern int16 ccAttDnOnRed_prev;		
extern int16 ccAttDnOnGreen_prev;	
extern int16 ccAttDnOnBlue_prev;		

extern int16 copFireOnRed_prev;		
extern int16 copFireOnGreen_prev;		
extern int16 copFireOnBlue_prev;		

extern int16 copMedOnRed_prev;		
extern int16 copMedOnGreen_prev;		
extern int16 copMedOnBlue_prev;		

extern int16 copEmOnRed_prev;			
extern int16 copEmOnGreen_prev;		
extern int16 copEmOnBlue_prev;		

extern int16 copOTSOnRed_prev;		
extern int16 copOTSOnGreen_prev;		
extern int16 copOTSOnBlue_prev;		

extern int16 CC_ColorOnOffChecksum;


extern int8 Drive_Type[car_size];			// Drive Type	1=hydro 2=invert 3=vector 4=gen 5=scr 6=vector open loop
extern int8 valid_cb_car[car_size]; 		// set to 1 for each valid code blue car
extern int8 valid_cbpr_car[car_size]; 		// set to 1 for each valid code blue primary riser car
extern int8 valid_cbsr_car[car_size]; 		// set to 1 for each valid code blue secondary riser car
extern int8 valid_ep_car[car_size]; 		// set to 1 for each valid emergency power car
extern int8 valid_vip_car[car_size]; 		// set to 1 for each valid vip car

//  #define fvcputiming 434			// Cpu timing output control

#define DIAGLED 		0
#define INCTIM 			0x0001
#define MAIN_1SEC 		0x4000
#define GRP_IO     		0x0002
#define TICK_10_MSEC 	0x0004
#define GRP_INCTIM 		0x0008

#define connmb 158
extern int16 cons[connmb];
extern int16 consB[connmb]; //  for use with board testing programs. // tagwbg 19
#define loadw 0         	// 000C - cons[loadw]	+1 = if load weighing switches (LWB LWA LWD etc)
							//						+2 = serial analog load weighing
							//						+4 = load weighing switch for LWB input only (load weighing bypass)
							//						+8 = load weigher board on machine room can bus (G4)
#define arst 1          	// 001C - cons[arst]=1 if automatic reset is enabled 0=no
#define att 2           	// 002C - cons[att]=1 if attendant operation with annunciator on att, 0=no, 2=annunciator in att + auto, 3=att without annunciator
#define behindc 3       	// 003C - cons[behindc]=1 if behind the car call cancel 0=no
#define bipolar 4       	// 004C - cons[bipolar]=1 if bipolar scurve 0 if unipolar
#define bottomf 5       	// 005C - cons[bottomf]= position of the bottom floor
#define carnmb 6        	// 006C - cons[carnmb] is the car number in the group
#define cblue 7         	// 007C - cons[cblue]=1 if code blue operation 0=no
							// 		  cons[cblue]=2 Code Blue inputs located on serial hall call aux inputs
#define dcpo 8          	// 008C - cons[dcpo]=1 if run with door close power 0=no
#define sercom 9   			// 009C - cons[sercom] 1= Serial Hall Call, 2=Serial Car, 4=CC in COP, 8=drive, 16=door, 32=hc drivers
#define dispis 10       	// 010C - cons[dispis]= 0 = No Binary PI, 1 if discrete position indicators, 
							//           		   +2 = Lobby lantern instead of ISER or RCM
							//           		    4 = Binary with OS flash Out of Service
							//           		    6 = Binary with Lobby Lanter and OS flash Out of Service
							//           		    8 = Use UDA and DDA on car for Att Up and Dn light
							//						16 = Use UDA and DDA on ATT but not on automatic
							//						32 = PI I/Os on 2nd car call board (used with RGB Car Call boards)
							//						64 = Output Radial Position Indicator
							//			 		  	128 = Binary PI in the car
#define equake 11       	// 011C - cons[equake]=1 if california earth quake 0=no
#define fax 12          	// 012C - cons[fax]=1 if fax modem installed
#define frghtd 13       	// 013C - cons[frghtd]: 0=no
							//				 		1=automatic power freight door  
							//				 		2=peele
							//				 		3=courion model E						
							//				 		4=ems,											
							//				 		5=courion model D						
							//				 		6=front automatic power freight door 						
							//				 		7=rear automatic power freight door 						
							//				 		8=front and rear  automatic power freight door 						
							//				 		9=Guilbert Freight Door						
							//				 		10=front automatic peelle door
							//				 		11=rear automatic peelle door
							//				 		12=front and rear automatic peelle door
#define home 14         	// 014C - cons[home] is the floor number for simplex homing
#define ansi 15      		// 015C - cons[ansi] = Ansi code compliance, 1996, 2000, etc --- Previously = cons[hydro] 1=hydro 2=invert 3=vector 4=gen 5=scr 6=vector open loop
#define incrs 16        	// 016C - cons[incrs]=1 if inconscpicous riser, 2=ir or'ed with std hall calls, 3=special ir for car op in group
#define mand 17         	// 017C - cons[mand]: 	1 = if manual doors 0 if automatic doors, 
							// 						2 = swing door w/power gate, 
							// 						3 = fixed cam special (deleted), 
							// 						4 = manual staggered rear/automatic front,
							// 						5 = swing w/pwr gate stggered rear/auto front
							// 						6 = Swing rear door with automatic front.
							//						7 = Automatic front/swing front and manual rear
							//						8 = Swing front/manual doors at staggered floor with manual rear
							//						9 = Guilbert Door Operator
#define modem 18        	// 018C - cons[modem]=1 if modem installed
#define nmbcars 19      	// 019C - cons[nmbcars] is the number of cars in the group
#define nodzdc 20       	// 020C - cons[nodzdc]=1 if canadian no door zone close door
#define firesv 21       	// 021C - cons[firesv]= fire service type: 0=Ansi 1996, 1=New York, 2=Ansi2000, 3=Chicago
#define otslbp 22       	// 022C - cons[otslbp]=1 out of service lobby park door open, 2 = cycle door closed, 4=Sec Em Recall (RTLDC) and (RTL)
#define predo 23        	// 023C - cons[predo]=1 if preopen doors on stop 0=no
#define rear 24         	// 024C - cons[rear]=1 if rear doors 0=no
#define epgrp 25 			// 025C - cons[epgrp]=car number if inter-group emergency operation; previously = cons[Encoder_PPM]
#define speed 26        	// 026C - cons[speed]= elevator speed in fpm
#define topf 27         	// 027C - cons[topf]= position of the top floor
#define brake 28        	// 028C - cons[brake] = 2 for electronic brake board, 3=3phase brake
#define grtopf 29       	// 029C - cons[grtopf]= the top floor of the group
#define BinPiIn 30      	// 030C - cons[BinPiIn] = 1 for binary input floor confirm
#define SecFlCfg 31			// 031C - cons[SecFlCfg] = 1 to set masks for security floor configuration, 2 = multiple floor mask security, 3=secure hall call per floor per car from keyswitch 
							// 		  cons[SecFlCfg] = 6 to set masks for hall/car calls security with keyswitch per car, 7=secure hall calls and car calls per floor per car from keyswitch
							// 		  cons[SecFlCfg] = 8 to enable SR car call security by direction and define unblocked car calls mask in the mask tables
#define hben 32				// 032C - cons[hben] = 1 to use enable button on car to hear handicap buzzer, 2 = beep when car call hit 4== beep on car call ONLY
#define exDHwL 33			// 033C - cons[exDHwL] = 1 extended door hold w/light and car call cancel door hold time      
#define medEm 34			// 034C - cons[medEm] = 1 Medical Emergency Service
#define rsbuffer 35			// 035C - cons[rsbuffer] = 1 Reduced stroke buffer, 0 = normal
#define naec 36				// 036C - cons[naec] = 1 Set for special show software
#define access_type 37 		// 037C - cons[access_type] = 0 No access, 1 = front top access, 2 front bottom, 4 = rear top, 8 = rear bottom
#define shortfl 38			// 038C - cons[shortfl] = 0 No short floor, # of bottom short floor
#define saf_test 39			// 039C - cons[saf_test] = 1 Shutdown if annual safety test flag not updated
#define ids 40				// 040C - cons[ids] = 1 Enable Lift-Net Protocol from IDS
#define LW_dis_sw 41 		// 041C - cons[LW_dis_sw] = 1 Load weigher disable switch
#define Elev_off 42			// 042C - cons[Elev_off]:	+1 = Elevator off switch in COP EOF, 
							//							+2 = Elevator Switch in Hall HEOF, 
							//							+4 = Elevator off reset EOR for HEOF,
							//							+8 = Elevator Off Output ELOO for HEOF,
#define Remote_CC 43		// 043C - cons[Remote_CC] = 1 Remote car call stations (car calls, DOB and DCB),
							//        cons[Remote_CC] = 2 = enable ED input instead of INDC to extend door dwell time on every floor
							//        cons[Remote_CC] = 3 enable ED input instead of INDC to extend door dwell time only at lobby floor
#define GLBP_LED 44			// 044C - cons[GLBP_LED] = 1 Rudy rule: output status of gate or lock bypass to LED mounted exernally.
#define Low_Press 45		// 045C - cons[Low_Press] = 1 Hydro Pressure Switch input (EMH)
#define Low_Oil 46			// 046C - cons[Low_Oil] = 1 Low Oil Switch	(COL)
#define MtrBlower 47		// 047C - cons[MtrBlower] = 1 Motor blower output (uses GEN)
#define Sabbath 48			// 048C - cons[Sabbath] = 1 Sabbath car calls, 2=cc set from gio, 3=Sabbath Hall calls 4 = car calls  5 = ir to ovride sabbath
							//	      cons[Sabbath] = 6 Sabbath cc mask set from gio with option to have up/down collective calls, 7 = Sabbath latch seq of car calls from sabbath tables. 
#define Car_Sw 49			// 049C - cons[Car_Sw] = 1 Car Switch
#define StagRear 50			// 050C - cons[StagRear] = 0, No staggered rear, 1 = staggered Rear floor flag
#define LbyUpReq 51			// 051C - cons[LbyUpReq] = 1 Lobby Up Hall Call Request (UC grtopf + 1), 1 in option but 5 in array = Lobby Up hall call but secf decides on standard hc assignment.
#define door_type 52		// 052C - cons[door_type] = 1 OVL/6970
#define housing	53			// 053C - cons[housing] = 1, NJ Housing Authority Job, 2 = NYCHA, 3 = Toronto housing
#define rearsel	54			// 054C - cons[rearsel] = 1, Rear door i/o on seperate board
#define EMPop	55			// 055C - cons[EMPop]: +1=Emergency Power Transfer, +2 = Hydro Emp Seq, +4 = Hydro S EPS, +8 Emp Status Indicators 
#define SFC_2004_IO	56		// 056C -  cons[SFC_2004_IO] == 1, SFC test (v3.06+) and 2004 I/O change: SS to CWS, FFS to SS and CTS.
#define sel_type 57			// 057C - cons[sel_type]: 1=Rail Target Selector/Governor Encoder, 2=Encoded Tape, 3=Rail Target/CPU Gov Enc
#define pbdoor	58			// 058C - cons[pbdoor] = 1, Momentary Push Button door operation, 2=Constant pressure pbdoor. 
#define DZDoorEn 59			// 059C - cons[DZDoorEn] = 1, Door Zone Door Enable output on EML
#define dpp_upd	60			// 060C - cons[dpp_upd] = 1, Dpp update and offset control if using dz interrupt (1038 j26 to +DZ) (1036 j6 to Ri1)
#define FSX_rear 61			// 061C - cons[FSX_rear] = 1, Special operation for FSX as the rear door fire key switch and FS is front
#define RCM_HWDoor 62		// 062C - cons[RCM_HWDoor] = 1, Retiring Cam to Close Hoistway Door, or with auto door
#define vshort_tfl 63 		// 063C - cons[vshort_tfl] = 0 No very short floor, # of top very short floor (DTR used to switch door zone inputs)
#define Reset_Jack 64		// 064C - cons[Reset_Jack] = 1, home and reset jack once a day
#define Micro_Relev 65		// 065C - cons[Micro_Relev] = 1, micro releveling using UPML and DNML outputs.
#define Auto_Swing_DO 66	// 066C - cons[Auto_Swing_DO] = 1, automatic swing door w/manual gate
						 	// 		  cons[Auto_Swing_DO] = 2, automatic swing door set auto door output after delay
#define Park_NCU_ALB 67		// 067C - cons[Park_NCU_ALB] = 1, Inputs for APRK, ALBY, NCU 
#define Handicap_HPB 68		// 068C - cons[Handicap_HPB] = 1, Input for handicap car request
#define F1_CT 69			// 069C - cons[F1CT] = 1, FB, FL on 1040 Car Top
#define F1_ML_AL 70			// 070C - cons[F1_ML_AL] = 1, F1ML replaces EQL, F1AL replaces EML
#define shortflmid 71		// 071C - cons[shortflmid] = 1, Second short floor in middle of hoistway
#define EM_BRK 72			// 072C - cons[EM_BRK] = 1 Secondary Emergency Brake, 2=Sheave brake, 3=Sheave brake 2007, 
							// 						 4=Secondary Em Brake OR Sheave brake 2007 pick and drop each run, 5=Sheave Brake seperate BC and UTM.
							// 						 6=Electronic emergency brake
#define heavy_door 73		// 073C - cons[heavy_door] = 1 heavy door operation.
#define stag_auto_door 74	// 074C - cons[stag_auto_door] = 1 for staggered auto/swing door (cons[mand] == 7)
#define CBCar 75			// 075C - cons[CBCar] = 1, Valid code blue car set in array in cons file.
#define Car_Asgn_Fl 76		// 076C - cons[Car_Asgn_Fl] = 1, Car assigned to particular floor in cons floor array.
#define EPCarRun 77			// 077C - cons[EPCarRun] = 1, Mask defines particular cars to run on EP.
#define Second_Riser 78		// 078C - cons[Second_Riser] = 1, Dispatch call from Second Riser to multiple cars
							// 		  cons[Second_Riser] = 2, Dispatch Second Riser simplex or duplex operation with ICR and ICR2 inputs and car selection with fvars[fvircar], fvars[fvircar2]
							// 		  cons[Second_Riser] = 3, Second Riser Cycling Operation with CYCLE input located in COP
#define VipSvc 79			// 079C - cons[VipSvc] = 1 Priority Service (vip) flag
							// 		  cons[VipSvc] = 2 Wheel Chair Passenger VIP Service
							// 		  consp[VipSvc] = 3 Map vip service labels on group car call security boards
							// 		  cons[VipSvc] = 4 Vip io are located on aux inputs of serial hc boards
#define IndSec 80			// 080C - cons[IndSec] = 1 Independent Security Service
#define hugs 81				// 081C - cons[hugs] 	== 1, Hugs Security Service in Group Operation HCS; 
							//						== 2, Hugs Security Service in Group Operation GCS;
							//						== 3, Hugs security Service in Car Operation
#define GOV_Reset 82		// 082C - cons[GOV_Reset] == 1, Electronic governor reset
#define cc_pb_sec 83		// 083C - cons[cc_pb_sec] == 1, car call push button security
#define loc_svc_tmr 84		// 084C - cons[loc_svc_tmr] == 1, Run service timer for non dispatcher car
#define Emp_Recover 85		// 085C - cons[Emp_Recover] == 1, Traction with battery recovery, 2 = single phase ups
#define Cycle_Run 86		// 086C - cons[Cycle_Run] == 1, Cycle car after sitting for cycle run hours (adjustable timer)
#define HCP_Fuse 87			// 087C - cons[HCP_Fuse] == 1, Seperate hall call fuse split from fire service hall power
#define Smoke_2004 88		// 088C - cons[Smoke_2004] == 1, for HWS2 Input, cons[Smoke_2004] == 2 only inputs but no comm. previously cons[Dual_Ser_Riser].
#define PWL_Brake 89		// 089C - cons[PWL_Brake] == 1, Manual Power Loss Brake, 2=MBC control only
#define INS_EN_SFC 90		// 090C - cons[INS_EN_SFC] == 1, not used on GALaxy 4 (always enabled)
#define FS2_SW 91			// 091C - cons[FS2_SW] == 1, Reverse FS2 switch logic
#define Voltage_Sensor 92	// 092C - Cons[Voltage_Sensor] == 1, Analog voltage sensor to detect phase loss and low voltage (3-phase + door)
#define Galileo 93			// 093C - cons[Galileo] == 1, Enable control function of Galileo
#define EP_Profile 94		// 094C - cons[EP_Profile] +1, Use Emergency Power Speed profile parameters on EP
							// 		  cons[EP_Profile] +2, Enables RSPD input to switch Speed profile parameters
#define UPS_Type 95			// 095C - cons[UPS_Type], 0 for APC, 1 for Eaton (used for serial protocol)
#define RGB_CC 96			// 096C - cons[RGB_CC], 1 = RBG Car call boards
#define aux_COP 97			// 097C - Previously used as cons[aux_COP] = 1.  Now used as cons[aux_COP], 1 = Front OR Rear COP, 2= Aux Front Only COP, 4 = Aux Rear Only, 3,5,6 or 7 = both (used for standard or RGB car calls)
#define FSautoRST 98		// 098C - cons[FSautoRST], 1 = FS Auto reset operation
#define Aux_Brake 99		// 099C - cons[Aux_Brake], 1 = Aux Electronic Brake follows normal electronic brake 
#define SAB_Fl 100			// 100C - cons[SAB_Fl] = 1, use floors from cons file table (if cons[Sabbath] == 1) use car call, (if cons[Sabbath] == 3) use hall calls
#define XA_Mask 101			// 101C - Cross-Assignment Mask for which group floors are assigned.  1=GAL, 2=Other, 3=both
#define Alt_Ascii_pi 102 	// 102C - enable alternate ascii pi's
#define Xref_PI_Pos 103		// 103C - Cross Reference PI (Use a different number for the PI position at the floor)
#define Dual_Ser_Riser 104	// 104C - cons[Dual_Ser_Riser] == 1, Dual Serial Risers
#define Blind_Floor 105 	// 105C - cons[Blind_Floor] == Floor, floor above the blind shaft to display blind shaft character on digital PI indicator.
#define Blind_Floor_2 106 	// 106C - cons[Blind_Floor_2] == Floor, floor below the blind shaft to display blind shaft character on digital PI indicator.
#define SecFlCfg_2 107		// 107C - cons[SecFlCfg_2] = 1 to set masks for security floor configuration, cons[SecFlCfg] must be set to 2 
#define SecFlCfg_3 108		// 108C - cons[SecFlCfg_3] = 1 to set masks for security floor configuration, cons[SecFlCfg] must be set to 2 
#define SecFlCfg_4 109		// 109C - cons[SecFlCfg_4] = 1 to set masks for security floor configuration, cons[SecFlCfg] must be set to 2 
#define CB_msk 110			// 110C - cons[CB_msk] = 1 use floor mask for CB valid floors
#define FI_EP_Ext 111		// 111C - cons[FI_EP_Ext] = 1 when 24 Volt Hall calls used	or for external Fire and Emp i/o	
#define Prison 112			// 112C - cons[Prison] = 1. Special service for REMOTE COP as well as emergency recall, 2 = Riot Control
#define Super_Sabbath 113	// 113C - cons[Super_Sabbath] will enable the ouputs for SABUZ and SABEE for special Sabbath operation
#define incarvip 114		// 114C - cons[incarvip] = 1. Special VIP feature, allows car to go into VIP service with an input called ICVIP located into COP 1st car call board  
#define exDHwLatFL 115		// 115C - cons[exDHwLatFL] = 1 extended door hold w/light and car call cancel door hold time for one floor only (fvar) 
#define distress 116		// 116C - cons[distress] = 1 enables osrL light output osrB buzzer output and osrA acknowledge input on external IO baord.
							// 		  cons[distress] = 2 enables OSERL out of service light output, OSERB emergency stop buzzer output, OSERD elev door open fault output
#define Australia 117		// 117C - cons[Australia] = 1 enables special code and i/o's for Australia
#define TUG 118				// 118C - cons[TUG] = 1 enables I/O interface and operation for TUG Hospital Robots
#define Ser_IGEP 119		// 119C - cons[Ser_IGEP] = 1 Serial Intergroup Emergency Power
#define dflt_MOVFR 120		// 120C - cons[dflt_MOVFR] + 1 EE in TOC, +2 mirrors DO,DC,NUD to original locations, +4 disable ee reopen from movfr
#define MR_FBZ 121			// 121C - cons[MR_FBZ] = 1, Fire buzzer in the machine room.
#define MR_INE 122			// 122C - cons[MR_INE] = 1,2,3 Machine Room inspection enable input used, always enable on GALaxy 4
#define EM_BKS 123			// 123C - cons[EM_BKS] = 1,2,3 Seperate brake input (each value for a different location on Main I/O board) 
							//     	  Not needed when cons[EM_BRK] equals 2 or 3. When cons[EM_BRK] = 5, cons[EM_BKS] must be non zero to use BKS input on board 27
#define zoneSW 124			// 124C - cons[zoneSW] = enables ZON input inside car station to allow the car to zone back to lobby when no calls are on
#define hotoilSW 125		// 125C - cons[hotoilSW] = 1 enables hot oil switch. Feature will only work on hydros.
#define servOUT 126			// 126C - cons[servOUT] = Service Message indicator +1= in COP +2 = Machine room, +4 = ISER output, +8 = OSERL, +16 Group Service output
#define enctype 127			// 127C - cons[enctype] = 0-Turck, 1-Dynapar, 2-Wachendorff
#define vip_en 128			// 128C - cons[vip_en] = +1. to have VIPEN input to enable/disable making all VIP calls (works similar to SECUR for ccs)
							// 		  cons[vip_en] = +2. to have VPECC input in TOC to enable latching car calls when at vip floor doors open on group VIP service
#define svc_tmr_en 129		// 129C - cons[svc_tmr_en] = 1 for enabling service timers enable input on the external io board
#define openlout 130		// 130C - cons[openlout] = 1 for putting the OPENL output on the Fire Service board,
							// 		  cons[openlout] = 2 for putting the OPENL output on EQL main IO
							// 		  cons[openlout] = 3 for putting the OPENL output on 3DA main IO
#define CB_SR 131			// 131C - cons[CB_SR] = 1 for code blue second riser
#define EPSF 132			// 132C - cons[EPSF] = 1 for Emergency Power Split Feeder
#define VIP_msk 133			// 133C - cons[VIP_msk] = 1 use floor mask for VIP valid floors
#define Hall_Lant 134		// 134C - cons[Hall_Lant] = +1 = add hall lanterns, 
							//				     		+2 = discrete PI's in hall
							//					 		+4 = Next Up Lantern at lobby (TCU)
							//                   		+128 = binary PI's in hall
#define CCS_BUS   135		// 135C - cons[CCS_BUS]  0 = MRCAN enable MRCAN inputs 
							//				  		 1 = CTCAN enable CTCAN inputs (push enable and inputs to CTCAN )
							//				  		 2 = CTCAN enable MRCAN inputs (push enable 			to CTCAN )
							//				  		 4 = MRCAN enable CTCAN inputs (push inputs 			to CTCAN )
#define IC_INE 136			// 136C - cons[IC_INE] = 1, use DCB as enable input for In Car Inspection mode, always enable on GALaxy 4
#define smoke_interface 137	// 137C - cons[smoke_interface], +1 = enables reading smoke sensor input per floor and provide ALTO and MESO outputs
							//					      +2 = disable hospital service over fire service if a call is made on the floor where smoke sensor was activated.
#define FiCntrlCentr 138  	// 138C - cons[FiCntrlCentr] = 1 add outputs for fire control center
#define sec_dis 139			// 139C - cons[sec_dis], +1 = disable car call security at lobby floor with a SCD pushbutton input for fvars[fvsecdist] amount of time
#define Intf_Voltage 140 	// 140C - cons[Intf_Voltage]: 0 = 24, 1 = 120Vac, bit 0: Lantern voltage, bit 1: HC Voltage, bit 2: FEP Voltage, bit 3: Car Call Voltage, bit 4: Discrete PI Volt, 
#define ep_auto_en 141		// 141C - cons[ep_auto_en] = 1, enables AUTEP input for allowing auto recalling and automatically selecting a car on emergency power service
#define Hardware_Config 142	// 142C - cons[Hardware_Config]: Bit 0 = 1 DET not on COP
#define fire_smoke_nc 143	// 143C - cons[fire_smoke_nc] = 1, all smoke detector inputs are normally closed contacts
#define eetst_dis 144		// 144C - cons[eetst_dis] = 1, disable ee test when using peele auto door interface
#define Car_Elev_off 145 	// 145C - cons[Car_Elev_off]:	+1 = Elevator Switch in Car CEOF, 
							//								+2 = Elevator off reset CEOFR for CEOF,
							//								+4 = Elevator Off OutputCEOFL for CEOF,
#define Unused146 146
#define Unused147 147
#define Unused148 148
#define Unused149 149
#define Unused150 150
#define Unused151 151
#define Unused152 152
#define Unused153 153

#define Third_Riser 154		// 154C - cons[Third_Riser] = 1, Dispatch calls from Third Riser to multiple cars, uses rear IR boards
#define EPSEL_tables 155	// 155C - cons[EPSEL_tables] = 1 use these tables to decide order and number of cars to recall and select
#define Dest_Disp 156	  	// 156C - cons[Dest_Disp] = 1, destination dispatch group
#define pbdoormsk 157		// 157C - cons[pbdoormsk] = 1, enable constant pressure pbdoor for floors with the mask set. 


// Field adjustable variables
extern unsigned char telnmb[20];
#define nmbfvar 600
extern int16 fvars[nmbfvar];
#define fvydel 0        		// 000 - Y Delta Time in 1/10 sec
#define fvfltt 1        		// 001 - Fault Time in 1/10 sec
#define fvrst 2         		// 002 - Reset Time in 1/10 sec
#define fvdbl 3         		// 003 - Double Stroke on Lanterns: 0=disable, 1=enable
#define fvlnton 4       		// 004 - Lantern On Time for Double Stroking in 1/10 sec
#define fvlntof 5       		// 005 - Lantern Off Time for Double Stroking in 1/10 sec
#define fvhbept 6       		// 006 - Handicap Floor Passing Beep Time in 1/10 sec
#define fvdpt 7         		// 007 - Door Protection Time in 1/10 sec
#define fvndt 8         		// 008 - Nudge Time in 1/10 sec
#define fvpred 9        		// 009 - Pre-opening Delay Time in 1/10 sec
#define fvccdt 10       		// 010 - Car Call Door Dwell Time in 1/10 sec
#define fvhcdt 11       		// 011 - Hall Call Door Dwell Time in 1/10 sec
#define fvldt 12        		// 012 - Lobby Call Door Dwell Time in 1/10 sec
#define fvexdt 13       		// 013 - Extra Door Dwell Time in 1/10 sec
#define fvnit 14        		// 014 - Non Interference Time in 1/10 sec
#define fvlob 15        		// 015 - Lobby Floor
#define fvfs1f 16       		// 016 - Fire Service Phase 1 Main Egress Floor
#define fvfs1af 17      		// 017 - Fire Service Phase 1 Alternate Egress Floor
#define fvstall 18      		// 018 - Stall Time in 1/10 sec
#define fvsst 19        		// 019 - Soft Stop Time in 1/10 sec
#define fvgrt 20        		// 020 - Generator Run Time in 1/10 sec
#define fvbrkpickdly 21    		// 021 - Brake Pick Delay Time in 1/10 sec
#define fvbrkdropdly 22        	// 022 - Brake Drop Delay Time in 1/10 sec
#define fvjrk 23        		// 023 - Soft Start Jerk Rate in fpm per second squared
#define fvacc 24        		// 024 - Acceleration Rate in fpm per second
#define fvdjrk 25       		// 025 - Deceleration Jerk Rate in fpm per second squared
#define fvdec 26        		// 026 - Deceleration Rate in fpm per second
#define fvm7dst 27      		// 027 - Floor Target Distance in 1/10 inch
#define fvlvs 28        		// 028 - Leveling Speed in 1/10 fpm
#define fvrlvs 29       		// 029 - Relevel Speed in 1/10 fpm
#define fvins 30        		// 030 - Inspection Speed in fpm
#define fvyear 31       		// 031 - Year
#define fvmon 32        		// 032 - Month
#define fvday 33        		// 033 - Day
#define fvhour 34       		// 034 - Hour
#define fvmin 35        		// 035 - Minute
#define fvjrk3 36       		// 036 - Roll Over Jerk Rate in fpm per second squared
#define fveemem 37       		// 037 - EE Memory Size: 0=32K, 1=8K (previously diagnostic led's 0=disable, 1=enable)
#define fvspt_year 38   		// 038 - Annual Safety Test Year
#define fvspt_mon 39    		// 039 - Annual Safety Test Month
#define fvspt_day 40    		// 040 - Annual Safety Test Day
#define fvpat_del 41    		// 041 - Pattern Delay in 1/100 sec
#define fvfweak 42				// 042 - Field Weaken Velocity Percent
#define fvencppr 43				// 043 - Encoder PPR
#define fvencrpm 44				// 044 - Encoder RPM
#define fvbaud 45				// 045 - User Interface Baud Rate: 0=2400,  1=4800,   2=9600,   3=19200
#define fvpark 46				// 046 - Parking: 0=disable, 1-8 number of cars to park
#define fvepfl 47				// 047 - Emergency Power Return Floor
#define fvepcars 48				// 048 - Number of Cars to Run on Emergency Power
#define fvretcar 49				// 049 - First Car to Return on Emergency Power
#define fvselcar 50				// 050 - First Car Selected on Emergency Power
#define fvrettim 51				// 051 - Return Car Time-out Time in 1/10 sec
#define fvdobndg 52				// 052 - DOB Opens Door On Nudging Is Set
#define fvemdisp 53				// 053 - Emergency Dispatch Enable:		1=ON, 2=Front, 3=Rear
#define fvlbyreq 54				// 054 - Number of Cars Requested at Lobby
#define fvncu 55				// 055 - Next Car Up Selection: 0=OFF, 1=NCU DO, 2=NCU DC, 4=NCU up peak
#define fvcbcar 56				// 056 - Code Blue Car Select
#define fvircar 57				// 057 - IR Car Select
#define fvbkliftsw 58			// 058 - Brake Lift Switch: 0=disable, 1=enable, 2=set gripper on fault
#define fvlwa 59				// 059 - Load Weighing Anti-nuisance Count (0=disabled)
#define fvfiresw 60				// 060 - Fire Switch Location: 0=front, 1=main rear, 2=alt rear, 3=main/alt rear
#define fvetamin 61				// 061 - ETA Minimum Time
#define fvetacocc 62			// 062 - ETA for Coincident Car Call
#define fvvidc 62				// 062 - Dummy for Video Car Position Index
#define fvvidc1 63				// 063 - Video Position for Car 1
#define fvvidc2 64				// 064 - Video Position for Car 2
#define fvvidc3 65				// 065 - Video Position for Car 3
#define fvvidc4 66				// 066 - Video Position for Car 4
#define fvvidc5 67				// 067 - Video Position for Car 5
#define fvvidc6 68				// 068 - Video Position for Car 6
#define fvrunct 69				// 069 - Run Count with No Passengers to Cancel Car Calls
#define fvindsec 70				// 070 - Independent Override Security: +1=overrides CCS security
								//										+2=overrides SECF
								//										+4=overrides car call station
#define fvbksrel 71				// 071 - Brake Pick Delay Time on Relevel
#define fvbkcrel 72				// 072 - Brake Cool on Relevel: 0=std brake cool none
								// 								1=use brake cool immediately (produce partial brake lift)
#define fvuptrigtim 73			// 073 - Up peak Trigger Time in 1/10 sec
#define fvuptrigcnt 74			// 074 - Up Peak Trigger Count
#define fvupcccnt 75			// 075 - Up Peak Car Call Count
#define fvupdurtim 76			// 076 - Up Peak Duration Time in 1/10 sec
#define fvdptrigtim 77			// 077 - Down Peak Trigger Time in 1/10 sec
#define fvdptrigcnt 78			// 078 - Down Peak Trigger Count
#define fvdpdurtim 79 			// 079 - Down Peak Duration Time in 1/10 sec
#define fvparkdly 80			// 080 - Parking Delay Time in 1/10 sec
#define fvxasgn 81				// 081 - Hall Call Cross Assignment: 0=0FF, 1=xasgn, 2=xcanc
#define fvxeta 82				// 082 - Hall Call Cross Assignment ETA
#define fvmedfl 83				// 083 - Medical Emergency Floor
#define fvmedloc 84				// 084 - Medical Emergency Hall Switch location: 0=front, 1=rear
#define fvfmrsen 85				// 085 - Fire Service Motor Room Sensor Return Floor: 0=main, 1=alt 
#define fvfhwsen 86				// 086 - Fire Service Hoistway Sensor Return Floor: 0=main, 1=alt 
#define fvfire_rst 87			// 087 - Phase 1 Fire Recall Reset:  0=reset after ph1 sequence (recall)
								//									 1=reset immediately
								//									+2=FS only reset from OFF
#define fvacclim 88				// 088 - Acceleration and Deceleration Limit at Final Stop
#define fvpatrel 89				// 089 - Pattern Delay During Relevel in 1/100 sec
#define fvdon 90				// 090 - Delay Start Until Drive On:	0=Immediate, 1=after DON
#define fvoptodly 91			// 091 - Brake Opto Trigger Delay
								//		  Previously Dynamic Brake Temperature Trip point degrees Fahrenheit
#define fvbls 92				// 092 - Invert Logic of Brake Lift Switch: 0=Brake lift switch normally open
								//											1=Brake lift switch normally closed
#define fviser 93				// 093 - Invert Logic of In-service Light Output or In-use Light or Out of Service Alarm:
								//														 0=ISER on when in service
								//														 1=ISER on when out of service
								//														 2=IN USE LIGHT
								//														 4=OTS
#define fvcmaster 94    		// 094 - Ability to Change a Car Other Than 1 or 2 to Become the Master
#define fvtovideo 95    		// 095 - Time-out Video Display: 0=do not time out
								//								 (if set to non-zero then time-out video display)
#define fvdcinsp 96				// 096 - Close Doors While Trying to Run on Inspection
								//		  0=do not turn on DC output while trying to run on inspection
								//		  1=turn on DC output while trying to run on inspection
#define fvparkfl1 97			// 097 - Parking Floor 1
#define fvparkfl2 98			// 098 - Parking Floor 2
#define fvparkfl3 99			// 099 - Parking Floor 3
#define fvparkfl4 100			// 100 - Parking Floor 4
#define fvparkfl5 101			// 101 - Parking Floor 5
#define fvparkwidth 102			// 102 - Parking Width
#define fvbalance 103			// 103 - Percent of Load to Balance the Counterweights (Balanced Load Percent)
#define fvtorque 104			// 104 - Drive Starting Torque Multiplier (Torque Amount Variable)
#define fvloadbp 105 			// 105 - Load Bypass Percent: 0%=disabled
#define fvloadan 106			// 106 - Load Anti-nuisance Percent: 0%=disabled
#define fvloaddis 107 			// 107 - Load Dispatch Percent: 0%=disabled
#define fvloadovl 108			// 108 - Load Overload Percent: 0%=disabled
#define fvirctl 109				// 109 - IR Control Flag: +1=IR enable
								//						  +2=finish car call then answer IR call
								//						  +4=finish car call before going off IR call
#define fvbkpick 110			// 110 - Brake Pick Voltage
#define fvbkpktm 111			// 111 - Brake Pick Duration Time
#define fvbkhold 112			// 112 - Brake Hold Voltage
#define fvbkrelv 113			// 113 - Brake Relevel Voltage
#define fvbkaclv 114			// 114 - Brake AC Line to Line Voltage
#define fvbkres  115			// 115 - Brake Resistance
#define fvattbz  116  			// 116 - Attendant Buzzer Delay Time
#define fvfsx 	 117			// 117 - Hall Fire Aux. Switch Enable: 0=no sw, 1=switch used
#define fvfsofl	 118			// 118 - Operation of Hall Fire Light: 0=F1&2, 1=F1, +2=BLK, 4=FSL
#define fvrmcc	 119			// 119 - COP/Remote CC Select:  0=both
								//							    1=Separate
								//							   +2=COP->Remote
								//							   +4=Remote->COP Ack Light
#define fvdrdly	 120			// 120 - Door Open or Close Delay Time
#define fvsecrec 121			// 121 - Security Recall Option: +1=recall to lobby
								//							     +2=cycle door at lobby
								//							     +4=rear door
								//							     +8=Always Recall
#define fvdobsec 122			// 122 - DOB Overrides Security: 1=CC Security
								//							     2=SECF uses normal masks
								//							     3=CCS Front Only
								//							     4=CCS Rear Only
								//							     5=SECF only in IND
#define fvdoorbz 123			// 123 - Manual Door Buzzer Delay Time
#define fvsecfl  124			// 124 - Security Floor
#define fvrcpkdly 125   		// 125 - Retiring Cam Pick Delay Time
#define fvshortdt 126			// 126 - Short Door Time From a Reopen Signal
#define fvircar2  127			// 127 - Second IR Car Select
#define fvstplby 128			// 128 - Stop At Lobby: +1=going up
								//					    +2=going down
								//					    +4=Onward HC too
								//					    +8=Return to lobby after hall call
#define fvclf 	 129			// 129 - Invert Cab Light Fan Output
#define fvtpl	 130			// 130 - Invert Temperature Low Switch (TPL)
#define fvlps	 131			// 131 - Invert Low Pressure Switch
#define fvlos	 132			// 132 - Invert Low Oil Switch
#define fvnudowc 133			// 133 - Nudging with No Onward Call
#define fvfireop 134			// 134 - Fire Option Flag: +1=disable blink on FS Key
								//						   +2=Turn Buzzer off at recall floor
#define fvattcc  135			// 135 - Attendant Car Call Flag: +1=set car call when hall call set
								//								  +2=set hall call in opposite opening when hc set
#define fvcbtim  136			// 136 - Code Blue Door Open Time
#define fvcbind	 137			// 137 - Request Ind Car For Code Blue
#define fvcblight 138			// 138 - Code Blue Light Control (Flash Light)
#define fvhsvdccc 139			// 139 - Hospital Service Door Close on Car Call
#define fvgriprst 140			// 140 - Gripper Reset Time(r)
#define fvinddccc 141			// 141 - Independent Door Close on Car Call
#define fvtorqoff 142			// 142 - Torque Offset (compensate for no compensating chains).
#define fvspbgrip 143  			// 143 - Gripper Test From Safety Processor Board:	+1=SPB, +2=overspeed
#define fvsapb 144				// 144 - Single Automatic Pushbutton Control: 1=enable
								//											  2=disable
								//											  4=allows one car call door open
#define fvdonodol 145			// 145 - Door Open When DOL Signal Not Active
#define fvunused146	146			// 146 - Previously fvdisglt - Disable Gate and Lock Test for New York City
#define fvrlvst  147    		// 147 - Relevel Start Speed in 1/10 fpm
#define fvemdec  148			// 148 - Emergency Deceleration Rate in fpm per sec
#define fvdcbdt  149 			// 149 - Disable DCB from Shortening Door Time
#define fvrtl 150				// 150 - Return to Lobby Option: +1=cycle door
								//							     +2=cancel car calls
								//							     +4=cycle door on reversal
#define fvvidc7 151				// 151 - Video Position for Car 7
#define fvvidc8 152				// 152 - Video Position for Car 8
#define fvparktype 153			// 153 - Parking Type: 0=busy floor park, 1=divide hoistway, 2=fixed park with adj variable
#define fvparkfl6 154			// 154 - Parking Floor 6
#define fvparkfl7 155			// 155 - Parking Floor 7
#define fvno_simul_do 156 		// 156 - Non-Simultaneous Doors Operation: 1=front first, 2=rear first
#define fvpredo 157				// 157 - Pre-open Doors:  0=no preopen
								//					      1=default
								//					     +2=no preopen doors short floor
#define fvovrspd 158			// 158 - Over Speed Percent
#define fvppupd 159				// 159 - Position Pulse Count Update: +1=enable
								//									  +2=use dz int
								//									  +4=constant velocity
#define fvppdly 160				// 160 - Position Pulse Delay Error
#define fvgripbz 161			// 161 - Gripper Buzzer Enable: 1=enable
#define fvrlvtim 162			// 162 - Relevel Delay Time
#define fvfireop2 163			// 163 - Fire Option 2: +1=ph2 recall open door first
								//					    +2=disable blink on ph2
#define fvclgate 164			// 164 - Close Gate on Swing Door with No Preference
#define fvlevfltcnt 165			// 165 - Leveling Fault Count
#define fvtorqdn 166			// 166 - Torque Amount in Down Direction (if non-zero, fvtorque is for up direction)
#define fvtrqrmptim 167			// 167 - Ramp Torque Time During Pattern Delay
#define fveeto 168				// 168 - Electric Eye Time-out Time
#define fvotshc 169 			// 169 - Cars Out of Service Hall Call Cancel; 1=enable
#define fvndgctl 170			// 170 - Nudging Control: 1=Disable Nudging Output
								//						 +2=reopen door on ee or se
								//						 +4=buzz blocked door 
#define fvfs1dto 171 			// 171 - Fire Phase 1 Door Time-out (Ind and Att)
#define fvnohcreop 172			// 172 - No Hall Call Button Door Reopen with Onward Call: 1=always on, 2=on up peak
#define fvdrvrst 173			// 173 - Drive Ready Reset Count
#define fvrtldt 174				// 174 - Return to Lobby Door Dwell Time
#define fvncul 175				// 175 - Next Up Lobby Lantern Control: 1=turn off hall lantern
								//									    2=turn off cab lantern
#define fvncupref 176			// 176 - Next Up Preference Control: 0=keep pref until door closed (DCL)
								//								     1=after NCU time
#define fvsrctl 177				// 177 - Second Riser Control: 1=no std HC assigned to SR Cars, 2=Or SR with std riser
#define fvsrlant 178			// 178 - Second Riser Lantern Control: 1=Cab Lant on SR
#define fvatt_eta_pref 179		// 179 - Attendant ETA Preference Time
#define fvhbz_nvfl 180			// 180 - Handicap Buzzer Disabled at Invalid Floor	 
#define fvviptim 181 			// 181 - VIP Door Open Time
#define fvvipop 182				// 182 - VIP Operation Flags: +1=Cancel HC if no cars
								//							  +2=Cancel car calls 1st
#define fvnmbvipcars 183 		// 183 - Number of VIP cars
#define fvepopled 184			// 184 - Emergency Power Op led (OP) Function: 0=Operational on EmPwr
								//											 : 1=normal pwr
								//											 : 2=emPower
								//											 : 3=ep recall
#define fveppkled 185			// 185 - Emergency Power Park led (PRK) Function: 0=Parked, 1=Recalled
#define fvheloff 186			// 186 - Hall Elevator Off Options: +1=Recall
								//								    +2=Keep Door Open
								//								    +4=Turn off CLF
								//									+8=Leave Door Close
#define fvdrvbaud 187			// 187 - Drive Baud Rate: 0=19200, 1=38400, 2=57600, 3=115200, 4=9600
#define fvdrvupd 188			// 188 - Drive Update Rate: 0=10 msec, 1=15 msec, 2=20 msec 
#define fvaltlob 189			// 189 - Alternate Lobby Floor
#define fvaltpark 190			// 190 - Alternate Parking Floor
#define fvlrqctl 191			// 191 - Lobby Request Control: 1=NCU
#define fvhandcap 192			// 192 - Handicap Car Capacity
#define fvfrtdcto 193			// 193 - Freight Door Close Time-out
#define fvhandwait 194			// 194 - Handicap Car Wait Time
#define fvshortdis 195			// 195 - Short Floor Distance in 1/10 inch
#define fvrcmctl 196			// 196 - Retiring Cam Control: +1=Hold retiring cam if door pilot at the floor
								//							 : +2=advance auto door
#define fvrecallfs1a 197 		// 197 - Recall from Fire Alternate Floor
#define fvhwstall 198			// 198 - Auto Learn Hoistway Stall Time
#define fvpperror 199 			// 199 - Position Pulse Count Error
#define fvattbzoff 200			// 200 - Attendant Buzzer Off Time 
#define fvattbzon 201			// 201 - Attendant Buzzer On Time
#define fvbkpkrate 202			// 202 - Brake Pick Voltage Rate Time
#define fvbkrvrate 203			// 203 - Brake Relevel Voltage Rate Time
#define fvbkpkst 204			// 204 - Brake Pick Start Voltage
#define fvbkrvst 205			// 205 - Brake Relevel Start Voltage
#define fvshort_dsdt 206  		// 206 - Short Floor Down Slowdown Time
#define fvshortm_dsdt 207 		// 207 - Mid Short Floor Down Slowdown Time
#define fvshort_usdt 208  		// 208 - Short Floor Up Slowdown Time
#define fvshortm_usdt 209 		// 209 - Mid Short Floor Up Slowdown Time
#define fvheoffl 210			// 210 - Hall Switch Elevator Off Return Floor
#define fvsvcmsg1 211			// 211 - Service Message 1 display
#define fvsvcmsg2 212			// 212 - Service Message 2 display
#define fvgovgrip 213			// 213 - Governor Gripper Trip Control: 0=gripper trip on GOV
								//									    1=no gripper trip on GOV
#define fvjrk_sf 214    		// 214 - Short Floor Soft Start Rate in fpm per second squared
#define fvacc_sf 215    		// 215 - Short Floor Acceleration Rate in fpm per second
#define fvjrk3_sf 216   		// 216 - Short Floor Roll Over Jerk Rate in fpm per second squared
#define fvdjrk_sf 217   		// 217 - Short Floor Deceleration Jerk Rate in fpm per second squared
#define fvdec_sf 218    		// 218 - Short Floor Deceleration Rate in fpm per second
#define fvm7dst_sf 219  		// 219 - Short Floor Target Distance
#define fvshrtflctl 220			// 220 - Short Floor Control: 1=very short floor run
								//							  2=short fl sd between fl
								//							  4=short mid fl sd between fl
#define fvsecrec2 221			// 221 - Security Recall Control 2: 0=out of group on first recall
								//								    1=OoG all recalls
								//								    2=no out of group recalls
#define fveprcldly 222			// 222 - Emergency Power Recall Delay Time
#define fvgccsect 223			// 223 - Group Car Call Security Override Time
#define fvebkdropdly 224		// 224 - Emergency Brake Drop Delay
#define fvebkpktim 225			// 225 - Emergency Brake Pick Time
#define fvhcbonbright 226 		// 226 - Hall Call Output On Brightness for led (higher number is brighter)
#define fvhcboffbright 227		// 227 - Hall Call Output Off Brightness for led (higher number is brighter)
#define fvacctopfl 228			// 228 - Access Top Floor
#define fvaccbotfl 229			// 229 - Access Bottom Floor
#define fvppstop 230			// 230 - Stop On Position Pulse
#define fvsabtim 231			// 231 - Sabbath Svc Restart Time
#define fvpwrdotim 232			// 232 - Power Door Open Relay On Time
#define fvfltdpy   233	 		// 233 - Display Fault Automatically on LCD when Fault Occurs	
#define fvpassword 234   		// 234 - Password for Adjusting Field Variables
#define fvpwtime 235			// 235 - Password Time-out Time
#define fvbehindcc 236			// 236 - Behind Car Call Cancel
#define fvdo_adv_de 237			// 237 - Door Open Advance Door Enable Non-Simultaneous Doors
#define fvgsvcpark 238			// 238 - Group Service Number of Parking Cars
#define fvpresetupd 239			// 239 - Binary Preset Update when Position Does Not Match
#define fvsfnohsv 240			// 240 - Short Floor hsf with No hsv High Speed Valve
#define fvuppkctl 241			// 241 - Up Peak Control: +1=Heavy Up
#define fvuppkpool 242			// 242 - Up Peak Pool: Cars taken out
#define fvdnpkctl 243			// 243 - Down Peak Control: +1=Heavy Down
#define fvdnpkpool 244			// 244 - Down Peak Pool: Cars taken out
#define fvxacars 245			// 245 - Cross Assignment Cars from Old Group
#define fvmodbus 246			// 246 - Modbus Protocol: 0-5=N1,N2,E1,E1,O1,O2 (Always 8 data bits, parity, stop bits)
#define fvaltrclfs 247			// 247 - Fire Alternate Recall After FS Turns Off: 	1=return to ALT from FS off
								// 													3=return to ALT from smoke reset or FS off
#define fvinsdecstp 248 		// 248 - Inspection Deceleration Stop Rate
#define fveprdir 249			// 249 - Emergency Power Recovery Direction
#define fvbkdrop 250			// 250 - Brake Drop Voltage 
#define fvbkdroprate 251		// 251 - Brake Voltage Drop Rate
#define fvbkdroplev 252			// 252 - Brake Drop Timing During Leveling Flag
#define fvrecvel 253			// 253 - Recovery Velocity in fpm
#define fvemdeclev 254			// 254 - Emergency Deceleration to Leveling Rate
#define fveprecvel 255			// 255 - Emergency Power Recovery Velocity
#define fvcycrun 256			// 256 - Run Cycle Time in hours: 0=off
#define fvhcbuponred 257		// 257 - Hall Call Up On Red Intensity
#define fvhcbupoffred 258  		// 258 - Hall Call Up Off Red Intensity
#define fvhcbupongreen 259  	// 259 - Hall Call Up On Green Intensity
#define fvhcbupoffgreen 260		// 260 - Hall Call Up Off Green Intensity
#define fvhcbuponblue 261		// 261 - Hall Call Up On Blue Intensity
#define fvhcbupoffblue 262 		// 262 - Hall Call Up Off Blue Intensity
#define fvopenlctl 263			// 263 - Door Open Light Control on CCS board:	1=FS1, 2=RTL, 4=EMP, 8=at lobby, 16=all
#define fvhcbuponcolor 264  	// 264 - Hall Call Button Up On Color
#define fvhcbupoffcolor 265  	// 265 - Hall Call Button Up Off Color
#define fvhcbseccolor 266  		// 266 - Hall Call Button Security Color
#define fvhcbseclt 267 			// 267 - Hall Call Button Security Light:  1=invert security
								//										  +2=flash security
								//										  +4=Sec Master Enable
#define fvhcbsecbright 268		// 268 - Hall Call Security Brightness
#define fvhcbsecred 269 		// 269 - Hall Call Security Red Intensity
#define fvhcbsecgreen 270		// 270 - Hall Call Security Green Intensity
#define fvhcbsecblue 271 		// 271 - Hall Call Security Blue Intensity
#define fvfhws2en 272 			// 272 - Fire Service Hoistway Sensor 2 Return Floor: 0=main, 1=alt 
#define fvdrvmult 273			// 273 - Drive Speed Multiplier
#define fvadvpretrq 274			// 274 - Advanced Pre-torque
#define fvcconcolor 275  		// 275 - Car Call Button On Color
#define fvccoffcolor 276  		// 276 - Car Call Button Off Color
#define fvcconbright 277 		// 277 - Car Call Output On Brightness for led (higher number is brighter)
#define fvccoffbright 278		// 278 - Car Call Output Off Brightness for led (higher number is brighter)
#define fvcconred 279			// 279 - Car Call On Red Intensity
#define fvccoffred 280   		// 280 - Car Call Off Red Intensity
#define fvccongreen 281  		// 281 - Car Call On Green Intensity
#define fvccoffgreen 282		// 282 - Car Call Off Green Intensity
#define fvcconblue 283			// 283 - Car Call On Blue Intensity
#define fvccoffblue 284 		// 284 - Car Call Off Blue Intensity
#define fvccltctl 285 			// 285 - Car Call Button Light Control: 1=flash security
								//									    2=Alternate Att Flash Sequence
#define fvccseccolor 286  		// 286 - Car Call Button Security Color
#define fvccsecbright 287		// 287 - Car Call Security Brightness
#define fvccsecred 288 			// 288 - Car Call Security Red Intensity
#define fvccsecgreen 289		// 289 - Car Call Security Green Intensity
#define fvccsecblue 290 		// 290 - Car Call Security Blue Intensity
#define fvsabben 291			// 291 - Sabbath Enable Control Variable: 1=allow momentary ir to override sabbath
								//										  2=Lobby Dwell timer
								//										  4=Lobby off
#define fvlowlinevolt 292		// 292 - Low Line Voltage (RMS value)
#define fvlowdoorvolt 293		// 293 - Low Door Voltage (RMS value)
#define fvts_ep 294				// 294 - Emergency Power Top Speed
#define fvjrk_ep 295    		// 295 - Emergency Power Soft Start Jerk Rate in fpm per second squared
#define fvacc_ep 296    		// 296 - Emergency Power Acceleration Rate in fpm per second
#define fvjrk3_ep 297   		// 297 - Emergency Power Roll Over Jerk Rate in fpm per second squared
#define fvdjrk_ep 298   		// 298 - Emergency Power Deceleration Jerk Rate in fpm per second squared
#define fvdec_ep 299    		// 299 - Emergency Power Deceleration Rate in fpm per second
#define fvm7dst_ep 300  		// 300 - Emergency Power Target Distance
#define fvtopspeed 301			// 301 - Car Top Speed
#define fvinsiser 302			// 302 - Inspection Velocity from ISER output
#define fvrcvrtim 303			// 303 - Emergency Power Recover to Floor Time
#define fvflemp 304				// 304 - Fire Light Control during EP: 1=FL off if not selected
#define fvepmanselen 305		// 305 - Emergency Power Manual Select Enable:	0=stop, 1=recall, 2=sequence all
#define fvflots 306				// 306 - Fire Light Control with Car Out of Service (Low Pressure, eaq, Stall)
#define fvsadt 307				// 307 - Sabbath Door Dwell Time
#define fvsabuzt 308			// 308 - Sabbath Door Buzzer Time Prior to Doors Closing
#define fvbatcap 309			// 309 - Low Battery Capacity Level
#define fvsabbdis 310			// 310 - Sabbath Disable Control Variable: +1=PI
								//										   +2=Lanterns
								//										   +4=arrow
#define fveeshortdt 311			// 311 - Electric Eye Cancel Door Time
#define fvepRclflDO 312			// 312 - Emergency Power Car at Recall Floor with Door Open: 0=override immediately
								//														     1=override after delay
#define fvepNRclflDO 313 		// 313 - Emergency Power Car Not at Recall Floor with Door Open: 0=override immediately
								//															     1=override after delay
#define fvAsgParkFlDO 314 		// 314 - Assign Parking Floor to Car with Door Open
#define fvhlandelay 315			// 315 - Hall Lantern Delay in 1/10 sec
#define fvsabben2 316			// 316 - Sabbath Enable Control 2 Variable: +1=Lobby On
								//										    +2=Cab Lanterns as Direction Lanterns
								//										    +4=Sabbath disable simul doors (front first)
#define fvclantctl 317			// 317 - Cab Lantern Control: 1=on door open, 2=on dpm limit
#define fvmaxdoorhld 318		// 318 - Max Door Hold Time from ED Input: 0=disabled
#define fvaccdoorcl 319			// 319 - Access Run Door Close
#define fvccattupcolor 320  	// 320 - Car Call Button Attendant Up Color
#define fvccattupbright 321		// 321 - Car Call Attendant Up Brightness
#define fvccattupred 322 		// 322 - Car Call Attendant Up Red Intensity
#define fvccattupgreen 323		// 323 - Car Call Attendant Up Green Intensity
#define fvccattupblue 324 		// 324 - Car Call Attendant Up Blue Intensity
#define fvccattdncolor 325  	// 325 - Car Call Button Attendant Dn Color
#define fvccattdnbright 326		// 326 - Car Call Attendant Dn Brightness
#define fvccattdnred 327 		// 327 - Car Call Attendant Dn Red Intensity
#define fvccattdngreen 328		// 328 - Car Call Attendant Dn Green Intensity
#define fvccattdnblue 329 		// 329 - Car Call Attendant Dn Blue Intensity
#define fvcopfirecolor 330  	// 330 - COP Fire light Color
#define fvcopfirebright 331		// 331 - COP Fire light Brightness
#define fvcopfirered 332 		// 332 - COP Fire light Red Intensity
#define fvcopfiregreen 333		// 333 - COP Fire light Green Intensity
#define fvcopfireblue 334 		// 334 - COP Fire light Blue Intensity
#define fvcopmedcolor 335  		// 335 - COP Medical light Color
#define fvcopmedbright 336		// 336 - COP Medical light Brightness
#define fvcopmedred 337 		// 337 - COP Medical light Red Intensity
#define fvcopmedgreen 338		// 338 - COP Medical light Green Intensity
#define fvcopmedblue 339 		// 339 - COP Medical light Blue Intensity
#define fvcopemcolor 340  		// 340 - COP Emergency light Color
#define fvcopembright 341		// 341 - COP Emergency light Brightness
#define fvcopemred 342 			// 342 - COP Emergency light Red Intensity
#define fvcopemgreen 343		// 343 - COP Emergency light Green Intensity
#define fvcopemblue 344 		// 344 - COP Emergency light Blue Intensity
#define fvcopotscolor 345  		// 345 - COP OTS light Color
#define fvcopotsbright 346		// 346 - COP OTS light Brightness
#define fvcopotsred 347 		// 347 - COP OTS light Red Intensity
#define fvcopotsgreen 348		// 348 - COP OTS light Green Intensity
#define fvcopotsblue 349 		// 349 - COP OTS light Blue Intensity
#define fvbklton 350			// 350 - Backlight Output lights: +1: Fire
								//								 +2: Medical
								//								 +4: Emergency
								//								 +8: OTS
								//								+16: Att Up/Dn Light
								//								+32:Non-CC
#define fvcansynccnt 351		// 351 - Can Sync Count in 250 msec increments
#define fvhcbdnoncolor 352		// 352 - Hall Call Dn On Color
#define fvhcbdnonbright 353		// 353 - Hall Call Dn On Brightness
#define fvhcbdnonred 354		// 354 - Hall Call Dn On Red Intensity
#define fvhcbdnongreen 355  	// 355 - Hall Call Dn On Green Intensity
#define fvhcbdnonblue 356		// 356 - Hall Call Dn On Blue Intensity
#define fvhcbdnoffcolor 357		// 357 - Hall Call Dn Off Color
#define fvhcbdnoffbright 358	// 358 - Hall Call Dn Off Brightness
#define fvhcbdnoffred 359		// 359 - Hall Call Dn Off Red Intensity
#define fvhcbdnoffgreen 360  	// 360 - Hall Call Dn Off Green Intensity
#define fvhcbdnoffblue 361		// 361 - Hall Call Dn Off Blue Intensity
#define fvhcbcboncolor 362		// 362 - Hall Call CB On Color
#define fvhcbcbonbright 363		// 363 - Hall Call CB On Brightness
#define fvhcbcbonred 364		// 364 - Hall Call CB On Red Intensity
#define fvhcbcbongreen 365  	// 365 - Hall Call CB On Green Intensity
#define fvhcbcbonblue 366		// 366 - Hall Call CB On Blue Intensity
#define fvhcbviponcolor 367		// 367 - Hall Call VIP On Color
#define fvhcbviponbright 368	// 368 - Hall Call VIP On Brightness
#define fvhcbviponred 369		// 369 - Hall Call VIP On Red Intensity
#define fvhcbvipongreen 370  	// 370 - Hall Call VIP On Green Intensity
#define fvhcbviponblue 371		// 371 - Hall Call VIP On Blue Intensity
#define fvhcbiruponcolor 372	// 372 - Hall Call IR Up On Color
#define fvhcbiruponbright 373	// 373 - Hall Call IR Up On Brightness
#define fvhcbiruponred 374		// 374 - Hall Call IR Up On Red Intensity
#define fvhcbirupongreen 375  	// 375 - Hall Call IR Up On Green Intensity
#define fvhcbiruponblue 376		// 376 - Hall Call IR Up On Blue Intensity
#define fvhcbirdnoncolor 377	// 377 - Hall Call IR Dn On Color
#define fvhcbirdnonbright 378	// 378 - Hall Call IR Dn On Brightness
#define fvhcbirdnonred 379		// 379 - Hall Call IR Dn On Red Intensity
#define fvhcbirdnongreen 380  	// 380 - Hall Call IR Dn On Green Intensity
#define fvhcbirdnonblue 381		// 381 - Hall Call IR Dn On Blue Intensity
#define fvhcbirupoffcolor 382	// 382 - Hall Call IR Up Off Color
#define fvhcbirupoffbright 383	// 383 - Hall Call IR Up Off Brightness
#define fvhcbirupoffred 384		// 384 - Hall Call IR Up Off Red Intensity
#define fvhcbirupoffgreen 385  	// 385 - Hall Call IR Up Off Green Intensity
#define fvhcbirupoffblue 386	// 386 - Hall Call IR Up Off Blue Intensity
#define fvhcbirdnoffcolor 387	// 387 - Hall Call IR Dn Off Color
#define fvhcbirdnoffbright 388	// 388 - Hall Call IR Dn Off Brightness
#define fvhcbirdnoffred 389		// 389 - Hall Call IR Dn Off Red Intensity
#define fvhcbirdnoffgreen 390  	// 390 - Hall Call IR Dn Off Green Intensity
#define fvhcbirdnoffblue 391	// 391 - Hall Call IR Dn Off Blue Intensity
#define fvhcbuponbright 392 	// 392 - Hall Call Output Up On brightness for led (higher number is brighter)
#define fvhcbupoffbright 393	// 393 - Hall Call Output Up Off brightness for led (higher number is brighter)
#define fvhcbcbloc 394			// 394 - Code Blue Location: 0=CB on CB, 1=CB on HCB, 2=CB on IR
#define fvhcbviploc 395			// 395 - VIP Location; 0=vip on vip, 1=Vip on HCB, 2=Vip on IR
#define fvhcbirlt 396			// 396 - IR light Color Control: 0=IR Color, 1=HC Color until IR activated
#define fvhcbcboffcolor 397		// 397 - Hall Call CB Off Color
#define fvhcbcboffbright 398 	// 398 - Hall Call CB Off Brightness
#define fvhcbcboffred 399		// 399 - Hall Call CB Off Red Intensity
#define fvhcbcboffgreen 400  	// 400 - Hall Call CB Off Green Intensity
#define fvhcbcboffblue 401		// 401 - Hall Call CB Off Blue Intensity
#define fvhcbvipoffcolor 402 	// 402 - Hall Call VIP Off Color
#define fvhcbvipoffbright 403	// 403 - Hall Call VIP Off Brightness
#define fvhcbvipoffred 404		// 404 - Hall Call VIP Off Red Intensity
#define fvhcbvipoffgreen 405  	// 405 - Hall Call VIP Off Green Intensity
#define fvhcbvipoffblue 406		// 406 - Hall Call VIP Off Blue Intensity
#define fvviplant 407			// 407 - VIP Lantern Control: 1=ring up and down at vip floor, 2=hall, 3=both
#define fvgccovride 408			// 408 - Group Car Call Override Control: 1=Key Switch Direct
#define fvexcflt1 409			// 409 - Exclusion Fault 1
#define fvexcflt2 410			// 410 - Exclusion Fault 2         
#define fvexcflt3 411			// 411 - Exclusion Fault 3
#define fvexcflt4 412			// 412 - Exclusion Fault 4        
#define fvexcflt5 413			// 413 - Exclusion Fault 5
#define fvexcflt6 414			// 414 - Exclusion Fault 6
#define fvebkpick 415			// 415 - Emergency Brake Pick Voltage
#define fvebkhold 416			// 416 - Emergency Brake Hold Voltage
#define fvebkpkrate 417			// 417 - Emergency Brake Pick Rate
#define fvebkdroprate 418		// 418 - Emergency Brake Drop Rate
#define fvebkaclv 419			// 419 - Emergency Brake AC Line to Line Voltage
#define fvebkoptodly 420 		// 420 - Brake Opto Trigger Delay
#define fvcanbaud 421			// 421 - Can Baud Rate: 0=115.2K, 1=57.6K
#define fvarrivallant 422		// 422 - Car Call Arrival Lantern; 1 = activate lant/gong without onward call
#define fvcbsrcar 423 			// 423 - Code Blue Second Riser Car Select
#define fvcbovrir 424			// 424 - Code Blue over IR car
#define fvircbpenalty 425		// 425 - IR Car Code Blue Penalty Time
#define fvinsec 426				// 426 - INSEC (In Security Output Invert)
#define fvattmandir 427			// 427 - Attendant Manual Direction Enable: +1=use ATTUP/ATTDN to set direction
								//											+2=use ATTUP as start
								//											+4=no CC latch until doors closed
#define fvencinterval 428 		// 428 - Encoder Debounce Interval
#define fvencsamples 429		// 429 - Encoder Number of Samples
#define fvhboncct 430       	// 430 - Handicap Buzzer on Car Call Beep Time in 1/10 sec
#define fvsrvups 431			// 431 - Service UPS Mode
#define fvcomdiag1 432			// 432 - Serial Port Comm Diag: 1=Debug, 2=Galcom, 3=DL20
#define fvbaud2 433				// 433 - User Port 2 Baud Rate: 0=2400,  1=4800,   2=9600,   3=19200
#define fvcputiming 434			// 434 - Cpu Timing Output Control
#define fvsvcmsg3 435			// 435 - Service Message 3 Display
#define fvhcasgnsectype 436		// 436 - Hall Call Assignment Secure Type:	+1=secure up calls
								//											+2=secure dn calls
								//											+4=secure rear up calls
								//											+8=secure rear dn calls
#define fvupsbaud 437			// 437 - UPS Baud Rate: 0=2400, 1=4800, 2=9600, 3=19200
#define fvhcattbuz	438			// 438 - HC Acknowledge Attendant Buzzer (Buzz once(1s) every time a call comes in):
								//																			 0=disable
								//																			 1=enable
#define fvcbcar2 439			// 439 - Second Code Blue Car Select
#define fvheloff2 440			// 440 - Hall Elevator Off Options 2: +1=Keep doors Closed (do not cycle in reversal)
								//									  +2=do not blink ELOO
								//									  +4=Elevator Off Auto Reset With Timer
								//									  +8=enable ELOO to indicate the car finished - 
								//															recall of elevator off mode
#define fvcbbuz 441				// 441 - Code Blue Buzzer Control: 1=Turn on while in code blue recall
#define fvcbovrind 442			// 442 - Code Blue Override Independent: 1=recall car after timer expires
#define fvcbanycar 443			// 443 - Code Blue Recall Any Car If CB Cars 1 And 2 Are Not Available
#define fvheofovrind 444		// 444 - Hall Elevator Off Override: +1 independent: recall car after timer expires
								//								     +2 attendant: recall car after timer expires
#define fvreopendoorfs2 445		// 445 - NYC FS2 Door Reopen on STP
								// if DCB is pressed in FS2, then in-car stop switch is pulled while doors are closing: 
								//															0=the doors continue to close (default)
								//									  						1=the doors re-open
#define fvpwlbkon 446			// 446 - Power Loss Brake On Time
#define fvpwlbkoff 447			// 447 - Power Loss Brake Off Time
#define fvattempcar 448		   	// 448 - Select and Prioritize the ATT car to Run on Emergency Power Service
								//		 (It won't be recalled. It recovers from it was left at.)
#define fvencdir  449			// 449 - Encoder Direction for Open Can Encoder: 0=CW, 1=CCW
#define fvemprecall	450			// 450 - Emergency Cars Finished Recalling Output enable (EPCOM)
#define fvoser1	451				// 451 - Out of Service Light control: +1=not responding to calls
								//									   +2=between floors for over a minute
								//									   +4=SS open
#define fvexdtflr 452			// 452 - Extra Door Time Location: floor number. Follows fvexdt timer (for EDHL only)
#define fvoser2	453				// 453 - Out of Service Light Control: +1=Alarm
#define fvosernocall 454		// 454 - Time for OSERL Not Answering Call
#define fvoserbtwfl 455			// 455 - Time for OSERL car Between Floors
#define fvoserssopen 456		// 456 - Time for OSERL Safety String Open
#define fvfhws2loc 457 			// 457 - Fire Service Hoistway Sensor Location: 0=same HW, 1=Separate HW
#define fvrcdrdly 458			// 458 - Retiring Cam Drop Fail Safe Delay
#define fvveldiff 459			// 459 - Velocity Difference 
#define fvveldifffltdis	460		// 460 - Velocity Difference Fault Disable
#define fvspbdirfltdis 461		// 461 - SPB Direction Fault Disable
#define fvlimdirfltdis 462		// 462 - Limit Direction Fault Disable
#define fvdirfltdis 463			// 463 - Encoder Direction Fault Disable
#define fvuldldirfltdis 464		// 464 - UL/DL Direction Fault Disable
#define fvrelevsttim 465		// 465 - Relevel Start Velocity Hold Time
#define fvmedreopen 466			// 466 - Med Door Open Sequence for Re-open: 0=Stop
								//										     1=Constant pressure
								//										     2=momentary to DOL
#define fvmedindovr 467			// 467 - Med Ind Override: 0=Immediate, 1=After Delay, 2=No override
#define fvhsaftercb 468			// 468 - Code Blue Bypass Disable: 0=from Auto to HS if EMS turns on
								//								   1=HS only after CB recall
#define fvextdoormsg 469		// 469 - Extended Door PI Message
#define fvretcar2 470			// 470 - First Return Car on Emergency Power for Feeder 2
								//		 (the rest are done sequentially in a loop)
#define fvselcar2 471			// 471 - First Car Selected on Emergency Power for Feeder 2
								//		 (the rest are done sequentially in a loop)
#define fvrcmfrght 472			// 472 - Retiring Cam for Freight Output Enable (reboot required)
#define fvlantprefdly 473		// 473 - Lantern Preference Change Delay
#define fvslipveldiff 474		// 474 - Slip Velocity Difference Fault Trip Value
#define fvdobovrdcb 475			// 475 - DOB Overrides DCB on Phase 2
#define fvdenfsv 476			// 476 - Denver Fire Service Amendment to Close the Phase 1 Doors After Recall and -
								//																Reopen From a Hall Call
#define fvf1dwell 477			// 477 - Denver FS Phase 1 Dwell Time
#define fvhcsecctrl 478			// 478 - Hall Call Security Control: 1=std plus SR hall call, 2=only SR hall calls
#define fvslipveldiffdis 479  	// 479 - Slip Velocity Difference Disable
#define fvlevflt 480			// 480 - Leveling Fault Trip: 0=drop everything, 1=set em brake or gripper
#define fvembksw 481			// 481 - Emergency Brake Switch Control: 0=don't start if brake switch does not pick
								//									     1=ignore brake switch on start
#define fvattbzctrl 482			// 482 - Attendant Buzzer Control
#define fvradposind 483			// 483 - Radial Position Indicator in inches
#define fvindrclen 484			// 484 - Force Car to Recall On Independent Service to the Lobby When No Demand Is -
								//																				Present
#define fvindrcltmr 485			// 485 - Time-out to Recall On Independent Service to the Lobby When No Demand Is -
								//																	Present in 1/10 sec
#define fvmtrrpm 486			// 486 - Motor RPM (used for keb drive)
#define fvvipmulticall 487		// 487 - VIP Multiple Calls Enable: 0=single call, 1=multiple calls
#define fvservoctrl	488			// 488 - Service Output Control: number of service flag
#define fvencbaud 489			// 489 - Can Encoder Baud Rate: 0=250k, 1=125k
#define	fvencnode 490			// 490 - Can Encoder Node ID: 1=Dynapar, 0x3F (63)=Turck, 0x7F (127)=Wachendorff
#define fvcbsinglecall 491		// 491 - Code Blue Single Car Call: 0=regular CB operation
								//									1=allow single car call to place car in HSV
#define fvsabcolmode 492		// 492 - Sabbath Collective Mode; 0=dn collective, 1=up collective
#define fvrtldoortyp 493		// 493 - Return to Lobby Door Open Type: 0=Front Only, 1=Rear Only, 2= Front and Rear
#define fvlaniralwys 494		// 494 - Ring the Lantern in IR All the Time: 0=not in NCU, 1=all the time
#define fvhighpriorityfl 495	// 495 - High Priority Floor Number
#define fvhighpriorityfltim 496	// 496 - High Priority Floor Time in 1/10 sec (Max car_eta)
#define fvep_selsw_rcl 497		// 497 - Emergency Manual Select Switch Recall Operation: 0=run
								//														  1=recall and run
								//														  2=recall only 
#define fvaccspd 498			// 498 - Access Speed in fpm
#define fvautosvctmout 499		// 499 - Auto Service Time-out Enable: 0=disable, 1=enable
#define fvautosvctmouttm 500	// 500 - Auto Service Time-out Time: Min=10, Max=3200 sec
#define fvccsonsab 501			// 501 - Car Call Lockouts on Sabbath Operation
#define fvmedccsovride 502		// 502 - Medical Service Override Car Call Security
#define fvheofautorst 503		// 503 - HEOF Auto Reset Time
#define fvccpbsongpccsec 504	// 504 - CCPBS on Group Car Call Security
#define fvuldeadzone 505		// 505 - Dead Zone Selection
#define fvaccutdist 506			// 506 - Access UT Distance in 1/10 inch
#define fvaccdtdist 507			// 507 - Access DT Distance in 1/10 inch
#define fvcomdiag2 508			// 508 - Serial Port Comm Diag 2:	1=Debug, 2=Galcom, 3=DL20
#define fvtrctrl 509			// 509 - Third Riser Control: 1 = TR Cars Std HC no assign
#define fv485baud 510			// 510 - Baud Rate for 485 Port:	0=2400,  1=4800,   2=9600,   3=19200
#define fvenctype 511			// 511 - Encoder Type: 0=cons file setting, 1=Turck, 2=Dynapar, 3=Wachendorff
#define fvkebdpytype 512		// 512 - Keb Drive Display Type: 0=Standard, 1=LCD
#define fvbrkphase 513			// 513 - Brake Phase Input: 0=single, 1=3 phase
#define fvebkphase 514			// 514 - Emergency Brake Phase Input: 0=single, 1=3 phase
#define fvdistff 515			// 515 - Distance Feed Forward
#define fvsrccsecbydir 516		// 516 - Second Riser Car Calls Security by Direction: 1=secure down dir cc's
								//													   2=secure up dir cc's only
#define fvminshortdten 517		// 517 - Minimum Short Door Time Enable
#define fvfire_rst_2 518		// 518 - Fire Recall Reset 2: +1=disable bypass input until recall floor reached
#define fvinvhcsec 519			// 519 - Invert Hall Call Security Input
#define fvdzgripctrl 520		// 520 - DZ Gripper Trip Control: 0=from DZ only. 1=DZ and DZA
#define fvsecdist 521			// 521 - Security Disable Time
#define fvdldeadzone 522		// 522 - DL Dead Zone Selection
#define fveetesttm 523			// 523 - EE Test Time in 1/10 sec
#define fvsecreasgncc 524		// 524 - Reassign Car Calls When on Multiple Floor Mask Security (cons[SecFlCfg] = 2)
								//		   0=disabled, +1=in case front CC are getting secured, reassign them as rear
								//		  +2=in case rear CC are getting secured, reassign them as front
#define fvdisopphc 525			// 525 - Disable Opposite Hall Call: +1=front hc riser
								//								     +2=rear hc riser
								//								     +4=ir front hc riser
								//								     +8=ir rear hc riser
#define fvdisopphctm 526		// 526 - Disable Opposite Hall Call Time
#define fvheloff3 527			// 527 - Hall Elevator Off Options 3: +1=only activate if doors are closed
#define fvceloff 528			// 528 - Car Elevator Off Options:	+1=Recall
								//								    +2=Keep Door Open
								//								    +4=Turn off CLF
								//									+8=Leave Door Close
#define fvceloff2 529			// 529 - Car Elevator Off Options 2: +1=Keep doors Closed (do not cycle in reversal)
								//								     +2=do not blink ELOO
								//								     +4=Elevator Off Auto Reset With Timer
								//								     +8=enable ELOO to indicate the car finished -
								//														recall of elevator off mode
#define fvcboverfs 530			// 530 - Code Blue Over Fire Service: +1=Enable (CB prevents car from recalling in FS)
#define fvrmccd 531				// 531 - COP/Remote Disable: +1=Up Direction Disable COP
								//						     +2=Dn Direction Disable COP
								//						     +4=Up Direction Disable RM
								//						     +8=Dn Direction Disable RM
#define fvfs2wsd 532			// 532 - Fire Service 2 with Special Devices (HYDRO ONLY)
#define fvautoswgdodly 533		// 533 - Auto Swing Door Open Delay
#define fvcltim 534				// 534 - CL Pulse Time

#define lastfvar 534			// set to the last variable number


extern int16 fvars_deci[nmbfvar];
extern int16 fvars_max[nmbfvar];
extern int16 fvars_min[nmbfvar];
extern int16 fvars_default[nmbfvar];

extern const uint32 fl_mask[33]; 

// Prototypes of all the functions hydro.cpp
extern void atfloor(void );
extern void chkrun(void );
extern void clrall(void );
extern void clrlant(void );
extern void clr_dir_arrow (void);
extern void diag(void );
extern void dirsel(void );
extern void hlants(void );
extern void new_inctime();
extern void inctime(int16 start_ck );
extern int16 ind (void);
extern void record_fault(uint16 code);
extern void inittim(void );
extern void stall(void );
extern int16 man_dcc(void);
extern int16 man_fdcc(void);
extern int16 man_rdcc(void);
extern void motion(void );
extern int16 needstop(void );
extern void petdog(void );
extern void pisout( int);
extern void reset(void );
extern void sstring(void );
extern int16 safeties (void);
extern void setpref(void );
extern int16 door_locks(void);
extern int16 safe(void);
extern int16 car_gate(void);
extern int16 chk_start(int16 cmd);
extern void jail_break(void);
extern void zoneelev(void);
extern void elevator_off (void);
extern void Set_Fault_Bits (void);
extern void set_ul_dl_dz (void);

extern void setfsf(void );
extern void seteqf(void);
extern void setempf(void);
extern void setmedf(void);
extern void setcodebf (void);
extern void sethsvf (void);
extern void setvipf(void);
extern void settugf(void);
extern void get_onward_calls (void);
extern void return_to_lobby(void);
extern void hot_oil(void);
extern int ep_recall_ok(void);
extern int16 safeties (void);
extern void set_position_from_dpp(void);

/* Revision History
	 10/26/10 v5.52.41 mhd 1. Changed Arrays for example for Tract_HR: iodata from 145 to 157
																	 IO_board from 49 to 53
                						   				for All others: iodata from 85 to 97
								  									   IO_board from 29 to 33
*/
