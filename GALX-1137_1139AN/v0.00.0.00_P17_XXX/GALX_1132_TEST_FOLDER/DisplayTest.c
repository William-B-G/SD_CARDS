#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



void Fill_A_Segment(int16 Row, int16 Col);
void FillAllSegments(int16 Special_Char);
void ShowTasks(void);

int16 Display_Test(int16 LCD_Menu);

int16 DisplayFailedCase = 0;
int16 DisplayPassedCase = 0;
static int16 Up_Dwn_Touched = 0;

enum {
    Init_Display_Test,
    CheckDisplay,
    Fill_Seqments,
    DisplaySegmentsOn,
    DisplayRestored,
    Display_TestFailed, // Required for Retest Option
    Display_TestPass, // Required for Retest Option
    Display_TestDone, // Required for Retest Option
    Retest_option  // Required for Retest Option
};

static int16 UpdateDisplay = 0;




#define TEST_Display_Fault_Display_Index	2
char TEST_Display_Fault_Display[TEST_Display_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC8, LCD1, CN12     ",	
};






int16 Display_Test(int16 LCD_Menu)  // externed in // GALX_1132_TEST.h
{
    static int8 count_num = 0;
    static int8 Debug_LED = 0;  // Change the state of Z0_LED
    static int8 DisplayDebug_1 = 0;	// REMOVE OR MODIFY FOR ACTUAL TEST
    static int16 Force_Fail_Pass = 0; // REMOVE OR MODIFY FOR ACTUAL TEST
    static int16 Special_Char_to_Display = -1;
    static int16 Row = 0;
    static int16 Col = 0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option

    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option

	static int16 DisplayTestCase=Init_Display_Test;
    int16 Menu_Index;
    int16 Previous_Special_Char_to_Display = 0;

    Menu_Index = LCD_Menu;
    Previous_Special_Char_to_Display = Special_Char_to_Display;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(DisplayTestCase) {
    case Init_Display_Test:
    	Clear_Log_Buffer(); 
        clr_lcd_dply();
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option
//        DoneButContinueDisplay = 1;
        Up_Dwn_Touched = 0;
        ShowTasks();
        DisplayFailedCase = 0;
        timers[t_Generic_Delay] = 0; // t_Display_Delay        
        Special_Char_to_Display = -1;
        DisplayTestCase = CheckDisplay;		
        break;

    case CheckDisplay:  // PERFORM THE TESTING IN THIS CASE
        if(Check_for_Up_Button()) {
            // display special characters or the menu
            timers[t_Generic_Delay] = 0;
            Up_Dwn_Touched = 1;
            Special_Char_to_Display++;
            if(Special_Char_to_Display >= 8) Special_Char_to_Display = -1;
        }
        if (Check_for_Down_Button()) {
            timers[t_Generic_Delay] = 0;
            Up_Dwn_Touched = 1;
            Special_Char_to_Display--;
            if(Special_Char_to_Display < -1) Special_Char_to_Display = 7;
        }
        if (Special_Char_to_Display != Previous_Special_Char_to_Display) {
            // do a display update
            switch(Special_Char_to_Display) {
            case -1:  // instructions
                ShowTasks();
                break;

            default:
                FillAllSegments(Special_Char_to_Display);
                break;
            }
        }
        if(Check_for_Enter_Button()) {
            if(Up_Dwn_Touched) {
                DisplayTestCase = Display_TestPass;
            }
        }
        if(Check_for_Mode_Button()) {
            if(Up_Dwn_Touched) 
            {
/* //-            
                clr_lcd_dply();
                New_write_display("ICxxx1, ICyyzz1     ",0,0); 
                New_write_display("ICxxx2, ICyyzz2     ",1,0);
                New_write_display("ICxxx3, ICyyzz3     ",2,0);
                New_write_display("ICxxx4, ICyyzz4     ",3,0); 
                Log_an_event("Test message 1"); 
                Log_an_event("Test message 2");
                Log_an_event("Test message 3"); 
                Log_an_event("Test message 4");
                Log_an_event("Test message 5"); 
                Log_an_event("Test message 6");
                Log_an_event("Test message 7"); 
                Log_an_event("Test message 8");
                Log_an_event("Test message 9");   
                DisplayTestCase = Retest_option;
                UpdateDisplay = 0;     
*/

				DisplayTestCase=Display_TestFailed;
                UpdateDisplay=0;	                          
            }
        }
        break;
        
    case Display_TestPass:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("Display Test        ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0); 
        DisplayTestCase = Display_TestDone;   
        break;
        
    case Display_TestFailed:
        CurrentTestPassFail = 0; // fail
     	clr_lcd_dply();
		New_write_display("Display Test        ",0,0);
        New_write_display(TEST_Display_Fault_Display[0],2,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_Display_Fault_Display[1]);
        DisplayTestCase = Retest_option; // Required for Retest Option
        UpdateDisplay = 0;
        ReturnVal = CONTINUE_SERVICE; // Idle_time    
        break;  
        
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) DisplayTestCase = Display_TestDone;
        else if(RetestResponse == 1) DisplayTestCase = Init_Display_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE; // Idle_time    
        break;             
        
    case Display_TestDone:
        DisplayTestCase = Init_Display_Test;
        UpdateDisplay = 0;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  
        break;        

    default:
        DisplayTestCase=Init_Display_Test;
        New_write_display("GENERIC TEST        ",1,0);
        New_write_display("Executing...DEFAULT ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }
    return ReturnVal; // Required for Retest Option	// 0
}

void Fill_A_Segment(int16 Row, int16 Col)
{
    if ( (Row < 4 ) && (Col < 20 ) ) {
        LCD_Display[Row][Col] = (char)All_Segs_On;
    }
}

void FillAllSegments(int16 Special_Char)
{
    int Row, Col;
    Row = 0;
    Col = 0;

    clr_lcd_dply();
    for (Row = 0; Row <= 3; Row++) {
        for(Col=0; Col<=19; Col++) {
//ZYM 2-6-2018
//Special Characters create by "load_special_char()"        
            LCD_Display[Row][Col] = (char)Special_Char;			
        }
    }
}

void ShowTasks(void)
{
    if (Up_Dwn_Touched) {
        New_write_display("Enter Btn 4 Pass.   ",0,0);
        New_write_display("                    ",1,0);
        New_write_display("Mode Btn 4 Fail.    ",2,0);
        New_write_display("                    ",3,0);
    } else {
        New_write_display("UP/Dwn Btn 2 scroll ",0,0);
        New_write_display("thru 8 diff displays",1,0);
        New_write_display("Mode Btn 4 Fail.    ",2,0);
        New_write_display("Enter Btn 4 Pass.   ",3,0);
    }
}
