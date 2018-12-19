#define d_GHALLCALL 1
#include "global.h"

// group hall call variables

int32 g_up_hall_call[2][car_size];
int32 g_dn_hall_call[2][car_size];
int32 g_upr_hall_call[2][car_size];
int32 g_dnr_hall_call[2][car_size];

int32 g_cc_req[2][car_size];		  		// car call request for car
int32 g_rcc_req[2][car_size];
int32 g_cc_dpy[2][car_size];		  		// car call destination display for car
int32 g_rcc_dpy[2][car_size];


int16 nmb_up_call[100][fl_size];
int16 nmb_dn_call[100][fl_size];
int16 g_asign_up[fl_size];
int16 g_asign_dn[fl_size];
int16 g_asign_upr[fl_size];
int16 g_asign_dnr[fl_size];

bool gc_nca_xasgn = 0;					// input for no cars available from other group

#if (Simulator == 1)
extern int16 dn_hc_range_1;
extern int16 dn_hc_range_2;
extern int16 dn_hc_range_3;
extern int16 dn_hc_range_4;
#endif		 

// Lobby Up call Request varibles
uint8 lby_up_req = 0;
int16 lby_up_req_car[car_size];
int16 lby_up_assign = 0;

int16 car_asgn_uc[fl_size];
int16 car_asgn_dc[fl_size];
int16 car_asgn_cc[fl_size];
int16 car_asgn_ruc[fl_size];
int16 car_asgn_rdc[fl_size];
int16 car_asgn_rcc[fl_size];
int16 car_asgn_sec = 0;

// Que to collect call in the order received (front and rear collected together)  rear Or'ed with 0x80
unsigned char up_hc_que[c_hc_que_len];
unsigned char dn_hc_que[c_hc_que_len];

// Traffic timers
int16 dctim[fl_size];          // down hall call timers
int16 uctim[fl_size];          // up hall call timers
int16 dcrtim[fl_size];         // down hall call timers
int16 ucrtim[fl_size];         // up hall call timers

// group hall call functions

void grp_clrhc(void );
void grp_clr_canc();
void group_hallcall(void );
int16 car_at_fl_cancel(int16 cancel_dir,int16 fl);
int16 place_uphc_in_que(int16 fl);
int16 remove_uphc_from_que(int16 fl);
int16 is_uphc_in_que(int16 fl);
void clr_uphc_que(void);
int16 place_dnhc_in_que(int16 fl);
int16 remove_dnhc_from_que(int16 fl);
int16 is_dnhc_in_que(int16 fl);
void clr_dnhc_que(void);
void latch_hallcall(void);
//void latch_ser_hallcall(void);
void assign_hallcall (void);
int16 keep_assignment (int16 assigned_car, int16 new_assign_car, int16 fl, int16 dir, int16 front_rear);
int16 make_assignment (int16 assigned_car, int16 new_assign_car, int16 fl, int16 dir, int16 front_rear);
void group_handicap_car_floor(void);
void hall_call_cross_assignment (void);
void group_hallcancel(void );
void clr_dest_cc (void);

int32 hc_dbg1 = 0;
int32 hc_dbg2 = 0;
int32 hc_dbg3 = 0;
int32 hc_dbg4 = 0;



// *******************************************
// This is the group clear hall call procedure
// *******************************************
void grp_clrhc()
{
  	int16 i;
  	timers[tgroup_hcc]  = 0;
  	for(i = 1; i < cons[grtopf]; i++)
  	{
		g_asign_up[i] = 0;
    	uctim[i] = 0;
	}
	gc_uc_canc[0] = 0xFFFFFFFFL;
	gc_uc_canc[1] = 0xFFFFFFFFL;
#if (Simulator == 1)
	gc_uc[0] = 0;
	gc_uc[1] = 0;
#endif

	clr_uphc_que();

	lby_up_assign = 0;
	g_up_hall_call[0][0] = 0;
	g_up_hall_call[1][0] = 0;
	for (i=1;i<car_size;i++)
	{
		if (i != ir_car)
		{
  			g_up_hall_call[0][i] = 0;
  			g_up_hall_call[1][i] = 0;
		}
  	}

  	for(i = 2; i <= cons[grtopf]; i++)
  	{
	   	g_asign_dn[i] = 0;
    	dctim[i] = 0;
	}			  
	gc_dc_canc[0] = 0xFFFFFFFFL;
	gc_dc_canc[1] = 0xFFFFFFFFL;
#if (Simulator == 1)
	gc_dc[0] = 0; 
	gc_dc[1] = 0;
#endif
	
	clr_dnhc_que();

  	g_dn_hall_call[0][0] = 0;
  	g_dn_hall_call[1][0] = 0;
	for (i=1;i<car_size;i++)
	{
		if (i != ir_car)
		{
		  	g_dn_hall_call[0][i] = 0;
		  	g_dn_hall_call[1][i] = 0;
		}
	}

  	if(grtop_rfl != 0)
  	{
	  	for(i = 1; i < cons[grtopf]; i++)
	  	{
			g_asign_upr[i] = 0;
		    ucrtim[i] = 0;

		}
		if (gc_hc_dev != 0)
		{
			gc_ruc_canc[0] = 0xFFFFFFFFL;
			gc_ruc_canc[1] = 0xFFFFFFFFL;
#if (Simulator == 1)
			gc_ruc[0] = 0;
			gc_ruc[1] = 0;
#endif
	  	}

	  	g_upr_hall_call[0][0] = 0;
	  	g_upr_hall_call[1][0] = 0;
		for (i=1;i<car_size;i++)
		{
			if (i != ir_car)
			{
			  	g_upr_hall_call[0][i] = 0;
			  	g_upr_hall_call[1][i] = 0;
			}
		}

	  	for(i = 2; i <= cons[grtopf]; i++)
	  	{
			g_asign_dnr[i] = 0;
		    dcrtim[i] = 0;
	  	}

		if (gc_hc_dev != 0)
		{
			gc_rdc_canc[0] = 0xFFFFFFFFL;
			gc_rdc_canc[1] = 0xFFFFFFFFL;
#if (Simulator == 1)
			gc_rdc[0] = 0;
			gc_rdc[1] = 0;
#endif
	  	}

	  	g_dnr_hall_call[0][0] = 0;
	  	g_dnr_hall_call[1][0] = 0;
		for (i=1;i<car_size;i++)
		{
			if (i != ir_car)
			{
			  	g_dnr_hall_call[0][i] = 0;
			  	g_dnr_hall_call[1][i] = 0;
			}
		}
  	}
  	return;
}

// ******************************************************
// This is the group clear hall call cancel procedure
// ******************************************************
void grp_clr_canc()
{
	gc_uc_canc[0] = 0;
	gc_dc_canc[0] = 0;
	gc_uc_canc[1] = 0;
	gc_dc_canc[1] = 0;
  	if(grtop_rfl != 0)
  	{
		gc_ruc_canc[0] = 0;
		gc_rdc_canc[0] = 0;
		gc_ruc_canc[1] = 0;
		gc_rdc_canc[1] = 0;
	}
}

// *******************************
// register a hall call procedure
// *******************************
void group_hallcall()
{
  	int16 i;
	int16 fl_ix;
	int16 no_calls=0;

	// Car Assign to floor security
	if (cons[Car_Asgn_Fl] != 0)
	{
		if ((cons[sercom] & 0x20) == 0)
		{		   // no serial hall call boards
			if (cons[epgrp] != 0)	   // determine if input is being used.
				i = cons[grtopf]+3;
			else
				i = cons[grtopf]+1;

			fl_ix = (i-1)/32;
			if ((gc_dc[fl_ix] & fl_mask[i]) != 0)// check to see if lobby up hall call pushed
				car_asgn_sec = 1;
			else
				car_asgn_sec = 0;
		}
		else
			car_asgn_sec = (gpi[g_invalid_io_loc] & 0x20) != 0;
		
	}

	if (((cons[att] != 0) || ((cons[Car_Sw] & 1) != 0)) && (cons[nmbcars] == 1))
	{			  // attendant hall call cancel (simplex cars only)
		if (gin_ATTHC == 1)
			grp_clrhc();
	}

  	if ((((cons[mand] == 1) && (cons[frghtd] == 0) && (cons[Car_Sw] == 0)) && (fvars[fvsapb] == 0))	|| 
  				(((fvars[fvsapb] & 0x01) != 0)  && (cons[nmbcars] == 1)))
  	{
      	if (((grp_Car_Stat[cons[carnmb]].g_run_statusf & rs_DOL) == 0) || 	  // if manual doors and the door is open
      		((cons[rear] != 0) && ((grp_Car_Stat[cons[carnmb]].g_run_statusf & rs_DOLR) == 0)))
      	{
		  	timers[tgroup_hcc] = 0;       // don't register a hall calls
		  	grp_clrhc();
      	}
  	}
  	if(timers[tgroup_hcc] < 5)
		return;
	if((all_cars_ots == 1) && (cons[frghtd] == 0) && 
		((cons[mand] == 0) || (cons[mand] == 2) || (cons[mand] == 5) || (cons[mand] == 6) || (cons[mand] == 7)))
	{                     // all cars are out of service cancel hall calls
		if ((fvars[fvxasgn] & 0x05) == 0)
		{		// Can be 0 or 2
			if ((fvars[fvxasgn] == 0) && ((fvars[fvotshc] & 0x01) == 0))
			{
				grp_clrhc();
				if (cons[Second_Riser] == 3)
					grp_clrirhc();
			}
			else if ((fvars[fvxasgn] & 0x02) != 0)
			{
				//grp_clrhc(); // added this on 6.1.2 and removed on 6.1.22
				grp_clr_canc();
			}
			return;
		}
	}


	// Hall call power lost

  	if((gin_HCP == 0) && (timers[thct] > 20) && (fvars[fvemdisp] != 0))
	{	 // Set hall calls to dispatch cars to all floors down collective 
		//  above the lobby	and up collective below the lobby
		hc_power_lost = 1;
		emdispf |= 0x01;
		no_calls = 1;
		if ((g_up_hall_call[0][0] != 0) || (g_dn_hall_call[0][0] != 0))
			no_calls = 0;
		if ((grtop_rfl != 0) && ((g_upr_hall_call[0][0] != 0) || (g_dnr_hall_call[0][0] != 0)))
			no_calls = 0;
#if (Tract_HR == 1)
		if ((g_up_hall_call[1][0] != 0) || (g_dn_hall_call[1][0] != 0))
			no_calls = 0;
		if ((grtop_rfl != 0) && ((g_upr_hall_call[1][0] != 0) || (g_dnr_hall_call[1][0] != 0)))
			no_calls = 0;
#endif
		if (no_calls == 1)
		{
		 	if (timers[thcpl] > 20) 
			{
				for(i=1; i<=lobby_fl; i++)
				{	// Set up calls for floors below and at the lobby
					fl_ix = (i-1)/32;
					if ((g_ucmsk[fl_ix] & fl_mask[i]) != 0)
						g_up_hall_call[fl_ix][0] |= fl_mask[i];
					if ((grtop_rfl != 0) && ((g_rucmsk[fl_ix] & fl_mask[i]) != 0))
						g_upr_hall_call[fl_ix][0] |= fl_mask[i];
				}
				for(i=lobby_fl+1; i <= cons[grtopf]; i++)
				{	// Set down calls for floors above the lobby
					fl_ix = (i-1)/32;
					if ((g_dcmsk[fl_ix] & fl_mask[i]) != 0)
						g_dn_hall_call[fl_ix][0] |= fl_mask[i];
					if ((grtop_rfl != 0) && ((g_rdcmsk[fl_ix] & fl_mask[i]) != 0))
						g_dnr_hall_call[fl_ix][0] |= fl_mask[i];
				}
			}
		}
		else
			timers[thcpl] = 0;
	}
	else if ((gin_HCP == 1) && (hc_power_lost == 1))
	{		// clear hall calls when hall call power comes back on
		hc_power_lost = 0;
		emdispf &= ~0x01;
		grp_clrhc();
		return;
	}
	else
	{
 		timers[thcpl] = 0;

		// Group Sabbath Service

		if ((cons[Sabbath] == 3) && ((gin_SAB == 1) || (tmr_svc[s_sabbath] == 1)))
		{
			group_sabbath();
			in_grp_sabbath = 1;			// set flag to clear hall calls after sabbath service turned off
		}
		else if (in_grp_sabbath == 1)
		{		// clear hall calls when hall call power comes back on
			in_grp_sabbath = 0;
			grp_clrhc();
			return;
		}
		else
			timers[tsabbath] = 0;
	}

	ir_control();
	
	if(cons[SecFlCfg] == 5)
	{
		if(gin_SECFM == 1)
		{
			evacuation_svc();
		}
	}

	if ((dpref == 0) || ((fvars[fvsapb] & 0x02) != 0) || (cons[nmbcars] > 1) ||
		( ((cons[mand] != 1)  || (cons[frghtd] != 0) || ((cons[Car_Sw] & 1) != 0)) && (fvars[fvsapb] == 0) ) )
	{			   // Normal hall call operation

		if (gc_hc_dev == 0)
			latch_hallcall();
		else 
		{
			hallcall_comm_loss();
//			latch_ser_hallcall();
		}

		if (cons[incrs] != 0)
		{
			if ((ir_car >= 1) && (ir_car <= cons[nmbcars]))
				latch_ir_hallcall();
		}
		else if (cons[Second_Riser] != 0)
				latch_sr_hallcall();
		
		if ((cons[Third_Riser] != 0) && (cons[rear] == 0))
			latch_tr_hallcall();
	}
	else if (gc_hc_dev != 0)
	{		// cancel all other call for single automatic push button
		gc_uc_canc[0] |= ~g_up_hall_call[0][0];
		gc_dc_canc[0] |= ~g_dn_hall_call[0][0];
		gc_ruc_canc[0] |= ~g_upr_hall_call[0][0];
		gc_rdc_canc[0] |= ~g_dnr_hall_call[0][0];
#if (Tract_HR == 1)
		gc_uc_canc[1] |= ~g_up_hall_call[1][0];
		gc_dc_canc[1] |= ~g_dn_hall_call[1][0];
		gc_ruc_canc[1] |= ~g_upr_hall_call[1][0];
		gc_rdc_canc[1] |= ~g_dnr_hall_call[1][0];
#endif
	}

	ir_dual_riser();			// has to be placed here to latch calls properly


#if (Tract_HR == 1)
	if (cons[Dual_Ser_Riser] == 1)
		dual_serial_riser();
#endif

	if ((cons[Handicap_HPB] != 0) && (fvars[fvhandwait] != 0))
		group_handicap_car_floor();

	assign_hallcall();

	if (cons[Second_Riser] != 0)
		assign_sr_hallcall();
	
	if ((cons[Third_Riser] != 0) && (cons[rear] == 0))
		assign_tr_hallcall();
	
	clr_dest_cc();
	
}

