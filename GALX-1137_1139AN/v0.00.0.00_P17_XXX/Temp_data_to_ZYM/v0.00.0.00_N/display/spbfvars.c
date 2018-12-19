// Safety Processor Board Adjustable Variables
#define d_SPBFVARS 1
#include "global.h"


void detect_spb_fvars_change (void);
void Adjust_SPB_Parameters (void);
void Display_SPB_IO (void);

#define numspbvars 31


	int16 spb_fvars[numspbvars];
	
	int16 spb_fvars_deci[numspbvars] = {
		0,				// 0  Top Speed
		1,				// 1  Encoder RPM
		0,				// 2  Encoder PPR
		0,				// 3  Encoder Type 0 = tape, 1 = governor encoder, 2 = absolute encoder tape, 3 = pulser from governor
		0,				// 4  Controller Type 0 = Hydro, 1 = Traction OL, 2 = Traction Distance Feedback
		0,				// 5  2 Stop 0 = Multi stop, 1 = 2 stop
		0,				// 6  Rear Doors 0 = no rear, 1 = rear doors
		0,				// 7  Terminal Slowdown Up Velocity
		0,				// 8  Terminal Slowdown Down Velocity
		0,				// 9  Inspection Velocity
		0,				// 10 Leveling Velocity
		0,				// 11 Up Terminal Velocity
		0,				// 12 Down Terminal Velocity
		0,				// 13 UT Count to set software slowdown limit 
		0,				// 14 DT Count to set software slowdown limit
		0,				// 15 Short Floor
		0,				// 16 Soft Stop Pulse timer 10 msec count
		0,				// 17 Buffer Type 0 = Normal, 1 = Reduced Stroke
		2,				// 18 Pulse count fail timer
		2,				// 19 Velocity Fault delay timer
		0,				// 20 Speed Check disable (0 = enable, 1 = disable)
		0,				// 21 Communications Check Disable, 0 = Use comm check, 1 = No comm check
		0,				// 22 Encoder Direction, 0 = Up, 1 = Down (used to change encoder direction flag)
		0,				// 23 Unitended motion check, 0=motion check, 1=Disable motion check
		0,				// 24 Pulses per foot
		0,				// 25 Can Baud Rate flag: 0=115.2K, 1=57.6K
		0,				// 26 Debug Mode 
		0,				// 27 CPLD Up Terminal Velocity
		0,				// 28 CPLD Down Terminal Velocity
		0,				// 29
		0,				// 30
	};


	int16 spb_fvars_max[numspbvars] = {
		2000,			// 0  Top Speed
		18000,			// 1  Encoder RPM
		18000,			// 2  Encoder PPR
		4,				// 3  Encoder Type 0 = tape, 1 = governor encoder, 2 = absolute encoder tape, 3 = pulser from governor, 4 = inc enc
		2,				// 4  Controller Type 0 = Hydro, 1 = Traction OL, 2 = Traction Distance Feedback
		1,				// 5  2 Stop 0 = Multi stop, 1 = 2 stop
		1,				// 6  Rear Doors 0 = no rear, 1 = rear doors
		1600,			// 7  Terminal Slowdown Up Velocity
		1600,			// 8  Terminal Slowdown Down Velocity
		200,			// 9  Inspection Velocity
		200,			// 10 Leveling Velocity
		1600,			// 11 Up Terminal Velocity
		1600,			// 12 Down Terminal Velocity
		10000,			// 13 UT Count to set software slowdown limit 
		10000,			// 14 DT Count to set software slowdown limit
		3,				// 15 Short floor
		10,				// 16 Soft Stop Pulse timer 10 msec count
		1,				// 17 Buffer Type 0 = Normal, 1 = Reduced Stroke
		1000,			// 18 Pulse count fail timer
		50,				// 19 Velocity Fault delay timer
		1,				// 20 Speed Check disable (0 = enable, 1 = disable)
		1,				// 21 Communications Check Disable, 0 = Use comm check, 1 = No comm check
		1,				// 22 Encoder Direction, 0 = Up, 1 = Down (used to change encoder direction flag)
		1,				// 23 Unitended motion check, 0=motion check, 1=Disable motion check
		32000,			// 24 Pulses per foot
		1,				// 25 Can Baud Rate flag: 0=115.2K, 1=57.6K
		2,				// 26 Setup Mode
		1600,			// 27 CPLD Up Terminal Velocity
		1600,			// 28 CPLD Down Terminal Velocity
		0,				// 29
		0,				// 30
	};

	int16 spb_fvars_min[numspbvars] = {
		25,				// 0  Top Speed
		25,				// 1  Encoder RPM
		10,				// 2  Encoder PPR
		0,				// 3  Encoder Type 0 = tape, 1 = governor encoder, 2 = absolute encoder tape, 3 = pulser from governor
		0,				// 4  Controller Type 0 = Hydro, 1 = Traction OL, 2 = Traction Distance Feedback
		0,				// 5  2 Stop 0 = Multi stop, 1 = 2 stop
		0,				// 6  Rear Doors 0 = no rear, 1 = rear doors
		0,				// 7  Terminal Slowdown Up Velocity
		0,				// 8  Terminal Slowdown Down Velocity
		0,				// 9  Inspection Velocity
		0,				// 10 Leveling Velocity
		0,				// 11 Up Terminal Velocity
		0,				// 12 Down Terminal Velocity
		0,				// 13 UT Count to set software slowdown limit 
		0,				// 14 DT Count to set software slowdown limit
		0,				// 15 Short Floor
		1,				// 16 Soft Stop Pulse timer 10 msec count
		0,				// 17 Buffer Type 0 = Normal, 1 = Reduced Stroke
		100,			// 18 Pulse count fail timer
		10,				// 19 Velocity Fault delay timer
		0,				// 20 Speed Check disable (0 = enable, 1 = disable)
		0,				// 21 Communications Check Disable, 0 = Use comm check, 1 = No comm check
		0,				// 22 Encoder Direction, 0 = Up, 1 = Down (used to change encoder direction flag)
		0,				// 23 Unitended motion check, 0=motion check, 1=Disable motion check
		1,				// 24 Pulses per foot
		0,				// 25 Can Baud Rate flag: 0=115.2K, 1=57.6K
		0,				// 26 Setup Mode
		0,				// 27 CPLD Up Terminal Velocity
		0,				// 28 CPLD Down Terminal Velocity
		0,				// 29
		0,				// 30
	};

	int16 spb_fvars_default[numspbvars] = {
		200,			// 0  Top Speed
		1050,			// 1  Encoder RPM
		2048,			// 2  Encoder PPR
		0,				// 3  Encoder Type 0 = tape, 1 = governor encoder, 2 = absolute encoder tape, 3 = pulser from governor
		0,				// 4  Controller Type 0 = Hydro, 1 = Traction OL, 2 = Traction Distance Feedback
		0,				// 5  2 Stop 0 = Multi stop, 1 = 2 stop
		0,				// 6  Rear Doors 0 = no rear, 1 = rear doors
		200,			// 7  Terminal Slowdown Up Velocity
		200,			// 8  Terminal Slowdown Down Velocity
		140,			// 9  Inspection Velocity
		140,			// 10 Leveling Velocity
		200,			// 11 Up Terminal Velocity
		200,			// 12 Down Terminal Velocity
		12,				// 13 UT Count to set software slowdown limit 
		12,				// 14 DT Count to set software slowdown limit
		0,				// 15 Short floor
		1,				// 16 Soft Stop Pulse timer 10 msec count default
		0,				// 17 Buffer Type 0 = Normal, 1 = Reduced Stroke
		200,			// 18 Pulse count fail timer
		18,				// 19 Velocity Fault delay timer
		0,				// 20 Speed Check disable (default 0 = enable)
		0,				// 21 Communications Check Disable, 0 = Use comm check, 1 = No comm check
		0,				// 22 Encoder Direction, 0 = Up, 1 = Down (used to change encoder direction flag)
		0,				// 23 Unitended motion check, 0=motion check, 1=Disable motion check
		16,				// 24 Pulses per foot
		0,				// 25 Can Baud Rate flag: 0=115.2K, 1=57.6K
		0,				// 26 Setup Mode
		200,			// 27 CPLD Up Terminal Velocity
		200,			// 28 CPLD Down Terminal Velocity
		0,				// 29
		0,				// 30
	};

