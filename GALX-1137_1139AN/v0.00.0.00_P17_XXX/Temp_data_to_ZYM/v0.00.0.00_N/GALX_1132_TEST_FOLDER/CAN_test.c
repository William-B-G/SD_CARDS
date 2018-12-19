// CAN_test.c

#include "global.h"
#include "GALX_1132_TEST.h"

#define GR_CAN CAN_C	// Use C for Group CAN
#define ENABLE 1
#define DISABLE 0

int16 The_CAN_test(int16 Operation);
extern int32 grcan_status;
extern int16 grcan_mode;
extern int16 grcan_int_status;
extern int16 grcan_error;

struct SebTag {
    unsigned char IN[3];
    unsigned char SET_OUT[3];
    unsigned char OUT[3];
    unsigned char OUT_ACK[3];
    unsigned char IN_ACK;
    unsigned char ADDR;
    unsigned char MsgReq;
    unsigned char io_Label_ix;
    unsigned char Online;
    unsigned char RxCnt;
    unsigned char Version;
    unsigned char Revision;
    unsigned char TxReSendCnt;
} SEB;

int16 Group_TxCnt = 0;
int16 Mr_TxCnt = 0;
int16 Car_TxCnt = 0;

int16 Group_RxCnt = 0;
int16 Mr_RxCnt = 0;
int16 Car_RxCnt = 0;

int16 Group_TxDebugCnt = 0;
int16 Mr_TxDebugCnt = 0;
int16 Car_TxDebugCnt = 0;

int16 Group_RxDebugCnt = 0;
int16 Mr_RxDebugCnt = 0;
int16 Car_RxDebugCnt = 0;

int16 Group_req = 0;
int16 Mr_req = 0;
int16 Car_req = 0;

uint8 Group_TxErrCnt = 0;
uint8 Group_RxErrCnt = 0;
uint8 Mr_TxErrCnt = 0;
uint8 Mr_RxErrCnt = 0;
uint8 Car_TxErrCnt = 0;
uint8 Car_RxErrCnt = 0;

uint8 Group_CAN_Test_OK = 0;
uint8 Mr_CAN_Test_OK = 0;
uint8 Car_CAN_Test_OK = 0;
uint8 Enable_CAN_Ctrl = 1; // disable while doing CAN test (wbg)

int16 Group_tpix = 0;	// can transmit buffer put index
int16 Group_tgix = 0;	// can transmit buffer get index
int16 Group_rpix = 0;	// can Receive buffer put index
int16 Group_rgix = 0;	// can Receive buffer get index


bool GrpCAN_TxComplete = 1;
// GrpCAN_TxComplete

enum {
    Init_CAN_Test,
    Send_Recieve_Packets,
    Wait_for_CAN_setup,
    CAN_test_Done,
    CAN_test_Fail,
    CAN_test_Pass,
    CAN_test_Wait_State,
    CAN_Now_Testing,
    Group_CAN,
    Car_CAN,
    MR_CAN,
    Prompt_for_Group_CAN,
    Prompt_for_MR_CAN,
    Prompt_for_CAR_CAN,
    WaitingOnEnter,
    Wait_6s,
};

enum {
    Init_Test_CAN,
    Idle_time,
    Check_Tx_Rx,
};

enum {
    TGC_Fail = -1,
    TGC_Continue,
    TGC_Pass
};

void Group_CAN_Control(void);
int16 IsGrcTxBufFul(void);
void pack_grcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req);
void tx_grcan_msg(int16 TxBufIx);
int16 Test_Group_CAN(void);


