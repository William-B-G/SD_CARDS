//ZYM 3-15-2017
#include "MPC5668G.h"

//ZYM 10/19/2017
#include "global.h"

//-#include "iodefine.h"
//-#include "IntcInterrupts.h"
//-#include "Timer.h"
//-#include "Util.h"
//-#include "cedes_can.h"
#include <math.h>
#include <string.h>
//-#include "comm_test.h"


//ZYM 3-15-2017
#define d_CEDES_CAN 1
//-#include "global.h"



//*********************************************************************
//*********************************************************************
//******* Routines to run the Cedes Absolute Positioning System CAN Bus
//*********************************************************************
//*********************************************************************

/*
Can Port Device Map

Can Protocol


Baud rate 250 K bits/s		Default

CAN PROTOCOL  (Standard address bits only)

Frame/Length     
Identifier 1 (6bits) (Addr Bits: 10-7) (Device Message:Bits 6,5)	
Identifier 2 (5bits) (Device Message:Bits 4-0)	 	
		Data Byte 1	 
		Data Byte 2
		Data Byte 3 
		Data Byte 4
		Data Byte 5
		Data Byte 6
		Data Byte 7
		Data Byte 8



Packet #11		Selector Count and Speed
		Byte 0: Sequence Count
		Byte 1: High Byte Position Count
		Byte 2: Middle Byte Position Count
		Byte 3: Low Byte Position Count
		Byte 4: High Byte Car speed
		Byte 5: Low Byte Car speed
		Byte 6: High Byte of CRC 16
		Byte 7: Low Byte of CRC 16

Packet #12		Selector Error and Status
Byte 0: Sequence Count

Byte 1: Error Byte
bit 7-6 Pos1 Err: Result of the internal position and velocity cross-check (channel 1 to channel 2)
00 = Position1 and Velocity1 OK (position/velocity is safe)
01 = Position1 Error (position comparison failed)
10 = Velocity1 Error (velocity comparison failed)
11 = Position1 and Velocity1 Error(position and velocity comparison failed)
bit 5-4 Pos2 Err: Result of the internal position and velocity cross-check (channel 2 to channel 1)
00 = Position2 and Velocity2 OK (position and velocity is safe)
01 = Position2 Error (position comparison failed)
10 = Velocity2 Error (velocity comparison failed)
11 = Position2 and Velocity2 Error (position and velocity comparison failed)
bit 3 Reserved: Transmitted as '0'
bit 2-0 General Error:
000 = OK, no general error
001 = Cannot Read Tape
010 = Alignment error - too close
011 = Alignment error - too far
100 = Alignment error - too far left
101 = Alignment error - too far right
110 = Internal Fault
111 = Communication Error		

Byte 2: High Byte of Status
3: Low Byte of Status
bit 15 Reserved: Transmitted as '0'
bit 14-12 Alignment:
000 = OK, no general Error
001 = Cannot Read Tape
010 = Alignment needed - too close
011 = Alignment needed - too far
100 = Alignment needed - too far left
101 = Alignment needed - too far right
110 = n/a
111 = n/a
bit 11-8 Revision: 4bit revision code
Note: This value is a combination of the hardware and software version. Changes will be disclosed with a release note.
bit 7-5 Reserved: Transmitted as '0'
bit 4 IN2: Compression Compensation Input 2
0 = low
1 = high
bit 3 IN1: Compression Compensation Input 1
0 = low
1 = high
bit 2-1 Contrast (b:w) :
00 = Code Tape Contrast OK (> 1:10, typ 1:20)
01 = Service Recommended (> 1:7)
10 = Warning (> 1:3)
11 = Stopped (< 1:3)
bit 0 Clip: Position Indicator Clip
0 = Clip not detected
1 = Clip detected						 

Byte 4: High Byte of CRC 16
5: Low Byte of CRC 16


*/




// Can Routine declarations
int16 CedChipFreezeMode(unsigned char set);
void ced_init_can_bus (int16 baud_change);
void Ced_Pack_Can_Interrupt(void);
void ced_wrong_can_int (void);
void CED_can_bus_off_and_warning_int (void);
void CED_can_error_int (void);
void CED_can_rcv_int (void);
void CED_can_rcv_fifo_warning_int (void);
void CED_can_rcv_fifo_overflow_int (void);
void CED_can_trm_int_b8 (void);
void CED_can_trm_int_b9 (void);
void CED_can_trm_int_b10 (void);
void CED_can_trm_int_b11 (void);
void Ced_Can_Control (void);
void ced_transmit (int16 TxBufIx);
void ced_receive_can_in (int16 BufIx);
void Tx_Ced_Can_Msg(int16 TxBufIx);

void Ced_Pack_Can (unsigned char msg, unsigned char tx_req);
void Ced_Unpack_Can (void);
unsigned char Ced_Put_Tx_Msg_Req (unsigned char msg);
unsigned char Ced_Get_Tx_Msg_Req (void);
int16 IsSelectorTxBufFul(void);
uint16 CRCSelector16 (unsigned char *data, int16 cnt);

//ZYM 3-15-2017
uint16 Ced_CRC16 (unsigned char *data, int16 cnt);

int16 ced_can_online = 0;		// ced can online
int16 ced_aps_can_error = 0;		// ced aps can error
int16 ced_aps_fault = 0;			// ced aps fault

uint16 can_ced_crc16 = 0;
uint16 can_ced_calc_crc16 = 0;
int16 can_ced_dir = 0;
int16 can_ced_seq_cnt = 0;
int16 prev_can_ced_seq_cnt = 0;
int16 can_ced_seq_cnt_2 = 0;
int16 prev_can_ced_seq_cnt_2 = 0;
int16 ced_data_in_cnt = 0;
int16 ced_data_in_capture_cnt = 0;
int16 ced_data_in_2_cnt = 0;
int16 ced_data_in_2_capture_cnt = 0;

int32 test_msg=0;
int32 test_msg1=0;
int32 test_msg2=0;
int32 test_msg3=0;
int32 test_msg4=0;


#define coom_dec_count  500
#define comm_min_count	500
#define comm_inti_count 8500
#define comm_test_mcnt	8



//ZYM 3-21-2017
//check "comm_test.c" for why set this initial pluse count
int32 tsm_cedes_pls=comm_inti_count;
 



// Cedes CAN Bus Selector - Direct access to video position data
struct CED_can_type_tap{
	int32	position_count;
	int16	velocity;
	uint8 	vel_dir;			// velocity direction
	union {
		uint16 R;
		struct {
			uint16:1;
			uint16 ALIGN:3;
			uint16 REV:4;
			uint16:3;
			uint16 IN1:1;
			uint16 IN2:1;
			uint16 CONTRAST:2;
			uint16 CLIP:1;
		} B;
	} Status;

	union {
		uint8 R;
		struct {
			uint8 CHANNEL1:2;
			uint8 CHANNEL2:2;
			uint8:1;
			uint8 GENERAL:3;
		} B;
	} Error;
	bool 	online;
	uint8	online_cnt;
	int16	tx_cnt;
	int16	rx_cnt;
}CED_can;						// Cedes Selector data CAN channel 1 - direct to CEDes device 
 

