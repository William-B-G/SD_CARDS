// This file contains subroutines to get and setup the car's load weight and to set the drive's pre-torque amount. 
#define d_LOADW 1
#include "global.h"


// Load weighing and pre-torque variables
int32 torque = 0;					// amount to torque drive
int32 torque_offset = 0;			// torque offset to compensate for no compensating cables
int32 hoistway_dist = 0;			// hoistway distance in dpp counts
int32 hoistway_loc = 0;				// hoistway location of car
uint32 drive_torque = 0;	// torque value to send to the drive
int8 pre_torque_set = 0;
int32 percent_load = 0;			// Percent load from load weigher
uint16 lw_data[10] = {0,0,0,0,0,0,0,0,0,0};		// raw load weigher data
int16 lw_ix = 0;					// lw data index
uint32 avg_load = 0; 		// average load weigher reading
uint8 lw_command = 0;	// command to load weigher
#define c_lw_read 1				// command to read load value
#define c_lw_calibrate 2		// Calibrate load weigher
#define c_lw_run_cal 4			// Run Calibrations routine
uint8 ret_lw_command = 0;	// command returned from load weigher
// load data variable for each floor stored in flash memory
int16 lw_empty[61] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0};
int16 lw_full[61] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
					0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char lw_status = 0;	// load weigher status
#define c_lw_setup 0x1			// load weigher device setup
#define c_lw_on 0x2				// load weigher is on
#define c_lw_error 0x4			// load weigher has an error
#define c_lw_empty_setup 0x8	// load weigher empty setup
#define c_lw_full_setup 0x10	// load weigher full setup
#define c_lw_avg_load 0x20		// load weigher uses up and down average for load value
unsigned char lw_dir = 0;		// load weigher direction
#define c_lw_up 1				// load in the up direction
#define c_lw_dn 2				// load in the down direction
#define c_lw_dir_error 0x80		// load direction error
unsigned char lw_error = 0;		// load weigher error code
int16 lw_setup_seq = 0;			// load weigher setup sequence
int16 load_bp = 0;				// load bypass variable 1 = bypass
int16 load_an = 0;				// load antinuisance 1 = not enough load
int16 load_dis = 0;				// load dispatch 1 = dispatch car
int16 load_ovl = 0;				// load overload 1 = car is overloaded
int16 no_wheelchair_cap = 0;		// load high so car has no room for wheelchair
int16 lw_cal_seq = 0;				// load weigher calibration sequence flag
int16 lw_disable = 0;				// logical flag to disable loadweigher from switch
const int16 day_of_week_tbl[9] = {0,1,3,5,2,4,6,3,4};
int16 next_fl = 0;
float fl_slope;
float fl_empty;
float fl_full;
int load;

void get_load (int16 force_upd);
void setup_load_weigher (void);
void set_pre_torque (void);
void calibrate_lw (void);

//**************************************************
// *** Get the load weigher load percent routine
//**************************************************

