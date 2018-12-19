// MenuManager.c

#include "global.h"
/*
#include "SD_TEST.h"
#include "RTC_TEST.h"
#include "GALX_1132_TEST.h"
#include "MRCAN_TEST.h"
#include "SLCAN_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1137_1139.h"
#include "Menus.h"
*/
#include "Tests.h"

#define Last_Main_Menu 10

#define NORMAL 0
#define POP_MENU_PTR 1
#define LED_TOGGLE_DELAY 10 // 500

extern int16 Check_for_Mode_Button(void);
extern int16 Check_for_Up_Button(void);
extern int16 Check_for_Down_Button(void);
extern int16 Check_for_Enter_Button(void);
void ClearTheButtons(void);
int16 Clear_All_Boards(int16 Boards);

void GALX_1132_TEST(void);
void clr_lcd_dply(void);
void fail_testagain(void);
void MenuManager(void);
void Push_Menu_LIFO(int16 New_LCD_Menu);
int16 Pop_Menu_LIFO(void);
int16 ServiceTheButtons(void);
int16 Display_SW_Version(int16 New_LCD_Menu);
int16 Repaint_Retest_Display(void);
int16 DoNothing(int16 New_LCD_Menu);
int16 Really_DoNothing(int16 New_LCD_Menu);
int16 (*foo)(int16) = Display_Test;
int16 (*Service_Request)(int16); // function pointer
//taking one int16 param and
// returning an int16

int16 PopMenuPtrArray(void);
int16 ClearFaults(int16 dummy);
int16 QueryMenuVariables(int16 dummy);
int16 Start_SubMenu_B(void);
int16 Start_SubMenu_C(void);
int16 TimeForTheSnapShot(int16 dummy);

int16 TESTCASE_1132 = 0;
int16 Menu_LIFO_Index = 0;

int16 LCD_Menu = 0;
static int16 New_LCD_Menu = 0;
static int16 DoNothingCase = 0;
int16 LCD_Pointer = 0;
int16 LCD_Upd_Dpy = 0;
int16 LCD_Init = 0;
int16 LCD_Sub_Init = 0;
int16 line_ptr = 0;
int16 start_ptr = 0;
int16 i_index = 0;
int16 j_index = 0;
int16 k_index = 0;
static int16 parameter = 0;

int16 Retest_Delay = 0;
char Saved_Lines[4][21];
int16 Line_4_Blank = 1; // 1 = line 4 is a blank line
int16 (*ServiceProvider)(int16);

#pragma section const_type

struct a_menu *MainMenuPtr;
struct a_menu *MyMenuPtr;
struct a_menu *MenuPtr = NULL;

struct menu_stack The_menu_stack[MAX_MENU_DEPTH];

int16 TheMenuLength = 0;

enum
{
    Init_the_Menu,              // 0
    CAN_test,                   // 1
    RS_232_test,                // 2
    RS_485_test,                // 3
    Watch_dog_test,             // 4
    MRAM_test,                  // 5
    SD_Card_test,               // 6
    WIFI_test,                  // 7
    SW_version_display,         // 8
    RTC_test,                   // 9
    Push_a_button_test,         // 10
    Four_line_display_test,     // 11
    Adjust_LCD_display,         // 12
    GAL_IO_Board_test,          // 13
    Diagnostics_test,           // 14
    Generic_test,               // 15
    Scroll_Main_Menu,           // 16
    Perform_Main_Menu_Function, // 17
    Function_Pass,              // 18
    Function_Fail,              // 19
    Idle_Time_wbg,              // 20
    xxTest,                     // 21
    Wait_for_Service_Request,   // 22
    Retest,
    Retest_after_delay,
    Hall_Off_1,
    Hall_Off_2,
    Hall_Off_3,
    Reset_1,
    Reset_2,
    Reset_3,
    Reset_4,
    LookAtButtons,
    /*    
    
    Hall_Off_2,
    Hall_Off_3,
*/
    Wait_on_Operator_Response,
    DefaultCase
};

