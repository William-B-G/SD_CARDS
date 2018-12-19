#define d_HYDRO 1
#include "global.h"

int32 SD_Count;                 // Slowdown Position Pulse Count
int32 Enc_Base_Count;           // Encoder Base Count
int32 DPP_Count;                // Digital Position Current Count in Pulses
int32 old_dpp;					// Old Digital Position Count
int32 DPP_DN;                   // Down Normal's Digital Position
int32 DPP_DT;	                // Down Terminal's Digital Positions
int32 DPP_Floor_Pos[fl_size];   // Digital position for each floor
int16 DPP_Fl_Up_SD[fl_size];	// Floor Up Slowdown Distance
int16 DPP_Fl_Dn_SD[fl_size];	// Floor Dn Slowdown Distance
int16 DPP_SFl_Up_SD[fl_size];	// Short Floor Up Slowdown Distance
int16 DPP_SFl_Dn_SD[fl_size];	// Short Floor Dn Slowdown Distance
int32 mid_hoistway;				// calculated middle of hoistway in pulse counts
int16 Up_fl_level_dist;	  		// Up floor Level distance from dead level
int16 Dn_fl_level_dist;	 		// Down floor Level distance from dead level
int32 DPP_UN;                   // UP Normal's Digital Position
int32 DPP_UT;	                // Up Terminal's Digital Positions

float Velocity;                	// Velocity in Pulses per Second
float Pulses_per_Inch;         	// How many digital pulses in an inch
int32 dead_zone = 0;			// Dead zone
int32 eight_inches = 0;			// Eight inch zone
int32 five_inches = 0;			// Six inch zone
int32 three_inches = 0;			// Three inches from dead level
int32 two_inches = 0;			// Two inches from dead level


float Enc_vel=0;				// Car velocity calculated from encoder pulses (pulses per sec)
int16 enc_dbn_interval = 1;		// Encoder debounce interval
int16 enc_nmb_samples = 2;		// Encoder number of samples
int16 enc_vel_fpm = 0;			// Encoder velocity in fpm 
int16 Enc_dir = 0;				// Car encoder direction 
int16 Prev_Enc_dir = 0;			// Previous Car encoder direction 
int16 t_Enc_dir = 0;			// Temporary Car encoder direction 
int16 dmd_vel_fpm = 0;			// Demand velocity in fpm
int16 vel_fpm = 0;				// Calculated velocity in fpm

int16 up_level = 0;				// up level logical input from selector or pulse count
int16 dn_level = 0;				// down level logical input from selector or pulse count
int16 leveling = 0;				// Leveling to the floor using up_level and dn_level
int16 door_zone = 0;			// door zone logical input from selector or pulse count
int16 door_zone_aux = 0;		// door zone auxilary logical input from selector or pulse count
int32 ul_dl_offset = 0;			// dpp offset at ul or dl
int16 lev_latch = 0;			// latch leveling offset
int16 level_stop_fault = 0;		// Leveling stop error from incorrect count
int16 relev_stop_count = 0;		// number of relevel stop fault tries

int32 Prev_DPP_Count=0;			// Previous DPP count for calculating actual car velocity
int32 vel_DPP[10];				// Car velocity DPP array
int32 tvel_DPP;
int16 vel_DPP_ptr;				// Car velocity Dpp array pointer
int32 prev_Enc_Count = 0;		// used to detect encoder error
int16 new_PPR_setting = 0;		// Variable used to reset array of DPP in the event that the PPR changes. This avoids a fake high speed in transition
int16 Clear_Vel_Array = 0;		// Variable used to reset array of DPP in the event that the PPR changes. This avoids a fake high speed in transition
int16 encoder_save = 0;			// Flag to save encoder parameters

	// Variables for selector position count
int32 SEL_Count = 0;			// Selector position count
int32 SEL_Offset = 0;			// Selector Offset count
int32 SEL_Preset_Count;			// Selector position preset count
int32 SEL_Top_Count;			// Selector top position count
int32 SEL_Bottom_Count;			// Selector bottom positioin count
int16 SEL_Count_Valid = 0;		// Selector count valid
int16 sel_init_cnt = 0;			// Selector init count
