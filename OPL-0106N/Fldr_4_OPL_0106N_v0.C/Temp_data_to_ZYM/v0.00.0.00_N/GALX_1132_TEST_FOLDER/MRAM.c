#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



int16 Test_MRAM(int16 theChip);
// int16 Test_MRAM_chip2(int16 LCD_Menu);

enum {
    InitMRAMTest,
    ClearAllMemory,
    WriteToMemoryAddr,
    CheckMemoryAddr,
    CheckMemoryPage,
    MoveToNextPage,
    MRAMTestDone,
    MRAM_quick_Test,
    Loop_Time,
    MRAMTestFailed,
    MRAMTestPassed,
    Retest_option  // Required for Retest Option
};


int16 MRAMFailedCase = 0;
uint16 MRAM_PAGE = 0x0000;
uint16 MRAM_ADDR = 0x0000;



#define TEST_MRAM_Fault_Display_Index	3
char TEST_MRAM_Fault_Display[TEST_MRAM_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC12                ",	
	"IC13                ",
};



#define MAX_MRAM_Counter		10



int16 Test_MRAM(int16 theChip)
{
	static int16 MRAMTestCase = InitMRAMTest;
	uint32 MRAM_Counter=0;
	int16 Delay_Count=test_1s_delay;
    static int16 Page = 0;
    static int16 The_Chip = 0;
    static uint16 WordAddr = 0;
    static uint16 MRAM_present_Response =0;
    int ABSADDR = 0;  // absolute address
    static uint16 ReturnData = 0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option

    switch(MRAMTestCase) {
    case InitMRAMTest:
    	Clear_Log_Buffer();
        clr_lcd_dply();
        The_Chip = theChip;
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option    
        CurrentTestPassFail =0;  // fail
        MRAMFailedCase = 0;
        MRAM_PAGE = 0;
        MRAM_ADDR = 0;
        WordAddr = 0;
        Page = 0;
        MRAM_Counter=0;
        New_write_display("MRAM TEST           ",0,0);
        if(The_Chip == 1)
        {
            New_write_display("MRAM_1 IC12         ",1,0);   
        }
        else if(The_Chip == 2) 
        {
        	New_write_display("MRAM_2 IC13         ",1,0);
        }
//-     sprintf(src,"theChip=%d", theChip);
//-     New_write_display(src,2,0);
		New_write_display("Checking...         ",2,0);
		
        if(ForceTestToIndicateFail) MRAMTestCase = MRAMTestFailed;
        else MRAMTestCase = MRAM_quick_Test; 
        timers[t_Generic_Delay]=0;        
        break;

    case MRAM_quick_Test: 
    	if(timers[t_Generic_Delay]<Delay_Count)
            break;  
    	
/* //-    
     	disable_dog();        
        MRAM_present_Response = test_MRAM_present(The_Chip);
        if(MRAM_present_Response == 0)
        {
        	if(MRAM_Counter>MAX_MRAM_Counter)
        	{
        		MRAMTestCase = MRAMTestPassed; // MRAMTestFailed; MRAMTestPassed
        		MRAM_Counter=0;
        	}
        	else
        	{
        		MRAMTestCase=Loop_Time;
        	}        	
        }           
        else MRAMTestCase = MRAMTestFailed;
        //test_hardware();	// set error and hardware_init_fault flag
        petdog();
        enable_dog();
*/        
         
		petdog();
        MRAM_present_Response = test_MRAM_present(The_Chip);
        if(MRAM_present_Response == 0) 
        {
 	         MRAMTestCase = MRAMTestPassed; // MRAMTestFailed; MRAMTestPassed        	
        }
        else MRAMTestCase = MRAMTestFailed;
        //test_hardware();	// set error and hardware_init_fault flag
        petdog();    
           		   		  
		timers[t_Generic_Delay]=0;
        break;
        
    case Loop_Time:
     	if(timers[t_Generic_Delay]<Delay_Count)
            break;
    	
    	MRAM_Counter++;    	
    	MRAMTestCase=MRAM_quick_Test;    
    	sprintf(&LCD_Display[1][13],"Loop=%2.2i",MRAM_Counter);	  
    	sprintf(&LCD_Display[3][13],"case=%2.2i",MRAMTestCase);	    	      
    	break;
    	
    case ClearAllMemory:
        if(Check_for_Mode_Button()) {
            MRAMTestCase = InitMRAMTest;
            ReturnVal =  FINISHED_WITH_SERVICE;
            break;
        }
        wr_mram (Page,(int16)WordAddr,0x0000);

        if(WordAddr >= (65535 - 101) ) {
            WordAddr = 0;
            Page++;
//-         sprintf(src,"change page to %d", Page );
//-         Add_msgx_First_TimeOnly(src, 0);
            if(Page > 3) {
                MRAMTestCase = MRAMTestDone;
            }
        } else WordAddr += 100;

        sprintf(&LCD_Display[3][5],"%i",Page);
        sprintf(&LCD_Display[3][14],"%i",WordAddr);
        break;

    case MRAMTestFailed:
        CurrentTestPassFail = 0; // fail
        MRAMTestCase = Retest_option; // Required for Retest Option
        GALX1121_Fault_Display_ADDIN_Function();
        if(The_Chip == 1)
        {
            New_write_display("MRAM_1 TST FAIL IC12",2,0); // f_MRAM_fault    
            Log_an_event(TEST_MRAM_Fault_Display[1]);
        }
        else if(The_Chip == 2) 
        {
        	New_write_display("MRAM_2 TST FAIL IC13",2,0);
        	Log_an_event(TEST_MRAM_Fault_Display[2]);
        }
        // MRAM_present_Response
//-     sprintf(src,"MRAM_pres=%d", MRAM_present_Response);
//-     New_write_display(src,3,0);
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option  
        MRAMTestCase = Retest_option; // Required for Retest Option 
        //return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        break;
        
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) MRAMTestCase = MRAMTestDone;
        else if(RetestResponse == 1) MRAMTestCase = InitMRAMTest; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;    
                
    case MRAMTestPassed:
        CurrentTestPassFail = 1; // pass
        if(The_Chip == 1)New_write_display("MRAM_1 TST PASS IC12",2,0);
        else if(The_Chip == 2)New_write_display("MRAM_2 TST PASS IC13",2,0); 
        MRAMTestCase = MRAMTestDone;
        ReturnVal = CONTINUE_SERVICE; // do not erase display
        //return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        break;        
    
    case MRAMTestDone:
       // clr_lcd_dply();
        // New_write_display("MRAM TEST PASS IC12 ",1,0);
        MRAMTestCase = InitMRAMTest;
                // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;

    default:
        New_write_display("MRAM TEST ERROR     ",2,0);
        New_write_display("default case taken  ",3,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)
        MRAMTestCase = InitMRAMTest;
        //return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        ReturnVal = DEFAULT;
        break;
    }
    return ReturnVal; // Required for Retest Option	
}

