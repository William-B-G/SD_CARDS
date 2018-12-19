// Bds_1137_1139.c

// Log_an_event("outputs not off     ");  if( () () )
#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1137_1139.h"
#include "GALX_1121_global.h"

#define GRtest_signal_index 30  // 25
#define NINETEEN_DOT_2 354 // 19.2 kb
#define  c_grcan_HCB_1	 15		  // Hall Call Board 1
#define BAD 0
#define GOOD 1
#define LED_ON 0
#define LED_OFF 1
#define HALL_OFF 0
#define HALL_ON 1
#define MAX_BDS_ABLE_TO_TEST 3  // max for 1137 or 1139 are 3
  // since the fixture only has only one 1107AN I/O board
#define GALx1137  0
#define GALx1139  1


#define Test_GALx1137_index 12
void *Test_GALx1137_out_array[Test_GALx1137_index]= {
    o_MRo0,
    o_MRo1,
    o_MRo2,
    o_MRo3,
    o_MRo4,
    o_MRo5,
    o_MRo6,
    o_MRo7,
    o_MRo8,
    o_MRo9,
    o_MRo10,
    o_MRo11
};

void *Test_GALx1137_in_array[Test_GALx1137_index]= {
    i_MRi0,
    i_MRi1,
    i_MRi2,
    i_MRi3,
    i_MRi4,
    i_MRi5,
    i_MRi6,
    i_MRi7,
    i_MRi8,
    i_MRi9,
    i_MRi10,
    i_MRi11
};

int16 Hall_Outputs = 0;
int16 Hall_Output_1 = 0;
int16 Hall_Output_2 = 0;
int16 Either_Hall_Outputs = 0;
uint8 UnpkByte[3];
int16 LoopCycles = 0;
int16 d0 = 5;
int16 d1 = 2;
int16 d2 = 3;

int16 Case_16_data = 0;
int16 Case_14_data = 0;
int16 CaseNumber = 0;
int16 Raw_data = 0;
int16 board_condition = BAD;
int16 GRC_Rgix = 0;
uint8 Hall_Test_Data = 0;  // Up, Down arrow data.  Up arrow bit 4, Down arrow bit 3.  Used in hlcom.c
int16 Lant_Dwn, Lant_Up = 0;
bool Multi_board_test = FALSE;


int16 Get_Srl_Hall_Station_Pkg(int16 dummy);
int16 Determine_1137_Output_Status(int16 dummy);
int16 Send_Srl_Hall_Station_Pkg(int16 pkg_num, int16 msg);
void Show_1139_Tasks(void);
void Over_ride_pack_data(int16 d0,int16 d1,int16 d2);
extern void pack_grcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req);
extern void tx_grcan_msg(int16 TxBufIx);
extern int16 GrcanTxBufIx;
extern bool grcan_tx_complete[];

enum {
    Init_GRCAN_Test,
    Wait_for_Operator_Response,
    Send_GRCAN_Signal,
    Check_GRCAN_Signal,
    Determine_Pass_Fail,
    GRCAN_test_Fail,
    GRCAN_test_Pass,
    GRCAN_test_Done,
    Send_Final,
    Monitor_unpk,
    Set_Hall_OFF_A,
    Set_Hall_OFF_B,
    Set_Hall_ON,
    Set_Hall_Chan_1_ON,
    Set_Hall_Chan_2_ON,
    Reset_Zero_A,
    Reset_Zero_B,
    Reset_Ten_A,
    Verify_OFF_A,
    Verify_ON,
    Send_24V_to_1137_chan_1,
    Send_24V_to_1137_chan_2,
    Send_0V_to_1137,
    Verify_Read_10_A,
    Verify_Read_10_B,
    Verify_Read_0,
    Retest_option  // Required for Retest Option    
};

enum
{
    initialize,
    run_test,
    test_pass,
    test_done,
    lant_off,
    lant_off_B,
    lant_u,
    lant_d,
    lant_u_and_d,
    test_failure,
    Test_Retest_option,    
};

int16 Use_CT_CAN(int16 dummy)
{
  int16 i = 0;
    Test_GALx1137_out_array[0] = o_CTo0 ;
    Test_GALx1137_out_array[1] = o_CTo1 ;
    Test_GALx1137_out_array[2] = o_CTo2 ;
    Test_GALx1137_out_array[3] = o_CTo3 ;
    Test_GALx1137_out_array[4] = o_CTo4 ;
    Test_GALx1137_out_array[5] = o_CTo5 ;
    Test_GALx1137_out_array[6] = o_CTo6 ;
    Test_GALx1137_out_array[7] = o_CTo7 ;
    Test_GALx1137_out_array[8] = o_CTo8 ;
    Test_GALx1137_out_array[9] = o_CTo9 ;
    Test_GALx1137_out_array[10] = o_CTo10;
    Test_GALx1137_out_array[11] = o_CTo11;
    
    Test_GALx1137_in_array[0] = i_CTi0 ;
    Test_GALx1137_in_array[1] = i_CTi1 ;
    Test_GALx1137_in_array[2] = i_CTi2 ;
    Test_GALx1137_in_array[3] = i_CTi3 ;
    Test_GALx1137_in_array[4] = i_CTi4 ;
    Test_GALx1137_in_array[5] = i_CTi5 ;
    Test_GALx1137_in_array[6] = i_CTi6 ;
    Test_GALx1137_in_array[7] = i_CTi7 ;
    Test_GALx1137_in_array[8] = i_CTi8 ;
    Test_GALx1137_in_array[9] = i_CTi9 ;
    Test_GALx1137_in_array[10] = i_CTi10;
    Test_GALx1137_in_array[11] = i_CTi11;
        
    return FINISHED_WITH_SERVICE;   
}