//***************************************************************
// Clear destination dispatch car call data since not dd group
//***************************************************************

void clr_dest_cc (void)
{
	int16 i;
	// Since this group is not a destination dispatch group, set the car call data to each car to zero
	for (i=1;i<=cons[nmbcars];i++)
	{
		g_cc_req[0][i] = 0;		  		// car call request for car
		g_cc_req[1][i] = 0;		  		// car call request for car
		g_rcc_req[0][i] = 0;
		g_rcc_req[1][i] = 0;
 		g_cc_dpy[0][i] = 0;		  		// car call request for car
		g_cc_dpy[1][i] = 0;		  		// car call request for car
		g_rcc_dpy[0][i] = 0;
		g_rcc_dpy[1][i] = 0;

	}
	
	
}
//***************************************************************
// Check if car cancelling call at floor in appropriate direction
//***************************************************************

int16 car_at_fl_cancel(int16 cancel_dir,int16 fl)
{
	int16 i;
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((grp_Car_Stat[i].g_cancel_up == fl) && (cancel_dir == 1))
			return(1);
		else if ((grp_Car_Stat[i].g_cancel_dn == fl) && (cancel_dir == 2))
			return(1);
	}
	return(0); 
}

// **********************************************************
// Up and Down hall call input queue
// **********************************************************

//lint -esym(534,place_uphc_in_que)
int16 place_uphc_in_que(int16 fl)
{
	int16 ix = 0;
	while (ix < c_hc_que_len)
 	{
		if (up_hc_que[ix] == fl)
		{
			return 2;
		}
		else if (up_hc_que[ix] == 0)
		{
			up_hc_que[ix] = (uint8)fl;
			return 1;
		}
		ix++;
	}		
	return 0;
}

//lint -esym(534,remove_uphc_from_que)
int16 remove_uphc_from_que(int16 fl)
{
	int16 ix = 0;
	while (ix < c_hc_que_len)
	{
		if (up_hc_que[ix] == fl)
			break;
		else ix++;
	}	
	
	while (ix < (c_hc_que_len-1))
	{
		if (up_hc_que[ix+1] != 0)
		{
			up_hc_que[ix] = up_hc_que[ix+1];
			ix++;
		}
		else
		{
			up_hc_que[ix] = 0;	
			return 1;
		}
	}		
	return 0;
}

int16 is_uphc_in_que(int16 fl)
{
	int16 ix = 0;
	while (ix < cons[nmbcars])
	{
		if (up_hc_que[ix] == fl)
			return 1;
		else ix++;
	}
	return 0;
}

void clr_uphc_que(void)
{
	int16 ix;
	for (ix=0;ix<c_hc_que_len;ix++)
		up_hc_que[ix] = 0;
}


//lint -esym(534,place_dnhc_in_que)
int16 place_dnhc_in_que(int16 fl)
{
	int16 ix = 0;
	while (ix < c_hc_que_len)
 	{
		if (dn_hc_que[ix] == fl)
		{
			return 2;
		}
		else if (dn_hc_que[ix] == 0)
		{
			dn_hc_que[ix] = (uint8)fl;
			return 1;
		}
		ix++;
	}		
	return 0;
}

//lint -esym(534,remove_dnhc_from_que)
int16 remove_dnhc_from_que(int16 fl)
{
	int16 ix = 0;
	while (ix < c_hc_que_len)
	{
		if (dn_hc_que[ix] == fl)
			break;
		else ix++;
	}	
	
	while (ix < (c_hc_que_len-1))
	{
		if (dn_hc_que[ix+1] != 0)
		{
			dn_hc_que[ix] = dn_hc_que[ix+1];
			ix++;
		}
		else
		{
			dn_hc_que[ix] = 0;	
			return 1;
		}
	}		
	return 0;
}

int16 is_dnhc_in_que(int16 fl)
{
	int16 ix = 0;
	while (ix < cons[nmbcars])
	{
		if (dn_hc_que[ix] == fl)
			return 1;
		else ix++;
	}
	return 0;
}

void clr_dnhc_que(void)
{
	int16 ix;
	for (ix=0;ix<c_hc_que_len;ix++)
		dn_hc_que[ix] = 0;
}

//***************************************************
// This routine latches the Hall Calls for PC-104 I/O
//***************************************************
/*
void latch_hallcall(void)
{

	int16 i;
	int16 fl_ix;

	// Latch UP Hall Calls

  	for(i=1; i < cons[grtopf]; i++)
  	{
	    if((uctim[i] >10) && (gin_UC[i] == 0))
	    {
						// but the out put did not turn on
						// refresh the 8255's
			uctim[i] = 0;
	    }
		fl_ix = (i-1)/32;
	    if(gin_UC[i] == 1)// check to see if up hall call pushed
	    {
			if ((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0)
				place_uphc_in_que(i);		// call being latched so put hall call in queue
			g_up_hall_call[fl_ix][0] |= fl_mask[i];
			if (car_at_fl_cancel(1,i) == 0)
				gout_UC[i] = 1;		// no car cancelling the call
	    }
	    else
	    {
			if((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0)
			{
				uctim[i] = 0;
				g_asign_up[i] = 0;
				remove_uphc_from_que(i);
			}
		}
  	}

	// Latch the Down Hall Calls

  	for(i=2; i <= cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
      	if(gin_DC[i] == 1)  //check to see if dn hall call pushed
      	{
			if ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0)
				place_dnhc_in_que(i);		// call being latched so put hall call in queue
			g_dn_hall_call[fl_ix][0] |= fl_mask[i];
			if (car_at_fl_cancel(2,i) == 0)
				gout_DC[i] = 1;
      	}
      	else
      	{
			if((g_dn_hall_call[fl_ix][0] & ((int32)1 << (i-1))) == 0)
			{
				dctim[i] = 0;
				g_asign_dn[i] = 0;
				remove_dnhc_from_que(i);
			}
      	}
  	}
}
*/

//****************************************************************
// This routine latches the Hall Calls from Serial Hall Call Board
//****************************************************************

