#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"
#include "v120AC_Power_Check.h"




int16 SW_TEST(int16 Operation);





enum 
{
    SW_Test_Init=0,  
    Flip_BYPASS_CAR_SW5_DOWN,
    Check_BYPASS_CAR_SW5_DOWN,
    Flip_BYPASS_CAR_SW5_UP,
    Check_BYPASS_CAR_SW5_UP,
    
    Flip_BYPASS_HWAY_SW7_DOWN,
    Check_BYPASS_HWAY_SW7_DOWN,
    Flip_BYPASS_HWAY_SW7_UP,
    Check_BYPASS_HWAY_SW7_UP,
            
    Flip_IND_SW8_DOWN,
    Check_IND_SW8_DOWN,
    Flip_IND_SW8_UP,
    Check_IND_SW8_UP,
            
    Flip_AUTO_DOOR_SW9_UP,
    Check_AUTO_DOOR_SW9_UP,
    Flip_AUTO_DOOR_SW9_DOWN,
    Check_AUTO_DOOR_SW9_DOWN,
    
    Flip_STOP_SW1_DOWN,
    Check_STOP_SW1_DOWN,
    Flip_STOP_SW1_UP,
    Check_STOP_SW1_UP,
    
    Flip_INSP_SW6_DOWN,
    Check_INSP_SW6_DOWN,
    Flip_INSP_SW6_UP,
    Check_INSP_SW6_UP,
    Check_INSP_SW6_UP_OFF,
    
    PRESS_ENABLE_SW2, 
    Check_PRESS_ENABLE_SW2,
    RELEASE_ENABLE_SW2,
    Check_RELEASE_ENABLE_SW2,
    
    PRESS_UP_SW3_ENABLE_SW2, 
    Check_PRESS_UP_SW3_ENABLE_SW2,
    RELEASE_UP_SW3_ENABLE_SW2,
    Check_RELEASE_UP_SW3_ENABLE_SW2,
    
    PRESS_DOWN_SW4_ENABLE_SW2, 
    Check_PRESS_DOWN_SW4_ENABLE_SW2,
    RELEASE_DOWN_SW4_ENABLE_SW2,
    Check_RELEASE_DOWN_SW4_ENABLE_SW2,                
	
    SW_Test_Fail,
    SW_Test_Pass,
    SW_Test_Done,
    Retest_option		 // Required for Retest Option    
};




#define sw_test_menu_index		30
static char Switch_Test_Menu[sw_test_menu_index][21]= 
{
  "SW TEST             ",       //0 
  "Checking SW...      ",		//1 
  "BYPASS_CAR_SW5 DOWN ",       //2  
  "BYPASS_CAR_SW5 UP   ",       //3
  "BYPASS_HWAY_SW7 DOWN",       //4
  "BYPASS_HWAY_SW7 UP  ",       //5
  "IND_SW8 DOWN        ",       //6
  "IND_SW8 UP          ",       //7
  "AUTO_DOOR_SW9 UP    ",       //8
  "AUTO_DOOR_SW9 DOWN  ",       //9
  "STOP_SW1 UP         ",       //10
  "STOP_SW1 DOWN       ",       //11    
  "INSP_SW6 DOWN       ",       //12
  "INSP_SW6 UP         ",       //13
  "PRESS ENABLE_SW2    ",		//14
  "RELEASE ENABLE_SW2  ",       //15
  "PS UP_SW3&ENABLE_SW2",		//16
  "RS UP_SW3&ENABLE_SW2",       //17
  "PS DN_SW4&ENABLE_SW2",		//18
  "RS DN_SW4&ENABLE_SW2",       //19  
  };


int16 Check_STOP_SW1_UP_Index=0;










