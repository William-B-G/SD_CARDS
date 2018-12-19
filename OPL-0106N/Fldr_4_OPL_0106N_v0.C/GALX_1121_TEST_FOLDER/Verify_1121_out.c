//  Verify_1121_out.c


#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"

enum
{
    VIZ_Init_Test,
    VIZ_Case01,
    VIZ_Case02,
    VIZ_Case03,
    VIZ_Case04,
    VIZ_Case05_Pass,
    VIZ_Case06_Fail,
    VIZ_Test_Done,
    VIZ_Wait_State,
    VIZ_Retest_option,   
    VIZ_Wait_for_Operator_Response, 
};

static int16 Delay_Count = 5; // each count = 100ms
static int16 NextCaseAfterWait = 0;
uint8 Prelim_Done = FALSE;
uint8 Functional_1121_Output_Test = FALSE;
/*
For CPLD 1: 0=PRUN_LP; 1=PCEN_LP; 2=PTSE_LP; 3=PMC_LP; (4-7 unused); 8=PSU_LP; 9=PSD_LP; 10=PSUF_LP; 11=PSDF_LP; 12=PUN_LP; 13=PDN_LP; 14=PUT_LP
For CPLD 2: 0=SUF; 1=SU; 2=SDF; 3=SD; 4=STE; 5=CEN; 6=MC; 7=RUN

0 = SUF = Solinoid Up Fast;  1 = SU = Solinoid Up;  2 = SDF = Solinoid Down Fast;  3 = SD = Solinoid Down;  4 = STE = STarter Enable;
5 = CED = Controller Enable/Disable (MC1);  6 = MC = Motor Contactor;  7 = RUN = Run
*/
const struct Output_Verify
{
    int16 Which_CPLD1_bits[12];
    int16 Which_CPLD2_bits[8];
    int16 Which_1106AN_outputs[12];
    int16 Which_1106AN_inputs[12];
    int16 NOT_1106AN_inputs[12];
    int16 Which_NTS_bits[4]; // DT, UT,  DN, UN (in that order)    
} The_Verify_map[]=  // tagwbg 109
    {//                 CPLD 1                        CPLD 2               1106AN OUT (o_4)                   1106AN IN  (i_3)                         NOT 1106AN IN       WHICH NTS BITS
    //                                                                    Virtual_Board_B_OUT               Virtual_Board_A_in
    //                                                             S3TPL,S1TPH,15TSD,17CST,12ICA,S5LPS,24LOS        suf, su, sdf, sd, sdc, s6s10 
    //                                                              23GOV,21TLH,H19PS,17HSS
//  ===  0   0   0   0  =======================================    
/* SUF(0) & UT(4) */ { {14,10,0,-1,0,0,0,0,0,0,0,0}, { 0, 7,-1,0,0,0,0,0 }, { 2,3,-1,0,0,0,0,0,0,0,0,0 }, { 7,-1,0,0,0,0,0,0,0,0,0,0 }, { 0,1,2,3,4,5,6,8,9,10,11,-1 }, { 1,-1,2,3 }    }, // 0
/*
SUF ==> [TG-SUF-L] + [TG-UT-L] + (TG-RUN-L) + {TSD + CST} = 
        [CPLD 1:PSUF_LP + CPLD 2:UPF_CPU] 
          + [CPLD 1:PUT_LP + UT_NTS] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU) 
              + {out1->TSD[2] + out2->CST[3]}
CPLD 1:PSUF_LP => BIT 10 
CPLD 1:PUT_LP  => BIT 14
CPLD 1:PRUN_LP => BIT  0
CPLD 2:UPF_CPU => SUF_output_cmnd_flag:0
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7            
*/
// ======   1   1   1   ======================================
/* SU(1) & UN(5) */ { { 12,8,0,-1,0,0,0,0,0,0,0,0}, { 1,7, -1,0,0,0,0,0 }, { 3, -1,-1,0,0,0,0,0,0,0,0,0 }, { 10,-1,0,0,0,0,0,0,0,0,0,0 }, { 0,1,2,3,4,5,6,7,8,9,11,-1 }, { 3,-1,2,3 }    }, // 1
/*
SU ==> [TG-SU-L] + [TG-UN-L] + (TG-RUN-L) + {CST} = 
        [CPLD 1:PSU_LP + CPLD 2:UPR_CPU] 
          + [CPLD 1:PUN_LP + UN_NTS] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU) 
              + {out2->CST}
CPLD 1:PSU_LP  => BIT  8 
CPLD 1:PUN_LP  => BIT 12
CPLD 1:PRUN_LP => BIT  0
CPLD 2:UPR_CPU => SU_output_cmnd_flag:1
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7            
*/
// ======   2   2   2   ======================================
              //       CPLD 1                        CPLD 2                   1106AN OUT                1106AN IN                         NOT 1106AN IN
/* SDF(2) & DT(6) */ { {15,11,0,-1,0,0,0,0,0,0,0,0}, { 2,7,-1,0,0,0,0,0 },  { 3, -1,-1,0,0,0,0,0,0,0,0,0 }, { 9,-1,0,0,0,0,0,0,0,0,0,0 }, { 0, 1,2,3,4,5,6,7,8,10,11,-1 }, { 0,-1,2,3 }  }, // 2
/*
SDF ==> [TG-SDF-L] + [TG-DT-L] + (TG-RUN-L) + {CST} = 
        [CPLD 1:PSDF_LP + CPLD 2:DNF_CPU] 
          + [CPLD 1:PDT_LP + DT_NTS] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU) 
              + {out2->CST}
CPLD 1:PSDF_LP => BIT 11 
CPLD 1:PDT_LP  => BIT 15
CPLD 1:PRUN_LP => BIT  0
CPLD 2:DNF_CPU => SDF_output_cmnd_flag:2
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7            
*/
// =======   3   3   3   =====================================
              //       CPLD 1                        CPLD 2                   1106AN OUT                1106AN IN                         NOT 1106AN IN
/* SD(3) & DN(7) */ { {13,9,0,-1,0,0,0,0,0,0,0,0}, { 3,7,-1,0,0,0,0,0 },  { 3,-1,0,0,0,0,0,0,0,0,0,0 }, { 8,-1,0,0,0,0,0,0,0,0,0,0 }, { 0, 1,2,3,4,5,6,7,9,10,11,-1 }, { 2,-1,2,3 }  }, // 3
/*
SD ==> [TG-SD-L] + [TG-DN-L] + (TG-RUN-L) + {CST} = 
        [CPLD 1:PSD_LP + CPLD 2:DNR_CPU] 
          + [CPLD 1:PDN_LP + DN_NTS] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU) 
              + {out2->CST}
CPLD 1:PSD_LP  => BIT  9 
CPLD 1:PDN_LP  => BIT  13
CPLD 1:PRUN_LP => BIT  0
CPLD 2:DNF_CPU => SD_output_cmnd_flag:3
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7            
*/
// =======   4   4   4   =====================================
              //       CPLD 1                        CPLD 2                   1106AN OUT                1106AN IN                         NOT 1106AN IN
/* UT(4)    */ { {14,0,-1,-1,0,0,0,0,0,0,0,0}, { 7,-1,0,0,0,0,0,0 },  
                 { 2,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN OUT (2 = TSD; 3 = CST)  
                 { -1,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN IN
                 { 0, 1,2,3,4,5,6,7,8,9,10,11 }, //  NOT 1106AN IN 
                 { 1,-1,2,3 }  // WHICH NTS BITS // 4
               },
/*
UT  ==> [TG-UT-L] + (TG-RUN-L) + {TSD + CST} =          
          + [CPLD 1:PUT_LP + UT_NTS] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU) 
              + {out1->TSD + out2->CST}
CPLD 1:PUT_LP  => BIT 14
CPLD 1:PRUN_LP => BIT  0
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7           
*/

// =======   5   5   5   =====================================
/* UN(5)    */ { {12,0,-1,-1,0,0,0,0,0,0,0,0}, // CPLD 1 
                 { 7,-1,0,0,0,0,0,0 }, // CPLD 2 :0=SUF; 1=SU; 2=SDF; 3=SD; 4=STE; 5=CEN; 6=MC; 7=RUN
                 { 3,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN OUT (2 = TSD; 3 = CST)
                 { -1,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN IN
                 { 0, 1,2,3,4,5,6,7,8,9,10,11 }, // NOT 1106AN IN
                 { 3,-1,2,3 }   // WHICH NTS BITS (// DT, UT,  DN, UN) // 5
               },
/*
UN  ==> [TG-UN-L] + (TG-RUN-L) +  {CST} =          
          + [CPLD 1:PUN_LP + UN_NTS] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU)             
              + {out2->CST}
CPLD 1:PUN_LP  => BIT 12
NTS   :UN      => 3 
CPLD 1:PRUN_LP => BIT  0
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7          
*/               
// =======   6   6   6   =====================================
/* DT(6)    */ { {15,0,-1,-1,0,0,0,0,0,0,0,0}, // CPLD 1  
                 { 7,-1,0,0,0,0,0,0 },  // CPLD 2: 0=SUF; 1=SU; 2=SDF; 3=SD; 4=STE; 5=CEN; 6=MC; 7=RUN
                 { 3,-1,0,0,0,0,0,0,0,0,0,0 },  // 1106AN OUT (2 = TSD; 3 = CST)
                 { -1,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN IN 
                 { 0, 1,2,3,4,5,6,7,8,9,10,11 },// NOT 1106AN IN 
                 { 0,-1,2,3 }   // WHICH NTS BITS (// DT, UT,  DN, UN) // 6
               },
/*
DT  ==> [TG-DT-L] + (TG-RUN-L) +  {CST} =          
          + [CPLD 1:PDT_LP + DT_NTS] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU)             
              + {out2->CST}
CPLD 1:PDT_LP  => BIT 15
NTS   :DT      => 0 
CPLD 1:PRUN_LP => BIT  0
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7          
*/               
// =======   7   7   7   =====================================               
/* DN(7)    */ { {13,0,-1,-1,0,0,0,0,0,0,0,0}, // CPLD 1
                 { 7,-1,0,0,0,0,0,0 },  // CPLD 2: 0=SUF; 1=SU; 2=SDF; 3=SD; 4=STE; 5=CEN; 6=MC; 7=RUN
                 { 3,-1,0,0,0,0,0,0,0,0,0,0 },  // 1106AN OUT (2 = TSD; 3 = CST)
                 { -1,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN IN
                 { 0, 1,2,3,4,5,6,7,8,9,10,11 }, // NOT 1106AN IN 
                 { 2,-1,2,3 } // WHICH NTS BITS (// DT, UT,  DN, UN)  // 7 
               }, 
/*
DN  ==> [TG-DN-L] + (TG-RUN-L) +  {CST} =          
          + [CPLD 1:PDN_LP + DN_NTS] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU)             
              + {out2->CST}
CPLD 1:PDN_LP  => BIT 13
NTS   :DT      => 0 
CPLD 1:PRUN_LP => BIT  0
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7          
*/ 
               
/* RUN(8)   */ { {10,14,0,-1,0,0,0,0,0,0,0,0}, { 0,-1,0,0,0,0,0,0 },  { 2,-1,0,0,0,0,0,0,0,0,0,0 }, { 11,-1,0,0,0,0,0,0,0,0,0,0 }, { 0, 1,2,3,4,5,6,7,8,9,10,-1 }, { 0,1,2,3 }  }, // 8

// =======   9    9   9   =====================================
/* MC(9)    */ { {3,0,-1,-1,0,0,0,0,0,0,0,0}, // CPLD 1
                 { 6,7,-1,0,0,0,0,0 },  // CPLD 2: 0=SUF; 1=SU; 2=SDF; 3=SD; 4=STE; 5=CEN; 6=MC; 7=RUN
                 { 3,-1,0,0,0,0,0,0,0,0,0,0 },  // 1106AN OUT (2 = TSD; 3 = CST)
                 { -1,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN IN
                 { 0, 1,2,3,4,5,6,7,8,9,10,11 }, // NOT 1106AN IN 
                 { -1,-1,2,3 } // WHICH NTS BITS (// DT, UT,  DN, UN)  // 9
               },
/*
MC  ==> [TG-MC-L] + (TG-RUN-L) +  {CST} =          
          + [CPLD 1:PMC_LP + CPLD 2:MC_CPU] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU)             
              + {out2->CST}
CPLD 1:PMC_LP  => BIT 3
CPLD 1:PRUN_LP => BIT  0
CPLD 2:MC_CPU  => 6
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7          
*/
// =======   10   10  10  =====================================
/* CEN(10)  */ { {1, 0,-1,-1,0,0,0,0,0,0,0,0}, 
                 { 5,7,-1,0,0,0,0,0 },  // CPLD 2: 0=SUF; 1=SU; 2=SDF; 3=SD; 4=STE; 5=CEN; 6=MC; 7=RUN
                 { 3,-1,0,0,0,0,0,0,0,0,0,0 },  // 1106AN OUT (2 = TSD; 3 = CST)
                 { -1,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN IN
                 { 0, 1,2,3,4,5,6,7,8,9,10,11 }, // NOT 1106AN IN 
                 { -1,-1,2,3 } // WHICH NTS BITS (// DT, UT,  DN, UN)  // 10
               },
/*
CEN  ==> [TG-MC1-L] + (TG-RUN-L) +  {CST} =          
          + [CPLD 1:PMC1_LP + CPLD 2:MC1_CPU] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU)             
              + {out2->CST}
CPLD 1:PMC1_LP  => BIT 1
CPLD 1:PRUN_LP => BIT  0
CPLD 2:MC1_CPU  => 5
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7          
*/               
// =======   11   11  11  =====================================
/* STE(11)  */ { {2,0,-1,-1,0,0,0,0,0,0,0,0}, 
                 { 4,7,-1,0,0,0,0,0 },  // CPLD 2: 0=SUF; 1=SU; 2=SDF; 3=SD; 4=STE; 5=CEN; 6=MC; 7=RUN
                 { 3,-1,0,0,0,0,0,0,0,0,0,0 },  // 1106AN OUT (2 = TSD; 3 = CST)
                 { -1,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN IN
                 { 0, 1,2,3,4,5,6,7,8,9,10,11 }, // NOT 1106AN IN 
                 { -1,-1,2,3 } // WHICH NTS BITS (// DT, UT,  DN, UN)  // 11
               },
/*
STE  ==> [TG-STE-L] + (TG-RUN-L) +  {CST} =          
          + [CPLD 1:PSTE_LP + CPLD 2:STE_CPU] 
            + (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU)             
              + {out2->CST}
CPLD 1:PSTE_LP  => BIT 2
CPLD 1:PRUN_LP => BIT  0
CPLD 2:STE_CPU  => 4
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7          
*/           
// =======   12   12  12  =====================================                 
/* MRSW(12) */ { {0,-1,0,-1,0,0,0,0,0,0,0,0}, 
                 { 7,-1,-1,0,0,0,0,0 },  // CPLD 2: 0=SUF; 1=SU; 2=SDF; 3=SD; 4=STE; 5=CEN; 6=MC; 7=RUN
                 { 3,-1,0,0,0,0,0,0,0,0,0,0 },  // 1106AN OUT (2 = TSD; 3 = CST)
                 { -1,-1,0,0,0,0,0,0,0,0,0,0 }, // 1106AN IN
                 { 0, 1,2,3,4,5,6,7,8,9,10,11 }, // NOT 1106AN IN 
                 { -1,-1,2,3 } // WHICH NTS BITS (// DT, UT,  DN, UN)  // 12
               },               
/*
MRSW  ==> (TG-RUN-L) + {CST} = (CPLD 1:PRUN_LP + CPLD 2:RUN_CPU) + {out2->CST}
CPLD 1:PRUN_LP => BIT  0
CPLD 2:RUN_CPU => RUN_output_cmnd_flag:7          
*/ 
    };


enum
{
    Init_Test,
    All_Input_Should_B_0,
    Wait_for_Operator_Response,
    Set_1109AN_out_x,
    Set_CST_120VAC,
    Set_1106AN,
    Send_msg_2,
    Set_bits_4_CPLDx,
    Check_4_Signal_Present,
    Check_4_No_Short,
    Test_Pass, 
    Test_Fail, 
    PauseTest,
    Retest_option, 
    Test_Almost_Done,
    Wait_State,
    Test_Done,
};

enum
{
    SUF_S14,
};

/*
PRELIMINARY - Turn on CST, TSD, TG_RUN_L to apply 120VAC through 1106AN # 4
*/

int16 Preliminary_120VAC(int16 Channel)
{
  uint8 i,k = 0;
  static TheLastChannel = -1;
  
        if(Channel == -1) return FINISHED_WITH_SERVICE;
        if(Prelim_Done && (TheLastChannel == Channel )) return FINISHED_WITH_SERVICE;
        // else
        TheLastChannel = Channel;
        Set_iospi_Ctrl_flag(1); // enable
        Force_CPLD1(0); // turn off all bits for CPLD1
        Set_All_outputs(0); // turn off all bits for CPLD2        
        k = 0;
        for(i=0; i<12; i++)
        {
          clroutp(Virtual_Board_B_OUT[i]); // Turn off outputs on 1106AN, bd 4 
        }
        UseFPGA1_B0 = 1; // Allow multiple bits to be set.
        if(Input_Test == FALSE)
        { // no need for CST and TSD if this is a simple input test.
            CST_ctrl(0); // apply 120VAC to CST via out2
            TSD_ctrl(Channel); // apply 120VAC to TSD via out1 if Channel < 5            
        }
        Prelim_Done = TRUE; 
        return FINISHED_WITH_SERVICE;
}

int16 TSD_ON_OFF(int16 On_or_Off)
{
    if(On_or_Off == 1) setoutp(Virtual_Board_B_OUT[2]); else  clroutp (Virtual_Board_B_OUT[2]); 
    return FINISHED_WITH_SERVICE;
}

int16 Verify_1121_OUT(int16 TheLoop)
{
/*
    Set appropriate signals in CPLD1 and CPLD2.
    Display msg to set switches for MR CAN.
    After short delay (1/2s), set appropriate 1106AN expander board output (addr 4).
    Display msg to turn on 120VAC.
    
    After another 1/2s delay, read the 1106AN expander board input (addr 3) and verify
    that the 120VAC is present. Verify that all the other (11) 1106AN inputs are at zero
    to confirm that there is no short.

*/
    static int16 Verify_1121_TestCase = Init_Test;
    static int16 Counter = 0;
//    static int16 index=0;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option  
    static int16 NextCaseAfterWait = 0;
    int16 Pause = 0;
    static int16 k = 0;
    int16 TheChannel = 0;  
    int16 i=0;
    int16 j=0;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
    Pause = (TheLoop & 0xf000) >> 12;
    TheChannel = TheLoop & 0x0fff;
    
    switch (Verify_1121_TestCase) {
    case Init_Test:
        clr_lcd_dply();
        Clear_Msg_Board();  
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        Preliminary_120VAC(7);  // turn on CST if not already on        
        NextCaseAfterWait =  All_Input_Should_B_0;  
        timers[t_Generic_Delay] = 0;  
        Verify_1121_TestCase = Wait_State;        
        break;         

    case Wait_State:
        if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms Delay_Count
            timers[t_Generic_Delay] = 0; //
            Verify_1121_TestCase = NextCaseAfterWait;
        }
        break;

    case All_Input_Should_B_0:     
        for(i=0; i< 12; i++) 
        {
           j = rdinp(Virtual_Board_A_in[i]); // All inputs should be zero.
           if (j)
           {
              Verify_1121_TestCase = Test_Fail;
              return CONTINUE_SERVICE;
           }            
        }      
 //       Verify_1121_TestCase = Set_1109AN_out_x; 
        Verify_1121_TestCase = Set_CST_120VAC;                       
        break;
        
    case Set_1109AN_out_x:
//        TG_RUN_L_ctrl(0);
//        CST_ctrl(0); // Apply 120 VAC to CST
        New_write_display("Turn on 120 VAC     ",1,0);
        New_write_display("                    ",2,0);
        New_write_display("Press Enter when rdy",3,0);
        NextCaseAfterWait = Set_CST_120VAC;
        Verify_1121_TestCase=Wait_for_Operator_Response;        
        break;        

    case Set_CST_120VAC:
//        UseFPGA1_B0 = 1; // Allow multiple bits to be set.
        Set_FPGA_bits(TheChannel); 
        timers[t_Generic_Delay] = 0;
        NextCaseAfterWait = Set_1106AN;        
        Verify_1121_TestCase=Wait_State;  // to allow 120VAC time to come up.      
        break;         

    case Set_1106AN:
        Set_1106AN_channels(TheChannel);  // set the 1106AN outx channels
          // to apply the 120VAC to the TSD & CST on the 1121 bd.
        timers[t_Generic_Delay] = 0;
        NextCaseAfterWait = Check_4_Signal_Present;        
        Verify_1121_TestCase=Wait_State;  // to allow 120VAC time to come up.      
        break;         

    case Wait_for_Operator_Response:
        if ( Check_for_Enter_Button() )
        {
            New_write_display("                    ",2,0);
            New_write_display("                    ",3,0);        
            Verify_1121_TestCase = NextCaseAfterWait;   
        }
        break;
        
    case Check_4_Signal_Present: // see that the AC voltage is at 1106AN, addr 4
        Verify_1121_TestCase = Check_4_No_Short;
        for(i = 0; i<12; i++)
        {        
            j = The_Verify_map[TheChannel].Which_1106AN_inputs[i];
            if(j == -1) break;
            // now NOT break
            if(!rdinp(Virtual_Board_A_in[j]))
            {
              Verify_1121_TestCase = Test_Fail;
              sprintf(src,"rdinp(bd3)[%d]fail", j) ;
              New_write_display(src,2,0);
              sprintf(src,"TheChannel=%d", TheChannel) ;
              New_write_display(src,3,0);              
              break;   
            }
        }
        break;
        
    case Check_4_No_Short:
        Verify_1121_TestCase = Test_Pass;
        for(i=0; i<12; i++)
        {
           j = The_Verify_map[TheChannel].NOT_1106AN_inputs[i];
           if(j == -1) break;
            // now NOT break
           if(rdinp(Virtual_Board_A_in[j]))
           {
              sprintf(src,"[%d] = %d", j, rdinp(Virtual_Board_A_in[j]));
              Log_an_event(src);
              Verify_1121_TestCase = Test_Fail;              
           }; 
        } 
        if(Verify_1121_TestCase == Test_Fail)
        {
          New_write_display("Short detected -fail",2,0);    
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
        CurrentTestPassFail = 1; // pass
        New_write_display("   PASS             ",1,0);
        if(Pause)
        {
           NextCaseAfterWait =  Test_Almost_Done;  
           Verify_1121_TestCase = PauseTest; 
        }
        else Verify_1121_TestCase = Test_Almost_Done;        
        break;
        
    case Test_Done:
        if(Functional_1121_Output_Test == 0)  Set_iospi_Ctrl_flag(0); // disable 
        Verify_1121_TestCase = Init_Test;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
        break;                        
        
    case Test_Almost_Done:
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
        Clear_FPGA_bits(TheChannel);
        if(Functional_1121_Output_Test == 0)
        {
            for(i=0; i<12; i++)
            {
              clroutp(Virtual_Board_B_OUT[i]); // Turn off outputs on 1106AN, bd 4 
            }
//            Set_All_outputs(0) ; // off CPLD2
//            Force_CPLD1(0); // off  
            Prelim_Done = FALSE;         
        }
        timers[t_Generic_Delay] = 0; 
        NextCaseAfterWait =  Test_Done;    
        Verify_1121_TestCase = Wait_State; 
  
        break;
        
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) Verify_1121_TestCase = Test_Almost_Done;
        else if(RetestResponse == 1) Verify_1121_TestCase = Init_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;    

    case Test_Fail:
        New_write_display("      FAILED        ",1,0);
        if(Pause)
        {
           NextCaseAfterWait =  Retest_option;  
           Verify_1121_TestCase = PauseTest; 
        }        
        else Verify_1121_TestCase = Retest_option;
        break;

    default:
        Verify_1121_TestCase=Init_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }
    return ReturnVal; // Required for Retest Option    
}

void Set_FPGA_bits(int16 which)
{
  int8 i,j,CPLD2_bit = 0;
  int16 CPLD2_Set = 0;
  
  if(which == -1) return;
  for(i = 0; i < 12; i++) // first set up CPLD1 bits
  { // 
    j = The_Verify_map[which].Which_CPLD1_bits[i];
    if(j == -1) break;
    // then the NOT break
    Set_a_BITx(j);  
  }
  
  for(i = 0; i < 12; i++) // now set up CPLD2 bits
  {
    j = The_Verify_map[which].Which_CPLD2_bits[i];
    if(j == -1) break;
    // then the NOT break
    // SUF_output_cmnd_flag = 1; // set CPLD2;
    CPLD2_Set = (j << 8) + 1;
    Set_Specific_CPLD2_Signal(CPLD2_Set);
  }   
  
  for(i = 0; i < 4; i++) // now set up NTS bits
  {
    j = The_Verify_map[which].Which_NTS_bits[i];
    if(j == -1) break;
    // then the NOT break
    Set_an_NTS_BITx(j);  
  }  
}

//================

void Clear_FPGA_bits(int16 which)
{
  int8 i,j,CPLD2_bit = 0;
  int16 CPLD2_Set = 0;
  
  if(which == -1) return;
  for(i = 0; i < 12; i++) // first set up CPLD1 bits
  {
    j = The_Verify_map[which].Which_CPLD1_bits[i];
    if( (j == -1) || (j == 0) ) break;
    // then the NOT break
    Clear_a_BITx(j);  
  }
  
  for(i = 0; i < 12; i++) // now set up CPLD2 bits
  {
    j = The_Verify_map[which].Which_CPLD2_bits[i];
    if( (j == -1) || (j == 7) ) break;
    // then the NOT break
    // SUF_output_cmnd_flag = 1; // set CPLD2;
    CPLD2_Set = (j << 8) ; // will set the command flag OFF
    Set_Specific_CPLD2_Signal(CPLD2_Set);
  }
  All_NTS_Bits(0); // turn off all NTS bits (4)       
}

void Set_1106AN_channels(int16 which)
{
  int8 i,j = 0;
  
  if(which == -1) return;
  for(i = 0; i < 12; i++) 
  {
    j = The_Verify_map[which].Which_1106AN_outputs[i];
    if(j == -1) return;
    // then the NOT break
    setoutp(Virtual_Board_B_OUT[j]);  
  }
  return;    
}

int16 Verify_Inputs_Zero(int16 TheLoop)
{
    static int16 VIZ_TestCase = VIZ_Init_Test;
    static int16 Counter = 0;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option  
    static int16 NextCaseAfterWait = 0;
    static int16 k = 0;  
    int16 i=0;
    int16 j=0;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
   
    switch (VIZ_TestCase) {
    case VIZ_Init_Test:
        clr_lcd_dply();
        Clear_Msg_Board();  // ClearFaults(0);
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
   
        NextCaseAfterWait =  VIZ_Case01;  
        timers[t_Generic_Delay] = 0;  
        VIZ_TestCase = VIZ_Wait_State;        
        break;  
        

    case VIZ_Wait_State:
        if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
            timers[t_Generic_Delay] = 0; //
            VIZ_TestCase = NextCaseAfterWait;
        }
        break;               
        
    case VIZ_Case01:
       for(i=0; i< 12; i++) 
        {
           j = rdinp(Virtual_Board_A_in[i]); // All inputs should be zero.
           if (j)
           {
              VIZ_TestCase = VIZ_Case06_Fail;
              return CONTINUE_SERVICE;
           }            
        } 
        NextCaseAfterWait = VIZ_Case02;
        VIZ_TestCase=VIZ_Case05_Pass;        
        break;  

   case VIZ_Case05_Pass:
        CurrentTestPassFail = 1; // pass
        New_write_display("   PASS             ",1,0);
        VIZ_TestCase = VIZ_Test_Done;        
        break;
        
    case VIZ_Test_Done:
        VIZ_TestCase = VIZ_Init_Test;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
        break;                        

    case VIZ_Case06_Fail:
        New_write_display("      FAILED        ",1,0);
        VIZ_TestCase = VIZ_Test_Done;
        break;

    default:
        VIZ_TestCase=VIZ_Init_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }

    return ReturnVal; // Required for Retest Option   
}

int16 Verify_Signal(int16 TheLoop)
{
    static int16 VIZ_TestCase = VIZ_Init_Test;
    static int16 Counter = 0;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option  
    static int16 NextCaseAfterWait = 0;
    static int16 k = 0;  
    int16 i=0;
    int16 j=0;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
   
    switch (VIZ_TestCase) {
    case VIZ_Init_Test:
        clr_lcd_dply();
        Clear_Msg_Board();  // ClearFaults(0);
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
   
        NextCaseAfterWait =  VIZ_Case01;  
        timers[t_Generic_Delay] = 0;  
        VIZ_TestCase = VIZ_Wait_State;        
        break;         

    case VIZ_Wait_State:
        if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
            timers[t_Generic_Delay] = 0; //
            VIZ_TestCase = NextCaseAfterWait;
        }
        break;               
         
    case VIZ_Case01:
        VIZ_TestCase=VIZ_Case05_Pass;
        for(i = 0; i<12; i++)
        {        
            j = The_Verify_map[TheLoop].Which_1106AN_inputs[i];
            if(j == -1) break;
            // now NOT break
            if(!rdinp(Virtual_Board_A_in[j]))
            {
              VIZ_TestCase = VIZ_Case06_Fail;
              sprintf(src,"rdinp(bd3)[%d]fail", j) ;
              New_write_display(src,2,0);
              sprintf(src,"TheLoop=%d", TheLoop) ;
              New_write_display(src,3,0);              
              break;   
            }
        }        
        break;  

   case VIZ_Case05_Pass:
        CurrentTestPassFail = 1; // pass
        New_write_display("   PASS             ",1,0);
        VIZ_TestCase = VIZ_Test_Done;        
        break;
        
    case VIZ_Test_Done:
        VIZ_TestCase = VIZ_Init_Test;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
        break;                        

    case VIZ_Case06_Fail:
        New_write_display("      FAILED        ",1,0);
        VIZ_TestCase = VIZ_Test_Done;
        break;

    default:
        VIZ_TestCase=VIZ_Init_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }

    return ReturnVal; // Required for Retest Option   
}

int16 Verify_the_Inputs_Zero(int16 TheLoop)
{
    static int16 VIZ_TestCase = VIZ_Init_Test;
    static int16 Counter = 0;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option  
    static int16 NextCaseAfterWait = 0;
    static int16 k = 0;  
    int16 i=0;
    int16 j=0;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
   
    switch (VIZ_TestCase) {
    case VIZ_Init_Test:
        clr_lcd_dply();
        Clear_Msg_Board();  // ClearFaults(0);
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED  
        VIZ_TestCase = VIZ_Case01;        
        break;         

    case VIZ_Wait_State:
        if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
            timers[t_Generic_Delay] = 0; //
            VIZ_TestCase = NextCaseAfterWait;
        }
        break;               
         
    case VIZ_Case01:
       for(i=0; i< 12; i++) 
        {
           j = rdinp(Virtual_Board_A_in[i]); // All inputs should be zero.
           if (j)
           {
              VIZ_TestCase = VIZ_Case06_Fail;
              return CONTINUE_SERVICE;
           }            
        } 
        VIZ_TestCase=VIZ_Case05_Pass;        
        break;  

   case VIZ_Case05_Pass:
        CurrentTestPassFail = 1; // pass
        New_write_display("   PASS             ",1,0);
        VIZ_TestCase = VIZ_Test_Done;        
        break;
        
    case VIZ_Test_Done:
        VIZ_TestCase = VIZ_Init_Test;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
        break;                        

    case VIZ_Case06_Fail:
        New_write_display("      FAILED        ",1,0);
        VIZ_TestCase = VIZ_Test_Done;
        break;

    default:
        VIZ_TestCase=VIZ_Init_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }

    return ReturnVal; // Required for Retest Option   
}

int16 Verify_No_Short(int16 TheLoop)
{
    static int16 VIZ_TestCase = VIZ_Init_Test;
    static int16 Counter = 0;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option  
    static int16 NextCaseAfterWait = 0;
    static int16 k = 0;  
    int16 i=0;
    int16 j=0;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
   
    switch (VIZ_TestCase) {
    case VIZ_Init_Test:
        clr_lcd_dply();
        Clear_Msg_Board();  // ClearFaults(0);
        CurrentTestPassFail =0;  // fail
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option        
        Counter = 0;
        SIU.GPDO[Z6_LED].R=1;  //Turn OFF LED
   
        NextCaseAfterWait =  VIZ_Case01;  
        timers[t_Generic_Delay] = 0;  
        VIZ_TestCase = VIZ_Wait_State;        
        break;         

    case VIZ_Wait_State:
        if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
            timers[t_Generic_Delay] = 0; //
            VIZ_TestCase = NextCaseAfterWait;
        }
        break;               
         
    case VIZ_Case01:
        VIZ_TestCase=VIZ_Case05_Pass;
        for(i=0; i<12; i++)
        {
           j = The_Verify_map[TheLoop].NOT_1106AN_inputs[i];
           if(j == -1) break;
            // now NOT break
           if(rdinp(Virtual_Board_A_in[j]))
           {
              sprintf(src,"[%d] = %d", j, rdinp(Virtual_Board_A_in[j]));
              Log_an_event(src);
              VIZ_TestCase = VIZ_Case06_Fail;              
           }; 
        } 
        if(VIZ_TestCase == VIZ_Case06_Fail)
        {
          New_write_display("Short detected -fail",2,0);    
        }        
        break;  

   case VIZ_Case05_Pass:
        CurrentTestPassFail = 1; // pass
        New_write_display("   PASS             ",1,0);
        VIZ_TestCase = VIZ_Test_Done;        
        break;
        
    case VIZ_Test_Done:
        VIZ_TestCase = VIZ_Init_Test;
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
        break;                        

    case VIZ_Case06_Fail:
        New_write_display("      FAILED        ",1,0);
        VIZ_TestCase = VIZ_Test_Done;
        break;

    default:
        VIZ_TestCase=VIZ_Init_Test;
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }

    return ReturnVal; // Required for Retest Option   
}

int16 The_Verify_map_Helper(int16 which, int16 The_Index, int16 In_or_Out) 
{
    if(In_or_Out)return The_Verify_map[which].Which_1106AN_inputs[The_Index];
    else return The_Verify_map[which].NOT_1106AN_inputs[The_Index];
}




// ================