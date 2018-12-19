// Menus.c

#include "global.h"
#include "Tests.h"

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

/*
  int16 Start_a_Menu(int16 WhichMenu)
  
    Main_MenuList[] =  0
    IndividualTests_Menu_List[]  = 1
    SubMenu_B_List[]  = 2
    SubMenu_C_List[]  = 3
    groupCom_Menu_List[]   = 4
    Diagnostic_Menu_List[] =  5
    FPGA_Diag_Menu_List[]  =  6
    MyMenuList[] =  7
*/

struct a_menu Diagnostic_1134_CTB_Menu[] = { /// = 11
    { " 1134 CTB Diagntc 11", Really_DoNothing,            0 },  //
    { " CSS_SEB_OUT        ", Diagnostic_Control,            0 },
    { " C120_SEB_OUT       ", Diagnostic_Control,            1 },
    { " L15_SEB_OUT        ", Diagnostic_Control,            2 },
    { " FSS_SEB_OUT        ", Diagnostic_Control,            3 }, 
    { " CST_SEB_OUT        ", Diagnostic_Control,           4 },
    { " ICC_SEB_OUT        ", Really_DoNothing,  0x8000 + 23 },
    { " ICA_SEB_OUT        ", Really_DoNothing,            0 },   
    { " ICI_SEB_OUT        ", Really_DoNothing,    0x1000 + 0},
    { " ACC_SEB_OUT        ", Really_DoNothing,    0x1000 + 1},
    { " BELL_SEB_OUT       ", Really_DoNothing,    0x5000 + 2},
    { " C24_SEB_OUT        ", Really_DoNothing,    0x1000 + 3},   
    { " IFL_TOC_OUT        ", Really_DoNothing,            0 },
    { " IFB_TOC_OUT        ", Really_DoNothing,            0 },
    { " CUL_TOC_OUT        ", Really_DoNothing,            0 },
    { " CDL_TOC_OUT        ", Really_DoNothing,            0 }, 
    { " FANR_TOC_OUT       ", Really_DoNothing,            0 },
    { " LIGR_TOC_OUT       ", Really_DoNothing,            0 },
    { " CTS_SEB_IN         ", Really_DoNothing,            0 },   
    { " INS_SEB_IN         ", Really_DoNothing,            0 },
    { " IEN_SEB_IN         ", Really_DoNothing,            0 },
    { " IU_SEB_IN          ", Really_DoNothing,            0 },
    { " ID_SEB_IN          ", Really_DoNothing,            0 },         
    { " Message Board A    ", DisplayMsgBrd_A,             0 },
    { " Main Menu        12", Start_a_Menu,               12 },        
    { " Run Power-Up Mode  ", JumpToPowerUp,               0 },     
    { " RESET MCU          ", WDT_Test,                    0 },
    { " Run Power-Up Mode  ", JumpToPowerUp,               0 },
};


struct a_menu Main_1134_CTB_Menu[] = { /// = 12
    { " GALX-1134 CTB    12", DoNothing,       0 },  //
    { " Function Test      ", DoNothing,     kDLB},
    { " Diagnostic         ", Start_a_Menu,    11},
    { " Tgl LDO7 LED       ", Toggle_4_0047, 1}, // index runs from 0 - 5 for LDO6 - LDO11
    { " Tgl LDO8 LED       ", Toggle_4_0047, 2}, // index runs from 0 - 5 for LDO6 - LDO11
    { " Toggle GALX4-0047N ", toggle_4_0047N,       0 },
    { " Debug              ", DoNothing,     kDLT},
    { " Software Version   ", Display_SW_Version,       0 }, 
    { " Test Module        ", DoNothing,       0 },
    { " Message Board A    ", DisplayMsgBrd_A,             0 },
    { " Faults in B        ", DisplayMsgBrd_B,      0 },
    { " Clear All Faults   ", Clear_All_Boards,     7 },              
    { " RESET MCU          ", WDT_Test,        0 }, 
    { " Run Power-Up Mode  ", JumpToPowerUp,   0 },       
};