void latch_hallcall(void)
{

	int16 i;
	int16 fl_ix;
	bool asgn = false;
	bool xasgn = false;
	bool asgn_r = false;
	bool xasgn_r = false;

	any_hall_call = 0;

	// Latch UP Hall Calls

  	for(i=1; i < cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
	    if (((gc_uc[fl_ix] & fl_mask[i]) != 0) || ((gc_uc_dsr[fl_ix] & fl_mask[i]) != 0))// check to see if up hall call pushed
		{
			if (((Grp_HC_Lockouts == 1) && ((fvars[fvhcsecctrl] & 0x02) == 0) && (gc_hsec == 1) && ((gc_hc_sec[fl_ix] & fl_mask[i]) != 0)) || 
					(((ids_uc_sec[fl_ix] & fl_mask[i]) != 0) && ((cons[ids] & 1) != 0)) ||
					(((ui_uc_sec[fl_ix] & fl_mask[i]) != 0) && ((cons[Galileo] & 1) != 0)) ||
					((gc_hugsec == 1) && ((gc_hugs_sec[fl_ix] & fl_mask[i]) != 0)) || 
					(((fvars[fvdisopphc] & 0x01) != 0) && (sec_ftimer[i] < fvars[fvdisopphctm])))
			{
				if (((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0) || (gc_hugsec == 1))
				{
					gc_uc_canc[fl_ix] |= fl_mask[i];	// Call is secured so cancel call
				}	
			}
			else
			{
				if ((fvars[fvdisopphc] & 0x01) != 0)
				{	// clear sec_ftimer timer when first up/down hc is pressed
					if (((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0))
						sec_ftimer[i] = 0;
				}
				if ((fvars[fvxasgn] & 0x01) != 0)
				{
					if ((cons[XA_Mask] == 1) && (xa_uc_msk[i] != 0))
					{
						if ((xa_uc_ots[fl_ix] & fl_mask[i]) != 0)		// cannot be assigned
							gc_xuct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
						else if (xa_uc_msk[i] == 1)
							g_up_hall_call[fl_ix][0] |= fl_mask[i];
						else if (xa_uc_msk[i] == 2)
							gc_xuct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
						else if (xa_uc_msk[i] == 3)
						{
							xasgn = (g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0;
							asgn = (gc_xucl[fl_ix] & fl_mask[i]) == 0;
							if (asgn == true)
								g_up_hall_call[fl_ix][0] |= fl_mask[i];
							if (xasgn == true)
								gc_xuct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
						}
					}
					else if ((xassign == 0) && ((gc_xucl[fl_ix] & fl_mask[i]) == 0) && ((xa_uc_ots[fl_ix] & fl_mask[i]) == 0))			// no cross assignment of call
						g_up_hall_call[fl_ix][0] |= fl_mask[i];
					else if ((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0)	// call is not already set in system
						gc_xuct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
				}
				else
				{
					if ((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0)
						place_uphc_in_que(i);		// call being latched so put hall call in queue
					g_up_hall_call[fl_ix][0] |= fl_mask[i];
				}
			}

			gc_uc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
			any_hall_call = 1;
	    }
	    else
	    {
			if((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0)
			{
				uctim[i] = 0;
				g_asign_up[i] = 0;
				remove_uphc_from_que(i);
			}

			if ((fvars[fvxasgn] & 0x02) != 0)
				g_up_hall_call[fl_ix][0] &= ~fl_mask[i];

			if ((gc_hugsec == 1) && ((gc_hugs_sec[fl_ix] & fl_mask[i]) != 0) && ((gc_uc_canc[fl_ix] & fl_mask[i]) != 0))
				g_up_hall_call[fl_ix][0] &= ~fl_mask[i];

 			if ((gc_uc_canc[fl_ix] & fl_mask[i]) != 0)
 				gc_uc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
	
			if ((cons[Dual_Ser_Riser] == 0) || ((gc_uc_dsr[fl_ix] & fl_mask[i]) == 0))
			{
				if((cons[incrs] == 2)&&(ir_hc == 1))
				{
					if(gc_iruc[fl_ix] == 0)	
						gc_uc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
				}			
				else
					gc_uc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			}
			gc_xuct[fl_ix] &= ~fl_mask[i];
		}
		if(grtop_rfl != 0)
		{
		    if (((gc_ruc[fl_ix] & fl_mask[i]) != 0) || ((gc_ruc_dsr[fl_ix] & fl_mask[i]) != 0)) // check to see if up hall call pushed
			{
				if (((Grp_HC_Lockouts == 1) && ((fvars[fvhcsecctrl] & 0x02) == 0) && (gc_rhsec == 1) && ((gc_rhc_sec[fl_ix] & fl_mask[i]) != 0)) ||
					(((ids_ruc_sec[fl_ix] & fl_mask[i]) != 0) && ((cons[ids] & 1) != 0)) ||
					(((ui_ruc_sec[fl_ix] & fl_mask[i]) != 0) && ((cons[Galileo] & 1) != 0)) ||
					((gc_rhugsec == 1) && ((gc_rhugs_sec[fl_ix] & fl_mask[i]) != 0)) ||
					(((fvars[fvdisopphc] & 0x02) != 0) && (sec_rtimer[i] < fvars[fvdisopphctm])))
				{
					if (((g_upr_hall_call[fl_ix][0] & fl_mask[i]) == 0) || (gc_rhugsec == 1))
						gc_ruc_canc[fl_ix] |= fl_mask[i];		// call is secured so cancel
				}
				else
				{
					if ((fvars[fvdisopphc] & 0x02) != 0)
					{	// clear sec_rtimer timer when first rear up/down hc is pressed
						if (((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((g_upr_hall_call[fl_ix][0] & fl_mask[i]) == 0))
							sec_rtimer[i] = 0;
					}
					if ((fvars[fvxasgn] & 0x04) != 0)
					{
						if ((xassign == 0) && ((gc_xrucl[fl_ix] & fl_mask[i]) == 0) && ((xa_urc_ots[fl_ix] & fl_mask[i]) == 0))			// no cross assignment of call
						{
							g_upr_hall_call[fl_ix][0] |= fl_mask[i];
						}
						else if ((g_upr_hall_call[fl_ix][0] & fl_mask[i]) == 0)	// call is not already set in system
						{
							gc_xruct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
						}
					}
					else
					{
						if ((g_upr_hall_call[fl_ix][0] & fl_mask[i]) == 0)
							place_uphc_in_que(i | 0x80);		// call is being latched so put hall call in queue
						g_upr_hall_call[fl_ix][0] |= fl_mask[i];
					}
				}
				gc_ruc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
				any_hall_call = 1;
		    }
			else
			{
				if((g_upr_hall_call[fl_ix][0] & fl_mask[i]) == 0)
				{
					ucrtim[i] = 0;
					g_asign_upr[i] = 0;
					remove_uphc_from_que(i | 0x80);
				}

				if ((fvars[fvxasgn] & 0x02) != 0)
					g_upr_hall_call[fl_ix][0] &= ~fl_mask[i];

				if ((gc_rhugsec == 1) && ((gc_rhugs_sec[fl_ix] & fl_mask[i]) != 0) && ((gc_ruc_canc[fl_ix] & fl_mask[i]) != 0))
					g_upr_hall_call[fl_ix][0] &= ~fl_mask[i];

				if ((gc_ruc_canc[fl_ix] & fl_mask[i]) != 0)
					gc_ruc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
	
				if ((cons[Dual_Ser_Riser] == 0) || ((gc_ruc_dsr[fl_ix] & fl_mask[i]) == 0))
				{
					if((cons[incrs] == 2)&&(rir_hc == 1))
					{
						if(gc_riruc[fl_ix] == 0)	
							gc_ruc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
					}			
					else
						gc_ruc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
				}
			}
	    }
  	}


	// Lobby up hall call request
	if (cons[LbyUpReq] != 0)
	{
		if ((cons[sercom] & 0x20) == 0)
		{			// no serial hc boards
			if (cons[epgrp] != 0)	   // determine if input is being used.
				i = cons[grtopf]+1;
			else
				i = cons[grtopf];

			fl_ix = (i-1)/32;
			if ((gc_uc[fl_ix] & fl_mask[i]) != 0)// check to see if lobby up hall call pushed
			{
				lby_up_req = 1;
				gc_uc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
				any_hall_call = 1;
			}
			else
			{
				if (lby_up_req == 0)
		 			lby_up_assign = 0;
				if ((gc_uc_canc[fl_ix] & fl_mask[i]) != 0)
					gc_uc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
		 		gc_uc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			}
		}
		else
		{
			if (gc_luc != 0)
			{
				lby_up_req = 1;
				any_hall_call = 1;
			}
			else
			{
				if (lby_up_req == 0)
					lby_up_assign = 0;
				gc_luc_canc = 0;	
			}
		}
	}

	// Latch the Down Hall Calls

  	for(i=2; i <= cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
      	if (((gc_dc[fl_ix] & fl_mask[i]) != 0) || ((gc_dc_dsr[fl_ix] & fl_mask[i]) != 0))  //check to see if dn hall call pushed
		{
			if (((Grp_HC_Lockouts == 1) && ((fvars[fvhcsecctrl] & 0x02) == 0) && (gc_hsec == 1) && ((gc_hc_sec[fl_ix] & fl_mask[i]) != 0))  ||
					(((ids_dc_sec[fl_ix] & fl_mask[i]) != 0) && ((cons[ids] & 1) != 0)) ||
					(((ui_dc_sec[fl_ix] & fl_mask[i]) != 0) && ((cons[Galileo] & 1) != 0))||
					((gc_hugsec == 1) && ((gc_hugs_sec[fl_ix] & fl_mask[i]) != 0)) ||
					(((fvars[fvdisopphc] & 0x01) != 0) && (sec_ftimer[i] < fvars[fvdisopphctm])))
			{
				if (((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0) || (gc_hugsec == 1))
				{
					gc_dc_canc[fl_ix] |= fl_mask[i];		// secured floor so cancel call
				}
			}
			else
			{
				if ((fvars[fvdisopphc] & 0x01) != 0)
				{	// clear sec_ftimer timer when first up/down hc is pressed
					if (((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0))
						sec_ftimer[i] = 0;
				}
				if ((fvars[fvxasgn] & 0x01) != 0)
				{
					if ((cons[XA_Mask] == 1) && (xa_dc_msk[i] != 0))
					{
						if ((xa_dc_ots[fl_ix] & fl_mask[i]) != 0)	  // cannot assign
							gc_xdct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
						else if (xa_dc_msk[i] == 1)
							g_dn_hall_call[fl_ix][0] |= fl_mask[i];
						else if (xa_dc_msk[i] == 2)
							gc_xdct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
						else if (xa_dc_msk[i] == 3)
						{
							xasgn = (g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0;
							asgn = (gc_xdcl[fl_ix] & fl_mask[i]) == 0;
							if (asgn == true)
								g_dn_hall_call[fl_ix][0] |= fl_mask[i];
							if (xasgn == true)
								gc_xdct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
						}
					}
					else if ((xassign == 0) && ((gc_xdcl[fl_ix] & fl_mask[i]) == 0) && ((xa_dc_ots[fl_ix] & fl_mask[i]) == 0))				// no cross assignment
						g_dn_hall_call[fl_ix][0] |= fl_mask[i];
					else if ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0)	// call is not already set in system
						gc_xdct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor
				}
				else 				// no cross assignment
				{
					if ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0)
						place_dnhc_in_que(i);		// call being latched so put hall call in queue
					g_dn_hall_call[fl_ix][0] |= fl_mask[i];
				}
			}

			gc_dc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
			any_hall_call = 1;
	    }
      	else
      	{
			if((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0)
			{
				dctim[i] = 0;
				g_asign_dn[i] = 0;
				remove_dnhc_from_que(i);
			}

			if ((fvars[fvxasgn] & 0x02) != 0)
				g_dn_hall_call[fl_ix][0] &= ~fl_mask[i];

			if ((gc_hugsec == 1) && ((gc_hugs_sec[fl_ix] & fl_mask[i]) != 0) && ((gc_dc_canc[fl_ix] & fl_mask[i]) != 0))
				g_dn_hall_call[fl_ix][0] &= ~fl_mask[i];

			if ((gc_dc_canc[fl_ix] & fl_mask[i]) != 0)
				gc_dc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag

			if ((cons[Dual_Ser_Riser] == 0) || ((gc_dc_dsr[fl_ix] & fl_mask[i]) == 0))
			{
				if((cons[incrs] == 2)&&(ir_hc == 1))
				{
					if(gc_irdc[fl_ix] == 0)
						gc_dc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
				}
				else
					gc_dc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			}
			gc_xdct[fl_ix] &= ~fl_mask[i];
      	}
      	if(grtop_rfl != 0)
      	{
	      	if (((gc_rdc[fl_ix] & fl_mask[i]) != 0) || ((gc_rdc_dsr[fl_ix] & fl_mask[i]) != 0))  //check to see if dn hall call pushed
			{
				if (((Grp_HC_Lockouts == 1) && ((fvars[fvhcsecctrl] & 0x02) == 0) && (gc_rhsec == 1) && ((gc_rhc_sec[fl_ix] & fl_mask[i]) != 0)) ||
						(((ids_rdc_sec[fl_ix] & fl_mask[i]) != 0) && ((cons[ids] & 1) != 0)) ||
						(((ui_rdc_sec[fl_ix] & fl_mask[i]) != 0) && ((cons[Galileo] & 1) != 0)) ||
						((gc_rhugsec == 1) && ((gc_rhugs_sec[fl_ix] & fl_mask[i]) != 0)) ||
						(((fvars[fvdisopphc] & 0x02) != 0) && (sec_rtimer[i] < fvars[fvdisopphctm])))
				{
					if (((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) == 0) || (gc_rhugsec == 1))
						gc_rdc_canc[fl_ix] |= fl_mask[i];		// call is secured so cancel
				}
				else
				{
					if ((fvars[fvdisopphc] & 0x02) != 0)
					{	// clear sec_rtimer timer when first rear up/down hc is pressed
						if (((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((g_upr_hall_call[fl_ix][0] & fl_mask[i]) == 0))
							sec_rtimer[i] = 0;
					}
					if ((fvars[fvxasgn] & 0x04) != 0)
					{
						if ((xassign == 0) && ((gc_xrdcl[fl_ix] & fl_mask[i]) == 0) && ((xa_drc_ots[fl_ix] & fl_mask[i]) == 0))				// no cross assignment
						{
							g_dnr_hall_call[fl_ix][0] |= fl_mask[i];
						}	
						else if ((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) == 0)	// call is not already set in system
						{
							gc_xrdct[fl_ix] |= fl_mask[i];			// set cross assignment transfer flag for floor	
						}
					}
					else
					{
						if ((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) == 0)
							place_dnhc_in_que(i | 0x80);		// call is being latched so put hall call in queue
						g_dnr_hall_call[fl_ix][0] |= fl_mask[i];
					}
				}
				gc_rdc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
				any_hall_call = 1;
		    }
	      	else
	      	{
				if((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) == 0)
				{
					dcrtim[i] = 0;
					g_asign_dnr[i] = 0;
					remove_dnhc_from_que(i | 0x80);
				}

				if ((fvars[fvxasgn] & 0x02) != 0)
					g_dnr_hall_call[fl_ix][0] &= ~fl_mask[i];

				if ((gc_rhugsec == 1) && ((gc_rhugs_sec[fl_ix] & fl_mask[i]) != 0) && ((gc_rdc_canc[fl_ix] & fl_mask[i]) != 0))
					g_dnr_hall_call[fl_ix][0] &= ~fl_mask[i];

				if ((gc_rdc_canc[fl_ix] & fl_mask[i]) != 0)
					gc_rdc_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
				
				if ((cons[Dual_Ser_Riser] == 0) || ((gc_rdc_dsr[fl_ix] & fl_mask[i]) == 0))
				{
					if((cons[incrs] == 2)&&(rir_hc == 1))
					{
						if(gc_rirdc[fl_ix] == 0)
							gc_rdc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
					}
					else
				 		gc_rdc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
				}
	      	}
      	}
  	}
}

// *********************************************************
// This routine assigns the hall call to the appropriate car
// *********************************************************

void assign_hallcall (void)
{
  	int16 i, k, assignment=0,rear_assignment=0;
	int16 keepcall;
	int16 fl_ix=0;

	int16 nmb_xa_hc = 0;
	int16 nmb_hc = 0;

	static int16 old_nmb_xa_hc;
	static int16 old_nmb_hc;

	nmb_asg_uc = 0;
	sum_uc_eta = 0;
	nmb_asg_dc = 0;
	sum_dc_eta = 0;

	set_master_hc_sec();

  	for(i=1; i<=cons[grtopf]; i++)        // decide who will take the calls
  	{
		fl_ix = (i-1)/32;
		xa_uc_ots[fl_ix] &= ~fl_mask[i];
		xa_dc_ots[fl_ix] &= ~fl_mask[i];
		xa_urc_ots[fl_ix] &= ~fl_mask[i];
		xa_drc_ots[fl_ix] &= ~fl_mask[i];
	    if(i < cons[grtopf])
	    {
	      	if((g_up_hall_call[fl_ix][0] & fl_mask[i]) != 0)  // there is an up call
	      	{
				assignment = 0;
				for(k=cons[nmbcars]; k>=1; k--)
				{       // find the first elegible car to answer the call
				  	if((Car_ots[k] == 0) && (ucmsk[k][i] == 1) && 
				  		(((master_uhc_carsec[fl_ix][k] & fl_mask[i]) == 0) || ((cons[SecFlCfg] == 4) && (has_assignment[k] == 0) && 
				  																(is_uphc_in_que(i) == 1) && ((fvars[fvhcasgnsectype] & 0x01) != 0)))	&&
  						((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0) && (Car_on_TR[k] == 0) &&(((lby_up_req_car[k] & 0x02) == 0) || (cons[LbyUpReq] == 0) || (i != lobby_fl)) )
						assignment = k;
				}
				for(k=(assignment + 1); k<=cons[nmbcars]; k++)
				{       // find the car that can answer the call the fastest
				  	if((Car_ots[k] == 0) && (ucmsk[k][i] == 1) && 
				  		(((master_uhc_carsec[fl_ix][k] & fl_mask[i]) == 0) || ((cons[SecFlCfg] == 4) && (has_assignment[k] == 0) && 
				  																(is_uphc_in_que(i) == 1) && ((fvars[fvhcasgnsectype] & 0x01) != 0))) && 
				  		((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0) && (Car_on_TR[k] == 0) && (((lby_up_req_car[k] & 0x02) == 0) || (cons[LbyUpReq] == 0) || (i != lobby_fl)) )
				  	{     // if it is inservice and there is an opening compare eta	
						if (make_assignment(assignment,k,i,1,0) == 1)
							assignment = k;
				  	}
				}
				// security option to force assignment calls to specific elevators
				if ((car_asgn_sec == 1) && (cons[Car_Asgn_Fl] == 1) && (car_asgn_uc[i] != 0) && (Car_ots_nbp[car_asgn_uc[i]] == 0))
				{
					assignment = car_asgn_uc[i];
				}
				else
				{
					if ((assignment != g_asign_up[i]) && (g_asign_up[i] != 0)  && (ucmsk[g_asign_up[i]][i] == 1))
					{
						if ((master_uhc_carsec[fl_ix][g_asign_up[i]] & fl_mask[i]) == 0)
						{
							if ((Car_on_SR[g_asign_up[i]] == 0) && ((g_asign_up[i] != ir_car) || (cons[incrs] == 3)) && (Car_ots[g_asign_up[i]] == 0) && (Car_on_TR[g_asign_up[i]] == 0))  // assigning to a diffrent car
							{             // checking to see if its the old cars next stop 
								if (keep_assignment(g_asign_up[i],assignment,i,1,0) == 1)
									assignment = g_asign_up[i];	   // if it was the old cars next stop let old car take it
							}
						}
						else if ((cons[SecFlCfg] == 4) && (hc_car_sec[g_asign_up[i]] == 1) && (is_uphc_in_que(i) == 1) && ((fvars[fvhcasgnsectype] & 0x01) != 0))
						{
							assignment = g_asign_up[i];
						}
					}

					 
					// Then next up car must get the up hall call at the lobby
					if((i == lobby_fl) && (ncu_car >= 1) && (ncu_car <= cons[nmbcars]))
					{	 // Call at the lobby floor
						if ((Car_ots[ncu_car] == 0) && (ucmsk[ncu_car][i] == 1) && ((master_uhc_carsec[fl_ix][ncu_car] & fl_mask[i]) == 0) && ((ncu_car != ir_car) || (cons[incrs] == 3))  && (Car_on_SR[k] == 0) && (Car_on_TR[k] == 0) 
								&& (((lby_up_req_car[k] & 0x02) == 0) || (cons[LbyUpReq] == 0)))
						{		// ncu car can take the call
							assignment = ncu_car;
						}
					}
				}
				for(k=1; k<=cons[nmbcars]; k++)
				{
			    	if(k == assignment)
						g_up_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
			    	else if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_iruc[fl_ix] & fl_mask[i]) == 0)))
						g_up_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
				}
				if ((assignment == 0) || (assignment > cons[nmbcars]))  
				{
					keepcall = 0;
					for(k=cons[nmbcars]; k>=1; k--)
					{       // find the first elegible car to answer the call
					  	if((grp_Car_Stat[k].g_servf == s_LWBP) && (ucmsk[k][i] == 1) && ((master_uhc_carsec[fl_ix][k] & fl_mask[i]) == 0) && ((k != ir_car) || (cons[incrs] == 3))  && (Car_on_SR[k] == 0) && (Car_on_TR[k] == 0)
					  				&& (((lby_up_req_car[k] & 0x02) == 0) || (cons[LbyUpReq] == 0) || (i != lobby_fl)) )
							keepcall = 1;	 // There is a car capible of taking the call but is on bypass so keep the call
	
						if ((cons[SecFlCfg] == 4) && (hc_car_sec[k] == 1) && (ucmsk[k][i] == 1) && ((master_uhc_carsec[fl_ix][k] & fl_mask[i]) != 0))
							keepcall = 1;		// car masked from special security so keep call

					}

					if ((ir_active == 1) && ((fvars[fvirctl] & 0x01) != 0) && (cons[incrs] == 1))
					{			// Car on ir but will switch back to take assignment
						if ((ir_car != 0) && (ucmsk[ir_car][i] == 1) && ((master_uhc_carsec[fl_ix][ir_car] & fl_mask[i]) == 0) && (Car_ots_nbp[ir_car] == 0))
							keepcall = 1;
					}

					if (assignment > cons[nmbcars])
					{
						keepcall = 0;
						assignment = 0;
					}

					if (keepcall == 0)
					{
			    		g_up_hall_call[fl_ix][0] &= ~fl_mask[i];// cancel call from group
					}
					

			    	for(k=1; k<=cons[nmbcars]; k++)
			    	{
						if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_iruc[fl_ix] & fl_mask[i]) == 0)))
							g_up_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1
			    	}
					if (keepcall == 0)
					{
						gc_uc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//						gc_uc[fl_ix] &= ~fl_mask[i];			// cancel call input
					}
				}
				g_asign_up[i] = assignment;

				if (g_asign_up[i] != 0)
				{
					sum_uc_eta += car_up_eta[g_asign_up[i]][i];
					nmb_asg_uc++;
				}
				else
					xa_uc_ots[fl_ix] |= fl_mask[i];
      		}
      		else
      		{
				for(k=1; k<=cons[nmbcars]; k++)
				{
					if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_iruc[fl_ix] & fl_mask[i]) == 0)))
		    	  		g_up_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
				}
      		}
    	}
    	if(i > 1)
    	{
		  	if((g_dn_hall_call[fl_ix][0] & fl_mask[i]) != 0)      // there is an down call
		  	{
				assignment = 0;
				for(k=cons[nmbcars]; k>=1; k--)
				{       // find the first elegible car to answer the call
			  		if((Car_ots[k] == 0) && (dcmsk[k][i] == 1) && 
			  			(((master_dhc_carsec[fl_ix][k] & fl_mask[i]) == 0) || ((cons[SecFlCfg] == 4) && (has_assignment[k] == 0) && 
			  																	(is_dnhc_in_que(i) == 1) && ((fvars[fvhcasgnsectype] & 0x02) != 0))) && 
			  			((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0) && (Car_on_TR[k] == 0))
			  		{
						assignment = k;
			  		}
			  		hc_dbg1 = Car_ots[k];
			  		hc_dbg2 = dcmsk[k][i];
			  		hc_dbg3 = master_dhc_carsec[fl_ix][k];
				}
				for(k=(assignment + 1); k<=cons[nmbcars]; k++)
				{       // find the car that can answer the call the fastest
			  		if((Car_ots[k] == 0) && (dcmsk[k][i] == 1) && 
			  			(((master_dhc_carsec[fl_ix][k] & fl_mask[i]) == 0) || ((cons[SecFlCfg] == 4) && (has_assignment[k] == 0) && 
			  																	(is_dnhc_in_que(i) == 1) && ((fvars[fvhcasgnsectype] & 0x02) != 0))) && 
			  			((k != ir_car) || (cons[incrs] == 3))  && (Car_on_SR[k] == 0) && (Car_on_TR[k] == 0))
			  		{     // if it is inservice and there is an opening compare eta
						if (make_assignment(assignment,k,i,2,0) == 1)
							assignment = k;
			  		}
				}

				// security option to force assignment calls to specific elevators
				if((car_asgn_sec == 1) && (cons[Car_Asgn_Fl] == 1) && (car_asgn_dc[i] != 0) && (Car_ots_nbp[car_asgn_dc[i]] == 0))
				{
					assignment = car_asgn_dc[i];
				}
				else
				{
					if ((assignment != g_asign_dn[i]) && (g_asign_dn[i] != 0)  && (dcmsk[g_asign_dn[i]][i] == 1)) 
					{
						if ((master_dhc_carsec[fl_ix][g_asign_dn[i]] & fl_mask[i]) == 0)
						{
							if ((Car_on_SR[g_asign_dn[i]] == 0) && ((g_asign_dn[i] != ir_car) || (cons[incrs] == 3)) && (Car_ots[g_asign_dn[i]] == 0) && (Car_on_TR[g_asign_dn[i]] == 0))  // assigning to a diffrent car
							{             // checking to see if its the old cars next stop
								if (keep_assignment(g_asign_dn[i],assignment,i,2,0) == 1)
									assignment = g_asign_dn[i];	   // if it was the old cars next stop let old car take it
							}
						}
						else if ((cons[SecFlCfg] == 4) && (hc_car_sec[g_asign_dn[i]] == 1) && (is_dnhc_in_que(i) == 1) && ((fvars[fvhcasgnsectype] & 0x02) != 0))
								assignment = g_asign_dn[i];
					}
				}

				for(k=1; k<=cons[nmbcars]; k++)
				{
			    	if(k == assignment)
						g_dn_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
			    	else if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_irdc[fl_ix] & fl_mask[i]) == 0)))
						g_dn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel the other cars
				}
				if ((assignment == 0) || (assignment > cons[nmbcars]))  
				{
					keepcall = 0;
					for(k=cons[nmbcars]; k>=1; k--)
					{       // find the first elegible car to answer the call
					  	if((grp_Car_Stat[k].g_servf == s_LWBP) && (dcmsk[k][i] == 1) && ((master_dhc_carsec[fl_ix][k] & fl_mask[i]) == 0) && ((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0) && (Car_on_TR[k] == 0))
							keepcall = 1;	 // There is a car capible of taking the call but is on bypass so keep the call

						if ((cons[SecFlCfg] == 4) && (hc_car_sec[k] == 1) && (dcmsk[k][i] == 1) && ((master_dhc_carsec[fl_ix][k] & fl_mask[i]) != 0)) 
						{
							keepcall = 1;	 // car masked from special security so keep call
						}
					}

					if ((ir_active == 1) && ((fvars[fvirctl] & 0x01) != 0) && (cons[incrs] == 1))
					{			// Car on ir but will switch back to take assignment
						if ((ir_car != 0) && (dcmsk[ir_car][i] == 1) && ((master_dhc_carsec[fl_ix][ir_car] & fl_mask[i]) == 0) && (Car_ots_nbp[ir_car] == 0))
							keepcall = 1;
					}

					if (assignment > cons[nmbcars])
					{
						keepcall = 0;
						assignment = 0;
					}


					if (keepcall == 0)
					{
			   		 	g_dn_hall_call[fl_ix][0] &= ~fl_mask[i];// cancel call from group
					}

			    	for(k=1; k<=cons[nmbcars]; k++)
			    	{
						if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_irdc[fl_ix] & fl_mask[i]) == 0)))
							g_dn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1
			    	}

					if (keepcall == 0)
					{
						gc_dc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//						gc_dc[fl_ix] &= ~fl_mask[i];			// cancel call input
					}
				}
				else if (g_asign_dn[i] == 0)
				{			// Call was not previously assigned
					if ((i > lobby_fl) && (nmb_dc_above_lby < 1638))		// Keep int16 positive (65535/2) / 20 samples = 1638
						nmb_dc_above_lby++;
				}
				g_asign_dn[i] = assignment;

				if (g_asign_dn[i] != 0)
				{
					sum_dc_eta += car_dn_eta[g_asign_dn[i]][i];
					nmb_asg_dc++;
				}
				else
					xa_dc_ots[fl_ix] |= fl_mask[i];
		  	}
		  	else
		  	{
				for(k=1; k<=cons[nmbcars]; k++)
				{
			      	if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_irdc[fl_ix] & fl_mask[i]) == 0)))
				      	g_dn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
				}
		  	}
	    }
			  
	    if(grtop_rfl != 0)
	    {
		  	if(i < cons[grtopf])
		  	{
		      	if((g_upr_hall_call[fl_ix][0] & fl_mask[i]) != 0)  // there is an up call
		      	{
					rear_assignment = 0;
					for(k=cons[nmbcars]; k>=1; k--)
					{       // find the first elegible car to answer the call
				  		if ((Car_ots[k] == 0) && (rucmsk[k][i] == 1) && 
				  			(((master_ruhc_carsec[fl_ix][k] & fl_mask[i]) == 0) || ((cons[SecFlCfg] == 4) && (has_assignment[k] == 0) && 
				  																	(is_uphc_in_que(i | 0x80) == 1) && ((fvars[fvhcasgnsectype] & 0x04) != 0))) &&
				  			((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0))
							rear_assignment = k;
					}
					for(k=(rear_assignment + 1); k<=cons[nmbcars]; k++)
					{       // find the car that can answer the call the fastest
				  		if((Car_ots[k] == 0) && (rucmsk[k][i] == 1) && 
				  			(((master_ruhc_carsec[fl_ix][k] & fl_mask[i]) == 0) || ((cons[SecFlCfg] == 4) && (has_assignment[k] == 0) && 
				  																	(is_uphc_in_que(i | 0x80) == 1) && ((fvars[fvhcasgnsectype] & 0x04) != 0))) &&
				  			((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0))
				  		{     // if it is inservice and there is an opening compare eta
					  		if (make_assignment(rear_assignment,k,i,1,1)	== 1)
								rear_assignment = k;
				  		}
					}

					// security option to force assignment calls to specific elevators
					if((car_asgn_sec == 1) && (cons[Car_Asgn_Fl] == 1) && (car_asgn_ruc[i] != 0) && (Car_ots_nbp[car_asgn_ruc[i]] == 0))
					{
						rear_assignment = car_asgn_ruc[i];
					}
					else
					{
						if((rear_assignment != g_asign_upr[i]) && (g_asign_upr[i] != 0)  && (rucmsk[g_asign_upr[i]][i] == 1))
						{
							if ((master_ruhc_carsec[fl_ix][g_asign_upr[i]] & fl_mask[i]) == 0)
							{
								if ((Car_on_SR[g_asign_upr[i]] == 0) && ((g_asign_upr[i] != ir_car) || (cons[incrs] == 3)) && (Car_ots[g_asign_upr[i]] == 0))  // assigning to a diffrent car
								{             // checking to see if its the old cars next stop
									if (keep_assignment(g_asign_upr[i],rear_assignment,i,1,1) == 1) 
										rear_assignment = g_asign_upr[i];	 // if it was the old cars next stop let old car take it
								}
							}
							else if ((cons[SecFlCfg] == 4) && (hc_car_sec[g_asign_upr[i]] == 1) && (is_uphc_in_que(i | 0x80) == 1) && ((fvars[fvhcasgnsectype] & 0x04) != 0))
							{
								rear_assignment = g_asign_upr[i];
							}
						}
					}

					for(k=1; k<=cons[nmbcars]; k++)
					{
				    	if(k == rear_assignment)
							g_upr_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
				    	else if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_riruc[fl_ix] & fl_mask[i]) == 0)))
							g_upr_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
					}

					if ((rear_assignment == 0) || (rear_assignment > cons[nmbcars]))  // ||(car_eta[rear_assignment][i+(g_eta_1u-1)] == 0xff))
					{
						keepcall = 0;
						for(k=cons[nmbcars]; k>=1; k--)
						{       // find the first elegible car to answer the call
						  	if((grp_Car_Stat[k].g_servf == s_LWBP) && (rucmsk[k][i] == 1) && ((master_ruhc_carsec[fl_ix][k] & fl_mask[i]) == 0) && ((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0))
								keepcall = 1;	 // There is a car capible of taking the call but is on bypass so keep the call
							
							if ((cons[SecFlCfg] == 4) && (hc_car_sec[k] == 1) && (rucmsk[k][i] == 1) && ((master_ruhc_carsec[fl_ix][k] & fl_mask[i]) != 0))
								keepcall = 1;	// car masked from special security
						}

						if ((ir_active == 1) && ((fvars[fvirctl] & 0x01) != 0) && (cons[incrs] == 1))
						{			// Car on ir but will switch back to take assignment
							if ((ir_car != 0) && (rucmsk[ir_car][i] == 1) && ((master_ruhc_carsec[fl_ix][ir_car] & fl_mask[i]) == 0) && (Car_ots_nbp[ir_car] == 0))
								keepcall = 1;
						}

						if (rear_assignment > cons[nmbcars])
						{
							keepcall = 0;
							rear_assignment = 0;
						}

						if (keepcall == 0)
					    	g_upr_hall_call[fl_ix][0] &= ~fl_mask[i];// cancel call from group
				    	for(k=1; k<=cons[nmbcars]; k++)
				    	{
							if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_riruc[fl_ix] & fl_mask[i]) == 0)))
								g_upr_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1
				    	}
						if (keepcall == 0)
						{
							if (gc_hc_dev != 0)
							{
								gc_ruc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//								gc_ruc[fl_ix] &= ~fl_mask[i];			// cancel call input
					  		}
						}
					}
					g_asign_upr[i] = rear_assignment;
					if (g_asign_upr[i] != 0)
					{
						sum_uc_eta += car_up_eta[g_asign_upr[i]][i];
						nmb_asg_uc++;
					}
					else
						xa_urc_ots[fl_ix] |= fl_mask[i];
		      	}
		      	else
		      	{
					for(k=1; k<=cons[nmbcars]; k++)
					{
						if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_riruc[fl_ix] & fl_mask[i]) == 0)))
				      		g_upr_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
					}
		      	}
		    }
		    if(i > 1)
		    {
			  	if((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) != 0)      // there is an down call
			  	{
					rear_assignment = 0;
					for(k=cons[nmbcars]; k>=1; k--)
					{       // find the first elegible car to answer the call
				  		if((Car_ots[k] == 0) && (rdcmsk[k][i] == 1) && 
				  			(((master_rdhc_carsec[fl_ix][k] & fl_mask[i]) == 0) || ((cons[SecFlCfg] == 4) && (has_assignment[k] == 0) && 
				  																	(is_dnhc_in_que(i | 0x80) == 1) && ((fvars[fvhcasgnsectype] & 0x08) != 0))) && 
				  			((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0))
							rear_assignment = k;
					}
					for(k=(rear_assignment + 1); k<=cons[nmbcars]; k++)
					{       // find the car that can answer the call the fastest
				  		if((Car_ots[k] == 0) && (rdcmsk[k][i] == 1) && 
				  			(((master_rdhc_carsec[fl_ix][k] & fl_mask[i]) == 0) || ((cons[SecFlCfg] == 4) && (has_assignment[k] == 0) && 
				  																	(is_dnhc_in_que(i | 0x80) == 1) && ((fvars[fvhcasgnsectype] & 0x08) != 0))) && 
				  			((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0))
				  		{     // if it is inservice and there is an opening compare eta
							if (make_assignment(rear_assignment,k,i,2,1) == 1)
								rear_assignment = k;
				  		}
					}

					// security option to force assignment calls to specific elevators
					if((car_asgn_sec == 1) && (cons[Car_Asgn_Fl] == 1) && (car_asgn_rdc[i] != 0) && (Car_ots_nbp[car_asgn_rdc[i]] == 0))
					{
						rear_assignment = car_asgn_rdc[i];
					}
					else
					{
						if ((rear_assignment != g_asign_dnr[i]) && (g_asign_dnr[i] != 0) && (rdcmsk[g_asign_dnr[i]][i] == 1))
						{
							if ((master_rdhc_carsec[fl_ix][g_asign_dnr[i]] & fl_mask[i]) == 0)
							{
								if ((Car_on_SR[g_asign_dnr[i]] == 0) && ((g_asign_dnr[i] != ir_car) || (cons[incrs] == 3)) && (Car_ots[g_asign_dnr[i]] == 0)) // assigning to a diffrent car
								{             // checking to see if its the old cars next stop
									if (keep_assignment(g_asign_dnr[i],rear_assignment,i,2,1) == 1)
										rear_assignment = g_asign_dnr[i];  // if it was the old cars next stop let old car take it
								}
							}
							else if ((cons[SecFlCfg] == 4) && (hc_car_sec[g_asign_dnr[i]] == 1) && (is_dnhc_in_que(i | 0x80) == 1) && ((fvars[fvhcasgnsectype] & 0x08) != 0))
							{
								rear_assignment = g_asign_dnr[i];
							}
						}
					}
					for(k=1; k<=cons[nmbcars]; k++)
					{
				    	if(k == rear_assignment)
							g_dnr_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
				    	else if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_rirdc[fl_ix] & fl_mask[i]) == 0)))
							g_dnr_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel the other cars
					}
					if ((rear_assignment == 0) || (rear_assignment > cons[nmbcars])) // || (grp_Car_Stat[rear_assignment][i+2] == 0xff))
					{
						keepcall = 0;
						for(k=cons[nmbcars]; k>=1; k--)
						{       // find the first elegible car to answer the call
						  	if((grp_Car_Stat[k].g_servf == s_LWBP) && (rdcmsk[k][i] == 1) && ((master_rdhc_carsec[fl_ix][k] & fl_mask[i]) == 0) && ((k != ir_car) || (cons[incrs] == 3)) && (Car_on_SR[k] == 0))
								keepcall = 1;	 // There is a car capible of taking the call but is on bypass so keep the call
							
							if ((cons[SecFlCfg] == 4) && (hc_car_sec[k] == 1) && (rdcmsk[k][i] == 1) && ((master_rdhc_carsec[fl_ix][k] & fl_mask[i]) != 0))
								keepcall = 1;	// car masked from special security

						}

						if ((ir_active == 1) && ((fvars[fvirctl] & 0x01) != 0) && (cons[incrs] == 1))
						{			// Car on ir but will switch back to take assignment
							if ((ir_car != 0) && (rdcmsk[ir_car][i] == 1) && ((master_rdhc_carsec[fl_ix][ir_car] & fl_mask[i]) == 0) && (Car_ots_nbp[ir_car] == 0))
								keepcall = 1;
						}

						if (rear_assignment > cons[nmbcars])
						{
							keepcall = 0;
							rear_assignment = 0;
						}

						if (keepcall == 0)
					    	g_dnr_hall_call[fl_ix][0] &= ~fl_mask[i];// cancel call from group
				    	for(k=1; k<=cons[nmbcars]; k++)
				    	{
							if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_rirdc[fl_ix] & fl_mask[i]) == 0)))
								g_dnr_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1
				    	}
						if (keepcall == 0)
						{
							if (gc_hc_dev != 0)
							{
								gc_rdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//								gc_rdc[fl_ix] &= ~fl_mask[i];			// cancel call input
					  		}
						}
					}
					g_asign_dnr[i] = rear_assignment;
					if (g_asign_dnr[i] != 0)
					{
						sum_dc_eta += car_dn_eta[g_asign_dnr[i]][i];
						nmb_asg_dc++;
					}
					else
						xa_drc_ots[fl_ix] |= fl_mask[i];					
			  	}
			  	else
			  	{
					for(k=1; k<=cons[nmbcars]; k++)
					{
						if ((k != ir_car) || ((cons[incrs] == 3) && (k == ir_car) && ((gc_rirdc[fl_ix] & fl_mask[i]) == 0)))
						  	g_dnr_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
					}
			  	}
		  	}
		}
  	}

	// Cross assignment check
	if (((fvars[fvxasgn] & 0x05) != 0) && (gc_nca_xasgn == 0))	// cross assignment enabled and NCA (No car available from old group) input is off
	{
		if (fvars[fvxacars] == 0)
		{
			if (nmb_asg_dc > 0)
				avg_dc_eta = sum_dc_eta/nmb_asg_dc;
			else
				avg_dc_eta = 0;
			
			if (nmb_asg_uc > 0)
				avg_uc_eta = sum_uc_eta/nmb_asg_uc;
			else
				avg_uc_eta = 0;
			
			if ((avg_dc_eta > fvars[fvxeta]) || (avg_uc_eta > fvars[fvxeta])) 
				xassign = 1;	   // Average ETA time too great so xassign new calls
			else
				xassign = 0;
		}
		else
		{
		  	for(i=1; i<=cons[grtopf]; i++)        // decide who will take the calls
			{
				fl_ix = (i-1)/32;
				
				if ((fvars[fvxasgn] & 0x01) != 0)
				{
					if (((gc_xucl[fl_ix] & fl_mask[i]) != 0) || ((gc_xdcl[fl_ix] & fl_mask[i]) != 0))
						nmb_xa_hc++;
					if (((g_up_hall_call[fl_ix][0] & fl_mask[i]) != 0) || ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) != 0))
						nmb_hc++;
				}
				
				if ((fvars[fvxasgn] & 0x04) != 0)
				{
					if (((gc_xrucl[fl_ix] & fl_mask[i]) != 0) || ((gc_xrdcl[fl_ix] & fl_mask[i]) != 0))
						nmb_xa_hc++;
					if (((g_upr_hall_call[fl_ix][0] & fl_mask[i]) != 0) || ((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) != 0))
						nmb_hc++;
				}

			}
			
			if (nmb_cars_in_svc != 0)
			{
				if ((60 * nmb_hc/nmb_cars_in_svc) < (60 * nmb_xa_hc/fvars[fvxacars])) // used 60 to get a better integer division
				{
					xassign = 0;
				}
				else if ((60 * nmb_hc/nmb_cars_in_svc) == (60 * nmb_xa_hc/fvars[fvxacars])) // used 60 to get a better integer division
				{
					if ((nmb_hc > old_nmb_hc) || (nmb_xa_hc > old_nmb_xa_hc))
					{
						if (xassign == 0)
							xassign = 1;
						else if (xassign == 1)
							xassign = 0;
					}
				}
				else
					xassign = 1;
			}
			else
				xassign = 1;
			
			old_nmb_hc = nmb_hc;
			old_nmb_xa_hc = nmb_xa_hc;
		}
	}
	else
		xassign = 0;
	

			
	// Lobby up hall call request

	if ((cons[LbyUpReq] != 0) && (lby_up_req == 1))
	{
		if (lobby_fl != 0)
			fl_ix = (lobby_fl-1)/32;
		else
			fl_ix = 0;

		assignment = 0;
		for(k=cons[nmbcars]; k>=1; k--)
		{       // find the first elegible car to answer the call
		  	if((Car_ots[k] == 0) && (ucmsk[k][lobby_fl] == 1) && ((master_uhc_carsec[fl_ix][k] & fl_mask[lobby_fl]) == 0) && (k != ir_car) && ((lby_up_req_car[k] & 0x01) != 0) && (Car_on_SR[k] == 0))
				assignment = k;
		}
		for(k=(assignment + 1); k<=cons[nmbcars]; k++)
		{       // find the car that can answer the call the fastest
		  	if((Car_ots[k] == 0) && (ucmsk[k][lobby_fl] == 1) && ((master_uhc_carsec[fl_ix][k] & fl_mask[lobby_fl]) == 0)  && (k != ir_car) && ((lby_up_req_car[k] & 0x01) != 0) && (Car_on_SR[k] == 0))
		  	{     // if it is inservice and there is an opening compare eta	
				if (make_assignment(assignment,k,lobby_fl,1,0) == 1)
					assignment = k;
		  	}
		}
		if((assignment != lby_up_assign) && (lby_up_assign != 0) && (ucmsk[lby_up_assign][lobby_fl] == 1) && ((master_uhc_carsec[fl_ix][lby_up_assign] & fl_mask[lobby_fl]) == 0) && (Car_on_SR[lby_up_assign] == 0) 
					&& (lby_up_assign != ir_car) && (Car_ots[lby_up_assign] == 0) )  // assigning to a diffrent car
		{             // checking to see if its the old cars next stop 
			if (keep_assignment(lby_up_assign,assignment,lobby_fl,1,0) == 1)
				assignment = lby_up_assign;
		}// if it was the old cars next stop let old car take it
		// Then next up car must get the up hall call at the lobby
		if((ncu_car >= 1) && (ncu_car <= cons[nmbcars]))
		{	 // Call at the lobby floor
			if ((Car_ots[ncu_car] == 0) && (ucmsk[ncu_car][lobby_fl] == 1)  && ((master_uhc_carsec[fl_ix][ncu_car] & fl_mask[lobby_fl]) == 0) && (Car_on_SR[ncu_car] == 0)
									&& (ncu_car != ir_car) && ((lby_up_req_car[ncu_car] & 0x01) != 0))
			{		// ncu car can take the call
				assignment = ncu_car;
			}
		}
		for(k=1; k<=cons[nmbcars]; k++)
		{
			if(k == assignment)
				g_up_hall_call[fl_ix][k] |= fl_mask[lobby_fl];// asign the fastest car
		}

		if (cons[epgrp] != 0)
			i = cons[grtopf]+1;
		else
			i = cons[grtopf];

		if (assignment == 0)   
		{
			if (gc_hc_dev != 0)
			{
				gc_uc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//				gc_uc[fl_ix] &= ~fl_mask[i];			// cancel call input
			}
		}
		lby_up_assign = assignment;
	}

  	return;
}

