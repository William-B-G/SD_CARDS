#define d_ENC_CAN 1
#include "global.h"
//****************************************************************
//****************************************************************
//******* Routines to run the CAN Bus
//****************************************************************
//****************************************************************

/*
		Can Port Device Map

	Can Open Protocol

	Device		  Number
	Encoder			3F			Encoder Device Address (bits 6-0) Default
		
	Baud rate 250 K bits/s		Default

	CAN PROTOCOL  (Standard address bits only)

	Frame/Length     
	Identifier 1 (6bits) (Message Bits: 10-7) (Device Address:Bits 6,5)	
	Identifier 2 (5bits) (Device Address:Bits 4-0)	 	
	Data Byte 1	 
	Data Byte 2
	Data Byte 3 
	Data Byte 4
	Data Byte 5
	Data Byte 6
	Data Byte 7
	Data Byte 8


	ID is setup as Message (bits 7-10) + Device Address (bits 0-6)
	
	Message Types:
	000 		Node Management 
	080 		Sync
	080 + Addr 	Emergency
	180 + Addr 	Position + Data from encoder
	280 + Addr 	Position + Data from encoder
	380 + Addr 	Position * Data from encoder
	480 + Addr 	Reserved
	200 + Addr 	Data to encoder (not used)
	300 + Addr 	Data to encoder (not used)
	400 + Addr 	Data to encoder (not used)
	500 + Addr 	Data to encoder (not used)
	
	580 + Addr 	Setup data from encoder
	600 + Addr 	Setup data to encoder
	
	700 + Addr 	Node Management monitor	

	Service Data Definitions:

		Host sends write and read commands to the slave device with an address(index) and index(sub index) for the data.  The slave
	    responds with a the data, a write acknowledge or an error code with an error number.
	
		The host sends:
		Byte 0 CMD 
				CMD = 0x23: Sending 4 bytes in bytes 4-7
					= 0x2B: Sending 2 bytes in bytes 4,5
					= 0x2F: Sending 1 byte in byte 4
					= 0x40: Request to receive data (bytes 4-7 not used)
		Byte 1 Index LSByte  Index = 16 bit address of the data
		Byte 2 Index MSByte
		Byte 3 Sub Index     Sub Index = 8 bit index from the address  
		Byte 4 Data LSB
		Byte 5 Data 
		Byte 6 Data
		Byte 7 Data MSB
	
		The Slave Responds:
		Byte 0: RES
				RES = 0x43 Returning read request 4 bytes in bytes 4-7 
					= 0x4B Returning read request 2 bytes in byte 4,5
					= 0x4F Returning read request 1 byte in byte 4
					= 60 Data from write request received successfully (bytes 4-7 not used)
					= 80 Error, bytes 4-7 contain error code
		Byte 1 Index LSByte  Index = 16 bit address of the data
		Byte 2 Index MSByte
		Byte 3 Sub Index     Sub Index = 8 bit index from the address  
		Byte 4 Data LSB
		Byte 5 Data 
		Byte 6 Data
		Byte 7 Data MSB


	Return Error Code:
	0503 0000h Toggle bit not alternated.
	0504 0000h SDO protocol timed out.
	0504 0001h Client/server command specifier not valid or unknown.
	0504 0002h Invalid block size (block mode only).
	0504 0003h Invalid sequence number (block mode only).
	0504 0004h CRC error (block mode only).
	0504 0005h Out of memory.
	0601 0000h Unsupported access to an object.	
	0601 0001h Attempt to read a write only object.
	0601 0002h Attempt to write a read only object.
	0602 0000h Object does not exist in the object dictionary.
	0604 0041h Object cannot be mapped to the PDO.
	0604 0042h The number and length of the objects to be mapped would exceed PDO length.
	0604 0043h General parameter incompatibility reason.
	0604 0047h General internal incompatibility in the device.
	0606 0000h Access failed due to a hardware error.
	0607 0010h Data type does not match, length of service parameter does not match
	0607 0012h Data type does not match, length of service parameter too high
	0607 0013h Data type does not match, length of service parameter too low
	0609 0011h Sub-index does not exist.
	0609 0030h Value range of parameter exceeded (only for write access).
	0609 0031h Value of parameter written too high.
	0609 0032h Value of parameter written too low.
	0609 0036h Maximum value is less than minimum value.
	0800 0000h general error
	0800 0020h Data cannot be transferred or stored to the application.
	0800 0021h Data cannot be transferred or stored to the application because of local control.
	0800 0022h Data cannot be transferred or stored to the application because of the present device state.
	0800 0023h Object dictionary dynamic generation fails or no object dictionary is present (e.g.
			   object dictionary is generated from file and generation fails because of a file error).


	Setup Sequence:
	
		Place device in pre-operational mode
		
		Encoder check and setup
			read position and save 6004h
			read and set event mode 1800h
			read and set mapping 1A00h
			read and set termination
			read and set error behavior 1029h
			read and set measuring units per rev 6001h
			read and set total measuring range 6002h
			read and set direction 6000h
			preset stored position 6003h
			return error code

		Place encoder in operational mode
		
		do forever
			read encoder and position
				return error code		



  Message 	To			From		 Bytes		Description
	180		CPU			ENC				3		Message #180: Encoder sends PROCESS data (position count)
													Position Count LSB
													Position Count high LSB
													Position Count low MSB
													Position Count MSB
													Speed LSB
													Speed MSB
													Acceleration LSB
													Acceleration MSB
													
	280		CPU			ENC				3		Message #280: Encoder sends PROCESS data (position count)
													Position Count LSB
													Position Count high LSB
													Position Count low MSB
													Position Count MSB
													Speed LSB
													Speed MSB
													Acceleration LSB
													Acceleration MSB

	380		CPU			ENC				3		Message #380: Encoder sends PROCESS data (position count)
													Position Count LSB
													Position Count high LSB
													Position Count low MSB
													Position Count MSB
													Speed LSB
													Speed MSB
													Acceleration LSB
													Acceleration MSB
													
	580		CPU			ENC				8		Message #580: ACK/RETURN Setup Data
													Byte 0: RES
															RES = 0x43 Returning read request 4 bytes in bytes 4-7 
																= 0x4B Returning read request 2 bytes in byte 4,5
																= 0x4F Returning read request 1 byte in byte 4
																= 60 Data from write request received successfully (bytes 4-7 not used)
																= 80 Error, bytes 4-7 contain error code
													Byte 1 Index LSByte  Index = 16 bit address of the data
													Byte 2 Index MSByte
													Byte 3 Sub Index     Sub Index = 8 bit index from the address  
													Byte 4 Data LSB
													Byte 5 Data 
													Byte 6 Data
													Byte 7 Data MSB
	 
	600 	ENC			CPU				8		Message #600: Read/Write Setup Data
													Byte 0 CMD 
															CMD = 0x23: Sending 4 bytes in bytes 4-7
																= 0x2B: Sending 2 bytes in bytes 4,5
																= 0x2F: Sending 1 byte in byte 4
																= 0x40: Request to receive data (bytes 4-7 not used)
													Byte 1 Index LSByte  Index = 16 bit address of the data
													Byte 2 Index MSByte
													Byte 3 Sub Index     Sub Index = 8 bit index from the address  
													Byte 4 Data LSB
													Byte 5 Data 
													Byte 6 Data
													Byte 7 Data MSB
					
																										
													 
												
*/

// Can Routine declarations
int16 EncChipFreezeMode(unsigned char set);
void read_encoder_init(void);
void init_encoder(void);
void enc_init_can_bus (int16 baud_change);
void enc_wrong_can_int (void);
void enc_can_bus_off_and_warning_int (void);
void enc_can_error_int (void);
void enc_can_rcv_int (void);
void enc_can_rcv_fifo_warning_int (void);
void enc_can_rcv_fifo_overflow_int (void);
void enc_can_trm_int_b8 (void);
void enc_can_trm_int_b9 (void);
void enc_can_trm_int_b10 (void);
void enc_can_trm_int_b11 (void);
void Enc_Can_Control (void);
void enc_transmit (int16 TxBufIx);
void enc_receive_can_in (int16 BufIx);
void Tx_Enc_Can_Msg(int16 TxBufIx);

void Enc_Pack_Can (unsigned char msg, unsigned char tx_req);
void Pack_Service_Msg (int16 msg, int16 read_write);
void Enc_Unpack_Can (void);
void Unpack_Service_Msg (void);
unsigned char Enc_Put_Tx_Msg_Req (unsigned char msg);
unsigned char Enc_Get_Tx_Msg_Req (void);
int16 IsEncTxBufFul(void);


unsigned char encoder_node_id = 0x3F;			// 63

#define EVENT_TIMER 3			// 3 Msec

int32 can_enc_count = 0;
int32 can_enc_preset_count = 0;
int32 can_enc_speed = 0;
int32 can_enc_accel = 0;
int16 can_enc_ppr = (uint16)0xFFFF;
int16 can_enc_dir = 0;
int32 can_enc_range = 0x02000000;
int16 can_enc_max_rev = 8192;
int16 can_enc_op_status = 0x4;	// bit set for scaling function enabled so that resolution can be changed
int16 can_enc_online_cnt = 0;
int16 can_enc_rx_cnt = 0;
int16 can_enc_tx_cnt = 0;
int16 can_enc_baud = 0;
int16 can_enc_statusf = 0;		// can encoder status flag bit 0: 1 = Initialized the encoeder to default values

bool can_enc_init = 0;
bool can_enc_com_err = 0;
bool can_enc_online = 0;

bool WriteEncCount = 0;	
bool WriteEncPPR = 0;
bool ReadEncPPR = 0;
bool WriteEncDir = 0;
bool WriteEncMaxPulses = 0;
bool SaveEncPar	= 0;
bool SaveEncAppPar	= 0;
bool EncCan_Upd = 0;
bool Enc_Cnt_Reset = 0;
bool SaveEncBusPar = 0;

int8 enc_app_par_sav = 0;
int8 enc_cnt_wrt = 0;
int8 enc_par_sav = 0;

int16 Enc_Packet_Req = 0;

int16 enc_can_init_count = 0;
int16 enc_init_mode_cnt = 0;
bool enc_in_chip_reset = false;

int16 can_enc_init_baud_cnt = 0;
int16 can_enc_init_fail_cnt = 0;
bool can_enc_init_fail = 0;
bool init_can_encoder = 0;
unsigned char enc_init_seq = 0;
unsigned char enc_read_init_seq = 0;
bool ReInitEncFlg = 0;
uint8 nmb_elements_mapped = 1;

int32 enc_can_status = 0;

int16 enc_can_error=0;
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

int16 enc_can_error_data=0;
#define e_inv_pack 4
#define e_inv_unpk 12
#define c_Freeze_Start		0x01	// put chip in freeze mode
#define c_Freeze_Stop		0x00	// Takes chip out of freeze mode

int16 enc_can_int_count = 0;
int16 enc_can_tx_err_cnt = 0;
int16 enc_can_rx_err_cnt = 0;
int16 enc_rcv_fifo_cnt = 0;
int16 enc_rcv_int_fail_cnt = 0;
int16 enc_WCI_cnt=0;	// Wrong can interrupt
int16 enc_BOI_cnt=0;	// Bus Off and tx/rx warning interrupt count
int16 enc_ERI_cnt=0;	// Error Interrupt count
int16 enc_RI_cnt=0;		// Receive fifo interrupt count
int16 enc_FWI_cnt=0;	// Fifo warning interrupt count
int16 enc_FOI_cnt=0;	// Fifo Overflow interrupt count
int16 enc_TI8_cnt=0;	// Transmit buffer 8 interrupt count
int16 enc_TI9_cnt=0;	// Transmit buffer 9 interrupt count
int16 enc_TI10_cnt=0;	// Transmit buffer 10 interrupt count
int16 enc_TI11_cnt=0;	// Transmit buffer 11 interrupt count
int16 enc_ack_error_cnt = 0;
int16 enc_bus_off_cnt = 0;
int16 enc_bus_idle_cnt = 0;

int16 enc_tx_B08_fail_cnt = 0;
int16 enc_tx_B09_fail_cnt = 0;
int16 enc_tx_B10_fail_cnt = 0;
int16 enc_tx_B11_fail_cnt = 0;

int32 enc_can_debug_1=0;
int32 enc_can_debug_2=0;
int32 enc_can_debug_3=0;
int32 enc_can_debug_4=0;
int32 enc_can_debug_5=0;
int32 enc_can_debug_6=0;


unsigned char enc_can_tx_msg_req[6];

int16 tx_sel = 0;

// Receive Buffer

#define c_enc_rx_buf_siz 10

struct tag_enc_can_rx {
		unsigned char valid_msg;	// 1 when valid message in buffer
		unsigned char len;
		unsigned char ex_id;	// flag for extended id mode
		unsigned char rtr;		// Remote Transmission Request
		unsigned char msg;		// Message
		unsigned char addr;		// Address
		uint32 id;
		unsigned char data [8];
		uint16 time;
} enc_can_rx[c_enc_rx_buf_siz+1];

struct tag_can_open_rx {
		unsigned char len;
		unsigned char msg;			
		unsigned char node_id;
		unsigned char cmd;			
		uint16 index;			
		unsigned char sub_index;			
		unsigned char data [8];
		uint32 error;
} can_open_rx;
	

int16 ecrpix = 0;					// encoder can receive buffer put index
int16 ecrgix = 0;					// encoder can receive buffer get index
int16 ectpix = 0;					// encoder can transmit buffer put index
int16 ectgix = 0;					// encoder can transmit buffer get index
int16 enc_availbuf = 0;				// transmit buffer space available
	
	
// Transmit buffer 
#define c_enc_tx_buf_siz 10

struct tag_enc_can_tx {
		unsigned char len;
		unsigned char ex_id;	// flag for extended id mode
		unsigned char rtr;		// Remote Transmission Request
		unsigned char srr;		// Substitute Remote Request
		unsigned char msg;		// Message
		unsigned char addr;		// Address
		uint32 id;
		unsigned char data [8];
} enc_can_tx[c_enc_tx_buf_siz+1];