#define lastspbfvar 28			// set to the last variable number


const char SPB_fvars_Min_Max[lastspbfvar+1][21] ={
  "Mn=25  Mx=2000   fpm",   // 0 Top Speed
  "Mn=25  Mx=18000  rpm",	// 1 Encoder RPM
  "Mn=10  Mx=18000  ppr",	// 2 Encoder PPR
  "Min=0  Mx=4     Type",	// 3 Encoder Type 0 = tape, 1 = governor encoder, 2 = absolute encoder tape, 3 = pulser from governor
  "0=Hydo,1=TrOL,2=TrDF",	// 4 Controller Type 0 = Hydro, 1 = Traction OL, 2 = Traction Distance Feedback
  "0=Multi-Stop,1=2Stop",	// 5 2 Stop 0 = Multi stop, 1 = 2 stop
  "0=No Rear,1=RearDoor",	// 6 Rear Doors 0 = no rear, 1 = rear doors
  "Min=0  Mx=1000   fpm",   // 7 Terminal Slowdown Up Velocity
  "Min=0  Mx=1000   fpm",   // 8 Terminal Slowdown Down Velocity
  "Min=0  Mx=200    fpm",   // 9 Inspection Velocity
  "Min=0  Mx=200    fpm",   // 10 Leveling Velocity
  "Min=0  Mx=1000   fpm",	// 11 Up Terminal Velocity
  "Min=0  Mx=1000   fpm",	// 12 Down Terminal Velocity
  "Min=0  Mx=10000  cnt",	// 13 UT Count to set software slowdown limit 
  "Min=0  Mx=10000  cnt",	// 14 DT Count to set software slowdown limit
  "0=non,1=T,2=B,3=Both",	// 15 Short Floor
  "Min=0  Mx=10     sec",	// 16 Soft Stop Pulse timer 10 msec count
  "0=Nrm,1=Reduce Strok",	// 17 Buffer Type 0 = Normal, 1 = Reduced Stroke
  "Mn=100 Mx=1000   sec",	// 18 Pulse count fail timer
  "Mn=10  Mx=50     sec",	// 19 Velocity Fault delay timer
  " 0=Enable, 1=Disale ",	// 20 Speed Check disable (0 = enable, 1 = disable)
  "0=CommCk,1=No CommCk",	// 21 Communications Check Disable, 0 = Use comm check, 1 = No comm check
  "0=Normal, 1=Invert  ",	// 22 Encoder Direction, 0 = Up, 1 = Down (used to change encoder direction flag)
  "0=UMotion Ck, 1=Dis ",	// 23 Unitended motion check, 0=motion check, 1=Disable motion check
  "Min=1  Mx=32000  ppf",	// 24 Pulses per foot
  "0=115.2, 1=57.6 Baud",	// 25 Can Baud Rate flag: 0=115.2K, 1=57.6K
  "0=Nrm,1=AdjPAL,2=Dbg",	// 26 Setup Mode
  "Min=0  Mx=1000   fpm",	// 27 CPLD Up Terminal Velocity
  "Min=0  Mx=1000   fpm",	// 28 CPLD Down Terminal Velocity
};

