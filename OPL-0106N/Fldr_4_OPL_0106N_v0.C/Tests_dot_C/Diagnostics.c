// Diagnostics.c

#include "global.h"
#include "Tests.h"
#define MaxFirePtrs 5

void* UnFirePtrs[MaxFirePtrs];
int16 UnFirePtrs_index = 0;
int16 IFL_IFB_debug = 0;

enum
{
    initialize,
    run_test,
    test_pass,
    test_done,
    test_failure,
    Retest_option,
    Check_Buttons,
    Read_After_Delay,
};

#define MAX_DIAG_SIGNALS 22

int16 Diag_output_state[MAX_DIAG_SIGNALS];

const int16 Diag_Labels[MAX_DIAG_SIGNALS][3] =
    { // see IO_Labels[]
        {O_CSS_lable_enum, -1, i_CSS_COPi1_lable_enum},                       //0  CSS
        {O_C120T_lable_enum, -1, -1},                     //1  C120
        {O_L15RL_lable_enum, i_L15_lable_enum, i_L25_lable_enum},       //2  L15
        {O_FSS_lable_enum, i_FFSWT_lable_enum, -1},       //3  FSS
        {O_CST_lable_enum, i_CST_T_lable_enum, -1},       //4  CST
        {O_ICC_lable_enum, i_ICC_T_lable_enum, -1},       //5  ICC
        {O_ICA_lable_enum, i_ICA_T_lable_enum, -1},       //6  ICA
        {O_ICI_lable_enum, i_ICI_T_lable_enum, -1},       //7  ICI
        {O_ACC_lable_enum, i_ACC_T_lable_enum, -1},       //8  ACC
        {O_BELL_lable_enum, -1, -1},                      //9  BELL
        {O_C24_lable_enum, i_C24_T_lable_enum, -1},       //10 C24
        {O_IFL_COPo0_lable_enum, -1, -1},                 //11 IFL
        {O_IFB_COPo1_lable_enum, -1, -1},                 //12 IFB
        {O_CUL_COPo2_lable_enum, i_CUL_T_lable_enum, -1}, //13 CUL
        {O_CDL_COPo3_lable_enum, i_CDL_T_lable_enum, -1}, //14 CDL
        {O_FAN_COPo4_lable_enum, i_FANST_lable_enum, -1}, //15 FAN
        {O_LIG_COPo5_lable_enum, i_LIGST_lable_enum, -1}, //16 LIG
        {-1, i_CTS_T_lable_enum, i_CTS_COPi2_lable_enum},                     //17 Stop SW
        {-1, i_INS_lable_enum,i_CTA_lable_enum},                       //18 INS SW
        {-1, i_IEN_lable_enum, -1},                       //19 EN SW
        {-1, i_IU_lable_enum, -1},                        //20 IU
        {-1, i_ID_lable_enum, -1},                        //21 ID
};

const int DiagIO_Map[MAX_DIAG_SIGNALS][6] =
    {
        //OutputType  // Output         //InputType    //Input     /InputType   //Input
        {SEBTYPE,   O_CSS_enum,       NOTYPE,        -1,           TOCTYPE,    i_CSS_COPi1_enum},   //0  CSS  -  CSS_SEB_OUT
        {SEBTYPE,   O_C120T_enum,     NOTYPE,        -1,           NOTYPE,                   -1},   //1  C120 -  C120_SEB_OUT
        {SEBTYPE,   O_L15RL_enum,     SEBTYPE,       i_L15_enum,   SEBTYPE,          i_L25_enum},   //2  L15  -  L15_SEB_OUT
        {SEBTYPE,   O_FSS_enum,       SEBTYPE,       i_FFSWT_enum, NOTYPE,                   -1},   //3  FSS  -  FSS_SEB_OUT
        {SEBTYPE,   O_CST_enum,       SEBTYPE,       i_CST_T_enum, NOTYPE,                   -1},   //4  CST  -  CST_SEB_OUT
        {SEBTYPE,   O_ICC_enum,       SEBTYPE,       i_ICC_T_enum, NOTYPE,                   -1},   //5  ICC  -  ICC_SEB_OUT
        {SEBTYPE,   O_ICA_enum,       SEBTYPE,       i_ICA_T_enum, NOTYPE,                   -1},   //6  ICA  -  ICA_SEB_OUT
        {SEBTYPE,   O_ICI_enum,       SEBTYPE,       i_ICI_T_enum, NOTYPE,                   -1},   //7  ICI  -  ICI_SEB_OUT
        {SEBTYPE,   O_ACC_enum,       SEBTYPE,       i_ACC_T_enum, NOTYPE,                   -1},   //8  ACC  -  ACC_SEB_OUT
        {SEBTYPE,   O_BELL_enum,      NOTYPE,        -1,           NOTYPE,                   -1},   //9  BELL -  BELL_SEB_OUT
        {SEBTYPE,   O_C24_enum,       SEBTYPE,       i_C24_T_enum, NOTYPE,                   -1},   //10 C24  -  C24_SEB_OUT
        {TOCTYPE,   O_IFL_COPo0_enum, NOTYPE,        -1,           NOTYPE,                   -1},   //11 IFL  -  IFL_TOC_OUT
        {TOCTYPE,   O_IFB_COPo1_enum, NOTYPE,        -1,           NOTYPE,                   -1},   //12 IFB  -  IFB_TOC_OUT
        {TOCTYPE,   O_CUL_COPo2_enum, SEBTYPE,       i_CUL_T_enum, NOTYPE,                   -1},   //13 CUL  -  CUL_TOC_OUT
        {TOCTYPE,   O_CDL_COPo3_enum, SEBTYPE,       i_CDL_T_enum, NOTYPE,                   -1},   //14 CDL  -  CDL_TOC_OUT
        {TOCTYPE,   O_FAN_COPo4_enum, SEBTYPE,       i_FANST_enum, NOTYPE,                   -1},   //15 FAN  -  FANR_TOC_OUT
        {TOCTYPE,   O_LIG_COPo5_enum, SEBTYPE,       i_LIGST_enum, NOTYPE,                   -1},   //16 LIG  -  LIGR_TOC_OUT
        {SWTYPE,    -1,               SEBTYPE,       i_CTS_T_enum, TOCTYPE,    i_CTS_COPi2_enum},   //17 Stop SW
        {SWTYPE,    -1,               SEBTYPE,       i_INS_enum,   SEBTYPE,          i_CTA_enum},   //18 INS SW
        {SWTYPE,    -1,               SEBTYPE,       i_IEN_enum,   NOTYPE,                   -1},   //19 EN SW
        {SWTYPE,    -1,               SEBTYPE,       i_IU_enum,    NOTYPE,                   -1},   //20 IU
        {SWTYPE,    -1,               SEBTYPE,       i_ID_enum,    NOTYPE,                   -1}    //21 ID
};
/*

enum 
{
  DONE_BUT_CONTINUE_DISPLAY = -1, // failed
  FINISHED_WITH_SERVICE, // passed
  CONTINUE_SERVICE, // still performing test
  RETEST_OPTION_AFTER_DELAY,  //  to see msg
  RETEST_OPTION, //failed, but give option to redo
  CLOSE_MSG_BOARD, //  Used by retest option
  DEFAULT  // Fatal error - abort	
};

Check_for_Mode_Button()
Check_for_Enter_Button()
Check_for_Up_Button()
Check_for_Down_Button()

rdinp();
setoutp()
clroutp()
clroutp((void *)SEB_outputs_array[O_L15RL_enum] );
setoutp( (void *)SEB_outputs_array[O_C120T_enum] );
*/

