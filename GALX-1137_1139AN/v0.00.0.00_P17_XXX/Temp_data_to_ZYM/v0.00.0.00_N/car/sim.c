// Simulator program
#define d_SIM 1
#include "global.h"

#define c_floor_height 144.0


// Simulator Global Variables
int32 sim_DPP_Count;		// dpp counts for simulator
int16 sim_vel_timer;		// multiplied times velocity to get distance traveled
int16 sim_door_timer;		// door motion timer
float fp_DPP_Count;
float fp_distance;

int16 sim_debug;

int32 one_sixteenth_inch;
int32 s_dead_zone;
int32 sd_dist,dist,sim_distance,top_diff,bot_diff;
int16 door_rate, door_timer,pre_pos;
int16 rdoor_rate, rdoor_timer;
int16 sim_gsr, sim_dlr, sim_dpr, sim_rdpr;
int16 sim_gs, sim_dl,sim_dc;
int16 sim_pos;
int16 sim_stall = 0;
int16 sim_SEL_test = 0;
int16 vix;
int16 dzu = 0;
int16 dzd = 0;
int16 sim_dtr =0;

int32 vel[35];
int32 vel_avg;
int32 dist_rem;		// sim distance remainder

int16 sim_gs_open = 0;
int16 sim_gs_close = 0;
int16 sim_dl_open = 0;
int16 sim_dl_close = 0;
int16 sim_gsr_open = 0;
int16 sim_gsr_close = 0;
int16 sim_dlr_open = 0;
int16 sim_dlr_close = 0; 
int16 sim_dc_close = 0;
int16 sim_dc_open = 0;
int16 sim_enc_dir = 0;
int16 sim_ul_dl_swap = 0;
int16 cen_on = 1;

int32 Sim_prev_dpp_count;

int32 sim_mid_pos;

int32 S_DPP_DN;                 // Down Normal's Digital Position
int32 S_DPP_DT;                 // Down Terminal's Digital Positions
int32 S_DPP_Floor_Pos[fl_size]; // Digital position for each floor
int32 S_DPP_UN;                 // UP Normal's Digital Position
int32 S_DPP_UT;                 // Up Terminal's Digital Positions

int16 toggle_array = 0;
int16 toggle_bit = 0;
int16 toggle_state = 0;

int16 dcl_test = 0;
int16 dol_test = 0;
int16 dc_test = 0;

void sim_init(int16 init);
void sim_hoistway (void);
void sim_motion (void);
void sim_motion_dpp_count (void);
void sim_door (void);
void sim_control (void);

int16 dn_hc_range_1;
int16 dn_hc_range_2;
int16 dn_hc_range_3;
int16 dn_hc_range_4;

struct hoistway_struct
{
	long dnl;
	long unl;
	long dtl;
	long utl;
	long pps[20]; // zero location unused
};

typedef struct hoistway_struct hoistway_struct_type;
hoistway_struct_type hstw;
int hw_tbl_file = 0;

float Sim_Vel;
float Sim_Accel;
float Sim_Vel_Step;
float Sim_Vel_Set;
	


