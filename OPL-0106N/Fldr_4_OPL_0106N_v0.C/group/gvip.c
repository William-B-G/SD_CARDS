#define d_GVIP 1
#include "global.h"

bool front_vip=0; 	// Set to 1 for front vip service
bool rear_vip=0;	// Set to 1 for rear vip service
unsigned char VipCarNmb;

int16 fvip_en = 0;				// VIP front enable input
int16 rvip_en = 0;				// VIP rear enable input

int16 hp_inserv = 0; 			// in high priority service
int16 prev_hp_car = 0;			// previous high priority car (must be zero on powerup to makek sure arrays are initialized)
int16 prev_hp_fl = 0;			// previous high priority floor (must be zero on powerup to makek sure arrays are initialized)

int16 g_vip_fl[car_size];
int16 g_rvip_fl[car_size];
int16 hp_car_sel[car_size];

uint32 g_vipmsk[2]={0,0};			// Bit mask for valid vip call floors
uint32 g_rvipmsk[2]={0,0};			// Bit mask for valid rear vip call floors

void vip_recall(void);
void high_priority_fl(void);
int16 best_hp_car(int16 fl_no);
int16 vip_eta(int16 el, int16 vip_fl);
int16 best_vip_car(int16 vip_fl);
int16 any_fvip_car (int16 fl);
int16 best_rvip_car(int16 vip_fl);
int16 any_rvip_car (int16 fl);


//********************************************************************************************
//********************************* Calculate ETA for Vip Car ********************************
//********************************************************************************************  
//
//  Note that this eta is an estimate value.  Since the Eta from the car includes hall calls
//  that ETA value is not a realistic value for a car that will have all hall call assignmets
//  removed and then answer car calls.  Instead of just looking at the number of car calls, the
//  position of car calls and the current direction of travel must also be considered.
//  The best way to do this would be to have each car calculate an ETA table strictly for vip
//  cars.  When the group needs to assign a vip call, it would request the ETA for that particular
//  floor for each car and make the assignment from that information. Due to the lack of time to
//  modify the system and re-test it, this way will have to suffice.