// Req_packet_14

int16 Debug_out_01(char Letter, int16 Kounter_2, int16 Found_Ten)
{
    sprintf(&LCD_Display[0][0],"%d",Chan_1_1137);
    sprintf(&LCD_Display[0][8],"%d",Chan_2_1137);        
    sprintf(&LCD_Display[1][0],"%c: %02i", Letter, CaseNumber);
    sprintf(&LCD_Display[1][8],"%02i",Raw_data);
    sprintf(&LCD_Display[1][14],"%d",Kounter_2++);
    sprintf(&LCD_Display[2][0],"%d",Found_Ten); 
    sprintf(&LCD_Display[2][8],"%d",GRC_Rgix);
    sprintf(&LCD_Display[2][14],"%d",grc_rgix); 
    /*        
            sprintf(&LCD_Display[2][0],"%02i",Case_14_data);
            sprintf(&LCD_Display[3][0],"%02i",Case_16_data);  
    */  
}

int16 Req_packet_14(int16 msg)
{
    pack_grcan_out(14,c_grcan_HCB_1,msg);
//    Send_Srl_Hall_Station_Pkg(14, msg);
    return FINISHED_WITH_SERVICE;   
}

int16 Reset_14_and_16(int16 dummy)
{
    Case_16_data = Case_14_data = 0;
    return FINISHED_WITH_SERVICE;   
}

int16 Determine_1137_Output_Status(int16 test_Loop)
{
    Hall_Output_1 = rdinp(Test_GALx1137_in_array[test_Loop * 2]);
    Hall_Output_2 = rdinp(Test_GALx1137_in_array[(test_Loop * 2) + 1]);
    Either_Hall_Outputs = Hall_Output_1 | Hall_Output_2;
    Hall_Outputs =  Hall_Output_1 & Hall_Output_2;
    sprintf(src,"a=%d; b=%d; c=%d; q=%d",test_Loop * 2, (test_Loop * 2) + 1,Hall_Output_1, Hall_Output_2);
    Log_an_event(src);
    Log_an_event_C(src);  
    
    sprintf(src,"E=%d; H=%d",Either_Hall_Outputs,Hall_Outputs);
    Log_an_event(src);
    Log_an_event_C(src);      
//    if(Hall_Outputs ==1)
//      SIU.GPDO[Z6_LED].R = LED_ON;
//        else SIU.GPDO[Z6_LED].R = LED_OFF;   
    return FINISHED_WITH_SERVICE; 
}

int16 Send_Hall_Reset(int16 msg)
{    
    Send_Srl_Hall_Station_Pkg(15, msg);
/*    
    Hall_Output_1 = rdinp(Test_GALx1137_in_array[0]);
    Hall_Output_2 = rdinp(Test_GALx1137_in_array[1]);
    Either_Hall_Outputs = Hall_Output_1 | Hall_Output_2;
    Hall_Outputs =  Hall_Output_1 & Hall_Output_2;
    if(Hall_Outputs ==1)
      SIU.GPDO[Z6_LED].R = LED_ON;
        else SIU.GPDO[Z6_LED].R = LED_OFF;
*/        
    return FINISHED_WITH_SERVICE;
}

int16 Display_channels(int16 command)
{
    if(command == 1)
    {
        if(rdinp(Test_GALx1137_in_array[0]))
          Set_or_Clear_1137_Input(0x0401);
        if(rdinp(Test_GALx1137_in_array[1]))
          Set_or_Clear_1137_Input(0x0801);        
        
    }
    else 
    { // turn off 1107AN channels 8 & 9
        Set_or_Clear_1137_Input(0x0C00);   
    }
       
}

int16 Send_Hall_Data(int16 msg)
{    
    Send_Srl_Hall_Station_Pkg(13, msg);
    return FINISHED_WITH_SERVICE;
}

