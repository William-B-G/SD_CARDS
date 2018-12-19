#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"


int16 RxBuffSize = 0;

int16 Check_RS485_hl_Rx_packet(void);
int16 Check_RS485_grp_Rx_packet(void);
int16 The_RS_485_test(int16 Operation);
void fill_grp_com_tx_buff(void);
void fill_hall_com_tx_buff(void);
void temp_4_RS485 (void);

extern void grp_com_tx_start(void);
extern unsigned char ups_com_seq;
extern void pack_apc_ups_com (void);

enum {
    Init_RS_485_Test,
    Send_Recieve_Packets,
    RS_485_test_Done,
    RS_485_test_Fail,
    RS_485_test_Pass,
    RS_485_test_Wait_State,
    RS_485_Send_from_GrpCom_State,
    RS_485_Rx_from_GrpCom_State,
    RS_485_Send_from_Hall_State,
    RS_485_Rx_from_Hall_State,
    RS_485_Rx_Q_from_Hall_State,
    RS_485_Rx_f_from_Hall_State,
    Display_TestFailed, // Required for Retest Option
    Display_TestPass, // Required for Retest Option
    Display_TestDone, // Required for Retest Option
    Retest_option  // Required for Retest Option
} ;

enum
{
    InitTask,
    RunTask,
    TaskDone 
};



#define TEST_RS_485_Fault_Display_Index	3
char TEST_RS_485_Fault_Display[TEST_RS_485_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC15,30,31,32,33 CN6",	
	"GALX-1138N CN17,18  ",
};







int16 ShowMe_tempBuff(int16 Dummy)
{
    WhichBuffer = 1;
    return ShowMeRxBuf(0);
}

int16 ShowMe_sx_recbuf(int16 Dummy)
{
    WhichBuffer = 0;
    return ShowMeRxBuf(0);
}

void temp_4_RS485 (void)
{
    // only temporary until version 'E' of 1132 is avail
    uint8 tempData[] = {17, 81, 18, 7, 166, 30, 85, 58, 2, 49};
    uint8 i = 0;
    for(i=0; i<10; i++) {
        temp_char = tempData[i];
        tempBuff[sx_rbf[SE1COM]] = temp_char;
        sx_recbuf[SE1COM][sx_rbf[SE1COM]] = temp_char;  // save the char
        sx_rbf[SE1COM]++;
    }
}

int16 ShowMeRxBuf(int16 Dummy)
{
    int16 Buff_end = sx_rbf[SE1COM];
    int16 temp_index = 0;
    int16 ReturnVal = 0;
    static int16 BuffRead_operation = Init_BuffRead;

    switch (BuffRead_operation) {
    case Init_BuffRead:
        Clear_Log_Buffer();
        sprintf(src,"Rx isr's: %d",
                Buff_end);
        Log_a_fault(src, 0);
        if(WhichBuffer == 0) Buff_end = sx_rbf[SE1COM];
        else Buff_end = 30;

      //for(temp_index = 0; temp_index < Buff_end; temp_index++) {
        for(temp_index = 0; temp_index < 8; temp_index++) {
 //           if(WhichBuffer == 0) sprintf(src,"idx:%d, data: %2.2X",temp_index,
 //                                            (uint8)sx_recbuf[SE1COM][temp_index]);
 //           else sprintf(src,"idx:%d, data: %2.2X",temp_index, (uint8)tempBuff[temp_index] );
             if(WhichBuffer == 0) sprintf(src,"%d: %2.2X: %d",temp_index,
                                             (uint8)sx_recbuf[SE1COM][temp_index], &sx_recbuf[SE1COM][temp_index]);
            else sprintf(src,"%d: %2.2X: %d",temp_index, (uint8)tempBuff[temp_index], &tempBuff[temp_index] );
            Log_an_event(src);
        }
        BuffRead_operation = Display_Buff;
        break;

    case Display_Buff:
        ReturnVal = DisplayMsgBrd_A(0);
        if(ReturnVal == CONTINUE_SERVICE) return ReturnVal;
        // else
        BuffRead_operation = Init_BuffRead;
        return ReturnVal;
        break;

    default:
        BuffRead_operation = Init_BuffRead;
        return DONE_BUT_CONTINUE_DISPLAY;
        break;
    }
    return CONTINUE_SERVICE;
}

