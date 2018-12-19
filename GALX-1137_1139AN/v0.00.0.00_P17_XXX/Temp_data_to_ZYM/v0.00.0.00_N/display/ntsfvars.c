// NTS CPU Adjustable Variables
#define d_NTSFVARS 1
#include "global.h"


void detect_nts_fvars_change (void);
int16 Adjust_NTS_Parameters (int16 lcd_menu);
int16 Display_NTS_IO (int16 lcd_menu);


#define numntsvars 31

	int16 nts_fvars[numntsvars];
	
	int16 nts_fvars_deci[numntsvars] = {
		0,				// 0  Top Speed
		0,				// 1  Number of Valid Floors
		0,				// 2  Number of Limits
		0,				// 3  UT/DT Limit Dist
		0,				// 4  Velocity Direction
		0,				// 5  Can Baud Rate
		0,				// 6  Debug Mode
	};


	int16 nts_fvars_max[numntsvars] = {
	  200,				// 0  Top Speed
	   16,				// 1  Number of Valid Floors
	   	1,				// 2  Number of Limits
	   50,				// 3  UT/DT Limit Dist
	   	1,				// 4  Velocity Direction
	   	1,				// 5  Can Baud Rate
	   	1,				// 6  Debug Mode
	};

	int16 nts_fvars_min[numntsvars] = {
		0,				// 0  Top Speed
		2,				// 1  Number of Valid Floors
		1,				// 2  Number of Limits
		0,				// 3  UT/DT Limit Dist
		0,				// 4  Velocity Direction
		0,				// 5  Can Baud Rate
		0,				// 6  Debug Mode
	};

	int16 nts_fvars_default[numntsvars] = {
	  100,				// 0  Top Speed
	    3,				// 1  Number of Valid Floors
		1,				// 2  Number of Limits
	  	0,				// 3  UT/DT Limit Dist
	  	0,				// 4  Velocity Direction
	  	0,				// 5  Can Baud Rate
	  	0,				// 6  Debug Mode
	};
	
#define lastntsfvar 6			// set to the last variable number
	

const char NTS_fvars_Min_Max[lastntsfvar+1][21] ={
  "Min=0  Max=200   fpm",	// 0  Top Speed
  "Min=2  Max=20    flr",   // 1  Number of Valid Floors
  "Min=0  Max=1     num",   // 2  Number of Limits
  "Min=0  Max=50   inch",	// 3  UT/DT Limit Dist
  "0=Normal, 1=Invert  ",	// 4  Velocity Direction
  "0=115.2, 1=57.6 Baud",	// 5  Can Baud Rate
  "Min=0  Max=1    Mode",	// 6  Debug Mode
};

const char NTS_Field_Vars[lastntsfvar+1][21] = {
  "    Top Speed=      ",	// 0  Top Speed
  " Num Valid Fl=      ",	// 1  Number of Valid Floors
  "Num of Limits=      ",	// 2  Number of Limits
  "UT/DT Lim Dst=      ",	// 3  UT/DT Limit Dist
  "      Vel Dir=      ",	// 4  Velocity Direction
  "Can Baud Rate=      ",	// 5  Can Baud Rate
  "   Debug Mode=      ",	// 6  Debug Mode
};

const char Wait [21] = 
{
  "Waiting for Data ..."
};


int16 nts_last_var = 6; 

uint16 const nts_var_ix[numntsvars] = {
// Car Speed Profile Variables for traction - Car Motion
 0, // 0  Top Speed
 1, // 1  Number of Valid Floors
 2, // 2  Number of Limits
 3, // 3  UT/DT Limit Dist
 4, // 4  Velocity Direction
 5, // 5  Can Baud Rate
 6, // 6  Debug Mode
 7, // 7  
 8, // 8  
 9, // 9  
10, // 10 
11, // 11 
12, // 12 
13, // 13 
14, // 14 
16, // 15 
15, // 16 
17, // 17 
18, // 18 
19, // 19 
20, // 20 
21, // 21 
22, // 22 
23, // 23 
24, // 24 
25,	// 25 
26, // 26 
27, // 27 
28, // 28 
29, // 29 
30  // 30 
};

