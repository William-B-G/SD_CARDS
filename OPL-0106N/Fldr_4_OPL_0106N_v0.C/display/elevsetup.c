// Elevator Setup routine
#define d_ELEVSETUP 1
#include "global.h"

int16 Elevator_Setup (int16 lcd_menu);
int16 hwl_number_valid_fl (void);
int16 APS_Sel_HW_Learn (int16 lcd_menu);
int16 Gov_Set_Reset (int16 lcd_menu);

#pragma section const_type

#define Num_Elev_Setup_Menus 6
const char LCD_Elev_Setup[Num_Elev_Setup_Menus][21] = {
  " Learn Hoistway     ",	// 0
  " Reset/Trip Governor",   // 1
  " Reset Low Oil/Stall",   // 2
  " Reset Hot Oil      ",   // 3
  " Open/Close F Door  ",   // 4
  " Open/Close R Door  ",	// 5
};

#define Last_Elev_Setup_Menu 6            

const char Learn_Hw_CED[34][21] = {
  "Hoistway Table Setup",	// 0
  "Place Car on Inspect",	// 1
  " Enter to Continue  ",	// 2
  "or Hit Mode to Abort",	// 3
  "APS SEL Adj Variable",	// 4
  "Number Valid Flr Err",	// 5
  "Number limits Error ",	// 6
  "  Top Speed Error   ",	// 7
  "  First Time Setup  ",	// 8
  " or Clear Hoistway  ",	// 9
  "  Table <U/D>? No   ",	// 10
  "Setup Active. Hit Up",	// 11
  "or Dn to Scroll thru",	// 12
  "  Instructions or   ",	// 13
  "Press Mode to Abort:",	// 14
  "1.Place car level at",	// 15
  " floor.  2.Select   ",	// 16
  "position on display.",	// 17
  "3. Press Enter to   ",	// 18
  "record floor count. ",	// 19
  "If Ready to Continue",	// 20
  "  then Hit Enter    ",	// 21
  "    Machine Room    ",	// 22
  "   Hoistway Setup   ",	// 23
  "      Aborted       ",	// 24
  "Hit Up or Dn Button ",	// 25
  "to Select floor:    ",	// 26
  " Then Hit Enter to  ",	// 27
  " Record Floor Count.",	// 28
  "  Recording Floor   ",	// 29
  " Count. Waiting for ",	// 30
  "    Response ...    ",	// 31
  "Floor Count Recorded",	// 32
  "  Setup Complete    ",	// 33
};


static  const char gov_dpy[5][21] = {
  "   Reset Governor   ",	// 0
  "   Trip Governor    ",	// 1
  "  Up, Dn or Enter   ",	// 2
  "Button is Stuck. Hit",	// 3
  "Mode then try again.",	// 4
};

static  const char gov_rset_dpy[7][21] = {
  "Hit Enter+UP Button ",	// 0
  " to reset Governor  ",	// 1
  "  GOV reset output  ",	// 2
  "    is active for   ",	// 3
  "   momentary pulse  ",	// 4
  " Governor is Reset  ",	// 5
  "to pulse GOVR output",	// 6
};

static  const char gov_trip_dpy[10][21] = {
  "  Governor is Reset ",	// 0
  "Hit Enter+DN Button ",	// 1
  "  to trip Governor  ",	// 2
  "Governor is Tripped ",	// 3
  "pick GOV trip output",	// 4
  "GOV trip active for ",	// 5
  " 15 seconds. Release",	// 6
  "   Buttons to drop  ",	// 7
  " GOV trip hold has  ",	// 8
  "     timed-out      ",	// 9
};


static  const char open_close_fdoor_dpy[2][21] = {
  "Hit UP to open Door ",
  "Hit DN to close Door",
};

static  const char open_close_rdoor_dpy[2][21] = {
  "Hit UP to open Door ",
  "Hit DN to close Door",
};

static  const char open_fault_dpy[2][21] = {
  "   Car must be on   ",
  "Inspect or Safeties ",
};

static  const char door_zone_dpy[2][21] = {
  "   Car must be in   ",
  "     Door Zone      ",
};


static  const char low_oil_dpy[12][21] = {
  "Hit Ent+UP Button to",	// 0
  "reset Low Oil/Stall ",	// 1
  " Low Oil Operation  ",	// 2
  "      is Reset      ",	// 3
  "   Reset Low Oil    ",   // 4
  "   Car must be on   ",	// 5
  "     Inspection     ",	// 6
  "   Low Oil Switch   ",	// 7
  "    must be off     ",	// 8
  " Up or Enter Button ",	// 9
  " is Stuck on.  Hit  ",	// 10
  "Mode then try again.",	// 11
};

