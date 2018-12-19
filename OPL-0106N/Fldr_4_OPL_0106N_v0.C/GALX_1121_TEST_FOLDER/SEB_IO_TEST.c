#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"
#include "v120AC_Power_Check.h"


int16 SEB_IO_TEST(int16 Operation);







enum
{
	SEB_IO_Test_Init=0,
	TurnON_SEB_IO_SUF_S14,
    CheckON_SEB_IO_SUF_S14,
    TurnOFF_SEB_IO_SUF_S14,
    CheckOFF_SEB_IO_SUF_S14,
    
    TurnON_SEB_IO_SU_S12,
    CheckON_SEB_IO_SU_S12,
    TurnOFF_SEB_IO_SU_S12,
    CheckOFF_SEB_IO_SU_S12,
    
    TurnON_SEB_IO_SDF_S10,
    CheckON_SEB_IO_SDF_S10,
    TurnOFF_SEB_IO_SDF_S10,
    CheckOFF_SEB_IO_SDF_S10,
  
    TurnON_SEB_IO_SD_S8,
    CheckON_SEB_IO_SD_S8,
    TurnOFF_SEB_IO_SD_S8,
    CheckOFF_SEB_IO_SD_S8,
            
    TurnON_SEB_IO_GOV_H22,
    CheckON_SEB_IO_GOV_H22,
    TurnOFF_SEB_IO_GOV_H22,
    CheckOFF_SEB_IO_GOV_H22,
    
    TurnON_SEB_IO_TLH_H20,
    CheckON_SEB_IO_TLH_H20,
    TurnOFF_SEB_IO_TLH_H20,
    CheckOFF_SEB_IO_TLH_H20,
    
    TurnON_SEB_IO_PS_H18,
    CheckON_SEB_IO_PS_H18,
    TurnOFF_SEB_IO_PS_H18,
    CheckOFF_SEB_IO_PS_H18,
    
    TurnON_SEB_IO_MC,
    CheckON_SEB_IO_MC,
    TurnOFF_SEB_IO_MC,
    CheckOFF_SEB_IO_MC,
    
    TurnON_SEB_IO_MC1,
    CheckON_SEB_IO_MC1,
    TurnOFF_SEB_IO_MC1,
    CheckOFF_SEB_IO_MC1,
    
    TurnON_SEB_IO_STE,
    CheckON_SEB_IO_STE,
    TurnOFF_SEB_IO_STE,
    CheckOFF_SEB_IO_STE,
    
    TurnON_SEB_IO_HSS_T18,
    CheckON_SEB_IO_HSS_T18,
    TurnOFF_SEB_IO_HSS_T18,
    CheckOFF_SEB_IO_HSS_T18,
    
    
    
//-    TurnON_AUTO_Signal,
//-    CheckON_AUTO_Signal,
	SEB_IO_TEST_Pass,
	SEB_IO_TEST_Done,
	SEB_IO_TEST_Fail,
	Retest_option				// Required for Retest Option    
};	




#define seb_io_test_menu_index		30
static char SEB_IO_Test_Menu[seb_io_test_menu_index][21]= 
{
  "SEB IO TEST         ",       //0   
  "Checking SEB_IO...  ",		//1
  "SEB_IO_SUF_S14 ON   ",       //2  
  "SEB_IO_SUF_S14 OFF  ",       //3
  "SEB_IO_SU_S12 ON    ",       //4  
  "SEB_IO_SU_S12 OFF   ",       //5
  "SEB_IO_SDF_S10 ON   ",       //6  
  "SEB_IO_SDF_S10 OFF  ",       //7
  "SEB_IO_SD_S8 ON     ",       //8   
  "SEB_IO_SD_S8 OFF    ",       //9
  "SEB_IO_GOV_H22 ON   ",       //10   
  "SEB_IO_GOV_H22 OFF  ",       //11  
  "SEB_IO_TLH_H20 ON   ",       //12   
  "SEB_IO_TLH_H20 OFF  ",       //13  
  "SEB_IO_PS_H18 ON    ",       //14   
  "SEB_IO_PS_H18 OFF   ",       //15  
  "SEB_IO_MC ON        ",       //16   
  "SEB_IO_MC OFF       ",       //17 
  "SEB_IO_MC1 ON       ",       //18   
  "SEB_IO_MC1 OFF      ",       //19 
  "SEB_IO_STE ON       ",       //20  
  "SEB_IO_STE OFF      ",       //21 
  "SEB_IO_HSS_T18 ON   ",       //22  
  "SEB_IO_HSS_T18 OFF  ",       //23 
  };







