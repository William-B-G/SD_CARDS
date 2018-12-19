// Generic test  Generic_Test.c

#include "global.h"
#include "GALX_1132_TEST.h"

int16 Generic_Test(int16 LCD_Menu);

int16 GenericTestCase = 0;
int16 GenericFailedCase = 0;
int16 GenericPassedCase = 0;

enum {
    Init_Generic_Test,
    CheckTime,
    Generic_TestDone,
    Generic_TestFailed
};

int16 Generic_Test(int16 LCD_Menu)  // externed in // GALX_1132_TEST.h
{
    // remove or alter code lines with comment of "REMOVE OR MODIFY FOR ACTUAL TEST"
// return 0 when test is done; 1 if test is not done; -1 if
    // test is done but the display should remain until Mode Button. (wbg)

    static int8 count_num = 0;
    static int8 Debug_LED = 0;  // Change the state of Z0_LED
    static int8 GenericDebug_1 = 0;	// REMOVE OR MODIFY FOR ACTUAL TEST
    static int16  Force_Fail_Pass = 0; // REMOVE OR MODIFY FOR ACTUAL TEST
    int16 Menu_Index;

    Menu_Index = LCD_Menu;

    //  0 == turn led on; 1 == turn led OFF
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(GenericTestCase) {
    case Init_Generic_Test:
        clr_lcd_dply();
        GenericFailedCase = 0;
        timers[t_Generic_Delay] = 0;// t_Generic_Delay
        GenericTestCase = CheckTime;

        break;

    case CheckTime:  // PERFORM THE TESTING IN THIS CASE

        // REMOVE OR MODIFY FOR ACTUAL TEST
        New_write_display("GENERIC             ",0,0); // REMOVE OR MODIFY FOR ACTUAL TEST
        New_write_display("Testing XYZ(s)      ",1,0); // REMOVE OR MODIFY FOR ACTUAL TEST

        if(timers[t_Generic_Delay] >= 30 ) { // each delay is 100ms (30 = 3s)
            // "t_Generic_Delay" must be enum in control.c and control.h
            timers[t_Generic_Delay] = 0;
            count_num++;  // Test has not yet failed, keep testing
            // REMOVE OR MODIFY FOR ACTUAL TEST
            if(count_num > 3) { // REMOVE OR MODIFY FOR ACTUAL TEST
                count_num = 0;
                GenericTestCase = Generic_TestDone;
                clr_lcd_dply();
                break;
            }

            if(Force_Fail_Pass++ < 2) {	// REMOVE OR MODIFY FOR ACTUAL TEST
                // demo - force the failed case for two times
                // then let it pass
                GenericTestCase = Generic_TestFailed;

                //----------------------------------------
                New_write_display("GENERIC TEST FAILED ",0,0);
                New_write_display("IC17 R31            ",1,0);
                //----------------------------------------
                break;
            } // END OF "if(Time[0]!= Time[1]+1)"

        } // END OF "if(timers[tRTCDelay] >= 10)"
        break;

    case Generic_TestDone:
        New_write_display("GENERIC TEST PASS   ",0,0);
        New_write_display("GOOD JOB            ",1,0);
        Force_Fail_Pass =0;  // will force a fail next time
        // REMOVE OR MODIFY FOR ACTUAL TEST
        GenericTestCase=Init_Generic_Test;
        GenericDebug_1 = 0; // REMOVE OR MODIFY FOR ACTUAL TEST
        GenericFailedCase=0;
        return DONE_BUT_CONTINUE_DISPLAY; // Idle_time

        break;

    case Generic_TestFailed:
        switch(GenericFailedCase) {
        case 0:
            fail_testagain();
            GenericFailedCase=1;
            break;

        case 1:
            if(Check_for_Mode_Button() ) {
                GenericTestCase=Init_Generic_Test;
                GenericDebug_1 = 0;
                GenericFailedCase=0;
                return FINISHED_WITH_SERVICE; // Idle_time
                break;
            }
            if(Check_for_Enter_Button() ) {
                GenericTestCase=Init_Generic_Test;
                GenericDebug_1 = 0;
                GenericFailedCase = 0;
                break;
            }
            break;

        default:
            GenericFailedCase = 0;
            break;
        }
        break;

    default:
        GenericTestCase=Init_Generic_Test;
        New_write_display("GENERIC TEST        ",1,0);
        New_write_display("Executing...DEFAULT ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;	// 0

}