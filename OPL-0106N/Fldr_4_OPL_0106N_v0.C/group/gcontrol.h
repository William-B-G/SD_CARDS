
extern int16 gservf;
			// 0 = Normal Service
			// 1 = Fire Service
			// 2 = Emergency Power
			// 3 = Fire and Emergency Power




struct grp_Car_Stat_struct {
	int16 g_pos;         		// group positions of each car
	int16 g_nxt_stp;     		// group position of next stop for each car
	int16 g_cancel_up;   		// group cancel up call at this floor
	int16 g_cancel_dn;   		// group cancel down call at this floor

	int16 g_procf;      		// group procedure flag for each car
	int16 g_servf;      		// group service flag for each car
	int16 g_empf;       		// group emergency power flag for each car (empf)
	int16 g_firef;      		// group fire service flag for each car
	int16 g_doorf;      		// group door flag for each car
	int16 g_rdoorf;     		// group rear door flag for each car
	int16 g_dirf;       		// group direction flag for each car
	int16 g_dpref;      		// group direction preference flag for each car
	int16 g_codebf;     		// group code blue flag for each car
	int16 g_dcalls;     		// group call above below or no calls
	int16 g_cancel_upr; 		// group cancel up call at this floor
	int16 g_cancel_dnr; 		// group cancel down call at this floor
	int16 g_cur_flt;			// group current fault for each car
	int16 g_control;			// group misc control bits 1 from each car
								// 		bit 0: up peak trigger
								//		bit 1: onward calls
								//		bit 2: no wheelchair capacity
								//		bits 3-15: Unused 
	int16 g_ss_stat;			// group safety string status from car see safety_string_status definitions
	int16 g_cancel_irup;   		// group cancel up call at this floor
	int16 g_cancel_irdn;   		// group cancel down call at this floor
	int16 g_cancel_rirup; 		// group cancel up call at this floor
	int16 g_cancel_rirdn; 		// group cancel down call at this floor
	int16 g_vipf;				// group priority servie (vip) flag
	int16 g_nmbcc;				// number of car calls
	int16 g_topcc;				// top car call
	int16 g_botcc;				// bottom car call
	int16 g_fire_status;		// car fire status sent from each car
	int16 g_ins_status;			// car inspection status sent from each car
	int16 g_pwr_status;			// power status	 see power_status
	int16 g_car_alrm;			// car alarm
	int16 g_car_has_fault;		// car has a fault logged
	int32 g_run_statusf;		// Run status for each car see bits below:
	int32 g_statusf1;			// group status flag for each car 
	int32 g_statusf2;			// additional car status flags
	int32 g_statusf3;			// additional car status flags 
	int32 g_statusf4;			// additional car status flags
	int16 g_dmd_vel;			// group demand velocity in fpm for each car
	int16 g_enc_vel;			// group encoder velocity in fpm for each car
	int16 g_enc_dir;			// group encoder direction for each car
	int32 g_flr_cnt;			// floor reference count for each car
	int32 g_pls_cnt;			// car encoder pulse count for each car

	uint32 g_cc[2];
	uint32 g_rcc[2];
	int16 g_L1_Voltage;
	int16 g_L2_Voltage;
	int16 g_L3_Voltage;
	int16 g_Door_Voltage;
	int16 g_Voltage_Dev_Online;

	int16 g_cancel_trup;   		// group cancel up call at this floor
	int16 g_cancel_trdn;  		// group cancel down call at this floor
	int16 g_cancel_rtrup; 		// group cancel up call at this floor
	int16 g_cancel_rtrdn; 		// group cancel down call at this floor
	
	uint8 g_ins_servf;			// inspection service flag
	
	uint8 g_non_simult_doorf;		// non-simultaneous door flag
	uint8 g_ccpbs_fl_access_req;	// CCPBS Car call floor
	uint8 g_ccpbs_codes[4];			// CCPBS 4 digit security code
	uint8 g_dwellt_reset;			// group door dwell time is reset status
	uint8 g_rdwellt_reset;			// group rear door dwell time is reset status

};

	extern struct grp_Car_Stat_struct grp_Car_Stat[car_size];


extern int16 g_fire_ctrl;				// group fire control to each car 
extern int16 grp_hc_fire_door_open;		// hall call fire door open

extern int16 in_grp_sabbath;		   	// set when in group sabbath operation

extern int16 any_hall_call;		// set to one when group has any hall call
extern int16 any_ir_hall_call;	// set to one when group has any ir or sr hall call in the system

extern int32 xa_uc_ots[2];
extern int32 xa_dc_ots[2];
extern int32 xa_urc_ots[2];
extern int32 xa_drc_ots[2];

