//  Verify_1121_in.c

#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "Expected_Pattern.h"
#include <string.h>

void CPLD_Details(void);
void Fill_Expected_Buffer( void *exp_FPGA_1, void *exp_FPGA_2);
int16 j_wbg, j_wbg2 =  0;
int16 Diff_Array_1[10];
int16 Diff_Array_2[10];

static int16 DebugMsg = 0;  //  set to 1 to log events
int16 Line = 0;
int16 CPLD_1_Buff[4];
int16 CPLD_2_Buff[4];
int16 Expected_FPGA_1[4];
int16 Expected_FPGA_2[4];
int16 Optional_Display = FALSE;
int16 Debug_A_wbg = 0;
int16 Debug_B_wbg = 0;
int16 Debug_C_wbg = 0;
int16 Input_Test = FALSE;

enum
{
    Init_Test,
    All_Input_Should_B_0,
    Wait_for_Operator_Response,
    Set_1109AN_out_x,
    Set_Required_Outputs,
    Set_1106AN,
    Send_msg_2,
    Set_bits_4_CPLDx,
    Check_4_Signal_Present,
    Check_4_Expected_Pattern,
    Check_4_No_Short,
    Test_Pass, 
    Test_Fail, 
    Display_The_Buff,
    PauseTest,
    Retest_option, 
    Test_Almost_Done,
    Wait_State,
    Test_Done,
    Track_Bug
};

void *Comm_Addr[] = 
{
    NULL,
    &i1spi_in,
    &iospi_in,
    NULL,
    NULL,        
};

int8 Last_Item_Array[] =
{
   0,
   63,
   63,
   0,
   0   
};
     
