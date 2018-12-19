//  All Traction Files 1.16 10/15/98

#define d_TRACTION 1
#include "global.h"

#if (Traction == 1)
// distance feed back variables

float Deceleration;            	// Deceleration rate in Pulses per Sec Squared
float Deceleration_SF;         	// Short Floor Deceleration rate in Pulses per Sec Squared
float Deceleration_EP;         	// Emergency Power Deceleration rate in Pulses per Sec Squared
float Deceleration_Ins;         // Inspection Deceleration rate in Pulses per Sec Squared
float Em_Decel; 	            // Emergency Deceleration rate in Pulses per Sec Squared
float Em_Lev_Decel;	            // Emergency Deceleration to Leveling speed rate in Pulses per Sec Squared
int32 DPP_Target;               // Target's Digital Position Pulse Count
int32 Enc_Base_Count;           // Encoder Base Count
int32 DPP_Count;                // Digital Position Current Count in Pulses
int32 old_dpp;					// Old Digital Position Count
int32 DPP_DN;                   // Down Normal's Digital Position
int32 DPP_DT[7];                // Down Terminal's Digital Positions
int32 DPP_Floor_Pos[fl_size];   // Digital position for each floor
int32 DPP_Fl_Offset[fl_size];	// Floor dead level offset
int32 Up_fl_level_dist;	  		// Up floor Level distance from dead level
int32 Dn_fl_level_dist;	 		// Down floor Level distance from dead level
int32 DZU_Dist;					// Door zone Up distance moving up
int32 DZD_Dist;					// Door zone Down distance moving down
int32 mid_hoistway;				// calculated middle of hoistway in pulse counts
int32 DPP_UN;                   // UP Normal's Digital Position
int32 DPP_UT[7];                // Up Terminal's Digital Positions
int16 DT_Open[7];                 // Keeps track of the down terminal slowdowns
int16 DT_Speed[7];				// Keeps track of the down terminal speed at opening
int16 UT_Open[7];                 // Keeps track of the up terminal slowdowns
int16 UT_Speed[7];				// Keeps track of the up terminal speed at opening
int32 DPP_DTS;					// Down Emergency Terminal Slowdown Digital Position
int32 DPP_UTS;					// Up Emergency Terminal Slowdown Digital Position
int16 DTS_Open;           	    // Keeps track of the down ETS terminal slowdowns
int16 DTS_Speed;					// Keeps track of the down ETS terminal speed at opening
int16 UTS_Open;   	            // Keeps track of the up ETS terminal slowdowns
int16 UTS_Speed;					// Keeps track of the up ETS terminal speed at opening
int32 Dist_Mode5;               // Distance Traveled In Mode 5 in Pulses
int32 Dist_Mode6;               // Distance Traveled In Mode 6 in Pulses
int32 Dist_To_Stop;             // Instantanious Distance To Stop
float End_M4_Velocity;         	// Velocity at the end of Mode 4
int32 End_M4_Dist_Tar;          // Distance to the target at the end of mode 4
float End_M6_Velocity;         	// Velocity at the end of Mode 6
int32 End_M6_Dist_Tar;          // Distance to the target at the end of mode 6
float Level_Dist;              	// Distance from the floor start mode 7 in inches
float Level_Dist_SF;           	// Distance from the SHORT floor start mode 7 in inches
float Level_Dist_EP;           	// Distance from the floor start mode 7 in inches for Emergency Power 
float Mode5_Mul_Factor;        	// Multiplication Factor to Make Curve Parabolic
float Pulses_per_Inch;         	// How many digital pulses in an inch
float Time_In_Mode3;           	// Time Spent in Mode 3
float Time_In_Mode5;           	// Time Spent in Mode 5
float Time_In_Mode3_SF;        	// Time Spent in Mode 3 Short floor run
float Time_In_Mode5_SF;        	// Time Spent in Mode 5 Short floor run
float Time_In_Mode3_EP;        	// Time Spent in Mode 3 Emergency Power run
float Time_In_Mode5_EP;        	// Time Spent in Mode 5 Emergency Power run
float Velocity;                	// Velocity in Pulses per Second
float Dmd_Velocity;			   	// Demand velocity to drive and DAC output.
float Vel_Dec_M7;              	// Velcoity Decrement per pulse in mode 7
float Vel_Level;               	// Leveling Velocity
float Vel_Max;                 	// Velocity Maximum in pulses per second
float Vel_Max_Set;             	// Velocity Maximum set point for this run
float Recovery_Vel;				// Recovery Velociy in pulses per second
float Ep_Recov_Vel;				// Emergency Power Recovery Velociy in pulses per second
float Ep_Top_Speed;				// Emergency Power Top Speed in pulses per second
float Top_Speed;				// Top Speed in pulses per second
float Vel_Mode3_Inc;           	// The Constant Velocity Increase of Mode 3
float Vel_Mode5_Dec;           	// The Constant Velocity Decrement of Mode 5
float Const_Dist_Mode5;        	// Constant Used In Calculating Distance of Mode 5
float Vel_Mode3_Inc_SF;        	// The Constant Velocity Increase of Mode 3 short floor run
float Vel_Mode5_Dec_SF;        	// The Constant Velocity Decrement of Mode 5 short floor run
float Const_Dist_Mode5_SF;     	// Constant Used In Calculating Distance of Mode 5 short floor run
float Vel_Mode3_Inc_EP;        	// The Constant Velocity Increase of Mode 3 Emergency Power run
float Vel_Mode5_Dec_EP;        	// The Constant Velocity Decrement of Mode 5 Emergency Power run
float Const_Dist_Mode5_EP;     	// Constant Used In Calculating Distance of Mode 5 Emergency Power run
float Vel_Peak;                	// Peak velocity (pulses/sec) if slowdown started now
float Vel_Relevel;             	// Releveling speed in pulses/sec
float Vel_Relev_St;            	// Releveling start speed in pulses/sec
float Vel_Step;                	// Veloctiy Step Added To Velocity Every 1/100th sec
float Vel_Step_Const;          	// Mode 2 Added to Veloctiy Every 1/100th sec
float Vel_Step_Dec;            	// Velocity Step Decrement Used in Mode 3
								// Subtracted from Velocity Step Every 1/100th sec
