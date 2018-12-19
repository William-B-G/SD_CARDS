// Menus.c

#include "global.h"
#include "Tests.h"

// FOR A COMPLETE LIST OF THE MENUS SEE ARCHIVED_MENUS.C
// OR XMENU.C IN //GALX_1132_TEST_FOLDER
// IN VERSION 'J' OF THE 1134 CTB TEST FIXTURE. (2018-5-24)

/*struct a_menu { // each menu consists of the text to
    // display, a function pointer, and the param
    // value to send to the function. The function
    // is executed when the user clicks the 'enter' button.
    char MenuItem[21]; // text to display.
    void *Service;  // function pointer
    int16 param;    // the parameter to send to the function.
};
*/

// Display_RS485_Baud

// sx_recbuf[SE1COM][sx_rbf[SE1COM]]
// ShowMeRxBuf
// Debug_IO_Eval
struct a_menu Main_Generic_Test_Menu[] = {
    /// = 0
    {" OPL-0106N Door Op 0", DoNothing, 0}, //
    {" VERIFY JUMPERS     ", DoNothing, 0},
    {" AUTO MODE TEST     ", DoNothing, 0},
    {" MANUAL MODE TEST   ", DoNothing, 0}, 
    {" CAN BAUD RATE TST  ", DoNothing, 0},
    {" FRONT/REAR DR TST  ", DoNothing, 0},
    {" SW VERSION         ", Display_SW_Version, 0},
    {" DIAGNOSTICS       1", Start_a_Menu, 1},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
/*    
    {" Perfrm single IO mk", Start_a_Menu, 12}, // this is for SW debug, not a real test
    {" Perform mock IO tst", Perform_all_IO_Tests, 0x8000}, // this is for SW debug, not a real test
    {" Show Msg Board B   ", DisplayMsgBrd_B, 0},
    {" Perform IO test    ", Perform_all_IO_Tests,      0}, // this is a real test
    {" Tgl LDO7 LED       ", Toggle_4_0047, 1}, // index runs from 0 - 5 for LDO6 - LDO11
    {" Tgl LDO8 LED       ", Toggle_4_0047, 2}, // index runs from 0 - 5 for LDO6 - LDO11
    {" Tgl Dbg8 LED       ", Toggle_O_CTxx, 8},
    {" Set All Dbgxx      ", Set_or_Clear_All_CTxx, 1},
    {" Clear All Dbgxx    ", Set_or_Clear_All_CTxx, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" Disply rdoutp(*ptr)", Read_and_Display_rdoutp, 0},
    {" Disply rdinp (*ptr)", Read_and_Display_Rd_Inputs, 0}, 
//  {" Disply All inputs  ", Read_and_Display_All_Inputs, 1},
//  {" Tgl SWi2 switch in ", Toggle_4_0047_input, 2}, // index runs from 0 - 5 for SWi0 - SWi5
    {" Toggle GALX4-0047N ", toggle_4_0047N, 0}, // Toggle_4_0047_input
    {" Debug IFL_COPo     ", Debug_IO_Eval, O_IFL_COPo0_enum},
    {" Debug IFB_COPo     ", Debug_IO_Eval, O_IFB_COPo1_enum},
    {" Debug CUL_COPo     ", Debug_IO_Eval, O_CUL_COPo2_enum},
    {" Debug L15RLo       ", Debug_IO_Eval, O_L15RL_enum},
    {" Show Msg Board B   ", DisplayMsgBrd_B, 0},
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Software Version   ", Display_SW_Version, 0},
    {" Test Module        ", DoNothing, 0},
    {" Faults in B        ", DisplayMsgBrd_B, 0},
    {" Clear All Faults   ", Clear_All_Boards, 7},
    {" RESET MCU          ", WDT_Test, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
*/    
};

// Toggle_O_CTxx

struct a_menu Diagnostic_Generic_Menu[] = {
    /// = 1
    {" Diagnostic menu   1", Really_DoNothing, 0}, //
    {" Set CN19V          ", Set_or_clear_CN19V, 1},
    {" Clear CN19V        ", Set_or_clear_CN19V, 0},
    {" Set CN18V          ", Set_or_clear_CN18V, 1},
    {" Clear CN18V        ", Set_or_clear_CN18V, 0},
    
    {" Set CLOSE          ", Set_or_clear_CLOSE, 1},
    {" Clear CLOSE        ", Set_or_clear_CLOSE, 0},
    {" Set OPEN           ", Set_or_clear_OPEN, 1},
    {" Clear OPEN         ", Set_or_clear_OPEN, 0},
    
    {" Set NUDGE          ", Set_or_clear_NUDGE, 1},
    {" Clear NUDGE        ", Set_or_clear_NUDGE, 0},
    {" Set HEAVY          ", Set_or_clear_HEAVY, 1},
    {" Clear HEAVY        ", Set_or_clear_HEAVY, 0},
    
    {" Set DCL            ", Set_or_clear_DCL, 1},
    {" Clear DCL          ", Set_or_clear_DCL, 0},
    {" Set RO             ", Set_or_clear_RO, 1},
    {" Clear RO           ", Set_or_clear_RO, 0},
    {" Set DOL            ", Set_or_clear_DOL, 1},
    {" Clear DOL          ", Set_or_clear_DOL, 0},
    
