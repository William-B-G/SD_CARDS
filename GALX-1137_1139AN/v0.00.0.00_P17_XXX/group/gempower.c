#define d_GEMPOWER 1
#include "global.h"

extern const uint16 bittbl[16];

// Emergency power variables

int16 gempf=0;			// data from grp_empf[car] of the group is transferred into gempf for this car
int16 grp_empf[car_size];	// emergency power group flag
			// grp_empf: Emergency power group flag							
			// grp_empf = 0 not selected to run								no_ep 0					
			// grp_empf = 1 wait on group emp								ep_wait	1				
			// grp_empf = 2 wait with doors open							ep_wait_door 2 			
			// grp_empf = 3 car selected to recall 							ep_recall 3				
			// grp_empf = 4 car recalled successfully; open doors			ep_home	4				
			// grp_empf = 5 car recalled successfully; doors cycled closed	ep_home_close 5			
			// grp_empf = 6 car selected to run on automatic				ep_select 6				
			// grp_empf = 7 car selected to run from selection switch		ep_switch 7			
			// grp_empf = 8 car timed-out during recall						ep_timed_out 8				
			// grp_empf = 9 car is out of service during recall				ep_out_of_svc 9	

#define no_ep 0					//  0 not selected to run
#define ep_wait	1				//  1 wait on group emp
#define ep_wait_door 2 			//  2 wait with doors open
#define ep_recall 3				//  3 car selected to recall 
#define ep_home	4				//  4 car recalled successfully
#define ep_home_close 5			//  5 car recalled and cycle doors closed
#define ep_select 6				//  6 car selected to run on automatic
#define ep_switch 7				//  7 car selected to run from selection switch
#define ep_timed_out 8			//  8 car timed-out during recall
#define ep_out_of_svc 9			//  9 car is out of service during recall

#define epc_wait_close 7			//  7 car is waiting with the doors closed				 

int16 ep_auto_op_en = 0;				// Emergency power service recall and select enable input
int16 ep_auto_recall_en = 0;  			// Emergency Power Auto Recall enable
int16 ep_auto_select_en = 0;  			// Emergency Power Auto Select Enable
int16 ep_auto_select_en_prev = 0;		// Emergency Power Auto Select Enable  previous
int16 ep_run_en = 0;					// Emergency Power Run enable
int16 ep_run_ctl = 0;					// Emergency power run ctl output

int16 man_sel_req[car_size];				// manual selection request from switch input
int16 car_recall_order[car_size];		  	// Car return order
int16 car_select_order[car_size];	  		// Car selection order
int16 car_recall_order_fixed[car_size]; 	// Car return order from cons file
int16 car_select_order_fixed[car_size]; 	// Car selection order from cons file
int16 ep_prk[car_size];						// EP Car returned and parked output 
int16 ep_op[car_size];						// EP Car operation output 
int16 ep_sel[car_size];						// EP Car selected output 

int16 nmb_recall_cars = 0;					// number of cars for each power feeder
int16 nmb_select_cars = 0;					// number of cars for each power feeder
int16 car_recall_en = 0;					// enable recall of cars for each power feeder
int16 car_select_en = 0;					// enable selection of cars for each power feeder
int16 select_req = 0;						// detect change in manual selection switch				
int16 prev_select_req = 0;					// detect change in manual selection switch
int16 man_sel_seq = 0;			  			// manual selection state machine sequence
int16 recall_seq = 0;				 	 	// Recall state machine sequence
int16 cars_recalling = 0;				  	// Number of cars recalling
int16 recalled_cars = 0;				  	// number of recalled cars
int16 recall_finished = 0;		 	 		// Recall sequence finished
int16 recall_attempt = 0;				  	// Number of recall attempts
int16 select_seq = 0;					  	// Car Select state machine sequence
int16 cars_selected = 0;				  	// Number of cars selected
int16 xcars_sel_from_eps = 0;				// Number of cars selected from EPS that are NOT part of select group
int16 cars_moving = 0;			  			// Set true when any car is moving
int16 cars_in_motion = 0;			 	 	// Number of cars in motion that are not selected


int16 fs2_car = 0;
int16 hsv_car = 0;
int16 car_fire_sw_on = 0;

uint16 run_cnt[9] = {0,0,0,0,0,0,0,0,0};
int16 run_cnt_latch[9] = {0,0,0,0,0,0,0,0,0};

void set_ep_run_ctl (int16 select_req);
void group_em_power(void);
void manual_select_cars(void);
void recall_cars(void);
void select_cars(void);
void set_recall_order (void);
void set_select_car_order (void);
int16 is_car_in_sel_list(int16 el);


// ********************************************
// emergency power control procedure
// ********************************************

// Note: Split Power Feeder operation does not work with wired intergroup emergency power but will work with serial intergroup em power