//------------------------------------------------------------------------------   
int16 SEB_IO_TEST(int16 Operation)
{
	static int16 SEB_IO_TEST_CASE=SEB_IO_Test_Init;
 	int16 Delay_Count=test_halfsec_delay;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    switch (SEB_IO_TEST_CASE) 
    {
    	case SEB_IO_Test_Init:
       		CurrentTestPassFail=0;  // fail
       		clr_lcd_dply();
        	ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        	Retest_Option_Parm = 0; // Required for Retest Option
        	RetestResponse = 1; // Required for Retest Option        
 	            	
       	 	New_write_display(SEB_IO_Test_Menu[0],0,0);       	 
                	                	
        	SEB_IO_TEST_CASE=TurnON_SEB_IO_SUF_S14;      
        	
        	Include_ALL_Short_Signal();        	
        	Reset_Fault_Type_Index();          
        	Turn_ALL_Output_OFF();        	
        	Exclude_PowerAndReset_InputPin();
        	Clear_Log_Buffer(); 			
			timers[t_Generic_Delay]=0;
        break;
//------------------------------------------------------------------------------ 
//SUF_S14
//i_suf_0, i_s2mrw_9, i_16mrw_14, i_14mrw_15, i_11mrw_16, i_mrsw_21
//UPF_CPU_iF1B1_45, RUN_CPU_iF1B2_68, UT_NTS_iF1B3_85, 
//TSD_iF2B0_93, UT_iF2B0_103, RUN_iF2B2_129, SUF_iF2B3_141, MRSW_iF2B3_142, CST_iF2B3_153                 
    	case TurnON_SEB_IO_SUF_S14:   
    		//100ms=1/10s once
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[2],1,0);
    		   		    		
    		Exclude_Short_Signal(i_suf_0);    		
    		Exclude_Short_Signal(i_s2mrw_9);
    		Exclude_Short_Signal(i_16mrw_14);
    		Exclude_Short_Signal(i_14mrw_15);
    		Exclude_Short_Signal(i_11mrw_16);
    		Exclude_Short_Signal(i_mrsw_21);
    		
    		Exclude_Short_Signal(UPF_CPU_iF1B1_45);
    		Exclude_Short_Signal(RUN_CPU_iF1B2_68);
    		Exclude_Short_Signal(UT_NTS_iF1B3_85);
    		
    		Exclude_Short_Signal(TSD_iF2B0_93);
    		Exclude_Short_Signal(UT_iF2B0_103);
    		Exclude_Short_Signal(RUN_iF2B2_129);
    		Exclude_Short_Signal(SUF_iF2B3_141);
    		Exclude_Short_Signal(MRSW_iF2B3_142);
    		Exclude_Short_Signal(CST_iF2B3_153);
    		    		
    		
    					        	    		
			Turn_Signal_ON(o_15TSD_8);			
			Turn_Signal_ON(o_17CST_21);
 			 		
			//UT Output Module MD1A
			Turn_Signal_ON(PUT_LP_oF1B0_38);
			Turn_Signal_ON(UT_NTS_SPIout_64);
			
			//SUF Output Module MD1B
			Turn_Signal_ON(PSUF_LP_oF1B0_34);
			Turn_Signal_ON(SUF_CPU_oF2B0_53);
			
			//RUN Output Module MD6A
			Turn_Signal_ON(PRUN_LP_oF1B0_40);
			Turn_Signal_ON(RUN_CPU_oF2B0_55);
 			
        	
        	SEB_IO_TEST_CASE=CheckON_SEB_IO_SUF_S14;
        	timers[t_Generic_Delay]=0;
    	break;
    	
    	case CheckON_SEB_IO_SUF_S14:    		
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;    	    		

			New_write_display(SEB_IO_Test_Menu[1],2,0);
         	         	         	    		
			
 			if((Check_Signal_ON(i_suf_0)!=0)||(Check_Signal_ON(i_s2mrw_9)!=0)||(Check_Signal_ON(i_16mrw_14)!=0)
 				||(Check_Signal_ON(i_14mrw_15)!=0)||(Check_Signal_ON(i_11mrw_16)!=0)||(Check_Signal_ON(i_mrsw_21)!=0)
 				||(Check_Signal_ON(UPF_CPU_iF1B1_45)!=0)||(Check_Signal_ON(RUN_CPU_iF1B2_68)!=0)||(Check_Signal_ON(UT_NTS_iF1B3_85)!=0)				
 				||(Check_Signal_ON(TSD_iF2B0_93)!=0)||(Check_Signal_ON(UT_iF2B0_103)!=0)||(Check_Signal_ON(RUN_iF2B2_129)!=0)
 				||(Check_Signal_ON(SUF_iF2B3_141)!=0)||(Check_Signal_ON(MRSW_iF2B3_142)!=0)||(Check_Signal_ON(CST_iF2B3_153)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_SUF_S14;
			}
			
			timers[t_Generic_Delay]=0;  		    		
    	break;
    	
    	case TurnOFF_SEB_IO_SUF_S14: 
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		    		
    		New_write_display(SEB_IO_Test_Menu[3],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		    		
			Turn_Signal_OFF(o_15TSD_8);
			Turn_Signal_OFF(o_17CST_21);
 			
 			
			//UT Output Module MD1A
			Turn_Signal_OFF(PUT_LP_oF1B0_38);
			Turn_Signal_OFF(UT_NTS_SPIout_64);
			
			//SUF Output Module MD1B
			Turn_Signal_OFF(PSUF_LP_oF1B0_34);
			Turn_Signal_OFF(SUF_CPU_oF2B0_53);
			
			//RUN Output Module MD6A
			Turn_Signal_OFF(PRUN_LP_oF1B0_40);
			Turn_Signal_OFF(RUN_CPU_oF2B0_55);
			
			SEB_IO_TEST_CASE=CheckOFF_SEB_IO_SUF_S14;
			timers[t_Generic_Delay]=0;
    	break;
			    	
    	case CheckOFF_SEB_IO_SUF_S14: 
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		
    	    if((Check_Signal_OFF(i_suf_0)!=0)||(Check_Signal_OFF(i_s2mrw_9)!=0)||(Check_Signal_OFF(i_16mrw_14)!=0)
 				||(Check_Signal_OFF(i_14mrw_15)!=0)||(Check_Signal_OFF(i_11mrw_16)!=0)||(Check_Signal_OFF(i_mrsw_21)!=0)
 				||(Check_Signal_OFF(UPF_CPU_iF1B1_45)!=0)||(Check_Signal_OFF(RUN_CPU_iF1B2_68)!=0)||(Check_Signal_OFF(UT_NTS_iF1B3_85)!=0)				
 				||(Check_Signal_OFF(TSD_iF2B0_93)!=0)||(Check_Signal_OFF(UT_iF2B0_103)!=0)||(Check_Signal_OFF(RUN_iF2B2_129)!=0)
 				||(Check_Signal_OFF(SUF_iF2B3_141)!=0)||(Check_Signal_OFF(MRSW_iF2B3_142)!=0)||(Check_Signal_OFF(CST_iF2B3_153)!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_SU_S12;
			}
        	
        	        
    		Include_Short_Signal(i_suf_0);    		
    		Include_Short_Signal(i_s2mrw_9);
    		Include_Short_Signal(i_16mrw_14);
    		Include_Short_Signal(i_14mrw_15);
    		Include_Short_Signal(i_11mrw_16);
    		Include_Short_Signal(i_mrsw_21);
    		
    		Include_Short_Signal(UPF_CPU_iF1B1_45);
    		Include_Short_Signal(RUN_CPU_iF1B2_68);
    		Include_Short_Signal(UT_NTS_iF1B3_85);
    		
    		Include_Short_Signal(TSD_iF2B0_93);
    		Include_Short_Signal(UT_iF2B0_103);
    		Include_Short_Signal(RUN_iF2B2_129);
    		Include_Short_Signal(SUF_iF2B3_141);
    		Include_Short_Signal(MRSW_iF2B3_142);
    		Include_Short_Signal(CST_iF2B3_153);
    		
    		timers[t_Generic_Delay]=0;
    	break;
//------------------------------------------------------------------------------ 
//SU_S12
//i_su_2, i_s2mrw_9, i_16mrw_14, i_14mrw_15, i_11mrw_16, i_mrsw_21
//UPR_CPU_iF1B1_46, RUN_CPU_iF1B2_68, UN_NTS_iF1B3_84
//UN_iF2B0_104, RUN_iF2B2_129, SU_iF2B3_140, MRSW_iF2B3_142, CST_iF2B3_153 
    	case TurnON_SEB_IO_SU_S12:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[4],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_su_2);
    		Exclude_Short_Signal(i_s2mrw_9);
    		Exclude_Short_Signal(i_16mrw_14);
    		Exclude_Short_Signal(i_14mrw_15);
    		Exclude_Short_Signal(i_11mrw_16);
    		Exclude_Short_Signal(i_mrsw_21);
    		    		
    		Exclude_Short_Signal(UPR_CPU_iF1B1_46);    		
    		Exclude_Short_Signal(RUN_CPU_iF1B2_68);    		
    		Exclude_Short_Signal(UN_NTS_iF1B3_84);    		
    		
     		Exclude_Short_Signal(UN_iF2B0_104);
     		Exclude_Short_Signal(RUN_iF2B2_129);     		
    		Exclude_Short_Signal(SU_iF2B3_140);
    		Exclude_Short_Signal(MRSW_iF2B3_142);
    		Exclude_Short_Signal(CST_iF2B3_153);
    		    		
    		Turn_Signal_ON(o_17CST_21);
    		
    		//UN Output Module MD2A
			Turn_Signal_ON(PUN_LP_oF1B0_36);
			Turn_Signal_ON(UN_NTS_SPIout_65);
			
			//SU Output Module MD2B
			Turn_Signal_ON(PSU_LP_oF1B0_32);
			Turn_Signal_ON(SU_CPU_oF2B0_52);
			
			//RUN Output Module MD6A
			Turn_Signal_ON(PRUN_LP_oF1B0_40);
			Turn_Signal_ON(RUN_CPU_oF2B0_55);
    		
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_SU_S12;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_SU_S12:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		
    		if((Check_Signal_ON(i_su_2)!=0)||(Check_Signal_ON(i_s2mrw_9)!=0)||(Check_Signal_ON(i_16mrw_14)!=0)
 				||(Check_Signal_ON(i_14mrw_15)!=0)||(Check_Signal_ON(i_11mrw_16)!=0)||(Check_Signal_ON(i_mrsw_21)!=0)
 				||(Check_Signal_ON(UPR_CPU_iF1B1_46)!=0)||(Check_Signal_ON(RUN_CPU_iF1B2_68)!=0)||(Check_Signal_ON(UN_NTS_iF1B3_84)!=0)				
 				||(Check_Signal_ON(UN_iF2B0_104)!=0)||(Check_Signal_ON(RUN_iF2B2_129)!=0)||(Check_Signal_ON(SU_iF2B3_140)!=0)
 				||(Check_Signal_ON(MRSW_iF2B3_142)!=0)||(Check_Signal_ON(CST_iF2B3_153)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}		
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_SU_S12;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_SU_S12:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[5],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_17CST_21);
    		
    		//UN Output Module MD2A
			Turn_Signal_OFF(PUN_LP_oF1B0_36);
			Turn_Signal_OFF(UN_NTS_SPIout_65);
			
			//SU Output Module MD2B
			Turn_Signal_OFF(PSU_LP_oF1B0_32);
			Turn_Signal_OFF(SU_CPU_oF2B0_52);
			
			//RUN Output Module MD6A
			Turn_Signal_OFF(PRUN_LP_oF1B0_40);
			Turn_Signal_OFF(RUN_CPU_oF2B0_55);
    		
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_SU_S12;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_SU_S12:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(i_su_2)!=0)||(Check_Signal_OFF(i_s2mrw_9)!=0)||(Check_Signal_OFF(i_16mrw_14)!=0)
 				||(Check_Signal_OFF(i_14mrw_15)!=0)||(Check_Signal_OFF(i_11mrw_16)!=0)||(Check_Signal_OFF(i_mrsw_21)!=0)
 				||(Check_Signal_OFF(UPR_CPU_iF1B1_46)!=0)||(Check_Signal_OFF(RUN_CPU_iF1B2_68)!=0)||(Check_Signal_OFF(UN_NTS_iF1B3_84)!=0)				
 				||(Check_Signal_OFF(UN_iF2B0_104)!=0)||(Check_Signal_OFF(RUN_iF2B2_129)!=0)||(Check_Signal_OFF(SU_iF2B3_140)!=0)
 				||(Check_Signal_OFF(MRSW_iF2B3_142)!=0)||(Check_Signal_OFF(CST_iF2B3_153)!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_SDF_S10;
			}
			    
			Include_Short_Signal(i_su_2);
    		Include_Short_Signal(i_s2mrw_9);
    		Include_Short_Signal(i_16mrw_14);
    		Include_Short_Signal(i_14mrw_15);
    		Include_Short_Signal(i_11mrw_16);
    		Include_Short_Signal(i_mrsw_21);
    		    		
    		Include_Short_Signal(UPR_CPU_iF1B1_46);    		
    		Include_Short_Signal(RUN_CPU_iF1B2_68);    		
    		Include_Short_Signal(UN_NTS_iF1B3_84);    		
    		
     		Include_Short_Signal(UN_iF2B0_104);
     		Include_Short_Signal(RUN_iF2B2_129);     		
    		Include_Short_Signal(SU_iF2B3_140);
    		Include_Short_Signal(MRSW_iF2B3_142);
    		Include_Short_Signal(CST_iF2B3_153);
					
    		timers[t_Generic_Delay]=0; 
    	break; 
//------------------------------------------------------------------------------ 
//SDF_S10
//i_sdf_4, i_s2mrw_9, i_16mrw_14, i_14mrw_15, i_11mrw_16, i_mrsw_21
//DNF_CPU_iF1B1_44, RUN_CPU_iF1B2_68, DT_NTS_iF1B3_86
//SDF_iF2B0_101, DT_iF2B0_105, RUN_iF2B2_129, MRSW_iF2B3_142, CST_iF2B3_153 
    	case TurnON_SEB_IO_SDF_S10:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[6],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_sdf_4);
    		Exclude_Short_Signal(i_s2mrw_9);
    		Exclude_Short_Signal(i_16mrw_14);
    		Exclude_Short_Signal(i_14mrw_15);
    		Exclude_Short_Signal(i_11mrw_16);
    		Exclude_Short_Signal(i_mrsw_21);
     		    		
    		Exclude_Short_Signal(DNF_CPU_iF1B1_44);    		
    		Exclude_Short_Signal(RUN_CPU_iF1B2_68);      		  		
    		Exclude_Short_Signal(DT_NTS_iF1B3_86);    		    		
    		
     		Exclude_Short_Signal(SDF_iF2B0_101);     		
     		Exclude_Short_Signal(DT_iF2B0_105);         		 		
    		Exclude_Short_Signal(RUN_iF2B2_129);    		
    		Exclude_Short_Signal(MRSW_iF2B3_142);   		
    		Exclude_Short_Signal(CST_iF2B3_153);
     		    		
    		Turn_Signal_ON(o_17CST_21);
    		
    		//DT Output Module MD5A
			Turn_Signal_ON(PDT_LP_oF1B0_39);
			Turn_Signal_ON(DT_NTS_SPIout_66);
			
			//SDF Output Module MD5B
			Turn_Signal_ON(PSDF_LP_oF1B0_35);
			Turn_Signal_ON(SDF_CPU_oF2B0_54);
			
			//RUN Output Module MD6A
			Turn_Signal_ON(PRUN_LP_oF1B0_40);
			Turn_Signal_ON(RUN_CPU_oF2B0_55);
    		
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_SDF_S10;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_SDF_S10:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		
    		if((Check_Signal_ON(i_sdf_4)!=0)||(Check_Signal_ON(i_s2mrw_9)!=0)||(Check_Signal_ON(i_16mrw_14)!=0)
 				||(Check_Signal_ON(i_14mrw_15)!=0)||(Check_Signal_ON(i_11mrw_16)!=0)||(Check_Signal_ON(i_mrsw_21)!=0)			
 				||(Check_Signal_ON(DNF_CPU_iF1B1_44)!=0)||(Check_Signal_ON(RUN_CPU_iF1B2_68)!=0)||(Check_Signal_ON(DT_NTS_iF1B3_86)!=0)				
 				||(Check_Signal_ON(SDF_iF2B0_101)!=0)||(Check_Signal_ON(DT_iF2B0_105)!=0)||(Check_Signal_ON(RUN_iF2B2_129)!=0)
 				||(Check_Signal_ON(MRSW_iF2B3_142)!=0)||(Check_Signal_ON(CST_iF2B3_153)!=0)				
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}		
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_SDF_S10;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_SDF_S10:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[7],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_17CST_21);
    		
    		//DT Output Module MD5A
			Turn_Signal_OFF(PDT_LP_oF1B0_39);
			Turn_Signal_OFF(DT_NTS_SPIout_66);
			
			//SDF Output Module MD5B
			Turn_Signal_OFF(PSDF_LP_oF1B0_35);
			Turn_Signal_OFF(SDF_CPU_oF2B0_54);
			
			//RUN Output Module MD6A
			Turn_Signal_OFF(PRUN_LP_oF1B0_40);
			Turn_Signal_OFF(RUN_CPU_oF2B0_55);
    		
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_SDF_S10;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_SDF_S10:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(i_sdf_4)!=0)||(Check_Signal_OFF(i_s2mrw_9)!=0)||(Check_Signal_OFF(i_16mrw_14)!=0)
 				||(Check_Signal_OFF(i_14mrw_15)!=0)||(Check_Signal_OFF(i_11mrw_16)!=0)||(Check_Signal_OFF(i_mrsw_21)!=0)			
 				||(Check_Signal_OFF(DNF_CPU_iF1B1_44)!=0)||(Check_Signal_OFF(RUN_CPU_iF1B2_68)!=0)||(Check_Signal_OFF(DT_NTS_iF1B3_86)!=0)				
 				||(Check_Signal_OFF(SDF_iF2B0_101)!=0)||(Check_Signal_OFF(DT_iF2B0_105)!=0)||(Check_Signal_OFF(RUN_iF2B2_129)!=0)
 				||(Check_Signal_OFF(MRSW_iF2B3_142)!=0)||(Check_Signal_OFF(CST_iF2B3_153)!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_SD_S8;
			}
			    
			Include_Short_Signal(i_sdf_4);
    		Include_Short_Signal(i_s2mrw_9);
    		Include_Short_Signal(i_16mrw_14);
    		Include_Short_Signal(i_14mrw_15);
    		Include_Short_Signal(i_11mrw_16);
    		Include_Short_Signal(i_mrsw_21);
     		    		
    		Include_Short_Signal(DNF_CPU_iF1B1_44);    		
    		Include_Short_Signal(RUN_CPU_iF1B2_68);      		  		
    		Include_Short_Signal(DT_NTS_iF1B3_86);    		    		
    		
     		Include_Short_Signal(SDF_iF2B0_101);     		
     		Include_Short_Signal(DT_iF2B0_105);         		 		
    		Include_Short_Signal(RUN_iF2B2_129);    		
    		Include_Short_Signal(MRSW_iF2B3_142);   		
    		Include_Short_Signal(CST_iF2B3_153);
					
    		timers[t_Generic_Delay]=0; 
    	break;  
