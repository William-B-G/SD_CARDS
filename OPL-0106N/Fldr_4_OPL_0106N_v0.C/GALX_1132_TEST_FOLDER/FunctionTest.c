// FunctionTest.c


#include "global.h"
#include "Tests.h"

#define TRUE   1
#define FALSE  0
#define PASS   1
#define FAIL   0
#define OK     1
#define ATTEMPT_TO_TEST     0x01
#define TEST_HAS_BEEN_DONE  0x04
#define TEST_FAILED         0x02
#define TEST_PASSED         0x02
#define BITS_1_2            0x06
#define NOT_BITS_1_2        0xfff9
#define SHOWSTARTUPMSG      0x010
#define SHOWPASSFAILMSG     0x020
#define DEFAULT_TEST_MASK   0x031
#define TESTMASK            0x0100

extern int16 LCD_Upd_Dpy;

int16 testmask = 0x100;
int16 Options = 0;
void reset_Functional_Flags(int16 max_FTMenu_Index);
int16 SetUpNextTest(int16 max_FTMenu_Index);
void SaveTestResult(int16 test_index);
int16 CheckForLongModeButton(int16 maxCounter);


//ZYM 2-2-2018
int16 StopOnFail         		= 0;
//-static int16 StopOnFail         = 0;  // 1 = halt all testing on a failure. 3 = continue with testing.
// 2 = do not retest functions that have already passed.


static int16 tst_param          = 0;  // the parameter to pass to the test. Comes from Functional_Menu[]
static int16 FTMenu_Index       = 0;
static int16 pauseDelayTime     = 10; // 10 = 1s
static int16 max_FTMenu_Index   = 0;
static int16 CancelCounter      = 0; // to cancel the functional testing


struct functions_to_test {
    void *Service;  // function pointer
    int16 param;    // the parameter to pass
    int16 OptionsMask;     // various options and a record of the last pass(1)/fail(0) status
    // bit 0 = Do not perform test(0), or OK to attempt test(1)
    // bits 1,2 = 00 = test has not yet been performed; 10 = test failed; 11 = test passed.
    // bit 3 = 1: do not monitor 'Mode' button during test.
    // bit 4 = OK to display startup text(1), or do not display startup text(0).
    // bit 5 = OK to display pass/fail text(1), or do not display pass/fail text(0).
    // bit 6 = 0: Save the results; 1: do not save results.
    char StartupText[21]; // text to display on startup for 2s.
    char PassText[21]; // text to display on test PASS.
    char FailText[21]; // text to display on test FAIL.
    char DebugMsg[21]; // short description of test.
};