void sim_init(int16 init)
{
	static int16 i,j;
	int32 a;
	float utdt_dist = 0.0;

	if ((cons[sercom] & 0x20) == 0)
	{		   // no serial hall call boards
		dn_hc_range_1 = 13;
		dn_hc_range_2 = 25;
		dn_hc_range_3 = 37;
		dn_hc_range_4 = 49;
	}
	else
	{	
		dn_hc_range_1 = 12;
		dn_hc_range_2 = 24;
		dn_hc_range_3 = 36;
		dn_hc_range_4 = 48;
	}

	Pulses_per_Inch = 50.8;
  	Sim_Accel = ((float)(110.0 * Pulses_per_Inch) / (float)5.0);  			// Pules/sec^2  (accel rate of 120 fpm/sec)
  	Sim_Vel_Step = (Sim_Accel / (float)100.00);						// ten msec
	Sim_Vel_Set = (float)(cons[speed] * Pulses_per_Inch)/(float)5.0;    // Velocity from feet/minute to pulses/sec
	Sim_Vel = 0;
	
	Hoistway_Learned = 1;

	if ((fvars[fvppstop] == 1) && (cons[dpp_upd] != 0))
		s_dead_zone = (int32)(Pulses_per_Inch/8.0);
	else
		s_dead_zone = (int32)(Pulses_per_Inch/16.0);
	
	one_sixteenth_inch = (int32)(Pulses_per_Inch/16.0);
	
	if (one_sixteenth_inch == 0)
		one_sixteenth_inch = 1;
	if (s_dead_zone == 0)
		s_dead_zone = one_sixteenth_inch;
	
	sd_dist = (int32)(40 * Pulses_per_Inch) - (int32)(Pulses_per_Inch * 2);

    if(hw_tbl_file == 0) // no hoistway.dat file
    {
		Set_limit_ix();
		
		utdt_dist = (float)limit_dist[limit_ix];
		
	    a = ((int32)(Hoistway_Init_Count + (2.0 * Pulses_per_Inch)));
	    for(i = cons[bottomf]; i <= cons[topf]; i++)
	    {
			S_DPP_Floor_Pos[i] = a;
			if ((cons[shortfl] != 0) && (cons[shortfl] == i) && (cons[vshort_tfl] == (i+1)))
			{
				if ((fvars[fvshrtflctl] & 0x01) != 0)
					a += ((int32)(10.5 * Pulses_per_Inch));
				else
					a += ((int32)(2.5 * Pulses_per_Inch));
			}
			else if ((cons[shortfl] != 0) && (cons[shortfl] == i) && (cons[vshort_tfl] == 0))
			{
				if ((fvars[fvshrtflctl] & 0x02) != 0)
					a += ((int32)(utdt_dist * Pulses_per_Inch));
				else
					a += ((int32)(18.0 * Pulses_per_Inch));
			}
			else if ((cons[shortflmid] != 0) && (cons[shortflmid] == i))
			{
				if ((fvars[fvshrtflctl] & 0x04) != 0)
					a += ((int32)(utdt_dist * Pulses_per_Inch));
				else
					a += ((int32)(18.0 * Pulses_per_Inch));
			}
			else
				a += ((int32)(c_floor_height * Pulses_per_Inch));
	    }
	    S_DPP_DN = Hoistway_Init_Count;// down normal position -2 inches
	    S_DPP_DT = ((int32)(Hoistway_Init_Count + (utdt_dist * Pulses_per_Inch))); // Down termianl position 10 inches

	    a = S_DPP_Floor_Pos[cons[topf]];
	    S_DPP_UT = ((int32)(a - (utdt_dist * Pulses_per_Inch))); // Up terminal position 10 inches
	    S_DPP_UN = ((int32)(a + (3.0 * Pulses_per_Inch))); // Up normal position +2 inches

		sim_mid_pos = S_DPP_Floor_Pos[cons[bottomf]] + (S_DPP_Floor_Pos[cons[topf]] - S_DPP_Floor_Pos[cons[bottomf]])/2;
    }
    else
    {

	    for(i = 1; i <= cons[topf]; i++)
	    {
	    	a = ((int32)(Hoistway_Init_Count + (hstw.pps[i] * Pulses_per_Inch))); 
	    	S_DPP_Floor_Pos[i] = a;
	    }

	    S_DPP_DN = Hoistway_Init_Count + (hstw.dnl * Pulses_per_Inch);	// DN 
		S_DPP_DT = Hoistway_Init_Count + (hstw.dtl * Pulses_per_Inch); // DT,DT1->DT6

	    S_DPP_UN = Hoistway_Init_Count + (hstw.unl * Pulses_per_Inch);	// UN 
		S_DPP_UT = Hoistway_Init_Count + (hstw.utl * Pulses_per_Inch); // UT,UT1->UT6

		sim_mid_pos = S_DPP_Floor_Pos[cons[bottomf]] + (S_DPP_Floor_Pos[cons[topf]] - S_DPP_Floor_Pos[cons[bottomf]])/2;

    }
    
	sim_gs = 0;
	sim_dl = 0;
	sim_gsr = 0;
	sim_dlr = 0;
	sim_dpr = 0;
	sim_rdpr = 0;
	door_rate = 1;
	door_timer = 0;	   // Start with the door closed

	if(fvars[fvins] == 41)
	{
		door_timer = 50;	   // Start with the door halfway
	}


	pre_pos = 0;
	if (init == 1)
	{
		sim_pos = position;
		sim_DPP_Count = S_DPP_Floor_Pos[sim_pos];
	}
	else
	{
		sim_pos = cons[bottomf] + 1;
		sim_DPP_Count = S_DPP_Floor_Pos[sim_pos];
	}

	fp_DPP_Count = (float)sim_DPP_Count;

	if (cons[sel_type] != 0)
	{
		SEL_Count = sim_DPP_Count;
		SEL_Count_Valid = 1;
		SEL_Bottom_Count = S_DPP_Floor_Pos[cons[bottomf]];
		SEL_Top_Count = S_DPP_Floor_Pos[cons[topf]];
	}

#if (Sim_Motion == 1)
	DPP_Count = sim_DPP_Count;
#endif

	Sim_prev_dpp_count = DPP_Count;


	// Initialize simulator i/o
	for (i=0;i<nmb_io_arrays;i++)
	{
		ioclr[i] = 0xFF;
		ioset[i] = 0;
	}
	for (i=0;i<c_max_ctcan_ser_array;i++)
	{
		ctc_ioclr[i] = 0xff;
		ctc_ioset[i] = 0;
	}
	for (i=0;i<c_max_mrcan_ser_array;i++)
	{
		mrc_ioclr[i] = 0xff;
		mrc_ioset[i] = 0;
	}
	for (i=0;i<c_max_grcan_ser_array;i++)
	{
		grc_ioclr[i] = 0xff;
		grc_ioset[i] = 0;
	}

	for(i=1; i<= cons[topf]; i++)
	{
		carcb[i] = 0;
		downcb[i] = 0;
		upcb[i] = 0;
		rcarcb[i] = 0;
		rdowncb[i] = 0;
		rupcb[i] = 0;
	}

}