int16 Verify_Input_Simple(int16 Param_and_which_input)
{
    static int16 Verify_1121_TestCase = Init_Test;
    static int16 Counter = 0;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option  
    static int16 NextCaseAfterWait = 0;
    static int16 Pause = 0;
    static int16 Com_to_use = 0;
    static int16 Channel_Bit = 0;
    static int16 Which_Input = 0;
    static int16 k = 0;
    int16 Need_Pwr = Param_and_which_input & 0x8000;   
    int16 i=0;
    int16 j=0;
    int16 R=0;
    int16 A=0;
    int16 Track=0;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED

    switch (Verify_1121_TestCase) {
    case Init_Test:
        clr_lcd_dply();
        Clear_Both_Boards(0);
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        sprintf(src,"param=%d",Param_and_which_input);
  //Add_msg_to_array_B(src, 8);
        Which_Input = Param_and_which_input & 0x00ff;
        sprintf(src,"which input=%d",Which_Input);
  //Add_msg_to_array_B(src, 9);
        DebugMsg = 0;
//        Add_msg_to_array_B("Init", 0);
        if(Need_Pwr == 0)Input_Test = TRUE; //  No need for CST & TSD for an input test.
        Preliminary_120VAC(6); // Sets 120VAC to CST        
        Input_Test = FALSE; // reset now so as not to forget.
//            sprintf(src,"cm,whch=%d,%d",Com_to_use,Which_Input);
//            Log_an_event(src);               
        NextCaseAfterWait = Set_Required_Outputs;  
        Delay_Count = 3; 
        timers[t_Generic_Delay] = 0;  
        Verify_1121_TestCase = Wait_State; 
               
        break; 
        
    case Wait_State:        
        if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms Delay_Count
            timers[t_Generic_Delay] = 0; //
            Verify_1121_TestCase = NextCaseAfterWait;
        }
        break;                

    case Set_Required_Outputs:   
// tagZYM 111 
// tagZYM 111A    
        Set_FPGA_bits_B(Which_Input + 0x0f00); // turns bits ON 
        if( Optional_Display )
        {
            Verify_1121_TestCase = Wait_for_Operator_Response;           
        }
        else
        {
            NextCaseAfterWait = Check_4_Expected_Pattern;
            Delay_Count = 20; // 2s   
            timers[t_Generic_Delay] = 0;  
            Verify_1121_TestCase = Wait_State;                     
        }        
        break;
        
   case Wait_for_Operator_Response:
        if(Check_for_Enter_Button() ) 
        {
            Verify_1121_TestCase = Check_4_Expected_Pattern;
            clr_lcd_dply();
        }        
        break; 
                
    case Check_4_Expected_Pattern: // see that the signal is indicated by looking at the       
        // bit on FPGA 2
        //Verify_1121_TestCase = Check_4_No_Short;
        Line = 1;
//        Add_msg_to_array_B("check 4 Pattern", 3);
        sprintf(src,"which input=%d",Which_Input);
//        Add_msg_to_array_B(src, 10);

        _disable();
		for (i=0;i<4;i++)
		{
            CPLD_1_Buff[i] = i1spi_in.buff[i];
            CPLD_2_Buff[i] = iospi_in.buff[i];                          				
		}   
		_enable();
		
        ClearFaults(0); // clear msg board A for receiving new msgs
        if(  (consB[22] & 0x80) ) 
        {
            CPLD_Details(); // depending on consB file, show CPLD details now
            // or show after determining Pass/Fail.
        }
// tagZYM 111 
// tagZYM 111C       
        j = Verrify_Pattern(Which_Input);
        if(j == 1) Verify_1121_TestCase = Test_Pass;
            else Verify_1121_TestCase = Test_Fail;
        Set_FPGA_bits_B(Which_Input); // turns bits OFF 
        if(  (consB[22] & 0x80) == 0 ) // either show the details before or after the problems.
        {
            CPLD_Details();
        }                                
        break;
        
   case PauseTest:
        if(Check_for_Mode_Button() ) 
        {
            Verify_1121_TestCase = NextCaseAfterWait;
            Pause = 0;
        }        
        break;        

   case Test_Pass:
//        Add_msg_to_array_B("pass", 4);
        CurrentTestPassFail = 1; // pass
        New_write_display("   PASS             ",1,0);
//        Verify_1121_TestCase = Init_Test;
//        return DONE_BUT_CONTINUE_DISPLAY;
        if(Pause)
        {
           NextCaseAfterWait = Test_Almost_Done;  
           Verify_1121_TestCase = PauseTest; 
        }
        else Verify_1121_TestCase = Test_Almost_Done;  // Test_Almost_Done;        
        break;
        
    case Test_Done:
//      Set_FPGA_bits_B(Which_Input); // turns bits OFF
//      if(Functional_1121_Output_Test == 0)  Set_iospi_Ctrl_flag(0); // disable 
        Verify_1121_TestCase = Init_Test;
        Optional_Display = FALSE;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
//        Add_msg_to_array_B("done", 6);              
        break;                        
        
    case Test_Almost_Done:
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED        
//      Set_FPGA_bits_B(Which_Input); // turns bits OFF 
        if(Functional_1121_Output_Test == 0)
        {
            for(i=0; i<12; i++)
            {
              clroutp(Virtual_Board_B_OUT[i]); // Turn off outputs on 1106AN, bd 4 
            }
//          Set_All_outputs(0) ; // off CPLD2
//          Force_CPLD1(0); // off  
            Prelim_Done = FALSE;         
        }
        timers[t_Generic_Delay] = 0; 
//        NextCaseAfterWait =  Test_Done;    
//        Verify_1121_TestCase = Wait_State; 
        Verify_1121_TestCase = Test_Done;
  
        break;
        
     case Retest_option: // Required for Retest Option
// return 0 for do not retest.
// return 1 for try again.
// return 2 for waiting for timeout.
// return 3 for waiting for operator.     
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) Verify_1121_TestCase = Test_Almost_Done;  // Test_Almost_Done;
        else if(RetestResponse == 1) Verify_1121_TestCase = Init_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;    

    case Test_Fail:
//        Add_msg_to_array_B("fail", 5);
        New_write_display("ICxxx               ",0,0);
        New_write_display("      FAILED        ",1,0);
        
        if(Pause)
        {
           NextCaseAfterWait =  Retest_option;  
           Verify_1121_TestCase = PauseTest; 
        }        
        else 
        { // Display unpk data			           
            Verify_1121_TestCase =  Display_The_Buff; // Retest_option;  // Display_The_Buff;    
        }
        break;

    case Display_The_Buff:                   
            ReturnVal = DisplayMsgBrd_A(0);
            if(ReturnVal == CONTINUE_SERVICE) break;
            else
            {
                Verify_1121_TestCase = Retest_option;
                ReturnVal = CONTINUE_SERVICE;                
            }                                     
        break;         

    default:
        Verify_1121_TestCase=Init_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }
    return ReturnVal; // Required for Retest Option      
}

