#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"
#include "v120AC_Power_Check.h"





int16 FPGA1_IO_TEST(int16 Operation);



enum
{
	FPGA1_IO_TEST_TEMP_Input_Index=0,
	FPGA1_IO_TEST_TEMP_Output_Index
};


#define FPGA1_IO_TEST_TEMP_IO_num	22
int16 FPGA1_IO_TEST_TEMP_ARRAY[FPGA1_IO_TEST_TEMP_IO_num][2]=
{
	//FPGA2 Output to FPGA1 ONLY
	//Input					Output
	{UL_CPU_iF1B1_43,		LUL_CPU_oF2B0_50},		//0
	{DNF_CPU_iF1B1_44,		SDF_CPU_oF2B0_54},		//1
	{UPF_CPU_iF1B1_45,		SUF_CPU_oF2B0_53},		//2
	{UPR_CPU_iF1B1_46,		SU_CPU_oF2B0_52},		//3
	{DNR_CPU_iF1B1_47,		SD_CPU_oF2B0_51},		//4
	{STE_CPU_iF1B1_48,		STE_CPU_oF2B0_58},		//5
	{MC_CPU_iF1B1_49,		MC_CPU_oF2B0_57},		//6
	{CEN_CPU_iF1B1_50,		CEN_CPU_oF2B0_56},		//7
	{RUN_CPU_iF1B2_68,		RUN_CPU_oF2B0_55},		//8
	{SEL_OK_CPU_iF1B2_69,	SELOK_CPU_oF2B0_59},	//9
	{SPD_75_CPU_iF1B2_70,	SPD75_CPU_oF2B0_61},	//10
	{SPD_150_CPU_iF1B2_71,	SPD150_CPU_oF2B0_62},	//11
	{DL_CPU_iF1B2_73,		LDL_CPU_oF2B0_48},      //12
	{DZ_CPU_iF1B2_74,		DZ_CPU_oF2B0_49},       //13
	
	//NTS Output to FPGA1 ONLY
	//Input					Output
	{SPD_75_NTS_iF1B3_79,	SPD_75_NTS_CANout_74},  //14
	{SPD_150_NTS_iF1B3_80,	SPD_150_NTS_CANout_73}, //15
	{SEL_OK_NTS_iF1B3_81,	SEL_OK_NTS_CANout_76},  //16
	{DZ_NTS_iF1B3_82,		DZ_NTS_CANout_72},  	//17
	{DN_NTS_iF1B3_83,		DN_NTS_SPIout_67},  	//18
	{UN_NTS_iF1B3_84,		UN_NTS_SPIout_65},  	//19
	{UT_NTS_iF1B3_85,		UT_NTS_SPIout_64},  	//20
	{DT_NTS_iF1B3_86,		DT_NTS_SPIout_66},  	//21
};





enum
{
	FPGA1_IO_Test_Init=0,
	TurnON_FPGA1_IO_ACC_L,
    CheckON_FPGA1_IO_ACC_L,
    TurnOFF_FPGA1_IO_ACC_L,
    CheckOFF_FPGA1_IO_ACC_L,
    
    TurnON_FPGA1_IO_GS_L,
    CheckON_FPGA1_IO_GS_L,
    TurnOFF_FPGA1_IO_GS_L,
    CheckOFF_FPGA1_IO_GS_L,
    
    TurnON_FPGA1_IO_RGS_L,
    CheckON_FPGA1_IO_RGS_L,
    TurnOFF_FPGA1_IO_RGS_L,
    CheckOFF_FPGA1_IO_RGS_L,
    
    TurnON_FPGA1_IO_LPS_L,
    CheckON_FPGA1_IO_LPS_L,
    TurnOFF_FPGA1_IO_LPS_L,
    CheckOFF_FPGA1_IO_LPS_L,
    
    TurnON_FPGA1_IO_RLM_L,
    CheckON_FPGA1_IO_RLM_L,
    TurnOFF_FPGA1_IO_RLM_L,
    CheckOFF_FPGA1_IO_RLM_L,
    
    TurnON_FPGA1_IO_DLT_L,
    CheckON_FPGA1_IO_DLT_L,
    TurnOFF_FPGA1_IO_DLT_L,
    CheckOFF_FPGA1_IO_DLT_L,
    
    TurnON_FPGA1_IO_DLM_L,
    CheckON_FPGA1_IO_DLM_L,
    TurnOFF_FPGA1_IO_DLM_L,
    CheckOFF_FPGA1_IO_DLM_L,
    
    TurnON_FPGA1_IO_DLB_L,
    CheckON_FPGA1_IO_DLB_L,
    TurnOFF_FPGA1_IO_DLB_L,
    CheckOFF_FPGA1_IO_DLB_L,
    
    TurnON_FPGA1_IO_TAD_L,
    CheckON_FPGA1_IO_TAD_L,
    TurnOFF_FPGA1_IO_TAD_L,
    CheckOFF_FPGA1_IO_TAD_L,
    
    TurnON_FPGA1_IO_TAU_L,
    CheckON_FPGA1_IO_TAU_L,
    TurnOFF_FPGA1_IO_TAU_L,
    CheckOFF_FPGA1_IO_TAU_L,
        