//------------------------------------------------------------------------------   
int16 SW_TEST(int16 Operation)
{
	static int16 SW_TEST_CASE=SW_Test_Init;
//-    int16 Delay_Count= consB[20];
 	int16 Delay_Count=test_halfsec_delay;
 	int16 Test_MAX_DT=test_20s_delay;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    switch (SW_TEST_CASE) 
    {
    	case SW_Test_Init:
       		CurrentTestPassFail=0;  // fail
       		clr_lcd_dply();
        	ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        	Retest_Option_Parm = 0; // Required for Retest Option
        	RetestResponse = 1; // Required for Retest Option        
 	            	
       	 	New_write_display(Switch_Test_Menu[0],0,0);       		
       		    
        	SW_TEST_CASE=Flip_BYPASS_CAR_SW5_DOWN;
        	
        	Include_ALL_Short_Signal();        	
        	Reset_Fault_Type_Index();          	
        	Turn_ALL_Output_OFF();        	
			Exclude_PowerAndReset_InputPin();	
			Clear_Log_Buffer(); 
			timers[t_Generic_Delay]=0;
			Check_STOP_SW1_UP_Index=0;								
        break;
//------------------------------------------------------------------------------  
//BYPASS_CAR_SW5      
//GBP_iF1B0_39
//GBP_iF2B3_143 
        case Flip_BYPASS_CAR_SW5_DOWN:   
        	//100ms=1/10s once
        	if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
        	
        	New_write_display(Switch_Test_Menu[2],1,0);
        	
        	Exclude_Short_Signal(GBP_iF1B0_39);
			Exclude_Short_Signal(GBP_iF2B3_143);
			     	    		    		
    		SW_TEST_CASE=Check_BYPASS_CAR_SW5_DOWN;  
    		timers[t_Generic_Delay]=0;     
    	break;
    	
    	case Check_BYPASS_CAR_SW5_DOWN:    		
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(GBP_iF1B0_39)==0)&&(Check_Signal_ON(GBP_iF2B3_143)==0))        	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
        			SW_TEST_CASE=Flip_BYPASS_CAR_SW5_UP;
        			        			        			
        			Include_Short_Signal(GBP_iF1B0_39);
					Include_Short_Signal(GBP_iF2B3_143);
					timers[t_Generic_Delay]=0;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_ON(GBP_iF1B0_39)!=0)||(Check_Signal_ON(GBP_iF2B3_143)!=0))  
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}        	         	
    	break;
        
    	case Flip_BYPASS_CAR_SW5_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;    		
    		
    		New_write_display(Switch_Test_Menu[3],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		SW_TEST_CASE=Check_BYPASS_CAR_SW5_UP;  
    		timers[t_Generic_Delay]=0; 
    	break;
        
    	case Check_BYPASS_CAR_SW5_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_OFF(GBP_iF1B0_39)==0)&&(Check_Signal_OFF(GBP_iF2B3_143)==0))
    		{
    			SW_TEST_CASE=Flip_BYPASS_HWAY_SW7_DOWN;        			        			        			        		
				timers[t_Generic_Delay]=0;
    		}
    		else
    		{
    			if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_OFF(GBP_iF1B0_39)!=0)||(Check_Signal_OFF(GBP_iF2B3_143)!=0))
    				{
    					SW_TEST_CASE=SW_Test_Fail;
    				}
        		}
    		}
    	break;
//------------------------------------------------------------------------------  
//BYPASS_HOISTWAY_SW7 
//GLB_iF1B0_40, LBP_iF1B0_41 , AUTO_iF1B0_42
//GLB_iF2B3_144,LBP_iF2B3_145, AUTO_iF2B3_148      
        case Flip_BYPASS_HWAY_SW7_DOWN:   
        	if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
        	
        	New_write_display(Switch_Test_Menu[4],1,0);
        	New_write_display(clr_dplyline[0],2,0);
        	
        	Exclude_Short_Signal(LBP_iF1B0_41);
			Exclude_Short_Signal(LBP_iF2B3_145);
			     	    		    		
    		SW_TEST_CASE=Check_BYPASS_HWAY_SW7_DOWN;  
    		timers[t_Generic_Delay]=0;     
    	break;
    	
    	case Check_BYPASS_HWAY_SW7_DOWN:    		
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(LBP_iF1B0_41)==0)&&(Check_Signal_ON(LBP_iF2B3_145)==0))        	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
        			SW_TEST_CASE=Flip_BYPASS_HWAY_SW7_UP;
        			        			        			
        			Include_Short_Signal(LBP_iF1B0_41);
					Include_Short_Signal(LBP_iF2B3_145);
					timers[t_Generic_Delay]=0;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_ON(LBP_iF1B0_41)!=0)||(Check_Signal_ON(LBP_iF2B3_145)!=0))  
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}        	         	
    	break;
    		
    	case Flip_BYPASS_HWAY_SW7_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[5],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(GLB_iF1B0_40);
			Exclude_Short_Signal(GLB_iF2B3_144);
    		Exclude_Short_Signal(AUTO_iF1B0_42);
			Exclude_Short_Signal(AUTO_iF2B3_148);
			
    		Turn_Signal_ON(o_12ICA_26);
    		
     		SW_TEST_CASE=Check_BYPASS_HWAY_SW7_UP;  
    		timers[t_Generic_Delay]=0; 
    	break;
    	
    	case Check_BYPASS_HWAY_SW7_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_OFF(LBP_iF1B0_41)==0)&&(Check_Signal_OFF(LBP_iF2B3_145)==0))
    		{    			    			    		
    			if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    				&&(Check_Signal_ON(AUTO_iF1B0_42)==0)&&(Check_Signal_ON(AUTO_iF2B3_148)==0))
    			{
    				if(Check_Input_Shorted()!=0)
        			{
    					SW_TEST_CASE=SW_Test_Fail;
        			}
        			else
        			{
         				SW_TEST_CASE=Flip_IND_SW8_DOWN;     
        				
        				Include_Short_Signal(GLB_iF1B0_40);
						Include_Short_Signal(GLB_iF2B3_144);
    					Include_Short_Signal(AUTO_iF1B0_42);
						Include_Short_Signal(AUTO_iF2B3_148); 
						
						Turn_Signal_OFF(o_12ICA_26);			        			        			        		
						timers[t_Generic_Delay]=0;
        			}
    			}
    			else
    			{
    				if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0)
    					||(Check_Signal_ON(AUTO_iF1B0_42)!=0)||(Check_Signal_ON(AUTO_iF2B3_148)!=0))
    				{
    					SW_TEST_CASE=SW_Test_Fail;
    				}
    			}
    		}
    		else
    		{
    			if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_OFF(LBP_iF1B0_41)!=0)||(Check_Signal_OFF(LBP_iF2B3_145)!=0))
    				{
    					SW_TEST_CASE=SW_Test_Fail;
    				}
        		}
    		}
    	break;    	    	