int16 Set_or_Clear_1137_Input(int16 Set_Clear)
{   
  int16 chan_1 = Set_Clear & 0x0100;
  int16 chan_2 = Set_Clear & 0x0200;
  int16 chan_3 = Set_Clear & 0x0400;
  int16 chan_4 = Set_Clear & 0x0800;  
  int16 chan_5 = Set_Clear & 0x1000;
  int16 chan_6 = Set_Clear & 0x2000;
  
    if(Set_Clear & 0x0f)
    {
        if(chan_1)setoutp(Test_GALx1137_out_array[6]); // Returns 0x08
        if(chan_2)setoutp(Test_GALx1137_out_array[7]);  // Returns 0x02
        if(chan_3)setoutp(Test_GALx1137_out_array[8]); // Returns 0x08
        if(chan_4)setoutp(Test_GALx1137_out_array[9]);  // Returns 0x02 
        if(chan_5)setoutp(Test_GALx1137_out_array[10]);
        if(chan_6)setoutp(Test_GALx1137_out_array[11]);       
 // Returns 0x0A if both are set      
    }
    else
    {
        if(chan_1)clroutp(Test_GALx1137_out_array[6]);
        if(chan_2)clroutp(Test_GALx1137_out_array[7]); 
        if(chan_3)clroutp(Test_GALx1137_out_array[8]);
        if(chan_4)clroutp(Test_GALx1137_out_array[9]);  
        if(chan_5)clroutp(Test_GALx1137_out_array[10]);
        if(chan_6)clroutp(Test_GALx1137_out_array[11]);                
    }

    return FINISHED_WITH_SERVICE;
}

int16 Set_or_Clear_All(int16 Set_1_or_clear_0)
{
  int16 i = 0;
    for(i=6; i<12; i++)
    {
        if(Set_1_or_clear_0)
        {
          setoutp(Test_GALx1137_out_array[i]);
//          setoutp(Test_GALx1137_in_array[i]);   
        }
        else
        {
          clroutp(Test_GALx1137_out_array[i]);
//          clroutp(Test_GALx1137_in_array[i]);             
        }
    }
    return FINISHED_WITH_SERVICE;    
}


int16 Hall_Station_CAN_Data(int16 Data)
{
    int16 returnval = CONTINUE_SERVICE;
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    New_write_display("Up/Dwn to chng data ",0,0);
    New_write_display("MODE to finish      ",1,0);
    New_write_display("                    ",2,0);
    sprintf(src,"data = %d",d0);
    New_write_display(src,2,0);
    if(Check_for_Mode_Button() ) returnval =  FINISHED_WITH_SERVICE;
    if(Check_for_Up_Button() )
    {
//        SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;
        d0++;
        returnval =  CONTINUE_SERVICE;
    }
    if(Check_for_Down_Button() )
    {
        if(d0)d0--;
        returnval = CONTINUE_SERVICE;
    }
    return returnval;    
}

