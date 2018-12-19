// MenuManager.c

#include "global.h"
#include "SD_TEST.h"
#include "RTC_TEST.h"
#include "GALX_1132_TEST.h"
#include "MRCAN_TEST.h"
#include "SLCAN_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1137_1139.h"


#define MAX_MENU_DEPTH 23
#define Last_Main_Menu 10

#define NORMAL 0
#define POP_MENU_PTR 1
#define LED_TOGGLE_DELAY 10 // 500


extern int16 Check_for_Mode_Button(void);
extern int16 Check_for_Up_Button(void);
extern int16 Check_for_Down_Button(void);
extern int16 Check_for_Enter_Button(void);
void ClearTheButtons(void);
int16 Clear_All_Boards(int16 Boards);

void GALX_1132_TEST(void);
void clr_lcd_dply(void);
void fail_testagain(void);
void MenuManager(void);
void Push_Menu_LIFO(int16 New_LCD_Menu);
int16 Pop_Menu_LIFO(void);
int16 ServiceTheButtons(void);
int16 Display_SW_Version(int16 New_LCD_Menu);
int16 Repaint_Retest_Display(void);
int16 DoNothing(int16 New_LCD_Menu);
int16 (*foo)(int16) = Display_Test;
int16 (*Service_Request)(int16); // function pointer
//taking one int16 param and
// returning an int16

int16 PushMenuPtrArray(void *MenuPtr);
int16 PopMenuPtrArray(void);
int16 ClearFaults(int16 dummy);
int16 QueryMenuVariables(int16 dummy);
int16 Start_SubMenu_B(void);
int16 Start_SubMenu_C(void);

int16 TESTCASE_1132=0;
int16 Menu_LIFO_Index = 0;

int16 LCD_Menu             = 0;
static int16 New_LCD_Menu  = 0;
static int16 DoNothingCase = 0;
int16 LCD_Pointer          = 0;
int16 LCD_Upd_Dpy          = 0;
int16 LCD_Init             = 0;
int16 LCD_Sub_Init         = 0;
int16 line_ptr             = 0;
int16 start_ptr            = 0;
int16 i_index              = 0;
int16 j_index              = 0;
int16 k_index              = 0;
static int16 parameter     = 0;

int16 Retest_Delay = 0;
char Saved_Lines[4][21];
int16 Line_4_Blank = 1; // 1 = line 4 is a blank line
int16 (*ServiceProvider)(int16);

#pragma section const_type

struct a_menu *MainMenuPtr;
struct a_menu *MyMenuPtr;
struct a_menu *MenuPtr = NULL;

struct menu_stack {
    // This get pushed onto the menu stack.
    // Handles sub-menus up to 20 deep.
    void *MenuPtrOnStack;
    int16 LCD_PointerOnStack;
    int16 LengthOfMenuOnStack;
} The_menu_stack[MAX_MENU_DEPTH];
/*
struct Menus_in_System {
    // This get pushed onto the menu stack.
    // Handles sub-menus up to 20 deep.
    void *MenuPtr;
    int16 LengthOfMenu;
} The_Menus_in_System[5] ; 
// The_Menus_in_System[0].MenuPtr = SubMenu_B_List;
// The_Menus_in_System[0].LengthOfMenu = 10;
/* = 
    {
        { &Main_MenuList[0], (sizeof(Main_MenuList) / sizeof(Main_MenuList[0])) - 1},
        { IndividualTests_Menu_List, (sizeof(IndividualTests_Menu_List) / sizeof(IndividualTests_Menu_List[0])) - 1},
        { SubMenu_B_List, (sizeof(SubMenu_C_List) / sizeof(SubMenu_C_List[0])) - 1},
        { SubMenu_B_List, (sizeof(SubMenu_B_List) / sizeof(SubMenu_B_List[0])) - 1},    
    };
*/
// int16 MenuPtrArrayIndex = 0;
int16 TheMenuLength     = 0;