//------------------------------------------------------------------------------  
//IND_SW8 
//IND_iF2B3_146    	
    	case Flip_IND_SW8_DOWN:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		    		
        	if((Check_Signal_OFF(GLB_iF1B0_40)!=0)||(Check_Signal_OFF(GLB_iF2B3_144)!=0)
    			||(Check_Signal_OFF(AUTO_iF1B0_42)!=0)||(Check_Signal_OFF(AUTO_iF2B3_148)!=0))
    		{
    			SW_TEST_CASE=SW_Test_Fail;
    		}
    				
        	New_write_display(Switch_Test_Menu[6],1,0);
        	New_write_display(clr_dplyline[0],2,0);
        	
        	Exclude_Short_Signal(IND_iF2B3_146);
			     	    		    		
    		SW_TEST_CASE=Check_IND_SW8_DOWN;  
    		timers[t_Generic_Delay]=0;     
    	break;

    	case Check_IND_SW8_DOWN:    		
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if(Check_Signal_ON(IND_iF2B3_146)==0)     	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
        			SW_TEST_CASE=Flip_IND_SW8_UP;
        			        			        			
        			Include_Short_Signal(IND_iF2B3_146);
					timers[t_Generic_Delay]=0;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if(Check_Signal_ON(IND_iF2B3_146)!=0)   
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}        	         	
    	break;
        
    	case Flip_IND_SW8_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;    		
    		
    		New_write_display(Switch_Test_Menu[7],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		SW_TEST_CASE=Check_IND_SW8_UP;  
    		timers[t_Generic_Delay]=0; 
    	break;
        
    	case Check_IND_SW8_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if(Check_Signal_OFF(IND_iF2B3_146)==0) 
    		{
     			SW_TEST_CASE=Flip_AUTO_DOOR_SW9_UP;        			        			        			        		
				timers[t_Generic_Delay]=0;
    		}
    		else
    		{
    			if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if(Check_Signal_OFF(IND_iF2B3_146)!=0) 
    				{
    					SW_TEST_CASE=SW_Test_Fail;
    				}
        		}
    		}
    	break;
//------------------------------------------------------------------------------  
//AUTO_DOOR_SW9 
//AD_iF2B3_147  	
    	case Flip_AUTO_DOOR_SW9_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
        	
        	New_write_display(Switch_Test_Menu[8],1,0);
        	New_write_display(clr_dplyline[0],2,0);
        	
        	Exclude_Short_Signal(AD_iF2B3_147);
			     	    		    		
    		SW_TEST_CASE=Check_AUTO_DOOR_SW9_UP;  
    		timers[t_Generic_Delay]=0;     
    	break;

    	case Check_AUTO_DOOR_SW9_UP:    		
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if(Check_Signal_ON(AD_iF2B3_147)==0)     	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
        			SW_TEST_CASE=Flip_AUTO_DOOR_SW9_DOWN;
        			        			        			
        			Include_Short_Signal(AD_iF2B3_147);
					timers[t_Generic_Delay]=0;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if(Check_Signal_ON(AD_iF2B3_147)!=0)   
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}        	         	
    	break;
        
    	case Flip_AUTO_DOOR_SW9_DOWN:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;    		
    		
    		New_write_display(Switch_Test_Menu[9],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		SW_TEST_CASE=Check_AUTO_DOOR_SW9_DOWN;  
    		timers[t_Generic_Delay]=0; 
    	break;
        
    	case Check_AUTO_DOOR_SW9_DOWN:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if(Check_Signal_OFF(AD_iF2B3_147)==0) 
    		{
    			SW_TEST_CASE=Flip_STOP_SW1_UP;        			        			        			        		
				timers[t_Generic_Delay]=0;
    		}
    		else
    		{
    			if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if(Check_Signal_OFF(AD_iF2B3_147)!=0) 
    				{
    					SW_TEST_CASE=SW_Test_Fail;
    				}
        		}
    		}
    	break; 