const char SPB_Field_Vars[lastspbfvar+1][21] = {
  "    Top Speed=      ",	// 0 Top Speed
  "  Encoder RPM=      ",	// 1 Encoder RPM
  "  Encoder PPR=      ",	// 2 Encoder PPR
  " Encoder Type=      ",	// 3 Encoder Type 0 = tape, 1 = governor encoder, 2 = absolute encoder tape, 3 = pulser from governor
  " Control Type=      ",	// 4 Controller Type 0 = Hydro, 1 = Traction OL, 2 = Traction Distance Feedback
  "       2 Stop=      ",	// 5 2 Stop 0 = Multi stop, 1 = 2 stop
  "   Rear Doors=      ",	// 6 Rear Doors 0 = no rear, 1 = rear doors
  " UTS Velocity=      ",	// 7 Terminal Slowdown Up Velocity
  " DTS Velocity=      ",	// 8 Terminal Slowdown Down Velocity
  "Insp Velocity=      ",	// 9 Inspection Velocity
  " Leveling Vel=      ",	// 10 Leveling Velocity
  "   ETS Up Vel=      ",	// 11 Up Terminal Velocity
  " ETS Down Vel=      ",	// 12 Down Terminal Velocity
  "    Unused 13=      ",	// 13 UT Count to set software slowdown limit 
  "    Unused 14=      ",	// 14 DT Count to set software slowdown limit
  "  Short Floor=      ",	// 15 Short Floor
  "Soft Stop Tim=      ",	// 16 Soft Stop Pulse timer 10 msec count
  "  Buffer Type=      ",	// 17 Buffer Type 0 = Normal, 1 = Reduced Stroke
  "Pulse Flt Tmr=      ",	// 18 Pulse count fail timer
  "Vel Flt Timer=      ",	// 19 Velocity Fault delay timer
  "Speed Chk Dis=      ",	// 20 Speed Check disable (0 = enable, 1 = disable)
  " Comm Chk Dis=      ",	// 21 Communications Check Disable, 0 = Use comm check, 1 = No comm check
  "  Encoder Dir=      ",	// 22 Encoder Direction, 0 = Up, 1 = Down (used to change encoder direction flag)
  "UMotionCk Dis=      ",	// 23 Unitended motion check, 0=motion check, 1=Disable motion check
  "Pulses Per Ft=      ",	// 24 Pulses per foot
  "Can Baud Rate=      ",	// 25 Can Baud Rate flag: 0=115.2K, 1=57.6K
  "ETS SetupMode=      ",   // 26 Debug Mode
  "PAL ETS UpVel=      ",	// 27 CPLD Up Terminal Velocity
  "PAL ETS DnVel=      ",	// 28 CPLD Down Terminal Velocity
};


