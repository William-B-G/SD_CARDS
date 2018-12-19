#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



int16 TEST_MLCAN(int16 Operation);

enum {
    Init_MLCAN_Test,   
    Send_MLCAN_Signal,
    Check_MLCAN_Signal,
    MLCAN_test_Fail,
    MLCAN_test_Pass,
    MLCAN_test_Done,
    Retest_option // Required for Retest Option    
};

#define MLtest_signal_index 12
void *MLtest_osignal_array[MLtest_signal_index]= {
    o_MLo0,
    o_MLo1,
    o_MLo2,
    o_MLo3,
    o_MLo4,
    o_MLo5,
    o_MLo6,
    o_MLo7,
    o_MLo8,
    o_MLo9,
    o_MLo10,
    o_MLo11
};

void *MLtest_isignal_array[MLtest_signal_index]= {
    i_MLi0,
    i_MLi1,
    i_MLi2,
    i_MLi3,
    i_MLi4,
    i_MLi5,
    i_MLi6,
    i_MLi7,
    i_MLi8,
    i_MLi9,
    i_MLi10,
    i_MLi11
};



#define TEST_MLCAN_Fault_Display_Index	2
char TEST_MLCAN_Fault_Display[TEST_MLCAN_Fault_Display_Index][20]=
{
	"Failed.             ",
	"CN11,1,10           ",	
};



int16 TEST_MLCAN(int16 Operation)
{
    static int16 MLCAN_TestCase=0;
//  int16 Delay_Count= consB[20];
    int16 Delay_Count=test_halfsec_delay;
    static int16 Counter = 0;
    static int16 index=0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option    
    int16 i=0;

    switch (MLCAN_TestCase) {
    case Init_MLCAN_Test:
    	Clear_Log_Buffer(); 
        clr_lcd_dply();
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
//-     SIU.GPDO[Z0_LED].R=1;  //Turn OFF LED
//-     if(Delay_Count < 1)  Delay_Count = 1;
//-     if(Delay_Count > 20) Delay_Count = 20;
        
        New_write_display("Machine Room CAN L  ",0,0);
        New_write_display("TX:      RX:        ",1,0);
        New_write_display("                    ",2,0);
        New_write_display("                    ",3,0);         
        index=0;
        for(i=0; i<MLtest_signal_index; i++) {
            clroutp(MLtest_osignal_array[i]);
        }
		MLCAN_TestCase=Send_MLCAN_Signal;    		
		timers[t_Generic_Delay]=0;
        break;
        
    case Send_MLCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

//-     SIU.GPDO[Z0_LED].R=0;  //Turn ON LED

        setoutp(MLtest_osignal_array[index]);

        MLCAN_TestCase=Check_MLCAN_Signal;
        timers[t_Generic_Delay]=0;
        sprintf(&LCD_Display[1][3],"%02i",index);
        break;

    case Check_MLCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

        if(rdinp(MLtest_isignal_array[index])!=1) {
            MLCAN_TestCase=MLCAN_test_Fail;
            break;
        } else {
//-         SIU.GPDO[Z0_LED].R=1;  //Turn OFF LED
            sprintf(&LCD_Display[1][12],"%02i",index);
        }
        index++;
        if(index<MLtest_signal_index) {
            MLCAN_TestCase=Send_MLCAN_Signal;
        } else {
            MLCAN_TestCase=MLCAN_test_Pass;
        }
        timers[t_Generic_Delay]=0;
        break;

   case MLCAN_test_Pass:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("Machine Room CAN L  ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0); 
        MLCAN_TestCase = MLCAN_test_Done;        
        break;
        
    case MLCAN_test_Done:
        MLCAN_TestCase = Init_MLCAN_Test; 
        if(Operation & 0x10 & consB[17])
        { // 16 = turn off LED's
            for(i=0; i<MLtest_signal_index; i++) {
                clroutp(MLtest_osignal_array[i]);
            }            
        } 
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
        
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) MLCAN_TestCase = MLCAN_test_Done;
        else if(RetestResponse == 1) MLCAN_TestCase = Init_MLCAN_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;    

    case MLCAN_test_Fail:
//-    	clr_lcd_dply();
        New_write_display(TEST_MLCAN_Fault_Display[0],2,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_MLCAN_Fault_Display[1]);
        MLCAN_TestCase = Retest_option;
        break;

    default:
        MLCAN_TestCase=Init_MLCAN_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }

    return ReturnVal; // Required for Retest Option
}





























