int16 vip_eta(int16 el, int16 vip_fl)
{
	int16 car_eta=254;
	int16 nmb_fl = 0;

	if ((grp_Car_Stat[el].g_dpref == 0) || (grp_Car_Stat[el].g_nmbcc == 0) ||
		(grp_Car_Stat[el].g_topcc == 0) || (grp_Car_Stat[el].g_botcc == 0) ||
		((fvars[fvvipop] & 0x02) != 0))
	{									 // no preference: go directly to vip floor or car calls will be cancelled
		
		if (grp_Car_Stat[el].g_pos > vip_fl)
			nmb_fl = grp_Car_Stat[el].g_pos - vip_fl;
		else
			nmb_fl = vip_fl - grp_Car_Stat[el].g_pos;
	}
	else if (grp_Car_Stat[el].g_dpref == 1)
	{											// car has an up preference

		// Calculate run to top call
		if (grp_Car_Stat[el].g_topcc > grp_Car_Stat[el].g_pos)
			nmb_fl = grp_Car_Stat[el].g_topcc - grp_Car_Stat[el].g_pos;
		else if (grp_Car_Stat[el].g_topcc < grp_Car_Stat[el].g_pos)
			nmb_fl = grp_Car_Stat[el].g_pos - grp_Car_Stat[el].g_topcc;

		// calculated run to bottom call and then to vip call
		if (grp_Car_Stat[el].g_botcc < grp_Car_Stat[el].g_pos)
		{	   
			nmb_fl += (grp_Car_Stat[el].g_topcc - grp_Car_Stat[el].g_botcc);

			if (grp_Car_Stat[el].g_botcc > vip_fl)
				nmb_fl += (grp_Car_Stat[el].g_botcc - vip_fl);
			else if (grp_Car_Stat[el].g_botcc < vip_fl)
				nmb_fl += (vip_fl - grp_Car_Stat[el].g_botcc);
		}
		else
		{
			if (grp_Car_Stat[el].g_topcc > vip_fl)
				nmb_fl += (grp_Car_Stat[el].g_topcc - vip_fl);
			else if (grp_Car_Stat[el].g_topcc < vip_fl)
				nmb_fl += (vip_fl - grp_Car_Stat[el].g_topcc);
		}

		if ((grp_Car_Stat[el].g_topcc != grp_Car_Stat[el].g_botcc) && (grp_Car_Stat[el].g_nmbcc > 2))
		   nmb_fl += (grp_Car_Stat[el].g_nmbcc - 2);	
		
	}
	else if (grp_Car_Stat[el].g_dpref == 2)
	{											// car has a down preference

		// Calculate run to bottom call
		if (grp_Car_Stat[el].g_botcc > grp_Car_Stat[el].g_pos)
			nmb_fl = grp_Car_Stat[el].g_botcc - grp_Car_Stat[el].g_pos;
		else if (grp_Car_Stat[el].g_botcc < grp_Car_Stat[el].g_pos)
			nmb_fl = grp_Car_Stat[el].g_pos - grp_Car_Stat[el].g_botcc;

		// calculated run to top call and then to vip call
		if (grp_Car_Stat[el].g_topcc > grp_Car_Stat[el].g_pos)
		{	   
			nmb_fl += (grp_Car_Stat[el].g_topcc - grp_Car_Stat[el].g_botcc);

			if (grp_Car_Stat[el].g_topcc > vip_fl)
				nmb_fl += (grp_Car_Stat[el].g_topcc - vip_fl);
			else if (grp_Car_Stat[el].g_topcc < vip_fl)
				nmb_fl += (vip_fl - grp_Car_Stat[el].g_topcc);
		}
		else
		{
			if (grp_Car_Stat[el].g_botcc > vip_fl)
				nmb_fl += (grp_Car_Stat[el].g_botcc - vip_fl);
			else if (grp_Car_Stat[el].g_botcc < vip_fl)
				nmb_fl += (vip_fl - grp_Car_Stat[el].g_botcc);
		}

		if ((grp_Car_Stat[el].g_topcc != grp_Car_Stat[el].g_botcc) && (grp_Car_Stat[el].g_nmbcc > 2))
		   nmb_fl += (grp_Car_Stat[el].g_nmbcc - 2);
	}
	// Estimate time to run total number of floors
	car_eta = (720 * nmb_fl)/cons[speed];	 // 12 ft floors * 60 seconds
	return car_eta;
}


//******************************************************
//***  Best front Priority Svc (vip) car to recall *****
//******************************************************

int16 best_vip_car(int16 vip_fl)
{
	int16 i;
	int16 vip_car;
	int16 vip_car_eta;
	int16 car_eta;
	int16 accel_time;
	int16 decel_time;
	int16 door_time;

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_vip_fl[i] == vip_fl) && (i != ir_car) && 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_VIP) || 
				((grp_Car_Stat[i].g_vipf == 4) && (grp_Car_Stat[i].g_pos == vip_fl))))
			return i;		// already assigned this floor
	}

	vip_car_eta = 255;
	car_eta = 254;
	vip_car = 0;

	// Use this car's flight times since the ETA from other cars is misrepresented by hall call stops

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((Car_ots_nbp[i] == 0) && (ccmsk[i][vip_fl] == 1) && (valid_vip_car[i] == 1))
		{
			car_eta = vip_eta(i,vip_fl);
			if ((grp_Car_Stat[i].g_nmbcc != 0) && ((fvars[fvvipop] & 0x02) == 0))
			{		  // Estimate time to stop at each floor
				door_time =  6 + fvars[fvccdt]/10;
				accel_time = cons[speed]/fvars[fvacc];
				decel_time = cons[speed]/fvars[fvdec];
				car_eta += (grp_Car_Stat[i].g_nmbcc * (door_time + accel_time + decel_time));		// 6 seconds + dwell time for each stop
			}
		}
		else
			car_eta = 255;

		if ((g_vip_fl[i] == 0) && (g_rvip_fl[i] != vip_fl) && (car_eta < vip_car_eta) && (Car_ots_nbp[i] == 0) && (ccmsk[i][vip_fl] == 1))
		{
			vip_car = i;
			vip_car_eta = car_eta;
		}
	}
	return vip_car;
}

