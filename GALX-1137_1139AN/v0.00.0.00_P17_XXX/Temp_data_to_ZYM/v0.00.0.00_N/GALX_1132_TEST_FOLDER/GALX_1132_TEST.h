// GALX_1132_TEST.h

#define DISABLED 0
#define ENABLED  1
#define SUCCESS  1
#define FAILED   0
#define TRUE 1
#define FALSE 0
#define PASS 1
#define FAIL 0

enum 
{
  DONE_BUT_CONTINUE_DISPLAY = -1, // failed
  FINISHED_WITH_SERVICE, // passed
  CONTINUE_SERVICE, // still performing test
  RETEST_OPTION_AFTER_DELAY,  //  to see msg
  RETEST_OPTION, //failed, but give option to redo
  CLOSE_MSG_BOARD, //  Used by retest option
  DEFAULT  // Fatal error - abort	
};

enum {
    Init_BuffRead,
    Display_Buff
};

extern void GALX_1132_TEST(void);
extern void clr_lcd_dply(void);
extern void fail_testagain(void);
extern void MenuManager(void);
//extern void PrepareFuntionPtrArray(void);
extern struct galtime t;
extern struct galdate d;

extern int16 Check_for_Mode_Button(void);
extern int16 Check_for_Up_Button(void);
extern int16 Check_for_Down_Button(void);
extern int16 Check_for_Enter_Button(void);

extern int16 TESTCASE_1132;
extern int16 Displayed_msg_page;
extern uint8 Enable_CAN_Ctrl;
extern uint8 Enable_CT_CAN_Ctrl; // disable while doing Car Top CAN test (wbg)
extern uint8 Enable_MR_CAN_Ctrl; // disable while doing Machine Room CAN test (wbg)
extern uint8 Enable_gr_CAN_Ctrl;
extern uint8 Enable_RS232_Ctrl; 
extern uint8 Enable_RS485_Ctrl;
extern uint8 Enable_iospi_Ctrl;
extern uint8 Enable_Hallcom_Ctrl;
extern int16 Max_Displayed_msg_page;
extern char *VerDate;
extern uint8 temp_char;
extern uint8 CurrentTestPassFail;
extern uint8 Second_Half_PassFail;
extern uint8 First_Half_PassFail;
extern uint8 Chan_1_1137;
extern uint8 Chan_2_1137;
extern int16 sx_rbs[] ;			// receive buffer start pointer com 1-2
extern int16 gc_rcv_dma_gix;
extern uint16 gc_rcv_dma_buf[];
extern int16 sx_tbs[];          // trasmit buffer start pointer com 1-2
extern void *GRtest_osignal_array[];
extern int16 ver_rev;
extern int32 min_ISR_timer;
extern int16 Display_ISR_timing;  // if 1 then send the ISR timing to msgboard A.
extern int16 Enable_Display_ISR_timing; // set/reset from a menu item (wbg).
extern int16 en_cmd_state[];
extern int16 Functional_Test_is_Running;
extern int16 WhichMenuToStart;
extern int16 consB[];
extern int16 MenuPtrArrayIndex;

extern uint8 SUF_output_cmnd_flag;
extern uint8 SU_output_cmnd_flag ;
extern uint8 SDF_output_cmnd_flag;
extern uint8 SD_output_cmnd_flag ;
extern uint8 STE_output_cmnd_flag ;
extern uint8 CEN_output_cmnd_flag ;
extern uint8 MC_output_cmnd_flag  ;
extern uint8 RUN_output_cmnd_flag;
extern uint8 ForceTestToIndicateFail;

//extern struct a_menu;
//extern int16 Msg_Board_Offset;

extern int16 Generic_Test(int16 LCD_Menu);
extern void New_write_display(char *s, int16 row, int16 col);
extern void Sprint_Lcd_SignalChar(int16 Row,int16 Col,unsigned char Ch);
extern int16 WDT_Test(int16 Delay);
extern int16 Display_Test(int16 LCD_Menu);
extern int16 PushButtons_Test(int16 LCD_Menu);
extern char Version_Display[21];
extern int16 JumpToPowerUp(int16 lcd_menu);
extern int16 The_RS_232_test(int16 Operation);
extern int16 The_RS_485_test(int16 Operation);

extern int16 Display_previous_msg_block(void);
extern int16 Display_next_msg_block(void);
extern int16 Get_Last_Msg_from_Board(void);
extern int16 Get_Msg_from_Board(int16 the_Msg_Board_index);
extern int16 Get_Msg_Board_index(void);
extern void Clear_Msg_Board(void);
extern int16 Add_msg_to_array(char *msg);
extern int16 Display_Message_Board(int16 dummy);
extern void Fill_up_msg_array(void);
extern int16 Try_to_display(int16 Start_msg_index);
extern void Display_4_lines(int16 Start_msg_index);
extern int16 MsgBrd_StartupPage(void);
extern int8 msgSent[40];
extern int16 Add_msgx_First_TimeOnly(char *msg, int16 MsgTag_0_39);

extern void Clear_Msg_Board_B(void);
extern int16 Display_next_msg_block_B(void) ;
extern int16 Display_previous_msg_block_B(void);
extern void Display_4_lines_B(int16 Start_msg_index);
extern int16 MsgBrd_StartupPage_B(void);
extern int16 Display_Message_Board_B(int16 dummy);
extern int16 DisplayMsgBrd_B(int16 dummy);
extern int16 Add_msg_to_array_B(char *msg, int16 index);