// Get_Srl_Hall_Station_Pkg
// Reset_1137_board
// extern int16 Send_Hall_Data(int16 msg);
// extern int16 Send_Hall_Reset(int16 dummy);
// DisplayGrpUnpkData
// Reset_14_and_16
struct a_menu Bds_Menu[] = { /// = 13
    { " 1137 & 1139 Menu 13", DoNothing,            0 },  //   
    { " 1137 Hall Stn test ", One_Hall_Station_CAN,     0 },     
    { " 1137 Clear out 1&2 ", Set_or_Clear_1137_Input, 0xff00 },
    { " 1137 Set out 1 & 2 ", Set_or_Clear_1137_Input, 0xff01 },
    { " 1137 Clear out 1   ", Set_or_Clear_1137_Input, 0xf100 },
    { " 1137 Set out 1     ", Set_or_Clear_1137_Input, 0xf101 },
    { " 1137 Clear out 2   ", Set_or_Clear_1137_Input, 0xf200 },
    { " 1137 Set out 2     ", Set_or_Clear_1137_Input, 0xf201 },        
    { " 1137 Hall OFF      ", Send_Hall_Data,       0 },
    { " 1137 Hall ON       ", Send_Hall_Data,       5 },
    { " RESET  All 1137s   ", Reset_All_1137s,      0 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " 1137 Hall RESET 0  ", Send_Hall_Reset,      0 },
    { " 1137 Hall RESET 10 ", Send_Hall_Reset,     10 },
    { " Request packet 14  ", Req_packet_14,        1 },
    { " Reset 14 and 16    ", Reset_14_and_16,      0 },
    { " Display Hall data  ", DisplayGrpUnpkData,   0 },
    { " Faults in B        ", DisplayMsgBrd_B,      0 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " 1137 Hall Data     ", Hall_Station_CAN_Data,0 },
//  { " 1137 Hall Stn 0    ", Hall_Station_CAN,     0 }, // do not overwrite pack data    
//  { " 1137 Hall Stn 5    ", Hall_Station_CAN,     5 },
    { " 1137 Board reset   ", Reset_1137_board,    15 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " Faults in A        ", DisplayMsgBrd_A,      0 },
    { " Faults in B        ", DisplayMsgBrd_B,      0 },
    { " Clear All Faults   ", Clear_All_Boards,     7 },
    { " 1137 Hall Stn 30   ", One_Hall_Station_CAN,     30}, // Tx and Rx with Hall Station
    { " Hall Stn forever   ", One_Hall_Station_CAN,     -1},
    { " Group CAN          ", TEST_GRCAN,           16},
    { " Faults in A        ", DisplayMsgBrd_A,      0 },    
    { " Faults in B        ", DisplayMsgBrd_B,      0 },
    { " 1139 Hall Lantern  ", HALL_Lantern_RS485,   2 }, // Tx (19.2) to Hall Lantern; No receive
       // connect 1139 to RS-485 port and set address to 20
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " RS-485 BAUD RATE   ", Start_a_Menu,         2 },  // menu 2 (Baud rate)
    { " Faults in A        ", DisplayMsgBrd_A,      0 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " Faults in B        ", DisplayMsgBrd_B,      0 },                 
    { " RESET MCU          ", WDT_Test,             0 },  
    { " SOFTWARE VERSION   ", Display_SW_Version,   0 },
    { " Validate 1137      ", Start_a_Menu,        14 },
    { " menu vector      20", Start_a_Menu,        20 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },  
};

