//  Elevator Automatic Learn Hoistway routines

#define d_BKTEST 1
#include "global.h"

void Brake_Test(void);
void bktest_run_cmd(int16 cmd);
void Run_Brake_Test (void);
void Auto_Brake_Test (int16 cmd);


int16 bkt_cmd = 0;			// Brake command
int16 bkt_auto_test = 0;	// Brake auto test variable (used in LCD)
int16 bkt_test_seq = 0;		// Brake test sequence
int16 bkt_run_cmd = 0;		// Brake run command: 1 to run up and 2 to run down
int16 bkt_run_err = 0;		// Error occured during Brake run
int16 bkt_sd = 0;			// Brake test slowdown
float bkt_run_vel = 0;		// Brake run velocity
float bkt_max_speed = 0;	// Brake max speed
int16 bkt_dir_select = 0;	// direction selection
int16 bkt_trip_speed = 0;	// set to 1 when trip speed reached
int16 trip_speed_fpm = 100;	// trip speed value
int16 max_trip_speed = 100;	// max trip speed value
int16 set_trip_speed = 0;	// set trip speed value
int16 test_pos = 0;
int16 skipped_pfc_sfc_test = 0;		// set when pfc sfc test skipped

 static  const char brake_test_dpy[15][25]  = {
  "to Run Up Dir Brake Test",	// 0
  "    Cancel Brake Test   ",	// 1
  "  Brake Test Complete   ",	// 2
  "  Brake Test Cancelled  ",	// 3
  "      Run Failure       ",	// 4
  "Preinstall Jumpr RG5-RG7",	// 5
  "& GOV-S10,then Hit Enter",	// 6
  "to Run Up Dir EmBrk Test",	// 7
  "Overspeed Test<Up/Dn>? N",	// 8
  "Press Enter to Continue ",	// 9
  "  Jump TST1 & Hit Enter ",	// 10
  "Set Drv OvSpd &Jump TST1",	// 11
  "Hit Enter:UpDir EBrk Tst",	// 12
  "Set Trip Spd = 00100 fpm",	// 13
  " then Mode to Continue  ",	// 14
  
};


