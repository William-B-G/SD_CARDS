
#if (Traction == 1)
// distance feed back variables

extern float Deceleration;            	// Deceleration rate in Pulses per Sec Squared
extern float Deceleration_SF;          	// Short Floor Deceleration rate in Pulses per Sec Squared
extern float Deceleration_EP;          	// Emergency Power Deceleration rate in Pulses per Sec Squared
extern float Deceleration_Ins;         	// Inspection Deceleration rate in Pulses per Sec Squared
extern float Em_Decel;            	   	// Emergency Deceleration rate in Pulses per Sec Squared
extern float Em_Lev_Decel;	            // Emergency Deceleration to Leveling speed rate in Pulses per Sec Squared
extern int32 DPP_Target;                // Target's Digital Position Pulse Count
extern int32 Enc_Base_Count;            // Encoder Base Count
extern int32 DPP_Count;                 // Digital Position Current Count in Pulses
extern int32 old_dpp;				   	// Old Digital Position Count
extern int32 DPP_DN;                    // Down Normal's Digital Position
extern int32 DPP_DT[7];                 // Down Terminal's Digital Positions
extern int32 DPP_Floor_Pos[fl_size];    // UP Normal's Digital Position
extern int32 DPP_Fl_Offset[fl_size];	// Floor dead level offset
extern int32 Up_fl_level_dist;	  		// Up floor Level distance from dead level
extern int32 Dn_fl_level_dist;	 		// Down floor Level distance from dead level
extern int32 DZU_Dist;	  				// Door zone Up distance moving up
extern int32 DZD_Dist;	  				// Door zone Down distance moving down
extern int32 mid_hoistway;			   	// calculated middle of hoistway in pulse counts
extern int32 DPP_UN;                    // UP Normal's Digital Position
extern int32 DPP_UT[7];                 // Up Terminal's Digital Positions
extern int16 DT_Open[7];                 	// Keeps track of the down terminal slowdowns
extern int16 DT_Speed[7];                	// Keeps track of speed terminal hit
extern int16 UT_Open[7];                 	// Keeps track of the up terminal slowdowns
extern int16 UT_Speed[7];				   	// Keeps track of speed terminal hit
extern int32 DPP_DTS;				   	// Down Emergency Terminal Slowdown Digital Position
extern int32 DPP_UTS;				   	// Up Emergency Terminal Slowdown Digital Position
extern int16 DTS_Open;           	       	// Keeps track of the down ETS terminal slowdowns
extern int16 DTS_Speed;				   	// Keeps track of the down ETS terminal speed at opening
extern int16 UTS_Open;   	               	// Keeps track of the up ETS terminal slowdowns
extern int16 UTS_Speed;				   	// Keeps track of the up ETS terminal speed at opening
extern int32 Dist_Mode5;                // Distance Traveled In Mode 5 in Pulses
extern int32 Dist_Mode6;                // Distance Traveled In Mode 6 in Pulses
extern int32 Dist_To_Stop;              // Instantanious Distance To Stop
extern float End_M4_Velocity;         	// Velocity at the end of Mode 4
extern int32 End_M4_Dist_Tar;           // Distance to the target at the end of mode 4
extern float End_M6_Velocity;         	// Velocity at the end of Mode 6
extern int32 End_M6_Dist_Tar;           // Distance to the target at the end of mode 6
extern float Level_Dist;              	// Distance from the floor start mode 7 in inches
extern float Level_Dist_SF;           	// Distance from the SHORT floor start mode 7 in inches
extern float Level_Dist_EP;           	// Distance from the floor start mode 7 in inches for Emergency Power run
extern float Mode5_Mul_Factor;        	// Multiplication Factor to Make Curve Parabolic
extern float Pulses_per_Inch;         	// How many digital pulses in an inch
extern float Time_In_Mode3;           	// Time Spent in Mode 3
extern float Time_In_Mode5;           	// Time Spent in Mode 5
extern float Time_In_Mode3_SF;        	// Time Spent in Mode 3 Short floor run
extern float Time_In_Mode5_SF;        	// Time Spent in Mode 5 Short floor run
extern float Time_In_Mode3_EP;        	// Time Spent in Mode 3 Emergency Power run
extern float Time_In_Mode5_EP;        	// Time Spent in Mode 5 Emergency Power run
extern float Velocity;                	// Velocity in Pulses per Second
extern float Dmd_Velocity;			   	// Demand velocity to drive and DAC output.
extern float Vel_Dec_M7;              	// Velcoity Decrement per pulse in mode 7
extern float Vel_Level;               	// Leveling Velocity
extern float Vel_Max;                 	// Velocity Maximum in pulses per second
extern float Vel_Max_Set;             	// Velocity Maximum set point for this run
extern float Recovery_Vel;				// Recovery Velociy in pulses per second
extern float Ep_Recov_Vel;				// Emergency Power Recovery Velociy in pulses per second
extern float Ep_Top_Speed;				// Emergency Power Top Speed in pulses per second
extern float Top_Speed;					// Top Speed in pulses per second
extern float Vel_Mode3_Inc;           	// The Constant Velocity Increase of Mode 3
extern float Vel_Mode5_Dec;           	// The Constant Velocity Decrement of Mode 5
extern float Const_Dist_Mode5;        	// Constant Used In Calculating Distance of Mode 5
extern float Vel_Mode3_Inc_SF;        	// The Constant Velocity Increase of Mode 3 short floor run
extern float Vel_Mode5_Dec_SF;        	// The Constant Velocity Decrement of Mode 5 short floor run
extern float Const_Dist_Mode5_SF;     	// Constant Used In Calculating Distance of Mode 5 short floor run
extern float Vel_Mode3_Inc_EP;        	// The Constant Velocity Increase of Mode 3 Emergency Power run
extern float Vel_Mode5_Dec_EP;        	// The Constant Velocity Decrement of Mode 5 Emergency Power run
extern float Const_Dist_Mode5_EP;     	// Constant Used In Calculating Distance of Mode 5 Emergency Power run
extern float Vel_Peak;                	// Peak velocity (pulses/sec) if slowdown started now
extern float Vel_Relevel;             	// Releveling speed in pulses/sec
extern float Vel_Relev_St;            	// Releveling start speed in pulses/sec
extern float Vel_Step;                	// Veloctiy Step Added To Velocity Every 1/100th sec
extern float Vel_Step_Const;          	// Mode 2 Added to Veloctiy Every 1/100th sec
extern float Vel_Step_Dec;            	// Velocity Step Decrement Used in Mode 3
										// Subtracted from Velocity Step Every 1/100th sec