void sim_hoistway (void)
{

	bool ul_on = 0;
	bool dl_on = 0;

	if (rdoutp(o_CEN) == 1)
		cen_on = 1;
	else
		cen_on = 0;
		
	if ((rdoutp(o_SU) == 1) && (rdoutp(o_SD) == 1))
		setinp(i_PALF);
	else
		clrinp(i_PALF);

	if (sim_pos == cons[bottomf])
		bot_diff = S_DPP_Floor_Pos[sim_pos] - eight_inches;
	else
		bot_diff =	(S_DPP_Floor_Pos[sim_pos] - S_DPP_Floor_Pos[sim_pos-1])/2;

	if (sim_pos == cons[topf])
		top_diff = S_DPP_Floor_Pos[sim_pos] + eight_inches;
	else
		top_diff =	(S_DPP_Floor_Pos[sim_pos+1] - S_DPP_Floor_Pos[sim_pos])/2;
	
	if (rdoutp(o_SU) == 1)
	{
		if ((sim_DPP_Count > (S_DPP_Floor_Pos[sim_pos] + top_diff)) && (sim_pos < cons[topf]))
			sim_pos++;
	}
	else if (rdoutp(o_SD) == 1)
	{

		if ((sim_DPP_Count < (S_DPP_Floor_Pos[sim_pos] - bot_diff)) && (sim_pos > cons[bottomf]))
			sim_pos--;
	}

	// floor level


	if (cons[StagRear] == 0)
	{
		if (cons[vshort_tfl] != 0)
		{ 
			if (rdoutp(o_DTR) == 1)
				sim_dtr = 1;
			else
				sim_dtr = 0;
		}
		else
			sim_dtr = 0;
	}
	else if (cons[exDHwL] == 0)
	{
		if (cons[vshort_tfl] != 0)
		{ 
			if (rdoutp(o_SFST) == 1)
				sim_dtr = 1;
			else
				sim_dtr = 0;
		}
		else
			sim_dtr = 0;
	}
	else
		sim_dtr = 0;
  
  
  
	if (valid_fl[sim_pos] == 1)
	{	// Have a valid floor

		if ((cons[vshort_tfl] != 0) && (sim_dtr == 1))
		{				// Have a very short floor and using secondary selector
			if( (labs(S_DPP_Floor_Pos[cons[vshort_tfl]] - sim_DPP_Count)) < eight_inches ) 
			{	   // within 8 inches

				if (sim_DPP_Count <= (S_DPP_Floor_Pos[cons[vshort_tfl]] + s_dead_zone))
					setinp(i_UL);		// On up level
				else
					clrinp(i_UL);

				if (sim_DPP_Count >= (S_DPP_Floor_Pos[cons[vshort_tfl]] - s_dead_zone)) 
					setinp(i_DL);		// On down level 
				else
					clrinp(i_DL);	
			}
			else
			{
				clrinp(i_DL);	
				clrinp(i_UL);
			}

			if( (labs(S_DPP_Floor_Pos[cons[vshort_tfl]] - sim_DPP_Count)) < three_inches ) 
			{
				setinp(i_DZ);	// within 3 inches
//				setinp(i_DZA);
			}
			else
			{
				clrinp(i_DZ);
//				clrinp(i_DZA);
			}

		}
		else if ((cons[vshort_tfl] != 0) && (sim_pos == cons[vshort_tfl]) && (sim_dtr == 0))
		{				// Have a very short floor and using primary selector
			if( (labs(S_DPP_Floor_Pos[cons[shortfl]] - sim_DPP_Count)) < eight_inches ) 
			{	   // within 8 inches

				if (sim_DPP_Count <= (S_DPP_Floor_Pos[cons[shortfl]] + s_dead_zone))
					setinp(i_UL);		// On up level
				else
					clrinp(i_UL);

				if (sim_DPP_Count >= (S_DPP_Floor_Pos[cons[shortfl]] - s_dead_zone)) 
					setinp(i_DL);		// On down level 
				else
					clrinp(i_DL);	
			}
			else
			{
				clrinp(i_DL);	
				clrinp(i_UL);
			}

			if( (labs(S_DPP_Floor_Pos[cons[shortfl]] - sim_DPP_Count)) < three_inches ) 
			{
				setinp(i_DZ);	// within 3 inches
//				setinp(i_DZA);
			}
			else
			{
				clrinp(i_DZ);
//				clrinp(i_DZA);
			}
		}
		else
		{		// Use normal selector
			if( (labs(S_DPP_Floor_Pos[sim_pos] - sim_DPP_Count)) < (eight_inches + s_dead_zone) ) 
			{	   // within 8 inches

				if (sim_ul_dl_swap == 1)
				{					 // Swap ul and dl for test
					if (sim_DPP_Count >= (S_DPP_Floor_Pos[sim_pos] - s_dead_zone))
						setinp(i_UL);		// On up level
					else
						clrinp(i_UL);

					if (sim_DPP_Count <= (S_DPP_Floor_Pos[sim_pos] + s_dead_zone)) 
						setinp(i_DL);		// On down level 
					else
						clrinp(i_DL);	
				}
				else
				{					// normal ul and dl
				
/*					if (cons[sel_type] == 0)
					{
						
						if (sim_DPP_Count <= (S_DPP_Floor_Pos[sim_pos] + s_dead_zone))
							setinp(i_UL1);		// On up level
						else
							clrinp(i_UL1);

						if (sim_DPP_Count <= (S_DPP_Floor_Pos[sim_pos] + s_dead_zone + one_sixteenth_inch))
							setinp(i_UL2);		// On up level
						else
							clrinp(i_UL2);

						if (sim_DPP_Count <= (S_DPP_Floor_Pos[sim_pos] + s_dead_zone + (2 * one_sixteenth_inch)))
							setinp(i_UL3);		// On up level
						else
							clrinp(i_UL3);

						if (sim_DPP_Count <= (S_DPP_Floor_Pos[sim_pos] + s_dead_zone + (3 * one_sixteenth_inch)))
							setinp(i_UL4);		// On up level
						else
							clrinp(i_UL4);

						if (sim_DPP_Count >= (S_DPP_Floor_Pos[sim_pos] - s_dead_zone)) 
							setinp(i_DL1);		// On down level 
						else
							clrinp(i_DL1);
						
						if (sim_DPP_Count >= (S_DPP_Floor_Pos[sim_pos] - s_dead_zone - one_sixteenth_inch)) 
							setinp(i_DL2);		// On down level 
						else
							clrinp(i_DL2);	
						if (sim_DPP_Count >= (S_DPP_Floor_Pos[sim_pos] - s_dead_zone - (2 * one_sixteenth_inch))) 
							setinp(i_DL3);		// On down level 
						else
							clrinp(i_DL3);	
						if (sim_DPP_Count >= (S_DPP_Floor_Pos[sim_pos] - s_dead_zone - ( 3 * one_sixteenth_inch))) 
							setinp(i_DL4);		// On down level 
						else
							clrinp(i_DL4);	
						
						
						ul_on = 0;
						if (((fvars[fvuldeadzone] & 0x01) || (fvars[fvuldeadzone] == 0)) && (rdinp(i_UL1) == 1))
							 ul_on |= 1;
						if ((fvars[fvuldeadzone] & 0x02) && (rdinp(i_UL2) == 1))
							 ul_on |= 1;
						if ((fvars[fvuldeadzone] & 0x04) && (rdinp(i_UL3) == 1))
							 ul_on |= 1;
						if ((fvars[fvuldeadzone] & 0x08) && (rdinp(i_UL4) == 1))
							 ul_on |= 1;
						
						if (ul_on == 1)
							setinp(i_UL);		// On up level
						else
							clrinp(i_UL);
						
						dl_on = 0;
						if (((fvars[fvdldeadzone] & 0x01) || (fvars[fvdldeadzone] == 0)) && (rdinp(i_DL1) == 1))
							 dl_on |= 1;
						if ((fvars[fvdldeadzone] & 0x02) && (rdinp(i_DL2) == 1))
							 dl_on |= 1;
						if ((fvars[fvdldeadzone] & 0x04) && (rdinp(i_DL3) == 1))
							 dl_on |= 1;
						if ((fvars[fvdldeadzone] & 0x08) && (rdinp(i_DL4) == 1))
							 dl_on |= 1;

						if (dl_on == 1) 
							setinp(i_DL);		// On down level 
						else
							clrinp(i_DL);
					}
					else */
//					{					// normal ul and dl
//						if (sim_DPP_Count <= (S_DPP_Floor_Pos[sim_pos] + s_dead_zone))
//							setinp(i_UL);		// On up level
//						else
//							clrinp(i_UL);
//
//						if (sim_DPP_Count >= (S_DPP_Floor_Pos[sim_pos] - s_dead_zone)) 
//							setinp(i_DL);		// On down level 
//						else
//							clrinp(i_DL);	
//					}
					
				}
			}
			else
			{
//				clrinp(i_DL);	
				clrinp(i_DL1);	
				clrinp(i_DL2);	
				clrinp(i_DL3);	
				clrinp(i_DL4);	
//				clrinp(i_UL);
				clrinp(i_UL1);
				clrinp(i_UL2);
				clrinp(i_UL3);
				clrinp(i_UL4);
			}

			if( (labs(S_DPP_Floor_Pos[sim_pos] - sim_DPP_Count)) < five_inches ) 
			{	   // within 5 inches

				if (sim_DPP_Count <= (S_DPP_Floor_Pos[sim_pos] + three_inches))
					dzu = 1;		// On up level
				else
					dzu = 0;

				if (sim_DPP_Count >= (S_DPP_Floor_Pos[sim_pos] - three_inches)) 
					dzd = 1;		// On down level 
				else
					dzd = 0;	
			}
			else
			{
				dzu = 0;
				dzd = 0;	
			}
			
			if (dzu == 1)
				setinp(i_DZU);
			else
				clrinp(i_DZU);
			
			if (dzd == 1)
				setinp(i_DZD);
			else
				clrinp(i_DZD);


//			if ((dzu == 1) || (dzd == 1))
//				setinp(i_DZA);
//			else
//				clrinp(i_DZA);

			if ((dzu == 1) && (dzd == 1))
			{
				setinp(i_DZ);	// within 3 inches
			}
			else		
				clrinp(i_DZ);
		}
	}
	else
	{
		clrinp(i_DL);	
		clrinp(i_DL1);	
		clrinp(i_DL2);	
		clrinp(i_DL3);	
		clrinp(i_DL4);	
		clrinp(i_UL);
		clrinp(i_UL1);
		clrinp(i_UL2);
		clrinp(i_UL3);
		clrinp(i_UL4);

		clrinp(i_DZ);
//		clrinp(i_DZA);
	}

//	if (rdinp(i_DZ) == 1)
//		setinp(i_DZsel);
//	else
//		clrinp(i_DZsel);
	

	
	// Terminal limits

	if (sim_DPP_Count <= S_DPP_DN)
	{
//		clrinp(i_DNsel);
		clrinp(i_DN);
	}
	else
	{
//		setinp(i_DNsel);
		setinp(i_DN);
	}
	if (sim_DPP_Count >= S_DPP_UN)
	{
//		clrinp(i_UNsel);
		clrinp(i_UN);
	}
	else
	{
//		setinp(i_UNsel);
		setinp(i_UN);
	}
	
	if (sim_DPP_Count <= S_DPP_DT)
	{
//		clrinp(i_DTsel);
		clrinp(i_DT);
	}
	else 
	{
//		setinp(i_DTsel);
		setinp(i_DT);
	}
	if (sim_DPP_Count >= S_DPP_UT)
	{
//		clrinp(i_UTsel);
		clrinp(i_UT);
	}
	else 
	{
//		setinp(i_UTsel);
		setinp(i_UT);
	}
}

