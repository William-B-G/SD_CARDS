#define d_GPARKING 1
#include "global.h"

int16 grp_parking_floor[9];
int16 prk_floor_full[9] = {0,0,0,0,0,0,0,0,0};
int16 car_start_latch[9] = {0,0,0,0,0,0,0,0,0};
int16 park_fl_car[9] = {0,0,0,0,0,0,0,0,0};
int16 park_car_fl_ix[9] = {0,0,0,0,0,0,0,0,0};
int16 how_many[9] = {0,0,0,0,0,0,0,0,0};
int16 park_dist[9] = {0,0,0,0,0,0,0,0,0};
int16 best_car;
int16 best_rp_car=0;
int16 prev_best_car = 0;
int16 prev_best_rp_car = 0;
int16 cars_being_parked = 0;
int16 nmb_parked_cars = 0;
uint16 parking_fl_taken[fl_size];

int16 g_park[car_size];

int16 cars_to_park = 0;

uint16 alt_park_inp = 0;

void group_park(void );
void park_car(int16 park_fl_ix);
int16 best_parking_car(int16 parking_fl);
int16 best_reparking_car(int16 parking_fl,int16 parked_fl,int16 how_many);


// ***************************************
// This is group parking function
//   Down peak is used to move the cars 
//   away from the lobby
// ***************************************
void group_park()
{
	int16 j,i,time_period;
	int16 fl_ix=0;
	static int16 second_cnt;
	int16 parking_fl;

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


	if (cons[Park_NCU_ALB] == 1) 
	{
		if ((cons[sercom] & 0x20) == 0)
		{		   // no serial hall call boards
			if ((cons[epgrp] != 0) && (cons[LbyUpReq] != 0))
				alt_park_inp = ((gc_uc[cons[grtopf]/32] & fl_mask[cons[grtopf]+2]) != 0);
			else if ((cons[epgrp] != 0) || (cons[LbyUpReq] != 0))
				alt_park_inp = ((gc_uc[cons[grtopf]/32] & fl_mask[cons[grtopf]+1]) != 0);
			else
				alt_park_inp = ((gc_uc[cons[grtopf]/32] & fl_mask[cons[grtopf]]) != 0);
		}
		else
			alt_park_inp = (gpi[g_invalid_io_loc] & 0x40) != 0;
	}
	else 
		alt_park_inp = 0;

	if ((cons[home] > 0) && (cons[nmbcars] == 1))
		cars_to_park = 1;
	else if ((dn_peak == 1) && (cons[nmbcars] > 1))
	{
		if (fvars[fvdnpkpool] < cons[nmbcars])
			cars_to_park = cons[nmbcars] - fvars[fvdnpkpool];
		else
			cars_to_park = 1;
	}
	else if ((up_peak == 1) && (cons[nmbcars] > 1))
	{
		if (fvars[fvuppkpool] > 0)
			cars_to_park = fvars[fvuppkpool];
		else
			cars_to_park = 0;
	}	
	else if ((fvars[fvpark] >= 0) && (fvars[fvpark] <= cons[nmbcars]))
	{
		if (fvars[fvpark] > 1)
			cars_to_park = fvars[fvpark];
		else if ((tmr_svc[s_parking] == 1) && (fvars[fvgsvcpark] >= 1) && (fvars[fvgsvcpark] <= cons[nmbcars]))
			cars_to_park = fvars[fvgsvcpark];
		else if (lobby_request == 0)
		{
			if ( (fvars[fvpark] > 0) || (next_up != 0)  )
				cars_to_park = 1; 
			else
				cars_to_park = 0;
		}
		else 			// 	if lobby request is set the lobby request routine will park the car
			cars_to_park = 0;
	}
	else
		cars_to_park = 0;

	if (cars_to_park == 0)
	{
		for(i=1; i<=cons[nmbcars]; i++)
			g_park[i] = 0;
		return;
	}
//	local_gettime();		// cannot be called from the group, the car will update the t and d structures
	time_period = (int16)((t.hour*4)+(t.minute/15));

	if ((dn_peak == 1) && (cons[nmbcars] > 1) && (up_peak == 0))
		grp_parking_floor[0] = cons[grtopf];
	else if( (alt_park_inp == 1) || (ids_alt_park == 1) || (ui_alt_park == 1) || (tmr_svc[s_alt_park] == 1))
		grp_parking_floor[0] = fvars[fvaltpark];
	else
		grp_parking_floor[0] = lobby_fl;

	if (((fvars[fvdnpkctl] & 0x01) != 0) && (dn_peak == 1))
	{
		if ((cons[nmbcars] > 1) && (up_peak == 0))
		{
			if (nmb_dn_call[time_period][0] >= fvars[fvdptrigcnt]) 
			{
				for(i=2; i<cons[grtopf]; i++)
					parking_fl_taken[i] = 0;

				for(j=0;j<cars_to_park;j++)
				{
					grp_parking_floor[j] = 2;
					for(i=2; i<cons[grtopf]; i++)
					{
						if( (nmb_dn_call[time_period][i+1] > nmb_dn_call[time_period][grp_parking_floor[j]]) &&
							(nmb_dn_call[time_period][i+1] >= (fvars[fvdptrigcnt]>>2)) &&
							(parking_fl_taken[i+1] == 0) )
							grp_parking_floor[j] = i+1;
					}
					if ((grp_parking_floor[j] == 2) && (nmb_dn_call[time_period][2] <  fvars[fvdptrigcnt]))
						grp_parking_floor[j] = 0;
					else
						parking_fl_taken[grp_parking_floor[j]] = 1;
				}
			}
			else 
			{
				for (i=1;i<cars_to_park;i++)
				{
					if ((cons[grtopf] - i) > 1)
						grp_parking_floor[i] = cons[grtopf] - i;
				}
			}
		}
	}
	else if ( (fvars[fvparktype]  == 0) &&
		((nmb_up_call[time_period][0]+nmb_dn_call[time_period][0]) > 10) )
	{
	
		//grp_parking_floor[0] is set to lobby by default. The following allows
		//the grp_parking_floor[0] to be any floor if parking width is nonzero
		if(fvars[fvparkwidth] != 0)
		{
			for(i=1; i<=cons[grtopf]; i++)
			{
				if( (nmb_up_call[time_period][i]+nmb_dn_call[time_period][i]) >
							(nmb_up_call[time_period][grp_parking_floor[0]]+nmb_dn_call[time_period][grp_parking_floor[0]]) )
					grp_parking_floor[0] = i;
			}	
		}

		if (fvars[fvparkfl1] != 0)
			grp_parking_floor[1] = fvars[fvparkfl1];
		else
			grp_parking_floor[1] = cons[grtopf];

		for(i=1; i<=cons[grtopf]; i++)
		{
			if(i != grp_parking_floor[0])
			{
				if( (nmb_up_call[time_period][i]+
					 nmb_dn_call[time_period][i]) >
					(nmb_up_call[time_period][grp_parking_floor[1]]+
					 nmb_dn_call[time_period][grp_parking_floor[1]]) )
						grp_parking_floor[1] = i;
			}
		}
		if(cons[nmbcars] > 2)
		{
		    if (fvars[fvparkfl2] != 0)
				grp_parking_floor[2] = fvars[fvparkfl2];
			else
			{
				for(i=1; i<=cons[grtopf]; i++)
				{
					if((i != grp_parking_floor[0])&&(i != grp_parking_floor[1]))
					{
						grp_parking_floor[2] = i;
						i = (cons[grtopf]+1);
					}
				}
				for(i=1; i<=cons[grtopf]; i++)
				{
					if((i != grp_parking_floor[0])&&(i != grp_parking_floor[1]))
					{
						if( (nmb_up_call[time_period][i]+nmb_dn_call[time_period][i]) >
							(nmb_up_call[time_period][grp_parking_floor[2]]+
							nmb_dn_call[time_period][grp_parking_floor[2]]) )
								grp_parking_floor[2] = i;
					}
				}
			}
		}
		if(cons[nmbcars] > 3)
		{
			if (fvars[fvparkfl3] != 0)
				grp_parking_floor[3] = fvars[fvparkfl3];
			else
			{
				for(i=1; i<=cons[grtopf]; i++)
				{
					if((i != grp_parking_floor[0])&&(i != grp_parking_floor[1])&&
					   (i != grp_parking_floor[2]))
					{
						grp_parking_floor[3] = i;
						i = (cons[grtopf]+1);
					}
				}
				for(i=1; i<=cons[grtopf]; i++)
				{
					if((i != grp_parking_floor[0])&&(i != grp_parking_floor[1])&&
					   (i != grp_parking_floor[2]))
					{
						if( (nmb_up_call[time_period][i]+
							nmb_dn_call[time_period][i]) >
							(nmb_up_call[time_period][grp_parking_floor[3]]+
							nmb_dn_call[time_period][grp_parking_floor[3]]) )
								grp_parking_floor[3] = i;
					}
				}
			}
		}
		if(cons[nmbcars] > 4)
		{
			if (fvars[fvparkfl4] != 0)
				grp_parking_floor[4] = fvars[fvparkfl4];
			else
			{
				for(i=1; i<=cons[grtopf]; i++)
				{
					if ( (i != grp_parking_floor[0]) && (i != grp_parking_floor[1]) &&
					   	 (i != grp_parking_floor[2]) && (i != grp_parking_floor[3]) )
					{
						grp_parking_floor[4] = i;
						i = (cons[grtopf]+1);
					}
				}
				for(i=1; i<=cons[grtopf]; i++)
				{
					if ( (i != grp_parking_floor[0]) && (i != grp_parking_floor[1]) &&
					   	 (i != grp_parking_floor[2]) && (i != grp_parking_floor[3]) )
					{
						if( (nmb_up_call[time_period][i]+
							nmb_dn_call[time_period][i]) >
							(nmb_up_call[time_period][grp_parking_floor[4]]+
							nmb_dn_call[time_period][grp_parking_floor[4]]) )
								grp_parking_floor[4] = i;
					}
				}
			}
		}
		if(cons[nmbcars] > 5)
		{
			if (fvars[fvparkfl5] != 0)
				grp_parking_floor[5] = fvars[fvparkfl5];
			else
			{
				for(i=1; i<=cons[grtopf]; i++)
				{
					if ( (i != grp_parking_floor[0]) && (i != grp_parking_floor[1]) &&
					   	 (i != grp_parking_floor[2]) && (i != grp_parking_floor[3]) &&
					     (i != grp_parking_floor[4]) )
					{
						grp_parking_floor[5] = i;
						i = (cons[grtopf]+1);
					}
				}
				for(i=1; i<=cons[grtopf]; i++)
				{
					if ( (i != grp_parking_floor[0]) && (i != grp_parking_floor[1]) &&
					   	 (i != grp_parking_floor[2]) && (i != grp_parking_floor[3]) &&
					     (i != grp_parking_floor[4]) )
					{
						if( (nmb_up_call[time_period][i]+
							nmb_dn_call[time_period][i]) >
							(nmb_up_call[time_period][grp_parking_floor[5]]+
							nmb_dn_call[time_period][grp_parking_floor[5]]) )
								grp_parking_floor[5] = i;
					}
				}
			}
		}
		if(cons[nmbcars] > 6)
		{
			if (fvars[fvparkfl6] != 0)
				grp_parking_floor[6] = fvars[fvparkfl6];
			else
			{
				for(i=1; i<=cons[grtopf]; i++)
				{
					if ( (i != grp_parking_floor[0]) && (i != grp_parking_floor[1]) &&
					   	 (i != grp_parking_floor[2]) && (i != grp_parking_floor[3]) &&
					     (i != grp_parking_floor[4]) && (i != grp_parking_floor[5]) )
					{
						grp_parking_floor[6] = i;
						i = (cons[grtopf]+1);
					}
				}
				for(i=1; i<=cons[grtopf]; i++)
				{
					if ( (i != grp_parking_floor[0]) && (i != grp_parking_floor[1]) &&
					   	 (i != grp_parking_floor[2]) && (i != grp_parking_floor[3]) &&
					     (i != grp_parking_floor[4]) && (i != grp_parking_floor[5]) )
					{
						if( (nmb_up_call[time_period][i]+
							nmb_dn_call[time_period][i]) >
							(nmb_up_call[time_period][grp_parking_floor[5]]+
							nmb_dn_call[time_period][grp_parking_floor[5]]) )
								grp_parking_floor[6] = i;
					}
				}
			}
		}
		else if(cons[nmbcars] > 7)
		{
			if (fvars[fvparkfl7] != 0)
				grp_parking_floor[7] = fvars[fvparkfl7];
			else
			{
				for(i=1; i<=cons[grtopf]; i++)
				{
					if ( (i != grp_parking_floor[0]) && (i != grp_parking_floor[1]) &&
					   	 (i != grp_parking_floor[2]) && (i != grp_parking_floor[3]) &&
					   	 (i != grp_parking_floor[4]) && (i != grp_parking_floor[5]) &&
					     (i != grp_parking_floor[6]) )
					{
						grp_parking_floor[7] = i;
						i = (cons[grtopf]+1);
					}
				}
				for(i=1; i<=cons[grtopf]; i++)
				{
					if ( (i != grp_parking_floor[0]) && (i != grp_parking_floor[1]) &&
					   	 (i != grp_parking_floor[2]) && (i != grp_parking_floor[3]) &&
					   	 (i != grp_parking_floor[4]) && (i != grp_parking_floor[5]) &&
					     (i != grp_parking_floor[6]) )
					{
						if( (nmb_up_call[time_period][i]+
							nmb_dn_call[time_period][i]) >
							(nmb_up_call[time_period][grp_parking_floor[5]]+
							nmb_dn_call[time_period][grp_parking_floor[5]]) )
								grp_parking_floor[7] = i;
					}
				}
			}
		}
	}
	else if	((fvars[fvparktype] == 2) && 
			 ((fvars[fvparkfl1] != 0) || (fvars[fvparkfl2] != 0) || 
			 (fvars[fvparkfl3]!= 0) || (fvars[fvparkfl4] |= 0) || 
			 (fvars[fvparkfl5]!= 0) || (fvars[fvparkfl6] |= 0) || 
			 (fvars[fvparkfl7] != 0)))
	{
		grp_parking_floor[1] = fvars[fvparkfl1];
		grp_parking_floor[2] = fvars[fvparkfl2];
		grp_parking_floor[3] = fvars[fvparkfl3];
		grp_parking_floor[4] = fvars[fvparkfl4];
		grp_parking_floor[5] = fvars[fvparkfl5];
		grp_parking_floor[6] = fvars[fvparkfl6];
		grp_parking_floor[7] = fvars[fvparkfl7];
	}
	else
	{	 // Default if number of calls not setup, no parking floor setup, or if (fvars[fvparktype] == 1)
		if((cons[nmbcars] > 2) && (cars_to_park > 2))
		{
			grp_parking_floor[1] = cons[grtopf];

			for (i=2;i<=cars_to_park;i++)
			{
				grp_parking_floor[i] = (cons[bottomf] +
					   (((i-1) * (cons[grtopf]))/cars_to_park));
			}
		}
		else
		{
			if (fvars[fvparkfl1] != 0)
				grp_parking_floor[1] = fvars[fvparkfl1];
			else
				grp_parking_floor[1] = ((cons[grtopf] / 2) + 1);
		}
	}

	if((cons[nmbcars] < 8) || (cars_to_park < 8))
		grp_parking_floor[7] = 0;
	if((cons[nmbcars] < 7) || (cars_to_park < 7))
		grp_parking_floor[6] = 0;
	if((cons[nmbcars] < 6) || (cars_to_park < 6))
		grp_parking_floor[5] = 0;
	if((cons[nmbcars] < 5) || (cars_to_park < 5))
		grp_parking_floor[4] = 0;
	if((cons[nmbcars] < 4) || (cars_to_park < 4))
		grp_parking_floor[3] = 0;
	if((cons[nmbcars] < 3) || (cars_to_park < 3))
		grp_parking_floor[2] = 0;
	if((cons[nmbcars] < 2) || (cars_to_park < 2))
		grp_parking_floor[1] = 0;


	if (((fvars[fvdnpkctl] & 0x01) == 0) || (dn_peak == 0))
	{
		for(i=1; i<=cons[nmbcars]; i++)
		{
			if( (grp_Car_Stat[i].g_servf != s_HOMING) ||
				(grp_Car_Stat[i].g_procf == 14) )
			{
				g_park[i] = 0;
			}
		}

		for(i=1; i<=cons[nmbcars]; i++)
		{
			if( (grp_Car_Stat[i].g_servf == s_HOMING) &&
				(grp_Car_Stat[i].g_procf < 14) )           // homing
			{
				timers[tparking] = 0;						// park one car at a time
				prev_best_car = 0;
				prev_best_rp_car = 0;
				return;
			}
		}
	}

	nmb_parked_cars = 0;
	for(j=0; j<cons[nmbcars]; j++)
	{
		prk_floor_full[j] = 0;
		how_many[j] = 0;
		for(i=1; i<=cons[nmbcars]; i++)
		{

			if ( (grp_Car_Stat[i].g_servf == s_AUTO) || (grp_Car_Stat[i].g_servf == s_HOMING) )    // inservice
			{
				if (grp_Car_Stat[i].g_pos > grp_parking_floor[j])
					park_dist[i] = grp_Car_Stat[i].g_pos - grp_parking_floor[j];
				else
					park_dist[i] = grp_parking_floor[j] - grp_Car_Stat[i].g_pos;
			}

			if( ((grp_Car_Stat[i].g_pos == grp_parking_floor[j]) ||
 				((park_dist[i] <= fvars[fvparkwidth]) && (fvars[fvparkwidth] != 0) && (grp_parking_floor[j] != lobby_fl))) && 
				( (grp_Car_Stat[i].g_servf == s_AUTO) || (grp_Car_Stat[i].g_servf == s_HOMING) )  &&    // inservice
				(i != ir_car) &&							// car not on ir service
				(Car_on_SR[i] == 0) &&
				(grp_parking_floor[j] != 0) && 				// not a valid parking floor
				((grp_Car_Stat[i].g_dpref == 0) ||
					((next_up != 0)  && (ncu_car == i))) )           // no calls
			{
				prk_floor_full[j] = i;
				nmb_parked_cars++;
				if (((grp_Car_Stat[i].g_dcalls == 0) &&        // no calls
					((cons[mand] == 2) || 
						((grp_Car_Stat[i].g_doorf == 0) && 			// Doors are closed
						 (grp_Car_Stat[i].g_rdoorf == 0))) && 		// Doors are closed
					(grp_Car_Stat[i].g_dirf == 0)) ||			// No direction flag
					((next_up != 0)  && (ncu_car == i)))	// Next up at the lobby
					how_many[j]++;
			}
		}
	}


	if (((fvars[fvdnpkctl] & 0x01) != 0) && (dn_peak == 1))
	{	   // Heavy Down Peak parking

		if (second_cnt >= 1)
		{
			second_cnt = 0;
			for (i=1;i<=cons[nmbcars];i++)
				car_start_latch[i]++;
		}
	
		cars_being_parked = 0;
		for(i=1; i<=cons[nmbcars]; i++)
		{
			if( (grp_Car_Stat[i].g_servf != s_HOMING) ||
				(grp_Car_Stat[i].g_procf == 14) )
			{
				if (car_start_latch[i] > 5)
				{
					if (park_fl_car[park_car_fl_ix[i]] == i)
					{
						park_fl_car[park_car_fl_ix[i]] = 0;
						park_car_fl_ix[i] = 0;
					}
					g_park[i] = 0;
				}
			}
			else if (grp_Car_Stat[i].g_servf == s_HOMING)
				cars_being_parked++;

		}
	
		for (i=1;i<=cons[nmbcars];i++)
		{
			if ((cars_being_parked + nmb_parked_cars) < cars_to_park)
			{
				parking_fl = 0;
				for (j=1;j<=cars_to_park;j++)
				{
					if ((grp_parking_floor[j-1] != 0) && (park_fl_car[j-1] == 0))
					{
						parking_fl = grp_parking_floor[j-1];
						fl_ix = j-1;
						break;
					}
				}

				if (parking_fl != 0)
				{		// need to send a car to the down peak floor
					best_car = best_parking_car(parking_fl);
					if ((best_car != 0) && (g_park[best_car] == 0))
					{
						cars_being_parked++;
						g_park[best_car] = parking_fl;
						park_fl_car[fl_ix] = best_car;
						park_car_fl_ix[best_car] = fl_ix;
						car_start_latch[best_car] = 0;
					}
				}
			}
		}
		return;			
	}	  // else normal parking
	else if ((cars_to_park >= 1) && (prk_floor_full[0] == 0) && (lobby_request == 0))	// lobby floor is empty
	{
		park_car(0);
		return;
	}
	else
	{
		for (i=2;i<=cons[nmbcars];i++)
		{
			if ((cars_to_park >= i) && (prk_floor_full[i-1] == 0))
			{
				park_car(i-1);
				return;			// do only one car at a time
			}
		}
	}
	timers[tparking] = 0;
	return;
}