int16 Toggle_RS_485_Ctrl(int16 dummy)
{
    if(Enable_RS485_Ctrl)
    {
        Enable_RS485_Ctrl   = DISABLED; 
 	    Enable_Hallcom_Ctrl = DISABLED;        
    }
    else
    {
        Enable_RS485_Ctrl   = ENABLED; 
 	    Enable_Hallcom_Ctrl = ENABLED;        
    }
//- SIU.GPDO[Z6_LED].R = 1;  // turn LED OFF
    return FINISHED_WITH_SERVICE;
}

// 	Enable_RS485_Ctrl = DISABLED;  // disable RS-485 until ready to test RS485
// 	Enable_Hallcom_Ctrl = DISABLED; // disable RS-485 until ready to test RS485
int16 EnableDisableRS485_com_Ctrl(int16 enable)
{
    if(enable)
    {
        Enable_RS485_Ctrl   = ENABLED; 
 	    Enable_Hallcom_Ctrl = ENABLED;        
    }
    else
    {
        Enable_RS485_Ctrl   = DISABLED; 
 	    Enable_Hallcom_Ctrl = DISABLED;        
    }
//- SIU.GPDO[Z6_LED].R = 1;  // turn LED OFF
    return FINISHED_WITH_SERVICE;
}

int16 Hl_Tx_to_Grp_Rx(int16 Dummy)
{
    static int16 NextCaseAfterWait = 0;
    static int16 RS_485_TestCase = 0;
    static int16 Delay_Count = 3;
    int16 The_Operation;

    The_Operation = Dummy;
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED
    CurrentTestPassFail = 0; // set for fail at start.
    
    switch(RS_485_TestCase) {
        case Init_RS_485_Test:
            Clear_Log_Buffer();
            CurrentTestPassFail =0;  // fail
           // sx_rbf[SE1COM] = 0; // Rx Buffer end pointer
            Delay_Count = 3;
            GRPport.CR1.B.SBR = SE1port.CR1.B.SBR = SE2port.CR1.B.SBR = RS485_Baud;  // all 19.2

            RS_485_TestCase = RS_485_Send_from_Hall_State;

            break;

         case RS_485_Send_from_Hall_State: // 
                // send msg from hall com to group comm and enter wait state
                //fill_hall_com_tx_buff(void);// the buffer is filled from the
                // pack_hl_com()

                pack_hl_com(25, 20); // fills the buffer
                timers[t_Generic_Delay] = 0;
                NextCaseAfterWait = RS_485_Rx_from_Hall_State;
                RS_485_TestCase = RS_485_test_Fail;
                break; 
                
         case RS_485_Rx_from_Hall_State: // 
            // call the group unpack
            if( Check_RS485_grp_Rx_packet() ) RS_485_TestCase = RS_485_test_Pass;
            else RS_485_TestCase = RS_485_test_Fail;
            break;
            
        case RS_485_test_Wait_State:
            if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
                timers[t_Generic_Delay] = 0; //
                RS_485_TestCase = NextCaseAfterWait;
            }
            break;  

        case RS_485_test_Done:
            RS_485_TestCase = Init_RS_485_Test;
//-         SIU.GPDO[Z6_LED].R = 1; // turn off LED
            return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
            break;

        case RS_485_test_Pass:  // E
            CurrentTestPassFail = 1; // pass
            RS_485_TestCase = RS_485_test_Done;
            New_write_display("RS-485 Test PASS    ",0,0);
            break;

        case RS_485_test_Fail: // F
            CurrentTestPassFail = 0; // fail
            RS_485_TestCase = RS_485_test_Done;
            New_write_display("RS-485 Test FAILED. ",0,0);
            break;

        default:
            RS_485_TestCase = Init_RS_485_Test;
            clr_lcd_dply();
            New_write_display("TEST ERROR          ",1,0);
            New_write_display("default case taken  ",2,0);
            return DEFAULT;
            break;
    }
    return CONTINUE_SERVICE;	// 1                      
                                          
 }

/**********************************************
* Function:                                   *
*   int16 The_RS_485_test(int16 Operation)    *
*                                             *
* Description: Sends packets from Group COM   *
*   to Hall COM and from Hall COM to Group    *
*   COM. Checks to be sure that the Hall      *
*   COM recieves the exact data that was      *
*   sent by Group COM. For data recieved by   *
*   Group COM, only check that the first      *
*   character recieved matches the first      *
*   character sent by Hall COM.               *
*                                             *
*                                             *
* Created: September, 2017                    *
*                                             *
* Example:                                    *
*                                             *
*                                             *
* Name: William Grauer                        *
*                                             *
**********************************************/