    TurnON_FPGA1_IO_BAD_L,
    CheckON_FPGA1_IO_BAD_L,
    TurnOFF_FPGA1_IO_BAD_L,
    CheckOFF_FPGA1_IO_BAD_L,
    
    TurnON_FPGA1_IO_BAU_L,
    CheckON_FPGA1_IO_BAU_L,
    TurnOFF_FPGA1_IO_BAU_L,
    CheckOFF_FPGA1_IO_BAU_L,

    TurnON_FPGA1_IO_ID_L,
    CheckON_FPGA1_IO_ID_L,
    TurnOFF_FPGA1_IO_ID_L,
    CheckOFF_FPGA1_IO_ID_L,
    
    TurnON_FPGA1_IO_IU_L,
    CheckON_FPGA1_IO_IU_L,
    TurnOFF_FPGA1_IO_IU_L,
    CheckOFF_FPGA1_IO_IU_L,
    
    TurnON_FPGA1_IO_CTIN_L,
    CheckON_FPGA1_IO_CTIN_L,
    TurnOFF_FPGA1_IO_CTIN_L,
    CheckOFF_FPGA1_IO_CTIN_L,
    
    TurnON_FPGA1_IO_ICI_L,
    CheckON_FPGA1_IO_ICI_L,
    TurnOFF_FPGA1_IO_ICI_L,
    CheckOFF_FPGA1_IO_ICI_L,
    
    TurnON_FPGA1_IO_NTS_FLT,
    CheckON_FPGA1_IO_NTS_FLT,
    TurnOFF_FPGA1_IO_NTS_FLT,
    CheckOFF_FPGA1_IO_NTS_FLT,
    
    TurnON_FPGA1_IO_PAL_FLT,
    CheckON_FPGA1_IO_PAL_FLT,
    TurnOFF_FPGA1_IO_PAL_FLT,
    CheckOFF_FPGA1_IO_PAL_FLT,
   
    TurnON_FPGA1_IO_TEST_MODE,
    CheckON_FPGA1_IO_TEST_MODE,
    TurnOFF_FPGA1_IO_TEST_MODE,
    CheckOFF_FPGA1_IO_TEST_MODE,
    
    TurnON_FPGA1_IO_REAR_DOOR,
    CheckON_FPGA1_IO_REAR_DOOR,
    TurnOFF_FPGA1_IO_REAR_DOOR,
    CheckOFF_FPGA1_IO_REAR_DOOR,
    
//-------------------------------------          
    TurnON_FPGA1_IO_SEQUENCE_CHECK,
    CheckON_FPGA1_IO_SEQUENCE_CHECK,
    TurnOFF_FPGA1_IO_SEQUENCE_CHECK,
    CheckOFF_FPGA1_IO_SEQUENCE_CHECK,
    
    
	FPGA1_IO_TEST_Pass,
	FPGA1_IO_TEST_Done,
	FPGA1_IO_TEST_Fail,
	Retest_option				// Required for Retest Option  
};

 
   
	
	