////////////////////////////////////////////////////////     
int16 Test_ixspi_in_bit(int16 i, int16 Com_to_use)
{
  int8 buff = 0;
  int16 j = 0;
  int16 k = 0;
  
    if(DebugMsg)
    {
        sprintf(src,"com,i=%d,%d",Com_to_use,i);
        Log_an_event(src); 
    }
    
  if(i < 16)
  {
    buff = 0; 
    j = 0x8000 >> i;
  }
      else if(i < 32)   
          {
            buff = 1; 
            j = 0x8000 >> (i-16);
          }
          else if(i < 48) 
              {
                buff = 2; 
                j = 0x8000 >> (i-32);
              }          
              else 
              {
                buff = 3; 
                j = 0x8000 >> (i-48);
              }            
  k = Test_a_spi_bit(buff, j, Com_to_use); // i1spi.buff[buff] & j;
            if(DebugMsg)
            {
                sprintf(src,"k=%d",k);
                Add_msg_to_array_B(src, 10);
                Log_an_event(src); 
                sprintf(src,"i=%d",i);
                Add_msg_to_array_B(src, 11);
                Log_an_event(src); 
                sprintf(src,"buff=%d",buff);
                Add_msg_to_array_B(src, 12);
                Log_an_event(src);                         
                sprintf(src,"j=%x",j);
                Add_msg_to_array_B(src, 13);
                Log_an_event(src);            
            }                  
  return k;                    
}

int16 Test_a_spi_bit(int16 buff, int16 j, int16 m)
{
  int16 i = 0;
    switch (m)
    {
        case 1: // FPGA 1
            i = (short)(i1spi_in.buff[buff] & j);
            break;
            
        case 2:  // FPGA 2
            i = (short)(iospi_in.buff[buff] & j);
            if(DebugMsg)
            {
                sprintf(src,"i,buff=%d,%d",i,buff);
                Add_msg_to_array_B(src, 14);
                Log_an_event(src);
                sprintf(src,"Line,j=%d,%x",Line,j);
                Add_msg_to_array_B(src, 15);
                Log_an_event(src);              
            }         
            break;
                        
        case 3: // spi - NTS
            i = 1;
            break;
            
        case 4: // CAN - NTS
            i = 1;
            break;            
               
        default:
            i = 1;
            break;                
    }
     if(i == 0) return 0; else return 1;    
}

int16 DebugHelper(int16 dummy)
{
    int16 dummy2;

    dummy2 = dummy;
        sprintf(src,"A,B,C,D=%d,%d,%d,%d",LogIndex, 
          Msg_Board_index, Displayed_msg_page, 
            Max_Displayed_msg_page);
        Add_msg_to_array_B(src, 30);

    return FINISHED_WITH_SERVICE;    
}

int16 Verrify_Pattern(int16 Which_Input)
{  
  int16 test = 0;
  int16 j = 0;
  int16 Pass = 1;  // assume pass

  test = Which_Input;
   
  sprintf(src,"which input=%d",Which_Input);
  Add_msg_to_array_B(src, 11);
  
  sprintf(src,"test=%d",test);
  Add_msg_to_array_B(src, 12);

  switch(Which_Input)
      { 
        case iDN:
//            Fill_Expected_Buffer( &Exp_iDN_FPGA_1[0],  &Exp_iDN_FPGA_2[0]);
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iDN_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iDN_FPGA_2[j];                   
            }      
            break;
            
        case iDT:
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iDT_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iDT_FPGA_2[j];                   
            }         
            break;
                        
        case iUN:
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iUN_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iUN_FPGA_2[j];                   
            } 
            break;
            
        case iUT: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iUT_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iUT_FPGA_2[j];                   
            }                  
            break; 

        case iSD:
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iSD_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iSD_FPGA_2[j];                   
            }        
            break;
            
        case iSDF:
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iSDF_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iSDF_FPGA_2[j];                   
            }          
            break;
                        
        case iCEN:
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iCEN_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iCEN_FPGA_2[j];                   
            }       
            break;
            
        case iL120:
            break;                       
                           
        case iMC:  
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iMC_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iMC_FPGA_2[j];                   
            }                
            break;                       
                        
        case iMRSW:
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iMRSW_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iMRSW_FPGA_2[j];                   
            }                     
            break;
            
        case iSUF:  
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iSUF_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iSUF_FPGA_2[j];                   
            }                          
            break; 

        case iSU: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iSU_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iSU_FPGA_2[j];                   
            }                            
            break;