float Vel_Step_Inc;            	// Velocity Step Increment Used in Mode 1
								// Added to Velocity Step Every 1/100th sec
float Vel_Step_Const_SF;       	// Short Floor Mode 2 Added to Veloctiy Every 1/100th sec
float Vel_Step_Dec_SF;         	// Short Floor Velocity Step Decrement Used in Mode 3
								// Subtracted from Velocity Step Every 1/100th sec
float Vel_Step_Inc_SF;         	// Short Floor Velocity Step Increment Used in Mode 1
								// Added to Velocity Step Every 1/100th sec
float Vel_Step_Const_EP;       	// Emergency Power Mode 2 Added to Veloctiy Every 1/100th sec
float Vel_Step_Dec_EP;         	// Emergency Power Velocity Step Decrement Used in Mode 3
								// Subtracted from Velocity Step Every 1/100th sec
float Vel_Step_Inc_EP;         	// Emergency Power Velocity Step Increment Used in Mode 1
								// Added to Velocity Step Every 1/100th sec
float Vel_Step_Limit;			// Limits the change in velocity
float Vel_Step_FS;				// Limits the change in velocity at final stop
float Vel_Step_Clamp;			// Limits the change in velocity when clammping
float Vel_Step_Ins;            	// Velocity Step for Inspection Decel
int16 Speed_Clamp_Mode = 0;		// Used to setup the speed clamps

float Enc_vel=0;				// Car velocity calculated from encoder pulses (pulses per sec)
int16 enc_dbn_interval = 1;		// Encoder debounce interval
int16 enc_nmb_samples = 2;		// Encoder number of samples
int16 enc_vel_fpm = 0;			// Encoder velocity in fpm 
int16 Enc_dir = 0;				// Car encoder direction 
int16 Prev_Enc_dir = 0;			// Previous Car encoder direction 
int16 t_Enc_dir = 0;			// Temporary Car encoder direction 
int16 drv_vel_fpm = 0;			// Drive velocity in fpm
int16 dmd_vel_fpm = 0;			// Demand velocity in fpm
int16 vel_fpm = 0;				// Calculated velocity in fpm

int32 Prev_DPP_Count=0;			// Previous DPP count for calculating actual car velocity
int32 vel_DPP[10];				// Car velocity DPP array
int32 tvel_DPP;
int16 vel_DPP_ptr;				// Car velocity Dpp array pointer
int32 prev_Enc_Count = 0;		// used to detect encoder error
int16 estop_recovery = 0;		// Set to 1 after an estop to run the car at recovery speed in emergency mode after constant velocity reached
int16 Clear_Vel_Array = 0;		// Variable used to reset array of DPP in the event that the PPR changes. This avoids a fake high speed in transition
int16 encoder_save = 0;			// Flag to save encoder parameters

	// Variables for dpp count update

int16 actual_pos = 1;				// Car has to keep tract of the actual position to update the dpp_count
int16 dz_latch = 0;				// Lacth to detect first hit on door zone
int32 max_error_count = 0;		// max error count value when to update dpp_count
int32 vel_error_count = 0;		// error count caused by 10msec delay to update the position
int32 DPP_Count_DZ_Int = 0;		// Dpp count from dz interrupt
int32 DPP_Count_at_DZ[fl_size];	// Debug table to view dpp_count at DZ (update point)
int32 DPP_Vel_ERR_Count[fl_size];// Velocity error count at each update position
char DPP_updf[fl_size];			// Flag that floor count was updated
int16 vel_at_upd[fl_size];		// Velocity at count update
int16 procf_at_upd[fl_size];	// Process flag (Motion mode) at count update
int32 ul_dl_offset = 0;			// dpp offset at ul or dl
int16 up_level = 0;				// up level logical input from selector or pulse count
int16 dn_level = 0;				// down level logical input from selector or pulse count
int16 leveling = 0;				// Leveling to the floor using up_level and dn_level
int16 lev_latch = 0;			// latch leveling offset
int16 level_stop_fault = 0;		// Leveling stop error from incorrect count
int16 relev_stop_count = 0;		// number of relevel stop fault tries
float fs_lev_vel = 0;			// final stop leveling velocity
float fs_enc_vel = 0;			// final stop encoder velocity
int32 fs_dpp_count = 0;			// final stop dpp count
int32 fs_offset = 0;			// final stop count offset
int32 dead_zone = 0;			// Dead zone
int32 eight_inches = 0;			// Eight inch zone
int32 six_inches = 0;			// Six inch zone
int32 three_inches = 0;			// Three inches from dead level
int32 two_inches = 0;			// Two inches from dead level

	// Variables for selector position count
int32 SEL_Count = 0;			// Selector position count
int32 SEL_Offset = 0;			// Selector Offset count
int32 SEL_Preset_Count;			// Selector position preset count
int32 SEL_Top_Count;			// Selector top position count
int32 SEL_Bottom_Count;			// Selector bottom positioin count
int16 SEL_Count_Valid = 0;		// Selector count valid
int16 sel_init_cnt = 0;			// Selector init count

float ETS_Dist = 10.0;

int32 M7_Dist_to_Target;
int32 prev_M7_Dist_to_Target;
float dist_10_msec;
float Dist_Feed_Forward;

#endif



#if(Traction == 1)
void DAC_Out(void);
void limit_velocity(void);
void Set_Short_Fl_Profile(void);
void Mode_1(void);
void Mode_2(void);
void Mode_3(void);
void Mode_4(void);
void Mode_5(void);
void Mode_6(void);
void Mode_7(void);
void Mode_8(void);
int16 Read_ADAC(void);
void update_dpp (void);
void Set_EP_Fl_Profile(void);


int16 Roll_early_exit = 0;
int32 dpp_error = 0;


// Actual speed profile parameters used during run
// These parameters are changes for normal or short floor run