//------------------------------------------------------------------------------ 
//SD_S8
//i_sd_6, i_s2mrw_9, i_16mrw_14, i_14mrw_15, i_11mrw_16, i_mrsw_21
//DNR_CPU_iF1B1_47, RUN_CPU_iF1B2_68, DN_NTS_iF1B3_83
//SD_iF2B0_102, DN_iF2B0_106, RUN_iF2B2_129, MRSW_iF2B3_142, CST_iF2B3_153 
    	case TurnON_SEB_IO_SD_S8:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[8],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_sd_6);
    		Exclude_Short_Signal(i_s2mrw_9);
    		Exclude_Short_Signal(i_16mrw_14);
    		Exclude_Short_Signal(i_14mrw_15);
    		Exclude_Short_Signal(i_11mrw_16);
    		Exclude_Short_Signal(i_mrsw_21);
      		    		
    		Exclude_Short_Signal(DNR_CPU_iF1B1_47);        				
    		Exclude_Short_Signal(RUN_CPU_iF1B2_68);    		      		  		
    		Exclude_Short_Signal(DN_NTS_iF1B3_83);      		  		    		
    		
     		Exclude_Short_Signal(SD_iF2B0_102);       		   		
     		Exclude_Short_Signal(DN_iF2B0_106);       		       		 		
    		Exclude_Short_Signal(RUN_iF2B2_129);     		   		
    		Exclude_Short_Signal(MRSW_iF2B3_142);    		   		
    		Exclude_Short_Signal(CST_iF2B3_153);
      		    		
    		Turn_Signal_ON(o_17CST_21);
    		
    		//DN Output Module MD3A
			Turn_Signal_ON(PDN_LP_oF1B0_37);
			Turn_Signal_ON(DN_NTS_SPIout_67);
			
			//SD Output Module MD3B
			Turn_Signal_ON(PSD_LP_oF1B0_33);
			Turn_Signal_ON(SD_CPU_oF2B0_51);
			
			//RUN Output Module MD6A
			Turn_Signal_ON(PRUN_LP_oF1B0_40);
			Turn_Signal_ON(RUN_CPU_oF2B0_55);
    		
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_SD_S8;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_SD_S8:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		
    		if((Check_Signal_ON(i_sd_6)!=0)||(Check_Signal_ON(i_s2mrw_9)!=0)||(Check_Signal_ON(i_16mrw_14)!=0)
 				||(Check_Signal_ON(i_14mrw_15)!=0)||(Check_Signal_ON(i_11mrw_16)!=0)||(Check_Signal_ON(i_mrsw_21)!=0)	  						
 				||(Check_Signal_ON(DNR_CPU_iF1B1_47)!=0)||(Check_Signal_ON(RUN_CPU_iF1B2_68)!=0)||(Check_Signal_ON(DN_NTS_iF1B3_83)!=0)				
 				||(Check_Signal_ON(SD_iF2B0_102)!=0)||(Check_Signal_ON(DN_iF2B0_106)!=0)||(Check_Signal_ON(RUN_iF2B2_129)!=0)
 				||(Check_Signal_ON(MRSW_iF2B3_142)!=0)||(Check_Signal_ON(CST_iF2B3_153)!=0)	  							
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}		
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_SD_S8;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_SD_S8:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[9],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_17CST_21);
    		
    		//DN Output Module MD3A
			Turn_Signal_OFF(PDN_LP_oF1B0_37);
			Turn_Signal_OFF(DN_NTS_SPIout_67);
			
			//SD Output Module MD3B
			Turn_Signal_OFF(PSD_LP_oF1B0_33);
			Turn_Signal_OFF(SD_CPU_oF2B0_51);
			
			//RUN Output Module MD6A
			Turn_Signal_OFF(PRUN_LP_oF1B0_40);
			Turn_Signal_OFF(RUN_CPU_oF2B0_55);
    		
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_SD_S8;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_SD_S8:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(i_sd_6)!=0)||(Check_Signal_OFF(i_s2mrw_9)!=0)||(Check_Signal_OFF(i_16mrw_14)!=0)
 				||(Check_Signal_OFF(i_14mrw_15)!=0)||(Check_Signal_OFF(i_11mrw_16)!=0)||(Check_Signal_OFF(i_mrsw_21)!=0)	  						
 				||(Check_Signal_OFF(DNR_CPU_iF1B1_47)!=0)||(Check_Signal_OFF(RUN_CPU_iF1B2_68)!=0)||(Check_Signal_OFF(DN_NTS_iF1B3_83)!=0)			
 				||(Check_Signal_OFF(SD_iF2B0_102)!=0)||(Check_Signal_OFF(DN_iF2B0_106)!=0)||(Check_Signal_OFF(RUN_iF2B2_129)!=0)
 				||(Check_Signal_OFF(MRSW_iF2B3_142)!=0)||(Check_Signal_OFF(CST_iF2B3_153)!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_GOV_H22;
			}
			    
			Include_Short_Signal(i_sd_6);
    		Include_Short_Signal(i_s2mrw_9);
    		Include_Short_Signal(i_16mrw_14);
    		Include_Short_Signal(i_14mrw_15);
    		Include_Short_Signal(i_11mrw_16);
    		Include_Short_Signal(i_mrsw_21);
      		    		
    		Include_Short_Signal(DNR_CPU_iF1B1_47);        				
    		Include_Short_Signal(RUN_CPU_iF1B2_68);    		      		  		
    		Include_Short_Signal(DN_NTS_iF1B3_83);      		  		    		
    		
     		Include_Short_Signal(SD_iF2B0_102);       		   		
     		Include_Short_Signal(DN_iF2B0_106);       		       		 		
    		Include_Short_Signal(RUN_iF2B2_129);     		   		
    		Include_Short_Signal(MRSW_iF2B3_142);    		   		
    		Include_Short_Signal(CST_iF2B3_153);
					
    		timers[t_Generic_Delay]=0; 
    	break;    	    	   	    	  	    	   	