static  const char hot_oil_dpy[12][21] = {
  "Hit Enter+UP Button ",	// 0
  "  to reset Hot Oil  ",	// 1
  " Hot Oil Operation  ",	// 2
  "      is Reset      ",	// 3
  "   Reset Hot Oil    ",   // 4
  "   Car must be on   ",	// 5
  "     Inspection     ",	// 6
  "   Hot Oil Switch   ",	// 7
  "    must be off     ",	// 8
  " Up or Enter Button ",	// 9
  " is Stuck on.  Hit  ",	// 10
  "Mode then try again.",	// 11
};


#pragma section all_types

int16 brake_timeout = 0;
static int16 line_dpy;
int16 dir_select = 0;
int16 lw_setup_type = 0;
int16 zero_hoistway = 0;

int16 Elevator_Setup (int16 lcd_menu)
{
	int16 i,j;
	int16 start_ptr;
	int16 line_ptr;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 last_menu;
	static int16 op_state;

	if (LCD_Init == 1)
	{
		LCD_Init = 0; 
		upd_dpy = 1;
		menu_level = 0;
		menu_ptr = 0;
		j = 0;
 	    for (i=0;i<Num_Elev_Setup_Menus;i++)
 	    {
			//  "   Learn Hoistway   ",	  // 0
			//  " Reset/Trip Governor",   // 1
			//  "   Reset Low Oil    ",   // 2
			//  "   Reset Hot Oil    ",   // 3
			//  "Open/Close Frnt Door",   // 4
			//  "Open/Close Rear Door",	  // 5

			if ((cons[GOV_Reset] == 0) && (i == 1))
				i = 2;
			if ((cons[rear] == 0) && (i == 5))
				i = 6;
			
			if (i >= Num_Elev_Setup_Menus)
				break;  // Have to break here if the last menu is not used
			else
			{
	 	    	Menu_list[j] = (int8)i;	
	 	    	j++;
			}
 	    }
 	    
 	    last_menu = j - 1;
 	    for (i=j;i<Num_Elev_Setup_Menus;i++)
 	    	Menu_list[i] = Num_Elev_Setup_Menus;
 	    	
  	}

	if (menu_level == 0)
	{			// Rotate through setup menu items
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
			  menu_ptr = last_menu;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
		    if(menu_ptr > last_menu)
			  menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	menu_level = (int16)(Menu_list[menu_ptr] + 1);
  		    LCD_Sub_Init = 1;
		  	return(lcd_menu);
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
	  		line_ptr = menu_ptr % 3;
	  		start_ptr = menu_ptr - line_ptr;
	  	    for(i=0; i<=19; i++)
	  	    {
	  			LCD_Display[0][i] = LCD_Main_Menu[8][i];
				LCD_Display[1][i] = LCD_Elev_Setup[Menu_list[start_ptr]][i];
				if ((start_ptr + 1) > last_menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else
			  	{
					LCD_Display[2][i] = LCD_Elev_Setup[Menu_list[start_ptr+1]][i];
					if ((start_ptr + 2) > last_menu)
				  		LCD_Display[3][i] = ' ';
					else
						LCD_Display[3][i] = LCD_Elev_Setup[Menu_list[start_ptr+2]][i];
			  	}
	  	    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}

	if (menu_level == 1)
	{		// Learn Hoistway
		menu_level = APS_Sel_HW_Learn(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 2)
	{
		menu_level = Gov_Set_Reset(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 3)
	{
		if(LCD_Sub_Init == 1)	// first time through low oil reset
		{
			LCD_Sub_Init = 0;
			if ((rdLCDio(Lcd_UP) == 1) || (rdLCDio(Lcd_ENTER) == 1))
				op_state = 1;
			else
				op_state = 0;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
		    menu_level = 0;
			upd_dpy = 1;
		    return(lcd_menu);
		}

		if (op_state == 1)
		{
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[2][i];  
			    LCD_Display[1][i] = low_oil_dpy[9][i];  
			    LCD_Display[2][i] = low_oil_dpy[10][i];
			    LCD_Display[3][i] = low_oil_dpy[11][i];  
			}
		}
		else if (op_state == 0)
		{
			
			if (stallf == 0)
			{
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = LCD_Elev_Setup[2][i];  
				    LCD_Display[1][i] = low_oil_dpy[2][i];  
				    LCD_Display[2][i] = low_oil_dpy[3][i];
				    LCD_Display[3][i] = ' ';  
				}
			}
			else if(chk_ins() == 1)
			{
				if ((cons[Low_Oil] == 0) || 
					(((rdinp(i_LOS) == 0) && (fvars[fvlos] == 0)) || 
					((rdinp(i_LOS) == 1) && (fvars[fvlos] == 1))))
				{
				  	if ((rdLCDio(Lcd_ENTER) == 1) && (EnterButton == 2) && (rdLCDio(Lcd_UP) == 1) && (UpButton == 2))
				  	{			// Press the enter button to reset LOW OIL
						stallf = 0;
						procf = 2;
					}
					else
					{
						for(i=0; i<=19; i++)
						{
						    LCD_Display[0][i] = LCD_Elev_Setup[2][i];  
						    LCD_Display[1][i] = low_oil_dpy[0][i];  
						    LCD_Display[2][i] = low_oil_dpy[1][i];
						    LCD_Display[3][i] = ' ';  
						}
					}
				}
				else
				{
					for(i=0; i<=19; i++)
					{
					    LCD_Display[0][i] = LCD_Elev_Setup[2][i];  
					    LCD_Display[1][i] = low_oil_dpy[7][i];  
					    LCD_Display[2][i] = low_oil_dpy[8][i];
					    LCD_Display[3][i] = ' ';  
					}
				}
			}
			else
			{
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = LCD_Elev_Setup[2][i];  
				    LCD_Display[1][i] = low_oil_dpy[5][i];  
				    LCD_Display[2][i] = low_oil_dpy[6][i];
				    LCD_Display[3][i] = ' ';  
				}
			}
		}
		
	}

	if (menu_level == 4)
	{
		if(LCD_Sub_Init == 1)	// first time through hot oil reset
		{
			LCD_Sub_Init = 0;
			if ((rdLCDio(Lcd_UP) == 1) || (rdLCDio(Lcd_ENTER) == 1))
				op_state = 1;
			else
				op_state = 0;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
		    menu_level = 0;
			upd_dpy = 1;
		    return(lcd_menu);
		}

		if (op_state == 1)
		{
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[3][i];  
			    LCD_Display[1][i] = hot_oil_dpy[9][i];  
			    LCD_Display[2][i] = hot_oil_dpy[10][i];
			    LCD_Display[3][i] = hot_oil_dpy[11][i];  
			}
		}
		else if (op_state == 0)
		{
			
			if (hotoilf == 0)
			{
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = LCD_Elev_Setup[3][i];  
				    LCD_Display[1][i] = low_oil_dpy[2][i];  
				    LCD_Display[2][i] = low_oil_dpy[3][i];
				    LCD_Display[3][i] = ' ';  
				}
			}
			else if(chk_ins() == 1)
			{
				if ((cons[hotoilSW] == 0) ||
					((cons[hotoilSW] == 1) && (rdinp(i_TPH) == 0)) || // TPH input is OK
					((cons[hotoilSW] == 2) && (rdinp(i_TPH) == 1) && (rdinp(i_CEN) == 1)))	 // TPH input is OK
				{
				  	if ((rdLCDio(Lcd_ENTER) == 1) && (EnterButton == 2) && (rdLCDio(Lcd_UP) == 1) && (UpButton == 2))
				  	{			// Press the enter button to reset HOT OIL
						hotoilf = 0;
						procf = 2;
					}
					else
					{
						for(i=0; i<=19; i++)
						{
						    LCD_Display[0][i] = LCD_Elev_Setup[3][i];  
						    LCD_Display[1][i] = hot_oil_dpy[0][i];  
						    LCD_Display[2][i] = hot_oil_dpy[1][i];
						    LCD_Display[3][i] = ' ';  
						}
					}
				}
				else
				{
					for(i=0; i<=19; i++)
					{
					    LCD_Display[0][i] = LCD_Elev_Setup[3][i];  
					    LCD_Display[1][i] = hot_oil_dpy[7][i];  
					    LCD_Display[2][i] = hot_oil_dpy[8][i];
					    LCD_Display[3][i] = ' ';  
					}
				}
			}
			else
			{
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = LCD_Elev_Setup[3][i];  
				    LCD_Display[1][i] = hot_oil_dpy[5][i];  
				    LCD_Display[2][i] = hot_oil_dpy[6][i];
				    LCD_Display[3][i] = ' ';  
				}
			}
		}
	}

	if (menu_level == 5)
	{
		if(LCD_Sub_Init == 1)	// first time through open front door on inspection
		{
			LCD_Sub_Init = 0;
			ins_doorf = 0;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
		    menu_level = 0;
			upd_dpy = 1;
		    return(lcd_menu);
		}

		if (chk_ins() == 0)
		{
			ins_doorf = 0;
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[4][i];  
			    LCD_Display[1][i] = open_fault_dpy[0][i];  
			    LCD_Display[2][i] = open_fault_dpy[1][i];
			    LCD_Display[3][i] = ' ';  
			}
		}
		else if (rdinp(i_DZ) == 0)	
		{
			ins_doorf = 0;
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[4][i];  
			    LCD_Display[1][i] = door_zone_dpy[0][i];  
			    LCD_Display[2][i] = door_zone_dpy[1][i];
			    LCD_Display[3][i] = ' ';  
			}
		}
		else
		{
			
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[4][i];  
			    LCD_Display[1][i] = open_close_fdoor_dpy[0][i];  
			    LCD_Display[2][i] = open_close_fdoor_dpy[1][i];
			    LCD_Display[3][i] = ' ';  
			}

			if ((rdLCDio(Lcd_UP) == 1) && (UpButton == 2))
				ins_doorf = 1;			

			if ((rdLCDio(Lcd_DN) == 1) && (DownButton == 2))
				ins_doorf = 3;			
		
			if ((rdLCDio(Lcd_DN) == 1) && (rdLCDio(Lcd_UP) == 1)) 
				ins_doorf = 0;
		}
	}

	if (menu_level == 6)
	{
		if(LCD_Sub_Init == 1)	// first time through open rear door on inspection
		{
			LCD_Sub_Init = 0;
			ins_rdoorf = 0;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
		    menu_level = 0;
			upd_dpy = 1;
		    return(lcd_menu);
		}

		if (chk_ins() == 0)
		{
			ins_doorf = 0;
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[5][i];  
			    LCD_Display[1][i] = open_fault_dpy[0][i];  
			    LCD_Display[2][i] = open_fault_dpy[1][i];
			    LCD_Display[3][i] = ' ';  
			}
		}
		else if (rdinp(i_DZ) == 0)	
		{
			ins_doorf = 0;
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[5][i];  
			    LCD_Display[1][i] = door_zone_dpy[0][i];  
			    LCD_Display[2][i] = door_zone_dpy[1][i];
			    LCD_Display[3][i] = ' ';  
			}
		}
		else
		{

			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[5][i];  
			    LCD_Display[1][i] = open_close_rdoor_dpy[0][i];  
			    LCD_Display[2][i] = open_close_rdoor_dpy[1][i];
			    LCD_Display[3][i] = ' ';  
			}

			if ((rdLCDio(Lcd_UP) == 1) && (UpButton == 2))
				ins_rdoorf = 1;			

			if ((rdLCDio(Lcd_DN) == 1) && (DownButton == 2))
				ins_rdoorf = 3;			
		
			if ((rdLCDio(Lcd_DN) == 1) && (rdLCDio(Lcd_UP) == 1))
				ins_rdoorf = 0;
		}
	}
	
	// make sure all buttons status are properly reset
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	    UpButton = 1;  
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	    DownButton = 1;  
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	    EnterButton = 1;
	
    return(lcd_menu);
}