struct a_menu Validate_Bds_Menu[] = { /// = 14
    { " Validate 1137    14", DoNothing,            0 },  //
    { " 1137 Hall Stn test ", One_Hall_Station_CAN,     0 },
    { " RESET 10           ", Send_Hall_Reset,     10 },
    { " Hall OFF           ", Send_Hall_Data,       0 },
    { " RESET 0            ", Send_Hall_Reset,      0 },    
    { " Hall ON            ", Send_Hall_Data,       5 },
    { " RESET 10           ", Send_Hall_Reset,     10 },
    { " Hall OFF           ", Send_Hall_Data,       0 },
    { " RESET 0            ", Send_Hall_Reset,      0 },
    { " 1137 Set out 1     ", Set_or_Clear_1137_Input, 0xf101 },
    { " RESET 8            ", Send_Hall_Reset,      8 },
    { " 1137 Clear out 1   ", Set_or_Clear_1137_Input, 0xf100 },
    { " RESET 0            ", Send_Hall_Reset,      0 },
    { " 1137 Set out 2     ", Set_or_Clear_1137_Input, 0xf201 },
    { " RESET 2            ", Send_Hall_Reset,      2 },
    { " 1137 Clear out 2   ", Set_or_Clear_1137_Input, 0xf200 },
    { " RESET 0            ", Send_Hall_Reset,      0 },
    { " -------------      ", DoNothing,            0 },  //
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " 1137 Set out 1 & 2 ", Set_or_Clear_1137_Input, 0xff01 },
    { " RESET 10           ", Send_Hall_Reset,     10 },
    { " RESET 0            ", Send_Hall_Reset,      0 },
    
    { " 1137 Clear out 1&2 ", Set_or_Clear_1137_Input, 0xff00 },
    { " 1137 Set out 1 & 2 ", Set_or_Clear_1137_Input, 0xff01 },    
    { " 1137 Clear out 1   ", Set_or_Clear_1137_Input, 0xf100 },
    { " 1137 Set out 1     ", Set_or_Clear_1137_Input, 0xf101 },
    { " 1137 Clear out 2   ", Set_or_Clear_1137_Input, 0xf200 },
    { " 1137 Set out 2     ", Set_or_Clear_1137_Input, 0xf201 },    
    { " Hall OFF           ", Send_Hall_Data,       0 },
    { " Hall chan 1 on     ", Send_Hall_Data,       4 },
    { " Hall chan 2 on     ", Send_Hall_Data,       1 },
    { " Display chanls 8,9 ", Display_channels,     1 },
    { " Reset chanls 8,9   ", Display_channels,     0 },
    { " RESET 0            ", Send_Hall_Reset,      0 },
    { " RESET 2            ", Send_Hall_Reset,      2 },
    { " RESET 8            ", Send_Hall_Reset,      8 },
    { " RESET 10           ", Send_Hall_Reset,     10 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " menu vector      20", Start_a_Menu,        20 },                
    { " RESET MCU          ", WDT_Test,             0 }, 
    { " 1137 & 1139 Menu 13", Start_a_Menu,        13 },
    { " Test 1137 & 1139   ", Start_a_Menu,        15 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },   
};

struct a_menu Hall_Bds_Menu[] = { /// = 15
    { " Test 1137 & 1139 15", DoNothing,            0 },  //
    { " 1137 Hall Stn test ", One_Hall_Station_CAN,     0 },
    { " 1139 Hall Lantern 0", HALL_Lantern_RS485,   0 },
    { " 1139 Hall Lantern 1", HALL_Lantern_RS485,   1 },
    { " 1139 Hall Lantern 2", HALL_Lantern_RS485,   2 }, // Send_Hall_Lantern_Floor
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " Select Lantern Up  ", Send_Hall_Lantern_Floor,   0x40 },
    { " Select Lantern Down", Send_Hall_Lantern_Floor,   0x80 },
//    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " Select Lantern Both", Send_Hall_Lantern_Floor,   0xc0 },
    { " Select Lantern None", Send_Hall_Lantern_Floor,   0 },
//    { " Time 4 Snap Shot   ", TimeForTheSnapShot    0 },  // forgot the comma after TimeForTheSnapShot 
    { " Time 4 Snap Shot   ", TimeForTheSnapShot,    0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " Clear Faults       ", ClearFaults,          0 },    
    { " 1137 & 1139 Menu 13", Start_a_Menu,        13 },                
    { " Validate 1137    14", Start_a_Menu,        14 },
    { " Test 1137 & 1139 16", Start_a_Menu,        16 },
    { " menu vector      20", Start_a_Menu,        20 },
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },   
};
    
struct a_menu Main_Hall_Bds_Menu[] = { /// = 16
    { " Test 1137 & 1139 16", DoNothing,            0 },  //
    { " 1137 QTY Menu      ", Start_a_Menu,        17 },
    { " 1139 QTY Menu      ", Start_a_Menu,        18 },
    { "      HELP          ", Start_a_Menu,        19 }, 
    { " RESET MCU          ", WDT_Test,             0 },   
/*    
    { " 1137 Hall Stn test ", Hall_Station_CAN,     0 },
    { " 1139 Hall Lantern  ", HALL_Lantern_RS485,   2 },
    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 }, 
    { " Show faults A      ", DisplayMsgBrd_A,      0 },
    { " Clear Faults A & C ", Clear_All_Boards,     5 },
         
    { " RESET  All 1137s   ", Reset_All_1137s,      0 },
    { " 1137 QTY Menu      ", Start_a_Menu,        17 },
    { " 1139 QTY Menu      ", Start_a_Menu,        18 },
    { " 1137 Hall OFF      ", Send_Hall_Data,       0 },
    { " 1137 Hall ON       ", Send_Hall_Data,       5 },
    { " 1137 Hall Up On    ", Send_Hall_Data,       1 },
    { " 1137 Hall Down On  ", Send_Hall_Data,       5 },
    { "       HELP         ", Start_a_Menu,        19 },
    { " menu vector      20", Start_a_Menu,        20 },
    { " Clear 1107AN LEDs  ", Set_or_Clear_All,     0 },
    { " Set   1107AN LEDs  ", Set_or_Clear_All,     1 },
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 }, 
*/      
};