int16 Read_the_Output(int16 DiagIO_Map_index)
{ // This is not really a test, just using the test code structure for convenience

    int16 i, j, k, Diag_IO_index = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0;       // Required for Retest Option
    static int16 RetestResponse = 1;           // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    static int16 State_of_Output = 0;
    static int16 State_of_Input = 0;
    int16 temp_read_var = 5;

    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R; // toggle LED;
    switch (test_Kase)
    {
    case initialize:
        //  ADD CODE
        //            clr_lcd_dply();
        ReturnVal = CONTINUE_SERVICE;
        CurrentTestPassFail = 1; // PASS
        Retest_Option_Parm = 0;
        RetestResponse = 1;
        Delay_Count = 3;
        timers[t_Generic_Delay] = 0;
        test_Kase = run_test;
        break;

    case run_test:
        if (timers[t_Generic_Delay] < Delay_Count)
            break;
        //  ADD CODE
        if (Check_for_Enter_Button())
        {
            timers[t_Generic_Delay] = 0;
            Diag_IO_index = (short)DiagIO_Map[DiagIO_Map_index][1];

            temp_read_var = rdoutp((void *)SEB_outputs_array[Diag_IO_index]);
            sprintf(src, "rdout= %d, idx=%d", temp_read_var, Diag_IO_index);
            New_write_display(src, 2, 0); // row, col
        }
        if (Check_for_Mode_Button())
        {
            timers[t_Generic_Delay] = 0;
            CurrentTestPassFail = 1; // pass
            RetestResponse = 0;      // FINISHED_WITH_SERVICE
            test_Kase = test_done;
        }
        break;

    case test_pass:
        test_Kase = test_done;
        break;

    case test_done:
        test_Kase = initialize;
        // Required for Retest Option
        if (RetestResponse == 0)
            ReturnVal = FINISHED_WITH_SERVICE; // Idle_time
        else
            ReturnVal = DONE_BUT_CONTINUE_DISPLAY;
        break;

    case test_failure:
        test_Kase = Retest_option;
        break;

    case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if (RetestResponse == 0)
            test_Kase = test_done; // do not retest
        else if (RetestResponse == 1)
            test_Kase = initialize; // test again
        else
            ReturnVal = CONTINUE_SERVICE;
        break;

    default:
        break;
    }
    return ReturnVal;
}

int16 Set_or_clear_CN19V(int16 On_Off)
{
    if(On_Off) setoutp(O_CN19V);
    else clroutp(O_CN19V);
    return FINISHED_WITH_SERVICE; 
}

int16 Set_or_clear_CN18V(int16 On_Off)
{
    if(On_Off) setoutp(O_CN18V);
    else clroutp(O_CN18V);
    return FINISHED_WITH_SERVICE; 
}

int16 Set_or_clear_CLOSE(int16 On_Off)
{
    if(On_Off) setoutp(O_CLOSE);
    else clroutp(O_CLOSE);
    return FINISHED_WITH_SERVICE; 
}

int16 Set_or_clear_OPEN(int16 On_Off)
{
    if(On_Off) setoutp(O_OPEN);
    else clroutp(O_OPEN);
    return FINISHED_WITH_SERVICE; 
}