// **********************************************
// Set the park variable for the car to park
// **********************************************

void park_car(int16 park_fl_ix)
{
	int16 j;

	best_car = best_parking_car(grp_parking_floor[park_fl_ix]);
	if ((best_car != 0) && (best_car == prev_best_car))
	{
		if ((timers[tparking] > fvars[fvparkdly]) || (next_up != 0) || (dn_peak == 1))		// delay for 4 seconds
			g_park[best_car] = grp_parking_floor[park_fl_ix];
		return;
	}
	prev_best_car = best_car;			// Restarts the time if best car changes
	for(j=0; j<cons[nmbcars]; j++)
	{			 // Repark extra cars at the parking floor 0
		best_rp_car = best_reparking_car(grp_parking_floor[park_fl_ix],grp_parking_floor[j],how_many[j]);
		if (best_rp_car != 0)
		{
			if (best_rp_car == prev_best_rp_car)
			{
				if ((timers[tparking] > fvars[fvparkdly]) || (dn_peak == 1))		// delay for 4 seconds
					g_park[best_rp_car] = grp_parking_floor[park_fl_ix];
				return;
			}
			break;
		}
	}
	prev_best_rp_car = best_rp_car;			// Restarts the time if best car changes
	timers[tparking] = 0;
}

//********************************************
//**  Find the best car to park
//********************************************

