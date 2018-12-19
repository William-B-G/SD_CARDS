#define d_GPEAK 1
#include "global.h"

// Up Peak Variables
int16 up_peak = 0;			// Group Up Peak flag
int16 local_up_peak = 0;			// Local Up Peak Flag
int16 up_peak_trig_cnt = 0;	// Up peak trigger count
unsigned char up_trig_mem [car_size];	// memory of a car's up peak trigger being set
int16 up_peak_trig[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};	// Up peak trigger count used for moving sum
int16 up_peak_trig_ptr = 0;	// Pointer for up peak trigger sum
int16 sum_up_peak_trig = 0;	// Sum of up peak trigger count
int16 nmb_cc_above_lby=0;	// number of latched car calls above the lobby
int16 lwd_set_at_lby=0;	// Load weigh dispatch set at lobby
int16 up_peak_trigger=0;	// Up peak trigger detection to group

// Down Peak Variables
int16 dn_peak=0;			// Down Peak Flag
int16 nmb_dc_above_lby=0;	// Number of down calls answered above the lobby
int16 dn_peak_trig[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Down peak trigger count used for moving sum
int16 dn_peak_trig_ptr = 0;	// Pointer for down peak trigger sum
int16 sum_dn_peak_trig = 0;	// Sum down peak trigger count
		

void group_dn_peak(void);
void group_up_peak(void);


//*****************************************
// Down peak detection and activation 
//*****************************************

void group_dn_peak(void)
{
	int16 i;

	if ((emdispf == 0) && (in_grp_sabbath == 0))
	{
		if (timers[tdptrig] >= (fvars[fvdptrigtim]/20))
		{
			if (dn_peak_trig_ptr < 19)
				dn_peak_trig_ptr ++;
			else
				dn_peak_trig_ptr = 0;
			
			dn_peak_trig[dn_peak_trig_ptr] = nmb_dc_above_lby;
			nmb_dc_above_lby = 0;
			sum_dn_peak_trig = 0;
			for(i=0;i<=19;i++)
				sum_dn_peak_trig += dn_peak_trig[i];
			timers[tdptrig] = 0;
		}

		if (fvars[fvdpdurtim] == 0)
		{		 // Down peak disabled
			timers[tdnpeak] = 0;	   
			dn_peak = 0;
			return;
		}

		if ((sum_dn_peak_trig >= fvars[fvdptrigcnt]) || (tmr_svc[s_dnpeak] == 1) || 
			(((cons[ids] & 1) != 0) && (ids_dnpeak == 1)) || (((cons[Galileo] & 1) != 0) && (ui_dnpeak == 1)))		// Lots of down hall calls
		{
			timers[tdnpeak] = 0;	   // Keep the car on up peak
			dn_peak = 1;
		}

		if (dn_peak == 1)
		{
			if (timers[tdnpeak] >= fvars[fvdpdurtim])
				dn_peak = 0;		// Down peak has timmed out
		}
	}
	else
	{
		if (dn_peak == 1)
		{
			for(i=0;i<=19;i++)
				dn_peak_trig[i] = 0;
		}
		nmb_dc_above_lby = 0;
		sum_dn_peak_trig = 0;
		timers[tdptrig] = 0;
		timers[tdnpeak] = fvars[fvdpdurtim] + 1;
		dn_peak = 0;		// turn off down peak
	}
}

//*****************************************
// Up peak detection and activation 
//*****************************************

void group_up_peak(void)
{
	int16 i;

	if ((emdispf == 0) && (in_grp_sabbath == 0))
	{
		// Get up peak trigger data from cars
		
		for (i=1;i<=cons[nmbcars]; i++)
		{
			if ((grp_Car_Stat[i].g_control & 0x1) != 0)	 // Flag is set from leaving the lobby with lwd set or > # car calls.
			{
				if (up_trig_mem[i] == 0) 
				{
					up_trig_mem[i] = 1;
					if (up_peak_trig_cnt < 1638)	// keep the int16 number positive (65535/2)/ 20 samples = 1638
						up_peak_trig_cnt++;
				}
			}
			else 
				up_trig_mem[i] = 0;
		}

		if (timers[tuptrig] >= (fvars[fvuptrigtim]/20))	 // Usually set to 60 seconds
		{				   // Use 20 samples for the moving average
			if (up_peak_trig_ptr < 19)
				up_peak_trig_ptr ++;
			else
				up_peak_trig_ptr = 0;
					
			up_peak_trig[up_peak_trig_ptr] = up_peak_trig_cnt;
			up_peak_trig_cnt = 0;
			sum_up_peak_trig = 0;
			for(i=0;i<=19;i++)
				sum_up_peak_trig += up_peak_trig[i];
			timers[tuptrig] = 0;
		}

		if (fvars[fvupdurtim] == 0)		
		{			// up peak disabled
			up_peak = 0;
			local_up_peak = 0;
			timers[tuppeak] = 0;
			return;
		}

		if ((sum_up_peak_trig >= fvars[fvuptrigcnt]) || (tmr_svc[s_uppeak] == 1) || 
			(((cons[ids] & 1) != 0) && (ids_uppeak == 1)) || (((cons[Galileo] & 1) != 0) && (ui_uppeak == 1)))
		{
			timers[tuppeak] = 0;	   // Keep the car on up peak
			up_peak = 1;
		}

		if (up_peak == 1)
		{
			if (timers[tuppeak] >= fvars[fvupdurtim])
				up_peak = 0;		// Up peak has timmed out
		}

		local_up_peak = up_peak;

	}
	else
	{
		if (up_peak == 1)
		{
			for (i=1;i<=cons[nmbcars]; i++)
				up_trig_mem[i] = 0;
			for(i=0;i<=19;i++)
				up_peak_trig[i] = 0;
		}
		up_peak_trig_cnt = 0;
		sum_up_peak_trig = 0;
		timers[tuptrig] = 0;
		timers[tuppeak] = fvars[fvupdurtim] + 1;
		up_peak = 0;		// Turn off Up peak 
		local_up_peak = 0;
	}
	

}

/* 	Revision History

9/13/11 v5.54.24 hdl 	1. Added local_up_peak to the data sent over to each car
1/9/12 v5.54.48 hdl		1. Changed the logic for local_up_peak. it did not work when the car was the dispatcher.

*/
