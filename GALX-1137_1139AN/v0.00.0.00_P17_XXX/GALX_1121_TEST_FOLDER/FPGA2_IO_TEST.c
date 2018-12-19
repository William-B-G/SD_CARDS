#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"
#include "v120AC_Power_Check.h"





int16 FPGA2_IO_TEST(int16 Operation);










//---------------------------------------
enum
{
	FPGA2_IO_TEST_TEMP_Input1_Index=0,
	FPGA2_IO_TEST_TEMP_Input2_Index,
	FPGA2_IO_TEST_TEMP_Input3_Index,
	FPGA2_IO_TEST_TEMP_Output1_Index,
	FPGA2_IO_TEST_TEMP_Output2_Index,
	FPGA2_IO_TEST_TEMP_Output3_Index
};




#define FPGA2_IO_TEST_TEMP_IO_num			12
#define Cutoff_FPGA2_IO_TEST_TEMP_IO_num	8
#define Special_FPGA2_IO_TEST_TEMP_IO_num	9
int16 FPGA2_IO_TEST_TEMP_ARRAY[FPGA2_IO_TEST_TEMP_IO_num][6]=
{
	//FPGA2 Output to FPGA1 ONLY
	//Input1		  Input2	         Input3	      	Output1		    Output2				Output3		
	{TSD_iF2B0_93,	  0x00,              0x00, 			o_15TSD_8,   	  0x00,				  0x00          },	//0
	{TPL_iF2B0_98,	  0x00,              0x00, 			o_S3TPL_2,        0x00,               0x00          },	//1
	{LOS_iF2B0_99,	  0x00,              0x00, 			o_S4LOS_1,        0x00,               0x00          },	//2			
	{IEN_iF2B2_130,	  0x00,              0x00, 			o_13IEN_25,       0x00,               0x00          },	//3
	{DZ_iF2B3_139,	  0x00,              0x00, 			DZ_PAL_oF1B0_45,  0x00,               0x00          },	//4
	{MTO_iF2B3_150,	  0x00,              0x00, 			o_0MTO_17,        0x00,               0x00          },	//5
	{SPD_iF2B3_151,	  0x00,              0x00,			o_0SPD_18,        0x00,               0x00          },	//6
	{CFLT_iF2B3_152,  0x00,              0x00,			o_0CFLT_19,       0x00,               0x00          },	//7
	
	
	{UT_iF2B0_103,    UT_NTS_iF1B3_85,   TSD_iF2B0_93,	o_15TSD_8,      PUT_LP_oF1B0_38,    UT_NTS_SPIout_64},	//8   Cutoff_FPGA2_IO_TEST_TEMP_IO_num
	{UN_iF2B0_104,	  UN_NTS_iF1B3_84,	 CST_iF2B3_153,	o_17CST_21,	    PUN_LP_oF1B0_36,	UN_NTS_SPIout_65},	//9	  Special_FPGA2_IO_TEST_TEMP_IO_num
	{DT_iF2B0_105,	  DT_NTS_iF1B3_86,	 CST_iF2B3_153,	o_17CST_21,	    PDT_LP_oF1B0_39,    DT_NTS_SPIout_66},	//10
	{DN_iF2B0_106,	  DN_NTS_iF1B3_83,	 CST_iF2B3_153,	o_17CST_21,	    PDN_LP_oF1B0_37,	DN_NTS_SPIout_67},	//11		
};






//---------------------------------------
enum
{
	FPGA2_IO_Test_Init=0,
	TurnON_FPGA2_IO_SEQUENCE_CHECK,
	CheckON_FPGA2_IO_SEQUENCE_CHECK,
	TurnOFF_FPGA2_IO_SEQUENCE_CHECK,
	CheckOFF_FPGA2_IO_SEQUENCE_CHECK,
	FPGA2_IO_TEST_Pass,
	FPGA2_IO_TEST_Done,
	FPGA2_IO_TEST_Fail,
	Retest_option				// Required for Retest Option  
};


	
	
#define fpga2_io_test_menu_index		30
static char FPGA2_IO_Test_Menu[fpga2_io_test_menu_index][20]= 
{
  "FPGA2 IO TEST       ",       //0
  "Checking FPGA2_IO...",		//1
  "FPGA2_IO_TSD_L ON   ",       //2 
  "FPGA2_IO_TSD_L OFF  ",       //3
  "FPGA2_IO_TPL_L ON   ",       //4
  "FPGA2_IO_TPL_L OFF  ",       //5
  "FPGA2_IO_LOS_L ON   ",       //6
  "FPGA2_IO_LOS_L OFF  ",       //7
  "FPGA2_IO_IEN_L ON   ",       //8
  "FPGA2_IO_IEN_L OFF  ",       //9
  "FPGA2_IO_DZ_PAL ON  ",       //10
  "FPGA2_IO_DZ_PAL OFF ",       //11
  "FPGA2_IO_MTO_L ON   ",       //12
  "FPGA2_IO_MTO_L OFF  ",       //13
  "FPGA2_IO_SPD_L ON   ",       //12
  "FPGA2_IO_SPD_L OFF  ",       //13
  "FPGA2_IO_CFLT_L ON  ",       //14
  "FPGA2_IO_CFLT_L OFF ",       //15
  "FPGA2_IO_UT_L ON    ",       //16
  "FPGA2_IO_UT_L OFF   ",       //17
  "FPGA2_IO_UN_L ON    ",       //18
  "FPGA2_IO_UN_L OFF   ",       //19
  "FPGA2_IO_DT_L ON    ",       //20
  "FPGA2_IO_DT_L OFF   ",       //21
  "FPGA2_IO_DN_L ON    ",       //22
  "FPGA2_IO_DN_L OFF   ",       //23
};