void group_em_power(void)
{
	int16 i;
	int16 k = 0;

	select_req = 0;

	// Inter-group emergency power i/o control
	if ((cons[epgrp] != 0) && (cons[EPSF] == 0))		// have wired inter-group emergency power
	{
		ep_auto_select_en = 0;
	}


  	if ((gin_EMP == 1) || (((cons[EMPop] & 0x01) != 0) && (gin_EPT == 1))) 
	{			// system is on emergency power or early emergency power transfer

		car_fire_sw_on = 0;

		// Check for manual selection switch
		for(i=1; i<=cons[nmbcars]; i++)
		{
			 // EPS from car Switch data sent from the car over the comm or from the IGEP box
			if (((grp_Car_Stat[i].g_empf & 0x08) != 0) || (igep_car_man_sel_req[i] == 1))
			{		 // manual selection switch is set for this car
				//special condition where autep is used but other cars required manual selection.
				if(cons[EPSEL_tables] == 1)
				{
					if(is_car_in_sel_list(i) == 1)
					{
						man_sel_req[i] = 1;	// request to select this car
						select_req ++;	// count the cars requesting manual selection	
					}
					else // blindly allow manual car selection since it is not part of default cars to be selected from fvepcars
					{
						if(k < fvars[fvepcars])
						{
							k++;
							grp_empf[i] = ep_select;
						}
					}
				}
				else
				{
					man_sel_req[i] = 1;	// request to select this car
					select_req ++;	// count the cars requesting manual selection
				}
			}
			else
			{
				if(cons[EPSEL_tables] == 1)
				{
					if((is_car_in_sel_list(i) == 0) && (grp_empf[i] == ep_select))
					{
						grp_empf[i] = ep_wait;
					}
				}
				man_sel_req[i] = 0;	// Deselect the car 
			}

			if (((grp_Car_Stat[i].g_fire_status & fs_FS) != 0) || ((grp_Car_Stat[i].g_fire_status & fs_FSX) != 0))
				car_fire_sw_on = 1;
		}

		xcars_sel_from_eps = k;

		if (select_req != prev_select_req)
		{
			if (select_req == 0)
			{				// switch changed back to auto select
				recall_finished = 0;		
				recall_seq = 0;		// reset recall sequence if sequence aborted
			}
			timers[tepsw] = 0;				// reset timer to wait if switch is changed
		}
		prev_select_req = select_req;

		if (timers[tepsw] > 30)	 // wait 3 seconds
		{
			if (((cons[EMPop] & 0x01) != 0) && (gin_EPT == 1))	 // Building requisting emergency power transfer
			{	// This group is not selected so shutdown its car
				man_sel_seq = 0;
				cars_recalling = 0;
				recall_seq = 0;		// reset recall sequence if sequence aborted
				recall_attempt = 0;		// restart recall attempts
				recall_finished = 0;
				select_seq = 0;
				for(i=1;i<=cons[nmbcars];i++)
				{
					if (grp_empf[i] == ep_select)
						grp_empf[i] = ep_home;			// had to have made the recall
					else if (grp_empf[i] != ep_home)	// ep_home = the car has the door open and memory of recall
						grp_empf[i] = ep_wait_door;		// have other cars open their doors
				}
			}
			else if ((select_req != 0) && 
							(((fvars[fvepmanselen] & 0x02) == 0) || 
							(((fvars[fvepmanselen] & 0x02) != 0) && (recall_finished == 1)) || 
							(car_fire_sw_on == 1) || ((fire_status & fs_FS) != 0)))
			{		// Manual Selection of Car
				recall_seq = 0;		// reset recall sequence if sequence aborted
				select_seq = 0;
				if ((ep_run_en == 1) || (cons[epgrp] == 0))
					manual_select_cars();	// allowed for this group
				else
				{	 // not allowed for this group
					for(i=1;i<=cons[nmbcars];i++)
					{
						grp_empf[i] = ep_wait_door;		// have other cars open their doors
					}
				}
			}
			else
			{		// Automatic recall and selection
				man_sel_seq = 0;
				if ((cons[epgrp] == 0) && (cons[Ser_IGEP] == 0))
				{ 	 
					if (cons[ep_auto_en] == 0)
					{	// Standard emergency power
						if (recall_finished == 0) 
							recall_cars();
						else
							select_cars();
					}
					else
					{
						if (ep_auto_op_en != 0)
						{	// normal recalling and selecting
							if (recall_finished == 0) 
								recall_cars();
							else
								select_cars();							
						}
						else
						{	// shutdown all cars
							cars_recalling = 0;
							select_seq = 0;
							for(i=1;i<=cons[nmbcars];i++)
							{
								if (grp_empf[i] == ep_select)
								{
									if(is_car_in_sel_list(i) == 1)			//if car is in the selct list, have it stop,
									{												//otherwise ignore
										grp_empf[i] = ep_home;			// had to have made the recall
									}
								}
								else if (grp_empf[i] != ep_home)	// ep_home = the car has the door open and memory of recall
								{
									grp_empf[i] = ep_wait_door;		// have other cars open their doors
								}
							}
						}
					}
				}
				else
				{	// Intergroup emergency power

					if ((ep_auto_select_en == 0) && (ep_auto_select_en_prev == 1))
					{
						recall_finished = 0;
						recall_seq = 0;						
					}

					if ((ep_auto_recall_en == 1) && (recall_finished == 0) && ((ep_run_en == 1) || (cons[epgrp] == cons[carnmb])))
						recall_cars();	 // This group can recall its cars
					else if ((ep_auto_recall_en == 1) && (recall_finished == 1) && (ep_run_en == 1) && ((ep_auto_select_en == 1) || (cons[epgrp] == cons[carnmb])))
						select_cars();	  // This group can select a car to run
					else
					{	// This group is not selected so shutdown its car
						cars_recalling = 0;
						select_seq = 0;
						for(i=1;i<=cons[nmbcars];i++)
						{
							if (grp_empf[i] == ep_select)
								grp_empf[i] = ep_home;			// had to have made the recall
							else if (grp_empf[i] != ep_home)	// ep_home = the car has the door open and memory of recall
								grp_empf[i] = ep_wait_door;		// have other cars open their doors
						}
					}
					ep_auto_select_en_prev = ep_auto_select_en; // keep history of auto select
				}
			}
		}
	}
	else
	{		// not on emergency power
		cars_selected = 0;
		for (i=1;i<=cons[nmbcars];i++)
		{
			grp_empf[i] = no_ep;
		}
		timers[tepsw] = 0;		
		prev_select_req = 0;	// preset to detect a manual switch change	
		recall_seq = 0;
		select_seq = 0;
		recall_attempt = 0;
		man_sel_seq = 0;
		recall_finished = 0;
		timers[teprcldly] = 0;		// delay on initial recall
	}

	grp_empf[0] = grp_empf[cons[carnmb]];	// Set the car's data if this is the group
	gempf = grp_empf[0];

	set_ep_run_ctl(select_req);				 // Set output data
    if ((cons[EMPop] & 0x08) != 0)
	{
 		for(i=1;i<=cons[nmbcars];i++)
 		{
			if (fvars[fveppkled] == 1)
			{
				if ((grp_empf[i] == ep_select) || (grp_empf[i] == ep_home) || ((grp_Car_Stat[i].g_pos == fvars[fvepfl]) && (grp_empf[i] == ep_wait_door)))
					ep_prk[i] = 1;		// parked with doors open
				else
					ep_prk[i] = 0;
			}
			else if(fvars[fveppkled] == 2)
			{
				if (((grp_empf[i] == ep_select)&&(grp_Car_Stat[i].g_pos == fvars[fvepfl])) || (grp_empf[i] == ep_home) || ((grp_Car_Stat[i].g_pos == fvars[fvepfl]) && (grp_empf[i] == ep_wait_door)))
					ep_prk[i] = 1;		// parked with doors open
				else
					ep_prk[i] = 0;

			}
			else
			{
				if ((grp_empf[i] == ep_home) || ((grp_Car_Stat[i].g_pos == fvars[fvepfl]) && (grp_empf[i] == ep_wait_door)))
					ep_prk[i] = 1;		// parked with doors open
				else
					ep_prk[i] = 0;
			}

			if (fvars[fvepopled] == 1)
			{
				if (grp_empf[i] == no_ep)
					ep_op[i] = 1;		// car is on normal power
				else
					ep_op[i] = 0;
			}
			else if (fvars[fvepopled] == 2)
			{
				if (grp_empf[i] == no_ep)
					ep_op[i] = 0;
				else
					ep_op[i] = 1;		// car is on emergency power
			}
			else if (fvars[fvepopled] == 3)
			{
				if (grp_empf[i] == ep_recall)
					ep_op[i] = 1;		// car is on normal power
				else
					ep_op[i] = 0;
			}
			else
			{
				if ((grp_empf[i] != no_ep) && (grp_empf[i] < ep_timed_out) && (grp_Car_Stat[i].g_servf != 0))
					ep_op[i] = 1;		// car is operational
				else
					ep_op[i] = 0;
			}
			
			if ((grp_empf[i] == ep_select) || (grp_empf[i] == ep_switch) || ((fvars[fvepopled] != 3) && (grp_empf[i] == ep_recall)))
				ep_sel[i] = 1;	// selected to run
			else
				ep_sel[i] = 1;
		}
	}
}

