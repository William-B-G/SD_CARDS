#include "MPC5668G.h"

//ZYM 10/19/2017
//-#include "pinassign.h"
#include "global.h"
#include "NEW_CAN_CON.h"
//-#include "Timer.h"
//-#include "CAN.h"

//-extern int32 timers[tnmb];

// CAN_A Int Base = 127
// CAN_B Int Base = 157
// CAN_C Int Base = 178
// CAN_D Int Base = 199

#define CAN_B_INT_BASE 157
#define CanB_BufSize 30

#define c_Freeze_Start		0x01	// put chip in freeze mode
#define c_Freeze_Stop		0x00	// Takes chip out of freeze mode

struct CanBuffer {
    unsigned char len;
    unsigned char rtr;		// Remote Transmission Request
    unsigned char srr;		// Substitute Remote Request
    unsigned char ex_id;
    unsigned char msg;		// Message
    unsigned char addr;		// Address
    unsigned char id2;
    unsigned char id3;
    unsigned char data [8];
    unsigned char ValidMsg;
    int16 time;
};

#define A_Address  1
#define B_Address  2
#define C_Address  3
#define D_Address  4

struct CanBuffer CanB_TX[CanB_BufSize + 1];
struct CanBuffer CanB_RX[CanB_BufSize + 1];

int16 CanB_tpix = 0;					// can transmit buffer put index
int16 CanB_tgix = 0;					// can transmit buffer get index
int16 CanB_rpix = 0;					// can Receive buffer put index
int16 CanB_rgix = 0;					// can Receive buffer get index

int16 CanBTxBufIx = 8;

extern struct SebTag SEB;

static COMMWORD_TYPE LabelCheckSum;

static const char SEB_LABEL[12][6] = {
    " SLi0",
    " SLi1",
    " SLi2",
    " SLi3",
    " SLi4",
    " SLi5",
    " SLi6",
    " SLi7",
    " SLi8",
    " SLi9",
    "SLi10",
    "SLi11"
};

bool CanBTxComplete = 1;
    
//ZYM 10/16/2017
//only using 1 SEB for testing, its CAN ADD is hardcode here
//CAN Add 12, dip switch setting 2 
#define SEB_TEST_BRDADD		12

/*********************************************************/
/**                 CAN B Function Prototype            **/
/*********************************************************/

int16 CAN_B_ChipFreezeMode(unsigned char set);
void Init_CanB_bus (void);
void Restart_CanB_bus(void);
void wrong_CanB_int (void);
void CanB_bus_off_and_warning_int (void);
void CanB_error_int (void);
void CanB_rcv_int (void);
void CanB_rcv_fifo_warning_int (void);
void CanB_rcv_fifo_overflow_int (void);
void CanB_trm_int_b8 (void);
void receive_CanB_in (int16 BufIx);
void unpack_CanB_in (void);
int16 CanB_IsTxBufFul(void);
void pack_CanB_out(unsigned char Msg, unsigned char Brd,unsigned char tx_req);
void CAN_B_Transmit(int16 TxBufIx);
void CAN_B_Control(void);
static unsigned short int CalculateLabelCheckSum(void);
void  CanB_Clear(void);

/*********************************************************/
/**                 CAN B Freeze Mode                   **/
/*********************************************************/
int16 CAN_B_ChipFreezeMode(unsigned char set)
{
    int init_mode_cnt;

    static bool CanB_in_chip_reset;

    if (CanB_in_chip_reset == true)
        return 0;

    CanB_in_chip_reset = true;

    init_mode_cnt = 0;

    CAN_B.MCR.B.MDIS = 0;			// Enable module after reset
    CAN_B.MCR.B.FRZ = 1;			// Set or clear the freeze bit
    CAN_B.MCR.B.HALT = set;			// Clear halt mode


    while (init_mode_cnt < 200) {
        if (CAN_B.MCR.B.FRZACK == set) {
            CanB_in_chip_reset = false;
            return 1;
        }
        init_mode_cnt++;
    }
    CanB_in_chip_reset = false;
    return 0;		// Failed to initialize
}

