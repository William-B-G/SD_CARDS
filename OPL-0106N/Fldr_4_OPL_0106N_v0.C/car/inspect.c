//  Elevator Inspection routines

#define d_INSPECT 1
#include "global.h"


uint8 ins_servf = 0;

#define c_INV_INS 0
#define c_CT_INS 1
#define c_MR_INS 2
#define c_ACC_INS 3
#define c_IC_INS 4
#define c_CT_LBP_INS 5
#define c_CT_GBP_INS 6
#define c_CT_LBP_GBP_INS 7
#define c_SEL_SETUP_INS 8
	

float ins_vel;

bool top_run_dn = 0;
bool bot_run_up = 0;
int32 ACC_UT_Dist = 0;			// Access UT distance (limit to run down on top access)
int32 ACC_DT_Dist = 0;			// Access DT distance (limit to run up on bottom access)

int chk_ins(void);
int chk_ins_sw(void);
int access_run_req (void);
void inspection(void );
void ins_run(int cmd);
int chk_bad_ins_sw(void);

// *******************************************************
// This routine verifies the car is on inspection 
// *******************************************************
int chk_ins(void)
{
	if ( (rdinp(i_CTIN) == 1) || (rdinp(i_ACC) == 1) ||
		(rdinp(i_MRIN) == 1) || (rdinp(i_ICI) == 1) ||
		(rdinp(i_LBP) == 1) || (rdinp(i_GBP) == 1) ||
		(rdinp(i_AUTO) == 0) ||
		((rdinp(i_HWSET) == 1) && (cop_can_online == 1)) )
		return 1;
	else
		return 0;
}
//*************************************************
// Check for bad inspection switches / inputs
// 			0 = stop, 1 = up, 2 = dn
//*************************************************
int chk_bad_ins_sw(void)
{

	if(cons[access_type] != 0)
	{
		if((rdinp(i_TAU) == 1) || (rdinp(i_TAD) == 1) || (rdinp(i_BAU) == 1) || (rdinp(i_BAD) == 1))
		{
			return 1;
		}
	}

  	if (rdinp(i_CTIN) == 1)
	{			// Car top inspection
		if ((rdinp(i_INE) == 1) || (rdinp(i_IU) == 1) || (rdinp(i_ID) == 1))
			return 1;
	}
	
	if (rdinp(i_MRIN) == 1)
	{
		if ((rdinp(i_MRIE) == 1) || (rdinp(i_MRIU) == 1) || (rdinp(i_MRID) == 1))
			return 1;
	}

	if (rdinp(i_ICI) == 1)
	{	// DCB is used as enable input for in car inspection
		if ((rdinp(i_DCB) == 1) || (rdinp(i_CC(cons[bottomf])) == 1) || (rdinp(i_CC(cons[topf])) == 1))
			return 1;
		if (cons[rear] != 0)
		{
			if ((rdinp(i_DCB) == 1) || (rdinp(i_CCR(cons[bottomf])) == 1) || (rdinp(i_CCR(cons[topf])) == 1))
				return 1;
		}
	}

	return 0;
}