float Lev_Dist_pf = 0;
float Vel_M3_Inc_pf = 0;
float Vel_M5_Dec_pf = 0;
float Time_In_M3_pf = 0;
float Time_In_M5_pf = 0;
float Const_Dist_M5_pf = 0;
float Decel_pf = 0;
float Vel_Step_Const_pf = 0;
float Vel_Step_Inc_pf = 0;
float Vel_Step_Dec_pf = 0;
float Em_Mode_8_Vel = 0;

//****************************************************
// This is Set The Digital to Analog Converter Value
//****************************************************
void DAC_Out()
{
/*
    int32 Dac_NMB;
    if(cons[bipolar] == 1)
    {
		if(dirf == 1)           // going up
			Dac_NMB = (32767 + ((int32)((Dmd_Velocity / Vel_Max) * 32767.0)));
		else
			Dac_NMB = (32767 - ((int32)((Dmd_Velocity / Vel_Max) * 32767.0)));
    }
    else
		Dac_NMB = ((int32)((Dmd_Velocity / Vel_Max) * 65535.0));
    if(Dac_NMB > 65535L)
		Dac_NMB = 65535L;
    if(Dac_NMB < 0)
		Dac_NMB = 0;

   	//Send Out Low Byte of Dac Number		
    outp(BRDSPD2A,(uint8)(Dac_NMB & 0xff));
	
    // Send Out HIGH Byte of Dac Number
    outp(BRDSPD2B,(uint8)((Dac_NMB & 0xff00) >> 8));

	// Send Out Load DAC Comand
    outp(BRDSPD2C,(uint8)(Dac_NMB));

	Dac_NMB = (int)(((float)(Read_ADAC())/4095.0) * 65535.0);
   	//Send Out Low Byte of Dac Number		
    outp(BRDSPD1A,(uint8)(Dac_NMB & 0xff));
	
    // Send Out HIGH Byte of Dac Number
    outp(BRDSPD1B,(uint8)((Dac_NMB & 0xff00) >> 8));

	// Send Out Load DAC Comand
    outp(BRDSPD1C,(uint8)(Dac_NMB));
*/
}

// **************************************************************
// **************************************************************
// Limit Velocity At slowdowns, inspection and leveling
// This is the digital speed clamp
// **************************************************************
// **************************************************************

#if (Traction == 1)