/*********************************************************/
/**                   Init CAN B  Bus                   **/
/*********************************************************/
void Init_CanB_bus (void)
{
    int j;

    for(j = 0; j<3; j++) {
        SEB.IN[j] = 0x00;
        SEB.SET_OUT[j] = 0x00;
        SEB.OUT[j] = 0x00;
        SEB.OUT_ACK[j] = 0x00;
    }
    SEB.IN_ACK = 0;
    SEB.ADDR = SEB_TEST_BRDADD;
    SEB.MsgReq = 0;
    SEB.io_Label_ix = 0;
    SEB.Online = 0;
    SEB.RxCnt = 0;
    SEB.TxReSendCnt = 0;
    SEB.Version = 0;
    SEB.Revision = 0;
    LabelCheckSum.word = CalculateLabelCheckSum();
    CanB_Clear();
    Restart_CanB_bus();
}

/*********************************************************/
/**                 Restart CAN B  Bus                  **/
/*********************************************************/
void Restart_CanB_bus(void)
{
//-	int i;
    int16 reset_count;

    CanB_tpix = 0;
    CanB_tgix = 0;
    CanB_rpix = 0;
    CanB_rgix = 0;

    CanBTxComplete = 1;

    _disable();

    // Setup up interrupt routine vector
    INTC_InstallINTCInterruptHandler(CanB_bus_off_and_warning_int, CAN_B_INT_BASE, 1);
    INTC_InstallINTCInterruptHandler(CanB_error_int, CAN_B_INT_BASE+1, 1);
    INTC_InstallINTCInterruptHandler(wrong_CanB_int, CAN_B_INT_BASE+3, 1);
    INTC_InstallINTCInterruptHandler(wrong_CanB_int, CAN_B_INT_BASE+4, 1);
    INTC_InstallINTCInterruptHandler(wrong_CanB_int, CAN_B_INT_BASE+5, 1);
    INTC_InstallINTCInterruptHandler(wrong_CanB_int, CAN_B_INT_BASE+6, 1);
    INTC_InstallINTCInterruptHandler(wrong_CanB_int, CAN_B_INT_BASE+7, 1);
    INTC_InstallINTCInterruptHandler(CanB_rcv_int, CAN_B_INT_BASE+8, 1);
    INTC_InstallINTCInterruptHandler(CanB_rcv_fifo_warning_int, CAN_B_INT_BASE+9, 1);
    INTC_InstallINTCInterruptHandler(CanB_rcv_fifo_overflow_int, CAN_B_INT_BASE+10, 1);
    INTC_InstallINTCInterruptHandler(CanB_trm_int_b8, CAN_B_INT_BASE+11, 1);

    if (CAN_B_ChipFreezeMode(c_Freeze_Start) == 1) {

        CAN_B.MCR.B.SOFTRST = 1;		// Soft Reset
        reset_count = 0;
        while ((CAN_B.MCR.B.SOFTRST == 1) && (reset_count < 1000))
            reset_count++;

        CAN_B.MCR.B.MDIS = 0;			// Enable module after reset

        // Clear of all interrupts
        CAN_B.IFLAG1.R = 0xFFFFFFFF;
        CAN_B.IFLAG2.R = 0xFFFFFFFF;

        // Disable of Interrupts
        CAN_B.IMASK1.R = 0;
        CAN_B.IMASK2.R = 0;

        CAN_B.MCR.B.BCC = 1;			// Allow backward compatibility since all mask will be set to 0xffff

//ZYM 10/19/2017
        CAN_B.MCR.B.MAXMB = 0x0B;		// Set to 12 (0xC - 1)
//-		CAN_B.MCR.B.MAXMB = 0x08;		// Set to 9 (0x9 - 1)

        CAN_B.MCR.B.WRNEN = 0;			// Warning interrupts
        CAN_B.MCR.B.SRXDIS = 1;			// Self Reception disabled
        CAN_B.MCR.B.FEN = 1;			// Enable RX fifo

//ZYM 10/19/2017
        CAN_B.MCR.B.ABTEN = 1;			// Abort enable
//-		CAN_B.MCR.B.AEN = 1;			// Abort enable
        CAN_B.MCR.B.LPRIOEN = 0;		// No local priority mode
//-		CAN_B.MCR.B.LPRIO_EN = 0;		// No local priority mode

        CAN_B.MCR.B.IDAM = 0;			// ID Acceptance mode (one full id per filter element)

        // Configure clock AND baud rate timer for 115200 baud

        /*
        	fTq = Bit Rate * Number of time Quanta = fCANCLK / Prescale Value
        	Prescale Value = fCANCLK/(Bit Rate * Number of Time Quanta)
        	Prescale Value = 108.75 Mhz / (115200 * 16) = 59.00065
        */

        CAN_B.CTRL.B.CLKSRC = 1; 		// System Clock 108.75 Mhz
        CAN_B.CTRL.B.PRESDIV = 58;		// Prescale Devision Factor + 1 = Prescale Value for 115200

        CAN_B.CTRL.B.RJW = 2;			// Resync Jump Width = RJW + 1 = 3
        // Total Tq = 16
        CAN_B.CTRL.B.PSEG1 = 7;			// Phase Segment 1 = (PSEG1 + 1) x Tq = 8 (also sync seg = 1)
        CAN_B.CTRL.B.PSEG2 = 5;			// Phase Segment 2 = (PSEG2 + 1) x Tq = 6
        CAN_B.CTRL.B.PROPSEG = 0;		// Propagation Segment time = (PROPSEG + 1) x Tq = 1
        CAN_B.CTRL.B.SMP = 1;			// Three samples

        CAN_B.CTRL.B.BOFFREC = 0;		// Automamtic recovery
        CAN_B.CTRL.B.TSYN = 0;			// Timer sync disabled
        CAN_B.CTRL.B.LBUF = 1;			// Lowest buffer transmitted first
        CAN_B.CTRL.B.LOM = 0;			// Listen only mode disabled

        CAN_B.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
        CAN_B.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
        CAN_B.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
        CAN_B.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled

        // Set Acceptance Mask

        CAN_B.RXIMR[0].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[1].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[2].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[3].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[4].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[5].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[6].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[7].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[8].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[9].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[10].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[11].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[12].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[13].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[14].R = 0x00000000;		// Enable all packets
        CAN_B.RXIMR[15].R = 0x00000000;		// Enable all packets

        // Set Receive ID
        CAN_B.MB[6].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
        CAN_B.MB[6].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
        CAN_B.MB[6].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
        CAN_B.MB[6].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID
        CAN_B.MB[7].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
        CAN_B.MB[7].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
        CAN_B.MB[7].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
        CAN_B.MB[7].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID

        CAN_B.MB[8].MSG.MCS.B.CODE = 0x08;		// write inactive mode
        CAN_B.MB[9].MSG.MCS.B.CODE = 0x08;		// write inactive mode
        CAN_B.MB[10].MSG.MCS.B.CODE = 0x08;		// write inactive mode
        CAN_B.MB[11].MSG.MCS.B.CODE = 0x08;		// write inactive mode

        if (CAN_B_ChipFreezeMode(c_Freeze_Stop) == 1) {

            // Enable interrupts
//ZYM 10/19/2017
            CAN_B.IMASK1.R = 0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
//-			CAN_B.IMASK1.R = 0x000001E0;	// MB 8 for transmit and 5-7 for receive

            CAN_B.IMASK2.R = 0x00;		// MB 32-63 NOT Enabled
            CAN_B.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
            CAN_B.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
            CAN_B.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
            CAN_B.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled
        }

    }
    _enable();
}

