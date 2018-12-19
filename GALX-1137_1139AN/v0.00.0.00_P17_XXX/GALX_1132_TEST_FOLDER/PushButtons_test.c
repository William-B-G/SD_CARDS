#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"


int16 Verify_only_one_Button(void);
int16 Buttons_State(void);

/* //-
enum {
    Init_PushButtons_Test,
    Push_Up_Button_wait,
    Push_Down_Button_wait,
    Push_Mode_Button_wait,
    Push_Enter_Button_wait,
    Check_for_only_UpButton,
    Check_for_only_DownButton,
    Check_for_only_ModeButton,
    Check_for_only_EnterButton,
    Fill_Seqments,
    PushButtonsSegmentsOn,
    PushButtonsRestored,
    PushButtons_TestDone,
    PushButtons_TestFailed,
    Failed_But_Delay,
    GeneralWaitState
};
*/

enum {
    Init_PushButtons_Test,
    Check_ALL_PB_Init_State,            
    Check_Only_UpButton_Press,
    Check_Only_UpButton_Release,
    
    Check_Only_DownButton_Press,
    Check_Only_DownButton_Release,
    
    Check_Only_ModeButton_Press,
    Check_Only_ModeButton_Release,
    
    Check_Only_EnterButton_Press,
    Check_Only_EnterButton_Release,
    
    PushButtons_TestDone,
    PushButtons_TestFailed,
    PushButtons_Passed,
    WaitBeforeRetest,
    Retest_option 
};


enum {
    For_Up_Button,
    For_Down_Button,
    For_Mode_Button,
    For_Enter_Button
};



#define TEST_PushButtons_Fault_Display_Index	6
char TEST_PushButtons_Fault_Display[TEST_PushButtons_Fault_Display_Index][20]=
{
	"Failed.             ",
	"ENTER SW4 Fail      ",
	"MODE SW3 Fail       ",
	"DOWN SW2 Fail       ",			
	"UP SW1 Fail         ",	
	"NO PTN PRESSED      ",
};


extern unsigned char BIT[8];