void get_load (int16 force_upd)
{

#if (Simulator == 1)
	if (((cons[loadw] & 2) != 0) && ((lw_status & c_lw_on) != 0) && ((lw_status & c_lw_setup) != 0) && ((lw_status & c_lw_error) == 0) && (lw_disable == 0)) 
#else
	if (((cons[loadw] & 2) != 0) && (lw_can_online == 1) && 
		((lw_status & c_lw_on) != 0) && ((lw_status & c_lw_setup) != 0) && ((lw_status & c_lw_error) == 0) && (lw_disable == 0)) 
#endif
	{

		if ( (((doorf == 2) || (rdoorf == 2) || (lw_cal_seq == 4) || (force_upd == 1)) && (((procf >= 14) && (procf <=16)) || (procf == 21))) 
					|| ((procf == 2) && (chk_ins_sw() == 0)) )
		{		// doors are not closing and at floor or on inspection
			if ((lw_command & c_lw_calibrate) == 0)
				lw_command |= c_lw_read;			// don't do a read while calibrating

			if ((lw_full[position] - lw_empty[position]) > 0)
			{
				if ((int)avg_load >= lw_empty[position])
					percent_load = (int32)(1000.0 * (((float)avg_load - (float)lw_empty[position])/((float)lw_full[position] - (float)lw_empty[position])));
				else
					percent_load = 0;
			}
			else
			{
				lw_status |= c_lw_error;				
				percent_load = 0;
			}

			load = percent_load/10;
		}
		else
			lw_command &= ~c_lw_read;

		if ((load > fvars[fvloadbp]) && (fvars[fvloadbp] != 0))
			load_bp = 1;
		else
			load_bp = 0;
	
		if ((load < fvars[fvloadan]) && (fvars[fvloadan] != 0))
			load_an = 1;
		else
			load_an = 0;

		if ((load > fvars[fvloaddis]) && (fvars[fvloaddis] != 0))
			load_dis = 1;
		else
			load_dis = 0;

		if ((load > fvars[fvloadovl]) && (fvars[fvloadovl] != 0))
			load_ovl = 1;
		else
			load_ovl = 0;

		if ((load > fvars[fvhandcap]) && (fvars[fvhandcap] != 0))
			no_wheelchair_cap = 1;
		else
			no_wheelchair_cap = 0;

		lw_dir = 0;
		if (percent_load < (fvars[fvbalance] - 50))			// +/- 5% load
			lw_dir |= c_lw_up;
		else if (percent_load > (fvars[fvbalance] + 50))
			lw_dir |= c_lw_dn;
	}
	else
	{
		percent_load = fvars[fvbalance];
		load_bp = 0;
		load_an = 0;
		load_dis = 0;
		load_ovl = 0;
		lw_dir = c_lw_dir_error;
		no_wheelchair_cap = 0;
		lw_command &= ~c_lw_read;
	}
	
}



//**************************************************
// *** Setup the load weigher routine ***
//**************************************************

