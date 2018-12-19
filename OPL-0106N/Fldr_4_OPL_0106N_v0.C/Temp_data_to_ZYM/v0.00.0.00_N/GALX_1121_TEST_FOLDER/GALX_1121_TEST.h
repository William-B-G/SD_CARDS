//  GALX_1121_TEST.h

extern int16 Forced_outs_on_CPLD1;
extern int16 Monitor_FPGA1;
extern int16 Monitor_unpkFPGA1;
extern int16 Monitor_FPGA2;
extern int16 Monitor_unpkFPGA2;
extern int16 Monitor_NTS;
extern int16 Monitor_unpkNTS;
extern void *i_4[12];
extern void *o_4[12];
extern void *i_3[12];
extern void *o_3[12];
extern int16 UseFPGA1_B0;
extern uint8 RUN_output_cmnd_flag;
extern uint8 SUF_output_cmnd_flag;
extern uint8 SU_output_cmnd_flag;
extern uint8 Functional_1121_Output_Test;
extern uint8 CPLD1_unpk_flg;
extern uint8 CPLD2_unpk_flg;
extern uint8 NTS_unpk_flg;
extern uint8 CPLD1_unpk_ctrl;
extern uint8 CPLD2_unpk_ctrl;
extern uint8 NTS_unpk_ctrl;
extern uint8 Prelim_Done;
extern int16 Msg_Board_index;
extern int16 LogIndex;
extern int16 Optional_Display;
extern void *Virtual_Board_B_OUT[12];
extern int16 zym_diagnostic_mode;  
extern int16 zym_mode_Enter_Button;  
extern int16 zym_mode_On_Off; 
extern int16 Debug_A_wbg ;
extern int16 Debug_B_wbg ;
extern int16 Debug_C_wbg ;
extern int16 CPLD_1_Buff[4];
extern int16 CPLD_2_Buff[4];
extern int16 Input_Test;
extern void *Virtual_Board_A_in[12];
extern void *Virtual_Board_A_OUT[12];
extern void *Virtual_Board_B_in[12];
extern void *Virtual_Board_B_OUT[12];
extern void *Virtual_Board_C_OUT[11];