    {" Tgl LDO6 LED       ", Toggle_4_0047, 0}, // index runs from 0 - 5 for LDO6 - LDO11
    {" Tgl LDO7 LED       ", Toggle_4_0047, 1}, // index runs from 0 - 5 for LDO6 - LDO11
    {" Tgl LDO8 LED       ", Toggle_4_0047, 2}, // index runs from 0 - 5 for LDO6 - LDO11
    {" Tgl LDO9 LED       ", Toggle_4_0047, 3}, // index runs from 0 - 5 for LDO6 - LDO11
    {" Tgl LDO10 LED      ", Toggle_4_0047, 4}, // index runs from 0 - 5 for LDO6 - LDO11
    {" Tgl LDO11 LED      ", Toggle_4_0047, 5}, // index runs from 0 - 5 for LDO6 - LDO11 
    {" Read SW0-5 states  ", Read_State_of_0047_input_SWs, 0},
    {" Show Msg Board B   ", DisplayMsgBrd_B, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0}, 
    {" Read SW0           ", Read_State_of_one_0047_input_SWs, 0},
    {" Read SW1           ", Read_State_of_one_0047_input_SWs, 1},
    {" Read SW2           ", Read_State_of_one_0047_input_SWs, 2},
    {" Read SW3           ", Read_State_of_one_0047_input_SWs, 3},
    {" Read SW4           ", Read_State_of_one_0047_input_SWs, 4},
    {" Read SW5           ", Read_State_of_one_0047_input_SWs, 5},
    {" Clear all out SEBs ", ClearAllSEB_from_a_Menu, 0}, 
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},          
    
/*    
Read_State_of_0047_input_SWs
Read_State_of_one_0047_input_SWs
    {" Read CSS_SEB_OUT   ", Read_the_Output, 0},
    {" C120_SEB_OUT      1", Diagnostic_Control, 1},
    {" L15_SEB_OUT       2", Diagnostic_Control, 2},
    {" Read L15_SEB_OUT   ", Read_the_Output, 2},
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" FSS_SEB_OUT       3", Diagnostic_Control, 3},
    {" CST_SEB_OUT       4", Diagnostic_Control, 4},
    {" ICC_SEB_OUT       5", Diagnostic_Control, 5},
    {" ICA_SEB_OUT       6", Diagnostic_Control, 6},
    {" ICI_SEB_OUT       7", Diagnostic_Control, 7},
    {" ACC_SEB_OUT       8", Diagnostic_Control, 8},
    {" BELL_SEB_OUT      9", Diagnostic_Control, 9},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" C24_SEB_OUT      10", Diagnostic_Control,10},
    {" IFL_TOC_OUT      11", Diagnostic_Control,11},
    {" IFB_TOC_OUT      12", Diagnostic_Control,12},
    {" CUL_TOC_OUT      13", Diagnostic_Control,13},
    {" CDL_TOC_OUT      14", Diagnostic_Control,14},
    {" FANR_TOC_OUT     15", Diagnostic_Control,15},
    {" LIGR_TOC_OUT     16", Diagnostic_Control,16},
    {" CTS_SEB_IN     xxxx", Really_DoNothing, 0},
    {" INS_SEB_IN     xxxx", Really_DoNothing, 0},
    {" IEN_SEB_IN     xxxx", Really_DoNothing, 0},
    {" IU_SEB_IN      xxxx", Really_DoNothing, 0},
    {" ID_SEB_IN      xxxx", Really_DoNothing, 0},
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Main Menu        12", Start_a_Menu, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" RESET MCU          ", WDT_Test, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
*/    
};


/*
struct a_menu Test_Module_Menu[] = {
    /// = 2
    {" 1134 CTB Tst Modl 2", Really_DoNothing, 0}, //
    {" Check 24 V         ", Check_24V, 0},
    {" Check Car Light    ", Check_Car_Light, 0}, 
    {" Check loud Buzzer  ", Check_Car_Buzzer_or_Bell, 0}, 
    {" Check BELL         ", Check_Car_Buzzer_or_Bell, 1}, 
    {" Turn on 120 vac    ", C120_VAC_on_off, 1},
    {" Turn OFF 120 vac   ", C120_VAC_on_off, 0},
    {" Check Fire Light   ", Check_Car_Buzzer_or_Bell, 2}, 
    {" Test Generic 1134-0", Generic_Tester, 0x8000 + 0 },
    {" Test Generic 1134-1", Generic_Tester, 0x8000 + 1 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" Test Generic 1134-2", Generic_Tester, 0x8000 + 2 },
    {" Test Generic 1134-3", Generic_Tester, 0x8000 + 3 }, 
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Test Mesg Board C  ", Tst_DisplayMsgBrd_C, 0}, // Tst_DisplayMsgBrd_C
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};
*/

