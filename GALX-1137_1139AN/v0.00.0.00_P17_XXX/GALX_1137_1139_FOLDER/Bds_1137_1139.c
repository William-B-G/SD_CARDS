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
#define GALX4_0047_index 12

int16 toggle_LDO6 = 0;
int16 toggle_LDO7 = 0;
int16 toggle_LDO8 = 0;
int16 toggle_LDO9 = 0;
int16 toggle_LDO10 = 0;
int16 toggle_LDO11 = 0;

int16 test_Loop = 0;
int16 Set_CT6 = 0;
int16 Set_CT7 = 0;
int16 DO_state[] = 
{
    0, 0, 0
};

int16 UO_state[] = 
{
    0, 0, 0
};

const int16 Hall_1137_bd_Addr[] =
{
    c_grcan_HCB_1, c_grcan_HCB_2, c_grcan_HCB_3	
};

void *GALX4_0047_in_array[GALX4_0047_index] = 
{
	i_SWi0,
	i_SWi1,
	i_SWi2,
	i_SWi3,
	i_SWi4,
	i_SWi5,
};

void *GALX4_0047_OUT_array[GALX4_0047_index] = 
{
    O_LDO6,
    O_LDO7,
    O_LDO8,
    O_LDO9,
    O_LDO10,
    O_LDO11,        	
};

void *Test_GALx1137_out_array[Test_GALx1137_index]= {
/*
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
*/    
    o_CTo0 ,
    o_CTo1 ,
    o_CTo2 ,
    o_CTo3 ,
    o_CTo4 ,
    o_CTo5 ,
    o_CTo6 ,
    o_CTo7 ,
    o_CTo8 ,
    o_CTo9 ,
    o_CTo10,
    o_CTo11
};

void *Test_GALx1137_in_array[Test_GALx1137_index]= {
/*
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
*/    
    i_CTi0, 
    i_CTi1, 
    i_CTi2, 
    i_CTi3 ,
    i_CTi4 ,
    i_CTi5 ,
    i_CTi6 ,
    i_CTi7 ,
    i_CTi8 ,
    i_CTi9 ,
    i_CTi10,
    i_CTi11 
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
int16 Data_Zero = 0;
int16 unPkMsg = 0;
int16 board_condition = BAD;
int16 GRC_Rgix = 0;
uint8 Hall_Test_Data = 0;  // Up, Down arrow data.  Up arrow bit 4, Down arrow bit 3.  Used in hlcom.c
int16 Lant_Dwn, Lant_Up = 0;
bool Multi_board_test = FALSE;
int16 Comm_to_1137  = 0;  // start OFF
int16 GALX4_0047N  = 1;  // start ON
int16 delay_for_Comm_to_1137 = 49; // 9 will give 1 msg/s, 49 = msg every 5s
int16 Latest_Hall_data = 0;


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
    Check_4_short_chan_1,
    Set_Hall_Chan_1_ON,
    Check_4_short_chan_2,
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
    Reset_2s,
	Hall_off_no_ack,
	Set_CTo6,
	Clear_CTo6,
	Enable_grp_CAN,
	Verify_0,
	Set_CTo6_again,
	Verify_8,
	Set_CTo6_and_7,
	Set_CTo7_only,
	Verify_10,
	Verify_2,
	Reset_2s_again,
    Clr_CTo6_a_7,
    Disable_grpCAN,
    Reset_2s_OnceMore,
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
    sprintf(&LCD_Display[1][8],"%02i",Data_Zero);
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
//    Log_an_event(src);
//    Log_an_event_C(src);  
    
    sprintf(src,"E=%d; H=%d",Either_Hall_Outputs,Hall_Outputs);
//    Log_an_event(src);
//    Log_an_event_C(src);      
//    if(Hall_Outputs ==1)
//      SIU.GPDO[Z6_LED].R = LED_ON;
//        else SIU.GPDO[Z6_LED].R = LED_OFF;   
    return FINISHED_WITH_SERVICE; 
}