int16 any_fvip_car (int16 fl)
{
	int16 i;

	if (fvars[fvnmbvipcars] == 0)
		return 0;

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((ccmsk[i][fl] == 1) && (valid_vip_car[i] == 1))
			return 1;
	}
	return 0;
}

//*********************************************
//***  Best rear code blue car to recall *****
//*********************************************

int16 best_rvip_car(int16 vip_fl)
{
	int16 i;
	int16 vip_car;
	int16 vip_car_eta;
	int16 car_eta;
	int16 accel_time;
	int16 decel_time;
	int16 door_time;

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_rvip_fl[i] == vip_fl) && (i != ir_car) && 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_VIP) || 
				((grp_Car_Stat[i].g_vipf == 4) && (grp_Car_Stat[i].g_pos == vip_fl))))
			return i;		// already assigned this floor
	}

	vip_car_eta = 255;
	car_eta = 254;
	vip_car = 0;
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((Car_ots_nbp[i] == 0) && (rccmsk[i][vip_fl] == 1) && (valid_vip_car[i] == 1))
		{
			car_eta = vip_eta(i,vip_fl);
			if ((grp_Car_Stat[i].g_nmbcc != 0) && ((fvars[fvvipop] & 0x02) == 0))
			{
				door_time =  6 + fvars[fvccdt]/10;
				accel_time = cons[speed]/fvars[fvacc];
				decel_time = cons[speed]/fvars[fvdec];
				car_eta += grp_Car_Stat[i].g_nmbcc * (door_time + accel_time + decel_time);		// 6 seconds + dwell time for each stop
			}
		}
		else
			car_eta = 255;

		if ((g_rvip_fl[i] == 0) && (g_vip_fl[i] != vip_fl) && (car_eta < vip_car_eta) && (Car_ots_nbp[i] == 0) && (rccmsk[i][vip_fl] == 1))
		{
			vip_car = i;
			vip_car_eta = car_eta;
		}
	}
	return vip_car;
}

int16 any_rvip_car (int16 fl)
{
	int16 i;

	if (fvars[fvnmbvipcars] == 0)
		return 0;

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((rccmsk[i][fl] == 1) && (valid_vip_car[i] == 1))
			return 1;
	}
	return 0;
}

//*********************************************
//** Vip Recall Selection *********************
//*********************************************