void setup_load_weigher (void)
{		 // This routine works with the LCD interface and the user
	int16 i;
#if (Simulator == 1)
	if ((cons[loadw] & 2) != 0)
#else
	if (((cons[loadw] & 2) != 0) && (lw_can_online == 1))
#endif
	{
		if ((firef != 0) || (rfiref != 0))
			lw_setup_seq = 0;
		
		if (lw_setup_seq == 1)	 // LCD interface routine sets lw_setup_seq to 1 to run no load sequence
		{	   // car is at the bottom floor with no load
			lw_status &= ~c_lw_setup;  // Clear the setup flag until finished
			lw_status &= ~c_lw_empty_setup;
			if (position == cons[bottomf])
			{
				next_fl = cons[bottomf];
				lw_setup_seq = 2;	// go to next setup state
			}
			else
			{
				timers[tloadw] = 0;		// clear timer for 10 second wait
				lw_setup_seq = 12;		// car did not reach bottom floor
				lw_status |= c_lw_error;
			}	
		}
		else if (lw_setup_seq == 2)	 // 
		{	   // car is at the next floor
			if ((position == next_fl) && (procf >= 14) && (procf <= 16))
			{
				timers[tloadw] = 0;		// clear timer for 10 second wait
				lw_setup_seq = 3;
			}
		}								
		else if (lw_setup_seq == 3)
		{			// setup empty 

			if (timers[tloadw] > 40)
			{
				lw_empty[position] = (int16)abs(avg_load);
				if (lw_empty[position] > 1024)
				{
					lw_setup_seq = 13;	// error condition
					lw_status |= c_lw_error;		// empty setup greater then 1024
				}
				else if (lw_empty[position] >= 0)
				{
					if (position < cons[topf])
					{
						next_fl = position + 1;
						for (i=(position+1);i<=cons[topf];i++)
						{
							if ((ccnmsk[cons[carnmb]][i] == 1) || (rccnmsk[cons[carnmb]][i] == 1))
							{
								next_fl = i;
								break;
							}
						}
						if (ccnmsk[cons[carnmb]][next_fl] == 1)
							set_carcall(cons[carnmb],next_fl);
						else if (rccnmsk[cons[carnmb]][next_fl] == 1)
							set_rcarcall(cons[carnmb],next_fl);
						lw_setup_seq = 2;	// go to next setup state
					}
					else
					{
						lw_setup_seq = 7;
						lw_status |= c_lw_empty_setup;
						wrlwvar();		 // write setup values
					}
				}
				else
				{
					lw_setup_seq = 14;	// error condition
					lw_status |= c_lw_error;		// empty value less than zero on empty setup
				}
			}	
		}
		else if (lw_setup_seq == 4)	 // LCD interface routine sets lw_setup_seq to 1 to run no load sequence
		{	   // car is at the bottom floor with no load
			lw_status &= ~c_lw_setup;  // Clear the setup flag until finished
			lw_status &= ~c_lw_full_setup;
			if (position == cons[bottomf])
			{
				next_fl = cons[bottomf];
				lw_setup_seq = 5;	// go to next setup state
			}
			else
			{
				timers[tloadw] = 0;		// clear timer for 10 second wait
				lw_setup_seq = 15;
				lw_status |= c_lw_error;		// full setup not at bottom floor for start
			}	
		}
		else if (lw_setup_seq == 5)	 // 
		{	   // car is at the next floor
			if ((position == next_fl) && (procf >= 14) && (procf <= 16))
			{
				timers[tloadw] = 0;		// clear timer for 10 second wait
				lw_setup_seq = 6;
			}
		}								
		else if (lw_setup_seq == 6)
		{
			if (timers[tloadw] > 40)
			{
				lw_full[position] = (int16)abs(avg_load);
				if (lw_full[position] > 1024)
				{
					lw_setup_seq = 16;	// error condition
					lw_status |= c_lw_error;		// full setup greater then 1024 
				}
				else if (lw_full[position] > lw_empty[position])
				{
					if (position < cons[topf])
					{
						next_fl = position + 1;
						for (i=(position+1);i<=cons[topf];i++)
						{
							if ((ccnmsk[cons[carnmb]][i] == 1) || (rccnmsk[cons[carnmb]][i] == 1))
							{
								next_fl = i;
								break;
							}
						}
						if (ccnmsk[cons[carnmb]][next_fl] == 1)
							set_carcall(cons[carnmb],next_fl);
						else if (rccnmsk[cons[carnmb]][next_fl] == 1)
							set_rcarcall(cons[carnmb],next_fl);
						lw_setup_seq = 5;	// go to previous state
					}
					else
					{
						lw_setup_seq = 7;
						lw_status |= c_lw_full_setup;
						wrlwvar();		 // write setup values
					}
				}
				else
				{
					lw_setup_seq = 17;	// error condition
					lw_status |= c_lw_error;	// full setup value not greater than empty value
				}
			}	
		}
		else if (lw_setup_seq == 7)	 // LCD interface routine sets lw_setup_seq to 11 to check for finished
		{	   // car is at the bottom floor with full loiad
			if (position == cons[topf])
			{
				next_fl = cons[bottomf];
				if (((lw_status & c_lw_empty_setup) != 0) && ((lw_status & c_lw_full_setup) != 0))
				{
					if (verify_lw_data() == 1)		// verify error
					{
						lw_setup_seq = 8;	// finished setup
						lw_status |= c_lw_setup;
						lw_status |= c_lw_on;
						lw_status &= ~c_lw_error;
						wrlwvar();		 // write setup values
					}
					else
					{
						lw_setup_seq = 19;
						lw_status |= c_lw_error;		// empty and full car not setup
					}
				}
				else
				{
					lw_status &= ~c_lw_setup;
					if ((lw_status & c_lw_empty_setup) != 0)
					{
						lw_setup_seq = 9;				// Empty setup finished
					}
					else if ((lw_status & c_lw_full_setup) != 0)
					{
						lw_setup_seq = 10;				// Full setup finished
					}
					else
					{
						lw_setup_seq = 11;				// 
						lw_status |= c_lw_error;		// empty and full car not setup
					}
						
				}
				
			}
			else
			{
				lw_setup_seq = 18;
				lw_status |= c_lw_error;	// position not a top floor to finish
			}	
		}
	}
}


//*****************************************
// *** Set the drive pre-toque amount ***
//*****************************************