/*
struct a_menu Test_B_Module_Menu[] = {
    /// = 3
    {" 1134 Module B     3", Really_DoNothing, 0}, //
    {" Perform mock IO tst", Perform_all_IO_Tests, 0x8000}, // this is for SW debug, not a real test
    {" Perform & dsply tst", Perform_all_IO_Tests, 0xC000},
    {" Show Msg Board B   ", DisplayMsgBrd_B, 0}, // ClearAllSEB
    {" Clear all out SEBs ", ClearAllSEB_from_a_Menu, 0},
    {" Perform IO test    ", Perform_all_IO_Tests,      0}, // this is a real test
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Set All Dbgxx      ", Set_or_Clear_All_CTxx, 1},
    {" Clear All Dbgxx    ", Set_or_Clear_All_CTxx, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},
    {" Disply rdoutp(*ptr)", Read_and_Display_rdoutp, 0},
    {" Disply rdinp (*ptr)", Read_and_Display_Rd_Inputs, 0}, 

    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};
*/

/*
struct a_menu Test_C_Module_Menu[] = {
    /// = 4
    {" 1134 Module C     4", Really_DoNothing, 0}, //
    {" Perform mock IO tst", Perform_all_IO_Tests, 0x8000}, // this is for SW debug, not a real test
    {" Perform & dsply tst", Perform_all_IO_Tests, 0xC000},
    {" Show Msg Board B   ", DisplayMsgBrd_B, 0}, // ClearAllSEB
    {" Clear all out SEBs ", ClearAllSEB_from_a_Menu, 0},
    {" Set C24V ON, SEB 4 ", Set_or_clear_24V_on_SEB4, 1},
    {" Clear C24V , SEB 4 ", Set_or_clear_24V_on_SEB4, 0},
    
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},

    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu Test_CAN_Msgs_Menu[] = {
    /// = 5
    {" 1134 Test CAN msg 5", Really_DoNothing, 0}, //
    {" Set L15_T ON, SEB 3", L15_T_on_off, 1},
    {"   Clr L15_T, SEB  3", L15_T_on_off, 0},     
    {" Show Msg Board B   ", DisplayMsgBrd_B, 0}, // ClearAllSEB
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    
    {" Clear all out SEBs ", ClearAllSEB_from_a_Menu, 0},
// CT_CAN_Enable_bit
    {" Turn on 120 vac    ", C120_VAC_on_off, 1},
    {"   Turn OFF 120 vac ", C120_VAC_on_off, 0}, 
    
    {" Set C24V ON, SEB 4 ", Set_or_clear_24V_on_SEB4, 1},
    {"   Clear C24V, SEB 4", Set_or_clear_24V_on_SEB4, 0}, 
    
    {" Set L15RL ON, SEB 3", GFI_Relay_on_off, 1},
    {"   Clr L15RL, SEB  3", GFI_Relay_on_off, 0},   
    
    {" Set L15_T ON, SEB 3", L15_T_on_off, 1},
    {"   Clr L15_T, SEB  3", L15_T_on_off, 0},   
         
    {" Set oCSS ON, SEB 1 ", Set_or_clear_oCSS_on_SEB1, 1},
    {"   Clear oCSS, SEB 1", Set_or_clear_oCSS_on_SEB1, 0},
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
// Read_and_Display_All_Inputs(0);    
    {" Read CSS from 1134 ", Read_1134_CAN_input, 0},     
    {" Read CTS from 1134 ", Read_1134_CAN_input, 1},
    {" Read DET from 1134 ", Read_1134_CAN_input, 2},
        
    {" Set IFL, insp FL   ", Set_CAN_msgs, 0},
    {"   Clr IFL, insp FL ", Clear_CAN_msgs, 0},
    
    {" Set IFB, insp F buz", Set_CAN_msgs, 1},
    {"   Clear insp F buz ", Clear_CAN_msgs, 1},    
    
    {" Set CUL, insp UP   ", Set_CAN_msgs, 2},
    {"   Clr CUL, insp UP ", Clear_CAN_msgs, 2},
    
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
    
    {" Set CDL, insp DWN  ", Set_CAN_msgs, 3},
    {"   Clr CDL, insp DWN", Clear_CAN_msgs, 3},    
    
    {" Set FAN,           ", Set_CAN_msgs, 4},
    {"   Clear FAN        ", Clear_CAN_msgs, 4},
    
    {" Set LIG, Cabin Ligt", Set_CAN_msgs, 5},
    {"   Clear LIG, Light ", Clear_CAN_msgs, 5},   
    
    {" Set Selector COMM  ", Set_CAN_msgs, 6},
    {"   Clr Selector COMM", Clear_CAN_msgs, 6},    
    
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
        
    {" Set NTS COMM       ", Set_CAN_msgs, 7},
    {"   Clear NTS COMM   ", Clear_CAN_msgs, 7},    
        
    {" Set Fire Srv Stp SW", Set_CAN_msgs, 8},
    {"   Clear Fire Stp SW", Clear_CAN_msgs, 8},    
    
    {" Set FSTP           ", Set_CAN_msgs, 9},
    {"   Clear FSTP       ", Clear_CAN_msgs, 9},        
    
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
                 
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},

    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu Main_Menu[] = {
    /// = 6
    {" Main Menu         6", Really_DoNothing,   0 }, //
    {" Function Test      ", FunctionalTest,     0 }, 
    {" Diagnostic         ", Start_a_Menu,       8 },
    {" Debug              ", Start_a_Menu,       9 }, // ClearAllSEB
    {" SW Version         ", Display_SW_Version, 0 },
    {" Test Module        ", Start_a_Menu,      10 },
    {" Fault Menu         ", Start_a_Menu,      11 },
    {" - - - - - - - - -  ", Really_DoNothing,   0 },
    
    {" Run Power-Up Mode  ", JumpToPowerUp,      0 },
    {" GALX-1134 CTB     0", Start_a_Menu,       0 },    
    {" Message Board A    ", DisplayMsgBrd_A,    0 },
    {" Message Board   B  ", DisplayMsgBrd_B,    0 },
    {" Message Board   C  ", DisplayMsgBrd_C,    0 },
    {" Clear Msg Board A  ", Clear_All_Boards,   1 },
    {" Clear Msg Board B  ", Clear_All_Boards,   2 },
    {" Clear Msg Board C  ", Clear_All_Boards,   4 },
    {" Clear All Faults   ", Clear_All_Boards,   7 },    
    {" RESET MCU          ", WDT_Test,           0 },
    {" Navigator Menu   14", Start_a_Menu, 14},
    {" ON / OFF          5", Start_a_Menu, 5},
    {" GALX-1134 CTB     0", Start_a_Menu,       0 },
    {" Run Power-Up Mode  ", JumpToPowerUp,      0 },
};

struct a_menu Function_Test_Menu[] = {
    /// = 7
    {" Function Test     7", Really_DoNothing, 0}, //
    
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},    
    {" RESET MCU          ", WDT_Test, 0},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu Diagnostic_Test_Menu[] = {
    /// = 8
    {" Diagnostic Test   8", Really_DoNothing, 0}, //
    {" CSS_SEB_OUT       0", Diagnostic_Control, 0},
    {" C120_SEB_OUT      1", Diagnostic_Control, 1},
    {" L15_SEB_OUT       2", Diagnostic_Control, 2},
    {" FSS_SEB_OUT       3", Diagnostic_Control, 3},
    {" CST_SEB_OUT       4", Diagnostic_Control, 4},
    {" ICC_SEB_OUT       5", Diagnostic_Control, 5},
    {" ICA_SEB_OUT       6", Diagnostic_Control, 6},
    {" ICI_SEB_OUT       7", Diagnostic_Control, 7},
    {" ACC_SEB_OUT       8", Diagnostic_Control, 8},
    {" BELL_SEB_OUT      9", Diagnostic_Control, 9},
    {" C24_SEB_OUT      10", Diagnostic_Control,10},
    {" IFL_TOC_OUT      11", Diagnostic_Control,11},
    {" IFB_TOC_OUT      12", Diagnostic_Control,12},
    {" CUL_TOC_OUT      13", Diagnostic_Control,13},
    {" CDL_TOC_OUT      14", Diagnostic_Control,14},
    {" FANR_TOC_OUT     15", Diagnostic_Control,15},
    {" LIGR_TOC_OUT     16", Diagnostic_Control,16},
    {" CTS_SEB_IN     xxxx", Really_DoNothing, 0},
    {" INS_SEB_IN     xxxx", Really_DoNothing, 0},
    {" IEN_SEB_IN     xxxx", Really_DoNothing, 0},
    {" IU_SEB_IN      xxxx", Really_DoNothing, 0},
    {" ID_SEB_IN      xxxx", Really_DoNothing, 0},    
    
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},    
    {" RESET MCU          ", WDT_Test, 0},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu Debug_Menu[] = {
    /// = 9
    {" Debug             9", Really_DoNothing, 0}, //    
    {" Clear all out SEBs ", ClearAllSEB_from_a_Menu, 0},
    {" Turn on 120 vac    ", C120_VAC_on_off, 1},
    {"   Turn OFF 120 vac ", C120_VAC_on_off, 0},     
    {" Set C24V ON, SEB 4 ", Set_or_clear_24V_on_SEB4, 1},
    {"   Clear C24V SEB 4 ", Set_or_clear_24V_on_SEB4, 0},
    {" Set oCSS ON, SEB 1 ", Set_or_clear_oCSS_on_SEB1, 1},
    {"   Clear oCSS, SEB 1", Set_or_clear_oCSS_on_SEB1, 0},
    
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
               
    {" Set IFL, insp FL   ", Set_CAN_msgs, 0},
    {"   Clear IFL,insp FL", Clear_CAN_msgs, 0},    
    {" Set IFB, insp F buz", Set_CAN_msgs, 1},
    {"   Clear insp F buz ", Clear_CAN_msgs, 1},        
    {" Set CUL, insp UP   ", Set_CAN_msgs, 2},
    {"   Clear CUL,insp UP", Clear_CAN_msgs, 2},    
    {" Set CDL, insp DWN  ", Set_CAN_msgs, 3},
    {"   Clr CDL,insp DWN ", Clear_CAN_msgs, 3}, 
    
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
                
    {" Set FAN,           ", Set_CAN_msgs, 4},
    {"   Clear FAN        ", Clear_CAN_msgs, 4},    
    {" Set LIG, Cabin Ligt", Set_CAN_msgs, 5},
    {"   Clear LIG, Light ", Clear_CAN_msgs, 5},       
    {" Set Selector COMM  ", Set_CAN_msgs, 6},
    {"   Clear Select COMM", Clear_CAN_msgs, 6}, 
    
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
                
    {" Set NTS COMM       ", Set_CAN_msgs, 7},
    {"   Clear NTS COMM   ", Clear_CAN_msgs, 7},            
    {" Set Fire Srv Stp SW", Set_CAN_msgs, 8},
    {"   Clear Fire Stp SW", Clear_CAN_msgs, 8},        
    {" Set FSTP           ", Set_CAN_msgs, 9},
    {"   Clear FSTP       ", Clear_CAN_msgs, 9}, 
    
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
         
    {" Read CSS from 1134 ", Read_1134_CAN_input, 0},     
    {" Read CTS from 1134 ", Read_1134_CAN_input, 1},
    {" Read DET from 1134 ", Read_1134_CAN_input, 2},               
    
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
         
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},    
    {" RESET MCU          ", WDT_Test, 0},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu Test_Module_10_Menu[] = {
    /// = 10
    {" Test Module      10", Really_DoNothing,         0 }, //
    {" Check 24 V         ", Check_24V,                0 },
    {" Check Car Light    ", Check_Car_Light,          0 },
    {" Check loud Buzzer  ", Check_Car_Buzzer_or_Bell, 0 },
    {" Check Bell         ", Check_Car_Buzzer_or_Bell, 1 },
    {" Check Fire Light   ", Check_Car_Buzzer_or_Bell, 2 },
    {" CHECK LOUD BUZZER!!", Generic_Tester, 7 },
    {" CHECK BELL !!      ", Generic_Tester, 8 },
    {" CHECK FIRE LIGHT !!", Generic_Tester, 9 },
    {" Check Run/Stop SW  ", Generic_Tester,       0 },
    {" Check Inspection SW", Generic_Tester,       1 },
    {" Check Enable SW    ", Generic_Tester,       2 },
    {" Check Enbl + UP SW ", Generic_Tester,       3 },
    {" Check Enbl + DWN SW", Generic_Tester,       4 },  
    {" Check GFI Reset ON ", Generic_Tester,       5 }, // should not trip
    {" Check GFI Reset OFF", Generic_Tester,       6 }, // will trip
    {" Perform IO test    ", Perform_all_IO_Tests,     0 },
    
    {" Message Board A    ", DisplayMsgBrd_A,          0 },
    {" Message Board   B  ", DisplayMsgBrd_B,          0 },
    {" Message Board   C  ", DisplayMsgBrd_C,          0 },
    {" Clear Msg Board A  ", Clear_All_Boards,         1 },
    {" Clear Msg Board B  ", Clear_All_Boards,         2 },
    {" Clear Msg Board C  ", Clear_All_Boards,         4 },
    {" Clear All Faults   ", Clear_All_Boards,         7 },    
    {" RESET MCU          ", WDT_Test,                 0 },
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 },
};


struct a_menu Fault_Menu[] = {
    /// = 11
    {" Fault Menu       11", Really_DoNothing, 0}, //
    
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},    
    {" RESET MCU          ", WDT_Test, 0},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

// {" Perform mock IO tst", Perform_all_IO_Tests, 0x8000},
struct a_menu Single_IO_Menu[] = {
    /// = 12
    {" Single IO test   12", Really_DoNothing, 0}, //
    {" O_IFL_COPo0_enum 0 ", Perform_all_IO_Tests, 0xA000}, // 0x400,600
    {" O_IFB_COPo1_enum 1 ", Perform_all_IO_Tests, 0xA001}, // 0x400,600
    
    {" O_CUL_COPo2_enum 2 ", Perform_all_IO_Tests, 0xA002}, // 0x400,600
    {" O_CDL_COPo3_enum 3 ", Perform_all_IO_Tests, 0xA003}, // 0x400,600
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    
    {" O_FAN_COPo4_enum 4 ", Perform_all_IO_Tests, 0xE004}, // 0x400,400
    {" O_LIG_COPo5_enum 5 ", Perform_all_IO_Tests, 0xE005}, // 0x400,200
    
    {" O_CSS_enum 6       ", Perform_all_IO_Tests, 0xE006}, // 0x400,600
    {" O_FSS_enum 10      ", Perform_all_IO_Tests, 0xE007}, // 0x400,600
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    
    {" O_CST_enum 11      ", Perform_all_IO_Tests, 0xE008}, // 0x408,600
    {" O_ICC_enum 12      ", Perform_all_IO_Tests, 0xE009}, // 0x400,600
    
    {" O_ICA_enum 13      ", Perform_all_IO_Tests, 0xE00A}, // 0x420,600
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" O_ICI_enum 14      ", Perform_all_IO_Tests, 0xE00B}, // 0x440,600
    {" O_ACC_enum 15      ", Perform_all_IO_Tests, 0xE00C}, // 0x480,600
    
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},    
    {" RESET MCU          ", WDT_Test, 0},
    {" IO Menu            ", Start_a_Menu,  13},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu IO_Menu[] = {
    /// = 13
    {" IO Menu       13   ", Really_DoNothing, 0}, //
    {" Perform IO test    ", Perform_all_IO_Tests,     0 },
    {" Single IO          ", Start_a_Menu,  12},
    {" Perform mock IO tst", Perform_all_IO_Tests, 0x8000}, // this is for SW debug, not a real test
    {" IO tst- w/msgs     ", Perform_all_IO_Tests, 0xC000}, // this is for SW debug, not a real test
    
    {" Message Board   A  ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},    
    {" RESET MCU          ", WDT_Test, 0},
    {" MAIN Menu         0", Start_a_Menu,  6 },
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu Navigator_Menu[] = {
    /// = 14
    {" Navigator Menu   14", Really_DoNothing, 0}, //
    {" Check GFI          ", Generic_Tester,       5 },
    {" ON - OFF      5    ", Start_a_Menu, 5},
    {" Diagnostics   1    ", Start_a_Menu, 1},
    {" IO            13   ", Start_a_Menu, 13},
    {" IO - Single   12   ", Start_a_Menu, 12},
    {" Main          6    ", Start_a_Menu, 6},
    {" Generic w/Optns 15 ", Start_a_Menu, 15},
    {" GFI & IO      16   ", Start_a_Menu, 16},
    {" Sngl IO NO OPT   17", Start_a_Menu, 17},
    
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7}, 
   
    {" RESET MCU          ", WDT_Test, 0},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu Generic_Options_Menu[] = {
    /// = 15
    {" Generic w/Options15", Really_DoNothing,         0 }, //
    
    {" Turn on 120 + 24   ", C120_Plus_24_on_off, 1}, 
        
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 },
    
    {" Check GFI          ", Generic_Tester,        5 },
    {" Check GFI w/ optns ", Generic_Tester,       0x8000 + 5 },
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 },
        
    {" Check Run/Stop SW  ", Generic_Tester,        0 },
    {" Check R/S w Optns  ", Generic_Tester,       0x8000 + 0 },
    
    {" Check Inspection SW", Generic_Tester,       1 },
    {" Check Insp w optns ", Generic_Tester,       0x8000 + 1 },
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 },
        
    {" Check Enable SW    ", Generic_Tester,       2 },
    {" Check Enbl w Optns ", Generic_Tester,       0x8000 + 2 },
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 }, 
     
    {" Check Enbl + UP SW ", Generic_Tester,       3 },
    {" Check Enbl+Up w opt", Generic_Tester,       0x8000 + 3 },
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 }, 
    
    {" Check Enbl + UP DWN", Generic_Tester,       4 },
    {" Check Enbl+Dwn opts", Generic_Tester,       0x8000 + 4 },
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 }, 
    
    {" Turn on 120 + 24   ", C120_Plus_24_on_off, 1},
    {" Turn OFF 120 + 24  ", C120_Plus_24_on_off, 0},          

    {" Message Board A    ", DisplayMsgBrd_A,          0 },
    {" Message Board   B  ", DisplayMsgBrd_B,          0 },
    {" Message Board   C  ", DisplayMsgBrd_C,          0 },
    {" Clear All Faults   ", Clear_All_Boards,         7 },
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 },
        
    {" Turn on 120 vac    ", C120_VAC_on_off, 1},
    {"   Turn OFF 120 vac ", C120_VAC_on_off, 0}, 
    
    {" Set C24V ON, SEB 4 ", Set_or_clear_24V_on_SEB4, 1},
    {"   Clear C24V, SEB 4", Set_or_clear_24V_on_SEB4, 0},                     
    
    {" Message Board A    ", DisplayMsgBrd_A,          0 },
    {" Message Board   B  ", DisplayMsgBrd_B,          0 },
    {" Message Board   C  ", DisplayMsgBrd_C,          0 },
    {" Clear Msg Board A  ", Clear_All_Boards,         1 },
    {" Clear Msg Board B  ", Clear_All_Boards,         2 },
    {" Clear Msg Board C  ", Clear_All_Boards,         4 },
    {" Clear All Faults   ", Clear_All_Boards,         7 },    
    {" RESET MCU          ", WDT_Test,                 0 },
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp,            0 },
};

struct a_menu GFI_a_IO_Menu[] = {
    /// = 16
    {" GFI and IO menu  16", Really_DoNothing, 0}, //
    {" Check GFI No Trip  ", Generic_Tester,       5 },
    {" Chk GFI Reset Trips", Generic_Tester,       6 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0}, 
    {" Navigator Menu   14", Start_a_Menu, 14},   
    {" Clear all out SEBs ", ClearAllSEB_from_a_Menu, 0},
    {" Perform IO test    ", Perform_all_IO_Tests,     0 },
    {" Perfm IO tst Optns ", Perform_all_IO_Tests,     0xC000 },
    {" Single IO          ", Start_a_Menu,  17},    
    {" Check BELL         ", Check_Car_Buzzer_or_Bell, 1 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    
    {" Turn on L15 Relay  ", L15RL_on_off, 1},
    {"   L15 Relay OFF    ", L15RL_on_off, 0},
    {" Turn on 120 vac    ", C120_VAC_on_off, 1},
    {"   Turn OFF 120 vac ", C120_VAC_on_off, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
         
    {" Set C24V ON, SEB 4 ", Set_or_clear_24V_on_SEB4, 1},
    {"   Clear C24V SEB 4 ", Set_or_clear_24V_on_SEB4, 0}, 
    {" Set L15_T ON, SEB 2", L15_T_on_off, 1},
    {"   Clear L15_T SEB 2", L15_T_on_off, 0},
    {" Set three ON,      ", Set_Clear_C120V_24_L15_T, 1},
    {"   Clear three      ", Set_Clear_C120V_24_L15_T, 0},     
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
       
    // L15_T_on_off   Set_Clear_C120V_24_L15_T
    {" Clear all out SEBs ", ClearAllSEB_from_a_Menu, 0},
    {" Turn on 120 + 24   ", C120_Plus_24_on_off, 1},
    {" Turn OFF 120 + 24  ", C120_Plus_24_on_off, 0},
    {" Show me SEB inputs ", Read_and_Display_All_Inputs, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},    
    
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    {" Main          6    ", Start_a_Menu, 6},
    {" Generic w/Optns 15 ", Start_a_Menu, 15},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},    
    {" RESET MCU          ", WDT_Test, 0},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
};

struct a_menu Single_IO_Menu_B[] = {
    /// = 17  NO OPTIONS - REAL THING
    {" Sngl IO NO OPT   17", Really_DoNothing, 0}, //
    {" RESET MCU          ", WDT_Test, 0},
    {" O_IFL_COPo0_enum 0 ", Perform_all_IO_Tests, 0x400 + 0}, // 0x400,600
    {" O_IFB_COPo1_enum 1 ", Perform_all_IO_Tests, 0x400 + 1}, // 0x400,600
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    
    {" O_CUL_COPo2_enum 2 ", Perform_all_IO_Tests, 0x400 + 2}, // 0x400,600
    {" O_CDL_COPo3_enum 3 ", Perform_all_IO_Tests, 0x400 + 3}, // 0x400,600
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    
    {" O_FAN_COPo4_enum 4 ", Perform_all_IO_Tests, 0x400 + 4}, // 0x400,400
    {" O_LIG_COPo5_enum 5 ", Perform_all_IO_Tests, 0x400 + 5}, // 0x400,200
    
    {" O_CSS_enum 6       ", Perform_all_IO_Tests, 0x400 + 6}, // 0x400,600
    {" O_FSS_enum 10      ", Perform_all_IO_Tests, 0x400 + 7}, // 0x400,600
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    
    {" O_CST_enum 11      ", Perform_all_IO_Tests, 0x400 + 8}, // 0x408,600
    {" O_ICC_enum 12      ", Perform_all_IO_Tests, 0x400 + 9}, // 0x400,600
    
    {" O_ICA_enum 13      ", Perform_all_IO_Tests, 0x400 + 0xA}, // 0x420,600
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" O_ICI_enum 14      ", Perform_all_IO_Tests, 0x400 + 0xB}, // 0x440,600
    {" O_ACC_enum 15      ", Perform_all_IO_Tests, 0x400 + 0xC}, // 0x480,600
    {" Run Power-Up Mode  ", JumpToPowerUp, 0},
    
    {" Message Board A    ", DisplayMsgBrd_A, 0},
    {" Message Board   B  ", DisplayMsgBrd_B, 0},
    {" Message Board   C  ", DisplayMsgBrd_C, 0},
    {" Clear Msg Board A  ", Clear_All_Boards, 1},
    {" Clear Msg Board B  ", Clear_All_Boards, 2},
    {" Clear Msg Board C  ", Clear_All_Boards, 4},
    {" Clear All Faults   ", Clear_All_Boards, 7},    
    {" RESET MCU          ", WDT_Test, 0},
    {" IO Menu            ", Start_a_Menu,  13},
    {" GFI and IO menu  16", Start_a_Menu,  16},
    {" Main Menu          ", Start_a_Menu,   6},
    {" GALX-1134 CTB     0", Start_a_Menu,  0 },
    {" Run Power-Up Mode  ", JumpToPowerUp,  0},
};
*/


