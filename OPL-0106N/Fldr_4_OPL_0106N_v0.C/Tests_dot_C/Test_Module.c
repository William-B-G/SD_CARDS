// Test_Module.c

#include "global.h"
#include "Tests.h"

#define MAX_DEBUG 10


int16 Delay_Count = 3; 
int16 Retest_Option_Parm = 0; // Required for Retest Option
int16 RetestResponse = 1; // Required for Retest Option
int16 (*Generic_Service_Request)( void *A_Ptr, int16[], int16, int16); // function pointer
    // taking a ptr, an array, and two numerics
extern const uint32 Expected_Pattern[];    

// Debug facility
const void *Test_Osignal_array[MAX_TEST_MODULE_SIGNAL_INDEX]= {
    o_CTo0,
    o_CTo1,
    o_CTo2,
    o_CTo3,
    o_CTo4,
    o_CTo5,
    o_CTo6,
    o_CTo7,
    o_CTo8,
    o_CTo9,
    o_CTo10,
    o_CTo11
};

const void *Test_isignal_array[MAX_TEST_MODULE_SIGNAL_INDEX]= {
    i_CTi0,
    i_CTi1,
    i_CTi2,
    i_CTi3,
    i_CTi4,
    i_CTi5,
    i_CTi6,
    i_CTi7,
    i_CTi8,
    i_CTi9,
    i_CTi10,
    i_CTi11,   
};

// wbg 2018-6-27
const void *SEB_debug_out_array[10] = {
    O_dbg01, 
    O_dbg02,
    O_dbg03,
    O_dbg04,
    O_dbg05,
    O_dbg06,
    O_dbg07,
    O_dbg08,
    O_dbg09,
    O_dbg10,             	
};

const char *Failure_msgs[] = 
{
    "                    ",
    "Initial FAIL        ", // 1
    "Run/Stop Switch not ", // Run / Stop SW { 1, 2, 3, 4 }
    "pushed in, or not   ",
    "wired correctly.    ",
    
    "Inspec SW not off or", // 5 // Inspection SW { 5, 6, 7, 0 }
    "not wired correctly.",
    "Or run/stop not out.",
    
    "Enable SW not wired ", // Enable { 8, 9, 10, 0 }
    "correctly, or other ",
    "SW's not in position", // 10
    
    "UP SW not wired corr", // Enable + UP { 11, 12, 13, 0 }    
    "or enable, insp, or ",
    "Run/Stop has problem", 
    
    "DWN SW not wired cor", // Enable + Down { 14, 12, 13, 0 } 
    
    "GFI not plugged in  ", // 15  // GFI should not trip { 15, 16, 17, 0 }  
    "or socket not reset,",  
    "or bad GFI socket.  ", 
    						// GFI should trip out { 15, 16, 17, 0 } 
    "pulled in, or not   ", // Run / Stop SW { 2, 18, 4, 0 }
    "BLANK MESSAGE       ", 
    "Inspec SW not ON or ", // 20  // Inspection SW { 20, 6, 7, 0 }
    "finl FAILURE        ", // 21
    "Check Fire Buzzer   ", // 22
    "Check Bell wiring   ", // 23
    "Check Fire Light    ", // 24
    "wiring              ", // 25
};

const char *Operator_LCD_msgs[] = 
{
    "                    ",
    "Inspec tst underway.", // 1
    "Mode(NO) Enter (YES)",
    "ENTER to Continue   ", // 3
    "MODE to Abort       ",
    "Pull Run/Stop SW out", // 5
    "All switches off    ",
    "Turn Inspec SW on   ",
    "Turn on Light SW    ",
    "Press 'Enable' btn  ",
    "Press Enable + UP   ", // 10
    "Press Enable + DOWN ",
    
    "Release buttons     ",
    "Is light ON?        ", 
    "Plug into GFI       ",
    "Is GFI 'reset' out? ", // 15  
    "Reset GFI button    ",  
    "GFI test            ", 
    "Enter(YES) Mode(NO) ", // 18
    "Release after test. ", 
    "Tst Run/Stop switch ", // 20
    "Tst Enable Btn      ",  // 21
    "Tst Enble + Up Btns ",    
    "Tst Enble + Dwn Btns",  // 23 
    "Light & Insp SW OFF.",  // 24 
    "RUN & Insp SWs ON.  ",  // 25 
    "RUN Switch ON.      ",  // 26  
    "Start with RUN SW ON",  // 27
    "and Insp SW OFF.    ",  // 28 
    "Start wth all SW OFF",  // 29
    "Reset GFI if Reqd   ",   // 30
    "Is GFI tripped?.    ",  // 31 
    "GFI Test            ",  // 32  
    "GFI No Trip Test    ",  // 33
    "GFI Reset Trip Test ",  // 34
    "Do you hear bell?   ",  // 35  
    "Do you hear buzzer? ",  // 36
};