//*************************************************
// Check inspection run command from input switches
// 			0 = stop, 1 = up, 2 = dn
//*************************************************
int chk_ins_sw(void)
{
	if (rdinp(i_CTIN) == 1)
	{			// Car top inspection
		if (rdinp(i_INE) == 0)
			return 0;
		else if (((rdinp(i_IU) == 1) && (rdinp(i_ID) == 0)) && (rdinp(i_UNsel) == 1))
			return 1;	   // Run up
		else if (((rdinp(i_IU) == 0) && (rdinp(i_ID) == 1)) && (rdinp(i_DNsel) == 1))
			return 2;	   // Run Down
		else
			return 0;
	}
	else if ((rdinp(i_ACC) == 1) && (eqf == 0))
	{
		if (( ((rdinp(i_TAU) == 1) && (rdinp(i_TAD) == 0) && (rdinp(i_DLT) == 0) && (rdinp(i_DLT_1) == 0)) ||
			((rdinp(i_BAU) == 1) && (rdinp(i_BAD) == 0)) ) && 
			(rdinp(i_UNsel) == 1) && (safe() == 1))
			return 1;	   	// Run Up
		else if (( ((rdinp(i_TAU) == 0) && (rdinp(i_TAD) == 1)) ||
			((rdinp(i_BAU) == 0) && (rdinp(i_BAD) == 1) && (rdinp(i_DLB) == 0) && (rdinp(i_DLB_1) == 0)) ) && 
			(rdinp(i_DNsel) == 1) && (safe() == 1))
			return 2;		// Run Down
		else
			return 0;
	}
	else if ((rdinp(i_ICI) == 1) && (eqf == 0))
	{
		if (rdinp(i_DCB) == 0)
			return 0;
		if ( (((rdinp(i_CC(cons[bottomf])) == 0) &&
			 (rdinp(i_CC(cons[topf])) == 1)) ||
			 (((cons[Car_Sw] & 2) != 0) && (rdinp(i_ATTUP) == 1) &&
			 (rdinp(i_ATTDN) == 0))||			 
			 ((cons[RGB_CC] == 1) && (rdinp(i_ICIU) == 1) && (rdinp(i_ICID) == 0)))
			   && (rdinp(i_UNsel) == 1))
			return 1;	   // Run Up
		else if ( (((rdinp(i_CC(cons[bottomf])) == 1) &&
				  (rdinp(i_CC(cons[topf])) == 0)) ||
				  (((cons[Car_Sw] & 2) != 0) && ((rdinp(i_ATTDN) == 1)) &&
				  (rdinp(i_ATTUP) == 0))||
				  ((cons[RGB_CC] == 1) && (rdinp(i_ICID) == 1) && (rdinp(i_ICIU) == 0))) 
				  && (rdinp(i_DNsel) == 1))
			return 2;		// Run Down
		else if (cons[rear] != 0)
		{ 
			if (( (rdinp(i_CCR(cons[bottomf])) == 0) &&
				(rdinp(i_CCR(cons[topf])) == 1) )  
				&& (rdinp(i_UNsel) == 1))
				return 1;	   // Run Up
			else if (( (rdinp(i_CCR(cons[bottomf])) == 1) &&
				(rdinp(i_CCR(cons[topf])) == 0) )  
				&& (rdinp(i_DNsel) == 1))
				return 2;		// Run Down
			else
				return 0;
		}
		else
			return 0;
	}
	else if ((rdinp(i_MRIN) == 1)  && (eqf == 0))
	{		// Motor room inspection
		if (rdinp(i_MRIE) == 0)
			return 0;
		if (((rdinp(i_MRIU) == 1) && (rdinp(i_MRID) == 0)) && (rdinp(i_UNsel) == 1))
			return 1;		// Run Up
		else if (((rdinp(i_MRIU) == 0) && (rdinp(i_MRID) == 1)) && (rdinp(i_DNsel) == 1))
			return 2;		// Run Down
		else
			return 0 ;
	}
	return 0;
}


//****************************
//  Access Run Request
//****************************

int access_run_req (void)
{

	// ACC UT distance first
	ACC_UT_Dist = ((int32)(fvars[fvaccutdist] * Pulses_per_Inch))/10;
	// ACC DT distance second
	ACC_DT_Dist = ((int32)(fvars[fvaccdtdist] * Pulses_per_Inch))/10;

	top_run_dn = (DPP_Count > (DPP_UT - ACC_UT_Dist));
	bot_run_up = (DPP_Count < (DPP_DT + ACC_DT_Dist));
	if ((rdinp(i_ACC) == 1) && (eqf == 0))
	{
		if (( ((rdinp(i_TAU) == 1) && (rdinp(i_TAD) == 0)) ||
			((rdinp(i_BAU) == 1) && (rdinp(i_BAD) == 0) && ((bot_run_up != 0) || (rdinp(i_DTsel) == 0))) ) && 
			(rdinp(i_UNsel) == 1))
			return 1;	   	// Run Up
		else if (( ( (rdinp(i_TAU) == 0) && (rdinp(i_TAD) == 1) && ((top_run_dn != 0) || (rdinp(i_UTsel) == 0)) )  ||
			((rdinp(i_BAU) == 0) && (rdinp(i_BAD) == 1)) ) && 
			(rdinp(i_DNsel) == 1))
			return 2;		// Run Down
		else
			return 0;
	}
	return 0;
}