enum
{
    Init1132TEST,

    RS232_TEST,
    SD_TEST,
    CAN_TEST,
    RS485_TEST,
    RTC_TEST,
    PC104_TEST,
    VideoSPI_TEST,
    MRAM_TEST,
    GALX1132_TESTDONE,
    GALX1132_FAILED,

};

extern int32 test_msg;
extern int32 test_msg1;
extern int32 test_msg2;
extern int32 test_msg3;
extern int32 test_msg4;

//ZYM 2-2-2018
extern int16 StopOnFail;

int16 DoNothing(int16 New_LCD_Menu)
{
    int16 TheMenu = New_LCD_Menu;

    // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
    //- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED

    switch (DoNothingCase)
    {
    case Init_the_Menu:
        timers[t_Generic_Delay] = 0;
        DoNothingCase = Idle_Time_wbg;
        break;

    case Idle_Time_wbg:
        if (timers[t_Generic_Delay] >= 3)
        {
            DoNothingCase = Init_the_Menu;
            //-         SIU.GPDO[Z6_LED].R = 1;	// turn led OFF (wbg)// tagwbg 15
            return FINISHED_WITH_SERVICE;
        }
        break;

    default:
        DoNothingCase = Init_the_Menu;
        Clear_Msg_Board();
        Clear_Msg_Board_B();
        sprintf(src, "11: %d", New_LCD_Menu); // New_LCD_Menu // TheMenuLength
        Add_msgx_First_TimeOnly(src, 0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return FINISHED_WITH_SERVICE;
        break;
    }
    return CONTINUE_SERVICE;
}

int16 Really_DoNothing(int16 New_LCD_Menu)
{
    return FINISHED_WITH_SERVICE;
}

/**********************************************
* Function: void MenuManager(void)            *
* Description: The cases handles all actions  *
*    for managing the menus. It will call     *
*    the fucntion associated with the menu    *
*    item pointed to by the '>' and will      *
*    continue to call the function as long as *
*    the function returns 1 If the function   *
*    returns 0, the display is updated with   *
*    the current menu structure. If the       *
*    function returns -1, the display is not  *
*    updated until the mode button is pressed.*
*    This allows any error msg displays       *
*    generated by the called function to be   *
*    continuosly displayed until dismissed via*
*    The mode button. If the function returns *                                             *
*    2, the option is given to perform a      *
*    retest.                                  *
*                                             *                                             *
* Created: September, 2017                    *
*                                             *
* Example:                                    *
*                                             *
*                                             *
* Name: William Grauer                        *
*                                             *
**********************************************/

void MenuManager(void)
{
    // ********** THE NEW MAIN MASTER MENU **************
    static int16 Init_Main_Menu = 0;
    static int16 loop = 0;

    int16 RtnVal = 0;
    int16 Test_ConsB_24 = 0;

    //    MainMenuPtr = Main_MenuList; // Main_MenuList
    //    MyMenuPtr = MyMenuList;

    switch (New_LCD_Menu)
    {

    case Init_the_Menu: // 0
                        //-     SIU.GPDO[Z0_LED].R = 1;  // turn LED OFF
        loop = 0;
        parameter = 0;
        //        if(consB[24] & 0x11 == 0x11) Test_ConsB_24 = 0;
        //        else Test_ConsB_24 = 1;
        if ((Initial_Pwr_Up_Complete == 0) && (Test_ConsB_24))
            New_LCD_Menu = Reset_1; // Reset_1;
        else
            New_LCD_Menu = LookAtButtons;
        SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R; // toggle LED
        break;

    case LookAtButtons:
        SIU.GPDO[Z6_LED].R = 1; // OFF
        if (loop++ > LED_TOGGLE_DELAY)
        {
            SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R; // toggle LED
            loop = 0;
        }

        if (ServiceTheButtons() == POP_MENU_PTR)
        {                      // pop the menu ptr
            PopMenuPtrArray(); // if the mode button pressed, go up one level.
            // New_LCD_Menu = LookAtButtons;
            // the init_control() calls init_LCD() which
            //            ClearFaults(0);
        };
        // sets LCD_Upd_Dpy = 1 so that the first time
        // into ServiceTheButtons() the display will be updated.

        Periodic_Background_Task(0); // specific task based on the board being tested.
        break;

    case Wait_for_Service_Request: // Call back function to handle the selected test.
        //  RtnVal =FunctionPtrs[11](0); // produces compiler error
        //Service_Request = FunctionPtrs[11];  // ok
        RtnVal = Service_Request(parameter); // Runs the test or service requested.
        if (PIT.CVAL1.R < min_ISR_timer)
            min_ISR_timer = PIT.CVAL1.R; // save the slowest process
        switch (RtnVal)
        {
        case FINISHED_WITH_SERVICE:
            if (Enable_Display_ISR_timing)
                DisplayISR_timing(min_ISR_timer);
            New_LCD_Menu = LookAtButtons;
            LCD_Upd_Dpy = 1;
            break;

        case DONE_BUT_CONTINUE_DISPLAY:
            if (Enable_Display_ISR_timing)
                DisplayISR_timing(min_ISR_timer);
            //-         SIU.GPDO[Z0_LED].R = 1;	// turn led OFF
            New_LCD_Menu = Idle_Time_wbg;
            break;

        case RETEST_OPTION:
            New_LCD_Menu = Retest;
            break;

        case RETEST_OPTION_AFTER_DELAY:
            timers[t_Generic_Delay] = 0;
            New_LCD_Menu = Retest_after_delay;
            break;

        case MODE_BUTTON: // operator halted testing
            if (Enable_Display_ISR_timing)
                DisplayISR_timing(min_ISR_timer);
            New_LCD_Menu = LookAtButtons;
            LCD_Upd_Dpy = 1;
            break;
            
        case CONTINUE_SERVICE:
            // do nothing
            break;            

        default:
            break;
        }
        break;

    case Retest_after_delay:
        if (timers[t_Generic_Delay] >= 25)
            New_LCD_Menu = Retest;
        break;

    case Retest:
        //-     SIU.GPDO[Z6_LED].R = 1; // turn off LED
        //ZYM 1-18-2018
        //Show retest message
        //-     fail_testagain();
        LCD_Upd_Dpy = 1;
        New_LCD_Menu = Wait_on_Operator_Response;
        break;

    case Wait_on_Operator_Response:
        //-     SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED
        if (Check_for_Mode_Button())
            New_LCD_Menu = LookAtButtons;
        else if (Check_for_Enter_Button())
            New_LCD_Menu = Wait_for_Service_Request;
        break;

    case Idle_Time_wbg:           // Continue to display the test results until Mode Button pressed.
                                  //-     SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED
        Check_for_Up_Button();    // Just to clear out the button flag if someone presses button
        Check_for_Down_Button();  // dito
        Check_for_Enter_Button(); // dito
        if (Check_for_Mode_Button())
        {
            // this is the only button (in this case) that will cause an action.
            //-         SIU.GPDO[Z6_LED].R = 1; // turn off LED
            New_LCD_Menu = LookAtButtons;
            LCD_Upd_Dpy = 1;
        }
        break;

        // case DefaultCase:
        // no break;
    default:
        New_LCD_Menu = Init_the_Menu;
        Clear_Msg_Board();
        Clear_Msg_Board_B();
        sprintf(src, "11: %d", New_LCD_Menu); // New_LCD_Menu // TheMenuLength
        Add_msgx_First_TimeOnly(src, 0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        break;
    }
}

/**********************************************
* Function: int16 ServiceTheButtons(void)     *
* Description: Debounces the 4 push buttons:  *
*   'Up', 'Down', 'Mode', and 'Enter'.        *
*    For up/down, the cursor will be moved.   *
*    For 'Enter' it will set up the function  *
*    pointer. For 'Mode' it returns 1,        *
*    otherwise return 0. If 1 is returned, it *
*    is the signal for MenuManager() exit     *
*    the current menu and move up to the      *
*    parent menu.                             *
*                                             *
*    This function also updates the mcu       *
*    display buffer if necessary.             *
*                                             *
*    This is only called from the             *                                              
*    MenuManager()                            *
*                                             *
*                                             *                                              
* Created: September, 2017                    *
*                                             *
* Example:                                    *
*                                             *
*                                             *
* Name: William Grauer                        *
*                                             *
**********************************************/

int16 ServiceTheButtons(void)
{
    // the init_control() calls init_LCD() which
    // sets LCD_Upd_Dpy = 1 so that the first time
    // into ServiceTheButtons() the display will be updated.
    int16 i_index = 0;
    static last_time_tic = 0;

    if (Check_for_Mode_Button())
    {
        //-     SIU.GPDO[Z6_LED].R = 1;	// turn led OFF (wbg)// tagwbg 15
        New_LCD_Menu = Init_the_Menu;
        //        sprintf(src,"index: %d",MenuPtrArrayIndex);
        //        Log_a_fault(src,0);
        return POP_MENU_PTR; // 1 (the menu manager() will actually
        // do the poping up to the parent menu.)
    }

    if (Check_for_Enter_Button())
    {
        Service_Request = MenuPtr[LCD_Pointer].Service;
        parameter = MenuPtr[LCD_Pointer].param;
        // sets the function pointer to point to the test routine.
        New_LCD_Menu = Wait_for_Service_Request;
        clr_lcd_dply();
    }

    if (Check_for_Up_Button())
    {
        LCD_Pointer--; // increment to the next menu item
        if (LCD_Pointer < 1)
            LCD_Pointer = TheMenuLength;
        LCD_Upd_Dpy = 1;
    }

    if (Check_for_Down_Button())
    {
        LCD_Pointer++; // decrement to the next menu item
        if (LCD_Pointer > TheMenuLength)
            LCD_Pointer = 1;
        LCD_Upd_Dpy = 1;
    }

    if (LCD_Upd_Dpy == 1)
    { // Display must be updated
        LCD_Upd_Dpy = 0;
        line_ptr = (LCD_Pointer - 1) % 3;
        start_ptr = LCD_Pointer - line_ptr;
        for (i_index = 0; i_index <= 19; i_index++)
        {
            LCD_Display[0][i_index] = MenuPtr[0].MenuItem[i_index];
            LCD_Display[1][i_index] =
                MenuPtr[start_ptr].MenuItem[i_index];
            if ((start_ptr + 1) > TheMenuLength)
            {
                LCD_Display[2][i_index] = ' ';
                LCD_Display[3][i_index] = ' ';
            }
            else
            {
                LCD_Display[2][i_index] =
                    MenuPtr[start_ptr + 1].MenuItem[i_index];
                if ((start_ptr + 2) > TheMenuLength)
                    LCD_Display[3][i_index] = ' ';
                else
                    LCD_Display[3][i_index] =
                        MenuPtr[start_ptr + 2].MenuItem[i_index];
            }
        }

        LCD_Display[line_ptr + 1][0] = '>'; // show cursor
    }

    if (New_LCD_Menu != Idle_Time_wbg)
    { // see if time to display
        // version #
        if (timers[tsec] != last_time_tic)
        {
            if (timers[tsec] == 0)
            {
                for (i_index = 0; i_index <= 19; i_index++)
                {
                    LCD_Display[0][i_index] = MenuPtr[0].MenuItem[i_index];
                }
            }
            else if (timers[tsec] == 2)
            {
                New_write_display(Version_Display, 0, 0);
            }
        }
        last_time_tic = timers[tsec];
    }
    return NORMAL; // 0
} // END OF ServiceTheButtons()

int16 Display_SW_Version(int16 New_LCD_Menu)
{
    int16 Menu_Index;

    Menu_Index = New_LCD_Menu;
    New_write_display("                    ", 0, 0);
    New_write_display(" Software Version:  ", 1, 0);
    New_write_display("      01.01.01      ", 2, 0);

    debug_msg(VerDate, 3);

    if ((version / 10) == 0)
        LCD_Display[2][6] = ' ';
    else
        LCD_Display[2][6] = (char)((version / 10) + '0');

    LCD_Display[2][7] = (char)((version % 10) + '0');
    LCD_Display[2][8] = '.';
    LCD_Display[2][9] = (char)((revision / 10) + '0');
    LCD_Display[2][10] = (char)((revision % 10) + '0');

    LCD_Display[2][11] = '.';
    LCD_Display[2][12] = (char)((interim_revision / 10) + '0');
    LCD_Display[2][13] = (char)((interim_revision % 10) + '0');
    if (Sandbox_revision != '0')
    {
        LCD_Display[2][14] = '-'; // Sandbox_revision
        LCD_Display[2][15] = (char)Sandbox_revision;
    }

    // ZYM 12-13-2017
    //-	sprintf(&LCD_Display[0][1],"%15i",test_msg);
    return -1;
}

void clr_lcd_dply(void)
{
    New_write_display("                    ", 0, 0);
    New_write_display("                    ", 1, 0);
    New_write_display("                    ", 2, 0);
    New_write_display("                    ", 3, 0);
}

void fail_testagain(void)
{
    //    New_write_display("TEST FAILED (AGAIN?)",2,0);
    //    New_write_display("NO(MODE)/YES(ENTER) ",3,0);
    New_write_display("RETST? N/Y:MODE/ENTR", 3, 0);
}

int16 Clear_All_Boards(int16 Boards)
{
    if (Boards & 0x1)
        Clear_Msg_Board(); // board A
    if (Boards & 0x2)
        Clear_Msg_Board_B();
    if (Boards & 0x4)
        Clear_Msg_Board_C();

    return FINISHED_WITH_SERVICE;
}

int16 ClearFaults(int16 dummy)
{
    Clear_Msg_Board();
    //    clrgoutp(GRtest_osignal_array[7]);
    return FINISHED_WITH_SERVICE;
}

int16 SetClearForceToFail(int16 on_off)
{
    ForceTestToIndicateFail = on_off;
    return FINISHED_WITH_SERVICE;
}

int16 QueryMenuVariables(int16 dummy)
{
    // temporary funtion to assist in debugging multilevel
    // menus.
    sprintf(src, "ML: %d", TheMenuLength);
    Log_a_fault(src, 0);

    sprintf(src, "lcdP: %d", LCD_Pointer);
    Log_a_fault(src, 0);

    sprintf(src, "MenP: %d", MenuPtr);
    Log_a_fault(src, 0);

    return FINISHED_WITH_SERVICE;
}

void ClearTheButtons(void)
{
    // flush out all the buttons.
    Check_for_Up_Button();
    Check_for_Down_Button();
    Check_for_Mode_Button();
    Check_for_Enter_Button();
}

int16 ReinitGrCom1(int16 dummy)
{
    //init_sx_com_port_1();
    return FINISHED_WITH_SERVICE;
}

int16 ISR_test(int16 dummy)
{
    if (Enable_Display_ISR_timing)
        Enable_Display_ISR_timing = 0;
    else
    {
        Enable_Display_ISR_timing = 1;
        min_ISR_timer = 1087500 - 1;
        //        SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
    }
    return FINISHED_WITH_SERVICE;
}

void DisplayISR_timing(int32 time)
{
    sprintf(src, "ISR:%d: tic %d", time, Timer_tic);
    Log_an_event(src);
    min_ISR_timer = 1087500 - 1;
}

int16 Display_consB_file(int16)
{
    int16 i = 0;
    int16 ReturnVal = 0;
    static int16 BuffRead_operation = Init_BuffRead;

    switch (BuffRead_operation)
    {
    case Init_BuffRead:
        Clear_Log_Buffer();
        sprintf(src, "which= %d", WhichMenuToStart);
        for (i = 0; i < 31; i++)
        {
            sprintf(src, "consB[%d]=%d", i, consB[i]);
            Log_an_event(src);
        }
        BuffRead_operation = Display_Buff;
        break;

    case Display_Buff:
        ReturnVal = DisplayMsgBrd_A(0);
        if (ReturnVal == CONTINUE_SERVICE)
            return ReturnVal;
        // else
        BuffRead_operation = Init_BuffRead;
        return ReturnVal;
        break;

    default:
        BuffRead_operation = Init_BuffRead;
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;
}

int16 Handle_Retest_Request(int16 The_Kase)
{
    enum
    {
        NoRetest,
        PleaseRepeat,
        InitRetest,
        WaitForTimeout,
        WaitOnOperator,
        ViewMsgBd_A,
    };

    // return 0 for do not retest.
    // return 1 for try again.
    // return 2 for waiting for timeout.
    // return 3 for waiting for operator.

    // New Work for Nov 18

    static int16 Kase;
    static int16 Toggle = 0;
    int16 ReturnVal = 2;
    int16 MsgBd_A = 0; // holds the return val from msgBd_A
    int16 j, k = 0;

    if (The_Kase == 0)
        Kase = InitRetest;
    switch (Kase)
    {
    case InitRetest:
        // save line 4 of display in local buffer
        for (k = 0; k < 3; k++)
        {
            for (j = 0; j < 20; j++)
            {
                Saved_Lines[k][j] = LCD_Display[k][j];
                //              if(Saved_Lines[k][j] == NULL)break;
            }
        }
        Line_4_Blank = 1; // = 1 if Line 4 display was blank, set "Line_4_Blank" equal "1", NO NEED TO Toggle Line 4;
        for (j = 0; j < 20; j++)
        {
            Saved_Lines[3][j] = LCD_Display[3][j];
            //          if(Saved_Lines[3][j] == NULL)break;
            if (Saved_Lines[3][j] != 0x20)
                Line_4_Blank = 0; // = 0 if Line 4 display was NOT blank, set "Line_4_Blank" equal "0", NEED TO Toggle Line 4;
        }
        //          SIU.GPDO[Z6_LED].R = 0; // On
        timers[t_Generic_Delay] = 0;
        Retest_Delay = 10; // 10 = 1s
        Toggle = 0;
        Kase = WaitForTimeout;
        ReturnVal = WaitForTimeout;
        break;

    case WaitForTimeout: // in order to allow operator to read msgs
        Check_for_Mode_Button();
        Check_for_Enter_Button(); // Release Mode & Enter keys

        //ZYM 2-2-2018
        //Adding "if(StopOnFail==3)" to make sure that during "Test - do not stop",
        //the "Handle_Retest_Request" GOT BYPASSED, so that the "FunctionalTest()"
        //could perform the next test NONE STOP
        if (StopOnFail == 3)
        {
            Kase = InitRetest;
            ReturnVal = NoRetest;  // NoRetest = 0
        }
        else
        {
            if (timers[t_Generic_Delay] >= Retest_Delay)
            {
                //-           	SIU.GPDO[Z6_LED].R = 1; // turn off LED
                fail_testagain(); // display retest prompt on line 4
                LCD_Upd_Dpy = 1;
                timers[t_Generic_Delay] = 0;
                Retest_Delay = 30; // 30 = 3s
                Kase = WaitOnOperator;
                ReturnVal = WaitOnOperator; // WaitOnOperator  = 4
            }
            else
                ReturnVal = WaitForTimeout; // WaitForTimeout = 3
        }
        break;

    case WaitOnOperator:                 // After 3s change the display to show 4 lines of error msgs.
                                         // Then after another 3s show 3 lines of error msgs and the 'Retest' prompt on line 4.
                                         // Toggle between these 2 displays every 3s. Toggle early if the UP of DOWN buttons
                                         // are pressed.
        Toggle = Toggle_Display(Toggle); // toggle if 3s elapsed and line 4 is not blank.
        if (Check_for_Down_Button() || Check_for_Up_Button())
        {
            // Toggle as soon as the Up or Down button pressed, without waiting
            // for 3s.
            Retest_Delay = -1;
            Toggle = Toggle_Display(Toggle);
        }
        if (Check_for_Mode_Button())
        {
            clr_lcd_dply();
            if (LogIndex == 0)
            { // nothing in the buffer
                Kase = InitRetest;
                ReturnVal = NoRetest;  // NoRetest = 0
                break;
            }
            Kase = ViewMsgBd_A;
            //Kase = InitRetest;
            ReturnVal = ViewMsgBd_A; // ViewMsgBd_A = 5;
            break;
        }
        else if (Check_for_Enter_Button())
        {
            Kase = InitRetest;
            ReturnVal = PleaseRepeat; // PleaseRepeat = 1
            break;
        }
        else
            ReturnVal = WaitOnOperator;
        break;

    case ViewMsgBd_A:
        MsgBd_A = Display_Message_Board(0x8000);
        if (MsgBd_A == CLOSE_MSG_BOARD) // Enter button. CLOSE_MSG_BOARD = 4
        {                               //  repaint 4-line display and then go back to
                                        // WaitOnOperator
            Repaint_Retest_Display();
            //ZYM 2-2-2018
            //-             Kase = WaitOnOperator;
            //-             ReturnVal = WaitOnOperator;
            timers[t_Generic_Delay] = 0;
            Retest_Delay = 5; // 5 = 0.5s
            Kase = WaitForTimeout;
            ReturnVal = WaitForTimeout;
            break;
        }
        else if (MsgBd_A == FINISHED_WITH_SERVICE) // via Mode Btn
        { // Mode button
            Kase = InitRetest;
            ReturnVal = NoRetest; // NoRetest = 0
            break;
        }
        else
            ReturnVal = ViewMsgBd_A;
        break;

    default:
        Kase = InitRetest;
        ReturnVal = NoRetest;
    }
    return ReturnVal;
}

int16 Repaint_Retest_Display(void)
{
    int16 j, k = 0;
    for (k = 0; k < 4; k++)
    {
        for (j = 0; j < 20; j++)
        {
            LCD_Display[k][j] = Saved_Lines[k][j];
            //          if(LCD_Display[k][j] == NULL) break;
        }
    }
    return 0;
}

int16 Clear_Both_Boards(int16 dummy)
{
    ClearFaults(0);
    Clear_Msg_Board_B();
    return FINISHED_WITH_SERVICE;
}

int16 Toggle_Display(int16 Toggle)
{
    int16 j = 0;
    if (Line_4_Blank)
        return Toggle; // no need to switch displays
    // if line 4 is a blank line.
    if (timers[t_Generic_Delay] >= Retest_Delay)
    {
        timers[t_Generic_Delay] = 0;
        Retest_Delay = 30;
        if (Toggle == 1)
        {
            Toggle = 0;
            //           SIU.GPDO[Z6_LED].R = 1; // turn off LED
            for (j = 0; j < 21; j++)
            {
                LCD_Display[3][j] = Saved_Lines[3][j];
                //              if(LCD_Display[3][j] == NULL) break;
            }
        }
        else
        {
            Toggle = 1;
            //           SIU.GPDO[Z6_LED].R = 0; // turn LED On
            fail_testagain();
        }
    }
    return Toggle;
}