/*********************************************************/
/**                 CAN_B Wrong Interrupt               **/
/*********************************************************/
void wrong_CanB_int(void)
{
    CAN_B.IFLAG1.B.BUF00I = 1;
    CAN_B.IFLAG1.B.BUF01I = 1;
    CAN_B.IFLAG1.B.BUF02I = 1;
    CAN_B.IFLAG1.B.BUF03I = 1;
    CAN_B.IFLAG1.B.BUF04I = 1;
}

/*********************************************************/
/**            CAN_B Bus off and warning                **/
/*********************************************************/
void CanB_bus_off_and_warning_int (void)
{
    // Bus off, Transmit count warning, Receive Count warning
    CAN_B.ESR.B.BOFFINT = 1;
    CAN_B.ESR.B.TWRNINT = 1;
    CAN_B.ESR.B.RWRNINT = 1;
}

/*********************************************************/
/**                 CAN_B Error Interrupt               **/
/*********************************************************/
void CanB_error_int (void)
{
    // Bit 0, Bit 1, Acknowledge, CRC, Form or Stuffing Errors
    CAN_B.ESR.B.ERRINT = 1;
}

/*********************************************************/
/**                 CAN_B RX Interrupt                  **/
/*********************************************************/
void CanB_rcv_int (void)
{
    // Receive Interrupt (MB5)
    int rcv_fifo_cnt = 0;

    while ((CAN_B.IFLAG1.B.BUF05I == 1) && (rcv_fifo_cnt < 9)) {
        // Have a can message
        receive_CanB_in (0);
        CAN_B.IFLAG1.B.BUF05I = 1;				// Clears the bit
        rcv_fifo_cnt++;
    }
}