extern int32 handicap_car_floor[2];		// Set to 1 to request handicap capacity car at hall call
extern int32 handicap_car_fl_latch[2];		// Set to 1 to latch request for handicap capacity car at hall call

extern unsigned char gpo[40];
extern unsigned char gpi[40];
extern unsigned char prev_gpi[40];
extern unsigned char cgpo[40];		// car ouput to group gpo
extern unsigned char cgpi[40];		// car input from gpi

extern uint8 Car_ots[9];
extern uint8 Car_ots_nbp[9];

extern bool em_disp_hc;

	// group service activation variables 

extern int16 tmr_svc[17];
extern int16 tmr_svc_en[17];
extern int16 tmr_svc_logic[17];
#define s_none 				0
#define s_first				1					// first service
#define s_parking 			1					// Parking
#define s_alt_park			2					// alternate parking floor
#define s_ncu				3					// next car up
#define s_uppeak 			4					// up peak
#define s_dnpeak			5					// down peak
#define s_alt_lby			6					// alternate lobby
#define s_alt_fl_sec		7					// alternate floor security
#define s_cc_lockouts		8					// Car call lockouts
#define s_gcc_lockouts 		9					// Group car call lockouts
#define s_ccl_override 		10					// Car call lockout override
#define s_gccl_override 	11					// Group car call lockout override
#define s_sabbath 			12					// Sabbath service
#define s_cc_pb_sec			13					// Car call push button security
#define s_fl_sec_tbl2		14 					// floor security table 2
#define s_fl_sec_tbl3		15 					// floor security table 3
#define s_fl_sec_tbl4		16 					// floor security table 4
#define s_last				16					// last service

// Hall and Car call color variables

extern int16 HcbOnBright;		// Hall Call On Brightness
extern int16 HcbOffBright;		// Hall Call Off Brightness

extern int16 HcbUpOnRed;		// Hall Call Up On Red
extern int16 HcbUpOnGreen;		// Hall Call Up On Green
extern int16 HcbUpOnBlue;		// Hall Call Up On Blue

extern int16 HcbUpOffRed;		// Hall Call Up Off Red
extern int16 HcbUpOffGreen;		// Hall Call Up Off Green
extern int16 HcbUpOffBlue;		// Hall Call Up Off Blue

extern int16 HcbDnOnRed;		// Hall Call Down On Red
extern int16 HcbDnOnGreen;	  	// Hall Call Down On Green
extern int16 HcbDnOnBlue;	  	// Hall Call Down On Blue

extern int16 HcbDnOffRed;		// Hall Call Down Off Red
extern int16 HcbDnOffGreen;	  	// Hall Call Down Off Green
extern int16 HcbDnOffBlue;	  	// Hall Call Down Off Blue

extern int16 HcbSecOnRed;		// Hall Call Security On Red
extern int16 HcbSecOnGreen;		// Hall Call Security On Green
extern int16 HcbSecOnBlue;	  	// Hall Call Security On Blue

extern int16 HcbCBOnRed;		// Hall Call CB On red intensity
extern int16 HcbCBOnGreen;	  	// Hall Call CB On green intensity
extern int16 HcbCBOnBlue;	  	// Hall Call CB On blue intensity

extern int16 HcbCBOffRed;		// Hall Call CB Off red intensity
extern int16 HcbCBOffGreen;	  	// Hall Call CB Off green intensity
extern int16 HcbCBOffBlue;	  	// Hall Call CB Off blue intensity

extern int16 HcbVipOnRed;		// Hall Call Vip On red intensity
extern int16 HcbVipOnGreen;	  	// Hall Call Vip On green intensity
extern int16 HcbVipOnBlue;	  	// Hall Call Vip On blue intensity

extern int16 HcbVipOffRed;		// Hall Call Vip Off red intensity
extern int16 HcbVipOffGreen;   	// Hall Call Vip Off green intensity
extern int16 HcbVipOffBlue;	  	// Hall Call Vip Off blue intensity

extern int16 HcbIRUpOnRed;		// Hall Call IR Up On red intensity
extern int16 HcbIRUpOnGreen;	// Hall Call IR Up On green intensity
extern int16 HcbIRUpOnBlue;	  	// Hall Call IR Up On blue intensity

extern int16 HcbIRDnOnRed;		// Hall Call IR Dn On red intensity
extern int16 HcbIRDnOnGreen;	// Hall Call IR Dn On green intensity
extern int16 HcbIRDnOnBlue;	  	// Hall Call IR Dn On blue intensity

