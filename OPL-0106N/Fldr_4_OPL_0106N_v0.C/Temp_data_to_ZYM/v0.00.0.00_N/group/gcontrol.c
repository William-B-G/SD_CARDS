//**************************************************************
// Main call for all group routines
//**************************************************************


#define d_GCONTROL 1
#include "global.h"

#define ServiceTimer 1

int16 gservf = 0;
			// 0 = Normal Service
			// 1 = Fire Service
			// 2 = Emergency Power
			// 3 = Fire and Emergency Power


struct grp_Car_Stat_struct {
	int16 g_pos;        	 	// group positions of each car
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
	int16 g_dcalls;    			// group call above below or no calls
	int16 g_cancel_upr;			// group cancel up call at this floor
	int16 g_cancel_dnr; 		// group cancel down call at this floor
	int16 g_cur_flt;			// group current fault for each car
	int16 g_control;			// group misc control bits 1 from each car			  18
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
	int16 g_car_has_fault;		// car has a fault logged					  	32
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

	uint32 g_cc[2];				// Car calls for each car
	uint32 g_rcc[2];			// Rear car calls for each car
	int16 g_L1_Voltage;		
	int16 g_L2_Voltage;		 
	int16 g_L3_Voltage;
	int16 g_Door_Voltage;
	int16 g_Voltage_Dev_Online;

	int16 g_cancel_trup;   		// group cancel up call at this floor
	int16 g_cancel_trdn;  		// group cancel down call at this floor
	int16 g_cancel_rtrup; 		// group cancel up call at this floor
	int16 g_cancel_rtrdn; 		// group cancel down call at this floor		  51
	
	uint8 g_ins_servf;			// inspection service flag
	
	uint8 g_non_simult_doorf;		// non-simultaneous door flag
	uint8 g_ccpbs_fl_access_req;	// CCPBS Car call floor
	uint8 g_ccpbs_codes[4];			// CCPBS 4 digit security code
	uint8 g_dwellt_reset;			// group door dwell time is reset status
	uint8 g_rdwellt_reset;			// group rear door dwell time is reset status

};

struct grp_Car_Stat_struct grp_Car_Stat[car_size];
	

int16 g_fire_ctrl = 0;				// group fire control to each car 
int16 grp_hc_fire_door_open = 0;		// hall call fire door open

int16 in_grp_sabbath = 0;				// set when in group sabbath operation

int16 any_hall_call = 0;				// set to one when group has any hall call in the system
int16 any_ir_hall_call = 0;			// set to one when group has any ir or sr hall call in the system

uint8 Car_ots[9] = {0,0,0,0,0,0,0,0,0};
uint8 Car_ots_nbp[9] = {0,0,0,0,0,0,0,0,0};
uint8 update_ots_time = 0;

uint16 car_index;


int16 asv_timer[9] = {0,0,0,0,0,0,0,0,0};
int16 run_asv_timer[9] = {0,0,0,0,0,0,0,0,0};
int16 asv_prev_pos[9] = {0,0,0,0,0,0,0,0,0};
int16 asv_penalty_tmr[9] = {0,0,0,0,0,0,0,0,0};	// Auto Service Time-Out penalty Timer
int16 asv_penalty_f = 0;						// penalty time flag 0=increment, 1=decrement


int32 xa_uc_ots[2] = {0,0};
int32 xa_dc_ots[2] = {0,0};
int32 xa_urc_ots[2] = {0,0};
int32 xa_drc_ots[2] = {0,0};

int32 handicap_car_floor[2] = {0,0};		// Set to 1 to request handicap capacity car at hall call
int32 handicap_car_fl_latch[2] = {0,0};		// Set to 1 to latch request for handicap capacity car at hall call


// Group I/O array
unsigned char gpo[40] = {0,0,0,0,0,0,0,0,0,0,		// group output data
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0};
unsigned char gpi[40] = {0,0,0,0,0,0,0,0,0,0,		// group input data
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0};
unsigned char prev_gpi[40] = {0,0,0,0,0,0,0,0,0,0,		// group input data
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0};

unsigned char cgpo[40] = {0,0,0,0,0,0,0,0,0,0,		// car ouput to group gpo
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0};
unsigned char cgpi[40] = {0,0,0,0,0,0,0,0,0,0,		// car input from gpi
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0};


bool em_disp_hc = 0;


	// group service activation variables 
	//   *** check nmbsvct in utils.h
	//   *** check c_tmr_svc[17] in control