#define spb_rear_door 6
#define spb_buffer_type 17
#define spb_setup_mode 26


const char Wait [21] = 
{
  "Waiting for Data ..."
};


uint16 spb_last_var = 26; 

uint16 const spb_var_ix[numspbvars] = {
// Car Speed Profile Variables for traction - Car Motion
 0, // 0 Top Speed
 1, // 1 Encoder RPM
 2, // 2 Encoder PPR
 3, // 3 Encoder Type 0 = tape, 1 = governor encoder, 2 = absolute encoder tape, 3 = pulser from governor
 4, // 4 Controller Type 0 = Hydro, 1 = Traction OL, 2 = Traction Distance Feedback
 5, // 5 2 Stop 0 = Multi stop, 1 = 2 stop
 6, // 6 Rear Doors 0 = no rear, 1 = rear doors
 7, // 7 Terminal Slowdown Up Velocity
 8, // 8 Terminal Slowdown Down Velocity
11, // 9 Up Terminal Velocity
12, // 10 Down Terminal Velocity
27, // 11 CPLD Up Terminal Velocity
28, // 12 CPLD Down Terminal Velocity
 9, // 13 Inspection Velocity
10, // 14 Leveling Velocity
15, // 15 Short Floor
16, // 16 Soft Stop Pulse timer 10 msec count
17, // 17 Buffer Type 0 = Normal, 1 = Reduced Stroke
18, // 18 Pulse count fail timer
19, // 19 Velocity Fault delay timer
20, // 20 Speed Check disable (0 = enable, 1 = disable)
21, // 21 Communications Check Disable, 0 = Use comm check, 1 = No comm check
22, // 22 Encoder Direction, 0 = Up, 1 = Down (used to change encoder direction flag)
23, // 23 Unitended motion check, 0=motion check, 1=Disable motion check
24, // 24 Pulses per foot
25,	// 25 Can Baud Rate flag: 0=115.2K, 1=57.6K
26, // 26 Setup Mode
13, // 27 Unused previously UT Count to set software slowdown limit 
14, // 28 Unused previously DT Count to set software slowdown limit
29, // 29 Unused
30  // 30 Unused
};