int16 PushButtons_Test(int16 LCD_Menu)  // externed in // GALX_1132_TEST.h
{
    static int8  count_num = 0;
    static int8 Debug_LED = 0;  // Change the state of Z0_LED
    static int8 PushButtonsDebug_1 = 0;
    static int16 Up_Dwn_Touched = 0;
    static int16 PushButtonsTestCase = Init_PushButtons_Test;
    static int16 NextCaseAfterWait = 0;
    static int16 The_Return_Val =0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
	//3s delay
    static int16 Delay_Count=20;
    //20s delay
    static int16 Delay_Count1=200;
    
    int16 Menu_Index;

	int16 i=0;
	
    Menu_Index = LCD_Menu;
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED

    switch(PushButtonsTestCase) 
    {
    	case Init_PushButtons_Test:
    		Clear_Log_Buffer(); 
        	clr_lcd_dply();
        	CurrentTestPassFail =0;  // fail
        	New_write_display("PUSH BUTTON TEST    ",0,0);
        	New_write_display("Plz Do Not Press Any",1,0);
        	New_write_display("Push-Button Yet     ",2,0);        
       		New_write_display("                    ",3,0);
       	 	LCD_Display[3][3] =  (char)Show_Button_Up;
        	LCD_Display[3][8] =  (char)Show_Button_Up;
        	LCD_Display[3][13] = (char)Show_Button_Up;
        	LCD_Display[3][18] = (char)Show_Button_Up;
        	timers[t_Generic_Delay] = 0; // t_PushButtons_Delay
        	Up_Dwn_Touched = 0;
        	The_Return_Val =0;
            ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
            Retest_Option_Parm = 0; // Required for Retest Option
            RetestResponse = 1; // Required for Retest Option        	
//-     	PushButtonsTestCase = Push_Up_Button_wait;
			PushButtonsTestCase=Check_ALL_PB_Init_State;
        break;

    	case Check_ALL_PB_Init_State:
    		if(timers[t_Generic_Delay]<=Delay_Count)
    		{
    			if((The_Return_Val=Buttons_State())!=0) 
    			{
                	//error
                	PushButtonsTestCase=PushButtons_TestFailed;
    			}
            } 
            else 
            {
            	timers[t_Generic_Delay]=0;
            	
            	New_write_display("PUSH BUTTON TEST    ",0,0);
                New_write_display("Plz Hold 'UP' Key   ",1,0);
                New_write_display("                    ",2,0);
                New_write_display("                    ",3,0);
                LCD_Display[3][3] =  (char)Show_Button_Down;
                LCD_Display[3][8] =  (char)Show_Button_Up;
                LCD_Display[3][13] = (char)Show_Button_Up;
                LCD_Display[3][18] = (char)Show_Button_Up;                
                PushButtonsTestCase = Check_Only_UpButton_Press;    
    		}
    	break;
	
    	case Check_Only_UpButton_Press:
    		if(timers[t_Generic_Delay]<=Delay_Count1)
    		{
    			if(rdLCDio(Lcd_UP)==1)
    			{    		
    				if((The_Return_Val=Buttons_State())==8) 
    				{
            			PushButtonsTestCase=Check_Only_UpButton_Release;
            		
            			timers[t_Generic_Delay]=0;
            			New_write_display("PUSH BUTTON TEST    ",0,0);
            			New_write_display("Plz Release 'UP' Key",1,0);
                		New_write_display("                    ",2,0);
               	 		New_write_display("                    ",3,0);
                		LCD_Display[3][3] =  (char)Show_Button_Up;
                		LCD_Display[3][8] =  (char)Show_Button_Up;
                		LCD_Display[3][13] = (char)Show_Button_Up;
                		LCD_Display[3][18] = (char)Show_Button_Up;
    				}    			
    				else
    				{
    					//error
            			PushButtonsTestCase=PushButtons_TestFailed;
    				}
    			}
        	} 
        	else
        	{
        		//error
            	PushButtonsTestCase = PushButtons_TestFailed;
        	}    	
    	break;
    	
		case Check_Only_UpButton_Release:
    		if(timers[t_Generic_Delay]<=Delay_Count1)
    		{
    			if(rdLCDio(Lcd_UP)==0)
    			{ 
    				if((The_Return_Val=Buttons_State())==0)
    				{    		    				
            			PushButtonsTestCase=Check_Only_DownButton_Press;
            		
            			timers[t_Generic_Delay]=0;
            			New_write_display("PUSH BUTTON TEST    ",0,0);
            			New_write_display("Plz Hold 'DOWN' Key ",1,0);
                		New_write_display("                    ",2,0);
               	 		New_write_display("                    ",3,0);
                		LCD_Display[3][3] =  (char)Show_Button_Up;
                		LCD_Display[3][8] =  (char)Show_Button_Down;
                		LCD_Display[3][13] = (char)Show_Button_Up;
                		LCD_Display[3][18] = (char)Show_Button_Up;  			    			
    				}
    				else
    				{
    					//error
            			PushButtonsTestCase=PushButtons_TestFailed;
    				}
        		}
    		}
        	else
        	{
        		//error
            	PushButtonsTestCase = PushButtons_TestFailed;
        	}    	
    	break;
    
    	case Check_Only_DownButton_Press:
    		if(timers[t_Generic_Delay]<=Delay_Count1)
    		{
    			if(rdLCDio(Lcd_DN)==1)
    			{    		
    				if((The_Return_Val=Buttons_State())==4) 
    				{
            			PushButtonsTestCase=Check_Only_DownButton_Release;
            		
            			timers[t_Generic_Delay]=0;
            			New_write_display("PUSH BUTTON TEST    ",0,0);
            			New_write_display("Plz Release 'DOWN'  ",1,0);
                		New_write_display("Key                 ",2,0);
               	 		New_write_display("                    ",3,0);
                		LCD_Display[3][3] =  (char)Show_Button_Up;
                		LCD_Display[3][8] =  (char)Show_Button_Up;
                		LCD_Display[3][13] = (char)Show_Button_Up;
                		LCD_Display[3][18] = (char)Show_Button_Up;
    				}    			
    				else
    				{
    					//error
             			PushButtonsTestCase=PushButtons_TestFailed;
    				}
    			}
        	} 
        	else
        	{
        		//error
            	PushButtonsTestCase = PushButtons_TestFailed;
        	}    	
    	break;
    	
		case Check_Only_DownButton_Release:
    		if(timers[t_Generic_Delay]<=Delay_Count1)
    		{
    			if(rdLCDio(Lcd_DN)==0)
    			{ 
    				if((The_Return_Val=Buttons_State())==0)
    				{    		    				
            			PushButtonsTestCase=Check_Only_ModeButton_Press;
            		
            			timers[t_Generic_Delay]=0;
            			New_write_display("PUSH BUTTON TEST    ",0,0);
            			New_write_display("Plz Hold 'MODE' Key ",1,0);
                		New_write_display("                    ",2,0);
               	 		New_write_display("                    ",3,0);
                		LCD_Display[3][3] =  (char)Show_Button_Up;
                		LCD_Display[3][8] =  (char)Show_Button_Up;
                		LCD_Display[3][13] = (char)Show_Button_Down;
                		LCD_Display[3][18] = (char)Show_Button_Up;
    				}        			
    				else
    				{
    					//error
            			PushButtonsTestCase=PushButtons_TestFailed;
    				}
    			}
        	} 
        	else
        	{
        		//error
            	PushButtonsTestCase = PushButtons_TestFailed;
        	}    	
    	break;	
    	
    	case Check_Only_ModeButton_Press:
    		if(timers[t_Generic_Delay]<=Delay_Count1)
    		{
    			if(rdLCDio(Lcd_MODE)==1)
    			{    		
    				if((The_Return_Val=Buttons_State())==2) 
    				{
            			PushButtonsTestCase=Check_Only_ModeButton_Release;
            		
            			timers[t_Generic_Delay]=0;
            			New_write_display("PUSH BUTTON TEST    ",0,0);
            			New_write_display("Plz Release 'MODE'  ",1,0);
                		New_write_display("Key                 ",2,0);
               	 		New_write_display("                    ",3,0);
                		LCD_Display[3][3] =  (char)Show_Button_Up;
                		LCD_Display[3][8] =  (char)Show_Button_Up;
                		LCD_Display[3][13] = (char)Show_Button_Up;
                		LCD_Display[3][18] = (char)Show_Button_Up;
    				}    			
    				else
    				{
    					//error
            			PushButtonsTestCase=PushButtons_TestFailed;
    				}
    			}
        	} 
        	else
        	{
        		//error
            	PushButtonsTestCase = PushButtons_TestFailed;
        	}    	
    	break;
    	
		case Check_Only_ModeButton_Release:
    		if(timers[t_Generic_Delay]<=Delay_Count1)
    		{
    			if(rdLCDio(Lcd_MODE)==0)
    			{
    				if((The_Return_Val=Buttons_State())==0)
    				{    		    				
            			PushButtonsTestCase=Check_Only_EnterButton_Press;
            			
            			timers[t_Generic_Delay]=0;
            			New_write_display("PUSH BUTTON TEST    ",0,0);
            			New_write_display("Plz Hold 'ENTER' Key",1,0);
                		New_write_display("                    ",2,0);
               	 		New_write_display("                    ",3,0);
                		LCD_Display[3][3] =  (char)Show_Button_Up;
                		LCD_Display[3][8] =  (char)Show_Button_Up;
                		LCD_Display[3][13] = (char)Show_Button_Up;
                		LCD_Display[3][18] = (char)Show_Button_Down;
    				}    			
    				else
    				{
    					//error
            			PushButtonsTestCase=PushButtons_TestFailed;
    				}
    			}
        	} 
        	else
        	{
        		//error
            	PushButtonsTestCase = PushButtons_TestFailed;
        	}    	
    	break;	
    	
    	case Check_Only_EnterButton_Press:
    		if(timers[t_Generic_Delay]<=Delay_Count1)
    		{
    			if(rdLCDio(Lcd_ENTER)==1)
    			{    		
    				if((The_Return_Val=Buttons_State())==1) 
    				{
            			PushButtonsTestCase=Check_Only_EnterButton_Release;
            		
            			timers[t_Generic_Delay]=0;
            			New_write_display("PUSH BUTTON TEST    ",0,0);
            			New_write_display("Plz Release 'ENTER' ",1,0);
                		New_write_display("Key                 ",2,0);
               	 		New_write_display("                    ",3,0);
                		LCD_Display[3][3] =  (char)Show_Button_Up;
                		LCD_Display[3][8] =  (char)Show_Button_Up;
                		LCD_Display[3][13] = (char)Show_Button_Up;
                		LCD_Display[3][18] = (char)Show_Button_Up;
    				}    			
    				else
    				{
    					//error
            			PushButtonsTestCase=PushButtons_TestFailed;
    				}
    			}
        	} 
        	else
        	{
        		//error
            	PushButtonsTestCase = PushButtons_TestFailed;
        	}    	
    	break;
    	
		case Check_Only_EnterButton_Release:
    		if(timers[t_Generic_Delay]<=Delay_Count1)
    		{
    			if(rdLCDio(Lcd_ENTER)==0)
    			{ 
    				if((The_Return_Val=Buttons_State())==0)
    				{    		    				
            			PushButtonsTestCase=PushButtons_Passed; // PushButtons_TestDone; //PushButtons_Passed
            		
            			timers[t_Generic_Delay]=0;
            			New_write_display("PUSH BUTTON TEST    ",0,0);          		
                		New_write_display("RELEASE ALL KEYS    ",1,0);
            			New_write_display("                    ",2,0);
            			New_write_display("                    ",3,0);
            			LCD_Display[3][3] =  (char)Show_Button_Up;
            			LCD_Display[3][8] =  (char)Show_Button_Up;
            			LCD_Display[3][13] = (char)Show_Button_Up;
            			LCD_Display[3][18] = (char)Show_Button_Up;  
            			The_Return_Val=0;          		
    				}    			
    				else
    				{
    					//error
            			PushButtonsTestCase=PushButtons_TestFailed;
    				}
    			}
        	} 
        	else
        	{
        		//error
            	PushButtonsTestCase = PushButtons_TestFailed;
        	}    	
    	break;	    	

    case PushButtons_TestDone:
        Check_for_Up_Button();
        Check_for_Down_Button();
        Check_for_Mode_Button();
        Check_for_Enter_Button();  // Release all keys
        PushButtonsTestCase = Init_PushButtons_Test;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
        
    case PushButtons_Passed:    
        CurrentTestPassFail = 1; // pass     
        clr_lcd_dply();   
        New_write_display("PUSH BUTTON TEST    ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0); 
        PushButtonsTestCase = PushButtons_TestDone;       
        break;
  
    case PushButtons_TestFailed:    
    	timers[t_Generic_Delay]=0;        
        
        Check_for_Up_Button();
        Check_for_Down_Button();
        Check_for_Mode_Button();
        Check_for_Enter_Button();  // Release all keys        
        PushButtonsTestCase = Retest_option; 
        
//-     clr_lcd_dply();        
        New_write_display("BUTTON_VAL=         ",2,0);
        sprintf(&LCD_Display[2][11],"%02i",The_Return_Val);    
        New_write_display(TEST_PushButtons_Fault_Display[0],3,0);    
        
        GALX1121_Fault_Display_ADDIN_Function();
        
        for(i=0;i<4;i++)
        {        
        	//UP BUTTON bit3(The_Return_Val=8), DOWN BUTTON bit2(4), MODE BUTTON bit1(2), ENTER BUTTON bit0(1)
        	if((The_Return_Val&BIT[i])==BIT[i])
        	{
        		Log_an_event(TEST_PushButtons_Fault_Display[i+1]);
        	} 
        	else if(The_Return_Val==0)
        	{
        		Log_an_event(TEST_PushButtons_Fault_Display[5]);
        	}
        }
                
        ReturnVal = CONTINUE_SERVICE;
        Retest_Option_Parm = 0;
        break;
// WaitBeforeRetest  // Retest_option; // Init_PushButtons_Test;
       
     case Retest_option: // Required for Retest Option     
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) PushButtonsTestCase = PushButtons_TestDone;
        else if(RetestResponse == 1) PushButtonsTestCase = Init_PushButtons_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;  

    default:
        PushButtonsTestCase=Init_PushButtons_Test;
        clr_lcd_dply();
        New_write_display("BUTTON TEST (error) ",1,0);
        New_write_display("default case taken  ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        //return DONE_BUT_CONTINUE_DISPLAY;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }
    return ReturnVal; // Required for Retest Option	
}

int16 Buttons_State()
{
    int16 The_State = 0;

	The_State=0;
	
    if( rdLCDio(Lcd_UP) != 0) { // up key pressed
        The_State |= 0x8;
    }
    if( rdLCDio(Lcd_DN) != 0) { // down key pressed
        The_State |= 0x4;
    }
    if( rdLCDio(Lcd_MODE) != 0) { // mode key pressed
        The_State |= 0x2;
    }
    if( rdLCDio(Lcd_ENTER) != 0) { // enter key pressed
        The_State |= 0x1;
    }

    return The_State;
}


int16 Verify_only_one_Button(void)
{
    int16 The_Buttons = 0;

    The_Buttons = ( rdLCDio(Lcd_UP) << 3 ) | ( rdLCDio(Lcd_DN) << 2 ) |
                  ( rdLCDio(Lcd_MODE) << 1 ) | ( rdLCDio(Lcd_ENTER) ) ;
    return The_Buttons;
}