int16 The_RS_485_test(int16 Operation)  // externed in // GALX_1132_TEST.h
{ // 
    static int16 NextCaseAfterWait = 0;
    static int16 RS_485_TestCase = Init_RS_485_Test;
    static int16 Delay_Count = 10;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    int16 The_Operation;

    The_Operation = Operation;
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

/*
#define GRPtxEn SIU.GPDO[RTS3].R = 0  // led on
#define GRPtxDis SIU.GPDO[RTS3].R = 1
*/
    switch(RS_485_TestCase) {
    case Init_RS_485_Test:
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
        Small_initcom();
        Clear_Log_Buffer();                       
        CurrentTestPassFail =0;  // fail
        Retest_Option_Parm = 0; // Required for Retest Option
        RetestResponse = 1; // Required for Retest Option
        clr_lcd_dply(); // clear 4-line display
        timers[t_Generic_Delay] = 0;
        Delay_Count = 10;
        sx_rbf[SE1COM] = 0; // Rx Buffer end pointer      
        SIU.GPDO[PB1].R = 1; // set = 0 when hall com is to Tx, and set = 1 when hall com is set to Rx (as 
        // when testing the RS485 with hall com set to receive a msg from group com).(wbg)
        GRPport.CR1.B.SBR = SE1port.CR1.B.SBR = SE2port.CR1.B.SBR = RS485_Baud;  // all 19.2
    	Enable_RS485_Ctrl = DISABLED;   // disable RS-485 while doing RS485
 	    Enable_Hallcom_Ctrl = DISABLED; // disable RS-485 while doing RS485 
 	    GRPtxEn;       
        NextCaseAfterWait = RS_485_Send_from_GrpCom_State;
        RS_485_TestCase = RS_485_test_Wait_State;
//       Enable_RS485_Ctrl = ENABLED;
		New_write_display("RS485 TEST          ",0,0);
		New_write_display("Checking...         ",2,0);
        break;

    case RS_485_Send_from_GrpCom_State: // A
        // output buffer is filled below with call to fill_grp_com_tx_buff()

        fill_grp_com_tx_buff();  // fills 'gc_trm_Buf[]'
        grp_com_tx_start();  // for group com // GRPport.CR1.B.SBR
        timers[t_Generic_Delay] = 0;
        if( (The_Operation & 0x03) == 3)
        { //  in order to create a 'one-shot' msg from group to hall com.
            RS_485_TestCase =   RS_485_test_Pass;
            break; 
        }  
        else
        {
            NextCaseAfterWait = RS_485_Rx_from_GrpCom_State;
            RS_485_TestCase = RS_485_test_Wait_State;            
        }
        break;

    case RS_485_test_Wait_State:
        if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
            timers[t_Generic_Delay] = 0; //
            RS_485_TestCase = NextCaseAfterWait;
        }
        break;

    case RS_485_Rx_from_GrpCom_State: // B
        RxBuffSize = receive_hl_com();  // RS_485_RxBuf[]
        if( Check_RS485_hl_Rx_packet() ) RS_485_TestCase = RS_485_Send_from_Hall_State; //RS_485_Send_from_Hall_State;
        else RS_485_TestCase = RS_485_test_Fail;
        SIU.GPDO[PB1].R = 0; // set = 0 when hall com is to Tx, and set = 1 when hall com is set to Rx (as 
        // when testing the RS485 with hall com set to receive a msg from group com).(wbg)
        break;

    case RS_485_Send_from_Hall_State: // C
        // send msg from hall com to group comm and enter wait state
        //fill_hall_com_tx_buff(void);// the buffer is filled from the
        // pack_hl_com()
        SIU.GPDO[PB1].R = 0; // set = 0 when hall com is to Tx, and set = 1 when hall com is set to Rx (as 
        // when testing the RS485 with hall com set to receive a msg from group com).(wbg) 
        //
        
        _disable();
    	GRPtxDis; 			// Turn off Transmit driver
    	// Turn off the receiver
    	GRPport.CR1.B.RE = 1;			// Receiver Enable: 1 = Enable, 0 = Disable
    	_enable();
                   
        sprintf(src,"02: entering case C:");
//        Log_an_event(src); //Add_msgx_First_TimeOnly(src,  2);
        clearDMA_RxBuff(); // prepare the group com receiver by clearing out the beginingg of receive DMA buffer
        gc_rcv_dma_gix = 0; // set the DMA buff index to start (wbg)
        pack_hl_com(25, 20); // fills the buffer. // tagwbg 21
        timers[t_Generic_Delay] = 0;
        NextCaseAfterWait = RS_485_Rx_from_Hall_State;
        RS_485_TestCase = RS_485_test_Wait_State;
        break;

    case RS_485_Rx_Q_from_Hall_State: // G
        sprintf(src,"06: entering case G:");
//        Log_an_event(src);  // Add_msgx_First_TimeOnly(src,  6);
        clearDMA_RxBuff(); // prepare the group com receiver by clearing out the beginingg of receive DMA buffer
        gc_rcv_dma_gix = 0; // set the DMA buff index to start (wbg)
        sx_enable_tx(SE1COM);
        sx_sendc(SE1COM,'Q');
        timers[t_Generic_Delay] = 0;
        NextCaseAfterWait = RS_485_Rx_from_Hall_State;
        RS_485_TestCase = RS_485_test_Wait_State;
        break;

    case RS_485_Rx_f_from_Hall_State: // H
        sprintf(src,"07: entering case H:");
//        Log_an_event(src);  // Add_msgx_First_TimeOnly(src,  7);
        sx_enable_tx(SE1COM);
        sx_sendc(SE1COM,'f');
        RS_485_TestCase = RS_485_test_Fail;
        break;

    case RS_485_Rx_from_Hall_State: // D
        // call the group unpack
        sprintf(src,"03: entering case D:");
//        Log_an_event(src);  // Add_msgx_First_TimeOnly(src,  3);
        if( CheckGroupRxBuff(0) ) RS_485_TestCase = RS_485_test_Pass;
        else RS_485_TestCase = RS_485_test_Fail;
        break;

    case RS_485_test_Done:
        RS_485_TestCase = Init_RS_485_Test;

//-     SIU.GPDO[Z6_LED].R = 1; // turn off LED
//    	Enable_RS485_Ctrl = ENABLED;   // enable Ctrl after doing RS485
// 	    Enable_Hallcom_Ctrl = ENABLED; // enable Ctrl after doing RS485       
        // Required for Retest Option
        if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
        else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  
        break;

    case RS_485_test_Pass:  // E
        CurrentTestPassFail = 1; // pass
        sprintf(src,"04: entering case E:");
//        Log_an_event(src);  // Add_msgx_First_TimeOnly(src,  4);
        RS_485_TestCase = RS_485_test_Done;
        clr_lcd_dply();
        New_write_display("RS485 TEST          ",0,0);
        New_write_display("TEST PASSED         ",1,0);
        New_write_display("GOOD JOB            ",2,0);
        break;

    case RS_485_test_Fail: // F
        CurrentTestPassFail = 0; // fail
//      sprintf(src,"05: entering case F:");
//      Add_msgx_First_TimeOnly(src,  5);
        RS_485_TestCase = Retest_option; // Required for Retest Option
//-    	clr_lcd_dply();
        New_write_display(TEST_RS_485_Fault_Display[0],2,0);
        
        GALX1121_Fault_Display_ADDIN_Function();
        Log_an_event(TEST_RS_485_Fault_Display[1]);
        Log_an_event(TEST_RS_485_Fault_Display[2]);
        break;

     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
        Retest_Option_Parm = 1;
        if(RetestResponse == 0) RS_485_TestCase = RS_485_test_Done;
        else if(RetestResponse == 1) RS_485_TestCase = Init_RS_485_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE; // Idle_time    
        break;  

    default:
        RS_485_TestCase = Init_RS_485_Test;
        clr_lcd_dply();
        New_write_display("TEST ERROR          ",1,0);
        New_write_display("default case taken  ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        //return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        ReturnVal = DEFAULT; // Required for Retest Option
        break;
    }
    return ReturnVal; // Required for Retest Option	
}