int16 Reset_All_1137s(int16 LED_Z6)
{ // toggle LED Z6 if 'LED_Z6' true

	enum 
	{
	    initialize,
	    send_reset,
	    done,  	
	};
////////////////////////////////

    static int16 i,j,k = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 2; 
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    int16 ExtraDelay = 0;
    int16 ResetArray[] = 
    {
    	0, 2, 5, 8, 10, 11
    }; // last value (11) a dummy place holder
    
    ExtraDelay = (LED_Z6 & 0x00f0) >> 4 ;
//    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE

            ReturnVal = CONTINUE_SERVICE;
            CurrentTestPassFail =1;  // pass
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 2; 
            timers[t_Generic_Delay] = 0;
            i = 0;
            j = ResetArray[i++];
            test_Kase = send_reset;
            break;
            
        case send_reset:
            if(timers[t_Generic_Delay] < ExtraDelay + 2*Delay_Count) break;            
            //  ADD CODE
            if(Send_Hall_Reset(j) == FINISHED_WITH_SERVICE)
            {
                j = ResetArray[i++];
                if( ( LED_Z6 & 0x1 ) == 1) SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED	
            }
            if(i > 5) test_Kase = done;
            timers[t_Generic_Delay] = 0;
            break;
                                  
        case done:
            test_Kase = initialize;
            timers[t_Generic_Delay] = 0;
            if( ( LED_Z6 & 0x1 ) == 1) SIU.GPDO[Z6_LED].R = 1; // off         
            ReturnVal = FINISHED_WITH_SERVICE; // Idle_time                
            break;         
               
        default:
            break;                
    }
    return ReturnVal;
      
	
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
    enum
    {
        send_hall_data,
        send_reset_0,
        send_reset_2,
        send_reset_5,
        send_reset_8,
        send_reset_10,
        test_pass,
        test_done,	
    };
    int16 i,j,k = 0;
    int16 Or_it = msg & 0x1000;
    int16 No_Ack = msg & 0x2000;
    static int16 test_Kase = send_hall_data;
    static int16 Delay_Count = 2; 
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
//    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    if(Or_it) msg |= Latest_Hall_data; // to be able to turn one lantern
      // without having to turn the other off. 
    switch (test_Kase)
    {
        case send_hall_data:
            //  ADD CODE
//            clr_lcd_dply();
//            Clear_Msg_Board();
			Send_Srl_Hall_Station_Pkg(13, msg);
			Latest_Hall_data = msg;
			            
            ReturnVal = CONTINUE_SERVICE;
            CurrentTestPassFail =1;  // pass
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 3; 
            timers[t_Generic_Delay_3] = 0;
            if(No_Ack) test_Kase = test_pass;
            else test_Kase = send_reset_0;
            break;
            
        case send_reset_0:
            if(timers[t_Generic_Delay_3] < Delay_Count) break;            
            //  ADD CODE
            timers[t_Generic_Delay_3] = 0;
            Send_Hall_Reset(0);
            test_Kase = send_reset_2;
            break;
            
        case send_reset_2:
            if(timers[t_Generic_Delay_3] < Delay_Count) break;            
            //  ADD CODE
            timers[t_Generic_Delay_3] = 0;
            Send_Hall_Reset(2);
            test_Kase = send_reset_5;
            break;            
                        
        case send_reset_5:
            if(timers[t_Generic_Delay_3] < Delay_Count) break;            
            //  ADD CODE
            timers[t_Generic_Delay_3] = 0;
            Send_Hall_Reset(5);
            test_Kase = send_reset_8;
            break;  
                        
        case send_reset_8:
            if(timers[t_Generic_Delay_3] < Delay_Count) break;            
            //  ADD CODE
            timers[t_Generic_Delay_3] = 0;
            Send_Hall_Reset(8);
            test_Kase = send_reset_10;
            break;                 
                        
        case send_reset_10:
            if(timers[t_Generic_Delay_3] < Delay_Count) break;            
            //  ADD CODE
            timers[t_Generic_Delay_3] = 0;
            Send_Hall_Reset(10);
            test_Kase = test_pass;
            break;                  
                        
        case test_pass:
            test_Kase = test_done;
            break;
            
        case test_done:
            test_Kase = send_hall_data;
            ReturnVal = FINISHED_WITH_SERVICE; // Idle_time                
            break;
                                             
               
        default:
            ReturnVal = FINISHED_WITH_SERVICE; // Idle_time
            break;                
    }
    return ReturnVal;

}