//***************************************
// Logic to make the hall call assignment
//***************************************


int16 make_assignment (int16 assigned_car, int16 new_assign_car, int16 fl, int16 dir, int16 front_rear)
{
	int16 eta_new;
	int16 eta_asg;
	int16 eta_new_extra_time;
	int16 eta_asg_extra_time;
	int16 cc_at_fl_asg;
	int16 cc_at_fl_new;
	int16 fl_ix;

	fl_ix = (fl-1)/32;


	if (front_rear == 1)
	{
		cc_at_fl_asg = (((grp_Car_Stat[assigned_car].g_rcc[fl_ix] & fl_mask[fl]) != 0) && (grp_Car_Stat[assigned_car].g_dpref == dir));
		cc_at_fl_new = (((grp_Car_Stat[new_assign_car].g_rcc[fl_ix] & fl_mask[fl]) != 0) && (grp_Car_Stat[new_assign_car].g_dpref == dir));
	}
	else
	{
		cc_at_fl_asg = (((grp_Car_Stat[assigned_car].g_cc[fl_ix] & fl_mask[fl]) != 0) && (grp_Car_Stat[assigned_car].g_dpref == dir));
		cc_at_fl_new = (((grp_Car_Stat[new_assign_car].g_cc[fl_ix] & fl_mask[fl]) != 0) && (grp_Car_Stat[new_assign_car].g_dpref == dir));
	}

	if ((grp_Car_Stat[assigned_car].g_servf == s_ATT) && (grp_Car_Stat[new_assign_car].g_servf != s_ATT) && (fvars[fvatt_eta_pref] != 0))
	{		 // assigned car is on attendant and other car is not so add time to other car
		eta_asg_extra_time = 0;
		eta_new_extra_time = fvars[fvatt_eta_pref];
	}  
	else if ((grp_Car_Stat[assigned_car].g_servf != s_ATT) && (grp_Car_Stat[new_assign_car].g_servf == s_ATT) && (fvars[fvatt_eta_pref] != 0))
	{			 // assigned car is not on attendant and other car is so add time to assigned car
		eta_asg_extra_time = fvars[fvatt_eta_pref];
		eta_new_extra_time = 0;
	}
	else
	{
		eta_new_extra_time = 0;
		eta_asg_extra_time = 0;
	}

	if ((cons[Handicap_HPB] != 0) && (fvars[fvhandwait] != 0))
	{
		if ((handicap_car_floor[fl_ix] & fl_mask[fl]) != 0)
		{
			if  ((grp_Car_Stat[assigned_car].g_control & 0x04) != 0) 
				eta_asg_extra_time += fvars[fvhandwait];
			if ((grp_Car_Stat[new_assign_car].g_control & 0x04) != 0) 
				eta_new_extra_time += fvars[fvhandwait];
		}
	}

	if (dir == 1)
	{
		eta_asg = car_up_eta[assigned_car][fl] + eta_asg_extra_time;
		eta_new = car_up_eta[new_assign_car][fl] + eta_new_extra_time;
	}
	else if (dir == 2)
	{
		eta_asg = car_dn_eta[assigned_car][fl] + eta_asg_extra_time;
		eta_new = car_dn_eta[new_assign_car][fl] + eta_new_extra_time;
	}
	else
	{		// cannot happen
		eta_asg = 0xff;
		eta_new = 0xff;
	}
	
	if (fvars[fvautosvctmout] != 0)
	{
		if (asvtof[assigned_car] == 1)
		{
			eta_asg += asv_penalty_tmr[assigned_car];
			if (eta_asg > 255)
				eta_asg = 255;
		}
		if (asvtof[new_assign_car] == 1)
		{
			eta_new += asv_penalty_tmr[new_assign_car];
			if (eta_new > 255)
				eta_new = 255;
		}
	}

	if (((cc_at_fl_new == 0) && (cc_at_fl_asg == 0)) || ((cc_at_fl_new == 1) && (cc_at_fl_asg == 1)))
	{	// no or both coincident car call so pick the best eta
		if (eta_asg > eta_new)
			return 1;
	}
	else if ((cc_at_fl_new == 1) && (cc_at_fl_asg == 0))
	{
		if ((eta_new - eta_asg) < fvars[fvetacocc])
			return 1;		 // assume eta_new has the larger eta so still assign if diff less than min
	}
	else if ((cc_at_fl_new == 0) && (cc_at_fl_asg == 1))
	{
		if ((eta_asg - eta_new) > (fvars[fvetacocc] + 1))	  // +1 to provide some hysteresis
			return 1;		  // assume asg has the larger eta so don't assign unless diff greater than min
	}
	return 0;
}