extern int8  msgSent_C[40];
extern void  Clear_Msg_Board_C(void);
extern int16 Display_next_msg_block_C(void) ;
extern int16 Display_previous_msg_block_C(void);
extern void  Display_4_lines_C(int16 Start_msg_index);
extern int16 MsgBrd_StartupPage_C(void);
extern int16 Display_Message_Board_C(int16 dummy);
extern int16 DisplayMsgBrd_C(int16 dummy);
extern int16 Add_msg_to_array_C(char *msg);
extern int16 Try_to_display_C(int16 Start_msg_index);
extern int16 Displayed_msg_page_C;
extern int16 Max_Displayed_msg_page_C;

extern void pack_hl_com (int16 pkt, int16 dev);
extern int16 receive_hl_com (void);
extern int16 Hal_com_Tx_test(int16 dummy);
extern int16 DisplayMsgBrd_A(int16 dummy);

extern int16 Test_MRAM(int16 LCD_Menu);
extern int16 Test_MRAM_chip2(int16 LCD_Menu);
extern int16 test_MRAM_present(int16 WhichChip);
extern int16 test_MRAM_present_chip2(void);
extern int16 Hal_com_Tx_test_Continuous(int16 dummy);

extern void unpack_grcan_in (void);
extern int16 Start_Main_MenuList(void);
extern int16 Start_MyMenuList(void);
extern int16 Start_MyMenuList_main(void);
extern int16 Start_SubMenu_B(void);
extern int16 Start_SubMenu_C(void);
extern int16 Start_Tstr_Diagnostics(void);
extern int16 Start_groupCom_Menu(void);
extern int16 Start_Individual_Tests(void);
extern int16 Start_FPGA_Diag_Menu_List(void);
extern int16 Log_a_fault(char *msg, int16 One2reset);
extern int16 Test_Group_CAN(void);
extern void ClearTheButtons(void);
extern int16 TEST_GRCAN(int16 Operation);
extern void Group_CAN_Control(void);
extern int16 TEST_CTCAN(int16 Operation);
extern int16 Group_CAN_Diagnostics(void);
extern int16 CT_CAN_Diagnostics(void);
extern int16 MR_CAN_Diagnostics(void);
extern int16 Selector_CAN_Diagnostics(void);
extern int16 Log_an_event(char *msg);
extern int16 Log_a_fault_C(char *msg, int16 One2reset);
extern int16 Clear_Log_Buffer(void);
extern int16 Start_Tstr_Diagnostics(void);
extern int16 Start_Individual_Tests(void);
extern unsigned char RS_485_RxBuf[];
extern int16 sx_rbf[];
extern int16 ShowMeRxBuf(int16 Dummy);
extern int16 ShowMe_sx_recbuf(int16 Dummy);
extern int16 ShowMe_tempBuff(int16 Dummy);
extern int16 WhichBuffer;
extern int16 FunctionalTest(int16 Dummy);
extern int16 Check_Mode_Enter_Combo(void);
extern int16 TEST_SD(int16 LCD_Menu);
extern int16 TEST_RTC(int16 LCD_Menu);
extern int16 Toggle_RS_485_Ctrl(int16 dummy);
extern int16 Call_temp_SW(int16 dummy);
extern int16 RS485_Baud;
extern int16 Set_RS485_Baud(int16 Counter);
extern int16 Display_RS485_Baud(int16 dummy);
extern int16 Hl_Tx_to_Grp_Rx(int16 Dummy);
extern void CAN_Init(void);
extern void CAN_Control(void);
extern void clearDMA_RxBuff(void);
extern int16 GrpRxPacket(int16 Dummy);
extern int16 CheckGroupRxBuff(int16 dummy);
extern int16 EnableDisableRS485_com_Ctrl(int16 enable);
extern int16 ReinitGrCom1(int16 dummy);
extern int16 Special_test_for_Johnny(int16 dummy);
extern int16 CAN_Ctrl_EN_bit(int16 enable_disable);
extern int16 Query_Ctrl_bits(int16 dummy);
extern int16 All_Ctrl_EN_bit(int16 enable_disable);
extern void Small_initcom(void);
extern void init_grp_DMA(void);
extern void Small_init_grp_DMA(void);
extern int16 Read_FPGA_ver_rev(int16 which_fpga);
extern void i1spi_unpack (void);
extern void iospi_unpack (void);
extern void nts_spi_unpack (void);
extern int16 ISR_test(int16 dummy);
extern void DisplayISR_timing(int32 time);
extern int16 WiFi_Test(int16 dummy);
extern int16 TEST_MRCAN(int16 Operation);
extern int16 TEST_MLCAN(int16 Operation);
extern int16 TEST_SLCAN(int16 Operation);
extern int UpdateAllSEB(void);
extern void ReadAllSEB(void);
extern int16 Display_consB_file(int16);
extern void local_setdate(void);
extern void local_settime(void);
extern void local_gettime(void);
extern void local_getdate(void);
extern int16 ViewRTC(int16);
extern void SetupRTC(void);
extern void SW_Vers_disp(void);
extern int16 Set_CPLD1_Addr(int16 param);
extern int16 Set_CPLD2_Addr(int16 param);
// extern void SetUpMenuPtrs(void);
extern int16 Start_a_Menu(int16 WhichMenu);
extern int16 Set_iospi_Ctrl_flag(int16 enable_disable);
extern int16 Set_All_outputs(int16 on_off);
extern int16 Set_specific_output(int16 Output_Num_and_on_off); 
extern int16 SetClearForceToFail(int16 on_off);    
extern int16 Handle_Retest_Request(int16 The_Kase);
extern int16 Read_1106(int16 Address);  
extern int16 CheckPB(int16 which);
extern int16 Set_out1_off(int16 which) ;
extern int16 Output_on_1106(int16 Channel);
extern int16 All_On_Off(int16 Addr);
extern int16 TEST_NTSCAN(int16 Operation);
extern int16 Rcv_Net_Response (int16 comnmb, int16 net_cmd, bool init_flag);










                                                         









