#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



static int16 Delay_Count = 0;
static int16 NextCaseAfterWait = 0;

enum
{
    Command_Mode,  
    InitTask,
    Wait,
    RunTask,
    Failed,
    Passed,
    TaskDone,
    Wait_for_Operator_Response,
    Wait_to_exit,
    Retest_option, // Required for Retest Option
    Exit_Command_Mode = 25
};



#define TEST_WIFI_Fault_Display_Index	3
char TEST_WIFI_Fault_Display[TEST_WIFI_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC9  CN13,14,15,16  ",	
	"Red_Brd BDR=230400? ",
};



#define  CPU_WIFI_BAUD_RATE_DEFAULT			230400 




int16 WiFi_Test(int16 Cmnd)
{

static int16 The_WiFi_loop = 0;
static int16 the_case = InitTask;
static int16 Retest_Option_Parm = 0; // Required for Retest Option
static int16 RetestResponse = 1; // Required for Retest Option
static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
static int16 SaveBaudRate = 354;  // 19.2 is default, but WiFi needs 9.6kb

    switch(the_case)
    {
        case InitTask:
            Clear_Log_Buffer(); 
            clr_lcd_dply(); // clear 4-line display                                            
            CurrentTestPassFail =0;  // fail
            ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
            Retest_Option_Parm = 0; // Required for Retest Option
            RetestResponse = 1; // Required for Retest Option            
            if(ForceTestToIndicateFail)
            {
              the_case = Failed;
              ReturnVal = CONTINUE_SERVICE; 
              break;   
            }     
            
            //1s delay       
            Delay_Count = 10;
            
            //MUST SET THIS BAUD RATE 230400 to MATCH WIFI Board's BAUD RATE,
            //otherwise, WIFI Board NO COMM
            USR1port.CR1.B.SBR = USR2port.CR1.B.SBR = baud_tbl[7]; // 230400
            
            The_WiFi_loop = 0;
            en_cmd_state[USR1COM] = 0;
            
            
            if(Cmnd == 0) 
            {
            	New_write_display("WIFI TEST           ",0,0);
            	New_write_display("Set Jumper to 1&2   ",1,0);
            	New_write_display("ENTER key when ready",2,0);             	
            }
            else
            {
            	New_write_display("WIFI Exit Command MD",0,0);
            	New_write_display("Set Jumper to 1&2   ",1,0);
            	New_write_display("ENTER key when ready",2,0);             	              	
            }
            timers[t_Generic_Delay] = 0;      
            NextCaseAfterWait=Wait_for_Operator_Response;                   
           	the_case=Wait;
            	 
            Send_Net_Cmd(USR1COM,Exit_Command_Mode);			// exit command mode
			Rcv_Net_Response(USR1COM,0,1);				    	// initialize state machine	 
        break;

        case Wait_for_Operator_Response:
            if ( Check_for_Enter_Button() )
            {
             	New_write_display("                    ",1,0);
                New_write_display("                    ",2,0);
                                                                        
                if(Cmnd == 0)
                { // enable WiFi               
                    En_Net_Cmd_Mode(USR1COM);
                    // Send_Net_Cmd( USR1COM, Command_Mode);			// 0 = enter command mode; 25 = exit command mode
                    // the_case = RunTask;
                    // or
                    NextCaseAfterWait = RunTask;
                    the_case = Wait; 
                    sprintf(&LCD_Display[1][0],"Command_Mode=%d",Command_Mode);
                }                                                                
                else
                {
                  	Send_Net_Cmd(USR1COM,Exit_Command_Mode);			// 0 = enter command mode; 25 = exit command mode
					Rcv_Net_Response(USR1COM,0,1);						// initialize state machine	

 					NextCaseAfterWait = TaskDone;
                    the_case = Wait; 
                    sprintf(&LCD_Display[1][0],"Exit_Command_Mode=%d",Exit_Command_Mode);                   
                }   
                timers[t_Generic_Delay] = 0;         
            }                        
            break;
                    
        case RunTask:
        	//the number of "en_cmd_state[]" comes back from "En_Net_Cmd_Mode()"
        	//the coming back number "3" meaning got into the Command Mode, 
        	//                       "4" meaning TIME OUT, FAIL getting into the Command Mode
            sprintf(src,"State=%d Loop=%d",en_cmd_state[USR1COM],The_WiFi_loop );
          //  Log_an_event(src); 
          	New_write_display(src,2,0);
          	
            NextCaseAfterWait = RunTask;            
            En_Net_Cmd_Mode(USR1COM);  // this will go thru the necessary cases.
            if(The_WiFi_loop++ > 12)the_case = Failed;
                else the_case = Wait;
                
            timers[t_Generic_Delay] = 0;
            break;
        
        case Wait:
            if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
                timers[t_Generic_Delay] = 0; //
                 if (en_cmd_state[USR1COM] == 3) the_case = Passed;
                 else if (en_cmd_state[USR1COM] == 4) the_case = Failed;
                 else the_case = NextCaseAfterWait;
            }        
            break;
                                        
        case TaskDone: // exit command mode
        	if(timers[t_Generic_Delay] <= Delay_Count*2)
        		break;
        	
            the_case = InitTask;                        			
            USR1port.CR1.B.SBR = USR2port.CR1.B.SBR = RS485_Baud; // restore baud rate
            if(Cmnd == 0)
            {
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time    
            }
            else ReturnVal = FINISHED_WITH_SERVICE;               
            break; 
        
        case Passed:      
        	clr_lcd_dply(); 
        	Send_Net_Cmd(USR1COM,Exit_Command_Mode);			// exit command mode
			Rcv_Net_Response(USR1COM,0,1);				    	// initialize state machine	
			  
            sprintf(src,"State=%d Loop=%d",en_cmd_state[USR1COM],The_WiFi_loop );
          //  Log_an_event(src);
          	New_write_display(src,2,0);
          
          	sprintf(&LCD_Display[3][0],"CPU(WIFI) BDR=%d", CPU_WIFI_BAUD_RATE_DEFAULT);          	          	          	            
             
            New_write_display("WIFI TEST           ",0,0);           
            New_write_display("TEST PASSED         ",1,0);
//-         New_write_display("GOOD JOB            ",2,0); 

			CurrentTestPassFail = PASS;  // Pass
            the_case = TaskDone;
            timers[t_Generic_Delay] = 0;
            break;
        
        case Failed:
            CurrentTestPassFail = FAIL;  // FAILED
//-    		clr_lcd_dply();
        	New_write_display(TEST_WIFI_Fault_Display[0],3,0);
        
       	 	GALX1121_Fault_Display_ADDIN_Function();
       	 	sprintf(src,"State=%d Loop=%d",en_cmd_state[USR1COM],The_WiFi_loop );
            Log_an_event(src);
        	Log_an_event(TEST_WIFI_Fault_Display[1]);            
            Log_an_event(TEST_WIFI_Fault_Display[2]); 
            the_case = Retest_option;            
            break; 
                    
         case Retest_option: // Required for Retest Option
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) the_case = TaskDone;
            else if(RetestResponse == 1) the_case = InitTask; 
            else 
            ReturnVal = CONTINUE_SERVICE;  
            break;                   
                                                          
        default:
          ReturnVal = DEFAULT;
          break;        
    }
    return ReturnVal; // Required for Retest Option	   
}

































