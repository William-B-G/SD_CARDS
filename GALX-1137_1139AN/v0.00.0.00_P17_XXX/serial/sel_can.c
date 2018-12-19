#define d_SEL_CAN 1
#include "global.h"
//****************************************************************
//****************************************************************
//******* Routines to run the Cedes/Elgo CAN Bus
//****************************************************************
//****************************************************************

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
						 2: Middle Byte Position Count
						 3: Low Byte Position Count
					Byte 4: High Byte Car speed
					     5: Low Byte Car speed
					Byte 6: High Byte of CRC 16
						 7: Low Byte of CRC 16
						 
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
							Note: This value is a combination of the hardware and software version. Changes will be disclosed 
							with a release note.
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
						 
						 
	Packet #0x1A 	Byte 0: SeqCnt Sequence counter, counts up in each message, must be checked for SIL applications
					Byte 1: Position A High byte  Actual position in [0.5mm] steps
					Byte 2: Position A Middle byte
					Byte 3: Position A Low byte
					Byte 4: Velocity A High byte  Actual velocity in [mm/s]
					Byte 5: Velocity A Low byte
					Byte 6: Error/Warn 
							Error
								Bit 0 Internal Error** Fatal
								Bit 1 Communication Error Fatal
								Bit 2 Position Validation Error according to SIL3** Fatal 
									  (more than one pos. extrapolated or crosscheck not successful)
								Bit 3 Velocity Validation Error according to SIL3** Fatal 
									  (crosscheck not successful)
							Warning
								Bit 4 Position A Warning, Position extrapolated
								Bit 5 Position B Warning, Position extrapolated
								Bit 6-7 Contrast:
									00 = OK
									01 = Service recommended (tape cleaning)
									10 = Warning (go to target floor and stop operation)
									11 = Stopped						 
					Byte 7: Status
							Clip
								Bit 0 Position Indicator Clip Camera A
									  Bit is set if a clip is detected on camera A - A offset if both set -- 
									  check clip offset for position information
								Bit 1 Position Indicator Clip Camera B
									  Bit is set if a clip is detected on camera B - 
									  check clip offset for position information
							Alignment
								Alignment left/right
									Bit 2 0 = left and 1 = right
									Bit 3-4 0 = centered, 1 = slightly off, 2 = moderately off, 3 = at the limit
								Alignment far/near
									Bit 5 0 = near and 1 = far
									Bit 6-7 0 = centered, 1 = slightly off, 2 = moderately off, 3 = at the limit

	Packet #0x1B 	Byte 0: Counterpart Position (Pos B) High byte  Actual position counterpart channel in [0.5mm] steps
					Byte 1: Counterpart Position (Pos B) Middle Byte
					Byte 2: Counterpart Position (Pos B) Low byte
					Byte 3: Clip offset  Offset of the clip position in [mm] to the position, indicated by the set
										 clip bit. If both clips are set, offset is related to position A.
					Byte 4: CRC32 High byte  CRC 32 (0x04C11DB7), over byte 0-7 from msg 0x1A/1B and byte 0-3 from msg 0x2A/2B
											 (over both msg) 
					Byte 5: CRC32 Middle High byte
					Byte 6: CRC32 Middle Low byte
					Byte 7: CRC32 Low byte
						 
	Packet #0x84	Elgo Selector Packet device 1 (address 1)
					Byte 0: High Byte Position Count
					Byte 1: Middle High Byte position count
					Byte 2: Middle Low Byte positon Count
					Byte 3: Low Byte Position Count
					Byte 4: High byte velocity
					Byte 5: Low Byte Velocity
					Byte 6: Checksum
					Byye 7: Error byte 1h = Tape error device 1, 0x10 = Tape error Device 2					 
												
	Packet #0x85	Elgo Selector Packet device 2 (address 1)
					Byte 0: High Byte Position Count
					Byte 1: Middle High Byte position count
					Byte 2: Middle Low Byte positon Count
					Byte 3: Low Byte Position Count
					Byte 4: High byte velocity
					Byte 5: Low Byte Velocity
					Byte 6: Checksum
					Byye 7: Error byte 1h = Tape error Device 2,  0x10 = Tape error Device 1					 
												
*/


// Can Routine declarations
int16 SelChipFreezeMode(unsigned char set);
void sel_init_can_bus (int16 baud_change);
void sel_wrong_can_int (void);
void sel_can_bus_off_and_warning_int (void);
void sel_can_error_int (void);
void sel_can_rcv_int (void);
void sel_can_rcv_fifo_warning_int (void);
void sel_can_rcv_fifo_overflow_int (void);
void sel_can_trm_int_b8 (void);
void sel_can_trm_int_b9 (void);
void sel_can_trm_int_b10 (void);
void sel_can_trm_int_b11 (void);
void Sel_Can_Control (void);
void sel_transmit (int16 TxBufIx);
void sel_receive_can_in (int16 BufIx);
void Tx_Sel_Can_Msg(int16 TxBufIx);

void Sel_Pack_Can (unsigned char msg, unsigned char tx_req);
void Sel_Unpack_Can (void);
unsigned char Sel_Put_Tx_Msg_Req (unsigned char msg);
unsigned char Sel_Get_Tx_Msg_Req (void);
int16 IsSelTxBufFul(void);
uint8 check_sum (unsigned char *data, int16 cnt);
uint16 CRC16 (unsigned char *data, int16 cnt);
uint32 crc32(uint8 *buff, int16 len); 

int16 aps_sel_can_error = 0;		// ced aps can error
int16 aps_sel_can_online = 0;		// ced aps can online
int16 aps_sel_fault = 0;			// ced aps fault
int32 sel_clip_cnt = 0;
int16 sel_clip = 0;

int16 tvelocity = 0;

uint16 can_sel_crc16 = 0;
uint16 can_sel_calc_crc16 = 0;
uint32 can_sel_crc32 = 0;
uint32 can_sel_calc_crc32 = 0;

int16 can_sel_dir = 0;
int16 can_sel_seq_cnt = 0;
int16 can_sel_seq_cnt_2 = 0;



struct tag_sel_can_v1 {
    union {
    	uint16 R;
        struct {
            uint16:1;				// bit 15 Reserved: Transmitted as '0'
            uint16 ALIGN:3;			// bit 14-12 Alignment: 
									//		000 = OK, no general Error
									//		001 = Cannot Read Tape
									//		010 = Alignment needed - too close
									//		011 = Alignment needed - too far
									//		100 = Alignment needed - too far left
									//		101 = Alignment needed - too far right
									//		110 = n/a
									//		111 = n/a