struct a_menu Select_1137_Qty_Menu[] = { /// = 17
    { " 1137 QTY Menu    17", DoNothing,            0 },  //
    { " 3 - Up (three brds)", Qty_113x_bds,         3 },
    { " 2 - Up (two boards)", Qty_113x_bds,         2 }, 
    { " 1 - Up (one only)  ", Qty_113x_bds,         1 },
    { " Diagnostics        ", Start_a_Menu,        25 },
    
//    { " Show grpcan_rx  #'s", Show_grp_can_Packet_num,      0 }, // grcan_rx
//    { " Toggle comm to 1137", toggle_1137_comm ,      0 }, // grcan_rx, delay_for_Comm_to_1137, Incr_Decr_10
//    { " Incr/Decr delay    ", Incr_Decr_10 ,      0 },
//    { " menu vector      20", Start_a_Menu,        20 },
        
//    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 }, 
//    { " Show faults A      ", DisplayMsgBrd_A,      0 },
//    { " Clear Faults A & C ", Clear_All_Boards,     5 },        
//    { " RESET MCU          ", WDT_Test,             0 },    
//    { " Run Power-Up Mode  ", JumpToPowerUp,        0 }, 
};

struct a_menu Select_1139_Qty_Menu[] = { /// = 18
    { " 1139 QTY Menu    18", DoNothing,            0 },  //
    { " 3 - Up (three brds)", Qty_113x_bds,         0x1103 },
    { " 2 - Up (two boards)", Qty_113x_bds,         0x1102 },
    { " 1 - Up (one only)  ", Qty_113x_bds,         0x1101 },
    { " Diagnostics        ", Start_a_Menu,        27 },
//    { " Show 1107 inputs   ", ShowMeCTi0_CTi5,      0 }, // Show_grp_can_Packet_num  
//    { " menu vector      20", Start_a_Menu,        20 },  
         
//    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 }, 
//    { " Show faults A      ", DisplayMsgBrd_A,      0 },
//    { " Clear Faults A & C ", Clear_All_Boards,     5 },        
//    { " RESET MCU          ", WDT_Test,             0 },    
//    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },     
};

struct a_menu min_documentation[] = { /// = 19
    { "      HELP       .19", DoNothing,            0 },  //
    { " SOFTWARE VERSION   ", Display_SW_Version,   0 },
    { " io.dat             ", Display_a_min_doc,    0 },
    { " consB.dat          ", Display_a_min_doc,    1 },  
    { " consB[]            ", Display_a_min_doc,    5 },
    { " 1107AN 24v I/O     ", Display_a_min_doc,    2 },  
    { " Files location     ", Display_a_min_doc,    3 },
    { " 1137;1139 Dip Swtch", Display_a_min_doc,    4 },
    { " Show faults A      ", DisplayMsgBrd_A,      0 },
    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 },     
    { " Clear Faults A & C ", Clear_All_Boards,     5 },
    { " menu vector      20", Start_a_Menu,        20 },   
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },     
};
    
struct a_menu menu_vector[] = { /// = 20
    { " menu vector      20", DoNothing,            0 },  //
    { " SOFTWARE VERSION   ", Display_SW_Version,   0 },
    { " 1137 & 1139 Menu 13", Start_a_Menu,        13 },
    { " Validate 1137    14", Start_a_Menu,        14 },
    { " Test 1137 & 1139 15", Start_a_Menu,        15 },  
    { " Test 1137 & 1139 16", Start_a_Menu,        16 },  
    { " 1137 QTY Menu    17", Start_a_Menu,        17 },
    { " 1139 QTY Menu    18", Start_a_Menu,        18 },
    { " minimal document.19", Start_a_Menu,        19 }, 
    { " Reset Prob 1137  21", Start_a_Menu,        21 }, 
    { " Short seq 2nd hlf22", Start_a_Menu,        22 },
    { " Long seq 2nd hlf 23", Start_a_Menu,        23 },  
    { " Easy start menu  24", Start_a_Menu,        24 }, 
    { " Select Position  25", Start_a_Menu,        25 },
    { " 1137 Diagnostics 26", Start_a_Menu,        26 }, 
    { " 1139 Diagnostics 27", Start_a_Menu,        27 },    
       
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },     
};    