/*********************************************************/
/**               CAN_B Fifo Warning Interrupt          **/
/*********************************************************/
void CanB_rcv_fifo_warning_int (void)
{
    // Receive fifo warning (MB6)
    CAN_B.IFLAG1.B.BUF06I = 1;
}

/*********************************************************/
/**              CAN_B Fifo Overflow Interrupt          **/
/*********************************************************/
void CanB_rcv_fifo_overflow_int (void)
{
    // Receive Fifo Overrun Interrupt (MB7)
    CAN_B.IFLAG1.B.BUF07I = 1;
}

/*********************************************************/
/**            CAN_B TX Interrupt Buffer 8              **/
/*********************************************************/
void CanB_trm_int_b8 (void)
{
    // Transmit Interrupt

    CAN_B.IFLAG1.B.BUF08I = 1;
    CanBTxComplete = 1;
    CAN_B_Transmit(8);
}

/*********************************************************/
/**                    CAN_B Receive                    **/
/*********************************************************/

void receive_CanB_in (int16 BufIx)
{
    int16 i;
    int16 read_dly_cnt;
    union {
        uint8 b[4];			// byte: char 0 - 3
        uint32 dw;			// double word: int
    } uiu;					// unsigned integer union

    union {
        uint8 b[4];			// byte: char 0 - 3
        uint32 dw;			// double word: int
    } uiu_2;					// unsigned integer union

    read_dly_cnt = 0;
    while (((CAN_B.MB[BufIx].MSG.MCS.B.CODE & 0x09) == 0x01) && (read_dly_cnt < 100))
        read_dly_cnt ++;

    if ((CAN_B.MB[BufIx].MSG.MCS.B.CODE & 0x09) != 0x01) {
        // read of CS locks the receive buffer
        if (CanB_RX[CanB_rpix].ValidMsg == 0) {
            CanB_RX[CanB_rpix].ex_id = (uint8)CAN_B.MB[BufIx].MSG.MCS.B.IDE;
            CanB_RX[CanB_rpix].len = (uint8)CAN_B.MB[BufIx].MSG.MCS.B.LENGTH;
            CanB_RX[CanB_rpix].rtr = (uint8)CAN_B.MB[BufIx].MSG.MCS.B.RTR;

            if (CanB_RX[CanB_rpix].len > 8)
                CanB_RX[CanB_rpix].len = 8;
            if (CanB_RX[CanB_rpix].ex_id == 1) {	// Extended frame
                uiu.dw = (CAN_B.MB[BufIx].MSG.MID.R << 3);
                CanB_RX[CanB_rpix].msg = uiu.b[hb];	// Message
                CanB_RX[CanB_rpix].addr = uiu.b[mhb];	// Address
                CanB_RX[CanB_rpix].id2 = uiu.b[mlb];
                CanB_RX[CanB_rpix].id3 = uiu.b[lb];
            } else {
                uiu.dw = ((CAN_B.MB[BufIx].MSG.MID.R & 0x1FFC0000) << 1) ;
                uiu_2.dw = ((CAN_B.MB[BufIx].MSG.MID.R & 0x1FFC0000) >> 2);
                CanB_RX[CanB_rpix].msg = uiu.b[hb];	// Message
                CanB_RX[CanB_rpix].addr = uiu_2.b[mhb];    // Address
            }

            i = 0;
            while ((i < CanB_RX[CanB_rpix].len) && (i < 8)) {
                CanB_RX[CanB_rpix].data[i] = CAN_B.MB[BufIx].MSG.MDATA.B[i];
                i++;
            }
            CanB_RX[CanB_rpix].ValidMsg = 1;
        }

    }

    CanB_RX[CanB_rpix].time = (uint16)CAN_B.MB[BufIx].MSG.MCS.B.TIMESTAMP;		// Unlocks the buffer

    CanB_rpix++;
    if (CanB_rpix >= CanB_BufSize)
        CanB_rpix = 0;
}