//********************************
// Elevator is on Inspection Mode
//********************************
void inspection()
{
	static int in_insp;

#if(Traction == 1)
   	int i;
   	int moving=0;
#endif
	trigger_arm_state |= INS_START;
	clrall();
	dirf = 0;
	in_insp = 0;
	startf = 0;
	ins_status = 0;
	statusf = 0;	// clear status flag
	statusf2 = 0;
	rset_error_cnt = 0;		// made it out of reset
	clroutp(o_FST);		// clear here incase inspection enable during test
	clroutp(o_FSTP);


	if ((enable_cc_test == 1) || (continue_cc_test == 1))
	{
		cancel_cc_test = 1;
		enable_cc_test = 0;
		continue_cc_test = 0;
	}
	
	

  	while(procf == 2)
   	{
		ins_status = 0;
		if (rdinp(i_CTIN) == 1)
			ins_status |= 1;
		if (rdinp(i_MRIN) == 1)
			ins_status |= 2;
		if (rdinp(i_ACC) == 1) 
			ins_status |= 4;
		if (rdinp(i_ICI) == 1)
			ins_status |= 8;
		if (rdinp(i_LBP) == 1)
			ins_status |= 0x10;
		if (rdinp(i_GBP) == 1) 
			ins_status |= 0x20;
		if (rdinp(i_AUTO) == 0)
			ins_status |= 0x40;
		if ((rdinp(i_HWSET) == 1) && (cop_can_online == 1))		// setup switch for selector interface board is on
			ins_status |= 0x80;

	 	if ((ins_status & 0x7F) == 0x41)
			ins_servf = c_CT_INS;
		else if ((ins_status & 0x7F) == 0x42)
			ins_servf = c_MR_INS;
		else if ((ins_status & 0x7F) == 0x44)
			ins_servf = c_ACC_INS;
		else if ((ins_status & 0x7F) == 0x48)
			ins_servf = c_IC_INS;
		else if ((ins_status & 0x7F) == 0x51)
			ins_servf = c_CT_LBP_INS;
		else if ((ins_status & 0x7F) == 0x61)
			ins_servf = c_CT_GBP_INS;
		else if ((ins_status & 0x7F) == 0x71)
			ins_servf = c_CT_LBP_GBP_INS;
		else if ((ins_status & 0xC0) == 0x80)
			ins_servf = c_SEL_SETUP_INS;		// if not on inspection and setup switch is on then inspection fault
		else 
			ins_servf = c_INV_INS;
		
			
		if (in_insp == 0)
		{
			if(chk_bad_ins_sw() != 0)
			{
				record_fault(f_ins_up_dn_sw);
				ins_sw_err = 1;
			}
			else if (chk_ins_sw() != 0)
			{
				record_fault(f_ins_up_dn_sw);
				ins_sw_err = 1;
			}
		}
		else if ((chk_ins_sw() == 0)&&(chk_bad_ins_sw() == 0))
			ins_sw_err = 0;

		in_insp = 1;
		servf = s_INSP;
		gripper_test = 20;			// to test GTS error in safe() routine.
		Overspeed_Test = 0;
		Buffer_Test = 0;
		NTS_Test = 0;
		ETS_Test = 0;
	    NTS_spi.Command2.B.SET_UPSD = 0;		// clear the command on exit just to make sure
	    NTS_spi.Command2.B.SET_DNSD = 0;		// clear the command on exit just to make sure
		redundancy_test = 0;
		in_redundancy_test = 0;
	 	DZ_ON_Fault = 0;
		DZA_ON_Fault = 0;
		DZ_OFF_Fault = 0;
		DZA_OFF_Fault = 0;
		UL_ON_Fault = 0;
		DL_ON_Fault = 0;
		UL_DL_OFF_Fault = 0;
		motion_fault = 0;
		fault_run = 0;
		fault_run_cnt = 0;
		stop_seq = 0;
	    leveling_fault_cnt = 0;
		leveling_fault = 0;
		lev_startf = 0;
		shutdown  = 0;
	    estops_1_run = 0;
		closetry = 0;
		rclosetry = 0;
		safe_closetry = 0;
		safe_rclosetry = 0;
		Door_Lock_GS_DPM_Fault = 0;
    	if ((cons[BinPiIn] == 1) || (cons[sel_type] == 1) || (cons[sel_type] == 2))
			rset = 0;
		rset_run_cnt = 0;
		drv_rst_cnt = 0;
		enc_dir_fault = 0;		// SPB direction opposite from car direction
  	  	dol_fail = 0;
  	  	dol_fail_cnt = 0;
  	  	rdol_fail = 0;
  	  	rdol_fail_cnt = 0;


		// Keep the Soft starter motor contactor on except when testing
		if ((Drive_Type[cons[carnmb]] == 9) && (in_redundancy_test == 0))
			setoutp(o_STE);

		write_trace(TR_INSPECTION1);
		

	    if ((hwl_mode == 1) && (ins_sw_err == 0) && (hwl_mode_car == 1) && ((rdinp(i_HWSET) == 1) && (cop_can_online == 1)))
	    {
	    	APS_Car_Hoistway_Setup();		// Run setup from car station
	    }
	    
	 	inctime(0);
		nudg_buz = 0;
		rnudg_buz = 0;
		
		set_position_from_dpp();
		set_ul_dl_dz();

		// Inspection door control (special case)

		if (((safe() == 0) || (cons[dcpo] == 1)) && (chk_ins_sw() != 0) && (fvars[fvdcinsp] != 0) && 
				(rdinp(i_ACC) == 0) && (((rdinp(i_GBP) == 0) && (rdinp(i_LBP) == 0)) || (rdinp(i_CTIN) == 0)))
		{		// Doors open but want to move the car	SO Close the door
			setoutp(o_DC);
			if (cons[door_type] == 1)	 // otis ovl door operator
				clroutp(o_DO);
			if (cons[rear] != 0)
			{
				setoutp(o_DCR);			
				if (cons[door_type] == 1)	 // otis ovl door operator
					clroutp(o_DOR);
			}
		}
		else if ((safe() == 0) && (rdinp(i_ACC) == 1) && (access_run_req() != 0))
		{			   // on access
			if (fvars[fvaccdoorcl] != 0)
			{ 
				setoutp(o_DC);
				if (cons[door_type] == 1)	 // otis ovl door operator
					clroutp(o_DO);
				if (cons[rear] != 0)
				{
					setoutp(o_DCR);			
					if (cons[door_type] == 1)	 // otis ovl door operator
						clroutp(o_DOR);
				}
			}
			else if ((cons[mand] == 6) && (cons[rear] != 0) && ((cons[access_type] & 0x03) == 0x03))
			{			// special rear swing door with automatic front
					setoutp(o_DCR);			
					if (cons[door_type] == 1)	 // otis ovl door operator
						clroutp(o_DOR);
			}
		}
		else
		{
			if (ins_doorf == 3)
			{
				if (cons[door_type] == 1)	 // otis ovl door operator
					clroutp(o_DO);
				setoutp(o_DC);
			}
			else
				clroutp(o_DC);

			if ((cons[rear] != 0) && (ins_rdoorf == 3))
			{
				if (cons[door_type] == 1)	 // otis ovl door operator
					clroutp(o_DOR);
				setoutp(o_DCR);			
			}
			else
				clroutp(o_DCR);			
		}

		if ((rdinp(i_DZ) == 1) && (chk_ins_sw() == 0))
		{
			if (ins_doorf == 1)
				setoutp(o_DO);
			else 
				clroutp(o_DO);

			if ((cons[rear] != 0) && (ins_rdoorf == 1))
				setoutp(o_DOR);
			else 
				clroutp(o_DOR);
		}
		else
		{
			clroutp(o_DO);
			clroutp(o_DOR);
		}

		clroutp(o_NUD);
		clroutp(o_NUDR);
  		clrcc(0);
  		clrhc();
  		clrlant();
	 	doorf = 0;
		setvipf();
		setcodebf();
		sethsvf();
	 	setfsf();
		setempf();
		seteqf();
		setmedf();
		settugf();

		write_trace(TR_INSPECTION2);

		// Testing to see if saftey string is open
    	if(safeties() == 1)
			timers[tsst] = 0;
    	if((safeties() == 0) && (timers[tsst] > 10))
	 	{
			if ((statusf & sf_SS) == 0)
				record_fault(f_sstring);
			statusf |= sf_SS;		// no safety string input
			ins_status = 0;
	   		procf = 12;      // go to saftey string open mode
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
			statusf |= sf_S10;		// LC fuse blown
			ins_status = 0;
			procf = 12;
			return;
		}
		else
			statusf &= ~sf_S10;

		if (cons[ansi] < 2007)
	 		stallf = 0;		
		
		hsf = 0;			// no high speed runs on inspection	(high speed allow on hydro less than 150 fpm)
		if ((safe() == 1) && (ins_sw_err == 0))
		{
			ins_run(chk_ins_sw());
		}
		else
		{		// not safe
			if (chk_ins_sw() == 0)
			{
				io_fault = 0;					// clear redundancy io fault flag (on insp and not running)
				io_fault_cnt = 0;
			}
			startf = 0;							// ins motion fix

		    clroutp(o_MCC);
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
			    clroutp(o_MST);
		    clroutp(o_SU);
		    clroutp(o_SD);
	    	clroutp(o_RUN);
		    clroutp(o_SUF);
		    clroutp(o_SDF);
		}
		
	 	if(rdinp(i_SU) == 0)
			timers[tmotion] = 0;

		write_trace(TR_INSPECTION3);
		

		// Testing to see if on inspection
    	if(chk_ins() == 0)
	 	{
			if (procf == 20)
				return;
//		  	record_fault(f_reset);
			ins_status = 0;
			gripper_test = 0;
			statusf = 0;	// clear status flag
       		dirf = 0;
       		procf = 0;       // go to reset mode
     	}
   	}

	ins_status = 0;
  	return;
}