extern int16 ALL_OFF_ctrl (int16 dummy);
extern int16 TG_SUF_L_ctrl (int16 dummy);
extern int16 TG_UT_L_ctrl (int16 dummy);
extern int16 TG_RUN_L_ctrl (int16 dummy);
extern int16 TG_SU_L_ctrl (int16 dummy);
extern int16 TG_UN_L_ctrl (int16 dummy);
extern int16 TSD_ctrl (int16 Channel);
extern int16 CST_ctrl (int16 dummy);
extern int16 ALL_OFF_ctrl (int16 dummy);
extern int16 Verify_Signal(int16 TheLoop);
extern int16 Verify_the_Inputs_Zero(int16 TheLoop);
extern int16 Verify_No_Short(int16 TheLoop);
extern int16 Preliminary_120VAC(int16 Channel);
extern int16 Force_CPLD1(int16 TheOutputs);
extern int16 Unpack_FPGA_Only(int16 Flags_and_which_fpga);
extern int16 Monitor_Flags_4_FPGAs (int16 Device);
extern int16 Monitor_Flags_4_unpkFPGAs (int16 Device);
extern int16 Get_Num_of_Menus(void);
extern int16 ClearFaults(int16 dummy);
extern int16 All_CPLD2_outputs_ON (int16 dummy);
extern int16 Set_a_BITx(int16 BitNum);
extern int16 Set_Specific_CPLD2_Signal(int16 TheSignal_and_On_Off);
extern int16 Use_FPGA1_HV_B0(int16 YesNo);
extern int16 Verify_1121_OUT(int16 TheLoop);
extern int16 Clear_a_BITx(int16 BitNum);
extern void Clear_FPGA_bits(int16 which);
extern int16 Set_an_NTS_BITx(int16 WhichBit);
extern int16 All_NTS_Bits(int16 On_Off);
extern int16 Set_Input_Module(int16 WhichModule, int16 WhichPort);
extern int16 Cmnd_120VAC(int16 On_Off_and_WhichOutput);
extern void Set_FPGA_bits(int16 which);
extern void Set_1106AN_channels(int16 which);
extern int16 CPLD_Bits(int16 FPGA_x);
extern int16 ParseOutFPGA1_Data(int16 dummy);
extern int16 ParseOutFPGA2_Data(int16 dummy);
extern int16 The_Verify_map_Helper(int16 which, int16 The_Index, int16 In_or_Out) ;
extern int16 Test_ixspi_in_bit(int16 i, int16 Com_to_use);
extern int16 Test_a_spi_bit(int16 buff, int16 j, int16 m);
//extern int16 Verify_Input(int16 Param_and_which_input);
extern int16 Clear_Both_Boards(int16 dummy);
//extern int16 Test_4_Exclusions(int16 i, int16 Standard_excludes_index, int16 Other_excludes_index);
extern int16 DebugHelper(int16 dummy);
extern int16 Set_FPGA_bits_B(int16 On_Off_which_input);
extern int16 Verify_Input_Simple(int16 Param_and_which_input);
extern int16 Verrify_Pattern(int16 Which_Input);
extern int16 TSD_ON_OFF(int16 On_or_Off);
extern int16 SW_TEST(int16 Operation);
extern int16 v120AC_Power_Check(int16 Operation);
extern int16 SEB_IO_TEST(int16 Operation);
extern int16 FPGA1_IO_TEST(int16 Operation);
extern int16 FPGA2_IO_TEST(int16 Operation);
extern int16 TEST_JIG_COMM(int16 Operation);
//-extern int16 SAMPLE_IO_TEST(int16 Operation);
extern int16 Find_elusive_msg(int16 position);
extern int16 Test_Msg_Board_A(int16 Task);
extern int16 Test_Msg_Board_C(int16 Task);
extern int16 Clear_Log_Buffer_C(void);
extern int16 Log_an_event_C(char *msg);
extern int16 UnPack_CPLD2(int16 dummy);
extern int16 Toggle_Display(int16 Toggle);
extern int16 Test_OpM_bufferAdd(int16 number_of_msgs_to_add);
extern int16 Display_Problem(int16 unpk_wrd, int16 exp_wrd, int16 J, int16 FPGA, int16 offset);
extern int16 Get_Bit_Status(int16 FPGA, int16 offset);
extern int16 Build_up_Diff_Array(int16 dummy);
extern int16 Add_Msg_to_C_for_zym_diag(int16 dummy);
extern int16 Compare_and_build(int16 which_item);
extern int16 Demo_OpM(int16 Fail_type_and_index);
extern int16 Flush_OpM(void);
extern int16 Add_OpM(int16 MessageIndex);
extern int16 Init_OpM(void);
extern int16 Test_OpM_bufferAdd(int16 number_of_msgs_to_add);
extern int16 Get_Num_of_Menus(void);
extern int16 Clear_Both_Boards(int16 dummy);
extern int16 ZYM_Diagnostic(int16 item_to_control);
extern void Set_FPGA_bits(int16 which);
extern int16 Set_FPGA_bits_B(int16 On_Off_which_input);
extern int16 TSD_ctrl (int16 Channel);
extern int16 CST_ctrl (int16 dummy);
extern int16 Clear_a_BITx(int16 BitNum);
extern int16 Add_to_MsgBrd_A(int16 dummy);

enum //  WBG step 01
{
    jAUTO, // FPGA1
    jICI= 16,
    jSTE_CPU = jICI + 10,
    
    iDN = 64, // FPGA2
    iDT,
    iUN,
    iUT,
    iSD,
    iSDF,
    iTSD = iDN + 13,
    iCEN ,
    iL120= iDN + 16,
    iBAU,
    iBAD,
    iTAU,
    iTAD,    
    iRESET= iDN + 32,
    iMRID= iDN + 42,
    iMRIE,
    iMRIU,
    iMRIN,
    iCTIN= iDN + 48,
    iCST,
    iCFLT,
    iSPD,
    iMTO,    
    iMC= iDN + 53,
    iAUTO,
    iAD,
    iIND =  iDN + 56,
    iLBP,
    iGLB,
    iGBP,    
    iMRSW,    
    iSUF,
    iSU,
    iDZ ,
    
    kDLT=128,
    kRLM,
    kDLM,
    kDLB,
    kLPS,
    kLOS,
    kTPL,
    kTPH,
    kTLH,
    kPS,    
    kS10_H24,
    kGOV, // -H22
    kHSS
};
/*
struct Menus_in_System {
    // This info gets pushed onto the menu stack 
    // with a call to "Start_a_Menu(WhichMenu)".  (WBG)
    // Handles sub-menus up to 23 deep.
    void *MenuPtr;
    int16 LengthOfMenu;
};

extern struct Menus_in_System The_Menus_in_System[15];

*/