int16 best_parking_car(int16 parking_fl)
{
	int16 i,best_car, best_fl_dist, fl_dist;
	int16 parked_car[9] = {0,0,0,0,0,0,0,0,0};

	// Set the table of other parking floors

	for (i=0;i<=7;i++)
	{
		if (prk_floor_full[i] != 0) 
			parked_car[prk_floor_full[i]] = 1;
	}

	best_car = 0;
	best_fl_dist = cons[grtopf];

	for(i=1;i<=cons[nmbcars];i++)
	{
		if(((grp_Car_Stat[i].g_servf == s_AUTO) || (grp_Car_Stat[i].g_servf == s_HOMING)) &&   // inservice
		   (grp_Car_Stat[i].g_dcalls == 0) &&         // no calls
		   ((cons[mand] == 2) || (fvars[fvAsgParkFlDO] == 1) ||						// Car has swing doors
		   		((grp_Car_Stat[i].g_doorf == 0) &&		// Doors are closed
			   	 (grp_Car_Stat[i].g_rdoorf == 0))) && 		// Doors are closed
		   (grp_Car_Stat[i].g_dirf == 0) &&			// No direction flag
		   (grp_Car_Stat[i].g_dpref == 0) &&			// No direction preference
		   (parked_car[i] == 0) &&						// not already parked
//		   (grp_Car_Stat[i].g_pos != pk_fl[0]) &&      // not at this floor
//		   (grp_Car_Stat[i].g_pos != pk_fl[1]) &&      // not at this floor
//		   (grp_Car_Stat[i].g_pos != pk_fl[2]) &&      // not at this floor
//		   (grp_Car_Stat[i].g_pos != pk_fl[3]) &&      // not at this floor
//		   (grp_Car_Stat[i].g_pos != pk_fl[4]) &&      // not at this floor
//		   (grp_Car_Stat[i].g_pos != pk_fl[5]) &&      // not at this floor
//		   (grp_Car_Stat[i].g_pos != pk_fl[6]) &&      // not at this floor
		   ((g_park[i] == 0) || ((fvars[fvdnpkctl] & 0x01) == 0) || (dn_peak == 0)) &&
		   (i != ir_car) &&								// car not on ir service
		   (Car_on_SR[i] == 0) &&
		   ((ccmsk[i][parking_fl] == 1) || (rccmsk[i][parking_fl] == 1))  )	// valid floor
		{
			
			if (grp_Car_Stat[i].g_pos > parking_fl)
				fl_dist = grp_Car_Stat[i].g_pos - parking_fl;
			else
				fl_dist = parking_fl -	grp_Car_Stat[i].g_pos;
		}
		else
			fl_dist = cons[grtopf];

		if ((fl_dist < best_fl_dist) && ((parking_fl == lobby_fl) || 
				((fl_dist > fvars[fvparkwidth]) && (parking_fl != lobby_fl))) )
		{
			best_car = i;
			best_fl_dist = fl_dist;
		}
	}
	return best_car;
}