// NUDGE
int16 Set_or_clear_NUDGE(int16 On_Off)
{
    if(On_Off) setoutp(O_NUDGE);
    else clroutp(O_NUDGE);
    return FINISHED_WITH_SERVICE; 
}

// HEAVY
int16 Set_or_clear_HEAVY(int16 On_Off)
{
    if(On_Off) setoutp(O_HEAVY);
    else clroutp(O_HEAVY);
    return FINISHED_WITH_SERVICE; 
}

// DCL
int16 Set_or_clear_DCL(int16 On_Off)
{
    if(On_Off) setoutp(O_DCL);
    else clroutp(O_DCL);
    return FINISHED_WITH_SERVICE; 
}

// RO
int16 Set_or_clear_RO(int16 On_Off)
{
    if(On_Off) setoutp(O_RO);
    else clroutp(O_RO);
    return FINISHED_WITH_SERVICE; 
}

// DOL
int16 Set_or_clear_DOL(int16 On_Off)
{
    if(On_Off) setoutp(O_DOL);
    else clroutp(O_DOL);
    return FINISHED_WITH_SERVICE; 
}

int16 Set_or_clear_24V_on_SEB4(int16 On_Off)
{
    if(On_Off) setoutp(O_C24_ctcan);
    else clroutp(O_C24_ctcan);
    return FINISHED_WITH_SERVICE; 
}

int16 Set_or_clear_oCSS_on_SEB1(int16 On_Off)
{
    if(On_Off) setoutp(O_CSS_ctcan);
    else clroutp(O_CSS_ctcan);
    return FINISHED_WITH_SERVICE; 
}

/***********************************************
*  Function/Routine/Class Name: 
*  	  int16 Diagnostic_Control(int16 DiagIO_Map_index) 
*  Date Created: May 2018
*  Last Updated: May 26, 2018
*  Version:      
*  Contributors: William Grauer
*
*  Explanation: Allows the operator to toggle each of the SEB outputs and 
*	to see the results on the associated inputs.
*
*//////////////////////////////////////////////
int16 Diagnostic_Control(int16 DiagIO_Map_index)
{
    int16 i, j, k  = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0;       // Required for Retest Option
    static int16 RetestResponse = 1;           // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    static int16 State_of_Output = 0;
    static int16 State_of_Input = 0;
    static int16 IO_item_at_start = 5;
    static int16 IO_item_at_end = 6;
    static int16 Diag_IO_index = 0;
    static int16 Diag_Input_index_1 = 0;
    static int16 Diag_Input_index_2 = 0;

    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R; // toggle LED;
    switch (test_Kase)
    {
    case initialize:
        clr_lcd_dply();
        //            Clear_Msg_Board();
        //			DisplayIo(DiagIO_Map_index);
        FirePreReqSignal(DiagIO_Map_index);
        Diag_IO_index = (short)DiagIO_Map[DiagIO_Map_index][1];
        Diag_Input_index_1 = (short)DiagIO_Map[DiagIO_Map_index][3];
        Diag_Input_index_2 = (short)DiagIO_Map[DiagIO_Map_index][5];
        IO_item_at_start = rdoutp((void *)SEB_outputs_array[Diag_IO_index]);            
        sprintf(src, "Int:i=%d,rdt=%d", Diag_IO_index, IO_item_at_start);
        New_write_display(src, 0, 0);        
        ReturnVal = CONTINUE_SERVICE;
        CurrentTestPassFail = 0; // fail
        Retest_Option_Parm = 0;
        RetestResponse = 1;
        Delay_Count = 3;
        timers[t_Generic_Delay] = 0;
        test_Kase = Check_Buttons ;  //run_test;
        break;

    case Check_Buttons:
        if (timers[t_Generic_Delay] < Delay_Count)
            break;
        
        if (Check_for_Enter_Button())
        {
            timers[t_Generic_Delay] = 0;
            if(DiagIO_Map[DiagIO_Map_index][1]  >= 0)
            { //  toggle the output upon entry
	            IO_item_at_start = rdoutp((void *)SEB_outputs_array[Diag_IO_index]);            
	            sprintf(src, "idx= %d, rdoutp=%d", Diag_IO_index, IO_item_at_start);
	            New_write_display(src, 0, 0);
	//            Log_an_event(src);
	//            Add_msg_to_array_B(src, 0);
	//            New_write_display(src, 1, 0);                          // row, col

	            if ( IO_item_at_start ) 
	                clroutp((void *)SEB_outputs_array[Diag_IO_index]);
	            else
	                setoutp((void *)SEB_outputs_array[Diag_IO_index]);         	
            }

            test_Kase = Read_After_Delay; // test_pass;
        }
        if (Check_for_Mode_Button())
        {
            timers[t_Generic_Delay] = 0;
            CurrentTestPassFail = 1; // pass
            RetestResponse = 0;      // FINISHED_WITH_SERVICE
            test_Kase = test_done;
        }
        break;

    case Read_After_Delay: // allows time for CTCAN to send and receive the msgs
        if (timers[t_Generic_Delay] < Delay_Count)
            break; 
        timers[t_Generic_Delay] = 0;

        if(DiagIO_Map[DiagIO_Map_index][1]  >= 0)
        { // there is an output to be generated.
	        k = Diag_Labels[DiagIO_Map_index][0];
	        IO_item_at_end = rdoutp((void *)SEB_outputs_array[Diag_IO_index]);            
	        if(IO_item_at_end) 
	          sprintf(src, "%s ON ",(void *)IO_Labels[k]);
	        else  sprintf(src, "%s off",(void *)IO_Labels[k]);
	        New_write_display(src, 1, 0);  
	        sprintf(src,"idx0=%d,k=%d,val=%d",DiagIO_Map_index, k, IO_item_at_end);
	        Log_an_event(src);      	
        }
        
        if( Diag_Input_index_1 >= 0 )  
        { // input must be displayed
            k = Diag_Labels[DiagIO_Map_index][1];
            IO_item_at_end = rdinp((void *)SEB_inputs_array[Diag_Input_index_1]);
	        if(IO_item_at_end) 
	          sprintf(src, "%s ON ",(void *)IO_Labels[k]);
	        else  sprintf(src, "%s off",(void *)IO_Labels[k]); 
            New_write_display(src, 2, 0); 
            sprintf(src,"idx1=%d,k=%d,val=%d",Diag_Input_index_1, k, IO_item_at_end);
	        Log_an_event(src);        	
        }
        
        if( Diag_Input_index_2 >= 0 )  
        { // input must be displayed
            k = Diag_Labels[DiagIO_Map_index][2];
            IO_item_at_end = rdinp((void *)SEB_inputs_array[Diag_Input_index_2]);
	        if(IO_item_at_end) 
	          sprintf(src, "%s ON ",(void *)IO_Labels[k]);
	        else  sprintf(src, "%s off",(void *)IO_Labels[k]);
            New_write_display(src, 3, 0); 
            sprintf(src,"idx2=%d,k=%d,val=%d",Diag_Input_index_2, k, IO_item_at_end);
	        Log_an_event(src);                  	
        }        
        
        test_Kase = Check_Buttons; // go back to monitor for Enter or Mode;
        break;

    case test_done:  // really not a test, just a means of toggleing output,
      // so Retest is not appropriate, therefore 'RetestResponse' was forced to zero
      // in 'Check_Buttons' (Mode button).
      
        // Do not clear 120VAC on exit
        if((void *)SEB_outputs_array[Diag_IO_index] != The_120VAC_Ptr) clroutp((void *)SEB_outputs_array[Diag_IO_index]);
        Un_FirePreReqSignal(); // undo all the set outputs except 120VAC
        test_Kase = initialize;
        // Required for Retest Option
        if (RetestResponse == 0)
            ReturnVal = FINISHED_WITH_SERVICE; // Idle_time
        else
            ReturnVal = DONE_BUT_CONTINUE_DISPLAY;
        break;
/*
    case test_failure:
        test_Kase = Retest_option;
        break;

    case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if (RetestResponse == 0)
            test_Kase = test_done; // do not retest
        else if (RetestResponse == 1)
            test_Kase = initialize; // test again
        else
            ReturnVal = CONTINUE_SERVICE;
        break;
*/
    default:
        break;
    }
    return ReturnVal;
}