// ***********************************************************
// *** Check if car is selection list
// ***********************************************************

int16 is_car_in_sel_list(int16 el)
{ // function to check if car is in the list for car_select_order[][]
	int i;
	int16 found = 0;
	for(i=1;i<=cons[nmbcars];i++)
	{												   
		if(car_select_order[i] == el)   
		{
			found = 1;
		}
	}
	return found;

}


// *************************************************************
// Inter-group emergency power routine to set run control output
// *************************************************************

void set_ep_run_ctl (int16 select_req)
{
	int16 i;

	ep_run_ctl = 0;


	if ((cons[epgrp] != 0) || (cons[EPSF] == 1))		// have inter-group emergency power
	{

		cars_moving = 0;
		for (i=1; i<=cons[nmbcars]; i++)
		{	
			if ( (((grp_Car_Stat[i].g_run_statusf & rs_DNR) != 0) || ((grp_Car_Stat[i].g_run_statusf & rs_UP) != 0) || ((grp_Car_Stat[i].g_run_statusf & rs_RUN) != 0)) &&
			    (Car_Not_Talking[i] == 0))
			{
				cars_moving = 1;	// a car is moving
			}
		}		
		if (ep_auto_recall_en == 1)
		{	  // auto recall enabled
			if ((recall_finished == 1) && (cars_moving == 0))
				ep_run_ctl = 1;		// allow next car
		}
		else
		{	  // manual selection switch is selected for a group
			if ((select_req != 0) || ((ep_run_en == 1) && (cars_moving == 0)))   
				ep_run_ctl = 1;		// Chain from output to input in a loop run_en shows if car is not selected and moving
		}

	}
}

