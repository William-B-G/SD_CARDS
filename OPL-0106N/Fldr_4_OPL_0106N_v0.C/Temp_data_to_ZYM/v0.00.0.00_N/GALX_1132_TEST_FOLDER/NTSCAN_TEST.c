#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"




int16 TEST_NTSCAN(int16 Operation);



enum {
    Init_NTSCAN_Test,
    Wait_for_Operator_Response,
    Send_NTSCAN_Signal,
    Check_NTSCAN_Signal,
    NTSCAN_test_Fail,
    NTSCAN_test_Pass,
    NTSCAN_test_Done,
    Retest_option, // Required for Retest Option    
};

#define coom_dec_count  500
#define comm_min_count	500
#define comm_inti_count 8500
#define comm_test_mcnt	8
 
extern int32 tsm_cedes_pls;


// Cedes CAN Bus Selector - Direct access to video position data
extern struct CED_can_type {
	int32	position_count;
	int16	velocity;
	uint8 	vel_dir;			// velocity direction
	union {
		uint16 R;
		struct {
			uint16:1;
			uint16 ALIGN:3;
			uint16 REV:4;
			uint16:3;
			uint16 IN1:1;
			uint16 IN2:1;
			uint16 CONTRAST:2;
			uint16 CLIP:1;
		} B;
	} Status;

	union {
		uint8 R;
		struct {
			uint8 CHANNEL1:2;
			uint8 CHANNEL2:2;
			uint8:1;
			uint8 GENERAL:3;
		} B;
	} Error;
	bool 	online;
	uint8	online_cnt;
	int16	tx_cnt;
	int16	rx_cnt;
}CED_can;						// Cedes Selector data CAN channel 1 - direct to CEDes device 

 


#define TEST_NTSCAN_Fault_Display_Index	2
char TEST_NTSCAN_Fault_Display[TEST_NTSCAN_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC6,7,8,9  CN5      ",	
};






int16 TEST_NTSCAN(int16 Operation)
{
	static int16 NTSCAN_TestCase=0;
//- int16 Delay_Count= consB[20];
	int16 Delay_Count=test_halfsec_delay;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option    
    
    
    switch (NTSCAN_TestCase) 
    {
    	case Init_NTSCAN_Test:
       		CurrentTestPassFail =0;  // fail
       		Clear_Log_Buffer(); 
       		clr_lcd_dply();
        	ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        	Retest_Option_Parm = 0; // Required for Retest Option
        	RetestResponse = 1; // Required for Retest Option        
 	    
//-        	if(Delay_Count < 1)  Delay_Count = 1;
//-        	if(Delay_Count > 20) Delay_Count = 20;
        	
       	 	New_write_display("NTSCAN TEST         ",0,0);
       		New_write_display("TX:       RX:       ",1,0);
        	New_write_display("                    ",2,0);
        	New_write_display("                    ",3,0);         
        
        	tsm_cedes_pls=comm_inti_count;
        	NTSCAN_TestCase=Send_NTSCAN_Signal;           	     
        	timers[t_Generic_Delay]=0;
        break;
        
        case Send_NTSCAN_Signal:
       		if(timers[t_Generic_Delay]<Delay_Count)
            	break;

        	
//-        		SIU.GPDO[Z6_LED].R=0;        		
        		tsm_cedes_pls-=coom_dec_count;
				if(tsm_cedes_pls<=comm_min_count)
				{
					tsm_cedes_pls=comm_min_count;
				}
       	 	

      		NTSCAN_TestCase=Check_NTSCAN_Signal;
      		timers[t_Generic_Delay]=0;
        	sprintf(&LCD_Display[1][3],"%05i",tsm_cedes_pls);        	
        break;
        
        case Check_NTSCAN_Signal:
        	if(timers[t_Generic_Delay]<Delay_Count)
            	break;
        		
        		
        	if(tsm_cedes_pls==CED_can.position_count)
        	{
//-        		SIU.GPDO[Z6_LED].R=1;
        		if((tsm_cedes_pls==comm_min_count)&&(CED_can.position_count==comm_min_count))
        		{
        			NTSCAN_TestCase=NTSCAN_test_Pass;
        			tsm_cedes_pls=comm_inti_count;
        		}
        		else
        		{
        			NTSCAN_TestCase=Send_NTSCAN_Signal;
        		}	        	
       	 	}
			else
			{
				NTSCAN_TestCase=NTSCAN_test_Fail;
				tsm_cedes_pls=comm_inti_count;
			}
						        	            	         	
        	timers[t_Generic_Delay]=0;
        	sprintf(&LCD_Display[1][13],"%05i",CED_can.position_count);  
        break;
        
    	case NTSCAN_test_Pass:
       		CurrentTestPassFail = 1; // pass
       		clr_lcd_dply();
       		New_write_display("NTSCAN TEST         ",0,0);
        	New_write_display("TEST PASSED         ",1,0);
            New_write_display("GOOD JOB            ",2,0); 
        	NTSCAN_TestCase = NTSCAN_test_Done;        
        break;
        
    	case NTSCAN_test_Done:
        	NTSCAN_TestCase = Init_NTSCAN_Test; 
        	
        	// Required for Retest Option
        	if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        		else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     	case Retest_option: // Required for Retest Option
        	RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        	Retest_Option_Parm = 1;
        	if(RetestResponse == 0) NTSCAN_TestCase = NTSCAN_test_Done;
        	else if(RetestResponse == 1) NTSCAN_TestCase = Init_NTSCAN_Test; 
        	else 
        		ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    case NTSCAN_test_Fail:
//-    		clr_lcd_dply();
        	New_write_display(TEST_NTSCAN_Fault_Display[0],2,0);
        
        	GALX1121_Fault_Display_ADDIN_Function();
       		Log_an_event(TEST_NTSCAN_Fault_Display[1]);
        	NTSCAN_TestCase = Retest_option;
        break;

    default:
        	NTSCAN_TestCase=Init_NTSCAN_Test;
        	ReturnVal = DEFAULT;
        break;    		    
    }
    return ReturnVal; // Required for Retest Option	
}


