int16 tmr_svc[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int16 tmr_svc_en[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int16 tmr_svc_logic[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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



int16 asvtof[9]={0,0,0,0,0,0,0,0,0};	// auto service Time-out flag
int16 asvto_pos[9] = {0,0,0,0,0,0,0,0,0};

// Hall call color variables

int16 HcbOnBright=0;		// Hall Call On Brightness
int16 HcbOffBright=0;		// Hall Call Off Brightness

int16 HcbUpOnRed=0;			// Hall Call Up On Red
int16 HcbUpOnGreen=0;		// Hall Call Up On Green
int16 HcbUpOnBlue=0; 		// Hall Call Up On Blue

int16 HcbUpOffRed=0;		// Hall Call Off Red
int16 HcbUpOffGreen=0;		// Hall Call Off Green
int16 HcbUpOffBlue=0;		// Hall Call Off Blue

int16 HcbDnOnRed=0;			// Hall Call Down On Red
int16 HcbDnOnGreen=0;	  	// Hall Call Down On Green
int16 HcbDnOnBlue=0;	  	// Hall Call Down On Blue

int16 HcbDnOffRed=0;		// Hall Call Down Off Red
int16 HcbDnOffGreen=0;		// Hall Call Down Off Green
int16 HcbDnOffBlue=0;		// Hall Call Down Off Blue

int16 HcbSecOnRed=0;		// Hall Call Security On Red
int16 HcbSecOnGreen=0;		// Hall Call Security On Green
int16 HcbSecOnBlue=0;		// Hall Call Security On Blue

int16 HcbCBOnRed=0;			// Hall Call CB On red intensity
int16 HcbCBOnGreen=0;	  	// Hall Call CB On green intensity
int16 HcbCBOnBlue=0;	  	// Hall Call CB On blue intensity

int16 HcbCBOffRed=0;		// Hall Call CB Off red intensity
int16 HcbCBOffGreen=0;	  	// Hall Call CB Off green intensity
int16 HcbCBOffBlue=0;	  	// Hall Call CB Off blue intensity

int16 HcbVipOnRed=0;		// Hall Call Vip On red intensity
int16 HcbVipOnGreen=0;	  	// Hall Call Vip On green intensity
int16 HcbVipOnBlue=0;	  	// Hall Call Vip On blue intensity

int16 HcbVipOffRed=0;		// Hall Call Vip Off red intensity
int16 HcbVipOffGreen=0;	  	// Hall Call Vip Off green intensity
int16 HcbVipOffBlue=0;	  	// Hall Call Vip Off blue intensity

int16 HcbIRUpOnRed=0;		// Hall Call IR Up On red intensity
int16 HcbIRUpOnGreen=0;	  	// Hall Call IR Up On green intensity
int16 HcbIRUpOnBlue=0;	  	// Hall Call IR Up On blue intensity

int16 HcbIRDnOnRed=0;		// Hall Call IR Dn On red intensity
int16 HcbIRDnOnGreen=0;	  	// Hall Call IR Dn On green intensity
int16 HcbIRDnOnBlue=0;	  	// Hall Call IR Dn On blue intensity

int16 HcbIRUpOffRed=0;		// Hall Call IR Up Off red intensity
int16 HcbIRUpOffGreen=0;	// Hall Call IR Up Off green intensity
int16 HcbIRUpOffBlue=0;	  	// Hall Call IR Up Off blue intensity

int16 HcbIRDnOffRed=0;		// Hall Call IR Dn Off red intensity
int16 HcbIRDnOffGreen=0;	// Hall Call IR Dn Off green intensity
int16 HcbIRDnOffBlue=0;	  	// Hall Call IR Dn Off blue intensity

int16 HcbOnBright_prev=0;		
int16 HcbOffBright_prev=0;		

int16 HcbUpOnRed_prev = 0;
int16 HcbUpOnGreen_prev = 0;
int16 HcbUpOnBlue_prev = 0;

int16 HcbUpOffRed_prev = 0;
int16 HcbUpOffGreen_prev = 0;
int16 HcbUpOffBlue_prev = 0;

int16 HcbSecOnRed_prev = 0;
int16 HcbSecOnGreen_prev = 0;
int16 HcbSecOnBlue_prev = 0;

int16 HcbDnOnRed_prev = 0;		// Hall Call Dn On red intensity
int16 HcbDnOnGreen_prev = 0;	// Hall Call Dn On green intensity
int16 HcbDnOnBlue_prev = 0;		// Hall Call Dn On blue intensity

int16 HcbDnOffRed_prev = 0;		// Hall Call Dn Off red intensity
int16 HcbDnOffGreen_prev = 0;	// Hall Call Dn Off green intensity
int16 HcbDnOffBlue_prev = 0;	// Hall Call Dn Off blue intensity

int16 HcbCBOnRed_prev=0;		// Hall Call CB On red intensity
int16 HcbCBOnGreen_prev=0;	  	// Hall Call CB On green intensity
int16 HcbCBOnBlue_prev=0;	  	// Hall Call CB On blue intensity

int16 HcbCBOffRed_prev=0;		// Hall Call CB Off red intensity
int16 HcbCBOffGreen_prev=0;	  	// Hall Call CB Off green intensity
int16 HcbCBOffBlue_prev=0;	  	// Hall Call CB Off blue intensity

int16 HcbVipOnRed_prev=0;		// Hall Call Vip On red intensity
int16 HcbVipOnGreen_prev=0;	  	// Hall Call Vip On green intensity
int16 HcbVipOnBlue_prev=0;	  	// Hall Call Vip On blue intensity

int16 HcbVipOffRed_prev=0;		// Hall Call Vip Off red intensity
int16 HcbVipOffGreen_prev=0;  	// Hall Call Vip Off green intensity
int16 HcbVipOffBlue_prev=0;	  	// Hall Call Vip Off blue intensity

int16 HcbIRUpOnRed_prev=0;		// Hall Call IR Up On red intensity
int16 HcbIRUpOnGreen_prev=0;	// Hall Call IR Up On green intensity
int16 HcbIRUpOnBlue_prev=0;	  	// Hall Call IR Up On blue intensity

int16 HcbIRDnOnRed_prev=0;		// Hall Call IR Dn On red intensity
int16 HcbIRDnOnGreen_prev=0;	// Hall Call IR Dn On green intensity
int16 HcbIRDnOnBlue_prev=0;	  	// Hall Call IR Dn On blue intensity

int16 HcbIRUpOffRed_prev=0;		// Hall Call IR Up Off red intensity
int16 HcbIRUpOffGreen_prev=0;	// Hall Call IR Up Off green intensity
int16 HcbIRUpOffBlue_prev=0;	// Hall Call IR Up Off blue intensity

int16 HcbIRDnOffRed_prev=0;		// Hall Call IR Dn Off red intensity
int16 HcbIRDnOffGreen_prev=0;	// Hall Call IR Dn Off green intensity
int16 HcbIRDnOffBlue_prev=0;	// Hall Call IR Dn Off blue intensity

int16 ColorUpHCChecksum = 0;
int16 ColorDnHCChecksum = 0;
int16 ColorBrightChecksum = 0;
int16 ColorSecChecksum = 0;
int16 ColorCBOnChecksum = 0;
int16 ColorCBOffChecksum = 0;
int16 ColorVipOnChecksum = 0;
int16 ColorVipOffChecksum = 0;
int16 ColorIRUpChecksum = 0;
int16 ColorIRDnChecksum = 0;


enum {
 tgs_sec,			// General Purpose second timer
 tgs_wd,			// watchdog timer
 tgs_nmb			// Must be last
};

int16 timer_grp_sec[tgs_nmb];


// Public function declarations

void group_control (void);
void group_car_ots(void);
void set_asv_timer_run (void);
void service_timer_control (void);
void inctime_group (void);
void init_group_control (void);


void init_group_control (void)
{
	timer_grp_sec[tgs_wd] = 0;
}

//**************************************************************
// Group Execution Control routines
//**************************************************************

void group_control (void)
{
	int16 i;
  	int16 temp_g_fire_ctrl;

  	if (cons[carnmb] == Dispatcher)
  	{
  	
  		inctime_group();
			
			
#if (ServiceTimer == 1)
		service_timer_control();
#endif
		group_car_ots();		// set the out of service cars
		gservf = 0;
		g_fire_ctrl = 0;
		temp_g_fire_ctrl = 0;
		for(i=1;i<=cons[nmbcars];i++)
		{
			if (Car_Not_Talking[i] == 0)
			{
				temp_g_fire_ctrl |= (grp_Car_Stat[i].g_fire_status & 0x7F);		// fire status from each car online
			}
				
			if(temp_g_fire_ctrl !=0)
			{
				if(timers[tfs1_grp] > 40) // wait four seconds
				{	
					g_fire_ctrl = temp_g_fire_ctrl;
				}
			}


			if (grp_Car_Stat[i].g_firef != 0)
				gservf |= 1;
			
			if ((grp_Car_Stat[i].g_empf & 0x07) != no_ep)
				gservf |= 2;
		}

		if(temp_g_fire_ctrl == 0)
		{
			timers[tfs1_grp] = 0;
		}
	
		if ((gservf & 0x01) != 0)	// at least one car is on fire service
		{
			if (fvars[fvdenfsv] == 1)
			{
				grp_hc_fire_door_open = (((gc_dc[fire_floor/32] & fl_mask[fire_floor]) != 0) || ((gc_uc[fire_floor/32] & fl_mask[fire_floor]) != 0));
			}
		}


#if ((Traction == 1) || (Tract_OL == 1))
		if (cons[Emp_Recover] == 0)
		{
			if (cons[EPSF] != 0)
			{
				if (cons[carnmb] == 1)			// only car 1 does both feeders
			  		group_em_power(0);
	  			group_em_power(1);				// car 2 does second split feeder if the dispatcher
			}
			else
		  		group_em_power(0);				//	no split feeder 
		}
#else
		// Hydro emergency power control
		if (cons[EMPop] != 0)
		{
		  	group_em_power();					// no split feeder on hydro
		}
		else if ((cons[EMPop] & 0x03) == 0)
		{
	  		// Hydro car has it's own power for lowering
		  	if (g_rdcinp(cons[carnmb],i_EMP) == 1)
		  	{
				if(g_rdcinp(cons[carnmb], i_EPS) == 1)
					gempf = ep_switch;
				else
				{
			  		if (((empf == ep_home) || (empf == ep_home_close)) && (position == ep_recall_fl))
			  			gempf = ep_home;
			  		else
			  			gempf = ep_recall;
				}
		  	}
			else
			{
				gempf = no_ep;
			}
			grp_empf[cons[carnmb]] = gempf;
			grp_empf[0] = gempf;
		}
		

#endif
		if (cons[cblue] == 1)
			code_blue();

		if (cons[VipSvc] != 0)
			vip_recall();
		
		high_priority_fl();

		if (fvars[fvdisopphc] != 0)
			hc_sec_timers();

#if ((Traction == 1) || (Tract_OL == 1))

		if (cons[nmbcars] > 1)
		{
			group_dn_peak();
			group_up_peak();
		}
		else
		{
			local_up_peak = 0;
			up_peak = 0;
			dn_peak = 0;
		}
#endif
		group_lobby_request();
		group_nextup();
	  	group_park();
	  	group_hallcall();
	  	group_hallcancel();
		if ((Grp_CC_Lockouts == 1) || (Grp_HC_Lockouts == 1) || (cons[ids] != 0))
			group_lockouts();
		if ((fvars[fvxasgn] & 0x05) != 0)
			hall_call_cross_assignment();
		if (Grp_CC_Override == 1)
			grp_cc_override();

		if (cons[SecFlCfg] == 2)
			group_swap_fl_tbl();

		if ((cons[SecFlCfg] == 3) || (cons[SecFlCfg] == 7))
			group_sec_car_fl_keysw();
		
		if (cons[SecFlCfg] == 6)
			group_swap_fl_tbl_2();

  	}
  	else
  	{			// group is not running on this car
   	//	SIU.GPDO[Z0_LED].R = 0;// tagwbg 14		// turn led on solid
  	
	  	recall_seq = 0;
	  	select_seq = 0;
	  	man_sel_seq = 0;
	  	for (i=1;i<=cons[nmbcars];i++)
			grp_empf[i] = no_ep;
  	
 		strtup_secf_rst_msks = 0;	// on non dispacher car reset
 	}
	if (alt_lobby == 1)
		lobby_fl = fvars[fvaltlob];
	else
		lobby_fl = fvars[fvlob];


}


//***************************************
void inctime_group(void)
{
	int16 i;
	static bool led1_stat;

  	if (timer_grp_sec[tgs_sec] > 0)
  	{
  		timer_grp_sec[tgs_sec] = 0;
		for (i=1;i<tgs_nmb;i++)
		{
			if (timer_grp_sec[i] < 32000)
				timer_grp_sec[i]++;
		}
  	}
  	


	if ((fvars[fvcputiming] & GRP_INCTIM) == 0)
	{
		if (timer_grp_sec[tgs_wd] == 1)
		{
//	   		SIU.GPDO[Z0_LED].R = 0; // tagwbg 14
		}
		if (timer_grp_sec[tgs_wd] >= 2)
		{
			timer_grp_sec[tgs_wd] = 0;	
//		  	SIU.GPDO[Z0_LED].R = 1;// tagwbg 14
		}
	}
	else 
	{
	   	if (led1_stat == 1)
	   	{
	   		led1_stat = 0;
//	   		SIU.GPDO[Z0_LED].R = 0;// tagwbg 14
	   	}
	   	else 
	   	{    		
			led1_stat = 1;
//		  	SIU.GPDO[Z0_LED].R = 1;// tagwbg 14
		}
	}
}

//***************************************
// Set the auto service timer run status
//***************************************

void set_asv_timer_run (void)
{
	int16 fl,el;

	for(el=1; el<=cons[nmbcars]; el++)
		run_asv_timer[el] = 0;

  	for(fl=1; fl < cons[grtopf]; fl++)
  	{
		if ((g_asign_up[fl] >= 1) && (g_asign_up[fl] <= cons[nmbcars]))
		{
			if ((grp_Car_Stat[g_asign_up[fl]].g_pos == fl) && ((grp_Car_Stat[g_asign_up[fl]].g_run_statusf & rs_RUN) == 0))
				run_asv_timer[g_asign_up[fl]] = 1;
		}
		if ((g_asign_upr[fl] >= 1) && (g_asign_upr[fl] <= cons[nmbcars]))
		{
			if ((grp_Car_Stat[g_asign_upr[fl]].g_pos == fl) && ((grp_Car_Stat[g_asign_upr[fl]].g_run_statusf & rs_RUN) == 0))
				run_asv_timer[g_asign_upr[fl]] = 1;
		}
	}
  	for(fl=2; fl <= cons[grtopf]; fl++)
  	{
		if ((g_asign_dn[fl] >= 1) && (g_asign_dn[fl] <= cons[nmbcars]))
		{
			if ((grp_Car_Stat[g_asign_dn[fl]].g_pos == fl) && ((grp_Car_Stat[g_asign_dn[fl]].g_run_statusf & rs_RUN) == 0))
				run_asv_timer[g_asign_dn[fl]] = 1;
		}
		if ((g_asign_dnr[fl] >= 1) && (g_asign_dnr[fl] <= cons[nmbcars]))
		{
			if ((grp_Car_Stat[g_asign_dnr[fl]].g_pos == fl) && ((grp_Car_Stat[g_asign_dnr[fl]].g_run_statusf & rs_RUN) == 0))
				run_asv_timer[g_asign_dnr[fl]] = 1;
		}
	}

	for(el=1; el<=cons[nmbcars]; el++)
	{
		if (asv_prev_pos[el] != grp_Car_Stat[el].g_pos)
		{		  // car is in service if the position is changing
			run_asv_timer[el] = 0;
			asv_prev_pos[el] = grp_Car_Stat[el].g_pos;
		}
		else if ((grp_Car_Stat[el].g_dpref != 0) && ((grp_Car_Stat[el].g_run_statusf & rs_RUN) == 0))
			run_asv_timer[el] = 1;
	}
}

// ****************************************************
// Determine which cars are out of service in the group
// ****************************************************

void group_car_ots(void)
{			  
	int16 i;

//	set_asv_timer_run();

	for(i=1; i<=cons[nmbcars]; i++)
  	{                             // checking to see which cars are in service

		Car_ots[i] = 0;
		Car_ots_nbp[i] = 0;
		run_asv_timer[i] = 0;

		if((grp_Car_Stat[i].g_servf != s_AUTO) && (grp_Car_Stat[i].g_servf != s_HOMING) && (grp_Car_Stat[i].g_servf != s_ATT))
		{
			if (grp_Car_Stat[i].g_servf != s_LWBP)
				Car_ots_nbp[i] = 1;			// Car not on load bypass so out of service by other means
			Car_ots[i] = 1;
		}

		if((grp_Car_Stat[i].g_firef != 0) || (grp_Car_Stat[i].g_codebf != 0) || (Car_Not_Talking[i] == 1))
		{
			Car_ots[i] = 1;
			Car_ots_nbp[i] = 1;
			if (Car_Not_Talking[i] == 1)
				grp_Car_Stat[i].g_servf = 0;
		}

		if ((fvars[fvautosvctmout] != 0) && (grp_Car_Stat[i].g_servf == s_AUTO) && (Car_ots_nbp[i] == 0))
		{	// if auto-service time out enabled and car is on automatic service

			if (((grp_Car_Stat[i].g_run_statusf & rs_RUN)  == 0) && ((grp_Car_Stat[i].g_procf < 3) || (grp_Car_Stat[i].g_procf > 11)))
			{	// car not running/moving
				run_asv_timer[i] = 1;	// enable timer
			}
			
			if (((grp_Car_Stat[i].g_dcalls != 0) || ((grp_Car_Stat[i].g_dcalls == 0) && (asvtof[i] == 1)))
				&& (run_asv_timer[i] == 1))	// car not running/moving and has hall calls or asv t-out flag is set
			{				
				
				if ((update_ots_time == 1) && (timers[tsec] != 0))
				{	// increment one sec timer

					if (asv_timer[i] < 32000)
						asv_timer[i]++;
				
					// set starting panelty time	
					if (asv_timer[i] == fvars[fvautosvctmouttm])
						asv_penalty_tmr[i] = fvars[fvautosvctmouttm];

					if (asvtof[i] == 1)
					{
						if (asv_penalty_f == 0)
						{	// increment penalty timer
							if (asv_penalty_tmr[i] < 255) 
								asv_penalty_tmr[i]++;
							else
								asv_penalty_f = 1;
						}
						else
						{	// decrement penalty timer
							if (asv_penalty_tmr[i] > 0)
								asv_penalty_tmr[i]--;
							else
								asv_penalty_f = 0;
						}
					}

				}
			
				if (asv_timer[i] > fvars[fvautosvctmouttm])

				{	// car timed out set the auto service time-out flag
					asvtof[i] = 1;
					if (asvto_pos[i] == 0)
					{
						asvto_pos[i] = grp_Car_Stat[i].g_pos;
						record_fault((uint16)(f_asv_to_car+i));
					}
					else if (grp_Car_Stat[i].g_pos != asvto_pos[i])
					{				// reset the auto service time-out

						asvto_pos[i] = 0;
					}
				}
			}	
			else
			{	// no more hall calls and auto svc time-out flag is off or car has moved
				asvtof[i] = 0;
				asv_timer[i] = 0;
				asv_penalty_tmr[i] = 0;
			}
		}
		else	// when the fvars[fvautosvctmout] is disabled or car is not on automatic service

		{
			asvtof[i] = 0;
			asv_timer[i] = 0;
			asv_penalty_tmr[i] = 0;
		}		


	}
	if (fvars[fvautosvctmout] != 0)
	{
		if (timers[tsec] == 0)
			update_ots_time = 1;
		else
			update_ots_time = 0;
	}

	all_cars_ots = 1;
	nmb_cars_in_svc = 0;
	for(i=1; i<=cons[nmbcars]; i++)
	{                    // checking to see if all cars are out of service
		if(Car_ots_nbp[i] == 0)
		{
			nmb_cars_in_svc++;
			all_cars_ots = 0;
		}
	}
	
	if (nmb_cars_in_svc > 0)
		gout_GSERV = 1;
	else
		gout_GSERV = 0;
}


// *********************************************
// ***** Group Timer Service Activation Control
// *********************************************

void service_timer_control (void)
{
	int16 i,j;
	bool set_month_day;
	static int16 second_cnt;

	
	if (timers[tsec] != 0) 
	{
		if ((second_cnt & 0x01) == 0)
			second_cnt ++;
	}
	else
	{
		if ((second_cnt & 0x01) != 0)
			second_cnt ++;
	}

	if (second_cnt >= 10)
	{
		second_cnt = 0;

//		local_getdate();		// cannot be called from the group, the car will update the t and d structures
//		local_gettime();

		for (i=0;i<=s_last;i++)
		{
			tmr_svc[i] = 0;
			tmr_svc_en[i] = 0;
		}
		for (i = 0; i < 10; i++)
		{
										  
			set_month_day = false;
			svc_t[i].status &= 0xFE;		// Clear the on bit
			// Process month/day timers
			for (j = 0; j <= 2; j++)
			{
				if ((svc_t[i].month[j] != 0) && (svc_t[i].day[j] != 0))
				{
					if ((svc_t[i].month[j] == d.month) && (svc_t[i].day[j] == d.day)) 
					{
						set_month_day = true;

						if ( (svc_t[i].md_off_hour[j] < svc_t[i].md_on_hour[j]) || ((svc_t[i].md_off_hour[j] == svc_t[i].md_on_hour[j]) && (svc_t[i].md_off_minute[j] < svc_t[i].md_on_minute[j])) )
						{
							if ( ((t.hour > svc_t[i].md_on_hour[j]) || ((t.hour == svc_t[i].md_on_hour[j]) && (t.minute >= svc_t[i].md_on_minute[j]))) ||
								 ((t.hour < svc_t[i].md_off_hour[j]) || ((svc_t[i].md_off_hour[j] == t.hour) && (t.minute <= svc_t[i].md_off_minute[j]))) )
								svc_t[i].status |= 0x01;   // set the on bit
						}
						else if ( ((t.hour > svc_t[i].md_on_hour[j]) || ((t.hour == svc_t[i].md_on_hour[j]) && (t.minute >= svc_t[i].md_on_minute[j]))) &&
							 ((t.hour < svc_t[i].md_off_hour[j]) || ((svc_t[i].md_off_hour[j] == t.hour) && (t.minute <= svc_t[i].md_off_minute[j]))) )
							svc_t[i].status |= 0x01;   // set the on bit
					}
				}
			}

			// Process the daily timers
			if (set_month_day == false) 
			{
				for (j = 0; j <= 6; j++)
				{
					if (j == d.dayofweek)
					{
						if ((svc_t[i].on_hour[j] != 0) ||  (svc_t[i].on_minute[j] != 0) || (svc_t[i].off_hour[j] != 0) || (svc_t[i].off_minute[j] != 0))
						{

							if ( (svc_t[i].off_hour[j] < svc_t[i].on_hour[j]) || ((svc_t[i].off_hour[j] == svc_t[i].on_hour[j])	&& (svc_t[i].off_minute[j] < svc_t[i].on_minute[j])) )
							{
								if ( ((t.hour > svc_t[i].on_hour[j]) || ((t.hour == svc_t[i].on_hour[j]) && (t.minute >= svc_t[i].on_minute[j]))) ||
									 ((t.hour < svc_t[i].off_hour[j]) || ((t.hour == svc_t[i].off_hour[j]) && (t.minute <= svc_t[i].off_minute[j]))) )
									svc_t[i].status |= 0x01;  // set the on bit
							}
							else if ( ((t.hour > svc_t[i].on_hour[j]) || ((t.hour == svc_t[i].on_hour[j]) && (t.minute >= svc_t[i].on_minute[j]))) &&
								 	  ((t.hour < svc_t[i].off_hour[j]) || ((t.hour == svc_t[i].off_hour[j]) && (t.minute <= svc_t[i].off_minute[j]))) )
							{
								svc_t[i].status |= 0x01;  // set the on bit
							}
						}
					}
				}
			}

			if ((svc_t[i].service >= s_first) && (svc_t[i].service <= s_last))
			{					// Valid service set
				if ((svc_t[i].status & 0x02) != 0)		// Timer enabled (0x02)
					tmr_svc_en[svc_t[i].service] = 1;		// Turn on service enable	
				if ((svc_t[i].status & 0x03) == 0x03)		// Timer is on (0x01) and enabled (0x02)  
				{
					if (cons[svc_tmr_en] == 1)
					{
						if (rdginp(i_TMREN) == 1)			// Check TMREN input
							tmr_svc[svc_t[i].service] = 1;		// Turn on the service
					}
					else
						tmr_svc[svc_t[i].service] = 1;		// Turn on the service
				}
				tmr_svc_logic[svc_t[i].service] = svc_t[i].logic;
			}
		}
	}	
}


/* Revision History

  5/31/01  1.03  mhd 	1. Clear iodata input bit to cancel hall call if running simulator.
  6/07/01  1.04  mhd	1. Changed comm packets to allow for 29 floors. 
  						2. In receive packet 2, rmbuff must be made long before 8 bit shift
						   otherwise, if the data == 80H then shifted first (8000h) and then
						   converted, the conversion changes the long into a negative signed 
						   long interger equal to 0FFFF8000H.
  6/21/01  1.05  mhd	1. Changed LCD display to rotate io screen through multiple pages.
					    2. Change LCD io display to 3 io's per row from 4 with up to 5 characters.
						3. Copy over ioname from video routine.
  6/29/01 1.05 mhd		2. Swapped location of FF with RST.
  7/05/01 1.06 mhd		1. Added Version number on lcd display.
						2. Modified mechanism to change the video display.
  7/11/01 1.07 mhd		1. Add debounce to LCD board push buttons.
  7/17/01 1.07 mhd		1. Correct logic to decrement video display from car 2 to group display
						   when car 2 is the group. (in Refresh LCD routine).
						2. Added field variable for field weakening.
  7/19/01 1.07 mhd		1. Set last_page whenever enter hit.
  7/26/01 1.08 mhd		1. Modified LCD display to match manual.
  8/1/01  1.08 mhd		1. Transmit new screen data from car to group (see g_enc_vel).
						2. Added Encoder PPR and RPM to field variables.
  8/22/01 1.09 mhd		1. When group hall calls are cleared, also clear the assignments.
  8/27/01 1.10 mhd		1. Added rear hall call cancel for the simulator.
  8/28/01 1.10 mhd		2. If eta greater than 255 then set to 255.
  9/14/01 1.10 mhd		3. Changed the call sequence for the door,fire and procf display.
 10/1/01  1.10 mhd		4. Added User Baud field adjustable variable.
 10/18/01 1.10 mhd		5. Replace cons[hydro] with Drive_Type[carnmb].
					    6. Changed Array sizes in group to six cars.
 10/19/01 1.10 mhd		7. Added Status buffer in battery backed memory and display of status. See
						   sys_stat_write.
  10/25/01 1.10 mhd		8. set old_firef = -1 if doorf changes to 0.
						9. Check firef != 0 to display fire status instead of LCD_firef.
						10. Display fire status if firef != 0 and procf >=14 and <= 16.
 11/10/01  1.10 mhd		11. Correct problem with receiving data for car three. Reading from rmbuf[1] instead
 							of rmbuf[2]  
 11/26/01  1.11 mhd		1. Use variable (fvars[fvpark]) to disable or set the number of cars to park.
 01/08/02  1.12 mhd		1. Deleted cancelling hall call if car's eta is equal to 255.  With one car having > 17 floors
						   the down calls above a down moving car or up calls below an up moving car will be cancelled
						   if all the up or down hall calls are entered.
 02/06/02 				2. Show dpp_count on LCD screen even if negative.
 02/07/02         		3. Add run status screen for traction cars under elevator status menu.
						4. Changed Learn hoistway screen and Display hoistway table to show
						   actuall car velocity from encoder and encoder direction.
 02/20/02				5. Added LCD menu to display fault screen on VGA display
 03/04/02 1.14 mhd		1. Moved LCD routines to LCD.ccp.
						2. Set down collective hall calls if hall call power lost.
 						3. Added Emergency Power return and selection.
						4. Set Global variable Dispatcher to display which car is the dispatcher.
4/1/02	2.00 mhd		1. Moved Group_Com to Comm.cpp.
4/17/02	2.01 mhd		2. When setting down collective hall calls if hall call power lost in group_hallcall routine,
						   use "i <= cons[grtopf]" instead of "i < cons[grtopf]" in for loop.
07/9/02 3.00 mhd		1. When using iodata array, use labels instead of numbers such as DNRD instead of 7.
10/21/02 3.01 mhd		1. For Tract non-distance feedback controllers, use UPF and DF as speed command signals B1:B0.
						   This caused logic for slowdown to be changed.
12/18/02 3.02 mhd		1. Seperated group_hallcall routine into group_hallcall,latach_hallcall and assign_hallcall.
						2. Added Code for Serial Hall Call board.
						3. If call assigned and car is at the floor keep assignment if car has no preference or a preference
						   in the direction of the call.
						4. Changed parking to adjust to the parking variable for how many cars to park.
						5. Park the closest car to the parking floor
						6. Added 4 second time delay before sending car to parking floor.
2/12/03 				7. Changed check for servf to consider att as in service.
						8. Use Car_Not_Talking to determine if car is communicating during emergency power recall and select.
3/18/03 3.03 mhd		1. Corrected bug when setting rear hall call (g_dnr_hall_call[0]) - Use gc_rdc instead of gc_dc. 
7/22/03 mhd				2. In select_cars, AND iodata with (Car_not_Talking == 0).
4/3/03 3.04 mhd			1. Use hall call serial comm if car serial comm used (sercom & 3) != 0.
						2. Set cancel flag for serial hall call board if call cannot be assigned.
						3. If serial hall calls, set up and down calls if board does not communicate.
6/27/03 				4. Fixed bug - used gc_rdc in latch_ser_hallcall for rear instead of gc_dc.
7/07/03 				5. Added lobby request and next up software.
						6. Modified group parking to work with lobby request and next up.
						7. Added group car call lockouts.
						8. Modified group_eta so as not to use iodata[][] to check for the car's slowdown and stopped status.  This
						   causes a jump in the eta. Use hsf and procf instead.  
7/31/03 3.05 mhd		1. Added code blue operation. 
						2. Added IR service. 
8/13/03 3.06 mhd		1. Deleted using ICR input for ir service, instead use gc_ir and gc_rir from hall call boards. 
						2. Added Inter-group emergency power.
8/27/03 3.07. mhd		1. Added group hall call lockouts from car call lockout inputs   
8/29/03 				2. Do not put car back on ncu_door in state 2 if dcalls == 0. 
9/12/03 3.08 mhd		1. Keep eta reassignment from jumping around by using etamin parameter and by having eta 3/4 better to reassign.
						2. Added coincident car call assignment.
						3. Clear g_up_hall_call[0],g_dn_hall_call[0],g_upr_hall_call[0],g_dnr_hall_call[0] outside of for loop
						   in grp_clrhc routine because since ir_car == 0 when ir is not used, these variables otherwise do no
						   get cleared. (Fix but that calls remain after all cars go out of service.)
10/2/03 3.09 mhd		1. Use (sercom & 1) != 0 to allow COP comm without hall call comm.
						2. Use serial comm for ir calls always.
10/10/03 3.10 mhd		1. On recall check if car is on earthquake op to take out of service when level.
						2. Also restart the time-out timer on emp recall if the car changes service.
						3. Changed detection of car moving in emergency power routines. Use run with up and down instead of 
						   up fast and down fast since DF input cannot be used for DC-Scr drive cars.
						4. In Keep_assignment routine, moved check for next stop ahead of check for coincident car calls to prevent
						   a car from loosing its assignment at the last moment.
						5. Provide some hysteresis for coincident car calls in keep_assignment and make_assignment routines.
11/5/03 3.11 mhd		1. Do not set ETA to 255 if door is nudging.  Doing so causes the next up car to not leave the floor
						   when just hall calls are in the system.
11/14/03 3.12 mhd		1. Allow hall calls to be cancelled if cons[mand] == 2.
12/22/03 3.17 mhd		1. Add new security_type[] = 4 for seperate car call and hall call lockouts.  Use the rear car call 
						   lockouts for the hall calls.
12/24/03 				2. Added up and down peak operation.
1/5/04					3. Use lobby_request insteady of fvars[fvlbyreq] since lobby_request is changed by up_peak.
						4. Changed best_reparking_car to reparked the car from the parked_fl to the parking_fl.  (parking_fl
						   was used incorrectly)
						5. Dynamically set the next_up flag and send to each car.
1/29/04	3.19 mhd		1. Added hall call cross assignment.
2/20/04 3.20 mhd		1. Modified emergency power to count the car on medical emergency service as recalled see servf == s_MED_EMS.
3/3/04 4.00 mhd			1. Deleted [io_sel] array acces to i/o's.
8/27/04 4.03 mhd		1. Used standard parking operation for parking a simplex car.
11/3/04 4.03 mhd		1. Changed otslbp flag from == 1 to != 0 to allow for the flag to be 2.
1/12/04	4.04 pn			1. Added parking floor and width parameters to park cars at specific floors.
4/21/05 4.08 mhd		1. Moved setting gc_sec to comm.cpp and added gc_rsec for rear.
4/22/05 4.08 mhd		2. Do not recall a car on emergency power that is turned off "elevator off switch".
						3. Extended the mask 0xffffffL to 0xffffffffL in the rear hall call assignment and cancel routines.
5/9/05 4.09 mhd			4. Corrected indes for variables using car_order[i] instead of i or car_ix in recall_cars and select_cars. 
4/24/05 v4.11 mhd 		1. Changed Security_Type to allow both car call lockouts and group car call lockouts.
4/26/05 v4.12 mhd		1. Added Lobby Up Hall Call Request call operation (extra hall call at the lobby).
5/6/05 v4.17 mhd		1. Fix bug when checking clearing call in g_up_hall_call(el), check for ir_car != to k instead of i.
6/22/05 V4.23 mhd		1. Added local gettime, getdate, settime setdate so that the 18.2 msec DOS interrupt could be disabled.
6/23/05 V4.23 mhd		2. With no car in service and (fvars[fvxasgn] == 2) cross cancellation, do not cancel hall calls.
6/27/05 v4.23 mhd		3. With (fvars[fvxasgn] == 2) cross cancellations, when the hall call goes away, clear the call in 
						   g_up_hall_call[0] and g_dn_hall_call[0] arrays to cancel the call. 
6/27/05 v4.24 mhd		1. Change the ids_cc_sec and ids_rcc_sec to arrays for each car.
7/11/05 v4.24 mhd		1. Added group_io routine to transfer group io data from group to car and car to group. 
7/14/04 v4.25 mhd		1. Added default statement at the end of all switch case commands is there were none.
8/17/05 V4.28 mhd		1. Added special IR control.
						2. Changed ir cancel to cancel call only when assignment is made i.e. bit set in g_up_hall_call[ir_car] and g_dn_hall_call[ir_car]
11/01/05 V4.37 mhd		1. Added Emergency Power Transfer input to shut the cars down prior to power transfet.
11/10/05 v4.39 mhd		1. Keep hall call if car on load bypass.
12/12/05 V4.43 mhd		1. Correct logic error in v4.24 when cancelling ir call when bit in g_up_hall_call[ir_car] is set.
						2. Modified ir_control to clear g_iruc_canc flag after 2 seconds when taken off of ir.
12/13/05 v4.44 mhd		1. Changed the emergency power car recalled logic to include low oil and low pressure.
2/9/06	v4.54 pn		1. Added the ability to add 2nd car to be able to swing into IR.
11/14/05 4.55 mhd		1. Allow hall calls to be cancelled if cons[mand] == 5.
3/23/06 v4.60 mhd		1. Allow emergency power sequencing for hydros.
						2. Use gpi inputs for front and rear hall call security.
4/6/06 v4.62 mhd		1. Clear ir call when ir car goes out of service.
4/24/06 v4.64 mhd		1. Modified code blue to keep call when car on independent.
						2. Added hospital service 22 instead of going on independent.
						3. Allow hospital service to continue to run even if fire service activated.
						4. Car_order was being set and used in set_ep_run_ctl which defaulted the order to the return car.  The car order
						   does not matter here since it is used just to see if any car is in motion.  Deleted its use to correct running the correct
						   selected car.
5/24/06 v4.67 mhd		1. Changed single automatic pushbutton to not run assign hall calls if car has a preference and it is moving. Otherwise
                           when the car gets to the call, the call never gets cancelled so the preference never goes away.
5/26/06 v4.68 mhd		1. Added emergency power car status indicators.
						2. No single automatic pushbutton if car swith used (works like attendant car).
6/5/06 v4.69 mhd		1. Corrected single automati pushbutton operation to cancel and disable hall calls properly.
						2. In car switch operation, latch hall calls in group but automatically bypass hall calls when 
						   car switch is on. Cancel hall calls when car stops at the floor.  Hall call board can be used 
						   for hall call annunciator.
6/12/06 v4.71 mhd		1. Correct bug in single automatic pushbutton code that caused the hall calls to be cancelled in 
						   normal operation.
8/30/06 v5.04 mhd		1. Allow hall calls to be cancelled if cons[mand] == 6.
8/30/06 v5.05 mhd		1. Allow hall calls to be cancelled if cons[mand] == 7 for staggerd auto/swing door.
10/20/06 v5.06 mhd		1. Cancel ir call if not a valid call floor.
						2. Added parking for 8 cars and added subroutine park_car.
						3. Added Parking type 0 = busyest floor park, 1 = divide hoistway, 2 = fixed parking
						4. Fixed parking width operation and modified best_parking_car routine to use the parking_floor_full
						   flag to determine which cars are parked.
10/30/06 v5.06 mhd		5. Set lobby up request mask to 1 or 5.  When set to 1 that car is assigned the lobby up request or a standard
					   	   up hall call. When set to 5, the car is assigned the lobby up request but uses the secf input to determine
					       if the standard up hall call can be assigned to this car.  Bit 2 is set in software to disable the standard
					       up hall call assignment when SECF is on.  To use this feature, the secf feature must also be programmed. 
10/31/06 v5.07 mhd		1. Allow IR riser to switch between standard riser and ir riser by or'ing the calls into the standard call array.
11/20/06 v5.08 mhd		1. Clear the ncu_seq when car's preference (dpref) goes to zero so the car does not get stuck in the next up sequence
                           when answering an opposite hall call.
02/16/07 v5.17 pn		1. For cars with rear IR riser in latch_ir_hallcall procedure
						   changed the logic from cons[rearop] to rear_op[ir_select] to
						   assign rear IR calls.
2/15/07 v5.18 mhd		1. Don't allow up peak or down peak during emergency dispatch (hall call power loss or comm loss).
						2. Modify hallcall_comm_loss to work for six boards up to 60 floors.
2/23/07 v5.19 mhd		1. Added ids: up and down peak activation.
2/26/07 		 		1. For group call assignment changed cons[rear] to grtop_rfl.
4/22/07 v5.23 mhd		1. Don't check doorf and rdoorf to park car if swing door.
5/25/07 v5.24 mhd		1. Added special IR service for ir car to stay in group while answering both ir and std hall calls.
6/6/07 v5.25 mhd		1. Added variable fvars[fvotshc] "OTS No HC Canc" to prevent from cancelling hall calls when all cars go out of service.
						2. Added auto service time-out feature to take car out of group service if assignment not answered or car
						   car stuck at floor with a direction preference for five minutes.
						3. Modified X-assignment to not cancel the hall calls and run the assign_hallcall routine even if all cars are out of service.
						4. Corrected statement for transferring hall calls from ((avg_dc_eta + avg_uc_eta)/2) > fvars[fvxeta]) to
						   ((avg_dc_eta > fvars[fvxeta]) || (avg_uc_eta > fvars[fvxeta])).
6/29/07 v5.28.1 mhd		1. Zero avg_uc_eta and avg_dc_eta the number of up or down calls go to zero to all the new calls to transfer back to the GALAXY.
7/02/07 v5.29	mhd		1. Added car assigned to floor security operation (similar to Rick's selective security).
						2. Modified ASV timer to clear "run_timer" flag if the position changes.
						3. Added valid car for code blue operation.
7/16/07 v5.29.2 mhd		1. Temporarily Deleted ASV timer until it can be tested further.
7/10/07 v5.30 mhd		1. Allow group to keep hall call if ir car is in service and is able to switch back from ir.
						2. Also set ir_call_enable if ir input on but ir_active not set this allows cons[incrs] == 3 option to work.
8/9/07 v5.32 mhd		1. Added option for valid emergency power car to be selected to run.
8/15/07 v5.33 mhd		1. Corrected gpi index for rear hall call security, 3,4,5 changed to 6,7,8.
10/1/07 v5.37 mhd		1. Seperated best code blue car selection for front and rear so that if a code blue call is entered for the 
                           front and rear door at the same floor, two different cars can answer the call.
10/4/07 v5.38 mhd		1. Addes second riser operation.
10/15/07 v5.38.1 mhd	1. Add extra time to eta of car that is not on attendant (if fvars[fvatt_eta_pref] != 0).
10/30/07 v5.38.3 mhd	1. Do not allow attendant car to be next up car.
11/13/07 v5.39 mhd		1. Added vip service.
						2. Deleted ots_time_out[] and instead set out of service immediately.
11/20/07 v5.39.1 mhd	1. Added parking enable and next up enable control and inputs.
11/28/07 v5.39.4 mhd	1. Use lobby_fl variable instead of fvars[fvlob] so that lobby_fl can be changed to alternate lobby fvars[fvaltlob]. 
						2. Changed parking enable and next up enable to alternate parking (change from parking at lobby floor to another floor) and
						   forced next up.  Added alternate lobby.
12/20/07 v5.39.8 mhd	1. Added handicap pushbutton car request software.  Set fvhandwait to 0 to disable or to 255 to assign overloaded car when not other available.	
						2. Change location for RICR from (cons[grtopf]+1) to (grtop_rfl + 1).
1/15/2008 v5.40.2 mhd	1. Corrected hall call security lockouts (modified index for gpi) for high rise group.
2/8/08 v5.40.4 mhd		1. Added ATTHC attendant hall call cancel input (works for simplex cars only).
3/21/08 v5.42.3 mhd		1. Change location for RICR for second riser operation to match standard IR opearation from (cons[grtopf]+1) to (grtop_rfl + 1).
4/15/08 v5.43.3 pn		1. Changed "cons[ids] == 1" check to "((cons[ids] & 1) != 0)" since I added
						   an option for cons[ids] bit 2 for adding an additional GPIO board for 
						   montoring of external signals through lift-net.
4/17/08 v5.43.5 mhd		1. Added EP recall delay time on first recall to allow car time to settle after power loss.
4/18/08 v5.43.6 mhd		1. Added grp_cc_override() for group security car call security operation (visitior pass).
4/24/08 v5.43.8 mhd 	1. Turn on transfer latch flag (gc_xucl[] and gc_xdcl[]) only after input has turned on (gc_xuc[] and gc_xdc[]). This is to prevent 
 						   the output from going off too quickly.
6/20/08 V5.44.5 mhd		1. Cancel serial hall call when secured only if call is not already in the system. 
8/15/08 V5.45.5 mhd		1. Add sabbath group service (set cons[Sabbath] == 3).
						2. Corrected hall call power loss logic and added seperate timer.
						3. When either hcp loss or group sabbath is restored to normal, prevent the car from going on up or down peak service.
8/19/08 v5.45.6 mhd		1. In group_io() routine make prev_gpi[12] a static variable so the io change of state detection works properly. 
8/20/08 V5.45.6 mhd		1. Added Security Recall as service 24.
						2. Set Sabbath floor mask from cons file for group sabbath service.
9/3/08 v5.45.8 mhd		1. Cancel the hall call right away if hugs security being used (cons[hugs] == 1).
						2. Use hall call security lockouts if cons[hugs] == 1.
10/30/08 v5.46.4 mhd	1. Set lobby_request = cons[nmbcars]-1 for up peak (instead of cons[nmbcars]).
11/4/08 v5.46.5 mhd		1. Adjust the lobby request number on up peak by subtracting fvars[fvuppkpool].
						2. Modified lobby request for heavy up peak (fvars[fvuppkctrl] & 0x01 != 0) to recall all cars a once.
						3. Modified down peak parking to call all cars to top or most down hall call floors if heavy down peak (fvars[fvdnpkctrl] & 0x01 != 0) required.
11/19/08 v5.46.6 mhd	1. Use a cross-assigmnent mask from the cons file to determine if both groups get the call. 
						2. Use the ratio of number of cars in service to the number other xa cars in service to determine when to transfer the call. If number of xa cars
						   zero, use the eta method.
						3. Moved the check for cross-assignment out of the loop.
12/11/08 v5.46.8 hdl	1. if assignment > nmbcars, i.e. is invalid, then cancel call.
						2. Added check for cons[att] == 2 to set attendant hall annunciator lights.
1/9/09 v5.47 mhd		1. Set the variable any_ir_hall_call from the latch_ir_hallcall() routine.
1/10/09 v5.47.2 mhd		1. Added service timer activation of sabbath service.
2/02/09 v5.47.4 pn		1. Take elevator out of group service when cons[Sabbath] = 4.
2/20/09 v5.47.6 mhd		1. Corrected bug in latch_ser_hallcall() to lock hall calls in from up and down calls (see else condition that was added).
4/23/09 v5.48.4 mhd		1. Added Dual serial riser operation for traction high rise only.
5/26/09 v5.48.7 mhd		1. Use gc_uc_dsr[], gc_dc_dsr, etc. to latch and clear hall calls in lacth_ser_hallcall() and group_hallcancel().
6/12/09 v5.48.11 mhd	1. Added faults for emergency power car recall time-out and recall out of service.
8/1/09 v5.49.6 mhd		1. Increase emergency power switch change debounce from 1 secon to 3 seconds.
8/6/09 v5.49.7 mhd		1. Modified any_fvip_car() and any_rvip_car() to return 1 if the car is a valid vip car and the floor is a valid floor.  Deleted checking if
           				   the car is on VIP.
						2. Added code to detect that VIP car went out of service to cancel the assignment.
9/29/09 v5.50.3 mhd		1. Allow special momentary ir service (fvars[fvirctl] == 0x05) to interrupt sabbath operation.
						2. Clear hall call set flags gc_uc_set[], etc, when ir turned off and (cons[incrs] == 2).
						3. Added clear ir cancel routine to leave cancel flag off until car goes out of service.
						4. Added get_any_ir_hall_call() routine to cancel ir hall calls when not running ir service.
						5. Corrected down hall call count. Logic for variable no_call was or'ed instead of and'ed.
10/15/09 v5.50.4 mhd	1. Added Galileo hall call lockouts.
						2. Added activation of group User Inteface variables ui_uppeak, ui_dnpeak, ui_alt_fl_sec, ui_ncu, ui_alt_lby and ui_alt_park.
10/23/09 v5.50.5 mhd	1. User Recover timer for non-automatic car to have time to recover to floor.
						2. After auto recall sequence, if a car is on phase 2, select it to run.
						3. Corrected bug with valid_ep_car index using valid_ep_car[i] instead of valid_ep_car[car_order[i]].
						4. Added emergency power manual select enable flag to prevent the manual selection from occuring until after the car return to the 
						    recall floor.
12/28/09 v5.51.2 mhd	1. Deleted check for car in reset mode or car in changing service since emergency power recall has a start delay and if a car gets
						   stuck in reset or changing from reset to atfloor, the next car will never get selected.
12/30/09 v5.51.3 mhd	1. Transfer next up to next car when the doors start to close on current next up car.
1/6/10 v5.51.4 mhd		1. Extensively modified emergency power operation.  
							a. Allow manual selection switch to override recall in three ways: immediately, after car finishes recall or after recall sequence is complete.  
							b. During recall sequence, allow out of service car 20-30 seconds to get car to floor or recall. 
						    c. Cycle through recall process two times.  
						    d. Give hospital service and phase 2 selection priority.  Reselct the higher priority car if hospital service or phase 2 turned off.  
01/9/10 V5.51.4 mhd		2. Added EP recover move select count variable for the number of times the car can move before shutting down when the door opens and 
						   added at floor recover select variable to shut down the out of service car immediately if it is at the floor whith the door open otherwise
						   it will have to time out first. 
2/8/10 v5.51.10 mhd		1. Added parameter to assign parking floor to car with door open.
3/15/10 v5.51.14 mhd	1. Added group_swap_fl_tbl() to swap floor configurations for up to four additional security tables.
						2. Added security activation timers for Floor security configurations 1, 2, and 3.
3/23/10 v5.51.16 mhd	1. Clear the code blue floor assigned to the car if the car leaves the floor on hospital service.
6/16/10 v5.52.9 mhd		1. Modify group_cc_override() routine to override car's car call security directly from a key switch input.
						2. Corrected bug in rear vip car to not increment the number of vip cars if the car is on vip.  This was already done for
						   the front vip.
8/2/10 v5.52.19 mhd		1. Added secondary riser and dual riser code blue operation.
						2. Added code blue selection of ir car.
						3. Added ir penalty time to select non-ir car first.
8/18/10 v5.52.25 mhd	1. Added group hall call security "group_sec_car_fl_keysw()" to secure hall calls per floor per car from
						   a key switch for each car and floor.
						2. Don't allow the car to keep the assignment if the call mask is no longer valid (ucmsk,dcmsk etc) in
						   assign_hallcall() and assign_sr_hallcall().
11/4/10	v5.52.43 pn		1. In assign_hallcall and assign_sr_call routine modified the check for call assignment 
						   from call mask array (ucmsk,dcmsk etc) which was using an index of k to group assign array instead.
11/10/10 v5.52.45 mhd	1. Added master security mask in assign_hallcall() routine to all hall call security for individual cars through 
						   the ids, galileo and special sec routines. sse set_master_hc_sec() routine and master_uhc_carsec[][], master_dhc_carsec[][], 
						   master_ruhc_carsec[][] and master_rdhc_carsec[][] variables.
11/11/10 v5.52.46 mhd	1. Added software to activate and de-activate modified express service security for hall calls on individual cars. See set_master_hc_sec().
11/28/10 v5.52.49 mhd	1. Modified express service security.
12/08/10 v5.52.51 hdl	1. Added sr_hc_addr in set_front_hallcall_loss to correct emergency dispatching when using serial hall calls (cons[sercom] & 0x20)

1/22/11	v5.52.57  hdl	1. Added a check in  latch_ser_hallcall  if((cons[incrs] == 2)&&(rir_hc == 1)) before we Clear the cancel flag
							if not the call from the IR board will get latched even though the call is not taken from the controller. This is when the two risers are ored together

2/2/11 v5.52.58	hdl		1. Changed the max for fveppkled from 1 to 2. It allos the PRK led to come on only when teh car is at the emp floor 
2/18/11 v5.52.02 hdl	2. Added fvcbanycar to recall any car in the event that the code blue cars are not available

4/26/11 v5.53.16 mhd	1. If cross cancellations and all cars out of service then clear the cancel flags.
5/2/11 v5.53.18 mhd		1. Added tmr_svc_en[] to gate service activation timers.
						2. Modified the service timer logic to have the service turn on in the morning, off during the day and then on again in the evening. This works when the off timer 
						   value is less than the on timer.
12/18/12 v6.00.26 mhd	1. Added changes from galaxy3 from 5.55.06 to 5.55.20. (Left Vip only with my changes).
8/23/13 v7.0.16 mhd		1. Added changes from Galaxy 3 below:
						   {
								v5.55.26 as 	1. Modified service_timer_control() for adding logic for Service Activation Timers enable input
								v5.55.32 as		1. Added logic for Auto Service Time-Out parameter in group_car_ots() routine.
								v5.55.40 as		1. Added cons[Sabbath] = 6 to have up/dn collective calls with gio board.
						   }

11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.

5/7/14 v7.0.43 mhd		1. group_car_ots() used o_RUN for this car so changed it to (grp_Car_Stat[i].g_run_statusf & rs_RUN). 
10/8/14 v7.1.14 mhd		1. Added non-simultaneous door flag in car to group's car structure.
1/24/17 v8.0.1 mhd		1. Added statusf3 and statusf4.				
*/

