// This is the main control program for hydro and traction cars.
// This version is used for ANSI 2000 Compliant boards only.

#define SLC386 0
#define Security 0	     // Full blown security
#define auto_elevator 0	// set to a zero if not automatic elevator

#define d_CONTROL 1
#include "global.h"


// ZYM 12-1-2017 work as cedes head
#include "cedes_can.h"


#define Hollister 0

#define CR 13
#define LF 10
#define ESC 27
  
extern uint8 Enable_CAN_Ctrl;
extern uint8 Enable_CT_CAN_Ctrl; // disable while doing Car Top CAN test (wbg)
extern uint8 Enable_MR_CAN_Ctrl; // disable while doing Machine Room CAN test (wbg)
extern uint8 Enable_gr_CAN_Ctrl;
extern uint8 Enable_RS232_Ctrl; 
extern uint8 Enable_RS485_Ctrl;
extern uint8 Enable_iospi_Ctrl;
extern uint8 Enable_Hallcom_Ctrl;
extern void CAN_Control(void);
extern void CAN_Init(void);
extern int UpdateAllSEB(void);
extern void ReadAllSEB(void);
extern void i1spi_pack (void);
extern void nts_spi_pack (uint8 msg);
extern void i1spi_unpack (void);
extern void iospi_unpack (void);
extern void nts_spi_unpack (void);
extern void iospi_pack (void);
  
int32 start_int = 0;

int16 led_stat = 0;

uint32 cons_date_code = 0;

int32 debug1 = 0;
int32 debug2 = 0;
int32 debug3 = 0;
int32 debug4 = 0;
int32 debug5 = 0;
int32 debug6 = 0;
int32 debug7 = 0;
int32 debug8 = 0;

uint8 chkrun_state = 0;
uint8 chkstart_state = 0;
uint8 chklevel_state = 0;
uint8 chkdoor_state = 0;

uint8 Timer_tic = 0;  // tagwbg 02 - Timer_tic gets set in timer
  // ISR, every 5ms.  Gets reset in main() 

int16 int_flag=0;
int16 int_err=0;
int16 ir_mask_1 = 0;
int16 ir_mask_2 = 0;
int16 dpy_io_data = 0;
int16 io_cfg=0;
int16 ps_int_cnt = 0;
int16 power_up_start_time = 0;
int16 inctime_count = 0;

int16 remote_cc=0;
int16 remote_rcc=0;

uint32 inctim_time = 0;
uint32 inctim_min = 0x7FFFFFFF;
uint32 inctim_max = 0;
uint32 inctim_avg = 0;
uint32 inctim_now = 0;
uint32 proctim_min = 0x7FFFFFFF;
uint32 proctim_max = 0;
uint32 proctim_avg = 0;
uint32 proctim_now = 0;

uint8 pwr_up_init = 0;				// flag that control software has been initialized on power up

float Dist_Ratio;
int32 Pos_Cnt_Ref;
float Dist_Ratio_S;
int32 Pos_Cnt_Ref_S;

int16 alt_can = 0;
int16 alt_gcom = 0;

int16 allow_run;						// Allow run during emergency power
int16 okto_run;

int16 reset_nonit = 0;
int16 ignore_uldl_fault = 0;

unsigned char misc_io_data = 0;			// misc io data include brake board inputs

		
// Timers used throught the program
int16 cctim[fl_size];          // car call timers
int16 ccrtim[fl_size];         // car call timers



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
 tredundancy, 		// redundancy relay test timer
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
 ttoutmandoordbnc, 	// timeout manual door for hall calls debopunce
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
 t_Functional_Test_Delay,
 tRefreshInp,
 trsCOM1Delay,
 tGR_CAN_Tx,
 tGR_CAN_Rx, 
 tCanB_Tx,          //10 Can B Tx
 tCanB_Rx,          //11 Can B Rx
 tnmb				// Must be last
};

int16 timers[tnmb];
//int16 new_timers[tnmb];

int16 tmr_cascade_state = 0;
int16 tmr_start = 0;
int16 tmr_end = 0;
int16 run_timer = 0;

uint16 tmr_10_msec = 0;

int16 tuser_com = tuser_com1;		// User interface comm timer
int16 tcom_recv = tcom1_recv;		// User interface comm timer
int16 tcom_tran = tcom1_tran;		// User interface comm timer
int16 tcom_pkreq = tcom1_pkreq;		// User interface auot packete request time-out comm timer
int16 tgcom_car = tgcom_car0;  		// group comunication timer for car 1
int16 tgrcan_car = tgrcan_car1 - 1;  	// group can comunication timer for car 1
int16 tepcar = tepcar1 - 1;			// Recall time-out timer for Emergency Power Return Car
int16 tcantx = tcantx1;				// Can device 1 output re-transmit timer
int16 tcanrx = tcanrx1;				// Can device 1 input timer
int16 temhc = temhc1;				// Emergency dispatch hall call timer 1
int16 temrhc = temrhc1; 			// Emergency dispatch rear hall call timer 1
int16 teprcvr	= teprcvr1 - 1;		// Recover time-out timer for Emergency Power Return Car
int16 tmrcantx = tmrcantx1;			// Can device 1 output re-transmit timer
int16 tmrcanrx = tmrcanrx1;			// Can device 1 input timer
int16 tgrcantx = tgrcantx1;			// Can device 1 output re-transmit timer
int16 tgrcanrx = tgrcanrx1;			// Can device 1 input timer

uint32 trace_tmr=0;

enum {
 ts_sec,			// General Purpose second timer
 ts_pwd,			// Password timer
 ts_datechk,		// Date chk sec timer
 ts_crtcsync,		// Car sync rtc with dispatcher fail timer
 ts_nmb				// Must be last
};

int16 timer_sec[ts_nmb];

// hour timer
int16 rt_day = 0;
int16 rt_hour = 0;
int16 rt_minute = 0;
int16 rt_second = 0;
int16 day_st = 0;
int16 hour_st = 0;
int16 minute_st = 0;
int16 prev_minute = 0;

int16 loop_cnt=0;
int16 loop_cnt_array[10] = {6,6,6,6,6,6,6,6,6,6};
int16 loop_cnt_ptr=0;
int16 avg_loop_cnt = 6;

int16 gripper_flt_timer = 0;
int16 gov_flt_timer = 0;
int16 gripper_spd_trip_timer = 0;
int16 gripper_spb_umotion_timer = 0;
int16 intended_motion = 0;			// intended motion for encoder rope gripper trip
int16 intended_motion_spb = 0;			// intended motion for safety processor board rope gripper trip
int16 em_stop_time = 0;				// emergency stop - time to stop
int32 enc_diff = 0;
int32 spb_diff = 0;
int32 st_spb_vel = 0;
int32 st_enc_vel = 0;
				
int16 pwr_up_reset = 0;				// power up reset flag
int16 pwr_up_firehold_rst = 0;		// power up fire hold reset

// Variables for limit test

int16 tmr_lim_dir = 0;		// Timer for limit direction fault
int16 tmr_vel_diff = 0;		// Timer for velocity difference fault
int16 tmr_spb_enc_dir = 0; 	// Timer for SPB encoder direction fault
int16 tmr_enc_dir = 0; 		// Timer for Controller encoder direction fault
int16 tmr_uldl_dir = 0;		// Timer for UL/DL direction fault check (prevent fault on power cycle)
int16 tmr_spb_vel_diff = 0;	// Timer for SPB velocity difference fault

int16 uldl_seq = 0;		// ul/dl sequence variable
int16 dlul_seq = 0;	// dl/ul sequence variable

int16 tmr_uts_dbn = 0;
int16 tmr_dts_dbn = 0;


// Messages sent out to the user
unsigned char message[2][15] = {
{'U','S','E','R',' ','N','A','M','E',' ',CR,'\n'},
{'P','A','S','S','W','O','R','D',' ',CR,'\n'},
};
// Flags used to direct the program
int16 codebf=0;           // code blue flag
			// codebf = 0 there is no emergency
			// codebf = 1 go to emergency floor
			// codebf = 2 at code blue floor
			// codebf = 3 at code blue floor with door open
			// codebf = 4 finished code blue return 
int16 in_code_blue=0;		// set when in code blue
int16 ind_codeb_alarm=0;	// set to alarm ind car that code blue requested 
int16 ems_onecall  = 0;	//Allow  one call in hospital service after code blue recall
int16 cb_over_ind=0;		// Set CB over independent
int16 heof_over_ind=0;	// Set HEOF over IND
int16 heof_over_att=0;	// Set HEOF over ATT
int16 hsvf=0;				// hospital service flag
			// hsvf = 1 car on hospital service
int16 vipf=0;    			// priority service (vip) flag
			// vipf = 0 there is no priority call
			// vipf = 1 go to vip floor
			// vipf = 2 at vip floor
			// vipf = 3 at vip with door open
			// vipf = 4 finished vip return 
			// vipf = 5 car is not moving but has moved
			// vipf = 6 put car back into automatic mode 

int16 tugf=0;		// TUG robot interface flag
			// tugf = 1 tehre is a tug call
			//
			//
			//
					 
int16 in_vip=0;	// set when in vip service
int16 in_overload = 0;	// set when car on overload
int16 in_att = 0;			// set when car in attendant service
int16 dcalls=0;           // direction of calls
			// dcalls = 0 there are no calls
			// dcalls = 1 there is a call above
			// dcalls = 2 there is a call below
			// dcalls = 3 the is a call above and below
int16 onward_calls = 0;		// direction of call no at the floor
			// onward_calls = 0 there are no calls
			// onward_calls = 1 there is a call above
			// onward_calls = 2 there is a call below
			// onward_calls = 3 the is a call above and below
int16 onward_cc = 0;
			// onward_cc = 0 there are no car calls
			// onward_cc = 1 there is a car call above
			// onward_cc = 2 there is a car call below
			// onward_cc = 3 the is a car call above and below
int16 onward_hc = 0;
			// onward_hc = 0 there are no hall calls
			// onward_hc = 1 there is a hall call above
			// onward_hc = 2 there is a hall call below
			// onward_hc = 3 the is a hall call above and below
int16 nohcreopen = 0;			// no hall call reopen
			// nohcreopen = 0 reopen the door when hc is pressed at floor
			// nohcreopen = 1 Dont reopen the door
int16 beyond_lby_cc = 0;		// Car call set above = 1 or below = 2 the lobby
int16 beyond_lby_call = 0;	// Car or Hall Call set above = 1 or below = 2 the lobby
int16 top_cc = 0;			// top most car call
int16 bot_cc = 0;			// bottom most car call
int16 nmb_cc = 0;			// number of car calls at different floors

int16 dirf=0;             // direction flag
			// dirf = 0 no direction
			// dirf = 1 last direction of travel was up
			// dirf = 2 last direction of travel was down
int16 prev_dirf=0;	// previous direction of travel

int16 doorf=0;            // door flag
			// doorf = 0 doors closed no action
			// doorf = 1 doors are opening
			// doorf = 2 doors are dwelling
			// doorf = 3 doors are closing
			// doorf = 4 doors are nudging closed
int16 swdorf = 0;		  // hall swing door flag
			// swdorf = 0 doors closed no action
			// swdorf = 1 doors are opening
			// swdorf = 2 doors are dwelling
			// swdorf = 3 doors are closing
int16 rswdorf = 0;		  // hall swing door flag
			// rswdorf = 0 doors closed no action
			// rswdorf = 1 doors are opening
			// rswdorf = 2 doors are dwelling
			// rswdorf = 3 doors are closing
int16 ins_doorf=0;
int16 fdoor_en=0;		// front door enable
int16 fdoor_req=0;	// front door request to open
int16 fdob_req=0;		// front door open button request
int16 fcc_req=0;		// front car call door open request
int16 nudgst=0;		// front door nudging status
						// 0x01 nudged closed with an up pilot
						// 0x02 nudged closed with a dn pilot
						// 0x04 nudged closed with an no pilot
int16 do_seq = 0;		// Set when door reaches full open (DOL)   
						// 0x01 door open sequence with an up pilot
						// 0x02 door open sequence with a dn pilot
						// 0x04 door open sequence with no pilot
int16 do_to = 0;		// Set when door open limit times out.
int16 ee_to = 0;		// Set when electric eye times out.
int16 manual_door = 0;		// set for manual door (added to have manual rear staggered and automatic front)
int16 drop_front_rcm = 0;		// set to drop front RCM
int16 auto_rcm = 0;

int16 doors_open = 0;		// local door open flag
int16 ins_do_run = 0;		// local inspection and allowed to run with door open 
int16 spb_vel_trip = 0;		// local spb velocity trip flag
int16 enc_vel_trip = 0;		// local encoder velocity trip flag

int16 freight_fdoor_alarm = 0;  	// frieght front door alarm
int16 fsdo=0;				// Front swing door open
int16 fsdo_seq=0;			// front swing door open sequence
int16 dpref=0;            // direction preference
			// dpref = 0 no direction selected as preference
			// dpref = 1 direction preference is up
			// dpref = 2 direction preference is down
int16 prev_dpref=0; 
int16 cc_above_lby=0;	// set to 1 if car calls above the lobby 
int16 cc_below_lby=0;	// set to 1 if car calls below the lobby  
int16 run_no_pass_cnt=0;	// Run with no passenger entering the car
int16 pass_det=0;			// Passenger detected in the car
int16 extended_door_time = 0;	// set when extended door time is active
int16 rm_extd_doort = 0;	// set when remote extended door time is active
int16 rcm_frght_out_en = 0;				// Retiring Cam for freight output enable


int16 eqf=0;			// Earthquake flag
			// eqf = 0 not on earthquake operation
			// eqf = 1 earthquake sensor has activated
			// eqf = 2 counterweight derailment sensor has activated
			// eqf = 3 recover away from the counterweight
			// eqf = 4 stopped at a floor
int16 old_eqf = 0;
int16 above_cwt = 0;		// 1 = car is above the counterweight
int16 old_above_cwt = 0;
int16 on_cws=0;
int16 eq_pos_reset = 0;		// earthquake position reset flag

int16 empf=0;             // emergency power car status flag
			// empf = 0 not on emergency power										no_ep 0			
			// empf = 1 on emp waiting 												ep_wait	1		
			// empf = 2 on emp waiting with doors open (cycle door close if hydro)	ep_wait_door 2 
			// empf = 3 on emp returning home										ep_recall 3		
			// empf = 4 on emp returned home with doors open						ep_home	4		
			// empf = 5 on emp returned home with doors closed						ep_home_close 5
			// empf = 6 on emp and selected to run									ep_select 6		
			// empf = 7 on emp waiting with doors closed							ep_switch 7		
				 
int16 in_emp_trying_to_reset = 0;

// Line voltage variables

int16 L1_Voltage = 0;
int16 L2_Voltage = 0;
int16 L3_Voltage = 0;
int16 Door_Voltage = 0;

int16 voltf = 0;					// voltage status flag 
								// bit 0: L1
								// bit 1: L2
								// bit 2: L3
								// bit 4: Door

// Hugs Security Variables

int16 hugsf=0;	// hugsf = 0 no hugs security
				// hugsf = 1 front door security
				// hugsf = 2 rear door security
				// hugsf = 3 front and rear door security


int16 adv_pre_trq_run = 0;	// Advanced pre-torque run enable flag
int16 adv_pre_trq_fault = 0;	// Advanced pre-torque run fault flag

int16 run_time = 0;			// Floor to floor run time from tmotion timer.
int16 pre_torque_time = 0;		// Time to pretorque the motor to prepare to tun
int16 targetfl_time = 0;		// Target floor tmotion time value when first entered
int16 preopen_time = 0;		// Time car enables preopoening door on tmotion timer
int16 ep_recall_fl = 1;		// emergency power recall floor set from setempf routine
int16 eexit = 0;        // emergency exit of the motion prodedure
int16 estop = 0; 				// emergency stop
int16 estops_1_run = 0;		// estops counter
int16 fault_run = 0;			// fault during run
int16 em_sd_run = 0;			// emergency slowdown during run
int16 fault_run_cnt = 0;		// number of runs with faults
int16 shutdown = 0;			// shutdown car due to too many runs with faults in a row
int16 pwl_ups_fault = 0;		// power loss ups fault (shuts car down)
int16 limit_dir_fault = 0;	// Fault from hitting limits from wrong direction
int16 spb_dir_fault = 0;		// SPB direction opposite from car direction
int16 enc_dir_fault = 0;		// Encoder direction opposite from car direction
int16 uldl_dir_fault = 0;		// UL and DL Direction fault 
int16 vel_decel_diff_fault = 0;	// Velocity decel tracking fault
int16 spb_vel_diff_fault = 0;		// SPB velocity difference fault
int16 spb_vel_diff = 0;			// SPB velocity difference greater than parameter value
int16 lev_to = 0;				// leveling time-out while targeting floor
int16 prev_fault=0;
int16 Current_Fault=0;    	// The Current Fault
int16 Display_Fault= 0;
int16 Rotate_Fault=0;
int16 BRK_Fault = 0;			// Brake lift switch fault
int16 BKS_Run_Fault = 0;		// Brake lift switch fault during run
int16 BRK_Pick_Test = 0;		// Brake lift switch Pick Test flag
int16 BRK_Drop_Test = 0;		// Brake lift switch Drop Test flag

uint8 Door_Lock_GS_DPM_Fault=0;		// single boolean to identify any Door Lock, GS or DPM fault

uint8 Door_Lock_fault=0;			// Door lock fault if dli on on when doors open
uint8 Door_Lock_front_fault=0;		// Door lock fault if dli on on when doors open - front
uint8 Door_Lock_rear_fault=0;		// Door lock fault if dli on on when doors open - rear

uint8 Door_GS_fault=0;				// Door GS fault if gs is on when door are open
uint8 Door_GS_front_fault=0;		// Door GS fault if gs is on when door are open - front
uint8 Door_GS_rear_fault=0;			// Door GS fault if gs is on when door are open - rear

uint8 Door_DPM_fault=0;				// Door DPM fault if dpm is on when door are open
uint8 Door_DPM_front_fault=0;		// Door DPM fault if dpm is on when door are open - front
uint8 Door_DPM_rear_fault=0;		// Door DPM fault if dpm is on when door are open - rear

uint8 Door_DPM_seq_fault=0;			//  Door DPM fault if dpm sequence failed
uint8 Door_DPM_seq_front_fault=0;	//  Door DPM fault if dpm sequence failed - front
uint8 Door_DPM_seq_rear_fault=0;	//  Door DPM fault if dpm sequence failed - rear

int16 RCM_Lock_Fault=0;		// Retiring lock Fault if lock does not make up after RCM
int16 DZ_ON_Fault=0;			// Door zone failed on fault flag
int16 DZA_ON_Fault=0;			// Door zone A failed on fault flag
int16 DZ_OFF_Fault = 0;		// Door zone failed off fault flag
int16 DZA_OFF_Fault = 0;		// Door zone A failed off fault flag
int16 DZsn_OFF_Fault = 0;		// Door zone sn (selector) off fault
int16 UL_ON_Fault=0;			// Detect if UL does not turn off during motion
int16 DL_ON_Fault=0;			// Detect if DL does not turn off during motion
int16 UL_DL_OFF_Fault = 0;	// UL or DL did not turn on at level zone
int16 io_fault=0;				// Io device fault
int16 io_fault_cnt=0;			// Lock up after 3 io faults
int16 limit_fault=0;			// UT,UTS,DT or DTS limit fault
int16 fault[25];  		    // fault buffer holds the faults
int16 old_fault[25];
int16 ins_fault = 0;			// inspection fault flag
int16 ins_fault_latch = 0;	// inspection fault latch
int16 ins_sw_err = 0;			// inspection switch error (up or down run)
int16 ins_status = 0;			// inspection status
								// bit 0: INS (car top inspection)
								// bit 1: MRIN (machine room inspection)
								// bit 2: ACC (access)
								// bit 3: ICI (in car inspection)
								// bit 4; LBP (lock bypass)
								// bit 5: GBP (gate bypass)
								// bit 6: AUTO (automatic)
int16 byp_fault = 0;			// gate or lock bypass fault
int16 byp_fault_latch = 0;	// gate or lock bypass fault latch
int16 gripper_test = 0;		// State flag for gripper test
int16 in_gripper_test = 0;	// In gripper test routine
int16 gripper_fault = 0;		// Rope gripper fault
int16 old_gripper_fault = 0;	// gripper fault memory
int16 gripper_trip_enable = 0; 	// gripper trip enable flag
int16 DBR_Temp_Fault = 0;		// Dynamic Braking Temperature Fault
int16 fault_device = 0;		// device that had the fault
int16 fault_device_2 = 0;		// device 2 that had the fault
int16 fault_prog_flag1 = 0;	// programmer multi-use flag 1
int16 fault_prog_flag2 = 0;	// programmer multi-use flag 2
int16 motion_fault = 0;		// Motion fault occurred: must stop car
int16 redundancy_test = 0;		// redundancy relay test
int16 in_redundancy_test = 0;	// in redundancy relay test
int16 reset_courion = 0;
int16 hydro_on_emp = 0;		// hydro on emergency power
int16 prev_hydro_on_emp = 0;	// previous flag for hydro on emergency power
int16 in_serv_lt = 0;			// in service light control for single automatic pushbutton operation
int16 oserl_enable = 0;		// Special Out of Service Light enable variable
int16 oser_lt = 0;			// Out of service light
int16 oser_bz = 0;			// Out of service buzzer
int16 oser_bz_pulse = 0;		// Out of service buzzer flash
int16 hardware_init_fault = 0;	// 1 if hardware failed on init
int16 drv_rst_cnt = 0;		// Counter for drive ready reset
int16 rdy_flt_latch = 0;		// fault latch for drive ready
bool ResetFaults = false;
int16 cen_fault = 0;		// CEN input fault
int16 door_motor_ovl = 0;		// Door motor overload flag
int16 safe_fault_latch = 0;	// safe fault latch
int16 eef2_enabled = 0;			// set when secondary electric eye input is enabled
int16 eer2_enabled = 0;			// set when secondary rear electric eye input is enabled
int16 fado_fault_cnt = 0;		// front auto swing hall door open fault count
int16 rado_fault_cnt = 0;		// rear auto swing hall door open fault count


//**************************************************************************
//**************************************************************************
// NOTE: Keep fault type to an even number because MRAM writes are word wide
//**************************************************************************
//**************************************************************************

int16 car_has_fault = 0;

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
	int32 sd_count;			// 76-79
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
union fault_union {
	uint8 fltb[max_flt_ix][sizeof(Fault_type)];
	uint16 fltw[max_flt_ix][sizeof(Fault_type)/2];
	struct Fault Flts[max_flt_ix];
}f;

union car_fault_union {
	uint8 fltb[max_flt_ix][sizeof(Fault_type)];
	uint16 fltw[max_flt_ix][sizeof(Fault_type)/2];
	struct Fault Flts[max_flt_ix];
}cf;

int16 Flt_ix = 0;
int16 Car_Flt_ix = 0;
	 
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


int16 FS_DOB_DIS=0;       // fire phase 1 door open button dissable
int16 fs2_to_fs1=0;       // fire phase 2 changing modes to fire phase 1
int16 old_fs2_to_fs1=0;		// old fs2 to fs1
int16 fs2_on = 0;			// fire phase 2 switch on
int16 fs2_hold = 0;		// fire phase 2 switch hold
int16 fs2_start = 0;		// fire phase 2 switch start
int16 fs_on = 0;			// fire phase 1 switch input
int16 prev_fs_on = 0;		// previous value fire phase 1 switch input
int16 fsx_on = 0;			// fire phase 1 aux switch input
int16 mes_on = 0;			// main egress input 
int16 alt_on = 0;			// alternate egress input
int16 mrs_on = 0;			// machine room sensor input
int16 hws_on = 0;			// hoistway sensor input
int16 hws2_on = 0;		// hoistway 2 sensor input
int16 fs1_reset_on = 0;	// fs1 reset input
int16 fs1_effect=0;       // fire service phase 1 is in effect
int16 fs1_effect_reset=0;	// fire phase 1 effect reset flag
int16 fire_sensor=0;		// fire sensor has been activated 1 = main recall, 2 = alternate recall
int16 fire_reset=0;		// fire reset enabled from reset switch
int16 fire_floor=1;		// fire recall floor
int16 old_fire_floor=0;	// old fire floor
int16 old_firef=0;        // old fire service phase flag
int16 old_rfiref=0;        // old rear fire service phase flag
int16 rfiref=0;           // rear door fire service phase flag
int16 Blink_FSL=0;        // Blinking FSL light fire in Machine room or hatch
int16 Extinguish_FSL=0;	// Extinguish Fire Service Output (Phase one before/after device actuation)
int16 off_FSL_state;
int16 firef=0;            // door fire service phase flag
			// firef = 0 not on fire service
			// firef = 1 phase 1 main eggress return
			// firef = 2 phase 1 alternate eggress return
			// firef = 3 phase 1 completed
			// firef = 4 phase 2 door hold operation
			// firef = 5 phase 2 peek-boo door open
			// firef = 6 phase 2 constant pressure dc
			// firef = 7 phase 2 reboot stay put
			// firef = 8 phase 2 momentary dcb door close
int16 fire_stopsw_hold=0;	// used in a17.1 2000 to hold car until car call entered after stop switch hit.
uint16 fire_status=0;		// Fire Status:
						// 	bit0: MES
						//	bit1: ALT
						// 	bit2: MRS
						//  bit3: HWS
						//  bit4: HWS2
						//  bit5: FS or FSX Switch
						//  bit6: unused
						//  bit7: Fire from other car

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
					
int16 fire_fdo_seq = 0;		// denver fire service phase 1 front door open sequence flag
int16 fire_rdo_seq = 0;		// denver fire service phase 1 rear door open sequence flag
int16 fs2_no_fdoor_op = 0;	//  Fire Phase 2 No Front Door Operation after reset flag
int16 fs2_no_rdoor_op = 0;	//  Fire Phase 2 No Rear Door Operation after reset flag

int16 momentary_dcb=0;	//momentary door close button
int16 momentary_rdcb=0;	//momentary rear door close button						
int16 hsf = 0;            // high speed flag
			// hsf = 0 no high speed hsf = 1 go high speed
int16 startf = 0;		// Start flag 1 = valid start, 0 = no start
int16 stallf=0;          // low oil or stalled flag
			// stallf = 0 no low oil condition
			// stallf = 1 low oil go to bottom 
			// stallf = 2 open doors
			// stallf = 3 at bottom dwelling doors
			// stallf = 4 at bottom closing doors
int16 old_stallf=0;
int16 lps_stall=0;		// 1 = low pressure switch stall
int16 nstopf = 0;         // need to stop flag
			// nstopf = 0 no need to stop
			// nstopf = 1 stop at next floor
int16 next_stop = 0;		// next stop floor
int16 short_fl_run = 0;		// Short floor run flag
int16 no_high_speed = 0;		// No high speed output
int16 front_slowdown = 0;		// Front slowdown
int16 rear_slowdown = 0;		// Rear slowdown
#define uc_sd 1				// up hall call slowdown
#define dc_sd 2				// down hall call slowdown
#define cc_sd 4				// car call slowdown
#define uc_do 0x10			// request up call door open
#define dc_do 0x20			// request down call door open
#define cc_do 0x40			// request car call door open
#define iruc_sd 0x100		// up hall call slowdown
#define irdc_sd 0x200		// down hall call slowdown

int16 predof = 0;         // preopen door flag
			// predof = 0 doors are not preopening
			// predof = 1 doors are preopening
int16 prev_procf=0;
int16 procf=0;            // procedure flag
			// procf = 1 elevator is in reset
			// procf = 2 elevator is in inspection
			// procf = 3 Up Fast motion mode
			// procf = 4 Up Transition mode
			// procf = 5 Up Leveling mode
			// procf = 6 Down Fast mode
			// procf = 7 Down Transition mode
			// procf = 8 Down Leveling mode
			// procf = 9 E-Stop mode
			// procf = 10 Not used
			// procf = 11 Emergency Slowdown
			// procf = 12 elevator is in open saftey string
			// procf = 13 elevator is turned off
			// procf = 14 elevator is parked
			// procf = 15 elevator is waiting assignment
			// procf = 16 elevator is working doors
			// procf = 17 elevator is in low oil mode
			// procf = 18 elevator is resetting hydro jack
			// procf = 19 elevator is on Low Oil Pressure mode
			// procf = 20 elevator is in automatic learn hoistway
			// procf = 21 elevator is in emergency power recovery
			// procf = 22 elevator is on hot oil operation
			// procf = 23 elevator is on brake test operation
#define last_procf 23

int32 prev_statusf=0;
int32 statusf=0;						// Status flag
#define sf_S10 			0x1L			// Status: NO LC power
#define sf_HC 			0x2L			// Status: NO HC power
#define sf_SS 			0x4L			// Status: NO SS input
#define sf_READY 		0x8L			// Status: Drive not ready
#define sf_GRIP 		0x10L			// Status: Gripper error
#define sf_IO 			0x20L 			// Status: I/O error during redundancy check
#define sf_INS 			0x40L			// Status: Inspection or lock bypass fault
#define sf_BPI 			0x80L			// Status: Binary Position Input Error
#define sf_POS 			0x100L 			// Status: Position Error
#define sf_AD 			0x200L			// Status: No automatic Doors
#define sf_STP 			0x400L			// Status: Stop switch open
#define sf_DZ 			0x800L			// Status: Door Zone fault
#define sf_GDL 			0x1000L			// Status: Gate or Door lock fault
#define sf_P 			0x2000L 		// Status: No Potential Input
#define sf_DCL 			0x4000L			// Status: No DCL
#define sf_DCC 			0x8000L			// Status: No Door Close Contact
#define sf_BKS 			0x10000L		// Status: Brake lift switch error
#define sf_TOC 			0x20000L		// Status: Top of Car Comm Error
#define sf_DRV 			0x40000L		// Status: Drive Comm Error
#define sf_SPB 			0x80000L		// Status: Safety Processor Board Comm Error
#define sf_DBR 			0x100000L		// Status: DB Resistor Temp. Error
#define sf_SHUTDN 		0x200000L		// Status: Shutdown (too many fault runs)
#define sf_ASTST 		0x400000L		// Status: Annual Safety Test
#define sf_SAFE 		0x800000L		// Status: Waiting for Car to be safe()
#define sf_TLimit 		0x1000000L		// Status: UT,UTS,DT or DTS limit error
#define sf_GTS 			0x2000000L		// Status: GTS input off
#define sf_ULDLDZ 		0x4000000L		// Status: UL, DL and DZ off at floor
#define sf_BKCAN 		0x8000000L		// Status: Brake Board Can Error
#define sf_FSTP 		0x10000000L 	// Status: Fire Fighter Stop Switch
#define sf_SELCAN 		0x20000000L 	// Status: Selector Can error
#define sf_ULDL 		0x40000000L		// Status: UL or DL fault
#define sf_LEV 			0x80000000L		// Status: Leveling fault

int32 prev_statusf2=0;
int32 statusf2=0;						// Status flag
#define sf_HWINIT 		0x1L			// Status: Hardware Init fault
#define sf_FDC			0x2L			// Status: Front Door Closing Fault
#define sf_RDC 			0x4L			// Status: Rear Door Closing Fault
#define sf_LVolt 		0x8L 	  		// Status: Line Voltage Fault
#define sf_DVolt 		0x10L 	  		// Status: Door Voltage Fault
#define sf_BKSR 		0x20L			// Status: Brake lift switch run error 
#define sf_DMO 			0x40L			// Status: Door motor overload
#define sf_LearnHW 		0x80L			// Status: Learn Hoistway Fault
#define sf_PWLUPS 		0x100L			// Status: Power Loss UPS Fault
#define sf_EBKCAN 		0x200L			// Status: Emergency Brake Can error
#define sf_KEBRUN 		0x400L			// Status: KEB Drive Not in Run Mode
#define sf_AF_SHUTDN 	0x800L			// Status: At Floor Shutdown
#define sf_1036 		0x1000L			// Status: 1036 board connected
#define sf_ResetRun 	0x2000L			// Status: Reset run fault
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

int32 prev_statusf3=0;
int32 statusf3=0;						// Status flag
#define sf_SPB_SAF_CAN 		0x1L		// Status: SPB safety CAN error
#define sf_APS_Vel_flt 		0x2L		// Status: APS Velocity Verificatin fault
#define sf_APS_SEL_Brd_Flt 	0x4L		// Status: APS Selector board fault
#define sf_APS_SEL_CAN_Flt	0x8L		// Status: APS Selector CAN fault
#define sf_NTS_SEL_CAN_Flt  0x10L		// Status: NTS APS Selector CAN fault

int32 prev_statusf4=0;
int32 statusf4=0;						// Status flag

int16 safety_string_status=0;
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
#define sss_FFS  0x0400		// Fire Fighter Stop Switch
#define sss_CTS 0x0800		// Car Top Stop switch open
#define sss_HMO 0x1000		// Hoist Motor Overload
#define sss_RLS 0x2000		// Rail Lock Switch
#define sss_MRSW 0x4000		// Machine Room Stop Switch

*/

uint32 run_statusf = 0;			// Run status for each car see bits below:
#define rs_RUN	0x00000001		// Car is running
#define rs_DNR  0x00000002		// Down run signal
#define rs_UP	0x00000004		// Up run signal
#define rs_DL	0x00000008		// Down door zone Limit
#define rs_UL	0x00000010		// Up door zone limit
#define rs_DZ	0x00000020		// Door Zone
#define rs_DLT	0x00000040		// Door Lock Top	
#define rs_DLM	0x00000080		// Door Lock Middle
#define rs_DLB	0x00000100		// Door Lock bottom
#define rs_GS	0x00000200		// Gate Switch
#define rs_RLM	0x00000400		// Rear Door Lock Middle
#define rs_RGS	0x00000800		// Rear Gate Switch
#define rs_DOL  0x00001000		// Door open limit
#define rs_DCL  0x00002000		// Door Close Limit
#define rs_DOLR 0x00004000		// Rear door open limit
#define rs_DCLR 0x00008000		// Rear door close limit
#define rs_EE	0x00010000		// Electric eye or Detector Edge
#define rs_SE	0x00020000		// Safety Edge
#define rs_EER	0x00040000		// Rear Electric eye or Detector edge
#define rs_SER	0x00080000		// Rear Safety Edge
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



int16 power_status = 0;
#define ps_HC   0x0001		// Hall call power loss
#define ps_HCL	0x0002		// Hall call light power loss
#define ps_CC	0x0004		// Car call power loss
#define ps_CCL	0x0008		// Car call light power loss
#define ps_LHC  0x0010		// Lobby Hall common power loss
#define ps_FEP  0x0020		// Fire/Emergency Power Loss

int16 rdoorf=0;           // rear door flag
			// rdoorf = 0 doors closed no action
			// rdoorf = 1 doors are opening
			// rdoorf = 2 doors are dwelling
			// rdoorf = 3 doors are closing
			// rdoorf = 4 doors are nudging closed
int16 ins_rdoorf=0;
int16 rdo_seq = 0;	// Set when rear door reaches full open (RDOL) 
int16 rdoor_en=0;		// rear door enable
int16 rdoor_req=0;	// rear door request to open
int16 rdob_req=0;		// rear door open button request
int16 rcc_req=0;		// rear car call door open request
int16 rnudgst=0;		// rear door nudging status
int16 rdo_to = 0;		// Set when door open limit times out.
int16 ree_to = 0;		// Set when rear electric eye times out.
int16 manual_rdoor = 0;		// set for manual door (added to have swing rear door with automatic front)
int16 drop_rear_rcm = 0;		// set to drop rear RCM
int16 freight_rdoor_alarm = 0;	// freight rear door alarm
int16 rsdo=0;				// Rear swing door open
int16 rsdo_seq=0;			// Rear swing door open sequence
int16 relevel=0;          // relevel
			// relevel = 0 elevator is not releveling
			// relevel = 1 elevator is releveling
int16 relevel_req=0;		// request to relevel 
						// 1 = car is not level and needs to relevel
						// 0 = car is level or in leveling
int16 stop_seq = 0;		// Stop sequence
						// 0 = stopped and run and mc dropped
						// 1 = stopped with soft stop timming
						// 2 = stopped with soft stop timmed out but run and mc timming
int16 leveling_fault_cnt = 0;	// Leveling fault counter (relevel called before the stop sequence went to zero
int16 leveling_fault = 0;	// leveling fault flag: 1 = leveling fault in the up, 2 = leveling fault in the down.
int16 micro_relevel = 0;	// set to 1 for active micro releveling 
int16 lev_startf = 0;		// leveling start flag

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

int16 prev_servf=0;
int16 servf=0;            // service flag
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
			
int16 rhjackf=0;			// Reset Hydro Jack Flag
			// rhjackf = 0	normal operation
			// rhjackf = 1	begin timing for reset jack operation
			// rhjackf = 2	run car down onto jack
			// rhjackf = 3	turn everything off for 2 seconds
			// rhjackf = 4  run car up to 1st floor
			// rhjackf = 5  reset jack complete

int16 stepf=1;            // position stepping flag
			// stepf = 1 allows a change of position (set when car hits DZ)
			// stepf = 0 the position can't change (Cleared when car hits US or DS)
int16 rev_stepf=0;	// Reverse step, 
			// rev_stepf = 0 car is between door zone and reverse direction slowdown
			// rev_stepf = 1 car passed the reverse direction slowdown
int16 pass_fl = 0;	// pass the next floor at leveling speed  (short floor operation)
			// pass_fl = 0 stop at the next floor when hsf is 0
			// pass_fl = 1 pass the next floor, inc or dec position then clear flag
/* Releationship of rev_stepf and stepf
				R=rev_stepf, S=stepf, P=pass_fl
				Down run			Up run				On a reversal direction mid flight stop
				R,S,P				R,S,P					  R,S --> R,S
		DZ		0,1,0				0,1,0					  1,0 --> 1,1
		US		1,1,0				1,0,0					  1,1 --> 1,0

		DS		1,0,0				1,1,0					  0,1 --> 0,1
		DZ		0,1,0				0,1,0
		US		1,1,0				1,0,0

		DS		1,0,0				1,1,0
		DZ		0,1,0				0,1,0
		US		1,1,0				1,0,0

		DS		1,0,1 no sd			1,1,0
		 DS off	1,1,1 no sd			1,1,0
		DS 		1,0,1
		DZ		0,1,1				0,1,0
											   Short floor
		DZ		0,1,0				0,1,1
		US		1,1,0				1,0,1	   dec position on recovery if pass fl set and clear stepf  
		 US off	1,1,0				1,1,1 no sd				  on forward run and set stepf on reversal
		US		1,1,0				1,0,1 no sd

		DS		1,0,0				1,1,0
		DZ		0,1,0				0,1,0
		US		1,1,0				1,0,0

		DS		1,0,0				1,1,0
		DZ		0,1,0	   			0,1,0
*/

int16 medf=0;			 // Mass. Medical Emergency Service
			// medf = 0 no medical emergency service
			// medf = 1 recall car to medical emergency floor
			// medf = 2 car at medical emergency floor with door open (return complete)
			// medf = 3 On EMS car call service
int16 med_latch = 0;	// Switch latch for medical emergency service

int16 hotoilf = 0;	// hot oil temperature fault
					// hotoilf = 0 no hot oil condition
					// hotoilf = 1 hot oil go to the bottom
					// hotoilf = 2 open doors
					// hotoilf = 3 at bottom dwelling doors
					// hotoilf = 4 at bottom closing doors
int16 old_hotoilf = 0;	
int16 old_trace_auto_start = 0;		// old variable to start trace automatically on power up


int16 govrstf = 0;		// Governor reset flag			
int16 govtripf = 0;		// Governor trip flag			
int16 pwl_brakef = 0;		// Flag to pick brake with power loss
		
int16 all_lant_clr = 0;		// all lanterns have been cleared	 
int16 keep_lants_on=0;    	// keeps the cab riding lanterns on
int16 keep_rlants_on=0;   	// keeps the rear cab riding lanterns on
int16 lant_up=0;			// logical boolean for lantern up
int16 lant_dn=0;			// logical boolean for lantern down
int16 rlant_up=0;			// logical boolean for rear lantern up
int16 rlant_dn=0;			// logical boolean for rear lantern down
int16 cab_lant_up=0;		// logical boolean for cab lantern up
int16 cab_lant_dn=0;		// logical boolean for cab lantern dn
int16 cab_rlant_up=0;		// logical boolean for cab rear lantern up
int16 cab_rlant_dn=0;		// logical boolean for cab rear lantern dn
int16 up_dir_arrow = 0;		// logical boolean for up direction arrow
int16 dn_dir_arrow = 0;		// logical boolean for down direction arrow

int16 flight_times[fl_size][fl_size];
int16 Viscosity = 0;
int16 bypass_hc = 0;
int16 hb_en = 0;			// handicap buzzer enable control flag
int16 handicap_buz = 0;	// handicap buzzer flag
int16 hb_cc_buz = 0;		// handicap car call buzzer flag
int16 cc_sec_buz = 0;		// car call security buzzer flag
int16 nudg_buz = 0;		// Nudging buzzer boolean
int16 rnudg_buz = 0;		// Nudging buzzer boolean for rear door
int16 fire_buz = 0;		// Fire service buzzer boolean
int16 med_buz = 0;		// Medical Emergency service buzzer boolean
int16 athcd_in_enable = 0;
int16 MST_enable = 0;		//allow operation of MST if 1
int16 att_buz = 0;		// Attendant Buzzer
int16 att_buz_on = 0;		// Attendant Buzzer On timer 
int16 att_nmb_hc2buz = 0; // Attendant Number of Hall Calls to Buzz
int16 att_nmb_hc = 0;		// Attendant Number of Hall Calls 
int16 update_att_nmb_hc = 0;	//Update the number of hall calls that the system needs to Buzz
int16 att_buz_hc = 0;		// Attendant Buzz on hall call
int16 att_buz_hc_en = 0;	// Attendant Buzz on hall call enable
int16 door_buz = 0;		// Manual door left open buzzer
int16 sabb_buz = 0;		// Sabbath warning buzzer before doors close
int16 sabb_ee_en = 0;		//sabbath electric eye enable
int16 sabbath_active = 0;	// Sabbath active flag (when sabbath is running)
int16 sabbath_start = 0;	// Wait until set to 1 to start sabbath operation
int16 timeout_mandoor = 0; //	timeout manual door for hall calls
int16 stp_sw = 0;			// debounced stp input
int16 ut_limit = 0;		// Debounced ut input
int16 dt_limit = 0;		// Debounced dt input
int16 uts_limit = 0;		// Debounced uts input
int16 dts_limit = 0;		// Debounced dts input
int16 ets_limit = 0;		// Debounced ets input
int16 start_floor;
int16 no_fault;
int16 park_fl = 0;			// parking floor
int16 lby_req_fl = 0;		// lobby request floor
int16 zone_fl=0;			// floor to zone car
int16 latch_HEOF=0;		// Latch HEOF operation
int16 latch_CEOF=0;		// Latch EOF operation
int16 latch_JAIL=0;		// Latch for Jail break input
int16 galcom_ver = 0;
int16 galcom_rev = 0;
int16 prev_disp_com2 = 0;		// previous dispatcher


// interger variables used throughout the program
int16 nmb_carcalls=0;		// number of latched car calls
int16 all_cars_ots=0;     // all cars are out of service
int16 nmb_cars_in_svc = 0;			// number of cars in service
int16 stops[fl_size * 2];          // keep an array of the sequence of stops
int16 nmbstops;           // number of stops to clear calls
int16 carcb[fl_size];          // car call buffer value = 1 there is a call
int16 rcarcb[fl_size];         // rear car call buffer value = 1 there is a rear call
int16 closetry = 0;       // number of times the doors have tryed to close
int16 rclosetry = 0; 		// number of times the rear doors have tryed to close
int16 safe_closetry = 0;  	// number of times the doors have tryed to close to get safe
int16 safe_rclosetry = 0; 	// number of times the rear doors have tryed to close to get safe
int16 codebfl=0;          // code blue floor value is position of emergency
int16 vipfl=0;			// priority service (vip) floor
int16 doort=50;           // door dwell time
int16 rdoort=50;          // rear door dwell time
int16 dtmrf = 0;			// door dwell timer flag
#define cc_dt 1			// car call door time was set
#define hc_dt 2			// hall call door time was set
int16 upcb[fl_size];           // up hall call buffer value = 1 there is a call
int16 rupcb[fl_size];          // rear up hall call buffer value = 1 there is a call
int16 downcb[fl_size];         // down hall call buffer value = 1 there is a call
int16 rdowncb[fl_size];        // rear down hall call buffer value = 1 there is a call
int16 nmbflts=0;          // number of faults that have occured
int16 position;           // the position of the elevator
int16 prev_position;	  // the previous position of the elevator
int16 scnd_pos;			  // temporary position between floors 
//								GOING UP					GOING DN
//							position 	scnd_pos        position 	scnd_pos
//				5---DZ----------5-----------5---------------5-----------5-------------
//								5			4               5			4
//					US			5			4               5			4
//								4			4               5			4
//					DS			4			4               4			4
//								4			4               4			4
//				4---DZ----------4-----------4---------------4-----------4
//								4			3               4			3
//					US			4			3               4			3
//								3			3               4			3
//					DS			3			3               3			3
//								3			3               3			3
//				3---DZ----------3-----------3---------------3-----------3
//								3			2               3			2
//					US			3			2               3			2
//								2			2               3			2
//					DS			2			2               2			2
//								2			2               2			2
//				2---DZ----------2-----------2---------------2-----------2
//								2			1               2			1
//					US			2			1               2			1
//								1			1               2			1
//					DS			1			1               1			1
//								1			1               1			1
//				1---DZ----------1-----------1---------------1-----------1
int16 prev_scnd_pos;
int16 preset = 0;			// set to 1 when binary preset is set
int16 preset_pos = 0;		// set to 1 to update the preset the position
int16 Lev_Mode=0;         // Used for emergency slowdown traction's
int16 Hoistway_Learned=0; // Hoistway Learned 1 to run automatic
int16 Field_Force=0;		// Field Force
int16 rset=1;             // in reset and have not found a floor yet
int16 rset_run_cnt=0;			// reset run count: consecutive times the car runs to find a floor
int16 rset_error_cnt=0;		// max times to log reset error looping in reset mode
int16 baud_default = 0;
int16 ext_temperature=0;	// External temperature
int16 mr_temperature=0;	// Motor room temperature
int16 safe_override_seq = 0;		// safe override sequence to lift brake on inspection
int16 eef_testtry = 0;		// front electric eye test try count 
int16 eer_testtry = 0;		// rear electric eye test try count 

int16 dol_fail_cnt = 0;		// door open limit fail count
int16 rdol_fail_cnt = 0;	// door open limit fail count
bool dol_fail = 0;			// door open limit fail flag
bool rdol_fail = 0;			// rear door open limit fail flag

float Max_Lant_Time = 0;

int16 epr_dirf = 0;				// Emergency power recovery direction
int16 prev_epr_dirf = 0;			// Emergency power recovery direction debounce flag

int16 speed_clamp[9] = {0,0,0,0,0,0,0,0,0};	// Software velocity clamp stored in non-volitile memory
#define sc_ins_lev 0
#define sc_UT_DT   1
#define sc_UT1_DT1 2
#define sc_UT2_DT2 3
#define sc_UT3_DT3 4
#define sc_UTS_DTS 5
#define sc_UT4_DT4 6
#define sc_UT5_DT5 7
#define sc_UT6_DT6 8

int16 dpp_store_tmr = 0;
int16 enc_vel_timer=0;		// velocity timer for encoder velocity calculation 
int16 enc_dir_timer=0;
int16 enc_save_timer=0;
int16 enc_start_read_cnt=0;
int16 dz_dbn_timer=0;
int16 drive_update_timer=0;
int16 dpyspi_timer=0;
int16 iospi_timer=0;
int16 can_sel_timer=0;
int16 can_sync_timer=0;
int16 ovs_dbn_tim=0;
int16 Overspeed_Test = 0;
int16 Buffer_Test = 0;
int16 NTS_Test = 0;
int16 ETS_Test = 0;
int16 don_delay = 0;			// drive on delay timer

// Status Bit flags
uint8 Door_Req_Flags = 0;
uint8 Door_Seq_Flags = 0;
uint8 Fault_Bits[4] = {0, 0, 0, 0};
uint8 Nudgst_Flags = 0;
uint8 Call_Flags = 0;


//  Local variable for service timers
int16 c_tmr_svc[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int16 c_tmr_svc_en[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int16 c_tmr_svc_logic[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


// Security variables

unsigned char zero_Code[4]={0,0,0,0};		// Holdes the security code being entered

int16 Security_Type[car_size];	// 0 = No Security, 
										// 1 = Car call lockouts in the car,  CCS
										// 2 = Group car call lockouts,  GCS
										// 4 = Group car call lockouts GCS and seperate hall call lockouts on rear cc lockout board	HCS
										// 8 = Group car and hall call lockouts	FLS
										// 16 = Car lockout override using lockout inputs on car   OCS
										// 32 = Seperate hall call security inputs on the GPIO boards  HCS
										// 64 = Car call latch ouputs to security system (use or'ed with option 1 (cc lockouts))
										// 128 = Override car call lockouts through group (use or'ed with option 1 (cc lockouts))
										// 256 = Hall Call Lockout on serial aux inputs
int16 Car_Call_Lockouts = 0; // Car call lockouts and car reader security
int16 Lockout_Override = 0;	// Key input to override group floor or ids floor security (uses car call lockout inputs)
int16 Sec_Fl_Mask = 0;		// Security Floor mask select switch
int16 ind_override = 0;		// Independent overrides security when set
int16 ind_secf_override = 0;	// Independent overrides security even from SECF tables
int16 med_override = 0;		// Medical service overrides security when set
int16 secdis_override = 0;	// Security disabling input overrides security when set
int16 sec_recall = 0;			// Set to recall car on security.
int16 recall_dir = 0;			// recall in the same direction as the recall floor 
int16 sec_recall_complete = 0;	// Set when security recall is complete.
int16 sec_panic_recall = 0;		// panic recall flag to cancel car calls and recall to security floor
int16 ind_sec_recall = 0;			// Initiate Independent security recall

uint32 cc_sec_mask[2] = {0,0};		// Car call security mask
uint32 rcc_sec_mask[2] = {0,0};		// Rear Car call security mask
uint32 master_cc_sec[2] = {0,0};		// Car call security mask
uint32 master_rcc_sec[2] = {0,0};		// Rear Car call security mask
uint32 master_cc_sec_prev[2] = {0,0};		// Car call security mask
uint32 master_rcc_sec_prev[2] = {0,0};		// Rear Car call security mask


int16 car_alrm = 0;

// Group I/O's common to car and group ... Must be set in car z6 cpu

uint8 gin_EMP = 0;
uint8 gin_EPT = 0;
uint8 gin_ATTHC = 0;
uint8 gin_SAB = 0;
uint8 gin_SECFM = 0;
uint8 gin_HCP = 0;
uint8 gout_GSERV = 0;


	
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

struct car_Grp_Stat_struct car_Grp_Stat;




// int32 variables used throught the program
#define nmblvar 20
int32 lvars[nmblvar];
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



int16 day_of_week;
int16 old_day_of_week;

int16 t_door_open = 30;
int16 t_door_dwell = 30;
int16 t_door_close = 30;
int16 t_rdoor_open = 30;
int16 t_rdoor_dwell = 30;
int16 t_rdoor_close = 30;


int16 sr_cc_mask = 0;	// second riser mask has been set when 1
int16 tr_cc_mask = 0;	// third riser mask has been set when 1


// float floating point variables used throught the program
#define nmbdvar 20
float dvars[nmbdvar];


//#pragma section sdata_type ".sdata"

__declspec(section ".sdata") unsigned char *ccmsk[car_size];     	// car call mask pointer to screen unused calls
__declspec(section ".sdata")unsigned char *ucmsk[car_size];     	// up call mask pointer to screen unused calls
__declspec(section ".sdata")unsigned char *dcmsk[car_size];     	// down call mask pointer to screen unused calls
__declspec(section ".sdata")unsigned char *rccmsk[car_size];    	// rear car call mask pointer to screen unused calls
__declspec(section ".sdata")unsigned char *rucmsk[car_size];    	// rear up call mask pointer to screen unused calls
__declspec(section ".sdata")unsigned char *rdcmsk[car_size];    	// rear down call mask pointer to screen unused calls


unsigned char ccsmsk[car_size][fl_size];    // car call security mask to screen unused calls
unsigned char ucsmsk[car_size][fl_size];    // up call security mask to screen unused calls
unsigned char dcsmsk[car_size][fl_size];    // down call security mask to screen unused calls
unsigned char rccsmsk[car_size][fl_size];   // rear car call security mask to screen unused calls
unsigned char rucsmsk[car_size][fl_size];   // rear up call security mask to screen unused calls
unsigned char rdcsmsk[car_size][fl_size];   // rear down call security mask to screen unused calls

unsigned char ccs2msk[car_size][fl_size];    // car call security mask to screen unused calls
unsigned char ucs2msk[car_size][fl_size];    // up call security mask to screen unused calls
unsigned char dcs2msk[car_size][fl_size];    // down call security mask to screen unused calls
unsigned char rccs2msk[car_size][fl_size];   // rear car call security mask to screen unused calls
unsigned char rucs2msk[car_size][fl_size];   // rear up call security mask to screen unused calls
unsigned char rdcs2msk[car_size][fl_size];   // rear down call security mask to screen unused calls

unsigned char ccs3msk[car_size][fl_size];    // car call security mask to screen unused calls
unsigned char ucs3msk[car_size][fl_size];    // up call security mask to screen unused calls
unsigned char dcs3msk[car_size][fl_size];    // down call security mask to screen unused calls
unsigned char rccs3msk[car_size][fl_size];   // rear car call security mask to screen unused calls
unsigned char rucs3msk[car_size][fl_size];   // rear up call security mask to screen unused calls
unsigned char rdcs3msk[car_size][fl_size];   // rear down call security mask to screen unused calls

unsigned char ccs4msk[car_size][fl_size];    // car call security mask to screen unused calls
unsigned char ucs4msk[car_size][fl_size];    // up call security mask to screen unused calls
unsigned char dcs4msk[car_size][fl_size];    // down call security mask to screen unused calls
unsigned char rccs4msk[car_size][fl_size];   // rear car call security mask to screen unused calls
unsigned char rucs4msk[car_size][fl_size];   // rear up call security mask to screen unused calls
unsigned char rdcs4msk[car_size][fl_size];   // rear down call security mask to screen unused calls

unsigned char ccnmsk[car_size][fl_size];    // car call normal mask to screen unused calls
unsigned char ucnmsk[car_size][fl_size];    // up call normal mask to screen unused calls
unsigned char dcnmsk[car_size][fl_size];    // down call normal mask to screen unused calls
unsigned char rccnmsk[car_size][fl_size];   // rear car call normal mask to screen unused calls
unsigned char rucnmsk[car_size][fl_size];   // rear up call normal mask to screen unused calls
unsigned char rdcnmsk[car_size][fl_size];   // rear down call normal mask to screen unused calls

unsigned char irccmsk[car_size][fl_size];    	// ir car call mask to screen unused calls
unsigned char irucmsk[car_size][fl_size];     	// ir up call mask pointer to screen unused calls
unsigned char irdcmsk[car_size][fl_size];     	// ir down call mask pointer to screen unused calls
unsigned char rirccmsk[car_size][fl_size];   	// rear ir car call mask to screen unused calls
unsigned char rirucmsk[car_size][fl_size];    	// rear ir up call mask pointer to screen unused calls
unsigned char rirdcmsk[car_size][fl_size];    	// rear ir down call mask pointer to screen unused calls

// ir masks backup for second riser
unsigned char irccmsk2[car_size][fl_size];    	// ir car call mask to screen unused calls
unsigned char irucmsk2[car_size][fl_size];     	// ir up call mask pointer to screen unused calls
unsigned char irdcmsk2[car_size][fl_size];     	// ir down call mask pointer to screen unused calls
unsigned char rirccmsk2[car_size][fl_size];   	// rear ir car call mask to screen unused calls
unsigned char rirucmsk2[car_size][fl_size];    	// rear ir up call mask pointer to screen unused calls
unsigned char rirdcmsk2[car_size][fl_size];    	// rear ir down call mask pointer to screen unused calls

unsigned char trccmsk[car_size][fl_size];    	// tr car call mask to screen unused calls
unsigned char trucmsk[car_size][fl_size];     	// tr up call mask pointer to screen unused calls
unsigned char trdcmsk[car_size][fl_size];     	// tr down call mask pointer to screen unused calls
unsigned char rtrccmsk[car_size][fl_size];   	// rear tr car call mask to screen unused calls
unsigned char rtrucmsk[car_size][fl_size];    	// rear tr up call mask pointer to screen unused calls
unsigned char rtrdcmsk[car_size][fl_size];    	// rear tr down call mask pointer to screen unused calls

unsigned char hpccmsk[car_size][fl_size];    // high priority floor car call mask to screen unused calls
unsigned char hpucmsk[car_size][fl_size];    // high priority floor up call security mask to screen unused calls
unsigned char hpdcmsk[car_size][fl_size];    // high priority floor down call security mask to screen unused calls
unsigned char rhpccmsk[car_size][fl_size];   // high priority floor rear car call security mask to screen unused calls
unsigned char rhpucmsk[car_size][fl_size];   // high priority floor rear up call security mask to screen unused calls
unsigned char rhpdcmsk[car_size][fl_size];   // high priority floor rear down call security mask to screen unused calls

unsigned char xa_uc_msk[fl_size];			// cross assignment up call mask to decide which group gets the floor hall call 1=gal,2=other,3=both 
unsigned char xa_dc_msk[fl_size];			// cross assignment down call mask to decide which group gets the floor hall call 1=gal,2=other,3=both
unsigned char valid_fl[fl_size];		// valid floors for this car
unsigned char stag_rmsk[fl_size];	// staggered rear floor mask
unsigned char heavy_door_fmsk[fl_size];	// heavy door front floor mask
unsigned char heavy_door_rmsk[fl_size];	// heavy door rear floor mask
unsigned char stag_ado_msk[fl_size];	// staggered auto door floor mask
unsigned char stag_ado_rmsk[fl_size];	// staggered auto door rear floor mask

unsigned char pbdoor_fmsk[fl_size];		// push button door front floor mask
unsigned char pbdoor_rmsk[fl_size];		// push button door rear floor mask

unsigned char sab_ccmsk[fl_size];     	// Sabbath service car call mask pointer to screen unused calls
unsigned char sab_ucmsk[fl_size];     	// Sabbath service up call mask pointer to screen unused calls
unsigned char sab_dcmsk[fl_size];     	// Sabbath service down call mask pointer to screen unused calls
unsigned char sab_rccmsk[fl_size];    	// Sabbath service rear car call mask pointer to screen unused calls
unsigned char sab_rucmsk[fl_size];    	// Sabbath service rear up call mask pointer to screen unused calls
unsigned char sab_rdcmsk[fl_size];   	// Sabbath service rear down call mask pointer to screen unused calls

unsigned char flmrk[fl_size][2];     // floor markings
unsigned char alt_flmrk[fl_size][2]; // alternate floor markings
unsigned char Blind_flmrk[2][2];     // Blind floor markings to display on digital PI indicator.
unsigned char xpi_pos[fl_size];
unsigned char jobnm[26];        // job name
unsigned char add1[26];         // address line #1
unsigned char add2[26];         // address line #2
unsigned char cmp[26];          // company name
unsigned char pass[8][26];      // password
unsigned char user[8][26];      // user name
unsigned char vidmes[3][26];    // video message three lines


// Car call color variables
																				
int16 ccOnBright=0;			// Call On Brightness
int16 ccOffBright=0;		// Call Off Brightness

int16 ccOnRed=0;			// Call On Red
int16 ccOnGreen=0;			// Call On Green
int16 ccOnBlue=0;			// Call On Blue

int16 ccOffRed=0;			// Call Off Red
int16 ccOffGreen=0;			// Call Off Green
int16 ccOffBlue=0;			// Call Off Blue

int16 ccSecOnRed=0;			// Call Security On Red
int16 ccSecOnGreen=0;		// Call Security On Green
int16 ccSecOnBlue=0;		// Call Security On Blue

int16 ccAttUpOnRed=0;		// Call Attendant Up On Red
int16 ccAttUpOnGreen=0;		// Call Attendant Up On Green
int16 ccAttUpOnBlue=0;		// Call Attendant Up On Blue

int16 ccAttDnOnRed=0;		// Call Attendant Down On Red
int16 ccAttDnOnGreen=0;		// Call Attendant Down On Green
int16 ccAttDnOnBlue=0;		// Call Attendant Down On Blue

int16 copFireOnRed=0;		// COP Fire On Red
int16 copFireOnGreen=0;		// COP Fire On Green
int16 copFireOnBlue=0;		// COP Fire On Blue

int16 copMedOnRed=0;		// COP Medical On Red
int16 copMedOnGreen=0;		// COP Medical On Green
int16 copMedOnBlue=0;		// COP Medical On Blue

int16 copEmOnRed=0;			// COP Emergency On Red
int16 copEmOnGreen=0;		// COP Emergency On Green
int16 copEmOnBlue=0;		// COP Emergency On Blue

int16 copOTSOnRed=0;		// COP OTS On Red
int16 copOTSOnGreen=0;		// COP OTS On Green
int16 copOTSOnBlue=0;		// COP OTS On Blue

int16 ccOnBright_prev=0;		
int16 ccOffBright_prev=0;		

int16 ccOnRed_prev = 0;
int16 ccOnGreen_prev = 0;
int16 ccOnBlue_prev = 0;

int16 ccOffRed_prev = 0;
int16 ccOffGreen_prev = 0;
int16 ccOffBlue_prev = 0;

int16 ccSecOnRed_prev = 0;
int16 ccSecOnGreen_prev = 0;
int16 ccSecOnBlue_prev = 0;

int16 ccAttUpOnRed_prev=0;		// Call Attendant Up On Red
int16 ccAttUpOnGreen_prev=0;	// Call Attendant Up On Green
int16 ccAttUpOnBlue_prev=0;		// Call Attendant Up On Blue

int16 ccAttDnOnRed_prev=0;		// Call Attendant Down On Red
int16 ccAttDnOnGreen_prev=0;	// Call Attendant Down On Green
int16 ccAttDnOnBlue_prev=0;		// Call Attendant Down On Blue

int16 copFireOnRed_prev=0;		// COP Fire On Red
int16 copFireOnGreen_prev=0;	// COP Fire On Green
int16 copFireOnBlue_prev=0;		// COP Fire On Blue

int16 copMedOnRed_prev=0;		// COP Medical On Red
int16 copMedOnGreen_prev=0;		// COP Medical On Green
int16 copMedOnBlue_prev=0;		// COP Medical On Blue

int16 copEmOnRed_prev=0;		// COP Emergency On Red
int16 copEmOnGreen_prev=0;		// COP Emergency On Green
int16 copEmOnBlue_prev=0;		// COP Emergency On Blue

int16 copOTSOnRed_prev=0;		// COP OTS On Red
int16 copOTSOnGreen_prev=0;		// COP OTS On Green
int16 copOTSOnBlue_prev=0;		// COP OTS On Blue

int16 CC_ColorOnOffChecksum = 0;


// Drive Type	1=hydro 2=Vector cl-ndf, 3=vector cl-df, 4=gen cl-df, 
//              5=scr cl-df (DSD-412), 6=vector ol-ndf, 7=gen cl-ndf, 8=scr cl-ndf (DSD-412),
//				9=hydro soft starter, 10-Quatro SCR-df, 11-Quatro AC-df,
//              12=KEB AC-df, 14=Delta VDF-VL, 15 = Yaskawa (L1000), 16 = New KEB AC
int8 Drive_Type[car_size]; 
int8 valid_cb_car[car_size];	  		// set to 1 for each valid code blue car
int8 valid_cbpr_car[car_size];	  	// set to 1 for each valid code blue primary riser car
int8 valid_cbsr_car[car_size];	  	// set to 1 for each valid code blue secondary riser car
int8 valid_ep_car[car_size];	  		// set to 1 for each valid code emergency power car
int8 valid_vip_car[car_size];	  		// set to 1 for each valid vip car

#define connmb 158
int16 cons[connmb];
int16 consB[connmb]; //  for use with board testing programs. // tagwbg 19
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
#define sercom 9   			// 009C - cons[sercom] 1= Serial Hall Call, 2=Serial Car, 4=CC in COP, 8=drive, 16=door, 32=hc drivers, 64=Serial hall lantern
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
#define sel_type 57			// 057C - cons[sel_type]: 0=Tape, 1=Rail Target Selector/SPB Governor Encoder, 2=Encoded Tape, 3=Rail Target/CPU Gov Enc, 4=Cedes Selector
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



//  #define fvcputiming 434			// Cpu timing output control

#define DIAGLED 		0
#define INCTIM 			0x0001
#define MAIN_1SEC 		0x4000
#define GRP_IO      	0x0002
#define TICK_10_MSEC 	0x0004
#define GRP_INCTIM 		0x0008



//*******************
// Field Vairables
//*******************

// Field adjustable variables
unsigned char telnmb[20];
#define nmbfvar 600				// changed from 360 to 420 6/7/10
int16 fvars[nmbfvar];
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

 
int16 fvars_deci[nmbfvar] ={	// 0 if fvars has no decimal place 1 if it does
	1,		// 000 - decimal - Y Delta Time
	1,		// 001 - decimal - Fault Time
	1,		// 002 - decimal - Reset Time
	0,		// 003 - decimal - Double Stroke on Lanterns
	1,		// 004 - decimal - Lantern On Time for Double Stroking
	1,		// 005 - decimal - Lantern Off Time for Double Stroking
	1,		// 006 - decimal - Handicap Floor Passing Beep Time
	1,		// 007 - decimal - Door Protection Time
	1,		// 008 - decimal - Nudge Time
	1,		// 009 - decimal - Pre-opening Delay Time
	1,		// 010 - decimal - Car Call Door Dwell Time
	1,		// 011 - decimal - Hall Call Door Dwell Time
	1,		// 012 - decimal - Lobby Call Door Dwell Time
	1,		// 013 - decimal - Extra Door Dwell Time
	1,		// 014 - decimal - Non Interference Time
	0,		// 015 - decimal - Lobby Floor
	0,		// 016 - decimal - Fire Service Phase 1 Main Egress Floor
	0,		// 017 - decimal - Fire Service Phase 1 Alternate Egress Floor
	1,		// 018 - decimal - Stall Time
	1,		// 019 - decimal - Soft Stop Time
	1,		// 020 - decimal - Generator Run Time
	1,		// 021 - decimal - Brake Pick Delay Time
	1,		// 022 - decimal - Brake Drop Delay Time
	0,		// 023 - decimal - Soft Start Jerk Rate
	0,		// 024 - decimal - Acceleration Rate
	0,		// 025 - decimal - Deceleration Jerk Rate
	0,		// 026 - decimal - Deceleration Rate
	1,		// 027 - decimal - Floor Target Distance
	1,		// 028 - decimal - Leveling Speed
	1,		// 029 - decimal - Relevel Speed
	0,		// 030 - decimal - Inspection Speed
	0,		// 031 - decimal - Year
	0,		// 032 - decimal - Month
	0,		// 033 - decimal - Day
	0,		// 034 - decimal - Hour
	0,		// 035 - decimal - Minute
	0,		// 036 - decimal - Roll Over Jerk Rate
	0,		// 037 - decimal - EE Memory Size
	0,		// 038 - decimal - Annual Safety Test Year
	0,		// 039 - decimal - Annual Safety Test Month
	0,		// 040 - decimal - Annual Safety Test Day
	2,		// 041 - decimal - Pattern Delay
	0,  	// 042 - decimal - Field Weaken Velocity Percent
    0,  	// 043 - decimal - Encoder PPR
	1,  	// 044 - decimal - Encoder RPM
	0,		// 045 - decimal - User Interface Baud Rate 
	0,		// 046 - decimal - Parking
	0,		// 047 - decimal - Emergency Power Return Floor
	0,		// 048 - decimal - Number of Cars to Run on Emergency Power
	0,		// 049 - decimal - First Car to Return on Emergency Power
	0,		// 050 - decimal - First Car Selected on Emergency Power
	1,		// 051 - decimal - Return Car Time-out Time
	0,		// 052 - decimal - DOB Opens Door On Nudging Is Set
	0,		// 053 - decimal - Emergency Dispatch Enable
	0,		// 054 - decimal - Number of Cars Requested at Lobby
	0,		// 055 - decimal - Next Up Selection
	0,		// 056 - decimal - Code Blue Car Select
	0,		// 057 - decimal - IR Car Select
	0,		// 058 - decimal - Brake Lift Switch
	0,		// 059 - decimal - Load Weighing Anti-nuisance Count
	0,		// 060 - decimal - Fire Switch Location
	0,		// 061 - decimal - ETA Minimum Time
	0,		// 062 - decimal - ETA for Coincident Car Call
	0,		// 063 - decimal - Video Position for Car 1
	0,		// 064 - decimal - Video Position for Car 2
	0,		// 065 - decimal - Video Position for Car 3
	0,		// 066 - decimal - Video Position for Car 4
	0,		// 067 - decimal - Video Position for Car 5
	0,		// 068 - decimal - Video Position for Car 6
	0,		// 069 - decimal - Run Count with No Passengers to Cancel Car Calls
	0,		// 070 - decimal - Independent Override Security
	1,		// 071 - decimal - Brake Pick Delay Time on Relevel
	0,		// 072 - decimal - Brake Cool on Relevel
	1,		// 073 - decimal - Up peak Trigger Time
	0,		// 074 - decimal - Up Peak Trigger Count
	0,		// 075 - decimal - Up Peak Car Call Count
	1,		// 076 - decimal - Up Peak Duration Time
	1,		// 077 - decimal - Down Peak Trigger Time
	0,		// 078 - decimal - Down Peak Trigger Count
	1,		// 079 - decimal - Down Peak Duration Time
	1,		// 080 - decimal - Parking Delay Time
	0,		// 081 - decimal - Hall Call Cross Assignment
	0,		// 082 - decimal - Hall Call Cross Assignment ETA
	0,		// 083 - decimal - Medical Emergency Floor
	0,		// 084 - decimal - Medical Emergency Hall Switch location
	0,		// 085 - decimal - Fire Service Motor Room Sensor Return Floor
	0,		// 086 - decimal - Fire Service Hoistway Sensor Return Floor
	0,		// 087 - decimal - Phase 1 Fire Recall Reset
	0,		// 088 - decimal - Acceleration and Deceleration Limit at Final Stop
	2,		// 089 - decimal - Pattern Delay During Relevel
	0,		// 090 - decimal - Delay Start Until Drive On
	0,		// 091 - decimal - Brake Opto Trigger Delay
	0,		// 092 - decimal - Invert Logic of Brake Lift Switch
	0,		// 093 - decimal - Invert Logic of In-service Light Output or In-use Light or Out of Service Alarm
	0,		// 094 - decimal - Ability to Change a Car Other Than 1 or 2 to Become the Master
	1,		// 095 - decimal - Time-out Video Display
	0,		// 096 - decimal - Close Doors While Trying to Run on Inspection
	0,		// 097 - decimal - Parking Floor 1
	0,		// 098 - decimal - Parking Floor 2
	0,		// 099 - decimal - Parking Floor 3
	0,		// 100 - decimal - Parking Floor 4
	0,		// 101 - decimal - Parking Floor 5
	0,		// 102 - decimal - Parking Width
	1,		// 103 - decimal - Percent of Load to Balance the Counterweights (Balanced Load Percent)
	2,		// 104 - decimal - Drive Starting Torque Multiplier (Torque Amount Variable)
	0,		// 105 - decimal - Load Bypass Percent
	0,		// 106 - decimal - Load Anti-nuisance Percent
	0,		// 107 - decimal - Load Dispatch Percent
	0,		// 108 - decimal - Load Overload Percent
	0,		// 109 - decimal - IR Control Flag
	0,		// 110 - decimal - Brake Pick Voltage
	1,		// 111 - decimal - Brake Pick Duration Time
	0,		// 112 - decimal - Brake Hold Voltage
	0,		// 113 - decimal - Brake Relevel Voltage
	0,		// 114 - decimal - Brake AC Line to Line Voltage
	1,		// 115 - decimal - Brake Resistance
	1,		// 116 - decimal - Attendant Buzzer Delay Time
	0,		// 117 - decimal - Hall Fire Aux. Switch Enable
	0,		// 118 - decimal - Operation of Hall Fire Light
	0,		// 119 - decimal - COP/Remote CC Select
	1,		// 120 - decimal - Door Open or Close Delay Time
	0,		// 121 - decimal - Security Recall Option
	0,		// 122 - decimal - DOB Overrides Security
	1,		// 123 - decimal - Manual Door Buzzer Delay Time
	0,		// 124 - decimal - Security Floor
	1,		// 125 - decimal - Retiring Cam Pick Delay Time
	1,		// 126 - decimal - Short Door Time From a Reopen Signal
	0,		// 127 - decimal - Second IR Car Select
	0,		// 128 - decimal - Stop At Lobby
	0,		// 129 - decimal - Invert Cab Light Fan Output
	0,		// 130 - decimal - Invert Temperature Low Switch (TPL)
	0,		// 131 - decimal - Invert Low Pressure Switch
	0,		// 132 - decimal - Invert Low Oil Switch
	0,		// 133 - decimal - Nudging with No Onward Call
	0,		// 134 - decimal - Fire Option Flag
	0,		// 135 - decimal - Attendant Car Call Flag
	1,		// 136 - decimal - Code Blue Door Open Time
	0,		// 137 - decimal - Request Ind Car For Code Blue
	0,		// 138 - decimal - Code Blue Light Control (Flash Light)
	0,		// 139 - decimal - Hospital Service Door Close on Car Call
	1,		// 140 - decimal - Gripper Reset Time(r)
	0,		// 141 - decimal - Independent Door Close on Car Call
	2,		// 142 - decimal - Torque Offset
	0,		// 143 - decimal - Gripper Test From Safety Processor Board
	0,		// 144 - decimal - Single Automatic Pushbutton Control
	0,		// 145 - decimal - Door Open When DOL Signal Not Active
	0,		// 146 - decimal - Previously: fvdisglt - Disable Gate and Lock Test for New York City
	1,		// 147 - decimal - Relevel Start Speed
	0,		// 148 - decimal - Emergency Deceleration Rate
	0,		// 149 - decimal - Disable DCB from Shortening Door Time
	0,		// 150 - decimal - Return to Lobby Option
	0,		// 151 - decimal - Video Position for Car 7
	0,		// 152 - decimal - Video Position for Car 8
	0,		// 153 - decimal - Parking Type
	0,		// 154 - decimal - Parking Floor 6
	0,		// 155 - decimal - Parking Floor 7
	0,		// 156 - decimal - Non-Simultaneous Doors Operation
	0,		// 157 - decimal - Pre-open Doors
	0,		// 158 - decimal - Over Speed Percent
	0,		// 159 - decimal - Position Pulse Count Update
	0,		// 160 - decimal - Position Pulse Delay Error
	0,		// 161 - decimal - Gripper Buzzer Enable
	1,		// 162 - decimal - Relevel Delay Time
	0,		// 163 - decimal - Fire Option 2
	0,		// 164 - decimal - Close Gate on Swing Door with No Preference
	0,		// 165 - decimal - Leveling Fault Count
	2,		// 166 - decimal - Torque Amount in Down Direction
	2,		// 167 - decimal - Ramp Torque Time During Pattern Delay
	1,		// 168 - decimal - Electric Eye Time-out Time
	0,		// 169 - decimal - Cars Out of Service Hall Call Cancel
	0,		// 170 - decimal - Nudging Control
	1,		// 171 - decimal - Fire Phase 1 Door Time-out (Ind and Att)
	0,		// 172 - decimal - No Hall Call Button Door Reopen with Onward Call
	0,		// 173 - decimal - Drive Ready Reset Count
	1,		// 174 - decimal - Return to Lobby Door Dwell Time
	0,		// 175 - decimal - Next Up Lobby Lantern Control
	0,		// 176 - decimal - Next Up Preference Control
	0,		// 177 - decimal - Second Riser Control
	0,		// 178 - decimal - Second Riser Lantern Control
	0,		// 179 - decimal - Attendant ETA Preference Time
	0,		// 180 - decimal - Handicap Buzzer Disabled at Invalid Floor	 
	1,		// 181 - decimal - VIP Door Open Time
	0,		// 182 - decimal - VIP Operation Flags
	0,		// 183 - decimal - Number of VIP cars
	0,		// 184 - decimal - Emergency Power Op led (OP) Function
	0,		// 185 - decimal - Emergency Power Park led (PRK) Function
	0,		// 186 - decimal - Hall Elevator Off Options
	0,		// 187 - decimal - Drive Baud Rate
	0,		// 188 - decimal - Drive Update Rate
	0,		// 189 - decimal - Alternate Lobby Floor
	0,		// 190 - decimal - Alternate Parking Floor
	0,		// 191 - decimal - Lobby Request Control
	0,		// 192 - decimal - Handicap Car Capacity
	1,		// 193 - decimal - Freight Door Close Time-out
	0,		// 194 - decimal - Handicap Car Wait Time
	1,		// 195 - decimal - Short Floor Distance
	0,		// 196 - decimal - Retiring Cam Control
	0,		// 197 - decimal - Recall from Fire Alternate Floor
	1,		// 198 - decimal - Auto Learn Hoistway Stall Time
	2,		// 199 - decimal - Position Pulse Count Error
	1,		// 200 - decimal - Attendant Buzzer Time (When Off)
	1,		// 201 - decimal - Attendant Buzzer Time (When On)
	1,		// 202 - decimal - Brake Pick Voltage Rate Time
	1,		// 203 - decimal - Brake Relevel Voltage Rate Time
	0,		// 204 - decimal - Brake Pick Start Voltage
	0,		// 205 - decimal - Brake Relevel Start Voltage
	1,		// 206 - decimal - Short Floor Down Slowdown Time
	1,		// 207 - decimal - Mid Short Floor Down Slowdown Time
	1,		// 208 - decimal - Short Floor Up Slowdown Time
	1,		// 209 - decimal - Mid Short Floor Up Slowdown Time
	0,		// 210 - decimal - Hall Switch Elevator Off Return Floor
	0,		// 211 - decimal - Service Message 1 display
	0,		// 212 - decimal - Service Message 2 display
	0,		// 213 - decimal - Governor Gripper Trip Control
	0,		// 214 - decimal - Short Floor Soft Start Rate
	0,		// 215 - decimal - Short Floor Acceleration Rate
	0,		// 216 - decimal - Short Floor Roll Over Jerk Rate
	0,		// 217 - decimal - Short Floor Deceleration Jerk Rate
	0,		// 218 - decimal - Short Floor Deceleration Rate
	1,		// 219 - decimal - Short Floor Target Distance
	0,		// 220 - decimal - Short Floor Control
	0,		// 221 - decimal - Security Recall Control 2
	1,		// 222 - decimal - Emergency Power Recall Delay Time
	0,		// 223 - decimal - Group Car Call Security Override Time
	1,		// 224 - decimal - Emergency Brake Drop Delay
	1,		// 225 - decimal - Emergency Brake Pick Time
	1,		// 226 - decimal - Hall Call Output On Brightness for led
	1,		// 227 - decimal - Hall Call Output Off Brightness for led
	0,		// 228 - decimal - Access Top Floor
	0,		// 229 - decimal - Access Bottom Floor
	0,		// 230 - decimal - Stop On Position Pulse
	1,		// 231 - decimal - Sabbath Svc Restart Time
	1,		// 232 - decimal - Power Door Open Relay On Time
	0,		// 233 - decimal - Display Fault Automatically on LCD when Fault Occurs	
	0,		// 234 - decimal - Password for Adjusting Field Variables
	1,		// 235 - decimal - Password Time-out Time
	0,		// 236 - decimal - Behind Car Call Cancel
	1,		// 237 - decimal - Door Open Advance Door Enable Non-Simultaneous Doors
	0,		// 238 - decimal - Group Service Number of Parking Cars
	0,		// 239 - decimal - Binary Preset Update when Position Does Not Match
	0,		// 240 - decimal - Short Floor hsf with No hsv High Speed Valve
	0,		// 241 - decimal - Up Peak Control
	0,		// 242 - decimal - Up Peak Pool
	0,		// 243 - decimal - Down Peak Control
	0,		// 244 - decimal - Down Peak Pool
	0,		// 245 - decimal - Cross Assignment Cars from Old Group
	0,		// 246 - decimal - Modbus Protocol
	0,		// 247 - decimal - Fire Alternate Recall After FS Turns Off
	0,		// 248 - decimal - Inspection Deceleration Stop Rate
	0,		// 249 - decimal - Emergency Power Recovery Direction
	0,		// 250 - decimal - Brake Drop Voltage 
	1,		// 251 - decimal - Brake Voltage Drop Rate
	0,		// 252 - decimal - Brake Drop Timing During Leveling Flag
	0,		// 253 - decimal - Recovery Velocity
	0,		// 254 - decimal - Emergency Deceleration to Leveling Rate
	0,		// 255 - decimal - Emergency Power Recovery Velocity
	0,		// 256 - decimal - Run Cycle Time
	1,		// 257 - decimal - Hall Call Up On Red Intensity
	1,		// 258 - decimal - Hall Call Up Off Red Intensity
	1,		// 259 - decimal - Hall Call Up On Green Intensity
	1,		// 260 - decimal - Hall Call Up Off Green Intensity
	1,		// 261 - decimal - Hall Call Up On Blue Intensity
	1,		// 262 - decimal - Hall Call Up Off Blue Intensity
	0,		// 263 - decimal - Door Open Light Control on CCS board
	0,		// 264 - decimal - Hall Call Button Up On Color
	0,		// 265 - decimal - Hall Call Button Up Off Color
	0,		// 266 - decimal - Hall Call Button Security Color
	0,		// 267 - decimal - Hall Call Button Security Light
	1,		// 268 - decimal - Hall Call Security Brightness
	1,		// 269 - decimal - Hall Call Security Red Intensity
	1,		// 270 - decimal - Hall Call Security Green Intensity
	1,		// 271 - decimal - Hall Call Security Blue Intensity
	0,		// 272 - decimal - Fire Service Hoistway Sensor 2 Return Floor
	3,		// 273 - decimal - Drive Speed Multiplier
	0,		// 274 - decimal - Advanced Pre-torque
	0,		// 275 - decimal - Car Call Button On Color
	0,		// 276 - decimal - Car Call Button Off Color
	1,		// 277 - decimal - Car Call Output On Brightness for led
	1,		// 278 - decimal - Car Call Output Off Brightness for led
	1,		// 279 - decimal - Car Call On Red Intensity
	1,		// 280 - decimal - Car Call Off Red Intensity
	1,		// 281 - decimal - Car Call On Green Intensity
	1,		// 282 - decimal - Car Call Off Green Intensity
	1,		// 283 - decimal - Car Call On Blue Intensity
	1,		// 284 - decimal - Car Call Off Blue Intensity
	0,		// 285 - decimal - Car Call Button Light Control
	0,		// 286 - decimal - Car Call Button Security Color
	1,		// 287 - decimal - Car Call Security Brightness
	1,		// 288 - decimal - Car Call Security Red Intensity
	1,		// 289 - decimal - Car Call Security Green Intensity
	1,		// 290 - decimal - Car Call Security Blue Intensity
	0,		// 291 - decimal - Sabbath Enable Control Variable
	1,		// 292 - decimal - Low Line Voltage (RMS value)
	1,		// 293 - decimal - Low Door Voltage (RMS value)
	0,		// 294 - decimal - Emergency Power Top Speed
	0,		// 295 - decimal - Emergency Power Soft Start Jerk Rate
	0,		// 296 - decimal - Emergency Power Acceleration Rate
	0,		// 297 - decimal - Emergency Power Roll Over Jerk Rate
	0,		// 298 - decimal - Emergency Power Deceleration Jerk Rate
	0,		// 299 - decimal - Emergency Power Deceleration Rate
	1,		// 300 - decimal - Emergency Power Target Distance
	0,		// 301 - decimal - Car Top Speed
	0,		// 302 - decimal - Inspection Velocity from ISER output
	1,		// 303 - decimal - Emergency Power Recover to Floor Time
	0,		// 304 - decimal - Fire Light Control during EP
	0,		// 305 - decimal - Emergency Power Manual Select Enable
	0,		// 306 - decimal - Fire Light Control with Car Out of Service (Low Pressure, eaq, Stall)
	1,		// 307 - decimal - Sabbath Door Dwell Time
	1,		// 308 - decimal - Sabbath Door Buzzer Time Prior to Doors Closing
	1,		// 309 - decimal - Low Battery Capacity Level
	0,		// 310 - decimal - Sabbath Disable Control Variable
	0,		// 311 - decimal - Electric Eye Cancel Door Time
	0,		// 312 - decimal - Emergency Power Car at Recall Floor with Door Open
	0,		// 313 - decimal - Emergency Power Car Not at Recall Floor with Door Open
	0,		// 314 - decimal - Assign Parking Floor to Car with Door Open
	1,		// 315 - decimal - Hall Lantern Delay
	0,		// 316 - decimal - Sabbath Enable Control 2 Variable
	0,		// 317 - decimal - Cab Lantern Control
	1,		// 318 - decimal - Max Door Hold Time from ED Input
	0,		// 319 - decimal - Access Run Door Close
	0,		// 320 - decimal - Car Call Button Attendant Up Color
	1,		// 321 - decimal - Car Call Attendant Up Brightness
	1,		// 322 - decimal - Car Call Attendant Up Red Intensity
	1,		// 323 - decimal - Car Call Attendant Up Green Intensity
	1,		// 324 - decimal - Car Call Attendant Up Blue Intensity
	0,		// 325 - decimal - Car Call Button Attendant Dn Color
	1,		// 326 - decimal - Car Call Attendant Dn Brightness
	1,		// 327 - decimal - Car Call Attendant Dn Red Intensity
	1,		// 328 - decimal - Car Call Attendant Dn Green Intensity
	1,		// 329 - decimal - Car Call Attendant Dn Blue Intensity
	0,		// 330 - decimal - Fire light Color
	1,		// 331 - decimal - Fire light Brightness
	1,		// 332 - decimal - Fire light Red Intensity
	1,		// 333 - decimal - Fire light Green Intensity
	1,		// 334 - decimal - Fire light Blue Intensity
	0,		// 335 - decimal - Medical light Color
	1,		// 336 - decimal - Medical light Brightness
	1,		// 337 - decimal - Medical light Red Intensity
	1,		// 338 - decimal - Medical light Green Intensity
	1,		// 339 - decimal - Medical light Blue Intensity
	0,		// 340 - decimal - Emergency light Color
	1,		// 341 - decimal - Emergency light Brightness
	1,		// 342 - decimal - Emergency light Red Intensity
	1,		// 343 - decimal - Emergency light Green Intensity
	1,		// 344 - decimal - Emergency light Blue Intensity
	0,		// 345 - decimal - OTS light Color
	1,		// 346 - decimal - OTS light Brightness
	1,		// 347 - decimal - OTS light Red Intensity
	1,		// 348 - decimal - OTS light Green Intensity
	1,		// 349 - decimal - OTS light Blue Intensity
	0,		// 350 - decimal - Backlight Output lights
	0,		// 351 - decimal - Can Sync Count
	0,		// 352 - decimal - Hall Call Dn On Color
	1,		// 353 - decimal - Hall Call Dn On Brightness
	1,		// 354 - decimal - Hall Call Dn On Red Intensity
	1,		// 355 - decimal - Hall Call Dn On Green Intensity
	1,		// 356 - decimal - Hall Call Dn On Blue Intensity
	0,		// 357 - decimal - Hall Call Dn Off Color
	1,		// 358 - decimal - Hall Call Dn Off Brightness
	1,		// 359 - decimal - Hall Call Dn Off Red Intensity
	1,		// 360 - decimal - Hall Call Dn Off Green Intensity
	1,		// 361 - decimal - Hall Call Dn Off Blue Intensity
	0,		// 362 - decimal - Hall Call CB On Color
	1,		// 363 - decimal - Hall Call CB On Brightness
	1,		// 364 - decimal - Hall Call CB On Red Intensity
	1,		// 365 - decimal - Hall Call CB On Green Intensity
	1,		// 366 - decimal - Hall Call CB On Blue Intensity
	0,		// 367 - decimal - Hall Call VIP On Color
	1,		// 368 - decimal - Hall Call VIP On Brightness
	1,		// 369 - decimal - Hall Call VIP On Red Intensity
	1,		// 370 - decimal - Hall Call VIP On Green Intensity
	1,		// 371 - decimal - Hall Call VIP On Blue Intensity
	0,		// 372 - decimal - Hall Call IR Up On Color
	1,		// 373 - decimal - Hall Call IR Up On Brightness
	1,		// 374 - decimal - Hall Call IR Up On Red Intensity
	1,		// 375 - decimal - Hall Call IR Up On Green Intensity
	1,		// 376 - decimal - Hall Call IR Up On Blue Intensity
	0,		// 377 - decimal - Hall Call IR Dn On Color
	1,		// 378 - decimal - Hall Call IR Dn On Brightness
	1,		// 379 - decimal - Hall Call IR Dn On Red Intensity
	1,		// 380 - decimal - Hall Call IR Dn On Green Intensity
	1,		// 381 - decimal - Hall Call IR Dn On Blue Intensity
	0,		// 382 - decimal - Hall Call IR Up Off Color
	1,		// 383 - decimal - Hall Call IR Up Off Brightness
	1,		// 384 - decimal - Hall Call IR Up Off Red Intensity
	1,		// 385 - decimal - Hall Call IR Up Off Green Intensity
	1,		// 386 - decimal - Hall Call IR Up Off Blue Intensity
	0,		// 387 - decimal - Hall Call IR Dn Off Color
	1,		// 388 - decimal - Hall Call IR Dn Off Brightness
	1,		// 389 - decimal - Hall Call IR Dn Off Red Intensity
	1,		// 390 - decimal - Hall Call IR Dn Off Green Intensity
	1,		// 391 - decimal - Hall Call IR Dn Off Blue Intensity
	1,		// 392 - decimal - Hall Call Output Up On brightness for led
	1,		// 393 - decimal - Hall Call Output Up Off brightness for led
	0,		// 394 - decimal - Code Blue Location
	0,		// 395 - decimal - VIP Location
	0,		// 396 - decimal - IR light Color Control
	0,		// 397 - decimal - Hall Call CB Off Color
	1,		// 398 - decimal - Hall Call CB Off Brightness
	1,		// 399 - decimal - Hall Call CB Off Red Intensity
	1,		// 400 - decimal - Hall Call CB Off Green Intensity
	1,		// 401 - decimal - Hall Call CB Off Blue Intensity
	0,		// 402 - decimal - Hall Call VIP Off Color
	1,		// 403 - decimal - Hall Call VIP Off Brightness
	1,		// 404 - decimal - Hall Call VIP Off Red Intensity
	1,		// 405 - decimal - Hall Call VIP Off Green Intensity
	1,		// 406 - decimal - Hall Call VIP Off Blue Intensity
	0,		// 407 - decimal - VIP Lantern Control
	0,		// 408 - decimal - Group Car Call Override Control
	0,		// 409 - decimal - Exclusion Fault 1
	0,		// 410 - decimal - Exclusion Fault 2         
	0,		// 411 - decimal - Exclusion Fault 3
	0,		// 412 - decimal - Exclusion Fault 4        
	0,		// 413 - decimal - Exclusion Fault 5
	0,		// 414 - decimal - Exclusion Fault 6        
	0,		// 415 - decimal - Emergency Brake Pick Voltage
	0,		// 416 - decimal - Emergency Brake Hold Voltage
	1,		// 417 - decimal - Emergency Brake Pick Rate
	1,		// 418 - decimal - Emergency Brake Drop Rate
	0,		// 419 - decimal - Emergency Brake AC Line to Line Voltage
	0,		// 420 - decimal - Brake Opto Trigger Delay
	0,		// 421 - decimal - Can Baud Rate
	0,		// 422 - decimal - Car Call Arrival Lantern
	0,		// 423 - decimal - Code Blue Second Riser Car Select
	0,		// 424 - decimal - Code Blue over IR car
	0,		// 425 - decimal - IR Car Code Blue Penalty Time
	0,		// 426 - decimal - INSEC (In Security Output Invert)
	0,		// 427 - decimal - Attendant Manual Direction Enable
	0,		// 428 - decimal - Encoder Debounce Interval
	0,		// 429 - decimal - Encoder Number of Samples
	1,		// 430 - decimal - Handicap Buzzer on Car Call Beep Time
	0,		// 431 - decimal - Service UPS Mode
	0,		// 432 - decimal - Serial Port Comm Diag
	0,		// 433 - decimal - User Port 2 Baud Rate
	0,		// 434 - decimal - Cpu Timing Output Control
	0,		// 435 - decimal - Service Message 3 Display
	0,		// 436 - decimal - Hall Call Assignment Secure Type
	0,		// 437 - decimal - UPS Baud Rate
	0,		// 438 - decimal - HC Acknowledge Attendant Buzzer
	0,		// 439 - decimal - Second Code Blue Car Select
	0,		// 440 - decimal - Hall Elevator Off Options 2 
	0,		// 441 - decimal - Code Blue Buzzer Control
	0,		// 442 - decimal - Code Blue Override Independent
	0,		// 443 - decimal - Code Blue Recall Any Car If CB Cars 1 And 2 Are Not Available
	0,		// 444 - decimal - Hall Elevator Off Override
	0,		// 445 - decimal - NYC FS2 Door Reopen on STP
	1,		// 446 - decimal - Power Loss Brake On Time
	1,		// 447 - decimal - Power Loss Brake Off Time
	0,		// 448 - decimal - Select and Prioritize the ATT car to Run on Emergency Power Service
	0,		// 449 - decimal - Encoder Direction for Open Can Encoder
	0,		// 450 - decimal - Emergency Cars Finished Recalling Output enable (EPCOM)
	0,		// 451 - decimal - Out of Service Light control
	0,		// 452 - decimal - Extra Door Time Location 
	0,		// 453 - decimal - Out of Service Light Control
	1,		// 454 - decimal - Time for OSERL Not Answering Call
	1,		// 455 - decimal - Time for OSERL car Between Floors
	1,		// 456 - decimal - Time for OSERL Safety String Open
	0,		// 457 - decimal - Fire Service Hoistway Sensor Location
	1,		// 458 - decimal - Retiring Cam Drop Fail Safe Delay
	0,		// 459 - decimal - Velocity Difference 
	0,		// 460 - decimal - Velocity Difference Fault Disable
	0,		// 461 - decimal - SPB Direction Fault Disable
	0,		// 462 - decimal - Limit Direction Fault Disable
	0,		// 463 - decimal - Encoder Direction Fault Disable
	0,		// 464 - decimal - UL/DL Direction Fault Disable
	1,		// 465 - decimal - Relevel Start Velocity Hold Time
	0,		// 466 - decimal - Med Door Open Sequence for Re-open
	0,		// 467 - decimal - Med Ind Override
	0,		// 468 - decimal - Code Blue Bypass Disable
	0,		// 469 - decimal - Extended Door PI Message
	0,		// 470 - decimal - First Return Car on Emergency Power for Feeder 2
	0,		// 471 - decimal - First Car Selected on Emergency Power for Feeder 2
	0,		// 472 - decimal - Retiring Cam for Freight Output Enable
	1,		// 473 - decimal - Lantern Preference Change Delay
	0,		// 474 - decimal - Slip Velocity Difference Fault Trip Value
	0,		// 475 - decimal - DOB Overrides DCB on Phase 2
	0,		// 476 - decimal - Denver Fire Service Amendment to Close the Phase 1 Doors After Recall and Reopen From a Hall Call
	1,		// 477 - decimal - Denver FS Phase 1 Dwell Time
	0,		// 478 - decimal - Hall Call Security Control
	0,		// 479 - decimal - Slip Velocity Difference Disable
	0,		// 480 - decimal - Leveling Fault Trip
	0,		// 481 - decimal - Emergency Brake Switch Control
	0,		// 482 - decimal - Attendant Buzzer Control
	0,		// 483 - decimal - Radial Position Indicator
	0,		// 484 - decimal - Force Car to Recall On Independent Service to the Lobby When No Demand Is Present
	1,		// 485 - decimal - Time-out to Recall On Independent Service to the Lobby When No Demand Is Present
	1,  	// 486 - decimal - Motor RPM
	0,		// 487 - decimal - VIP Multiple Calls Enable
	0,		// 488 - decimal - Service Output Control
	0,		// 489 - decimal - Can Encoder Baud Rate
	0,		// 490 - decimal - Can Encoder Node ID
	0,		// 491 - decimal - Code Blue Allows Single Free Car Call Enable
	0,		// 492 - decimal - Sabbath Collective Mode
	0,		// 493 - decimal - Return to Lobby Door Open Type
	0,		// 494 - decimal - Ring the Lantern in IR All the Time
	0,		// 495 - decimal - High Priority Floor Number
	1,		// 496 - decimal - High Priority Floor Time
	0,		// 497 - decimal - Emergency Manual Select Switch Recall Operation
	0,		// 498 - decimal - Access Speed
	0,		// 499 - decimal - Auto Service Time-out Enable
	0,		// 500 - decimal - Auto Service Time-out Time
	0,		// 501 - decimal - Car Call Lockouts on Sabbath Operation
	0,		// 502 - decimal - Medical Service Override Car Call Security
	1,		// 503 - decimal - HEOF Auto Reset Time
	0,		// 504 - decimal - CCPBS on Group Car Call Security
	0,		// 505 - decimal - Dead Zone Selection
	1,		// 506 - decimal - Access UT Distance
	1,		// 507 - decimal - Access DT Distance
	0,		// 508 - decimal - Serial Port Comm Diag 2
	0,		// 509 - decimal - Third Riser Control
	0,		// 510 - decimal - Baud Rate for 485 Port
	0,		// 511 - decimal - Encoder Type
	0,		// 512 - decimal - Keb Drive Display Type
	0,		// 513 - decimal - Brake Phase Input
	0,		// 514 - decimal - Emergency Brake Phase Input
	3,		// 515 - decimal - Distance Feed Forward
	0,		// 516 - decimal - Second Riser Car Calls Security by Direction
	0,		// 517 - decimal - Minimum Short Door Time Enable
	0,		// 518 - decimal - Fire Recall Reset 2
	0,		// 519 - decimal - Invert Hall Call Security Input
	0,		// 520 - decimal - DZ Gripper Trip Control
	1,		// 521 - decimal - Security Disable Time
	0,		// 522 - decimal - DL Dead Zone Selection
	1,		// 523 - decimal - EE Test Time
	0,		// 524 - decimal - Reassign Car Calls When on Multiple Floor Mask Security (cons[SecFlCfg] = 2)
	0,		// 525 - decimal - Disable Opposite Hall Call
	0,		// 526 - decimal - Disable Opposite Hall Call Time
	0,		// 527 - decimal - Hall Elevator Off Options 3
	0,		// 528 - decimal - Car Elevator Off Options
	0,		// 529 - decimal - Car Elevator Off Options 2
	0,		// 530 - decimal - Code Blue Over Fire Service
	0,		// 531 - decimal - COP/Remote Disable
	0,		// 532 - decimal - Fire Service 2 with Special Devices (HYDRO ONLY)
	1,		// 533 - decimal - Auto Swing Door Open Delay
	0,		// 534 - decimal - CL Pulse Time
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 539
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 544
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 549
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 554
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 559
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 564
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 569
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 574
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 579
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 584
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 589
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 594
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 599
};
int16 fvars_max[nmbfvar] ={	   // Note: -1 == cons[nmbcars] ,  -2 == cons[topf], -3 = cons[speed]
	50,		// 000 - Max =         5.0		Y Delta Time
	100,	// 001 - Max =        10.0		Fault Time
	100,	// 002 - Max =        10.0		Reset Time
	1,		// 003 - Max =           1		Double Stroke on Lanterns
	20,		// 004 - Max =         2.0		Lantern On Time for Double Stroking
	20,		// 005 - Max =         2.0		Lantern Off Time for Double Stroking
	20,		// 006 - Max =         2.0		Handicap Floor Passing Beep Time
	32000,	// 007 - Max =      3200.0		Door Protection Time
	32000,	// 008 - Max =      3200.0		Nudge Time
	32000,	// 009 - Max =      3200.0		Pre-opening Delay Time
	600,	// 010 - Max =        60.0		Car Call Door Dwell Time
	600,	// 011 - Max =        60.0		Hall Call Door Dwell Time
	600,	// 012 - Max =        60.0		Lobby Call Door Dwell Time
	1200,	// 013 - Max =       120.0		Extra Door Dwell Time
	600,	// 014 - Max =        60.0		Non Interference Time
	-2,		// 015 - Max =   Top Floor		Lobby Floor
	-2,		// 016 - Max =   Top Floor		Fire Service Phase 1 Main Egress Floor
	-2,		// 017 - Max =   Top Floor		Fire Service Phase 1 Alternate Egress Floor
	32000,	// 018 - Max =      3200.0		Stall Time
	300,	// 019 - Max =         3.0		Soft Stop Time
	32000,	// 020 - Max =      3200.0		Generator Run Time
	50,		// 021 - Max =         5.0		Brake Pick Delay Time
	50,		// 022 - Max =         5.0		Brake Drop Delay Time
	480,	// 023 - Max =         480		Soft Start Jerk Rate
	300,	// 024 - Max =         300		Acceleration Rate
	480,	// 025 - Max =         480		Deceleration Jerk Rate
	300,	// 026 - Max =         300		Deceleration Rate
	250,	// 027 - Max =        25.0		Floor Target Distance
	150,	// 028 - Max =        15.0		Leveling Speed
	150,	// 029 - Max =        15.0		Relevel Speed
	150,	// 030 - Max =         150		Inspection Speed
	2999,	// 031 - Max =        2999		Year
	12,		// 032 - Max =          12		Month
	31,		// 033 - Max =          31		Day
	23,		// 034 - Max =          23		Hour
	59,		// 035 - Max =          59		Minute
	480,	// 036 - Max =         480		Roll Over Jerk Rate
	1,		// 037 - Max =           1		EE Memory Size
	2999,	// 038 - Max =        2999		Annual Safety Test Year
	12,		// 039 - Max =          12		Annual Safety Test Month
	31,		// 040 - Max =          31		Annual Safety Test Day
	300,	// 041 - Max =        3.00		Pattern Delay
	110,  	// 042 - Max =         110		Field Weaken Velocity Percent
    32000,  // 043 - Max =       32000		Encoder PPR
	30000,  // 044 - Max =        3000		Encoder RPM
	9,		// 045 - Max =           9		User Interface Baud Rate 
	-1,		// 046 - Max =           8		Parking
	-2,		// 047 - Max =   Top Floor		Emergency Power Return Floor
	-1,		// 048 - Max =           8		Number of Cars to Run on Emergency Power
	-1,		// 049 - Max =           8		First Car to Return on Emergency Power
	-1,		// 050 - Max =           8		First Car Selected on Emergency Power
	6000,	// 051 - Max =       600.0		Return Car Time-out Time
	1,		// 052 - Max =           1		DOB Opens Door On Nudging Is Set
	5,		// 053 - Max =           5		Emergency Dispatch Enable
	-1,		// 054 - Max =           8		Number of Cars Requested at Lobby
	7,		// 055 - Max =           7		Next Up Selection
	-1,		// 056 - Max =           8		Code Blue Car Select
	-1,		// 057 - Max =           8		IR Car Select
	2,		// 058 - Max =           2		Brake Lift Switch
	50,		// 059 - Max =          50		Load Weighing Anti-nuisance Count
	4,		// 060 - Max =           4		Fire Switch Location
	60,		// 061 - Max =          60		ETA Minimum Time
	60,		// 062 - Max =          60		ETA for Coincident Car Call
	-1,		// 063 - Max =           8		Video Position for Car 1
	-1,		// 064 - Max =           8		Video Position for Car 2
	-1,		// 065 - Max =           8		Video Position for Car 3
	-1,		// 066 - Max =           8		Video Position for Car 4
	-1,		// 067 - Max =           8		Video Position for Car 5
	-1,		// 068 - Max =           8		Video Position for Car 6
	10,		// 069 - Max =          10		Run Count with No Passengers to Cancel Car Calls
	7,		// 070 - Max =           7		Independent Override Security
	50,		// 071 - Max =         5.0		Brake Pick Delay Time on Relevel
	1,		// 072 - Max =           1		Brake Cool on Relevel
	32000,	// 073 - Max =      3200.0		Up peak Trigger Time
	100,	// 074 - Max =         100		Up Peak Trigger Count
	40,		// 075 - Max =          40		Up Peak Car Call Count
	32000,	// 076 - Max =      3200.0		Up Peak Duration Time
	32000,	// 077 - Max =      3200.0		Down Peak Trigger Time
	100,	// 078 - Max =         100		Down Peak Trigger Count
	32000,	// 079 - Max =      3200.0		Down Peak Duration Time
	1200,	// 080 - Max =       120.0		Parking Delay Time
	5,		// 081 - Max =           5		Hall Call Cross Assignment
	500,	// 082 - Max =         500		Hall Call Cross Assignment ETA
	-2,		// 083 - Max =   Top Floor		Medical Emergency Floor
	1,		// 084 - Max =           1		Medical Emergency Hall Switch location
	1,		// 085 - Max =           1		Fire Service Motor Room Sensor Return Floor
	1,		// 086 - Max =           1		Fire Service Hoistway Sensor Return Floor
	2,		// 087 - Max =           3		Phase 1 Fire Recall Reset
	225,	// 088 - Max =         225		Acceleration and Deceleration Limit at Final Stop
	300,	// 089 - Max =        3.00		Pattern Delay During Relevel
	1,		// 090 - Max =           1		Delay Start Until Drive On
	7372, 	// 091 - Max =        7372		Brake Opto Trigger Delay
	1,		// 092 - Max =           1		Invert Logic of Brake Lift Switch
	5,		// 093 - Max =           5		Invert Logic of In-service Light Output or In-use Light or Out of Service Alarm
	-1,		// 094 - Max =           8		Ability to Change a Car Other Than 1 or 2 to Become the Master
	32000,	// 095 - Max =      3200.0		Time-out Video Display
	1,		// 096 - Max =           1		Close Doors While Trying to Run on Inspection
	-2,		// 097 - Max =   Top Floor		Parking Floor 1
	-2,		// 098 - Max =   Top Floor		Parking Floor 2
	-2,		// 099 - Max =   Top Floor		Parking Floor 3
	-2,		// 100 - Max =   Top Floor		Parking Floor 4
	-2,		// 101 - Max =   Top Floor		Parking Floor 5
	-2,		// 102 - Max =   Top Floor		Parking Width
	1000,  	// 103 - Max =       100.0		Percent of Load to Balance the Counterweights (Balanced Load Percent)
	10000,	// 104 - Max =      100.00		Drive Starting Torque Multiplier (Torque Amount Variable)
	100,	// 105 - Max =         100		Load Bypass Percent
	100,	// 106 - Max =         100		Load Anti-nuisance Percent
	100,	// 107 - Max =         100		Load Dispatch Percent
	125,	// 108 - Max =         125		Load Overload Percent
	7,		// 109 - Max =         0xf		IR Control Flag
	400,	// 110 - Max =         400		Brake Pick Voltage
	60,		// 111 - Max =         6.0		Brake Pick Duration Time
	400,	// 112 - Max =         400		Brake Hold Voltage
	400,	// 113 - Max =         400		Brake Relevel Voltage
	300,	// 114 - Max =         300		Brake AC Line to Line Voltage
	5000, 	// 115 - Max =       500.0		Brake Resistance
	9000,	// 116 - Max =       900.0		Attendant Buzzer Delay Time
	1,		// 117 - Max =           1		Hall Fire Aux. Switch Enable
	4,		// 118 - Max =           4		Operation of Hall Fire Light
	7,		// 119 - Max =           7		COP/Remote CC Select
	15,		// 120 - Max =         1.5		Door Open or Close Delay Time
	15,		// 121 - Max =          15		Security Recall Option
	5,		// 122 - Max =           5		DOB Overrides Security
	9000, 	// 123 - Max =       900.0		Manual Door Buzzer Delay Time
	-2,		// 124 - Max =   Top Floor		Security Floor
	70,		// 125 - Max =         7.0		Retiring Cam Pick Delay Time
	600,	// 126 - Max =        60.0		Short Door Time From a Reopen Signal
	-1,		// 127 - Max =           8		Second IR Car Select
	15,		// 128 - Max =          15		Stop At Lobby
	1,		// 129 - Max =           1		Invert Cab Light Fan Output
	1,		// 130 - Max =           1		Invert Temperature Low Switch (TPL)
	1,		// 131 - Max =           1		Invert Low Pressure Switch
	1,		// 132 - Max =           1		Invert Low Oil Switch
	1,		// 133 - Max =           1		Nudging with No Onward Call
	3,		// 134 - Max =           3		Fire Option Flag
	1,		// 135 - Max =           1		Attendant Car Call Flag
	32000,	// 136 - Max =      3200.0		Code Blue Door Open Time
	1,		// 137 - Max =           1		Request Ind Car For Code Blue
	1,		// 138 - Max =           1		Code Blue Light Control (Flash Light)
	1,		// 139 - Max =           1		Hospital Service Door Close on Car Call
	200,	// 140 - Max =        20.0		Gripper Reset Time(r)
	1,		// 141 - Max =           1		Independent Door Close on Car Call
	5000,	// 142 - Max =       50.00		Torque Offset
	3,		// 143 - Max =           3		Gripper Test From Safety Processor Board
	7,		// 144 - Max =           7		Single Automatic Pushbutton Control
	1,		// 145 - Max =           1		Door Open When DOL Signal Not Active
	0,		// 146 - Max =           0		Previously: fvdisglt - Disable Gate and Lock Test for New York City
	150,	// 147 - Max =        15.0		Relevel Start Speed
	360,	// 148 - Max =         360		Emergency Deceleration Rate
	1,		// 149 - Max =           1		Disable DCB from Shortening Door Time
	7,		// 150 - Max =           7		Return to Lobby Option
	-1,		// 151 - Max =           8		Video Position for Car 7
	-1,		// 152 - Max =           8		Video Position for Car 8
	3,		// 153 - Max =           3		Parking Type
	-2,		// 154 - Max =   Top Floor		Parking Floor 6
	-2,		// 155 - Max =   Top Floor		Parking Floor 7
	2,		// 156 - Max =           2		Non-Simultaneous Doors Operation
	3,		// 157 - Max =           3		Pre-open Doors
	125,	// 158 - Max =         125		Over Speed Percent
	7,		// 159 - Max =           7		Position Pulse Count Update
	10000,	// 160 - Max =       10000		Position Pulse Delay Error
	1,		// 161 - Max =           1		Gripper Buzzer Enable
	20,		// 162 - Max =         2.0		Relevel Delay Time
	3,		// 163 - Max =           3		Fire Option 2
	1,		// 164 - Max =           1		Close Gate on Swing Door with No Preference
	10,		// 165 - Max =          10		Leveling Fault Count
	10000,	// 166 - Max =      100.00		Torque Amount in Down Direction
	100,	// 167 - Max =        1.00		Ramp Torque Time During Pattern Delay
	32000,	// 168 - Max =      3200.0		Electric Eye Time-out Time
	1,		// 169 - Max =           1		Cars Out of Service Hall Call Cancel
	7,		// 170 - Max =           7		Nudging Control
	600,	// 171 - Max =        60.0		Fire Phase 1 Door Time-out (Ind and Att)
	3,		// 172 - Max =           3		No Hall Call Button Door Reopen with Onward Call
	10,		// 173 - Max =          10		Drive Ready Reset Count
	600,	// 174 - Max =        60.0		Return to Lobby Door Dwell Time
	3,		// 175 - Max =           3		Next Up Lobby Lantern Control
	1,		// 176 - Max =           1		Next Up Preference Control
	3,		// 177 - Max =           3		Second Riser Control
	1,		// 178 - Max =           1		Second Riser Lantern Control
	60,		// 179 - Max =          60		Attendant ETA Preference Time
	1,		// 180 - Max =           1		Handicap Buzzer Disabled at Invalid Floor	 
	32000,	// 181 - Max =      3200.0		VIP Door Open Time
	3,		// 182 - Max =           3		VIP Operation Flags
	-1,		// 183 - Max =           8		Number of VIP cars
	3,		// 184 - Max =           3		Emergency Power Op led (OP) Function
	2,		// 185 - Max =           2		Emergency Power Park led (PRK) Function
	15,		// 186 - Max =          15		Hall Elevator Off Options
	3,		// 187 - Max =           3		Drive Baud Rate
	2,		// 188 - Max =           2		Drive Update Rate
	-2,		// 189 - Max =   Top Floor		Alternate Lobby Floor
	-2,		// 190 - Max =   Top Floor		Alternate Parking Floor
	1,		// 191 - Max =           1		Lobby Request Control
	100,	// 192 - Max =         100		Handicap Car Capacity
	32000,	// 193 - Max =      3200.0		Freight Door Close Time-out
	255,	// 194 - Max =         255		Handicap Car Wait Time
#if (Hollister) == 1
	1600,	// 195 - Max =       160.0		Short Floor Distance *Special for 1000 fpm Car
#else
	720,	// 195 - Max =        72.0		Short Floor Distance
#endif
	3,		// 196 - Max =           3		Retiring Cam Control
	1,		// 197 - Max =           1		Recall from Fire Alternate Floor
	32000,	// 198 - Max =      3200.0		Auto Learn Hoistway Stall Time
	200,	// 199 - Max =        2.00		Position Pulse Count Error
	300,	// 200 - Max =        30.0		Attendant Buzzer Time (When Off)
	300,	// 201 - Max =        30.0		Attendant Buzzer Time (When On)
	20,		// 202 - Max =         2.0		Brake Pick Voltage Rate Time
	20,		// 203 - Max =         2.0		Brake Relevel Voltage Rate Time
	400,	// 204 - Max =         400		Brake Pick Start Voltage
	400,	// 205 - Max =         400		Brake Relevel Start Voltage
	300,	// 206 - Max =        30.0		Short Floor Down Slowdown Time
	300,	// 207 - Max =        30.0		Mid Short Floor Down Slowdown Time
	300,	// 208 - Max =        30.0		Short Floor Up Slowdown Time
	300,	// 209 - Max =        30.0		Mid Short Floor Up Slowdown Time
	-2,		// 210 - Max =   Top Floor		Hall Switch Elevator Off Return Floor
	last_servf+1,		// 211 - Max =          30		Service Message 1 display
	last_servf+1,		// 212 - Max =          30		Service Message 2 display
	1,		// 213 - Max =           1		Governor Gripper Trip Control
	480,	// 214 - Max =         480		Short Floor Soft Start Rate
	300,	// 215 - Max =         300		Short Floor Acceleration Rate
	480,	// 216 - Max =         480		Short Floor Roll Over Jerk Rate
	480,	// 217 - Max =         480		Short Floor Deceleration Jerk Rate
	300,	// 218 - Max =         300		Short Floor Deceleration Rate
	250,	// 219 - Max =        25.0		Short Floor Target Distance
	7,		// 220 - Max =          31		Short Floor Control
	2,		// 221 - Max =           2		Security Recall Control 2
	32000,	// 222 - Max =      3200.0		Emergency Power Recall Delay Time
	240, 	// 223 - Max =         240		Group Car Call Security Override Time
	3600,	// 224 - Max =       360.0		Emergency Brake Drop Delay
	100,	// 225 - Max =        10.0		Emergency Brake Pick Time
	1000,	// 226 - Max =       100.0		Hall Call Output On Brightness for led
	1000,	// 227 - Max =       100.0		Hall Call Output Off Brightness for led
	-2,		// 228 - Max =   Top Floor		Access Top Floor
	-2,		// 229 - Max =   Top Floor		Access Bottom Floor
	1,		// 230 - Max =           1		Stop On Position Pulse
	32000,	// 231 - Max =      3200.0		Sabbath Svc Restart Time
	300,	// 232 - Max =        30.0		Power Door Open Relay On Time
	1,		// 233 - Max =           1		Display Fault Automatically on LCD when Fault Occurs	
	9999,	// 234 - Max =        9999		Password for Adjusting Field Variables
	32000,	// 235 - Max =      3200.0		Password Time-out Time
	1,		// 236 - Max =           1		Behind Car Call Cancel
	2400,	// 237 - Max =       240.0		Door Open Advance Door Enable Non-Simultaneous Doors
	-1,		// 238 - Max =           8		Group Service Number of Parking Cars
	1,		// 239 - Max =           1		Binary Preset Update when Position Does Not Match
	1,		// 240 - Max =           1		Short Floor hsf with No hsv High Speed Valve
	1,		// 241 - Max =           1		Up Peak Control
	-1,		// 242 - Max =           8		Up Peak Pool
	1,		// 243 - Max =           1		Down Peak Control
	-1,		// 244 - Max =           8		Down Peak Pool
	-1,		// 245 - Max =           8		Cross Assignment Cars from Old Group
	5,		// 246 - Max =           5		Modbus Protocol
	3,		// 247 - Max =           3		Fire Alternate Recall After FS Turns Off
	480,	// 248 - Max =         480		Inspection Deceleration Stop Rate
	1,		// 249 - Max =           1		Emergency Power Recovery Direction
	200,	// 250 - Max =         200		Brake Drop Voltage 
	50,		// 251 - Max =         5.0		Brake Voltage Drop Rate
	1,		// 252 - Max =           1		Brake Drop Timing During Leveling Flag
	100,	// 253 - Max =         100		Recovery Velocity
	300, 	// 254 - Max =         300		Emergency Deceleration to Leveling Rate
	100,	// 255 - Max =         100		Emergency Power Recovery Velocity
	300,	// 256 - Max =       300.0		Run Cycle Time
	1000,	// 257 - Max =       100.0		Hall Call Up On Red Intensity
	1000,	// 258 - Max =       100.0		Hall Call Up Off Red Intensity
	1000,	// 259 - Max =       100.0		Hall Call Up On Green Intensity
	1000,	// 260 - Max =       100.0		Hall Call Up Off Green Intensity
	1000,	// 261 - Max =       100.0		Hall Call Up On Blue Intensity
	1000,	// 262 - Max =       100.0		Hall Call Up Off Blue Intensity
	16,		// 263 - Max =          16		Door Open Light Control on CCS board
	15,		// 264 - Max =          15		Hall Call Button Up On Color
	15,		// 265 - Max =          15		Hall Call Button Up Off Color
	15,		// 266 - Max =          15		Hall Call Button Security Color
	7,		// 267 - Max =           7		Hall Call Button Security Light
	1000,  	// 268 - Max =       100.0		Hall Call Security Brightness
	1000,  	// 269 - Max =       100.0		Hall Call Security Red Intensity
	1000,  	// 270 - Max =       100.0		Hall Call Security Green Intensity
	1000,  	// 271 - Max =       100.0		Hall Call Security Blue Intensity
	1,		// 272 - Max =           1		Fire Service Hoistway Sensor 2 Return Floor
	2000,	// 273 - Max =       2.000		Drive Speed Multiplier
	1,		// 274 - Max =           1		Advanced Pre-torque
	15,		// 275 - Max =          15		Car Call Button On Color
	15,		// 276 - Max =          15		Car Call Button Off Color
	1000,	// 277 - Max =       100.0		Car Call Output On Brightness for led
	1000,	// 278 - Max =       100.0		Car Call Output Off Brightness for led
	1000,	// 279 - Max =       100.0		Car Call On Red Intensity
	1000,	// 280 - Max =       100.0		Car Call Off Red Intensity
	1000,	// 281 - Max =       100.0		Car Call On Green Intensity
	1000,	// 282 - Max =       100.0		Car Call Off Green Intensity
	1000,	// 283 - Max =       100.0		Car Call On Blue Intensity
	1000,	// 284 - Max =       100.0		Car Call Off Blue Intensity
	3,		// 285 - Max =           3		Car Call Button Light Control
	15,		// 286 - Max =          15		Car Call Button Security Color
	1000,	// 287 - Max =       100.0		Car Call Security Brightness
	1000,	// 288 - Max =       100.0		Car Call Security Red Intensity
	1000,	// 289 - Max =       100.0		Car Call Security Green Intensity
	1000,	// 290 - Max =       100.0		Car Call Security Blue Intensity
	7,		// 291 - Max =           7		Sabbath Enable Control Variable
	6000,	// 292 - Max =       600.0		Low Line Voltage (RMS value)
	6000,	// 293 - Max =       600.0		Low Door Voltage (RMS value)
	-3,		// 294 - Max =   Top Speed		Emergency Power Top Speed
	480,	// 295 - Max =         480		Emergency Power Soft Start Jerk Rate
	300,	// 296 - Max =         300		Emergency Power Acceleration Rate
	480,	// 297 - Max =         480		Emergency Power Roll Over Jerk Rate
	480,	// 298 - Max =         480		Emergency Power Deceleration Jerk Rate
	300,	// 299 - Max =         300		Emergency Power Deceleration Rate
	250,	// 300 - Max =        25.0		Emergency Power Target Distance
	-3,		// 301 - Max =   Top Speed		Car Top Speed
	1,		// 302 - Max =           1		Inspection Velocity from ISER output
	600,	// 303 - Max =        60.0		Emergency Power Recover to Floor Time
	1,		// 304 - Max =           1		Fire Light Control during EP
	3,		// 305 - Max =           3		Emergency Power Manual Select Enable
	1,		// 306 - Max =           1		Fire Light Control with Car Out of Service (Low Pressure, eaq, Stall)
	600,	// 307 - Max =        60.0		Sabbath Door Dwell Time
	100,	// 308 - Max =         100		Sabbath Door Buzzer Time Prior to Doors Closing
	1010,	// 309 - Max =       101.0		Low Battery Capacity Level
	7,		// 310 - Max =           7		Sabbath Disable Control Variable
	1,		// 311 - Max =           1		Electric Eye Cancel Door Time
	1,		// 312 - Max =           1		Emergency Power Car at Recall Floor with Door Open
	1,		// 313 - Max =           1		Emergency Power Car Not at Recall Floor with Door Open
	1,		// 314 - Max =           1		Assign Parking Floor to Car with Door Open
	150,	// 315 - Max =        15.0		Hall Lantern Delay
	7,		// 316 - Max =           7		Sabbath Enable Control 2 Variable
	2,		// 317 - Max =           2		Cab Lantern Control
	32000,	// 318 - Max =      3200.0		Max Door Hold Time from ED Input
	1,		// 319 - Max =           1		Access Run Door Close
	15,		// 320 - Max =          15		Car Call Button Attendant Up Color
	1000,	// 321 - Max =       100.0		Car Call Attendant Up Brightness
	1000,	// 322 - Max =       100.0		Car Call Attendant Up Red Intensity
	1000,	// 323 - Max =       100.0		Car Call Attendant Up Green Intensity
	1000,	// 324 - Max =       100.0		Car Call Attendant Up Blue Intensity
	15,		// 325 - Max =          15		Car Call Button Attendant Dn Color
	1000,	// 326 - Max =       100.0		Car Call Attendant Dn Brightness
	1000,	// 327 - Max =       100.0		Car Call Attendant Dn Red Intensity
	1000,	// 328 - Max =       100.0		Car Call Attendant Dn Green Intensity
	1000,	// 329 - Max =       100.0		Car Call Attendant Dn Blue Intensity
	15,		// 330 - Max =          15		Fire light Color
	1000,	// 331 - Max =       100.0		Fire light Brightness
	1000,	// 332 - Max =       100.0		Fire light Red Intensity
	1000,	// 333 - Max =       100.0		Fire light Green Intensity
	1000,	// 334 - Max =       100.0		Fire light Blue Intensity
	15,		// 335 - Max =          15		Medical light Color
	1000,	// 336 - Max =       100.0		Medical light Brightness
	1000,	// 337 - Max =       100.0		Medical light Red Intensity
	1000,	// 338 - Max =       100.0		Medical light Green Intensity
	1000,	// 339 - Max =       100.0		Medical light Blue Intensity
	15,		// 340 - Max =          15		Emergency light Color
	1000,	// 341 - Max =       100.0		Emergency light Brightness
	1000,	// 342 - Max =       100.0		Emergency light Red Intensity
	1000,	// 343 - Max =       100.0		Emergency light Green Intensity
	1000,	// 344 - Max =       100.0		Emergency light Blue Intensity
	15,		// 345 - Max =          15		OTS light Color
	1000,	// 346 - Max =       100.0		OTS light Brightness
	1000,	// 347 - Max =       100.0		OTS light Red Intensity
	1000,	// 348 - Max =       100.0		OTS light Green Intensity
	1000,	// 349 - Max =       100.0		OTS light Blue Intensity
	63,		// 350 - Max =          63		Backlight Output lights
	7,		// 351 - Max =           7		Can Sync Count
	15,		// 352 - Max =          15		Hall Call Dn On Color
	1000,	// 353 - Max =       100.0		Hall Call Dn On Brightness
	1000,	// 354 - Max =       100.0		Hall Call Dn On Red Intensity
	1000,	// 355 - Max =       100.0		Hall Call Dn On Green Intensity
	1000,	// 356 - Max =       100.0		Hall Call Dn On Blue Intensity
	15,		// 357 - Max =          15		Hall Call Dn Off Color
	1000,	// 358 - Max =       100.0		Hall Call Dn Off Brightness
	1000,	// 359 - Max =       100.0		Hall Call Dn Off Red Intensity
	1000,	// 360 - Max =       100.0		Hall Call Dn Off Green Intensity
	1000,	// 361 - Max =       100.0		Hall Call Dn Off Blue Intensity
	15,		// 362 - Max =          15		Hall Call CB On Color
	1000,	// 363 - Max =       100.0		Hall Call CB On Brightness
	1000,	// 364 - Max =       100.0		Hall Call CB On Red Intensity
	1000,	// 365 - Max =       100.0		Hall Call CB On Green Intensity
	1000,	// 366 - Max =       100.0		Hall Call CB On Blue Intensity
	15,		// 367 - Max =          15		Hall Call VIP On Color
	1000,	// 368 - Max =       100.0		Hall Call VIP On Brightness
	1000,	// 369 - Max =       100.0		Hall Call VIP On Red Intensity
	1000,	// 370 - Max =       100.0		Hall Call VIP On Green Intensity
	1000,	// 371 - Max =       100.0		Hall Call VIP On Blue Intensity
	15,		// 372 - Max =          15		Hall Call IR Up On Color
	1000,	// 373 - Max =       100.0		Hall Call IR Up On Brightness
	1000,	// 374 - Max =       100.0		Hall Call IR Up On Red Intensity
	1000,	// 375 - Max =       100.0		Hall Call IR Up On Green Intensity
	1000,	// 376 - Max =       100.0		Hall Call IR Up On Blue Intensity
	15,		// 377 - Max =          15		Hall Call IR Dn On Color
	1000,	// 378 - Max =       100.0		Hall Call IR Dn On Brightness
	1000,	// 379 - Max =       100.0		Hall Call IR Dn On Red Intensity
	1000,	// 380 - Max =       100.0		Hall Call IR Dn On Green Intensity
	1000,	// 381 - Max =       100.0		Hall Call IR Dn On Blue Intensity
	15,		// 382 - Max =          15		Hall Call IR Up Off Color
	1000,	// 383 - Max =       100.0		Hall Call IR Up Off Brightness
	1000,	// 384 - Max =       100.0		Hall Call IR Up Off Red Intensity
	1000,	// 385 - Max =       100.0		Hall Call IR Up Off Green Intensity
	1000,	// 386 - Max =       100.0		Hall Call IR Up Off Blue Intensity
	15,		// 387 - Max =          15		Hall Call IR Dn Off Color
	1000,	// 388 - Max =       100.0		Hall Call IR Dn Off Brightness
	1000,	// 389 - Max =       100.0		Hall Call IR Dn Off Red Intensity
	1000,	// 390 - Max =       100.0		Hall Call IR Dn Off Green Intensity
	1000,	// 391 - Max =       100.0		Hall Call IR Dn Off Blue Intensity
	1000,	// 392 - Max =       100.0		Hall Call Output Up On brightness for led
	1000,	// 393 - Max =       100.0		Hall Call Output Up Off brightness for led
	2,		// 394 - Max =           2		Code Blue Location
	2,		// 395 - Max =           2		VIP Location
	1,		// 396 - Max =           1		IR light Color Control
	15,		// 397 - Max =          15		Hall Call CB Off Color
	1000,	// 398 - Max =       100.0		Hall Call CB Off Brightness
	1000,	// 399 - Max =       100.0		Hall Call CB Off Red Intensity
	1000,	// 400 - Max =       100.0		Hall Call CB Off Green Intensity
	1000,	// 401 - Max =       100.0		Hall Call CB Off Blue Intensity
	15,		// 402 - Max =          15		Hall Call VIP Off Color
	1000,	// 403 - Max =       100.0		Hall Call VIP Off Brightness
	1000,	// 404 - Max =       100.0		Hall Call VIP Off Red Intensity
	1000,	// 405 - Max =       100.0		Hall Call VIP Off Green Intensity
	1000,	// 406 - Max =       100.0		Hall Call VIP Off Blue Intensity
	3,		// 407 - Max =           3		VIP Lantern Control
	1,		// 408 - Max =           1		Group Car Call Override Control
	max_nmbflts,	// 409 - Max = max_nmbflts		Exclusion Fault 1
	max_nmbflts,	// 410 - Max = max_nmbflts		Exclusion Fault 2         
	max_nmbflts,	// 411 - Max = max_nmbflts		Exclusion Fault 3
	max_nmbflts,	// 412 - Max = max_nmbflts		Exclusion Fault 4        
	max_nmbflts,	// 413 - Max = max_nmbflts		Exclusion Fault 5
	max_nmbflts,	// 414 - Max = max_nmbflts		Exclusion Fault 6        
	400,  	// 415 - Max =         400		Emergency Brake Pick Voltage
	400,  	// 416 - Max =         400		Emergency Brake Hold Voltage
	20,		// 417 - Max =         2.0		Emergency Brake Pick Rate
	15,		// 418 - Max =         1.5		Emergency Brake Drop Rate
	300,	// 419 - Max =         300		Emergency Brake AC Line to Line Voltage
	7372, 	// 420 - Max =        7372		Brake Opto Trigger Delay
	1,		// 421 - Max =           1		Can Baud Rate
	1,		// 422 - Max =           1		Car Call Arrival Lantern
	-1,		// 423 - Max =           8		Code Blue Second Riser Car Select
	1,		// 424 - Max =           1		Code Blue over IR car
	60,		// 425 - Max =          60		IR Car Code Blue Penalty Time
	1,		// 426 - Max =           1		INSEC (In Security Output Invert)
	4,		// 427 - Max =           4		Attendant Manual Direction Enable
	5,		// 428 - Max =           5		Encoder Debounce Interval
	10,		// 429 - Max =          10		Encoder Number of Samples
	20,		// 430 - Max =         2.0		Handicap Buzzer on Car Call Beep Time
	1,		// 431 - Max =           1		Service UPS Mode
	7,		// 432 - Max =          15		Serial Port Comm Diag
	9,		// 433 - Max =           3		User Port 2 Baud Rate
	0x7FFF,	// 434 - Max =      0x7FFF		Cpu Timing Output Control
	last_servf+1,		// 435 - Max =          30		Service Message 3 Display
	15,		// 436 - Max =          15		Hall Call Assignment Secure Type
	3,		// 437 - Max =           3		UPS Baud Rate
	1,		// 438 - Max =           1		HC Acknowledge Attendant Buzzer
	-1,		// 439 - Max =           8		Second Code Blue Car Select
   	15,		// 440 - Max =          15		Hall Elevator Off Options 2 
	1,		// 441 - Max =           1		Code Blue Buzzer Control
	1,		// 442 - Max =           1		Code Blue Override Independent
	1,		// 443 - Max =           1		Code Blue Recall Any Car If CB Cars 1 And 2 Are Not Available
	3,		// 444 - Max =           3		Hall Elevator Off Override
	1,		// 445 - Max =           1		NYC FS2 Door Reopen on STP
	50,		// 446 - Max =         5.0		Power Loss Brake On Time
	50,		// 447 - Max =         5.0		Power Loss Brake Off Time
	1,		// 448 - Max =           1		Select and Prioritize the ATT car to Run on Emergency Power Service
	1,		// 449 - Max =           1		Encoder Direction for Open Can Encoder
	1,		// 450 - Max =           1		Emergency Cars Finished Recalling Output enable (EPCOM)
	7,		// 451 - Max =           7		Out of Service Light control
	-2,		// 452 - Max =   Top Floor		Extra Door Time Location 
	1,		// 453 - Max =           1		Out of Service Light Control
	9000,	// 454 - Max =       900.0		Time for OSERL Not Answering Call
	9000,	// 455 - Max =       900.0		Time for OSERL car Between Floors
	9000,	// 456 - Max =       900.0		Time for OSERL Safety String Open
	1,		// 457 - Max =           1		Fire Service Hoistway Sensor Location
	55,		// 458 - Max =         5.5		Retiring Cam Drop Fail Safe Delay
	300,	// 459 - Max =         300		Velocity Difference 
	1,		// 460 - Max =           1		Velocity Difference Fault Disable
	1,		// 461 - Max =           1		SPB Direction Fault Disable
	1,		// 462 - Max =           1		Limit Direction Fault Disable
	1,		// 463 - Max =           1		Encoder Direction Fault Disable
	1,		// 464 - Max =           1		UL/DL Direction Fault Disable
	20,		// 465 - Max =         2.0		Relevel Start Velocity Hold Time
	2,		// 466 - Max =           2		Med Door Open Sequence for Re-open
	2,		// 467 - Max =           2		Med Ind Override
	1,		// 468 - Max =           1		Code Blue Bypass Disable
	1,		// 469 - Max =           1		Extended Door PI Message
	-1,		// 470 - Max =           8		First Return Car on Emergency Power for Feeder 2
	-1,		// 471 - Max =           8		First Car Selected on Emergency Power for Feeder 2
	1,		// 472 - Max =           1		Retiring Cam for Freight Output Enable
	30,		// 473 - Max =         3.0		Lantern Preference Change Delay
	300,	// 474 - Max =         300		Slip Velocity Difference Fault Trip Value
	1,		// 475 - Max =           1		DOB Overrides DCB on Phase 2
	1,		// 476 - Max =           1		Denver Fire Service Amendment to Close the Phase 1 Doors After Recall and Reopen From a Hall Call
	900,	// 477 - Max =        90.0		Denver FS Phase 1 Dwell Time
	2,		// 478 - Max =           2		Hall Call Security Control
	1,		// 479 - Max =           1		Slip Velocity Difference Disable
	1,		// 480 - Max =           1		Leveling Fault Trip
	1,		// 481 - Max =           1		Emergency Brake Switch Control
	1,		// 482 - Max =           1		Attendant Buzzer Control
	48,		// 483 - Max =          48		Radial Position Indicator
	1,		// 484 - Max =           1		Force Car to Recall On Independent Service to the Lobby When No Demand Is Present
	600,	// 485 - Max =        60.0		Time-out to Recall On Independent Service to the Lobby When No Demand Is Present
	30000,  // 486 - Max =      3000.0		Motor RPM
	1,		// 487 - Max =           1		VIP Multiple Calls Enable
	30,		// 488 - Max =          30		Service Output Control
	1,		// 489 - Max =           1		Can Encoder Baud Rate
	127,	// 490 - Max =         127		Can Encoder Node ID
	1,		// 491 - Max =           1		Code Blue Allows Single Free Car Call Enable
	1,		// 492 - Max =           1		Sabbath Collective Mode
	2,		// 493 - Max =           2		Return to Lobby Door Open Type
	1,		// 494 - Max =           1		Ring the Lantern in IR All the Time
	-2,		// 495 - Max =   Top Floor		High Priority Floor Number
	2540,	// 496 - Max =       254.0		High Priority Floor Time
	2,		// 497 - Max =           2		Emergency Manual Select Switch Recall Operation
	75,		// 498 - Max =          75		Access Speed
	1,		// 499 - Max =           1		Auto Service Time-out Enable
	3200,	// 500 - Max =        3200		Auto Service Time-out Time
	1,		// 501 - Max =           1		Car Call Lockouts on Sabbath Operation
	1,		// 502 - Max =           1		Medical Service Override Car Call Security
	32000,	// 503 - Max =      3200.0		HEOF Auto Reset Time
	1,		// 504 - Max =           1		CCPBS on Group Car Call Security
	15,		// 505 - Max =        0x0F		Dead Zone Selection
	1200,	// 506 - Max =        72.0		Access UT Distance
	1200,	// 507 - Max =        72.0		Access DT Distance
	7,		// 508 - Max =           3		Serial Port Comm Diag 2
	1,		// 509 - Max =           1		Third Riser Control
	6,		// 510 - Max =           3		Baud Rate for 485 Port
	4,		// 511 - Max =           3		Encoder Type
	1,		// 512 - Max =           1		Keb Drive Display Type
	1,		// 513 - Max =           1		Brake Phase Input
	1,		// 514 - Max =           1		Emergency Brake Phase Input
	1300,	// 515 - Max =       1.300		Distance Feed Forward
	2,		// 516 - Max =           2		Second Riser Car Calls Security by Direction
	1,		// 517 - Max =           1		Minimum Short Door Time Enable
	1,		// 518 - Max =           1		Fire Recall Reset 2
	1,		// 519 - Max =           1		Invert Hall Call Security Input
	1,		// 520 - Max =           1		DZ Gripper Trip Control
	32000,	// 521 - Max =      3200.0		Security Disable Time
	15,		// 522 - Max =        0x0F		DL Dead Zone Selection
	20,		// 523 - Max =         2.0		EE Test Time
	3,		// 524 - Max =           3		Reassign Car Calls When on Multiple Floor Mask Security (cons[SecFlCfg] = 2)
	15,		// 525 - Max =          15		Disable Opposite Hall Call
	30,		// 526 - Max =          30		Disable Opposite Hall Call Time
	1,		// 527 - Max =           1		Hall Elevator Off Options 3
	15,		// 528 - Max =          15		Car Elevator Off Options
	15,		// 529 - Max =          15		Car Elevator Off Options 2
	1,		// 530 - Max =           1		Code Blue Over Fire Service
	14,		// 531 - Max =          14		COP/Remote Disable
	1,		// 532 - Max =           1		Fire Service 2 with Special Devices (HYDRO ONLY)
	100,	// 533 - Max =        10.0		Auto Swing Door Open Delay
	320,	// 534 - Max =         320		CL Pulse Time
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 539
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 544
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 549
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 554
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 559
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 564
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 569
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 574
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 579
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 584
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 589
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 594
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 599
};
int16 fvars_min[nmbfvar] ={		// Note: -7 = cons[carnmb], -4 = cons[bottomf]
	10,		// 000 - Min =		 	   0	Y Delta Time 	
	0,		// 001 - Min =		 	   0	Fault Time 	
	0,		// 002 - Min =		 	   0	Reset Time 	
	0,		// 003 - Min =		 	   0	Double Stroke on Lanterns 	
	0,		// 004 - Min =		       0	Lantern On Time for Double Stroking 	
	0,		// 005 - Min =		 	   0	Lantern Off Time for Double Stroking 	
	2,		// 006 - Min =	   		 0.2	Handicap Floor Passing Beep Time 	
	100,	// 007 - Min =	  		10.0	Door Protection Time 	
	200,	// 008 - Min =	  		20.0	Nudge Time 	
	0, 		// 009 - Min =		 	   0	Pre-opening Delay Time 	
	10,		// 010 - Min =	         1.0	Car Call Door Dwell Time 	
	10,		// 011 - Min =	   		 1.0	Hall Call Door Dwell Time 	
	10,		// 012 - Min =	   		 1.0	Lobby Call Door Dwell Time 	
	10,		// 013 - Min =	   		 1.0	Extra Door Dwell Time 	
	10,		// 014 - Min =	   		 1.0	Non Interference Time 	
	-4,		// 015 - Min =	Bottom Floor	Lobby Floor 	
	-4,		// 016 - Min =	Bottom Floor	Fire Service Phase 1 Main Egress Floor 	
	-4,		// 017 - Min =	Bottom Floor	Fire Service Phase 1 Alternate Egress Floor 	
	200,	// 018 - Min = 			20.0	Stall Time 	
	2,		// 019 - Min = 			 0.2	Soft Stop Time 	
	300,	// 020 - Min = 			30.0	Generator Run Time 	
	0,		// 021 - Min = 			   0	Brake Pick Delay Time 	
	0,		// 022 - Min = 			   0	Brake Drop Delay Time 	
	50,		// 023 - Min = 			  50	Soft Start Jerk Rate 	
	50,		// 024 - Min = 			  50	Acceleration Rate 	
	50,		// 025 - Min = 			  50	Deceleration Jerk Rate 	
	50,		// 026 - Min = 			  50	Deceleration Rate 	
	12,		// 027 - Min =			 1.2	Floor Target Distance 	
	10,		// 028 - Min =			 1.0	Leveling Speed 	
	10,		// 029 - Min = 			 1.0	Relevel Speed 	
	0,		// 030 - Min = 			   0	Inspection Speed 	
	2000,	// 031 - Min = 			2000	Year 	
	1,		// 032 - Min = 			   1	Month 	
	31,		// 033 - Min = 			   1	Day 	
	0,		// 034 - Min = 			   0	Hour 	
	0,		// 035 - Min = 			   0	Minute 	
	50,		// 036 - Min = 			  50	Roll Over Jerk Rate 	
	0,		// 037 - Min = 			   0	EE Memory Size 	
	2000,	// 038 - Min = 			2000	Annual Safety Test Year 	
	1,		// 039 - Min = 			   1	Annual Safety Test Month 	
	1,		// 040 - Min =			   1	Annual Safety Test Day 	
	0,		// 041 - Min = 			   0	Pattern Delay 	
	60, 	// 042 - Min = 			 60%	Field Weaken Velocity Percent 	
    60,  	// 043 - Min = 			  60	Encoder PPR 	
	10,  	// 044 - Min = 			 1.0	Encoder RPM 	
	0, 		// 045 - Min = 			   0	User Interface Baud Rate  	
	0,		// 046 - Min = 			   0	Parking 	
	-4,		// 047 - Min = 	Bottom Floor	Emergency Power Return Floor 	
	1,		// 048 - Min = 			   1	Number of Cars to Run on Emergency Power 	
	0,		// 049 - Min = 			   0	First Car to Return on Emergency Power 	
	0,		// 050 - Min = 			   0	First Car Selected on Emergency Power 	
	10,		// 051 - Min = 			   1	Return Car Time-out Time 	
	0,		// 052 - Min = 			   0	DOB Opens Door On Nudging Is Set 	
	0,		// 053 - Min = 			   0	Emergency Dispatch Enable 	
	0,		// 054 - Min = 			   0	Number of Cars Requested at Lobby 	
	0,		// 055 - Min = 			   0	Next Up Selection 	
	0,		// 056 - Min = 			   0	Code Blue Car Select 	
	0,		// 057 - Min = 			   0	IR Car Select 	
	0,		// 058 - Min = 			   0	Brake Lift Switch 	
	0,		// 059 - Min = 			   0	Load Weighing Anti-nuisance Count 	
	0,		// 060 - Min = 			   0	Fire Switch Location 	
	0,		// 061 - Min = 			   0	ETA Minimum Time 	
	0,		// 062 - Min = 			   0	ETA for Coincident Car Call 	
	1,		// 063 - Min =			   1	Video Position for Car 1 	
	1,		// 064 - Min = 			   1	Video Position for Car 2 	
	1,		// 065 - Min =			   1	Video Position for Car 3 	
	1,		// 066 - Min = 			   1	Video Position for Car 4 	
	1,		// 067 - Min = 			   1	Video Position for Car 5 	
	1,		// 068 - Min = 			   1	Video Position for Car 6 	
	0,		// 069 - Min = 			   0	Run Count with No Passengers to Cancel Car Calls 	
	0,		// 070 - Min = 			   0	Independent Override Security 	
	0,		// 071 - Min = 			   0	Brake Pick Delay Time on Relevel 	
	0,		// 072 - Min = 			   0	Brake Cool on Relevel 	
	0,		// 073 - Min = 			   0	Up peak Trigger Time 	
	1,		// 074 - Min = 			   1	Up Peak Trigger Count 	
	1,		// 075 - Min = 			   1	Up Peak Car Call Count 	
	0,		// 076 - Min = 			   0	Up Peak Duration Time 	
	0,		// 077 - Min =			   0	Down Peak Trigger Time 	
	1,		// 078 - Min = 			   1	Down Peak Trigger Count 	
	0,		// 079 - Min =			   0	Down Peak Duration Time 	
	0,		// 080 - Min = 			   0	Parking Delay Time 	
	0,		// 081 - Min = 		  	   0	Hall Call Cross Assignment 	
	0,		// 082 - Min = 			   0	Hall Call Cross Assignment ETA 	
	-4,		// 083 - Min = 			   1	Medical Emergency Floor 	
	0,		// 084 - Min = 			   0	Medical Emergency Hall Switch location 	
	0,		// 085 - Min = 			   0	Fire Service Motor Room Sensor Return Floor 	
	0,		// 086 - Min = 			   0	Fire Service Hoistway Sensor Return Floor 	
	0,		// 087 - Min = 			   0	Phase 1 Fire Recall Reset 	
	5,		// 088 - Min =			   5	Acceleration and Deceleration Limit at Final Stop 	
	0,		// 089 - Min = 			   0	Pattern Delay During Relevel 	
	0,		// 090 - Min = 			   0	Delay Start Until Drive On 	
	0,		// 091 - Min = 			   0	Brake Opto Trigger Delay 	
	0,		// 092 - Min = 			   0	Invert Logic of Brake Lift Switch 	
	0,		// 093 - Min = 			   0	Invert Logic of In-service Light Output or In-use Light or Out of Service Alarm 	
	0,		// 094 - Min = 			   0	Ability to Change a Car Other Than 1 or 2 to Become the Master 	
	0,		// 095 - Min = 			   0	Time-out Video Display 	
	0,		// 096 - Min = 			   0	Close Doors While Trying to Run on Inspection 	
	0,		// 097 - Min = 			   0	Parking Floor 1 	
	0,		// 098 - Min = 			   0	Parking Floor 2 	
	0,		// 099 - Min = 			   0	Parking Floor 3 	
	0,		// 100 - Min =			   0	Parking Floor 4 	
	0,		// 101 - Min = 			   0	Parking Floor 5 	
	0,		// 102 - Min = 			   0	Parking Width 	
	0,		// 103 - Min = 			   0	Percent of Load to Balance the Counterweights (Balanced Load Percent) 	
	0,		// 104 - Min =			   0	Drive Starting Torque Multiplier (Torque Amount Variable) 	
	0,		// 105 - Min = 			   0	Load Bypass Percent 	
	0,		// 106 - Min = 			   0	Load Anti-nuisance Percent 	
	0,		// 107 - Min = 			   0	Load Dispatch Percent 	
	0,		// 108 - Min = 			   0	Load Overload Percent 	
	0,		// 109 - Min = 			   0	IR Control Flag 	
	20,		// 110 - Min = 			  20	Brake Pick Voltage 	
	1,		// 111 - Min = 			   0	Brake Pick Duration Time 	
	20,		// 112 - Min = 			  20	Brake Hold Voltage 	
	10,		// 113 - Min = 			  10	Brake Relevel Voltage 	
	80,		// 114 - Min = 			  80	Brake AC Line to Line Voltage 	
	1,		// 115 - Min = 			0.01	Brake Resistance 	
	0,		// 116 - Min = 			   0	Attendant Buzzer Delay Time 	
	0,		// 117 - Min = 			   0	Hall Fire Aux. Switch Enable 	
	0,		// 118 - Min = 			   0	Operation of Hall Fire Light 	
	0,		// 119 - Min = 			   0	COP/Remote CC Select 	
	0,		// 120 - Min = 			   0	Door Open or Close Delay Time 	
	0,		// 121 - Min = 			   0	Security Recall Option 	
	0,		// 122 - Min = 			   0	DOB Overrides Security 	
	0,		// 123 - Min = 			   0	Manual Door Buzzer Delay Time 	
	0,		// 124 - Min = 			   0	Security Floor 	
	0,		// 125 - Min = 			   0	Retiring Cam Pick Delay Time 	
	0,		// 126 - Min = 			   0	Short Door Time From a Reopen Signal 	
	0,		// 127 - Min = 			   0	Second IR Car Select 	
	0,		// 128 - Min = 			   0	Stop At Lobby 	
	0,		// 129 - Min = 			   0	Invert Cab Light Fan Output 	
	0,		// 130 - Min = 			   0	Invert Temperature Low Switch (TPL) 	
	0,		// 131 - Min = 			   0	Invert Low Pressure Switch 	
	0,		// 132 - Min = 			   0	Invert Low Oil Switch 	
	0,		// 133 - Min = 			   0	Nudging with No Onward Call 	
	0,		// 134 - Min = 			   0	Fire Option Flag 	
	0,		// 135 - Min = 			   0	Attendant Car Call Flag 	
	10,		// 136 - Min = 			 1.0	Code Blue Door Open Time 	
	0,		// 137 - Min = 			   0	Request Ind Car For Code Blue 	
	0,		// 138 - Min = 			   0	Code Blue Light Control (Flash Light) 	
	0,		// 139 - Min = 			   0	Hospital Service Door Close on Car Call 	
	0,		// 140 - Min = 			   0	Gripper Reset Time(r) 	
	0,		// 141 - Min = 			   0	Independent Door Close on Car Call 	
	0,		// 142 - Min = 			0.00	Torque Offset 	
	0,		// 143 - Min = 			   0	Gripper Test From Safety Processor Board 	
	0,		// 144 - Min = 			   0	Single Automatic Pushbutton Control 	
	0,		// 145 - Min = 			   0	Door Open When DOL Signal Not Active 	
	0,		// 146 - Min = 			   0	Previously: fvdisglt - Disable Gate and Lock Test for New York City 	
	0,		// 147 - Min = 			 0.0	Relevel Start Speed 	
	80,		// 148 - Min = 			  80	Emergency Deceleration Rate 	
	0,		// 149 - Min = 			   0	Disable DCB from Shortening Door Time 	
	0,		// 150 - Min = 			   0	Return to Lobby Option 	
	1,		// 151 - Min = 			   1	Video Position for Car 7 	
	1,		// 152 - Min = 			   1	Video Position for Car 8 	
	0,		// 153 - Min = 			   0	Parking Type 	
	0,		// 154 - Min = 			  29	Parking Floor 6 	
	0,		// 155 - Min = 			  29	Parking Floor 7 	
	0,		// 156 - Min = 			   0	Non-Simultaneous Doors Operation 	
	0,		// 157 - Min = 			   0	Pre-open Doors 	
	80,		// 158 - Min = 			  80	Over Speed Percent 	
	0,		// 159 - Min = 			   0	Position Pulse Count Update 	
	1,		// 160 - Min = 			   1	Position Pulse Delay Error 	
	0,		// 161 - Min = 			   0	Gripper Buzzer Enable 	
	0,		// 162 - Min = 			   0	Relevel Delay Time 	
	0,		// 163 - Min = 			   0	Fire Option 2 	
	0,		// 164 - Min = 			   0	Close Gate on Swing Door with No Preference 	
	3,		// 165 - Min = 			   3	Leveling Fault Count 	
	0,		// 166 - Min = 			   0	Torque Amount in Down Direction 	
	0,		// 167 - Min = 			   0	Ramp Torque Time During Pattern Delay 	
	0,		// 168 - Min =			 0.0	Electric Eye Time-out Time 	
	0,		// 169 - Min = 			   0	Cars Out of Service Hall Call Cancel 	
	0,		// 170 - Min = 			   0	Nudging Control 	
	100,	// 171 - Min = 			10.0	Fire Phase 1 Door Time-out (Ind and Att) 	
	0,		// 172 - Min = 			   0	No Hall Call Button Door Reopen with Onward Call 	
	1,		// 173 - Min = 			   1	Drive Ready Reset Count 	
	10,		// 174 - Min = 			 1.0	Return to Lobby Door Dwell Time 	
	0,		// 175 - Min = 			   0	Next Up Lobby Lantern Control 	
	0,		// 176 - Min = 			   0	Next Up Preference Control 	
	0,		// 177 - Min = 			   0	Second Riser Control 	
	0,		// 178 - Min = 			   0	Second Riser Lantern Control 	
	0,		// 179 - Min = 			   0	Attendant ETA Preference Time 	
	0,		// 180 - Min = 			   0	Handicap Buzzer Disabled at Invalid Floor	  	
	10,		// 181 - Min = 			 1.0	VIP Door Open Time 	
	0,		// 182 - Min = 			   0	VIP Operation Flags 	
	0,		// 183 - Min = 			   0	Number of VIP cars 	
	0,		// 184 - Min = 			   0	Emergency Power Op led (OP) Function 	
	0,		// 185 - Min = 			   0	Emergency Power Park led (PRK) Function 	
	0,		// 186 - Min = 			   0	Hall Elevator Off Options 	
	0,		// 187 - Min = 			   0	Drive Baud Rate 	
	0,		// 188 - Min = 			   0	Drive Update Rate 	
	1,		// 189 - Min = 			   1	Alternate Lobby Floor 	
	1,		// 190 - Min = 			   1	Alternate Parking Floor 	
	0,		// 191 - Min = 			   0	Lobby Request Control 	
	0,		// 192 - Min = 			   0	Handicap Car Capacity 	
	10,		// 193 - Min = 			 1.0	Freight Door Close Time-out 	
	0,		// 194 - Min = 			   0	Handicap Car Wait Time 	
	100,	// 195 - Min = 			10.0	Short Floor Distance 	
	0,		// 196 - Min = 			   0	Retiring Cam Control 	
	0,		// 197 - Min = 			   0	Recall from Fire Alternate Floor 	
	10,		// 198 - Min = 			   0	Auto Learn Hoistway Stall Time 	
	0,		// 199 - Min = 			   0	Position Pulse Count Error 	
	0,		// 200 - Min = 			   0	Attendant Buzzer Time (When Off) 	
	10,		// 201 - Min = 			 1.0	Attendant Buzzer Time (When On) 	
	0,		// 202 - Min = 			   0	Brake Pick Voltage Rate Time 	
	0,		// 203 - Min = 			   0	Brake Relevel Voltage Rate Time 	
	0,		// 204 - Min = 			   0	Brake Pick Start Voltage 	
	0,		// 205 - Min = 			   0	Brake Relevel Start Voltage 	
	0,		// 206 - Min = 			   0	Short Floor Down Slowdown Time 	
	0,		// 207 - Min = 			   0	Mid Short Floor Down Slowdown Time 	
	0,		// 208 - Min = 			   0	Short Floor Up Slowdown Time 	
	0,		// 209 - Min = 			   0	Mid Short Floor Up Slowdown Time 	
	0,		// 210 - Min = 			   0	Hall Switch Elevator Off Return Floor 	
	0,		// 211 - Min = 			   0	Service Message 1 display 	
	0,		// 212 - Min = 			   0	Service Message 2 display 	
	0,		// 213 - Min = 			   0	Governor Gripper Trip Control 	
	50,		// 214 - Min = 			  50	Short Floor Soft Start Rate 	
	50,		// 215 - Min = 			  50	Short Floor Acceleration Rate 	
	50,		// 216 - Min = 			  50	Short Floor Roll Over Jerk Rate 	
	50,		// 217 - Min = 			  50	Short Floor Deceleration Jerk Rate 	
	50,		// 218 - Min = 			  50	Short Floor Deceleration Rate 	
	12,		// 219 - Min = 			 1.2	Short Floor Target Distance 	
	0,		// 220 - Min = 			   0	Short Floor Control 	
	0,		// 221 - Min = 			   0	Security Recall Control 2 	
	0,		// 222 - Min = 			 0.0	Emergency Power Recall Delay Time 	
	1,		// 223 - Min = 			   1	Group Car Call Security Override Time 	
	0,		// 224 - Min = 			 0.0	Emergency Brake Drop Delay 	
	0,		// 225 - Min = 			 0.0	Emergency Brake Pick Time 	
	0,		// 226 - Min = 			   0	Hall Call Output On Brightness for led 	
	0,		// 227 - Min = 			   0	Hall Call Output Off Brightness for led 	
	-4,		// 228 - Min = 	Bottom Floor	Access Top Floor 	
	-4,		// 229 - Min = 	Bottom Floor	Access Bottom Floor 	
	0,		// 230 - Min = 			   0	Stop On Position Pulse 	
	0,		// 231 - Min = 			   0	Sabbath Svc Restart Time 	
	0,		// 232 - Min = 			   0	Power Door Open Relay On Time 	
	0,		// 233 - Min = 			   0	Display Fault Automatically on LCD when Fault Occurs	 	
	0,		// 234 - Min =			   0	Password for Adjusting Field Variables 	
	0,		// 235 - Min = 			   0	Password Time-out Time 	
	0,		// 236 - Min = 			   0	Behind Car Call Cancel 	
	0,		// 237 - Min = 			   0	Door Open Advance Door Enable Non-Simultaneous Doors 	
	0,		// 238 - Min = 			   0	Group Service Number of Parking Cars 	
	0,		// 239 - Min = 			   0	Binary Preset Update when Position Does Not Match 	
	0,		// 240 - Min = 			   0	Short Floor hsf with No hsv High Speed Valve 	
	0,		// 241 - Min = 			   0	Up Peak Control 	
	0,		// 242 - Min = 			   0	Up Peak Pool 	
	0,		// 243 - Min = 			   0	Down Peak Control 	
	0,		// 244 - Min = 			   0	Down Peak Pool 	
	0,		// 245 - Min = 			   0	Cross Assignment Cars from Old Group 	
	0,		// 246 - Min = 			   0	Modbus Protocol 	
	0,		// 247 - Min = 			   0	Fire Alternate Recall After FS Turns Off 	
	50,		// 248 - Min = 			  50	Inspection Deceleration Stop Rate 	
	0,		// 249 - Min = 			   0	Emergency Power Recovery Direction 	
	0,		// 250 - Min = 			   0	Brake Drop Voltage  	
	0,		// 251 - Min = 			 0.0	Brake Voltage Drop Rate 	
	0,		// 252 - Min = 			   0	Brake Drop Timing During Leveling Flag 	
	15,		// 253 - Min = 			  15	Recovery Velocity 	
	50,		// 254 - Min = 			  50	Emergency Deceleration to Leveling Rate 	
	10,		// 255 - Min = 			  10	Emergency Power Recovery Velocity 	
	0,		// 256 - Min = 			   0	Run Cycle Time 	
	0,		// 257 - Min = 			   0	Hall Call Up On Red Intensity 	
	0,		// 258 - Min = 			   0	Hall Call Up Off Red Intensity 	
	0,		// 259 - Min = 			   0	Hall Call Up On Green Intensity 	
	0,		// 260 - Min = 			   0	Hall Call Up Off Green Intensity 	
	0,		// 261 - Min = 			   0	Hall Call Up On Blue Intensity 	
	0,		// 262 - Min = 			   0	Hall Call Up Off Blue Intensity 	
	0,		// 263 - Min = 			   0	Door Open Light Control on CCS board 	
	0,		// 264 - Min = 			   0	Hall Call Button Up On Color 	
	0,		// 265 - Min = 			   0	Hall Call Button Up Off Color 	
	0,		// 266 - Min = 			   0	Hall Call Button Security Color 	
	0,		// 267 - Min = 			   0	Hall Call Button Security Light 	
	0,		// 268 - Min = 			   0	Hall Call Security Brightness 	
	0,		// 269 - Min = 			   0	Hall Call Security Red Intensity 	
	0,		// 270 - Min = 			   0	Hall Call Security Green Intensity 	
	0,		// 271 - Min = 			   0	Hall Call Security Blue Intensity 	
	0,		// 272 - Min = 			   0	Fire Service Hoistway Sensor 2 Return Floor 	
	250,	// 273 - Min = 		   0.250	Drive Speed Multiplier 	
	0,		// 274 - Min = 			   0	Advanced Pre-torque 	
	0,		// 275 - Min = 			   0	Car Call Button On Color 	
	0,		// 276 - Min = 			   0	Car Call Button Off Color 	
	0,		// 277 - Min = 			   0	Car Call Output On Brightness for led 	
	0,		// 278 - Min = 			   0	Car Call Output Off Brightness for led 	
	0,		// 279 - Min = 			   0	Car Call On Red Intensity 	
	0,		// 280 - Min = 			   0	Car Call Off Red Intensity 	
	0,		// 281 - Min = 			   0	Car Call On Green Intensity 	
	0,		// 282 - Min = 			   0	Car Call Off Green Intensity 	
	0,		// 283 - Min = 			   0	Car Call On Blue Intensity 	
	0,		// 284 - Min = 			   0	Car Call Off Blue Intensity 	
	0,		// 285 - Min = 			   0	Car Call Button Light Control 	
	0,		// 286 - Min = 			   0	Car Call Button Security Color 	
	0,		// 287 - Min = 			   0	Car Call Security Brightness 	
	0,		// 288 - Min = 			   0	Car Call Security Red Intensity 	
	0,		// 289 - Min = 			   0	Car Call Security Green Intensity 	
	0,		// 290 - Min = 			   0	Car Call Security Blue Intensity 	
	0,		// 291 - Min = 			   0	Sabbath Enable Control Variable 	
	0,		// 292 - Min = 			   0	Low Line Voltage (RMS value) 	
	0,		// 293 - Min = 			   0	Low Door Voltage (RMS value) 	
	10,		// 294 - Min = 			  10	Emergency Power Top Speed 	
	50,		// 295 - Min = 			  50	Emergency Power Soft Start Jerk Rate 	
	50,		// 296 - Min = 			  50	Emergency Power Acceleration Rate 	
	50,		// 297 - Min = 			  50	Emergency Power Roll Over Jerk Rate 	
	50,		// 298 - Min = 			  50	Emergency Power Deceleration Jerk Rate 	
	50,		// 299 - Min = 			  50	Emergency Power Deceleration Rate 	
	12,		// 300 - Min = 			 1.2	Emergency Power Target Distance 	
	25,		// 301 - Min = 			  25	Car Top Speed 	
	0,		// 302 - Min = 			   0	Inspection Velocity from ISER output 	
	10,		// 303 - Min = 			 1.0	Emergency Power Recover to Floor Time 	
	0,		// 304 - Min = 			   0	Fire Light Control during EP 	
	0,		// 305 - Min = 			   0	Emergency Power Manual Select Enable 	
	0,		// 306 - Min = 			   0	Fire Light Control with Car Out of Service (Low Pressure, eaq, Stall) 	
	10,		// 307 - Min = 			  10	Sabbath Door Dwell Time 	
	10,		// 308 - Min = 			  10	Sabbath Door Buzzer Time Prior to Doors Closing 	
	0,		// 309 - Min = 		   000.0	Low Battery Capacity Level 	
	0,		// 310 - Min = 			   0	Sabbath Disable Control Variable 	
	0,		// 311 - Min = 			   0	Electric Eye Cancel Door Time 	
	0,		// 312 - Min = 			   0	Emergency Power Car at Recall Floor with Door Open 	
	0,		// 313 - Min = 			   0	Emergency Power Car Not at Recall Floor with Door Open 	
	0,		// 314 - Min = 			   0	Assign Parking Floor to Car with Door Open 	
	0,		// 315 - Min = 			   0	Hall Lantern Delay 	
	0,		// 316 - Min = 			   0	Sabbath Enable Control 2 Variable 	
	0,		// 317 - Min = 			   0	Cab Lantern Control 	
	0,		// 318 - Min = 			   0	Max Door Hold Time from ED Input 	
	0,		// 319 - Min = 			   0	Access Run Door Close 	
	0,		// 320 - Min = 			   0	Car Call Button Attendant Up Color 	
	0,		// 321 - Min = 			   0	Car Call Attendant Up Brightness 	
	0,		// 322 - Min = 			   0	Car Call Attendant Up Red Intensity 	
	0,		// 323 - Min = 			   0	Car Call Attendant Up Green Intensity 	
	0,		// 324 - Min = 			   0	Car Call Attendant Up Blue Intensity 	
	0,		// 325 - Min = 			   0	Car Call Button Attendant Dn Color 	
	0,		// 326 - Min = 			   0	Car Call Attendant Dn Brightness 	
	0,		// 327 - Min = 			   0	Car Call Attendant Dn Red Intensity 	
	0,		// 328 - Min = 			   0	Car Call Attendant Dn Green Intensity 	
	0,		// 329 - Min = 			   0	Car Call Attendant Dn Blue Intensity 	
	0,		// 330 - Min = 			   0	Fire light Color 	
	0,		// 331 - Min = 			   0	Fire light Brightness 	
	0,		// 332 - Min = 			   0	Fire light Red Intensity 	
	0,		// 333 - Min = 			   0	Fire light Green Intensity 	
	0,		// 334 - Min = 			   0	Fire light Blue Intensity 	
	0,		// 335 - Min = 			   0	Medical light Color 	
	0,		// 336 - Min = 			   0	Medical light Brightness 	
	0,		// 337 - Min = 			   0	Medical light Red Intensity 	
	0,		// 338 - Min = 			   0	Medical light Green Intensity 	
	0,		// 339 - Min = 			   0	Medical light Blue Intensity 	
	0,		// 340 - Min = 			   0	Emergency light Color 	
	0,		// 341 - Min = 			   0	Emergency light Brightness 	
	0,		// 342 - Min = 			   0	Emergency light Red Intensity 	
	0,		// 343 - Min = 			   0	Emergency light Green Intensity 	
	0,		// 344 - Min = 			   0	Emergency light Blue Intensity 	
	0,		// 345 - Min = 			   0	OTS light Color 	
	0,		// 346 - Min = 			   0	OTS light Brightness 	
	0,		// 347 - Min = 			   0	OTS light Red Intensity 	
	0,		// 348 - Min = 			   0	OTS light Green Intensity 	
	0,		// 349 - Min = 			   0	OTS light Blue Intensity 	
	0,		// 350 - Min = 			   0	Backlight Output lights 	
	0,		// 351 - Min = 			   0	Can Sync Count 	
	0,		// 352 - Min = 			   0	Hall Call Dn On Color 	
	0,		// 353 - Min = 			   0	Hall Call Dn On Brightness 	
	0,		// 354 - Min = 			   0	Hall Call Dn On Red Intensity 	
	0,		// 355 - Min = 			   0	Hall Call Dn On Green Intensity 	
	0,		// 356 - Min = 			   0	Hall Call Dn On Blue Intensity 	
	0,		// 357 - Min = 			   0	Hall Call Dn Off Color 	
	0,		// 358 - Min = 			   0	Hall Call Dn Off Brightness 	
	0,		// 359 - Min = 			   0	Hall Call Dn Off Red Intensity 	
	0,		// 360 - Min = 			   0	Hall Call Dn Off Green Intensity 	
	0,		// 361 - Min = 			   0	Hall Call Dn Off Blue Intensity 	
	0,		// 362 - Min = 			   0	Hall Call CB On Color 	
	0,		// 363 - Min = 			   0	Hall Call CB On Brightness 	
	0,		// 364 - Min = 			   0	Hall Call CB On Red Intensity 	
	0,		// 365 - Min = 			   0	Hall Call CB On Green Intensity 	
	0,		// 366 - Min = 			   0	Hall Call CB On Blue Intensity 	
	0,		// 367 - Min = 			   0	Hall Call VIP On Color 	
	0,		// 368 - Min = 			   0	Hall Call VIP On Brightness 	
	0,		// 369 - Min = 			   0	Hall Call VIP On Red Intensity 	
	0,		// 370 - Min = 			   0	Hall Call VIP On Green Intensity 	
	0,		// 371 - Min = 			   0	Hall Call VIP On Blue Intensity 	
	0,		// 372 - Min = 			   0	Hall Call IR Up On Color 	
	0,		// 373 - Min = 			   0	Hall Call IR Up On Brightness 	
	0,		// 374 - Min = 			   0	Hall Call IR Up On Red Intensity 	
	0,		// 375 - Min = 			   0	Hall Call IR Up On Green Intensity 	
	0,		// 376 - Min = 			   0	Hall Call IR Up On Blue Intensity 	
	0,		// 377 - Min = 			   0	Hall Call IR Dn On Color 	
	0,		// 378 - Min = 			   0	Hall Call IR Dn On Brightness 	
	0,		// 379 - Min = 			   0	Hall Call IR Dn On Red Intensity 	
	0,		// 380 - Min = 			   0	Hall Call IR Dn On Green Intensity 	
	0,		// 381 - Min = 			   0	Hall Call IR Dn On Blue Intensity 	
	0,		// 382 - Min = 			   0	Hall Call IR Up Off Color 	
	0,		// 383 - Min = 			   0	Hall Call IR Up Off Brightness 	
	0,		// 384 - Min = 			   0	Hall Call IR Up Off Red Intensity 	
	0,		// 385 - Min = 			   0	Hall Call IR Up Off Green Intensity 	
	0,		// 386 - Min = 			   0	Hall Call IR Up Off Blue Intensity 	
	0,		// 387 - Min = 			   0	Hall Call IR Dn Off Color 	
	0,		// 388 - Min = 			   0	Hall Call IR Dn Off Brightness 	
	0,		// 389 - Min = 			   0	Hall Call IR Dn Off Red Intensity 	
	0,		// 390 - Min = 			   0	Hall Call IR Dn Off Green Intensity 	
	0,		// 391 - Min = 			   0	Hall Call IR Dn Off Blue Intensity 	
	0,		// 392 - Min = 			   0	Hall Call Output Up On brightness for led 	
	0,		// 393 - Min = 			   0	Hall Call Output Up Off brightness for led 	
	0,		// 394 - Min = 			   0	Code Blue Location 	
	0,		// 395 - Min = 			   0	VIP Location 	
	0,		// 396 - Min = 			   0	IR light Color Control 	
	0,		// 397 - Min = 			   0	Hall Call CB Off Color 	
	0,		// 398 - Min = 			   0	Hall Call CB Off Brightness 	
	0,		// 399 - Min = 			   0	Hall Call CB Off Red Intensity 	
	0,		// 400 - Min = 			   0	Hall Call CB Off Green Intensity 	
	0,		// 401 - Min = 			   0	Hall Call CB Off Blue Intensity 	
	0,		// 402 - Min = 			   0	Hall Call VIP Off Color 	
	0,		// 403 - Min = 			   0	Hall Call VIP Off Brightness 	
	0,		// 404 - Min = 			   0	Hall Call VIP Off Red Intensity 	
	0,		// 405 - Min = 			   0	Hall Call VIP Off Green Intensity 	
	0,		// 406 - Min = 			   0	Hall Call VIP Off Blue Intensity 	
	0,		// 407 - Max = 			   0	VIP Lantern Control 	
	0,		// 408 - Max = 			   0	Group Car Call Override Control 	
	0,		// 409 - Min = 			   0	Exclusion Fault 1 	
	0,		// 410 - Min = 			   0	Exclusion Fault 2          	
	0,		// 411 - Min = 			   0	Exclusion Fault 3 	
	0,		// 412 - Min = 			   0	Exclusion Fault 4         	
	0,		// 413 - Min = 			   0	Exclusion Fault 5 	
	0,		// 414 - Min = 			   0	Exclusion Fault 6         	
	0,		// 415 - Min = 			  20	Emergency Brake Pick Voltage 	
	0,		// 416 - Min = 			  20	Emergency Brake Hold Voltage 	
	0,		// 417 - Min = 			 0.0	Emergency Brake Pick Rate 	
	0,		// 418 - Min = 			 0.0	Emergency Brake Drop Rate 	
	80,		// 419 - Min = 			  80	Emergency Brake AC Line to Line Voltage 	
	0,		// 420 - Min = 			   0	Brake Opto Trigger Delay 	
	0,		// 421 - Min = 			   0	Can Baud Rate 	
	0,		// 422 - Min = 			   0	Car Call Arrival Lantern 	
	0,		// 423 - Min = 			   0	Code Blue Second Riser Car Select 	
	0,		// 424 - Min = 			   0	Code Blue over IR car 	
	0,		// 425 - Min = 			   0	IR Car Code Blue Penalty Time 	
	0,		// 426 - Min = 			   0	INSEC (In Security Output Invert) 	
	0,		// 427 - Min = 			   0	Attendant Manual Direction Enable 	
	1,		// 428 - Min = 			   1	Encoder Debounce Interval 	
	2,		// 429 - Min = 			   2	Encoder Number of Samples 	
	1,		// 430 - Min = 			 0.1	Handicap Buzzer on Car Call Beep Time 	
	0,		// 431 - Min = 			   0	Service UPS Mode 	
	0,		// 432 - Min = 			   0	Serial Port Comm Diag 	
	0,		// 433 - Min = 			   0	User Port 2 Baud Rate 	
	0,		// 434 - Min = 			   0	Cpu Timing Output Control 	
	0,		// 435 - Min = 			   0	Service Message 3 Display 	
	0,		// 436 - Min = 			   0	Hall Call Assignment Secure Type 	
	0,		// 437 - Min = 			   0	UPS Baud Rate 	
	0,		// 438 - Min = 			   0	HC Acknowledge Attendant Buzzer 	
	0,		// 439 - Min = 			   0	Second Code Blue Car Select 	
	0,		// 440 - Min = 			   0	Hall Elevator Off Options 2  	
	0,		// 441 - Min = 			   0	Code Blue Buzzer Control 	
	0,		// 442 - Min = 			   0	Code Blue Override Independent 	
	0,		// 443 - Min = 			   0	Code Blue Recall Any Car If CB Cars 1 And 2 Are Not Available 	
	0,		// 444 - Min = 			   0	Hall Elevator Off Override 	
	0,		// 445 - Min = 			   0	NYC FS2 Door Reopen on STP 	
	0,		// 446 - Min = 			 0.0	Power Loss Brake On Time 	
	0,		// 447 - Min = 			 0.0	Power Loss Brake Off Time 	
	0,		// 448 - Min = 			   0	Select and Prioritize the ATT car to Run on Emergency Power Service 	
	0,		// 449 - Min = 			   0	Encoder Direction for Open Can Encoder 	
	0,		// 450 - Min = 			   0	Emergency Cars Finished Recalling Output enable (EPCOM) 	
	0,		// 451 - Min = 			   0	Out of Service Light control 	
	-4,		// 452 - Min = 	Bottom Floor	Extra Door Time Location  	
	0,		// 453 - Min = 			   1	Out of Service Light Control 	
	0,		// 454 - Min = 			 600	Time for OSERL Not Answering Call 	
	0,  	// 455 - Min = 			 600	Time for OSERL car Between Floors 	
	0, 		// 456 - Min = 			 600	Time for OSERL Safety String Open 	
	0,		// 457 - Min = 			   0	Fire Service Hoistway Sensor Location 	
	5,		// 458 - Min = 			   5	Retiring Cam Drop Fail Safe Delay 	
	50,		// 459 - Min = 			  50	Velocity Difference  	
	0,		// 460 - Min = 			   0	Velocity Difference Fault Disable 	
	0,		// 461 - Min = 			   0	SPB Direction Fault Disable 	
	0,		// 462 - Min = 			   0	Limit Direction Fault Disable 	
	0,		// 463 - Min = 			   0	Encoder Direction Fault Disable 	
	0,		// 464 - Min = 			   0	UL/DL Direction Fault Disable 	
	0,		// 465 - Min = 			   0	Relevel Start Velocity Hold Time 	
	0,		// 466 - Min = 			   0	Med Door Open Sequence for Re-open 	
	0,		// 467 - Min = 			   0	Med Ind Override 	
	0,		// 468 - Min = 			   0	Code Blue Bypass Disable 	
	0,		// 469 - Min = 			   0	Extended Door PI Message 	
	0,		// 470 - Min = 			   0	First Return Car on Emergency Power for Feeder 2 	
	0,		// 471 - Min = 			   0	First Car Selected on Emergency Power for Feeder 2 	
	0,		// 472 - Min = 			   0	Retiring Cam for Freight Output Enable 	
	0,		// 473 - Min = 			   0	Lantern Preference Change Delay 	
	0,		// 474 - Min = 			   0	Slip Velocity Difference Fault Trip Value 	
	0,		// 475 - Min = 			   0	DOB Overrides DCB on Phase 2 	
	0,		// 476 - Min = 			   0	Denver Fire Service Amendment to Close the Phase 1 Doors After Recall and Reopen From a Hall Call 	
	10,		// 477 - Min = 			 1.0	Denver FS Phase 1 Dwell Time 	
	0,		// 478 - Min = 			   0	Hall Call Security Control 	
	0,		// 479 - Min = 			   0	Slip Velocity Difference Disable 	
	0,		// 480 - Min = 			   0	Leveling Fault Trip 	
	0,		// 481 - Min = 			   0	Emergency Brake Switch Control 	
	0,		// 482 - Min = 			   0	Attendant Buzzer Control 	
	0,		// 483 - Min = 			   6	Radial Position Indicator 	
	0,		// 484 - Min = 			   0	Force Car to Recall On Independent Service to the Lobby When No Demand Is Present 	
	100,	// 485 - Min = 			  10	Time-out to Recall On Independent Service to the Lobby When No Demand Is Present 	
	0,  	// 486 - Min = 			   0	Motor RPM 	
	0,		// 487 - Min = 			   0	VIP Multiple Calls Enable 	
	0,		// 488 - Min = 			   0	Service Output Control 	
	0,		// 489 - Min = 			   0	Can Encoder Baud Rate 	
	1,		// 490 - Min = 			   1	Can Encoder Node ID 	
	0,		// 491 - Min = 			   0	Code Blue Allows Single Free Car Call Enable 	
	0,		// 492 - Min = 			   0	Sabbath Collective Mode 	
	0,		// 493 - Min = 			   0	Return to Lobby Door Open Type 	
	0,		// 494 - Min = 			   0	Ring the Lantern in IR All the Time 	
	0,		// 495 - Min = 			   0	High Priority Floor Number 	
	60,		// 496 - Min = 			   6	High Priority Floor Time 	
	0,		// 497 - Min = 			   0	Emergency Manual Select Switch Recall Operation 	
	0,		// 498 - Min = 			   0	Access Speed 	
	0,		// 499 - Min = 			   0	Auto Service Time-out Enable 	
	10,		// 500 - Min = 			  10	Auto Service Time-out Time 	
	0,		// 501 - Min = 			   0	Car Call Lockouts on Sabbath Operation 	
	0,		// 502 - Min = 			   0	Medical Service Override Car Call Security 	
	0,		// 503 - Min = 			 0.0	HEOF Auto Reset Time 	
	0,		// 504 - Min = 			   0	CCPBS on Group Car Call Security 	
	0,		// 505 - Min = 			   0	Dead Zone Selection 	
	0,		// 506 - Min = 			   0	Access UT Distance 	
	0,		// 507 - Min = 			   0	Access DT Distance 	
	0,		// 508 - Min = 			   0	Serial Port Comm Diag 2 	
	0,		// 509 - Min = 			   0	Third Riser Control 	
	0,		// 510 - Min = 			   0	Baud Rate for 485 Port 	
	0,		// 511 - Min = 			   0	Encoder Type 	
	0,		// 512 - Min = 			   0	Keb Drive Display Type 	
	0,		// 513 - Min = 			   0	Brake Phase Input 	
	0,		// 514 - Min = 			   0	Emergency Brake Phase Input 	
	0,		// 515 - Min = 			   0	Distance Feed Forward 	
	0,		// 516 - Min = 			   0	Second Riser Car Calls Security by Direction 	
	0,		// 517 - Min = 			   0	Minimum Short Door Time Enable 	
	0,		// 518 - Min = 			   0	Fire Recall Reset 2 	
	0,		// 519 - Min = 			   0	Invert Hall Call Security Input 	
	0,		// 520 - Min = 			   0	DZ Gripper Trip Control 	
	0,		// 521 - Min = 			   0	Security Disable Time 	
	0,		// 522 - Min = 			   0	DL Dead Zone Selection 	
	0,		// 523 - Min = 			   0	EE Test Time 	
	0,		// 524 - Min = 			   0	Reassign Car Calls When on Multiple Floor Mask Security (cons[SecFlCfg] = 2) 	
	0,		// 525 - Min = 			   0	Disable Opposite Hall Call 	
	10,		// 526 - Min = 			  10	Disable Opposite Hall Call Time 	
	0,		// 527 - Min = 			   0	Hall Elevator Off Options 3 	
	0,		// 528 - Min = 			   0	Car Elevator Off Options 	
	0,		// 529 - Min = 			   0	Car Elevator Off Options 2 	
	0,		// 530 - Min = 			   0	Code Blue Over Fire Service 	
	0,		// 531 - Min = 			   0	COP/Remote Disable 	
	0,		// 532 - Min = 			   0	Fire Service 2 with Special Devices (HYDRO ONLY) 	
	0,		// 533 - Min = 			   0	Auto Swing Door Open Delay 	
	0,		// 534 - Min = 			   0	CL Pulse Time 	
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 539
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 544
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 549
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 554
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 559
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 564
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 569
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 574
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 579
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 584
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 589
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 594
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 599
};
int16 fvars_default[nmbfvar] = {
	15,		// 000 - default =    1.5	Y Delta Time
	20,		// 001 - default =    2.0	Fault Time
	50,     // 002 - default =    5.0	Reset Time
	1,      // 003 - default =      1	Double Stroke on Lanterns
	7,      // 004 - default =    0.7	Lantern On Time for Double Stroking
	2,      // 005 - default =    0.2	Lantern Off Time for Double Stroking
	5,      // 006 - default =    0.5	Handicap Floor Passing Beep Time
	250,    // 007 - default =   25.0	Door Protection Time
	600,    // 008 - default =   60.0	Nudge Time
	5,      // 009 - default =    0.5	Pre-opening Delay Time
	20,     // 010 - default =    2.0	Car Call Door Dwell Time
	40,     // 011 - default =    4.0	Hall Call Door Dwell Time
	50,     // 012 - default =    5.0	Lobby Call Door Dwell Time
	250,    // 013 - default =   25.0	Extra Door Dwell Time
	20,     // 014 - default =    2.0	Non Interference Time
	1,      // 015 - default =      1	Lobby Floor
	1,      // 016 - default =      1	Fire Service Phase 1 Main Egress Floor
	2,      // 017 - default =      2	Fire Service Phase 1 Alternate Egress Floor
	600,    // 018 - default =   60.0	Stall Time
	10,     // 019 - default =    1.0	Soft Stop Time
	3600,   // 020 - default =  360.0	Generator Run Time
	0,      // 021 - default =    0.0	Brake Pick Delay Time
	1,      // 022 - default =    0.1	Brake Drop Delay Time
	125,    // 023 - default =    125	Soft Start Jerk Rate
	92,     // 024 - default =     92	Acceleration Rate
	125,    // 025 - default =    125	Deceleration Jerk Rate
	92,     // 026 - default =     92	Deceleration Rate
	120,    // 027 - default =   12.0	Floor Target Distance
	50,     // 028 - default =    5.0	Leveling Speed
	60,     // 029 - default =    6.0	Relevel Speed
	40,     // 030 - default =     40	Inspection Speed
	0,      // 031 - default =      0	Year
	0,      // 032 - default =      0	Month
	0,      // 033 - default =      0	Day
	0,      // 034 - default =      0	Hour
	0,      // 035 - default =      0	Minute
	125,    // 036 - default =    125	Roll Over Jerk Rate
	0,      // 037 - default =      0	EE Memory Size
	0,      // 038 - default =      0	Annual Safety Test Year
	0,      // 039 - default =      0	Annual Safety Test Month
	0,      // 040 - default =      0	Annual Safety Test Day
	0,      // 041 - default =   0.00	Pattern Delay
	110,	// 042 - default =    110	Field Weaken Velocity Percent
	2048,	// 043 - default =   2048	Encoder PPR
	1050,	// 044 - default = 1050.0	Encoder RPM
	6,		// 045 - default =      6	User Interface Baud Rate 
	1,		// 046 - default =      1	Parking
	1,		// 047 - default =      1	Emergency Power Return Floor
	1,		// 048 - default =      1	Number of Cars to Run on Emergency Power
	1,		// 049 - default =      1	First Car to Return on Emergency Power
	1,		// 050 - default =      1	First Car Selected on Emergency Power
	600,	// 051 - default =   60.0	Return Car Time-out Time
	0,		// 052 - default =      0	DOB Opens Door On Nudging Is Set
	0,		// 053 - default =      0	Emergency Dispatch Enable
	0,		// 054 - default =      0	Number of Cars Requested at Lobby
	0,		// 055 - default =      0	Next Up Selection
	0,		// 056 - default =      0	Code Blue Car Select
	0,		// 057 - default =      0	IR Car Select
	1,		// 058 - default =      1	Brake Lift Switch
	0,		// 059 - default =      0	Load Weighing Anti-nuisance Count
	0,		// 060 - default =      0	Fire Switch Location
	6,		// 061 - default =      6	ETA Minimum Time
	15,		// 062 - default =     15	ETA for Coincident Car Call
	1,		// 063 - default =      1	Video Position for Car 1
	2,		// 064 - default =      2	Video Position for Car 2
	3,		// 065 - default =      3	Video Position for Car 3
	4,		// 066 - default =      4	Video Position for Car 4
	5,		// 067 - default =      5	Video Position for Car 5
	6,		// 068 - default =      6	Video Position for Car 6
	0,		// 069 - default =      0	Run Count with No Passengers to Cancel Car Calls
	0,		// 070 - default =      0	Independent Override Security
	0,		// 071 - default =    0.0	Brake Pick Delay Time on Relevel
	0,		// 072 - default =      0	Brake Cool on Relevel
	600,	// 073 - default =   60.0	Up peak Trigger Time
	3,		// 074 - default =      3	Up Peak Trigger Count
	3,		// 075 - default =      3	Up Peak Car Call Count
	1800,	// 076 - default =  180.0	Up Peak Duration Time
	600,	// 077 - default =   60.0	Down Peak Trigger Time
	12,		// 078 - default =     12	Down Peak Trigger Count
	1800,	// 079 - default =  180.0	Down Peak Duration Time
	80,		// 080 - default =    8.0	Parking Delay Time
	0,		// 081 - default =      0	Hall Call Cross Assignment
	60,		// 082 - default =     60	Hall Call Cross Assignment ETA
	1,		// 083 - default =      1	Medical Emergency Floor
	0,		// 084 - default =      0	Medical Emergency Hall Switch location
	0,		// 085 - default =      0	Fire Service Motor Room Sensor Return Floor
	0,		// 086 - default =      0	Fire Service Hoistway Sensor Return Floor
	0,		// 087 - default =      0	Phase 1 Fire Recall Reset
	50,		// 088 - default =     50	Acceleration and Deceleration Limit at Final Stop
	0,      // 089 - default =   0.00	Pattern Delay During Relevel
	1,		// 090 - default =      1	Delay Start Until Drive On
	5500,   // 091 - default =   5500	Brake Opto Trigger Delay
	0,		// 092 - default =      0	Invert Logic of Brake Lift Switch
	0,		// 093 - default =      0	Invert Logic of In-service Light Output or In-use Light or Out of Service Alarm
	0,		// 094 - default =      0	Ability to Change a Car Other Than 1 or 2 to Become the Master
	0,		// 095 - default =    0.0	Time-out Video Display
	0,		// 096 - default =      0	Close Doors While Trying to Run on Inspection
	0,		// 097 - default =      0	Parking Floor 1
	0,		// 098 - default =      0	Parking Floor 2
	0,		// 099 - default =      0	Parking Floor 3
	0,		// 100 - default =      0	Parking Floor 4
	0,		// 101 - default =      0	Parking Floor 5
	0,		// 102 - default =      0	Parking Width
	400,	// 103 - default =   40.0	Percent of Load to Balance the Counterweights (Balanced Load Percent)
	0,		// 104 - default =   0.00	Drive Starting Torque Multiplier (Torque Amount Variable)
	60,		// 105 - default =     60	Load Bypass Percent
	20,		// 106 - default =     20	Load Anti-nuisance Percent
	40,		// 107 - default =     40	Load Dispatch Percent
	110,	// 108 - default =    110	Load Overload Percent
	0,		// 109 - default =      0	IR Control Flag
	230,	// 110 - default =    230	Brake Pick Voltage
	30,		// 111 - default =    3.0	Brake Pick Duration Time
	160,	// 112 - default =    160	Brake Hold Voltage
	230,	// 113 - default =    230	Brake Relevel Voltage
	240,	// 114 - default =    240	Brake AC Line to Line Voltage
	2830,	// 115 - default =  283.0	Brake Resistance
	600,  	// 116 - default =   60.0	Attendant Buzzer Delay Time
	0,		// 117 - default =      0	Hall Fire Aux. Switch Enable
	0,		// 118 - default =      0	Operation of Hall Fire Light
	0,		// 119 - default =      0	COP/Remote CC Select
	0,		// 120 - default =    0.0	Door Open or Close Delay Time
	0,		// 121 - default =      0	Security Recall Option
	0,		// 122 - default =      0	DOB Overrides Security
	0,		// 123 - default =    0.0	Manual Door Buzzer Delay Time
	1,		// 124 - default =      1	Security Floor
	0,		// 125 - default =    0.0	Retiring Cam Pick Delay Time
	10,		// 126 - default =    1.0	Short Door Time From a Reopen Signal
	0,		// 127 - default =      0	Second IR Car Select
	0,		// 128 - default =      0	Stop At Lobby
	1,		// 129 - default =      1	Invert Cab Light Fan Output
	0,		// 130 - default =      0	Invert Temperature Low Switch (TPL)
	0,		// 131 - default =      0	Invert Low Pressure Switch
	0,		// 132 - default =      0	Invert Low Oil Switch
	0,		// 133 - default =      0	Nudging with No Onward Call
	0,		// 134 - default =      0	Fire Option Flag
	0,		// 135 - default =      0	Attendant Car Call Flag
	600,	// 136 - default =   60.0	Code Blue Door Open Time
	0,		// 137 - default =      0	Request Ind Car For Code Blue
	0,		// 138 - default =      0	Code Blue Light Control (Flash Light)
	0,		// 139 - default =      0	Hospital Service Door Close on Car Call
	40,		// 140 - default =    4.0	Gripper Reset Time(r)
	0,		// 141 - default =      0	Independent Door Close on Car Call
	0,		// 142 - default =   0.00	Torque Offset
	3,		// 143 - default =      3	Gripper Test From Safety Processor Board
	0,		// 144 - default =      0	Single Automatic Pushbutton Control
	0,		// 145 - default =      0	Door Open When DOL Signal Not Active
	0,		// 146 - default =      0	Previously: fvdisglt - Disable Gate and Lock Test for New York City
	0,		// 147 - default =    0.0	Relevel Start Speed
	180,	// 148 - default =    180	Emergency Deceleration Rate
	0,		// 149 - default =      0	Disable DCB from Shortening Door Time
	0,		// 150 - default =      0	Return to Lobby Option
	7,		// 151 - default =      7	Video Position for Car 7
	8,		// 152 - default =      8	Video Position for Car 8
	0,		// 153 - default =      0	Parking Type
	0,		// 154 - default =      1	Parking Floor 6
	0,		// 155 - default =      1	Parking Floor 7
	0,		// 156 - default =      0	Non-Simultaneous Doors Operation
	0,		// 157 - default =      0	Pre-open Doors
	115,	// 158 - default =    115	Over Speed Percent
	0,		// 159 - default =      0	Position Pulse Count Update
	5000,	// 160 - default =   5000	Position Pulse Delay Error
	0,		// 161 - default =      0	Gripper Buzzer Enable
	10,		// 162 - default =    1.0	Relevel Delay Time
	1,		// 163 - default =      1	Fire Option 2
	0,		// 164 - default =      0	Close Gate on Swing Door with No Preference
	3,		// 165 - default =      3	Leveling Fault Count
	0,		// 166 - default =   0.00	Torque Amount in Down Direction
	0,		// 167 - default =   0.00	Ramp Torque Time During Pattern Delay
	400,    // 168 - default =   40.0	Electric Eye Time-out Time
	0,		// 169 - default =      0	Cars Out of Service Hall Call Cancel
	0,		// 170 - default =      0	Nudging Control
	200,	// 171 - default =   20.0	Fire Phase 1 Door Time-out (Ind and Att)
	0,		// 172 - default =      0	No Hall Call Button Door Reopen with Onward Call
	5,		// 173 - default =      5	Drive Ready Reset Count
	80,		// 174 - default =    8.0	Return to Lobby Door Dwell Time
	0,		// 175 - default =      0	Next Up Lobby Lantern Control
	0,		// 176 - default =      0	Next Up Preference Control
	0,		// 177 - default =      0	Second Riser Control
	0,		// 178 - default =      0	Second Riser Lantern Control
	0,		// 179 - default =      0	Attendant ETA Preference Time
	0,		// 180 - default =      0	Handicap Buzzer Disabled at Invalid Floor	 
	200,	// 181 - default =   20.0	VIP Door Open Time
	0,		// 182 - default =      0	VIP Operation Flags
	1,		// 183 - default =      1	Number of VIP cars
	0,		// 184 - default =      0	Emergency Power Op led (OP) Function
	0,		// 185 - default =      0	Emergency Power Park led (PRK) Function
	0,		// 186 - default =      0	Hall Elevator Off Options
	0,		// 187 - default =      0	Drive Baud Rate
	0,		// 188 - default =      0	Drive Update Rate
	1,		// 189 - default =      1	Alternate Lobby Floor
	1,		// 190 - default =      1	Alternate Parking Floor
	0,		// 191 - default =      0	Lobby Request Control
	40,		// 192 - default =     40	Handicap Car Capacity
	10,     // 193 - default =    1.0	Freight Door Close Time-out
	0,		// 194 - default =      0	Handicap Car Wait Time
	300,	// 195 - default =   30.0	Short Floor Distance
	0,		// 196 - default =      0	Retiring Cam Control
	0,		// 197 - default =      0	Recall from Fire Alternate Floor
	300,	// 198 - default =   30.0	Auto Learn Hoistway Stall Time
	100,	// 199 - default =   1.00	Position Pulse Count Error
	0,		// 200 - default =    0.0	Attendant Buzzer Time (When Off)
	50,		// 201 - default =    5.0	Attendant Buzzer Time (When On)
	0,		// 202 - default =    0.0	Brake Pick Voltage Rate Time
	0,		// 203 - default =    0.0	Brake Relevel Voltage Rate Time
	0,		// 204 - default =      0	Brake Pick Start Voltage
	0,		// 205 - default =      0	Brake Relevel Start Voltage
	0,		// 206 - default =    0.0	Short Floor Down Slowdown Time
	0,		// 207 - default =    0.0	Mid Short Floor Down Slowdown Time
	0,		// 208 - default =    0.0	Short Floor Up Slowdown Time
	0,		// 209 - default =    0.0	Mid Short Floor Up Slowdown Time
	0,		// 210 - default =      0	Hall Switch Elevator Off Return Floor
	0,		// 211 - default =      0	Service Message 1 display
	0,		// 212 - default =      0	Service Message 2 display
	0,		// 213 - default =      0	Governor Gripper Trip Control
	125,    // 214 - default =    125	Short Floor Soft Start Rate
	92,     // 215 - default =     92	Short Floor Acceleration Rate
	125,    // 216 - default =    125	Short Floor Roll Over Jerk Rate
	125,    // 217 - default =    125	Short Floor Deceleration Jerk Rate
	92,     // 218 - default =     92	Short Floor Deceleration Rate
	80,     // 219 - default =    8.0	Short Floor Target Distance
	0,		// 220 - default =      0	Short Floor Control
	0,		// 221 - default =      0	Security Recall Control 2
	150,	// 222 - default =   15.0	Emergency Power Recall Delay Time
	60,		// 223 - default =     60	Group Car Call Security Override Time
	0,		// 224 - default =    0.0	Emergency Brake Drop Delay
	30,		// 225 - default =    3.0	Emergency Brake Pick Time
   1000,	// 226 - default =  100.0	Hall Call Output On Brightness for led
	200,	// 227 - default =   20.0	Hall Call Output Off Brightness for led
	2,		// 228 - default =      2	Access Top Floor *Overridden by cons[topf] (Top Floor) 
	1,		// 229 - default =      1	Access Bottom Floor
	0,		// 230 - default =      0	Stop On Position Pulse
	80,		// 231 - default =    8.0	Sabbath Svc Restart Time
	10,		// 232 - default =    1.0	Power Door Open Relay On Time
	0,		// 233 - default =      0	Display Fault Automatically on LCD when Fault Occurs	
	0,		// 234 - default =      0	Password for Adjusting Field Variables
	3000, 	// 235 - default =  300.0	Password Time-out Time
	0,		// 236 - default =      0	Behind Car Call Cancel
	0,		// 237 - default =    0.0	Door Open Advance Door Enable Non-Simultaneous Doors
	0,		// 238 - default =      0	Group Service Number of Parking Cars
	0,		// 239 - default =      0	Binary Preset Update when Position Does Not Match
	0,		// 240 - default =      0	Short Floor hsf with No hsv High Speed Valve
	0,		// 241 - default =      0	Up Peak Control
	1,		// 242 - default =      1	Up Peak Pool
	0,		// 243 - default =      0	Down Peak Control
	0,		// 244 - default =      0	Down Peak Pool
	0,		// 245 - default =      0	Cross Assignment Cars from Old Group
	0,		// 246 - default =      0	Modbus Protocol
	0,		// 247 - default =      0	Fire Alternate Recall After FS Turns Off
	300,	// 248 - default =    300	Inspection Deceleration Stop Rate
	0,		// 249 - default =      0	Emergency Power Recovery Direction
	0,		// 250 - default =      0	Brake Drop Voltage 
	0,		// 251 - default =    0.0	Brake Voltage Drop Rate
	0,		// 252 - default =      0	Brake Drop Timing During Leveling Flag
	25,		// 253 - default =     25	Recovery Velocity
	100,	// 254 - default =    100	Emergency Deceleration to Leveling Rate
	25,		// 255 - default =     25	Emergency Power Recovery Velocity
	0,		// 256 - default =    0.0	Run Cycle Time
	625,	// 257 - default =   62.5	Hall Call Up On Red Intensity
	625,	// 258 - default =   62.5	Hall Call Up Off Red Intensity
    1000,	// 259 - default =  100.0	Hall Call Up On Green Intensity
	1000,	// 260 - default =  100.0	Hall Call Up Off Green Intensity
	500,	// 261 - default =   50.0	Hall Call Up On Blue Intensity
	500,	// 262 - default =   50.0	Hall Call Up Off Blue Intensity
	0,		// 263 - default =      0	Door Open Light Control on CCS board
	15,		// 264 - default =     15	Hall Call Button Up On Color
	15,		// 265 - default =     15	Hall Call Button Up Off Color
	15,		// 266 - default =     15	Hall Call Button Security Color
	0,		// 267 - default =      0	Hall Call Button Security Light
	1000,	// 268 - default =  100.0	Hall Call Security Brightness
	1000,	// 269 - default =  100.0	Hall Call Security Red Intensity
	0,		// 270 - default =    0.0	Hall Call Security Green Intensity
	1000,	// 271 - default =  100.0	Hall Call Security Blue Intensity
	0,		// 272 - default =      0	Fire Service Hoistway Sensor 2 Return Floor
	1000,	// 273 - default =  1.000	Drive Speed Multiplier
	0,		// 274 - default =    0.0	Advanced Pre-torque
	15,		// 275 - default =     15	Car Call Button On Color
	15,		// 276 - default =     15	Car Call Button Off Color
	1000,	// 277 - default =  100.0	Car Call Output On Brightness for led
	200, 	// 278 - default =   20.0	Car Call Output Off Brightness for led
	625, 	// 279 - default =   62.5	Car Call On Red Intensity
	625, 	// 280 - default =   62.5	Car Call Off Red Intensity
	1000,	// 281 - default =  100.0	Car Call On Green Intensity
	1000,	// 282 - default =  100.0	Car Call Off Green Intensity
	500, 	// 283 - default =   50.0	Car Call On Blue Intensity
	500, 	// 284 - default =   50.0	Car Call Off Blue Intensity
	0,		// 285 - default =      0	Car Call Button Light Control
	15,		// 286 - default =     15	Car Call Button Security Color
	1000,	// 287 - default =  100.0	Car Call Security Brightness
	1000, 	// 288 - default =  100.0	Car Call Security Red Intensity
	0,		// 289 - default =    0.0	Car Call Security Green Intensity
	1000,	// 290 - default =  100.0	Car Call Security Blue Intensity
	0,		// 291 - default =      0	Sabbath Enable Control Variable
	1980,	// 292 - default =  198.0	Low Line Voltage (RMS value)
	1980,	// 293 - default =  198.0	Low Door Voltage (RMS value)
	100, 	// 294 - default =    100	Emergency Power Top Speed
	125,    // 295 - default =    125	Emergency Power Soft Start Jerk Rate
	92,     // 296 - default =     92	Emergency Power Acceleration Rate
	125,    // 297 - default =    125	Emergency Power Roll Over Jerk Rate
	125,    // 298 - default =    125	Emergency Power Deceleration Jerk Rate
	92,     // 299 - default =     92	Emergency Power Deceleration Rate
	120,    // 300 - default =   12.0	Emergency Power Target Distance
	0,		// 301 - default =      0	Car Top Speed *Overridden by cons[speed] (Top Speed)
	0,		// 302 - default =      0	Inspection Velocity from ISER output
	200,	// 303 - default =   20.0	Emergency Power Recover to Floor Time
	1,		// 304 - default =      1	Fire Light Control during EP
	1,		// 305 - default =      1	Emergency Power Manual Select Enable
	0,		// 306 - default =      0	Fire Light Control with Car Out of Service (Low Pressure, eaq, Stall)
	100,	// 307 - default =   10.0	Sabbath Door Dwell Time
	50,		// 308 - default =    5.0	Sabbath Door Buzzer Time Prior to Doors Closing
	500,	// 309 - default =   50.0	Low Battery Capacity Level
	0,		// 310 - default =      0	Sabbath Disable Control Variable
	0,		// 311 - default =      0	Electric Eye Cancel Door Time
	0,		// 312 - default =      0	Emergency Power Car at Recall Floor with Door Open
	0,		// 313 - default =      0	Emergency Power Car Not at Recall Floor with Door Open
	0,		// 314 - default =      0	Assign Parking Floor to Car with Door Open
	0,		// 315 - default =    0.0	Hall Lantern Delay
	0,		// 316 - default =      0	Sabbath Enable Control 2 Variable
	0,		// 317 - default =      0	Cab Lantern Control
	0,		// 318 - default =    0.0	Max Door Hold Time from ED Input
	0,		// 319 - default =      0	Access Run Door Close
	0,		// 320 - default =      0	Car Call Button Attendant Up Color
	1000,	// 321 - default =  100.0	Car Call Attendant Up Brightness
	0,		// 322 - default =    0.0	Car Call Attendant Up Red Intensity
	1000,	// 323 - default =  100.0	Car Call Attendant Up Green Intensity
	0,		// 324 - default =    0.0	Car Call Attendant Up Blue Intensity
	0,		// 325 - default =      0	Car Call Button Attendant Dn Color
	1000,	// 326 - default =  100.0	Car Call Attendant Dn Brightness
	1000,	// 327 - default =  100.0	Car Call Attendant Dn Red Intensity
	0,		// 328 - default =    0.0	Car Call Attendant Dn Green Intensity
	0,		// 329 - default =    0.0	Car Call Attendant Dn Blue Intensity
	0,		// 330 - default =      0	Fire light Color
	1000,	// 331 - default =  100.0	Fire light Brightness
	625,	// 332 - default =   62.5	Fire light Red Intensity
	1000,	// 333 - default =  100.0	Fire light Green Intensity
	500,	// 334 - default =   50.0	Fire light Blue Intensity
	0,		// 335 - default =      0	Medical light Color
	1000,	// 336 - default =  100.0	Medical light Brightness
	0,		// 337 - default =    0.0	Medical light Red Intensity
	0,		// 338 - default =    0.0	Medical light Green Intensity
	1000,	// 339 - default =  100.0	Medical light Blue Intensity
	0,		// 340 - default =      0	Emergency light Color
	1000,	// 341 - default =  100.0	Emergency light Brightness
	750,	// 342 - default =   75.0	Emergency light Red Intensity
	1000,	// 343 - default =  100.0	Emergency light Green Intensity
	0,		// 344 - default =    0.0	Emergency light Blue Intensity
	0,		// 345 - default =      0	OTS light Color
	1000,	// 346 - default =  100.0	OTS light Brightness
	1000,	// 347 - default =  100.0	OTS light Red Intensity
	400,	// 348 - default =   40.0	OTS light Green Intensity
	0,		// 349 - default =    0.0	OTS light Blue Intensity
	0,		// 350 - default =      0	Backlight Output lights
	4,		// 351 - default =      4	Can Sync Count
	15,		// 352 - default =     15	Hall Call Dn On Color
	1000,	// 353 - default =  100.0	Hall Call Dn On Brightness
	625,	// 354 - default =   62.5	Hall Call Dn On Red Intensity
	1000,	// 355 - default =  100.0	Hall Call Dn On Green Intensity
	500,	// 356 - default =   50.0	Hall Call Dn On Blue Intensity
	15,		// 357 - default =     15	Hall Call Dn Off Color
	200,	// 358 - default =   20.0	Hall Call Dn Off Brightness
	625,	// 359 - default =   62.5	Hall Call Dn Off Red Intensity
	1000,	// 360 - default =  100.0	Hall Call Dn Off Green Intensity
	500,	// 361 - default =   50.0	Hall Call Dn Off Blue Intensity
	0,		// 362 - default =      0	Hall Call CB On Color
	1000,	// 363 - default =  100.0	Hall Call CB On Brightness
	0,		// 364 - default =    0.0	Hall Call CB On Red Intensity
	0,		// 365 - default =    0.0	Hall Call CB On Green Intensity
	1000,	// 366 - default =  100.0	Hall Call CB On Blue Intensity
	0,		// 367 - default =      0	Hall Call VIP On Color
	1000,	// 368 - default =  100.0	Hall Call VIP On Brightness
	1000,	// 369 - default =  100.0	Hall Call VIP On Red Intensity
	400,	// 370 - default =   40.0	Hall Call VIP On Green Intensity
	0,		// 371 - default =    0.0	Hall Call VIP On Blue Intensity
	15,		// 372 - default =     15	Hall Call IR Up On Color
	1000,	// 373 - default =  100.0	Hall Call IR Up On Brightness
	625,	// 374 - default =   62.5	Hall Call IR Up On Red Intensity
	1000,	// 375 - default =  100.0	Hall Call IR Up On Green Intensity
	500,	// 376 - default =   50.0	Hall Call IR Up On Blue Intensity
	15,		// 377 - default =     15	Hall Call IR Dn On Color
	200,	// 378 - default =   20.0	Hall Call IR Dn On Brightness
	625,	// 379 - default =   62.5	Hall Call IR Dn On Red Intensity
	1000,	// 380 - default =  100.0	Hall Call IR Dn On Green Intensity
	500,	// 381 - default =   50.0	Hall Call IR Dn On Blue Intensity
	15,		// 382 - default =     15	Hall Call IR Up Off Color
	1000,	// 383 - default =  100.0	Hall Call IR Up Off Brightness
	625,	// 384 - default =   62.5	Hall Call IR Up Off Red Intensity
	1000,	// 385 - default =  100.0	Hall Call IR Up Off Green Intensity
	500,	// 386 - default =   50.0	Hall Call IR Up Off Blue Intensity
	15,		// 387 - default =     15	Hall Call IR Dn Off Color
	200,	// 388 - default =   20.0	Hall Call IR Dn Off Brightness
	625,	// 389 - default =   62.5	Hall Call IR Dn Off Red Intensity
	1000,	// 390 - default =  100.0	Hall Call IR Dn Off Green Intensity
	500,	// 391 - default =   50.0	Hall Call IR Dn Off Blue Intensity
   1000,	// 392 - default =  100.0	Hall Call Output Up On brightness for led
	200,	// 393 - default =   20.0	Hall Call Output Up Off brightness for led
	0,		// 394 - default =      0	Code Blue Location
	0,		// 395 - default =      0	VIP Location
	0,		// 396 - default =      0	IR light Color Control
	0,		// 397 - default =      0	Hall Call CB Off Color
	200,	// 398 - default =   20.0	Hall Call CB Off Brightness
	0,		// 399 - default =    0.0	Hall Call CB Off Red Intensity
	0,		// 400 - default =    0.0	Hall Call CB Off Green Intensity
	1000,	// 401 - default =  100.0	Hall Call CB Off Blue Intensity
	0,		// 402 - default =      0	Hall Call VIP Off Color
	200,	// 403 - default =   20.0	Hall Call VIP Off Brightness
	1000,	// 404 - default =  100.0	Hall Call VIP Off Red Intensity
	400,	// 405 - default =   40.0	Hall Call VIP Off Green Intensity
	0,		// 406 - default =    0.0	Hall Call VIP Off Blue Intensity
	0,		// 407 - default =      0	VIP Lantern Control
	0,		// 408 - default =      0	Group Car Call Override Control
	0,		// 409 - default =      0	Exclusion Fault 1
	0,		// 410 - default =      0	Exclusion Fault 2         
	0,		// 411 - default =      0	Exclusion Fault 3
	0,		// 412 - default =      0	Exclusion Fault 4        
	0,		// 413 - default =      0	Exclusion Fault 5
	0,		// 414 - default =      0	Exclusion Fault 6        
	230,	// 415 - default =    230	Emergency Brake Pick Voltage
	160,	// 416 - default =    160	Emergency Brake Hold Voltage
	0,		// 417 - default =    0.0	Emergency Brake Pick Rate
	0,		// 418 - default =    0.0	Emergency Brake Drop Rate
	240,	// 419 - default =    240	Emergency Brake AC Line to Line Voltage
	5500,   // 420 - default =   5500	Brake Opto Trigger Delay
	0,		// 421 - default =      0	Can Baud Rate
	0,		// 422 - default =      0	Car Call Arrival Lantern
	0,		// 423 - default =      0	Code Blue Second Riser Car Select
	0,		// 424 - default =      0	Code Blue over IR car
	10,		// 425 - default =     10	IR Car Code Blue Penalty Time
	0,		// 426 - default =      0	INSEC (In Security Output Invert)
	0,		// 427 - default =      0	Attendant Manual Direction Enable
	3,		// 428 - default =      3	Encoder Debounce Interval
	10,		// 429 - default =     10	Encoder Number of Samples
	2,		// 430 - default =    0.2	Handicap Buzzer on Car Call Beep Time
	0,		// 431 - default =      0	Service UPS Mode
	6,		// 432 - default =      6	Serial Port Comm Diag
	3,		// 433 - default =      3	User Port 2 Baud Rate
	0,		// 434 - default =      0	Cpu Timing Output Control
	0,		// 435 - default =      0	Service Message 3 Display
	0,		// 436 - default =      0	Hall Call Assignment Secure Type
	2,		// 437 - default =      2	UPS Baud Rate
	0,		// 438 - default =      0	HC Acknowledge Attendant Buzzer
	0,		// 439 - default =      0	Second Code Blue Car Select
	0,		// 440 - default =      0	Hall Elevator Off Options 2 
	0,		// 441 - default =      0	Code Blue Buzzer Control
	0,		// 442 - default =      0	Code Blue Override Independent
	0,		// 443 - default =      0	Code Blue Recall Any Car If CB Cars 1 And 2 Are Not Available
	0,		// 444 - default =      0	Hall Elevator Off Override
	0,		// 445 - default =      0	NYC FS2 Door Reopen on STP
	10,		// 446 - default =    1.0	Power Loss Brake On Time
	0,		// 447 - default =    0.0	Power Loss Brake Off Time
	0,		// 448 - default =      0	Select and Prioritize the ATT car to Run on Emergency Power Service
	0,		// 449 - default =      0	Encoder Direction for Open Can Encoder
	0,		// 450 - default =      0	Emergency Cars Finished Recalling Output enable (EPCOM)
	0,		// 451 - default =      0	Out of Service Light control
	1,		// 452 - default =      1	Extra Door Time Location 
	0,		// 453 - default =      0	Out of Service Light Control
	6000,	// 454 - default =  600.0	Time for OSERL Not Answering Call
	600,	// 455 - default =   60.0	Time for OSERL car Between Floors
	600,	// 456 - default =   60.0	Time for OSERL Safety String Open
	0,		// 457 - default =      0	Fire Service Hoistway Sensor Location
	5,		// 458 - default =    0.5	Retiring Cam Drop Fail Safe Delay
	150,	// 459 - default =    150	Velocity Difference 
	0,		// 460 - default =      0	Velocity Difference Fault Disable
	0,		// 461 - default =      0	SPB Direction Fault Disable
	0,		// 462 - default =      0	Limit Direction Fault Disable
	0,		// 463 - default =      0	Encoder Direction Fault Disable
	0,		// 464 - default =      0	UL/DL Direction Fault Disable
	0,		// 465 - default =    0.0	Relevel Start Velocity Hold Time
	0,		// 466 - default =      0	Med Door Open Sequence for Re-open
	0,		// 467 - default =      0	Med Ind Override
	0,		// 468 - Default =      0	Code Blue Bypass Disable
	0,		// 469 - Default =      0	Extended Door PI Message
	2,		// 470 - default =      2	First Return Car on Emergency Power for Feeder 2
	2,		// 471 - default =      2	First Car Selected on Emergency Power for Feeder 2
	0,		// 472 - Default =      0	Retiring Cam for Freight Output Enable
	0,		// 473 - default =    0.0	Lantern Preference Change Delay
	150,	// 474 - default =    150	Slip Velocity Difference Fault Trip Value
	0,		// 475 - default =      0	DOB Overrides DCB on Phase 2
	0,		// 476 - default =      0	Denver Fire Service Amendment to Close the Phase 1 Doors After Recall and Reopen From a Hall Call
	600,	// 477 - default =   60.0	Denver FS Phase 1 Dwell Time
	0,		// 478 - default =      0	Hall Call Security Control
	0,		// 479 - default =      0	Slip Velocity Difference Disable
	0,		// 480 - default =      0	Leveling Fault Trip
	0,		// 481 - default =      0	Emergency Brake Switch Control
	1,		// 482 - default =      1	Attendant Buzzer Control
	6,		// 483 - default =      6	Radial Position Indicator
	0,		// 484 - default =      0	Force Car to Recall On Independent Service to the Lobby When No Demand Is Present
	200,	// 485 - default =   20.0	Time-out to Recall On Independent Service to the Lobby When No Demand Is Present
	5000,	// 486 - default =  500.0	Motor RPM
	0,		// 487 - default =      0	VIP Multiple Calls Enable
	0,		// 488 - default =      0	Service Output Control
	0,		// 489 - default =      0	Can Encoder Baud Rate
	63,		// 490 - default =     63	Can Encoder Node ID
	0,		// 491 - default =      0	Code Blue Allows Single Free Car Call Enable
	0,		// 492 - default =      0	Sabbath Collective Mode
	0,		// 493 - default =      0	Return to Lobby Door Open Type
	0,		// 494 - default =      0	Ring the Lantern in IR All the Time
	0,		// 495 - default =      0	High Priority Floor Number
	600,	// 496 - default =   60.0	High Priority Floor Time
	0,		// 497 - default =      0	Emergency Manual Select Switch Recall Operation
	35,		// 498 - default =     35	Access Speed
	0,		// 499 - default =      0	Auto Service Time-out Enable
	120,	// 500 - default =    120	Auto Service Time-out Time
	0,		// 501 - default =      0	Car Call Lockouts on Sabbath Operation
	0,		// 502 - default =      0	Medical Service Override Car Call Security
	0,		// 503 - default =    0.0	HEOF Auto Reset Time
	0,		// 504 - default =      0	CCPBS on Group Car Call Security
	3,		// 505 - default =      3	Dead Zone Selection
	240,	// 506 - default =   24.0	Access UT Distance
	240,	// 507 - default =   24.0	Access DT Distance
	0,		// 508 - default =      0	Serial Port Comm Diag 2
	0,		// 509 - default =      0	Third Riser Control
	0,		// 510 - default =      0	Baud Rate for 485 Port
	0,		// 511 - default =      0	Encoder Type
	0,		// 512 - default =      0	Keb Drive Display Type
	0,		// 513 - default =      0	Brake Phase Input
	0,		// 514 - default =      0	Emergency Brake Phase Input
	0,		// 515 - default = 00.000	Distance Feed Forward
	0,		// 516 - default =      0	Second Riser Car Calls Security by Direction
	0,		// 517 - default =      0	Minimum Short Door Time Enable
	0,		// 518 - default =      0	Fire Recall Reset 2
	0,		// 519 - default =      0	Invert Hall Call Security Input
	0,		// 520 - default =      0	DZ Gripper Trip Control
	0,		// 521 - default =    0.0	Security Disable Time
	3,		// 522 - default =      3	DL Dead Zone Selection
	2,		// 523 - default =    0.2	EE Test Time
	0,		// 524 - default =      0	Reassign Car Calls When on Multiple Floor Mask Security (cons[SecFlCfg] = 2)
	0,		// 525 - default =      0	Disable Opposite Hall Call
	10,		// 526 - default =     10	Disable Opposite Hall Call Time
	0,		// 527 - default =      0	Hall Elevator Off Options 3
	0,		// 528 - default =      0	Car Elevator Off Options
	0,		// 529 - default =      0	Car Elevator Off Options 2
	0,		// 530 - default =      0	Code Blue Over Fire Service
	0,		// 531 - default =      0	COP/Remote Disable
	0,		// 532 - default =      0	Fire Service 2 with Special Devices (HYDRO ONLY)
	10,		// 533 - default =    1.0	Auto Swing Door Open Delay
	5,		// 534 - default =      5	CL Pulse Time
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 539
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 544
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 549
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 554
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 559
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 564
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 569
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 574
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 579
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 584
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 589
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 594
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused
	0,		// unused	 599
};

int16 tout_A, tout_B, tout_C;
int16 DIO1_in;
int16 DIO1_in_dbn;
int16 DIO2_out;

const uint32 fl_mask[65] =   	// Note: this table starts at floor 0
{0,0x00000001L,0x00000002L,0x00000004L,0x00000008L,0x00000010L,0x00000020L,0x00000040L,0x00000080L,
   0x00000100L,0x00000200L,0x00000400L,0x00000800L,0x00001000L,0x00002000L,0x00004000L,0x00008000L,
   0x00010000L,0x00020000L,0x00040000L,0x00080000L,0x00100000L,0x00200000L,0x00400000L,0x00800000L,
   0x01000000L,0x02000000L,0x04000000L,0x08000000L,0x10000000L,0x20000000L,0x40000000L,0x80000000L,
   0x00000001L,0x00000002L,0x00000004L,0x00000008L,0x00000010L,0x00000020L,0x00000040L,0x00000080L,
   0x00000100L,0x00000200L,0x00000400L,0x00000800L,0x00001000L,0x00002000L,0x00004000L,0x00008000L,
   0x00010000L,0x00020000L,0x00040000L,0x00080000L,0x00100000L,0x00200000L,0x00400000L,0x00800000L,
   0x01000000L,0x02000000L,0x04000000L,0x08000000L,0x10000000L,0x20000000L,0x40000000L,0x80000000L,
   };

void init_car_size_variables (void);
void init_control(void);
void control(void);


// Prototypes of all the functions control.cc  
void guilbert_doors();
void diag_led_on(void);
void diag_led_off(void);
void atfloor(void );
int16 car_gate(void);
int16 chk_start(int16 cmd);
int16 chk_lev_start(int16 cmd);
void chkrun(void );
void clrall(void );
void clr_dir_arrow (void);
void clrlant(void );
void cycle_run(void);
void diag(void);
void dirsel(void );
void elevator_off (void);
void jail_break(void);
int16 ep_recall_ok(void);
void epr_dir(void);
void epr_set_power(void);
void dog_ints_off(void);
void dog_ints_on(void);
int16 door_locks(void);
int16 get_valid_fl(int16 pos, int16 dir);
void hlants(void );
void set_hall_lant (void);
void hydro_control (void);
void hot_oil(void);
void zoneelev(void);
void status_control (int16 start_ck);
void output_control (int16 start_ck);
void Car_Not_Dispatcher (void);
void comm_control (void);
void door_control (int16 start_ck);
void light_buzzer_control (int16 start_ck);
void set_att_annunciator(void);
void chk_motion_fault (int16 start_ck);
void security_control (int16 start_ck);
void hoistway_control (int16 start_ck);
void Set_Fault_Bits (void);
void deactivate_sabbath(void);
void inctime(int16 start_ck );
void new_inctime();
int16 ind (void);
void inttim(void);
void int_tsec(void);
void inittim(void );
void initial_stop (void);
void init_ram_data(void);
void key_ots(void );
void stall(void );
void low_pressure(void);
int16 main(int16 _argc, char *_argv[] );
int16 man_dcc(void);
int16 man_fdcc(void);
int16 man_rdcc(void);
void motion(void );
//lint -esym(534,needstop)
int16 needstop(void );
void pisout( int16);
void record_fault (uint16 code);
extern void rdfvar(void );
void reset(void );
void close_doors_during_reset(int16 force_close);
void set_door_close_during_reset(void);
void reset_jack (void);
void return_to_lobby(void);
int16 safe(void);
void set_safe_fault(void);
int16 access_door_safe(void);
void sstring(void );
int16 safeties (void);
void set_ss_fault (void);
void setfsf(void );
void seteqf(void);
void setempf(void);
void sethugsf(void);
void settugf(void); 
void setmedf(void);
void setcodebf (void);
void sethsvf(void);
void setvipf (void);
void smoke_sensor_interface(void);
void set_ul_dl(void);
void set_ul_dl_dz(void);
void setpref(void );
void set_position_from_dpp(void);
void set_dir_arrow (void);

void schedule_st_times(void);

void set_rcm(void);
void set_scnd_position(void);

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;
						// unsigned integer union
static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu2;					// unsigned integer union

void init_car_size_variables (void)
{
	int16 i;
	
	for (i=0;i<car_size;i++)
	{
		Security_Type[i] = 0;
		Drive_Type[i] = 0; 
		valid_cb_car[i] = 0;
		valid_cbpr_car[i] = 0;
		valid_cbsr_car[i] = 0;
		valid_ep_car[i] = 0;
		valid_vip_car[i] = 0;
		g_cb_fl[i] = 0;
		g_rcb_fl[i] = 0;
		grp_codebf[i] = 0;
		grp_empf[i] = 0;
		man_sel_req[i] = 0;			
	    car_recall_order[i] = 0;	  
	    car_select_order[i] = 0;	  
	    car_recall_order_fixed[i] = 0;
	    car_select_order_fixed[i] = 0;
	    lby_up_req_car[i] = 0;
	    ncu_door[i] = 0;
		ncu_que[i] = 0;
		lby_req[i] = 0;
	    g_park[i] = 0;
		up_trig_mem [i] = 0;
		sr_cc_ctrl[i] = 0;
		Car_on_SR[i] = 0;
		Valid_SR_Car[i] = 0;
		Valid_SR_Car_Backup[i] = 0; 
		tr_cc_ctrl[i] = 0;
		Car_on_TR[i] = 0;
		Valid_TR_Car[i] = 0;
		g_vip_fl[i] = 0;
		g_rvip_fl[i] = 0;
	    hp_car_sel[i] = 0;
	    ui_cc_sec[0][i] = 0;
	    ui_cc_sec[1][i] = 0;
	    ui_cc_sec_prev[0][i] = 0; 
	    ui_cc_sec_prev[1][i] = 0; 
	    ui_rcc_sec[0][i] = 0;
	    ui_rcc_sec[1][i] = 0;
	    ui_rcc_sec_prev[0][i] = 0;
	    ui_rcc_sec_prev[1][i] = 0;
	    ui_uhc_carsec[0][i] = 0;
	    ui_uhc_carsec[1][i] = 0;
	    ui_dhc_carsec[0][i] = 0;
	    ui_dhc_carsec[1][i] = 0;
	    ui_ruhc_carsec[0][i] = 0;
	    ui_ruhc_carsec[1][i] = 0;
	    ui_rdhc_carsec[0][i] = 0;
	    ui_rdhc_carsec[1][i] = 0;
	    ui_ind[i] = 0;
	    ui_rtl[i] = 0;
	    ui_heof[i] = 0;
	    ui_ad_off[i] = 0;
	    rear_op[i] = 0;
	    top_fl[i] = 0;
	    top_rfl[i] = 0;
	    grcan_car_online[i] = 0;
	    Car_Not_Talking[i] = 0;
	    full_update_req[i] = 0;
	}
}
 
void init_control(void)
{
	int16 i;
	
	disable_dog();
	delay(1000);			// delay to allow the MRAM to stabalize (must be at least 400 usec)
 	init_rtc_mram();
	
  	local_getdate();

	if (d.year > 3000)
	{
		d.year = 3000;
		local_setdate();
	}

  	day_of_week = calc_dayofweek();

	if ((day_of_week != d.dayofweek) || (d.dayofweek > 6))
		local_setdate();	// update day of the week  (calc_dayofweek() is called in local_setdate())
  	day_of_week = d.dayofweek;
  	old_day_of_week = day_of_week;

 	init_sdspi();		// Init hardware for SD Card

  	zero_req_que();
  	zero_status_que();
  	
  	init_car_size_variables();

  	init_dpyspi();
 	
 //	petdog();

 	init_LCD();

	enable_dog();
  	position = 2;
//  	petdog();
  	rdcons();     // Initialize all of the constants that define the job
  	position = (cons[topf] - 1);
//  	petdog();
#if (Simulator == 1)
  	rdhoistway();
#endif
//  	petdog();
// at this point, comm_baud[0] is still 354
  	rdfvar();     // Initialize all of the field adjustable varriables
  	
	enable_dog();
  	petdog();
  	rdflts();     // Initialize the fault log
  	petdog();
  	init_LT_flts_ix();
  	petdog();
	test_hardware();	// set error and hardware_init_fault flag
  	petdog();
	init_iospi();
	petdog();
	init_call_data();
  	petdog();
	init_MRAM_Traffic_Log();
	petdog();
	init_wifi_defaults();
  	petdog();
//	Init_Encoder();
	sel_init_can_bus(0);			// Initialize can bus encoder	

  	Init_Hoistway(); // initializing the hoist way digital pulse tables

  	petdog();

#if(Security == 1)
  	Security_Init();    // initializing security data
  	petdog();
#endif
  	if(cons[rear] == 0)
  	{
		  for(i=1; i<= cons[topf]; i++)
		  {
			  rcarcb[i] = 0;
			  rdowncb[i] = 0;
			  rupcb[i] = 0;
		  }
  	}
  	petdog();

	if (cons[cc_pb_sec] == 1)
		Init_Code_Check_Table();

  	petdog();

	init_ram_data();

 	disable_dog();
 	io_init();
	enable_dog();
 	petdog();

	if (Drive_Type[cons[carnmb]] == 9)
		setoutp(o_STE);
		

 	petdog();
 	init_mrcan_bus();
 	petdog();
 	init_ctcan_bus();
 	petdog();
 	
// ZYM 12-1-2017 work as cedes head 	
	init_grcan_bus();
//-	ced_init_can_bus(2);
	
 	petdog();
 	petdog();
 // next try	
	initcom();	// must be after init_can_bus() since the can bus chip provides the clock
  	 	
     	
  	petdog();

	if ((cons[ids] & 1) != 0)
		init_ids();
 

  	petdog();

	sx_initcom();


  	petdog();

#if (Simulator == 1)
  	sim_init(1);
#endif


  	petdog();

	record_fault(f_pwrup_reset);   // Always set a power up reset on power up

	petdog();
	if (cons[modem] == 1)
	{
		Init_Modem(USR1COM);
		disable_dog();
		delay(500);
		Modem_Quiet_Mode(USR1COM);
		enable_dog();

		Init_Modem(USR2COM);
		disable_dog();
		delay(500);
		Modem_Quiet_Mode(USR2COM);
		enable_dog();

	}

	grp_clrhc();

	motion_fault = 0;
	SD_Count = 0;
	reset_courion = 1;
	power_up_start_time = (cons[carnmb] - 1) * 20;		// add 2 seconds for each car
	pwr_up_reset = 1;
	pwr_up_firehold_rst = 1;
	timers[tcen] = 0;
					
	// Check if on FS PHASE 2 No auto door operation after power reset 
	if (firef >= 4)
		fs2_no_fdoor_op = 1;
	if (rfiref >= 4)
		fs2_no_rdoor_op = 1;

 	inittim();    // Zero timers - hardware setup in main

 	
 	
#if (Traction == 1)
 	if (trace_auto_start == 1)
 	{
		init_debug_trace();
 		start_debug_trace();
 	}
#endif

 	pwr_up_init = 1;
 	
//ZYM 10/16/2017
	CAN_Init(); 	
}


//*************************************************************************************
//*************************************************************************************
// Main Control Loop
//*************************************************************************************
//*************************************************************************************


void control(void)
{
  	int16 x=0;
 /* 	// tagwbg 01
	if (fvars[fvcomdiag1] == 1)
	{
			
	  	sendc(USR1COM,'G');
		sendc(USR1COM,'A');
		sendc(USR1COM,'L');
		sendc(USR1COM,'a');
		sendc(USR1COM,'x');
		sendc(USR1COM,'y');
		sendc(USR1COM,' ');
		sendc(USR1COM,'C');
		sendc(USR1COM,'o');
		sendc(USR1COM,'n');
		sendc(USR1COM,'t');
		sendc(USR1COM,'r');
		sendc(USR1COM,'o');
		sendc(USR1COM,'l');
		sendc(USR1COM,'s');
		sendc(USR1COM,CR);
		sendc(USR1COM,LF);
		sendc(USR1COM,'>');
	}
*/	
	
//	clrall();  // tagwbg 01
	
	x = 0;
  	while(x == 0)   
  	{
  	
//		inctime(0); // tagwbg 01
        x = 1; // tagwbg 02 only go through loop once
		
		switch(procf)
		{
			case 0:
				servf = s_RESET;
				procf = 1;		 
			  	timers[treset] = 0;
			//lint -fallthrough
		  	case 1:
				servf = s_RESET;
				reset();
				break;
  	    	case 2:
				servf = s_INSP;
				inspection();
				break;
  	    	case 3:
 				motion();
				break;
  	    	case 4:
				motion();
				break;
  	    	case 5:
				motion();
				break;
  	    	case 6:
				motion();
				break;
  	    	case 7:
				motion();
				break;
  	    	case 8:
				motion();
				break;
  	    	case 9:
				motion();
				break;
  	    	case 10:
				motion();
				break;
  	    	case 11:
				motion();
				break;
  	    	case 12:
				servf = s_SSTRING;
				sstring();
				break;
  	    	case 13:
				servf = 0;
				key_ots();
				break;
  	    	case 14:
				atfloor();
				break;
  	    	case 15:
				atfloor();
				break;
  	    	case 16:
				atfloor();
				break;
  	    	case 17:
				servf = s_STALLED;
				stall();
				break;
	  		case 18:
				servf = s_RST_JACK;		// reset hydro jack
				reset_jack();
				break;
			case 19:
				servf = s_LOW_PRES;
				low_pressure();
				break;

			case 20:
#if (Traction == 1)
				servf = s_HWLEARN;
				hoistway();
#else
				procf = 0;
#endif
				break;

			case 21:		
				procf = 0;
				break;

 			case 22:
				if(cons[hotoilSW] != 0)
				{
					servf = s_HOT_OIL;
					hot_oil();
				}
				else
				{
					procf = 0;
				}
				break;

			case 23:		
				procf = 0;
				break;

	    	default:
				procf = 0;      // Go to reset
				break;
  	  	}
  	}
}


// *******************************
// This is at a floor procedure
// *******************************
void atfloor()
{

	int16 tmppos=0;
	static int16 prev_pref;

	prev_pref = dpref;
	timers[tlantchange] = 10;

  	if((rset != 0)  || (servf == s_RESET_UP) || (servf == s_RESET_DN))
	{
		if (firef == 0)
			servf = s_AUTO;
	}
	handicap_buz = 0;
	if (dpref == 0)
		hb_en = 0;
    else if((dpref == 1) && (dirf != 1))
		hb_en = 0;
    else if((dpref == 2) && (dirf != 2))
		hb_en = 0;

	// Clear door close outputs	first time through
	clroutp(o_DC);
	if (cons[rear] != 0)
		clroutp(o_DCR);

	rset = 0;
	startf = 0;
	up_peak_trigger = 0;
	limit_fault = 0;
  	redundancy_test = 0;
  	in_redundancy_test = 0;
	timers[tsufsdfdly] = 0;
	timers[tuldloff] = 0;
	statusf &= ~sf_BPI;
	pwr_up_reset = 0;
	pwr_up_firehold_rst = 0;
	lev_startf = 0;
	
	if (fault_run == 1)
	{
		if (fault_run_cnt > 3)
		{	
			record_fault(f_shutdown);
			shutdown = 1;
		}
	}
	else
		fault_run_cnt = 0;
	
	if (dol_fail == 1)
	{
		dol_fail_cnt++;
		dol_fail = 0;
	}
	
	if (rdol_fail == 1)
	{
		rdol_fail_cnt++;
		rdol_fail = 0;
	}
	
	timers[tnit] = 0;
	timers[tdoorrq] = 0;

	timers[tlimit] = 0;
	ut_limit = rdinp(i_UTsel);
	dt_limit = rdinp(i_DTsel);
	stp_sw = rdinp(i_MRSW);
	lev_startf = 0;

	leveling = 0;
	level_stop_fault = 0;
	relev_stop_count = 0;

	run_time = timers[tmotion];
	trigger_arm_state |= INIT_ATFLOOR;

  	while((procf >= 14) && (procf <= 16))
  	{	
  	
		estop = 0;
	    estops_1_run = 0;

		// Keep the Soft starter motor contactor on except when testing
		if ((Drive_Type[cons[carnmb]] == 9) && (in_redundancy_test == 0))
			setoutp(o_STE);
		
		// incrementing the timers
    	inctime(0);

		set_position_from_dpp();
	
		write_trace(TR_ATFLOOR1);
		
		if ((rdinp(i_UTsel) != ut_limit) || (rdinp(i_DTsel) != dt_limit) || (rdinp(i_MRSW) != stp_sw))
			timers[tlimit] = 0;
		stp_sw = rdinp(i_MRSW);
		ut_limit = rdinp(i_UTsel);
		dt_limit = rdinp(i_DTsel);

		
		if((up_level == 1) && (dn_level == 1) && (reset_nonit == 1))    // dead level
   		{
			reset_nonit = 0;
			timers[tnit] = 0;
		}

		if (cons[Low_Press] == 1)
		{
			if (((rdinp(i_LPS) == 1) && 
				(fvars[fvlps] == 0)) || ((rdinp(i_LPS) == 0) && (fvars[fvlps] == 1)))
			{
				procf = 19;
				return;
			}
		}
		if ((cons[Low_Oil] == 1) && (stallf == 0) &&
			(((rdinp(i_LOS) == 1) && (fvars[fvlos] == 0)) || 
			((rdinp(i_LOS) == 0) && (fvars[fvlos] == 1))))
		{
		 	record_fault(f_lowoil_switch);
			stallf = 1;
		}
		if(((cons[hotoilSW] == 1) && (rdinp(i_TPH) == 1) && (hotoilf == 0)) ||
		   ((cons[hotoilSW] == 2) && (rdinp(i_TPH) == 0) && (hotoilf == 0) && (rdinp(i_CEN) == 1)))
		{
			if((timers[ttphdbnc]>5))
			{
		 		record_fault(f_hotoil);
				hotoilf = 1;
			}
		}
		else
		{
			timers[ttphdbnc] = 0;
		}

		// Testing to see if saftey string is open
    	if (safeties() == 1)
			timers[tsst] = 0;
    	else if (timers[tsst] > 3)
    	{
			if ((statusf & sf_SS) == 0)
				record_fault(f_sstring);
			statusf |= sf_SS;
      		procf = 12;       // go to saftey string open mode
      		return;
    	}
		else
			statusf &= ~sf_SS;

		// Testing to see if LC fuse is blown
    	if(rdinp(i_S10) == 1)
			timers[tlct] = 0;
    	if((rdinp(i_S10) == 0) && (timers[tlct] > 10))
    	{
			if ((statusf & sf_S10) == 0)
				record_fault(f_S10_fuse);
			statusf |= sf_S10;
			procf = 12;
			return;
    	}
		else
			statusf &= ~sf_S10;


		// Testing to see if on inspection
    	if ((chk_ins() == 1) && (safeties() == 1))
    	{
			if (timers[tinst] > 2)
			{
      			procf = 2;        // go to inspection mode
      			return;
			}
    	}
		else
			timers[tinst] = 0;

		// Testing to see if at a floor
    	if((up_level == 0) && (dn_level == 0))
    	{
			if (timers[tuldloff] > 5)
			{			// off for more than 500 msec
				if (door_zone == 1)
				{
					if (UL_DL_OFF_Fault == 0)
						record_fault(f_ul_dl_off);

				    UL_DL_OFF_Fault = 1;	
				}
				else
	      		{
					statusf |= sf_ULDLDZ;
					record_fault(f_ul_dl_dz_off);
	      			procf = 0;        // reset elevator no door zone
	   	  			return;
				}
			}
    	}
		else
		{
		    UL_DL_OFF_Fault = 0;	
			timers[tuldloff] = 0;
			statusf &= ~sf_ULDLDZ;
		}

		// Check to see if the last run recorded an up or down level limit fault 
		if ((UL_ON_Fault == 1) || (DL_ON_Fault == 1) || (UL_DL_OFF_Fault == 1))
		{
			if (ignore_uldl_fault == 0)
			{
			 	if ((statusf & sf_ULDL) == 0)

			 	{
			 		if (UL_ON_Fault == 1)
			 			record_fault(f_ul_on);
			 		if (DL_ON_Fault == 1)
			 			record_fault(f_dl_on);

			 	}
				statusf |= sf_ULDL;
			}
			else
			{
				ignore_uldl_fault = 0;
				UL_ON_Fault = 0;
				DL_ON_Fault = 0;
				UL_DL_OFF_Fault = 0;
			}
		}
		else
			statusf &= ~sf_ULDL;

	  	if (((door_zone == 0) || (door_zone_aux == 0)) && (relevel == 0) && (rdinp(i_S10) == 1))
	  	{													 
			if((up_level == 1) && (dn_level == 1))    // dead level
			{
				if (door_zone == 0)
				{
					if (DZ_OFF_Fault == 0)
						record_fault(f_dz_off);
					DZ_OFF_Fault = 1;
				}
				if (door_zone_aux == 0)
				{
					if ((sel_can_error == 0) && (sel_can_online == 1))
					{
						if (DZA_OFF_Fault == 0)
							record_fault(f_dza_off);
						DZA_OFF_Fault = 1;
					}
				}
			}
		}

		if (DZA_OFF_Fault == 1)
		{
				   // Don't latch the error if it were from a can comm error
			if  ((sel_can_error == 1) ||
				((door_zone_aux == 1) && (up_level == 1) && (dn_level == 1)))
				DZA_OFF_Fault = 0;
		}

		if (DZ_OFF_Fault == 1)
		{
			if ((door_zone == 1) && (up_level == 1) && (dn_level == 1))
				DZ_OFF_Fault = 0;		// don't latch fault if input comes back on
		}

		// Check to see if the last run recorded a door zone fault 
		if ((DZ_ON_Fault == 1) || (DZA_ON_Fault == 1) || (DZ_OFF_Fault == 1) || (DZA_OFF_Fault == 1))
		{
			if ((statusf & sf_DZ) == 0)
			{
				if (DZ_ON_Fault == 1)
					record_fault(f_dz_on);
				if (DZA_ON_Fault == 1)
					record_fault(f_dza_on);
			}
			statusf |= sf_DZ;
		}
		else
			statusf &= ~sf_DZ;

		if (rdinp(i_DZsel) != door_zone)
		{
			if (timers[tDZsel] > 20)
			{
					
				if (rdinp(i_DZsel) == 0)
				{
					if (DZsn_OFF_Fault == 0)
						record_fault(f_dzsl_off);
					DZsn_OFF_Fault = 1;
				}
			}
		}
		else
		{
			timers[tDZsel] = 0;
			DZsn_OFF_Fault = 0;
		}
		

		if ((cons[equake] == 0) || (eqf == 0))
		{	// (don't want to move the car on earthquake)
	    	if(((dt_limit == 1) && (timers[tlimit] > 5) && (ut_limit == 1) && (stp_sw == 1)) &&	(Buffer_Test == 0) && (NTS_Test == 0) && (ETS_Test == 0) &&
	       		((position == cons[bottomf]) || (position == cons[topf])))
	    	{   //position is at top or bottom but elevator in the middle
				statusf |= sf_POS;
				record_fault(f_pos_error);
	      		if(position == cons[topf])
		  			position = (cons[topf] - 1);
	      		if(position == cons[bottomf])
		  			position = (cons[bottomf] + 1);
				if (cons[Car_Sw] == 0)
				{
		      		procf = 0;        // reset elevator lost
		      		rset = 3;
				}
	      		return;
	    	}
			else
				statusf &= ~sf_POS;

	    	if ( (dt_limit == 0) && (timers[tlimit] > 5) && (stp_sw == 1) && 
	    		 ( ((position != cons[bottomf]) && (cons[shortfl] != cons[bottomf]))  ||
	    		   ((position > (cons[bottomf]+1)) && (cons[shortfl] == cons[bottomf])) ) )
	    	{   // elevator at bottom but position does not match
				statusf |= sf_POS;
				record_fault(f_pos_error);
				if (cons[Car_Sw] == 0)
		      		procf = 0;        // reset elevator lost
	      		position = cons[bottomf];
	      		return;
	    	}
			else
				statusf &= ~sf_POS;

	    	if ( (ut_limit == 0) && (timers[tlimit] > 5) && (stp_sw == 1) &&
	    		 ( ((position != cons[topf]) && ((cons[shortfl] == 0) || (cons[shortfl] != (cons[topf]-1))))  ||
	    		   ((position < (cons[topf]-1)) && (cons[shortfl] == (cons[topf]-1))) ) )
	    	{   // elevator at top but position does not match
				statusf |= sf_POS;
				record_fault(f_pos_error);
				if (cons[Car_Sw] == 0)
	      			procf = 0;        // reset elevator lost
      			position = cons[topf];
	      		return;
	    	}
			else
				statusf &= ~sf_POS;

			if ((door_zone == 1) && (Buffer_Test == 0) && (NTS_Test == 0) && (ETS_Test == 0))
			{
				if ((position == cons[topf]) && (timers[tlimit] > 5) && (stp_sw == 1))
				{
					if (ut_limit == 1)
					{
						if (limit_fault == 0)
							record_fault(f_UT_flt);
						limit_fault = 1;
					}
				}
				if ((position == cons[bottomf]) && (timers[tlimit] > 5) && (stp_sw == 1))
				{
					if (dt_limit == 1)
					{
						if (limit_fault == 0)
							record_fault(f_DT_flt);
						limit_fault = 1;
					}
				}
			}
			else limit_fault = 0;
		}
		else limit_fault = 0;


		if (limit_fault == 1)
			statusf |= sf_TLimit;
		else
			statusf &= ~sf_TLimit;					
		
		// Turning off high speed outputs
    	clroutp(o_SUF);       // clear up fast output
   		clroutp(o_SDF);       // clear down fast output

		if ((io_fault == 0) && (rdoutp(o_RUN) == 0)) 
		{		   // No i/o fault AND not commanding a run
			if ((rdinp(i_SUF) == 0) && (rdinp(i_SDF) == 0))
				timers[tsufsdfdly] = 0;

			if ((rdinp(i_SUF) == 1) && (rdoutp(o_SUF) == 0) && (timers[tsufsdfdly] > 5)) 
			{	  // Debounce for 500 msec
				record_fault(f_SUF_on);
				io_fault = 1;
				timers[tsufsdfdly] = 0;
			}

			if ((rdinp(i_SDF) == 1) && (rdoutp(o_SDF) == 0) && (timers[tsufsdfdly] > 5))
			{
				record_fault(f_SDF_on);
				io_fault = 1;
				timers[tsufsdfdly] = 0;
			} 
		}
		else
			timers[tsufsdfdly] = 0;
		
		// Checking to see if dead floor level
		// ************************************************
		// DEAD LEVEL
		// ************************************************

		write_trace(TR_ATFLOOR2);

		set_ul_dl_dz();		// sets up_level, dn_level and door_zone


		if ((dn_level == 1) && (up_level == 1))				// dead floor level
		{
			if (rdoutp(o_MCC) == 0)		// MCC turned off
			{
				leveling_fault_cnt = 0;			// reset the fault count when car stops with a good relevel
				timers[trelev] = 0;
				leveling = 0;
				relev_stop_count = 0;
				lev_latch = 0;
			}

			if (relevel == 1)
			{
				timers[tgen] = 0;
				timers[tsoftstp] = 0;
				timers[tnit] = 0;
			}
			relevel = 0;              // not releveling
			relevel_req = 0;
			rset_run_cnt = 0;
			lev_startf = 0;

			// Hydro only
			if (in_redundancy_test == 0)
			{
				clroutp(o_SD);   // clear down output
				clroutp(o_SU);      // clear up output
			}
			clroutp(o_RUN);   // clear run output

	        // Checking to see if a soft stop is needed
			if((timers[tsoftstp] <= fvars[fvsst]) && ((cons[Micro_Relev] == 0) || (micro_relevel == 0)))  // soft stop needed
			{
				timers[tmotion] = 0;      // not in motion clear motion timer
				if(dirf == 1)
				{
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
				    	setoutp(o_MST);
				    setoutp(o_MCC);
				}
			}
			else      // No soft stop
			{
				dirf = 0;

				// viscoustiy control run the motor
				if( (servf > s_LOW_OIL) && (dpref == 0) && (dcalls == 0) &&
					(((rdinp(i_TPL) == 1) && (fvars[fvtpl] == 0)) || 
				 	((rdinp(i_TPL) == 0) && (fvars[fvtpl] == 1))) && 
					(rdoorf == 0) && (doorf == 0) && (position == lobby_fl) &&
					(((redundancy_test >= last_redundancy_test) && (in_redundancy_test == 0)) || (cons[SFC_2004_IO] == 0)) )
				{
					if(timers[tmotion] > 100)
						timers[tmotion] = 100;
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
					{
					  	if(timers[tmotion] > fvars[fvydel])
					       	setoutp(o_MST);
					  	else
					       	clroutp(o_MST);
					}
				  	setoutp(o_MCC);
				  	Viscosity = timers[tmotion];
			    }
			    else
			    {
				  	if (((rdinp(i_TPL) == 0) && (fvars[fvtpl] == 0)) || 
				  		((rdinp(i_TPL) == 1) && (fvars[fvtpl] == 1)))
				  	{
						if(Viscosity == 0)
						{
					    	timers[tmotion] = 0;      // not in motion clear motion timer
						}
						else
						{
					    	if((timers[tmotion] - Viscosity) > 25)
								Viscosity = 0;
						}
				  	}
				  	else
				  	{
						if((timers[tmotion] - Viscosity) > 25)
						{
							timers[tmotion] = 0;
							Viscosity = 0;
						}
				  	}
				  	clroutp(o_MCC);
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
					  	clroutp(o_MST);
				}
	      	}
		    if (cons[Micro_Relev] == 1) 
			{
				if (timers[tsoftstp] >= 3)
				{
					clroutp(o_UPML);
					clroutp(o_DNML);
				}
			}
	    }
	// Elevator is not level needs to relevel
	    else
	    {

	    	// can't relevel down if the stop switch is pulled on a hydro
		  	if (((rdinp(i_CEN) == 0) && ((Drive_Type[cons[carnmb]] == 1) || (Drive_Type[cons[carnmb]] == 9)) && (up_level == 0)) ||
			(io_fault != 0) ||			// can't relevel with an io fault
	    	// can't relevel too long
		  	(stallf != 0) ||
		  	// can't relevel if run has dropped out but MC is still in 
		  	((rdoutp(o_RUN) == 0) && (rdoutp(o_MCC) == 1)) ||
		  	// can't relevel if testing the sfc output
		  	(in_redundancy_test == 1) ||
			// wait for relevel timer to expire
			(timers[trelev] < fvars[fvrlvtim]) ||	 // wait 2 seconds
	    	// can't relevel up if on the up directional stop
		  	((up_level == 1) && (rdinp(i_UNsel) == 0) && (door_zone == 0)) ||
	    	// can't relevel down if on the down directional stop
		  	((dn_level == 1) && (rdinp(i_DNsel) == 0) && (door_zone == 0)) ||
			// ul or dl has failed
			(UL_DL_OFF_Fault == 1) ||
	    	// can't relevel until soft stop has occured
		  	(timers[tsoftstp] <= (fvars[fvsst] + 15)) ||
			((door_zone == 0) && ((door_locks() == 0) || (car_gate() == 0))) ||
		  	((empf != no_ep) && (empf != ep_recall) && (empf != ep_select)) )
	      	{

				fault_prog_flag1 = 3;
			  	timers[tmotion] = 0;
				timers[tpatdly] = 0;
				relevel = 0;
				relevel_req = 1;
				lev_startf = 0;
				

			  	if ((rdinp(i_CEN) == 0) && (up_level == 0)) // zero the relevel timer to give the safety processor time to reset
					timers[trelev] = 0;

				if((timers[tsoftstp] > fvars[fvsst]) || (dirf == 2) || ((cons[Micro_Relev] == 1) && (micro_relevel == 1)))
				{
					clroutp(o_MCC);      // Clear motor starter output
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
						clroutp(o_MST); // Turn off motor starter timer output
				}
			    if (cons[Micro_Relev] == 1)
				{
					clroutp(o_UPML);
					clroutp(o_DNML);
				}
				clroutp(o_SU);      // Clear up output
				clroutp(o_SD);   // Clear down output
				clroutp(o_RUN);   // Clear run output
			}
		    else
		    {

				fault_prog_flag1 = 4;

				relevel = 1;
				relevel_req = 0;
				timers[tnit] = 0;

				if (level_stop_fault == 1)
				{
					relev_stop_count ++;
					if (relev_stop_count < 4)
					{
						leveling = 0;
						lev_latch = 0;
						level_stop_fault = 0;
					}
					else if (relev_stop_count == 4)
						record_fault(f_lev_stop_cnt);

				}

				if (stallf == 0)
				{
					if (timers[tmotion] >= fvars[fvstall])
					{
						record_fault(f_stall_fault);
						stallf = 1;
					}
				}

		      	// relevel up
				if ((dn_level == 0) && (up_level == 1) && (in_redundancy_test == 0))
				{
			  		dirf = 1;
				  	trigger_arm_state |= RELEV_START;
			  		

					if ((cons[Micro_Relev] == 0) || (door_zone == 0) || (rdoutp(o_SU) == 1))
					{
						if (lev_startf == 0)
						{
							
							if (chk_lev_start(dirf) == 1)
								lev_startf = 1;
							else
								lev_startf = 0;
						}
						else
						{
							if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
							{		
					  			if(timers[tmotion] > fvars[fvydel])
					     			setoutp(o_MST);
						  		else
						     		clroutp(o_MST);
							}
					  		setoutp(o_MCC);    // Turn on motor starter output
													// Turn on up output when DEL is on
							if(rdinp(i_SPD) == 0)
								clroutp(o_SU);
							else
								setoutp(o_SU);

							if (cons[Micro_Relev] == 1)
								clroutp(o_UPML);
							micro_relevel = 0;
					  		setoutp(o_RUN); // Turn on the run output
					  		clroutp(o_SD); // Turn off down output
						}
					}
					else
					{
						micro_relevel = 1;
						setoutp(o_UPML);
				  		setoutp(o_RUN); // Turn on the run output
				  		clroutp(o_SD); // Turn off down output
					}
				}

		      	// relevel down
				if ((up_level == 0) && (dn_level == 1) && (in_redundancy_test == 0))
				{
			  		dirf = 2;
				  	trigger_arm_state |= RELEV_START;

			 // Hydro only
					if ((cons[Micro_Relev] == 0) || (door_zone == 0) || (rdoutp(o_SD) == 1))
					{
						if (lev_startf == 0)
						{
							
							if (chk_lev_start(dirf) == 1)
								lev_startf = 1;
							else
								lev_startf = 0;
						}
						else
						{
					  		clroutp(o_MCC);    // Turn off motor starter output
					  		setoutp(o_SD); // Turn on down output
							if (cons[Micro_Relev] == 1)
								clroutp(o_DNML);
							micro_relevel = 0;
					  		setoutp(o_RUN); // Turn on the run output
					  		clroutp(o_SU);    // Turn off up output
							if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
						  		clroutp(o_MST); // Turn off motor starter timer output

						}
					}
					else
					{
						micro_relevel = 1;
						setoutp(o_DNML);
				  		setoutp(o_RUN); // Turn on the run output
				  		clroutp(o_SU);    // Turn off up output
						if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
					  		clroutp(o_MST); // Turn off motor starter timer output
					}
				}
			}
		}
	    if (((firef == 0) || (firef >= 4) || (rfiref >= 4)) && ((medf == 0) || (medf >= 3)))
	    {
			if ((in_redundancy_test == 0) && (car_Grp_Stat.c_ids_return_typ == 0))
			{
		      	clroutp(o_FST);  // turn off the fire service stopswitch bypass
	    	  	clroutp(o_FSTP);  // turn off the fire service stopswitch bypass
	      		FS_DOB_DIS = 0;
		    }
	    }
	    else
			clr_dir_arrow();
	    

		write_trace(TR_ATFLOOR3);
		
	    pisout(position);
	    if (chkdoor()== 1)
	    {		// gripper test cannot be run
			if (in_redundancy_test != 0)
			{
				clroutp(o_SU);
				clroutp(o_SD);
				clroutp(o_FST);
				clroutp(o_FSTP);
				in_redundancy_test = 0;
				redundancy_test = 0;
			}
	    }

		write_trace(TR_ATFLOOR4);

		if((manual_door == 2) && (dpref != 0))
		{
		    if(rdinp(i_DOL) == 1)
			    timers[tdwel] = 0;
		    if((timers[tdwel] > doort) && (rdinp(i_DOL) == 0))
			    dirsel();
		}
		if((manual_rdoor == 2) && (dpref != 0) && (cons[rear] != 0))
		{
		    if (rdinp(i_DOLR) == 1) 
			    timers[trdwel] = 0;			 
		    if((timers[trdwel] > rdoort) && (rdinp(i_DOLR) == 0))
			    dirsel();
		}

	    if(dpref == 0)
	    {
	      	dirsel();
	      	if (((fvars[fvviplant] == 0) || (cons[VipSvc] == 0) || (vipf != 3)) && (fvars[fvarrivallant] == 0))			
	      		clrlant();
	      	timers[tlant] = 0;			
	      	timers[trlant] = 0;
			if (fvars[fvarrivallant] == 1)
				hlants();
	    }
	    else
	    {
	      	if((doorf != 0) || (rdoorf != 0) || ((cons[frghtd] != 0) && (rdinp(i_DOL) == 0))
	      		|| ((cons[rear] !=0) && (cons[frghtd] != 0) && 
	      			(rdinp(i_DOLR) == 0)) )
			{
				if ((prev_pref != dpref) && (dpref != 0))
					timers[tlantchange] = 0;
				prev_pref = dpref;

				if (timers[tlantchange] >= fvars[fvlantprefdly])
					hlants();
				else
			    	clrlant();
				set_dir_arrow();
			}			
	    }

		if (cons[hugs] != 0)
			sethugsf();

		if (cons[Cycle_Run] == 1)
			cycle_run();
		
	    doors();
	    
	    if(cons[rear] != 0)
			rdoors();
	    else
	    {
			rdoorf = 0;
			rfiref = 0;
	    }

	    chkrun();				// from atfloor


  	}
}

                       

// ****************************************************
// Status of the car gate (0=gate open, 1=gates closed)
// ****************************************************

int16 car_gate(void)
{
	if ( (rdinp(i_GS) == 1) && (rdinp(i_GS_1) == 1) &&
		((cons[rear] == 0)	|| ((rdinp(i_RGS) == 1) && (rdinp(i_RGS_1) == 1))) )
		return 1;
	else
		return 0;
}

// *******************************************************
// This is check to see if there is a need to run or move
// *******************************************************
void chkrun()
{

	write_trace(TR_CHKRUN1);


	
 	if(cons[rear] == 0)
		rdoorf = 0;

	statusf &= ~(sf_BKS | sf_P | sf_DCL);	   // Clear here if subroutine return occurs before condition can be cleared
			
	write_trace(TR_CHKRUN2);

  	if(Hoistway_Learned != 1)
	{
		if ((statusf2 & sf_LearnHW) == 0)
			record_fault(f_hw_learn);
		statusf2 |= sf_LearnHW;
		chkrun_state = 1;
     	return;
	}
	else
		statusf2 &= ~sf_LearnHW;
		
	if (rdinp(i_PALF) == 1)
	{
		chkrun_state = 2;
		return;
	}
			
	if ((cop_can_error == 1) || 
		(((statusf & sf_P) != 0) && (Drive_Type[cons[carnmb]] == 9)) ||
		(hardware_init_fault == 1) || (door_motor_ovl == 1) || 
		((aps_sel_can_error == 1) && (cons[sel_type] == 4)) ||
		((sel_can_error == 1) && (cons[sel_type] == 2))) 
	{
		chkrun_state = 3;
		return;						// No automatic run without the TOP of car board
	}
	
	if ((fvars[fvlwa] != 0) && (firef == 0) && (rfiref == 0))
	{
		if ((nmb_carcalls > fvars[fvlwa]) && ((cons[loadw] & 1) != 0) && (rdinp(i_LWA) == 0))
		{
			clrcc(0);
		}
	}
	
	if ((fvars[fvrunct] != 0) && (firef == 0) && (rfiref == 0) && (servf == s_AUTO))
	{
		if (run_no_pass_cnt >= fvars[fvrunct])
		{
			run_no_pass_cnt = 0;
			clrcc(0);
		}
	}
	else
		run_no_pass_cnt = 0;
			

	if((safe() == 1) || (dpref == 0) || (rdinp(i_DCL) == 1) || (rdoutp(o_DC) == 0) ||
      	(((rdinp(i_DCLR) == 1) || (rdoutp(o_DCR) == 0)) && (cons[rear] != 0)) )
	{
		safe_fault_latch = 0;
		timers[tsafe] = 0;
	}

 	if (in_redundancy_test == 1)		// cannot run until after SFC is tested
	{
		chkrun_state = 4;
		statusf2 |= sf_IO_Test;
		return;
	}
	statusf2 &= ~sf_IO_Test;
 	
 
  	if ((timers[tnit] < fvars[fvnit]) || (timers[tnit] < (fvars[fvsst] + 15)) ||        // wait for non interance and soft stop time
  		((cons[pbdoormsk] != 0) && ((pbdoor_fmsk[position] != 0) || (pbdoor_rmsk[position] != 0)) && (timers[tpbdoort] < 100)))	// stop the car at least for 5 seconds for pbdoors fl
	{
		chkrun_state = 5;
		statusf2 |= sf_NIT;
    	return;
	}
	statusf2 &= ~sf_NIT;
	

	if (((manual_door == 1) &&
			((ccmsk[cons[carnmb]][position] != 0) || 
				(ucmsk[cons[carnmb]][position] != 0) || 
				(dcmsk[cons[carnmb]][position] != 0))) || 
		((cons[rear] != 0) && (manual_rdoor == 1) && 
			((rccmsk[cons[carnmb]][position] != 0) || 
				(rucmsk[cons[carnmb]][position] != 0) || 
				(rdcmsk[cons[carnmb]][position] != 0))) )
	{
		if ((prev_dpref != dpref) && (cons[frghtd] != 0) && ((doorf != 0) || ((rdoorf != 0) && (cons[rear] != 0))))
			timers[tnit] = 0;
		prev_dpref = dpref;
		if(((fvars[fvattmandir] & 0x04)!=0)  ||  ((athcd_in_enable == 1)&&(rdinp(i_ATHCD) == 1)))		
		{
			if((cons[att] != 0) && (servf == s_ATT)) // on attendant
			{
			   	if(allow_run == 0)			
				{
					if((safe() == 1) && (onward_cc != 0))
					{
						allow_run = 1;
						return;
					}
					else if((manual_door == 1) && (car_gate() == 1) && (man_dcc() == 1) && (onward_cc != 0))
					{
						allow_run = 1;
						return;
					}
					else
					{
						chkrun_state=6;
						allow_run = 0;
						timers[tnit] = 0;
						return;
					}
				}
			}
		}
		else if ((fvars[fvattmandir] & 0x01)!=0)
		{
			if((cons[att] != 0) && (servf == s_ATT)) // on attendant
			{
				if(allow_run == 0)
				{
					if(safe() == 1)
					{
						if((fvars[fvattmandir] & 0x02) == 0)
						{
							if(rdinp(i_ATTUP) == 1)
								allow_run = 1;
							else if(rdinp(i_ATTDN) == 1)
								allow_run = 1;
						}
						chkrun_state = 7;
						return;
					}
					else
					{
						allow_run = 0;
						timers[tnit] = 0;
						chkrun_state = 8;
						return;
					}
				}

			}
			else if(cons[att] != 0)
			{
				if((fvars[fvattmandir] & 0x02) != 0)
				{
					if((allow_run == 0))
					{
						if(safe() == 1)
						{
							if(rdinp(i_ATTUP) == 1)
								allow_run = 1;
							chkrun_state = 9;
							return;
						}
						else
						{
							allow_run = 0;
							timers[tnit] = 0;
							chkrun_state = 10;
							return;
						}
					}
					
				}
			}
		}	
		
		if ((cons[Car_Sw] & 1) != 0)
		{
			if((car_gate() == 0))
			{
				timers[tnit] = 0; // added this statement to force extra delay as long as doors are open				
				if((rdinp(i_ATTUP) == 1) || (rdinp(i_ATTDN) == 1))
				{
					okto_run = 0;
					timers[tnit] = 0;
					chkrun_state = 11;
					return;
				}
				else
				{
					okto_run = 1;					
				}
				
			}
			else if (okto_run == 0)
			{
				if((rdinp(i_ATTUP) == 0) && (rdinp(i_ATTDN) == 0))
				{
					okto_run = 1;
				}
				else	// WAIT for operate to release the handle
				{
					timers[tnit] = 0;
					chkrun_state = 12;
					return;	
				}
			}
			
		}			
				

		if (timers[tnit] < fvars[fvhcdt])	// wait for door time a manual door floor
		{
			chkrun_state = 13;
			return;
		}
		dirsel();
	}


  	day_of_week = d.dayofweek;
  	if(day_of_week != old_day_of_week)
  	{
		timers[tgroup_hcc]  = 0;
		timers[ccct] = 0;
		if(cons[nmbcars] > 1)
			Up_Date_Calls();
		local_getdate();
		if ((day_of_week != d.dayofweek) || (d.dayofweek > 6))
			local_setdate();	// update day of the week  (calc_dayofweek() is called in local_setdate())
	  	day_of_week = d.dayofweek;
		old_day_of_week = day_of_week;
		rhjackf = 1;		// enable reset hydro jack mode
  	}

	if (cons[saf_test] == 1)
	{		 // Check for annual safety test
	  	if(d.year > (signed)fvars[fvspt_year])
		{
			statusf |= sf_ASTST;
			chkrun_state = 14;
		 	return;
		}

	  	if((d.year == (signed)fvars[fvspt_year]) && (d.month > fvars[fvspt_mon]))
		{
			statusf |= sf_ASTST;
			chkrun_state = 15;
		 	return;
		}

	  	if((d.year == (signed)fvars[fvspt_year]) && (d.month == fvars[fvspt_mon]) &&
	     	(d.day > fvars[fvspt_day]))
		{
			statusf |= sf_ASTST;
			chkrun_state = 16;
		 	return;
		}
	}

	statusf &= ~sf_ASTST;

	if (((fdoor_req == 0) && ((rdoor_req == 0) || (cons[rear] == 0))) || 
		((doorf != 0) || (rdoorf != 0)))
		timers[tdoorrq] = 0;

	if (((fdoor_req != 0) || ((cons[rear] != 0) && (rdoor_req != 0))) && (timers[tdoorrq] < 50) && (firef == 0) && (rfiref == 0))
	{
		chkrun_state = 17;
		statusf2 |= sf_DOOR_Req;
  		return;		// waiting for non-simultaneous door to switch operation
	}
	statusf2 &= ~sf_DOOR_Req;

  	if ((doorf == 1) || (doorf == 2) || (rdoorf == 1) || (rdoorf == 2))
	{
		chkrun_state = 18;
    	return;
	}

    if ((firef != 0) || (rfiref != 0) || (servf != s_AUTO) || (manual_door == 1) || (manual_rdoor == 1) || (cons[frghtd] != 0)) 
		adv_pre_trq_run = 0;

  	if ((doorf == 3) || (doorf == 4) || (rdoorf == 3) || (rdoorf == 4))
	{
		chkrun_state = 19;
    	return;
	}

	prev_dpref = dpref;

  	dirsel();

	rset_error_cnt = 0;		// made it out of reset
	
  	if( (dpref == 2) && (downcb[position] == 1) && (doorf == 0) && ((nudgst & 0x02) == 0) && 
  									((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0)) )
	{
		chkrun_state = 21;
    	return;
	}
  	if( (dpref == 1) && (upcb[position] == 1) && (ncu_door_seq != 3) && (doorf == 0) && ((nudgst & 0x01) == 0) &&
  									((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0)) )
	{
		chkrun_state = 22;
    	return;
	}
  	if( (dpref == 1) && (lby_req_fl == position) && (ncu_door_seq != 3) && (doorf == 0) && ((nudgst & 0x01) == 0) &&
  									((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0)))
	{
		chkrun_state = 23;
    	return;
	}
  	if(cons[rear] != 0)
  	{
	  	if( (dpref == 2) && (rdowncb[position] == 1) && (rdoorf == 0) && ((rnudgst & 0x02) == 0) &&
	  								((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0)) )
		{
			chkrun_state = 24;
	    	return;
		}
	  	if( (dpref == 1) && (rupcb[position] == 1) && (rdoorf == 0) && ((rnudgst & 0x01) == 0) &&
	  								((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0)) )
		{
			chkrun_state = 25;
	    	return;
		}
  	}

  	if(Viscosity != 0)
	{
		chkrun_state = 26;
		statusf2 |= sf_VISC;
		return;
	}
	statusf2 &= ~sf_VISC;


	if (((relevel == 1) || (relevel_req == 1)) && (stallf == 0))
	{
		chkrun_state = 27;
		statusf2 |= sf_LEV_Req;
		return;					// wait until relevel finishes before the run starts
	}
	statusf2 &= ~sf_LEV_Req;
  	if(dcalls == 0)                       // no calls
	{
		chkrun_state = 28;
    	return;
	}
	if (eqf != 0)
	{
		chkrun_state = 29;
		return;					// do not allow an automatic run on earthquake
	}

  	if (rdinp(i_DCL) == 1)         // wait until doors are closed
  	{
    	if ((doorf == 0) && (cons[frghtd] == 0) && ((cons[pbdoor] & 2) == 0) && (fs2_no_fdoor_op == 0))
      	{
      		setoutp(o_DC);
 			timers[tdodly] = 0;
		}
		statusf |= sf_DCL;		// waiting for DCL
		chkrun_state = 30;
    	return;
  	}

  	if ((rdinp(i_DCLR) == 1) && (cons[rear] != 0))
  	{
    	if((rdoorf == 0) && (cons[frghtd] == 0) && (fs2_no_rdoor_op == 0))		// wait until doors are closed
      	{
      		setoutp(o_DCR);
 			timers[trdodly] = 0;
		}
		statusf |= sf_DCL;	// waiting for RDCL
		chkrun_state = 31;
    	return;
  	}

  	if (safe() == 0)
  	{
  		if ((cons[frghtd] == 0) && ((manual_door == 0) || (manual_door == 2)) && (fs2_no_fdoor_op == 0))  // wait until safe
	  	{
	    	timers[tdc] = 0;
	    	timers[tdo] = 0;
	    	timers[tdwel] = 0;
	    	setoutp(o_DC);
 			timers[tdodly] = 0;
	    	if((timers[tsafe] >= 30) && (firef == 0) && (door_zone == 1))
	    	{
				record_fault(f_car_safe_start);
				set_safe_fault();
				clroutp(o_DC);
				if(ccmsk[cons[carnmb]][position] == 1)
				{
					doorf = 1;
	  	  		  	if ((manual_door == 0) || (manual_door == 2))   
				  		safe_closetry++;			// protection timer did not time out lets count the close tries
				}
	    	}
		}
  		if ((cons[frghtd] == 0) && ((manual_rdoor == 0) || (manual_rdoor == 2)) && (fs2_no_rdoor_op == 0))  // wait until safe
	  	{
	    	timers[trdc] = 0;
	    	timers[trdo] = 0;
	    	timers[trdwel] = 0;
	    	setoutp(o_DCR);
 			timers[trdodly] = 0;
	    	if((timers[tsafe] >= 30) && (rfiref == 0) && (door_zone == 1))
	    	{
				record_fault(f_car_safe_start);
				set_safe_fault();
				clroutp(o_DCR);
				if(rccmsk[cons[carnmb]][position] == 1)
				{
					rdoorf = 1;
		  	    	if ((manual_rdoor == 0) || (manual_rdoor == 2))
						safe_rclosetry++;  // protection timer did not time out lets count the close tries
				}
	    	}
		}
		statusf |= sf_SAFE;
		chkrun_state = 32;
    	return;
  	}
	statusf &= ~sf_SAFE;


	door_buz = 0;
	freight_fdoor_alarm = 0;
	freight_rdoor_alarm = 0;
	timers[tdoorbz] = 0;

  	if( (rdinp(i_SU) == 0) && (rdinp(i_SD) == 0) )
  	{
		timers[ccct] = 0;
  	}
  	timers[tgen] = 0;
	timers[tebkdrop] = 0;
	timers[tsoftstp] = 0;
  	timers[tmotion] = 0;
	timers[tpatdly] = 0;
  	hsf = 1;
	old_dpp = DPP_Count;
	timers[tdppflt] = 0;
  	start_floor = position;
  	no_fault = 1;


	chkrun_state = 33;
	

  	if((dpref == 1) && ((dcalls == 1) || (dcalls == 3)))  // going up
  	{
		if ((cons[shortfl] != 0) && (cons[vshort_tfl] != 0) && (position == cons[shortfl]))
		{			 // At the lower short floor
			if ((carcb[cons[vshort_tfl]] == 1) || (upcb[cons[vshort_tfl]] == 1) || (downcb[cons[vshort_tfl]] == 1) ||
				(rcarcb[cons[vshort_tfl]] == 1) || (rupcb[cons[vshort_tfl]] == 1) || (rdowncb[cons[vshort_tfl]] == 1) ||
				 (stops[0] == cons[vshort_tfl]))
			{	// Need to run to the upper short floor
				if ((fvars[fvshrtflctl] & 0x01) == 0)
				{		 // Very Short floor relevel
					position++;
					front_slowdown = 0;
					rear_slowdown = 0;
					needstop();				// Set the slowdown flags to open the door
					initial_stop();			// clear timers and record flight times
					reset_nonit = 1;
					clr_door_vars();
					relevel_req = 1;
					preset = 0;
					if (cons[StagRear] == 0)
						setoutp(o_DTR);
					else if (cons[exDHwL] == 0)
						setoutp(o_SFST);
					return;
				}
				else   // Very short floor run
				{
					ignore_uldl_fault = 1;
					UL_ON_Fault = 0;
					DL_ON_Fault = 0;
					DZ_ON_Fault = 0;
					DZA_ON_Fault = 0;
					if (cons[StagRear] == 0)
						setoutp(o_DTR);
					else if (cons[exDHwL] == 0)
						setoutp(o_SFST);
				}
			}
		}

		short_fl_run = 0;
		no_high_speed = 0;
		pass_fl = 0;
		if (position < cons[topf])
		{
			if ((cons[shortfl] == position) || (cons[shortflmid] == position))
			{		// Next to a short floor 
				short_fl_run = 1;	 // Need to run to the upper short floor
				if ((carcb[position+1] == 1) || (upcb[position+1] == 1) || (downcb[position+1] == 1) ||
					(rcarcb[position+1] == 1) || (rupcb[position+1] == 1) || (rdowncb[position+1] == 1) ||
					(stops[0] == (position+1)))
				{
					if (((fvars[fvshort_usdt] == 0) && (cons[shortfl] == position)) || 
						((fvars[fvshortm_usdt] == 0) && (cons[shortflmid] == position)))
					{
						position++;			// no slowdown magnets between short floors
						front_slowdown = 0;
						rear_slowdown = 0;
						needstop();				// Set the slowdown flags to open the door
						hsf = 0;
					}		 // check in else part so slowdown timer is non-zero
					else if ((fvars[fvsfnohsv] == 1) && ((cons[shortfl] == position) || (cons[shortflmid] == position)))
						no_high_speed = 1;
				}
			}
		}

		timers[tshortfl] = 0;

    	dirf = 1;

		if (chk_start(1) == 1)
		{
			startf = 1;
			front_slowdown = 0;
			rear_slowdown = 0;
			SD_Count = 0;
			dtmrf = 0;
	    	DZ_ON_Fault = 1;
			DZA_ON_Fault = 1;
			UL_ON_Fault = 1;
			DL_ON_Fault = 1;
			if ((servf == s_AUTO) && (procf != 3))
			{
				if (pass_det == 1)
					run_no_pass_cnt = 0;
				else
					run_no_pass_cnt++;
			}
			pass_det = 0;
    		procf = 3;          // go into motion from chkrun()
			timers[tpatdly] = 0;
			timers[tadvptrq] = 0;
			timers[tsafe] = 0;
			safe_fault_latch = 0;

			gripper_test = 15;
			io_fault_cnt = 0;
			don_delay = 0;
			clr_door_vars();
		}
		else
		{
			startf = 0;
			clroutp(o_SU);
			clroutp(o_SD);
			clroutp(o_SUF);
			clroutp(o_SDF);
			clroutp(o_RUN);
			clroutp(o_MCC);

			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
				clroutp(o_MST);
			timers[tpatdly] = 0;
			timers[tmotion] = 0;
			timers[tsoftstp] = (int16)(fvars[fvsst] + 15);
		}
    	return;
  	}
  	if((dpref == 2) && (dcalls > 1))      // going down
  	{
		if ((cons[shortfl] != 0) && (cons[vshort_tfl] != 0) && (position == cons[vshort_tfl]))
		{			 // At the upper short floor
			if ((carcb[cons[shortfl]] == 1) || (upcb[cons[shortfl]] == 1) || (downcb[cons[shortfl]] == 1) ||
				(rcarcb[cons[shortfl]] == 1) || (rupcb[cons[shortfl]] == 1) || (rdowncb[cons[shortfl]] == 1) || 
				(stops[0] == cons[shortfl]))
			{	// Need to run to the lower short floor
				if ((fvars[fvshrtflctl] & 0x01) == 0)
				{		 // Very Short floor relevel
					position--;
					front_slowdown = 0;
					rear_slowdown = 0;
					needstop();				// Set the slowdown flags to open the door
					initial_stop();			// clear timers and record flight times
					reset_nonit = 1;
					clr_door_vars();
					relevel_req = 1;
					preset = 0;
					if (cons[StagRear] == 0)
						clroutp(o_DTR);
					else if (cons[exDHwL] == 0)
						clroutp(o_SFST);
					return;
				}
				else   // Very short floor run
				{
					ignore_uldl_fault = 1;
					UL_ON_Fault = 0;
					DL_ON_Fault = 0;
					DZ_ON_Fault = 0;
					DZA_ON_Fault = 0;
					if (cons[StagRear] == 0)
						clroutp(o_DTR);
					else if (cons[exDHwL] == 0)
						clroutp(o_SFST);
				}
			}
		}


		short_fl_run = 0;
		no_high_speed = 0;
		if (position > 1)
		{
			if (((cons[shortfl]+1) == position) || ((cons[shortflmid]+1) == position))
			{
				short_fl_run = 1;	 // Need to run to the lower short floor
				if ((carcb[position-1] == 1) || (upcb[position-1] == 1) || (downcb[position-1] == 1) ||
					(rcarcb[position-1] == 1) || (rupcb[position-1] == 1) || (rdowncb[position-1] == 1) ||
					(stops[0] == (position-1)))
				{
					if (((fvars[fvshort_dsdt] == 0) && ((cons[shortfl]+1) == position)) || 
						((fvars[fvshortm_dsdt] == 0) && ((cons[shortflmid]+1) == position)))
					{
						position--;			// no slowdown magnets between short floors
						front_slowdown = 0;
						rear_slowdown = 0;
						needstop();				// Set the slowdown flags to open the door
						hsf = 0;
					}		 // check in else part so slowdown timer is non-zero
					else if ((fvars[fvsfnohsv] == 1) && (((cons[shortfl]+1) == position) || ((cons[shortflmid]+1) == position)))
						no_high_speed = 1;
				}
			}
		}

		timers[tshortfl] = 0;

    	dirf = 2;
			
		if (chk_start(2) == 1)
		{
			startf = 1;
			front_slowdown = 0;
			rear_slowdown = 0;
			SD_Count = 0;
			dtmrf = 0;
	    	DZ_ON_Fault = 1;
			DZA_ON_Fault = 1;
			UL_ON_Fault = 1;
			DL_ON_Fault = 1;
			if ((servf == s_AUTO) && (procf != 3))
			{
				if (pass_det == 1)
					run_no_pass_cnt = 0;
				else
					run_no_pass_cnt++;
			}
			pass_det = 0;
    		procf = 3;          // go into motion from chkrun()
			timers[tsafe] = 0;
			timers[tpatdly] = 0;
			timers[tadvptrq] = 0;
			safe_fault_latch = 0;

			gripper_test = 15;
			io_fault_cnt = 0;
			don_delay = 0;
			clr_door_vars();
		}
		else
		{
			startf = 0;
			clroutp(o_SU);
			clroutp(o_SD);
			clroutp(o_SUF);
			clroutp(o_SDF);
			clroutp(o_RUN);
			clroutp(o_MCC);
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
				clroutp(o_MST);
			timers[tmotion] = 0;
			timers[tpatdly] = 0;
			timers[tsoftstp] = (int16)(fvars[fvsst] + 15);
		}
  	}
  	return;
}
//******************************
// Check start of run for Hydro 
//******************************


int16 chk_start(int16 cmd)
{
	static int16 dbn_cnt;

	if (io_fault != 0)
	{
//		chkstart_state = 1;		Not set here, will overwrite data
		return(0);
	}

    if (cons[Micro_Relev] == 1)
	{
		clroutp(o_UPML);
		clroutp(o_DNML);
	}

	if (cmd == 1)
	{		// run up

		if (rdinp(i_UNsel) == 0)
		{			// UN must be on to run
			chkstart_state = 1;
			return(0);
		}
			

		if ((firef == 0) && (car_Grp_Stat.c_ids_return_typ == 0) && (medf == 0))
		{
			// FST must be off
			timers[tiofault] = 0;
			dbn_cnt = 0;
			while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
			{
				inctime(1);
				if (rdinp(i_FST) == 0)	  // Fst is off
					dbn_cnt ++;		   // Exit the loop when FST goes oFF
				else
					dbn_cnt = 0;
			}

			if(timers[tiofault] >= c_io_dbn_time) 
			{	// FST failed on
				record_fault(f_FST_failed_on);
				io_fault = 1;
				chkstart_state = 2;
				return 0;			
			}
		}

		// Verify that Up to speed SPD is off

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SPD) == 0)
				dbn_cnt++;
			else
				dbn_cnt = 0;
		}

		if (timers[tiofault] >= c_io_dbn_time) 
		{	// SPD failed on
			record_fault(f_SPD_on);
			io_fault = 1;
			chkstart_state = 3;
			return 0;			
		}
		
		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_MCC) == 0)	  // MCC turns off
				dbn_cnt ++;		   // Exit the loop when MCC turns off
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	// MCC should not be on
			record_fault(f_MCC_on);
			io_fault = 1;
			chkstart_state = 4;
			return 0;
		}

		setoutp(o_MCC);			// Turn on MCC
		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			setoutp(o_MCC);
			if (rdinp(i_MCC) == 1)
				dbn_cnt ++;			// Exit loop when MCCI is on for 10 counts
			else
				dbn_cnt = 0;
		}

		if (timers[tiofault] >= c_io_dbn_time) 
		{		// if timer expired then MCCI failed off
			record_fault(f_MCC_off);
			io_fault = 1;
			chkstart_state = 5;
			return 0;
		}


		// Check to see if Up to speed SPD is on

		timers[tmotion] = 0;
		dbn_cnt = 0;
		while( (dbn_cnt < c_dbn_cnt) && (timers[tmotion] < (2*fvars[fvydel]))) 
		{
			inctime(1);
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
			{
			 	if((timers[tmotion] >= fvars[fvydel]) && (rdinp(i_MCC) == 1))
					setoutp(o_MST);			// Turn ofn MST for Y-Delta start
			 	else
					clroutp(o_MST);
			}
			else
			{
				setoutp(o_MCC);			// Soft Starter and MC will enable the SPD
			}
			if (rdinp(i_SPD) == 1)
				dbn_cnt ++;						// Exit loop after delta on for 10 counts
			else
				dbn_cnt = 0;
		}

		if (timers[tmotion] >= (2* fvars[fvydel]))
		{			// Up to speed did not come on
			record_fault(f_SPD_off);
			io_fault = 1;
			chkstart_state = 6;
			return 0;
		}


		// Verify that run is off

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_RUN) == 0)	  // Run is off
				dbn_cnt ++;		   // Exit the loop when Run goes off
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	  // Run is on when it should be off
			record_fault(f_run_on);
			io_fault = 1;
			chkstart_state = 7;
			return 0;
		}

		// Verify that run comes on
		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time_short) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			setoutp(o_RUN);
			if (rdinp(i_RUN) == 1)
				dbn_cnt ++;				// Exit loop when RUNI on for 10 counts
			else
				dbn_cnt = 0;
		}
		if (timers[tiofault] >= c_io_dbn_time_short)
		{				// RUNI did not come on
			record_fault(f_run_off);	// run failed off
			io_fault = 1;
			chkstart_state = 8;
			return 0;
		}

		setoutp(o_RUN);		 // Turn on RUN  

		// Verify that SUi is OFF

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SU) == 0)
				dbn_cnt ++;		   // Exit the loop when up is off
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	// UP input failed
			record_fault(f_up_on);
			io_fault = 1;
			chkstart_state = 9;
			return 0;
		}

		// Verify that SUFi is off

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SUF) == 0)
				dbn_cnt ++;		   // Exit the loop when up is off
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	// SUF input failed
			record_fault(f_upf_on);
			io_fault = 1;
			chkstart_state = 10;
			return 0;
		}

		setoutp(o_SU);		// Turn on UP
//		if ((hsf == 1) && (no_high_speed == 0))
//			setoutp(o_SUF);				// Turn on UP fast

		timers[tiofault] = 0;

		while(timers[tiofault] < 5)
		{						// Delay to allow RUNI input to come on	if RUNA output shorted
			inctime(1);
			setoutp(o_SU);
//			if ((hsf == 1) && (no_high_speed == 0))
//				setoutp(o_SUF);
		}


		// Verify that UP is on

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SU) == 1)
				dbn_cnt ++;		   // Exit the loop when up is on
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{		   // UPI should be on at this point
			record_fault(f_up_off);
			io_fault = 1;
			chkstart_state = 11;
			return 0;
		}

		if ((hsf == 1) && (rdinp(i_UTsel) == 1) && (no_high_speed == 0)) 
		{
			// Verify that UF is on

			setoutp(o_SUF);				// Turn on UP fast
			
			timers[tiofault] = 0;
			dbn_cnt = 0;
			while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
			{
				inctime(1);
				if (rdinp(i_SUF) == 1)
					dbn_cnt ++;		   // Exit the loop when up is on
				else
					dbn_cnt = 0;
			}

			if(timers[tiofault] >= c_io_dbn_time) 
			{		   // SUFI should be on at this point
				record_fault(f_upf_off);
				io_fault = 1;
				chkstart_state = 12;
				return 0;
			}
		}
		else
		{
			// Verify that UFI stayted off

			timers[tiofault] = 0;
			dbn_cnt = 0;
			while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
			{
				inctime(1);
				if (rdinp(i_SUF) == 0)
					dbn_cnt ++;		   // Exit the loop when up is off
				else
					dbn_cnt = 0;
			}

			if(timers[tiofault] >= c_io_dbn_time) 
			{	// SUF input failed
				record_fault(f_upf_on);
				io_fault = 1;
				chkstart_state = 13;
				return 0;
			}
		}

		chkstart_state = 14;
		
		return 1;
	}
	else if (cmd == 2)
	{		// run down
	
	
		if (rdinp(i_DNsel) == 0)
		{			// DN must be on to run
			chkstart_state = 15;
			return(0);
		}
			

		if ((firef == 0) && (car_Grp_Stat.c_ids_return_typ == 0) && (medf == 0))
		{
			// FST must be off
			timers[tiofault] = 0;
			dbn_cnt = 0;
			while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
			{
				inctime(1);
				if (rdinp(i_FST) == 0)	  // Fst is off
					dbn_cnt ++;		   // Exit the loop when FST goes oFF
				else
					dbn_cnt = 0;
			}

			if(timers[tiofault] >= c_io_dbn_time) 
			{	// FST failed on
				record_fault(f_FST_failed_on);
				io_fault = 1;
				chkstart_state = 16;
				return 0;			
			}
		}

		// Verify that run is off

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_RUN) == 0)	  // Run is off
				dbn_cnt ++;		   // Exit the loop when Run goes off
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	  // Run is on when it should be off
			record_fault(f_run_on);
			io_fault = 1;
			chkstart_state = 17;
			return 0;
		}
		
		setoutp(o_RUN);	// Turn on RUN: RUNAI and RUNI should go off

		// Verify RUN goes on
		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			setoutp(o_RUN);
			if (rdinp(i_RUN) == 1)
				dbn_cnt ++;		   // Exit the loop when runA goes off
			else
				dbn_cnt = 0;

		}
		if(timers[tiofault] >= c_io_dbn_time) 
		{		// RUNI did not go on
			record_fault(f_run_off);	// run failed off 
			io_fault = 1;
			chkstart_state = 18;
			return 0;
		}

		
		// Verify that DN is off

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SD) == 0)	  // Down run is off
				dbn_cnt ++;		   // Exit the loop when down run goes off
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	   // SDi is on when it should be off
			record_fault(f_dnr_on);
			io_fault = 1;
			chkstart_state = 19;
			return 0;
		}

		// Verify that SDF is off

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SDF) == 0)	  // Down run FAST is off
				dbn_cnt ++;		   // Exit the loop when down run FAST goes off
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	   // DFI is on when it should be off
			record_fault(f_df_on);
			io_fault = 1;
			chkstart_state = 20;
			return 0;
		}

		setoutp(o_SD);		// Turn on DNR
//		if ((hsf == 1) && (no_high_speed == 0))
//			setoutp(o_SDF);		// Turn on DF
		timers[tiofault] = 0;

		while(timers[tiofault] < 5)
		{				// Delay to allow RUNI to turn on if RUN output shorted
			inctime(1);
			setoutp(o_SD);		
//			if ((hsf == 1) && (no_high_speed == 0))
//				setoutp(o_SDF);
		}

		// Verify that DN is on

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SD) == 1)	  // Down run is on
				dbn_cnt ++;		   // Exit the loop when down run goes on
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	   // SDi is off when it should be on
			record_fault(f_dnr_off);
			io_fault = 1;
			chkstart_state = 21;
			return 0;
		}


		if ((hsf == 1) && (rdinp(i_DTsel) == 1) && (no_high_speed == 0))
		{
			// Verify that DF is on

			setoutp(o_SDF);

			timers[tiofault] = 0;
			dbn_cnt = 0;
			while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
			{
				inctime(1);
				if (rdinp(i_SDF) == 1)	  // Down run fast is on
					dbn_cnt ++;		   // Exit the loop when down fast run goes on
				else
					dbn_cnt = 0;
			}

			if(timers[tiofault] >= c_io_dbn_time) 
			{	   // SDFi is off when it should be on
				record_fault(f_df_off);
				io_fault = 1;
				chkstart_state = 22;
				return 0;
			}
		}
		else
		{
			// Verify that SDF stayed off

			timers[tiofault] = 0;
			dbn_cnt = 0;
			while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
			{
				inctime(1);
				if (rdinp(i_SDF) == 0)	  // Down run FAST is off
					dbn_cnt ++;		   // Exit the loop when down run FAST goes off
				else
					dbn_cnt = 0;
			}

			if(timers[tiofault] >= c_io_dbn_time) 
			{	   // SDFi is on when it should be off
				record_fault(f_df_on);
				io_fault = 1;
				chkstart_state = 23;
				return 0;
			}
		}

		chkstart_state = 24;
		
		return 1;
	}
	else
	{
//		chkstart_state = 26;
		return 0;
	}

}

//**********************************
// Check start of LEVELING for Hydro 
//**********************************


int16 chk_lev_start(int16 cmd)
{
	static int16 dbn_cnt;

	if (io_fault != 0)
	{
//		chklevel_state = 1;	do not set here, you overwrite the data
		return 0;
	}

	if (cmd == 1)
	{		// run up


		setoutp(o_MCC);			// Turn on MCC
		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			setoutp(o_MCC);
			if (rdinp(i_MCC) == 1)
				dbn_cnt ++;			// Exit loop when MCCI is on for 10 counts
			else
				dbn_cnt = 0;
		}

		if (timers[tiofault] >= c_io_dbn_time) 
		{		// if timer expired then MCCI failed off
			record_fault(f_MCC_off);
			io_fault = 1;
			chkstart_state = 1;
			return 0;
		}


		// Check to see if Up to speed SPD is on

		timers[tmotion] = 0;
		dbn_cnt = 0;
		while( (dbn_cnt < c_dbn_cnt) && (timers[tmotion] < (2*fvars[fvydel]))) 
		{
			inctime(1);
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
			{
			 	if((timers[tmotion] >= fvars[fvydel]) && (rdinp(i_MCC) == 1))
					setoutp(o_MST);			// Turn ofn MST for Y-Delta start
			 	else
					clroutp(o_MST);
			}
			else
			{
				setoutp(o_MCC);			// Soft Starter and MC will enable the SPD
			}
			if (rdinp(i_SPD) == 1)
				dbn_cnt ++;						// Exit loop after delta on for 10 counts
			else
				dbn_cnt = 0;
		}

		if (timers[tmotion] >= (2* fvars[fvydel]))
		{			// Up to speed did not come on
			record_fault(f_SPD_off);
			io_fault = 1;
			chkstart_state = 2;
			return 0;
		}


		setoutp(o_RUN);		 // Turn on RUN 


		// Verify RUN goes ON
		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			setoutp(o_RUN);
			if (rdinp(i_RUN) == 1)
				dbn_cnt ++;				// Exit loop if RUNI goes off
			else
				dbn_cnt = 0;
		}
		if(timers[tiofault] >= c_io_dbn_time) 
		{		   // RUNI did not go ON
			record_fault(f_run_off);			// RUN failed off 
			io_fault = 1;
			chklevel_state = 3;
			return 0;
		}


		clroutp(o_SD);
		clroutp(o_SDF);

		setoutp(o_SU);		// Turn on UP
		clroutp(o_SUF);		// Turn off UP fast

		timers[tiofault] = 0;

		while(timers[tiofault] < 5)
		{						// Delay to allow RUNI input to come on
			inctime(1);
			setoutp(o_SU);
		}

		// Verify that UP is on

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SU) == 1)
				dbn_cnt ++;		   // Exit the loop when up is on
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{		   // SUi should be on at this point
			record_fault(f_up_off);
			io_fault = 1;
			chklevel_state = 4;
			return 0;
		}

		// Verify that UFI stayted off

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SUF) == 0)
				dbn_cnt ++;		   // Exit the loop when up is off
			else
				dbn_cnt = 0;
		}
		if(timers[tiofault] >= c_io_dbn_time) 
		{	// SUF input failed
			record_fault(f_upf_on);
			io_fault = 1;
			chklevel_state = 5;
			return 0;
		}

		chklevel_state = 6;
		
		return 1;
	}
	else if (cmd == 2)
	{		// run down


		setoutp(o_RUN);	// Turn on RUN: RUNAI and RUNI should go off

		// Verify RUN goes on
		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			setoutp(o_RUN);
			if (rdinp(i_RUN) == 1)
				dbn_cnt ++;		   // Exit the loop when runA goes off
			else
				dbn_cnt = 0;

		}
		if(timers[tiofault] >= c_io_dbn_time) 
		{		// RUNI did not go on
			record_fault(f_run_off);	// run failed off 
			io_fault = 1;
			chklevel_state = 7;
			return 0;
		}


		clroutp(o_SU);
		clroutp(o_SUF);
		
		setoutp(o_SD);		// Turn on SD
		clroutp(o_SDF);		// Turn off SDF

		timers[tiofault] = 0;

		while(timers[tiofault] < 5)
		{				// Delay to allow RUNI to turn on if RUN output shorted
			inctime(1);
			setoutp(o_SD);		
		}


		// Verify that DN is on

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SD) == 1)	  // Down run is on
				dbn_cnt ++;		   // Exit the loop when down run goes on
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	   // SDi is off when it should be on
			record_fault(f_dnr_off);
			io_fault = 1;
			chklevel_state = 8;
			return 0;
		}


		// Verify that DF stayed off

		timers[tiofault] = 0;
		dbn_cnt = 0;
		while ((timers[tiofault] < c_io_dbn_time) && (dbn_cnt < c_dbn_cnt))
		{
			inctime(1);
			if (rdinp(i_SDF) == 0)	  // Down run FAST is off
				dbn_cnt ++;		   // Exit the loop when down run FAST goes off
			else
				dbn_cnt = 0;
		}

		if(timers[tiofault] >= c_io_dbn_time) 
		{	   // SDFi is on when it should be off
			record_fault(f_df_on);
			io_fault = 1;	 
			chklevel_state = 9;
			return 0;
		}
		

		chklevel_state = 10;
		return 1;
	}
	else
	{
//		chklevel_state = 11;	// no state here
		return 0;
	}

}


// *****************************************
// This Procedure clears all motion outputs
// *****************************************
void clrall()
{
	int16 i;

  	clroutp(o_DO);
  	clroutp(o_DC);
  	if (cons[rear] != 0)
  	{
			clroutp(o_DCR);			
			clroutp(o_DOR);
  	}
  	if (cons[Auto_Swing_DO] == 1)
  	{
    	clroutp(o_FSDO);
	  	if (cons[rear] != 0)
			clroutp(o_RSDO);
  	}
   	else if (cons[Auto_Swing_DO] == 2)		// clr auto door open f/r output
  	{
  		for (i=cons[bottomf]; i<=cons[topf];i++)
  		{
  			clroutp(o_ADOF(i));
  			clroutp(o_ADOR(i));
  		}
  	}

 	nudg_buz = 0;
  	rnudg_buz	= 0;
  	clroutp(o_RUN);
  	clroutp(o_MCC);
	lev_startf = 0;

	if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
	{
  		if(rdinp(i_SU) == 0)
	  		clroutp(o_MST);
	}
    if (cons[Micro_Relev] == 1)
	{
		clroutp(o_UPML);
		clroutp(o_DNML);
	}	
  	clroutp(o_SD);
  	clroutp(o_SDF);
  	clroutp(o_SUF);
  	clroutp(o_SU);
  	clrcc(0);
  	clrhc();
  	clrlant();
}

//*********************************************
// Clear direction Arrow procedure
//*********************************************

void clr_dir_arrow (void)
{
	up_dir_arrow = 0;
	dn_dir_arrow = 0;
	clroutp(o_UDAC);
	clroutp(o_DDAC);
	clroutp(o_ATUL);
	clroutp(o_ATDL);
}

// ******************************************
// This is the clear lanterns procedure
// ******************************************
void clrlant()
{
	int16 i;

	lant_up = 0;
	lant_dn = 0;
	rlant_up = 0;
	rlant_dn = 0;
	cab_lant_up = 0;
	cab_lant_dn = 0;
	cab_rlant_up = 0;
	cab_rlant_dn = 0;

	if ((cons[dispis] & 2) != 0)
	{
		if (cons[rear] != 0)
			clroutp(o_LDR);
    	clroutp(o_LD);
	}
	if (((cons[mand] == 0) && (cons[frghtd] == 0) && (cons[RCM_HWDoor] == 0)) || ((cons[dispis] & 2) != 0))
	{
		if (cons[rear] != 0)
			clroutp(o_LDR);
     	clroutp(o_LU);
	}
	clroutp(o_CDL);
	clroutp(o_CUL);
	clroutp(o_CDLR);
	clroutp(o_CULR);
   	if ((cons[Hall_Lant] & 0x01) != 0)
   	{
		if (all_lant_clr == 0)
		{
			all_lant_clr = 1;
			for (i=1;i<=cons[topf];i++)
			{
				clroutp(o_UPL(i));
				clroutp(o_DNL(i));
				if (cons[rear] != 0)
				{	
					clroutp(o_UPLR(i));
					clroutp(o_DNLR(i));
				}
			}
		}
		else 
	   		set_hall_lant();		// clear the lantern output for current position
   	}
  	return;
}

//******************************************************************
// This is the scheduling for the ST times before we do Cycle_run
//******************************************************************
void schedule_st_times(void)
{				
	int16 carry_hour = 0;
	int16 carry_day = 0;
	
	carry_hour = (int16)((rt_minute + (fvars[fvcycrun] % 10) * 6) / 60);
	carry_day = (int16)((carry_hour + rt_hour + fvars[fvcycrun]/10) / 24);
	
	day_st = (int16)(carry_day + rt_day + (rt_hour + fvars[fvcycrun]/10) / 24);
	if (day_st > 364)
		day_st -= 364;
	hour_st = (int16)((carry_hour + rt_hour + fvars[fvcycrun]/10) % 24);
	minute_st = (int16)((rt_minute + 6 * (fvars[fvcycrun] % 10)) % 60);
}

//*******************************************
// This is the Cycle Run Routine
//*******************************************

void cycle_run(void)
{
	if (fvars[fvcycrun] != 0)
	{
		if ( (servf == s_AUTO) && (dcalls == 0) &&         	// in service and no calls
		     ((cons[mand] == 2) || ((doorf == 0) && (rdoorf == 0))) && 		// Car has swing doors or Doors are closed
		     (dirf == 0) && (dpref == 0) )									// No direction flag and No direction preference
		{
			if ((minute_st == rt_minute) && (hour_st == rt_hour) && (day_st >= rt_day))
			{
				schedule_st_times();
				if (position > (((cons[topf] - cons[bottomf]) + 1)/2))
					zone_fl = cons[bottomf];
				else
					zone_fl = cons[topf];
			}
			prev_minute = 61;		// force an update
		}
		else
		{
			if (rt_minute != prev_minute)
			{
				schedule_st_times();			
			}
			prev_minute = rt_minute;
		}
	}
	else
	{
		if (rt_minute != prev_minute)
		{
			schedule_st_times();
		}
		prev_minute = rt_minute;
	}
}


//*******************************************
// This is the Diagnostics Mode
//*******************************************
void diag(void)
{


	trafficLogControl();

	if ((inctime_count & 0x01) == 0)
   		Refresh_LCD();
   	
   	LCD_Buffer_to_spi();
   	

	if (fvars[fvcomdiag1] == 1)
		comm_debug(USR1COM);
	else if ((fvars[fvcomdiag1] == 2) || (fvars[fvcomdiag1] == 4) || (fvars[fvcomdiag1] == 5) || 
				(fvars[fvcomdiag1] == 6) || (fvars[fvcomdiag1] == 7)) 
		galcom(USR1COM);
	else if ((fvars[fvcomdiag1] == 3) && (fvars[fvcomdiag2] != 3))
		DL20_comm(USR1COM);
	else
		comm_diag(USR1COM);
	
	if (fvars[fvcomdiag2] == 1)
		comm_debug(USR2COM);
	else if ((fvars[fvcomdiag2] == 2) || (fvars[fvcomdiag2] == 4) || (fvars[fvcomdiag2] == 5) || 
				(fvars[fvcomdiag2] == 6) || (fvars[fvcomdiag2] == 7)) 
		galcom(USR2COM);
	else if ((fvars[fvcomdiag2] == 3) && (fvars[fvcomdiag1] != 3))
		DL20_comm(USR2COM);
	else
	{
			
		if (Dispatcher == cons[carnmb])
		{
			if ((cons[ids] & 1) != 0)
			{
				ids_comm();
			}
			else
			{
				ids_uppeak = 0;
				ids_dnpeak = 0;
				ids_alt_fl_sec = 0;
				ids_ncu = 0;
				ids_alt_lby = 0;
				ids_alt_park = 0;
				g_ids_fsx = 0;
				comm_diag(USR2COM);
			}
		}
		else
		{
			ids_refresh = 1;
			ids_uppeak = 0;
			ids_dnpeak = 0;
			ids_alt_fl_sec = 0;
			ids_ncu = 0;
			ids_alt_lby = 0;
			ids_alt_park = 0;
			
			ui_uppeak = 0;
			ui_dnpeak = 0;
			ui_alt_fl_sec = 0;
			ui_ncu = 0;
			ui_alt_lby = 0;
			ui_alt_park = 0;
		}

		// Restart the com to enable or disable the RTS (485 Transmitter)
		if (prev_disp_com2 != Dispatcher)
			Restart_Com(USR2COM);
		prev_disp_com2 = Dispatcher;
	}
	
  	if(Current_Fault != 0)
  	{
    	if(timers[tfltled] >= 300)
	    	Current_Fault = 0;
  	}
}

// **********************************************
// Turn on the diagnostic Led
// **********************************************

void diag_led_on(void)
{
//	if (fvars[fvcputiming] == 0)
	//	SIU.GPDO[Z6_LED].R = 0x01; // tagwbg 14
	
	if (aps_sel_can_online == 1)
		setoutp(o_SELCM);
	if ((nts_spi_online == 1) && (NTS_spi.Status1.B.ced_can_flt == 0) && (NTS_spi.Status1.B.sel_online == 1))
		setoutp(o_NTSCM);


}

// **********************************************
// Turn off the diagnostic Led
// **********************************************

void diag_led_off(void)
{
//	if (fvars[fvcputiming] == 0)
//		SIU.GPDO[Z6_LED].R = 0x00; // tagwbg 14
	
	clroutp(o_SELCM);
	clroutp(o_NTSCM);
}

// **********************************************
// This is direction selection procedure
// **********************************************
void dirsel()
{
int16 i,cbelow=0,cabove=0;

// int16 ocbelow=0,ocabove=0;
int16 recall_fl;

	write_trace(TR_DIRSEL);


// if earth quake
  	if(cons[equake] != 0)                 // is earth quake feature enabled
  	{
		if (eqf == 4)
		{
			dpref = 0;
			dcalls = 0;
			set_dir_arrow();
			return;
		}
  	}

	if (((cons[Car_Sw] & 1) != 0) && (cons[att] == 0))
	{
		if ((rdinp(i_ATTUP) == 1) && (position != cons[topf]))
			cabove = 1;

		if ((rdinp(i_ATTDN) == 1) && (position != cons[bottomf]))
			cbelow = 1;

    	if((cabove == 0) && (cbelow == 0))
      		dcalls = 0;
    	if(cabove == 1)
      		dcalls = 1;
    	if(cbelow == 1)
      		dcalls = 2;
    	if((cbelow == 1)&&(cabove == 1))
      		dcalls = 3;

		if ((servf == s_ELEV_OFF_CS) && (firef == 0) && (rfiref == 0))
			dcalls = 0;

		if ((empf != 0) && (empf != ep_select) && (empf != ep_recall))
			dcalls = 0;

    	setpref();
    	return;
	}

// Emergency Power Recall Operation
	if ((empf != 0) && (empf != ep_select))			// on emergency power and not selected to run automatic
	{
		if (empf == ep_recall)	// Recall Car 
		{
			
		    park_fl = 0;
			lby_req_fl = 0;

			if (firef >= 4)
			{					  // on phase 2
		    	if( ((firef == 5) || (firef == 6)) && ((cons[Australia] == 0) || (fs2_start == 1) || ((procf >= 3) && (procf <= 11)) ) )   // normal phase 2 operation
		    	{									  // and no emp seq operaiton
		      		for(i=cons[bottomf]; i <= cons[topf]; i++)
		      		{
						if(carcb[i] == 1)
						{
							if ((i > position) && ((cons[EMPop] & 0x02) != 0))
				    			cabove = 1;
				  			if(i < position)
				    			cbelow = 1;
						}
						if(cons[rear] != 0)
						{
							if(rcarcb[i] == 1)
							{
								if ((i > position) && ((cons[EMPop] & 0x02) != 0))
					    			cabove = 1;
					  			if(i < position)
					    			cbelow = 1;
							}
						}
		      		}
		      		if((cabove == 0) && (cbelow == 0))
						dcalls = 0;
		      		if(cabove == 1)
						dcalls = 1;
		      		if(cbelow == 1)
						dcalls = 2;
		      		if((cbelow == 1)&&(cabove == 1))
						dcalls = 3;
		      		setpref();
		    	}
		    	else        // door hold operation or power went off
		    	{
		      		dpref = 0;
		      		dcalls = 0;
		    	}
			}
			else if ((cons[ansi] >= 2007) && ((servf == s_IND) || (servf == s_ATT)))
			{			 // Allow independent or attendant car to run during recall
	      		for(i=cons[bottomf]; i <= cons[topf]; i++)
	      		{
					if(carcb[i] == 1)
					{
			 			if(i > position)
			    			cabove = 1;
			  			if(i < position)
			    			cbelow = 1;
					}
					if(cons[rear] != 0)
					{
						if(rcarcb[i] == 1)
						{
				 			if(i > position)
				   	 			cabove = 1;
				  			if(i < position)
				    			cbelow = 1;
						}
					}
	      		}
	      		if((cabove == 0) && (cbelow == 0))
					dcalls = 0;
	      		if(cabove == 1)
					dcalls = 1;
	      		if(cbelow == 1)
					dcalls = 2;
	      		if((cbelow == 1)&&(cabove == 1))
					dcalls = 3;
	      		setpref();
	      		return;
			}
			else if (medf == 3)
			{
			    dpref = 0;        // no preference
			    dcalls = 0;       // no calls
			}
			else
			{	   // normal operation or on phase 1

				if (medf == 1) 
					recall_fl = fvars[fvmedfl];
				else if (medf == 2)
					recall_fl = position;
				else if (firef == 1)
					recall_fl = fvars[fvfs1f];
				else if (firef == 2)
					recall_fl = fvars[fvfs1af];
				else if (firef == 3)
					recall_fl = position;
				else
					recall_fl = fvars[fvepfl];

			    if(position > recall_fl) // elevator higher than main eggress
			    {
			      	dpref = 2;        // down preference
			      	dcalls = 2;       // call below
			    }
				else if ((cons[EMPop] & 0x02) != 0)
				{
				    if(position < recall_fl) // elevator lower than recall floor
				    {
				      	dpref = 1;        // up preference
				      	dcalls = 1;       // call above
				    }
				    else if(position == recall_fl) // elevator at the recall floor
				    {
				      	dpref = 0;        // no preference
				      	dcalls = 0;       // no calls
				    }
				}
			    else
			    {
			    	 if((position == recall_fl) && (dn_level == 0) && (rdinp(i_DTsel) == 1)) 
				    {
				      	dpref = 2;        // down preference
				      	dcalls = 2;       // call below
						dirf = 2;
				    }
				    else if((position == recall_fl) || (rdinp(i_DTsel) == 0))  // elevator at the recall floor
				    {
				      	dpref = 0;        // no preference
				      	dcalls = 0;       // no calls
				    }
					else if(rdinp(i_DTsel) == 1) // not at botom floor
				    {
				      	dpref = 2;        // down preference
				      	dcalls = 2;       // call below
						dirf = 2;
				    }
				}
				if ((cons[ansi] < 2007) && ((procf >= 14) && (procf <= 16)) && ((ind() == 1) || 
					((cons[att] != 0) && (rdinp(i_ATT) == 1))))
				{			 // Prevent independent or attendant car from running during recall	when at the floor
					dpref = 0;
					dcalls = 0;
					dirf = 0;
				}
			    stops[0] = recall_fl;
			}
		}
		else
		{
	      	dpref = 0;        // no preference
	      	dcalls = 0;       // no calls
		}
	    return;
	}

// if on low or hot oil on a hydro
  	if ((stallf != 0) || (hotoilf != 0))
  	{
		park_fl = 0;
		lby_req_fl = 0;

		if (firef >= 4)
		{					  // on phase 2
	    	if( ((firef == 5) || (firef == 6)) && ((cons[Australia] == 0) || (fs2_start == 1) || ((procf >= 3) && (procf <= 11)) ) )   // normal phase 2 operation
	    	{
				cabove = 0;
	      		for(i=cons[bottomf]; i <= cons[topf]; i++)
		      	{
					if(carcb[i] == 1)
					{
			  			if(i < position)
			    			cbelow = 1;
					}
					if(cons[rear] != 0)
					{
						if(rcarcb[i] == 1)
						{
				  			if(i < position)
				    			cbelow = 1;
						}
					}
	      		}
	      		if(cbelow == 1)
					dcalls = 2;
				else
					dcalls = 0;
	      		setpref();
	    	}
	    	else        // door hold operation or power went off
	    	{
	      		dpref = 0;
	      		dcalls = 0;
	    	}
		}
		else if (medf == 3)
		{
		    dpref = 0;        // no preference
		    dcalls = 0;       // no calls
		}
		else
		{	   // normal operation or on phase 1

			if (medf == 1) 
				recall_fl = fvars[fvmedfl];
			else if (medf == 2)
				recall_fl = position;
			else if (firef == 1)
				recall_fl = fvars[fvfs1f];
			else if (firef == 2)
				recall_fl = fvars[fvfs1af];
			else if (firef == 3)
				recall_fl = position;
			else
				recall_fl = cons[bottomf];

		    if(position > recall_fl) // elevator higher than main eggress
		    {
		      	dpref = 2;        // down preference
		      	dcalls = 2;       // call below
				dirf = 2;
		    }
		    else if((position == recall_fl) && (dn_level == 0) && (rdinp(i_DTsel) == 1)) 
		    {
		      	dpref = 2;        // down preference
		      	dcalls = 2;       // call below
				dirf = 2;
		    }
		    else if((position == recall_fl) || (rdinp(i_DTsel) == 0))  // elevator at the recall floor
		    {
		      	dpref = 0;        // no preference
		      	dcalls = 0;       // no calls
		    }
			else if(rdinp(i_DTsel) == 1) // not at botom floor
		    {
		      	dpref = 2;        // down preference
		      	dcalls = 2;       // call below
				dirf = 2;
		    }
			stops[0] = recall_fl;

		}
	    return;
	}


// if reset
  	if(servf == s_RESET_UP)       // reset dive up
  	{
	    park_fl = 0;
		lby_req_fl = 0;
	    dpref = 1;          // up preference
	    dcalls = 1;         // call above
	    stops[0] = cons[topf];
	    return;
  	}
  	if(servf == s_RESET_DN)       // reset dive down
  	{
	    park_fl = 0;
		lby_req_fl = 0;
	    dpref = 2;          // down preference
	    dcalls = 2;         // call below
	    stops[0] = cons[bottomf];
	    return;
  	}

	if (cons[cblue] == 1)
	{
    	if (hsvf == 1)    // Hospital Emergency Operation
    	{
      		for(i=cons[bottomf]; i <= cons[topf]; i++)
      		{
				if(carcb[i] == 1)
				{
		 			if(i > position)
		    			cabove = 1;
		  			if(i < position)
		    			cbelow = 1;
				}
				if(cons[rear] != 0)
				{
					if(rcarcb[i] == 1)
					{
			 			if(i > position)
			   	 			cabove = 1;
			  			if(i < position)
			    			cbelow = 1;
					}
				}
      		}
      		if((cabove == 0) && (cbelow == 0))
				dcalls = 0;
      		if(cabove == 1)
				dcalls = 1;
      		if(cbelow == 1)
				dcalls = 2;
      		if((cbelow == 1)&&(cabove == 1))
				dcalls = 3;
      		setpref();
      		return;
    	}
    	else if((codebf != 0) && (fvars[fvcboverfs] != 0))      // On code blue operation and priority over fire service
    	{
      		park_fl = 0;
			lby_req_fl = 0;
      		if((codebf == 1)||(codebf==2))   // On code blue returning to emergency
      		{
				if (codebfl == 0)
				{
	  				dpref = 0;
	  				dcalls = 0;
				}
				else if(position > codebfl)  // elevator higher than emergency
				{
	  				dpref = 2;        // down preference
	  				dcalls = 2;       // call below
	  				stops[0] = codebfl;
				}
				else if(position < codebfl)  // elevator lower than emergency
				{
	  				dpref = 1;
	  				dcalls = 1;
	  				stops[0] = codebfl;
				}
				else if(position == codebfl) // elevator at emergency floor
				{
	  				dpref = 0;
	  				dcalls = 0;
				}
				set_dir_arrow();
				return;
      		}
      		return;
    	}
	}

	if (medf != 0)
	{
		if (medf == 1)
		{
	    	park_fl = 0;
			lby_req_fl = 0;
	    	if(position > fvars[fvmedfl]) // elevator higher than main eggress
	    	{
	      		dpref = 2;        // down preference
	      		dcalls = 2;       // call below
	    	}
	    	if(position < fvars[fvmedfl]) // elevator lower than main eggress
	    	{
	      		dpref = 1;        // up preference
	      		dcalls = 1;       // call above
	    	}
	    	if(position == fvars[fvmedfl]) // elevator at the main eggress
	    	{
	      		dpref = 0;        // no preference
	      		dcalls = 0;       // no calls
	    	}
	    	stops[0] = fvars[fvmedfl];
			set_dir_arrow();
	    	return;
		}
	}
  	else if(firef == 1)
  	{	// if fire service phase 1 and main eggress
    	park_fl = 0;
		lby_req_fl = 0;
    	if(position > fvars[fvfs1f]) // elevator higher than main eggress
    	{
      		dpref = 2;        // down preference
      		dcalls = 2;       // call below
    	}
    	if(position < fvars[fvfs1f]) // elevator lower than main eggress
    	{
      		dpref = 1;        // up preference
      		dcalls = 1;       // call above
    	}
    	if(position == fvars[fvfs1f]) // elevator at the main eggress
    	{
      		dpref = 0;        // no preference
      		dcalls = 0;       // no calls
    	}
    	stops[0] = fvars[fvfs1f];
    	return;
  	}
// if fire service phase 1 and alternate eggress
  	else if(firef == 2)
  	{
    	park_fl = 0;
		lby_req_fl = 0;
    	if(position > fvars[fvfs1af]) // elevator higher than alternate eggress
    	{
      		dpref = 2;        // down preference
      		dcalls = 2;       // call below
    	}
    	if(position < fvars[fvfs1af]) // elevator lower than alternate eggress
    	{
      		dpref = 1;        // up preference
      		dcalls = 1;       // call above
    	}
    	if(position == fvars[fvfs1af]) // elevator at the alternate eggress
    	{
      		dpref = 0;        // no preference
      		dcalls = 0;       // no calls
    	}
    	stops[0] = fvars[fvfs1af];
   		return;
  	}
// if fire service phase 2
  	if(firef >= 3)
  	{
    	park_fl = 0;
		lby_req_fl = 0;
    	if( ((firef == 5) || (firef == 6)) && ((cons[Australia] == 0) || (fs2_start == 1) || ((procf >= 3) && (procf <= 11)) ) )   // normal phase 2 operation
    	{
      		for(i=cons[bottomf]; i <= cons[topf]; i++)
      		{
				if(carcb[i] == 1)
				{
		 			if(i > position)
		    			cabove = 1;
		  			if(i < position)
		    			cbelow = 1;
				}
				if(cons[rear] != 0)
				{
					if(rcarcb[i] == 1)
					{
			 			if(i > position)
			   	 			cabove = 1;
			  			if(i < position)
			    			cbelow = 1;
					}
				}
      		}
      		if((cabove == 0) && (cbelow == 0))
				dcalls = 0;
      		if(cabove == 1)
				dcalls = 1;
      		if(cbelow == 1)
				dcalls = 2;
      		if((cbelow == 1)&&(cabove == 1))
				dcalls = 3;
      		setpref();
      		return;
    	}
    	else        // door hold operation or power went off
    	{
      		dpref = 0;
      		dcalls = 0;
      		return;
    	}
  	}

  	if(medf >= 2)
  	{	   // Medical Emergency Service at the floor or running
    	park_fl = 0;
		lby_req_fl = 0;
    	if ((medf == 3) && (rdinp(i_EMS) == 1))    // Medical Emergency Operation
    	{
      		for(i=cons[bottomf]; i <= cons[topf]; i++)
      		{
				if(carcb[i] == 1)
				{
		 			if(i > position)
		    			cabove = 1;
		  			if(i < position)
		    			cbelow = 1;
				}
				if(cons[rear] != 0)
				{
					if(rcarcb[i] == 1)
					{
			 			if(i > position)
			   	 			cabove = 1;
			  			if(i < position)
			    			cbelow = 1;
					}
				}
      		}
      		if((cabove == 0) && (cbelow == 0))
				dcalls = 0;
      		if(cabove == 1)
				dcalls = 1;
      		if(cbelow == 1)
				dcalls = 2;
      		if((cbelow == 1)&&(cabove == 1))
				dcalls = 3;
      		setpref();
      		return;
    	}
    	else        // door hold operation 
    	{
      		dpref = 0;
      		dcalls = 0;
      		return;
    	}
  	}

// if code blue operation
  	if(cons[cblue] != 0)  // Code blue operation is enabled
  	{
    	if(codebf != 0)     // On code blue operation
    	{
      		park_fl = 0;
			lby_req_fl = 0;
      		if((codebf == 1)||(codebf==2))   // On code blue returning to emergency
      		{
				if (codebfl == 0)
				{
	  				dpref = 0;
	  				dcalls = 0;
				}
				else if(position > codebfl)  // elevator higher than emergency
				{
	  				dpref = 2;        // down preference
	  				dcalls = 2;       // call below
	  				stops[0] = codebfl;
				}
				else if(position < codebfl)  // elevator lower than emergency
				{
	  				dpref = 1;
	  				dcalls = 1;
	  				stops[0] = codebfl;
				}
				else if(position == codebfl) // elevator at emergency floor
				{
	  				dpref = 0;
	  				dcalls = 0;
				}
				set_dir_arrow();
				return;
      		}
      		return;
    	}
  	}

// if priority service (vip) operation
  	if (cons[VipSvc] != 0)  // Vip operation is enabled
  	{
    	if (vipf != 0)     // On Vip operation
    	{
      		park_fl = 0;
			lby_req_fl = 0;
      		if ((vipf == 1) || (vipf == 2))   // On vip returning to emergency
      		{
				if (position > vipfl)  // elevator higher than emergency
				{
	  				dpref = 2;        // down preference
	  				dcalls = 2;       // call below
	  				stops[0] = vipfl;
				}
				if (position < vipfl)  // elevator lower than emergency
				{
	  				dpref = 1;
	  				dcalls = 1;
	  				stops[0] = vipfl;
				}
				if (position == vipfl) // elevator at emergency floor
				{
	  				dpref = 0;
	  				dcalls = 0;
				}
				set_dir_arrow();
				return;
      		}
    	}
  	}

	if ((cop_can_error == 1) || (nts_spi_error == 1) ||
		(hardware_init_fault == 1) || (door_motor_ovl == 1) ||
		((aps_sel_can_error == 1) && (cons[sel_type] == 4)) ||
		((sel_can_error == 1) && (cons[sel_type] == 2)))
	{			   // Top of car communications error
    	park_fl = 0;
		lby_req_fl = 0;
   		dcalls = 0;
    	setpref();
    	return;
	}

// if independent operation or load weighing bypass or vip
  	if((servf == s_IND) || (servf == s_LWBP) || (servf == s_VIP)) // answer car calls only
  	{
    	park_fl = 0;
		lby_req_fl = 0;
		zone_fl = 0;
//		onward_cc = 0;
    	for(i=cons[bottomf]; i <= cons[topf]; i++)
    	{
      		if(carcb[i] == 1)
      		{
				if(i > position)
	  				cabove = 1;
				if(i < position)
	  				cbelow = 1;
      		}
      		if(cons[rear] != 0)
      		{
	      		if(rcarcb[i] == 1)
	      		{
					if(i > position)
		  				cabove = 1;
					if(i < position)
		  				cbelow = 1;
	      		}
      		}
    	}
    	if((cabove == 0) && (cbelow == 0))
      		dcalls = 0;
    	if(cabove == 1)
      		dcalls = 1;
    	if(cbelow == 1)
      		dcalls = 2;
    	if((cbelow == 1)&&(cabove == 1))
      		dcalls = 3;
    	setpref();
    	return;
  	}

// if full automatic operation

	cc_above_lby = 0;
	cc_below_lby = 0;

  	for(i=cons[bottomf]; i <= cons[topf]; i++)
  	{
    	if(carcb[i] == 1)  // There is a car call
    	{
      		if(i > position) // call above car
				cabove = 1;
      		if(i < position) // call below car
				cbelow = 1;
    	}
    	if(cons[rear] != 0)
    	{
	    	if(rcarcb[i] == 1)  // There is a rear car call
	    	{
	      		if(i > position) // call above car
					cabove = 1;
	      		if(i < position) // call below car
					cbelow = 1;
	    	}
    	}
		if (i > lobby_fl) 
		{
			if ((carcb[i] == 1) || ((cons[rear] != 0) && (rcarcb[i] == 1)))
				cc_above_lby = 1;
		}
		if (i < lobby_fl) 
		{
			if ((carcb[i] == 1) || ((cons[rear] != 0) && (rcarcb[i] == 1)))
				cc_below_lby = 1;
		}
    	if((upcb[i] == 1)&&(i != cons[topf]))   // There is a up hall call
    	{
			if (ncu_door_seq == 3)
			{			 // only at the lobby on next up
      			if(i > position) // call above car
					cabove = 1;
			}
			else if (i > position)
				cabove = 1;
			else if (((i == position) && ((nudgst & 0x01) == 0)) && ((manual_door != 1) || (cons[frghtd] != 0)))
				cabove = 1;
      		if(i < position) // call below car
				cbelow = 1;
    	}
		if ((lby_req_fl == i) && (i != cons[topf]))	// There is an up request at the lobby
		{
			if (ncu_door_seq == 3)
			{			 // only at the lobby on next up
      			if(i > position) // call above car
					cabove = 1;
			}
			else if (i > position)
				cabove = 1;
			else if (((i == position) && ((nudgst & 0x01) == 0)) && ((manual_door != 1) || (cons[frghtd] != 0)))
				cabove = 1;
      		if(i < position) // call below car
				cbelow = 1;
		}
    	if(cons[rear] != 0)
    	{
	    	if((rupcb[i] == 1)&&(i != cons[topf]))   // There is a rear up hall call
	    	{
	      		if(i > position) // call above car
					cabove = 1;
				else if (((i == position) && ((rnudgst & 0x01) == 0)) && ((manual_rdoor != 1) || (cons[frghtd] != 0)))
					cabove = 1;
	      		if(i < position) // call below car
					cbelow = 1;
	    	}
    	}
    	if((downcb[i] == 1)&&(i != cons[bottomf]))   // There is a down hall call
    	{
      		if(i > position) // call above car
				cabove = 1;
      		if(i < position) // call below car
				cbelow = 1;
			else if (((i == position) && ((nudgst & 0x02) == 0))  && ((manual_door != 1) || (cons[frghtd] != 0)))
			   cbelow = 1;
    	}
    	if(cons[rear] != 0)
    	{
	    	if((rdowncb[i] == 1)&&(i != cons[bottomf]))   // There is a down hall call
	    	{
	      		if(i > position) // call above car
					cabove = 1;
	      		if(i < position) // call below car
					cbelow = 1;
				else if (((i == position) && ((rnudgst & 0x02) == 0)) && ((manual_rdoor != 1) || (cons[frghtd] != 0)))
					cbelow = 1;
	    	}
    	}
  	}

  	if((cabove == 0) && (cbelow == 0))
    	dcalls = 0;
  	if(cabove == 1)
    	dcalls = 1;
  	if(cbelow == 1)
    	dcalls = 2;
  	if((cbelow == 1)&&(cabove == 1))
    	dcalls = 3;


//	onward_calls = (ocabove	& 1) | ((ocbelow & 1) << 1);

  	if( (dcalls != 0) && (park_fl != 0) )
		park_fl = 0;

  	setpref();

// if attendant operation
  	if(cons[att] != 0)
  	{
    	if(servf == s_ATT)
    	{
	      	if((rdinp(i_ATTUP) == 1) && (position != cons[topf]) && ((dcalls & 0x1) == 1))
				dpref = 1;
	      	if((rdinp(i_ATTDN) == 1)	&& (position != cons[bottomf]) && ((dcalls & 0x2) == 0x2))
				dpref = 2;
		  	if (dpref != 0)
		    	set_dir_arrow();
    	}
  	}

// viscosity control return to lobby floor
	if((Drive_Type[cons[carnmb]] == 1) || (Drive_Type[cons[carnmb]] == 9))
	{
	  	if( (servf > s_LOW_OIL) && (dpref == 0) && (dcalls == 0) &&
	      	(((rdinp(i_TPL) == 1) && (fvars[fvtpl] == 0)) || ((rdinp(i_TPL) == 0) && (fvars[fvtpl] == 1))) )
	  	{
			if(position > lobby_fl)
			{
				dpref = 2;
				dcalls = 2;
			}
			if(position < lobby_fl)
			{
				dpref = 1;
				dcalls = 1;
			}
			return;
	  	}
	}

// if homing
  	if( (servf > s_ATT) && (dpref == 0) && (dcalls == 0))
	{
		if ((park_fl >= cons[bottomf]) &&
      		(park_fl <= cons[topf]) )
  		{
	    	if(position > park_fl)       // car above homeing floor
	    	{
	      		servf = s_HOMING;
	      		dpref = 2;
	      		dcalls = 2;
	      
	      		stops[0] = (park_fl);
	//	  		set_dir_arrow();
	    	}
	   	 	if(position < park_fl)       // car below homeing floor
	    	{
	      		servf = s_HOMING;
	      		dpref = 1;
	      		dcalls = 1;
	      		stops[0] = (park_fl);
	//	  		set_dir_arrow();
	    	}
		    if(position == park_fl)      // car at homeing floor
		    {
				if ((procf >= 14) && (procf <= 16))	// we are at the floor
			        servf = s_AUTO; // Car is at homing floor so go back to automatic
	    		else
	    	    	servf = s_HOMING;
	      		dpref = 0;
	      		dcalls = 0;
		    }
		}
		else if (servf == s_HOMING)
			servf = s_AUTO;
	}
	else if ((servf == s_HOMING) && (park_fl == 0))
		servf = s_AUTO;

  	if( ( (servf != 0) || 
  		  ((servf == s_ADOFF) && 
  			  (rdinp(i_AD) == 0) || 
  		  	  (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) ||
  		  	  (((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ad_off == 1))) ||
  		  ((servf == s_RST_JACK) && (rhjackf >= 2) && (cons[Reset_Jack] == 1))  
  	    ) && (dpref == 0) && (dcalls == 0))
	{
		if ((zone_fl >= cons[bottomf]) &&
      		(zone_fl <= cons[topf]) )
  		{
	    	if(position > zone_fl)      // car above homing floor
	    	{
	      		dpref = 2;
	      		dcalls = 2;
	      
	      		stops[0] = zone_fl;
		  		set_dir_arrow();
	    	}
	   	 	if(position < zone_fl)       // car below homing floor
	    	{
	      		dpref = 1;
	      		dcalls = 1;
	      		stops[0] = zone_fl;
		  		set_dir_arrow();
	    	}
		    if(position == zone_fl)      // car at homing floor
		    {
				if ((procf >= 14) && (procf <= 16))	// we are at the floor
			        zone_fl = 0;
	      		dpref = 0;
	      		dcalls = 0;
		  		set_dir_arrow();
		    }
		}
	}

  	return;
}

//************************************
// Turn off Watchdog and all interrups
//************************************

void dog_ints_off(void)
{
/*
	int16 i;
	disable_dog();
  	_disable();
  	i = inp(0x21);
  	i = (i | 0x80);       // Masking interupt #7 Pg #3-140
  	outp(0x21,(uint8)i);
  	outp(c_timer_en_addr,0);		// Turn off timer enable output
  	_enable();
  	_disable();
  	for(i=0; i<=3; i++)
  	{
		// disable interrupts for all other ports
      	outp(0x21,(inp(0x21) | (uint8)(~com[i][1])));
     	// send any other control signals to uart
     	outp(com[i][7], 0x0); // disable interrupts (bit3), and pins low
     	outp(com[i][4], 0x00); //disable recieve interupt Pg 4-9
     	// thoroughly flush receiver input buffer
     	inp(com[i][8]);
     	inp(com[i][8]);  // clear any int_pending line status
     	inp(com[i][0]);
     	inp(com[i][0]);  // get rid of any old data
     	inp(com[i][0]);
  	}
  	_enable();
*/
}

void dog_ints_on(void)
{
	enable_dog();
  	inittim();    // Setup the hardware for a 1/100th of a second interupt
}

//*********************************************************
// Status of the door locks	(0=locks open, 1= locks closed)
// ********************************************************

int16 door_locks(void)
{		
	if (cons[topf] > 2)
	{
		if ( (rdinp(i_DLT) == 1) && (rdinp(i_DLB) == 1) && 
			 (rdinp(i_DLT_1) == 1) && (rdinp(i_DLB_1) == 1) && 
			 (rdinp(i_DLM) == 1) && (rdinp(i_DLM_1) == 1) &&
			 ((cons[rear] == 0) || ((rdinp(i_RLM) == 1) && (rdinp(i_RLM_1) == 1))) &&
			 (((cons[mand] == 0) && (cons[frghtd] == 0) && (cons[Australia] == 0)) || (man_dcc() == 1)) ) 
			return 1;
		else
			return 0;
	}
	else
	{	  // special case - only two floors
		if ( (rdinp(i_DLT) == 1) && (rdinp(i_DLB) == 1) && 
			(rdinp(i_DLT_1) == 1) && (rdinp(i_DLB_1) == 1) && 
			(((cons[access_type] & 0x0C) == 0) || ((rdinp(i_DLM) == 1) && (rdinp(i_DLM_1) == 1))) && 
			((cons[rear] == 0) || ((rdinp(i_RLM) == 1) && (rdinp(i_RLM_1) == 1))) &&
			(((cons[mand] == 0) && (cons[frghtd] == 0) && (cons[Australia] == 0)) || (man_dcc() == 1)) ) 
			return 1;
		else
			return 0;
	}
}
	


//************************************
// Set service flag for elevator off
//************************************
void elevator_off (void)
{
	int16 fl;
	bool heof_or_ceof_or_eof_enabled;

	bool heof_enable;
	bool heof_active;
	bool heof_latched;

	bool ceof_enable;
	bool ceof_active;
	bool ceof_latched;

	bool eof_enable;
	bool eof_active;
		
	heof_enable = ((cons[Elev_off] & 2) != 0);
	heof_active = ((rdinp(i_HEOF) == 1) && ((fvars[fvheloff3] & 0x01) == 0)) ||
				  ((rdinp(i_HEOF) == 1) && ((fvars[fvheloff3] & 0x01) != 0) && (safe() == 1));
	heof_latched = ((latch_HEOF == 1) && ((cons[Elev_off] & 4) != 0));

	eof_enable = ((cons[Elev_off] & 1) != 0);
	eof_active = (rdinp(i_ELOF) == 1);
	
	ceof_enable = ((cons[Car_Elev_off] & 1) != 0);
	ceof_active = (rdinp(i_CEOF) == 1);
	ceof_latched = ((latch_CEOF == 1) && ((cons[Car_Elev_off] & 2) != 0));
	
	
	
	heof_or_ceof_or_eof_enabled = heof_enable || ceof_enable || eof_enable;
	
	if ( heof_or_ceof_or_eof_enabled || ((cons[ids] & 1) != 0) || ((cons[Galileo] & 1) != 0))
	{
	 	if ((firef == 0) && (rfiref == 0) && (codebf == 0) && (vipf == 0) && (eqf == 0) && (medf == 0))
		{
			//LOGIC FOR EOF
			if ((rdinp(i_ELOF) == 1) && ((cons[Elev_off] & 1) != 0) && ((cons[loadw] & 1) == 0))
			{
				if (servf == s_AUTO) 
					servf = s_ELEV_OFF_CS;
			}
			else if (servf == s_ELEV_OFF_CS)
			{
				servf = s_AUTO;
			}

			//LOGIC FOR HEOF
			if (( heof_enable && (heof_active || heof_latched)) || 
				(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_heof == 1)) ||
				(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_return_fl != 0)) || 
				(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_heof == 1))) 
			{
				if (servf == s_AUTO)
				{
					servf = s_ELEV_OFF_HS;	// take car out of group service
				}
				
				if ((cons[Elev_off] & 4) != 0)		// elevator off with reset
				{
					if ((fvars[fvheofovrind] & 0x02) != 0)
					{
						if ((rdinp(i_HEOF) == 1) && (servf == s_ATT))		// on att and heof
						{
							if (timers[theofovratt] > 10)		// put the elevator on heof after 1 sec panic button delay
							{
								heof_over_att = 1;
								servf = s_ELEV_OFF_HS;
							}
						}
					}
				}
						
				if((cons[Elev_off] & 8) != 0) // ELOO control
				{
					if(((fvars[fvheloff2] & 0x02) != 0)&&(latch_HEOF == 1))
					{
						setoutp(o_HEOFL);
					}
					else if ((latch_HEOF == 1) && (timers[tsec] != 0))
					{	
							setoutp(o_HEOFL);
					}
					else
					{
							clroutp(o_HEOFL);
					}
				}
				if (((cons[Elev_off] & 4) != 0)  && (timers[theoff] >= 10))
					latch_HEOF = 1;

				if(car_Grp_Stat.c_ids_return_fl != 0)
				{
					fl = car_Grp_Stat.c_ids_return_fl;
					if ((fl < cons[bottomf]) || (fl > cons[topf]))
						fl = cons[bottomf];
					if ((position != fl) && (servf == s_ELEV_OFF_HS))
		  			{
						clrcc(0);
						rear_slowdown &= ~cc_do;
						front_slowdown &= ~cc_do;
						zone_fl = fl;
						set_carcall(cons[carnmb],fl);
					}						
				}					
				else if ((fvars[fvheloff] & 0x01) != 0)
				{						// Recall Car
					if (fvars[fvheoffl] == 0)
						fl = lobby_fl;
					else if((fvars[fvheoffl] < cons[bottomf]) || (fvars[fvheoffl] > cons[topf]))
						fl = cons[bottomf];
					else
						fl = fvars[fvheoffl];
					
					if ((position != fl) && (servf == s_ELEV_OFF_HS))
		  			{
						if((fvars[fvheloff2] & 0x01) != 0)
						{
							clrcc(0);
							rear_slowdown &= ~cc_do;
							front_slowdown &= ~cc_do;
						}
						zone_fl = fl;
						set_carcall(cons[carnmb],fl);
					}
				}
			}
			else if ((servf == s_ELEV_OFF_HS) && (cons[Prison] == 0))
			{
				timers[theoff] = 0;
				timers[theofautorst] = 0;
				if (servf == s_ELEV_OFF_HS)
				{
					servf = s_AUTO;
				}
			}

			//LOGIC FOR CEOF
			if (ceof_enable && (ceof_active || ceof_latched))
			{
				if (servf == s_AUTO) 
				{
					servf = s_CAR_OFF;
				}	

				if((cons[Car_Elev_off] & 4) != 0) // CELOFL control
				{
					if ((latch_CEOF == 1) && (timers[tsec] != 0))
					{	
							setoutp(o_CEOFL);
					}
					else
					{
							clroutp(o_CEOFL);
					}
				}

				if (((cons[Car_Elev_off] & 2) != 0)  && (timers[tceoff] >= 10))
				{
					latch_CEOF = 1;
				}

				if(rdinp(i_CEOFR) == 1)
				{
					latch_CEOF = 0;
				}

				if ((fvars[fvceloff] & 0x01) != 0)
				{						// Recall Car

					if (fvars[fvheoffl] == 0)
						fl = lobby_fl;
					else if ((fvars[fvheoffl] < cons[bottomf]) || (fvars[fvheoffl] > cons[topf]))
						fl = cons[bottomf];
					else
						fl = fvars[fvheoffl];


					if ((position != fl) && (servf == s_CAR_OFF))
		  			{
						if((fvars[fvceloff2] & 0x01) != 0)
						{
							rear_slowdown &= ~cc_do;
							front_slowdown &= ~cc_do;
							clrcc(0);
						}
						zone_fl = fl;
						set_carcall(cons[carnmb],fl);
					}

				}
			}
			else if (servf == s_CAR_OFF)
			{
				servf = s_AUTO;
				timers[tceoff] = 0;
			}
		}
		else 
		{
			if ((servf == s_ELEV_OFF_CS) || (servf == s_ELEV_OFF_HS) || (servf == s_CAR_OFF))
			{
				if (latch_JAIL == 0)
				{
					servf = s_AUTO;
				}
			}
		}
		
		if(heof_over_ind == 0)
			timers[theofovrind] = 0;
		
	}
	
	if (((cons[Elev_off] & 4) != 0) && ((fvars[fvheofovrind] & 0x02) != 0))
	{
		if (rdinp(i_HEOF) == 0)
		{
			timers[theofovratt] = 0;
			timers[theoff] = 0;		
		}
	}
}

//************************************
// Set service flag for jail break
//************************************
void jail_break (void)
{
	int16 fl;
	if((cons[Prison] == 1) &&((cons[Elev_off] & 0x06) == 0))
	{
		if ((firef == 0) && (rfiref == 0) && (codebf == 0) && (vipf == 0) && (eqf == 0) && (medf == 0))
		{
			if(rdinp(i_JAILB) == 1)
			{
				if(timers[theoff] >= 10)
					latch_JAIL = 1;
			}
			else
			{
				timers[theoff] = 0;
			}
			if (rdinp(i_HEOFR) == 1)
			{
				latch_JAIL = 0;
				if(servf == s_ELEV_OFF_HS)
				{
					servf = s_AUTO;
				}
			}
			if(latch_JAIL == 1)
			{
				if(servf == s_AUTO)
					servf = s_ELEV_OFF_HS;
				setoutp(o_HEOFL);
				//RECALL THE CAR
				if (fvars[fvheoffl] == 0)
					fl = lobby_fl;
				else
					fl = fvars[fvheoffl];
				 
				if ((fl < cons[bottomf]) || (fl > cons[topf]))
					fl = cons[bottomf];

				if ((position != fl) && (servf == s_ELEV_OFF_HS))
	  			{
					clrcc(0);
					rear_slowdown &= ~cc_do;
					front_slowdown &= ~cc_do;
					zone_fl = fl;
					set_carcall(cons[carnmb],fl);
				}
				if (position == fl)
				{
					clrcc(0);
				}

			}
			else
			{
				clroutp(o_HEOFL);
				
			}
			

		}
	}


}

void zoneelev(void)
{
	int16 fl;

	if ( (servf == s_AUTO) && (dcalls == 0) &&         	// in service and no calls
	     ((cons[mand] == 2) || ((doorf == 0) && (rdoorf == 0))) && 		// Car has swing doors or Doors are closed
	     (dirf == 0) && (dpref == 0) )
	{
		fl = fvars[fvlob];
		if (position != fl) 
		{
			zone_fl = fl;
		}
	}
	
	
}

//****************************************
// Set service flag for hot oil operation
//****************************************
void hot_oil(void)
{
	while (procf == 22)
	{
		servf = s_HOT_OIL;
		inctime(0);
		set_ul_dl_dz();		// sets up_level, dn_level and door_zone
		setfsf();
		clrall();
		if (chk_ins() == 1)
		{
			if (cons[ansi] < 2007)
			{
				// clear hotoil flag
				if ((cons[hotoilSW] == 0) ||
					((cons[hotoilSW] == 1) && (rdinp(i_TPH) == 0)) || // TPH input is OK
					((cons[hotoilSW] == 2) && (rdinp(i_TPH) == 1) && (rdinp(i_CEN) == 1)))	 // TPH input is OK
				{
						hotoilf = 0;
						procf = 2;
				}
			}
		}
		if (ccnmsk[cons[carnmb]][position] != 0)
		{
			if ((rdinp(i_DOB) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) || 
				((manual_door == 2) && (man_fdcc() == 0)))
			{
				procf = 15;
				hotoilf = 1;
			}
		}
		if ((cons[rear] != 0) && (rccnmsk[cons[carnmb]][position] != 0))
		{
			if ((rdinp(i_DOBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) || 
				((manual_rdoor == 2) && (man_rdcc() == 0)))
			{
				procf = 15;
				hotoilf = 1;
			}
		}
	}
	return;
}



//*******************************************
// Test if okay to recall on emergency power
//*******************************************

int16 ep_recall_ok(void)
{
//	if (ind() == 1)
	if (servf == s_IND)
		return(0);
//	if ((rdinp(i_ATT) == 1) && (cons[att] != 0))
	if (servf == s_ATT)
		return(0);
	if (stallf != 0)
		return(0);
 	if ((firef != 0) || (rfiref != 0) || (hsvf != 0) || (eqf != 0) || (medf != 0))
		return(0);
	if ((rdinp(i_ELOF) == 1) && ((cons[Elev_off] & 1) != 0))
		return(0);
  	if (((rdinp(i_RTLH) == 1) && (cons[otslbp] != 0)) || 
  		(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_rtl == 1)) || 
  		(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_rtl == 1)) || 
  		((rdinp(i_RTLDC) == 1) && ((cons[otslbp] & 0x04) != 0)))
		return(0);

	if (((rdinp(i_HEOF) == 1) && ((cons[Elev_off] & 2) != 0)) || (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_heof == 1)) ||
		(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_return_fl != 0)) || (((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_heof == 1))) 
		return(0);

	return(1);
}


// ************************************************
// This routine gets the next valid position
// ************************************************

int16 get_valid_fl(int16 pos, int16 dir)
{
	int16 i;

	if (dir == 1)
	{	// going up
		for (i=pos;i<cons[topf];i++)
		{
			if (valid_fl[i] == 1)
				return i;
		}
		return i;		// return top floor if none found
	}
	else if (dir == 2)
	{	// going down
		for (i=pos;i>cons[bottomf];i--)
		{
			if (valid_fl[i] == 1)
				return i;
		}
		return i;		// return bottom floor if none found
	}
	else
		return cons[topf];
}

// ******************************************
// This is the hall lantern procedure
// ******************************************
void hlants()
{
	int16 dis_lant_ncu = 0;
	int16 timed_lant = 0; // 0= fire as soon as hsf = 0 (slow down)
	int32 lantern_delay = 0;
	int32 Time_to_Stop = 0;

	Time_to_Stop = 0;

	//hall lanterns fire up as soon we slow down unless fv is nonzero or Timetostop > fv
	if ((fvars[fvhlandelay] != 0) && (Time_to_Stop > fvars[fvhlandelay]))
	{
		timed_lant = 1;
		lantern_delay = Time_to_Stop - fvars[fvhlandelay];
	}
	else
		timed_lant = 0;


	if ((car_Grp_Stat.c_next_up != 0) && (onward_calls == 0) && (position == lobby_fl) && (ncu_door_seq == 2) && (timers[tncu] > fvars[fvldt]))
		dis_lant_ncu = 1;
	else
		dis_lant_ncu = 0;

  	if( (firef != 0) || (ind() == 1) ||	(eqf != 0) || (codebf != 0) || (vipf != 0) || (hsvf == 1) ||
  	    ((rdinp(i_AD) == 0) ||
  	     ((servf == s_SABBATH) && ((fvars[fvsabbdis] & 0x02) != 0)) || 
  	     (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) || 
  	     (((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ad_off == 1))) || ((dpref == 0) && (fvars[fvarrivallant] == 0)) )
  	{
    	clrlant();
		if (((fvars[fvsabben2] & 0x02) != 0) && (servf == s_SABBATH)) 
		{
			if ((dirf == 1) || ((dirf == 0) && (dpref == 1)))
			{
				setoutp(o_CUL);
				cab_lant_up = 1;
				if (cons[rear] != 0)
				{
					setoutp(o_CULR);
					cab_rlant_up = 1;
				}
			}
			else if ((dirf == 2) || ((dirf == 0) && (dpref == 2)))
			{
				setoutp(o_CDL);
				cab_lant_dn = 1;
				if (cons[rear] != 0)
				{
					setoutp(o_CDLR);
					cab_rlant_dn = 1;
				}
			}
		}
		if ((position == vipfl) && (servf == s_VIP) && (vipf < 4))
		{
			if (car_Grp_Stat.c_vip_fl != 0)
			{
				if ((fvars[fvviplant] & 0x01) != 0)
				{
			   		if ((rdinp(i_DOL) == 0) || ((doorf != 0) && (((fvars[fvclantctl] & 0x01) != 0) || 
			   			((doorf != 0) && ((fvars[fvclantctl] & 0x02) != 0) && (rdinp(i_DPM) == 0)))))
					{
						if (position <= lobby_fl)  // at or below the lobby floor
						{
							setoutp(o_CUL);
							cab_lant_up = 1;
						}
						else
						{
							setoutp(o_CDL);
							cab_lant_dn = 1;
						}
					}
				}
				if ((fvars[fvviplant] & 0x02) != 0)
				{
					if (position <= lobby_fl)  // at or below the lobby floor
						lant_up = 1;
					else
						lant_dn = 1;
				}
			}
			else if ((car_Grp_Stat.c_rvip_fl != 0) && (cons[rear] != 0))
			{
				if ((fvars[fvviplant] & 0x01) != 0)
				{
			   		if ((rdinp(i_DOLR) == 0) || ((rdoorf != 0) && (((fvars[fvclantctl] & 0x01) != 0) || 
			   			((rdoorf != 0) && ((fvars[fvclantctl] & 0x02) != 0) && (rdinp(i_RPM) == 0)))))
					{
						if (position <= lobby_fl)  // at or below the lobby floor
						{
							setoutp(o_CULR);
							cab_rlant_up = 1;
						}
						else
						{
							setoutp(o_CDLR);
							cab_rlant_dn = 1;
						}
					}
				}
				if ((fvars[fvviplant] & 0x02) != 0)
				{
					if (position <= lobby_fl)  // at or below the lobby floor
						rlant_up = 1;
					else
						rlant_dn = 1;
				}
			}
		}

    	return;
  	}

	if (fvars[fvarrivallant] == 1)
	{

   		if ((rdinp(i_DOL) == 0) || ((doorf != 0) && (((fvars[fvclantctl] & 0x01) != 0) || 
   			((doorf != 0) && ((fvars[fvclantctl] & 0x02) != 0) && (rdinp(i_DPM) == 0)))))
		{
			if ((front_slowdown != 0) || (doorf !=0))
			{
				if (position <= lobby_fl)  // at or below the lobby floor
				{
					setoutp(o_CUL);
					clroutp(o_CDL);
					cab_lant_up = 1;
					cab_lant_dn = 0;
				}
				else
				{
					setoutp(o_CDL);
					clroutp(o_CUL);
					cab_lant_dn = 1;
					cab_lant_up = 0;
				}
			}
		}
		else if (doorf == 0)
		{
			cab_lant_up = 0;
			cab_lant_dn = 0;
			clroutp(o_CDL);
			clroutp(o_CUL);
		}

   		if ((rdinp(i_DOLR) == 0) || ((rdoorf != 0) && (((fvars[fvclantctl] & 0x01) != 0) || 
   			((rdoorf != 0) && ((fvars[fvclantctl] & 0x02) != 0) && (rdinp(i_RPM) == 0)))))
		{
			if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
			{
				if (position <= lobby_fl)  // at or below the lobby floor
				{
					setoutp(o_CULR);
					clroutp(o_CDLR);
					cab_rlant_up = 1;
					cab_rlant_dn = 0;
				}
				else
				{
					setoutp(o_CDLR);
					clroutp(o_CULR);
					cab_rlant_dn = 1;
					cab_rlant_up = 0;
				}
			}
		}
		else if (rdoorf == 0)
		{
			cab_rlant_up = 0;
			cab_rlant_dn = 0;
			clroutp(o_CDLR);
			clroutp(o_CULR);
		}

		if (((front_slowdown != 0) && (rdoutp(o_RUN) == 1)) || (doorf !=0))
		{
			if (position <= lobby_fl)  // at or below the lobby floor
			{
				lant_up = 1;
				lant_dn = 0;
			}
			else
			{
				lant_dn = 1;
				lant_up = 0;
			}
		}
		else
		{
			lant_up = 0;
			lant_dn = 0;
		}

		if (((rear_slowdown != 0) && (rdoutp(o_RUN) == 1)) || (rdoorf != 0))			// Rear call stop
		{
			if (position <= lobby_fl)  // at or below the lobby floor
			{
 				rlant_up = 1;
				rlant_dn = 0;
 			}
 			else
 			{
 				rlant_dn = 1;
				rlant_up = 0;
			}
		}
		else
		{
			rlant_up = 0;
			rlant_dn = 0;
		}
		all_lant_clr = 1;
	   	if ((cons[Hall_Lant] & 0x01) != 0)
 		  	set_hall_lant();
		return;
	}

  	if(dpref == 0)
  	{
	 	timers[tcablnt] = 0;
	 	timers[trcablnt] = 0;
	 	timers[tlant] = 0;
	 	timers[trlant] = 0;
  	}
  	if(dpref == 1)
  	{
		if ( ((cons[mand] == 0) && (cons[frghtd] == 0) && (cons[RCM_HWDoor] == 0)) || ((cons[dispis] & 2) != 0))
		{		// use LU for lobby up hall lantern
			if ( ((position == lobby_fl) && (car_Grp_Stat.c_next_up == 0)) || 
				 (car_Grp_Stat.c_ncu_door == 1) || 
				 ((car_Grp_Stat.c_ir_car == cons[carnmb]) && (fvars[fvlaniralwys] == 1)) )			
			{
				if (timed_lant == 0)
				{
					setoutp(o_LU);	  // Ring the lantern at the lobby only when you are the next up car or when not running next up
				}
				else if (timers[tlant] >= (lantern_delay))
				{
					setoutp(o_LU);	  // Ring the lantern at the lobby only when you are the next up car or when not running next up
				}
				if (cons[rear] != 0)
				{
					if (timed_lant == 0)
					{
						setoutp(o_LUR);	  // Ring the lantern at the lobby only when you are the next up car or when not running next up
					}
					else if (timers[trlant] >= (lantern_delay))
					{
						setoutp(o_LUR);	  // Ring the lantern at the lobby only when you are the next up car or when not running next up
					}
				}
			}
		}

		// Standard lantern operation
		if ( (car_Grp_Stat.c_next_up == 0) || (position != lobby_fl) || 
			  (car_Grp_Stat.c_ncu_door == 1) ||
			 ((car_Grp_Stat.c_ir_car == cons[carnmb]) && (fvars[fvlaniralwys] == 1)) ) 
		{
			if (((front_slowdown != 0) || (doorf !=0)) && ((dis_lant_ncu == 0) || ((fvars[fvncul] & 0x01) == 0)))
			{
    			if (timed_lant == 0)
				{
					lant_up = 1;
				}
				else if (timers[tlant] >= fvars[fvhlandelay])
			    	lant_up = 1;	  // Ring the lantern at the lobby only when you are the next up car or when not running next up
				else
					lant_up = 0;
			}
			else if ((dis_lant_ncu == 1) && ((fvars[fvncul] & 0x01) != 0))
				lant_up = 0;

			if ((rear_slowdown != 0) || (rdoorf != 0))
			{
				if (timed_lant == 0)
				{
			    	rlant_up = 1;	  // Ring the lantern at the lobby only when you are the next up car or when not running next up
				}
		   		else if (timers[trlant] >= (lantern_delay))
				{
			    	rlant_up = 1;	  // Ring the lantern at the lobby only when you are the next up car or when not running next up
				}
			}
		}

		if ((cons[dispis] & 2) != 0)
		{
			if (cons[rear] != 0)
				clroutp(o_LDR);
	    	clroutp(o_LD);
		}
		lant_dn = 0;
		rlant_dn = 0;
		cab_lant_dn = 0;
		cab_rlant_dn = 0;
    	clroutp(o_CDL);
    	clroutp(o_CDLR);
   		if ((rdinp(i_DOL) == 0) || ((doorf != 0) && (((fvars[fvclantctl] & 0x01) != 0) || 
   			((doorf != 0) && ((fvars[fvclantctl] & 0x02) != 0) && (rdinp(i_DPM) == 0)))))
		{
			if (((front_slowdown != 0) || (doorf !=0))  && ((dis_lant_ncu == 0) || ((fvars[fvncul] & 0x02) == 0)))
			{
				if (((fvars[fvsrlant] & 0x01) == 0) || (((fvars[fvsrlant] & 0x01) != 0) && ((front_slowdown & iruc_sd) != 0)))
				{ 
					setoutp(o_CUL);
					cab_lant_up = 1;
				}
			}
			else if ((dis_lant_ncu == 1) && ((fvars[fvncul] & 0x02) != 0))
			{
 		    	clroutp(o_CUL);
				cab_lant_up = 0;
			}
  		}
   		if ((rdinp(i_DOLR) == 0) || ((rdoorf != 0) && (((fvars[fvclantctl] & 0x01) != 0) || 
   			((rdoorf != 0) && ((fvars[fvclantctl] & 0x02) != 0) && (rdinp(i_RPM) == 0)))))
		{
			if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
			{
				if (((fvars[fvsrlant] & 0x01) == 0) || (((fvars[fvsrlant] & 0x01) != 0) && ((rear_slowdown & iruc_sd) != 0)))
				{ 
					setoutp(o_CULR);
					cab_rlant_up = 1;
				}
			}
  		}
  	}
  	if(dpref == 2)
  	{
  	   	if (rdinp(i_DCL) == 0) 
			keep_lants_on = 0;
  	   	if ((cons[rear] == 0) || (rdinp(i_DCLR) == 0))  
			keep_rlants_on = 0;
		if (((cons[mand] == 0) && (cons[frghtd] == 0) && (cons[RCM_HWDoor] == 0)) || ((cons[dispis] & 2) != 0))	
		{
			if (cons[rear] != 0)
				clroutp(o_LUR);
	     	clroutp(o_LU);
		}
		lant_up = 0;
		rlant_up = 0;
		cab_lant_up = 0;
		cab_rlant_up = 0;
     	clroutp(o_CUL);
     	clroutp(o_CULR);

     	if(fvars[fvdbl] == 0)
     	{
			if (((cons[dispis] & 2) != 0) && (position == lobby_fl))
			{
				if ((front_slowdown != 0) || (doorf !=0))
				{
					if (timed_lant == 0)
					{
						setoutp(o_LD);
				   	}
				   	else if (timers[tlant] >= (lantern_delay))
					{
						setoutp(o_LD);
				   	}
				}
				if (cons[rear] != 0)
				{
		     	   	if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
					{
						if (timed_lant == 0)
						{
							setoutp(o_LDR);
					   	}
						else if (timers[trlant] >= (lantern_delay))
						{
							setoutp(o_LDR);
					   	}
				  	}
				}
      		}

			if ((front_slowdown != 0) || (doorf !=0))
			{
				if (timed_lant == 0)
				{
					lant_dn = 1;
				}
				else if (timers[tlant] >= (lantern_delay))

					lant_dn = 1;
			}
    	   	if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
			{
				if (timed_lant == 0)
				{	
					rlant_dn = 1;
				}
				else if (timers[trlant] >= (lantern_delay))

		    	 	rlant_dn = 1;
		  	}

	   		if ((rdinp(i_DOL) == 0) || ((doorf != 0) && (((fvars[fvclantctl] & 0x01) != 0) || 
	   			((doorf != 0) && ((fvars[fvclantctl] & 0x02) != 0) && (rdinp(i_DPM) == 0)))))
	   		{
				if ((front_slowdown != 0) || (doorf !=0))
				{
					if (((fvars[fvsrlant] & 0x01) == 0) || (((fvars[fvsrlant] & 0x01) != 0) && ((front_slowdown & irdc_sd) != 0)))
					{ 
			   			setoutp(o_CDL);
						cab_lant_dn = 1;
					}
				}
     		}
	   		if ((rdinp(i_DOLR) == 0) || ((rdoorf != 0) && (((fvars[fvclantctl] & 0x01) != 0) || 
	   			((rdoorf != 0) && ((fvars[fvclantctl] & 0x02) != 0) && (rdinp(i_RPM) == 0)))))
	   		{
				if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
				{
					if (((fvars[fvsrlant] & 0x01) == 0) || (((fvars[fvsrlant] & 0x01) != 0) && ((rear_slowdown & irdc_sd) != 0)))
					{ 
			   			setoutp(o_CDLR);
						cab_rlant_dn = 1;
					}
				}
     		}
     	}
     	else
     	{	  
     		// Double gong for front lantern
       		if (timed_lant == 0)
			{
		   		if( (timers[tlant] > fvars[fvlnton]) ||
		   						(timers[tlant] < fvars[fvlntof]) )
	       		{
					if (((cons[dispis] & 2) != 0) && (position == lobby_fl))
					{
						if ((front_slowdown != 0) || (doorf !=0))
							setoutp(o_LD);
					}
					if ((front_slowdown != 0) || (doorf !=0))
						lant_dn = 1;
	       		}
				else
	       		{
					if ((cons[dispis] & 2) != 0)
						clroutp(o_LD);
					lant_dn = 0;
	       		}
       		}
       		else
       		{
	       		if( (((timers[tlant] - (lantern_delay)) > fvars[fvlnton]) && ((timers[tlant] - (lantern_delay)) > 0)) ||
					(((timers[tlant] - (lantern_delay)) > 0) && ((timers[tlant] - (lantern_delay)) < fvars[fvlntof])))
	       		{
					if (((cons[dispis] & 2) != 0) && (position == lobby_fl))
					{
						if ((front_slowdown != 0) || (doorf !=0))
							setoutp(o_LD);
					}
					if ((front_slowdown != 0) || (doorf !=0))
						lant_dn = 1;
	       		}
	       		else
	       		{
					if ((cons[dispis] & 2) != 0)
						clroutp(o_LD);
					lant_dn = 0;
	       		}
			}

     		// Double gong for rear lantern
			if (timed_lant == 0)
			{
	       		if( (timers[trlant] > fvars[fvlnton]) ||
		   						(timers[trlant] < fvars[fvlntof]) )
				{
					if ((cons[rear] != 0) && ((cons[dispis] & 2) != 0) && (position == lobby_fl))
					{
						if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
							setoutp(o_LDR);
					}
					if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
						rlant_dn = 1;
	       		}
	       		else
	       		{
					if (((cons[rear] != 0) && (cons[dispis] & 2) != 0))
						clroutp(o_LDR);
					rlant_dn = 0;
	       		}
			}
			else
			{
				if((((timers[trlant] - (Time_to_Stop - (lantern_delay))) > fvars[fvlnton]) && ((timers[trlant] - (lantern_delay)) > 0)) ||
								(((timers[trlant] - fvars[fvhlandelay]) < fvars[fvlntof]) && ((timers[trlant] - (lantern_delay)) > 0) ))
	       		{
					if ((cons[rear] != 0) && ((cons[dispis] & 2) != 0) && (position == lobby_fl))
					{
						if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
							setoutp(o_LDR);
					}
					if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
						rlant_dn = 1;
	       		}
	       		else
	       		{
					if (((cons[rear] != 0) && (cons[dispis] & 2) != 0))
						clroutp(o_LDR);
					rlant_dn = 0;
	       		}
       		}

     		// Double gong for front cab lantern
       		if ( ((doorf != 2) && (doorf != 3) && (cons[frghtd] == 0) && ((fvars[fvclantctl] & 0x03) == 0)) ||  
       				((doorf == 0) && ((fvars[fvclantctl] & 0x01) != 0)) ||
					(((doorf == 0) || (rdinp(i_DPM) != 0)) && ((fvars[fvclantctl] & 0x02) != 0)) ||
       				((cons[frghtd] != 0) && (rdinp(i_DOL) != 0) && (front_slowdown !=0)) )
			 			timers[tcablnt] = 0;
       		if ((timers[tcablnt] > fvars[fvlnton]+2) || (timers[tcablnt] > doort))
	   				keep_lants_on = 1;

       		if((((timers[tcablnt] > fvars[fvlnton]+2) ||
	   		  (timers[tcablnt] < fvars[fvlntof]+2)) &&
	   		  (timers[tcablnt] > 2)) || (keep_lants_on == 1))
			{
				if ((front_slowdown != 0) || (doorf !=0))
				{
					if (((fvars[fvsrlant] & 0x01) == 0) || (((fvars[fvsrlant] & 0x01) != 0) && ((front_slowdown & irdc_sd) != 0)))
					{ 
						setoutp(o_CDL);
						cab_lant_dn = 1;
					}
				}
       		}
       		else
			{
				clroutp(o_CDL);
				cab_lant_dn = 0;
			}

     		// Double gong for rear cab lantern
       		if ( (cons[rear] != 0) && (((rdoorf != 2) && (rdoorf != 3) && (cons[frghtd] == 0) && ((fvars[fvclantctl] & 0x03) == 0)) || 
				 	((rdoorf == 0) && ((fvars[fvclantctl] & 0x01) != 0)) ||
					(((rdoorf == 0) || (rdinp(i_RPM) != 0)) && ((fvars[fvclantctl] & 0x02) != 0)) ||
       			 	((cons[frghtd] != 0) && (rdinp(i_DOLR) != 0) && (rear_slowdown !=0))) ) 
			 			timers[trcablnt] = 0;

       		if ((timers[trcablnt] > fvars[fvlnton]+2) || (timers[trcablnt] > rdoort))
	   				keep_rlants_on = 1;
       		if((((timers[trcablnt] > fvars[fvlnton]+2) ||
	   		  (timers[trcablnt] < fvars[fvlntof]+2)) &&
	   		  (timers[trcablnt] > 2)) || (keep_rlants_on == 1))
			{
				if ((rear_slowdown != 0) || (rdoorf != 0))			// Rear call stop
				{
					if (((fvars[fvsrlant] & 0x01) == 0) || (((fvars[fvsrlant] & 0x01) != 0) && ((rear_slowdown & irdc_sd) != 0)))
					{ 
						setoutp(o_CDLR);
						cab_rlant_dn = 1;
					}
				}
       		}
       		else
			{
				clroutp(o_CDLR);
				cab_rlant_dn = 0;
			}
     	}
   	}
   	
	all_lant_clr = 0;
   	if ((cons[Hall_Lant] & 0x01) != 0)
   		set_hall_lant();
	
  	return;
}

//**************************************
// Set Hall Lantern Outputs
//**************************************

void set_hall_lant (void)
{
	if (lant_up == 1)
		setoutp(o_UPL(position));		// set up lantern
	else
		clroutp(o_UPL(position));
	
	if (lant_dn == 1)
		setoutp(o_DNL(position));		// set down lantern
	else
		clroutp(o_DNL(position));
	
	if (cons[rear] != 0) 
	{
		if (rlant_up == 1)
			setoutp(o_UPLR(position));		// set up rear lantern
		else
			clroutp(o_UPLR(position));
		if (rlant_dn == 1)
			setoutp(o_DNLR(position));		// set down rear lantern
		else
			clroutp(o_DNLR(position));
	}
}

//***************************************
// Hydro control called from Inctime
//***************************************

void hydro_control (void)
{
	
	write_trace(TR_HYDRO_CONTROL);
	if (cons[Reset_Jack] == 1)
	{			// Reset Jack Operation Start
		if (((firef != 0) || (rfiref != 0)) && (procf != 18))
		{
			if (rhjackf >= 1)
				rhjackf = 1;		// place jack reset on hold
	 		timers[trhjack] = 0;
			timers[tjackdly] = 0;
		}
		// Check to reset hydro jack
		if ((rhjackf == 1) || ((rhjackf == 2) && (procf != 18)))
		{
			if (timers[trhjack] > 600)		// check timer every 1 minute
			{
				local_gettime();
				if (t.hour >= 2)
				{
					if (position != cons[bottomf])
						zone_fl = cons[bottomf];
					else
						zone_fl = 0;
					if ((position == cons[bottomf]) && 
						(doorf == 0) && (rdinp(i_DCL) == 0) && 
						((cons[rear] == 0) || ((rdoorf == 0) && (rdinp(i_DCLR) == 0))) &&
						(dirf == 0) && (dpref == 0) && (dcalls == 0) &&
						((procf >= 14) && (procf <= 16)) &&
						((safe() == 1) || (((cons[mand] !=0) || (cons[frghtd] != 0)) && (man_dcc() == 1) && (car_gate() == 1))) && 
						(door_zone == 1))
					{
						rhjackf = 2;
						servf = s_RST_JACK;
						if (timers[tjackdly] > 200)
						{
							procf = 18;
							zone_fl = 0;
						}
					}
					else
						timers[tjackdly] = 0;
				}
				else
				{
					timers[trhjack] = 0;
					timers[tjackdly] = 0;
				}
			}
			else
				timers[tjackdly] = 0;
		}
		else if (rhjackf == 0)
		{
	 		timers[trhjack] = 0;
			timers[tjackdly] = 0;
		}		
	}

}


//*******************************************
// Status Control from Inctime
//*******************************************
void status_control (int16 start_ck)
{
	


	if (start_ck == 0)
	{
		write_trace(TR_STATUS_CONTROL);
		
		if (cons[brake] == 3)
		{
			rdinp(i_BKS1);
			rdinp(i_BKS2);
			rdinp(i_BDBS);
		}

		
	  	if( (old_firef != firef) || (old_rfiref != rfiref) || 
	  		(old_fire_floor != fire_floor) || (old_gripper_fault != gripper_fault) ||
	  		(old_eqf != eqf) || (old_above_cwt != above_cwt) || (old_trace_auto_start != trace_auto_start) ||
	  		(old_fs2_to_fs1 != fs2_to_fs1) ||
	  		(old_stallf != stallf) || (old_hotoilf != hotoilf) || (old_fs2_to_fs1 != fs2_to_fs1))
	  	{
			Update_Control_Flags();
	      	old_firef = firef;
	      	old_rfiref = rfiref;
			old_fire_floor = fire_floor;
			old_gripper_fault = gripper_fault;
			old_eqf = eqf;
			old_above_cwt = above_cwt;
			old_stallf = stallf;
			old_hotoilf = hotoilf;
			old_fs2_to_fs1 = fs2_to_fs1;
	  		old_trace_auto_start = trace_auto_start;
	  	}



		if (position != prev_position)
			Update_Pos();
		prev_position = position;

		set_scnd_position();
		if(scnd_pos != prev_scnd_pos)
		{
			Update_scnd_Pos();
			prev_scnd_pos = scnd_pos;
		}

		if (rset_run_cnt > 5)
			statusf2 |= sf_ResetRun;
		else
			statusf2 &= ~sf_ResetRun;
		
		
		if ((rdinp(i_TSTM) == 1) ||
			((NTS_spi.Status2.B.rear_door == 1) && (cons[rear] == 0)) || 
			((NTS_spi.Status2.B.rear_door == 0) && (cons[rear] == 1)) ) 
		{
			if ((ind() == 0) || (rdinp(i_AD) == 1)) // not already out of service set service flag = 0
				servf = s_CONFIG;		// take car out of group service
			statusf2 |= sf_Setup_Svc;
		}
		else
			statusf2 &= ~sf_Setup_Svc;
		
	}
	
	if (start_ck == 0)
	{
	  	if(rdinp(i_FEP) == 1)
			timers[tfept] = 0;
	  	if((rdinp(i_FEP) == 0) && (timers[tfept] >10))
	  	{
		  	// FEP fuse blown
			if ((power_status & ps_FEP) == 0)
				record_fault(f_FEP_fuse);
			power_status |= ps_FEP;
	  	}
		else				  
			power_status &= ~ps_FEP;

		if ((cons[housing] == 1) || (cons[housing] == 2))
		{
			if ((rdinp(i_CCP) == 1) && (rdinp(i_CCLP) == 1) && (rdinp(i_HCLP) == 1)  && ((rdinp(i_LHCP) == 1) || (cons[housing] != 2)))
				timers[tpst] = 0;
			if ((timers[tpst] > 10) && ((rdinp(i_CCP) == 0) || (rdinp(i_CCLP) == 0) || (rdinp(i_HCLP) == 0) || 
				((rdinp(i_LHCP) == 0) && (cons[housing] == 2)) ))
			{
				if (rdinp(i_CCP) == 0)
				{
					if ((power_status & ps_CC) == 0)
						record_fault(f_CC_fuse);
					power_status |= ps_CC;
				}
				if (rdinp(i_CCLP) == 0)
				{
					if ((power_status & ps_CCL) == 0)
						record_fault(f_CCL_fuse);
					power_status |= ps_CCL;
				}
				if (rdinp(i_HCLP) == 0)
				{
					if ((power_status & ps_HCL) == 0)
						record_fault(f_HCL_fuse);
					power_status |= ps_HCL;
				}
				if ((rdinp(i_LHCP) == 0) && (cons[housing] == 2))
				{
					if ((power_status & ps_LHC) == 0)
						record_fault(f_LHC_fuse);
					power_status |= ps_LHC;
				}

			}
			else power_status &= ~(ps_CC | ps_CCL | ps_HCL | ps_LHC);
		
		}
		else power_status &= ~(ps_CC | ps_CCL | ps_HCL  | ps_LHC);

		if (cons[housing] == 2)
		{
		 	if (rdinp(i_DMO) == 1)
				timers[tdmo] = 0;
		 	if ((rdinp( i_DMO) == 0) && (timers[tdmo] >10))
		 	{
				door_motor_ovl = 1;
		 		if ((statusf2 & sf_DMO) == 0)
		 			record_fault(f_Door_Motor_OVL);
		 		statusf2 |= sf_DMO;
		 	}
			else
			{
				door_motor_ovl = 0;
				statusf2 &= ~sf_DMO;
			}
		}
		else
		{
			door_motor_ovl = 0;
			statusf &= ~sf_DMO;
		}


		if (io_fault == 0)
		{
		
			if (rdinp(i_DLB) != rdinp(i_DLB_1))
			{
				if (timers[tdlb] > 3)
				{
					record_fault(f_dlb_fail);
				    io_fault = 1;
				}
			}
			else
				timers[tdlb] = 0;
				
			if (rdinp(i_DLM) != rdinp(i_DLM_1))
			{
				if (timers[tdlm] > 3)
				{
					record_fault(f_dlm_fail);
				    io_fault = 1;
				}
			}
			else
				timers[tdlm] = 0;

			if (rdinp(i_DLT) != rdinp(i_DLT_1))
			{
				if (timers[tdlt] > 3)
				{
					record_fault(f_dlt_fail);
				    io_fault = 1;
				}
			}
			else
				timers[tdlt] = 0;

			if (rdinp(i_GS) != rdinp(i_GS_1))
			{
				if (timers[tgs] > 3)
				{
					record_fault(f_gs_fail);
				    io_fault = 1;
				}
			}
			else
				timers[tgs] = 0;
			
			if ((cons[rear] == 1) && (rdinp(i_RLM) != rdinp(i_RLM_1)))
			{
				if (timers[trlm] > 3)
				{
					record_fault(f_rlm_fail);
				    io_fault = 1;
				}
			}
			else
				timers[trlm] = 0;

			if ((cons[rear] == 1) && (rdinp(i_RGS) != rdinp(i_RGS_1)))
			{
				if (timers[trgs] > 3)
				{
					record_fault(f_rgs_fail);
				    io_fault = 1;
				}
			}
			else
				timers[trgs] = 0;
		}
		else
		{
			timers[tdlt] = 0;
			timers[tdlm] = 0;
			timers[tdlb] = 0;
			timers[tgs] = 0;
			timers[trlm] = 0;
			timers[trgs] = 0;
			timers[tdl] = 0;
			timers[tul] = 0;
		}
	

	}

	if (start_ck == 0)
	{
	
		// Set Run Status Data: RUN, DNR, UP, UL, DL, DZ, DLT, DLM, DLB, GS, RLM, RGS, DOL, DCL, RDOL, RDCL, EE, SE, EER, SER, hsf, startf, lev_startf
		if (rdoutp(o_RUN) == 1)
			run_statusf |= rs_RUN;
		else
			run_statusf &= ~rs_RUN;
		
		if (rdoutp(o_SD) == 1)
			run_statusf |= rs_DNR;
		else
			run_statusf &= ~rs_DNR;
		
		if (rdoutp(o_SU) == 1)
			run_statusf |= rs_UP;
		else
			run_statusf &= ~rs_UP;
		
		if (dn_level == 1)
			run_statusf |= rs_DL;
		else
			run_statusf &= ~rs_DL;
		
		if (up_level == 1)
			run_statusf |= rs_UL;
		else
			run_statusf &= ~rs_UL;
		
		if (door_zone == 1)
			run_statusf |= rs_DZ;
		else
			run_statusf &= ~rs_DZ;
		
		if (door_zone_aux == 1)
			run_statusf |= rs_DZA;
		else
			run_statusf &= ~rs_DZA;
		
		if (rdinp(i_DLT) == 1)
			run_statusf |= rs_DLT;
		else
			run_statusf &= ~rs_DLT;
		
		if (rdinp(i_DLM) == 1)
			run_statusf |= rs_DLM;
		else
			run_statusf &= ~rs_DLM;
		
		if (rdinp(i_DLB) == 1)
			run_statusf |= rs_DLB;
		else
			run_statusf &= ~rs_DLB;
		
		if (rdinp(i_GS) == 1)
			run_statusf |= rs_GS;
		else
			run_statusf &= ~rs_GS;
		
		if (rdinp(i_DOL) == 1)
			run_statusf |= rs_DOL;
		else
			run_statusf &= ~rs_DOL;
		
		if (rdinp(i_DCL) == 1)
			run_statusf |= rs_DCL;
		else
			run_statusf &= ~rs_DCL;
		
		if ((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1))
			run_statusf |= rs_EE;
		else
			run_statusf &= ~rs_EE;
		
		if (rdinp(i_SE) == 1)
			run_statusf |= rs_SE;
		else
			run_statusf &= ~rs_SE;
		
		if (rdoutp(o_DO) == 1)
			run_statusf |= rs_DO;
		else
			run_statusf &= ~rs_DO;
			
		if (rdoutp(o_DC) == 1)
			run_statusf |= rs_DC;
		else
			run_statusf &= ~rs_DC;
			
		if (cons[rear] != 0)
		{
			if (rdinp(i_RLM) == 1)
				run_statusf |= rs_RLM;
			else
				run_statusf &= ~rs_RLM;
			
			if (rdinp(i_RGS) == 1)
				run_statusf |= rs_RGS;
			else
				run_statusf &= ~rs_RGS;
			
			if (rdinp(i_DOLR) == 1)
				run_statusf |= rs_DOLR;
			else
				run_statusf &= ~rs_DOLR
				;
			if (rdinp(i_DCLR) == 1)
				run_statusf |= rs_DCLR;
			else
				run_statusf &= ~rs_DCLR;
			
			if ((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1))
				run_statusf |= rs_EER;
			else
				run_statusf &= ~rs_EER;
			
			if (rdinp(i_SER) == 1)
				run_statusf |= rs_SER;
			else
				run_statusf &= ~rs_SER;
		
			if (rdoutp(o_DOR) == 1)
				run_statusf |= rs_DOR;
			else
				run_statusf &= ~rs_DOR;
			
			if (rdoutp(o_DCR) == 1)
				run_statusf |= rs_DCR;
			else
				run_statusf &= ~rs_DCR;
			
		}
		else 
			run_statusf &= ~(rs_RGS | rs_RLM | rs_DOLR | rs_DCLR | rs_EER | rs_SER);
		
		if (hsf == 1)
			run_statusf |= rs_HSF;
		else
			run_statusf &= ~rs_HSF;
		
		if (startf == 1)
			run_statusf |= rs_STF;
		else
			run_statusf &= ~rs_STF;
		
		if (lev_startf == 1)
			run_statusf |= rs_LSTF;
		else
			run_statusf &= ~rs_LSTF;
		
		if (rdinp(i_UN) == 1)
			run_statusf |= rs_UN;
		else
			run_statusf &= ~rs_UN;
		
		if (rdinp(i_DN) == 1)
			run_statusf |= rs_DN;
		else
			run_statusf &= ~rs_DN;
		

		if (hardware_init_fault == 1)
			statusf2 |= sf_HWINIT;
		else
			statusf2 &= ~sf_HWINIT;

		if (safe_closetry >= 6)
			statusf2 |= sf_FDC;
		else
			statusf2 &= ~sf_FDC;

		if (safe_rclosetry >= 6)
			statusf2 |= sf_RDC;
		else
			statusf2 &= ~sf_RDC;

		if (cons[Voltage_Sensor] == 1)
		{
			if ((voltf & 0x07) != 0)
				statusf2 |= sf_LVolt;
			else
				statusf2 &= ~sf_LVolt;

			if ((voltf & 0x08) != 0)
				statusf2 |= sf_DVolt;
			else
				statusf2 &= ~sf_DVolt;
		}
		else
			statusf2 &= ~(sf_LVolt | sf_DVolt);

		if (pwl_ups_fault == 1)
			statusf2 |= sf_PWLUPS;
		else
			statusf2 &= ~sf_PWLUPS;
		
		Door_Req_Flags = (uint8)(((rdoor_en & 0x01) << 7) | ((rcc_req & 0x01) << 6) | 
										((rdob_req & 0x01) << 5) | ((rdoor_req & 0x01) << 4) |
										((fdoor_en & 0x01) << 3) | ((fcc_req & 0x01) << 2) | 
										((fdob_req & 0x01) << 1) | (fdoor_req & 0x01));
										
		Door_Seq_Flags = (uint8)(((rdo_seq & 0x07) << 5) | ((do_seq & 0x07) << 1) | (predof & 0x01));

		Nudgst_Flags = (uint8)(((rnudgst & 0x0f) << 4) | (nudgst & 0x0f));
		Call_Flags = (uint8)(((onward_hc & 0x03) << 6) | ((onward_cc & 0x03) << 4) | 
										 ((onward_calls & 0x03) << 2) | (dcalls & 0x03));
		Set_Fault_Bits();

		if (in_redundancy_test == 0)
		{

			if (motion_fault == 0)
			{
				setoutp(o_CEN);
				if ((rdinp(i_CEN) == 0) && (rdinp(i_MRSW) == 1))
				{			  // Lost CEN but still have Machine room stop switch
					if (timers[tcen] > 3)
					{
						if (cen_fault == 0)
							record_fault(f_CEN_flt);
						cen_fault = 1;
					}
				}
				else
				{
					cen_fault = 0;
					timers[tcen] = 0;
				} 
				
			}
			else
			{
				clroutp(o_CEN);
				cen_fault = 0;
				timers[tcen] = 0;
			} 
		}
		else
		{
			cen_fault = 0;
			timers[tcen] = 0;
		} 


	}
	
	if (io_fault == 0)
		timers[tiofltrst] = 0;
	
	if (timers[tiofltrst] > 20)
	{
		if (io_fault_cnt < 2)
			io_fault = 0;
		io_fault_cnt ++;
		timers[tiofltrst] = 0;
	}

	if((io_fault != 0) && (io_fault_cnt > 3))
	{
		statusf |= sf_IO;		// I/O Error
	}
	else
	{
		statusf &= ~sf_IO;
	}
	
	
	if (Buffer_Test == 1)
	{
		NTS_spi.Command2.B.TL_TEST = 1;		 	// Disable limit on NTS processor
	}
	else
	{
		NTS_spi.Command2.B.TL_TEST = 0;
		if (NTS_Test == 1)
		{
			NTS_spi.Command2.B.NTS_TEST = 1;
		}
		else
		{
			NTS_spi.Command2.B.NTS_TEST = 0;
		}
	}

	if ((rdinp(i_AD) == 0) && (disable_auto_door == 1))
	{
		NTS_spi.Command2.B.AD_OFF = 1;
	}
	else
	{
		NTS_spi.Command2.B.AD_OFF = 0;
	}
		
	
	
	if (start_ck == 0)
	{

		if (cons[Voltage_Sensor] == 1)  
		{
#if (Simulator == 0)		
			if (vs_can_online == 1)
			{
				if (L1_Voltage < fvars[fvlowlinevolt]) 
				{
					if ((voltf & 0x01) == 0)
						record_fault(f_L1_Voltage_Low);
					voltf |= 0x01;
				}
				else
					voltf &= ~0x01;
				if (L2_Voltage < fvars[fvlowlinevolt]) 
				{
					if ((voltf & 0x02) == 0)
						record_fault(f_L2_Voltage_Low);
					voltf |= 0x02;
				}
				else
					voltf &= ~0x02;
				if (L3_Voltage < fvars[fvlowlinevolt]) 
				{
					if ((voltf & 0x04) == 0)
						record_fault(f_L3_Voltage_Low);
					voltf |= 0x04;
				}
				else
					voltf &= ~0x04;
				if (Door_Voltage < fvars[fvlowdoorvolt]) 
				{
					if ((voltf & 0x08) == 0)
						record_fault(f_Door_Voltage_Low);
					voltf |= 0x08;
				}
				else
					voltf &= ~0x08;
			}
			else
				voltf = 0;
#endif
		}
		else
			voltf = 0;
	}
	
	
	read_ports();
}
//*******************************************
// Set Car Outputs from Inctime
//*******************************************
void output_control (int16 start_ck)
{
	if (start_ck == 0)
	{
		
		write_trace(TR_OUTPUT_CONTROL);
		
		if (enc_vel_fpm > 75)
			setoutp(o_SP75);
		else
			clroutp(o_SP75);
		
		if (enc_vel_fpm > 150)
			setoutp(o_SP150);
		else
			clroutp(o_SP150);
		
		if (Hoistway_Learned == 1)
			setoutp(o_HWLRN);
		else
			clroutp(o_HWLRN);
		
		if (((procf >= 3) && (procf <= 11)) || (rdoutp(o_SU) == 1) || (rdoutp(o_SD) == 1))
		{	  // The car is moving
			setoutp(o_DBC);
			DBR_Temp_Fault = 0;
			statusf &= ~sf_DBR;
		}
		else 
		{
		  	if(SIU.GPDI[DBR_TEMP].B.PDI == 0)		// detected when input low
			{	   // Temperature is too high: sensor trip point
				clroutp(o_DBC);
				if (DBR_Temp_Fault == 0)
					record_fault(f_dbr_temp_flt);
				DBR_Temp_Fault = 1;
				statusf |= sf_DBR;
			}
			else 
			{		 
				setoutp(o_DBC); 
				DBR_Temp_Fault = 0;
				statusf &= ~sf_DBR;
			}
		}


		// Motor Blower control
		if ((cons[MtrBlower] == 1) && (cons[GOV_Reset] == 0))
		{
		  	if ( (timers[tgen] < fvars[fvgrt]) || (dpref != 0) || 
		  				(rdoutp(o_SU) == 1) || (rdoutp(o_SD) == 1) )
				setoutp(o_MTB);		  // Motor Blower output   (GEN output)
		  	else
				clroutp(o_MTB);		// turn off MTB output
		}

		if (cons[F1_ML_AL] == 1)
		{
			if (cons[equake] == 0)
			{
				if ((firef == 1) || (rfiref == 1) || (((firef == 3) || (rfiref == 3)) && (fire_floor == fvars[fvfs1f])))
					setoutp(o_F1ML);
				else
					clroutp(o_F1ML);
			}

			if (cons[medEm] == 0)
			{
				if ((firef == 2) || (rfiref == 2) || (((firef == 3) || (rfiref == 3)) && (fire_floor == fvars[fvfs1af])))
					setoutp(o_F1AL);
				else
					clroutp(o_F1AL);
			}
		}

		set_rcm();


		if ((cons[dispis] & 0x01) == 0)
		{
			if (cons[StagRear] == 1) 
			{
				if (stag_rmsk[position] == 1)
					setoutp(o_DTR);
				else
					clroutp(o_DTR);

				if (cons[exDHwL] == 0)
				{
					if (cons[vshort_tfl] == position)
						setoutp(o_SFST);
					else
						clroutp(o_SFST);
				}
			}
			else if (cons[vshort_tfl] != 0) 
			{
				if (cons[vshort_tfl] == position)
					setoutp(o_DTR);
				else
					clroutp(o_DTR);
			}
		}

		if ((cons[DZDoorEn] == 1) && (cons[medEm] == 0) && (cons[cblue] == 0))
		{
			if ((procf == 2) && (startf == 1))
				clroutp(o_DZDE);
			else if ((hsf == 0) && (door_zone == 1))
				setoutp(o_DZDE);
			else
				clroutp(o_DZDE);
		}

		if ((cons[GLBP_LED] == 1) && (cons[medEm] == 0) && (cons[equake] == 0))
		{							// Uses EMLH, EQL
			if (rdinp(i_GBP) == 1) 
				setoutp(o_GBL);
			else
				clroutp(o_GBL);  
			
			if (rdinp(i_LBP) == 1)
				setoutp(o_LBL);
			else
				clroutp(o_LBL);
		}

		if (cons[GOV_Reset] == 1)
		{
			if ((govrstf == 1) && (timers[tgovrst] < 20))
				setoutp(o_GOVR);
			else 
			{
				if (govrstf == 1)
					govrstf = 2;
				clroutp(o_GOVR);
			}

			if ((govtripf == 1) && (timers[tgovtrip] < 150))
				setoutp(o_GOVT);
			else 
			{
				if (govtripf == 1)
					govtripf = 2;
				clroutp(o_GOVT);
			}
		}
	}
}


//**********************************
// Comm control called from xx(Inctime), No longer.  Now called from main()
//**********************************
void comm_control (void)
{
	static int16 Kounter = 0;
	
//ZYM 10/16/2017            
    ReadAllSEB();  
    UpdateAllSEB();
          	  
	write_trace(TR_COMM_CONTROL);
	
	pwl_ups_fault = 0;    
    
	if (alt_can == 1)
	{
		alt_can = 0;
	   if(Enable_CT_CAN_Ctrl) ctcan_control();
	}
	else
	{
		alt_can = 1;
		if(Enable_MR_CAN_Ctrl) mrcan_control();
	}
	
	if (alt_gcom == 1)
	{
		alt_gcom = 0;
		
// ZYM 12-1-2017 work as cedes head			
//	    if(Enable_gr_CAN_Ctrl) grcan_control();
//-		if(Enable_gr_CAN_Ctrl) Ced_Can_Control();
	}
	else
	{
		alt_gcom = 1;
 	    if(Enable_RS485_Ctrl) Group_Com();  
	}
	
	// Serial Hall Lanterns
/* wbg 2018-2-22 ---> hl_com() now called from HALL_Lantern_RS485() test
	if ((cons[sercom] & 0x40) != 0)
	{
	    if(Enable_Hallcom_Ctrl) hl_com();
	}
*/	 
	 
//ZYM 10/16/2017
	CAN_Control();	 
}


//**********************************
// Car is not Dispatcher
//**********************************

void Car_Not_Dispatcher (void)
{
	int16 i;
	int16 fl_ix;
	
#if (ServiceTimer == 1)
	if (cons[loc_svc_tmr] == 1)
		service_timer_control();
#endif
	if ((cons[carnmb] > 2) && (Group_Comm == 0) && (fvars[fvemdisp] != 0))
	{			// Comm lost with group	 
		if ((c_up_hall_call[0] == 0) && (c_dn_hall_call[0] == 0) &&
			(c_up_hall_call[1] == 0) && (c_dn_hall_call[1] == 0))
		{
			if (timers[temhc1] > 20)
			{
				em_disp_hc = 1;
				for(i=1; i<=lobby_fl; i++)
				{	// Set up calls for floors below and at the lobby
					if (ucmsk[cons[carnmb]][i] == 1)
						c_up_hall_call[(i-1)/32] |= fl_mask[i];
				}
				for(i=lobby_fl+1; i <= cons[topf]; i++)
				{	// Set down calls for floors above the lobby
					if (dcmsk[cons[carnmb]][i] == 1)
						c_dn_hall_call[(i-1)/32] |= fl_mask[i];
				}
			}
		}
		else
			timers[temhc1] = 0;

		for(i=1; i<= cons[topf]; i++)
		{
			fl_ix = (i-1)/32;
			if (cancel_up == i)
				c_up_hall_call[fl_ix] &= ~fl_mask[i]; 
			if (cancel_dn == i)
				c_dn_hall_call[fl_ix] &= ~fl_mask[i];
		}

		if (cons[rear] != 0) 
		{
			if ((c_upr_hall_call[0] == 0) && (c_dnr_hall_call[0] == 0) &&
				(c_upr_hall_call[1] == 0) && (c_dnr_hall_call[1] == 0))
			{
				if (timers[temrhc1] > 20)
				{
					em_disp_hc = 1;
					for(i=1; i<=lobby_fl; i++)
					{	// Set up calls for floors below and at the lobby
						if (rucmsk[cons[carnmb]][i] == 1)
							c_upr_hall_call[(i-1)/32] |= fl_mask[i];
					}
					for(i=lobby_fl+1; i <= cons[topf]; i++)
					{	// Set down calls for floors above the lobby
						if (rdcmsk[cons[carnmb]][i] == 1)
							c_dnr_hall_call[(i-1)/32] |= fl_mask[i];
					}
				}
			}
			else
				timers[temrhc1] = 0;
			
			for(i=1; i<= cons[topf]; i++)
			{
				fl_ix = (i-1)/32;
				if (cancel_upr == i)
					c_upr_hall_call[fl_ix] &= ~fl_mask[i]; 
				if (cancel_dnr == i)
					c_dnr_hall_call[fl_ix] &= ~fl_mask[i];
			}
		}
	}
	else if (em_disp_hc == 1)
	{
		em_disp_hc = 0;
		clrhc();
	}
							
}


//*******************************************
// Door Control from Inctime
//*******************************************

void door_control (int16 start_ck)
{
	if (start_ck == 0)
	{
		// Set Manual door flag here for front door and rear door
		// Note: This flag is set for any manual door operation.  Also manual_door is set on a per
		//       floor basis for special manual and automatic door so for lock checks the cons[mand] flag is used
		//       so that the door close contact must be on to have door locks.   

		write_trace(TR_DOOR_CONTROL);
		if ((cons[mand] == 1) || (cons[mand] == 3))
		{
			manual_door = 1;		// Manual doors
			manual_rdoor = 1;
		}
		else if (cons[mand] == 2)
		{						   // Swing doors
				manual_door = 2;
				manual_rdoor = 2;
		}
		else if (((cons[mand] == 4) || (cons[mand] == 5)) && (cons[StagRear] == 1) && (stag_rmsk[position] == 1))
		{							
			if (cons[mand] == 4) 
			{					   // Automatic front with manual rear at staggard rear floor
				manual_door = 1;
				manual_rdoor = 0;
			}
			else if (cons[mand] == 5) 
			{					  // Automatic front with Swing rear at staggard rear floor
				manual_door = 2;
				manual_rdoor = 0;
			}
		}
		else if (cons[mand] == 6)	// Automatic front with swing rear door
		{
			manual_door = 0;
			manual_rdoor = 2;
		}
		else if ((cons[mand] == 7) && (cons[stag_auto_door] != 0))	// Automatic/swing fron with manual rear
		{													
			if (cons[stag_auto_door] == 1)
			{								// auto/swing front manual rear
				if (stag_ado_msk[position] == 1)
					manual_door = 2;		
				else
					manual_door = 0;
				manual_rdoor = 1;
			}
			else if (cons[stag_auto_door] == 2)
			{								 // auto/swing rear manual front
				manual_door = 1;
				if (stag_ado_rmsk[position] == 1)
					manual_rdoor = 2;
				else
					manual_rdoor = 0;
			}
			else if (cons[stag_auto_door] == 3)
			{								  // auto/swing front and rear
				if (stag_ado_msk[position] == 1)
					manual_door = 2;		
				else
					manual_door = 0;
				if (stag_ado_rmsk[position] == 1)
					manual_rdoor = 2;
				else
					manual_rdoor = 0;
			}
		}
		else if ((cons[mand] == 8) && (cons[stag_auto_door] != 0))	// Automatic/swing fron with manual rear
		{													
			if (cons[stag_auto_door] == 1)
			{								// auto/swing front manual rear
				if (stag_ado_msk[position] == 1)
					manual_door = 2;		
				else
					manual_door = 1;
				manual_rdoor = 1;
			}
			else if (cons[stag_auto_door] == 2)
			{								 // auto/swing rear manual front
				manual_door = 1;
				if (stag_ado_rmsk[position] == 1)
					manual_rdoor = 2;
				else
					manual_rdoor = 1;
			}
			else if (cons[stag_auto_door] == 3)
			{								  // auto/swing front and rear
				if (stag_ado_msk[position] == 1)
					manual_door = 2;		
				else
					manual_door = 1;
				if (stag_ado_rmsk[position] == 1)
					manual_rdoor = 2;
				else
					manual_rdoor = 1;
			}
		}
		else if ((cons[mand] == 9)&&(cons[stag_auto_door] != 0))
		{
			manual_door = 0;
			manual_rdoor = 0;
			if (stag_ado_msk[position] == 1)
			{
				auto_rcm = 1;
				manual_door = 2;
			}
			else if	(stag_ado_rmsk[position] == 1)
			{
				auto_rcm = 2;
				manual_rdoor = 2;
			}
			else
				auto_rcm = 0;

		}		
		else 
		{
			manual_door = 0;
			manual_rdoor = 0;
		}

		// INSO Inspection output for otis ovl door operator

		if(cons[GLBP_LED] == 0)
		{
			if (cons[door_type] == 1)	// otis ovl door operator
			{
				if (chk_ins() == 1)				 // turn on inspection output for otis ovl door operator
					setoutp(o_INSO);
				else
					clroutp(o_INSO);
			}
		}		

		// 2=peele,3=courion,4=ems
		if (cons[frghtd] == 2)
			peelle_doors();

		if ((cons[frghtd] == 10) || (cons[frghtd] == 11) || (cons[frghtd] == 12))
			auto_peelle_doors();

		if ((cons[frghtd] == 3) || (cons[frghtd] == 5))
			courion_doors();

		if (cons[frghtd] == 4)
			ems_doors();

		// Set for automatic freight door operation 

		if ((cons[frghtd] == 6) || (cons[frghtd] == 7) || (cons[frghtd] == 8))
			auto_freight_doors();
		
		if (cons[frghtd] == 9)
			guilbert_doors();
	
		if (cons[heavy_door] == 1)
		{
			if (heavy_door_fmsk[position] == 1)
				setoutp(o_HVD);
			else
				clroutp(o_HVD);

			if (heavy_door_rmsk[position] == 1)
				setoutp(o_HVDR);
			else
				clroutp(o_HVDR);
		}
		
		if (cons[Auto_Swing_DO] == 2)	
			auto_swg_doors();
	}
}

//*******************************************
// Buzzer Control from Inctime
//*******************************************

void light_buzzer_control (int16 start_ck)
{
	int16 keep_timer_zero;
	int16 keep_fan_on;

#if (Hollister) == 1
	static int16 sec_count;

		// 	Temp for Hollister
			
	if (start_ck == 0)
	{

		write_trace(TR_LIGHT_BUZZER_CONTROL);
		if (timers[tsec] != 0)
		{
			if (sec_count >= 1)
			{			 
				sec_count = 0;
				if (fvars[fvhcboncolor] != 0)
				{
					if (fvars[fvhcboncolor] < 15)
						fvars[fvhcboncolor] ++;
					else
						fvars[fvhcboncolor] = 1;

					fvars[fvhcboffcolor] = fvars[fvhcboncolor];
					calculate_button_color();
				}
			}
		}
		else
			sec_count ++;
	}
#endif


	if (start_ck == 0)
	{
		if ((cons[hben] & 0x01) != 0)
		{
			if ((rd_both_inps(i_HBE,i_XHBE) == 1) || ((cons[rear] != 0) && (rd_both_inps(i_HBE,i_XHBE) == 1)))
				hb_en = 1;			
		}
		else
			hb_en = 1;

		// Timing for Cab Light and fan control
		
		if ((((servf == s_ELEV_OFF_HS) || (servf == s_ELEV_OFF_CS)) && ((fvars[fvheloff] & 0x04) != 0)) ||
			((servf == s_CAR_OFF) && ((fvars[fvceloff] & 0x04) != 0)))
			keep_timer_zero = 0;	  // time exp okay with door open
		else if ((doorf != 0) || ((cons[rear] != 0) && (rdoorf != 0))) 
			keep_timer_zero = 1;
		else
			keep_timer_zero = 0;

		if ((dpref != 0) || (keep_timer_zero == 1) ||
			(up_level == 0) || (dn_level == 0) || (door_zone == 0) ||
			(rdoutp(o_SU) == 1) || (rdoutp(o_SD) == 1) )
		{		 // IF a run preference, doors open, not in the door zone or moving then zero 
				 // the timer to turn on the lights
			timers[tclf] = 0;
		}


		if ((servf != s_AUTO) && (((servf != s_ELEV_OFF_HS) && (servf != s_ELEV_OFF_CS)) || ((fvars[fvheloff] & 0x04) == 0)))
			keep_fan_on = 1;
		else
			keep_fan_on = 0;
			
		if ((timers[tclf] < fvars[fvgrt]) || (keep_fan_on == 1))
		{
			if (fvars[fvclf] == 0)
			{
				setoutp(o_LIGHT);
				setoutp(o_FAN);
				setoutp(o_CLF);	 // Cab light fan
			}
			else
			{
				clroutp(o_LIGHT);
				clroutp(o_FAN);
				clroutp(o_CLF);
			}
		}
		else
		{
			if (fvars[fvclf] == 0)
			{
				clroutp(o_LIGHT);
				clroutp(o_FAN);
				clroutp(o_CLF);
			}
			else
			{
				setoutp(o_LIGHT);
				setoutp(o_FAN);
				setoutp(o_CLF);	 // Cab light fan
			}
		}

		// Set output to indicate car is on fire service
		if ((fvars[fvfsofl] & 0x04) != 0)
		{
			if (rdoutp(o_FL) == 1)
			{
				setoutp(o_FSO);
				setoutp(o_FLH);
			}
			else
			{
				clroutp(o_FSO);
				clroutp(o_FLH);
			}
				
		}
		else if ((fvars[fvfsofl] & 0x01) == 0)
		{
			if (firef != 0)
	    	{
	      		if (((fvars[fvfsofl] & 0x02) != 0) && (timers[tsec] != 0))
	      		{
	      			clroutp(o_FSO);
	      			clroutp(o_FLH);
	      		}
				else				
				{
					setoutp(o_FSO);
					setoutp(o_FLH);
				}
			}
			else
			{
				clroutp(o_FSO);
				clroutp(o_FLH);
			}
				
		}
		else 
		{
			if (fs1_effect != 0)
	    	{
	      		if (((fvars[fvfsofl] & 0x02) != 0) && (timers[tsec] != 0))
	      		{
					clroutp(o_FSO);
					clroutp(o_FLH);
	      		}
				else				
				{
					setoutp(o_FSO);
					setoutp(o_FLH);
				}
			}
			else
			{
				clroutp(o_FSO);
				clroutp(o_FLH);
			}
		}

		if ((cons[FI_EP_Ext] != 0) || (cons[FiCntrlCentr] == 1) || (cons[Australia] == 1))
		{				// Car emregency power hall light operation
			if (empf != 0)
			{
				setoutp(o_EPLH);
				if (empf == ep_recall)		// flash Recall light
				{
					if (timers[tsec] != 0)
						setoutp(o_EPRL);
					else
						clroutp(o_EPRL);
					clroutp(o_EPSL);
				}
				else if ((empf == ep_home) || (empf == ep_home_close))	// on steady
				{
					setoutp(o_EPRL);
					clroutp(o_EPSL);
				}
				else if (empf == ep_select)	// Selected to run
				{
					setoutp(o_EPSL);
					clroutp(o_EPRL);
				}
				else if ((empf == ep_wait) || (empf == ep_wait_door))	// clear otherwise 
				{
					clroutp(o_EPSL);
					clroutp(o_EPRL);
				}
			}
			else
			{
				clroutp(o_EPLH);
				clroutp(o_EPRL);
				clroutp(o_EPSL);
			}

		}	
		  
		if (cons[FiCntrlCentr] == 1)
		{
			if (rdoutp(o_FL) == 1)
				setoutp(o_FS1L);
			else			  
				clroutp(o_FS1L);
			
			if ((firef > 3)  || (rfiref > 3))
				setoutp(o_FS2L);
			else			  
				clroutp(o_FS2L);
			
			if (empf == 0)
				setoutp(o_NPL);
			else			  
				clroutp(o_NPL);

		}	
			
		

		if (cons[RGB_CC] == 1)
		{
			if (rdoutp(o_FL) == 1)
				setoutp(o_FiFL);
			else
				clroutp(o_FiFL);
		}

		if ((cons[F1_CT] == 1) && ((cons[sercom] & 0x04) != 0))
		{
			if (rdoutp(o_FL) == 1)
				setoutp(o_FLCT);
			else
				clroutp(o_FLCT);

			if (rdoutp(o_FBNB) == 1)
				setoutp(o_FBNBCT);
			else
				clroutp(o_FBNBCT);
		}


		//Set or clear the att_buz_hc
		if(fvars[fvhcattbuz] == 1)
		{
			if((timers[tsec] == 0))
				att_buz_hc = 0;
			if((att_nmb_hc2buz>0) && (att_buz_hc ==0) && (att_buz_hc_en == 0) && ((timers[tsec] == 0)))
			{
				att_nmb_hc2buz--;
				att_buz_hc_en = 1;
			}
			if (att_buz_hc_en == 1)
			{
				if (timers[tsec] != 0)
				{
					att_buz_hc = 1;
					att_buz_hc_en = 0;
				}

			}
		}
		else
		{
			att_buz_hc_en = 0;
			att_buz_hc_en = 0;		
		}
		// Set or clear car buzzer

		// fire buzzer, nudging buzzer and medical emergency buzzer are the same output
		if ((fire_buz == 1) || (nudg_buz == 1) || (rnudg_buz == 1) || 
			(med_buz  == 1) || (att_buz_hc == 1) || ((att_buz == 1) && (att_buz_on == 1)) || (door_buz == 1) || (
			(gripper_fault == 1) && (fvars[fvgripbz] == 1)) )
			setoutp(o_FBNB);      // Turn on the buzzer
		else
			clroutp(o_FBNB);      // Turn off the buzzer

		if ((hb_en == 1) && (hb_cc_buz == 1))
		{
			if (timers[thbcc] >= fvars[fvhboncct])
				hb_cc_buz = 0;
		}

		if ((cons[frghtd] == 6) || (cons[frghtd] == 7) || (cons[frghtd] == 8))
		{
			if ((freight_fdoor_alarm == 1) || (freight_rdoor_alarm == 1))
				setoutp(o_DCA);
			else
				clroutp(o_DCA);
		}

		if ((cons[frghtd] == 10) || (cons[frghtd] == 11) || (cons[frghtd] == 12))
		{
			if ((freight_fdoor_alarm == 1) || (freight_rdoor_alarm == 1))
				setoutp(o_PDCA);
			else
				clroutp(o_PDCA);
		}

		if ((procf == 2) || (chk_ins() == 1))
		{				 // Inspection Fire outputs
			
			if (rdoutp(o_FL) == 1)
				setoutp(o_IFL);      // Turn on the machine room buzzer
			else
				clroutp(o_IFL);      // Turn off the machine room buzzer
		
		
			if (fire_buz == 1)
				setoutp(o_IFB);      // Turn on the machine room buzzer
			else
				clroutp(o_IFB);      // Turn off the machine room buzzer
		}
			

		if (fire_buz == 1)
			setoutp(o_BUZ);      // Turn on the machine room buzzer
		else
			clroutp(o_BUZ);      // Turn off the machine room buzzer

		if ( ((handicap_buz == 1) 
			 || ((hb_cc_buz == 1) && (((cons[hben] & 0x02) != 0) || ((cons[hben] & 0x04) != 0))))
			 || (cc_sec_buz == 1))
			setoutp(o_HB);
		else
		 	clroutp(o_HB);
		
		if (cons[Sabbath] != 0) 
		{
			if ((firef != 0) || (rfiref != 0))
			{
				sabb_buz = 0;
				sabb_ee_en = 0;
			}
			if(cons[Super_Sabbath] == 1)
			{
				if (sabb_buz ==1)
				{
					setoutp(o_SABUZ);
				}
				else
				{
					clroutp(o_SABUZ);
				}
				if(sabb_ee_en == 1)
				{
					setoutp(o_SABEE);
				}
				else
				{
					clroutp(o_SABEE);	
				}
			}
		}

		if (cons[Sabbath] != 0)
		{
			if ((cons[Sabbath] == 7) && ((firef != 0) || (rfiref != 0) || (medf != 0) || (empf != 0)))
			{	// fire service or med, emp serv change the masks to normal masks
				sabbath_active = 0;
				if (sabbath_auto_calls != 0)
					sabbath_tables_cc();
			}
			else if ((firef != 0) || (rfiref != 0))
				sabbath_active = 0;
			else if ((rdinp(i_SAB) == 1) && (c_tmr_svc_en[s_sabbath] == 0))
				sabbath_active = 1;			// on sabbath operation
			else if (c_tmr_svc_en[s_sabbath] == 1)
			{
				if (c_tmr_svc_logic[s_sabbath] == 1)
				{
					if ((rdinp(i_SAB) == 1) || (c_tmr_svc[s_sabbath] == 1))
						sabbath_active = 1;			// on sabbath operation
					else
						deactivate_sabbath();
						
				}
				else if (c_tmr_svc_logic[s_sabbath] == 2)
				{
					if ((rdinp(i_SAB) == 1) && (c_tmr_svc[s_sabbath] == 0))
						sabbath_active = 1;			// on sabbath operation
					else
						deactivate_sabbath();
				}
				else if (c_tmr_svc_logic[s_sabbath] == 3)
				{
					if ((rdinp(i_SAB) == 1) && (c_tmr_svc[s_sabbath] == 1))
						sabbath_active = 1;			// on sabbath operation
					else
					   deactivate_sabbath();
				}
				else
				{
					if (rdinp(i_SAB) == 1)
						sabbath_active = 1;			// on sabbath operation
					else
					   deactivate_sabbath();			
				}
			}
			else
			{
					 // Go off of sabbath immediately or when car reaches lobby floor
				deactivate_sabbath();
			}

		}

	  	if ((((fvars[fvsapb] & 0x01) != 0) && (cons[nmbcars] == 1)) || ((fvars[fviser] & 0x02) != 0))
	  	{
      		if ((rdinp(i_DOL) == 0) || (dpref != 0) || // if door is open or there is an onward pilot
				((cons[rear] != 0) && (rdinp(i_DOLR) == 0)))
				in_serv_lt = 0;
			else
				in_serv_lt = 1;
			
			if ((manual_door == 2) || (manual_rdoor == 2))
			{
				if ((man_dcc() == 0) || (dpref != 0))	
					in_serv_lt = 0;
				else
					in_serv_lt = 1;
			}
		}
		else
			in_serv_lt = 1;

		if ((Drive_Type[cons[carnmb]] == 1) || (Drive_Type[cons[carnmb]] == 9))  
		{		   // Hydro car
			// Inspection speed output
			if ((servf == s_INSP) && (procf == 2))		// run faster on inspection on inspection
				setoutp(o_ISPD);	  
			else
				clroutp(o_ISPD);
		}
		
		if ((cons[servOUT] & 0x04) != 0)
		{
			// Set in service light
			if ( ( (servf == s_AUTO) || (servf == s_ATT) || (servf == s_HOMING) || 
				   (  ( ((fvars[fviser] & 0x04) != 0) && 
						  ( (servf == s_IND) || (procf == 2) || 
						  	((safe() == 1) && ((servf == s_RESET_UP) || (servf == s_RESET_DN) || (procf == 1)))
						  ) 
					  ) 
				   ) 
			     ) && (in_serv_lt == 1) && (estop == 0))
			{					// Car is in service (if (fviser & 0x04) != 0 then car still operational)
				if ((fvars[fviser] & 0x01) == 0)
					setoutp(o_ISER);	   // auto, attendant, or homing(parking) or special case to not show inspection as out of service to use iser as an alarm
				else
					clroutp(o_ISER);
			}
			else
			{									  // invert logic of in-service light
				if ((fvars[fviser] & 0x01) == 0)
					clroutp(o_ISER);
				else
					setoutp(o_ISER);	   // auto, attendant, or homing(parking)
			}
		}
		

		car_alrm = (rd_both_inps(i_ALRM,i_XALRM) == 1) || ((cons[rear] != 0) && (rd_both_inps(i_ALRMR,i_XALRMR) == 1));

		if(oserl_enable == 1)
		{
			if((ind() == 0) && (chk_ins() == 0) && (firef == 0))
			{
				oser_lt = 0;
				oser_bz = 0;
				oser_bz_pulse = 0;
				if (cons[distress] == 2)
				{
					// out of service light output
					if ((servf == s_AUTO) || (servf == s_IND) || (servf == s_ATT) || (servf == s_HOMING))
						oser_lt = 0;
					else 			
						oser_lt = 1;
					
					// out of service stop switch input off buzzer
					if (((fvars[fvoser1] & 0x04) != 0) && (rdinp(i_MRSW) == 0) && (in_redundancy_test == 0))
						oser_bz = 1;
					else
						oser_bz = 0;
					
					// door open button stuck so set output
					if ((closetry >= 6) || ((cons[rear] != 0) && (rclosetry >= 6)))
						setoutp(o_OSERD);
					else
						clroutp(o_OSERD);
				}
				else
				{
					if((fvars[fvoser1] == 0) && (fvars[fvoser2] == 0))
					{
						if((servf == s_AUTO)||(servf == s_IND)||(servf == s_ATT)|| (servf == s_HOMING))
							oser_lt = 0;
						else 			
							oser_lt = 1;	
					}
					else
					{
						if(((fvars[fvoser1] & 0x01) != 0)&&(dcalls != 0)&&(timers[tosernocall] > fvars[fvosernocall]))
						{
							oser_lt = 1;
							oser_bz = 1;
							oser_bz_pulse = 1;
						}
						else if (((front_slowdown & (cc_do | uc_do | dc_do)) != 0) || ((rear_slowdown & (cc_do | uc_do | dc_do)) != 0 ))
							timers[tosernocall] = 0;
						else if (dcalls == 0)
							timers[tosernocall] = 0;
							

						if(((fvars[fvoser1] & 0x02) != 0) && ((door_zone == 0) && (timers[toserbtwfl] > fvars[fvoserbtwfl])))
							oser_lt = 1;
						else if(door_zone == 1)
							timers[toserbtwfl] = 0;

						if(((fvars[fvoser1] & 0x04) != 0) && (rdinp(i_MRSW) == 0) && (timers[toserssopen] > fvars[fvoserssopen]))
						{
							oser_lt = 1;
							oser_bz = 1;
						}
						else if (rdinp(i_MRSW) == 1)
							timers[toserssopen] = 0;
						if ( ((fvars[fvoser1] & 0x01) != 0) && ((rd_both_inps(i_ALRM,i_XALRM) == 1) || ((cons[rear] != 0) && (rd_both_inps(i_ALRMR,i_XALRMR) == 1))) )
						{
							oser_lt = 1;
							oser_bz = 1;
						}
								
					}
				}
				
				if (cons[distress] != 0)
				{
					if(rdinp(i_OSERA) == 1)
					{
						oser_lt = 0;
						oser_bz = 0;
						oser_bz_pulse = 0;
						timers[tosernocall] = 0;
						timers[toserbtwfl] = 0;
						timers[toserssopen] = 0;
					}
					if(oser_bz == 1)
					{
						if((oser_bz_pulse == 1) && (timers[tsec] != 0))
						{
							clroutp(o_OSERB);

						}
						else if (oser_bz == 1)
						{
							setoutp(o_OSERB);
						}
						else
							clroutp(o_OSERB);
					}
					else
					{
						clroutp(o_OSERB);
					}

				}

				if(oser_lt == 1)
				{
					if ((fvars[fviser] & 0x01) == 0)
						setoutp(o_OSERL);
					else
						clroutp(o_OSERL);
				}
				else
				{
					if ((fvars[fviser] & 0x01) == 0)
						clroutp(o_OSERL);
					else
						setoutp(o_OSERL);
				}
			}	
			else if (firef == 0)
			{
				if(cons[distress] == 0)
				{
					if ((fvars[fviser] & 0x01) == 0)
						setoutp(o_OSERL);
					else
						clroutp(o_OSERL);
				}
			}	
			else
			{
				if ((fvars[fviser] & 0x01) == 0)
					clroutp(o_OSERL);
				else
					setoutp(o_OSERL);
			}
		}
		if ((cons[servOUT] & 0x01) != 0)
		{
			if(servf == fvars[fvservoctrl])
				setoutp(o_SERV);
			else
				clroutp(o_SERV);
		}
		if ((cons[servOUT] & 0x02) != 0)
		{
			if(servf == fvars[fvservoctrl])
				setoutp(o_SERVC);
			else
				clroutp(o_SERVC);
		}
		if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) || ((cons[Car_Sw] & 1) != 0))
			set_att_annunciator();
	}
}


void set_att_annunciator(void)
{
	int16 i;
	int16 fl_ix;
	
	for (i=cons[bottomf];i<=cons[topf];i++)
	{
		fl_ix = (i-1)/32;
		if (cons[Second_Riser] != 0)
		{
			uiu.dw = (c_up_hall_call[fl_ix] | c_irup_hall_call[fl_ix]);
			uiu2.dw = (c_dn_hall_call[fl_ix] | c_irdn_hall_call[fl_ix]);
		}
		else
		{
			uiu.dw = c_up_hall_call[fl_ix];
			uiu2.dw = c_dn_hall_call[fl_ix];
		}
		if ((uiu.dw & fl_mask[i]) != 0)
			setoutp(o_UAL(i));
		else
			clroutp(o_UAL(i));
		
		if ((uiu2.dw & fl_mask[i]) != 0)
			setoutp(o_DAL(i));
		else
			clroutp(o_DAL(i));
	}		
	if (cons[rear] != 0)
	{
		for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
		{
			fl_ix = (i-1)/32;
			if (cons[Second_Riser] != 0)
			{
				uiu.dw = (c_upr_hall_call[fl_ix] | c_rirup_hall_call[fl_ix]);
				uiu2.dw = (c_dnr_hall_call[fl_ix] | c_rirdn_hall_call[fl_ix]);
			}
			else
			{
				uiu.dw = c_upr_hall_call[fl_ix];
				uiu2.dw = c_dnr_hall_call[fl_ix];
			}
			if ((uiu.dw & fl_mask[i]) != 0)
				setoutp(o_UALR(i));
			else
				clroutp(o_UALR(i));
			
			if ((uiu2.dw & fl_mask[i]) != 0)
				setoutp(o_DALR(i));
			else
				clroutp(o_DALR(i));
		}		
		
	}
}


//*************************************************
// Check Motion Fault called from Inctime
//*************************************************

void chk_motion_fault (int16 start_ck)
{
	int16 i;
	
	write_trace(TR_CHK_MOTION_FAULT1);
	
	if (fvars[fvencinterval] < 1)
		enc_dbn_interval = 1;
	else
		enc_dbn_interval = fvars[fvencinterval];

	if (fvars[fvencsamples] < 2)
		enc_nmb_samples = 2;
	else
		enc_nmb_samples = fvars[fvencsamples];

	if (enc_vel_timer >= enc_dbn_interval) 
	{			
  		if (DPP_Count > Prev_DPP_Count)
		{
			t_Enc_dir = 1;			// Up direction
  			vel_DPP[vel_DPP_ptr++] = (DPP_Count - Prev_DPP_Count);		// Store dpp count at 10msec interval 
		}
  		else if (DPP_Count < Prev_DPP_Count)
		{
			t_Enc_dir = 2;			// Down Direction
			vel_DPP[vel_DPP_ptr++] = (Prev_DPP_Count - DPP_Count);		// Store dpp count at 10msec interval 
		}
		else				
		{
			t_Enc_dir = 0;			// No direction
			vel_DPP[vel_DPP_ptr++] = 0;
		}

  		Prev_DPP_Count = DPP_Count;

		if ((t_Enc_dir == Prev_Enc_dir) && (enc_dir_timer >= 3))
		{
			Enc_dir = t_Enc_dir;
			enc_dir_timer = 0;
		}

		Prev_Enc_dir = t_Enc_dir;

		// Update velocity calculation from encoder every 10 msec
		if (vel_DPP_ptr > (enc_nmb_samples - 1))
			vel_DPP_ptr = 0;

		tvel_DPP = 0;

		for(i=0;i<=(enc_nmb_samples - 1);i++)
			tvel_DPP = tvel_DPP + vel_DPP[i];		
		Enc_vel = (float)tvel_DPP;
		Enc_vel = Enc_vel * ((float)100.0/(float)(enc_nmb_samples * enc_dbn_interval));	  // * 100 to get seconds / 10 samples

		enc_vel_timer -= enc_dbn_interval;		
	}

	enc_vel_fpm = (int16)(((Enc_vel * 5.0)/(Pulses_per_Inch)) + 0.5);

	if (start_ck == 0)
	{
		write_trace(TR_CHK_MOTION_FAULT4);

	}
}

//******************************************
// Security control set from Inctime
//******************************************
void security_control (int16 start_ck)
{
	int16 i;
  	int16 fl;
  	int16 fl_ix;

	if (start_ck == 0)
	{
		write_trace(TR_SECURITY_CONTROL);
		if (cons[cc_pb_sec] == 1)
		{
			for (i = 0; i <= 3; i++)
			{
				if (Clear_All_Codes_All_Floors(0,i) != 0)		// Delete codes in multiple calls
					break;
				if (Add_One_Code_All_Floors(0,i,zero_Code) != 0)  // add codes in multiple calls
					break; 
				if (Delete_One_Code_All_Floors(0,i,zero_Code) != 0)	  // delete codes in multiple calls
					break;
			}
		}
	  	if ((cons[SecFlCfg] == 1) || (cons[SecFlCfg] == 5))
	  	{
			if (((rdinp(i_SECFM) == 1) || (c_tmr_svc[s_alt_fl_sec] == 1) || 
				(((cons[ids] & 1) != 0) && (ids_alt_fl_sec == 1)) ||
				(((cons[Galileo] & 1) != 0) && (ui_alt_fl_sec == 1))) && 
					(firef == 0) && (medf == 0) && (empf == 0) && (ind_secf_override == 0))
			{
				if (Sec_Fl_Mask == 0)
				{
					Sec_Fl_Mask = 1;
				    for(i=1; i<car_size; i++)
				    {
						ccmsk[i] = &ccsmsk[i][0];
						ucmsk[i] = &ucsmsk[i][0];
						dcmsk[i] = &dcsmsk[i][0];
						rccmsk[i] = &rccsmsk[i][0];
						rucmsk[i] = &rucsmsk[i][0];
						rdcmsk[i] = &rdcsmsk[i][0];
						if (cons[LbyUpReq] != 0)
						{
							if ((lby_up_req_car[i] & 0x04) != 0)
								lby_up_req_car[i] |= 0x02;		  // set flag to disable standard up hall call
						}	
					}
				 	clrcc(1);		// cancel the call with the invalid mask
				}
			}
			else
			{
				if (Sec_Fl_Mask != 0)
				{
					Sec_Fl_Mask = 0;
				    for(i=1; i<car_size; i++)
				    {
						ccmsk[i] = &ccnmsk[i][0];
						ucmsk[i] = &ucnmsk[i][0];
						dcmsk[i] = &dcnmsk[i][0];
						rccmsk[i] = &rccnmsk[i][0];
						rucmsk[i] = &rucnmsk[i][0];
						rdcmsk[i] = &rdcnmsk[i][0];
						if ((cons[LbyUpReq] & 0x04) != 0)
							lby_up_req_car[i] &= ~0x02;			// clear disable bit
					}
				 	clrcc(1);	// cancel the call with the invalid mask
				}
			}
		}
	  	else if (cons[SecFlCfg] == 2)
		{
			if ((car_Grp_Stat.c_sec_fl_ctl == 0) || (ind_secf_override != 0) ||(firef != 0) || (medf != 0) || (empf != 0))
			{
				if (Sec_Fl_Mask != 0)
				{
					Sec_Fl_Mask = 0;
					ccmsk[cons[carnmb]] = &ccnmsk[cons[carnmb]][0];
					ucmsk[cons[carnmb]] = &ucnmsk[cons[carnmb]][0];
					dcmsk[cons[carnmb]] = &dcnmsk[cons[carnmb]][0];
					rccmsk[cons[carnmb]] = &rccnmsk[cons[carnmb]][0];
					rucmsk[cons[carnmb]] = &rucnmsk[cons[carnmb]][0];
					rdcmsk[cons[carnmb]] = &rdcnmsk[cons[carnmb]][0];
					if ((fvars[fvsecreasgncc] != 0) && (cons[rear] != 0))
					{	// reassign secured floors	 
						for (i=cons[bottomf]; i<=cons[topf]; i++)
						{
							if ((fvars[fvsecreasgncc] & 0x01) != 0)
							{	// assign front car calls as rear	
								if ((ccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CC(i)) == 1))
									set_rcarcall(cons[carnmb],i);
							}
							
							if ((fvars[fvsecreasgncc] & 0x02) != 0)
							{	// assign rear car calls as front
								if ((rccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CCR(i)) == 1))
									set_carcall(cons[carnmb],i);
							}
						}
					}
				 	clrcc(1);		// cancel the call with the invalid mask
				}
			}
			else if (car_Grp_Stat.c_sec_fl_ctl == 1)
			{
				if (Sec_Fl_Mask != 1)
				{
					Sec_Fl_Mask = 1;
					ccmsk[cons[carnmb]] = &ccsmsk[cons[carnmb]][0];
					ucmsk[cons[carnmb]] = &ucsmsk[cons[carnmb]][0];
					dcmsk[cons[carnmb]] = &dcsmsk[cons[carnmb]][0];
					rccmsk[cons[carnmb]] = &rccsmsk[cons[carnmb]][0];
					rucmsk[cons[carnmb]] = &rucsmsk[cons[carnmb]][0];
					rdcmsk[cons[carnmb]] = &rdcsmsk[cons[carnmb]][0];
					if ((fvars[fvsecreasgncc] != 0) && (cons[rear] != 0))
					{	// reassign secured floors	 
						for (i=cons[bottomf]; i<=cons[topf]; i++)
						{
							if ((fvars[fvsecreasgncc] & 0x01) != 0)
							{	// assign front car calls as rear	
								if ((ccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CC(i)) == 1))
									set_rcarcall(cons[carnmb],i);
							}
							
							if ((fvars[fvsecreasgncc] & 0x02) != 0)
							{	// assign rear car calls as front
								if ((rccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CCR(i)) == 1))
									set_carcall(cons[carnmb],i);
							}
						}
					}
				 	clrcc(1);		// cancel the call with the invalid mask
				}
			}
			else if ((car_Grp_Stat.c_sec_fl_ctl == 2) && (cons[SecFlCfg_2] != 0))
			{
				if (Sec_Fl_Mask != 2)
				{
					Sec_Fl_Mask = 2;
					ccmsk[cons[carnmb]] = &ccs2msk[cons[carnmb]][0];
					ucmsk[cons[carnmb]] = &ucs2msk[cons[carnmb]][0]; 
					dcmsk[cons[carnmb]] = &dcs2msk[cons[carnmb]][0];
					rccmsk[cons[carnmb]] = &rccs2msk[cons[carnmb]][0];
					rucmsk[cons[carnmb]] = &rucs2msk[cons[carnmb]][0]; 
					rdcmsk[cons[carnmb]] = &rdcs2msk[cons[carnmb]][0];
					if ((fvars[fvsecreasgncc] != 0) && (cons[rear] != 0))
					{	// reassign secured floors	 
						for (i=cons[bottomf]; i<=cons[topf]; i++)
						{
							if ((fvars[fvsecreasgncc] & 0x01) != 0)
							{	// assign front car calls as rear	
								if ((ccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CC(i)) == 1))
									set_rcarcall(cons[carnmb],i);
							}
							
							if ((fvars[fvsecreasgncc] & 0x02) != 0)
							{	// assign rear car calls as front
								if ((rccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CCR(i)) == 1))
									set_carcall(cons[carnmb],i);
							}
						}
					}
				 	clrcc(1);		// cancel the call with the invalid mask
				}
			}
			else if ((car_Grp_Stat.c_sec_fl_ctl == 3) && (cons[SecFlCfg_3] != 0))
			{
				if (Sec_Fl_Mask != 3)
				{
					Sec_Fl_Mask = 3;
					ccmsk[cons[carnmb]] = &ccs3msk[cons[carnmb]][0];
					ucmsk[cons[carnmb]] = &ucs3msk[cons[carnmb]][0];
					dcmsk[cons[carnmb]] = &dcs3msk[cons[carnmb]][0];
					rccmsk[cons[carnmb]] = &rccs3msk[cons[carnmb]][0];
				 	rucmsk[cons[carnmb]] = &rucs3msk[cons[carnmb]][0];
				 	rdcmsk[cons[carnmb]] = &rdcs3msk[cons[carnmb]][0];
				 	if ((fvars[fvsecreasgncc] != 0) && (cons[rear] != 0))
					{	// reassign secured floors	 
						for (i=cons[bottomf]; i<=cons[topf]; i++)
						{
							if ((fvars[fvsecreasgncc] & 0x01) != 0)
							{	// assign front car calls as rear	
								if ((ccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CC(i)) == 1))
									set_rcarcall(cons[carnmb],i);
							}
							
							if ((fvars[fvsecreasgncc] & 0x02) != 0)
							{	// assign rear car calls as front
								if ((rccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CCR(i)) == 1))
									set_carcall(cons[carnmb],i);
							}
						}
					}
				 	clrcc(1);		// cancel the call with the invalid mask
				}
			}
			else if ((car_Grp_Stat.c_sec_fl_ctl == 4) && (cons[SecFlCfg_4] != 0))
			{
				if (Sec_Fl_Mask != 4)
				{
					Sec_Fl_Mask = 4;
					ccmsk[cons[carnmb]] = &ccs4msk[cons[carnmb]][0];
					ucmsk[cons[carnmb]] = &ucs4msk[cons[carnmb]][0];
					dcmsk[cons[carnmb]] = &dcs4msk[cons[carnmb]][0];
					rccmsk[cons[carnmb]] = &rccs4msk[cons[carnmb]][0];
					rucmsk[cons[carnmb]] = &rucs4msk[cons[carnmb]][0];
					rdcmsk[cons[carnmb]] = &rdcs4msk[cons[carnmb]][0];
				 	if ((fvars[fvsecreasgncc] != 0) && (cons[rear] != 0))
					{	// reassign secured floors	 
						for (i=cons[bottomf]; i<=cons[topf]; i++)
						{
							if ((fvars[fvsecreasgncc] & 0x01) != 0)
							{	// assign front car calls as rear	
								if ((ccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CC(i)) == 1))
									set_rcarcall(cons[carnmb],i);
							}
							
							if ((fvars[fvsecreasgncc] & 0x02) != 0)
							{	// assign rear car calls as front
								if ((rccmsk[cons[carnmb]][i] == 0) && (rdinp(i_CCR(i)) == 1))
									set_carcall(cons[carnmb],i);
							}
						}
					}
				 	clrcc(1);		// cancel the call with the invalid mask
 				}
			}
		}
		else if (cons[SecFlCfg] == 6)
		{  
			if ((secf_msks_applied == 1) && 
				((car_Grp_Stat.c_sec_fl_ctl == 0) || (firef != 0) || 
				 (medf != 0) || (empf != 0)))
			{
				secf_msks_applied = 0;
				ccmsk[cons[carnmb]] = &ccnmsk[cons[carnmb]][0];
				ucmsk[cons[carnmb]] = &ucnmsk[cons[carnmb]][0];
				dcmsk[cons[carnmb]] = &dcnmsk[cons[carnmb]][0];
				rccmsk[cons[carnmb]] = &rccnmsk[cons[carnmb]][0];
				rucmsk[cons[carnmb]] = &rucnmsk[cons[carnmb]][0];
				rdcmsk[cons[carnmb]] = &rdcnmsk[cons[carnmb]][0];
			 	clrcc(1);		// cancel the call with the invalid mask
			}
			else if ((car_Grp_Stat.c_sec_fl_ctl == 1) && (secf_msks_applied == 0))
			{
				secf_msks_applied = 1;
				ccmsk[cons[carnmb]] = &ccsmsk[cons[carnmb]][0];
				ucmsk[cons[carnmb]] = &ucsmsk[cons[carnmb]][0];
				dcmsk[cons[carnmb]] = &dcsmsk[cons[carnmb]][0];
				rccmsk[cons[carnmb]] = &rccsmsk[cons[carnmb]][0];
				rucmsk[cons[carnmb]] = &rucsmsk[cons[carnmb]][0];
				rdcmsk[cons[carnmb]] = &rdcsmsk[cons[carnmb]][0];
				clrcc(1);	// clear car calls with invalid mask
			}
		}
		else if (cons[SecFlCfg] == 7)
		{
			if ((fvars[fvccpbsongpccsec] == 0) || 
				((fvars[fvccpbsongpccsec] == 1) && (cons[cc_pb_sec] == 0)))
			{
				if ((secf_msks_applied == 1) && 
					(((gc_ccs_per_fl[0][cons[carnmb]] == 0) && (gc_ccs_per_fl[1][cons[carnmb]] == 0)) ||
					(firef != 0) || (medf != 0) || (empf != 0)))
				{
					for (i=0;i<=1;i++)
						prev_g_cc_sec[i] = 0;
					secf_msks_applied = 0;
					ccmsk[cons[carnmb]] = &ccnmsk[cons[carnmb]][0];
					ucmsk[cons[carnmb]] = &ucnmsk[cons[carnmb]][0];
					dcmsk[cons[carnmb]] = &dcnmsk[cons[carnmb]][0];
					rccmsk[cons[carnmb]] = &rccnmsk[cons[carnmb]][0];
					rucmsk[cons[carnmb]] = &rucnmsk[cons[carnmb]][0];
					rdcmsk[cons[carnmb]] = &rdcnmsk[cons[carnmb]][0];
				 	clrcc(1);		// cancel the call with the invalid mask		
				}
				else if (((gc_ccs_per_fl[0][cons[carnmb]] != 0) && (gc_ccs_per_fl[0][cons[carnmb]] != prev_g_cc_sec[0])) ||
						 ((gc_ccs_per_fl[1][cons[carnmb]] != 0) && (gc_ccs_per_fl[1][cons[carnmb]] != prev_g_cc_sec[1])))
				{
					for (i=0;i<=1;i++)
						prev_g_cc_sec[i] = gc_ccs_per_fl[i][cons[carnmb]];
					secf_msks_applied = 1;
					
					// change car call mask to what you get from the dispatcher
					for (i=cons[bottomf];i<=cons[topf];i++)
					{
						fl_ix = (i-1)/32;
						// check if call is secured
						if ((gc_ccs_per_fl[fl_ix][cons[carnmb]] & fl_mask[i]) != 0)
							ccs4msk[cons[carnmb]][i] = 0;	// secure the call
						else if (ccnmsk[cons[carnmb]][i] == 1)
							ccs4msk[cons[carnmb]][i] = 1;	// allow the call
					}
					ccmsk[cons[carnmb]] = &ccs4msk[cons[carnmb]][0];
					clrcc(1);	// clear car calls with invalid mask
				}
			}
			else if (secf_msks_applied == 1)
			{
				for (i=0;i<=1;i++)
					prev_g_cc_sec[i] = 0;
				secf_msks_applied = 0;
				ccmsk[cons[carnmb]] = &ccnmsk[cons[carnmb]][0];
				ucmsk[cons[carnmb]] = &ucnmsk[cons[carnmb]][0];
				dcmsk[cons[carnmb]] = &dcnmsk[cons[carnmb]][0];
				rccmsk[cons[carnmb]] = &rccnmsk[cons[carnmb]][0];
				rucmsk[cons[carnmb]] = &rucnmsk[cons[carnmb]][0];
				rdcmsk[cons[carnmb]] = &rdcnmsk[cons[carnmb]][0];
			 	clrcc(1);		// cancel the call with the invalid mask
			}
		}		
				
	

		if (cons[Second_Riser] == 1)
		{
	 		if (((car_Grp_Stat.c_sr_cc_ctrl & 0x01) != 0) && (firef == 0) && (medf == 0) && (empf == 0))
			{
				if (sr_cc_mask == 0)
				{
					sr_cc_mask = 1;
				    for(i=1; i<car_size; i++)
				    {
						if (Valid_SR_Car[i] != 0)
						{
							ccmsk[i] = &irccmsk[i][0];
							rccmsk[i] = &rirccmsk[i][0];
						}
					}
				 	clrcc(1);	// cancel the call with the invalid mask
				}
			}
			else
			{
				if (sr_cc_mask == 1)
				{
					sr_cc_mask = 0;
					if (((cons[SecFlCfg] == 1) || (cons[SecFlCfg] == 5)) && (Sec_Fl_Mask == 1) && (firef == 0) && (medf == 0) && (empf == 0))
					{		   // car still on security so use security mask
					    for(i=1; i<car_size; i++)
					    {
							if (Valid_SR_Car[i] != 0)
							{
								Sec_Fl_Mask = 0;
								ccmsk[i] = &ccsmsk[i][0];
								rccmsk[i] = &rccsmsk[i][0];
							}
	 					}
					}
					else
					{	  // otherwise on normal service so use normal mask
					    for(i=1; i<car_size; i++)
					    {
							if (Valid_SR_Car[i] != 0)
							{
								Sec_Fl_Mask = 0;
								ccmsk[i] = &ccnmsk[i][0];
								rccmsk[i] = &rccnmsk[i][0];
							}
						}
					}
				 	clrcc(1);	// cancel the call with the invalid mask
				}
			}
		}
		
		else if (cons[Second_Riser] == 2)
		{
			if (((sr_cc_ctrl[cons[carnmb]] & 0x01) != 0) && (firef == 0) && (medf == 0) && (empf == 0))
			{
				if (sr_cc_mask == 0)
				{
					sr_cc_mask = 1;
					if (Valid_SR_Car[cons[carnmb]] != 0)
					{
						ccmsk[cons[carnmb]] = &irccmsk[cons[carnmb]][0];
						rccmsk[cons[carnmb]] = &rirccmsk[cons[carnmb]][0];
					}
					clrcc(1);	// cancel the call with the invalid mask
				}
			}
			else
			{
				if (sr_cc_mask == 1)
				{
					sr_cc_mask = 0;
					if (((cons[SecFlCfg] == 1) || (cons[SecFlCfg] == 5)) && (Sec_Fl_Mask == 1) && (firef == 0) && (medf == 0) && (empf == 0))
					{		   // car still on security so use security mask
						if ((Valid_SR_Car[cons[carnmb]] != 0) || (Valid_SR_Car_Backup[cons[carnmb]] != 0))
						{
							ccmsk[cons[carnmb]] = &ccsmsk[cons[carnmb]][0];
							rccmsk[cons[carnmb]] = &rccsmsk[cons[carnmb]][0];
						}
					}
					else
					{	  // otherwise on normal service so use normal mask
						if ((Valid_SR_Car[cons[carnmb]] != 0) || (Valid_SR_Car_Backup[cons[carnmb]] != 0))
						{
							ccmsk[cons[carnmb]] = &ccnmsk[cons[carnmb]][0];
							rccmsk[cons[carnmb]] = &rccnmsk[cons[carnmb]][0];
						}
					}
				 	clrcc(1);	// cancel the call with the invalid mask
				}
			}
		}

		if ((cons[Third_Riser] != 0) && (cons[rear] == 0)) 
		{
	 		if (((tr_cc_ctrl[cons[carnmb]] & 0x01) != 0) && (firef == 0) && (medf == 0) && (empf == 0))
			{
				if (tr_cc_mask == 0)
				{
					tr_cc_mask = 1;
				    for(i=1;i<=cons[nmbcars];i++)
				    {
						if (Valid_TR_Car[i] != 0)
						{
							ccmsk[i] = &trccmsk[i][0];
							rccmsk[i] = &rtrccmsk[i][0];
						}
					}
				 	clrcc(1);	// cancel the call with the invalid mask
				}
			}
			else
			{
				if (tr_cc_mask == 1)
				{
					tr_cc_mask = 0;
					if (((cons[SecFlCfg] == 1) || (cons[SecFlCfg] == 5)) && (Sec_Fl_Mask == 1) && (firef == 0) && (medf == 0) && (empf == 0))
					{		   // car still on security so use security mask
					    for(i=1; i<car_size; i++)
					    {
							if (Valid_TR_Car[i] != 0)
							{
								Sec_Fl_Mask = 0;
								ccmsk[i] = &ccsmsk[i][0];
								rccmsk[i] = &rccsmsk[i][0];
							}
	 					}
					}
					else
					{	  // otherwise on normal service so use normal mask
					    for(i=1;i<=cons[nmbcars];i++)
					    {
							if (Valid_TR_Car[i] != 0) 
							{
								Sec_Fl_Mask = 0;
								ccmsk[i] = &ccnmsk[i][0];
								rccmsk[i] = &rccnmsk[i][0];
							}
						}
					}
				 	clrcc(1);	// cancel the call with the invalid mask
				}
			}
		}
		

		if (Grp_CC_Override == 1)
		{
			if ((fvars[fvsecfl] >= cons[bottomf]) && (fvars[fvsecfl] <= cons[topf]))
				fl = fvars[fvsecfl];
			else
				fl = fvars[fvlob];					
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				fl_ix = (i-1)/32;
				if (((gc_cc_sec_ovr_panic[fl_ix] & fl_mask[i]) != 0) && (carcb[i] == 1) && ((position != i) || (position != fl) || (procf <14) || (procf > 16)))
				{
					if (sec_panic_recall == 0)
					{
						sec_recall_complete = 0;
						sec_panic_recall = 1;	
					}
				}
				if (((gc_rcc_sec_ovr_panic[fl_ix] & fl_mask[i]) != 0) && (rcarcb[i] == 0) && ((position != i) || (position != fl) || (procf <14) || (procf > 16)))
				{
					if (sec_panic_recall == 0)
					{
						sec_recall_complete = 1;
						sec_panic_recall = 1;
					}
				}
			}
		}

			// Independent Security Recall
		if (cons[IndSec] == 1)
		{
			if (rdinp(i_INDS) == 0)
				ind_sec_recall = 0;
			else if (ind_sec_recall == 0)
			{
				ind_sec_recall = 1;
				sec_recall_complete = 0;
			}
		}
		else 
			ind_sec_recall = 0;

		if ((((Security_Type[cons[carnmb]] != 0) || (cons[Remote_CC] != 0)) && ((fvars[fvsecrec] & 0x01) != 0) || (Grp_CC_Override == 1)) ||
			((cons[IndSec] == 1) && (rdinp(i_INDS) == 1)))
		{
			if ((firef == 0) && (rfiref == 0) && (medf == 0) &&	(empf == 0) &&
				((gc_sec == 1) || (rdinp(i_SECUR) == 1) || ((rdinp(i_SECFM) == 1) && ((cons[SecFlCfg] == 1) || (cons[SecFlCfg] == 5))) ||
				  ((cons[IndSec] == 1) && (rdinp(i_INDS) == 1)) ||
				  (remote_cc == 1) || (remote_rcc == 1) || (sec_panic_recall == 1)))
			{
				if (sec_recall_complete == 0)
				{
					if (sec_recall == 0)
					{
						if ((cons[carnmb] == Dispatcher) && (((fvars[fvsecrec2] & 0x03) == 0) || ((fvars[fvsecrec2] & 0x01) != 0)))
							grp_clrhc();
					 	clrcc(0);
						if ((fvars[fvsecfl] >= cons[bottomf]) && (fvars[fvsecfl] <= cons[topf]))
							fl = fvars[fvsecfl];
						else
							fl = fvars[fvlob];					
						if ((fvars[fvsecrec] & 0x02) != 0)
							set_carcall(cons[carnmb],fl);	  // OPEN DOOR AT FLOOR
						if ((fvars[fvsecrec] & 0x04) != 0)
							set_rcarcall(cons[carnmb],fl);	  // OPEN DOOR AT FLOOR
					}
					sec_recall = 1;
				}
				if ((servf == s_AUTO) || (servf == s_SEC_RCL) || ((cons[IndSec] == 1) && (rdinp(i_INDS) == 1) && (servf == s_IND))) 
				{
					if ((fvars[fvsecfl] >= cons[bottomf]) && (fvars[fvsecfl] <= cons[topf]))
						fl = fvars[fvsecfl];
					else
						fl = fvars[fvlob];					
					if (sec_recall == 1)
					{
						if (position != fl) 
						{
							zone_fl = fl;
							if(rdinp(i_IDSO) == 0)
							{
								sec_recall_complete = 0;
							}
							if (((sec_recall_complete == 0) && ((fvars[fvsecrec2] & 0x03) == 0)) || ((fvars[fvsecrec2] & 0x01) != 0))
								servf = s_SEC_RCL;
						}
						else if ((procf >= 14) && (procf <= 16))
						{	
							servf = s_AUTO;
							sec_recall = 0;
							sec_recall_complete = 1;
							sec_panic_recall = 0;
							zone_fl = 0;
						}
					}
					else if ((fvars[fvsecrec] & 0x08) != 0)
					{
						if (position > fl)
						{
							recall_dir = 2;
						}
						else if (position < fl)
						{
							recall_dir = 1;
						}
						if ((position != fl) && (procf >= 14) && (procf <= 16) && ((dpref == 0) || dirf == recall_dir))
						{
							sec_recall = 1;
							if(rdinp(i_IDSO) == 0)
							{
								sec_recall_complete = 0;
							}
							if ((fvars[fvsecrec] & 0x02) != 0)
								set_carcall(cons[carnmb],fl);	  // OPEN DOOR AT FLOOR
							if ((fvars[fvsecrec] & 0x04) != 0)
								set_rcarcall(cons[carnmb],fl);	  // OPEN DOOR AT FLOOR
						}
					}
				}
			}
			else
			{
				if (((sec_recall_complete == 1) || (sec_recall == 1)) && (servf == s_SEC_RCL))
					servf = s_AUTO;
				sec_recall = 0;
				sec_recall_complete = 0;
			}
		}
		else
		{ 
			sec_recall = 0;
			sec_recall_complete = 0;
		}

		if (cons[Remote_CC] != 0)
		{
			if ((rdinp(i_SCS) == 1) && (firef == 0) && 									
				(((fvars[fvrmccd] & 0x04) == 0) || ((dirf != 1 ) && (dpref != 1 ))) &&  // not disable when not up or going up
				(((fvars[fvrmccd] & 0x08) == 0) || ((dirf != 2 ) && (dpref != 2 ))) &&  // not disable when not dn or going dn
				(((fvars[fvindsec] & 0x04) == 0)|| (servf != s_IND)))						// not override when indsec!=4 and servf 2
				remote_cc = 1;
			else if ((tugf == 1)&&(rdinp(i_THDS) == 1) && 
				(((fvars[fvrmccd] & 0x04) == 0) || ((dirf != 1 ) && (dpref != 1 ))) &&  // not disable when not up or going up
				(((fvars[fvrmccd] & 0x08) == 0) || ((dirf != 2 ) && (dpref != 2 ))) &&  // not disable when not dn or going dn
				(((fvars[fvindsec] & 0x04) == 0)|| (servf != s_IND)))						// not override when indsec!=4 and servf 2
				remote_cc = 1;
			else 
				remote_cc = 0;
			
			if ((cons[rear] == 1) && (rdinp(i_SCSR) == 1) && (rfiref == 0) && 
				(((fvars[fvrmccd] & 0x04) == 0) || ((dirf != 1 ) && (dpref != 1 ))) &&  // not disable when not up or going up
				(((fvars[fvrmccd] & 0x08) == 0) || ((dirf != 2 ) && (dpref != 2 ))) &&  // not disable when not dn or going dn
				(((fvars[fvindsec] & 0x04) == 0)|| (servf != s_IND)))						// not override when indsec!=4 and servf 2
				remote_rcc = 1;
			else if ((tugf == 1)&&(rdinp(i_THDS) == 1) && (firef == 0) && 
				(((fvars[fvrmccd] & 0x04) == 0) || ((dirf != 1 ) && (dpref != 1 ))) &&  // not disable when not up or going up
				(((fvars[fvrmccd] & 0x08) == 0) || ((dirf != 2 ) && (dpref != 2 ))) &&  // not disable when not dn or going dn
				(((fvars[fvindsec] & 0x04) == 0)|| (servf != s_IND)))						// not override when indsec!=4 and servf 2
				remote_rcc = 1;
			else
				remote_rcc = 0;
		}
		else
		{
			remote_cc = 0;
			remote_rcc = 0;
		}
		
		if ((((Security_Type[cons[carnmb]] & 0x40) == 0) && (Security_Type[cons[carnmb]] & 0x01) != 0) || (cons[openlout] != 0))	
		{
		    if((rdinp(i_DOL) == 0) || ((cons[rear] !=0) && (rdinp(i_DOLR) == 0)))
			{
				if (((fvars[fvopenlctl] & 0x10) != 0))
				{
					setoutp(o_OPENL);
				}
				else
				{
					if(((fvars[fvopenlctl] & 0x01) != 0) && (firef == 3))
					{
						setoutp(o_OPENL);
					}
					if(((fvars[fvopenlctl] & 0x02) != 0) &&
							(((rdinp(i_RTLH) == 1) && (cons[otslbp] != 0)) || 
					  		(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_rtl == 1)) || 
					  		(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_rtl == 1)) || 
					  		((rdinp(i_RTLDC) == 1) && ((cons[otslbp] & 0x04) != 0))))
					{
						setoutp(o_OPENL);
					}
					if((fvars[fvopenlctl] & 0x04) != 0)
					{
						if(((empf == 4) || (empf == 2) || (empf == 6)) && (position == fvars[fvepfl]))
							setoutp(o_OPENL);
						else
							clroutp(o_OPENL);
						
					}
					if(((fvars[fvopenlctl] & 0x08) != 0) && (position == lobby_fl))
					{
						setoutp(o_OPENL);
					}

				}
			}
			else
			{
				clroutp(o_OPENL);				
			}
		 
		}

		if((Security_Type[cons[carnmb]] & 0x40) == 0)
		{
			if (((Security_Type[cons[carnmb]] & 0x01) != 0) || ((Security_Type[cons[carnmb]] & 0x10) != 0))
			{
				if (fvars[fvinsec] == 0)
				{
					if ((rdinp(i_SECUR) == 1) || (rdinp(i_RSECU) == 1))
						setoutp(o_INSEC);
					else
						clroutp(o_INSEC);		
				}
				else
				{
					if ((rdinp(i_SECUR) == 1) || (rdinp(i_RSECU) == 1))
						clroutp(o_INSEC);		
					else
						setoutp(o_INSEC);
				}

			}
			else
			{
				clroutp(o_INSEC);
			}
		}
		
		if (((Security_Type[cons[carnmb]] & 0x01) != 0) || ((Security_Type[cons[carnmb]] & 0x10) != 0))
		{
			if (firef != 0)
				setoutp(o_FSOCS);
			else
				clroutp(o_FSOCS);
			if (cons[rear] != 0)
			{
				if (firef != 0)
					setoutp(o_FSORS);
				else
					clroutp(o_FSORS);
			}
		}

	}
}


//****************************************
// Encoder Control called from Inctime
//****************************************

void hoistway_control (int16 start_ck)
{
		
	
	write_trace(TR_HOISTWAY_CONTROL);
	Sel_Can_Control();

	if (start_ck == 1)
	{
		enc_start_read_cnt++;
		if (enc_start_read_cnt >= 5)
		{						// slowdown encoder read during start

			enc_start_read_cnt = 0;
			read_hoistway_count();
		}
	}
	else
	{

		enc_start_read_cnt = 5;		// set count to read encoder the first time through
		read_hoistway_count();
	}
	set_HW_IO_from_APS_Sel();
}

// ***********************************************
//  Set Fault bits to save space for debug
// ***********************************************

void Set_Fault_Bits (void)
{

// Make sure logic sets bit to 1 for fault

	Fault_Bits[0] = (uint8)( ((Hoistway_Learned == 0) & 0x01) ||
									((hardware_init_fault & 0x01) << 1) ||
									((limit_dir_fault & 0x01) << 2) ||
									((spb_dir_fault & 0x01) << 3) ||
									((vel_decel_diff_fault & 0x01) << 4) ||
									((spb_vel_diff_fault & 0x01) << 5) ||
									((uldl_dir_fault & 0x01) << 6) ||
									((leveling_fault & 0x01) << 7) );

	Fault_Bits[1] = (uint8)( (cop_can_error & 0x01) ||
									((nts_spi_error & 0x01) << 1) ||
									((aps_sel_can_error & 0x01) << 5));

	Fault_Bits[2] = (uint8)( (DBR_Temp_Fault & 0x01) ||
									((door_motor_ovl & 0x01) << 2) );
	Fault_Bits[3] = (uint8)( (gripper_fault & 0x01) ||
									((enc_dir_fault & 0x01) << 1) );						;

	
}

// ***********************************************
// This is the deactivation procedure for sabbath
//************************************************

void deactivate_sabbath(void)
{
	if (cons[Sabbath] == 7)
	{	// Special feature use elevator off recall floor parameter to put the car back to normal service. 
		if ((onward_cc == 0) && (dpref == 0) && (procf >= 14) && (procf <= 16) && (doorf == 3))
			sabbath_active = 0;
	}
	else
	{
		if ((fvars[fvsabben] & 0x04) == 0)
			sabbath_active = 0;
		else if ((position == lobby_fl) && (procf >= 14) && (procf <= 16)&&(doorf == 3))	// at floor at lobby
		{
			sabbath_active = 0;
		}
	}
}


// ******************************************
// This is the NEW increment timers procedure
//*******************************************
  
void new_inctime()
{
	int i;
	while(timers[tinc] >= 10)		//"timers[tinc]" is 10ms=1/100s once
  	{
  	  	timers[tten]++;				//"timers[tten]" is 100ms=1/10s once
  	  	timers[tbuf]++;
  	  	timers[tinc] = (timers[tinc] - 10);		
  	}
  	
  	if(timers[tbuf] >= 10)
  	{
  	  	timers[tbuf] = (timers[tbuf] - 10);
  	  	timers[tsec]++;
	  	  
    	if(timers[tsec] == 1)       // turn on diag light 
    	{
//			SIU.GPDO[Z6_LED].R = 0x01; // tagwbg 14
    	}
    	if(timers[tsec] >= 4)       // turn off diag light 
    	{
//			SIU.GPDO[Z6_LED].R = 0x00; // tagwbg 14
    	  	timers[tsec] = 0;
    	}
  	}
  	
  	if(timers[tten] >= 1)			//100ms=1/10s once
  	{
  	  	for(i=8; i<tnmb; i++)       // incrementing all 1/10th of second timers
  	  	{
  	  	  	if((timers[i] + timers[tten]) < 32000)
				timers[i] = (timers[i] + timers[tten]);
  	  	}
  	  	timers[tten] = 0;
  	}  	  	
}

// ******************************************
// This is the increment timers procedure
//*******************************************
void inctime(int16 start_ck)  // 0 increments timers; 1 disables timers tagwbg
{
  	int16 i;

//	if ((fvars[fvcputiming] & INCTIM) != 0)
//		SIU.GPDO[Z6_LED].R = 0x01; // tagwbg 14
	
	if (start_ck == 0)
	{
		if (STM._CR.B.TEN == 1)
		{
			STM._CR.B.TEN = 0;				// Disable timer
			inctim_time = STM.CNT.R;		// Read timer

			if (inctim_time < proctim_min)
				proctim_min = inctim_time;
			
			if (inctim_time > proctim_max)
				proctim_max = inctim_time;
			
			proctim_now = inctim_time;
			proctim_avg += inctim_time/500;
			proctim_avg = (proctim_avg * 499)/500;
		}
		STM._CR.B.TEN = 0;		// Disable timer
		STM.CNT.R = 0;			// Zero timer
		STM._CR.B.TEN = 1;		// Enable timer
	}
	else
		STM._CR.B.TEN = 0;		// Disable timer
	
	write_trace(TR_INCTIME1);

	// *******************************
	// Increment Timer Section
	// *******************************

  	while(timers[tinc] >= 10)
  	{
  	  	timers[tten]++; // tten from 1/100 s interrupt tagwbg
  	  	timers[tbuf]++;
  	  	timers[tinc] = (timers[tinc] - 10);
		timers[tcom_que] = (timers[tcom_que] + 1) & 0x7FFF;
#if (Simulator == 1)
		sim_door_timer++;
#endif
  	}
  	if(timers[tbuf] >= 10)
  	{
  	  	timers[tbuf] = (timers[tbuf] - 10);
  	  	timers[tsec]++;
		rt_second ++;
		if (rt_second > 59)
		{
			rt_second = 0;
			rt_minute++;
			if (rt_minute > 59)
			{
				rt_minute = 0;
				rt_hour++;
				if (rt_hour > 23)
				{
					rt_hour = 0;
					if (rt_day > 364)
						rt_day = 0;
				}
			}
		}
				
  	  	petdog();                   // petting the watchdog Pg#2-9
  	  	for(i=cons[bottomf]; i<=cons[topf]; i++)
    	{
      		cctim[i]++;
      		if(cctim[i] > 32000)
				cctim[i] = 32000;
      		if(cons[rear] != 0)
      		{
				ccrtim[i]++;
				if(ccrtim[i] > 32000)
				ccrtim[i] = 32000;
      		}
    	}
    	for(i=cons[bottomf]; i<cons[topf]; i++)
    	{
    	  	uctim[i]++;
    	  	if(uctim[i] > 32000)
				uctim[i] = 32000;
    	  	if(grtop_rfl != 0)
    	  	{
				ucrtim[i]++;
				if(ucrtim[i] > 32000)
				ucrtim[i] = 32000;
    	  	}
    	}
    	for(i=(cons[bottomf]+1); i<=cons[topf]; i++)
    	{
    	  	dctim[i]++;
    	  	if(dctim[i] > 32000)
				dctim[i] = 32000;
    	  	if(grtop_rfl != 0)
    	  	{
				dcrtim[i]++;
				if(dcrtim[i] > 32000)
					dcrtim[i] = 32000;
    	  	}
    	}

    	if(timers[tsec] == 1)       // turn on diag light 
    	{
		//	diag_led_on(); // tagwbg 14
    	}
    	if(timers[tsec] >= 2)       // turn off diag light 
    	{
		//	diag_led_off(); // tagwbg 14
    	  	timers[tsec] = 0;
    	}
  	}
  	
  	if(timers[tten] >= 1)
  	{
  		switch (tmr_cascade_state)
  		{
  			case 0:
  				tmr_start = 9;
  				tmr_end = tnmb/4;
  				tmr_cascade_state = 1;
  				run_timer = 1;
  				break;
  			case 1:
  				tmr_start = tnmb/4;
  				tmr_end = tnmb/2;
  				tmr_cascade_state = 2;
  				run_timer = 1;
  				break;
  			case 2:
  				tmr_start = tnmb/2;
				tmr_end = (tnmb * 3)/4;
				tmr_cascade_state = 3;
				run_timer = 1;
				break;
  			case 3:
  				tmr_start = (tnmb * 3)/4;
  				tmr_end = tnmb;
  				tmr_cascade_state = 0;
  				run_timer = 1;
  				break;
  			default:
  				run_timer = 0;
  				tmr_cascade_state = 0;
  				break;
  		}
  			
  		if (run_timer == 1)
  		{
			run_timer = 0;
	  	  	for(i=tmr_start; i<tmr_end; i++)       // incrementing all 1/10th of second timers
	  	  	{
	  	  	  	if((timers[i] + timers[tten]) < 32000)
					timers[i] = (timers[i] + timers[tten]);
	  	  	}
	  		if (tmr_cascade_state == 0)
	  	  		timers[tten] = 0;
  		}
  	}
  	
  	if (timer_sec[ts_sec] > 0)
  	{
  		timer_sec[ts_sec] = 0;
		for (i=1;i<ts_nmb;i++)
		{
			if (timer_sec[i] < 32000)
				timer_sec[i]++;
		}
  	}
  	
	inctime_count++;
	
	if(timers[tlocalgetdate] >= 5)			// updates every 500ms
	{			// Must run in every service
		local_gettime();
		local_getdate();

		// Check for date change occurance
		if ((d.day != prev_d.day) || (d.month != prev_d.month) || (d.year != prev_d.year))
		{
			if (cons[carnmb] == Dispatcher)		// Sync time with other cars 
				put_pkt_req_all_cars(109);
			
			prev_d.day = d.day;
			prev_d.month = d.month;
			prev_d.year = d.year;
		}
		
		timers[tlocalgetdate] = 0;	
	}

	//***********************
	// End of timer section
	//***********************
	
//	iospi_control();  // tagwbg 01
	
//	hoistway_control(start_ck);  // tagwbg 01

//	chk_motion_fault(start_ck);  // tagwbg 01

//	status_control(start_ck);  // tagwbg 01


//	door_control(start_ck);  // tagwbg 01

//	write_trace(TR_INCTIME2);  // tagwbg 01

//	diag();  // tagwbg 01

	
	if (start_ck == 0)
	{
		
//		Car_ETA();  // tagwbg 01

	//	if(cons[home] != 0)
	//	  group_park();

	}

	comm_control();

	if (start_ck == 0)
	{
  		set_grp_io();

 	 	if (cons[carnmb] != Dispatcher)
  			Car_Not_Dispatcher();
 	 	
//		group_control();
		
		hydro_control();
		

	}

//	security_control(start_ck);  // tagwbg 01

//	light_buzzer_control(start_ck);  // tagwbg 01

//	output_control(start_ck);  // tagwbg 01

//	write_trace(TR_INCTIME3);  // tagwbg 01

	
#if (Simulator == 1)
  	sim_control();
	write_trace(TR_INCTIME4);
#endif

//	Write_System_Trace();  // tagwbg 01
	
	if (start_ck == 0)
	{
		if (STM._CR.B.TEN == 1)
		{
			
			STM._CR.B.TEN = 0;				// Disable timer
			inctim_time = STM.CNT.R;		// Read timer

			if (inctim_time < inctim_min)
				inctim_min = inctim_time;
			
			if (inctim_time > inctim_max)
				inctim_max = inctim_time;
			
			inctim_now = inctim_time;
			inctim_avg += inctim_time/500;
			inctim_avg = (inctim_avg * 499)/500;
		}

		STM._CR.B.TEN = 0;		// Disable timer
		STM.CNT.R = 0;			// Zero timer
		STM._CR.B.TEN = 1;		// Enable timer

	}
	else
		STM._CR.B.TEN = 0;		// Disable timer
		
//	if ((fvars[fvcputiming] & INCTIM) != 0)
//		SIU.GPDO[Z6_LED].R = 0x00; // tagwbg 14

  return;
}

//**********************************************************
//** Independent service from MR or COP ********************
//**********************************************************

int16 ind (void)
{
	if((cb_over_ind == 1)&&(timers[tcbovrind] > fvars[fvfs1dto]))
		return 0;
	if((heof_over_ind == 1)&&(timers[theofovrind] > 100))
		return 0;
	if((cons[hugs] == 3) && (rdinp(i_HUGB) == 0) && (rdinp(i_HUGI) == 1))
		return 0;
	if (rdinp(i_IND) == 1)
		return 1;
	if ((rdinp(i_INDC) == 1) && ((cons[sercom] & 2) != 0))
		return 1;
	if ((rdinp(i_XINDC) == 1) && ((cons[aux_COP] & 0x03) != 0))
		return 1;
	if ((remote_cc == 1) && (cons[Remote_CC] == 1) && (rdinp(i_RM_INDC) == 1))
		return 1;
	if (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ind == 1) && (firef == 0) && (rfiref == 0))
		return 1;
	if (((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ind == 1) && (firef == 0) && (rfiref == 0))
		return 1;
	if ((cons[IndSec] == 1) && (rdinp(i_INDS) == 1) && (ind_sec_recall == 1) && (sec_recall_complete == 1))
		return 1;
	return 0;
}


// *********************************************************
// This is the 1/100th of a second interupt timer procedure
// *********************************************************
void inttim(void)
{
	int16 i;

    Timer_tic++;  // main() waits until this gets set. tagwbg 02
    
	if ((fvars[fvcputiming] & TICK_10_MSEC) != 0)
//		SIU.GPDO[Z6_LED].R ^= 0x01; // tagwbg 01 xx
	
 	if (int_flag == 1)
  	{
		int_flag = 0;
  	}
  	else
  	{
		int_flag = 1;
  	}
  	timers[tinc]++;
  	timers[tvpat]++;
  	timers[tcom_response]++;
		
/*		// tagwbg 01
	if (timers[tpatdly] < 32000)
		timers[tpatdly]++;
	
  	timers[tiofault]++;

  	if (gripper_flt_timer < 32000)
  		gripper_flt_timer++;

  	if (gov_flt_timer < 32000)
  		gov_flt_timer++;

  	if (gripper_spd_trip_timer < 32000)
  		gripper_spd_trip_timer++;
  	
  	if (gripper_spb_umotion_timer < 32000)
  		gripper_spb_umotion_timer++;
  	
  	if (ovs_dbn_tim < 32000)
  		ovs_dbn_tim++;
  	
  	if (tmr_uts_dbn < 32000)
		tmr_uts_dbn++;		// Timer for uts limit fault

  	if (tmr_dts_dbn < 32000)
		tmr_dts_dbn++;		// Timer for dts limit fault
  	
  	if (tmr_lim_dir < 32000)
		tmr_lim_dir++;		// Timer for limit direction fault

	if (tmr_vel_diff < 32000)
		tmr_vel_diff++;	// Timer for velocity difference fault

	if (tmr_spb_enc_dir < 32000)
		tmr_spb_enc_dir++; 	// Timer for SPB encoder direction fault

	if (tmr_enc_dir < 32000)
		tmr_enc_dir++; 	// Timer for controller encoder direction fault

	if (tmr_uldl_dir < 32000)
		tmr_uldl_dir++;		// Timer for limit direction fault

	if (tmr_spb_vel_diff < 32000)
		tmr_spb_vel_diff++; 	// Timer for SPB velocity difference fault

	if (enc_save_timer < 32000)
		enc_save_timer++;
	
	if (DL20_tx_timer < 32000)
		DL20_tx_timer++;
	
	dpp_store_tmr ++;
	
	tmr_10_msec ++; // tagwbg 09 This code could probably be removed

	drive_update_timer++;

  	can_sel_timer++;
	can_sync_timer++;

	if (enc_dir_timer < 32000)
		enc_dir_timer++;

	if (dz_dbn_timer < 32000)
		dz_dbn_timer ++;

#if (Simulator == 0)	// when using the simulator this timer is incremented by the sim_motion
  	enc_vel_timer++;
#else
  	sim_vel_timer++;
  #if (Sim_Motion == 0)
  	enc_vel_timer++;
  #endif
#endif

*/
	//they got called by 10ms every time
	dpyspi_timer++; // tagwbg 06 
	iospi_timer++;  // tagwbg 06
//	wbg_02++;       // tagwbg 02
	trace_tmr++;
	
	
	if (loop_cnt > 10)
		loop_cnt = 10;

	if (loop_cnt_ptr >= 9)
		loop_cnt_ptr = 0;
	else
		loop_cnt_ptr++;

	loop_cnt_array[loop_cnt_ptr] = loop_cnt;
	loop_cnt = 0;

	for(i=0;i<=9;i++)
		avg_loop_cnt += loop_cnt_array[i];
	avg_loop_cnt = avg_loop_cnt/10;

    PIT.TFLG1.R = 0x00000001;    
}

// *****************************************************
// Interrupt for seconds timer
// *****************************************************

void int_tsec (void)
{

    /* toggle LED */
	if ((fvars[fvcputiming] & MAIN_1SEC) != 0)
	//	SIU.GPDO[Z6_LED].R ^= 0x01; // tagwbg 01

	timer_sec[ts_sec]++;
	timer_grp_sec[tgs_sec]++;
	
    PIT.TFLG3.R = 0x00000001;    
	
}

// *****************************************************
//	Zero important timers
// *****************************************************
void inittim()
{
  	// Zero critical i/o timers

  	timers[tdl] = 0;
  	timers[tul] = 0;
  	timers[tdlm] = 0;
  	timers[tdlb] = 0;
  	timers[tdlt] = 0;
  	timers[tgs] = 0;
	timers[tlct] = 0;
	timers[tsst] = 0;
	timers[tinst] = 0;
	timers[thct] = 0;
	timers[tdmo] = 0;
	timers[tpst] = 0;
	timers[teprcldly] = 0;		// emergency power recall delay.
	timers[tebrkio] = 0;
	timers[tebkdrop] = 0;
	timers[tcol] = 0;
  	timers[tinc] = 0;
  	timers[tten] = 0;
  	timers[tbuf] = 0;

	timers[tEP_npd] = 0;
	timers[tEP_epd] = 0;
	timers[tEP_epo] = 0;
  return;
}

// ***************************************************
// Routine to set appropriate flags on an initial stop
// ***************************************************

void initial_stop (void)
{
  	trigger_arm_state |= INIT_STOP;
	Overspeed_Test = 0;
	Buffer_Test = 0;
	NTS_Test = 0;
	ETS_Test = 0;
    NTS_spi.Command2.B.SET_UPSD = 0;		// clear the command on exit just to make sure
    NTS_spi.Command2.B.SET_DNSD = 0;		// clear the command on exit just to make sure

	if( (rset == 0) && (no_fault == 1) )
	{
		if(start_floor < position)
			flight_times[start_floor][position] = (timers[tmotion]/10);
		else
			flight_times[position][start_floor] = (timers[tmotion]/10);
		Update_Flight_Times();
	}
	Update_Control_Flags();
	timers[tcablnt] = 0;
 	timers[trcablnt] = 0;
	timers[tdo] = 0;
	timers[trdo] = 0;
	timers[tgen] = 0;
	timers[tebkdrop] = 0;
	timers[tsoftstp] = 0;
	timers[tbkdrop] = 0;	 // used for brake drop test	
	timers[tnit] = 0;
    timers[tdwel] = 0;
	timers[tattbzini] = 0;
    timers[trdwel] = 0;
	timers[tnudge] = 0;
	timers[trnudge] = 0;
	timers[teeto] = 0;
	timers[treeto] = 0;
	timers[tfdo_adv_de] = 0;
	timers[trdo_adv_de] = 0;
	timers[tindrcl] = 0;
	timers[tpbdoort] = 0;
	nudgst = 0;
	rnudgst = 0;
	closetry = 0;
	rclosetry = 0;
	safe_closetry = 0;
	safe_rclosetry = 0;

	Door_Lock_GS_DPM_Fault = 0;
	statusf &= ~sf_GDL;		// cleared in initial_stop()

	RCM_Lock_Fault = 0;
	lev_startf = 0;
	startf = 0;
	eef_testtry = 0;
	eer_testtry = 0;
	fado_fault_cnt = 0;
	rado_fault_cnt = 0;

	timers[tgripper] = 0;
	if( (firef > 3) || (rfiref > 3) || (medf == 3) || 
		((fvars[fvhsvdccc] == 1) && (hsvf == 1)) || ((fvars[fvinddccc] == 1) && (servf == s_IND)))
		clrcc(0);
}


//***********************************************
// Initialize Ram Data for car and group
//***********************************************

void init_ram_data(void)
{

int16 i;

	// zero group data from cars

	for (i=1;i<=cons[nmbcars];i++)
	{
		grp_Car_Stat[i].g_procf = 0;
		grp_Car_Stat[i].g_servf = 0;
		grp_Car_Stat[i].g_empf = 0;
		grp_Car_Stat[i].g_firef = 0;
		grp_Car_Stat[i].g_doorf = 0;
		grp_Car_Stat[i].g_rdoorf = 0;
		grp_Car_Stat[i].g_dirf = 0;
		grp_Car_Stat[i].g_dpref = 0;
		grp_Car_Stat[i].g_codebf = 0;
		grp_Car_Stat[i].g_dcalls = 0;
		grp_Car_Stat[i].g_cancel_upr = 0;
		grp_Car_Stat[i].g_cancel_dnr = 0;
		grp_Car_Stat[i].g_cur_flt = 0;
		grp_Car_Stat[i].g_statusf1 = 0;
		grp_Car_Stat[i].g_control = 0;
		grp_Car_Stat[i].g_ss_stat = 0;
		grp_Car_Stat[i].g_cancel_irup = 0;
		grp_Car_Stat[i].g_cancel_irdn = 0;
		grp_Car_Stat[i].g_cancel_rirup = 0;
		grp_Car_Stat[i].g_cancel_rirdn = 0;
		grp_Car_Stat[i].g_vipf = 0;
		grp_Car_Stat[i].g_nmbcc = 0;
		grp_Car_Stat[i].g_topcc = 0;
		grp_Car_Stat[i].g_botcc = 0;
		grp_Car_Stat[i].g_fire_status = 0;
		grp_Car_Stat[i].g_ins_status = 0;
		grp_Car_Stat[i].g_dmd_vel = 0;			// group demand velocity in fpm for each car
		grp_Car_Stat[i].g_enc_vel = 0;			// group encoder velocity in fpm for each car
		grp_Car_Stat[i].g_enc_dir = 0;			// group encoder direction for each car
		grp_Car_Stat[i].g_flr_cnt = 0;			// floor reference count for each car
		grp_Car_Stat[i].g_pls_cnt = 0;			// car encoder pulse count for each car
		grp_Car_Stat[i].g_L1_Voltage = 0;
		grp_Car_Stat[i].g_L2_Voltage = 0;
		grp_Car_Stat[i].g_L3_Voltage = 0;
		grp_Car_Stat[i].g_Door_Voltage = 0;
		grp_Car_Stat[i].g_Voltage_Dev_Online = 0;
		grp_Car_Stat[i].g_cancel_trup = 0;
		grp_Car_Stat[i].g_cancel_trdn = 0;
		grp_Car_Stat[i].g_cancel_rtrup = 0;
		grp_Car_Stat[i].g_cancel_rtrdn = 0;
		g_cc_req[0][i] = 0;		  		// car call request for car
		g_cc_req[1][i] = 0;		  		// car call request for car
		g_rcc_req[0][i] = 0;
		g_rcc_req[1][i] = 0;
 		g_cc_dpy[0][i] = 0;		  		// car call request for car
		g_cc_dpy[1][i] = 0;		  		// car call request for car
		g_rcc_dpy[0][i] = 0;
		g_rcc_dpy[1][i] = 0;

		
	}

  	// Zero critical i/o timers

  	timers[tdl] = 0;
  	timers[tul] = 0;
  	timers[tdlm] = 0;
  	timers[tdlb] = 0;
  	timers[tdlt] = 0;
  	timers[tgs] = 0;
	timers[tlct] = 0;
	timers[tsst] = 0;
	timers[tinst] = 0;
	timers[thct] = 0;
	timers[tdmo] = 0;
	timers[tpst] = 0;
	timers[teprcldly] = 0;		// emergency power recall delay.
	timers[tebrkio] = 0;
	timers[tebkdrop] = 0;
	timers[tcol] = 0;
}

//******************************************
// Elevator is turned off by keyed a switch
//******************************************
void key_ots()
{
  set_ul_dl_dz();		// sets up_level, dn_level and door_zone
  return;
}

//*********************************
// Elevator stalled out of service
//*********************************
void stall()
{
	startf = 0;
	lev_startf = 0;
	while(procf == 17)
	{
		servf = s_STALLED;
		inctime(0);
		set_ul_dl_dz();		// sets up_level, dn_level and door_zone
		setfsf();
		clrall();
	    if(chk_ins() == 1)
		{
			if (cons[ansi] < 2007)
			{
				if ((cons[Low_Oil] == 0) || 
					(((rdinp(i_LOS) == 0) && (fvars[fvlos] == 0)) || 
						((rdinp(i_LOS) == 1) && (fvars[fvlos] == 1))))
				{
					stallf = 0;
					procf = 2;
				}
			}
		}
		if (ccnmsk[cons[carnmb]][position] != 0)
		{
			if ((rd_both_inps(i_DOB,i_XDOB) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) || 
				((manual_door == 2) && (man_fdcc() == 0)))
			{
				procf = 15;
				stallf = 1;
			}
		}
		if ((cons[rear] != 0) && (rccnmsk[cons[carnmb]][position] != 0))
		{
			if ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) || 
				((manual_rdoor == 2) && (man_rdcc() == 0)))
			{
				procf = 15;
				stallf = 1;
			}
		}

  	}
  	return;
}
//*********************************
// Elevator has low pressure switch
//*********************************
void low_pressure(void)
{
	timers[tfault] = 0;
	record_fault(f_lowpress);
	clrall();
	doorf = 3;
	if (cons[rear] == 1)
		rdoorf = 3;
	dpref = 0;

 	startf = 0;
	lev_startf = 0;

 	while(procf == 19)
  	{
		servf = s_LOW_PRES;
		inctime(0);
		set_ul_dl_dz();		// sets up_level, dn_level and door_zone
		// Testing to see if saftey string is open
    	if(safeties() == 1)
			timers[tsst] = 0;
    	else if(timers[tsst] > 10)
    	{
			if ((statusf & sf_SS) == 0)
				record_fault(f_sstring);
			statusf |= sf_SS;
      		procf = 12;       // go to saftey string open mode
      		return;
    	}
		else
			statusf &= ~sf_SS;

		// Testing to see if LC fuse is blown
    	if(rdinp(i_S10) == 1)
			timers[tlct] = 0;
    	if((rdinp(i_S10) == 0) && (timers[tlct] > 10))
    	{
			if ((statusf & sf_S10) == 0)
				record_fault(f_S10_fuse);
			statusf |= sf_S10;
			procf = 12;
			return;
    	}
		else
			statusf &= ~sf_S10;
  	    if (chk_ins() == 1)
		{			   // exit on inspection
			lps_stall = 0;
			procf = 2;
			return;
		}
		if (door_zone == 1)
		{		// Allowed to open the door
				
			if ((rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
			{
				doort = fvars[fvccdt];
				doorf = 1;	// open door
		    	timers[tdo] = 0;
			}
			if (cons[rear] != 0)
			{
				if ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)))
				{
					rdoort = fvars[fvccdt];
					rdoorf = 1;	// open door
			    	timers[trdo] = 0;
				}
			}
			else
				rdoorf = 0;
		}
		else 
		{
			if (rdinp(i_DCL) == 0)
				doorf = 0;
			else
				doorf = 3;	// close door
			if (cons[rear] != 0)
			{
				if (rdinp(i_DCLR) == 0)
					rdoorf = 0;
				else
					rdoorf = 3;	// close door
			}
			else
				rdoorf = 0;
		}

	  	if(doorf == 0)
	  	{
	    	timers[tdo] = 0;
	    	timers[tdwel] = 0;
	    	timers[tdc] = 0;
	    	timers[tlant] = 0;
	    	timers[trlant] = 0;
	  	}
	  	if( (cons[rear] == 0) || (rdoorf == 0) )
	  	{
	    	timers[trdo] = 0;
	    	timers[trdwel] = 0;
	    	timers[trdc] = 0;
	  	}


		// Checking to see if dead floor level
		if((dn_level == 1) && (up_level == 1))   // dead floor level
		{
			if (((rdinp(i_LPS) == 0) && 
				(fvars[fvlps] == 0)) || ((rdinp(i_LPS) == 1) && (fvars[fvlps] == 1)))
			{
				if ((timers[tfault] >= fvars[fvfltt]) && (lps_stall == 0))
				{			 // Go to reset mode
					procf = 0;
					return;
				}
			}

			if(relevel == 1)
				timers[tsoftstp] = 0;
			relevel = 0;              // not releveling
			relevel_req = 0;

			timers[trelev] = 0;
			// Hydro only
			clroutp(o_SD);   // clear down output
			clroutp(o_SU);      // clear up output
			clroutp(o_RUN);   // clear run output

	        // Checking to see if a soft stop is needed
			if(timers[tsoftstp] <= fvars[fvsst])  // soft stop needed
			{
				timers[tmotion] = 0;      // not in motion clear motion timer
				if(dirf == 1)
				{
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
					    setoutp(o_MST);
				    setoutp(o_MCC);
				}
			}
			else      // No soft stop
			{
				dirf = 0;
				// viscoustiy control run the motor
				if( (servf > s_LOW_OIL) && (dpref == 0) && (dcalls == 0) &&
				(((rdinp(i_TPL) == 1) && (fvars[fvtpl] == 0)) || ((rdinp(i_TPL) == 0) && (fvars[fvtpl] == 1))) &&
				(rdoorf == 0) && (doorf == 0) && (position == lobby_fl) )
				{
					if(timers[tmotion] > 100)
						timers[tmotion] = 100;
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
					{
				  		if(timers[tmotion] > fvars[fvydel])
				       		setoutp(o_MST);
				  		else
				       		clroutp(o_MST);
					}
				  	setoutp(o_MCC);
					setoutp(o_MCA);
				  	Viscosity = timers[tmotion];
			    }
			    else
			    {
				  	if (((rdinp(i_TPL) == 0) && (fvars[fvtpl] == 0)) || ((rdinp(i_TPL) == 1) && (fvars[fvtpl] == 1)))
				  	{
						if(Viscosity == 0)
						{
					    	timers[tmotion] = 0;      // not in motion clear motion timer
						}
						else
						{
					    	if((timers[tmotion] - Viscosity) > 25)
								Viscosity = 0;
						}
				  	}
				  	else
				  	{
						if((timers[tmotion] - Viscosity) > 25)
						{
							timers[tmotion] = 0;
							Viscosity = 0;
						}
				  	}
				  	clroutp(o_MCC);
					clroutp(o_MCA);
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
					  	clroutp(o_MST);
				}
	      	}
		}
	// Elevator is not level needs to relevel
		else
		{
	    		// can't relevel too long
		  	 if ( ((timers[tmotion] >= fvars[fvstall]) || (lps_stall == 1)) ||
				// wait for relevel timer to expire
				(timers[trelev] < 20) ||	 // wait 2 seconds
	    		// can't relevel up if on the up directional stop
		  		(rdinp(i_UNsel) == 0) ||
	    		// can't relevel until soft stop has occured
		  		(timers[tsoftstp] <= (fvars[fvsst] + 15)) ||
				((door_zone == 0) && ((door_locks() == 0) || (car_gate() == 0))) ||
		  		((empf != no_ep) && (empf != ep_recall) && (empf != ep_select)) )
	      	{

				dirf = 0;
				if((timers[tmotion] >= fvars[fvstall]) && (lps_stall == 0))
				  lps_stall = 1;
				else
				  timers[tmotion] = 0;
				relevel_req = 1;
				relevel = 0;
				if((timers[tsoftstp] > fvars[fvsst]) || (dirf == 2))
				{
					clroutp(o_MCC);      // Clear motor starter output
					clroutp(o_MCA);
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
						clroutp(o_MST); // Turn off motor starter timer output
				}
		    	setoutp(o_LE);        // set the leveling enable output
				setoutp(o_LE1);
				clroutp(o_SU);      // Clear up output
				clroutp(o_SD);   // Clear down output
				clroutp(o_RUN);   // Clear run output
				clroutp(o_RUNA);
			}
		    else
		    {
		      	// relevel up
				if((dn_level == 0) && (up_level == 1) || 
						((door_zone == 0) && (door_locks() == 1) && (car_gate() == 1)) )
				{
					relevel = 1;
					relevel_req = 0;
			  		dirf = 1;
					if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
					{
			  			if(timers[tmotion] > fvars[fvydel])
			     			setoutp(o_MST);
				  		else
				     		clroutp(o_MST);
					}
			  		setoutp(o_MCC);    // Turn on motor starter output
			  		setoutp(o_SU);    // Turn on up output
			  		setoutp(o_RUN); // Turn on the run output
			  		clroutp(o_SD); // Turn off down output
				}
			}
		}
	    doors();
	    if(cons[rear] != 0)
			rdoors();

  	}
  	return;
}

//**********************************
// Manual door close contact is made
//**********************************

int16 man_dcc(void)
{						
	return ((man_fdcc() == 1) && ((man_rdcc() == 1) || (cons[rear] == 0)));
}

int16 man_fdcc(void)
{
	if ((cons[mand] != 0) || (cons[frghtd] != 0) || (cons[Australia] == 1))
	{
		if (cons[topf] > 2)
		{
			if (((rdinp(i_BDC) == 1) || (((cons[access_type] & 0x0A) == 0x08) && (cons[rear] != 0))) && 
				((rdinp(i_TDC) == 1) || (((cons[access_type] & 0x05) == 0x04) && (cons[rear] != 0))) &&
				(rdinp(i_MDC) == 1)) 
				return(1);
			else
				return(0);
		}
		else
		{		  // Special case of only top and bottom floor
			if (((rdinp(i_BDC) == 1) || (((cons[access_type] & 0x0A) == 0x08) && (cons[rear] != 0))) && 
				((rdinp(i_TDC) == 1) || (((cons[access_type] & 0x05) == 0x04) && (cons[rear] != 0))) &&
			    (((cons[access_type] & 0x0C) == 0) || (rdinp(i_MDC) == 1)))
				return(1);
			else
				return(0);
		}
	}
	else
		return(0); 
}

int16 man_rdcc(void)
{
	if ((cons[mand] != 0) || (cons[frghtd] != 0) || (cons[Australia] == 1))
	{
		if (cons[topf] > 2)
		{
			if (((rdinp(i_BDC) == 1) || (((cons[access_type] & 0x0A) == 0x02) || (cons[access_type] == 0))) && 
				((rdinp(i_TDC) == 1) || (((cons[access_type] & 0x05) == 0x01) || (cons[access_type] == 0))) &&
				(rdinp(i_MDCR) == 1)) 
				return(1);
			else
				return(0);
		}
		else
		{		  // Special case of only top and bottom floor
			if (((rdinp(i_BDC) == 1) || (((cons[access_type] & 0x0A) == 0x02) || (cons[access_type] == 0))) && 
				((rdinp(i_TDC) == 1) || (((cons[access_type] & 0x05) == 0x01) || (cons[access_type] == 0))) &&
			    (((cons[access_type] & 0x3) == 0) || (rdinp(i_MDCR) == 1)))
				return(1);
			else
				return(0);
		}
	}
	else
		return(0); 
}

// ********************************
// This is the motion procedure
// ********************************
void motion()
{
  int16 running=0;
  
  static int16 fire_stopsw_hold_once;

  eexit = 0;        // emergency exit of the motion prodedure

  prev_dirf = dirf;
  redundancy_test = 0;
  in_redundancy_test = 0;
  ncu_door_seq = 0;
  leveling_fault_cnt = 0;
  ovs_dbn_tim = 0;
  preset = 0;
  
  timers[tnudge] = 0;
  timers[trnudge] = 0;
  timers[teeto] = 0;
  timers[treeto] = 0;
  timers[tindrcl] = 0;
  timers[tgts_ex] = 0;

  preopen_time = 0;
  pwr_up_reset = 0;
  pwr_up_firehold_rst = 0;
  timers[tdonbrkflt] = 0;
  front_slowdown = 0;
  rear_slowdown = 0;
  lev_startf = 0;

  clr_door_vars();

  if ((dirf == 1) && (position == lobby_fl))
  {
	if ((lwd_set_at_lby == 1) || (nmb_cc_above_lby >= fvars[fvupcccnt]))
		up_peak_trigger = 1;
  }

  fault_run = 0;
  timers[tlevto] = 0;
  lev_to = 0;

  trigger_arm_state |= MOTION_START;

  while((procf >= 3) && (procf <= 11))
  {

	write_trace(TR_MOTION1);
	
	set_position_from_dpp();		// set the valid position from APS system
	
	set_ul_dl_dz();		// sets up_level, dn_level and door_zone



	// procf == 3 hsf==1 && dirf==1
	// procf == 4 hsf==0 && UL==0
	// procf == 5 hsf==0 && UL==1
	// procf == 6 hsf==1 && dirf==2
	// procf == 7 hsf==0 && DL==0
	// procf == 8 hsf==0 && DL==1

	if ((procf >= 3) && (procf <= 9))
	{
		if (estop == 1)
			procf = 9;
		else if (dirf == 1)
		{
			if (hsf == 1)
				procf = 3;
			else
			{
				if (rdinp(i_UL) == 1)
					procf = 5;
				else 
					procf = 4;
			}
		}
		else if (dirf == 2)
		{
			if (hsf == 1)
				procf = 6;
			else
			{
				if (rdinp(i_DL) == 1)
					procf = 8;
				else 
					procf = 7;
			}
		}
	}
		
	// Keep the Soft starter motor contactor on except when testing
	if ((Drive_Type[cons[carnmb]] == 9) && (in_redundancy_test == 0))
		setoutp(o_MST);

	if(door_zone == 0)
	    DZ_ON_Fault = 0;
	if (door_zone_aux == 0)
		DZA_ON_Fault = 0;
	if(up_level == 0)
	    UL_ON_Fault = 0;
	if (dn_level == 0)
		DL_ON_Fault = 0;
	
 	//intermittent problem with pretorque will not let 
 	//nudging output clear before going in motion
 	clroutp(o_NUD);
	if(cons[rear] != 0)
		clroutp(o_NUDR);
	nudg_buz = 0;
	rnudg_buz = 0;
	att_buz = 0;
	sabb_buz = 0;
	sabb_ee_en = 0;
	att_buz_on = 0;
	timers[tattbz] = 0;
	timers[tattbzcyc] = 0;
	door_buz = 0;
	freight_fdoor_alarm = 0;
	freight_rdoor_alarm = 0;
	timers[tdoorbz] = 0;
	timers[theofautorst] = 0;

	if (timers[tdppflt] >= 20)
	{	//check every 2 seconds and speed > 75 fpm
	
		if (hsf == 1)
		{
			if(((cons[speed] >= 50)&&(labs(DPP_Count - old_dpp) < (2 * Pulses_per_Inch))) ||
				((cons[speed] < 50)&&(labs(DPP_Count - old_dpp) < (1 * Pulses_per_Inch)) ))
			{
				hsf = 0;		// Immediate slowdown
				if (fault_run == 0)
					fault_run_cnt++;
				fault_run = 1;
		  		record_fault(f_pulse_75fpm_2in);
			}
			if((dirf == 1) && (DPP_Count < old_dpp))
			{
				if (fault_run == 0)
					fault_run_cnt++;
				fault_run = 1;
				hsf = 0;		// Immediate slowdown
		  		record_fault(f_pulse_dir_up);
			}
			if((dirf == 2) && (DPP_Count > old_dpp))
			{
				hsf = 0;		// Immediate slowdown
				if (fault_run == 0)
					fault_run_cnt++;
				fault_run = 1;
		  		record_fault(f_pulse_dir_dn);
			}
		}
		old_dpp = DPP_Count;
		timers[tdppflt] = 0;
	}

	write_trace(TR_MOTION2);

	cancel_up = 0;
	cancel_dn = 0;
	cancel_upr = 0;
	cancel_dnr = 0;
	inctime(0);          // increment the timers
	if (((((firef == 0) || (hsvf == 1)) && (( empf == no_ep) || (empf == ep_select))) || (firef >= 4) || (rfiref >= 4)) )
	  	carcall();			// not on fire or on phase 2 and not on emergency power or selected to run
	if (((firef == 0) && (ind() == 0) && (codebf == 0) && (vipf == 0)) && (( empf == no_ep) || (empf == ep_select)))
	  	hallcall();

	return_to_lobby();
	elevator_off();
	jail_break();
	setvipf();
	setcodebf();
	sethsvf();
	setfsf();
	setempf();
	seteqf();
	setmedf();
	settugf();

 	write_trace(TR_MOTION3);

	if (cons[hotoilSW] != 0)
		hot_oil();

	if(rdinp(i_ZON) == 1)
		zoneelev();
	if ( (cons[Sabbath] == 5) && (sabbath_active == 1) )
	{
		if ((car_Grp_Stat.c_ir_active == 1) && ((fvars[fvsabben] & 0x01) != 0))
		{
			if (servf == s_SABBATH)
			{
				servf = s_AUTO;
				clrcc(0);
			}
		}
	}


	if (((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1))) && 
				((firef > 3) || ((firef == 0) && ((servf == s_IND) || (servf == s_ATT)))))
	{
	    clrcc(0);
		dpref = 0;
	}
	if ((firef >= 1) && (firef <= 3) && (medf == 0) && (hsvf == 0))
	{
		clrcc(0);
		clrhc();
		setoutp(o_FST);  // turn on the fire service stopswitch bypass
		setoutp(o_FSTP);  // turn on the fire service stopswitch bypass
		FS_DOB_DIS = 1;
		clr_dir_arrow();
	}
	else if ((medf >= 1) && (medf <= 2))
	{		  // override stop swtich on medical emergency service
		clrcc(0);
		clrhc();
		setoutp(o_FST);  // turn on the fire service stopswitch bypass
		setoutp(o_FSTP);  // turn on the fire service stopswitch bypass
		FS_DOB_DIS = 1;
		clr_dir_arrow();
	}
	else if((car_Grp_Stat.c_ids_return_typ & 0x01) != 0)
	{
		clrhc();
		setoutp(o_FST);  // turn on the fire service stopswitch bypass
		setoutp(o_FSTP);  // turn on the fire service stopswitch bypass
		FS_DOB_DIS = 1;
		clr_dir_arrow();		
	}
	else
	{
	  clroutp(o_FST);  // turn off the fire service stopswitch bypass
	  clroutp(o_FSTP);  // turn off the fire service stopswitch bypass
	  FS_DOB_DIS = 0;
	}

   	if(safeties() == 1)
		timers[tsst] = 0;
   	else if (timers[tsst] > 2)
	{		// the saftey string is open
	  	eexit = 1;
		if ((statusf & sf_SS) == 0)
		  	record_fault(f_sstring);
      	statusf |= sf_SS;
	  	procf = 12;
	}
	else
		statusf &= ~sf_SS;
				// Testing to see if LC fuse is blown
	if(rdinp(i_S10) == 0)
	{
		eexit = 1;
		if ((statusf & sf_S10) == 0)
		  	record_fault(f_S10_fuse);
		statusf |= sf_S10;
		procf = 12;
	}
	else
		statusf &= ~sf_S10;

 	if((chk_ins() == 1) && (safeties() == 1))
	{
		if (timers[tinst] > 3)
		{
			procf = 2;
			eexit = 1;
			record_fault(f_ins_eexit);
		}
	}
	else
		timers[tinst] = 0;
	
	write_trace(TR_MOTION4);

	if(eexit == 1)
	{
		record_fault(f_eexit);
		Overspeed_Test = 0;
		Buffer_Test = 0;
		NTS_Test = 0;
		ETS_Test = 0;
		DZ_ON_Fault = 0;
		DZA_ON_Fault = 0;
		UL_ON_Fault = 0;
		DL_ON_Fault = 0;
		clroutp(o_SU);
		clroutp(o_SD);
		clroutp(o_SDF);
		clroutp(o_SUF);
		clroutp(o_RUN);

		clroutp(o_DC);
		clroutp(o_DO);
		clroutp(o_DCR);
		clroutp(o_DOR);
		write_trace(TR_MOTION5);
		return;
	}

	if((dirf == 1) && (rdinp(i_UNsel) == 0))
	{                                   // hit the up directional stop
		clroutp(o_SUF);
		clroutp(o_SU);
		if (estop == 0)
		{		// Write fault once
		  	record_fault(f_un_open);
		}
		timers[tcablnt] = 0;
	 	timers[trcablnt] = 0;
		timers[tdo] = 0;
		estop = 1;
	}
	if((dirf == 2) && (rdinp(i_DNsel) == 0))
	{                                   // hit the up directional stop
		clroutp(o_SDF);
		clroutp(o_SD);
		if (estop == 0)
		{	   // Write fault once
		  	record_fault(f_dn_open);
		}
		timers[tcablnt] = 0;
	 	timers[trcablnt] = 0;
		timers[tdo] = 0;
		estop = 1;
	}

	if ((rdinp(i_MRSW) == 0) && ((rdinp(i_FFS) == 1) || (cons[firesv] != 2) || (cons[ansi] < 2004)))       // stop switch pulled
	{
		if (estop == 0)
		{
		  	record_fault(f_stop_sw);
		}
		estop = 1;
		timers[tmotion] = 0;		// car is not moving
	}

	if (((cons[firesv] == 2) && (cons[ansi] >= 2004)) && 
		(((rdinp(i_CTS) == 1) && (rdinp(i_FFS) == 0)) || (fire_stopsw_hold == 1)))        // stop switch pulled
	{
		estops_1_run = 0;
		if (estop == 0)
		  	record_fault(f_FF_stop_sw);
		estop = 1;
		timers[tmotion] = 0;		// car is not moving
	}

	if (safe() == 0)
	{
		if (timers[tsafe] > 1)
		{		// delay for 100 msec
			if (predof == 0)
			{
				if (estop == 0)
			  		record_fault(f_car_safe);
				estop = 1;
				set_safe_fault();
			}
			if((predof == 1) && (door_zone == 0))
			{
				if (estop == 0)
		  			record_fault(f_car_safe_preop);
				estop = 1;
				set_safe_fault();
			}
		}
	}
	else
	{
		timers[tsafe] = 0;
		safe_fault_latch = 0;
	}

	if ((cons[Low_Press] == 1) &&
		(((rdinp(i_LPS) == 1) && (fvars[fvlps] == 0)) || 
		((rdinp(i_LPS) == 0) && (fvars[fvlps] == 1))))
	{
		if (dirf == 2)
		{		  // stop the car from going down
			if (estop == 0)
			{
				if ((hsf == 0) || ((hsf == 1) && (stepf == 0)))		// had already decremented position but we are changing direction and going up
					position++;
				if (position > cons[topf])
					position = cons[topf];
			}
			hsf = 0;
			estop = 1;
		}
	}

	if ( (estop == 0) && 
		((timers[tmotion] >= fvars[fvstall]) || 
			((cons[Low_Oil] == 1) && (stallf == 0) &&
				(((rdinp(i_LOS) == 1) && (fvars[fvlos] == 0)) || 
				 ((rdinp(i_LOS) == 0) && (fvars[fvlos] == 1))) ) ) )
	{
		if (fault_run == 0)
			fault_run_cnt++;
		fault_run = 1;
		// Clear direction arrows
		clr_dir_arrow();
		clrcc(0);		// clear car calls  (This will cancel the calls on a Phase 2 car)
		if ((cons[Low_Press] == 1) &&
			(((rdinp(i_LPS) == 1) && (fvars[fvlps] == 0)) || 
			((rdinp(i_LPS) == 0) && (fvars[fvlps] == 1))))
		{
			lps_stall = 1;
			hsf = 0;
		}
		else
		{
			if ((cons[Low_Oil] == 1) &&
				(((rdinp(i_LOS) == 1) && (fvars[fvlos] == 0)) || 
			 	 ((rdinp(i_LOS) == 0) && (fvars[fvlos] == 1))) ) 
		  		record_fault(f_lowoil_switch);
			else
		  		record_fault(f_stall_fault);
			stallf = 1;			// take out anti stall
			servf = s_STALLED;			// take out anti stall
			dirf = 2;		
			dpref = 2;		// force a down run
			if (rdinp(i_DTsel)==0)		// on the down terminal limit
				hsf = 0;			  // level to the floor
			else
				hsf = 1;			  // else run high speed to the floor
				
			if((fvars[fvfs2wsd] == 1) && (firef>=4))
			{
				fire_stopsw_hold = 1;
			}
		}
		estop = 1;
	}


	if ((estop == 0) && (cons[hotoilSW] != 0) && (hotoilf == 0) &&
		(((cons[hotoilSW] == 1) && (rdinp(i_TPH) == 1)) ||
		 ((cons[hotoilSW] == 2) && (rdinp(i_TPH) == 0) && (rdinp(i_CEN) == 1))))
	{	// code executes only once to stop car and force down direction run
		if (fault_run == 0)
			fault_run_cnt++;
		fault_run = 1;
		hotoilf = 1;
		servf = s_HOT_OIL;			// take out anti stall
		// Clear direction arrows
		clr_dir_arrow();
		clrcc(0);		// clear car calls  (This will cancel the calls on a Phase 2 car)
		record_fault(f_hotoil);
		dirf = 2;		
		dpref = 2;		// force a down run
		if (rdinp(i_DTsel)==0)		// on the down terminal limit
			hsf = 0;			  // level to the floor
		else
			hsf = 1;			  // else run high speed to the floor
		estop = 1;
		if((fvars[fvfs2wsd] == 1) && (firef>=4))
		{
			fire_stopsw_hold = 1;
		}
			
	}

	if ((cons[EMPop] & 0x02) == 0)
	{
		if((empf == ep_recall) && (dirf == 1)) 
		{	// Emergency Power has been initiated while car was moving so stop (Hydro)
			if (estop == 0)
			{
				dirf = 2;		// this code should only execute once
				dpref = 2;		// force a down run
				// Clear direction arrows
				clr_dir_arrow();
				if (rdinp(i_DTsel)==0)		// on the down terminal limit
					hsf = 0;			  // level to the floor
				else
					hsf = 1;			  // else run high speed to the floor
			}

			estop = 1;
	 	}
	 	
		if((empf == ep_recall) && (fvars[fvfs2wsd] == 1) && (firef>=4))
		{//Stop the car travelling either up or down if emp comes on
			if(fire_stopsw_hold_once == 0)
			{
				fire_stopsw_hold_once = 1;
				fire_stopsw_hold = 1;
				estop = 1;
				dpref = 0;
				dirf = 0;
			}
			
	 	}
	 	if(empf == 0)
	 	{
	 		fire_stopsw_hold_once = 0;
	 	}
	 	
	}
	write_trace(TR_MOTION6);

	if(estop == 1)
	{
		DZ_ON_Fault = 0;
		DZA_ON_Fault = 0;
		UL_ON_Fault = 0;
		DL_ON_Fault = 0;
		no_fault = 0;
		clroutp(o_SU);
		clroutp(o_SD);
		clroutp(o_SDF);
		clroutp(o_SUF);
		clroutp(o_RUN);
		timers[tdodly] = 0;
		timers[trdodly] = 0;

		if((timers[tsoftstp] > fvars[fvsst]) || (dirf == 2))	  // change from tfault to tsoftstp 1/8/08 mhd
		{
			clroutp(o_MCC);
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
				clroutp(o_MST);
		}

		if ((safe() == 0) || (fire_stopsw_hold == 1) ||  	
			((empf != no_ep) && (empf != ep_select) && (empf != ep_recall)))
		{
			timers[tmotion] = 0;	// Don't want a stall fault if the gate or locks are open.
			if (timers[tfault] > fvars[fvfltt]) 
			{
				if ((door_zone == 1) && (rset == 0) && (fire_stopsw_hold == 0) && 
					((eqf == 0) || (empf == ep_select)) )
				{				   // if on door zone then relevel to the floor
					procf = 15;               // end of the run go to the floor procedure
					eexit = 1;
				}	  

				servf = 0;
			}
		}
		else if (timers[tfault] >= fvars[fvfltt])
		{		 // Can't run until fault timer expires	and stop condition is cleared

			if ((cons[Emp_Recover] == 2) && (rdinp(i_EMP) == 1))
			{
		  		eexit = 1;
				procf = 0;		// go into reset mode
				break;
			}
			
			if ((Buffer_Test != 0) || (Overspeed_Test != 0) || (ETS_Test != 0) || (NTS_Test != 0))
			{		  
				Overspeed_Test = 0;
				Buffer_Test = 0;
				NTS_Test = 0;
				ETS_Test = 0;
		  		eexit = 1;
				procf = 0;		// go into reset mode
				break;
			}
			else
			{
				Overspeed_Test = 0;
				Buffer_Test = 0;
				NTS_Test = 0;
				ETS_Test = 0;
		    }

		    estops_1_run++;
			if (estops_1_run > 3)
			{		  
		  		eexit = 1;
			  	record_fault(f_estop);
		  		procf = 12;
				break;
			}

			if ((cons[Low_Press] == 1) && 
				(((rdinp(i_LPS) == 1) && (fvars[fvlps] == 0)) || 
				((rdinp(i_LPS) == 0) && (fvars[fvlps] == 1))))
			{
		  		eexit = 1;
				procf = 19;
				return;
			}

			if(running == 1)
			{
				timers[ccct] = 0;
				running = 0;
			}
			estop = 0;
			timers[tfault] = 0;
			timers[tsafe] = 0;
			safe_fault_latch = 0;
			timers[tlevto] = 0;

			dirsel();		// dirf flag can get changed in this routine

			if (position > cons[topf])
				position = cons[topf];
			if (position < cons[bottomf])
				position = cons[bottomf];
			dirsel();		// dirf flag can get changed in this routine
			write_trace(TR_MOTION7);

			if (dirf == 0)
			{						// incase dirf gets cleared
				if (prev_dirf != 0)
					dirf = prev_dirf;
				else
					dirf = 2;
			}

			if ((cons[equake] != 0) && (eqf != 0))	   
			{		  // on earthquake operation so run at low speed
				nstopf = 1;
				hsf = 0;
			}
			else
			{
				front_slowdown = 0;
				rear_slowdown = 0;
				nstopf = needstop();
				hsf = 1;		// slowdow counts will set the slowdown
			}

			timers[tpatdly] = 0;
			timers[tmotion] = 0;

			if((cons[frghtd] == 1) || (cons[mand] != 0))
			{
				setoutp(o_DC);
				clroutp(o_DO);
				setoutp(o_DCR);
				clroutp(o_DOR);
			}
			else
			{
				clroutp(o_DC);
				clroutp(o_DO);
				clroutp(o_DCR);
				clroutp(o_DOR);
			}

			if(rdinp(i_UNsel) == 0)
			{                                   // hit the up directional stop
				if ((safeties() == 1) && (rdinp(i_CEN) == 1) && (rdinp(i_MRSW) == 1)) 
					position = cons[topf];
				procf = 15;               // end of the run go to the floor procedure
				eexit = 1;
			}
			if(rdinp(i_DNsel) == 0)
			{                                   // hit the up directional stop
				if ((safeties() == 1) && (rdinp(i_CEN) == 1) && (rdinp(i_MRSW) == 1)) 
					position = cons[bottomf];
				procf = 15;               // end of the run go to the floor procedure
				eexit = 1;
			}

			if (((door_zone == 1) || (up_level == 1) || (dn_level == 1)) &&
				(pass_fl == 0) && (hsf == 0) && (rset == 0) && (eqf == 0))
			{				   // if on door zone then relevel to the floor
				procf = 15;               // end of the run go to the floor procedure
				eexit = 1;
			}	  
		}
		else
		{
			setoutp(o_DC);
			clroutp(o_DO);
			setoutp(o_DCR);
			clroutp(o_DOR);
		}

		if (timers[tfault] >= fvars[fvfltt])
		{			// allow soft stop to occur and the level into floor
			if ((door_zone == 1) && (rdinp(i_GS) == 0))
			{					 // Car is stopped in the door zone and the gate is open
				if ((cons[BinPiIn] == 1) || (cons[sel_type] != 0))
				{
					procf = 15;
					eexit = 1;
				}
			}
		}
		write_trace(TR_MOTION8);

	}
	else                // everything is safe to run
	{


		prev_dirf = dirf;

		
		write_trace(TR_MOTION9);
		
		if(timers[tmotion] >= 20)
			running = 1;
		if ( (servf == s_ADOFF) && (rdinp(i_AD) == 1) && 
			  ((cons[ids] == 0) || (car_Grp_Stat.c_ids_ad_off == 0)) &&
			  ((cons[Galileo] == 0) || (car_Grp_Stat.c_ui_ad_off == 0)) )
				servf = s_AUTO;
			  
		if ( (servf == s_RST_JACK) && ((rhjackf < 2) || (cons[Reset_Jack] == 0)) )
				servf = s_AUTO;
				
		timers[tfault] = 0;
		if (((fvars[fvpredo] & 0x01) != 0) && ((short_fl_run == 0) || ((fvars[fvpredo] & 0x02) == 0)))
		{
		    if((hsf == 1) || (door_zone == 0) || ((up_level == 0) && (dn_level == 0)) ||
		       (rdinp(i_AD) == 0) || 
		       (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) ||
		       (((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ad_off == 1)))
		    {
				predof = 0;
				timers[tpredo] = 0;
		    }
		    else
		    {
				if(timers[tpredo] > fvars[fvpred])
				{
					if (predof == 0)
						preopen_time = timers[tmotion];
				    predof = 1;
				}
		    }
		    if ((door_zone == 1) && (predof == 1) && ((up_level == 1) || (dn_level == 1)) && 
		    		(DZ_ON_Fault == 0) && (UL_ON_Fault == 0) && (DL_ON_Fault == 0))
		    {
				doorsel();
				if (fdoor_en == 1)
				{
					if( ((carcb[position] == 1) && (firef == 0)) ||
					    ((dpref == 1) && (upcb[position] == 1) && (firef == 0)) ||
					    ((dpref == 2) && (downcb[position] == 1) && (firef == 0)) )
					{
						if(ccmsk[cons[carnmb]][position] == 1)
						{
							if (timers[tdodly] >= fvars[fvdrdly])
								setoutp(o_DO);
						}
						clroutp(o_DC);
					}
				}
				else clroutp(o_DO);

				if ((cons[rear] != 0) && (rdoor_en == 1))
				{
					if( ((rcarcb[position] == 1) && (firef == 0)) ||
						((dpref == 1) && (rupcb[position] == 1) && (firef == 0)) ||
						((dpref == 2) && (rdowncb[position] == 1) && (firef == 0)) )
					{
						if(rccmsk[cons[carnmb]][position] == 1)
						{
							if (timers[trdodly] >= fvars[fvdrdly])
								setoutp(o_DOR);
						}
						clroutp(o_DCR);
					}
				}
				else
					clroutp(o_DOR);
		    }
		    else
		    {
				timers[tdodly] = 0;
				timers[trdodly] = 0;
				if ((cons[dcpo] == 0) && ((adv_pre_trq_run == 0) || (fvars[fvadvpretrq] == 0)))
				{
					clroutp(o_DC);
					clroutp(o_DCR);
				}
				else
				{
					setoutp(o_DC);
					setoutp(o_DCR);
				}
		    }
		}
		else
		{
 			timers[tdodly] = 0;
 			timers[trdodly] = 0;
			if ((cons[dcpo] == 0) && ((adv_pre_trq_run == 0) || (fvars[fvadvpretrq] == 0)))
			{
				clroutp(o_DC);
				clroutp(o_DCR);
			}
			else
			{
				setoutp(o_DC);
				setoutp(o_DCR);
			}
			clroutp(o_DO);
			clroutp(o_DOR);
		}

		timers[tdo] = 0;
		timers[trdo] = 0;
		timers[tsoftstp] = 0;
		if (timers[tbep] >= fvars[fvhbept])
			handicap_buz = 0;
		if(hsf == 0)
		{

			if (dpref == 0)
			{
				dirsel();
				timers[tlant] = 0;
				timers[trlant] = 0;
			}
			needstop();			// called here to set the slowdown flag
			if (front_slowdown == 0)
				timers[tlant] = 0;
			if (rear_slowdown == 0)
				timers[trlant] = 0;
			hlants();
			set_dir_arrow();

			if  ( ((up_level == 1) && (dn_level == 1) && ((door_zone == 1) || (door_zone_aux == 1)))
				|| ((door_zone == 1) && (door_zone_aux == 1) && (up_level == 0) && (dn_level == 0)) ) 
			{
				initial_stop();			// clear timers and record flight times
				procf = 15;           // end of the run go to the floor procedure
			}
		}
		else
		{
			front_slowdown = 0;
			rear_slowdown = 0;
			timers[tlevto] = 0;
			clrlant();
			if (((fvars[fvsabben2] & 0x02) != 0) && (servf == s_SABBATH)) 
			{
				if ((dirf == 1) || ((dirf == 0) && (dpref == 1)))
				{
					setoutp(o_CUL);
					cab_lant_up = 1;
					if (cons[rear] != 0)
					{
						setoutp(o_CULR);
						cab_rlant_up = 1;
					}
				}
				else if ((dirf == 2) || ((dirf == 0) && (dpref == 2)))
				{
					setoutp(o_CDL);
					cab_lant_dn = 1;
					if (cons[rear] != 0)
					{
						setoutp(o_CDLR);
						cab_rlant_dn = 1;
					}
				}
			}
			if (rdoutp(o_SUF)== 1)
			{
				if (DPP_Count < DPP_Floor_Pos[position])
					SD_Count = DPP_Floor_Pos[position] - DPP_Fl_Up_SD[position];
				else
					SD_Count = DPP_Floor_Pos[position+1] - DPP_Fl_Up_SD[position+1];
			}		
			else if (rdoutp(o_SDF) == 1)
			{
				if (DPP_Count > DPP_Floor_Pos[position])
					SD_Count = DPP_Floor_Pos[position] + DPP_Fl_Dn_SD[position];
				else if (position > 1)
					SD_Count = DPP_Floor_Pos[position-1] + DPP_Fl_Dn_SD[position-1];
				else
					SD_Count = 0;				
			}	
				
		}
		
		write_trace(TR_MOTION11);
		
		if(dirf == 1)             // going up
		{
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
			{
				if(timers[tmotion] >= fvars[fvydel])
					setoutp(o_MST);
				else
					clroutp(o_MST);
			}
			if(rdinp(i_SPD) == 0)
				clroutp(o_SU);
			else
				setoutp(o_SU);

			setoutp(o_RUN);
			setoutp(o_MCC);
			clroutp(o_SD);
			clroutp(o_SDF);
			write_trace(TR_MOTION12);
			if(hsf == 1)
			{
				timers[tlant] = 0;
				timers[trlant] = 0;

				if(rdinp(i_SPD) == 0)
					clroutp(o_SUF);
				else if (no_high_speed == 0)
					setoutp(o_SUF);
				
				
				if (((short_fl_run == 0) && (DPP_Count < DPP_Floor_Pos[position]) && (DPP_Count >= (DPP_Floor_Pos[position] - DPP_Fl_Up_SD[position]))) ||
					((short_fl_run == 1) && (DPP_Count < DPP_Floor_Pos[position]) && (DPP_Count >= (DPP_Floor_Pos[position] - DPP_SFl_Up_SD[position]))) ||
					(rdinp(i_UTsel) == 0))
				{
					if (rdinp(i_UTsel) == 0)
							hsf = 0;
					dirsel();
					nstopf = needstop();
					if (((cons[Car_Sw] & 1) != 0) && (cons[att] == 0))
					{
						if (rdinp(i_ATT) == 0)
							nstopf = 1;
					}
					if(nstopf != 0)
						hsf = 0;
					pisout(position);
					timers[tbep] = 0;
					if ((hb_en == 1) && ((valid_fl[position] != 0) || (fvars[fvhbz_nvfl] == 0)) && ((cons[hben] & 0x04) != 0x04))
						handicap_buz = 1;
					timers[tlant] = 0;
					timers[trlant] = 0;
					if (hsf != 0)
						short_fl_run = 0;
				}
			}
			else
			{
				clroutp(o_SUF);
			}
		}
		
		write_trace(TR_MOTION13);
		
		if(dirf == 2)             // going down
		{

			setoutp(o_RUN);


			setoutp(o_SD);
			clroutp(o_MCC);
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
				clroutp(o_MST);
			
			clroutp(o_SU);
			clroutp(o_SUF);

			write_trace(TR_MOTION14);
			
			if(hsf == 1)
			{
		     	timers[tlant] = 0;
		     	timers[trlant] = 0;
				if (no_high_speed == 0)
			  		setoutp(o_SDF);
				
				if (((short_fl_run == 0) && (DPP_Count > DPP_Floor_Pos[position]) && (DPP_Count <= (DPP_Floor_Pos[position] + DPP_Fl_Dn_SD[position]))) ||
					((short_fl_run == 1) && (DPP_Count > DPP_Floor_Pos[position]) && (DPP_Count <= (DPP_Floor_Pos[position] + DPP_SFl_Dn_SD[position]))) ||
					(rdinp(i_DTsel) == 0))
				{
					if (rdinp(i_DTsel) == 0) 
			     	  	hsf = 0;
			     	dirsel();
			     	nstopf = needstop();
					if (((cons[Car_Sw] & 1) != 0) && (cons[att] == 0))
					{
						if (rdinp(i_ATT) == 0)
							nstopf = 1;
					}
			     	if(nstopf != 0)
			     	  	hsf = 0;
			     	pisout(position);
			     	timers[tbep] = 0;
			     	if ((hb_en == 1) && ((valid_fl[position] != 0) || (fvars[fvhbz_nvfl] == 0)) && ((cons[hben] & 0x04) != 0x04))
						handicap_buz = 1;
			     	timers[tlant] = 0;
			     	timers[trlant] = 0;
					if (hsf != 0)
						short_fl_run = 0;
			  	}
			}
			else
			{
			  	clroutp(o_SDF);
			}
      	}
    }
  }
  up_peak_trigger = 0;		 // This flag is set only while the car is moving
  return;
}

// *******************************************************
// This procedure checks to see if there is a need to stop
// *******************************************************
int16 needstop()
{
	int16 fl_ix;

	write_trace(TR_NEEDSTOP);

	fl_ix = (position-1)/32;

	allow_run = 0;

	if (((fvars[fvstplby] & 0x04) != 0) && (position == lobby_fl))
	{
		if (((dirf == 2) && ((fvars[fvstplby] & 0x02) != 0)) ||
			((dirf == 1) && ((fvars[fvstplby] & 0x01) != 0)))
			front_slowdown |= (cc_sd | cc_do);
	}

  	if((carcb[position] == 1) && ((((firef == 0) || (hsvf == 1)) && (( empf == no_ep) || (empf == ep_select))) || (firef >= 4)) )
		front_slowdown |= (cc_sd | cc_do);

  	if(cons[rear] != 0)
  	{
	  	if((rcarcb[position] == 1) && ((((firef == 0) || (hsvf == 1)) && (( empf == no_ep) || (empf == ep_select))) || (rfiref >= 4)) )
	    	rear_slowdown |= (cc_sd | cc_do);
	}

	if ((cons[att] != 0) && (servf == s_ATT) && (rdinp(i_ATTBY) == 1) && (firef == 0))
		bypass_hc = 1;		// Okay to bypass hall calls
	else
		bypass_hc = 0;

	if (((cons[Car_Sw] & 1) != 0) && (cons[att] == 0))
	{
		bypass_hc = 1;		// Okay to bypass hall calls
		if ((rdinp(i_ATTUP) == 0) &&
			(rdinp(i_ATTDN) == 0))
			front_slowdown = (cc_sd | cc_do);
	}

  	if( (dpref == 1) && (upcb[position] == 1) && (firef == 0) && (bypass_hc == 0) && (( empf == no_ep) || (empf == ep_select)))
	{
    	front_slowdown |= (uc_sd | uc_do);
		if ((c_irup_hall_call[fl_ix] & fl_mask[position]) != 0)
			front_slowdown |= iruc_sd;
  	}
  	if( (dpref == 2) && (downcb[position] == 1) && (firef == 0) && (bypass_hc == 0) && (( empf == no_ep) || (empf == ep_select)))
	{
    	front_slowdown |= (dc_sd | dc_do);
		if ((c_irdn_hall_call[fl_ix] & fl_mask[position]) != 0)
			front_slowdown |= irdc_sd;
	}
  	if(cons[rear] != 0)
  	{
	  	if( (dpref == 1) && (rupcb[position] == 1) && (firef == 0) && (bypass_hc == 0) && (( empf == no_ep) || (empf == ep_select)))
		{
	    	rear_slowdown |= (uc_sd | uc_do);
			if ((c_rirup_hall_call[fl_ix] & fl_mask[position]) != 0)
				rear_slowdown |= iruc_sd;
	  	}
	  	if( (dpref == 2) && (rdowncb[position] == 1) && (firef == 0) && (bypass_hc == 0) && (( empf == no_ep) || (empf == ep_select)))
		{
	    	rear_slowdown = (dc_sd | dc_do);
			if ((c_rirdn_hall_call[fl_ix] & fl_mask[position]) != 0)
				rear_slowdown |= irdc_sd;
	  	}
  	}

  	if((rdinp(i_UTsel) == 0) && (dirf == 1) && ((cons[shortfl] < (cons[topf]-1)) || (hsf != 0)))
    	return(1);
  	if((rdinp(i_DTsel) == 0) && (dirf == 2) && ((cons[shortfl] != cons[bottomf]) || (hsf != 0)))
    	return(1);
 
 #if (Simulator == 0) 	
	if ((cop_can_error == 1) || 
		((aps_sel_can_error == 1) && (cons[sel_type] == 4)) ||
		((cons[Voltage_Sensor] == 1) && (voltf != 0)) )
		return(1);					// Stop at next floor if toc comm error
 #endif 
 
  	if((position == cons[topf]) && (dirf == 1))
    	return(1);
  	if((position == cons[bottomf]) && (dirf == 2))
    	return(1);
  	if(rset != 0)
    	return(0);
  	if(dirf == 0)
    	return(1);

  	if(cons[equake] != 0)                 // is earth quake feature enabled
	{
		if ((eqf != 0) || (servf == s_EAQ))
			return (1);
	}

	// Hydro
	if (empf != no_ep)
	{
		if ((cons[EMPop] & 0x02) == 0)
		{
			if (dirf == 1)
				return(1);		// cannot run up on emergency power loweriing
			else if (firef == 0)
				return(0);		// not on fire so continue running down
		}
	}
	if (medf != 0)
	{
		if (medf == 1)
		{
      		if( ((dirf == 1) && (fvars[fvmedfl] <= position)) ||
	  		((dirf == 2) && (fvars[fvmedfl] >= position)) )
	  			return(1);
		}
  	}
  	else if ((firef == 1) && (hsvf == 0) && ((codebf == 0) || (fvars[fvcboverfs] == 0)))	// On fireservice phase 1 return to main
  	{	
      	if( ((dirf == 1) && (fvars[fvfs1f] <= position)) ||
	  	((dirf == 2) && (fvars[fvfs1f] >= position)) )
	  		return(1);
  	}
  	else if ((firef == 2) && (hsvf == 0) && ((codebf == 0) || (fvars[fvcboverfs] == 0)))		// On fireservice phase 1 return to alternate
  	{
      	if( ((dirf == 1) && (fvars[fvfs1af] <= position)) ||
	  	((dirf == 2) && (fvars[fvfs1af] >= position)) )
	  		return(1);
  	}
	if ((cons[EMPop] & 0x02) != 0)
  	{
  		if (empf == ep_recall) 	// First on Emergency power or during EP recall
		{
	      	if( ((dirf == 1) && (fvars[fvepfl] <= position)) ||
		  	((dirf == 2) && (fvars[fvepfl] >= position)) )
		  		return(1);
		}
		else if ((empf != no_ep) && (empf != ep_select))
			return(1);		// if not selected to run the stop the car
	}

	if ((front_slowdown != 0) || (rear_slowdown != 0))
		return(1);

  	if( (position == lby_req_fl) && (servf == s_AUTO) && (dpref == 1) && (firef == 0) )
	{		// at the lobby request floor
		if ((cc_above_lby == 0) || (dirf == 2))
    		return(1);		// allow to stop if no car call above the lobby or moving down
	}
  	if( (position == park_fl) && (servf == s_HOMING) )
    	return(1);
  	if( (position == zone_fl) && (servf == s_LW_CAL) )		// Calibrate load weigher
    	return(1);
  	if((dirf == 1) && ((dcalls & 0x01) == 0))
    	return(1);          //absence of a call going up
  	if((dirf == 2) && ((dcalls & 0x02) == 0))
    	return(1);          //absence of a call going down
  	return(0);
}
// *****************************************
// This is the position indicator procedure
// *****************************************
void pisout(int16 pos)
{
	int16 loc_pi_pos = 1;
	int16 i;

	if ((valid_fl[pos] == 0) && (fvars[fvhbz_nvfl] == 1))
		return;

	if (cons[Xref_PI_Pos] == 1)
		loc_pi_pos = xpi_pos[pos];
	else
		loc_pi_pos = pos;

	if (((cons[dispis] & 0x01) != 0) || ((cons[Hall_Lant] & 0x02) != 0))
	{		// discrete PI's
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if ((cons[dispis] & 0x01) != 0)
			{
				if (i == loc_pi_pos)
					setoutp(o_PIC(i));
			  	else
					clroutp(o_PIC(i));
			}
		  	if ((cons[Hall_Lant] & 0x02) != 0)
		  	{
				if (i == loc_pi_pos)
					setoutp(o_PI(i));
			  	else
					clroutp(o_PI(i));
		  	}
		}
	}
	if ((cons[dispis] & 0x80) != 0) 
	{		// binary pi in the car
	  	if((loc_pi_pos & 0x01) != 0)
			setoutp(o_DPI1C);
	  	else
			clroutp(o_DPI1C);
		
	  	if((loc_pi_pos & 0x02) != 0)
			setoutp(o_DPI2C);
	  	else
			clroutp(o_DPI2C);

	  	if((loc_pi_pos & 0x04) != 0)
			setoutp(o_DPI3C);
	  	else
			clroutp(o_DPI3C);

	  	if((loc_pi_pos & 0x08) != 0)
			setoutp(o_DPI4C);
	  	else
			clroutp(o_DPI4C);

      	if((loc_pi_pos & 0x10) != 0)
			setoutp(o_DPI5C);
	  	else
			clroutp(o_DPI5C);

      	if((loc_pi_pos & 0x20) != 0)
			setoutp(o_DPI6C);
	  	else
			clroutp(o_DPI6C);
	}
	if ((cons[Hall_Lant] & 0x80) != 0)
	{
	  	if((loc_pi_pos & 0x01) != 0)
			setoutp(o_DPI1);
	  	else
			clroutp(o_DPI1);
		
	  	if((loc_pi_pos & 0x02) != 0)
			setoutp(o_DPI2);
	  	else
			clroutp(o_DPI2);

	  	if((loc_pi_pos & 0x04) != 0)
			setoutp(o_DPI3);
	  	else
			clroutp(o_DPI3);

	  	if((loc_pi_pos & 0x08) != 0)
			setoutp(o_DPI4);
	  	else
			clroutp(o_DPI4);

      	if((loc_pi_pos & 0x10) != 0)
			setoutp(o_DPI5);
	  	else
			clroutp(o_DPI5);

      	if((loc_pi_pos & 0x20) != 0)
			setoutp(o_DPI6);
	  	else
			clroutp(o_DPI6);
	}
		
		
	return;
}


//*********************************************
// Record fault routine
//*********************************************

void record_fault (uint16 code)
{
	if (code > max_nmbflts)
		return;
	if ((code == fvars[fvexcflt1]) || (code == fvars[fvexcflt2]) || (code == fvars[fvexcflt3])
		|| (code == fvars[fvexcflt4]) || (code == fvars[fvexcflt5]) || (code == fvars[fvexcflt6]))
	{
		return;
	}
	

	Current_Fault = code;
   	timers[tfltled] = 0;
	local_getdate();
	local_gettime();
	if ((f.Flts[Flt_ix].code == code) && (f.Flts[Flt_ix].code != f_MRCAN_device_fault) && 
		(f.Flts[Flt_ix].code != f_CTCAN_device_fault) && (f.Flts[Flt_ix].code != f_GRCAN_device_fault))
		f.Flts[Flt_ix].count++;
	else
	{
		if (Flt_ix >= (max_flt_ix-1))
			Flt_ix = 0;
		else
			Flt_ix++;
		Set_Fault_Bits();

		f.Flts[Flt_ix].count = 1;
		f.Flts[Flt_ix].code = code;
		f.Flts[Flt_ix].day = d.day;
		f.Flts[Flt_ix].mon = d.month;
		f.Flts[Flt_ix].year = d.year;
		f.Flts[Flt_ix].min = t.minute;
		f.Flts[Flt_ix].hour = t.hour;
		f.Flts[Flt_ix].sec = t.second;
		f.Flts[Flt_ix].pos = (uint8)position;
		f.Flts[Flt_ix].servf = (uint8)servf;
		f.Flts[Flt_ix].procf = (uint8)procf;
		f.Flts[Flt_ix].doorf = (uint8)doorf;
		f.Flts[Flt_ix].rdoorf = (uint8)rdoorf;
		f.Flts[Flt_ix].dpref = (uint8)dpref;
		f.Flts[Flt_ix].dirf = (uint8)dirf;
		f.Flts[Flt_ix].empf = (uint8)empf;
		f.Flts[Flt_ix].medf = (uint8)medf;
		f.Flts[Flt_ix].codebf = (uint8)codebf;
		f.Flts[Flt_ix].eqf = (uint8)eqf;
		f.Flts[Flt_ix].firef = (uint8)firef;
		f.Flts[Flt_ix].rfiref = (uint8)rfiref;
		f.Flts[Flt_ix].hsf = (uint8)hsf;
		f.Flts[Flt_ix].startf = (uint8)startf;
		f.Flts[Flt_ix].dcalls = (uint8)dcalls;
		f.Flts[Flt_ix].estop = (uint8)estop;
		f.Flts[Flt_ix].nstopf = (uint8)nstopf;
		f.Flts[Flt_ix].relevel = (uint8)relevel;
		f.Flts[Flt_ix].stepf = (uint8)stepf;
		f.Flts[Flt_ix].door_seqf = (uint8)Door_Seq_Flags;
		f.Flts[Flt_ix].next_stop = (uint8)next_stop;
		f.Flts[Flt_ix].ins_status = (uint8)ins_status;
		f.Flts[Flt_ix].ncu_door_seq = (uint8)ncu_door_seq;

		if (fault_device != 0)
		{
			f.Flts[Flt_ix].device = (uint8)fault_device;
			fault_device = 0;
		}
		else
			f.Flts[Flt_ix].device = 0;

		if (fault_prog_flag1 != 0)
		{
			f.Flts[Flt_ix].prog_flag1 = (uint8)fault_prog_flag1;
			fault_prog_flag1 = 0;
		}
		else
			f.Flts[Flt_ix].prog_flag1 = 0;

		if (fault_prog_flag2 != 0)
		{
			f.Flts[Flt_ix].prog_flag2 = (uint8)fault_prog_flag2;
			fault_prog_flag2 = 0;
		}
		else
			f.Flts[Flt_ix].prog_flag2 = 0;

		if (fault_device_2 != 0)
		{
			f.Flts[Flt_ix].device_2 = (uint8)fault_device_2;
			fault_device_2 = 0;
		}
		else
			f.Flts[Flt_ix].device_2 = 0;
		

		f.Flts[Flt_ix].iodata[0] = iodata[cons[carnmb]][0];
		f.Flts[Flt_ix].iodata[1] = iodata[cons[carnmb]][1];
		f.Flts[Flt_ix].iodata[2] = iodata[cons[carnmb]][2];
		f.Flts[Flt_ix].iodata[3] = iodata[cons[carnmb]][3];
		f.Flts[Flt_ix].iodata[4] = iodata[cons[carnmb]][4];
		f.Flts[Flt_ix].iodata[5] = iodata[cons[carnmb]][5];
		f.Flts[Flt_ix].iodata[6] = iodata[cons[carnmb]][6];
		f.Flts[Flt_ix].iodata[7] = iodata[cons[carnmb]][7];
		f.Flts[Flt_ix].iodata[8] = iodata[cons[carnmb]][8];
		f.Flts[Flt_ix].iodata[9] = iodata[cons[carnmb]][9];
		f.Flts[Flt_ix].iodata[10] = iodata[cons[carnmb]][10];	// ioA
		f.Flts[Flt_ix].iodata[11] = iodata[cons[carnmb]][11];	// ioB
		f.Flts[Flt_ix].iodata[12] = iodata[cons[carnmb]][12];	// ioC
		f.Flts[Flt_ix].iodata[13] = iodata[cons[carnmb]][13];	// ioD

		f.Flts[Flt_ix].iodata[14] = iodata[cons[carnmb]][18];			// fire/emp ioE
		f.Flts[Flt_ix].iodata[15] = iodata[cons[carnmb]][19];			// fire/emp ioF
		f.Flts[Flt_ix].iodata[16] = iodata[cons[carnmb]][138];			// COP	ioG
		f.Flts[Flt_ix].iodata[17] = iodata[cons[carnmb]][140];			// COP	ioH
		f.Flts[Flt_ix].iodata[18] = iodata[cons[carnmb]][141];			// SEL DZ ioI
		f.Flts[Flt_ix].iodata[19] = iodata[cons[carnmb]][142];			// SEL DZ ioJ
		f.Flts[Flt_ix].iodata[20] = iodata[cons[carnmb]][143];			// SEL DZ ioK
		f.Flts[Flt_ix].iodata[21] = iodata[cons[carnmb]][144];			// SEL LIM ioL
		f.Flts[Flt_ix].iodata[22] = iodata[cons[carnmb]][145];			// SEL LIM ioM
		f.Flts[Flt_ix].iodata[23] = iodata[cons[carnmb]][146];			// SEL LIM ioN
		f.Flts[Flt_ix].iodata[24] = iodata[cons[carnmb]][147];			// front door operator ioO
		f.Flts[Flt_ix].iodata[25] = iodata[cons[carnmb]][149];			// front door operator ioP
		f.Flts[Flt_ix].iodata[26] = iodata[cons[carnmb]][153];			// CC1 WITH FS2	ioQ
		f.Flts[Flt_ix].iodata[27] = iodata[cons[carnmb]][156];			// CC1 ioR
		f.Flts[Flt_ix].iodata[28] = iodata[cons[carnmb]][150];			// rear door operator ioS
		f.Flts[Flt_ix].iodata[29] = iodata[cons[carnmb]][152];			// rear door operator ioT
		f.Flts[Flt_ix].statusf = statusf;

		f.Flts[Flt_ix].dpp_count = DPP_Count;
		f.Flts[Flt_ix].sd_count = SD_Count;
		f.Flts[Flt_ix].vel_fpm = 0;
		f.Flts[Flt_ix].drv_vel_fpm = 0;
		f.Flts[Flt_ix].dmd_vel_fpm = dmd_vel_fpm;
		f.Flts[Flt_ix].enc_vel_fpm = enc_vel_fpm;
		
		// nts data - 13 bytes
		f.Flts[Flt_ix].NTS_status1 = NTS_spi.Status1.R;
		f.Flts[Flt_ix].NTS_status2 = NTS_spi.Status1.R;
		f.Flts[Flt_ix].NTS_vel = NTS_spi.velocity;
		f.Flts[Flt_ix].NTS_poscnt= NTS_spi.pos_cnt;
		f.Flts[Flt_ix].NTS_command1 = NTS_spi.Command1.R;
		f.Flts[Flt_ix].NTS_command2 = NTS_spi.Command2.R;
		f.Flts[Flt_ix].NTS_doorzone = NTS_spi.doorzone;
		f.Flts[Flt_ix].NTS_limits = NTS_spi.limits;
		f.Flts[Flt_ix].NTS_vel_dir = NTS_spi.vel_dir;
		f.Flts[Flt_ix].unused1 = 0;
		f.Flts[Flt_ix].unused2 = 0;
				

		f.Flts[Flt_ix].fault_bits[0] = Fault_Bits[0];

		f.Flts[Flt_ix].fault_bits[1] = Fault_Bits[1];

		f.Flts[Flt_ix].fault_bits[2] = Fault_Bits[2];
		f.Flts[Flt_ix].fault_bits[3] = Fault_Bits[3];


		f.Flts[Flt_ix].statusf2 = statusf2;
		f.Flts[Flt_ix].ss_status = safety_string_status;
		f.Flts[Flt_ix].power_status = power_status;
		
		// unused data - 11 bytes
		f.Flts[Flt_ix].fl_dpp_count = DPP_Floor_Pos[position];;
		f.Flts[Flt_ix].unused7 = 0;
		f.Flts[Flt_ix].unused8 = 0;
		f.Flts[Flt_ix].unused9 = 0;
		f.Flts[Flt_ix].unused10 = 0;
		f.Flts[Flt_ix].unused11 = 0;
		f.Flts[Flt_ix].unused12 = 0;
		f.Flts[Flt_ix].unused13 = 0;
		
  		f.Flts[Flt_ix].run_statusf = run_statusf;
		
		f.Flts[Flt_ix].nudgst_flags = Nudgst_Flags;
		f.Flts[Flt_ix].door_req_flags = Door_Req_Flags;
		f.Flts[Flt_ix].call_flags = Call_Flags;
  		f.Flts[Flt_ix].chkrun_st = chkrun_state;
  		f.Flts[Flt_ix].chkstart_st = chkstart_state;
  		f.Flts[Flt_ix].chklevel_st = chklevel_state;
  		f.Flts[Flt_ix].chkdoor_st = chkdoor_state;
		f.Flts[Flt_ix].front_sd = front_slowdown;
		f.Flts[Flt_ix].rear_sd = rear_slowdown;
		f.Flts[Flt_ix].motion_tmr = timers[tmotion];
		
		f.Flts[Flt_ix].PAL_vel = 0;
		f.Flts[Flt_ix].PAL_stat = 0;
		f.Flts[Flt_ix].ins_servf = ins_servf;
		f.Flts[Flt_ix].enc_dir = (uint8)Enc_dir;
		
		f.Flts[Flt_ix].drv_cmd = 0;
		f.Flts[Flt_ix].drv_status = 0;
		f.Flts[Flt_ix].drv_mtr_torque = 0;
		f.Flts[Flt_ix].drv_torque_cmd = 0;
		f.Flts[Flt_ix].percent_load = (uint8)(percent_load/10);
		f.Flts[Flt_ix].lev_startf = (uint8)lev_startf;
		f.Flts[Flt_ix].statusf3 = statusf3;
		f.Flts[Flt_ix].statusf4 = statusf4;
		f.Flts[Flt_ix].spare = 0;

		Update_Flt();
		Update_LT_Flt();
	}
}


//***************************
// Elevator is in Reset Mode
//***************************
void reset()
{
	int16 start_time;
  	int16 tmppos = 0;
	int16 call_at_fl = 0;

	int16 recover_dir = 2;
	
	write_trace(TR_RESET1);
  	
  	if(rset != 3)
	  	rset = 1;
  	
#if (Simulator == 1)
	if (firef >= 4)
		fs2_no_fdoor_op = 1;
	if (rfiref >= 4)
		fs2_no_rdoor_op = 1;
#endif  	

// Wait in reset until treset tim has expired
  	doorf = 0;
	rdoorf = 0;
	clr_door_vars();
	preset = 0;
	in_redundancy_test = 0;
	redundancy_test = 0;
	timers[trelev] = 0;
	startf = 0;
	lev_startf = 0;

	
	if ((in_emp_trying_to_reset == 1) && (empf != 0))
		servf = s_EMP;

  	timers[trst] = 0;
	start_time = 0;		// not a generator job
	if (Drive_Type[cons[carnmb]] == 9)
		setoutp(o_STE);
	timers[tydel] = 0;			// reset timer for y-deta start

	start_time += power_up_start_time;		// power up start time is non zero only on power up
	power_up_start_time = 0;

	
	if (cons[Cycle_Run] == 1) 
	{
		schedule_st_times();
	}

	if ((manual_door == 2) && (man_dcc() == 1))
	{			// clear the protection time to allow the car door to close
		timers[tdc] = 0;
	}
	if ((manual_rdoor == 2) && (man_dcc() == 1))
	{			// clear the protection time to allow the car door to close
		timers[trdc] = 0;
	}

	while (timers[treset] <= (fvars[fvrst] + start_time))
  	{
    	if(((timers[trst] > 30)&&(timers[trst] < 40)) && (cons[arst] != 0)) 
			setoutp(o_RST);
    	else
			clroutp(o_RST);

		if (rdinp(i_GOV) == 1)
			gov_flt_timer = 0;		// prevent gripper trip during reset.

		if (cons[Australia] == 1)
		{
			fs2_on = (rdinp(i_FS2) == 0) || (rdinp(i_FS2H) == 1);
			fs2_hold = 0;
			fs2_start = (rdinp(i_FS2H) == 1);
		}
		else
		{
			if (cons[FS2_SW] == 1)	// New logic FS2 = NOT FS2 and NOT FS2H, HOLD = FS2H, OFF = FS2
				fs2_on = (rdinp(i_FS2) == 0) && (rdinp(i_FS2H) == 0);
			else					// Original logic FS2 = FS2 input, HOLD = FS2H, OFF = NOT FS2 and NOT FS2H
				fs2_on = (rdinp(i_FS2) == 1);
			fs2_hold = (rdinp(i_FS2H) == 1);
			fs2_start =0;
		}
    	inctime(0);
    	clrall();
    	
		if (fs2_on == 1)
			fire_status |= fs_FS2;
		else
			fire_status &= fs_FS2;
		
		if (fs2_hold == 1)
			fire_status |= fs_FS2H;
		else
			fire_status &= fs_FS2H;
	
		if (procf == 20)
			return;
  	}

	read_hoistway_count();
	set_position_from_dpp();
	set_ul_dl_dz();		// sets up_level, dn_level and door_zone

	statusf &= ~sf_POS;

  	clroutp(o_RST);

	closetry = 0;
	rclosetry = 0;
	safe_closetry = 0;
	safe_rclosetry = 0;

	Door_Lock_GS_DPM_Fault = 0;
	statusf &= ~sf_GDL;		// cleared in reset()

	Overspeed_Test = 0;
	em_sd_run = 0;
	Buffer_Test = 0;
	NTS_Test = 0;
	ETS_Test = 0;
    NTS_spi.Command2.B.SET_UPSD = 0;		// clear the command on exit just to make sure
    NTS_spi.Command2.B.SET_DNSD = 0;		// clear the command on exit just to make sure
	NTS_spi.Command2.B.RESET_JACK = 0;
	estop = 0;
    estops_1_run = 0;
	timers[tgripper] = 0;
	ins_fault = 0;
	eef_testtry = 0;
	eer_testtry = 0;
	fado_fault_cnt = 0;
	rado_fault_cnt = 0;

	if (cons[Low_Press] == 1)
	{
		if (((rdinp(i_LPS) == 1) && 
			(fvars[fvlps] == 0)) || ((rdinp(i_LPS) == 0) && (fvars[fvlps] == 1)))
		{
			procf = 19;
			return;
		}
	}

	// Testing to see if saftey string is open
    if(safeties() == 0)
    {
		if ((statusf & sf_SS) == 0)
	  		record_fault(f_sstring);
		statusf |= sf_SS;
      	procf = 12;       // go to saftey string open mode
      	timers[tmotion] = 0;
		timers[tpatdly] = 0;
      	return;
    }
	else
		statusf &= ~sf_SS;

	// Testing to see if LC fuse is blown
	if(rdinp(i_S10) == 0)
    {
		if ((statusf & sf_S10) == 0)
		  	record_fault(f_S10_fuse);
		statusf |= sf_S10;
		procf = 12;
		timers[tmotion] = 0;
		timers[tpatdly] = 0;
		return;
    }
	else
		statusf &= ~sf_S10;


	// Testing to see if on inspection
    if(chk_ins() == 1)
    {
      	procf = 2;        // go to inspection mode
      	timers[tmotion] = 0;
		timers[tpatdly] = 0;
      	return;
    }

	if (rset_error_cnt < 5)
	{
		rset_error_cnt++;
		record_fault(f_reset);
	}
	
	
	if (rdinp(i_MRSW) == 0)
	{
	
		if ((pwr_up_reset == 1) && (firef != 0))
		{	// update FL and FBNB outputs on power up reset in case Stop Switch is in the off position
			if (firef <= 3)
			{	// only on phase 1
				if (firef != 3)
					setoutp(o_FBNB);    // Turn on the buzzer
				else
					clroutp(o_FBNB);    // Turn off the buzzer			
			
				if((Blink_FSL == 1) && (timers[tsec] != 0))
					clroutp(o_FL);
				else
					setoutp(o_FL);
			}
		}
		statusf |= sf_STP;
		rset_run_cnt = 0;
		return;		// cannot do anything until stop switch is set to run
	}
	else
		statusf &= ~sf_STP;

	write_trace(TR_RESET2);


	if ((door_zone == 0) || ((rset == 3) && (eqf == 0)))
	{						  // Not on the door zone so Close the doors
		close_doors_during_reset(0);
	}

	

	if (((manual_door == 2) || (manual_rdoor == 2)) && (man_dcc() == 0))
	{
		statusf |= sf_DCC;
		rset_run_cnt = 0;
		return;		// cannot do anything until the outside swing door is closed and not in door zone
	}
	else
		statusf &= ~sf_DCC;

  	timers[tmotion] = 0;
	timers[tpatdly] = 0;
	timers[tfault] = 0;


	

	// Make sure elevator not lost
  	if ( (rdinp(i_DTsel) == 0) && (cons[shortfl] != cons[bottomf]))
  	{     // elevator at bottom but position does not match
    	position = cons[bottomf];
    	if((up_level == 1) || (dn_level == 1))
     	// at the bottom floor on a door zone
    	{
			set_door_close_during_reset();
      		procf = 15;
      		return;
    	}
  	}

	if ((rdinp(i_EMP) == 1) && (empf == no_ep) && (pwr_up_reset == 1))
		setempf();

	
  	if (( (rdinp(i_UTsel) == 0) && (cons[shortfl] < (cons[topf]-1))) &&
		((cons[EMPop] != 0) || ((cons[EMPop] == 0) && (empf != ep_recall))))
 	{	  // Car is at the top 
    	position = cons[topf];
    	if((up_level == 1) || (dn_level == 1)) // at the top floor on a door zone
    	{	   
			set_door_close_during_reset();
      		procf = 15;
      		return;
    	}
  	}

 	
 	if (cons[sel_type] != 0)       // binary position inputs floor confirm
  	{
    	if (((door_zone == 1) && (preset == 1) && (rset != 3)) && ((cons[EMPop] != 0) || ((cons[EMPop] == 0) && (empf != ep_recall))))
    	{		  // Within 3 inches of dead level
			set_door_close_during_reset();
      		procf = 15;
      		return;
    	}
  	}

	
 	if( (up_level == 1) && (dn_level == 1) &&
    	  (rdinp(i_UTsel) == 1) && (rdinp(i_DTsel) == 1) && (rset != 3) )
  	{				// car is dead level
		set_door_close_during_reset();
    	procf = 15;
    	return;
  	}

	
	if ( ((cons[BinPiIn] == 1) || (cons[sel_type] != 0)) && (rset != 3) && ((up_level == 1) || (dn_level == 1)) )
	{	// With binary presets just relevel the car
		set_door_close_during_reset();
		
		if (((cons[equake] == 0) || (eqf <= 1) || (eqf == 4)) && 
			((cons[EMPop] != 0) || ((cons[EMPop] == 0) && (empf != ep_recall))))
    	{				  // Relevel if not on counterweight derailment
    		procf = 15;
    		return;
		}
	}

	write_trace(TR_RESET3);
	
	
	if (((cons[Car_Sw] & 1) != 0) && (cons[att] == 0))
	{	
		if (((up_level == 1) || (dn_level == 1) || (door_zone == 1)) &&
		    ((cons[EMPop] != 0) || ((cons[EMPop] == 0) && (empf != ep_recall))))
		{	
    		procf = 15;
    		return;
		}
		else
		{
			if ((rdinp(i_ATTUP) == 0) && 
				(rdinp(i_ATTDN) == 0))
			{
				rset_run_cnt = 0;
				return;
			}
			if (rdinp(i_ATTUP) == 1)
				position = cons[bottomf];

			if (rdinp(i_ATTDN) == 1)
				position = cons[topf];
		}
	}


	if (((firef != 0) || ((cons[rear] != 0) && (rfiref != 0))) && 
		((cons[ansi] >= 2007) || ((pwr_up_reset == 0) && (cons[ansi] >= 2004))))
	{
		if (cons[Australia] == 1)
		{
			fs2_on = (rdinp(i_FS2) == 0) || (rdinp(i_FS2H) == 1);
			fs2_hold = 0;
			fs2_start = (rdinp(i_FS2H) == 1);
		}
		else
		{
			if (cons[FS2_SW] == 1)	// New logic FS2 = NOT FS2 and NOT FS2H, HOLD = FS2H, OFF = FS2
				fs2_on = (rdinp(i_FS2) == 0) && (rdinp(i_FS2H) == 0);
			else					// Original logic FS2 = FS2 input, HOLD = FS2H, OFF = NOT FS2 and NOT FS2H
				fs2_on = (rdinp(i_FS2) == 1);
			fs2_hold = (rdinp(i_FS2H) == 1);
			fs2_start =0;
		}

		if (fs2_on == 1)
			fire_status |= fs_FS2;
		else
			fire_status &= fs_FS2;
		
		if (fs2_hold == 1)
			fire_status |= fs_FS2H;
		else
			fire_status &= fs_FS2H;
		

		if ((firef > 3) || ((cons[rear] != 0) && (rfiref > 3)))	// car on phase 2
		{
			carcall();
			dirsel();
			if ((rd_both_inps(i_CC(position),i_XCC(position)) == 1) ||
					((cons[rear] == 1) && (rd_both_inps(i_CCR(position),i_XCCR(position)) == 1)))
				call_at_fl = 1;
			else
				call_at_fl = 0;

			if ((fs2_hold == 1) || ((cons[Australia] == 1) && (fs2_start == 0)) ||
				((fs2_on == 1) && (dcalls == 0) && (call_at_fl == 0)))
			{
				rset_run_cnt = 0;
				return;			// do not move
			}
		}

		if (scnd_pos >= fire_floor)
			recover_dir = 2;
		else
			recover_dir = 1;
	}
	else if (((position != cons[bottomf]) && (cons[shortfl] != cons[bottomf])) ||
  		(cons[shortfl] >= (cons[topf] - 1))) // go down and establish floor position
		recover_dir = 2;
	else
		recover_dir = 1;
  	
  	if (recover_dir == 2)
  	{		 // go down and establish floor position
    	servf = s_RESET_DN;
    	dirf = 2;
  	}
  	else                          // go up and establish floor position
  	{
		servf = s_RESET_UP;
		dirf = 1;
  	}

	write_trace(TR_RESET4);

	
	setempf();
	if ((empf != no_ep)  && (empf != ep_recall) && (empf != ep_select))
	{			 // cannot run unless selected to run
		servf = s_EMP;
		in_emp_trying_to_reset = 1;
		dirf = 0;
		procf = 0;		// cause another reset delay
		return;
	}
	else
	{
		if ((empf == ep_recall) && (dirf == 1))
		{
		 	dirf = 2;		// this code should only execute once
		 	dpref = 2;		// force a down run
		 	// Clear direction arrows
			clr_dir_arrow();
			if (rdinp(i_DTsel)==0)		// on the down terminal limit
				hsf = 0;			  // level to the floor
			else
				hsf = 1;			  // else run high speed to the floor
		}
	}

	in_emp_trying_to_reset = 0;

	
   	if((firef != 0) && (rset != 3) && (door_zone == 1))
    {
		timers[tnudge] = 0;
		timers[trnudge] = 0;
		timers[tindrcl] = 0;
  		timers[teeto] = 0;
  		timers[treeto] = 0;
 		procf = 15;
		timers[tmotion] = 0;
		timers[tpatdly] = 0;
		return;
    }

	if (eqf == 0)
	{
    	if ((cons[BinPiIn] == 1) && (rset != 3)) 
	      	hsf = 0;
	  	else
	      	hsf = 1;
	}
	else
		hsf = 0;

  	timers[tmotion] = 0;
	timers[tpatdly] = 0;
	timers[tsafe] = 0;
	safe_fault_latch = 0;


	write_trace(TR_RESET5);
	
	
	if (safe() == 1) 
	{
		if (rset_run_cnt < 5)		// give up after five tries
		{
			rset_run_cnt++;
			if (((stallf == 0) || (dirf == 2)) && (dirf != 0))
			{
				procf = 3;          // go into motion from reset
			}
			else
			{
				procf = 0;		// cause another reset delay
			}
		}
		else
		{
			if (rset_run_cnt == 5)
			{
				rset_run_cnt++;
				record_fault(f_reset_run_inhibit);
			}
			dirf = 0;
			procf = 0;		// cause another reset delay
		}			
	}
	else if ((door_locks() == 0) || (car_gate() == 0))
	{		// we reached this point so we need to run to recover so shut the door
		close_doors_during_reset(1);
	}
  	return;
}	
//***********************************
// Close Elevator Doors in Reset Mode
//***********************************

void close_doors_during_reset (int16 force_close)
{
	timers[tdc] = 0;
  	timers[trdc] = 0;
	while ( ((rdinp(i_DCL) == 1) && (timers[tdc] <= fvars[fvdpt])) || 
		    ((rdinp(i_DCLR) == 1) && (timers[trdc] <= fvars[fvdpt])) ||
		    ((force_close == 1) && (timers[tdc] <= fvars[fvdpt]) && ((door_locks() == 0) || (car_gate() == 0))) )
	{
		inctime(0);
		nudg_buz = 0;
		rnudg_buz = 0;
		// Testing to see if on inspection
		if(chk_ins() == 1)
		{
	  		procf = 2;        // go to inspection mode
	  		return;
		}
		if(firef >= 4)      // On Fire Service Phase 2
		{
	  		timers[tdc] = 0;
	  		if (((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1))) && 
	  			(fs2_hold == 0))
	  		{
				if (timers[tdcdly] >= fvars[fvdrdly])
				{
					setoutp(o_DC);
	 				timers[tdodly] = 0;
				}

				if (cons[door_type] == 1)	 // otis ovl door operator
		  			clroutp(o_DO);
	  		}
	  		else
	  		{
				clroutp(o_DC);
				if (cons[door_type] == 1)	 // otis ovl door operator
		  		{
					if (timers[tdodly] >= fvars[fvdrdly])
					{
			  			setoutp(o_DO);
		 				timers[tdcdly] = 0;
					}
				}
				else
				{
					if(rdinp(i_DOL) == 1)
					{
						if (timers[tdodly] >= fvars[fvdrdly])
						{
				  			setoutp(o_DO);
			 				timers[tdcdly] = 0;
						}
					}
					else
			  			clroutp(o_DO);
				}
	  		}
		}
		else 
		{
	  		clroutp(o_DO);
	  		if ((rdinp(i_SE) == 1) || (rd_both_inps(i_DOB,i_XDOB) == 1) ||
	  			((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
	  		{
				clroutp(o_DC);
	  		}
	  		else
	  		{
				if (timers[tdcdly] >= fvars[fvdrdly])
				{
					if ((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input
						((eef2_enabled == 1) && (rdinp(i_EE2) == 1)) )	// EE2 input
					{
						nudg_buz = 1;
						setoutp(o_NUD);
					}
					setoutp(o_DC);
	 				timers[tdodly] = 0;
				}
	  		}
		}
		if (cons[door_type] == 1)
		{
			if (cons[equake] == 0)
			{
		  	  	if ((rdinp(i_SE) == 1) && (firef <= 3))    // saftey edge
					setoutp(o_RVD);
				else
					clroutp(o_RVD);
			}

			if  ((cons[medEm] == 0) && (cons[cblue] == 0))
			{
				if ((rdoutp(o_DO) == 1) && (rdinp(i_DOL) == 1))
					setoutp(o_REV);
				else
					clroutp(o_REV);
			}
		}
		// Close The Rear Doors
		if(cons[rear] != 0)
		{
		  	if(rfiref >= 4)      // On Fire Service Phase 2
		  	{
		      	timers[trdc] = 0;
		      	if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) || 
		      		((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1))) && 
		      			(fs2_hold == 0))
		      	{
					if (cons[door_type] == 1)	 // otis ovl door operator
			  			clroutp(o_DOR);

					if (timers[trdcdly] >= fvars[fvdrdly])
					{
					  	setoutp(o_DCR);
		 				timers[trdodly] = 0;
					}

		      	}
		      	else
		      	{
				  	clroutp(o_DCR);
					if (cons[door_type] == 1)
					{	// otis ovl door operator
						if (timers[trdodly] >= fvars[fvdrdly])
						{
					      	setoutp(o_DOR);
			 				timers[trdcdly] = 0;
						}
					}
					else
					{
					  	if(rdinp(i_DOLR) == 1)
						{
							if (timers[trdodly] >= fvars[fvdrdly])
							{
						      	setoutp(o_DOR);
				 				timers[trdcdly] = 0;
							}
					  	}
					  	else
						  	clroutp(o_DOR);
					}
		      	}
		  	}
		  	else
		  	{
		      	clroutp(o_DOR);
		      	if ((rdinp(i_SER) == 1) || 
		      		(rd_both_inps(i_DOBR,i_XDOBR) == 1) ||
		      		((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)))
		      	{
				  	clroutp(o_DCR);
		      	}
		     	else
		      	{
					if (timers[trdcdly] >= fvars[fvdrdly])
					{
						if ((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||		// EER1 input
							((eer2_enabled == 1) && (rdinp(i_EER2) == 1)) )		// EER2 input
						{
							rnudg_buz = 1;
						  	setoutp(o_NUDR);
						}
					  	setoutp(o_DCR);
		 				timers[trdodly] = 0;
					}
		      	}
		  	}
			if (cons[door_type] == 1)
			{
		    	if ((rdinp(i_SER) == 1) && (rfiref <= 3))   // saftey edge
					setoutp(o_RVDR);
				else
					clroutp(o_RVDR);

				if ((rdoutp(o_DOR) == 1) && (rdinp(i_DOLR) == 1))
					setoutp(o_REVR);
				else
					clroutp(o_REVR);
			}

	  	}
	}
	if (cons[door_type] == 1)
	{
		clroutp(o_DO);
		if (cons[equake] == 0)
			clroutp(o_RVD);
		if ((cons[medEm] == 0) && (cons[cblue] == 0))
			clroutp(o_REV);
	}
	clroutp(o_DC);
	clroutp(o_NUD);
	nudg_buz = 0;

	if (cons[door_type] == 1)
	{
      	clroutp(o_DOR);
		clroutp(o_RVDR);
		clroutp(o_REVR);
	}
	clroutp(o_DCR);
  	clroutp(o_NUDR);
	nudg_buz = 0;
}
//**************************************
//**** Command to close doors
//**************************************
void set_door_close_during_reset(void)
{
	
	if(((firef == 4) || ((rfiref == 4) && (cons[rear] != 0))) && (cons[ansi] >= 2007) && (rdinp(i_FS2H) == 1)) // NOT ok to close the doors-  
	{
		if((firef == 4) && (rdinp(i_DOL) == 1) && (rdinp(i_DCL) == 1))
		{
			fs2_no_fdoor_op = 0;
		}
		if((rfiref == 4) &&(cons[rear] != 0)&&(rdinp(i_DOLR) == 1) && (rdinp(i_DCLR) == 1))
		{
			fs2_no_rdoor_op = 0;
		}
	}
	else if(((firef == 5) || ((rfiref == 5) && (cons[rear] != 0))) && (cons[ansi] >= 2007) && (rdinp(i_FS2H) == 1)) // NOT ok to close the doors-  
	{
		if((rdinp(i_DOL) == 1) && (rdinp(i_DCL) == 1))
		{
			firef = 6;
		}
		else if((cons[rear] != 0)&&(rdinp(i_DOLR) == 1) && (rdinp(i_DCLR) == 1))
		{
			rfiref = 6;
		}
	}
	else if(((firef == 6) || ((rfiref == 6) && (cons[rear] != 0))) && (cons[ansi] >= 2007) && (rdinp(i_FS2H) == 1)) // NOT ok to close the doors-  
	{
		if((rdinp(i_DOL) == 1) && (rdinp(i_DCL) == 1))
		{
			fs2_no_fdoor_op = 0;
		}
		else if((cons[rear] != 0)&&(rdinp(i_DOLR) == 1) && (rdinp(i_DCLR) == 1))
		{
			fs2_no_fdoor_op = 0;
		}
	}
	else
	{
		timers[tdc] = 0;
		timers[trdc] = 0;
	   	timers[tnudge] = 0;
	   	timers[trnudge] = 0;
	    timers[teeto] = 0;
	    timers[treeto] = 0;
		if ((firef == 0) && (empf == 0) && (eqf == 0) && (in_att == 0))
		{
			doorf = 3;			// Close the door if open
			if (cons[rear] != 0)
				rdoorf = 3;
		}
	}
	pwr_up_firehold_rst = 0; // Unused boolean
}
//*************************************
// ** Reset Hydro Jack Operation
//*************************************


void reset_jack (void)
{
	static exit_reset_jack;
	
	if (cons[Reset_Jack] == 1)
	{
		timers[trhjack] = 0;
		rhjackf = 2;
		startf = 0;
		lev_startf = 0;
		exit_reset_jack = 0;
		while (procf == 18)
		{
			servf = s_RST_JACK;
			inctime(0);
			set_ul_dl_dz();		// sets up_level, dn_level and door_zone
			
	    	if(safeties() == 1)
				timers[tsst] = 0;
	    	else if(timers[tsst] > 10)
	    	{
				if ((statusf & sf_SS) == 0)
					record_fault(f_sstring);
				statusf |= sf_SS;
				exit_reset_jack = 1;
	      		procf = 12;       // go to saftey string open mode
	    	}
			else
				statusf &= ~sf_SS;

			// Testing to see if LC fuse is blown
	    	if(rdinp(i_S10) == 1)
				timers[tlct] = 0;
	    	if((rdinp(i_S10) == 0) && (timers[tlct] > 10))
	    	{
				if ((statusf & sf_S10) == 0)
					record_fault(f_S10_fuse);
				statusf |= sf_S10;
				exit_reset_jack = 1;
				procf = 12;
	    	}
			else
				statusf &= ~sf_S10;
			
	  	    if (chk_ins() == 1)
			{			   // exit on inspection
				procf = 2;
				exit_reset_jack = 1;
			}
			
			if (exit_reset_jack == 1)
			{
				exit_reset_jack = 0;
				clroutp(o_SD);
				clroutp(o_SDF);
				if ((Drive_Type[cons[carnmb]] != 9)	&& (cons[GOV_Reset] == 0))
					clroutp(o_MST);
				clroutp(o_SU);
				clroutp(o_SUF);
				clroutp(o_MCC);
				clroutp(o_RUN);
				clroutp(o_DNO);	// remove normal limit override
				rhjackf = 0;
				return;
			}
			
			setoutp(o_DC);		// Keep the door closed
			if ((safe() == 1) && (rdinp(i_DCL) == 0))
			{
				if (rhjackf == 2)
				{
					// Run the car all the way down
					setoutp(o_RUN);
					setoutp(o_SD);
					if ((Drive_Type[cons[carnmb]] != 9) && (cons[GOV_Reset] == 0))
						clroutp(o_MST);
					clroutp(o_SU);
					clroutp(o_SUF);
					clroutp(o_MCC);
					setoutp(o_DNO);  // override normal limit 
					if (timers[trhjack] > 600)
					{
						rhjackf = 3;
						timers[trhjack] = 0;
					}
					leveling = 0;		// prevent level stop Count fault
				}
				if (rhjackf == 3)
				{	// Turn off everything for 2 seconds
					clroutp(o_SD);
					clroutp(o_SDF);
					if ((Drive_Type[cons[carnmb]] != 9) && (cons[GOV_Reset] == 0))
						clroutp(o_MST);
					clroutp(o_SU);
					clroutp(o_SUF);
					clroutp(o_MCC);
					clroutp(o_RUN);
					setoutp(o_DNO);  // override normal limit 
					if (timers[trhjack] > 20)
					{	  // Time to run back up
						rhjackf = 4;
						timers[tmotion] = 0;
						timers[trhjack] = 0;
						leveling = 0;		
						level_stop_fault = 0;
						lev_latch = 0;
					}
				}
				if (rhjackf == 4)
				{	   // Run car back up to floor level
					clroutp(o_SD);
					clroutp(o_SDF);
					if ((Drive_Type[cons[carnmb]] != 9)	&& (cons[GOV_Reset] == 0))
					{
						if(timers[tmotion] >= fvars[fvydel])
							setoutp(o_MST);
						else
							clroutp(o_MST);
					}
					if(rdinp(i_SPD) == 0)
						clroutp(o_SU);
					else
						setoutp(o_SU);
					setoutp(o_RUN);
					setoutp(o_MCC);
					setoutp(o_DNO);  // override normal limit 

					if (((up_level == 1) && (dn_level == 1)) || (timers[trhjack] > 1200))
					{		// Level to the floor or timed out
						rhjackf = 5;
						timers[trhjack] = 0;
					}
				}
				if (rhjackf == 5)
				{	  // Jack reset complete
					clroutp(o_SD);
					clroutp(o_SDF);
					if ((Drive_Type[cons[carnmb]] != 9) && (cons[GOV_Reset] == 0))
						clroutp(o_MST);
					clroutp(o_SU);
					clroutp(o_SUF);
					clroutp(o_MCC);
					clroutp(o_RUN);
					clroutp(o_DNO);	// remove normal limit override
					rhjackf = 0;
					procf = 0;			// go to reset mode
				}
			}
			else if (timers[trhjack] > 200)
			{
				clroutp(o_SD);
				clroutp(o_SDF);
				if ((Drive_Type[cons[carnmb]] != 9)	&& (cons[GOV_Reset] == 0))
					clroutp(o_MST);
				clroutp(o_SU);
				clroutp(o_SUF);
				clroutp(o_MCC);
				clroutp(o_RUN);
				clroutp(o_DNO);	// remove normal limit override
				rhjackf = 0;
				procf = 0;			// go back to reset mode
			}
		}
		
		if (rdoutp(o_DNO) == 1)
			NTS_spi.Command2.B.RESET_JACK = 1;
		else
			NTS_spi.Command2.B.RESET_JACK = 0;
	}
	else
		procf = 0;
}


// ************************************
// ** Set a car call to return to lobby
// ************************************

void return_to_lobby(void)
{
	int16 fl;
  	// out of service return to lobby
  	if ((cons[otslbp] != 0) || ((cons[ids] & 1) != 0))
	{
	 	if ((firef == 0) && (rfiref == 0) && (codebf == 0) && (vipf == 0) && (eqf == 0) && (medf == 0))
 		{
		
  			if (((rdinp(i_RTLH) == 1) && (cons[otslbp] != 0)) || 
  				(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_rtl == 1)) || 
  				(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_rtl == 1)) || 
  				((rdinp(i_RTLDC) == 1) && ((cons[otslbp] & 0x04) != 0)))
			{
				
				fl = lobby_fl;
					 
				if ((fl < cons[bottomf]) || (lobby_fl > cons[topf]))
					fl = cons[bottomf];

				if ((position != fl) && ((servf == s_AUTO) || (servf == s_LBY_RCL)))
		  		{
					if ((fvars[fvrtl] & 0x02) != 0)
					{
						clrcc(0);
						if ((fvars[fvrtl] & 0x04) != 0)
						{
							if ((hsf == 0) && (procf >= 3) && (procf <= 11))
							{			   // car is in slowdown
								if ((ccmsk[cons[carnmb]][position] != 0) && (fvars[fvrtldoortyp] != 1))
									front_slowdown |= cc_do;
								else if ((cons[rear] != 0) && (rccmsk[cons[carnmb]][position] != 0))
									rear_slowdown |= cc_do;
								else if ((ccnmsk[cons[carnmb]][position] != 0) && (fvars[fvrtldoortyp] != 1))
									front_slowdown |= cc_do;
								else if ((cons[rear] != 0) && (rccnmsk[cons[carnmb]][position] != 0))
									rear_slowdown |= cc_do;
							}
						}
						else
						{
							rear_slowdown &= ~cc_do;
							front_slowdown &= ~cc_do;
						}
					} 
					zone_fl = fl;
					if ((cons[otslbp] & 0x04) == 0)
					{
						if (fvars[fvrtldoortyp] == 0)
						{	// open front door only
							if (ccnmsk[cons[carnmb]][lobby_fl] != 0)
							{//do front by default
								set_carcall(cons[carnmb],lobby_fl);
							}
							if ((ccmsk[cons[carnmb]][position] == 0) && (cons[rear] == 1) && (rccnmsk[cons[carnmb]][lobby_fl] != 0)) 
							{//if front not available, try rear anyhow
								set_rcarcall(cons[carnmb],lobby_fl);
							}
						}
						else if (fvars[fvrtldoortyp] == 1)
						{	// open rear door only
							if ((cons[rear] == 1) && (rccnmsk[cons[carnmb]][lobby_fl] != 0)) 
								set_rcarcall(cons[carnmb],lobby_fl);
						}
						else if (fvars[fvrtldoortyp] == 2)
						{	// open both front and rear doors
							if (ccnmsk[cons[carnmb]][lobby_fl] != 0)
								set_carcall(cons[carnmb],lobby_fl);
							if ((cons[rear] == 1) && (rccnmsk[cons[carnmb]][lobby_fl] != 0)) 
								set_rcarcall(cons[carnmb],lobby_fl);
						}
					}
					servf = s_LBY_RCL;
				}
			}
			else if (servf == s_LBY_RCL)
			{
				servf = s_AUTO;
			}
		}
		else if (servf == s_LBY_RCL)
		{
			servf = s_AUTO;
		}
  	}
}

//****************************
// Car is Safe to move
//****************************
int16 safe(void)
{
	statusf &= ~sf_INS;

	if ((rdinp(i_FFS) == 0) && (rdinp(i_CSS) == 1) && (cons[firesv] == 2) && (cons[ansi] >= 2004))
	{
		statusf &= ~sf_STP;
		statusf |= sf_FSTP;
		timers[tins_flt] = 0;
		return 0;					// NOT Safe without fire stop switch
	}
	else
	{
		statusf &= ~sf_FSTP;
		if (((rdinp(i_CST) == 0) || (rdinp(i_MRSW) == 0)) && rdinp(i_FFS) == 1)
		{
			statusf |= sf_STP;
			return 0;		// not safe without stop switch or GTS
		}
		else
			statusf &= ~sf_STP;
	}

	if (ins_fault == 1)
	{	// previously had an inspectionn error
		if (timers[tins_flt] > 5)
      	{
			statusf |= sf_INS;
			if (ins_fault_latch == 0)
			{
				ins_fault_latch = 1;
			  	record_fault(f_ins_err);
			}
		}
	}
	else
	{
		ins_fault_latch = 0;
		timers[tins_flt] = 0;
	}

	ins_fault = 0;		// This flag will be set below before next time through
		
	if (byp_fault == 1)
	{	// previously had an inspectionn error
		if (timers[tbyp_flt] > 5)
      	{
			statusf |= sf_INS;
			if (byp_fault_latch == 0)
			{
				byp_fault_latch = 1;
			  	record_fault(f_byp_err);
			}
		}
	}
	else
	{
		byp_fault_latch = 0;
		timers[tbyp_flt] = 0;
	}

	byp_fault = 0;		// This flag will be set below before next time through

	if (rdinp(i_AUTO) == 1)
	{	  // ON AUTOMATIC
		if ((rdinp(i_CTIN) == 1) || (rdinp(i_MRIN) == 1) ||
				(rdinp(i_ACC) == 1) || (rdinp(i_ICI) == 1))
		{
			ins_fault = 1;
			return 0;		// not safe from input failure  
		}
		
		if ((rdinp(i_GBP) == 1) || (rdinp(i_LBP) == 1))
		{
			byp_fault = 1;
			return 0;
		} 
	
		if ((door_locks() == 1) && (car_gate() == 1))
			return 1;
		else
			return 0;
	}
	else if (rdinp(i_CTIN) == 1)
	{	// Top of Car Inspection
		if ( (rdinp(i_AUTO) == 1) || (rdinp(i_MRIN) == 1) ||
				(rdinp(i_ACC) == 1) || (rdinp(i_ICI) == 1) )
		{
			ins_fault = 1;
			return 0;		// not safe from input failure
		}

		if ((rdinp(i_LBP) == 1) && (rdinp(i_GBP) == 1))
		{		// Bypassing both the gate and lock
			return 1;
		}
		else if ((rdinp(i_LBP) == 1) && (rdinp(i_GBP) == 0)) 
		{	  // Locks are bypassed but not the gate
			if (car_gate() == 1)
				return 1;		// gates are locked
			else
				return 0;
		}
		else if ((rdinp(i_LBP) == 0) && (rdinp(i_GBP) == 1))
		{	  // Gates are bypassed but not the locks
			if (door_locks() == 1)
				return 1;
			else
				return 0;
		}
		else 	
		{	 // No Gate or Lock Bypass
			if ((door_locks() == 1) && (car_gate() == 1))
				return 1;
			else
				return 0;
		}
	}
	else if (rdinp(i_ACC) == 1)
	{		  // Access operation
		if ( (rdinp(i_AUTO) == 1) || (rdinp(i_MRIN) == 1) ||
				(rdinp(i_CTIN) == 1) || (rdinp(i_ICI) == 1) )
		{
			ins_fault = 1;
			return 0;		// not safe from input failure
		}

		if ((rdinp(i_GBP) == 1) || (rdinp(i_LBP) == 1))
		{
			byp_fault = 1;
			return 0;
		} 

		if ((rdinp(i_TAU) == 0) && (rdinp(i_TAD) == 0) &&
					(rdinp(i_BAU) == 0) && (rdinp(i_BAD) == 0))
			return 1;

		if (cons[topf] > 2)
		{
			if ((rdinp(i_DLM) == 0) || (rdinp(i_DLM_1) == 0))
				return 0;
				
			if (((cons[mand] != 0) || (cons[Australia] == 1)) && (rdinp(i_MDC) == 0))
				return 0;	  // if manual door then middle aux lock must be made

			if (cons[rear] == 1)
			{
				if ((rdinp(i_RLM) == 0) || (rdinp(i_RLM_1) == 0))
					return 0;		// middle door locks must always be	on
				if (((cons[mand] != 0) || (cons[Australia] == 1)) && (rdinp(i_MDCR) == 0))
					return 0;		 // if manual door then middle aux lock must be made
			}
		}				

		if (((rdinp(i_TAU) == 1) || (rdinp(i_TAD) == 1)) && 
			(rdinp(i_BAU) == 0) && (rdinp(i_BAD) == 0))
		{		// Top access
			if (((cons[access_type] & 1) != 0) && (cons[rear] != 0)) 
			{
				if ((rdinp(i_RGS) == 0) || (rdinp(i_RGS_1) == 0))
					return 0;	// front top access but rear gate is open
				if ((rdinp(i_RLM) == 0) || (rdinp(i_RLM_1) == 0))
					return 0;	// front top access but rear middle lock is open (even if only two floors)
				if (((cons[mand] != 0) || (cons[Australia] == 1)) && (rdinp(i_MDCR) == 0))
					return 0;
			}
			if ((cons[access_type] & 4) != 0)
			{
				if ((rdinp(i_GS) == 0) || (rdinp(i_GS_1) == 0))
					return 0; 	// rear top access but front gate is open
				if ((rdinp(i_DLM) == 0) || (rdinp(i_DLM_1) == 0))
					return 0;	// rear top access but middle door lock open (even if only two floors)
				if (((cons[mand] != 0) || (cons[Australia] == 1)) && (rdinp(i_MDC) == 0))
					return 0;
			}
			if (((cons[mand] != 0) || (cons[Australia] == 1)) && (rdinp(i_BDC) == 0))
				return 0;

			if ((rdinp(i_DLT) == 1) && (rdinp(i_DLT_1) == 1)) 
				return 0;	 // Must have top hall door open
			
			if (((rdinp(i_DLB) == 1) && (rdinp(i_DLB_1) == 1)) && ((fvars[fvaccdoorcl] == 0) || (car_gate() == 1)))
				return 1;	 // Must have the bottom door lock
			else
				return 0;
		}
		else if (((rdinp(i_BAU) == 1) || (rdinp(i_BAD) == 1)) && 
			(rdinp(i_TAU) == 0) && (rdinp(i_TAD) == 0))
		{		// Bottom Access
			if (((cons[access_type] & 2) != 0) && (cons[rear] != 0))
			{
				if ((rdinp(i_RGS) == 0) || (rdinp(i_RGS_1) == 0))
					return 0;	// front bottom access but rear gate is open
				if ((rdinp(i_RLM) == 0) || (rdinp(i_RLM_1) == 0))
					return 0;	// front bottom access but rear middle lock is open	(even if only two floors)
				if (((cons[mand] != 0) || (cons[Australia] == 1)) && (rdinp(i_MDCR) == 0))
					return 0;
			}
			if ((cons[access_type] & 8) != 0) 
			{
				if ((rdinp(i_GS) == 0) || (rdinp(i_GS_1) == 0))
					return 0; 	// rear bottom access but front gate is open
				if ((rdinp(i_DLM) == 0) || (rdinp(i_DLM_1) == 0))
					return 0;	// rear bottom access but middle door lock open	(even if only two floors)
				if (((cons[mand] != 0) || (cons[Australia] == 1)) && (rdinp(i_MDC) == 0))
					return 0;
			}
			if (((cons[mand] != 0) || (cons[Australia] == 1)) && (rdinp(i_TDC) == 0)) 
				return 0;

			if ((rdinp(i_DLB) == 1) && (rdinp(i_DLB_1) == 1))
				return 0;	 // Must have the bottom hall door open
			
			if (((rdinp(i_DLT) == 1) && (rdinp(i_DLT_1) == 1)) && ((fvars[fvaccdoorcl] == 0) || (car_gate() == 1)))
				return 1;	 // Must have the top door lock
			else
				return 0;
		}
	}
	else if (rdinp(i_ICI) == 1)
	{			// In-car Inspection
		if ( (rdinp(i_AUTO) == 1) || (rdinp(i_MRIN) == 1) ||
				(rdinp(i_CTIN) == 1) || (rdinp(i_ACC) == 1) )
		{
			ins_fault = 1;
			return 0;		// not safe from input failure
		}

		if ((rdinp(i_GBP) == 1) || (rdinp(i_LBP) == 1))
		{
			byp_fault = 1;
			return 0;
		} 

		if ((door_locks() == 1) && (car_gate() == 1))
			return 1;	  // Doors must be closed on in car inspection
		else
			return 0;
	}
	else if (rdinp(i_MRIN) == 1)
	{			// In-car Inspection
		if ( (rdinp(i_AUTO) == 1) || (rdinp(i_ICI) == 1) ||
				(rdinp(i_CTIN) == 1) || (rdinp(i_ACC) == 1) )
		{
			ins_fault = 1;
			return 0;		// not safe from input failure
		}

		if ((rdinp(i_GBP) == 1) || (rdinp(i_LBP) == 1))
		{
			byp_fault = 1;
			return 0;
		} 

		if ((door_locks() == 1) && (car_gate() == 1))
			return 1;	  // Doors must be closed on in car inspection
		else
			return 0;
	}

	ins_fault = 1;
	return 0;		// must have one input on
}

void set_safe_fault(void)
{
	if (door_locks() == 0)
	{
		if ((safe_fault_latch & 0x01) == 0)
			record_fault(f_dl_safe_flt);
		safe_fault_latch |= 0x01;
	}
	
	if (car_gate() == 0)
	{
		if ((safe_fault_latch & 0x02) == 0)
			record_fault(f_cg_safe_flt);
		safe_fault_latch |= 0x02;
	}

	if (manual_door != 0)
	{
		if (man_fdcc() == 0)
		{
			if ((safe_fault_latch & 0x04) == 0)
				record_fault(f_fdcc_safe_flt);
			safe_fault_latch |= 0x04;
		}
	}

	if (manual_rdoor != 0)
	{
		if ((man_rdcc() == 0) && (cons[rear] != 0))
		{
			if ((safe_fault_latch & 0x08) == 0)
				record_fault(f_rdcc_safe_flt);
			safe_fault_latch |= 0x08;
		}
	}
}



int16 access_door_safe(void)
{
	if (rdinp(i_ACC) == 1)
	{		  // Access operation
		if ( (rdinp(i_AUTO) == 1) || (rdinp(i_MRIN) == 1) ||
				(rdinp(i_CTIN) == 1) || (rdinp(i_ICI) == 1) )
			return 0;		// not safe from input failure

		if ((rdinp(i_GBP) == 1) || (rdinp(i_LBP) == 1))
			return 0;

		if ((rdinp(i_TAU) == 0) && (rdinp(i_TAD) == 0) &&
					(rdinp(i_BAU) == 0) && (rdinp(i_BAD) == 0))
			return 1;

		if (cons[topf] > 2)
		{
			if (((cons[mand] == 0)&&(cons[frghtd] == 0)) && ((rdinp(i_DLM) == 0) || (rdinp(i_DLM_1) == 0)))
 				return 0;						
				
			if (((cons[mand] != 0)||(cons[frghtd] != 0)) && (rdinp(i_MDC) == 0))
				return 0;	  // if manual door then middle aux lock must be made

			if (cons[rear] == 1)
			{
				if (((cons[mand] == 0)&&(cons[frghtd] == 0)) && ((rdinp(i_RLM) == 0) || (rdinp(i_RLM_1) == 0)))
					return 0;		// middle door locks must always be	on
				if (((cons[mand] != 0)||(cons[frghtd] != 0)) && (rdinp(i_MDCR) == 0))
					return 0;		 // if manual door then middle aux lock must be made
			}
		}				

		if (((rdinp(i_TAU) == 1) || (rdinp(i_TAD) == 1)) && 
			(rdinp(i_BAU) == 0) && (rdinp(i_BAD) == 0))
		{		// Top access
			if (((cons[access_type] & 1) != 0) && (cons[rear] != 0)) 
			{
				if ((rdinp(i_RGS) == 0) || (rdinp(i_RGS_1) == 0))
					return 0;	// front top access but rear gate is open
			}
			if ((cons[access_type] & 4) != 0)
			{
				if ((rdinp(i_GS) == 0) || (rdinp(i_GS_1) == 0))
					return 0; 	// rear top access but front gate is open
			}
			if (((cons[mand] != 0)||(cons[frghtd] != 0)) && (rdinp(i_BDC) == 0))
				return 0;

			if ((rdinp(i_DLB) == 1) && (rdinp(i_DLB_1) == 1))
				return 1;	 // Must have the bottom door lock
			else
				return 0;
		}
		else if (((rdinp(i_BAU) == 1) || (rdinp(i_BAD) == 1)) && 
			(rdinp(i_TAU) == 0) && (rdinp(i_TAD) == 0))
		{		// Bottom Access
			if (((cons[access_type] & 2) != 0) && (cons[rear] != 0))
			{
				if ((rdinp(i_RGS) == 0) || (rdinp(i_RGS_1) == 0))
					return 0;	// front bottom access but rear gate is open
			}
			if ((cons[access_type] & 8) != 0) 
			{
				if ((rdinp(i_GS) == 0) || (rdinp(i_GS_1) == 0))
					return 0; 	// rear bottom access but front gate is open
			}
			if (((cons[mand] != 0)||(cons[frghtd] != 0)) && (rdinp(i_TDC) == 0))
				return 0;

			if ((rdinp(i_DLT) == 1) && (rdinp(i_DLT_1) == 1))
				return 1;	 // Must have the top door lock
			else
				return 0;
		}
	}
	return 0;
}
//****************************
// Saftey String is Open Mode
//****************************
void sstring()
{
	int16 ss_fault_latch = 0;

	if ((enable_cc_test == 1) || (continue_cc_test == 1))
	{
		cancel_cc_test = 1;
		enable_cc_test = 0;
		continue_cc_test = 0;
	}

	Overspeed_Test = 0;
   	Buffer_Test = 0;
	NTS_Test = 0;
	ETS_Test = 0;
    NTS_spi.Command2.B.SET_UPSD = 0;		// clear the command on exit just to make sure
    NTS_spi.Command2.B.SET_DNSD = 0;		// clear the command on exit just to make sure
	startf = 0;
	lev_startf = 0;
   	timers[trst] = 0;
	timers[tpwl_pwr_off] = 0;
	rset_error_cnt = 0;		// made it out of reset

	trigger_arm_state |= SS_START;
	
   	while(procf == 12)
   	{

     	servf = s_SSTRING;
     	
		write_trace(TR_SSTRING1);
     	inctime(0);
		set_ul_dl_dz();		// sets up_level, dn_level and door_zone
		setfsf();		// set fire service flags
     	clrall();
     	doorf = 0;
    	if(safeties() == 1)
			timers[tsst] = 0;
    	else if (timers[tsst] > 10)
     	{
			if (ss_fault_latch == 0)
				set_ss_fault();
			ss_fault_latch = 1;

			if ((statusf & sf_SS) == 0)
		  		record_fault(f_sstring);
			statusf |= sf_SS;
     	}
		else
			statusf &= ~sf_SS;

		// Testing to see if LC fuse is blown
    	if(rdinp(i_S10) == 1)
			timers[tlct] = 0;
    	if((rdinp(i_S10) == 0) && (timers[tlct] > 10))
    	{
			if ((statusf & sf_S10) == 0)
			  	record_fault(f_S10_fuse);
			statusf |= sf_S10;
    	}
		else
			statusf &= ~sf_S10;

		write_trace(TR_SSTRING2);
		
		// Testing to see if saftey string is still open
     	if((safeties() == 1) && (rdinp(i_S10) == 1))
     	{
			statusf &= ~sf_SS;
			statusf &= ~sf_S10;

			// Testing to see if on inspection
		    if(chk_ins() == 1)
		    {
		      	procf = 2;        // go to inspection mode
		      	timers[tmotion] = 0;
				timers[tpatdly] = 0;
		      	return;
		    }
			procf = 0;       // go to reset mode
			timers[tins_flt] = 0;
     	}
   	}
	return;
}

//***************************
// Set Safety String Status
//***************************

//***************************
// Galaxy 4 Controller
// Set Safety String Status
//***************************

int16 safeties (void)
{
	safety_string_status = 0;
	if (rdinp(i_GOV) == 0)
		safety_string_status |= sss_GOV;
//	else if (rdinp(i_TF) == 0)
//		safety_string_status |= sss_TF;
//	else if (rdinp(i_BF) == 0)
//		safety_string_status |= sss_BF;
//	else if (rdinp(i_PS) == 0)
//		safety_string_status |= sss_PS;
	else if (rdinp(i_HSS) == 0)
		safety_string_status |= sss_HSS;
#if (Simulator == 0)
	else if ((rdinp(i_CSS) == 0) && (cop_can_online == 1) && (cop_can_error == 0) && (rdinp(i_MRIN) == 0) && (rdinp(i_CTIN) == 0))
		safety_string_status |= sss_CSS;
	else if ((rdinp(i_CTS) == 0) && (cop_can_online == 1) && (cop_can_error == 0) && (rdinp(i_MRIN) == 0) && (rdinp(i_CTIN) == 0))
		safety_string_status |= sss_CTS;
	else if ((rdinp(i_FFS) == 0) && (cop_can_online == 1) && (cop_can_error == 0) && (rdinp(i_MRIN) == 0) && (rdinp(i_CTIN) == 0))
		safety_string_status |= sss_FFS;
#endif	
//	else if (rdinp(i_FFS) == 0) 
//		safety_string_status |= sss_FFS;
	else if (rdinp(i_CST) == 0) 
		safety_string_status |= sss_CST;
	else if (rdinp(i_MRSW) == 0)
		safety_string_status |= sss_MRSW;
//	else if (rdinp(i_GTS) == 1)
//		safety_string_status |= sss_GTSW;

	return (safety_string_status == 0);			
}

//********************************
// Set Safety String Status	Fault
//********************************
void set_ss_fault (void)
{
	if (rdinp(i_GOV) == 0)
		record_fault(f_gov);
//	else if (rdinp(i_TF) == 0)
//		record_fault(f_top_final);
//	else if (rdinp(i_BF) == 0)
//		record_fault(f_bottom_final);
//	else if (rdinp(i_PS) == 0)
//		record_fault(f_pit_sw);
	else if (rdinp(i_HSS) == 0)
		record_fault(f_hatch_safety);
#if (Simulator == 0)
	else if ((rdinp(i_CSS) == 0) && (cop_can_online == 1) && (cop_can_error == 0))
		record_fault(f_car_safety_sw);
	else if ((rdinp(i_CTS) == 0) && (cop_can_online == 1) && (cop_can_error == 0))
		record_fault(f_Car_Top_Stop_Sw);
	else if ((rdinp(i_FFS) == 0) && (cop_can_online == 1) && (cop_can_error == 0))
		record_fault(f_FF_stop_sw);
#endif	
//	else if (rdinp(i_FFS) == 0)
//	  	record_fault(f_FF_stop_sw);
	else if (rdinp(i_MRSW) == 0)
		record_fault(f_Machine_Room_Stop_Sw);
}
// ****************************************
// Set the code blue flag
// ****************************************

void setcodebf (void)
{
	if (cons[cblue] == 1)
	{
		if (((car_Grp_Stat.c_cb_fl != 0) || (car_Grp_Stat.c_rcb_fl != 0)) && (hsvf == 0)  && (Group_Comm == 1))
		{
			servf = s_CBLUE;
			in_code_blue = 1;
			if (car_Grp_Stat.c_cb_fl != 0)
				codebfl = car_Grp_Stat.c_cb_fl;
			else 
				codebfl = car_Grp_Stat.c_rcb_fl;

			if (position != codebfl)
			{
				codebf = 1;		// want to get to the code blue floor
				if ((rdinp(i_EMS) == 1)&&(fvars[fvhsaftercb] == 0))
				{
					codebf = 4;
					doort = 0;
					rdoort = 0;
				}
				clrcc(0);
				clrhc();
				clrlant();
			}
			else if (position == codebfl)
			{			// at the code blue floor
				if ((procf < 3) || (procf > 11))
				{			   // car is not moving
					if (car_Grp_Stat.c_cb_fl != 0)	
					{		// Front door
						if ((doorf == 2) && (codebf == 2) && (doort != 0))
							codebf = 3;	  // front door is open
						else if ((codebf == 0) || (codebf == 1))
							codebf = 2;		// command to open the door and set the doort
						if (((codebf == 3) && (timers[tdwel] >= doort) && (doort != 0)) || 
							(rdinp(i_EMS) == 1))
						{
							codebf = 4;
							timers[tdwel] = 0;
//							doort = 0;
						}
						if ((fvars[fvcbsinglecall] == 1) && (codebf == 3) && (nmb_carcalls != 0))
						{	// at code blue floor and ready to go in hospital service
							codebf = 4;
							ems_onecall = 1;

						}
					}
					else
					{	   // Rear door
						if ((rdoorf == 2) && (codebf == 2) && (rdoort != 0))
							codebf = 3;		// rear door is open
						else if ((codebf == 0) || (codebf == 1))
							codebf = 2;		// command to open the door and set the doort
						if (((codebf == 3) && (timers[trdwel] >= rdoort) && (rdoort != 0)) ||
							(rdinp(i_EMS) == 1))
						{
							codebf = 4;
							timers[trdwel] = 0;
//							rdoort = 0;
						}
						if ((fvars[fvcbsinglecall] == 1) && (codebf == 3) && (nmb_carcalls != 0))
						{	// at code blue floor and ready to go in hospital service
							codebf = 4;
							ems_onecall = 1;
						}
					}
				}
			}
		}
		else
		{
			if (in_code_blue == 1)
			{
				in_code_blue = 0;
				doort = 0;
				rdoort = 0;
				if (servf == s_CBLUE)
					servf = s_AUTO;
				if (hsvf == 0)
					clroutp(o_EML);
				med_buz = 0;
			}
			codebf = 0;
			codebfl = 0;
		}
		if (((car_Grp_Stat.c_grp_codebf & 0x0001) != 0) && (in_code_blue == 0))
		{
			if (ind() == 1)
			{
				if((fvars[fvcbovrind] & 0x01) !=0)
				{
					cb_over_ind = 1;
				}
				ind_codeb_alarm = 1;
				if ((fvars[fvcblight] & 0x0001) != 0) 
				{
					if (timers[tsec] != 0)
					{
						setoutp(o_EML);
						med_buz = 1;
					}
					else
					{
						clroutp(o_EML);
						med_buz = 0;
					}
				}
				else
				{
					setoutp(o_EML);
					med_buz = 1;
				}
			}
		}
		else if (ind_codeb_alarm == 1)
		{
			ind_codeb_alarm = 0;
			if (hsvf == 0)
				clroutp(o_EML);
			med_buz = 0;
		}

		if(cb_over_ind == 1)
		{
			if(codebf == 3)
				cb_over_ind = 0;
		}
		else
		{
			timers[tcbovrind] = 0;
		}
		
		if (in_code_blue == 1)
		{		// code blue or hospital service
			if ((fvars[fvcblight] & 0x0001) != 0) 
			{
				if (timers[tsec] != 0)
				{
					setoutp(o_EML);
					if (ind() == 1)
						med_buz = 1;
				}
				else
				{
					clroutp(o_EML);
					med_buz = 0;
				}
			}
			else
			{
				setoutp(o_EML);
				if (ind() == 1)
					med_buz = 1;
				else
					med_buz = 0;
			}
		}
	}
	else
	{
		in_code_blue = 0;
		if (servf == s_CBLUE) 
		{
			servf = s_AUTO;
			med_buz = 0;
		}
		codebf = 0;
		codebfl = 0;
	}
	
	if((fvars[fvcbbuz] == 1)&&(ind_codeb_alarm == 0))
	{
		if((codebf == 1)||(codebf == 2))
			med_buz = 1;
		else
			med_buz = 0;
	}
	
}

//**************************************************************************
// Set hospital service flag (part of code blue operation ******************
//**************************************************************************

void sethsvf(void)
{
	if (cons[cblue] == 1)
	{
		if (((codebf == 0) || (codebf == 4)) && 
			((hsvf == 1) || ((firef == 0) || ((codebf == 4) && (fvars[fvcboverfs] == 1)))))
		{

			if ((fvars[fvhsaftercb] == 0) || (codebf == 4) || (hsvf == 1))
			{
				if((fvars[fvcbsinglecall] == 1) && (ems_onecall == 1) && (onward_cc != 0))
				{
					hsvf = 1;
				}
				else if (rdinp(i_EMS) == 1)
				{
					if (hsvf == 0)
					{
						clrcc(0);
						dpref = 0;
						clr_dir_arrow();
			    	}
					hsvf = 1;
				}
				else
				{
					hsvf = 0;
					ems_onecall = 0;
				}
			}
			else
			{
				ems_onecall = 0;
			}
			
			if (hsvf == 1)
			{
		    	servf = s_HSV;
				if ((fvars[fvcblight] & 0x0001) != 0) 
				{
					if (timers[tsec] != 0)
					{
						setoutp(o_EML);
						if (cons[GLBP_LED] == 0)
							setoutp(o_EMLH);
					}
					else
					{
						clroutp(o_EML);
						if (cons[GLBP_LED] == 0)
							clroutp(o_EMLH);
					}
				}
				else
				{
					setoutp(o_EML);
					if (cons[GLBP_LED] == 0)
						setoutp(o_EMLH);
				}
			}
			else 
			{
				if (ind_codeb_alarm == 0)
					clroutp(o_EML);
				if (cons[GLBP_LED] == 0)
					clroutp(o_EMLH);
			}
		}
		else
		{
			if (servf == s_HSV)
				servf = s_AUTO;
			hsvf = 0;
		}
	}
	else
	{
		if (servf == s_HSV)
			servf = s_AUTO;
		hsvf = 0;
	}
}

// *******************************************
// This is set Earthquake flags procedure
// *******************************************

void seteqf(void)
{

  	if(cons[equake] != 0)
  	{
  		if ((eqf != 0) || ((rdinp(i_ETH) == 1) && (rdinp(i_S10) == 1)))
  		{

			setoutp(o_EQL);
			setoutp(o_EQLC);

			if (eqf == 0)
			{
				eqf = 1;			// Stop at next landing
			}
			else
			{
				if ((eqf == 1) && ((procf < 3) || (procf > 11)) && (up_level == 1) && (dn_level == 1))
					eqf = 4; 			// dead level at a floor and not moving
			}						

			if ((rdinp(i_EQR) == 1) && (rdinp(i_ETH) == 0) && (rdinp(i_S10) == 1))
				eqf = 0;

			if ((eqf == 2) || (eqf == 3))		// invalid modes for a hydro
				eqf = 1;

			if (eqf == 4)	// prevents the car call from being set on fire service
				clrcc(0);

			servf = s_EAQ;		 
			
		}
		else 
		{
			eq_pos_reset = 0;
			eqf = 0;
			clroutp(o_EQL);
			clroutp(o_EQLC);
			if (servf == s_EAQ)
				servf = s_AUTO;
		}
	}
	else
	{
		eq_pos_reset = 0;
		eqf = 0;
		if (servf == s_EAQ)
			servf = s_AUTO;
	}
}

// *******************************************
// This is set Emergency Power flags procedure
// *******************************************

void setempf()
{
	// Hydro Car
	if ((cons[EMPop] & 0x02) != 0)
	{
	  	if((rdinp(i_EMP) == 1) || ((rdinp(i_EPT) == 1) && ((cons[EMPop] & 0x01) != 0)) || ((car_Grp_Stat.c_gempf != no_ep) && (Group_Comm == 1)))
	  	{				// on emergency power
			hydro_on_emp = 1; 
			if ((rdinp(i_EPT) == 1) && ((cons[EMPop] & 0x01) != 0))	   // Must stop the car
			{
		   		if ((timers[tdwel] >= fvars[fvhcdt]) && (procf >= 14) && (procf <= 16))	// Door will open from the door routine, cloose door after dwell time
		   			empf = ep_home_close;
				else
					empf = ep_wait_door;
				if ((empf == ep_home_close) && ((doorf == 1) || (rdoorf == 1)))
					empf = ep_wait_door;					// Dwell door on a reopen
			}
			else if (Group_Comm == 1)
			{
				if (car_Grp_Stat.c_gempf == ep_recall)
				{
		 			if ((medf == 1) || (medf == 2))
		 				ep_recall_fl = fvars[fvmedfl];
		 			else if(firef == 1)
		 				ep_recall_fl = fvars[fvfs1f];
		 			else if (firef == 2)
		 				ep_recall_fl = fvars[fvfs1af];
		 			else if (firef == 3)
		 				ep_recall_fl = position;	// current floor
		 			else
		 				ep_recall_fl = fvars[fvepfl];
		 
		 			if (((position == ep_recall_fl) && (procf >= 14) && (procf <= 16)) &&	 // AT the return floor
						((servf > s_LOW_OIL) && (servf != s_IND) && (servf != s_ATT) && (servf != s_FIRE2) && (servf != s_EAQ) &&  // ots, ind, att, f2, eq
						 (servf != s_STALLED) && (servf != s_LBY_RCL) && (servf != s_ELEV_OFF_CS) && (servf != s_ELEV_OFF_HS) && (servf != s_LOW_PRES)) ) // stall, rtl, cse, hse, lowPres
		 				empf = ep_home;
		 			else
		 				empf = ep_recall;
				}
				else if ((car_Grp_Stat.c_gempf == ep_select) || (car_Grp_Stat.c_gempf == ep_switch))
					empf = ep_select;	// car selected to run from manual switch or after auto recall
				else if ((car_Grp_Stat.c_gempf == ep_wait) || (car_Grp_Stat.c_gempf == no_ep))
					empf = ep_wait;		// Wait	for the group to direct the car
				else if (car_Grp_Stat.c_gempf == ep_home)
				{
			   		if (timers[tdwel] >= fvars[fvhcdt])	// Door will open from the door routine, cloose door after dwell time
			   			empf = ep_home_close;
					else
						empf = ep_wait_door;
					if ((empf == ep_home_close) && ((doorf == 1) || (rdoorf == 1)))
						empf = ep_wait_door;					// Dwell door on a reopen
				}
				else   
				{
					if ((empf == ep_wait) || (empf == epc_wait_close))
					{
						if ((doorf == 1) || (rdoorf == 1))
							empf = ep_wait_door;					// Dwell door on a reopen

						if ((doorf == 0) && (rdoorf == 0))
							empf = ep_wait;
					}
					else if (empf == ep_wait_door)
					{
						if (timers[tdwel] >= fvars[fvhcdt])	// Door will open from the door routine, cloose door after dwell time
			   				empf = epc_wait_close;
					}
					else
						empf = ep_wait;
						
				}

			}
			else if (rdinp(i_EPS) == 1)
				empf = ep_select;		// car manually selected and no group comm
			else 
			{
		   		if (timers[tdwel] >= fvars[fvhcdt])	// Door will open from the door routine, cloose door after dwell time
		   			empf = ep_home_close;
				else
					empf = ep_wait_door;
				if ((empf == ep_home_close) && ((doorf == 1) || (rdoorf == 1)))
					empf = ep_wait_door;					// Dwell door on a reopen
			}
	  	}
		else
		{
			hydro_on_emp = 0; 
			empf = no_ep;	// no longer on emergency power
			ep_recall_fl = fvars[fvepfl];
		}
	}
	else
	{
	  	if(rdinp(i_EMP) == 1)
	  	{				// on emergency power
			hydro_on_emp = 1; 
			if (empf == 0)
				clrcc(0);		// clear car calls on first entry (This will cancel the calls on a Phase 2 car)
			if (car_Grp_Stat.c_gempf == ep_recall)
			{
	 			if(((medf == 1) || (medf == 2)) && ((position >= fvars[fvmedfl]) || (scnd_pos >= fvars[fvmedfl])))
	 			{
	 				ep_recall_fl = fvars[fvmedfl];
	 			}
				else if((firef == 1) && ((position > fvars[fvfs1f]) || (scnd_pos >= fvars[fvfs1f])))
				{
					ep_recall_fl = fvars[fvfs1f];
				}
				else if((firef == 2) && ((position > fvars[fvfs1af]) || (scnd_pos >= fvars[fvfs1af])))
				{
					ep_recall_fl = fvars[fvfs1af];
				}
				else if (firef == 3)
				{
					ep_recall_fl = position;	// current floor
				}
				else
				{
					ep_recall_fl = cons[bottomf];		// Set floor to bottom
				}

				if ((((position == ep_recall_fl) || (rdinp(i_DTsel) == 0)) && (procf >= 14) && (procf <= 16)) && // AT the return floor
					((ind() == 0) && ((rdinp(i_ATT) == 0) || (cons[att] == 0)) && (stallf == 0))) // Can run on emp
						empf = ep_home;
				else
					empf = ep_recall;
		   	}
		   	else if ((car_Grp_Stat.c_gempf == ep_select) || (car_Grp_Stat.c_gempf == ep_switch))
		   		empf = ep_select;	// car selected to run from manual switch or after auto recall
		   	else if ((empf == ep_home) || (empf == ep_home_close)) // allow door to close after dwell time-out
		   	{
		   		if (timers[tdwel] >= fvars[fvhcdt])	// Door will open from the door routine, cloose door after dwell time
		   			empf = ep_home_close;
				if ((empf == ep_home_close) && ((doorf == 1) || (rdoorf == 1)))
					empf = ep_home;					// Dwell door on a reopen
		   	}
	  	}
		else
		{
			hydro_on_emp = 0; 
			empf = no_ep;	// no longer on emergency power
			ep_recall_fl = fvars[fvepfl];
		}
	}

	prev_hydro_on_emp = hydro_on_emp;


	if ((empf >= ep_wait) && (empf != ep_select))
	{
		if (((servf > s_LOW_OIL) && (ep_recall_ok() == 1)) ||
			((in_emp_trying_to_reset == 1)&&(ep_recall_ok() == 1)))
		{
			if (empf == ep_recall)	// Recall Car 
				servf = s_EPR;		// Emergency power reecall
			else
				servf = s_EMP;		// Only show emergency power if automatic
		}
	}
	else if (servf == s_EMP)
		servf = s_AUTO;			// first time out of emergency power

}

// *********************************************
// This is set the fire service flags procedure
// *********************************************
void setfsf()
{

	static int16 off_FSL_state;
	static int16 dbnc;

  	int16 A_Door_Is_Open = 0;
	int16 fire_sw_loc = 0;
	Extinguish_FSL = 0;


	if ((cons[smoke_interface] & 0x01) != 0)
		smoke_sensor_interface();
	
	if(off_FSL_state < 5)
		dbnc = 0;

	switch(off_FSL_state)
	{
		case 0://firef and stallf = 0
			if ((stallf != 0) || (hotoilf != 0) || (empf != 0)) 
				off_FSL_state = 1;
			if(firef != 0)
				off_FSL_state = 2;
			break;
		case 1: // already LOS condition. Wait for firef to change
			if (firef != 0)		
				off_FSL_state = 3;
			break;
		case 2:
			if (firef == 3)		// successful fs recall		
				off_FSL_state = 4;
			if ((stallf != 0) || (hotoilf != 0) || (empf != 0))
				off_FSL_state = 5;
			break;
		case 3:
			//if(firef == 3)		// successful fs recall		
			//	off_FSL_state = 6;	// extingish anyway	after tssa request
			if ((stallf == 4) || (hotoilf == 4) || (empf >3))		//finished Low Oil recall
				off_FSL_state = 7;
			break;
		case 4:
			if ((stallf != 0) || (hotoilf != 0) || (empf > 3))
				off_FSL_state = 6;
			break;
		case 5:
			if(firef == 3)		// successful fs recall		
				off_FSL_state = 7; // it was  off_FSL_state = 6 but iut was changed based on 3.27.2
			else if ((stallf > 2) || (hotoilf > 2) || (empf> 3))	//finished Low Oil recall
			{
				if(dbnc > 20)
					off_FSL_state = 7;
				dbnc++;
			}
			break;
		case 6: // Let it Flash and Check position to see if it is possible to run
//			if(position == cons[bottomf])	   //removed to allow blinking light even when car is at
//				off_FSL_state = 7;			   // the bottom floor	
			break;
		case 7:	//Extinguish the output
			break;
		default:
			break;
	}
	if ((firef == 0) && (stallf == 0) && (hotoilf == 0) && (empf == 0))
	{
		Extinguish_FSL = 0;
		off_FSL_state = 0;
	}
	else if (off_FSL_state == 7)
		Extinguish_FSL = 1;


	if((firef == 0) || (firef == 1) || (firef == 2)) // 2007 rule only applies for when firef == 3 so clear state for variable
	{
		fire_stopsw_hold = 0;
	}


	// Fires phase 2 switch logic

	if (cons[Australia] == 1)
	{
		fs2_on = (rdinp(i_FS2) == 0) || (rdinp(i_FS2H) == 1);
		fs2_hold = 0;
		fs2_start = (rdinp(i_FS2H) == 1);
	}
	else
	{
		if (cons[FS2_SW] == 1)	// New logic FS2 = NOT FS2 and NOT FS2H, HOLD = FS2H, OFF = FS2
			fs2_on = (rdinp(i_FS2) == 0) && (rdinp(i_FS2H) == 0);
		else					// Original logic FS2 = FS2 input, HOLD = FS2H, OFF = NOT FS2 and NOT FS2H
			fs2_on = (rdinp(i_FS2) == 1);
		fs2_hold = (rdinp(i_FS2H) == 1);
		fs2_start =0;
	}
		
		
	if (fs2_on == 1)
		fire_status |= fs_FS2;
	else
		fire_status &= fs_FS2;
	
	if (fs2_hold == 1)
		fire_status |= fs_FS2H;
	else
		fire_status &= fs_FS2H;
	
	if (fs2_no_fdoor_op == 1)
	{
		if ((rdinp(i_DOL) == 0) && (rdinp(i_DCL) == 1))	// doors are open so clear fs2_no_fdoor_op 
		{
			doorf = 2;
			firef = 6;
			fs2_no_fdoor_op = 0;
			return;			
		}
		if ((rdinp(i_DOL) == 1) && (rdinp(i_DCL) == 0))	// doors are closed so clear fs2_no_fdoor_op
		{
			doorf = 0;
			firef = 5;
			fs2_no_fdoor_op = 0;
			return;			
		}

		// no front door operation until either DOB or DCB is pressed
		if ((rd_both_inps(i_DCB,i_XDCB) == 1) || (rd_both_inps(i_DOB,i_XDOB) == 1))
		{
			fs2_no_fdoor_op = 0;
			if (rd_both_inps(i_DCB,i_XDCB) == 1) 
			{
				if(rdinp(i_DCL) == 1)
					doorf = 3;
				firef = 6; // constant pressure DC
			}
			else if (rd_both_inps(i_DOB,i_XDOB) == 1)
			{
				if(rdinp(i_DOL) == 0)
					doorf = 2;
				else
					doorf = 1;
				
				if(rdinp(i_FS2H) == 1) //fs2H, pwr dn, drift doors half way, pwr on, dob - > back to hold
					firef = 4;
				else
					firef = 5;
			}
			return;
		}				 
	}
	
	if (cons[rear] != 0)
	{
		if (fs2_no_rdoor_op == 1)		
		{
			if ((rdinp(i_DOLR) == 0) && (rdinp(i_DCLR) == 1))	// doors are open so clear fs2_no_rdoor_op 
			{
				rdoorf = 2;
				rfiref = 6;
				fs2_no_rdoor_op = 0;
				return;			
			}
			if ((rdinp(i_DOLR) == 1) && (rdinp(i_DCLR) == 0))	// doors are closed so clear fs2_no_rdoor_op
			{
				rdoorf = 0;
				rfiref = 5;
				fs2_no_rdoor_op = 0;
				return;			
			}

			// no front door operation until either DOBR or DCBR is pressed
			if ((rd_both_inps(i_DCBR,i_XDCBR) == 1) || (rd_both_inps(i_DOBR,i_XDOBR) == 1))
			{
				fs2_no_rdoor_op = 0;
				if (rd_both_inps(i_DCBR,i_XDCBR) == 1) 
				{
					if (rdinp(i_DCLR) == 1)
						rdoorf = 3;
					rfiref = 6; // constant pressure DC
				}
				else if (rd_both_inps(i_DOBR,i_XDOBR) == 1)
				{
					if (rdinp(i_DOLR) == 0)
						rdoorf = 2;
					else
						rdoorf = 1;
					
					if(rdinp(i_FS2H) == 1) //fs2H, pwr dn, drift doors half way, pwr on, dob - > back to hold
						rfiref = 4;
					else
						rfiref = 5;
				}
				return;
			}
		}
	}


	

	// read the hall fire inputs
	if (cons[Australia] == 0)
	{
		if (rdinp(i_MES) == 1)
			mes_on = 0;
		else
			mes_on = 1;

		if (rdinp(i_ALT) == 1)
			alt_on = 0;
		else
			alt_on = 1;
		
		if (cons[fire_smoke_nc] == 1)
		{
			if (rdinp(i_MRS) == 1)
				mrs_on = 0;
			else
				mrs_on = 1;

			if (rdinp(i_HWS) == 1)
				hws_on = 0;
			else
	      		hws_on = 1;

			if (cons[Smoke_2004] != 0)
			{
				if (rdinp(i_HWS2) == 1) 
					hws2_on = 0;
				else
					hws2_on = 1;
			}
			else
				hws2_on = 0;
		}
		else
		{
			if (rdinp(i_MRS) == 1)
				mrs_on = 1;
			else
				mrs_on = 0;

			if (rdinp(i_HWS) == 1)
				hws_on = 1;
			else
	      		hws_on = 0;

			if (cons[Smoke_2004] != 0)
			{
				if (rdinp(i_HWS2) == 1) 
					hws2_on = 1;
				else
					hws2_on = 0;
			}
			else
				hws2_on = 0;
		}

		if (rdinp(i_BP) == 1)
			fs1_reset_on = 1;
		else
			fs1_reset_on = 0;
    	
	}
	else
	{
		mes_on = 0;
		alt_on = 0;
		mrs_on = 0;
		hws_on = 0;
		hws2_on = 0;
		fs1_reset_on = 0;
	}

	if (((cons[firesv] == 2) && (fvars[fvfsx] == 1)) || (cons[FSX_rear] == 1) || (cons[firesv] == 3)) 
    {
    	if ((rdinp(i_FSX) == 1) || (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_fsx == 1)))
			fsx_on = 1;
		else
			fsx_on = 0;
	}
	else 
		fsx_on = 0;
	
	if (rdinp(i_FS) == 1) 
		fs_on = 1;
	else
		fs_on = 0;

  	if(firef == 0)
  	{
		fire_reset = 0;
		fire_sensor = 0;
    	fs1_effect = 0;
    	Blink_FSL = 0;
		Extinguish_FSL = 0;
		fire_status &= 0xFF00;

    	if( (mes_on == 0) && (alt_on == 0) &&
			(fs_on == 0) && (hws_on == 0) && ((hws2_on == 0) || (cons[Smoke_2004] == 0)) &&
			((fsx_on == 0) || (((cons[firesv] != 2) || (fvars[fvfsx] == 0)) && (cons[FSX_rear] == 0) && (cons[firesv] != 3))) &&
			(mrs_on == 0) )
	    	timers[tfs1t] = 0;

		if( (cons[Smoke_2004] == 1) && ((car_Grp_Stat.c_grp_fire_ctrl & 0x1C) != 0) && (Group_Comm == 1) && 
			((fs1_reset_on == 0) || (cons[firesv] == 2) || (cons[firesv] == 3)))
		{		   // Another car has smoke in the hoistway
	    	fs1_effect = 1;
	    	firef = 1;                // smoke detector any floor
	    	rfiref = 1;
			fire_sensor = 1;
			fire_status |= fs_ANY_FI;
		}

	    if( (mes_on == 1) && ((fs1_reset_on == 0) || (cons[firesv] == 2) || (cons[firesv] == 3)) &&
			((rdinp(i_FEP) == 1) || ((rdinp(i_S10) == 1) && (cons[HCP_Fuse] == 1))) && 
			(timers[tfs1t] > 20) )		 // ansi 2000 activates sensor even if reset on
	    {															 // chigaco has no reset or bypass switch
	    	fs1_effect = 1;
	    	firef = 1;                // smoke detector any floor
	    	rfiref = 1;
			fire_sensor = 1;
			fire_status |= fs_MES;
	    }
		//smoke detector in elevator hoistway
	    if( (hws_on == 1) && ((fs1_reset_on == 0) || (cons[firesv] == 2) || (cons[firesv] == 3)) &&
			((rdinp(i_FEP) == 1) || ((rdinp(i_S10) == 1) && (cons[HCP_Fuse] == 1))) && 
			(timers[tfs1t] > 20) )
	    {															  // ansi 2000 activates sensor even if reset on
	    	fs1_effect = 1;											  // chigaco has no reset or bypass switch
			if (fvars[fvfhwsen] == 1)
			{
	    		firef = 2;                // smoke detector in hoistway	go to alternate landing
	    		rfiref = 2;
			}
			else
			{
	    		firef = 1;                // smoke detector in hoistway
	    		rfiref = 1;
	    	}
	    	Blink_FSL = 1;
			fire_sensor = 1;
			fire_status |= fs_HWS;
	    }
	    if( ((cons[Smoke_2004] != 0) && (hws2_on == 1)) && ((fs1_reset_on == 0) || (cons[firesv] == 2) || (cons[firesv] == 3)) &&
			((rdinp(i_FEP) == 1) || ((rdinp(i_S10) == 1) && (cons[HCP_Fuse] == 1))) && 
			(timers[tfs1t] > 20) )
	    {															  // ansi 2000 activates sensor even if reset on
	    	fs1_effect = 1;											  // chigaco has no reset or bypass switch
			if (fvars[fvfhws2en] == 1)
			{
	    		firef = 2;                // smoke detector in hoistway	go to alternate landing
	    		rfiref = 2;
			}
			else
			{
	    		firef = 1;                // smoke detector in hoistway
	    		rfiref = 1;
	    	}
			if(fvars[fvfhws2loc] == 0)
	    		Blink_FSL = 1;
			fire_sensor = 1;
			fire_status |= fs_HWS2;
	    }
	    if( (mrs_on == 1) && ((fs1_reset_on == 0) || (cons[firesv] == 2) || (cons[firesv] == 3)) &&
			((rdinp(i_FEP) == 1) || ((rdinp(i_S10) == 1) && (cons[HCP_Fuse] == 1))) && 
			(timers[tfs1t] > 20) )			  // ansi 2000 activates sensor even if reset on
	    {															  // chigaco has no reset or bypass switch
	    	fs1_effect = 1;
			if (fvars[fvfmrsen] == 1)
			{
	    		firef = 2;                // smoke detector in the machine room go to alternate landing
	    		rfiref = 2;
			}
			else
			{
	    		firef = 1;                // smoke detector in the machine room
	    		rfiref = 1;
			}
	    	Blink_FSL = 1;
			fire_sensor = 1;
			fire_status |= fs_MRS;
	    }
	    if( (alt_on == 1) && ((fs1_reset_on == 0) || (cons[firesv] == 2) || (cons[firesv] == 3)) &&
			((rdinp(i_FEP) == 1) || ((rdinp(i_S10) == 1) && (cons[HCP_Fuse] == 1))) && 
			(timers[tfs1t] > 20) )			  // ansi 2000 activates sensor even if reset on
	    {															  // chigaco has no reset or bypass switch
	    	fs1_effect = 1;
	    	firef = 2;                // smoke detector at lobby
	    	rfiref = 2;
			fire_sensor = 2;
			fire_status |= fs_ALT;
	    }
		

    	if ( (fs_on == 1) && (timers[tfs1t] > 20) )
    	{
      		fs1_effect = 1;           // fire service switch in lobby
      		firef = 1;
      		rfiref = 1;
			fire_status |= fs_FS;
    	}
    	
    	if ( (fsx_on == 1) && 
    			(((cons[firesv] == 2) && (fvars[fvfsx] == 1)) || (cons[FSX_rear] == 1) || (cons[firesv] == 3)) && 
    			(timers[tfs1t] > 20) )
    	{
      		fs1_effect = 1;           // fire service switch in lobby
      		firef = 1;
      		rfiref = 1;
			fire_status |= fs_FSX;
    	}
    	return;
  	}
	else
	{
		if (mes_on == 0)
			fire_status &= ~fs_MES;

		if (alt_on == 0)
			fire_status &= ~fs_ALT;

		if (mrs_on == 0)
			fire_status &= ~fs_MRS;
		else
      		Blink_FSL = 1;

		if (hws_on == 0)
			fire_status &= ~fs_HWS;
		else
      		Blink_FSL = 1;

		if (cons[Smoke_2004] != 0)
		{
			if (hws2_on == 0) 
				fire_status &= ~fs_HWS2;
			else if (fvars[fvfhws2loc] == 0) // Blink the FSL if HW sensor is located in the same HW
	      		Blink_FSL = 1;
		}
		else
			fire_status &= ~fs_HWS2;

    	
		if (((cons[firesv] == 2) && (fvars[fvfsx] == 1)) || (cons[FSX_rear] == 1) || (cons[firesv] == 3)) 
    	{
    		if (fs_on == 1) 
				fire_status |= fs_FS;
			else
				fire_status &= ~fs_FS;
			
    		if (fsx_on == 1)
				fire_status |= fs_FSX;
			else
				fire_status &= ~fs_FSX;
		}
		else 
		{
		
			fire_status &= ~fs_FSX;

			if (fs_on == 1) 
				fire_status |= fs_FS;
			else
				fire_status &= ~fs_FS;
		}
	}

  	if((firef != 0) || (rfiref != 0))
  	{
		// Fire service phase 1 reset

		if (cons[firesv] == 0)
		{	   // ansi 1996
			if (((fs1_reset_on == 1) && (fs_on == 0)) ||    // Smoke detector by pass
				((fire_sensor == 0) && (fs_on == 0)))
				fire_reset = 1;
			else if ((mes_on == 1) || (alt_on == 1) ||
				(fs_on == 1) || (hws_on == 1) || 
				((hws2_on == 1) && (cons[Smoke_2004] != 0)) ||
				(mrs_on == 1)) 
				fire_reset = 0;					 // fire device still active
			if ((fire_reset == 1) && (firef <= 3))
			{
				if ((firef == 3) || ((fs2_to_fs1 == 0) && ((fvars[fvfire_rst] & 0x01) != 0)) 
					|| ((servf > s_GATE_LOCK) && (servf != s_FIRE2) && (servf != s_F1MAIN) && (servf != s_F1ALT) && (servf != s_RESET_UP) && (servf != s_RESET_DN)))
				{
					fs1_effect = 0;
					fire_reset = 0;
			  		rfiref = 0;
			  		firef = 0;
					fire_buz = 0;
					fire_status &= 0xFF00;
			  		clroutp(o_FL);      // Turn off the fire service light
		  		}
			}
		}
	  	else if ((cons[firesv] == 1) || (cons[firesv] == 3))   // New York or Chicago Fire service
	  	{
	      	if ((fs_on == 0)&&(mes_on == 0)&&
		 		(alt_on == 0) && (hws_on == 0) &&	 
		 		((hws2_on == 0) || (cons[Smoke_2004] == 0)) &&
		 		(mrs_on == 0) && ((fsx_on == 0) || ((cons[FSX_rear] == 0) && (cons[firesv] != 3))) )
			{				  // all fire devices are off
				if ((fire_sensor == 0) || ((firef == 3) && (fire_sensor != 0)))
					fire_reset = 1;	   // activated from the fs switch or (activated from sensor but car has returned)
			}
			else  
				fire_reset = 0;				  // fire device still active
			if ((fire_reset == 1) && (firef <= 3))
		  	{
				if ((firef == 3) || ((fs2_to_fs1 == 0) && ((fvars[fvfire_rst] & 0x01) != 0)) 
					|| ((servf > s_GATE_LOCK) && (servf != s_FIRE2) && (servf != s_F1MAIN) && (servf != s_F1ALT) && (servf != s_RESET_UP) && (servf != s_RESET_DN)))
				{
					fs1_effect = 0;
					fire_reset = 0;
			  		rfiref = 0;
			  		firef = 0;
					fire_buz = 0;
					fire_status &= 0xFF00;
			  		clroutp(o_FL);      // Turn off the fire service light
		  		}
			}
	  	}
	  	else  // else ansi 2000
	  	{
		    if ((fs1_reset_on == 1) && (mrs_on == 0) && (cons[Australia] == 0) &&
			 	(mes_on == 0) && (alt_on == 0) &&
			 	(fs_on == 0) && ((fsx_on == 0) || (fvars[fvfsx] == 0)) && 
			 	(hws_on == 0) && ((hws2_on == 0) || (cons[Smoke_2004] == 0)) &&
			 	((cons[Smoke_2004] != 1) || ((car_Grp_Stat.c_grp_fire_ctrl & 0x1C) == 0)) )    // Fire reset switch
				fire_reset = 1;
			else if ((cons[FSautoRST] == 1) && (fire_reset == 0) && (mrs_on == 0) &&
			 	(mes_on == 0) && (alt_on == 0) &&
			 	(fs_on == 0) && ((fsx_on == 0) || (fvars[fvfsx] == 0)) && 
			 	(hws_on == 0) && ((hws2_on == 0) || (cons[Smoke_2004] == 0)) &&
			 	((cons[Smoke_2004] != 1) || ((car_Grp_Stat.c_grp_fire_ctrl & 0x1C) == 0)) )    // Fire reset switch
				fire_reset = 1;
			else if ((mes_on == 1) || (alt_on == 1) || 
				(fs_on == 1) || ((fsx_on == 1) && 
				(fvars[fvfsx] == 1)) || (hws_on == 1) ||
				(mrs_on == 1) || ((hws2_on == 1) && (cons[Smoke_2004] != 0)) ||
				((cons[Smoke_2004] == 1) && ((car_Grp_Stat.c_grp_fire_ctrl & 0x1C) != 0)) ||
				((firef != 3) && ((fvars[fvfire_rst_2] & 0x01) != 0))) 
				fire_reset = 0;
		    else if (((fire_reset == 1) || (cons[Australia] == 1) || ((fire_sensor == 0) && ((fvars[fvfire_rst] & 0x02) != 0))) && (firef <= 3))
		    {		 // reset then off  or sensor was not set and allowed to reset (FS only) from OFF position
				if ((firef == 3) || ((fs2_to_fs1 == 0) && ((fvars[fvfire_rst] & 0x01) != 0)) 
					|| ((servf > s_GATE_LOCK) && (servf != s_FIRE2) && (servf != s_F1MAIN) && (servf != s_F1ALT) && (servf != s_RESET_UP) && (servf != s_RESET_DN)))
				{
					if ((fs_on == 0) && ((fsx_on == 0) || (fvars[fvfsx] == 0)))
					{
						fs1_effect = 0;
						fire_reset = 0;
						rfiref = 0;
						firef = 0;
						fire_buz = 0;
						fire_status &= 0xFF00;
						clroutp(o_FL);      // Turn off the fire service light
					}
				}
		    }
	  	}

		if (firef > 3)
		{		 // Determine which floor the phase 2 car sees as the fire_floor
    		if (((mes_on == 1) || (alt_on == 1) || 
				(fs_on == 1) || (hws_on == 1) || 
				((hws2_on == 1) && (cons[Smoke_2004] != 0)) ||
				((fsx_on == 1) && (((cons[firesv] == 2) && (fvars[fvfsx] == 1)) || (cons[FSX_rear] == 1) || (cons[firesv] == 3))) ||
				(mrs_on == 1)) && ((fs1_reset_on == 0) || (cons[firesv] == 2) || (cons[firesv] == 3))) 
			{
				fs1_effect = 1;
				if (cons[firesv] == 2)
				{
					if ((fs_on == 1) && ((fsx_on == 1) || (fvars[fvfsx] == 0)))
						fire_floor = fvars[fvfs1f];
					else if ((fire_sensor == 2) && (alt_on == 1) && 
						(fs_on == 0) && ((fsx_on == 0) || (fvars[fvfsx] == 0)))
						fire_floor = fvars[fvfs1af];
				}
				else if ((fs_on == 1) || ((fsx_on == 1) && ((cons[FSX_rear] == 1) || (cons[firesv] == 3))))
					fire_floor = fvars[fvfs1f];
				else if (cons[firesv] == 3)
				{			 // return to active fire floor
					if (alt_on == 1)
						fire_floor = fvars[fvfs1af];
					else if ((mes_on == 1) || (hws_on == 1) || 
						((hws2_on == 1) && (cons[Smoke_2004] != 0)) || (mrs_on == 1))
						fire_floor = fvars[fvfs1f];
				}
			}
			else if (cons[firesv] == 3)
				fs1_effect = 0;		  // chicago fire service (no fire input is on)
			else if (cons[firesv] == 2)
			{
				if ((fire_sensor != 0) || ((fvars[fvfire_rst] & 0x02) == 0)) 
				{				// Sensors were activated or must always see the reset input
					if ((fs1_reset_on == 1) && (cons[firesv] == 2))
						fs1_effect_reset = 1;
					else if ((fs1_reset_on == 0) && (fs1_effect_reset == 1))
					{
						fs1_effect = 0;
						fs1_effect_reset = 0;
					}
					else
						fs1_effect_reset = 0;
				}
				else if ((fs_on == 0) && ((fsx_on == 0)	|| (fvars[fvfsx] == 0)))
				{		// Allow reset for FS key only recall when key switch is off
					fs1_effect = 0;
					fs1_effect_reset = 0;
				}
			}
			else if ((fire_sensor == 0) && (fs_on == 0) && (cons[firesv] != 2))
				fs1_effect = 0;		// new york or ansi 96 but not ansi 2000
			else if ((fs1_reset_on == 1) && (cons[firesv] == 0))
				fs1_effect = 0;		// ansi 96 only
		}
    	if(firef == 1)       // fire service phase 1 main lobby return
    	{
			if ((empf != 0) && (empf != ep_select) && (empf != ep_recall) && (fvars[fvflemp] == 1))
	      		clroutp(o_FL);                // Turn off Fire service light
			else if (((eqf != 0) || (stallf != 0)) && (fvars[fvflots] == 1))
	      		clroutp(o_FL);                // Turn off Fire service light
			else if (Extinguish_FSL == 1)
	      		clroutp(o_FL);                // Turn off Fire service light
	  		else if (((Blink_FSL == 1) || (eqf != 0) || (stallf != 0) || (hotoilf != 0)) && (timers[tsec] != 0))
	      		clroutp(o_FL);                // Turn off Fire service light
			else
	  			setoutp(o_FL);				  // Turn on Fire service light

			if ((eqf != 0) || (stallf != 0) || (hotoilf != 0) || ((empf != 0) && (empf != ep_select) && (empf != ep_recall)))
				FS_DOB_DIS = 0;			// Allow door open button at the landing if the car cannot run

	    	if ((chk_ins() == 1) || ((safeties() == 0) && (rdinp(i_S10) == 1)) || 
	    				(medf != 0) || (hsvf == 1) || ((codebf != 0) && (fvars[fvcboverfs] == 1)))
			{
				fire_buz = 1;
      			return;
      		}
      		else if( ((position == fvars[fvfs1f]) && (rdinp(i_DOL) == 0)) ||
	  		((position == fvars[fvfs1f]) && (rdinp(i_DOLR) == 0) &&
	    	(cons[rear] != 0)) )
      		{         // At fire service main return
				fire_buz = 0;
				firef = 3;      // completed fire service phase 1
				rfiref = 3;
      		}
      		else if ((position == fvars[fvfs1f]) && ((fvars[fvfireop] & 0x02) != 0) && (procf >= 14) && (procf <= 16))
				fire_buz = 0;		   // added for chicago fire service
			else
				fire_buz = 1;
      		servf = s_F1MAIN;
			fire_floor = fvars[fvfs1f];
    	}
    	if(firef == 2)     // fire service phase 1 aternate floor return
    	{
			if ((empf != 0) && (empf != ep_select) && (empf != ep_recall) && (fvars[fvflemp] == 1))
	      		clroutp(o_FL);                // Turn off Fire service light
			else if (((eqf != 0) || (stallf != 0)) && (fvars[fvflots] == 1))
	      		clroutp(o_FL);                // Turn off Fire service light
			else if (Extinguish_FSL == 1)
	      		clroutp(o_FL);                // Turn off Fire service light
	  		else if (((Blink_FSL == 1) || (eqf != 0) || (hotoilf != 0) || (stallf != 0)) && (timers[tsec] != 0))
	      		clroutp(o_FL);                // Turn off Fire service light
			else
	  			setoutp(o_FL);				  // Turn on Fire service light

			if ((eqf != 0) || (stallf != 0) || (hotoilf != 0) || ((empf != 0) && (empf != ep_select) && (empf != ep_recall)))
				FS_DOB_DIS = 0;			// Allow door open button at the landing if the car cannot run

      		if ((chk_ins() == 1) || ((safeties() == 0) && (rdinp(i_S10) == 1)) ||
      					(medf != 0) || (hsvf == 1) || ((codebf != 0) && (fvars[fvcboverfs] == 1)))
			{
				fire_buz = 1;
      			return;
      		}
      		else if( ((position == fvars[fvfs1af]) && (rdinp(i_DOL) == 0)) ||
	  			((position == fvars[fvfs1af]) && (rdinp(i_DOLR) == 0) &&
	    		(cons[rear] != 0)) )
      		{         // At fire service alternate return
				fire_buz = 0;
				firef = 3;      // completed fire service phase 1
				rfiref = 3;
      		}
      		else if ((position == fvars[fvfs1af]) && ((fvars[fvfireop] & 0x02) != 0) && (procf >= 14) && (procf <= 16))
				fire_buz = 0;			// added for chicago fire service
      		else
				fire_buz = 1;
	  		if (((fs_on == 1) && ((cons[firesv] != 2) || (fvars[fvfsx] == 0))) || 
	  			  ((fs_on == 1) && (cons[firesv] == 2) && (fsx_on == 1) && (fvars[fvfsx] == 1)))
			{
				rfiref = 1;
				firef = 1;
			}
      		servf = s_F1ALT;
			fire_floor = fvars[fvfs1af];
    	}
		if (firef == 3)       // completed fire service phase 1
		{
			fs2_to_fs1 = 0;
			FS_DOB_DIS = 0;			// Allow door open button at the designated landing

			if ((fs_on == 1) && (cons[firesv] == 3) && ((fvars[fvfireop] & 0x01) != 0))
				Blink_FSL = 0;			// added for chicago fire service
						
			if (cons[firesv] == 2)
			{
				if ((fs_on == 1) && ((fsx_on == 1) || (fvars[fvfsx] == 0) || (fvars[fvrecallfs1a] == 1)) &&
				  	(position != fvars[fvfs1f]))
		  		{
			  		firef = 1;
			  		rfiref = 1;
					if (fvars[fvaltrclfs] == 0)
						fire_sensor = 0;		// once recalled from FS stay at the main fire floor
		  		}
				else if ((fire_sensor == 2) && ((alt_on == 1) || ((fvars[fvaltrclfs] & 0x02) != 0)) &&
					 (fs_on == 0) && ((fsx_on == 0) || (fvars[fvfsx] == 0)) &&
					 (position != fvars[fvfs1af]))
				{			  // if sensor still active allow return to alternate floor for fire service
		  			firef = 2;	   // this check is here in case the car has to recover to another floor from going out of the door zone.
		  			rfiref = 2;
				}
			}
	  		else if((fs_on == 1) && (position != fvars[fvfs1f]))
	  		{
		  		firef = 1;
		  		rfiref = 1;
	  		}
			if ((cons[firesv] == 3) && (fs_on == 0) && (position != fvars[fvfs1af]) && 
					(fire_sensor == 2) && (alt_on == 1))
			{			  // if sensor still active allow return to alternate floor for chicago fire service
	  			firef = 2;
	  			rfiref = 2;
			}
			// fire service return to lobby key switch
	  		if ((fs2_on == 1) && // fire service phase 2
					(((fs_on == 1) && ((position == fvars[fvfs1f]))) || (cons[firesv] != 3)))
	  		{			   // only chicago fire srevice must have the phase 1 switch on (not just a smoke sensor)
							// and at the main fire recall landing
		  		servf = s_FIRE2;
		  		if(cons[firesv] == 1)   // New York Fire service
		  		{
			 		if(rdinp(i_DCL) == 1)
						firef = 8;    // momentary presure of dcb will close doors
			 		else
						firef = 5;	// peekaboo door open
			 		if(cons[rear] != 0)
			 		{
				 		if(rdinp(i_DCLR) == 1)
					 		rfiref = 8;    // momentary presure of dcb will close doors
				 		else
					 		rfiref = 5;	// peekaboo rear door open
			 		}
		  		}
		  		else if (cons[firesv] == 3)
				{	// Chicago fire service
					firef = 5;			// fire flag will be set again in (firef == 5) section
			  		if (cons[rear] != 0) 
				  		rfiref = 5;    // fire flag will be set again in (firef == 5) section
				}
		  		else
		  		{	   // ansi 1996 or 2000
			  		if(rdinp(i_DCL) == 1)
				  		firef = 6;    // constant presure of dcb will close doors
			  		else
				  		firef = 5;    // peekaboo door open
			  		if(cons[rear] != 0)
			  		{
				  		if(rdinp(i_DCLR) == 1)
					  		rfiref = 6;    // constant presure of dcb will close rear doors
				  		else
					  		rfiref = 5;	   // peekaboo rear door open
			  		}
		  		}
		  		return;
	  		}
			else
			{
				if (((servf != s_F1MAIN) && (servf != s_F1ALT)) && ((empf == 0) || (empf == ep_recall) || (empf == ep_select)) )
				{	  // reset the service flag if it got cleared
					if (fire_floor == fvars[fvfs1f])
					{
						if (position != fvars[fvfs1f])
							firef = 1;
						servf = s_F1MAIN;	  	// Main fire floor
					}
					else if (fire_floor == fvars[fvfs1af])
					{
						if (position != fvars[fvfs1af])
							firef = 2;
						servf = s_F1ALT;		// Alternate fire floor
					}
					else
					{
						if (position != fvars[fvfs1f])
							firef = 1;
						servf = s_F1MAIN;
					}
				}
			}
			if ((cons[EMPop] & 0x02) != 0)
			{				  // Emergency power generator
				if ((empf != 0) && (empf != ep_select) && (empf != ep_recall) && (fvars[fvflemp] == 1))
		      		clroutp(o_FL);                // Turn off Fire service light
				else if (((eqf != 0) || (stallf != 0) || (servf == s_LOW_PRES)) && (fvars[fvflots] == 1))
		      		clroutp(o_FL);                // Turn off Fire service light
		  		else if (Extinguish_FSL == 1)
					clroutp(o_FL);                // Turn off Fire service light
		  		else if (((Blink_FSL == 1) || (eqf != 0) || (hotoilf != 0) || (stallf != 0))&& (timers[tsec] != 0))
		      		clroutp(o_FL);                // Turn off Fire service light
		  		else
		      		setoutp(o_FL);
			}
			else
			{				// Emergency power lowering
	  			if (((Blink_FSL == 1) || (empf != 0) || (eqf != 0) || (hotoilf != 0) || (stallf != 0)) && (timers[tsec] != 0))
		      		clroutp(o_FL);                // Turn off Fire service light
		  		else if (Extinguish_FSL == 1)
					clroutp(o_FL);                // Turn off Fire service light
		  		else
		      		setoutp(o_FL);
			}

	    	if(chk_ins() == 1)
				fire_buz = 1;
			else
				fire_buz = 0;
    	}

		if ((cons[firesv] == 2) && (cons[ansi] >= 2004))
		{
			if ((rdinp(i_FFS) == 1) && 
				(rdinp(i_CSS) == 1) &&
				(fire_stopsw_hold == 1))
			{
				if ((procf >= 14) && (procf <= 16)) 
					fire_stopsw_hold = 0;		// stop switch not pressed and at the floor
				else if ((procf >= 3) && (procf <= 11) && (firef != 0) && (firef <= 3))
				{				  // Trying to run the car on phase 1
					dirsel();
					if ((dpref == 1) && ((dcalls == 1) || (dcalls == 3)))
						dirf = 1;
					else if ((dpref == 2) && (dcalls > 1))
						dirf = 2;
					fire_stopsw_hold = 0;
				}
			}
		}
		else
			fire_stopsw_hold = 0;


		if (firef <= 3)
			return;

		// on phase 2
		if ((cons[firesv] == 2) && (cons[ansi] >= 2004))
		{
			if ((rdinp(i_FFS) == 0) && (rdinp(i_CSS) == 1))
			{
				clrcc(0);
				dpref = 0;
				dirf = 0;
				clr_dir_arrow();
				fire_stopsw_hold = 1;
			}			
			else if (fire_stopsw_hold == 1)
			{
				dirsel();
				if (((dcalls != 0) || 
					(rd_both_inps(i_CC(position),i_XCC(position)) == 1) ||
					((cons[rear] == 1) && (rd_both_inps(i_CCR(position),i_XCCR(position)) == 1))) &&
						((cons[Australia] == 0) || (fs2_start == 1)))
				{
					if ((dcalls & 0x1) == 1)
						dirf = 1;
					else if ((dcalls & 0x2) == 2)
						dirf = 2;
					else 
					{
						if (prev_dirf == 1)
						{
							if ((stepf == 0) || (pass_fl == 1))
								dirf = 1;
							else
								dirf = 2;
						}
						else if (prev_dirf == 2)
						{
							if ((stepf == 0) || (pass_fl == 1))
								dirf = 2;
							else
								dirf = 1;
						}
						else  
							dirf = 2;
					}
					fire_stopsw_hold = 0;
				}

			}
		}
		if ((cons[firesv] == 3) && ((fvars[fvfireop2] & 0x02) != 0))   // chicago fire service (blinking disabled)
      		setoutp(o_FL);				  // Turn on Fire service light
		else
		{
	   
			if ((cons[EMPop] & 0x02) != 0)
			{
				if ((empf != 0) && (empf != ep_select) && (empf != ep_recall) && (fvars[fvflemp] == 1))
		      		clroutp(o_FL);                
				else if (((eqf != 0) || (stallf != 0)) && (fvars[fvflots] == 1))
		      		clroutp(o_FL);                
				else if (Extinguish_FSL == 1)
		      		clroutp(o_FL);                // Turn off Fire service light
		  		else if (((eqf != 0) || (hotoilf != 0) || (stallf != 0) || ((hws2_on == 1) && (cons[Smoke_2004] != 0)) || (mrs_on == 1) || (hws_on == 1)) && (timers[tsec] != 0))
		      		clroutp(o_FL);                
		  		else
		      		setoutp(o_FL);				  // Turn on Fire service light
			}
			else
			{
		  		if (((empf != 0) || (eqf != 0) || (hotoilf != 0) || (stallf != 0) || ((hws2_on == 1) && (cons[Smoke_2004] != 0)) ||
		  				(mrs_on == 1) || (hws_on == 1)) && (timers[tsec] != 0))
		      		clroutp(o_FL);                
		  		else
		      		setoutp(o_FL);				  // Turn on Fire service light
			}
		}

		if((fvars[fvfiresw] & 4) != 0)
		{//use car_Grp_Stat.c_fvfiresw_loc from group
			if (fvars[fvfs1f] == position)
			{
				if (cons[FSX_rear] == 1)
				{
					if (fs_on == 1)
						fire_sw_loc = 0;	// front fire door
					else if (fsx_on == 1)
						fire_sw_loc = 1; 	// rear fire door
					else
						fire_sw_loc = ((car_Grp_Stat.c_fvfiresw_loc & 1) != 0);
				}
				else
					fire_sw_loc = ((car_Grp_Stat.c_fvfiresw_loc & 1) != 0);
			}
			else if (fvars[fvfs1af] == position)
				fire_sw_loc = ((car_Grp_Stat.c_fvfiresw_loc & 2) != 0);
			else 
				fire_sw_loc = 0;			
		}
		else
		{
			if (fvars[fvfs1f] == position)
			{
				if (cons[FSX_rear] == 1)
				{
					if (fs_on == 1)
						fire_sw_loc = 0;	// front fire door
					else if (fsx_on == 1)
						fire_sw_loc = 1; 	// rear fire door
					else
						fire_sw_loc = ((fvars[fvfiresw] & 1) != 0);
				}
				else
					fire_sw_loc = ((fvars[fvfiresw] & 1) != 0);
			}
			else if (fvars[fvfs1af] == position)
				fire_sw_loc = ((fvars[fvfiresw] & 2) != 0);
			else 
				fire_sw_loc = 0;
		}

    	if( (rdinp(i_DOL) == 0) ||
		((rdinp(i_DOLR) == 0) && (cons[rear] != 0)) )
			A_Door_Is_Open = 1;
    	else
			A_Door_Is_Open = 0;

    	if ((fs2_on == 1) || 
    		((fs2_hold == 1) && (cons[firesv] != 3)))
		{										// no hold switch on chicago fire service
			fs2_to_fs1 = 0;
		 	timers[tfs2t] = 0;        // clear fire service phase 2 timer
		}

    	if((fs2_on == 0) && (timers[tfs2t] >= 15) && (firef > 3))
    	{
	    	if ((ccnmsk[cons[carnmb]][position] != 0) && 							// valid front floor
				(position == fire_floor) &&										// car at the fire floor
		    	(rdinp(i_DOL) == 0) && (fs1_effect == 0) &&	 				// door open and not on phase 1
		    	((rccnmsk[cons[carnmb]][position] == 0) || (fire_sw_loc == 0)))		// valid fire floor switch in front
			{		  
				firef = 0;		// go off of fire service
				rfiref = 0;
		  		clroutp(o_FL);      // Turn off the fire service light
			}
		    else if ((rccnmsk[cons[carnmb]][position] != 0) && 						// valid rear floor
				(position == fire_floor) &&										// car at the fire floor
		    	(rdinp(i_DOLR) == 0) && (fs1_effect == 0) && 	// door open and not on phase 1
		    	((ccnmsk[cons[carnmb]][position] == 0) || (fire_sw_loc != 0)))				// valid fire floor switch in rear
			{
	   			firef = 0;		// go off of fire service
	   			rfiref = 0;
		  		clroutp(o_FL);      // Turn off the fire service light
			}
			else if ((eqf == 0) && ((empf == no_ep) || (empf == ep_recall) || (empf == ep_select) || (position == fire_floor)))
			{					// cannot revert to phase 1 if eqf != 0: The door will open and fireman will loose control of the car 
				if (cons[firesv] == 2)
				{				// Ansi 2000 - Return to the designated landing
					firef = 1;
					rfiref = 1;
					if (fire_floor == fvars[fvfs1af])
					{
						firef = 2;
						rfiref = 2;
					}
					fs2_to_fs1 = 1;
				}
				else if (cons[firesv] == 3)
				{				// Chicago fire
					if (fs1_effect == 1)
					{
						if ( (((fvars[fvfireop2] & 0x01) != 0) && 
								((A_Door_Is_Open == 1) || (((manual_door == 1) || (manual_rdoor == 1)) && (car_gate() == 0)))) ||
							 ((fvars[fvfireop2] & 0x01) == 0) )
						{
							firef = 1;
							rfiref = 1;
							if (fire_floor == fvars[fvfs1af])
							{
								firef = 2;
								rfiref = 2;
							}
							fs2_to_fs1 = 1;
						}
					}
				} 
				else if ((fs1_effect == 1) && (A_Door_Is_Open == 1))
				{		// New York fire or Ansi 1996 
					firef = 1;
					rfiref = 1;
					if (fire_floor == fvars[fvfs1af])
					{
						firef = 2;
						rfiref = 2;
					}
					fs2_to_fs1 = 1;
				}
			}
    	}
		if(firef >= 4)
			servf = s_FIRE2;
    	if((firef == 4) || ((firef > 4) && ((rdinp(i_DOL) == 0) || (manual_door == 1)))) 
    	{
      		if( ((fs2_hold == 1) && (cons[firesv] != 3)) ||
	  		((fs_on == 0) && (mes_on == 0) &&
			((fsx_on == 0) || (((cons[firesv] != 2) || (fvars[fvfsx] == 0)) && (cons[FSX_rear] == 0) && (cons[firesv] != 3))) &&
	   		(alt_on == 0) && (fs2_on == 0) &&	((hws2_on == 0) || (cons[Smoke_2004] == 0)) &&
	   		(hws_on == 0) && (mrs_on == 0)) )
      		{
				if (cons[firesv] == 3)
				{
					if ((ccnmsk[cons[carnmb]][position] != 0) && ((rdinp(i_DOL) == 0) ||
								(rccnmsk[cons[carnmb]][position] == 0) || (rdinp(i_DCLR) == 0)))
					{			  // Front door already open or no valid rear or rear door closed
						firef = 4;      // fire service phase 2 door hold
						clrcc(0);
					}
				}
				else
				{
					firef = 4;      // fire service phase 2 door hold
					clrcc(0);
      			}
      		}
			if (cons[firesv] == 3)
			{		 // Chicago fire service
				if ((firef == 4) && (fs2_on == 1))
					firef = 5;
			}
      		else if((firef==4)&&(fs2_hold == 0)&&(fs2_on == 1))
      		{
				if(cons[firesv] == 1)
				{
		  			firef = 8;		// New York fire service
				}
				else
		  			firef = 6;		// Ansi 1996 or 2000
      		}
    	}
    	if(firef >= 5)      // fire service phase 2 peek a boo service
    	{
			if (cons[firesv] == 3)
			{		// Chicago fire service
				if ((position == fire_floor) && ((rccnmsk[cons[carnmb]][position] == 0) || (fire_sw_loc == 0)))
					firef = 6;
			}
      		else if (rdinp(i_DOL) == 0)		// when doors are fully open
      		{
				if (cons[firesv] == 1)
				{
		  			firef = 8;	   // New York fire service
					momentary_dcb = 0;
				}
				else
		  			firef = 6;	   // Ansi 1996 or 2000
      		}
      		else if (rdinp(i_DCL) == 0)		// when doors are fully closed
      		{
      			if (cons[firesv] == 1)	
					momentary_dcb = 0;
      		}
    	}
    	if(firef == 6)      // fire service phase 2 constant presure door closed
    	{
			if (cons[firesv] == 3)
			{	   // Chicago fire service
				if ((position != fire_floor) || ((rccnmsk[cons[carnmb]][position] == 1) && (fire_sw_loc != 0)) )
					firef = 5;
			}
      		else if(rdinp(i_DCL) == 0)
				firef = 5;
    	}
    	if(firef == 8)      // fire service phase 2 momentary dcb door closed
    	{
			//added logic to just the DCL == 0 because the firef is being set to 8 when it should be set to 5. Possibly noise
			//and this caused door to reopen without peekaboo
      		if((rdinp(i_DCL) == 0)||
			((rdinp(i_DOL) == 1) && (rd_both_inps(i_DOB,i_XDOB) == 1) && (cons[firesv] == 1)))
				firef = 5;
    	}
    	if ((rfiref == 4) || ((rfiref > 4) && ((rdinp(i_DOLR) == 0) || (manual_rdoor == 1))))
    	{
      		if( (fs2_hold == 1) ||
	  		((fs_on == 0) && (mes_on == 0) &&
			((fsx_on == 0) || (((cons[firesv] != 2) || (fvars[fvfsx] == 0)) && (cons[FSX_rear] == 0) && (cons[firesv] != 3))) &&
	   		(alt_on == 0) && (fs2_on == 0) &&	 ((hws2_on == 0) || (cons[Smoke_2004] == 0)) &&
	   		(hws_on == 0) && (mrs_on == 0)) )
      		{
				if (cons[firesv] == 3)
				{
					if ((rccnmsk[cons[carnmb]][position] != 0) && 
							((rdinp(i_DOLR) == 0) ||
							(ccnmsk[cons[carnmb]][position] == 0)) )	// rear door already open or no valid front
					{
						rfiref = 4;      // fire service phase 2 door hold
						clrcc(0);
					}
				}
				else
				{
					rfiref = 4;      // fire service phase 2 door hold
					clrcc(0);
      			}
      		}
			if (cons[firesv] == 3)
			{		 // Chicago fire service
				if ((rfiref == 4) && (fs2_on == 1))
					rfiref = 5;
			}
      		else if((rfiref==4)&&(fs2_hold == 0)&&(fs2_on == 1))
      		{
				if(cons[firesv] == 1)
				{
		  			rfiref = 8;		  // New York fire service
				}
				else
		  			rfiref = 6;		  // Ansi 1996 or 2000
      		}
    	}
    	if(rfiref >= 5)      // fire service phase 2 peek a boo service
    	{
			if (cons[firesv] == 3)
			{		// Chicago fire service
				if ((position == fire_floor) && ((ccnmsk[cons[carnmb]][position] == 0) || (fire_sw_loc != 0)))
					rfiref = 6;
			}
      		else if (rdinp(i_DOLR) == 0)		// when doors are fully open
      		{
				if(cons[firesv] == 1)
				{
		  			rfiref = 8;		  // New York fire service
					momentary_rdcb = 0;
				}
				else
		  			rfiref = 6;		  // Ansi 1996 or 2000
      		}
      		else if (rdinp(i_DCLR) == 0)		// when doors are fully closed
      		{
      			if(cons[firesv] == 1)
				{
					momentary_rdcb = 0;
				}
      		}
    	}
    	if(rfiref == 6)      // fire service phase 2 constant presure door closed
    	{
			if (cons[firesv] == 3)
			{	   // Chicago fire service
				if ((position != fire_floor) || ((ccnmsk[cons[carnmb]][position] == 1) && (fire_sw_loc == 0)))
					rfiref = 5;
			}
      		else if(rdinp(i_DCLR) == 0)
				rfiref = 5;
    	}
		if(rfiref == 8)      // fire service phase 2 momentary dcb door closed
		{
	    	if(rdinp(i_DCLR) == 0)
				rfiref = 5;
		}
  	}
}

// ***********************************************
// This is the HUGS Security Service Procedure
// ***********************************************

void sethugsf(void)
{
	int16 fl_ix;
	int16 flash_hugo = 0;
	int hugs_alarm = 0;
	static int hugs_state;


	// Call from atfloor()

	if ((cons[hugs] == 1) || (cons[hugs] == 2))
	{
		fl_ix = (position-1)/32;

		hugsf = 0;
		if ((gc_hugsec == 1) && ((gc_hugs_sec[fl_ix] & fl_mask[position]) != 0) && (doorf != 0))
		{
			hugsf |= 1;
			flash_hugo = 1;
				
		}
		if ((gc_rhugsec == 1) && ((gc_rhugs_sec[fl_ix] & fl_mask[position]) != 0) && (cons[rear] != 0) && (rdoorf != 0))
		{
			hugsf |= 2;
			flash_hugo = 1;
		}
	}
	else if(cons[hugs] == 3)
	{

		if(rdinp(i_HUGB) == 1)
		{	
			hugs_state = 0;
			hugsf = 0;
			timers[thugs1] = 0;
			timers[thugs2] = 0;
			flash_hugo = 0;
			hugs_alarm = 0;
		}
		else if(rdinp(i_HUGI) == 1)
		{
			if(doorf != 0)
			{
				hugsf |= 1;
			}
			if((cons[rear] != 0) && (rdoorf != 0))
			{
				hugsf |= 2;
			}			
		}
		switch(hugs_state)
		{
			case 0:				//Normal Operation. Check for input to trigger
				if ((rdinp(i_HUGI) == 1) &&(rdinp(i_HUGB) == 0))
				{
					hugs_state = 1;
				}
				timers[thugs1] = 0;
				timers[thugs2] = 0;
				break;

			case 1: 			// Input has trigger Enter Pre Alarm State
				flash_hugo = 1;				
				timers[thugs2] = 0;
				if(timers[thugs1] > 110)
				{
					hugs_state = 2;
				}
				else if(rdinp(i_HUGI) == 0)
				{
					hugs_state = 3; // reset
				}					   
				break;
			case 2:				//Latch Exit Alarm and attempt reset after 10 sec of inactivity
				if(rdinp(i_HUGI) == 1)
				{
					timers[thugs2] = 0;
				}
				else if(timers[thugs2] > 100)
				{
					hugs_state = 3; // reset
				}
				flash_hugo = 2;
				hugs_alarm = 1;
				break;
			case 3:				//Reset State
				hugs_state = 0;
				hugsf = 0;
				timers[thugs1] = 0;
				timers[thugs2] = 0;
				flash_hugo = 0;
				hugs_alarm = 0;
				break;
			default:
				hugs_state = 0;
				break;
		}
		
		if (hugs_alarm == 1)
			setoutp(o_HUGA);
		else
			clroutp(o_HUGA);
		
	}
	else
		hugsf = 0;

	if (flash_hugo == 2)
		setoutp(o_HUGO);
	else if ((flash_hugo == 1) && (timers[tsec] != 0))
		setoutp(o_HUGO);
	else
		clroutp(o_HUGO);

}

// ***********************************************
// This is the Medical Emergency Service Procedure
// ***********************************************

void setmedf(void)
{
	if (cons[medEm] == 1)
	{		// Medical emergency service option is selected

		if (medf == 0)		// Not on Medical Emergency Service
		{
			if (rdinp(i_EMSH) == 0)
				timers[tmedf] = 0;
			if (((rdinp(i_EMSH) == 1) && (timers[tmedf] > 3)) || (med_latch == 1))
			{ 
				med_latch = 1;
				setoutp(o_EMLH);
				setoutp(o_EML);
				med_buz = 1;
				if ((chk_ins() == 1) || (firef != 0) || (rfiref != 0))
				{	 // Turn off buzzzer and light if could not go to next state and switch turned off
					if (med_latch == 1)
					{
						if (((firef > 3) || (rfiref > 3)) && (position == fvars[fvmedfl]) && ((doorf != 0) || (rdoorf != 0)))
						{		// Fireman has control over the car at the medical emergency floor
							med_latch = 0;
							med_buz = 0;
							clroutp(o_EMLH);
							clroutp(o_EML);
						}
					}
				}				
				else if ((ind() == 0) || (fvars[fvmedindovr] != 2))
					medf = 1;			// Return car to Med Floor
			}
		}
		else if (medf == 1)
		{				 // Look for car at Medical Emergency Floor
			med_latch = 0;
			clrcc(0);
			setoutp(o_EMLH);
			setoutp(o_EML);
			if (chk_ins() == 1) 
				med_buz = 1;
	      	else if( ((position == fvars[fvmedfl]) && (rdinp(i_DOL) == 0) &&
	      				((fvars[fvmedloc] == 0) || (cons[rear] == 0))) ||
		  			((position == fvars[fvmedfl]) && (rdinp(i_DOLR) == 0) &&
		    			(cons[rear] != 0) && (fvars[fvmedloc] != 0)) )
	      	{         // At Medical Emergency Service return floor
				med_buz = 0;
				medf = 2;			// Car at the Medical Emergency Floor
				timers[tmedf] = 0;
	      	}
	      	else
				med_buz = 1;
		}
		else if (medf == 2)
		{				// Car at the Medical emergency floor - Place on Med Service or return to automatic
			clrcc(0);
			if (rdinp(i_EMSH) == 1)
				timers[tmedf] = 0;		// Switch is still on so don't let it time out
			if (chk_ins() == 1) 
				med_buz = 1;
			else if (rdinp(i_EMS) == 1)
			{
				medf = 3;
				med_buz = 0;
				med_latch = 0;
			}
			else if (timers[tmedf] > 610) 		// Timed out after 61 seconds
			{
				clroutp(o_EMLH);
				clroutp(o_EML);
				medf = 0;
			}
			else if (position != fvars[fvmedfl])
				medf = 1;	// must get to medical floor
		}
		else if (medf == 3)
		{		   		// On Medical Emergency Service
			if (rdinp(i_EMS) == 0)
			{ 
				if ( ((position == fvars[fvmedfl]) && (rdinp(i_DOL) == 0) && 
										((fvars[fvmedloc] == 0) || (cons[rear] == 0))) ||
		  			((position == fvars[fvmedfl]) && (rdinp(i_DOLR) == 0) &&
		    							(cons[rear] != 0) && (fvars[fvmedloc] != 0)) )
				{			  // At the Medical Emergency Floor with the door open 
					if (rdinp(i_EMSH) == 1)
						medf = 2;	 // Hall switch is still on
					else
					{
						clroutp(o_EMLH);
						clroutp(o_EML);
						medf = 0;
						med_latch = 0;
					}
				}
				else
					clrcc(0);
			}
			if ((empf != 0) && (empf != ep_select)) 
				clrcc(0);
		}
		if ((medf >= 1) && (medf <= 2) && (firef != 0) && (position == fire_floor))
			medf = 0;
		if ((medf != 0) && (firef < 3) && (eqf == 0) && ((empf == 0) || (empf == ep_select) || ((empf == ep_recall) && (medf == 3))))
			servf = s_MED_EMS;
		else if ((servf == s_MED_EMS) && (medf == 0))
			servf = s_AUTO;
	}
	else
		medf = 0;
}

// ****************************************
// Set the Vip flag
// ****************************************

void setvipf (void)
{
	if ((cons[VipSvc] != 0) || (cons[incarvip] == 1))
	{
		if ((((car_Grp_Stat.c_vip_fl != 0) || (car_Grp_Stat.c_rvip_fl != 0) || (vipf == 4) || (vipf == 5)) && (Group_Comm == 1) && (vipf != 6)) || 
			((cons[incarvip] == 1) && ((rdinp(i_ICVIP) == 1) || (vipf == 4) || (vipf == 5)) && (vipf != 6)))
		{
			servf = s_VIP;
			in_vip = 1;

			if (cons[VipSvc] != 0)
			{
				if (car_Grp_Stat.c_vip_fl != 0)
					vipfl = car_Grp_Stat.c_vip_fl;
				else 
					vipfl = car_Grp_Stat.c_rvip_fl;
			}
			else if (cons[incarvip] == 1)
			{
				if ((rdinp(i_ICVIP)) && (vipf == 0))
				{
					vipfl = position;	// set vip floor to the current position
					vipf = 4; 	// advance the vip flag to being at the vip floor
				}
			}


			if ((position != vipfl) && (vipf < 2))
			{
				timers[tvipseqto] = 0;
				if ( ((fvars[fvvipop] & 0x02) != 0) || (vipf == 1) || 
					 ( ((onward_cc == 0) && ((front_slowdown & cc_do) == 0) && (((rear_slowdown & cc_do) == 0) || (cons[rear] == 0))) && 
						(((doorf == 0) && ((cons[rear] == 0) || (rdoorf == 0))) || (((manual_door == 2) || (manual_rdoor == 2)) && (man_dcc() == 1))) ) )
				{							 // The door is closed and no onward car calls
					if (vipf == 0)
						clr_dir_arrow();
					vipf = 1;		// want to get to the vip floor
					clrcc(0);
					clrhc();
					clrlant();
				}
			}
			else if ((position == vipfl) && (vipf < 4))
			{			// at the vip floor
				timers[tvipseqto] = 0;
				if ((procf < 3) || (procf > 11))
				{			   // car is not moving
					if (car_Grp_Stat.c_vip_fl != 0)	
					{		// Front door
						if ((doorf == 2) && (vipf == 2) && (doort != 0))
						{
							vipf = 3;	  // front door is open
						}
						else if ((vipf == 0) || (vipf == 1))
							vipf = 2;		// command to open the door and set the doort
						if ((vipf == 3) && (((timers[tdwel] >= doort) && (doort != 0)) || ((onward_cc != 0) && (timers[tdwel] >= fvars[fvccdt]))))
						{
							if (fvars[fvviplant] != 0)
								clrlant();
							vipf = 4;
							doort = 0;
						}
					}
					else
					{	   // Rear door
						if ((rdoorf == 2) && (vipf == 2) && (rdoort != 0))
						{
							vipf = 3;		// rear door is open
						}
						else if ((vipf == 0) || (vipf == 1))
							vipf = 2;		// command to open the door and set the doort
						if ((vipf == 3) && (((timers[trdwel] >= rdoort) && (rdoort != 0)) || ((onward_cc != 0) && (timers[tdwel] >= fvars[fvccdt]))))
						{
							if (fvars[fvviplant] != 0)
								clrlant();
							vipf = 4;
							rdoort = 0;
						}
					}
				}
			}
			else 
			{
				if ((procf > 3) && (procf < 11)) 
				{			   // car is moving
					if (vipf == 4)
						vipf = 5;
					timers[tvipseqto] = 0;		// reset timer while moving
				}
				
				if (((procf < 3) || (procf > 11)) && (vipf == 5))
				{			   // car is not moving	but has moved 
					if (fvars[fvvipmulticall] != 0)		// multi call enabled
					{
						if (onward_cc != 0)
						{
							vipf = 4;
							timers[tvipseqto] = 0;
						}
						else if (timers[tvipseqto] > 100)
							vipf = 6;					// go to normal mode
					}
					else if (timers[tvipseqto] > 30)
						vipf = 6;					// go to normal mode
				}
				if ( (((car_Grp_Stat.c_vip_fl == 0) && (car_Grp_Stat.c_rvip_fl == 0)) || (timers[tvipseqto] > 30)) && (onward_cc == 0) && (vipf == 4))
				{		// no call so car must have timed out
					vipf = 6;
				}
			}
		}
		else
		{
			if (in_vip == 1)
			{
				in_vip = 0;
				doort = 0;
				rdoort = 0;
				if (servf == s_VIP)
					servf = s_AUTO;
			}
			vipf = 0;
			vipfl = 0;
			timers[tvipseqto] = 0;
		}
		
	}
	else
	{
		in_vip = 0;
		if (servf == s_VIP) 
			servf = s_AUTO;
		vipf = 0;
		vipfl = 0;
	}
}




//*********************************
//****  Set Flag for TUG systems 
//*********************************

void settugf(void) 
{


	if(cons[TUG] == 1)
	{

		if(rdinp(i_DOL) == 1)
			setoutp(o_TDOL);
		else
			clroutp(o_TDOL);

		if (rdinp(i_DCL) == 1)
			setoutp(o_TDCL);
		else
			clroutp(o_TDCL);

		if(rdinp(i_DOLR) == 1)
			setoutp(o_TDOLR);
		else
			clroutp(o_TDOLR);

		if(rdinp(i_DCLR) == 1)
			setoutp(o_TDCLR);
		else
			clroutp(o_TDCLR);

    	if ((chk_ins() == 1) || (safeties() == 0))
			clroutp(o_TAVL);		  // out of service and no door operation
		else
			setoutp(o_TAVL);		  
		
		if ((statusf2 & sf_AF_SHUTDN) != 0)
			clroutp(o_TAVL2);		  // out of service and no door operation
		else
			setoutp(o_TAVL2);

		if(ind() == 1)
			setoutp(o_TIND);
		else
			clroutp(o_TIND);

		if((codebf !=0) || (hsvf != 0))
			setoutp(o_TMED);
		else
			clroutp(o_TMED);

		if((firef != 0)||(rfiref != 0))
			setoutp(o_TFSO);
		else
			clroutp(o_TFSO);

	  	if((position & 0x01) != 0)
			setoutp(o_TP1);
		else
			clroutp(o_TP1);

	  	if((position & 0x02) != 0)
			setoutp(o_TP2);
		else
			clroutp(o_TP2);

	  	if((position & 0x04) != 0)			
			setoutp(o_TP3);
		else
			clroutp(o_TP3);

	  	if((position & 0x08) != 0)			
			setoutp(o_TP4);
		else
			clroutp(o_TP4);

	  	if((position & 0x10) != 0)			
			setoutp(o_TP5);
		else
			clroutp(o_TP5);

	  	if((position & 0x20) != 0)
			setoutp(o_TP6);
		else
			clroutp(o_TP6);				



		if((firef == 0) && (rfiref == 0) && (codebf == 0) && (hsvf == 0) && (eqf == 0) && (empf == 0))
		{
			if (rdinp(i_THDS) == 1)
			{
				if ((ind() == 0) && (servf > s_LOW_OIL) && ((statusf2 & sf_AF_SHUTDN) == 0))
					servf = s_TUGS;
				tugf = 1;

			}
			else
			{
				if(servf == s_TUGS)
					servf = s_AUTO;
				tugf = 0;

			}
		}
		else
		{

			if(servf == s_TUGS)
				servf = s_AUTO;
			tugf = 0;

		}

	}
	
}



//***********************************
//****  Set up level, dn level and dz 
//***********************************
/*
void set_ul_dl_dz(void)
{
	int32 Floor_DPP = DPP_Floor_Pos[position]; 

	if (cons[sel_type] == 4)
	{
		
		if( (labs(Floor_DPP - DPP_Count)) <= eight_inches) 
		{	   // within 8 inches
			if (DPP_Count <= (Floor_DPP + dead_zone))
				up_level = 1;		// On up level
			else
				up_level = 0;
			
			if (DPP_Count >= (Floor_DPP - dead_zone)) 
				dn_level = 1;		// On down level 
			else
				dn_level = 0;	
		}
		
		if( (labs(Floor_DPP - DPP_Count)) <= three_inches ) 
		{	   // within 3 inches
			door_zone = 1;		// On up level
		}
		else
			door_zone = 0;

		if( (labs(Floor_DPP - DPP_Count)) <= five_inches ) 
		{	   // within 5 inches
			door_zone_aux = 1;		// On down level 
		}
		else
			door_zone_aux = 0;	
	}
	else
	{
		dn_level = rdinp(i_DL);
		up_level = rdinp(i_UL);
		door_zone = rdinp(i_DZ);
		door_zone_aux = rdinp(i_DZA);
	}
		
}
*/
	
//***********************************************
//****  Set leveling up level and dn level and DZ
//***********************************************

void set_ul_dl_dz(void)
{
	dn_level = rdinp(i_DL);
	up_level = rdinp(i_UL);
	door_zone = rdinp(i_DZ);
	door_zone_aux = rdinp(i_DZA);

	if ((fvars[fvppstop] == 1) && (hsf == 0))
	{
		if ((dn_level == 1) && (up_level == 1) && (level_stop_fault == 0))
		{			   // Car is in the dead level zone
			if ((leveling == 0) && (lev_latch == 0) && ((rdoutp(o_SU) == 1) || (rdoutp(o_SD) == 1)) && (in_redundancy_test == 0))
			{
				ul_dl_offset = DPP_Count;
				if (rdoutp(o_SU) == 1)
					ul_dl_offset += (int32)Up_fl_level_dist;
				else if (rdoutp(o_SD) == 1)
					ul_dl_offset -= (int32)Dn_fl_level_dist;
				lev_latch = 1;
			}

			if ((rdoutp(o_SU) == 1) && (in_redundancy_test == 0))
			{	// Leveling so use position pulse count
				leveling = 1;					// car looking for dead level from counts
				up_level = rdinp(i_UL);
				if (DPP_Count >= ul_dl_offset)
					dn_level = 1;
				else
					dn_level = 0;
			}
			else if ((rdoutp(o_SD) == 1) && (in_redundancy_test == 0))
			{
				leveling = 1;					// car looking for dead level from counts
				dn_level = rdinp(i_DL);	
				if (DPP_Count <= ul_dl_offset)
					up_level = 1;
				else
					up_level = 0;
			}
			else
			{
				ul_dl_offset = 0;
				leveling = 0;
			}
		}
		else
		{
			lev_latch = 0;
			if ((leveling == 1) && ( ((up_level == 0) && (rdoutp(o_SU) == 1)) || ((dn_level == 0) && (rdoutp(o_SD) == 1)) ))
			{
				record_fault(f_lev_stop_cnt);
				level_stop_fault = 1;
			}
			ul_dl_offset = 0;
			leveling = 0;
		}
	}
	else
	{
		leveling = 0;
		ul_dl_offset = 0;
		level_stop_fault = 0;
		relev_stop_count = 0;
		lev_latch = 0;
	}
	write_trace(TR_SET_UL_DL);
}


																				   
// ****************************************
// This is the set preference procedure
// ****************************************
void setpref()
{
    if((dpref == 2) && (dcalls != 2) && (dcalls != 3))
        dpref = 0;  // prefrence was down no calls below clear preference
    if((dpref == 1) && (dcalls != 1) && (dcalls != 3))
        dpref = 0;  // preference was up no calls above clear preference
    if(dpref == 0)  // no preference selected
    {
        if((dcalls == 1) || (dcalls == 3))  // calls above
            dpref = 1;
        if(dcalls == 2)     // calls below
            dpref = 2;
    }
	if((manual_door == 1) && (cons[att] == 1) &&
		(((fvars[fvattmandir] & 0x04) !=0)  || ((athcd_in_enable == 1)&&(rdinp(i_ATHCD) == 1))	))
	{								    
		if(allow_run == 0) 
		 	dpref = 0;
		if(onward_cc == 1)
			dpref = 1;
		if(onward_cc ==2)
			dpref = 2;		
	}
	set_dir_arrow();
	return;
}

//***************************************************
// Set the position from the pulse count
//***************************************************
void set_position_from_dpp(void)
{
	int16 i;
	
	for(i=cons[bottomf];i<cons[topf];i++)
	{
		if (DPP_Count < ((DPP_Floor_Pos[i+1] - DPP_Floor_Pos[i] )/2) + DPP_Floor_Pos[i])
		{
			position = i;
			break;
		}
	}
	if (DPP_Count > ((DPP_Floor_Pos[cons[topf]] - DPP_Floor_Pos[cons[topf]-1] )/2) + DPP_Floor_Pos[cons[topf]-1])
		position = cons[topf];
	if (DPP_Count > DPP_Floor_Pos[cons[topf]])
		position = cons[topf];
								 
}

//*************************************************************
//**  The routine set the up and down direction arrows
//*************************************************************

void set_dir_arrow (void)
{
    if(firef == 0)
    {
		if(dpref == 0)
		{
			clr_dir_arrow();
		}

		if ((cons[att] != 0) && (rdinp(i_ATT) == 1))
		{
			if ((dcalls & 0x1) != 0)	// Call above
				setoutp(o_ATUL);
			else
				clroutp(o_ATUL);

			if ((dcalls & 0x2) != 0)	// Call below
				setoutp(o_ATDL);
			else
				clroutp(o_ATDL);
		}

		if ((((cons[dispis] & 0x08) != 0) || ((cons[dispis] & 0x10) != 0))	&&
			(cons[att] != 0) && (rdinp(i_ATT) == 1))
		{		// Attendant direction arrows on the car using the UDA and DDA
			if ((dcalls & 0x1) != 0)	// Call above
				setoutp(o_UDAC);
			else
				clroutp(o_UDAC);

			if ((dcalls & 0x2) != 0)	// Call below
				setoutp(o_DDAC);
			else
				clroutp(o_DDAC);
		}
		else if ((cons[dispis] & 0x10) != 0)
		{				  // Turn off the arrows if not on att
			clroutp(o_UDAC);
			clroutp(o_DDAC);
		}
		else
		{	   // use uda and dda on automatic
			if(dpref == 1)
			{
				setoutp(o_UDAC);
				clroutp(o_DDAC);
			}
			if(dpref == 2)
			{
				clroutp(o_UDAC);
				setoutp(o_DDAC);
			}
		}


		if(dpref == 1)
		{
			up_dir_arrow = 1;
			dn_dir_arrow = 0;
		}
		if(dpref == 2)
		{
			up_dir_arrow = 0;
			dn_dir_arrow = 1;
		}
		if ((servf == s_SABBATH) && ((fvars[fvsabbdis] & 0x04) != 0))
		{
			clr_dir_arrow();
		}
	}
	else
	{
		clr_dir_arrow();
	}
}

void set_rcm(void)
{
	static int16 rcm_pick_attempt;
	static int16 rcm_attempt_frgt;

	if (cons[RCM_HWDoor] == 1)
	{
		if (chk_ins() != 0)  
		{
			if ((chk_ins_sw() != 0) && 
				(rdinp(i_ACC) == 0) && 
					(((rdinp(i_GBP) == 0) && (rdinp(i_LBP) == 0)) || 
																(rdinp(i_CTIN) == 0)) ||
				(rdoutp(o_DC) == 1) || 
				((cons[rear] == 1) && (rdoutp(o_DCR) == 1)))
					setoutp(o_RCM); //cons[RCM_HWDoor] == 1
				else
				{
					clroutp(o_RCM); //cons[RCM_HWDoor] == 1
				}
		}
		else
		{ 
			if ((door_zone == 0) || (hsf == 1))
			{
				if ((rdoutp(o_DC) == 1) || (dirf != 0) ||
					((cons[rear] == 1) && (rdoutp(o_DCR) == 1)))
				{
						setoutp(o_RCM); //cons[RCM_HWDoor] == 1
				}
				else
				{
					clroutp(o_RCM);	 // Retiring Cam  cons[RCM_HWDoor] == 1
				}
			}
			else
			{
				 if ((hsf == 0) && (door_zone == 1) && 
				 	(((procf >= 14) && (procf <= 16)) || ((fvars[fvrcmctl] & 0x02) != 0) || ((fvars[fvpredo] & 0x01) != 0) || ((up_level == 1) && (dn_level == 1))) &&
					(doorf != 3) && (doorf != 4) && ((cons[rear] == 0) || ((rdoorf != 3) && (rdoorf != 4)))  )
				{

					if ((statusf & sf_SAFE) != 0)
						timers[tpredo] = 0;

					if (timers[tpredo] > fvars[fvpred])
						clroutp(o_RCM);	 // Retiring Cam cons[RCM_HWDoor] == 1
					else
 						setoutp(o_RCM);		//	cons[RCM_HWDoor] == 1
				}
				else
				{	  // door is closing or speed greater than 150 fpm
					if ((fvars[fvpredo] & 0x01) == 0)
						timers[tpredo] = 0;
					setoutp(o_RCM);			//cons[RCM_HWDoor] == 1
				}
			}
		}
		return;
	}

	if((cons[mand] == 9) && (auto_rcm != 0) && (chk_ins() == 0))
	{
		if(auto_rcm == 1)
		{
			if(doorf == 0)
				setoutp(o_RCM);
			else
				clroutp(o_RCM);

		}
		else
		{
			if(rdoorf == 0)
				setoutp(o_RCM);
			else
				clroutp(o_RCM);

		}

		return;
	}

	if ((cons[mand] != 0) || (cons[frghtd] != 0))
	{
		if (cons[RCM_HWDoor] == 0)
		{
			if (((manual_door != 0) || (cons[frghtd] != 0))
					&& (ccmsk[cons[carnmb]][position] != 0) 
					&& (((fvars[fvrcmctl] & 0x01) == 0) || (doorf == 1)) )
				drop_front_rcm = 1;
			else
				drop_front_rcm = 0;

			if (((manual_rdoor != 0) || (cons[frghtd] != 0))
			       && (cons[rear] != 0) && (rccmsk[cons[carnmb]][position] != 0) 
			       && (((fvars[fvrcmctl] & 0x01) == 0) || (rdoorf == 1)) )
				drop_rear_rcm = 1;
			else
				drop_rear_rcm = 0;

			if (chk_ins() == 0)
			{		// not on inspection
				if ((((timers[tnit] > fvars[fvhcdt]) && (dpref != 0)) || (relevel_req == 1) || (dirf != 0) ||
					(((procf == 1) || (procf == 21)) && (door_zone == 0) && ( (rdoutp(o_DC) == 1) || (rdoutp(o_DCR) == 1) ) ) ||
							((rhjackf >=2) && (procf == 18)) ) &&
					(car_gate() == 1) && (man_dcc() == 1) &&
					(timers[tmotion] < fvars[fvstall]) &&	   // The car is not stalled
				  	(doorf != 1) && ((rdoorf != 1) || (cons[rear] == 0)))
				{
					timers[trcdrdly] = 0;
					if ((procf >= 14) && (procf <= 16))	
					{
						if((rcm_pick_attempt >= 4) || (rcm_attempt_frgt >= 4) || (RCM_Lock_Fault == 1))
						{
							if(RCM_Lock_Fault == 0)
							{
								fault_device = 1;
								record_fault(f_rcm_lock_off);
								RCM_Lock_Fault = 1;
							}
							else
							{
								if(timers[trcmon] > 400)
									shutdown = 1;
							}
							clroutp(o_RCM);	 // Retiring Cam
						}	
						else if (timers[trcmon] < 300)
						{
							setoutp(o_RCM);	 // Retiring Cam
						}

						else if (timers[trcmon] < 350)
						{
							clroutp(o_RCM);	 // Retiring Cam
						}
						else
						{
							timers[trcmon] = 0;	
							rcm_pick_attempt++;
							if ((cons[frghtd] != 0) && (car_gate() == 1) && (man_dcc() == 1))  // wait until safe
							{
								rcm_attempt_frgt++;
								doorf = 1;
								fault_device = 2;
								record_fault(f_rcm_lock_off);
							}
						 }	
					}
					else	//when car is moving
					{
						timers[trcmon] = 0;//clear timer while in motion so it wont cause a fault while coming to a floor
						setoutp(o_RCM);	 // Retiring Cam
						rcm_pick_attempt = 0; // This line was added because the manual door car was faulting out after five succesful runs (5.52.48)
						rcm_attempt_frgt = 0;
					}
				}
				else if (((timers[trcdrdly] > fvars[fvrcdrdly]) || (doorf == 1) || (rdoorf == 1)) && (door_zone == 1))
				{
					timers[trcmon] = 0;
					rcm_pick_attempt = 0;
					if ((drop_front_rcm == 1) || (drop_rear_rcm == 1))
						clroutp(o_RCM);
					else if (((timers[tnit] > 3000) && (procf >= 14) && (procf <= 16)) || 
								(timers[tmotion] >= fvars[fvstall]))		// greater than 5 minutes (tnit not cleared until dead level)
						clroutp(o_RCM);
				}
			}
			else	// on inspection
			{
				RCM_Lock_Fault = 0;

				
				if (((chk_ins_sw() != 0) || (access_run_req() != 0)) &&
		  			(((car_gate() == 1) && (man_dcc() == 1)) || (access_door_safe() != 0)) &&
				  		(doorf != 1) && ((rdoorf != 1) || (cons[rear] == 0)))
				{
					setoutp(o_RCM);	 // Retiring Cam
				}
				else
				{
					clroutp(o_RCM);
				}
			}
		}
	}
	if(rcm_frght_out_en == 1)
	{
		if (rdoutp(o_RCM) == 1)
		{
			if (timers[trcpkdly] > fvars[fvrcpkdly])
				setoutp(o_RCF);
		}
		else
		{
			timers[trcpkdly] = 0;
			clroutp(o_RCF);
		}
	}
}

void set_scnd_position(void)
{	
static int update_scnd_pos;

	if((up_level == 1) && (dn_level == 1) && (door_zone == 1))
	{
		scnd_pos = position;
	}

	switch(update_scnd_pos)
	{
		case 0: // at floor
			if((up_level == 1) && (dn_level == 1) && (door_zone == 1))
			{
				update_scnd_pos = 1;
			}
			break;
		case 1: // check direction of travel
			if( (dn_level == 0) && (up_level == 1) && (door_zone == 0)) //dn
			{	
				scnd_pos--;
				update_scnd_pos = 0;
			}
			else if((up_level == 0) && (dn_level == 0) && (door_zone == 0))	//up
			{
				update_scnd_pos = 0;
			}
			break;
		default:
			break;
	}
}


//****************************************************
// Monitor individual smoke sensor input per floor  
// and provide with MESO and ALTO outputs
//****************************************************

void smoke_sensor_interface(void)
{
	int16 i;
	int16 smoke_detected = 0;
	
	// Check smoke sensor input per floor
	for(i=1;i<=cons[grtopf];i++)	
	{
		if (rdinp(i_SMK(i)) == 0)
		{
			if (i == fvars[fvfs1f])
				smoke_detected |= 0x01;		// bit 0 = smoke detected at lobby floor
			else
				smoke_detected |= 0x02;		// bit 1 = smoke detected at any other floor than lobby fl
		}
	}
	
	if (smoke_detected == 0)
	{
		setoutp(o_ALTO);
		setoutp(o_MESO);
	}
	else	// detected smoke sensor activated
	{
		// If smoke is detected on main fire floor
		if ((smoke_detected & 0x01) != 0)
			clroutp(o_ALTO);
		else
			setoutp(o_ALTO);

		// If smoke sensor is detected on any other floor other than main fire fl 
		if ((smoke_detected & 0x02) != 0)
			clroutp(o_MESO);
		else
			setoutp(o_MESO);
	}
}


/* Revision History	
	
11/10/16 v8.0.0 mhd		1. Converted from GALaxy 4 software platform and deleted traction parts.
01/24/17 v8.0.1 mhd		1. Added statusf3 and statusf4.				
3/24/17 v8.0.2 mhd		1. Added Down Normal Override for reset jack.
8/14/17 v8.0.7 mhd		1. Added hall lantern comm hl_com().
						2. Use bit in cons[sercom] = 64 (0x40) to enable serial hall lanterns.
8/18/17 V8.0.9 mhd		1. Made the following changes from G4:
						07/06/17 v7.3.05 hl		1. Renamed/Added the following variables: 
														Door_Lock_GS_DPM_Fault (previously was Door_Lock_fault), 
														Door_Lock_fault, Door_Lock_front_fault, Door_Lock_rear_fault, 
														Door_GS_fault,   Door_GS_front_fault,   Door_GS_rear_fault, 
														Door_DPM_fault,  Door_DPM_front_fault,  Door_DPM_rear_fault, 
												2. doors() rdoors() and chkdoor() routines were updated for door lock monitoring logic using booleans above.

*/