void clearDMA_RxBuff(void)
{ // (wbg) 2017-10-28
    int i = 0;
    
    gc_rcv_dma_gix = 0; // set the DMA buff index to start (wbg)
    for(i=0; i<30; i++)
    { // zero out first 30 spots
        gc_rcv_dma_buf[i] = 0; 
    }
}

int16 Check_RS485_hl_Rx_packet(void)
{
    int16 index = 0;

    for(index = 0; index < RxBuffSize; index++) {
        if((uint8)sx_recbuf[SE1COM][index] != (uint8)gc_trm_buf[index]) return FAILED;
          // if hall com receive is not equal to group com transmit, then return FAILED.
    }
// (uint8)sx_recbuf[SE1COM][temp_index]
    return SUCCESS;
}

void fill_grp_com_tx_buff(void)
{
    //gc_trm_buf
    gc_trm_buf[0] = 14;
    gc_trm_buf[1] = 15;
    gc_trm_buf[2] = 110;
    gc_trm_buf[3] = 10; //  len = 8
    gc_trm_buf[4] = 18;
    gc_trm_buf[5] = 19;
    gc_trm_buf[6] = 20;
    gc_trm_buf[7] = 21;
    gc_trm_buf[8] = 22;  // '$';
    gc_trm_buf[9] = NULL;
}