//*******************************************
//*** Find the best car to re-park
//*******************************************
int16 best_reparking_car(int16 parking_fl,int16 parked_fl,int16 how_many)
{
//  Re-park the floor from the parked_fl
//  to the parking_fl
	int16 i;

	if ((how_many > 1) && ((parked_fl != lobby_fl) ||  		// more than one car at the parking floor
		((how_many > lobby_request) && (parked_fl == lobby_fl))) )
	{
		for(i=1; i<=cons[nmbcars]; i++)
		{

 		    if( (grp_Car_Stat[i].g_pos == parked_fl) && // at parked floor
 				( (grp_Car_Stat[i].g_servf == s_AUTO) || (grp_Car_Stat[i].g_servf == s_HOMING) )  &&    // inservice
				(ncu_door[i] == 0) && (ncu_que[i] == 0) &&	// Car is not on next up or is not the next next up car
			   	(grp_Car_Stat[i].g_dcalls == 0) &&         // no calls
			   	((cons[mand] == 2) || (fvars[fvAsgParkFlDO] == 1) ||	// Car has swing doors
			   		((grp_Car_Stat[i].g_doorf == 0) &&		// Doors are closed
			   	 	 (grp_Car_Stat[i].g_rdoorf == 0))) && 		// Doors are closed
			   	(grp_Car_Stat[i].g_dirf == 0) &&			// No direction flag
				(grp_Car_Stat[i].g_dpref == 0) &&          // no calls
			    (i != ir_car) &&							// car not on ir service
				(Car_on_SR[i] == 0) &&
				((ccmsk[i][parking_fl] == 1) || (rccmsk[i][parking_fl] == 1))  )	// valid floor
		    {
				return i;
		    }
		}
	}
	return 0;
}

/* 	Revision History

8/23/11 v5.54.23 hdl	1. Added logic in group_park() to disable parking cars based on up peak pool. There was no logic before and the cars were being sent to park when
							they were supposed to be at the lobby for up peak operation 

11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.
12/18/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
							4/15/14 v6.1.50  hdl	1. grp_parking_floor[0] is set to lobby by default. Added logic to allow 
										 				grp_parking_floor[0] to be any floor if parking width is nonzero and parking type is zero

*/
