#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"
#include "v120AC_Power_Check.h"





int16 SAMPLE_IO_TEST(int16 Operation);




enum
{
	SAMPLE_IO_Test_Init=0,
	SAMPLE_IO_TEST_Pass,
	SAMPLE_IO_TEST_Done,
	SAMPLE_IO_TEST_Fail,
	Retest_option				// Required for Retest Option  
};


	
	
#define sample_io_test_menu_index		30
static char SAMPLE_IO_Test_Menu[sample_io_test_menu_index][21]= 
{
  "SAMPLE IO TEST       ",       //0
};






//------------------------------------------------------------------------------   
int16 SAMPLE_IO_TEST(int16 Operation)
{
	static int16 SAMPLE_IO_TEST_CASE=SAMPLE_IO_Test_Init;
 	int16 Delay_Count=test_1s_delay;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    switch (SAMPLE_IO_TEST_CASE) 
    {
    	case SAMPLE_IO_Test_Init:
       		CurrentTestPassFail=0;  // fail
       		clr_lcd_dply();
        	ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        	Retest_Option_Parm = 0; // Required for Retest Option
        	RetestResponse = 1; // Required for Retest Option        
 	            	
       	 	New_write_display(SAMPLE_IO_Test_Menu[0],0,0);       	 
                	                	
//-        	SAMPLE_IO_TEST_CASE=TurnON_SAMPLE_IO_SUF_S14;      
        	
        	Include_ALL_Short_Signal();        	
        	Reset_Fault_Type_Index();          
        	Turn_ALL_Output_OFF();        	
        	Exclude_PowerAndReset_InputPin();			
			timers[t_Generic_Delay]=0;
        break;
//------------------------------------------------------------------------------          
        case SAMPLE_IO_TEST_Pass:
       		 CurrentTestPassFail = 1; // pass
        	 clr_lcd_dply();
        	 New_write_display("SAMPLE IO TEST       ",0,0);
        	 New_write_display("TEST PASSED         ",1,0);
        	 New_write_display("GOOD JOB            ",2,0);     
        	 SAMPLE_IO_TEST_CASE=SAMPLE_IO_TEST_Done;        
        break;
        
    	case SAMPLE_IO_TEST_Done:
			 SAMPLE_IO_TEST_CASE=SAMPLE_IO_Test_Init; 
        
          	 // Required for Retest Option
        	 if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        	 else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     	case Retest_option: // Required for Retest Option
        	 RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        	 Retest_Option_Parm = 1;
        	 if(RetestResponse == 0) SAMPLE_IO_TEST_CASE=SAMPLE_IO_TEST_Done;
        	 else if(RetestResponse == 1) SAMPLE_IO_TEST_CASE=SAMPLE_IO_Test_Init; 
        	 else 
        	 	ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    	case SAMPLE_IO_TEST_Fail:
    		clr_lcd_dply();
     		Disply_Fault();
//-        	New_write_display("ICxxx               ",0,0);
        	SAMPLE_IO_TEST_CASE = Retest_option;
        break;

    	default:
        	SAMPLE_IO_TEST_CASE=SAMPLE_IO_Test_Init;
        	ReturnVal = DEFAULT;
        break;
    }	
    
    return ReturnVal; // Required for Retest Option	            
}
























