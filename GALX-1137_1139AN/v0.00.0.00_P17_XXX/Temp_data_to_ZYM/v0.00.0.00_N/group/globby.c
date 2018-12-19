#define d_GLOBBY 1
#include "global.h"

uint8 alt_lobby_inp = 0;
uint8 next_up_inp = 0;

// Lobby Variables
int16 lobby_fl = 1;					// lobby floor
uint8 alt_lobby = 0;					// set to 1 for alternate lobby operation

// lobby request and next up variables
int16 next_up = 0;		// master next up service flag, 1 = system using next up
int16 ncu_car = 0;
int16 ncu_door[car_size];
int16 ncu_que[car_size];

int16 nmb_ncu_que=0;
int16 ncu_door_seq = 0;
int16 ncu_seq = 0;

int16 lobby_request=0;
int16 lobby_req_car=0;
int16 nmb_cars_at_lobby=0;
int16 nmb_lby_recall_req=0;
int16 lby_req[car_size];


void group_lobby_request(void);
void group_nextup(void);
int16 best_lobby_car(void);


//****************************************
//  Find the best lobby recall car
//****************************************
int16 best_lobby_car(void)
{
	int16 i,lobby_req_car;

	lobby_req_car = 0;
	for(i=cons[nmbcars]; i>=1; i--)
	{       // find the first elegible car to answer the call
	   if ( (Car_ots[i] == 0) 
			&& (i != ir_car)  && (Car_on_SR[i] == 0)
			&& ( (grp_Car_Stat[i].g_servf == s_AUTO) || (grp_Car_Stat[i].g_servf == s_HOMING) )     // inservice
	   	 	&& (ucmsk[i][lobby_fl] == 1) 
			&& ((lby_req[i] == 0) || ((fvars[fvuppkctl] & 0x01) == 0))		// not already a lobby request car
	   	 	&& ( (grp_Car_Stat[i].g_pos != lobby_fl) 				// Not at the lobby  
				  || ((grp_Car_Stat[i].g_pos == lobby_fl)
						&& ((grp_Car_Stat[i].g_dpref == 0) || (grp_Car_Stat[i].g_dpref == 1))
 			 			&& ((grp_Car_Stat[i].g_procf <= 14) || (grp_Car_Stat[i].g_procf >= 16)))	// not moving
				)
	       )
		{
	   		lobby_req_car = i;
		}
	}
	if (lobby_req_car != 0)
	{			// A car can be recalled
		for(i=lobby_req_car+1; i<=cons[nmbcars]; i++)
		{       // find the car that can answer the call the fastest
		  	if ( (Car_ots[i] == 0) 
				 && (i != ir_car)  && (Car_on_SR[i] == 0)
 				 && ( (grp_Car_Stat[i].g_servf == s_AUTO) || (grp_Car_Stat[i].g_servf == s_HOMING) )     // inservice
		  		 && (ucmsk[i][lobby_fl] == 1) 
	  			 && ((lby_req[i] == 0) || ((fvars[fvuppkctl] & 0x01) == 0))		// not already a lobby request car
	   	 		 && ( (grp_Car_Stat[i].g_pos != lobby_fl) 				// Not at the lobby  
				       || ((grp_Car_Stat[i].g_pos == lobby_fl)
						    && ((grp_Car_Stat[i].g_dpref == 0) || (grp_Car_Stat[i].g_dpref == 1))
 			 		        && ((grp_Car_Stat[i].g_procf <= 14) || (grp_Car_Stat[i].g_procf >= 16)))	// not moving
				)
	       )
		  	{     // compare eta
			  	if (car_up_eta[lobby_req_car][lobby_fl] > car_up_eta[i][lobby_fl])
				{  	
					lobby_req_car = i;
				}
		  	}
		}

	}
	return(lobby_req_car);
}
//*****************************************
//  This routine recalls one or more cars
// to the lobby for next up or uppeak
// operation.
//*******************************************
void group_lobby_request(void)
{

	int16 i;
	int16 okay_to_assign;

	if (cons[Park_NCU_ALB] == 1) 
	{
		if ((cons[sercom] & 0x20) == 0)
		{		   // no serial hall call boards
			if ((cons[epgrp] != 0) && (cons[Car_Asgn_Fl] != 0))
				alt_lobby_inp = ((gc_dc[cons[grtopf]/32] & fl_mask[cons[grtopf]+4]) != 0);
			else if ((cons[epgrp] != 0) && (cons[Car_Asgn_Fl] == 0))
				alt_lobby_inp = ((gc_dc[cons[grtopf]/32] & fl_mask[cons[grtopf]+3]) != 0);
			else if ((cons[epgrp] == 0) && (cons[Car_Asgn_Fl] != 0))
				alt_lobby_inp = ((gc_dc[cons[grtopf]/32] & fl_mask[cons[grtopf]+2]) != 0);
			else
				alt_lobby_inp = ((gc_dc[cons[grtopf]/32] & fl_mask[cons[grtopf]+1]) != 0);
		}
		else
			alt_lobby_inp = (gpi[g_invalid_io_loc+1] & 0x01) != 0;
	}
	else
		alt_lobby_inp = 0;

	if ((ids_alt_lby == 1) || (ui_alt_lby == 1) || (alt_lobby_inp == 1) || (tmr_svc[s_alt_lby] == 1))
	{
		alt_lobby = 1;
		lobby_fl = fvars[fvaltlob];
	}
	else
	{
		alt_lobby = 0;
		lobby_fl = fvars[fvlob];
	}

	if ((dn_peak == 1) && (up_peak == 0))				// cancel lobby request if on down peak
		lobby_request = 0;
	else if ((up_peak == 1)	&& (dn_peak == 0))			// request all the cars to the lobby
	{
		if (fvars[fvuppkpool] < cons[nmbcars])
			lobby_request = cons[nmbcars] - fvars[fvuppkpool];
		else
			lobby_request = 1;
	}
	else if ((up_peak == 1) && (dn_peak == 1))			// peak only on groups greater than 1 car
		lobby_request = 1;
	else if (fvars[fvlbyreq] == 0)
		lobby_request = 0;
	else if ((cons[Park_NCU_ALB] != 0) || (fvars[fvlrqctl] == 1))
	{
		if ((next_up == 1) && (fvars[fvlbyreq] <= cons[nmbcars]))
			lobby_request = fvars[fvlbyreq];
		else
			lobby_request = 0;
	} 
	else if (fvars[fvlbyreq] <= cons[nmbcars])
		lobby_request = fvars[fvlbyreq];
	else 
		lobby_request = 0;

	if (lobby_request != 0) 
	{
		if ((nmb_cars_at_lobby + nmb_lby_recall_req) < lobby_request)
		{			  // We need more cars
			// Find the best car to recall to the lobby
			lobby_req_car = best_lobby_car();
			if (lobby_req_car != 0)
			{
				okay_to_assign = 1;
				if ((fvars[fvuppkctl] & 0x01) == 0)		// Not a heavy Up peak control (do one car at a time)
				{
					for (i=1;i<=cons[nmbcars];i++)
					{		   
						if ((lby_req[i] != 0) && (i != lobby_req_car))
						{		// new lobby request assitnment
							okay_to_assign = 0;
							if ((g_asign_up[lobby_fl] != i) && (grp_Car_Stat[i].g_nxt_stp != lobby_fl))
							{
								okay_to_assign = 1;
								lby_req[i] = 0;		// remove lobby request
							}
						}  	
					}
				}
				if (okay_to_assign == 1)
					lby_req[lobby_req_car] = lobby_fl;
			}
		}
		else if ((nmb_cars_at_lobby + nmb_lby_recall_req) > lobby_request)
		{
			for (i=1;i<=cons[nmbcars];i++)
				lby_req[i] = 0;
		}
	}
	else 
	{
		for (i=1;i<=cons[nmbcars];i++)
			lby_req[i] = 0;
	}


	
	nmb_lby_recall_req = 0;
	nmb_cars_at_lobby = 0;
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((grp_Car_Stat[i].g_pos == lobby_fl) 
			 && (Car_ots[i] == 0) 
			 && (i != ir_car)  && (Car_on_SR[i] == 0)
			 && ( (grp_Car_Stat[i].g_servf == s_AUTO) || (grp_Car_Stat[i].g_servf == s_HOMING) )     // inservice
			 && (ucmsk[i][lobby_fl] == 1) 
		     && ((grp_Car_Stat[i].g_dpref == 0) || (grp_Car_Stat[i].g_dpref == 1))
 			 && ((grp_Car_Stat[i].g_procf >= 14) && (grp_Car_Stat[i].g_procf <= 16)))	// not moving
		{			  // Car is at the lobby, has been recalled 
			if (lby_req[i] != 0)
				lby_req[i] = 0;			// Request has been fulfilled
			nmb_cars_at_lobby++;
		}

		if (lby_req[i] != 0)
		{		// Car is being recalled
			if ((fvars[fvuppkctl] & 0x01) == 0)		// Not a heavy Up peak control (do one car at a time)
			{
				if ( (grp_Car_Stat[i].g_pos != lobby_fl)
					   && (best_lobby_car() != i))								// This is no longer the best lobby car
				{		// release the car
					lby_req[i] = 0;
				}
				else
					nmb_lby_recall_req++;
			}
			else																			
			{
				
			   	if ( (Car_ots[i] == 1) 
					|| (i == ir_car)  || (Car_on_SR[i] == 1)
					|| ( (grp_Car_Stat[i].g_servf != s_AUTO) && (grp_Car_Stat[i].g_servf != s_HOMING) )     // inservice
			   	 	|| (ucmsk[i][lobby_fl] != 1) 
			       )
				{
					lby_req[i] = 0;
				}
				else
					nmb_lby_recall_req++;
			}

			if (lby_req[i] != lobby_fl)		// lobby floor has been changed
				lby_req[i] = 0;

		}
	}
}