int16 Set_Diag_output(int16 DiagIO_Map_index, int16 state)
{
    if (state)
        setoutp((void *)SEB_outputs_array[DiagIO_Map_index]);
    else
        clroutp((void *)SEB_outputs_array[DiagIO_Map_index]);
    return FINISHED_WITH_SERVICE;
}


/*********************************************************/
/**                Fire Prerequest IO   	            **/
/*********************************************************/
void FirePreReqSignal(int IoIndex)
{

    switch (IoIndex)
    {
    case 0: //CSS
            //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        break;
    case 1: //C120
        break;
    case 2: //L15
            //			FireIO(L15RL_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_L15RL_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_L15RL_enum];  
        }        
        break;
    case 3: //FSS
        break;
    case 4: //CST
            //			ClearIO(L15RL_SEB_OUT);
        clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
        break;
        
/*
void* UnFirePtrs[MaxFirePtrs];
int16 UnFirePtrs_index = 0;

*/        
    case 5: //ICC
            //			ClearIO(L15RL_SEB_OUT);
        clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
        break;
    case 6: //ICA
            //			ClearIO(L15RL_SEB_OUT);
        clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
        break;
    case 7: //ICI
            //			ClearIO(L15RL_SEB_OUT);
        clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
        break;
    case 8: //ACC
            //			ClearIO(L15RL_SEB_OUT);
        clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
        break;
    case 9: //BELL
        break;
    case 10: //C24
        break;
    case 11: //IFL
             //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(L15_SEB_OUT); // O_L15_1_enum
        setoutp((void *)SEB_outputs_array[O_L15_1_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        } 
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_L15_1_enum];  
        }                 
        break;
    case 12: //IFB
             //			FireIO(C120_SEB_OUT);
             //			FireIO(L15_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        setoutp((void *)SEB_outputs_array[O_L15_1_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        } 
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_L15_1_enum];  
        }         
        break;
    case 13: //CUL
             //			ClearIO(L15RL_SEB_OUT);
        clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
        //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(CLC_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_CLC_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        } 
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_CLC_enum];  
        }                 
        break;
    case 14: //CDL
             //			ClearIO(L15RL_SEB_OUT);
        clroutp((void *)SEB_outputs_array[O_L15RL_enum]);
        //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(CLC_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_CLC_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_CLC_enum];  
        }                
        break;
    case 15: //FAN
             //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(L15_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_L15_1_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_L15_1_enum];  
        }                
        break;
    case 16: //LIG
             //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(L15_SEB_OUT);
         setoutp((void *)SEB_outputs_array[O_L15_1_enum]);
        //setoutp((void *)SEB_outputs_array[O_LIG_COPo5_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_L15_1_enum];
        //    UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_LIG_COPo5_enum];  
        }        
        break;
    case 17: //StopSw
             //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(CSS_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_CSS_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_CSS_enum];  
        }        
        break;
    case 18: //InsSw
             //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(CSS_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_CSS_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_CSS_enum];  
        }        
        break;
    case 19: //EnSw
             //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(CSS_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_CSS_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_CSS_enum];  
        }        
        break;
    case 20: //IuSw
             //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(CSS_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_CSS_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_CSS_enum];  
        }        
        break;
    case 21: //IdSw
             //			FireIO(C120_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_C120T_enum]);
        //			FireIO(CSS_SEB_OUT);
        setoutp((void *)SEB_outputs_array[O_CSS_enum]);
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_C120T_enum];  
        }
        if(UnFirePtrs_index < MaxFirePtrs - 1)
        {
            UnFirePtrs[UnFirePtrs_index++] = (void *)SEB_outputs_array[O_CSS_enum];  
        }        
        break;
        
    default:
        break;
    }
    return;
}