int16 hwl_number_valid_fl (void)
{
	int16 i;
	int16 j = 0;
	
	for(i=1; i <= cons[topf]; i++) 
	{
		if (valid_fl[i] == 1)
		{
			j++;
	  	}
	}
	return j;
}


int16 APS_Sel_HW_Learn (int16 lcd_menu)
{
	int16 i;
	static int16 upd_dpy;
	static int16 aps_sel_state;
	static int16 line_dpy;

	if (PasswordFlag == false)
   	{
  		StoreLCDInfo();
		clrLCDdpy();
  		LCD_Menu = 25;
  		LCD_Init = 1;
		return(lcd_menu);
   	}
  	if(LCD_Sub_Init == 1)	// first time through Inputs and Outputs
  	{
  	    upd_dpy = 1;
  	    LCD_Sub_Init = 0;
  	    aps_sel_state = 0;
		line_dpy = 0;
		hwl_mode = 0;
		hwl_zero_hoistway = 0;
		hwl_mode_car = 0;
		hwl_floor = cons[bottomf];
    	zero_hoistway = 0;
		APS_Hoistway_Unmark_Floor();
		NTS_spi.eep_req = 1;
		NTS_spi.eep_rcvd = 0;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
		lcd_menu = 0;
		hwl_zero_hoistway = 0;
	    hwl_mode = 0;
	    hwl_mode_car = 0;
		APS_Hoistway_Unmark_Floor();
		return(lcd_menu);
	}
	if(aps_sel_state == 0)    // Check if okay to continue
	{
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{		 // placed here so the update flag sets the display in LCD_Sub_Point == 2
			EnterButton = 1;
			aps_sel_state = 1;
			hwl_num_floors = hwl_number_valid_fl();
			hwl_num_limits = 1;
			hwl_top_speed = cons[speed];
			return(lcd_menu);
		}
		else
		{
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[0][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[1][i];  
			    LCD_Display[2][i] = Learn_Hw_CED[2][i];
			    LCD_Display[3][i] = Learn_Hw_CED[3][i];  
			}
		}
	}
	if(aps_sel_state == 1)    // checking to make sure it is on INS
	{
		if (hwl_num_floors != nts_fvars[nts_fvnum_floors])
		{
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[0][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[4][i];  
			    LCD_Display[2][i] = Learn_Hw_CED[5][i];	// Number of floors error
			    LCD_Display[3][i] = ' ';  
			}
		}
		else if (hwl_num_limits != nts_fvars[nts_fvnum_limits])
		{
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[0][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[4][i];  
			    LCD_Display[2][i] = Learn_Hw_CED[6][i];	// Number of Limits error
			    LCD_Display[3][i] = ' ';  
			}
		}
		else if (hwl_top_speed != nts_fvars[nts_fvtop_speed])
		{
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[0][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[4][i];  
			    LCD_Display[2][i] = Learn_Hw_CED[7][i];	// Top Speed error
			    LCD_Display[3][i] = ' ';  
			}
		}
  		else if(chk_ins() == 0)
	    {
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[0][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[1][i]; // Put elevetor on Inspection 
			    LCD_Display[2][i] = Learn_Hw_CED[2][i];	
			    LCD_Display[3][i] = Learn_Hw_CED[3][i];  
			}
	    }
	    else
	    {
		    upd_dpy = 1;	 // Update display
			aps_sel_state = 2;
			hwl_mode = 1;
	    }
	}
	if(aps_sel_state == 2)    // checking for first time to zero hoistway table
	{
  		if(chk_ins() == 0)
	    {
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[0][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[1][i]; // Put elevetor on Inspection 
			    LCD_Display[2][i] = Learn_Hw_CED[2][i];	
			    LCD_Display[3][i] = Learn_Hw_CED[3][i];  
			}
	    }
	    else
	    {
	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	  		    EnterButton = 1;
				upd_dpy = 1;
				if (zero_hoistway == 1)
				{
					hwl_zero_hoistway = 1;
					zero_hoistway_table();
				}
				aps_sel_state = 3;
				return(lcd_menu);
	  		}
	  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  		{
	  		    UpButton = 1;  // incriment
	  		    upd_dpy = 1;
	  		    if (zero_hoistway == 1)
	  		    	zero_hoistway = 0;
	  		    else
	  		    	zero_hoistway = 1;
	  		}
	  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  		{
	  		    DownButton = 1;  // decriment
	  		    upd_dpy = 1;
	  		    if (zero_hoistway == 1)
	  		    	zero_hoistway = 0;
	  		    else
	  		    	zero_hoistway = 1;
	  		}
	  		if(upd_dpy == 1)
	  		{
	  		   	upd_dpy = 0;
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = Learn_Hw_CED[0][i];  
				    LCD_Display[1][i] = Learn_Hw_CED[8][i];  
				    LCD_Display[2][i] = Learn_Hw_CED[9][i];	// Put elevator on inspection
				    LCD_Display[3][i] = Learn_Hw_CED[10][i];  
				}
				if (zero_hoistway == 1)
				{
					LCD_Display[3][15] = 'Y';	
					LCD_Display[3][16] = 'e';	
					LCD_Display[3][17] = 's';	
				}
				else
				{
					LCD_Display[3][15] = 'N';	
					LCD_Display[3][16] = 'o';	
					LCD_Display[3][17] = ' ';	
				}
	  		}
	    }
	}
	if(aps_sel_state == 3)    // Scroll Display
	{
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;	 // Update display
			line_dpy++;
		    if (line_dpy > 2)
		    	line_dpy = 0;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decrement
  		    upd_dpy = 1;
			line_dpy--;
		    if (line_dpy < 0)
		    	line_dpy = 2;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{		 
			EnterButton = 1;
		    upd_dpy = 1;	 // Update display
			line_dpy = 0;
			aps_sel_state = 4;
  		    hwl_floor = cons[bottomf];
			return(lcd_menu);
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
		    if (line_dpy == 0)
		    {
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = Learn_Hw_CED[11][i];  
				    LCD_Display[1][i] = Learn_Hw_CED[12][i];  
				    LCD_Display[2][i] = Learn_Hw_CED[13][i];	
				    LCD_Display[3][i] = Learn_Hw_CED[14][i];  
				}
		    }
			else if (line_dpy == 1)
			{
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = Learn_Hw_CED[15][i];  
				    LCD_Display[1][i] = Learn_Hw_CED[16][i];  
				    LCD_Display[2][i] = Learn_Hw_CED[17][i];	
				    LCD_Display[3][i] = Learn_Hw_CED[18][i];  
				}
			}
			else if (line_dpy == 2)
			{
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = Learn_Hw_CED[19][i];  
				    LCD_Display[1][i] = Learn_Hw_CED[20][i];  
				    LCD_Display[2][i] = Learn_Hw_CED[21][i];	
				    LCD_Display[3][i] = ' ';  
				}
			}
		}
	}
	if(aps_sel_state == 4)    // Hoistway setup from the machine room
	{
  		if ((chk_ins() == 0) || (hwl_mode == 0))
	    {
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[22][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[23][i];  
			    LCD_Display[2][i] = Learn_Hw_CED[24][i];	
			    LCD_Display[3][i] = ' ';  
			}
	    	aps_sel_state = 6;			// abort
			timers[tautolnhw] = 0;
	    }
	    else if (hwl_mode == 1)
	    {
	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	  		    EnterButton = 1;
  			    upd_dpy = 1;
				hwl_mark = 1;
				hwl_fl_update = 0;
		    	aps_sel_state = 5;			// store for return to correct place
				timers[tautolnhw] = 0;
				APS_Hoistway_Mark_Floor();
				return(lcd_menu);
	  		}
	  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  		{
	  		    UpButton = 1;  // incriment
  			    upd_dpy = 1;
	  		    hwl_floor++;
	  		    if(hwl_floor > cons[topf])
					hwl_floor = cons[bottomf];
	  		}
	  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  		{
	  		    DownButton = 1;  // decriment
  			    upd_dpy = 1;
	  		    hwl_floor--;
	  		    if(hwl_floor < cons[bottomf])
					hwl_floor = cons[topf];
	  		}
	  		if(upd_dpy == 1)
	  		{
	  		   	upd_dpy = 0;
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = Learn_Hw_CED[25][i];  
				    LCD_Display[1][i] = Learn_Hw_CED[26][i];  
				    LCD_Display[2][i] = Learn_Hw_CED[27][i];	
				    LCD_Display[3][i] = Learn_Hw_CED[28][i];  
				}
	  		   	LCD_Display[1][17] = flmrk[hwl_floor][0];
	  		   	LCD_Display[1][18] = flmrk[hwl_floor][1];
	  		}
  		}
	}
	if(aps_sel_state == 5)    // Waiting for response from Selector Board
	{
		if ((hwl_mark == 1) && (hwl_fl_update == 1)) 
		{
			APS_Hoistway_Unmark_Floor();
			if (verify_floor_table() == 0)		// non-zer value is a fault 
			{
				Set_Invalid_Floors();
				Set_Limit_Table();
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
		else if ((hwl_mark == 1) && (hwl_fl_update == 0))
		{		// waiting for response from selector board
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[29][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[30][i];  
			    LCD_Display[2][i] = Learn_Hw_CED[31][i];	
			    LCD_Display[3][i] = ' ';  
			}
		}
		else 
		{		// floor has been marked
		
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = Learn_Hw_CED[0][i];  
			    LCD_Display[1][i] = Learn_Hw_CED[32][i];  
				if (Hoistway_Learned == 1)
			  	  LCD_Display[2][i] = Learn_Hw_CED[33][i]; 
				else
			  	  LCD_Display[2][i] = ' ';	
			    LCD_Display[3][i] = ' ';  
			}
		}

		if (timers[tautolnhw] > 50)
		{
			APS_Hoistway_Unmark_Floor();
			aps_sel_state = 4;	
		}
	}
	if(aps_sel_state == 6)    // output screen during abort
	{
		if (timers[tautolnhw] > 20)
		{
			aps_sel_state = 0;	
  	   		LCD_Sub_Init = 1;	// reset setup since aborted
		}
	}
	
	return(lcd_menu);
}