void set_pre_torque (void)
{

#if (Simulator == 1)
  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && 
#else
  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && (lw_can_online == 1) &&
#endif
  		((lw_status & c_lw_setup) != 0) && ((lw_status & c_lw_on) != 0) && 
  								((lw_status & c_lw_error) == 0) && (lw_disable == 0))
  	{						// load weigher setup and on
		if (abs(percent_load - (int32)fvars[fvbalance]) > 1000)
		{				// Max 100 percent torque 
			if (percent_load > fvars[fvbalance])
				torque = 10000;
			else
				torque = -10000;
		}
		else
		{
#if (Traction == 1)
			hoistway_dist = (DPP_Floor_Pos[cons[topf]] + DPP_Fl_Offset[cons[topf]]) - (DPP_Floor_Pos[cons[bottomf]] + DPP_Fl_Offset[cons[bottomf]]);
			hoistway_loc = ((DPP_Count - (DPP_Floor_Pos[cons[bottomf]] + DPP_Fl_Offset[cons[bottomf]]))/hoistway_dist);
			if (DPP_Count > DPP_DN)	   // car is above the normal limit
				torque_offset = (int)((int32)fvars[fvtorqoff] * hoistway_loc);
#else
			torque_offset = 0;
#endif
			if (fvars[fvtorqdn] != 0)
			{
				if (dirf == 2)	// down direction
					torque = (int)(((int32)((fvars[fvtorqdn] + torque_offset)) * (int32)(percent_load - fvars[fvbalance]))/1000.0);	// note that torque can go negative
				else  // up or no direction
					torque = (int)(((int32)((fvars[fvtorque] + torque_offset)) * (int32)(percent_load - fvars[fvbalance]))/1000.0);	// note that torque can go negative
			}
			else
				torque = (int)(((int32)((fvars[fvtorque] + torque_offset)) * (int32)(percent_load - fvars[fvbalance]))/1000.0);	// note that torque can go negative
		}

		pre_torque_set = 1;
  	}
  	else
	{
		pre_torque_set = 1;
		torque = 0;
	}
}

// **************************************
// ** Calibrate load weigher **
// **************************************

void calibrate_lw (void)
{
	int empty_diff;
	int i;

#if (Simulator == 1)
	if ((cons[loadw] & 2) != 0)
#else
	if (((cons[loadw] & 2) != 0) && (lw_can_online == 1))
#endif
	{
	  	if( ((servf == s_AUTO) || ((servf == s_ADOFF) && ((statusf & sf_AD) != 0))) && (dpref == 0) && (onward_calls == 0) &&
	  		((lw_status & c_lw_on) != 0) && ((lw_status & c_lw_setup) != 0) && (lw_disable == 0) && (procf >= 14) && (procf <=16) )
		{
	  		if ((day_of_week_tbl[cons[carnmb]] == day_of_week) || ((lw_command & c_lw_run_cal) != 0))
			{
				local_gettime();
				if (((t.hour >= 2) || ((lw_command & c_lw_run_cal) != 0)) && (lw_cal_seq == 1))
				{				// time to calibrate and doors closed
					lw_cal_seq = 2;
					timers[tloadw] = 0;		// clear timer for 10 second wait
				}
			}
			else
				lw_cal_seq = 0;
		}

		if ((lw_cal_seq == 2) && (timers[tloadw] > 15))
		{		// Run the load weigher calibration routine
			servf = s_LW_CAL;
			if (position != cons[bottomf]) 
				zone_fl = cons[bottomf];
			else if ((procf >= 14) && (procf <=16))
				lw_cal_seq = 3;
		}
		else if (lw_cal_seq == 3)
		{	   // Set for the load weigher to null op-amps or to calibrate device
			servf = s_LW_CAL;
			lw_command |= c_lw_calibrate;
			lw_command &= ~c_lw_read;
			lw_cal_seq = 4;
			timers[tloadw] = 0;
		}
		else if ((lw_cal_seq == 4) && (timers[tloadw] > 30))
		{	   // Clear the calibration flag
			servf = s_LW_CAL;
			lw_command &= ~c_lw_calibrate;
			lw_cal_seq = 5;
			timers[tloadw] = 0;
		}
		else if ((lw_cal_seq == 5) && (timers[tloadw] > 15))
		{	   // finished device calibration - read in new data
			servf = s_LW_CAL;
			lw_command &= ~c_lw_run_cal;
			lw_command |= c_lw_read;
			lw_cal_seq = 6;
			timers[tloadw] = 0;
		}
		else if (lw_cal_seq == 6) 
		{	   // Read calibrated device data
			servf = s_LW_CAL;
			lw_command |= c_lw_read;
			if (timers[tloadw] > 40)
			{	// Re-calibrate parameters for zero
				empty_diff = (int)avg_load - lw_empty[position];
				if ((lw_full[cons[topf]] + empty_diff) <= 1024) // 10 bits
				{			// within range
					for (i=cons[bottomf];i<=cons[topf];i++)
					{
						lw_empty[i] += empty_diff;
						if (lw_empty[i] < 0)
							lw_empty[i] = 0;
						lw_full[i] += empty_diff;
						if (lw_full[i] > 1023)
							lw_full[i] = 1023;
					}
					if (verify_lw_data() == 1)
					{
						lw_status |= c_lw_setup;		// set setup flag
						lw_status &= ~c_lw_error;		// clear error flag
					}
					else
					{
						lw_status &= ~c_lw_on;			// load weigher is turned off
						lw_status &= ~c_lw_setup;		// error so clear setup flag
						lw_status |= c_lw_error;		// device has error
						record_fault(f_lw_load_table);
					}
				}
				else
				{
					lw_status |= c_lw_error;
					lw_status &= ~c_lw_on;				// load weigher is turned off
					record_fault(f_lw_calibrate);		// load weigher calibration error
				}
				wrlwvar();
				lw_cal_seq = 7;
			}			
		}
		else if (lw_cal_seq == 7)
		{		   // Finished load weigher calibration
			if (servf == s_LW_CAL)
				servf = s_AUTO;
			zone_fl = 0;
			servf = s_AUTO;
			lw_cal_seq = 0;
			lw_command &= ~c_lw_read;
		}

		if ((lw_cal_seq >= 2) && (lw_cal_seq <= 6))
		{
			if ((onward_calls != 0) || (firef != 0) || (rfiref != 0))
			{				// pospone service until call is finished
				lw_cal_seq = 1;
				zone_fl = 0;
				if ((firef == 0) && (rfiref == 0))
					servf = s_AUTO;
				lw_command &= ~c_lw_run_cal;
				lw_command &= ~c_lw_read;
			}
		}
		else 
		{
			if (servf == s_LW_CAL)
				servf = s_AUTO;
		}
	}
}