int16 ced_can_init_count = 0;
int16 ced_init_mode_cnt = 0;
bool ced_in_chip_reset = false;

int32 ced_can_status = 0;

int16 ced_can_init_error=0;
/*
Can Error  
Bit			Description
00000001	Chip would not come out of reset mode during initialization
00000010	Chip would not go into reset mode during initialization
00000100	Can Bus off error
00001000	Receive Buffer not released
00010000	Transmit Buffer not released
00100000	Transmit length error
*/

int16 ced_can_error_data=0;
#define e_inv_pack 4
#define e_inv_unpk 12
#define c_Freeze_Start		0x01	// put chip in freeze mode
#define c_Freeze_Stop		0x00	// Takes chip out of freeze mode

int16 ced_can_int_count = 0;
int16 ced_can_tx_err_cnt = 0;
int16 ced_can_rx_err_cnt = 0;

int16 ced_rcv_fifo_cnt = 0;
int16 ced_rcv_int_fail_cnt = 0;
int16 ced_WCI_cnt=0;	// Wrong can interrupt
int16 ced_BOI_cnt=0;	// Bus Off and tx/rx warning interrupt count
int16 ced_ERI_cnt=0;	// Error Interrupt count
int16 ced_RI_cnt=0;		// Receive fifo interrupt count
int16 ced_FWI_cnt=0;	// Fifo warning interrupt count
int16 ced_FOI_cnt=0;	// Fifo Overflow interrupt count
int16 ced_TI8_cnt=0;	// Transmit buffer 8 interrupt count
int16 ced_TI9_cnt=0;	// Transmit buffer 9 interrupt count
int16 ced_TI10_cnt=0;	// Transmit buffer 10 interrupt count
int16 ced_TI11_cnt=0;	// Transmit buffer 11 interrupt count
int16 ced_ack_error_cnt = 0;
int16 ced_bus_off_cnt = 0;
int16 ced_bus_idle_cnt = 0;

int16 ced_tx_B08_fail_cnt = 0;
int16 ced_tx_B09_fail_cnt = 0;
int16 ced_tx_B10_fail_cnt = 0;
int16 ced_tx_B11_fail_cnt = 0;

int32 CED_can_debug_1=0;
int32 CED_can_debug_2=0;
int32 CED_can_debug_3=0;
int32 CED_can_debug_4=0;
int32 CED_can_debug_5=0;
int32 CED_can_debug_6=0;


unsigned char CED_can_tx_msg_req[6];


// Receive Buffer

#define c_ced_rx_buf_siz 10

struct tag_CED_can_rx {
	unsigned char valid_msg;	// 1 when valid message in buffer
	unsigned char len;
	unsigned char ex_id;	// flag for extended id mode
	unsigned char rtr;		// Remote Transmission Request
	unsigned char msg;		// Message
	unsigned char addr;		// Address
	uint32 id;
	unsigned char data [8];
	uint16 time;
} CED_can_rx[c_ced_rx_buf_siz+1];


static int16 scrpix = 0;					// selector can receive buffer put index
static int16 scrgix = 0;					// selector can receive buffer get index
static int16 sctpix = 0;					// selector can transmit buffer put index
static int16 sctgix = 0;					// selector can transmit buffer get index
static int16 ced_availbuf = 0;				// transmit buffer space available


// Transmit buffer 
#define c_ced_tx_buf_siz 10

struct tag_CED_can_tx {
	unsigned char len;
	unsigned char ex_id;	// flag for extended id mode
	unsigned char rtr;		// Remote Transmission Request
	unsigned char srr;		// Substitute Remote Request
	unsigned char msg;		// Message
	unsigned char addr;		// Address
	uint32 id;
	unsigned char data [8];
} CED_can_tx[c_ced_tx_buf_siz+1];

static union {
	uint8 b[4];			// byte: char 0 - 3
	uint32 dw;			// double word: int
}uiu;					// unsigned integer union

static union {
	uint8 b[4];			// byte: char 0 - 3
	uint32 dw;			// double word: int
}uiu_2;					// unsigned integer union


typedef union
{
	uint16 word;
	uint8 byte[2];

}COMMWORD_TYPEE;

typedef union
{
	uint32 dword;
	uint16 word[2];
	uint8 byte[4];
}COMMDWORD_TYPE; 


union
{
	int8 byte[4];
	int32 dw;

}rcv_dword;


struct
{
	int8 ver :1;	
	int8 rev :1;
	int8 UNUSED :6; 
}NTS_ver_rev;




// CAN_A Int Base = 127			Car Top CAN
// CAN_B Int Base = 157 		CAN Open Encoder
// CAN_C Int Base = 178			Group CAN
// CAN_D Int Base = 199			Mechine Room CAN


//#if (Simulator == 0)
//ZYM 3-15-2017
//#define T_SEL_CAN CAN_A
//#define T_SEL_CAN_INT_BASE 127


//ZYM 3-15-2017
//working as CAN_C
#define T_SEL_CAN CAN_C
#define T_SEL_CAN_INT_BASE 178

extern int32 DPP_to_ft(int32 dpp_local);
extern int32 DPP_to_in(int32 dpp_local);
extern int32 DPP_to_cedes_pls(int32 dpp_local);
extern int32 DPP_to_mm(int32 dpp_local);


union galx_1121tester_NTS_Modify_Output_tag
{
	uint8 byte;
	struct 
	{
		uint8 UNUSED : 2;
		uint8 PAL_FAULT_out   : 1;  //5
		uint8 SEL_OK_NTS_out  : 1;	//4
		uint8 NTS_FLT_out 	  : 1;	//3
		uint8 SPD_75_NTS_out  : 1;	//2
		uint8 SPD_150_NTS_out : 1;	//1	
		uint8 DZ_NTS_out      : 1;	//0
	};
} galx_1121tester_NTS_Modify_Output;


union galx_1121tester_NTS_Modify_Input_tag
{
	uint8 buff[1];
	
	struct galx_1121tester_NTS_Modify_tag
	{
		union
		{
			uint8 byte;
			struct 
			{		
				uint8 UNUSED 		  : 7;	
				uint8 test_mode_in    : 1;	//0										
			}B;
		}buff0;		
	}ST;	
} galx_1121tester_NTS_Modify_Input;


int16 Copy_galx_1121tester_NTS_Modify_Output_byte=0;









//*******************************************************************
// Sends the freeze bit to the chip
// Value of 1 puts the chip in freeze mode
// Value of 0 takes is out of freeze mode
// Have to see the freeze mode acknowledge bit gets set
//*******************************************************************
int16 CedChipFreezeMode(unsigned char set)
{


	if (ced_in_chip_reset == true)
		return 0;

	ced_in_chip_reset = true;

	ced_init_mode_cnt = 0;

	T_SEL_CAN.MCR.B.MDIS = 0;			// Enable module after reset
	T_SEL_CAN.MCR.B.FRZ = 1;			// Set or clear the freeze bit
	T_SEL_CAN.MCR.B.HALT = set;			// Clear halt mode


	while (ced_init_mode_cnt < 200)
	{
		if (T_SEL_CAN.MCR.B.FRZACK == set)
		{
			ced_in_chip_reset = false;
			return 1;
		}
		ced_init_mode_cnt++;
	}
	ced_in_chip_reset = false;
	return 0;		// Failed to initialize
}

