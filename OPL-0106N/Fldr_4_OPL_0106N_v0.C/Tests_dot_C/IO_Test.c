// IO_Test.c

#include "global.h"
#include "Tests.h"

#define MAX_IO_LIST_ITEM 13
#define MAX_IOFDAR 13

enum
{
    initialize,
    run_test,
    handle_options,
    wait_4_operator,
    test_pass,
    test_done,
    test_failure,
    Retest_option,
    Check_Buttons,
    Read_After_Delay,
    init_msg_wait,
    Await_Enter,
    Wait_A,
    Wait_B,
};

const char *IO_Failure_msgs[] =
{
    "                    ",
    "Fire Light did not  ", // 1
    "turn on.            ",
    "Fire Buzzer did not ",
    "com on - no sound.  ",
    "No car up lantern   ", // 5  // Car Up lantern
    "indicated.          ",
    "No car down lantern ",  // Car Down lantern
    "No car fan          ",  // car fan
    "No car light        ",  // car light
    "No car safety switch", // 10 // Car Safety Switch
    "No in car inspection", // in car inspection
    
    "No Access Oper Input", // Access Operation Input
    "Is light ON?        ", 
    "Plug into GFI       ",
    "Is GFI 'reset' out? ", // 15  
    "Reset GFI button    ",  
    "GFI test            ", 
    "Enter(YES) Mode(NO) ", 
    "Test msg line 0     ",
    "Test msg line 1     ", // 20
    "Test msg line 2     ",
    "Test msg line 3     ",
    "INDEXING ERROR      ",	
    "No In Car Automatic ", // 24
    "No Car Stop Switch  ", // 25
};

const int16 Iofdar[14][4] =  // IO Failure Detail ARray
{
    { 1, 2, 0, 0 },  // [0] [] // IFL (fire light)
    { 3, 4, 0, 0 },  // [1] [] // Fire Buzzer
    { 5, 6, 0, 0 },  // [2] [] // Car Up lantern
    { 7, 6, 0, 0 },  // [3] [] // Car Down lantern
    { 8, 6, 0, 0 },  // [4] [] // car fan
    { 9, 6, 0, 0 },  // [5] [] // car light
    { 10, 6, 0, 0 },  // [6] [] // Car Safety Switch
    { 19, 20, 21, 22 },  // [7] []  
    { 25,  6,  0, 0  },  // [8] [] // CST - Car Stop Switch
    { 19, 20,  0, 0  },  // [9] [] 
    { 24, 6, 21, 22 },  // [10 [] // ICA - In Car Automatic 
    { 11, 6, 0, 0 },  // [11 [] // in car inspection
    { 12, 6, 0, 0 },  // [12 [] // Access Operation Input 
    { 19, 20, 21, 22 },  // [13 [] // ERROR	
};

int16 Basic_IO_set = 0;


uint32 All_inputs = 0;
const uint32 Expected_Pattern[MAX_IO_LIST_ITEM] = 
{ // since the # of inputs is between 17 and 32, use the uint32
	0x400600,
	0x400600,
	0x400600,
	0x400600,
	0x400400,
	0x400200,
	0x400600,
	0x400600,
	0x408600,
	0x400600,
	0x420600,
	0x440600,
	0x480600,    
};

int16 IO_output_Tests_List[MAX_IO_LIST_ITEM] = 
{
   O_IFL_COPo0_enum, // 0 Fire Light
   O_IFB_COPo1_enum, // 1 Fire Buzzer
   O_CUL_COPo2_enum, // 2 // Car Up lantern
   O_CDL_COPo3_enum, // 3 // Car Down lantern
   O_FAN_COPo4_enum, // 4 // car fan
   O_LIG_COPo5_enum, // 5 // car light
   O_CSS_enum,       // 6  [6] // Car Safety Switch
   O_FSS_enum,       // 10 [7] // Fire Fighter Stop Switch
   O_CST_enum,       // 11 [8] // Car Stop Switch
   O_ICC_enum,       // 12 [9] // Inspection in Car Common
   O_ICA_enum,       // 13 [10] // In Car Automatic
   O_ICI_enum,       // 14 [11] // in car inspection
   O_ACC_enum,       // 15 [12] // Access Operation Input //  total of 13 items in this list
};

