#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



int16 TEST_RTC(int16 LCD_Menu);



int16 RTCFailedCase = 0;

enum {
    InitRTCTest,
    CheckTime,
    RTCTestDone,
    RTCTestFailed,
    RTCTestPassed,
    Retest_option  // Required for Retest Option
};





#define TEST_RTC_Fault_Display_Index	2
char TEST_RTC_Fault_Display[TEST_RTC_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC17,14             ",	
};






int16 TEST_RTC(int16 LCD_Menu)
{
    // return 0 when test is done; 1 if test is not done; -1 if
    // test is done but the display should remain until Mode Button. (wbg)
    static int8 Time[8]= {0,0,0,0,0,0,0,0};
    static int16 DotDisplayCase = 0;
    static int8 count_num=0;
    int16 Menu_Index;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option

    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option    
	
	static int16 RTCTestCase = InitRTCTest;
	
    Menu_Index = LCD_Menu;
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED
    //  0 == turn led on; 1 == turn led OFF
    switch(RTCTestCase) {
    case InitRTCTest:
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option 
        Clear_Log_Buffer();        
        clr_lcd_dply();
        RTCFailedCase = 0;        
        local_gettime();
        sprintf(src,"RTC TEST %d/%d/%d", d.year, d.month, d.day );              
          New_write_display(src,0,0); // date        
        //New_write_display("RTC TEST            ",0,0);
        New_write_display("Executing...        ",1,0);
        DotDisplayCase = 0;
        timers[tRTCDelay] = 0;                
        Time[0]=t.second;
        Time[1]=t.second;
        if(t.second<30) {
            Time[2]=t.second+30;
        } else {
            Time[2]=t.second-30;
        }
        if(ForceTestToIndicateFail)
        {
          RTCTestCase = RTCTestFailed;              
        }
        else RTCTestCase = CheckTime;        
        break;

    case CheckTime:
        if(timers[tRTCDelay] >= 10) {
            timers[tRTCDelay] = 0;
            local_gettime();
            Time[7]=(char)d.year;
            Time[6]=d.month;
            Time[5]=d.day;
            Time[4]=t.hour;
            Time[3]=t.minute;
            Time[0]=t.second;
            if(Time[0]==0) {
                Time[1]=-1;
            }
            if(Time[0]!= Time[1]+1) {
                RTCTestCase = RTCTestFailed;
                //----------------------------------------
//-             New_write_display("RTC TEST FAIL       ",0,0);
//-             New_write_display("IC17 R31            ",1,0);
                //----------------------------------------
                break;
            } // END OF "if(Time[0]!= Time[1]+1)"
            count_num++;
            Time[1]++;
            if(count_num > 29) {
                count_num = 0;
                RTCTestCase = RTCTestPassed;                
                break;
            }
            if(DotDisplayCase == 0) {
                LCD_Display[1][10] = '.';
                LCD_Display[1][11] = ' ';
                LCD_Display[1][12] = ' ';
                DotDisplayCase = 1;
            } else if(DotDisplayCase == 1) {
                LCD_Display[1][10] = '.';
                LCD_Display[1][11] = '.';
                LCD_Display[1][12] = ' ';
                DotDisplayCase = 2;
            } else if(DotDisplayCase == 2) {
                LCD_Display[1][10] = '.';
                LCD_Display[1][11] = '.';
                LCD_Display[1][12] = '.';
                DotDisplayCase = 3;
            } else if (DotDisplayCase == 3) {
                LCD_Display[1][10] = ' ';
                LCD_Display[1][11] = ' ';
                LCD_Display[1][12] = ' ';
                DotDisplayCase = 0;
            }
            New_write_display("RTC:  :  :          ",2,0);
            New_write_display("Testing RTC(s):     ",3,0);
            sprintf(&LCD_Display[1][17],"%02i",count_num);
            sprintf(&LCD_Display[2][4],"%02i",Time[4]);
            sprintf(&LCD_Display[2][7],"%02i",Time[3]);
            sprintf(&LCD_Display[2][10],"%02i",Time[0]);
            sprintf(&LCD_Display[3][15],"%02i",Time[1]);
            LCD_Display[1][19]='s';
            LCD_Display[2][6]=':';
            LCD_Display[2][9]=':';
            LCD_Display[2][12]=' ';
            LCD_Display[3][17]=' ';
        } // END OF "if(timers[tRTCDelay] >= 10)"
        break;

    case RTCTestDone:
        RTCTestCase=InitRTCTest;
        RTCFailedCase=0;         
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY; 
        break;

    case RTCTestPassed:
        CurrentTestPassFail =1;  
        clr_lcd_dply();
        New_write_display("RTC TEST            ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0); 
        RTCTestCase = RTCTestDone;      
        break;
        
    case RTCTestFailed:
        CurrentTestPassFail =0;  // fail
        RTCTestCase = Retest_option; // Required for Retest Option
//-    	clr_lcd_dply();
        New_write_display(TEST_RTC_Fault_Display[0],1,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_RTC_Fault_Display[1]);
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option  
        break;
        
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) RTCTestCase = RTCTestDone;
        else if(RetestResponse == 1) RTCTestCase = InitRTCTest; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;   

    default:
        RTCTestCase=InitRTCTest;
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DEFAULT;
        break;
    }
    return ReturnVal;
}
































