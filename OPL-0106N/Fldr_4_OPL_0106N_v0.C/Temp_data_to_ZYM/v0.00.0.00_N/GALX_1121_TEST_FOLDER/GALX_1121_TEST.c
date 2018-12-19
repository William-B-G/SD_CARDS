#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_global.h"
#include "cedes_can.h"




int Check_Input_Shorted(void); 
void Exclude_Short_Signal(int16 Exclude_Signal);
void Include_Short_Signal(int16 Include_Signal);
void Turn_Signal_ON(int16 Signal_ON);
void Turn_Signal_OFF(int16 Signal_OFF);
int16 Check_Signal_ON(int16 Signal_ON);
int16 Check_Signal_OFF(int16 Signal_OFF);
void Disply_Fault(void);
void Reset_Fault_Type_Index(void);
void Turn_ALL_Output_OFF(void);
void Include_ALL_Short_Signal(void);
void GALX1121_Fault_Display_ADDIN_Function(void);


extern int16 return_CPLD1_SPI_TEST_byte;
extern int16 return_CPLD2_SPI_TEST_byte;

extern int32 test_msg;
extern int32 test_msg1;
extern int32 test_msg2;
extern int32 test_msg3;
extern int32 test_msg4;







//------------------------------------------------------------------------------
                  //Bit0     1      2      3      4      5      6      7      
uint16 BIT_16[16]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,

				  //Bit8     9     10     11	 12     13     14     15
				   0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
				   
				   
extern unsigned char BIT[8];


char clr_dplyline[1][21]={"                    "};

      

//SEB Input Index
enum
{	
	I_suf,						//0				 
    I_sufc,						//1			 
    I_su,						//2			 
    I_suc,						//3			 
    I_sdf,						//4			 
    I_sdfc,						//5			 
    I_sd,						//6			 
    I_sdc,						//7			 
    I_s6s10,					//8			 
    I_s2mrw,					//9			 
	I_24s10,					//10		 
	I_22gov,					//11			 
	I_20tlh,					//12			 
	I_h18ps,					//13		 
	I_16mrw,					//14		 	
	I_14mrw,					//15		 
	I_11mrw,					//16		 
	I_h8s10,					//17		 
	I_h6s10,					//18		 
	I_h4s10,					//19		 
	I_h2s10,					//20		 
	I_mrsw,						//21		 
	I_mc,						//22
	I_mc1,		                //23
	I_ste,						//24		 
	I_s10,						//25		 
	I_18hss,					//26																
	glax1121_SEB_input_io_num	//27
};

//SEB Input Name
char SEB_Input_Name[glax1121_SEB_input_io_num][21] = 
{
	"i_suf_0             ",	    //0
	"i_sufc_1            ",	    //1
	"i_su_2              ",	    //2
	"i_suc_3             ",	    //3
	"i_sdf_4             ",	    //4
	"i_sdfc_5            ",	    //5
	"i_sd_6              ",	    //6
	"i_sdc_7             ",	    //7
	"i_s6s10_8           ",	    //8
	"i_s2mrw_9           ",	    //9
	"i_24s10_10          ",	    //10
	"i_22gov_11          ",	    //11
	"i_20tlh_12          ",	    //12
	"i_h18ps_13          ",	    //13
	"i_16mrw_14          ",	    //14
	"i_14mrw_15          ",	    //15
	"i_11mrw_16          ",	    //16
	"i_h8s10_17          ",	    //17
	"i_h6s10_18          ",	    //18
	"i_h4s10_19          ",	    //19
	"i_h2s10_20          ",	    //20
	"i_mrsw_21           ",	    //21
	"i_mc_22             ",	    //22
	"i_mc1_23            ",	    //23 
	"i_ste_24            ",	    //24
	"i_s10_25            ",	    //25
	"i_18hss_26          ",	    //26	
};



//SEB Input Signal  
void *glax1121_SEB_input_io_array[glax1121_SEB_input_io_num]= 
{										 		
    i_suf,						//0				 
    i_sufc,						//1			 
    i_su,						//2			 
    i_suc,						//3			 
    i_sdf,						//4			 
    i_sdfc,						//5			 
    i_sd,						//6			 
    i_sdc,						//7			 
    i_s6s10,					//8			 
    i_s2mrw,					//9			 
	i_24s10,					//10		 
	i_22gov,					//11			 
	i_20tlh,					//12			 
	i_h18ps,					//13		 
	i_16mrw,					//14		 	
	i_14mrw,					//15		 
	i_11mrw,					//16		 
	i_h8s10,					//17		 
	i_h6s10,					//18		 
	i_h4s10,					//19		 
	i_h2s10,					//20		 
	i_mrsw,						//21		 
	i_mc,						//22
	i_mc1,						//23		 
	i_ste,						//24		 
	i_s10,						//25		 
	i_18hss						//26		 
};



//SEB Output Index
enum
{	
	O_S5LPS,					//0				 
    O_S4LOS,					//1			 
    O_S3TPL,					//2			 
    O_S1TPH,					//3			 
    O_23GOV,					//4			 
    O_21TLH,					//5			 
    O_H19PS,					//6			 
    O_17HSS,					//7			 
    O_15TSD,					//8			 
    O_13BAU,					//9			 
	O_12BAD,					//10		 
	O_10TAU,					//11			 
	O_H9TAD,					//12			 
	O_H7DLB,					//13		 
	O_H5DLM,					//14		 	
	O_H3DLT,					//15		 
	O_RLM,						//16		 
	O_0MTO,						//17		 
	O_0SPD,						//18		 
	O_0CFLT,					//19
	O_TMRLY,					//20		 
	O_17CST,					//21		 
	O_16INS,					//22		 
	O_T15IU,					//23		 
	O_T14ID,					//24		 
	O_13IEN,					//25
	O_12ICA,					//26
	O_11ICI,					//27
	O_10ACC,					//28
	O_T9GS,						//29
	O_RGS,						//30	 
	O_RDRLY,					//31													
	glax1121_SEB_output_io_num	//32
};


//SEB Output Signal  
void *glax1121_SEB_output_io_array[glax1121_SEB_output_io_num]= 
{										 		
    o_S5LPS,					//0				 
    o_S4LOS,					//1			 
    o_S3TPL,					//2			 
    o_S1TPH,					//3			 
    o_23GOV,					//4			 
    o_21TLH,					//5			 
    o_H19PS,					//6			 
    o_17HSS,					//7			 
    o_15TSD,					//8			 
    o_13BAU,					//9			 
	o_12BAD,					//10		 
	o_10TAU,					//11			 
	o_H9TAD,					//12			 
	o_H7DLB,					//13		 
	o_H5DLM,					//14		 	
	o_H3DLT,					//15		 
	o_RLM,						//16		 
	o_0MTO,						//17		 
	o_0SPD,						//18		 
	o_0CFLT,					//19
	o_TMRLY,					//20		 
	o_17CST,					//21		 
	o_16INS,					//22		 
	o_T15IU,					//23		 
	o_T14ID,					//24		 
	o_13IEN,					//25
	o_12ICA,					//26
	o_11ICI,					//27
	o_10ACC,					//28
	o_T9GS,						//29
	o_RGS,						//30
	o_RDRLY	 					//31
};



//---------------------------------------
//FPGA1 SPI Input Index
enum
{	
	//Buff0	
	ACC_iF1B0=0,				//0		//bit 0
	GS_iF1B0,					//1	
	RGS_iF1B0,					//2
	LPS_iF1B0,					//3
	RLM_iF1B0,					//4	
	DLT_iF1B0,					//5
	DLM_iF1B0,					//6
	DLB_iF1B0,					//7
	TAD_iF1B0,					//8	
	TAU_iF1B0,					//9
	BAD_iF1B0,					//10	
	BAU_iF1B0,					//11	
	GBP_iF1B0,					//12	
	GLB_iF1B0,					//13	
	LBP_iF1B0,					//14	
	AUTO_iF1B0,					//15	//bit 15																																	    				    			    			    			    				    				    				    			    																										
	
	//Buff1			
	UL_CPU_iF1B1,				//16	//bit 0	
	DNF_CPU_iF1B1,				//17 //SAME AS SDF
	UPF_CPU_iF1B1,				//18 //SAME AS SUF
	UPR_CPU_iF1B1,				//19 //SAME AS SU
	DNR_CPU_iF1B1,				//20 //SAME AS SD
	STE_CPU_iF1B1,				//21
	MC_CPU_iF1B1,				//22
	CEN_CPU_iF1B1,				//23 //SAME AS MC1_CPU
	UNUSED0_iF1B1,				//24 *UNUSED
	MRID_iF1B1,					//25
	MRIU_iF1B1,					//26
	MRI_iF1B1,					//27
	ID_iF1B1,					//28
	IU_iF1B1,					//29
	CTIN_iF1B1,					//30 //SAME AS INS
	ICI_iF1B1,					//31	//bit 15
																																					 														
	//Buff2
	PSTE_iF1B2,					//32	//bit 0 	SAME AS STE	 *DONT_CARE													
	PDN_iF1B2,					//33 *DONT_CARE
	PUN_iF1B2,					//34 *DONT_CARE
	PUT_iF1B2,					//35 *DONT_CARE
	PDT_iF1B2,					//36 *DONT_CARE
	PSD_iF1B2,					//37 *DONT_CARE
	PSU_iF1B2,					//38 *DONT_CARE
	PSUF_iF1B2,					//39 *DONT_CARE
	PSDF_iF1B2,					//40 *DONT_CARE
	RUN_CPU_iF1B2,				//41
	SEL_OK_CPU_iF1B2,			//42
	SPD_75_CPU_iF1B2,			//43
	SPD_150_CPU_iF1B2,			//44
	DZ_PAL_iF1B2,				//45
	DL_CPU_iF1B2, 				//46
	DZ_CPU_iF1B2,				//47	//bit 15								
			
	//Buff3
	UNUSED1_iF1B3,				//48	//bit 0  *UNUSED																
	TEST_MODE_iF1B3,			//49	
	REAR_DOOR_iF1B3, 			//50
	NTS_FLT_iF1B3, 				//51
	SPD_75_NTS_iF1B3, 			//52
	SPD_150_NTS_iF1B3, 			//53
	SEL_OK_NTS_iF1B3,			//54
	DZ_NTS_iF1B3,				//55
	DN_NTS_iF1B3, 				//56
	UN_NTS_iF1B3,				//57
	UT_NTS_iF1B3,				//58
	DT_NTS_iF1B3, 				//59
	PAL_FLT_iF1B3,				//60
	PRUN_iF1B3,					//61 *DONT_CARE
	PCEN_iF1B3,					//62 *DONT_CARE			
	PMC_iF1B3,					//63	//bit 15 *DONT_CARE	
	glax1121_FPGA1_input_io_num	//64	
};


//FPGA1 SPI Input Name
char FPGA1_Input_Name[glax1121_FPGA1_input_io_num][20] = 
{
	//Buff0
	"ACC_iF1B0_27        ",	    //0		//bit 0															
	"GS_iF1B0_28         ",	    //1
	"RGS_iF1B0_29        ",	    //2
	"LPS_iF1B0_30        ",	    //3
	"RLM_iF1B0_31        ",	    //4
	"DLT_iF1B0_32        ",	    //5
	"DLM_iF1B0_33        ",	    //6
	"DLB_iF1B0_34        ",	    //7
	"TAD_iF1B0_35        ",	    //8
	"TAU_iF1B0_36        ",	    //9
	"BAD_iF1B0_37        ",	    //10
	"BAU_iF1B0_38        ",	    //11
	"GBP_iF1B0_39        ",	    //12
	"GLB_iF1B0_40        ",	    //13
	"LBP_iF1B0_41        ",	    //14
	"AUTO_iF1B0_42       ",	    //15	//bit 15
		
	//Buff1
	"UL_CPU_iF1B1_43     ",	    //16	//bit 0
	"DNF_CPU_iF1B1_44    ",	    //17 //SAME AS SDF	
	"UPF_CPU_iF1B1_45    ",	    //18 //SAME AS SUF
	"UPR_CPU_iF1B1_46    ",	    //19 //SAME AS SU
	"DNR_CPU_iF1B1_47    ",	    //20 //SAME AS SD
	"STE_CPU_iF1B1_48    ",	    //21
	"MC_CPU_iF1B1_49     ",	    //22
	"CEN_CPU_iF1B1_50    ",	    //23 //SAME AS MC1
	"UNUSED0_iF1B1_51    ",		//24 *UNUSED  
	"MRID_iF1B1_52       ",	    //25
	"MRIU_iF1B1_53       ",	    //26
	"MRI_iF1B1_54        ",	    //27
	"ID_iF1B1_55         ",	    //28
	"IU_iF1B1_56         ",	    //29
	"CTIN_iF1B1_57       ",	    //30	
	"ICI_iF1B1_58        ",	    //31	//bit 15
															
	//Buff2
	"PSTE_iF1B2_59       ",	    //32	//bit 0   SAME AS STE	 *DONT_CARE
	"PDN_iF1B2_60        ",	    //33 *DONT_CARE	
	"PUN_iF1B2_61        ",	    //34 *DONT_CARE
	"PUT_iF1B2_62        ",	    //35 *DONT_CARE
	"PDT_iF1B2_63        ",	    //36 *DONT_CARE
	"PSD_iF1B2_64        ",	    //37 *DONT_CARE
	"PSU_iF1B2_65        ",	    //38 *DONT_CARE
	"PSUF_iF1B2_66       ",	    //39 *DONT_CARE
	"PSDF_iF1B2_67       ",	    //40 *DONT_CARE
	"RUN_CPU_iF1B2_68    ",	    //41
	"SEL_OK_CPU_iF1B2_69 ",	    //42
	"SPD_75_CPU_iF1B2_70 ",	    //43
	"SPD_150_CPU_iF1B2_71",	    //44
	"DZ_PAL_iF1B2_72     ",	    //45
	"DL_CPU_iF1B2_73     ",	    //46	
	"DZ_CPU_iF1B2_74     ",	    //47	//bit 15
																		
	//FPGA1 Buff3
	"UNUSED1_iF1B3_75    ",		//48    //bit 0  *UNUSED																		
	"TEST_MODE_iF1B3_76  ",	    //49	
	"REAR_DOOR_iF1B3_77  ",	    //50
	"NTS_FLT_iF1B3_78    ",	    //51
	"SPD_75_NTS_iF1B3_79 ",	    //52
	"SPD_150_NTS_iF1B3_80",	    //53
	"SEL_OK_NTS_iF1B3_81 ",	    //54
	"DZ_NTS_iF1B3_82     ",	    //55
	"DN_NTS_iF1B3_83     ",	    //56
	"UN_NTS_iF1B3_84     ",	    //57
	"UT_NTS_iF1B3_85     ",	    //58
	"DT_NTS_iF1B3_86     ",	    //59
	"PAL_FLT_iF1B3_87    ",	    //60
	"PRUN_iF1B3_88       ",	    //61 *DONT_CARE
	"PCEN_iF1B3_89       ",	    //62 *DONT_CARE	
	"PMC_iF1B3_90        ",	    //63	//bit 15 *DONT_CARE
};