//************************************************
// This routine select the car at the lobby
// to be next up
//************************************************
void group_nextup(void)
{
	int16 i;
	int16 nmb_ncu = 0;

	// Next car up routine

	// Next up enable input
	if (cons[Park_NCU_ALB] == 1) 
	{
		if ((cons[sercom] & 0x20) == 0)
		{		   // no serial hall call boards
			if ((cons[epgrp] != 0) && (cons[LbyUpReq] != 0))
				next_up_inp = ((gc_uc[cons[grtopf]/32] & fl_mask[cons[grtopf]+3]) != 0);
			else if ((cons[epgrp] != 0) || (cons[LbyUpReq] != 0))
				next_up_inp = ((gc_uc[cons[grtopf]/32] & fl_mask[cons[grtopf]+2]) != 0);
			else
				next_up_inp = ((gc_uc[cons[grtopf]/32] & fl_mask[cons[grtopf]+1]) != 0);
		}
		else
			next_up_inp = (gpi[g_invalid_io_loc] & 0x80) != 0;

	}
	else
		next_up_inp = 0;

	if ((dn_peak == 1) && (up_peak == 0))
		next_up = 0;	 // no next up while on down peak
	else if (((fvars[fvncu] & 4) != 0) || (cons[Park_NCU_ALB] != 0)) 
	{		// Next up on up peak only or select
		if ((up_peak == 1) || (next_up_inp != 0) || (ids_ncu != 0) || (ui_ncu != 0) || (tmr_svc[s_ncu] == 1))
			next_up = 1;		
		else
			next_up = 0;
	}
	else if ((fvars[fvncu] & 3) != 0)
		next_up = 1;	// Always next up operation
	else if ((ids_ncu != 0) || (ui_ncu != 0) || (tmr_svc[s_ncu] == 1))
		next_up = 1;
	else
		next_up = 0;


	if (next_up == 1)		
	{		// Next car up is enabled
		nmb_ncu_que = 0;
		for (i=1;i<=cons[nmbcars];i++)
		{
			if (ncu_que[i] == 1)
				nmb_ncu_que++;
		}

		for (i=1;i<=cons[nmbcars];i++)
		{
			if ((grp_Car_Stat[i].g_pos == lobby_fl) 
				 && (Car_ots[i] == 0) 
 				 && ( (grp_Car_Stat[i].g_servf == s_AUTO) || (grp_Car_Stat[i].g_servf == s_HOMING) )     // inservice
				 && (i != ir_car) && (Car_on_SR[i] == 0)
				 && (ucmsk[i][lobby_fl] == 1) 
				 && (grp_Car_Stat[i].g_dirf == 0)			 // car is not moving
				 && (grp_Car_Stat[i].g_doorf != 3)			 // doors are not closiing
				 && (grp_Car_Stat[i].g_doorf != 4)
			     && ((grp_Car_Stat[i].g_dpref == 0) || (grp_Car_Stat[i].g_dpref == 1))
	 			 && ((grp_Car_Stat[i].g_procf >= 14) && (grp_Car_Stat[i].g_procf <= 16)))	// car is at floor and not moving
			{			  // Car is at the lobby 
				if ((ncu_car == 0) || (ncu_car == i))
				{			 // Select next up car
					if (ncu_que[i] == 1)
					{
						if (nmb_ncu_que > 0)
							nmb_ncu_que--;
					}
					ncu_que[i] = 0;
					if (ncu_seq == 0)
					{
						if ((grp_Car_Stat[i].g_dcalls == 0) || ((grp_Car_Stat[i].g_dcalls & 2) == 0)
							|| (grp_Car_Stat[i].g_doorf == 1))
						{			 // no calls or no calls below the floor and not moving
							ncu_car = i;		
							ncu_door[i] = 1;	 // open door
							ncu_seq = 1;
						}
					}
					else if (ncu_seq == 1)
					{	
						if ((grp_Car_Stat[i].g_doorf == 3) && (grp_Car_Stat[i].g_dcalls != 0))
						{		   // door is closing for an onward call
							ncu_door[i] = 0;
							ncu_seq = 2;
						}
						else if (grp_Car_Stat[i].g_dpref == 0)
							ncu_seq = 0;
					}
					else if (ncu_seq == 2)
					{
						if (grp_Car_Stat[i].g_doorf == 1)
						{
							ncu_car = i;
							ncu_door[i] = 1;	 // open door
							ncu_seq = 1;
						}
						else if (grp_Car_Stat[i].g_dpref == 0)
							ncu_seq = 0;
					} 
				}
				else 
				{	 
				
					if ((grp_Car_Stat[ncu_car].g_control & 0x2) != 0)		// onward call
						nmb_ncu = 0;			// Next up car is getting ready to leave the floor
					else
						nmb_ncu = 1;

					if ((nmb_ncu_que + nmb_ncu) < lobby_request)
					{		// Car is at the lobby so put it in the next up que
						ncu_que[i] = 1;
						nmb_ncu_que++;
					}
					else if (((nmb_ncu_que + nmb_ncu) > lobby_request) && (ncu_que[i] == 1))
					{		 // Too many in the que so delete one
						ncu_que[i] = 0;
						if (nmb_ncu_que > 0)
							nmb_ncu_que--;
					}
				}
			}
			else 		
			{			// next up car has left the lobby
				if (ncu_car == i)
				{	
					ncu_car = 0;
					ncu_seq = 0;
				}
				ncu_door[i] = 0;
				ncu_que[i] = 0;
			}
		}	
	}
	else
	{
		ncu_car = 0;
		ncu_seq = 0;
		for (i=0;i<=cons[nmbcars];i++)
		{
			ncu_door[i] = 0;
			ncu_que[i] = 0;
		}
	}
}


/* 
Revision History


11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.

*/