void limit_velocity(void)
{
	float clamp_pps; 
	bool clamp_spd = 0; 

	Dmd_Velocity = Velocity;
	
	if ( ((Buffer_Test == 0) && (NTS_Test == 0) && (ETS_Test == 0)) || 
				((NTS_Test == 1) && (Drive_Type[cons[carnmb]] == 12) && (fvars[fvkebdpytype] == 0)) || 
				((NTS_Test == 1) && (Drive_Type[cons[carnmb]] != 12) && (fvars[fvkebdpytype] == 1)) || 
				(rdinp(i_AD) == 1) )           // dont check fault when testing limits
    {
	    	
		if ( (rdoutp(o_LE) == 1) && (hsf == 0) && ((rdinp(i_UL) == 1) || (rdinp(i_DZ) == 1) || (rdinp(i_DL) == 1)) )
		{		// leveling or inspection speed
			clamp_pps = ((float)(speed_clamp[0]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
		if (((dirf == 1) && (rdinp(i_UT) == 0)) ||
			((dirf == 2) && (rdinp(i_DT) == 0)))
		{
			clamp_pps = ((float)(speed_clamp[1]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
		if (((cons[speed] > 200) || (cons[rsbuffer] == 1)) && (((dirf == 1) && (rdinp(i_UTS) == 0)) ||
			((dirf == 2) && (rdinp(i_DTS) == 0))))
		{
			clamp_pps = ((float)(speed_clamp[5]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
		if ((cons[speed] > 250) && (((dirf == 1) && (rdinp(i_UT1) == 0)) ||
			((dirf == 2) && (rdinp(i_DT1) == 0))))
		{
			clamp_pps = ((float)(speed_clamp[2]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
		if ((cons[speed] > 500) && (((dirf == 1) && (rdinp(i_UT2) == 0)) ||
			((dirf == 2) && (rdinp(i_DT2) == 0))))
		{
			clamp_pps = ((float)(speed_clamp[3]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
		if ((cons[speed] > 700) && (((dirf == 1) && (rdinp(i_UT3) == 0)) ||
			((dirf == 2) && (rdinp(i_DT3) == 0))))
		{
			clamp_pps = ((float)(speed_clamp[4]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
		if ((cons[speed] > 900) && (((dirf == 1) && (rdinp(i_UT4) == 0)) ||
			((dirf == 2) && (rdinp(i_DT4) == 0))))
		{
			clamp_pps = ((float)(speed_clamp[6]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
		if ((cons[speed] > 1100) && (((dirf == 1) && (rdinp(i_UT5) == 0)) ||
			((dirf == 2) && (rdinp(i_DT5) == 0))))
		{
			clamp_pps = ((float)(speed_clamp[7]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
		if ((cons[speed] > 1300) && (((dirf == 1) && (rdinp(i_UT6) == 0)) ||
			((dirf == 2) && (rdinp(i_DT6) == 0))))
		{
			clamp_pps = ((float)(speed_clamp[8]) * (Vel_Max))/(float)cons[speed]; 
			if (Dmd_Velocity > clamp_pps)
			{
				clamp_spd = 1;
				Dmd_Velocity = clamp_pps;
			}
		}
    }
    
    if (clamp_spd == 1)
    	statusf2 |= sf_Term_Spd;
    else
    	statusf2 &= ~sf_Term_Spd;
	write_trace(TR_LIMIT_VELOCITY);
}
#endif

#if (Traction == 1)
void update_dpp (void)
{
	if ((fvars[fvppupd] & 0x02) == 0)
	{
		if ((rdinp(i_DZ) == 1) && (dz_latch == 0))
			dz_latch = 1;
	}

	if ((hsf == 1) && (rset == 0))
	{
		max_error_count = (int32)((Pulses_per_Inch * (float)(fvars[fvpperror]))/100.0);
		vel_error_count = (int32)(Enc_vel/fvars[fvppdly]);  // number of pulses 
		if ((rdinp(i_DZ) == 1) && (dz_latch == 1))
		{
			dz_latch = 2;
			if ((dirf == 1) && (rdinp(i_UL)== 1))
			{
   				if ((fvars[fvppupd] & 0x02) == 0)
 					Read_Encoder();
				actual_pos ++;
				DPP_updf[actual_pos] = 1;
				vel_at_upd[actual_pos] = enc_vel_fpm;
				procf_at_upd[actual_pos] = procf;
				DPP_Vel_ERR_Count[actual_pos] = vel_error_count;
				if ((fvars[fvppupd] & 0x02) != 0)
					DPP_Count_at_DZ[actual_pos] = DPP_Count_DZ_Int;
				else
					DPP_Count_at_DZ[actual_pos] = DPP_Count;
				dpp_error = (DPP_Floor_Pos[actual_pos] - DZU_Dist) - (DPP_Count_at_DZ[actual_pos] - vel_error_count);
				if ((labs(dpp_error) > max_error_count) || (((fvars[fvppupd] & 0x04) != 0) && (procf == 7)))
				{
					if ((fvars[fvppupd] & 0x01) != 0)
					{
#if (Simulator == 1)
						DPP_Count = DPP_Count + dpp_error;
#else
						Enc_Base_Count = Enc_Base_Count + dpp_error;
#endif
						DPP_updf[actual_pos] |= 4;
					}
				}
			}
			else if ((dirf == 2) && (rdinp(i_DL)== 1))
			{
   				if ((fvars[fvppupd] & 0x02) == 0)
					Read_Encoder();
				actual_pos --;
				DPP_updf[actual_pos] = 2;
				vel_at_upd[actual_pos] = enc_vel_fpm;
				procf_at_upd[actual_pos] = procf;
				DPP_Vel_ERR_Count[actual_pos] = vel_error_count;
				if ((fvars[fvppupd] & 0x02) != 0)
					DPP_Count_at_DZ[actual_pos] = DPP_Count_DZ_Int;
				else
					DPP_Count_at_DZ[actual_pos] = DPP_Count;
				dpp_error = (DPP_Floor_Pos[actual_pos] + DZD_Dist) - (DPP_Count_at_DZ[actual_pos] + vel_error_count);
				if ((labs(dpp_error)  > max_error_count) || (((fvars[fvppupd] & 0x04) != 0) && (procf == 7)))
				{
					if ((fvars[fvppupd] & 0x01) != 0)
					{
#if (Simulator == 1)
						DPP_Count = DPP_Count + dpp_error;
#else
						Enc_Base_Count = Enc_Base_Count + dpp_error;
#endif
						DPP_updf[actual_pos] |= 4;
					}
				}
			}
		}
	}
	else
	{

		if ((rdinp(i_DZ) == 1) && (dz_latch == 1))
		{
			dz_latch = 2;
			if ((dirf == 1) && (rdinp(i_UL)== 1))
				actual_pos ++;
			else if ((dirf == 2) && (rdinp(i_DL)== 1))
				actual_pos --;
		}
	}

	if (rdinp(i_DZ) == 1)
		dz_dbn_timer = 0;
	if ((rdinp(i_DZ) == 0) && (dz_latch == 2) && (dz_dbn_timer > 10))
	{
		dz_latch = 0;
		if ((fvars[fvppupd] & 0x02) != 0)
		{
//			_disable();
//	  		outp(com[COM3][c_int_enable], (inp(com[COM3][c_int_enable]) | 0x08)); //enable recieve interupt
//			_enable();
		}
	}
	write_trace(TR_UPDATE_DPP);
}

#endif

//**********************************************
//** Set the short floor speed profile variables
//**********************************************

void Set_Short_Fl_Profile(void)
{
  	if (short_fl_run == 1)
	{			   // set on short floor run
		Vel_Step_Const_pf = Vel_Step_Const_SF;
		Vel_Step_Inc_pf = Vel_Step_Inc_SF;
		Vel_Step_Dec_pf = Vel_Step_Dec_SF;
		Vel_M3_Inc_pf = Vel_Mode3_Inc_SF;
		Vel_M5_Dec_pf = Vel_Mode5_Dec_SF;
		Time_In_M3_pf = Time_In_Mode3_SF;
		Time_In_M5_pf = Time_In_Mode5_SF;
		Const_Dist_M5_pf = Const_Dist_Mode5_SF;
		Decel_pf = Deceleration_SF;
		Lev_Dist_pf = Level_Dist_SF;
	}
  	else
	{		// Normal run
		Vel_Step_Const_pf = Vel_Step_Const;
		Vel_Step_Inc_pf = Vel_Step_Inc;
		Vel_Step_Dec_pf = Vel_Step_Dec;
		Vel_M3_Inc_pf = Vel_Mode3_Inc;
		Vel_M5_Dec_pf = Vel_Mode5_Dec;
		Time_In_M3_pf = Time_In_Mode3;
		Time_In_M5_pf = Time_In_Mode5;
		Const_Dist_M5_pf = Const_Dist_Mode5;
		Decel_pf = Deceleration;
  	  	Lev_Dist_pf = Level_Dist;
	}
}

//********************************************************
//** Set the Emergency Power floor speed profile variables
//********************************************************

void Set_EP_Fl_Profile(void)
{
	if((short_fl_run == 0)&&		//no short floor run and (emp mode or i_RSPD = 1)
		((((cons[EP_Profile] & 0x01) != 0) && (empf != no_ep)) || (((cons[EP_Profile] & 0x02) != 0) && (rdinp(i_RSPD) == 1))))
	{			   // set on Emergency Power floor run
		Vel_Step_Const_pf = Vel_Step_Const_EP;
		Vel_Step_Inc_pf = Vel_Step_Inc_EP;
		Vel_Step_Dec_pf = Vel_Step_Dec_EP;
		Vel_M3_Inc_pf = Vel_Mode3_Inc_EP;
		Vel_M5_Dec_pf = Vel_Mode5_Dec_EP;
		Time_In_M3_pf = Time_In_Mode3_EP;
		Time_In_M5_pf = Time_In_Mode5_EP;
		Const_Dist_M5_pf = Const_Dist_Mode5_EP;
		Decel_pf = Deceleration_EP;
		Lev_Dist_pf = Level_Dist_EP;
	}
  	else
	{		// Normal run
		Vel_Step_Const_pf = Vel_Step_Const;
		Vel_Step_Inc_pf = Vel_Step_Inc;
		Vel_Step_Dec_pf = Vel_Step_Dec;
		Vel_M3_Inc_pf = Vel_Mode3_Inc;
		Vel_M5_Dec_pf = Vel_Mode5_Dec;
		Time_In_M3_pf = Time_In_Mode3;
		Time_In_M5_pf = Time_In_Mode5;
		Const_Dist_M5_pf = Const_Dist_Mode5;
		Decel_pf = Deceleration;
  	  	Lev_Dist_pf = Level_Dist;
	}
}

//******************************************************************
//  Motion Mode 1 "soft start"(rounding into constant acceleration)
//******************************************************************
void Mode_1()
{
  	int16 i;
  	int32 Dist_to_Target;
  	float vel_peak;

	Set_Short_Fl_Profile();
	if (cons[EP_Profile] != 0)
	{
		Set_EP_Fl_Profile();
	}

	Roll_early_exit = 0;
  	if (fvars[fvdon] != 0) 
  	{
  	 	if ((rdinp(i_DON) == 0) && (timers[tpatdly] < 400))
     	{
     	 	don_delay = timers[tpatdly];

	 		if (don_delay == 0)
				don_delay = 1;
  	 	}
  	}
  	else
		don_delay = 0;

  	if ((timers[tpatdly] <= (fvars[fvpat_del] + don_delay +1)) || (rdinp(i_MCX) == 0) || (safe() == 0) ||
	   (rdinp(i_DCL) == 1) ||
	   ((rdinp(i_DCLR) == 1) && (cons[rear] != 0)) ||
  	   ((rdinp(i_UP) == 0) && (rdinp(i_DNR) == 0)) )
  	{
		set_pre_torque();
		Velocity = 0;
		timers[tvpat] = 0;
		Dist_To_Stop =  0;
		timers[tdppflt] = 0;
		pre_torque_time = timers[tmotion];
		return;
  	}

	adv_pre_trq_run = 0;
	torque = 0;
  	don_delay = 0;

  	if(DPP_Count >= DPP_Target)
  	  	Dist_to_Target = (DPP_Count - DPP_Target);
  	else
  	  	Dist_to_Target = (DPP_Target - DPP_Count);
  	for(i=1; i <= timers[tvpat]; i++)
  	{
  	  	if((Vel_Step < Vel_Step_Const_pf) && (Velocity < (Vel_Max_Set-Vel_M3_Inc_pf)))
  	  	{
  	  	  	Velocity += Vel_Step;
  	  	  	Vel_Step += Vel_Step_Inc_pf;
  	  	}
  	}
  	timers[tvpat] = 0;

  	vel_peak = (Velocity + Vel_M3_Inc_pf);
  	Dist_Mode5 = (int32)((vel_peak * Time_In_M5_pf) - Const_Dist_M5_pf);
  	if(Dist_Mode5 < 0)
  	  	Dist_Mode5 = 0;
  	Dist_Mode6 = (int32)(((vel_peak-Vel_M5_Dec_pf) *
			 (vel_peak-Vel_M5_Dec_pf)/Decel_pf)/2.0);
  	if(Dist_Mode6 < 0)
  	  	Dist_Mode6 = 0;

	Dist_To_Stop = ((int32)(vel_peak * Time_In_M3_pf) +
			  (int32)(Lev_Dist_pf/2.0) + Dist_Mode5 + Dist_Mode6);
  	if((Vel_Step >= Vel_Step_Const_pf)||(Dist_To_Stop >= Dist_to_Target)||
  	   (Velocity >= (Vel_Max_Set-Vel_M3_Inc_pf)))
  	{
  	  	procf = 5;
  	  	Vel_Step = Vel_Step_Const_pf;
  	  	timers[tvpat] = 0;
  	}

	if (Vel_Max_Set > Vel_M5_Dec_pf)
		Max_Lant_Time = (Vel_Max_Set - Vel_M5_Dec_pf)/Decel_pf + Time_In_M5_pf; 
	else
		Max_Lant_Time = Velocity/Decel_pf;

}
//*******************************************
//   Motion Mode 2 "Constant Acceleration"
//*******************************************
void Mode_2()
{
  	int16 i;
  	int32 Dist_to_Target;
  	float vel_peak;
  	if(DPP_Count >= DPP_Target)
  	  Dist_to_Target = (DPP_Count - DPP_Target);
  	else
  	  Dist_to_Target = (DPP_Target - DPP_Count);
  	for(i=1; i <= timers[tvpat]; i++)
  	{
  	  if(Velocity <= Vel_Max_Set)
  	    Velocity += Vel_Step_Const_pf;
  	}
  	timers[tvpat] = 0;

  	vel_peak = (Velocity + Vel_M3_Inc_pf);
  	Dist_Mode5 = (int32)((vel_peak * Time_In_M5_pf) - Const_Dist_M5_pf);
  	if(Dist_Mode5 < 0)
  	  Dist_Mode5 = 0;
  	Dist_Mode6 = (int32)(((vel_peak-Vel_M5_Dec_pf) *
			 (vel_peak-Vel_M5_Dec_pf)/Decel_pf)/2.0);
  	if(Dist_Mode6 < 0)
  	  Dist_Mode6 = 0;
	

	Dist_To_Stop = ((int32)(vel_peak * Time_In_M3_pf) +
			  (int32)(Lev_Dist_pf/2.0) + Dist_Mode5 + Dist_Mode6);

  	if((Velocity >= (Vel_Max_Set - Vel_M3_Inc_pf)) ||
  	   (Dist_To_Stop >= Dist_to_Target))
  	{
  	  procf = 6;
  	  timers[tvpat] = 0;
  	}

	if (Vel_Max_Set > Vel_M5_Dec_pf)
		Max_Lant_Time = (Vel_Max_Set - Vel_M5_Dec_pf)/Decel_pf + Time_In_M5_pf; 
	else
		Max_Lant_Time = Velocity/Decel_pf;
}
//***********************************************
//   Motion Mode 3 "Acceleration Reduction"
//***********************************************
void Mode_3()
{
  	int16 i;
  	int32 Dist_to_Target;
  	if(DPP_Count >= DPP_Target)
  		Dist_to_Target = (DPP_Count - DPP_Target);
  	else
  	  	Dist_to_Target = (DPP_Target - DPP_Count);
  	for(i=1; i <= timers[tvpat]; i++)
  	{
  	  	if(Vel_Step > 0)
  	  	{
  	  	  	Velocity += Vel_Step;
			if (Velocity > Vel_Max_Set)
				Velocity = Vel_Max_Set;
  	  	  	Vel_Step -= Vel_Step_Dec_pf;
  	  	}
  	}
  	timers[tvpat] = 0;

  	Dist_Mode5 = (int32)((Velocity * Time_In_M5_pf) - Const_Dist_M5_pf);
  	if(Dist_Mode5 < 0)
  	  	Dist_Mode5 = 0;
  	Dist_Mode6 = (int32)(((Velocity-Vel_M5_Dec_pf)*(Velocity-Vel_M5_Dec_pf) /
			  Decel_pf)/2.0);
  	if(Dist_Mode6 < 0)
  	  	Dist_Mode6 = 0;

	Dist_To_Stop = ((int32)(Lev_Dist_pf/2.0) + Dist_Mode5 + Dist_Mode6);

  	if((Vel_Step <= 0) || (Dist_To_Stop >= Dist_to_Target) ||
  	    (Velocity >= Vel_Max_Set))
  	{
  	  	timers[tvpat] = 0;
  	  	procf = 7;
  	}
	if (Vel_Max_Set > Vel_M5_Dec_pf)
		Max_Lant_Time = (Vel_Max_Set - Vel_M5_Dec_pf)/Decel_pf + Time_In_M5_pf; 
	else
		Max_Lant_Time = Velocity/Decel_pf;
}
//*****************************************
//   Motion Mode 4 "Constant Velocity"
//*****************************************
void Mode_4()
{
  	int32 Dist_to_Target;
  	if(DPP_Count >= DPP_Target)
  	  	Dist_to_Target = (DPP_Count - DPP_Target);
  	else
  	  	Dist_to_Target = (DPP_Target - DPP_Count);
  	timers[tvpat] = 0;

  	if(Dist_To_Stop >= Dist_to_Target)
  	{
  	  	procf = 8;
	  	hsf = 0;						// car started slowdown make sure hsf is zero
		Roll_early_exit = 0;
  	  	End_M4_Dist_Tar = Dist_to_Target;
  	  	End_M4_Velocity = Velocity;
  	  	Dist_Mode5=(int32)((Velocity*Time_In_M5_pf) - Const_Dist_M5_pf);
  	  	Mode5_Mul_Factor  = (Vel_M5_Dec_pf / ((float)Dist_Mode5*(float)Dist_Mode5));
  	}
	if (estop_recovery != 0)
	{
		procf = 11;
	  	hsf = 0;						// car started slowdown make sure hsf is zero
	}

	if (Velocity > Vel_M5_Dec_pf)
		Max_Lant_Time = (Velocity - Vel_M5_Dec_pf)/Decel_pf + Time_In_M5_pf; 
	else
		Max_Lant_Time = Velocity/Decel_pf;
}
//**********************************************************
//*   Motion Mode 5 "Rounding Into Constant Deceleration"
//**********************************************************
void Mode_5()
{
  	int32 Dist_to_Target;

  	if(DPP_Count >= DPP_Target)
  	  	Dist_to_Target = (DPP_Count - DPP_Target);
  	else
  	  	Dist_to_Target = (DPP_Target - DPP_Count);
  	timers[tvpat] = 0;

  	if( ((End_M4_Dist_Tar - Dist_to_Target) > Dist_Mode5) ||
  	    (Dist_to_Target <= (int32)Lev_Dist_pf) )
  	{
		if ((Dist_to_Target <= (int32)Lev_Dist_pf) && ((End_M4_Dist_Tar - Dist_to_Target) < Dist_Mode5))
			Roll_early_exit = 1;
  	  	procf = 9;
  	}
  	else
  	{
  	  	Velocity  = (End_M4_Velocity - (Mode5_Mul_Factor *
			(float)(End_M4_Dist_Tar-Dist_to_Target) *
			(float)(End_M4_Dist_Tar-Dist_to_Target)));
		if (Velocity <= 0)
		{
			procf = 11;
			Velocity = 0;
			record_fault(f_zero_vel_error);
		}
  	}
}
//******************************************
//   Motion Mode 6 "Constant Deceleration"
//******************************************
void Mode_6()
{
  	int32 Dist_to_Target;

  	if(DPP_Count >= DPP_Target)
	  	  Dist_to_Target = (DPP_Count - DPP_Target);
	  else
  	  	Dist_to_Target = (DPP_Target - DPP_Count);
  	timers[tvpat] = 0;

  	if(Dist_to_Target > (int32)(Lev_Dist_pf/2.0))
  	  	Velocity = (float)sqrt(2.0 * ((float)Dist_to_Target-(Lev_Dist_pf/2.0))*Decel_pf);
  	if(Dist_to_Target <= (int32)Lev_Dist_pf)
  	{
		if(Dist_to_Target == 0)
			Dist_to_Target = 1;
  	  	Vel_Dec_M7 = (Velocity/(float)Dist_to_Target);
  	  	End_M6_Dist_Tar = Dist_to_Target;
  	  	End_M6_Velocity = Velocity;
  	  	procf = 10;
  	  	M7_Dist_to_Target = Dist_to_Target;
  	  	prev_M7_Dist_to_Target = Dist_to_Target;
  		Dist_Feed_Forward = 0.0;
	  	hsf = 0;						// car is in slowdown make sure hsf is zero
		targetfl_time = timers[tmotion];
  	}
}
//******************************************
//    Motion Mode 7 "Targeting the Floor"
//*******************************************
void Mode_7()
{
  	int16 i;
	
	hsf = 0;						// car is targeting floor, clear hsf to make sure it can find the target.
  	if(DPP_Count >= DPP_Target)
  	  	M7_Dist_to_Target = (DPP_Count - DPP_Target);
  	else
  	  	M7_Dist_to_Target = (DPP_Target - DPP_Count);
  	
  	if ((M7_Dist_to_Target == prev_M7_Dist_to_Target) && (fvars[fvdistff] != 0))
  	{		// add incremental distance feed forward

	  	for(i=1; i <= timers[tvpat]; i++)
		{
			
	  		dist_10_msec = (float)((Velocity/100.0) * (float)fvars[fvdistff]/1000.0);
	  		if (dist_10_msec > 0.5)
	  			dist_10_msec = 0.45;
	  		
	  		Dist_Feed_Forward += dist_10_msec;
	  		if (Dist_Feed_Forward > 1.0)
	  			Dist_Feed_Forward = 0.95;
	  		
	  		if (M7_Dist_to_Target < Dist_Feed_Forward)
	  			Dist_Feed_Forward = 0.0;
	  		
		}
  		timers[tvpat] = 0;
  	}
  	else
  	{
  		dist_10_msec = 0.0;
  		Dist_Feed_Forward = 0.0;
 	 	timers[tvpat] = 0;
  	}
  	
  	prev_M7_Dist_to_Target = M7_Dist_to_Target;
  	
  	if(Velocity > Vel_Level)
  	    Velocity = (End_M6_Velocity - (Vel_Dec_M7 *
					    ((float)(End_M6_Dist_Tar) - ((float)(M7_Dist_to_Target) - Dist_Feed_Forward))));
  	if(Velocity < Vel_Level)
  	    Velocity = Vel_Level;
}
//******************************************
//   Motion Mode 8 "EMERGENCY SLOWDOWN"
//******************************************
void Mode_8()
{
  	int16 i;

	if (((rdinp(i_DT) == 0) && (dirf == 2)) ||
		((rdinp(i_UT) == 0) && (dirf == 1)))
	{
		Vel_Max_Set = (float)(5.0 * Pulses_per_Inch);     // 25 feet minute
	}

	if (Recovery_Vel > Vel_Max_Set)
		Em_Mode_8_Vel = Vel_Max_Set;
	else
		Em_Mode_8_Vel = Recovery_Vel;

	if (Velocity < Vel_Level)
	{			// Minimum speed is leveling velocity
		for(i=1; i <= timers[tvpat]; i++)
		{
			Velocity += (Deceleration / 100.00);
		}
 	 	timers[tvpat] = 0;
		if (Velocity > Vel_Level)
			Velocity = Vel_Level;		// don't go faster than Vel_Level
  	}
  	else if (Velocity <= Em_Mode_8_Vel)
  	{									// Look for level zone
  		if ( ((dirf == 1) && (rdinp(i_UL) == 1)) ||
		     ((dirf == 2) && (rdinp(i_DL) == 1)) ||
  			 (((rdinp(i_UL) == 1) || (rdinp(i_DL) == 1) || (rdinp(i_DZ) == 1)) && ((rdinp(i_UT)==0) || (rdinp(i_DT)==0))) )
		{		   // on a level zone
		  	nstopf = 1;
		  	hsf = 0;
			if ((cons[ansi] < 2000) || (gripper_fault == 0))
			{
				setoutp(o_LE);
				setoutp(o_LE1);
			}
			if (Velocity > Vel_Level)
			{			  // Slow down to leveling velocity
			    for(i=1; i <= timers[tvpat]; i++)
			    {						   // use a softer decel rate to stop at the floor
				 	Velocity -= (Em_Lev_Decel / 100.00);
			    }
 			 	timers[tvpat] = 0;
				if (Velocity < Vel_Level)
					Velocity = Vel_Level;		// don't go slower than Vel_Level
			}
		}
		else if (Velocity < Em_Mode_8_Vel)
		{		   // Maintain speed of recovery velocity if not on level zone
			for(i=1; i <= timers[tvpat]; i++)
			{
				Velocity += (Deceleration / 100.00);
			}
 		 	timers[tvpat] = 0;
			if (Velocity > Em_Mode_8_Vel)
				Velocity = Em_Mode_8_Vel;	 // don't go faster than recovery fpm
		}
	 	timers[tvpat] = 0;					// added for v7.1.24 change
	}
	else 
	{		 // decel to recovery velocity
	    for(i=1; i <= timers[tvpat]; i++)
	    {
		 	Velocity -= (Em_Decel / 100.00);
	    }
 	 	timers[tvpat] = 0;
		if (Velocity < Em_Mode_8_Vel)
			Velocity = Em_Mode_8_Vel; 
	}

}
//****************************************************
//       Read the Analog to Digital Converter
//****************************************************
int16 Read_ADAC()
{
/*
	int32 ADC_In;
   	//Send Out Convert signal		
    outp(BRDSPD1,0);

   	// Read In HIGH Byte of AD 
	ADC_In = inp(BRDSPD1B);
	ADC_In = (ADC_In << 4);

	// Read in Low Nibble of AD
	ADC_In |= (inp(BRDSPD1A)& 0x0f);
	return((int16)ADC_In);
*/
	return 0;
}

#endif


/* Revision History

  5/31/01  1.03  mhd 	1. In Init_Hoistway, when calculating upper terminal limit
						   locations, use (cons[topf] - 1) instead of cons[topf] to place
						   limits just below the top floor.
  7/05/01  1.06  mhd	1. In mode 1 do not start the speed profile unless DEL input is on.
  9.13.01 1.10	mhd		1. Delay the speed profile until the UNI or DNI inputs turn on.
  10/18/01 1.10 mhd		2. Replace cons[hydro] with Drive_Type[carnmb].
					    3. Changed Array sizes in group to six cars.
  06/13/02 2.02 mhd		1. In Emergency mode (mode 8), do not allow velocity to go below leveling
						   when Lev_Mode = 2 (leveling to find the floor) otherwise do not allow velocity
						   to go above 25 fpm.  This keeps the velocity from oscillating.
  06/26/02 2.03 mhd		1. Init Hoistway to correct for bottom floor not equal to 1.
  06/04/03 3.04 mhd		1. Expect floor zone on valid floors only.  Place dummy dpp_count at floor positions of
						   of invalid floors. See Lear Hoistway.
  2/26/04 4.00 mhd		1. Moved init_hoistway and Wrt_Hoistway to utils.cpp.

12/2/04 v4.03 mhd		1. Added pattern delay on relevel.  Use DON input to qualify the start of pattern delay and brake pick delay. This 
						   function can be disabled. 
2/9/05 v4.07 mhd		1. Added pre_torque for drive with analog load weigher.
4/23/05 v4.11 mhd		1. Check that velocity is not greater than Vel_Max in mode 3.
5/8/06 v4.66 mhd		1. Turn on LE and LE1 in mode 8 insead of just LE.
8/16/06 v5.02 mhd		1. Use fvars[fvemdec] (Em_Decel) for the emergency decel rate.
11/6/06 v5.08 mhd		1. Added UT6/DT6, UT5/DT5 and UT4/DT4 to slowdown limit speed clamp and to Learn Hoistway.
						2. Use Velocity as the calculated value for the speed profile but use Dmd_Velocity as the clamped velocity
						   to the drive, display and the check velocity.  This is to correct a problem that if limit_velocity routine
						   changed Velocity (as in older versions) when the car first hits roll mode, a velocity calculation error can
						   occur causing a f_zero_vel_error, or the velocity to go to zero undetected and the car to stall, or causing
						   the velocity to decel and then jump back up to the clamp velocity after zero reached. Seperating the clamped velocity
						   from the calculated velocity fixes this problem.
12/20/06 v5.10 mhd		1. Added update_dpp routine to update the position pulse when the dz hit while moving and not in slowdown.
1/8/06 v5.11 mhd		1. In emergency mode 8, check for Lev_Mode == 2 to determine whether to check speed limit for 25 fpm or Vel_Level.
						2. Changed all double data types to float for the djgpp compiler.
5/10/07 v5.23 mhd		1. Added setup SPB when SPB used as absolute selector..
6/15/07 v5.27 mhd		1. Add Learn Hoistway code for very short floor (see cons[vshort_tfl]).
7/23/07 v5.31 mhd		1. Delete chedk for At_A_Floor when setting door zone offset in learn_hoistway routine.
11/28/07 v5.39.4 mhd	1. Use lobby_fl variable instead of fvars[fvlob] so that lobby_fl can be changed to alternate lobby fvars[fvaltlob]. 	
12/14/07 v5.39.7 mhd	1. Added run time, leveling time and preopen time variables.
12/28/07 v5.39.9 mhd	1. Added Roll_early_exit variable to detect error with short floor run.
2/20/08 v5.41 mhd		1. Added update all the time at constant velocity and also added variable to adjust max error.
3/25/08 v5.42.4 mhd		1. Added seperate speed profile variables for short floor run.
4/17/08 v5.43.4 mhd		1. Clear hsf flag when in any slowdown mode to make sure car can find the target when targeting floor.
7/17/08 v5.45.1 mhd		1. Changed Vel_Max to Vel_Max_Set in mode 3.
7/22/08 v5.45.2 mhd		1. Use ((fvars[fvppupd] & 0x02) != 0) instead of ((cons[dpp_upd] & 0x02) != 0) to enable interrupts.
2/10/09 v5.47.6 mhd		1. Security routines moved to Security.cc.
3/10/09 v5.47.10 mhd	1. Added check for (cons[sel_type] == 1) in mode 8 to set Lev_Mode = 1 to allow car to stop at floor.
						2. Seperated decel rate for Lev_Mode == 2 for a softer stop at the floor. 
3/16/09 		mhd		3. Don't update count at DZ if rset != 0 since the car is looking for the floor position.
						4. If velocity ever lower than leveling velocity in mode 8 the set it to leveling velocity.
3/18/09 v5.47.11 mhd	1. Allow car to recover at Recovery_Vel (up to 80 fpm).
						3. Added Check for Recovery_Vel to be greater that Vel_Max_Set to set emergency power recovery velocity.
						4. Added Em_Lev_Decel Emergency Deceleration to Leveling Rate to reach leveling speed.
7/30/09 v5.49.5 mhd		1. In mode 1, keep velociy zero if safe() == 0.  Used for advanced pre-torque.
8/10/09 v5.49.7 mhd		1. In limit_velocity(), check for leveling velocity to also includes having UL, DL or DZ inputs on to clamp velocity.
8/27/09	v5.49.10 mhd 	1. Added .01 second to start pattern delay check to call set_pre_torque().
12/18/09 v5.51 mhd		1. During contstant velocity, check for estop_recovery flag to permit car to stop at next landing.

6/3/10 v5.52.8 mhd		1. Max_Lant_Time was added to estimate the time the car takes to come to stop. The parameter is used to trigger lanterns
9/24/14 v7.1.10 mhd 	1. In mode 7 (targeting floor) use timers[tvpat] to qualify when to make distance feed forward adjustment.
						2. Place timers[tvpat] immediate after for loop to limit the chance that tvap is cleared immediately after it is incremented
						   without being tested.
11/25/14 v7.1.21 mhd	1. Set statusf2 flag if velocity is clampped.
1/13/15 v7.1.24 mhd		1. In Mode 8, must clear timers[tvpat] while running at Em_Mode_8_Vel.
8/20/15 v7.1.40 mhd		1. Use ((NTS_Test == 1) && (Drive_Type[cons[carnmb]] != 12) && (fvars[fvkebdpytype] == 1)) to allow main CPU to do NTS Test.
*/