extern float Vel_Step_Inc;            	// Velocity Step Increment Used in Mode 1
										// Added to Velocity Step Every 1/100th sec
extern float Vel_Step_Const_SF;       	// Short Floor Mode 2 Added to Veloctiy Every 1/100th sec
extern float Vel_Step_Dec_SF;         	// Short Floor Velocity Step Decrement Used in Mode 3
										// Subtracted from Velocity Step Every 1/100th sec
extern float Vel_Step_Inc_SF;         	// Short Floor Velocity Step Increment Used in Mode 1
										// Added to Velocity Step Every 1/100th sec
extern float Vel_Step_Const_EP;       	// Emergency Power Mode 2 Added to Veloctiy Every 1/100th sec
extern float Vel_Step_Dec_EP;         	// Emergency Power Velocity Step Decrement Used in Mode 3
										// Subtracted from Velocity Step Every 1/100th sec
extern float Vel_Step_Inc_EP;         	// Emergency Power Velocity Step Increment Used in Mode 1
										// Added to Velocity Step Every 1/100th sec
extern float Vel_Step_Limit;			// Limits the change in velocity
extern float Vel_Step_FS;				// Limits the change in velocity at final stop
extern float Vel_Step_Clamp;			// Limits the change in velocity when clammping
extern float Vel_Step_Ins;            	// Velocity Step for Inspection Decel
extern int16 Speed_Clamp_Mode;   			// Used to setup the speed clamps