// **************************************************
// ****  Initialize the ECAN 1000 CAN interface Board
// **************************************************


void ced_init_can_bus (int16 baud_change)
{	 
	// baud_change = 0: use parameter
	// baud_change = 1: set 125K
	// baud_change = 2: set 250k
	int16 reset_count;

	// See if the board is there
	ced_can_init_count++;

	_disable();

	/*  Can Interrupt vectors

	178	CAN C Bus Of and Warning
	179 CAN C Error
	180 CAN C Wake Up
	181 CAN C Buffer 0
	182 CAN C Buffer 1
	183 CAN C Buffer 2
	184 CAN C Buffer 3
	185 CAN C Buffer 4
	186 CAN C Buffer 5
	187 CAN C Buffer 6
	188 CAN C Buffer 7
	189 CAN C Buffer 8
	190 CAN C Buffer 9
	191 CAN C Buffer 10
	192 CAN C Buffer 11
	193 CAN C Buffer 12
	194 CAN C Buffer 13
	195 CAN C Buffer 14
	196 CAN C Buffer 15
	197 CAN C Buffer 16-31
	198 CAN C Buffer 32-63



	*/


	// Setup up interrupt routine vector
	INTC_InstallINTCInterruptHandler(CED_can_bus_off_and_warning_int, T_SEL_CAN_INT_BASE, 1);  
	INTC_InstallINTCInterruptHandler(CED_can_error_int, T_SEL_CAN_INT_BASE+1, 1);  
	INTC_InstallINTCInterruptHandler(ced_wrong_can_int, T_SEL_CAN_INT_BASE+3, 1);  
	INTC_InstallINTCInterruptHandler(ced_wrong_can_int, T_SEL_CAN_INT_BASE+4, 1);  
	INTC_InstallINTCInterruptHandler(ced_wrong_can_int, T_SEL_CAN_INT_BASE+5, 1);  
	INTC_InstallINTCInterruptHandler(ced_wrong_can_int, T_SEL_CAN_INT_BASE+6, 1);  
	INTC_InstallINTCInterruptHandler(ced_wrong_can_int, T_SEL_CAN_INT_BASE+7, 1);  
	INTC_InstallINTCInterruptHandler(CED_can_rcv_int, T_SEL_CAN_INT_BASE+8, 1);  
	INTC_InstallINTCInterruptHandler(CED_can_rcv_fifo_warning_int, T_SEL_CAN_INT_BASE+9, 1);  
	INTC_InstallINTCInterruptHandler(CED_can_rcv_fifo_overflow_int, T_SEL_CAN_INT_BASE+10, 1);  
	INTC_InstallINTCInterruptHandler(CED_can_trm_int_b8, T_SEL_CAN_INT_BASE+11, 1);  
	INTC_InstallINTCInterruptHandler(CED_can_trm_int_b9, T_SEL_CAN_INT_BASE+12, 1);  
	INTC_InstallINTCInterruptHandler(CED_can_trm_int_b10, T_SEL_CAN_INT_BASE+13, 1);  
	INTC_InstallINTCInterruptHandler(CED_can_trm_int_b11, T_SEL_CAN_INT_BASE+14, 1);  

	// Send init to can board
	if (CedChipFreezeMode(c_Freeze_Start) == 1)
	{

		ced_can_init_count++;

		T_SEL_CAN.MCR.B.SOFTRST = 1;		// Soft Reset
		reset_count = 0;
		while ((T_SEL_CAN.MCR.B.SOFTRST == 1) && (reset_count < 1000))
			reset_count++;

		T_SEL_CAN.MCR.B.MDIS = 0;			// Enable module after reset

		// Clear of all interrupts
		T_SEL_CAN.IFLAG1.R = 0xFFFFFFFF;
		T_SEL_CAN.IFLAG2.R = 0xFFFFFFFF;

		// Disable of Interrupts
		T_SEL_CAN.IMASK1.R = 0;
		T_SEL_CAN.IMASK2.R = 0;

		T_SEL_CAN.MCR.B.BCC = 1;			// Allow backward compatibility since all mask will be set to 0xffff
		T_SEL_CAN.MCR.B.MAXMB = 0x0B;		// Set to 12 (0xC - 1)
		T_SEL_CAN.MCR.B.WRNEN = 0;			// Warning interrupts
		T_SEL_CAN.MCR.B.SRXDIS = 1;			// Self Reception disabled
		T_SEL_CAN.MCR.B.FEN = 1;			// Enable RX fifo
		T_SEL_CAN.MCR.B.ABTEN = 1;			// Abort enable
		T_SEL_CAN.MCR.B.LPRIOEN = 0;		// No local priority mode
		T_SEL_CAN.MCR.B.IDAM = 0;			// ID Acceptance mode (one full id per filter element)


		// Configure clock AND baud rate timer for 250,000 baud

		/*	
		fTq = Bit Rate * Number of time Quanta = fCANCLK / Prescale Value
		Prescale Value = fCANCLK/(Bit Rate * Number of Time Quanta)
		Prescale Value = 40 Mhz / (250000 * 16) = 10.000
		*/


		T_SEL_CAN.CTRL.B.CLKSRC = 0; 		// System Clock 40.0 Mhz
		if (baud_change == 0)
		{

			//if (fvars[fvencbaud] == 0)
			T_SEL_CAN.CTRL.B.PRESDIV = 9;		// Prescale Devision Factor + 1 = Prescale Value for 250000
			//else 
			//	T_SEL_CAN.CTRL.B.PRESDIV = 19;		// Prescale Devision Factor + 1 = Prescale Value for 125000
		}
		else if (baud_change == 1)
			T_SEL_CAN.CTRL.B.PRESDIV = 19;		// Prescale Devision Factor + 1 = Prescale Value for 125000
		else
			T_SEL_CAN.CTRL.B.PRESDIV = 9;		// Prescale Devision Factor + 1 = Prescale Value for 250000

		T_SEL_CAN.CTRL.B.RJW = 2;			// Resync Jump Width = RJW + 1 = 3
		// Total Tq = 16
		T_SEL_CAN.CTRL.B.PSEG1 = 7;			// Phase Segment 1 = (PSEG1 + 1) x Tq = 8 (also sync seg = 1)
		T_SEL_CAN.CTRL.B.PSEG2 = 5;			// Phase Segment 2 = (PSEG2 + 1) x Tq = 6
		T_SEL_CAN.CTRL.B.PROPSEG = 0;		// Propagation Segment time = (PROPSEG + 1) x Tq = 1
		T_SEL_CAN.CTRL.B.SMP = 1;			// Three samples

		T_SEL_CAN.CTRL.B.BOFFREC = 0;		// Automamtic recovery
		T_SEL_CAN.CTRL.B.TSYN = 0;			// Timer sync disabled
		T_SEL_CAN.CTRL.B.LBUF = 1;			// Lowest buffer transmitted first
		T_SEL_CAN.CTRL.B.LOM = 0;			// Listen only mode disabled

		T_SEL_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
		T_SEL_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
		T_SEL_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
		T_SEL_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled


		// Set Acceptance Mask

		T_SEL_CAN.RXIMR[0].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[1].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[2].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[3].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[4].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[5].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[6].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[7].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[8].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[9].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[10].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[11].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[12].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[13].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[14].R = 0x00000000;		// Enable all packets
		T_SEL_CAN.RXIMR[15].R = 0x00000000;		// Enable all packets

		// Set Receive ID
		T_SEL_CAN.MB[6].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		T_SEL_CAN.MB[6].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		T_SEL_CAN.MB[6].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		T_SEL_CAN.MB[6].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID
		T_SEL_CAN.MB[7].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		T_SEL_CAN.MB[7].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		T_SEL_CAN.MB[7].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		T_SEL_CAN.MB[7].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID

		T_SEL_CAN.MB[8].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		T_SEL_CAN.MB[9].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		T_SEL_CAN.MB[10].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		T_SEL_CAN.MB[11].MSG.MCS.B.CODE = 0x08;		// write inactive mode

		if (CedChipFreezeMode(c_Freeze_Stop) == 1)
		{		// initialize can board
			ced_can_init_count++;

			// Enable interrupts 
			T_SEL_CAN.IMASK1.R = 0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
			T_SEL_CAN.IMASK2.R = 0x00;		// MB 32-63 NOT Enabled
			T_SEL_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
			T_SEL_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
			T_SEL_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
			T_SEL_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled
		}
		else
		{
			// Set error code
			ced_can_init_error = 0x01;
		}		
	}
	else
	{
		// Set error code
		ced_can_init_error = 0x02;
	} 
	_enable();
}