#define fpga1_io_test_menu_index		100
static char FPGA1_IO_Test_Menu[fpga1_io_test_menu_index][20]= 
{
  "FPGA1 IO TEST       ",       //0
  "Checking FPGA1_IO...",		//1
  "FPGA1_IO_ACC_L ON   ",       //2 
  "FPGA1_IO_ACC_L OFF  ",       //3
  "FPGA1_IO_GS_L ON    ",       //4 
  "FPGA1_IO_GS_L OFF   ",       //5
  "FPGA1_IO_RGS_L ON   ",       //6 
  "FPGA1_IO_RGS_L OFF  ",       //7
  "FPGA1_IO_LPS_L ON   ",       //8 
  "FPGA1_IO_LPS_L OFF  ",       //9
  "FPGA1_IO_RLM_L ON   ",       //10 
  "FPGA1_IO_RLM_L OFF  ",       //11
  "FPGA1_IO_DLT_L ON   ",       //12 
  "FPGA1_IO_DLT_L OFF  ",       //13
  "FPGA1_IO_DLM_L ON   ",       //14 
  "FPGA1_IO_DLM_L OFF  ",       //15
  "FPGA1_IO_DLB_L ON   ",       //16
  "FPGA1_IO_DLB_L OFF  ",       //17
  "FPGA1_IO_TAD_L ON   ",       //18
  "FPGA1_IO_TAD_L OFF  ",       //19
  "FPGA1_IO_TAU_L ON   ",       //20
  "FPGA1_IO_TAU_L OFF  ",       //21
  "FPGA1_IO_BAD_L ON   ",       //22
  "FPGA1_IO_BAD_L OFF  ",       //23 
  "FPGA1_IO_BAU_L ON   ",       //24
  "FPGA1_IO_BAU_L OFF  ",       //25 
  "FPGA1_IO_ID_L ON    ",       //26
  "FPGA1_IO_ID_L OFF   ",       //27
  "FPGA1_IO_IU_L ON    ",       //28
  "FPGA1_IO_IU_L OFF   ",       //29
  "FPGA1_IO_CTIN_L ON  ",       //30
  "FPGA1_IO_CTIN_L OFF ",       //31
  "FPGA1_IO_ICI_L ON   ",       //32
  "FPGA1_IO_ICI_L OFF  ",       //33
  "FPGA1_IO_NTS_FLT ON ",       //34
  "FPGA1_IO_NTS_FLT OFF",       //35
  "FPGA1_IO_PAL_FLT ON ",       //36
  "FPGA1_IO_PAL_FLT OFF",       //37
  "FPGA1_IO_TST_MODE ON",       //38
  "FPGA1_IO_TST_MODE OF",       //39
  "FPGA1_IO_RER_MODE ON",       //40
  "FPGA1_IO_RER_MODE OF",       //41
  
//-------------------------------------  
  "FPGA1_IO_UL_CPU ON  ",       //42
  "FPGA1_IO_UL_CPU OFF ",       //43
  "FPGA1_IO_DNF_CPU ON ",       //44
  "FPGA1_IO_DNF_CPU OFF",       //45
  "FPGA1_IO_UPF_CPU ON ",       //46
  "FPGA1_IO_UPF_CPU OFF",       //47
  "FPGA1_IO_UPR_CPU ON ",       //48
  "FPGA1_IO_UPR_CPU OFF",       //49
  "FPGA1_IO_DNR_CPU ON ",       //50
  "FPGA1_IO_DNR_CPU OFF",       //51
  "FPGA1_IO_STE_CPU ON ",       //52
  "FPGA1_IO_STE_CPU OFF",       //53
  "FPGA1_IO_MC_CPU ON  ",       //54
  "FPGA1_IO_MC_CPU OFF ",       //55
  "FPGA1_IO_CEN_CPU ON ",       //56
  "FPGA1_IO_CEN_CPU OFF",       //57  
  "FPGA1_IO_RUN_CPU ON ",       //58
  "FPGA1_IO_RUN_CPU OFF",       //59  
  
  "F1_IO_SEL_OK_CPU ON ",       //60
  "F1_IO_SEL_OK_CPU OFF",       //61  
  "F1_IO_SPD_75_CPU ON ",       //62
  "F1_IO_SPD_75_CPU OFF",       //63 
  "F1_IO_SPD_150_CPU ON",       //64
  "F1_IO_SPD_150_CPU OF",       //65 
  
  "FPGA1_IO_DL_CPU ON  ",       //66
  "FPGA1_IO_DL_CPU OFF ",       //67 
  "FPGA1_IO_DZ_CPU ON  ",       //68
  "FPGA1_IO_DZ_CPU OFF ",       //69 
  
//-------------------------------------  
  "F1_IO_SPD_75_NTS ON ",       //70
  "F1_IO_SPD_75_NTS OFF",       //71 
  "F1_IO_SPD_150_NTS ON",       //72
  "F1_IO_SPD_150_NTS OF",       //73 
  "F1_IO_SEL_OK_NTS ON ",       //74
  "F1_IO_SEL_OK_NTS OFF",       //75  
  
  "FPGA1_IO_DZ_NTS ON  ",       //76
  "FPGA1_IO_DZ_NTS OFF ",       //77 
  "FPGA1_IO_DN_NTS ON  ",       //78
  "FPGA1_IO_DN_NTS OFF ",       //79 
  "FPGA1_IO_UN_NTS ON  ",       //80
  "FPGA1_IO_UN_NTS OFF ",       //81 
  "FPGA1_IO_UT_NTS ON  ",       //82
  "FPGA1_IO_UT_NTS OFF ",       //83 
  "FPGA1_IO_DT_NTS ON  ",       //84
  "FPGA1_IO_DT_NTS OFF ",       //85 
};


 
 
 




//------------------------------------------------------------------------------   
int16 FPGA1_IO_TEST(int16 Operation)
{
	static int16 FPGA1_IO_TEST_CASE=FPGA1_IO_Test_Init;
 	int16 Delay_Count=test_halfsec_delay;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    static int16 f1_seqIO_i=0;
    
    
    switch (FPGA1_IO_TEST_CASE) 
    {
    	case FPGA1_IO_Test_Init:
    		f1_seqIO_i=0;
       		CurrentTestPassFail=0;  // fail
       		clr_lcd_dply();
        	ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        	Retest_Option_Parm = 0; // Required for Retest Option
        	RetestResponse = 1; // Required for Retest Option        
 	            	
       	 	New_write_display(FPGA1_IO_Test_Menu[0],0,0);       	 
                	                	
         	FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_ACC_L;      
        	
        	Include_ALL_Short_Signal();        	
        	Reset_Fault_Type_Index();          
        	Turn_ALL_Output_OFF();        	
        	Exclude_PowerAndReset_InputPin();
        	Clear_Log_Buffer(); 			
			timers[t_Generic_Delay]=0;
        break;
//------------------------------------------------------------------------------ 
//ACC_L
//ACC_iF1B0_27
//ACC_iF2B1_107
    	case TurnON_FPGA1_IO_ACC_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[2],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(ACC_iF1B0_27);
     		Exclude_Short_Signal(ACC_iF2B1_107);
    		    		
     		Turn_Signal_ON(o_10ACC_28);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_ACC_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_ACC_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(ACC_iF1B0_27)!=0)||(Check_Signal_ON(ACC_iF2B1_107)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_ACC_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_ACC_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[3],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_10ACC_28);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_ACC_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_ACC_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(ACC_iF1B0_27)!=0)||(Check_Signal_OFF(ACC_iF2B1_107)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_GS_L;
			}
			    
			Include_Short_Signal(ACC_iF1B0_27);
			Include_Short_Signal(ACC_iF2B1_107);
					
    		timers[t_Generic_Delay]=0; 
    	break;    	        