/*
int16 Test_MRAM_chip2(int16 LCD_Menu)
{
    static int16 Page = 0;
    static uint16 WordAddr = 0;
    int ABSADDR = 0;  // absolute address
    static uint16 ReturnData = 0;

    switch(MRAMTestCase) {
    case InitMRAMTest:
        CurrentTestPassFail =0;  // fail
        MRAMFailedCase = 0;
        MRAM_PAGE = 0;
        MRAM_ADDR = 0;
        WordAddr = 0;
        Page = 0;
        New_write_display("     MRAM TEST IC13 ",1,0);
        if(ForceTestToIndicateFail)
        {
          MRAMTestCase = MRAMTestFailed;
          return CONTINUE_SERVICE;    
        }       
        MRAMTestCase = MRAM_quick_Test;
        break;

    case MRAM_quick_Test:
        petdog();
        if(test_MRAM_present_chip2() ) MRAMTestCase = MRAMTestFailed;
        else MRAMTestCase = MRAMTestDone;
        //test_hardware();	// set error and hardware_init_fault flag
        petdog();
        break;

    case ClearAllMemory:
        if(Check_for_Mode_Button()) {
            MRAMTestCase = InitMRAMTest;
            return FINISHED_WITH_SERVICE;
        }
        wr_mram (Page,(int16)WordAddr,0x0000);

        if(WordAddr >= (65535 - 101) ) {
            WordAddr = 0;
            Page++;
            sprintf(src,"change page to %d", Page );
            Add_msgx_First_TimeOnly(src, 0);
            if(Page > 3) {
                MRAMTestCase = MRAMTestDone;
            }
        } else WordAddr += 100;

        sprintf(&LCD_Display[3][5],"%i",Page);
        sprintf(&LCD_Display[3][14],"%i",WordAddr);
        break;

    case MRAMTestFailed:
        New_write_display("MRAM TEST FAIL IC13 ",1,0); // f_MRAM_fault
        MRAMTestCase = InitMRAMTest;
        return RETEST_OPTION_AFTER_DELAY  ;
        //return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        break;

    case MRAMTestDone:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("MRAM TEST PASS IC13 ",1,0);
        MRAMTestCase = InitMRAMTest;
        return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        break;

    default:
        New_write_display("MRAM TEST ERROR     ",2,0);
        New_write_display("default case taken  ",3,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)
        MRAMTestCase = InitMRAMTest;
        //return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;	// 0
}

*/





