void vip_recall(void)
{
	int16 i;
	int16 vip_car;
	int16 vip_call;
	int16 vip_demand;
	int16 nmb_vip_cars;
	int16 fl_ix;
		
	// Set the number of cars already on vip
	nmb_vip_cars = 0;	 
	for (i=1;i<=cons[nmbcars];i++)
	{
		if (grp_Car_Stat[i].g_servf == s_VIP)
			nmb_vip_cars++;
	}

	if ((cons[vip_en] & 0x01) != 0)
	{	// VIPEN input is enabled
		if ((cons[sercom] & 0x20) == 0)
		{		// no serial hall call boards
			fvip_en = ((gc_vip[cons[grtopf]/32] & fl_mask[cons[grtopf]+1]) != 0);
			rvip_en = ((gc_rvip[cons[grtopf]/32] & fl_mask[cons[grtopf]+1]) != 0);
		}
		else
		{
			fvip_en = (gpi[g_invalid_io_loc+2] & 0x08) != 0;
			rvip_en = (gpi[g_invalid_io_loc+2] & 0x16) != 0;
		}
	}
	else
	{
		fvip_en = 1;
		rvip_en = 1;
	}

	vip_demand = 0;
	vip_car = 0;

  	for(i=1; i <= cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
		if (front_vip != 0)
		{
		    if((gc_vip[fl_ix] & fl_mask[i]) != 0)// check to see if vip call pushed
			{
				if(fvip_en == 1)
				{
					vip_demand = 1;
					gc_vip_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
					gc_vip_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
					if(	VipCarNmb == 0)
						vip_car = best_vip_car(i); 
					else
					{
						vip_car = VipCarNmb;
						VipCarNmb = 0;
				   	}
				}
				
				if ((vip_car != 0) && (fvars[fvnmbvipcars] != 0))
				{		
					if ((g_vip_fl[vip_car] == i) && (grp_Car_Stat[vip_car].g_pos == i) 
							&& ((grp_Car_Stat[vip_car].g_vipf == 3) || (grp_Car_Stat[vip_car].g_vipf == 4)))
					{	//  vip car is at the floor
						gc_vip_canc[fl_ix] |= fl_mask[i];
					}
					else if ((nmb_vip_cars < fvars[fvnmbvipcars]) && (grp_Car_Stat[vip_car].g_servf != s_VIP)) 
					{															 // ^^ already counted above
						g_vip_fl[vip_car] = i;		   // assign the call
						if (nmb_vip_cars < cons[nmbcars])
							nmb_vip_cars++;
					}
					else if ((g_vip_fl[vip_car] != i) && (((fvars[fvvipop] & 0x01) != 0) || (any_fvip_car(i) == 0)))   // car not assigned to this call and there is not car available
					{
						gc_vip_canc[fl_ix] |= fl_mask[i];	   // cannot assign the call so cancel
					}
				}
				else if (((fvars[fvvipop] & 0x01) != 0) || (any_fvip_car(i) == 0))	  // Cancel call if vip car busy or no car available
				{
					gc_vip_canc[fl_ix] |= fl_mask[i];	   // cannot assign the call so cancel
				}
		  	}
		    else
		    {
				gc_vip_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			}
		}

		if ((grtop_rfl != 0) && (rear_vip != 0))
		{
		    if((gc_rvip[fl_ix] & fl_mask[i]) != 0)// check to see if up hall call pushed
			{
				if(rvip_en == 1)
				{
					vip_demand = 1;
					gc_rvip_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
					
					if(	VipCarNmb == 0)
						vip_car = best_rvip_car(i);
					else
					{
						vip_car = VipCarNmb;
						VipCarNmb = 0;
					}
				}
				
				if ((vip_car != 0) && (fvars[fvnmbvipcars] != 0))
				{			
					if ((g_rvip_fl[vip_car] == i) && (grp_Car_Stat[vip_car].g_pos == i) 
						&& ((grp_Car_Stat[vip_car].g_vipf == 3) || (grp_Car_Stat[vip_car].g_vipf == 4)))
					{	// vip car is at the floor
						gc_rvip_canc[fl_ix] |= fl_mask[i];
					}
					else if ((nmb_vip_cars < fvars[fvnmbvipcars]) && (grp_Car_Stat[vip_car].g_servf != s_VIP)) 
					{
						g_rvip_fl[vip_car] = i;		// Assign the call
						if (nmb_vip_cars < cons[nmbcars])
							nmb_vip_cars++;
					}
					else if ((g_rvip_fl[vip_car] != i) && (any_rvip_car(i) == 0))	 // car not assigned to this call and there is not car available
						gc_rvip_canc[fl_ix] |= fl_mask[i];	   // cannot assign the call so cancel
				}
				else if (((fvars[fvvipop] & 0x01) != 0) || (any_rvip_car(i) == 0))	  // Cancel call if vip car busy or no car available
					gc_rvip_canc[fl_ix] |= fl_mask[i];	// cannot assign the call
		    }
			else
			{
				gc_rvip_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			}
	    }
  	}
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_vip_fl[i] != 0) && (front_vip != 0))
		{
			fl_ix = (g_vip_fl[i]-1)/32;

			vip_call = ((gc_vip[fl_ix] & fl_mask[g_vip_fl[i]]) != 0);

			if (((grp_Car_Stat[i].g_vipf == 4) || (grp_Car_Stat[i].g_servf != s_VIP)) && (vip_call == 0) )
				g_vip_fl[i] = 0;		 // call answered so cancel vip call at the floor
			if ((Car_ots_nbp[i] == 1) && (grp_Car_Stat[i].g_servf != s_VIP))	// car went out of service
				g_vip_fl[i] = 0;
		}
		if ((g_rvip_fl[i] != 0) && (rear_vip != 0))
		{
			fl_ix = (g_rvip_fl[i]-1)/32;

			vip_call = ((gc_rvip[fl_ix] & fl_mask[g_rvip_fl[i]]) != 0);

			if (((grp_Car_Stat[i].g_vipf == 4) || (grp_Car_Stat[i].g_servf != s_VIP)) && (vip_call == 0) )
				g_rvip_fl[i] = 0;		// call answered so cancel vip call at the floor
			if ((Car_ots_nbp[i] == 1) && (grp_Car_Stat[i].g_servf != s_VIP))	// car went out of service
				g_rvip_fl[i] = 0;
		}
	}
}