///////////////////////////            
        case iGBP: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iGBP_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iGBP_FPGA_2[j];                   
            }                            
            break;    
            
        case iGLB: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iGLB_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iGLB_FPGA_2[j];                   
            }                            
            break;     

        case iLBP: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iLBP_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iLBP_FPGA_2[j];                   
            }                            
            break;    
            
        case iIND: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iIND_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iIND_FPGA_2[j];                   
            }                            
            break;               

        case iAD : 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iAD_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iAD_FPGA_2[j];                   
            }                            
            break;    
            
        case iAUTO: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iAUTO_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iAUTO_FPGA_2[j];                   
            }                            
            break; 
            
        case iMRIN: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iMRIN_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iMRIN_FPGA_2[j];                   
            }                            
            break;              
            
        case iMRID: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iMRID_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iMRID_FPGA_2[j];                   
            }                            
            break; 
            
        case iMRIE: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iMRIE_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iMRIE_FPGA_2[j];                   
            }                            
            break;    
            
        case iMRIU: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iMRIU_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iMRIU_FPGA_2[j];                   
            }                            
            break;    
            
        case iTSD: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iTSD_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iTSD_FPGA_2[j];                   
            }                            
            break;  
            
        case kDLT: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kDLT_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kDLT_FPGA_2[j];                   
            }                            
            break;   
            
        case kRLM: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kRLM_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kRLM_FPGA_2[j];                   
            }                            
            break;   
            
        case kDLM: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kDLM_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kDLM_FPGA_2[j];                   
            }                            
            break;           
            
        case kDLB: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kDLB_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kDLB_FPGA_2[j];                   
            } 
// tagZYM 111 
// tagZYM 111D            
/*              
               Rslt = Check_Input_Shorted(); 
               if(Rslt)
               {
                  Display_First_Failure(Rslt)
               }
*/               
            break;    
            
        case kLPS: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kLPS_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kLPS_FPGA_2[j];                   
            }                            
            break; 
              
        case kLOS: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kLOS_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kLOS_FPGA_2[j];                   
            }                            
            break;   
            
        case kTPL: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kTPL_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kTPL_FPGA_2[j];                   
            }                            
            break;           
            
        case kTPH: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kTPH_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kTPH_FPGA_2[j];                   
            }                            
            break;    

        case kTLH: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kTLH_H21_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kTLH_H21_FPGA_2[j];                   
            }                            
            break; 
                        
        case kGOV: // -H23
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kGOV_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kGOV_FPGA_2[j];                   
            }                            
            break;  

        case kHSS: // -H17
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kHSS_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kHSS_FPGA_2[j];                   
            }                            
            break;  

            
        case kPS: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kPS_H19_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kPS_H19_FPGA_2[j];                   
            }                            
            break; 
                        
        case kS10_H24: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_kS10_H24_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_kS10_H24_FPGA_2[j];                   
            }                            
            break;  
            
        case iBAU: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iBAU_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iBAU_FPGA_2[j];                   
            }                            
            break;  
            
        case iBAD: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iBAD_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iBAD_FPGA_2[j];                   
            }                            
            break;
            
        case iTAU: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iTAU_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iTAU_FPGA_2[j];                   
            }                            
            break;
            
        case iTAD: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iTAD_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iTAD_FPGA_2[j];                   
            }                            
            break;      
/*            
        case iCST: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iCST_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iCST_FPGA_2[j];                   
            }                            
            break; 
  */          
        case iCFLT: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iCFLT_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iCFLT_FPGA_2[j];                   
            }                            
            break;  
            
        case iSPD: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iSPD_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iSPD_FPGA_2[j];                   
            }                            
            break;   
            
        case iMTO: 
            for(j=0; j<4; j++)
            {
                Expected_FPGA_1[j] = Exp_iMTO_FPGA_1[j];
                Expected_FPGA_2[j] = Exp_iMTO_FPGA_2[j];                   
            }                            
            break;                                                                          
//  WBG step 04

 ////////////////////////                       
        case iDZ:
            break;
                                          
        default:
            break;                
    }     