#define nts_fvtop_speed 0
#define nts_fvnum_floors 1
#define nts_fvnum_limits 2
#define nts_fvlimit_dist 3
#define nts_debug_mode 5

void detect_nts_fvars_change (void)
{
	int16 i = 0;
	if (NTS_spi.eep_req == 0)
	{
		for(i=0;i<=nts_last_var;i++)
		{
			if (nts_fvars[nts_var_ix[i]] != NTS_spi.eeprom[nts_var_ix[i]])
			{
				NTS_spi.eep_req = 1;			
			}
		}
	}
}

//****************************************************
// Adjust Safety Processor Field Variables
//****************************************************

int16 Adjust_NTS_Parameters (int16 lcd_menu)
{
	int16 i;
	int16 fv_line = 0;
	int16 start_ptr;
	int16 line_ptr;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 var_menu_ptr;
	static int16 var_ptr;
	static int16 mode_sel;
	
	

	if(LCD_Sub_Init == 1)		// first time through procedure
 	{
 	    LCD_Sub_Init = 0;
 	    menu_level = 0;
 	    upd_dpy = 1;
		NTS_spi.eep_req = 1;
		NTS_spi.eep_rcvd = 0;
		timers[tnts_ui] = 0;		
	    LCD_Dig_Point = 0;
 	    LCD_Init = 0;
 	    mode_sel = 0;
		var_menu_ptr = 15;
		var_ptr = 0;
	}
  
#if (Simulator == 1)
	if (timers[tnts_ui] > 3)
	{
		timers[tnts_ui] = 0;
		NTS_spi.eep_rcvd = 1;
	}
#endif

	if ((NTS_spi.eep_rcvd == 1) || (timers[tnts_ui] > 30))
	{			// Display and adjust drive parameters	if (NTS_spi.eep_rcvd == 1)
	
		if ((timers[tnts_ui] > 30) && (NTS_spi.eep_rcvd == 0))
		{
		    upd_dpy = 1;
			NTS_spi.eep_req = 1;
			timers[tnts_ui] = 0;
		}
	
		// Change adjustable variables
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
  		    mode_sel++;
  		    upd_dpy = 1;
  		    if(mode_sel > 1)	// save field new value entered
  		    {
				NTS_spi.eep_req = 2;
				NTS_spi.eep_rcvd = 0;
				timers[tnts_ui] = 0;
				NTS_eep_ix = nts_var_ix[var_ptr];	
				NTS_eep_nmb = 1;	    
				wr_nts_fvar();
	  		    send_galcom_fvar(NTS_CPU,nts_var_ix[var_ptr]);
				mode_sel = 0;
				LCD_Dig_Point = 0;
  		    }
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
  		    if(mode_sel == 0)	// Leave the field var procedure
  		    {
				menu_level = 0;
				upd_dpy = 1;
				lcd_menu = 0;
				return(lcd_menu);
  		    }
  		    if(mode_sel > 0)
  		    {
				upd_dpy = 1;
				LCD_Dig_Point = set_adj_digit(nts_fvars_max[nts_var_ix[var_ptr]],LCD_Dig_Point);			
  		    }
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
  		    UpButton = 1;  // incriment
  		    upd_dpy = 1;
  		    if(mode_sel == 0)
  		    {
				var_ptr--;
				if(var_ptr < 0)
				    var_ptr = nts_last_var;	 // Last fvars # 
  		    }
  		    else	// increment fieldvar
				adjust_variable(&nts_fvars[nts_var_ix[var_ptr]],
						nts_fvars_max[nts_var_ix[var_ptr]],
					nts_fvars_min[nts_var_ix[var_ptr]],1,LCD_Dig_Point);
  		}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
  		    DownButton = 1;  // decriment
  		    upd_dpy = 1;
  		    if(mode_sel == 0)
  		    {
				var_ptr++;
				if(var_ptr > nts_last_var)	  // Last fvars # 
				    var_ptr = 0;
  		    }
  		    else	// decrement fieldvar
				adjust_variable(&nts_fvars[nts_var_ix[var_ptr]],
						nts_fvars_max[nts_var_ix[var_ptr]],
						nts_fvars_min[nts_var_ix[var_ptr]],0,LCD_Dig_Point);
  		}

	    line_ptr = (var_ptr % 3);
		if(mode_sel > 0)
			fv_line = 2;
		else
			fv_line = line_ptr+1;
  	
  		if(upd_dpy == 1)
  		{
		    upd_dpy = 0;
		    
		    start_ptr = var_ptr - line_ptr;
		    
			
 		    for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = getcaps(CAN_Test_Items[var_menu_ptr][i]);
				if(mode_sel > 0)
				{
					LCD_Display[1][i] = NTS_fvars_Min_Max[nts_var_ix[var_ptr]][i];
					LCD_Display[2][i] = NTS_Field_Vars[nts_var_ix[var_ptr]][i];
					LCD_Display[3][i] = ' ';
				}
				else
				{
					LCD_Display[1][i] = NTS_Field_Vars[nts_var_ix[start_ptr]][i];
					if ((start_ptr+1) > nts_last_var)
					{
						LCD_Display[2][i] = ' ';
						LCD_Display[3][i] = ' ';
					}
					else
					{
						LCD_Display[2][i] = NTS_Field_Vars[nts_var_ix[start_ptr+1]][i];
					
						if ((start_ptr+2) > nts_last_var)
							LCD_Display[3][i] = ' ';
						else
							LCD_Display[3][i] = NTS_Field_Vars[nts_var_ix[start_ptr+2]][i];
					}
				}
  		    }

			if(mode_sel > 0)
			{
				Display_Var_Label(var_menu_ptr,nts_var_ix[var_ptr],nts_fvars[nts_var_ix[var_ptr]]);
			}
			for (i=0;i<=2;i++)
			{
				if (mode_sel > 0)
				{
					display_variable(fv_line, 14, nts_fvars_deci[nts_var_ix[var_ptr]], nts_fvars[nts_var_ix[var_ptr]], 1);
					break;		// only show one character when changing value
				}
				else if ((start_ptr+i) <= nts_last_var)
				{
					display_variable(i+1, 14, nts_fvars_deci[nts_var_ix[start_ptr+i]], nts_fvars[nts_var_ix[start_ptr+i]], 1);
				}
			}
			LCD_Flash_TMP[1] = LCD_Display[fv_line][LCD_Dig_Loc];
			if (mode_sel == 0)		// show cursor
			{
				LCD_Display[fv_line][13] = '>';
			}
			
  		}
  		flash_digit(LCD_Flash_TMP[1], fv_line,LCD_Dig_Loc,(mode_sel > 0));
	}
	else
	{
 	    upd_dpy = 1;
 		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
 			menu_level = 0;
			lcd_menu = 0;
			return(lcd_menu);
  		}
  		for(i=0; i<=19; i++)
  		{
  			LCD_Display[1][i] = Wait[i];
  			LCD_Display[2][i] = ' ';
  			LCD_Display[3][i] = ' ';
  		}
	}
	return(lcd_menu);
}