//*********************************************
//****  High Priority Floor 
//*********************************************

void high_priority_fl(void)
{
	int16 fl_ix;
	int16 hp_fl=0;
	int16 hp_car;
	int16 i = 0, j = 0;

	hp_fl = fvars[fvhighpriorityfl];

	if ((hp_fl != prev_hp_fl) && (hp_fl != 0))
	{			   // do once on power up or if the parameter is changed
		for (i=1; i<=cons[nmbcars]; i++)
		{
			for(j=1; j <= cons[grtopf]; j++)
			{
				hpucmsk[i][j] = ucnmsk[i][j];
				hpdcmsk[i][j] = dcnmsk[i][j];
			}
			hp_car_sel[i] = 0;
		}
		timers[thighprioritytm] = 0;	// also reset timer if fl changed
		prev_hp_car = 0;
	}
	prev_hp_fl = hp_fl;

	if ((hp_fl != 0) && (gservf == 0) && (nmb_cars_in_svc > 1))		// cars not on fire service or emergency power1
	{	// High Priority Floor is Set
		fl_ix = (hp_fl-1)/32;
		if (((g_up_hall_call[fl_ix][0] & fl_mask[hp_fl]) != 0) || ((g_dn_hall_call[fl_ix][0] & fl_mask[hp_fl]) != 0))
		{	// check to see if high priority up or down hall call pushed
			if (timers[thighprioritytm] > fvars[fvhighpriorityfltim])
			{	// Timer exceeds the time set by fvar now choose best car
				// and remove hall calls and consider only car calls.

				hp_inserv = 1;
				
				hp_car = best_hp_car(hp_fl);

				if (hp_car != prev_hp_car)
				{

					// set selection to select best_hp_car() only once
					hp_car_sel[hp_car] = hp_fl;

					if (prev_hp_car != 0)
					{
						// clear previous car selection
						hp_car_sel[prev_hp_car] = 0;
					
						// reset previous selected car's up/dn mask to be same as normal mask
						for (i=1; i<=cons[grtopf]; i++)
						{
							hpucmsk[prev_hp_car][i] = ucnmsk[prev_hp_car][i];
							hpdcmsk[prev_hp_car][i] = dcnmsk[prev_hp_car][i];
						}
						
						// on old car do not allow up/dn call on the hp_fl
						hpucmsk[prev_hp_car][hp_fl] = 0;
						hpdcmsk[prev_hp_car][hp_fl] = 0;
						
					}


					if (hp_car != 0)
					{
						hp_car_sel[hp_car] = 1;

						// on sel car clr all up/dn calls
						for (i=1; i<=cons[grtopf]; i++)
						{
							hpucmsk[hp_car][i] = 0;
							hpdcmsk[hp_car][i] = 0;
						}
						
						// on all other cars disable up/dn
						for (i=1; i<=cons[nmbcars]; i++)
						{
							hpucmsk[i][hp_fl] = 0;
							hpdcmsk[i][hp_fl] = 0;
						}

						hpucmsk[hp_car][hp_fl] = ucnmsk[hp_car][hp_fl];
						hpdcmsk[hp_car][hp_fl] = dcnmsk[hp_car][hp_fl];

						// point uc/dc msk to hpuc/dc array
						for (i=1; i<car_size; i++)
						{
							ucmsk[i] = &hpucmsk[i][0];
							dcmsk[i] = &hpdcmsk[i][0];
						}

						prev_hp_car = hp_car;

					}
					
				}
			}
		}
		else
		{	// if buttons not pressed
			if (hp_inserv != 0)
			{
				for (i=1; i<car_size; i++)
				{
					ucmsk[i] = &ucnmsk[i][0];
					dcmsk[i] = &dcnmsk[i][0];
					hp_car_sel[i] = 0;
					if (i <= cons[nmbcars])
					{
						for(j=1; j <= cons[grtopf]; j++)
						{
							hpucmsk[i][j] = ucnmsk[i][j];
							hpdcmsk[i][j] = dcnmsk[i][j];
						}
					}
				}
				prev_hp_car = 0;
				hp_inserv = 0;
			}
			timers[thighprioritytm] = 0;
		}
	}
	else
	{	// HP Fl fvar not set
		if (hp_inserv != 0)
		{
			for (i=1; i<car_size; i++)
			{
				ucmsk[i] = &ucnmsk[i][0];
				dcmsk[i] = &dcnmsk[i][0];
				hp_car_sel[i] = 0;
				if (i <= cons[nmbcars])
				{
					for(j=1; j <= cons[grtopf]; j++)
					{
						hpucmsk[i][j] = ucnmsk[i][j];
						hpdcmsk[i][j] = dcnmsk[i][j];
					}
				}
			}
			prev_hp_car = 0;
			hp_inserv = 0;
		}
		timers[thighprioritytm] = 0;
	}
}