//------------------------------------------------------------------------------   
int16 FPGA2_IO_TEST(int16 Operation)
{
	static int16 FPGA2_IO_TEST_CASE=FPGA2_IO_Test_Init;
 	int16 Delay_Count=test_halfsec_delay;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    static int16 f2_seqIO_i=0;
    
    
    switch (FPGA2_IO_TEST_CASE) 
    {
    	case FPGA2_IO_Test_Init:
    		f2_seqIO_i=0;
       		CurrentTestPassFail=0;  // fail
       		clr_lcd_dply();
        	ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        	Retest_Option_Parm = 0; // Required for Retest Option
        	RetestResponse = 1; // Required for Retest Option        
 	            	
       	 	New_write_display(FPGA2_IO_Test_Menu[0],0,0);       	 
                	                	
         	FPGA2_IO_TEST_CASE=TurnON_FPGA2_IO_SEQUENCE_CHECK;      
        	
        	Include_ALL_Short_Signal();        	
        	Reset_Fault_Type_Index();          
        	Turn_ALL_Output_OFF();        	
        	Exclude_PowerAndReset_InputPin();
        	Clear_Log_Buffer(); 			
			timers[t_Generic_Delay]=0;
        break;
//------------------------------------------------------------------------------ 
//SEQUENCE_CHECK
    	case TurnON_FPGA2_IO_SEQUENCE_CHECK:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;  
    		 
    		New_write_display(FPGA2_IO_Test_Menu[2+f2_seqIO_i*2],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
     		Exclude_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input1_Index]); 
     		     	
     		Turn_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Output1_Index]);
     		
     		if(f2_seqIO_i==Cutoff_FPGA2_IO_TEST_TEMP_IO_num)
     		{
     			Exclude_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input2_Index]); 
     			Exclude_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input3_Index]); 
     			
     			Turn_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Output2_Index]);
     			Turn_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Output3_Index]);	
     		} 
     		else if(f2_seqIO_i>=Special_FPGA2_IO_TEST_TEMP_IO_num) 
     		{
     			Exclude_Short_Signal(i_s2mrw_9);
    			Exclude_Short_Signal(i_16mrw_14);
    			Exclude_Short_Signal(i_14mrw_15);
    			Exclude_Short_Signal(i_11mrw_16);
    			Exclude_Short_Signal(i_mrsw_21);
    			Exclude_Short_Signal(RUN_iF2B2_129);    		
     			Exclude_Short_Signal(MRSW_iF2B3_142); 
     			
     			Exclude_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input2_Index]); 
     			Exclude_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input3_Index]); 
     			
     			Turn_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Output2_Index]);
     			Turn_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Output3_Index]);
     		}
    		   
    		FPGA2_IO_TEST_CASE=CheckON_FPGA2_IO_SEQUENCE_CHECK;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckON_FPGA2_IO_SEQUENCE_CHECK:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA2_IO_Test_Menu[1],2,0);
    		
    		
    		if(((Check_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input1_Index])!=0)&&(f2_seqIO_i<Cutoff_FPGA2_IO_TEST_TEMP_IO_num))
    			||
    		   (((Check_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input1_Index])!=0)||(Check_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input2_Index])!=0)||(Check_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input3_Index])!=0))&&(f2_seqIO_i==Cutoff_FPGA2_IO_TEST_TEMP_IO_num))
 				||
 			   (((Check_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input1_Index])!=0)||(Check_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input2_Index])!=0)||(Check_Signal_ON(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input3_Index])!=0)
 			   		||(Check_Signal_ON(i_s2mrw_9)!=0)||(Check_Signal_ON(i_16mrw_14)!=0)
 					||(Check_Signal_ON(i_14mrw_15)!=0)||(Check_Signal_ON(i_11mrw_16)!=0)||(Check_Signal_ON(i_mrsw_21)!=0) 
 					||(Check_Signal_ON(RUN_iF2B2_129)!=0)||(Check_Signal_ON(MRSW_iF2B3_142)!=0)			   
 			   		)&&(f2_seqIO_i>=Special_FPGA2_IO_TEST_TEMP_IO_num))	
 				||(Check_Input_Shorted()!=0)
 			   )			
			{
 				FPGA2_IO_TEST_CASE=FPGA2_IO_TEST_Fail;
			}
			else
			{
				FPGA2_IO_TEST_CASE=TurnOFF_FPGA2_IO_SEQUENCE_CHECK;
			}     	
    					
			timers[t_Generic_Delay]=0;  
    	break;
    	
    	case TurnOFF_FPGA2_IO_SEQUENCE_CHECK:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    	
    		New_write_display(FPGA2_IO_Test_Menu[3+f2_seqIO_i*2],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    	
    		Turn_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Output1_Index]);
    		
    		if(f2_seqIO_i>=Cutoff_FPGA2_IO_TEST_TEMP_IO_num)
     		{
     			Turn_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Output2_Index]);
     			Turn_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Output3_Index]);
     		}
     		
    		FPGA2_IO_TEST_CASE=CheckOFF_FPGA2_IO_SEQUENCE_CHECK;
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case CheckOFF_FPGA2_IO_SEQUENCE_CHECK:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(FPGA2_IO_Test_Menu[1],2,0);    		    		
    		
    		
    		if(((Check_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input1_Index])!=0)&&(f2_seqIO_i<Cutoff_FPGA2_IO_TEST_TEMP_IO_num))
    			||
    		   (((Check_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input1_Index])!=0)||(Check_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input2_Index])!=0)||(Check_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input3_Index])!=0))&&(f2_seqIO_i==Cutoff_FPGA2_IO_TEST_TEMP_IO_num))
    		    ||
    		   (((Check_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input1_Index])!=0)||(Check_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input2_Index])!=0)||(Check_Signal_OFF(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input3_Index])!=0)
    		    	||(Check_Signal_OFF(i_s2mrw_9)!=0)||(Check_Signal_OFF(i_16mrw_14)!=0)
 					||(Check_Signal_OFF(i_14mrw_15)!=0)||(Check_Signal_OFF(i_11mrw_16)!=0)||(Check_Signal_OFF(i_mrsw_21)!=0)
 					||(Check_Signal_OFF(RUN_iF2B2_129)!=0)||(Check_Signal_OFF(MRSW_iF2B3_142)!=0)
    		    	)&&(f2_seqIO_i>=Special_FPGA2_IO_TEST_TEMP_IO_num))
    		    )	
    		{    			    		
 				FPGA2_IO_TEST_CASE=FPGA2_IO_TEST_Fail;
			}
			else
			{				
				Include_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input1_Index]);
				
				if(f2_seqIO_i==Cutoff_FPGA2_IO_TEST_TEMP_IO_num)
				{
					Include_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input2_Index]);
					Include_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input3_Index]);										
				}
				else if(f2_seqIO_i>=Special_FPGA2_IO_TEST_TEMP_IO_num) 
     			{
     				Include_Short_Signal(i_s2mrw_9);
    				Include_Short_Signal(i_16mrw_14);
    				Include_Short_Signal(i_14mrw_15);
    				Include_Short_Signal(i_11mrw_16);
    				Include_Short_Signal(i_mrsw_21);
     				Include_Short_Signal(RUN_iF2B2_129); 
     				Include_Short_Signal(MRSW_iF2B3_142); 
     				
     				Include_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input2_Index]); 
     				Include_Short_Signal(FPGA2_IO_TEST_TEMP_ARRAY[f2_seqIO_i][FPGA2_IO_TEST_TEMP_Input3_Index]); 
     			}
     			
     			
				f2_seqIO_i++;
				
				if(f2_seqIO_i>=FPGA2_IO_TEST_TEMP_IO_num)
				{
					FPGA2_IO_TEST_CASE=FPGA2_IO_TEST_Pass;
					f2_seqIO_i=0;
				}
				else
				{
					FPGA2_IO_TEST_CASE=TurnON_FPGA2_IO_SEQUENCE_CHECK;
				}				
			}			    			         		     		      		
					
    		timers[t_Generic_Delay]=0; 
    	break;    	                   
