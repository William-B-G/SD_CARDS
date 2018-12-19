// Archived_Menus.c


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

*/

//ZYM 12-18-2017	     
//-    { " TEST JIG COMM      ", TEST_JIG_COMM,          0 },
//-    { " Machine Room CAN L ", TEST_MLCAN,             16},        
//-    { " NTS CAN            ", TEST_NTSCAN,            0 },
//-	   { " v120AC Power Check ", v120AC_Power_Check,     0 },
//-    { " SW TEST            ", SW_TEST,                0 }, 
//-    { " SEB IO TEST        ", SEB_IO_TEST,            0 }, 
//-    { " FPGA1 IO TEST      ", FPGA1_IO_TEST,          0 }, 
//-    { " FPGA2 IO TEST      ", FPGA2_IO_TEST,          0 }, 