//------------------------------------------------------------------------------ 
//GS_L
//GS_iF1B0_28
//GS1_iF2B2_134, GS_iF2B2_135
    	case TurnON_FPGA1_IO_GS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[4],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(GS_iF1B0_28);
     		
     		Exclude_Short_Signal(GS1_iF2B2_134);
    		Exclude_Short_Signal(GS_iF2B2_135);    
    				
     		Turn_Signal_ON(o_T9GS_29);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_GS_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_GS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(GS_iF1B0_28)!=0)
    			||(Check_Signal_ON(GS1_iF2B2_134)!=0)||(Check_Signal_ON(GS_iF2B2_135)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_GS_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_GS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[5],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_T9GS_29);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_GS_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_GS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(GS_iF1B0_28)!=0)
    			||(Check_Signal_OFF(GS1_iF2B2_134)!=0)||(Check_Signal_OFF(GS_iF2B2_135)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_RGS_L;
			}
			    
			Include_Short_Signal(GS_iF1B0_28);
     		
     		Include_Short_Signal(GS1_iF2B2_134);
    		Include_Short_Signal(GS_iF2B2_135);    
					
    		timers[t_Generic_Delay]=0; 
    	break; 
//------------------------------------------------------------------------------ 
//RGS_L
//RGS_iF1B0_29
//RGS1_iF2B2_132, RGS_iF2B2_133
    	case TurnON_FPGA1_IO_RGS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[6],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(RGS_iF1B0_29);
     		
     		Exclude_Short_Signal(RGS1_iF2B2_132);
    		Exclude_Short_Signal(RGS_iF2B2_133);    
    				
     		Turn_Signal_ON(o_RGS_30);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_RGS_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_RGS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(RGS_iF1B0_29)!=0)
    			||(Check_Signal_ON(RGS1_iF2B2_132)!=0)||(Check_Signal_ON(RGS_iF2B2_133)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_RGS_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_RGS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[7],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_RGS_30);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_RGS_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_RGS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(RGS_iF1B0_29)!=0)
    			||(Check_Signal_OFF(RGS1_iF2B2_132)!=0)||(Check_Signal_OFF(RGS_iF2B2_133)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_LPS_L;
			}
			    
			Include_Short_Signal(RGS_iF1B0_29);
     		
     		Include_Short_Signal(RGS1_iF2B2_132);
    		Include_Short_Signal(RGS_iF2B2_133);    
					
    		timers[t_Generic_Delay]=0; 
    	break;    
//------------------------------------------------------------------------------ 
//LPS_L
//LPS_iF1B0_30
//LPS_iF2B0_100 
    	case TurnON_FPGA1_IO_LPS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[8],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(LPS_iF1B0_30);
     		
     		Exclude_Short_Signal(LPS_iF2B0_100);
    				
     		Turn_Signal_ON(o_S5LPS_0);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_LPS_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_LPS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(LPS_iF1B0_30)!=0)
    			||(Check_Signal_ON(LPS_iF2B0_100)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_LPS_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_LPS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[9],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_S5LPS_0);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_LPS_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_LPS_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(LPS_iF1B0_30)!=0)
    			||(Check_Signal_OFF(LPS_iF2B0_100)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_RLM_L;
			}
			    
			Include_Short_Signal(LPS_iF1B0_30);
     		
     		Include_Short_Signal(LPS_iF2B0_100); 
					
    		timers[t_Generic_Delay]=0; 
    	break;   
//------------------------------------------------------------------------------ 
//RLM_L
//RLM_iF1B0_31
//RLM1_iF2B1_110, RLM_iF2B1_111 
    	case TurnON_FPGA1_IO_RLM_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[10],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(RLM_iF1B0_31);
     		
     		Exclude_Short_Signal(RLM1_iF2B1_110);
    		Exclude_Short_Signal(RLM_iF2B1_111);
    				
     		Turn_Signal_ON(o_RLM_16);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_RLM_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_RLM_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(RLM_iF1B0_31)!=0)
    			||(Check_Signal_ON(RLM1_iF2B1_110)!=0)||(Check_Signal_ON(RLM_iF2B1_111)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_RLM_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_RLM_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[11],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_RLM_16);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_RLM_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_RLM_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(RLM_iF1B0_31)!=0)
    			||(Check_Signal_OFF(RLM1_iF2B1_110)!=0)||(Check_Signal_OFF(RLM_iF2B1_111)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_DLT_L;
			}
			    
			Include_Short_Signal(RLM_iF1B0_31);
     		
     		Include_Short_Signal(RLM1_iF2B1_110);
    		Include_Short_Signal(RLM_iF2B1_111); 
					
    		timers[t_Generic_Delay]=0; 
    	break;    