int16 Perform_all_IO_Tests(int16 IO_Options) // if Option = 0x8000 then this is a SW debug
{ // call for the purpose of building a table of expected IO patterns.  The table is
  // written to msg board B in IO_Tests(). For real test change Option = 0;
  // To test only one IO, set IO_Options = 0xE0zT, were T = the index into the 
  //IO_output_Tests_List[] array.
  // OPTIONS: bit 15 for indicating that this is not a real test (1).
  //          bit 14 for generating info on the LCD (1).
  //          bit 13 to indicate that only one IO test (1) is to be performed
  //             not the entire array of tests (0).
    static int16 idx_to_io_test_list = 0 ;   
    int16 i,j,k = 0;
    int16 Option = IO_Options & 0xF000;
    int16 Single_IO_Option = IO_Options & 0x2000;
    int16 Single_IO = IO_Options & 0x0FF;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 Debug_display_B = 0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
// ClearAllSEB();
// C120_Plus_24_on_off(1); // 120 & 24 VAC on    
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            ReturnVal = Standard_Init(CLEAR_MSG_BOARD_A + CLEAR_MSG_BOARD_B + CLEAR_MSG_BOARD_C, DELAY_3);
            idx_to_io_test_list = 0 ;
            Debug_display_B = 0; 
            Basic_IO_set = 0;// to force the "int16 set_C120V(int16 dummy)" to be run.
            ClearAllSEB();  // Turn off pwr to 1134 bd
            test_Kase = Wait_A;
            break;
            
        case Wait_A:
            if(timers[t_Generic_Delay_4] < 4 * Delay_Count) break; // 
            Read_All_inputs(0);  // Sets variable 'All_inputs' 
            if(All_inputs != 0x400000) 
            {   
                sprintf(src,"AI=0x%X", All_inputs);
                New_write_display(src,0,0); // row, col
                New_write_display("Set all switches OFF",1,0); // row, col
                
                New_write_display("Press 'Enter' to    ",2,0); // row, col
                New_write_display("continue.           ",3,0); // row, col
                test_Kase = Await_Enter;
            }
            break;
            
        case Wait_B:    
            if(timers[t_Generic_Delay_4] < 4 * Delay_Count) break; // Long delay to allow 1134 bd
            // to power up.       
            test_Kase = run_test;                        
            break;
            
        case Await_Enter:
            if(Check_for_Enter_Button() ) {
                C120_Plus_24_on_off(1); // 120 & 24 VAC on 
                // WAIT UNTIL 1134 MCU COMES ON            
               test_Kase = Wait_B;
            }
            if(Check_for_Mode_Button() ) {
                ClearAllSEB();  // Turn off pwr to 1134 bd
//                C120_Plus_24_on_off(0); // 120 & 24 VAC off 
                test_Kase = initialize;  
                return FINISHED_WITH_SERVICE;
            }            
            break;
            
        case run_test:           
            //  ADD CODE
            if(Single_IO_Option) idx_to_io_test_list = Single_IO;
            ////// THE IO TEST BELOW **************************** 
            ReturnVal = IO_Tests(Option + idx_to_io_test_list ); ////// THE IO TEST
              // The above line actually performs the test.  But the test will take many cycles
              // through the menuManager() to complete, thus the break below.
            if(ReturnVal == FINISHED_WITH_SERVICE)
            {
                sprintf(src,"option=0x%X", Option );
                Add_msg_to_array_C(src);
                sprintf(src,"Single=0x%X", idx_to_io_test_list );
                Add_msg_to_array_C(src);
                sprintf(src,"Table=0x%X", IO_output_Tests_List[idx_to_io_test_list] );
                Add_msg_to_array_C(src);
                sprintf(src,"Option+=0x%X", Option + IO_output_Tests_List[idx_to_io_test_list] );
                Add_msg_to_array_C(src);                
                             
//                clroutp((void *)SEB_outputs_array[ IO_output_Tests_List[idx_to_io_test_list] ]); 
// the clroutp is done in "IO_Tests(x)"
//                idx_to_io_test_list = 0;  //// THE SOURCE OF NASTY PROBLEM 
            }
               
            if(ReturnVal == CONTINUE_SERVICE) break;
            
            else if(ReturnVal == MODE_BUTTON)
            {  // allow operator to abort via mode button
                //clroutp((void *)SEB_outputs_array[O_C120T_enum]);// can't do this because it will shut
                // down 1134 and therefore no CAN msgs, so the next two 'clroutp' lines will have
                // no effect.
//                clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
//                clroutp((void *)SEB_outputs_array[O_L15_1_enum]);
//                ClearAllSEB();
//                Basic_IO_set = 0;                
                test_Kase = initialize; 
                C120_Plus_24_on_off(0); // 120 & 24 VAC off  
                return FINISHED_WITH_SERVICE;                   
            }
            else if( CurrentTestPassFail == 0 )
            {  // exit on first fail
                test_Kase = initialize;
                ClearAllSEB();  // Turn off pwr to 1134 bd 
//                C120_Plus_24_on_off(0); // 120 & 24 VAC off  
                ReturnVal =  FINISHED_WITH_SERVICE;    
            }
            else
            {                 
                if( (++idx_to_io_test_list >= MAX_IO_LIST_ITEM) || Single_IO_Option )
                {  //  end of all IO test so turn things off and reset case.
                    //clroutp((void *)SEB_outputs_array[O_C120T_enum]);// can't do this because it will shut
                    // down 1134 and therefore no CAN msgs, so the next two 'clroutp' lines will have
                    // no effect.
//                    clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
//                    clroutp((void *)SEB_outputs_array[O_L15_1_enum]);
//                    ClearAllSEB();
//                    Basic_IO_set = 0;                
                    test_Kase = initialize;
                    ClearAllSEB();  // Turn off pwr to 1134 bd  
//                    C120_Plus_24_on_off(0); // 120 & 24 VAC off 
                    ReturnVal =  FINISHED_WITH_SERVICE;
                }
                else
                { 
                    ReturnVal =  CONTINUE_SERVICE; //  not at end of list so next time in we
                  // will do the next IO since the idx_to_io_test_list has been incremented and tested.   
                } 
            }
            break;

        default:
            break;                
    }
    return ReturnVal;        
}