//------------------------------------------------------------------------------
//ZYM 1-22-2018
extern int Check_Input_Shorted(void); 
extern void Exclude_Short_Signal(int16 Exclude_Signal);
extern void Include_Short_Signal(int16 Include_Signal);
extern void Turn_Signal_ON(int16 Signal_ON);
extern void Turn_Signal_OFF(int16 Signal_OFF);
extern int16 Check_Signal_ON(int16 Signal_ON);
extern int16 Check_Signal_OFF(int16 Signal_OFF);
extern void Disply_Fault(void);
extern void Reset_Fault_Type_Index(void);
extern void Turn_ALL_Output_OFF(void);
extern void Include_ALL_Short_Signal(void);




//------------------------------------------------------------------------------
//Rename All Input and New Index
enum
{
	//SEB
	i_suf_0=0,					//0				
    i_sufc_1,					//1				
    i_su_2,						//2				
    i_suc_3,					//3				
    i_sdf_4,					//4				
    i_sdfc_5,					//5				
    i_sd_6,						//6				
    i_sdc_7,					//7				
    i_s6s10_8,					//8				
    i_s2mrw_9,					//9				
	i_24s10_10,					//10			
	i_22gov_11,					//11			
	i_20tlh_12,					//12			
	i_h18ps_13,					//13			
	i_16mrw_14,					//14				
	i_14mrw_15,					//15			
	i_11mrw_16,					//16			
	i_h8s10_17,					//17			
	i_h6s10_18,					//18			
	i_h4s10_19,					//19			
	i_h2s10_20,					//20			
	i_mrsw_21,					//21			
	i_mc_22,					//22
	i_mc1_23,					//23			
	i_ste_24,					//24			
	i_s10_25,					//25			
	i_18hss_26,					//26
	
	//FPGA1 Buff0
	ACC_iF1B0_27,				//27	//bit 0			
    GS_iF1B0_28,				//28				
    RGS_iF1B0_29,				//29				
    LPS_iF1B0_30,				//30				
    RLM_iF1B0_31,				//31				
    DLT_iF1B0_32,				//32				
    DLM_iF1B0_33,				//33				
    DLB_iF1B0_34,				//34				
    TAD_iF1B0_35,				//35				
    TAU_iF1B0_36,				//36				
	BAD_iF1B0_37,				//37			
	BAU_iF1B0_38,				//38			
	GBP_iF1B0_39,				//39			
	GLB_iF1B0_40,				//40			
	LBP_iF1B0_41,				//41				
	AUTO_iF1B0_42,				//42	//bit 15	
	
	//FPGA1 Buff1			
	UL_CPU_iF1B1_43,			//43	//bit 0			
	DNF_CPU_iF1B1_44,			//44 //SAME AS SDF			
	UPF_CPU_iF1B1_45,			//45 //SAME AS SUF			
	UPR_CPU_iF1B1_46,			//46 //SAME AS SU			
	DNR_CPU_iF1B1_47,			//47 //SAME AS SD			
	STE_CPU_iF1B1_48,			//48			
	MC_CPU_iF1B1_49,			//49			
	CEN_CPU_iF1B1_50,			//50 //SAME AS MC1
	UNUSED0_iF1B1_51,			//51 *UNUSED
	MRID_iF1B1_52,				//52			
	MRIU_iF1B1_53,				//53
	MRI_iF1B1_54,				//54	
	ID_iF1B1_55,				//55
	IU_iF1B1_56,				//56
	CTIN_iF1B1_57,				//57 //SAME AS INS
	ICI_iF1B1_58,				//58	//bit 15
	
	//FPGA1 Buff2
	PSTE_iF1B2_59,				//59	//bit 0	  *DONT_CARE
	PDN_iF1B2_60,				//60 *DONT_CARE
	PUN_iF1B2_61,				//61 *DONT_CARE
	PUT_iF1B2_62,				//62 *DONT_CARE
	PDT_iF1B2_63,				//63 *DONT_CARE
	PSD_iF1B2_64,				//64 *DONT_CARE
	PSU_iF1B2_65,				//65 *DONT_CARE
	PSUF_iF1B2_66,				//66 *DONT_CARE
	PSDF_iF1B2_67,				//67 *DONT_CARE
	RUN_CPU_iF1B2_68,			//68
	SEL_OK_CPU_iF1B2_69,		//69
	SPD_75_CPU_iF1B2_70,		//70
	SPD_150_CPU_iF1B2_71,		//71
	DZ_PAL_iF1B2_72,			//72
	DL_CPU_iF1B2_73, 			//73
	DZ_CPU_iF1B2_74,			//74	//bit 15
	