// NTS IO display  --- make an even number of lines all the time
const char NTS_IOS [4][21] = 
{
  "  UN*| UT*|UNo*|UTo*",
  "  DN*| DT*|DNo*|DTo*",
  "  UL*| DZ*|DZA*| DL*",
  " SOK*|DZC*|RDR*|MOD*",
};


int16 Display_NTS_IO (int16 lcd_menu)
{

	int16 i;
	static int16 upd_dpy;
	
	if (LCD_Sub_Init == 1)
	{		// first time through
		LCD_Sub_Init = 0;
  	    upd_dpy = 1;
	}
	
 	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  	{
  	    EnterButton = 1;
  	    upd_dpy = 1;
  	}
  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  	{
  	    ModeButton = 1;  //
		lcd_menu = 0;
		return(lcd_menu);
  	}
  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  	{
  	    UpButton = 1;  // incriment
  	    upd_dpy = 1;
  	}
  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  	{
  	    DownButton = 1;  // decriment
  	    upd_dpy = 1;
  	}

	if (upd_dpy == 1)
	{
		upd_dpy = 0;
		for(i=0; i<=19; i++)
		{
		    LCD_Display[0][i] = NTS_IOS[0][i];
			LCD_Display[1][i] = NTS_IOS[1][i];
		    LCD_Display[2][i] = NTS_IOS[2][i];
			LCD_Display[3][i] = NTS_IOS[3][i];
		}
		
	}

	//       4    9   14   19 
	//  "  UN*| UT*|UNo*|UTo*",
	//  "  DN*| DT*|DNo*|DTo*",
	//  "  UL*| DZ*|DZA*| DL*",
  	//  " SOK*|DZC*|RDR*|MOD*",

	if ((NTS_spi.limits & 0x01) != 0)	// UN
		LCD_Display[0][4] = io_on;
	else
		LCD_Display[0][4] = io_off;
	
	if ((NTS_spi.limits & 0x02) != 0)	// UT
		LCD_Display[0][9] = io_on;
	else
		LCD_Display[0][9] = io_off;
		
	if (NTS_spi.Status2.B.UNo == 1)	// UNo
		LCD_Display[0][14] = io_on;
	else
		LCD_Display[0][14] = io_off;
	
	if (NTS_spi.Status2.B.UTo == 1)	// UTo
		LCD_Display[0][19] = io_on;
	else
		LCD_Display[0][19] = io_off;
		
		
	if ((NTS_spi.limits & 0x10) != 0)	// DN
		LCD_Display[1][4] = io_on;
	else
		LCD_Display[1][4] = io_off;
		
	if ((NTS_spi.limits & 0x20) != 0)	// DT
		LCD_Display[1][9] = io_on;
	else
		LCD_Display[1][9] = io_off;
		
	if (NTS_spi.Status2.B.DNo == 1)	// DNo
		LCD_Display[1][14] = io_on;
	else
		LCD_Display[1][14] = io_off;
		
	if (NTS_spi.Status2.B.DTo == 1)	// DTo
		LCD_Display[1][19] = io_on;
	else
		LCD_Display[1][19] = io_off;
		
		
		
	if ((NTS_spi.doorzone & 0x01) != 0)	// UL
		LCD_Display[2][4] = io_on;
	else
		LCD_Display[2][4] = io_off;
		
	if ((NTS_spi.doorzone & 0x02) != 0)	// DZ
		LCD_Display[2][9] = io_on;
	else
		LCD_Display[2][9] = io_off;
		
	if ((NTS_spi.doorzone & 0x04) != 0)	// DZA
		LCD_Display[2][14] = io_on;
	else
		LCD_Display[2][14] = io_off;
		
	if ((NTS_spi.doorzone & 0x08) != 0)	// DL
		LCD_Display[2][19] = io_on;
	else
		LCD_Display[2][19] = io_off;
		
		
		
	if (NTS_spi.Status1.B.sel_ok == 1)		// sel ok
		LCD_Display[3][4] = io_on;
	else
		LCD_Display[3][4] = io_off;
		
	if ((NTS_spi.doorzone & 0x10) != 0)		// DZclip
		LCD_Display[3][9] = io_on;
	else
		LCD_Display[3][9] = io_off;
		
	if (NTS_spi.Status2.B.rear_door == 1)	// Rear Door Jumper
		LCD_Display[3][14] = io_on;
	else
		LCD_Display[3][14] = io_off;
		
	if (NTS_spi.Status2.B.setup == 1)		// Setup Jumper
		LCD_Display[3][19] = io_on;
	else
		LCD_Display[3][19] = io_off;
		
	return(lcd_menu);
}

/*
	Revision History:
	
11/10/14  V7.1.20 mhd	1. When field variable changed, send data to galcom device.

*/