int16 set_C120V(int16 dummy)
{
    if(Basic_IO_set) return FINISHED_WITH_SERVICE;
    else
    {
      setoutp((void *)SEB_outputs_array[O_C120T_enum]);
      setoutp((void *)SEB_outputs_array[O_C24_enum]);
//      setoutp((void *)SEB_outputs_array[O_L15RL_enum]);
      setoutp((void *)SEB_outputs_array[O_L15_1_enum]);
      Basic_IO_set = 1;          
    }
}
int16 Set_Clear_C120V_24_L15_T(int16 On_Off)
{
    if(On_Off) 
    {
      setoutp((void *)SEB_outputs_array[O_C120T_enum]);
      setoutp((void *)SEB_outputs_array[O_C24_enum]);
      setoutp((void *)SEB_outputs_array[O_L15_1_enum]);        
    }
    else
    {
      clroutp((void *)SEB_outputs_array[O_C120T_enum]);
      clroutp((void *)SEB_outputs_array[O_C24_enum]);
      clroutp((void *)SEB_outputs_array[O_L15_1_enum]);         
    }
    return FINISHED_WITH_SERVICE;
}

int16 IO_Tests(int16 Which_IO_and_Options)
{ 
    int16 wbg_i_dx,wbg_j_dx,wbg_k_dx = 0;
    int16 Which_IO = Which_IO_and_Options & 0x0FFF; // strip off bits 12 - 15
    int16 Option = Which_IO_and_Options & 0xC000; // options in bits 15, 14
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 Debug_display = 0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
//    uint32 The_Expected_Pattern = Expected_Pattern[Which_IO];
    
    SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED;
    // SIU.GPDO[Z0_LED].R can not be used because it fights with the toggle
    // of SIU.GPDO[Z0_LED].R in the parent test "Perform_all_IO_Tests()".
    switch (test_Kase)
    {
        case initialize:
// can not use the standard init because this is a test within a test.            
//            ReturnVal = Standard_Init(CLEAR_MSG_BOARD_A, DELAY_3);
            clr_lcd_dply();
            
//            Which_IO = Which_IO_and_Options & 0x0FFF; // strip off bits 12 - 15
//            Option = Which_IO_and_Options & 0xC000; // options in bits 15, 14
            Read_All_inputs(0);  // Sets variable 'All_inputs'
            sprintf(src,"io=%d, 0x%X", Which_IO, All_inputs); // All_inputs
            New_write_display(src, 0, 0);                                   
            Clear_Msg_Board(); // only clears msg board A
            ReturnVal = CONTINUE_SERVICE;
            CurrentTestPassFail =0;  // fail
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 3; 
            timers[t_Generic_Delay_4] = 0;
            set_C120V(0);  // turn on 120VAC if not already on (Basic_IO_set = 1)
            setoutp((void *)SEB_outputs_array[ IO_output_Tests_List[Which_IO] ]);          
            test_Kase = init_msg_wait;
            break;
            
        case init_msg_wait: // used for debuging to allow msgs to be read on LCD
            if(timers[t_Generic_Delay_4] < 2 *  Delay_Count) break;
            timers[t_Generic_Delay_4] = 0;        
            test_Kase = run_test;
            break;  
                     
        case run_test:           
           if( Check_for_Mode_Button() ) 
           {  // to allow operator to abort IO test
                test_Kase = initialize;
                return MODE_BUTTON;                
           }
            
            if(timers[t_Generic_Delay_4] < 4 * Delay_Count) break; // Long delay to allow 1134 bd
            // to power up.
            timers[t_Generic_Delay_4] = 0;            	              
            Read_All_inputs(0);  // Sets variable 'All_inputs'
            Log_IO_Results_for_Analysis(Which_IO); // puts All_inputs in msg brd B so that a table can be created manually
            //// EVALUATION OF RESULTS  *************************
            if( Evaluate_Results(Which_IO_and_Options) ) test_Kase = handle_options; // wait_4_operator; // test_Kase = handle_options;
               else test_Kase = test_failure;
            // The if(Eval...) will be 1 if the expected pattern matches the 'All_inputs'
            break;
                        
        case handle_options: // only for debug
            if(Option & 0x4000)
            { // display all inputs and which IO // wait_4_operator
                sprintf(src,"io=%X;idx=%d", All_inputs, Which_IO );
                New_write_display(src, 0, 0);
                New_write_display("Which_IO_and_Options", 1, 0);
                New_write_display("shown below", 2, 0);
                sprintf(src,"0x%X", Which_IO_and_Options );
                New_write_display(src, 3, 0);
                test_Kase = wait_4_operator;                
            }
            else test_Kase = test_pass;
            break;

        case wait_4_operator:  // only for debug                 
            if(Check_for_Enter_Button() ) {
                test_Kase = test_pass;
                break;
            }
            else if( Check_for_Mode_Button() ) {
                test_Kase = initialize;
                return MODE_BUTTON;
                
                break;
            } 
            break;
                                                
        case test_pass:
            CurrentTestPassFail = 1; // pass
            test_Kase = test_done;
            break;
            
        case test_done: // turn off the IO being tested.                 
            clroutp((void *)SEB_outputs_array[ IO_output_Tests_List[Which_IO] ]);
            test_Kase = initialize;
            RetestResponse = 0;
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;   
            SIU.GPDO[Z6_LED].R = 1; // led Off           
            break;
                                    
        case test_failure: // Display clues
            Display_Clues(Which_IO_and_Options);
            test_Kase = Retest_option;     
/*              
            ReturnVal = DisplayMsgBrd_A(0); // faults generated in Evaluate_The_Results(Exp,Which_IO)
            if(ReturnVal == CONTINUE_SERVICE) break;
            else
            {
                test_Kase = Retest_option;
                ReturnVal = CONTINUE_SERVICE;                
            }         
*/            
            break;
            
        case Retest_option: // Required for Retest Option            
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) test_Kase = test_done;  // do not retest
            else if(RetestResponse == 1) test_Kase = initialize; // test again
                else ReturnVal = CONTINUE_SERVICE;    
            break;           
               
        default:            
            break;                
    }
    return ReturnVal;       
}