struct a_menu { // each menu consists of the text to
    // display, a function pointer, and the param
    // value to send to the function. The function
    // is executed when the user clicks the 'enter' button.
    char MenuItem[21]; // text to display.
    void *Service;  // function pointer
    int16 param;    // the parameter to send to the function.
};
// Enable_RS485_Ctrl = DISABLED;
//Call_temp_SW  Special_test_for_Johnny

// ReinitGrCom1
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
    { " 1137 Hall Stn test ", Hall_Station_CAN,     0 },     
    { " 1137 Clear out 1&2 ", Set_or_Clear_1137_Input, 0xff00 },
    { " 1137 Set out 1 & 2 ", Set_or_Clear_1137_Input, 0xff01 },
    { " 1137 Clear out 1   ", Set_or_Clear_1137_Input, 0xf100 },
    { " 1137 Set out 1     ", Set_or_Clear_1137_Input, 0xf101 },
    { " 1137 Clear out 2   ", Set_or_Clear_1137_Input, 0xf200 },
    { " 1137 Set out 2     ", Set_or_Clear_1137_Input, 0xf201 },        
    { " 1137 Hall OFF      ", Send_Hall_Data,       0 },
    { " 1137 Hall ON       ", Send_Hall_Data,       5 },
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
    { " 1137 Hall Stn 30   ", Hall_Station_CAN,     30}, // Tx and Rx with Hall Station
    { " Hall Stn forever   ", Hall_Station_CAN,     -1},
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
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },  
};

struct a_menu Validate_Bds_Menu[] = { /// = 14
    { " Validate 1137    14", DoNothing,            0 },  //
    { " 1137 Hall Stn test ", Hall_Station_CAN,     0 },
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
    { " RESET MCU          ", WDT_Test,             0 }, 
    { " 1137 & 1139 Menu 13", Start_a_Menu,        13 },
    { " Test 1137 & 1139   ", Start_a_Menu,        15 },
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },   
};

struct a_menu Hall_Bds_Menu[] = { /// = 15
    { " Test 1137 & 1139 15", DoNothing,            0 },  //
    { " 1137 Hall Stn test ", Hall_Station_CAN,     0 },
    { " 1139 Hall Lantern  ", HALL_Lantern_RS485,   2 }, // Send_Hall_Lantern_Floor
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " Select Lantern Up  ", Send_Hall_Lantern_Floor,   0x80 },
    { " Select Lantern Down", Send_Hall_Lantern_Floor,   0x40 },
//    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },
    { " Select Lantern Both", Send_Hall_Lantern_Floor,   0xc0 },
    { " Select Lantern None", Send_Hall_Lantern_Floor,   0 },
    { " Show faults        ", DisplayMsgBrd_A,      0 },
    { " Clear Faults       ", ClearFaults,          0 },    
    { " 1137 & 1139 Menu 13", Start_a_Menu,        13 },                
    { " Validate 1137    14", Start_a_Menu,        14 },
    { " Test 1137 & 1139 16", Start_a_Menu,        16 },
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },   
};
    
struct a_menu Main_Hall_Bds_Menu[] = { /// = 16
    { " Test 1137 & 1139 16", DoNothing,            0 },  //
/*    
    { " 1137 Hall Stn test ", Hall_Station_CAN,     0 },
    { " 1139 Hall Lantern  ", HALL_Lantern_RS485,   2 },
    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 }, 
    { " Show faults A      ", DisplayMsgBrd_A,      0 },
    { " Clear Faults A & C ", Clear_All_Boards,     5 },
*/         
    { " 1137 QTY Menu      ", Start_a_Menu,        17 },
    { " 1139 QTY Menu      ", Start_a_Menu,        18 },
//    { " Clear 1107AN LEDs  ", Set_or_Clear_All,     0 },
//    { " Set   1107AN LEDs  ", Set_or_Clear_All,     1 },
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },   
};

struct a_menu Select_1137_Qty_Menu[] = { /// = 17
    { " 1137 QTY Menu    17", DoNothing,            0 },  //
    { " 3 - Up (three brds)", Qty_113x_bds,         3 },
    { " 2 - Up (two boards)", Qty_113x_bds,         2 }, 
    { " 1 - Up (one only)  ", Qty_113x_bds,         1 },
        
//    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 }, 
//    { " Show faults A      ", DisplayMsgBrd_A,      0 },
//    { " Clear Faults A & C ", Clear_All_Boards,     5 },        
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 }, 
};