/*********************************************************/
/**              Unpack CAN B RX Buffer                 **/
/*********************************************************/
void unpack_CanB_in (void)
{
    int16 loop_cnt = 0;
//-	int SebIx;

    while ((loop_cnt <= 4)  && (CanB_rgix != CanB_rpix)) {
        if (CanB_RX[CanB_rgix].ValidMsg == 1) {


            if (CanB_RX[CanB_rgix].rtr == 0) {
                // OK to unpack the data

                switch (CanB_RX[CanB_rgix].msg) {
                case 10:
                    // dont really care for this packet now
                    SEB.RxCnt = 3;
                    B_RxDebugCnt++;
                    SEB.Online =1;
                    break;
                case 12:
                    SEB.Version = CanB_RX[CanB_rgix].data[0];
                    SEB.Revision = CanB_RX[CanB_rgix].data[1];
                    SEB.RxCnt = 5;
                    SEB.Online =1;
                    timers[tCanB_Rx] = 0;
                    break;
                case 14:
                    SEB.IN[0] = CanB_RX[CanB_rgix].data[0];
                    SEB.IN[1] = CanB_RX[CanB_rgix].data[1];
                    SEB.IN[2] = CanB_RX[CanB_rgix].data[2];
                    B_RxCnt++;
                    timers[tCanB_Rx] = 0;
                    SEB.IN_ACK = 1;
                    SEB.RxCnt = 5;
                    SEB.Online =1;
                    break;
                case 16:
                    SEB.OUT_ACK[0] = CanB_RX[CanB_rgix].data[1];
                    SEB.OUT_ACK[1] = CanB_RX[CanB_rgix].data[2];
                    SEB.OUT_ACK[2] = CanB_RX[CanB_rgix].data[3];
                    SEB.RxCnt = 5;
                    SEB.Online =1;
                    break;
                case 46:
                    if(SEB.MsgReq == 0) {
                        SEB.MsgReq = 47;
                    }
                    timers[tCanB_Rx] = 0;
                    SEB.RxCnt = 5;
                    SEB.Online =1;
                    break;
                default:
                    break;
                }
            } else {
                if(SEB.MsgReq == 0)
                    SEB.MsgReq = CanB_RX[CanB_rgix].msg;
            }
            CanB_RX[CanB_rgix].ValidMsg = 0;
        }
        CanB_rgix++;
        if (CanB_rgix > CanB_BufSize)
            CanB_rgix = 0;
        loop_cnt++;
    }
}

/*********************************************************/
/**               Is CAN_B Tx Buffer Full               **/
/*********************************************************/
int16 CanB_IsTxBufFul(void)
{
    int16 availbuf;

    _disable();
    if (CanB_tpix == CanB_tgix)
        availbuf = CanB_BufSize + 1;
    else if (CanB_tpix > CanB_tgix)
        availbuf = (CanB_BufSize - CanB_tpix) + CanB_tgix + 1;
    else
        availbuf = CanB_tgix - CanB_tpix;

    _enable();
    if (availbuf > 9)
        return 0;
    else
        return 1;
}

/*********************************************************/
/**              Unpack CAN B TX Buffer                 **/
/*********************************************************/

