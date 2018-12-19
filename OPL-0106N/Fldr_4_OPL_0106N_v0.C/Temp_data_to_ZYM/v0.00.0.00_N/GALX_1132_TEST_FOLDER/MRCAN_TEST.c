#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



int16 TEST_MRCAN(int16 Operation);

enum {
    Init_MRCAN_Test,
    Wait_for_Operator_Response,
    Send_MRCAN_Signal,
    Check_MRCAN_Signal,
    MRCAN_test_Fail,
    MRCAN_test_Pass,
    MRCAN_test_Done,
    Retest_option // Required for Retest Option    
};

#define MRtest_signal_index 12
void *MRtest_osignal_array[MRtest_signal_index]= {
    o_MRo0,
    o_MRo1,
    o_MRo2,
    o_MRo3,
    o_MRo4,
    o_MRo5,
    o_MRo6,
    o_MRo7,
    o_MRo8,
    o_MRo9,
    o_MRo10,
    o_MRo11
};

void *MRtest_isignal_array[MRtest_signal_index]= {
    i_MRi0,
    i_MRi1,
    i_MRi2,
    i_MRi3,
    i_MRi4,
    i_MRi5,
    i_MRi6,
    i_MRi7,
    i_MRi8,
    i_MRi9,
    i_MRi10,
    i_MRi11
};



#define TEST_MRCAN_Fault_Display_Index	2
char TEST_MRCAN_Fault_Display[TEST_MRCAN_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC21,23,25,26  CN9  ",	
};






int16 TEST_MRCAN(int16 Operation)
{
    static int16 MRCAN_TestCase=Init_MRCAN_Test;
//  int16 Delay_Count= consB[20];
    int16 Delay_Count=test_halfsec_delay;
    static int16 Counter = 0;
    static int16 index=0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option    
    int16 i=0;

    switch (MRCAN_TestCase) {
    case Init_MRCAN_Test:
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
        
        New_write_display("Machine Room CAN R  ",0,0);
        New_write_display("TX:      RX:        ",1,0);
        New_write_display("Set correct switch  ",2,0);
        New_write_display("Press Enter when rdy",3,0);  
//-     New_write_display("                    ",2,0);
//-     New_write_display("                    ",3,0);         
        index=0;
        for(i=0; i<MRtest_signal_index; i++) {
            clroutp(MRtest_osignal_array[i]);
        }
        MRCAN_TestCase=Wait_for_Operator_Response;    
//-		MRCAN_TestCase=Send_MRCAN_Signal;  
		timers[t_Generic_Delay]=0;
        break;

    case Wait_for_Operator_Response:
        if ( Check_for_Enter_Button() )
        {
            New_write_display("                    ",2,0);
            New_write_display("                    ",3,0);        
            MRCAN_TestCase = Send_MRCAN_Signal;   
        }
        break;
        
    case Send_MRCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

//-     SIU.GPDO[Z0_LED].R=0;  //Turn ON LED

        setoutp(MRtest_osignal_array[index]);

        MRCAN_TestCase=Check_MRCAN_Signal;
        timers[t_Generic_Delay]=0;
        sprintf(&LCD_Display[1][3],"%02i",index);
        break;

    case Check_MRCAN_Signal:
        if(timers[t_Generic_Delay]<Delay_Count)
            break;

        if(rdinp(MRtest_isignal_array[index])!=1) {
            MRCAN_TestCase=MRCAN_test_Fail;
            break;
        } else {
//-         SIU.GPDO[Z0_LED].R=1;  //Turn OFF LED
            sprintf(&LCD_Display[1][12],"%02i",index);
        }
        index++;
        if(index<MRtest_signal_index) {
            MRCAN_TestCase=Send_MRCAN_Signal;
        } else {
            MRCAN_TestCase=MRCAN_test_Pass;
        }
        timers[t_Generic_Delay]=0;
        break;

   case MRCAN_test_Pass:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("Machine Room CAN R  ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0); 
        MRCAN_TestCase = MRCAN_test_Done;        
        break;
        
    case MRCAN_test_Done:
//-     SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        MRCAN_TestCase = Init_MRCAN_Test; 
        if(Operation & 0x10 & consB[17])
        { // 16 = turn off LED's
            for(i=0; i<MRtest_signal_index; i++) {
                clroutp(MRtest_osignal_array[i]);
            }            
        } 
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
        
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) MRCAN_TestCase = MRCAN_test_Done;
        else if(RetestResponse == 1) MRCAN_TestCase = Init_MRCAN_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;    

    case MRCAN_test_Fail:
//-    	clr_lcd_dply();
        New_write_display(TEST_MRCAN_Fault_Display[0],2,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_MRCAN_Fault_Display[1]);
        MRCAN_TestCase = Retest_option;
        break;

    default:
        MRCAN_TestCase=Init_MRCAN_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }

    return ReturnVal; // Required for Retest Option
}

/* //-
int16 CheckPB(int16 which)
{// 
   int16 tempVar = 0;
   
   setoutp(o_out1);
   
   return FINISHED_WITH_SERVICE;    
}

int16 Set_out1_off(int16 which)
{// 
   int16 tempVar = 0;
   
   clroutp(o_out1);
   
   return FINISHED_WITH_SERVICE;    
}
*/





