const struct Generic_test_params
{
    int16  Initial_output_Item[4]; // Before starting test, 
           // set these items to the
    int16  Initial_output_State[4]; // states indicated 
           // in the Initial_output_State[] and then wait 
             // so that they have time to take effect.
    uint32 expected_final_result;
    int16  LCD_msgs[4]; // initial msgs to display
    int16  LCD_msgs_while_waiting_for_event[4];
    int16  Gen_outputs_to_make[5];  // last item [4] 
    // { -1 = turn off items 0, 1, 2, 3 at end of test; -2 = leave them on.}
    uint32 expected_initial_result; // after Gen_outputs_to_make 
           // and after a wait time. 
    int16  Specific_outputs_to_make[5]; 
      // last item [4] { -1 = turn off items 0, 1, 2, 3 at 
      // end of test; -2 = leave them on.}
    char   Wait_4_operator_input; // (0 or 1): 'Enter' btn, 'Mode' 
           // button to abort.
    char   Wait_4_specific_input; 
    // (0, 1, or 2): Read IO, (if 1: in addition to 'Enter' btn) 
    // to cause logic to proceed. If 2 read all inputs to see
    // if final conditions are already met so we do not have to
    // wait on operator input to continue. If 0, just procedd, do
    // not wait for anything.
    char   Request_confirmation_by_oprtr; // (0 or 1): ask the 
    // operator if he/she (hesh) confirms expected result,
    int16  Input_2_wait_on;  // which input to wait on 
    int16  Which_IO;  // for SW debug only
    int16  Prelim_Failure_msg[4];
    int16  Final_Failure_msg[4];
    void   *Service_1;  // function pointer (Future capability)
              // takes 4 params:
    void      *A_Ptr_1;     // function pointer
    int16     *Array_for_1; // Ptr to an array for service_1
    int16     int_Param_1A; // int16 param A for service_1 
    int16     int_Param_1B; // int16 param B for service_1
    void   *Service_2;  // function pointer (Future capability)
              // takes 4 params:    
    void      *A_Ptr_2;  // function pointer
    int16     *Array_for_2; // Ptr to an array for service_2
    int16     int_Param_2A; // int16 param A for service_2 
    int16     int_Param_2B; // int16 param B for service_2    
       
}GenericTests[MAX_GENERIC_TEST] = 
{  // 0   pull run/stop SW out
   { {O_C120T_enum, O_C24_enum, -1, -1},  // Before starting test, set these items to the
   {1, 1, -1, -1},                      // states indicated in the Initial_output_State[]
   0x412004,         // expected_final_result
   {29, 24, 5, 3},       // LCD_msgs[4]   
   {0, 5, 3, 0},       // LCD_msgs_while_waiting_for_event[4]
   { O_CSS_enum, O_FAN_COPo4_enum, O_LIG_COPo5_enum, -1, -2}, // Gen_outputs_to_make[5]
   0x400000,           // expected_initial_result
   {-1, -1, -1, -1, -1}, // Specific_outputs_to_make[5]          
   1, 2, 0, i_CSS_COPi1_enum, i_CSS_COPi1_lable_enum,  // Wait_4_operator_input, Wait_4_specific_input, 
       // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {1, 2,  3,  4},    // Prelim_Failure_msg[4] 
   {2, 18, 4, 0},    // Final_Failure_msg[4]
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function
   // pull run/stop SW out
       
   // 1   turn on Inspection SW
   { {O_C120T_enum, O_C24_enum, -1, -1},  // Before starting test, set these items to the
   {1, 1, -1, -1},                      // states indicated in the Initial_output_State[]   
   0x402024,    // expected_final_result 0x412004
   {27, 28, 7, 3},    // LCD_msgs[4]  
   {27, 28, 7, 3},// LCD_msgs_while_waiting_for_event[4] 
   {O_CSS_enum, O_FAN_COPo4_enum, O_LIG_COPo5_enum, -1, -2}, // Gen_outputs_to_make[5]
   0x412004,     // expected_initial_result
   {-1, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   1, 2, 0, i_CSS_COPi1_enum, i_CSS_COPi1_lable_enum, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {5, 6,  7,  0},    // Prelim_Failure_msg[4] 
   {20, 6, 7, 0},    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function 
   // turn on Inspection SW
       
   // 2   press 'Enable' SW
   { {O_C120T_enum, O_C24_enum, -1, -1},  // Before starting test, set these items to the
   {1, 1, -1, -1},                      // states indicated in the Initial_output_State[]   
   0x402064,    // expected_final_result 0x412004
   {25, 9, 19, 3},    // LCD_msgs[4]  
   {25, 9, 19, 3},// LCD_msgs_while_waiting_for_event[4] 
   {O_CSS_enum, O_FAN_COPo4_enum, O_LIG_COPo5_enum, -1, -2}, // Gen_outputs_to_make[5]
   0x402024,     // expected_initial_result
   {-1, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   1, 2, 0, i_CSS_COPi1_enum, i_CSS_COPi1_lable_enum, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {8, 9, 10, 0},    // Prelim_Failure_msg[4] 
   {8, 9, 10, 0},    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function  
       // press 'Enable' SW
   
   // 3    press 'Enable' + UP SW  
   { {O_C120T_enum, O_C24_enum, -1, -1},  // Before starting test, set these items to the
   {1, 1, -1, -1},                      // states indicated in the Initial_output_State[]   
   0x4020e4,    // expected_final_result 0x412004
   {25, 10, 19, 3},    // LCD_msgs[4]  
   {25, 10, 19, 3},// LCD_msgs_while_waiting_for_event[4] 
   {O_CSS_enum, O_FAN_COPo4_enum, O_LIG_COPo5_enum, -1, -2}, // Gen_outputs_to_make[5]
   0x402024,     // expected_initial_result
   {-1, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   1, 2, 0, i_CSS_COPi1_enum, i_CSS_COPi1_lable_enum, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {11, 12, 13, 0},    // Prelim_Failure_msg[4] 
   {11, 12, 13, 0},    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function 
   // press 'Enable' + UP SW
   
   // 4  press 'Enable' + DOWN SW  
   { {O_C120T_enum, O_C24_enum, -1, -1},  // Before starting test, set these items to the
   {1, 1, -1, -1},                      // states indicated in the Initial_output_State[]   
   0x402164,    // expected_final_result 0x412004
   {25, 11, 19, 3},    // LCD_msgs[4]  
   {25, 11, 19, 3},// LCD_msgs_while_waiting_for_event[4] 
   {O_CSS_enum, O_FAN_COPo4_enum, O_LIG_COPo5_enum, -1, -2}, // Gen_outputs_to_make[5]
   0x402024,     // expected_initial_result
   {-1, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   1, 2, 0, i_CSS_COPi1_enum, i_CSS_COPi1_lable_enum, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   { 14, 12, 13, 0 },    // Prelim_Failure_msg[4] 
   { 14, 12, 13, 0 },    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function 
   // press 'Enable' + DOWN SW
      
/////////////////////////////////      
      
   // 5   Check GFI Reset does not trip  
   { {O_C120T_enum, O_C24_enum, O_L15RL_enum, O_L15_1_enum},  // Before starting test, set these items to the
   {1, 1, 1, 0},                      // states indicated in the Initial_output_State[] 
       // GFI relay on, L15_T OFF  
   0x0500600,    // expected_final_result 0x412004
   {33, 0, 30, 3},    // LCD_msgs[4]  
   {33, 0, 30, 3}, // LCD_msgs_while_waiting_for_event[4] 
   {O_L15_1_enum, -1, -1, -1, -1}, // Gen_outputs_to_make[5]; L15_T ON
   0x0500600,     // expected_initial_result // see L15 input
   {O_L15RL_enum, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   1, 0, 0, -1, -1, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {15, 16, 17, 0},    // Prelim_Failure_msg[4] 
   {21,15, 16, 17},    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function  
   // Check GFI Reset does not trip
   
///////////////////////////////////   
   // 6   Check GFI Reset goes on (trips) 
   { {O_C120T_enum, O_C24_enum, O_L15RL_enum, O_L15_1_enum},  // Before starting test, set these items to the
   {1, 1, 0, 0},   // states indicated in the Initial_output_State[] 
       // GFI relay off, L15_T OFF   
   0x0500600,    // expected_final_result 0x412004
   {34, 29, 30, 3},    // LCD_msgs[4]  
   {34, 0, 31, 2 },// LCD_msgs_while_waiting_for_event[4] 
   {-1, -1, -1, -1, -1}, // Gen_outputs_to_make[5]; L15_T ON
   0xffffffff,     // expected_initial_result
   {O_L15_1_enum, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   1, 0, 1, -1, -1, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {15, 16, 17, 0},    // Prelim_Failure_msg[4] 
   {21, 15, 16, 17},    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function  
   // Check GFI Reset goes on (trips)  
   
///////////////////////////////////   
   // 7   Check Fire Buzzer (0)
   { {O_C120T_enum, -1, -1, -1},  // Before starting test, set these items to the
   {1, 0, 0, 0},   // states indicated in the Initial_output_State[] 
       //   
   0xffffffff,    // expected_final_result 0x412004
   {0, 0, 0, 0},    // LCD_msgs[4]  
   {36, 2, 0, 0 },// LCD_msgs_while_waiting_for_event[4] 
   {O_C24_enum, -1, -1, -1, -1}, // Gen_outputs_to_make[5]; L15_T ON
   0xffffffff,     // expected_initial_result
   {O_IFB_COPo1_enum, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   0, 0, 1, -1, -1, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {0, 0, 0, 0},    // Prelim_Failure_msg[4] 
   {22, 25, 0, 0},    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function  
   // Check Fire Buzzer    
      
///////////////////////////////////   
   // 8   Check Bell (1)
   { {O_C120T_enum, -1, -1, -1},  // Before starting test, set these items to the
   {1, 0, 0, 0},   // states indicated in the Initial_output_State[] 
       //   
   0xffffffff,    // expected_final_result 0x412004
   {0, 0, 0, 0},    // LCD_msgs[4]  
   {35, 2, 0, 0 },// LCD_msgs_while_waiting_for_event[4] 
   {O_BELL_enum, -1, -1, -1, -1}, // Gen_outputs_to_make[5]; L15_T ON
   0xffffffff,     // expected_initial_result
   {O_BELL_enum, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   0, 0, 1, -1, -1, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {0, 0, 0, 0},    // Prelim_Failure_msg[4] 
   {23, 0, 0, 0},    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function  
   // Check Bell    
   
      
///////////////////////////////////   
   // 9   Check Fire Light (2)
   { {O_C120T_enum, -1, -1, -1},  // Before starting test, set these items to the
   {1, 0, 0, 0},   // states indicated in the Initial_output_State[] 
       //   
   0xffffffff,    // expected_final_result 0x412004
   {0, 0, 0, 0},    // LCD_msgs[4]  
   {13, 2, 0, 0 },// LCD_msgs_while_waiting_for_event[4] 
   {O_C24_enum, -1, -1, -1, -1}, // Gen_outputs_to_make[5]; L15_T ON
   0xffffffff,     // expected_initial_result
   {O_IFL_COPo0_enum, -1, -1, -1, -1}, // Specific_outputs_to_make[5]           
   0, 0, 1, -1, -1, // Wait_4_operator_input, Wait_4_specific_input,
        // Request_confirmation_by_oprtr, // which input to wait on, Which_IO;  // for SW debug only
   {0, 0, 0, 0},    // Prelim_Failure_msg[4] 
   {23, 0, 0, 0},    // Final_Failure_msg[4] 
   NULL, NULL, 0, 0,    // function ptr_1 and three parameters for the function
   NULL, NULL, 0, 0  }, // function ptr_2 and three parameters for the function  
   // Check Fire Light       
};

// GenericTests[MAX_GENERIC_TEST].expected_final_result ;
// GenericTests[MAX_GENERIC_TEST].Wait_4_operator_input;
// GenericTests[MAX_GENERIC_TEST].Wait_4_specific_input;
// GenericTests[Test].Request_confirmation_by_oprtr;
// GenericTests[MAX_GENERIC_TEST].Gen_outputs_to_make[5];
// GenericTests[MAX_GENERIC_TEST].Specific_outputs_to_make[5];

enum
{
	initialize,
	check_initial_IO,
	Wait_on_event,
	require_confirmation,
	run_test,
	View_Log,
	test_pass,
	test_done,
	test_failure,
	Retest_option,
	Set_Initial_IO,
};

enum
{
    dbg01_enum,
    dbg02_enum,
    dbg03_enum,
    dbg04_enum,
    dbg05_enum,
    dbg06_enum,
    dbg07_enum,
    dbg08_enum,
    dbg09_enum,
    dbg10_enum,
};

///////////////////////////////////////////////////

int16 Set_or_Clear_All_CTxx(int16 Set_1_Clear_0)
{
	int16 idx = 0;
	
	for(idx = 0; idx < 12; idx++)
	{
		if(Set_1_Clear_0) setoutp( (void *)Test_Osignal_array[idx] ); // Debug facility
		  else clroutp( (void *)Test_Osignal_array[idx] );
	}
	return FINISHED_WITH_SERVICE;
}

///////////////////////////////////////////////////

int16 Set_or_Clear_All_dbgxx(int16 Set_1_Clear_0)
{
	int16 idx = 0;
	
	for(idx = 0; idx < MAX_DEBUG; idx++)
	{
		if(Set_1_Clear_0) setoutp( (void *)SEB_debug_out_array[idx] );
		  else clroutp( (void *)SEB_debug_out_array[idx] );
	}
	return FINISHED_WITH_SERVICE;
}

///////////////////////////////////////////////////

int16 Read_and_Display_rdoutp(int16 dummy)
{
	static int16 First_Pass = 0;
	uint8 Val = 0;
	uint8 idx = 0;
	int16 Rtrnval = 0;
		
	if(First_Pass == 0)
	{
	    Clear_Msg_Board_C();
		for(idx = 0; idx < 12; idx++)
		{
			Val = rdoutp( (void *)Test_Osignal_array[idx] ); // Debug facility
			sprintf(src, "rdoutp[%d]= %d ", idx, Val );
			Log_an_event_C(src);
		}
        First_Pass = 1;	
	}
	Rtrnval = Display_Message_Board_C(0);
	if(Rtrnval == FINISHED_WITH_SERVICE)
	{
	    First_Pass = 0;	
	}
	return Rtrnval;	
}

///////////////////////////////////////////////////

// SEB_0047N_inputs_array
int16 Read_and_Display_Rd_Inputs(int16 dummy)
{
	static int16 First_Pass = 0;
	uint8 Val = 0;
	uint8 idx = 0;
	int16 Rtrnval = 0;
		
	if(First_Pass == 0)
	{
	    Clear_Msg_Board_C();
		for(idx = 0; idx < 4; idx++)
		{
			Val = rdinp( (void *)SEB_0047N_inputs_array[idx] );
			sprintf(src, "rdinp[%d]= %d ", idx, Val );
			Log_an_event_C(src);
		}
        First_Pass = 1;	
	}
	Rtrnval = Display_Message_Board_C(0);
	if(Rtrnval == FINISHED_WITH_SERVICE)
	{
	    First_Pass = 0;	
	}
	return Rtrnval;	
}

///////////////////////////////////////////////////


int16 Read_and_Display_All_Inputs(int16 dummy)
{
	static int16 First_Pass = 0;
	uint8 Val = 0;
	uint8 idx = 0;
	int16 Rtrnval = 0;
		
	if(First_Pass == 0)
	{
	    Clear_Msg_Board_C();
        Read_All_inputs(1);
        First_Pass = 1;	
	}
	Rtrnval = Display_Message_Board_C(0);
	if(Rtrnval == FINISHED_WITH_SERVICE)
	{
	    First_Pass = 0;	
	}
	return Rtrnval;	
}
///////////////////////////////////////////////////

int16 Toggle_O_CTxx(int16 channel)
{ // toggle_O_CTxx_state

    if (channel < 0 || channel > 11)
        return FINISHED_WITH_SERVICE; // error
          
    if ( rdoutp((void *)Test_Osignal_array[channel]) == 0 ) // Debug facility
        setoutp( (void *)Test_Osignal_array[channel] ); //  LED from 'Dbg0' - 'Dbg11'
    else
        clroutp( (void *)Test_Osignal_array[channel] );
    
    return FINISHED_WITH_SERVICE;
}

///////////////////////////////////////////////////

int16 Perform_Switch_tests(int16 Options)
{ // Options: bit 15 = 1, indicates SW debug and not a real test
    static int16 First_Pass = 0;
    static int16 Tst_index = 0;
           int16 ReturnVal = 0;
    
    if(First_Pass == 0)
    {
        Clear_All_Boards(3); // clear the message boards
        First_Pass = 1;
        Tst_index = 0;
    }
    ReturnVal = Generic_Tester(Tst_index + Options); // performs the test
    if( ReturnVal == FINISHED_WITH_SERVICE )
    {
    	if( ++Tst_index >= MAX_GENERIC_TEST )
    	{  // Done
    	    First_Pass = 0;
    	    ClearAllSEB();    		
    	}
    	else ReturnVal = CONTINUE_SERVICE;         
    }
    else if( ReturnVal == MODE_BUTTON )
    { // operator initiated abort
    	    First_Pass = 0;
    	    ClearAllSEB();        
    }
    return ReturnVal;  
}

///////////////////////////////////////////////////

/*******************************************************************************

  Function:
   int16 Generic_Tester(int16 Test_with_Options)

  Summary:
    Performs one test as defined by data in array of structure, 
    GenericTests[MAX_GENERIC_TEST]. This 'one' test consists of a preliminary test
    and a final test.
     
  Author:
    William Grauer
    
  Description:
    Many of the tests for the UUT take a similar form: display an opnening message:
    set some preliminary stimuli (SEB outputs); insure that the expected response
    is seen; ask the operator to say Yes or No to continue; on continue, to 
    send some more outputs and examine the response; for some tests, to ask the
    operator if he/she (hesh) saw the expected result; and to see if the final
    result is what was expected. This function performs these operations by using
    a table to define and control the above operations.
    
    If the preliminary or final test Fails, the failure msgs will be sent to
    Msg Board A, and at the end of the test, this msg board will be displayed. If 
    a fail, the msg board can be exited with the 'Enter' button to try a 'Retest' 
    or the Mode button to exit test and return to Menu Mgr or Function
    Test Mgr.

  Precondition:
    None.

  Parameters:
    The index into the array GenericTests[MAX_GENERIC_TEST] and certain Options.
      Index into array: Bits 13 thru 0.
      Options: Bits 15 and 14.  Bit 15, if set, will display Msg Board A after test
        and will always show a PASS test result.  Bit 14, if set, will not wait for a
        specific SEB input.
          
  Hardware Requirements:
    Run on the GALx-1132AN MCU board with 4-line, 20 segment display.

  Returns:
    integer to be used by the Menu Manager to continue or terminate further calls
    to the Generic_Tester() function. The return enum, 'CONTINUE_SERVICE' is 
    the most common return until the test is completed, upon which the enum
    'FINISHED_WITH_SERVICE' is returned.

  Remarks:

 */
/******************************************************************************/

int16 Generic_Tester(int16 Test_with_Options) //// "A"
{ //  Tests for switchs; RUN/STOP; ENABLE; UP, DOWN; INSPECTION (but not LIGHT)
  //  Also, tests the GFI socket.
  
    int16 i,j,k = 0;
    static int16 test_Kase = initialize;
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    static int16 NewMsgsDisplayed = 0;
           int16 OK_2_continue_Q = 0; // -1 = MODE button; 0 continue to wait; 1 found event.
           int16 Test = Test_with_Options & 0x3fff; // discard bits 15 and 17
           int16 SW_Options = Test_with_Options & 0xC000; // set options to bits 15 and 14
    
    if(Test > MAX_TEST_DEFINED) return FINISHED_WITH_SERVICE;

    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (test_Kase)
    {
        case initialize:
            Standard_Init(CLEAR_ALL_MSG_BOARDS, DELAY_3);
            ReturnVal = CONTINUE_SERVICE;						
			Set_or_Clear_All_CTxx(0); // turn off all test LEDs on 0047 or SEB used as a debug tool
			Set_or_Clear_All_dbgxx(0); // turn off all 'dbgxx' leds on SEB 6, (addr= 6)
			setoutp( (void *)Test_Osignal_array[0] ); // Set first debug LED ON, on the GALX4-0047
//			setoutp( (void *)SEB_debug_out_array[dbg01_enum] ); // Set first debug LED ON, on SEB 6, (addr= 6)
            Setup_Initial_SEB_States(Test); //// "B"  // Before starting test, 
				 // set these items to the
				 // states indicated. Then wait.            
            NewMsgsDisplayed = 0;
            Display_LCD_msgs(Test); //// "C" //  start up display from Operator_LCD_msgs[]
            test_Kase = Set_Initial_IO;
            break;
            
        case Set_Initial_IO: // for a time delay
            if(timers[t_Generic_Delay] < Delay_Count) break;
            timers[t_Generic_Delay] = 0;
            Set_Initial_Conditions(Test); //// "D" // set general output (up to 4) 
                // from GenericTests[Test].Gen_outputs_to_make[idx]. Then wait
            test_Kase = check_initial_IO;                    
            break;
       
        case check_initial_IO:
            if(timers[t_Generic_Delay] < Delay_Count) break;  
            timers[t_Generic_Delay] = 0; 
            Read_All_inputs(0); // All_inputs
            sprintf(src,"inital:0x%X",All_inputs);
            Log_an_event_C(src);           
// CHECK INITIAL
            if( Check_Initial_Conditions(Test_with_Options) ) //// "E"
            { // 1 = pass. Expected are contained in GenericTests[Test].expected_initial_result
//                Display_LCD_msgs(Test);
//                setoutp( (void *)SEB_debug_out_array[dbg02_enum] ); // debug LED
                test_Kase = Wait_on_event;  
            }
            else
            { 
                Send_Failure_msgs_to_msg_brd_A(0, Test_with_Options); // preliminary error msgs
                test_Kase = test_failure;	
            }
            break;
                       
        case Wait_on_event: //// "F" 3)
        //  or maybe just time delay depending on flags in struct GenericTests[Test]
            if(timers[t_Generic_Delay] < (1 * Delay_Count)  ) break;
            setoutp( (void *)SEB_debug_out_array[dbg01_enum] ); // debug LED
            OK_2_continue_Q = check_4_event(Test_with_Options);
            if( OK_2_continue_Q == 1 ) // event found = 1
            {
                Log_an_event_C("into Wait on event  ");
                timers[t_Generic_Delay] = 0;
                Set_Specific_outputs(Test); //// "G"
//                setoutp( (void *)SEB_debug_out_array[dbg07_enum] );
                test_Kase = require_confirmation; 
            }
            else if(OK_2_continue_Q == -1)  
            { // abort via MODE btn
                Turn_Off_IOs(Test + ABORT); 
                RetestResponse = 0;
//                ReturnVal =  MODE_BUTTON; // operator aborted testing
                test_Kase = test_done;  
            }
            break;
                        
        case require_confirmation: // or maybe not, depending 
            // on GenericTests[Test].Request_confirmation_by_oprtr
            //// "H" 4)
            if(timers[t_Generic_Delay] < Delay_Count) break; 
            timers[t_Generic_Delay] = 0; 
//            setoutp( (void *)SEB_debug_out_array[dbg01_enum] );
            if( GenericTests[Test].Request_confirmation_by_oprtr )
            { // display new msgs perhaps
//                setoutp( (void *)SEB_debug_out_array[dbg04_enum] ); 
            	// display msgs unless already displayed.
            	if(NewMsgsDisplayed == 0) 
            	{
            	    Log_an_event_C("awaiting confirmaton");
            	    Display_new_msgs(Test); // dislpay lines //// "H"
            	    // indicated by "...while_waiting_for_event" 
            	}
            	NewMsgsDisplayed = 1;
            	// wait for operator cofirmation.
            	OK_2_continue_Q = Wait_on_operator();  ///////////  WAIT FOR OPERATOR BEFORE GOING ON
	            if( OK_2_continue_Q == 1 ) // Enter btn
	            {	                	  
//	                setoutp( (void *)Test_Osignal_array[3] ); // Debug facility	                            
	                test_Kase = test_pass; 
	            }
	            else if(OK_2_continue_Q == -1) // Mode btn  
	            { // abort via MODE btn
//	                setoutp( (void *)SEB_debug_out_array[dbg05_enum] );
	                Send_Failure_msgs_to_msg_brd_A(1, Test_with_Options); // final error msgs
	                test_Kase = test_failure;  
	            }  
	            
	            break;          		
            }
            else 
            {
//                setoutp( (void *)SEB_debug_out_array[dbg06_enum] );
                test_Kase = run_test;   
            }
            break;
                                               
        case run_test:
            if(timers[t_Generic_Delay] < Delay_Count) break;
            timers[t_Generic_Delay] = 0;  
//            setoutp( (void *)SEB_debug_out_array[dbg07_enum] );          
// CHECK FINAL //// "I"
            Read_All_inputs(0); // All_inputs
            sprintf(src,"final:0x%X",All_inputs);
            Log_an_event_C(src);
            if( Check_final_condition(Test_with_Options) ) //  1= pass; 0 = fail
            {
//                setoutp( (void *)SEB_debug_out_array[dbg08_enum] );
//                Turn_Off_IOs(Test); //  only turn off those that the struct will indicate
                setoutp( (void *)Test_Osignal_array[4] ); // Debug facility
                test_Kase =  test_pass;  
            }
            else
            { // test fail
                Send_Failure_msgs_to_msg_brd_A(1, Test_with_Options); // final error msgs
                test_Kase =  test_failure;	
            }
            break;
                        
        case test_pass: 
            CurrentTestPassFail = 1;  
            if(SW_Options)
            {
                setoutp( (void *)Test_Osignal_array[5] ); // Debug facility
                test_Kase = View_Log;	
            }
            else
            {
                New_write_display("                    ",0,0); // row, col
                New_write_display("     Good           ",1,0); // row, col
                New_write_display("   Test PASSED      ",2,0); // row, col
                New_write_display("                    ",3,0); // row, col                
                test_Kase = test_done;   
            }
            break;

	    case View_Log:
	        ReturnVal = DisplayMsgBrd_A(0);
	        if(ReturnVal == FINISHED_WITH_SERVICE)
	        {	            
	            ReturnVal = CONTINUE_SERVICE;
	            setoutp( (void *)Test_Osignal_array[6] ); // Debug facility
	            test_Kase = test_done;	
	        }
	        break;
            
        case test_done: //// "J"
            Turn_Off_IOs(Test); //  only turn off those that the struct will indicate
            test_Kase = initialize;
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;              
            break;
                                    
        case test_failure:
            Turn_Off_IOs(Test); //  only turn off those that the struct will indicate
            setoutp( (void *)Test_Osignal_array[7] ); // Debug facility
            test_Kase = Retest_option;
//            ReturnVal = Display_Message_Board(0);
//            if(ReturnVal == FINISHED_WITH_SERVICE) test_Kase = Retest_option;
//            else if(ReturnVal == CLOSE_MSG_BOARD) test_Kase = test_done;  //  CLOSE_MSG_BOARD = 4
            break;
// RetestParam            
        case Retest_option: // Required for Retest Option
            setoutp( (void *)Test_Osignal_array[8] ); // Debug facility
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) test_Kase = test_done;  // do not retest
            else if(RetestResponse == 1) test_Kase = initialize; // test again
                else ReturnVal = CONTINUE_SERVICE;    
            break;           
               
        default:
            break;                
    } // end of "switch (test_Kase)"
    
    return ReturnVal;
}


/*****************************************************/
/***                   Clear All                ******/
/*****************************************************/
void ClearAllSEB(void)
{
	int i;
//	int j = sizeof(SEB_outputs_array) / sizeof(SEB_outputs_array[0]);
//	int j =  (sizeof(SEB_outputs_array) / sizeof(SEB_outputs_array[0]));
	for (i = 0; i < 9; i++)
	{		
		clroutp( (void *)SEB_outputs_array[i]); // SEB_inputs_array
	}
}

/*****************************************************/
/***                   Clear All    COP         ******/
/*****************************************************/
void ClearAll_COP(void)
{
	int i;
	for (i = 0; i <= MAX_COP_out_index; i++)
	{		
		clroutp( (void *)SEB_1134_COP_out_array[i]);
	}
}


/*****************************************************/
/***                   Clear All from Menu      ******/
/*****************************************************/
int16 ClearAllSEB_from_a_Menu(int16 dummy)
{
    ClearAllSEB();
    return FINISHED_WITH_SERVICE;
}


/*****************************************************/
/***              Set up the inital conditions  ******/
/*****************************************************/
int16 Set_Initial_Conditions(int16 Test)
{
    int16 idx = 0;
    
    for(idx = 0; idx < 4; idx++)
    {
    	if( GenericTests[Test].Gen_outputs_to_make[idx] >= 0)
    	{
    		setoutp((void *)SEB_outputs_array[ GenericTests[Test].Gen_outputs_to_make[idx] ]);
    		if(GenericTests[Test].Gen_outputs_to_make[idx] < 6)
    		{ // These are not going to SEB and thus can not see if on by looking at LED on SEB.
    		  // Therefore, send these sets to the GALX4-0047N so that for SW debug, the LEDs can be seen
    		  // O_LDO6_enum
    		    setoutp((void *)SEB_outputs_array[ GenericTests[Test].Gen_outputs_to_make[idx] + O_LDO6_enum ]);	
    		}
    	}
    	    
    }
    return 0; 
}

/*****************************************************/
/***        Set the specific I/O for this test  ******/
/*****************************************************/
int16 Set_Specific_outputs(int16 Test)
{
    int16 idx = 0;
    
    for(idx = 0; idx < 4; idx++)
    {
    	if( GenericTests[Test].Specific_outputs_to_make[idx] >= 0)
    	{ 
    	    setoutp((void *)SEB_outputs_array[ GenericTests[Test].Specific_outputs_to_make[idx] ]);
            sprintf(src,"Tst=%d,idx=%d,Output=%d", Test, idx, GenericTests[Test].Specific_outputs_to_make[idx] );
            Add_msg_to_array_B(src, 17 + idx);      	    
    		if(GenericTests[Test].Specific_outputs_to_make[idx] < 6)
    		{ // These are not going to SEB and thus can not see if on by looking at LED on SEB.
    		  // Therefore, send these sets to the GALX4-0047N so that for SW debug, the LEDs can be seen
    		  // O_LDO6_enum
    		    setoutp((void *)SEB_outputs_array[ GenericTests[Test].Specific_outputs_to_make[idx] + O_LDO6_enum ]);	
    		}    	    	
    	}    	    
    }
    return 0;    
}

//setoutp((void *)SEB_outputs_array[Diag_IO_index]);
//clroutp((void *)SEB_outputs_array[Diag_IO_index]);

//rdinp((void *)SEB_outputs_array[Diag_IO_index]) 
// GenericTests[MAX_GENERIC_TEST].Gen_outputs_to_make[4];
// GenericTests[MAX_GENERIC_TEST].Specific_outputs_to_make[4];

/*****************************************************/
/***             Put LCD msgs up for operator   ******/
/*****************************************************/
int16 Display_LCD_msgs(int16 Test)
{
    int16 wbg_index = 0;
    
    for (wbg_index = 0; wbg_index < 4; wbg_index++)
    {
        if(GenericTests[Test].LCD_msgs_while_waiting_for_event[wbg_index] >= 0)
           	New_write_display( (char *)Operator_LCD_msgs[ GenericTests[Test].LCD_msgs[wbg_index] ] , wbg_index, 0 );
    }

    return 0;     
}

// Display_new_msgs(Test);
/*****************************************************/
/***        Put new  LCD msgs up for operator   ******/
/*****************************************************/
int16 Display_new_msgs(int16 Test)
{
    int16 wbg_index = 0;
    
    for (wbg_index = 0; wbg_index < 4; wbg_index++)
    {
        if(GenericTests[Test].LCD_msgs_while_waiting_for_event[wbg_index] >= 0)
           	New_write_display( (char *)Operator_LCD_msgs[ GenericTests[Test].LCD_msgs_while_waiting_for_event[wbg_index] ] , wbg_index, 0 );
    }

    return 0;     
}

// GenericTests[Test].LCD_msgs_while_waiting_for_event[0];
/*****************************************************/
/***         Check for Enter button or I/O on   ******/
/***    or final pattern match or mode button   ******/
/*****************************************************/
int16 check_4_event(int16 Test_with_Options)
{
    int16 Test = Test_with_Options & 0x3fff; // discard bits 15 and 14
    int16 SW_Options = Test_with_Options & 0xC000; // set options to bit 15
    int16  IO_Value_Read = 0;
    uint32 The_Expected_Pattern = GenericTests[Test].expected_final_result;   
   
//    setoutp( (void *)SEB_debug_out_array[dbg03_enum] );
    if(SW_Options)
    { // just return as though event happened
//        timers[t_Generic_Delay] = 0;
//        return 1; // simulate that event happened    	
    }
    
    if( GenericTests[Test].Wait_4_operator_input == 0 &&  GenericTests[Test].Wait_4_specific_input == 0)
    { // no need to wait on event, just return
//        timers[t_Generic_Delay] = 0;
//        setoutp( (void *)SEB_debug_out_array[dbg04_enum] );
        return 1;	
    }
    
    if( GenericTests[Test].Wait_4_operator_input ) 
    { // wait for Enter or Mode btn   
        Add_msg_to_array_B("waiting for enter   ", 0); 
        
	    if(Check_for_Mode_Button() ) {
			return -1;
	    }          
	    if(Check_for_Enter_Button() ) {
//	        timers[t_Generic_Delay] = 0;
			return 1;
	    }  
	    else if(SW_Options & 0x4000) return 1; // special debug mode
	    	
    }
	if( GenericTests[Test].Wait_4_specific_input == 2)
	{ // see if final conditions are already met.
        Read_All_inputs(0);        
        if(All_inputs == The_Expected_Pattern ) return 1;
	}    
    if(GenericTests[Test].Wait_4_specific_input == 1)
    { // rdinp  //  Read_SEB_invert
    	IO_Value_Read = rdinp((void *)SEB_outputs_array[ GenericTests[Test].Input_2_wait_on ]);
//    	setoutp( (void *)SEB_debug_out_array[dbg05_enum] );
    	if(IO_Value_Read)
    	{
//    	    timers[t_Generic_Delay] = 0;
//            setoutp( (void *)SEB_debug_out_array[dbg06_enum] );
    	    return 1;
    	}	
    }
    
    if(Check_for_Mode_Button() ) {
		return -1; // operator aborted testing.
    }
        
    return 0; // still waiting for event.        
}

/////////////////////////////////////////////

/*****************************************************/
/***         Check for Enter or Mode buttons    ******/
/*****************************************************/
int16 Wait_on_operator(void)
{

    if(Check_for_Enter_Button() ) {
//        setoutp( (void *)SEB_debug_out_array[dbg02_enum] ); // Set debug LED ON, on SEB 6, (addr= 6)
		return 1;
    } 
    
    if(Check_for_Mode_Button() ) {
//        setoutp( (void *)SEB_debug_out_array[dbg03_enum] ); // Set debug LED ON, on SEB 6, (addr= 6)
		return -1; // operator aborted testing.
    }
        
    return 0; // still waiting for event.        
}

//////////////////////////////////////////////

/*****************************************************/
/***         Check for Initial inputs OK        ******/
/*****************************************************/
int16 Check_Initial_Conditions(int16 Test_with_Options)
{
    int16 Test = Test_with_Options & 0x7fff; // discard bit 15
    int16 SW_Options = Test_with_Options & 0x8000; // set options to bit 15
    uint32 The_Expected_Pattern = GenericTests[Test].expected_initial_result;
    int16 Which_IO = 0 ; 
   
    Read_All_inputs(0);
    Which_IO = GenericTests[Test].Which_IO;
    sprintf(src,"AI_init=%x", All_inputs); 
    Log_an_event(src);    
    if(SW_Options)
    { // add All_inputs to msg board for SW debug
        return PASS; // pass    	
    }
    else return Evaluate_The_Results( The_Expected_Pattern, Which_IO); // the real thing
      // not SW debug
}

////////////////////////////////////////////// 

/*****************************************************/
/***       Check that all expected IOs are met  ******/
/*****************************************************/
int16 Check_final_condition(int16 Test_with_Options)
{
    int16 Test = Test_with_Options & 0x7fff; // discard bit 15
    int16 SW_Options = Test_with_Options & 0x8000; // set options to bit 15
    uint32 The_Expected_Pattern = GenericTests[Test].expected_final_result;
    int16 Which_IO =0; 
      
    Read_All_inputs(0);
    Which_IO = GenericTests[Test].Which_IO;
 // add All_inputs to msg board for SW debug
    sprintf(src,"AI_final=%x", All_inputs); 
    Log_an_event(src);    
    if(SW_Options)
    {
        return PASS; // pass    	
    }
    else return Evaluate_The_Results( The_Expected_Pattern, Which_IO); // the real thing
      // not SW debug
}

//////////////////////////////////////////////

// GenericTests[MAX_GENERIC_TEST].Gen_outputs_to_make[5];
// GenericTests[MAX_GENERIC_TEST].Specific_outputs_to_make[5];

/*****************************************************/
/***         Leave system in the initial condts ******/
/*****************************************************/
int16 Turn_Off_IOs(int16 Test_and_options)
{   int16 Abort = Test_and_options & ABORT;
    int16 Test  =  Test_and_options & 0xBFFF; // exclude ABORT 
    int16 idx   = 0; 

    if(Abort) ClearAllSEB();
     else 
     {  // { -1 = turn off items 0, 1, 2, 3 at end of test; -2 = leave them on.}
//        setoutp( (void *)SEB_debug_out_array[dbg02_enum] ); // debug LED
//        sprintf(src,"[4a]=%d", GenericTests[Test].Gen_outputs_to_make[4] );
//                Log_an_event(src);
         if(GenericTests[Test].Gen_outputs_to_make[4] == -1) // if A
         { // turn off
             for(idx = 0; idx < 4; idx++)
             {  // sprintf(src,"idx=%d io=%d ",idx, GenericTests[Test].Gen_outputs_to_make[idx] );
                // Log_an_event(src);
		    	if( GenericTests[Test].Gen_outputs_to_make[idx] >= 0)
		    	{  // setoutp( (void *)SEB_debug_out_array[dbg03_enum] ); // debug LED
		    		clroutp((void *)SEB_outputs_array[ GenericTests[Test].Gen_outputs_to_make[idx] ]);
		    		if(GenericTests[Test].Gen_outputs_to_make[idx] < 6) // Debug facility
		    		{ // These are not going to SEB and thus can not see if on by looking at LED on SEB.
		    		  // Therefore, send these sets to the GALX4-0047N so that for SW debug, the LEDs can be seen
		    		  // O_LDO6_enum
		    		    clroutp((void *)SEB_outputs_array[ GenericTests[Test].Gen_outputs_to_make[idx] + O_LDO6_enum ]);	
		    		}
		    	}             	
             }                      	
         } // end of "if A"
     	sprintf(src,"[4b]=%d", GenericTests[Test].Specific_outputs_to_make[4] );
                Log_an_event(src);
         if(GenericTests[Test].Specific_outputs_to_make[4] == -1) // if B
         { // turn off
             for(idx = 0; idx < 4; idx++)
             {  // sprintf(src,"IDX=%d IO=%d ",idx, GenericTests[Test].Gen_outputs_to_make[idx] );
                // Log_an_event(src);
		    	if( GenericTests[Test].Specific_outputs_to_make[idx] >= 0)
		    	{ // setoutp( (void *)SEB_debug_out_array[dbg04_enum] ); // debug LED
		    		clroutp((void *)SEB_outputs_array[ GenericTests[Test].Specific_outputs_to_make[idx] ]);
		    		if(GenericTests[Test].Specific_outputs_to_make[idx] < 6)
		    		{ // These are not going to SEB and thus can not see if on by looking at LED on SEB.
		    		  // Therefore, send these sets to the GALX4-0047N so that for SW debug, the LEDs can be seen
		    		  // O_LDO6_enum
		    		    clroutp((void *)SEB_outputs_array[ GenericTests[Test].Specific_outputs_to_make[idx] + O_LDO6_enum ]);	
		    		}
		    	}             	
             }                      	
         } // end of "if B"     	     	     	
     } // end of else     
}

int16 Standard_Init(int16 Clear_msg_Boards, int16 Stnd_Time_Delay)
{
            clr_lcd_dply();
//          Clear_Msg_Board();
			Clear_All_Boards(Clear_msg_Boards); // bit 0 for A; bit 1 for B; bit 2 for C
            CurrentTestPassFail =0;  // fail
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 3; 
            timers[t_Generic_Delay] = 0;
            return CONTINUE_SERVICE;
}

//     'Prelim_Failure_msg[]'  and 'Final_Failure_msg[]'
// and 'Prelim_Failure_msgs[]' and 'Final_Failure_msgs[]'

int16 Send_Failure_msgs_to_msg_brd_A(int16 Prelim_or_Final, int16 Test_with_Options)
{ // Log_an_event(src);
	int16 wbg_index = 0;
	int16 which_array = Prelim_or_Final;
    int16 Test = Test_with_Options & 0x3fff; // discard bits 15 and 17
    int16 SW_Options = Test_with_Options & 0xC000; // set options to bits 15 and 14	
		
	switch (which_array)
	{
		case 0: // Preliminary test
		    for (wbg_index = 0; wbg_index < 4; wbg_index++)
		    {
		        if(GenericTests[Test].Prelim_Failure_msg[wbg_index] >= 0)
		           	Log_an_event( (char *)Failure_msgs[ GenericTests[Test].Prelim_Failure_msg[wbg_index] ] );
		    }		
		    break;
		   
		case 1: // Preliminary test
		    for (wbg_index = 0; wbg_index < 4; wbg_index++)
		    {
		        if(GenericTests[Test].Final_Failure_msg[wbg_index] >= 0)
		           	Log_an_event( (char *)Failure_msgs[ GenericTests[Test].Final_Failure_msg[wbg_index] ] );
		    }		
		    break;

		default: 
		    break;		    		    		    
	} // end of "switch (which_array)"    

    return 0;
}

//    int16  Initial_output_Item[4]; // Before starting test, set these items to the
//    int16  Initial_output_State[4];

void Setup_Initial_SEB_States(int16 index_to_Generic_Test)
{
    int16 wbg_idx = 0;
    int16 OnOff = 0;
    int16 Item_to_set = 0;
    
    for(wbg_idx = 0; wbg_idx < 4; wbg_idx++)
    {
        Item_to_set = GenericTests[index_to_Generic_Test].Initial_output_Item[wbg_idx];
        OnOff       = GenericTests[index_to_Generic_Test].Initial_output_State[wbg_idx];
        
        if( Item_to_set >= 0) 
        {
            if(OnOff == 1) setoutp((void *)
                SEB_outputs_array[ GenericTests[index_to_Generic_Test].Initial_output_Item[wbg_idx] ]); 
            else clroutp((void *)
                SEB_outputs_array[ GenericTests[index_to_Generic_Test].Initial_output_Item[wbg_idx] ]);
        }
    }
}