void pack_CanB_out(unsigned char Msg, unsigned char Brd,unsigned char tx_req)
{
    unsigned char LABEL_IX =0;

    CAN_B.IMASK1.R = 0x00000000;
    if (CanB_tpix < CanB_BufSize)
        CanB_tpix++;
    else
        CanB_tpix = 0;
//ZYM 10/19/2017
    CAN_B.IMASK1.R = 0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
//-	CAN_B.IMASK1.R = 0x000001E0;	// MB 8 for transmit and 5-7 for receive

    if (tx_req == 1) {
        // Used to request a packet from a device
        CanB_TX[CanB_tpix].rtr = 1;
        CanB_TX[CanB_tpix].srr = 1;
        CanB_TX[CanB_tpix].len = 0;
        CanB_TX[CanB_tpix].ex_id = 1;		// Extended Id
        CanB_TX[CanB_tpix].msg = Msg;
        CanB_TX[CanB_tpix].addr = SEB.ADDR;
        CanB_TX[CanB_tpix].id2 = 0;
        CanB_TX[CanB_tpix].id3 = 0;
        CanB_TX[CanB_tpix].data[0] = 0;
        CanB_TX[CanB_tpix].data[1] = 0;
        CanB_TX[CanB_tpix].data[2] = 0;
        CanB_TX[CanB_tpix].data[3] = 0;
        CanB_TX[CanB_tpix].data[4] = 0;
        CanB_TX[CanB_tpix].data[5] = 0;
        CanB_TX[CanB_tpix].data[6] = 0;
        CanB_TX[CanB_tpix].data[7] = 0;

    } else switch (Msg) {

        case 11:
            CanB_TX[CanB_tpix].len = 5;
            CanB_TX[CanB_tpix].rtr = 0;
            CanB_TX[CanB_tpix].srr = 1;
            CanB_TX[CanB_tpix].ex_id = 1;
            CanB_TX[CanB_tpix].msg = Msg;
            CanB_TX[CanB_tpix].addr = SEB.ADDR;
            CanB_TX[CanB_tpix].id2 = 0;
            CanB_TX[CanB_tpix].id3 = 0;
            CanB_TX[CanB_tpix].data[0] = 0;
            CanB_TX[CanB_tpix].data[1] = 0;
            CanB_TX[CanB_tpix].data[2] = 0;
            CanB_TX[CanB_tpix].data[3] = 0;
            CanB_TX[CanB_tpix].data[4] = 0;
            CanB_TX[CanB_tpix].data[5] = 0;
            CanB_TX[CanB_tpix].data[6] = 0;
            CanB_TX[CanB_tpix].data[7] = 0;
            A_TxCnt++;
            break;
        case 13:
            CanB_TX[CanB_tpix].len = 8;
            CanB_TX[CanB_tpix].rtr = 0;
            CanB_TX[CanB_tpix].srr = 1;
            CanB_TX[CanB_tpix].ex_id = 1;
            CanB_TX[CanB_tpix].msg = Msg;
            CanB_TX[CanB_tpix].addr = SEB.ADDR;
            CanB_TX[CanB_tpix].id2 = 0;
            CanB_TX[CanB_tpix].id3 = 0;
            CanB_TX[CanB_tpix].data[0] = SEB.OUT[0];
            CanB_TX[CanB_tpix].data[1] = SEB.OUT[1];
            CanB_TX[CanB_tpix].data[2] = SEB.OUT[2];
            CanB_TX[CanB_tpix].data[3] = 0;
            CanB_TX[CanB_tpix].data[4] = 0;
            CanB_TX[CanB_tpix].data[5] = LabelCheckSum.byte[1];
            CanB_TX[CanB_tpix].data[6] = LabelCheckSum.byte[0];
            CanB_TX[CanB_tpix].data[7] = 0;
            break;

        case 15:
            CanB_TX[CanB_tpix].len = 3;
            CanB_TX[CanB_tpix].rtr = 0;
            CanB_TX[CanB_tpix].srr = 1;
            CanB_TX[CanB_tpix].ex_id = 1;
            CanB_TX[CanB_tpix].msg = Msg;
            CanB_TX[CanB_tpix].addr = SEB.ADDR;
            CanB_TX[CanB_tpix].id2 = 0;
            CanB_TX[CanB_tpix].id3 = 0;
            CanB_TX[CanB_tpix].data[0] = SEB.IN[0];
            CanB_TX[CanB_tpix].data[1] = SEB.IN[1];
            CanB_TX[CanB_tpix].data[2] = SEB.IN[2];
            CanB_TX[CanB_tpix].data[3] = 0;
            CanB_TX[CanB_tpix].data[4] = 0;
            CanB_TX[CanB_tpix].data[5] = 0;
            CanB_TX[CanB_tpix].data[6] = 0;
            CanB_TX[CanB_tpix].data[7] = 0;
            break;
        case 47:
            CanB_TX[CanB_tpix].len = 8;
            CanB_TX[CanB_tpix].rtr = 0;
            CanB_TX[CanB_tpix].srr = 1;
            CanB_TX[CanB_tpix].ex_id = 1;
            CanB_TX[CanB_tpix].msg = Msg;
            CanB_TX[CanB_tpix].addr = SEB.ADDR;
            CanB_TX[CanB_tpix].id2 = 0;
            CanB_TX[CanB_tpix].id3 = 0;
            LABEL_IX = 	SEB.io_Label_ix;
            CanB_TX[CanB_tpix].data[0] = LABEL_IX;
            CanB_TX[CanB_tpix].data[1] = SEB_LABEL[LABEL_IX][0];
            CanB_TX[CanB_tpix].data[2] = SEB_LABEL[LABEL_IX][1];
            CanB_TX[CanB_tpix].data[3] = SEB_LABEL[LABEL_IX][2];
            CanB_TX[CanB_tpix].data[4] = SEB_LABEL[LABEL_IX][3];
            CanB_TX[CanB_tpix].data[5] = SEB_LABEL[LABEL_IX][4];
            CanB_TX[CanB_tpix].data[6] = 0;
            CanB_TX[CanB_tpix].data[7] = 0;

            SEB.io_Label_ix++;
            if(SEB.io_Label_ix <= 11) {
                if(SEB.MsgReq == 0)
                    SEB.MsgReq = 47;
            } else
                SEB.io_Label_ix = 0;
            break;
        default:
            break;
        }
}