/*
struct Menus_in_System  {
    // This info gets pushed onto the menu stack 
    // with a call to "Start_a_Menu(WhichMenu)".  (WBG)
    // Handles sub-menus up to 23 deep.
    void *MenuPtr;
    int16 LengthOfMenu;
} 
*/

struct Menus_in_System The_Menus_in_System[] = 
{
        {Main_Generic_Test_Menu, (sizeof(Main_Generic_Test_Menu) / sizeof(Main_Generic_Test_Menu[0])) - 1},
       
        {Diagnostic_Generic_Menu, (sizeof(Diagnostic_Generic_Menu) / sizeof(Diagnostic_Generic_Menu[0])) - 1}, // 5
/*        
        {Test_Module_Menu, (sizeof(Test_Module_Menu) / sizeof(Test_Module_Menu[0])) - 1},
        {Test_B_Module_Menu, (sizeof(Test_B_Module_Menu) / sizeof(Test_B_Module_Menu[0])) - 1},
        {Test_C_Module_Menu, (sizeof(Test_C_Module_Menu) / sizeof(Test_C_Module_Menu[0])) - 1},
        {Test_CAN_Msgs_Menu, (sizeof(Test_CAN_Msgs_Menu) / sizeof(Test_CAN_Msgs_Menu[0])) - 1},
        {Main_Menu, (sizeof(Main_Menu) / sizeof(Main_Menu[0])) - 1}, 							    	// 6
        {Function_Test_Menu, (sizeof(Function_Test_Menu) / sizeof(Function_Test_Menu[0])) - 1},  		// 7
        {Diagnostic_Test_Menu, (sizeof(Diagnostic_Test_Menu) / sizeof(Diagnostic_Test_Menu[0])) - 1},   // 8
        {Debug_Menu, (sizeof(Debug_Menu) / sizeof(Debug_Menu[0])) - 1},  								// 9
        {Test_Module_10_Menu, (sizeof(Test_Module_10_Menu) / sizeof(Test_Module_10_Menu[0])) - 1},      // 10
        {Fault_Menu, (sizeof(Fault_Menu) / sizeof(Fault_Menu[0])) - 1},  							    // 11
        {Single_IO_Menu, (sizeof(Single_IO_Menu) / sizeof(Single_IO_Menu[0])) - 1},  				    // 12
        {IO_Menu, (sizeof(IO_Menu) / sizeof(IO_Menu[0])) - 1},  				                        // 13
        {Navigator_Menu, (sizeof(Navigator_Menu) / sizeof(Navigator_Menu[0])) - 1},                     // 14
        {Generic_Options_Menu, (sizeof(Generic_Options_Menu) / sizeof(Generic_Options_Menu[0])) - 1},   // 15
        {GFI_a_IO_Menu, (sizeof(GFI_a_IO_Menu) / sizeof(GFI_a_IO_Menu[0])) - 1},                        // 16
        {Single_IO_Menu_B, (sizeof(Single_IO_Menu_B) / sizeof(Single_IO_Menu_B[0])) - 1},  
*/                              // 17
};
        

