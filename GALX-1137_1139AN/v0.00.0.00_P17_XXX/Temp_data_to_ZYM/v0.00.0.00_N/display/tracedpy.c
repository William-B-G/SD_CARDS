// WiFi Adjustable Variables
#define d_TRACEDPY 1
#include "global.h"

#define Num_Trace_Menus 10
static const char LCD_Trace_menu[Num_Trace_Menus][21] = {
  " Stop Trace         ",	// 0
  " Start Trace        ",	// 1
  " Trace Time Interval",	// 2
  " Trace Trigger Arm  ",	// 3
  " Trace Trig Window  ",	// 4
  " Setup Trace Trigger",	// 5
  " Show Trigger Logic ", 	// 6
  " Clear Trace Trigger",	// 7
  " Start Debug Trace  ",	// 8
  " Clear Debug Trace  ",	// 9
};

static const char LCD_Trace_Message[9][21] = {
  "   Trace Stopped    ",	// 0
  "   Trace Started    ",	// 1
  "   Trigger Cleared  ",	// 2
  " Debug Trace Started",	// 3
  " Debug Trace Cleared",	// 4
  "Not While Car Moving",	// 5
  "No Trigger Logic Set",	// 6
  "  Press Up to Edit  ",	// 7
  " or Enter to Select ",	// 8
};

static const char LCD_Trace_Logic_Message[9][21] = {
  " Trigger Trace ON:  ",	// 0
  "Sel Trig Logic <U/D>",	// 1
  "AND Logic Press Entr",	// 2
  "OR Logic Press Enter",	// 3
  "Press Up to Continue",	// 4
  "or Ent to Set Triggr",	// 5
  "   Trigger Trace    ",	// 6
  "   Setup Complete   ",	// 7
  "   Trigger On:      ",	// 8
};

static const char LCD_Trace_Interval_Message[9][21] = {
  "   10 msec (5 sec)  ",	// 0
  "   20 msec (10 sec) ",	// 1
  "   30 msec (15 sec) ",	// 2
  "   40 msec (20 sec) ",	// 3
};

static const char LCD_Trigger_Window_Message[21] = {
  "Trace Increments:   ",	// 0
};

static const char LCD_Trigger_Arm_Message[14][21] = {
  "    Always Armed    ",	// 0
  "   Power Up Reset   ",	// 1
  "  Initial At Floor  ",	// 2
  "    Motion Start    ",	// 3
  "   Initial Stop     ",	// 4
  "   Relevel Start    ",	// 5
  "Frnt Door Open Start",	// 6
  "Frnt Door Dwell Strt",	// 7
  "Frnt Door Close Strt",	// 8
  "Rear Door Open Start",	// 9
  "Rear Door Dwell Strt",	// 10
  "Rear Door Close Strt",	// 11
  "  Inspection Start  ",	// 12
  " Safety String Start",	// 13
};



static const char LCD_Trace_Logic[32][21] = {
  "Fault Change        ",	// 0
  "Fault Match         ",	// 1
  "Service Change      ",	// 2
  "Service Match       ",	// 3
  "Process Change      ",	// 4
  "Process Match       ",	// 5
  "Run Stat Change     ",	// 6
  "Run Status Match    ",	// 7
  "Slowdown Change     ",	// 8
  "Slowdown Match      ",	// 9
  "RSlowdown Change    ",	// 10
  "RSlowdown Match     ",	// 11
  "Status Change       ",	// 12
  "Status Match        ",	// 13
  "Status 2 Change     ",	// 14
  "Status 2 Match      ",	// 15
  "Flt Bits0 Change    ",	// 16
  "Flt Bits0 Match     ",	// 17
  "Flt Bits1 Change    ",	// 18
  "Flt Bits1 Match     ",	// 19
  "Flt Bits2 Change    ",	// 20
  "Flt Bits2 Match     ",	// 21
  "Flt Bits3 Change    ",	// 22
  "Flt Bits3 Match     ",	// 23
  "SPB Serv Change     ",	// 24
  "SPB Stat Change     ",	// 25
  "SPB Cmd Change      ",	// 26
  "NTS Serv Change     ",	// 27
  "NTS Stat Change     ",	// 28
  "NTS Cmd Change      ",	// 29
  "Status 3 Change     ",	// 30
  "Status 4 Change     ",	// 31
};

const char procf_table[24][19] = {
  "No Procedure Flag ",   // prpcf == 0 (no process flag)
  "Reset Mode        ",	  // procf == 1
  "Inspection Mode   ",	  // procf == 2
  "Motion            ",   // procf == 3 
  "Soft Start Mode   ",   // procf == 4
  "Constant Accel    ",   // procf == 5
  "Roll Over Max Vel ",   // procf == 6
  "Constant Velocity ",   // procf == 7
  "Roll Over Decel   ",   // procf == 8
  "Constant Decel    ",   // procf == 9
  "Targeting Floor   ",   // procf == 10
  "Emergency Slowdown",   // procf == 11
  "Safety String Open",   // procf == 12
  "Elevator Off Line ",   // procf == 13
  "Elevator Parked   ",   // procf == 14
  "Waiting Assignment",   // procf == 15
  "Door Procedure    ",   // procf == 16
  "Elevator Stalled  ",   // procf == 17
  "Reset Hydro Jack  ",   // procf == 18
  "Low Pressure Mode ",	  // procf == 19
  "Auto Learn HW Mode",	  // procf == 20
  "Emp Recovery Mode ",	  // procf == 21
  "Hot Oil Mode      ",	  // procf == 22
  "Brake Test Mode   ",	  // procf == 23
};

