#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



int16 TEST_GRCAN(int16 Operation);

enum {
    Init_GRCAN_Test,
    Wait_for_Operator_Response,
    Send_GRCAN_Signal,
    Check_GRCAN_Signal,
    Determine_Pass_Fail,
    GRCAN_test_Fail,
    GRCAN_test_Pass,
    GRCAN_test_Done,
    Retest_option // Required for Retest Option    
};

#define GRtest_signal_index 12
void *GRtest_osignal_array[GRtest_signal_index]= {
    o_GRo0,
    o_GRo1,
    o_GRo2,
    o_GRo3,
    o_GRo4,
    o_GRo5,
    o_GRo6,
    o_GRo7,
    o_GRo8,
    o_GRo9,
    o_GRo10,
    o_GRo11
};

void *GRtest_isignal_array[GRtest_signal_index]= {
    i_GRi0,
    i_GRi1,
    i_GRi2,
    i_GRi3,
    i_GRi4,
    i_GRi5,
    i_GRi6,
    i_GRi7,
    i_GRi8,
    i_GRi9,
    i_GRi10,
    i_GRi11
};



#define TEST_GRCAN_Fault_Display_Index	2
char TEST_GRCAN_Fault_Display[TEST_GRCAN_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC1,2,3,22   CN1    ",	
};




int16 TEST_GRCAN(int16 Operation)
{
    static int16 GRCAN_TestCase=Init_GRCAN_Test;
//  int16 Delay_Count= consB[20];
    int16 Delay_Count=test_halfsec_delay;
    static int16 Counter = 0;
    static int16 index=0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
        
    int16 i=0;

    switch (GRCAN_TestCase) {
    case Init_GRCAN_Test:
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option      
        Clear_Log_Buffer();  
        clr_lcd_dply();
        Counter = 0;       
        timers[t_Generic_Delay]=0;
        New_write_display("GRCAN TEST          ",0,0);
        New_write_display("TX:      RX:        ",1,0);
        New_write_display("Set correct switch  ",2,0);
        New_write_display("Press Enter when rdy",3,0);        
        index=0;
        for(i=0; i<GRtest_signal_index; i++) {
            clrgoutp(GRtest_osignal_array[i]);
        }
        GRCAN_TestCase = Wait_for_Operator_Response;  // Send_GRCAN_Signal;        
        break;

    case Wait_for_Operator_Response:
        if ( Check_for_Enter_Button() )
        {
            New_write_display("                    ",2,0);
            New_write_display("                    ",3,0);        
            GRCAN_TestCase = Send_GRCAN_Signal;   
        }
        break;
        
    case Send_GRCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

//-     SIU.GPDO[Z0_LED].R=0;  //Turn ON LED
        setgoutp(GRtest_osignal_array[index]);
        GRCAN_TestCase=Check_GRCAN_Signal;
        timers[t_Generic_Delay]=0;
        sprintf(&LCD_Display[1][3],"%02i",index);
        break;

    case Check_GRCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;
                
        if(rdginp(GRtest_isignal_array[index])!=1) {
            GRCAN_TestCase=GRCAN_test_Fail;
            break;
        } else {
//-         SIU.GPDO[Z0_LED].R=1;  //Turn OFF LED
            sprintf(&LCD_Display[1][12],"%02i",index);
        }
        index++;
        if(index<GRtest_signal_index) {
            GRCAN_TestCase=Send_GRCAN_Signal;
        } else {
            GRCAN_TestCase=GRCAN_test_Pass;
        }
        timers[t_Generic_Delay]=0;
        break;

    case GRCAN_test_Pass:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("GRCAN TEST          ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0); 
        GRCAN_TestCase = GRCAN_test_Done;        
        break;
        
    case GRCAN_test_Done:
//-     SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        GRCAN_TestCase = Init_GRCAN_Test; 
        if(Operation & 0x010 & consB[16])
        { // 16 = turn off LED's
            for(i=0; i<GRtest_signal_index; i++) {
                clrgoutp(GRtest_osignal_array[i]);
            }            
        } 
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
                
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) GRCAN_TestCase = GRCAN_test_Done;
        else if(RetestResponse == 1) GRCAN_TestCase = Init_GRCAN_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;           

    case GRCAN_test_Fail:;
//-    	clr_lcd_dply();
        New_write_display(TEST_GRCAN_Fault_Display[0],2,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_GRCAN_Fault_Display[1]);
        GRCAN_TestCase = Retest_option;
        break;

    default:
        GRCAN_TestCase=Init_GRCAN_Test;
        ReturnVal = DEFAULT;
        break;
    }
    return ReturnVal; // Required for Retest Option
}
































