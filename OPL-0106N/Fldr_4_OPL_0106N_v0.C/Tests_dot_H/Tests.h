// Tests.h

#include "SD_TEST.h"
#include "RTC_TEST.h"
#include "GALX_1132_TEST.h"
#include "MRCAN_TEST.h"
#include "SLCAN_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1137_1139.h"
#include "GALX_1121_global.h"
#include "Menus.h"
#include "SEB.h"
#include "Diagnostics.h"

#define MAX_GENERIC_TEST 12
#define MAX_TEST_DEFINED 9
#define ABORT 0x4000 // Turn off all IO's even if structure says to leave them on

#define MAX_TEST_MODULE_SIGNAL_INDEX 12
#define CLEAR_MSG_BOARD_A 1
#define CLEAR_MSG_BOARD_B 2
#define CLEAR_MSG_BOARD_C 4
#define CLEAR_ALL_MSG_BOARDS 7
#define DELAY_3 3
#define MAX_COP_out_index 5

enum
{
    CSS_ReqSignal_enum,
    C120_ReqSignal_enum,
    L15_ReqSignal_enum,
    FSS_ReqSignal_enum,
    CST_ReqSignal_enum,
    ICC_ReqSignal_enum, // 5
    ICA_ReqSignal_enum,
    ICI_ReqSignal_enum,
    ACC_ReqSignal_enum,
    BELL_ReqSignal_enum,
    C24_ReqSignal_enum, // 10
    IFL_ReqSignal_enum,
    IFB_ReqSignal_enum,
    CUL_ReqSignal_enum,
    CDL_ReqSignal_enum,
    FAN_ReqSignal_enum, // 15
    LIG_ReqSignal_enum, // 16
    StopSw_ReqSignal_enum,
    InsSw_ReqSignal_enum,
    EnSw_ReqSignal_enum,
    IuSw_ReqSignal_enum,    // 20
    IdSw_ReqSignal_enum,
};

enum
{ // 23 inputs, not including the GALX4-0047N board (i_SWix_enum)
    i_RLS_COPi0_enum,
    i_CSS_COPi1_enum,
    i_CTS_COPi2_enum,
    i_DET_COPi3_enum,
    i_CTA_enum,
    i_INS_enum,     // 5
    i_IEN_enum,
    i_IU_enum,
    i_ID_enum,
    i_FANST_enum,
    i_LIGST_enum,   // 10
    i_CDL_T_enum,
    i_CUL_T_enum,
    i_CTS_T_enum,
    i_FFSWT_enum,
    i_CST_T_enum,   // 15
    i_ICC_T_enum,
    i_ICA_T_enum,
    i_ICI_T_enum,
    i_ACC_T_enum,
    i_L15_enum,     // 20
    i_L25_enum,
    i_C24_T_enum,
    i_SWi0_enum,
    i_SWi1_enum,
    i_SWi2_enum,    // 25
    i_SWi3_enum,
    i_SWi4_enum,
    i_SWi5_enum,

    i_Last_enum,    // 29	
};

enum
{
    O_IFL_COPo0_enum, // 0
    O_IFB_COPo1_enum,
    O_CUL_COPo2_enum,
    O_CDL_COPo3_enum,
    O_FAN_COPo4_enum,
    O_LIG_COPo5_enum, // 5
    O_CSS_enum,
    O_C120T_enum, 
    O_L15RL_enum,
    O_L15_1_enum,
    O_FSS_enum, // 10
    O_CST_enum,
    O_ICC_enum,
    O_ICA_enum, 
    O_ICI_enum,
    O_ACC_enum, // 15
    O_BELL_enum,
    O_CLC_enum,
    O_C24_enum,// 18
    O_LDO6_enum,
    O_LDO7_enum, // 20
    O_LDO8_enum,
    O_LDO9_enum,
    O_LDO10_enum,
    O_LDO11_enum,