// Unpk_Hall_Station
struct a_menu Locate_Reset_Problem[] = { /// = 21
    { " Reset Prob 1137  21", DoNothing,                    0 },  //
    { " Hall OFF no ack    ", Send_Hall_Data,          0x2000 },    
    { " Hall ON  no ack    ", Send_Hall_Data,          0x2005 }, 
    { " RESET  All 1137s  0", Reset_All_1137s,      0x0001 },   
    { " 1107 Clear out 1&2 ", Set_or_Clear_1137_Input, 0xff00 },
    { " 1107 Set out 1 & 2 ", Set_or_Clear_1137_Input, 0xff01 },
    { " 1107 Clear out 1   ", Set_or_Clear_1137_Input, 0xf100 },
    { " 1107 Set out 1     ", Set_or_Clear_1137_Input, 0xf101 },
    { " 1107 Clear out 2   ", Set_or_Clear_1137_Input, 0xf200 },
    { " 1107 Set out 2     ", Set_or_Clear_1137_Input, 0xf201 }, 
    { " ----  A  ----      ", DoNothing,            0 }, 
    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Disable group CAN  ", Enable_gr_CAN_Ctrl_flag,     0 },
    { " Request packet 14  ", Req_packet_14,        1 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " Clear Faults       ", ClearFaults,          0 }, 
    { " ----  B  ----      ", DoNothing,            0 },  
    { " Hall OFF  2 no ack ", Send_Hall_Data,       0x2000 },    
    { " Hall ON   2 no ack ", Send_Hall_Data,       0x2005 },      
    { " ----  C  ----      ", DoNothing,            0 },              
    { " RESET  All 1137s  0", Reset_All_1137s,      0x0001 },
    { " RESET  All 1137s  2", Reset_All_1137s,      0x0021 },
    { " menu vector      20", Start_a_Menu,        20 },  
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " ----  D  ----      ", DoNothing,            0 },
    { " Get Hall unpk data ", Get_Srl_Hall_Station_Pkg, 0 },
    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Disable group CAN  ", Enable_gr_CAN_Ctrl_flag,     0 },    
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " Clear Faults       ", ClearFaults,          0 },    
    { " RESET  All 1137s  4", Reset_All_1137s,      0x0041 },
 
    { " Get Hall unpk data ", Get_Srl_Hall_Station_Pkg, 0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " Clear Faults       ", ClearFaults,          0 },       
    { " RESET  All 1137s  8", Reset_All_1137s,      0x0081 },
    { " RESET 1137 no LED  ", Reset_All_1137s,      0 },
    { " RESET  0           ", Send_Hall_Reset,      0 },
    { " RESET  2           ", Send_Hall_Reset,      2 },
    { " RESET  5           ", Send_Hall_Reset,      5 },    
    { " RESET  8           ", Send_Hall_Reset,      8 },
    { " RESET 10           ", Send_Hall_Reset,     10 },
    { " Hall OFF           ", Send_Hall_Data,       0 },    
    { " Hall ON            ", Send_Hall_Data,       5 },    
    { " Hall chan 1 on     ", Send_Hall_Data,       0x1004 },
    { " Hall chan 2 on     ", Send_Hall_Data,       0x1001 }, 
    { " Hall chan 1 on only", Send_Hall_Data,       0x0004 },
    { " Hall chan 2 on only", Send_Hall_Data,       0x0001 },       
    { " MESSAGE BOARD B    ", DisplayMsgBrd_B,      0 },
    { " Clear msg board B  ", Clear_All_Boards,     2 }, 
    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Disable group CAN  ", Enable_gr_CAN_Ctrl_flag,     0 },
    { " Clear msg board B  ", Clear_All_Boards,     2 }, // Reset_All_1137s
    { " menu vector      20", Start_a_Menu,        20 },  
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },   
};