	//FPGA1 Buff3
	UNUSED1_iF1B3_75,			//75	//bit 0	 *UNUSED
	TEST_MODE_iF1B3_76,			//76
	REAR_DOOR_iF1B3_77, 		//77
	NTS_FLT_iF1B3_78, 			//78
	SPD_75_NTS_iF1B3_79, 		//79
	SPD_150_NTS_iF1B3_80, 		//80
	SEL_OK_NTS_iF1B3_81,		//81
	DZ_NTS_iF1B3_82,			//82
	DN_NTS_iF1B3_83, 			//83
	UN_NTS_iF1B3_84,			//84
	UT_NTS_iF1B3_85,			//85
	DT_NTS_iF1B3_86, 			//86
	PAL_FLT_iF1B3_87,			//87
	PRUN_iF1B3_88,				//88 *DONT_CARE
	PCEN_iF1B3_89,				//89 *DONT_CARE
	PMC_iF1B3_90,				//90	//bit 15 *DONT_CARE
	
	//FPGA2 Buff0												
	L120B_iF2B0_91,				//91	//bit 0		
    CEN_iF2B0_92,				//92 //SAME AS MC1			
    TSD_iF2B0_93,				//93				
    HSS_iF2B0_94,				//94				
    GOV_iF2B0_95,				//95				
    S10_iF2B0_96,				//96				
    TPH_iF2B0_97,				//97				
    TPL_iF2B0_98,				//98				
    LOS_iF2B0_99,				//99				
    LPS_iF2B0_100,				//100				
	SDF_iF2B0_101,				//101			
	SD_iF2B0_102,				//102			
	UT_iF2B0_103,				//103			
	UN_iF2B0_104,				//104			
	DT_iF2B0_105,				//105				
	DN_iF2B0_106,				//106	//bit 15
	
	//FPGA2 Buff1			
	ACC_iF2B1_107,				//107	//bit 0		
	REAR_DOOR_iF2B1_108,		//108			
	TEST_MODE_iF2B1_109,		//109			
	RLM1_iF2B1_110,				//110			
	RLM_iF2B1_111,				//111			
	DLT1_iF2B1_112,				//112			
	DLT_iF2B1_113,				//113			
	DLM1_iF2B1_114,				//114 
	DLM_iF2B1_115,				//115			
	DLB1_iF2B1_116,				//116			
	DLB_iF2B1_117,				//117
	TAD_iF2B1_118,				//118	
	TAU_iF2B1_119,				//119
	BAD_iF2B1_120,				//120
	BAU_iF2B1_121,				//121
	L120_iF2B1_122,				//122	//bit 15
	
	//FPGA2 Buff2
	IU_iF2B2_123,				//123	//bit 0 	 
	ID_iF2B2_124,				//124
	MRIN_iF2B2_125,				//125 //SAME AS MRI
	MRIU_iF2B2_126,				//126
	MRIE_iF2B2_127,				//127
	MRID_iF2B2_128,				//128
	RUN_iF2B2_129,				//129
	IEN_iF2B2_130,				//130
	ICI_iF2B2_131,				//131
	RGS1_iF2B2_132,				//132
	RGS_iF2B2_133,				//133
	GS1_iF2B2_134,				//134
	GS_iF2B2_135,				//135
	NTS_FLT_iF2B2_136,			//136
	PAL_FLT_iF2B2_137, 			//137
	RESET_iF2B2_138,			//138	//bit 15
	
	//FPGA2 Buff3
	DZ_iF2B3_139,				//139 	//bit 0  //SAME AS DZ_PAL
	SU_iF2B3_140,				//140	
	SUF_iF2B3_141, 				//141
	MRSW_iF2B3_142, 			//142
	GBP_iF2B3_143, 				//143
	GLB_iF2B3_144, 				//144
	LBP_iF2B3_145,				//145
	IND_iF2B3_146,				//146
	AD_iF2B3_147, 				//147
	AUTO_iF2B3_148,				//148
	MC_iF2B3_149,				//149
	MTO_iF2B3_150, 				//150
	SPD_iF2B3_151,				//151
	CFLT_iF2B3_152,				//152
	CST_iF2B3_153,				//153
	CTIN_iF2B3_154,				//154 	//bit 15 //SAME AS INS	
	
	//NTS Buff0
	TEST_MODE_iNTSB0_155,		//155	//bit 0	
	glax1121_ALL_input_io_num	//156		
};	