            uint16 REV:4;			// bit 11-8 Revision: 4bit revision code
            uint16:3;				// bit 7-5 Reserved: Transmitted as '0'
            uint16 IN1:1;			// bit 4 IN2: Compression Compensation Input 2
            uint16 IN2:1;			// bit 3 IN1: Compression Compensation Input 1
            uint16 CONTRAST:2;		// bit 2-1 Contrast (b:w) :
									//		00 = Code Tape Contrast OK (> 1:10, typ 1:20)
									//		01 = Service Recommended (> 1:7)
									//		10 = Warning (> 1:3)
									//		11 = Stopped (< 1:3)
            uint16 CLIP:1;			// bit 0 Clip: Position Indicator Clip
	    } B;
    } Status;

    union {
        uint8 R;
        struct {
            uint8 CHANNEL1:2;	// bit 7-6 Pos1 Err: Result of the internal position and velocity cross-check (channel 1 to channel 2)
								// 		00 = Position1 and Velocity1 OK (position/velocity is safe)
								// 		01 = Position1 Error (position comparison failed)
								// 		10 = Velocity1 Error (velocity comparison failed)
								// 		11 = Position1 and Velocity1 Error(position and velocity comparison failed)
            
            uint8 CHANNEL2:2;	// bit 5-4 Pos2 Err: Result of the internal position and velocity cross-check (channel 2 to channel 1)
								// 		00 = Position2 and Velocity2 OK (position and velocity is safe)
								// 		01 = Position2 Error (position comparison failed)
								// 		10 = Velocity2 Error (velocity comparison failed)
								// 		11 = Position2 and Velocity2 Error (position and velocity comparison failed)
         	uint8:1;			// bit 3 Reserved: Transmitted as '0'
            uint8 GENERAL:3;	// bit 2-0 General Error:
								//		000 = OK, no general error
								//		001 = Cannot Read Tape
								//		010 = Alignment error - too close
								//		011 = Alignment error - too far
								//		100 = Alignment error - too far left
								//		101 = Alignment error - too far right
								//		110 = Internal Fault
								//		111 = Communication Error       
		} B;
    } Error;
} sel_can_v1;

struct SEL_can_type {
	int32	pos_cnt_A;
	int32	pos_cnt_B;
	int16	vel;
	uint8 	vel_dir;			// velocity direction
 	int8 	clip_offset;
    union {
    	uint8 R;
        struct {
            uint8 NF_ALIGN:2;		// 0=Centered, 1=slightly off, 1=modertly off, 3=at the limit
            uint8 FAR:1;			// 0 = near, 1 = far
            uint8 LR_ALIGN:2;		// 0=Centered, 1=slightly off, 1=modertly off, 3=at the limit
            uint8 RIGHT:1;			// 0 = left, 1 = right
            uint8 CLIP_B:1;			// ClipB is detected (offset is for B)
            uint8 CLIP_A:1;			// CiipA is detected (offset is for A) (if both A and B then offset is for A)
       } B;
    } Status;

    union {
        uint8 R;
        struct {
            uint8 CONTRAST:2;		// 00=OK, 01=Service Recommended, 10=Warning (go to target floor and shutdown), 11=stopped
            uint8 POS_B_EX:1;		// Position B extrapolated
            uint8 POS_A_EX:1;		// Position A extrapolated
            uint8 VEL_INVALID:1;	// Velocity Validation Error
            uint8 POS_INVALID:1;	// Position Validation Error
            uint8 COMM:1;			// Communications Error
            uint8 INTERNAL:1;		// Internal Error
        } B;
    } Error_Warning;
    uint8	ver;					// protocol version 1 = initial, 2= latested with two pos counts
	bool 	online;
	uint8	online_cnt;
	int16	tx_cnt;
	int16	rx_cnt;
};

struct SEL_can_type sel_can;

uint8 sel_can_data[16];

int16 sel_can_init_count = 0;
int16 sel_init_mode_cnt = 0;
bool sel_in_chip_reset = false;

int32 sel_can_status = 0;

int16 sel_can_init_error=0;
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

int16 sel_can_error_data=0;
#define e_inv_pack 4
#define e_inv_unpk 12
#define c_Freeze_Start		0x01	// put chip in freeze mode
#define c_Freeze_Stop		0x00	// Takes chip out of freeze mode

int16 sel_can_int_count = 0;
int16 sel_can_tx_err_cnt = 0;
int16 sel_can_rx_err_cnt = 0;
int16 sel_rcv_fifo_cnt = 0;
int16 sel_rcv_int_fail_cnt = 0;
int16 sel_WCI_cnt=0;	// Wrong can interrupt
int16 sel_BOI_cnt=0;	// Bus Off and tx/rx warning interrupt count
int16 sel_ERI_cnt=0;	// Error Interrupt count
int16 sel_RI_cnt=0;		// Receive fifo interrupt count
int16 sel_FWI_cnt=0;	// Fifo warning interrupt count
int16 sel_FOI_cnt=0;	// Fifo Overflow interrupt count
int16 sel_TI8_cnt=0;	// Transmit buffer 8 interrupt count
int16 sel_TI9_cnt=0;	// Transmit buffer 9 interrupt count
int16 sel_TI10_cnt=0;	// Transmit buffer 10 interrupt count
int16 sel_TI11_cnt=0;	// Transmit buffer 11 interrupt count
int16 sel_ack_error_cnt = 0;
int16 sel_bus_off_cnt = 0;
int16 sel_bus_idle_cnt = 0;

int16 sel_tx_B08_fail_cnt = 0;
int16 sel_tx_B09_fail_cnt = 0;
int16 sel_tx_B10_fail_cnt = 0;
int16 sel_tx_B11_fail_cnt = 0;

int32 sel_can_debug_1=0;
int32 sel_can_debug_2=0;
int32 sel_can_debug_3=0;
int32 sel_can_debug_4=0;
int32 sel_can_debug_5=0;
int32 sel_can_debug_6=0;


unsigned char sel_can_tx_msg_req[6];

int16 tx_sel = 0;

// Receive Buffer

#define c_sel_rx_buf_siz 10

struct tag_sel_can_rx {
		unsigned char valid_msg;	// 1 when valid message in buffer
		unsigned char len;
		unsigned char ex_id;	// flag for extended id mode
		unsigned char rtr;		// Remote Transmission Request
		unsigned char msg;		// Message
		unsigned char addr;		// Address
		uint32 id;
		unsigned char data [8];
		uint16 time;
} sel_can_rx[c_sel_rx_buf_siz+1];


int16 scrpix = 0;					// selector can receive buffer put index
int16 scrgix = 0;					// selector can receive buffer get index
int16 sctpix = 0;					// selector can transmit buffer put index
int16 sctgix = 0;					// selector can transmit buffer get index
int16 sel_availbuf = 0;				// transmit buffer space available
	
	
// Transmit buffer 
#define c_sel_tx_buf_siz 10