//FPGA1 SPI Output Index
enum
{	
	//Buff0												
	PSU_LP_oF1B0=0,				//0		//bit 0	
	PSD_LP_oF1B0,				//1	
	PSUF_LP_oF1B0,				//2	
	PSDF_LP_oF1B0,				//3	
	PUN_LP_oF1B0,				//4	
	PDN_LP_oF1B0,				//5	
	PUT_LP_oF1B0,				//6	
	PDT_LP_oF1B0,				//7	
	PRUN_LP_oF1B0,				//8	
	PCEN_LP_oF1B0,				//9		SAME AS MC1
	PSTE_LP_oF1B0,				//10	
	PMC_LP_oF1B0,				//11	
	CPLD1_SPI_TEST_bit_oF1B0,	//12 *NO_Output_TYPE	
	DZ_PAL_oF1B0,				//13
	UNUSED0_oF1B0,				//14 *UNUSED
	UNUSED1_oF1B0,				//15	//bit 15  *UNUSED
	glax1121_FPGA1_output_io_num//16
};
	


//---------------------------------------
//FPGA2 SPI Input Index
enum
{
	//Buff0												
	L120B_iF2B0=0,				//0		//bit 0		
    CEN_iF2B0,					//1	//SAME AS MC1			
    TSD_iF2B0,					//2				
    HSS_iF2B0,					//3				
    GOV_iF2B0,					//4				
    S10_iF2B0,					//5				
    TPH_iF2B0,					//6				
    TPL_iF2B0,					//7				
    LOS_iF2B0,					//8				
    LPS_iF2B0,				    //9				
	SDF_iF2B0,					//10			
	SD_iF2B0,					//11			
	UT_iF2B0,					//12			
	UN_iF2B0,					//13			
	DT_iF2B0,					//14				
	DN_iF2B0,					//15	//bit 15
	
	//Buff1			
	ACC_iF2B1,					//16	//bit 0		
	REAR_DOOR_iF2B1,			//17			
	TEST_MODE_iF2B1,			//18			
	RLM1_iF2B1,					//19			
	RLM_iF2B1,					//20			
	DLT1_iF2B1,					//21			
	DLT_iF2B1,					//22			
	DLM1_iF2B1,					//23 
	DLM_iF2B1,					//24			
	DLB1_iF2B1,					//25			
	DLB_iF2B1,					//26
	TAD_iF2B1,					//27	
	TAU_iF2B1,					//28
	BAD_iF2B1,					//29
	BAU_iF2B1,					//30
	L120_iF2B1,					//31	//bit 15
	
	//Buff2
	IU_iF2B2,					//32	//bit 0 	 
	ID_iF2B2,					//33
	MRIN_iF2B2,					//34
	MRIU_iF2B2,					//35
	MRIE_iF2B2,					//36
	MRID_iF2B2,					//37
	RUN_iF2B2,					//38
	IEN_iF2B2,					//39
	ICI_iF2B2,					//40
	RGS1_iF2B2,					//41
	RGS_iF2B2,					//42
	GS1_iF2B2,					//43
	GS_iF2B2,					//44
	NTS_FLT_iF2B2,				//45
	PAL_FLT_iF2B2, 				//46
	RESET_iF2B2,				//47	//bit 15
	
	//Buff3
	DZ_iF2B3,					//48	//bit 0
	SU_iF2B3,					//49	
	SUF_iF2B3, 					//50
	MRSW_iF2B3, 				//51
	GBP_iF2B3, 					//52
	GLB_iF2B3, 					//53
	LBP_iF2B3,					//54
	IND_iF2B3,					//55
	AD_iF2B3, 					//56
	AUTO_iF2B3,					//57
	MC_iF2B3,					//58
	MTO_iF2B3, 					//59
	SPD_iF2B3,					//60
	CFLT_iF2B3,					//61
	CST_iF2B3,					//62
	CTIN_iF2B3,					//63	//bit 15		
	glax1121_FPGA2_input_io_num	//64		
};



//FPGA2 SPI Input Name
char FPGA2_Input_Name[glax1121_FPGA2_input_io_num][20] = 
{
	//Buff0
	"L120B_iF2B0_91      ",	    //0		//bit 0
	"CEN_iF2B0_92        ",	    //1 //SAME AS MC1
	"TSD_iF2B0_93        ",	    //2
	"HSS_iF2B0_94        ",	    //3
	"GOV_iF2B0_95        ",	    //4
	"S10_iF2B0_96        ",	    //5
	"TPH_iF2B0_97        ",	    //6
	"TPL_iF2B0_98        ",	    //7
	"LOS_iF2B0_99        ",	    //8
	"LPS_iF2B0_100       ",	    //9
	"SDF_iF2B0_101       ",	    //10
	"SD_iF2B0_102        ",	    //11
	"UT_iF2B0_103        ",	    //12
	"UN_iF2B0_104        ",	    //13
	"DT_iF2B0_105        ",	    //14
	"DN_iF2B0_106        ",	    //15	//bit 15
	
	//Buff1
	"ACC_iF2B1_107       ",	    //16	//bit 0
	"REAR_DOOR_iF2B1_108 ",	    //17
	"TEST_MODE_iF2B1_109 ",	    //18
	"RLM1_iF2B1_110      ",	    //19
	"RLM_iF2B1_111       ",	    //20
	"DLT1_iF2B1_112      ",	    //21
	"DLT_iF2B1_113       ",	    //22
	"DLM1_iF2B1_114      ",	    //23
	"DLM_iF2B1_115       ",	    //24
	"DLB1_iF2B1_116      ",	    //25
	"DLB_iF2B1_117       ",	    //26
	"TAD_iF2B1_118       ",	    //27
	"TAU_iF2B1_119       ",	    //28
	"BAD_iF2B1_120       ",	    //29
	"BAU_iF2B1_121       ",	    //30
	"L120_iF2B1_122      ",	    //31	//bit 15
	
	//Buff2
	"IU_iF2B2_123        ",	    //32	//bit 0
	"ID_iF2B2_124        ",	    //33
	"MRIN_iF2B2_125      ",	    //34
	"MRIU_iF2B2_126      ",	    //35
	"MRIE_iF2B2_127      ",	    //36
	"MRID_iF2B2_128      ",	    //37
	"RUN_iF2B2_129       ",	    //38
	"IEN_iF2B2_130       ",	    //39
	"ICI_iF2B2_131      ",	    //40
	"RGS1_iF2B2_132      ",	    //41
	"RGS_iF2B2_133       ",	    //42
	"GS1_iF2B2_134       ",	    //43
	"GS_iF2B2_135        ",	    //44
	"NTS_FLT_iF2B2_136   ",	    //45
	"PAL_FLT_iF2B2_137   ",	    //46
	"RESET_iF2B2_138     ",	    //47	//bit 15
	
	//Buff3		
	"DZ_iF2B3_139        ",	    //48	//bit 0
	"SU_iF2B3_140        ",	    //49
	"SUF_iF2B3_141       ",	    //50
	"MRSW_iF2B3_142      ",	    //51
	"GBP_iF2B3_143       ",	    //52
	"GLB_iF2B3_144       ",	    //53
	"LBP_iF2B3_145       ",	    //54
	"IND_iF2B3_146       ",	    //55
	"AD_iF2B3_147        ",	    //56
	"AUTO_iF2B3_148      ",	    //57
	"MC_iF2B3_149        ",	    //58
	"MTO_iF2B3_150       ",	    //59
	"SPD_iF2B3_151       ",	    //60
	"CFLT_iF2B3_152      ",	    //61
	"CST_iF2B3_153       ",	    //62
	"CTIN_iF2B3_154      ",	    //63	//bit 15
};


//FPGA2 SPI Output Index
enum
{
	//Buff0	
    LDL_CPU_oF2B0=0,			//0 	//bit 0		DL_CPU
    DZ_CPU_oF2B0,				//1
    LUL_CPU_oF2B0,				//2 UL_CPU
    SD_CPU_oF2B0,				//3 DNR_CPU
    SU_CPU_oF2B0,				//4 UPR_CPU  
    SUF_CPU_oF2B0,				//5 UPF_CPU   
    SDF_CPU_oF2B0,				//6 DNF_CPU   
    RUN_CPU_oF2B0,				//7    
    CEN_CPU_oF2B0,				//8 MC1_CPU   
    MC_CPU_oF2B0,				//9    
    STE_CPU_oF2B0,				//10    
    SELOK_CPU_oF2B0,			//11    
	SPDLEV_CPU_oF2B0,			//12 SPD_LEV_CPU  *NO_Output_TYPE
	SPD75_CPU_oF2B0,			//13
	SPD150_CPU_oF2B0,			//14	 
	CPLD2_SPI_TEST_bit_oF2B0,	//15	//bit 15  *NO_Output_TYPE
	glax1121_FPGA2_output_io_num//16
};





//---------------------------------------
//NTS CAN Input Index
enum
{
	//Buff0
	TEST_MODE_iNTSB0=0,			//0		//bit 0
	UNUSED0_iNTSB0,				//1 *UNUSED
	UNUSED1_iNTSB0,				//2 *UNUSED
	UNUSED2_iNTSB0,				//3 *UNUSED
	UNUSED3_iNTSB0,				//4 *UNUSED
	UNUSED4_iNTSB0,				//5 *UNUSED
	UNUSED5_iNTSB0,				//6 *UNUSED
	UNUSED6_iNTSB0,				//7 *UNUSED //bit 7
	glax1121_NTS_input_io_num	//8
};


//NTS CAN Input Name
char NTS_Input_Name[glax1121_NTS_input_io_num][20] =
{
	//Buff0
	"TEST_MODE_iNTSB0_155",	    //0		//bit 0
	"UNUSED0_iNTSB0_156  ",	    //1	*UNUSED
	"UNUSED1_iNTSB0_157  ",	    //2 *UNUSED
	"UNUSED2_iNTSB0_158  ",	    //3 *UNUSED
	"UNUSED3_iNTSB0_159  ",	    //4 *UNUSED
	"UNUSED4_iNTSB0_160  ",	    //5 *UNUSED
	"UNUSED5_iNTSB0_161  ",	    //6 *UNUSED
	"UNUSED6_iNTSB0_162  ",	    //7 *UNUSED	//bit 7						
};



//NTS SPI Output Index
enum
{
	//Buff0
	UT_NTS_SPIout=0,			//0		//bit 0
	UN_NTS_SPIout,				//1
	DT_NTS_SPIout,				//2
	DN_NTS_SPIout,				//3
	UNUSED0_SPIout,				//4 *UNUSED
	UNUSED1_SPIout,				//5 *UNUSED
	UNUSED2_SPIout,				//6 *UNUSED
	UNUSED3_SPIout,				//7 *UNUSED //bit 7
	glax1121_NTS_SPIoutput_io_num	//8
};


//NTS CAN Output Index
enum
{
	//Buff0
	DZ_NTS_CANout=0,			//0		//bit 0
	SPD_150_NTS_CANout,			//1
	SPD_75_NTS_CANout,			//2
	NTS_FLT_CANout,				//3
	SEL_OK_NTS_CANout,			//4
	PAL_FAULT_CANout,			//5
	UNUSED0_CANout,				//6 *UNUSED
	UNUSED1_CANout,				//7 *UNUSED //bit 7
	glax1121_NTS_CANoutput_io_num	//8
};

	
	

//------------------------------------------------------------------------------
enum
{
	Input_TYPE=0,
	Input_INDEX
};

#define NO_Input_TYPE		0x0000
#define Input_Type_SEB		0x0001
#define Input_Type_FPGA1	0x0002
#define Input_Type_FPGA2	0x0004
#define Input_Type_NTS		0x0008




enum
{
	Output_TYPE=0,
	Output_INDEX
};

#define NO_Output_TYPE		0x0000
#define Output_Type_SEB		0x0001
#define Output_Type_FPGA1	0x0002
#define Output_Type_FPGA2	0x0004
#define Output_Type_NTS_SPI	0x0008
#define Output_Type_NTS_CAN	0x0010




enum
{		
	Fail_ON_Type_SEB=1,
	Fail_OFF_Type_SEB,
	Shorted_Type_SEB,
	
	Fail_ON_Type_FPGA1,
	Fail_OFF_Type_FPGA1,
	Shorted_Type_FPGA1,
	
	Fail_ON_Type_FPGA2,
	Fail_OFF_Type_FPGA2,
	Shorted_Type_FPGA2,
	
	Fail_ON_Type_NTS,				
	Fail_OFF_Type_NTS,				
	Shorted_Type_NTS
};







    