//////////////////////////////////////
    for(j=0; j<4; j++)
    {
        if(CPLD_1_Buff[j] != Expected_FPGA_1[j]) 
        {
          Pass = Display_Problem(CPLD_1_Buff[j] , Expected_FPGA_1[j], j, 1, 0 );
          if (zym_diagnostic_mode == 0) break;  // bail out on first error   
          // unless we are doing diagnostics as per zym.
        }                  
    }
    for(j=0; j<4; j++)
    {
        if(CPLD_2_Buff[j]  != Expected_FPGA_2[j])
        {
          Pass = Display_Problem(CPLD_2_Buff[j] , Expected_FPGA_2[j], j, 2, 0);
          if (zym_diagnostic_mode == 0) break;  // bail out on first error   
          // unless we are doing diagnostics as per zym.
        }                  
    }                                    
    if(Pass == 1) 
    {
    //  Add_msg_to_array_B("DN Passed", 13);
      return 1;     
    }                    
    else
    {
    //  Add_msg_to_array_B("DN Failed", 13);
      return 0;    
    }         
    return Pass;
}
// wbg123
int16 Display_Problem(int16 unpk_wrd, int16 exp_wrd, int16 J, int16 FPGA, int16 offset)
{ // Display_Problem(i1spi_in.buff[j], Exp_iDN_FPGA_1[j], j, 1, 0)
    int16 i = 0;
    int16 A = 0;
    int16 B = 0;
    int16 k = 0;
    int16 M = 0;  
    
    if(FPGA == 2) k = 4;
    sprintf(src,"  0x%4.4X, 0x%4.4X",unpk_wrd & 0xFFFF, exp_wrd & 0xFFFF );
    Add_msg_to_array_B(src, k+J);    
    
    k = 1; // new role for 'k'    
    for(i=0; i<16; i++)
    { // look at each bit of the 16-bit word, starting from the right. 
      // If the bits in A and B do not
      // agree then print short or open.       
        A = unpk_wrd  & k;
        B = exp_wrd   & k;
        k = k << 1; 

        switch (FPGA)
        {
            case 1:
                if(A != B)
                {   if(M++ >2 & zym_diagnostic_mode == 0) break; //  continue
                      // if we are doing special zym diagnostics do not break.
                    if (zym_diagnostic_mode)
                    {
                       if (j_wbg < 10) Diff_Array_1[j_wbg++] = offset + (15 - i) ;
                       sprintf(src,"[%d,%d,%d,%d] %s",FPGA,J, i, offset, FPGA_1_char[offset + (15 - i) ] );
                       Log_an_event(src);
                    }
                    else{
                        if(A) sprintf(src,"(%d,%d,%d,%d)shrt %s",FPGA,J, i, offset, FPGA_1_char[offset + (15 - i) ] );
                        else  sprintf(src,"(%d,%d,%d,%d)opn %s", FPGA,J, i, offset, FPGA_1_char[offset + (15 - i) ] ); 
                        Log_an_event(src);
                         
                        sprintf(src,"%s", FPGA_1_Look_At[offset + (15 - i) ] ); 
                        Log_an_event(src);
                        if(strlen(FPGA_1_Look_At_Additional[offset + (15 - i) ] ) > 0)
                        {
                            sprintf(src,"%s", FPGA_1_Look_At_Additional[offset + (15 - i) ] ); 
                            Log_an_event(src);
                        }   
                    }                                    
                }            
                break;
                
            case 2:
                if(A != B)
                {   if(M++ >2 & zym_diagnostic_mode == 0) break; //  continue
                      // if we are doing special zym diagnostics
                    if (zym_diagnostic_mode)
                    {
                       if (j_wbg2 < 10) Diff_Array_2[j_wbg2++] = offset + (15 - i) ;
                       sprintf(src,"{%d,%d,%d,%d} %s",FPGA,J, i, offset, FPGA_2_char[offset + (15 - i) ] );
                       Log_an_event(src);                        
                    }
                    else{                    
                        if(A) sprintf(src,"(%d,%d,%d,%d)shrt %s",FPGA,J, i,offset, FPGA_2_char[offset + (15 - i) ] );
                        else  sprintf(src,"(%d,%d,%d,%d)opn %s", FPGA,J, i,offset, FPGA_2_char[offset + (15 - i) ] ); 
                        Log_an_event(src);
                         
                        sprintf(src,"%s", FPGA_2_Look_At[offset + (15 - i) ] ); 
                        Log_an_event(src);
                        if(strlen(FPGA_2_Look_At_Additional[offset + (15 - i) ] ) > 0)
                        {
                            sprintf(src,"%s", FPGA_2_Look_At_Additional[offset + (15 - i) ] ); 
                            Log_an_event(src);
                        }    
                    }                                   
                }                        
                break;            
                   
            default:
                break;                
        }        
    }
    return 0;
}