struct a_menu Select_1139_Qty_Menu[] = { /// = 18
    { " 1139 QTY Menu    19", DoNothing,            0 },  //
    { " 3 - Up (three brds)", Qty_113x_bds,         0x1103 },
    { " 2 - Up (two boards)", Qty_113x_bds,         0x1102 },
    { " 1 - Up (one only)  ", Qty_113x_bds,         0x1101 },
         
//    { " Show Msg Bd C      ", DisplayMsgBrd_C,      0 }, 
//    { " Show faults A      ", DisplayMsgBrd_A,      0 },
//    { " Clear Faults A & C ", Clear_All_Boards,     5 },        
    { " RESET MCU          ", WDT_Test,             0 },    
    { " Run Power-Up Mode  ", JumpToPowerUp,        0 },     
};
    
struct Menus_in_System {
    // This info gets pushed onto the menu stack 
    // with a call to "Start_a_Menu(WhichMenu)".  (WBG)
    // Handles sub-menus up to 23 deep.
    void *MenuPtr;
    int16 LengthOfMenu;
} The_Menus_in_System[] =
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
    };			
      
enum {
    Init_the_Menu,                // 0
    CAN_test,                     // 1
    RS_232_test,                  // 2
    RS_485_test,                  // 3
    Watch_dog_test,               // 4
    MRAM_test,                    // 5
    SD_Card_test,                 // 6
    WIFI_test,                    // 7
    SW_version_display,           // 8
    RTC_test,                     // 9
    Push_a_button_test,           // 10
    Four_line_display_test,       // 11
    Adjust_LCD_display,           // 12
    GAL_IO_Board_test,            // 13
    Diagnostics_test,             // 14
    Generic_test,                 // 15
    Scroll_Main_Menu,             // 16
    Perform_Main_Menu_Function,   // 17
    Function_Pass,                // 18
    Function_Fail,                // 19
    Idle_Time_wbg,                // 20
    xxTest,                       // 21
    Wait_for_Service_Request,      // 22
    DefaultCase
};

enum {
    Init1132TEST,
    LookAtButtons,
    RS232_TEST,
    SD_TEST,
    CAN_TEST,
    RS485_TEST,
    RTC_TEST,
    PC104_TEST,
    VideoSPI_TEST,
    MRAM_TEST,
    GALX1132_TESTDONE,
    GALX1132_FAILED,
    Retest,
    Retest_after_delay,
    Wait_on_Operator_Response
};


extern int32 test_msg;
extern int32 test_msg1;
extern int32 test_msg2;
extern int32 test_msg3;
extern int32 test_msg4;



//ZYM 2-2-2018
extern int16 StopOnFail;