// ********************************************
// emergency power manual switch car selection
// ********************************************
void manual_select_cars(void)
{
	int16 i;

	switch (man_sel_seq)
	{
		case 0:
			cars_selected = 0;

			for (i=1; i<=cons[nmbcars]; i++)
			{
				if ((grp_empf[i] != ep_recall) || ((fvars[fvepmanselen] & 0x01) == 0)) 	 // car has not been given the recall command	
					grp_empf[i] = ep_wait_door;	// Make sure all cars are waiting on Emergency Power
			}

			man_sel_seq = 1;
			break;

		case 1:
			cars_in_motion = 0;
			// De-select car if switch changes
			for (i=1; i<=cons[nmbcars]; i++)
			{	
				if (grp_empf[i] == ep_switch)
				{			 // This car was selected to run from manual select switch
					if (man_sel_req[i] == 0)
					{
						grp_empf[i] = ep_wait_door;	// Deselect car
						if (cars_selected > 0)
							cars_selected --;  // Allow another car
					}
				}

				if (grp_empf[i] == ep_recall)
				{			 // This car was in the process of recall
					if ( ( ((grp_Car_Stat[i].g_empf & 0x07) == ep_home)	|| 
						 	((grp_Car_Stat[i].g_empf & 0x07) == ep_home_close) )	||
						 ( ((grp_Car_Stat[i].g_run_statusf & rs_RUN) == 0) && (grp_Car_Stat[i].g_servf == s_MED_EMS) &&
						 	((grp_Car_Stat[i].g_run_statusf & rs_DL) != 0) && ((grp_Car_Stat[i].g_run_statusf & rs_UL) != 0)) )
					{			 // Car reached the recall floor
						grp_empf[i] = ep_wait_door;	// Make sure all cars are waiting on Emergency Power
					}
				}

				if( (((grp_Car_Stat[i].g_run_statusf & rs_DNR) != 0) || ((grp_Car_Stat[i].g_run_statusf & rs_UP) != 0) || ((grp_Car_Stat[i].g_run_statusf & rs_RUN) != 0)) &&
				    (Car_Not_Talking[i] == 0) )
				{
					if (grp_empf[i] != ep_switch)
						cars_in_motion ++;  // Count the cars in motion that are not selected to run
				}
			}
			man_sel_seq = 2;
			break;

		case 2:
			// Manual car Selection
			i = 1;
			while ((i <= cons[nmbcars]) &&
						((cars_in_motion + cars_selected ) < fvars[fvepcars] + xcars_sel_from_eps)) 
			{		// Need to select a car
				if (grp_empf[i] == ep_wait_door)
				{		   // Car is not selected
					if (man_sel_req[i] == 1)   // Selection switch request
					{
						grp_empf[i] = ep_switch;   // Select this car 
						cars_selected++;
					}
				}
				i++;
			}
			man_sel_seq = 1;
			break;
		default:
			break;
	}

}
	
void set_recall_order (void)
{
	int16 i, j, car_ix;

	j = 0;
	if ((cons[Ser_IGEP] == 1) && (igep_com_online == 1))
	{
		for(i=1;i<=cons[nmbcars];i++)
		{
			car_recall_order[i] = igep_car_recall_order[i];
	 		if (car_recall_order[i] != 0)
				j++;
		}
	}
	
	if (j != 0)
	{			// valid data on serial comm
		car_recall_en = 1;
		nmb_recall_cars = j;
	}
	else
	{		// else use parameters

		car_ix = fvars[fvretcar];

		j = 0;
		if (car_ix == 0)
		{
			for(i=1;i<=cons[nmbcars];i++)
				car_recall_order[i] = 0;
			car_recall_en = 0;
		}
		else if (cons[EPSEL_tables] == 1)
		{
			for(i=1;i<=cons[nmbcars];i++)
			{
				car_recall_order[j+1] = car_recall_order_fixed[j+1];
				j++;
			}
			car_recall_en = 1;
		}
		else
		{
			for(i=1;i<=cons[nmbcars];i++)
			{
				car_recall_order[j+1] = car_ix;	  // with fvretcar2 = 7; order array are packed consecutive i.e. cars 7,8,3,5 will show up in locations j = 0,1,2,3
				j++;
				car_ix++;
				if (car_ix > cons[nmbcars])
					car_ix = 1;
			}
			car_recall_en = 1;
		}
		nmb_recall_cars = j;
	}
}