//********************************************************
// Inspection run command for Hydro 
//********************************************************


void ins_run(int cmd)
{
	int lps_ins;
	int inhibit_ins_run;
	static int dbn_cnt;
	
	if (cons[Low_Press] == 1)
	{
		if (((rdinp(i_LPS) == 1) && (fvars[fvlps] == 0)) || 
			((rdinp(i_LPS) == 0) && (fvars[fvlps] == 1)))
			lps_ins = 1;
		else
			lps_ins = 0;
	}
	else 
		lps_ins = 0;

	if ((Drive_Type[cons[carnmb]] == 9) && (cons[SFC_2004_IO] == 1))
	{
		if (rdinp(i_P) == 1)
			inhibit_ins_run = 1;
		else
			inhibit_ins_run = 0;
	}
	else
		inhibit_ins_run = 0;

	if ((cmd == 0) || ((cmd == 2) && (lps_ins == 1)) || (inhibit_ins_run == 1))
	{		// no run
		io_fault = 0;					// clear redundancy io fault flag (on insp and not running)
		io_fault_cnt = 0;
		dbn_cnt = 0;
		statusf &= ~sf_IO;

		clroutp(o_SUF);
		clroutp(o_SDF);
		
        // Checking to see if a soft stop is needed
		if((startf == 1) && (timers[tsoftstp] <= fvars[fvsst]))  // soft stop needed
		{
			timers[tmotion] = 0;      // not in motion clear motion timer
			timers[tpatdly] = 0;
			if(dirf == 1)
			{
				if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
				    setoutp(o_MST);
			    setoutp(o_MCC);
			}
		}
		else      // No soft stop
		{
			startf = 0;
			dirf = 0;
		  	clroutp(o_MCC);
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
			  	clroutp(o_MST);
		}

		clroutp(o_RUN);
		clroutp(o_SU);
		clroutp(o_SD);
	}
	else
	{
/*
		if ((fvars[fvins] != cons[speed]) || (cons[speed] > 150))
		{
			if (cmd == 1)
			{
				if ((rdinp(UFI,UFIB,UFID) == 1) && (io_fault == 0))
				{
					if (dbn_cnt > c_dbn_cnt)
					{
						record_fault(f_upf_on);
						io_fault = 1;
					}
					dbn_cnt++;
				}
				else
					dbn_cnt = 0;
			}
			else if (cmd == 2)
			{
				if ((rdinp(DFI,DFIB,DFID) == 1) && (io_fault == 0))
				{
					if (dbn_cnt > c_dbn_cnt)
					{
						record_fault(f_df_on);
						io_fault = 1;
					}
					dbn_cnt++;
				}
				else
					dbn_cnt = 0;
			}
		}
*/			
		if ((safe() == 0) || (io_fault == 1))
		{
			startf = 0;
		    clroutp(o_MCC);
			if ((Drive_Type[cons[carnmb]] != 9) && (MST_enable == 1))
			    clroutp(o_MST);
		    clroutp(o_SU);
		    clroutp(o_SD);
		    clroutp(o_RUN);
		    clroutp(o_SUF);
		    clroutp(o_SDF);
		  	return;	
		}
		
		if (cmd == 1)
		{		// run up
			dirf = 1;
			clroutp(o_SD);
		    clroutp(o_SDF);

			if (startf == 0)
			{
				if ((fvars[fvins] == cons[speed]) && (cons[speed] <= 150) &&
					(door_locks() == 1) && (car_gate() == 1) && (rdinp(i_UTsel) == 1))
					hsf = 1;
				if (chk_start(1) == 1)
				{
					startf = 1;
					statusf &= ~sf_IO;
				}
				else
				{
					statusf |= sf_IO;		// I/O Error
					clrall();
				}
			}
			else
			{
				setoutp(o_SU);
				if ((fvars[fvins] == cons[speed]) && (cons[speed] <= 150) &&
					(door_locks() == 1) && (car_gate() == 1) && (rdinp(i_UTsel) == 1))
				    setoutp(o_SUF);
				else
			    	clroutp(o_SUF);
				setoutp(o_RUN);
				setoutp(o_MCC);
				timers[tsoftstp] = 0;
			}
		}
		else if ((cmd == 2) && (lps_ins == 0))
		{		// run down
			dirf = 2;
			clroutp(o_MCC);
			clroutp(o_SU);
		    clroutp(o_SUF);

			if (startf == 0)
			{
				if ((fvars[fvins] == cons[speed]) && (cons[speed] <= 150) &&
					(door_locks() == 1) && (car_gate() == 1) && (rdinp(i_DTsel) == 1))
					hsf = 1;
				if (chk_start(2) == 1)
				{
					startf = 1;
					statusf &= ~sf_IO;
				}
				else
				{
					statusf |= sf_IO;		// I/O Error
					clrall();
				}
			}
			else
			{

				setoutp(o_SD);
				if ((fvars[fvins] == cons[speed]) && (cons[speed] <= 150) &&
					(door_locks() == 1) && (car_gate() == 1) && (rdinp(i_DTsel) == 1))
				    setoutp(o_SDF);
				else
			    	clroutp(o_SDF);
				setoutp(o_RUN);
				timers[tsoftstp] = 0;
			}
		}

	}
}


/* Revision History

*/