/* Revision History

2/10/04 v4.07 mhd	1. Added analog load weigher with pre-torque. 
4/22/05 4.08 mhd	1. Modified load weigher calibration sequence to pospone service if interrupted.
4/25/05 4.14 mhd	1. Modify load weighing setup to record empty and full value at each floor.
					2. Modify load weighing calibration routine to adjust new setup configuration.
5/5/05 v4.17 mhd	1. If comm lost to load weigher set percent_load = fvars[fvbalance] (balanced load).
5/24/05 v4.19 mhd	1. Allow a value of zero at the bottom floor
7/11/05 v4.23 mhd	1. Set flag when pretorque value set (pre_torque_set).
8/22/05 v4.29 mhd	1. Changed start index for can from c_can_SPB to c_can_START to add the brake board c_can_BRK.
613/06 v4.71 mhd	1. On start of load weighing setup, set next_fl = cons[bottomf] instead of 1.
7/19/06 v4.77 mhd	1. Allow analog load value to be zero at all floors.
8/26/06 5.03 mhd	1. Change load_data[] array from 5 to 10.
1/8/07 v5.11 mhd	1. Changed all double data types to float for the djgpp compiler.
3/28/07 v5.22 mhd	1. Check if avg_load is not less than the empty load value at the floor to prevent the precent load
                       from going negative.
					2. Added torque down variable fvars[fvtorqdn] to adjust up and down direction torque differently.
5/10/07 v5.23 mhd	1. Added an extra second delay between stops at floor to setup load weigher.
					2. Take the abs() value of avg_load when setting lw_full and lw_empty.
					3. Change avg_load to int when calculating empty_diff.
12/20/07 v5.39.8 mhd 	1. Added no_wheelchair_cap if load greater than fvhandcap (handicap car capacity).
12/28/07 				2. In setup_load_weigher(), check for valid floor before running to next floor.
12/16/08 v5.46.8 mhd	1. Add DPP_Fl_Offset[position] to DPP_Floor_Pos[position].
4/15/09 v5.48.2 mhd		1. Allow get_load() when in ep recovery (procf == 21).
10/14/09 v5.50.4 mhd	1. Modified setup_load_weigher() to allow setup at rear only floors.
10/21/09 v5.50.4 mhd/hl 1. Increase size of lw_empty[] and lw_full[] arrays from 32 to 61.
4/10/15 v7.1.30 mhd		1. Check for load weigher to be out of range, i.e. > 1024, during setup.
8/5/15 v7.1.40 mhd		1. Changed load weighing routines to read load values in both up and down direction of stop.
9/3/15 V7.1.41 mhd		1. Changed load weighing setup routine to read load values in the up direction.
*/