// ********************************************
// emergency power automatic recall procedure
// ********************************************
void recall_cars(void)
{
	int16 i;
	bool dl_open = 0;
	bool gs_open = 0;
	bool door_open = 0;
	bool ck_out_of_svc = 0;


	switch (recall_seq)
	{
		case 0: 		// initialize variables
			set_recall_order();
			for(i=1;i<=cons[nmbcars];i++)
			{
				grp_empf[i] = ep_wait;		  // make sure all cars are on Emergency Power
			}
			recalled_cars = 0;
			cars_recalling = 0;
			recall_attempt = 0;		// restart recall attempts
			recall_finished = 0;
			cars_selected = 0;
			if (timers[teprcldly] >= fvars[fveprcldly])
			{
				if (car_recall_en == 0)
				{
					for(i=1;i<=cons[nmbcars];i++)
					{
						grp_empf[i] = ep_home;		  // all cars to be selected 
					}
					recall_seq = 5;
				}
				else
					recall_seq = 1;
			}
			break;
		case 1:			// Verify that the number of cars in motion	is not more than permitted
			cars_in_motion = 0;
			for(i=1;i<=nmb_recall_cars;i++)
			{
				if (car_recall_order[i] != 0)
				{
					if( (((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_DNR) != 0) || ((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_UP) != 0) || 
						((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_RUN) != 0)) &&
					    (Car_Not_Talking[car_recall_order[i]] == 0) ) 
					{				   // Car is moving
						if (grp_empf[car_recall_order[i]] != ep_recall)
							cars_in_motion ++;  // Count the cars in motion that are not recalling
					}
				}
			}				
			if (cars_in_motion < fvars[fvepcars])
				recall_seq = 2;
			break;		
		case 2:			// Recall Cars
			i = 1;
			while ((i <= nmb_recall_cars) && ((cars_recalling + cars_in_motion) < fvars[fvepcars]))
			{
				if (car_recall_order[i] != 0)
				{
					if ((grp_empf[car_recall_order[i]] == ep_wait) || (grp_empf[car_recall_order[i]] == ep_wait_door))
					{			 // car has not been select to recall but can be recalled	  
						grp_empf[car_recall_order[i]] = ep_recall;	 // Tell car to recall
						timers[tepcar+car_recall_order[i]] = 0; // reset car timer
						timers[teprcvr+car_recall_order[i]] = 0;
						run_cnt[car_recall_order[i]] = 0;
						run_cnt_latch[car_recall_order[i]] = 0;
						cars_recalling++;
					}
				}
				i++;
			}
			recall_seq = 3;
			break;
		case 3:		// Check for recall finished
			for(i=1;i<=nmb_recall_cars;i++)
			{
				if (car_recall_order[i] != 0)
				{
					if (grp_empf[car_recall_order[i]] == ep_recall)
					{   // Car is in a recall sequence

						if ( ( ((grp_Car_Stat[car_recall_order[i]].g_empf & 0x07) == ep_home)	|| 
							 	((grp_Car_Stat[car_recall_order[i]].g_empf & 0x07) == ep_home_close) )	||
							 ( ((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_RUN) == 0) && (grp_Car_Stat[car_recall_order[i]].g_servf == s_MED_EMS) &&
							 	((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_DL) != 0) && ((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_UL) != 0)) )
						{			 // Car reached the recall floor
							grp_empf[car_recall_order[i]] = ep_home;   // Set that the car successfully recalled
							if (cars_recalling > 0)
								cars_recalling--;
							recalled_cars++;
						}
						else
						{
							if (((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_DLT) != 0) && ((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_DLB) != 0) && 
								((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_DLM) != 0) &&
								(((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_RLM) != 0) || (rear_op[car_recall_order[i]] == 0)))
								dl_open = 0;
							else
								dl_open = 1;
									
							if (((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_GS) != 0) && (((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_RGS) != 0)|| (rear_op[car_recall_order[i]] == 0)))		
								gs_open = 0;
							else
								gs_open = 1;

							if ((dl_open == 1) && (gs_open == 1))		// both gate and lock must be open for the door to be open
								door_open = 1;
							else
								door_open = 0;

							if ((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_RUN) != 0)
							{
								timers[teprcvr+car_recall_order[i]] = 0;	   // car is running so clear timer
								if ((run_cnt_latch[car_recall_order[i]] == 0) && (run_cnt[car_recall_order[i]] < 100))
									run_cnt[car_recall_order[i]]++;
								run_cnt_latch[car_recall_order[i]] = 1;
							}
							else
								run_cnt_latch[car_recall_order[i]] = 0;

							if (timers[tepcar+car_recall_order[i]] > fvars[fvrettim])
							{
								grp_empf[car_recall_order[i]] = ep_timed_out;	// car timed-out
								record_fault((uint16)(f_ep_to_car + car_recall_order[i]));		// set error for car timed out
								if (cars_recalling > 0)
									cars_recalling--;
								recalled_cars++;
							}
							else if ((timers[tepcar+car_recall_order[i]] > 20) && (Car_Not_Talking[car_recall_order[i]] == 1))
							{					 // Car not communicating for 2 seconds
								grp_empf[car_recall_order[i]] = ep_out_of_svc;	// car is out of service during recall sequence
								record_fault((uint16)(f_ep_ots_car + car_recall_order[i]));		// set error for car out of service
								if (cars_recalling > 0)
									cars_recalling--;
								recalled_cars++;
							}
							else if ((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_RUN) == 0)		// car is not moving 
							{
								ck_out_of_svc = 0;
								if (timers[teprcvr+car_recall_order[i]] > fvars[fvrcvrtim]) 	// timed out
									ck_out_of_svc = 1;
								else if (((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_DL) != 0) && ((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_UL) != 0) && (door_open == 1))	
								{			  // car is dead level and door is open
									if (grp_Car_Stat[car_recall_order[i]].g_pos == fvars[fvepfl])
									{								 // At the recall floor
										if ((fvars[fvepRclflDO] == 0) || (recall_attempt > 0))	  // car is at the recall floor
											ck_out_of_svc = 1;
									}
									else 
									{		// Not at the recall floor
										if ((fvars[fvepNRclflDO] == 0) || (run_cnt[car_recall_order[i]] > 0))  	 // car not at the floor 
											ck_out_of_svc = 1;		 // car has moved the count number of times (can also be zero)
									}
								}
								if (ck_out_of_svc == 1)
								{
									if ((grp_Car_Stat[car_recall_order[i]].g_servf == 0) || (grp_Car_Stat[car_recall_order[i]].g_servf == s_IND) ||	   // out of service or independent service
								    	(grp_Car_Stat[car_recall_order[i]].g_servf == s_ATT) || (grp_Car_Stat[car_recall_order[i]].g_servf == s_FIRE2) ||	   // attendant service or fire phase 2 service
								    	(grp_Car_Stat[car_recall_order[i]].g_servf == s_EAQ) || (grp_Car_Stat[car_recall_order[i]].g_servf == s_STALLED) ||	   // earth quake service or Stalled operation
								    	(grp_Car_Stat[car_recall_order[i]].g_servf == s_LBY_RCL) || (grp_Car_Stat[car_recall_order[i]].g_servf == s_ELEV_OFF_CS) ||   // return to lobby or CS Elevator Off
								    	(grp_Car_Stat[car_recall_order[i]].g_servf == s_ELEV_OFF_HS) || (grp_Car_Stat[car_recall_order[i]].g_servf == s_LOW_PRES) ||   // HS Elevator Off or Low Pressure Operation
								    	(grp_Car_Stat[car_recall_order[i]].g_servf == s_HSV) || (grp_Car_Stat[car_recall_order[i]].g_servf == s_CAR_OFF) ) 	   // Hospital Service or CEOF Operation
									{			   // Allow car timer in seconds to make sure it is dead level and
												   // out of service (ind, att, ins, phase 2, stall/low oil)
										grp_empf[car_recall_order[i]] = ep_out_of_svc;	// car is out of service during recall sequence
										record_fault((uint16)(f_ep_ots_car + car_recall_order[i]));		// set error for car out of service
										if (cars_recalling > 0)
											cars_recalling--;
										recalled_cars++;
									}
								}
							}
						}
					}
					else if (grp_empf[car_recall_order[i]] == ep_wait)
					{
						grp_empf[car_recall_order[i]] = ep_wait_door;		// have other cars open their doors
						timers[teprcvr+car_recall_order[i]] = 0;
						run_cnt[car_recall_order[i]] = 0;
						run_cnt_latch[car_recall_order[i]] = 0;
					}
				}
			}												
			if (recalled_cars < nmb_recall_cars)
				recall_seq = 1;
			else
			{
				recall_attempt++;
				recall_seq = 4;
			}
			break;
		case 4:			  // If all cars did not recall then attempt again
			if (recall_attempt >= 2)
				recall_seq = 5;
			else
			{
				recall_seq = 5;
		  		i = 1;
				while (i <= nmb_recall_cars)
				{
					if (car_recall_order[i] != 0)
					{
						if ((grp_empf[car_recall_order[i]] == ep_out_of_svc) || (grp_empf[car_recall_order[i]] == ep_timed_out))
						{			 // car failed recall so try again	  
							grp_empf[car_recall_order[i]] = ep_wait_door;	 // allow car to try again
							if (recalled_cars > 0)
								recalled_cars--;
							recall_seq = 1;					// set sequence to try again
						}
					}
					i++;
				}
			}
			break;
		case 5:			// Select Car
			recall_seq = 5;
			recall_finished = 1;
			break;
		default:
			break;
	}
}