bool start_of_run = 0;
bool nts_ut_hit = 0;
bool nts_dt_hit = 0;


void sim_motion (void)
{
	int16 i;


	if (((rdoutp(o_SU) == 1) || (rdoutp(o_SD) == 1)) ||
	   ((cons[Micro_Relev] == 1) && ((rdoutp(o_UPML) == 1) || (rdoutp(o_DNML) == 1))))
	{
	
		if ((start_of_run == 0) && (in_redundancy_test == 0))
		{
			start_of_run = 1;
			nts_ut_hit = 0;
			NTS_spi.ut_vel = 0;
			nts_dt_hit = 0;
			NTS_spi.dt_vel = 0;
		}
		
		if ((sim_vel_timer >= 1) && (sim_stall == 0))
		{

			if ((rdoutp(o_SUF) == 1) || (rdoutp(o_SDF) == 1))
			{
				Sim_Vel_Set = ((float)cons[speed] * Pulses_per_Inch)/(float)5.0;    // Velocity from feet/minute to pulses per 10 msec
				if (Sim_Vel < Sim_Vel_Set)
				{
					Sim_Vel += Sim_Vel_Step;
				}
				else 
					Sim_Vel = Sim_Vel_Set;
				if (Sim_Vel > Sim_Vel_Set)
					Sim_Vel = Sim_Vel_Set;
				fp_distance = Sim_Vel/(float)(100.0);
//				sim_distance = ((int32)cons[speed] * (int32)Pulses_per_Inch)/500;
			}
			else if ((rdinp(i_UL) == 1) || (rdinp(i_DL) == 1) || (rdinp(i_DZ) == 1) || (rdinp(i_DZA) == 1))
			{
				Sim_Vel = (float)(10.0 * Pulses_per_Inch)/(float)5.0;    // Velocity from feet/minute to pulses per 10 msec
				fp_distance = Sim_Vel/(float)(100.0);
//				sim_distance = ((int32)10 * (int32)Pulses_per_Inch)/500;	
			}
			else if ((cons[Micro_Relev] == 1) && ((rdoutp(o_UPML) == 1) || (rdoutp(o_DNML) == 1)))
			{
				Sim_Vel_Set = (float)(1.0 * Pulses_per_Inch)/(float)5.0;    // Velocity from feet/minute to pulses per 10 msec
				if (Sim_Vel < Sim_Vel_Set)
				{
					Sim_Vel += Sim_Vel_Step;
				}
				else if (Sim_Vel > Sim_Vel_Set)
				{
					Sim_Vel -= Sim_Vel_Step;
				}
				else 
					Sim_Vel = Sim_Vel_Set;
				fp_distance = Sim_Vel/(float)(100.0);
//				sim_distance = ((int32)1 * (int32)Pulses_per_Inch)/500;	
			}
			else
			{
				Sim_Vel_Set = (float)(10.0 * Pulses_per_Inch)/(float)5.0;    // Velocity from feet/minute to pulses per 10 msec 
				if (Sim_Vel > Sim_Vel_Set)
				{
					Sim_Vel -= Sim_Vel_Step;
				}
				else 
					Sim_Vel = Sim_Vel_Set;
				
				if (Sim_Vel < Sim_Vel_Set)
					Sim_Vel = Sim_Vel_Set;
				
				fp_distance = Sim_Vel/(float)(100.0);
//					sim_distance = ((int32)30 * (int32)Pulses_per_Inch)/500;	
			}
			
			
			// Both position counts change at the same rate.
			// The controller will synchronize the two.
			if ((rdoutp(o_SU) == 1) && (rdoutp(o_SD) == 0))
			{
				
				fp_DPP_Count = fp_DPP_Count + fp_distance;
				NTS_spi.vel_dir = 1;
			}
			else if ((rdoutp(o_SD) == 1) && (rdoutp(o_SU) == 0))
			{
				if (sim_DPP_Count > (S_DPP_DN - 508))
				{						// Greater than 10 inches below the down normal limit (used for jack reset)
					fp_DPP_Count = fp_DPP_Count - fp_distance;
				}
				NTS_spi.vel_dir = 2;
			}
			else
			{
				
				if (cons[Micro_Relev] == 1)
				{
					if  (rdoutp(o_UPML) == 1)
						fp_DPP_Count = fp_DPP_Count + fp_distance;
					else if (rdoutp(o_DNML) == 1)
						fp_DPP_Count = fp_DPP_Count - fp_distance;
				}
				NTS_spi.vel_dir = 0;
			}

			enc_vel_timer ++;
			sim_vel_timer = 0;

		}
		
		if ((rdinp(i_UTsel) == 0) && (nts_ut_hit == 0))
		{
			nts_ut_hit = 1;
			NTS_spi.ut_vel = NTS_spi.velocity;
		}
		if ((rdinp(i_DTsel) == 0) && (nts_dt_hit == 0))
		{
			nts_dt_hit = 1;
			NTS_spi.dt_vel = NTS_spi.velocity;
		}
		NTS_spi.velocity = (int16)(((Sim_Vel * 5.0)/(Pulses_per_Inch)) + 0.5);
	}	
	else
	{
		Sim_Vel = 0;
		start_of_run = 0;
		if (sim_vel_timer >= 1)
		{
			sim_vel_timer = 0;
			enc_vel_timer ++;		 // Incremented here for simulation else the velocity calc is off
		}
		dist_rem = 0;
		for (i=0;i<=20;i++)
			vel[i] = 0; 
		vix = 0; 
		NTS_spi.velocity = 0;
		NTS_spi.vel_dir = 0;
			
	}

	sim_DPP_Count = (int32)fp_DPP_Count;
	DPP_Count = sim_DPP_Count;
	NTS_spi.pos_cnt = DPP_Count - 130;
}