int16 Start_a_Menu(int16 WhichMenu)
{
    if (PushMenuPtrArray(MenuPtr))
    {
        MenuPtr = The_Menus_in_System[WhichMenu].MenuPtr; //
        TheMenuLength = The_Menus_in_System[WhichMenu].LengthOfMenu;
        //    MenuPtrArrayIndex = 0;
        LCD_Pointer = 1;
        LCD_Upd_Dpy = 1; // force update of display
    }
    return FINISHED_WITH_SERVICE;
}

int16 PushMenuPtrArray(void *MenuPtr) // saves the current menu ifo on a stack
{
    if (MenuPtr == NULL)
        return 1; // indicate success even though not.
                  // This is necessary, since the start menue may not be the
                  // main menu, and thus we should not push a NULL ptr to the stack
                  // prior to starting a child menu from a parent menu.
    if (MenuPtrArrayIndex < MAX_MENU_DEPTH)
    {
        The_menu_stack[MenuPtrArrayIndex].MenuPtrOnStack = MenuPtr;
        The_menu_stack[MenuPtrArrayIndex].LCD_PointerOnStack = LCD_Pointer;
        The_menu_stack[MenuPtrArrayIndex].LengthOfMenuOnStack = TheMenuLength;
        MenuPtrArrayIndex++;
        SW_Vers_disp();
        return SUCCESS; // = 1
    }
    else
        return FAILED; // = 0
}

