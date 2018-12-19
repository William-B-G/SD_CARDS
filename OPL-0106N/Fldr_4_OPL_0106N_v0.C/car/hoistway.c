//  Elevator Automatic Learn Hoistway routines

#define d_HOISTWAY 1
#include "global.h"

int32 Hoistway_Init_Count = 5000;	// Hoistway init count

void Set_Invalid_Floors (void);
void init_limit_values(void);
void set_HW_IO_from_APS_Sel (void);
void Set_limit_ix (void);
void Set_Limit_Table (void);
void APS_Car_Hoistway_Setup(void);
void APS_Hoistway_Unmark_Floor(void);
void APS_Hoistway_Mark_Floor(void);
int16 verify_hoistway(void);
int16 verify_floor_table(void);
void zero_hoistway_table(void);
void read_hoistway_count(void);

//****************************************************************************************

// 2" for each 10 feet per minute
uint8 limit_ix = 0;
const uint16 limit_dist[5] = 
//        fpm   DT/UT,  	Table in inches
//      50fpm,   75fpm, 100fpm, 150fpm, 200fpm,  
		{   10,   	15,		20,		30,		40};

//****************************************************************************************


int16 hwl_mode = 0;
int16 hwl_mode_car = 0;
int16 hwl_mark = 0;
int16 hwl_fl_update = 0;
int16 hwl_floor = 0;
int16 hwl_floor_marked = 0;
int16 hwl_num_floors = 8;
int16 hwl_num_limits = 3;
int16 hwl_top_speed = 200;
int16 hwl_seq = 0;
int16 hwl_zero_hoistway = 0;

// *************************************************
// Verify Floor Table
// Return 0 for valid table or number for fault
// *************************************************

int16 verify_floor_table(void)
{
	int16 i;
	int32 max_offset;
	
	for(i=cons[bottomf];i<cons[topf];i++)
	{
		if (((DPP_Floor_Pos[i] > DPP_Floor_Pos[i+1]) && (valid_fl[i+1] == 1)) || ((DPP_Floor_Pos[i] == 0) && (valid_fl[i] == 1)))
		{
			fault_device = i;
			return f_hw_inv_fl_cnt;
		}
	}

	max_offset = (int32)(Pulses_per_Inch * 50.0);
	for(i=cons[bottomf];i<cons[topf];i++)
	{
		if (labs(DPP_Fl_Up_SD[i]) > max_offset)
		{
			fault_device = i;
			return f_hw_sd_cnt;
		}
		if (labs(DPP_Fl_Dn_SD[i]) > max_offset)
		{
			fault_device = i;
			return f_hw_sd_cnt;
		}
	}
	
	max_offset = (int32)(Pulses_per_Inch * 30.0);
	for(i=cons[bottomf];i<cons[topf];i++)
	{
		if (labs(DPP_SFl_Up_SD[i]) > max_offset)
		{
			fault_device = i;
			return f_hw_sd_cnt;
		}
		if (labs(DPP_SFl_Dn_SD[i]) > max_offset)
		{
			fault_device = i;
			return f_hw_sd_cnt;
		}
	}
	
	return 0;
	
}


// *************************************************
// Zero hoistway table 
// *************************************************

void zero_hoistway_table(void)
{
	int16 i;
	
  	for(i= 1; i<= cons[topf]; i++)             /* initialize floor positions */
  	{
		DPP_Floor_Pos[i] = 0;
  	}

	DPP_DN = 0;

	DPP_DT = 0;

	DPP_UT = 0;

  	DPP_UN = 0;

 	Hoistway_Learned = 0;

	Up_fl_level_dist = 0;	
					
	Dn_fl_level_dist = 0;
	
	for(i= 1; i<= cons[topf]; i++)             /* initialize floor offsets */
	{
		DPP_Fl_Up_SD[i] = 0;
		DPP_Fl_Dn_SD[i] = 0;
		DPP_SFl_Up_SD[i] = 0;
		DPP_SFl_Dn_SD[i] = 0;
	}


	mid_hoistway = 0;
	
	Wrt_Hoistway();

}


// *************************************************
// Verify the hoistway table is setup correctly
// Return 0 if correct else return error code if not
// *************************************************