    O_Last_enum, // 25	
};


enum
{
    i_RLS_COPi0_lable_enum,
    i_CSS_COPi1_lable_enum,
    i_CTS_COPi2_lable_enum,
    i_DET_COPi3_lable_enum,
    i_CTA_lable_enum,
    i_INS_lable_enum,
    i_IEN_lable_enum,
    i_IU_lable_enum,
    i_ID_lable_enum,
    i_FANST_lable_enum,
    i_LIGST_lable_enum,
    i_CDL_T_lable_enum,
    i_CUL_T_lable_enum,
    i_CTS_T_lable_enum,
    i_FFSWT_lable_enum,
    i_CST_T_lable_enum, // 15
    i_ICC_T_lable_enum,
    i_ICA_T_lable_enum,
    i_ICI_T_lable_enum,
    i_ACC_T_lable_enum,
    i_L15_lable_enum,	// 20
    i_L25_lable_enum,
    i_C24_T_lable_enum,
    i_SWi0_lable_enum,
    i_SWi1_lable_enum,
    i_SWi2_lable_enum,
    i_SWi3_lable_enum,
    i_SWi4_lable_enum,
    i_SWi5_lable_enum,	// 28    

    O_IFL_COPo0_lable_enum,
    O_IFB_COPo1_lable_enum, // 30
    O_CUL_COPo2_lable_enum,
    O_CDL_COPo3_lable_enum,
    O_FAN_COPo4_lable_enum,
    O_LIG_COPo5_lable_enum,	// 34
    O_CSS_lable_enum,		// CSS_SEB_OUT,   // 0  CSS    SEB1 IO1; InspectionTest.c (Diag_Labels[] )
    O_C120T_lable_enum, 
    O_L15RL_lable_enum,
    O_L15_lable_1_enum,
    O_FSS_lable_enum,
    O_CST_lable_enum,
    O_ICC_lable_enum,
    O_ICA_lable_enum, 
    O_ICI_lable_enum,
    O_ACC_lable_enum,
    O_BELL_lable_enum,
    O_CLC_lable_enum,
    O_C24_lable_enum,
    O_LDO6_lable_enum,
    O_LDO7_lable_enum,
    O_LDO8_lable_enum,
    O_LDO9_lable_enum,
    O_LDO10_lable_enum,
    O_LDO11_lable_enum,  
    
};

extern const void *SEB_inputs_array[];
extern const void *SEB_outputs_array[];

extern const void *SEB_1_in_array[];
extern const void *SEB_1_out_array[];

extern const void *SEB_2_in_array[];
extern const void *SEB_2_out_array[];

extern const void *SEB_3_in_array[];
extern const void *SEB_3_out_array[];

extern const void *SEB_4_in_array[];
extern const void *SEB_4_out_array[];

extern const void *SEB_1134_COP_in_array[];
extern const void *SEB_1134_COP_out_array[];

extern const void *Extr_1134_COP_in_array[];
extern const void *Extr_1134_COP_out_array[];

extern const void *SEB_0047N_inputs_array[];

extern const void *SEB_debug_out_array[];

extern const char *IO_Labels[];

extern uint32 All_inputs;
extern int16 Delay_Count; 
extern int16 Retest_Option_Parm; // Required for Retest Option
extern int16 RetestResponse; // Required for Retest Option
extern void *The_120VAC_Ptr;
extern int16 IFL_IFB_debug;
extern int16 CtcanTxBufIx;