//------------------------------------------------------------------------------ 
//GOV_H22
//i_22gov_11
//GOV_iF2B0_95
    	case TurnON_SEB_IO_GOV_H22:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[10],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_22gov_11);
    		Exclude_Short_Signal(GOV_iF2B0_95);
    		    		
    		Turn_Signal_ON(o_23GOV_4);
    		
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_GOV_H22;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_GOV_H22:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(i_22gov_11)!=0)||(Check_Signal_ON(GOV_iF2B0_95)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_GOV_H22;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_GOV_H22:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[11],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_23GOV_4);
    		
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_GOV_H22;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_GOV_H22:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(i_22gov_11)!=0)||(Check_Signal_OFF(GOV_iF2B0_95)!=0))
    		{    			    		
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_TLH_H20;
			}
			    
			Include_Short_Signal(i_22gov_11);
			Include_Short_Signal(GOV_iF2B0_95);
					
    		timers[t_Generic_Delay]=0; 
    	break;
//------------------------------------------------------------------------------ 
//TLH_H20
//i_20tlh_12
    	case TurnON_SEB_IO_TLH_H20:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[12],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_20tlh_12);
    		    		
    		Turn_Signal_ON(o_21TLH_5);
    		
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_TLH_H20;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_TLH_H20:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(i_20tlh_12)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_TLH_H20;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_TLH_H20:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[13],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_21TLH_5);
    		
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_TLH_H20;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_TLH_H20:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if(Check_Signal_OFF(i_20tlh_12)!=0) 
    		{    			    		
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_PS_H18;
			}
			    
			Include_Short_Signal(i_20tlh_12);
					
    		timers[t_Generic_Delay]=0; 
    	break;    	