extern float Enc_vel;				// Car velocity calculated from encoder pulses (pulses per sec)
extern int16 enc_dbn_interval;		// Encoder debounce interval
extern int16 enc_nmb_samples;		// Encoder number of samples
extern int16 enc_vel_fpm;			// Encoder velocity in fpm 
extern int16 Enc_dir;				// Car encoder direction 
extern int16 Prev_Enc_dir;			// Previous Car encoder direction 
extern int16 t_Enc_dir;				// Temporary Car encoder direction 
extern int16 drv_vel_fpm;			// Drive velocity in fpm
extern int16 dmd_vel_fpm;			// Demand velocity in fpm
extern int16 vel_fpm;				// Calculated velocity in fpm

extern int32 Prev_DPP_Count;			// Previous DPP count for calculating actual car velocity
extern int32 vel_DPP[10];				// Car velocity DPP array
extern int32 tvel_DPP;
extern int16 vel_DPP_ptr;				// Car velocity Dpp array pointer
extern int32 prev_Enc_Count;		// used to detect encoder error

extern int16 estop_recovery;				// Set to 1 after an estop to run the car at recovery speed in emergency mode after constant velocity reached
extern int16 Clear_Vel_Array;		// Variable used to reset array of DPP in the event that the PPR changes. This avoids a fake high speed in transition
extern int16 encoder_save;			// Flag to save encoder parameters


int32 dead_zone, eight_inches, six_inches, three_inches, two_inches;

	// Variables for dpp count update

extern int16 actual_pos;				// Car has to keep tract of the actual position to update the dpp_count 
extern int16 dz_latch;				// Lacth to detect first hit on door zone
extern int32 max_error_count;		// max error count value when to update dpp_count
extern int32 vel_error_count;		// error count caused by 10msec delay to update the position
extern int32 DPP_Count_DZ_Int;		// Dpp count from dz interrupt
extern int32 DPP_DZ_Count[fl_size];	// Debug table to view location of start of dz
extern int32 DPP_Count_at_DZ[fl_size];	// Debug table to view dpp_count at DZ (update point)
extern int32 DPP_Vel_ERR_Count[fl_size]; // Velocity error count at each update position
extern char DPP_updf[fl_size];		// Flag that floor count was updated
extern int16 vel_at_upd[fl_size];		// Velocity at count update
extern int16 procf_at_upd[fl_size];	// Process flag (Motion mode) at count update
extern int32 ul_dl_offset;			// dpp offset at ul or dl
extern int16 up_level;				// up level logical input from selector or pulse count
extern int16 dn_level;				// down level logical input from selector or pulse count
extern int16 leveling;				// Leveling to the floor using up_level and dn_level
extern int16 lev_latch;				// latch leveling offset
extern int16 level_stop_fault;		// Leveling stop error from incorrect count
extern int16 relev_stop_count;		// number of relevel stop fault tries

extern float fs_lev_vel;			// final stop leveling velocity
extern float fs_enc_vel;			// final stop encoder velocity
extern int32 fs_dpp_count;			// final stop dpp count
extern int32 fs_offset;				// final stop count offset


	// Variables for selector position count
extern int32 SEL_Count;				// Selector position count
extern int32 SEL_Offset;			// Selector offset count
extern int32 SEL_Preset_Count;		// Selector position preset count
extern int32 SEL_Top_Count;			// Selector top position count
extern int32 SEL_Bottom_Count;		// Selector bottom positioin count
extern int16 SEL_Count_Valid;		// Selector count valid
extern int16 sel_init_cnt;			// Selector init count

extern float ETS_Dist;

#endif

extern int Roll_early_exit;
extern int32 dpp_error;

extern void DAC_Out(void );
extern void limit_velocity(void);
extern void Set_Short_Fl_Profile(void);
extern void Mode_1(void );
extern void Mode_2(void );
extern void Mode_3(void );
extern void Mode_4(void );
extern void Mode_5(void );
extern void Mode_6(void );
extern void Mode_7(void );
extern void Mode_8(void );
extern int16 Read_ADAC(void );
extern void update_dpp (void);