int16 The_CAN_test(int16 Operation)  // externed in // GALX_1132_TEST.h
{
    // return 0 when test is done; 1 if test is not done; -1 if
    // test is done but the display should remain until Mode Button. (wbg)

    static int16 Next_CAN_Test = 0;
    static int16 CAN_TestCase = Init_CAN_Test;
    static int16 Delay_Count = 60; // 60 =~ 6s
    static int16 CaseAfterWaitingForButton = 0;
    int16 The_Operation;
    int16 Result = 0;

    The_Operation = Operation;
    SIU.GPDO[Z0_LED].R = ~SIU.GPDO[Z0_LED].R;  // toggle LED

    switch(CAN_TestCase) {
    case Init_CAN_Test:
        Enable_CAN_Ctrl = DISABLE; // so that comm_control() is not called from
        // main() but only from here.
        Clear_Msg_Board_B();
        timers[t_Generic_Delay]= 0;
        Group_CAN_Test_OK = 0; // set for No Good (wbg)
        CAN_TestCase = Prompt_for_Group_CAN;
        break;

        /*
        sequence: group CAN; Car CAN; MR CAN
        */

    case Prompt_for_Group_CAN:
        clr_lcd_dply();
        ClearTheButtons();
        debug_msg("  Please setup for  ", 1);
        debug_msg(" Group CAN and then ", 2);
        debug_msg(" Press ENTER Button ", 3);
        CaseAfterWaitingForButton = Group_CAN;
        Next_CAN_Test = Prompt_for_CAR_CAN;
        sprintf(src, "Testing Group CAN");
        CAN_TestCase = WaitingOnEnter;
        break;

    case Prompt_for_MR_CAN:
        clr_lcd_dply();
        ClearTheButtons();
        debug_msg("  Please setup for  ", 1);
        debug_msg("  MR CAN and then   ", 2);
        debug_msg(" Press ENTER Button ", 3);
        CaseAfterWaitingForButton = MR_CAN;
        Next_CAN_Test = CAN_test_Done;
        sprintf(src, "Testing MR CAN");
        CAN_TestCase = WaitingOnEnter;
        break;

    case Prompt_for_CAR_CAN:
        clr_lcd_dply();
        ClearTheButtons();
        debug_msg("  Please setup for  ", 1);
        debug_msg("  CAR CAN and then  ", 2);
        debug_msg(" Press ENTER Button ", 3);
        CaseAfterWaitingForButton = Car_CAN;
        sprintf(src, "Testing CAR CAN");
        Next_CAN_Test = MR_CAN;
        CAN_TestCase = WaitingOnEnter;
        break;

    case WaitingOnEnter:
        timers[t_Generic_Delay] = 0;
        if(Check_for_Enter_Button()) {
            clr_lcd_dply();
            debug_msg(src, 1);
            CAN_TestCase = CaseAfterWaitingForButton;
        }
        break;

    case Wait_6s: // to allow 1107AN to reset
        if(timers[t_Generic_Delay] >= 60 )
            CAN_TestCase = Wait_for_CAN_setup;
        break;

    case Wait_for_CAN_setup:
        comm_control();
        //if(Group_CAN_Test_OK) CAN_TestCase =  CAN_test_Pass; // early exit if passed sensed.
        if(timers[t_Generic_Delay] >= Delay_Count )
            CAN_TestCase = CaseAfterWaitingForButton;
        break;

    case Group_CAN: // sends msgs to group CAN every 20ms and tests for response
        // Group_CAN_Control();
        //ZYM 10/6/2017
        comm_control();
        if(Check_for_Mode_Button()) return DONE_BUT_CONTINUE_DISPLAY;

        Result = Test_Group_CAN();
        if(Result == TGC_Fail) CAN_TestCase = CAN_test_Fail;
        else if(Result == TGC_Pass) CAN_TestCase = CAN_test_Pass;
        // else continue testing on next cycle

        break;

    case Car_CAN:   // sends msgs to Car CAN every 20ms
        CAN_TestCase = CAN_test_Done;
        break;

    case MR_CAN: // sends msgs to machine room CAN every 20ms
        CAN_TestCase = CAN_test_Done;
        break;
        /*
            case CAN_test_Wait_State:
                if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
                    timers[t_Generic_Delay] = 0; // DO NOT USE THIS TIMER, AS Test_Group_CAN() IS.
                    CAN_TestCase = NextCaseAfterWait;
                }
                break;

            case CAN_Now_Testing:
                break;
        */
    case CAN_test_Done: // done with all CANs
        debug_msg("CAN Testing complete", 1);
        CAN_TestCase = Init_CAN_Test;
        Enable_CAN_Ctrl = ENABLE;
        return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        // return FINISHED_WITH_SERVICE;
        break;

    case CAN_test_Pass: // start testing next CAN
        clr_lcd_dply();
        New_write_display("CAN Test Pass       ",0,0);
        CAN_TestCase = Next_CAN_Test;
        // return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        break;

    case CAN_test_Fail: // go onto next CAN
        clr_lcd_dply();
        New_write_display("CAN Test FAILED     ",0,0);
        CAN_TestCase = Next_CAN_Test;
        // return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        break;

    default:  // fatal CAN error. Program bug
        CAN_TestCase = Init_CAN_Test;
        clr_lcd_dply();
        New_write_display("TEST ERROR          ",1,0);
        New_write_display("default case taken  ",2,0);
        SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        break;
    }
    return CONTINUE_SERVICE;	// 1
}


/*********************************************************/
/**                Group CAN Control                    **/
/*********************************************************/