void sim_motion_dpp_count (void)
{

	if ((rdoutp(o_SU) == 1) || (rdoutp(o_SD) == 1))
	{
		if (Sim_prev_dpp_count > DPP_Count)
			sim_distance = Sim_prev_dpp_count - DPP_Count;
		else
			sim_distance = DPP_Count - Sim_prev_dpp_count;

		Sim_prev_dpp_count = DPP_Count;

		// The controller will synchronize the two.
		if ((rdoutp(o_SU) == 1) && (rdoutp(o_SD) == 0))
			sim_DPP_Count = sim_DPP_Count + sim_distance;
		else if ((rdoutp(o_SD) == 1) && (rdoutp(o_SU) == 0))
			sim_DPP_Count = sim_DPP_Count - sim_distance;
	}	
	else
		Sim_prev_dpp_count = DPP_Count;
}

void sim_door (void)
{

	int16 dl_enable;

	/* Front door operation */

	if (rdoutp(o_NUD) == 1)
		door_rate = sim_door_timer;		// slower during nudging
	else
		door_rate = sim_door_timer * 2;

	if ((rdoutp(o_DO) == 1) || (rdoutp(o_DC) == 1) || (dc_test == 1))
	{
		if ((rdoutp(o_DO) == 1) && ((manual_door == 0) || (manual_door == 2) || (cons[frghtd] == 6) || (cons[frghtd] ==8)))
		{
			if (door_timer < 100)
				door_timer += door_rate;
		}
		else if ((rdoutp(o_DC) == 1) || (dc_test == 1))
		{
			if (door_timer > door_rate)
				door_timer -= door_rate;
			else
				door_timer = 0;
		}	
	}

	// Door close Limit
	if (dcl_test == 1)
		setinp(i_DCL);
	else   
	{
		if (door_timer < 4)
			clrinp(i_DCL);
		else
			setinp(i_DCL);
	}
	
	// Door open Limit
	if (dol_test == 1)
		setinp(i_DOL);
	else   
	{
		if (door_timer > 80)
			clrinp(i_DOL);
		else
			setinp(i_DOL);
	}

	
	/* Rear door operation */

	if (cons[rear] == 1)
	{		// have rear doors

		NTS_spi.Status2.B.rear_door = 1;
		
		if (rdoutp(o_NUDR) == 1)
			rdoor_rate = sim_door_timer;		// slower during nudging
		else
			rdoor_rate = sim_door_timer * 2;

		if ((rdoutp(o_DOR) == 1) || (rdoutp(o_DCR) == 1))
		{
			if ((rdoutp(o_DOR) == 1) && ((manual_rdoor == 0) || (manual_rdoor == 2) || (cons[frghtd] == 6) || (cons[frghtd] ==8)))
			{
				if (rdoor_timer < 60)
					rdoor_timer += rdoor_rate;
			}
			else if (rdoutp(o_DCR) == 1)
			{
				if (rdoor_timer > rdoor_rate)
					rdoor_timer -= rdoor_rate;
				else
					rdoor_timer = 0;
			}	
		}

		// Rear Door close Limit
		if (rdoor_timer < 4)
			clrinp(i_DCLR);
		else
			setinp(i_DCLR);

		// Rear Door open Limit
		if (rdoor_timer > 48)
			clrinp(i_DOLR);
		else
			setinp(i_DOLR);

	}
	else 
	{
		NTS_spi.Status2.B.rear_door = 0;
		rdoor_timer = 0;
	}
	
	sim_door_timer = 0;


	if ((cons[mand] != 0) || ((cons[frghtd] != 0) && (cons[frghtd] <= 8))) 
	{
		if (rdoutp(o_RCM) == 1)
			dl_enable = 1;	  // retiring cam is picked
		else
			dl_enable = 0;
	}
	else
		dl_enable = 1;

	// Gate and Lock
	if (door_timer < 5)
	{
		sim_gs = 1;
		if ((dl_enable == 1) || (manual_door == 0))
			sim_dl = 1;
		else
			sim_dl = 0;
	}
	else
	{
		sim_gs = 0;
		sim_dl = 0;
	}

	if (rdoor_timer < 5)
	{
		sim_gsr = 1;
		if ((dl_enable == 1) || (manual_rdoor == 0))
			sim_dlr = 1;
		else
			sim_dlr = 0;
	}
	else
	{
		sim_gsr = 0;
		sim_dlr = 0;
	}

	if (manual_door == 2)
		sim_dpr = 1; 	// set to 1 all the time so bit can be cleared by keyboard command
	else
	{
		// Door Protection
		if (door_timer < 12)
			sim_dpr = 1;
		else
			sim_dpr = 0;
	}

	if (manual_rdoor == 2)
		sim_rdpr = 1;
	else
	{
		if (rdoor_timer < 12)
			sim_rdpr = 1;
		else
			sim_rdpr = 0;
	}

	if (sim_gs_open == 1)
		sim_gs = 0;
	if (sim_gs_close == 1)
		sim_gs = 1;
	if (sim_dl_open == 1)
		sim_dl = 0;
	if (sim_dl_close == 1)
		sim_dl = 1;

	if (sim_gsr_open == 1)
		sim_gsr = 0;
	if (sim_gsr_close == 1)
		sim_gsr = 1;
	if (sim_dlr_open == 1)
		sim_dlr = 0;
	if (sim_dlr_close == 1)
		sim_dlr = 1;


	if ((cons[mand] != 0) || ((cons[frghtd] != 0) && (cons[frghtd] <= 8)))
	{
		if (sim_dc_open == 1)
			sim_dc = 0;
		if (sim_dc_close == 1)
			sim_dc = 1;

		if (sim_dc == 1)
		{
			sim_rdpr = 1;
			sim_dpr = 1;
		}
	}

	if ((cons[frghtd] != 0) && (cons[frghtd] <= 5))
	{		   // simulate the dol connected to gs and dcl not connected
		clrinp(i_DCL);
		clrinp(i_DCLR);
	
		if (sim_gs == 1)
			setinp(i_DOL);
		else
			clrinp(i_DOL);
		if (sim_gsr == 1)
			setinp(i_DOLR);
		else
			clrinp(i_DOLR);
	}

	
}