//Note:IM meaning Input Module 
int16 Input_Signal_Detail[glax1121_SEB_input_io_num+glax1121_FPGA1_input_io_num+glax1121_FPGA2_input_io_num+glax1121_NTS_input_io_num][2]=
{
	//Input_TYPE	  Input_INDEX
												//Input											Output
												  //TO				   FROM						FROM	 FROM	  FROM	               FROM	              FROM		FROM												
												  //SEB Board SEB Pin  GALX1121	  GALX1121		FPGA1	 FPGA2	  GALX1121	GALX1121   SEB Board SEB Pin  NTS Pin	Component
											      //#(4to8)	  #(0to11) Left Side  Right Side	Pin#	 Pin#  	  Left Side	Right Side #(4to8)   #(0to11) #(1to44)
	//SEB																						(1to100) (1to144)          	     	  																
	{Input_Type_SEB,  I_suf},				//0	    4		  0		   SUF-S14					95,99    141      TSD-H15              6         0        9         MD1A,MD1B
											//													85 		 127				CST-T17    8         1              	MD6A,RLY2A,IC12,IC13,IC15,SW1  
	{Input_Type_SEB,  I_sufc},				//1	    4		  1		   SUFC-S13
	{Input_Type_SEB,  I_su},				//2		4		  2		   SU-S12                   92,98    140                CST-T17    8         1        8         MD2A,MD2B               
											//                                                  85 		 127				                                      	MD6A,RLY2A,IC12,IC13,IC15,SW1
	{Input_Type_SEB,  I_suc},				//3		4		  3		   SUC-S11
	{Input_Type_SEB,  I_sdf},				//4		4		  4		   SDF-S10					96,100	 142				CST-T17	   8		 1		  14        MD5A,MD5B
											//                                                  85 		 127				                                      	MD6A,RLY2A,IC12,IC13,IC15,SW1                    
	{Input_Type_SEB,  I_sdfc},				//5		4		  5		   SDFC-S9                                              
	{Input_Type_SEB,  I_sd},				//6		4		  6		   SD-S8					91,97    139                CST-T17	   8		 1        15        MD3A,MD3B
											//					                                85 		 127				                                      	MD6A,RLY2A,IC12,IC13,IC15,SW1								
	{Input_Type_SEB,  I_sdc},				//7		4		  7		   SDC-S7
	{Input_Type_SEB,  I_s6s10},				//8		4		  8		   S10-S6																						S10
	{Input_Type_SEB,  I_s2mrw},				//9		5		  0		   MRSW-S2					   				    		CST-T17	   8		 1 					SW1
	{Input_Type_SEB,  I_24s10},				//10	5		  3		   S10-H24																						S10 
	{Input_Type_SEB,  I_22gov},				//11    5		  5		   GOV-H22							  		  GOV-H23			   5         4
	{Input_Type_SEB,  I_20tlh},				//12	5		  7		   TLH-H20							  		  TLH-H21			   5         6
	{Input_Type_SEB,  I_h18ps},				//13    5		  9		   PS-H18							  		  PS-H19			   5         8
	{Input_Type_SEB,  I_16mrw},				//14	5		  11	   MRSW-H16									    		SAME AS SEB "I_s2mrw"							   		
	{Input_Type_SEB,  I_14mrw},				//15	6		  1		   MRSW-H14								        		SAME AS SEB "I_s2mrw"							   
	{Input_Type_SEB,  I_11mrw},				//16	6		  4		   MRSW-H11								        		SAME AS SEB "I_s2mrw"							   
	{Input_Type_SEB,  I_h8s10},				//17	6		  7		   S10-H8																						S10
	{Input_Type_SEB,  I_h6s10},				//18	6		  9		   S10-H6																						S10
	{Input_Type_SEB,  I_h4s10},				//19	6		  11	   S10-H4																						S10
	{Input_Type_SEB,  I_h2s10},				//20	7		  1		   S10-H2																						S10
	{Input_Type_SEB,  I_mrsw},				//21	7		  3					  MRSW							    		SAME AS SEB "I_s2mrw"							   
	{Input_Type_SEB,  I_mc},				//22	7		  4					  MC            87       130                CST-T17    8         1                  SW1,MD6B,IC15
	{Input_Type_SEB,  I_mc1},				//23    7         5                   MC1           86       129                CST-T17    8         1                  SW1,MD4A,IC15
	{Input_Type_SEB,  I_ste},				//24	7		  9					  STE		    89       131	  TPH-S1			   5         1					MD4B,IC15
	{Input_Type_SEB,  I_s10},				//25	7		  10			  	  S10																				S10
	{Input_Type_SEB,  I_18hss},				//26	8		  0					  HSS-T18				  		  HSS-H17			   5	     10
	
		
		
		
		
												//Input  	 Output
												  //TO		 FROM	  FROM	  FROM				   FROM	              FROM	    FROM
												  //FPGA1	 FPGA1	  FPGA2	  GALX1121	GALX1121   SEB Board SEB Pin  NTS Pin	Component
												  //Pin#  	 Pin# 	  Pin#    Left Side	Right Side #(4to8)   #(0to11) #(1to44)
	//FPGA1 Buff0	     							(1to100) (1to100) (1to144)          	     	  
 	{Input_Type_FPGA1,  ACC_iF1B0},			//27  	49			 	 	 	            ACC-T10	   8		 8					IC14,IM8
 	{Input_Type_FPGA1,  GS_iF1B0},			//28  	48						   			GS-T9 	   8		 9					IC14,IM8
	{Input_Type_FPGA1,  RGS_iF1B0},			//29  	47						  			RGS-T8     8		 10					IC14,IM8
	{Input_Type_FPGA1,  LPS_iF1B0},		    //30	43						  LPS-S5      		   4		 9					IC16,IM3
	{Input_Type_FPGA1,  RLM_iF1B0},			//31	42						  RLM-H1			   7		 2					IC23,IM1
	{Input_Type_FPGA1,  DLT_iF1B0},			//32	41						  DLT-H3			   7		 0					IC23,IM1
	{Input_Type_FPGA1,  DLM_iF1B0},			//33	40						  DLM-H5			   6		 10					IC23,IM1
	{Input_Type_FPGA1,  DLB_iF1B0},			//34    38						  DLB-H7			   6         8					IC23,IM1
	{Input_Type_FPGA1,  TAD_iF1B0},			//35    36						  TAD-H9			   6		 6					IC18,IM2
	{Input_Type_FPGA1,  TAU_iF1B0},			//36    35						  TAU-H10			   6         5					IC18,IM2
	{Input_Type_FPGA1,  BAD_iF1B0},			//37    34						  BAD-H12		       6		 3					IC18,IM2
	{Input_Type_FPGA1,  BAU_iF1B0},			//38    33				 		  BAU-H13			   6		 2					IC18,IM2
	{Input_Type_FPGA1,  GBP_iF1B0},			//39    30									   		   								S10,SW5,IC19,IM5		  					    
	{Input_Type_FPGA1,  GLB_iF1B0},			//40    29									ICA-T12    8		 6					SW5,SW7,IC19,IM5
	{Input_Type_FPGA1,  LBP_iF1B0},			//41	28							   		   										S10,SW7,IC19,IM5		 			
 	{Input_Type_FPGA1,  AUTO_iF1B0},		//42    27			  						ICA-T12    8		 6					SW5,SW7,SW6,IC19,IM5
 	
 	//FPGA1 Buff1
 	{Input_Type_FPGA1,  UL_CPU_iF1B1},		//43	15		 		  108	  
{Input_Type_FPGA1,  DNF_CPU_iF1B1},//SDF	//44	8		 		  142                                                         
{Input_Type_FPGA1,  UPF_CPU_iF1B1},//SUF	//45  	7		 		  141		                                                 
{Input_Type_FPGA1,  UPR_CPU_iF1B1},//SU		//46    6				  140                                                         
{Input_Type_FPGA1,  DNR_CPU_iF1B1},//SD		//47    5		 		  139                                                         
 	{Input_Type_FPGA1,  STE_CPU_iF1B1},		//48    4		 		  131
 	{Input_Type_FPGA1,  MC_CPU_iF1B1},		//49 	3		 		  130
{Input_Type_FPGA1,  CEN_CPU_iF1B1},//MC1_CPU//50 	2		 		  129
 	{Input_Type_FPGA1,  UNUSED0_iF1B1},		//51	*UNUSED
	{Input_Type_FPGA1,  MRID_iF1B1},		//52	66									ICA-T12    8		 6  				SW5,SW7,SW6,SW2,SW4,IC25,IM6 																						 				 
	{Input_Type_FPGA1,  MRIU_iF1B1},		//53    67									ICA-T12    8		 6  				SW5,SW7,SW6,SW2,SW3,IC25,IM6																						 					 
 	{Input_Type_FPGA1,  MRI_iF1B1},			//54    68 									ICA-T12    8		 6					SW5,SW7,SW6,IC25,IM6
 	{Input_Type_FPGA1,  ID_iF1B1},			//55	76									 ID-T14	   8		 4					IC17,IM7
 	{Input_Type_FPGA1,  IU_iF1B1},			//56    77									 IU-T15	   8		 3					IC17,IM7
 	{Input_Type_FPGA1,  CTIN_iF1B1},//INS	//57    78									INS-T16	   8         2					IC17,IM7
 	{Input_Type_FPGA1,  ICI_iF1B1},     	//58    50									ICI-T11	   8		 7					IC14,IM8
 	
 	//FPGA1 Buff2
 	//PSTE,PDN,PUN,PUT,PDT,PSD,PSU,PSUF,PSDF,
 	//are Output Signal from FPGA1 that Energize the Hardware Gate,
 	//however, FPGA1 sends these output signal back to CPU(Production Code) by communication;
 	//for Testing Code here, DONT_CARE becasue they DONT GET Physical Wiring involved
 	{Input_Type_FPGA1,  PSTE_iF1B2},//STE  	//59	*DONT_CARE	 	
 	{Input_Type_FPGA1,  PDN_iF1B2},//DN  	//60	*DONT_CARE
 	{Input_Type_FPGA1,  PUN_iF1B2},//UN  	//61	*DONT_CARE
 	{Input_Type_FPGA1,  PUT_iF1B2},//UT  	//62	*DONT_CARE
 	{Input_Type_FPGA1,  PDT_iF1B2},//DT  	//63	*DONT_CARE
 	{Input_Type_FPGA1,  PSD_iF1B2},//SD  	//64    *DONT_CARE
 	{Input_Type_FPGA1,  PSU_iF1B2},//SU  	//65	*DONT_CARE
 	{Input_Type_FPGA1,  PSUF_iF1B2},//SUF  	//66	*DONT_CARE
 	{Input_Type_FPGA1,  PSDF_iF1B2},//SDF  	//67	*DONT_CARE
 	
 	{Input_Type_FPGA1,  RUN_CPU_iF1B2},   	//68	1		 		  127	                                                    
 	{Input_Type_FPGA1,  SEL_OK_CPU_iF1B2},  //69	21		 		  101	
 	{Input_Type_FPGA1,  SPD_75_CPU_iF1B2},  //70	20		 		  103
 	{Input_Type_FPGA1,  SPD_150_CPU_iF1B2}, //71	19		 		  104 
 															//*NOT THE OUTPUT HERE, BUT GOT PHYSICAL WIRING
 	{Input_Type_FPGA1,  DZ_PAL_iF1B2}, 		//72	18				  (105)    *DZ_PAL is OUT from FPGA1 TO FPGA2, 
 											//									For Production Code:
 											//														CPU NEEDS to READ back From FPGA1 and FPGA2									
 											//								    But For Testing Code: 
											//								                        CPU NO NEED to READ back From FPGA1(DEFAULT Value '0')
											//								   						BUT CPU READS back from FPGA2  
 	
 	{Input_Type_FPGA1,  DL_CPU_iF1B2}, 		//73	17		 		  106
 	{Input_Type_FPGA1,  DZ_CPU_iF1B2}, 		//74	16		 		  107	
 	
 	//FPGA1 Buff3	
	{Input_Type_FPGA1,  UNUSED1_iF1B3},		//75	*UNUSED	
 	{Input_Type_FPGA1,  TEST_MODE_iF1B3}, 	//76	51									   										CN18 Pin 1	 	
 	{Input_Type_FPGA1,  REAR_DOOR_iF1B3}, 	//77	52									   										CN17 Pin 1 	
 	{Input_Type_FPGA1,  NTS_FLT_iF1B3}, 	//78	53														  		  35 																												 																							 								  	 											
	{Input_Type_FPGA1,  SPD_75_NTS_iF1B3}, 	//79	55														  	      22
	{Input_Type_FPGA1,  SPD_150_NTS_iF1B3}, //80	54														  		  21
	{Input_Type_FPGA1,  SEL_OK_NTS_iF1B3},  //81	56														  		  31
	{Input_Type_FPGA1,  DZ_NTS_iF1B3},  	//82	57														  		  19
	{Input_Type_FPGA1,  DN_NTS_iF1B3},  	//83	81													  	  		  15
	{Input_Type_FPGA1,  UN_NTS_iF1B3},  	//84	82														  		  8
	{Input_Type_FPGA1,  UT_NTS_iF1B3},  	//85	83														  		  9
	{Input_Type_FPGA1,  DT_NTS_iF1B3},  	//86	84														  		  14	
	{Input_Type_FPGA1,  PAL_FLT_iF1B3},  	//87	69													  	  		  25
	
	//SAME AS PSTE
	{Input_Type_FPGA1,  PRUN_iF1B3},  		//88	*DONT_CARE											                       
	{Input_Type_FPGA1,  PCEN_iF1B3},  		//89	*DONT_CARE
	{Input_Type_FPGA1,  PMC_iF1B3},  		//90	*DONT_CARE
	
		
		
		     
		
												//Input		 Output
												  //TO		 FROM	  FROM    FROM				   FROM	    		  FROM		FROM
												  //FPGA2	 FPGA1 	  FPGA2	  GALX1121	GALX1121   SEB Board SEB Pin  NTS Pin   Component
												  //Pin#  	 Pin#  	  Pin#	  Left Side	Right Side #(4to8)   #(0to11) #(1to44)
	//FPGA2 Buff0	     							(1to144) (1to100) (1to144)    
	{Input_Type_FPGA2,  L120B_iF2B0},  		//91  	24   	     		  				  										S10,IC18,IM2
	{Input_Type_FPGA2,  CEN_iF2B0},//MC1	//92    125		 86		  129				CST-T17	   8		 1                  IC19,IM5,MD4A,IC15,SW1
	{Input_Type_FPGA2,  TSD_iF2B0}, 		//93	16				  		  TSD-H15			   6         0					IC25,IM6
	{Input_Type_FPGA2,  HSS_iF2B0}, 		//94	15				  		  HSS-H17			   5         10					IC16,IM3
	{Input_Type_FPGA2,  GOV_iF2B0}, 		//95	14				  		  GOV-H23			   5		 4					IC16,IM3
	{Input_Type_FPGA2,  S10_iF2B0}, 		//96	13									  										S10,IC16,IM3 			
	{Input_Type_FPGA2,  TPH_iF2B0}, 		//97	12		 89       131	  TPH-S1			   5         9					IC16,IM3,MD4B,IC15
	{Input_Type_FPGA2,  TPL_iF2B0}, 		//98	11				  		  TPL-S3			   4         11					IC16,IM3
	{Input_Type_FPGA2,  LOS_iF2B0}, 		//99	8				  		  LOS-S4			   4		 10					IC16,IM3
	{Input_Type_FPGA2,  LPS_iF2B0}, 		//100	7  	SAME AS FPGA1 Buff0 "LPS_iF1B0" 
	{Input_Type_FPGA2,  SDF_iF2B0}, 		//101	6        96,100	  142				CST-T17	   8		 1		  14		MD5A,MD5B
											//               85       127                                                       MD6A,RLY2A,IC12,IC13,IC15,SW1,IC26,IM4
	{Input_Type_FPGA2,  SD_iF2B0}, 			//102	5		 91,97    139				CST-T17	   8		 1		  15		MD3A,MD3B, 
										    //               85       127                                                       MD6A,RLY2A,IC12,IC13,IC15,SW1,IC26,IM4
	{Input_Type_FPGA2,  UT_iF2B0}, 			//103   4		 95		  		  TSD-H15              6         0		  9			IC26,IM4,MD1A,IC13
	{Input_Type_FPGA2,  UN_iF2B0}, 			//104	3		 92							CST-T17	   8		 1		  8			IC26,IM4,MD2A,IC13,SW1
	{Input_Type_FPGA2,  DT_iF2B0}, 			//105	2		 96							CST-T17	   8		 1		  14		IC26,IM4,MD5A,SW1,IC13
	{Input_Type_FPGA2,  DN_iF2B0}, 			//106	1		 91							CST-T17	   8		 1		  15		IC26,IM4,MD3A,SW1
	
	//FPGA2 Buff1
	{Input_Type_FPGA2,  ACC_iF2B1}, 		//107   73	SAME AS FPGA1 Buff0 "ACC_iF1B0"
	{Input_Type_FPGA2,  REAR_DOOR_iF2B1}, 	//108	49	SAME AS FPGA1 Buff3 "REAR_DOOR_iF1B3"
	{Input_Type_FPGA2,  TEST_MODE_iF2B1}, 	//109	48	SAME AS FPGA1 Buff3 "TEST_MODE_iF1B3"
	{Input_Type_FPGA2,  RLM1_iF2B1}, 		//110	44	SAME AS FPGA1 Buff0 "RLM_iF1B0" 
	{Input_Type_FPGA2,  RLM_iF2B1}, 		//111	43	SAME AS FPGA1 Buff0 "RLM_iF1B0" 
	{Input_Type_FPGA2,  DLT1_iF2B1}, 		//112	42	SAME AS FPGA1 Buff0 "DLT_iF1B0" 			  		  
	{Input_Type_FPGA2,  DLT_iF2B1}, 		//113	41	SAME AS FPGA1 Buff0 "DLT_iF1B0"
	{Input_Type_FPGA2,  DLM1_iF2B1}, 		//114	40	SAME AS FPGA1 Buff0 "DLM_iF1B0" 
	{Input_Type_FPGA2,  DLM_iF2B1}, 		//115	39	SAME AS FPGA1 Buff0 "DLM_iF1B0" 
	{Input_Type_FPGA2,  DLB1_iF2B1}, 		//116	38  SAME AS FPGA1 Buff0 "DLB_iF1B0" 
	{Input_Type_FPGA2,  DLB_iF2B1}, 		//117	37  SAME AS FPGA1 Buff0 "DLB_iF1B0" 
	{Input_Type_FPGA2,  TAD_iF2B1}, 		//118	31	SAME AS FPGA1 Buff0 "TAD_iF1B0"  
	{Input_Type_FPGA2,  TAU_iF2B1}, 		//119	30	SAME AS FPGA1 Buff0 "TAU_iF1B0" 
	{Input_Type_FPGA2,  BAD_iF2B1}, 		//120	29	SAME AS FPGA1 Buff0 "BAD_iF1B0" 
	{Input_Type_FPGA2,  BAU_iF2B1}, 		//121	28  SAME AS FPGA1 Buff0 "BAU_iF1B0" 
	{Input_Type_FPGA2,  L120_iF2B1}, 		//122	27	SAME AS FPGA2 Buff0 "L120B_iF2B0"  
	
	//FPGA2 Buff2
	{Input_Type_FPGA2,  IU_iF2B2}, 		 	//123	110	SAME AS FPGA1 Buff1 "IU_iF1B1" 
	{Input_Type_FPGA2,  ID_iF2B2}, 		 	//124   109	SAME AS FPGA1 Buff1 "ID_iF1B1"	
	{Input_Type_FPGA2,  MRIN_iF2B2},//MRI	//125	81	SAME AS FPGA1 Buff1 "MRI_iF1B1" 
	{Input_Type_FPGA2,  MRIU_iF2B2}, 		//126	80	SAME AS FPGA1 Buff1 "MRIU_iF1B1"  
	{Input_Type_FPGA2,  MRIE_iF2B2}, 		//127	79									ICA-T12	   8		 6                  SW5,SW7,SW6,SW2,IC25,IM6             
	{Input_Type_FPGA2,  MRID_iF2B2}, 		//128	78 	SAME AS FPGA1 Buff1 "MRID_iF1B1"  
	{Input_Type_FPGA2,  RUN_iF2B2},//MRSW	//129	77									CST-T17	   8		 1 					IC25,IM6,SW1
	{Input_Type_FPGA2,  IEN_iF2B2},     	//130	75									IEN-T13	   8	     5					IC14,IM8
	{Input_Type_FPGA2,  ICI_iF2B2},     	//131	74	SAME AS FPGA1 Buff1 "ICI_iF1B1"  
	{Input_Type_FPGA2,  RGS1_iF2B2},     	//132	72	SAME AS FPGA1 Buff0 "RGS_iF1B0"
	{Input_Type_FPGA2,  RGS_iF2B2},     	//133	71	SAME AS FPGA1 Buff0 "RGS_iF1B0" 
	{Input_Type_FPGA2,  GS1_iF2B2},     	//134	70	SAME AS FPGA1 Buff0 "GS_iF1B0"
	{Input_Type_FPGA2,  GS_iF2B2},     		//135	69	SAME AS FPGA1 Buff0 "GS_iF1B0" 
	{Input_Type_FPGA2,  NTS_FLT_iF2B2},     //136 	67	SAME AS FPGA1 Buff3 "NTS_FLT_iF1B3"  	
	{Input_Type_FPGA2,  PAL_FLT_iF2B2},     //137	66  SAME AS FPGA1 Buff3 "PAL_FLT_iF1B3"													  		  
	{Input_Type_FPGA2,  RESET_iF2B2},       //138	61																			CN1
	
	//FPGA2 Buff3
	{Input_Type_FPGA2,  DZ_iF2B3},//DZ_PAL 	//139	105		18	  *DZ_PAL is OUT from FPGA1 TO FPGA2, 
 											//									For Production Code:
 											//														CPU NEEDS to READ back From FPGA1 and FPGA2									
 											//								    But For Testing Code: 
											//								                        CPU NO NEED to READ back From FPGA1(DEFAULT Value '0')
											//								   						BUT CPU READS back from FPGA2   
	{Input_Type_FPGA2,  SU_iF2B3}, 		  	//140	144		92,98     140				CST-T17	   8		 1		  8			MD2B,MD2A,
										    //              85        127                                                       MD6A,RLY2A,IC12,IC13,IC15,SW1,IC26,IM4                                  
	{Input_Type_FPGA2,  SUF_iF2B3}, 		//141	143     99.95     141     TSD-H15			   6         0        9         MD1A,MD1B,
											//				85        127     			CST-T17	   8		 1					MD6A,RLY2A,IC12,IC13,IC15,SW1,IC26,IM4
	{Input_Type_FPGA2,  MRSW_iF2B3}, 		//142	23						    		CST-T17	   8		 1 					IC18,IM2,SW1
	{Input_Type_FPGA2,  GBP_iF2B3}, 		//143	124	SAME AS FPGA1 Buff0 "GBP_iF1B0" 
	{Input_Type_FPGA2,  GLB_iF2B3}, 		//144  	123	SAME AS FPGA1 Buff0 "GLB_iF1B0"  
	{Input_Type_FPGA2,  LBP_iF2B3}, 		//145	122 SAME AS FPGA1 Buff0 "LBP_iF1B0"  	
	{Input_Type_FPGA2,  IND_iF2B3}, 		//146	121																			S10,IC19,IM5,SW8
	{Input_Type_FPGA2,  AD_iF2B3}, 			//147	120																			S10,IC19,IM5,SW9
	{Input_Type_FPGA2,  AUTO_iF2B3}, 		//148	119 SAME AS FPGA1 Buff0 "AUTO_iF1B0"  
	{Input_Type_FPGA2,  MC_iF2B3}, 			//149	118 	 87       130				CST-T17	   8		 1					SW1,MD6B,IC15,IC17,IM7
	{Input_Type_FPGA2,  MTO_iF2B3}, 		//150	117									MTO		   7         6					IC17,IM7
	{Input_Type_FPGA2,  SPD_iF2B3}, 		//151	114 								SPD		   7         7					IC17,IM7
	{Input_Type_FPGA2,  CFLT_iF2B3}, 		//152	113									CFLT	   7         8					IC17,IM7
	{Input_Type_FPGA2,  CST_iF2B3}, 		//153	112									CST-T17	   8		 1					IC17,IM7
	{Input_Type_FPGA2,  CTIN_iF2B3},//INS	//154	111	SAME AS FPGA1 Buff1 "CTIN_iF1B1" 

		
		
		
		
												//Input		Output
												  //TO		 FROM	  FROM				   FROM	    		  FROM		FROM
												  //NTS	 	 FPGA1	  GALX1121	GALX1121   SEB Board SEB Pin  FPGA2     Component
												  //#(1to44) Pin#  	  Left Side	Right Side #(4to8)   #(0to11) Pin#
												  //		 (1to100)										  #(1to144)
	//NTS
	{Input_Type_NTS,  TEST_MODE_iNTSB0}, 	//155	32	SAME AS FPGA1 Buff3 "TEST_MODE_iF1B3"
	{Input_Type_NTS,  UNUSED0_iNTSB0},		//156	*UNUSED	
	{Input_Type_NTS,  UNUSED1_iNTSB0},		//157	*UNUSED
	{Input_Type_NTS,  UNUSED2_iNTSB0},		//158	*UNUSED
	{Input_Type_NTS,  UNUSED3_iNTSB0},		//159	*UNUSED
	{Input_Type_NTS,  UNUSED4_iNTSB0},		//160	*UNUSED
	{Input_Type_NTS,  UNUSED5_iNTSB0},		//161	*UNUSED
	{Input_Type_NTS,  UNUSED6_iNTSB0},		//162	*UNUSED
};	


      


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
	PSTE_iF1B2_59,				//59	//bit 0	 *DONT_CARE
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
	UNUSED0_iNTSB0_156,			//156 *UNUSED
	UNUSED1_iNTSB0_157,			//157 *UNUSED
	UNUSED2_iNTSB0_158,			//158 *UNUSED
	UNUSED3_iNTSB0_159,			//159 *UNUSED
	UNUSED4_iNTSB0_160,			//160 *UNUSED
	UNUSED5_iNTSB0_161,			//161 *UNUSED
	UNUSED6_iNTSB0_162,			//162 *UNUSED	//bit 7
	glax1121_ALL_input_io_num	//163		
};	