struct tag_sel_can_tx {
		unsigned char len;
		unsigned char ex_id;	// flag for extended id mode
		unsigned char rtr;		// Remote Transmission Request
		unsigned char srr;		// Substitute Remote Request
		unsigned char msg;		// Message
		unsigned char addr;		// Address
		uint32 id;
		unsigned char data [8];
} sel_can_tx[c_sel_tx_buf_siz+1];

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

}COMMWORD_TYPE;

typedef union
{
	uint32 dword;
	uint16 word[2];
	uint8 byte[4];
}COMMDWORD_TYPE; 

struct {
	uint16 pkt;
	uint8 stat;
} pkt_ret;
	
// CAN_A Int Base = 127
// CAN_B Int Base = 157 
// CAN_C Int Base = 178
// CAN_D Int Base = 199


//#if (Simulator == 0)
#define SEL_CAN CAN_B
#define SEL_CAN_INT_BASE 157
//#define SEL_CAN CAN_A
//#define SEL_CAN_INT_BASE 127

//#define SEL_CAN CAN_C
//#define SEL_CAN_INT_BASE 178

//*******************************************************************
// Sends the freeze bit to the chip
// Value of 1 puts the chip in freeze mode
// Value of 0 takes is out of freeze mode
// Have to see the freeze mode acknowledge bit gets set
//*******************************************************************
int16 SelChipFreezeMode(unsigned char set)
{

	
	if (sel_in_chip_reset == true)
		return 0;

	sel_in_chip_reset = true;

	sel_init_mode_cnt = 0;

	SEL_CAN.MCR.B.MDIS = 0;			// Enable module after reset
	SEL_CAN.MCR.B.FRZ = 1;			// Set or clear the freeze bit
	SEL_CAN.MCR.B.HALT = set;			// Clear halt mode

	
	while (sel_init_mode_cnt < 200)
	{
		if (SEL_CAN.MCR.B.FRZACK == set)
		{
			sel_in_chip_reset = false;
			return 1;
		}
		sel_init_mode_cnt++;
	}
	sel_in_chip_reset = false;
	return 0;		// Failed to initialize
}

// **************************************************
// ****  Initialize the ECAN 1000 CAN interface Board
// **************************************************


void sel_init_can_bus (int16 baud_change)
{	 
					// baud_change = 0: use parameter
					// baud_change = 1: set 125K
					// baud_change = 2: set 250k
	int16 reset_count;

	// See if the board is there
	sel_can_init_count++;

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
	INTC_InstallINTCInterruptHandler(sel_can_bus_off_and_warning_int, SEL_CAN_INT_BASE, 1);  
	INTC_InstallINTCInterruptHandler(sel_can_error_int, SEL_CAN_INT_BASE+1, 1);  
	INTC_InstallINTCInterruptHandler(sel_wrong_can_int, SEL_CAN_INT_BASE+3, 1);  
	INTC_InstallINTCInterruptHandler(sel_wrong_can_int, SEL_CAN_INT_BASE+4, 1);  
	INTC_InstallINTCInterruptHandler(sel_wrong_can_int, SEL_CAN_INT_BASE+5, 1);  
	INTC_InstallINTCInterruptHandler(sel_wrong_can_int, SEL_CAN_INT_BASE+6, 1);  
	INTC_InstallINTCInterruptHandler(sel_wrong_can_int, SEL_CAN_INT_BASE+7, 1);  
	INTC_InstallINTCInterruptHandler(sel_can_rcv_int, SEL_CAN_INT_BASE+8, 1);  
	INTC_InstallINTCInterruptHandler(sel_can_rcv_fifo_warning_int, SEL_CAN_INT_BASE+9, 1);  
	INTC_InstallINTCInterruptHandler(sel_can_rcv_fifo_overflow_int, SEL_CAN_INT_BASE+10, 1);  
	INTC_InstallINTCInterruptHandler(sel_can_trm_int_b8, SEL_CAN_INT_BASE+11, 1);  
	INTC_InstallINTCInterruptHandler(sel_can_trm_int_b9, SEL_CAN_INT_BASE+12, 1);  
	INTC_InstallINTCInterruptHandler(sel_can_trm_int_b10, SEL_CAN_INT_BASE+13, 1);  
	INTC_InstallINTCInterruptHandler(sel_can_trm_int_b11, SEL_CAN_INT_BASE+14, 1);  
	
	// Send init to can board
	if (SelChipFreezeMode(c_Freeze_Start) == 1)
	{

		sel_can_init_count++;

		SEL_CAN.MCR.B.SOFTRST = 1;		// Soft Reset
		reset_count = 0;
		while ((SEL_CAN.MCR.B.SOFTRST == 1) && (reset_count < 1000))
			reset_count++;
			
		SEL_CAN.MCR.B.MDIS = 0;			// Enable module after reset

	    // Clear of all interrupts
	    SEL_CAN.IFLAG1.R = 0xFFFFFFFF;
	    SEL_CAN.IFLAG2.R = 0xFFFFFFFF;
	    
	    // Disable of Interrupts
		SEL_CAN.IMASK1.R = 0;
		SEL_CAN.IMASK2.R = 0;
		
		SEL_CAN.MCR.B.BCC = 1;			// Allow backward compatibility since all mask will be set to 0xffff
		SEL_CAN.MCR.B.MAXMB = 0x0B;		// Set to 12 (0xC - 1)
		SEL_CAN.MCR.B.WRNEN = 0;			// Warning interrupts
		SEL_CAN.MCR.B.SRXDIS = 1;			// Self Reception disabled
		SEL_CAN.MCR.B.FEN = 1;			// Enable RX fifo
		SEL_CAN.MCR.B.ABTEN = 1;			// Abort enable
		SEL_CAN.MCR.B.LPRIOEN = 0;		// No local priority mode
		SEL_CAN.MCR.B.IDAM = 0;			// ID Acceptance mode (one full id per filter element)
		
		
	    // Configure clock AND baud rate timer for 250,000 baud
	    
	    /*	
	    	fTq = Bit Rate * Number of time Quanta = fCANCLK / Prescale Value
	    	Prescale Value = fCANCLK/(Bit Rate * Number of Time Quanta)
	    	Prescale Value = 40 Mhz / (250000 * 16) = 10.000
	    */
	    
	    
	    SEL_CAN.CTRL.B.CLKSRC = 0; 		// System Clock 40.0 Mhz
	    if (baud_change == 0)
	    {
	    	
		    if (fvars[fvencbaud] == 0)
		    	SEL_CAN.CTRL.B.PRESDIV = 9;		// Prescale Devision Factor + 1 = Prescale Value for 250000
			else 
		    	SEL_CAN.CTRL.B.PRESDIV = 19;		// Prescale Devision Factor + 1 = Prescale Value for 125000
	    }
	    else if (baud_change == 1)
	    	SEL_CAN.CTRL.B.PRESDIV = 19;		// Prescale Devision Factor + 1 = Prescale Value for 125000
	    else
	    	SEL_CAN.CTRL.B.PRESDIV = 9;		// Prescale Devision Factor + 1 = Prescale Value for 250000
		
		SEL_CAN.CTRL.B.RJW = 2;			// Resync Jump Width = RJW + 1 = 3
		// Total Tq = 16
		SEL_CAN.CTRL.B.PSEG1 = 7;			// Phase Segment 1 = (PSEG1 + 1) x Tq = 8 (also sync seg = 1)
		SEL_CAN.CTRL.B.PSEG2 = 5;			// Phase Segment 2 = (PSEG2 + 1) x Tq = 6
		SEL_CAN.CTRL.B.PROPSEG = 0;		// Propagation Segment time = (PROPSEG + 1) x Tq = 1
		SEL_CAN.CTRL.B.SMP = 1;			// Three samples
		
		SEL_CAN.CTRL.B.BOFFREC = 0;		// Automamtic recovery
		SEL_CAN.CTRL.B.TSYN = 0;			// Timer sync disabled
		SEL_CAN.CTRL.B.LBUF = 1;			// Lowest buffer transmitted first
		SEL_CAN.CTRL.B.LOM = 0;			// Listen only mode disabled
		
		SEL_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
		SEL_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
		SEL_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
		SEL_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled


		// Set Acceptance Mask
		
		SEL_CAN.RXIMR[0].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[1].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[2].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[3].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[4].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[5].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[6].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[7].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[8].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[9].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[10].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[11].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[12].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[13].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[14].R = 0x00000000;		// Enable all packets
		SEL_CAN.RXIMR[15].R = 0x00000000;		// Enable all packets

		// Set Receive ID
		SEL_CAN.MB[6].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		SEL_CAN.MB[6].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		SEL_CAN.MB[6].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		SEL_CAN.MB[6].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID
		SEL_CAN.MB[7].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		SEL_CAN.MB[7].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		SEL_CAN.MB[7].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		SEL_CAN.MB[7].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID

		SEL_CAN.MB[8].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		SEL_CAN.MB[9].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		SEL_CAN.MB[10].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		SEL_CAN.MB[11].MSG.MCS.B.CODE = 0x08;		// write inactive mode

		if (SelChipFreezeMode(c_Freeze_Stop) == 1)
		{		// initialize can board
			sel_can_init_count++;
			
		    // Enable interrupts 
			SEL_CAN.IMASK1.R = 0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
			SEL_CAN.IMASK2.R = 0x00;		// MB 32-63 NOT Enabled
			SEL_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
			SEL_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
			SEL_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
			SEL_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled
		}
		else
		{
			// Set error code
			sel_can_init_error = 0x01;
		}		
	}
	else
	{
		// Set error code
		sel_can_init_error = 0x02;
	} 
	_enable();
}
	