//------------------------------------------------------------------------------ 
//DLT_L
//DLT_iF1B0_32
//DLT1_iF2B1_112, DLT_iF2B1_113 
    	case TurnON_FPGA1_IO_DLT_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[12],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(DLT_iF1B0_32);
     		
     		Exclude_Short_Signal(DLT1_iF2B1_112);
    		Exclude_Short_Signal(DLT_iF2B1_113);
    				
     		Turn_Signal_ON(o_H3DLT_15);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_DLT_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_DLT_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(DLT_iF1B0_32)!=0)
    			||(Check_Signal_ON(DLT1_iF2B1_112)!=0)||(Check_Signal_ON(DLT_iF2B1_113)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_DLT_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_DLT_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[13],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_H3DLT_15);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_DLT_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_DLT_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(DLT_iF1B0_32)!=0)
    			||(Check_Signal_OFF(DLT1_iF2B1_112)!=0)||(Check_Signal_OFF(DLT_iF2B1_113)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_DLM_L;
			}
			    
			Include_Short_Signal(DLT_iF1B0_32);
     		
     		Include_Short_Signal(DLT1_iF2B1_112);
    		Include_Short_Signal(DLT_iF2B1_113);
					
    		timers[t_Generic_Delay]=0; 
    	break;   
//------------------------------------------------------------------------------ 
//DLM_L
//DLM_iF1B0_33
//DLM1_iF2B1_114, DLM_iF2B1_115 
    	case TurnON_FPGA1_IO_DLM_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[14],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(DLM_iF1B0_33);
     		
     		Exclude_Short_Signal(DLM1_iF2B1_114);
    		Exclude_Short_Signal(DLM_iF2B1_115);
    				
     		Turn_Signal_ON(o_H5DLM_14);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_DLM_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_DLM_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(DLM_iF1B0_33)!=0)
    			||(Check_Signal_ON(DLM1_iF2B1_114)!=0)||(Check_Signal_ON(DLM_iF2B1_115)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_DLM_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_DLM_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[15],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_H5DLM_14);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_DLM_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_DLM_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(DLM_iF1B0_33)!=0)
    			||(Check_Signal_OFF(DLM1_iF2B1_114)!=0)||(Check_Signal_OFF(DLM_iF2B1_115)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_DLB_L;
			}
			    
			Include_Short_Signal(DLM_iF1B0_33);
     		
     		Include_Short_Signal(DLM1_iF2B1_114);
    		Include_Short_Signal(DLM_iF2B1_115);
					
    		timers[t_Generic_Delay]=0; 
    	break;  
//------------------------------------------------------------------------------ 
//DLB_L
//DLB_iF1B0_34
//DLB1_iF2B1_116, DLB_iF2B1_117 
    	case TurnON_FPGA1_IO_DLB_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[16],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(DLB_iF1B0_34);
     		
     		Exclude_Short_Signal(DLB1_iF2B1_116);
    		Exclude_Short_Signal(DLB_iF2B1_117);
    				
     		Turn_Signal_ON(o_H7DLB_13);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_DLB_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_DLB_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(DLB_iF1B0_34)!=0)
    			||(Check_Signal_ON(DLB1_iF2B1_116)!=0)||(Check_Signal_ON(DLB_iF2B1_117)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_DLB_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_DLB_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[17],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_H7DLB_13);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_DLB_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_DLB_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(DLB_iF1B0_34)!=0)
    			||(Check_Signal_OFF(DLB1_iF2B1_116)!=0)||(Check_Signal_OFF(DLB_iF2B1_117)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_TAD_L;
			}
			    
			Include_Short_Signal(DLB_iF1B0_34);
     		
     		Include_Short_Signal(DLB1_iF2B1_116);
    		Include_Short_Signal(DLB_iF2B1_117);
					
    		timers[t_Generic_Delay]=0; 
    	break; 
//------------------------------------------------------------------------------ 
//TAD_L
//TAD_iF1B0_35
//TAD_iF2B1_118 
    	case TurnON_FPGA1_IO_TAD_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[18],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(TAD_iF1B0_35);
     		
     		Exclude_Short_Signal(TAD_iF2B1_118);  
    				
     		Turn_Signal_ON(o_H9TAD_12);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_TAD_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_TAD_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(TAD_iF1B0_35)!=0)
    			||(Check_Signal_ON(TAD_iF2B1_118)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_TAD_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_TAD_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[19],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_H9TAD_12);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_TAD_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_TAD_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(TAD_iF1B0_35)!=0)
    			||(Check_Signal_OFF(TAD_iF2B1_118)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_TAU_L;
			}
			    
			Include_Short_Signal(TAD_iF1B0_35);
     		
     		Include_Short_Signal(TAD_iF2B1_118);  
					
    		timers[t_Generic_Delay]=0; 
    	break;    	   	         