void CPLD_Details(void)
{
    int16 i=0;
    
	for (i=0;i<4;i++)
	{
	    sprintf(src,"i1[%d]=0x%X", i, CPLD_1_Buff[i] & 0xffff );
        Log_an_event(src);                                    				
	}  
	for (i=0;i<4;i++)
	{
	    sprintf(src,"io[%d]=0x%X", i, CPLD_2_Buff[i] & 0xffff );
        Log_an_event(src);                                    				
	}     
}

int16 Find_elusive_msg(int16 position)
{
    New_write_display("                    ",0,0);
    sprintf(src,"%d",position);
    New_write_display(src,0,0);
    New_write_display("Mode to quit.       ",2,0);
    New_write_display("Enter to go on.     ",3,0);
    if(Check_for_Enter_Button() ) 
    {
        return 1;
    } 
    if(Check_for_Mode_Button() ) 
    {
        return 2;
    }    
  return 0;    
}


int16 Build_up_Diff_Array(int16 For_which_item) // Currently, For_which_item is not used.
{   // Determines which bits are changed between on/off and uses this to
    // fill an array of items to monitor (up to 10 max)
    static int16 a = 0;
    static int16 ReturnVal = CONTINUE_SERVICE;
    
    if (zym_diagnostic_mode == 1)    
    {
        for(j_wbg = 0; j_wbg < 10; j_wbg++)
        {
            Diff_Array_1[j_wbg] = -1;
            Diff_Array_2[j_wbg] = -1;
        }
        //Log_an_event_C(src);
        j_wbg = j_wbg2 = 0;

//        Verrify_Pattern(For_which_item);
        // builds up an array that will be used to display up to 
        // ten items onto msg board C.
    }

    return ReturnVal; 
        // FINISHED_WITH_SERVICE; DONE_BUT_CONTINUE_DISPLAY; 
        // CONTINUE_SERVICE;            
} // END OF Build_up_Diff_Array()


