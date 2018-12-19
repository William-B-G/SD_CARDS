#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"





int16 v120AC_Power_Check(int16 Operation);
void Exclude_PowerAndReset_InputPin(void);






enum
{
	Init_v120AC_Power_Check=0,
	Check_ALL_v120AC_Input,
	v120AC_Power_Check_Pass,
	v120AC_Power_Check_Done,
	v120AC_Power_Check_Fail,
	Retest_option				// Required for Retest Option    
};	




//------------------------------------------------------------------------------   
int16 v120AC_Power_Check(int16 Operation)
{
	static int16 v120AC_Power_Check_CASE=Init_v120AC_Power_Check;
 	int16 Delay_Count=test_2s_delay;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    switch (v120AC_Power_Check_CASE) 
    {
    	case Init_v120AC_Power_Check:
       		CurrentTestPassFail=0;  // fail
       		clr_lcd_dply();
        	ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        	Retest_Option_Parm = 0; // Required for Retest Option
        	RetestResponse = 1; // Required for Retest Option        
 	            	
       	 	New_write_display("v120AC Power Check  ",0,0);       	 
                	                	
        	v120AC_Power_Check_CASE=Check_ALL_v120AC_Input;      
        	
        	Include_ALL_Short_Signal();        	
        	Reset_Fault_Type_Index();          
        	Turn_ALL_Output_OFF();        	
        	Exclude_PowerAndReset_InputPin();		
        	Clear_Log_Buffer(); 	
			timers[t_Generic_Delay]=0;			
        break;
        
    	case Check_ALL_v120AC_Input:   
    		//100ms=1/10s once
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;      		    					        	    					
 			
        	
        	if((Check_Signal_ON(i_s6s10_8)!=0)||(Check_Signal_ON(i_24s10_10)!=0)||(Check_Signal_ON(i_h8s10_17)!=0)
        	 	||(Check_Signal_ON(i_h6s10_18)!=0)||(Check_Signal_ON(i_h4s10_19)!=0)||(Check_Signal_ON(i_h2s10_20)!=0)
        	    ||(Check_Signal_ON(i_s10_25)!=0)
        	    ||(Check_Signal_ON(L120B_iF2B0_91)!=0)||(Check_Signal_ON(S10_iF2B0_96)!=0)||(Check_Signal_ON(L120_iF2B1_122)!=0)
        	    ||(Check_Signal_ON(RESET_iF2B2_138)!=0)
        	    ||(Check_Input_Shorted()!=0))
        	{
        	    v120AC_Power_Check_CASE=v120AC_Power_Check_Fail;	
            }
        	else
        	{
        		v120AC_Power_Check_CASE=v120AC_Power_Check_Pass;
        	}        	        	         	        	
        	timers[t_Generic_Delay]=0;
    	break;
    	    	    	    	    
    	case v120AC_Power_Check_Pass:
       		 CurrentTestPassFail = 1; // pass
        	 clr_lcd_dply();
        	 New_write_display("v120AC Power Check  ",0,0);
        	 New_write_display("TEST PASSED         ",1,0);
        	 New_write_display("GOOD JOB            ",2,0);     
        	 v120AC_Power_Check_CASE=v120AC_Power_Check_Done;        
        break;
        
    	case v120AC_Power_Check_Done:
			 v120AC_Power_Check_CASE=Init_v120AC_Power_Check; 
        
          	 // Required for Retest Option
        	 if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        	 else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     	case Retest_option: // Required for Retest Option
        	 RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        	 Retest_Option_Parm = 1;
        	 if(RetestResponse == 0) v120AC_Power_Check_CASE=v120AC_Power_Check_Done;
        	 else if(RetestResponse == 1) v120AC_Power_Check_CASE=Init_v120AC_Power_Check; 
        	 else 
        	 	ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    	case v120AC_Power_Check_Fail:
//-    		clr_lcd_dply();
    		Disply_Fault();		
        	v120AC_Power_Check_CASE = Retest_option;
        break;

    	default:
        	v120AC_Power_Check_CASE=Init_v120AC_Power_Check;
        	ReturnVal = DEFAULT;
        break;
    }	
    
    return ReturnVal; // Required for Retest Option	
}







void Exclude_PowerAndReset_InputPin(void)
{
	//While v120AC Power Connected, these signal are HIGH
    Exclude_Short_Signal(i_s6s10_8);
    Exclude_Short_Signal(i_24s10_10);
    Exclude_Short_Signal(i_h8s10_17);
    Exclude_Short_Signal(i_h6s10_18);
    Exclude_Short_Signal(i_h4s10_19);
    Exclude_Short_Signal(i_h2s10_20);
    Exclude_Short_Signal(i_s10_25);

	Exclude_Short_Signal(L120B_iF2B0_91);
	Exclude_Short_Signal(S10_iF2B0_96);
	Exclude_Short_Signal(L120_iF2B1_122);
			
			
	//Reset Bit is ALWAYS HIGH
	Exclude_Short_Signal(RESET_iF2B2_138);
	
	
	//FPGA1 Buff2
 	//PSTE,PDN,PUN,PUT,PDT,PSD,PSU,PSUF,PSDF,
 	//are Output Signal from FPGA1 that Energize the Hardware Gate,
 	//however, FPGA1 sends these output signal back to CPU(Production Code) by communication;
 	//for Testing Code here, DONT_CARE becasue they DONT GET Physical Wiring involved
 	Exclude_Short_Signal(PSTE_iF1B2_59);
 	Exclude_Short_Signal(PDN_iF1B2_60);
 	Exclude_Short_Signal(PUN_iF1B2_61);
 	Exclude_Short_Signal(PUT_iF1B2_62);
 	Exclude_Short_Signal(PDT_iF1B2_63); 	
 	Exclude_Short_Signal(PSD_iF1B2_64);
 	Exclude_Short_Signal(PSU_iF1B2_65);
 	Exclude_Short_Signal(PSUF_iF1B2_66);
	Exclude_Short_Signal(PSDF_iF1B2_67);
	Exclude_Short_Signal(PRUN_iF1B3_88);
	Exclude_Short_Signal(PCEN_iF1B3_89);
	Exclude_Short_Signal(PMC_iF1B3_90);
	
	
	//*DZ_PAL is OUT from FPGA1 TO FPGA2, 
 	//									For Production Code:
 	//														CPU NEEDS to READ back From FPGA1 and FPGA2									
 	//								    But For Testing Code: 
	//								                        CPU NO NEED to READ back From FPGA1(DEFAULT Value '0')
	//								   						BUT CPU READS back from FPGA2 
	Exclude_Short_Signal(DZ_PAL_iF1B2_72);
}


























