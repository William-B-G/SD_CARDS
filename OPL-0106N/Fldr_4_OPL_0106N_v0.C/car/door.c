// this file contains all the door routines

#define d_DOOR 1
#include "global.h"

void set_auto_servf (void);
void auto_svc_time_out_recover (void);
void doors(void );
void rdoors(void );
void clr_door_vars(void);
void ck_swing_door(void);
void doorsel(void);
void peelle_doors(void);
void guilbert_doors();
void auto_peelle_doors(void);
void auto_freight_doors();
void auto_swg_doors(void);
void courion_doors();
void ems_doors();
int16 at_floor_check (void);
uint8 emp_doorop (void);

uint8 lock_gs_dpm_check(void);
uint8 gs_only_check(void);
uint8 lock_gs_dpm_flt_front(void);
uint8 lock_gs_dpm_flt_rear(void);

int16 sabbath_svc(void);
int16 chkdoor(void);
void lockgsdpm_fault_doorop(void);

#define last_redundancy_test 9

int16 f_door_open_device = 0;
int16 r_door_open_device = 0;
int16 disable_auto_door = 0;
int16 courion_fs2_to_fs1 = 0;
int16 FF_STP;
int16 ind_door_sel = 0;
int16 dis_cc_reopen = 0;
int16 sec_do_en=0;
int16 sec_rdo_en=0;
int16 allow_do = 0;
int16 short_doort_en = 0;		// short door dwelling enable flag
int16 short_rdoort_en = 0;		// rear short door dwelling enable flag

int16 no_simul_do = 0;			// 0=allow simul do, 1=do not allow simul do

// peelle electric edge test procedure routines and variables
int16 freight_eef_test(void);
int16 freight_eer_test(void);

//            _________
//    STAGE1 |  STAGE2 | STAGE3
//      OFF  |    ON   |  OFF
//   ________|         |________
//

// the following constants used to verify electric edge pulse test
#define EE_TEST_STAGE1_OFF 1	// verified edge input was initially OFF
#define EE_TEST_STAGE2_ON 2		// verified edge input turned ON 
#define EE_TEST_STAGE3_OFF 3	// verified edge input turned OFF
#define EE_TEST_PASSED 3		// electric edge test passed
#define EE_TEST_FAILED 4		// electric edge test failed

// ee and ee2 testing variables
int16 in_eef_test = 0;
int16 reset_eef_test = 0;

// eer and eer2 testing variables
int16 in_eer_test = 0;
int16 reset_eer_test = 0;

// ******************************************************
// This is check and set door flags to control the doors
// All procedures before the gripper test must return 1
//  so the gripper test can be cancelled
// ******************************************************

int16 chkdoor(void)
{		
	int16 fire_sw_loc = 0;
	int16 enable_cc;
	int16 sec_override;
	int16 fl_ix;
	int16 elof_fl;
 	//lint -esym(438,tmppos)
	//lint -esym(550,tmppos)
 	int16 tmppos = 0;
	static int16 prev_Sec_Fl_Mask;
	static int16 any_hall_call_dbnc;

#if((Traction == 0) && (Tract_OL == 0))
	int recall_fl;
#endif

	write_trace(TR_CHKDOOR);
	

	if ((cons[ansi] >= 2004) && (cons[firesv] == 2) && (rdinp(i_FFS) == 0))
		FF_STP = 0;
	else
		FF_STP = 1;

  	if ((firef != 0) || (rfiref != 0)) 
	{
		fdoor_req = 0;
		rdoor_req = 0;
		if ((enable_cc_test == 1) || (continue_cc_test == 1))
		{
			cancel_cc_test = 1;
			enable_cc_test = 0;
			continue_cc_test = 0;
		}
	}
	
	if ((fvars[fvno_simul_do] != 0) || ((servf == s_SABBATH) && ((fvars[fvsabben2] & 0x04) != 0)))
		no_simul_do = 1;
	else
		no_simul_do = 0;
	
	if (((rdinp(i_AD) == 0) || ((statusf2 & sf_Setup_Svc) != 0) ||
		(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) ||
		(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ad_off == 1))) && (firef == 0) && (rfiref == 0))       
	{
		if ( (servf == s_IND) || (servf == s_ATT) || 
			( (nmb_carcalls == 0) && 
			  ((rdinp(i_DCL) == 0) || (manual_door == 2)) && 
			  (front_slowdown == 0) &&
			  ((cons[rear] == 0) || 
				((cons[rear] != 0) && 
				 ((rdinp(i_DCLR) == 0) || (manual_rdoor == 2)) &&
				 (rear_slowdown == 0)) ) ) ) 
			disable_auto_door = 1;
	}
	else
		disable_auto_door = 0;

	get_onward_calls();

	if ( ( (rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
		   ((eef2_enabled == 1) && (rdinp(i_EE2) == 1)) ) &&	// EE2 input ON
	 	  		(fvars[fveeto] != 0) && ((fvars[fvndgctl] & 0x01) == 0) )
	{
		if (timers[teeto] > fvars[fveeto])
		{
			if (ee_to == 0)
				record_fault(f_fdet_edg_to);		// Detector Edge Time-out
			ee_to = 1;
		}
		else
			ee_to = 0;
	}
	else
	{
		ee_to = 0;
		timers[teeto] = 0;
	}

	if ((cons[rear] != 0) && 
		( (rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		  ((eer2_enabled == 1) && (rdinp(i_EER2) == 1)) ) && 	// EER2 input ON
				(fvars[fveeto] != 0) && ((fvars[fvndgctl] & 0x01) == 0))
	{
		if (timers[treeto] > fvars[fveeto])
		{
			if (ree_to == 0)
				record_fault(f_rdet_edg_to);		// Detector Edge Time-out
			ree_to = 1;
		}
		else
			ree_to = 0;
	}
	else
	{
		ree_to = 0;
		timers[treeto] = 0;
	}


	if (((rdinp(i_SE) == 1) && (firef <= 3)) ||   // saftey edge
	 	 (((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
		   ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && (firef == 0)) ||   // EE2 input ON
	 	 (((rd_both_inps(i_DOB,i_XDOB) == 1) ||
		  ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) ||
		  ((remote_cc == 1) && ((rdinp(i_RM_DOB) == 1) || ((tugf == 1)&&(rdinp(i_TGDO) == 1)))) ||
		  (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDOB) == 1)))
	 		&& (FS_DOB_DIS == 0)) )// door open button
		f_door_open_device = 1;
	else
		f_door_open_device = 0;

	
	if (((rdinp(i_SER) == 1) && (rfiref <= 3)) ||   // saftey edge
	 	(((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 ((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) && (rfiref == 0)) ||   		// EER2 input ON
	 	(((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
		  ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
		  ((remote_rcc == 1) && ((rdinp(i_RM_DOBR) == 1) || ((tugf == 1)&&(rdinp(i_TGDOR) == 1)))) ||
		  (((cons[frghtd] == 11) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDOBR) == 1)))
	 		&& (FS_DOB_DIS == 0)) )// door open button
		r_door_open_device = 1;
	else
		r_door_open_device = 0;


	if ((manual_door == 2) || (manual_rdoor == 2))
	{
		if (man_dcc() == 0) 
			statusf |= sf_DCC;
		else
			statusf &= ~sf_DCC;
	}

    //not on a door zone make sure doors are closed
  	if (rdinp(i_DZ) == 0) 
  	{													 
		if( (safe() == 0) || (rdinp(i_DCL) == 1) )
		{
			if (doorf != 4)
		    	doorf = 3;
		    timers[tdo] = 0;
		    timers[tdc] = 0;
		    timers[tdwel] = 0;
		    timers[tlant] = 0;
		    timers[trlant] = 0;
		    timers[tcablnt] = 0;
		    timers[trcablnt] = 0;
		    keep_lants_on = 0;
		    keep_rlants_on = 0;
		    timers[tdcycle] = 0;
		}
		else
		    doorf = 0;
		if(cons[rear] != 0)
		{
			if( (safe() == 0) ||
			    (rdinp(i_DCLR) == 1) )
			{
				if (rdoorf != 4)
			    	rdoorf = 3;
			    timers[trdo] = 0;
			    timers[trdc] = 0;
			    timers[trdwel] = 0;
//			    timers[trnudge] = 0;
			    timers[tdcycle] = 0;
			}
			else
			    rdoorf = 0;
		}
  	}

	setvipf();
	setcodebf();
	sethsvf();
  	setfsf();
  	setempf();
	seteqf();
	setmedf();
	settugf();



	// Doors operate according to the elevator services from highest to lowest priority 
	//Door Monitoring in phase 2
	if((cons[firesv] != 1) && ((firef >= 4) || ((cons[rear] != 0) && (rfiref >= 4))))
	{
		if((lock_gs_dpm_check() == 0))		// DPM, DL and GS Door check
		{
			lockgsdpm_fault_doorop();
			return(0);
		}
		
	}
	else if((cons[firesv] == 1) && ((firef >= 4) || ((cons[rear] != 0) && (rfiref >= 4))))
	{//Door Monitoring in phase 2 NY only
		if(gs_only_check() == 0)		// DPM and GS Door check
		{
			lockgsdpm_fault_doorop();
			return(0);	
		}
		
	}


	// Phase 2 Door operation for emergency power recovery (not enough power to run to ALL landings)
	if(((cons[ansi] == 2007) || (cons[ansi] == 2010)) &&
		(cons[Emp_Recover] != 0))
	{
	  	if (emp_doorop() != 0)			// Doors operate according to the empf, if empf set
	  		return(1);
	}


// fire service phase 2 door operation
  	if((firef == 4) || (firef == 7))      // door hold
  	{
		if (rdinp(i_DOL) == 1)
		{	 // doors are not open but should be
			if ((doorf != 1) || (doorf != 2))
				doorf = 1;
		}
		else if ((manual_door == 0) || (manual_door == 2))
		    doorf = 2;          // dwell door
    	timers[tdc] = 0;
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
		timers[teeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
  	}
// fire service phase 2 rear door operation
  	if((rfiref == 4) || (rfiref == 7))      // door hold
  	{
		if (rdinp(i_DOLR) == 1)
		{
			if ((rdoorf != 1) || (rdoorf != 2))
				rdoorf = 1;
		}
		else if ((manual_rdoor == 0) || (manual_rdoor == 2))
		    rdoorf = 2;          // dwell door
    	timers[trdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
  	}
  	if(firef == 5)        // peek a boo
  	{
		if (fs2_no_fdoor_op == 0)
		{	// front doors operation allowed
			
	    	if ((rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
			{
	    	  	doorf = 1;        // open door
			}
	    	else
	    	  	doorf = 3;        // close door
	    	  	
		}

//		if((manual_door == 2) && (man_dcc() == 0) && (momentary_dcb == 1)&&(fvars[fvreopendoorfs2] == 1))
//			doorf = 1;        // open door

 //		Instead of being an option we are making this change permanent NYC

		if ((momentary_dcb == 1) && 	// when NYC fs DCB pressed
			(((manual_door == 2) && (man_dcc() == 0)) || 	// auto swing doors and door contacts made
			 ((fvars[fvreopendoorfs2] == 1) && (rdinp(i_MRSW) == 0))) )	// reopen on stop switch enabled
		{
			doorf = 1;        // open door
		}

	   	carcall();
    	if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
		{
		    clrcc(0);
			dpref = 0;
		}
    	timers[tdc] = 0;
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
		timers[teeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
  	}
  	if(rfiref == 5)        // peek a boo
  	{
		if (fs2_no_rdoor_op == 0)
		{	// rear doors operation allowed
	    	if ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)))
	    	  	rdoorf = 1;        // open door
	    	else
	    	  	rdoorf = 3;        // close door

		}
		
//		if((manual_rdoor == 2) && (man_rdcc() == 0) && (momentary_rdcb == 1)&&(fvars[fvreopendoorfs2] == 1))
//			rdoorf = 1;        // open door

//		Instead of being an option we are making this change permanent NYC
		if ((momentary_rdcb == 1) &&
			(((manual_rdoor == 2) && (man_rdcc() == 0)) ||
			 ((fvars[fvreopendoorfs2] == 1) && (rdinp(i_MRSW) == 0))))
		{
			rdoorf = 1;        // open door
		}

    	carcall();
    	if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
		{
		    clrcc(0);
			dpref = 0;
		}
    	timers[trdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
  	}
  	if(firef == 6)        // constant pressure close
  	{
 		if (fs2_no_fdoor_op == 0)
		{	// front doors operation allowed
	    	if ( (((rd_both_inps(i_DCB,i_XDCB) == 0) && ((cons[RGB_CC] == 0) || (rdinp(i_FiDCB) == 0))) ||
	    		(((rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) && (fvars[fvdobovrdcb] == 1))) && 	// allow door to open button to close door with stuck door close button
	    		((cons[Australia] == 0) || (fs2_start == 0)) &&  ((dcalls == 0) || (cons[firesv] != 3)) )
			{
				if ((cons[Australia] == 1) && (doorf == 3))
					clrcc(0);			// reopen because switch was removed after closing so cancel calls
	    	  	doorf = 1;        // open door
			}
	    	else if ((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1)))
	    	  	doorf = 3;        // close door
		}

    	carcall();
    	if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
		{
		    clrcc(0);
			dpref = 0;
		}
    	timers[tdc] = 0;
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
		timers[teeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
  	}
  	if(rfiref == 6)        // constant pressure close
  	{
		if (fs2_no_rdoor_op == 0)
		{	// rear doors operation allowed
		   	if ( (((rd_both_inps(i_DCBR,i_XDCBR) == 0) && ((cons[RGB_CC] == 0) || (rdinp(i_FiDCBR) == 0))) ||
	    		(((rd_both_inps(i_DOBR,i_XDOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1))) && (fvars[fvdobovrdcb] == 1))) &&	   // allow door to open button to close door with stuck door close button
	    		((cons[Australia] == 0) || (fs2_start == 0)) && ((dcalls == 0) || (cons[firesv] != 3)) )
			{
				if ((cons[Australia] == 1) && (rdoorf == 3))
					clrcc(0);			// reopen because switch was removed after closing so cancel calls
	    	  	rdoorf = 1;        // open door
			}
	    	else if ((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1)))
	    	  	rdoorf = 3;        // close door
		}
		
    	carcall();
    	if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
		{
		    clrcc(0);
			dpref = 0;
		}
    	timers[trdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
  	}
  	if(firef == 8)        // momentary pressure close
  	{
  		if (rdinp(i_DOL) == 1)
		{	 // doors are not open but should be
			if ((doorf != 1) || (doorf != 2))
				doorf = 1;
		}
		else if ((manual_door == 0) || (manual_door == 2))
		    doorf = 2;          // dwell door   
		    
  	
    	if (((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1))) &&
		  ((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))) )
    	{
      		doorf = 3;        // close door
      		firef = 5;
			momentary_dcb = 1;      		
    	}
    	carcall();
    	if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
		{
		    clrcc(0);
			dpref = 0;
		}
    	timers[tdc] = 0;
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
		timers[teeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
  	}
  	if(rfiref == 8)        // momentary pressure close
  	{
  		if (rdinp(i_DOLR) == 1)
		{
			if ((rdoorf != 1) || (rdoorf != 2))
				rdoorf = 1;
		}
		else if ((manual_rdoor == 0) || (manual_rdoor == 2))
		    rdoorf = 2;          // dwell door  	
  	
    	if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1))) &&
			((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
    	{
	      	rdoorf = 3;        // close door
	      	rfiref = 5;
 			momentary_rdcb = 1;	      	
	   	}
    	carcall();
    	if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
		{
		    clrcc(0);
			dpref = 0;
		}
    	timers[trdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
  	}

  	if ( ((firef == 1) && (fvars[fvfs1f] != position)) ||
	  	((firef == 2) && (fvars[fvfs1af] != position)) ) 
  	{    // on phase one and not at the return floor
		if( (fs2_to_fs1 == 1) && ((fs2_on == 1) || ((fs2_hold == 1) && (cons[firesv] != 3))) )
		{		// return to phase 2
			fire_buz = 0;
	  		if(cons[rear] != 0)
	  		{
				if ((rdinp(i_DCLR) != 0) || 
				  		((rdinp(i_RGS) == 0) && (((cons[frghtd] != 0) && (cons[frghtd] <= 7)) || (manual_rdoor != 0))))
				{
					if ((rdinp(i_DCL) != 0)  || 
						((rdinp(i_GS) == 0) && (((cons[frghtd] != 0) && (cons[frghtd] <= 6)) || (manual_door != 0))))
					{
						firef = 6;
					}
					else
					{
					  	firef = 5;
					}
				}
				else
				{
					if ((rdinp(i_DCL) != 0) || 
						((rdinp(i_GS) == 0) && (((cons[frghtd] != 0) && (cons[frghtd] <= 6)) || (manual_door != 0))))
					{
						firef = 6;
					}
				}
				if ((rdinp(i_DCL) != 0) || 
				  	((rdinp(i_GS) == 0) && (((cons[frghtd] != 0) && (cons[frghtd] <= 6)) || (manual_door != 0))))
				{
					if ((rdinp(i_DCLR) != 0) || 
						((rdinp(i_RGS) == 0) && (((cons[frghtd] != 0) && (cons[frghtd] <= 7)) || (manual_rdoor != 0))))
					{
						rfiref = 6;
					}
					else
					{
						rfiref = 5;
					}
				}
				else
				{
					if ((rdinp(i_DCLR) != 0) || 
						((rdinp(i_RGS) == 0) && (((cons[frghtd] != 0) && (cons[frghtd] <= 7)) || (manual_rdoor != 0))))
					{
						rfiref = 6;
					}
				}
			}
	  		else
	  		{
				if ((rdinp(i_DCL) != 0) || 
					((rdinp(i_GS) == 0) && (((cons[frghtd] != 0) && (cons[frghtd] <= 6)) || (manual_door != 0))))
				{
					firef = 6;
				}
	  		}
		}
	}

  	if( (firef >= 4) || (rfiref >= 4) )
  	{
		chkdoor_state = 1;
    	return(1);
  	}
	
	// Medical emergency service

	if (medf == 3)
	{		  // Keep the door open unless call is set and key switch is on
//		get_onward_calls();
    	if(ccmsk[cons[carnmb]][position] != 0)
    	{
			if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
			{
			    clrcc(0);
				dpref = 0;
			}
			if ((fvars[fvmedreopen] == 2) && (doorf == 1))
			{
				if (rdinp(i_DOL) == 0)
					doorf = 2;
				else
					doorf = 1;
			}
			else if ((fvars[fvmedreopen] != 0) && ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) || 
				 (((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
		 		 ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && (firef == 0)) || 	// EE2 input ON
				 ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))))
			{
				doorf = 1;    // open door
			}
			else if(((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) || (onward_cc != 0)) &&
				((rdinp(i_MRSW) == 1) && (FF_STP == 1)) && (rdinp(i_EMS) == 1) &&					   
				((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))) )
			{
			    if(rdinp(i_DCL) == 1)
					doorf = 3;      // close door
			    else
					doorf = 0;
			}
			else
			     doorf = 1;        // open door
    	}
    	if( (cons[rear] != 0) && (rccmsk[cons[carnmb]][position] != 0) )
    	{
			if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
			{
			    clrcc(0);
				dpref = 0;
			}
			if ((fvars[fvmedreopen] == 2) && (rdoorf == 1))
			{
				if (rdinp(i_DOLR) == 0)
					rdoorf = 2;
				else
					rdoorf = 1;
			}
			else if ((fvars[fvmedreopen] != 0) && ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
				(((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 		 ((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) && (rfiref == 0))||			// EER2 input ON
				(rdinp(i_SER) == 1)))
			{
				rdoorf = 1;    // rear open door
			}
			else if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) || (onward_cc != 0)) &&
				((rdinp(i_MRSW) == 1) && (FF_STP == 1)) &&  (rdinp(i_EMS) == 1) &&
				((manual_rdoor == 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
			{
			    if(rdinp(i_DCLR) == 1)
					rdoorf = 3;      // close door
			    else
					rdoorf = 0;
			}
			else
			    rdoorf = 1;        // open door
    	}
    	carcall();
    	timers[tdo] = 0;
    	timers[tdc] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdo] = 0;
    	timers[trdc] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
    	carcancel();
		ck_swing_door();
		chkdoor_state = 2;
    	return(1);
  	}


// earth quake open doors keep them open
 	if((cons[equake] != 0) && (eqf != 0))
  	{
    	if(ccnmsk[cons[carnmb]][position] != 0)
		    doorf = 1;
    	else
    	{
			if(rccnmsk[cons[carnmb]][position] != 0)
				rdoorf = 1;
    	}
		dpref = 0;
		dcalls = 0;
    	clrcc(0);
    	clrhc();
    	clrlant();
    	timers[tdo] = 0;
    	timers[tdc] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[trdo] = 0;
    	timers[trdc] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
		ck_swing_door();
		chkdoor_state = 3;
    	return(1);
  	}

  	if (emp_doorop() != 0)			// Doors operate according to the empf, if empf set
  		return(1);


// low oil or stall condition cycle doors and turn off

  	if(stallf != 0)
  	{
    	servf = s_STALLED;
    	clrcc(0);
    	clrhc();
    	clrlant();
    	timers[tdc] = 0;
    	timers[trdc] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
		timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
		timers[tdcycle] = 0;
		// low oil and at the bottom floor open doors
#if((Traction == 0) && (Tract_OL == 0))
		if (firef == 1)
			recall_fl = fvars[fvfs1f];
		else if (firef == 2) 
			recall_fl = fvars[fvfs1af];
		else if (firef == 3)
			recall_fl = position;	// current floor
		else
			recall_fl = cons[bottomf];		// Set floor to bottom
		//if position fault present, car should not open doors until DT breaks or in a recall floor in the middle of hoistway
		if ((stallf == 1) && (((position == recall_fl) && (recall_fl != cons[bottomf])) || (rdinp(i_DTsel) == 0)))
#else
		if (stallf == 1)
#endif
		{
		  	stallf = 1;      // This will let the se,ee, and dob work
	    	do_seq = 0;
			rdo_seq = 0;
	    	timers[tdo] = 0;
	    	timers[trdo] = 0;
	    	timers[tnudge] = 0;
	    	timers[trnudge] = 0;
		  	if (rdinp(i_DZ) == 1)
			{
			  	if(ccnmsk[cons[carnmb]][position] != 0)
			  	{
					doorf = 1;	  
					do_to = 0;
					stallf = 2;
			  	}
			  	else
			  	{
					if(rccnmsk[cons[carnmb]][position] != 0)
					{
						rdoorf = 1;
						rdo_to = 0;
						stallf = 2;
					}
			  	}
#if((Traction == 0) && (Tract_OL == 0))
				if (stallf == 2)
				{  
					dpref = 0;
					dcalls = 0;
					clr_dir_arrow();
				}
#endif
			}
			else stallf = 4;
		}
#if((Traction == 0) && (Tract_OL == 0))
		else if (stallf == 1)	// but not at the correct landing
		{
			if ((dcalls == 2) && (dpref == 2))
			{
				if(ccnmsk[cons[carnmb]][position] != 0)
				{
					if( ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) ||
						((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) &&
						 (FS_DOB_DIS == 0) )
						doorf = 1;    // open door
					else if ((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1)))
					{
						if (timers[tnudge] > fvars[fvndt])
							doorf = 4; 	// nudge the door closed
						else
							doorf = 3;    // close door
					}
				}
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					if( ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
						((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
						(rdinp(i_SER) == 1)) &&
						 (FS_DOB_DIS == 0) )
						rdoorf = 1;    // rear open door
					else if ((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1)))
					{
						if (timers[trnudge] > fvars[fvndt])
							rdoorf = 4; 	// nudge the door closed
						else
							rdoorf = 3;    // rear close door
					}
				}
			}
			else
			{
				 // binary position inputs floor confirm (only comes in here if lost and stalled)
				if(cons[BinPiIn] == 1)        // binary position inputs floor confirm
			 	{
			    	if((rdinp(i_UL) == 1) && (rdinp(i_DL) == 1))      // dead level
			    	{
			  	   		tmppos = 0;
			      		if(rdinp(i_BP1) == 1)
							tmppos = (tmppos + 1);
			      		if(rdinp(i_BP2) == 1)
							tmppos = (tmppos + 2);
			      		if((cons[topf] > 3) && (rdinp(i_BP4) == 1))
							tmppos = (tmppos + 4);
			      		if((cons[topf] > 7) && (rdinp(i_BP8) == 1))
							tmppos = (tmppos + 8);
			      		if((cons[topf] > 15) && (rdinp(i_BP16) == 1))
							tmppos = (tmppos + 16);
						if ((tmppos > 0) && (tmppos <= cons[topf]))
				    		position = tmppos;
						preset = 1;
					}
				}
			}
		}
#endif
		if ((stallf == 2) || (((doorf == 1) || (rdoorf == 1)) && (stallf != 1)))
		{
		  	tmppos = 0; //placed here to avoid warnings
		  	timers[tdwel] = 0;
		  	timers[trdwel] = 0;
			if (do_seq == 0)
	    		timers[tnudge] = 0;
			if (rdo_seq == 0)
	    		timers[trnudge] = 0;
			doort = fvars[fvexdt];
			rdoort = fvars[fvexdt];
		  	if(ccnmsk[cons[carnmb]][position] != 0)
		  	{
				doorf = 1;	  
				if ((rdinp(i_DOL) == 0) || (do_to == 1))     // doors are open
				{
					do_seq = 4;
					stallf = 3;
				}
		  	}
		  	else
		  	{
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					rdoorf = 1;
					if ((rdinp(i_DOLR) == 0) || (rdo_to == 1))      // doors are open
					{
						rdo_seq = 4;
						stallf = 3;
					}
				}
			}
		}
		// dwell the doors
		if(stallf == 3)
		{
		  	if(ccnmsk[cons[carnmb]][position] != 0)
		  	{
				if (manual_door == 2)
				{
			    	timers[tdo] = 0;
			    	timers[tnudge] = 0;
				  	timers[tdwel] = 0;
					if ((doorf != 1) || (doorf != 2))
						doorf = 1;
				}
				else
				{
					if ((rdinp(i_DOL) == 1) && (do_to == 0))
					{
						if ((doorf != 1) || (doorf != 2))
							doorf = 1;
					}
					else if (do_to == 1)
						stallf = 4;
					else
						doorf = 2;
			  	  	if(timers[tdwel] >= fvars[fvexdt])
					{
						doorf = 3;
						stallf = 4;
					}
					if (timers[tnudge] > fvars[fvndt])
					{
						doorf = 4;
					    stallf = 4;
					}
				}
		  	}
		  	else
		  	{
		  	  	if(rccnmsk[cons[carnmb]][position] != 0)
		  	  	{
					if (manual_rdoor == 2)
					{
				    	timers[trdo] = 0;
				    	timers[trnudge] = 0;
					  	timers[trdwel] = 0;
						if ((rdoorf != 1) || (rdoorf != 2))
							rdoorf = 1;
					}
					else
					{
						if ((rdinp(i_DOLR) == 1) && (rdo_to == 0))
						{
						 	if ((rdoorf != 1) || (rdoorf != 2))
						 		rdoorf = 1;
						}
						else if (rdo_to == 1)
							stallf = 4;
						else
						     rdoorf = 2;
			  	  	    if(timers[trdwel] >= fvars[fvexdt])
						{
							rdoorf = 3;
						    stallf = 4;
						}
						if (timers[trnudge] > fvars[fvndt])
						{
							rdoorf = 4;
						    stallf = 4;
						}
			  	  	}
				}
		  	}
    	}
    	// close the doors
    	if(stallf == 4)
    	{
    	  	if(ccnmsk[cons[carnmb]][position] != 0)
    	  	{
				if ((doorf != 3) && (timers[tnudge] > fvars[fvndt]))
					doorf = 4;
				else
					doorf = 3;
				if(rd_both_inps(i_DOB,i_XDOB) == 1) // THIS SECTION WAS ADDED TO ALLOW DOORS TO REOPEN. BEFORE THEY WILL CLOSE ON PEOPLE
			  	{
					stallf = 1;
			  	}
				if (rdinp(i_DCL) == 0)       // doors are closed
				    procf = 17;     // go to the low oil procedure and shut off
    	  	}
    	  	else
    	  	{
				if (rccnmsk[cons[carnmb]][position] != 0)
				{
					if ((rdoorf != 3) && (timers[trnudge] > fvars[fvndt]))
						rdoorf = 4;
					else
					 	rdoorf = 3;
					if (rdinp(i_DCLR) == 0) // doors are closed
						procf = 17;     // go to the low oil procedure and shut off
				}
    	  	}
    	}
		ck_swing_door();
		chkdoor_state = 8;
   		return(1);
  	}


// hot oil door operation

  	if(hotoilf != 0)
  	{
		servf = s_HOT_OIL;
		clrcc(0);	// don't answer more car calls
    	clrhc();
    	clrlant();
    	timers[tdc] = 0;
    	timers[trdc] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
		timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
		timers[tdcycle] = 0;
		
		if (firef == 1)
			recall_fl = fvars[fvfs1f];
		else if (firef == 2) 
			recall_fl = fvars[fvfs1af];
		else if (firef == 3)
			recall_fl = position;	// current floor
		else
			recall_fl = cons[bottomf];		// Set floor to bottom

		if ((hotoilf == 1) && (((position == recall_fl) && (recall_fl != cons[bottomf])) || (rdinp(i_DTsel) == 0)))
		{
			// This will let the se,ee, and dob work
	    	do_seq = 0;
			rdo_seq = 0;
	    	timers[tdo] = 0;
	    	timers[trdo] = 0;
	    	timers[tnudge] = 0;
	    	timers[trnudge] = 0;
		  	if (rdinp(i_DZ) == 1)
			{
			  	if(ccnmsk[cons[carnmb]][position] != 0)
			  	{
					doorf = 1;	  
					do_to = 0;
					hotoilf = 2;
			  	}
			  	else
			  	{
					if(rccnmsk[cons[carnmb]][position] != 0)
					{
						rdoorf = 1;
						rdo_to = 0;
						hotoilf = 2;
					}
			  	}
				if (hotoilf == 2)
				{  
					dpref = 0;
					dcalls = 0;
					clr_dir_arrow();
				}
			}
			else 
				hotoilf = 4;
		}
		else if (hotoilf == 1)	// but not at the correct landing
		{
			if ((dcalls == 2) && (dpref == 2))
			{
				if(ccnmsk[cons[carnmb]][position] != 0)
				{
					if( ((rdinp(i_DOB) == 1) || (rdinp(i_SE) == 1) ||
						((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) &&
						 (FS_DOB_DIS == 0) )
						doorf = 1;    // open door
					else if ((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1)))
					{
						if (timers[tnudge] > fvars[fvndt])
							doorf = 4; 	// nudge the door closed
						else
							doorf = 3;    // close door
					}
				}
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					if( ((rdinp(i_DOBR) == 1) || 
						((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
						(rdinp(i_SER) == 1)) &&
						 (FS_DOB_DIS == 0) )
						rdoorf = 1;    // rear open door
					else if ((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1)))
					{
						if (timers[trnudge] > fvars[fvndt])
							rdoorf = 4; 	// nudge the door closed
						else
							rdoorf = 3;    // rear close door
					}
				}
			}
			else
			{
				 // binary position inputs floor confirm (only comes in here if lost and stalled)
				if(cons[BinPiIn] == 1)        // binary position inputs floor confirm
			 	{
			    	if((rdinp(i_UL) == 1) && (rdinp(i_DL) == 1))      // dead level
			    	{
			  	   		tmppos = 0;
			      		if(rdinp(i_BP1) == 1)
							tmppos = (tmppos + 1);
			      		if(rdinp(i_BP2) == 1)
							tmppos = (tmppos + 2);
			      		if((cons[topf] > 3) && (rdinp(i_BP4) == 1))
							tmppos = (tmppos + 4);
			      		if((cons[topf] > 7) && (rdinp(i_BP8) == 1))
							tmppos = (tmppos + 8);
			      		if((cons[topf] > 15) && (rdinp(i_BP16) == 1))
							tmppos = (tmppos + 16);
						if ((tmppos > 0) && (tmppos <= cons[topf]))
				    		position = tmppos;
						preset = 1;
					}
				}
			}
		}
		if ((hotoilf == 2) || (((doorf == 1) || (rdoorf == 1)) && (hotoilf != 1)))
		{
		  	tmppos = 0; //placed here to avoid warnings
		  	timers[tdwel] = 0;
		  	timers[trdwel] = 0;
			if (do_seq == 0)
	    		timers[tnudge] = 0;
			if (rdo_seq == 0)
	    		timers[trnudge] = 0;
			doort = fvars[fvexdt];
			rdoort = fvars[fvexdt];
		  	if(ccnmsk[cons[carnmb]][position] != 0)
		  	{
				doorf = 1;	  
				if ((rdinp(i_DOL) == 0) || (do_to == 1))     // doors are open
				{
					do_seq = 4;
					hotoilf = 3;
				}
		  	}
		  	else
		  	{
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					rdoorf = 1;
					if ((rdinp(i_DOLR) == 0) || (rdo_to == 1))      // doors are open
					{
						rdo_seq = 4;
						hotoilf = 3;
					}
				}
			}
		}
		// dwell the doors
		if(hotoilf == 3)
		{
		  	if(ccnmsk[cons[carnmb]][position] != 0)
		  	{
				if (manual_door == 2)
				{
			    	timers[tdo] = 0;
			    	timers[tnudge] = 0;
				  	timers[tdwel] = 0;
					if ((doorf != 1) || (doorf != 2))
						doorf = 1;
				}
				else
				{
					if ((rdinp(i_DOL) == 1) && (do_to == 0))
					{
						if ((doorf != 1) || (doorf != 2))
							doorf = 1;
					}
					else if (do_to == 1)
						hotoilf = 4;
					else
						doorf = 2;
			  	  	if(timers[tdwel] >= fvars[fvexdt])
					{
						doorf = 3;
						hotoilf = 4;
					}
					if (timers[tnudge] > fvars[fvndt])
					{
						doorf = 4;
					    hotoilf = 4;
					}
				}
		  	}
		  	else
		  	{
		  	  	if(rccnmsk[cons[carnmb]][position] != 0)
		  	  	{
					if (manual_rdoor == 2)
					{
				    	timers[trdo] = 0;
				    	timers[trnudge] = 0;
					  	timers[trdwel] = 0;
						if ((rdoorf != 1) || (rdoorf != 2))
							rdoorf = 1;
					}
					else
					{
						if ((rdinp(i_DOLR) == 1) && (rdo_to == 0))
						{
						 	if ((rdoorf != 1) || (rdoorf != 2))
						 		rdoorf = 1;
						}
						else if (rdo_to == 1)
							hotoilf = 4;
						else
						     rdoorf = 2;
			  	  	    if(timers[trdwel] >= fvars[fvexdt])
						{
							rdoorf = 3;
						    hotoilf = 4;
						}
						if (timers[trnudge] > fvars[fvndt])
						{
							rdoorf = 4;
						    hotoilf = 4;
						}
			  	  	}
				}
		  	}
    	}
    	// close the doors
    	if(hotoilf == 4)
    	{
    	  	if(ccnmsk[cons[carnmb]][position] != 0)
    	  	{
				if ((doorf != 3) && (timers[tnudge] > fvars[fvndt]))
					doorf = 4;
				else
					doorf = 3;
				if(rdinp(i_DOB) == 1) // THIS SECTION WAS ADDED TO ALLOW DOORS TO REOPEN. BEFORE THEY WILL CLOSE ON PEOPLE
			  	{
					hotoilf = 1;
			  	}
				if (rdinp(i_DCL) == 0)       // doors are closed
				    procf = 22;     // go to the low oil procedure and shut off
    	  	}
    	  	else
    	  	{
				if (rccnmsk[cons[carnmb]][position] != 0)
				{
					if ((rdoorf != 3) && (timers[trnudge] > fvars[fvndt]))
						rdoorf = 4;
					else
					 	rdoorf = 3;
					if (rdinp(i_DCLR) == 0) // doors are closed
						procf = 22;     // go to the low oil procedure and shut off
				}
    	  	}
    	}
		ck_swing_door();
		chkdoor_state = 9;
   		return(1);
  	}

	// Medical Emergency door operation

	if ((medf == 1) && (position == fvars[fvmedfl]))
	{			// On Medical Emergency Recall and at the floor
    	if ((ccnmsk[cons[carnmb]][position] != 0) && ((rccnmsk[cons[carnmb]][position] == 0) || (fvars[fvmedloc] == 0)))
    	{			 // Fire switch at front door and valid front landing 
			if(rdinp(i_DOL) == 1)        // doors are not open
		   		doorf = 1;        // open door
			else
		   		doorf = 2;        // dwell door
			
			// close the rear door unless someone hitting a door open device			
			if ((rd_both_inps(i_DOBR,i_XDOBR) == 1) ||
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) || 
				(rdinp(i_SER) == 1))
				rdoorf = 1;    // rear open door
			else
				rdoorf = 3;    // rear close door
    	}
    	else if ((rccnmsk[cons[carnmb]][position] != 0) && ((ccnmsk[cons[carnmb]][position] == 0) || (fvars[fvmedloc] != 0)))
		{		  // Fire switch at rear door and valid rear landing
		    if(rdinp(i_DOLR) == 1)        // rear doors are not open
		       rdoorf = 1;        // rear open door
		    else
		       rdoorf = 2;        // rear dwell door
			
			// close the front door unless someone hitting a door open device
			if ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
				doorf = 1;    // open door
			else
				doorf = 3;    // close door
    	}
    	timers[tdo] = 0;
		timers[tdc] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdo] = 0;
    	timers[trdc] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
		timers[tdcycle] = 0;
		clrcc(0);
		clrhc();
		clrlant();
		ck_swing_door();
		chkdoor_state = 10;
		return(1);
  	}

	if ((medf == 1) && (position != fvars[fvmedfl]))
	{
		if ((rdinp(i_MRSW) == 0) || (FF_STP == 0))
			doorf = 1;    // open door
		else if ((fvars[fvmedindovr] == 1) && (ind() == 1) && (timers[tmedf] < 300))		   
		{
 			if(ccnmsk[cons[carnmb]][position] != 0)
 			{
 				if (((rd_both_inps(i_DCB,i_XDCB) == 1) ||
 					((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) ||
 					((remote_cc == 1) && (rdinp(i_RM_DCB) == 1))) &&
 					((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))))
 					doorf = 3;
 				else
 					doorf = 1;
 			}
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) ||
					((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) ||
					((remote_rcc == 1) && (rdinp(i_RM_DCBR) == 1))) &&
					((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
					rdoorf = 3;
				else
					rdoorf = 1;
			}
		}  
		else if ((rdinp(i_ATT) == 1) && (cons[att] != 0) && (timers[tmedf] < 300)) 
		{						 // must give attendant time to close the door
			if(ccnmsk[cons[carnmb]][position] != 0)
			{
				if ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) || 	
					(((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||		// EE1 input ON
		 			 ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && (firef == 0)) ||		// EE2 input ON
					 ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
				{
					doorf = 1;    // open door
				}
				else if (((rd_both_inps(i_DCB,i_XDCB) == 1) ||
						  ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) || 
						  (rdinp(i_ATTUP) == 1) || 
						  (rdinp(i_ATTDN) == 1))	&&
						  	((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))) )
				{
				    doorf = 3;	  // close door
				}
			}

			if (rccnmsk[cons[carnmb]][position] != 0)
		    {
			    if((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
					(((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 			 ((eer2_enabled == 1) && (rdinp(i_EER2) == 1)))  && (rfiref == 0)) ||			// EER2 input ON
					 (((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1)) && (rfiref == 0))||
					 (rdinp(i_SER) == 1))
				{
					rdoorf = 1;    // rear open door
				}
				else if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) ||
					 ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) || 
					 (rdinp(i_ATTUP) == 1) || 
					 (rdinp(i_ATTDN) == 1)) &&
					 	((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
				{
				    rdoorf = 3;
				}
			}
		}
		else
		{
			if(ccnmsk[cons[carnmb]][position] == 1)
			{
				if ((fvars[fvmedreopen] == 2) && (doorf == 1))
				{
					if (rdinp(i_DOL) == 0)
						doorf = 2;
					else
						doorf = 1;
				}
				else if ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) || 
					(((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||		// EE1 input ON
		 			 ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && (firef == 0)) ||		// EE2 input ON
					 ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
				{
					doorf = 1;    // open door
				}
				else if ((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1)))
				{
					doorf = 3;    // close door
				}
			}
			if(rccnmsk[cons[carnmb]][position] == 1)
			{
				if ((fvars[fvmedreopen] == 2) && (rdoorf == 1))
				{
					if (rdinp(i_DOLR) == 0)
						rdoorf = 2;
					else
						rdoorf = 1;
				}
				else if ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
					(((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 			 ((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) && (rfiref == 0)) ||			// EER2 input ON
					 (((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1)) && (rfiref == 0)) ||
					 (rdinp(i_SER) == 1))
				{
					rdoorf = 1;    // rear open door
				}
				else if ((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1)))
				{
					rdoorf = 3;    // rear close door
				}
			}
		}

		timers[tdc] = 0;
		timers[tdo] = 0;
   		timers[tdwel] = 0;
    	timers[trdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdc] = 0;
    	timers[trdo] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrcc(0);
    	clrhc();
    	clrlant();
		ck_swing_door();
		chkdoor_state = 11;
		return(1);
	}


  	if(medf == 2)
  	{		// Car at the medical emergency floor
    	if ((ccnmsk[cons[carnmb]][position] != 0) && ((rccnmsk[cons[carnmb]][position] == 0) || (fvars[fvmedloc] == 0)))
    	{
			if (rdinp(i_DOL) == 1)
			{
				if ((doorf != 1) || (doorf != 2))
					doorf = 1;
			}
			else
				doorf = 2;

			// close the rear door unless someone hitting a door open device			
			if ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
				(rdinp(i_SER) == 1))
				rdoorf = 1;    // rear open door
			else
				rdoorf = 3;    // rear close door
    	}
    	else if ((rccnmsk[cons[carnmb]][position] != 0) && ((ccnmsk[cons[carnmb]][position] == 0) || (fvars[fvmedloc] != 0)))
    	{
			if (rdinp(i_DOLR) == 1)
			{
				if ((rdoorf != 1) || (rdoorf != 2))
					rdoorf = 1;
			}
			else
			    rdoorf = 2;

			// close the front door unless someone hitting a door open device
			if ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) ||
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
				doorf = 1;    // open door
			else
				doorf = 3;    // close door
    	}
    	timers[tdc] = 0;
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrcc(0);
    	clrhc();
    	clrlant();
		ck_swing_door();
		chkdoor_state = 12;
    	return(1);
  	}

  	if (medf != 0)
	{
		ck_swing_door();
		chkdoor_state = 13;
		return(1);
	}

	// Hospital Emergency service
  	if ((cons[cblue] == 1) && ((rdinp(i_EMS) == 1) || (ems_onecall == 1)) && (hsvf == 1) && ((firef == 0) || (hsvf == 1))) // constant pressure close
  	{
    	if(ccmsk[cons[carnmb]][position] != 0)
    	{
			if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
			{
			    clrcc(0);
				dpref = 0;
				clr_dir_arrow();
			}
			if (((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) || 
				((onward_cc != 0) && (fvars[fvhsvdccc] == 1) && (f_door_open_device == 0)) ||
				((cons[rear] != 0) && ((rd_both_inps(i_DCBR,i_XDCBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)))) ||
				((remote_cc == 1) && (rdinp(i_RM_DCB) == 1))) && 
				((rdinp(i_MRSW) == 1) && (FF_STP == 1)) &&
				((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))) )
			{
			    if(rdinp(i_DCL) == 1)
					doorf = 3;      // close door
			    else
					doorf = 0;
			}
			else if ((front_slowdown != 0) || (rdinp(i_DCL) == 1) || (cons[rear] == 0) ||
					(rd_both_inps(i_DOB,i_XDOB) == 1) || 
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) ||
					((cons[rear] != 0) && (rear_slowdown == 0) && (rdinp(i_DCLR) == 0))) 
			     doorf = 1;        // open door
    	}
    	if( (cons[rear] != 0) && (rccmsk[cons[carnmb]][position] != 0) )
    	{
			if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
			{
			    clrcc(0);
				dpref = 0;
			}
			if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) || 
				((onward_cc != 0) && (fvars[fvhsvdccc] == 1) && (r_door_open_device == 0)) ||
				(rd_both_inps(i_DCB,i_XDCB) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) || 
				((remote_rcc == 1) && (rdinp(i_RM_DCBR) == 1))) && 
				((rdinp(i_MRSW) == 1) && (FF_STP == 1)) &&
				((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
			{
			    if(rdinp(i_DCLR) == 1)
					rdoorf = 3;      // close door
			    else
					rdoorf = 0;
			}
			else if ((rear_slowdown != 0) || (rdinp(i_DCLR) == 1) ||
					(rd_both_inps(i_DOBR,i_XDOBR) == 1) ||
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)))
			    rdoorf = 1;        // open door
    	}
		if(fvars[fvcbsinglecall] == 0)
		{
			carcall();
		}
		else if((nmb_carcalls < 1))
		{
			carcall();
		}
    	timers[tdo] = 0;
    	timers[tdc] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdo] = 0;
    	timers[trdc] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
    	carcancel();
		ck_swing_door();
		chkdoor_state = 14;
    	return(1);
  	}

  	
	if(fvars[fvcboverfs] == 1)
	{
			// code blue service door operation

		if (codebf == 1)
		{		// Being recalled to the code blue floor and closing the door
			if ((ind() == 0) && (stallf == 0))
			{		 // not on ind 
				clrcc(0);
				clrhc();
				clrlant();
				timers[tdc] = 0;
				timers[tdo] = 0;
				timers[tnudge] = 0;
				timers[trdc] = 0;
				timers[trdo] = 0;
				timers[trnudge] = 0;
				timers[teeto] = 0;
				timers[treeto] = 0;
				timers[tlant] = 0;
				timers[trlant] = 0;
				keep_lants_on = 0;
				keep_rlants_on = 0;
				timers[tcablnt] = 0;
				timers[trcablnt] = 0;
				timers[tdcycle] = 0;
				timers[tdwel] = 0;
				timers[trdwel] = 0;
		
				if(ccnmsk[cons[carnmb]][position] != 0)
				{
					if ((((rdinp(i_DOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) && 
						(rdinp(i_DCL) == 1)) || 
							(rdinp(i_SE) == 1) || (doorf == 1))
						doorf = 1;    // open door if door open device or door opening, allow it to open all the way
					else
					{
						if(cb_over_ind == 1)
							doorf = 4;    // close door in nudging
						else
							doorf = 3;    // close door
					}
				}
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					if ((((rdinp(i_DOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1))) && 
							(rdinp(i_DCLR) == 1)) || 
						(rdinp(i_SER) == 1) || (rdoorf == 1))
						rdoorf = 1;    // rear open door if door open device or door opening, allow it to open all the way
					else
					{
						if(cb_over_ind == 1)
							rdoorf = 4;    // close door in nudging
						else
							rdoorf = 3;    // rear close door
					}
				}
				ck_swing_door();
				chkdoor_state = 15;
				return(0);
			}
		}

		if((codebf == 2) || (codebf == 3) || (codebf == 4))
		{
			if((fvars[fvcbsinglecall] == 0)||(codebf == 2))
			{
				clrcc(0);
				clrhc();
				clrlant();
				dpref = 0;
				timers[tdc] = 0;
				timers[tdo] = 0;
				timers[tnudge] = 0;
				timers[trdc] = 0;
				timers[trdo] = 0;
				timers[trnudge] = 0;
				timers[teeto] = 0;
				timers[treeto] = 0;
				timers[tlant] = 0;
				timers[trlant] = 0;
				keep_lants_on = 0;
				keep_rlants_on = 0;
				timers[tcablnt] = 0;
				timers[trcablnt] = 0;
				timers[tdcycle] = 0;
				doort = fvars[fvcbtim];		// set timer for 3 minute
				rdoort = fvars[fvcbtim];		// set timer for 3 minute
			}
			else		  // codebf = 3 or codebf = 4 and waiting for single call
			{
				if(nmb_carcalls < 1)
				{
					carcall();
				}
			}
			if (codebf == 2)
			{
				timers[tdwel] = 0;
				timers[trdwel] = 0;
			}
			if (g_cb_fl[cons[carnmb]] != 0)
			{	// Front floor
				if(ccnmsk[cons[carnmb]][position] != 0)
				{
					if (rdinp(i_DOL) == 1)
					{
						if ((doorf != 1) || (doorf != 2))
							doorf = 1;
					}
					else
						doorf = 2;
				}
			}
			else
			{	// Rear floor
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					if (rdinp(i_DOLR) == 1)
					{
						if ((rdoorf != 1) || (rdoorf != 2))
							rdoorf = 1;
					}
					else
						rdoorf = 2;
				}
			}
			ck_swing_door();
			chkdoor_state = 16;
			return(0);
		}

	}

	//Door Monitoring in phase 1
	if((cons[firesv] == 2) || (cons[firesv] == 3) ||		// not a new york job
		((cons[firesv] == 1) && (fs2_to_fs1 == 0)))			// ny job going to fs for the first time
	{
		if((firef != 0) || ((cons[rear] != 0) && (rfiref != 0)))
		{
			if(lock_gs_dpm_check() == 0)		// DPM, DL and GS Door check
			{
				lockgsdpm_fault_doorop();
				return(0);
			}
		}
	}

	// fire service phase 1 door operation

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

 	//cancel gripper test since we are in fire service
	if((firef != 0) ||(rfiref != 0))
	{
		if(in_gripper_test == 1)
			in_gripper_test = 0;
	}
	
 	if(( ((firef == 1) && (fvars[fvfs1f] == position)) ||
      	((firef == 2) && (fvars[fvfs1af] == position)) ) && 
       	(((cons[BinPiIn] == 0) && (cons[sel_type] == 0)) || (preset_pos == 0)))
  	{     // on phase one and at the return floor
		

 		fire_fdo_seq = 0;
		fire_rdo_seq = 0;
	
	   	if ((ccnmsk[cons[carnmb]][position] != 0) && ((rccnmsk[cons[carnmb]][position] == 0) || (fire_sw_loc == 0)))
    	{			 // Fire switch at front door and valid front landing 
			if ( (rdinp(i_DOL) == 1) && ((cons[rear] == 0) || (no_simul_do == 0) || 
					(rdinp(i_DCLR) == 0) ||
					(rdinp(i_RGS) == 1) || (rdinp(i_RGS_1) == 1)) )
		   		doorf = 1;        // doors are not open: open door
			else if ((rdinp(i_DOL) == 0) || (cons[rear] == 0) || (no_simul_do == 0))
		   		doorf = 2;        // dwell door
			else
				doorf = 0;

			// close the rear door unless someone hitting a door open device			
			if( ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
				(rdinp(i_SER) == 1)) &&
				 (FS_DOB_DIS == 0) && ((rdinp(i_DCL) == 0) || (no_simul_do == 0) || 
		    	 (rdinp(i_GS) == 1) || (rdinp(i_GS_1) == 1)) )  // okay to open rear or front closed
				rdoorf = 1;    // rear open door
			else
				rdoorf = 3;    // rear close door
    	}
    	else if ((rccnmsk[cons[carnmb]][position] != 0) && ((ccnmsk[cons[carnmb]][position] == 0) || (fire_sw_loc != 0)))
		{		  // Fire switch at rear door and valid rear landing
		    if ( (rdinp(i_DOLR) == 1) && 
		    	((rdinp(i_DCL) == 0) || (no_simul_do == 0) || 
		    	 (rdinp(i_GS) == 1) || (rdinp(i_GS_1) == 1)) )       
		       rdoorf = 1;        // rear doors are not open: open REAR door
		    else if ((rdinp(i_DOLR) == 0) || (no_simul_do == 0))
		       rdoorf = 2;        // rear dwell door
			else 
				rdoorf = 0;
			
			// close the front door unless someone hitting a door open device
			if ( ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) ||
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) &&
				 (FS_DOB_DIS == 0) && ((cons[rear] == 0) || (no_simul_do == 0) || 
					(rdinp(i_DCLR) == 1) ||
					(rdinp(i_RGS) == 1) || (rdinp(i_RGS_1) == 1)) )
				doorf = 1;    // open door
			else
				doorf = 3;    // close door
    	}
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
		timers[tdcycle] = 0;
		clrcc(0);
		clrhc();
		clrlant();
		ck_swing_door();
		chkdoor_state = 17;
		return(1);
  	}

  	if ( ((firef == 1) && (fvars[fvfs1f] != position)) ||
	  	((firef == 2) && (fvars[fvfs1af] != position)) ) 
  	{    // on phase one and not at the return floor

		fire_fdo_seq = 0;
		fire_rdo_seq = 0;

		if ((rdinp(i_MRSW) == 0) || (FF_STP == 0))
		{
			if(fs2_to_fs1 == 0)
			{
				if(ccnmsk[cons[carnmb]][position] != 0)
					doorf = 1;    // open door
				if(rccnmsk[cons[carnmb]][position] != 0)
					rdoorf = 1;	  // open rear door
			}
			else	//Transitioning from fs2 to fs1 (only reopen door if not clos
			{
				if (rdinp(i_DCL) == 1)
					doorf = 1;
				if (((cons[rear] == 1) && (rdinp(i_DCLR) == 1)))
					rdoorf = 1;
			}
		}
		else if( ((ind() == 1) || ((rdinp(i_ATT) == 1) && (cons[att] != 0))) && 
			((timers[tfs1t] < fvars[fvfs1dto]) && (FS_DOB_DIS == 0)) )
		{						 // must give attendant time to close the door
			if (ind() == 1)
			{
				if(ccnmsk[cons[carnmb]][position] != 0)
				{
					if (((rd_both_inps(i_DCB,i_XDCB) == 1) ||
						((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) ||
						((remote_cc == 1) && (rdinp(i_RM_DCB) == 1))) &&
						((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))) )
						doorf = 3;
					else if (doorf != 0)
						doorf = 1;
				}
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) ||
						((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) ||
						((remote_rcc == 1) && (rdinp(i_RM_DCBR) == 1))) &&
						((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
						rdoorf = 3;
					else if (rdoorf != 0)
						rdoorf = 1;
				}
			}
			else if ((cons[att] != 0) && (rdinp(i_ATT) == 1))
			{
				if(ccnmsk[cons[carnmb]][position] != 0)
				{
					if((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) ||
						((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
						doorf = 1;    // open door
					else if (((rd_both_inps(i_DCB,i_XDCB) == 1) || 
							  ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) ||
							  (rdinp(i_ATTUP) == 1) || 
							  (rdinp(i_ATTDN) == 1))	&&
							  	((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))) )
					    doorf = 3;	  // close door
				}

				if(rccnmsk[cons[carnmb]][position] != 0)
		    	{
					if((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
						((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
						(rdinp(i_SER) == 1))
						rdoorf = 1;    // rear open door
					if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) ||
						((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) || 
						 (rdinp(i_ATTUP) == 1) || 
						 (rdinp(i_ATTDN) == 1)) &&
						 	((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
					    rdoorf = 3;
				}
			}
		}
		else
		{
			if(ccnmsk[cons[carnmb]][position] != 0)		// phase 1 recall and not at rec fl and has normal openning 
			{
				if( ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) ||
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) &&
					 (FS_DOB_DIS == 0) )
					doorf = 1;    // open door
				else if ((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1)))
				{
					if (cons[ansi] >= 2013)
					{
						if (doorf == 2)
							doorf = 3;
					}
					else
					{
						doorf = 3;    // close door
					}
				}
			}
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if( ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
					(rdinp(i_SER) == 1)) &&
					 (FS_DOB_DIS == 0) )
				{
					rdoorf = 1;    // rear open door					
				}
				else if ((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1)))
				{
					if (cons[ansi] >= 2013)
					{
						if (rdoorf == 2)
							rdoorf = 3;
					}
					else
					{
						rdoorf = 3;    // rear close door
					}
				}
			}
		}
		timers[tdc] = 0;
		timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrcc(0);
    	clrhc();
    	clrlant();
		ck_swing_door();
		chkdoor_state = 18;
    	return(1);
  	}

  	if (firef == 3)
  	{

  		if (fvars[fvdenfsv] == 1)
		{
			if ( (car_Grp_Stat.c_car_hc_fire_door_open == 1) || ((fs_on == 1) && (prev_fs_on == 0)) )
			{				  // hall call door open request or fs switch changed from off to on
				fire_fdo_seq = 0;
				fire_rdo_seq = 0;
			}
			prev_fs_on = fs_on;
		}


    	if ((ccnmsk[cons[carnmb]][position] != 0) && ((rccnmsk[cons[carnmb]][position] == 0) || (fire_sw_loc == 0)))
    	{
    	
			if (fvars[fvdenfsv] == 1)
			{				   // Special denver fire service closes the door after time-out on phase 1.
				if ((fire_fdo_seq == 0) || (rd_both_inps(i_DOB,i_XDOB) == 1))
				{
					timers[tfire_fdo] = 0;
					if (rdinp(i_DOL) == 0)
					{
						doorf = 2;
						fire_fdo_seq = 1;
					}
					else
					{
						doorf = 1;
						if (rd_both_inps(i_DOB,i_XDOB) == 1)
							fire_fdo_seq = 0;
					}
				}
				else
				{
					if (timers[tfire_fdo] < fvars[fvf1dwell])
						doorf = 2;
					else 
						doorf = 3; 
				}
			}
			else
			{		// normal phase 1 door open
				if ( (rdinp(i_DOL) == 1) && ((cons[rear] == 0) || (no_simul_do == 0) || 
						(rdinp(i_DCLR) == 0) ||
						(rdinp(i_RGS) == 1) || (rdinp(i_RGS_1) == 1)) )
				{
					if ((doorf != 1) || (doorf != 2))
						doorf = 1;
				}
				else if ((rdinp(i_DOL) == 0) || (cons[rear] == 0) || (no_simul_do == 0))
			   		doorf = 2;        // dwell door
				else
					doorf = 0;
			}
			
			// close the rear door unless someone hitting a door open device			
			if( ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
				(rdinp(i_SER) == 1)) &&
				 (FS_DOB_DIS == 0) && ((rdinp(i_DCL) == 0) || (no_simul_do == 0) || 
		    	 (rdinp(i_GS) == 1) || (rdinp(i_GS_1) == 1)) )  // okay to open rear or front closed
				rdoorf = 1;    // rear open door
			else
				rdoorf = 3;    // rear close door
    	}
    	else if ((rccnmsk[cons[carnmb]][position] != 0) && ((ccnmsk[cons[carnmb]][position] == 0) || (fire_sw_loc != 0)))
    	{

			if (fvars[fvdenfsv] == 1)
			{				   // Special denver fire service closes the door after time-out on phase 1.
				if ((fire_rdo_seq == 0) || (rd_both_inps(i_DOBR,i_XDOBR) == 1))
				{
					timers[tfire_rdo] = 0;
					if (rdinp(i_DOLR) == 0)
					{
						rdoorf = 2;
						fire_rdo_seq = 1;
					}
					else
					{
						rdoorf = 1;
						if (rd_both_inps(i_DOBR,i_XDOBR) == 1)
							fire_rdo_seq = 1;
					}
				}
				else
				{
					if (timers[tfire_rdo] < fvars[fvf1dwell])
						rdoorf = 2;
					else 
						rdoorf = 3; 
				}
			}
			else
			{		// normal phase 1 door open
			    if ( (rdinp(i_DOLR) == 1) && 
			    	((rdinp(i_DCL) == 0) || (no_simul_do == 0) || 
			    	 (rdinp(i_GS) == 1) || (rdinp(i_GS_1) == 1)) )       
				{
					if ((rdoorf != 1) || (rdoorf != 2))
						rdoorf = 1;
				}
			    else if ((rdinp(i_DOLR) == 0) || (no_simul_do == 0))
			       rdoorf = 2;        // rear dwell door
				else 
					rdoorf = 0;
			}
			
			// close the front door unless someone hitting a door open device
			if ( ((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) ||
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) &&
				 (FS_DOB_DIS == 0) && ((cons[rear] == 0) || (no_simul_do == 0) || 
					(rdinp(i_DCLR) == 1) ||
					(rdinp(i_RGS) == 1) || (rdinp(i_RGS_1) == 1)) )
				doorf = 1;    // open door
			else
				doorf = 3;    // close door
    	}
    	timers[tdc] = 0;
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrcc(0);
    	clrhc();
    	clrlant();
		ck_swing_door();
		chkdoor_state = 19;
    	return(1);
  	}

  	if ((firef != 0) || (rfiref != 0)) 
	{
		ck_swing_door();
		chkdoor_state = 20;
		return(1);
	}


	if (at_floor_check() == 0)
	{
		chkdoor_state = 21;
		return(0);
	}

	//Door Monitoring
	if(lock_gs_dpm_check() == 0)	// DPM, DL and GS Door check
	{
		lockgsdpm_fault_doorop();
		return(0);
	}

// code blue service door operation

  	if (codebf == 1)
	{		// Being recalled to the code blue floor and closing the door
		if ((ind() == 0) && (stallf == 0))
		{		 // not on ind 
		    clrcc(0);
		    clrhc();
		    clrlant();
		    timers[tdc] = 0;
		    timers[tdo] = 0;
		    timers[tnudge] = 0;
		    timers[trdc] = 0;
		    timers[trdo] = 0;
		    timers[trnudge] = 0;
			timers[teeto] = 0;
			timers[treeto] = 0;
		    timers[tlant] = 0;
	    	timers[trlant] = 0;
		    keep_lants_on = 0;
		    keep_rlants_on = 0;
		    timers[tcablnt] = 0;
		    timers[trcablnt] = 0;
		    timers[tdcycle] = 0;
		    timers[tdwel] = 0;
		    timers[trdwel] = 0;
	
			if(ccnmsk[cons[carnmb]][position] != 0)
			{
				if ((((rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) && 
					(rdinp(i_DCL) == 1)) || 
						(rdinp(i_SE) == 1) || (doorf == 1))
					doorf = 1;    // open door if door open device or door opening, allow it to open all the way
				else
				{
					if(cb_over_ind == 1)
						doorf = 4;    // close door in nudging
					else
						doorf = 3;    // close door
				}
			}
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if ((((rd_both_inps(i_DOBR,i_XDOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1))) && 
						(rdinp(i_DCLR) == 1)) || 
					(rdinp(i_SER) == 1) || (rdoorf == 1))
					rdoorf = 1;    // rear open door if door open device or door opening, allow it to open all the way
				else
				{
					if(cb_over_ind == 1)
						rdoorf = 4;    // close door in nudging
					else
						rdoorf = 3;    // rear close door
				}
			}
			ck_swing_door();
			chkdoor_state = 24;
			return(0);
		}
	}

  	if((codebf == 2) || (codebf == 3) || (codebf == 4))
  	{
		if((fvars[fvcbsinglecall] == 0)||(codebf == 2))
		{
		    clrcc(0);
		    clrhc();
		    clrlant();
			dpref = 0;
		    timers[tdc] = 0;
		    timers[tdo] = 0;
		    timers[tnudge] = 0;
		    timers[trdc] = 0;
		    timers[trdo] = 0;
		    timers[trnudge] = 0;
			timers[teeto] = 0;
			timers[treeto] = 0;
		    timers[tlant] = 0;
	    	timers[trlant] = 0;
		    keep_lants_on = 0;
		    keep_rlants_on = 0;
		    timers[tcablnt] = 0;
		    timers[trcablnt] = 0;
		    timers[tdcycle] = 0;
			doort = fvars[fvcbtim];		// set timer for 3 minute
			rdoort = fvars[fvcbtim];		// set timer for 3 minute
		}
		else		  // codebf = 3 or codebf = 4 and waiting for single call
		{
			if(nmb_carcalls < 1)
			{
				carcall();
			}
		}
		
		if (codebf == 2)
		{
		    timers[tdwel] = 0;
		    timers[trdwel] = 0;
		}
		if (car_Grp_Stat.c_cb_fl != 0)
		{	// Front floor
		    if(ccnmsk[cons[carnmb]][position] != 0)
			{
				if (rdinp(i_DOL) == 1)
				{
					if ((doorf != 1) || (doorf != 2))
						doorf = 1;
				}
				else
					doorf = 2;
		    }
		}
		else
	    {	// Rear floor
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if (rdinp(i_DOLR) == 1)
				{
					if ((rdoorf != 1) || (rdoorf != 2))
						rdoorf = 1;
				}
				else
				    rdoorf = 2;
			}
	    }
		ck_swing_door();
		chkdoor_state = 25;
	    return(0);
  	}

// Prioroty Service (vip) door operation

  	if (vipf == 1)
	{		// Being recalled to the vip floor and closing the door
		if ((ind() == 0) && (stallf == 0))
		{		 // not on ind 
		    clrcc(0);
		    clrhc();
			if (fvars[fvviplant] == 0)
		    	clrlant();
		    timers[tdc] = 0;
		    timers[tdo] = 0;
		    timers[tnudge] = 0;
		    timers[trdc] = 0;
		    timers[trdo] = 0;
		    timers[trnudge] = 0;
			timers[teeto] = 0;
			timers[treeto] = 0;
		    timers[tlant] = 0;
	    	timers[trlant] = 0;
		    keep_lants_on = 0;
		    keep_rlants_on = 0;
		    timers[tcablnt] = 0;
		    timers[trcablnt] = 0;
		    timers[tdcycle] = 0;
		    timers[tdwel] = 0;
		    timers[trdwel] = 0;
	
			if(ccnmsk[cons[carnmb]][position] != 0)
			{
				if ((((rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) && 
					(rdinp(i_DCL) == 1)) || 
						(rdinp(i_SE) == 1) || (doorf == 1))
					doorf = 1;    // open door if door open device or door opening, allow it to open all the way
				else
					doorf = 3;    // close door
			}
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if ((((rd_both_inps(i_DOBR,i_XDOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1))) && 
					(rdinp(i_DCLR) == 1)) || 
					(rdinp(i_SER) == 1) || (rdoorf == 1))
					rdoorf = 1;    // rear open door if door open device or door opening, allow it to open all the way
				else
					rdoorf = 3;    // rear close door
			}
			ck_swing_door();
			chkdoor_state = 26;
			return(0);
		}
	}

  	if ((vipf == 2) || (vipf == 3))
  	{
		if ((ind() == 0) && (stallf == 0))
		{		 // not on ind 
			if (vipf == 3)
			{
				if ((cons[vip_en] & 0x02) != 0)
				{	// only allow car calls if VPECC enable input is ON
					if (rdinp(i_VPECC) != 0)
						carcall();
				}
				else
					carcall();
		    }
		    clrhc();
			if (fvars[fvviplant] == 0)
			    clrlant();
			dpref = 0;
		    timers[tdc] = 0;
		    timers[tdo] = 0;
		    timers[tnudge] = 0;
		    timers[trdc] = 0;
		    timers[trdo] = 0;
		    timers[trnudge] = 0;
			timers[teeto] = 0;
			timers[treeto] = 0;
		    timers[tlant] = 0;
	    	timers[trlant] = 0;
		    keep_lants_on = 0;
		    keep_rlants_on = 0;
		    timers[tcablnt] = 0;
		    timers[trcablnt] = 0;
		    timers[tdcycle] = 0;
			doort = fvars[fvviptim];		// set timer for 3 minute
			rdoort = fvars[fvviptim];		// set timer for 3 minute
			if (vipf == 2)
			{
			    timers[tdwel] = 0;
			    timers[trdwel] = 0;
			}
			if (car_Grp_Stat.c_vip_fl != 0)
			{	// Front floor
			    if(ccnmsk[cons[carnmb]][position] != 0)
				{
					if (rdinp(i_DOL) == 1)
					{
						if ((doorf != 1) || (doorf != 2))
							doorf = 1;
					}
					else
						doorf = 2;
			    }
			}
			else
		    {	// Rear floor
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					if (rdinp(i_DOLR) == 1)
					{
						if ((rdoorf != 1) || (rdoorf != 2))
							rdoorf = 1;
					}
					else
					    rdoorf = 2;
				}
		    }
			ck_swing_door();
			chkdoor_state = 27;
		    return(0);
		}
  	}

// independent service door operation
  	if ((ind() == 1) && (FS_DOB_DIS == 0)) // constant pressure close
  	{
		if (servf != s_IND)
		{
			clrcc(0);
			dpref = 0;
			clr_dir_arrow();
			if (((fvars[fvno_simul_do] == 2) && (rccmsk[cons[carnmb]][position] != 0)) || (ccmsk[cons[carnmb]][position] == 0))
				ind_door_sel = 1;		// rear door first
			else
				ind_door_sel = 0;		// 0 = front door, 1 = rear 
    	}
		else if(((fvars[fvheofovrind]& 0x01) != 0)&&((cons[Elev_off] & 4) != 0)&&(rdinp(i_HEOF) == 1))
		{
			heof_over_ind = 1;
		}	
		
    	servf = s_IND;
       	if (((cons[BinPiIn] == 0) && (cons[sel_type] == 0)) || (preset_pos == 0))
		{
			
	    	if ((ccmsk[cons[carnmb]][position] != 0) ||
	   		 	 ((ccnmsk[cons[carnmb]][position] != 0) && ((fvars[fvdobsec] == 2) || (fvars[fvdobsec] == 5))))
	    	{
				if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
				{
				    clrcc(0);
					dpref = 0;
					clr_dir_arrow();
				}
				if (((rd_both_inps(i_DCB,i_XDCB) == 1) || 
					((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) ||
					((onward_cc != 0) && (fvars[fvinddccc] == 1) && (doorf != 1) && (f_door_open_device == 0)) ||
					((remote_cc == 1) && ((rdinp(i_RM_DCB) == 1)
											||((tugf == 1)&&(rdinp(i_TGDC) == 1))))) && 
					((rdinp(i_MRSW) == 1) && (FF_STP == 1)) &&
					((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))))
			   	{
				    if(rdinp(i_DCL) == 1)
						doorf = 3;      // close door
				    else
					{
						doorf = 0;
						if ((cons[rear] != 0) && (rccmsk[cons[carnmb]][position] != 0) && (onward_cc == 0))
							ind_door_sel = 1;
						else
							ind_door_sel = 0;
					}
				}
				else if ((front_slowdown != 0) || (rdinp(i_DCL) == 1) || (cons[rear] == 0) ||
						(rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) || 
						(rccmsk[cons[carnmb]][position] == 0) ||
						((cons[rear] != 0) && (ind_door_sel == 0) && (rear_slowdown == 0) &&
							(rdinp(i_DCLR) == 0) && (onward_cc == 0))) 
				    doorf = 1;        // open door
	    	}
	    	
	    	if (((cons[rear] != 0) && (rccmsk[cons[carnmb]][position] != 0)) ||
	    			(((rccnmsk[cons[carnmb]][position] != 0) && ((fvars[fvdobsec] == 2) || (fvars[fvdobsec] == 5)))))
	    	{
				if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
				{
				    clrcc(0);
					dpref = 0;
				}
				if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) || 
					((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) ||
					((onward_cc != 0) && (fvars[fvinddccc] == 1) && (rdoorf != 1) && (r_door_open_device == 0)) ||
					((remote_rcc == 1) && (rdinp(i_RM_DCBR) == 1))) && 
					((rdinp(i_MRSW) == 1) && (FF_STP == 1)) &&
					((manual_rdoor == 0) || (cons[frghtd] != 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
				{
				    if(rdinp(i_DCLR) == 1)
						rdoorf = 3;      // close door
				    else
					{
						rdoorf = 0;
						if ((ccmsk[cons[carnmb]][position] != 0) && (onward_cc == 0))
							ind_door_sel = 0;
						else
							ind_door_sel = 1;
					} 
				}
				else if ((rear_slowdown != 0) || (rdinp(i_DCLR) == 1) ||
						(ccmsk[cons[carnmb]][position] == 0) || (rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
						((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
						((ind_door_sel == 1) && (front_slowdown == 0) && (rdinp(i_DCL) == 0) && (onward_cc == 0)))
				    rdoorf = 1;        // open door
	    	}
		}
    	carcall();
    	timers[tdo] = 0;
    	timers[tdc] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[trdo] = 0;
    	timers[trdc] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
    	keep_lants_on = 0;
	    keep_rlants_on = 0;
    	timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
    	timers[tdcycle] = 0;
    	clrhc();
    	clrlant();
    	carcancel();
		ck_swing_door();
		if(((fvars[fvheloff2] & 0x08) != 0) && (rdinp(i_HEOF) == 0))
		{	  
			clroutp(o_HEOFL);
		}

		if((fvars[fvindrclen] == 1) && (ind() == 1))
			next_stop_lobby();
		
		chkdoor_state = 28;

    	return(0);
  	}
  	else
  	{
		if(servf == s_IND)
			servf = s_AUTO;
  	}

#if ((Traction == 1) || (Tract_OL == 1))
	if ( (((rdinp(i_OVL) == 1) && ((cons[loadw] & 1) != 0) && (lw_disable == 0)) || (((cons[loadw] & 2) != 0) && (load_ovl == 1)))
		 && (firef == 0) && (rfiref == 0))
#else
	if (((rdinp(i_OVL) == 1) && ((cons[loadw] & 1) != 0)) && (firef == 0) && (rfiref == 0))
#endif
	{		// Load overload
		servf = s_LW_OVL;
		in_overload = 1;
	    clrcc(0);
	    clrhc();
	    clrlant();
    	if(ccmsk[cons[carnmb]][position] != 0)
    	{
		  	if(rdinp(i_DOL) == 1)
		  	    doorf = 1;      //keep the doors dwelling if the stop switch pulled
		  	else
		  	    doorf = 2;      // open the doors if the stop switch pulled
    	}
    	if(rccmsk[cons[carnmb]][position] != 0)
    	{
		  	if(rdinp(i_DOLR) == 1)
		  	    rdoorf = 1;      //keep the doors dwelling if the stop switch pulled
		  	else
		  	    rdoorf = 2;      // open the doors if the stop switch pulled
    	}
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[tdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
    	timers[trdc] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
		if ((cons[sercom] & 2) != 0)
			setoutp(o_OLL);
		ck_swing_door();
		chkdoor_state = 29;
		return(0);
	}
	else if (in_overload == 1)
	{
		in_overload = 0;
		if ((cons[sercom] & 2) != 0)
			clroutp(o_OLL);
		if (servf == s_LW_OVL)
			servf = s_AUTO;
	}

	if((fvars[fvnohcreop] & 0x01) != 0)
	{
		nohcreopen = 1;
	}
	else if(((fvars[fvnohcreop] & 0x02) != 0)&&(car_Grp_Stat.c_local_up_peak == 1))
	{
	 	nohcreopen = 1;
	}
	else
	{
	 	nohcreopen = 0;
	}
	
	//LOGIC FOR EOF
	if((firef == 0) && (rfiref == 0) && (codebf == 0) && (eqf == 0) && (medf == 0))
	{
		if ((((rdinp(i_ELOF) == 1) && ((cons[Elev_off] & 1) != 0) && ((cons[loadw] & 1) == 0)) ||
			(((cons[hugs] == 1) || (cons[hugs] == 2)|| (cons[hugs] == 3)) && (hugsf != 0))))
		{		// Elevator Off
			servf = s_ELEV_OFF_CS;
		    clrcc(0);
		    clrhc();
		    clrlant();
			if ((rdinp(i_ELOF) == 1) && ((cons[Elev_off] & 1) != 0))
			{
		    	if(ccmsk[cons[carnmb]][position] != 0)
		    	{
				  	if(rdinp(i_DOL) == 1)
				  	    doorf = 1;      //open the doors
				  	else
				  	    doorf = 2;      // keep door dwelling
		    	}
		    	if(rccmsk[cons[carnmb]][position] != 0)
		    	{
				  	if(rdinp(i_DOLR) == 1)
				  	    rdoorf = 1;      //open the doors
				  	else
				  	    rdoorf = 2;      // keep the doors dwelling
		    	}
			}
			else if (((cons[hugs] == 1)||(cons[hugs] == 2)||(cons[hugs] == 3)) && (hugsf != 0))
			{
		    	if ((ccmsk[cons[carnmb]][position] != 0) && ((hugsf & 0x01) != 0))
		    	{
				  	if(rdinp(i_DOL) == 1)
				  	    doorf = 1;      //open the doors
				  	else
				  	    doorf = 2;      // keep door dwelling
		    	}
		    	if ((rccmsk[cons[carnmb]][position] != 0) && ((hugsf & 0x02) != 0))
		    	{
				  	if(rdinp(i_DOLR) == 1)
				  	    rdoorf = 1;      //open the doors
				  	else
				  	    rdoorf = 2;      // keep the doors dwelling
		    	}
			}
	    	timers[tdo] = 0;
	    	timers[tdwel] = 0;
	    	timers[tnudge] = 0;
	    	timers[tdc] = 0;
	    	timers[trdo] = 0;
	    	timers[trdwel] = 0;
	    	timers[trnudge] = 0;
			timers[teeto] = 0;
			timers[treeto] = 0;
	    	timers[trdc] = 0;
	    	timers[tlant] = 0;
	    	timers[trlant] = 0;
			ck_swing_door();
			chkdoor_state = 30;
			return(0);
		}
		else if (servf == s_ELEV_OFF_CS)
			servf = s_AUTO;
	}
	
  	carcall();

  	hallcall();
  	
  	if(dpref == 0)
	{
    	dirsel();
    	timers[tlant] = 0;
    	timers[trlant] = 0;
	}
	
	if((cons[Prison] != 0) && (remote_cc == 1))
	{
 		if(cons[Prison] == 2)
			servf = s_RIOT;
	   	if(ccmsk[cons[carnmb]][position] != 0)
		{
			if(rdinp(i_RM_DCB) == 1)
			{
				if(cons[Prison] != 2)
					timers[tnit] = fvars[fvnit]+1;
				doorf = 3;
			}
			else if(rdinp(i_RM_DOB) == 1) //prison
			{
				doorf = 1;
			}
			else
			{
				if (rdinp(i_DOL) == 0)
					doorf = 2;
				else
					doorf = 0;
			}
			if ((rdinp(i_DOL) == 0) && (rdinp(i_DCL) == 1))
				timers[tnit] = fvars[fvnit]+1;
	    	timers[tdc] = 0;
	    	timers[tdo] = 0;
	    	timers[tdwel] = 0;
	    	timers[tnudge] = 0;
			timers[teeto] = 0;
		}
		else
		{
			doorf = 0;
		}
	  	carcancel();
  		hallcancel();

		set_auto_servf();
		jail_break();
		chkdoor_state = 31;
		return(0);

	}
	else
	{
 		if((cons[Prison] != 0) && (remote_cc == 0) && (servf == s_RIOT))
 			servf = s_AUTO;
	}
	

	if (((cons[pbdoor] & 3) != 0) || ((cons[pbdoormsk] != 0) && ((pbdoor_fmsk[position] != 0) || (pbdoor_rmsk[position] != 0))))
	{
    	if(ccmsk[cons[carnmb]][position] != 0)
		{
			if ((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)))
			{
				timers[tnit] = fvars[fvnit]+1;
				doorf = 3;
			}
			else if ((rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
				doorf = 1;
			else if (((cons[pbdoor] & 2) != 0) || ((cons[pbdoormsk] != 0) && (pbdoor_fmsk[position] != 0)))
			{
				if (rdinp(i_DOL) == 0)
					doorf = 2;
				else
					doorf = 0;
			}
			else if ((doorf == 1) && (rdinp(i_DOL) == 0))
				doorf = 2;
			else if ((doorf == 3) && (rdinp(i_DCL) == 0))
				doorf = 0;
			if ((rdinp(i_DOL) == 0) && (rdinp(i_DCL) == 1))
				timers[tnit] = fvars[fvnit]+1;
	    	timers[tdc] = 0;
	    	timers[tdo] = 0;
	    	timers[tdwel] = 0;
	    	timers[tnudge] = 0;
			timers[teeto] = 0;
		}
		else
			doorf = 0;
    	if ((cons[rear] != 0) && (rccmsk[cons[carnmb]][position] != 0))
		{
			if ((rd_both_inps(i_DCBR,i_XDCBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)))
			{
				timers[tnit] = fvars[fvnit]+1;
				rdoorf = 3;
			}
			else if ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)))
				rdoorf = 1;
			else if (((cons[pbdoor] & 2) != 0) || ((cons[pbdoormsk] != 0) && (pbdoor_rmsk[position] != 0)))
			{
				if (rdinp(i_DOLR) == 0)
					rdoorf = 2;
				else
					rdoorf = 0;
			}
			else if ((rdoorf == 1) && (rdinp(i_DOLR) == 0))
				rdoorf = 2;
			else if ((rdoorf == 3) && (rdinp(i_DCLR) == 0))
				rdoorf = 0;

			if ((rdinp(i_DOLR) == 0) &&
				(rdinp(i_DCLR) == 1))
				timers[tnit] = fvars[fvnit]+1;
	    	timers[trdc] = 0;
	    	timers[trdo] = 0;
	    	timers[trdwel] = 0;
	    	timers[trnudge] = 0;
			timers[treeto] = 0;
		}
		else 
			rdoorf = 0;

	  	carcancel();
  		hallcancel();

		set_auto_servf();
		chkdoor_state = 32;
		return(0);
	}

  	if (((rdinp(i_MRSW) == 0) || (FF_STP == 0)) && (firef == 0) && (rdinp(i_DZ) == 1))        // stop switch pulled
  	{
	  	if ((ccmsk[cons[carnmb]][position] != 0) || (ucmsk[cons[carnmb]][position] != 0) || (dcmsk[cons[carnmb]][position] != 0))
    	{
		  	if(rdinp(i_DOL) == 1)
		  	    doorf = 1;      //keep the doors dwelling if the stop switch pulled
		  	else
		  	    doorf = 2;      // open the doors if the stop switch pulled
    	}

	  	if((rccmsk[cons[carnmb]][position] != 0) || (rucmsk[cons[carnmb]][position] != 0) || (rdcmsk[cons[carnmb]][position] != 0))
    	{
		  	if(rdinp(i_DOLR) == 1)
		  	    rdoorf = 1;      //keep the doors dwelling if the stop switch pulled
		  	else
		  	    rdoorf = 2;      // open the doors if the stop switch pulled
    	}
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
    	timers[tnudge] = 0;
    	timers[tdc] = 0;
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
    	timers[trnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
    	timers[trdc] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
  	}

	// Select the door to open
	doorsel();

	// Manual Hall door Automatic gate door 

	if (manual_door == 2)
	{		  // Special case for manual hoistway door with automatic gate
		if ((rdinp(i_DZ) == 1) && (fdoor_en == 1) && ((man_dcc() == 0) || ((dpref == 0) && (fvars[fvclgate] == 0))))
		{		   // use dpr for the door close switch on the door
		  	if(ccmsk[cons[carnmb]][position] != 0)
			{
				if (((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1))) && (man_dcc() == 1) && 
					((rd_both_inps(i_DOB,i_XDOB) == 0) && ((cons[RGB_CC] == 0) || (rdinp(i_FiDOB) == 0))) ) 
					doorf = 3;		// close door
			  	else if ((rdoor_req == 0) || (no_simul_do == 0) || (man_dcc() == 0))
				{
				  	if (rdinp(i_DOL) == 1)
					{
				  	    doorf = 1;      //open the door
			    		timers[tdwel] = 0;
				  	}
					else
					{
				  	    doorf = 2;      // keep the door dwelling 
			    		timers[tdwel] = 0;
	 				}
				}
// 				else 	 allow the dwell timer to expire and close the door

		    	timers[tdo] = 0;
		    	timers[tnudge] = 0;
				timers[teeto] = 0;
		    	timers[tdc] = 0;
		  	}
			if ((doort == 0) || (dpref == 0))
				doort = fvars[fvccdt];
		}	
		else
		{
			ck_swing_door();
		}
	}
	else 
	{
		if (((timers[tnudge] >= fvars[fvndt]) || (ee_to == 1))  && ((onward_calls != 0) || (fvars[fvnudowc] == 1)) && (Door_Lock_GS_DPM_Fault == 0))
  		{		  // nudgeing door
      		if((rdinp(i_DCL) == 1) && (ccmsk[cons[carnmb]][position] != 0) &&
      			(((rd_both_inps(i_DOB,i_XDOB) == 0) && ((cons[RGB_CC] == 0) || (rdinp(i_FiDOB) == 0)))
      					|| (fvars[fvdobndg] == 0))
      			&& doorf != 1)
				doorf = 4;
		}
  	}

	if (manual_rdoor == 2)
	{		  // Special case for manual hoistway door with automatic gate
		if ((rdinp(i_DZ) == 1) && (rdoor_en == 1) && ((man_dcc() == 0) || ((dpref == 0) && (fvars[fvclgate] == 0))))
		{		   // use dpr for the door close switch on the door
		  	if(rccmsk[cons[carnmb]][position] != 0)
			{
				if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1))) && 
					(man_dcc() == 1) && (rd_both_inps(i_DOBR,i_XDOBR) == 0) && ((cons[RGB_CC] == 0) || (rdinp(i_FiDOBR) == 0)))
			  		rdoorf = 3;	   // close door
			  	else if ((fdoor_req == 0) || (no_simul_do == 0) || (man_dcc() == 0))
				{
				  	if(rdinp(i_DOLR) == 1)
					{
				  	    rdoorf = 1;      //open the door 
			    		timers[trdwel] = 0;
				  	}
				 	else
				 	{
				  	    rdoorf = 2;      // keep the door dwelling 
			    		timers[trdwel] = 0;
			    	}
				}
 //				else 	 allow the dwell timer to expire and close the door
		    	timers[trdo] = 0;
		    	timers[trnudge] = 0;
				timers[treeto] = 0;
		    	timers[trdc] = 0;
			}
			if ((rdoort == 0) || (dpref == 0))
				rdoort = fvars[fvccdt];
		}		
		else
		{
			ck_swing_door();
		}
	}
	else 
	{  // note that the nudging timer starts timing as soon as the door opens and is reset when the door is fully closed.
		if (((timers[trnudge] >= fvars[fvndt]) || (ree_to == 1)) && ((onward_calls != 0) || (fvars[fvnudowc] == 1)) && (Door_Lock_GS_DPM_Fault == 0))
  		{		  // nudgeing door
	      	if ((rdinp(i_DCLR) == 1) && (rccmsk[cons[carnmb]][position] != 0) &&
    	  		(((rd_both_inps(i_DOBR,i_XDOBR) == 0) && ((cons[RGB_CC] == 0) || (rdinp(i_FiDOBR) == 0))) || (fvars[fvdobndg] == 0))
    	  		&& rdoorf != 1)
				rdoorf = 4;
		}
  	}

	// Attendant buzzer cycle control
	if (((cons[Car_Sw] & 1) != 0) || (cons[att] != 0))
	{
		if ((att_buz == 1) && (firef == 0) && (rfiref == 0))
		{
			if(fvars[fvattbzoff] == 0)
				att_buz_on = 1;
			else
			{
				if(att_buz_on == 1)
				{
					if(timers[tattbzcyc] > fvars[fvattbzon])
					{
						att_buz_on = 0;	
						timers[tattbzcyc] = 0;
					}
				}
				else
				{
					if(timers[tattbzcyc] > fvars[fvattbzoff])
					{
						att_buz_on = 1;	
						timers[tattbzcyc] = 0;
					}
				}
			}
		}
		else
		{
			if ((firef != 0) || (rfiref != 0))
				att_buz_on = 0;
			else
				att_buz_on = 1;	
			timers[tattbzcyc] = 0;
		}
	}

	if (((cons[Car_Sw] & 1) != 0) && (cons[att] == 0))
	{	 // No car or hall calls
		clrcc(0);
		att_buz = 0;
		if (timers[tattbzini] > fvars[fvhcdt])
		{
			if (((c_up_hall_call[0] & ~fl_mask[position]) != 0) || 
				((c_dn_hall_call[0] & ~fl_mask[position]) != 0) ||
				((cons[rear] != 0) && (
					((c_upr_hall_call[0] & ~fl_mask[position]) != 0) ||
					((c_dnr_hall_call[0] & ~fl_mask[position]) != 0))))
			{
				if ((timers[tattbz] > fvars[fvattbz]) && (fvars[fvattbz] != 0) && (timers[tattbz] < (fvars[fvattbz] + 3000)))
				{
					att_buz = 1;
				}
			}
			else
				timers[tattbz] = 0;
		}
		else
			timers[tattbz] = 0;
	}

	if ((manual_door == 1) || (cons[frghtd] <= 5))
	{
		if (((door_locks() == 0) || (car_gate() == 0)) && (dcalls != 0))
		{
			if ((timers[tdoorbz] > fvars[fvdoorbz]) && (fvars[fvdoorbz] != 0))
				door_buz = 1;
		}
		else
		{
			door_buz = 0;
			timers[tdoorbz] = 0;
		}
	}

	if(cons[nmbcars] > 1)
	{
		if((door_buz == 1))
		{
			if ((timers[ttoutmandoor] > 100) && (timeout_mandoor == 0))
			{
				timeout_mandoor = 1;
			}
			timers[ttoutmandoordbnc] = 0;
		}
		else
		{
			if(timers[ttoutmandoordbnc] > 10)
				timeout_mandoor = 0;
			timers[ttoutmandoor] = 0;
		}

	}

	// HEOF used for security recall in conjunction with EOR

	if (((fvars[fvheloff2] & 0x01) != 0) && (latch_HEOF == 1) && ((cons[Elev_off] & 2) != 0) &&((cons[Elev_off] & 4) != 0))
	{
    	clrcc(0);
		rear_slowdown &= ~cc_do;
		front_slowdown &= ~cc_do;

    	clrhc();
    	clrlant();
		dirsel();
		
		if(position != fvars[fvheoffl])
		{
		  	if(rdinp(i_DCL) == 1)
			{
		   	    doorf = 3;      //keep the doors CLOSED 
			}
			if ((cons[rear] != 0) && (rdinp(i_DCLR) == 1))
				rdoorf = 3; 	// keep rear door closed
		}
		else
		{
			if(f_door_open_device == 1)
				allow_do = 1;

		}
	}

	// CEOF used for security recall in conjunction with EOR

	if(((fvars[fvceloff2] & 0x01) != 0) && (latch_CEOF == 1) && ((cons[Car_Elev_off] & 1) != 0) &&((cons[Car_Elev_off] & 2) != 0))
	{
    	clrcc(0);
		rear_slowdown &= ~cc_do;
		front_slowdown &= ~cc_do;

    	clrhc();
    	clrlant();
		dirsel();
		
		if(position != fvars[fvheoffl])
		{
		  	if(rdinp(i_DCL) == 1)
			{
		   	    doorf = 3;      //keep the doors CLOSED 
			}
			if ((cons[rear] != 0) && (rdinp(i_DCLR) == 1))
				rdoorf = 3; 	// keep rear door closed
		}
		else
		{
			if(f_door_open_device == 1)
				allow_do = 1;

		}
	}


	if(latch_JAIL == 1)
	{
    	clrcc(0);
		rear_slowdown &= ~cc_do;
		front_slowdown &= ~cc_do;
    	clrhc();
    	clrlant();
		dirsel();

		if(position != fvars[fvheoffl])
		{
		  	if(rdinp(i_DCL) == 1)
			{
		   	    doorf = 3;      //keep the doors CLOSED 
			}
			if ((cons[rear] != 0) && (rdinp(i_DCLR) == 1))
				rdoorf = 3; 	// keep rear door closed
		}
		else
		{
			if(f_door_open_device == 1)
				allow_do = 1;

		}
	}


	//IF switching SECF tables we need reset door open button security

	if(prev_Sec_Fl_Mask != Sec_Fl_Mask)
	{
		prev_Sec_Fl_Mask = Sec_Fl_Mask;
		sec_do_en = 0;
		sec_rdo_en = 0;		
	}



	// normal door operation

	// Open the door from a car call input
  	
  	if (ccmsk[cons[carnmb]][position] != 0)
  	{
		fl_ix = (position-1)/32;

		if (((Lockout_Override == 1) && ((rdinp(i_CCS(position)) == 1) || (rdinp(i_SECUR) == 1)) && (Car_Call_Lockouts == 0)) ||
		     ((Grp_CC_Override == 1) && ((gc_cc_sec_ovr_set[fl_ix] & fl_mask[position]) != 0)))
			sec_override = 1;		// ccs input used for override or lockout but not both
		else
			sec_override = 0;

		if ((((Car_Call_Lockouts == 0) || (rdinp(i_CCS(position)) == 0) || (rdinp(i_SECUR) == 0)) &&
			 ((cons[hugs] == 0) || ((gc_hugsec == 0) || ((gc_hugs_sec[fl_ix] & fl_mask[position]) == 0) )) &&
			 ((Grp_CC_Lockouts == 0) || (gc_sec == 0) || ((gc_cc_sec[fl_ix] & fl_mask[position]) == 0)) &&
			 ((cc_sec_mask[fl_ix] & fl_mask[position]) == 0) &&
			 (((cons[otslbp] & 0x04) == 0) || (rdinp(i_RTLDC) == 0)) &&
			 (((ids_cc_sec[fl_ix][cons[carnmb]] & fl_mask[position]) == 0) || (cons[ids] == 0))) || 
			 (position == fvars[fvsecfl]) || (sec_override == 1) || (firef != 0))
		{
			sec_do_en = 1;
		}
		else
		{
			fcc_req = 0;
			sec_do_en = 0;
		}

		if (((sec_do_en == 1) || (doorf != 0) || ((front_slowdown & cc_do) != 0)) && (fdoor_en == 1))
		{
			if ((remote_cc == 0) || ((fvars[fvrmcc] & 0x01) == 0) || (cons[Remote_CC] == 0)) 
				enable_cc = 1;
			else
				enable_cc = 0;

		  	if( ((rdinp(i_CCPBS) == 1) || (c_tmr_svc[s_cc_pb_sec] == 1)) && (cons[cc_pb_sec] == 1) && (Security_code_flag != 0))
				dis_cc_reopen = 1;			 // prevent the door from reopening when code entered
			else
				dis_cc_reopen = 0;

		  	if ( ((front_slowdown & cc_do) != 0) ||	(fcc_req == 1)	||
		  		 ((enable_cc == 1) && (dis_cc_reopen == 0) && (rd_both_inps(i_CC(position),i_XCC(position))==1)) || 
		  		 ((remote_cc == 1) && (dis_cc_reopen == 0) && (rdinp(i_RM_CC(position)) == 1)) )
			{
				fcc_req = 0;
				front_slowdown &= ~cc_do;
			  	if ((doorf != 2) && (doorf != 4) && (((timers[tnudge] < fvars[fvndt]) && (ee_to == 0)) || ((onward_calls == 0) && (fvars[fvnudowc] == 0)) ) )
			  	{
					if (((cons[Elev_off] & 4) != 0)&&((latch_HEOF == 1)||((rdinp(i_HEOF) == 1)))&&(fvars[fvheloff2] & 0x02) != 0)
					{
						if(allow_do == 1)
						   	doorf = 1;
					}
					else
					{
					   	if ((/*((cons[Remote_CC] != 4) || (rdinp(i_RM_DCOP) == 0)) ||*/(rdinp(i_HEOF) == 0)) && 
							((cons[Car_Elev_off] == 0) || (rdinp(i_CEOF) == 0) || ((fvars[fvceloff] & 0x08) == 0)) &&
							(((cons[Elev_off] & 0x02) == 0) || (rdinp(i_HEOF) == 0) || ((fvars[fvheloff] & 0x08) == 0)))
					   		doorf = 1;
					}
			    	timers[tdo] = 0;
					dtmrf |= cc_dt;
			    	if ((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))
			    		doort = fvars[fvfrtdcto];
					else
				    	doort = fvars[fvccdt];
 					fsdo = 1;
			  	}  
			}
		}
	}
	else
	{
		if((rdinp(i_SECFM) == 1) && (cons[SecFlCfg] == 1))
		{
			sec_do_en = 0;
			fdoor_en = 0;
			fdoor_req = 0;
		}
		fcc_req = 0;
	  	if((ucmsk[cons[carnmb]][position] == 0) && (dcmsk[cons[carnmb]][position] == 0))
		{
			fdoor_en = 0;
			fdoor_req = 0;
		}
	}
	
  	if ((cons[rear] != 0) && (rccmsk[cons[carnmb]][position] != 0))
  	{
		fl_ix = (position-1)/32;

		if (((Lockout_Override == 1) && ((rdinp(i_RCCS(position)) == 1) || (rdinp(i_RSECU) == 1)) && (Car_Call_Lockouts == 0)) ||
		      ((Grp_CC_Override == 1) && ((gc_rcc_sec_ovr_set[fl_ix] & fl_mask[position]) != 0)) )
			sec_override = 1;		// rccs input used for override or lockout but not both
		else
			sec_override = 0;

		if ((((Car_Call_Lockouts == 0) || (rdinp(i_RCCS(position)) == 0) || (rdinp(i_RSECU) == 0)) &&
			 ((rcc_sec_mask[fl_ix] & fl_mask[position]) == 0) &&
			 ((cons[hugs] != 2) || (gc_rhugsec == 0) || ((gc_rhugs_sec[fl_ix] & fl_mask[position]) == 0)) &&
			 ((Grp_CC_Lockouts == 0) || (gc_rsec == 0) || ((gc_rcc_sec[fl_ix] & fl_mask[position]) == 0)) &&
			 (((cons[otslbp] & 0x04) == 0) || (rdinp(i_RTLDC) == 0)) &&
			 (((ids_rcc_sec[fl_ix][cons[carnmb]] & fl_mask[position]) == 0) || (cons[ids] == 0))) || 
			 (position == fvars[fvsecfl]) || (sec_override == 1) || (rfiref != 0))
		{
			sec_rdo_en = 1;
		}
		else
		{
			rcc_req = 0;
			sec_rdo_en = 0;
		}

		if (((sec_rdo_en == 1) || (rdoorf != 0) || ((rear_slowdown & cc_do) != 0)) && (rdoor_en == 1))
		{
			if ((remote_rcc == 0) || ((fvars[fvrmcc] & 0x01) == 0) || (cons[Remote_CC] == 0))
				enable_cc = 1;
			else
				enable_cc = 0;

		  	if( ((rdinp(i_CCPBS) == 1) || (c_tmr_svc[s_cc_pb_sec] == 1)) && (cons[cc_pb_sec] == 1) && (Security_code_flag != 0))
				dis_cc_reopen = 1;			 // prevent the door from reopening when code entered
			else
				dis_cc_reopen = 0;

			if (((rear_slowdown & cc_do) != 0) || (rcc_req == 1) ||
				((enable_cc == 1) && (dis_cc_reopen == 0) && (rd_both_inps(i_CCR(position),i_XCCR(position))==1)) ||
				((remote_rcc == 1) && (dis_cc_reopen == 0) && (rdinp(i_RM_CCR(position))==1)) )
			{
				rcc_req = 0;
				rear_slowdown &= ~cc_do;
				if ((rdoorf != 2) && (rdoorf != 4) && (((timers[trnudge] < fvars[fvndt]) && (ree_to == 0)) || ((onward_calls == 0) && (fvars[fvnudowc] == 0)))	)
				{
					if ((/*((cons[Remote_CC] != 4) || (rdinp(i_RM_DCOP) == 0)) ||*/ (rdinp(i_HEOF) == 0)) && 
						((cons[Car_Elev_off] == 0) || (rdinp(i_CEOF) == 0) || ((fvars[fvceloff] & 0x08) == 0)) &&
						(((cons[Elev_off] & 0x02) == 0) || (rdinp(i_HEOF) == 0) || ((fvars[fvheloff] & 0x08) == 0)))			   	
				    	rdoorf = 1;
				    timers[trdo] = 0;
					dtmrf |= cc_dt;
				   	if ((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12))
			    		rdoort = fvars[fvfrtdcto];
					else
						rdoort = fvars[fvccdt];
					rsdo = 1;
				}
			}
		}
  	}
	else
	{
		if((rdinp(i_SECFM) == 1) && (cons[SecFlCfg] == 1))
		{
			sec_rdo_en = 0;
			rdoor_en = 0;
			rdoor_req = 0;
		}
		rcc_req = 0;
	  	if((rucmsk[cons[carnmb]][position] == 0) && (rdcmsk[cons[carnmb]][position] == 0))
		{
			rdoor_en = 0;
			rdoor_req = 0;
		}
	}


	// Open the door from a hall call input

  	if (((downcb[position] == 1) || ((front_slowdown & dc_do) != 0)) && (dpref != 1) && (fdoor_en == 1))
  	{
		front_slowdown &= ~dc_do;
  		if ((doorf != 2) && (doorf != 4) && 
  			((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0)) &&
  		 	(((nudgst & 0x02) == 0) || ((onward_calls == 0) && (fvars[fvnudowc] == 0))) )
	  	{
	    	doorf = 1;
	    	timers[tdo] = 0;
			dtmrf |= hc_dt;
	    	if ((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))
				doort = fvars[fvfrtdcto];
		    else
				doort = fvars[fvhcdt];
			run_no_pass_cnt = 0;
			fsdo = 1;
	  	}
	}
  	if (((rdowncb[position] == 1) || ((rear_slowdown & dc_do) != 0)) && (dpref != 1) && (rdoor_en == 1))
	{
		rear_slowdown &= ~dc_do;
		if ((rdoorf != 2) && (rdoorf != 4) && 
  			((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0)) &&
			(((rnudgst & 0x02) == 0) || ((onward_calls == 0) && (fvars[fvnudowc] == 0)))	)
	  	{
	    	rdoorf = 1;
	    	timers[trdo] = 0;
			dtmrf |= hc_dt;
	    	if ((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12))
				rdoort = fvars[fvfrtdcto];
			else
				rdoort = fvars[fvhcdt];

			run_no_pass_cnt = 0;
			rsdo = 1;
	  	}
	}
  	if (((upcb[position] == 1) || ((front_slowdown & uc_do) != 0)) && (dpref != 2) && (fdoor_en == 1))
  	{ 
		front_slowdown &= ~uc_do;
  		if ((ncu_door_seq != 3) && (doorf != 2) && (doorf != 4) && 
  			((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0)) &&
  			(((nudgst & 0x01) == 0) || ((onward_calls == 0) && (fvars[fvnudowc] == 0))) )
	  	{
	    	doorf = 1;
	    	timers[tdo] = 0;
			dtmrf |= hc_dt;
	    	if ((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))
				doort = fvars[fvfrtdcto];
			else
				doort = fvars[fvhcdt];

			run_no_pass_cnt = 0;
			fsdo = 1;
	  	}
	}
  	if (((rupcb[position] == 1) || ((rear_slowdown & uc_do) != 0)) && (dpref != 2) && (rdoor_en == 1))
  	{
		rear_slowdown &= ~uc_do;
  		if ((rdoorf != 2) && (rdoorf != 4) && 
  			((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0)) &&
  			(((rnudgst & 0x01) == 0) || ((onward_calls == 0) && (fvars[fvnudowc] == 0)))	)
	  	{
	    	rdoorf = 1;
	    	timers[trdo] = 0;
			dtmrf |= hc_dt;
	    	if ((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12))
				rdoort = fvars[fvfrtdcto];
			else
				rdoort = fvars[fvhcdt];
			run_no_pass_cnt = 0;
			rsdo = 1;
	  	}
	}

  	if ((ccmsk[cons[carnmb]][position] != 0) || (ucmsk[cons[carnmb]][position] != 0) || (dcmsk[cons[carnmb]][position] != 0))
  	{
		if ((fvars[fvdobsec] == 1) || (fvars[fvdobsec] == 3) || (sec_do_en == 1) || (doorf != 0))
		{
			if ((fdoor_en == 1) &&
				((rd_both_inps(i_DOB,i_XDOB) == 1) || (fdob_req == 1) ||
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) ||
				 ((remote_cc == 1) && ((rdinp(i_RM_DOB) == 1) || ((tugf == 1)&&(rdinp(i_TGDO) == 1)))) ||
				 (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDOB) == 1))))
			{
				fdob_req = 0;
				if ((fvars[fvdobndg] == 1) || ((doorf != 2) && (doorf != 4) && 
					(((timers[tnudge] < fvars[fvndt]) && (ee_to == 0)) || ((onward_calls == 0) && (fvars[fvnudowc] == 0)))))
				{
				    timers[tdo] = 0;
					if ((timers[tnudge] > fvars[fvndt]) || (ee_to == 1))
						doort = 0;
					else if ((doorf != 1) && (doorf != 2))		// not openning or dwelling
					{
						if ((fvars[fvshortdt] != 0) && (short_doort_en == 1))
				    		doort = fvars[fvshortdt];
						else if ((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))
			    			doort = fvars[fvfrtdcto];
						else
							doort = fvars[fvccdt];
					}
				    doorf = 1;
					fsdo = 1;
				}
			}
		}
		else
			fdob_req = 0;
		
		if ( ((rdinp(i_SE) == 1) || (rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
		 	  							((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && // EE2 input ON
		 														(fvars[fveeshortdt] == 0) && (do_seq != 0))
		{
			if (timers[tnudge] > fvars[fvndt])
				doort = 0;
			else if (((dtmrf == 0) || (dtmrf == cc_dt) || (dtmrf == hc_dt)) && (short_doort_en == 1))
		    	doort = fvars[fvshortdt];

		}
  	}
	else if(ccnmsk[cons[carnmb]][position] != 0)
  	{
		if (((fvars[fvdobsec] == 2) || ((fvars[fvdobsec] == 5) && (ind() == 1))) ||
																 (sec_do_en == 1) || (doorf != 0))
		{
			if ((fdoor_en == 1) &&
				((rd_both_inps(i_DOB,i_XDOB) == 1) || (fdob_req == 1) ||
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) ||
				 ((remote_cc == 1) && ((rdinp(i_RM_DOB) == 1) || ((tugf == 1)&&(rdinp(i_TGDO) == 1)))) ||
				 (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDOB) == 1))))
			{
				fdob_req = 0;
				if ((fvars[fvdobndg] == 1) || ((doorf != 2) && (doorf != 4) && 
					(((timers[tnudge] < fvars[fvndt]) && (ee_to == 0)) || ((onward_calls == 0) && (fvars[fvnudowc] == 0)))))
				{
				    timers[tdo] = 0;
					if ((timers[tnudge] > fvars[fvndt]) || (ee_to == 1))
						doort = 0;
					else if ((doorf != 1) && (doorf != 2)) 
					{
						if ((fvars[fvshortdt] != 0) && (short_doort_en == 1))
				    		doort = fvars[fvshortdt];
						else if ((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))
			    			doort = fvars[fvfrtdcto];
						else
							doort = fvars[fvccdt];
					}
				    doorf = 1;
					fsdo = 1;
				}
			}
		}
		else
			fdob_req = 0;
		
		if (((rdinp(i_SE) == 1) || (rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
		 	 							 ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && // EE2 input ON
		 																(fvars[fveeshortdt] == 0) && (do_seq != 0))
		{
			if (timers[tnudge] > fvars[fvndt])
				doort = 0;
			else if (((dtmrf == 0) || (dtmrf == cc_dt) || (dtmrf == hc_dt)) && (short_doort_en == 1))
		    	doort = fvars[fvshortdt];

		}
  	}
	else
		fdob_req = 0;

  	if ((cons[rear] != 0) && ((rccmsk[cons[carnmb]][position] != 0) || (rucmsk[cons[carnmb]][position] != 0) || (rdcmsk[cons[carnmb]][position] != 0)))
  	{
		if ((fvars[fvdobsec] == 1) || (fvars[fvdobsec] == 4) || (sec_rdo_en == 1) || (rdoorf != 0))
		{
			if  ((rdoor_en == 1) && 
				 ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || (rdob_req == 1) ||
				  ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
				  ((remote_rcc == 1) && (rdinp(i_RM_DOBR) == 1)) ||
				  ((tugf == 1)&&(rdinp(i_TGDOR) == 1)) || 
				  (((cons[frghtd] == 11) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDOBR) == 1))))
			{
				rdob_req = 0;
				if ((fvars[fvdobndg] == 1) || ((rdoorf != 2) && (rdoorf != 4) && 
					(((timers[trnudge] < fvars[fvndt]) && (ree_to == 0)) || ((onward_calls == 0) && (fvars[fvnudowc] == 0)))))
				{
				    timers[trdo] = 0;
					if ((timers[trnudge] > fvars[fvndt]) || (ree_to == 1))
						rdoort = 0;
					else if ((rdoorf != 1) && (rdoorf != 2)) 
					{
						if ((fvars[fvshortdt] != 0) && (short_rdoort_en == 1))
					    	rdoort = fvars[fvshortdt];
						else if ((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12))
			    			rdoort = fvars[fvfrtdcto];
						else
							rdoort = fvars[fvccdt];
					}
				    rdoorf = 1;
					rsdo = 1;
				}
			}
		}
		else
			rdob_req = 0;
		
		if (((rdinp(i_SER) == 1) || (rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 	  							((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) &&  				// EER2 input ON
																(fvars[fveeshortdt] == 0) && (rdo_seq != 0))
		{
			if (timers[trnudge] > fvars[fvndt])
				rdoort = 0;
			else if (((dtmrf == 0) || (dtmrf == cc_dt) || (dtmrf == hc_dt)) && (short_rdoort_en == 1))
		    	rdoort = fvars[fvshortdt];

		}
	}
	else if ((cons[rear] != 0) && (rccnmsk[cons[carnmb]][position] != 0))
  	{
		if ((fvars[fvdobsec] == 2) || ((fvars[fvdobsec] == 5) && (ind() == 1)) || (sec_rdo_en == 1) || (rdoorf != 0))
		{
			if  ((rdoor_en == 1) && 
				 ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || (rdob_req == 1) ||
				  ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
				  ((remote_rcc == 1) && (rdinp(i_RM_DOBR) == 1)) ||
				  ((tugf == 1) && (rdinp(i_TGDOR) == 1)) || 
				  (((cons[frghtd] == 11) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDOBR) == 1))))
			{
				rdob_req = 0;
				if ((fvars[fvdobndg] == 1) || ((rdoorf != 2) && (rdoorf != 4) && 
					(((timers[trnudge] < fvars[fvndt]) && (ree_to == 0)) || ((onward_calls == 0) && (fvars[fvnudowc] == 0)))))
				{
				    timers[trdo] = 0;
					if ((timers[trnudge] > fvars[fvndt]) || (ree_to == 1))
						rdoort = 0;
					else if ((rdoorf != 1) && (rdoorf != 2)) 
					{
						if ((fvars[fvshortdt] != 0) && (short_rdoort_en == 1))
					    	rdoort = fvars[fvshortdt];
						else if ((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12))
			    			rdoort = fvars[fvfrtdcto];
						else
							rdoort = fvars[fvccdt];
					}
				    rdoorf = 1;
					rsdo = 1;
				}
			}
		}
		else
			rdob_req = 0;
		
		if (((rdinp(i_SER) == 1) || (rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 	  							((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) 			// EER2 input ON
												&& (fvars[fveeshortdt] == 0) && (rdo_seq != 0))
		{
			if (timers[trnudge] > fvars[fvndt])
				rdoort = 0;
			else if (((dtmrf == 0) || (dtmrf == cc_dt) || (dtmrf == hc_dt)) && (short_rdoort_en == 1))
		    	rdoort = fvars[fvshortdt];

		}
	}
	else
		rdob_req = 0;

	if ((car_Grp_Stat.c_next_up == 0) && (in_att == 0))
	{
 		if (cons[frghtd] == 0)
 		{
  			if (position == lobby_fl)  // at the lobby floor
			{
    			doort = fvars[fvldt];
				if (cons[rear] == 1)
				{
					rdoort = fvars[fvldt];
				}
			}
			
		}
  	}

  	if ((rd_both_inps(i_DOB,i_XDOB) == 1) ||
		((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) ||
		(rdinp(i_SE) == 1) || (rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
		((eef2_enabled == 1) && (rdinp(i_EE2) == 1)))	// EE2 input ON
			pass_det = 1;
	if ((cons[rear] != 0) && ((rd_both_inps(i_DOBR,i_XDOBR) == 1) ||
		((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
		 (rdinp(i_SER) == 1) || (rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		  ((eer2_enabled == 1) && (rdinp(i_EER2) == 1))))		// EER2 input ON
			pass_det = 1;
		
				
  	if(doorf == 0)
  	{
    	timers[tdo] = 0;
    	timers[tdwel] = 0;
//    	timers[tnudge] = 0;
    	timers[tdc] = 0;
    	timers[tlant] = 0;
    	timers[trlant] = 0;
  	}
  	if( (cons[rear] == 0) || (rdoorf == 0) )
  	{
    	timers[trdo] = 0;
    	timers[trdwel] = 0;
//    	timers[trnudge] = 0;
    	timers[trdc] = 0;
  	}
  	carcancel();
  	hallcancel();

// attendant service door operation

  	if(((cons[att] != 0) && (rdinp(i_ATT) == 1)) && 
  	 (((fvars[fvheofovrind] & 0x02) == 0) || (heof_over_att == 0)))
	{
		servf = s_ATT;		// attendant service
		
		if ((fvars[fvheofovrind] & 0x02) != 0)
		{
			if (((cons[Elev_off] & 4) != 0) && (rdinp(i_HEOF) == 1))
			{
				if (timers[theofovratt] > 10)
					heof_over_att = 1;
			}
			else
			{
				timers[theofovratt] = 0;
			}
		}
		
		if (in_att == 0)
		{			  // first time on attendant
			if (((fvars[fvno_simul_do] == 2) && (rccmsk[cons[carnmb]][position] != 0)) || (ccmsk[cons[carnmb]][position] == 0))
			{
				if ((doorf == 0) && (rccmsk[cons[carnmb]][position] != 0))
					rdoorf = 1;		// open rear door 
			}
			else
			{
				if ((rdoorf == 0) && (ccmsk[cons[carnmb]][position] != 0))
					doorf = 1;		// open front door
			}
		}

		in_att = 1;
		att_buz = 0;
		

		if(car_Grp_Stat.c_any_hall_call == 0)
		{	
			if(any_hall_call_dbnc > 0)
				any_hall_call_dbnc--;	
		}
		else
			any_hall_call_dbnc = 200;
		
		
		if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
		{
			timers[tdwel] = 0;
			timers[trdwel] = 0;
		    clrcc(0);
			dpref = 0;
		}

		if (rdinp(i_ATTBY) == 1)
		{
			timers[tdwel] = 0;
			timers[trdwel] = 0;
		}

    	carcall();

    	if((ccmsk[cons[carnmb]][position] != 0) && ((doorf == 2) || (cons[mand] == 1)))
    	{
			if (((rd_both_inps(i_DCB,i_XDCB) == 1) ||
				 ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) || 
				 ((rdinp(i_ATTUP) == 1) && ((dcalls & 0x1) == 1)) || 
				 ((rdinp(i_ATTDN) == 1) && ((dcalls & 0x2) == 2)) )  &&
				 	((manual_door == 0) || (cons[frghtd] != 0) || ((manual_door == 2) && (man_dcc() == 1))) )
			{
				timers[tattbz] = 0;
			    doorf = 3;
				dirsel();
			}
			else if (timers[tattbzini] > fvars[fvhcdt])
			{
				dirsel();
				if ((any_hall_call_dbnc != 0) || ((dcalls != 0) && (fvars[fvattbzctrl] != 0)))
				{
					if ((timers[tattbz] > fvars[fvattbz]) && (fvars[fvattbz] != 0) && (timers[tattbz] < (fvars[fvattbz] + 3000)))
					{
						att_buz = 1;
					}
				}
				else
					timers[tattbz] = 0;
			}
			else
				timers[tattbz] = 0;

	    	timers[tdo] = 0;
	    	timers[tdc] = 0;

		  	if (((onward_hc == 0) || ((timers[tdwel] > (fvars[fvatt_eta_pref] * 20)) && (fvars[fvatt_eta_pref] != 0))) &&	
		  		((rd_both_inps(i_DOB,i_XDOB) == 1) ||
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) ||
				 (rdinp(i_SE) == 1) || (rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
				  ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))))	// EE2 input ON
				timers[tdwel] = 0;

			if (((car_Grp_Stat.c_ir_car != cons[carnmb]) && (car_Grp_Stat.c_any_hall_call == 0)) || ((car_Grp_Stat.c_ir_car == cons[carnmb]) && (car_Grp_Stat.c_any_ir_hall_call == 0)))
				timers[tdwel] = 0;

			if (timers[tdwel] < fvars[fvhcdt])
				doort = fvars[fvhcdt];
			else
				doort = (timers[tdwel] << 1) - fvars[fvhcdt];			// eta will increase while door is dwelling

	    	timers[tnudge] = 0;
			timers[teeto] = 0;
    	}
		else if ((cons[rear] == 0) || 
						((cons[mand] != 1) && (rdoorf != 2)))
		{
			timers[tattbz] = 0;
		}

    	if((rccmsk[cons[carnmb]][position] != 0) && ((rdoorf == 2) && (cons[rear] == 1)  || (cons[mand] == 1)))
    	{
			if (((rd_both_inps(i_DCBR,i_XDCBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) ||
				 ((rdinp(i_ATTUP) == 1) && ((dcalls & 0x1) == 1)) || 
				 ((rdinp(i_ATTDN) == 1) && ((dcalls & 0x2) == 2)) ) &&
				 	((manual_rdoor == 0) || ((manual_rdoor == 2) && (man_dcc() == 1))) )
			{
			    rdoorf = 3;
				dirsel();
			}
			else if (timers[tattbzini] > fvars[fvhcdt])
			{
				dirsel();
				if ((any_hall_call_dbnc != 0) || ((dcalls != 0) && (fvars[fvattbzctrl] != 0)))
				{
					if ((timers[tattbz] > fvars[fvattbz]) && (fvars[fvattbz] != 0) && (timers[tattbz] < (fvars[fvattbz] + 3000)))
						{
							att_buz = 1;
						}

				}
				else
					timers[tattbz] = 0;
			}
			else
				timers[tattbz] = 0;

	    	timers[trdo] = 0;
    		timers[trdc] = 0;

			if (((onward_hc == 0) || ((timers[trdwel] > (fvars[fvatt_eta_pref] * 20)) && (fvars[fvatt_eta_pref] != 0))) &&	
				(cons[rear] != 0) && ((rd_both_inps(i_DOBR,i_XDOBR) == 1) ||
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
					(rdinp(i_SER) == 1) || (rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 			((eer2_enabled == 1) && (rdinp(i_EER2) == 1))))	// EER2 input ON
				timers[trdwel] = 0;

			if (car_Grp_Stat.c_any_hall_call == 0)
				timers[trdwel] = 0;

			if (timers[trdwel] < fvars[fvhcdt])
				rdoort = fvars[fvhcdt];
			else
				rdoort = (timers[trdwel] << 1) - fvars[fvhcdt];

    		timers[trnudge] = 0;
			timers[treeto] = 0;
	   	}
		else if (doorf != 2)
		{
			if (cons[mand] != 1)
			{
				timers[tattbz] = 0;
			}
		 }

		chkdoor_state = 33;
    	return(0);
	}
	else
	{
		if (in_att == 1)
		{
			in_att = 0;
			timers[trdwel] = 0;
			timers[tdwel] = 0;			// Reset door dwell
	    	if ((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))
				doort = fvars[fvfrtdcto];
			else  
				doort = fvars[fvhcdt];
			if ((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12)) 
				rdoort = fvars[fvfrtdcto];
		   	else
				rdoort = fvars[fvhcdt];
		}
		if (cons[Car_Sw] == 0)
		{ 		   // can't have both attendant and car switch (car switch uses att buzzer)
			timers[tattbz] = 0;
			att_buz = 0;
		}
		if (servf == s_ATT)
			servf = s_AUTO;
	}

  	if ((rd_both_inps(i_ED,i_XED) == 1) ||      // extra door time input in on
  	     ((cons[rear] != 0) && (rd_both_inps(i_EDR,i_XEDR) == 1))  ||
  		 ((position == fvars[fvexdtflr]) && (rdinp(i_EDHL) == 1) && (cons[exDHwLatFL] == 1)) ||     		
 		 ((remote_cc == 1) && (cons[Remote_CC] == 2) && (rdinp(i_RM_ED) == 1)))
    {
		if ((timers[tdoorhold] < fvars[fvmaxdoorhld]) || (fvars[fvmaxdoorhld] == 0))
		{
			if ((fvars[fvmaxdoorhld] > 0) || (rdinp(i_ED) == 1))
			{
				if (doorf == 3)
				{
			    	timers[tdo] = 0;
					doorf = 1;
				}
				if (cons[rear] == 1)
				{
					if (rdoorf == 3)
					{
				    	timers[trdo] = 0;
						rdoorf = 1;
					}
				}
			}
			pass_det = 1;
			timers[tnudge] = 0;
			timers[teeto] = 0;
			if (cons[Remote_CC] == 3)	// remote lobby extended door dwell time
			{
				if ((rdinp(i_ED) == 1) || ((position == fvars[fvexdtflr]) && (rdinp(i_EDHL) == 1) && (cons[exDHwLatFL] == 1)))
				{	
					timers[tdwel] = 0;
					if (cons[rear] == 1)
						timers[trdwel] = 0;
					extended_door_time = 1;		// use handicap extended door time
				}
				
				if ((extended_door_time == 0) && (position == lobby_fl) && (remote_cc == 1) && (rdinp(i_RM_ED) == 1))
					rm_extd_doort = 1;			//	use vip door time
				else
					rm_extd_doort = 0;
			}
			else
			{
				timers[tdwel] = 0;
				if (cons[rear] == 1)
					timers[trdwel] = 0;
				extended_door_time = 1;
			}
			if (cons[rear] == 1)
			{
				timers[trnudge] = 0;
				timers[treeto] = 0;
			}
			if (cons[exDHwL] == 1)
			{
				setoutp(o_EDL);	// Turn on light
			}
		}
		else
		{
			rm_extd_doort = 0;
			extended_door_time = 0;
		}
	}
	else 
	{
		timers[tdoorhold] = 0;
		if (rm_extd_doort == 1)
			rm_extd_doort = 0;
		if (fvars[fvmaxdoorhld] > 0)
			extended_door_time = 0;
	}

	if (((extended_door_time == 1) || (rm_extd_doort == 1)) && (in_att == 0))
	{
		if (fvars[fvmaxdoorhld] > 0)
		{
	   		doort = fvars[fvhcdt];
	   		rdoort = fvars[fvhcdt];
		}
		else
		{
			if ((cons[Remote_CC] == 3) && (rm_extd_doort == 1))
			{	// use vip door time with remote ED input
				doort = fvars[fvviptim];
				rdoort = fvars[fvviptim];
			}
			else
			{	// use handicap extended door time
		   		doort = fvars[fvexdt];
		   		rdoort = fvars[fvexdt];
			}
		}
	}

	if ((extended_door_time == 1) && (fvars[fvmaxdoorhld] > 0))
	{	  // take car out of service from door hold key switch
		servf = s_EXT_DOOR;
		chkdoor_state = 34;
		return(0);
	}
	
	if (set_long_door_time == 1)
	{
	 	doort = fvars[fvexdt];
	 	rdoort = fvars[fvexdt];
	}
	
	if (cons[Sabbath] != 0)
	{
		if (sabbath_svc() == 0)
		{
			chkdoor_state = 35;
			return(0);
		}
	}
	
	if ((enable_cc_test == 1) || (continue_cc_test == 1)) 
		carcall_test();

	if(((fvars[fvstplby] & 0x08) != 0) && (cons[nmbcars] == 1))
		next_stop_lobby();	


	// Next Car Up Door open
	if ((car_Grp_Stat.c_next_up != 0) && (manual_door == 0) && (fdoor_en == 1))
	{																	  // not alt lobby or alt lobby uses next up
		if ((position == lobby_fl) && (car_Grp_Stat.c_ncu_door == 1))
		{
			if (ncu_door_seq == 0)
			{
				doorf = 1;
		   		timers[tdo] = 0;
				timers[tdwel] = 0;			// Reset door dwell
			    timers[tnudge] = 0;
				timers[teeto] = 0;
				timers[tncu] = 0;
				dpref = 1;					// Set up preference
				ncu_door_seq = 1;
    			doort = fvars[fvldt] + fvars[fvccdt];
			}
			else if (ncu_door_seq == 1)
			{
				
			  	if(rdinp(i_DOL) == 1)
			  	    doorf = 1;      // open the doors 
			  	else
			  	    doorf = 2;      //keep the doors dwelling 

				if (doorf == 2)
					ncu_door_seq = 2;
				if (timers[tdo] >= fvars[fvdpt])
					ncu_door_seq = 4;			// allow doors to close
				dpref = 1;					// Set up preference
				timers[tdwel] = 0;			// Reset door dwell
			    timers[tnudge] = 0;
				timers[teeto] = 0;
				timers[tncu] = 0;
	    		doort = fvars[fvldt] + fvars[fvccdt];
			}
			else if (ncu_door_seq == 2)
			{
//				get_onward_calls();
				if (timers[tncu] <= fvars[fvldt])
				{
				  	if(rdinp(i_DOL) == 1)
				  	    doorf = 1;      // open the doors 
				  	else
				  	    doorf = 2;      //keep the doors dwelling 

					dpref = 1;					// Set up preference
					timers[tdwel] = 0;			// Reset door dwell
				    timers[tnudge] = 0;
					timers[teeto] = 0;
					if (fvars[fvldt] > timers[tncu])
						doort = (fvars[fvldt] - timers[tncu]) + fvars[fvccdt];
#if ((Traction == 1) || (Tract_OL == 1))
					if ( (((rdinp(i_LWD) == 1) && ((cons[loadw] & 1) != 0) && (cons[ansi] < 2004)) || 
						  (((cons[loadw] & 2) != 0) && (load_dis == 1))) && (onward_calls != 0))
#else
					if ((rdinp(i_LWD) == 1) && ((cons[loadw] & 1) != 0) && (cons[ansi] < 2004) && (onward_calls != 0))
#endif
					{
						doort = fvars[fvccdt];
						ncu_door_seq = 3;
					}
				}
				else
				{
					if ((onward_calls != 0) || (downcb[position] == 1) || (rdowncb[position] == 1) || (rdoor_req == 1))
					{
						doort = fvars[fvccdt];
						if ((rdowncb[position] == 0) && (rdoor_req == 0) && (fvars[fvncupref] == 1))
						{
							if ((onward_calls == 2) || ((onward_calls == 0) && (downcb[position] == 1))) 
							{
								dpref = 2;	 // onward call in opposite direction and next up has timed out
								doort = fvars[fvhcdt];
							}
						}
						ncu_door_seq = 3;
					}
					else if (((fvars[fvncu] & 2) != 0) || (timers[tdo] >= fvars[fvdpt]))
					{
						doort = fvars[fvccdt];
						ncu_door_seq = 4;
					}
					else
					{
					  	if(rdinp(i_DOL) == 1)
					  	    doorf = 1;      // open the doors 
					  	else
						{
					  	    doorf = 2;      //keep the doors dwelling 
							timers[tdo] = 0;
						}
						dpref = 1;					// Set up preference
						timers[tdwel] = 0;			// Reset door dwell
					    timers[tnudge] = 0;
						timers[teeto] = 0;
						doort = fvars[fvccdt];
					}
				}
			}
			else if (ncu_door_seq == 3) 
			{		// allow the door to close for onward call or at floor down call
				if (dpref == 0)
					ncu_door_seq = 0;  // restart next up sequence to re-open door
				   
				doort = fvars[fvccdt];
				if ((rdowncb[position] == 0) && (rdoor_req == 0) && (fvars[fvncupref] == 1))
				{
					if ((onward_calls == 2) || ((onward_calls == 0) && (downcb[position] == 1))) 
					{
						dpref = 2;	 // onward call in opposite direction and next up has timed out
						doort = fvars[fvhcdt];
					}
				}
			}
			else // (ncu_door_seq == 4)
			{	   // Allow doors to close on next up
				doort = fvars[fvccdt];
				if (doorf == 1)
					ncu_door_seq = 0;		// reset the sequence when re-opening the door
			}
		}
		else
		{
			if (ncu_door_seq != 0)
			{
				doort = fvars[fvhcdt];
				if ((ncu_door_seq != 3) && (doorf == 1))
					ncu_door_seq = 0;
			}
		}
	}


	// Set lwd flag for up peak trigger

#if ((Traction == 1) || (Tract_OL == 1))
	if (((((cons[loadw] & 1) != 0) && (cons[ansi] < 2004) && (rdinp(i_LWD) == 1) && (lw_disable == 0)) ||
	     (((cons[loadw] & 2) != 0) && (load_dis == 1))) && (position == lobby_fl))
#else
	if (((cons[loadw] & 1) != 0) && (cons[ansi] < 2004) && (rdinp(i_LWD) == 1) && (position == lobby_fl))
#endif
		lwd_set_at_lby = 1;
	else
		lwd_set_at_lby = 0;

 	
  	if ((cons[otslbp] != 0) || ((cons[ids] & 1) != 0))
  	{
  		if ((rdinp(i_RTLDC) == 1) && ((cons[otslbp] & 0x04) != 0) && (firef == 0) && (rfiref == 0) && (codebf == 0) && 
  				(eqf == 0) && (medf == 0) && (position == lobby_fl) )
		{
	    	clrcc(0);
	    	clrhc();
	    	clrlant();
			dirsel();

		  	if(rdinp(i_DCL) == 1)
		   	    doorf = 3;      //keep the doors CLOSED 
			if ((cons[rear] != 0) && (rdinp(i_DCLR) == 1))
				rdoorf = 3; 	// keep rear door closed

		    timers[tdo] = 0;
		    timers[tdwel] = 0;
		    timers[tnudge] = 0;
		    timers[tdc] = 0;
		    timers[trdo] = 0;
		    timers[trdwel] = 0;
		    timers[trnudge] = 0;
			timers[teeto] = 0;
			timers[treeto] = 0;
		    timers[trdc] = 0;
		    timers[tlant] = 0;
		    timers[trlant] = 0;
			servf = s_LBY_RCL;
			chkdoor_state = 36;
			return(0);
		}
		else

#if ((Traction == 1) || (Tract_OL == 1))
  		if ((((rdinp(i_RTL) == 1) && (cons[otslbp] != 0)) || 
  			(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_rtl == 1)) || 
  			(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_rtl == 1))) && 
  				(firef == 0) && (rfiref == 0) && (codebf == 0) && 
  				(eqf == 0) && (medf == 0) && (position == lobby_fl) )
#else
  		if ((((rdinp(i_RTLH) == 1) && (cons[otslbp] != 0)) || 
  			(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_rtl == 1)) || 
  			(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_rtl == 1))) && 
  			(firef == 0) && (rfiref == 0) && (codebf == 0) && (eqf == 0) && (medf == 0) && (position == lobby_fl) )
#endif
	  	{
	    	clrcc(0);
	    	clrhc();
	    	clrlant();
			dirsel();
			doort = fvars[fvrtldt];
			if (cons[rear] == 1)
				rdoort = fvars[fvrtldt];
			if (((fvars[fvrtl] & 0x01) == 0) && (cons[otslbp] != 2))
			{				// keep door open (0x01 == 1 cycles the door)
		    	if ((ccmsk[cons[carnmb]][position] != 0) && (fvars[fvrtldoortyp] != 1))
		    	{
				  	if(rdinp(i_DOL) == 1)
				  	    doorf = 1;      //keep the doors dwelling 
					else
				  	    doorf = 2;      // open the doors 
		    	}
		    	else if(rccmsk[cons[carnmb]][position] != 0)
		    	{
				  	if(rdinp(i_DOLR) == 1)
				  	    rdoorf = 1;      //keep the doors dwelling 
				  	else
				  	    rdoorf = 2;      // open the doors 
		    	}
		    	else if(ccnmsk[cons[carnmb]][position] != 0)
		    	{
				  	if(rdinp(i_DOL) == 1)
				  	    doorf = 1;      //keep the doors dwelling 
					else
				  	    doorf = 2;      // open the doors 
		    	}
		    	else if(rccnmsk[cons[carnmb]][position] != 0)
		    	{
				  	if(rdinp(i_DOLR) == 1)
				  	    rdoorf = 1;      //keep the doors dwelling 
				  	else
				  	    rdoorf = 2;      // open the doors 
		    	}
		    	timers[tdo] = 0;
		    	timers[tdwel] = 0;
		    	timers[tnudge] = 0;
		    	timers[tdc] = 0;
		    	timers[trdo] = 0;
		    	timers[trdwel] = 0;
		    	timers[trnudge] = 0;
				timers[teeto] = 0;
				timers[treeto] = 0;
		    	timers[trdc] = 0;
		    	timers[tlant] = 0;
		    	timers[trlant] = 0;
			}
			if (servf != s_LBY_RCL)
			{
				if (fvars[fvrtldoortyp] == 0)
				{	// open front door only
					if (ccnmsk[cons[carnmb]][lobby_fl] != 0)
						set_carcall(cons[carnmb],lobby_fl);
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
			chkdoor_state = 37;
			return(0);
		}
		else
		{
			return_to_lobby();
		}
	}

	if(rdinp(i_ZON) == 1)
		zoneelev();
	
	
	//LOGIC FOR CEOF
	if(((cons[Car_Elev_off] & 1) != 0) && (firef == 0) && (rfiref == 0) && (codebf == 0) && (eqf == 0) && (medf == 0))
	{
		if (fvars[fvheoffl] == 0)
			elof_fl = lobby_fl;
		else
			elof_fl = fvars[fvheoffl];

		if (((rdinp(i_CEOF) == 1) || ((latch_CEOF == 1) && ((cons[Car_Elev_off] & 2) != 0)))
					&& ((position == elof_fl) || (((fvars[fvceloff] & 0x01) == 0) )))
		{		// Elevator Off
			servf = s_CAR_OFF;
		    clrcc(0);
		    clrhc();
		    clrlant();
    		dirsel();
			doort = fvars[fvrtldt];
			if((cons[Car_Elev_off] & 4) != 0)
			{
				if(((fvars[fvceloff2] & 0x02) != 0)&&(latch_CEOF == 1))
				{
					setoutp(o_CEOFL);
				}
				else if ((latch_CEOF == 1) && (timers[tsec] != 0))
				{	
					setoutp(o_CEOFL);
				}
				else
				{
					clroutp(o_CEOFL);
				}
			}
			if (rdinp(i_CEOFR) == 1)
			{
				latch_CEOF = 0;
				if((cons[Car_Elev_off] & 4) != 0)
				{
					clroutp(o_CEOFL);
				}
			}
			else if(((cons[Car_Elev_off] & 2) != 0) && (timers[tceoff] >= 10))
			{
				latch_CEOF = 1;
			}

		    
		    if ((fvars[fvceloff] & 0x02) != 0)
		    {
		      	if(ccmsk[cons[carnmb]][position] != 0)
		    	{
				  	if(rdinp(i_DOL) == 1)
				  	    doorf = 1;      //open the doors
				  	else
				  	    doorf = 2;      // keep door dwelling
		    	}
		    	else if(rccmsk[cons[carnmb]][position] != 0)
		    	{
				  	if(rdinp(i_DOLR) == 1)
				  	    rdoorf = 1;      //open the doors
				  	else
				  	    rdoorf = 2;      // keep the doors dwelling
		    	}
		    }
		    
			if (((fvars[fvceloff] & 0x01) != 0) && (servf != s_CAR_OFF))	 // recalled bit set to open door at lobby floor
				set_carcall(cons[carnmb],elof_fl);
			servf = s_CAR_OFF;		// Show out of service but answer last call			
			chkdoor_state = 38;
			return(0);
		}
		else 
		{
			elevator_off();
			if(rdinp(i_CEOF) == 1)
			{
				if (((cons[Car_Elev_off] & 2) != 0) && (timers[tceoff] >= 10))
					latch_CEOF  = 1;
			}
		}
	}
	else if (servf == s_CAR_OFF)
	{
		servf = s_AUTO;
	}
	
	//LOGIC FOR HEOF	
	if (((cons[Elev_off] & 2) != 0) || ((cons[ids] & 1) != 0) || ((cons[Galileo] & 1) != 0))
	{		// Security out of group service
	 	if ((firef == 0) && (rfiref == 0) && (codebf == 0) && (eqf == 0) && (medf == 0))
		{
			if (car_Grp_Stat.c_ids_return_fl != 0)
				elof_fl = car_Grp_Stat.c_ids_return_fl;
			else if (fvars[fvheoffl] == 0)
				elof_fl = lobby_fl;
			else
				elof_fl = fvars[fvheoffl];
			
			if ((((((rdinp(i_HEOF) == 1) && ((fvars[fvheloff3] & 0x01) == 0)) || 
				((rdinp(i_HEOF) == 1) && ((fvars[fvheloff3] & 0x01) != 0) && (safe() == 1)) ||
				((latch_HEOF == 1) && ((cons[Elev_off] & 4) != 0))) && ((cons[Elev_off] & 2) != 0)) || 
				(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_heof == 1)) ||
				(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_return_fl != 0)) ||  
				(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_heof == 1)) ) 
				&& ((position == elof_fl) || (((fvars[fvheloff] & 0x01) == 0) && (car_Grp_Stat.c_ids_return_fl == 0))))
			{
				clrcc(0);
	    		clrhc();
		    	clrlant();
				dirsel();
				doort = fvars[fvrtldt];
				if((cons[Elev_off] & 8) != 0)
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
				else if((fvars[fvheloff2] & 0x08) != 0)
				{
					if(position == elof_fl)
						setoutp(o_HEOFL);	
				}

				if (rdinp(i_HEOFR) == 1)
				{
					heof_over_ind = 0;
					heof_over_att = 0;
					allow_do = 0;
					latch_HEOF = 0;
					latch_JAIL = 0;
					if((cons[Elev_off] & 8) != 0)
					{
						clroutp(o_HEOFL);
					}
						
				}
				else if (((cons[Elev_off] & 4) != 0) && (timers[theoff] >= 10))
					latch_HEOF = 1;

				if (((fvars[fvheloff] & 0x01) != 0) && ((fvars[fvheloff2] & 0x04) != 0) && (cons[Prison] == 0))
				{	// HEOF Auto Reset After Reaching Recall Floor
					if ((rdinp(i_HEOF) == 0) && (position == elof_fl) && (rdinp(i_DL) == 1) && (rdinp(i_UL) == 1)) 
					{	// When input HEOF removed wait until the timer expires
						if (timers[theofautorst] > fvars[fvheofautorst])
						{	// Reset afterwards
							heof_over_ind = 0;
							heof_over_att = 0;
							allow_do = 0;
							latch_HEOF = 0;
							if ((cons[Elev_off] & 8) != 0)
								clroutp(o_HEOFL);
							timers[theofautorst] = 0;
						}
					}
					else
						timers[theofautorst] = 0;
				}
				else
					timers[theofautorst] = 0;
				
				if (((fvars[fvheloff] & 0x02) != 0) || 
					(car_Grp_Stat.c_ids_return_fl != 0))
				{				// keep door open the door)
			    	if(ccnmsk[cons[carnmb]][position] != 0)
			    	{
						if((car_Grp_Stat.c_ids_return_fl != 0) && (car_Grp_Stat.c_ids_allw_door == 0) && (car_Grp_Stat.c_ids_return_typ == 0))
						{
							if(rdinp(i_DCL) == 1)
							{
								doorf = 3;
							}
					  	}
					  	else if(rdinp(i_DOL) == 1)

					  	    doorf = 1;      //keep the doors dwelling 
						else
					  	    doorf = 2;      // open the doors 
			    	}
			    	else if(rccnmsk[cons[carnmb]][position] != 0)
		    		{
						if((car_Grp_Stat.c_ids_return_fl != 0) && (car_Grp_Stat.c_ids_allw_door == 0) && (car_Grp_Stat.c_ids_return_typ == 0))
						{
							if(rdinp(i_DCLR) == 1)
							{
								rdoorf = 3;
							}
					  	}
					  	else if(rdinp(i_DOLR) == 1)
					  	    rdoorf = 1;      //keep the doors dwelling 
					  	else
					  	    rdoorf = 2;      // open the doors 
			    	}
		    		timers[tdo] = 0;
		    		timers[tdwel] = 0;
			    	timers[tnudge] = 0;
			    	timers[tdc] = 0;
			    	timers[trdo] = 0;
			    	timers[trdwel] = 0;
		    		timers[trnudge] = 0;
					timers[teeto] = 0;
					timers[treeto] = 0;
			    	timers[trdc] = 0;
			    	timers[tlant] = 0;
			    	timers[trlant] = 0;
				}
				if (((fvars[fvheloff] & 0x01) != 0) && (servf != s_ELEV_OFF_HS))	 // recalled bit set to open door at lobby floor
					set_carcall(cons[carnmb],elof_fl);
				servf = s_ELEV_OFF_HS;		// Show out of service but answer last call
				chkdoor_state = 39;
				return(0);
			}
			else
			{
				elevator_off();
				if(((rdinp(i_HEOF) == 1) && ((fvars[fvheloff3] & 0x01) == 0)) ||
				   ((rdinp(i_HEOF) == 1) && ((fvars[fvheloff3] & 0x01) != 0) && (safe() == 1)))
				{
					if (((cons[Elev_off] & 4) != 0) && (timers[theoff] >= 10))
						latch_HEOF = 1;
				}
				else if (cons[Prison] == 0)
				{
					timers[theoff] = 0;
					timers[theofautorst] = 0;
				}
				
				if(((fvars[fvheloff2] & 0x08) != 0) && (rdinp(i_HEOF) == 0))
				{	  
					clroutp(o_HEOFL);
				}
				
			}
		}
		else if (servf == s_ELEV_OFF_HS)
		{
			if (latch_JAIL == 0)
			{
				servf = s_AUTO;
			}
		}
		if((cons[Elev_off] & 4) != 0)
		{
			rdinp(i_HEOFR);
		}
	}

	jail_break();

	set_auto_servf();

	auto_svc_time_out_recover();

#if ((Traction == 0) && (Tract_OL == 0))
	if(cons[hotoilSW] != 0)
		hot_oil();
#endif

	if(rdinp(i_ZON) == 1)
		zoneelev();

  	chkdoor_state = 40;
  	return(0);
}

uint8 emp_doorop (void)
{
  	if (empf == ep_wait)
	{		// Wait for instructions from the group on emergency power
	    clrcc(0);
	    clrhc();
	    clrlant();
	    timers[tnudge] = 0;
	    timers[trnudge] = 0;
	    timers[tlant] = 0;
    	timers[trlant] = 0;
	    keep_lants_on = 0;
	    keep_rlants_on = 0;
	    timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
	    timers[tdcycle] = 0;
	    timers[tdwel] = 0;
	    timers[trdwel] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
		if(ccnmsk[cons[carnmb]][position] != 0)
		{
			if((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) ||
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
				doorf = 1;    // open door if door open device 
			else if ((doorf != 1) && (doorf != 2) && (doorf != 0))
				doorf = 3;
		}
		if(rccnmsk[cons[carnmb]][position] != 0)
		{
			if((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
				((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) || 
				(rdinp(i_SER) == 1))
				rdoorf = 1;    // rear open door if door open device 
			else if ((rdoorf != 1) && (rdoorf != 2) && (rdoorf != 0))
				rdoorf = 3;
		}
		ck_swing_door();
		chkdoor_state = 4;
		return(1);
	}

  // Emergency Power dwell the doors
  	if((empf == ep_home) || (empf == ep_wait_door))
  	{
	    clrcc(0);
	    clrhc();
	    clrlant();
		dpref = 0;
	    timers[tdc] = 0;
	    timers[tdo] = 0;
	    timers[tnudge] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
	    timers[trdc] = 0;
	    timers[trdo] = 0;
	    timers[trnudge] = 0;
	    timers[tlant] = 0;
    	timers[trlant] = 0;
	    keep_lants_on = 0;
	    keep_rlants_on = 0;
	    timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
	    timers[tdcycle] = 0;
#if ((Traction == 1) || (Tract_OL == 1))
		if ((cons[Emp_Recover] == 0) && 
			((fvars[fvdenfsv] == 0) || ((firef == 0) && (rfiref == 0))) &&
			 (((fvars[fvep_selsw_rcl] & 0x01) == 0) || (rdinp(i_EPS) == 0)))
		{
		   // prevent timer from time-out during door dwell to keep doors open (Traction Cars)
	    	timers[tdwel] = 0;
	   	 	timers[trdwel] = 0;
		}
#endif
	    if(ccnmsk[cons[carnmb]][position] != 0)
		{
			if (rdinp(i_DOL) == 1)
			{
			    timers[tdwel] = 0;
				if ((doorf != 1) || (doorf != 2))
					doorf = 1;
			}
			else
				doorf = 2;
			
			// only do the front door. if rear is available, close it
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if (rdinp(i_DCLR) == 1)
				{
					rdoorf = 3;
				}
			}
	    }
		else
	    {
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if (rdinp(i_DOLR) == 1)
				{
				    timers[trdwel] = 0;
					if ((rdoorf != 1) || (rdoorf != 2))
						rdoorf = 1;
				}
				else
				    rdoorf = 2;
			}
	    }
		ck_swing_door();
		set_auto_servf();
		chkdoor_state = 5;
	    return(1);
  	}

	 // Hydro or EPRS Traction car
  	if ((empf == ep_home_close) || (empf == epc_wait_close))
	{		// At home floor and closing the door
		if ((ind() == 0) && ((cons[cblue] == 0) || (rdinp(i_EMS) == 0)) && 
			((rdinp(i_ATT) == 0) || (cons[att] == 0)) && (stallf == 0) )
		{		 // not on ind or att
		    clrcc(0);
		    clrhc();
		    clrlant();
		    timers[tdc] = 0;
		    timers[tdo] = 0;
		    timers[tnudge] = 0;
		    timers[trdc] = 0;
		    timers[trdo] = 0;
		    timers[trnudge] = 0;
			timers[teeto] = 0;
			timers[treeto] = 0;
		    timers[tlant] = 0;
	    	timers[trlant] = 0;
		    keep_lants_on = 0;
		    keep_rlants_on = 0;
		    timers[tcablnt] = 0;
		    timers[trcablnt] = 0;
		    timers[tdcycle] = 0;
			if(ccnmsk[cons[carnmb]][position] != 0)
			{
				if((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) || (doorf == 1) ||
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
				{
				    timers[tdwel] = 0;
					doorf = 1;    // open door if door open device or door opening, allow it to open all the way
				}
				else
					doorf = 3;    // close door
			}
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
					(rdinp(i_SER) == 1) || (rdoorf == 1))
				{
				    timers[trdwel] = 0;
					rdoorf = 1;    // rear open door if door open device or door opening, allow it to open all the way
				}
				else
					rdoorf = 3;    // rear close door
			}
			ck_swing_door();
			chkdoor_state = 6;
			return(1);
		}
	}

	// car selected to recall from the group
  	if (empf == ep_recall)
	{		// Being recalled or at home floor and closing the door
		if (ep_recall_ok() == 1)
		{
		    clrcc(0);
		    clrhc();
		    clrlant();
		    timers[tdc] = 0;
		    timers[tdo] = 0;
		    timers[tnudge] = 0;
		    timers[trdc] = 0;
		    timers[trdo] = 0;
		    timers[trnudge] = 0;
		    timers[tlant] = 0;
	    	timers[trlant] = 0;
		    keep_lants_on = 0;
	  		keep_rlants_on = 0;
		    timers[tcablnt] = 0;
		    timers[trcablnt] = 0;
		    timers[tdcycle] = 0;
		    timers[tdwel] = 0;
		    timers[trdwel] = 0;
			timers[teeto] = 0;
			timers[treeto] = 0;
			if(ccnmsk[cons[carnmb]][position] != 0)
			{
				if((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) || (doorf == 1) ||
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))
					doorf = 1;    // open door if door open device or door opening, allow it to open all the way
				else
					doorf = 3;    // close door
			}
			if(rccnmsk[cons[carnmb]][position] != 0)
			{
				if((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
					(rdinp(i_SER) == 1) || (rdoorf == 1))
					rdoorf = 1;    // rear open door if door open device or door opening, allow it to open all the way
				else
					rdoorf = 3;    // rear close door
			}
			ck_swing_door();
			chkdoor_state = 7;
			return(1);
		}
	}
	return(0);
}

uint8 lock_gs_dpm_check(void)
{
	if (Door_Lock_GS_DPM_Fault != 0)
	{
		return(0);
	}
	return(1);
}

uint8 gs_only_check(void)
{
	if (Door_GS_fault != 0)
	{
		return(0);
	}
	return(1);
}

uint8 lock_gs_dpm_flt_front(void)
{
	if(Door_Lock_GS_DPM_Fault != 0)
	{
		if(((Door_Lock_fault != 0) && (Door_Lock_front_fault != 0)) ||
		   ((Door_GS_fault != 0) && (Door_GS_front_fault != 0)) ||
		   ((Door_DPM_fault != 0) && (Door_DPM_front_fault != 0)) ||
		   ((Door_DPM_seq_fault != 0) && (Door_DPM_seq_front_fault != 0)))
		{
			return (1);
		}
	}
	return (0);
}

uint8 lock_gs_dpm_flt_rear(void)
{
	if(Door_Lock_GS_DPM_Fault != 0)
	{
		if(((Door_Lock_fault != 0) && (Door_Lock_rear_fault != 0)) ||
		   ((Door_GS_fault != 0) && (Door_GS_rear_fault != 0)) ||
		   ((Door_DPM_fault != 0) && (Door_DPM_rear_fault != 0)) ||
		   ((Door_DPM_seq_fault != 0) && (Door_DPM_seq_rear_fault != 0)))
		{
			return (1);
		}
	}
	return (0);
}

int16 at_floor_check (void)
{

	//*******************************************************************
	//*******************************************************************
	//**********  HYDRO Redundancy Check ********************************
	//*******************************************************************
	//*******************************************************************
	
	if (cons[SFC_2004_IO] == 1)
	{
		if ((rdinp(i_DZ) == 1) && (rdinp(i_DL) == 1) && (rdinp(i_UL) == 1) && (io_fault == 0) && (relevel == 0) && 
		    (relevel_req == 0) && (rdinp(i_MRSW) == 1) &&
			(timers[tsoftstp] > fvars[fvsst]) && (dirf == 0) && (firef == 0) && (Viscosity == 0) && (medf == 0))
		{
			if (redundancy_test == 0)
			{
				if (rdinp(i_MRSW) == 1) 
				{
					timers[tredundancy] = 0;
					redundancy_test = 1;
					in_redundancy_test = 1;
					clroutp(o_FSTP);
					clroutp(o_FST);
				}
				else
					in_redundancy_test = 0;	// someone could have pulled the stop switch
			}
			// FST test first
			if ((timers[tredundancy] > 1) && (redundancy_test == 1))
			{
				setoutp(o_FST);
				redundancy_test = 2;
				timers[tredundancy] = 0;
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 2))
			{
				if (rdinp(i_FST) == 1)
				{
					redundancy_test = 3;
					timers[tredundancy] = 0;
					clroutp(o_FST);
				}
				else if (timers[tredundancy] > 5)
				{
					redundancy_test = 0;
					clroutp(o_FST);
					timers[tredundancy] = 0;
					if (rdinp(i_MRSW) == 1)
					{
						io_fault = 1;	// FST fault - FST did not PICK as expected
						record_fault(f_FST_failed_off);
					}
					else
					{
						in_redundancy_test = 0;	// restart to verify the stop switch using FST
					}
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 3))
			{
				if (rdinp(i_FST) == 0)
				{
					redundancy_test = 4;
					timers[tredundancy] = 0;
					setoutp(o_FSTP);
				}
				else if (timers[tredundancy] > 5)
				{
					io_fault = 1;	// FST fault - FST did not drop as expected
					redundancy_test = 0;
					record_fault(f_FST_failed_on);
					clroutp(o_FST);
					timers[tredundancy] = 0;
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 4))
			{
				if (rdinp(i_FST) == 1)
				{
					redundancy_test = 5;
					timers[tredundancy] = 0;
					clroutp(o_FSTP);
				}
				else if (timers[tredundancy] > 5)
				{
					io_fault = 1;	// fstp fault - FSTP did not PICK as expected
					redundancy_test = 0;
					record_fault(f_FSTP_failed_off);
					clroutp(o_FSTP);
					timers[tredundancy] = 0;
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 5))
			{
				if (rdinp(i_FST) == 0)
				{
					redundancy_test = 6;
					timers[tredundancy] = 0;
					clroutp(o_FSTP);
				}
				else if (timers[tredundancy] > 5)
				{
					io_fault = 1;	// fstp fault - FSTP did not drop as expected
					redundancy_test = 0;
					record_fault(f_FSTP_failed_on);
					clroutp(o_FSTP);
					timers[tredundancy] = 0;
				}
			}

			// PAL Test
			if ((timers[tredundancy] > 1) && (redundancy_test == 6))
			{	
				if (rdinp(i_PALF) == 0)
				{
					setoutp(o_SU);
					setoutp(o_SD);
					redundancy_test = 7;
					timers[tredundancy] = 0;
				}
				else
				{		// else exit test
					io_fault = 1;	// PAL did not pick as expected
					redundancy_test = 0;
					in_redundancy_test = 0;	  // someone could have pulled the stop switch
					record_fault(f_PAL_failed_on);
					timers[tredundancy] = 0;
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 7))
			{
				if (rdinp(i_PALF) == 1)
				{
					redundancy_test = 8;
					timers[tredundancy] = 0;
					clroutp(o_SU);
					clroutp(o_SD);
				}
				else if (timers[tredundancy] > 5)
				{
					io_fault = 1;	// CEN from PAL did not drop as expected
					redundancy_test = 0;
					record_fault(f_PAL_failed_off);
					timers[tredundancy] = 0;
					clroutp(o_SU);
					clroutp(o_SD);
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 8))
			{
				if (rdinp(i_PALF) == 0)
				{
					redundancy_test = last_redundancy_test;
					in_redundancy_test = 0;
					timers[tredundancy] = 0;
				}
				else if (timers[tredundancy] > 15)
				{
					io_fault = 1;	// PAL did not pick as expected
					redundancy_test = 0;
					in_redundancy_test = 0;	  // someone could have pulled the stop switch
					record_fault(f_PAL_failed_on);
					timers[tredundancy] = 0;
				}
			}

/*			
			// STE STarter Enable and C contactor Fault CFLT Test
			if ((timers[tredundancy] > 1) && (redundancy_test == 9))
			{
				if ((rdinp(i_CEN) == 1) && (rdinp(i_TPH) == 1) && (rdinp(i_CFLT) == 0))
				{		// Have power to circuit and fault relay is open
					clroutp(o_STE);
					redundancy_test = 10;
					timers[tredundancy] = 0;
				}
				else
				{		// CEN or TPH not on or CFLT not off
					io_fault = 1;	// CFLT did not turn on as expected
					redundancy_test = 0;
					if ((rdinp(i_CEN) == 0) || (rdinp(i_TPH) == 0))
						record_fault(f_CEN_TPH_failed_off);
					else
						record_fault(f_CFLT_failed_on);
					timers[tredundancy] = 0;
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 10))
			{
				if (rdinp(i_CFLT) == 1)
				{
					redundancy_test = 11;
					timers[tredundancy] = 0;
				}
				else if (timers[tredundancy] > 5)
				{
					io_fault = 1;	// CFLT did not turn on as expected
					redundancy_test = 0;
					record_fault(f_CFLT_failed_off);
					timers[tredundancy] = 0;
					setoutp(o_STE);
				}
			}
			// Start Controller Enable (CEN) test
			if ((timers[tredundancy] > 1) && (redundancy_test == 11))
			{
				if (rdinp(i_CEN) == 1)
				{
					clroutp(o_CEN);
					redundancy_test = 12;
					timers[tredundancy] = 0;
				}
				else
				{		// else exit test
					io_fault = 1;	// CEN did not pick as expected
					redundancy_test = 0;
					in_redundancy_test = 0;	  // someone could have pulled the stop switch
					if (rdinp(i_CEN) == 0)
						record_fault(f_CEN_failed_off);
					timers[tredundancy] = 0;
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 12))
			{
				if (rdinp(i_CEN) == 0)
				{
					redundancy_test = 13;
					timers[tredundancy] = 0;
					setoutp(o_CEN);
				}
				else if (timers[tredundancy] > 5)
				{
					io_fault = 1;	// pfc fault - PFC did not drop as expected
					redundancy_test = 0;
					if (rdinp(i_CEN) == 1)
						record_fault(f_CEN_failed_on);
					setoutp(o_CEN);
					setoutp(o_STE);
					timers[tredundancy] = 0;
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 13))
			{
				if (rdinp(i_CEN) == 1)
				{
					redundancy_test = 14;
					timers[tredundancy] = 0;
					setoutp(o_STE);
				}
				else if (timers[tredundancy] > 5)
				{
					io_fault = 1;	// CEN did not pick as expected
					redundancy_test = 0;
					in_redundancy_test = 0;	  // someone could have pulled the stop switch
					if (rdinp(i_CEN) == 0)
						record_fault(f_CEN_failed_off);
					timers[tredundancy] = 0;
				}
			}
			if ((timers[tredundancy] > 1) && (redundancy_test == 14))
			{
				if (rdinp(i_CFLT) == 0)
				{
					redundancy_test = 15;
					in_redundancy_test = 0;
					timers[tredundancy] = 0;
				}
				else if (timers[tredundancy] > 15)
				{
					io_fault = 1;	// CFLT did not turn off as expected
					redundancy_test = 0;
					in_redundancy_test = 0;	  // someone could have pulled the stop switch
					record_fault(f_CFLT_failed_on);
					timers[tredundancy] = 0;
				}
			}
*/
		}
		else
		{
			if (in_redundancy_test != 0)
			{
				timers[trelev] = 0;
				clroutp(o_SU);
				clroutp(o_SD);
				if (((firef == 0) || (firef > 3)) && ((medf == 0) || (medf >= 3)))
				{
					clroutp(o_FST);
					clroutp(o_FSTP);
				}
				if (rdinp(i_DZ) == 0)
				{
					io_fault = 1;			// cannot run test because DZ is dropping
					record_fault(f_Redundancy_DZ_lost);	// lost DZ during Redundancy Test	
				}
				in_redundancy_test = 0;
				redundancy_test = 0;
			}
			timers[tredundancy] = 0;
		}
	}
  	if (((statusf & sf_IO) != 0) || (limit_fault != 0) || (DBR_Temp_Fault == 1)
  			|| (DZ_ON_Fault == 1) || (DZA_ON_Fault == 1) || (DZA_OFF_Fault == 1) || (DZ_OFF_Fault == 1)
			|| (UL_ON_Fault == 1) || (DL_ON_Fault == 1)	|| (UL_DL_OFF_Fault == 1) || (hardware_init_fault == 1)
	  		|| (safe_closetry >= 6) || (safe_rclosetry >= 6) || (dol_fail_cnt >= 3) || (rdol_fail_cnt >= 3) 
			|| (cop_can_error == 1) || (door_motor_ovl == 1) || (shutdown == 1) 
			|| ((aps_sel_can_error == 1) && (cons[sel_type] == 4))
			|| ((sel_can_error == 1) && (cons[sel_type] == 2))
			|| (((statusf & sf_P) != 0) && (Drive_Type[cons[carnmb]] == 9)) 
			|| ((cons[sel_type] == 4) && ((statusf3 & sf_APS_Vel_flt) != 0))
			|| ((statusf3 & sf_NTS_SEL_CAN_Flt) != 0)
			|| ((cons[Voltage_Sensor] == 1) && (voltf != 0))
			|| (eef_testtry >= 6) || (eer_testtry >= 6))
  	{			// io fault so keep the doors open
		if (servf != s_SHUTDOWN)
		{			// first time into this routine
			do_seq = 0;
			do_to = 0;
			rdo_seq = 0;
			rdo_to = 0;
		    timers[tdo] = 0;
		    timers[trdo] = 0;
		    timers[tdwel] = 0;
		    timers[trdwel] = 0;
		    timers[tnudge] = 0;
		    timers[trnudge] = 0;
			doort = fvars[fvexdt];
			rdoort = fvars[fvexdt];
			record_fault(f_atfloor_shutdown);
		}
		servf = s_SHUTDOWN;
		
		if (eef_testtry >= 6) 
			statusf2 |= sf_EE_SHUTDN;
		if (eer_testtry >= 6)
			statusf2 |= sf_EER_SHUTDN;
		
		if (shutdown == 1)
			statusf |= sf_SHUTDN;

		if ((statusf == 0) && (statusf2 == 0) && (statusf3 == 0) && (statusf4 == 0))
			statusf2 |= sf_AF_SHUTDN;			 // Allow to show other faults
	    clrcc(0);
	    clrhc();
	    clrlant();
		dpref = 0;
	    timers[tdc] = 0;
	    timers[trdc] = 0;
		timers[teeto] = 0;
		timers[treeto] = 0;
	    timers[tlant] = 0;
    	timers[trlant] = 0;
	    keep_lants_on = 0;
	    keep_rlants_on = 0;
	    timers[tcablnt] = 0;
	    timers[trcablnt] = 0;
	    timers[tdcycle] = 0;
   	  	if ((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)) || 
   	  		((cons[rear] != 0) && ((rd_both_inps(i_DCBR,i_XDCBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)))))
		{
			safe_closetry = 0;
			safe_rclosetry = 0;
		}
	  	if (rdinp(i_DZ) == 1)
		{
		    if(ccnmsk[cons[carnmb]][position] != 0)
			{
				if (rdinp(i_DCL) == 0)
					timers[tnudge] = 0;
				else if (rdinp(i_DOL) == 0)
				{
					do_seq |= 0x04;
					if (timers[tnudge] > fvars[fvndt])
						doorf = 4;
				}

				if (((rd_both_inps(i_DOB,i_XDOB) == 1) || (rdinp(i_SE) == 1) ||
					((remote_cc == 1) && ((rdinp(i_RM_DOB) == 1) || ((tugf == 1) && (rdinp(i_TGDO) == 1)))) ||
					((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) && (do_to == 0))
					doorf = 1;

				if ((rdinp(i_DOL) == 1) && (do_to == 0) && ((do_seq == 0) || (safe_closetry >= 6)))
				{						  // cycle the door unless close try failure
					if ((doorf != 1) || (doorf != 2))
						doorf = 1;
				}
				else if ((safe_closetry >= 6) || (eef_testtry >= 6) || (manual_door == 2))
				{
				    timers[tdwel] = 0;
					timers[tnudge] = 0;
					if (do_to == 1)
						doorf = 1;
					else
						doorf = 2;
				}
				
				if (cons[rear] != 0)
				{	
					if ((rdinp(i_DCL) == 0) && (rdinp(i_DOL) == 1) && (eef_testtry < 6) && (eer_testtry >= 6))	
					{	// only rear failed ee test
						doorf = 0;	// front doors closed so no action
					}
				}
		    }

			if (cons[rear] != 0)			
		    {
				if(rccnmsk[cons[carnmb]][position] != 0)
				{
					if (rdinp(i_DCLR) == 0)
						timers[trnudge] = 0;
					else if (rdinp(i_DOLR) == 0)
					{
						rdo_seq |= 0x04;
						if (timers[trnudge] > fvars[fvndt])
							rdoorf = 4;
					}

					if( ((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
						((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
						((tugf == 1) && (rdinp(i_TGDOR) == 1))	||
						(rdinp(i_SER) == 1)) && (do_to == 0))
						rdoorf = 1;

					if ((rdinp(i_DOLR) == 1) && (rdo_to == 0) && 
							((rdo_seq == 0) || (safe_rclosetry >= 6)))
					{					// cycle the door unless close try failure
						if ((rdoorf != 1) || (rdoorf != 2))
							rdoorf = 1;
					}
					else if ((safe_rclosetry >= 6) || (eer_testtry >= 6) || (manual_rdoor == 2))
					{
					    timers[trdwel] = 0;
						timers[trnudge] = 0;
						if (rdo_to == 1)
							rdoorf = 1;
						else
							rdoorf = 2;
					}

					if ((rdinp(i_DCLR) == 0) && (rdinp(i_DOLR) == 1) && (eer_testtry < 6) && (eef_testtry >= 6))
					{	// only front failed ee test
						rdoorf = 0;	// rear doors closed so no action
					}
				}
		    }
		}
		else
		{		   // not in the door zone so close the door
		    if (ccnmsk[cons[carnmb]][position] != 0)
			{
				if (doorf != 4)
					doorf = 3;
			}
			if (rccnmsk[cons[carnmb]][position] != 0)
			{
				if (rdoorf != 4)
					rdoorf = 3;
			}
		}
		ck_swing_door();
	    return(0);
  	}
	else
	{
		statusf &= ~sf_SHUTDN;
		statusf2 &= ~sf_AF_SHUTDN;
		statusf2 &= ~sf_EE_SHUTDN;
		statusf2 &= ~sf_EER_SHUTDN;
	}

	return(1);
}

void ck_swing_door(void)
{

		fdoor_req = 0;		 // placed here since ck_swing_door is called prior to return statement and called before doorsel().
		rdoor_req = 0;

	// Special case for manual hoistway door with automatic gate
	if ((manual_door == 2) && (rdinp(i_DZ) == 1) && (man_fdcc() == 0) && (ccmsk[cons[carnmb]][position] != 0))
	{	
		if (rdinp(i_DOL) == 1)
		    doorf = 1;      //open the door
		else
		    doorf = 2;      // keep the door dwelling 

		timers[tdwel] = 0;
		timers[tdo] = 0;
		timers[tnudge] = 0;
		timers[teeto] = 0;
		timers[tdc] = 0;
		if ((doort == 0) || (dpref == 0))
			doort = fvars[fvccdt];
	}		
	
    // Special case for REAR manual hoistway door with automatic gate
	if ((manual_rdoor == 2) && (rdinp(i_DZ) == 1) && (cons[rear] != 0) && (man_rdcc() == 0) && (rccmsk[cons[carnmb]][position] != 0))
	{
 	  	if (rdinp(i_DOLR) == 1)
 	  	    rdoorf = 1;      //open the door 
	 	else
	  	    rdoorf = 2;      // keep the door dwelling 

	    timers[trdwel] = 0;
    	timers[trdo] = 0;
    	timers[trnudge] = 0;
		timers[treeto] = 0;
    	timers[trdc] = 0;
		if ((rdoort == 0) || (dpref == 0))
			rdoort = fvars[fvccdt];
	}
}

void doorsel(void)
{
	int16 enable_cc;

	if ((no_simul_do == 0) || (cons[rear] == 0) || (manual_door == 1) || (manual_rdoor == 1) )
	{					 
		fdoor_en = 1;
		if (cons[rear] == 0)
			rdoor_en = 0;
		else
			rdoor_en = 1;

		fcc_req = 0;
		fdob_req = 0;
		fdoor_req = 0;
		rcc_req = 0;
		rdob_req = 0;
		rdoor_req = 0;

		if ((cons[mand] != 0) && (cons[mand] != 2))
		{				// set nudging flag since it won't be set in door()
			if ((timers[tnudge] >= fvars[fvndt]) || (ee_to == 1))
			{
				if ((dpref & 0x03) == 0)
					nudgst |= 0x04;
				if ((dpref & 0x01) != 0) 
					nudgst |= 0x01;
				if ((dpref & 0x02) != 0)
					nudgst |= 0x02;
			}
			if ((cons[rear] != 0) && ((timers[trnudge] >= fvars[fvndt]) || (ree_to == 1)))
			{
				if ((dpref & 0x03) == 0)
					rnudgst |= 0x04;
				if ((dpref & 0x01) != 0) 
					rnudgst |= 0x01;
				if ((dpref & 0x02) != 0)
					rnudgst |= 0x02;
			}
		}
	}
	else
	{	   // only for automatic front and rear
	
		// Front Door open request

		if ((remote_cc == 0) || ((fvars[fvrmcc] & 0x01) == 0) || (cons[Remote_CC] == 0)) 
			enable_cc = 1;
		else
			enable_cc = 0;

	  	if( ((rdinp(i_CCPBS) == 1) || (c_tmr_svc[s_cc_pb_sec] == 1)) && (cons[cc_pb_sec] == 1) && (Security_code_flag != 0))
			dis_cc_reopen = 1;			 // prevent the door from reopening when code entered
		else
			dis_cc_reopen = 0;

		if ((enable_cc == 1) && (dis_cc_reopen == 0) && (rd_both_inps(i_CC(position),i_XCC(position))==1)	||
			((remote_cc == 1) && (dis_cc_reopen == 0) && (rdinp(i_RM_CC(position))==1)))
			fcc_req = 1;

		if ( (rd_both_inps(i_DOB,i_XDOB) == 1) ||
			((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) ||
			((remote_cc == 1) && ((rdinp(i_RM_DOB) == 1) || ((tugf == 1) && (rdinp(i_TGDO) == 1)))) )
			fdob_req = 1;

		if ( (((front_slowdown & uc_do) != 0) && ((nudgst & 0x01) == 0)) ||
			 (((front_slowdown & dc_do) != 0) && ((nudgst & 0x02) == 0)) ||
			 ( (((front_slowdown & cc_do) != 0) || (fcc_req == 1)) && ((nudgst & 0x04) == 0)) ||
			 ((car_Grp_Stat.c_next_up != 0)  && (car_Grp_Stat.c_ncu_door == 1) && (ncu_door_seq == 0) && (position == lobby_fl)) || 
			 (fdob_req == 1))
			fdoor_req = 1;			// Request for the front door
		else if (fdoor_en == 1)
			fdoor_req = 0;

	  	if (firef != 0) 
			fdoor_req = 0;


		// Rear door open request
	
		if ((remote_rcc == 0) || ((fvars[fvrmcc] & 0x01) == 0) || (cons[Remote_CC] == 0))
			enable_cc = 1;
		else
			enable_cc = 0;

	  	if( ((rdinp(i_CCPBS) == 1) || (c_tmr_svc[s_cc_pb_sec] == 1)) && (cons[cc_pb_sec] == 1) && (Security_code_flag != 0))
			dis_cc_reopen = 1;			 // prevent the door from reopening when code entered
		else
			dis_cc_reopen = 0;

		if (((enable_cc == 1) && (dis_cc_reopen == 0) && (rd_both_inps(i_CCR(position),i_XCCR(position))==1)) ||
			((remote_rcc == 1) && (dis_cc_reopen == 0) && (rdinp(i_RM_CCR(position))==1)))
			rcc_req = 1;
		if ((rd_both_inps(i_DOBR,i_XDOBR) == 1) ||
			((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
			((remote_rcc == 1) && (rdinp(i_RM_DOBR) == 1)) ||
			((tugf == 1) && (rdinp(i_TGDOR) == 1)))
			rdob_req = 1;

		if ( (((rear_slowdown & uc_do) != 0) && ((rnudgst & 0x01) == 0)) ||
			 (((rear_slowdown & dc_do) != 0) && ((rnudgst & 0x02) == 0)) ||
			 ( (((rear_slowdown & cc_do) != 0) || (rcc_req == 1)) && ((rnudgst & 0x04) == 0)) ||
			 (rdob_req == 1))
			rdoor_req = 1;
		else if (rdoor_en == 1)
			rdoor_req = 0;

	  	if (rfiref != 0) 
			rdoor_req = 0;

		// Set the door enable flag
	
		if (((fvars[fvno_simul_do] == 1)  || ((servf == s_SABBATH) && ((fvars[fvsabben2] & 0x04) != 0))))
		{		 // Open the front door first
			if ((rdoor_req == 0) && (rdoorf == 0))
			{
				fdoor_en = 1;
				rdoor_en = 0;
				timers[trdo_adv_de] = 0;
			}
			else if ((fdoor_req == 1) && (rdoor_req == 1))
			{
				if ((rdinp(i_DCL) == 1) || (doorf == 1))
				{
					fdoor_en = 1;		// front door already open
					rdoor_en = 0;
				}
				else if ((car_Grp_Stat.c_next_up != 0) && (ncu_door_seq == 0) && (position == lobby_fl))		// request for next up so open the rear door first
				{
					rdoor_en = 1;		// open rear first
					fdoor_en = 0;		
				}
                else if ((((rear_slowdown & cc_do) != 0) || (rcc_req == 1)) && ((rnudgst & 0x04) == 0) && ((front_slowdown & cc_do) == 0))
                {
	                rdoor_en = 1;           // open rear first
					fdoor_en = 0;          
	            }
	            else if ((rdinp(i_DCLR) == 0) || (cons[rear] == 0))
				{
					fdoor_en = 1;
					rdoor_en = 0;
				}
				else 	// else open the rear door
				{
					rdoor_en = 1;
					fdoor_en = 0;
				}
			}
			else if ((fdoor_req == 0) && (rdoor_req == 1))
			{

				if (doorf == 0)
				{
					fdoor_en = 0;
					rdoor_en = 1;
				}
				else if ((fvars[fvdo_adv_de] != 0) && (timers[tfdo_adv_de] > fvars[fvdo_adv_de]))
					rdoor_en = 1;
			}
		}
		else if (fvars[fvno_simul_do] == 2)
		{			   // Open the rear door first
			if ((car_Grp_Stat.c_next_up != 0) && (position == lobby_fl) && (rdoor_req == 0) && (rdoorf == 0))
			{
				fdoor_en = 1;
				rdoor_en = 0;
			}
			else if ((fdoor_req == 0) && (doorf == 0))
			{
				rdoor_en = 1;
				fdoor_en = 0;
				timers[tfdo_adv_de] = 0;
			}
			else if ((rdoor_req == 1) && (fdoor_req == 1))
			{
				if ((rdinp(i_DCLR) == 1) || (rdoorf == 1))
				{
					rdoor_en = 1;		// rear door already open
					fdoor_en = 0;
				}
                else if ((((front_slowdown & cc_do) != 0) || (fcc_req == 1)) && ((nudgst & 0x04) == 0) && ((rear_slowdown & cc_do) == 0))
                {
                        fdoor_en = 1;
                        rdoor_en = 0;
                }
				else if (rdinp(i_DCL) == 0)
				{
					rdoor_en = 1;
					fdoor_en = 0;
				}
				else if ((car_Grp_Stat.c_next_up != 0) && (ncu_door_seq == 0) && (position == lobby_fl))		// request for next up so open the rear door first
				{
					rdoor_en = 1;		// open rear first
					fdoor_en = 0;		
				}
				else 	// else open the front door
				{
					fdoor_en = 1;
					rdoor_en = 0;
				}
			}
			else if ((rdoor_req == 0) && (fdoor_req == 1))
			{
				if (rdoorf == 0)
				{
					fdoor_en = 1;
					rdoor_en = 0;
				}
				else if ((fvars[fvdo_adv_de] != 0) && (timers[tfdo_adv_de] > fvars[fvdo_adv_de]))
					fdoor_en = 1;
			}
		}

		if (fdoor_en == 0)
		{
			timers[tnudge] = 0;
			timers[teeto] = 0;
		}

		if (rdoor_en == 0)
		{
			timers[trnudge] = 0;
			timers[treeto] = 0;
		}

		if (dpref == 0)
		{					// no onward calls so reset door status
			do_seq = 0;
			rdo_seq = 0;
			nudgst = 0;				
			rnudgst = 0;
		}

	}
}

// ****************************************
// Clear door control variables
// ****************************************
void clr_door_vars (void)
{
	do_seq = 0;
	rdo_seq = 0;
	do_to = 0;
	rdo_to = 0;
	fcc_req = 0;
	fdob_req = 0;
	fdoor_req = 0;
	rcc_req = 0;
	rdob_req = 0;
	rdoor_req = 0;
	nudgst = 0;
	rnudgst = 0;
  	if (cons[Auto_Swing_DO] == 1)
  	{
		fsdo = 0;
		fsdo_seq = 0;
		rsdo = 0;
		rsdo_seq = 0;
    	clroutp(o_FSDO);
	  	if (cons[rear] != 0)
			clroutp(o_RSDO);
  	}
  	if (fvars[fvminshortdten] != 0)
	{
		if (firef == 0)
			short_doort_en = 0;
		if ((cons[rear] != 0) && (rfiref == 0))
			short_rdoort_en = 0;
	}
	if ((cons[frghtd] == 10) || (cons[frghtd] == 12))
	{
		reset_eef_test = 1;
		clroutp(o_FETST);
	}
	if ((cons[frghtd] == 11) || (cons[frghtd] == 12))
	{
		reset_eer_test = 1;
		clroutp(o_RETST);
	}
}

//*****************************
// Set the Service flag to auto
//*****************************

void set_auto_servf (void)
{
	statusf &= ~(sf_AD | sf_STP | sf_GTS | sf_FSTP);
  	if((rdinp(i_AD) == 0) || (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) || 
			(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ad_off == 1)) ||
  			((rdinp(i_MRSW) == 0) || (FF_STP == 0)) ||
			((aps_sel_can_error == 1) && (cons[sel_type] == 4)) ||
  			 (cop_can_error == 1) || (nts_spi_error == 1) || ((sel_can_error == 1) && (cons[sel_type] == 2)) )
  	{
		if (rdinp(i_MRSW) == 0)
		{
			if (rdinp(i_GTS) == 0)
				statusf |= sf_GTS;
			else
				statusf |= sf_STP;
		 	servf = s_MRSTOP;
		}
		else if (FF_STP == 0) 
		{
			statusf |= sf_FSTP;
			servf = s_FFSTOP;
		}
		else if ((rdinp(i_AD) == 0) || (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) || (((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ad_off == 1)))
		{
#if ((Traction == 1) || (Tract_OL == 1))
			if (servf != s_LW_CAL)
#endif
				servf = s_ADOFF;
			statusf |= sf_AD;
		}
		else if ((cop_can_error == 1) || (nts_spi_error == 1) || 
				((aps_sel_can_error == 1) && (cons[sel_type] == 4)) ||
				((sel_can_error == 1) && (cons[sel_type] == 2))) 
		{
			clrcc(0);
			clrhc();
			clrlant();
			servf = s_COM_FAIL;
		}
  	}
  	else
  	{
#if ((Traction == 1) || (Tract_OL == 1))
		if ((((((cons[loadw] & 1) != 0) && (rdinp(i_LWB) == 1) && (lw_disable == 0)) ||
			(((cons[loadw] & 4) != 0) && (rdinp(i_LWB) == 1) && (lw_disable == 0)) ||
			(((cons[loadw] & 2) != 0) && (load_bp == 1)) ) && (onward_cc != 0)) ||
			(timeout_mandoor == 1))
#else
		if ((((cons[loadw] & 1) != 0) && (rdinp(i_LWB) == 1) && (onward_cc != 0))||
		(timeout_mandoor == 1))
#endif
		{	// Load weighing bypass
			servf = s_LWBP;
		}
		else if ((servf != s_HOMING) && (servf != s_LW_CAL) && (servf != s_LBY_RCL) && (servf != s_ELEV_OFF_HS) && (servf != s_CAR_OFF) && (servf != s_VIP) && 
				((cons[Prison] != 2) || (servf != s_RIOT)) && (servf != s_TUGS) && ((rhjackf < 2) || (cons[Reset_Jack] == 0)))
		{
			if ((empf == ep_home) || (empf == ep_wait_door))

				servf = s_EMP;
			else
				servf = s_AUTO;
		}
  	}
}


//***********************************
// Automatic Service Time-out Recover
//    Car must make a run
//***********************************
void auto_svc_time_out_recover (void)
{
	int16 i;

	if ((car_Grp_Stat.c_asvto_pos != 0) && (servf == s_AUTO))
	{
		if ((car_Grp_Stat.c_asvto_pos == position) && (zone_fl == 0))
		{
			if (position < cons[topf])
			{				
				i = position;
				while ((i < cons[topf]) && (zone_fl == 0))
				{
					i++;
					if (ccmsk[cons[carnmb]][i] != 0)
						zone_fl = i;
				}
			}
			if ((zone_fl == 0) && (position > cons[bottomf]))
			{
				i = position;
				while ((i > cons[bottomf]) && (zone_fl == 0))
				{
					i--;
					if (ccmsk[cons[carnmb]][i] != 0)
						zone_fl = i;
				}
			}
		}
	}
}

//****************************
// This is the door procedure
// ***************************
void doors()
{
  	int16 disable_dcb;
	int16 front_door_closed =0;

	write_trace(TR_DOORS);

	if (cons[door_type] == 1)
	{
		if (cons[equake] == 0)
		{
	  	  	if (((rdinp(i_SE) == 1) && (firef <= 3)) ||   // saftey edge
			 	(((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
				 ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && (firef == 0) && (ee_to == 0)))		// EE2 input ON
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
   	if((ccnmsk[cons[carnmb]][position] == 0) && (ucmsk[cons[carnmb]][position] == 0)	 
   		 && (dcmsk[cons[carnmb]][position] == 0))
	{
		if ((doorf == 1) || (doorf == 2))
		{
	  	  	if ( (rdinp(i_DCL) == 1) || 
	  	  		( (rdinp(i_DZ) == 0) && ((door_locks() == 0) || (car_gate() == 0)) ) )
				doorf = 4;	// nudge closed
			else
				doorf = 0;
		}
	}


	// Special Automatic Swing door open
	if (cons[Auto_Swing_DO] == 1)
	{
		if ((rdinp(i_DZ) == 1) && (firef == 0))
		{
			if ((fsdo == 1) && (fsdo_seq == 0))
				fsdo_seq = 1;

			if (fsdo_seq == 0)
				clroutp(o_FSDO);
			else if (fsdo_seq == 1)
			{
				timers[tfsdo] = 0;
				if (preset == 1)
					fsdo_seq = 2;
				clroutp(o_FSDO);
			}
			else if (fsdo_seq == 2)
			{
				if (timers[tfsdo] < 15)
					setoutp(o_FSDO);
				else
				{
					if (fsdo == 0)
						fsdo_seq = 0;
					clroutp(o_FSDO);
				}
			}
			fsdo = 0;
		}
		else
		{
			fsdo = 0;
			fsdo_seq = 0;
			clroutp(o_FSDO);
		}
	}
	
	//********************************************************************
	//	Door_Lock_GS_DPM_Fault mechanism monitors DPM sequence and
	//	DPM, GS or DLT,DLM,DLB and RLM jumped conditions
	//********************************************************************
	
	// front DPM sequence check for timing of DPM versus GS and Door locks
	if((cons[mand] == 0) && (cons[frghtd] == 0))
	{
		if ((rdinp(i_DPM) == 0) && (front_op[cons[carnmb]] != 0) &&
			((door_locks() == 1) || ((rdinp(i_GS) == 1) && (rdinp(i_GS_1) == 1))))
		{
			if(timers[tdpm] > 3)
			{
				if (Door_DPM_seq_front_fault == 0)
					record_fault(f_dpm_gl);
				statusf |= sf_GDL;	
				servf = s_GATE_LOCK;
				Door_Lock_GS_DPM_Fault = 1;	// dpm seq - front
				Door_DPM_seq_fault = 1;
				Door_DPM_seq_front_fault = 1;
			}
		}
		else 
		{
			Door_DPM_seq_front_fault = 0;
			timers[tdpm] = 0;
			if(Door_DPM_seq_fault == 1)
			{
				if((cons[rear] == 0) || (Door_DPM_seq_rear_fault == 0))
				{// no rear or rear without any DPMseq issues
					Door_DPM_seq_fault = 0;
				}
			}
		}
	}
	else
	{//	zero values only in doors(), no need to be done in rdoors()
		Door_DPM_seq_fault = 0;
		Door_DPM_seq_front_fault = 0;
		Door_DPM_seq_rear_fault = 0;
	}
	// reset Door_Lock_GS_DPM_Fault mechanism
	if((manual_door == 0) || (manual_door == 2))
	{// do in both doors() and rdoors()
		if((Door_DPM_seq_front_fault == 0) && ((cons[rear] == 0) || (Door_DPM_seq_rear_fault == 0)))
		{
			Door_DPM_seq_fault = 0;
		}
		if((Door_Lock_front_fault == 0) && ((cons[rear] == 0) || (Door_Lock_rear_fault == 0)))
		{
			Door_Lock_fault = 0;	
		}
		if((Door_GS_front_fault == 0) && ((cons[rear] == 0) || (Door_GS_rear_fault == 0)))
		{
			Door_GS_fault = 0;	
		}
		if((Door_DPM_front_fault == 0) && ((cons[rear] == 0) || (Door_DPM_rear_fault == 0)))
		{
			Door_DPM_fault = 0;	
		}
		if (doorf != 2)
		{// clear front failed on conditions
			Door_Lock_front_fault = 0;
			Door_GS_front_fault = 0;
			Door_DPM_front_fault = 0;
		}
	}
	else
	{
		Door_DPM_seq_front_fault = 0;
		Door_Lock_front_fault = 0;
		Door_GS_front_fault = 0;
		Door_DPM_front_fault = 0;
	}

	if(Door_Lock_GS_DPM_Fault == 1)
	{	
		if((Door_DPM_seq_fault == 0) && (Door_Lock_fault == 0) && (Door_GS_fault == 0) && (Door_DPM_fault == 0))
		{
			Door_Lock_GS_DPM_Fault = 0;		
			statusf &= ~sf_GDL;				// in doors() autoreset if fault contition clears.
			servf = s_AUTO;
		}
	}	
	//********************************************************************


		
		
				
  	if (doorf == 0)
  	{
		timers[tlant] = 0;
    	timers[trlant] = 0;
		timers[tdo] = 0;
		if ((cons[rear] == 0) || (rdoorf == 0))
		{
			if (cons[exDHwL] == 1)
				clroutp(o_EDL);
			extended_door_time = 0;
		}
		if (dpref == 0)
		{
			nudgst = 0;
			timers[tnudge] = 0;
			timers[teeto] = 0;
		}
		clroutp(o_DO);
		if(((safe() == 1) || (cons[pbdoor] != 0)) && ((dpref == 0) || ((cons[pbdoor] & 2) != 0)))
		    clroutp(o_DC);
		else if ((rdinp(i_DCL) == 1) && (firef == 0))
			doorf = 3;            // go to door close but not if in fire service
		
		clroutp(o_NUD);
		freight_fdoor_alarm = 0;
		if ((fvars[fvminshortdten] != 0) && (firef == 0))
			short_doort_en = 0;
		else
			short_doort_en = 1;
		return;
  	}
  	if(doorf == 1)        // The doors are in openning mode
  	{
		timers[tdcdly] = 0;
		t_door_dwell = doort;

		trigger_arm_state |= DO_START;
		
		if ((cons[frghtd] == 10) || (cons[frghtd] == 12))
		{
	  	  	freight_fdoor_alarm = 0;
			if (reset_eef_test == 0)
				reset_eef_test = 1;
		}

  	    if( ((disable_auto_door == 1) && ((rdinp(i_AD) == 0) || 
  	    	(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) || 
  	    	(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ad_off == 1))) && 
  	    	(firef == 0) && (stallf == 0)) || ((manual_door == 1) && (cons[frghtd] == 0)) )
  	    {
			doorf = 0;
			clroutp(o_DC);
			clroutp(o_NUD);
			clroutp(o_DO);
			return;
  	    }
		do_to = 0;
  	  	if(rdinp(i_DOL) == 1)
  	  	{
  	  	  	clroutp(o_DC);
  	  	  	clroutp(o_NUD);
  	  	  	if (timers[tdo] >= fvars[fvdpt])   // door open failure
  	  	  	{
  	  	  		dol_fail = 1;
				front_slowdown = 0;
				dtmrf = 0;
				if ((dpref & 0x03) == 0)
					do_seq |= 0x04;
				if ((dpref & 0x01) != 0) 
					do_seq |= 0x01;
				if ((dpref & 0x02) != 0)
					do_seq |= 0x02;
				do_to = 1;  
				if ((manual_door == 0) || (manual_door == 2))
					record_fault(f_door_open);
				clroutp(o_DO);
				doorf = 3;      // close the doors
				if ((manual_door == 0) || (manual_door == 2) || (door_locks() == 0) || (car_gate() == 0))
					timers[tnit] = 0; 		// reset non-interference timer
				timers[tdc] = 0;			   
				return;
  	  	  	}

			if (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (rdinp(i_DSI) == 1))
			{
				timers[tdo] = 0;
			    clroutp(o_DO);
			}
			else if ((cons[door_type] == 1) && ((cons[vshort_tfl] == 0) || ((relevel == 0) && (relevel_req == 0)) || (do_seq != 0)) && ((rdinp(i_DZ) == 1) || (rdinp(i_DZA) == 1))) 	// otis ovl door operator
			      setoutp(o_DO);
			else if((timers[tdoor_reopen] >= 3) && ((cons[vshort_tfl] == 0) || ((relevel == 0) && (relevel_req == 0)) || (do_seq != 0)) && ((rdinp(i_DZ) == 1) || (rdinp(i_DZA) == 1)) && (timers[tdodly] >= fvars[fvdrdly]))
			      setoutp(o_DO);
  	  	}
  	  	else
  	  	{
  	  		dol_fail = 0;
  	  		dol_fail_cnt = 0;
  	  	  	t_door_open = timers[tdo];
  	  	  	clroutp(o_DC);
  	  	  	clroutp(o_NUD);
			if (cons[door_type] != 1)	// leave the door open on for otis ovl door operator
				clroutp(o_DO);
  	  	  	timers[tdwel] = 0;
  	  	  	if ((cons[frghtd] != 0) && (cons[frghtd] <= 5))
				doorf = 0;
			else
  	  	  		doorf = 2;
			if ((dpref & 0x03) == 0)
				do_seq |= 0x04;
			if ((dpref & 0x01) != 0) 
				do_seq |= 0x01;
			if ((dpref & 0x02) != 0)
				do_seq |= 0x02;
  	  	}
  	  	return;
  	}
  	if(doorf == 2)        // The doors are in dwel mode
  	{
		trigger_arm_state |= DD_START;

		if((manual_door == 1)&&(firef == 0))
 	    {
			doorf = 0;
			clroutp(o_DC);
			clroutp(o_NUD);
			clroutp(o_DO);
			return;
  	    }

		t_door_dwell = doort;
		if (((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12)) && (fvars[fvdoorbz] != 0))
			timers[tdcdly] = 0;
  	  	clroutp(o_DC);
  	  	clroutp(o_NUD);
  	  	freight_fdoor_alarm = 0;
		if ((cons[door_type] == 1) && (rdinp(i_DZ) == 1)) 	// otis ovl door operator
		    setoutp(o_DO);		// keep door open output on
		else if (fvars[fvdonodol] == 1)
		{
	  	  	if(rdinp(i_DOL) == 1)
			    setoutp(o_DO);		// Turn on door open output if DOL not hit
			else
	  	  		clroutp(o_DO);
		}
		else
  	  		clroutp(o_DO);

#if ((Traction == 0) && (Tract_OL == 0))
		if ((manual_door == 0) && (do_to == 0) && (rdinp(i_DOL) == 0) && (procf != 19))	 // hydro do not check door lock on low pressure switch operation
#else
		if ((manual_door == 0) && (do_to == 0) && (rdinp(i_DOL) == 0))		// automatic door
#endif
		{									   // disable gate and lock test if bit set and new york fire service
			if ((rdinp(i_GS) == 1) || (rdinp(i_GS_1) == 1))
	  	  	{
			    timers[tnudge] = 0;
				timers[teeto] = 0;
			    timers[tdwel] = 0;
			  	statusf |= sf_GDL;
				if (Door_GS_fault == 0)
					record_fault(f_gs_on);
			    servf = s_GATE_LOCK;
			    Door_Lock_GS_DPM_Fault = 1;
			    Door_GS_fault = 1;
			    Door_GS_front_fault = 1;
			    return;
	  	  	}

			if ((position == fvars[fvacctopfl]) && ((cons[access_type] == 0) || ((cons[access_type] & 1) != 0)))
			{
				if ((rdinp(i_DLT) == 1) || (rdinp(i_DLT_1) == 1))
			    {
				    timers[tnudge] = 0;
					timers[teeto] = 0;
				    timers[tdwel] = 0;
				  	statusf |= sf_GDL;
					if (Door_Lock_front_fault == 0)
						record_fault(f_dlt_on);
				    servf = s_GATE_LOCK;
				    Door_Lock_GS_DPM_Fault = 1;
				    Door_Lock_fault = 1;
				    Door_Lock_front_fault = 1;
				    return;
			    }
			}
			else if ((position == fvars[fvaccbotfl]) && ((cons[access_type] == 0) || ((cons[access_type] & 2) != 0)))
			{
				if ((rdinp(i_DLB) == 1) || (rdinp(i_DLB_1) == 1))
			    {
			      	timers[tnudge] = 0;
					timers[teeto] = 0;
			      	timers[tdwel] = 0;
				  	statusf |= sf_GDL;
					if (Door_Lock_front_fault == 0)
						record_fault(f_dlb_on);
			      	servf = s_GATE_LOCK;
			      	Door_Lock_GS_DPM_Fault = 1;
			      	Door_Lock_fault = 1;
			      	Door_Lock_front_fault = 1;
			      	return;
			    }
			}
			else // middle floor
			{
				if ((rdinp(i_DLM) == 1) || (rdinp(i_DLM_1) == 1))
			    {
			      	timers[tnudge] = 0;
					timers[teeto] = 0;
			      	timers[tdwel] = 0;
				  	statusf |= sf_GDL;
					if (Door_Lock_front_fault == 0)
						record_fault(f_dlm_on);
			      	servf = s_GATE_LOCK;
			      	Door_Lock_GS_DPM_Fault = 1;
			      	Door_Lock_fault = 1;
			      	Door_Lock_front_fault = 1;
			      	return;
			    }
			}

			if ((cons[mand] == 0) && (cons[frghtd] == 0))
			{
			    if(rdinp(i_DPM) == 1)
			    {
				    timers[tnudge] = 0;
					timers[teeto] = 0;
					if(timers[tdwel] > 100)
					{
						if (Door_DPM_fault == 0)
							record_fault(f_dpm_on);
				        servf = s_GATE_LOCK;
				        Door_Lock_GS_DPM_Fault = 1;
				        Door_DPM_fault = 1;
				        Door_DPM_front_fault = 1;
					}
				    return;
			    }
			}
		}

  	  	if(Door_Lock_GS_DPM_Fault == 1)		// 		doors()
  	  	{	// clear front failed on conditions

			Door_Lock_front_fault = 0;
			Door_GS_front_fault = 0;
			Door_DPM_front_fault = 0;
			//return because it still has issues!
			return;	
 	  	}
  	  


	  	if( ((rdinp(i_CCPBS) == 1) || (c_tmr_svc[s_cc_pb_sec] == 1)) && (cons[cc_pb_sec] == 1) && (Security_code_flag != 0))
			return;

    	if (closetry >= 6)
    	{
    	  	servf = s_DC_FAIL;
    	  	if ((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1)))
    	  	{
				servf = s_AUTO;
				closetry = 0;
    	  	}
    	  	timers[tnudge] = 0;
			timers[teeto] = 0;
    	  	timers[tdwel] = 0;
    	}

		if (fvars[fvdcbdt] == 1)
			disable_dcb = 1;
		else if ((position == lobby_fl) && (car_Grp_Stat.c_ncu_door == 1) && (car_Grp_Stat.c_next_up != 0) && (manual_door == 0)) 
		{	// This car is next up
			if ((timers[tncu] <= fvars[fvldt]) || (onward_calls == 0))
				disable_dcb = 1;
			else
				disable_dcb = 0;
		}
		else if (((manual_door == 2) && (man_dcc() == 0)) || ((cons[cblue] !=0) && ((codebf == 3) || (codebf == 4))))
			disable_dcb = 1;
		else
			disable_dcb = 0;
     	
      	if ((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))		// clear nudging timer if front 
     		timers[tnudge] = 0;								// automatic freight door.
			
		if ((fvars[fvminshortdten] != 0) && (firef == 0))
		{
			if (short_doort_en == 0)
			{
				if ((dtmrf & hc_dt) != 0)	// hall call door time was set
				{
					if (timers[tdwel] < fvars[fvhcdt])
						disable_dcb = 1;		// disable DCB and shortenning of doort
					else
						short_doort_en = 1;
				}
				else	// otherwise disable DCB only until cc dwell time
				{
					if (timers[tdwel] < fvars[fvccdt])
						disable_dcb = 1;
					else
						short_doort_en = 1;
				}
			}
		}
		else
			short_doort_en = 1;
		
		
    	if ((timers[tdwel] > doort) || 
     	    ((disable_dcb == 0) &&
			 (((rd_both_inps(i_DCB,i_XDCB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCB) == 1))) ||
			  ((remote_cc == 1) && ((rdinp(i_RM_DCB) == 1) || ((tugf == 1) && (rdinp(i_TGDC) == 1)))) ||
			  (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDCB) == 1)))) ||
    	    (((timers[tnudge] >= fvars[fvndt]) || (ee_to == 1)) && ((onward_calls != 0) || (fvars[fvnudowc] == 1))) )
    	// door dwell time is up or the door close button was pushed or nudging
    	{
    	  	Update_Job_Statistics();
    	  	if (((timers[tnudge] >= fvars[fvndt]) || (ee_to == 1)) && ((onward_calls != 0) || (fvars[fvnudowc] == 1)))
    	  	{
				nudg_buz = 1;
				doorf = 4;
    	  	}
    	  	else
    	  	{
				nudg_buz = 0;
				
				if (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (cons[eetst_dis] == 0))
				{
					if (freight_eef_test() == 1)
						doorf = 3;
					timers[tdodly] = 0;
				}
				else
				{
					doorf = 3;        // close the doors					
				}

				if ((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))
				    timers[tdodly] = 0;
    	  	}
    	  	timers[tdc] = 0;
			if (cons[exDHwL] == 1)
				clroutp(o_EDL);
			extended_door_time = 0;
    	  	return;
    	}
    	return;
  	}
  	if(doorf == 3)        // The doors are in closing mode
  	{
		trigger_arm_state |= DC_START;
		front_slowdown = 0;
		dtmrf = 0;
		set_long_door_time = 0;
		if((cons[frghtd] == 6) || (cons[frghtd] == 8) || (cons[frghtd] == 10) || (cons[frghtd] == 12))
		{
		    if(rdinp(i_DCL) == 1)
				 timers[tdodly] = 0;			
			if (timers[tdodly] <= 30)		// applies DC output for 3 s extra
				front_door_closed = 0;
			else
			{
				front_door_closed = 1;
				//timers[tdodly] = 0;
			}
		}
		else
		{
		    timers[tdodly] = 0;		
 		    if(rdinp(i_DCL) == 1)
				front_door_closed = 0;
			else
				front_door_closed = 1;
		}

  	  	if ( (front_door_closed == 0) || ( (rdinp(i_DZ) == 0) && ((door_locks() == 0) || (car_gate() == 0)) ) )
  	  	{
  	  	  	clroutp(o_DO);
  	  	  	if(((rdinp(i_MRSW) == 0) || (FF_STP == 0)) && (firef == 0) && (rdinp(i_DZ) == 1))
  	  	  	{
		  		clroutp(o_DC);		  
		  		clroutp(o_NUD);
		  		clroutp(o_DO);
				nudg_buz = 0;
		  		closetry = 0;
		  		return;
  	  	  	}
  	  	  	if((timers[tdc] >= fvars[fvdpt]) && (manual_door == 0))   // door close failure
  	  	  	{
		  		closetry++;
		  		if(closetry >= 6)
		  		{
					record_fault(f_door_close);
		  		}
		  		clroutp(o_DC);
		  		clroutp(o_NUD);
		  		doorf = 1;      // open the doors
		  		timers[tdo] = 0;
		  		timers[tnudge] = 0;
				timers[teeto] = 0;
		  		return;
  	  	  	}
  	  	  	if (((timers[tnudge] >= fvars[fvndt]) || (ee_to == 1)) && ((onward_calls != 0) || (fvars[fvnudowc] == 1) || (rdinp(i_DZ) == 0)))
  	  	  	{
				doorf = 4;
				timers[tdc] = 0;
				return;
  	  	  	}
  	  	  	if (((rdinp(i_SE) == 1) && (firef <= 3)) ||   // saftey edge
			 	(((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
		 		 ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && (firef == 0)) ||   // EE2 input ON
			 	(((rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)) ||
				  ((remote_cc == 1) && ((rdinp(i_RM_DOB) == 1) || ((tugf == 1)&&(rdinp(i_TGDO) == 1)))) ||
				  (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDOB) == 1)))
			 		&& (FS_DOB_DIS == 0)))	// door open button
  	  	  	{
				clroutp(o_DC);
				clroutp(o_NUD);
				timers[tdc] = 0;
				if (rdinp(i_DZ) == 1)
				{
					doorf = 1;      // open the doors
					timers[tdo] = 0;
				}
				return;
  	  	  	}

  	  	  	timers[tdoor_reopen] = 0;
  	  	  	if( (firef >= 1) && (firef <= 3) && (fs2_to_fs1 == 0) )
				setoutp(o_NUD);
  	  	  	else
				clroutp(o_NUD);
  			nudg_buz = 0;
			if (((cons[frghtd] == 6) || (cons[frghtd] == 8)) && (fvars[fvdoorbz] != 0))
			{
				if (timers[tdcdly] >= fvars[fvdoorbz])
  	  			{
  	  				setoutp(o_DC);
					freight_fdoor_alarm = 0;
				}
				else
					freight_fdoor_alarm = 1;
			} 
			else if (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (firef == 0) && (fvars[fvdoorbz] != 0))
			{	// peele freight doors with buzzer and ee and ee2 test

				if (timers[tdcdly] >= fvars[fvdoorbz])
  	  			{
					if (rdinp(i_DSI) == 0)
  	  					setoutp(o_DC);
  	  				else
					{
						timers[tdc] = 0;
  	  					clroutp(o_DC);
					} 
					freight_fdoor_alarm = 0;
				}
				else
					freight_fdoor_alarm = 1;
			}
			else
			{
				if (((cons[frghtd] == 10) || (cons[frghtd] == 12)))
				{
					if (timers[tdcdly] >= fvars[fvdrdly])
					{
						if (rdinp(i_DSI) == 0)
							setoutp(o_DC);
	  	  				else
						{
							timers[tdc] = 0;
	  	  					clroutp(o_DC);
						}
					}
					if (freight_fdoor_alarm != 0)
						freight_fdoor_alarm = 0;
				}
				else if (timers[tdcdly] >= fvars[fvdrdly])
				{
	  				setoutp(o_DC);
				}
			}
			if ((cons[rear] == 0) || (rdoorf == 0) || (rdoorf == 3) || (rdoorf == 4))
			{
				if (cons[exDHwL] == 1)
					clroutp(o_EDL);
				extended_door_time = 0;
			}
  	  	}
  	  	else
  	  	{
  	  	  	t_door_close = timers[tdc];
			if (dpref == 0)
  	  	  		clroutp(o_DC);
  	  	  	clroutp(o_NUD);
  	  	  	clroutp(o_DO);
  			nudg_buz = 0;
  	  	  	closetry = 0;
  	  	  	doorf = 0;
  	  	}
  	  	return;
  	}
  	if(doorf == 4)        // The doors are in nudgeing close mode
  	{
		trigger_arm_state |= DC_START;
		set_long_door_time = 0;
		if ((dpref & 0x03) == 0)
			nudgst |= 0x04;
		if ((dpref & 0x01) != 0) 
			nudgst |= 0x01;
		if ((dpref & 0x02) != 0)
			nudgst |= 0x02;
		front_slowdown = 0;
		dtmrf = 0;
		timers[tdodly] = 0;
  	  	if ( (rdinp(i_DCL) == 1) || ( (rdinp(i_DZ) == 0) && ((door_locks() == 0) || (car_gate() == 0)) ) )
  	  	{
  	  	  	clroutp(o_DO);
  	  	  	if((timers[tdc] >= fvars[fvdpt]) && (manual_door == 0))   // door close failure
  	  	  	{
				closetry++;
				if(closetry >= 6)
				{
					record_fault(f_door_close);
				}
				clroutp(o_DC);
				clroutp(o_NUD);
 				timers[tnudge] = 0;
				timers[teeto] = 0;
 				doorf = 1;      // open the doors
				timers[tdo] = 0;
				timers[tdc] = 0;
				return;
  	  	  	}

	  	  	if ( (((rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) && 	// door open button
  	  	  							(FS_DOB_DIS == 0) && (fvars[fvdobndg] == 1)) || // door open is allowed to reopen door
  	  	  	    	( ((fvars[fvndgctl] & 0x02) != 0) &&
    			 	   ((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
		 				((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && (firef == 0) && (ee_to == 0))) 	// EE2 input ON
  	  	  	{
				clroutp(o_DC);
				clroutp(o_NUD);
				nudg_buz = 1;		// buz on reopen

				timers[tdc] = 0;
				if (rdinp(i_DZ) == 1)
				{
					doorf = 1;      // open the doors
					timers[tdo] = 0;
				}
				return;
  	  	  	}
  	  	  	else if (((rdinp(i_SE) == 1) && (firef <= 3)) ||   // saftey edge
    			 	 (((rdinp(i_EE) == 1) || (rdinp(i_DET) == 1) ||	// EE1 input ON
					   ((eef2_enabled == 1) && (rdinp(i_EE2) == 1))) && 	// EE2 input ON
	 				  (firef == 0) && (ee_to == 0) && ((fvars[fvndgctl] & 0x05) != 0)) || 
			  			(rd_both_inps(i_DOB,i_XDOB) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1)))                // door open button
  	  	  	{			   // stop the door
				clroutp(o_DC);
				clroutp(o_NUD);
	  			nudg_buz = 1;
				timers[tdc] = 0;
				return;
  	  	  	}
  	  	  	timers[tdoor_reopen] = 0;

			if (((fvars[fvndgctl] & 0x01) == 0) || (firef != 0) || (ee_to != 0))	// this bit forces the ee to remain active
  	  	  		setoutp(o_NUD);
			else
  	  	  		clroutp(o_NUD);

			if (((fvars[fvndgctl] & 0x04) == 0) || (firef != 0) || (ee_to != 0))
	  			nudg_buz = 1;
			else
				nudg_buz = 0;

			if (((cons[frghtd] == 6) || (cons[frghtd] == 8)) && (fvars[fvdoorbz] != 0))
			{
				if (timers[tdcdly] >= fvars[fvdoorbz])
				{
		  	    	setoutp(o_DC);
					freight_fdoor_alarm = 0;
				}
				else
					freight_fdoor_alarm = 1;
			} 
			else if (((cons[frghtd] == 10) || (cons[frghtd] == 12)) && (firef == 0) && (fvars[fvdoorbz] != 0))
			{
				if (timers[tdcdly] >= fvars[fvdoorbz])
  	  			{
					if (rdinp(i_DSI) == 0)
						setoutp(o_DC);
					else
					{
						timers[tdc] = 0;
  	  					clroutp(o_DC);
					}
					freight_fdoor_alarm = 0;
				}
				else
					freight_fdoor_alarm = 1;
			}
			else
			{	
				if ((cons[frghtd] == 10) || (cons[frghtd] == 12))
				{
					if (timers[tdcdly] >= fvars[fvdrdly])
					{
						if (rdinp(i_DSI) == 0)
							setoutp(o_DC);
	  	  				else
						{
							timers[tdc] = 0;
	  	  					clroutp(o_DC);
						}
					}
					if (freight_fdoor_alarm != 0)
						freight_fdoor_alarm = 0;
				}
				else
				{
					if (timers[tdcdly] >= fvars[fvdrdly])
		  	  	  		setoutp(o_DC);					
				}
			} 
			
			if ((cons[rear] == 0) || (rdoorf == 0) || (rdoorf == 3) || (rdoorf == 4))
			{
				if (cons[exDHwL] == 1)
					clroutp(o_EDL);
				extended_door_time = 0;
			}
  	  	}
  	  	else
  	  	{
			if (dpref == 0)
			    clroutp(o_DC);
  	  	  	clroutp(o_NUD);
  	  	  	clroutp(o_DO);
  			nudg_buz = 0;
  	  	  	closetry = 0;
  	  	  	doorf = 0;
  	  	}
  	  	return;
  	}

  	return;
}

//**********************************
// This is the rear door procedure
// *********************************
void rdoors()
{
   	int16 disable_dcb;
	int16 rear_door_closed =0;

	write_trace(TR_RDOORS);

	if (cons[door_type] == 1)
	{
    	if ( ((rdinp(i_SER) == 1) && (rfiref <= 3)) ||   // saftey edge
			 (((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 	    ((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) && (rfiref == 0) && (ree_to == 0)))  	// EER2 input ON
			setoutp(o_RVDR);
		else
			clroutp(o_RVDR);

		if ((rdoutp(o_DOR) == 1) && (rdinp(i_DOLR) == 1))
			setoutp(o_REVR);
		else
			clroutp(o_REVR);
	}

   	if((rccnmsk[cons[carnmb]][position] == 0) && (rucmsk[cons[carnmb]][position] == 0)	 
  		&& (rdcmsk[cons[carnmb]][position] == 0))
	{
		if ((rdoorf == 1) || (rdoorf == 2))
		{
	  	  	if ((rdinp(i_DCLR) == 1) || 
	  	  			( (rdinp(i_DZ) == 0) && ((door_locks() == 0) || (car_gate() == 0)) ) )
				rdoorf = 4;		// nudge closed
			else
				rdoorf = 0;
		}
	}

	if (cons[DZDoorEn] == 2)
	{
		if ((rccnmsk[cons[carnmb]][position] == 1) || 
			(rucnmsk[cons[carnmb]][position] == 1) || 
			(rdcnmsk[cons[carnmb]][position] == 1))
		{	// SPECIAL, for any service if rear side no land exists then nudging close 
			if ((rdinp(i_RDZE) == 0) && ((rdoorf == 1) || (rdoorf == 2) || (rdinp(i_DCLR) != 0)))		// LOST REAR DZ AND DOOR IS OPENNING OR DWELL
			{
		  	  	if ((rdinp(i_DCLR) != 0) || (door_locks() == 0) || (car_gate() == 0))
		  	  	{
		  	  		rdoorf = 4;		// nudge close
		  	  		setoutp(o_PFDCA);
		  	  	}
				else
				{
					clroutp(o_PFDCA);
					rdoorf = 0;
				}
			}
			else
			{
				clroutp(o_PFDCA);
			}
		}
	}


	// Special Automatic Swing door open
	if (cons[Auto_Swing_DO] == 1)
	{
		if ((rdinp(i_DZ) == 1) && (rfiref == 0))
		{
			if ((rsdo == 1) && (rsdo_seq == 0))
				rsdo_seq = 1;

			if (rsdo_seq == 0)
				clroutp(o_RSDO);
			else if (rsdo_seq == 1)
			{
				timers[trsdo] = 0;
				if (preset == 1)
					rsdo_seq = 2;
				clroutp(o_RSDO);
			}
			else if (rsdo_seq == 2)
			{
				if (timers[tfsdo] < 10)
					setoutp(o_RSDO);
				else
				{
					if (rsdo == 0)
						rsdo_seq = 0;
					clroutp(o_RSDO);
				}
			}
			rsdo = 0;
		}
		else
		{
			rsdo = 0;
			rsdo_seq = 0;
			clroutp(o_RSDO);
		}
	}
				
	//********************************************************************
	//	Door_Lock_GS_DPM_Fault mechanism monitors DPM sequence and
	//	DPM, GS or DLT,DLM,DLB and RLM jumped conditions
	//********************************************************************				

	// rear DPM check for timing of rear DPM versus rear GS and Door locks
	if((cons[mand] == 0) && (cons[frghtd] == 0))
	{
		if ((cons[rear] != 0) && (rdinp(i_RPM) == 0) && 
			((door_locks() == 1) || ((rdinp(i_RGS) == 1) && (rdinp(i_RGS_1) == 1))))
		{
			if(timers[trpm] > 3)
			{
				if (Door_DPM_seq_rear_fault == 0)
					record_fault(f_rpm_gl);
				statusf |= sf_GDL;	
				servf = s_GATE_LOCK;
				Door_Lock_GS_DPM_Fault = 1; // dpm seq - rear
				Door_DPM_seq_fault = 1;
				Door_DPM_seq_rear_fault = 1;
			}
		}
		else/* if(Door_DPM_seq_fault == 1)*/
		{
			Door_DPM_seq_rear_fault = 0;
			timers[trpm] = 0;
/*			if(Door_DPM_seq_front_fault == 0)
			{// front without any DPMseq issues
				Door_DPM_seq_fault = 0;
			}*/
		}
	}
//	else {clears Door_DPM_seq_fault, Door_DPM_seq_front_fault, Door_DPM_seq_rear_fault, //  done in doors()}	
				
	// reset Door_Lock_GS_DPM_Fault mechanism
	if((manual_rdoor == 0) || (manual_rdoor == 2))
	{// do in both doors() and rdoors()
		if((Door_DPM_seq_rear_fault == 0) && (Door_DPM_seq_front_fault == 0))
		{
			Door_DPM_seq_fault = 0;
		}
		if((Door_Lock_rear_fault == 0) && (Door_Lock_front_fault == 0))
		{
			Door_Lock_fault = 0;	
		}
		if((Door_GS_rear_fault == 0) && (Door_GS_front_fault == 0))
		{
			Door_GS_fault = 0;	
		}
		if((Door_DPM_rear_fault == 0) && (Door_DPM_front_fault == 0))
		{
			Door_DPM_fault = 0;	
		}
		if (rdoorf != 2)
		{// clear rear failed on conditions
			Door_Lock_rear_fault = 0;
			Door_GS_rear_fault = 0;
			Door_DPM_rear_fault = 0;
		}
	}
	else
	{
		Door_DPM_seq_rear_fault = 0;
		Door_Lock_rear_fault = 0;
		Door_GS_rear_fault = 0;
		Door_DPM_rear_fault = 0;
	}
	//********************************************************************				



  	if(rdoorf == 0)
  	{
		timers[trlant] = 0;
		timers[trdo] = 0;
		if (doorf == 0)
		{
			if (cons[exDHwL] == 1)
				clroutp(o_EDL);
			extended_door_time = 0;
		}
		if (dpref == 0)
		{
			rnudgst = 0;
			timers[trnudge] = 0;
			timers[treeto] = 0;
		}
		clroutp(o_DOR);
		if(((safe() == 1) || (cons[pbdoor] != 0)) && ((dpref == 0) || ((cons[pbdoor] & 2) != 0)))
		{
		    clroutp(o_DCR);
		}
		else if ((rdinp(i_DCLR) == 1) && (rfiref == 0))
		{
			rdoorf = 3;           // go to door close but not if in fire service
		}

		clroutp(o_NUDR);
		freight_rdoor_alarm = 0;
		if ((fvars[fvminshortdten] != 0) && (rfiref == 0))
			short_rdoort_en = 0;
		else
			short_rdoort_en = 1;
		return;
  	}
  	if(rdoorf == 1)        // The rear doors are in openning mode
  	{
		trigger_arm_state |= RDO_START;
		timers[trdcdly] = 0;
		t_door_dwell = rdoort;
		if ((cons[frghtd] == 11) || (cons[frghtd] == 12))
		{
		 	freight_rdoor_alarm = 0;
			if (reset_eer_test == 0)
				reset_eer_test = 1;
		}

		if( ((disable_auto_door == 1) && ((rdinp(i_AD) == 0) || 
			(((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) ||
			(((cons[Galileo] & 1) != 0) && (car_Grp_Stat.c_ui_ad_off == 1)) ) && 
			(rfiref == 0) && (stallf == 0)) || ((manual_rdoor == 1) && (cons[frghtd] == 0)) )
  	  	{
			rdoorf = 0;
			clroutp(o_DCR);
			clroutp(o_NUDR);
			clroutp(o_DOR);
			return;
  	  	}
		rdo_to = 0;
  	  	if(rdinp(i_DOLR) == 1)
  	  	{
  	  	  	clroutp(o_DCR);
  	  	  	clroutp(o_NUDR);
  	  	  	if(timers[trdo] >= fvars[fvdpt])   // door open failure
  	  	  	{
				rdo_to = 1;
  	  	  		rdol_fail = 1;
				rear_slowdown = 0;
				dtmrf = 0;
				record_fault(f_rdoor_open);
				clroutp(o_DOR);
				rdoorf = 3;      // close the doors
				timers[tnit] = 0; 		// reset non-interference timer
				timers[trdc] = 0;
				if ((dpref & 0x03) == 0)
					rdo_seq |= 0x04;
				if ((dpref & 0x01) != 0) 
					rdo_seq |= 0x01;
				if ((dpref & 0x02) != 0)
					rdo_seq |= 0x02;
				return;
  	  	  	}
			if (((cons[frghtd] == 11) || ((cons[frghtd] == 12) && (cons[rear] != 0))) && (rdinp(i_RDSI) == 1))
			    clroutp(o_DOR);
			else if ((cons[door_type] == 1) && ((cons[vshort_tfl] == 0) || ((relevel == 0) && (relevel_req == 0)) || (rdo_seq != 0)) && ((rdinp(i_DZ) == 1) || (rdinp(i_DZA) == 1))) 	// otis ovl door operator
			    setoutp(o_DOR);
  	  	  	else if((timers[trdoor_reopen] >= 3) && ((cons[vshort_tfl] == 0) || ((relevel == 0) && (relevel_req == 0)) || (rdo_seq != 0)) && ((rdinp(i_DZ) == 1) || (rdinp(i_DZA) == 1)) && (timers[trdodly] >= fvars[fvdrdly]))
			    setoutp(o_DOR);
  	  	}
  	  	else
  	  	{
  	  		rdol_fail = 0;
  	  		rdol_fail_cnt = 0;
  	    	t_rdoor_open = timers[trdo];
  	    	clroutp(o_DCR);
  	    	clroutp(o_NUDR);
			if (cons[door_type] != 1)	// leave the door open on for otis ovl door operator
	  	    	clroutp(o_DOR);
  	    	timers[trdwel] = 0;
  	  	  	if ((cons[frghtd] != 0) && (cons[frghtd] <= 5))
				rdoorf = 0;
			else
  	  	  		rdoorf = 2;
			if ((dpref & 0x03) == 0)
				rdo_seq |= 0x04;
			if ((dpref & 0x01) != 0) 
				rdo_seq |= 0x01;
			if ((dpref & 0x02) != 0)
				rdo_seq |= 0x02;
  	  	}
  	  	return;
  	}
  	if(rdoorf == 2)        // The doors are in dwel mode
  	{
		trigger_arm_state |= RDD_START;
		if((manual_rdoor == 1)&&(rfiref == 0))
  	  	{
			rdoorf = 0;
			clroutp(o_DCR);
			clroutp(o_NUDR);
			clroutp(o_DOR);
			return;
  	  	}

		t_rdoor_dwell = rdoort;
		freight_rdoor_alarm = 0;
		if (((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12)) && (fvars[fvdoorbz] != 0))
			timers[trdcdly] = 0;
    	clroutp(o_DCR);
    	clroutp(o_NUDR);
		if ((cons[door_type] == 1) && (rdinp(i_DZ) == 1)) 	// otis ovl door operator
		    setoutp(o_DOR);
		else if (fvars[fvdonodol] == 1)
		{
	  	  	if(rdinp(i_DOLR) == 1)
			    setoutp(o_DOR);
			else
		   		clroutp(o_DOR);
		}
		else
	   		clroutp(o_DOR);

#if ((Traction == 0) && (Tract_OL == 0))
		if ((manual_rdoor == 0 )&& (rdo_to == 0) && (rdinp(i_DOLR) == 0) && (procf != 19))	 // hydro do not check door lock on low pressure switch operation
#else
		if ((manual_rdoor == 0) && (rdo_to == 0) && (rdinp(i_DOLR) == 0))		// automatic door
#endif
		{							   // disable gate and lock test if bit set and new york fire service
			if ((rdinp(i_RGS) == 1) || (rdinp(i_RGS_1) == 1))
	    	{
	    	  	timers[trnudge] = 0;
				timers[treeto] = 0;
	    	  	timers[trdwel] = 0;
	    	  	statusf |= sf_GDL;
				if (Door_GS_rear_fault == 0)
					record_fault(f_rgs_on);
	    	  	servf = s_GATE_LOCK;
	    	  	Door_Lock_GS_DPM_Fault = 1;				// 		rdoors()
	    	  	Door_GS_fault = 1;
	    	  	Door_GS_rear_fault = 1;
	    	  	return;
	    	}

			if ((position == fvars[fvacctopfl]) && ((cons[access_type] == 0) || ((cons[access_type] & 4) != 0)))
			{
				if ((rdinp(i_DLT) == 1) || (rdinp(i_DLT_1) == 1))
			    {
			      	timers[trnudge] = 0;
					timers[treeto] = 0;
			      	timers[trdwel] = 0;
			      	statusf |= sf_GDL;
					if (Door_Lock_rear_fault == 0)
						record_fault(f_rdlt_on);
			      	servf = s_GATE_LOCK;
			      	Door_Lock_GS_DPM_Fault = 1;
			      	Door_Lock_fault = 1;			// 		rdoors()
			      	Door_Lock_rear_fault = 1;
			      	return;
			    }
			}
			else if ((position == fvars[fvaccbotfl]) && ((cons[access_type] == 0) || ((cons[access_type] & 8) != 0)))
			{
				if ((rdinp(i_DLB) == 1) || (rdinp(i_DLB_1) == 1))
		    	{
		    	  	timers[trnudge] = 0;
					timers[treeto] = 0;
		    	  	timers[trdwel] = 0;
		    	  	statusf |= sf_GDL;
					if (Door_Lock_rear_fault == 0)
						record_fault(f_rdlb_on);
		    	  	servf = s_GATE_LOCK;
			      	Door_Lock_GS_DPM_Fault = 1;
			      	Door_Lock_fault = 1;			// 		rdoors()
			      	Door_Lock_rear_fault = 1;
		    	  	return;
		    	}
			}
			else // middle floor
			{
				if ((rdinp(i_RLM) == 1) || (rdinp(i_RLM_1) == 1))
			    {
			      	timers[trnudge] = 0;
					timers[treeto] = 0;
			      	timers[trdwel] = 0;
			      	statusf |= sf_GDL;
					if (Door_Lock_rear_fault == 0)
						record_fault(f_rlm_on);
			      	servf = s_GATE_LOCK;
			      	Door_Lock_GS_DPM_Fault = 1;
			      	Door_Lock_fault = 1;			// 		rdoors()
			      	Door_Lock_rear_fault = 1;
			      	return;
			    }
			}

			if ((cons[mand] == 0) && (cons[frghtd] == 0))
			{
			    if(rdinp(i_RPM) == 1)
			    {
			      	timers[trnudge] = 0;
					timers[treeto] = 0;
				  	if(timers[tdwel] > 100)
				  	{
						if (Door_DPM_rear_fault == 0)
							record_fault(f_rpm_on);
			      	    servf = s_GATE_LOCK;
						Door_Lock_GS_DPM_Fault = 1;
						Door_DPM_fault = 1;			      	    
			      	    Door_DPM_rear_fault = 1;		// 		rdoors()
				  	}
			      	return;
			    }
			}
		}

    	if(Door_Lock_GS_DPM_Fault == 1)		// 		rdoors()
    	{	// clear rear failed on conditions
		  	Door_Lock_rear_fault = 0;
		  	Door_GS_rear_fault = 0;
			Door_DPM_rear_fault = 0;
			//return because it still has issues!
			return;	
    	}

	  	if( ((rdinp(i_CCPBS) == 1) || (c_tmr_svc[s_cc_pb_sec] == 1)) && (cons[cc_pb_sec] == 1) && (Security_code_flag != 0))
			return;

    	if (rclosetry >= 6)
    	{
    	  	servf = s_DC_FAIL;
    	  	if ((rd_both_inps(i_DCBR,i_XDCBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)))
    	  	{
				servf = s_AUTO;
				rclosetry = 0;
    	  	}
    	  	timers[trnudge] = 0;
			timers[treeto] = 0;
    	  	timers[trdwel] = 0;
    	}

		if (fvars[fvdcbdt] == 1)
			disable_dcb = 1;
		else if (((manual_rdoor == 2) && (man_dcc() == 0)) || ((cons[cblue] !=0) && ((codebf == 3) || (codebf == 4))))
			disable_dcb = 1;
		else
			disable_dcb = 0;

      	if ((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12))		// clear nudging timer if rear 
     		timers[trnudge] = 0;							// automatic freight door.

		if ((fvars[fvminshortdten] != 0) && (rfiref == 0))
		{
			if (short_rdoort_en == 0)
			{
				if ((dtmrf & hc_dt) != 0)	// hall call door time was set
				{
					if (timers[trdwel] < fvars[fvhcdt])
						disable_dcb = 1;		// disable DCB and shortenning of doort
					else
						short_rdoort_en = 1;
				}
				else	// otherwise disable DCB only until cc dwell time
				{
					if (timers[trdwel] < fvars[fvccdt])
						disable_dcb = 1;
					else
						short_rdoort_en = 1;
				}
			}
		}
		else
			short_rdoort_en = 1;

    	if ((timers[trdwel] > rdoort) || 
     	    ((disable_dcb == 0) && 
    	    ((rd_both_inps(i_DCBR,i_XDCBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDCBR) == 1)) || 
    	     (((cons[frghtd] == 11) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDCBR) == 1)))) ||
    	    (((timers[trnudge] >= fvars[fvndt]) || (ree_to == 1)) && ((onward_calls != 0) || (fvars[fvnudowc] == 1))) )
    	// door dwell time is up or the door close button was pushed or nudging
    	{
    	  	Update_Job_Statistics();
    	  	if (((timers[trnudge] >= fvars[fvndt]) || (ree_to == 1)) && ((onward_calls != 0) || (fvars[fvnudowc] == 1)))
    	  	{
	  			rnudg_buz = 1;
		  		rdoorf = 4;
    	  	}
    	  	else
    	  	{
	  			rnudg_buz = 0;
				if (((cons[frghtd] == 11) || (cons[frghtd] == 12)) && (cons[eetst_dis] == 0))
				{
					if (freight_eer_test() == 1)
						rdoorf = 3;
					timers[trdodly] = 0;
				}
				else
				{
					rdoorf = 3;        // close the doors					
				}
				if ((cons[frghtd] == 7) || (cons[frghtd] == 8))
				    timers[trdodly] = 0;
    	  	}
    	  	timers[trdc] = 0;
			if (cons[exDHwL] == 1)
				clroutp(o_EDL);
			extended_door_time = 0;
    	  	return;
    	}
    	return;
  	}
  	if(rdoorf == 3)        // The doors are in closeing mode
  	{
		trigger_arm_state |= RDC_START;
		rear_slowdown = 0;
		dtmrf = 0;

		if((cons[frghtd] == 7) || (cons[frghtd] == 8) || (cons[frghtd] == 11) || (cons[frghtd] == 12))
		{
		    if(rdinp(i_DCLR) == 1)
				 timers[trdodly] = 0;			
			if (timers[trdodly] <= 30)
				rear_door_closed = 0;
			else
			{
				rear_door_closed = 1;
				//timers[trdodly] = 0;
			}
		}
		else
		{
		    timers[trdodly] = 0;		
 		    if(rdinp(i_DCLR) == 1)
			     rear_door_closed = 0;
			else
				rear_door_closed = 1;
		}

  	  	if ( (rear_door_closed == 0) || 
  	  		( (rdinp(i_DZ) == 0) && ((door_locks() == 0) || (car_gate() == 0)) ) )
  	  	{
  	    	clroutp(o_DOR);
  	    	if (((rdinp(i_MRSW) == 0) || (FF_STP == 0)) && (rfiref == 0) && (rdinp(i_DZ) == 1))
  	    	{
				clroutp(o_DCR);
				clroutp(o_NUDR);
				clroutp(o_DOR);
	  			rnudg_buz = 0;
				rclosetry = 0;
				return;
  	    	}
  	    	if((timers[trdc] >= fvars[fvdpt]) && (manual_rdoor == 0))  // door close failure
  	    	{
				rclosetry++;
				if(rclosetry >= 6)
				{
					record_fault(f_rdoor_close);
				}
				clroutp(o_DCR);
				clroutp(o_NUDR);
				rdoorf = 1;      // open the doors
				timers[trdo] = 0;
				timers[trnudge] = 0;
				timers[treeto] = 0;
				return;
  	    	}
  	    	if (((timers[trnudge] >= fvars[fvndt]) || (ree_to == 1)) && ((onward_calls != 0) || (fvars[fvnudowc] == 1) || (rdinp(i_DZ) == 0)))
  	    	{
				rdoorf = 4;
				timers[trdc] = 0;
				return;
  	    	}
  	    	if (((rdinp(i_SER) == 1) && (rfiref <= 3)) ||   // saftey edge
			 	(((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 		 ((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) && (rfiref == 0)) ||   // electric eye
				  (((rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
				  ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) ||
				  ((remote_rcc == 1) && (rdinp(i_RM_DOBR) == 1)) ||
				  ((tugf == 1) && (rdinp(i_TGDO) == 1)) || 
				  (((cons[frghtd] == 11) || (cons[frghtd] == 12)) && (servf == s_AUTO) && (rdinp(i_HDOBR) == 1)))
			    	&& (FS_DOB_DIS == 0)) )	// door open button
  	    	{
				clroutp(o_DCR);
				clroutp(o_NUDR);
				timers[trdc] = 0;
				if (rdinp(i_DZ) == 1) 
				{
					rdoorf = 1;      // open the doors
					timers[trdo] = 0;
				}
				return;
  	    	}
  	    	timers[trdoor_reopen] = 0;
  	    	if( (rfiref >= 1) && (rfiref <= 3) && (fs2_to_fs1 == 0) )
			  	setoutp(o_NUDR);
  	    	else
			  	clroutp(o_NUDR);
  			rnudg_buz = 0;
			if (((cons[frghtd] == 7) || (cons[frghtd] == 8)) && (fvars[fvdoorbz] != 0))
			{
				if (timers[trdcdly] >= fvars[fvdoorbz])
				{
		  	    	setoutp(o_DCR);
					freight_rdoor_alarm = 0;
				}
				else
					freight_rdoor_alarm = 1;
			} 
			else if (((cons[frghtd] == 11) || (cons[frghtd] == 12)) && (rfiref == 0) && (fvars[fvdoorbz] != 0))
			{
				if (timers[trdcdly] >= fvars[fvdoorbz])
  	  			{
  	  				if (rdinp(i_RDSI) == 0)
						setoutp(o_DCR);
  	  				else
  	  				{
						timers[trdc] = 0;
  	  					clroutp(o_DCR);
  	  				}
					freight_rdoor_alarm = 0;
				}
				else
					freight_rdoor_alarm = 1;
			}
			else
			{	
				if ((cons[frghtd] == 11) || (cons[frghtd] == 12))
				{
					if (timers[trdcdly] >= fvars[fvdrdly])
					{
	  	  				if (rdinp(i_RDSI) == 0)
							setoutp(o_DCR);
	  	  				else
	  	  				{
							timers[trdc] = 0;
	  	  					clroutp(o_DCR);
	  	  				}
					}
					if (freight_rdoor_alarm != 0)
						freight_rdoor_alarm = 0;
				}
				else if (timers[trdcdly] >= fvars[fvdrdly])
		  	    	setoutp(o_DCR);
			}
			if ((doorf == 0) || (doorf == 3) || (doorf == 4))
			{
				if (cons[exDHwL] == 1)
					clroutp(o_EDL);
				extended_door_time = 0;
			}
  	  	}
  	  	else
  	  	{
  	  	  	t_rdoor_close = timers[trdc];
			if (dpref == 0)
				clroutp(o_DCR);
  	  	  	clroutp(o_NUDR);
  	  	  	clroutp(o_DOR);
  			rnudg_buz = 0;
  	  	  	rclosetry = 0;
  	  	  	rdoorf = 0;
  	  	}
  	  	return;
  	}
  	if(rdoorf == 4)        // The rear doors are in nudgeing close mode
  	{
		trigger_arm_state |= RDC_START;
		if ((dpref & 0x03) == 0)
			rnudgst |= 0x04;
		if ((dpref & 0x01) != 0) 
			rnudgst |= 0x01;
		if ((dpref & 0x02) != 0)
			rnudgst |= 0x02;
		rear_slowdown = 0;
		dtmrf = 0;
		timers[trdodly] = 0;
  	  	if ((rdinp(i_DCLR) == 1) || 
  	  			( (rdinp(i_DZ) == 0) && ((door_locks() == 0) || (car_gate() == 0)) ) )
  	  	{
  	  	  	clroutp(o_DOR);
  	  	  	if((timers[trdc] >= fvars[fvdpt])  && (manual_rdoor == 0))  // door close failure
  	  	  	{
				rclosetry++;
				if(rclosetry >= 6)
				{
					record_fault(f_rdoor_close);
				}
				clroutp(o_DCR);
				clroutp(o_NUDR);
				rdoorf = 1;      // open the doors
				timers[trdo] = 0;
				timers[trdc] = 0;
				return;
  	  	  	}
  	  	  	if ( (((rd_both_inps(i_DOBR,i_XDOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1))) && 	// door open button
  	  	  							(FS_DOB_DIS == 0) && (fvars[fvdobndg] == 1))  || // door open is allowed to reopen door
  	  	  	    	( ((fvars[fvndgctl] & 0x02) != 0) &&
     			 	   ((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 				((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) && (ree_to == 0) && (rfiref == 0)))   // EER2 input ON
  	  	  	{
				clroutp(o_DCR);
				clroutp(o_NUDR);
				timers[trdc] = 0;
	  			rnudg_buz = 1;		// set buzzer on reopen
				if (rdinp(i_DZ) == 1) 
				{
					rdoorf = 1;      // open the doors
					timers[trdo] = 0;
				}
				return;
  	  	  	}
  	  	  	else if ( ((rdinp(i_SER) == 1) && (rfiref <= 3)) ||   // saftey edge
    			 	  (((rdinp(i_EER) == 1) || (rdinp(i_RDET) == 1) ||	// EER1 input ON
		 				 ((eer2_enabled == 1) && (rdinp(i_EER2) == 1))) &&		 // EER2 input ON
		 					(ree_to == 0) && (rfiref == 0) && ((fvars[fvndgctl] & 0x05) != 0) ) ||   // electric eye
				  	  (rd_both_inps(i_DOBR,i_XDOBR) == 1) || 
				  	  ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1)) )                // door open button
  	  	  	{
				clroutp(o_DCR);
				clroutp(o_NUDR);
	  			rnudg_buz = 1;
				timers[trdc] = 0;
				return;
  	  	  	}
  	  	  	timers[trdoor_reopen] = 0;

			if (((fvars[fvndgctl] & 0x01) == 0) || (rfiref != 0) || (ree_to != 0))	// this bit forces the ee to remain active
	  	  	  	setoutp(o_NUDR);
			else
	  	  	  	clroutp(o_NUDR);

			if (((fvars[fvndgctl] & 0x04) == 0) || (rfiref != 0) || (ree_to != 0))
	  			rnudg_buz = 1;
			else
				rnudg_buz = 0;

			if (((cons[frghtd] == 7) || (cons[frghtd] == 8)) && (fvars[fvdoorbz] != 0))
			{
				if (timers[trdcdly] >= fvars[fvdoorbz])
				{
		  	    	setoutp(o_DCR);
					freight_rdoor_alarm = 0;
				}
				else
					freight_rdoor_alarm = 1;
			} 
			else if (((cons[frghtd] == 11) || (cons[frghtd] == 12)) && (rfiref == 0) && (fvars[fvdoorbz] != 0))
			{

				if (timers[trdcdly] >= fvars[fvdoorbz])
  	  			{
  	  				if (rdinp(i_RDSI) == 0)
						setoutp(o_DCR);
  	  				else
  	  				{
						timers[trdc] = 0;
  	  					clroutp(o_DCR);
  	  				}
					freight_rdoor_alarm = 0;
				}
				else
					freight_rdoor_alarm = 1;
			}
			else
			{	
				if ((cons[frghtd] == 11) || (cons[frghtd] == 12))
				{
					if (timers[trdcdly] >= fvars[fvdrdly])
					{
	  	  				if (rdinp(i_RDSI) == 0)
							setoutp(o_DCR);
	  	  				else
	  	  				{
							timers[trdc] = 0;
	  	  					clroutp(o_DCR);
	  	  				}
					}
					if (freight_rdoor_alarm != 0)
						freight_rdoor_alarm = 0;
				}
				else if (timers[trdcdly] >= fvars[fvdrdly])
		  	    	setoutp(o_DCR);
			}

			if ((doorf == 0) || (doorf == 3) || (doorf == 4))
			{
				if (cons[exDHwL] == 1)
					clroutp(o_EDL);
				extended_door_time = 0;
			}
  	  	}
  	  	else
  	  	{
		  	if (dpref == 0)
		  	    clroutp(o_DCR);
  	  	  	clroutp(o_NUDR);
  	  	  	clroutp(o_DOR);
  			rnudg_buz = 0;
  	  	  	rclosetry = 0;
  	  	  	rdoorf = 0;
  	  	}
  	  	return;
  	}
  	return;
}

void peelle_doors()
{

	int hdoor_dis = 0;
	
	if(procf == 17)
	{
		hdoor_dis = 1;
		setoutp(o_HDB);
		clroutp(o_DESL);
	}
	else
		hdoor_dis = 0;

	if (chk_ins() == 1)				 // turn on inspection output for peelle door
		setoutp(o_INSP);
	else
		clroutp(o_INSP); 

	if ((doorf == 1) && (rdinp(i_DZ) == 1))
	{
		if(timers[tpwrdo] >= fvars[fvpwrdotim])
			setoutp(o_ADO);
		else
			clroutp(o_ADO);
	}
	else
	{
		timers[tpwrdo] = 0;
		clroutp(o_ADO);
	}
	
	if (cons[rear] == 1)
	{
		if ((rdoorf == 1) && (rdinp(i_DZ) == 1))
		{
			if(timers[tpwrdor] >= fvars[fvpwrdotim])
				setoutp(o_RADO);
			else
				clroutp(o_RADO);
		}
		else
		{
			timers[tpwrdor] = 0;
			clroutp(o_RADO);
		}

	}

	if  (((firef == 1) || (firef == 2) || (firef == 3)) || 
		((firef < 4) && (empf >= 1) && (empf <= 5)) ) 	// ep recall or control door on empower
	{
		if ((firef == 3) || (empf == ep_home) || (empf == ep_wait_door)) 	 // phase 1 return completed
		{	 // Open the door but door buttons are disabled
			if(hdoor_dis == 0)
				setoutp(o_DESL);
			setoutp(o_PH1R);		 // phase 1 recall
			setoutp(o_HDB);
		}
		else if (empf == ep_home_close)
		{	// Close the door but allow the push button to open
			clroutp(o_DESL);
			clroutp(o_PH1R);
			if(hdoor_dis == 0)
				clroutp(o_HDB);
		}
		else
		{	// Close the door but door buttons are disabled
			setoutp(o_PH1R);		 // phase 1 recall
			clroutp(o_DESL);
			if(hdoor_dis == 0)
				clroutp(o_HDB);
		}
		clroutp(o_PH2HD);			// clear phase 2 i/o's for peelle door
		clroutp(o_PH2ON);
		setoutp(o_PH2OF);			// set phase 2 off output
	}
	else if ((firef == 4) || ((cons[rear] != 0) && (rfiref == 4)))
	{
		setoutp(o_PH2HD);		   // set phase 2 hold output
		clroutp(o_DESL);
		clroutp(o_PH1R);
		clroutp(o_PH2ON);
		clroutp(o_PH2OF);
		if(hdoor_dis == 0)
			clroutp(o_HDB);
	}
	else if ((firef > 4) || ((cons[rear] != 0) && (rfiref > 4)))
	{
		setoutp(o_PH2ON);		   // set phase 2 on output
		clroutp(o_DESL);
		clroutp(o_PH1R);
		clroutp(o_PH2HD);
		clroutp(o_PH2OF);
		if(hdoor_dis == 0)
			clroutp(o_HDB);
	}
	else
	{									  // clear all phase 1 and 2 outputs except for phase 2 off
		if (ind() == 1)
		{
			setoutp(o_DESL);
			setoutp(o_HDB);
			setoutp(o_DAO);
		}
		else
		{
			clroutp(o_DESL);
			if(hdoor_dis == 0)
				clroutp(o_HDB);
			if((cons[att] != 0) && (servf == s_ATT))
				setoutp(o_DAO);
			else
				clroutp(o_DAO);
		}
		clroutp(o_PH1R);
		clroutp(o_PH2HD);
		clroutp(o_PH2ON);
		setoutp(o_PH2OF);
	}

	if (rdoutp(o_RCM) == 1)
	{
		if (timers[trcpkdly] > fvars[fvrcpkdly])
		{
			if (cons[StagRear] == 0)
				clroutp(o_SQUEZ);
			setoutp(o_RC);
		}
		else if (cons[StagRear] == 0)
			setoutp(o_SQUEZ);
	}
	else
	{
		timers[trcpkdly] = 0;
		clroutp(o_RC);
		if (cons[StagRear] == 0)
			clroutp(o_SQUEZ);
	}

	if (cons[StagRear] == 1)
	{
		if (stag_rmsk[position] == 1)
			setoutp(o_RDT);
		else
			clroutp(o_RDT);
	}
}

void auto_peelle_doors()
{
	if (((firef != 0) || (chk_ins() != 0)) && (in_eef_test != 0))
	{
		in_eef_test = 0;
		clroutp(o_FETST);
	}
	
	if (((rfiref != 0) || (chk_ins() != 0)) && (in_eer_test != 0))
	{
		in_eer_test = 0;
		clroutp(o_RETST);
	}

	if (rdoutp(o_RCM) == 1)
	{
		if (timers[trcpkdly] > fvars[fvrcpkdly])
			setoutp(o_RC);
	}
	else
	{
		timers[trcpkdly] = 0;
		clroutp(o_RC);
	}
	
	rdinp(i_FETST);
	rdinp(i_RETST);
}


void courion_doors()
{

	if (((chk_ins() == 1) || (safeties() == 0)) ||
		   ((cons[frghtd] == 5) && (firef == 3)))	// turn on when on inspection or safeties for Courion door
		clroutp(o_CODG);					// or when on phase 1 recall complete for model D operator.
	else
		setoutp(o_CODG); 

	if (reset_courion == 1)
	{
		setoutp(o_MPTR);
		if (timers[tsec] != 0)
		{
			reset_courion = 0;
			clroutp(o_MPTR);			
		}
	}

	if ((ind() == 1) || ((cons[frghtd] == 5) && (firef == 3)) || 
		((rdinp(i_ATT) == 1) && (cons[att] != 0)))
	{
   		clroutp(o_COHS);
		clroutp(o_ATO);
	}
	else
	{
		if ((cons[frghtd] == 5) && (firef == 0))
			setoutp(o_COHS);
		else if (cons[frghtd] == 3)
			setoutp(o_COHS);

		setoutp(o_ATO);
	}

	if ((doorf == 1) && (rdinp(i_DZ) == 1))
		setoutp(o_IAO);
	else
		clroutp(o_IAO);


	if ((firef == 1) || (firef == 2) || (firef == 3))
	{
		if (firef == 3)	 			    	// phase 1 return completed
		{
			clroutp(o_FPH1);
			clroutp(o_ATO);
		}	
		else if (servf != s_HSV)			 //if not in hospital service
			setoutp(o_FPH1);		 // phase 1 recall

		if ((fs2_to_fs1 == 1) && (FS_DOB_DIS == 0))
		{
			courion_fs2_to_fs1 = 1;
			setoutp(o_RAHS);
			clroutp(o_PH2HD);
			clroutp(o_FPH2);
			setoutp(o_PH2HD);
//			setoutp(o_FPH2);
		}
		else
		{
			courion_fs2_to_fs1 = 0;
			clroutp(o_RAHS);
			setoutp(o_PH2HD);			// clear phase 2 i/o's for Courion door
			clroutp(o_FPH2);
		}

	}
	else if (firef == 4)
	{
		clroutp(o_RAHS);
		clroutp(o_PH2HD);		   // set phase 2 hold output
		clroutp(o_FPH1);
		clroutp(o_FPH2);
	}
	else if (firef > 4)
	{
		clroutp(o_RAHS);
		setoutp(o_FPH2);		   // set phase 2 on output
		clroutp(o_FPH1);

		if ((courion_fs2_to_fs1 == 1) && (rdinp(i_DOL) == 1))
			clroutp(o_PH2HD);
		else
		{
			courion_fs2_to_fs1 = 0;
			setoutp(o_PH2HD);
		}
	}
	else
	{									  // clear all phase 1 and 2 outputs except for phase 2 off
		clroutp(o_RAHS);
		clroutp(o_FPH1);
		setoutp(o_PH2HD);
		clroutp(o_FPH2);
		if ((ind() == 0) && ((rdinp(i_ATT) == 0) || (cons[att] == 0)))
			setoutp(o_ATO);
	}

	if (rdoutp(o_RCM) == 1)
	{
		if (timers[trcpkdly] > fvars[fvrcpkdly])
			setoutp(o_RC);
	}
	else
	{
		timers[trcpkdly] = 0;
		clroutp(o_RC);
	}

	if (cons[rear] == 1)
	{
		if ((rdoorf == 1) && (rdinp(i_DZ) == 1))
			setoutp(o_RADO);
		else
			clroutp(o_RADO);

	}

}
void ems_doors()
{
			
	if ((chk_ins() == 1) || (safeties() == 0))			 // turn off when on inspection or safeties for Courion door
		clroutp(o_DSC);
	else
		setoutp(o_DSC);

	if ((doorf == 1) && (rdinp(i_DZ) == 1))					  // open the front doors
		setoutp(o_FDOB);
	else
		clroutp(o_FDOB);

	if ((rdoorf == 1) && (rdinp(i_DZ) == 1))				   // open the rear doors
		setoutp(o_RDOB);
	else
		clroutp(o_RDOB);

	if (ind() == 1)
   		clroutp(o_TC);
	else
		setoutp(o_TC);

	if (rdoutp(o_RCM) == 1)
	{
		if (timers[trcpkdly] > fvars[fvrcpkdly])
			setoutp(o_RC);
	}
	else
	{
		timers[trcpkdly] = 0;
		clroutp(o_RC);
	}

	if (((firef == 1) || (firef == 2) || (firef == 3)) && ((eqf == 0) || (cons[equake] == 0)))
	{							   // fire phase 1
		setoutp(o_FRCL);
		setoutp(o_HCO);	  // Disable always
		if (firef == 3)
			setoutp(o_FH);
		else
			clroutp(o_FH);
		clroutp(o_FPS2);

		if (rdoutp(o_FSTP) == 1)
			setoutp(o_EFST);
		else
			clroutp(o_EFST);
	}
	else if (firef == 4)	   // phase 2 door hold
	{
		setoutp(o_FRCL);
		setoutp(o_HCO);	  // Disable always
		setoutp(o_FPS2);
		setoutp(o_FH);	   // At return floor and fire hold turn on fh.	
		clroutp(o_EFST);
	}
	else if (firef > 4)		// phase 2 except door hold
	{
		setoutp(o_FRCL);
		setoutp(o_HCO);	  // Disable always
		setoutp(o_FPS2);
		clroutp(o_FH);
		clroutp(o_EFST);
	}
	else	// not on fire service
	{
		clroutp(o_FPS2);
		clroutp(o_FRCL);
		clroutp(o_HCO);
		clroutp(o_EFST);
		if ((cons[equake] != 0) && (eqf == 4))
			setoutp(o_FH);	   // open door on earthquake	
		else
			clroutp(o_FH);
	}		


	if ((rdinp(i_DZ) == 1) && (rdoutp(o_RC) == 0))
	{								 // if rc dropped and on dz
		if (position == 1) 			 // turn appropriate output for floor zoning when car is at a floor
			setoutp(o_EMS1Z);		 
		else
			clroutp(o_EMS1Z);

		if (position == 2)
			setoutp(o_EMS2Z);
		else
			clroutp(o_EMS2Z);

		if (position == 3)
			setoutp(o_EMS3Z);
		else
			clroutp(o_EMS3Z);

		if (position == 4)
			setoutp(o_EMS4Z);
		else
			clroutp(o_EMS4Z);

		if (position == 5)
			setoutp(o_EMS5Z);
		else
			clroutp(o_EMS5Z);

		if (position == 6)
			setoutp(o_EMS6Z);
		else
			clroutp(o_EMS6Z);

		if (position == 7)
			setoutp(o_EMS7Z);
		else
			clroutp(o_EMS7Z);
		
		if (position == 8)
			setoutp(o_EMS8Z);
		else
			clroutp(o_EMS8Z);

		if (position == 9)
			setoutp(o_EMS9Z);
		else
			clroutp(o_EMS9Z);

		if (position == 10)
			setoutp(o_EMS10Z);
		else
			clroutp(o_EMS10Z);

		if (position == 11)
			setoutp(o_EMS11Z);
		else
			clroutp(o_EMS11Z);
	}
	else
	{
		clroutp(o_EMS1Z);
		clroutp(o_EMS2Z);
		clroutp(o_EMS3Z);
		clroutp(o_EMS4Z);
		clroutp(o_EMS5Z);
		clroutp(o_EMS6Z);
		clroutp(o_EMS7Z);
		clroutp(o_EMS8Z);
		clroutp(o_EMS9Z);
		clroutp(o_EMS10Z);
		clroutp(o_EMS11Z);
	}
}

void auto_freight_doors()
{
	if (rdoutp(o_RCM) == 1)
	{
		if (timers[trcpkdly] > fvars[fvrcpkdly])
			setoutp(o_RC);
	}
	else
	{
		timers[trcpkdly] = 0;
		clroutp(o_RC);
	}

	if ((rdinp(i_DZ) == 1) && (rdoutp(o_RC) == 0))
	{								 // if rc dropped and on dz
		if (position == 1) 			 // turn appropriate output for floor zoning when car is at a floor
			setoutp(o_FRT1Z);		 
		else
			clroutp(o_FRT1Z);

		if (position == 2)
			setoutp(o_FRT2Z);
		else
			clroutp(o_FRT2Z);

		if (position == 3)
			setoutp(o_FRT3Z);
		else
			clroutp(o_FRT3Z);

	}
	else
	{
		clroutp(o_FRT1Z);
		clroutp(o_FRT2Z);
		clroutp(o_FRT3Z);
	}

	if (rdoutp(o_DO) == 1) 
	{
		if (timers[tpwrdo] < fvars[fvpwrdotim])
			setoutp(o_PDO);
		else
			clroutp(o_PDO);
	}
	else
		timers[tpwrdo] = 0;

	if (rdoutp(o_DOR) == 1)
	{
		if (timers[tpwrdor] < fvars[fvpwrdotim])
			setoutp(o_PDOR);
		else
			clroutp(o_PDOR);
	}
	else
		timers[tpwrdor] = 0;
}

void guilbert_doors()
{

	if (chk_ins() == 1)				 // turn on inspection output for peelle door
		setoutp(o_G_INSP);
	else
		clroutp(o_G_INSP); 

	if ((doorf == 1) && (rdinp(i_DZ) == 1))
		setoutp(o_G_ADO);
	else
		clroutp(o_G_ADO);
			
	if(rd_both_inps(i_DCB,i_XDCB) == 1)
		setoutp(o_G_FDCBO);
	else
		clroutp(o_G_FDCBO);

	if(rd_both_inps(i_DCBR,i_XDCBR) == 0)	
		setoutp(o_G_RDCBO);
	else
		clroutp(o_G_RDCBO);

	if ((doorf == 2) && (rdinp(i_DZ) == 1))
		setoutp(o_G_ADOH);
	else
		clroutp(o_G_ADOH);

	if (cons[rear] == 1)
	{
		if ((rdoorf == 1) && (rdinp(i_DZ) == 1))
			setoutp(o_G_RDO);
		else
			clroutp(o_G_RDO);

		if ((rdoorf == 2) && (rdinp(i_DZ) == 1))
			setoutp(o_G_RDOH);
		else
			clroutp(o_G_RDOH);
	}

	if (rdoutp(o_RCM) == 1)
	{
		if (timers[trcpkdly] > fvars[fvrcpkdly])
			setoutp(o_G_RC);
	}
	else
	{
		timers[trcpkdly] = 0;
		clroutp(o_G_RC);
	}

	if(rdinp(i_READY) == 0)
		setoutp(o_G_SOPEN);
	else
		clroutp(o_G_SOPEN);


	if  (((firef == 1) || (firef == 2) || (firef == 3)) || 
		((firef < 4) && (empf >= 1) && (empf <= 5)) ) 	// ep recall or control door on empower
	{
		if ((firef == 3) || (empf == ep_home) || (empf == ep_wait_door)) 	 // phase 1 return completed
		{	 // Open the door but door buttons are disabled
			setoutp(o_G_ININD);
			setoutp(o_G_FH1R);		 // phase 1 recall
			setoutp(o_G_ADOH);
		}
		else if (empf == ep_home_close)
		{	// Close the door but allow the push button to open
			clroutp(o_G_ININD);
			clroutp(o_G_FH1R);
			clroutp(o_G_ADOH);
		}
		else
		{	// Close the door but door buttons are disabled
			setoutp(o_G_FH1R);		 // phase 1 recall
			clroutp(o_G_ININD);
			clroutp(o_G_ADOH);
		}
		clroutp(o_G_PH2HD);			// clear phase 2 i/o's for peelle door
		clroutp(o_G_PH2ON);
		setoutp(o_G_PH2OF);			// set phase 2 off output
	}
	else if ((firef == 4) || ((cons[rear] != 0) && (rfiref == 4)))
	{
		setoutp(o_G_PH2HD);		   // set phase 2 hold output
		clroutp(o_G_ININD);
		clroutp(o_G_FH1R);
		clroutp(o_G_PH2ON);
		clroutp(o_G_PH2OF);
		clroutp(o_G_ADOH);
	}
	else if ((firef > 4) || ((cons[rear] != 0) && (rfiref > 4)))
	{
		setoutp(o_G_PH2ON);		   // set phase 2 on output
		clroutp(o_G_ININD);
		clroutp(o_G_FH1R);
		clroutp(o_G_PH2HD);
		clroutp(o_G_PH2OF);
		clroutp(o_G_ADOH);
	}
	else
	{									  // clear all phase 1 and 2 outputs except for phase 2 off
		if (ind() == 1)
		{
			setoutp(o_G_ININD);
			setoutp(o_G_ADOH);
		}
		else
		{
			clroutp(o_G_ININD);
			clroutp(o_G_ADOH);
		}
		clroutp(o_G_FH1R);
		clroutp(o_G_PH2HD);
		clroutp(o_G_PH2ON);
		setoutp(o_G_PH2OF);
	}

}


// Sabbath Service Operation routine

int16 sabbath_svc(void)
{

	if (servf == s_SABBATH)
	{

		if (((fvars[fvsabben] & 0x02) != 0) && (position == lobby_fl))
		{
			doort = fvars[fvexdt];
			rdoort = fvars[fvexdt];
		}
		else
		{
			doort = fvars[fvsadt];
			rdoort = fvars[fvsadt];
		}

		if ((timers[tdwel] + fvars[fvsabuzt] > doort) || ((cons[rear] == 1) &&(timers[tdwel] + fvars[fvsabuzt] > rdoort)))
		{
			sabb_buz = 1;
		}
		else
		{
			sabb_buz = 0;
		}
		if ((timers[tdwel] + 10 > doort) || ((cons[rear] == 1) &&(timers[tdwel] + 10 > rdoort)))
		{
			sabb_ee_en = 1;
		}
		else
		{
			sabb_ee_en = 0;		
		}

	}
	else
	{
		sabb_buz = 0;
		sabb_ee_en = 0;
	}

	if (((cons[Sabbath] == 1) || (cons[Sabbath] == 2)) && (sabbath_active == 1))
	{
		if ((sabbath_start == 1) || ((fvars[fvsabben2] & 0x01) == 0))
	 		sabbath_std_svc();
		else
			sabbath_start_at_lobby();
	
		if(cons[Super_Sabbath] == 1)	 
		{//This will enable Display for Sabbath (and no hall calls)
			servf = s_SABBATH;
			return(0);
		}
		
	}
	else if ( (cons[Sabbath] == 4) && (sabbath_active == 1) )
	{
		if ((sabbath_start == 1) || ((fvars[fvsabben2] & 0x01) == 0))
		{
			servf = s_SABBATH;
			if(fvars[fvsabcolmode] == 0)
				sabbath_down_col_svc();
			else
				sabbath_up_col_svc();
		}
		else
			sabbath_start_at_lobby();
		return(0);
	 }
	else if ( (cons[Sabbath] == 5) && (sabbath_active == 1) )
	{
		if ((car_Grp_Stat.c_ir_active == 1) && ((fvars[fvsabben] & 0x01) != 0))
		{			 // allow momentary ir to override sabbath operation
			if (servf == s_SABBATH)
			{
				servf = s_AUTO;
				clrcc(0);
			}
		}
		else
		{
			if ((sabbath_start == 1) || ((fvars[fvsabben2] & 0x01) == 0))
			{
				servf = s_SABBATH;
				sabbath_odd_even_svc();
			}
			else
				sabbath_start_at_lobby();
		}
		return(0);
	}
	else if ((cons[Sabbath] == 6) && (sabbath_active == 1))
	{
		sab_up_dn_col_cc_msk_swap();
		return(0);
	}
	else if ((cons[Sabbath] == 7) && ((sabbath_active == 1) || (sabbath_auto_calls != 0)))  
	{
		servf = s_SABBATH;
		sabbath_tables_cc();
		return(0);
	}
	else
	{
		sabbath_rst_msk = 0;
		sabbath_start = 0;
		if (servf == s_SABBATH)
	 	{
	 		servf = s_AUTO;
	 		clrcc(0);
	 	}
	}


	return(1);

}

// ********************************************************************************
// Front electric edge test procedure routine 
// Returns 1 if successfully passed electric edge test or on fire service or on ins,
// otherwise, returns 0.
// The EE test is to verify if the front edge inputs (EE and EE2) pulse
// while the front edge test (FETST) output is ON.
// ********************************************************************************

int16 freight_eef_test(void)
{
	static int16 eef_test_success = 0;		// flag sets if front both EE and EE2 inputs successfully passed the electric edge test
	static int16 eef1_TEST_state = 0;		// flag to verify 3 stages of EE input for electric edge test
	static int16 eef2_TEST_state = 0;		// flag to verify 3 stages of EE2 input for electric edge test

	if ((reset_eef_test == 1) && (eef_testtry < 6))
	{	// reset all electric edge test variables
		reset_eef_test = 0;
		in_eef_test = 0;
		eef_testtry = 0;
		eef1_TEST_state = 0;
		eef2_TEST_state = 0;
		eef_test_success = 0;
		timers[teeftest] = 0;
		clroutp(o_FETST);
	}

	// test success or not necessary, don't go further
	if ((eef_test_success == 1) || (servf != s_AUTO))
	{
		in_eef_test = 0;
		eef_testtry = 0;
		clroutp(o_FETST);
		return(1);
	}

	// eef test failed too many times
	if (eef_testtry >= 6)
		return(0);

	// both ee1 and ee2 have to be in the off state to begin ee test
	if (in_eef_test == 0)
	{
		if (rdinp(i_FETST) == 0)
		{
			if ((rdinp(i_EE) == 0) && (rdinp(i_EE2) == 0))	// initially both EE and EE2 must be in OFF state
			{
				in_eef_test = 1;
				setoutp(o_FETST);		// provide FETST output
				eef1_TEST_state = 0;
				eef2_TEST_state = 0;
				timers[teeftest] = 0;	// reset test timer
			}
		}
		else
		{
			clroutp(o_FETST);	// clear output
			if (timers[teeftest] > 2)
			{
				eef_testtry++;
				timers[teeftest] = 0;
			}
			if (eef_testtry >= 6)
				record_fault(f_FETST_ON_fault);
		}
	}
	else	// when in eef test
	{
		if (timers[teeftest] < fvars[fveetesttm])
		{
			setoutp(o_FETST);	// continue setting FETST output

			if (rdoutp(o_FETST) != 0)		// when test output turns ON, expect EE and EE2 to come ON and then OFF independently
			{
				switch (eef1_TEST_state)
				{
					case 0:		// verify ee input turning ON
						if (rdinp(i_EE) == 1)
							eef1_TEST_state = 1;
						break;

					case 1:		// verify ee input turning OFF
						if (rdinp(i_EE) == 0)
							eef1_TEST_state = 2;		// passing state
						break;
					
					default:
						break;
				}

				switch (eef2_TEST_state)
				{
					case 0:		// verify ee2 input turning ON
						if (rdinp(i_EE2) == 1)
							eef2_TEST_state = 1;
						break;

					case 1:		// verify ee2 input turning OFF
						if (rdinp(i_EE2) == 0)
							eef2_TEST_state = 2;	// passing state
						break;

					default:
						break;
				}

				// test success both ee and ee2 pulsed
				if ((eef1_TEST_state == 2) && (eef2_TEST_state == 2))
				{
					clroutp(o_FETST);
					eef_test_success = 1;
				}

			}
			else	// FETST output did not turn ON fault
			{
				if (timers[teeftest] != 0)	// did not turn ON at least for 100ms
				{
					in_eef_test = 0;
					timers[teeftest] = 0;
					clroutp(o_FETST);
					eef_testtry++;		// increment test tries
					if (eef_testtry >= 6)
						record_fault(f_FETST_OFF_fault);
				}
			}
		}
		else	// when the test time has expired
		{
			in_eef_test = 0;
			timers[teeftest] = 0;
			clroutp(o_FETST);
			eef_testtry++;		// increment test tries
			
			if (eef_testtry >= 6)	// fail count exceeded limit
			{
				if (eef1_TEST_state != 2)	// EE1 input did not pass the test
				{
					if (eef1_TEST_state == 0)				// EE1 failed to turn ON
						record_fault(f_EE1_Failed_ON);
					else if (eef1_TEST_state == 1)			// EE1 failed to turn OFF
						record_fault(f_EE1_Failed_OFF);
				}

				if (eef2_TEST_state != 2)	// EE2 input did not pass the test
				{
					if (eef2_TEST_state == 0)				// EE2 failed to turn ON
						record_fault(f_EE2_Failed_ON);
					else if (eef2_TEST_state == 1)			// EE2 failed to turn OFF
						record_fault(f_EE2_Failed_OFF);
				}
			}
		}
	}
	return(0);
}


// ********************************************************************************
// Rear electric edge test procedure routine 
// Returns 1 if successfully passed electric edge test or on fire service or on ins,
// otherwise, returns 0.
// The rear edge test is to verify if the rear edge inputs (EER and EER2) pulse
// while the rear edge test (RETST) output is ON.
// ********************************************************************************

int16 freight_eer_test(void)
{
	static int16 eer_test_success = 0;		// flag sets if rear both EER and EER2 inputs successfully passed the electric edge test
	static int16 eer1_TEST_state = 0;		// flag to verify 3 stages of EER input for electric edge test
	static int16 eer2_TEST_state = 0;		// flag to verify 3 stages of EER2 input for electric edge test

	if ((reset_eer_test == 1) && (eer_testtry < 6))
	{	// reset all electric edge test variables
		reset_eer_test = 0;
		in_eer_test = 0;
		eer_testtry = 0;
		eer1_TEST_state = 0;
		eer2_TEST_state = 0;
		eer_test_success = 0;
		timers[teertest] = 0;
		clroutp(o_RETST);
	}

	// test success or not necessary, don't go further
	if ((eer_test_success == 1) || (servf != s_AUTO))
	{
		in_eer_test = 0;
		eer_testtry = 0;
		clroutp(o_RETST);
		return(1);
	}

	// eer test failed too many times
	if (eer_testtry >= 6)
		return(0);

	// both eer1 and eer2 have to be in the off state to begin eer test
	if (in_eer_test == 0)
	{
		if (rdinp(i_RETST) == 0)
		{
			if ((rdinp(i_EER) == 0) && (rdinp(i_EER2) == 0))	// initially both EER and EER2 must be in OFF state
			{
				in_eer_test = 1;
				setoutp(o_RETST);		// provide RETST output
				eer1_TEST_state = 0;
				eer2_TEST_state = 0;
				timers[teertest] = 0;	// reset test timer
			}
		}
		else
		{
			clroutp(o_RETST);	// clear output
			if (timers[teertest] > 2)
			{
				eer_testtry++;
				timers[teertest] = 0;
			}
			if (eer_testtry >= 6)
				record_fault(f_RETST_ON_fault);
		}
	}
	else	// when in eer test
	{
		if (timers[teertest] < fvars[fveetesttm])
		{
			setoutp(o_RETST);	// continue setting RETST output

			if (rdoutp(o_RETST) != 0)		// when test output turns ON, expect EER and EER2 to come ON and then OFF independently
			{
				switch (eer1_TEST_state)
				{
					case 0:		// verify eer input turning ON
						if (rdinp(i_EER) == 1)
							eer1_TEST_state = 1;
						break;

					case 1:		// verify eer input turning OFF
						if (rdinp(i_EER) == 0)
							eer1_TEST_state = 2;		// passing state
						break;
					
					default:
						break;
				}

				switch (eer2_TEST_state)
				{
					case 0:		// verify eer2 input turning ON
						if (rdinp(i_EER2) == 1)
							eer2_TEST_state = 1;
						break;

					case 1:		// verify eer2 input turning OFF
						if (rdinp(i_EER2) == 0)
							eer2_TEST_state = 2;	// passing state
						break;

					default:
						break;
				}

				// test success both eer and eer2 pulsed
				if ((eer1_TEST_state == 2) && (eer2_TEST_state == 2))
				{
					clroutp(o_RETST);
					eer_test_success = 1;
				}
			}
			else	// RETST output did not turn ON fault
			{
				if (timers[teertest] != 0)	// did not turn ON at least for 100ms
				{
					in_eer_test = 0;
					timers[teertest] = 0;
					clroutp(o_RETST);
					eer_testtry++;		// increment test tries
					if (eer_testtry >= 6)
						record_fault(f_RETST_OFF_fault);
				}
			}
		}
		else	// when the test time has expired
		{
			in_eer_test = 0;
			timers[teertest] = 0;
			clroutp(o_RETST);
			eer_testtry++;		// increment test tries
			
			if (eer_testtry >= 6)	// fail count exceeded limit
			{
				if (eer1_TEST_state != 2)	// EER1 input did not pass the test
				{
					if (eer1_TEST_state == 0)				// EER1 failed to turn ON
						record_fault(f_EER1_Failed_ON);
					else if (eer1_TEST_state == 1)			// EER1 failed to turn OFF
						record_fault(f_EER1_Failed_OFF);
				}

				if (eer2_TEST_state != 2)	// EER2 input did not pass the test
				{
					if (eer2_TEST_state == 0)				// EER2 failed to turn ON
						record_fault(f_EER2_Failed_ON);
					else if (eer2_TEST_state == 1)			// EER2 failed to turn OFF
						record_fault(f_EER2_Failed_OFF);
				}
			}
		}
	}
	return(0);
}

//************************************************ 
//*** Auto swing doors output per floor   ********
//************************************************ 

void auto_swg_doors(void)
{
	// on automatic service, door in openning or dwelling state
	if ((servf == s_AUTO) && ((doorf == 1) || (doorf == 2)) && (rdinp(i_DZ) == 1))
	{
		if (doorf == 1)		// doorf = 1 cab doors openning first time
		{
			if (timers[tpwrdo] >= fvars[fvautoswgdodly])	// first time open ADO output provided with ajustable delay
			{
				setoutp(o_ADOF(position));
				if (swdorf == 0)
					swdorf = 1;
			}
			timers[tpwrdwel] = 0;
		}
		else if (doorf == 2)	// doorf = 2 cab doors are dwelling
		{
			if (swdorf == 0)		// swing doors closed
			{
				if ((((c_up_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 	// up hc
					((c_dn_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 	// dn hc
					(rdinp(i_CC(position)) != 0)) || (rdinp(i_DOB) != 0) ||			// cc or DOB button pressed
					(man_fdcc() == 0))		// hall door contacts are not made
					swdorf = 1;
				clroutp(o_ADOF(position));
				timers[tpwrdwel] = 0;
				timers[tpwrdo] = 0;
			}
			else if (swdorf == 1)
			{
				if (timers[tpwrdo] <= 20) 		// assume door fully opens in 2 seconds
					setoutp(o_ADOF(position));
				else
					swdorf = 2;
				
				timers[tpwrdwel] = 0;
			}
			else if (swdorf == 2)		// swing doors ADO output is kept ON to keep the door open for dwelling
			{
				// reset dwell time in case hc or cc is pressed 
				if (((c_up_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 
					((c_dn_hall_call[(position-1)/32] & fl_mask[position]) != 0) ||
					(rdinp(i_CC(position)) != 0))
					timers[tpwrdwel] = 0;
				
				if ((timers[tpwrdwel] < fvars[fvhcdt]) || (rdinp(i_DOB) != 0) || (rdinp(i_EE) != 0))
					setoutp(o_ADOF(position));
				else 
					swdorf = 3;
			}
			else if (swdorf == 3)		// doors closing, ADO output cleared to close the swing doors (mechanically close by it self)
			{
				if ((((c_up_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 	// up hc
					((c_dn_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 	// dn hc
					(rdinp(i_CC(position)) != 0)) || (rdinp(i_DOB) != 0))
				{
					swdorf = 1;
					timers[tpwrdo] = 1;
				}
					
				if (timers[tpwrdwel] < fvars[fvdpt])
					clroutp(o_ADOF(position));
				else
				{
					if (man_fdcc() != 0)
						swdorf = 0;
					else		// doors didn't close for
					{
						swdorf = 1;
						fado_fault_cnt++;			// increment fault count
						timers[tpwrdo] = 0;
						timers[tpwrdwel] = 0;
						clroutp(o_ADOF(position));		// clr ADO output
					}
					if (fado_fault_cnt >= 3)
					{
						shutdown = 1;		// shut down car
						fault_device = position;
						record_fault(f_swg_fhdoor_open);
					}
				}
			}
		}
	}
	else
	{
		swdorf = 0;
		timers[tpwrdo] = 0;
		clroutp(o_ADOF(position));
	}

	if (cons[rear] != 0)
	{
		// on automatic service, door in openning or dwelling state
		if ((servf == s_AUTO) && ((rdoorf == 1) || (rdoorf == 2)) && (rdinp(i_DZ) == 1))
		{
			if (rdoorf == 1)		// rdoorf = 1 cab doors openning first time
			{
				if (timers[tpwrdor] >= fvars[fvautoswgdodly])	// first time open ADO output provided with ajustable delay
				{
					setoutp(o_ADOR(position));
					if (rswdorf == 0)
						rswdorf = 1;
				}
				timers[tpwrdwelr] = 0;
			}
			else if (rdoorf == 2)	// rdoorf = 2 cab doors are dwelling
			{
				if (rswdorf == 0)		// swing doors closed
				{
					if ((((c_upr_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 	// up hc
						((c_dnr_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 	// dn hc
						(rdinp(i_CCR(position)) != 0)) || (rdinp(i_DOBR) != 0) ||			// cc or DOB button pressed
						(man_rdcc() == 0))		// hall door contacts are not made
						rswdorf = 1;
					clroutp(o_ADOR(position));
					timers[tpwrdwelr] = 0;
					timers[tpwrdor] = 0;
				}
				else if (rswdorf == 1)			
				{
					if (timers[tpwrdor] <= 20) 		// assume door fully opens in 2 seconds
						setoutp(o_ADOR(position));
					else
						rswdorf = 2;
					
					timers[tpwrdwelr] = 0;
				}
				else if (rswdorf == 2)		// swing doors ADO output is kept ON to keep the door open for dwelling
				{
					// reset dwell time in case hc or cc is pressed 
					if (((c_upr_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 
						((c_dnr_hall_call[(position-1)/32] & fl_mask[position]) != 0) ||
						(rdinp(i_CCR(position)) != 0))
						timers[tpwrdwelr] = 0;
					
					if ((timers[tpwrdwelr] < fvars[fvhcdt]) || (rdinp(i_DOBR) != 0) || (rdinp(i_EER) != 0))
						setoutp(o_ADOR(position));
					else 
						rswdorf = 3;
				}
				else if (rswdorf == 3)		// doors closing, ADO output cleared to close the swing doors (mechanically close by it self)
				{
					if ((((c_upr_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 	// up hc
						((c_dnr_hall_call[(position-1)/32] & fl_mask[position]) != 0) || 	// dn hc
						(rdinp(i_CCR(position)) != 0)) || (rdinp(i_DOBR) != 0))			// cc or DOB button pressed
					{
						rswdorf = 1;
						timers[tpwrdor] = 0;
					}
					
					if (timers[tpwrdwelr] < fvars[fvdpt])
						clroutp(o_ADOR(position));
					else
					{
						if (man_rdcc() != 0)
							rswdorf = 0;
						else		// doors didn't close for
						{
							rswdorf = 1;
							rado_fault_cnt++;			// increment fault count
							timers[tpwrdor] = 0;
							timers[tpwrdwelr] = 0;
							clroutp(o_ADOR(position));		// clr ADO output
						}
						if (rado_fault_cnt >= 3)
						{
							shutdown = 1;		// shut down car
							record_fault(f_swg_rhdoor_open);
						}
					}
				}
			}
		}
		else
		{
			rswdorf = 0;
			timers[tpwrdor] = 0;
			clroutp(o_ADOR(position));
		}
	}
}

void lockgsdpm_fault_doorop(void) 
{
	if(ccnmsk[cons[carnmb]][position] != 0)
	{
		if(lock_gs_dpm_flt_front() != 0)
		{ // fault condition - keep doors open
			if (rdinp(i_DOL) == 1)
			{
				if ((doorf != 1) || (doorf != 2))
					doorf = 1;
			}
			else
			{
				doorf = 2;
			}	
			chkdoor_state = 22;
		}
		else if((manual_door == 0) || (manual_door == 2))
		{ // no fault condition but if doors open then keep them open
			if(doorf != 0)
			{
				if (rdinp(i_DOL) == 1)
				{
					if ((doorf != 1) || (doorf != 2))
						doorf = 1;
				}
				else
				{
					doorf = 2;
				}					
			}
			else if(((rdinp(i_DOB) == 1)  || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOB) == 1))) &&
				 (FS_DOB_DIS == 0))
			{
				doorf = 1;    // open door
			}
		}
	}	
	if ((cons[rear] != 0) && (rccnmsk[cons[carnmb]][position] != 0))
	{
		if(lock_gs_dpm_flt_rear() != 0)
		{
			if (rdinp(i_DOLR) == 1)
			{
				if ((rdoorf != 1) || (rdoorf != 2))
					rdoorf = 1;
			}
			else
			{
			    rdoorf = 2;
			}
			chkdoor_state = 23;
		}
		else if((manual_rdoor == 0) || (manual_rdoor == 2))
		{// no fault condition but if doors open then keep them open
			if(rdoorf != 0)
			{
				if (rdinp(i_DOLR) == 1)
				{
					if ((rdoorf != 1) || (rdoorf != 2))
						rdoorf = 1;
				}
				else
				{
				    rdoorf = 2;
				}
			}
			else if(((rdinp(i_DOBR) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiDOBR) == 1))) &&
					(FS_DOB_DIS == 0))
			{
				rdoorf = 1;    // open rear door
			}
		}
	}
}



/* Revision History	
	
2/18/04 v4.00 mhd	1. Brought subroutines from control.cpp.
					2. Added Medical emergency service
5/13/04 v4.00 mhd	1. Allow doors to close full speed if on fire phase 2 to phase 1 return.
6/9/04 v4.01 mhd	1. Reset non-interference timer if door open timeout to preven an immediate start.
7/1/04 mhd			2. Use in_att flag to reset attendant door timer when taken off of attendant.
9/2/04 v4.03 mhd	1. On hydro car, make sure that doors are opened and call cancelled when there is an 
					   i/o error ((statusf & sf_IO) != 0).
9/20/04 v4.03 mhd	1. Added changes for Chicago fire service (see cons[firesv] != 3).
11/2/04 v4.03 mhd	2. Added Special out of service from RTL switch.
					3. Clear front and rear slowdown flag at door dwell.
12/5/04 v4.03 mhd	1. Added cons[access_type] to verify the locks correctly at the top or bottom.
					2. Check for Door_Lock_Fault == 0 to allow the door to nudge closed.
1/3/04 v4.04 mhd	1. Prevent car from running with Safety processor board can error.
1/27/04				2. Make fix for Chicago fire service in (fire == 5) and (fire == 6) routines.
2/9/05 v4.07 mhd	1. Added load overload, load dispatch and load bypass for analog load weigher.
					2. Check for servf == s_LW_CAL to allow servf == s_AUTO.
3/22/05 v4.08 mhd	1. Check for servf != s_LBY_RCL to allow servf = s_AUTO in chkdoor.
4/17/05 v4.10 mhd	1. Donot do DPM door lock fault check if manual doors.
4/18/05 v4.11 mhd	1. Delete using timers[tnit] for door timer	when cons[mand] == 2.  Use car call dwell timer control.cpp.
4/21/05 v4.12 mhd	1. Added manual_door to set dynamically with automatic front and staggered rear.
					2. Added Sabbath service.
5/12/05 v4.18 mhd	1. Added man_dcc() routine to check door close contacts for manual doors.
					2. Do not allow a door close fault if manual door. 
					3. Added check for GTS input on.
6/2/05 v4.19 mhd	1. Use seperate RPM input when rear doors instead of DPM.  Also seperate the front and rear gate switches
					   when checking FOR dpm AND rpm for zero in chkdoor.
6/15/05 v4.21 mhd	1. Added debounce timer for dpm and rpm to check for GS and RGS.
7/15/05 V4.25 mhd	1. In door dwell state, check dwell timer to be > doort instead of >= doort.  If doort is zero and the
					   dwell timer is continuously set to zero, the door flag will not be set to (3) door close.
					2. Correct rear door operation for independent.
8/24/05 V4.29 mhd	1. Cancel car calls on attendant from cancel button(FS2C) in COP.
9/7/05 v4.20 mhd	1. Sound nudging buzzer if on attendant and hall call is not answered within the att buzzer delay time.
9/14/05 V4.32 mhd	1. Added door ovl door operator operation. see cons[door_type] == 1
9/21/05 v4.33 pn	1. Allow front or rear doors to operate as long as the car call, up call and down call mask
					   are not set to zero.
9/23/05 v4.34 pn	1. Added freight door option 2 for Peelle door interface.
10/5/05 v4.35 pn	1. When on code blue at the code blue floor disable the door close button.  Otherwise once the 
					   door close button is pressed it would place the car immediately back on automatic opertion 
					   before is has a chance to time out or put on independent service.
10/19/05 v4.36 mhd	1. Do not set sf_GRIP flag in door routine.
11/4/05 v4.38 mhd	1. Added state to gripper_test to make sure gripper timers starts off at zero so now gripper test ends at 4.
11/10/05 v4.39 mhd	1. When stuck button only set the door reopen doorf = 1 if (doorf != 2). This will allow the nudging to 
                       time out properly. Check onward_calls with the nudging timer to nudge only when there  is an onward call
					   so the door stays open with only a stuck button at the floor.
					2. In chkdoor in the \ door control, had to add the remote car call input check. see remote_cc.
					3. Delete setting get_onward_calls in (medf == 3) chkdoor and (ncu_door_seq == 2) chkdoor. Instead
					   call get_onward_calls at the beginning of chkdoor all the time.
					4. IF in a not at a valid floor, ccmsk[position] == 0, set doorf = 0 in doors routine only if doorf == 1 or doorf == 2.
					   if the doorf == 3 or 4 let the door close. 
					5. Added door open or door close delay so you can avoid an instant door reversal.
					6. Added safe_closetry and safe_rclosetry. Open door if safe_closetry >= 6.
11/22/05 V4.40 mhd	1. Added manual door open buzzer.  Prevent door open button to open door at secured floor.
11/29/05 v4.41 mhd	1. Added security recall floor.	 Prevent car call at floor from opening door at secured floor and modified
                       door open button logic for the same security operation.
12/6/05 v4.42 mhd	1. Don't set doorf = 2 on fire door hold if a manual door. (prevent car from being stuck at the floor).
12/13/05 v4.44 mhd	1. Clear direction arrow when entering independent for the first time.
1/5/06 V4.45 mhd	1. Allow disable_auto_door to be set when door opens and not onward calls if car has a swing door (cons[mand] == 2).
1/13/06 v4.48 mhd	1. Don't use input LWD if A17.1-2004 code used, input is used for FSS instead.
1/16/06 V4.48 mhd	2. Added FF_STP for firefighters stop switch.
1/23/06 v4.49 pn	1. Added courion and ems automatic freight doors.
2/1/06 v4.50 mhd	1. Allow LWA, LWB and OVL for hydro and traction ndf cars.
2/1/06 v4.51 mhd	1. Allow EE OR SE to shorten door time.
2/2/06 v4.51 mhd	2. Changed front_slowdown and rear_slowdown to set slowdown and door open bits.  The door open bits will cause
                       the door to open even if the car or hall call inputs go away.
					3. Allow to always open door from a car call slowdown even if security is set (card reader security).
2/6/06 v4.53 mhd	1. Added pfc and sfc test.
					2. Added DZA test and moved DZ_ON_Fault to chkdoor.
02/09/06 v4.54 pn   1. Modified operation of extended door time feature.
2/16/06 v4.55 mhd	1. Use manual_door == 2 instead of cons[mand] == 2 for front doors to allow swing door with automatic gate
                       at staggerd rear floors.
2/24/06 v4.56 mhd	1. Added P input test to PFC/SFC test for hydro with soft starters.
					2. Move redundancy_test = 0 to DZ lost to prevent test after a normal relevel.
					3. Turn off FST and FSTP when starting the redundancy_test.
					4. Allow next up door close on next up and door open time-out. (To prevent from loosing service at the lobby).  
3/1/06 v4.57 pn		1. Added seperate retiring cam output on courion interface I/O board.
3/14/06 v4.59 mhd	1. Added field variable to enable nudging without onward call.
3/21/06 v4.59 mhd	2. Clear door open time-out timer if on door open limit in next up section.
					3. Set RDT for rear door transfer relay on peele doors.
3/24/06 V4.60 mhd	1. Modified peelle and courion fire 1 door flag for main and alternate.
3/30/06 v4.61 pn    1. Added rear auto door open for Courion door operator.
4/6/06 V4.62 mhd	1. Added push button door operation, see cons[pbdoor].
					2. Use extended door time flag to override the lobby door time to extend the time.
4/12/06 V4.63 mhd	1. Modify EMS door operator to turn on zone outputs only when at the door zone and 
					   the retiring cam is dropped. Also turn off DSC if on inspection or safeties. Last
					   Set Fire door hold when at the phase 1 foor or on fire hold. 
4/26/06 v4.64 mhd	1. On code blue, do not allow the door to open once the door is closed until it reaches the code blue floor.
                       This will keep the door closed on a reversal.
					2. Use do_seq flag to enable using the short door time so you have to have the door reach to DOL and 
					   then someone breaking the EE for a short door time.
4/27/06 v4.65 mhd	1. EMS door operator: Turn on FH when on earthquake and stopped at a floor to keep the door open.
					2. EMS door operator: Add EFST output to prevent door open button from working on phase 1.
5/5/06 v4.66 mhd	1. Set the non-interference timer to fvar[fvnit] time if DCB or DOL hit to allow the car to leave the floor early on
                       cons[pbdoor] operation. (User set NIT time to max to allow more time to open the door).
                    2. Made correction to EMS door routine to not look at (doorf==1)(deleted) when (eqf == 4) to set FH. 
5/30/06 V4.68 mhd	1. Added ED for remoted car call station instead of INDC.
					2. Clear the nudging and dwell timers when ED button pressed.
6/2/06 V4.68 mhd	3. Correct bug for EMT with hydro to cycle door. EMS input logic was inverted.
6/5/06 v4.69 mhd	1. Added att_buz for car switch operation.
6/16/06 v4.73 mhd	1. Added peelle door control for emergency power recall.
					2. Use GS gate switch for phase 2 to phase 1 return (fs2_fs1 == 1).
7/5/06 V4.74 mhd 	1. When on phase1 and must close the door check for cons[frght] != 0 to set doorf == 3
7/6/06/ v4.75 mhd	1. When on ind or hsv, bit set too close door with onward call, if dob hit while door closing, door would stall.
                       Check if door_open_device is active before closing door to fix problem. see door_open_device.
					2. Added variables fvdonodol to turn on the door open output if dol is not active when dwelling and 
					   also added fvdisglt to disable the gate and lock test for New York City.
7/13/06 v4.76 mhd	1. Clear redundancy_test state flag if test is interrupted due to a relevel. This problem will cause an FST, SFC or PFC 
					   fault when there is no fault.
8/3/06 V5.00 mhd	1. Moved door zone fault check to atfloor routine.
					2. Allow the door to open with dz or dza inputs on.
8/17/06 v5.02 mhd	1. Added varible fvars[fvdcbdt], when set to disable cancelling the door dwell time. 
8/31/06 v5.04 mhd	1. Use manual_rdoor for manual rear door operation.
					2. Added return to lobby flag to cycle door at the lobby and to cycle door on reversal.
10/10/06 v5.05 mhd	1. Added door time-out flag do_to to detect a dol time-out and to prevent keeping power on the DO when
					   the car has > 6 car safe start faults.
					2. On independent, with close door with onward car call, check for doorf != 1 to prevent from closing
					   the door if the car call is press while the door is opening, i.e. don't close the door right away, wait
					   until the door open limit is hit.
11/7/06 v5.08 mhd	1. Added att == 1, to turn on ATO output for courrion door operator. 
					2. Added non-simultaneous door operation. Added doorsel and clr_door_vars routines. Note that a door_req is set
					   from a door open device or a car or hall call and is cleared when the door_en flag is set.  The car is not
					   allowed to run from chkrun if a door_req flag is set. The door_req, fcc_req and fdob_req flags cause the 
					   doorf to be set causing the door to open.
					3. Set nudgst flag according to the dpref when door is nudged closed. This flag is used with non-simultaneous 
					   doors to switch door operation when there is a stuck button. 
1/4/07 v5.11 mhd	1. Change shutdown car routine to always cycle the door unless there was a door close failure, then keep
                       the door open. Use the extended door open timer if shutdown or lowoil and set the door close flag if dz not on. 
1/15/07 v5.15 mhd	1. If car on stall mode and door held open from EE then close door on nudging.
		 pn/mhd		2. Set DESL output for peelle doors if not on fire and on independent to disable the door timer.
					3. Make sure that ATO does not get set in firef == 0 part of courion door operater routine when car on attendant.
					4. Use in_overload flag to turn off OLL output when overload turned off.
					5. In doors() routine allow nudging timer to close door if not in DZ and also do not allow open from 
					   door open device if not on DZ only turn off the DO output.  Also do not zero the nudging timer if no DZ.
					6. In shutdown mode suchas DZ_on_Fault, nudge the door closed if EE stuck on.
2/9/07 v5.16 mhd	1. Corrected io check for PFC,SFC, and FST to take into account if someone pulls the stop switch during the test.
1/26/07 v5.18 mhd	1. Use Door_Lock_Fault flag to set door lock fault only once.
					2. Set Door Close Contact status flag in reset mode and in chkdoor when swing door is open.
2/23/07 v5.19 mhd	1. Added functions for ids: rtl, ad_off, heof and ind.
3/8/07 v5.19 mhd	2. Made ids_ind, ids_rtl, ids_heof and ids_ad_off array values.
					3. Check for door open limit to validate gate or lock fault.
3/14/07 v5.20 mhd	1. Use door_en to select front or rear swing door (manual_door == 2).  Also move subroutine call for doorsel in front
                       of swing door operation.	When using non-simultaneous door operation and swing doors, the swing door has to close to allow
					   the opposite door to open.  When using non-simultaneous door op with one swing and one auto door, when the auto door is enabled
					   the swing door stays closed leaving both doors closed unless the last door open was the swing door.  
					2. Added variable fvclgate to allow the gate to close on swing door operation.  The gate will stay open if the door contacts are
					   open or if a call is entered.
					3. Also set the doort for swing door to fvccdt when pilot zero otherwise leave doort set from the call that opened the door.
5/10/07 v5.23 mhd	1. Do not allow the fire door to open on phase 1 untile the position has been preset.
					2. Added leveling_fault to shutdown procedure.
5/30/07 v5.24 mhd	1. Added electric eye time-out mechanism.
6/6/07 v5.25 mhd	1. Added auto service  time-out recovery to zone car to a floor so car can go back into service.
					2. Added feature to disable the nudging output and to reopen door fully from ee or se. Also to turn off
					   buzzer when doors are closing if input devices are not disabled.
6/15/07 V5.26 MHD	1. Corrected remote car call to set fcc_req and fdob_req in doorsel routine. 
					2. Added code to prevent door open (turning on DO) in doors routine with doorf == 1 if (vshort_tfl != 0) until the car has releveled
					   to the next floor. 
6/25/07 V5.28 mhd	1. If not valid car call floor check for valid hall call before clearing f_door_en flag. (Allow door to open for hall call).
					2. Check for valid hall call floor or car call floor to open door with stop switch or DOB Door open button.
					3. Clear the lantern timers if the preference is zero and dirsel is called.
7/10/07 v5.30 mhd	1. Use fire 1 variable to time door close time-out on ind or att.
					2. Modified ind() operation so that with rear door, DCB will close the open front door but if there is no onward car call
					   the rear door will pop open.	If there is an onward car call, both doors will close.
					3. Added RTLDC for emergency return to lobby security (Close door).
8/6/07 v5.31 mhd  	1. Added variable fvnohcreop to prevent hall call from reopening when set and onward hall call.
				 	2. Added check for (rhjack < 2) to allow servf to be set to 1.  When rhjackf is > 2, servf is set to zero.
8/16/07 v5.33 mhd	1. Allow load weigh disable with any type load weigher.
8/24/07 v5.35 mhd	1. Don't open the door if stop switch pulled and not in door zone. Also delete check for relevel == 0 when not in door zone
                       and not safe to close the door. see safe(). The operation is as follows:  If stop switch pulled and not in door zone then 
                       shut the door. If hydro car, relevel the car up and reopen the door.  If traction car then relevel the car after the stop 
                       switch is released.
8/28/07 v5.35.1 mhd	1. Add Automatic Swing Door Open with manual gate.	Note: cons[mand] == 1.
9/4/07 v5.35.2 mhd	1. Use fvars[fvdisglt] to disable DPM check if new york fire service.
9/26/07 v5.36 mhd	1. Added fvrtldt (return to lobby door dwell time). 
10/1/07 v5.37 mhd	1. Added fvncupref to allow preference to change after the hall dwell time times out rather than waiting for the door to close.
					2. Use g_cb_fl[] instead of (codebfl & 0x8000) to open front or rear door.
10/15/07 v5.38.1 mhd	1. Set fdoor_req and rdoor_req to zero if on fire service.
						2. Set ind_door_sel = 1 for rear door on independent if car at floor with no front door.
						3. On Att, set doort to hc door time until dwell timer == door time, then set to dwell timer * 2 to
						   cause eta to start incrementing.  Eventually the group will re-assign attn call to automatic car.
10/30/07 v5.38.3 mhd	1. Modified attendant service to reset the dwell timer if there are no hall calls in the group or if no
						   hall call assignment to this car and EE hit.  This is to allow call assignment by resetting the ETA
						   time for the car.
11/14/07 v5.39 pn/mhd	1. Added car call test.
11/20/07 v5.39.2 mhd	1. Modified hoistway elevator off to be able to recall and to shut off the Cab light and fan.
11/28/07 v5.39.4 mhd	1. Use lobby_fl variable instead of fvars[fvlob] so that lobby_fl can be changed to alternate lobby fvars[fvaltlob]. 
						2. Zero extended_door_time and turn off EDL output if doors are closed. 	
12/12/07 v5.39.6 mhd	1. Added rdoort variable.
						2. Added ck_swing_door to keep swing door open if door close contact is not made.
12/19/07 v5.39.7 mhd	1. Modified stall and shutdown sections of chkdoor to keep door open if swing door.
1/3/08 v5.40 mhd		1. Added earthquake operation for hydro.
						2. Added i/o change cons[F1_CT] (Fire 1 Car Top) to place FBNB and FL on 1040 TOC board in place of EML and EQL.  When this option 
                           is selected, EML, EQL, and OLL are moved to P2, P3 and P4 on the 1040 Car call board.  HVD is moved to DZDE location and REV is 
						   moved to the DTR location on the 1040 (if option for HVD and REV selected).
						3. Added outputs F1ML and F1AL to EQL and EML outputs.  These outputs will follow the location of EML and EQL even when cons[F1_CT]
						   selected.
						4. Clear door timers in medf == 1 to prevent door close timeout.
1/24/08 v5.40.3 mhd		1. Any place that FS2C is used to cancel car calls, clear the pilot so that dirsel() will be called to allow car calls in the
						   opposite direction.
3/1/08 v5.41.1 hdl/mhd	1. Added attendant and car switch cycle buzzer.
3/7/08 v5.41.3 hdl		1. PH1 output used for fire recall was not being cleared on courion doors. It was being set even when the car got to the fire
						   service floor. Corrected courion_doors()	 under door.cc so when the firef==3 ph1 should clear
3/10/08 v5.41.4 mhd		1. Do not run relay check if Viscosity operation is running (Viscosity != 0).
3/18/08 v5.42.2 hdl/pn	1. Modify Courion_doors() to avoid the car to attempt a recall [fs1] if the car is in hospital service
						2. att_bz_on is now initialized as soon as the timer buzzer delays expires. Before we started in the off cycle.
3/21/08 v5.42.3 mhd		1. Modified HEOF to recall to floor set by fvars[fvheoffl].  If variable is zero then recall to the lobby.
3/28/08 v5.42.6 mhd		1. Run sfc/pfc test after soft stop timer expires.  Hydro would drop MST during the test during soft stop.
4/3/08 v5.42.7 rjc/pn	1. When the doors are closing (front or rear, door flag = 3) keep the door close signal on for an additional
						   3 seconds once DCL is made.  This is for automatic front or rear doors application only.
4/10/08 v5.43.2 mhd		1. Added door close alarm for freight doors.
						2. Turn off DCR, NUDR and DOR, when auto door is off and rdoorf set to 1.  Deleted clearing these rear output in in doors() when auto door
						   is off.
4/15/08 v5.43.3 pn		1. Changed "cons[ids] == 1" check to "((cons[ids] & 1) != 0)" since I added
						   an option for cons[ids] bit 2 for adding an additional GPIO board for 
						   montoring of external signals through lift-net.
6/13/08 v5.44.3 pn		1. Removed reference for traction to include hall elevator off feature for hydro.
7/7/8 5.44.6/5.45 pn/md	1. Clear PH2 and PH2HD on phase 2 to phase 1 return for currion doors.
						2. Use fvars[fvacctopfl] and fvars[fvaccbotfl] to test top and bottom floor door locks.
8/4/08 v5.45.4 mhd		1. Changed SPB version check from ((SPB_can.version >= 5) && (SPB_can.revision >= 8)) to 
						   ((SPB_can.version >= 5) || ((SPB_can.version == 5) && (SPB_can.revision >= 8)).
						2. Added GTS test for emergency brake PM machines.
						3. Added Contactor test for emergency sheave brake.	Increased the gripper_test variable maximum.
8/18/08 v5.45.5 mhd		1. Use cons[EM_BRK] == 4 for secondary emergency brake on PM AC machine if A17.1 2007 required instead of cons[EM_BRK] == 1.
8/19/08 v5.45.5 pn		1. Added 4 additional outputs for EMS door operator.
8/22/08 v5.45.6 mhd		1. Added cons[EM_BRK] = 5 for seperate EBC and UTM control.
8/29/08 v5.45.7 mhd		1. Changed SPB version check from ((SPB_can.version >= 5) to ((SPB_can.version > 5).
9/3/08 v5.45.8 mhd		1. Added hugsf flag to turn on EOF service if flag is set.
10/16/08 V5.46.1 hdl	1. Added temperature sensor shutdown for hydro.
10/20/08 v5.46.2 mhd	1. Added door enable timer for non-simultaneous doors to start the opposite door open before the first door closes.
10/30/08 v5.46.4 pn/mhd	1. Turn on PH2H for phase 2 to phase 1 return on courion doors.
12/16/08 v5.46.8 hdl	1. Call carcall() routine if (vipf == 3) to allow entry of car call while vip car waiting at the floor.
						2. Added check for cons[att] == 2 to set attendant hall annunciator lights.
12/10/08 v5.46.9 mhd	1. Add change to return to phase 2 ahead of emergency power so that is emp and phase 2 car turned off
 						   phase 2, doors closing and key switch turned back to phase 2 will cause door to reopen.
						2. Move call for carcall() when (vipf == 3) to vip section after med and emp section. see ((vipf == 2)||(vipf==3)).
1/9/09 v5.47 mhd		1. Use the ir_car flag, any_hall_call flag and any_ir_hall_call flags to clear the dwell timer so the feature for the att_buz works
						   for standard and ir hall calls.
1/10/09 v5.47.2 mhd		1. Added service timer activation of sabbath service.
1/23/09 v5.47.3 mhd		1. Zero relevel timer if pfc_sfc test is interrupted to give SPB time to reset before a relevel.
						2. Clear fdoor_req and rdoor_req in ck_swing_door() to prevent non-simultaneous door flags from keeping car at the floor when on ind, att
						   and any other non auto door service.
1/27/09 v5.47.4 mhd		1. Modified gripper test to exit and reset the gripper test flag to the SPB and reset the I/O so that the system
						   does not lock up it test aborts.  Added in_gripper_test flag.
						2. Modified PFC/SFC test to reset the test SFC flag to the SPB if test aborts.
2/02/09 v5.47.4 pn		1. Call different sabbath subroutine when cons[sabbath] = 4.
						   Car will only answer car calls in the down direction and then
						   express to the top floor to repeat process.
2/20/09 v5.47.6 mhd		1. Disable car calls to reopen the floor when security codes are being entered from car call panel (dis_cc_reopen).
						2. Added service activation timer for car call push button security and input CCPBS.
3/11/2009 v5.47.9 hdl	1. Added one timer timers[theoff] to debounce signal for HEOF. when we had "cons[Elev_off] & 4) != 0" the signal
							would misfire and make the car go out of service   
3/27/09 v5.48 mhd		1. Do not clear EDL output or extended_door_time if using rear door and opposite door is open or opening.
4/09/09 v5.48.1 hdl		1. Changes the logic for stallf = 2. It will only open the doors for a hydro when the car is at recall floor
							if car is stalled and lost, it will update position based on binaries before opening doors.
4/14/09 v.5.48.2 mhd 	1. Added dz check for peele, courion and ems doors to turn on the door open.
						2. On Peele door turn on HDB to keep door open on ind and emp.
5/18/09 v5.48.6 mhd		1. Added pwl_ups_fault check to shutdown door opeartion to cycle door and to take car out of service.
6/15/09 V5.49 mhd		1. Added new swith logic for FS2 when cons[FS2_SW] == 1. fs2_on = NOT FS2 and NOT FS2H.	Not that FS2 is then displayed as FS2OF.
7/23/09 v5.49.2 hdl		1. Added timer tattbzini to replace the non-interference timer everywhere
							in the logic for attendant buzzer operation.
						2. Modify the logic for the car to check the doors to be dwelling or just
							having manuals doors for the attendant buzzer sequence to commence.
9/16/2009 v5.50.1 hdl	1. added a routine call to next_stop_lobby() 
9/28/09 v5.50.3 mhd		1. Added sabbath service = 5 to stop at odd or even floors up or down.
						2. Added sabbath control flag to allow momentary ir service to override sabbath.
10/7/09 v5.50.4 mhd		1. Added detector edge time-out fault.
						2. Added Galileo car control variables - ui_ind, ui_rtl, ui_heof, and ui_ad_off
						3. Added check for galileo (((cons[Elev_off] & 3) != 0) || ((cons[ids] & 1) != 0) || ((cons[Galileo] & 1) != 0)) to execute
						   hoistway elevator from ui_heof.

11/25/2009 v5.50.8 hdl	1.  Sabbath service was modified. Rename sel_col_sabbath_svc to sabbath_sel_col_svc
						2.  Rename sabbath_svc to sabbath_std_svc
						3.  sabbath_svc becomes the function that controls every option of sabbath
12/4/09 v5.50.10   pn   1. In sabbath_svc routine, removed code to check if position has changed
						   in order to set the front and rear door time to sabbath door dwell field 
						   variable.  When on sabbath always set the doort and rdoort to sabbath door
						   time.
12/10/09 v5.50.11 mhd	1. Deleted spb_can_error and Hoistway_Learned checks from hydro at floor shutdown. 
12/28/09 v5.51.2 mhd	1. Use variable fveeshortdt to disable short door time from ee. 
						2. Use ((fvars[fvsabben] & 0x02) != 0) to change sabbath door time to lobby door time when car at lobby.
						3. Changed name of sabbath_sel_col_svc() to sabbath_down_col_svc() to reflect the down collective operation.
1/15/10 v5.51.5			1. Changes the chkdoor routine for for the ids_return services. It allows liftnet to control the doors when 
							the car is emerngency return from liftnet (used elevator off for both)
1/26/10 v5.51.9 mhd		1. Run pfc/sfc test with traction emergency power lowering only if drive has power.
3/12/10 v5.51.14 mhd	1. If the floor becomes secured while door is open, nudge the door closed.
3/23/10 v5.51.16 mhd	1. Execute code blue door open operation for codebf 2, 3, and 4. (added 4).
						2. Disable door close button if codebf == 4 (same as codebf == 3).
						3. Added Sabbath_start_at_lobby() routine.
3/25/10 v5.52 mhd		1. Set servf = 0 when the car has a "DPM AND gate or lock" fault to take the car out of group service 
						   even if the door doesn't open.
3/26/10 v5.52.1 mhd		1. Added max door hold timer for ED button being pressed constantly.
						2. When fvmaxdoorhld > 0 the ED input becomes the door hold open input. Open the door if closing.
						3. Take car out of service if ED button pressed and fvmaxdoorhld > 0.
4/2/10 v5.52.2 mhd		1. Added p input fault to at floor shutdown if hydro and (Drive_Type[cons[carnmb]] == 9).

4/27/10 v5.52.3 hdl		1. Previously Extended Door (ED) time input was not being applied to rear openings.
						2. Lobby Dwell Time was not working for rear lobby openings.
5/4/10 v5.52.7 mhd		1. Added inputs FiDOB, FiDCB, FiFS2C, FiDOBR, FiDCBR, and output FiFL for rgb car call boards.
6/17/10 v5.52.9 mhd		1. Turn on up or down hall lanterns when Vip car reaches the vip floor. (fvars[fvviplant] == 1).
7/1/10 v5.52.12 mhd		1. Added check for ebrk_can_error to prevent the car from running automatic.
						2. Added check for UTM when cons[EM_BRK] == 6.
8/4/10 v5.52.19 mhd		1. Don't open door on ind() until preset_pos has been set to zero, i.e. the preset has been set.
8/12/10 v5.52.23 mhd	1. Modified emergency recovery to cycle door instead of keeping it open.
						2. Do not automatically cycle the door on leveling_fault unless door open button hit because car may be moving if
						   the brake did not hold.
9/20/10 v5.52.31  hdl   1. Added ELOO for a flashing output when in elevator off (servf = s_ELEV_OFF_HS)

10/1/10	v5.52.34 hdl	1. Correct the ELOO output so it does not rurn on and off unless the  ((cons[Elev_off] & 8) != 0)
							before this the ISER output will blink when HEOF was enabled
10/6/10 v5.52.35 mhd	1. Allow independent to override vip service.
10/14/10 v5.52.37 mhd	1. Delete using SPB_can.command |= spbc_SFC_test to test SFC in versions below version 6 
						   because SPB timing does not detect that it should time-out quicker. Do the test with version 6 
						   or above.
						2. Changed time out for SFC relay from 50 msec to 150 and 1.5 second to 3.0 seconds.
12/21/10 v5.52.53 hdl	1. Added Filed variable for  Elevator Off Options 2. The only one bit being used is DC to keep doors closed. This is used to force the doors to stay closed
							when the car is recalled with HEOF and latched the car in this mode. Securty reason to trap the person inside the car until the EOR is activated

1/18/11	v5.52.56 mhd	1. Modified emergency power for hydro to have seperate door cycle for car at home floor from cycling door in other conditions.
							see epc_wait_close.

1/26/11 v5.52.58 hdl	1. Added outputs for EMS door operator. EMS8,9,10,11
2/2/11 v5.52.58	 hdl	2. Changes doors to allow the doors to work with constant pressure when the remote cop is enabled and cons[prison] is set on

2/18/11 v5.52.3 hdl		1. Added parameter fvheofovrind to take the car out of independednt service to latch HEOF for recalling purposes (cons[Elev_off] & 6)
 						2. Added fvars[fvreopendoorfs2] to enable the door to reopen if it momentary close active and hall door is open (swing door type only)
						3. In door before Normal Automatic door operation, check for prev_Sec_Fl_Mask and upon change reset door security. Race condition will
							allow the DOB to work in secure floors

2/19/11	v5.53.13		1. Added software for cons[frghtd] == 9 for guilbert door interface

6/2/11 v5.54.11 mhd		1. Added changes to ansi 2004 fire for Australia
							a. Close door when fire start input on.
							b. Reopen door and cancel car call if start released before door fully closed.
6/15/11 v5.54.05 hdl	1. Added the changes for cons[TUG]. It enables an interface for Aethon Robots for medical supplies transport

6/15/11 v5.54.06 mhd	1. Corrected the firef == 6 logic not to keep the doorf = 1 at all the times

6/23/11 5.54.07	hdl		1. Added shutdown flag logic for hydro as well

7/8/11 5.54.12 mhd		1. Do Not do relay test that changes the state of FST when on Mass Medical Emergency service.
						2. Allow TUG robot (TDCB) AND/OR remote_cc DCB to closse the door immediately on automatic service.
						3. Deleted allowing TGDC when codebf == 1 and ems == 1. (TUG will exit car when it goes on code blue.)
						4. Corrected mistake with ind() and fire phase 1 where the front door close inputs were mistakenly changed to the rear.

7/13/11 v5.54.14 mhd	1. Use parameters to change reopen operation for Mass Med. Allow constant pressure or momentary.
						2. Use parameter to change independent override for Mass Med. Do immediate, after delay or no override.
						3. If on Mass Med and fire service activated, disable detector edge and close door at nudging speed.

8/12/11 V5.52.21 HDL	1. Added an ouput for DAO for Designated Attendant Operation
							DAO is what initiates the 20sec delay timer in the Peelle controller when there is a Phase 1 Recall 

9/8/11	5.54.24 hdl		1. 1. Removed fvreopendoorfs2 fvar number 445 and save it as a spare
9/13/11 v5.54.24 hdl 	1. Changed the fvnohcreopen to nohcreopen to allow up peak to changed this logic

10/24/11 V5.54.36 hdl	1. Added two more outputs to teh guilbert interface. No othe requirement but to mirror the dcb input in the COP.

12/5/1 v5.54.42 mhd		1. Turn off GR1 from SPB when running GTS test with (cons[EM_BRK] == 6).
						2. Deleted ability to disable the gate and lock check for NY.

2/1/12 v5.54.53 mhd		1. In fire phase 2, constant pressurre door close operation, allow door to open button to close door with stuck door close button.
						2. Added Denver fire service ammendment to ansi 2007 to close the doors after the car recalls on phase 1.  Doors are reopened 
						   from the hall call button at the recall floor.
1/24/12 v5.54.55 hdl	1. Changed the door routine for clearing up the doorf = 0 from when doorf == 2 and off fire service
							problem before was the controller will get trapped in this mode take calls but wouldnot run after fire service unless you reset it.
2/24/12 v5.54.58 mhd	1. Added changes to emergency power not zero the dwell time so that the door cycle closed at the homing floor if the car is on fire service 
						   and the variable is set for denver fire service.
						2. Set fire_fdo_seq and fire_rdo_seq to 1 when DOB and RDOB are hit respectively to reopen the door for denver fire service.
3/3/12 V5.54.61 hdl 	1. Added (cons[Elev_off] & 0x10) to enable the ELOO to show just for telling the car finished recall of elevator off mode

3/26/12 v5.54.68 hdl	1. hdoor_dis was added to peelle_doors() to check on the low oil condition to stop the hall buttons from operating
4/17/12 v6.0.06 mhd		1. Set UTM output if sfc/pfc test terminates early.
						2. Clear spbc_GR1_test flag to SPB to make sure the GR1 relay does not stay off from an early termination of the sfc/pfc test.
						3. Also with an early termination of a gripper test also check the in_redundancy_test flag to clear the spbc_GR1_test flag.							
5/1/12 v6.0.7 mhd		1. Added new brake control cons[EM_BRK] == 7.  This is to have a controled drop of the emergency brake even after MC drops.
4/17/12 v5.54.74 mhd	1. Set UTM output if sfc/pfc test terminates early.
						2. Clear spbc_GR1_test flag to SPB to make sure the GR1 relay does not stay off from an early termination of the sfc/pfc test.
						3. Also with an early termination of a gripper test also check the in_redundancy_test flag to clear the spbc_GR1_test flag.							
5/1/12 v5.54.76 mhd		1. Added new brake control cons[EM_BRK] == 7.  This is to have a controled drop of the emergency brake even after MC drops.

5/10/12 v5.54.78 hdl	1. Tugs. Door open button rear was not working. There was some logic missing on the door routines for rear door operation.

5/12/12 v5.54.79 hdl    1.  change from dcalls any_hall_call in the logic for attendant buzzer so it cown come on on car calls. Only hall calls

5/29/12 v5.54.84 hdl	1. 	When the elevator was stalled out of service and we were in the closing sequence the door will not reopen 
							or stay open if the door open button was presesed. Door would close on people and then allow reopen once fully close.
							Now, it allows doors to stay open
7/12/12	v5.54.94 hdl	1. swing door operation on both fron and rear and no simultanious door will exhibit issue where the controller will opent he correct door but the other 
							door wont get selcted to open even if the door contact opened up. Added a ch_swing_door to run even if door is not selected to correct issue
7/25/12 v6.0.12 mhd		1. Updated code from version 5.54.71 to v5.54.95.
8/21/12	v5.55.01 hdl	1. Change doors() and rdoors() so it wont clear the door open flag based on ccnmsk instead of the ccmsk
						2. Added a bit to fvdobsec to allow it to reopen door based on ccnmsk instead of the ccmsk
9/7/12 v6.00.21 mhd		1. Added changes from v5.55.01.
12/3/12 5.55.19 as		1. Added logic to reset the dob security when secf is enabled
12/18/12 6.0.26 mhd		1. Added galaxy3 changes 5.55.06 to 5.55.20.
1/4/13 v7.0.0 mhd		1. Added Riot Control service when cons[prison] == 2, servf = s_RIOT from v5.55.15 atl_fed_pen.
2/13/13 v7.0.1 mhd		1. Added manual select switch recall parameter and functiion to allow a car without group communications to be selected from the EPS input with one
						   of the following function: 0-run, 1-recall and run, 2 - recall only.	The parameter is checked when empf == ep_home to not clear the dwell timer.
6/29/13 v7.0.9 mhd		1. Car must be dead level to run sfc test
8/21/13 v7.0.16 mhd		1. Added changes below from Galaxy 3:
						{
							 as		1. Added fs2_no_fdoor_op and fs2_no_rdoor_op to correct the following condition. FS2 door closed, power loss. 
										If the door is half way open, controller was putting power on the door and it should not have automatic door operation unless
										the fireman pushes the DOB or DCB.
							 hdl/as	1. Changed  non-simulatneous doors operation in door select to 
										not just look at the DCL to determine of door is opening, chech doorf flag too
										
							 mhd	1. Added manual select switch recall parameter and functiion to allow a car without group communications to be selected from the EPS input with one
									   of the following function: 0-run, 1-recall and run, 2 - recall only.	The parameter is checked when empf == ep_home to not clear the dwell timer.
							 as		1. Added galaxy3 changes 5.55.25 to 5.55.30.

							 hdl	1. Added SQUEZ output to the peele door interface board
							 as		1. Modified sabbath_svc() routine to add logic for cons[Sabbath] = 6

							 as		1. Added below Galaxy III changes made in v5.55.41 - v5.55.43
									1. Modified chkdoor() routine and added any_hall_call_dbnc to fix FB/NB Attendent Buzzer Output timing issue 
									2. Modified chkdoor() routine to add logic for fvars[fvheofautorst] to auto reset HEOF hall elevator off input with timer
						}
9/3/13 v7.0.18 mhd/as	1. Added "DOB Over Sec" parameter option, 3 for DOB override CCS front only, 4 for DOB override CCS rear only
11/7/13 v7.0.27 mhd/hdl	1. Changed GRT1 timing from 500 msec to 1.5 seconds to allow for comm delay.
11/22/13 v7.0.29 mhd	1. Corrected i_EDHL; was o_EDHL.
6/3/14 v7.1.3 mhd		1. If other statusf flags are set in at_floor_check(), do not set sf_AF_SHUTDOWN flag so the problem is displayed. 
10/9/14 v7.1.16 mhd		1. Modified changes from galaxy 3 version 6.0.53 to 6.0.60.  See below:
						v6.00.57 as		1. Modified chkdoor() routine, complex hydro only, to clear dpref, dcalls and direction arrows when (low oil flag) stallf advances from 1 to 2 for being at the recall floor

10/17/14 v7.1.17 mhd	1. Modified changes from galaxy 3 version 6.1.0 to 6.0.10.  See below:
							v6.1.10 as 	1.	Added minimum short door time.
							
12/12/14 v7.1.23 mhd	1. Added dol and rdol limit fail operation.
12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: Note changes made for the MOVFR/EE and service light were different than G3.
							9/19/13 v6.00.57 as		1. Modified chkdoor () routine, complex hydro only, to clear dpref, dcalls and direction arrows when (low oil flag) stallf advances from 1 to 2 for being at the recall floor
							11/22/13 v6.1.14 mhd	1. Corrected i_EDHL; was o_EDHL.
							11/26/13 v6.1.17 as		1. Modified chkdoor (), when car has F/Rear opennings, car on independent service with doors open, put on phase one and if not at the recall floor, openned rear door without any command, 
												       made change so that rear doors do not open if they are fully closed.
							12/12/13 v6.1.20 as		1. Changed fvars[fvminshortdten] to be based on wheather the doort was set from hall call or car call
							1/10/14 v6.1.27 as		1. Disabled setting clearing SQUEZ output when we have staggered rear cons[StagRear] = 1 and DTR output is enabled

							1/14/14 v6.1.28 hdl		1. in chkdoor cancel gripper test if we are in fire service Set in_gripper_test to zero.

							2/9/14  v6.1.36 hdl		1. enable the door to reopen if momentary close active and hall door is open (swing door type only) (instead of using  
														fvreopendoorfs2 we made the change permanent. NYC fire service only.
							2/9/14  v6.1.38 hdl		1. when firef == 8, force doors to stay open if you lose your door open limit. There was a rece condition 
													between firef =5 and 8 when the doors needed to be reopen for swing doors in phase 2 NYC fire service
							3/21/14 v6.1.39 as		1. Added auto_peelle_doors() routine for adding logic for automatic peele doors operator
													2. Added front and rear electric edge test routines freight_eef_test() and freight_eef_test for peele automatic door
							6/24/14 v6.1.58 pn/as	1. Modified chkdoor () routine to allow doort to be set as handicap dwell time when ed input is on and fvars[fvmaxdoorhld] = 0
							8/06/14 v6.1.63 as		1. Added logic for HDOB, HDCB hall door open and close f+r buttons for the peele door operator when galaxy is master when car servf == s_AUTO only.

							8/11/14 v6.1.65 as		1. Added below changes made by Hernan to the project.
													8/07/14 v6.1.63 hdl		1. Added logic for doorf == 0 and rdoorf = 0. It checks for door close and if door is not close, it initiates a door close command.
																			   If rear door was forced open, controller will never close it. It will just stay tehre with demand and display waiting assignment
							8/14/14 v6.1.67 as		1. Modified doors() and rdoors() routine to have the logic for DSI and RDSI door stop switch input the the peele door operator
							8/22/14 v6.1.72 as		1. Disabled PDCA (peele door close alarm) output when on fire service.

							10/02/14 v6.1.83 hdl		1. revisited change done on 8/11/14 v6.1.65. Not okay to force door close (when doorf is zero) if fire service is active.
							10/13/14 v6.1.88 hdl	1.Modified the logic for fvcboverfs to allow code blue to override FS. It bypasses the check in needtostop() and 
														allows hsvf to advance to 1 even when in fire service.
							11/19/14 v6.1.99 as		1. Modified doors() and rdoors() routines to do the electric eye test when doorf/rdoorf = 2 and door dwell time has expired.
							12/08/14 v6.2.03 as		1. Added auto_swg_doors() for auto swing doors, set ADO and ADOR output to allow GALaxy to control openning of Auto Swing Door on CC, HC, DOB and EE devices.
							12/12/14 v6.2.06 mhd	1. Added dol and rdol limit fail operation.
							
04/15/15 v7.1.30 mhd		1. Added check for drive comm error to at_floor_check() to put car into at floor shutdown if comm lost to drive.  see sf_DRV.
5/26/15 v7.1.32 mhd		1. Turn on LE and LE1 in state zero of the gripper test to make sure the DZ relays are picked.
7/13/15 v7.1.37 as		1. Added below 2013 fire code related changes from GALaxy 3 project.
						01/26/15 v6.2.13 as		1. Modified chkdoor() routine for ASME A.17.1-2013/CSA B44-13 section 2.27.3.1.6, cons[ansi] >= 2013 phase 1 main/alt if not at rec fl, allow DOB to fully open doors and reclose within 15 seconds.
12/2/15 v7.1.51 mhd		1. Put car on Af Floor Shutdown when ther is an encoder com error.
12/21/15 v7.1.54 tb		1. Migrated checks for parameters ((fvars[fvceloff] & 0x08) == 0) and ((fvars[fvheloff] & 0x08) == 0) in chkdoor() routine.
01/05/16 v7.1.59 tb		1. Migrated the following change from G3:
							01/05/16 v6.4.11 hl		1. Changed the logic to clear the attendant buzzer timer.
														Problem: Controller with manual doors and rear only opening will never turn on buzzer while at rear landing
														Solution: Change the logic to clear timer so it wont clear when rear manual door and rear only landing.
									  							  ((cons[mand] != 1) && (rdoorf != 2)))
01/11/16 v7.1.61 tb		1. Migrated the following change from G3:
							01/07/16 v6.4.12 hl		1. change any_hall_call to any_hall_call_dbnc to enable the att_buz for rear openings. any_hall_call_dbnc was
						   							   already fixed for the front in version 6.00.41
01/12/16 v7.1.62 tb		1. Modified chkdoor() logic to match G3 checks for fvheofovrind. When ((fvars[fvheofovrind] & 0x02) != 0), it will allow HEOF to override attendant.
1/24/17 v8.0.1 mhd		1. Added check for statusf3 and statusf4 in at floor shutdown. Search f_rpm_gl.
8/18/17	V8.0.9 mhd		1. Added the following changes from G4:
						07/06/17 v7.3.05 hl		0. Migrate changes from G3 for Door fault monitoring from G3 100 CPU (v6.5.09)
												1. renamed: dpm_lock_check() to lock_gs_dpm_check() and dpm_lock_check_for_gs() to gs_only_check().
												2. doors() rdoors() and chkdoor() routines were updated for door lock monitoring logic using booleans above.
												3. created new routine lockgsdpm_fault_doorop() to keep doors open when out of service on a Door_Lock_GS_DPM_Fault != 0
												4. Corrected timer used to generate fault f_rpm_on. [tdwel] was being used instead of [trdwel].
			
*/