extern void ClearAllSEB(void);
extern int16 Check_24V(int16 dummy);
extern int16 Check_Car_Light(int16 dummy);
extern int16 Check_Car_Buzzer_or_Bell(int16 Buzzer_or_Bell);
extern int16 set_C120V(int16 dummy);
extern int16 IO_Tests(int16 Which_IO_and_Options);
extern void Read_All_inputs(int16 ShowResults);
extern int16 Evaluate_Results(int16 Which_IO_and_Options);
extern void Display_the_Faults(void);
extern int16 Perform_all_IO_Tests(int16 dummy);
extern int16 Log_IO_Results_for_Analysis(int16 Which_IO);
extern int16 Set_Initial_Conditions(int16 Test);
extern int16 Display_LCD_msgs(int16 Test);
extern int16 Display_new_msgs(int16 Test);
extern int16 check_4_event(int16 Test);
extern int16 Check_Initial_Conditions(int16 Test_with_Options);
extern int16 Turn_Off_IOs(int16 Test_and_options);
extern int16 Generic_Tester(int16 Test);
extern int16 Set_Specific_outputs(int16 Test);
extern int16 Check_final_condition(int16 Test_with_Options);
extern int16 Perform_Switch_tests(int16 Options);
extern int16 Evaluate_The_Results(uint32 The_Expected_Pattern, int16 Which_IO_and_Options );
extern int16 Wait_on_operator(void);
extern int16 Display_new_msgs(int16 Test);
extern int16 Toggle_O_CTxx(int16 channel);
extern int16 Set_or_Clear_All_CTxx(int16 Set_1_Clear_0);
extern int16 Set_or_Clear_All_dbgxx(int16 Set_1_Clear_0);
extern int16 Read_and_Display_rdoutp(int16 dummy);
extern int16 Read_and_Display_Rd_Inputs(int16 dummy);
extern int16 Read_and_Display_All_Inputs(int16 dummy);
extern int16 Standard_Init(int16 Clear_msg_Boards, int16 Stnd_Time_Delay);
extern int16 ClearAllSEB_from_a_Menu(int16 dummy);
extern int16 Set_or_clear_24V_on_SEB4(int16 On_Off);
extern void Un_FirePreReqSignal(void);
extern int16 Set_or_clear_Fire_Light(int16 On_Off);
extern int16 C120_VAC_on_off(int16 on_off);
extern int16 C120_Plus_24_on_off(int16 on_off);
extern int16 GFI_Relay_on_off(int16 on_off);
extern int16 Set_CAN_msgs(int16 CAN_MsgNum);
extern int16 Clear_CAN_msgs(int16 CAN_MsgNum);
extern int16 Read_State_of_0047_input_SWs(int16 dummy);
extern int16 Read_State_of_one_0047_input_SWs(int16 SW);
extern int16 CT_CAN_Enable_bit(int16 enable_disable);
extern int16 Check_to_manualy_Pack_and_send_data(void);
extern void pack_ctcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req);
extern void tx_ctcan_msg(int16 TxBufIx);
extern int16 Read_1134_CAN_input(int16 channel);
extern int16 Set_or_clear_oCSS_on_SEB1(int16 On_Off);
extern int16 Set_or_clear_CN19V(int16 On_Off);
extern int16 Set_or_clear_CN18V(int16 On_Off);
extern int16 Set_or_clear_CLOSE(int16 On_Off);
extern int16 Set_or_clear_OPEN(int16 On_Off);
extern int16 Set_or_clear_NUDGE(int16 On_Off);
extern int16 Set_or_clear_HEAVY(int16 On_Off);
extern int16 Set_or_clear_DCL(int16 On_Off);
extern int16 Set_or_clear_RO(int16 On_Off);
extern int16 Set_or_clear_DOL(int16 On_Off);
extern int16 Send_Failure_msgs_to_msg_brd_A(int16 Prelim_or_Final, int16 Test_with_Options);
extern void Setup_Initial_SEB_States(int16 index_to_Generic_Test);
extern void ClearAll_COP(void);
extern int16 L15_T_on_off(int16 on_off);
extern int16 Set_Clear_C120V_24_L15_T(int16 On_Off);
extern int16 L15RL_on_off(int16 on_off);
extern void Display_Clues(int16 Test_and_Options);
//extern int16 Read_All_1134_SEB(int16 dummy);