//------------------------------------------------------------------------------  
//STOP_SW1
//i_s2mrw_9,  i_16mrw_14,  i_14mrw_15,   i_11mrw_16,   i_mrsw_21
//RUN_iF2B2_129,  MRSW_iF2B3_142,  CST_iF2B3_153       
    	case Flip_STOP_SW1_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
        	
        	New_write_display(Switch_Test_Menu[10],1,0);
        	New_write_display(clr_dplyline[0],2,0);
        	
        	Exclude_Short_Signal(i_s2mrw_9);   
			Exclude_Short_Signal(i_16mrw_14);
			Exclude_Short_Signal(i_14mrw_15);
			Exclude_Short_Signal(i_11mrw_16);
			Exclude_Short_Signal(i_mrsw_21);
			
			Exclude_Short_Signal(RUN_iF2B2_129);
			Exclude_Short_Signal(MRSW_iF2B3_142);
			Exclude_Short_Signal(CST_iF2B3_153);
			     	    		    		
			Turn_Signal_ON(o_17CST_21);
			
    		SW_TEST_CASE=Check_STOP_SW1_UP;  
    		timers[t_Generic_Delay]=0;     
    	break;
		
    	case Check_STOP_SW1_UP:    		
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(i_s2mrw_9)==0)&&(Check_Signal_ON(i_16mrw_14)==0)&&(Check_Signal_ON(i_14mrw_15)==0)   
    			&&(Check_Signal_ON(i_11mrw_16)==0)&&(Check_Signal_ON(i_mrsw_21)==0)
    			&&(Check_Signal_ON(RUN_iF2B2_129)==0)&&(Check_Signal_ON(MRSW_iF2B3_142)==0)&&(Check_Signal_ON(CST_iF2B3_153)==0))    	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{        			
        			if(Check_STOP_SW1_UP_Index==2)
        			{
        				SW_TEST_CASE=Flip_INSP_SW6_DOWN;
        				Turn_Signal_OFF(o_17CST_21);
        			}
        			else
        			{
        				SW_TEST_CASE=Flip_STOP_SW1_DOWN;
//ZYM 1-24-2018
//NO HERE, IF IT IS HERE, SW IS NOT TESTED								
//						Turn_Signal_OFF(o_17CST_21);
        			}
        			
        			        			        			
        			Include_Short_Signal(i_s2mrw_9);   
					Include_Short_Signal(i_16mrw_14);
					Include_Short_Signal(i_14mrw_15);
					Include_Short_Signal(i_11mrw_16);
					Include_Short_Signal(i_mrsw_21);
					
					Include_Short_Signal(RUN_iF2B2_129);
					Include_Short_Signal(MRSW_iF2B3_142);
					Include_Short_Signal(CST_iF2B3_153);

					timers[t_Generic_Delay]=0;
					Check_STOP_SW1_UP_Index=1;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_ON(i_s2mrw_9)!=0)||(Check_Signal_ON(i_16mrw_14)!=0)||(Check_Signal_ON(i_14mrw_15)!=0) 
        				||(Check_Signal_ON(i_11mrw_16)!=0)||(Check_Signal_ON(i_mrsw_21)!=0)
        				||(Check_Signal_ON(RUN_iF2B2_129)!=0)||(Check_Signal_ON(MRSW_iF2B3_142)!=0)||(Check_Signal_ON(CST_iF2B3_153)!=0))   
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}        	         	
    	break;
        
    	case Flip_STOP_SW1_DOWN:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;    		
    		
    		New_write_display(Switch_Test_Menu[11],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		SW_TEST_CASE=Check_STOP_SW1_DOWN;  
    		timers[t_Generic_Delay]=0; 
    	break;
        
    	case Check_STOP_SW1_DOWN:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_OFF(i_s2mrw_9)==0)&&(Check_Signal_OFF(i_16mrw_14)==0)&&(Check_Signal_OFF(i_14mrw_15)==0) 
    			&&(Check_Signal_OFF(i_11mrw_16)==0)&&(Check_Signal_OFF(i_mrsw_21)==0)
    			&&(Check_Signal_OFF(RUN_iF2B2_129)==0)&&(Check_Signal_OFF(MRSW_iF2B3_142)==0)
//ZYM 1-24-2018
//"CST_iF2B3_153" is ALWAYS ON while "Turn_Signal_ON(o_17CST_21)"    			
    			&&(Check_Signal_ON(CST_iF2B3_153)==0))
    		{
//-    			SW_TEST_CASE=Flip_INSP_SW6_DOWN;  
				SW_TEST_CASE=Flip_STOP_SW1_UP;
				Check_STOP_SW1_UP_Index=2;
//ZYM 1-24-2018 
//MUST HERE    			
    			Turn_Signal_OFF(o_17CST_21);      			        			        			        		
				timers[t_Generic_Delay]=0;
    		}
    		else
    		{
    			if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_OFF(i_s2mrw_9)!=0)||(Check_Signal_OFF(i_16mrw_14)!=0)||(Check_Signal_OFF(i_14mrw_15)!=0)
        				||(Check_Signal_OFF(i_11mrw_16)!=0)||(Check_Signal_OFF(i_mrsw_21)!=0)
        				||(Check_Signal_OFF(RUN_iF2B2_129)!=0)||(Check_Signal_OFF(MRSW_iF2B3_142)!=0)
//ZYM 1-24-2018
//"CST_iF2B3_153" is ALWAYS ON while "Turn_Signal_ON(o_17CST_21)"        				
        				||(Check_Signal_ON(CST_iF2B3_153)!=0)) 
    				{
    					SW_TEST_CASE=SW_Test_Fail;
    				}
        		}
    		}
    	break;    	    	    	    	    	    	    	    	    	    	    	    	