int16 Output_Signal_Detail[glax1121_SEB_output_io_num+glax1121_FPGA1_output_io_num+glax1121_FPGA2_output_io_num+glax1121_NTS_SPIoutput_io_num+glax1121_NTS_CANoutput_io_num][2]=
{
	//Output_TYPE	  Output_INDEX
												//Output										Input
												  //FROM			   TO						TO	     TO       TO	    		   TO		          TO		TO										
												  //SEB Board SEB Pin  GALX1121	  GALX1121		FPGA1	 FPGA2	  GALX1121	GALX1121   SEB Board SEB Pin  NTS Pin	Component
											      //#(4to8)	  #(0to11) Left Side  Right Side	Pin#	 Pin#  	  Left Side	Right Side #(4to8)   #(0to11) #(1to44)
	//SEB																						(1to100) (1to144)          	     	  																
	{Output_Type_SEB,  O_S5LPS},			//0	    4		  9		   LPS-S5					43 		 7															IC16,IM3
	{Output_Type_SEB,  O_S4LOS},			//1	    4		  10	   LOS-S4							 8															IC16,IM3
	{Output_Type_SEB,  O_S3TPL},			//2		4		  11	   TPL-S3					         11															IC16,IM3
	{Output_Type_SEB,  O_S1TPH},			//3		5		  1		   TPH-S1 		                     12														    IC16,IM3
	{Output_Type_SEB,  O_23GOV},			//4		5		  4		   GOV-H23				             14		  GOV-H22			   5         5					IC16,IM3
	{Output_Type_SEB,  O_21TLH},			//5	    5         6        TLH-H21                                    TLH-H20              5         7
	{Output_Type_SEB,  O_H19PS},			//6	    5         8         PS-H19                                     PS-H18              5         9		
	{Output_Type_SEB,  O_17HSS},			//7		5         10       HSS-H17 							 15					HSS-T18    8         0					IC16,IM3																			
	{Output_Type_SEB,  O_15TSD},			//8	    6         0	       TSD-H15                           16															IC25,IM6
	{Output_Type_SEB,  O_13BAU},			//9	 	6		  2		   BAU-H13		            33		 28															IC18,IM2	   				    
	{Output_Type_SEB,  O_12BAD},			//10	6         3        BAD-H12                  34       29															IC18,IM2	 	
	{Output_Type_SEB,  O_10TAU},			//11    6         5        TAU-H10                  35       30                                                         IC18,IM2                	  
	{Output_Type_SEB,  O_H9TAD},			//12    6         6        TAD-H9                   36       31                                                         IC18,IM2              	
	{Output_Type_SEB,  O_H7DLB},			//13    6         8        DLB-H7                   38       37,38                                                      IC23,IM1                                                                                                       	 																		 
	{Output_Type_SEB,  O_H5DLM},			//14    6         10       DLM-H5                   40       39,40                                                      IC23,IM1
	{Output_Type_SEB,  O_H3DLT},			//15    7         0        DLT-H3                   41       41,42                                                      IC23,IM1								   
	{Output_Type_SEB,  O_RLM},				//16 	7         2        RLM-H1                   42       43,44                                                      IC23,IM1						   
	{Output_Type_SEB,  O_0MTO},				//17	7         6                   MTO                    117                                                        IC17,IM7                                                                                     	  	
	{Output_Type_SEB,  O_0SPD},				//18 	7         7                   SPD                    114                                                        IC17,IM7   	 
	{Output_Type_SEB,  O_0CFLT},			//19    7         8                   CFLT                   113                                                        IC17,IM7                   		 													 
	{Output_Type_SEB,  O_TMRLY},			//20    7         11		                                                                                            TEST_MODE_RELAY
	{Output_Type_SEB,  O_17CST},			//21    8	      1                   CST-T17                112                                                        IC17,IM7                                                                                    		 				   
	{Output_Type_SEB,  O_16INS},			//22    8         2                   INS-T16       78       111                                                        IC17,IM7             	
	{Output_Type_SEB,  O_T15IU},			//23    8         3                    IU-T15       77       110                                                        IC17,IM7              	
	{Output_Type_SEB,  O_T14ID},			//24    8         4                    ID-T14       76       109                                                        IC17,IM7  		  
	{Output_Type_SEB,  O_13IEN},			//25    8         5                   IEN-T13                75                                                         IC14,IM8      	
	{Output_Type_SEB,  O_12ICA},			//26    8         6			*used for FPGA1 "GLB_iF1B0" "AUTO_iF1B0" "MRID_iF1B1" 
																  	   //               "MRIU_iF1B1" "MRI_iF1B1" 
																  	   //         FPGA2 "MRIE_iF2B2"	
	{Output_Type_SEB,  O_11ICI},			//27    8         7                   ICI-T11       50       74                                                         IC14,IM8	
	{Output_Type_SEB,  O_10ACC},			//28    8         8                   ACC-T10       49       73                                                         IC14,IM8
	{Output_Type_SEB,  O_T9GS},				//29    8         9                    GS-T9        48       69,70                                                      IC14,IM8      
	{Output_Type_SEB,  O_RGS},				//30    8         10                  RGS           47       71,72                                                      IC14,IM8
	{Output_Type_SEB,  O_RDRLY},			//31    8         11                                                                                                    REAR_DOOR_RELAY           

		
		
		
		
												//Output     Input	  		
												  //FROM     TO		  TO
												  //FPGA1    FPGA2	  Component
												  //Pin#     Pin#
												  //(1to100) (1to144)
	//FPGA1 Buff0
	{Output_Type_FPGA1,  PSU_LP_oF1B0},	    //32    98      		  IC12A,MD2B  
	{Output_Type_FPGA1,  PSD_LP_oF1B0},	    //33    97				  IC12B,MD3B
	{Output_Type_FPGA1,  PSUF_LP_oF1B0},    //34    99				  IC12C,MD1B
	{Output_Type_FPGA1,  PSDF_LP_oF1B0},    //35    100				  IC12D,MD5B
	{Output_Type_FPGA1,  PUN_LP_oF1B0},     //36    92                IC13A,MD2A
	{Output_Type_FPGA1,  PDN_LP_oF1B0},     //37    91				  IC13B,MD3A
	{Output_Type_FPGA1,  PUT_LP_oF1B0},     //38    95				  IC13C,MD1A
	{Output_Type_FPGA1,  PDT_LP_oF1B0},     //39    96				  IC13D,MD5A
	{Output_Type_FPGA1,  PRUN_LP_oF1B0},    //40    85				  IC15A,MD6A
{Output_Type_FPGA1,  PCEN_LP_oF1B0},//MC1   //41    86                IC15B,MD4A
	{Output_Type_FPGA1,  PSTE_LP_oF1B0},    //42    89                IC15C,MD4B
	{Output_Type_FPGA1,  PMC_LP_oF1B0},     //43    87				  IC15D,MD6B   
{NO_Output_TYPE,  CPLD1_SPI_TEST_bit_oF1B0},//44		*NO_Output_TYPE		*ONLY for SPI communication checking  
													   //					 NO PHYSICAL WIRING
	{Output_Type_FPGA1,  DZ_PAL_oF1B0},     //45	18       105	*DZ_PAL is OUT from FPGA1 TO FPGA2, 
 											//									For Production Code:
 											//														CPU NEEDS to READ back From FPGA1 and FPGA2									
 											//								    But For Testing Code: 
											//								                        CPU NO NEED to READ back From FPGA1(DEFAULT Value '0')
											//								   						BUT CPU READS back from FPGA2 
	{NO_Output_TYPE,  UNUSED0_oF1B0},       //46	*UNUSED											   
	{NO_Output_TYPE,  UNUSED1_oF1B0},       //47	*UNUSED
	
		
		
		
		
												//Output     Input	  		
												  //FROM     TO		 TO
												  //FPGA2    FPGA1   Component
												  //Pin#     Pin#
												  //(1to144) (1to100)
	//FPGA2 Buff0
{Output_Type_FPGA2,  LDL_CPU_oF2B0},//DL_CPU//48    106      17
	{Output_Type_FPGA2,  DZ_CPU_oF2B0},     //49    107      16
{Output_Type_FPGA2,  LUL_CPU_oF2B0},//UL_CPU//50    108      15
{Output_Type_FPGA2,  SD_CPU_oF2B0},//DNR_CPU//51    139      5		 IC12B,MD3B
{Output_Type_FPGA2,  SU_CPU_oF2B0},//UPR_CPU//52    140      6		 IC12A,MD2B
{Output_Type_FPGA2,  SUF_CPU_oF2B0},//UPF_CPU//53   141      7		 IC12C,MD1B
{Output_Type_FPGA2,  SDF_CPU_oF2B0},//DNF_CPU//54   142      8		 IC12D,MD5B
	{Output_Type_FPGA2,  RUN_CPU_oF2B0},	//55    127      1		 IC15A,MD6A
{Output_Type_FPGA2,  CEN_CPU_oF2B0},//MC1_CPU//56   129      2		 IC15B,MD4A
	{Output_Type_FPGA2,  MC_CPU_oF2B0},	    //57    130      3		 IC15D,MD6B
	{Output_Type_FPGA2,  STE_CPU_oF2B0},	//58    131      4		 IC15C,MD4B	
	{Output_Type_FPGA2,  SELOK_CPU_oF2B0},	//59    101      21	
{NO_Output_TYPE,  SPDLEV_CPU_oF2B0},//SPD_LEV_CPU	 //60   *NO_Output_TYPE
	{Output_Type_FPGA2,  SPD75_CPU_oF2B0},	//61    103      20
	{Output_Type_FPGA2,  SPD150_CPU_oF2B0},	//62    104      19
{NO_Output_TYPE,  CPLD2_SPI_TEST_bit_oF2B0},//63    *NO_Output_TYPE  *ONLY for SPI communication checking
											        //				  NO PHYSICAL WIRING
		
		
		
		
		
												//Output     Input	  		
												  //FROM     TO		  TO		TO
												  //NTS      FPGA1    FPGA2		Component
												  //Pin#     Pin#     Pin#
												  //(1to44)  (1to100) (1to144)
  	//NTS SPI Buff0
{Output_Type_NTS_SPI,  UT_NTS_SPIout},	    //64    9        83       		    IC13C,MD1A
{Output_Type_NTS_SPI,  UN_NTS_SPIout},	    //65    8        82				    IC13A,MD2A
{Output_Type_NTS_SPI,  DT_NTS_SPIout},	    //66    14       84				    IC13D,MD5A
{Output_Type_NTS_SPI,  DN_NTS_SPIout},	    //67    15       81				    IC13B,MD3A
  	{NO_Output_TYPE,  UNUSED0_SPIout},   	//68    *UNUSED 
  	{NO_Output_TYPE,  UNUSED1_SPIout},   	//69    *UNUSED 
  	{NO_Output_TYPE,  UNUSED2_SPIout},   	//70    *UNUSED 
  	{NO_Output_TYPE,  UNUSED3_SPIout},   	//71    *UNUSED 
  	
  	
  	//NTS CAN Buff0
{Output_Type_NTS_CAN,  DZ_NTS_CANout},   	//72    19       57
{Output_Type_NTS_CAN,  SPD_150_NTS_CANout}, //73    21       54
{Output_Type_NTS_CAN,  SPD_75_NTS_CANout},  //74    22       55
{Output_Type_NTS_CAN,  NTS_FLT_CANout},     //75    35       53       67
{Output_Type_NTS_CAN,  SEL_OK_NTS_CANout},  //76    31       56
{Output_Type_NTS_CAN,  PAL_FAULT_CANout},   //77    25       69       66
  	{NO_Output_TYPE,  UNUSED0_CANout},   	//78    *UNUSED 
  	{NO_Output_TYPE,  UNUSED1_CANout},   	//79    *UNUSED 
};



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
	UNUSED3_SPIout_71,			//71	//bit 7	  *UNUSED	
	
	//NTS CAN Output Buff0
	DZ_NTS_CANout_72,			//72	//bit 0
	SPD_150_NTS_CANout_73,		//73
	SPD_75_NTS_CANout_74,		//74
	NTS_FLT_CANout_75,			//75
	SEL_OK_NTS_CANout_76,		//76
	PAL_FAULT_CANout_77,		//77
	UNUSED0_CANout_78,			//78 *UNUSED
	UNUSED1_CANout_79,			//79	//bit 7  *UNUSED
	glax1121_ALL_output_io_num  //80
}; 

	
	