int16 verify_hoistway(void)
{
	int16 i;
	int32 max_offset;
	int32 mid_dpp_count;

	for(i=cons[bottomf];i<cons[topf];i++)
	{
		if ((DPP_Floor_Pos[i] > DPP_Floor_Pos[i+1]) || (DPP_Floor_Pos[i] == 0))
		{
			fault_device = i;
			return f_hw_inv_fl_cnt;
		}
	}
	
	mid_dpp_count = ((DPP_Floor_Pos[cons[topf]] - DPP_Floor_Pos[cons[bottomf]]) >> 2) + DPP_Floor_Pos[cons[bottomf]];

	if (DPP_Floor_Pos[cons[topf]] == 0)
		return f_hw_topf_cnt;
	if ((DPP_DN > DPP_DT) || (DPP_DN == 0) || (DPP_DT == 0))
		return f_hw_dn_dt_cnt;
	
	if ((DPP_UT > DPP_UN) || (DPP_UT == 0) || (DPP_UN == 0) || (DPP_UT < mid_dpp_count) || (DPP_UN < mid_dpp_count))
		return f_hw_ut_un_cnt;

	if ((cons[sel_type] == 1) || (cons[sel_type] == 2))
	{
		if (cons[sel_type] == 1)
		{
			if (SEL_Bottom_Count < 4000)
				return f_hw_sel_bot_cnt;
		}
		else 
		{
			if (SEL_Bottom_Count < 1)
				return f_hw_sel_bot_cnt;
		}
		if ((SEL_Top_Count - SEL_Bottom_Count) < (cons[topf] * 30))
			return f_hw_sel_top_cnt;
	}

	max_offset = (int32)(Pulses_per_Inch * 2.0);
	if ((int32)Up_fl_level_dist > max_offset)
	{
		fault_device = i;
		return f_hw_fl_off_cnt;
	}
	if ((int32)Dn_fl_level_dist > max_offset)
	{
		fault_device = i;
		return f_hw_fl_off_cnt;
	}

	Set_limit_ix();

	max_offset = (int32)(Pulses_per_Inch * 50.0);
	for(i=cons[bottomf];i<=cons[topf];i++)
	{
		if (i != cons[bottomf])
		{
			if (DPP_Fl_Up_SD[i] == 0)
				DPP_Fl_Up_SD[i] = (int16)((float)limit_dist[limit_ix] * Pulses_per_Inch);
				
			if (labs(DPP_Fl_Up_SD[i]) > max_offset)
			{
				fault_device = i;
				return f_hw_sd_cnt;
			}
		}
		
		if (i != cons[topf])
		{
			if (DPP_Fl_Dn_SD[i] == 0)
				DPP_Fl_Dn_SD[i] = (int16)((float)limit_dist[limit_ix] * Pulses_per_Inch);
			
			if (labs(DPP_Fl_Dn_SD[i]) > max_offset)
			{
				fault_device = i;
				return f_hw_sd_cnt;
			}
		}
	}

	max_offset = (int32)(Pulses_per_Inch * 50.0);
	for(i=cons[bottomf];i<=cons[topf];i++)
	{
		if (i != cons[bottomf])
		{
			if (DPP_SFl_Up_SD[i] == 0)
				DPP_SFl_Up_SD[i] = (int16)(2.0 * Pulses_per_Inch);
			
			if (labs(DPP_SFl_Up_SD[i]) > max_offset)
			{
				fault_device = i;
				return f_hw_sd_cnt;
			}
		
		}
		
		if (i != cons[topf])
		{
			if (DPP_SFl_Dn_SD[i] == 0)
				DPP_SFl_Dn_SD[i] = (int16)(2.0 * Pulses_per_Inch);
			
			if (labs(DPP_SFl_Dn_SD[i]) > max_offset)
			{
				fault_device = i;
				return f_hw_sd_cnt;
			}
		}
	}

	return 0;
}


//************************************************
// Set the invalid floors counts to valid numbers
//************************************************