// *************************************************
// *** Initialize Baud Control   Registers *********
// *************************************************
/*void BaudCntReg_Init(void)
{	

	C1CTRLbits.CANCKS = 0;    /// 0 --> FCAN = 4*FCYL = 25MHZ   
	C1CFG1bits.BRP = 4; 
	C1CFG1bits.SJW = 1;      
	
							/// External clock is 25MHZ
	      					/// TQ =  [1/BaudRate]/10;
								// --> BaudRate = 250,000 bit/s
								// 10 = The number of TQ in 1 Bit transmission
								// Therefor TQ =   .4 MicroSec 
	    					/// TQ = 2*(BPR + 1)*TCAN
								// TCAN = 1/FCAN = .04 MicroSec
          					/// BPR = TQ/[2*TCAN] - 1 
								// .4/[2*.04] - 1 =  4
	
	C1CFG2bits.PRSEG = 0;			//Propagation Time Seg = 1 TQ (0b000),
	C1CFG2bits.SEG1PH = 4;			//Phase Seg 1 =  5 TQ (0b101)
	C1CFG2bits.SAM = 1;				//SAM = 1 (Sample three times)
	C1CFG2bits.SEG2PHTS = 1;		//Set Phase Seg 2 in C1CFG2 (SEG2PHTS = 1)
	C1CFG2bits.SEG2PH = 2;			//Phase Seg 2 = 3 TQ (0b010)
	C1CFG2bits.WAKFIL = 0;			//Wake up filter = 0

}
*/

// **************************************************
// ********  Can Board Interrupt Routines  **********
// **************************************************

void sel_wrong_can_int (void)
{
	SEL_CAN.IFLAG1.B.BUF00I = 1;
	SEL_CAN.IFLAG1.B.BUF01I = 1;
	SEL_CAN.IFLAG1.B.BUF02I = 1;
	SEL_CAN.IFLAG1.B.BUF03I = 1;
	SEL_CAN.IFLAG1.B.BUF04I = 1;
	sel_WCI_cnt++;
}
void sel_can_bus_off_and_warning_int (void)
{		// Bus off, Transmit count warning, Receive Count warning
	SEL_CAN.ESR.B.BOFFINT = 1;
	SEL_CAN.ESR.B.TWRNINT = 1;
	SEL_CAN.ESR.B.RWRNINT = 1;
	sel_BOI_cnt++;
}

void sel_can_error_int (void)
{		// Bit 0, Bit 1, Acknowledge, CRC, Form or Stuffing Errors
	SEL_CAN.ESR.B.ERRINT = 1;
	sel_ERI_cnt++;
}

void sel_can_rcv_int (void)
{	// Receive Interrupt (MB5)
	sel_RI_cnt++;

	sel_rcv_fifo_cnt = 0;
	while ((SEL_CAN.IFLAG1.B.BUF05I == 1) && (sel_rcv_fifo_cnt < 9))
	{			// Have a can message
		sel_receive_can_in (0);
		SEL_CAN.IFLAG1.B.BUF05I = 1;				// Clears the bit
		sel_rcv_fifo_cnt++;
	}
	if (sel_rcv_fifo_cnt > 1)
		sel_can_debug_5 = sel_rcv_fifo_cnt;
}

void sel_can_rcv_fifo_warning_int (void)
{	// Receive fifo warning (MB6)
	SEL_CAN.IFLAG1.B.BUF06I = 1;
	sel_FWI_cnt++;
}

void sel_can_rcv_fifo_overflow_int (void)
{	// Receive Fifo Overrun Interrupt (MB7)
	SEL_CAN.IFLAG1.B.BUF07I = 1;
	sel_FOI_cnt++;
}

void sel_can_trm_int_b8 (void)
{	// Transmit Interrupt
	sel_TI8_cnt++;
	SEL_CAN.IFLAG1.B.BUF08I = 1;
	sel_transmit(8);
}

void sel_can_trm_int_b9 (void)
{	// Transmit Interrupt
	sel_TI9_cnt++;
	SEL_CAN.IFLAG1.B.BUF09I = 1;
	sel_transmit(9);
}