//------------------------------------------------------------------------------  
//INSP_SW6 DOWN  
//GLB_iF1B0_40,  MRI_iF1B1_54
//GLB_iF2B3_144, MRIN_iF2B2_125   	
    	case Flip_INSP_SW6_DOWN:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
        	
        	New_write_display(Switch_Test_Menu[12],1,0);
        	New_write_display(clr_dplyline[0],2,0);
        	
        	Exclude_Short_Signal(GLB_iF1B0_40);
			Exclude_Short_Signal(GLB_iF2B3_144); 
			   					    		        	
        	Exclude_Short_Signal(MRI_iF1B1_54);
			Exclude_Short_Signal(MRIN_iF2B2_125);
			   			   
			Turn_Signal_ON(o_12ICA_26);
			     	    		    		
    		SW_TEST_CASE=Check_INSP_SW6_DOWN;  
    		timers[t_Generic_Delay]=0;             	        	
    	break;
    	
    	case Check_INSP_SW6_DOWN:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    			&&(Check_Signal_ON(MRI_iF1B1_54)==0)&&(Check_Signal_ON(MRIN_iF2B2_125)==0))        	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
        			SW_TEST_CASE=PRESS_ENABLE_SW2;
        			 
        			Include_Short_Signal(GLB_iF1B0_40);
					Include_Short_Signal(GLB_iF2B3_144);     
					   			        			
        			Include_Short_Signal(MRI_iF1B1_54);
					Include_Short_Signal(MRIN_iF2B2_125);
					
					Turn_Signal_OFF(o_12ICA_26);										
					timers[t_Generic_Delay]=0;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0)
        				||(Check_Signal_ON(MRI_iF1B1_54)!=0)||(Check_Signal_ON(MRIN_iF2B2_125)!=0))  
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}        	         	
    	break;
//------------------------------------------------------------------------------  
//INSP_SW6 DOWN, ENABLE_SW2 
//GLB_iF1B0_40,  MRI_iF1B1_54
//GLB_iF2B3_144, MRIN_iF2B2_125,  MRIE_iF2B2_127
    	case PRESS_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		     	
    		if((Check_Signal_OFF(GLB_iF1B0_40)!=0)||(Check_Signal_OFF(GLB_iF2B3_144)!=0)
        		||(Check_Signal_OFF(MRI_iF1B1_54)!=0)||(Check_Signal_OFF(MRIN_iF2B2_125)!=0))  
        	{
        		SW_TEST_CASE=SW_Test_Fail;
        	} 
        			
        			
    		New_write_display(Switch_Test_Menu[14],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(GLB_iF1B0_40);
			Exclude_Short_Signal(GLB_iF2B3_144); 
			
			Exclude_Short_Signal(MRI_iF1B1_54);
			Exclude_Short_Signal(MRIN_iF2B2_125);
			
    		Exclude_Short_Signal(MRIE_iF2B2_127);
    		
    		Turn_Signal_ON(o_12ICA_26);
    		
     		SW_TEST_CASE=Check_PRESS_ENABLE_SW2;  
    		timers[t_Generic_Delay]=0;  
    	break;
    	
    	case Check_PRESS_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    			&&(Check_Signal_ON(MRI_iF1B1_54)==0)&&(Check_Signal_ON(MRIN_iF2B2_125)==0)
    			&&(Check_Signal_ON(MRIE_iF2B2_127)==0))        	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
        			SW_TEST_CASE=RELEASE_ENABLE_SW2;
        			
        			Include_Short_Signal(GLB_iF1B0_40);
					Include_Short_Signal(GLB_iF2B3_144); 
					
					Include_Short_Signal(MRI_iF1B1_54);
					Include_Short_Signal(MRIN_iF2B2_125);
			
        			Include_Short_Signal(MRIE_iF2B2_127);        			  
//ZYM 1-24-2018
//NO HERE, IF IT IS HERE, SW IS NOT TESTED								
//        			Turn_Signal_OFF(o_12ICA_26);          			    			        			        													
					timers[t_Generic_Delay]=0;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0) 
        				||(Check_Signal_ON(MRI_iF1B1_54)!=0)||(Check_Signal_ON(MRIN_iF2B2_125)!=0) 
        				||(Check_Signal_ON(MRIE_iF2B2_127)!=0)) 
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}     
    	break;
    	
    	case RELEASE_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[15],1,0);
    		New_write_display(clr_dplyline[0],2,0);    		    
    		
     		SW_TEST_CASE=Check_RELEASE_ENABLE_SW2;  
    		timers[t_Generic_Delay]=0;  
    	break;
    	
    	case Check_RELEASE_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);