struct a_menu Verify_Second_Half[] = { /// = 22
    { " Verify 2nd half  22", DoNothing,            0 },  //
    { " Hall OFF no ack    ", Send_Hall_Data,          0x2000 },
//    { " 1107 Set out 1 & 2 ", Set_or_Clear_1137_Input, 0xff01 },
    { " 1107 Clear out 1&2 ", Set_or_Clear_1137_Input, 0xff00 },
    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " --- Verify 0 ----  ", DoNothing,            0 },
//    { " Disable group CAN  ", Enable_gr_CAN_Ctrl_flag,     0 },
    { " 1107 Set out 1     ", Set_or_Clear_1137_Input, 0xf101 },
//    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " --- Verify 8 ----  ", DoNothing,            0 },  
    { " 1107 Clear out 1   ", Set_or_Clear_1137_Input, 0xf100 },
    { " 1107 Set out 2     ", Set_or_Clear_1137_Input, 0xf201 },
//    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " --- Verify 2 ----  ", DoNothing,            0 },       
    
    { " menu 21          21", Start_a_Menu,        21 },
    { " long  seq 1137/2 23", Start_a_Menu,        23 },  
    { " menu vector      20", Start_a_Menu,        20 },
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },     
};

struct a_menu Long_Second_Half[] = { /// = 23
    { " Modfy sq 2nd half23", DoNothing,            0 },  //
//    { " RESET  2           ", Send_Hall_Reset,      2 },
    { " RESET  All 1137s  2", Reset_All_1137s,      0x0021 },
    { " Hall OFF no ack    ", Send_Hall_Data,          0x2000 },
    { " 1107 Set CTo6      ", Set_or_Clear_1137_Input, 0x0101 },
    { " 1107 Clear CTo6    ", Set_or_Clear_1137_Input, 0x0100 },
//    { " 1107 Set CTo 6 - 11", Set_or_Clear_1137_Input, 0xff01 },
//    { " 1107 Clr CTo 6 - 11", Set_or_Clear_1137_Input, 0xff00 },
    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " --- Verify 0 ----  ", DoNothing,            0 },
//    { " Disable group CAN  ", Enable_gr_CAN_Ctrl_flag,     0 },
    { " 1107 Set CTo6      ", Set_or_Clear_1137_Input, 0x0101 },
//    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " --- Verify 8 ----  ", DoNothing,            0 },  
//    { " 1107 Clear CTo 6   ", Set_or_Clear_1137_Input, 0x0100 },
    { " 1107 Set CTo7      ", Set_or_Clear_1137_Input, 0x0201 },
    { " 1107 Clear CTo6    ", Set_or_Clear_1137_Input, 0x0100 },
//    { " Enable group CAN   ", Enable_gr_CAN_Ctrl_flag,     1 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " --- Verify 2  ---- ", DoNothing,            0 }, 
    { " 1107 Clr CTo 6 & 7 ", Set_or_Clear_1137_Input, 0x0300 },
    { " RESET  All 1137s  2", Reset_All_1137s,      0x0021 },
    { " Disable group CAN  ", Enable_gr_CAN_Ctrl_flag,     0 },  
    { " 1107 Clear CTo6    ", Set_or_Clear_1137_Input, 0x0100 },
    { " 1107 Clear CTo7    ", Set_or_Clear_1137_Input, 0x0200 },
    { " -- HALL STN Strt --", DoNothing,            0 },
    { " Hall Stn first half", One_Hall_Station_CAN,     0x1000 }, // first half only
    { " Hall Stn secnd half", One_Hall_Station_CAN,     0x2000 }, // second half only
    { " Hall Stn Full test1", One_Hall_Station_CAN,     0 },  // full test 
    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 },
    { " Clear msg board  C ", Clear_All_Boards,     4 }, 
    { " -- HALL STN End -- ", DoNothing,            0 },
    { " menu 21          21", Start_a_Menu,        21 },  
    { " short seq 1137/2 22", Start_a_Menu,        22 }, 
    { " menu vector      20", Start_a_Menu,        20 },
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " Disable group CAN  ", Enable_gr_CAN_Ctrl_flag,     0 },     
};