void sel_can_trm_int_b10 (void)
{	// Transmit Interrupt
	sel_TI10_cnt++;
	SEL_CAN.IFLAG1.B.BUF10I = 1;
	sel_transmit(10);
}

void sel_can_trm_int_b11 (void)
{	// Transmit Interrupt
	sel_TI11_cnt++;
	SEL_CAN.IFLAG1.B.BUF11I = 1;
	sel_transmit(11);
}

//************************************************************
// Receive the can input
//************************************************************

void sel_receive_can_in (int16 BufIx)
{
	int16 i;
	int16 read_dly_cnt;
	
	read_dly_cnt = 0;
	while (((SEL_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) == 0x01) && (read_dly_cnt < 100))
		read_dly_cnt ++;			// checking if buffer is busy
		
	if (read_dly_cnt > 1)
		sel_can_debug_6 = read_dly_cnt;

	if ((SEL_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) != 0x01)
	{			// read of CS locks the receive buffer
		if (sel_can_rx[scrpix].valid_msg == 0)
		{
			sel_can_rx[scrpix].ex_id = (uint8)SEL_CAN.MB[BufIx].MSG.MCS.B.IDE;
			sel_can_rx[scrpix].len = (uint8)SEL_CAN.MB[BufIx].MSG.MCS.B.LENGTH;
			sel_can_rx[scrpix].rtr = (uint8)SEL_CAN.MB[BufIx].MSG.MCS.B.RTR;

			if (sel_can_rx[scrpix].len > 8)
				sel_can_rx[scrpix].len = 8;
			
			sel_can_rx[scrpix].id = SEL_CAN.MB[BufIx].MSG.MID.R;

			i = 0;
			while ((i < sel_can_rx[scrpix].len) && (i < 8))
			{
				sel_can_rx[scrpix].data[i] = SEL_CAN.MB[BufIx].MSG.MDATA.B[i];
				i++;
			}
			sel_can_rx[scrpix].valid_msg = 1;
		}
	
	}
	else
	{
		sel_can_debug_4 = SEL_CAN.MB[BufIx].MSG.MCS.B.CODE;
		sel_rcv_int_fail_cnt++;	
	}
		
	sel_can_rx[scrpix].time = (uint16)SEL_CAN.MB[BufIx].MSG.MCS.B.TIMESTAMP;		// Unlocks the buffer

	scrpix++;
	if (scrpix >= c_sel_rx_buf_siz)
		scrpix = 0;
}


//************************************************************
//*********  Check to see if the Transmit buffer is full *****
//************************************************************

int16 IsSelTxBufFul(void)
{

	_disable();
	if (sctpix == sctgix)
		sel_availbuf = c_sel_tx_buf_siz + 1;
	else if (sctpix > sctgix)
		sel_availbuf = (c_sel_tx_buf_siz - sctpix) + sctgix + 1;
	else 
		sel_availbuf = sctgix - sctpix;

	_enable();
	if (sel_availbuf > 2)
		return 0;
	else
		return 1;
}

//********************************************
// Look for data to change then transmit data
//********************************************


void Sel_Can_Control (void)
{
	unsigned char msg;
	
	Sel_Unpack_Can();
	
	sel_can_status = SEL_CAN.ESR.R;
	if (SEL_CAN.ESR.B.ERRINT == 1)
	{
		sel_can_status = SEL_CAN.ESR.R;
		SEL_CAN.ESR.B.ERRINT = 1;
	}
	else
		sel_can_status = 0;
		
	sel_can_tx_err_cnt = (uint8)SEL_CAN.ECR.B.TXECNT;
	sel_can_rx_err_cnt = (uint8)SEL_CAN.ECR.B.RXECNT;

	if (SEL_CAN.ESR.B.BOFFINT == 1)
	{		// The CAN bus is off
		SEL_CAN.ESR.B.BOFFINT = 1;
		sel_bus_off_cnt++;
		sel_can_init_error |= 0x04;
		sel_init_can_bus(0);
		record_fault(f_enc_can_bus_off);
	}
	else if (sel_bus_off_cnt > 0)
		sel_bus_off_cnt--;

#if ((Simulator == 0) || (Sim_Motion == 0))
	if ((sel_can_status & 0x00000080) != 0)
	{		// Bus is idle
		sel_bus_idle_cnt++;
//		sel_can_init_error |= 0x08;
//		record_fault(f_sel_can_bus_idle);	
	}
	else if (sel_bus_idle_cnt > 0)
		sel_bus_idle_cnt--;
	
	if ((sel_can_status & 0x00002000) != 0)
	{		// Transmit acknowledge error
		sel_ack_error_cnt++;
		sel_can_init_error |= 0x10;
//		record_fault(f_sel_can_ack_err);	
	}
	else if (sel_ack_error_cnt > 0)
		sel_ack_error_cnt--;
	
#else   
	// simulator mode
//	sel_can.online = 1;
//	return;

#endif

	if (timers[tselcanrx] > 3)
	{
		timers[tselcanrx] = 0;
		if (sel_can.online_cnt > 0)
			sel_can.online_cnt--;
		if (sel_can.online_cnt > 0)
		{
			sel_can.online = 1;
			aps_sel_can_online = 1;
			aps_sel_can_error = 0;
			statusf3 &= ~sf_APS_SEL_CAN_Flt;
		}
		else
		{
#if (Simulator == 0)
			aps_sel_can_error = 1;
			if (sel_can.online == 1)
				record_fault(f_CED_APS_can_error);
			statusf3 |= sf_APS_SEL_CAN_Flt;
#endif
			sel_can.online = 0;
			aps_sel_can_online = 0;
		}
	}
	
	// Check to see if a tx buffer is available
	if (IsSelTxBufFul() == 0)
	{		// A buffer is available

		if ((sel_can.Error_Warning.B.INTERNAL != 0) || 
			(sel_can.Error_Warning.B.COMM != 0) || 
			(sel_can.Error_Warning.B.POS_INVALID != 0) || 
			(sel_can.Error_Warning.B.VEL_INVALID != 0))
		{
			if (aps_sel_fault == 0)
				record_fault(f_CED_APS_fault);
			aps_sel_fault = 1;
		}
		else
			aps_sel_fault = 0;
		
		msg = Sel_Get_Tx_Msg_Req();
		if (msg != 0)
		{
			Sel_Pack_Can(msg,0);				// Transmit back msg that was requested
		}
	}

}


//********************************************
//*********  Pack Can data   *****************
//********************************************