void Run_Brake_Test (void)
{
	int16 i = 0;
	
	if ((Menu_level == 18) || (Menu_level == 19))
	{		// Emergency or Normal Brake Test
	
		if(PasswordFlag == false)
		{	
			LCD_Pointer = Menu_level + 8;
			Menu_level = 0;			          
			StoreLCDInfo();
			clrLCDdpy();
			LCD_Mode = 25;
			LCD_Pointer = 25;
		    return;
		} 
		if ((LCD_Pointer == 26) || (LCD_Pointer == 27)) 	// first time through Brake Test
		{
		    LCD_Pointer++;
			bkt_dir_select = 1;		// default to up direction
			Overspeed_Test = 0;
    		max_trip_speed = cons[speed];		// else contract speed
			trip_speed_fpm = cons[speed];
			test_pos = cons[bottomf];
			if (Menu_level == 19)
				LCD_Sub_Point = 13;
			else
	    		LCD_Sub_Point = 1;
		    Up_Date_LCD = 1;	 // Update display
		    bkt_cmd = 0;
		    bkt_run_err = 0;
		    bkt_sd = 0;
		    skipped_pfc_sfc_test = 0;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
		{

			if ((LCD_Sub_Point != 9) && (LCD_Sub_Point != 15))
			{   		// use enter command when == 9
				if (skipped_pfc_sfc_test == 1)		
					pfc_sfc_test = 0;

			    LCD_Mode_PB = 1;  //
			    LCD_Pointer = Menu_level + 8;
			    Menu_level = 0; 
			    if (procf == 23)
				    procf = 0;
				strncpy(LCD_Display[0],LCD_Main_Menu[8],24);
				strncpy(LCD_Display[1],LCD_Elev_Setup[LCD_Pointer-9],24);
			    return;
			}
		}
		if(LCD_Sub_Point == 1)    // Find Direction of Test
		{
			if ((bkt_dir_select != 1) && (bkt_dir_select != 2))
				bkt_dir_select = 1;
		  	if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		  	{
				LCD_UP_PB = 1;
				bkt_dir_select = 1;		
			}
		  	else if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		  	{
				LCD_DN_PB = 1;
				bkt_dir_select = 2;		
			}
	  		else if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
	  		{
				LCD_Enter_PB = 1;
				if ((bkt_dir_select == 1) || (bkt_dir_select == 2))
					LCD_Sub_Point = 2;			// Continue with test
			}
			else
  		    {
				strncpy(LCD_Display[0],limit_test_dpy[0],24);	   // Press Up to select Up or
				strncpy(LCD_Display[1],limit_test_dpy[1],24);	   // Dn to select down
				if (bkt_dir_select == 1)
				{
					LCD_Display[0][20] = 'U';
					LCD_Display[0][21] = 'p';
				}
				else if (bkt_dir_select == 2)
				{
					LCD_Display[0][20] = 'D';
					LCD_Display[0][21] = 'n';
				}
				else
				{
					bkt_dir_select = 1;
					LCD_Display[0][20] = 'U';
					LCD_Display[0][21] = 'p';
				}
				
  		    }
		}
		if(LCD_Sub_Point == 2)    // checking to make sure auto door is disabled
		{
 	  		if(chk_ins() == 0)
		    {
				strncpy(LCD_Display[0],Learn[0],24);    // Put elevator on inspection
				strncpy(LCD_Display[1],Learn[1],24);
		    }
		    else
				  LCD_Sub_Point = 3;
		}
		if (LCD_Sub_Point == 3)
		{
			strncpy(LCD_Display[0],limit_test_dpy[1],24);	 // Hit enter to proceed
			strncpy(LCD_Display[1],limit_test_dpy[6],24);
	  		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
	  		{
			    LCD_Enter_PB = 1;
				LCD_Sub_Point = 4;			// Check auto door
				return;
	  		}
		}
		if(LCD_Sub_Point == 4)    // checking to make sure auto door is disabled
		{
			if (rdinp(i_AD) == 1)
		    {
				strncpy(LCD_Display[0],carbuffer_dpy[0],24);
				strncpy(LCD_Display[1],carbuffer_dpy[1],24);
		    }
		    else
		    {
				LCD_Sub_Point = 5;
		    }
		}
		if(LCD_Sub_Point == 5)    // make sure the car is on automatic
		{
 			if (rdinp(i_AD) == 1)
				  LCD_Sub_Point = 4;
 	  		else if ((servf == 2) && (procf == 15) && (chk_ins() == 0))	  // autodoor disabled and waiting for a call
			{
		    	if (cons[EM_BRK] != 0)
		    	{
			    	io_fault = 0;
			    	statusf &= ~sf_IO;
					pfc_sfc_test = 15;
					in_pfc_sfc_test = 0;
					io_fault_cnt = 0;
					skipped_pfc_sfc_test = 1;
		    	}
	
				LCD_Sub_Point = 6;
			}
		    else
		    {
				strncpy(LCD_Display[0],carbuffer_dpy[2],24);
				strncpy(LCD_Display[1],carbuffer_dpy[3],24);
		    }
		}
		if(LCD_Sub_Point == 6)    // Put the car in position
		{
			if (bkt_dir_select == 2)
			{		 // running down
				test_pos = cons[topf];
			}
			else
			{   	// running up
				test_pos = cons[bottomf];
			}

			if ((position != test_pos) && (procf == 15))
			{		// Car not in position
				if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
			  	{
					LCD_UP_PB = 1;
					test_pos = position;
					LCD_Sub_Point = 7;		// Skip	to run test from where the car is
					return;
				}
		  		else if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		  		{
				    LCD_Enter_PB = 1;
					if(ccmsk[cons[carnmb]][test_pos] != 0)
						set_carcall(cons[carnmb],test_pos);
					else if ( (cons[rear] != 0) && (rccmsk[cons[carnmb]][test_pos] != 0) )
						set_rcarcall(cons[carnmb],test_pos);
					LCD_Sub_Point = 7;			// Put car in position
					return;
				}
				else
	  		    {
					strncpy(LCD_Display[0],carbuffer_dpy[6],24);  // Push enter to position car
					strncpy(LCD_Display[1],carbuffer_dpy[7],24);  // or push up to skip
	  		    }
			}
			else
			{
				LCD_Sub_Point = 7;
				return;
			}
		}
		if(LCD_Sub_Point == 7)  // Verify car in position   
		{
			if ((procf == 15) && (dpref == 0) && (dirf == 0))
			{
				if (position == test_pos)
				{
			    	if (cons[EM_BRK] != 0)
			    	{
				    	io_fault = 0;
				    	statusf &= ~sf_IO;
						pfc_sfc_test = 15;
						in_pfc_sfc_test = 0;
						io_fault_cnt = 0;
						skipped_pfc_sfc_test = 1;	
			    	}
					Overspeed_Test = 0;
		    		max_trip_speed = cons[speed];		// else contract speed
		    		trip_speed_fpm = cons[speed];
					LCD_Sub_Point = 14;	// Check to run brake test with overspeed
				}
				else
				{
					LCD_Sub_Point = 6;
				}
			}
			else  
			{
				strncpy(LCD_Display[0],carbuffer_dpy[8],24);    // Wait for car to reach position
				strncpy(LCD_Display[1],carbuffer_dpy[9],24);
			}
			return;
		}
		if (LCD_Sub_Point == 8)		 // Run Brake test
		{
		  	if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		  	{
			    LCD_Enter_PB = 1;
				Auto_Brake_Test(0);		// init variables for test
			    
				if (bkt_dir_select == 2)		 // run down
				{
					if (Menu_level == 19)
						bkt_cmd = 2;				// em brake dn
					else if (Menu_level == 18)
						bkt_cmd = 4;			   	// normal brake dn
				}
				else
				{					 	// run up
					if (Menu_level == 19)
						bkt_cmd = 1;			   	// em brake up
					else if (Menu_level == 18)
						bkt_cmd = 3;
				}									// normal brake up
				LCD_Sub_Point = 9;
				return;
			}
			else
			{
				if (Menu_level == 19)
				{
					if (Overspeed_Test == 1)
					{
						strncpy(LCD_Display[0],brake_test_dpy[11],24);	 // Ready for Brake test
						strncpy(LCD_Display[1],brake_test_dpy[12],24);
						if (bkt_dir_select == 2)
						{
							LCD_Display[1][10] = 'D';
							LCD_Display[1][11] = 'n';
						}
					}
					else
					{
						strncpy(LCD_Display[0],brake_test_dpy[10],24);	 // Ready for Brake test
						strncpy(LCD_Display[1],brake_test_dpy[7],24);
						if (bkt_dir_select == 2)
						{
							LCD_Display[1][7] = 'D';
							LCD_Display[1][8] = 'n';
						}
					}
				}
				else
				{
					strncpy(LCD_Display[0],carbuffer_dpy[10],24);	 // Ready for Brake test
					strncpy(LCD_Display[1],brake_test_dpy[0],24);
					if (bkt_dir_select == 2)
					{
						LCD_Display[1][8] = 'D';
						LCD_Display[1][9] = 'n';
					}
				}
			}
		}
		if (LCD_Sub_Point == 9)
		{
			if (bkt_cmd != 0)
			{
				Auto_Brake_Test(bkt_cmd);
			  	if (((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0)) ||
			  		(rdinp(i_AD) == 1) || (servf != 2))
			  	{			// Press the enter button to cancel
				    LCD_Enter_PB = 1;
					LCD_Sub_Point = 10;
					bkt_sd = 1;
					timers[tbktest] = 0;
					clrcc(0);
				}
				else if (bkt_run_err != 0)
				{
					LCD_Sub_Point = 11;
					timers[tbktest] = 0;
					clrcc(0);
				}
				else
				{										  
					strncpy(LCD_Display[0],carbuffer_dpy[12],24);		 // enter to cancel
					strncpy(LCD_Display[1],brake_test_dpy[1],24);
				}
				if (bkt_test_seq >= 4)
					bkt_cmd = 0;
			}
			else
			{
				LCD_Sub_Point = 12;
				timers[tbktest] = 0;
			}
		}
		if (LCD_Sub_Point == 10)
		{
									// not running Auto_Brake_Test() so OK to use timers[tbktest]
			if (enc_vel_fpm < 3)
			{
			    if ((procf == 23) && (timers[tbktest] > 20))
				    procf = 0;			// GO to reset mode
			}
			else
				timers[tbktest] = 0;
				
			strncpy(LCD_Display[0],brake_test_dpy[3],24);		// Test Cancelled press mode for main menu
			strncpy(LCD_Display[1],carbuffer_dpy[15],24);
		}
		if (LCD_Sub_Point == 11)
		{
			if ((procf == 23) && (timers[tbktest] > 60))
				procf = 0;
			if (Menu_level == 19)
				strncpy(LCD_Display[0],LCD_Elev_Setup[18],24);
			else 
				strncpy(LCD_Display[0],LCD_Elev_Setup[17],24);
			strncpy(LCD_Display[1],brake_test_dpy[4],24);
		}
		if (LCD_Sub_Point == 12)
		{
			if ((procf == 23) && (timers[tbktest] > 20))
				procf = 0;
			strncpy(LCD_Display[0],brake_test_dpy[2],24);		 // Brake test complete
			strncpy(LCD_Display[1],carbuffer_dpy[15],24);
		}
		if (LCD_Sub_Point == 13)
		{
			strncpy(LCD_Display[0],brake_test_dpy[5],24);	 // Check jumpers
			strncpy(LCD_Display[1],brake_test_dpy[6],24);	 // hit enter to continue
	  		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
	  		{
			    LCD_Enter_PB = 1;
				LCD_Sub_Point = 1;			// Check auto door
				bkt_dir_select = 1;		// default to up direction
			    bkt_cmd = 0;
			    bkt_run_err = 0;
			    bkt_sd = 0;
				return;
	  		}
		}
		if(LCD_Sub_Point == 14)    // Select to run overspeed test with brake test
		{
		  	if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		  	{
				LCD_UP_PB = 1;
				Overspeed_Test = 1;		
		    	max_trip_speed = 2000;
			}
		  	else if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		  	{
				LCD_DN_PB = 1;
				Overspeed_Test = 0;		
		    	max_trip_speed = cons[speed];		// else contract speed
			}
	  		else if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
	  		{
				LCD_Enter_PB = 1;
				LCD_Sub_Point = 15;			// get trip speed value
			}
			else
  		    {
				strncpy(LCD_Display[0],brake_test_dpy[8],24);	   // Select to overspeed
				strncpy(LCD_Display[1],brake_test_dpy[9],24);	   // Press enter to continue
				if (Overspeed_Test == 1)
				{
						LCD_Display[0][23] = 'Y';
				}
				else
				{
						LCD_Display[0][23] = 'N';
				}
  		    }
		}
				
  		if(LCD_Sub_Point == 15)    // Select the trip speed value
		{
		  	if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		  	{
				LCD_UP_PB = 1;
		    	Up_Date_LCD = 1;
   		
		   		adjust_variable (&trip_speed_fpm, max_trip_speed, 0, 1, LCD_Dig_Point);
			}
		  	else if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		  	{
				LCD_DN_PB = 1;
		    	Up_Date_LCD = 1;
			    adjust_variable (&trip_speed_fpm, max_trip_speed, 0, 0, LCD_Dig_Point);
			}
	  		else if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
	  		{
				LCD_Enter_PB = 1;
		    	Up_Date_LCD = 1;
				if (set_trip_speed == 0)
					set_trip_speed = 1;
				else
					set_trip_speed = 0;
			}
			if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
			{
			    LCD_Mode_PB = 1;  
		    	Up_Date_LCD = 1;
		    	if (set_trip_speed == 1)
					LCD_Dig_Point = set_adj_digit(32000,LCD_Dig_Point);			
		    	else
					LCD_Sub_Point = 8;			// Continue with test
			}
			if (Up_Date_LCD == 1)
			{
				Up_Date_LCD = 0;
		    	for(i=0; i<=23; i++)
		    	{
					LCD_Display[0][i] = brake_test_dpy[13][i];
					LCD_Display[1][i] = brake_test_dpy[14][i];
		    	}
				display_variable(0, 14, 0, trip_speed_fpm, 1);
			}
	  		flash_digit(LCD_Flash_TMP[0], 0,LCD_Dig_Loc,(set_trip_speed == 1));
		}
	}
}


//************************************
// Automatic Learn Hoistway Routine
//************************************

void Auto_Brake_Test (int16 cmd)
{				  // Procedure called by LCD Routine
					// cmd: 0 = reset
					// cmd: 1 = run up em brake test
					// cmd: 2 = run dn em brake test
					// cmd: 3 = run up normal brake test
					// cmd: 4 = run dn normal brake test

	if (cmd == 0)
	{
		fault_run = 0;
		bkt_test_seq = 0;
		bkt_run_cmd = 0;
		bkt_run_err = 0;
	    bkt_sd = 0;
		bkt_run_vel = 0;
		bkt_trip_speed = 0;
		bkt_max_speed = (float)fvars[fvtopspeed];
		return;
	}

						  
	if (bkt_run_err != 0)
		bkt_test_seq = 4;

	switch (bkt_test_seq)
	{
		case 0:		// Start learn mode
			bkt_test_seq = 1;
			procf = 23;
			bkt_trip_speed = 0;
			bkt_run_cmd = 0;
			timers[tbktest] = 0;
			break;
		case 1: 	// Delay for 1 seconds
			if (timers[tbktest] > 10)
			{
				if ((cmd == 1) || (cmd == 3))
					bkt_run_cmd = 1; 	// Run up
				else if (( cmd == 2) || (cmd == 4))
					bkt_run_cmd = 2; 	// Run down
				
				bkt_test_seq = 2;
				timers[tbktest] = 0;
			}
			else
				bkt_run_cmd = 0;
			break;
		case 2:		// Run to test speed
			if (bkt_trip_speed == 1)
			{
				if ((cmd == 1) || (cmd == 2))
					bkt_run_cmd = 3;		// emergency brake test
				else if ((cmd == 3) || (cmd == 4))
					bkt_run_cmd = 4; 		// normal brake test
				else	
					bkt_run_cmd = 0;
				bkt_test_seq = 3;
				timers[tbktest] = 0;
			}
			else
			{
				if (((rdinp(i_UN) == 0) && (dirf == 1)) || ((rdinp(i_DN) == 0) && (dirf == 2)))
				{
					if (bkt_trip_speed == 0)
						record_fault(f_bkt_un_dn);

					bkt_trip_speed = 1;
				}
				
				if (((rdinp(i_UT) == 0) && (dirf == 1)) || ((rdinp(i_DT) == 0) && (dirf == 2)))
				{
					if (bkt_trip_speed == 0)
						record_fault(f_bkt_ut_dt);

					bkt_trip_speed = 1;
				}
				
				if (((rdinp(i_UTS) == 0) && (dirf == 1)) || ((rdinp(i_DTS) == 0) && (dirf == 2)))
				{
					if (bkt_trip_speed == 0)
						record_fault(f_bkt_uts_dts);

					bkt_trip_speed = 1;
				}
			
				if (timers[tbktest] > 100)
				{
					if (bkt_trip_speed == 0)
						record_fault(f_bkt_time_out);

					bkt_trip_speed = 1;		  		// have 10 seconds to hit trip speed
				}
					
				if (enc_vel_fpm >= trip_speed_fpm)
						bkt_trip_speed = 1;
			}
			break;
	
		case 3:		// Delay for test to finish
			if (((Drive_Type[cons[carnmb]] == 11) && (timers[tbktest] >= 47)) || (timers[tbktest] > 60))
			{
				timers[tbktest] = 0;
				bkt_test_seq = 4;
			}
			break;
			
		case 4:		// Brake Test Complete
			bkt_run_cmd = 0;
			bkt_test_seq = 4;
			break;


		default:
			bkt_test_seq = 4;
			bkt_run_cmd = 0;
			break;
	}

	fault_prog_flag1 = bkt_test_seq;
	fault_prog_flag2 = bkt_run_cmd;
}


//******************************
// Brake Test Procedure
//******************************

void Brake_Test(void)
{
   	int16 i;
   	int16 moving=0;
//   	int16 brake_dropped = 0;
 
	clrall();
	dirf = 0;
	bkt_run_vel = 0;
	bkt_run_err = 0;
    bkt_sd = 0;
	fault_run = 0;

   	while(procf == 23)
   	{

	 	inctime(0);
		set_ul_dl_dz();		// sets up_level, dn_level and door_zone

		// Testing to see if saftey string is open
	    if(safeties() == 0)
	    {
			if ((statusf & sf_SS) == 0)
		  		record_fault(f_sstring);
			statusf |= sf_SS;
	      	procf = 12;       // go to saftey string open mode
	      	timers[tmotion] = 0;
			timers[tpatdly] = 0;
	      	return;
	    }
		else
			statusf &= ~sf_SS;


		// Testing to see if LC fuse is blown
		if(rdinp(i_S10) == 0)
	    {
			if ((statusf & sf_S10) == 0)
			  	record_fault(f_S10_fuse);
			statusf |= sf_S10;
			timers[tmotion] = 0;
			timers[tpatdly] = 0;
			bkt_run_err = 1;
	    }
		else
			statusf &= ~sf_S10;

		// Testing to see if the ready from the Drive is open
//		if((rdinp(i_READY) == 0) && (rdinp(i_GTS) == 1))
//	    {
//			if ((statusf & sf_READY) == 0)
//		  		record_fault(f_drive_rdy);
//			statusf |= sf_READY;
//	      	timers[tmotion] = 0;
//			timers[tpatdly] = 0;
//			bkt_run_err = 1;
//	    }
//		else
//			statusf &= ~sf_READY;
//
		// Testing to see if on inspection
		if(chk_ins() == 1)
		{  // On inspection
		  	procf = 2;        // go to inspection mode
		  	timers[tmotion] = 0;
			timers[tpatdly] = 0;
		  	return;
		}
		
		if ((DBR_Temp_Fault != 0) || (gripper_fault != 0) || ((fault_run != 0) && (enc_vel_fpm > 10)))
			bkt_run_err = 1;	
	
		

		if ((safe() == 1) && (bkt_run_err == 0) && (in_pfc_sfc_test == 0) && (in_gripper_test == 0))
		{
			bktest_run_cmd(bkt_run_cmd);
		}
		else
		{		// not safe
			startf = 0;							// ins motion fix
			
			if ((car_gate() == 0) || (door_locks() == 0))  // not safe because doors are open
			{
				if (((rdinp(i_DPM) == 1) || (rdinp(i_DCL) == 0)) && ((cons[rear] == 0) || (rdinp(i_RPM) == 1) || (rdinp(i_DCLR) == 0)))
				{
					setoutp(o_DC);
					clroutp(o_DO);
					if (cons[rear] != 0)
					{
						setoutp(o_DCR);
						clroutp(o_DOR);
					}
				}
			}
			else if ((in_pfc_sfc_test == 1) || (in_gripper_test == 1))
			{
				at_floor_check();	
			}
			else
			{
				if (bkt_run_err == 0)
			  		record_fault(f_hw_safe_fault);
				bkt_run_err = 1;
		    }
		    clroutp(o_MCC);
		    clroutp(o_MCA);
		    clroutp(o_MST);
		    clroutp(o_UP);
		    clroutp(o_DNR);
	    	clroutp(o_RUN);
	    	clroutp(o_RUNA);
		    clroutp(o_UPF);
		    clroutp(o_DF);
			clroutp(o_FF);
		}
		
		
	 	if((bkt_run_cmd == 0) || (bkt_run_err != 0))
	 	{
	 	
/*	 		if ( ((rdinp(i_BKS) != 0) && (fvars[fvbls] == 0)) || ((rdinp(i_BKS) == 0) && (fvars[fvbls] != 0))) 
	 			brake_dropped = 1;
	 		else brake_dropped = 0;	

	 		if ((gripper_fault == 0) && (rdoutp(o_BRK) == 0) && (brake_dropped == 1))
	 		{		// Brake is dropped so okay to pick gripper 
			 	setoutp(o_LE);
				setoutp(o_LE1);
				setoutp(o_GR2R);
	 		}
*/
		 	moving = 0;
		 	Velocity = 0;
			Vel_Step = 0;
		 	timers[tvpat] = 0;
		 	timers[tmotion] = 0;
			don_delay = 30;		// don input not valid because it is jumped out
//			don_delay = 0;
			
			get_load(1);

		    if (bkt_run_err == 0)
			{
				if (startf == 1)
 					drop_brake(c_drop_brake_timed);
 			    else
 					drop_brake(c_drop_brake_reset);
 				timers[tpatdly] = 0;
 		    }
		    else
			{
				timers[tpatdly] = 0;
				drop_brake(c_drop_brake_reset);
			}
	 	}
	 	else
	 	{
			if ((bkt_run_cmd != 0) && (bkt_run_cmd < 3) && (startf == 1) && (bkt_run_err == 0))
			{	
			 	if(moving == 0)
				 	moving = 1;


				if(cons[dcpo] == 0)
				{
					clroutp(o_DC);
					if (cons[rear] != 0)
						clroutp(o_DCR);
				}
				else
				{
					setoutp(o_DC);
					if (cons[rear] != 0)
						setoutp(o_DCR);			
				}

				don_delay = 30;		// don input not valid because it is jumped out
/*				if (fvars[fvdon] != 0)
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
*/
				if (timers[tpatdly] < (fvars[fvpat_del] + don_delay))
				{
					set_pre_torque();
					Velocity = 0;
					Vel_Step = 0;
				}
				else
				{
					torque = 0;
					don_delay = 0;
				 	for(i=1; i<=timers[tvpat]; i++)
				 	{
						if (((rdinp(i_UT) == 0) && (dirf == 1)) || ((rdinp(i_DT) == 0) && (dirf == 2)) || (bkt_sd == 1))
						{
							bkt_run_vel = 0;

//							if (Vel_Step < Vel_Step_Const)
//				  	  	  		Vel_Step += Vel_Step_Inc;
						 	if(Velocity > bkt_run_vel)
						 	{
							 	Velocity -= (Em_Decel / 100.00);
//								Velocity -= Vel_Step;
								if (Velocity < bkt_run_vel)
									Velocity = bkt_run_vel;
						 	}
							else 
							{
							/*
							 	if (Velocity < bkt_run_vel)
							 	{
									Velocity += Vel_Step;
									if (Velocity > bkt_run_vel)
										Velocity = bkt_run_vel;
							 	}
								else
									Velocity = bkt_run_vel;			// ins motion fix
						   */

						   		Velocity = 0;
								bkt_run_cmd = 0;
							}
						}
				     	else	
				     	{
							bkt_run_vel = (float)((bkt_max_speed * Pulses_per_Inch) / 5.0);
							if (Vel_Step < Vel_Step_Const)
				  	  	  		Vel_Step += Vel_Step_Inc;
						 	if(Velocity < bkt_run_vel)
						 	{
								Velocity += Vel_Step;
								if (Velocity > bkt_run_vel)
									Velocity = bkt_run_vel;
						 	}
							else 
								Velocity = bkt_run_vel;			// ins motion fix
				     	}
				 	}
				}	
				if ((startf == 1) && (bkt_run_err == 0))
					pick_brake(c_pick_brake_run);
				else
					drop_brake(c_drop_brake_reset);
			}
			else
			{
				if (bkt_run_cmd == 3)
				{			  	// drop gripper circuit
					clroutp(o_LE);
					clroutp(o_LE1);
					clroutp(o_GR2R);
//					if (bkt_test_seq >= 4)		// drop the normal brake before the emergency brake is picked
//						drop_brake(c_drop_brake_reset);
//					else
					pick_brake(c_pick_brake_run);
				}
				else if (bkt_run_cmd == 4)
				{
					clroutp(o_BRK);
					drop_brake(c_drop_brake_reset);
				}
				torque = 0;
				Velocity = 0;
				Vel_Step = 0;
			}
		 	timers[tvpat] = 0;
	 	}

		Dmd_Velocity = Velocity;

		if ((timers[tmotion] >= fvars[fvhwstall]) && (bkt_run_err == 0))
		{
			bkt_run_err = 1;
			fault_run = 1;
	  		record_fault(f_bkt_stall_fault);
		}

	}

}

//******************************
// Brake Test run command
//******************************

void bktest_run_cmd(int16 cmd)
{

	if (cmd == 0)
	{		// no run

		io_fault = 0;					// clear redundancy io fault flag (on insp and not running)
		io_fault_cnt = 0;
		statusf &= ~sf_IO;

		clroutp(o_UPF);
		clroutp(o_DF);

        // Checking to see if a soft stop is needed
		if((startf == 1) && (timers[tsoftstp] <= fvars[fvsst]))  // soft stop needed
	    {
			setoutp(o_MCC);
			setoutp(o_MCA);
			setoutp(o_RUN);   // set run output
			setoutp(o_RUNA);
		 	setoutp(o_FF);
			if(dirf == 1)
			{
				setoutp(o_UP);      // set up output
				clroutp(o_DNR);   // clear down output
			}
			else
			{
				setoutp(o_DNR);   // set down output
				clroutp(o_UP);      // clear up output
			}
        }
        else
        {
			dirf = 0;
			clroutp(o_UP);
			clroutp(o_DNR);
	    	if(timers[tsoftstp] >= fvars[fvsst] + 10)  // Delay drop of ms and mc
			{
				startf = 0;				// Allow the next run
				clroutp(o_RUN);
				clroutp(o_RUNA);
				clroutp(o_MCC);
				clroutp(o_MCA);
			}
		 	clroutp(o_FF);
        }
	}
	else
	{	
		if (safe() == 0)
		{
		    clroutp(o_MCC);
		    clroutp(o_MCA);
			if (Drive_Type[cons[carnmb]] != 9)
			    clroutp(o_MST);
		    clroutp(o_UP);
		    clroutp(o_DNR);
		    clroutp(o_RUN);
		    clroutp(o_RUNA);
		    clroutp(o_UPF);
		    clroutp(o_DF);
		  	return;	
		}

		clroutp(o_UPF);
		clroutp(o_DF);

		if (cmd == 1)
		{		// run up
			dirf = 1;
			if (startf == 0)
			{
				if (chk_start(1) == 1)
				{
					startf = 1;
					timers[tpatdly] = 0;
					statusf &= ~sf_IO;
				}
				else
				{
					statusf |= sf_IO;		// I/O Error
					clrall();
				}
			}
			else
			{
				setoutp(o_UP);
				clroutp(o_DNR);
				setoutp(o_RUN);
				setoutp(o_RUNA);
				setoutp(o_MCC);
				setoutp(o_MCA);
				setoutp(o_FF);
				timers[tsoftstp] = 0;
			}
		}
		else if (cmd == 2)
		{		// run down
			dirf = 2;
			if (startf == 0)
			{
				if (chk_start(2) == 1)
				{
					startf = 1;
					timers[tpatdly] = 0;
					statusf &= ~sf_IO;
				}
				else
				{
					statusf |= sf_IO;		// I/O Error
					clrall();
				}
			}
			else
			{
				setoutp(o_DNR);
				clroutp(o_UP);
				setoutp(o_RUN);
				setoutp(o_RUNA);
			    setoutp(o_MCC);
				setoutp(o_MCA);
			 	setoutp(o_FF);
				timers[tsoftstp] = 0;
			}
		}
		else if (cmd == 3)
		{		// Emergency Brake Test
			if (timers[tsoftstp] <= 60)  // drop everything after 7 seconds
		    {
				setoutp(o_MCC);
				setoutp(o_MCA);
				setoutp(o_RUN);   // set run output
				setoutp(o_RUNA);
	        }
	        else
	        {
				startf = 0;				// Allow the next run
				clroutp(o_RUN);
				clroutp(o_RUNA);
				clroutp(o_MCC);
				clroutp(o_MCA);
	        }
			dirf = 0;
		 	clroutp(o_FF);
			clroutp(o_DNR);   // clear down output
			clroutp(o_UP);      // clear up output
		}
		else if (cmd == 4)
		{		// Normal Brake Test
			if (timers[tsoftstp] <= 60)  // drop everything after 7 seconds
		    {
				setoutp(o_MCC);
				setoutp(o_MCA);
	        }
	        else
	        {
				startf = 0;				// Allow the next run
				clroutp(o_MCC);
				clroutp(o_MCA);
	        }
			dirf = 0;
		 	clroutp(o_FF);
			clroutp(o_DNR);   // clear down output
			clroutp(o_UP);      // clear up output
			clroutp(o_RUN);
			clroutp(o_RUNA);
		}
		
	}
}


/* 

Revision History

04/15/15 v7.1.30 mhd	1. Corrected index for carbuffer_dpy[] from 17 to 15 when brake test cancelled.
6/3/15 v7.1.32 mhd		1. Change the direction selection for brake test to default in the up direction.
						2. Added check display for jumpers and show direction on final run test display.
7/21/15 v7.1.40 mhd		1. Added brake test with overspeed.
11/23/15 v7.1.49 mhd	1. Change brake test sequence to drop normal brake before emergency brake is picked
						   when test is finished.
12/11/15 v7.1.53 mhd	1. Change brake test sequence to not pick the gripper until after leaving the brake test module.
						   To insure this operation, brake test must exit to reset routine by setting procf = 0.
*/