//------------------------------------------------------------------------------
#define GALX1121_Fault_Display_ADDIN_Index		2
char GALX1121_Fault_Display_ADDIN[GALX1121_Fault_Display_ADDIN_Index][20]=
{
	"UP/DOWN:Page Rolling",    //0
	"MOD/ENT:Exit/GO BACK",    //1
};



#define GALX1121_Fault_Display_index		300
char GALX1121_Fault_Display[GALX1121_Fault_Display_index][20]=
{
//SEB
	//i_suf_0
	"TSD-H15,CST-T17     ",    //0	
	"SUF-S14,SW1,RLY2A   ",    //1
	"MD1A,MD1B,MD6A      ",    //2
	"IC12,IC13,IC15      ",	   //3
	"F1_P99,95,85        ",	   //4
	"F2_P141,127 NTS_P9  ",    //5
	
	
	//i_su_2
	"CST-T17,SU-S12      ",    //6	
	"SW1,RLY2A           ",    //7
	"MD2A,MD2B,MD6A      ",    //8
	"IC12,IC13,IC15      ",    //9
	"F1_P92,98,85        ",    //10
	"F2_P140,127 NTS_P8  ",    //11
	
	//i_sdf_4
	"CST-T17,SDF-S10     ",    //12	
	"SW1,RLY2A           ",    //13
	"MD5A,MD5B,MD6A      ",    //14
	"IC12,IC13,IC15      ",    //15
	"F1_P96,100,85       ",    //16
	"F2_P142,127 NTS_P14 ",	   //17
	
	
	//i_sd_6
	"CST-T17,SD-S8       ",	   //18
	"SW1,RLY2A           ",    //19
	"MD3A,MD3B,MD6A      ",	   //20
	"IC12,IC13,IC15      ",    //21
	"F1_P91,97,85        ",	   //22
	"F2_P139,127 NTS_P15 ",    //23
	
	//i_s6s10_8
	"S10,S10-S6          ",	   //24
	
	//i_s2mrw_9
	"CST-T17,SW1,MRSW-S2 ",    //25
	
	//i_24s10_10
	"S10,S10-H24         ",    //26
	
	//i_22gov_11
	"GOV_H23,GOV_H22     ",    //27
	
	//i_20tlh_12
	"TLH-H21,TLH-H20     ",    //28
	
	//i_h18ps_13
	"PS_H19,PS_H18       ",    //29
	
	//i_16mrw_14
	"CST-T17,SW1,MRSW-H16",    //30
	
	//i_14mrw_15
	"CST-T17,SW1,MRSW-H14",    //31
	
	//i_11mrw_16
	"CST-T17,SW1,MRSW-H11",    //32
	
	//i_h8s10_17
	"S10,S10-H8          ",    //33
	
	//i_h6s10_18
	"S10,S10-H6          ",    //34
	
	//i_h4s10_19
	"S10,S10-H4          ",    //35
	
	//i_h2s10_20
	"S10,S10-H2          ",    //36
	
	//i_mrsw_21
	"CST-T17,SW1,MRSW    ",    //37
	
	//i_mc_22
	"CST-T17,MC,F2_P130  ",    //38
	"MD6B,IC15,SW1,F1_P87",    //39    
	
	//i_mc1_23
	"CST-T17,MC1,F2_P129 ",	   //40
	"MD4A,IC15,SW1,F1_P86",    //41
	
	//i_ste_24
	"TPH-S1,STE,F2_P131  ",    //42
	"MD4B,IC15,F1_P89    ",    //43
	
	//i_s10_25
	"S10,S10(CN21)       ",    //44
	
	//i_18hss_26
	"HSS-T17,HSS-T18     ",    //45

//FPGA1	
	//ACC_iF1B0_27
	"ACC-T10,F1_P49      ",    //46
	"IC14,IM8            ",    //47
	
	//GS_iF1B0_28
	"GS-T9,F1_P48        ",    //48
	"IC14,IM8            ",    //49
	
	//RGS_iF1B0_29
	"RGS-T8,F1_P47       ",    //50
	"IC14,IM8            ",    //51
	
	//LPS_iF1B0_30
	"LPS-S5,F1_P43       ",    //52
	"IC16,IM3            ",    //53
	
	//RLM_iF1B0_31
	"RLM-H1,F1_P42       ",    //54
	"IC23,IM1            ",    //55
	
	//DLT_iF1B0_32
	"DLT-H3,F1_P41       ",    //56
	"IC23,IM1            ",    //57
	
	//DLM_iF1B0_33
	"DLM-H5,F1_P40       ",    //58
	"IC23,IM1            ",    //59
	
	//DLB_iF1B0_34
	"DLB-H7,F1_P38       ",    //60
	"IC23,IM1            ",    //61
	
	//TAD_iF1B0_35
	"TAD-H9,F1_P36       ",    //62
	"IC18,IM2            ",    //63
	
	//TAU_iF1B0_36
	"TAU-H10,F1_P35      ",    //64
	"IC18,IM2            ",    //65
	
	//BAD_iF1B0_37
	"BAD-H12,F1_P34      ",    //66
	"IC18,IM2            ",    //67
	
	//BAU_iF1B0_38
	"BAU-H13,F1_P33      ",    //68
	"IC18,IM2            ",    //69
	
	//GBP_iF1B0_39
	"S10,SW5,F1_P30      ",    //70
	"IC19,IM5            ",    //71
	
	//GLB_iF1B0_40
	"ICA-T12,F1_P29      ",    //72
	"SW5,SW7,IC19,IM5    ",    //73
	
	//LBP_iF1B0_41
	"S10,F1_P28          ",    //74
	"SW7,IC19,IM5        ",    //75
	
	//AUTO_iF1B0_42
	"ICA-T12,F1_P27      ",    //76
	"SW5,SW7,SW6,IC19,IM5",    //77
	
	//UL_CPU_iF1B1_43
	"F2_P108,F1_P15      ",    //78
	 
	//DNF_CPU_iF1B1_44
	"F2_P142,F1_P8       ",    //79
	 
	//UPF_CPU_iF1B1_45
	"F2_P141,F1_P7       ",    //80
	
	//UPR_CPU_iF1B1_46
	"F2_P140,F1_P6       ",    //81
	
	//DNR_CPU_iF1B1_47
	"F2_P139,F1_P5       ",    //82
	
	//STE_CPU_iF1B1_48
	"F2_P131,F1_P4       ",    //83
	
	//MC_CPU_iF1B1_49
	"F2_P130,F1_P3       ",    //84
	
	//CEN_CPU_iF1B1_50
	"F2_P129,F1_P2       ",    //85
	
	//MRID_iF1B1_52
	"ICA-T12,F1_P66      ",    //86
	"SW5,SW7,SW6,SW2,SW4 ",    //87
	"IC25,IM6            ",    //88
	
	//MRIU_iF1B1_53
	"ICA-T12,F1_P67      ",    //89
	"SW5,SW7,SW6,SW2,SW3 ",    //90
	"IC25,IM6            ",    //91
	
	//MRI_iF1B1_54
	"ICA-T12,F1_P68      ",    //92
	"SW5,SW7,SW6,IC25,IM6",    //93
	
	//ID_iF1B1_55
	"ID-T14,F1_P76       ",    //94
	"IC17,IM7            ",    //95
	
	//IU_iF1B1_56
	"IU-T15,F1_P77       ",    //96
	"IC17,IM7            ",    //97
	
	//CTIN_iF1B1_57
	"INS-T16,F1_P78      ",    //98
	"IC17,IM7            ",    //99
	
	//ICI_iF1B1_58
	"ICI-T11,F1_P50      ",    //100
	"IC14,IM8            ",    //101
	
	//RUN_CPU_iF1B2_68
	"F2_P127,F1_P1       ",    //102
	
	//SEL_OK_CPU_iF1B2_69
	"F2_P101,F1_P21      ",    //103
	
	//SPD_75_CPU_iF1B2_70
	"F2_P103,F1_P20      ",    //104
	
	//SPD_150_CPU_iF1B2_71
	"F2_P104,F1_P19      ",    //105
	
	//DL_CPU_iF1B2_73
	"F2_P106,F1_P17      ",    //106
	
	//DZ_CPU_iF1B2_74
	"F2_P107,F1_P16      ",    //107
	
	//TEST_MODE_iF1B3_76
	"CN18_P1,F1_P51      ",    //108
	
	//REAR_DOOR_iF1B3_77
	"CN17_P1,F1_P52      ",    //109
	
	//NTS_FLT_iF1B3_78
	"NTS_P35,F1_P53      ",    //110
	
	//SPD_75_NTS_iF1B3_79
	"NTS_P22,F1_P55      ",    //111
	
	//SPD_150_NTS_iF1B3_80
	"NTS_P21,F1_P54      ",    //112
	
	//SEL_OK_NTS_iF1B3_81
	"NTS_P31,F1_P56      ",    //113
	
	//DZ_NTS_iF1B3_82
	"NTS_P19,F1_P57      ",    //114
	
	//DN_NTS_iF1B3_83
	"NTS_P15,F1_P81      ",    //115
	
	//UN_NTS_iF1B3_84
	"NTS_P8,F1_P82       ",    //116
	
	//UT_NTS_iF1B3_85
	"NTS_P9,F1_P83       ",    //117
	
	//DT_NTS_iF1B3_86
	"NTS_P14,F1_P84      ",    //118
	
	//PAL_FLT_iF1B3_87
	"NTS_P25,F1_P69      ",    //119

//FPGA2	
	//L120B_iF2B0_91
	"S10,IC18,IM2,F2_P24 ",    //120
	
	//CEN_iF2B0_92
	"CST-T17,IC19,IM5    ",    //121
	"F1_P86,F2_P129      ",    //122
	"SW1,MD4A,IC15       ",    //123
	
	//TSD_iF2B0_93
	"TSD-H15,F2_P16      ",    //124
	"IC25,IM6            ",    //125
	
	//HSS_iF2B0_94
	"HSS-T17,F2_P15      ",    //126
	"IC16,IM3            ",    //127
	
	//GOV_iF2B0_95
	"GOV_H23,F2_P14      ",    //128
	"IC16,IM3            ",    //129
	
	//S10_iF2B0_96
	"S10,IC16,IM3,F2_P13 ",    //130
	
	//TPH_iF2B0_97
	"TPH-S1,MD4B,IC15    ",    //131
	"F1_P89,F2_P131,12   ",    //132
	"IC16,IM3            ",    //133
	
	//TPL_iF2B0_98
	"TPL-S3,F2_P11       ",    //134
	"IC16,IM3            ",    //135
	
	//LOS_iF2B0_99
	"LOS-S4,F2_P8        ",    //136
	"IC16,IM3            ",    //137
	
	//LPS_iF2B0_100
	"LPS-S5,F2_P7        ",    //138
	"IC16,IM3            ",    //139
	
	//SDF_iF2B0_101
	"CST-T17,NTS_P14     ",    //140
	"SW1,RLY2A,IC26,IM4  ",    //141      
	"MD5A,MD5B,MD6A      ",    //142
	"IC12,IC13,IC15      ",    //143
	"F1_P96,100,85       ",    //144
	"F2_P142,127,6       ",    //145 		 

	//SD_iF2B0_102
	"CST-T17,NTS_Pin15   ",    //146
	"SW1,RLY2A,IC26,IM4  ",    //147
	"MD3A,MD3B,MD6A      ",    //148
	"IC12,IC13,IC15      ",    //149
	"F1_P91,97,85        ",    //150
	"F2_P139,127,5       ",    //151
	
	//UT_iF2B0_103
	"TSD-H15,IC13        ",	   //152	
	"F1_P95,NTS_P9,F2_P4 ",    //153	 
	"IC26,IM4,MD1A       ",    //154
	
	//UN_iF2B0_104
	"CST-T17,SW1         ",    //155
	"F1_P92,NTS_P8,F2_P3 ",    //156
	"IC26,IM4,MD2A,IC13  ",    //157
	
	//DT_iF2B0_105
	"CST-T17,SW1         ",    //158
	"F1_P96,NTS_P14,F2_P2",    //159
	"MD5A,IC26,IM4,IC13  ",    //160
	
	//DN_iF2B0_106
	"CST-T17,SW1         ",    //161
	"F1_P91,NTS_P15,F2_P1",    //162
	"IC26,IM4,MD3A       ",    //163
	
	//ACC_iF2B1_107
	"ACC-T10,IC14,IM8    ",    //164
	"F2_P73              ",    //165
	
	//REAR_DOOR_iF2B1_108
	"CN17_P1,F2_P49      ",    //166
	
	//TEST_MODE_iF2B1_109
	"CN18_P1,F2_P48      ",    //167
	
	//RLM1_iF2B1_110
	"RLM-H1,IC23,IM1     ",    //168
	"F2_P44              ",    //169
	
	//RLM_iF2B1_111
	"RLM-H1,IC23,IM1     ",    //170
	"F2_P43              ",    //171
	
	//DLT1_iF2B1_112
	"DLT-H3,IC23,IM1     ",    //172
	"F2_P42              ",    //173
	
	//DLT_iF2B1_113
	"DLT-H3,IC23,IM1     ",    //174
	"F2_P41              ",    //175
	
	//DLM1_iF2B1_114
	"DLM-H5,IC23,IM1     ",    //176
	"F2_P40              ",    //177
	
	//DLM_iF2B1_115
	"DLM-H5,IC23,IM1     ",    //178
	"F2_P39              ",    //179
		
	//DLB1_iF2B1_116
	"DLB-H7,IC23,IM1     ",    //180
	"F2_P38              ",    //181
	
	//DLB_iF2B1_117
	"DLB-H7,IC23,IM1     ",    //182
	"F2_P37              ",    //183
	
	//TAD_iF2B1_118
	"TAD-H9,IC18,IM2     ",    //184
	"F2_P31              ",    //185
	
	//TAU_iF2B1_119
	"TAU-H10,IC18,IM2    ",    //186
	"F2_P30              ",    //187
	
	//BAD_iF2B1_120
	"BAD-H12,IC18,IM2    ",    //188
	"F2_P29              ",    //189
	
	//BAU_iF2B1_121
	"BAU-H13,IC18,IM2    ",    //190
	"F2_P28              ",    //191
	
	//L120_iF2B1_122
	"S10,IC18,IM2,F2_P27 ",    //192
	
	//IU_iF2B2_123
	"IU-T15,IC17,IM7     ",    //193
	"F2_P110             ",    //194
	
	//ID_iF2B2_124
	"ID-T14,IC17,IM7     ",    //195
	"F2_P109             ",    //196
	
	//MRIN_iF2B2_125
	"ICA-T12,F2_P81      ",    //197
	"SW5,SW7,SW6,IC25,IM6",    //198
	
	//MRIU_iF2B2_126
	"ICA-T12,,F2_P80     ",    //199
	"SW5,SW7,SW6,SW2,SW3 ",    //200
    "IC25,IM6            ",    //201
    
    //MRIE_iF2B2_127
    "ICA-T12,F2_P79      ",    //202
    "SW5,SW7,SW6,SW2     ",    //203
    "IC25,IM6            ",    //204  
    
    //MRID_iF2B2_128
    "ICA-T12,F2_P78      ",    //205
    "SW5,SW7,SW6,SW2,SW4 ",    //206
    "IC25,IM6            ",    //207
    
    //RUN_iF2B2_129
    "CST-T17,IC25,IM6,SW1",    //208
    "F2_P77              ",    //209
    
    //IEN_iF2B2_130
    "IEN-T13,IC14,IM8    ",    //210
    "F2_P75              ",    //211
    
    //ICI_iF2B2_131
    "ICI-T11,IC14,IM8    ",    //212
    "F2_P74              ",    //213
    
    //RGS1_iF2B2_132
    "RGS-T8,IC14,IM8     ",    //214
    "F2_P72              ",    //215
    
    //RGS_iF2B2_133
    "RGS-T8,IC14,IM8     ",    //216
    "F2_P71              ",    //217
    
    //GS1_iF2B2_134
    "GS-T9,IC14,IM8      ",    //218
    "F2_P70              ",    //219
    
    //GS_iF2B2_135
    "GS-T9,IC14,IM8      ",    //220 
    "FPGA2_Pin69	     ",    //221
    
    //NTS_FLT_iF2B2_136
    "NTS_P35,F2_P67      ",    //222
    
    //PAL_FLT_iF2B2_137
    "NTS_P25,F2_P66      ",    //223
    
    //RESET_iF2B2_138
    "CN1,F2_P61          ",    //224
    
    //DZ_iF2B3_139
    "F1_P18,F2_P105      ",    //225
    
    //SU_iF2B3_140
    "CST-T17,NTS_P8      ",    //226
    "SW1,RLY2A,IC26,IM4  ",    //227
    "MD2A,MD2B,MD6A      ",    //228
    "IC12,IC13,IC15      ",    //229
    "F1_P92,98,85        ",    //230
    "F2_P140,127,144     ",    //231
                                
	//SUF_iF2B3_141
	"TSD-H15,CST-T17     ",    //232
	"SW1,RLY2A,NTS_P9    ",    //233
	"MD1A,MD1B,MD6A.IM4  ",    //234
	"IC12,IC13,IC15,IC26 ",    //235
	"F1_P99,95,85        ",    //236
	"F2_P141,127,143     ",    //237 
 
 	//MRSW_iF2B3_142
 	"CST-T17,IC18,IM2,SW1",    //238 
 	"F2_P23              ",    //239
 	
 	//GBP_iF2B3_143
 	"S10,SW5,IC19,IM5    ",    //240
 	"F2_P124             ",    //241
 	
 	//GLB_iF2B3_144       
    "ICA-T12,SW5,SW7,IC19",    //242
    "IM5,F2_P123         ",    //243
    
    //LBP_iF2B3_145
    "S10,SW7,IC19,IM5    ",    //244
    "F2_P122             ",    //245
    
    //IND_iF2B3_146
    "S10,IC19,IM5,SW8    ",    //246
    "F2_P121             ",    //247
    
    //AD_iF2B3_147
    "S10,IC19,IM5,SW9    ",    //248
    "F2_P120             ",    //249
    
    //AUTO_iF2B3_148
    "ICA-T12,SW5,SW7,SW6 ",    //250 
    "IC19,IM5,F2_P119    ",    //251
    
    //MC_iF2B3_149
    "CST-T17,SW1,MD6B    ",    //252 
    "IC15,F1_P87,IC17,IM7",    //253
    "F2_P130,118         ",    //254
    
    //MTO_iF2B3_150
    "MTO,IC17,IM7,F2_P117",    //255
    
    //SPD_iF2B3_151
    "SPD,IC17,IM7,F2_P114",    //256
    
    //CFLT_iF2B3_152
    "CFLT,IC17,IM7       ",    //257 
    "F2_P113             ",    //258
    
    //CST_iF2B3_153
    "CST-T17,IC17,IM7    ",    //259 
    "F2_P112             ",    //260
    
    //CTIN_iF2B3_154
    "INS-T16,IC17,IM7    ",    //261
    "F2_P111             ",    //262
   
//NTS    
    //TEST_MODE_iNTSB0_155
    "CN18_P1,NTS_P32     ",    //263	  
    
//SEB
	//i_sufc_1,i_suc_3,i_sdfc_5,i_sdc_7
	"SW1,RLY2A,RUN_COM   ",    //264     																		
};    
     
	 
    
  
    
     
	
	
    