//***************************************
// Logic to keep the assigned a hall call
//***************************************

int16 keep_assignment (int16 assigned_car, int16 new_assign_car, int16 fl, int16 dir, int16 front_rear)
{
	int16 eta_new;
	int16 eta_asg;
	int16 eta_new_extra_time;
	int16 eta_asg_extra_time;
	int16 cc_at_fl_asg;
	int16 cc_at_fl_new;
	int16 fl_ix;

	if ((grp_Car_Stat[assigned_car].g_servf == s_ATT) && (grp_Car_Stat[new_assign_car].g_servf != s_ATT) && (fvars[fvatt_eta_pref] != 0))
	{		 // assigned car is on attendant and other car is not so add time to other car
		eta_asg_extra_time = 0;
		eta_new_extra_time = fvars[fvatt_eta_pref];
	}  
	else if ((grp_Car_Stat[assigned_car].g_servf != s_ATT) && (grp_Car_Stat[new_assign_car].g_servf == s_ATT) && (fvars[fvatt_eta_pref] != 0))
	{			 // assigned car is not on attendant and other car is so add time to assigned car
		eta_asg_extra_time = fvars[fvatt_eta_pref];
		eta_new_extra_time = 0;
	}
	else
	{
		eta_new_extra_time = 0;
		eta_asg_extra_time = 0;
	}

	fl_ix = (fl-1)/32;

	if ((cons[Handicap_HPB] != 0) && (fvars[fvhandwait] != 0))
	{
		if ((handicap_car_floor[fl_ix] & fl_mask[fl]) != 0)
		{
			if  ((grp_Car_Stat[assigned_car].g_control & 0x04) != 0) 
				eta_asg_extra_time += fvars[fvhandwait];
			if ((grp_Car_Stat[new_assign_car].g_control & 0x04) != 0) 
				eta_new_extra_time += fvars[fvhandwait];
		}
	}

	if (dir == 1)
	{
		eta_asg = car_up_eta[assigned_car][fl] + eta_asg_extra_time;
		eta_new = car_up_eta[new_assign_car][fl] + eta_new_extra_time;
	}
	else if (dir == 2)
	{
		eta_asg = car_dn_eta[assigned_car][fl] + eta_asg_extra_time;
		eta_new = car_dn_eta[new_assign_car][fl] + eta_new_extra_time;
	}
	else
	{		// cannot happen
		eta_asg = 0xff;
		eta_new = 0xff;
	}

	if (fvars[fvautosvctmout] != 0)
	{
		if (asvtof[assigned_car] == 1)
		{
			eta_asg += asv_penalty_tmr[assigned_car];
			if (eta_asg > 255)
				eta_asg = 255;
			if (grp_Car_Stat[assigned_car].g_pos == fl)
				return 0;
		}
		if (asvtof[new_assign_car] == 1)
		{
			eta_new += asv_penalty_tmr[new_assign_car];
			if (eta_new > 255)
				eta_new = 255;
		}
	}

	if (front_rear == 1)
	{	// rear call
		cc_at_fl_asg = (((grp_Car_Stat[assigned_car].g_rcc[fl_ix] & fl_mask[fl]) != 0) && (grp_Car_Stat[assigned_car].g_dpref == dir));
		cc_at_fl_new = (((grp_Car_Stat[new_assign_car].g_rcc[fl_ix] & fl_mask[fl]) != 0) && (grp_Car_Stat[new_assign_car].g_dpref == dir));
	}
	else
	{	// front
		cc_at_fl_asg = (((grp_Car_Stat[assigned_car].g_cc[fl_ix] & fl_mask[fl]) != 0) && (grp_Car_Stat[assigned_car].g_dpref == dir));
		cc_at_fl_new = (((grp_Car_Stat[new_assign_car].g_cc[fl_ix] & fl_mask[fl]) != 0) && (grp_Car_Stat[new_assign_car].g_dpref == dir));
	}

	if ((cons[Handicap_HPB] == 0) || (fvars[fvhandwait] < 255) || (fvars[fvhandwait] == 0) || ((grp_Car_Stat[assigned_car].g_control & 0x04) == 0) || (eta_asg < 255))
	{		// not a condition to re-assign on handicap time penalty
		if ((grp_Car_Stat[assigned_car].g_pos == fl) && ((grp_Car_Stat[assigned_car].g_dpref == dir) || (grp_Car_Stat[assigned_car].g_dpref == 0)))
			return 1;			// the car is at the floor with the correct preference or no preference
		if (grp_Car_Stat[assigned_car].g_nxt_stp == fl)
			return 1;			// this assignment is the next stop for the car
	}

//	if (((cc_at_fl_asg == 0) && (cc_at_fl_new == 0)) || ((cc_at_fl_asg == 1) && (cc_at_fl_new == 1)))
//	{	// both have coincident car calls
//	}
	if ((cc_at_fl_asg == 1) && (cc_at_fl_new == 0))
	{
		if ((eta_asg - eta_new) < (fvars[fvetacocc] + 1))	  // +1 to provide some hysteresis
			return 1;		// difference in eta is not too great
	}
	else if ((cc_at_fl_asg == 0) && (cc_at_fl_new == 1))
	{
		if ((eta_new - eta_asg) < fvars[fvetacocc])
			return 0;		// New assignment has a coincident car call so let it re-assign
	}
	if (((3*eta_asg) < (4*eta_new)) && ((eta_asg - eta_new) < fvars[fvetamin]))
		return 1;	  		// keep eta from jumping around
	return 0;				// else don't keep the assignment
}