int16 PopMenuPtrArray(void)
{
    if (MenuPtrArrayIndex > 0)
    {
        --MenuPtrArrayIndex;
        LCD_Pointer = The_menu_stack[MenuPtrArrayIndex].LCD_PointerOnStack;
        MenuPtr = The_menu_stack[MenuPtrArrayIndex].MenuPtrOnStack;
        //        sprintf(src,"index/MenP1: %d: %d",MenuPtrArrayIndex, MenuPtr);
        //        Log_a_fault(src,0);
        TheMenuLength = The_menu_stack[MenuPtrArrayIndex].LengthOfMenuOnStack;
        //        sprintf(src,"menLen: %d", TheMenuLength);
        //        Log_a_fault(src,0);
        //        Log_a_fault("pop OK",0);
        //        if(MenuPtr != MainMenuPtr) Log_a_fault("pointers NG",0);
        //        sprintf(src,"ML: %d", TheMenuLength);
        //        Log_a_fault(src,0);
        //        sprintf(src,"lcdP: %d", LCD_Pointer);
        //        Log_a_fault(src,0);
        //        sprintf(src,"MenP2: %d", MenuPtr);
        //        Log_a_fault(src,0);
        LCD_Upd_Dpy = 1; // force update of display
        SW_Vers_disp();
        return SUCCESS;
    }
    else
    {
        return FAILED;
    }
}

int16 Get_Num_of_Menus(void)
{
    return (sizeof(The_Menus_in_System) / sizeof(The_Menus_in_System[0]));
}