int16 Set_or_Clear_1137_Input(int16 Set_Clear)
{   
  int16 chan_1 = Set_Clear & 0x0100; // CTo6
  int16 chan_2 = Set_Clear & 0x0200; // CTo7
  int16 chan_3 = Set_Clear & 0x0400; // CTo8
  int16 chan_4 = Set_Clear & 0x0800; // CTo9  
  int16 chan_5 = Set_Clear & 0x1000; // CT10
  int16 chan_6 = Set_Clear & 0x2000; // CT11
  
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
//        SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25
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
//    static int16 test_Loop = 0;
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
//            Log_an_event_C(src);            
            break;
                        
        case run_test: 
            Multi_board_test = TRUE;   
            switch(The_Test) 
            {            
                case 0:                
//                  The_result = Hall_Station_CAN( 0x2000 | test_Loop); // second half of test only
                    The_result = Hall_Station_CAN( test_Loop);
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
            LocalTestPassFail[test_Loop] = Current_113x_TestPassFail;
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
            test_Loop = 0;
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


int16 Full_Station_CAN_Test(int16 dummy)
{
	return Hall_Station_CAN(test_Loop);
}


int16 One_Hall_Station_CAN(int16 Test_loop)
{
	test_Loop =Test_loop;
	return Hall_Station_CAN(Test_loop);
}

int16 Hall_Station_CAN(int16 Test_loop)
{
    static int16 GRCAN_TestCase=Init_GRCAN_Test;
    static int16 Verify_Delay = 0;
    int16 Delay_Count=test_halfsec_delay;
    int16 First_or_Second_Half = Test_loop & 0xf000; // 0= full test; 0x1000 = first half; 0x2000 = second half
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
//        CurrentTestPassFail =0;  // fail
        Current_113x_TestPassFail = 0; // fail
        if( (First_or_Second_Half == 0) || (First_or_Second_Half == 0x1000) )
           First_Half_PassFail = 0;  
        else First_Half_PassFail = 1;
        Chan_1_1137 = 0;
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option        
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option      
        if(Multi_board_test == FALSE) Clear_Msg_Board(); //  Clear_Log_Buffer();  
        clr_lcd_dply();
        Set_or_Clear_1137_Input(0xff00); // clear 1107 I/O board
        Case_14_data = 0;
        Case_16_data = 0;
        board_condition = BAD;
        init_grcan_bus();
        Counter = 0;
        Kounter_2 = 0;
        Found_Ten = 0;
        Found_Zero = 0;
        Verify_Delay = 0;
        Set_CT6 = 1 << ( (test_Loop * 2) + 8 ); 
        // 1137-1 = 0x0100; 1137-2 = 0x0400; 1137-3 = 0x1000 
        Set_CT7 = 1 << ( (test_Loop * 2) + 9 ); 
        // 1137-1 = 0x0100; 1137-2 = 0x0400; 1137-3 = 0x1000                 
        SIU.GPDO[Z6_LED].R = 1; // led 6 off  
//        Send_Hall_Reset(10);    
        timers[t_Generic_Delay]=0;
        timers[t_Generic_Delay_4]=0;
        if( !StartUpMsgSent++ )
        {
            New_write_display("Set dip switch addrs",0,0);
            New_write_display("to 1, 2, 3 before   ",1,0); 
            New_write_display("applying pwr. Enter ",2,0);
            New_write_display("when ready.         ",3,0);      

            GRCAN_TestCase = Wait_for_Operator_Response;  // Send_GRCAN_Signal;            
        }
        else
        {
/*        
            New_write_display("Now sending cmd to  ",0,0);
            New_write_display("set outputs high and",1,0); 
            New_write_display("verifying with      ",2,0);
            New_write_display("GALx 1107AN         ",3,0); 
*/        
            New_write_display("Now doing first half",0,0);
            sprintf(src,"GALX-1137 positin %d", Test_loop + 1);
            New_write_display(src,1,0); 
            New_write_display("                    ",2,0);
            New_write_display("                    ",3,0);    
            if( (First_or_Second_Half == 0) || (First_or_Second_Half == 0x1000) )       
              GRCAN_TestCase = Set_Hall_OFF_A;
            else GRCAN_TestCase = Reset_2s;
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
//            Send_Hall_Reset(10); // Send reset ten 
            New_write_display("Now doing first half",0,0);
            sprintf(src,"GALX-1137 positin %d", Test_loop + 1);
            New_write_display(src,1,0);
            New_write_display("Insure Up/Dwn R off ",2,0);
            New_write_display("                    ",3,0);     
            timers[t_Generic_Delay]=0;
            if( (First_or_Second_Half == 0) || (First_or_Second_Half == 0x1000) )       
              GRCAN_TestCase = Set_Hall_OFF_A;
            else GRCAN_TestCase = Reset_2s;   
        }
        break;
        
    case Set_Hall_OFF_A:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if( Send_Hall_Data(0) == FINISHED_WITH_SERVICE) // set it off
        {
	        timers[t_Generic_Delay]=0;
	        GRCAN_TestCase = Verify_OFF_A;        	
        }
        break;
/*
    case Reset_Zero_A:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Send_Hall_Reset(0); // Send reset zero
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Verify_OFF_A;        
        break;
*/           
    case Verify_OFF_A:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Determine_1137_Output_Status(Test_loop);
        if(Either_Hall_Outputs == HALL_OFF) 
        {
            GRCAN_TestCase = Set_Hall_ON;
            New_write_display("Turn Up/Dwn on      ",2,0);    
        }
        else 
        {
            Log_an_event("outputs not off     ");
            GRCAN_TestCase = Reset_2s_OnceMore; 
        } 
        timers[t_Generic_Delay]=0;    
        break;

    case Set_Hall_ON:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if( Send_Hall_Data(5) == FINISHED_WITH_SERVICE)// Hall on
        {
	        timers[t_Generic_Delay]=0;
	        GRCAN_TestCase = Verify_ON;        	
        }     
        break;       
/*
    case Reset_Ten_A:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Send_Hall_Reset(10);
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Verify_ON;     
        break;          
*/
    case Verify_ON:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Determine_1137_Output_Status(Test_loop);
        if(Hall_Outputs == HALL_ON) 
        {
//            Send_Hall_Data(4); // Hall chan 1 on
            GRCAN_TestCase = Set_Hall_Chan_1_ON; // Check_4_short_chan_2;  // Set_Hall_Chan_1_ON ; // Set_Hall_OFF_B;  // Set_Hall_Chan_1_ON    
            New_write_display("Turn Up on, Dwn off ",2,0); 
            // chan 1 on; chan 2 Off
        }
          else 
          {
              if(Hall_Output_1 == 0)Log_an_event("IO_1_OUT not ON     ");
              if(Hall_Output_2 == 0)Log_an_event("IO_2_OUT not ON     ");
              GRCAN_TestCase = Reset_2s_OnceMore;
          }
        timers[t_Generic_Delay]=0;     
        break;    
        
    case Set_Hall_Chan_1_ON: // only chan 1 on
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if( Send_Hall_Data(1) == FINISHED_WITH_SERVICE)// Hall on
        {
	        timers[t_Generic_Delay]=0;
	        GRCAN_TestCase = Check_4_short_chan_2;        	
        }      
        break;        
        
        
    case Check_4_short_chan_2:
        if(timers[t_Generic_Delay] < 2*Delay_Count) break;
        if(rdinp(Test_GALx1137_in_array[ (2*Test_loop) + 1]) )
        { // short
            Log_an_event("IO_2_OUT shorted    ");
            sprintf(src,"rdinp %d ", (2*Test_loop) + 1 );
            Log_an_event(src);
            GRCAN_TestCase = Reset_2s_OnceMore;   
        }
        else
        {
           // Send_Hall_Data(1); // Hall chan 2 on
            GRCAN_TestCase = Set_Hall_Chan_2_ON ;  
            New_write_display("Turn Up off, Dwn on ",2,0);           
        }
        timers[t_Generic_Delay]=0;     
        break;
        
    case Set_Hall_Chan_2_ON: // only chan 2 on
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if( Send_Hall_Data(4) == FINISHED_WITH_SERVICE)// Hall 2 on
        {
	        timers[t_Generic_Delay]=0;
	        GRCAN_TestCase = Check_4_short_chan_1;        	
        }      
        break;        
                

    case Check_4_short_chan_1:
        if(timers[t_Generic_Delay] < 2*Delay_Count) break;
        if(rdinp(Test_GALx1137_in_array[2*Test_loop]))
        { // short
            Log_an_event("IO_1_OUT shorted    ");
            GRCAN_TestCase = Reset_2s_OnceMore;   
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
/*        
        New_write_display("OK, check. Now setng",0,0);
        New_write_display("chan 1 high and chek",1,0); 
        New_write_display("that 1137 sends back",2,0);
        New_write_display("proper msg.         ",3,0);
*/                  
        First_Half_PassFail = 1; // pass
        // Hall Off - no ack
        if( Send_Hall_Data(0x2000) == FINISHED_WITH_SERVICE)// Hall 1 & 2 off
        {
	        timers[t_Generic_Delay]=0;
            if( (First_or_Second_Half == 0) || (First_or_Second_Half == 0x1000) ) 
            {
            	New_write_display("Now strtg second 1/2",0,0);
            	New_write_display("                    ",2,0);
                New_write_display("                    ",3,0);
            	sprintf(src,"Clr CTo%d and CTo%d", (test_Loop * 2) + 6, (test_Loop * 2) + 7 );
            	New_write_display(src,2,0);  
            	New_write_display("verify CAN msg is 0 ",3,0) ;
            	GRCAN_TestCase = Reset_2s;
            }              
            else GRCAN_TestCase = GRCAN_test_Pass;        	
        }             
        break;           

/*
    case Reset_Zero_B:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        Send_Hall_Reset(0); // Send reset zero
        timers[t_Generic_Delay]=0;
        GRCAN_TestCase = Send_24V_to_1137_chan_1;        
        break;
*/        
///////////////////////////   FIRST HALF DONE  //////////////////////////////// 

// { " RESET  All 1137s  2", Reset_All_1137s,      0x0021 },
    case Reset_2s:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if( Reset_All_1137s (0x0021)  ==  FINISHED_WITH_SERVICE )
        { 
//          sprintf(src, "                    ")
            sprintf(src, "reset 1137 @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src);
            GRCAN_TestCase = Hall_off_no_ack;
            timers[t_Generic_Delay] = 0; 
        }   
    	break;
    	
    case Hall_off_no_ack:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if( Send_Hall_Data(0x2000) == FINISHED_WITH_SERVICE) // set it off
        {
//          sprintf(src, "                    ")
            sprintf(src, "hall off @ %d", timers[t_Generic_Delay_4] );
            Log_an_event_C(src);
            timers[t_Generic_Delay]=0;
	        GRCAN_TestCase = Set_CTo6;        	
        }            	
    	break;
    	
    	
/*   Hall_Output_1 = rdinp(Test_GALx1137_in_array[test_Loop * 2]);
     Hall_Output_2 = rdinp(Test_GALx1137_in_array[ (test_Loop * 2) + 1 ] );    
  int16 chan_1 = Set_Clear & 0x0100; // CTo6
  int16 chan_2 = Set_Clear & 0x0200; // CTo7
  int16 chan_3 = Set_Clear & 0x0400; // CTo8
  int16 chan_4 = Set_Clear & 0x0800; // CTo9  
  int16 chan_5 = Set_Clear & 0x1000; // CT10
  int16 chan_6 = Set_Clear & 0x2000; // CT11	
*/    	
    case Set_CTo6: // 8 for 1137-2 and 10 for 1137-3
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay]=0;
        Set_CT6 = 1 << ( (test_Loop * 2) + 8 ); 
        // 1137-1 = 0x0100; 1137-2 = 0x0400; 1137-3 = 0x1000 
        Set_or_Clear_1137_Input( Set_CT6 | 0x0001);
//          sprintf(src, "                    ")
            sprintf(src, "CTo6 ON @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src);        
        GRCAN_TestCase = Clear_CTo6;    	
    	break;
    	
    case Clear_CTo6:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay]=0;
        Set_or_Clear_1137_Input( Set_CT6 | 0x0000); 
//          sprintf(src, "                    ")
            sprintf(src, "CTo6 OFF @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src);         
        GRCAN_TestCase = Enable_grp_CAN;  	
    	break;   
    	
    case Enable_grp_CAN:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay]=0;
        Enable_gr_CAN_Ctrl_flag(1);
//          sprintf(src, "                    ")
            sprintf(src, "CAN ON @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src);         
        GRCAN_TestCase = Verify_0;        
            	
    	break;
    	
    case Verify_0:
        if(timers[t_Generic_Delay] < 3*Delay_Count) break;
        timers[t_Generic_Delay]=0;
        if(Data_Zero == 0)
        {
            GRCAN_TestCase = Set_CTo6_again;
//          sprintf(src, "                    ")
            sprintf(src, "Vfy0 @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src); 
            sprintf(src, "VfyDly= %d", Verify_Delay);
            Log_an_event_C(src);                        
            Verify_Delay = 0;	
        	New_write_display("                    ",2,0);
            New_write_display("                    ",3,0);            
        	sprintf(src,"Set CTo%d on and", (test_Loop * 2) + 6);
        	New_write_display(src,2,0);  
        	New_write_display("verify CAN msg is 8 ",3,0);            
        }
        else
        {
            if(Verify_Delay++ > 5)
            {
	            Log_an_event("Data[0] not 0       ");
	            GRCAN_TestCase = Reset_2s_OnceMore;            	
            }             
        }
                      	
    	break;
    	
    case Set_CTo6_again:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay]=0;
        Set_or_Clear_1137_Input( Set_CT6 | 0x0001);
 //          sprintf(src, "                    ")
            sprintf(src, "CTo6 On @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src); 
        GRCAN_TestCase = Verify_8;            	
    	break;
    	
    case Verify_8:
        if(timers[t_Generic_Delay] < 3*Delay_Count) break;
        timers[t_Generic_Delay]=0;
        if(Data_Zero == 8)
        {
            GRCAN_TestCase = Set_CTo7_only;
//          sprintf(src, "                    ")
            sprintf(src, "Vfy8 @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src); 
            sprintf(src, "VfyDly= %d", Verify_Delay);
            Log_an_event_C(src);            
            Verify_Delay = 0;
        	New_write_display("                    ",2,0);
            New_write_display("                    ",3,0);            	
        	sprintf(src,"Set CTo%d on and", (test_Loop * 2) + 7);
        	New_write_display(src,2,0);  
        	New_write_display("verify CAN msg is 2 ",3,0);             
        }
        else
        {
            if(Verify_Delay++ > 5)
            {
	            Log_an_event("Data[0] not 8       ");
	            GRCAN_TestCase = Reset_2s_OnceMore;            	
            }             
        }            	
    	break; 
    	
    case Set_CTo7_only:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay]=0;
        Set_or_Clear_1137_Input( Set_CT7 | 0x0001); // set CTo7
        Set_or_Clear_1137_Input( Set_CT6 | 0x0000); // clear CTo6
//          sprintf(src, "                    ")
            sprintf(src, "CTo7 On @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src); 
        GRCAN_TestCase = Verify_2;            	
    	break;
    	
    case Verify_2:
        if(timers[t_Generic_Delay] < 3*Delay_Count) break;
        timers[t_Generic_Delay]=0;
        if(Data_Zero == 2)
        {
            GRCAN_TestCase = Clr_CTo6_a_7;
//          sprintf(src, "                    ")
            sprintf(src, "Vfy2 @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src); 
            sprintf(src, "VfyDly= %d", Verify_Delay);
            Log_an_event_C(src);            
            Verify_Delay = 0;	
        }
        else
        {
            if(Verify_Delay++ > 5)
            {
	            Log_an_event("Data[0] not 2       ");
	            GRCAN_TestCase = Reset_2s_OnceMore;            	
            }             
        }  
            	
    	break;
    	
    case Clr_CTo6_a_7:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay]=0;
        Set_or_Clear_1137_Input( Set_CT6 |  Set_CT7 | 0x0000); // clear CTo6 and 7
//          sprintf(src, "                    ")
            sprintf(src, "CTo6&7 Off @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src); 
        GRCAN_TestCase = Reset_2s_again;            	
    	break;    	
 
     case Reset_2s_again:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if( Reset_All_1137s (0x0021)  ==  FINISHED_WITH_SERVICE )
        { 
            GRCAN_TestCase = Disable_grpCAN;
            timers[t_Generic_Delay] = 0; 
//          sprintf(src, "                    ")
            sprintf(src, "reset2 @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src);            
        }   
    	break;   	
    	   	 	
 
     case Disable_grpCAN:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        timers[t_Generic_Delay] = 0;   
        Enable_gr_CAN_Ctrl_flag(0); // Disable CAN
//          sprintf(src, "                    ")
            sprintf(src, "CAN off @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src);        
        GRCAN_TestCase = GRCAN_test_Pass;
    	break;            	
       
       
////////////////// FINALLY DONE  ///////////////////////////       
                      
    case GRCAN_test_Pass:
        Current_113x_TestPassFail = 1; // pass
        
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
        
     case Reset_2s_OnceMore:
        if(timers[t_Generic_Delay]<Delay_Count) break;
        if( Reset_All_1137s (0x0021)  ==  FINISHED_WITH_SERVICE )
        { 
            GRCAN_TestCase = Retest_option;
            timers[t_Generic_Delay] = 0; 
//          sprintf(src, "                    ")
            sprintf(src, "reset2 @ %d", timers[t_Generic_Delay_4]);
            Log_an_event_C(src);            
        }   
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
//    SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25
    Hall_Test_Data = (int8)Data;
    hl_com(); // Tx to Hall Lantern via RS485
    return FINISHED_WITH_SERVICE; 
}


int16 TimeForTheSnapShot(int16 dummy)
{ // look at sx_tranbuf[SE1COM] and send to msgbrd A
    Time_4_snap_shot = 1;
    return FINISHED_WITH_SERVICE;	
}

int16 ShowMeCTi0_CTi5(int16 dummy)
{
    int16 i = 0;
    int16 data = 0;
    
    Clear_Msg_Board();
    for(i = 0; i < 6; i++)
    {
      data = rdinp(Test_GALx1137_in_array[i]);
      sprintf(src, "CTi%d=%d", i, data);  
//      sprintf(src,"lf:cL=%d;T0=%d;T1=%d",current_Loop, Lant_Dwn, Lant_Up);
      Log_an_event(src);         
    }
    return Display_Message_Board(0);    
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
            Current_113x_TestPassFail =0;  // fail
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 5; // 1/2 second
            timers[t_Generic_Delay] = 0;
            Set_RS485_Baud(NINETEEN_DOT_2);
            Hall_Lantern_Data_A = 0xff;
            Hall_Lantern_Data_B = 0xff;
            // Hall_Lantern_Select
            Send_Hall_Lantern_Floor(0);  // up, down OFF // Hall_Test_Data
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
                Send_Hall_Lantern_Floor(0x040); //  turn up arrow on
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
                Send_Hall_Lantern_Floor(0x080); //  turn down arrow on
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
            Current_113x_TestPassFail = 1;
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
     // grcan_rx[grc_rgix].data[0]
		_enable();
		
		for(i=0;i<3;i++)
        {
            sprintf(src,"[%d]= %d",i, UnpkByte[i]);
            Log_an_event(src); // let's see what the data looks like
        }
        // Data_Zero is set in grcan.c; unpack
        sprintf(src,"Data_Zero= %d",Data_Zero);
        Log_an_event(src); 

    return FINISHED_WITH_SERVICE; 
}

/*
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
*/

int16 Send_Srl_Hall_Station_Pkg(int16 pkg_num, int16 msg)
{
    int16 i = 0;
    
    for(i=0;i<3;i++)
    {
        UnpkByte[i] = 0;
    }
    
//    pack_grcan_out(pkg_num,c_grcan_HCB_1,0);
    pack_grcan_out(pkg_num, Hall_1137_bd_Addr[test_Loop],0);
//    sprintf(src, "reset %d, %d ", pkg_num, Hall_1137_bd_Addr[test_Loop] );
//    Log_an_event(src);
//    if(LoopCycles != 0)
//    { // send alt data
//       Over_ride_pack_data(d0,msg,d2) ;  
//    }
    Over_ride_pack_data(d0,msg,d2) ; 
    grcan_tx_complete[8] = 1;
	sprintf(src,"d0,1,2= %d;%d,%d",grcan_tx[grc_tpix].data[0],
	    grcan_tx[grc_tpix].data[1],
	    grcan_tx[grc_tpix].data[2] );
//    Add_msg_to_array_B(src,10);    
    tx_grcan_msg(GrcanTxBufIx);	// Check if data waiting and transmit
    sprintf(src,"CAN;grCAN- %d;%d",Enable_CAN_Ctrl, Enable_gr_CAN_Ctrl);
//    Add_msg_to_array_B(src,1);   
    return FINISHED_WITH_SERVICE;
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

//extern int16 wbg_grp_pack_num[];
//extern int16 wbg_grp_pack_index;

int16 toggle_1137_comm(int16 dummy)
{
    Comm_to_1137 = !Comm_to_1137;
    return FINISHED_WITH_SERVICE;
}


int16 toggle_4_0047N(int16 dummy)
{
    GALX4_0047N = !GALX4_0047N;
    return FINISHED_WITH_SERVICE;
}

int16 Incr_Decr_10(int16 dummy) // delay_for_Comm_to_1137 Incr_Decr_10
{
  static int16 init = 0;

    if(init == 0)
    {
       clr_lcd_dply();
       sprintf(src, "delay = %d  ", delay_for_Comm_to_1137);
       New_write_display(src,0,0);
       init = 1;        
    }
    
    if ( Check_for_Mode_Button()  )
    {
        clr_lcd_dply();
        init = 0;
        return FINISHED_WITH_SERVICE ;              
    }
    
    if ( Check_for_Up_Button() )
    {
        if(delay_for_Comm_to_1137 < 200) delay_for_Comm_to_1137 += 10;
          else delay_for_Comm_to_1137 += 100;
        if(delay_for_Comm_to_1137 > 600) delay_for_Comm_to_1137 = 600;
        sprintf(src, "delay = %d  ", delay_for_Comm_to_1137);
        New_write_display(src,0,0);
        return CONTINUE_SERVICE;        
    }
    
    if ( Check_for_Down_Button() )
    {
        if(delay_for_Comm_to_1137 < 200) delay_for_Comm_to_1137 -= 10;
          else delay_for_Comm_to_1137 -= 100;
        if(delay_for_Comm_to_1137 < 0) delay_for_Comm_to_1137 = 0;
        sprintf(src, "delay = %d  ", delay_for_Comm_to_1137);
        New_write_display(src,0,0);
        return CONTINUE_SERVICE;         
    }   
    
    return CONTINUE_SERVICE;        
}

int16 Show_grp_can_Packet_num(int16 dummy)
{
    int16 i = 0;
    int16 data = 0;
    
    Clear_Msg_Board();
//    sprintf(src, "total Packs =%d",Total_Packs);
//    Log_an_event(src);
//    sprintf(src, "addr =%d",Hall_Station_Addr);
//    Hall_Station_Addr = 0;
//    Log_an_event(src);    
    for(i = 0; i < 12; i++)
    {
//      data = wbg_grp_pack_num[i];
//      sprintf(src, "grpcan_rx %d =%d", i, data);  
//      sprintf(src,"lf:cL=%d;T0=%d;T1=%d",current_Loop, Lant_Dwn, Lant_Up);
//      Log_an_event(src);         
    }
    return Display_Message_Board(0);     
}

// The following is specifice to 1137 & 1139 boards.  Modify this routine for other boards.
int16 Periodic_Background_Task(int16 dummy)
{ // Just to be able to see the Comm LED blinking on 1137 & 1139 to give a visual indication that we
  // are communicating.  Disabling this will cause no harm.
  //
    int16 i = 0;
    int16 j = 0;
// Nothing being tested so tickle the 1137 via a group CAN msg so as to keep the COMM LED flashing.
// Do the same for 1139 via RS485.
// If we are actively performing the test, this does not get called. wbg_2018_4_27	
	  if(Comm_to_1137) // toggled via a menu (17) operation.
	  { // send msg to hall station periodicly
	     if(timers[tGR_CAN_Tx] > delay_for_Comm_to_1137) // Default is 49 ( = 5s) 
	     { //  50 = 5s. Delay changed via menu 17
	       timers[tGR_CAN_Tx] = 0;
	       Send_Hall_Data(Latest_Hall_data); 
	        // sending this constantly, results in the GALX-1137 'Comm' LED to toggle - GOOD.
	        // 1137 dip switch was set to 1. 
	      // hl_com(); // Tx to Hall Lantern via RS485
	       Send_Hall_Lantern_Floor(Hall_Test_Data);  // send last piece of data
//	       SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25	       
	     }	    
	  }

		  if(GALX4_0047N) // toggled via a menu (24) operation.
		  { // test io board

		       if ( rdinp(GALX4_0047_in_array[1])  ) // switch 2
		       {
/*		       
		           for(i=0; i < 30; i++)
		           {
			           	sprintf(src,"consB[%d] = %d", i, consB[i]);
					    Add_msg_to_array_B(src,i);
		           }
*/; 		       
		           if(consB[24] & 0x1)
		       	       setoutp(GALX4_0047_OUT_array[0]);
		               else clroutp(GALX4_0047_OUT_array[0]);
		           if(consB[24] & 0x2)
		       	       setoutp(GALX4_0047_OUT_array[1]);
		               else clroutp(GALX4_0047_OUT_array[1]);		               
		       }
		       
/*		       
		       if ( toggle_LDO11 )
		       {
		       	   clroutp(GALX4_0047_OUT_array[5]);
		       	   toggle_LDO11 = 0;
		       }
		       else
		       {
		           toggle_LDO11 = 1;
		           setoutp(GALX4_0047_OUT_array[5]);
		       }
*/		       		       
	     }	    
	  	  
    return 0;	
}
//setoutp(Test_GALx1137_out_array[6]
//clroutp(Test_GALx1137_out_array[6]

int16 Toggle_LDOxx(int16 data_zero)
{
  int16 Kase = data_zero;
  
  if( rdinp(GALX4_0047_in_array[0]) == 0) // switch 1
  return FINISHED_WITH_SERVICE;
  else
  switch (Kase)
    {
        case 0:
            //  ADD CODE
	       if ( toggle_LDO6 = !toggle_LDO6 )
	       {
	       	   clroutp(GALX4_0047_OUT_array[0]);
	       }
	       else
	       {
	           setoutp(GALX4_0047_OUT_array[0]);
	       }
           break;
            
        case 1:            
            //  ADD CODE
            
            break;
                        
        case 2:
 	       if ( toggle_LDO7 = !toggle_LDO7 )
	       {
	       	   clroutp(GALX4_0047_OUT_array[1]);
	       }
	       else
	       {
	           setoutp(GALX4_0047_OUT_array[1]);
	       }
            break;
            
        case 3:
              
            break;
                                    
        case 4:
 
            break;
            
        case 8: 
	       if ( toggle_LDO8 = !toggle_LDO8 )
	       {
	       	   clroutp(GALX4_0047_OUT_array[2]);
	       }
	       else
	       {
	           setoutp(GALX4_0047_OUT_array[2]);
	       }            
            break;           
               
        default:
        
            break;                
    }
    return FINISHED_WITH_SERVICE;
}

int16 Enable_gr_CAN_Ctrl_flag(int16 enable_disable)
{ // Enable_gr_CAN_Ctrl
    if(enable_disable) Enable_gr_CAN_Ctrl = 1; else Enable_gr_CAN_Ctrl = 0;
//- SIU.GPDO[Z6_LED].R = 1;	// turn led off (wbg)// tagwbg 15
    return FINISHED_WITH_SERVICE;
}

int16 Set_1137_Diagnostic_Position(int16 position)
{
    test_Loop = position;
    timers[t_Generic_Delay_4] = 0;
    return Start_a_Menu(26);  
} 
  
int16 Set_DI_on(int16 dummy)
{
    int16 bit_position = 0;
    
    bit_position = 1 <<  2*test_Loop + 9;
    Set_or_Clear_1137_Input(bit_position | 0x01);
    return FINISHED_WITH_SERVICE;  
}

int16 Clear_DI_off(int16 dummy)
{
    int16 bit_position = 0;
    
    bit_position = 1 <<  2*test_Loop + 9;
    Set_or_Clear_1137_Input(bit_position | 0x00);
    return FINISHED_WITH_SERVICE;  
}

int16 Set_UI_on(int16 dummy)
{
    int16 bit_position = 0;
    
    bit_position = 1 <<  2*test_Loop + 8;
    Set_or_Clear_1137_Input(bit_position | 0x01);
    return FINISHED_WITH_SERVICE;  
}

int16 Clear_UI_off(int16 dummy)
{
    int16 bit_position = 0;
    
    bit_position = 1 <<  2*test_Loop + 8;
    Set_or_Clear_1137_Input(bit_position | 0x00);
    return FINISHED_WITH_SERVICE;  
}

int16 Cmnd_DO_on(int16 dummy)  // 0x01
{
    if( Send_Hall_Data(0x01 | UO_state[test_Loop]) == FINISHED_WITH_SERVICE )
    {        
        DO_state[test_Loop] = 0x01;
        return FINISHED_WITH_SERVICE;                
    }
    else return CONTINUE_SERVICE;  
}

int16 Cmnd_DO_off(int16 dummy)  // 0x00
{
    if( Send_Hall_Data(0x00 | UO_state[test_Loop]) == FINISHED_WITH_SERVICE  )
    {      
        DO_state[test_Loop] = 0x00;
        return FINISHED_WITH_SERVICE;              
    }
    else return CONTINUE_SERVICE;   
}

int16 Cmnd_UO_on(int16 dummy)  // 0x04
{
    if( Send_Hall_Data(0x04 | DO_state[test_Loop]) == FINISHED_WITH_SERVICE  )
    {        
        UO_state[test_Loop] = 0x04;
        return FINISHED_WITH_SERVICE;                
    }
    else return CONTINUE_SERVICE;  
}

int16 Cmnd_UO_off(int16 dummy)  // 0x00
{
    if( Send_Hall_Data(0x00 | DO_state[test_Loop])  == FINISHED_WITH_SERVICE ) 
    {        
        UO_state[test_Loop] = 0x00;
        return FINISHED_WITH_SERVICE;                 
    }
    else return CONTINUE_SERVICE; 
}

int16 Cmnd_All_on(int16 dummy)  
{

    if(Send_Hall_Data(0x05)  == FINISHED_WITH_SERVICE )
    {
        
        DO_state[test_Loop] = 0x01;
        UO_state[test_Loop] = 0x04;
        return FINISHED_WITH_SERVICE;        
        
    }
    else return CONTINUE_SERVICE;  
}

int16 Cmnd_All_off(int16 dummy)  
{
    if(Send_Hall_Data(0x00)  == FINISHED_WITH_SERVICE )
    {
        DO_state[test_Loop] = 0x00;
        UO_state[test_Loop] = 0x00;
        return FINISHED_WITH_SERVICE;


    }
    else return CONTINUE_SERVICE;  
}


int16 Clear_100ms_timer(int16 The_timer)  
{
    timers[The_timer] = 0;
    return FINISHED_WITH_SERVICE;  
}

// timers[t_Generic_Delay_4]