//------------------------------------------------------------------------------ 
//TAU_L
//TAU_iF1B0_36
//TAU_iF2B1_119 
    	case TurnON_FPGA1_IO_TAU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[20],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(TAU_iF1B0_36);
     		
     		Exclude_Short_Signal(TAU_iF2B1_119);  
    				
     		Turn_Signal_ON(o_10TAU_11);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_TAU_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_TAU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(TAU_iF1B0_36)!=0)
    			||(Check_Signal_ON(TAU_iF2B1_119)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_TAU_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_TAU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[21],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_10TAU_11);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_TAU_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_TAU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(TAU_iF1B0_36)!=0)
    			||(Check_Signal_OFF(TAU_iF2B1_119)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_BAD_L;
			}
			    
			Include_Short_Signal(TAU_iF1B0_36);
     		
     		Include_Short_Signal(TAU_iF2B1_119);  
					
    		timers[t_Generic_Delay]=0; 
    	break;    	
//------------------------------------------------------------------------------ 
//BAD_L
//BAD_iF1B0_37
//BAD_iF2B1_120 
    	case TurnON_FPGA1_IO_BAD_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[22],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(BAD_iF1B0_37);
     		
     		Exclude_Short_Signal(BAD_iF2B1_120);  
    				
     		Turn_Signal_ON(o_12BAD_10);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_BAD_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_BAD_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(BAD_iF1B0_37)!=0)
    			||(Check_Signal_ON(BAD_iF2B1_120)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_BAD_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_BAD_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[23],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_12BAD_10);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_BAD_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_BAD_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(BAD_iF1B0_37)!=0)
    			||(Check_Signal_OFF(BAD_iF2B1_120)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_BAU_L;
			}
			    
			Include_Short_Signal(BAD_iF1B0_37);
     		
     		Include_Short_Signal(BAD_iF2B1_120);  
					
    		timers[t_Generic_Delay]=0; 
    	break;    
//------------------------------------------------------------------------------ 
//BAU_L
//BAU_iF1B0_38
//BAU_iF2B1_121 
    	case TurnON_FPGA1_IO_BAU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[24],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(BAU_iF1B0_38);
     		
     		Exclude_Short_Signal(BAU_iF2B1_121);  
    				
     		Turn_Signal_ON(o_13BAU_9);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_BAU_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_BAU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(BAU_iF1B0_38)!=0)
    			||(Check_Signal_ON(BAU_iF2B1_121)!=0) 
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_BAU_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_BAU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[25],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_13BAU_9);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_BAU_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_BAU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(BAU_iF1B0_38)!=0)
    			||(Check_Signal_OFF(BAU_iF2B1_121)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_ID_L;
			}
			    
			Include_Short_Signal(BAU_iF1B0_38);
     		
     		Include_Short_Signal(BAU_iF2B1_121);  
					
    		timers[t_Generic_Delay]=0; 
    	break;        	
//------------------------------------------------------------------------------ 
//ID_L
//ID_iF1B1_55 
//ID_iF2B2_124
    	case TurnON_FPGA1_IO_ID_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[26],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(ID_iF1B1_55); 
     		
     		Exclude_Short_Signal(ID_iF2B2_124);    		
    				
     		Turn_Signal_ON(o_T14ID_24);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_ID_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_ID_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(ID_iF1B1_55)!=0)
    			||(Check_Signal_ON(ID_iF2B2_124)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_ID_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_ID_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[27],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_T14ID_24);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_ID_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_ID_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(ID_iF1B1_55)!=0)
    			||(Check_Signal_OFF(ID_iF2B2_124)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_IU_L;
			}
			    
			Include_Short_Signal(ID_iF1B1_55); 
     		
     		Include_Short_Signal(ID_iF2B2_124);     		
					
    		timers[t_Generic_Delay]=0; 
    	break;    
//------------------------------------------------------------------------------ 
//IU_L
//IU_iF1B1_56 
//IU_iF2B2_123
    	case TurnON_FPGA1_IO_IU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[28],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(IU_iF1B1_56); 
     		
     		Exclude_Short_Signal(IU_iF2B2_123);    		
    				
     		Turn_Signal_ON(o_T15IU_23);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_IU_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_IU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(IU_iF1B1_56)!=0)
    			||(Check_Signal_ON(IU_iF2B2_123)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_IU_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_IU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[29],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_T15IU_23);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_IU_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_IU_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(IU_iF1B1_56)!=0)
    			||(Check_Signal_OFF(IU_iF2B2_123)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_CTIN_L;
			}
			    
			Include_Short_Signal(IU_iF1B1_56); 
     		
     		Include_Short_Signal(IU_iF2B2_123); 		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	   	      
//------------------------------------------------------------------------------ 
//CTIN_L
//CTIN_iF1B1_57 
//CTIN_iF2B3_154
    	case TurnON_FPGA1_IO_CTIN_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[30],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(CTIN_iF1B1_57); 
     		
     		Exclude_Short_Signal(CTIN_iF2B3_154);    		
    				
     		Turn_Signal_ON(o_16INS_22);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_CTIN_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_CTIN_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(CTIN_iF1B1_57)!=0)
    			||(Check_Signal_ON(CTIN_iF2B3_154)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_CTIN_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_CTIN_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[31],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_16INS_22);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_CTIN_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_CTIN_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(CTIN_iF1B1_57)!=0)
    			||(Check_Signal_OFF(CTIN_iF2B3_154)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_ICI_L;
			}
			    
			Include_Short_Signal(CTIN_iF1B1_57); 
     		
     		Include_Short_Signal(CTIN_iF2B3_154);    		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	   	      	          