void sim_control (void)
{

int16 i;

#if (Sim_Control == 1)

	if (Drive_Type[cons[carnmb]] == 1)
	{
		if (rdoutp(o_MST) == 1)
			setinp(i_SPD);
		else
			clrinp(i_SPD);
		if ((rdoutp(o_MCC) == 1) && (rdoutp(o_MST) == 1))
			setinp(i_MCC);
		else
			clrinp(i_MCC);
	}
	else // if (Drive_Type[cons[carnmb]] == 9)
	{
		if (rdoutp(o_MCC) == 1)
			setinp(i_MCC);
		else
			clrinp(i_MCC);
		
		if (rdinp(i_MCC) == 1)
			setinp(i_SPD);
		else
			clrinp(i_SPD);
		
		if(cons[hotoilSW] == 2)
		{
			if (rdoutp(o_CEN) == 1) 
				setinp(i_TPH);
			else
				clrinp(i_TPH);
		}
		
	}


	if (rdoutp(o_RUN) == 1)
		setinp(i_RUN);
	else
		clrinp(i_RUN);
	
	if (rdinp(i_RUN) == 1)
	{
		

		if ((rdoutp(o_SU) == 1) && (rdinp(i_UNsel) == 1))
			setinp(i_SU);
		else
			clrinp(i_SU);

		if ((rdoutp(o_SD) == 1) && (rdinp(i_DNsel) == 1))
			setinp(i_SD);
		else
			clrinp(i_SD);

		if ((rdoutp(o_SUF) == 1) && (rdinp(i_UTsel) == 1))
			setinp(i_SUF);
		else
			clrinp(i_SUF);

		if ((rdoutp(o_SDF) == 1) && (rdinp(i_DTsel) == 1))
			setinp(i_SDF);
		else
			clrinp(i_SDF);
	}
	else
	{
		clrinp(i_SU);
		clrinp(i_SD);
		clrinp(i_SUF);
		clrinp(i_SDF);
	}

#endif



#if (Sim_Door == 1)
	if (sim_pos == fvars[fvaccbotfl])
	{
		setinp(i_DLT);
		setinp(i_DLT_1);
		if (cons[access_type] == 0)
		{
			if ((sim_dl == 1) && (sim_dlr == 1))
			{	  // front and rear doors close the top lock
				setinp(i_DLB);
				setinp(i_DLB_1);
			}
			else
			{
				clrinp(i_DLB);
				clrinp(i_DLB_1);
			}
			setinp(i_RLM);
			setinp(i_RLM_1);
			setinp(i_DLM);
			setinp(i_DLM_1);
		}
		else 
		{
			if ((cons[access_type] & 2) != 0)
			{	// Bottom front access
				if (sim_dl == 1)
				{  // front door closes the bottom lock
					setinp(i_DLB);
					setinp(i_DLB_1);
				}
				else
				{
					clrinp(i_DLB);
					clrinp(i_DLB_1);
				}
				if (sim_dlr == 1)
				{	 // rear door closes the middle lock
					setinp(i_RLM);
					setinp(i_RLM_1);
				}
				else
				{
					clrinp(i_RLM);
					clrinp(i_RLM_1);
				}
				setinp(i_DLM);
				setinp(i_DLM_1);
			}
			else if ((cons[access_type] & 8) != 0)
			{	// Bottom rear access
				if (sim_dlr == 1)
				{	   // rear door closes the bottom lock
					setinp(i_DLB);
					setinp(i_DLB_1);
				}
				else
				{
					clrinp(i_DLB);
					clrinp(i_DLB_1);
				}
				if (sim_dl == 1)
				{	  // Front door closes the middle lock
					setinp(i_DLM);
					setinp(i_DLM_1);
				}
				else
				{
					clrinp(i_DLM);
					clrinp(i_DLM_1);
				}
				setinp(i_RLM);
				setinp(i_RLM_1);
			}
		}
	}
	else if (sim_pos == fvars[fvacctopfl])
	{
		setinp(i_DLB);
		setinp(i_DLB_1);
		if (cons[access_type] == 0)
		{
			if ((sim_dl == 1) && (sim_dlr == 1))
			{	  // front and rear door closes the top lock
				setinp(i_DLT);
				setinp(i_DLT_1);
			}
			else
			{
				clrinp(i_DLT);
				clrinp(i_DLT_1);
			}
			setinp(i_RLM);
			setinp(i_RLM_1);
			setinp(i_DLM);
			setinp(i_DLM_1);
		}
		else 
		{
			if ((cons[access_type] & 1) != 0)
			{	// Top front access
				if (sim_dl == 1)
				{  // front door closes the top lock
					setinp(i_DLT);
					setinp(i_DLT_1);
				}
				else
				{
					clrinp(i_DLT);
					clrinp(i_DLT_1);
				}
				if (sim_dlr == 1)
				{	  // Rear door closes the middle lock
					setinp(i_RLM);
					setinp(i_RLM_1);
				}
				else
				{
					clrinp(i_RLM);
					clrinp(i_RLM_1);
				}
				setinp(i_DLM);
				setinp(i_DLM_1);
			}
			else if ((cons[access_type] & 4) != 0)
			{	// Top rear access
				if (sim_dlr == 1)
				{	   // rear door closes the top lock
					setinp(i_DLT);
					setinp(i_DLT_1);
				}
				else
				{
					clrinp(i_DLT);
					clrinp(i_DLT_1);
				}
				if (sim_dl == 1)
				{	  // Front door closes the middle lock
					setinp(i_DLM);
					setinp(i_DLM_1);
				}
				else
				{
					clrinp(i_DLM);
					clrinp(i_DLM_1);
				}
				setinp(i_RLM);
				setinp(i_RLM_1);
			}
		}
	}
	else if (rdinp(i_ACC) == 0)
	{		// not at the top or bottom so set middle locks
		setinp(i_DLT);
		setinp(i_DLT_1);
		setinp(i_DLB);
		setinp(i_DLB_1);
		if (sim_dl == 1)
		{
			setinp(i_DLM);
			setinp(i_DLM_1);
		}
		else
		{
			clrinp(i_DLM);
			clrinp(i_DLM_1);
		}
		if (sim_dlr == 1)
		{
			setinp(i_RLM);
			setinp(i_RLM_1);
		}
		else
		{
			clrinp(i_RLM);
			clrinp(i_RLM_1);
		}
	}

	if (sim_gs == 1)
	{
		setinp(i_GS);
		setinp(i_GS_1);
	}
	else
	{
		clrinp(i_GS);
		clrinp(i_GS_1);
	}

	if (sim_gsr == 1)
	{
		setinp(i_RGS);
		setinp(i_RGS_1);
	}
	else
	{
		clrinp(i_RGS);
		clrinp(i_RGS_1);
	}




	if (sim_dpr == 1)
		setinp(i_DPM);
	else
		clrinp(i_DPM);
	
	if (cons[rear] != 0)
	{
		if (sim_rdpr == 1)
			setinp(i_RPM);
		else
			clrinp(i_RPM);
	}

	if (cons[Australia] == 1)
	{
		if (rdinp(i_DLT) == 1)
			setinp(i_TDC);
		else
			clrinp(i_TDC);

		if (rdinp(i_DLB) == 1)
			setinp(i_BDC);
		else
			clrinp(i_BDC);

		if (rdinp(i_DLM) == 1)
			setinp(i_MDC);
		else
			clrinp(i_MDC);

		if (cons[rear] != 0)
		{
			if (rdinp(i_RLM) == 1)
				setinp(i_MDCR);
			else
				clrinp(i_MDCR);
		}
	}
	else if ((cons[mand] != 0) || ((cons[frghtd] != 0) && (cons[frghtd] <= 12)))
	{
		if (sim_dc == 1)
		{
			setinp(i_TDC);
			setinp(i_MDC);
			setinp(i_BDC);
			if (cons[rear] != 0)
				setinp(i_MDCR);
		}
		else
		{
			clrinp(i_TDC);
			clrinp(i_MDC);
			clrinp(i_BDC);
			if (cons[rear] != 0)
				clrinp(i_MDCR);
		}

	}

	clrinp(i_SE);		// Safety Edge
	clrinp(i_EE);		// Electric Eye
	clrinp(i_DET);		// Electric Eye
#endif


#if (Sim_Control == 1)

 
	if ((rdoutp(o_FST) == 1) || (rdoutp(o_FSTP) == 1))
		setinp(i_FST);
	else
		clrinp(i_FST);
		
	if (rdoutp(o_STE) == 1)
		clrinp(i_CFLT);
	else
		setinp(i_CFLT);

	// Set controller to automatic
	setinp(i_GOV);	// Gov not tripped

	if ((cen_on == 1) && (rdinp(i_MRSW) == 1))
		setinp(i_CEN);
	else
		clrinp(i_CEN);

	setinp(i_AUTO);	// Car on automatic
	clrinp(i_CTIN);	// Car on automatic
	clrinp(i_ICI);	// Car on automatic
	clrinp(i_MRIN);	// Car on automatic
	clrinp(i_ACC);	// Car on automatic
	setinp(i_S10);		// Controller Power made
	setinp(i_L120); // Local controller power
	setginp(i_HC);		// Hall Call power

	setinp(i_AD);		// Automatic Doors
	clrinp(i_IND);	// Not on Independent
	if (cons[equake] != 0)
		clrinp(i_ETH);	// Not on earthquake

	// No Fire Service
	if ((cons[FI_EP_Ext] == 0) && (cons[Australia] == 0))
	{
		setinp(i_FEP);		// Fire/Emergency i/o Power
		clrinp(i_FS);		// Not on Hall Key Switch Fire Return
		clrinp(i_FSX);	// Not on Hall Key Switch Fire Aux Return
		setinp(i_BP);		// On Fire Bypass
		setinp(i_ALT);	// Not on Alternate Fire Return
		setinp(i_MES);	// Not on Main Egress Fire Return
		if (cons[fire_smoke_nc] == 1)
		{
			setinp(i_MRS);	// Motor room fire sensor input
			setinp(i_HWS);	// Hoistway fire sensor input
			setinp(i_HWS2);	// Hoistway fire sensor input
		}
		else
		{
			clrinp(i_MRS);	// Motor room fire sensor input
			clrinp(i_HWS);	// Hoistway fire sensor input
			clrinp(i_HWS2);	// Hoistway fire sensor input
		}
		clrinp(i_EMP);	// Not on Emergency Power
		clrinp(i_EPS);	// No EP manual selection
		clrinp(i_EPT);	// Not on Emergency Power Transfer
	}
	else
	{
		setinp(i_FEP);		// Fire/Emergency i/o Power
		clrinp(i_FS);		// Not on Hall Key Switch Fire Return
		clrinp(i_FSX);	// Not on Hall Key Switch Fire Aux Return
		setinp(i_BP);		// On Fire Bypass
		setinp(i_ALT);	// Not on Alternate Fire Return
		setinp(i_MES);	// Not on Main Egress Fire Return
		if (cons[fire_smoke_nc] == 1)
		{
			setinp(i_MRS);	// Motor room fire sensor input
			setinp(i_HWS);	// Hoistway fire sensor input
			setinp(i_HWS2);	// Hoistway fire sensor input
		}
		else
		{
			clrinp(i_MRS);	// Motor room fire sensor input
			clrinp(i_HWS);	// Hoistway fire sensor input
			clrinp(i_HWS2);	// Hoistway fire sensor input
		}
		clrinp(i_EMP);	// Not on Emergency Power
		clrinp(i_EPS);	// No EP manual selection
		clrinp(i_EPT);	// Not on Emergency Power Transfer
	}

	setinp(i_CSS);		// Safety string made
	setinp(i_CTS);	// No SAFETY STRING OPEN
	setinp(i_FFS);	// No Stop switch open
	setinp(i_RLS);
	setinp(i_MRSW);
	setinp(i_CST);	// NO in car stop switch
	setinp(i_TF);
	setinp(i_BF);
	setinp(i_HSS);		// Hoistway Safeties not tripped
		
	// No gate and lock bypass
	clrinp(i_GBP);	// No gate bypass
	clrinp(i_LBP);	// NO lock bypass

	clrinp(i_TPL);	// Temperature Low input
#endif
	
		// Placed here for the show
	
	rdinp(i_DZA);
#if (Sim_Motion == 1)
	sim_motion();
#else
	sim_motion_dpp_count();
	sim_DPP_Count = DPP_Count;
#endif
	sim_hoistway();
#if (Sim_Door == 1)
	sim_door();
#endif


	for (i=0;i<nmb_io_arrays;i++)
	{
		iodata[cons[carnmb]][i] = (iodata[cons[carnmb]][i] & ioclr[i]) | ioset[i];		// sim io is set to complement the io bit
	}

	for (i=0;i<c_max_ctcan_ser_array;i++)
	{
		serial_ctc.brd_io[i] = (serial_ctc.brd_io[i] & ctc_ioclr[i]) | ctc_ioset[i];
	}
	for (i=0;i<c_max_mrcan_ser_array;i++)
	{
		serial_mrc.brd_io[i] = (serial_mrc.brd_io[i] & mrc_ioclr[i]) | mrc_ioset[i];
	}
	for (i=0;i<c_max_grcan_ser_array;i++)
	{
		serial_grc.brd_io[i] = (serial_grc.brd_io[i] & grc_ioclr[i]) | grc_ioset[i];
	}
}


