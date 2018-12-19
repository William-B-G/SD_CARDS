// GALX_1137_1139.h

#define NINETEEN_DOT_2 354 // 19.2 kb

extern int16 Hall_Station_CAN(int16 cycles);
extern int16 One_Hall_Station_CAN(int16 Test_loop);
extern int16 Hall_Station_CAN_Data(int16 Data);
extern int16 HALL_Lantern_RS485(int16 dummy);
extern int16 Get_Srl_Hall_Station_Pkg(int16 dummy);
extern int16 Unpk_Hall_Station(int16 dummy);
extern int16 Reset_1137_board(int16 Kase);
extern int16 Send_Hall_Data(int16 msg);
extern int16 Send_Hall_Reset(int16 dummy);
extern int16 DisplayGrpUnpkData(int16 dummy);
extern int16 Clear_All_Boards(int16 Boards);
extern int16 Case_16_data;
extern int16 Case_14_data;
extern int16 CaseNumber;
extern int16 Data_Zero;
extern int16 Comm_to_1137;
extern int16 GALX4_0047N;
extern int16 delay_for_Comm_to_1137 ;
extern int16 Latest_Hall_data;
extern int16 test_Loop;
extern const int16 Hall_1137_bd_Addr[];
extern int16 wbg_grp_pack_num[];
extern void *GALX4_0047_OUT_array[];
extern void *GALX4_0047_in_array[];


extern int16 Reset_14_and_16(int16 dummy);
extern int16 Req_packet_14(int16 msg);
extern int16 Set_or_Clear_1137_Input(int16 Set_Clear);
extern int16 Display_channels(int16 command);
extern int16 Send_Hall_Lantern_Floor(int16 Lantern_Select);
extern uint8 Hall_Lantern_Select;
extern uint8 Hall_Lantern_floor_A;
extern uint8 Hall_Lantern_floor_B;
extern uint8 Hall_Lantern_Data_A;
extern uint8 Hall_Lantern_Data_B;
extern int16 GALX4_0047N;


extern int16 DoNothing(int16 New_LCD_Menu);
extern int16 Really_DoNothing(int16 New_LCD_Menu);
extern int16 Qty_113x_bds(int16 Test_and_Qty);
extern int16 Set_or_Clear_All(int16 Set_1_or_clear_0);
extern int16 Use_CT_CAN(int16 dummy);
extern int16 Generate_mini_doc(int16 which_doc);
extern int16 Display_a_min_doc(int16 which_doc);
extern int16 ShowMeCTi0_CTi5(int16 dummy);
extern int16 Show_grp_can_Packet_num(int16 dummy);
extern int16 toggle_1137_comm(int16 dummy);
extern int16 toggle_4_0047N(int16 dummy);
extern int16 Incr_Decr_10(int16 dummy);
extern int16 Periodic_Background_Task(int16 dummy);
extern int16 Reset_All_1137s(int16 dummy);
extern int16 Enable_gr_CAN_Ctrl_flag(int16 enable_disable);
extern int16 Set_1137_Diagnostic_Position(int16 position);
extern int16 Set_DI_on(int16 dummy);
extern int16 Clear_DI_off(int16 dummy);
extern int16 Set_UI_on(int16 dummy);
extern int16 Clear_UI_off(int16 dummy);
extern int16 Cmnd_DO_on(int16 dummy);
extern int16 Cmnd_DO_off(int16 dummy);
extern int16 Cmnd_UO_on(int16 dummy);
extern int16 Cmnd_UO_off(int16 dummy);
extern int16 Cmnd_All_on(int16 dummy) ;
extern int16 Cmnd_All_off(int16 dummy) ;
extern int16 Clear_100ms_timer(int16 The_timer);
extern int16 Toggle_LDOxx(int16 data_zero);
extern int16 Full_Station_CAN_Test(int16 dummy);