// **************************************************
// ********  Can Board Interrupt Routines  **********
// **************************************************

void ced_wrong_can_int (void)
{
	T_SEL_CAN.IFLAG1.B.BUF00I = 1;
	T_SEL_CAN.IFLAG1.B.BUF01I = 1;
	T_SEL_CAN.IFLAG1.B.BUF02I = 1;
	T_SEL_CAN.IFLAG1.B.BUF03I = 1;
	T_SEL_CAN.IFLAG1.B.BUF04I = 1;
	ced_WCI_cnt++;
}
void CED_can_bus_off_and_warning_int (void)
{		// Bus off, Transmit count warning, Receive Count warning
	T_SEL_CAN.ESR.B.BOFFINT = 1;
	T_SEL_CAN.ESR.B.TWRNINT = 1;
	T_SEL_CAN.ESR.B.RWRNINT = 1;
	ced_BOI_cnt++;
}

void CED_can_error_int (void)
{		// Bit 0, Bit 1, Acknowledge, CRC, Form or Stuffing Errors
	T_SEL_CAN.ESR.B.ERRINT = 1;
	ced_ERI_cnt++;
}

void CED_can_rcv_int (void)
{	// Receive Interrupt (MB5)
	ced_RI_cnt++;

	ced_rcv_fifo_cnt = 0;
	while ((T_SEL_CAN.IFLAG1.B.BUF05I == 1) && (ced_rcv_fifo_cnt < 9))
	{			// Have a can message
		ced_receive_can_in (0);
		T_SEL_CAN.IFLAG1.B.BUF05I = 1;				// Clears the bit
		ced_rcv_fifo_cnt++;
	}
	if (ced_rcv_fifo_cnt > 1)
		CED_can_debug_5 = ced_rcv_fifo_cnt;
}

void CED_can_rcv_fifo_warning_int (void)
{	// Receive fifo warning (MB6)
	T_SEL_CAN.IFLAG1.B.BUF06I = 1;
	ced_FWI_cnt++;
}

void CED_can_rcv_fifo_overflow_int (void)
{	// Receive Fifo Overrun Interrupt (MB7)
	T_SEL_CAN.IFLAG1.B.BUF07I = 1;
	ced_FOI_cnt++;
}

void CED_can_trm_int_b8 (void)
{	// Transmit Interrupt
	ced_TI8_cnt++;
	T_SEL_CAN.IFLAG1.B.BUF08I = 1;
	ced_transmit(8);
}

void CED_can_trm_int_b9 (void)
{	// Transmit Interrupt
	ced_TI9_cnt++;
	T_SEL_CAN.IFLAG1.B.BUF09I = 1;
	ced_transmit(9);
}

void CED_can_trm_int_b10 (void)
{	// Transmit Interrupt
	ced_TI10_cnt++;
	T_SEL_CAN.IFLAG1.B.BUF10I = 1;
	ced_transmit(10);
}

void CED_can_trm_int_b11 (void)
{	// Transmit Interrupt
	ced_TI11_cnt++;
	T_SEL_CAN.IFLAG1.B.BUF11I = 1;
	ced_transmit(11);
}

//************************************************************
// Receive the can input
//************************************************************

void ced_receive_can_in (int16 BufIx)
{
	int16 i;
	int16 read_dly_cnt;
	
	
	read_dly_cnt = 0;
	while (((T_SEL_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) == 0x01) && (read_dly_cnt < 100))
		read_dly_cnt ++;			// checking if buffer is busy

	if (read_dly_cnt > 1)
		CED_can_debug_6 = read_dly_cnt;

	if ((T_SEL_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) != 0x01)
	{			// read of CS locks the receive buffer
		if (CED_can_rx[scrpix].valid_msg == 0)
		{
			CED_can_rx[scrpix].ex_id = (uint8)T_SEL_CAN.MB[BufIx].MSG.MCS.B.IDE;
			CED_can_rx[scrpix].len = (uint8)T_SEL_CAN.MB[BufIx].MSG.MCS.B.LENGTH;
			CED_can_rx[scrpix].rtr = (uint8)T_SEL_CAN.MB[BufIx].MSG.MCS.B.RTR;

			if (CED_can_rx[scrpix].len > 8)
				CED_can_rx[scrpix].len = 8;

			CED_can_rx[scrpix].id = T_SEL_CAN.MB[BufIx].MSG.MID.R;

			i = 0;
			while ((i < CED_can_rx[scrpix].len) && (i < 8))
			{
				CED_can_rx[scrpix].data[i] = T_SEL_CAN.MB[BufIx].MSG.MDATA.B[i];
				i++;
			}
			CED_can_rx[scrpix].valid_msg = 1;  
		}

	}
	else
	{
		CED_can_debug_4 = T_SEL_CAN.MB[BufIx].MSG.MCS.B.CODE;
		ced_rcv_int_fail_cnt++;	
	}

	CED_can_rx[scrpix].time = (uint16)T_SEL_CAN.MB[BufIx].MSG.MCS.B.TIMESTAMP;		// Unlocks the buffer

	scrpix++;
	if (scrpix >= c_ced_rx_buf_siz)
		scrpix = 0;
}


