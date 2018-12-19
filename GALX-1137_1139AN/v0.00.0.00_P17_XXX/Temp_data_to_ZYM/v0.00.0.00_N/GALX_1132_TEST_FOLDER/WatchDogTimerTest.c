// WatchDogTimerTest.c


#include "global.h"
#include "GALX_1132_TEST.h"

int16 WDT_Test(int16 Delay);

int16 WDTTestCase =  0;
int16 WDTFailedCase = 0;
int16 WDTPassedCase = 0;

enum {
    Init_WDT_Test,
    CheckTime,
    WDT_TestDone,
    WDT_TestFailed
};

int16 WDT_Test(int16 Delay)  // externed in // GALX_1132_TEST.h
{
    // remove or alter code lines with comment of "REMOVE OR MODIFY FOR ACTUAL TEST"
// return 0 when test is done; 1 if test is not done; -1 if
    // test is done but the display should remain until Mode Button. (wbg)

    static int8 count_num = 0;
    static int8 Debug_LED = 0;  // Change the state of Z0_LED
    static int8 WDTDebug_1 = 0;	// REMOVE OR MODIFY FOR ACTUAL TEST
    static int16 Force_Fail_Pass = 0; // REMOVE OR MODIFY FOR ACTUAL TEST
    int16 Menu_Index;

    Menu_Index = LCD_Menu;
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(WDTTestCase) {
    case Init_WDT_Test:
        clr_lcd_dply();
        WDTFailedCase = 0;
        timers[t_Generic_Delay] = 0;// t_WDT_Delay
        WDTTestCase = CheckTime;
        break;

    case CheckTime:
    // SWT.TO.R = 0x00F42400; // 16 MHz * 1.00 (1 sec) = 16000000 = 0x00F42400
        if(Delay == 0) SWT.TO.R = 1; // trip out right away.
        New_write_display("WDT TEST            ",0,0);
        New_write_display("WILL RESET IN 3s    ",1,0);

        if(timers[t_Generic_Delay] >= Delay ) { // each delay is 100ms (30 = 3s)
            // "t_WDT_Delay" must be enum in control.c and control.h

            // Now force a reset
//            SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
            while(1) {  // fall in here after 3s and never return
                timers[t_Generic_Delay] = 0;
            }
            count_num++;  // Test has not yet failed, keep testing
            if(count_num > 3) {
                count_num = 0;
                WDTTestCase = WDT_TestDone;
                clr_lcd_dply();
                break;
            }
            if(Force_Fail_Pass++ < 2) {	// REMOVE OR MODIFY FOR ACTUAL TEST
                // demo - force the failed case for two times
                // then let it pass
                WDTTestCase = WDT_TestFailed;
                //----------------------------------------
                New_write_display("GENERIC TEST FAILED ",0,0);
                New_write_display("IC17 R31            ",1,0);
                //----------------------------------------
                break;
            } // END OF "if(Time[0]!= Time[1]+1)"
        } // END OF "if(timers[tRTCDelay] >= 10)"
        break;

    case WDT_TestDone:
        New_write_display("GENERIC TEST PASS   ",0,0);
        New_write_display("GOOD JOB            ",1,0);
        Force_Fail_Pass =0;  // will force a fail next time
        // REMOVE OR MODIFY FOR ACTUAL TEST
        WDTTestCase=Init_WDT_Test;
        WDTDebug_1 = 0; // REMOVE OR MODIFY FOR ACTUAL TEST
        WDTFailedCase=0;
//               SIU.GPDO[Z0_LED].R = 1;	// turn led OFF
        return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        break;

    case WDT_TestFailed:
        WDTTestCase=Init_WDT_Test;
        return RETEST_OPTION_AFTER_DELAY  ;        
        break;

    default:
        WDTTestCase=Init_WDT_Test;
        New_write_display("GENERIC TEST        ",1,0);
        New_write_display("Executing...DEFAULT ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        break;
    }
    return CONTINUE_SERVICE;	// 0

}