#define d_CETA 1
#include "global.h"

// Average ETA time variables
int16 nmb_asg_uc = 0;		// Number of assigned up calls
int32 avg_uc_eta = 0;		// Average Up call ETA
int32 sum_uc_eta = 0;		// Sum of uc eta
int16 nmb_asg_dc = 0;		// Number of assigned down calls
int32 avg_dc_eta = 0;		// Average Down call ETA
int32 sum_dc_eta = 0;		// Sum of dc eta				
int16 xassign = 0;			// Enable cross assignment operation			
int16 xasgn_toggle = 0;		// When ratio of calls is the same dispatch one to each system
int16 nmb_asg_iruc = 0;		// Number of assigned ir up calls
int16 nmb_asg_irdc = 0;		// Number of assigned ir down calls


int16 door_cycle = 0;							

int16 car_up_eta[car_size][fl_size];
int16 car_dn_eta[car_size][fl_size];




void Car_ETA(void );

//*****************************************************
// group estimated time of arrivial to answer a call
//*****************************************************
void Car_ETA()
{
  	int16 i,time_to_dc=0,hdoor_time,cdoor_time,total_time=0,motiont,time_to_rdc=0,ncu_que_time=0;
  	motiont = (timers[tmotion]/10);

  	if( (((servf != s_AUTO)||((cons[Prison] != 0) && (remote_cc == 1) && (servf == s_AUTO))) && (servf != s_HOMING) && (servf != s_ATT)) ||	// out of service
  	    (closetry != 0) || (rclosetry != 0))    // door close failure
  	{
		if ((servf == s_AUTO) || (servf == s_IND) || (servf == s_FIRE2) || (servf == s_CBLUE) || (servf == s_ATT) || (servf == s_LWBP) || 
			(servf == s_LBY_RCL) || (servf == s_MED_EMS) || (servf == s_ELEV_OFF_CS) || (servf == s_ELEV_OFF_HS) ||
		 	(servf == s_HSV) || (servf == s_VIP) || (servf == s_SEC_RCL) || (servf == s_SABBATH) || ((cons[Prison] != 0) && (remote_cc == 1)) ||
		 	(servf == s_TUGS) || (servf == s_RIOT))							// calculating next stop when out of service
		{
			stops[0] = 0;
			if(dpref == 1)	// going up
			{
			  	for(i=position; i<=cons[topf]; i++)
			  	{
					if( ((carcb[i] == 1)||(rcarcb[i] == 1)) && (stops[0] == 0) )
						stops[0] = i;
			  	}
			}
			if(dpref == 2)	// going down
			{
			  	for(i=position; i>=cons[bottomf]; i--)
			  	{
					if( ((carcb[i] == 1)||(rcarcb[i] == 1)) && (stops[0] == 0) )
						stops[0] = i;
			  	}
			}
		}

		for(i=2; i<=cons[grtopf]; i++)
    	{   //set max down bids can't answer down hall calls
    	  	car_dn_eta[cons[carnmb]][i] = 0xff;
    	}
    	for(i=1; i<cons[grtopf]; i++)
    	{   //set max up bids can't anser up hall calls
    	  	car_up_eta[cons[carnmb]][i] = 0xff;
    	}
  	}
  	else          // inservice calculate ETA for all calls
  	{
    	cdoor_time = ((t_door_open + fvars[fvccdt] + t_door_close)/10); //total door t
    	hdoor_time = ((t_door_open + fvars[fvhcdt] + t_door_close)/10); //total door t
    	time_to_dc = 0;
		if ((procf >=3) && (procf <= 11))
			door_cycle = 0;		// clear door cycle flag while moving
		else if (doorf > 0)		// set door cycle flag after car has stopped and opened the door
			door_cycle = 1;

		if(doorf == 0)		// door is closed
		{
			if (door_cycle == 0)
			{
				if ((car_Grp_Stat.c_ncu_door == 1) && (ncu_door_seq == 0))
					time_to_dc = ((t_door_open + fvars[fvldt] + fvars[fvccdt] + t_door_close)/10);
			    else if( (upcb[position] == 1) || (rupcb[position] == 1) || (downcb[position] == 1) || (rdowncb[position] == 1) )
				    time_to_dc += hdoor_time;
			    else if( (carcb[position] == 1) || (rcarcb[position] == 1) )
				    time_to_dc += cdoor_time;
				else
					time_to_dc = 0;
			}
			else
				time_to_dc = 0;
		}			
    	if(doorf == 1)              //doors are opening
    	{
			if(t_door_open > timers[tdo])
			    time_to_dc = ((t_door_open - timers[tdo] + t_door_dwell +
					   t_door_close)/10);
			else
			    time_to_dc = ((t_door_dwell + t_door_close)/10);
    	}
    	if(doorf == 2)              //doors are dwelling
    	{
    	  	if(t_door_dwell > timers[tdwel])
				  time_to_dc = ((t_door_dwell - timers[tdwel] + t_door_close)/10);
    	  	else
				  time_to_dc = (t_door_close /10);
    	}
    	if((doorf == 3) || (doorf == 4))            //doors closing
    	{
    	  	if(t_door_close > timers[tdc])
				  time_to_dc = ((t_door_close - timers[tdc])/10);
    	  	else
				  time_to_dc = 0;
    	}
    	if(cons[rear] != 0)
    	{
		    if(rdoorf == 1)              //rear doors are opening
		    {
				if(t_rdoor_open > timers[trdo])
				    time_to_rdc = ((t_rdoor_open - timers[trdo] + t_rdoor_dwell +
						   t_rdoor_close)/10);
				else
				    time_to_rdc = ((t_rdoor_dwell + t_rdoor_close)/10);
		    }
		    if(rdoorf == 2)              //rear doors are dwelling
		    {
		      if(t_rdoor_dwell > timers[trdwel])
				  time_to_rdc = ((t_rdoor_dwell - timers[trdwel] + t_rdoor_close)/10);
		      else
				  time_to_rdc = (t_rdoor_close /10);
		    }
		    if((rdoorf == 3) || (rdoorf == 4))   //rear doors closing
		    {
		      if(t_rdoor_close > timers[trdc])
				  time_to_rdc = ((t_rdoor_close - timers[trdc])/10);
		      else
				  time_to_rdc = 0;
		    }
    	}
    	if(time_to_dc < time_to_rdc)
			time_to_dc = time_to_rdc;
    	if((dpref == 0) && (procf >=14))
// no calls just waiting assignment not in motion
    	{
			if ((car_Grp_Stat.c_ncu_que == 1) && (position == lobby_fl) && (car_Grp_Stat.c_next_up != 0))
				ncu_que_time = ((t_door_open + fvars[fvldt] + fvars[fvccdt] + t_door_close)/10);
			else
				ncu_que_time = 0;

      		for(i=cons[bottomf]; i<cons[topf]; i++)
      		{
				if(i < position)
				  car_up_eta[cons[carnmb]][i] = (flight_times[i][position] + time_to_dc);
				if(i == position)
				  car_up_eta[cons[carnmb]][i] = 0;
				if(i > position)
				  car_up_eta[cons[carnmb]][i] = (flight_times[position][i] + time_to_dc + ncu_que_time);
	  		}
	  		for(i=(cons[bottomf]+1); i<=cons[topf]; i++)
	  		{
				if(i < position)
				  car_dn_eta[cons[carnmb]][i] =	(flight_times[i][position] + time_to_dc);
				if(i == position)
				  	car_dn_eta[cons[carnmb]][i] = 0;
				if(i > position)
				  car_dn_eta[cons[carnmb]][i] = (flight_times[position][i] + time_to_dc + ncu_que_time);
      		}
    	}
    	else        // either there are calls or in motion
    	{
			//got to figure out where and what order the calls will be answered
      		nmbstops = 0;
			// got up preference
      		if(dpref == 1)    // up prefrence
      		{
				// got an up prefernce and up call at this floor
				if(procf >= 14)
					car_up_eta[cons[carnmb]][position] = 0;
				// got an up preference in motion and slowing down into the floor
				if ((hsf == 0) && (procf >= 3) && (procf <= 11))
				{               // slowing down into a call	  
		    		if(start_floor < position)
		    		{
						if(flight_times[start_floor][position] > motiont)
							car_up_eta[cons[carnmb]][position] = (flight_times[start_floor][position] - motiont);
						else
							car_up_eta[cons[carnmb]][position] = 0;
		    		}
		    		else
		    		{
						if(flight_times[position][start_floor] > motiont)
							car_up_eta[cons[carnmb]][position] = (flight_times[position][start_floor] - motiont);
						else
							car_up_eta[cons[carnmb]][position] = 0;
		    		}
		    		total_time += car_up_eta[cons[carnmb]][position];
		    		if( (carcb[position] == 1) || (rcarcb[position] == 1) )
					    total_time += cdoor_time;
		    		else
					    total_time += hdoor_time;
		    		stops[nmbstops++] = position;
				}
				//calculating calls from position to top floor - 1
				for(i=(position+1); i<cons[topf]; i++)
				{
				    if(nmbstops == 0)
				    {
						if(procf >= 14)         // at a floor
						    car_up_eta[cons[carnmb]][i] = (flight_times[position][i] + time_to_dc);
						else                    // in motion
						{
						    if(flight_times[start_floor][i] > motiont)
								car_up_eta[cons[carnmb]][i] = (flight_times[start_floor][i] - motiont);
						    else
								car_up_eta[cons[carnmb]][i] = 0;
						}
		    		}
		    		else
		    		{
						car_up_eta[cons[carnmb]][i] = (flight_times[stops[nmbstops-1]][i] + total_time);
		    		}
		    		if( (upcb[i] == 1) || (carcb[i] == 1) || (lby_req_fl == i) || (rupcb[i] == 1) || (rcarcb[i] == 1) )
		    		{
						if(nmbstops == 0)
						{
						    if(procf >= 14)             // at a floor
								total_time =(flight_times[position][i] + time_to_dc);
						    else                        // in motion
						    {
								if(flight_times[start_floor][i]>motiont)
								   total_time =(flight_times[start_floor][i]-motiont);
								else
								   total_time = 0;
						    }
						}
						else
						{
						    total_time += flight_times[stops[nmbstops-1]][i];
						}
						if( (upcb[i] == 1) || (rupcb[i] == 1) || (lby_req_fl == i) )
						    total_time += hdoor_time;
						else
						    total_time += cdoor_time;
						stops[nmbstops++] = i;
		    		}
				}
				// checked all the way to the top and no calls
				if(nmbstops == 0)
				{
				    if(procf >= 14)         // at a floor
						car_dn_eta[cons[carnmb]][cons[topf]] =
								(flight_times[position][cons[topf]] + time_to_dc);
				    else                    // in motion
				    {
						if(flight_times[start_floor][cons[topf]] > motiont)
						    car_dn_eta[cons[carnmb]][cons[topf]] = (flight_times[start_floor][cons[topf]] - motiont);
						else
						    car_dn_eta[cons[carnmb]][cons[topf]] = 0;
				    }
				}
				else
				{
		    		car_dn_eta[cons[carnmb]][cons[topf]] = (flight_times[stops[nmbstops-1]][cons[topf]] + total_time);
				}
				if( (carcb[cons[topf]] == 1) || (downcb[cons[topf]] == 1) || (rcarcb[cons[topf]] == 1) || (rdowncb[cons[topf]] == 1) )
				{
				    if(nmbstops == 0)
				    {
						if(procf >= 14)             // at a floor
						    total_time =(flight_times[position][cons[topf]] + time_to_dc);
						else                        // in motion
						{
						    if(flight_times[start_floor][cons[topf]] > motiont)
						      total_time =(flight_times[start_floor][cons[topf]]-motiont);
						    else
						      total_time = 0;
						}
				    }
				    else
				    {
						total_time += flight_times[stops[nmbstops-1]][cons[topf]];
				    }
				    if( (downcb[cons[topf]] == 1) || (rdowncb[cons[topf]] == 1) )
						total_time += hdoor_time;
				    else
						total_time += cdoor_time;
				    stops[nmbstops++] = cons[topf];
				}
				// checking from the top floor down to the position
				for(i=(cons[topf]-1); ((i>=position) && (i > cons[bottomf])); i--)
				{
				    if(nmbstops == 0)
				    {
						if(procf >= 14)         // at a floor
						    car_dn_eta[cons[carnmb]][i] =
									    (flight_times[position][i] + time_to_dc);
						else                    // in motion
						{
						    if(flight_times[start_floor][i] > motiont)
								car_dn_eta[cons[carnmb]][i] =
											(flight_times[start_floor][i] - motiont);
						    else
								car_dn_eta[cons[carnmb]][i] = 0;
						}
				    }
				    else
				    {
						if(stops[nmbstops-1] < i)
						    car_dn_eta[cons[carnmb]][i] =
							    (flight_times[stops[nmbstops-1]][i] + total_time);
						else
						    car_dn_eta[cons[carnmb]][i] =
							    (flight_times[i][stops[nmbstops-1]] + total_time);
				    }
				    if( (downcb[i] == 1) || (rdowncb[i] == 1) )
				    {
						if(nmbstops == 0)
						{
						    if(procf >= 14)             // at a floor
								total_time =(flight_times[position][i] + time_to_dc);
						    else                        // in motion
						    {
								if(flight_times[start_floor][i] > motiont)
								    total_time =(flight_times[start_floor][i]-motiont);
								else
								    total_time = 0;
						    }
						}
						else
						{
						    if(stops[nmbstops -1] < i)
								total_time += flight_times[stops[nmbstops-1]][i];
						    else
								total_time += flight_times[i][stops[nmbstops-1]];
						}
						total_time += hdoor_time;
						stops[nmbstops++] = i;
				    }
				}
				// checking from position down to the bottom floor
				for(i=(position - 1); i>cons[bottomf]; i--)
				{
				    if(nmbstops == 0)
				    {
						if(procf >= 14)         // at a floor
						    car_dn_eta[cons[carnmb]][i] = (flight_times[i][position] + time_to_dc);
						else                    // in motion
						{
						    if(flight_times[start_floor][position + 1] > motiont)
								car_dn_eta[cons[carnmb]][i] = (flight_times[start_floor][position + 1] - motiont);
						    car_dn_eta[cons[carnmb]][i] += flight_times[i][position + 1];
						}
				    }
				    else
				    {
						car_dn_eta[cons[carnmb]][i] = (flight_times[i][stops[nmbstops-1]] + total_time);
				    }
				    if( (downcb[i] == 1) || (carcb[i] == 1) || (rdowncb[i] == 1) || (rcarcb[i] == 1) )
				    {
						if(nmbstops == 0)
						{
						    if(procf >= 14)             // at a floor
								total_time =(flight_times[i][position] + time_to_dc);
						    else
						    {          // in motion
								if(flight_times[start_floor][position + 1] > motiont)
								    total_time =(flight_times[start_floor][position + 1] - motiont);
								total_time += flight_times[i][position + 1];
						    }
						}
						else
						{
						    total_time += flight_times[i][stops[nmbstops-1]];
						}
						if( (downcb[i] == 1) || (rdowncb[i] == 1) )
						    total_time += hdoor_time;
						else
						    total_time += cdoor_time;
						stops[nmbstops++] = i;
				    }
				}
				i = cons[bottomf];
				// need to block if at the bottom floor
				if(position != cons[bottomf])
				{
				    if(nmbstops == 0)
				    {
						if(procf >= 14)         // at a floor
						    car_up_eta[cons[carnmb]][i] = (flight_times[i][position] + time_to_dc);
						else                    // in motion
						{
						    if(flight_times[start_floor][position + 1] > motiont)
								car_up_eta[cons[carnmb]][i] = (flight_times[start_floor][position + 1] - motiont);
						    car_up_eta[cons[carnmb]][i] += flight_times[i][position + 1];
						}
				    }
				    else
				    {
						car_up_eta[cons[carnmb]][i] = (flight_times[i][stops[nmbstops-1]] + total_time);
				    }
				    if( (upcb[i] == 1) || (carcb[i] == 1) ||  (lby_req_fl == i) || (rupcb[i] == 1) || (rcarcb[i] == 1) )
				    {
						if(nmbstops == 0)
						{
						    if(procf >= 14)             // at a floor
								total_time =(flight_times[i][position] + time_to_dc);
						    else
						    {          // in motion
								if(flight_times[start_floor][position + 1] > motiont)
								    total_time =(flight_times[start_floor][position + 1] - motiont);
								total_time += flight_times[i][position + 1];
						    }
						}
						else
						{
						    total_time += flight_times[i][stops[nmbstops-1]];
						}
						if( (upcb[i] == 1) || (rupcb[i] == 1) || (lby_req_fl == i) )
						    total_time += hdoor_time;
						else
						    total_time += cdoor_time;
						stops[nmbstops++] = i;
				    }
				}
				// check from the bottom floor back up to position
				for(i=(cons[bottomf]+1); i<position; i++)
				{
				    if(nmbstops == 0)
				    {
						if(procf >= 14)         // at a floor
						    car_up_eta[cons[carnmb]][i] = (flight_times[i][position] + time_to_dc);
						else                    // in motion
						{
						    if(flight_times[start_floor][position + 1] > motiont)
						       car_up_eta[cons[carnmb]][i] = (flight_times[start_floor][position + 1] - motiont);
						    car_up_eta[cons[carnmb]][i] += flight_times[i][position + 1];
						}
				    }
				    else
				    {
						if(stops[(nmbstops-1)] < i)
						    car_up_eta[cons[carnmb]][i] = (flight_times[stops[nmbstops-1]][i] + total_time);
						else
						    car_up_eta[cons[carnmb]][i] =(flight_times[i][stops[nmbstops-1]] + total_time);
				    }
				    if( (upcb[i] == 1) || (rupcb[i] == 1) || (lby_req_fl == i) )
				    {
						if(nmbstops == 0)
						{
						    if(procf >= 14)             // at a floor
							total_time =(flight_times[i][position] + time_to_dc);
						    else
						    {          // in motion
								if(flight_times[start_floor][position + 1] > motiont)
								    total_time =(flight_times[start_floor][position + 1] - motiont);
								total_time += flight_times[i][position + 1];
						    }
						}
						else
						{
						    if(stops[nmbstops-1] > i)
							    total_time += flight_times[i][stops[nmbstops-1]];
						    else
							    total_time += flight_times[stops[nmbstops-1]][i];
						}
						total_time += hdoor_time;
						stops[nmbstops++] = i;
				    }
				}
				// if going up and no calls stop at the top floor  going high speed
				if ((hsf == 1) && (procf >= 3) && (procf <= 11) && (position != cons[topf]))   // running high speed and not at top floor
				{
		      		if(nmbstops == 0)
		      		{
						if(flight_times[start_floor][position + 1] > motiont)
						    car_up_eta[cons[carnmb]][position] = (flight_times[start_floor][position + 1] - motiont);
						car_up_eta[cons[carnmb]][position] += flight_times[position][position + 1];
		      		}
		      		else
		      		{
						if(stops[(nmbstops-1)] < position)
						{
						    car_up_eta[cons[carnmb]][position] = (flight_times[stops[(nmbstops-1)]][position]);
						    car_up_eta[cons[carnmb]][position] += total_time;
						}
						else
						{
						    car_up_eta[cons[carnmb]][position] =(flight_times[position][stops[(nmbstops-1)]]);
						    car_up_eta[cons[carnmb]][position] += total_time;
						}
		      		}
				}
	  		}
	  		if(dpref == 2)    // down prefrence
	  		{
	 			if(procf >= 14)
	 				car_dn_eta[cons[carnmb]][position] = 0;
				if ((hsf == 0) && (procf >= 3) && (procf <= 11))
				{	  // slowing down into a call
	 			    if(start_floor < position)
	 			    {
	 					if(flight_times[start_floor][position] > motiont)
							car_dn_eta[cons[carnmb]][position] = (flight_times[start_floor][position] - motiont);
	 					else
							car_dn_eta[cons[carnmb]][position] = 0;
	 			    }
	 			    else
	 			    {
	 					if(flight_times[position][start_floor] > motiont)
							car_dn_eta[cons[carnmb]][position] = (flight_times[position][start_floor] - motiont);
	 					else
	 						car_dn_eta[cons[carnmb]][position] = 0;
	 			    }
	 			    total_time += car_dn_eta[cons[carnmb]][position];
	 			    if( (carcb[position] == 1) || (rcarcb[position] == 1) )
						total_time += cdoor_time;
	 			    else
						total_time += hdoor_time;
					stops[nmbstops++] = position;
	 			}
	 			for(i=(position-1); i>cons[bottomf]; i--)
	 			{
	 			    if(nmbstops == 0)
	 			    {
	 					if(procf >= 14)         // at a floor
							    car_dn_eta[cons[carnmb]][i] = (flight_times[i][position] + time_to_dc);
	 					else                    // in motion
	 					{
	 					    if(flight_times[i][start_floor] - motiont)
								car_dn_eta[cons[carnmb]][i] = (flight_times[i][start_floor] - motiont);
	 					    else
								car_dn_eta[cons[carnmb]][i] = 0;
	 					}
	 			    }
	 			    else
	 			    {
							car_dn_eta[cons[carnmb]][i] = (flight_times[i][stops[nmbstops-1]] + total_time);
	 			    }
	 			    if( (downcb[i] == 1) || (carcb[i] == 1) || (rdowncb[i] == 1) || (rcarcb[i] == 1) )
	 			    {
	 					if(nmbstops == 0)
	 					{
	 					    if(procf >= 14)             // at a floor
								total_time =(flight_times[i][position] + time_to_dc);
	 					    else                        // in motion
	 					    {
	 							if(flight_times[i][start_floor] > motiont)
									total_time =(flight_times[i][start_floor]-motiont);
	 							else
									total_time = 0;
	 					    }
	 					}
	 					else
	 					{
							total_time += flight_times[i][stops[nmbstops-1]];
	 					}
	 					if( (downcb[i] == 1) || (rdowncb[i] == 1) )
							total_time += hdoor_time;
	 					else
							total_time += cdoor_time;
	 					stops[nmbstops++] = i;
	 			    }
	 			}
	 			if(nmbstops == 0)
	 			{
	 			    if(procf >= 14)         // at a floor
							car_up_eta[cons[carnmb]][cons[bottomf]] = (flight_times[cons[bottomf]][position] + time_to_dc);
	 			    else                    // in motion
	 			    {
	 					if(flight_times[cons[bottomf]][start_floor] > motiont)
							car_up_eta[cons[carnmb]][cons[bottomf]] = (flight_times[cons[bottomf]][start_floor] - motiont);
	 					else
							car_up_eta[cons[carnmb]][cons[bottomf]]=0;
	 			    }
	 			}
	 			else
	 			{
					    car_up_eta[cons[carnmb]][cons[bottomf]] = (flight_times[cons[bottomf]][stops[nmbstops-1]] + total_time);
	 			}
	 			if( (carcb[cons[bottomf]] == 1) || (upcb[cons[bottomf]] == 1) ||  (lby_req_fl == cons[bottomf]) ||
					    (rcarcb[cons[bottomf]] == 1) || (rupcb[cons[bottomf]] == 1) )
	 			{
	 			    if(nmbstops == 0)
	 			    {
	 					if(procf >= 14)             // at a floor
							total_time =(flight_times[cons[bottomf]][position] + time_to_dc);
	 					else                        // in motion
	 					{
	 					    if(flight_times[cons[bottomf]][start_floor]>motiont)
								total_time =(flight_times[cons[bottomf]][start_floor]-motiont);
	 					    else
								total_time = 0;
	 					}
	 			    }
	 			    else
	 			    {
							total_time += flight_times[cons[bottomf]][stops[nmbstops-1]];
	 			    }
	 			    if( (upcb[cons[bottomf]] == 1) || (rupcb[cons[bottomf]] == 1) || (lby_req_fl == cons[bottomf]) )
						total_time += hdoor_time;
	 			    else
						total_time += cdoor_time;
					stops[nmbstops++] = cons[bottomf];
	 			}
	 			for(i=(cons[bottomf]+1); ((i<=position) && (i < cons[topf])); i++)
	 			{
	 			    if(nmbstops == 0)
	 			    {
	 					if(procf >= 14)         // at a floor
							car_up_eta[cons[carnmb]][i] = (flight_times[i][position] + time_to_dc);
	 					else                    // in motion
						{
	 					    if(flight_times[i][start_floor] > motiont)
								car_up_eta[cons[carnmb]][i] = (flight_times[i][start_floor] - motiont);
	 					    else
								car_up_eta[cons[carnmb]][i] = 0;
	 					}
	 			    }
	 			    else
	 			    {
	 					if(stops[nmbstops-1] < i)
							car_up_eta[cons[carnmb]][i] = (flight_times[stops[nmbstops-1]][i] + total_time);
	 					else
							car_up_eta[cons[carnmb]][i] = (flight_times[i][stops[nmbstops-1]] + total_time);
	 			    }
	 			    if( (upcb[i] == 1) || (rupcb[i] == 1) || (lby_req_fl == i) )
	 			    {
	 					if(nmbstops == 0)
	 					{
	 					    if(procf >= 14)             // at a floor
								total_time =(flight_times[i][position] + time_to_dc);
	 					    else                        // in motion
	 					    {
	 							if(flight_times[i][start_floor]>motiont)
								    total_time =(flight_times[i][start_floor]-motiont);
								else
								    total_time = 0;
						    }
	 					}
	 					else
	 					{
	 					    if(stops[nmbstops -1] < i)
								total_time += flight_times[stops[nmbstops-1]][i];
	 					    else
								total_time += flight_times[i][stops[nmbstops-1]];
	 					}
	 					total_time += hdoor_time;
	 					stops[nmbstops++] = i;
	 			    }
	 			}
	 			for(i=(position + 1); i<cons[topf]; i++)
	 			{
	 			    if(nmbstops == 0)
	 			    {
	 					if(procf >= 14)         // at a floor
							car_up_eta[cons[carnmb]][i] = (flight_times[position][i] + time_to_dc);
	 					else                    // in motion
	 					{
	 					    if(flight_times[position - 1][start_floor] > motiont)
								car_up_eta[cons[carnmb]][i] = (flight_times[position - 1][start_floor] - motiont);
							car_up_eta[cons[carnmb]][i] += flight_times[position - 1][i];
	 					}
	 			    }
	 			    else
	 			    {
						car_up_eta[cons[carnmb]][i] = (flight_times[stops[nmbstops-1]][i] + total_time);
	 			    }
	 			    if( (upcb[i] == 1) || (carcb[i] == 1) || (lby_req_fl == i) || (rupcb[i] == 1) || (rcarcb[i] == 1) )
	 			    {
	 					if(nmbstops == 0)
	 					{
	 					    if(procf >= 14)             // at a floor
								total_time =(flight_times[position][i] + time_to_dc);
	 					    else
	 					    {          // in motion
	 							if(flight_times[position - 1][start_floor] > motiont)
								    total_time =(flight_times[position - 1][start_floor] - motiont);
								total_time += flight_times[position - 1][i];
	 					    }
	 					}
	 					else
	 					{
							    total_time += flight_times[stops[nmbstops-1]][i];
	 					}
	 					if( (upcb[i] == 1) || (rupcb[i] == 1) || (lby_req_fl == i) )
							total_time += hdoor_time;
	 					else
							total_time += cdoor_time;
						stops[nmbstops++] = i;
	 			    }
	 			}
	 			i = cons[topf];
	 			if(position != cons[topf])
	 			{
	 			    if(nmbstops == 0)
	 			    {
	 					if(procf >= 14)         // at a floor
							car_dn_eta[cons[carnmb]][i] = (flight_times[position][i] + time_to_dc);
	 					else                    // in motion
	 					{
	 					    if(flight_times[position - 1][start_floor] > motiont)
								car_dn_eta[cons[carnmb]][i] = (flight_times[position - 1][start_floor] - motiont);
							car_dn_eta[cons[carnmb]][i] += flight_times[position - 1][i];
	 					}
	 			    }
	 			    else
	 			    {
						car_dn_eta[cons[carnmb]][i] = (flight_times[stops[nmbstops-1]][i] + total_time);
	 			    }
	 			    if( (downcb[i] == 1) || (carcb[i] == 1) || (rdowncb[i] == 1) || (rcarcb[i] == 1) )
	 			    {
	 					if(nmbstops == 0)
	 					{
	 					    if(procf >= 14)             // at a floor
								total_time =(flight_times[position][i] + time_to_dc);
	 					    else
	 					    {          // in motion
	 							if(flight_times[position - 1][start_floor] > motiont)
								    total_time =(flight_times[position - 1][start_floor] - motiont);
								total_time += flight_times[position - 1][i];
	 					    }
	 					}
	 					else
	 					{
							    total_time += flight_times[stops[nmbstops-1]][i];
	 					}
	 					if( (downcb[i] == 1) || (rdowncb[i] == 1) )
							total_time += hdoor_time;
	 					else
							total_time += cdoor_time;
						stops[nmbstops++] = i;
	 			    }
	 			}
	 			for(i=(cons[topf]-1); i>position; i--)
	 			{
	 			    if(nmbstops == 0)
	 			    {
	 					if(procf >= 14)         // at a floor
						    car_dn_eta[cons[carnmb]][i] = (flight_times[position][i] + time_to_dc);
						else                    // in motion
						{
						    if(flight_times[position - 1][start_floor] > motiont)
								car_dn_eta[cons[carnmb]][i] = (flight_times[position - 1][start_floor] - motiont);
						    car_dn_eta[cons[carnmb]][i] += flight_times[position - 1][i];
						}
				    }
				    else
				    {
						if(stops[(nmbstops-1)] < i)
							car_dn_eta[cons[carnmb]][i] = (flight_times[stops[nmbstops-1]][i] + total_time);
						else
							car_dn_eta[cons[carnmb]][i] = (flight_times[i][stops[nmbstops-1]] + total_time);
				    }
				    if( (downcb[i] == 1) || (rdowncb[i] == 1) )
				    {
						if(nmbstops == 0)
						{
						    if(procf >= 14)             // at a floor
								total_time =(flight_times[position][i] + time_to_dc);
						    else
						    {          // in motion
								if(flight_times[position - 1][start_floor] > motiont)
								    total_time =(flight_times[position - 1][start_floor] - motiont);
								total_time += flight_times[position - 1][i];
						    }
						}
						else
						{
						    if(stops[nmbstops-1] > i)
							    total_time += flight_times[i][stops[nmbstops-1]];
						    else
							    total_time += flight_times[stops[nmbstops-1]][i];
						}
						total_time += hdoor_time;
						stops[nmbstops++] = i;
					}
				}

				if ((hsf == 1) && (procf >= 3) && (procf <= 11) && (position != cons[bottomf]))   // running high speed	and not at bottom floor
				{
			    	if(nmbstops == 0)
			      	{
						if(flight_times[position - 1][start_floor] > motiont)
							car_dn_eta[cons[carnmb]][position] =(flight_times[position - 1][start_floor] - motiont);
						car_dn_eta[cons[carnmb]][position] += flight_times[position - 1][position];
			      	}
			      	else
			      	{
						if(stops[(nmbstops-1)] < position)
						{
							car_dn_eta[cons[carnmb]][position] = (flight_times[stops[(nmbstops-1)]][position]);
							car_dn_eta[cons[carnmb]][position] += total_time;
						}
						else
						{
							car_dn_eta[cons[carnmb]][position] = (flight_times[position][stops[(nmbstops-1)]]);
							car_dn_eta[cons[carnmb]][position] += total_time;
						}
		      	  	}
	   	    	}
	    	}
	  	}
  	}

  	for(i=1; i<cons[topf]; i++)
  	{   // eta cannot be greater than 255
  	  if (abs(car_up_eta[cons[carnmb]][i]) > 255)
		    car_up_eta[cons[carnmb]][i] = 0xff;
  	}
  	for(i=2; i<=cons[topf]; i++)
  	{   // eta cannot be greater than 255
  	  if (abs(car_dn_eta[cons[carnmb]][i]) > 255)
		    car_dn_eta[cons[carnmb]][i] = 0xff;
  	}

	if((nmbstops != 0) && (procf <= 11))          // in motion
	  next_stop = stops[0];
	else
	  next_stop = 0;

	if((car_Grp_Stat.c_park_fl != 0) && (procf <= 11))
	  next_stop = car_Grp_Stat.c_park_fl;

	// Test for slowdown condition for this car 
	if ((hsf == 0) && (procf >= 3) && (procf <= 11))
	  	next_stop = position;
}

/* 	Revision History

7/11/11 v5.54.11 mhd	1. Added servf == s_TUGS to set stops[0] when car on tugs service.
1/4/13 v7.0.0 mhd		1. Added Riot Control service when cons[prison] == 2, servf = s_RIOT from v5.55.15 atl_fed_pen.
1/29/14 v7.0.32 mhd		1. Changed car_eta[][] array to car_up_eta[][] and car_dn_eta[][].

*/