//************************************************************
//*********  Check to see if the Transmit buffer is full *****
//************************************************************

int16 IsSelectorTxBufFul(void)
{

	_disable();
	if (sctpix == sctgix)
		ced_availbuf = c_ced_tx_buf_siz + 1;
	else if (sctpix > sctgix)
		ced_availbuf = (c_ced_tx_buf_siz - sctpix) + sctgix + 1;
	else 
		ced_availbuf = sctgix - sctpix;

	_enable();
	if (ced_availbuf > 2)
		return 0;
	else
		return 1;
}



//********************************************
// Look for data to change then transmit data
//********************************************


void Ced_Can_Control (void)
{
	//unsigned char msg;

	Ced_Unpack_Can();

	ced_can_status = T_SEL_CAN.ESR.R;
	if (T_SEL_CAN.ESR.B.ERRINT == 1)
	{
		ced_can_status = T_SEL_CAN.ESR.R;
		T_SEL_CAN.ESR.B.ERRINT = 1;
	}
	else
		ced_can_status = 0;

	ced_can_tx_err_cnt = (uint8)T_SEL_CAN.ECR.B.TXECNT;
	ced_can_rx_err_cnt = (uint8)T_SEL_CAN.ECR.B.RXECNT;

	if (T_SEL_CAN.ESR.B.BOFFINT == 1)
	{		// The CAN bus is off
		T_SEL_CAN.ESR.B.BOFFINT = 1;
		ced_bus_off_cnt++;
		ced_can_init_error |= 0x04;
		ced_init_can_bus(2);
		
//ZYM 3-15-2017		
//-		record_fault(f_enc_can_bus_off);
	}
	else if (ced_bus_off_cnt > 0)
		ced_bus_off_cnt--;

#if ((Simulator == 0) || (Sim_Motion == 0))
	if ((ced_can_status & 0x00000080) != 0)
	{		// Bus is idle
		ced_bus_idle_cnt++;
		//		ced_can_init_error |= 0x08;
		//		record_fault(f_CED_can_bus_idle);	
	}
	else if (ced_bus_idle_cnt > 0)
		ced_bus_idle_cnt--;

	if ((ced_can_status & 0x00002000) != 0)
	{		// Transmit acknowledge error
		ced_ack_error_cnt++;
		ced_can_init_error |= 0x10;
		//		record_fault(f_CED_can_ack_err);	
	}
	else if (ced_ack_error_cnt > 0)
		ced_ack_error_cnt--;

#else   
	// simulator mode
	//	CED_can.online = 1;
	//	return;

#endif



	// 	if (timers[tcedcanrx] > 3)
	// 	{
	// 		timers[tcedcanrx] = 0;
	// 		if (CED_can.online_cnt > 0)
	// 			CED_can.online_cnt--;
	// 		if (CED_can.online_cnt > 0)
	// 		{
	// 			ced_can_online = 1;
	// 			CED_can.online = 1;
	// 			ced_aps_can_error = 0;
	// 		}
	// 		else
	// 		{
	// #if (Simulator == 0)
	// 			ced_aps_can_error = 1;
	// 			if (CED_can.online == 1)
	// 				record_fault(f_CED_APS_can_error);
	// #endif
	// 			ced_can_online = 0;
	// 			CED_can.online = 0;
	// 		}
	// 	}

	// Check to see if a tx buffer is available

	/*******************************************/
	/*******************************************/
	/*commennted out by hernan on 11-01-2016*/
	/*******************************************/
	/*******************************************/
	/*	if (IsSelectorTxBufFul() == 0)
	{		// A buffer is available

	if ((CED_can.Error.B.CHANNEL1 != 0) || (CED_can.Error.B.CHANNEL2 != 0) || (CED_can.Error.B.GENERAL != 0))
	{
	//	if (ced_aps_fault == 0)
	//		record_fault(f_CED_APS_fault);
	ced_aps_fault = 1;
	}
	else
	ced_aps_fault = 0;

	msg = Ced_Get_Tx_Msg_Req();
	if (msg != 0)
	{
	Ced_Pack_Can(msg,0);				// Transmit back msg that was requested
	}
	}*/

}



//ZYM 3-10-2017 work as cedes head
// cedes pack can interrupt drive routine 
void Ced_Pack_Can_Interrupt(void)
{
	unsigned char msg = 0;
	static int alt_msg;

	if(alt_msg == 0)
	{
//ZYM 3-21-2017	
		msg = 0x11;
//-		msg=0x1A;		//New Protocal Address
		
		alt_msg = 1;
	}
	else if(alt_msg==1)
	{
		msg = 0x12;
		alt_msg = 2;
	}
	else if(alt_msg==2)
	{
		msg = 0x21;
		alt_msg = 3;
	}
	else if(alt_msg==3)
	{
		msg = 0x22;
		alt_msg = 0;
	}
	else
	{
		alt_msg = 0;		
	}


//ZYM 3-15-2017
//-	if(fvars[fvcedesen] != 0)
//-	{
		Ced_Pack_Can(msg,1);
//-	}


	//clear interrupt
	PIT.TFLG6.R = 0x00000001; 

}



//********************************************************
// Calculate 16 bit CRC for CEDES Data (CRC-CCITT(0xFFFF))
//********************************************************

uint16 Ced_CRC16 (unsigned char *data, int16 cnt)
{
	uint16 crc = 0xffff;
	uint16 i,x;

	for(i = 0; i < cnt; i++) 
	{
		x = (uint16)(((crc>>8) ^ data[i]) & 0xff);
		x ^= x>>4;
		crc = (crc << 8) ^ (x << 12) ^ (x <<5) ^ x;
	}
	return (crc &= 0xffff);
}





//********************************************
//*********  Pack Can data   *****************
//********************************************