void fill_hall_com_tx_buff(void)
{

}

int16 CheckGroupRxBuff(int16 dummy)
{
    unsigned char k = 0;
    unsigned char m = 0;
    int16 i = 0;
    int16 pass_fail = SUCCESS;  // assume pass 
    
        for(i=0; i<= dummy; i++)
        {
           k = sx_tranbuf[SE1COM][i]; // hall com transmit buffere
           m = (uint8)gc_rcv_dma_buf[i];  // group com DMA receive buffer
           if(m != k)pass_fail=FAILED;
        }    
    return pass_fail;   
}

int16 GrpRxPacket(int16 Dummy)
{
    return Check_RS485_grp_Rx_packet();
}

int16 Check_RS485_grp_Rx_packet(void)
{
    int16 i = 0;
    int16 ReturnVal = 0;
    unsigned char k = 0;
    unsigned char m = 0;
    int16 pass_fail = 1;  // assume pass  
    static int16 BuffRead_operation = Init_BuffRead;

    switch (BuffRead_operation) {
    case Init_BuffRead:
        Clear_Log_Buffer();
        for(i=0; i<16; i++)
        {
           k = sx_tranbuf[SE1COM][i]; // hall com transmit buffere
           m = (uint8)gc_rcv_dma_buf[i];  // group com DMA receive buffer
           sprintf(src,"TxBf[%d] %3.3d Rx %3.3d", i, k, m);
           Log_an_event(src);
           if(m != k)pass_fail=0; // tagwbg 21
        }        
        BuffRead_operation = Display_Buff;
        break;

    case Display_Buff:
        ReturnVal = DisplayMsgBrd_A(0);
        if(ReturnVal == CONTINUE_SERVICE) return ReturnVal;
        // else
        BuffRead_operation = Init_BuffRead;
        return ReturnVal;
        break;

    default:
        BuffRead_operation = Init_BuffRead;
        return DONE_BUT_CONTINUE_DISPLAY;
        break;
    }
    return CONTINUE_SERVICE; 
}

int16 TransmitFromHallCom(int16)
{
    
}

int16 Call_temp_SW(int16 dummy)
{
    temp_4_RS485();
    return FINISHED_WITH_SERVICE;
}

int16 Set_RS485_Baud(int16 Counter)
{
    RS485_Baud = Counter;
    GRPport.CR1.B.SBR = SE1port.CR1.B.SBR = SE2port.CR1.B.SBR = RS485_Baud;
    USR1port.CR1.B.SBR = USR2port.CR1.B.SBR = RS485_Baud;
    return FINISHED_WITH_SERVICE;
}

int16 Display_RS485_Baud(int16 dummy)
{
    clr_lcd_dply();

    // GRPport.CR1.B.SBR = SE1port.CR1.B.SBR = SE2port.CR1.B.SBR    
    if( SE1port.CR1.B.SBR == RS485_Baud && SE2port.CR1.B.SBR == RS485_Baud) {
        switch (RS485_Baud) {
        case 2832:
            New_write_display(" Baud = 2400        ",1,0);
            break;

        case 1416:
            New_write_display(" Baud = 4800        ",1,0);
            break;

        case 708:
            New_write_display(" Baud = 9600        ",1,0);
            break;

        case 354:
            New_write_display(" Baud = 19,200      ",1,0);
            break;

        case 177:
            New_write_display(" Baud = 38,400      ",1,0);
            break;

        case 118:
            New_write_display(" Baud = 57,600      ",1,0);
            break;

        case 59:
            New_write_display(" Baud = 115,200     ",1,0);
            break;

        default:
            sprintf(src,"%d, %d", SE1port.CR1.B.SBR, SE2port.CR1.B.SBR ) ;           
            New_write_display(src,1,0);
//            New_write_display(" Baud = UNKNOWN xxx ",1,0);
            break;
        }
    } else
    {
        sprintf(src,"%d_%d", SE1port.CR1.B.SBR, SE2port.CR1.B.SBR ) ;           
        New_write_display(src,1,0);
        
    }
    return DONE_BUT_CONTINUE_DISPLAY;
}