//  word2nd(#of lines to display)    word1st(is the index of "GALX1121_Fault_Display[]")
//0x0006        					 0012  
 
 #define Max_FaultType		13    
 #define Max_Fault_Index 	64	
 uint32 GALX1121_FaultType_FaultIndex[Max_FaultType][Max_Fault_Index]=
 { 	              
 	//0       1		   2        3        4        5        6        7		 8		  9        10       11       12       13       14       15       16       17       18       19       20       21       22       23       24       25       26       27        28      29       30       31       32       33       34       35       36       37       38       39       40       41       42       43       44       45       46       47       48       49       50       51       52       53       54       55       56       57       58       59       60       61       62       63
 	{0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000},

//SEB Fail ON 			
 	{0x60000, 0x10108, 0x60006, 0x10108, 0x6000C, 0x10108, 0x60012, 0x10108, 0x10018, 0x10019, 0x1001A, 0x1001B, 0x1001C, 0x1001D, 0x1001E, 0x1001F, 0x10020, 0x10021, 0x10022, 0x10023, 0x10024, 0x10025, 0x20026, 0x20028, 0x2002A, 0x1002C, 0x1002D, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000},
//SEB Fail OFF
 	{0x60000, 0x10108, 0x60006, 0x10108, 0x6000C, 0x10108, 0x60012, 0x10108, 0x10018, 0x10019, 0x1001A, 0x1001B, 0x1001C, 0x1001D, 0x1001E, 0x1001F, 0x10020, 0x10021, 0x10022, 0x10023, 0x10024, 0x10025, 0x20026, 0x20028, 0x2002A, 0x1002C, 0x0002D, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000},
//SEB Fail SHORT
	{0x60000, 0x10108, 0x60006, 0x10108, 0x6000C, 0x10108, 0x60012, 0x10108, 0x10018, 0x10019, 0x1001A, 0x1001B, 0x1001C, 0x1001D, 0x1001E, 0x1001F, 0x10020, 0x10021, 0x10022, 0x10023, 0x10024, 0x10025, 0x20026, 0x20028, 0x2002A, 0x1002C, 0x0002D, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000},
 
//FPGA1 Fail ON
	{0x2002E, 0x20030, 0x20032, 0x20034, 0x20036, 0x20038, 0x2003A, 0x2003C, 0x2003E, 0x20040, 0x20042, 0x20044, 0x20046, 0x20048, 0x2004A, 0x2004C, 0x1004E, 0x1004F, 0x10050, 0x10051, 0x10052, 0x10053, 0x10054, 0x10055, 0x00000, 0x30056, 0x30059, 0x2005C, 0x2005E, 0x20060, 0x20062, 0x20064, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x10066, 0x10067, 0x10068, 0x10069, 0x00000, 0x1006A, 0x1006B, 0x00000, 0x1006C, 0x1006D, 0x1006E, 0x1006F, 0x10070, 0x10071, 0x10072, 0x10073, 0x10074, 0x10075, 0x10076, 0x10077, 0x00000, 0x00000, 0x00000},
//FPGA1 Fail OFF
	{0x2002E, 0x20030, 0x20032, 0x20034, 0x20036, 0x20038, 0x2003A, 0x2003C, 0x2003E, 0x20040, 0x20042, 0x20044, 0x20046, 0x20048, 0x2004A, 0x2004C, 0x1004E, 0x1004F, 0x10050, 0x10051, 0x10052, 0x10053, 0x10054, 0x10055, 0x00000, 0x30056, 0x30059, 0x2005C, 0x2005E, 0x20060, 0x20062, 0x20064, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x10066, 0x10067, 0x10068, 0x10069, 0x00000, 0x1006A, 0x1006B, 0x00000, 0x1006C, 0x1006D, 0x1006E, 0x1006F, 0x10070, 0x10071, 0x10072, 0x10073, 0x10074, 0x10075, 0x10076, 0x10077, 0x00000, 0x00000, 0x00000},
//FPGA1 Fail SHORT
	{0x2002E, 0x20030, 0x20032, 0x20034, 0x20036, 0x20038, 0x2003A, 0x2003C, 0x2003E, 0x20040, 0x20042, 0x20044, 0x20046, 0x20048, 0x2004A, 0x2004C, 0x1004E, 0x1004F, 0x10050, 0x10051, 0x10052, 0x10053, 0x10054, 0x10055, 0x00000, 0x30056, 0x30059, 0x2005C, 0x2005E, 0x20060, 0x20062, 0x20064, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x10066, 0x10067, 0x10068, 0x10069, 0x00000, 0x1006A, 0x1006B, 0x00000, 0x1006C, 0x1006D, 0x1006E, 0x1006F, 0x10070, 0x10071, 0x10072, 0x10073, 0x10074, 0x10075, 0x10076, 0x10077, 0x00000, 0x00000, 0x00000},

//FPGA2 Fail ON	
	{0x10078, 0x30079, 0x2007C, 0x2007E, 0x20080, 0x10082, 0x30083, 0x20086, 0x20088, 0x2008A, 0x6008C, 0x60092, 0x30098, 0x3009B, 0x3009E, 0x300A1, 0x200A4, 0x100A6, 0x100A7, 0x200A8, 0x200AA, 0x200AC, 0x200AE, 0x200B0, 0x200B2, 0x200B4, 0x200B6, 0x200B8, 0x200BA, 0x200BC, 0x200BE, 0x100C0, 0x200C1, 0x200C3, 0x200C5, 0x300C7, 0x300CA, 0x300CD, 0x200D0, 0x200D2, 0x200D4, 0x200D6, 0x200D8, 0x200DA, 0x200DC, 0x100DE, 0x100DF, 0x100E0, 0x100E1, 0x600E2, 0x600E8, 0x200EE, 0x200F0, 0x200F2, 0x200F4, 0x200F6, 0x200F8, 0x200FA, 0x300FC, 0x100FF, 0x10100, 0x20101, 0x20103, 0x20105},
//FPGA2 Fail OFF
	{0x10078, 0x30079, 0x2007C, 0x2007E, 0x20080, 0x10082, 0x30083, 0x20086, 0x20088, 0x2008A, 0x6008C, 0x60092, 0x30098, 0x3009B, 0x3009E, 0x300A1, 0x200A4, 0x100A6, 0x100A7, 0x200A8, 0x200AA, 0x200AC, 0x200AE, 0x200B0, 0x200B2, 0x200B4, 0x200B6, 0x200B8, 0x200BA, 0x200BC, 0x200BE, 0x100C0, 0x200C1, 0x200C3, 0x200C5, 0x300C7, 0x300CA, 0x300CD, 0x200D0, 0x200D2, 0x200D4, 0x200D6, 0x200D8, 0x200DA, 0x200DC, 0x100DE, 0x100DF, 0x100E0, 0x100E1, 0x600E2, 0x600E8, 0x200EE, 0x200F0, 0x200F2, 0x200F4, 0x200F6, 0x200F8, 0x200FA, 0x300FC, 0x100FF, 0x10100, 0x20101, 0x20103, 0x20105},
//FPGA1 Fail SHORT	
	{0x10078, 0x30079, 0x2007C, 0x2007E, 0x20080, 0x10082, 0x30083, 0x20086, 0x20088, 0x2008A, 0x6008C, 0x60092, 0x30098, 0x3009B, 0x3009E, 0x300A1, 0x200A4, 0x100A6, 0x100A7, 0x200A8, 0x200AA, 0x200AC, 0x200AE, 0x200B0, 0x200B2, 0x200B4, 0x200B6, 0x200B8, 0x200BA, 0x200BC, 0x200BE, 0x100C0, 0x200C1, 0x200C3, 0x200C5, 0x300C7, 0x300CA, 0x300CD, 0x200D0, 0x200D2, 0x200D4, 0x200D6, 0x200D8, 0x200DA, 0x200DC, 0x100DE, 0x100DF, 0x100E0, 0x100E1, 0x600E2, 0x600E8, 0x200EE, 0x200F0, 0x200F2, 0x200F4, 0x200F6, 0x200F8, 0x200FA, 0x300FC, 0x100FF, 0x10100, 0x20101, 0x20103, 0x20105},

//NTS Fail ON	
	{0x10107},
//NTS Fail OFF	
	{0x10107},
//NTS Fail SHORT	
	{0x10107},		
 };







	