//------------------------------------------------------------------------------ 
//PS_H18
//i_h18ps_13
    	case TurnON_SEB_IO_PS_H18:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[14],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_h18ps_13);
    		    		
    		Turn_Signal_ON(o_H19PS_6);
    		   
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_PS_H18;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_PS_H18:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(i_h18ps_13)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_PS_H18;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_PS_H18:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[15],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_H19PS_6);
    		
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_PS_H18;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_PS_H18:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if(Check_Signal_OFF(i_h18ps_13)!=0) 
    		{    			    		
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_MC;
			}
			    
			Include_Short_Signal(i_h18ps_13);
					
    		timers[t_Generic_Delay]=0; 
    	break;    	
//------------------------------------------------------------------------------ 
//MC
//i_mc_22, i_s2mrw_9, i_16mrw_14, i_14mrw_15, i_11mrw_16, i_mrsw_21
//MC_CPU_iF1B1_49
//RUN_iF2B2_129, MRSW_iF2B3_142, MC_iF2B3_149, CST_iF2B3_153
    	case TurnON_SEB_IO_MC:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[16],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_mc_22);
    		Exclude_Short_Signal(i_s2mrw_9);
    		Exclude_Short_Signal(i_16mrw_14);
    		Exclude_Short_Signal(i_14mrw_15);
    		Exclude_Short_Signal(i_11mrw_16);
    		Exclude_Short_Signal(i_mrsw_21);
    		
    		Exclude_Short_Signal(MC_CPU_iF1B1_49);
    		
    		Exclude_Short_Signal(RUN_iF2B2_129);
    		Exclude_Short_Signal(MRSW_iF2B3_142);
    		Exclude_Short_Signal(MC_iF2B3_149);
    		Exclude_Short_Signal(CST_iF2B3_153);
    				
    		Turn_Signal_ON(o_17CST_21);
    		
    		//MC Output Module MD6B
			Turn_Signal_ON(PMC_LP_oF1B0_43);
			Turn_Signal_ON(MC_CPU_oF2B0_57); 
    		   
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_MC;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_MC:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(i_mc_22)!=0)||(Check_Signal_ON(i_s2mrw_9)!=0)||(Check_Signal_ON(i_16mrw_14)!=0)
 				||(Check_Signal_ON(i_14mrw_15)!=0)||(Check_Signal_ON(i_11mrw_16)!=0)||(Check_Signal_ON(i_mrsw_21)!=0)	 
    			||(Check_Signal_ON(MC_CPU_iF1B1_49)!=0) 
    			||(Check_Signal_ON(RUN_iF2B2_129)!=0)||(Check_Signal_ON(MRSW_iF2B3_142)!=0)
    			||(Check_Signal_ON(MC_iF2B3_149)!=0)||(Check_Signal_ON(CST_iF2B3_153)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_MC;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_MC:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[17],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_17CST_21);
    		
    		//MC Output Module MD6B
			Turn_Signal_OFF(PMC_LP_oF1B0_43);
			Turn_Signal_OFF(MC_CPU_oF2B0_57); 
			
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_MC;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_MC:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(i_mc_22)!=0)||(Check_Signal_OFF(i_s2mrw_9)!=0)||(Check_Signal_OFF(i_16mrw_14)!=0)
 				||(Check_Signal_OFF(i_14mrw_15)!=0)||(Check_Signal_OFF(i_11mrw_16)!=0)||(Check_Signal_OFF(i_mrsw_21)!=0)	 
    			||(Check_Signal_OFF(MC_CPU_iF1B1_49)!=0) 
    			||(Check_Signal_OFF(RUN_iF2B2_129)!=0)||(Check_Signal_OFF(MRSW_iF2B3_142)!=0)
    			||(Check_Signal_OFF(MC_iF2B3_149)!=0)||(Check_Signal_OFF(CST_iF2B3_153)!=0))			
			{    			    		
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_MC1;
			}
			    
			Include_Short_Signal(i_mc_22);
    		Include_Short_Signal(i_s2mrw_9);
    		Include_Short_Signal(i_16mrw_14);
    		Include_Short_Signal(i_14mrw_15);
    		Include_Short_Signal(i_11mrw_16);
    		Include_Short_Signal(i_mrsw_21);
    		
    		Include_Short_Signal(MC_CPU_iF1B1_49);
    		
    		Include_Short_Signal(RUN_iF2B2_129);
    		Include_Short_Signal(MRSW_iF2B3_142);
    		Include_Short_Signal(MC_iF2B3_149);
    		Include_Short_Signal(CST_iF2B3_153);						
					
    		timers[t_Generic_Delay]=0; 
    	break;    	