// ************************************
// Set group handicap car floor request
// ************************************

void group_handicap_car_floor(void)
{
	int16 i;
	int16 fl_ix;
	int16 ix;

	for (i=1;i<=cons[grtopf];i++)
	{
		fl_ix = (i-1)/32;
		ix = (i-1)/8;

		if (((gpi[ix] & bittbl[(i-1) & 0x07]) != 0) || ((handicap_car_fl_latch[fl_ix] & fl_mask[i]) != 0))
			handicap_car_fl_latch[fl_ix] |= fl_mask[i];

		if ((handicap_car_fl_latch[fl_ix] & fl_mask[i]) != 0)
		{
			if (((g_up_hall_call[fl_ix][0] & fl_mask[i]) != 0) || ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) != 0))
				handicap_car_floor[fl_ix] |= fl_mask[i];
			else if ((handicap_car_floor[fl_ix] & fl_mask[i]) != 0)
			{
				handicap_car_floor[fl_ix] &= ~fl_mask[i];
				handicap_car_fl_latch[fl_ix] &= ~fl_mask[i];
			}
		}
	}
}



//******************************************************
//  Hall call cross assignment routine
//******************************************************

void hall_call_cross_assignment (void)
{			// This routine uses a serial board for the cross assignment i/o's
			// Execution of this routine is every second so that the output to transfer the call is 
			// turned on for at least a second for the exixting group to allow time to latch the call. 

	int16 i;
	int16 fl_ix;

	if (timers[txasgn] > 5)		// every second
	{
		timers[txasgn] = 0;

		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;
			
			if ((fvars[fvxasgn] & 0x01) != 0)
			{				// Front Assignment Only
			    if(i < cons[grtopf])
			    {		// UP CALLS
					if ((gc_xucl[fl_ix] & fl_mask[i]) != 0)
					{		// Have memory of call transfer
						gc_xuc_set[fl_ix] &= ~fl_mask[i];	// turn off the output 
		 				if ((gc_xuc[fl_ix] & fl_mask[i]) == 0)		// call has been answered
						{
							gc_uc_set[fl_ix] &= ~fl_mask[i];		// reset hall call flag
							if ((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0)	 // The group does not have the call assigned so cancel the call
							{
								gc_uc_canc[fl_ix] |= fl_mask[i];
							}
							if (((gc_uc[fl_ix] & fl_mask[i]) == 0) || ((g_up_hall_call[fl_ix][0] & fl_mask[i]) != 0))	  // The acknowledge light has been turned off
							{
								gc_xucl[fl_ix] &= ~fl_mask[i];
								gc_xuct[fl_ix] &= ~fl_mask[i];
							}
						}
						else
						{
		 					if (((gc_uc[fl_ix] & fl_mask[i]) == 0) && ((g_up_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((gc_uc_canc[fl_ix] & fl_mask[i]) == 0))		// the controller light is not on
	 							gc_uc_set[fl_ix] |= fl_mask[i];		// set hall call flag
							else
								gc_uc_set[fl_ix] &= ~fl_mask[i];		// reset hall call flag
						}
					}
					else
					{
						if ((gc_xuct[fl_ix] & fl_mask[i]) != 0)		// Have a call transferred to the old group
						{
			 				if ((gc_xuc[fl_ix] & fl_mask[i]) != 0)		// call input has been turned on from set output
								gc_xucl[fl_ix] |= fl_mask[i];				// latch memory of the call
							gc_xuc_set[fl_ix] |= fl_mask[i];		// turn on an output so the old group can latch the call
						}
			 			else
			 			{
		 					if ((gc_xuc[fl_ix] & fl_mask[i]) != 0)		// the old group has its own call
	 						{
								gc_xucl[fl_ix] |= fl_mask[i];			// set a transfer latch flag
								gc_xuc_set[fl_ix] |= fl_mask[i];		// turn on an output so the old group can latch the call
	 							gc_uc_set[fl_ix] |= fl_mask[i];		// set hall call flag
							}
						}
	 				}
				}

		    	if(i > 1)
				{		// DOWN CALLS
					if ((gc_xdcl[fl_ix] & fl_mask[i]) != 0)
					{		// Have memory of call transfer
						gc_xdc_set[fl_ix] &= ~fl_mask[i];	// turn off the output 
		 				if ((gc_xdc[fl_ix] & fl_mask[i]) == 0)		// call has been answered
						{
							gc_dc_set[fl_ix] &= ~fl_mask[i];		// reset hall call flag
							if ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0)	 // The group does not have the call assigned so cancel the call
								gc_dc_canc[fl_ix] |= fl_mask[i];
							if (((gc_dc[fl_ix] & fl_mask[i]) == 0) || ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) != 0))	  // The acknowledge light has been turned off
							{
								gc_xdcl[fl_ix] &= ~fl_mask[i];
								gc_xdct[fl_ix] &= ~fl_mask[i];
							}
						}
						else
						{
		 					if (((gc_dc[fl_ix] & fl_mask[i]) == 0) && ((g_dn_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((gc_dc_canc[fl_ix] & fl_mask[i]) == 0))		// the controller light is not on
	 							gc_dc_set[fl_ix] |= fl_mask[i];		// set hall call flag
							else
								gc_dc_set[fl_ix] &= ~fl_mask[i];		// reset hall call flag
						}
					}
					else
					{
						if ((gc_xdct[fl_ix] & fl_mask[i]) != 0)		// Have a call transferred to the old group
						{
			 				if ((gc_xdc[fl_ix] & fl_mask[i]) != 0)		// call input has been turned on from set output
								gc_xdcl[fl_ix] |= fl_mask[i];				// latch memory of the call
							gc_xdc_set[fl_ix] |= fl_mask[i];		// turn on an output so the old group can latch the call
						}
			 			else
			 			{
		 					if ((gc_xdc[fl_ix] & fl_mask[i]) != 0)		// the old group has its own call
	 						{
								gc_xdcl[fl_ix] |= fl_mask[i];			// set a transfer latch flag
								gc_xdc_set[fl_ix] |= fl_mask[i];		// turn on an output so the old group can latch the call
	 							gc_dc_set[fl_ix] |= fl_mask[i];		// set hall call flag
							}
						}
	 				}
				}
			}
		
			
			if ((fvars[fvxasgn] & 0x04) != 0)
			{				// Rear Assignment Only
				if(i < cons[grtopf])
				{		// UP REAR CALLS
					if ((gc_xrucl[fl_ix] & fl_mask[i]) != 0)
					{		// Have memory of call transfer
						gc_xruc_set[fl_ix] &= ~fl_mask[i];	// turn off the output 
						if ((gc_xruc[fl_ix] & fl_mask[i]) == 0)		// call has been answered
						{
							gc_ruc_set[fl_ix] &= ~fl_mask[i];		// reset hall call flag
							if ((g_upr_hall_call[fl_ix][0] & fl_mask[i]) == 0)	 // The group does not have the call assigned so cancel the call
								gc_ruc_canc[fl_ix] |= fl_mask[i];
							if (((gc_ruc[fl_ix] & fl_mask[i]) == 0) || ((g_upr_hall_call[fl_ix][0] & fl_mask[i]) != 0))	  // The acknowledge light has been turned off
							{
								gc_xrucl[fl_ix] &= ~fl_mask[i];
								gc_xruct[fl_ix] &= ~fl_mask[i];
							}
						}
						else
						{
							if (((gc_ruc[fl_ix] & fl_mask[i]) == 0) && ((g_upr_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((gc_ruc_canc[fl_ix] & fl_mask[i]) == 0))		// the controller light is not on
								gc_ruc_set[fl_ix] |= fl_mask[i];		// set hall call flag
							else
								gc_ruc_set[fl_ix] &= ~fl_mask[i];		// reset hall call flag
						}
					}
					else
					{
						if ((gc_xruct[fl_ix] & fl_mask[i]) != 0)		// Have a call transferred to the old group
						{
							if ((gc_xruc[fl_ix] & fl_mask[i]) != 0)		// call input has been turned on from set output
								gc_xrucl[fl_ix] |= fl_mask[i];				// latch memory of the call
							gc_xruc_set[fl_ix] |= fl_mask[i];		// turn on an output so the old group can latch the call
						}
						else
						{
							if ((gc_xruc[fl_ix] & fl_mask[i]) != 0)		// the old group has its own call
							{
								gc_xrucl[fl_ix] |= fl_mask[i];			// set a transfer latch flag
								gc_xruc_set[fl_ix] |= fl_mask[i];		// turn on an output so the old group can latch the call
								gc_ruc_set[fl_ix] |= fl_mask[i];		// set hall call flag
							}
						}
					}
				}

				if(i > 1)
				{		// DOWN REAR CALLS
					if ((gc_xrdcl[fl_ix] & fl_mask[i]) != 0)
					{		// Have memory of call transfer
						gc_xrdc_set[fl_ix] &= ~fl_mask[i];	// turn off the output 
						if ((gc_xrdc[fl_ix] & fl_mask[i]) == 0)		// call has been answered
						{
							gc_rdc_set[fl_ix] &= ~fl_mask[i];		// reset hall call flag
							if ((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) == 0)	 // The group does not have the call assigned so cancel the call
								gc_rdc_canc[fl_ix] |= fl_mask[i];
							if (((gc_rdc[fl_ix] & fl_mask[i]) == 0) || ((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) != 0))	  // The acknowledge light has been turned off
							{
								gc_xrdcl[fl_ix] &= ~fl_mask[i];
								gc_xrdct[fl_ix] &= ~fl_mask[i];
							}
						}
						else
						{
							if (((gc_rdc[fl_ix] & fl_mask[i]) == 0) && ((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((gc_rdc_canc[fl_ix] & fl_mask[i]) == 0))		// the controller light is not on
								gc_rdc_set[fl_ix] |= fl_mask[i];		// set hall call flag
							else
								gc_rdc_set[fl_ix] &= ~fl_mask[i];		// reset hall call flag
						}
					}
					else
					{
						if ((gc_xrdct[fl_ix] & fl_mask[i]) != 0)		// Have a call transferred to the old group
						{
							if ((gc_xrdc[fl_ix] & fl_mask[i]) != 0)		// call input has been turned on from set output
								gc_xrdcl[fl_ix] |= fl_mask[i];				// latch memory of the call
							gc_xrdc_set[fl_ix] |= fl_mask[i];		// turn on an output so the old group can latch the call
						}
						else
						{
							if ((gc_xrdc[fl_ix] & fl_mask[i]) != 0)		// the old group has its own call
							{
								gc_xrdcl[fl_ix] |= fl_mask[i];			// set a transfer latch flag
								gc_xrdc_set[fl_ix] |= fl_mask[i];		// turn on an output so the old group can latch the call
								gc_rdc_set[fl_ix] |= fl_mask[i];		// set hall call flag
							}
						}
					}
				}
			}
		}		// end of for loop
	}
}

// ********************************************
// this is the group hall call cancel procedure
// ********************************************
void group_hallcancel()
{
  	int16 i,j,no_call;
  	int16 fl_ix;

  	for(i=1; i<cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
		for(j=1; j<=cons[nmbcars]; j++)
		{
			if( (grp_Car_Stat[j].g_cancel_up == i) && (g_asign_up[i] == j) )
			{
				gc_uc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
			}

			no_call = 0;
			if (((gc_uc[fl_ix] & fl_mask[i]) == 0) && ((gc_uc_dsr[fl_ix] & fl_mask[i]) == 0))
				no_call = 1;

			if( ((grp_Car_Stat[j].g_cancel_up == i) && (g_asign_up[i] == j)) &&
				((g_up_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
				(no_call == 1) )
			{
				if(uctim[i] < 15)
					lvars[uc_less15]++;
				if((uctim[i] < 30) && (uctim[i] >= 15))
					lvars[uc_less30]++;
				if((uctim[i] < 45) && (uctim[i] >= 30))
					lvars[uc_less45]++;
				if((uctim[i] < 60) && (uctim[i] >= 45))
					lvars[uc_less60]++;
				if(uctim[i] > 60)
					lvars[uc_more60]++;
				lvars[nmbuc]++;
				nmb_up_call[(int16)((t.hour*4)+(t.minute/15))][i]++;
				nmb_up_call[(int16)((t.hour*4)+(t.minute/15))][0]++;
				g_up_hall_call[fl_ix][0] &= ~fl_mask[i];
				last_uhc_cancelled[j] = (uint8)i;
			}
		}
  	}

	if (cons[LbyUpReq] != 0)
	{
		if (cons[epgrp] != 0)
			i = cons[grtopf]+1;
		else
			i = cons[grtopf];

		fl_ix = (i-1)/32;
		for(j=1; j<=cons[nmbcars]; j++)
		{
			if( (grp_Car_Stat[j].g_cancel_up == lobby_fl) && (lby_up_assign == j) )
			{
				gc_uc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
				gc_luc_canc = 1;
			}

			if ( (grp_Car_Stat[j].g_cancel_up == lobby_fl) && (lby_up_assign == j) && (lby_up_req != 0) && 
					( (((gc_uc[fl_ix] & fl_mask[i]) == 0) && ((cons[sercom] & 0x20) == 0)) ||
			  											((gc_luc == 0) && ((cons[sercom] & 0x20) != 0)) ) )
			{
				lby_up_req = 0;
			}
		}
	}

  	for(i=2; i<=cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
	    for(j=1; j<=cons[nmbcars]; j++)
	    {
			if( (grp_Car_Stat[j].g_cancel_dn == i) && (g_asign_dn[i] == j) )
			{
				gc_dc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
			}

			no_call = 0;

			if (((gc_dc[fl_ix] & fl_mask[i]) == 0) && ((gc_dc_dsr[fl_ix] & fl_mask[i]) == 0))
				no_call = 1;

			if( ((grp_Car_Stat[j].g_cancel_dn == i) && (g_asign_dn[i] == j)) &&
				((g_dn_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
			    (no_call == 1) )
			{
			    if(dctim[i] < 15)
					lvars[dc_less15]++;
			    if((dctim[i] < 30) && (dctim[i] >= 15))
					lvars[dc_less30]++;
			    if((dctim[i] < 45) && (dctim[i] >= 30))
					lvars[dc_less45]++;
			    if((dctim[i] < 60) && (dctim[i] >= 45))
					lvars[dc_less60]++;
			    if(dctim[i] > 60)
					lvars[dc_more60]++;
			    lvars[nmbdc]++;
				nmb_dn_call[((t.hour*4)+(t.minute/15))][i]++;
				nmb_dn_call[((t.hour*4)+(t.minute/15))][0]++;
			    g_dn_hall_call[fl_ix][0] &= ~fl_mask[i];
			    last_dhc_cancelled[j] = (uint8)i;
			}
    	}
  	}
  	if(grtop_rfl != 0)
  	{
	  	for(i=1; i<cons[grtopf]; i++)
	  	{
			fl_ix = (i-1)/32;
		    for(j=1; j<=cons[nmbcars]; j++)
		    {
				if( (grp_Car_Stat[j].g_cancel_upr == i) && (g_asign_upr[i] == j) )
				{
					gc_ruc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
				}

				no_call = 0;
				if (((gc_ruc[fl_ix] & fl_mask[i]) == 0) && ((gc_ruc_dsr[fl_ix] & fl_mask[i]) == 0))
						no_call = 1;
				if( ((grp_Car_Stat[j].g_cancel_upr == i) && (g_asign_upr[i] == j)) &&
				    ((g_upr_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
					(no_call == 1) )
				{
				    if(ucrtim[i] < 15)
						lvars[uc_less15]++;
				    if((ucrtim[i] < 30) && (ucrtim[i] >= 15))
						lvars[uc_less30]++;
				    if((ucrtim[i] < 45) && (ucrtim[i] >= 30))
						lvars[uc_less45]++;
				    if((ucrtim[i] < 60) && (ucrtim[i] >= 45))
						lvars[uc_less60]++;
					if(ucrtim[i] > 60)
						lvars[uc_more60]++;
				    lvars[nmbuc]++;
					nmb_up_call[((t.hour*4)+(t.minute/15))][i]++;
					nmb_up_call[((t.hour*4)+(t.minute/15))][0]++;
				    g_upr_hall_call[fl_ix][0] &= ~fl_mask[i];
				    last_ruhc_cancelled[j] = (uint8)i;
				}
	    	}
	  	}
	  	for(i=2; i<=cons[grtopf]; i++)
	  	{
			fl_ix = (i-1)/32;
		    for(j=1; j<=cons[nmbcars]; j++)
		    {
				if( (grp_Car_Stat[j].g_cancel_dnr == i) && (g_asign_dnr[i] == j) )
				{
					gc_rdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
				}

				no_call = 0;
				if (((gc_rdc[fl_ix] & fl_mask[i]) == 0)	&& ((gc_rdc_dsr[fl_ix] & fl_mask[i]) == 0))
					no_call = 1;

				if( ((grp_Car_Stat[j].g_cancel_dnr == i) && (g_asign_dnr[i] == j)) &&
			    ((g_dnr_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
			    (no_call == 1) )
				{
				    if(dcrtim[i] < 15)
						lvars[dc_less15]++;
				    if((dcrtim[i] < 30) && (dcrtim[i] >= 15))
						lvars[dc_less30]++;
				    if((dcrtim[i] < 45) && (dcrtim[i] >= 30))
						lvars[dc_less45]++;
				    if((dcrtim[i] < 60) && (dcrtim[i] >= 45))
						lvars[dc_less60]++;
				    if(dcrtim[i] > 60)
						lvars[dc_more60]++;
				    lvars[nmbdc]++;
					nmb_dn_call[((t.hour*4)+(t.minute/15))][i]++;
					nmb_dn_call[((t.hour*4)+(t.minute/15))][0]++;
				    g_dnr_hall_call[fl_ix][0] &= ~fl_mask[i];
				    last_rdhc_cancelled[j] = (uint8)i;
				}
	    	}
	  	}
  	}

	if (cons[incrs] != 0)
	{
		if (Car_ots_nbp[ir_car] == 1)
		{
			if ((any_ir_hall_call == 1) && (timers[tirots] > 20))
				grp_clrir();
		}
		else 
		{
			if ((ir_car >= 1) && (ir_car <= cons[nmbcars]))
			{
				timers[tirots] = 0;
				group_ir_hallcancel();
			}
			else
			{
				if (get_any_ir_hall_call() == 1)
				{
					timers[tirots] = 0;
					grp_clrir();
				}
				else if (timers[tirots] < 10)
					grp_clrir_canc();
			}
		}
		prev_ir_active = ir_active;
	}
	else if (cons[Second_Riser] != 0)
	{
		group_sr_hallcancel();
	}
	else
		grp_clrir();
	
	if ((cons[Third_Riser] != 0) && (cons[rear] == 0))
		group_tr_hallcancel();

  return;
}

/* Revision History

 1/6/12 v6.0.0 mhd/hdl	1. Modified cross assignment so that when the ratio of calls for galaxy cars is equal to the ration of calls to the old group cars, the assignment switches 
 					   		back and forth.
					   		Before the galaxy took all calls regardless of the number of cars in the old system. Now it alternates the calls back and forth (new-old-new...)
2/2/2012 v5.54.55 HDL	1. Added a fvars[fvhcsecctrl] to enable the hall call security to lockout the Second riser
5/10/12 v5.54.79 hdl	1. in a previous version we build a mechanism to alternate the cross assigment calls between old and new cars and 
							there was an "else"	missing so the xassign variable satyed at 0 all the time

6/26/12 v5.54.92 hdl	1. In grp_clrhc, i changed the logic so we wont clear gc_dc and gc_uc if a board is connected to the simulators.
							 In simulation mode, old logic will not allow the test for intergroup emerngency power to work
8/23/13 v7.0.16 mhd		1. Added changes for third riser and autoservice time-out.  Both from Galaxy 3 v6.0.53 as.
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.
1/29/14 v7.0.32 mhd		1. Changed car_eta[][] array to car_up_eta[][] and car_dn_eta[][].
9/5/15 v7.1.9 mhd		1. Modified cross assignment to work with individual I/O instead of the ir riser I/O's.
10/17/14 v7.1.17 mhd	1. Modified changes from galaxy 3 version 6.1.0 to 6.0.10.  See below:
							v6.1.02 hdl	1. modified logic for X-cancel so when no cars are in service we drop the calls and keep them from registering in the system.
12/18/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
							12/23/13 v6.1.23 hdl	1. reversed logic for X-cancel so when no cars are in service we drop the calls and keep them from registering in the system.
							03/04/14 v6.1.35 as		1. Added grp_clrirhc() group clear IR hall call procedure similar to grp_clrhc()
							04/09/14 v6.1.47 as		1. Modified latch_ser_hallcall() to disable opposite hallcall when the first hall call is pressed								
							08/20/14 v6.1.70 as		1. Added NCA (no car available input from the old group) when using cross assignment, in case NCA is ON do not dispatch calls to the old system.
11/04/15 v7.1.45 as		1. Added setting of last_uhc_cancelled[], last_dhc_cancelled[], and for rear, in group_hallcancel() routine.
*
*/
 					   