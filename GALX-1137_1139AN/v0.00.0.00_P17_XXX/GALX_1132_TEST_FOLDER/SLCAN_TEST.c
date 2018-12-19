#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"

#include "SEB_D.h"
#include "NEW_CAN_CON.h"




int16 TEST_SLCAN(int16 Operation);

enum {
    Init_SLCAN_Test,
    Wait_for_Operator_Response,
    Send_SLCAN_Signal,
    Check_SLCAN_Signal,
    SLCAN_test_Fail,
    SLCAN_test_Pass,
    SLCAN_test_Done,
    Retest_option, // Required for Retest Option    
};

#define SLtest_signal_index 12
unsigned char SLtest_iosignal_array[SLtest_signal_index]= {
    SLi0,
    SLi1,
    SLi2,
    SLi3,
    SLi4,
    SLi5,
    SLi6,
    SLi7,
    SLi8,
    SLi9,
    SLi10,
    SLi11,
};




#define TEST_SLCAN_Fault_Display_Index	2
char TEST_SLCAN_Fault_Display[TEST_SLCAN_Fault_Display_Index][20]=
{
	"Failed.             ",
	"CN9                 ",	
};







int16 TEST_SLCAN(int16 Operation)
{
    static int16 SLCAN_TestCase=Init_SLCAN_Test;
//  int16 Delay_Count= consB[20];
    int16 Delay_Count=test_halfsec_delay;
    static int16 Counter = 0;
    static int16 index=0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option    
    int16 i=0;
      
    switch (SLCAN_TestCase) {
    case Init_SLCAN_Test:
    	Clear_Log_Buffer();
        clr_lcd_dply();
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
//-     SIU.GPDO[Z0_LED].R=1;  //Turn OFF LED
//-     SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
//-     if(Delay_Count < 1)  Delay_Count = 1;
//-     if(Delay_Count > 20) Delay_Count = 20;
        
        New_write_display("SLCAN TEST          ",0,0);
        New_write_display("TX:      RX:        ",1,0);
        New_write_display("Set correct switch  ",2,0);
        New_write_display("Press Enter when rdy",3,0);  
//-     New_write_display("                    ",2,0);
//-     New_write_display("                    ",3,0);         
        index=0;

        for(i=0; i<SLtest_signal_index; i++) {
            SET_SEB_IO[SLtest_iosignal_array[i]]=0;
        }
        SLCAN_TestCase=Wait_for_Operator_Response;        
//-		SLCAN_TestCase=Send_SLCAN_Signal; 		
		timers[t_Generic_Delay]=0;
        break;

    case Wait_for_Operator_Response:
        if ( Check_for_Enter_Button() )
        {
           New_write_display("                    ",2,0);
           New_write_display("                    ",3,0);        
           SLCAN_TestCase = Send_SLCAN_Signal;   
        }
        break;
                

    case Send_SLCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

//-     SIU.GPDO[Z0_LED].R=0;  //Turn ON LED

        SET_SEB_IO[SLtest_iosignal_array[index]]=1;
        SLCAN_TestCase=Check_SLCAN_Signal;
        timers[t_Generic_Delay]=0;
        sprintf(&LCD_Display[1][3],"%02i",index);
        break;

    case Check_SLCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

        if(SEB_IO[SLtest_iosignal_array[index]]!=1) {
            SLCAN_TestCase=SLCAN_test_Fail;
            break;
        } else {
//-         SIU.GPDO[Z0_LED].R=1;  //Turn OFF LED
            sprintf(&LCD_Display[1][12],"%02i",index);
        }

        index++;
        if(index<SLtest_signal_index) {
            SLCAN_TestCase=Send_SLCAN_Signal;
        } else {
            SLCAN_TestCase=SLCAN_test_Pass;
        }
        timers[t_Generic_Delay]=0;
        break;
/*
    case SLCAN_test_Pass:
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        SLCAN_TestCase = Init_SLCAN_Test;
        New_write_display("   PASS             ",1,0);
        return DONE_BUT_CONTINUE_DISPLAY;
        break;

    case SLCAN_test_Fail:
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        SLCAN_TestCase = Init_SLCAN_Test;
        New_write_display("      FAILED        ",1,0);
        return RETEST_OPTION;
        break;
*/
    case SLCAN_test_Pass:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("SLCAN TEST          ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0); 
        SLCAN_TestCase = SLCAN_test_Done;        
        break;
        
    case SLCAN_test_Done:
//-     SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        SLCAN_TestCase = Init_SLCAN_Test; 
        if(Operation & 0x10 & consB[19])
        { // 16 = turn off LED's
            for(i=0; i<SLtest_signal_index; i++) {
                SET_SEB_IO[SLtest_iosignal_array[i]]=0;
            }            
        }          
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
      
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) SLCAN_TestCase = SLCAN_test_Done;
        else if(RetestResponse == 1) SLCAN_TestCase = Init_SLCAN_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;     
        
    case SLCAN_test_Fail:
//-    	clr_lcd_dply();
        New_write_display(TEST_SLCAN_Fault_Display[0],2,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_SLCAN_Fault_Display[1]);
        SLCAN_TestCase = Retest_option;        
        break;

    default:
        SLCAN_TestCase=Init_SLCAN_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }

    return ReturnVal; // Required for Retest Option	
}