void detect_spb_fvars_change (void)
{
	int16 i = 0;
	if (SPB_can.eep_req == 0)
	{
		for(i=0;i<=spb_last_var;i++)
		{
			if (spb_fvars[spb_var_ix[i]] != SPB_can.eeprom[spb_var_ix[i]])
			{
				SPB_can.eep_req = 1;			
			}
		}
	}
}

//****************************************************
// Adjust Safety Processor Field Variables
//****************************************************
#if (Traction == 1)

void Adjust_SPB_Parameters (void)
{
	int16 i;
	
	if(LCD_Pointer == 3)		// first time through procedure
	{
	    LCD_Pointer = 4;
		SPB_can.eep_req = 1;			
		timers[tspb_ui] = 0;
		SPB_can.eep_rcvd = 0;
	    Up_Date_LCD = 1;
	    LCD_Sub_Point = 0;
	    LCD_Dig_Point = 0;
		LCD_Var_ptr = 0;
	    for(i=0; i<=19; i++)
			LCD_Display[0][i] = LCD_Field_Variables[LCD_Menu_ptr][i];

	}

#if (Simulator == 1)
	if ((SPB_can.eep_req != 0) && (timers[tspb_ui] > 3))
		SPB_can.eep_rcvd = 1;
#endif
	
	if ((SPB_can.eep_rcvd == 1) || (timers[tspb_ui] > 30))
	{			// Display and adjust drive parameters	if (SPB_can.eep_rcvd == 1)
		if ((timers[tspb_ui] > 30) && (SPB_can.eep_rcvd == 0))
		{
		    Up_Date_LCD = 1;
			SPB_can.eep_req = 1;			
			timers[tspb_ui] = 0;
		}
	
 		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
  		{
  		    LCD_Enter_PB = 1;
  		    LCD_Sub_Point++;
  		    Up_Date_LCD = 1;
  		    if(LCD_Sub_Point > 1)	// save field new value entered
  		    {
				SPB_can.eep_req = 2;
				SPB_can.eep_rcvd = 0;
				timers[tspb_ui] = 0;
				SPB_eep_ix = spb_var_ix[LCD_Var_ptr];	
				SPB_eep_nmb = 1;	    
				wr_spb_fvar();
	  		    send_galcom_fvar(SPB_CPU,spb_var_ix[LCD_Var_ptr]);
				LCD_Sub_Point = 0;
				LCD_Dig_Point = 0;
  		    }
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
  		{
  		    LCD_Mode_PB = 1;  //
  		    if(LCD_Sub_Point == 0)	// Leave the field var procedure
  		    {
				Menu_level = 0;
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = LCD_Main_Menu[2][i];
					LCD_Display[1][i] = LCD_Field_Variables[LCD_Menu_ptr][i];
				}
				return;
  		    }
  		    else if(LCD_Sub_Point > 0)
  		    {
				Up_Date_LCD = 1;
				LCD_Dig_Point = set_adj_digit(spb_fvars_max[spb_var_ix[LCD_Var_ptr]],LCD_Dig_Point);			
  		    }
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
  		{
  		    LCD_UP_PB = 1;  // incriment
  		    Up_Date_LCD = 1;
  		    if(LCD_Sub_Point == 0)
  		    {
				LCD_Var_ptr++;
				if(LCD_Var_ptr > spb_last_var)	  // Last fvars # 
				    LCD_Var_ptr = 0;
  		    }
  		    else	// increment fieldvar
				adjust_variable(&spb_fvars[spb_var_ix[LCD_Var_ptr]],
						spb_fvars_max[spb_var_ix[LCD_Var_ptr]],
						spb_fvars_min[spb_var_ix[LCD_Var_ptr]],1,LCD_Dig_Point);
  		}
  		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
  		{
  		    LCD_DN_PB = 1;  // decriment
  		    Up_Date_LCD = 1;
  		    if(LCD_Sub_Point == 0)
  		    {
				LCD_Var_ptr--;
				if(LCD_Var_ptr < 0)
			    	LCD_Var_ptr = (int16)(spb_last_var);	 // Last fvars #
  		    }
  		    else	// decrement fieldvar
				adjust_variable(&spb_fvars[spb_var_ix[LCD_Var_ptr]],
						spb_fvars_max[spb_var_ix[LCD_Var_ptr]],
						spb_fvars_min[spb_var_ix[LCD_Var_ptr]],0,LCD_Dig_Point);
  		}

  		if(Up_Date_LCD == 1)
  		{
   		    Up_Date_LCD = 0;
  		    for(i=0; i<=19; i++)
  		    {
				  if(LCD_Sub_Point > 0)
				      LCD_Display[0][i] = SPB_fvars_Min_Max[spb_var_ix[LCD_Var_ptr]][i];
				  else
				      LCD_Display[0][i] = LCD_Field_Variables[LCD_Menu_ptr][i];
				  
				  LCD_Display[1][i] = SPB_Field_Vars[spb_var_ix[LCD_Var_ptr]][i];
  		    }
			if(LCD_Sub_Point > 0)
				Display_Var_Label(LCD_Menu_ptr,spb_var_ix[LCD_Var_ptr],spb_fvars[spb_var_ix[LCD_Var_ptr]]);
			
			display_variable(1, 14, spb_fvars_deci[spb_var_ix[LCD_Var_ptr]], spb_fvars[spb_var_ix[LCD_Var_ptr]], 1);
  		}
  		flash_digit(LCD_Flash_TMP[1], 1,LCD_Dig_Loc,(LCD_Sub_Point > 0));
	}
	else
	{
 	    Up_Date_LCD = 1;
 		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
  		{
  		    LCD_Mode_PB = 1;  //
			Menu_level = 0;
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Main_Menu[2][i];
				LCD_Display[1][i] = LCD_Field_Variables[LCD_Menu_ptr][i];
			}
			return;
  		}
  		for(i=0; i<=19; i++)
  			LCD_Display[1][i] = Wait[i];

	}
}

