#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



int16 TEST_CTCAN(int16 Operation);

enum {
    Init_CTCAN_Test,
    Wait_for_Operator_Response,
    Send_CTCAN_Signal,
    Check_CTCAN_Signal,
    CTCAN_test_Fail,
    CTCAN_test_Pass,
    CTCAN_test_Done,
    Retest_option // Required for Retest Option    
};


#define CTtest_signal_index 12
void *CTtest_osignal_array[CTtest_signal_index]= {
    o_CTo0,
    o_CTo1,
    o_CTo2,
    o_CTo3,
    o_CTo4,
    o_CTo5,
    o_CTo6,
    o_CTo7,
    o_CTo8,
    o_CTo9,
    o_CTo10,
    o_CTo11
};

void *CTtest_isignal_array[CTtest_signal_index]= {
    i_CTi0,
    i_CTi1,
    i_CTi2,
    i_CTi3,
    i_CTi4,
    i_CTi5,
    i_CTi6,
    i_CTi7,
    i_CTi8,
    i_CTi9,
    i_CTi10,
    i_CTi11
};

#define TEST_CTCAN_Fault_Display_Index	2
char TEST_CTCAN_Fault_Display[TEST_CTCAN_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC24,27,28,29  CN2  ",	
};




int16 TEST_CTCAN(int16 Operation)
{
    static int16 CTCAN_TestCase=Init_CTCAN_Test;
//  int16 Delay_Count= consB[20];
    int16 Delay_Count=test_halfsec_delay;
    static int16 Counter = 0;
    static int16 index=0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option    
    int16 i=0;

    switch (CTCAN_TestCase) {
    case Init_CTCAN_Test:
        CurrentTestPassFail =0;  // fail
        Clear_Log_Buffer();
        clr_lcd_dply();
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
//-	    SIU.GPDO[Z0_LED].R=1;  //Turn OFF LED
//-     SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
//-     if(Delay_Count < 1)  Delay_Count = 1;
//-     if(Delay_Count > 20) Delay_Count = 20;
        timers[t_Generic_Delay]=0;
        New_write_display("CTCAN TEST          ",0,0);
        New_write_display("TX:      RX:        ",1,0);
        New_write_display("Set correct switch  ",2,0);
        New_write_display("Press Enter when rdy",3,0);        
        index=0;
        for(i=0; i<CTtest_signal_index; i++) {
            clroutp(CTtest_osignal_array[i]);
        }
        CTCAN_TestCase=Wait_for_Operator_Response;        
        break;

    case Wait_for_Operator_Response:
        if ( Check_for_Enter_Button() )
        {
            New_write_display("                    ",2,0);
            New_write_display("                    ",3,0);        
            CTCAN_TestCase = Send_CTCAN_Signal;   
        }
        break;

    case Send_CTCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

//-     SIU.GPDO[Z0_LED].R=0;  //Turn ON LED

        setoutp(CTtest_osignal_array[index]);

        CTCAN_TestCase=Check_CTCAN_Signal;
        timers[t_Generic_Delay]=0;
        sprintf(&LCD_Display[1][3],"%02i",index);
        break;

    case Check_CTCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

        if(rdinp(CTtest_isignal_array[index])!=1) {
            CTCAN_TestCase=CTCAN_test_Fail;
            break;
        } else {
//-         SIU.GPDO[Z0_LED].R=1;  //Turn OFF LED
            sprintf(&LCD_Display[1][12],"%02i",index);
        }

        index++;
        if(index<CTtest_signal_index) {
            CTCAN_TestCase=Send_CTCAN_Signal;
        } else {
            CTCAN_TestCase=CTCAN_test_Pass;
        }
        timers[t_Generic_Delay]=0;
        break;
/*
    case CTCAN_test_Pass:
        CurrentTestPassFail = 1; // pass
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        CTCAN_TestCase = Init_CTCAN_Test;
        New_write_display("   PASS             ",1,0);
        for(i=0; i<CTtest_signal_index; i++) {
            clroutp(CTtest_osignal_array[i]);
        }
        return DONE_BUT_CONTINUE_DISPLAY;
        break;

    case CTCAN_test_Fail:
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        CTCAN_TestCase = Init_CTCAN_Test;
        New_write_display("      FAILED        ",1,0);
        for(i=0; i<CTtest_signal_index; i++) {
            clroutp(CTtest_osignal_array[i]);
        }
        return RETEST_OPTION;
        break;
*/
    case CTCAN_test_Pass:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("CTCAN TEST          ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0);     
        CTCAN_TestCase = CTCAN_test_Done;        
        break;
        
    case CTCAN_test_Done:
//-     SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        CTCAN_TestCase = Init_CTCAN_Test; 
        if(Operation & 0x10 & consB[18])
        { // 16 = turn off LED's
            for(i=0; i<CTtest_signal_index; i++) {
                clroutp(CTtest_osignal_array[i]);
            }            
        } 
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) CTCAN_TestCase = CTCAN_test_Done;
        else if(RetestResponse == 1) CTCAN_TestCase = Init_CTCAN_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    case CTCAN_test_Fail:
//-    	clr_lcd_dply();
        New_write_display(TEST_CTCAN_Fault_Display[0],2,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_CTCAN_Fault_Display[1]);
        CTCAN_TestCase = Retest_option;
        break;

    default:
        CTCAN_TestCase=Init_CTCAN_Test;
        ReturnVal = DEFAULT;
        break;
    }

    return ReturnVal; // Required for Retest Option	
}


