struct a_menu easy_start_menu[] = { /// = 24
    { " Easy start menu  24", DoNothing,            0 },  //
    { " Modfy sq 2nd half23", Start_a_Menu,        23 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " Clear Faults       ", ClearFaults,          0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },    
    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 },
    { " Clear msg board  C ", Clear_All_Boards,     4 },     
    { " 1 - Up (one 1137 1)", Qty_113x_bds,         1 }, 
    { " 1137 Hall Stn test1", One_Hall_Station_CAN,     0 }, 
    { " 1137 Hall Stn test2", One_Hall_Station_CAN,     1 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " 1137 Hall Stn test3", One_Hall_Station_CAN,     2 }, 
    { " 1139 Hall Lantern 0", HALL_Lantern_RS485,   0 },
    { " 1139 Hall Lantern 1", HALL_Lantern_RS485,   1 },
    { " 1139 Hall Lantern 2", HALL_Lantern_RS485,   2 },    
    { " Test GALX4-0047N   ", toggle_4_0047N,       0 },
    { " menu vector      20", Start_a_Menu,        20 },
    { " RESET MCU          ", WDT_Test,             0 },        
}; 

struct a_menu Diagnostics_1137_Sel_Position[] = { /// = 25
    { " Select Position  25", DoNothing,            0 },  //
    { " Position 1 (1137)  ", Set_1137_Diagnostic_Position,   0 }, // vectors to menu 26
    { " Position 2 (1137)  ", Set_1137_Diagnostic_Position,   1 },
    { " Position 3 (1137)  ", Set_1137_Diagnostic_Position,   2 },           
};

struct a_menu Diagnostics_1137[] = { /// = 26
    { " 1137 Diagnostics 26", DoNothing,      0 },  //
    { " 1137 Full Stn test ", Full_Station_CAN_Test,     0 },
    { " Set UI on          ", Set_UI_on,      0 }, 
    { " Clear UI off       ", Clear_UI_off,   0 },    
    { " Set DI on          ", Set_DI_on,      0 },
    { " Clear DI off       ", Clear_DI_off,   0 },
     
    { " Ack 0,2,5,8,10,11  ", Reset_All_1137s,             0x0021 },     
    { " Cmnd DO on         ", Cmnd_DO_on,     0 },
    { " Cmnd DO off        ", Cmnd_DO_off,    0 },
    { " Cmnd UO on         ", Cmnd_UO_on,     0 },
    { " Cmnd UO off        ", Cmnd_UO_off,    0 }, 
    { " Cmnd All on        ", Cmnd_All_on,    0 },
    { " Cmnd All off       ", Cmnd_All_off,   0 },
    
    { " Ack 0,2,5,8,10,11  ", Reset_All_1137s,             0x0021 },  
    { " View Msg Board A   ", DisplayMsgBrd_A,                  0 },
    { " View Msg Board B   ", DisplayMsgBrd_B,                  0 },
    { " View Msg Board C   ", DisplayMsgBrd_C,                  0 },
    { " Clear all msg bds  ", Clear_All_Boards,                 7 },
    { "      HELP          ", Start_a_Menu,        19 },
//    { " Reset 100ms timer  ", Clear_100ms_timer,t_Generic_Delay_4 },     
} ;
    
struct a_menu Diagnostics_1139[] = { // = 27
    { " 1139 Diagnostics 27", DoNothing,      0 },  //
    { " 1139 Hall Lantern 1", HALL_Lantern_RS485,   0 },
    { " 1139 Hall Lantern 2", HALL_Lantern_RS485,   1 },
    { " 1139 Hall Lantern 3", HALL_Lantern_RS485,   2 },
    { " Select Lantern Up  ", Send_Hall_Lantern_Floor,   0x40 },
    { " Select Lantern Down", Send_Hall_Lantern_Floor,   0x80 },
    { " Select Lantern Both", Send_Hall_Lantern_Floor,   0xc0 },
    { " Select Lantern None", Send_Hall_Lantern_Floor,   0 },    
  