//////////////////////////////////////
// setoutp((void *)SEB_outputs_array[Diag_IO_index]);
// clroutp((void *)SEB_outputs_array[Diag_IO_index]);
// rdoutp((void *)SEB_outputs_array[Diag_IO_index]) 
// 			sprintf(src, "rdinp[%d]= %d ", idx, Val );
//			Log_an_event_C(src);
// rdoutp
// rdinp
void Read_All_inputs(int16 ShowResults)
{
    uint32 temp_1 = 0;
    int16  idx    = 0;

    All_inputs = 0;
    for (idx = 0; idx < i_SWi0_enum; idx++)
    {
        temp_1 = rdinp((void *)SEB_inputs_array[idx]);        
        temp_1 = temp_1 << idx;
        All_inputs |= temp_1;	               
    } 
    if(ShowResults)
    {
        sprintf(src," 0x%X", All_inputs );
        if(ShowResults == 1)Add_msg_to_array_C(src);
        Add_msg_to_array_B(src, ShowResults);      
    }
    return;           
}

//////////////////////////////////////

int16 Evaluate_Results(int16 Which_IO_and_Options)
{ // Eval All_inputs against the expected pattern stored in 'Expected_Pattern[]'
  int16 Eval_idx = 0;
  // Which_IO_and_Options
  int16 Which_IO = Which_IO_and_Options & 0x3FFF; // strip off bits 15, 14
  int16 Option = Which_IO_and_Options & 0xC000; // options in bits 15, 14  
  uint32 cmpar_Actual, cmpar_expected, Mask = 0;
  uint32 The_Expected_Pattern = Expected_Pattern[Which_IO];
  
  return Evaluate_The_Results(The_Expected_Pattern, Which_IO_and_Options);       
}