int16 Qty_113x_bds(int16 Test_and_Qty)
{
    enum
    {
       initialize,
       review_Qty,
       run_test,
       done_with_this_one,
       test_pass,
       display_results,
       waiting_on_dismisal,
       test_done,
       test_failure,
       Retest_option, 
    };
    int16 i,j,k = 0;
    int16 The_result = 0;
    int16 The_Test = (Test_and_Qty & 0x0f00) >> 8;
    int16 Qty = Test_and_Qty & 0x0ff;
    int16 Direction = 0;
//    int16 Direction = (Test_and_Qty & 0xf000) >> 12;
    static int16 test_Loop = 0;
    static int16 current_Loop = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 LocalTestPassFail[3];
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 0; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            clr_lcd_dply();
            Clear_Msg_Board();
            ReturnVal = CONTINUE_SERVICE;
            //CurrentTestPassFail =0;  // fail
            for(i=0; i< MAX_BDS_ABLE_TO_TEST; i++)
            {
                LocalTestPassFail[i] = -1; // -1 = not yet tested  
            }
            if(The_Test == GALx1139)Direction = consB[27] & 0x01;
              else Direction = consB[27] & 0x10; 
            test_Loop = (Direction) ? MAX_BDS_ABLE_TO_TEST - 1 : 0 ;
            current_Loop = 1;
            sprintf(src,"A=%d; B=%d; C=%d; Q=%d",current_Loop, Direction,The_Test, Qty);
//            Log_an_event(src);
            Log_an_event_C(src);
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            test_Kase = review_Qty;
            break;
            
        case review_Qty:
            if(current_Loop <= Qty) test_Kase = run_test;
            else test_Kase = display_results;
            if(Direction) sprintf(src,"Dec, tL=%d, CL=%d",test_Loop,current_Loop);
              else sprintf(src,"Inc, tL=%d, CL=%d",test_Loop,current_Loop);
            Log_an_event_C(src);            
            break;
                        
        case run_test: 
            Multi_board_test = TRUE;   
            switch(The_Test) 
            {            
                case 0:                
                    The_result = Hall_Station_CAN(test_Loop);
                    break;
            
                case 1:
                    The_result = HALL_Lantern_RS485(test_Loop);
                    break;            
            
                case 2:
                    The_result = DoNothing(0);                
                    break;
            
                case 3:
                    The_result = DoNothing(0);                
                    break;
                        
                default:
                    The_result = DoNothing(0);                
                    break;                                              
            }
            if(The_result == CONTINUE_SERVICE)
            {
                ReturnVal = The_result;                   
            }
            else test_Kase = done_with_this_one;
            //  ADD CODE
            break;
                        
        case done_with_this_one:
            LocalTestPassFail[test_Loop] = CurrentTestPassFail;
            current_Loop++;
            if(Direction) test_Loop-- ;
              else test_Loop++ ;             
            test_Kase = review_Qty;
            break;
                                    
        case test_pass:
            test_Kase = display_results;
            break;
                                                
        case display_results:
            clr_lcd_dply();
  
            for(i=0; i< MAX_BDS_ABLE_TO_TEST; i++)
            {
              if(LocalTestPassFail[i] == 1) 
              { // pass
                sprintf(src,"Board %d PASS", i+1);
                New_write_display(src,i,0);
                setoutp(Test_GALx1137_out_array[(i * 2) + 6]);
                setoutp(Test_GALx1137_out_array[(i * 2) + 7]);
                
              }
              else if(LocalTestPassFail[i] == 0)
              { // fail
                sprintf(src,"Board %d failed", i+1);
                New_write_display(src,i,0);
                clroutp(Test_GALx1137_out_array[(i * 2) + 6]);
                clroutp(Test_GALx1137_out_array[(i * 2) + 7]);                
              }
            }
            New_write_display("Mode or Enter to ext",3,0);
            test_Kase = waiting_on_dismisal;
            break;      
            
        case waiting_on_dismisal:
            if ( Check_for_Mode_Button() || Check_for_Enter_Button() )
            {
                RetestResponse = 0;
                clr_lcd_dply();
                Set_or_Clear_All(0);
                test_Kase = test_done;
                break;                
            }
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

int16 Hall_Station_CAN(int16 test_Loop)
{
    static int16 GRCAN_TestCase=Init_GRCAN_Test;
    int16 Delay_Count=test_halfsec_delay;
    static int16 Counter = 0;
    static int16 StartUpMsgSent = 0;
    static int16 Kounter_2 = 0;
    static int16 Found_Ten = 0;
    static int16 Found_Zero = 0;
    static int16 index=0;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
    int16 i=0;
    int16 target = 0;
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (GRCAN_TestCase) {
    case Init_GRCAN_Test:
        CurrentTestPassFail =0;  // fail
        First_Half_PassFail = 0;
        Chan_1_1137 = 0;
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option      
        if(Multi_board_test == FALSE) Clear_Msg_Board(); //  Clear_Log_Buffer();  
        clr_lcd_dply();
        Set_or_Clear_1137_Input(0xff00); // clear
        Case_14_data = 0;
        Case_16_data = 0;
        board_condition = BAD;
        init_grcan_bus();
        Counter = 0;
        Kounter_2 = 0;
        Found_Ten = 0;
        Found_Zero = 0;
        SIU.GPDO[Z6_LED].R = 1; // led 6 off  
        Send_Hall_Reset(10);    
        timers[t_Generic_Delay]=0;
        if( !StartUpMsgSent++ )
        {
            New_write_display("Set dip switch addr ",0,0);
            New_write_display("to 1 before applying",1,0); 
            New_write_display("pwr. Enter when     ",2,0);
            New_write_display("ready to continue   ",3,0);      

            GRCAN_TestCase = Wait_for_Operator_Response;  // Send_GRCAN_Signal;            
        }
        else
        {
            New_write_display("Now sending cmd to  ",0,0);
            New_write_display("set outputs high and",1,0); 
            New_write_display("verifying with      ",2,0);
            New_write_display("GALx 1107AN         ",3,0);        
            GRCAN_TestCase = Set_Hall_OFF_A;    
        }        
        break;        

    case Wait_for_Operator_Response:
        if ( Check_for_Mode_Button() ) 
        {
            StartUpMsgSent = 0;
            GRCAN_TestCase = Init_GRCAN_Test;
            ReturnVal = FINISHED_WITH_SERVICE; 
            break;   
        }   
        if ( Check_for_Enter_Button() )
        { 
            clr_lcd_dply();
            Send_Hall_Reset(10); // Send reset ten 
            New_write_display("Now sending cmd to  ",0,0);
            New_write_display("set outputs high and",1,0); 
            New_write_display("verifying with      ",2,0);
            New_write_display("GALx 1107AN         ",3,0);     
            timers[t_Generic_Delay]=0;
            GRCAN_TestCase = Set_Hall_OFF_A;   
        }
        break;
        
    case Set_Hall_OFF_A:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Send_Hall_Data(0); // set it off
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Reset_Zero_A;
        break;

    case Reset_Zero_A:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Send_Hall_Reset(0); // Send reset zero
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Verify_OFF_A;        
        break;
           
    case Verify_OFF_A:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Determine_1137_Output_Status(test_Loop);
        if(Either_Hall_Outputs == HALL_OFF) GRCAN_TestCase = Set_Hall_ON;
          else 
          {
              Log_an_event("outputs not off     ");
              GRCAN_TestCase = Retest_option; 
          } 
        timers[t_Generic_Delay]=0;    
        break;

    case Set_Hall_ON:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Send_Hall_Data(5); // Hall on
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Reset_Ten_A;     
        break;       

    case Reset_Ten_A:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Send_Hall_Reset(10);
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Verify_ON;     
        break;          

    case Verify_ON:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Determine_1137_Output_Status(test_Loop);
        if(Hall_Outputs == HALL_ON) 
        {
            Send_Hall_Data(4); // Hall chan 1 on
            GRCAN_TestCase = Set_Hall_Chan_1_ON ; // Set_Hall_OFF_B;  // Set_Hall_Chan_1_ON    
        }
          else 
          {
              if(Hall_Output_1 == 0)Log_an_event("IO_1_OUT not ON     ");
              if(Hall_Output_2 == 0)Log_an_event("IO_2_OUT not ON     ");
              GRCAN_TestCase = Retest_option;
          }
        timers[t_Generic_Delay]=0;     
        break;    

    case Set_Hall_Chan_1_ON:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if(rdinp(Test_GALx1137_in_array[(test_Loop) + 1]))
        { // short
            Log_an_event("IO_2_OUT shorted    ");
            GRCAN_TestCase = Retest_option;   
        }
        else
        {
            Send_Hall_Data(1); // Hall chan 2 on
            GRCAN_TestCase = Set_Hall_Chan_2_ON ;            
        }
        timers[t_Generic_Delay]=0;     
        break;
        

    case Set_Hall_Chan_2_ON:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if(rdinp(Test_GALx1137_in_array[test_Loop]))
        { // short
            Log_an_event("IO_1_OUT shorted    ");
            GRCAN_TestCase = Retest_option;   
        }
        else
        {
            //Send_Hall_Data(0); // Hall off
            GRCAN_TestCase = Set_Hall_OFF_B ;            
        }
        timers[t_Generic_Delay]=0;     
        break;
                
    case Set_Hall_OFF_B:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        New_write_display("OK, check. Now setng",0,0);
        New_write_display("chan 1 high and chek",1,0); 
        New_write_display("that 1137 sends back",2,0);
        New_write_display("proper msg.         ",3,0);          
        First_Half_PassFail = 1; // pass
        Send_Hall_Data(0); // set it off
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Reset_Zero_B;     
        break;           

    case Reset_Zero_B:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Send_Hall_Reset(0); // Send reset zero
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Send_24V_to_1137_chan_1;        
        break;
        
    case  Send_24V_to_1137_chan_1:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        target = 0x0100 << (test_Loop * 2);
        Set_or_Clear_1137_Input(target | 0x0001); // set channel 1
        timers[t_Generic_Delay]=0;
        CaseNumber = 0;
        Kounter_2 = 0;
        if(consB[21] == 128) clr_lcd_dply();  
        GRCAN_TestCase = Verify_Read_10_A;        
        break;        
               
    case Verify_Read_10_A: // 'A'
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay]=0;        
        Get_Srl_Hall_Station_Pkg(0); // unpack
        if(Raw_data == 8) Found_Ten++;
          else Found_Ten = 0;        
        if(consB[21] == 128)
        {
            Debug_out_01('A', Kounter_2, Found_Ten);
            Kounter_2++;
/*            
            sprintf(&LCD_Display[0][0],"%d",Chan_1_1137);
            sprintf(&LCD_Display[0][8],"%d",Chan_2_1137);        
            sprintf(&LCD_Display[1][0],"A: %02i",CaseNumber);
            sprintf(&LCD_Display[1][8],"%02i",Raw_data);
            sprintf(&LCD_Display[1][14],"%d",Kounter_2++);
            sprintf(&LCD_Display[2][0],"%d",Found_Ten); 
            sprintf(&LCD_Display[2][8],"%d",GRC_Rgix);
            sprintf(&LCD_Display[2][14],"%d",grc_rgix); // grc_rgix
          
            sprintf(&LCD_Display[2][0],"%02i",Case_14_data);
            sprintf(&LCD_Display[3][0],"%02i",Case_16_data);  
    */             
        }   
        Raw_data = 33;          

        if(consB[21] == 128)
        { // For debug
            if(Check_for_Enter_Button() )
            {
                Send_Hall_Reset(Raw_data); // ack to GALx1137
                if(Found_Ten > 2) Chan_1_1137 = 1;  // pass for channel 1
                  else Log_an_event("IO_1_IN NG");
                Found_Ten = 0;
                Kounter_2 = 0;
                Set_or_Clear_1137_Input(0xff00); // clear
                target = 0x0100 << (test_Loop * 2) + 1 ;
                Set_or_Clear_1137_Input(target | 0x0001);
                // Set_or_Clear_1137_Input(0x0201); // set channel 2                
                GRCAN_TestCase= Verify_Read_10_B;  //Send_0V_to_1137;
                break;    
            }            
        }
        else
        { // normal
            if(Check_for_Enter_Button() || Kounter_2++ > 8 || Found_Ten > 2 )
            {
                Send_Hall_Reset(Raw_data); // ack to GALx1137
                if(Found_Ten > 2) Chan_1_1137 = 1;  // pass
                  else Log_an_event("IO_1_IN NG");
                Found_Ten = 0;
                Kounter_2 = 0;
                Set_or_Clear_1137_Input(0xff00); // clear
                target = 0x0100 << (test_Loop * 2) + 1 ;
                Set_or_Clear_1137_Input(target | 0x0001);                
                // Set_or_Clear_1137_Input(0xf201); // set channel 2
                GRCAN_TestCase= Verify_Read_10_B;  // Send_0V_to_1137;
                break;    
            }             
        }             
        break;

              
    case Verify_Read_10_B: // 'B'
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay]=0;        
        Get_Srl_Hall_Station_Pkg(0); // unpack
        if(Raw_data == 2) Found_Ten++;
          else Found_Ten = 0;        
        if(consB[21] == 128)
        {
            Debug_out_01('B', Kounter_2, Found_Ten);
            Kounter_2++;  
/*                  
            sprintf(&LCD_Display[0][0],"%d",Chan_1_1137);
            sprintf(&LCD_Display[0][8],"%d",Chan_2_1137);
            sprintf(&LCD_Display[1][0],"B: %02i",CaseNumber);
            sprintf(&LCD_Display[1][8],"%02i",Raw_data);
            sprintf(&LCD_Display[1][14],"%d",Kounter_2++);
            sprintf(&LCD_Display[2][0],"%d",Found_Ten);
            sprintf(&LCD_Display[2][8],"%d",GRC_Rgix); // grc_rgix
            sprintf(&LCD_Display[2][14],"%d",grc_rgix); // grc_rgix Chan_1_1137
          
            sprintf(&LCD_Display[2][0],"%02i",Case_14_data);
            sprintf(&LCD_Display[3][0],"%02i",Case_16_data);  
    */             
        }   
        Raw_data = 44;           

        if(consB[21] == 128)
        {
            if(Check_for_Enter_Button() )
            {
                Send_Hall_Reset(Raw_data); // ack to GALx1137
                if(Found_Ten > 2) Chan_2_1137 = 1;  // pass for channel 2
                  else Log_an_event("IO_2_IN NG");
                Found_Ten = 0;
                Kounter_2 = 0;
                GRCAN_TestCase= Send_0V_to_1137;
                break;    
            }            
        }
        else
        {
            if(Check_for_Enter_Button() || Kounter_2++ > 8 || Found_Ten > 2 )
            {
                Send_Hall_Reset(Raw_data); // ack to GALx1137
                if(Found_Ten > 2) Chan_2_1137 = 1;  // pass
                  else Log_an_event("IO_2_IN NG");
                Found_Ten = 0;
                Kounter_2 = 0;
                GRCAN_TestCase= Send_0V_to_1137;
                break;    
            }             
        }             
        break;

    case Send_0V_to_1137:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Set_or_Clear_1137_Input(0xff00); // clear
        CaseNumber = 0;
        Kounter_2 = 0;
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Verify_Read_0;        
        break;

    case Verify_Read_0: // 'B'
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Get_Srl_Hall_Station_Pkg(0); // unpack
              
        timers[t_Generic_Delay]=0;
        if(Raw_data == 0) Found_Zero++;
          else Found_Zero = 0;  
        Raw_data = 1; // reset it so we know that it is actually read in unpack      
        if(Check_for_Enter_Button() || Kounter_2++ > 8 || Found_Zero > 2  )
        {
            Send_Hall_Reset(Raw_data); // ack to GALx1137 
            if(Chan_1_1137 && Chan_2_1137)
              GRCAN_TestCase=GRCAN_test_Pass;
            else
            {
                Log_an_event("Did not receive msg ");
                GRCAN_TestCase = Retest_option;
            }
            Chan_1_1137 = 0; 
            Chan_2_1137 = 0;                
        }        
        break;                                                                  
                       
    case GRCAN_test_Pass:
        CurrentTestPassFail = 1; // pass
        clr_lcd_dply();
        New_write_display("Hall Station Test   ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0); 
        GRCAN_TestCase = GRCAN_test_Done;        
        break;
        
    case GRCAN_test_Done:
        GRCAN_TestCase = Init_GRCAN_Test;
        Multi_board_test = FALSE; 
        Send_Hall_Reset(0); // final ack to GALx1137
        SIU.GPDO[Z6_LED].R = 1; // led 6 off
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
        break;
                
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) GRCAN_TestCase = GRCAN_test_Done;
        else if(RetestResponse == 1) GRCAN_TestCase = Init_GRCAN_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE;  
        break;           

    case GRCAN_test_Fail:
        CurrentTestPassFail = 0; // fail
        clr_lcd_dply();
        New_write_display("Hall Station Test   ",0,0);
        New_write_display("TEST FAILED         ",1,0); 
        GRCAN_TestCase = GRCAN_test_Done;     
//-    	clr_lcd_dply();
//        New_write_display(TEST_GRCAN_Fault_Display[0],2,0);
        
//      GALX1121_Fault_Display_ADDIN_Function();
//      Log_an_event(TEST_GRCAN_Fault_Display[1]);
//        GRCAN_TestCase = Retest_option;
        break;

    default:
        GRCAN_TestCase=Init_GRCAN_Test;
        ReturnVal = DEFAULT;
        break;
    }
    return ReturnVal; // Required for Retest Option
}