//------------------------------------------------------------------------------          
        case FPGA2_IO_TEST_Pass:
       		 CurrentTestPassFail = 1; // pass
        	 clr_lcd_dply();
        	 New_write_display("FPGA2 IO TEST       ",0,0);
        	 New_write_display("TEST PASSED         ",1,0);
        	 New_write_display("GOOD JOB            ",2,0);     
        	 FPGA2_IO_TEST_CASE=FPGA2_IO_TEST_Done;        
        break;
        
    	case FPGA2_IO_TEST_Done:
			 FPGA2_IO_TEST_CASE=FPGA2_IO_Test_Init; 
        
          	 // Required for Retest Option
        	 if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        	 else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     	case Retest_option: // Required for Retest Option
        	 RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        	 Retest_Option_Parm = 1;
        	 if(RetestResponse == 0) FPGA2_IO_TEST_CASE=FPGA2_IO_TEST_Done;
        	 else if(RetestResponse == 1) FPGA2_IO_TEST_CASE=FPGA2_IO_Test_Init; 
        	 else 
        	 	ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    	case FPGA2_IO_TEST_Fail:
//-    		clr_lcd_dply();
    		Disply_Fault();
        	FPGA2_IO_TEST_CASE = Retest_option;
        break;

    	default:
        	FPGA2_IO_TEST_CASE=FPGA2_IO_Test_Init;
        	ReturnVal = DEFAULT;
        break;
    }	
    
    return ReturnVal; // Required for Retest Option	            
}
