//------------------------------------------------------------------------------ 
//MC1
//i_mc1_23, i_s2mrw_9, i_16mrw_14, i_14mrw_15, i_11mrw_16, i_mrsw_21
//CEN_CPU_iF1B1_50   
//CEN_iF2B0_92, RUN_iF2B2_129, MRSW_iF2B3_142, CST_iF2B3_153
    	case TurnON_SEB_IO_MC1:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[18],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_mc1_23);
    		Exclude_Short_Signal(i_s2mrw_9);
    		Exclude_Short_Signal(i_16mrw_14);
    		Exclude_Short_Signal(i_14mrw_15);
    		Exclude_Short_Signal(i_11mrw_16);
    		Exclude_Short_Signal(i_mrsw_21);
    		
    		Exclude_Short_Signal(CEN_CPU_iF1B1_50);    		
    		
    		Exclude_Short_Signal(CEN_iF2B0_92);
    		Exclude_Short_Signal(RUN_iF2B2_129);
    		Exclude_Short_Signal(MRSW_iF2B3_142);    		
    		Exclude_Short_Signal(CST_iF2B3_153);
    				
    		Turn_Signal_ON(o_17CST_21);
    		
    		//MC1 Output Module MD4A
			Turn_Signal_ON(PCEN_LP_oF1B0_41);
			Turn_Signal_ON(CEN_CPU_oF2B0_56); 
    		   
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_MC1;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_MC1:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(i_mc1_23)!=0)||(Check_Signal_ON(i_s2mrw_9)!=0)||(Check_Signal_ON(i_16mrw_14)!=0)
 				||(Check_Signal_ON(i_14mrw_15)!=0)||(Check_Signal_ON(i_11mrw_16)!=0)||(Check_Signal_ON(i_mrsw_21)!=0)	   				
    			||(Check_Signal_ON(CEN_CPU_iF1B1_50)!=0)
    			||(Check_Signal_ON(CEN_iF2B0_92)!=0)||(Check_Signal_ON(RUN_iF2B2_129)!=0)
    			||(Check_Signal_ON(MRSW_iF2B3_142)!=0)||(Check_Signal_ON(CST_iF2B3_153)!=0)   		
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_MC1;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_MC1:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[19],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_17CST_21);
    		
    		//MC1 Output Module MD4A
			Turn_Signal_OFF(PCEN_LP_oF1B0_41);
			Turn_Signal_OFF(CEN_CPU_oF2B0_56); 
			
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_MC1;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_MC1:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(i_mc1_23)!=0)||(Check_Signal_OFF(i_s2mrw_9)!=0)||(Check_Signal_OFF(i_16mrw_14)!=0)
 				||(Check_Signal_OFF(i_14mrw_15)!=0)||(Check_Signal_OFF(i_11mrw_16)!=0)||(Check_Signal_OFF(i_mrsw_21)!=0)	   				
    			||(Check_Signal_OFF(CEN_CPU_iF1B1_50)!=0)
    			||(Check_Signal_OFF(CEN_iF2B0_92)!=0)||(Check_Signal_OFF(RUN_iF2B2_129)!=0)
    			||(Check_Signal_OFF(MRSW_iF2B3_142)!=0)||(Check_Signal_OFF(CST_iF2B3_153)!=0))			
			{   			    		
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_STE;
			}
			    
			Include_Short_Signal(i_mc1_23);
    		Include_Short_Signal(i_s2mrw_9);
    		Include_Short_Signal(i_16mrw_14);
    		Include_Short_Signal(i_14mrw_15);
    		Include_Short_Signal(i_11mrw_16);
    		Include_Short_Signal(i_mrsw_21);
    		
    		Include_Short_Signal(CEN_CPU_iF1B1_50);    		
    		
    		Include_Short_Signal(CEN_iF2B0_92);
    		Include_Short_Signal(RUN_iF2B2_129);
    		Include_Short_Signal(MRSW_iF2B3_142);    		
    		Include_Short_Signal(CST_iF2B3_153);						
					
    		timers[t_Generic_Delay]=0; 
    	break;    	