//------------------------------------------------------------------------------ 
//ICI_L
//ICI_iF1B1_58 
//ICI_iF2B2_131
    	case TurnON_FPGA1_IO_ICI_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[32],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(ICI_iF1B1_58); 
     		
     		Exclude_Short_Signal(ICI_iF2B2_131);    		
    				
     		Turn_Signal_ON(o_11ICI_27);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_ICI_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_ICI_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(ICI_iF1B1_58)!=0)
    			||(Check_Signal_ON(ICI_iF2B2_131)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_ICI_L;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_ICI_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[33],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_11ICI_27);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_ICI_L;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_ICI_L:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(ICI_iF1B1_58)!=0)
    			||(Check_Signal_OFF(ICI_iF2B2_131)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_NTS_FLT;
			}
			    
			Include_Short_Signal(ICI_iF1B1_58); 
     		
     		Include_Short_Signal(ICI_iF2B2_131);      		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	      	
//------------------------------------------------------------------------------ 
//NTS_FLT
//NTS_FLT_iF1B3_78 
//NTS_FLT_iF2B2_136
    	case TurnON_FPGA1_IO_NTS_FLT:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[34],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(NTS_FLT_iF1B3_78); 
     		
     		Exclude_Short_Signal(NTS_FLT_iF2B2_136);    		
    				
     		Turn_Signal_ON(NTS_FLT_CANout_75);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_NTS_FLT;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_NTS_FLT:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(NTS_FLT_iF1B3_78)!=0)
    			||(Check_Signal_ON(NTS_FLT_iF2B2_136)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_NTS_FLT;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_NTS_FLT:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[35],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(NTS_FLT_CANout_75);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_NTS_FLT;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_NTS_FLT:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(NTS_FLT_iF1B3_78)!=0)
    			||(Check_Signal_OFF(NTS_FLT_iF2B2_136)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_PAL_FLT;
			}
			    
			Include_Short_Signal(NTS_FLT_iF1B3_78); 
     		
     		Include_Short_Signal(NTS_FLT_iF2B2_136);      		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	      
//------------------------------------------------------------------------------ 
//PAL_FLT
//PAL_FLT_iF1B3_87 
//PAL_FLT_iF2B2_137
    	case TurnON_FPGA1_IO_PAL_FLT:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[36],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(PAL_FLT_iF1B3_87); 
     		
     		Exclude_Short_Signal(PAL_FLT_iF2B2_137);    		
    				
     		Turn_Signal_ON(PAL_FAULT_CANout_77);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_PAL_FLT;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_PAL_FLT:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(PAL_FLT_iF1B3_87)!=0)
    			||(Check_Signal_ON(PAL_FLT_iF2B2_137)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_PAL_FLT;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_PAL_FLT:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[37],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(PAL_FAULT_CANout_77);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_PAL_FLT;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_PAL_FLT:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(PAL_FLT_iF1B3_87)!=0)
    			||(Check_Signal_OFF(PAL_FLT_iF2B2_137)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_TEST_MODE;
			}
			    
			Include_Short_Signal(PAL_FLT_iF1B3_87); 
     		
     		Include_Short_Signal(PAL_FLT_iF2B2_137);      		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	     
//------------------------------------------------------------------------------ 
//TEST_MODE
//TEST_MODE_iF1B3_76 
//TEST_MODE_iF2B1_109
//TEST_MODE_iNTSB0_155
    	case TurnON_FPGA1_IO_TEST_MODE:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[38],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(TEST_MODE_iF1B3_76); 
     		
     		Exclude_Short_Signal(TEST_MODE_iF2B1_109);    
     		
     		Exclude_Short_Signal(TEST_MODE_iNTSB0_155);  		
    				
     		Turn_Signal_ON(o_TMRLY_20);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_TEST_MODE;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_TEST_MODE:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(TEST_MODE_iF1B3_76)!=0)
    			||(Check_Signal_ON(TEST_MODE_iF2B1_109)!=0)
    			||(Check_Signal_ON(TEST_MODE_iNTSB0_155)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_TEST_MODE;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_TEST_MODE:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[39],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_TMRLY_20);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_TEST_MODE;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_TEST_MODE:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(TEST_MODE_iF1B3_76)!=0)
    			||(Check_Signal_OFF(TEST_MODE_iF2B1_109)!=0)
    			||(Check_Signal_OFF(TEST_MODE_iNTSB0_155)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_REAR_DOOR;
			}
			    
			Include_Short_Signal(TEST_MODE_iF1B3_76); 
     		
     		Include_Short_Signal(TEST_MODE_iF2B1_109);    
     		
     		Include_Short_Signal(TEST_MODE_iNTSB0_155); 		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	           	  		 