//------------------------------------------------------------------------------
int16 Exclude_Short_SEB_Signal[glax1121_SEB_input_io_num];

#define FPGA1_BYTE_NUM	4
#define FPGA1_BIT_NUM	16
int16 Exclude_Short_FPGA1_Signal[FPGA1_BYTE_NUM][FPGA1_BIT_NUM];
int16 FPGA1_BYTE=0;
int16 FPGA1_BIT=0;

#define FPGA2_BYTE_NUM	4
#define FPGA2_BIT_NUM	16
int16 Exclude_Short_FPGA2_Signal[FPGA2_BYTE_NUM][FPGA2_BIT_NUM];
int16 FPGA2_BYTE=0;
int16 FPGA2_BIT=0;

#define NTS_BYTE_NUM	1
#define NTS_BIT_NUM	8
int16 Exclude_Short_NTS_Signal[NTS_BYTE_NUM][NTS_BIT_NUM];
int16 NTS_BYTE=0;
int16 NTS_BIT=0;



int16 Shorted_Sig_index=0;
int16 Shorted_Sig_type=0;
int16 Fail_ON_Sig_index=0;
int16 Fail_ON_Sig_type=0;
int16 Fail_OFF_Sig_index=0;
int16 Fail_OFF_Sig_type=0;


int16 Fail_Sig_Type=0;
int16 Fail_Sig_Index=0;
        
#define word1st		0x0000FFFF
#define word2nd		0xFFFF0000

     
     
//------------------------------------------------------------------------------
int Check_Input_Shorted(void)
{	
	int16 i=0;
	int16 j,k=0;
	int16 l,m=0;
	int16 o,p=0;
	
	//check to see if any other SEB Input Signal is turned ON	
	for(i=0;i<glax1121_SEB_input_io_num;i++)
	{
		if((rdinp(glax1121_SEB_input_io_array[i])==1)&&(Exclude_Short_SEB_Signal[i]==0))
		{		
			//SEB Input Index				
			Shorted_Sig_index=i;	
			Shorted_Sig_type=Shorted_Type_SEB;				
			return Shorted_Type_SEB;					
		}
	}
	
	//check to see if any other FPGA1 Input Signal is turned ON
	for(j=0;j<FPGA1_BYTE_NUM;j++)
	{
		for(k=0;k<FPGA1_BIT_NUM;k++)
		{
			if(((i1spi_in.buff[j]&BIT_16[k])==BIT_16[k])&&(Exclude_Short_FPGA1_Signal[j][k]==0))
			{
				//FPGA1 Input Index
				Shorted_Sig_index=(j*FPGA1_BIT_NUM)+k;	
				Shorted_Sig_type=Shorted_Type_FPGA1;	
				return Shorted_Type_FPGA1;		  
			}
		}
	}
	
	//check to see if any other FPGA2 Input Signal is turned ON
	for(l=0;l<FPGA2_BYTE_NUM;l++)
	{
		for(m=0;m<FPGA2_BIT_NUM;m++)
		{
			if(((iospi_in.buff[l]&BIT_16[m])==BIT_16[m])&&(Exclude_Short_FPGA2_Signal[l][m]==0))
			{
				//FPGA2 Input Index
				Shorted_Sig_index=(l*FPGA2_BIT_NUM)+m;	
				Shorted_Sig_type=Shorted_Type_FPGA2;	
				return Shorted_Type_FPGA2;		  
			}
		}
	}
	
	//check to see if any other NTS Input Signal is turned ON
	for(o=0;o<NTS_BYTE_NUM;o++)
	{
		for(p=0;p<NTS_BIT_NUM;p++)
		{
			if(((galx_1121tester_NTS_Modify_Input.buff[o]&BIT[p])==BIT[p])&&(Exclude_Short_NTS_Signal[o][p]==0))
			{
				//NTS Input Index
				Shorted_Sig_index=(o*NTS_BIT_NUM)+p;	
				Shorted_Sig_type=Shorted_Type_NTS;	
				return Shorted_Type_NTS;		  
			}
		}
	}
	
	return 0;
}




void Exclude_Short_Signal(int16 Exclude_Signal)
{
	//SEB Exclude 
	if((Input_Signal_Detail[Exclude_Signal][Input_TYPE]&Input_Type_SEB)==Input_Type_SEB)
	{
		Exclude_Short_SEB_Signal[Input_Signal_Detail[Exclude_Signal][Input_INDEX]]=1;
	}
	
	//FPGA1 Exclude 
	if((Input_Signal_Detail[Exclude_Signal][Input_TYPE]&Input_Type_FPGA1)==Input_Type_FPGA1)
	{		
		FPGA1_BYTE=Input_Signal_Detail[Exclude_Signal][Input_INDEX]/FPGA1_BIT_NUM;
		FPGA1_BIT=Input_Signal_Detail[Exclude_Signal][Input_INDEX]%FPGA1_BIT_NUM;
		Exclude_Short_FPGA1_Signal[FPGA1_BYTE][FPGA1_BIT]=1;
	}
	
	//FPGA2 Exclude
	if((Input_Signal_Detail[Exclude_Signal][Input_TYPE]&Input_Type_FPGA2)==Input_Type_FPGA2)
	{		
		FPGA2_BYTE=Input_Signal_Detail[Exclude_Signal][Input_INDEX]/FPGA2_BIT_NUM;
		FPGA2_BIT=Input_Signal_Detail[Exclude_Signal][Input_INDEX]%FPGA2_BIT_NUM;
		Exclude_Short_FPGA2_Signal[FPGA2_BYTE][FPGA2_BIT]=1;
	}
	
	//NTS Exclude
	if((Input_Signal_Detail[Exclude_Signal][Input_TYPE]&Input_Type_NTS)==Input_Type_NTS)
	{
		NTS_BYTE=Input_Signal_Detail[Exclude_Signal][Input_INDEX]/NTS_BIT_NUM;
		NTS_BIT=Input_Signal_Detail[Exclude_Signal][Input_INDEX]%NTS_BIT_NUM;
		Exclude_Short_NTS_Signal[NTS_BYTE][NTS_BIT]=1;
	}
}





void Include_Short_Signal(int16 Include_Signal)
{
	//SEB Include
	if((Input_Signal_Detail[Include_Signal][Input_TYPE]&Input_Type_SEB)==Input_Type_SEB)
	{
		Exclude_Short_SEB_Signal[Input_Signal_Detail[Include_Signal][Input_INDEX]]=0;
	}
	
	//FPGA1 Include
	if((Input_Signal_Detail[Include_Signal][Input_TYPE]&Input_Type_FPGA1)==Input_Type_FPGA1)
	{		
		FPGA1_BYTE=Input_Signal_Detail[Include_Signal][Input_INDEX]/FPGA1_BIT_NUM;
		FPGA1_BIT=Input_Signal_Detail[Include_Signal][Input_INDEX]%FPGA1_BIT_NUM;
		Exclude_Short_FPGA1_Signal[FPGA1_BYTE][FPGA1_BIT]=0;
	}
	
	//FPGA2 Include
	if((Input_Signal_Detail[Include_Signal][Input_TYPE]&Input_Type_FPGA2)==Input_Type_FPGA2)
	{		
		FPGA2_BYTE=Input_Signal_Detail[Include_Signal][Input_INDEX]/FPGA2_BIT_NUM;
		FPGA2_BIT=Input_Signal_Detail[Include_Signal][Input_INDEX]%FPGA2_BIT_NUM;
		Exclude_Short_FPGA2_Signal[FPGA2_BYTE][FPGA2_BIT]=0;
	}
	
	//NTS Include
	if((Input_Signal_Detail[Include_Signal][Input_TYPE]&Input_Type_NTS)==Input_Type_NTS)
	{
		NTS_BYTE=Input_Signal_Detail[Include_Signal][Input_INDEX]/NTS_BIT_NUM;
		NTS_BIT=Input_Signal_Detail[Include_Signal][Input_INDEX]%NTS_BIT_NUM;
		Exclude_Short_NTS_Signal[NTS_BYTE][NTS_BIT]=0;
	}
}





//------------------------------------------------------------------------------
void Turn_Signal_ON(int16 Signal_ON)
{
	//SEB CAN Output
	if((Output_Signal_Detail[Signal_ON][Output_TYPE]&Output_Type_SEB)==Output_Type_SEB)
	{
		 setoutp(glax1121_SEB_output_io_array[Output_Signal_Detail[Signal_ON][Output_INDEX]]);
	}
	
	//FPGA1 SPI Output
	if((Output_Signal_Detail[Signal_ON][Output_TYPE]&Output_Type_FPGA1)==Output_Type_FPGA1)
	{
		FPGA1_HW_Modify_Output.mc_drop_delay|=BIT_16[Output_Signal_Detail[Signal_ON][Output_INDEX]];
	}
	
	//FPGA2 SPI Output
	if((Output_Signal_Detail[Signal_ON][Output_TYPE]&Output_Type_FPGA2)==Output_Type_FPGA2)
	{
		iospi_out.ST.buff0.R|=BIT_16[Output_Signal_Detail[Signal_ON][Output_INDEX]];
	}
	
	//NTS SPI Output
	if((Output_Signal_Detail[Signal_ON][Output_TYPE]&Output_Type_NTS_SPI)==Output_Type_NTS_SPI)
	{
		NTS_HW_Modify_Output.byte|=BIT[Output_Signal_Detail[Signal_ON][Output_INDEX]];
	}
	
	//NTS CAN Output
	if((Output_Signal_Detail[Signal_ON][Output_TYPE]&Output_Type_NTS_CAN)==Output_Type_NTS_CAN)
	{
		galx_1121tester_NTS_Modify_Output.byte|=BIT[Output_Signal_Detail[Signal_ON][Output_INDEX]];
	}
}