#endif

// SPB IO display  --- make an even number of lines all the time
const char SPB_IOS [12][25] = 
{
  " ACC=0 AUTO=0  INS=0",
  " ICI=0  MRI=0   GS=0",
  " DLT=0  DLM=0  DLB=0",
  "  UL=0   DL=0  GBP=0",
  " ETS=0  LBP=0  GR1=0",
  " UTS=0  DTS=0 PICE=0",
  "  UP=0  DNR=0 SFCO=0",
  " RLM=0  RGS=0  LSH=0",
  "COMF=0 VELF=0 ACTF=0",
  " PFT=0  UTF=0  DTF=0",
  "PALF=0  RSB=0  TST=0",
  " REN=0              ",
};

#if (Traction == 1)

void Display_SPB_IO (void)
{

	int16 i;
	
	if (LCD_Pointer == 7)
	{		// first time through
		LCD_Pointer++;
		SPB_can.io_req = 1;
		LCD_Sub_Point = 0;
		timers[tspb_ui] = 0;
  	    Up_Date_LCD = 1;
	}
	
	if (timers[tspb_ui] >= 3)		// request io status every 300 msec
	{
		SPB_can.io_req = 1;
		timers[tspb_ui] = 0;
	}
	
 	if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
  	{
  	    LCD_Enter_PB = 1;
  	    Up_Date_LCD = 1;
  	}
  	if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
  	{
  	    LCD_Mode_PB = 1;  //
		LCD_Pointer = 7;
		Menu_level = 0;
		for(i=0; i<=19; i++)
		{
		    LCD_Display[0][i] = LCD_Main_Menu[4][i];
			LCD_Display[1][i] = LCD_Input_Output[LCD_Pointer-5][i];
		}
		return;
  	}
  	if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
  	{
  	    LCD_UP_PB = 1;  // incriment
  	    Up_Date_LCD = 1;
  	    LCD_Sub_Point++;
		if(LCD_Sub_Point > 5)	  
			LCD_Sub_Point = 0;
  	}
  	if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
  	{
  	    LCD_DN_PB = 1;  // decriment
  	    Up_Date_LCD = 1;
  	    LCD_Sub_Point--;
		if(LCD_Sub_Point < 0)
		    LCD_Sub_Point = 5;
  	}

	if (Up_Date_LCD == 1)
	{
		for(i=0; i<=19; i++)
		{
		    LCD_Display[0][i] = SPB_IOS[(LCD_Sub_Point * 2)][i];
			LCD_Display[1][i] = SPB_IOS[(LCD_Sub_Point * 2) +1][i];
		}
	}

	if (LCD_Sub_Point == 0)
	{
		
		//  "  ACC=0  AUTO=0   INS=0 ",
		//  "  ICI=0   MRI=0    GS=0 ",
		if ((SPB_can.ios[1] & 0x02) != 0)	// acc
			LCD_Display[0][5] = '1';
		else
			LCD_Display[0][5] = '0';
		
		if ((SPB_can.ios[0] & 0x04) != 0)	// auto
			LCD_Display[0][12] = '1';
		else
			LCD_Display[0][12] = '0';
		
		if ((SPB_can.ios[0] & 0x80) != 0)	// ins
			LCD_Display[0][19] = '1';
		else
			LCD_Display[0][19] = '0';
		
		if ((SPB_can.ios[1] & 0x01) != 0)	// ici
			LCD_Display[1][5] = '1';
		else
			LCD_Display[1][5] = '0';
		
		if ((SPB_can.ios[0] & 0x20) != 0)	// mri
			LCD_Display[1][12] = '1';
		else
			LCD_Display[1][12] = '0';
		
		if ((SPB_can.ios[2] & 0x20) != 0)	// gs
			LCD_Display[1][19] = '1';
		else
			LCD_Display[1][19] = '0';
		
	}
	else if (LCD_Sub_Point == 1)
	{
		//  "  DLT=0   DLM=0   DLB=0 ",
  		//	"   UL=0    DL=0   GBP=0 ",
		if ((SPB_can.ios[2] & 0x40) != 0)	// dlt
			LCD_Display[0][5] = '1';
		else
			LCD_Display[0][5] = '0';
		
		if ((SPB_can.ios[0] & 0x01) != 0)	// dlm
			LCD_Display[0][12] = '1';
		else
			LCD_Display[0][12] = '0';
		
		if ((SPB_can.ios[1] & 0x40) != 0)	// dlb
			LCD_Display[0][19] = '1';
		else
			LCD_Display[0][19] = '0';
		
		if ((SPB_can.ios[0] & 0x40) != 0)	// ul
			LCD_Display[1][5] = '1';
		else
			LCD_Display[1][5] = '0';
		
		if ((SPB_can.ios[0] & 0x02) != 0)	// dl
			LCD_Display[1][12] = '1';
		else
			LCD_Display[1][12] = '0';
		
		if ((SPB_can.ios[0] & 0x08) != 0)	// gbp
			LCD_Display[1][19] = '1';
		else
			LCD_Display[1][19] = '0';
		
	}
	else if (LCD_Sub_Point == 2)
	{
		
 		//  "  ETS=0   LBP=0   GR1=0",
  		//	"  UTS=0   DTS=0 PICEN=0",
		
		if ((SPB_can.ios[2] & 0x04) != 0)	// ets
			LCD_Display[0][5] = '1';
		else
			LCD_Display[0][5] = '0';
		
		if ((SPB_can.ios[0] & 0x10) != 0)	// lbp
			LCD_Display[0][12] = '1';
		else
			LCD_Display[0][12] = '0';
		
		if ((SPB_can.ios[4] & 0x01) != 0)	// gr1
			LCD_Display[0][19] = '1';
		else
			LCD_Display[0][19] = '0';
		
		if ((SPB_can.ios[1] & 0x04) != 0)	// uts	
			LCD_Display[1][5] = '1';
		else
			LCD_Display[1][5] = '0';
		
		if ((SPB_can.ios[1] & 0x08) != 0)	// dts
			LCD_Display[1][12] = '1';
		else
			LCD_Display[1][12] = '0';
		
		if ((SPB_can.ios[3] & 0x02) != 0)	// pic EN
			LCD_Display[1][19] = '1';
		else
			LCD_Display[1][19] = '0';
		
	}
	else if (LCD_Sub_Point == 3)
	{
	
		//  "   UP=0   DNR=0  SFCO=0 ",
		//  "  RLM=0   RGS=0   LCS=0 ",
		if ((SPB_can.ios[2] & 0x08) != 0) 	// up
			LCD_Display[0][5] = '1';
		else
			LCD_Display[0][5] = '0';
		
		if ((SPB_can.ios[2] & 0x10) != 0)	// dnr
			LCD_Display[0][12] = '1';
		else
			LCD_Display[0][12] = '0';
		
		if ((SPB_can.ios[3] & 0x01) != 0)	// sfco
			LCD_Display[0][19] = '1';
		else
			LCD_Display[0][19] = '0';
		
		if ((SPB_can.ios[1] & 0x20) != 0)	// rlm
			LCD_Display[1][5] = '1';
		else
			LCD_Display[1][5] = '0';
		
		if ((SPB_can.ios[1] & 0x10) != 0)	// rgs
			LCD_Display[1][12] = '1';
		else
			LCD_Display[1][12] = '0';
		
		if ((SPB_can.ios[3] & 0x04) != 0)	// lcs
			LCD_Display[1][19] = '1';
		else
			LCD_Display[1][19] = '0';
		
	}
	else if (LCD_Sub_Point == 4)
	{
			
		//  " COMF=0  VELF=0  ACTF=0 ",
 		//  "  PFT=0   UTF=0   DTF=0 ",
		if ((SPB_can.ios[3] & 0x08) != 0)	// comf
			LCD_Display[0][5] = '1';
		else
			LCD_Display[0][5] = '0';
		
		if ((SPB_can.ios[3] & 0x10) != 0)	// velf
			LCD_Display[0][12] = '1';
		else
			LCD_Display[0][12] = '0';
		
		if ((SPB_can.ios[3] & 0x40) != 0)	// actf
			LCD_Display[0][19] = '1';
		else
			LCD_Display[0][19] = '0';
		
		if ((SPB_can.pal_status & 0x01) != 0)	// pulse fault
			LCD_Display[1][5] = '1';
		else
			LCD_Display[1][5] = '0';
		
		if ((SPB_can.pal_status & 0x02) != 0)	// Up ETS fault
			LCD_Display[1][12] = '1';
		else
			LCD_Display[1][12] = '0';
		
		if ((SPB_can.pal_status & 0x04) != 0)	// Down ETS fault
			LCD_Display[1][19] = '1';
		else
			LCD_Display[1][19] = '0';
	}
 	else if (LCD_Sub_Point == 5)
	{
			
		//  " PALF=0   RSB=0   TST=0 ",
		//  "  REN=0                 ",
		if ((SPB_can.pal_status & 0x08) != 0)	// pal fault
			LCD_Display[0][5] = '1';
		else
			LCD_Display[0][5] = '0';
		
		if ((SPB_can.pal_status & 0x10) != 0)	// reduced stroke buffer
			LCD_Display[0][12] = '1';
		else
			LCD_Display[0][12] = '0';
		
		if ((SPB_can.pal_status & 0x20) != 0)	// Test mode (setup)
			LCD_Display[0][19] = '1';
		else
			LCD_Display[0][19] = '0';
		
		if ((SPB_can.pal_status & 0x40) != 0)	// Rear Enable
			LCD_Display[1][5] = '1';
		else
			LCD_Display[1][5] = '0';
	}
  
}
#endif

/*	Revision History:

11/8/13 v7/0/27 mhd 1. Changed DTS,UTS, ETU and ETD velocity max to 1600 fpm.
11/10/14  V7.1.20 mhd	1. When field variable changed, send data to galcom device.

*/