//------------------------------------------------------------------------------
//Rename All Output and New Index
enum
{
	//SEB
	o_S5LPS_0=0,				//0
	o_S4LOS_1,					//1			 
    o_S3TPL_2,					//2			 
    o_S1TPH_3,					//3			 
    o_23GOV_4,					//4			 
    o_21TLH_5,					//5			 
    o_H19PS_6,					//6			 
    o_17HSS_7,					//7			 
    o_15TSD_8,					//8			 
    o_13BAU_9,					//9			 
	o_12BAD_10,					//10		 
	o_10TAU_11,					//11			 
	o_H9TAD_12,					//12			 
	o_H7DLB_13,					//13		 
	o_H5DLM_14,					//14		 	
	o_H3DLT_15,					//15		 
	o_RLM_16,					//16		 
//-	o_MC1_17,					//17		 
	o_0MTO_17,					//17		 
	o_0SPD_18,					//18		 
	o_0CFLT_19,					//19
	o_TMRLY_20,					//20		 
	o_17CST_21,					//21		 
	o_16INS_22,					//22		 
	o_T15IU_23,					//23		 
	o_T14ID_24,					//24		 
	o_13IEN_25,					//25
	o_12ICA_26,					//26
	o_11ICI_27,					//27
	o_10ACC_28,					//28
	o_T9GS_29,					//29
	o_RGS_30,					//30
	o_RDRLY_31,					//31
	
	//FPGA1 Buff0
	PSU_LP_oF1B0_32,			//32	//bit 0
	PSD_LP_oF1B0_33,			//33	
	PSUF_LP_oF1B0_34,			//34	
	PSDF_LP_oF1B0_35,			//35	
	PUN_LP_oF1B0_36,			//36	
	PDN_LP_oF1B0_37,			//37	
	PUT_LP_oF1B0_38,			//38	
	PDT_LP_oF1B0_39,			//39	
	PRUN_LP_oF1B0_40,			//40	
	PCEN_LP_oF1B0_41,			//41	SAME AS MC1
	PSTE_LP_oF1B0_42,			//42	
	PMC_LP_oF1B0_43,			//43	
	CPLD1_SPI_TEST_bit_oF1B0_44,//44 *NO_Output_TYPE	
	DZ_PAL_oF1B0_45,			//45
	UNUSED0_oF1B0_46,			//46 *UNUSED
	UNUSED1_oF1B0_47,			//47	//bit 15  *UNUSED	
	
	//FPGA2 Buff0
	LDL_CPU_oF2B0_48,			//48 	//bit 0		DL_CPU
    DZ_CPU_oF2B0_49,			//49
    LUL_CPU_oF2B0_50,			//50 UL_CPU
    SD_CPU_oF2B0_51,			//51 DNR_CPU
    SU_CPU_oF2B0_52,			//52 UPR_CPU  
    SUF_CPU_oF2B0_53,			//53 UPF_CPU   
    SDF_CPU_oF2B0_54,			//54 DNF_CPU   
    RUN_CPU_oF2B0_55,			//55    
    CEN_CPU_oF2B0_56,			//56 MC1_CPU   
    MC_CPU_oF2B0_57,			//57    
    STE_CPU_oF2B0_58,			//58    
    SELOK_CPU_oF2B0_59,			//59    
	SPDLEV_CPU_oF2B0_60,		//60 SPD_LEV_CPU  *NO_Output_TYPE
	SPD75_CPU_oF2B0_61,			//61
	SPD150_CPU_oF2B0_62,		//62	 
	CPLD2_SPI_TEST_bit_oF2B0_63,//63	//bit 15  *NO_Output_TYPE
	
	//NTS SPI Output Buff0
	UT_NTS_SPIout_64,			//64	//bit 0
	UN_NTS_SPIout_65,			//65
	DT_NTS_SPIout_66,			//66
	DN_NTS_SPIout_67,			//67
	UNUSED0_SPIout_68,			//68 *UNUSED
	UNUSED1_SPIout_69,			//69 *UNUSED
	UNUSED2_SPIout_70,			//70 *UNUSED
	UNUSED3_SPIout_71,			//71	//bit 8	  *UNUSED	
	
	//NTS CAN Output Buff0
	DZ_NTS_CANout_72,			//72	//bit 0
	SPD_150_NTS_CANout_73,		//73
	SPD_75_NTS_CANout_74,		//74
	NTS_FLT_CANout_75,			//75
	SEL_OK_NTS_CANout_76,		//76
	PAL_FAULT_CANout_77,		//77
	UNUSED0_CANout_78,			//78 *UNUSED
	UNUSED1_CANout_79,			//79	//bit 8  *UNUSED
	glax1121_ALL_output_io_num  //80
}; 



extern char clr_dplyline[1][21];
extern void GALX1121_Fault_Display_ADDIN_Function(void);

