struct functions_to_test Functional_Menu_A[] = { //  for 1132 board test WBG 2017-12-26
    // 0x31 ==> OK to display startup and pass/fail text, and attempt the test = TRUE
/* //Service         param Options    Startup text         Pass text                Fail text             Descript
    {Display_Test,     0, 0x31,   "Start Display test  ", "Display test PASS   ", "Display test Fail   " , "Dspl"},
    {WiFi_Test,        0, 0x31,   "Start CN16 (WiFi)   ", "CN16 (WiFi) tst PASS", "CN16 (WiFi) tst Fail" , "WiFi"},
    {WiFi_Test,        25,0x41,   "Start CN16 (WiFi)   ", "CN16 (WiFi) tst PASS", "CN16 (WiFi) tst Fail" , "WiFi"},    
    {PushButtons_Test, 0, 0x0139, "Start Buttons test  ", "Buttons test PASS   ", "Buttons test Fail   " , "Btns"},
    {The_RS_485_test,  0, 0x31,   "Start RS485 tst RT  ", "RS485 RT tst PASS   ", "RS485 RxTx tst Fail " , "RS485"},
    {The_RS_232_test,  0, 0x31,   "Start RS232 tst     ", "RS232 tst PASS      ", "RS232 tst Fail      " , "RS232"},
    {TEST_SD,          0, 0x31,   "Start SD card test  ", "SD card test PASS   ", "SD card test Fail   " , "SD card"},
    {TEST_RTC,         0, 0x31,   "Start RT Clock test ", "Real Time clk PASS  ", "Real Time clk Fail  " , "RTC"},
    {Test_MRAM,        1, 0x31,   "Start MRAM test cp1 ", "MRAM test PASS      ", "MRAM test Fail      " , "MRAM"},
    {Test_MRAM,        2, 0x31,   "Start MRAM test cp2 ", "MRAM test PASS cp2  ", "MRAM test Fail cp2  " , "MRAM2"},
    {TEST_GRCAN,       0, 0x31,   "Start Group CAN test", "Group CAN test PASS ", "Group CAN test Fail " , "grCAN"},
    {TEST_MRCAN,       0, 0x31,   "Start MR CAN test   ", "MR CAN test PASS    ", "MR CAN test Fail    " , "MRCAN"},
    {TEST_CTCAN,       0, 0x31,   "Start CT CAN test   ", "CT CAN test PASS    ", "CT CAN test Fail    " , "CTCAN" },
    {TEST_SLCAN,       0, 0x31,   "Start Select CAN tst", "SEL CAN test PASS   ", "SEL CAN test Fail   " , "SLCAN"},
    {Read_FPGA_ver_rev,0, 0x31,   "Start CPLD1 FPGA tst", "CPLD1 FPGA test PASS", "CPLD1 FPGA test Fail" , "CPLD 1"},
    {Read_FPGA_ver_rev,1, 0x31,   "Start CPLD2 FPGA tst", "CPLD2 FPGA test PASS", "CPLD2 FPGA test Fail" , "CPLD 2"},
    {Read_FPGA_ver_rev,2, 0x31,   "Start NTS tst       ", "NTS test PASS       ", "NTS test Fail       " , "NTS"},
    {WiFi_Test,        0, 0x31,   "Start CN16 (WiFi)   ", "CN16 (WiFi) tst PASS", "CN16 (WiFi) tst Fail" , "WiFi"},
    {WiFi_Test,        25,0x41,   "Start CN16 (WiFi)   ", "CN16 (WiFi) tst PASS", "CN16 (WiFi) tst Fail" , "WiFi"},
  
	{PushButtons_Test, 0, 0x0119, "Start Buttons test  ", "Buttons test PASS   ", "Buttons test Fail   " , "Btns"}, 
	{Display_Test,     0, 0x11,   "Start Display test  ", "Display test PASS   ", "Display test Fail   " , "Dspl"},           
    {The_RS_232_test,  0, 0x11,   "Start RS232 tst     ", "RS232 tst PASS      ", "RS232 tst Fail      " , "RS232"},
    {WiFi_Test,        0, 0x11,   "Start CN16 (WiFi)   ", "CN16 (WiFi) tst PASS", "CN16 (WiFi) tst Fail" , "WiFi"},  
    {TEST_GRCAN,       0, 0x11,   "Start Group CAN test", "Group CAN test PASS ", "Group CAN test Fail " , "grCAN"},
    {TEST_SLCAN,       0, 0x11,   "Start Select CAN tst", "SEL CAN test PASS   ", "SEL CAN test Fail   " , "SLCAN"},
    {TEST_CTCAN,       0, 0x11,   "Start CT CAN test   ", "CT CAN test PASS    ", "CT CAN test Fail    " , "CTCAN" },
    {TEST_MRCAN,       0, 0x11,   "Start MR CAN test   ", "MR CAN test PASS    ", "MR CAN test Fail    " , "MRCAN"},        
    {The_RS_485_test,  0, 0x11,   "Start RS485 tst RT  ", "RS485 RT tst PASS   ", "RS485 RxTx tst Fail " , "RS485"},  
    {TEST_SD,          0, 0x11,   "Start SD card test  ", "SD card test PASS   ", "SD card test Fail   " , "SD card"},
    {TEST_RTC,         0, 0x11,   "Start RT Clock test ", "Real Time clk PASS  ", "Real Time clk Fail  " , "RTC"},
    {Test_MRAM,        1, 0x11,   "Start MRAM test cp1 ", "MRAM test PASS      ", "MRAM test Fail      " , "MRAM"},
    {Test_MRAM,        2, 0x11,   "Start MRAM test cp2 ", "MRAM test PASS cp2  ", "MRAM test Fail cp2  " , "MRAM2"},    
    {Read_FPGA_ver_rev,0, 0x11,   "Start CPLD1 FPGA tst", "CPLD1 FPGA test PASS", "CPLD1 FPGA test Fail" , "CPLD 1"},
    {Read_FPGA_ver_rev,1, 0x11,   "Start CPLD2 FPGA tst", "CPLD2 FPGA test PASS", "CPLD2 FPGA test Fail" , "CPLD 2"},
    {Read_FPGA_ver_rev,2, 0x11,   "Start NTS tst       ", "NTS test PASS       ", "NTS test Fail       " , "NTS"},    
*/
	//Service                 param Options    Startup text         Pass text                Fail text           Descript
    {Check_24V,                0, 0x031,   "Checking 4 24V      ", "24V is present      ", "No 24V detected     " , "24V ?"},	
    {Check_Car_Light,          0, 0x031,   "Checking Car light  ", "Car light test PASS ", "Car light test Fail " , "Car Light"},
    {Check_Car_Buzzer_or_Bell, 1, 0x031,   "Checking Car bell   ", "Bell      test PASS ", "Bell test Fail      " , "Car Bess"},  
    {Check_Car_Buzzer_or_Bell, 0, 0x031,   "Checking Car buzzer ", "Buzzer    test PASS ", "Buzzer test Fail    " , "Car Buzzer"},
    {Check_Car_Buzzer_or_Bell, 2, 0x031,   "Checking Fire Light ", "Fire Light OK       ", "No Fire Light seen  " , "Fire Light"},
    {Generic_Tester,       0, 0x031,   "Checking Run/Stp SW ", "Run/Stop SW OK      ", "Run/Stop SW Fail    " , "Run/Stop SW"},
    {Generic_Tester,       1, 0x031,   "Check Inspection SW ", "Inspeciont SW OK    ", "Inspection SW Fail  " , "Inspection SW"},
    {Generic_Tester,       2, 0x031,   "Checking Enable SW  ", "Enable SW OK        ", "Enable SW Fail      " , "Enable SW"},
    {Generic_Tester,       3, 0x031,   "Checking Enable + UP", "Enable + UP SW OK   ", "Enable + UP SW Fail " , "Enable + UP SW"},
    {Generic_Tester,       4, 0x031,   "Check Enable + DOWN ", "Enable + DOWN SW OK ", "Enable + DWN SW Fail" , "Enable + DWN SW"},    
    {Perform_all_IO_Tests, 0, 0x031,   "Checking IO's       ", "IO test PASS        ", "IO test Fail        " , "IO test"},  
    {Generic_Tester,       5, 0x031,   "Check GFI, no trip  ", "GFI No trip OK      ", "GFI No trip Fail    " , "GFI should not trip"},
    {Generic_Tester,       6, 0x031,   "Check GFI Trip out  ", "GFI trip out OK     ", "GFI trip out Fail   " , "GFI should trip"},	
   
}; // 