void Group_CAN_Control(void)
{
    unsigned char MsgReq = 0;
    static unsigned char SebIndex = 0;
    static int DataReq = 0;
    static int DataReqLoopDelay =0;
    static int TxReSendCnt = 0;
    int Status;
    int16 dev = 12;

    unpack_grcan_in();
//	unpack_CanA_in();
    Status = GR_CAN.ESR.R;

    if(GR_CAN.ESR.B.ERRINT == 1) {
        Status = GR_CAN.ESR.R;
        GR_CAN.ESR.B.ERRINT = 1;
    }
    Group_TxErrCnt = (uint8)GR_CAN.ECR.B.TXECNT;
    Group_RxErrCnt = (uint8)GR_CAN.ECR.B.RXECNT;

    if(GR_CAN.ESR.B.BOFFINT == 1) {
        // The CAN bus is off
        GR_CAN.ESR.B.BOFFINT = 1;
//		Restart_CanA_bus();
        grcan_error |= 0x04;
        init_grcan_bus();
    }

    if(IsGrcTxBufFul() == 0) {
        // buffer is not full
        if((SEB.SET_OUT[0]!= SEB.OUT[0])||
                (SEB.SET_OUT[1]!= SEB.OUT[1])||
                (SEB.SET_OUT[2]!= SEB.OUT[2])) {
            SEB.OUT[0] = SEB.SET_OUT[0];
            SEB.OUT[1] = SEB.SET_OUT[1];
            SEB.OUT[2] = SEB.SET_OUT[2];
            pack_grcan_out(13,dev, 0);
            SEB.TxReSendCnt = 0;
            timers[tGR_CAN_Tx] = 0;
        } else if((SEB.OUT_ACK[0]!= SEB.OUT[0])||
                  (SEB.OUT_ACK[1]!= SEB.OUT[1])||
                  (SEB.OUT_ACK[2]!= SEB.OUT[2])) {
            if(timers[tGR_CAN_Tx] >= 2 ) {
                pack_grcan_out(13,dev, 0);
                timers[tGR_CAN_Tx] = 0;
                SEB.TxReSendCnt++;
            }
            if(SEB.TxReSendCnt > 10) {
                SEB.TxReSendCnt = 0;
                SEB.OUT_ACK[0]= SEB.OUT[0];
                SEB.OUT_ACK[1]= SEB.OUT[1];
                SEB.OUT_ACK[2]= SEB.OUT[2];
            }
        } else {
            if(timers[tGR_CAN_Tx] > 6 ) {
                SEB.OUT[0] = SEB.SET_OUT[0];
                SEB.OUT[1] = SEB.SET_OUT[1];
                SEB.OUT[2] = SEB.SET_OUT[2];
                pack_grcan_out(13,dev, 0);
                SEB.TxReSendCnt = 0;
                timers[tGR_CAN_Tx] = 0;
            }
        }
        // Check Input
        if(SEB.IN_ACK == 1) {
            SEB.IN_ACK = 0;
            pack_grcan_out(15,dev, 0);
        } else if(SEB.MsgReq != 0) {
            MsgReq = SEB.MsgReq;
            SEB.MsgReq = 0;
            pack_grcan_out(MsgReq,dev, 0);
        } else if(Group_req == 1) {
            pack_grcan_out(10,dev, 1);
            Group_req = 0;
            Group_TxDebugCnt++;
        } else if(timers[tGR_CAN_Rx] > 3) {
            if(SEB.Version == 0) {
                pack_grcan_out(dev, dev, 0);
                timers[tGR_CAN_Rx] = 0; // tGR_CAN_Tx, tGR_CAN_Rx
                if(SEB.RxCnt > 0)
                    SEB.RxCnt--;
            } else {
                pack_grcan_out(14,dev, 1);
                Group_TxCnt++;
                timers[tGR_CAN_Rx] = 0;
                if(SEB.RxCnt > 0)
                    SEB.RxCnt--;
            }
        }
        if(SEB.RxCnt > 0) {
            SEB.Online = 1;
        } else {
            SEB.Online = 0;
            SEB.Version = 0;
            SEB.IN[0] = 0;
            SEB.IN[1] = 0;
            SEB.IN[2] = 0;
        }
    }
    //    Transmit  Can  Packet

    Add_msg_to_array_B(" test to send pkt", 3);
    GR_CAN.IMASK1.R = 0x00000000;
    if (Group_tgix != Group_tpix) {
        if((GR_CAN.MB[8].MSG.MCS.B.CODE == 0x08 || GR_CAN.MB[8].MSG.MCS.B.CODE == 0x04 ) && (GrpCAN_TxComplete == 1)) {
            //CAN_A_Transmit(8);
            Add_msg_to_array_B(" Send packet", 4);
            tx_grcan_msg(8);	// GrcanTxBufIx
            // Check if data waiting and transmit
        }
    }
    GR_CAN.IMASK1.R = 0x000001E0;
}

int16 Test_Group_CAN(void)
{
    static int16 NextCaseAfterWait = 0;
    static int16 TGC_Case = 0;

    switch(TGC_Case) {
    case Init_Test_CAN:
        timers[t_Generic_Delay] = 0;
        TGC_Case = Idle_time;
        break;

    case Idle_time:
        if(timers[t_Generic_Delay] >= 3) {
            timers[t_Generic_Delay] = 0;
            TGC_Case = Check_Tx_Rx;
        }

        break;

    case Check_Tx_Rx:
        TGC_Case = Init_Test_CAN;
        if(Group_CAN_Test_OK) return TGC_Pass;
        //	if( (Group_TxCnt > 2) && (Group_RxCnt > 2) )
        //		return TGC_Pass;
        else { // sprintf(src,"11: %d %d",
            sprintf(src, "12: %d, %d", Group_TxCnt, Group_RxCnt);
            Log_a_fault(src, 1); // erases fault log
            Log_a_fault(src, 0); // add the fault
            return TGC_Fail;
        }
        break;

    default:
        TGC_Case = Init_Test_CAN;
        Clear_Msg_Board();
        Clear_Msg_Board_B();
        Log_a_fault("default in TGC",0);
        SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return TGC_Fail;
        break;
    }
    return TGC_Continue;
}