//ZYM 1-24-2018
//"GLB_iF1B0_40" and "GLB_iF2B3_144" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP  
//"MRI_iF1B1_54" and "MRIN_iF2B2_125" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP; 
//also, "INSP_SW6 DOWN" is FLIPED DOWN    		
    		if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    			&&(Check_Signal_ON(MRI_iF1B1_54)==0)&&(Check_Signal_ON(MRIN_iF2B2_125)==0)
    			&&(Check_Signal_OFF(MRIE_iF2B2_127)==0))        	    
        	{        		
        		SW_TEST_CASE=PRESS_UP_SW3_ENABLE_SW2;
//ZYM 1-24-2018 
//MUST HERE      
				Turn_Signal_OFF(o_12ICA_26);   			        			        			        													
				timers[t_Generic_Delay]=0;        		      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
//ZYM 1-24-2018
//"GLB_iF1B0_40" and "GLB_iF2B3_144" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP  
//"MRI_iF1B1_54" and "MRIN_iF2B2_125" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP; 
//also, "INSP_SW6 DOWN" is FLIPED DOWN    		        		
        			if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0)
        				||(Check_Signal_ON(MRI_iF1B1_54)!=0)||(Check_Signal_ON(MRIN_iF2B2_125)!=0)
        				||(Check_Signal_OFF(MRIE_iF2B2_127)!=0))  
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}     
    	break;    	    	    	    	    	    	    	    	    	    	    	    	
//------------------------------------------------------------------------------  
//INSP_SW6 DOWN, MRIU(UP)_SW3, ENABLE_SW2
//GLB_iF1B0_40,  MRI_iF1B1_54,                     MRIU_iF1B1_53
//GLB_iF2B3_144, MRIN_iF2B2_125,  MRIE_iF2B2_127,  MRIU_iF2B2_126
    	case PRESS_UP_SW3_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[16],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(GLB_iF1B0_40);
			Exclude_Short_Signal(GLB_iF2B3_144); 
			
			Exclude_Short_Signal(MRI_iF1B1_54);
			Exclude_Short_Signal(MRIN_iF2B2_125);
			
    		Exclude_Short_Signal(MRIE_iF2B2_127);
    		
    		Exclude_Short_Signal(MRIU_iF1B1_53);
    		Exclude_Short_Signal(MRIU_iF2B2_126);
    		    		    		
    		Turn_Signal_ON(o_12ICA_26);
    		
     		SW_TEST_CASE=Check_PRESS_UP_SW3_ENABLE_SW2;  
    		timers[t_Generic_Delay]=0;
    	break;
    	
    	case Check_PRESS_UP_SW3_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    			&&(Check_Signal_ON(MRI_iF1B1_54)==0)&&(Check_Signal_ON(MRIN_iF2B2_125)==0)
    			&&(Check_Signal_ON(MRIE_iF2B2_127)==0)
    			&&(Check_Signal_ON(MRIU_iF1B1_53)==0)&&(Check_Signal_ON(MRIU_iF2B2_126)==0))       	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
        			SW_TEST_CASE=RELEASE_UP_SW3_ENABLE_SW2;
        			
        			Include_Short_Signal(GLB_iF1B0_40);
					Include_Short_Signal(GLB_iF2B3_144); 
			
					Include_Short_Signal(MRI_iF1B1_54);
					Include_Short_Signal(MRIN_iF2B2_125);
			
    				Include_Short_Signal(MRIE_iF2B2_127);
    		
    				Include_Short_Signal(MRIU_iF1B1_53);
    				Include_Short_Signal(MRIU_iF2B2_126);
//ZYM 1-24-2018
//NO HERE, IF IT IS HERE, SW IS NOT TESTED	    			    		    		
//    				Turn_Signal_OFF(o_12ICA_26);       			        			        													
					timers[t_Generic_Delay]=0;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0)
        				||(Check_Signal_ON(MRI_iF1B1_54)!=0)||(Check_Signal_ON(MRIN_iF2B2_125)!=0)
        				||(Check_Signal_ON(MRIE_iF2B2_127)!=0)
        				||(Check_Signal_ON(MRIU_iF1B1_53)!=0)||(Check_Signal_ON(MRIU_iF2B2_126)!=0))  
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}     
    	break;

    	case RELEASE_UP_SW3_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[17],1,0);
    		New_write_display(clr_dplyline[0],2,0);    		    
    		
     		SW_TEST_CASE=Check_RELEASE_UP_SW3_ENABLE_SW2;  
    		timers[t_Generic_Delay]=0;  
    	break;
    	
    	
    	case Check_RELEASE_UP_SW3_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
//ZYM 1-24-2018
//"GLB_iF1B0_40" and "GLB_iF2B3_144" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP  
//"MRI_iF1B1_54" and "MRIN_iF2B2_125" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP; 
//also, "INSP_SW6 DOWN" is FLIPED DOWN    		    		
    		if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    			&&(Check_Signal_ON(MRI_iF1B1_54)==0)&&(Check_Signal_ON(MRIN_iF2B2_125)==0)
    			&&(Check_Signal_OFF(MRIE_iF2B2_127)==0)
    			&&(Check_Signal_OFF(MRIU_iF1B1_53)==0)&&(Check_Signal_OFF(MRIU_iF2B2_126)==0))       	    
        	{        		
        		SW_TEST_CASE=PRESS_DOWN_SW4_ENABLE_SW2;
//ZYM 1-24-2018 
//MUST HERE          		
        		Turn_Signal_OFF(o_12ICA_26);	        			        			        													
				timers[t_Generic_Delay]=0;        		      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
//ZYM 1-24-2018
//"GLB_iF1B0_40" and "GLB_iF2B3_144" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP  
//"MRI_iF1B1_54" and "MRIN_iF2B2_125" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP; 
//also, "INSP_SW6 DOWN" is FLIPED DOWN    	        		
        			if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0)
        				||(Check_Signal_ON(MRI_iF1B1_54)!=0)||(Check_Signal_ON(MRIN_iF2B2_125)!=0)
        				||(Check_Signal_OFF(MRIE_iF2B2_127)!=0)
        				||(Check_Signal_OFF(MRIU_iF1B1_53)!=0)||(Check_Signal_OFF(MRIU_iF2B2_126)!=0)) 
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}     
    	break;    	   