void Turn_Signal_OFF(int16 Signal_OFF)
{
	//SEB CAN Output
	if((Output_Signal_Detail[Signal_OFF][Output_TYPE]&Output_Type_SEB)==Output_Type_SEB)
	{
		 clroutp(glax1121_SEB_output_io_array[Output_Signal_Detail[Signal_OFF][Output_INDEX]]);
	}
	
	//FPGA1 SPI Output
	if((Output_Signal_Detail[Signal_OFF][Output_TYPE]&Output_Type_FPGA1)==Output_Type_FPGA1)
	{
		FPGA1_HW_Modify_Output.mc_drop_delay&=(!BIT_16[Output_Signal_Detail[Signal_OFF][Output_INDEX]]);
	}
	
	//FPGA2 SPI Output
	if((Output_Signal_Detail[Signal_OFF][Output_TYPE]&Output_Type_FPGA2)==Output_Type_FPGA2)
	{
		iospi_out.ST.buff0.R&=(!BIT_16[Output_Signal_Detail[Signal_OFF][Output_INDEX]]);
	}
	
	//NTS SPI Output
	if((Output_Signal_Detail[Signal_OFF][Output_TYPE]&Output_Type_NTS_SPI)==Output_Type_NTS_SPI)
	{
		NTS_HW_Modify_Output.byte&=(!BIT[Output_Signal_Detail[Signal_OFF][Output_INDEX]]);
	}
	
	//NTS CAN Output
	if((Output_Signal_Detail[Signal_OFF][Output_TYPE]&Output_Type_NTS_CAN)==Output_Type_NTS_CAN)
	{
		galx_1121tester_NTS_Modify_Output.byte&=(!BIT[Output_Signal_Detail[Signal_OFF][Output_INDEX]]);
	}
}






int16 Check_Signal_ON(int16 Signal_ON)
{
	//SEB Check ON 
	if((Input_Signal_Detail[Signal_ON][Input_TYPE]&Input_Type_SEB)==Input_Type_SEB)
	{
		if(rdinp(glax1121_SEB_input_io_array[Input_Signal_Detail[Signal_ON][Input_INDEX]])!=1)
		{
			Fail_ON_Sig_index=Input_Signal_Detail[Signal_ON][Input_INDEX];
			Fail_ON_Sig_type=Fail_ON_Type_SEB;
			return Fail_ON_Type_SEB;
		}
	}
	
	//FPGA1 Check ON 
	if((Input_Signal_Detail[Signal_ON][Input_TYPE]&Input_Type_FPGA1)==Input_Type_FPGA1)
	{		
		FPGA1_BYTE=Input_Signal_Detail[Signal_ON][Input_INDEX]/FPGA1_BIT_NUM;
		FPGA1_BIT=Input_Signal_Detail[Signal_ON][Input_INDEX]%FPGA1_BIT_NUM;
		if((i1spi_in.buff[FPGA1_BYTE]&BIT_16[FPGA1_BIT])!=BIT_16[FPGA1_BIT])
		{
			Fail_ON_Sig_index=Input_Signal_Detail[Signal_ON][Input_INDEX];
			Fail_ON_Sig_type=Fail_ON_Type_FPGA1;
//-			sprintf(&LCD_Display[1][1],"B1=0x%04X",Input_Signal_Detail[Signal_ON][Input_TYPE]);
//-			sprintf(&LCD_Display[1][11],"B1=0x%04X",Input_Type_FPGA1);
			return Fail_ON_Type_FPGA1;
		}
	}

	//FPGA2 Check ON
	if((Input_Signal_Detail[Signal_ON][Input_TYPE]&Input_Type_FPGA2)==Input_Type_FPGA2)
	{		
		FPGA2_BYTE=Input_Signal_Detail[Signal_ON][Input_INDEX]/FPGA2_BIT_NUM;
		FPGA2_BIT=Input_Signal_Detail[Signal_ON][Input_INDEX]%FPGA2_BIT_NUM;
		if((iospi_in.buff[FPGA2_BYTE]&BIT_16[FPGA2_BIT])!=BIT_16[FPGA2_BIT])
		{
			Fail_ON_Sig_index=Input_Signal_Detail[Signal_ON][Input_INDEX];
			Fail_ON_Sig_type=Fail_ON_Type_FPGA2;
//-			sprintf(&LCD_Display[2][1],"B2=0x%04X",Input_Signal_Detail[Signal_ON][Input_TYPE]);
//-			sprintf(&LCD_Display[2][11],"B2=0x%04X",Input_Type_FPGA2);
			return Fail_ON_Type_FPGA2;
		}
	}
 		
	//NTS Check ON
	if((Input_Signal_Detail[Signal_ON][Input_TYPE]&Input_Type_NTS)==Input_Type_NTS)
	{
		NTS_BYTE=Input_Signal_Detail[Signal_ON][Input_INDEX]/NTS_BIT_NUM;
		NTS_BIT=Input_Signal_Detail[Signal_ON][Input_INDEX]%NTS_BIT_NUM;
		if((galx_1121tester_NTS_Modify_Input.buff[NTS_BYTE]&BIT[NTS_BIT])!=BIT[NTS_BIT])
		{
			Fail_ON_Sig_index=Input_Signal_Detail[Signal_ON][Input_INDEX];
			Fail_ON_Sig_type=Fail_ON_Type_NTS;
			return Fail_ON_Type_NTS;
		}
	}
 
	return 0;	
}




int16 Check_Signal_OFF(int16 Signal_OFF)
{
	//SEB Check OFF 
	if((Input_Signal_Detail[Signal_OFF][Input_TYPE]&Input_Type_SEB)==Input_Type_SEB)
	{
		if(rdinp(glax1121_SEB_input_io_array[Input_Signal_Detail[Signal_OFF][Input_INDEX]])!=0)
		{
			Fail_OFF_Sig_index=Input_Signal_Detail[Signal_OFF][Input_INDEX];
			Fail_OFF_Sig_type=Fail_OFF_Type_SEB;
			return Fail_OFF_Type_SEB;
		}
	}
	
	//FPGA1 Check OFF 
	if((Input_Signal_Detail[Signal_OFF][Input_TYPE]&Input_Type_FPGA1)==Input_Type_FPGA1)
	{		
		FPGA1_BYTE=Input_Signal_Detail[Signal_OFF][Input_INDEX]/FPGA1_BIT_NUM;
		FPGA1_BIT=Input_Signal_Detail[Signal_OFF][Input_INDEX]%FPGA1_BIT_NUM;
		if((i1spi_in.buff[FPGA1_BYTE]&BIT_16[FPGA1_BIT])==BIT_16[FPGA1_BIT])
		{
			Fail_OFF_Sig_index=Input_Signal_Detail[Signal_OFF][Input_INDEX];
			Fail_OFF_Sig_type=Fail_OFF_Type_FPGA1;
			return Fail_OFF_Type_FPGA1;
		}
	}

	//FPGA2 Check OFF
	if((Input_Signal_Detail[Signal_OFF][Input_TYPE]&Input_Type_FPGA2)==Input_Type_FPGA2)
	{		
		FPGA2_BYTE=Input_Signal_Detail[Signal_OFF][Input_INDEX]/FPGA2_BIT_NUM;
		FPGA2_BIT=Input_Signal_Detail[Signal_OFF][Input_INDEX]%FPGA2_BIT_NUM;
		if((iospi_in.buff[FPGA2_BYTE]&BIT_16[FPGA2_BIT])==BIT_16[FPGA2_BIT])
		{
			Fail_OFF_Sig_index=Input_Signal_Detail[Signal_OFF][Input_INDEX];
			Fail_OFF_Sig_type=Fail_OFF_Type_FPGA2;
			return Fail_OFF_Type_FPGA2;
		}
	}
 		
	//NTS Check OFF
	if((Input_Signal_Detail[Signal_OFF][Input_TYPE]&Input_Type_NTS)==Input_Type_NTS)
	{
		NTS_BYTE=Input_Signal_Detail[Signal_OFF][Input_INDEX]/NTS_BIT_NUM;
		NTS_BIT=Input_Signal_Detail[Signal_OFF][Input_INDEX]%NTS_BIT_NUM;
		if((galx_1121tester_NTS_Modify_Input.buff[NTS_BYTE]&BIT[NTS_BIT])==BIT[NTS_BIT])
		{
			Fail_OFF_Sig_index=Input_Signal_Detail[Signal_OFF][Input_INDEX];
			Fail_OFF_Sig_type=Fail_OFF_Type_NTS;
			return Fail_OFF_Type_NTS;
		}
	}
 
	return 0;	
}   






//------------------------------------------------------------------------------
void Disply_Fault(void)
{
	int16 i=0;
	
	
	//Fail ON
	if(Fail_ON_Sig_type!=0)
	{
		//SEB Fail ON
		if(Fail_ON_Sig_type==Fail_ON_Type_SEB)
		{
			New_write_display(SEB_Input_Name[Fail_ON_Sig_index],2,0);			
		}
		
		//FPGA1 Fail ON
		if(Fail_ON_Sig_type==Fail_ON_Type_FPGA1)
		{
			New_write_display(FPGA1_Input_Name[Fail_ON_Sig_index],2,0);
		}
		
		//FPGA2 Fail ON
		if(Fail_ON_Sig_type==Fail_ON_Type_FPGA2)
		{
			New_write_display(FPGA2_Input_Name[Fail_ON_Sig_index],2,0);
		}
		
		//NTS Fail ON
		if(Fail_ON_Sig_type==Fail_ON_Type_NTS)
		{
			New_write_display(NTS_Input_Name[Fail_ON_Sig_index],2,0);
		}
					
 	
//-		sprintf(&LCD_Display[1][8],"T=%02Xh", Fail_ON_Sig_type);
//-		sprintf(&LCD_Display[1][14],"Ih=%02Xh", Fail_ON_Sig_index);
		
		
		New_write_display("is OFF. Failed ON   ",3,0); 
		
		Fail_Sig_Type=Fail_ON_Sig_type;
		Fail_Sig_Index=Fail_ON_Sig_index;
	}
	         
	
	//Fail OFF
	if(Fail_OFF_Sig_type!=0)
	{
		//SEB Fail OFF
		if(Fail_OFF_Sig_type==Fail_OFF_Type_SEB)
		{
			New_write_display(SEB_Input_Name[Fail_OFF_Sig_index],2,0);			
		}
		
		//FPGA1 Fail OFF
		if(Fail_OFF_Sig_type==Fail_OFF_Type_FPGA1)
		{
			New_write_display(FPGA1_Input_Name[Fail_OFF_Sig_index],2,0);
		}
		
		//FPGA2 Fail OFF
		if(Fail_OFF_Sig_type==Fail_OFF_Type_FPGA2)
		{
			New_write_display(FPGA2_Input_Name[Fail_OFF_Sig_index],2,0);
		}
		
		//NTS Fail OFF
		if(Fail_OFF_Sig_type==Fail_OFF_Type_NTS)
		{
			New_write_display(NTS_Input_Name[Fail_OFF_Sig_index],2,0);
		}
		
 
//-		sprintf(&LCD_Display[1][8],"T=%02Xh", Fail_OFF_Sig_type);
//-		sprintf(&LCD_Display[1][14],"I=%02Xh", Fail_OFF_Sig_index);	
			 
 		New_write_display("is ON. Failed OFF   ",3,0); 
				
		Fail_Sig_Type=Fail_OFF_Sig_type;
		Fail_Sig_Index=Fail_OFF_Sig_index;
	}
	
	
	//Fail Short
	if(Shorted_Sig_type!=0)
	{
		//SEB Fail Short 	
		if(Shorted_Sig_type==Shorted_Type_SEB)
		{
			New_write_display(SEB_Input_Name[Shorted_Sig_index],2,0);
		}
	
		//FPGA1 Fail Short
		if(Shorted_Sig_type==Shorted_Type_FPGA1)
		{
			New_write_display(FPGA1_Input_Name[Shorted_Sig_index],2,0);
		}
	
		//FPGA2 Fail Short
		if(Shorted_Sig_type==Shorted_Type_FPGA2)
		{
			New_write_display(FPGA2_Input_Name[Shorted_Sig_index],2,0);
		}
	
		//NTS Fail Short
		if(Shorted_Sig_type==Shorted_Type_NTS)
		{
			New_write_display(NTS_Input_Name[Shorted_Sig_index],2,0);
		}
		
//-		sprintf(&LCD_Display[1][8],"T=%02Xh", Shorted_Sig_type);
//-		sprintf(&LCD_Display[1][14],"I=%02Xh", Shorted_Sig_index);

 		New_write_display("is Shorted.         ",3,0);
				
		Fail_Sig_Type=Shorted_Sig_type;
		Fail_Sig_Index=Shorted_Sig_index;
	}		
	
//- sprintf(&LCD_Display[2][1],"%04X", i1spi_in.buff[0]);   //i1spi_in.buff[0]		  
//- sprintf(&LCD_Display[2][6],"%04X", i1spi_in.buff[1]);	//i1spi_in.buff[1]
//- sprintf(&LCD_Display[2][11],"%04X", i1spi_in.buff[2]);	//i1spi_in.buff[2]	
//- sprintf(&LCD_Display[2][16],"%04X", i1spi_in.buff[3]);	//i1spi_in.buff[3]	
    
//- sprintf(&LCD_Display[3][1],"%04X", iospi_in.buff[0]);   //iospi_in.buff[0]	 
//- sprintf(&LCD_Display[3][6],"%04X", iospi_in.buff[1]);	//iospi_in.buff[1]	
//- sprintf(&LCD_Display[3][11],"%04X",iospi_in.buff[2]);	//iospi_in.buff[2]
//- sprintf(&LCD_Display[3][16],"%04X", iospi_in.buff[3]);	//iospi_in.buff[3]    
     
   
     
//Display Detail of the Faults   
	GALX1121_Fault_Display_ADDIN_Function();
		  
    for(i=(GALX1121_FaultType_FaultIndex[Fail_Sig_Type][Fail_Sig_Index])&word1st;
    	i<(((GALX1121_FaultType_FaultIndex[Fail_Sig_Type][Fail_Sig_Index])&word1st)+(((GALX1121_FaultType_FaultIndex[Fail_Sig_Type][Fail_Sig_Index])&word2nd)>>16));
    	i++)
    {
    	Log_an_event(GALX1121_Fault_Display[i]);
    }  
    
    sprintf(src,"FType=%2.2i FIndex=%2.2i", Fail_Sig_Type,Fail_Sig_Index);
    Log_an_event(src);   
}




void Reset_Fault_Type_Index(void)
{
	Shorted_Sig_index=0;
	Shorted_Sig_type=0;

	Fail_ON_Sig_index=0;
	Fail_ON_Sig_type=0;

	Fail_OFF_Sig_index=0;
	Fail_OFF_Sig_type=0;
}
   
   
   
   


//------------------------------------------------------------------------------   
void Turn_ALL_Output_OFF(void)
{
	int16 i=0;
	
	for(i=o_S5LPS_0;i<glax1121_ALL_output_io_num;i++)
	{
		Turn_Signal_OFF(i);
	}	
}
   
   
   
   
void Include_ALL_Short_Signal(void) 
{
	int16 i=0;
	
	for(i=i_suf_0;i<glax1121_ALL_input_io_num;i++)
	{
		Include_Short_Signal(i);
	}	
}
   
   





//------------------------------------------------------------------------------ 
void GALX1121_Fault_Display_ADDIN_Function(void)
{
	int16 i=0;
	for(i=0;i<GALX1121_Fault_Display_ADDIN_Index;i++)
	{
		Log_an_event(GALX1121_Fault_Display_ADDIN[i]);
	}		
}