/*********************************************************/
/**              Transmit CAN B TX Buffer               **/
/*********************************************************/
void CAN_B_Transmit (int16 TxBufIx)
{
    int16 i;
    union {
        uint8 b[4];			// byte: char 0 - 3
        uint32 dw;			// double word: int
    } uiu;					// unsigned integer union

    if (CanB_tgix != CanB_tpix) {
        // There is data in the buffer

        CanBTxComplete = 0;

        if (CanB_tgix < CanB_BufSize)
            CanB_tgix++;
        else
            CanB_tgix = 0;

        if (CanB_TX[CanB_tgix].len > 8) {
            CanB_TX[CanB_tgix].len = 8;
        }

        if (CanB_TX[CanB_tgix].ex_id == 1) {
            uiu.b[hb] = CanB_TX[CanB_tgix].msg;		// Message
            uiu.b[mhb] = CanB_TX[CanB_tgix].addr;	// Address
            uiu.b[mlb] = 0;
            uiu.b[lb] = 0;
            CAN_B.MB[TxBufIx].MSG.MID.R = uiu.dw >> 3;		// priority bits get set to zero on shift
        } else {
            uiu.b[hb] = (uint8)(CanB_TX[CanB_tgix].msg & 0x3F);		// Message
            uiu.b[mhb] = (uint8)(CanB_TX[CanB_tgix].addr << 3);	// Address
            uiu.b[mlb] = 0;
            uiu.b[lb] = 0;
            CAN_B.MB[TxBufIx].MSG.MID.R = uiu.dw >> 3;		// priority bits get set to zero on shift
        }


        i = 0;
        while ((i<CanB_TX[CanB_tgix].len) && (i < 8)) {
            CAN_B.MB[TxBufIx].MSG.MDATA.B[i++] = CanB_TX[CanB_tgix].data[i];
        }


        CAN_B.MB[TxBufIx].MSG.MCS.B.LENGTH = CanB_TX[CanB_tgix].len;

        if (CanB_TX[CanB_tgix].rtr == 1)
            CAN_B.MB[TxBufIx].MSG.MCS.B.RTR = 1;
        else
            CAN_B.MB[TxBufIx].MSG.MCS.B.RTR = 0;

        if (CanB_TX[CanB_tgix].ex_id == 1)
            CAN_B.MB[TxBufIx].MSG.MCS.B.IDE = 1;
        else
            CAN_B.MB[TxBufIx].MSG.MCS.B.IDE = 0;

        if (CanB_TX[CanB_tgix].srr == 1)
            CAN_B.MB[TxBufIx].MSG.MCS.B.SRR = 1;
        else
            CAN_B.MB[TxBufIx].MSG.MCS.B.SRR = 0;

        CAN_B.MB[TxBufIx].MSG.MCS.B.CODE = 0x0C;		// Initiate transmit
    }
}

/*********************************************************/
/**                  CAN B Control                      **/
/*********************************************************/