//------------------------------------------------------------------------------  
//INSP_SW6 DOWN, MRID(DOWN)_SW4, ENABLE_SW2  
//GLB_iF1B0_40,  MRI_iF1B1_54,                     MRID_iF1B1_52
//GLB_iF2B3_144, MRIN_iF2B2_125,  MRIE_iF2B2_127,  MRID_iF2B2_128  	
    	case PRESS_DOWN_SW4_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[18],1,0);
    		New_write_display(clr_dplyline[0],2,0);
    		
    		Exclude_Short_Signal(GLB_iF1B0_40);
			Exclude_Short_Signal(GLB_iF2B3_144); 
			
			Exclude_Short_Signal(MRI_iF1B1_54);
			Exclude_Short_Signal(MRIN_iF2B2_125);
			
    		Exclude_Short_Signal(MRIE_iF2B2_127);
    		
    		Exclude_Short_Signal(MRID_iF1B1_52);
    		Exclude_Short_Signal(MRID_iF2B2_128);
    		    		    		
    		Turn_Signal_ON(o_12ICA_26);
    		
     		SW_TEST_CASE=Check_PRESS_DOWN_SW4_ENABLE_SW2;  
    		timers[t_Generic_Delay]=0;
    	break;
    	
    	case Check_PRESS_DOWN_SW4_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    			&&(Check_Signal_ON(MRI_iF1B1_54)==0)&&(Check_Signal_ON(MRIN_iF2B2_125)==0)
    			&&(Check_Signal_ON(MRIE_iF2B2_127)==0)
    			&&(Check_Signal_ON(MRID_iF1B1_52)==0)&&(Check_Signal_ON(MRID_iF2B2_128)==0))       	    
        	{
        		if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
        			SW_TEST_CASE=RELEASE_DOWN_SW4_ENABLE_SW2;
        			
        			Include_Short_Signal(GLB_iF1B0_40);
					Include_Short_Signal(GLB_iF2B3_144); 
			
					Include_Short_Signal(MRI_iF1B1_54);
					Include_Short_Signal(MRIN_iF2B2_125);
			
    				Include_Short_Signal(MRIE_iF2B2_127);
    		
    				Include_Short_Signal(MRID_iF1B1_52);
    				Include_Short_Signal(MRID_iF2B2_128);
//ZYM 1-24-2018
//NO HERE, IF IT IS HERE, SW IS NOT TESTED	       			    		    		
//    				Turn_Signal_OFF(o_12ICA_26);       			        			        													
					timers[t_Generic_Delay]=0;
        		}      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0)
        				||(Check_Signal_ON(MRI_iF1B1_54)!=0)||(Check_Signal_ON(MRIN_iF2B2_125)!=0)
        				||(Check_Signal_ON(MRIE_iF2B2_127)!=0)
        				||(Check_Signal_ON(MRID_iF1B1_52)!=0)||(Check_Signal_ON(MRID_iF2B2_128)!=0))  
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}     
    	break;

    	case RELEASE_DOWN_SW4_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[19],1,0);
    		New_write_display(clr_dplyline[0],2,0);    		    
    		
     		SW_TEST_CASE=Check_RELEASE_DOWN_SW4_ENABLE_SW2;  
    		timers[t_Generic_Delay]=0;  
    	break;
    	
    	
    	case Check_RELEASE_DOWN_SW4_ENABLE_SW2:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break; 
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
//ZYM 1-24-2018
//"GLB_iF1B0_40" and "GLB_iF2B3_144" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP  
//"MRI_iF1B1_54" and "MRIN_iF2B2_125" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP; 
//also, "INSP_SW6 DOWN" is FLIPED DOWN    	    		
    		if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    			&&(Check_Signal_ON(MRI_iF1B1_54)==0)&&(Check_Signal_ON(MRIN_iF2B2_125)==0)
    			&&(Check_Signal_OFF(MRIE_iF2B2_127)==0)
    			&&(Check_Signal_OFF(MRID_iF1B1_52)==0)&&(Check_Signal_OFF(MRID_iF2B2_128)==0))       	    
        	{        		
        		SW_TEST_CASE=Flip_INSP_SW6_UP;
//ZYM 1-24-2018 
//MUST HERE         
				Turn_Signal_OFF(o_12ICA_26); 			        			        			        													
				timers[t_Generic_Delay]=0;        		      	    	
            }
        	else
        	{
        		if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
//ZYM 1-24-2018
//"GLB_iF1B0_40" and "GLB_iF2B3_144" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP  
//"MRI_iF1B1_54" and "MRIN_iF2B2_125" are ALWAYS ON 
//while "Turn_Signal_ON(o_12ICA_26)" and "BYPASS_CAR_SW5" and "BYPASS_HOISTWAY_SW7" are FLIPED UP; 
//also, "INSP_SW6 DOWN" is FLIPED DOWN            		
        			if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0)
        				||(Check_Signal_ON(MRI_iF1B1_54)!=0)||(Check_Signal_ON(MRIN_iF2B2_125)!=0)
        				||(Check_Signal_OFF(MRIE_iF2B2_127)!=0)
        				||(Check_Signal_OFF(MRID_iF1B1_52)!=0)||(Check_Signal_OFF(MRID_iF2B2_128)!=0)) 
        			{
        				SW_TEST_CASE=SW_Test_Fail;
        			}        			
        		}
        	}     
    	break;    	                   