int16 Add_Msg_to_C_for_zym_diag(int16 dummy)
{   // adds text to msg_board C
    static int16 a = 0;
    static int16 ReturnVal = CONTINUE_SERVICE;
    int16 i, j, k, jk = 0;
        // FINISHED_WITH_SERVICE; DONE_BUT_CONTINUE_DISPLAY; 
        // CONTINUE_SERVICE;
        // Clear the msg_board and then fill from the Disabled_unpack_1 
        // array.
             
    // fill up a char buffer with contents of Disabled_unpack_1,
    // use a max of 4 lines in msg bd C.  Then do the same for
    // Disabled_unpack_2 on the next 4 lines.  Use string concatenation
    // functions.
    
    sprintf(src,""); // start with empty string buffer.
    sprintf(src_B,"");
    sprintf(src_C,"");
    for ( i = 0; i < 10; ++i)
    { // first do FPGA 1
        j = Diff_Array_1[i];
        if(j < 0) break;
        k = Get_Bit_Status(1, j);  // see if bit is 0 or 1.  If 0 set
        // text to lower-case; if 1 set to upper-case.        
//        sprintf(src,"1)%s%d]%d,%d,%x,%d", FPGA_1_char[j], k,Debug_A_wbg, Debug_B_wbg, Debug_C_wbg,j );
        sprintf(src,"(1) %s [%d]", FPGA_1_char[j], k );
        
        Log_an_event_C(src);
/*        
        k = Get_Bit_Status(1, j);  // see if bit is 0 or 1.  If 0 set
        // text to lower-case; if 1 set to upper-case.
        if(k) sprintf(src_B,"(1)%s ", i, j, FPGA_1_char[i] );
        else 
        {
            sprintf(src_C,"%s",FPGA_1_char[i]);
            strlwr(src_C);
            sprintf(src_B,"(1)%s ", i, j, src_C ) ;   
        } 
        strncat(src,src_B ,9);  // concat src and src_B
        if(i%2 == 0){ // if i is even, print the line else add another item.
            Log_an_event_C(src);
            sprintf(src,""); // erase buffer
        }
*/        
    }
    //if(strlen(src) > 1) Log_an_event_C(src);
   /// sprintf(src,""); // start with empty string buffer.
// now do FPGA 2
    for ( i = 0; i < 10; ++i)
    { // now do FPGA 2
        j = Diff_Array_2[i];
        if(j < 0) break;
        k = Get_Bit_Status(2, j);  // see if bit is 0 or 1.  If 0 set
        // text to lower-case; if 1 set to upper-case.
//        sprintf(src,"2)%s%d]%d,%d,%x,%d", FPGA_2_char[j], k,Debug_A_wbg, Debug_B_wbg, Debug_C_wbg,j);
        sprintf(src,"(2) %s [%d]", FPGA_2_char[j], k );
        Log_an_event_C(src);
/*        
        if(k) sprintf(src_B,"(2)%s ", i, j, FPGA_2_char[i] );
        else 
        {
            sprintf(src_C,"%s",FPGA_2_char[i]);
            strlwr(src_C);
            sprintf(src_B,"(1)%s ", i, j, src_C ) ;   
        }  
        strncat(src,src_B ,9);  // concat src and src_B
        if(i%2 == 0){ // if i is even, print the line else add another item.
            Log_an_event_C(src);
            sprintf(src,""); // erase buffer
        }
*/        
    }
    //if(strlen(src) > 1) Log_an_event_C(src);
    //sprintf(src,""); // start with empty string buffer.


/*    Log_an_event_C(src); // write the first of possible 4 lines.
    sprintf(src,""); // start with empty string buffer.
    for ( i = 3; i < 6; ++i)
    {
        j = Disabled_unpack_1[i];
        if (j < 0) break;
        if(strlen(src) < 13) strncat(src, FPGA_1_char[j],5);
    }
    Log_an_event_C(src); // write the next line of possible 4 lines.
    sprintf(src,""); // start with empty string buffer.
    for ( i = 6; i < 10; ++i)
    {
        j = Disabled_unpack_1[i];
        if (j < 0) break;
        if(strlen(src) < 13) strncat(src, FPGA_1_char[j],5) ;
    }
    Log_an_event_C(src); // write the next line of possible 4 lines.

    // Now do 4 lines for FPGA 2

    sprintf(src,""); // start with empty string buffer.
    for ( i = 0; i < 3; ++i)
    {
        j = Disabled_unpack_2[i];
        if (j < 0) break;
        if(strlen(src) < 13) strncat(src, FPGA_2_char[j],5);
    }
    Log_an_event_C(src); // write the first of possible 4 lines.
    sprintf(src,""); // start with empty string buffer.
    for ( i = 3; i < 6; ++i)
    {
        j = Disabled_unpack_2[i];
        if (j < 0) break;
        if(strlen(src) < 13) strncat(src, FPGA_2_char[j],5);
    }
    Log_an_event_C(src); // write the next line of possible 4 lines.
    sprintf(src,""); // start with empty string buffer.
    for ( i = 6; i < 10; ++i)
    {
        j = Disabled_unpack_2[i];
        if (j < 0) break;
        if(strlen(src) < 13) strncat(src, FPGA_2_char[j],5);
    }
    Log_an_event_C(src); // write the next line of possible 4 lines.
*/
/*    sprintf(src,"test msg C");
    Log_an_event_C(src);
    Log_an_event_C(src);
    Log_an_event(src);
    Add_msg_to_array_B(src,0);
    Add_msg_to_array_B(src,1);    
*/
    return ReturnVal; 
        // FINISHED_WITH_SERVICE; DONE_BUT_CONTINUE_DISPLAY; 
        // CONTINUE_SERVICE;            
} // END OF Add_Msg_to_C_for_zym_diag()

/*
void Fill_Expected_Buffer(void *exp_FPGA_1, void *exp_FPGA_2)
{
    int16 j = 0;
    int16 s = 0;
    
    s = exp_FPGA_1[0];
    for(j=0; j<4; j++)
    {
        Expected_FPGA_1[j] = (Exp_iDT_FPGA_1)exp_FPGA_1[j];
        Expected_FPGA_2[j] = exp_FPGA_2[j];                   
    }     
}
*/