//  WBG step 06
struct functions_to_test Functional_Menu_B[] = {  //  for 1121 board test WBG 2017-12-26
//-    {Display_Test,     0, 0x31,   "Start Display test  ", "Display test PASS   ", "Display test Fail   " , "Dspl"},
//-    {WiFi_Test,        0, 0x31,   "Start CN16 (WiFi)   ", "CN16 (WiFi) tst PASS", "CN16 (WiFi) tst Fail" , "WiFi"},
//-    {WiFi_Test,        25,0x41,   "Start CN16 (WiFi)   ", "CN16 (WiFi) tst PASS", "CN16 (WiFi) tst Fail" , "WiFi"},    
//-    {PushButtons_Test, 0, 0x0139, "Start Buttons test  ", "Buttons test PASS   ", "Buttons test Fail   " , "Btns"},
	{TEST_JIG_COMM,     0,0x11,   "TEST_JIG_COMM       ", "TEST_JIG_COMM_PASS  ", "TEST_JIG_COMM_Fail  " , "TEST_JIG_COMM"},
	
	{TEST_SLCAN,       16,0x11,   "SLCAN_TEST          ", "SLCAN_test_PASS     ", "SLCAN_test_Fail     " , "SLCAN"},
	{TEST_MRCAN,       16,0x11,   "Machine_Room_CAN_R  ", "MRCAN_test_PASS     ", "MRCAN_test_Fail     " , "MRCAN"},
	{TEST_MLCAN,       16,0x11,   "Machine_Room_CAN_L  ", "MLCAN_test PASS     ", "MLCAN_test_Fail     " , "MLCAN"},
	{TEST_NTSCAN,       0,0x11,   "NTS_CAN             ", "NTSCAN_test PASS    ", "NTSCAN_test_Fail    " , "NTSCAN"},
	