//------------------------------------------------------------------------------ 
//STE
//i_ste_24  
//STE_CPU_iF1B1_48
//TPH_iF2B0_97
    	case TurnON_SEB_IO_STE:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[20],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_ste_24);     		    		
    		
    		Exclude_Short_Signal(STE_CPU_iF1B1_48);    	    			
    		
    		Exclude_Short_Signal(TPH_iF2B0_97);    		    		
    		
    		Turn_Signal_ON(o_S1TPH_3);
    		
    		//STE Output Module MD4B
			Turn_Signal_ON(PSTE_LP_oF1B0_42);
			Turn_Signal_ON(STE_CPU_oF2B0_58); 
    		   
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_STE;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_STE:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(i_ste_24)!=0)     		 				   				
    			||(Check_Signal_ON(STE_CPU_iF1B1_48)!=0) 
    			||(Check_Signal_ON(TPH_iF2B0_97)!=0)   			  		
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_STE;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_STE:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[21],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_S1TPH_3);
    		
    		//STE Output Module MD4B
			Turn_Signal_OFF(PSTE_LP_oF1B0_42);
			Turn_Signal_OFF(STE_CPU_oF2B0_58); 
			
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_STE;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_STE:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(i_ste_24)!=0)     		 				   				
    			||(Check_Signal_OFF(STE_CPU_iF1B1_48)!=0)
    			||(Check_Signal_OFF(TPH_iF2B0_97)!=0))			
			{   			    		
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnON_SEB_IO_HSS_T18;
			}
			    
			Include_Short_Signal(i_ste_24);     		    		
    		
    		Include_Short_Signal(STE_CPU_iF1B1_48);    	    			
    		
    		Include_Short_Signal(TPH_iF2B0_97); 						
					
    		timers[t_Generic_Delay]=0; 
    	break;    	