int16 Toggle_4_0047(int16 channel)
{
    static int16 toggle_state[MAX_0047_INDEX];

    if (channel < 0 || channel > 5)
        return FINISHED_WITH_SERVICE; // error

    sprintf(src, "chnel=%d,state=%d", channel, toggle_state);
    Log_an_event(src);
    Add_msg_to_array_B(src, channel);
    //    toggle_state = !toggle_state;
    if (toggle_state[channel] = !toggle_state[channel])
        setoutp(GALX4_0047_OUT_array[channel]); //  LED from 'SWi0' - 'LDO11'
    else
        clroutp(GALX4_0047_OUT_array[channel]);
    return FINISHED_WITH_SERVICE;
}

int16 C120_VAC_on_off(int16 on_off)
{
    if(on_off) setoutp( (void *)SEB_outputs_array[O_C120T_enum]); // Apply 120 VAC
    else clroutp( (void *)SEB_outputs_array[O_C120T_enum]); //  120 VAC OFF
    
    return FINISHED_WITH_SERVICE;
}

int16 L15RL_on_off(int16 on_off)
{
    if(on_off) setoutp( (void *)SEB_outputs_array[O_L15RL_enum]); 
    else clroutp( (void *)SEB_outputs_array[O_L15RL_enum]); 
    
    return FINISHED_WITH_SERVICE;
}
/*
int16 L15_T_on_off(int16 on_off)
{
    if(on_off) setoutp( (void *)SEB_outputs_array[O_L15_1_enum]); // Apply 120 VAC
    else clroutp( (void *)SEB_outputs_array[O_L15_1_enum]); //  120 VAC OFF
    
    return FINISHED_WITH_SERVICE;
}
*/

int16 C120_Plus_24_on_off(int16 on_off)
{
    if(on_off) 
    {
        setoutp( (void *)SEB_outputs_array[O_C120T_enum]); // Apply 120 VAC
        setoutp(O_C24_ctcan);   
    }
    else 
    {
        clroutp( (void *)SEB_outputs_array[O_C120T_enum]); //  120 VAC OFF
        clroutp(O_C24_ctcan);
    }
    
    return FINISHED_WITH_SERVICE;
}

int16 GFI_Relay_on_off(int16 on_off)
{
    if(on_off) setoutp( (void *)SEB_outputs_array[O_L15RL_enum]); // Apply 120 VAC
    else clroutp( (void *)SEB_outputs_array[O_L15RL_enum]); //  120 VAC OFF
    
    return FINISHED_WITH_SERVICE;
}

int16 L15_T_on_off(int16 on_off)
{
    if(on_off) setoutp( (void *)SEB_outputs_array[O_L15_1_enum]); // Apply 120 VAC
    else clroutp( (void *)SEB_outputs_array[O_L15_1_enum]); //  120 VAC OFF
    
    return FINISHED_WITH_SERVICE;
}

void Un_FirePreReqSignal(void)
{
    int16 idx = 0;
//    void *The_120VAC_Ptr = (void *)SEB_outputs_array[O_C120T_enum];
    for(idx = 0; idx < MaxFirePtrs ; idx++)
    {
        if( UnFirePtrs[idx] != NULL ) 
        { // Do not clear the 120VAC SEB
            if(UnFirePtrs[idx] != The_120VAC_Ptr) clroutp( (void *)UnFirePtrs[idx] );
            UnFirePtrs[idx] = NULL;   
        }   
    } 
    UnFirePtrs_index = 0; 
}

