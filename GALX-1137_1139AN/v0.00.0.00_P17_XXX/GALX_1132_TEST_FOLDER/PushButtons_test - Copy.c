// PushButtons_test.c

#include "global.h"
#include "GALX_1132_TEST.h"

int16 Verify_only_one_Button(void);
int16 Buttons_State(void);

enum {
    Init_PushButtons_Test,
    Push_Up_Button_wait,
    Push_Down_Button_wait,
    Push_Mode_Button_wait,
    Push_Enter_Button_wait,
    Check_for_only_UpButton,
    Check_for_only_DownButton,
    Check_for_only_ModeButton,
    Check_for_only_EnterButton,
    Fill_Seqments,
    PushButtonsSegmentsOn,
    PushButtonsRestored,
    PushButtons_TestDone,
    PushButtons_TestFailed,
    Failed_But_Delay,
    GeneralWaitState
};

enum {
    For_Up_Button,
    For_Down_Button,
    For_Mode_Button,
    For_Enter_Button
};

int16 PushButtons_Test(int16 LCD_Menu)  // externed in // GALX_1132_TEST.h
{
    static int8  count_num = 0;
    static int8 Debug_LED = 0;  // Change the state of Z0_LED
    static int8 PushButtonsDebug_1 = 0;
    static int16 Up_Dwn_Touched = 0;
    static int16 PushButtonsTestCase = 0;
    static int16 NextCaseAfterWait = 0;
    static int16 The_Button_State =0;
    static int16 The_Return_Val =0;

    int16 Menu_Index;

    Menu_Index = LCD_Menu;
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(PushButtonsTestCase) {
    case Init_PushButtons_Test:
        CurrentTestPassFail =0;  // fail
        New_write_display("    Do not press any",0,0);
        New_write_display("push-button until   ",1,0);
        New_write_display("requested.          ",2,0);
        New_write_display("                    ",3,0);
        LCD_Display[3][3] =  (char)Show_Button_Up;
        LCD_Display[3][8] =  (char)Show_Button_Up;
        LCD_Display[3][13] = (char)Show_Button_Up;
        LCD_Display[3][18] = (char)Show_Button_Up;
        timers[t_Generic_Delay] = 0; // t_PushButtons_Delay
        Up_Dwn_Touched = 0;
        The_Button_State =0;
        The_Return_Val =0;
        PushButtonsTestCase = Push_Up_Button_wait;

        break;

    case Push_Up_Button_wait:
        if(timers[t_Generic_Delay] >= 30 ) {
            // each delay is 100ms (30 = 3s)
            // first check that no button is pressed.

            if ((The_Return_Val=Buttons_State()) != 0) {
                // error
                PushButtonsTestCase = PushButtons_TestFailed;
            } else {
                timers[t_Generic_Delay] = 0; // t_PushButtons_Delay
                New_write_display("Please press and    ",0,0);
                New_write_display("hold down the Up Key",1,0);
                New_write_display("                    ",2,0);
                //	New_write_display("                    ",3,0);
                LCD_Display[3][3] =  (char)Show_Button_Down;
                LCD_Display[3][8] =  (char)Show_Button_Up;
                LCD_Display[3][13] = (char)Show_Button_Up;
                LCD_Display[3][18] = (char)Show_Button_Up;
                NextCaseAfterWait = Check_for_only_UpButton;
                PushButtonsTestCase = GeneralWaitState;
            }
        }
        break;

    case GeneralWaitState:
        if(timers[t_Generic_Delay] >= 30 ) {
            // each delay is 100ms (30 = 3s)
            timers[t_Generic_Delay] = 0; // t_PushButtons_Delay
            PushButtonsTestCase = NextCaseAfterWait;
        }
        break;

    case Push_Down_Button_wait:
        if(timers[t_Generic_Delay] >= 30 ) {
            timers[t_Generic_Delay] = 0;
            New_write_display("Now Please press and",0,0);
            New_write_display("hold the Down Key   ",1,0);
            LCD_Display[3][3] =  (char)Show_Button_Up;
            LCD_Display[3][8] =  (char)Show_Button_Down;
            LCD_Display[3][13] = (char)Show_Button_Up;
            LCD_Display[3][18] = (char)Show_Button_Up;
            NextCaseAfterWait = Check_for_only_DownButton;
            PushButtonsTestCase = GeneralWaitState;
        }
        break;

    case Push_Mode_Button_wait:
        if(timers[t_Generic_Delay] >= 30 ) {
            timers[t_Generic_Delay] = 0;
            New_write_display("Now Please press and",0,0);
            New_write_display("hold the Mode Key   ",1,0);
            LCD_Display[3][3] =  (char)Show_Button_Up;
            LCD_Display[3][8] =  (char)Show_Button_Up;
            LCD_Display[3][13] = (char)Show_Button_Down;
            LCD_Display[3][18] = (char)Show_Button_Up;
            NextCaseAfterWait = Check_for_only_ModeButton;
            PushButtonsTestCase = GeneralWaitState;
        }
        break;

    case Push_Enter_Button_wait:
        if(timers[t_Generic_Delay] >= 30 ) {
            timers[t_Generic_Delay] = 0;
            New_write_display("Now Please press and",0,0);
            New_write_display("hold the Enter Key  ",1,0);
            LCD_Display[3][3] =  (char)Show_Button_Up;
            LCD_Display[3][8] =  (char)Show_Button_Up;
            LCD_Display[3][13] = (char)Show_Button_Up;
            LCD_Display[3][18] = (char)Show_Button_Down;
            NextCaseAfterWait = Check_for_only_EnterButton;
            PushButtonsTestCase = GeneralWaitState;
        }
        break;

    case Check_for_only_UpButton:
        if ((The_Return_Val=Buttons_State()) != 8) {
            // error
            PushButtonsTestCase = PushButtons_TestFailed;
        } else PushButtonsTestCase = Push_Down_Button_wait;
        break;

    case Check_for_only_DownButton:
        if ((The_Return_Val=Buttons_State()) != 4) {
            // error
            PushButtonsTestCase = PushButtons_TestFailed;
        } else PushButtonsTestCase = Push_Mode_Button_wait;
        break;
    case Check_for_only_ModeButton:
        if ((The_Return_Val=Buttons_State()) != 2) {
            // error
            PushButtonsTestCase = PushButtons_TestFailed;
        } else PushButtonsTestCase = Push_Enter_Button_wait;
        break;

    case Check_for_only_EnterButton:
        if ((The_Return_Val=Buttons_State()) != 1) {
            // error
            PushButtonsTestCase = PushButtons_TestFailed;
        } else {
            New_write_display("RELEASE ALL KEYS    ",0,0);
            New_write_display("                    ",1,0);
            LCD_Display[3][3] =  (char)Show_Button_Up;
            LCD_Display[3][8] =  (char)Show_Button_Up;
            LCD_Display[3][13] = (char)Show_Button_Up;
            LCD_Display[3][18] = (char)Show_Button_Up;
            NextCaseAfterWait = PushButtons_TestDone;
            PushButtonsTestCase = GeneralWaitState;
        }

        break;

    case PushButtons_TestDone:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("BUTTONS TEST PASS   ",0,0);
        New_write_display("GOOD JOB            ",1,0);
        Check_for_Up_Button();
        Check_for_Down_Button();
        Check_for_Mode_Button();
        Check_for_Enter_Button();  // Release all keys
        PushButtonsTestCase = Init_PushButtons_Test;
        return DONE_BUT_CONTINUE_DISPLAY;

        break;

    case Failed_But_Delay:
        New_write_display("RELEASE ALL KEYS    ",0,0);
        New_write_display("                    ",1,0);
        LCD_Display[3][3] =  (char)Show_Button_Up;
        LCD_Display[3][8] =  (char)Show_Button_Up;
        LCD_Display[3][13] = (char)Show_Button_Up;
        LCD_Display[3][18] = (char)Show_Button_Up;
        NextCaseAfterWait = PushButtons_TestFailed;
        PushButtonsTestCase = GeneralWaitState;
        break;

    case PushButtons_TestFailed:
        The_Button_State = Buttons_State();  // Which buttons are pressed.
        PushButtonsTestCase = Init_PushButtons_Test;
        clr_lcd_dply();
        New_write_display("Button Test FAILED. ",0,0);
        sprintf(&LCD_Display[1][10],"%02i",The_Button_State);
        sprintf(&LCD_Display[2][10],"%02i",The_Return_Val);
        Check_for_Up_Button();
        Check_for_Down_Button();
        Check_for_Mode_Button();
        Check_for_Enter_Button();  // Release all keys
        if(CurrentTestPassFail == 0)  
            return RETEST_OPTION_AFTER_DELAY; 
                else return DONE_BUT_CONTINUE_DISPLAY;                
        break;

    default:
        PushButtonsTestCase=Init_PushButtons_Test;
        clr_lcd_dply();
        New_write_display("BUTTON TEST (error) ",1,0);
        New_write_display("default case taken  ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        //return DONE_BUT_CONTINUE_DISPLAY;
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;	// 1

}

int16 Buttons_State()
{
    int16 The_State = 0;

    if( rdLCDio(Lcd_UP) != 0) { // up key pressed
        The_State |= 0x8;
    }
    if( rdLCDio(Lcd_DN) != 0) { // down key pressed
        The_State |= 0x4;
    }
    if( rdLCDio(Lcd_MODE) != 0) { // mode key pressed
        The_State |= 0x2;
    }
    if( rdLCDio(Lcd_ENTER) != 0) { // enter key pressed
        The_State |= 0x1;
    }

    return The_State;
}


int16 Verify_only_one_Button(void)
{
    int16 The_Buttons = 0;

    The_Buttons = ( rdLCDio(Lcd_UP) << 3 ) | ( rdLCDio(Lcd_DN) << 2 ) |
                  ( rdLCDio(Lcd_MODE) << 1 ) | ( rdLCDio(Lcd_ENTER) ) ;
    return The_Buttons;
}