int16 Reset_1137_board(int16 Kase)
{
    Send_Srl_Hall_Station_Pkg(Kase,d0); // to device - Board output
    return FINISHED_WITH_SERVICE;
}

int16 Send_Hall_Lantern_Floor(int16 Data)
{   
    Hall_Test_Data = (int8)Data;
    hl_com(); // Tx to Hall Lantern via RS485
    return FINISHED_WITH_SERVICE; 
}

void Read_Lanterns(int16 current_Loop)
{
    Lant_Dwn = rdinp(Test_GALx1137_in_array[current_Loop * 2]);
    Lant_Up = rdinp(Test_GALx1137_in_array[(current_Loop * 2) + 1]);
}

int16 HALL_Lantern_RS485(int16 current_Loop)
{ // SET BAUD RATE TO 19 dot 2. Dip switch setting must be: = floor # 
  // (minimum is 1) & TERM ON . Floor is set via consB[23].
  // send out data from GALX-1138N and use 1107AN I/O bd to verify outputs. 
  // GALX-1139 Hall Lantern
  // SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
    int16 i,j,k = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    static int16 Hall_Lantern_Loop_Counter = 0;   
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            clr_lcd_dply();
            New_write_display("Executing...        ",1,0);
            if(Multi_board_test == FALSE) Clear_Msg_Board();  // Clear_Log_Buffer();            
//            Clear_Msg_Board();
//            Log_an_event("Test Fault Msg");
//            Show_1139_Tasks();
            SIU.GPDO[Z6_LED].R = 1;  // led 6 off
            ReturnVal = CONTINUE_SERVICE;
            CurrentTestPassFail =0;  // fail
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 5; // 1/2 second
            timers[t_Generic_Delay] = 0;
            Set_RS485_Baud(NINETEEN_DOT_2);
            Hall_Lantern_Data_A = 0xff;
            Hall_Lantern_Data_B = 0xff;
            // Hall_Lantern_Select
            Send_Hall_Lantern_Floor(0);  // up, down OFF
            sprintf(src,"init:cL=%d",current_Loop);
            Log_an_event(src);            
            test_Kase = lant_off; // not 'run_test'
            break;
            
        case lant_off:
            if(timers[t_Generic_Delay]<Delay_Count) break;
            timers[t_Generic_Delay] = 0;
            Read_Lanterns(current_Loop);
            sprintf(src,"lf:cL=%d;T0=%d;T1=%d",current_Loop, Lant_Dwn, Lant_Up);
            Log_an_event(src);            
            if( (Lant_Dwn == 0 ) && ( Lant_Up == 0  ) )
            {
                Send_Hall_Lantern_Floor(0x080); //  turn up arrow on
                test_Kase = lant_u;                              
            }
            else
            {  
                Log_an_event("A - channel(s) on   ");
                test_Kase = Test_Retest_option;    
            }
            break;
            
        case lant_off_B:
            if(timers[t_Generic_Delay]<Delay_Count) break;
            timers[t_Generic_Delay] = 0;
            LCD_Display[1][14] = '.';
            Read_Lanterns(current_Loop);
            sprintf(src,"loB:cL=%d;T0=%d;T1=%d",current_Loop, Lant_Dwn, Lant_Up);
            Log_an_event(src); 
            if( (Lant_Dwn == 0 ) && ( Lant_Up == 0  ) )            
            {
                test_Kase = test_pass;                              
            }
            else
            {  
                Log_an_event("E - channel(s) on   ");
                test_Kase = Test_Retest_option;    
            }
            break;            
                        
        case lant_u:
            if(timers[t_Generic_Delay]<Delay_Count) break;
            timers[t_Generic_Delay] = 0;
            LCD_Display[1][11] = '.';
            Read_Lanterns(current_Loop);
            sprintf(src,"lu:cL=%d;T0=%d;T1=%d",current_Loop, Lant_Dwn, Lant_Up);
            Log_an_event(src);
            if( Lant_Dwn == 0 && Lant_Up == 1 )
            {
                Send_Hall_Lantern_Floor(0x040); //  turn down arrow on
                test_Kase = lant_d;              
            }
            else
            {   sprintf(src,"t0= %d; t1= %d", Lant_Dwn, Lant_Up);
                Log_an_event(src);
                Log_an_event("B - lant_u not on   ");
                Log_an_event("B - or lant_d  on   ");
                test_Kase = Test_Retest_option;                 
            }
            break;
            
        case lant_d:
            if(timers[t_Generic_Delay]<Delay_Count) break;
            timers[t_Generic_Delay] = 0;
            LCD_Display[1][12] = '.';
            Read_Lanterns(current_Loop);
            sprintf(src,"ld:cL=%d;T0=%d;T1=%d",current_Loop, Lant_Dwn, Lant_Up);
            Log_an_event(src); 
            if( Lant_Dwn == 1 && Lant_Up == 0 )           
            {
                Send_Hall_Lantern_Floor(0x0c0); //  turn Up and Down on
                test_Kase = lant_u_and_d;              
            }
            else
            {
                Log_an_event("C - lant_u     on   ");
                Log_an_event("C - or lant_d not on");
                test_Kase = Test_Retest_option;                 
            }
            break;                                    
            
        case lant_u_and_d:
            if(timers[t_Generic_Delay]<Delay_Count) break;
            timers[t_Generic_Delay] = 0;
            LCD_Display[1][13] = '.';
            Read_Lanterns(current_Loop);
            sprintf(src,"lud:cL=%d;T0=%d;T1=%d",current_Loop, Lant_Dwn, Lant_Up);
            Log_an_event(src); 
            if( Lant_Dwn == 1 && Lant_Up == 1 )           
            {
                Send_Hall_Lantern_Floor(0x00); //  turn both off
                test_Kase = lant_off_B;              
            }
            else
            {
                Log_an_event("D - lant_u not on   ");
                Log_an_event("D - or lant_d not on");
                test_Kase = Test_Retest_option;                 
            }
            break;  
                        
        case test_pass:
            RetestResponse = 1;
            CurrentTestPassFail = 1; // pass
            clr_lcd_dply();
            New_write_display("Hall Lantern Test   ",0,0);
            New_write_display("TEST PASSED         ",1,0);
            New_write_display("GOOD JOB            ",2,0);            
            test_Kase = test_done;
            break;
            
        case test_done:
            test_Kase = initialize;
            Multi_board_test = FALSE;
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY; 
            SIU.GPDO[Z6_LED].R = 1;  // led 6 off             
            break;
                                    
        case test_failure:
            test_Kase = Test_Retest_option;
            break;
            
        case Test_Retest_option: // Required for Retest Option
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
   
  
    return 0;
}