int16 Set_CAN_msgs(int16 CAN_MsgNum)
{ // setoutp((void *)SEB_outputs_array[DiagIO_Map_index]);
  int16 CAN_Kase = CAN_MsgNum;
  static int16 ReturnVal = CONTINUE_SERVICE;
  static int16 FirstPass = 0;
  static int16 ViewingMsgBrd = 0;
  int16 Set_Idx = 0;
  
  if(FirstPass == 1)
  { // check for mode button
      if (Check_for_Enter_Button() || ViewingMsgBrd)
      {    
          ViewingMsgBrd = 1;    
//          ModeButton =  ButtonPressAck;  
          // gets reset in the pack IFL_IFB_debug = 0;  // to disable pack_can_out to display debug msgs to msg board B
          ReturnVal = DisplayMsgBrd_B(0); // show me the data  // FINISHED_WITH_SERVICE; 
          if(ReturnVal == FINISHED_WITH_SERVICE)
          {
              FirstPass = 0;
              ViewingMsgBrd = 0;
              return ReturnVal;	
          }
          else return ReturnVal;
      }
      else return CONTINUE_SERVICE;
  }
  
  FirstPass = 1;
  Clear_All_Boards(2); // msg board B
  switch(CAN_Kase)
  {
  	case 0: // O_IFL_COPo0_enum 
        Set_Idx = O_IFL_COPo0_enum;
  	    setoutp((void *)SEB_outputs_array[O_IFL_COPo0_enum]);
  	    break;
  	    
  	case 1: // O_IFB_COPo1_enum
        Set_Idx = O_IFB_COPo1_enum;  	
  	    setoutp((void *)SEB_outputs_array[O_IFB_COPo1_enum]);
  	    break;
  	      	    
  	case 2: // O_CUL_COPo2_enum
  	    setoutp((void *)SEB_outputs_array[O_CUL_COPo2_enum]);
  	    Set_Idx = O_CUL_COPo2_enum;
  	    break;
  	    
  	case 3: // O_CDL_COPo3_enum
  	    setoutp((void *)SEB_outputs_array[O_CDL_COPo3_enum]);
  	    Set_Idx = O_CDL_COPo3_enum;
  	    break;
  	          	      	    
  	case 4: // O_FAN_COPo4_enum
  	    setoutp((void *)SEB_outputs_array[O_FAN_COPo4_enum]);
  	    Set_Idx = O_FAN_COPo4_enum;
  	    break;
  	    
  	case 5: // O_LIG_COPo5_enum
  	    setoutp((void *)SEB_outputs_array[O_LIG_COPo5_enum]);
  	    Set_Idx = O_LIG_COPo5_enum;
  	    break;
  	    
  	case 6: // Selector Comm
  	    setoutp(o_SELCM);
  	    break;  	  
  	    
  	case 7: // NTS Comm
  	    setoutp(o_NTSCM);
  	    break;    	      
  	    
  	case 8: // Selector Comm
  	    setoutp(o_FST); // turn ON the fire service stopswitch bypass
  	    break;  	  
  	    
  	case 9: // NTS Comm
  	    setoutp(o_FSTP);
  	    break;  	          	        	      	    
    default:
        break;
  }
  
/*  
    sprintf(src,"set_idx= %d", Set_Idx);
    New_write_display(src, 3, 0);
    IFL_IFB_debug = 1;  // to allow pack_can_out to display debug msgs to msg board B
    Check_to_manualy_Pack_and_send_data(); // send the data if Enable_CT_CAN_Ctrl == 0
    return CONTINUE_SERVICE; 
*/   
    FirstPass = 0;
    return FINISHED_WITH_SERVICE;    
}

int16 Read_State_of_0047_input_SWs(int16 dummy)
{
    uint16 i, j, k = 0;
    
    i = 0;
    j = 0;
    k = 0;
    for(i = 0; i < 6; i++)
    {
        k = rdinp(GALX4_0047_in_array[i]); 
        sprintf(src,"k=0x%2X,j=0x%2X,i=%d", k,j, i);
        Add_msg_to_array_B(src, i*3) ;        
        j += k;
        sprintf(src,"k=0x%2X,j=0x%2X,i=%d", k,j, i);
        Add_msg_to_array_B(src, (i*3) + 1) ;        
        j = j << 1; 
        sprintf(src,"k=0x%2X,j=0x%2X,i=%d", k,j, i);
        Add_msg_to_array_B(src, (i*3) + 2) ;
    }
    j = j >> 1; // undo last unnecessary left shift
    sprintf(src,"k=0x%2X,j=0x%2X,i=%d", k,j, i);
    Add_msg_to_array_B(src, (i*3) + 4) ;    
    sprintf(src,"SW0-SW5= 0x%2X", j);
    New_write_display(src, 1, 0);
    if(Check_for_Mode_Button() ) {
		return FINISHED_WITH_SERVICE;
    }    
    Check_for_Up_Button();      // Release key 
    Check_for_Down_Button();    // Release key 
    Check_for_Enter_Button();   // Release key    
    return CONTINUE_SERVICE;
}

int16 Read_State_of_one_0047_input_SWs(int16 SW)
{ // SW runs from 0 to 5
    int16 i, j, k = 0;
    
    k = rdinp(GALX4_0047_in_array[SW]);

    sprintf(src,"SW%d= %2X", SW, k);
    New_write_display(src, 1, 0);
    if(Check_for_Mode_Button() ) {
		return FINISHED_WITH_SERVICE;
    }    
    Check_for_Up_Button();      // Release key 
    Check_for_Down_Button();    // Release key 
    Check_for_Enter_Button();   // Release key    
    return CONTINUE_SERVICE;
}