/* Revision History
  
  7/05/01  1.06  mhd	1. Turn on DEL input if only MST is on (delete OR with MS).
  8/27/91  1.10 mhd		1. Added rear door simulation
						2. Zero sim_vel_timer if car not running.
  9/12/01  1.10 mhd		3. Add iosim to complement io data through user interface
  9/13/01	1.10 mhd	4. If hydro or gen, use MST for DEL else use RUN to set DEL.
  10/18/01 1.10 mhd     5. Replace cons[hydro] with Drive_Type[carnmb].
					    6. Changed Array sizes in group to six cars.
						7. Changed eighth_inch to s_dead_zone and shrunk the dead zone to 1/32 inch.
  03/12/02 1.14 mhd		1. Set EPS to zero so simiulator works properly to test EP manual selection.
  4/18/02 2.01 mhd		2. Reduced the door open and close times by 40%.
  05/24/02 2.02 mhd		1. Added door open and close limit test.
  6/13/02 2.02 mhd		2. Clear ICR input so IOS command can set inconspicious riser.
						3. Corrected simulator distance calculation from velocity in sim_motion routine.
  06/25/02 2.03 mhd		1. Changed Simulator command to set i/o see ioset[] and ioclr[].
						2. Start simulator from bottom floor + 1.
  07/9/02 3.00 mhd		1. When using iodata array, use labels instead of numbers such as DNRD instead of 7.
  10/21/02 3.01 mhd		1. For Tract non-distance feedback controllers, use UPF and DF as speed command signals B1:B0.
  1/3/02 3.02 mhd		1. Changed door_timer check for > door_rate instead or sim_door_timer. (front and rear)
  6/04/03 3.04 mhd		1. Output floor zone data (ul,dl, bp1 etc) if valid floor.	 
  8/14/03 3.06 mhd		1. Added serial i/o set and clear commands from the keyboard.
  3/3/04 4.00 mhd		1. Deleted [io_sel] array acces to i/o's.
  12/6/04 4.03 mhd		1. Added sim_dl and sim_gs for access operation
  1/21/05 4.06 mhd		1. Modified simulator for short floor operation ndf and hydro.
  1/5/06 4.45 mhd		1. Correct bug when using swing door cons[mand] == 2 (to open door).
  1/16/06 4.48 mhd		1. Added FFS input.
2/24/06 v4.56 mhd		1. Simulate P input from MST.
8/31/06 v5.04 mhd		1. Seperated operation for manual_rdoor.
11/6/06 v5.08 mhd		1. Added UT6/DT6, UT5/DT5 and UT4/DT4 slowdown limit simulation.
1/8/07 v5.11 mhd		1. Changed all double data types to float for the djgpp compiler.
6/15/07 v5.27 mhd		1. Add code for very short floor (see cons[vshort_tfl]).
						2. Seperated simulator floor positions from car's floor position table.
						3. Replace BDC with BDC[bdc_io_sel] to move BDC from EMH to EQ if Mass. Med Em service used.
12/28/07 v5.39.9 mhd	1. Added short floors at 29 inches.
3/13/08 v5.42.1 mhd		1. Added mid short floor and allow short floor slowdowns to overlap the other floor's door zone.
7/7/08 v5.45 mhd		1. Use parameters fvacctopfl and fvaccbotfl for access simulation.
8/19/08 v5.45.6 mhd		1. Initialize SEL_Bottom_Count and SEL_Top_Count if tapeless selector.
6/1/11 v5.54.03 mhd		1. Added special code and I/O's for Australila.
4/18/12 v6.0.06 mhd		1. Added emergency brake input for cons[EM_BRK] 1, 4 and 5 when cons[EM_BKS] != 0.
5/1/12 v6.0.7 mhd		1. Added new brake control cons[EM_BRK] == 7.  This is to have a controled drop of the emergency brake even after MC drops.
11/12/13 v7.0.28 mhd	1. Replaced iodata[1] with iodata[cons[carnmb]] where necessary.
3/2/15 v7.1.27 mhd		1. Added Up and Down to spb, nts and pal velocity display on trace screen.
6/24/15 V7.1.35 mhd		1. Correct the pal_status direction bit.
8/18/15 V7.1.40 MHD		1. Added GIS and GIC commands to simulate group IOs.
12/22/15 v7.1.55 mhd	1. Clear sfc and pfc if drive ready is not on.
*/