//------------------------------------------------------------------------------ 
//HSS_T18
//i_18hss_26
    	case TurnON_SEB_IO_HSS_T18:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(SEB_IO_Test_Menu[22],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(i_18hss_26);
    		Exclude_Short_Signal(HSS_iF2B0_94);
    			    		
    		Turn_Signal_ON(o_17HSS_7);
    		   
    		SEB_IO_TEST_CASE=CheckON_SEB_IO_HSS_T18;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_SEB_IO_HSS_T18:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(i_18hss_26)!=0)||(Check_Signal_ON(HSS_iF2B0_94)!=0)
 				||(Check_Input_Shorted()!=0))			
			{
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=TurnOFF_SEB_IO_HSS_T18;
			}
			
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_SEB_IO_HSS_T18:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(SEB_IO_Test_Menu[23],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(o_17HSS_7);
    		
    		SEB_IO_TEST_CASE=CheckOFF_SEB_IO_HSS_T18;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_SEB_IO_HSS_T18:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(SEB_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if((Check_Signal_OFF(i_18hss_26)!=0)||(Check_Signal_OFF(HSS_iF2B0_94)!=0))
    		{    			    		
 				SEB_IO_TEST_CASE=SEB_IO_TEST_Fail;
			}
			else
			{
				SEB_IO_TEST_CASE=SEB_IO_TEST_Pass;
			}
			    
			Include_Short_Signal(i_18hss_26);
			Include_Short_Signal(HSS_iF2B0_94);
					
    		timers[t_Generic_Delay]=0; 
    	break;    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	 
//------------------------------------------------------------------------------     	    	    	    	    
    	case SEB_IO_TEST_Pass:
       		 CurrentTestPassFail = 1; // pass
        	 clr_lcd_dply();
        	 New_write_display("SEB IO TEST         ",0,0);
        	 New_write_display("TEST PASSED         ",1,0);
        	 New_write_display("GOOD JOB            ",2,0);     
        	 SEB_IO_TEST_CASE=SEB_IO_TEST_Done;        
        break;
        
    	case SEB_IO_TEST_Done:
			 SEB_IO_TEST_CASE=SEB_IO_Test_Init; 
        
          	 // Required for Retest Option
        	 if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        	 else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     	case Retest_option: // Required for Retest Option
        	 RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        	 Retest_Option_Parm = 1;
        	 if(RetestResponse == 0) SEB_IO_TEST_CASE=SEB_IO_TEST_Done;
        	 else if(RetestResponse == 1) SEB_IO_TEST_CASE=SEB_IO_Test_Init; 
        	 else 
        	 	ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    	case SEB_IO_TEST_Fail:
//-    		clr_lcd_dply();
    		Disply_Fault();
        	SEB_IO_TEST_CASE = Retest_option;
        break;

    	default:
        	SEB_IO_TEST_CASE=SEB_IO_Test_Init;
        	ReturnVal = DEFAULT;
        break;
    }	
    
    return ReturnVal; // Required for Retest Option	
}

































