#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"
#include "v120AC_Power_Check.h"

#include "NEW_CAN_CON.h"






int16 TEST_JIG_COMM(int16 Operation);




enum
{
	TEST_JIG_COMM_Init=0,
	Check_TEST_JIG_SLCAN_COMM,
	Check_TEST_JIG_Mechine_Room_CAN_COMM,
	TEST_JIG_COMM_Pass,
	TEST_JIG_COMM_Done,
	TEST_JIG_COMM_Fail,
	Retest_option				// Required for Retest Option  
};


	
	
#define test_jig_comm_menu_index		30
static char TEST_JIG_COMM_Menu[test_jig_comm_menu_index][21]= 
{
  "TEST JIG COMM       ",       //0
  "COMM Checking...    ",		//1
};



#define test_jig_comm_fail_menu_index		30
static char TEST_JIG_COMM_Fail_Menu[test_jig_comm_fail_menu_index][21]= 
{
  "                    ",       //0
  "SEB #1 CAN COMM FAIL",       //1
  "SEB #2 CAN COMM FAIL",       //2
  "SEB #3 CAN COMM FAIL",       //3
  "SEB #4 CAN COMM FAIL",       //4
  "SEB #5 CAN COMM FAIL",       //5
  "SEB #6 CAN COMM FAIL",       //6
  "SEB #7 CAN COMM FAIL",       //7
  "SEB #8 CAN COMM FAIL",       //8
};




#define MAX_DEVIX_ADD_TEST_JIG_COMM		13	

#define devix_offset					5







//------------------------------------------------------------------------------   
int16 TEST_JIG_COMM(int16 Operation)
{
	static int16 TEST_JIG_COMM_CASE=TEST_JIG_COMM_Init;
 	int16 Delay_Count=test_1s_delay;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    
    //SLCAN IO Board starts from #1 SEB Board which CAN ADD is 11
    //Mechine Room CAN IO Board starts from #2 SEB Board which CAN ADD is 12
    //"devix" inside "mrcan.c" (this is the devix number, example: devix=6 is for Mechine Room CAN SEB Board #1 CAN ADD is 11)
    //							        the devix number meaning the INDEX of "int16 mrcan_addr[c_max_mrcan_dev]"
    static int16 devix=7;
    
    
    static int16 COMM_Fail_index=0;
        
    
    switch (TEST_JIG_COMM_CASE) 
    {
    	case TEST_JIG_COMM_Init:
    		devix=7;
    		COMM_Fail_index=0;
       		CurrentTestPassFail=0;  // fail
       		Clear_Log_Buffer();
       		clr_lcd_dply();
        	ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        	Retest_Option_Parm = 0; // Required for Retest Option
        	RetestResponse = 1; // Required for Retest Option        
 	            	
       	 	New_write_display(TEST_JIG_COMM_Menu[0],0,0);       	 
                	                	
         	TEST_JIG_COMM_CASE=Check_TEST_JIG_SLCAN_COMM;      
        	
        	Include_ALL_Short_Signal();        	
        	Reset_Fault_Type_Index();          
        	Turn_ALL_Output_OFF();        	
        	Exclude_PowerAndReset_InputPin();        				
			timers[t_Generic_Delay]=0;			
        break;
//------------------------------------------------------------------------------  
    	case Check_TEST_JIG_SLCAN_COMM:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(TEST_JIG_COMM_Menu[1],1,0); 
    		
    		if(SEB.Online==0)
    		{
    			TEST_JIG_COMM_CASE=TEST_JIG_COMM_Fail;    			
    		}
    		else 
    		{    			
    			TEST_JIG_COMM_CASE=Check_TEST_JIG_Mechine_Room_CAN_COMM;
    		}
    		
    		COMM_Fail_index=1;
    		
    		sprintf(&LCD_Display[2][0],"SEB#%02i  Online=%01i",COMM_Fail_index,SEB.Online);   
    		timers[t_Generic_Delay]=0;
    	break;
//------------------------------------------------------------------------------  
    	case Check_TEST_JIG_Mechine_Room_CAN_COMM:
    		if(timers[t_Generic_Delay]<Delay_Count)
    			break;    		
     	
     		
     		COMM_Fail_index=devix-devix_offset;
     		sprintf(&LCD_Display[2][0],"SEB#%02i  Online=%01i",COMM_Fail_index,mrcan_dev[devix].online);  
     		
    		if(mrcan_dev[devix].online == 0)
    		{
     			TEST_JIG_COMM_CASE=TEST_JIG_COMM_Fail;    				
    		}
    		else
    		{
    			devix++;
    				
    			if(devix>MAX_DEVIX_ADD_TEST_JIG_COMM)
    			{
     				TEST_JIG_COMM_CASE=TEST_JIG_COMM_Pass;	
    			}
    		}    		     		
     		 
    		timers[t_Generic_Delay]=0;
       	break;    	
//------------------------------------------------------------------------------          
        case TEST_JIG_COMM_Pass:
       		 CurrentTestPassFail = 1; // pass
        	 clr_lcd_dply();
        	 New_write_display("TEST JIG COMM       ",0,0);
        	 New_write_display("TEST PASSED         ",1,0);
        	 New_write_display("GOOD JOB            ",2,0);     
        	 TEST_JIG_COMM_CASE=TEST_JIG_COMM_Done;        
        break;
        
    	case TEST_JIG_COMM_Done:
			 TEST_JIG_COMM_CASE=TEST_JIG_COMM_Init; 
        
          	 // Required for Retest Option
        	 if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        	 else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     	case Retest_option: // Required for Retest Option
        	 RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        	 Retest_Option_Parm = 1;
        	 if(RetestResponse == 0) TEST_JIG_COMM_CASE=TEST_JIG_COMM_Done;
        	 else if(RetestResponse == 1) TEST_JIG_COMM_CASE=TEST_JIG_COMM_Init; 
        	 else 
        	 	ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    	case TEST_JIG_COMM_Fail:
//-    		clr_lcd_dply();
//-    		Disply_Fault();
        	New_write_display(TEST_JIG_COMM_Fail_Menu[COMM_Fail_index],1,0);
        	
        	GALX1121_Fault_Display_ADDIN_Function();
        	Log_an_event(TEST_JIG_COMM_Fail_Menu[COMM_Fail_index]);
        	TEST_JIG_COMM_CASE = Retest_option;
        break;

    	default:
        	TEST_JIG_COMM_CASE=TEST_JIG_COMM_Init;
        	ReturnVal = DEFAULT;
        break;
    }	
    
    return ReturnVal; // Required for Retest Option	            
}
