//------------------------------------------------------------------------------ 
//REAR_MODE
//REAR_DOOR_iF1B3_77 
//REAR_DOOR_iF2B1_108
    	case TurnON_FPGA1_IO_REAR_DOOR:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[40],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(REAR_DOOR_iF1B3_77); 
     		
     		Exclude_Short_Signal(REAR_DOOR_iF2B1_108);    
     		
    				
     		Turn_Signal_ON(o_RDRLY_31);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_REAR_DOOR;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_REAR_DOOR:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(REAR_DOOR_iF1B3_77)!=0)
    			||(Check_Signal_ON(REAR_DOOR_iF2B1_108)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_REAR_DOOR;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_REAR_DOOR:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[41],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_RDRLY_31);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_REAR_DOOR;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_REAR_DOOR:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(REAR_DOOR_iF1B3_77)!=0)
    			||(Check_Signal_OFF(REAR_DOOR_iF2B1_108)!=0)) 
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_SEQUENCE_CHECK;
			}
			    
			Include_Short_Signal(REAR_DOOR_iF1B3_77); 
     		
     		Include_Short_Signal(REAR_DOOR_iF2B1_108);         		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	           	  
//------------------------------------------------------------------------------ 
//SEQUENCE_CHECK
    	case TurnON_FPGA1_IO_SEQUENCE_CHECK:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA1_IO_Test_Menu[42+f1_seqIO_i*2],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(FPGA1_IO_TEST_TEMP_ARRAY[f1_seqIO_i][FPGA1_IO_TEST_TEMP_Input_Index]); 
     		     	
     		Turn_Signal_ON(FPGA1_IO_TEST_TEMP_ARRAY[f1_seqIO_i][FPGA1_IO_TEST_TEMP_Output_Index]);
    		   
    		FPGA1_IO_TEST_CASE=CheckON_FPGA1_IO_SEQUENCE_CHECK;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA1_IO_SEQUENCE_CHECK:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(FPGA1_IO_TEST_TEMP_ARRAY[f1_seqIO_i][FPGA1_IO_TEST_TEMP_Input_Index])!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				FPGA1_IO_TEST_CASE=TurnOFF_FPGA1_IO_SEQUENCE_CHECK;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA1_IO_SEQUENCE_CHECK:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA1_IO_Test_Menu[43+f1_seqIO_i*2],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(FPGA1_IO_TEST_TEMP_ARRAY[f1_seqIO_i][FPGA1_IO_TEST_TEMP_Output_Index]);
    		
    		FPGA1_IO_TEST_CASE=CheckOFF_FPGA1_IO_SEQUENCE_CHECK;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA1_IO_SEQUENCE_CHECK:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA1_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if(Check_Signal_OFF(FPGA1_IO_TEST_TEMP_ARRAY[f1_seqIO_i][FPGA1_IO_TEST_TEMP_Input_Index])!=0)
    		{    			    		
 				FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Fail;
			}
			else
			{
				Include_Short_Signal(FPGA1_IO_TEST_TEMP_ARRAY[f1_seqIO_i][FPGA1_IO_TEST_TEMP_Input_Index]);
				
				f1_seqIO_i++;
				
				if(f1_seqIO_i>=FPGA1_IO_TEST_TEMP_IO_num)
				{
					FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Pass;
					f1_seqIO_i=0;
				}
				else
				{
					FPGA1_IO_TEST_CASE=TurnON_FPGA1_IO_SEQUENCE_CHECK;
				}				
			}			    			      		     		      		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	               			     	   	 		   	                 	 	                 	           	    	   	             	    	     	   	   	             	    	  	   	             		   	     	 	   	             		   	                    	 	   	                		   	            	
//------------------------------------------------------------------------------          
        case FPGA1_IO_TEST_Pass:
       		 CurrentTestPassFail = 1; // pass
        	 clr_lcd_dply();
        	 New_write_display("FPGA1 IO TEST       ",0,0);
        	 New_write_display("TEST PASSED         ",1,0);
        	 New_write_display("GOOD JOB            ",2,0);     
        	 FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Done;        
        break;
        
    	case FPGA1_IO_TEST_Done:
			 FPGA1_IO_TEST_CASE=FPGA1_IO_Test_Init; 
        
          	 // Required for Retest Option
        	 if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        	 else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     	case Retest_option: // Required for Retest Option
        	 RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        	 Retest_Option_Parm = 1;
        	 if(RetestResponse == 0) FPGA1_IO_TEST_CASE=FPGA1_IO_TEST_Done;
        	 else if(RetestResponse == 1) FPGA1_IO_TEST_CASE=FPGA1_IO_Test_Init; 
        	 else 
        	 	ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    	case FPGA1_IO_TEST_Fail:
//-    		clr_lcd_dply();
    		Disply_Fault();
        	FPGA1_IO_TEST_CASE = Retest_option;
        break;

    	default:
        	FPGA1_IO_TEST_CASE=FPGA1_IO_Test_Init;
        	ReturnVal = DEFAULT;
        break;
    }	
    
    return ReturnVal; // Required for Retest Option	            
}
