void set_select_car_order (void)
{
	int16 i, j, car_ix;

	j = 0;
	if ((cons[Ser_IGEP] == 1) && (igep_com_online == 1))
	{
		for(i=1;i<=cons[nmbcars];i++)
		{
			car_select_order[i] = igep_car_select_order[i];
	 		if (car_select_order[i] != 0)
				j++;
		}
	}
	if (j != 0)
	{			// valid data on serial comm
		car_select_en = 1;
		nmb_select_cars = j;
	}
	else
	{		// else use parameters

		car_ix = fvars[fvselcar];

		j = 0;
		if (car_ix == 0)
		{
			for(i=1;i<=cons[nmbcars];i++)
				car_select_order[j] = 0;
			car_select_en = 0;
		}
		else if (cons[EPSEL_tables] == 1)
		{
			for(i=1;i<=cons[nmbcars];i++)
			{
				car_select_order[j+1] = car_select_order_fixed[j+1];	   
				j++;
			}
			car_select_en = 1;
		}
		else
		{
			for(i=1;i<=cons[nmbcars];i++)
			{
				car_select_order[j+1] = car_ix;	   // with fvselcar2 = 7; order array are packed consecutive i.e. cars 7,8,3,5 will show up in locations j = 0,1,2,3
				j++;
				car_ix++;
				if (car_ix > cons[nmbcars])
					car_ix = 1;
			}
			car_select_en = 1;
		}
		nmb_select_cars = j;
	}
}
// ********************************************
// emergency automatic car selection procedure
// ********************************************
void select_cars(void)
{
	int16 i;

	switch (select_seq)
	{
		case 0:
			set_select_car_order();
			if (car_select_en != 0)
			{			// Allowed to select a car
				cars_selected = 0;
				select_seq = 1;
			}
			break;

		case 1:	   // Make sure there are no cars in motion that should not be
			cars_in_motion = 0;
			// Select Fire phase 2 Next highest priority
			fs2_car = 0;
			hsv_car = 0;
			for(i=1;i<=nmb_select_cars;i++)
			{
				if (car_select_order[i] != 0)
				{
					if 	(valid_ep_car[car_select_order[i]] == 1)	 // this car is allowed to run
					{
						if (grp_empf[car_select_order[i]] != ep_select)
						{
							
							if (grp_Car_Stat[car_select_order[i]].g_servf == s_FIRE2)		// fire phase 2	car not selected
								fs2_car++;	   // Number of fs2 cars not selected
							else if (grp_Car_Stat[car_select_order[i]].g_servf == s_HSV) // hospital service car not selected
								hsv_car++;		// Number of hsv cars not selected
						}
					}
				}
			}

			for(i=1;i<=nmb_select_cars;i++)
			{
				if (car_select_order[i] != 0)
				{

					if ((grp_Car_Stat[car_select_order[i]].g_servf == 0) ||
					   (grp_Car_Stat[car_select_order[i]].g_servf == s_STALLED) || 	  // low oil
					   (grp_Car_Stat[car_select_order[i]].g_servf == s_LBY_RCL) || 	  // return to lobby
					   (grp_Car_Stat[car_select_order[i]].g_servf == s_ELEV_OFF_CS) || 	  // CS Elev Off
					   (grp_Car_Stat[car_select_order[i]].g_servf == s_ELEV_OFF_HS) || 	  // HS Elev Off
					   (grp_Car_Stat[car_select_order[i]].g_servf == s_CAR_OFF) || 	  // CEOF Elev Off
					   (grp_Car_Stat[car_select_order[i]].g_servf == s_LOW_PRES) ||	  // Low Pressure
					   (((grp_Car_Stat[car_select_order[i]].g_servf == s_F1MAIN) || (grp_Car_Stat[car_select_order[i]].g_servf == s_F1ALT)) 	 // phase 1 recall and at the fire floor
					   			&& (grp_Car_Stat[car_select_order[i]].g_firef == 3) && ((fs2_car > 0) || (hsv_car > 0))) ||
					   ((grp_Car_Stat[car_select_order[i]].g_servf != s_FIRE2) && (grp_Car_Stat[car_select_order[i]].g_servf != s_HSV) && 
					   		(grp_Car_Stat[car_select_order[i]].g_servf != s_F1MAIN) && (grp_Car_Stat[car_select_order[i]].g_servf != s_F1ALT) && ((fs2_car > 0) || (hsv_car > 0))) ||
					   ((grp_Car_Stat[car_select_order[i]].g_servf == s_FIRE2) && (hsv_car > 0)) )
					{		 // Car is out of service
						if ((grp_empf[car_select_order[i]] == ep_select) && 
							((timers[tepcar+car_select_order[i]] > fvars[fvrcvrtim]) || 
								(((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_DZ) != 0) && (Car_Not_Talking[car_select_order[i]] == 0))) )
						{	  // This car had been selected and is in the door zone
							grp_empf[car_select_order[i]] = ep_home;
							if (cars_selected > 0)
								cars_selected--;
						}
					}
					else
						timers[tepcar+car_select_order[i]] = 0;


					if( (((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_DNR) != 0) || ((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_UP) != 0) || 
						((grp_Car_Stat[car_recall_order[i]].g_run_statusf & rs_RUN) != 0) ) &&
					    (Car_Not_Talking[car_select_order[i]] == 0) ) 
					{					// car is moving
						if (grp_empf[car_select_order[i]] != ep_select)
							cars_in_motion ++;  // Count the cars in motion that are not selected
					}
				}
			}			
			select_seq = 2;
			break;

		case 2:	  // Select cars that were previously recalled

			// Select Hospital Service Cars First
			i = 1;
			while ((i <= nmb_select_cars) && ((cars_selected + cars_in_motion) < fvars[fvepcars]))
			{
				if (car_select_order[i] != 0)
				{
					if 	((valid_ep_car[car_select_order[i]] == 1) &&	 // this car is allowed to run
							(grp_Car_Stat[car_select_order[i]].g_servf == s_HSV))		// Or Hospital Service
					{				   // car recalled successfully and in service
						grp_empf[car_select_order[i]] = ep_select;
						cars_selected++;
					}
				}
				i++;
			}

			// Select Fire phase 2 Next highest priority
			i = 1;
			while ((i <= nmb_select_cars) && ((cars_selected + cars_in_motion) < fvars[fvepcars]))
			{
				if (car_select_order[i] != 0)
				{
					if 	((valid_ep_car[car_select_order[i]] == 1) &&	 // this car is allowed to run
							(grp_Car_Stat[car_select_order[i]].g_servf == s_FIRE2))		// Or on fire phase 2
					{				   // car recalled successfully and in service
						grp_empf[car_select_order[i]] = ep_select;
						cars_selected++;
					}
				}
				i++;
			}

			//Select attendant cars as long as field variables is turned on
			i = 1;
			while ((fvars[fvattempcar] == 1)&&(i <= nmb_select_cars) && ((cars_selected + cars_in_motion) < fvars[fvepcars]))
			{
				if (car_select_order[i] != 0)
				{
					if 	((valid_ep_car[car_select_order[i]] == 1) &&	 // this car is allowed to run
							(grp_Car_Stat[car_select_order[i]].g_servf == s_ATT))		// Or on fire phase 2
					{				   // car recalled successfully and in service
						grp_empf[car_select_order[i]] = ep_select;
						cars_selected++;
					}
				}
				i++;
			}

			i = 1;
			while ((i <= nmb_select_cars) && ((cars_selected + cars_in_motion) < fvars[fvepcars]))
			{
				if (car_select_order[i] != 0)
				{
					if (((grp_empf[car_select_order[i]] == ep_home) &&
						(valid_ep_car[car_select_order[i]] == 1) &&	 // this car is allowed to run
						(grp_Car_Stat[car_select_order[i]].g_servf != s_STALLED) &&		 // not low oil
						(grp_Car_Stat[car_select_order[i]].g_servf != s_LBY_RCL) &&		 // not return to lobby
						(grp_Car_Stat[car_select_order[i]].g_servf != s_ELEV_OFF_CS) &&		 // not CS Elev Off
						(grp_Car_Stat[car_select_order[i]].g_servf != s_ELEV_OFF_HS) &&		 // not HS Elev Off
					    (grp_Car_Stat[car_select_order[i]].g_servf != s_CAR_OFF) &&	 	 // not CEOF Elev Off
						(grp_Car_Stat[car_select_order[i]].g_servf != s_LOW_PRES) &&		 // not Low Pressure
						(grp_Car_Stat[car_select_order[i]].g_servf != 0)))		 // not out of service
					{				   // car recalled successfully and in service
						grp_empf[car_select_order[i]] = ep_select;
						cars_selected++;
					}
				}
				i++;
			}	

			if (cars_selected > fvars[fvepcars])
			{	   // selected too many cars (parameter could have been changed)
				for(i=1;i<=nmb_select_cars;i++)
				{
					if (car_select_order[i] != 0)
					{
						if ((grp_empf[car_select_order[i]] == ep_select) || (grp_empf[car_select_order[i]] == ep_recall))	  
							grp_empf[car_select_order[i]] = ep_home;	  // Unselect all the cars that are selected
					}
				}
				cars_selected = 0;
			}
			else
				select_seq = 1;	 
			break;
		default:
			break;
	}
}


/* Revision History

9/22/11 v5.54.25 mhd	1. Modified emergency power operation for split power feeders.  All control veriables became arrays to allow for two feeders.
5/9/12 5.57.76	hdl		1. 	Added ep_auto_select_en_prev to keep history of auto recall from igep interface. This way controllers can recall if deselected
12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
							8/7/14 v6.1.63 hdl		1. Added xcars_sel_from_eps. They are manual selected cars that should not be conted for number of emergency cars in service
														normal cars from table are selected automatically but more cars could run in emp if selected (intergroup)
3/27/17 V8.0.2 mhd		1. Deleted split feeder for hydro car.

*/