void Sel_Pack_Can (unsigned char msg,unsigned char tx_req)
{
	bool valid_msg = 0;

	_disable();
	if (sctpix < c_sel_tx_buf_siz)
		sctpix++;
	else
		sctpix = 0;
	_enable();

	if (tx_req == 1)
	{
		if (msg != 0x00)
		{		
			sel_can_tx[sctpix].len = 0;
			sel_can_tx[sctpix].ex_id = 0;		// No extended frame
			sel_can_tx[sctpix].rtr = 1;			// remote transmit
			sel_can_tx[sctpix].srr = 0;			// substitute remote request (extended frame only)
			sel_can_tx[sctpix].msg = msg;
			sel_can_tx[sctpix].addr = 0x01;	
			sel_can_tx[sctpix].data[0] = 0;
			sel_can_tx[sctpix].data[1] = 0;
			sel_can_tx[sctpix].data[2] = 0;
			sel_can_tx[sctpix].data[3] = 0;
			sel_can_tx[sctpix].data[4] = 0;
			sel_can_tx[sctpix].data[5] = 0;
			sel_can_tx[sctpix].data[6] = 0;
			sel_can_tx[sctpix].data[7] = 0;
			valid_msg = 1;
		}
	}
	else switch (msg)
	{
		case 0:
			sel_can_init_error = e_inv_pack;
			sel_can_error_data = msg;
			break;
					

		default:
			sel_can_init_error = e_inv_pack;
			sel_can_error_data = msg;
			break;
	}
	
	if (valid_msg == 1)
	{
		if (sel_can_tx[sctpix].len > 8)
		{
			sel_can_tx[sctpix].len = 8;
			sel_can_init_error |= 0x40;
		}

		uiu.b[hb] = (uint8)((sel_can_tx[sctpix].msg >> 3) & 0x0F);		// Message
		uiu.b[mhb] = (uint8)((sel_can_tx[sctpix].addr & 0x7F) << 1);	// Address
		uiu.b[mlb] = 0;
		uiu.b[lb] = 0;
	 	sel_can_tx[sctpix].id = uiu.dw << 1;		// priority bits get set to zero on shift
		Tx_Sel_Can_Msg(8);
	}

}

//************************************************************
//*********  Transmit Can Message  ***************************
//************************************************************

