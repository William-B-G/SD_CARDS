#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"

#include "fat_filelib.h"
#include <string.h>




int16 TEST_SD(int16 LCD_Menu);



enum {
    InitSDTest,
    SetUpSPISD,
    ReadSDCard,
    CompareString,
    SDTestDone,
    SDTestFailed,
    SDTestPassed,
    Display_TestFailed, // Required for Retest Option
    Display_TestPass, // Required for Retest Option
    Display_TestDone, // Required for Retest Option
    Retest_option  // Required for Retest Option    
};

//SD Card got ONLY one scentence :"GAL OF EXCELLENCE   " NO CR and LF
//-char SD_STRING[] = "ABCDEFGHIJKLMNOPQRST";
char SD_STRING[] = "GAL OF EXCELLENCE   ";
char MY_STRING[] = "                    ";




#define TEST_SD_Fault_Display_Index	2
char TEST_SD_Fault_Display[TEST_SD_Fault_Display_Index][20]=
{
	"Failed.             ",
	"CN5  R23->R29       ",	
};




int16 TEST_SD(int16 LCD_Menu)
{
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    FL_FILE *fp1;
    int i;
    int16 Menu_Index;
	static int16 SDTestCase=InitSDTest;
    Menu_Index = LCD_Menu;
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED

    switch(SDTestCase) {
    case InitSDTest:
        CurrentTestPassFail =0;  // fail
        Clear_Log_Buffer(); 
        clr_lcd_dply();
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        New_write_display("SD TEST             ",0,0);
        New_write_display("Executing...        ",1,0);
        for(i=0; i<20; i++) MY_STRING[i] = ' ';

        SDTestCase = SetUpSPISD;
        break;
        
    case SetUpSPISD:
        if(sd_initialization() != 0) {
            SDTestCase = SDTestFailed;
            CurrentTestPassFail = 0; // 0 = Fail; 1 = Pass
            //----------------------------------------
//-         New_write_display("SD TEST FAIL        ",0,0);
//-         New_write_display("CN5, R23->R29       ",1,0);
            //----------------------------------------
            break;
        } else
            SDTestCase = ReadSDCard;
        timers[tSDDelay] = 0;
        break;
        
    case ReadSDCard:
        if(timers[tSDDelay]<40)
            break;
        fl_init();
        if (fl_attach_media(media_read, media_write) != FAT_INIT_OK) {
            SDTestCase = SDTestFailed;
            CurrentTestPassFail = 0; // 0 = Fail; 1 = Pass
            //----------------------------------------
//-           New_write_display("SD TEST FAIL        ",0,0);
//-           New_write_display("CN5, R23->R29       ",1,0);
            //----------------------------------------
            break;
        } else {
            fp1 = fl_fopen("/SD_TestVector.DAT","r");
            if(fp1 == NULL) {
                SDTestCase = SDTestFailed;
                fl_fclose(fp1);
                //----------------------------------------
//-             New_write_display("SD TEST FAIL        ",0,0);
//-             New_write_display("CN5, R23->R29       ",1,0);
                //----------------------------------------
                break;
            } else {
                for(i= 0; i<20; i++) {
                    MY_STRING[i] = (char)fl_fgetc(fp1);
                    //SD Card got ONLY one scentence :"GAL OF EXCELLENCE   " NO CR and LF
//-						fl_fgetc(fp1);   // skip CR
//-						fl_fgetc(fp1);   // skip LF
                }
                SDTestCase = CompareString;
                timers[tSDDelay] = 0;
                New_write_display(MY_STRING,2,0);
                New_write_display(SD_STRING,3,0);
            }
            fl_fclose(fp1);
        }
        break;

    case CompareString:
        if(timers[tSDDelay]<40)
            break;
//ZYM
///			for(i = 0;i<20;i++)
///			{
        if(strcmp(MY_STRING,SD_STRING)!=0)  // will pass
//				if(strcmp(MY_STRING,"ABCDEFG")!=0)  // will FAIL
///				if(MY_STRING[i] != SD_STRING[i])
        {
            SDTestCase = SDTestFailed;
            break;
        }
///			}
        SDTestCase = SDTestPassed;        
        break;
        
    case SDTestDone:
        SDTestCase = InitSDTest;
        SDTestCase=0;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  
        break;
        
    case SDTestFailed:
//-    	clr_lcd_dply();
        CurrentTestPassFail = 0; // 0 = Fail; 1 = Pass
        //----------------------------------------
//-     New_write_display("SD TEST FAIL        ",0,0);
//-     New_write_display("CN5, R23->R29       ",1,0);
        //----------------------------------------        
        New_write_display(TEST_SD_Fault_Display[0],2,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_SD_Fault_Display[1]);
        SDTestCase = Retest_option; // Required for Retest Option
        ReturnVal = CONTINUE_SERVICE;         
        break;

     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) SDTestCase = SDTestDone;
        else if(RetestResponse == 1) SDTestCase = InitSDTest; 
        else 
        ReturnVal = CONTINUE_SERVICE; // Idle_time    
        break;           
        
    case SDTestPassed:
    	clr_lcd_dply();
    	New_write_display("SD TEST             ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0);             
        CurrentTestPassFail = 1; // 0 = Fail; 1 = Pass
        SDTestCase = SDTestDone;        
        break;                       

    default:
        SDTestCase=InitSDTest;
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        ReturnVal = DEFAULT;
        break;
    }
    return ReturnVal; // Required for Retest Option	
}

















