//*********************************************
//****  High Priority Floor Best Car Selection  
//*********************************************
int16 best_hp_car(int16 fl_no)
{
	int16 i;
	int16 hp_car;
	int16 hp_car_eta;
	int16 car_eta;
	int16 accel_time;
	int16 decel_time;
	int16 door_time;

	hp_car_eta = 255;
	car_eta = 254;
	hp_car = 0;
	
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((hp_car_sel[i] == fl_no) && (i != ir_car) && (Car_ots_nbp[i] == 0)
			&& (grp_Car_Stat[i].g_servf == s_AUTO))	// car is in automatic service
			return i;		// already assigned this floor
	}

	// Use this car's flight times since the ETA from other cars is misrepresented by hall call stops

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((Car_ots_nbp[i] == 0) && (ccmsk[i][fl_no] == 1) && (grp_Car_Stat[i].g_servf == s_AUTO))
		{
			car_eta = vip_eta(i,fl_no);	// we could use vip_eta() routine because it provides eta by carcalls only
			if ((grp_Car_Stat[i].g_nmbcc != 0))
			{		  // Estimate time to stop at each floor
				door_time =  6 + fvars[fvccdt]/10;
				accel_time = cons[speed]/fvars[fvacc];
				decel_time = cons[speed]/fvars[fvdec];
				car_eta += (grp_Car_Stat[i].g_nmbcc * (door_time + accel_time + decel_time));		// 6 seconds + dwell time for each stop
			}
		}
		else
			car_eta = 255;

		if ((car_eta < hp_car_eta) && (Car_ots_nbp[i] == 0) && (ccmsk[i][fl_no] == 1))
		{
			hp_car = i;
			hp_car_eta = car_eta;
		}
	}
	return hp_car;
}

/* 	Revision History

1/11/12	 v5.54.49 WC    1.  Modify vip_recall(), to enable Galileo to choose which car takes  the VIP call.
10/24/12 v5.55.20 as/hdl	1. Modified vip_recall() for allowing the car to accept vip calls only when vip enable input is on
						   if cons[vip_en] is enabled
12/18/12 v6.00.26 mhd	1. Added changes from galaxy3 from 5.55.06 to 5.55.20. (Left Vip only with my changes).

2/12/13 v5.55.29 as 	1. Added high priority floor routine high_priority_fl() and best high priority car selection routine best_hp_car()
8/23/13 v7.0.16 mhd		1. Added changes from GALaxy 3 below:
						   {
								 v5.55.32 as		1. Modified high_priority_fl() to fix a scenario when the best high priority car goes out of service and a different car gets selected then hpucmsk/hpdcmsk mask needed to be reset for all cars,
						   								also don't allow high priority fl service if only one car is in service.
													2. Modified best_hp_car() to fix group automatic service flag, (Group_IO_Com[i][g_servf] == 0) to (Group_IO_Com[i][g_servf] == 1)
							}
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.
12/18/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
								3/12/14 v6.1.37 as		1. Fixed bug, to allow VIP on more than 32 floors when clearing vip call in vip_recall() routine, changed the fl_ix from ((i-1)/32;) to (g_vip_fl[i]-1)/32;
								11/13/14 v6.1.98 as		1. Added below change made in GALaxy III old CPU.
								11/4/14 v5.55.91 as     1. Re-did high_priority_fl() to fix an issue with down hall calls dropping.

*/