static const char LCD_Trace_Match[30] = {
0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0
};

static const uint32 Trace_ix_Max[30] = {
0,
max_nmbflts,
0,
last_servf,
0,
last_procf,
0,
0xFFFFFFFF,
0,
0xFFFF,
0,
0xFFFF,
0,
0xFFFFFFFF,
0,
0xFFFFFFFF,
0,
0xFF,
0,
0xFF,
0,
0xFF,
0,
0xFF,
0,0,0,0,0,0
};

			
const int8 digit_loc_dw[8] = {14,13,12,11,10,9,8,7};
const int8 digit_loc_w[8] = {12,11,10,9,0.0,0,0};
const int8 digit_loc_b[8] = {11,10,0,0,0.0,0,0};



int16 trace_trigger_menu (int16 lcd_menu);
void set_trigger_logic (int16 t_logic_ix, int16 logic);
void set_trigger_enable (int16 t_logic_ix);
void set_trigger_match (int16 t_logic_ix, uint32 t_match_ix);
void display_trigger (int16 t_logic_ix, uint32 t_match_ix, int16 row, int16 dpy_trigger_logic);
int16 set_adj_hex_digit (uint32 max,int16 hex_digit);
void adjust_hex_variable (uint32 *pvar, uint32 max, uint16 min, int16 inc, int16 digit);
int16 find_trace_trigger (int16 inc, int16 t_logic_ix);
uint32 get_trigger_match (int16 t_logic_ix);
void set_trigger_status (uint32 bitnmb);

int16 trace_menu_level = 0;
int16 trace_menu_sel = 0;
int16 trace_menu_state = 0;
int16 trace_message = 0;
int16 logic_message = 0;
int16 logic_ix = 0;			// and or or logic
int16 trace_logic_ix = 0;	// variable selection
uint32 trace_match_ix = 0;	// variable match
int16 hex_digit = 0;
int8 hex_digit_loc = 0;
int8 hex_digit_hold = 0;
int16 trace_interval_ix = 0;
int16 trigger_arm_ix = 0;
int16 trace_flash = 0;
int16 number_of_triggers = 0;
int16 last_trigger = 0;
int16 first_trigger = 0;