int16 Special_test_for_Johnny(int16 dummy)
{
    static int16 ZYM_position = 2;
    int16 i = 0;
    
    // setgoutp(GRtest_osignal_array[1]);
    if( (ZYM_position % 2) == 0 ) setgoutp(GRtest_osignal_array[0]);
    else clrgoutp(GRtest_osignal_array[0]);
    
    if( !(ZYM_position % 2) ) setgoutp(GRtest_osignal_array[2]);
    else clrgoutp(GRtest_osignal_array[2]);
    
    ZYM_position++;    
    
    _disable();
   	GRPtxDis; 			// Turn off Transmit driver
    // Turn off the receiver
   	GRPport.CR1.B.RE = 1;	// Receiver Enable: 1 = Enable, 0 = Disable
   	_enable();
    	
    pack_hl_com(24, 20); // fills the buffer
	
	return FINISHED_WITH_SERVICE;
}

int16 CAN_Ctrl_EN_bit(int16 enable_disable)
{ // Enable_gr_CAN_Ctrl
    if(enable_disable) Enable_CAN_Ctrl = 1; else Enable_CAN_Ctrl = 0;
//- SIU.GPDO[Z6_LED].R = 1;	// turn led off (wbg)// tagwbg 15
    return FINISHED_WITH_SERVICE;
}

int16 Query_Ctrl_bits(int16 dummy)
{
    static int16 Ctrl_bitsCase = InitTask;
    int16 The_Operation; 
    int16 ReturnVal = 0;    
    
    The_Operation = dummy;
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED
    CurrentTestPassFail = 1; // set for pass since this is not a test. 
    
    switch (Ctrl_bitsCase) {
    case InitTask:
        Clear_Log_Buffer();
        sprintf(src,"CAN ctrl bit = %d",     Enable_CAN_Ctrl);
        Log_an_event(src); 
            
        sprintf(src,"CT CAN ctrl bit = %d ", Enable_CT_CAN_Ctrl);
        Log_an_event(src); 
        
        sprintf(src,"MR CAN ctrl bit = %d ", Enable_MR_CAN_Ctrl);
        Log_an_event(src);                          
            
        sprintf(src,"gr CAN ctrl bit = %d ", Enable_gr_CAN_Ctrl);
        Log_an_event(src); 
        
        sprintf(src,"RS485 ctrl bit = %d  ", Enable_RS485_Ctrl);
        Log_an_event(src);                           
        
        sprintf(src,"Hall com ctrl bit =%d", Enable_Hallcom_Ctrl);
        Log_an_event(src);   
                               
        Ctrl_bitsCase = RunTask;
        break ; 
        
    case RunTask:
            ReturnVal = DisplayMsgBrd_A(0);
            if(ReturnVal == CONTINUE_SERVICE) return ReturnVal;
            // else
            Ctrl_bitsCase = InitTask;
            return ReturnVal;
        break; 
         
    default:
        Ctrl_bitsCase = InitTask;
        clr_lcd_dply();
        New_write_display("TEST ERROR          ",1,0);
        New_write_display("default case taken  ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        //return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        return DEFAULT;
        break;                                           
    }
    return CONTINUE_SERVICE;	// 1       
}

int16 All_Ctrl_EN_bit(int16 enable_disable)
{
    Enable_CAN_Ctrl = Enable_CT_CAN_Ctrl = Enable_MR_CAN_Ctrl = Enable_iospi_Ctrl =
       Enable_gr_CAN_Ctrl =  Enable_RS485_Ctrl = 
           Enable_Hallcom_Ctrl = enable_disable;
//- SIU.GPDO[Z6_LED].R = 1;	// turn led off (wbg)// tagwbg 15
    return FINISHED_WITH_SERVICE;
}

int16 CT_CAN_Enable_bit(int16 enable_disable)
{
    Enable_CT_CAN_Ctrl = enable_disable;
//- SIU.GPDO[Z6_LED].R = 1;	// turn led off (wbg)// tagwbg 15
    return FINISHED_WITH_SERVICE;
}