void Ced_Pack_Can (unsigned char msg,unsigned char tx_req)
{
	bool valid_msg = 0;
	static unsigned char id_11_cnt;
	
//ZYM 3-15-2017	
//-	int32 TMP_cedes_pls = 0;
	int32 TMP_cedes_pls=1000;

	
	COMMDWORD_TYPE tx_dword;
	COMMWORD_TYPEE tx_word;

	_disable();
	if (sctpix < c_ced_tx_buf_siz)
		sctpix++;
	else
		sctpix = 0;
	_enable();

	if (tx_req == 1)
	{
		switch(msg)
		{
//ZYM 3-21-2017				
		case 0x11:
			CED_can_tx[sctpix].len = 8;
			CED_can_tx[sctpix].ex_id = 0;		// No extended frame
			CED_can_tx[sctpix].rtr = 0;			// remote transmit
			CED_can_tx[sctpix].srr = 0;			// substitute remote request (extended frame only)

			//CED_can_tx[sctpix].addr = 1 << 2;	
			//CED_can_tx[sctpix].msg  = 1;
			
//ZYM 3-21-2017			
			CED_can_tx[sctpix].id = 0x11<<18;		// priority bits get set to zero on shift

//ZYM 4-3-2017			
//-			CED_can_tx[sctpix].data[0] = id_11_cnt++;			// data[0] = 0; //changed for now
			CED_can_tx[sctpix].data[0]=0;

			//position count - 3 bytes where tx_dword.byte[hb] is not used

//ZYM 3-15-2017			
//-			TMP_cedes_pls = DPP_to_cedes_pls(DPP_Count);
			//tx_dword.dword = DPP_Count;			
			
			
			tx_dword.dword=tsm_cedes_pls;
			CED_can_tx[sctpix].data[1] =  tx_dword.byte[mhb];			// 
			CED_can_tx[sctpix].data[2] =  tx_dword.byte[mlb];			// 
			CED_can_tx[sctpix].data[3] =  tx_dword.byte[lb];			// 
		
//ZYM 1-30-2018			
			//"can_rx[rx_ix].data[4]" IS HIGH BYTE, "can_rx[rx_ix].data[5]" IS LOW BYTE
			//WHICH MATCHING GALX1121 NTS		
 			CED_can_tx[sctpix].data[4]=0;
 			CED_can_tx[sctpix].data[5]=galx_1121tester_NTS_Modify_Output.byte;
/* //-			
//ZYM 3-15-2017	
			//velocity value - 2 bytes
//-			tx_word.word = ((Enc_vel * 5.0)/(Pulses_per_Inch) + 0.5);
			tx_word.word=(int16)(tsm_cedes_pls/1000);
			
			CED_can_tx[sctpix].data[4] = tx_word.byte[hbw];				// 
			CED_can_tx[sctpix].data[5] = tx_word.byte[lbw];				// 
*/ 			
 			tx_word.word = Ced_CRC16(CED_can_tx[sctpix].data,6);
 			CED_can_tx[sctpix].data[6] = tx_word.byte[hbw];				// 
 			CED_can_tx[sctpix].data[7] = tx_word.byte[lbw];				// 						
			valid_msg = 1;
			break;

//ZYM 1-30-2018
/* //-			
		case 0x12:
			CED_can_tx[sctpix].len = 8;
			CED_can_tx[sctpix].ex_id = 0;		// No extended frame
			CED_can_tx[sctpix].rtr = 0;			// remote transmit
			CED_can_tx[sctpix].srr = 0;			// substitute remote request (extended frame only)

			//CED_can_tx[sctpix].addr = 0x01 << 3;	
			//CED_can_tx[sctpix].msg  = 0x01 << 3;
			CED_can_tx[sctpix].id = 0x12<<18;		// priority bits get set to zero on shift

//ZYM 4-3-2017
//-			CED_can_tx[sctpix].data[0] = id_11_cnt++;			// data[0] = 0; //changed for now
			CED_can_tx[sctpix].data[0]=0;
			
			CED_can_tx[sctpix].data[1] = 0;			// position error 1 = 0
			CED_can_tx[sctpix].data[2] = 0;			// status 1 error a = 0
			CED_can_tx[sctpix].data[3] = 0;			// status 1 error b = 0
			tx_word.word = Ced_CRC16(CED_can_tx[sctpix].data,4);
			CED_can_tx[sctpix].data[4] = tx_word.byte[hbw];			// data[4] = 0; //changed for now
			CED_can_tx[sctpix].data[5] = tx_word.byte[lbw];			// data[5] = 0; //changed for now
			
 			CED_can_tx[sctpix].data[6] = 0;			// data[6] = 0; //changed for now
			CED_can_tx[sctpix].data[7] = 0;			// data[7] = 0; //changed for now
			valid_msg = 1;			
			break;
*/			


//ZYM 1-30-2017 			
//MUST DO AS "case 0x11:" OTHERWISE, NO WORKING				
		case 0x21:
			CED_can_tx[sctpix].len = 8;
			CED_can_tx[sctpix].ex_id = 0;		// No extended frame
			CED_can_tx[sctpix].rtr = 0;			// remote transmit
			CED_can_tx[sctpix].srr = 0;			// substitute remote request (extended frame only)

			//CED_can_tx[sctpix].addr = 1 << 2;	
			//CED_can_tx[sctpix].msg  = 1;
			
//ZYM 3-21-2017			
			CED_can_tx[sctpix].id = 0x21<<18;		// priority bits get set to zero on shift
			
//ZYM 4-3-2017			
//-			CED_can_tx[sctpix].data[0] = id_11_cnt++;			// data[0] = 0; //changed for now
			CED_can_tx[sctpix].data[0]=0;

			//position count - 3 bytes where tx_dword.byte[hb] is not used

//ZYM 3-15-2017			
//-			TMP_cedes_pls = DPP_to_cedes_pls(DPP_Count);
			//tx_dword.dword = DPP_Count;
			
			tx_dword.dword=tsm_cedes_pls;
			CED_can_tx[sctpix].data[1] =  tx_dword.byte[mhb];			// 
			CED_can_tx[sctpix].data[2] =  tx_dword.byte[mlb];			// 
			CED_can_tx[sctpix].data[3] =  tx_dword.byte[lb];			// 
			
//ZYM 1-30-2018				
			//"can_rx[rx_ix].data[4]" IS HIGH BYTE, "can_rx[rx_ix].data[5]" IS LOW BYTE
			//WHICH MATCHING GALX1121 NTS
			CED_can_tx[sctpix].data[4]=0;
 			CED_can_tx[sctpix].data[5]=galx_1121tester_NTS_Modify_Output.byte;
/* //-	    		
//ZYM 3-15-2017			
			//velocity value - 2 bytes
//-			tx_word.word = ((Enc_vel * 5.0)/(Pulses_per_Inch) + 0.5);
			tx_word.word=(int16)(tsm_cedes_pls/1000);
			
			CED_can_tx[sctpix].data[4] = tx_word.byte[hbw];				// 
			CED_can_tx[sctpix].data[5] = tx_word.byte[lbw];				// 
*/			
			tx_word.word = Ced_CRC16(CED_can_tx[sctpix].data,6);
			CED_can_tx[sctpix].data[6] = tx_word.byte[hbw];				// 
			CED_can_tx[sctpix].data[7] = tx_word.byte[lbw];				// 
			valid_msg = 1;
			break;
			
//ZYM 1-30-2018			
/* //-			
		case 0x22:
			CED_can_tx[sctpix].len = 8;
			CED_can_tx[sctpix].ex_id = 0;		// No extended frame
			CED_can_tx[sctpix].rtr = 0;			// remote transmit
			CED_can_tx[sctpix].srr = 0;			// substitute remote request (extended frame only)

			//CED_can_tx[sctpix].addr = 0x01 << 3;	
			//CED_can_tx[sctpix].msg  = 0x01 << 3;
			CED_can_tx[sctpix].id = 0x22<<18;		// priority bits get set to zero on shift

//ZYM 4-3-2017
//-			CED_can_tx[sctpix].data[0] = id_11_cnt++;			// data[0] = 0; //changed for now
			CED_can_tx[sctpix].data[0]=0;
			
			CED_can_tx[sctpix].data[1] = 0;			// position error 1 = 0
			CED_can_tx[sctpix].data[2] = 0;			// status 1 error a = 0
			CED_can_tx[sctpix].data[3] = 0;			// status 1 error b = 0
			tx_word.word = Ced_CRC16(CED_can_tx[sctpix].data,4);
			CED_can_tx[sctpix].data[4] = tx_word.byte[hbw];			// data[4] = 0; //changed for now
			CED_can_tx[sctpix].data[5] = tx_word.byte[lbw];			// data[5] = 0; //changed for now
			CED_can_tx[sctpix].data[6] = 0;			// data[6] = 0; //changed for now
			CED_can_tx[sctpix].data[7] = 0;			// data[7] = 0; //changed for now
			valid_msg = 1;			
			break;
*/ 			
			
						
		default:
			break;
		}
	}
	else switch (msg)
	{
	case 0:
		ced_can_init_error = e_inv_pack;
		ced_can_error_data = msg;
		break;


	default:
		ced_can_init_error = e_inv_pack;
		ced_can_error_data = msg;
		break;
	}

	if (valid_msg == 1)
	{

		// if (CED_can_tx[sctpix].len > 8)
		// {
		// 	CED_can_tx[sctpix].len = 8;
		// 	ced_can_init_error |= 0x40;
		// }
		// uiu.b[hb] = (uint8)((CED_can_tx[sctpix].msg >> 3) & 0x0F);		// Message
		// uiu.b[mhb] = (uint8)((CED_can_tx[sctpix].addr & 0x7F) << 1);	// Address
		// uiu.b[mlb] = 0;
		// uiu.b[lb] = 0;
		// 	CED_can_tx[sctpix].id = uiu.dw << 1;		// priority bits get set to zero on shift




		Tx_Ced_Can_Msg(8);
	}

}