//////////////////////////////////////

int16 Evaluate_The_Results(uint32 The_Expected_Pattern, int16 Which_IO_and_Options )
{
  int16 Eval_idx = 0;
//  int16 RetrnVal = PASS;
    // Which_IO_and_Options
  int16 Which_IO = Which_IO_and_Options & 0x3FFF; // strip off bits 15, 14
  int16 Option = Which_IO_and_Options & 0xC000; // options in bits 15,14 
  uint32 cmpar_Actual, cmpar_expected, Mask = 0;
 
  
// TODO wbg i_SWi0_enum
     if(Option || ( The_Expected_Pattern == 0xffffffff ) ) return PASS; 
       // SW debug in order to create the table for expected patterns
     if(All_inputs == The_Expected_Pattern ) return PASS; // FAIL;
     else      
     {// discern the problem. Could be a short or an open
     	for(Eval_idx = 0; Eval_idx < i_SWi0_enum; Eval_idx++ )
     	{  // see which bits are different
     	    Mask = 1 << Eval_idx;
     	    cmpar_Actual = All_inputs & Mask;
     	    cmpar_expected = The_Expected_Pattern & Mask;
     	    if(cmpar_Actual != cmpar_expected)
     	    { // There is a difference, so determine if it is a short or an open.
     	        if(cmpar_Actual > cmpar_expected)
     	        {  // There is a short.
     	            sprintf(src,"%s s %s", IO_Labels[Eval_idx], 
     	                IO_Labels[Which_IO + O_IFL_COPo0_lable_enum]);
                    Log_an_event(src);     	        	
     	        }
     	        else
     	        { // There is an open.
     	             sprintf(src,"%s O %s", IO_Labels[Eval_idx], 
     	                IO_Labels[Which_IO + O_IFL_COPo0_lable_enum]);
                     Log_an_event(src);                         	        	
     	        }
                 sprintf(src," %x / %X M=0x%x",  cmpar_Actual, cmpar_expected, Mask );
                 Add_msg_to_array_B(src, Eval_idx);      	        
     	    }     		
     	}
     }
     
     return FAIL;	
}

int16 Log_IO_Results_for_Analysis(int16 Which_IO)
{ // first use this procedure to store the results so that a
  // table can be fabricated and displayed. Save data in msg board B
 
     sprintf(src," 0x%x",  All_inputs);
     Add_msg_to_array_B(src, Which_IO);
     
     return PASS;        
}


void Display_Clues(int16 Test_and_Options)
{
    int16 wbg_index = 0;
    int16 TheseOptions = Test_and_Options &0xf000;
    int16 TheTest = Test_and_Options &0xfff;
    int16 TheFailureMsgIndex = 0;
    
    clr_lcd_dply(); // clear display
    if(TheTest > MAX_IOFDAR) TheTest = MAX_IOFDAR;
    for (wbg_index = 0; wbg_index < 4; wbg_index++)
    {  
        TheFailureMsgIndex = Iofdar[TheTest][wbg_index];             
        New_write_display( (char *)IO_Failure_msgs[TheFailureMsgIndex], wbg_index, 0 );
    }	
}