void Tx_Sel_Can_Msg(int16 TxBufIx)
{
	if (sctgix != sctpix)
	{
		
		switch (TxBufIx)
		{
			
			case 8:
				if ((SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
				{		// The transmitter is not busy
					sel_transmit(TxBufIx);
					SEL_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
					break;
				}
				else
				{
					sel_tx_B08_fail_cnt++;
					if (sel_tx_B08_fail_cnt > 5)
					{
						sel_tx_B08_fail_cnt = 0;
						SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
					 
			case 9:
				if ((SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
				{		// The transmitter is not busy
					sel_transmit(TxBufIx);
					SEL_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
					break;
				}
				else
				{
					sel_tx_B09_fail_cnt++;
					if (sel_tx_B09_fail_cnt > 5)
					{
						sel_tx_B09_fail_cnt = 0;
						SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			case 10:
				if ((SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
				{		// The transmitter is not busy
					sel_transmit(TxBufIx);
					SEL_CAN.IFLAG1.B.BUF10I = 1;
					break;
				}
				else
				{
					sel_tx_B10_fail_cnt++;
					if (sel_tx_B10_fail_cnt > 5)
					{
						sel_tx_B10_fail_cnt = 0;
						SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			case 11:
				if ((SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
				{		// The transmitter is not busy
					sel_transmit(TxBufIx);
					SEL_CAN.IFLAG1.B.BUF11I = 1;
					break;
				}
				else
				{
					sel_tx_B11_fail_cnt++;
					if (sel_tx_B11_fail_cnt > 5)
					{
						sel_tx_B11_fail_cnt = 0;
						SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
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

void sel_transmit (int16 TxBufIx)
{
	int16 i;
	if (sctgix != sctpix)
	{		// There is data in the buffer 
 
		if (sctgix < c_sel_tx_buf_siz)
			sctgix++;
		else
			sctgix = 0;
		
				
	 	SEL_CAN.MB[TxBufIx].MSG.MID.R = sel_can_tx[sctgix].id;	

		i = 0;
		while ((i<sel_can_tx[sctgix].len) && (i < 8))
		{
		 	SEL_CAN.MB[TxBufIx].MSG.MDATA.B[i++] = sel_can_tx[sctgix].data[i];
		}
		

		SEL_CAN.MB[TxBufIx].MSG.MCS.B.LENGTH = sel_can_tx[sctgix].len;
		 	
		SEL_CAN.MB[TxBufIx].MSG.MCS.B.RTR = sel_can_tx[sctgix].rtr;
	 	
		SEL_CAN.MB[TxBufIx].MSG.MCS.B.IDE = sel_can_tx[sctgix].ex_id;

		SEL_CAN.MB[TxBufIx].MSG.MCS.B.SRR = sel_can_tx[sctgix].srr;
		 	
		 	
		SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x0C;		// Initiate transmit
		sel_can_debug_1 = SEL_CAN.MB[TxBufIx].MSG.MCS.B.CODE;
		sel_can.tx_cnt++;
	}
}

//*****************************************
// Unpack the can message
//*****************************************

void Sel_Unpack_Can (void)
{
	COMMWORD_TYPE rcv_word;
	COMMDWORD_TYPE rcv_dword;
	int16 loop_cnt = 0;
	uint8 chksum;
	uint8 calc_chksum;
	uint8 msg,addr;
	
	while ((loop_cnt <= 4)  && (scrgix != scrpix))
	{
		if (sel_can_rx[scrgix].valid_msg == 1)
		{		
		
			if (sel_can_rx[scrpix].ex_id == 1)		// Extended frame
			{
				uiu.dw = ((sel_can_rx[scrgix].id & 0x1FFC0000) << 2) ;
				uiu_2.dw = ((sel_can_rx[scrgix].id & 0x1FFC0000) >> 2);
				sel_can_rx[scrgix].addr = uiu.b[hb] & 0x7F;	// Message
				sel_can_rx[scrgix].msg = uiu_2.b[mhb] & 0x0F;    // Address
			}
			else
			{
				uiu.dw = ((sel_can_rx[scrgix].id & 0x1FFC0000) << 2) ;
				uiu_2.dw = ((sel_can_rx[scrgix].id & 0x1FFC0000) >> 2);
				sel_can_rx[scrgix].addr = uiu.b[hb] & 0x7F;	// Message
				sel_can_rx[scrgix].msg = uiu_2.b[mhb] & 0x0F;    // Address
			}
			
			msg = sel_can_rx[scrgix].msg;
			addr = sel_can_rx[scrgix].addr;
			
			if (sel_can_rx[scrgix].rtr == 1)
			{
				Sel_Put_Tx_Msg_Req(sel_can_rx[scrgix].msg);	
				sel_can.rx_cnt++;
				sel_can.online_cnt = 5;
			}
			else switch (sel_can_rx[scrgix].msg)
			{
				case 0x01:			// Message 0x01: Selector Position and Speed Data
					if (sel_can_rx[scrgix].addr == 0x01)
					{
						can_sel_seq_cnt = sel_can_rx[scrgix].data[0];
						rcv_word.byte[hbw] = sel_can_rx[scrgix].data[6];
						rcv_word.byte[lbw] = sel_can_rx[scrgix].data[7];
						can_sel_crc16 = rcv_word.word;
						can_sel_calc_crc16 = CRC16 (sel_can_rx[scrgix].data, 6);
							
						if (can_sel_calc_crc16 == can_sel_crc16)
						{
							rcv_dword.byte[hb] = 0;
							rcv_dword.byte[mhb] = sel_can_rx[scrgix].data[1];
							rcv_dword.byte[mlb] = sel_can_rx[scrgix].data[2];
							rcv_dword.byte[lb] = sel_can_rx[scrgix].data[3];
							
							if (rcv_dword.dword != 0xFFFFFF)		// not an invalid count from a bad tape read
								sel_can.pos_cnt_A = rcv_dword.dword;
								
							rcv_word.byte[hbw] = sel_can_rx[scrgix].data[4];
							rcv_word.byte[lbw] = sel_can_rx[scrgix].data[5];
							tvelocity = rcv_word.word;
							if (tvelocity != 0x7FFF)
							{
								sel_can.vel = (int16)abs(tvelocity);
								if (tvelocity > 0)
									sel_can.vel_dir = 1;
								else if (tvelocity < 0)
									sel_can.vel_dir = 2;
								else
									sel_can.vel_dir = 0;
							}
								
							sel_can.rx_cnt++;
							sel_can.online_cnt = 5;
						}
					}
					break;		
				case 0x02:			// Message 0x02: Selector Status and Errors
					if (sel_can_rx[scrgix].addr == 0x01)
					{
						can_sel_seq_cnt_2 = sel_can_rx[scrgix].data[0];
						if (can_sel_seq_cnt_2 == can_sel_seq_cnt)
						{
							rcv_word.byte[hbw] = sel_can_rx[scrgix].data[4];
							rcv_word.byte[lbw] = sel_can_rx[scrgix].data[5];
							can_sel_crc16 = rcv_word.word;
							can_sel_calc_crc16 = CRC16 (sel_can_rx[scrgix].data, 4);
							
							if (can_sel_calc_crc16 == can_sel_crc16)
							{
								
								sel_can_v1.Error.R = sel_can_rx[scrgix].data[1];
								
								
								rcv_word.byte[hbw] = sel_can_rx[scrgix].data[2];
								rcv_word.byte[lbw] = sel_can_rx[scrgix].data[3];
								sel_can_v1.Status.R = rcv_word.word;
								if (sel_can_v1.Status.B.CLIP == 1)
								{
									sel_clip_cnt = sel_can.pos_cnt_A;
									sel_clip = 1;
								}
								
								// Match Version 1 to version 2
								sel_can.Error_Warning.R = 0;		// clear all bits
								if (sel_can_v1.Error.B.GENERAL == 0x07)
									sel_can.Error_Warning.B.COMM = 1;
								if (sel_can_v1.Error.B.GENERAL == 0x06)
									sel_can.Error_Warning.B.INTERNAL = 1;
								if (sel_can_v1.Error.B.GENERAL == 0x01)
									sel_can.Error_Warning.B.POS_INVALID = 1;
								if ((sel_can_v1.Error.B.CHANNEL1 == 0x01) || (sel_can_v1.Error.B.CHANNEL2 == 0x01))
									sel_can.Error_Warning.B.POS_INVALID = 1;
								if ((sel_can_v1.Error.B.CHANNEL1 == 0x02) || (sel_can_v1.Error.B.CHANNEL2 == 0x02))
									sel_can.Error_Warning.B.VEL_INVALID = 1;
								if ((sel_can_v1.Error.B.CHANNEL1 == 0x03) || (sel_can_v1.Error.B.CHANNEL2 == 0x03))
								{
									sel_can.Error_Warning.B.POS_INVALID = 1;
									sel_can.Error_Warning.B.VEL_INVALID = 1;
								}

								// set warning and status
								
								if (sel_can_v1.Status.B.ALIGN != 0)
									sel_can.Error_Warning.B.POS_A_EX = 1;
								sel_can.Error_Warning.B.CONTRAST = sel_can_v1.Status.B.CONTRAST;
								
								sel_can.Status.R = 0;			// clear status to start
								if (sel_can_v1.Status.B.ALIGN == 0x02)
								{				// Set to moderately too close
									sel_can.Status.B.NF_ALIGN = 2;
									sel_can.Status.B.FAR = 0;
								}
								if (sel_can_v1.Status.B.ALIGN == 0x03)
								{				// Set to moderately too far
									sel_can.Status.B.NF_ALIGN = 2;
									sel_can.Status.B.FAR = 1;
								}
								if (sel_can_v1.Status.B.ALIGN == 0x04)
								{				// Set to moderately left
									sel_can.Status.B.LR_ALIGN = 2;
									sel_can.Status.B.RIGHT = 0;
								}
								if (sel_can_v1.Status.B.ALIGN == 0x05)
								{				// Set to moderately right
									sel_can.Status.B.LR_ALIGN = 2;
									sel_can.Status.B.RIGHT = 1;
								}
								
								if (sel_can_v1.Status.B.CLIP == 1)
									sel_can.Status.B.CLIP_A = 1;
								
								
								sel_can.ver = 1;		// set protocol varsion to 1
								
								sel_can.rx_cnt++;
								sel_can.online_cnt = 5;
							}
						}
					}
					break;	
				case 0x0A:			// Message 0x0A: Selector Position and Speed Data
					if (sel_can_rx[scrgix].addr == 0x01)
					{
						sel_can_data[0] = sel_can_rx[scrgix].data[0];
						sel_can_data[1] = sel_can_rx[scrgix].data[1];
						sel_can_data[2] = sel_can_rx[scrgix].data[2];
						sel_can_data[3] = sel_can_rx[scrgix].data[3];
						sel_can_data[4] = sel_can_rx[scrgix].data[4];
						sel_can_data[5] = sel_can_rx[scrgix].data[5];
						sel_can_data[6] = sel_can_rx[scrgix].data[6];
						sel_can_data[7] = sel_can_rx[scrgix].data[7];
								
						sel_can.rx_cnt++;
						sel_can.online_cnt = 5;
					}
					break;		
				case 0x0B:			// Message 0x0B: Selector Status and Errors
					if (sel_can_rx[scrgix].addr == 0x01)
					{
						sel_can_data[8] = sel_can_rx[scrgix].data[0];
						sel_can_data[9] = sel_can_rx[scrgix].data[1];
						sel_can_data[10] = sel_can_rx[scrgix].data[2];
						sel_can_data[11] = sel_can_rx[scrgix].data[3];
						
						rcv_dword.byte[hb]  = sel_can_rx[scrgix].data[4];
						rcv_dword.byte[mhb] = sel_can_rx[scrgix].data[5];
						rcv_dword.byte[mlb] = sel_can_rx[scrgix].data[6];
						rcv_dword.byte[lb]  = sel_can_rx[scrgix].data[7];
						can_sel_crc32 = rcv_dword.dword;
						can_sel_calc_crc32 = crc32(sel_can_data, 12);
						can_sel_seq_cnt = sel_can_data[0];
							
						if ((can_sel_calc_crc32 == can_sel_crc32) && (can_sel_seq_cnt_2 != can_sel_seq_cnt))
						{

							rcv_dword.byte[hb] = 0;
							rcv_dword.byte[mhb] = sel_can_data[1];
							rcv_dword.byte[mlb] = sel_can_data[2];
							rcv_dword.byte[lb] = sel_can_data[3];
							if (rcv_dword.dword != 0xFFFFFF)		// not an invalid count from a bad tape read
								sel_can.pos_cnt_A = rcv_dword.dword;
								
							rcv_word.byte[hbw] = sel_can_data[4];
							rcv_word.byte[lbw] = sel_can_data[5];
							tvelocity = rcv_word.word;
							if (tvelocity != 0x7FFF)
							{
								tvelocity = 0;
								sel_can.vel = (int16)abs(tvelocity);
								if (tvelocity > 0)
									sel_can.vel_dir = 1;
								else if (tvelocity < 0)
									sel_can.vel_dir = 2;
								else
									sel_can.vel_dir = 0;
							}

							sel_can.Error_Warning.R = sel_can_data[6];
							sel_can.Status.R = sel_can_data[7];
							rcv_dword.byte[hb] = 0;
							rcv_dword.byte[mhb] = sel_can_data[8];
							rcv_dword.byte[mlb] = sel_can_data[9];
							rcv_dword.byte[lb] = sel_can_data[10];
							if (rcv_dword.dword != 0xFFFFFF)		// not an invalid count from a bad tape read
								sel_can.pos_cnt_B = rcv_dword.dword;
							
							sel_can.clip_offset = sel_can_data[11];
							if (sel_can.Status.B.CLIP_A == 1)
							{
								sel_clip_cnt = sel_can.pos_cnt_A + (int32)sel_can.clip_offset;
								sel_clip = 1;
							}
//							else if (sel_can.Status.B.CLIP_B == 1)
//							{
//								sel_clip_cnt = sel_can.pos_cnt_B + (int32)sel_can.clip_offset;
//								sel_clip = 1;
//							}

							sel_can.ver = 2;		// set protocol varsion to 2
							
							sel_can.rx_cnt++;
							sel_can.online_cnt = 5;
						}
						can_sel_seq_cnt_2 = can_sel_seq_cnt;
					}
					break;	
				case 0x05:		//	Elgo position count packet
					if (sel_can_rx[scrgix].addr == 0x18)
					{
						chksum = sel_can_rx[scrgix].data[7];
						calc_chksum = check_sum(sel_can_rx[scrgix].data,7);
						if (chksum == calc_chksum)
						{
							
							rcv_dword.byte[hb] = sel_can_rx[scrgix].data[0];
							rcv_dword.byte[mhb] = sel_can_rx[scrgix].data[1];
							rcv_dword.byte[mlb] = sel_can_rx[scrgix].data[2];
							rcv_dword.byte[lb] = sel_can_rx[scrgix].data[3];
							sel_can.pos_cnt_A = rcv_dword.dword;

							rcv_word.byte[hbw] = sel_can_rx[scrgix].data[4];
							rcv_word.byte[lbw] = sel_can_rx[scrgix].data[5];
							tvelocity = rcv_word.word;
							if (tvelocity != 0x7FFF)
							{
								sel_can.vel = (int16)abs(tvelocity);
								if (tvelocity > 0)
									sel_can.vel_dir = 1;
								else if (tvelocity < 0)
									sel_can.vel_dir = 2;
								else
									sel_can.vel_dir = 0;
							}
							sel_can.Error_Warning.R = 0;
							if ((sel_can_rx[scrgix].data[6] & 0x01) != 0)
								sel_can.Error_Warning.B.POS_INVALID = 1;
							if ((sel_can_rx[scrgix].data[6] & 0x10) != 0)
								sel_can.Error_Warning.B.POS_B_EX = 1;

							sel_can.ver = 3;		// set protocol varsion to 3
							
							sel_can.rx_cnt++;
							sel_can.online_cnt = 5;
						}
					}
					break;
				default:
					if (sel_can_rx[scrgix].msg != 0) 
					{
						sel_can_init_error = e_inv_unpk;
						sel_can_error_data = sel_can_rx[scrgix].msg;
					}
					break;
			}
			sel_can_rx[scrgix].valid_msg = 0;
		}
		scrgix++;
		if (scrgix >= c_sel_rx_buf_siz)
			scrgix = 0; 
		loop_cnt++;
	}
}

// ******************************************************
// **********  Transmit Msg Request Put Queue ***********
// ******************************************************

unsigned char Sel_Put_Tx_Msg_Req (unsigned char msg)
{
	unsigned char n;
	for(n=0;n<=4;n++)
	{
		if (sel_can_tx_msg_req[n] == 0)
		{
			sel_can_tx_msg_req[n] = msg;
			return 1;
		}
	}
	return 0;
}
// ******************************************************
// **********  Transmit Msg Request Get Queue ***********
// ******************************************************

unsigned char Sel_Get_Tx_Msg_Req (void)
{
	unsigned char n;
	unsigned char msg;

	msg = 0;
	for(n=0;n<=4;n++)
	{
		if (sel_can_tx_msg_req[n] != 0)
		{
			msg = sel_can_tx_msg_req[n];
			sel_can_tx_msg_req[n] = 0;
			return msg;
		}
	}
	return 0;
}

//**************************************************
// Calculate checksum for ELGO selector
//**************************************************

uint8 check_sum (unsigned char *data, int16 cnt)
{
	int16 i;
	uint8 chksum = 0;
	
	for (i=0;i<cnt;i++)
	{
		chksum += data[i];
	}
	return (chksum);
}


//**************************************************
// Calculate 16 bit CRC for Data (CRC-CCITT(0xFFFF))
//**************************************************

uint16 CRC16 (unsigned char *data, int16 cnt)
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


//**************************************************
// Calculate 32 bit CRC for Data 
//**************************************************

uint32 crc32(uint8 *buff, int16 len) 
{
	int16 i, j;
 	uint32 byte, crc, mask;
	crc = 0xFFFFFFFF;
	for (i=0;i<len;i++) 
	{
		byte = (uint32)buff[i]; // Get next byte.
		crc = crc ^ byte;
		for (j = 7; j >= 0; j--) 
		{ // Do eight times.
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
	}
	return ~crc;
}
/* Revision History
*/