	{Read_FPGA_ver_rev, 0,0x11,   "FPGA1_SPI           ", "FPGA1_SPI_test_PASS ", "FPGA1_SPI_test_Fail " , "FPGA1_SPI"},
	{Read_FPGA_ver_rev, 1,0x11,   "FPGA2_SPI           ", "FPGA2_SPI_test_PASS ", "FPGA2_SPI_test_Fail " , "FPGA2_SPI"},
	{Read_FPGA_ver_rev, 2,0x11,   "NTS_SPI             ", "NTS_SPI_test_PASS   ", "NTS_SPI_test_Fail   " , "NTS_SPI"},
	
	{v120AC_Power_Check,0,0x11,   "v120AC_Power_Check  ", "v120AC_PW_test_PASS ", "v120AC_PW_test_Fail " , "v120AC_PW"},
	{SW_TEST,           0,0x11,   "SW_TEST             ", "SW_TEST_test_PASS   ", "SW_TEST_test_Fail   " , "SW_TEST"},
	{SEB_IO_TEST,       0,0x11,   "SEB_IO_TEST         ", "SEB_IO_test_PASS    ", "SEB_IO_test_Fail    " , "SEB_IO"},
	{FPGA1_IO_TEST,     0,0x11,   "FPGA1_IO_TEST       ", "FPGA1_IO_test_PASS  ", "FPGA1_IO_test Fail  " , "FPGA1_IO"},
	{FPGA2_IO_TEST,     0,0x11,   "FPGA2_IO_TEST       ", "FPGA2_IO_test_PASS  ", "FPGA2_IO_test Fail  " , "FPGA2_IO"},
};
struct functions_to_test *Functional_Menu;

int16 Sz_Menu_A = (sizeof(Functional_Menu_A) / sizeof(Functional_Menu_A[0]));
int16 Sz_Menu_B = (sizeof(Functional_Menu_B) / sizeof(Functional_Menu_B[0]));

enum {
    InitFunctionTest,
    ObtainNextTest,
    RunTheTest,
    The_RS_485_test_case,
    The_RS_232_test_case,
    TEST_SD_case,
    PauseCase,
    UserOptionCase,
    DoneWith_FT,
    PerformTheTestCase,
    DoneWithThisTest,
    MonitorButtons,
    Retest_after_delay
};
enum {
    InitPauseCase,
    Wait_on_Event_Case,
    InitTheTestCase
};

int16 FunctionalTest(int16 Dummy);
int16 FTRq_Helper(int16);
int16 (*FunctionalTest_Request)(int16); // function pointer
//taking one int16 param and
// returning an int16


// FunctionalTest() moves thru all the tests.   Error messages
// will allow for the test to be redone.  'Function Test A' will quit
// on first test that fails. 'Function Test B' will continue with
// next test wether the current test passes of fails.