struct tag_can_open_tx {
		unsigned char len;
		unsigned char msg;			
		unsigned char node_id;
		unsigned char cmd;			
		uint16 index;			
		unsigned char sub_index;			
		unsigned char data [8];
		uint32 error;
} can_open_tx;

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
	#define ENC_CAN CAN_B
	#define ENC_CAN_INT_BASE 157
//#else
//	#define ENC_CAN CAN_D
//	#define ENC_CAN_INT_BASE 199
//#endif


//#define ENC_CAN CAN_C
//#define ENC_CAN_INT_BASE 178

//*******************************************************************
// Sends the freeze bit to the chip
// Value of 1 puts the chip in freeze mode
// Value of 0 takes is out of freeze mode
// Have to see the freeze mode acknowledge bit gets set
//*******************************************************************
int16 EncChipFreezeMode(unsigned char set)
{
	
	if (enc_in_chip_reset == true)
		return 0;

	enc_in_chip_reset = true;

	enc_init_mode_cnt = 0;

	ENC_CAN.MCR.B.MDIS = 0;			// Enable module after reset
	ENC_CAN.MCR.B.FRZ = 1;			// Set or clear the freeze bit
	ENC_CAN.MCR.B.HALT = set;			// Clear halt mode

	
	while (enc_init_mode_cnt < 200)
	{
		if (ENC_CAN.MCR.B.FRZACK == set)
		{
			enc_in_chip_reset = false;
			return 1;
		}
		enc_init_mode_cnt++;
	}
	enc_in_chip_reset = false;
	return 0;		// Failed to initialize
}

// **************************************************
// ****  Initialize the ECAN 1000 CAN interface Board
// **************************************************