int16 DoNothing(int16 New_LCD_Menu)
{
    int16 TheMenu = New_LCD_Menu;

    // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED

    switch(DoNothingCase) {
    case Init_the_Menu:
        timers[t_Generic_Delay] = 0;
        DoNothingCase = Idle_Time_wbg;
        break;

    case Idle_Time_wbg:
        if(timers[t_Generic_Delay] >= 3) {
            DoNothingCase = Init_the_Menu;
//-         SIU.GPDO[Z6_LED].R = 1;	// turn led OFF (wbg)// tagwbg 15
            return FINISHED_WITH_SERVICE;
        }
        break;

    default:
        DoNothingCase = Init_the_Menu;
        Clear_Msg_Board();
        Clear_Msg_Board_B();
        sprintf(src,"11: %d", New_LCD_Menu);  // New_LCD_Menu // TheMenuLength
        Add_msgx_First_TimeOnly(src,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return FINISHED_WITH_SERVICE;
        break;
    }
    return CONTINUE_SERVICE;
}

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

/**********************************************
* Function: void MenuManager(void)            *
* Description: The cases handles all actions  *
*    for managing the menus. It will call     *
*    the fucntion associated with the menu    *
*    item pointed to by the '>' and will      *
*    continue to call the function as long as *
*    the function returns 1 If the function   *
*    returns 0, the display is updated with   *
*    the current menu structure. If the       *
*    function returns -1, the display is not  *
*    updated until the mode button is pressed.*
*    This allows any error msg displays       *
*    generated by the called function to be   *
*    continuosly displayed until dismissed via*
*    The mode button. If the function returns *                                             *
*    2, the option is given to perform a      *
*    retest.                                  *
*                                             *                                             *
* Created: September, 2017                    *
*                                             *
* Example:                                    *
*                                             *
*                                             *
* Name: William Grauer                        *
*                                             *
**********************************************/

void MenuManager(void)
{
    // ********** THE NEW MAIN MASTER MENU **************
    static int16 Init_Main_Menu = 0;
    static int16 loop = 0;

    int16 RtnVal= 0;

    MainMenuPtr = Main_MenuList; // Main_MenuList
    MyMenuPtr = MyMenuList;

    switch(New_LCD_Menu) {

    case Init_the_Menu:	// 0
//-     SIU.GPDO[Z0_LED].R = 1;  // turn LED OFF
        loop = 0;
        parameter = 0;
        New_LCD_Menu = LookAtButtons;
        break;

    case LookAtButtons:    
        if(loop++ > LED_TOGGLE_DELAY)
        {
            SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
            loop = 0;  
        }       

        if (ServiceTheButtons() == POP_MENU_PTR) { // pop the menu ptr
            PopMenuPtrArray(); // if the mode button pressed, go up one level.
            // New_LCD_Menu = LookAtButtons;
            // the init_control() calls init_LCD() which
            ClearFaults(0);
        };
        // sets LCD_Upd_Dpy = 1 so that the first time
        // into ServiceTheButtons() the display will be updated.
        break;

    case Wait_for_Service_Request: // Call back function to handle the selected test.
        //  RtnVal =FunctionPtrs[11](0); // produces compiler error
        //Service_Request = FunctionPtrs[11];  // ok
        RtnVal = Service_Request(parameter);  // Runs the test or service requested.
        if( PIT.CVAL1.R < min_ISR_timer) min_ISR_timer =  PIT.CVAL1.R; // save the slowest process
        switch(RtnVal) {
        case FINISHED_WITH_SERVICE:
            if(Enable_Display_ISR_timing) DisplayISR_timing(min_ISR_timer);
            New_LCD_Menu = LookAtButtons;
            LCD_Upd_Dpy = 1;
            break;

        case DONE_BUT_CONTINUE_DISPLAY:
            if(Enable_Display_ISR_timing) DisplayISR_timing(min_ISR_timer);
//-         SIU.GPDO[Z0_LED].R = 1;	// turn led OFF
            New_LCD_Menu = Idle_Time_wbg;
            break;

        case RETEST_OPTION:
            New_LCD_Menu = Retest;
            break;
            
        case RETEST_OPTION_AFTER_DELAY:
            timers[t_Generic_Delay] = 0;
            New_LCD_Menu = Retest_after_delay;
            break;            

        case CONTINUE_SERVICE:
            // do nothing
            break;

        default:
            break;
        }
        break;

    case Retest_after_delay:
        if(timers[t_Generic_Delay] >= 25 )
           New_LCD_Menu = Retest;
        break;
            
    case Retest:
//-     SIU.GPDO[Z6_LED].R = 1; // turn off LED
//ZYM 1-18-2018
//Show retest message
//-     fail_testagain();
        LCD_Upd_Dpy = 1;
        New_LCD_Menu = Wait_on_Operator_Response;
        break;

    case Wait_on_Operator_Response:
//-     SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED
        if(Check_for_Mode_Button() ) New_LCD_Menu = LookAtButtons;
        else if(Check_for_Enter_Button() ) New_LCD_Menu = Wait_for_Service_Request;
        break;

    case Idle_Time_wbg: // Continue to display the test results until Mode Button pressed.
//-     SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED
        Check_for_Up_Button(); // Just to clear out the button flag if someone presses button
        Check_for_Down_Button(); // dito
        Check_for_Enter_Button();// dito
        if(Check_for_Mode_Button() ) {
            // this is the only button (in this case) that will cause an action.
//-         SIU.GPDO[Z6_LED].R = 1; // turn off LED
            New_LCD_Menu = LookAtButtons;
            LCD_Upd_Dpy = 1;
        }
        break;

        // case DefaultCase:
        // no break;
    default:
        New_LCD_Menu = Init_the_Menu;
        Clear_Msg_Board();
        Clear_Msg_Board_B();
        sprintf(src,"11: %d", New_LCD_Menu);  // New_LCD_Menu // TheMenuLength
        Add_msgx_First_TimeOnly(src,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        break;
    }
}

/**********************************************
* Function: int16 ServiceTheButtons(void)     *
* Description: Debounces the 4 push buttons:  *
*   'Up', 'Down', 'Mode', and 'Enter'.        *
*    For up/down, the cursor will be moved.   *
*    For 'Enter' it will set up the function  *
*    pointer. For 'Mode' it returns 1,        *
*    otherwise return 0. If 1 is returned, it *
*    is the signal for MenuManager() exit     *
*    the current menu and move up to the      *
*    parent menu.                             *
*                                             *
*    This function also updates the mcu       *
*    display buffer if necessary.             *
*                                             *
*    This is only called from the             *                                              
*    MenuManager()                            *
*                                             *
*                                             *                                              
* Created: September, 2017                    *
*                                             *
* Example:                                    *
*                                             *
*                                             *
* Name: William Grauer                        *
*                                             *
**********************************************/

int16 ServiceTheButtons(void)
{
    // the init_control() calls init_LCD() which
    // sets LCD_Upd_Dpy = 1 so that the first time
    // into ServiceTheButtons() the display will be updated.
    int16 i_index = 0;
    static last_time_tic = 0;

    if(Check_for_Mode_Button() ) {
//-     SIU.GPDO[Z6_LED].R = 1;	// turn led OFF (wbg)// tagwbg 15
        New_LCD_Menu = Init_the_Menu;
//        sprintf(src,"index: %d",MenuPtrArrayIndex);
//        Log_a_fault(src,0);
        return POP_MENU_PTR; // 1 (the menu manager() will actually
        // do the poping up to the parent menu.)
    }

    if(Check_for_Enter_Button() ) {
        Service_Request =  MenuPtr[LCD_Pointer].Service;
        parameter  =  MenuPtr[LCD_Pointer].param;
        // sets the function pointer to point to the test routine.
        New_LCD_Menu = Wait_for_Service_Request;
        clr_lcd_dply();
    }

    if(Check_for_Up_Button()) {
        LCD_Pointer--; // increment to the next menu item
        if(LCD_Pointer < 1) LCD_Pointer = TheMenuLength;
        LCD_Upd_Dpy = 1;
    }

    if(Check_for_Down_Button()) {
        LCD_Pointer++; // decrement to the next menu item
        if(LCD_Pointer > TheMenuLength ) LCD_Pointer = 1;
        LCD_Upd_Dpy = 1;
    }

    if(LCD_Upd_Dpy == 1) {  // Display must be updated
        LCD_Upd_Dpy = 0;
        line_ptr = (LCD_Pointer - 1) % 3;
        start_ptr = LCD_Pointer - line_ptr;
        for(i_index=0; i_index<=19; i_index++) {
            LCD_Display[0][i_index] = MenuPtr[0].MenuItem[i_index];
            LCD_Display[1][i_index] =
                MenuPtr[start_ptr].MenuItem[i_index];
            if ((start_ptr + 1) > TheMenuLength) {
                LCD_Display[2][i_index] = ' ';
                LCD_Display[3][i_index] = ' ';
            } else {
                LCD_Display[2][i_index] =
                    MenuPtr[start_ptr+1].MenuItem[i_index];
                if ((start_ptr + 2) > TheMenuLength)
                    LCD_Display[3][i_index] = ' ';
                else
                    LCD_Display[3][i_index] =
                        MenuPtr[start_ptr+2].MenuItem[i_index];
            }
        }

        LCD_Display[line_ptr+1][0] = '>';	// show cursor
    }

    if (New_LCD_Menu != Idle_Time_wbg) { // see if time to display
        // version #
        if (timers[tsec] != last_time_tic) {
            if(timers[tsec] == 0) {
                for(i_index=0; i_index<=19; i_index++) {
                    LCD_Display[0][i_index] = MenuPtr[0].MenuItem[i_index];
                }
            } else if (timers[tsec] == 2) {
                New_write_display(Version_Display,0,0);
            }
        }
        last_time_tic = timers[tsec];
    }
    return NORMAL; // 0
} // END OF ServiceTheButtons()


int16 Display_SW_Version(int16 New_LCD_Menu)
{
    int16 Menu_Index;

    Menu_Index = New_LCD_Menu;
    New_write_display("                    ",0,0);
    New_write_display(" Software Version:  ",1,0);
    New_write_display("      01.01.01      ",2,0);

    debug_msg(VerDate, 3);

    if ((version/10) == 0)
        LCD_Display[2][6] = ' ';
    else
        LCD_Display[2][6] = (char)((version/10) + '0');


    LCD_Display[2][7]  = (char)((version%10)  + '0');
    LCD_Display[2][8]  = '.';
    LCD_Display[2][9]  = (char)((revision/10) + '0');
    LCD_Display[2][10] = (char)((revision%10) + '0');

    LCD_Display[2][11] = '.';
    LCD_Display[2][12] = (char)((interim_revision/10) + '0');
    LCD_Display[2][13] = (char)((interim_revision%10) + '0');
    if(Sandbox_revision != '0') {
        LCD_Display[2][14] = '-'; // Sandbox_revision
        LCD_Display[2][15] = (char)Sandbox_revision;
    }
    
// ZYM 12-13-2017
//-	sprintf(&LCD_Display[0][1],"%15i",test_msg);      
    return -1;
}

void clr_lcd_dply(void)
{
    New_write_display("                    ",0,0);
    New_write_display("                    ",1,0);
    New_write_display("                    ",2,0);
    New_write_display("                    ",3,0);
}

void fail_testagain(void)
{
//    New_write_display("TEST FAILED (AGAIN?)",2,0);
//    New_write_display("NO(MODE)/YES(ENTER) ",3,0);
    New_write_display("RETST? N/Y:MODE/ENTR",3,0);
}

int16 Clear_All_Boards(int16 Boards)
{
     if(Boards & 0x1) Clear_Msg_Board(); // board A
     if(Boards & 0x2) Clear_Msg_Board_B();
     if(Boards & 0x4) Clear_Msg_Board_C();
     
     return FINISHED_WITH_SERVICE;
}


int16 ClearFaults(int16 dummy)
{
    Clear_Msg_Board();
//    clrgoutp(GRtest_osignal_array[7]);
    return FINISHED_WITH_SERVICE;
}

int16 SetClearForceToFail(int16 on_off)
{
    ForceTestToIndicateFail = on_off; 
    return FINISHED_WITH_SERVICE;
}

int16 QueryMenuVariables(int16 dummy)
{
    // temporary funtion to assist in debugging multilevel
    // menus.
    sprintf(src,"ML: %d", TheMenuLength);
    Log_a_fault(src,0);

    sprintf(src,"lcdP: %d", LCD_Pointer);
    Log_a_fault(src,0);

    sprintf(src,"MenP: %d", MenuPtr);
    Log_a_fault(src,0);

    return FINISHED_WITH_SERVICE;
}

void ClearTheButtons(void)
{
    // flush out all the buttons.
    Check_for_Up_Button();
    Check_for_Down_Button();
    Check_for_Mode_Button();
    Check_for_Enter_Button();
}

int16 ReinitGrCom1(int16 dummy)
{
    //init_sx_com_port_1();
    return FINISHED_WITH_SERVICE;
}

int16 ISR_test(int16 dummy)
{
    if(Enable_Display_ISR_timing) Enable_Display_ISR_timing = 0;
    else
    {
        Enable_Display_ISR_timing = 1;  
        min_ISR_timer = 1087500 - 1;
//        SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15 
    }
    return FINISHED_WITH_SERVICE;
}

void DisplayISR_timing(int32 time)
{
    sprintf(src,"ISR:%d: tic %d", time, Timer_tic );
    Log_an_event(src);
    min_ISR_timer = 1087500 - 1; 
}

int16 Display_consB_file(int16)
{ int16 i = 0; 
  int16 ReturnVal= 0;
  static int16 BuffRead_operation = Init_BuffRead;

    switch (BuffRead_operation) {
    case Init_BuffRead:
        Clear_Log_Buffer();    
        sprintf(src,"which= %d", WhichMenuToStart);              
        for(i=0;i < 31; i++)
        {
            sprintf(src,"consB[%d]=%d", i, consB[i] );
            Log_an_event(src);
        }    
        BuffRead_operation = Display_Buff;
        break;

    case Display_Buff:
        ReturnVal = DisplayMsgBrd_A(0);
        if(ReturnVal == CONTINUE_SERVICE) return ReturnVal;
        // else
        BuffRead_operation = Init_BuffRead;
        return ReturnVal;
        break;  

    default:
        BuffRead_operation = Init_BuffRead;
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;
}


int16 Handle_Retest_Request(int16 The_Kase)
{
    enum {
        NoRetest,
        PleaseRepeat,
        InitRetest,
        WaitForTimeout,
        WaitOnOperator,
        ViewMsgBd_A,
    };

// return 0 for do not retest.
// return 1 for try again.
// return 2 for waiting for timeout.
// return 3 for waiting for operator.

// New Work for Nov 18

    static int16 Kase;
    static int16 Toggle = 0;
    int16 ReturnVal = 2;
    int16 MsgBd_A = 0;  // holds the return val from msgBd_A
    int16 j,k = 0;

    if(The_Kase == 0) Kase = InitRetest;
    switch (Kase) {
    case InitRetest:
    // save line 4 of display in local buffer
       for(k = 0; k < 3; k++)
       {
           for(j =0; j <20; j++)
           {
              Saved_Lines[k][j] =  LCD_Display[k][j] ;
//              if(Saved_Lines[k][j] == NULL)break;          
           }        
       }
       Line_4_Blank = 1; // = 1 if Line 4 display was blank, set "Line_4_Blank" equal "1", NO NEED TO Toggle Line 4;
       for(j =0; j <20; j++)
       {
          Saved_Lines[3][j] =  LCD_Display[3][j] ;
//          if(Saved_Lines[3][j] == NULL)break;
          if(Saved_Lines[3][j] != 0x20)Line_4_Blank = 0; // = 0 if Line 4 display was NOT blank, set "Line_4_Blank" equal "0", NEED TO Toggle Line 4;         
       }
//          SIU.GPDO[Z6_LED].R = 0; // On
        timers[t_Generic_Delay] = 0;
        Retest_Delay = 10; // 10 = 1s
        Toggle = 0;
        Kase = WaitForTimeout;
        ReturnVal = WaitForTimeout;
        break;

    case WaitForTimeout: // in order to allow operator to read msgs
        Check_for_Mode_Button();
        Check_for_Enter_Button();  // Release Mode & Enter keys
        
//ZYM 2-2-2018
//Adding "if(StopOnFail==3)" to make sure that during "Test - do not stop",
//the "Handle_Retest_Request" GOT BYPASSED, so that the "FunctionalTest()" 
//could perform the next test NONE STOP
        if(StopOnFail==3)
        {
        	Kase = InitRetest;
            ReturnVal = NoRetest; 	
        }
        else
        {
        	if(timers[t_Generic_Delay] >= Retest_Delay ) {
//-           	SIU.GPDO[Z6_LED].R = 1; // turn off LED
            	fail_testagain(); // display retest prompt on line 4
            	LCD_Upd_Dpy = 1;
            	timers[t_Generic_Delay] = 0;
            	Retest_Delay = 30;  // 30 = 3s
            	Kase = WaitOnOperator;
            	ReturnVal = WaitOnOperator;
        	} 	else ReturnVal = WaitForTimeout;
        }        
        break;

    case WaitOnOperator: // After 3s change the display to show 4 lines of error msgs.
    // Then after another 3s show 3 lines of error msgs and the 'Retest' prompt on line 4.
    // Toggle between these 2 displays every 3s. Toggle early if the UP of DOWN buttons
    // are pressed.
        Toggle = Toggle_Display(Toggle); // toggle if 3s elapsed and line 4 is not blank.        
        if( Check_for_Down_Button() || Check_for_Up_Button() ) 
        { 
          // Toggle as soon as the Up or Down button pressed, without waiting
          // for 3s.
           Retest_Delay = -1; 
           Toggle = Toggle_Display(Toggle);  
        }
        if(Check_for_Mode_Button() ) {
        	clr_lcd_dply();
            if(LogIndex == 0)
            { // nothing in the buffer
               Kase = InitRetest;
               ReturnVal = NoRetest; 
               break;  
            }
            Kase = ViewMsgBd_A;
            //Kase = InitRetest;
            ReturnVal = ViewMsgBd_A; // NoRetest;
            break;
        } else if(Check_for_Enter_Button() ) {
            Kase = InitRetest;
            ReturnVal = PleaseRepeat;
            break;
        } else ReturnVal = WaitOnOperator;
        break;
        
    case ViewMsgBd_A:
            MsgBd_A = Display_Message_Board(0x8000);
            if(MsgBd_A == CLOSE_MSG_BOARD) // Enter button
            { //  repaint 4-line display and then go back to
              // WaitOnOperator
                Repaint_Retest_Display();
//ZYM 2-2-2018                
//-             Kase = WaitOnOperator;
//-             ReturnVal = WaitOnOperator; 
                timers[t_Generic_Delay] = 0;
        		Retest_Delay = 5; // 5 = 0.5s
                Kase = WaitForTimeout;
                ReturnVal = WaitForTimeout; 
                break;
            }
            else if(MsgBd_A == FINISHED_WITH_SERVICE)
            { // Mode button
               Kase = InitRetest;
               ReturnVal = NoRetest; 
               break;
            }
            else ReturnVal = ViewMsgBd_A;
        break;
        
    default:
        Kase = InitRetest;
        ReturnVal = NoRetest;
    }
    return ReturnVal;
}

int16 Repaint_Retest_Display(void)
{
    int16 j,k = 0;
    for(k=0; k<4; k++)
    {
       for(j =0; j <20; j++)
       {
          LCD_Display[k][j] = Saved_Lines[k][j] ;
//          if(LCD_Display[k][j] == NULL) break;
       }  
    }
    return 0;
}

int16 Get_Num_of_Menus(void)
{
    return ( sizeof(The_Menus_in_System) / sizeof(The_Menus_in_System[0])  );
}

int16 Clear_Both_Boards(int16 dummy)
{
    ClearFaults(0);
    Clear_Msg_Board_B();
    return FINISHED_WITH_SERVICE;
}


int16 Toggle_Display(int16 Toggle)
{
    int16 j = 0;
    if(Line_4_Blank) return Toggle; // no need to switch displays
    // if line 4 is a blank line.
    if(timers[t_Generic_Delay] >= Retest_Delay )
    {
       timers[t_Generic_Delay] = 0; 
       Retest_Delay = 30;
       if(Toggle == 1)
       {
           Toggle = 0;
//           SIU.GPDO[Z6_LED].R = 1; // turn off LED
           for(j =0; j <21; j++)
           {
              LCD_Display[3][j] = Saved_Lines[3][j] ;
//              if(LCD_Display[3][j] == NULL) break;
           } 
       }
       else
       {
           Toggle = 1;
//           SIU.GPDO[Z6_LED].R = 0; // turn LED On
           fail_testagain();             
       }   
    }
    return Toggle;
}


 