//------------------------------------------------------------------------------  
//INSP_SW6 UP  
//GLB_iF1B0_40,  AUTO_iF1B0_42	    
//GLB_iF2B3_144, AUTO_iF2B3_148
    	case Flip_INSP_SW6_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[13],1,0);
    		
    		Exclude_Short_Signal(GLB_iF1B0_40);
			Exclude_Short_Signal(GLB_iF2B3_144); 
			
    		Exclude_Short_Signal(AUTO_iF1B0_42);
			Exclude_Short_Signal(AUTO_iF2B3_148);
			
			Turn_Signal_ON(o_12ICA_26);
			
    		SW_TEST_CASE=Check_INSP_SW6_UP;  
    		timers[t_Generic_Delay]=0; 
    	break;
        
    	case Check_INSP_SW6_UP:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		New_write_display(Switch_Test_Menu[1],2,0);
    		
    		
    		if((Check_Signal_ON(GLB_iF1B0_40)==0)&&(Check_Signal_ON(GLB_iF2B3_144)==0)
    			&&(Check_Signal_ON(AUTO_iF1B0_42)==0)&&(Check_Signal_ON(AUTO_iF2B3_148)==0))
    		{
    			if(Check_Input_Shorted()!=0)
        		{
        			SW_TEST_CASE=SW_Test_Fail;
        		}  
        		else
        		{
    				SW_TEST_CASE=Check_INSP_SW6_UP_OFF;  
    			
    				Include_Short_Signal(GLB_iF1B0_40);
					Include_Short_Signal(GLB_iF2B3_144); 
			
    				Include_Short_Signal(AUTO_iF1B0_42);
					Include_Short_Signal(AUTO_iF2B3_148);
			  	
			  		Turn_Signal_OFF(o_12ICA_26);		        			        			        		
					timers[t_Generic_Delay]=0;
        		}
    		}    		
    		else
    		{
    			if(timers[t_Generic_Delay]>=Test_MAX_DT)
        		{
        			if((Check_Signal_ON(GLB_iF1B0_40)!=0)||(Check_Signal_ON(GLB_iF2B3_144)!=0)
        				||(Check_Signal_ON(AUTO_iF1B0_42)!=0)||(Check_Signal_ON(AUTO_iF2B3_148)!=0))
    				{
    					SW_TEST_CASE=SW_Test_Fail;
    				}
        		}
    		}
    	break;
    	
    	case Check_INSP_SW6_UP_OFF:
    		if(timers[t_Generic_Delay]<Delay_Count)
            	break;
    		
    		if((Check_Signal_OFF(GLB_iF1B0_40)!=0)||(Check_Signal_OFF(GLB_iF2B3_144)!=0)
        		||(Check_Signal_OFF(AUTO_iF1B0_42)!=0)||(Check_Signal_OFF(AUTO_iF2B3_148)!=0))
    		{
    			SW_TEST_CASE=SW_Test_Fail;
    		}
    		else
    		{
    			SW_TEST_CASE=SW_Test_Pass;
    		}
    	break;    	    	    	    	        	
//------------------------------------------------------------------------------  	
    	case SW_Test_Pass:
       		 CurrentTestPassFail = 1; // pass
        	 clr_lcd_dply();
        	 New_write_display("SW TEST             ",0,0);
        	 New_write_display("TEST PASSED         ",1,0);
        	 New_write_display("GOOD JOB            ",2,0);     
        	 SW_TEST_CASE=SW_Test_Done;        
        break;
        
    	case SW_Test_Done:
			 SW_TEST_CASE=SW_Test_Init; 
        
          	 // Required for Retest Option
        	 if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        	 else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
       
     	case Retest_option: // Required for Retest Option
        	 RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        	 Retest_Option_Parm = 1;
        	 if(RetestResponse == 0) SW_TEST_CASE = SW_Test_Done;
        	 else if(RetestResponse == 1) SW_TEST_CASE = SW_Test_Init; 
        	 else 
        	 	ReturnVal = CONTINUE_SERVICE;  
        break;             
        
    	case SW_Test_Fail:
//-    		clr_lcd_dply();
    		Disply_Fault();
        	SW_TEST_CASE = Retest_option;
        break;

    	default:
        	SW_TEST_CASE=SW_Test_Init;
        	ReturnVal = DEFAULT;
        break;
    }	
    
    return ReturnVal; // Required for Retest Option	
}








