int16 Gov_Set_Reset (int16 lcd_menu)
{
	int16 i;
	static int16 upd_dpy;
	static int16 gov_set_reset_state;
	static int16 menu_ptr;
	
	if(LCD_Sub_Init == 1)	// first time through governor reset
	{
		govrstf = 0;
		govtripf = 0;
	    LCD_Sub_Init = 0;
		timers[tgovrst] = 0;
		timers[tgovtrip] = 0;
		upd_dpy = 1;	 // Update display
		if ((rdLCDio(Lcd_UP) == 1) || (rdLCDio(Lcd_DN) == 1) || (rdLCDio(Lcd_ENTER) == 1))
			gov_set_reset_state = 3;
		else
			gov_set_reset_state = 0;		
	}
	
	if (gov_set_reset_state == 0)
	{
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
		    ModeButton = 1;  
			govrstf = 0;
			govtripf = 0;
		    lcd_menu = 0;
		  	return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
				menu_ptr = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
	    	if(menu_ptr > 1)
				menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	gov_set_reset_state = menu_ptr + 1;
		  	LCD_Sub_Init = 1;
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = LCD_Elev_Setup[1][i];
			  	LCD_Display[1][i] = gov_dpy[0][i];
			  	LCD_Display[2][i] = gov_dpy[1][i];
			  	LCD_Display[3][i] = ' ';
		    }
			LCD_Display[menu_ptr+1][0] = '>';		// show cursor
		}
	}
	
	if (gov_set_reset_state == 1)
	{			// Governor Reset
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			govrstf = 0;
		    ModeButton = 1;  
			gov_set_reset_state = 0;
		    return(lcd_menu);
		}

	  	if ((rdLCDio(Lcd_ENTER) == 0) && (rdLCDio(Lcd_UP) == 0))
	  	{			// Press the enter button to reset governor
			EnterButton = 1;
			UpButton = 1;

			if (govrstf == 0)
				timers[tgovrst] = 0;
			govrstf = 1;
				
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = gov_dpy[0][i];
			  	LCD_Display[1][i] = gov_rset_dpy[2][i];
			  	LCD_Display[2][i] = gov_rset_dpy[3][i];
			  	LCD_Display[3][i] = gov_rset_dpy[4][i];
		    }
		}
		else
		{
			if (timers[tgovrst] > 30)
				govrstf = 0;
			if (rdinp(i_GOV) == 0)
			{
			    for(i=0; i<=19; i++)
			    {
					LCD_Display[0][i] = gov_dpy[0][i];
				  	LCD_Display[1][i] = gov_rset_dpy[0][i];
				  	LCD_Display[2][i] = gov_rset_dpy[1][i];
				  	LCD_Display[3][i] = ' ';
			    }
			}
			else
			{
			    for(i=0; i<=19; i++)
			    {
					LCD_Display[0][i] = gov_dpy[0][i];
				  	LCD_Display[1][i] = gov_rset_dpy[5][i];
				  	LCD_Display[2][i] = gov_rset_dpy[0][i];
			  		LCD_Display[3][i] = gov_rset_dpy[6][i];
			    }
			}
		}
	}

	if (gov_set_reset_state == 2)
	{
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			govtripf = 0;
		    ModeButton = 1;  
			gov_set_reset_state = 0;
		    return(lcd_menu);
		}
	  	if ((rdLCDio(Lcd_ENTER) == 0) && (rdLCDio(Lcd_DN) == 0))
	  	{			// Press the enter button to trip governor
			EnterButton = 1;
			DownButton = 1;

			if (govtripf == 0)
				timers[tgovtrip] = 0;
			if (govtripf != 2)				
				govtripf = 1;
					
		}
		else
		{
			if (govtripf == 1)
				govtripf = 0;		// button released before time-out
			if ((timers[tgovtrip] > 180) && (govtripf == 2))
				govtripf = 0;
		}
		if (govtripf == 0)
		{
			if (rdinp(i_GOV) == 1)
			{
			    for(i=0; i<=19; i++)
			    {
					LCD_Display[0][i] = gov_dpy[1][i];
				  	LCD_Display[1][i] = gov_trip_dpy[0][i];
				  	LCD_Display[2][i] = gov_trip_dpy[1][i];
				  	LCD_Display[3][i] = gov_trip_dpy[2][i];
			    }
			}
			else
			{
			    for(i=0; i<=19; i++)
			    {
					LCD_Display[0][i] = gov_dpy[1][i];
				  	LCD_Display[1][i] = gov_trip_dpy[3][i];
				  	LCD_Display[2][i] = gov_trip_dpy[1][i];
				  	LCD_Display[3][i] = gov_trip_dpy[4][i];
			    }
			}
		}
		else if (govtripf == 1)
		{
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = gov_dpy[1][i];
			  	LCD_Display[1][i] = gov_trip_dpy[5][i];
			  	LCD_Display[2][i] = gov_trip_dpy[6][i];
			  	LCD_Display[3][i] = gov_trip_dpy[7][i];
		    }
		}
		else if (govtripf == 2)
		{				  // else trip timed-out  15 sec + 3 seconds for display
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = gov_dpy[1][i];
			  	LCD_Display[1][i] = gov_trip_dpy[8][i];
			  	LCD_Display[2][i] = gov_trip_dpy[9][i];
			  	LCD_Display[3][i] = ' ';
		    }
		}
	}
	
	if (gov_set_reset_state == 3)
	{
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			govrstf = 0;
			govtripf = 0;
			gov_set_reset_state = 0;
		  	return(lcd_menu);
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			// Stuck button
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = LCD_Elev_Setup[1][i];  
			    LCD_Display[1][i] = gov_dpy[2][i];  
			    LCD_Display[2][i] = gov_dpy[3][i];
			    LCD_Display[3][i] = gov_dpy[4][i];  
			}
		}
	}
	return(lcd_menu);
}