int16 Clear_CAN_msgs(int16 CAN_MsgNum)
{ // setoutp((void *)SEB_outputs_array[DiagIO_Map_index]);
  int16 CAN_Kase = CAN_MsgNum;
  static int16 ReturnVal = CONTINUE_SERVICE;
  static int16 FirstPass = 0;
  static int16 ViewingMsgBrd = 0;
  int16 Set_Idx = 0;
  
  if(FirstPass == 1)
  { // check for mode button
      if (Check_for_Enter_Button() || ViewingMsgBrd)
      {    
          ViewingMsgBrd = 1;    
//          ModeButton =  ButtonPressAck;  
          // gets reset in the pack IFL_IFB_debug = 0;  // to disable pack_can_out to display debug msgs to msg board B
          ReturnVal = DisplayMsgBrd_B(0); // show me the data  // FINISHED_WITH_SERVICE; 
          if(ReturnVal == FINISHED_WITH_SERVICE)
          {
              FirstPass = 0;
              ViewingMsgBrd = 0;
              return ReturnVal;	
          }
          else return ReturnVal;
      }
      else return CONTINUE_SERVICE;
  }
  
  FirstPass = 1;
  Clear_All_Boards(2); // msg board B
  switch(CAN_Kase)
  {
  	case 0: // O_IFL_COPo0_enum 
        Set_Idx = O_IFL_COPo0_enum;
  	    clroutp((void *)SEB_outputs_array[O_IFL_COPo0_enum]);
  	    break;
  	    
  	case 1: // O_IFB_COPo1_enum
        Set_Idx = O_IFB_COPo1_enum;  	
  	    clroutp((void *)SEB_outputs_array[O_IFB_COPo1_enum]);
  	    break;
  	      	    
  	case 2: // O_CUL_COPo2_enum
  	    clroutp((void *)SEB_outputs_array[O_CUL_COPo2_enum]);
  	    Set_Idx = O_CUL_COPo2_enum;
  	    break;
  	    
  	case 3: // O_CDL_COPo3_enum
  	    clroutp((void *)SEB_outputs_array[O_CDL_COPo3_enum]);
  	    Set_Idx = O_CDL_COPo3_enum;
  	    break;
  	          	      	    
  	case 4: // O_FAN_COPo4_enum
  	    clroutp((void *)SEB_outputs_array[O_FAN_COPo4_enum]);
  	    Set_Idx = O_FAN_COPo4_enum;
  	    break;
  	    
  	case 5: // O_LIG_COPo5_enum
  	    clroutp((void *)SEB_outputs_array[O_LIG_COPo5_enum]);
  	    Set_Idx = O_LIG_COPo5_enum;
  	    break;
  	      	    
  	case 6: // Selector Comm
  	    clroutp(o_SELCM);
  	    break;  	  
  	    
  	case 7: // NTS Comm
  	    clroutp(o_NTSCM);
  	    break;    
  	    
  	case 8: // Selector Comm
  	    clroutp(o_FST); // turn off the fire service stopswitch bypass
  	    break;  	  
  	    
  	case 9: // NTS Comm
  	    clroutp(o_FSTP);
  	    break;     	    	      
  	           	    
  	          	        	      	    
    default:
        break;
  }
/*  
    sprintf(src,"set_idx= %d", Set_Idx);
    New_write_display(src, 3, 0);
    IFL_IFB_debug = 1;  // to allow pack_can_out to display debug msgs to msg board B
    Check_to_manualy_Pack_and_send_data(); // send the data if Enable_CT_CAN_Ctrl == 0
    return CONTINUE_SERVICE; 
*/   
    FirstPass = 0;
    return FINISHED_WITH_SERVICE;  
}

int16 Check_to_manualy_Pack_and_send_data(void)
{ // send the data if Enable_CT_CAN_Ctrl == 0
    if(Enable_CT_CAN_Ctrl) return;
    else
    {
        pack_ctcan_out(13,8,0);	// Data has changed so pack the data
		CtcanTxBufIx ++;
		if (CtcanTxBufIx > 11)
			CtcanTxBufIx = 8;
		tx_ctcan_msg(CtcanTxBufIx);		// Check if data waiting and transmit    	
    }
}

int16 Read_1134_CAN_input(int16 channel)
{ // Read unpack data. Data from 1134 via CTCAN

    int16 i,j,k = 0;
    int16 CAN_Data = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            clr_lcd_dply();
            Clear_Msg_Board();
            ReturnVal = CONTINUE_SERVICE;
            CurrentTestPassFail =1;  // not a test
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 3; 
            timers[t_Generic_Delay] = 0;
            test_Kase = run_test;
            break;
            
        case run_test:
            if(timers[t_Generic_Delay] < Delay_Count) break;
            timers[t_Generic_Delay] = 0; 
            switch(channel) 
            {
                case 0:
                    CAN_Data = rdinp( (void *)SEB_inputs_array[i_CSS_COPi1_enum] );
                    sprintf(src,"CSS= %d",CAN_Data);
                    break;
                    
                case 1:
                    CAN_Data = rdinp( (void *)SEB_inputs_array[i_CTS_COPi2_enum] );
                    sprintf(src,"CTS= %d",CAN_Data);
                    break;  
                    
                case 2:
                    CAN_Data = rdinp( (void *)SEB_inputs_array[i_DET_COPi3_enum] );
                    sprintf(src,"DET= %d",CAN_Data);
                    break;
                    
                default:
                    break;                                       
            
            };
            New_write_display(src, 0, 0);
            
            if(Check_for_Mode_Button() ) {
                test_Kase = test_pass;
                RetestResponse = 0;
            }                      
            //  ADD CODE
            break;
                        
        case test_pass:
            test_Kase = test_done;
            break;
            
        case test_done:
            test_Kase = initialize;
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;              
            break;
                                    
        case test_failure:
            test_Kase = Retest_option;
            break;
            
        case Retest_option: // Required for Retest Option
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) test_Kase = test_done;  // do not retest
            else if(RetestResponse == 1) test_Kase = initialize; // test again
                else ReturnVal = CONTINUE_SERVICE;    
            break;           
               
        default:
            break;                
    }
    return ReturnVal;
}