void Set_Invalid_Floors (void)
{
 	int16 i;
 	int16 j;
 	int16 num_invalid_fl = 0;
 	
 	for(i=cons[topf]; i > cons[bottomf]; i--)
 	{
 		if (valid_fl[i] == 0)
 		{
 			num_invalid_fl++;
 		}
 		else if ((valid_fl[i] == 1) && (num_invalid_fl != 0))
 		{
 			if ((i + num_invalid_fl) < cons[topf])
 			{
 				for(j=1;j<=num_invalid_fl;j++)
	 			{
	 				DPP_Floor_Pos[i+j] = DPP_Floor_Pos[i+(j-1)] + (DPP_Floor_Pos[(i+num_invalid_fl)+1] - DPP_Floor_Pos[i])/(num_invalid_fl + 1);
	 			}
	 			num_invalid_fl = 0;
 			}
 		}
 	}
}


//  Number of slowdown num_limits not counting UN/DN  

void Set_limit_ix (void)
{
	if (cons[speed]<= 50)
		limit_ix = 0;
	else if (cons[speed]<= 75)
		limit_ix = 1;
	else if (cons[speed]<= 100)
		limit_ix = 2;
	else if (cons[speed]<= 150)
		limit_ix = 3;
	else 
		limit_ix = 4;
}

void Set_Limit_Table (void)
{
	Set_limit_ix();
	if ((cons[topf] > 0) && (cons[speed]> 0))
	{
		if (cons[topf] < fl_size)
		{
			if (DPP_Floor_Pos[cons[topf]] > (uint32)((float)limit_dist[limit_ix] * Pulses_per_Inch))
			{
				DPP_UN = DPP_Floor_Pos[cons[topf]] + two_inches;		// 2 inches
				if (nts_fvars[nts_fvlimit_dist] != 0)
					DPP_UT = DPP_Floor_Pos[cons[topf]] - (uint32)((float)nts_fvars[nts_fvlimit_dist] * Pulses_per_Inch);
				else
					DPP_UT = DPP_Floor_Pos[cons[topf]] - (uint32)((float)limit_dist[limit_ix] * Pulses_per_Inch);
			}
		}
	}
	if ((cons[topf] > 0) && (cons[speed]> 0))
	{
		if (DPP_Floor_Pos[1] > two_inches)
		{
			DPP_DN = DPP_Floor_Pos[1] - two_inches;		// 50.8 pulses per inch
			if (nts_fvars[nts_fvlimit_dist] != 0)
				DPP_DT = DPP_Floor_Pos[1] + (uint32)((float)nts_fvars[nts_fvlimit_dist] * Pulses_per_Inch);
			else
				DPP_DT = DPP_Floor_Pos[1] + (uint32)((float)limit_dist[limit_ix] * Pulses_per_Inch);
		}
	}
}


// *******************************************************
// Read Hoistway position count
// *******************************************************

void read_hoistway_count(void)
{
	int32 count_diff;


	if (sel_can.online == 1)
	{
		DPP_Count = Enc_Base_Count + sel_can.pos_cnt_A;

	  	if (prev_Enc_Count > DPP_Count)
			count_diff = prev_Enc_Count - DPP_Count;
	  	else
			count_diff = DPP_Count - prev_Enc_Count;

		if (count_diff > (Pulses_per_Inch * 4.0))		//  Can't move 4 in 2-3 msec
		{
			record_fault(f_hw_cnt_read);
		}
		prev_Enc_Count = DPP_Count;	
	}
	
	if (dpp_store_tmr > 0)
	{
		dpp_store_tmr = 0;
		Update_DPP_EE();
	}
}

// *******************************************************
// Set the Hoistway I/O from the Cedes Selector
// *******************************************************