int16 Unpk_Hall_Station(int16 dummy)
{
   Get_Srl_Hall_Station_Pkg(0);
   return FINISHED_WITH_SERVICE; 
}

int16 Get_Srl_Hall_Station_Pkg(int16 dummy)
{ // serial_grc.dev[iodev_ix].iob[0]
    int16 i = 0;
    
        unpack_grcan_in();
        _disable();
        for(i=0;i<3;i++)
        {
            UnpkByte[i] = serial_grc.dev[0].iob[i]; // grcan_rx[grc_rgix].data[0];
        }
        grcan_rx[grc_rgix].data[0] = 1;
		_enable();
/*		
		for(i=0;i<3;i++)
        {
            sprintf(src,"[%d]= %d",i, UnpkByte[i]);
            Log_an_event(src); // let's see what the data looks like
        }
*/        

    return 1; // force a pass for debug purposes.
}

int16 Send_Srl_Hall_Station_Pkg(int16 pkg_num, int16 msg)
{
    int16 i = 0;
    
    for(i=0;i<3;i++)
    {
        UnpkByte[i] = 0;
    }
    grcan_tx[grc_tpix].data[0] = msg;
    pack_grcan_out(pkg_num,c_grcan_HCB_1,0); 
    grcan_tx_complete[8] = 1;
	sprintf(src,"d0,1,2= %d;%d,%d",grcan_tx[grc_tpix].data[0],
	    grcan_tx[grc_tpix].data[1],
	    grcan_tx[grc_tpix].data[2] );
    Add_msg_to_array_B(src,10);    
    tx_grcan_msg(GrcanTxBufIx);	// Check if data waiting and transmit
    sprintf(src,"CAN;grCAN- %d;%d",Enable_CAN_Ctrl, Enable_gr_CAN_Ctrl);
    Add_msg_to_array_B(src,1);   
    return 0;
}

void Over_ride_pack_data(int16 d0,int16 msg,int16 d2)
{
    grcan_tx[grc_tpix].data[0] = msg; 
//    grcan_tx[grc_tpix].data[1] = d1;
//    grcan_tx[grc_tpix].data[2] = d2;  
}
 
void Show_1139_Tasks(void)
{
        New_write_display("Confirm LED's are   ",0,0);
        New_write_display("blinking on 1139 bd ",1,0);
        New_write_display("Enter Btn 4 Pass.   ",2,0);
        New_write_display("Mode Btn 4 Fail.    ",3,0);
}