int16 FTRq_Helper(int16 param)  // performs the actual test
{
    // handles request to redo the test, and returns PASS/FAIL/RETEST
    int16 ReturnVal = 0;
    static int16 RetestCase = 0;

    ReturnVal = FunctionalTest_Request(param);
    switch (ReturnVal) {
    case CONTINUE_SERVICE:
        return CONTINUE_SERVICE;
        break;

    case DONE_BUT_CONTINUE_DISPLAY: // CurrentTestPassFail =0;  // fail
        return DONE_BUT_CONTINUE_DISPLAY;
        break;

    case FINISHED_WITH_SERVICE:
        return FINISHED_WITH_SERVICE;
        break;

//ZYM 2-1-2018
//UNUSED CODE, MAYBE FOR FUTURE DEVELOPMENT
//-    case RETEST_OPTION: //  Give operator option to retest
//-        fail_testagain();
//-        return RETEST_OPTION;
//-        break;

    default:
        return DEFAULT;
        break;
    }
    return 0; // FAIL
}



//ZYM 2-2-2018
int16 functiontest_disply_passfail_flag=0;


int16 FunctionalTest(int16 param)
{ // if param & 0x0100 ====> menu B, else menu A
  // if param & 0x00ff ====> stop on first failure
    static int16 FunctionTestCase   = 0;
    static int16 NextCaseAfterPause = 0;
    static int16 PauseSubCase       = 0;
    static int16 responseFromTest   = 0;
    static int16 TheTestCase        = InitTheTestCase;
    static int16 Which_Menu         = 0;

    switch(FunctionTestCase) {
    case InitFunctionTest:
//        setoutp(Virtual_Board_B_OUT[6]); 
//        clroutp (o_4[5]);
//        clroutp (o_4[7]);
        Clear_Log_Buffer();
        Clear_Log_Buffer_C();
        
        //prevent any one using "Message Board C" during "Functional Test" is running
        Functional_Test_is_Running = TRUE;		
        FTMenu_Index = 0;
        StopOnFail = param & 0x00ff;
        Which_Menu = (param & 0x0100) >> 8;
        if(Which_Menu == 1) {
            Functional_Menu = Functional_Menu_B;
            max_FTMenu_Index = Sz_Menu_B;
        } else {
            Functional_Menu = Functional_Menu_A;
            max_FTMenu_Index = Sz_Menu_A;
        }
        clr_lcd_dply(); // clear 4-line display
//        max_FTMenu_Index = (sizeof(Functional_Menu) / sizeof(Functional_Menu[0]));
        reset_Functional_Flags(max_FTMenu_Index); // reset bits 1,2 if applicable
        FunctionTestCase = ObtainNextTest;
        break;

    case ObtainNextTest:
        if (FTMenu_Index < max_FTMenu_Index) {
            if(SetUpNextTest(max_FTMenu_Index) == FALSE)
            {  
                FunctionTestCase = DoneWith_FT;
                Add_msg_to_array_B("A",10);
            }
                
            else {
                Options = Functional_Menu[FTMenu_Index].OptionsMask;
                sprintf(src,"options = %X, %X",
//                  Functional_Menu[FTMenu_Index].OptionsMask,
//                    (Functional_Menu[FTMenu_Index].OptionsMask) & 0x8);
                        Functional_Menu[FTMenu_Index].OptionsMask,
                        (Functional_Menu[FTMenu_Index].OptionsMask) & 0x100L);
//                Add_msg_to_array_B(src,0);
                sprintf(src,"FTMenuIdx=%d ",FTMenu_Index);
//                Add_msg_to_array_B(src,1);
                sprintf(src,"Options=%x %x %x",Options, Options & TESTMASK, Options & testmask);
//                Add_msg_to_array_B(src,2);
                NextCaseAfterPause = RunTheTest;
                TheTestCase = InitTheTestCase;
                PauseSubCase =  InitPauseCase;
                pauseDelayTime = 10;  // 10 = 1s
                FunctionTestCase =  PauseCase ;
            }
        } else 
          {  
            Add_msg_to_array_B("B",11);
            FunctionTestCase = DoneWith_FT;
          }
        break;

    case RunTheTest:
        switch(TheTestCase) {
        case InitTheTestCase:
            clr_lcd_dply();
            CurrentTestPassFail = FAIL; // set 'failed' flag.
            // The test must set this flag = 1 if the test
            // has passed.
            TheTestCase = PerformTheTestCase;
            break;

        case PerformTheTestCase:
            responseFromTest = FTRq_Helper(tst_param);  // performs the requested test or service
//-         if( (Functional_Menu[FTMenu_Index].OptionsMask & 0x8)== 8 ) SIU.GPDO[Z6_LED].R =0; // ON
//-             else SIU.GPDO[Z6_LED].R = 1; // OFF

            if(CheckForLongModeButton(50) &&
                    !(Functional_Menu[FTMenu_Index].OptionsMask & 0x8) ) {
                FunctionTestCase = InitFunctionTest;
                return FINISHED_WITH_SERVICE; // if button held for (50*0.02)=1s
            } // Else

            switch (responseFromTest) {
            case CONTINUE_SERVICE:
                return CONTINUE_SERVICE;
                break;

            case RETEST_OPTION_AFTER_DELAY:
            case DONE_BUT_CONTINUE_DISPLAY:
                TheTestCase = InitTheTestCase;
                PauseSubCase =  InitPauseCase;
                pauseDelayTime = 20;  // 20 = 2s
                NextCaseAfterPause = DoneWithThisTest;
                FunctionTestCase = PauseCase; // pause to allow operator to read display

//ZYM 2-2-2018                
                if(functiontest_disply_passfail_flag)
                {
                	if(CurrentTestPassFail)New_write_display(Functional_Menu[FTMenu_Index].PassText,0,0);
                	else
                	{ 
                    	//setoutp(o_4[7]);
                    	New_write_display(Functional_Menu[FTMenu_Index].FailText,0,0);
//ZYM 2-1-2018                    
//-                    Log_an_event(Functional_Menu[FTMenu_Index].FailText); 
                       Log_an_event_C(Functional_Menu[FTMenu_Index].FailText);   
                	} 
                }
                
                // read display
                return CONTINUE_SERVICE;
                break;
                // Log_an_event(src);
            case FINISHED_WITH_SERVICE:
                sprintf(src,"p/f %d",CurrentTestPassFail);
                Add_msg_to_array_B(src,5);
                
//ZYM 2-2-2018
				FunctionTestCase = PauseCase; // pause to allow operator to read display
                PauseSubCase =  InitPauseCase;
                pauseDelayTime = 20;  // 20 = 2s
                NextCaseAfterPause=DoneWithThisTest;
//-             FunctionTestCase = DoneWithThisTest;
                                
//ZYM 2-2-2018                
                if(functiontest_disply_passfail_flag)
                {                
                	if(CurrentTestPassFail)
                	{
                  	  	Add_msg_to_array_B(Functional_Menu[FTMenu_Index].PassText, 6);
                    	New_write_display(Functional_Menu[FTMenu_Index].PassText,0,0);
                	}
                	else
               	 	{ 
                    	//Log_an_event_C(Functional_Menu[FTMenu_Index].FailText);
                    	//setoutp(o_4[7]);
                   	 	sprintf(src,"%s", Functional_Menu[FTMenu_Index].FailText);
                    	//Log_an_event_C(src);
                    	//Log_an_event_C(Functional_Menu[FTMenu_Index].FailText);                     
                    	Add_msg_to_array_B(Functional_Menu[FTMenu_Index].FailText, 7);
                    	New_write_display(Functional_Menu[FTMenu_Index].FailText,0,0);
//ZYM 2-1-2018                     
//-                     Log_an_event(Functional_Menu[FTMenu_Index].FailText);
                    	Log_an_event_C(Functional_Menu[FTMenu_Index].FailText);                                                               
                	}    
                }
                return CONTINUE_SERVICE;
                break;
                /*
                // RETEST_OPTION_AFTER_DELAY
                            case RETEST_OPTION: //  Give operator option to retest
                                FunctionTestCase = MonitorButtons;
                                fail_testagain();
                                LCD_Upd_Dpy = 1;
                                return CONTINUE_SERVICE;
                                break;

                            case RETEST_OPTION_AFTER_DELAY: //  Give operator option to retest, after a sufficeint
                              //  delay to let him read the display
                                timers[t_Generic_Delay] = 0;
                                FunctionTestCase = Retest_after_delay;
                                return CONTINUE_SERVICE;
                                break;
                */
            default:
                return DEFAULT;
                break;
            }
            break;

        default:
            return DEFAULT;
            break;
        }
        break;
// Functional_Menu[test_index].OptionsMask

    case Retest_after_delay:
        if(timers[t_Generic_Delay] >= 25 ) {
            fail_testagain();
            LCD_Upd_Dpy = 1;
            FunctionTestCase = MonitorButtons;
        }
        break;

    case MonitorButtons:
        if(Check_for_Mode_Button() &&
           !(Functional_Menu[FTMenu_Index].OptionsMask & 0x8) ) FunctionTestCase = DoneWithThisTest;
        else if(Check_for_Enter_Button() ) FunctionTestCase = RunTheTest;
        break;

	//From Individual Test Return for Test Failed(Stop), Pass(Go to Next Test)
    case DoneWithThisTest:
    	//without this line code, test result may not be stored
        SaveTestResult(FTMenu_Index);
        if((StopOnFail == TRUE) && (CurrentTestPassFail == FAIL)) {
            // exit on first failure.
            Add_msg_to_array_B("C",12);
            FunctionTestCase = DoneWith_FT;
        } else {
            FTMenu_Index++;
            FunctionTestCase = ObtainNextTest;
        }
        break;

    case DoneWith_FT:
        FunctionTestCase = InitFunctionTest;
        clr_lcd_dply(); // clear 4-line display
        
//ZYM 2-2-2018        
        // display msg
        //Function Test NONE STOP
        if(StopOnFail==3)
        {
        	New_write_display("Test - Do Not Stop  ",0,0);
        	New_write_display("DONE                ",1,0);        		
        }
        //Function Test STOPS AT ERROR
        else if(StopOnFail==1)
        {
        	New_write_display("Test & Stop On Err  ",0,0);
        	New_write_display("DONE                ",1,0);
        	
        	if(CurrentTestPassFail)
        	{
        		New_write_display("NO FAULT            ",2,0);	
        	}        	
        }
        //Only test functions that have failed, and StopOnFail = FALSE
        else if(StopOnFail==2)
        {
        	New_write_display("Test @ Failed Funct ",0,0);
        	New_write_display("DONE                ",1,0); 	
        }        
        Functional_Test_is_Running = FALSE;
        
//ZYM 2-2-2018
//MUST RESET FOR "if(StopOnFail==3)" on "Line 1318" from "MenuManager.c"
//prevent NO RESET while doing INDIVIDUAL TEST, after FUNCTION TEST(special after "Test - Do Not Stop"
		StopOnFail=0; 
		        
        if(StopOnFail) return DONE_BUT_CONTINUE_DISPLAY; // FINISHED_WITH_SERVICE;
            else return FINISHED_WITH_SERVICE;       
        break;

    case PauseCase:  // leave this case after pauseDelayTime or if the 'mode' button is pressed.
        switch(PauseSubCase) {
        case InitPauseCase:
            timers[t_Functional_Test_Delay] = 0;
            PauseSubCase = Wait_on_Event_Case;
            break;

        case Wait_on_Event_Case:
            if (timers[t_Functional_Test_Delay] > pauseDelayTime || Check_for_Mode_Button() ) {
                PauseSubCase = InitPauseCase;
                FunctionTestCase = NextCaseAfterPause;
            }
            break;

        default:
            return DEFAULT;
            break;
        }
        break;

    case UserOptionCase: // test failed; should we continue
        if(StopOnFail == TRUE) {
            FunctionTestCase = InitFunctionTest;
            return DONE_BUT_CONTINUE_DISPLAY;
        } else {
            PauseSubCase = InitPauseCase;
            pauseDelayTime = 35;
            FunctionTestCase = PauseCase;
        }
        break;

    default:
        FunctionTestCase = InitFunctionTest;
        clr_lcd_dply();
        New_write_display("Functional Tst error",1,0);
        New_write_display("default case taken  ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DONE_BUT_CONTINUE_DISPLAY;
        break;
    }
    return CONTINUE_SERVICE;	// 1
}

void reset_Functional_Flags(int16 max_FTMenu_Index)
{
    // reset bits 1,2 if applicable
    int16 i = 0;

    if(StopOnFail != 2) {
        for(i=0; i< max_FTMenu_Index; i++) {
            Functional_Menu[i].OptionsMask &= NOT_BITS_1_2;
        }
    }
}

int16 SetUpNextTest(int16 max_FTMenu_Index)
{
    int16 attemptTest       = 0;
    int16 optionsMsk        = 0;
    int16 testHasBeenDone   = 0;
    int16 testHasPassed     = 0;
    int16 FoundTheNextTest  = FALSE;

    FoundTheNextTest  = FALSE;
    for(; FTMenu_Index < max_FTMenu_Index; FTMenu_Index++ ) {
        optionsMsk  = Functional_Menu[FTMenu_Index].OptionsMask;
        attemptTest = optionsMsk & ATTEMPT_TO_TEST;
        testHasBeenDone = optionsMsk & TEST_HAS_BEEN_DONE;
        testHasPassed = optionsMsk & TEST_FAILED;
        tst_param = Functional_Menu[FTMenu_Index].param;
        				       
        sprintf(src,"FTM index = %d", FTMenu_Index);
        Add_msg_to_array_B(src,15);
        if(StopOnFail == 2) {
            // only test a function that has failed
            if(attemptTest && testHasBeenDone && !testHasPassed) {
//-             SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
                FunctionalTest_Request = Functional_Menu[FTMenu_Index].Service;
                FoundTheNextTest = TRUE;
            }
        } else {
            // test all functions (as appropriate) even if it has passed.
            if(attemptTest) {
                FunctionalTest_Request = Functional_Menu[FTMenu_Index].Service;
                FoundTheNextTest = TRUE;
            }
        }
        if(FoundTheNextTest == TRUE) {
            if(optionsMsk & SHOWSTARTUPMSG ) {
                clr_lcd_dply(); // clear 4-line display
                New_write_display(Functional_Menu[FTMenu_Index].StartupText,0,0);
            }
            
//ZYM 2-2-2018
			if(optionsMsk & SHOWPASSFAILMSG)
			{
				functiontest_disply_passfail_flag=1;
			}
			else
			{
				functiontest_disply_passfail_flag=0;
			}   
			        
            return FoundTheNextTest;
        }
    }
    return FoundTheNextTest;
}

void SaveTestResult(int16 test_index)
{
    if(Functional_Menu[test_index].OptionsMask & 0x40)
    {
        CurrentTestPassFail = PASS; //  To handle WIFI test Stop (bit 6 or param)
        return;
    }
    if(test_index >= max_FTMenu_Index) return;
    if(CurrentTestPassFail == FAIL) {
        sprintf(src,"%s: Failed",Functional_Menu[test_index].DebugMsg);
        Functional_Menu[test_index].OptionsMask &= NOT_BITS_1_2;
    } else {
        sprintf(src,"%s: PASSED",Functional_Menu[test_index].DebugMsg);
        Functional_Menu[test_index].OptionsMask |= TEST_PASSED;
    }
    Functional_Menu[test_index].OptionsMask |= (TEST_HAS_BEEN_DONE);
    
//ZYM 2-1-2018    
//-    Log_an_event(src);
	Log_an_event_C(src);

    sprintf(src, "OptionsMask=%X",Functional_Menu[test_index].OptionsMask);
//    Log_an_event(src);
}

int16 CheckForLongModeButton(int16 maxCounter)
{
    if(rdLCDio(Lcd_MODE) == 0) timers[t_Generic_Delay_2] =0;
    if(timers[t_Generic_Delay_2] > maxCounter) return 1;
    else return 0;
}



