void CAN_B_Control(void)
{
//-	unsigned char  i;
    unsigned char MsgReq = 0;
    static unsigned char SebIndex = 0;
    static int DataReq = 0;
    static int DataReqLoopDelay =0;
    static int TxReSendCnt = 0;
    int Status;

    unpack_CanB_in();
    Status = CAN_B.ESR.R;

    if(CAN_B.ESR.B.ERRINT == 1) {
        Status = CAN_B.ESR.R;
        CAN_B.ESR.B.ERRINT = 1;
    }

    CanB_TxErrCnt = (uint8)CAN_B.ECR.B.TXECNT;
    CanB_RxErrCnt = (uint8)CAN_B.ECR.B.RXECNT;

    if(CAN_B.ESR.B.BOFFINT == 1) {
        CAN_B.ESR.B.BOFFINT = 1;
        Restart_CanB_bus();
    }

    if(CanB_IsTxBufFul() == 0) {

        if((SEB.SET_OUT[0]!= SEB.OUT[0])||
                (SEB.SET_OUT[1]!= SEB.OUT[1])||
                (SEB.SET_OUT[2]!= SEB.OUT[2])) {
            SEB.OUT[0] = SEB.SET_OUT[0];
            SEB.OUT[1] = SEB.SET_OUT[1];
            SEB.OUT[2] = SEB.SET_OUT[2];
            pack_CanB_out(13,SEB_TEST_BRDADD,0);
            SEB.TxReSendCnt = 0;
            timers[tCanB_Tx] = 0;

        } else if((SEB.OUT_ACK[0]!= SEB.OUT[0])||
                  (SEB.OUT_ACK[1]!= SEB.OUT[1])||
                  (SEB.OUT_ACK[2]!= SEB.OUT[2])) {
            if(timers[tCanB_Tx] >= 2 ) {
                pack_CanB_out(13,SEB_TEST_BRDADD,0);
                timers[tCanB_Tx] = 0;
                SEB.TxReSendCnt++;
            }
            if(SEB.TxReSendCnt > 10) {
                SEB.TxReSendCnt = 0;
                SEB.OUT_ACK[0]= SEB.OUT[0];
                SEB.OUT_ACK[1]= SEB.OUT[1];
                SEB.OUT_ACK[2]= SEB.OUT[2];
            }
        }

        else {
            if(timers[tCanB_Tx] > 6) {

                SEB.OUT[0] = SEB.SET_OUT[0];
                SEB.OUT[1] = SEB.SET_OUT[1];
                SEB.OUT[2] = SEB.SET_OUT[2];
                pack_CanB_out(13,SEB_TEST_BRDADD,0);
                SEB.TxReSendCnt = 0;
                timers[tCanB_Tx] = 0;
            }
        }

        // Check Input

        if(SEB.IN_ACK == 1) {
            SEB.IN_ACK = 0;
            pack_CanB_out(15,SEB_TEST_BRDADD,0);
        } else if(SEB.MsgReq != 0) {
            MsgReq = SEB.MsgReq;
            SEB.MsgReq = 0;
            pack_CanB_out(MsgReq,SEB_TEST_BRDADD,0);

        } else if(B_req == 1) {
            pack_CanB_out(10,SEB_TEST_BRDADD,1);
            B_req = 0;
            B_TxDebugCnt++;
        } else if(timers[tCanB_Rx] > 3) {
            if(SEB.Version == 0) {
                pack_CanB_out(11,SEB_TEST_BRDADD,0);
                timers[tCanB_Rx] = 0;
                if(SEB.RxCnt > 0)
                    SEB.RxCnt--;
            } else {
                pack_CanB_out(14,SEB_TEST_BRDADD,1);
                B_TxCnt++;
                timers[tCanB_Rx] = 0;
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
    CAN_B.IMASK1.R = 0x00000000;
    if (CanB_tgix != CanB_tpix) {
        if((CAN_B.MB[8].MSG.MCS.B.CODE == 0x08 || CAN_B.MB[8].MSG.MCS.B.CODE == 0x04 ) && (CanBTxComplete == 1)) {
            CAN_B_Transmit(8);
        }
    }

//ZYM 10/16/2017
//-	CAN_B.IMASK1.R = 0x000001E0;
    CAN_B.IMASK1.R=0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
}

/*********************************************************/
/**                Clear Can A parameter                **/
/*********************************************************/
void  CanB_Clear(void)
{
//-	int i;

    /*
    CanB_tpix = 0;					// can transmit buffer put index
    CanB_tgix = 0;					// can transmit buffer get index
    CanB_rpix = 0;					// can Receive buffer put index
    CanB_rgix = 0;					// can Receive buffer get index


    for(i = 0;i<(CanB_BufSize + 1);i++)
    {
    	CanB_TX[i].ValidMsg = 0;
    	CanB_RX[i].ValidMsg = 0;
    }


    timers[tCanB_Rx] = 0;
    timers[tCanB_Tx] = 0;
    */

}

/*********************************************************/
/**                Clear Can A parameter                **/
/*********************************************************/
static unsigned short int CalculateLabelCheckSum(void)
{
    int i,j;
    unsigned short int sum = 0;

    for(i = 0; i<12; i++) {
        for(j= 0; j<5; j++) {
            sum += SEB_LABEL[i][j];
        }
    }
    return sum;
}



