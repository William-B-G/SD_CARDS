// Menus.c

#include "global.h"
#include "SD_TEST.h"
#include "RTC_TEST.h"
#include "GALX_1132_TEST.h"
#include "MRCAN_TEST.h"
#include "SLCAN_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1137_1139.h"
#include "Menus.h"

/*struct a_menu { // each menu consists of the text to
    // display, a function pointer, and the param
    // value to send to the function. The function
    // is executed when the user clicks the 'enter' button.
    char MenuItem[21]; // text to display.
    void *Service;  // function pointer
    int16 param;    // the parameter to send to the function.
};
*/


/*
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 }, 
    { " RS-485             ", The_RS_485_test,        0 },  //   The_RS_485_test(x);// Hl_Tx_to_Grp_Rx
    { " RS-485             ", Hl_Tx_to_Grp_Rx,        0 },  //   The_RS_485_test(x);// Hl_Tx_to_Grp_Rx
    { " Faults             ", DisplayMsgBrd_A,        0 },
    { " ver_rev CPLD1      ", Read_FPGA_ver_rev,      0 }, 
    { " Faults             ", DisplayMsgBrd_A,        0 },
    { " ver_rev CPLD2      ", Read_FPGA_ver_rev,      1 },
    { " Faults             ", DisplayMsgBrd_A,        0 },
    { " ver_rev NTS        ", Read_FPGA_ver_rev,      2 },     
    { " RESET MCU NOW      ", WDT_Test,               0 },
    { " Check ISR timing   ", ISR_test,               0 },
    { " Hall Com - one shot", Hal_com_Tx_test,        0 },
    { " Show the Grp Rx Buf", GrpRxPacket,            0 },       // GrpRxPacket            
    { " Show the Hal Rx Buf", ShowMe_sx_recbuf,       0 },
    { " Show tempBuff      ", ShowMe_tempBuff,        0 },
    { " Clear Faults       ", ClearFaults,            0 },
    { " Display RS-485 BAUD", Display_RS485_Baud,     0 },
    { " Clear Faults       ", ClearFaults,            0 },
    { " Query menu variabls", QueryMenuVariables,     0 },
    { " Fill the 2 Rx Buffs", Call_temp_SW,           0 }, 
    { " Toggle RS485 Ctrl  ", Toggle_RS_485_Ctrl,     0 },   
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
    { " RESET MCU NOW      ", WDT_Test,               0 },    
    { " Machine Room CAN   ", TEST_MRCAN,             0 },
    { " Selector CAN       ", TEST_SLCAN,             0 },    
    { " RS-485             ", The_RS_485_test,        0 },
    { " Toggle RS485 Ctrl  ", Toggle_RS_485_Ctrl,     0 },    
    { " Show the Hal Rx Buf", ShowMeRxBuf,            0 },
    { " MESSAGE BOARD A    ", DisplayMsgBrd_A,        0 },
    { " MESSAGE BOARD B    ", DisplayMsgBrd_B,        0 },
    { " SOFTWARE VERSION   ", Display_SW_Version,     0 },
    { " Car Top CAN        ", TEST_CTCAN,             0 },
    { " Group CAN          ", TEST_GRCAN,             0 },
    { " Clear Faults       ", ClearFaults,            0 },
    { " Query menu variabls", QueryMenuVariables,     0 },
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
    { " Hall Com - one shot", Hal_com_Tx_test,        0 },
    { " Hall Com - ContinUs", Hal_com_Tx_test_Continuous, 0 },
    { " MRAM               ", Test_MRAM,              0 },
    { " RS-232             ", The_RS_232_test,        0 },
    { " WATCH DOG          ", WDT_Test,              30 },
    { " SD CARD            ", TEST_SD,                0 },
    { " WIFI               ", DoNothing,              0 },
    { " REAL TIME CLOCK    ", TEST_RTC,               0 },
    { " PUSH BUTTONS       ", PushButtons_Test,       0 },
    { " 4-LINE DISPLAY     ", Display_Test,           0 },
    { " ADJUST LCD DISPLAY ", DoNothing,              0 },
    { " GAL1132 IO BOARD   ", DoNothing,              0 },           
    { " Enable CAN ctrl    ", CAN_Ctrl_EN_bit,        1 },//Enable_CAN_Ctrl
    { " Disable CAN ctrl   ", CAN_Ctrl_EN_bit,        0 },//Enable_CAN_Ctrl
    { " Query all com ctrl ", DoNothing,              0 },
    { " Query all com ctrl ", Query_Ctrl_bits,        0 },
    { " Item 05 on C       ", DoNothing,              0 },
    { " ver_rev CPLD1      ", Read_FPGA_ver_rev,      0 },     
    { " ver_rev CPLD2      ", Read_FPGA_ver_rev,      1 },
    { " Faults             ", DisplayMsgBrd_A,        0 },
    { " ver_rev NTS        ", Read_FPGA_ver_rev,      2 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
    { " Software Utilities ", Display_SW_Version,     0 },    
    { " Display RS-xxx BAUD", Display_RS485_Baud,     0 },               
    { " Faults             ", DisplayMsgBrd_A,        0 },
    { " ver_rev CPLD2      ", Read_FPGA_ver_rev,      1 },   
    { " Faults             ", DisplayMsgBrd_A,        0 }, 
    { " WIFI enable        ", WiFi_Test,              0 },
    { " WIFI disable       ", WiFi_Test,             25 },   
    { " RESET MCU NOW      ", WDT_Test,               0 },    
    { " Toggle ISR timing  ", ISR_test,               0 },    
    { " RS-485             ", The_RS_485_test,        0 },
    { " Hall Com - one shot", Hal_com_Tx_test,        0 },
    { " Show the Grp Rx Buf", GrpRxPacket,            0 },     
    { " Once frm Grp Com   ", The_RS_485_test,        3 }, 
    { " Send one character ", Special_test_for_Johnny,0 },        
    { " MESSAGE BOARD B    ", DisplayMsgBrd_B,        0 }, 
    { " Re-init GrCom 1    ", ReinitGrCom1,           0 },       
    { " Test 1132 MCU Board", DoNothing,              0 },
    { " Test 1121 IO Board ", DoNothing,              0 },
    { " Selector CAN       ", TEST_SLCAN,             0 },
    { " FPGA CPLD1         ", Read_FPGA_ver_rev,      0 },
    { " FPGA CPLD1         ", Read_FPGA_ver_rev,      1 },
    { " NTS                ", Read_FPGA_ver_rev,      2 }, 
    { " MESSAGE BOARD B    ", DisplayMsgBrd_B,        0 },   
    { " Development        ", Start_MyMenuList,       0 },
    { " Select Baud for COM", Start_SubMenu_B,        0 },
    { " Select Baud for 485", Start_SubMenu_B,        0 },
    { " Sub Menu B         ", Start_SubMenu_B,         0},  
    { " Sub Menu C         ", Start_SubMenu_C,         0},
    { " Utilities          ", Start_SubMenu_C,        0 },          
    { " Diagnostics        ", Start_Tstr_Diagnostics, 0 },
    { " DIAGNOSTICS        ", Start_Tstr_Diagnostics, 0 },
    { " Individual Tests   ", Start_Individual_Tests, 0 },           

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

struct a_menu Main_MenuList[] = { /// = 0
    { "       GALaxy       ", DoNothing,              0 },  // WhichMenu = 0
    { " Test & Stop On Err ", FunctionalTest,       0x01},  //  StopOnFail = TRUE(1 Fail then Stop) 	//ADDing "0x100" is for GALX1121 FUNCTIONAL TEST MENU
    { " Test - Do Not Stop ", FunctionalTest,       0x03},  //  StopOnFail = FALSE(3 Fail dont care, keep going next test)
    { " Test @ Failed Funct", FunctionalTest,       0x02},  //  Only test functions that have failed, and StopOnFail = FALSE
    { " Individual Tests   ", Start_Individual_Tests, 0 },
    { " Faults             ", DisplayMsgBrd_A,        0 },    
    { " Clear Faults       ", ClearFaults,            0 }, 
    { " SOFTWARE VERSION   ", Display_SW_Version,     0 },
    { " Diagnostics        ", Start_a_Menu,           5 },  // menu 5         .
    { " Utilities          ", Start_a_Menu,           3 },	// menu 3
//    { " Main I/O BD TEST   ", Start_a_Menu,          10 },   	
	{ " MESSAGE BOARD A    ", DisplayMsgBrd_A,        0 },
    { " MESSAGE BOARD B    ", DisplayMsgBrd_B,        0 },
    { " MESSAGE BOARD C    ", DisplayMsgBrd_C,        0 },  
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },    
};

struct a_menu IndividualTests_Menu_List[] = { /// WhichMenu  = 1
    { " Individual Tests   ", DoNothing,              0 },
    { " Push Buttons       ", PushButtons_Test,       0 },
    { " 4-Line Display     ", Display_Test,           0 },
    { " RS-232             ", The_RS_232_test,        0 },  
    { " WIFI TEST          ", WiFi_Test,              0 },
//-    { " WIFI Exit Comand MD", WiFi_Test,             25 },
    { " Group CAN          ", TEST_GRCAN,             16},
    { " Selector CAN       ", TEST_SLCAN,             16},
    { " Car Top CAN        ", TEST_CTCAN,             16},
    { " Machine Room CAN R ", TEST_MRCAN,             16},        
    { " RS-485             ", The_RS_485_test,        0 },
    { " SD Card            ", TEST_SD,                0 },          
    { " Real Time Clock    ", TEST_RTC,               0 },
    { " MRAM Chip 1        ", Test_MRAM,              1 },
    { " MRAM Chip 2        ", Test_MRAM,              2 },    
    { " FPGA1 SPI          ", Read_FPGA_ver_rev,      0 },
    { " FPGA2 SPI          ", Read_FPGA_ver_rev,      1 },
    { " NTS SPI            ", Read_FPGA_ver_rev,      2 },    

    
     
//ZYM 12-18-2017	     
//-    { " TEST JIG COMM      ", TEST_JIG_COMM,          0 },
//-    { " Machine Room CAN L ", TEST_MLCAN,             16},        
//-    { " NTS CAN            ", TEST_NTSCAN,            0 },
//-	   { " v120AC Power Check ", v120AC_Power_Check,     0 },
//-    { " SW TEST            ", SW_TEST,                0 }, 
//-    { " SEB IO TEST        ", SEB_IO_TEST,            0 }, 
//-    { " FPGA1 IO TEST      ", FPGA1_IO_TEST,          0 }, 
//-    { " FPGA2 IO TEST      ", FPGA2_IO_TEST,          0 }, 

    
    { " Watch Dog Timer tst", WDT_Test,               0 }, 
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },   
//- { " SAMPLE IO TEST     ", SAMPLE_IO_TEST,         0 },   
};


struct a_menu SubMenu_B_List[] = { /// WhichMenu  = 2
    { "      Sub Menu B    ", DoNothing,              0 },     
    { " Display RS-485 BAUD", Display_RS485_Baud,     0 },
    { " RSxxx Baud = 2.4k  ", Set_RS485_Baud,      2832 },
    { " RSxxx Baud = 4.8k  ", Set_RS485_Baud,      1416 },
    { " RSxxx Baud = 9.6k  ", Set_RS485_Baud,       708 },
    { " RSxxx Baud = 19.2k ", Set_RS485_Baud,       354 },
    { " RSxxx Baud = 38.4k ", Set_RS485_Baud,       177 },
    { " RSxxx Baud = 57.6k ", Set_RS485_Baud,       118 },
    { " RSxxx Baud = 115.2k", Set_RS485_Baud,        59 },
    { " Faults             ", DisplayMsgBrd_A,        0 }, 
    { " Clear Faults       ", ClearFaults,            0 },
    { " Query menu variabls", QueryMenuVariables,     0 },  
    { " Utilities          ", Start_a_Menu,           3 },  // menu 3  
};

struct a_menu SubMenu_C_List[] = { ///  WhichMenu = 3
    { "      Utilities     ", DoNothing,              0 },  //same as SubMenu_C  
    { " View RTC           ", ViewRTC      ,          0 },    
    { " MESSAGE BOARD A    ", DisplayMsgBrd_A,        0 },
    { " MESSAGE BOARD B    ", DisplayMsgBrd_B,        0 },
//-    { " Disable RS485 com  ", EnableDisableRS485_com_Ctrl, 0 },
//-    { " Enable  RS485 com  ", EnableDisableRS485_com_Ctrl, 1 },
//-    { " Toggle RS485 Ctrl  ", Toggle_RS_485_Ctrl,     0 },
//-    { " Enable CAN ctrl    ", CAN_Ctrl_EN_bit,        1 },//Enable_CAN_Ctrl
//-    { " Disable CAN ctrl   ", CAN_Ctrl_EN_bit,        0 },//Disable_CAN_Ctrl
//-    { " Query all com ctrl ", Query_Ctrl_bits,        0 },
//-    { " Enable all com ctrl", All_Ctrl_EN_bit,        1 },
//-    { " Disable all com ctl", All_Ctrl_EN_bit,        0 },
//-    { " Item 05 on C       ", DoNothing,              0 },
//-    { " Faults             ", DisplayMsgBrd_A,        0 },
//-    { " Clear Faults       ", ClearFaults,            0 },
    { " Query menu variabls", QueryMenuVariables,     0 },
    { " Development        ", Start_a_Menu,           7 }, // menu 7
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
};

struct a_menu groupCom_Menu_List[] = { ///  WhichMenu = 4	
    { " Group Com Study    ", DoNothing,              0 },
    { " Set/Review Baud    ", Start_a_Menu,           2 },	// menu 2
    { " HalCom '[' one shot", Hal_com_Tx_test,       25 },
    { " Show the Grp Rx Buf", GrpRxPacket,            0 },
    { " HalCom 'A' one shot", Hal_com_Tx_test,       22 },
    { " HalCom 'B' one shot", Hal_com_Tx_test,       23 },
    { " Sliding 'A' one sht", Hal_com_Tx_test,       22 },
    { " Clear Msg Board    ", ClearFaults,            0 }, 
    { " MESSAGE BOARD A    ", DisplayMsgBrd_A,        0 },   
    { " Main Menu          ", Start_a_Menu,           0 },	// menu 0
    { " Development        ", Start_a_Menu,           7 },	// menu 7 
    { " WATCH DOG          ", WDT_Test,               0 },         
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
};

struct a_menu Diagnostic_Menu_List[] = {  ///  WhichMenu = 5
    { "      Diagnostics   ", DoNothing,              0 },
//-    { " Watch Dog Timer tst", WDT_Test,               0 },
    { " Hall Com - one shot", Hal_com_Tx_test,        0 },
    { " Show the Grp Rx Buf", GrpRxPacket,            0 },    
//    { " RS-485 GrRx &HlTx  ", GrpRxPacket,            0 },
//-    { " MESSAGE BOARD A    ", DisplayMsgBrd_A,        0 },
//-    { " MESSAGE BOARD B    ", DisplayMsgBrd_B,        0 },
    { " Show the Hal Rx Buf", ShowMe_sx_recbuf,       0 },
    { " Show tempBuff      ", ShowMe_tempBuff,        0 },
//-    { " Faults             ", DisplayMsgBrd_A,        0 },
//-    { " Group CAN          ", Group_CAN_Diagnostics,  0 },
//-    { " Car Top CAN        ", CT_CAN_Diagnostics,     0 },
//-    { " Machine Room CAN   ", MR_CAN_Diagnostics,     0 },
    { " Main Menu          ", Start_a_Menu,           0 },	// menu 0
    { " FPGA Diagnostics   ", Start_a_Menu,           6 },	// menu 6
//-    { " Toggle ISR timing  ", ISR_test,               0 },
    { " Development        ", Start_a_Menu,           7 },	// menu 7
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
};

// Start_FPGA_Diag_Menu_List
struct a_menu FPGA_Diag_Menu_List[] = {  ///  WhichMenu = 6
    { " FPGA Diagnostics   ", DoNothing,              0 },
    { " FPGA1 SPI unpack   ", Read_FPGA_ver_rev,      0 },
    { " View FPGA1 unpk inf", DisplayMsgBrd_A,        0 },
    { " FPGA2 SPI unpack   ", Read_FPGA_ver_rev,      1 }, 
    { " View FPGA2 unpk inf", DisplayMsgBrd_A,        0 },
    { " Set individual out ", Start_a_Menu,           9 },	// menu 9
//-    { " All I/O on         ", Set_All_outputs,        1 },
//-    { " All I/O off        ", Set_All_outputs,        0 },   
//-    { " Enable_iospi_Ctrl  ", Set_iospi_Ctrl_flag,    1 },
//-    { " Disable_iospi_Ctrl ", Set_iospi_Ctrl_flag,    0 },
//-    { " Change FPGA Addr's ", Start_a_Menu,           8 }, 	// menu 8
//-    { " Clear Msg Board    ", ClearFaults,            0 },   
    { " Main Menu          ", Start_a_Menu,           0 },	// menu 0     
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
};

// sx_recbuf[SE1COM][sx_rbf[SE1COM]]
// ShowMeRxBuf
struct a_menu MyMenuList[] = {  ///  WhichMenu = 7   // abcptr = MyMenuList;
    { "     Development    ", DoNothing,              0 },
    { " display consB file ", Display_consB_file,     0 },  // Display_consB_file  
    { " MESSAGE BOARD A    ", DisplayMsgBrd_A,        0 },       
//-    { " Machine Room CAN   ", TEST_MRCAN,             0 },
//-    { " Selector CAN       ", TEST_SLCAN,             0 },    
//-    { " RS-485             ", The_RS_485_test,        0 },        
    { " Show the Hal Rx Buf", ShowMeRxBuf,            0 },    
    { " MESSAGE BOARD B    ", DisplayMsgBrd_B,        0 },      
//-    { " Car Top CAN        ", TEST_CTCAN,             0 },
//-    { " Group CAN          ", TEST_GRCAN,             0 },
    { " Clear Faults       ", ClearFaults,            0 },
    { " Query menu variabls", QueryMenuVariables,     0 },
//-    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
    { " Hall Com - one shot", Hal_com_Tx_test,        0 },
    { " Show the Grp Rx Buf", GrpRxPacket,            0 },
    { " Hall Com - ContinUs", Hal_com_Tx_test_Continuous, 0 },
//-    { " MRAM               ", Test_MRAM,              1 },
//-    { " RS-232             ", The_RS_232_test,        0 },
//-    { " WIFI               ", DoNothing,              0 },
//-    { " REAL TIME CLOCK    ", TEST_RTC,               0 },
//-    { " PUSH BUTTONS       ", PushButtons_Test,       0 },
//-    { " 4-LINE DISPLAY     ", Display_Test,           0 },
//-    { " ADJUST LCD DISPLAY ", DoNothing,              0 },
//-    { " GAL1132 IO BOARD   ", DoNothing,              0 },
    { " GENERIC TEST       ", Generic_Test,           0 },
//-    { " MSG BD A SWITCHES  ", DoNothing,              0 },
//-    { " MSG BD B SWITCHES  ", DoNothing,              0 },
    { " 1106 I/O BD        ", Start_a_Menu,          10 },	// menu 10
    { " FORCE TESTS TO FAIL", SetClearForceToFail,    1 },
    { " Clear the FORCE flg", SetClearForceToFail,    0 },
//-    { " Sub Menu B         ", Start_a_Menu,           2 },	// menu 2
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },
};

struct a_menu FPGA_SetAddrs[] = {  ///  WhichMenu = 8
    { " FPGA Set Addresses ", DoNothing,              0 }, 
    { " CPLD1 Default      ", Set_CPLD1_Addr,         0 },
    { " CPLD1 0x9356       ", Set_CPLD1_Addr,         1 },
    { " CPLD1 0x9357       ", Set_CPLD1_Addr,         2 },
    { " CPLD1 0x9358       ", Set_CPLD1_Addr,         3 },
    { " CPLD1 0x9359       ", Set_CPLD1_Addr,         4 },
    { " CPLD1 0x935A       ", Set_CPLD1_Addr,         5 },
    { " Show FPGA Addr's   ", DisplayMsgBrd_A,        0 },
    { " CPLD2 Default      ", Set_CPLD2_Addr,         0 },
    { " CPLD2 0x612B       ", Set_CPLD2_Addr,         1 },
    { " CPLD2 0x612C       ", Set_CPLD2_Addr,         2 },
    { " CPLD2 0x612D       ", Set_CPLD2_Addr,         3 },
    { " CPLD2 0x612E       ", Set_CPLD2_Addr,         4 },
    { " CPLD2 0x612F       ", Set_CPLD2_Addr,         5 },
    { " Main Menu          ", Start_a_Menu,           0 },	// menu 0
    { " RESET MCU          ", WDT_Test,               0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,          0 },      
};

struct a_menu Set_main_IO[] = {  ///  WhichMenu = 9
    { " Set main I/O       ", DoNothing,                0 },
    { " SUF on             ", Set_specific_output, 0xf000 },
    { " SUF off            ", Set_specific_output,      0 },
    { " SU on              ", Set_specific_output, 0x0f001}, 
    { " SU off             ", Set_specific_output, 0x0001 },
    { " SDF on             ", Set_specific_output, 0xf002 },
    { " SDF off            ", Set_specific_output,      2 },
    { " SD on              ", Set_specific_output, 0xf003 }, 
    { " SD off             ", Set_specific_output,      3 },   
    { " STE on             ", Set_specific_output, 0xf004 },
    { " STE off            ", Set_specific_output,      4 },
    { " MC1 on             ", Set_specific_output, 0xf005 }, 
    { " MC1 off            ", Set_specific_output,      5 },
    { " MC on              ", Set_specific_output, 0xf006 },
    { " MC off             ", Set_specific_output,      6 },
    { " RUN on             ", Set_specific_output, 0xf007 },
    { " RUN off            ", Set_specific_output,      7 },    
// ZYM 12-5-2017
	{ " UT on              ", Set_specific_output, 0xf008 },
    { " UT off             ", Set_specific_output,      8 },
    { " UN on              ", Set_specific_output, 0xf009 },
    { " UN off             ", Set_specific_output,      9 },
    { " DT on              ", Set_specific_output, 0xf00A },
    { " DT off             ", Set_specific_output,      10},
    { " DN on              ", Set_specific_output, 0xf00B },
    { " DN off             ", Set_specific_output,      11},
        
    { " Main Menu          ", Start_a_Menu,             0 },	// menu 0       
    { " RESET MCU          ", WDT_Test,                 0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,            0 },
};
// CheckPB
// Set_out1_off
struct a_menu Set_1106AN[] = {  ///  WhichMenu = 10
    { "     1106 I/O BD    ", DoNothing,                0 },
//-    { " Control 1106AD bd 3", Output_on_1106,           3 },
//-    { " Control 1106AD bd 4", Output_on_1106,           4 },
//-    { " All outputs OFF   3", All_On_Off,          0x0003 },
//-    { " All outputs ON bd 3", All_On_Off,          0x1003 },  
//-    { " All outputs OFF   4", All_On_Off,          0x0004 },
//-    { " All outputs ON bd 4", All_On_Off,          0x1004 },
//-    { " Run Power-Up Mode  ", JumpToPowerUp,            0 },      
//-    { " Read 1106 I/O BD 3 ", Read_1106,                3 },
//-    { " Read 1106 I/O BD 4 ", Read_1106,                4 },
    { " MESSAGE BOARD A    ", DisplayMsgBrd_A,          0 },
    { " Clear Faults       ", ClearFaults,              0 },
//-   { " Output out1 on     ", CheckPB,                  0 },
//-   { " Output out1 OFF    ", Set_out1_off,             0 },
//-    { " Run Power-Up Mode  ", JumpToPowerUp,            0 },
    { " All I/O on         ", Set_All_outputs,          1 },
    { " All I/O off        ", Set_All_outputs,          0 },
    { " Set individual out ", Start_a_Menu,             9 },	// menu 9
//-    { " Enable_iospi_Ctrl  ", Set_iospi_Ctrl_flag,      1 },
//-    { " Disable_iospi_Ctrl ", Set_iospi_Ctrl_flag,      0 },      
    { " Main Menu          ", Start_a_Menu,             0 }, 	// menu 0      
    { " Run Power-Up Mode  ", JumpToPowerUp,            0 },
};

struct a_menu Menu_Start_Pt[] = { /// = 11
    { " Starting Point   11", DoNothing,            0 },  //
    { " Test OpM buff Add 1", Test_OpM_bufferAdd,   1 },
    { " Test OpM buff Add 2", Test_OpM_bufferAdd,   2 },
    { " Test OpM buff Add 4", Test_OpM_bufferAdd,   4 },
    { " Test OpM buff Add 9", Test_OpM_bufferAdd,   9 }, 
    { " Test OpM buff Add23", Test_OpM_bufferAdd,  23 },
    { " Trim spaces   Add23", Test_OpM_bufferAdd, 0x8000 + 23 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },   
    { " Demo Operator Msg 0", Demo_OpM,     0x1000 + 0},
    { " Demo Operator Msg 1", Demo_OpM,     0x1000 + 1},
    { " Demo Operator Msg 2", Demo_OpM,     0x5000 + 2},
    { " Demo Operator Msg 3", Demo_OpM,     0x1000 + 3}, 
    { " Message Board A    ", DisplayMsgBrd_A,      0 },
    { " Main Menu         0", Start_a_Menu,         0 },
    { " ZYM Diagnostics  12", Start_a_Menu,        12 },        
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },     
    { " RESET MCU          ", WDT_Test,             0 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
};


struct a_menu ZYM_Diag_Menu[] = { /// = 12
    { " ZYM Diagnostics  12", DoNothing,            0 },  //
    { " DLB                ", ZYM_Diagnostic,     kDLB},
    { " DLM                ", ZYM_Diagnostic,     kDLM},
    { " DLT                ", ZYM_Diagnostic,     kDLT},
    { " RLM                ", ZYM_Diagnostic,     kRLM},
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 }, 
    { " Faults in A        ", DisplayMsgBrd_A,      0 },
    { " Faults in B        ", DisplayMsgBrd_B,      0 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },      
    { " Main Menu         0", Start_a_Menu,         0 },
    { " Starting Point   11", Start_a_Menu,        11 },    
    { " RESET MCU          ", WDT_Test,             0 },    
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
         { Main_MenuList, (sizeof(Main_MenuList) / sizeof(Main_MenuList[0])) - 1},										//0
         { IndividualTests_Menu_List, (sizeof(IndividualTests_Menu_List) / sizeof(IndividualTests_Menu_List[0])) - 1},	//1
         { SubMenu_B_List, (sizeof(SubMenu_B_List) / sizeof(SubMenu_B_List[0])) - 1},									//2
         { SubMenu_C_List, (sizeof(SubMenu_C_List) / sizeof(SubMenu_C_List[0])) - 1},  									//3
         { groupCom_Menu_List, (sizeof(groupCom_Menu_List) / sizeof(groupCom_Menu_List[0])) - 1},						//4
         { Diagnostic_Menu_List, (sizeof(Diagnostic_Menu_List) / sizeof(Diagnostic_Menu_List[0])) - 1},					//5
         { FPGA_Diag_Menu_List, (sizeof(FPGA_Diag_Menu_List) / sizeof(FPGA_Diag_Menu_List[0])) - 1},					//6
         { MyMenuList, (sizeof(MyMenuList) / sizeof(MyMenuList[0])) - 1},												//7
         { FPGA_SetAddrs, (sizeof(FPGA_SetAddrs) / sizeof(FPGA_SetAddrs[0])) - 1},										//8
         { Set_main_IO, (sizeof(Set_main_IO) / sizeof(Set_main_IO[0])) - 1},											//9
         { Set_1106AN, (sizeof(Set_1106AN) / sizeof(Set_1106AN[0])) - 1},
         { Menu_Start_Pt, (sizeof(Menu_Start_Pt) / sizeof(Menu_Start_Pt[0])) - 1},
         { ZYM_Diag_Menu, (sizeof(ZYM_Diag_Menu) / sizeof(ZYM_Diag_Menu[0])) - 1},
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