//************************************************************
//*********  Transmit Can Message  ***************************
//************************************************************

void Tx_Ced_Can_Msg(int16 TxBufIx)
{
	if (sctgix != sctpix)
	{

		switch (TxBufIx)
		{

		case 8:
			if ((T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
				(T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
				(T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
			{		// The transmitter is not busy
				ced_transmit(TxBufIx);
				T_SEL_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
				break;
			}
			else
			{
				ced_tx_B08_fail_cnt++;
				if (ced_tx_B08_fail_cnt > 5)
				{
					ced_tx_B08_fail_cnt = 0;
					T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
				}
				TxBufIx++;		
			}

		case 9:
			if ((T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
				(T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
				(T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
			{		// The transmitter is not busy
				ced_transmit(TxBufIx);
				T_SEL_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
				break;
			}
			else
			{
				ced_tx_B09_fail_cnt++;
				if (ced_tx_B09_fail_cnt > 5)
				{
					ced_tx_B09_fail_cnt = 0;
					T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
				}
				TxBufIx++;		
			}
		case 10:
			if ((T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
				(T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
				(T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
			{		// The transmitter is not busy
				ced_transmit(TxBufIx);
				T_SEL_CAN.IFLAG1.B.BUF10I = 1;
				break;
			}
			else
			{
				ced_tx_B10_fail_cnt++;
				if (ced_tx_B10_fail_cnt > 5)
				{
					ced_tx_B10_fail_cnt = 0;
					T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
				}
				TxBufIx++;		
			}
		case 11:
			if ((T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
				(T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
				(T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
			{		// The transmitter is not busy
				ced_transmit(TxBufIx);
				T_SEL_CAN.IFLAG1.B.BUF11I = 1;
				break;
			}
			else
			{
				ced_tx_B11_fail_cnt++;
				if (ced_tx_B11_fail_cnt > 5)
				{
					ced_tx_B11_fail_cnt = 0;
					T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
				}
			}
		default:
			break;
		}
	}
}





//************************************************************
//*********  Transmit Buffer  ********************************
//************************************************************

void ced_transmit (int16 TxBufIx)
{
	int16 i;
	if (sctgix != sctpix)
	{		// There is data in the buffer 

		if (sctgix < c_ced_tx_buf_siz)
			sctgix++;
		else
			sctgix = 0;


		T_SEL_CAN.MB[TxBufIx].MSG.MID.R = CED_can_tx[sctgix].id;	

		i = 0;
		while ((i<CED_can_tx[sctgix].len) && (i < 8))
		{
			T_SEL_CAN.MB[TxBufIx].MSG.MDATA.B[i++] = CED_can_tx[sctgix].data[i];
		}


		T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.LENGTH = CED_can_tx[sctgix].len;

		T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.RTR = CED_can_tx[sctgix].rtr;

		T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.IDE = CED_can_tx[sctgix].ex_id;

		T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.SRR = CED_can_tx[sctgix].srr;


		T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x0C;		// Initiate transmit
		CED_can_debug_1 = T_SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE;
		CED_can.tx_cnt++;
	}
}

//*****************************************
// Unpack the can message
//*****************************************

void Ced_Unpack_Can (void)
{
//ZYM 3-21-2017
//-	COMMWORD_TYPEE rcv_word;
//-	COMMDWORD_TYPE rcv_dword;
	int16 loop_cnt = 0;
	uint8 msg,addr;
	int16 tvelocity = 0;


	while ((loop_cnt <= 4)  && (scrgix != scrpix))
	{			
		if (CED_can_rx[scrgix].valid_msg == 1)
		{	

			if (CED_can_rx[scrpix].ex_id == 1)		// Extended frame
			{
				uiu.dw = ((CED_can_rx[scrgix].id & 0x1FFC0000) << 2) ;
				uiu_2.dw = ((CED_can_rx[scrgix].id & 0x1FFC0000) >> 2);
				CED_can_rx[scrgix].addr = uiu.b[hb] & 0x7F;	// Message
				CED_can_rx[scrgix].msg = uiu_2.b[mhb] & 0x0F;    // Address
			}
			//standard ID
/* //-			
			else
			{
			
				uiu.dw = ((CED_can_rx[scrgix].id & 0x1FFC0000) << 2) ;
				uiu_2.dw = ((CED_can_rx[scrgix].id & 0x1FFC0000) >> 2);
				CED_can_rx[scrgix].addr = uiu.b[hb] & 0x7F;	// Message
				CED_can_rx[scrgix].msg = uiu_2.b[mhb] & 0x0F;    // Address								
			}

			msg = CED_can_rx[scrgix].msg;
			addr = CED_can_rx[scrgix].addr;
*/


// ZYM 12-13-2017
//for standard ID
//check the decimal value of the "CED_can_rx[scrgix].id" which got from the CPU's register
//then change the message number from 1 to 12 of Slave(in this case, GALX1121 NTS MCU is the Slave)	
//after the comparsion, found out "CED_can_rx[scrgix].id" splits to 4 bytes,  
//message number = bit27,26,25,24,23,22,21	  
//address number = bit20,19,18,17,16,15,14,13	

//-				uiu.dw=(CED_can_rx[scrgix].id & 0xFE00000);	// Message
//-				uiu_2.dw=(CED_can_rx[scrgix].id & 0x1FE000);// Address
//-				CED_can_rx[scrgix].msg=uiu.dw>>21;			// Message
//-				CED_can_rx[scrgix].addr=uiu_2.dw>>13;    	// Address
				

//-test_msg=CED_can_rx[scrgix].id;
//-test_msg=(CED_can_rx[scrgix].id & 0xFE00000)>>21;
//-test_msg=(CED_can_rx[scrgix].id & 0x1FE000)>>13;
//-test_msg=CED_can.position_count;


			msg=(CED_can_rx[scrgix].id & 0xFE00000)>>21;
			addr=(CED_can_rx[scrgix].id & 0x1FE000)>>13;   
			
			
			if (CED_can_rx[scrgix].rtr == 1)
			{	
				Ced_Put_Tx_Msg_Req(CED_can_rx[scrgix].msg);	
				CED_can.rx_cnt++;
				CED_can.online_cnt = 5;
			}
			else switch(msg)
			{
				//GALX1121 NTS return packet 12, address 3
				case 12:
					if(addr==3)
					{
//-						test_msg++;
//-						if(test_msg>=0xFFFFFFFF)
//-						{
//-							test_msg=0;	
//-						}
//-						SIU.GPDO[Z6_LED].R!=SIU.GPDO[Z6_LED].R;


						NTS_ver_rev.ver=CED_can_rx[scrgix].data[0];
						NTS_ver_rev.rev=CED_can_rx[scrgix].data[1];
						rcv_dword.byte[hb]=0;
						rcv_dword.byte[mhb]=CED_can_rx[scrgix].data[3];
						rcv_dword.byte[mlb]=CED_can_rx[scrgix].data[4];
						rcv_dword.byte[lb]=CED_can_rx[scrgix].data[5];
						CED_can.position_count=rcv_dword.dw;
						
						Copy_galx_1121tester_NTS_Modify_Output_byte=CED_can_rx[scrgix].data[6];
						galx_1121tester_NTS_Modify_Input.buff[0]=CED_can_rx[scrgix].data[7];
					}				
				break;
			}
			
//ZYM 3-21-2017			
/* // MUST NOT leave these code here to read back the data which Tester sends to Selector as APS,
	  MUST use the other CAN channel From Tester to do it such as doing inside "Read_cedes_can.c"			
			else switch (CED_can_rx[scrgix].msg)
			{			
			case 0x01:			// Message 0x01: Selector Position and Speed Data
				if (CED_can_rx[scrgix].addr == 0x01)
				{
					can_ced_seq_cnt = CED_can_rx[scrgix].data[0];
					ced_data_in_cnt++;

					rcv_word.byte[hbw] = CED_can_rx[scrgix].data[6];
					rcv_word.byte[lbw] = CED_can_rx[scrgix].data[7];
					can_ced_crc16 = rcv_word.word;
					can_ced_calc_crc16 = CRCSelector16 (CED_can_rx[scrgix].data, 6);

					if (can_ced_calc_crc16 == can_ced_crc16)
					{
						ced_data_in_capture_cnt++;
						rcv_dword.byte[hb] = 0;
						rcv_dword.byte[mhb] = CED_can_rx[scrgix].data[1];
						rcv_dword.byte[mlb] = CED_can_rx[scrgix].data[2];
						rcv_dword.byte[lb] = CED_can_rx[scrgix].data[3];
						CED_can.position_count = rcv_dword.dword;

						rcv_word.byte[hbw] = CED_can_rx[scrgix].data[4];
						rcv_word.byte[lbw] = CED_can_rx[scrgix].data[5];
						tvelocity = rcv_word.word;
						if (tvelocity == 0x7FFF)
							tvelocity = 0;
						
//ZYM 3-15-2017						
//-						CED_can.velocity = (int16)abs(tvelocity);

						
						if (tvelocity > 0)
							CED_can.vel_dir = 1;
						else if (tvelocity < 0)
							CED_can.vel_dir = 2;
						else
							CED_can.vel_dir = 0;


						CED_can.rx_cnt++;
						CED_can.online_cnt = 5;
					}
				}
				break;		
			case 0x02:			// Message 0x02: Selector Status and Errors
				if (CED_can_rx[scrgix].addr == 0x01)
				{
					can_ced_seq_cnt_2 = CED_can_rx[scrgix].data[0];
					ced_data_in_2_cnt++;
					if (can_ced_seq_cnt_2 == can_ced_seq_cnt)
					{
						rcv_word.byte[hbw] = CED_can_rx[scrgix].data[4];
						rcv_word.byte[lbw] = CED_can_rx[scrgix].data[5];
						can_ced_crc16 = rcv_word.word;
						can_ced_calc_crc16 = CRCSelector16 (CED_can_rx[scrgix].data, 4);

						if (can_ced_calc_crc16 == can_ced_crc16)
						{

							ced_data_in_2_capture_cnt++;
							CED_can.Error.R = CED_can_rx[scrgix].data[1];


							rcv_word.byte[hbw] = CED_can_rx[scrgix].data[2];
							rcv_word.byte[lbw] = CED_can_rx[scrgix].data[3];
							CED_can.Status.R = rcv_word.word;

							CED_can.rx_cnt++;
							CED_can.online_cnt = 5;
						}
					}
				}
				break;					

									
			default:
				if (CED_can_rx[scrgix].msg != 0) 
				{
					ced_can_init_error = e_inv_unpk;
					ced_can_error_data = CED_can_rx[scrgix].msg;
				}
				break;
			}
*/			
			CED_can_rx[scrgix].valid_msg = 0;
		}
		scrgix++;
		if (scrgix >= c_ced_rx_buf_siz)
			scrgix = 0; 
		loop_cnt++;
	}
}

// ******************************************************
// **********  Transmit Msg Request Put Queue ***********
// ******************************************************

unsigned char Ced_Put_Tx_Msg_Req (unsigned char msg)
{
	unsigned char n;
	for(n=0;n<=4;n++)
	{
		if (CED_can_tx_msg_req[n] == 0)
		{
			CED_can_tx_msg_req[n] = msg;
			return 1;
		}
	}
	return 0;
}
// ******************************************************
// **********  Transmit Msg Request Get Queue ***********
// ******************************************************

unsigned char Ced_Get_Tx_Msg_Req (void)
{
	unsigned char n;
	unsigned char msg;

	msg = 0;
	for(n=0;n<=4;n++)
	{
		if (CED_can_tx_msg_req[n] != 0)
		{
			msg = CED_can_tx_msg_req[n];
			CED_can_tx_msg_req[n] = 0;
			return msg;
		}
	}
	return 0;
}


//**************************************************
// Calculate 16 bit CRC for Data (CRC-CCITT(0xFFFF))
//**************************************************

uint16 CRCSelector16 (unsigned char *data, int16 cnt)
{
	uint16 crc = 0xffff;
	uint16 i,x;

	for(i = 0; i < cnt; i++) 
	{
		x = (uint16)(((crc>>8) ^ data[i]) & 0xff);
		x ^= x>>4;
		crc = (crc << 8) ^ (x << 12) ^ (x <<5) ^ x;
	}
	return (crc &= 0xffff);
}






























