// Hal_com_Tx_test.c

#include "global.h"
#include "GALX_1132_TEST.h"

static int16 loopCount = 0;
static int16 Delay_Count = 3;
static int16 NextCaseAfterWait = 0;


enum {
    Init_Hal_com_Tx,
    Hal_com_Tx_Done,
    Hal_Bad_Baud,
    Hal_com_Tx,
    Hal_com_Wait_State,
    Hal_com_2s_delay_State
};

int16 Hal_com_Tx_test_Continuous(int16 dummy)
{
    int16 Nothing=0;
    static int16 Hal_Tx_State = 0;

    Nothing = dummy;  // so the compiler doesn't give warning
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED

    switch(Hal_Tx_State) {
    case Init_Hal_com_Tx:
        clr_lcd_dply(); // clear 4-line display
        Delay_Count = 3;
        SE1port.CR1.B.SBR = 354; // 19.2 kb
        New_write_display("Tx from Hal Comm    ",1,0);
        Hal_Tx_State = Hal_com_Tx;
        break;

    case Hal_com_Tx:
        if(++loopCount > 19) {
            loopCount = 0;
            Delay_Count = 20;
//          SIU.GPDO[Z6_LED].R = 0;  //  LED on
        }
        pack_hl_com(25, 20); // thinking that this should send
        // a package of characters out on SE1COM.
        sprintf(src,"19: %d", loopCount);
        debug_msg(src, 3);
        timers[t_Generic_Delay] = 0;
        Hal_Tx_State = Hal_com_Wait_State;
        NextCaseAfterWait = Hal_com_Tx;
        break;

    case Hal_com_Wait_State:
        if(Check_for_Mode_Button()) Hal_Tx_State = Hal_com_Tx_Done;
        else if(timers[t_Generic_Delay] >= Delay_Count ) {
            // each delay is 100ms
            timers[t_Generic_Delay] = 0; //
            Delay_Count = 3;
//-         SIU.GPDO[Z6_LED].R = 1;  //  LED off
            Hal_Tx_State = NextCaseAfterWait;
        }
        break;

    case Hal_Bad_Baud:
        New_write_display("Baud rate not 19.2  ",1,0);
        sprintf(src," %d", SE1port.CR1.B.SBR);
        debug_msg(src, 2);
        Hal_Tx_State = Init_Hal_com_Tx;
        return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        break;

    case Hal_com_Tx_Done:
        New_write_display("TEST DONE           ",1,0);
        Hal_Tx_State = Init_Hal_com_Tx;
        return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        break;

    default:
        New_write_display("TEST ERROR          ",1,0);
        New_write_display("default case taken  ",2,0);
        sprintf(src,"11: %d", Hal_Tx_State);
        debug_msg(src, 3);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)
        Hal_Tx_State = Init_Hal_com_Tx;
        //return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE; // comes back into function in 20ms
}

int16 Hal_com_Tx_test(int16 packet)
{
    // simple test function to send character block out on
    // Hall Com (SE1COM), using the 1138 board. Send only one block
    // of data.
    
    int16 Nothing=0;
    static int16 Hal_Tx_State = Init_Hal_com_Tx;

    switch(Hal_Tx_State) {
    case Init_Hal_com_Tx:
        initcom();
        clr_lcd_dply(); // clear 4-line display
        sx_rbf[SE1COM] = 0; // Rx Buffer end pointer      
       
        GRPport.CR1.B.SBR = SE1port.CR1.B.SBR = SE2port.CR1.B.SBR = RS485_Baud;  // all 19.2                
        SIU.GPDO[PB1].R = 0; // set = 0 when hall com is to Tx, and set = 1 when hall com is set to Rx (as 
        // when testing the RS485 with hall com set to receive a msg from group com).(wbg)        

        _disable();
    	GRPtxDis; 			// Turn off Transmit driver
    	// Turn off the receiver
    	GRPport.CR1.B.RE = 1;			// Receiver Enable: 1 = Enable, 0 = Disable
    	_enable();
        
        clearDMA_RxBuff(); // prepare the group com receiver by clearing out the beginingg of receive DMA buffer
//        Small_init_grp_DMA(); // added by Mark Duckworth on Nov 15, 2017
        gc_rcv_dma_gix = 0; // set the DMA buff index to start (wbg)
        pack_hl_com(packet, 20); // tagwbg 22
        Hal_Tx_State = Hal_com_Tx_Done;       
        break;

    case Hal_Bad_Baud:
        New_write_display("Baud rate not 19.2  ",1,0);
        sprintf(src," %d", SE1port.CR1.B.SBR);
        debug_msg(src, 2);
        Hal_Tx_State = Init_Hal_com_Tx;
        return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        break;

    case Hal_com_Tx_Done:
        Hal_Tx_State = Init_Hal_com_Tx;
        New_write_display("TEST DONE           ",1,0);
        return DONE_BUT_CONTINUE_DISPLAY; // do not erase display 
        break;

    default:
        New_write_display("TEST ERROR          ",2,0);
        New_write_display("default case taken  ",3,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)
        Hal_Tx_State = Init_Hal_com_Tx;
        //return DONE_BUT_CONTINUE_DISPLAY; // do not erase display
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE; // comes back into function in 20ms
}