void enc_init_can_bus (int16 baud_change)
{	 
					// baud_change = 0: use parameter
					// baud_change = 1: set 125K
					// baud_change = 2: set 250k
	int16 reset_count;

	// See if the board is there
	enc_can_init_count++;

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
	INTC_InstallINTCInterruptHandler(enc_can_bus_off_and_warning_int, ENC_CAN_INT_BASE, 1);  
	INTC_InstallINTCInterruptHandler(enc_can_error_int, ENC_CAN_INT_BASE+1, 1);  
	INTC_InstallINTCInterruptHandler(enc_wrong_can_int, ENC_CAN_INT_BASE+3, 1);  
	INTC_InstallINTCInterruptHandler(enc_wrong_can_int, ENC_CAN_INT_BASE+4, 1);  
	INTC_InstallINTCInterruptHandler(enc_wrong_can_int, ENC_CAN_INT_BASE+5, 1);  
	INTC_InstallINTCInterruptHandler(enc_wrong_can_int, ENC_CAN_INT_BASE+6, 1);  
	INTC_InstallINTCInterruptHandler(enc_wrong_can_int, ENC_CAN_INT_BASE+7, 1);  
	INTC_InstallINTCInterruptHandler(enc_can_rcv_int, ENC_CAN_INT_BASE+8, 1);  
	INTC_InstallINTCInterruptHandler(enc_can_rcv_fifo_warning_int, ENC_CAN_INT_BASE+9, 1);  
	INTC_InstallINTCInterruptHandler(enc_can_rcv_fifo_overflow_int, ENC_CAN_INT_BASE+10, 1);  
	INTC_InstallINTCInterruptHandler(enc_can_trm_int_b8, ENC_CAN_INT_BASE+11, 1);  
	INTC_InstallINTCInterruptHandler(enc_can_trm_int_b9, ENC_CAN_INT_BASE+12, 1);  
	INTC_InstallINTCInterruptHandler(enc_can_trm_int_b10, ENC_CAN_INT_BASE+13, 1);  
	INTC_InstallINTCInterruptHandler(enc_can_trm_int_b11, ENC_CAN_INT_BASE+14, 1);  
	
	// Send init to can board
	if (EncChipFreezeMode(c_Freeze_Start) == 1)
	{

		enc_can_init_count++;
		if ((fvars[fvencnode] >= 1) && (fvars[fvencnode] <= 127))
			encoder_node_id = (uint8)(fvars[fvencnode] & 0x7F);
		else
			encoder_node_id = 0x3F;		// 63
		

		ENC_CAN.MCR.B.SOFTRST = 1;		// Soft Reset
		reset_count = 0;
		while ((ENC_CAN.MCR.B.SOFTRST == 1) && (reset_count < 1000))
			reset_count++;
			
		ENC_CAN.MCR.B.MDIS = 0;			// Enable module after reset

	    // Clear of all interrupts
	    ENC_CAN.IFLAG1.R = 0xFFFFFFFF;
	    ENC_CAN.IFLAG2.R = 0xFFFFFFFF;
	    
	    // Disable of Interrupts
		ENC_CAN.IMASK1.R = 0;
		ENC_CAN.IMASK2.R = 0;
		
		ENC_CAN.MCR.B.BCC = 1;			// Allow backward compatibility since all mask will be set to 0xffff
		ENC_CAN.MCR.B.MAXMB = 0x0B;		// Set to 12 (0xC - 1)
		ENC_CAN.MCR.B.WRNEN = 0;			// Warning interrupts
		ENC_CAN.MCR.B.SRXDIS = 1;			// Self Reception disabled
		ENC_CAN.MCR.B.FEN = 1;			// Enable RX fifo
		ENC_CAN.MCR.B.ABTEN = 1;			// Abort enable
		ENC_CAN.MCR.B.LPRIOEN = 0;		// No local priority mode
		ENC_CAN.MCR.B.IDAM = 0;			// ID Acceptance mode (one full id per filter element)
		
		
	    // Configure clock AND baud rate timer for 250,000 baud
	    
	    /*	
	    	fTq = Bit Rate * Number of time Quanta = fCANCLK / Prescale Value
	    	Prescale Value = fCANCLK/(Bit Rate * Number of Time Quanta)
	    	Prescale Value = 40 Mhz / (250000 * 16) = 10.000
	    */
	    
	    
	    ENC_CAN.CTRL.B.CLKSRC = 0; 		// System Clock 40.0 Mhz
	    if (baud_change == 0)
	    {
	    	
		    if (fvars[fvencbaud] == 0)
		    	ENC_CAN.CTRL.B.PRESDIV = 9;		// Prescale Devision Factor + 1 = Prescale Value for 250000
			else 
		    	ENC_CAN.CTRL.B.PRESDIV = 19;		// Prescale Devision Factor + 1 = Prescale Value for 125000
	    }
	    else if (baud_change == 1)
	    	ENC_CAN.CTRL.B.PRESDIV = 19;		// Prescale Devision Factor + 1 = Prescale Value for 125000
	    else
	    	ENC_CAN.CTRL.B.PRESDIV = 9;		// Prescale Devision Factor + 1 = Prescale Value for 250000
		
		ENC_CAN.CTRL.B.RJW = 2;			// Resync Jump Width = RJW + 1 = 3
		// Total Tq = 16
		ENC_CAN.CTRL.B.PSEG1 = 7;			// Phase Segment 1 = (PSEG1 + 1) x Tq = 8 (also sync seg = 1)
		ENC_CAN.CTRL.B.PSEG2 = 5;			// Phase Segment 2 = (PSEG2 + 1) x Tq = 6
		ENC_CAN.CTRL.B.PROPSEG = 0;		// Propagation Segment time = (PROPSEG + 1) x Tq = 1
		ENC_CAN.CTRL.B.SMP = 1;			// Three samples
		
		ENC_CAN.CTRL.B.BOFFREC = 0;		// Automamtic recovery
		ENC_CAN.CTRL.B.TSYN = 0;			// Timer sync disabled
		ENC_CAN.CTRL.B.LBUF = 1;			// Lowest buffer transmitted first
		ENC_CAN.CTRL.B.LOM = 0;			// Listen only mode disabled
		
		ENC_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
		ENC_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
		ENC_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
		ENC_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled


		// Set Acceptance Mask
		
		ENC_CAN.RXIMR[0].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[1].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[2].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[3].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[4].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[5].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[6].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[7].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[8].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[9].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[10].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[11].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[12].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[13].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[14].R = 0x00000000;		// Enable all packets
		ENC_CAN.RXIMR[15].R = 0x00000000;		// Enable all packets

		// Set Receive ID
		ENC_CAN.MB[6].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		ENC_CAN.MB[6].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		ENC_CAN.MB[6].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		ENC_CAN.MB[6].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID
		ENC_CAN.MB[7].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		ENC_CAN.MB[7].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		ENC_CAN.MB[7].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		ENC_CAN.MB[7].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID

		ENC_CAN.MB[8].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		ENC_CAN.MB[9].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		ENC_CAN.MB[10].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		ENC_CAN.MB[11].MSG.MCS.B.CODE = 0x08;		// write inactive mode

		if (EncChipFreezeMode(c_Freeze_Stop) == 1)
		{		// initialize can board
			enc_can_init_count++;
			
		    // Enable interrupts 
			ENC_CAN.IMASK1.R = 0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
			ENC_CAN.IMASK2.R = 0x00;		// MB 32-63 NOT Enabled
			ENC_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
			ENC_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
			ENC_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
			ENC_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled
		}
		else
		{
			// Set error code
			enc_can_error = 0x01;
		}		
	}
	else
	{
		// Set error code
		enc_can_error = 0x02;
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

void enc_wrong_can_int (void)
{
	ENC_CAN.IFLAG1.B.BUF00I = 1;
	ENC_CAN.IFLAG1.B.BUF01I = 1;
	ENC_CAN.IFLAG1.B.BUF02I = 1;
	ENC_CAN.IFLAG1.B.BUF03I = 1;
	ENC_CAN.IFLAG1.B.BUF04I = 1;
	enc_WCI_cnt++;
}
void enc_can_bus_off_and_warning_int (void)
{		// Bus off, Transmit count warning, Receive Count warning
	ENC_CAN.ESR.B.BOFFINT = 1;
	ENC_CAN.ESR.B.TWRNINT = 1;
	ENC_CAN.ESR.B.RWRNINT = 1;
	enc_BOI_cnt++;
}

void enc_can_error_int (void)
{		// Bit 0, Bit 1, Acknowledge, CRC, Form or Stuffing Errors
	ENC_CAN.ESR.B.ERRINT = 1;
	enc_ERI_cnt++;
}

void enc_can_rcv_int (void)
{	// Receive Interrupt (MB5)
	enc_RI_cnt++;

	enc_rcv_fifo_cnt = 0;
	while ((ENC_CAN.IFLAG1.B.BUF05I == 1) && (enc_rcv_fifo_cnt < 9))
	{			// Have a can message
		enc_receive_can_in (0);
		ENC_CAN.IFLAG1.B.BUF05I = 1;				// Clears the bit
		enc_rcv_fifo_cnt++;
	}
	if (enc_rcv_fifo_cnt > 1)
		enc_can_debug_5 = enc_rcv_fifo_cnt;
}

void enc_can_rcv_fifo_warning_int (void)
{	// Receive fifo warning (MB6)
	ENC_CAN.IFLAG1.B.BUF06I = 1;
	enc_FWI_cnt++;
}

void enc_can_rcv_fifo_overflow_int (void)
{	// Receive Fifo Overrun Interrupt (MB7)
	ENC_CAN.IFLAG1.B.BUF07I = 1;
	enc_FOI_cnt++;
}

void enc_can_trm_int_b8 (void)
{	// Transmit Interrupt
	enc_TI8_cnt++;
	ENC_CAN.IFLAG1.B.BUF08I = 1;
	enc_transmit(8);
}

void enc_can_trm_int_b9 (void)
{	// Transmit Interrupt
	enc_TI9_cnt++;
	ENC_CAN.IFLAG1.B.BUF09I = 1;
	enc_transmit(9);
}

void enc_can_trm_int_b10 (void)
{	// Transmit Interrupt
	enc_TI10_cnt++;
	ENC_CAN.IFLAG1.B.BUF10I = 1;
	enc_transmit(10);
}

void enc_can_trm_int_b11 (void)
{	// Transmit Interrupt
	enc_TI11_cnt++;
	ENC_CAN.IFLAG1.B.BUF11I = 1;
	enc_transmit(11);
}

//************************************************************
// Receive the can input
//************************************************************

void enc_receive_can_in (int16 BufIx)
{
	int16 i;
	int16 read_dly_cnt;
	
	read_dly_cnt = 0;
	while (((ENC_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) == 0x01) && (read_dly_cnt < 100))
		read_dly_cnt ++;			// checking if buffer is busy
		
	if (read_dly_cnt > 1)
		enc_can_debug_6 = read_dly_cnt;

	if ((ENC_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) != 0x01)
	{			// read of CS locks the receive buffer
		if (enc_can_rx[ecrpix].valid_msg == 0)
		{
			enc_can_rx[ecrpix].ex_id = (uint8)ENC_CAN.MB[BufIx].MSG.MCS.B.IDE;
			enc_can_rx[ecrpix].len = (uint8)ENC_CAN.MB[BufIx].MSG.MCS.B.LENGTH;
			enc_can_rx[ecrpix].rtr = (uint8)ENC_CAN.MB[BufIx].MSG.MCS.B.RTR;

			if (enc_can_rx[ecrpix].len > 8)
				enc_can_rx[ecrpix].len = 8;
			
			enc_can_rx[ecrpix].id = ENC_CAN.MB[BufIx].MSG.MID.R;

			i = 0;
			while ((i < enc_can_rx[ecrpix].len) && (i < 8))
			{
				enc_can_rx[ecrpix].data[i] = ENC_CAN.MB[BufIx].MSG.MDATA.B[i];
				i++;
			}
			enc_can_rx[ecrpix].valid_msg = 1;
		}
	
	}
	else
	{
		enc_can_debug_4 = ENC_CAN.MB[BufIx].MSG.MCS.B.CODE;
		enc_rcv_int_fail_cnt++;	
	}
		
	enc_can_rx[ecrpix].time = (uint16)ENC_CAN.MB[BufIx].MSG.MCS.B.TIMESTAMP;		// Unlocks the buffer

	ecrpix++;
	if (ecrpix >= c_enc_rx_buf_siz)
		ecrpix = 0;
}

//***********************************************
// *** Read the encoder initialization 
//***********************************************
/*
		Place device in pre-operational mode
		
		Encoder check and setup
			read position and save 6004h
			read and set event mode 1800h
			read and set mapping 1A00h
			read and set termination
			read and set error behavior 1029h
			read and set measuring units per rev 6001h
			read and set total measuring range 6002h
			read and set direction 6000h
			preset stored position 6003h
			return error code

		Place encoder in operational mode
		
		do forever
			read encoder and position
				return error code		
*/

void read_encoder_init(void)
{
	COMMDWORD_TYPE read_enc_dw;
	COMMWORD_TYPE read_enc_w;
	int16 t_baud = 0;

	switch (enc_read_init_seq)
	{
		case 0:		// start of init sequence 
			Enc_Pack_Can(0x80,0);		// put device in pre-operational mode
			enc_read_init_seq = 1;					
			break;
		case 1:						// Get the current position
			Pack_Service_Msg(64,0);			// Read position count
			timers[tenc_init] = 0;
			enc_read_init_seq = 2;
			break;
		case 2:		// receive poscnt
			if ((pkt_ret.pkt == 64) && (pkt_ret.stat ==  2))
			{
				read_enc_dw.byte[0] = can_open_rx.data[lb];
				read_enc_dw.byte[1] = can_open_rx.data[mlb];
				read_enc_dw.byte[2] = can_open_rx.data[mhb];
				read_enc_dw.byte[3] = can_open_rx.data[hb];
				can_enc_count = read_enc_dw.dword;
//				prev_Enc_Count = can_enc_count;	
//				Prev_DPP_Count = can_enc_count;
//				Clear_Vel_Array = 1;

				enc_read_init_seq = 3;
				timers[tenc_init]  = 0;
				Pack_Service_Msg(10,0);			// read Transmit Communications Parameters	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 3:		// read Transmit Communications Parameters
			if ((pkt_ret.pkt == 10) && (pkt_ret.stat ==  2))
			{
				if ((can_open_rx.data[0] < 1) || (can_open_rx.data[0] > 5))
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 4;
					timers[tenc_init] = 0;
					Pack_Service_Msg(12,0);			// read Transmit PDO-1 Type
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 4:		// read Transmit PDO-1 Type
			if ((pkt_ret.pkt == 12) && (pkt_ret.stat ==  2))
			{
				if ((can_open_rx.data[0] != 255) && (can_open_rx.data[0] != 254))
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 5;
					timers[tenc_init] = 0;
					Pack_Service_Msg(15,0);			// read Transmit PDO-1 Event Timer
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 5:		// read Transmit PDO-1 Event Timer
			if ((pkt_ret.pkt == 15) && (pkt_ret.stat ==  2))
			{
				if (can_open_rx.data[0] != EVENT_TIMER)
					init_can_encoder = 1;
				else
				{
					if (((cons[enctype] == 1) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 2))
					{					 // only dynapar
						enc_read_init_seq = 12;
						timers[tenc_init] = 0;
						Pack_Service_Msg(61,0);			// read Measuring Steps per Revolution					
					}
					else
					{
						enc_read_init_seq = 6;
						timers[tenc_init] = 0;
						Pack_Service_Msg(16,0);			// read Transmit PDO-1 Mapping Number of Elements
					}
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 6:		// read Transmit PDO-1 Mapping Number of Elements
			if ((pkt_ret.pkt == 16) && (pkt_ret.stat ==  2))
			{
				if (can_open_rx.data[0] != nmb_elements_mapped)
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 7;
					timers[tenc_init] = 0;
					Pack_Service_Msg(17,0);			// read Transmit PDO-1 Mapping position location
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 7:		// read Transmit PDO-1 Mapping position location
			if ((pkt_ret.pkt == 17) && (pkt_ret.stat ==  2))
			{
				read_enc_dw.byte[0] = can_open_rx.data[lb];
				read_enc_dw.byte[1] = can_open_rx.data[mlb];
				read_enc_dw.byte[2] = can_open_rx.data[mhb];
				read_enc_dw.byte[3] = can_open_rx.data[hb];
				if (read_enc_dw.dword != 0x60040020)
					init_can_encoder = 1;
				else
				{
					timers[tenc_init] = 0;
					if (((cons[enctype] == 0) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 1))
					{						  // only turck
						enc_read_init_seq = 8;
						Pack_Service_Msg(51,0);			// read Comm fault error behavior
					}
					else
					{
						enc_read_init_seq = 12;
						timers[tenc_init] = 0;
						Pack_Service_Msg(61,0);			// read Measuring Steps per Revolution					
					}
					
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 8:		// read Comm fault error behavior
			if ((pkt_ret.pkt == 51) && (pkt_ret.stat ==  2))
			{
				if (can_open_rx.data[0] != 0x01)
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 9;
					timers[tenc_init] = 0;
					Pack_Service_Msg(52,0);			// read Device fault error behavior
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 9:		// // read Device fault error behavior
			if ((pkt_ret.pkt == 52) && (pkt_ret.stat ==  2))
			{
				if (can_open_rx.data[0] != 0x01)
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 10;
					timers[tenc_init] = 0;
					Pack_Service_Msg(82,0);			// read Termination Resistor					
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 10:		// read Termination Resistor
			if ((pkt_ret.pkt == 82) && (pkt_ret.stat ==  2))
			{
				if (can_open_rx.data[0] != 0x01)
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 11;
					timers[tenc_init] = 0;
					Pack_Service_Msg(80,0);			// read baud rate setting					
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 11:		// read baud rate
			if ((pkt_ret.pkt == 80) && (pkt_ret.stat ==  2))
			{
			    if (fvars[fvencbaud] == 0)
				  	t_baud = 5;
			    else if (fvars[fvencbaud] == 1)
			    	t_baud = 4;
			    else
			    	t_baud = 0xFF;
				if (can_open_rx.data[0] != t_baud)
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 12;
					timers[tenc_init] = 0;
					Pack_Service_Msg(61,0);			// read Measuring Steps per Revolution					
				}	
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 12:		// read Measuring Steps per Revolution
			if ((pkt_ret.pkt == 61) && (pkt_ret.stat ==  2))
			{
						
				read_enc_w.byte[lbw] = can_open_rx.data[0];
				read_enc_w.byte[hbw] = can_open_rx.data[1];
				can_enc_ppr = read_enc_w.word;
				if (can_enc_ppr != fvars[fvencppr]) 
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 13;
					timers[tenc_init] = 0;
					Pack_Service_Msg(62,0);			// read Total Measuring Range		
				}
			
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 13:		// read Total Measuring Range
			if ((pkt_ret.pkt == 62) && (pkt_ret.stat ==  2))
			{
				read_enc_dw.byte[lb] = can_open_rx.data[0];
				read_enc_dw.byte[mlb] = can_open_rx.data[1];
				read_enc_dw.byte[mhb] = can_open_rx.data[2];
				read_enc_dw.byte[hb] = can_open_rx.data[3];
				can_enc_range = read_enc_dw.dword;
				if (can_enc_range < 0x02000000)
					init_can_encoder = 1;
				else
				{
					enc_read_init_seq = 14;
					timers[tenc_init] = 0;
					Pack_Service_Msg(60,0);			// read Operating parameters
				}
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 14:		// read Operating parameters
			if ((pkt_ret.pkt == 60) && (pkt_ret.stat ==  2))
			{
				read_enc_w.byte[lbw] = can_open_rx.data[0];
				read_enc_w.byte[hbw] = can_open_rx.data[1];		
				enc_read_init_seq = 15;
				timers[tenc_init] = 0;
				
				Pack_Service_Msg(64,0);			// Read position count
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 15:		// receive poscnt
			if ((pkt_ret.pkt == 64) && (pkt_ret.stat ==  2))
			{
				read_enc_dw.byte[0] = can_open_rx.data[lb];
				read_enc_dw.byte[1] = can_open_rx.data[mlb];
				read_enc_dw.byte[2] = can_open_rx.data[mhb];
				read_enc_dw.byte[3] = can_open_rx.data[hb];
				can_enc_count = read_enc_dw.dword;
				DPP_Count = Enc_Base_Count + can_enc_count;
				prev_Enc_Count = DPP_Count;	
				Prev_DPP_Count = DPP_Count;
#if (Simulator == 1)
				sim_DPP_Count = DPP_Count;
#endif
				Clear_Vel_Array = 1;

				enc_read_init_seq = 0;
				timers[tenc_init]  = 0;
				can_enc_init = 1;
				Enc_Pack_Can(0x01,0);			// Start Node				
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		default:
			can_enc_init_fail = 1;
			break;
	}
}	

void init_encoder(void)
{
	switch (enc_init_seq)
	{
		case 0:		// start of init sequence 
			Enc_Pack_Can(0x80,0);		// put device in pre-operational mode
			enc_init_seq = 1;					
			break;
			
		case 1:						// Load Defaults
			timers[tenc_init] = 0;
			if ((can_enc_statusf & 0x01) != 0)
				enc_init_seq = 3;
			else
			{
				enc_init_seq = 2;
				Pack_Service_Msg(42,1);			// write Load Defaults
			}
			break;

		case 2:		// Verify the defaults were loaded
			if ((pkt_ret.pkt == 42) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 3;
				can_enc_statusf |= 0x01;
				Update_Enc_Status_Flag();
				timers[tenc_init] = 0;
//				Enc_Pack_Can(0x81,0);		// Reset Node
			}
			else if (timers[tenc_init] > 50)
				can_enc_init_fail = 1;
			break; 
			
		case 3: 	// write baud rate
			if (((cons[enctype] == 0) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 1))
			{							// only for Turck encoder
				enc_init_seq = 4;
				timers[tenc_init] = 0;
				Pack_Service_Msg(80,1);			// Write Baud rate
			}
			else
				enc_init_seq = 7;
			break;
						
		case 4:		// Verify Write Baud rate 
			if ((pkt_ret.pkt == 80) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 5;
				Pack_Service_Msg(82,1);			// Write Termination resistor
				timers[tenc_init] = 0;
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 5:		// Verify Write Termination resistor
			if ((pkt_ret.pkt == 82) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 6;
				timers[tenc_init] = 0;
				Pack_Service_Msg(85,1);			// Save baud rate
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 6:		// Verify save baud rate 
			if ((pkt_ret.pkt == 85) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 7;
				timers[tenc_init] = 0;
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		
			
		case 7:	// Write Transmit PD0-1 Type
			enc_init_seq = 8;
			timers[tenc_init] = 0;
			Pack_Service_Msg(12,1);			// write Transmit PDO-1 Type
			break; 
		
		case 8:		// Verify Write Transmit PDO-1 Type
			if ((pkt_ret.pkt == 12) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 9;
				timers[tenc_init] = 0;
				Pack_Service_Msg(15,1);			// write Transmit PDO-1 Event Timer
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
			
		case 9:		// Verify Write Command Transmit PDO-1 Event Timer
			if ((pkt_ret.pkt == 15) && (pkt_ret.stat ==  1))
			{
				if (((cons[enctype] == 1) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 2))
				{							// only for Dynapar encoder
					nmb_elements_mapped = 1;
					enc_init_seq = 16;
					timers[tenc_init] = 0;
					Pack_Service_Msg(60,1);			// Write Operating parameters
				}
				else
				{
					enc_init_seq = 10;
					timers[tenc_init] = 0;
					nmb_elements_mapped = 0;
					Pack_Service_Msg(16,1);			// Write Clear Mapping number of elements
				}
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;

		case 10:		// Verify Write Clear Mapping number of elements
			if ((pkt_ret.pkt == 16) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 11;
				timers[tenc_init] = 0;
				Pack_Service_Msg(17,1);			// Write Position Count Map
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
			
		case 11:		// Verify Write Position Count Map
			if ((pkt_ret.pkt == 17) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 12;
				timers[tenc_init] = 0;
				nmb_elements_mapped = 1;
				Pack_Service_Msg(16,1);			// Write Transmit PDO-1 Mapping Number of Elements
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
			
		case 12:		// Verify Write Transmit PDO-1 Mapping Number of Elements
			if ((pkt_ret.pkt == 16) && (pkt_ret.stat ==  1))
			{
				if (((cons[enctype] == 0) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 1))
				{							// only for Turck encoder
					enc_init_seq = 13;
					timers[tenc_init] = 0;
					Pack_Service_Msg(51,1);			// Write Comm fault error behavior
				}
				else 
				{
					enc_init_seq = 16;
					timers[tenc_init] = 0;
					Pack_Service_Msg(60,1);			// Write Operating parameters
				}
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
			
		case 13:		// Verify Write Comm fault error behavior
			if ((pkt_ret.pkt == 51) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 14;
				timers[tenc_init] = 0;
				Pack_Service_Msg(52,1);			// Write Device fault error behavior
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;

		case 14:		// Verify Write Device fault error behavior
			if ((pkt_ret.pkt == 52) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 15;
				timers[tenc_init] = 0;
				Pack_Service_Msg(53,1);			// Write Internal fault error behavior
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
			
		case 15:		// Verify Write Internal fault error behavior
			if ((pkt_ret.pkt == 53) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 16;
				timers[tenc_init] = 0;
				Pack_Service_Msg(60,1);			// Write Operating parameters
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 16:		// Verify Write Operating parameters
			if ((pkt_ret.pkt == 60) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 17;
				timers[tenc_init] = 0;
				Pack_Service_Msg(61,1);			// Write Measuring Steps per Revolution
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 17:		// Verify Write Measuring Steps per Revolution
			if ((pkt_ret.pkt == 61) && (pkt_ret.stat ==  1))
			{
				enc_init_seq = 18;
				timers[tenc_init] = 0;
//				Pack_Service_Msg(62,1);			// Write Total Measuring Range
			}
			else if (timers[tenc_init] > 10)
				can_enc_init_fail = 1;
			break;
		case 18:		// Verify Write Total Measuring Range
//			if ((pkt_ret.pkt == 62) && (pkt_ret.stat ==  1))
			{
				init_can_encoder = 0;
				can_enc_init_fail = 0;
				enc_init_seq = 0;
				timers[tenc_init] = 0;
				enc_read_init_seq = 0;
				SaveEncPar = 1;				// save setup data
				if (((cons[enctype] == 0) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 1))
					SaveEncBusPar = 1;			// save encoder bus parameters
			}
//			else if (timers[tenc_init] > 40)
//				can_enc_init_fail = 1;
			break;
		default:
			can_enc_init_fail = 1;
			break;
	}
	if (can_enc_init_fail == 1)
		init_can_encoder = 0;
}

//************************************************************
//*********  Check to see if the Transmit buffer is full *****
//************************************************************

int16 IsEncTxBufFul(void)
{

	_disable();
	if (ectpix == ectgix)
		enc_availbuf = c_enc_tx_buf_siz + 1;
	else if (ectpix > ectgix)
		enc_availbuf = (c_enc_tx_buf_siz - ectpix) + ectgix + 1;
	else 
		enc_availbuf = ectgix - ectpix;

	_enable();
	if (enc_availbuf > 2)
		return 0;
	else
		return 1;
}

//********************************************
// Look for data to change then transmit data
//********************************************


void Enc_Can_Control (void)
{
	unsigned char msg;
	
	if ((Enc_Cnt_Reset == 1) && (SaveEncPar == 0) && (timers[tenc_busy] < 30))
	{
		if (fvars[fvenctype] == 4)
			DPP_Count = 64.0 * 13.5 * (float)(cons[topf] + 1); 	// set count to top of hoistway
		else		
			DPP_Count = (((float)fvars[fvencrpm] * (float)fvars[fvencppr]) / ((float)cons[speed] * 10.0)) * 13.5 * (float)(cons[topf] + 1); 	// set count to top of hoistway
		init_encoder_count(DPP_Count);
		Enc_Cnt_Reset = 0;
	}
	
	Enc_Unpack_Can();
	
	enc_can_status = ENC_CAN.ESR.R;
	if (ENC_CAN.ESR.B.ERRINT == 1)
	{
		enc_can_status = ENC_CAN.ESR.R;
		ENC_CAN.ESR.B.ERRINT = 1;
	}
	else
		enc_can_status = 0;
		
	enc_can_tx_err_cnt = (uint8)ENC_CAN.ECR.B.TXECNT;
	enc_can_rx_err_cnt = (uint8)ENC_CAN.ECR.B.RXECNT;

	if (ENC_CAN.ESR.B.BOFFINT == 1)
	{		// The CAN bus is off
		ENC_CAN.ESR.B.BOFFINT = 1;
		enc_bus_off_cnt++;
		enc_can_error |= 0x04;
		enc_init_can_bus(0);
		ReInitEncFlg = 1;
		record_fault(f_enc_can_bus_off);
	}
	else if (enc_bus_off_cnt > 0)
		enc_bus_off_cnt--;

#if ((Simulator == 0) || (Sim_Motion == 0))
	if ((enc_can_status & 0x00000080) != 0)
	{		// Bus is idle
		enc_bus_idle_cnt++;
//		enc_can_error |= 0x08;
//		record_fault(f_enc_can_bus_idle);	
	}
	else if (enc_bus_idle_cnt > 0)
		enc_bus_idle_cnt--;
	
	if ((enc_can_status & 0x00002000) != 0)
	{		// Transmit acknowledge error
		enc_ack_error_cnt++;
		enc_can_error |= 0x10;
//		record_fault(f_enc_can_ack_err);	
	}
	else if (enc_ack_error_cnt > 0)
		enc_ack_error_cnt--;
	
#else   
	// simulator mode
	can_enc_init = 1;
	can_enc_init_fail = 0;
	init_can_encoder = 0;
	enc_init_seq = 0;
	enc_read_init_seq = 0;
	EncCan_Upd = 0;
	return;

#endif

	if (timers[tenc_busy] < 3)
		return;

	// Check to see if a tx buffer is available
	if (IsEncTxBufFul() == 0)
	{		// A buffer is available

		if(ReInitEncFlg == 1)
		{
			enc_init_can_bus(0);
			can_enc_com_err = 0;
			can_enc_init = 0;
			can_enc_init_fail = 0;
			init_can_encoder = 0;
			enc_init_seq = 0;
			enc_read_init_seq = 0;
			ReInitEncFlg = 0;
			timers[tenc_inifail] = 0;
		}

		msg = Enc_Get_Tx_Msg_Req();
		if (msg != 0)
		{
			Enc_Pack_Can(msg,0);				// Transmit back msg that was requested
		}
 		else if ((can_enc_init == 0) && (can_enc_init_fail == 0) && (init_can_encoder == 0))
		{
			read_encoder_init();
			timers[tenc_inifail] = 0;
		}
		else if (init_can_encoder == 1)
		{			// Initialize encoder
			init_encoder();
			timers[tenc_inifail] = 0;
		}	
		else if (can_enc_init_fail == 1)
		{		// delay then retry init
			if (timers[tenc_inifail] > 30)
			{				// try to re-init
				can_enc_init_baud_cnt++;
				if (can_enc_init_baud_cnt >= 3)
				{
					can_enc_init_baud_cnt = 0;
					if ((can_enc_baud == 0) || (can_enc_baud == 1))
						can_enc_baud = 2;		// change baud rate to 125K
					else if (can_enc_baud == 2)
						can_enc_baud = 1;
					if (((cons[enctype] == 2) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 3))   	// not for Wachendorff
						enc_init_can_bus(0);    		// don't switch the baud on the Wachendorff encoder because it auto detects the baud rate.
					else
						enc_init_can_bus(can_enc_baud);
				}
				can_enc_init = 0;
				can_enc_init_fail = 0;
				init_can_encoder = 0;
				enc_init_seq = 0;
				enc_read_init_seq = 0;
				timers[tenc_inifail] = 0;
				record_fault(f_encoder_init);
				can_enc_init_fail_cnt++;
				if (can_enc_init_fail_cnt > 10)
				{
					can_enc_init_fail_cnt = 0;
					if ((can_enc_statusf & 0x01) != 0)
					{
						can_enc_statusf &= ~0x01;
						Update_Enc_Status_Flag();
					}
				}
				
			}
		}	
		else if (can_enc_init == 1)
		{	
		
			if (Enc_Packet_Req != 0)
			{		// Updating Can Open Encoder
				EncCan_Upd = 1;
				if ((pkt_ret.pkt == Enc_Packet_Req) && ((pkt_ret.stat == 1) || (pkt_ret.stat == 1)))
					Enc_Packet_Req = 0;
				else if (((Enc_Packet_Req == 41) && (timers[tenc_pktreq] > 30)) || ((Enc_Packet_Req != 41) && (timers[tenc_pktreq] > 3)))
				{
					fault_device = Enc_Packet_Req;
					record_fault(f_enc_can_pkt_flt);
					Enc_Packet_Req = 0;
				}
			}
			else 
			{
				
				EncCan_Upd = 0;

				if (WriteEncCount == 1)
				{
					EncCan_Upd = 1;
					Pack_Service_Msg(63,1);
					WriteEncCount = 0;	
					timers[tenc_hb] = 0;
					Enc_Packet_Req = 63;
					timers[tenc_pktreq] = 0;
					if (((cons[enctype] == 2) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 3))
						SaveEncAppPar = 1;
					else
						SaveEncPar = 1;
					enc_cnt_wrt = 1;
				}
				else if (WriteEncPPR == 1)
				{
					EncCan_Upd = 1;
					Pack_Service_Msg(61,1);
					WriteEncPPR = 0;	
					timers[tenc_hb] = 0;
					Enc_Packet_Req = 61;
					timers[tenc_pktreq] = 0;
					Enc_Cnt_Reset = 1;
					SaveEncPar = 1;
				}
				else if (ReadEncPPR == 1)
				{
					EncCan_Upd = 1;
					Pack_Service_Msg(61,0);
					ReadEncPPR = 0;	
					timers[tenc_hb] = 0;
					Enc_Packet_Req = 61;
					timers[tenc_pktreq] = 0;
				}
				else if (WriteEncDir == 1)
				{
					EncCan_Upd = 1;
					Pack_Service_Msg(60,1);
					WriteEncDir = 0;	
					timers[tenc_hb] = 0;
					Enc_Packet_Req = 60;
					timers[tenc_pktreq] = 0;
					Enc_Cnt_Reset = 1;
					SaveEncPar = 1;
				}
				else if (WriteEncMaxPulses == 1)
				{
					EncCan_Upd = 1;
					Pack_Service_Msg(62,1);
					WriteEncMaxPulses = 0;	
					timers[tenc_hb] = 0;
					Enc_Packet_Req = 62;
					timers[tenc_pktreq] = 0;
					SaveEncPar = 1;
				}
				else if (SaveEncAppPar == 1)
				{
					EncCan_Upd = 1;
					Pack_Service_Msg(43,1);
					SaveEncAppPar = 0;	
					timers[tenc_hb] = 0;
					Enc_Packet_Req = 43;
					timers[tenc_pktreq] = 0;
					timers[tenc_busy] = 0;
					enc_app_par_sav = 1;
				}
				else if (SaveEncBusPar == 1)
				{
					EncCan_Upd = 1;
					Pack_Service_Msg(85,1);
					SaveEncBusPar = 0;	
					timers[tenc_hb] = 0;
					Enc_Packet_Req = 85;
					timers[tenc_pktreq] = 0;
					timers[tenc_busy] = 0;
				}
				else if (SaveEncPar == 1)
				{
					EncCan_Upd = 1;
					Pack_Service_Msg(41,1);
					SaveEncPar = 0;	
					timers[tenc_hb] = 0;
					Enc_Packet_Req = 41;
					timers[tenc_pktreq] = 0;
					timers[tenc_busy] = 0;
					enc_par_sav = 1;
				}
				if (timers[tenc_hb] > 20)
				{		// send heart beat
					timers[tenc_hb] = 0;
//					Enc_Pack_Can(0x18,1);
					if (can_enc_online_cnt > 0)
					{
						can_enc_com_err = 0;
						can_enc_online = 1;
						statusf2 &= ~sf_ENCCAN;		// Encoder can comm error
						can_enc_online_cnt--;
					}
					else
					{
						if (can_enc_com_err == 0)
							record_fault(f_encoder_com_err);
						can_enc_com_err = 1;
						statusf2 |= sf_ENCCAN;		// Encoder can comm error
						can_enc_online = 0;
						ReInitEncFlg = 1;
					}
				}
			}
		}		
	}

}


//********************************************
//*********  Pack Can data   *****************
//********************************************

void Enc_Pack_Can (unsigned char msg,unsigned char tx_req)
{
	COMMWORD_TYPE packt;
	bool valid_msg = 0;

	_disable();
	if (ectpix < c_enc_tx_buf_siz)
		ectpix++;
	else
		ectpix = 0;
	_enable();

	if (tx_req == 1)
	{
		if ((msg == 0x18) || (msg == 0x28) || (msg == 0x38))
		{		// msg 180, 280 or 380 + node id
			enc_can_tx[ectpix].len = 0;
			enc_can_tx[ectpix].ex_id = 0;		// No extended frame
			enc_can_tx[ectpix].rtr = 1;			// remote transmit
			enc_can_tx[ectpix].srr = 0;			// substitute remote request (extended frame only)
			enc_can_tx[ectpix].msg = msg;
			enc_can_tx[ectpix].addr = encoder_node_id;	
			enc_can_tx[ectpix].data[0] = 0;
			enc_can_tx[ectpix].data[1] = 0;
			enc_can_tx[ectpix].data[2] = 0;
			enc_can_tx[ectpix].data[3] = 0;
			enc_can_tx[ectpix].data[4] = 0;
			enc_can_tx[ectpix].data[5] = 0;
			enc_can_tx[ectpix].data[6] = 0;
			enc_can_tx[ectpix].data[7] = 0;
			valid_msg = 1;
		}
	}
	else switch (msg)
	{
		case 0x01:			// Start Node
			enc_can_tx[ectpix].len = 2;
			enc_can_tx[ectpix].ex_id = 0;		// No extended frame
			enc_can_tx[ectpix].rtr = 0;			// no remote transmit
			enc_can_tx[ectpix].srr = 0;			// substitute remote request (extended frame only)
			enc_can_tx[ectpix].msg = 0;
			enc_can_tx[ectpix].addr = 0;	
			enc_can_tx[ectpix].data[0] = 0x01;
			enc_can_tx[ectpix].data[1] = encoder_node_id;
			enc_can_tx[ectpix].data[2] = 0;
			enc_can_tx[ectpix].data[3] = 0;
			enc_can_tx[ectpix].data[4] = 0;
			enc_can_tx[ectpix].data[5] = 0;
			enc_can_tx[ectpix].data[6] = 0;
			enc_can_tx[ectpix].data[7] = 0;
			valid_msg = 1;
			break;
		case 0x02:			// Stop Node
			enc_can_tx[ectpix].len = 2;
			enc_can_tx[ectpix].ex_id = 0;		// No extended frame
			enc_can_tx[ectpix].rtr = 0;			// no remote transmit
			enc_can_tx[ectpix].srr = 0;			// substitute remote request (extended frame only)
			enc_can_tx[ectpix].msg = 0;
			enc_can_tx[ectpix].addr = 0;	
			enc_can_tx[ectpix].data[0] = 0x02;
			enc_can_tx[ectpix].data[1] = encoder_node_id;
			enc_can_tx[ectpix].data[2] = 0;
			enc_can_tx[ectpix].data[3] = 0;
			enc_can_tx[ectpix].data[4] = 0;
			enc_can_tx[ectpix].data[5] = 0;
			enc_can_tx[ectpix].data[6] = 0;
			enc_can_tx[ectpix].data[7] = 0;
			valid_msg = 1;
			break;
			
		case 0x60:			// (96) Transmit can message to encoder address 0x600 + node id
			enc_can_tx[ectpix].len = can_open_tx.len;
			enc_can_tx[ectpix].ex_id = 0;		// No extended frame
			enc_can_tx[ectpix].rtr = 0;			// no remote transmit
			enc_can_tx[ectpix].srr = 0;			// substitute remote request (extended frame only)
			enc_can_tx[ectpix].msg = msg;
			enc_can_tx[ectpix].addr = can_open_tx.node_id;	
			enc_can_tx[ectpix].data[0] = can_open_tx.cmd;
			packt.word = can_open_tx.index;
			enc_can_tx[ectpix].data[1] = packt.byte[lbw];
			enc_can_tx[ectpix].data[2] = packt.byte[hbw];
			enc_can_tx[ectpix].data[3] = can_open_tx.sub_index;
			enc_can_tx[ectpix].data[4] = can_open_tx.data[0];
			enc_can_tx[ectpix].data[5] = can_open_tx.data[1];
			enc_can_tx[ectpix].data[6] = can_open_tx.data[2];
			enc_can_tx[ectpix].data[7] = can_open_tx.data[3];
			valid_msg = 1;
			break;

		case 0x80:			// (128) Pre-operational mode
			enc_can_tx[ectpix].len = 2;
			enc_can_tx[ectpix].ex_id = 0;		// No extended frame
			enc_can_tx[ectpix].rtr = 0;			// no remote transmit
			enc_can_tx[ectpix].srr = 0;			// substitute remote request (extended frame only)
			enc_can_tx[ectpix].msg = 0;
			enc_can_tx[ectpix].addr = 0;	
			enc_can_tx[ectpix].data[0] = 0x80;
			enc_can_tx[ectpix].data[1] = encoder_node_id;
			enc_can_tx[ectpix].data[2] = 0;
			enc_can_tx[ectpix].data[3] = 0;
			enc_can_tx[ectpix].data[4] = 0;
			enc_can_tx[ectpix].data[5] = 0;
			enc_can_tx[ectpix].data[6] = 0;
			enc_can_tx[ectpix].data[7] = 0;
			valid_msg = 1;
			break;
			
		case 0x81:			// (129) Reset Node
			enc_can_tx[ectpix].len = 2;
			enc_can_tx[ectpix].ex_id = 0;		// No extended frame
			enc_can_tx[ectpix].rtr = 0;			// no remote transmit
			enc_can_tx[ectpix].srr = 0;			// substitute remote request (extended frame only)
			enc_can_tx[ectpix].msg = 0;
			enc_can_tx[ectpix].addr = 0;	
			enc_can_tx[ectpix].data[0] = 0x81;
			enc_can_tx[ectpix].data[1] = encoder_node_id;
			enc_can_tx[ectpix].data[2] = 0;
			enc_can_tx[ectpix].data[3] = 0;
			enc_can_tx[ectpix].data[4] = 0;
			enc_can_tx[ectpix].data[5] = 0;
			enc_can_tx[ectpix].data[6] = 0;
			enc_can_tx[ectpix].data[7] = 0;
			valid_msg = 1;
			break;
					
		case 0x82:			// (130) Reset Communications
			enc_can_tx[ectpix].len = 2;
			enc_can_tx[ectpix].ex_id = 0;		// No extended frame
			enc_can_tx[ectpix].rtr = 0;			// no remote transmit
			enc_can_tx[ectpix].srr = 0;			// substitute remote request (extended frame only)
			enc_can_tx[ectpix].msg = 0;
			enc_can_tx[ectpix].addr = 0;	
			enc_can_tx[ectpix].data[0] = 0x82;
			enc_can_tx[ectpix].data[1] = encoder_node_id;
			enc_can_tx[ectpix].data[2] = 0;
			enc_can_tx[ectpix].data[3] = 0;
			enc_can_tx[ectpix].data[4] = 0;
			enc_can_tx[ectpix].data[5] = 0;
			enc_can_tx[ectpix].data[6] = 0;
			enc_can_tx[ectpix].data[7] = 0;
			valid_msg = 1;
			break;
					

		default:
			enc_can_error = e_inv_pack;
			enc_can_error_data = msg;
			break;
	}
	
	if (valid_msg == 1)
	{
		if (enc_can_tx[ectpix].len > 8)
		{
			enc_can_tx[ectpix].len = 8;
			enc_can_error |= 0x40;
		}

		uiu.b[hb] = (uint8)((enc_can_tx[ectpix].msg >> 3) & 0x0F);		// Message
		uiu.b[mhb] = (uint8)((enc_can_tx[ectpix].addr & 0x7F) << 1);	// Address
		uiu.b[mlb] = 0;
		uiu.b[lb] = 0;
	 	enc_can_tx[ectpix].id = uiu.dw << 1;		// priority bits get set to zero on shift
		Tx_Enc_Can_Msg(8);
	}

}

//************************************************************
//*********  Transmit Can Message  ***************************
//************************************************************

void Tx_Enc_Can_Msg(int16 TxBufIx)
{
	if (ectgix != ectpix)
	{
		
		switch (TxBufIx)
		{
			
			case 8:
				if ((ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
				{		// The transmitter is not busy
					enc_transmit(TxBufIx);
					ENC_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
					break;
				}
				else
				{
					enc_tx_B08_fail_cnt++;
					if (enc_tx_B08_fail_cnt > 5)
					{
						enc_tx_B08_fail_cnt = 0;
						ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
					 
			case 9:
				if ((ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
				{		// The transmitter is not busy
					enc_transmit(TxBufIx);
					ENC_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
					break;
				}
				else
				{
					enc_tx_B09_fail_cnt++;
					if (enc_tx_B09_fail_cnt > 5)
					{
						enc_tx_B09_fail_cnt = 0;
						ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			case 10:
				if ((ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
				{		// The transmitter is not busy
					enc_transmit(TxBufIx);
					ENC_CAN.IFLAG1.B.BUF10I = 1;
					break;
				}
				else
				{
					enc_tx_B10_fail_cnt++;
					if (enc_tx_B10_fail_cnt > 5)
					{
						enc_tx_B10_fail_cnt = 0;
						ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			case 11:
				if ((ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00))
				{		// The transmitter is not busy
					enc_transmit(TxBufIx);
					ENC_CAN.IFLAG1.B.BUF11I = 1;
					break;
				}
				else
				{
					enc_tx_B11_fail_cnt++;
					if (enc_tx_B11_fail_cnt > 5)
					{
						enc_tx_B11_fail_cnt = 0;
						ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
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

void enc_transmit (int16 TxBufIx)
{
	int16 i;
	if (ectgix != ectpix)
	{		// There is data in the buffer 
 
		if (ectgix < c_enc_tx_buf_siz)
			ectgix++;
		else
			ectgix = 0;
		
				
	 	ENC_CAN.MB[TxBufIx].MSG.MID.R = enc_can_tx[ectgix].id;	

		i = 0;
		while ((i<enc_can_tx[ectgix].len) && (i < 8))
		{
		 	ENC_CAN.MB[TxBufIx].MSG.MDATA.B[i++] = enc_can_tx[ectgix].data[i];
		}
		

		ENC_CAN.MB[TxBufIx].MSG.MCS.B.LENGTH = enc_can_tx[ectgix].len;
		 	
		ENC_CAN.MB[TxBufIx].MSG.MCS.B.RTR = enc_can_tx[ectgix].rtr;
	 	
		ENC_CAN.MB[TxBufIx].MSG.MCS.B.IDE = enc_can_tx[ectgix].ex_id;

		ENC_CAN.MB[TxBufIx].MSG.MCS.B.SRR = enc_can_tx[ectgix].srr;
		 	
		 	
		ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x0C;		// Initiate transmit
		enc_can_debug_1 = ENC_CAN.MB[TxBufIx].MSG.MCS.B.CODE;
		can_enc_tx_cnt++;
	}
}

//*****************************************
// Unpack the can message
//*****************************************

void Enc_Unpack_Can (void)
{
	COMMWORD_TYPE rcv_word;
	COMMDWORD_TYPE rcv_dword;
	int16 loop_cnt = 0;
	uint8 msg,addr;
	
	while ((loop_cnt <= 4)  && (ecrgix != ecrpix))
	{
		if (enc_can_rx[ecrgix].valid_msg == 1)
		{		
		
			if (enc_can_rx[ecrpix].ex_id == 1)		// Extended frame
			{
				uiu.dw = ((enc_can_rx[ecrgix].id & 0x1FFC0000) << 2) ;
				uiu_2.dw = ((enc_can_rx[ecrgix].id & 0x1FFC0000) >> 2);
				enc_can_rx[ecrgix].msg = uiu.b[hb] & 0x78;	// Message
				enc_can_rx[ecrgix].addr = uiu_2.b[mhb] & 0x7F;    // Address
			}
			else
			{
				uiu.dw = ((enc_can_rx[ecrgix].id & 0x1FFC0000) << 2) ;
				uiu_2.dw = ((enc_can_rx[ecrgix].id & 0x1FFC0000) >> 2);
				enc_can_rx[ecrgix].msg = uiu.b[hb] & 0x78;	// Message
				enc_can_rx[ecrgix].addr = uiu_2.b[mhb] & 0x7F;    // Address
			}
			
			msg = enc_can_rx[ecrgix].msg;
			addr = enc_can_rx[ecrgix].addr;
			
			if (enc_can_rx[ecrgix].rtr == 1)
			{
				Enc_Put_Tx_Msg_Req(enc_can_rx[ecrgix].msg);	
				can_enc_rx_cnt++;
				can_enc_online_cnt = 5;
			}
			else switch (enc_can_rx[ecrgix].msg)
			{
				case 0x18:			// Message 0x180: Encoder process data for position
					if (enc_can_rx[ecrgix].addr == encoder_node_id)
					{
						rcv_dword.byte[lb] = enc_can_rx[ecrgix].data[0];
						rcv_dword.byte[mlb] = enc_can_rx[ecrgix].data[1];
						rcv_dword.byte[mhb] = enc_can_rx[ecrgix].data[2];
						rcv_dword.byte[hb] = enc_can_rx[ecrgix].data[3];
						can_enc_count = rcv_dword.dword;

						
						rcv_word.byte[lbw] = enc_can_rx[ecrgix].data[4];
						rcv_word.byte[hbw] = enc_can_rx[ecrgix].data[5];
						can_enc_speed = rcv_word.word;
						
						rcv_word.byte[lbw] = enc_can_rx[ecrgix].data[6];
						rcv_word.byte[hbw] = enc_can_rx[ecrgix].data[7];
						can_enc_accel = rcv_word.word;
						can_enc_rx_cnt++;
						can_enc_online_cnt = 5;
					}
					break;		
				case 0x58:			// Message 0x580: Encoder Service data
					if (enc_can_rx[ecrgix].addr == encoder_node_id)
					{
						can_open_rx.msg = enc_can_rx[ecrgix].msg;
						can_open_rx.node_id = enc_can_rx[ecrgix].addr;
						can_open_rx.len = enc_can_rx[ecrgix].len;
						can_open_rx.cmd = enc_can_rx[ecrgix].data[0];
						
						rcv_word.byte[lbw] = enc_can_rx[ecrgix].data[1];
						rcv_word.byte[hbw] = enc_can_rx[ecrgix].data[2];
						can_open_rx.index = rcv_word.word;
						
						can_open_rx.sub_index = enc_can_rx[ecrgix].data[3];

						can_open_rx.data[0] = enc_can_rx[ecrgix].data[4];
						can_open_rx.data[1] = enc_can_rx[ecrgix].data[5];
						can_open_rx.data[2] = enc_can_rx[ecrgix].data[6];
						can_open_rx.data[3] = enc_can_rx[ecrgix].data[7];

						can_enc_rx_cnt++;
						Unpack_Service_Msg();		
						can_enc_online_cnt = 5;
					}
					break;	
				case 0x78:			// Boot message from encoder
					if (enc_can_rx[ecrgix].addr == encoder_node_id)
					{
						can_open_rx.msg = enc_can_rx[ecrgix].msg;
						can_open_rx.node_id = enc_can_rx[ecrgix].addr;
						can_open_rx.cmd = 0;
						can_open_rx.index = 0;
						can_open_rx.sub_index = 0;
						can_open_rx.data[0] = 0;
						can_open_rx.data[1] = 0;
						can_open_rx.data[2] = 0;
						can_open_rx.data[3] = 0;
						can_enc_rx_cnt++;
						can_enc_online_cnt = 5;
					}
					break;
				default:
					if (enc_can_rx[ecrgix].msg != 0) 
					{
						enc_can_error = e_inv_unpk;
						enc_can_error_data = enc_can_rx[ecrgix].msg;
					}
					break;
			}
			enc_can_rx[ecrgix].valid_msg = 0;
		}
		ecrgix++;
		if (ecrgix >= c_enc_rx_buf_siz)
			ecrgix = 0; 
		loop_cnt++;
	}
}

// ******************************************************
// **********  Transmit Msg Request Put Queue ***********
// ******************************************************

unsigned char Enc_Put_Tx_Msg_Req (unsigned char msg)
{
	unsigned char n;
	for(n=0;n<=4;n++)
	{
		if (enc_can_tx_msg_req[n] == 0)
		{
			enc_can_tx_msg_req[n] = msg;
			return 1;
		}
	}
	return 0;
}
// ******************************************************
// **********  Transmit Msg Request Get Queue ***********
// ******************************************************

unsigned char Enc_Get_Tx_Msg_Req (void)
{
	unsigned char n;
	unsigned char msg;

	msg = 0;
	for(n=0;n<=4;n++)
	{
		if (enc_can_tx_msg_req[n] != 0)
		{
			msg = enc_can_tx_msg_req[n];
			enc_can_tx_msg_req[n] = 0;
			return msg;
		}
	}
	return 0;
}



//**************************************************
//*** Pack Service Message
//**************************************************

/*
		The host sends:
		Byte 0 CMD 
				CMD = 0x23: Sending 4 bytes in bytes 4-7
					= 0x2B: Sending 2 bytes in bytes 4,5
					= 0x2F: Sending 1 byte in byte 4
					= 0x40: Request to receive data (bytes 4-7 not used)
		Byte 1 Index LSByte  Index = 16 bit address of the data
		Byte 2 Index MSByte
		Byte 3 Sub Index     Sub Index = 8 bit index from the address  
		Byte 4 Data LSB
		Byte 5 Data 
		Byte 6 Data
		Byte 7 Data MSB
	

	10		ENC			CPU						Message #10: Transmit Communications Parameters
																WRITE			READ
													CMD 		2F	1 byte		40
													INDEX LSB	00				00	
													INDEX MSB	18				18
													SUB INDEX	00				00
													DATA		05				--

													
	12		ENC			CPU						Message #12: Transmit PDO-1 Transmit Type
																WRITE			READ
													CMD  		2F	1 bytes		40	
													INDEX LSB	00				00
													INDEX MSB	18				18	
													SUB INDEX	02				02
													DATA		255				--		(254 for Dynapar and Wachendorff)
													
	15		ENC			CPU						Message #15: Transmit PDO-1 Event Timer (msec)
																WRITE			READ
													CMD  		2B	2 bytes		40
													INDEX LSB	00				00
													INDEX MSB	18				18
													SUB INDEX	05				05	
													DATA LSB	05	5 msec		--
													DATA MSB	00				--	
													
													(Dynapar only)
																WRITE			READ
													CMD  		2B	2 bytes		40
													INDEX LSB	00				00
													INDEX MSB	62				62
													SUB INDEX	00				00	
													DATA LSB	03	3 msec		--
													DATA MSB	00				--	

	16 		ENC 		CPU						Message #16: PDO-1 Mapping (Number of elements)
																WRITE			READ
													CMD 		2f	1 byte		40
													INDEX LSB	00				00	
													INDEX MSB	1A				1A
													SUB INDEX	00				00
													DATA LSB	03				--
																										
	17 		ENC 		CPU						Message #17: PDO-1 Mapping (position)
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX LSB	00				00
													INDEX MSB	1A				1A
													SUB INDEX	01				01
													DATA 4		20				--
													DATA 5		00				--
													DATA 6		04				--
													DATA 7		60				--
																										
	18 		ENC 		CPU						Message #18: PDO-1 Mapping (speed)
																WRITE			READ
													CMD  		23	4 bytes		40
													INDEX LSB	00				00
													INDEX MSB	1A				1A
													SUB INDEX	02				02
													DATA 4		10				--
													DATA 5		01				--
													DATA 6		30				--
													DATA 7		60				--
																										
	19 		ENC 		CPU						Message #19: PDO-1 Mapping (acceleration)
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX LSB	00				00
													INDEX MSB	1A				1A
													SUB INDEX	03				03
													DATA 4		10				--	
													DATA 5		01				--
													DATA 6		40				--
													DATA 7		60				--
														
	20		ENC			CPU						Message #20: Transmit PDO-2 Largest supported sub index
																WRITE			READ
													CMD 		2F	1 byte		40
													INDEX LSB	01				01
													INDEX MSB	18				18
													SUB INDEX	00				00
													DATA		05				--

													
	22		ENC			CPU						Message #22: Transmit PDO-2 Transmit Type
																WRITE			READ
													CMD 		2F	1 bytes		40
													INDEX LSB	01				01
													INDEX MSB	18				18
													SUB INDEX	02				02
													DATA		255				--
													
	25		ENC			CPU						Message #25: Transmit PDO-2 Event Timer (msec)
																WRITE			READ
													CMD 		2B	2 bytes		40
													INDEX LSB	01				01
													INDEX MSB	18				18
													SUB INDEX	05				05
													DATA LSB	05	5 msec		--
													DATA MSB	00				--

	26 		ENC 		CPU						Message #26: PDO-2 Mapping (Number of elements)
																WRITE			READ
													CMD 		2f	1 byte		40
													INDEX LSB	01				01
													INDEX MSB	1A				1A
													SUB INDEX	00				00
													DATA LSB	03				--
																										
	27 		ENC 		CPU						Message #27: PDO-2 Mapping (position)
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX LSB	01				01
													INDEX MSB	1A				1A
													SUB INDEX	01				01
													DATA 4		20				--
													DATA 5		00				--
													DATA 6		04				--
													DATA 7		60				--
																										
	28 		ENC 		CPU						Message #28: PDO-2 Mapping (speed)
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX LSB	01				01
													INDEX MSB	1A				1A
													SUB INDEX	02				02
													DATA 4		10				--	
													DATA 5		01				--
													DATA 6		30				--
													DATA 7		60				--
																										
	29 		ENC 		CPU						Message #29: PDO-2 Mapping (acceleration)
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX LSB	01				01
													INDEX MSB	1A				1A
													SUB INDEX	03				03
													DATA 4		10				--
													DATA 5		01				--
													DATA 6		40				--
													DATA 7		60				--
													
	30		ENC			CPU						Message #30: Transmit PDO-3 Largest supported sub index
																WRITE			READ
													CMD 		2F	1 byte		40
													INDEX LSB	02				02
													INDEX MSB	18				18
													SUB INDEX	00				00	
													DATA		05				--

													
	32		ENC			CPU						Message #32: Transmit PDO-3 Transmit Type
																WRITE			READ
													CMD			2F	1 bytes		40
													INDEX LSB	02				02
													INDEX MSB	18				18
													SUB INDEX	02				02
													DATA		255				--
													
	35		ENC			CPU						Message #35: Transmit PDO-3 Event Timer (msec)
																WRITE			READ
													CMD 		2B	2 bytes		40
													INDEX LSB	02				02
													INDEX MSB	18				18
													SUB INDEX	05				05
													DATA LSB	05	5 msec		--
													DATA MSB	00				--

	36 		ENC 		CPU						Message #36: PDO-3 Mapping (Number of elements)
																WRITE			READ
													CMD 		2f	1 byte		40
													INDEX LSB	02				02
													INDEX MSB	1A				1A
													SUB INDEX	00				00
													DATA LSB	03				--
																										
	37 		ENC 		CPU						Message #37: PDO-3 Mapping (position)
																WRITE			READ
													CMD			23	4 bytes		40
													INDEX LSB	02				02
													INDEX MSB	1A				1A
													SUB INDEX	01				01
													DATA 4		20				--
													DATA 5		00				--
													DATA 6		04				--	
													DATA 7		60				--
																										
	38 		ENC 		CPU						Message #38: PDO-3 Mapping (speed)
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX LSB	02				02
													INDEX MSB	1A				1A
													SUB INDEX	02				02
													DATA 4		10				--	
													DATA 5		01				--
													DATA 6		30				--
													DATA 7		60				--
																										
	39 		ENC 		CPU						Message #39: PDO-3 Mapping (acceleration)
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX LSB	02				02
													INDEX MSB	1A				1A
													SUB INDEX	03				03
													DATA 4		10				--
													DATA 5		01				--
													DATA 6		40				--
													DATA 7		60				--
													
	41		ENC			CPU						Message #41:	Save All Parameters
																		WRITE			
													DATA 0	CMD			23	4 bytes
													DATA 1	INDEX LSB	10	1010h
													DATA 2	INDES MSB	10
													DATA 3	UB INDEX 	01
													DATA 4				's'
													DATA 5				'a'
													DATA 6				'v'
													DATA 7				'e'
	
	42		ENC			CPU						Message #42:	Load Default values
																		WRITE		
													DATA 0	CMD 		23	4 bytes
													DATA 1	INDEX LSB 	11	1011h
													DATA 2  INDEX MSB	10
													DATA 3 	SUB INDEX 	01
													DATA 4				'l'
													DATA 5				'o'
													DATA 6				'a'
													DATA 7				'd'
													
	43		ENC			CPU						Message #43:	Save Application Parameters
																		WRITE			
													DATA 0	CMD			23	4 bytes
													DATA 1	INDEX LSB	10	1010h
													DATA 2	INDES MSB	10
													DATA 3	UB INDEX 	03
													DATA 4				's'
													DATA 5				'a'
													DATA 6				'v'
													DATA 7				'e'
	
	42		ENC			CPU						Message #44:	Load Application Default values
																		WRITE		
													DATA 0	CMD 		23	4 bytes
													DATA 1	INDEX LSB 	11	1011h
													DATA 2  INDEX MSB	10
													DATA 3 	SUB INDEX 	03
													DATA 4				'l'
													DATA 5				'o'
													DATA 6				'a'
													DATA 7				'd'
													

	50		ENC			CPU						Message #50:	Heartbeat Time
																		WRITE		
													DATA 0	CMD 		2B	2 bytes
													DATA 1	INDEX LSB 	17	1017h
													DATA 2  INDEX MSB	10
													DATA 3 	SUB INDEX 	01
													DATA 4				dd		DD = 500msec
													DATA 5				dd
													DATA 6				--
													DATA 7				--
	
	51		ENC			CPU						Message #51:	Comm fault error behavior (heartbeat failure)
																		WRITE		
													DATA 0	CMD 		2F	1 byte
													DATA 1	INDEX LSB 	29	1029h
													DATA 2  INDEX MSB	10
													DATA 3 	SUB INDEX 	01
													DATA 4				1		NO CHANGE
													DATA 5				--
													DATA 6				--
													DATA 7				--
													
	52		ENC			CPU						Message #52:	Device error behavior
																		WRITE		
													DATA 0	CMD 		2F	1 byte
													DATA 1	INDEX LSB 	29	1029h
													DATA 2  INDEX MSB	10
													DATA 3 	SUB INDEX 	02
													DATA 4				1		NO CHANGE
													DATA 5				--
													DATA 6				--
													DATA 7				--
													
	53		ENC			CPU						Message #53:	Internal error behavior
																		WRITE		
													DATA 0	CMD 		2F	1 byte
													DATA 1	INDEX LSB 	29	1029h
													DATA 2  INDEX MSB	10
													DATA 3 	SUB INDEX 	03
													DATA 4				1		NO CHANGE
													DATA 5				--
													DATA 6				--
													DATA 7				--
	
	60		ENC 		CPU						Message #60: Set Operating parameters
																WRITE			READ
													CMD  		2B	2 bytes		40
													INDEX 		6000			6000
													SUB INDEX	0				0
													DATA LSB	Bit 0: Direction 0=CW, 1=CCW
																Bit 1: Commissioning Diag Control 1=Enbable
																Bit 2: Scaling 1=Enbaled

													DATA MSB	Bit 13: Speed Format 0=RPM, 1=Units/sec
																Bit 14: Startup auto in Op-Mode 1=Enable
																Bit 15: Event mode position 1=Enable
									
	61		ENC		CPU							Message #61: Measuring Steps per revolution
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX		6001			6001
													SUB INDEX	0				0
													DATA LSB	FF				--		FFFFH = 65535
													DATA mLSB	FF				--
													DATA mMSB	00				--
													DATA MSB	00				--
													
	62		ENC		CPU							Message #62: Total Measuring Range
																WRITE			READ
													CMD 		23	4 bytes		40			
													INDEX		6002			6002
													SUB INDEX	0				0
													DATA LSB	00				--		0FFFF000H = 268431360
													DATA mLSB	F0				--
													DATA mMSB	FF				--
													DATA MSB	0F				--
													
	63		ENC		CPU							Message #63: Preset Position
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX		6003			6003
													SUB INDEX	0				0
													DATA LSB	xx		poscnt	--
													DATA mLSB	xx				--
													DATA mMSB	xx				--
													DATA MSB	xx				--
													
	64		ENC		CPU							Message #64: Read Position
																READ
													CMD  		40	
													INDEX		6004
													SUB INDEX	0
													
	65      ENC    CPU                          Message #65  Total Number of Revolution												
																WRITE			READ
													CMD  		2B	2 bytes		40
													INDEX 		6502			6502
													SUB INDEX	0				0
													DATA LSB
													DATA MSB	

													
	80		ENC		CPU							Message #80: Baud Rate
																WRITE			READ
													CMD 		2F	1 byte		40
													INDEX		2100			2100
													SUB INDEX	0				0
													DATA 		FF		USE JUMPER 250 KBits/s
											
	81		ENC		CPU							Message #81: Node Address
																WRITE			READ
													CMD 		2F	1 byte		40
													INDEX		2101			2101
													SUB INDEX	0				0
													DATA 		FF		USE JUMPER 63
											
	82		ENC		CPU							Message #82: Termination Resistor
																WRITE			READ
													CMD  		2F	1 byte		40		
													INDEX		2102			2102
													SUB INDEX	0				0
													DATA 		00		USE JUMPER TO HAVE TERMINATION
											
	85		ENC		CPU							Message #85:	Save All bus parameters
																WRITE			
													CMD = 23	4 bytes
													INDEX 		2105h
													SUB INDEX 	00
													DATA 4		's'
													DATA 5		'a'
													DATA 6		'v'
													DATA 7		'e'
													
	86		ENC		CPU							Message #86:	Store Init Code
																WRITE			READ
													CMD 		23	4 bytes		40
													INDEX		2140h			2140
													SUB INDEX	0				0
													DATA 4		'I'				--
													DATA 5		'n'				--	
													DATA 6		'i'				--
													DATA 7		't'				--
													
																										
*/
void Pack_Service_Msg (int16 msg, int16 read_write)
{							// read_write: 0 = read, 1 = write
	COMMWORD_TYPE packw;
	COMMDWORD_TYPE packdw;
				
	unsigned char valid_msg = 0;
	
	if (read_write == 0)
	{		// read command
		can_open_tx.len = 8;
		can_open_tx.cmd = 0x40;
		valid_msg = 1;
	}
	
	can_open_tx.data[0] = 0;			
	can_open_tx.data[1] = 0;
	can_open_tx.data[2] = 0;
	can_open_tx.data[3] = 0;
	
	switch(msg)
	{
		case 10:		// Message #10: Transmit PDO-1 Largest supported sub index
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1800;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 5;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 12:		// Message #12: Transmit PDO-1 Transmit Typex
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1800;
			can_open_tx.sub_index = 0x02;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				if (((cons[enctype] == 1) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 2))
					can_open_tx.data[0] = 254;
				else
					can_open_tx.data[0] = 255;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 15:		// Message #15: Transmit PDO-1 Event Timer (msec)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			if (((cons[enctype] == 1) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 2))
			{
				can_open_tx.index = 0x6200;
				can_open_tx.sub_index = 0x00;
			}
			else
			{
				can_open_tx.index = 0x1800;
				can_open_tx.sub_index = 0x05;
			}
			
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2B;
				can_open_tx.data[0] = EVENT_TIMER;			// 3 msec
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 16:		// Message #16: PDO-1 Mapping (Number of elements)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A00;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = nmb_elements_mapped;			
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 17:		// Message #17: PDO-1 Mapping (position)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A00;
			can_open_tx.sub_index = 0x01;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x20;			
				can_open_tx.data[1] = 0x00;
				can_open_tx.data[2] = 0x04;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 18:		// Message #18: PDO-1 Mapping (speed)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A00;
			can_open_tx.sub_index = 0x02;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x10;			
				can_open_tx.data[1] = 0x01;
				can_open_tx.data[2] = 0x30;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 19:		// Message #19: PDO-1 Mapping (acceleration)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A00;
			can_open_tx.sub_index = 0x03;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x10;			
				can_open_tx.data[1] = 0x01;
				can_open_tx.data[2] = 0x40;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 20:		// Message #20: Transmit PDO-2 Largest supported sub index
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1801;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 5;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 22:		// Message #22: Transmit PDO-2 Transmit Typex
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1801;
			can_open_tx.sub_index = 0x02;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 255;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 25:		// Message #25: Transmit PDO-2 Event Timer (msec)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1801;
			can_open_tx.sub_index = 0x05;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2B;
				can_open_tx.data[0] = 5;			// 5 msec
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 26:		// Message #26: PDO-2 Mapping (Number of elements)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A01;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 3;			
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 27:		// Message #27: PDO-2 Mapping (position)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A01;
			can_open_tx.sub_index = 0x01;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x20;			
				can_open_tx.data[1] = 0x00;
				can_open_tx.data[2] = 0x04;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 28:		// Message #28: PDO-2 Mapping (speed)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A01;
			can_open_tx.sub_index = 0x02;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x10;			
				can_open_tx.data[1] = 0x01;
				can_open_tx.data[2] = 0x30;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 29:		// Message #29: PDO-2 Mapping (acceleration)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A01;
			can_open_tx.sub_index = 0x03;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x10;			
				can_open_tx.data[1] = 0x01;
				can_open_tx.data[2] = 0x40;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 30:		// Message #30: Transmit PDO-3 Largest supported sub index
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1802;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 5;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 32:		// Message #32: Transmit PDO-3 Transmit Typex
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1802;
			can_open_tx.sub_index = 0x02;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 255;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 35:		// Message #35: Transmit PDO-3 Event Timer (msec)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1802;
			can_open_tx.sub_index = 0x05;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2B;
				can_open_tx.data[0] = 5;			// 5 msec
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 36:		// Message #36: PDO-3 Mapping (Number of elements)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A02;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 3;			
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 37:		// Message #37: PDO-3 Mapping (position)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A02;
			can_open_tx.sub_index = 0x01;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x20;			
				can_open_tx.data[1] = 0x00;
				can_open_tx.data[2] = 0x04;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 38:		// Message #38: PDO-3 Mapping (speed)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A02;
			can_open_tx.sub_index = 0x02;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x10;			
				can_open_tx.data[1] = 0x01;
				can_open_tx.data[2] = 0x30;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 39:		// Message #39: PDO-3 Mapping (acceleration)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1A02;
			can_open_tx.sub_index = 0x03;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 0x10;			
				can_open_tx.data[1] = 0x01;
				can_open_tx.data[2] = 0x40;
				can_open_tx.data[3] = 0x60;
				valid_msg = 1;
			}
			break;
			
		case 41:		// Message #41:	Save All Parameters
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1010;
			can_open_tx.sub_index = 0x01;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 's';
				can_open_tx.data[1] = 'a';
				can_open_tx.data[2] = 'v';
				can_open_tx.data[3] = 'e';
				valid_msg = 1;
			}
			break;
			
		case 42:		// Message #42:	Load Default values
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1011;
			can_open_tx.sub_index = 0x01;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 'l';
				can_open_tx.data[1] = 'o';
				can_open_tx.data[2] = 'a';
				can_open_tx.data[3] = 'd';
				valid_msg = 1;
			}
			break;
			
		case 43:		// Message #43:	Save All Parameters
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1010;
			can_open_tx.sub_index = 0x03;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 's';
				can_open_tx.data[1] = 'a';
				can_open_tx.data[2] = 'v';
				can_open_tx.data[3] = 'e';
				valid_msg = 1;
			}
			break;
			
		case 44:		// Message #44:	Load Application Default values
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1011;
			can_open_tx.sub_index = 0x03;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 'l';
				can_open_tx.data[1] = 'o';
				can_open_tx.data[2] = 'a';
				can_open_tx.data[3] = 'd';
				valid_msg = 1;
			}
			break;
			
		case 50:		// Message #50: Heartbeat timeout
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1017;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2B;
				can_open_tx.data[0] = 0xF4;			// 500 msec
				can_open_tx.data[1] = 0x01;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 51:		// Message #51: Comm Fault error behavior (heartbeat)
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1029;
			can_open_tx.sub_index = 0x01;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 1;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 52:		// Message #52: Device Fault error behavior 
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1029;
			can_open_tx.sub_index = 0x02;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 1;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 53:		// Message #53: Internal Fault error behavior
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x1029;
			can_open_tx.sub_index = 0x03;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 1;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 60:		// Message #60: Set Operating parameters
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x6000;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				if ((fvars[fvencdir] & 0x01) != 0)
					can_enc_op_status |= 1;
				else 
					can_enc_op_status &= 0xfffe;
				
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2B;
				packw.word = can_enc_op_status;
				can_open_tx.data[0] = packw.byte[lbw];			
				can_open_tx.data[1] = packw.byte[hbw];
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 61:		// Message #61: Measuring Steps per revolution
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x6001;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_enc_ppr = fvars[fvencppr];
				packw.word = can_enc_ppr;
				can_open_tx.data[0] = packw.byte[lbw];			
				can_open_tx.data[1] = packw.byte[hbw];
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 62:		// Message #62: Total Measuring Range
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x6002;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				packdw.dword = can_enc_range;
				can_open_tx.data[0] = packdw.byte[lb];			
				can_open_tx.data[1] = packdw.byte[mlb];
				can_open_tx.data[2] = packdw.byte[mhb];
				can_open_tx.data[3] = packdw.byte[hb];
				valid_msg = 1;
			}
			break;
			
		case 63:		// Message #63: Preset Position
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x6003;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				packdw.dword = can_enc_preset_count;
				can_enc_count = can_enc_preset_count;
				can_open_tx.data[0] = packdw.byte[lb];			
				can_open_tx.data[1] = packdw.byte[mlb];
				can_open_tx.data[2] = packdw.byte[mhb];
				can_open_tx.data[3] = packdw.byte[hb];
				valid_msg = 1;
			}
			break;
			
		case 64:		// Message #64: Read Position
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x6004;
			can_open_tx.sub_index = 0x00;
			break;
			
		case 65:		// Message #65: Number of revolutions
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x6502;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2B;
				packw.word = can_enc_max_rev;
				can_open_tx.data[0] = packw.byte[lbw];			
				can_open_tx.data[1] = packw.byte[lbw];
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;
			}
			break;
			
		case 80:		// Message #80: Baud Rate
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x2100;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
			    if (fvars[fvencbaud] == 0)
					can_open_tx.data[0] = 5;			//	250k
				else if (fvars[fvencbaud] == 1)
					can_open_tx.data[0] = 4;			//	125k
				else					
					can_open_tx.data[0] = 0xFF;			//	USE JUMPER
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;	
			}
			break;
			
		case 81:		// Message #81: Node Address
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x2101;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
			//	can_open_tx.data[0] = 0xFF;		//		USE JUMPER
				can_open_tx.data[0] = 63;
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;	
			}
			break;
			
		case 82:		// Message #82: Termination Resistor
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x2102;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x2F;
				can_open_tx.data[0] = 1;		//		USE Termination resistor
				can_open_tx.data[1] = 0;
				can_open_tx.data[2] = 0;
				can_open_tx.data[3] = 0;
				valid_msg = 1;	
			}
			break;

		case 85:		// Message #85:	Save All bus parameters
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x2105;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 's';
				can_open_tx.data[1] = 'a';
				can_open_tx.data[2] = 'v';
				can_open_tx.data[3] = 'e';
				valid_msg = 1;
			}
			break;
			
		case 86:		// Message #86:	Store Init Code
			can_open_tx.msg = 0x60;
			can_open_tx.node_id = encoder_node_id;
			can_open_tx.index = 0x2140;
			can_open_tx.sub_index = 0x00;
			if (read_write == 1)
			{		// write command
				can_open_tx.len = 8;
				can_open_tx.cmd = 0x23;
				can_open_tx.data[0] = 'I';
				can_open_tx.data[1] = 'n';
				can_open_tx.data[2] = 'i';
				can_open_tx.data[3] = 't';
				valid_msg = 1;
			}
			break;			
		default:
			valid_msg = 0;
			break;
			
	}	
	if (valid_msg == 1)
	{
		Enc_Pack_Can(can_open_tx.msg,0);
		pkt_ret.pkt = 0;
		pkt_ret.stat = 0;
	}
}
									
//**************************************************
//*** Unpack Service Message
//**************************************************

/*
		The Slave Responds:
		Byte 0: RES
				RES = 0x43 Returning read request 4 bytes in bytes 4-7 
					= 0x4B Returning read request 2 bytes in byte 4,5
					= 0x4F Returning read request 1 byte in byte 4
					= 60 Data from write request received successfully (bytes 4-7 not used)
					= 80 Error, bytes 4-7 contain error code
		Byte 1 Index LSByte  Index = 16 bit address of the data
		Byte 2 Index MSByte
		Byte 3 Sub Index     Sub Index = 8 bit index from the address  
		Byte 4 Data LSB
		Byte 5 Data 
		Byte 6 Data
		Byte 7 Data MSB

	10		ENC			CPU						Message #10: Transmit Communications Parameters
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(05)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
	
	12		ENC			CPU						Message #12: Transmit PDO-1 Transmit Type
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	02			02			02
													DATA 4				--			dd(255)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
													
	15		ENC			CPU						Message #15: Transmit PDO-1 Event Timer (msec)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4B			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	03			05			05		
													DATA 4				--			dd(03)		ww		error code
													DATA 5				--			dd(00)		xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
														
														(Dynapar only)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4B			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	62			18			62
													DATA 3	SUB INDEX 	00			00			05	
													DATA 4				--			dd(03)		ww		error code
													DATA 5				--			dd(00)		xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz

	16 		ENC 		CPU						Message #16: PDO-1 Mapping (Number of elements)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(03)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
																										
	17 		ENC 		CPU						Message #17: PDO-1 Mapping (position)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	01			01			01
													DATA 4				--			dd(20)		ww		error code
													DATA 5				--			dd(00)		xx
													DATA 6				--			dd(04)		yy
													DATA 7				--			dd(60)		zz
																										
	18 		ENC 		CPU						Message #18: PDO-1 Mapping (speed)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	02			02			02
													DATA 4				--			dd(10)		ww		error code
													DATA 5				--			dd(01)		xx
													DATA 6				--			dd(30)		yy
													DATA 7				--			dd(60)		zz
																										
	19 		ENC 		CPU						Message #19: PDO-1 Mapping (acceleration)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	03			03			03
													DATA 4				--			dd(10)		ww		error code
													DATA 5				--			dd(01)		xx
													DATA 6				--			dd(40)		yy
													DATA 7				--			dd(60)		zz
														
	20		ENC			CPU						Message #20: Transmit PDO-2 Largest supported sub index
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(05)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
													
	22		ENC			CPU						Message #22: Transmit PDO-2 Transmit Type
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	02			02			02
													DATA 4				--			dd(255)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
													
	25		ENC			CPU						Message #25: Transmit PDO-2 Event Timer (msec)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4B			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	05			05			05
													DATA 4				--			dd(05)		ww		error code
													DATA 5				--			dd(00)		xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz

	26 		ENC 		CPU						Message #26: PDO-2 Mapping (Number of elements)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(03)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
																										
	27 		ENC 		CPU						Message #27: PDO-2 Mapping (position)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	01			01			01
													DATA 4				--			dd(20)		ww		error code
													DATA 5				--			dd(00)		xx
													DATA 6				--			dd(04)		yy
													DATA 7				--			dd(60)		zz
																										
	28 		ENC 		CPU						Message #28: PDO-2 Mapping (speed)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	02			02			02
													DATA 4				--			dd(10)		ww		error code
													DATA 5				--			dd(01)		xx
													DATA 6				--			dd(30)		yy
													DATA 7				--			dd(60)		zz
																										
	29 		ENC 		CPU						Message #29: PDO-2 Mapping (acceleration)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	03			03			03
													DATA 4				--			dd(10)		ww		error code
													DATA 5				--			dd(01)		xx
													DATA 6				--			dd(40)		yy
													DATA 7				--			dd(60)		zz
													
	30		ENC			CPU						Message #30: Transmit PDO-3 Largest supported sub index
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(05)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz

													
	32		ENC			CPU						Message #32: Transmit PDO-3 Transmit Type
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	02			02			02
													DATA 4				--			dd(255)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
													
	35		ENC			CPU						Message #35: Transmit PDO-3 Event Timer (msec)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4B			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	18			18			18
													DATA 3	SUB INDEX 	05			05			05
													DATA 4				--			dd(05)		ww		error code
													DATA 5				--			dd(00)		xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz

	36 		ENC 		CPU						Message #36: PDO-3 Mapping (Number of elements)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(03)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
																										
	37 		ENC 		CPU						Message #37: PDO-3 Mapping (position)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	01			01			01
													DATA 4				--			dd(20)		ww		error code
													DATA 5				--			dd(00)		xx
													DATA 6				--			dd(04)		yy
													DATA 7				--			dd(60)		zz
																										
	38 		ENC 		CPU						Message #38: PDO-3 Mapping (speed)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	02			02			02
													DATA 4				--			dd(10)		ww		error code
													DATA 5				--			dd(01)		xx
													DATA 6				--			dd(30)		yy
													DATA 7				--			dd(60)		zz
																										
	39 		ENC 		CPU						Message #39: PDO-3 Mapping (acceleration)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	1A			1A			1A
													DATA 3	SUB INDEX 	03			03			03
													DATA 4				--			dd(10)		ww		error code
													DATA 5				--			dd(01)		xx
													DATA 6				--			dd(40)		yy
													DATA 7				--			dd(60)		zz
													
	41		ENC			CPU						Message #41:	Save All Parameters
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	10	1010h	10			10
													DATA 2	INDES MSB	10			10			10
													DATA 3	SUB INDEX 	01			01			01
													DATA 4				--			's'			ww		error code
													DATA 5				--			'a'			xx		
													DATA 6				--			'v'			yy		
													DATA 7				--			'e'			zz
	
	42		ENC			CPU						Message #42:	Load Default values
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	11	1011h	11			11
													DATA 2	INDES MSB	10			10			10
													DATA 3	SUB INDEX 	01			01			01
													DATA 4				--			'l'			ww		error code
													DATA 5				--			'o'			xx		
													DATA 6				--			'a'			yy		
													DATA 7				--			'd'			zz
	
	43		ENC			CPU						Message #43:	Save Application Parameters
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	10	1010h	10			10
													DATA 2	INDES MSB	10			10			10
													DATA 3	SUB INDEX 	03			03			03
													DATA 4				--			's'			ww		error code
													DATA 5				--			'a'			xx		
													DATA 6				--			'v'			yy		
													DATA 7				--			'e'			zz
	
	43		ENC			CPU						Message #44:	Load Application Default values
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	11	1011h	11			11
													DATA 2	INDES MSB	10			10			10
													DATA 3	SUB INDEX 	03			03			03
													DATA 4				--			'l'			ww		error code
													DATA 5				--			'o'			xx		
													DATA 6				--			'a'			yy		
													DATA 7				--			'd'			zz
	
	50		ENC			CPU						Message #50:	Heartbeat Time
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4B			80
													DATA 1	INDEX LSB	17			17			17
													DATA 2	INDES MSB	10			10			10
													DATA 3	SUB INDEX 	01			01			01
													DATA 4				--			dd(F4)(500)	ww		error code
													DATA 5				--			dd(01)		xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
	
	51		ENC			CPU						Message #51:	Comm fault error behavior (heartbeat failure)
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	29			29			29
													DATA 2	INDES MSB	10			10			10
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(01)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
													
	52		ENC			CPU						Message #52:	Device error behavior
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	29			29			29
													DATA 2	INDES MSB	10			10			10
													DATA 3	SUB INDEX 	02			02			02
													DATA 4				--			dd(01)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
													
	53		ENC			CPU						Message #53:	Internal error behavior
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	29			29			29
													DATA 2	INDES MSB	10			10			10
													DATA 3	SUB INDEX 	03			03			03
													DATA 4				--			dd(01)		ww		error code
													DATA 5				--			--			xx
													DATA 6				--			--			yy
													DATA 7				--			--			zz
	
	60		ENC 		CPU						Message #60: Set Operating parameters
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4B			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	60			60			60
													DATA 3	SUB INDEX 	00			00			00		error code		Data
													DATA 4				--			dd			ww		zzyyxxww		Bit 0: Direction 0=CW, 1=CCW
																														Bit 1: Commissioning Diag Control 1=Enbable
																														Bit 2: Scaling 1=Enbaled

													DATA 5				--			dd			xx						Bit 13: Speed Format 0=RPM, 1=Units/sec
																														Bit 14: Startup auto in Op-Mode 1=Enable
																														Bit 15: Event mode position 1=Enable
													DATA 6				--			--			yy
													DATA 7				--			--			zz

									
	61		ENC		CPU							Message #61: Measuring Steps per revolution
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	60			60			60
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(FF)		ww		error code
													DATA 5				--			dd(FF)		xx
													DATA 6				--			dd(00)		yy
													DATA 7				--			dd(00)		zz
													
	62		ENC		CPU							Message #62: Total Measuring Range
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	60			60			60
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(00)		ww		error code
													DATA 5				--			dd(F0)		xx
													DATA 6				--			dd(FF)		yy		0FFFF000H = 268431360
													DATA 7				--			dd(0F)		zz
													
	63		ENC		CPU							Message #63: Preset Position
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	03			03			03
													DATA 2	INDES MSB	60			60			60
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd			ww		error code
													DATA 5				--			dd			xx
													DATA 6				--			dd			yy		dddddddd = poscnt preset value
													DATA 7				--			dd			zz
													
	64		ENC		CPU							Message #64: Read Position
																		READ		ERROR			
													DATA 0	CMD			43			80
													DATA 1	INDEX LSB	04			04
													DATA 2	INDES MSB	60			60
													DATA 3	SUB INDEX 	00			00
													DATA 4				dd			ww		error code
													DATA 5				dd			xx
													DATA 6				dd			yy		dddddddd = poscnt preset
													DATA 7				dd			zz
													
	80		ENC		CPU							Message #80: Baud Rate
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	00			00			00
													DATA 2	INDES MSB	21			21			21
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(FF)		ww		error code
													DATA 5				--			--			xx		dd = FF USE JUMPER 250 KBits/s
													DATA 6				--			--			yy		
													DATA 7				--			--			zz
											
	81		ENC		CPU							Message #81: Node Address
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	01			01			01
													DATA 2	INDES MSB	21			21			21
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(3F)		ww		error code
													DATA 5				--			--			xx		dd = FF USE JUMPER Node 63
													DATA 6				--			--			yy		
													DATA 7				--			--			zz
											
	82		ENC		CPU							Message #82: Termination Resistor
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			4F			80
													DATA 1	INDEX LSB	02			02			02
													DATA 2	INDES MSB	21			21			21
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			dd(00)		ww		error code
													DATA 5				--			--			xx		dd = 00 USE JUMPER TO HAVE TERMINATION RESISTOR
													DATA 6				--			--			yy		
													DATA 7				--			--			zz
											
	85		ENC		CPU							Message #85:	Save All bus parameters
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	05			05			05
													DATA 2	INDES MSB	21			21			21
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			's'			ww		error code
													DATA 5				--			'a'			xx		
													DATA 6				--			'v'			yy		
													DATA 7				--			'e'			zz
	
	86		ENC		CPU							Message #86:	Store Init Code
																		WRITE OKAY	READ		ERROR			
													DATA 0	CMD			60			43			80
													DATA 1	INDEX LSB	40			40			40
													DATA 2	INDES MSB	21			21			21
													DATA 3	SUB INDEX 	00			00			00
													DATA 4				--			'I'			ww		error code
													DATA 5				--			'n'			xx		
													DATA 6				--			'i'			yy		
													DATA 7				--			't'			zz
													

*/

void Unpack_Service_Msg (void)
{
	COMMDWORD_TYPE unpackdw;
	unsigned char return_status=0;
	
	if (can_open_rx.cmd == 0x60)
	{
		return_status = 1;			// 1 = ret write okay, 2= read okay, 4 = returned with error
		can_open_rx.data[0] = 0;
		can_open_rx.data[1] = 0;
		can_open_rx.data[2] = 0;
		can_open_rx.data[3] = 0;
	}	
	else if ((can_open_rx.cmd == 0x43) && (can_open_rx.len == 8))
		return_status = 2;
	else if ((can_open_rx.cmd == 0x4B) && ((can_open_rx.len == 6) || (can_open_rx.len == 8)))
	{
		return_status = 2;
		can_open_rx.data[2] = 0;
		can_open_rx.data[3] = 0;
	}
	else if ((can_open_rx.cmd == 0x4F) && ((can_open_rx.len == 5) || (can_open_rx.len == 8)))
	{
		return_status = 2;
		can_open_rx.data[1] = 0;
		can_open_rx.data[2] = 0;
		can_open_rx.data[3] = 0;
	}
	else if ((can_open_rx.cmd == 0x80) && (can_open_rx.len == 8))
	{
		return_status = 4;
		unpackdw.byte[lb] = can_open_rx.data[4];
		unpackdw.byte[mlb] = can_open_rx.data[5];
		unpackdw.byte[mhb] = can_open_rx.data[6];
		unpackdw.byte[hb] = can_open_rx.data[7];
		can_open_rx.error = unpackdw.dword;
	}
	else
		return_status = 8;
	
	switch(can_open_rx.index)
	{
		case 0x1800:		// Transmit PDO-1 
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 10;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 2)
			{
				pkt_ret.pkt = 12;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 5)
			{
				if ((((cons[enctype] == 0) || (cons[enctype] == 2)) && (fvars[fvenctype] == 0)) || ((fvars[fvenctype] == 1) || (fvars[fvenctype] == 3)))
				{
					pkt_ret.pkt = 15;
					pkt_ret.stat = return_status;
				}
			}
			break;
		case 0x6200:
			if(can_open_rx.sub_index == 0)
			{
				if (((cons[enctype] == 1) && (fvars[fvenctype] == 0)) || (fvars[fvenctype] == 2))
				{
					pkt_ret.pkt = 15;
					pkt_ret.stat = return_status;
				}
			}
			break;		
		case 0x1A00:		// PDO-1 Largest Mapping
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 16;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 1)
			{
				pkt_ret.pkt = 17;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 2)
			{
				pkt_ret.pkt = 18;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 3)
			{
				pkt_ret.pkt = 19;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x1801:		// Transmit PDO-2 
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 20;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 2)
			{
				pkt_ret.pkt = 22;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 5)
			{
				pkt_ret.pkt = 25;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x1A01:		// PDO-2 Largest Mapping
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 26;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 1)
			{
				pkt_ret.pkt = 27;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 2)
			{
				pkt_ret.pkt = 28;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 3)
			{
				pkt_ret.pkt = 29;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x1802:		// Transmit PDO-3 
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 30;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 2)
			{
				pkt_ret.pkt = 32;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 5)
			{
				pkt_ret.pkt = 35;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x1A02:		// PDO-3 Largest Mapping
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 36;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 1)
			{
				pkt_ret.pkt = 37;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 2)
			{
				pkt_ret.pkt = 38;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 3)
			{
				pkt_ret.pkt = 39;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x1010:		// Save parameters
			if (can_open_rx.sub_index == 1) 
			{
				pkt_ret.pkt = 41;				// save all
				pkt_ret.stat = return_status;
			}
			if (can_open_rx.sub_index == 3) 
			{
				pkt_ret.pkt = 43;				// save application pars
				pkt_ret.stat = return_status;
			}
			break;
		case 0x1011:		// Load Default values
			if (can_open_rx.sub_index == 1) 
			{
				pkt_ret.pkt = 42;		// load all
				pkt_ret.stat = return_status;
			}
			if (can_open_rx.sub_index == 3) 
			{
				pkt_ret.pkt = 44;		// load application pars
				pkt_ret.stat = return_status;
			}
			break;
		case 0x1017:		// Message #50: Heartbeat
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 50;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x1029:		//  Error behavior
			if (can_open_rx.sub_index == 1) 
			{			// communications fault
				pkt_ret.pkt =51;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 2) 
			{			// Device fault
				pkt_ret.pkt = 52;
				pkt_ret.stat = return_status;
			}
			else if (can_open_rx.sub_index == 3) 
			{			// Internal fault
				pkt_ret.pkt = 53;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x6000:		// Message #60: Set Operating parameters
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 60;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x6001:		// Message #61: Measuring Steps per revolution
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 61;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x6002:		// Message #62: Total Measuring Rangen
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 62;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x6003:		// Message #63: Preset Position
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 63;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x6004:		// Message #64: Read Position
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 64;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x2100:		// Message #80: Baud Rate
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 80;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x2101:		// Message #81: Node Address
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 81;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x2102:		// Message #82: Termination Resistor
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 82;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x2105:		// Message #85:	Save All bus parameters
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 85;
				pkt_ret.stat = return_status;		
			}
			break;
		case 0x2140:		// Message #86:	Save All bus parameters
			if (can_open_rx.sub_index == 0) 
			{
				pkt_ret.pkt = 86;
				pkt_ret.stat = return_status;
			}
			break;
		case 0x6502:
			if(can_open_rx.sub_index == 0)
			{
				pkt_ret.pkt = 65;
				pkt_ret.stat = return_status;				
			}
			break;		
		default:
			break;
	}	
}
	

/* Revision History
6/9/13 v7.0.8 mhd		1. Changed statement (ecrgix > c_enc_rx_buf_siz) to (ecrgix >= c_enc_rx_buf_siz) in Enc_Unpack_Can().
6/29/13 v7.0.9 mhd 		1. Do not update prev_Enc_Count or Prev_DPP_Count when position can_enc_count is updated.
8/11/13 V7.0.16 mhd		1. Modified read_encoder_int() to update the DPP_Count using the encoder count and the Encoder base count to prevent a velocity error. Also update
					 	   prev_Enc_Count and Prev_DPP_Count.
9/3/13 v7.0.18 mhd		1. Added parameter to select the encoder type instead of using the cons file parameter.
9/20/13 v7.0.19 mhd		1. Modified initialization sequence to detect a device with a baud rate of 250k or 125k and allow to set the baudrate for the electronic turck encoder. This software
						   must be tested with a Wachendorff encoder.
11/11/13 v7.1.28 mhd	1. Deleted checking o_RUN when updating pulse count change.
11/19/13 v7.0.29 mhd	1. Used fvars[fvenctype] == to tape selector feedback to set DPP reset count amount.
6/3/14 v7.1.3 mhd		1. Set statusf2 flag for encoder comm error.
5/11/15 v6.2.45 mhd		1. Place the SaveEncPar last in the if statements so that when both SaveEncBusPar and SaveEncPar are set, the SaveEncPar runs last otherwise the Turck encoder will 
						   not always save the event timer.
*/