/*
int16 Read_All_1134_SEB(int16 dummy)
{ // Read and display the SEB

    int16 i,j,k = 0;
    int16 CAN_Data = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            clr_lcd_dply();
            Clear_Msg_Board();
            ReturnVal = CONTINUE_SERVICE;
            CurrentTestPassFail =1;  // not a test
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 3; 
            timers[t_Generic_Delay] = 0;
            test_Kase = run_test;
            break;
            
        case run_test:
            if(timers[t_Generic_Delay] < Delay_Count) break;
            timers[t_Generic_Delay] = 0; 
            switch(channel) 
            {
                case 0:
                    CAN_Data = rdinp( (void *)SEB_inputs_array[i_CSS_COPi1_enum] );
                    sprintf(src,"CSS= %d",CAN_Data);
                    break;
                    
                case 1:
                    CAN_Data = rdinp( (void *)SEB_inputs_array[i_CTS_COPi2_enum] );
                    sprintf(src,"CTS= %d",CAN_Data);
                    break;  
                    
                case 2:
                    CAN_Data = rdinp( (void *)SEB_inputs_array[i_DET_COPi3_enum] );
                    sprintf(src,"DET= %d",CAN_Data);
                    break;
                    
                default:
                    break;                                       
            
            };
            New_write_display(src, 0, 0);
            
            if(Check_for_Mode_Button() ) {
                test_Kase = test_pass;
                RetestResponse = 0;
            }                      
            //  ADD CODE
            break;
                        
        case test_pass:
            test_Kase = test_done;
            break;
            
        case test_done:
            test_Kase = initialize;
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;              
            break;
                                    
        case test_failure:
            test_Kase = Retest_option;
            break;
            
        case Retest_option: // Required for Retest Option
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) test_Kase = test_done;  // do not retest
            else if(RetestResponse == 1) test_Kase = initialize; // test again
                else ReturnVal = CONTINUE_SERVICE;    
            break;           
               
        default:
            break;                
    }
    return ReturnVal;    
}
*/

/*
int16 Clear_CAN_msgs(int16 CAN_MsgNum)
{ // setoutp((void *)SEB_outputs_array[DiagIO_Map_index]);
  int16 CAN_Kase = CAN_MsgNum;
  
  switch(CAN_Kase)
  {
  	case 0: // O_IFL_COPo0_enum
  	    clroutp((void *)SEB_outputs_array[O_IFL_COPo0_enum]);
  	    break;
  	    
  	case 1: // O_IFB_COPo1_enum
  	    clroutp((void *)SEB_outputs_array[O_IFB_COPo1_enum]);
  	    break;
  	      	    
  	case 2: // O_CUL_COPo2_enum
  	    clroutp((void *)SEB_outputs_array[O_CUL_COPo2_enum]);
  	    break;
  	    
  	case 3: // O_CDL_COPo3_enum
  	    clroutp((void *)SEB_outputs_array[O_CDL_COPo3_enum]);
  	    break;
  	          	      	    
  	case 4: // O_FAN_COPo4_enum
  	    clroutp((void *)SEB_outputs_array[O_FAN_COPo4_enum]);
  	    break;
  	    
  	case 5: // O_LIG_COPo5_enum
  	    clroutp((void *)SEB_outputs_array[O_LIG_COPo5_enum]);
  	    break;
  	          	        	      	    
    default:
        break;
  }
    
    return FINISHED_WITH_SERVICE;  
}

*/

/*

    O_IFL_COPo0_enum, // 0
    O_IFB_COPo1_enum,
    O_CUL_COPo2_enum,
    O_CDL_COPo3_enum,
    O_FAN_COPo4_enum,
    O_LIG_COPo5_enum, // 5
    
setoutp((void *)SEB_outputs_array[O_IFL_COPo0_enum]); 
setoutp((void *)SEB_outputs_array[Diag_IO_index]);   


int16 Toggle_4_0047_input(int16 channel)
{ //// DOES NOT WORK BECAUSE TRYING TO SET AN INPUT CHANNEL
    static int16 toggle_state[MAX_0047_INDEX];

    if (channel < 0 || channel > 5)
        return FINISHED_WITH_SERVICE; // error

    sprintf(src, "chnel=%d,state=%d", channel, toggle_state);
    Log_an_event(src);
    Add_msg_to_array_B(src, channel);
    //    toggle_state = !toggle_state;
    if (toggle_state[channel] = !toggle_state[channel])
        setoutp(GALX4_0047_in_array[channel]); //  LED from 'SWi0' - 'LDO11'
    else
        clroutp(GALX4_0047_in_array[channel]);
    return FINISHED_WITH_SERVICE;
}
*/