void set_HW_IO_from_APS_Sel (void)
{
	int16 dzu = 0;
	int16 dzd = 0;
	
	if (valid_fl[position] == 1)
	{
	
		if( (labs(DPP_Floor_Pos[position] - DPP_Count)) < (eight_inches + dead_zone) ) 
		{	   // within 8 inches -  normal ul and dl
			if (DPP_Count <= (DPP_Floor_Pos[position] + dead_zone))
				setinp(i_UL);		// On up level
			else
				clrinp(i_UL);

			if (DPP_Count >= (DPP_Floor_Pos[position] - dead_zone)) 
				setinp(i_DL);		// On down level 
			else
				clrinp(i_DL);	
		}
		else
		{
			clrinp(i_DL);	
			clrinp(i_UL);
		}


		if( (labs(DPP_Floor_Pos[position] - DPP_Count)) < five_inches ) 
		{	   // within 5 inches

			if (DPP_Count <= (DPP_Floor_Pos[position] + three_inches))
				dzu = 1;		// On up level
			else
				dzu = 0;

			if (DPP_Count >= (DPP_Floor_Pos[position] - three_inches)) 
				dzd = 1;		// On down level 
			else
				dzd = 0;	
		}
		else
		{
			dzu = 0;
			dzd = 0;	
		}
				
	}
	else
	{
		clrinp(i_DL);	
		clrinp(i_UL);
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


	if ((dzu == 1) || (dzd == 1))
		setinp(i_DZA);
	else
		clrinp(i_DZA);

	if ((dzu == 1) && (dzd == 1))
		setinp(i_DZsel);	// within 3 inches
	else		
		clrinp(i_DZsel);


	// Terminal limits


	if ((rdinp(i_TSTM) == 1) || (hwl_mode == 1) || ((rdinp(i_HWSET) == 1) && (cop_can_online == 1)))
	{
		setinp(i_DNsel);
		setinp(i_UNsel);
	}
	else
	{
		if (rdoutp(o_DNO) == 1)
		{	// Down normal orerride
			setinp(i_DNsel);
		}
		else
		{
			if (DPP_Count <= DPP_DN)
			{
				clrinp(i_DNsel);
			}
			else
			{
				setinp(i_DNsel);
			}
		}
		
		if (DPP_Count >= DPP_UN)
		{
			clrinp(i_UNsel);
		}
		else
		{
			setinp(i_UNsel);
		}
	}

	if (hwl_mode == 1)
	{
		setinp(i_DTsel);
		setinp(i_UTsel);
	}
	else
	{
		if (DPP_Count <= DPP_DT)
		{
			clrinp(i_DTsel);
		}
		else 
		{
			setinp(i_DTsel);
		}
		if (DPP_Count >= DPP_UT)
		{
			clrinp(i_UTsel);
		}
		else 
		{
			setinp(i_UTsel);
		}
	}
	
	if (rdinp(i_DL) == 1)
		setoutp(o_DL);
	else
		clroutp(o_DL);
	
	if (rdinp(i_UL) == 1)
		setoutp(o_UL);
	else
		clroutp(o_UL);
	
	if ((rdinp(i_TSTM) == 1) || (hwl_mode == 1) || ((rdinp(i_HWSET) == 1) && (cop_can_online == 1)))
	{
		setoutp(o_SELOK);
		clroutp(o_DZ);
	}
	
#if (Simulator == 1)
	else if (rdinp(i_DZsel) == 1)
	{
		setoutp(o_SELOK);
		setoutp(o_DZ);
	}
	else
	{
		setoutp(o_SELOK);
		clroutp(o_DZ);
	}
#else
	else if ((sel_can.online == 1) && (valid_fl[position] == 1) &&
		(sel_can.Error_Warning.B.INTERNAL == 0) && 
		(sel_can.Error_Warning.B.POS_INVALID == 0) &&
		(sel_can.Error_Warning.B.COMM == 0))
	{
		setoutp(o_SELOK);
		if (rdinp(i_DZsel) == 1)
			setoutp(o_DZ);
		else
			clroutp(o_DZ);
	
	}
	else
	{
		clroutp(o_SELOK);
		clroutp(o_DZ);
	}
#endif

}

void APS_Car_Hoistway_Setup(void)
{
	int16 i;
	
	switch (hwl_seq)
	{
		case 0: 
			if ((rd_both_inps(i_CC(1),i_XCC(1))==0) && (rd_both_inps(i_CC(2),i_XCC(2))==1))
			{
				hwl_seq = 1;
				timers[tsecure] = 0;
			}
			break;
		
		case 1: 
			if ((rd_both_inps(i_CC(2),i_XCC(2))==0) && (rd_both_inps(i_CC(1),i_XCC(1))==1))
			{
				hwl_seq = 2;
				timers[tsecure] = 0;
			}
			break;
		
		case 2: 
			if ((rd_both_inps(i_CC(1),i_XCC(1))==0) && (rd_both_inps(i_CC(2),i_XCC(2))==1))
			{
				hwl_seq = 3;
				timers[tsecure] = 0;
			}
			break;
		
		case 3: 
			if ((rd_both_inps(i_CC(2),i_XCC(2))==0) && (rd_both_inps(i_CC(1),i_XCC(1))==1))
			{
				hwl_seq = 4;
				timers[tsecure] = 0;
			}
			break;
		
		case 4: 
			if ((rd_both_inps(i_CC(1),i_XCC(1))==0) && (rd_both_inps(i_DCB,i_XDCB) == 1))
			{
				hwl_seq = 5;
				timers[tsecure] = 0;
				timers[tsecbep] = 0;
			}
			break;
		
		case 5:
			if (rd_both_inps(i_DCB,i_XDCB) == 0)
			{
				set_both_outps(o_CCL(1),o_XCCL(1));
				set_both_outps(o_CCL(2),o_XCCL(2));
				if (timers[tsecbep] > 10)
				{
					clr_both_outps(o_CCL(1),o_XCCL(1));
					clr_both_outps(o_CCL(2),o_XCCL(2));
					hwl_seq = 6;
					timers[tsecure] = 0;
					timers[tsecbep] = 0;
				}
			}
			break;
			
		case 6:
			if (timers[tsecbep] > 2)
			{
				hwl_seq = 7;
				timers[tsecure] = 0;
			}
			break;
			
		case 7:
			hwl_floor = 0;
		  	for(i=cons[bottomf]; i <= cons[topf]; i++) 
		  	{
				if (rd_both_inps(i_CC(i),i_XCC(i))==1)
					hwl_floor = i;
				
			}
			if (hwl_floor != 0)
			{
				hwl_num_floors = hwl_number_valid_fl();
				if ((hwl_num_floors != nts_fvars[nts_fvnum_floors]) ||
					(nts_fvars[nts_fvnum_limits] != 1) ||
					(nts_fvars[nts_fvtop_speed] != cons[speed]))
				{
					hwl_seq = 0;
					timers[tsecure] = 0;
					clr_both_outps(o_CCL(1),o_XCCL(1));
					clr_both_outps(o_CCL(2),o_XCCL(2));
					record_fault(f_NTS_setup_flt);
				}
				timers[tsecure] = 0;
				timers[tsecbep] = 0;
				hwl_mark = 1;
				hwl_fl_update = 0;
				hwl_seq = 8;
				APS_Hoistway_Mark_Floor();
			}
			break;
			
		case 8:
			set_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			if (hwl_fl_update == 1)
			{
				timers[tsecure] = 0;
				timers[tsecbep] = 0;
				hwl_seq = 9;
				APS_Hoistway_Unmark_Floor();
				if (verify_floor_table() == 0)		// non-zero value is a fault 
				{
					Set_Invalid_Floors();
					Set_Limit_Table ();
					i = verify_hoistway();
					if (i == 0)
					{
						//mid_hoistway calculation done here so we adjust the value and prevent faults.
						mid_hoistway = DPP_Floor_Pos[cons[bottomf]] + (DPP_Floor_Pos[cons[topf]] - DPP_Floor_Pos[cons[bottomf]])/2;
		  	  	  		Hoistway_Learned = 1;
					}
					else
						record_fault(i);
					
		 	  	  	Wrt_Hoistway();
				}
			}
			break;
			
		case 9:
			if ((timers[tsecbep] > 5) && (timers[tsecbep] < 10))
			{
				clr_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			}
			else if (timers[tsecbep] < 13)
			{
				set_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			}
			else if (timers[tsecbep] < 16)
			{
				clr_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			}
			else if (timers[tsecbep] < 19)
			{
				set_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			}
			else if (timers[tsecbep] < 21)
			{
				clr_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			}
			else if (timers[tsecbep] < 24)
			{
				set_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			}
			else if (timers[tsecbep] < 27)
			{
				clr_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			}
			else if (timers[tsecbep] < 30)
			{
				set_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
			}
			else if (timers[tsecbep] < 33)
			{
				clr_both_outps(o_CCL(hwl_floor),o_XCCL(hwl_floor));
				hwl_seq = 0;
			}
			break;
	}
	if ((timers[tsecure] > 100) && (hwl_seq > 0) && (hwl_seq != 9))
	{                       // end of code or timer up  or to many codes
		if (hwl_seq == 8)
			APS_Hoistway_Unmark_Floor();	// timed out after the floor was marked
		hwl_seq = 0;
		timers[tsecure] = 0;
		clr_both_outps(o_CCL(1),o_XCCL(1));
		clr_both_outps(o_CCL(2),o_XCCL(2));
	}

}


void APS_Hoistway_Mark_Floor(void)
{
	int16 i;
	int16 j = 0;
	
	if ((hwl_floor != 0) && (hwl_mark == 1))
	{
			
	  	for(i=cons[bottomf]; i <= cons[topf]; i++) 
	  	{
			if (valid_fl[i] == 1)
			{
				j++;
				if (hwl_floor == i)
					break;
			}
	  	}
	  	if ((j>= 1) && (j <= hwl_floor) && (j <= hwl_num_floors))
	  	{		// valid floor found
#if ((Simulator == 1) && (Sim_Motion == 1))
			DPP_Count = sel_can.pos_cnt_A; 
#endif
  	  	  	DPP_Floor_Pos[hwl_floor] = DPP_Count;
  	  	  	hwl_floor_marked = 1;
	  		NTS_spi.hwl_floor = (uint8)j;
	  		NTS_spi.hwl_num_floors = (uint8)hwl_num_floors;
	  		NTS_spi.hwl_num_limits = (uint8)hwl_num_limits;
	  		NTS_spi.hwl_speed = hwl_top_speed;
	  		if (NTS_spi.hwl_floor == 1)
	  		{
	  			NTS_spi.Command1.B.BOT_FL = 1;
	  			NTS_spi.Command1.B.TOP_FL = 0;
	  		}
	  		else if (NTS_spi.hwl_floor == hwl_num_floors)
	  		{
	  			NTS_spi.Command1.B.BOT_FL = 0;
	  			NTS_spi.Command1.B.TOP_FL = 1;
	  		}
	  		else
	  		{
	  			NTS_spi.Command1.B.BOT_FL = 0;
	  			NTS_spi.Command1.B.TOP_FL = 0;
	  		}

	  		NTS_spi.Command1.B.MARK_FL = (hwl_mark == 1);
	  	}
	}
}

void APS_Hoistway_Unmark_Floor(void)
{
	NTS_spi.Command1.B.MARK_FL = 0;
	NTS_spi.Command1.B.BOT_FL = 0;
	NTS_spi.Command1.B.TOP_FL = 0;
	NTS_spi.hwl_floor = 0;
	hwl_mark = 0;
   	hwl_floor_marked = 0;
}


/* Revision History

3/11/08 v5.42 mhd		1. Added softstart velocity when running.
7/22/08 v5.45.2 mhd		1. Deleted DPP_DZU_Count[], DPP_DZD_Count[] and DPP_FL_Offset[] to use DZU_Dist, DZD_Dist, Up_fl_level_dist and Dn_fl_level_dist instead.
1/6/09 v5.47 mhd		1. Use ((fvars[fvshrtflctl] & 0x01) == 0) to allow the normal door zone detection for the short floor since the UL and DL input will go off
						   before the very short floor is seen on the second selector.  This bit needs to be set to allow a short floor run instead of leveling 
						   to the short floor.
						2. Use DTR2 instead of DTR if staggard rear option is used.
1/15/09 v5.47.1 mhd		1. When making the change in v5.47, I inavertently changed the check for (At_Very_Short_Floor == 2) to == 3, so I changed it back to == 2.
						   Otherwise, short floors that switch onto the next door zone will not work (shorter than 8 inches).
1/21/09 v5.47.3 mhd		1. Modify Learn_Hoistway to measure the DZ distance and the Dead zone distance.
1/27/09 V5.47.4 mhd		1. Modified Dead zone distance calculation in Learn_Hoistway to use the turn on of the sensors only.
3/20/09 v5.47.9 pn      1. Corrected problem when doing a hoistway learn and UT4 breaks it records the dpp count
                           in the wrong array location.
3/11/09 v5.47.10 mhd	1. Added flag hw_keep_offset flag to keep the ul and dl offsets during hoistway learn.
6/3/09 v5.48.9 mhd		1. Added fault code for ul,dl setup count fault.  If rpm is incorrect the calculation for 8 inches is incorrect.
						   On UL,DL count fault, set the fault_run flag if level zone being setup.
7/18/09 v5.49.1 mhd		1. Corrected start of learn hoistway to use (rdinp(DN,DNB,DND) == 0) instead of (rdoutp(o_DNR) == 1). Also AND 
						   statement with instead of ORing it.
7/20/09 v5.49.1 mhd		1. Added error check for up and down normal limit setup.
						2. Added error check for up and down level distance less than zero or greater than 8 inches.
8/12/09 v5.49.7 mhd		1. Turn off door power during hoistway learn if cons[dcpo] = 0.	
10/27/10 v5.52.41 mhd	1. Set SEL_Count_Valid = 1 if simulator and finished hoistway learn.
						2. Added better for reaching the top floor.				
1/11/11 v5.52.56 mhd	1. Replaced setting the encoder directly with preset_encoder_count() routine.
2/17/12 v6.0.1 mhd		1. During auto hoistway learn, when the DN is reached, delay for fvars[fvsst] + 20, or the softstop time
						   plus 2 seconds so the the car does not try to run with the emergency brake still up.
9/1/12 v6.0.19 mhd		1. Preset selector position to 35000 during hoistway init.
9/26/12 v6.0.23 mhd		1. Close doors in Auto_Learn_Hoistway if doors are closed and locks or gate not made.
9/30/13 v7.0.21 mhd		1. Store NTS and SPB parameters to devices.
11/21/13 v7.0.29 mhd	1. Corrected ul dl width check from ((DPP_Count - lev_dist_count) > ((int32)(Pulses_per_Inch * 8.0))) to (Pulses_per_Inch * 8.5).
						   The 8.0 was too tight of a requirement.
						2. In hoistway_run_cmd() set timers[tpatdly] = 0 immediately after chk_start() returns a 1.
						3. In hoistway() routine, call get_load(1) with a 1 to force an update. 
						4. Run 5 fpm until off the down normal limit and until DZ hit.  Then run at 15 fpm until off UL on hw learn start with auto learn mode.
12/18/13 v7.0.31 mhd	1. Added additional checks to make sure that hoistway learn could not start if UN is not 1, That cam_mode is verifyed to be greater than the previous
						   highest mode when setting the up limit positions and the the learnn finish cannot occure if DN is not 1.
11/17/14 v7.1.20 mhd	1. Set speed_adj to 5 fpm as default.
11/25/14 v7.1.21 mhd	1. Write field variables (limit values) when limit velocities are initialized.
5/21/15 v7.1.32 mhd		1. Corrected speed_adj default.
1/5/16 v7.1.58 mhd		1. Brought in the following change from GALaxy III:
						12/18/15 v6.4.07 hl		1. Added mid_hoistway calculation as soon as we finish learning the hoistway to 
												   prevent faults from ocurring after hoistway learn.
8/6/16 v7.2.9 mhd		1. Added code for Cedes selector. See the routines: void set_HW_IO_from_APS_Sel (void); Set_limit_ix(),
							Set_Limit_Table(), APS_Car_Hoistway_Setup(), APS_Hoistway_Unmark_Floor(), and APS_Hoistway_Mark_Floor(void).
8/18/16 v7.2.14 mhd		1. Modified timing for car call input in APS_Car_Hoistway_Setup().
10/19/16 v7.2.30 mhd	1. Call APS_Hoistway_Unmark_Floor if time-out occurs after floor has been marked.
3/24/17 v8.0.2 mhd		1. Added Down Normal Override for reset jack.
*/