/* Revision History

9/12/13 v7.0.19 mhd		1. Seperated Elevator_Setup from lcd.c.
						2, Added NTS and ETS test to ignore limits.
11/15/13 v7.0.28 mhd	1. Calculate floors to high speed to run buffer and limit tests
04/10/14 v7.0.42 mhd	1. Modified overspeed test to set car call and run to top or bottom.
						2. Deleted overspeed brake hold..
5/28/14 v7.1.1 mhd		1. Corrected index for LCD_Elev_Setup menu when fixed values were used.
6/23/14 v7.1.4 mhd		1. Fixed Menu_level value check for buffer test and nts/ets limit test.
10/23/14 v7.1.18 mhd	1. Allow overspeed test when on independent.
4/8/15 v7.1.30 mhd		1. Added Trip NTSD on NTS processor from LCD interface.
						2. Added error check for load weigher to be out of range during setup.
5/27/15 v7.1.32 mhd		1. Made correction to calculating which floor to run test from.  If calc_floor_to_Top_Speed() returns zero, the distance too far so
						   set value to top floor - 1 to run down and bottom floor + 1 to run up. 
						2. Added direction to last check to start limit test and added car/cwt buffer test.
6/3/15 					3. Change the direction selection for normal and emergency limit test to default in the up direction.
6/15/15 v7,1,34 mhd		1. In NTS and ETS Test, allow mode button if test finished and EST_Test and NTS_Test equal zero.  Otherwise screen get locked-up.
7/21/15 v7.1.40 mhd		1. Modified Lift brake on inspection and PWL brake lift to work with changes to galaxy 4.
						2. Set NTS_can.cmd_req = 1 when setting emergency slowdown flag (ntsc_Set_EM_SD) for NTS processor.
9/17/15 V7.1.41 mhd		1. Added earthquake reset to display.  Moved open/close door on inspection, hot oil and low oil menu items below earthquake.
12/22/15 V7.1.55 mhd	1. Corrected Safety check logic for power loss brake: ((rdinp(i_SFC) == 1) || (rdinp(i_EMP) == 0)) instead of
						   ((rdinp(i_SFC) == 0) || (rdinp(i_EMP) == 1)).
*/