extern int16 HcbIRUpOffRed;		// Hall Call IR Up Off red intensity
extern int16 HcbIRUpOffGreen;	// Hall Call IR Up Off green intensity
extern int16 HcbIRUpOffBlue;	// Hall Call IR Up Off blue intensity

extern int16 HcbIRDnOffRed;		// Hall Call IR Dn Off red intensity
extern int16 HcbIRDnOffGreen;	// Hall Call IR Dn Off green intensity
extern int16 HcbIRDnOffBlue;   	// Hall Call IR Dn Off blue intensity


extern int16 HcbOnBright_prev;		
extern int16 HcbOffBright_prev;		

extern int16 HcbUpOnRed_prev;
extern int16 HcbUpOnGreen_prev;
extern int16 HcbUpOnBlue_prev;

extern int16 HcbUpOffRed_prev;
extern int16 HcbUpOffGreen_prev;
extern int16 HcbUpOffBlue_prev;

extern int16 HcbSecOnRed_prev;
extern int16 HcbSecOnGreen_prev;
extern int16 HcbSecOnBlue_prev;

extern int16 HcbDnOnRed_prev;		// Hall Call Dn On red intensity
extern int16 HcbDnOnGreen_prev;		// Hall Call Dn On green intensity
extern int16 HcbDnOnBlue_prev;		// Hall Call Dn On blue intensity

extern int16 HcbDnOffRed_prev;		// Hall Call Dn Off red intensity
extern int16 HcbDnOffGreen_prev;	// Hall Call Dn Off green intensity
extern int16 HcbDnOffBlue_prev;		// Hall Call Dn Off blue intensity

extern int16 HcbCBOnRed_prev;		// Hall Call CB On red intensity
extern int16 HcbCBOnGreen_prev;	  	// Hall Call CB On green intensity
extern int16 HcbCBOnBlue_prev;	  	// Hall Call CB On blue intensity

extern int16 HcbCBOffRed_prev;		// Hall Call CB Off red intensity
extern int16 HcbCBOffGreen_prev;	// Hall Call CB Off green intensity
extern int16 HcbCBOffBlue_prev;	  	// Hall Call CB Off blue intensity

extern int16 HcbVipOnRed_prev;		// Hall Call Vip On red intensity
extern int16 HcbVipOnGreen_prev;	// Hall Call Vip On green intensity
extern int16 HcbVipOnBlue_prev;	  	// Hall Call Vip On blue intensity

extern int16 HcbVipOffRed_prev;		// Hall Call Vip Off red intensity
extern int16 HcbVipOffGreen_prev;	// Hall Call Vip Off green intensity
extern int16 HcbVipOffBlue_prev;  	// Hall Call Vip Off blue intensity

extern int16 HcbIRUpOnRed_prev;		// Hall Call IR Up On red intensity
extern int16 HcbIRUpOnGreen_prev;	// Hall Call IR Up On green intensity
extern int16 HcbIRUpOnBlue_prev;	// Hall Call IR Up On blue intensity

extern int16 HcbIRDnOnRed_prev;		// Hall Call IR Dn On red intensity
extern int16 HcbIRDnOnGreen_prev;	// Hall Call IR Dn On green intensity
extern int16 HcbIRDnOnBlue_prev;	// Hall Call IR Dn On blue intensity

extern int16 HcbIRUpOffRed_prev;		// Hall Call IR Up Off red intensity
extern int16 HcbIRUpOffGreen_prev;	// Hall Call IR Up Off green intensity
extern int16 HcbIRUpOffBlue_prev;	// Hall Call IR Up Off blue intensity

extern int16 HcbIRDnOffRed_prev;		// Hall Call IR Dn Off red intensity
extern int16 HcbIRDnOffGreen_prev;	// Hall Call IR Dn Off green intensity
extern int16 HcbIRDnOffBlue_prev;	// Hall Call IR Dn Off blue intensity

extern int16 ColorUpHCChecksum;
extern int16 ColorDnHCChecksum;
extern int16 ColorBrightChecksum;
extern int16 ColorSecChecksum;
extern int16 ColorCBOnChecksum;
extern int16 ColorCBOffChecksum;
extern int16 ColorVipOnChecksum;
extern int16 ColorVipOffChecksum;
extern int16 ColorIRUpChecksum;
extern int16 ColorIRDnChecksum;


extern int16 asvtof[9];
extern int16 asvto_pos[9];
extern int16 asv_penalty_tmr[9];

enum {
 tgs_sec,			// General Purpose second timer
 tgs_wd,			// watchdog timer
 tgs_nmb			// Must be last
};

extern int16 timer_grp_sec[tgs_nmb];


extern void group_control (void);
extern void init_group_control (void);