int16 trace_trigger_menu (int16 lcd_menu)
{
	int16 i;
	int16 valid_trigger = 0;
	static int16 upd_dpy;
	int16 last_menu;
	int16 start_ptr;
	int16 line_ptr;
	
	last_menu = Num_Trace_Menus - 1;
	
	if (PasswordFlag == false)
   	{
  		StoreLCDInfo();
		clrLCDdpy();
  		LCD_Menu = 25;
  		LCD_Init = 1;
		return(lcd_menu);
   	}
	if(LCD_Sub_Init == 1)	
	{
		LCD_Sub_Init = 0;
	    upd_dpy = 1;
	    trace_menu_level = 0;
	    trace_menu_sel = 0;
	}
	if (trace_menu_level == 0)
	{	// Trace Menu
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    if (trace_menu_sel > 0)
			    trace_menu_sel--;
		    else
		    	trace_menu_sel = last_menu;
		}

		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    if (trace_menu_sel < last_menu)
		    	trace_menu_sel++;
		    else
		    	trace_menu_sel = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    trace_menu_state = 0;
	   		trace_menu_level = trace_menu_sel+1;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
	  		ModeButton = 1;  
		    trace_menu_sel = 0;
		    trace_menu_level = 0;
		    trace_menu_state = 0;
			lcd_menu = 0;
	  		return(lcd_menu);
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			line_ptr = (trace_menu_sel % 3);
			start_ptr = trace_menu_sel - line_ptr;
		    for(i=0; i<=19; i++)
		    {
	  			LCD_Display[0][i] = getcaps(LCD_Software_Menu[8][i]);
			  	LCD_Display[1][i] = LCD_Trace_menu[start_ptr][i];
			  	if ((start_ptr + 1) > last_menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else 
			  	{
				  	LCD_Display[2][i] = LCD_Trace_menu[start_ptr+1][i];
			  		if ((start_ptr + 2) > last_menu)
				  		LCD_Display[3][i] = ' ';
		    		else
					  	LCD_Display[3][i] = LCD_Trace_menu[start_ptr+2][i];
			  	}
		    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}
	else if (trace_menu_level == 1)
	{		// Stop (Trigger) Trace
		if (trace_menu_state == 0)
		{
			Trace_Trigger = 1;
			trace_menu_state = 1;
			upd_dpy = 1;
			trace_message = 0;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
				LCD_Display[2][i] = LCD_Trace_Message[trace_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			trace_menu_level = 0;
			trace_menu_sel = 0;
			upd_dpy = 1;
			return(lcd_menu);
		}
	}
	else if (trace_menu_level == 2)
	{		// Start Trace Recording
		if (trace_menu_state == 0)
		{
			start_trace();
			trace_menu_state = 1;
			upd_dpy = 1;
			trace_message = 1;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
				LCD_Display[2][i] = LCD_Trace_Message[trace_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			trace_menu_level = 0;
			trace_menu_sel = 1;
			upd_dpy = 1;
			return(lcd_menu);
		}
	}
	if (trace_menu_level == 3)
	{	// Set Trace Time Interval
		if (trace_menu_state == 0)
		{
			trace_menu_state = 1;
			upd_dpy = 1;
			trace_interval_ix = trace_interval;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    if (trace_menu_state == 2)
		    {
			    if (trace_interval_ix < 3)
			    	trace_interval_ix++;
			    else
			    	trace_interval_ix = 0;
		    }
		    upd_dpy = 1;
		}

		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    if (trace_menu_state == 2)
		    {
			    if (trace_interval_ix > 0)
				    trace_interval_ix--;
			    else
			    	trace_interval_ix = 3;
		    }
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			upd_dpy = 1;
		    if (trace_menu_state == 2)
		    {
				trace_interval_ix = trace_interval;
				trace_menu_state = 1;
		    }
		    else
		    {
				trace_menu_level = 0;
				trace_menu_sel = 2;
				return(lcd_menu);
		    }
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    if (trace_menu_state == 1)
		    {
		    	trace_menu_state = 2;
		    }
		    else 
		    {
			    trace_interval = trace_interval_ix;
			    write_trace_vars();
				start_trace();
				trace_menu_state = 1;
		    }
		}
		if (trace_flash != LCD_Flash)
		{
			upd_dpy = 1;
			trace_flash = LCD_Flash;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			if ((LCD_Flash == 0) && (trace_menu_state == 2))
			{
		    	for(i=0; i<=19; i++)
					LCD_Display[2][i] = ' ';
			}
			else 
			{
		    	for(i=0; i<=19; i++)
		    	{
		    		LCD_Display[0][i] = ' ';
					LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
					LCD_Display[2][i] = LCD_Trace_Interval_Message[trace_interval_ix][i];
		    		LCD_Display[3][i] = ' ';
		    	}
			}
		}
	}
	if (trace_menu_level == 4)
	{	// Set Trace Trigger Arm
		if (trace_menu_state == 0)
		{
			trace_menu_state = 1;
			upd_dpy = 1;
			for(i=1;i<=16;i++)
			{
				if ((trigger_arm & bittbl[i-1]) != 0) 
					break;
			}
			if (i <= 16)	
				trigger_arm_ix = i;
			else
				trigger_arm_ix = 0;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    if (trace_menu_state == 2)
		    {
			    if (trigger_arm_ix < 13)
			    	trigger_arm_ix++;
			    else
			    	trigger_arm_ix = 0;
		    }
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    if (trace_menu_state == 2)
		    {
			    if (trigger_arm_ix > 0)
				    trigger_arm_ix--;
			    else
			    	trigger_arm_ix = 13;
		    }
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			upd_dpy = 1;
		    if (trace_menu_state == 2)
		    {
				for(i=1;i<=16;i++)
				{
					if ((trigger_arm & bittbl[i-1]) != 0) 
						break;
				}
				if (i <= 16)	
					trigger_arm_ix = i;
				else
					trigger_arm_ix = 0;
				trace_menu_state = 1;
		    }
		    else
		    {
				trace_menu_level = 0;
				trace_menu_sel = 3;
				return(lcd_menu);
		    }
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    if (trace_menu_state == 1)
		    {
		    	trace_menu_state = 2;
		    }
		    else 
		    {
			    trigger_arm = (uint16)(l_bit_mask[trigger_arm_ix] >> 1);	// start from a value of zero
			    write_trace_vars();
				start_trace();
				trace_menu_state = 1;
		    }
		}
		if (trace_flash != LCD_Flash)
		{
			upd_dpy = 1;
			trace_flash = LCD_Flash;
		}
			
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			if ((LCD_Flash == 0) && (trace_menu_state == 2))
			{
		    	for(i=0; i<=19; i++)
					LCD_Display[2][i] = ' ';
			}
			else 
			{
		    	for(i=0; i<=19; i++)
		    	{
	    			LCD_Display[0][i] = ' ';
					LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
					LCD_Display[2][i] = LCD_Trigger_Arm_Message[trigger_arm_ix][i];
	    			LCD_Display[3][i] = ' ';
		    	}
			}
		}
	}
	if (trace_menu_level == 5)
	{	// Set Trigger Window
		if (trace_menu_state == 0)
		{
			trace_menu_state = 1;
			upd_dpy = 1;
			LCD_Dig_Point = 0;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    if (trace_menu_state == 2)
		    	adjust_variable (&trigger_window, 499, 0, 1, LCD_Dig_Point);
		    upd_dpy = 1;
		}

		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    if (trace_menu_state == 2)
			    adjust_variable (&trigger_window, 499, 0, 0, LCD_Dig_Point);
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    if (trace_menu_state == 1)
		    	trace_menu_state = 2;
		    else
		    {
		    	trace_menu_state = 1;
			    write_trace_vars();
				start_trace();
		    }
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			upd_dpy = 1;
		    if (trace_menu_state == 2)
		    {
				LCD_Dig_Point = set_adj_digit(499,LCD_Dig_Point);			
		    }
		    else
		    {
				trace_menu_level = 0;
				trace_menu_sel = 4;
				return(lcd_menu);
		    }
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
				LCD_Display[2][i] = LCD_Trigger_Window_Message[i];
	    		LCD_Display[3][i] = ' ';
	    	}
			display_variable(3, 6, 0, trigger_window, 1);
		}
  		flash_digit(LCD_Flash_TMP[3], 3,LCD_Dig_Loc,(trace_menu_state == 2));
	}
	else if (trace_menu_level == 6)
	{		// Setup Trace Logic
		if (trace_menu_state == 0)
		{
			trace_menu_state = 1;
			upd_dpy = 1;
			logic_message = 0;
			trace_logic_ix = 0;
			logic_ix = 0;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    if (trace_menu_state == 1)
		    {
			    if (trace_logic_ix < 31)
			    	trace_logic_ix++;
			    else
			    	trace_logic_ix = 0;
		    }
		    else
		    {
		    	if (logic_ix < 1)
		    		logic_ix++;
		    	else
		    		logic_ix = 0;
		    }
		    upd_dpy = 1;
		}

		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    if (trace_menu_state == 1)
		    {
			    if (trace_logic_ix > 0)
				    trace_logic_ix--;
			    else
			    	trace_logic_ix = 31;
		    }
		    else
		    {
		    	if (logic_ix > 0)
		    		logic_ix--;
		    	else
		    		logic_ix = 1;
		    }
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    if (trace_menu_state == 1)
		    {
		    	trace_menu_state = 2;
		    	logic_message = 1;
		    }
		    else 
		    {
	    		set_trigger_logic(trace_logic_ix,logic_ix);
		    	if (LCD_Trace_Match[trace_logic_ix] == 1)
	   				trace_menu_level = 11;
		    	else
		    		trace_menu_level = 12;
		    	trace_menu_state = 0;
				return(lcd_menu);
		    }
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			trace_menu_level = 0;
			trace_menu_sel = 5;
			upd_dpy = 1;
			return(lcd_menu);
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
		    if (trace_menu_state == 1)
		    {
		    	for(i=0; i<=19; i++)
		    	{
		    		LCD_Display[0][i] = ' ';
					LCD_Display[1][i] = LCD_Trace_Logic_Message[logic_message][i];
					LCD_Display[2][i] = LCD_Trace_Logic[trace_logic_ix][i];
		    		LCD_Display[3][i] = ' ';
		    	}
		    }
		    else
		    {
		    	for(i=0; i<=19; i++)
		    	{
		    		LCD_Display[0][i] = ' ';
					LCD_Display[1][i] = LCD_Trace_Logic_Message[logic_message][i];
					LCD_Display[2][i] = LCD_Trace_Logic_Message[logic_message+logic_ix+1][i];
		    		LCD_Display[3][i] = ' ';
		    	}
		    }
		}
	}
	else if (trace_menu_level == 7)
	{		// Show Trace Logic
		if (trace_menu_state == 0)
		{
			trace_menu_state = 1;
			upd_dpy = 1;
			logic_message = 0;
			trace_logic_ix = 29;		// value gets incremented and will roll over to zero
			trace_match_ix = 0;
			valid_trigger = find_trace_trigger(1,trace_logic_ix);		// sets trace_logic_ix and trace_match_ix
			set_trigger_status(trigger_enable);		// Number of triggers bits set and last trigger
			first_trigger = trace_logic_ix;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
			valid_trigger = find_trace_trigger(1,trace_logic_ix);		// sets trace_logic_ix and trace_match_ix
			
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
			valid_trigger = find_trace_trigger(0, trace_logic_ix);		// sets trace_logic_ix and trace_match_ix
		    upd_dpy = 1;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			if (valid_trigger == 1)
			{
	
				if (first_trigger == trace_logic_ix)
				{
					for(i=0; i<=19; i++)
			    	{
						LCD_Display[0][i] = LCD_Trace_Logic_Message[8][i];
			    		LCD_Display[1][i] = ' ';
			    		LCD_Display[2][i] = ' ';
			    		LCD_Display[3][i] = ' ';
			    	}
				}
				else
				{
					for(i=0; i<=19; i++)
					{
			    		LCD_Display[0][i] = ' ';
			    		LCD_Display[1][i] = ' ';
			    		LCD_Display[2][i] = ' ';
			    		LCD_Display[3][i] = ' ';
					}
				}
				trace_match_ix = get_trigger_match(trace_logic_ix);
				display_trigger(trace_logic_ix,trace_match_ix, 1,1);		
			}
			else
			{
				for(i=0; i<=19; i++)
				{
		    		LCD_Display[0][i] = ' ';
					LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
					LCD_Display[2][i] = LCD_Trace_Message[6][i];
	 		   		LCD_Display[3][i] = ' ';
				}
			}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			trace_menu_level = 0;
			trace_menu_sel = 6;
			upd_dpy = 1;
			return(lcd_menu);
		}
	}
	else if (trace_menu_level == 8)
	{		// Clear Trace Logic
		if (trace_menu_state == 0)
		{
			clear_trigger_logic();
			write_trace_vars();
			trace_menu_state = 1;
			upd_dpy = 1;
			trace_message = 2;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
				LCD_Display[2][i] = LCD_Trace_Message[trace_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			trace_menu_level = 0;
			trace_menu_sel = 7;
			upd_dpy = 1;
			return(lcd_menu);
		}
	}
	else if (trace_menu_level == 9)
	{		// Initialize Debug Trace
		if (trace_menu_state == 0)
		{
		    if (trace_auto_start == 0)
		    {
			    init_debug_trace();
		   		start_debug_trace();		  		    	
		    }
		    trace_auto_start = 1;	// update flag
			trace_menu_state = 1;
			upd_dpy = 1;
			trace_message = 3;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
				LCD_Display[2][i] = LCD_Trace_Message[trace_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			trace_menu_level = 0;
			trace_menu_sel = 8;
			upd_dpy = 1;
			return(lcd_menu);
		}
	}
	else if (trace_menu_level == 10)
	{		// Clear Debug Trace
		if (trace_menu_state == 0)
		{
			trace_menu_state = 1;
			upd_dpy = 1;
  		    if ((procf < 3) || (procf > 11))
			{
				clear_debug_trace();
				trace_message = 4;
			}
			else
				trace_message = 5;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Trace_menu[trace_menu_sel][i];
				LCD_Display[2][i] = LCD_Trace_Message[trace_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			trace_menu_level = 0;
			trace_menu_sel = 9;
			upd_dpy = 1;
			return(lcd_menu);
		}
	}
	else if (trace_menu_level == 11)
	{		// Set the service Match
		if (trace_menu_state == 0)
		{
			trace_menu_state = 1;
			upd_dpy = 1;
			trace_match_ix = 0;
			hex_digit = 0;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		  	if (trace_logic_ix <= 5)
		  	{
			    if (trace_match_ix < Trace_ix_Max[trace_logic_ix])
			    	trace_match_ix++;
			    else
			    	trace_match_ix = 0;
		  	}
		  	else
		  	{
				if (trace_menu_state == 2)
					trace_menu_state = 1;
				else
		  			adjust_hex_variable(&trace_match_ix,Trace_ix_Max[trace_logic_ix],0,1,hex_digit);
		  	}
		    upd_dpy = 1;
		}

		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		  	if (trace_logic_ix <= 5)
		  	{
			    if (trace_match_ix > 0)
				    trace_match_ix--;
			    else
			    	trace_match_ix = Trace_ix_Max[trace_logic_ix];
		  	}
		  	else
		  	{
		  		adjust_hex_variable(&trace_match_ix,Trace_ix_Max[trace_logic_ix],0,0,hex_digit);
		  	}
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	if (trace_logic_ix <= 5)
		  	{
			    upd_dpy = 1;
			    set_trigger_match(trace_logic_ix, trace_match_ix);
		   		trace_menu_level = 12;
		    	trace_menu_state = 0;
				return(lcd_menu);
			}
			else
			{
				if (trace_menu_state == 1)
				{
					for(i=0; i<=19; i++)
					{
	    				LCD_Display[0][i] = ' ';
						LCD_Display[1][i] = LCD_Trace_Message[7][i];
						LCD_Display[2][i] = LCD_Trace_Message[8][i];
	    				LCD_Display[3][i] = ' ';
					}
					trace_menu_state = 2;
				}
				else
			  	{
				    upd_dpy = 1;
				    set_trigger_match(trace_logic_ix, trace_match_ix);
			   		trace_menu_level = 12;
			    	trace_menu_state = 0;
				}
			}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			upd_dpy = 1;
		  	if ((trace_logic_ix <= 5) || (trace_menu_state == 2))
			{
				trace_menu_level = 0;
				trace_menu_sel = 5;
				return(lcd_menu);
			}
			else 
			{
				hex_digit = set_adj_hex_digit(Trace_ix_Max[trace_logic_ix],hex_digit);
			}
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			display_trigger(trace_logic_ix,trace_match_ix, 1, 0);
		}
	  	if (trace_logic_ix > 5)
		{
			flash_digit(hex_digit_hold, 2, (int16)hex_digit_loc, (int16)(trace_menu_state == 1));
		}
	}
	else if (trace_menu_level == 12)
	{		// Complete the Trigger setup
		if (trace_menu_state == 0)
		{
			trace_menu_state = 1;
			upd_dpy = 1;
			logic_message = 4;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    if (trace_menu_state == 1)
		    {
				set_trigger_enable(trace_logic_ix);
				write_trace_vars();
				start_trace();
			    upd_dpy = 1;
			    if (trace_logic_ix < 29)
			    	trace_logic_ix++;
			    else
			    	trace_logic_ix = 0;
				trace_menu_level = 6;
				trace_menu_state = 0;
				return(lcd_menu);
		    }
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    trace_menu_state = 2;
			logic_message = 6;
			set_trigger_enable(trace_logic_ix);
			write_trace_vars();
			start_trace();
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Trace_Logic_Message[logic_message][i];
				LCD_Display[2][i] = LCD_Trace_Logic_Message[logic_message+1][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			trace_menu_level = 0;
			trace_menu_sel = 6;
			upd_dpy = 1;
			return(lcd_menu);
		}
	}
	return(lcd_menu);
}

void set_trigger_logic (int16 t_logic_ix, int16 logic)
{
	if (logic == 0)
	{
		trigger_logic &= ~l_bit_mask[t_logic_ix];
		and_trigger_logic |= l_bit_mask[t_logic_ix];
	}
	else
	{
		trigger_logic |= l_bit_mask[t_logic_ix];
		and_trigger_logic &= ~l_bit_mask[t_logic_ix];
	}
	
}

void set_trigger_enable (int16 t_logic_ix)
{
	trigger_enable |= l_bit_mask[t_logic_ix];
}

void set_trigger_match (int16 t_logic_ix, uint32 t_match_ix)
{
	
	switch (t_logic_ix)
	{
		case 1:		// Fault Match
			trigger_fault = (uint16)(t_match_ix & 0x0000ffff);
			break;
		case 3:		// Service Match
			trigger_servf = (uint16)(t_match_ix & 0x000000ff);
			break;
		case 5:		// Process Match
			trigger_procf = (uint16)(t_match_ix & 0x000000ff);
			break;
		case 7:		// Run Status Match
			trigger_run_statusf = t_match_ix;
			break;
		case 9:		// Slowdown Match
			trigger_front_sd = (uint16)(t_match_ix & 0x0000ffff);
			break;
		case 11:	// Rear Slowdown Match
			trigger_rear_sd = (uint16)(t_match_ix & 0x0000ffff);
			break;
		case 13:	// Statusf Match
			trigger_statusf = t_match_ix;
			break;
		case 15:	// Statusf2 Match
			trigger_statusf2 = t_match_ix;
			break;
		case 17:	// Fault Bits 0
			trigger_fault_bits[0] = (uint8)(t_match_ix & 0x000000ff);
			break;
		case 19:	// Fault Bits 1
			trigger_fault_bits[1] = (uint8)(t_match_ix & 0x000000ff);
			break;
		case 21:	// Fault Bits 2
			trigger_fault_bits[2] = (uint8)(t_match_ix & 0x000000ff);
			break;
		case 23:	// Fault Bits 3
			trigger_fault_bits[3] = (uint8)(t_match_ix & 0x000000ff);
			break;
		default:
			break;
	}
}

uint32 get_trigger_match (int16 t_logic_ix)
{
	uint32 t_match_ix = 0;
	
	switch (t_logic_ix)
	{
		case 1:		// Fault Match
			t_match_ix = (uint32)trigger_fault;
			break;
		case 3:		// Service Match
			t_match_ix = (uint32)trigger_servf;
			break;
		case 5:		// Process Match
			t_match_ix = (uint32)trigger_procf;
			break;
		case 7:		// Run Status Match
			t_match_ix = (uint32)trigger_run_statusf;
			break;
		case 9:		// Slowdown Match
			t_match_ix = (uint32)trigger_front_sd;
			break;
		case 11:	// Rear Slowdown Match
			t_match_ix = (uint32)trigger_rear_sd;
			break;
		case 13:	// Statusf Match
			t_match_ix = (uint32)trigger_statusf;
			break;
		case 15:	// Statusf2 Match
			t_match_ix = (uint32)trigger_statusf2;
			break;
		case 17:	// Fault Bits 0
			t_match_ix = (uint32)trigger_fault_bits[0];
			break;
		case 19:	// Fault Bits 1
			t_match_ix = (uint32)trigger_fault_bits[1];
			break;
		case 21:	// Fault Bits 2
			t_match_ix = (uint32)trigger_fault_bits[2];
			break;
		case 23:	// Fault Bits 3
			t_match_ix = (uint32)trigger_fault_bits[3];
			break;
		default:
			t_match_ix = 0;
			break;
	}
	return(t_match_ix);
}

void display_trigger (int16 t_logic_ix, uint32 t_match_ix, int16 row, int16 dpy_trigger_logic)
{
	int16 i;

	
   	for(i=0; i<=19; i++)
   	{
    	LCD_Display[row][i] = LCD_Trace_Logic[t_logic_ix][i];
   	}

	switch (t_logic_ix)
	{
		case 1:		// Fault Match
   			row++;
	    	for(i=0; i<=19; i++)
	    	{
				LCD_Display[row][i] = LCD_Fault[t_match_ix][i];
	    	}
			break;
		case 3:		// Service Match
   			row++;
	    	for(i=0; i<=19; i++)
	    	{
				LCD_Display[row][i] = Service[t_match_ix][i];
	    	}
			break;
		case 5:		// Process Match
   			row++;
   			LCD_Display[row][0] = ' ';
	    	for(i=0; i<=17; i++)
	    	{
				LCD_Display[row][i+1] = procf_table[t_match_ix][i];
	    	}
   			LCD_Display[row][19] = ' ';
			break;
		case 7:		// Run Status Match
   			row++;
			sprintf(LCD_String,"%08x",t_match_ix);
		  	LCD_Display[row][7] = LCD_String[0];
		  	LCD_Display[row][8] = LCD_String[1];
		  	LCD_Display[row][9] = LCD_String[2];
		  	LCD_Display[row][10] = LCD_String[3];
		  	LCD_Display[row][11] = LCD_String[4];
		  	LCD_Display[row][12] = LCD_String[5];
		  	LCD_Display[row][13] = LCD_String[6];
		  	LCD_Display[row][14] = LCD_String[7];
		  	LCD_Display[row][15] = 'h';
		  	hex_digit_loc = digit_loc_dw[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
		case 9:		// Slowdown Match
   			row++;
			sprintf(LCD_String,"%04x",t_match_ix);
		  	LCD_Display[row][9] = LCD_String[0];
		  	LCD_Display[row][10] = LCD_String[1];
		  	LCD_Display[row][11] = LCD_String[2];
		  	LCD_Display[row][12] = LCD_String[3];
		  	LCD_Display[row][13] = 'h';
		  	hex_digit_loc = digit_loc_w[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
		case 11:	// Rear Slowdown Match
   			row++;
			sprintf(LCD_String,"%04x",t_match_ix);
		  	LCD_Display[row][9] = LCD_String[0];
		  	LCD_Display[row][10] = LCD_String[1];
		  	LCD_Display[row][11] = LCD_String[2];
		  	LCD_Display[row][12] = LCD_String[3];
		  	LCD_Display[row][13] = 'h';
		  	hex_digit_loc = digit_loc_w[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
		case 13:	// Statusf Match
   			row++;
			sprintf(LCD_String,"%08x",t_match_ix);
		  	LCD_Display[row][7] = LCD_String[0];
		  	LCD_Display[row][8] = LCD_String[1];
		  	LCD_Display[row][9] = LCD_String[2];
		  	LCD_Display[row][10] = LCD_String[3];
		  	LCD_Display[row][11] = LCD_String[4];
		  	LCD_Display[row][12] = LCD_String[5];
		  	LCD_Display[row][13] = LCD_String[6];
		  	LCD_Display[row][14] = LCD_String[7];
		  	LCD_Display[row][15] = 'h';
		  	hex_digit_loc = digit_loc_dw[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
		case 15:	// Statusf2 Match
   			row++;
			sprintf(LCD_String,"%08x",t_match_ix);
		  	LCD_Display[row][7] = LCD_String[0];
		  	LCD_Display[row][8] = LCD_String[1];
		  	LCD_Display[row][9] = LCD_String[2];
		  	LCD_Display[row][10] = LCD_String[3];
		  	LCD_Display[row][11] = LCD_String[4];
		  	LCD_Display[row][12] = LCD_String[5];
		  	LCD_Display[row][13] = LCD_String[6];
		  	LCD_Display[row][14] = LCD_String[7];
		  	LCD_Display[row][15] = 'h';
		  	hex_digit_loc = digit_loc_dw[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
		case 17:	// Fault Bits 0
   			row++;
			sprintf(LCD_String,"%02x",t_match_ix);
		  	LCD_Display[row][10] = LCD_String[0];
		  	LCD_Display[row][11] = LCD_String[1];
		  	LCD_Display[row][12] = 'h';
		  	hex_digit_loc = digit_loc_b[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
		case 19:	// Fault Bits 1
   			row++;
			sprintf(LCD_String,"%02x",t_match_ix);
		  	LCD_Display[row][10] = LCD_String[0];
		  	LCD_Display[row][11] = LCD_String[1];
		  	LCD_Display[row][12] = 'h';
		  	hex_digit_loc = digit_loc_b[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
		case 21:	// Fault Bits 2
   			row++;
			sprintf(LCD_String,"%02x",t_match_ix);
		  	LCD_Display[row][10] = LCD_String[0];
		  	LCD_Display[row][11] = LCD_String[1];
		  	LCD_Display[row][12] = 'h';
		  	hex_digit_loc = digit_loc_b[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
		case 23:	// Fault Bits 3
   			row++;
			sprintf(LCD_String,"%02x",t_match_ix);
		  	LCD_Display[row][10] = LCD_String[0];
		  	LCD_Display[row][11] = LCD_String[1];
		  	LCD_Display[row][12] = 'h';
		  	hex_digit_loc = digit_loc_b[hex_digit];
		  	hex_digit_hold = LCD_Display[row][hex_digit_loc];
			break;
			
		default:
			break;
	}
	
	if (dpy_trigger_logic == 1)
	{
		row++;
		if ((trigger_logic & l_bit_mask[t_logic_ix]) != 0)
		{	// ORed
			LCD_Display[row][6] = 'O';
			LCD_Display[row][7] = 'R';
			LCD_Display[row][8] = ' ';
		  	LCD_Display[row][9] = 'L';
		  	LCD_Display[row][10] = 'o';
		  	LCD_Display[row][11] = 'g';
		  	LCD_Display[row][12] = 'i';
		  	LCD_Display[row][13] = 'c';
		  	LCD_Display[row][14] = ' ';
		}
		else if ((and_trigger_logic & l_bit_mask[t_logic_ix]) != 0)
		{	// ANDed
			LCD_Display[row][6] = 'A';
			LCD_Display[row][7] = 'N';
			LCD_Display[row][8] = 'D';
			LCD_Display[row][9] = ' ';
		  	LCD_Display[row][10] = 'L';
		  	LCD_Display[row][11] = 'o';
		  	LCD_Display[row][12] = 'g';
		  	LCD_Display[row][13] = 'i';
		  	LCD_Display[row][14] = 'c';
		}
		else
		{	//  no logic selected
			LCD_Display[row][6] = ' ';
			LCD_Display[row][7] = ' ';
			LCD_Display[row][8] = ' ';
			LCD_Display[row][9] = ' ';
		  	LCD_Display[row][10] = ' ';
		  	LCD_Display[row][11] = ' ';
		  	LCD_Display[row][12] = ' ';
		  	LCD_Display[row][13] = ' ';
		  	LCD_Display[row][14] = ' ';
		}
			
	}
		
}

int16 find_trace_trigger (int16 inc, int16 t_logic_ix)
{
	int16 i,k;
	int16 found = 0;
	
	if (inc == 1)
	{
		i = t_logic_ix;
		k = 0;
		while ((k <= 29) && (found == 0))
		{
			i++;
			if (i > 29)
				i = 0;
			if ((trigger_enable & l_bit_mask[i]) != 0)
			{
				found = 1;
				trace_logic_ix = i;
//				trace_match_ix = get_trigger_match(i);
			}
			k++;
		}
	}
	else
	{
		i = t_logic_ix;
		k = 0;
		while ((k <= 29) && (found == 0))
		{
			i--;
			if (i < 0)
				i = 29;
			if ((trigger_enable & l_bit_mask[i]) != 0)
			{
				found = 1;
				trace_logic_ix = i;
//				trace_match_ix = get_trigger_match(i);
			}
			k++;
		}
	}
	return (found);
		
}

//***********************************
// Adjust field adjustable variable
//***********************************

const uint32 adj_hex_amt[10] = {0x01,0x10,0x100,0x1000,0x10000,0x100000,0x1000000,0x10000000,-1,-1};

void adjust_hex_variable (uint32 *pvar, uint32 max, uint16 min, int16 inc, int16 hex_digit)
{

	if (hex_digit > 7)
		hex_digit = 0;

	if (inc == 1)		// Increment variable
	{
		if (((*pvar & 0xF0000000) == 0xF0000000) && (hex_digit == 7))
			*pvar = max;
		else
			*pvar += adj_hex_amt[hex_digit];
	}
	else
	{
		if (((*pvar & 0xF0000000) == 0) && (hex_digit == 7))
			*pvar = min;
		else
		{
			*pvar -= adj_hex_amt[hex_digit];
			if (*pvar >= max)
				*pvar = min;
		}
	}
	
	if (*pvar > max)
		*pvar = max;
	else if ((min >= 0) && (*pvar < 0)) 
		*pvar = min;
	else if (*pvar < min)
		*pvar = min;
}

//********************************************
// Set the digit to alter adjustable hex variable
//********************************************

int16 set_adj_hex_digit (uint32 max,int16 hex_digit)
{
	uint32 tmax;

	hex_digit++;
	if (hex_digit > 7)
		hex_digit = 0;

	tmax = max;

	if (adj_hex_amt[hex_digit] > tmax)
		hex_digit = 0;
	return hex_digit;
}

//*********************************************************
// Set the trigger status
//*********************************************************

void set_trigger_status (uint32 bitnmb)
{
	int16 i,j,k,l;

	j = 0;
	k = 0;
	l = 0xff;
	for (i=0;i<=31;i++)
	{
		if ((bitnmb & l_bit_mask[i]) != 0)
		{
			if (l == 0xff)
				l = i;
			k = i;
			j++;
		}
	}
	if (l == 0xff)
		l = 0;
	last_trigger = k;
	number_of_triggers = j;
	first_trigger = l;
}


