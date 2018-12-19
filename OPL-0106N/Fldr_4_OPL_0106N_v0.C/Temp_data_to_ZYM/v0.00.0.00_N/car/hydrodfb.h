extern int32 SD_Count;               	// Slowdown Position Pulse Count
extern int32 Enc_Base_Count;           	// Encoder Base Count
extern int32 DPP_Count;                	// Digital Position Current Count in Pulses
extern int32 old_dpp;					// Old Digital Position Count
extern int32 DPP_DN;                   	// Down Normal's Digital Position
extern int32 DPP_DT; 	               	// Down Terminal's Digital Positions
extern int32 DPP_Floor_Pos[fl_size];   	// Digital position for each floor
extern int16 DPP_Fl_Up_SD[fl_size];		// Floor Up Slowdown Distance
extern int16 DPP_Fl_Dn_SD[fl_size];		// Floor Dn Slowdown Distance
extern int16 DPP_SFl_Up_SD[fl_size];	// Short Floor Up Slowdown Distance
extern int16 DPP_SFl_Dn_SD[fl_size];	// Short Floor Dn Slowdown Distance
extern int32 mid_hoistway;				// calculated middle of hoistway in pulse counts
extern int16 Up_fl_level_dist;	  		// Up floor Level distance from dead level
extern int16 Dn_fl_level_dist;	 		// Down floor Level distance from dead level
extern int32 DPP_UN;                   	// UP Normal's Digital Position
extern int32 DPP_UT;                	// Up Terminal's Digital Positions

extern float Velocity;                	// Velocity in Pulses per Second
extern float Pulses_per_Inch;         	// How many digital pulses in an inch
extern int32 dead_zone;				// Dead zone
extern int32 eight_inches;			// Eight inch zone
extern int32 five_inches;			// Six inch zone
extern int32 three_inches;			// Three inches from dead level
extern int32 two_inches;			// Two inches from dead level


extern float Enc_vel;				// Car velocity calculated from encoder pulses (pulses per sec)
extern int16 enc_dbn_interval;		// Encoder debounce interval
extern int16 enc_nmb_samples;		// Encoder number of samples
extern int16 enc_vel_fpm;			// Encoder velocity in fpm 
extern int16 Enc_dir;				// Car encoder direction 
extern int16 Prev_Enc_dir;			// Previous Car encoder direction 
extern int16 t_Enc_dir;				// Temporary Car encoder direction 
extern int16 dmd_vel_fpm;			// Demand velocity in fpm
extern int16 vel_fpm;				// Calculated velocity in fpm

extern int16 up_level;				// up level logical input from selector or pulse count
extern int16 dn_level;				// down level logical input from selector or pulse count
extern int16 leveling;				// Leveling to the floor using up_level and dn_level
extern int16 door_zone;				// door zone logical input from selector or pulse count
extern int16 door_zone_aux;			// door zone auxilary logical input from selector or pulse count
extern int32 ul_dl_offset;			// dpp offset at ul or dl
extern int16 lev_latch;				// latch leveling offset
extern int16 level_stop_fault;		// Leveling stop error from incorrect count
extern int16 relev_stop_count;		// number of relevel stop fault tries


extern int32 Prev_DPP_Count;		// Previous DPP count for calculating actual car velocity
extern int32 vel_DPP[10];			// Car velocity DPP array
extern int32 tvel_DPP;
extern int16 vel_DPP_ptr;			// Car velocity Dpp array pointer
extern int32 prev_Enc_Count;		// used to detect encoder error
extern int16 new_PPR_setting;		// Variable used to reset array of DPP in the event that the PPR changes. This avoids a fake high speed in transition
extern int16 Clear_Vel_Array;		// Variable used to reset array of DPP in the event that the PPR changes. This avoids a fake high speed in transition
extern int16 encoder_save;			// Flag to save encoder parameters

	// Variables for selector position count
extern int32 SEL_Count;				// Selector position count
extern int32 SEL_Offset;			// Selector offset count
extern int32 SEL_Preset_Count;		// Selector position preset count
extern int32 SEL_Top_Count;			// Selector top position count
extern int32 SEL_Bottom_Count;		// Selector bottom positioin count
extern int16 SEL_Count_Valid;		// Selector count valid
extern int16 sel_init_cnt;			// Selector init count