    { " View Msg Board A   ", DisplayMsgBrd_A,                  0 },
    { " View Msg Board B   ", DisplayMsgBrd_B,                  0 },
    { " View Msg Board C   ", DisplayMsgBrd_C,                  0 },
    { " Clear all msg bds  ", Clear_All_Boards,                 7 },
    { "      HELP          ", Start_a_Menu,        19 },
//    { " Reset 100ms timer  ", Clear_100ms_timer,t_Generic_Delay_4 },    
};

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
         { Diagnostic_1134_CTB_Menu, (sizeof(Diagnostic_1134_CTB_Menu) / sizeof(Diagnostic_1134_CTB_Menu[0])) - 1},
         { Main_1134_CTB_Menu, (sizeof(Main_1134_CTB_Menu) / sizeof(Main_1134_CTB_Menu[0])) - 1},
         { Bds_Menu, (sizeof(Bds_Menu) / sizeof(Bds_Menu[0])) - 1},	
         { Validate_Bds_Menu, (sizeof(Validate_Bds_Menu) / sizeof(Validate_Bds_Menu[0])) - 1},
         { Hall_Bds_Menu, (sizeof(Hall_Bds_Menu) / sizeof(Hall_Bds_Menu[0])) - 1},
         { Main_Hall_Bds_Menu, (sizeof(Main_Hall_Bds_Menu) / sizeof(Main_Hall_Bds_Menu[0])) - 1},											//10
         { Select_1137_Qty_Menu, (sizeof(Select_1137_Qty_Menu) / sizeof(Select_1137_Qty_Menu[0])) - 1},  
         { Select_1139_Qty_Menu, (sizeof(Select_1139_Qty_Menu) / sizeof(Select_1139_Qty_Menu[0])) - 1},   
         { min_documentation, (sizeof(min_documentation) / sizeof(min_documentation[0])) - 1}, 
         { menu_vector, (sizeof(menu_vector) / sizeof(menu_vector[0])) - 1}, 
         { Locate_Reset_Problem, (sizeof(Locate_Reset_Problem) / sizeof(Locate_Reset_Problem[0])) - 1}, 
         { Verify_Second_Half, (sizeof(Verify_Second_Half) / sizeof(Verify_Second_Half[0])) - 1},  
         { Long_Second_Half, (sizeof(Long_Second_Half) / sizeof(Long_Second_Half[0])) - 1},  
         { easy_start_menu, (sizeof(easy_start_menu) / sizeof(easy_start_menu[0])) - 1},
         { Diagnostics_1137_Sel_Position, (sizeof(Diagnostics_1137_Sel_Position) / sizeof(Diagnostics_1137_Sel_Position[0])) - 1},
         { Diagnostics_1137, (sizeof(Diagnostics_1137) / sizeof(Diagnostics_1137[0])) - 1},
         { Diagnostics_1139, (sizeof(Diagnostics_1139) / sizeof(Diagnostics_1139[0])) - 1},
    };			



int16 Start_a_Menu(int16 WhichMenu)
{
    if( PushMenuPtrArray(MenuPtr) ) {
        MenuPtr = The_Menus_in_System[WhichMenu].MenuPtr; // 
        TheMenuLength = The_Menus_in_System[WhichMenu].LengthOfMenu;
    //    MenuPtrArrayIndex = 0;
        LCD_Pointer = 1;
        LCD_Upd_Dpy = 1; // force update of display
    }
    return FINISHED_WITH_SERVICE;
}

int16 Start_Individual_Tests(void) // passes control from a parent menu to this specific child menu
{
    if( PushMenuPtrArray(MenuPtr) ) {
        MenuPtr = IndividualTests_Menu_List;
        TheMenuLength = (sizeof(IndividualTests_Menu_List) / sizeof(IndividualTests_Menu_List[0])) - 1;
        LCD_Pointer = 1;
        LCD_Upd_Dpy = 1; // force update of display
    }
    return FINISHED_WITH_SERVICE;
}

int16 PushMenuPtrArray(void *MenuPtr)  // saves the current menu ifo on a stack
{
    if(MenuPtr == NULL) return 1; // indicate success even though not.
      // This is necessary, since the start menue may not be the
      // main menu, and thus we should not push a NULL ptr to the stack
      // prior to starting a child menu from a parent menu.
    if(MenuPtrArrayIndex < MAX_MENU_DEPTH) {
        The_menu_stack[MenuPtrArrayIndex].MenuPtrOnStack = MenuPtr;
        The_menu_stack[MenuPtrArrayIndex].LCD_PointerOnStack = LCD_Pointer;
        The_menu_stack[MenuPtrArrayIndex].LengthOfMenuOnStack = TheMenuLength;
        MenuPtrArrayIndex++;
        SW_Vers_disp();
        return SUCCESS; // = 1
    } else return FAILED; // = 0
}

int16 PopMenuPtrArray(void)
{
    if(MenuPtrArrayIndex > 0) {
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
    } else {
        return FAILED;
    }
}


int16 Get_Num_of_Menus(void)
{
    return ( sizeof(The_Menus_in_System) / sizeof(The_Menus_in_System[0])  );
}

