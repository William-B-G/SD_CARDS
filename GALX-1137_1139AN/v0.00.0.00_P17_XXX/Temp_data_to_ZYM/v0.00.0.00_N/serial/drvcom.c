//  These are the Drive Comm Routines
//     Drives supported so far are:  HPV-900, DSD-413, Quatro (HPV-900 Protocol), KEB Combivert F5
//

#define d_DRVCOM 1
#include "global.h"


//*********************************
// Drive Comm Definitions
//*********************************

// Mod bus and KEB drive comm symbols

#define EOT 0x04
#define STX 0x02
#define ETX 0x03
#define ENQ 0x05
#define ACK 0x06  
#define NAK 0x15

// Drive comm variables

int32 drv_debug1 = 0;
int32 drv_debug2 = 0;
int32 drv_debug3 = 0;
int32 drv_debug4 = 0;

#define drvrbufsize 200
#define drvtbufsize 200

// character strings used for drive communications
uint32 chk_sum;
uint8 drmbuf[drvrbufsize+1];             // drive recieve message buffer
uint8 dtranbuf[drvtbufsize+1];  			// drive transmit buffer
uint8 drecbuf[drvrbufsize+1];   			// drive receive buffer
unsigned char drv_trm_buf[40];
unsigned char drv_rcv_buf[40];
int16 dmpnt=0;       						// message pointer for com 1-2
int16 drbs=0; 							// receive buffer start pointer com 1-2
int16 drbf=0; 							// receive buffer end pointer com 1-2
int16 dtbs=0;				            	// trasmit buffer start pointer com 1-2
int16 dtbf=0;             				// trasmit buffer end pointer com 1-2
int16 Drv_Com_Tran_Int=0;					// comunication port transmit interupt running
int16 Drv_Com_Recv_Int=0;					// comunication port transmit interupt running
unsigned char Drv_Tranbuf_Empty = 1; 	// Drive Transmiter buffer is empty if 1 has chars if 0
unsigned char Drv_Tranbuf_Full = 0;		// Drive Transmit buffer is full if 1 and 0 if not

//					        {19200,38400,57600,115200,9600}		// 118 for 57.6 and 121 for 55500, 59 for 115.2 and 61 for 111000
int16 keb_drv_baud_tbl[5] = {  354,  177,  121,    61, 708};		// baud setting for 108.75 MHz
int16 drv_baud_tbl[5] =     {  354,  177,  118,    59, 708};		// baud setting for 108.75 MHz
int16 drv_modbus_tbl[6] = {0x03,0x07,0x1B,0x1F,0x13,0x0F};
uint32 tx_off_delay[5] = {124608,62304,41536,20768,249216};		 // (108.75 (1usec) * bit time * number bits * 2 bytes)

int16 drv_modbus = 0;
uint16 drv_baud = 354;		 				// default drive baud to 19200

int16 drv_half_duplex = 0;
int16 drive_fault=0;
int16 speed_ref_release = 0;		// drive speed reference release (speed reg rel if not brake timing from drive)
int16 drive_com_error=0;
int16 drive_online = 1;		// init to 1 so only 1 comm error on powerup
int16 drive_port = 0;
int16 drv_trm_fail_cnt = 0;
int16 rcv_drive_msg = 0;
int16 drive_vel=0;
int16 drv_trq = 0;
int16 ramp_dly=0;
int16 drv_trq_cmd=0;
int16 drive_rcv_cnt=0;
int16 drive_trm_cnt=0;
int16 drive_version=0;
int16 drive_revision=0;
int16 drive_pkt_err_cnt=0;
float Drv_Velocity=0;
long drv_tmp=0;
int16 dmd_trq=0;
int16 trq_inc=0;
int16 drv_com_seq = 0;		// sequence count for DSD 15 msec timer
int16 run_drive_com = 0;		// set to 1 to run drive com
int16 delay_com_cnt = 0;		// delay count for DSD 15 msec timer
int16 drv_soft_reset = 0;		// set to send software reset to drive
int16 drv_pre_trq_seq = 0;	// drive pre toqrue sequence
int16 pre_torque_cnt = 0;
int16 pre_trq_hold = 0;
uint8 Invoke_ID = 1;
uint8 rx_Invoke_ID = 1;
uint8 rx_data_ID = 0;
uint8 tx_data_ID = 0;
uint16 drv_tx_len = 0;

int16 drv_pwr_up_delay = 0;

int32 drv_status = 0;
int32 drv_command = 0;
int16 drv_motor_current = 0;
int16 drv_encoder_spd = 0;
int16 drv_motor_voltage = 0;
int16 drv_motor_frequency = 0;
int16 drv_fault_code = 0;
int16 drv_cmd_addr = 0;
int32 drv_wpar_data = 0;
int32 drv_rpar_data = 0;
int16 drv_wpar_addr = 0;
int16 drv_rpar_addr = 0;
int32 drv_wpar_ret_data = 0;
int16 drv_wpar_ret_addr = 0;
int16 drv_rpar_ret_addr = 0;
int16 drv_wpar_set = 0;
int16 drv_rpar_set = 0;
int16 drv_init_seq = 0;
int16 drv_init_dly = 0;
int16 drv_mode = 100;
int16 drv_read_status = 0;
int16 drv_rstatus_addr = 0;
uint8 read_reg_status = 0;
uint8 drv_nak = 0;
uint8 drv_ack = 0;
uint8 drv_eot = 0;
int16 drv_error = 0;
int16 drv_pkt_status = 0;
uint8 drv_response_code[2] = {0,0};
int16 drv_ep_timer = 0;
int16 drv_delay_cnt = 0;		// drive response delay timer
 
int16 drv_ep_mode = 0;
int16 drv_rset_ep_mode = 0;
int16 drv_has_power = 0;				// used for emergency power lowering, 1 = normal power, 2 = emergency power
 
int16 drv_com_count = 0;
//int32 drv_com_source = 0;
 
int16 drv_line_voltage = 0;
int16 drv_motor_torque = 0;
int16 drv_regen_torque = 0;

int16 prev_com_hour = 0;
int16 drive_com_error_cnt = 0;

char drv_rcv_string[10];
char drv_trm_string[10];

void drvcomint(void);
char drv_asigetc(void);
void drv_sendc(char letter);

void init_drv_com (void);
void init_drv_pit (void);
void drv_dma_trm_int(void);
void drv_com_timer_int(void);
void drv_com_start_timer_int (void);
int16 set_drv_ep_mode (int16 drv_ep_mode_seq);
int16 reset_drv_ep_mode (int16 drv_rset_ep_mode_seq);
void drv_com (void);
void rcv_HPV_drv_com (void);
void rcv_DSD_drv_com (void);
void send_HPV_drv_msg(int16 msg);
void send_DSD_drv_msg(void);
void send_L1000_drv_msg(int16 msg);
void rcv_L1000_drv_com (void);
uint16 crc_chk (unsigned char* cm_buff, uint16 length);
void CBV_F5_drv_tx_ctl (int16 drv_msg);
void send_CBV_F5_drv_msg(int16 msg);
void rcv_CBV_F5_drv_com (void);
extern void set_KEB_par_min_max (void);
uint8 ascii_to_hex(uint8 value);
void init_drv_DMA(void);
int16 drv_isrxempty(void);
unsigned char ismsgvalid (unsigned char msg);
unsigned char get_msg_len(unsigned char msg, unsigned char nextbyte);
void verify_run_mode (void);


extern void drv_com_tx_start (int16 len);

extern int16 put_pkt_req(int16 pkt_nmb, int16 addr, int16 com_nmb);
extern int16 get_pkt_req(int16 addr, int16 com_nmb);
void put_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb);

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		

// ***************************************************************
// ***************************************************************
// Initialize data for drive comm
// ***************************************************************
// ***************************************************************


void init_drv_com (void)
{
	uint16 data_in;
	
	_disable();

	drive_port = DRVCOM;
	// Drive Comm
		
	DRVtxDis; 						// Turn off Transmit driver

	DRVport.CR1.B.TE = 0;			// Transmitter Disable
	DRVport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable

	// Install our handlers   
//	INTC_InstallINTCInterruptHandler(drvcomint, DRVintLoc, 1);  


	// Control register 1	  
	DRVport.LCR.B.LIN = 0;			// Place in SCI mode	
	DRVport.CR1.B.SBR = drv_baud;
	
	DRVport.CR1.B.LOOPS = 0;		// Receiver source mode LOOPS:RSRC 00 = Dual wire mode
	DRVport.CR1.B.RSRC = 0;
	
	if (Drive_Type[cons[carnmb]] == 12) 
	{		// KEB drive 1 stop / 7 data bits / Even parity
		
		DRVport.CR1.B.M = 0;			// Frame Format Mode: set to 1 for nine bits
		DRVport.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
		DRVport.CR1.B.PE = 1;			// Parity Enable: 0 = disabled, 1 = enabled 
		DRVport.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd
		DRVport.CR3.B.M2 = 0;
//		if (fvars[fvkebdpytype] == 1)
//			drv_half_duplex = 1;
//		else
			drv_half_duplex = 0;			// else full duplex
	}
	else
	{
		
		DRVport.CR1.B.M = 0;			// Frame Format Mode: set to 1 for nine bits
		DRVport.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
		DRVport.CR1.B.PE = 0;			// Parity Enable: 0 = disabled, 1 = enabled 
		DRVport.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd
		DRVport.CR3.B.M2 = 0;
		drv_half_duplex = 0;			// else full duplex
	}
	DRVport.CR3.B.EROE = 0;			// Set error bit in receive data register 
	DRVport.CR3.B.ERFE = 0;			// Set error bit in receive data register
	DRVport.CR3.B.ERPE = 0;			// Set error bit in receive data register


	DRVport.CR1.B.ILT = 0;			// Idle Line Type: 0 = idle line detection after low bit
	DRVport.CR1.B.TIE = 0;			// Transmit Interrupt Enable (data tranferred to shift register)
	DRVport.CR1.B.TCIE = 0;			// Transmit Complete Interrupt Enable (data shifted out of shift register)
	DRVport.CR1.B.RIE = 0;			// Receive Interrupt Enable
	DRVport.CR1.B.ILIE = 0;			// Idle line interrupt enable
	DRVport.CR1.B.RWU = 0;			// Receiver Wake Up mode: 0 = normal, 1 = wakeup on 9th bit address
	DRVport.CR1.B.SBK = 0;			// Send Break Character
	
	// Control Register 2
	DRVport.CR2.B.MDIS = 0;			// Module Disable
	DRVport.CR2.B.FBR = 0;			// Fast Bit Error Detect: 0 = normal, 1 = use in LIN mode	
	DRVport.CR2.B.BSTP = 0;			// DMA Stop on Bit Error: 0 = tx dma regardless of bit errors
	DRVport.CR2.B.IEBERR = 0;		// Bit Error Interrupt Enable
	DRVport.CR2.B.RXDMA = 1;		// Receive DMA Control: 0 = disabled, 1 = enabled
	DRVport.CR2.B.TXDMA = 0;		// Transmit DMA Control: 0 = disabled, 1 = enabled
	DRVport.CR2.B.BRK13 = 0;		// Break Character Length
	DRVport.CR2.B.TXDIR = 0;		// TXD pin output enable: Used in signel wire mode
	DRVport.CR2.B.BESM13 = 0;		// Fast Bit Error Detection Sample Mode: 0=RS9,1=RS10 LIN mode only
	DRVport.CR2.B.SBSTP = 0;		// Transmit Stop on Bit Error
	DRVport.CR2.B.RXPOL = 0;		// RXD Pin polarity: 0 = normal, 1 = inverted
	DRVport.CR2.B.PMSK = 0;			// Parity Bit Masking: 0 = in bit position, 1 = 0 in parity position	
	DRVport.CR2.B.ORIE = 0;			// Overrun Interrupt Enable
	DRVport.CR2.B.NFIE = 0;			// Noise Interrupt Enable
	DRVport.CR2.B.FEIE = 0;			// Frame Error Interrupt Enable
	DRVport.CR2.B.PFIE = 0;			// Parity Error Interrupt Enable
	

	data_in = DRVport.SDR.R;
	data_in = DRVport.SDR.R;

	init_drv_DMA();

  	Drv_Tranbuf_Empty = 1;
	Drv_Tranbuf_Full = 0;
	dtbs = 0;
	dtbf = 0;
	drbs = 0;
	drbf = 0;

	// Enable must be last
	DRVport.CR1.B.TE = 1;			// Transmitter Enable
	
	if (drv_half_duplex == 0)
		DRVport.CR1.B.RE = 1;			// Receiver Enable

	if ((Drive_Type[cons[carnmb]] == 2) || (Drive_Type[cons[carnmb]] == 3) || (Drive_Type[cons[carnmb]] == 6) || 
	    (Drive_Type[cons[carnmb]] == 10) || (Drive_Type[cons[carnmb]] == 11))
	{	// any vector drive (HPV) or Quatro
		put_pkt_req(0x11,2,drive_port);
		put_pkt_req(0x21,2,drive_port);
		put_pkt_req(0x31,2,drive_port);
		put_pkt_req(0x41,2,drive_port);
		put_pkt_req(0x51,2,drive_port);
		drv_wpar_addr = 0;
		uiu.b[hb] = 0;				// Msb Data
		uiu.b[mhb] = 0;				// Low Msb Data
		uiu.b[mlb] = 0x03;			// High Lsb Data (torque reference)
		uiu.b[lb] = 0x01;			// Lsb Data (speed feedback)
		drv_wpar_data = uiu.dw;
		put_pkt_req(0x13,2,drive_port);	// set monitor data
	}
	
	init_drv_pit();
	
	_enable();
}

//*****************************************
// Init Drive Programmable Interval Timer
//*****************************************
void init_drv_pit (void)
{
	PIT.TCTRL4.B.TEN = 0;		// Stop Timer
	INTC_InstallINTCInterruptHandler(drv_com_timer_int, 152, 2);  // Set interrupt vector for PIT4

	PIT.TCTRL5.B.TEN = 0;		// Stop Timer
	INTC_InstallINTCInterruptHandler(drv_com_start_timer_int, 153, 1);  // Set interrupt vector for PIT5
	// 10 msec interrupt timer - PIT 5
	if (fvars[fvdrvupd] == 1)
	{		// 15 msec update
		PIT.LDVAL5.R = 1631250 - 1;		// setup timer 1 for 10 msec with 108.75 MHz fsys
	}
	else if (fvars[fvdrvupd] == 2)
	{		// 20 msec update
		PIT.LDVAL5.R = 2175000 - 1;		// setup timer 1 for 10 msec with 108.75 MHz fsys
	}
	else
		PIT.LDVAL5.R = 1087500 - 1;		// setup timer 1 for 10 msec with 108.75 MHz fsys
	PIT.TFLG5.B.TIF = 1;			// Clear interrupt flag so an interrupt does not occur right away
	PIT.TCTRL5.B.TIE = 1;		// Enable interrupts
	PIT.TCTRL5.B.TEN = 1;		// Start Timer

}

//*****************************************
// DMA Initialization for drive Comm
//*****************************************


void init_drv_DMA(void) {

	// Initialize Transmit Comm DMA
  
  	EDMA.CERQR.R = DRVdmaChTx;      	// Disable Group transmit EDMA channel   

	INTC_InstallINTCInterruptHandler(drv_dma_trm_int, DRVdmaTxIntLoc, 1);  

  	EDMA.CPR[DRVdmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[DRVdmaChTx].R = DRVdmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[DRVdmaChTx].SADDR = (uint32) &drv_trm_buf[0];  	// Load address of source data 
  	EDMA.TCD[DRVdmaChTx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[DRVdmaChTx].SOFF = 1;                         		// After transfer, add 1 to src addr
  	
  	// init to ten bytes for hpv 900
  	EDMA.TCD[DRVdmaChTx].SLAST = -10;                      		// After major loop, reset src addr 
  	EDMA.TCD[DRVdmaChTx].SMOD = 0;                         		// Source modulo feature not used 

  	EDMA.TCD[DRVdmaChTx].DADDR =  DRVdr; 						// Load address of destination 
  	EDMA.TCD[DRVdmaChTx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[DRVdmaChTx].DOFF = 0;                         		// Do not increment destination addr 
  	EDMA.TCD[DRVdmaChTx].DLAST_SGA = 0;                    		// After major loop, no dest addr change 
  	EDMA.TCD[DRVdmaChTx].DMOD = 0;                         		// Destination modulo feature not used 
  
  	EDMA.TCD[DRVdmaChTx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[DRVdmaChTx].BITER = 10;                       		// 10 major loop iterations 
  	EDMA.TCD[DRVdmaChTx].CITER = 10;                       		// Initialize current iteraction count 
  	EDMA.TCD[DRVdmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[DRVdmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[DRVdmaChTx].INT_MAJ = 1;
  	EDMA.TCD[DRVdmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[DRVdmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[DRVdmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[DRVdmaChTx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[DRVdmaChTx].E_SG = 0; 
  	EDMA.TCD[DRVdmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[DRVdmaChTx].DONE = 0;
  	EDMA.TCD[DRVdmaChTx].ACTIVE = 0;
  	EDMA.TCD[DRVdmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = DRVdmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = DRVdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = DRVdmaChTx;           	// DMA Clear error Register 
  
  
  
  	// Initialize Receive Comm DMA

 	EDMA.CERQR.R = DRVdmaChRx;      		// Disable group EDMA channel   
  
  	EDMA.CPR[DRVdmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[DRVdmaChRx].R = DRVdmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[DRVdmaChRx].SADDR = DRVdr;  						// Load address of source data 
  	EDMA.TCD[DRVdmaChRx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[DRVdmaChRx].SOFF = 0;                         		// Do not increment source address
  	
  	EDMA.TCD[DRVdmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
  	EDMA.TCD[DRVdmaChRx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[DRVdmaChRx].DADDR =  (uint32) &drecbuf[0]; 		// Load address of destination 
  	EDMA.TCD[DRVdmaChRx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[DRVdmaChRx].DOFF = 1;                         		// Increment destination addr by 1 
  	EDMA.TCD[DRVdmaChRx].DLAST_SGA = (0 - drvrbufsize);           // After major loop reset destination address
  	EDMA.TCD[DRVdmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
  
  	EDMA.TCD[DRVdmaChRx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[DRVdmaChRx].BITER = drvrbufsize;             		// Major loop iterations 
  	EDMA.TCD[DRVdmaChRx].CITER = drvrbufsize;             		// Initialize current iteraction count 
  	EDMA.TCD[DRVdmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
  	EDMA.TCD[DRVdmaChRx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[DRVdmaChRx].INT_MAJ = 0;
  	EDMA.TCD[DRVdmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[DRVdmaChRx].BITERE_LINK = 0;
  	EDMA.TCD[DRVdmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[DRVdmaChRx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[DRVdmaChRx].E_SG = 0; 
  	EDMA.TCD[DRVdmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[DRVdmaChRx].DONE = 0;
  	EDMA.TCD[DRVdmaChRx].ACTIVE = 0;
  	EDMA.TCD[DRVdmaChRx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = DRVdmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = DRVdmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = DRVdmaChRx;           	// DMA Clear error Register 
  	EDMA.SERQR.R = DRVdmaChRx;				// Enable group coomm DMA channel
  
}



//*********************************************
// Drive DMA transmit finished interrupt
//*********************************************

void drv_dma_trm_int(void)
{

  	EDMA.CIRQR.R = DRVdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = DRVdmaChTx;      	// Disable Drive EDMA channel   
	DRVport.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	drv_tx_len = 0;

	if (drv_half_duplex == 1)
	{
		// Transmit end timer - PIT 4
		PIT.TCTRL4.B.TEN = 0;		// Reset Timer
		PIT.LDVAL4.R = tx_off_delay[fvars[fvdrvbaud]] + 10875;		// setup timer 1 for (100 usec) * incremental amount with 108.75 MHz fsys
		PIT.TCTRL4.B.TEN = 1;		// Start Timer
		PIT.TFLG4.B.TIF = 1;			// Clear interrupt flag so an interrupt does not occur right away
		PIT.TCTRL4.B.TIE = 1;		// Enable interrupts
	}
}

//*********************************************
// Drive Transmit Finished Timer Interrupt
//*********************************************

void drv_com_timer_int(void)
{
	static int16 tmr_count;
	
	PIT.TFLG4.B.TIF = 1;		// Clear interrupt flag 
	PIT.TCTRL4.B.TIE = 0;		// Disable interrupts
	PIT.TCTRL4.B.TEN = 0;		// Stop Timer
		
	tmr_count = 100;
	while ((tmr_count > 0) && (DRVport.SR.B.TC == 0))
	{
		tmr_count--;
	}
	DRVtxDis; 				// Turn off Transmit driver
	
	// Turn on the receiver
	DRVport.CR1.B.RE = 1;			// Receiver Enable: 1 = Enable, 0 = Disable

}


//*********************************************
// Drive Transmit Start 10 msec Timer Interrupt
//*********************************************
void drv_com_start_timer_int (void)
{
	PIT.TFLG5.B.TIF = 1;		// Clear interrupt flag 
	run_drive_com = 1;			// allow to pack next message
	if (drv_tx_len != 0)
	{				// message has been packed
		drv_com_tx_start(drv_tx_len);
	}
}

// ***************************************************************
// ******** Start Drive Comm *************************************
// ***************************************************************

void drv_com_tx_start (int16 len)
{
	_disable();	
	
	if (drv_half_duplex == 1)
	{	// hal duplex
		
		// Turn off the receiver
		DRVport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable
	}
	
	// Enable transmitter
	DRVtxEn;
	// Start DMA transfer for com
	EDMA.CER.R = DRVdmaChTx;           	// DMA Clear error Register 
	DRVport.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
    EDMA.TCD[DRVdmaChTx].SADDR = (uint32) &drv_trm_buf[0];  	// Load address of source data 
 	EDMA.TCD[DRVdmaChTx].SLAST = (0-len);        // After major loop, reset src addr 
 	EDMA.TCD[DRVdmaChTx].NBYTES = 1;	// Transfer len number byte per minor loop 
  	EDMA.TCD[DRVdmaChTx].BITER = len;   // 10 major loop iterations 
  	EDMA.TCD[DRVdmaChTx].CITER = len;   // Initialize current iteraction count 
  	EDMA.SERQR.R = DRVdmaChTx;      	// Enable Drive EDMA channel   
	DRVport.CR2.B.TXDMA = 1;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	_enable();
}


//************************************
// serial interrupt for drive com port  
//************************************
/*
void drvcomint(void)
{

	drv_com_count++;
	drv_com_source = DRVport.SR.R;
	if ((drv_com_source & 0x0F000000) != 0)		// Overrun, Noise, Framing or Parity error
	{
		DRVport.SR.R = 0x0F000000;	// Clear error flags by writing ones.
	}
	if ((DRVport.SR.B.RDRF == 1) && (DRVport.CR1.B.RIE == 1))	//Interupt source is a recieve data ready
	{
		Drv_Com_Recv_Int = 1;		// set a flag receive interupts running
		drecbuf[drbf] = (char)DRVport.SDR.B.D;	// Read data byte from buffer
		DRVport.SR.R = 0x20000000;           // Clear RDRF flag 
		drbf++;
		if(drbf >= drvrbufsize)
			drbf = 0;
	}
	if ((DRVport.SR.B.TDRE == 1) && (DRVport.CR1.B.TIE == 1))  // Interupt source is transmiter hold reg empty
	{
		Drv_Com_Tran_Int = 1;		// set a flag Transmit interupts running
	
		DRVport.SR.R = 0x80000000;           // Clear TDRE flag 
//		DRVport.SDR.B.D = dtranbuf[dtbf];
	
		dtbf++;
		if(dtbf >= drvtbufsize)
			dtbf = 0;
		
			
		if (dtbs >= dtbf)
		{
			if ((dtbf + (drvtbufsize - dtbs)) >= 50)
				Drv_Tranbuf_Full = 0;
		}
		else
		{
			if ((dtbf - dtbs) >= 50)
				Drv_Tranbuf_Full = 0;
		}
		if(dtbs == dtbf)   // disabel transmiter empty interupt
		{
			DRVport.CR1.B.TIE = 0;			// Transmit Interrupt Disabled
			Drv_Tranbuf_Empty = 1;
			Drv_Tranbuf_Full = 0;
		}
	}
}

*/

// ********************************************************
// This is interupt driven get char from the receive buffer
// ********************************************************
char drv_asigetc(void)
{
  	char i;
  	i = drecbuf[drbs];
//  	drecbuf[drbs] = 0;
  	drbs++;
  	if(drbs >= drvrbufsize)
		drbs = 0;
  	return(i);
}

//****************************************
// Test to see if receive buffer is empty
//****************************************
int16 drv_isrxempty(void)
{
	static int16 ite_count, beg_count, dma_count;

	beg_count = (int16)EDMA.TCD[DRVdmaChRx].BITER;
	ite_count = (int16)EDMA.TCD[DRVdmaChRx].CITER;
	dma_count = (int16)(beg_count - ite_count);
	
	if (drbs == dma_count) 
      	return(1);
    else
      	return(0);
}




//*********************************************
// This is send a character out on a COM Port
//*********************************************
void drv_sendc(char letter)
{
//	disableIrq();
	dtranbuf[dtbs] = letter;
	dtbs++;
	if(dtbs >= drvtbufsize)
		dtbs = 0;
	
	if (dtbs >= dtbf)
	{
		if ((dtbf + (drvtbufsize - dtbs)) < 50)
			Drv_Tranbuf_Full = 1;
	}
	else
	{
		if ((dtbf - dtbs) < 50)
			Drv_Tranbuf_Full = 1;
	}
	Drv_Com_Tran_Int = 0;
	Drv_Tranbuf_Empty = 0;
		
	if ((Drive_Type[cons[carnmb]] != 14) && (Drive_Type[cons[carnmb]] != 15))
	{
//			if ((DRVport.SR.B.TACT == 0) && (DRVport.CR1.B.TIE == 1))
//				DRVport.CR1.B.TIE = 0;			// Transmit Interrupt Disable first to allow enable to cause an interrupt
		DRVport.CR1.B.TIE = 1;			// Transmit Interrupt Enabled
	}

//	enableIrq();

    // enable transmiter empty interupt
}


//********************************************************
// Set Drive Emergency Power Mode
// Modify drive parameters to run with single phase UPS
//********************************************************

int16 set_drv_ep_mode (int16 drv_ep_mode_seq)
{
	int16 i;
	static int16 drv_set_req_attempts;
	switch (drv_ep_mode_seq)
	{
		case 0:
			if ((drive_online == 1) && (rdinp(i_READY) == 1))
			{
				drv_ep_mode_seq = 1;
				drv_line_voltage = 0;
				drv_motor_torque = 0;
				drv_regen_torque = 0;
				drv_set_req_attempts = 0;
			}
			break;
		case 1:				// request input line voltage
			drv_rpar_addr = 103;
			put_pkt_req(0x61,2,drive_port);
			drv_ep_mode_seq = 2;
			drv_ep_timer = 0;
			break;
		case 2:	// get and store input line voltage
			i = get_pkt_status(0x61,2,drive_port, 40);
			if(i == 1)
			{
				drv_line_voltage = (int16)drv_rpar_data;
				drv_ep_mode_seq = 3;
			}
			else if(drv_ep_timer>100)
			{
				if(drv_set_req_attempts<10)
				{
					drv_ep_mode_seq = 1;
					drv_set_req_attempts++;
				}
				else
				{
					drv_ep_mode_seq = 3;
				}
			}
			break;
		case 3: // request motor torque
			drv_rpar_addr = 28;		 // request motor torque
			put_pkt_req(0x61,2,drive_port);
			drv_ep_mode_seq = 4;
			drv_ep_timer = 0;		
			break;
		case 4:	// get and store motor torque
			i = get_pkt_status(0x61,2,drive_port, 40);		
			if (i == 1)
			{
				drv_motor_torque = (int16)drv_rpar_data;
				drv_ep_mode_seq = 5;
			}		
			else if(drv_ep_timer>100)
			{
				if(drv_set_req_attempts<10)
				{
					drv_ep_mode_seq = 3;
					drv_set_req_attempts++;
				}
				else
				{
					drv_ep_mode_seq = 5;
				}
			}
			break;
		case 5: // request regen torque
			drv_rpar_addr = 29;		 // request regen torque
			put_pkt_req(0x61,2,drive_port);
			drv_ep_mode_seq = 6;
			drv_ep_timer = 0;		
			break;
		case 6:	// get and store motor torque
			i = get_pkt_status(0x61,2,drive_port, 40);		
			if (i == 1)
			{
				drv_regen_torque = (int16)drv_rpar_data;
				drv_ep_mode_seq = 7;
			}		
			else if(drv_ep_timer>100)
			{
				if(drv_set_req_attempts<10)
				{
					drv_ep_mode_seq = 5;
					drv_set_req_attempts++;
				}
				else
				{
					drv_ep_mode_seq = 7;
				}
			}
			break;
		case 7: // set low input line voltage
			drv_wpar_addr = 103;			// set low input line voltage
			drv_wpar_data = 140;			// 240 volts ac
			put_pkt_req(0x43,2,drive_port);
			drv_ep_mode_seq = 8;
			drv_ep_timer = 0;
			break;
		case 8:	//get low input line voltage confirmation
			i = get_pkt_status(0x43,2,drive_port, 40);
			if (i == 1)
			{
				drv_ep_mode_seq = 9;
			}		
			else if(drv_ep_timer>100)
			{
				if(drv_set_req_attempts<10)
				{
					drv_ep_mode_seq = 7;
					drv_set_req_attempts++;
				}
				else
				{
					drv_ep_mode_seq = 9;
				}
			}			
			break;
		case 9:	//set motor torque
			drv_wpar_addr = 28;				// Set Motor Torque Limit
			drv_wpar_data = 1000;			// 100.0 percent
			put_pkt_req(0x43,2,drive_port);
			drv_ep_mode_seq = 10;
			drv_ep_timer = 0;
			break;
		case 10:	// get motor torque confirmation
			i = get_pkt_status(0x43,2,drive_port, 40);
			if (i == 1)
			{
				drv_ep_mode_seq = 11;
			}		
			else if(drv_ep_timer>100)
			{
				if(drv_set_req_attempts<10)
				{
					drv_ep_mode_seq = 9;
					drv_set_req_attempts++;
				}
				else
				{
					drv_ep_mode_seq = 11;
				}
			}			
			break;	
		case 11:	//set regen torque
			drv_wpar_addr = 29;				// Set Regen Torque Limit
			drv_wpar_data = 1000;			// 100.0 percent
			put_pkt_req(0x43,2,drive_port);
			drv_ep_mode_seq = 12;
			drv_ep_timer = 0;
			break;
		case 12:	// get regen torque confirmation
			i = get_pkt_status(0x43,2,drive_port, 40);
			if (i == 1)
			{
				drv_ep_mode_seq = 13;
				save_drive_to_MRAM();
			}		
			else if(drv_ep_timer>100)
			{
				if(drv_set_req_attempts<10)
				{
					drv_ep_mode_seq = 11;
					drv_set_req_attempts++;
				}
				else
				{
					drv_ep_mode_seq = 13;
				}
			}			
			break;
		case 13: //DONE
			drv_ep_mode_seq = 13;
			break;
		default:
			drv_ep_mode_seq = 0;
			break;
	}
	return drv_ep_mode_seq;
}


//********************************************************
// Set Drive Emergency Power Mode
// Modify drive parameters to run with single phase UPS
//********************************************************

int16 reset_drv_ep_mode (int16 drv_rset_ep_mode_seq)
{
	int16 i;
	static int16 drv_reset_req_attempts;
	switch (drv_rset_ep_mode_seq)
	{
		case 0:
			if ((drive_online == 1) && (rdinp(i_READY) == 1))
			{
				drv_rset_ep_mode_seq = 1;
				read_drive_from_MRAM();
				drv_reset_req_attempts = 0;
			}
			break;
		case 1://send line voltage
			if (drv_line_voltage != 0)
			{
				drv_wpar_addr = 103;						// input line voltage
				drv_wpar_data = drv_line_voltage;			// 240 volts ac
				put_pkt_req(0x43,2,drive_port);
			}
			drv_rset_ep_mode_seq = 2;
			drv_ep_timer = 0;
			break;
		case 2://get line voltage confirmation
			i = get_pkt_status(0x43,2,drive_port, 40);
			if (i == 1)
			{
				drv_rset_ep_mode_seq = 3;
			}
			else if(drv_ep_timer > 100)
			{
				if((drv_reset_req_attempts<10)&&(drv_line_voltage != 0))
				{//go back to reset voltage again
					drv_rset_ep_mode_seq = 1;
					drv_reset_req_attempts++;
				}
				else
				{
					drv_rset_ep_mode_seq = 3;
				}
			}
			break;
		case 3:// send motor torque
			if (drv_motor_torque != 0)
			{
				drv_wpar_addr = 28;							// Motor Torque Limit
				drv_wpar_data = drv_motor_torque;			// xxx.x percent
				put_pkt_req(0x43,2,drive_port);
			}		
			drv_rset_ep_mode_seq = 4;
			drv_ep_timer = 0;
			break;
		case 4: // get motor torque confirmation
			i = get_pkt_status(0x43,2,drive_port, 40);
			if (i == 1)
			{
				drv_rset_ep_mode_seq = 5;
			}
			else if(drv_ep_timer > 100)
			{
				if((drv_reset_req_attempts<10)&&(drv_motor_torque != 0))
				{//go back to reset motor torque again
					drv_rset_ep_mode_seq = 3;
					drv_reset_req_attempts++;
				}
				else
				{
					drv_rset_ep_mode_seq = 5;
				}
			}
			break;
		case 5:// send regen torque
			if (drv_regen_torque != 0)
			{
				drv_wpar_addr = 29;							// regen Torque Limit
				drv_wpar_data = drv_regen_torque;			// xxx.x percent
				put_pkt_req(0x43,2,drive_port);
			}
			drv_rset_ep_mode_seq = 6;
			drv_ep_timer = 0;
			break;

		case 6: // get regen torque confirmation
			i = get_pkt_status(0x43,2,drive_port, 40);
			if (i == 1)
			{
				drv_rset_ep_mode_seq = 7;
			}
			else if(drv_ep_timer > 100)
			{
				if((drv_reset_req_attempts<10)&&(drv_regen_torque != 0))
				{//go back to reset regen torque again
					drv_rset_ep_mode_seq = 5;
					drv_reset_req_attempts++;
				}
				else
				{
					drv_rset_ep_mode_seq = 7;
				}
			}
			break;
		case 7:
			drv_rset_ep_mode_seq = 7;
			break;
		default:
			drv_rset_ep_mode_seq = 0;
			break;

	}
	return drv_rset_ep_mode_seq;
}


// ***************************************************************
// ***************************************************************
// Communications routine for the hpv-900 drive
// ***************************************************************
// ***************************************************************

#if ((Traction == 1) || (Tract_OL == 1))

void drv_com (void)
{
	int16 i,drv_msg;
	bool clamp_spd_rate = 0;

 #if (Simulator == 1)
 	rcv_drive_msg = 1;
	drive_com_error = 0;
 #endif

	if (drv_pwr_up_delay < 500)		// 5 second delay with 10 msec update rate
		timers[tdrive_com] = 0;

	if ((rcv_drive_msg == 1) && (drive_com_error_cnt <= 20))
	{
		statusf &= ~sf_DRV;		// Drive comm ok
		rcv_drive_msg = 0;
		timers[tdrive_com] = 0;
		drive_online = 1;
		if (drv_init_seq == 0)
			drv_init_seq = 1;
 		SIU.GPDO[TST3].R = 0x00;
	}
	else if (timers[tdrive_com] > 10)
	{
		statusf |= sf_DRV;		// Drive comm error

		if (drive_online == 1)
		{
		  	record_fault(f_drv_rcv_error);
		}
		SIU.GPDO[TST3].R = 0x01;
		drive_online = 0;
		speed_ref_release = 0;
		timers[tdrive_com] = 0;
		if ((Drive_Type[cons[carnmb]] == 12) && (fvars[fvkebdpytype] == 0))
		{
			if (drv_init_seq >= 12)
				drv_init_seq = 0;
		}
		if (drive_com_error_cnt > 20)
		{
			rcv_drive_msg = 0;
			drive_com_error_cnt = 0;
		}
	}

	if (drive_com_error != 0)
	{
		drive_com_error = 0;
		drv_soft_reset = 1;
		if (drv_pwr_up_delay > 500)		// 5 second delay with 10 msec update rate
			drive_com_error_cnt++;
		
		if (drive_com_error_cnt > 20)
		{
			if (drive_online == 1)
	  			record_fault(f_drv_com_error);
			drive_online = 0;
		}
	}

	if (drive_com_error_cnt > 0)
	{
		if (rt_hour != prev_com_hour)
			drive_com_error_cnt--;
	}
	prev_com_hour = rt_hour;

	if ((Drive_Type[cons[carnmb]] == 5) || (Drive_Type[cons[carnmb]] == 8))
		rcv_DSD_drv_com();
	else if ((Drive_Type[cons[carnmb]] == 2) || (Drive_Type[cons[carnmb]] == 3) || (Drive_Type[cons[carnmb]] == 6) ||
			 (Drive_Type[cons[carnmb]] == 10) || (Drive_Type[cons[carnmb]] == 11))
		rcv_HPV_drv_com();		// HPV or Quatro
	else if (Drive_Type[cons[carnmb]] == 12) 
		rcv_CBV_F5_drv_com();		// KEB Combivert F5
//	else if (Drive_Type[cons[carnmb]] == 14)
//		rcv_VDF_VL_drv_com();
	else if (Drive_Type[cons[carnmb]] == 15)
		rcv_L1000_drv_com();
/*
	if (fvars[fvdrvupd] == 1)
	{		// 15 msec update
		if ((drive_update_timer > 0) && (drv_com_seq == 0))
		{						   // run on the first interrupt count
			drive_update_timer = 0;
			drv_com_seq = 1;
			run_drive_com = 1;
			delay_com_cnt = 0;
		}
		else if ((drive_update_timer > 0) && (drv_com_seq == 1))
		{						  // skip the second interrupt count and run after delay
			delay_com_cnt++;
			if (delay_com_cnt > ((avg_loop_cnt+1)/2))
			{				 // run 5 msec after 10 msec interrupt
				delay_com_cnt = 0;
				run_drive_com = 1;
				drv_com_seq = 2;
				drive_update_timer = 0;
			}
		}
		else if ((drive_update_timer > 0) && (drv_com_seq == 2))
		{						  // skip the third interrupt count reset to run on the first
			drive_update_timer = 0;
			drv_com_seq = 0;
		}
	}
	else if (fvars[fvdrvupd] == 2)
	{		 // 20 msec update
		if (drive_update_timer > 0)
		{			 // Update drive every 20 msec
			drive_update_timer = 0;
			drv_com_seq++;
			if (drv_com_seq >= 2)
			{
				drv_com_seq = 0;
				run_drive_com = 1;
			}
		}
	}
	else
	{		// 10 msec update
		if (drive_update_timer > 0)
		{			 // Update drive every 10 msec
			drive_update_timer = 0;
			run_drive_com = 1;
		}
	}

	if (run_drive_com == 1)
	{
		if (drv_delay_cnt > 0)
		{
			drv_delay_cnt--;
			if (rdoutp(o_RUN) == 0)	 // car is not moving
				run_drive_com = 0;
		}
	}
*/
	if ((run_drive_com == 1) && (drv_tx_len == 0))
	{
		if (drv_ep_timer < 32000)
			drv_ep_timer ++;
		
		if (drv_pwr_up_delay < 32000)
			drv_pwr_up_delay ++;

#if (Traction == 1)

		if (((procf >= 2) && (procf <= 11)) || (procf == 20) || (procf == 21) || (procf == 23))
		{
			if (fvars[fvdrvupd] == 1)
				Vel_Step_Limit = (float)(Vel_Step_Clamp * 6.0);		// 15 msec increment
			else if (fvars[fvdrvupd] == 2)
				Vel_Step_Limit = (float)(Vel_Step_Clamp * 8.0);		// 20 msec increment
			else
				Vel_Step_Limit = (float)(Vel_Step_Clamp * 4.0);		// hpv on 10 msec increment
		}
		else
			Vel_Step_Limit = Vel_Step_FS;
		
		if ((rdoutp(o_RUN) == 1) && ((rdoutp(o_DNR) == 1) || (rdoutp(o_UP) == 1)))
		{
			if (Dmd_Velocity != Drv_Velocity)
			{
				if (Vel_Step_Limit > 0)
				{
					if (Dmd_Velocity > Drv_Velocity)
					{
						if ((Dmd_Velocity - Drv_Velocity) > Vel_Step_Limit)
						{
							clamp_spd_rate = 1;
							Drv_Velocity = Drv_Velocity + Vel_Step_Limit;
						}
						else
							Drv_Velocity = Dmd_Velocity;
					}
					else
					{
						if ((Drv_Velocity - Dmd_Velocity) > Vel_Step_Limit)
						{
							clamp_spd_rate = 1;
							Drv_Velocity = Drv_Velocity - Vel_Step_Limit;
						}
						else
							Drv_Velocity = Dmd_Velocity;
					}
				}
				else
					Drv_Velocity = Dmd_Velocity;
			}
			
		}
		else 
			Drv_Velocity = Dmd_Velocity;
		
		if (clamp_spd_rate == 1)
			statusf2 |= sf_Spd_Rate;
		else 
			statusf2 &= ~sf_Spd_Rate;

		if (Drv_Velocity > Vel_Max)
			Drv_Velocity = Vel_Max;
#endif


		if (Drive_Type[cons[carnmb]] != 5)
		{
			if ((Drive_Type[cons[carnmb]] == 12) && (fvars[fvkebdpytype] == 1))
			{
				drv_msg = 'x';		// message for KEB Combivert F5 Drive
			}
			else if (Drive_Type[cons[carnmb]] == 12)
			{
				if ((drive_online == 1) && (drv_pkt_status == 1)) // okay to look for message
					i = get_pkt_req(2,drive_port);
				else
					i = 0;				
					
				if (i != 0)	  // message available and car not moving
				{
					drv_pkt_status = 0;
					drv_msg = i;
				}
				else
				{
					drv_msg = 'x';		// message for KEB Combivert F5 Drive
					if (drv_init_seq >= 12)
						drv_pkt_status = 1;
				}
			}
			else
			{			// all other types of drives
				if ((drive_online == 1) && (drv_pkt_status == 1)) // okay to look for message
				{
					i = get_pkt_req(2,drive_port);
				}
				else
					i = 0;				
					
				if ((i != 0) && (rdoutp(o_RUN) == 0))	  // message available and car not moving
				{
					drv_pkt_status = 0;
					drv_msg = i;
					drv_delay_cnt = 4;
				}
				else
				{
					if (Drive_Type[cons[carnmb]] == 15)
					{
/*						if (rdoutp(o_RUN) == 1)
						{		// only send speed command while running
				   			drv_read_status = 0;
				   			drv_pkt_status = 1;
				   			drv_msg = 2;
						}
						else
						*/
						{
/*
							if (drv_read_status == 0)
							{					  // Alternate between reading status and sending command
								drv_msg = 1;
								drv_read_status = 1;
							}
							else
							*/
							{
								drv_read_status = 0;
								drv_pkt_status = 1;
								drv_msg = 2;
							}
						}
					}
					else if (Drive_Type[cons[carnmb]] == 14)
					{
						if (drv_read_status == 0)
						{					  // Alternate between reading status and sending command
							drv_msg = 1;
							drv_read_status = 1;
						}
						else
						{
							drv_read_status = 0;
							drv_pkt_status = 1;
							drv_msg = 2;
						}
					}
					else 
					{
						drv_pkt_status = 1;
						drv_read_status = 0;
						drv_msg = 2;		// run message
					}
				}
			}
		}
		else
			drv_msg = 0;

		if (EDMA.TCD[1].ACTIVE == 0)
		{
			drv_trm_fail_cnt = 0;
			if ((Drive_Type[cons[carnmb]] == 5) || (Drive_Type[cons[carnmb]] == 8))
				send_DSD_drv_msg();
			else if ((Drive_Type[cons[carnmb]] == 2) || (Drive_Type[cons[carnmb]] == 3) || (Drive_Type[cons[carnmb]] == 6) ||
					(Drive_Type[cons[carnmb]] == 10) || (Drive_Type[cons[carnmb]] == 11))
				send_HPV_drv_msg(drv_msg);		   // HPV or quatro
			else if ((Drive_Type[cons[carnmb]] == 12) && (fvars[fvkebdpytype] == 1))
				send_CBV_F5_drv_msg(drv_msg);
			else if (Drive_Type[cons[carnmb]] == 12)
	   			CBV_F5_drv_tx_ctl(drv_msg);
//			else if (Drive_Type[cons[carnmb]] == 14)
//			{
//				if ((drv_msg != 2) || (fvars[fvdrvbaud] != 3))
//					send_VDF_VL_drv_msg(drv_msg);
//			}
			else if (Drive_Type[cons[carnmb]] == 15)
				send_L1000_drv_msg(drv_msg);
			
			run_drive_com = 0;

		}
	}
}

// ***************************************************************
// ***************************************************************
// Transmit data to HPV drive
// ***************************************************************
// ***************************************************************


void send_HPV_drv_msg(int16 msg)
{
	uint16 i;
	char setup_msg;
	char sub_msg;
	uint32 chk_sum;
	uint8 valid_msg;

	setup_msg = (char)((msg & 0xf0) >> 4);
	sub_msg = (char)((msg & 0x0c) >> 2);
	msg = msg & 0x03;
	valid_msg = 0;
	switch (msg)
	{
		case 0:		 // invalid message
			break;
		case 1:		 // Request to read information from drive
			drv_trm_buf[0] = 0xfa;					// sync byte
			drv_trm_buf[1] = 1;
			switch (setup_msg)
			{
				case 1:
					drv_trm_buf[2] = 1;
					drv_trm_buf[3] = 0;
					drv_trm_buf[4] = 0;
					drv_trm_buf[5] = 0;
					drv_trm_buf[6] = 0;
					drv_trm_buf[7] = 0;
					drv_trm_buf[8] = sub_msg;
					break;
				case 2:
					drv_trm_buf[2] = 2;
					drv_trm_buf[3] = 0;
					drv_trm_buf[4] = 0;
					drv_trm_buf[5] = 0;
					drv_trm_buf[6] = 0;
					drv_trm_buf[7] = 0;
					drv_trm_buf[8] = sub_msg;
					break;
				case 3:
					drv_trm_buf[2] = 3;
					drv_trm_buf[3] = 0;
					drv_trm_buf[4] = 0;
					drv_trm_buf[5] = 0;
					drv_trm_buf[6] = 0;
					drv_trm_buf[7] = 0;
					drv_trm_buf[8] = sub_msg;
					break;
				case 4:
					drv_trm_buf[2] = 4;
					drv_trm_buf[3] = 0;
					drv_trm_buf[4] = 0;
					drv_trm_buf[5] = 0;
					drv_trm_buf[6] = 0;
					drv_trm_buf[7] = 0;
					drv_trm_buf[8] = sub_msg;
					break;
				case 5:
					drv_trm_buf[2] = 5;
					drv_trm_buf[3] = 0;
					drv_trm_buf[4] = 0;
					drv_trm_buf[5] = 0;
					drv_trm_buf[6] = 0;
					drv_trm_buf[7] = 0;
					drv_trm_buf[8] = sub_msg;
					break;
				case 6:
					drv_trm_buf[2] = 6;
					drv_trm_buf[3] = 0;
					drv_trm_buf[4] = 0;
					drv_trm_buf[5] = 0;
					drv_trm_buf[6] = 0;
					drv_trm_buf[7] = 0;
					drv_trm_buf[8] = (uint8)drv_rpar_addr;
					break;
				default:
					break;
			}
			valid_msg = 1;
			break;
		case 2:	  	// Run message to drive
#if (Traction == 1)
			if (dirf == 1)
				drv_tmp = (long)((Drv_Velocity/Vel_Max) * 8191.0);
			else if (dirf == 2)
				drv_tmp = 0 - (long)((Drv_Velocity/Vel_Max) * 8191.0);
			else
#endif
				drv_tmp = 0;
		
			drive_vel = (int16)(drv_tmp);
	
			drv_trm_buf[0] = 0xfa;					// sync byte
			drv_trm_buf[1] = 2;
			drv_trm_buf[2] = 0;
			drv_trm_buf[3] = 0;

			if ((rdoutp(o_RST) == 1)|| (drv_soft_reset == 1))		// RESET drive
			{
				drv_soft_reset = 0;
				drv_trm_buf[2] |= 2;				// set the reset bit
			}

			if ((Drive_Type[cons[carnmb]] == 10) || (Drive_Type[cons[carnmb]] == 11))
			{
				if (Drive_Type[cons[carnmb]] == 10) 
				{
					
					if ((rdoutp(o_FF) == 1) || (rdoutp(o_RUN) == 1))
						drv_trm_buf[3] = 4;				// Set full field bit
					else
						drv_trm_buf[3] = 2;			// set standby field bit
				}
				else
				{
					if ((rdoutp(o_RUN) == 1) && (rdoutp(o_MCC) == 1))
						drv_trm_buf[3] = 4;				// Set Drive Enable
				}
			}
			else
				drv_trm_buf[3] = 0;
			uwu.w = (uint16)drive_vel;
			drv_trm_buf[4] = uwu.b[hbw];
			drv_trm_buf[5] = uwu.b[lbw];
			drv_trm_buf[6] = 0;

			if (rdoutp(o_RUN) == 0)
				drv_pre_trq_seq = 0;


#if (Traction == 1)

	  #if (Simulator == 1)
		  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && 
	  #else
		  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && (lw_can_online == 1) &&
	  #endif
		  		((lw_status & c_lw_setup) != 0) && ((lw_status & c_lw_on) != 0) && 
		  								((lw_status & c_lw_error) == 0) && (lw_disable == 0))
		  	{						// load weigher setup and on
				if (drv_pre_trq_seq == 0)
				{
					pre_torque_cnt = 0;
					if ((rdoutp(o_RUN) == 1) && (pre_torque_set == 1))
					{
						drv_pre_trq_seq = 1;
						if ((fvars[fvtrqrmptim] > fvars[fvpat_del])	&& (fvars[fvpat_del] != 0))
							ramp_dly = fvars[fvpat_del]/2;
						else if (fvars[fvtrqrmptim] != 0)
							ramp_dly = fvars[fvtrqrmptim];
						else
							ramp_dly = 1;
						drv_trq_cmd = 0;
						if (fvars[fvtrqrmptim] == 0)
						{
							drv_trm_buf[2] |= 0x4;
							drv_trm_buf[6] = 1;
						}
						else
							drv_trm_buf[6] = 2;
					}
					uwu.w = 0;
				}
				else if (drv_pre_trq_seq == 1)
				{

					if (ramp_dly == 0)
						ramp_dly = 1;		// prevent divide by zero

					if ((drive_vel == 0) && (Dmd_Velocity == 0))
					{
						drv_trq = (int16)(((long)(torque) * 8191.0)/(10.0 * (1000 - fvars[fvbalance])));

						if (drv_trq != 0)
						{
							if (fvars[fvtrqrmptim] != 0)
							{
								if (abs(drv_trq_cmd + (drv_trq/ramp_dly)) > abs(drv_trq))
									drv_trq_cmd = drv_trq;
								else  
									drv_trq_cmd += drv_trq/ramp_dly;
							}
							else 
								drv_trq_cmd = drv_trq;
						}

						uwu.w = (uint16)drv_trq_cmd;
						if (fvars[fvtrqrmptim] == 0)
						{
							drv_trm_buf[2] |= 0x4;
							drv_trm_buf[6] = 1;
						}
						else
							drv_trm_buf[6] = 2;

						if ((pre_torque_set == 1) && (pre_torque_cnt < 10))
							pre_torque_cnt ++;
						if (rdoutp(o_RUN) == 1)
						{
							if ((pre_torque_cnt > 3) || (fvars[fvtrqrmptim] != 0))		   // delay the run signal to latch the pre-torque value
								drv_trm_buf[2] |= 1;				// set the run bit
						}
					}
					else
					{
						drv_trm_buf[2] |= 1;				// set the run bit
						pre_trq_hold = 0;
						uwu.w = (uint16)drv_trq_cmd;
						drv_pre_trq_seq = 2;
					}
				}
				else if (drv_pre_trq_seq == 2)
				{
					drv_trm_buf[2] |= 1;				// set the run bit
					pre_trq_hold++;
					if (pre_trq_hold >= 3)
						drv_pre_trq_seq = 3;
					else
					{
						if (fvars[fvtrqrmptim] == 0)
						{
							drv_trm_buf[2] |= 0x4;
							drv_trm_buf[6] = 1;
						}
						else
							drv_trm_buf[6] = 2;
					}
					uwu.w = (uint16)drv_trq_cmd;
				}
				else if (drv_pre_trq_seq == 3)
				{
					if (ramp_dly == 0)
						ramp_dly = 1;		// prevent divide by zero

					if ((pre_torque_set == 1) && (fvars[fvtrqrmptim] != 0))
					{

						if ((rdoutp(o_UP) == 0) && (rdoutp(o_DNR) == 0))
						{
							drv_trm_buf[6] = 2;

							if (pre_torque_cnt > 0)
								pre_torque_cnt --;
							else
							{
								if ((abs(drv_trq_cmd) - abs(drv_trq/ramp_dly)) > 0)
									drv_trq_cmd -= drv_trq/ramp_dly;
								else
								{
									drv_trq_cmd = 0;
									pre_torque_set = 0;
								}
							}
							uwu.w = (uint16)drv_trq_cmd;
						}
					}
					else
					{
						pre_torque_set = 0;
						drv_trq_cmd = 0;
						uwu.w = 0;
					}

					if (fvars[fvtrqrmptim] != 0)
						drv_trm_buf[6] = 2;

					if (rdoutp(o_RUN) == 1) 
						drv_trm_buf[2] |= 1;				// set the run bit
					else
						drv_pre_trq_seq = 0;
				}
				
			}
			else 
			{
				pre_torque_set = 0;
				uwu.w = 0;
				if  (rdoutp(o_RUN) == 1)
					drv_trm_buf[2] |= 1;				// set the run bit
			}

#else
			if  (rdoutp(o_RUN) == 1)
				drv_trm_buf[2] |= 1;				// set the run bit
			uwu.w = 0;
#endif

			if ((procf == 23) && (bkt_test_seq == 3) && ((bkt_run_cmd == 3) || (bkt_run_cmd == 4)))
			{
				if ((rdoutp(o_UP) == 0) && (rdoutp(o_DNR) == 0))
				{
					drv_trm_buf[2] &= ~0x1;		// Turn off the run command for brake test
//					if (Drive_Type[cons[carnmb]] == 11)
//						drv_trm_buf[3] &= ~0x4;		// Turn off the enable command for brake test
				}
			}

			drv_trm_buf[7] = uwu.b[hbw];
			drv_trm_buf[8] = uwu.b[lbw];
			uwu.b[lbw] = drv_trm_buf[2];
			uwu.b[hbw] = drv_trm_buf[3];
			drv_command = uwu.w;

			valid_msg = 1;
			break;
		case 3:		  // Set Variables in drive
			drv_trm_buf[0] = 0xfa;					// sync byte
			drv_trm_buf[1] = 3;
			drv_trm_buf[2] = setup_msg;
			drv_trm_buf[3] = 0;				// used
			uwu.w = (uint16)drv_wpar_addr;
			drv_trm_buf[4] = uwu.b[lbw];				// address
			uiu.dw = drv_wpar_data;
			drv_trm_buf[5] = uiu.b[hb];				// Msb Data
			drv_trm_buf[6] = uiu.b[mhb];				// Low Msb Data
			drv_trm_buf[7] = uiu.b[mlb];				// High Lsb Data
			drv_trm_buf[8] = uiu.b[lb];				// Lsb Data
			valid_msg = 1;
			break;
		default:
			break;
	}
	
	if (valid_msg == 1)
	{	
		chk_sum = 0;
		for (i=0;i<=8;i++)
		{
			chk_sum += drv_trm_buf[i];
//			drv_sendc(drv_trm_buf[i]);
		}
		chk_sum %= 256;
		drv_trm_buf[9] = (uint8)chk_sum;
//		drv_sendc((uint8)chk_sum);

//	 	drv_com_tx_start(10);
	 	drv_tx_len = 10;

		if (drive_trm_cnt < 32000)
			drive_trm_cnt++;
		else
			drive_trm_cnt = 0;
	}
}

// ***************************************************************
// ***************************************************************
// Transmit data to DSD drive
// ***************************************************************
// ***************************************************************

void send_DSD_drv_msg(void)
{
	int16 i;
	uint32 chk_sum;
	int16 valid_msg;

	valid_msg = 0;
	
#if (Traction == 1)
	if (dirf == 1)
		drv_tmp = (long)((Drv_Velocity/Vel_Max) * 32767.0);
	else if (dirf == 2)
		drv_tmp = 0 - (long)((Drv_Velocity/Vel_Max) * 32767.0);
	else
#endif
		drv_tmp = 0;

 	drive_vel = (int16)(drv_tmp);

	drv_trm_buf[0] = 0xfa;					// sync byte
	drv_trm_buf[1] = 5;						// sync byte
	drv_trm_buf[2] = 3;						// sync bits

	if (rdoutp(o_RUN) == 0)
		drv_pre_trq_seq = 0;

	if (dirf == 2)							// going down
		drv_trm_buf[2] |= 0x10;
	else if (dirf == 1)							// going up
		drv_trm_buf[2] |= 0x20;
	if ((rdoutp(o_FF) == 1) || (rdoutp(o_RUN) == 1))
		drv_trm_buf[2] |= 0x40;				// set the field force bit

	if ((rdoutp(o_RST) == 1)|| (drv_soft_reset == 1))		// RESET drive
	{
		drv_trm_buf[2] |= 8;
		drv_soft_reset = 0;
	}

#if (Traction == 1)
  #if (Simulator == 1)
  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && 
  #else
  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && (lw_can_online == 1) &&
  #endif
  		((lw_status & c_lw_setup) != 0) && ((lw_status & c_lw_on) != 0) && 
  								((lw_status & c_lw_error) == 0) && (lw_disable == 0))
  	{						// load weigher setup and on
		drv_trq = (int16)(((long)(torque) * 32767.0)/(10.0 * (1000 - fvars[fvbalance])));		
		if ((drive_vel == 0) && (Dmd_Velocity == 0))
		{	 // Pre-torque sequence
			switch (drv_pre_trq_seq)
			{
				case 0:			 // Set the torque value
					uwu.w = 0;
					if ((pre_torque_set == 1) && (rdoutp(o_RUN) == 1))
					{
						uwu.w = (uint16)drv_trq;
						drv_trm_buf[2] |= 0x80;				// bit 7 = pre torque
						drv_pre_trq_seq++;
					}
					break;
				case 1:			 // Set the torque value and set pretorque bit
					uwu.w = (uint16)drv_trq;
					drv_trm_buf[2] |= 0x80;				// bit 7 = pre torque
					drv_pre_trq_seq++;
					break;
				case 2:			// Set the torque value and set pretorque bit
					uwu.w = (uint16)drv_trq;
					drv_trm_buf[2] |= 0x80;				// bit 7 = pre torque
					drv_pre_trq_seq++;
					break;
				case 3:			// Clear the pretorque bit to latch the value in the drive
					uwu.w = (uint16)drv_trq;
					drv_pre_trq_seq++;
					break;
				case 4:			// Clear the pretorque bit to latch the value in the drive
					uwu.w = (uint16)drv_trq;
					drv_pre_trq_seq++;
					break;
				case 5:			// Clear the pretorque bit to latch the value in the drive
					uwu.w = (uint16)drv_trq;
					drv_pre_trq_seq++;
					break;
				case 6:		   // Clear the pretorque value
					uwu.w = 0x00;
					drv_pre_trq_seq++;
					break;
				case 7:		   // Clear the pretorque value
					uwu.w = 0x00;
					drv_pre_trq_seq++;
					break;
				case 8:		   // Clear the pretorque value
					if (rdoutp(o_RUN) == 1)
						drv_trm_buf[2] |= 0x04;				// set the run bit
					uwu.w = 0x00;
					drv_pre_trq_seq++;
					break;
				case 9:		   // Send the velocity	  (this state will also work as the hold zero velocity on stop)
					if (rdoutp(o_RUN) == 1)
						drv_trm_buf[2] |= 0x04;				// set the run bit
					uwu.w = (uint16)drive_vel;
					pre_torque_set = 0;
					break;

				default: 
					if (rdoutp(o_RUN) == 1)
						drv_trm_buf[2] |= 0x04;				// set the run bit
					uwu.w = (uint16)drive_vel;	
					pre_torque_set = 0;
					break;
			}
		}
		else
		{	 // Normal run after pre-torque
			if (rdoutp(o_RUN) == 1)
			{
				drv_trm_buf[2] |= 0x04;				// set the run bit
				drv_pre_trq_seq = 9;
			}
			uwu.w = (uint16)drive_vel;
			pre_torque_set = 0;
		}
	}
	else
#endif
	{	// Run with no pre-torque
		if (rdoutp(o_RUN) == 1)
			drv_trm_buf[2] |= 0x04;				// set the run bit
		uwu.w = (uint16)drive_vel;
		drv_pre_trq_seq = 0;
		pre_torque_set = 0;
	}

	drv_trm_buf[4] = uwu.b[hbw];					// demand velocity or pre torque
	drv_trm_buf[5] = uwu.b[lbw];
	drv_trm_buf[3] = 0;						// Velocity loop gain bit 0
	drv_trm_buf[6] = 0;						// PCDU command
	valid_msg = 1;
	
	if (valid_msg == 1)
	{	
		chk_sum = 0;
		for (i=0;i<=6;i++)
		{
			chk_sum += drv_trm_buf[i];
//			drv_sendc(drv_trm_buf[i]);
		}
		chk_sum %= 256;
		drv_trm_buf[7] = (uint8)chk_sum;
//		drv_sendc((uint8)chk_sum);
//	 	drv_com_tx_start(8);
	 	drv_tx_len = 8;

		if (drive_trm_cnt < 32000)
			drive_trm_cnt++;
		else
			drive_trm_cnt = 0;
	}
}

// ***************************************************************
// ***************************************************************
// Receive data from HPV drive comm
// ***************************************************************
// ***************************************************************

void rcv_HPV_drv_com (void)
{
  	int16 i,j,rbempt=0,endmess=0;
	int16 len,msg,setup_msg;
	int16 msg_len[4] = {0,10,16,10};

  	rbempt = drv_isrxempty();
  	while((rbempt == 0) && (endmess == 0))
  	{
	  	drmbuf[dmpnt] = drv_asigetc();
	  	dmpnt++;
	  	rbempt = drv_isrxempty();
	  	if (drmbuf[0] != 0xfa) 
	  	  	dmpnt = 0;
	  	else if ((drmbuf[0] == 0xfa) && (drmbuf[1] == 0xfa))
			dmpnt = 1;
		else if ((drmbuf[0] == 0xfa) && (drmbuf[1] != 0xfa))
	  	{
	  	  	if ((dmpnt >= 200) || ((dmpnt > 1) &&
	  	  	 	(dmpnt > msg_len[drmbuf[1]])))
	  	  	{
	  			rbempt = 1;
	  			dmpnt = 0;
	  	  	}
	  	  	if ((dmpnt > 1) && (drmbuf[1] < 4) && (dmpnt == msg_len[drmbuf[1]]))
	  	  	{		// Correct length packet
				
				msg = drmbuf[1];
				len = msg_len[msg];		// last byte of message is the checksum
				chk_sum = 0;
				for (i=0;i<=len-2;i++)
				{
					chk_sum += drmbuf[i];
					drv_rcv_buf[i] = drmbuf[i];
				}
				chk_sum %= 256;
				drv_rcv_buf[i] = drmbuf[i];
	  	  	    if(drmbuf[len-1] == chk_sum) // Good Packet
	  	  	    {
					setup_msg = drmbuf[2];
					switch(msg)
					{
						case 0:
							rcv_drive_msg = 1;
							break;
						case 1:
							if (setup_msg == 6)
							{		// return of read data command
								if (drmbuf[3] == 0)
								{	   // valid read
									uwu.b[lbw] = drmbuf[4];
									uwu.b[hbw] = 0;
									drv_rpar_ret_addr = uwu.w;
									uiu.b[hb] = drmbuf[5];
									uiu.b[mhb] = drmbuf[6];
									uiu.b[mlb] = drmbuf[7];
									uiu.b[lb] = drmbuf[8];
									drv_rpar_data = uiu.dw;
									if (drv_rpar_ret_addr == drv_rpar_addr)
										put_pkt_status(((setup_msg << 4) | msg),2,DRVCOM);
								}
							}
							rcv_drive_msg = 1;
							break;
						case 2:
							uiu.b[hb] = 0;
							uiu.b[mhb] = drmbuf[2];
							uiu.b[mlb] = drmbuf[3];
							uiu.b[lb] = drmbuf[4];
							drv_status = uiu.dw;
							if ((drmbuf[2] & 1) != 0)
								drive_fault = 1;
							else
								drive_fault = 0;

							if ((drmbuf[2] & 4) != 0)
								speed_ref_release = 1;
							else
								speed_ref_release = 0;

							if ((drmbuf[3] & 0x30) != 0)
								drive_com_error = 1;
							else 
								drive_com_error = 0;
							
							if (drmbuf[9] == 1)
							{
								uwu.b[hbw] = drmbuf[10];
								uwu.b[lbw] = drmbuf[11];
								drv_encoder_spd = uwu.w;
							}
							
							if (drmbuf[12] == 1)
							{
								uwu.b[hbw] = drmbuf[13];
								uwu.b[lbw] = drmbuf[14];
								drv_encoder_spd = uwu.w;
							}
							
							if (drmbuf[9] == 3)
							{
								uwu.b[hbw] = drmbuf[10];
								uwu.b[lbw] = drmbuf[11];
								drv_motor_torque = uwu.w;
							}
							
							if (drmbuf[12] == 3)
							{
								uwu.b[hbw] = drmbuf[13];
								uwu.b[lbw] = drmbuf[14];
								drv_motor_torque = uwu.w;
							}
							
							rcv_drive_msg = 1;
							break;
						case 3:
							if (setup_msg == 4)
							{			 // return of write data command
								if (drmbuf[3] == 0)
								{	   // valid write
									uwu.b[lbw] = drmbuf[4];
									uwu.b[hbw] = 0;
									drv_wpar_ret_addr = uwu.w;
									uiu.b[hb] = drmbuf[5];
									uiu.b[mhb] = drmbuf[6];
									uiu.b[mlb] = drmbuf[7];
									uiu.b[lb] = drmbuf[8];
									drv_wpar_ret_data = uiu.dw;
									if ((drv_wpar_ret_addr == drv_wpar_addr) && (drv_wpar_ret_data == drv_wpar_data))
									{
										put_pkt_status(((setup_msg << 4) | msg),2,DRVCOM);
										drv_rpar_addr = drv_wpar_ret_addr;
										drv_rpar_data = drv_wpar_ret_data;
									}
								}
							}
							rcv_drive_msg = 1;
							break;
						default:
							break;
					}

					if (drive_rcv_cnt < 32000)
	  				    drive_rcv_cnt++;
					else
						drive_rcv_cnt = 0;
					drmbuf[0] = 0;
					drmbuf[1] = 0;
	  				drmbuf[len-1] = 0;
	  				dmpnt = 0;
					endmess = 1;
	  	  	    }
	  	  	    else                                // Bad Packet
	  	  	    {
					drive_pkt_err_cnt++;
	  				j = 1;
	  				while((drmbuf[j] != 0xfa) && (j < dmpnt))
	  				{
	  					j++;
	  				}
	  				i = 0;
	  				while(j < dmpnt)        /* found another start character */
	  				{
	  					drmbuf[i] = drmbuf[j];
	  					i++;
	  					j++;
	  				}
	  				dmpnt = i;
	  	  	    }
	  	  	    rbempt = 1;
	  	  	}
	  	}
	  	else if (dmpnt >= 200)
	  	{
	  		dmpnt = 0;
	  		rbempt = 1;
	  	}
  	}
  	return;
}

			
// ***************************************************************
// ***************************************************************
// Receive data from DSD drive comm
// ***************************************************************
// ***************************************************************

void rcv_DSD_drv_com (void)
{
  	int16 i,j,rbempt=0,endmess=0;
 #define c_msg_len 11
 
   	rbempt = drv_isrxempty();
  	while((rbempt == 0) && (endmess == 0))
  	{
	  	drmbuf[dmpnt] = drv_asigetc();
	  	dmpnt++;
	  	rbempt = drv_isrxempty();
	  	if (drmbuf[0] != 0xfa) 
	  	  	dmpnt = 0;
	  	else if ((drmbuf[0] == 0xfa) && (drmbuf[1] == 0xfa))
			dmpnt = 1;
		else if ((drmbuf[0] == 0xfa) && (drmbuf[1] == 0x05))
	  	{
	  	  	if ((dmpnt >= 200) || ((dmpnt > 1) &&
	  	  	 	(dmpnt > c_msg_len)))
	  	  	{
	  			rbempt = 1;
	  			dmpnt = 0;
	  	  	}
	  	  	if ((dmpnt > 1) && (dmpnt == c_msg_len))
	  	  	{		// Correct length packet
				
				chk_sum = 0;
				for (i=0;i<=c_msg_len-2;i++)
				{
					chk_sum += drmbuf[i];
					drv_rcv_buf[i] = drmbuf[i];
				}
				chk_sum %= 256;
				drv_rcv_buf[i] = drmbuf[i];
	  	  	    if(drmbuf[c_msg_len-1] == chk_sum) // Good Packet
	  	  	    {
					rcv_drive_msg = 1;
					uiu.b[hb] = 0;
					uiu.b[mhb] = 0;
					uiu.b[mlb] = drmbuf[2];
					uiu.b[lb] = drmbuf[3];
					drv_status = uiu.dw;
					if ((drmbuf[2] & 0x80) != 0)
						drive_com_error = 1;
					else 
						drive_com_error = 0;

					if ((drmbuf[3] & 0x40) != 0)
						drive_fault = 1;
					else
						drive_fault = 0;

					if (drive_rcv_cnt < 32000)
	  				    drive_rcv_cnt++;
					else
						drive_rcv_cnt = 0;

					drmbuf[0] = 0;
					drmbuf[1] = 0;
	  				drmbuf[c_msg_len-1] = 0;
	  				dmpnt = 0;
					endmess = 1;
	  	  	    }
	  	  	    else                                // Bad Packet
	  	  	    {
					drive_pkt_err_cnt++;
	  				j = 1;
	  				while((drmbuf[j] != 0xfa) && (j < dmpnt))
	  				{
	  					j++;
	  				}
	  				i = 0;
	  				while(j < dmpnt)        /* found another start character */
	  				{
	  					drmbuf[i] = drmbuf[j];
	  					i++;
	  					j++;
	  				}
	  				dmpnt = i;
	  	  	    }
	  	  	    rbempt = 1;
	  	  	}
	  	}
 	  	else if (dmpnt >= 200)
	  	{
	  		dmpnt = 0;
	  		rbempt = 1;
	  	}
 	}
  	return;
}


unsigned char ismsgvalid (unsigned char msg)
{
	if ((msg == 3) || (msg == 6) || (msg == 0x10) || (msg == 0x5A) || (msg == 0xDA) || (msg == 0x83) || (msg == 0x86)|| (msg == 0x90))
		return 1;
	else
		return 0;
}

unsigned char get_msg_len(unsigned char msg, unsigned char nextbyte)
{
	switch (msg)
	{
		case 3:	return(5 + nextbyte);
		case 6: return 8;
		case 8: return 8;
#if (Traction == 1)
		case 0x10:	return 8;
#else
		case 0x10:	return 6;
#endif 
		case 0x5A:	return 17;
		case 0xDA:	return 14;
		case 0x83:  return 5;
		case 0x86:  return 5;
		case 0x90: 	return 5;
		default:	return 2;
	}
	return 2;
}


/* ----------------------------------------------------------------------------------------
	 Modbus protocol for Yaskawa L1000 AC drive
	 Transmit routine
	  drive_msg Table:
							00 no message
							01 Read Drive Status
	                        02 Write Run Command and Velocity
	    					03 Read data using drv_rpar_addr, drv_rpar_data
	                        06 Write data using drv_wpar_addr, drv_wpar_data 
							08 Loop Detection Operation
							10 Write multiple Registers 


		Command Data
		------------
		Drive Run Command	0001H 
			bit 0: Up Command
			bit 1: Down Command
			bit 2: External Fault (EF0)
			bit 3: Fault Reset
			bit 4: ComRef
			bit 5: ComCtrl
			bit 6: Multi-Function Input 3
			bit 7: Multi-Function Input 4
			bit 8: Multi-Function Input 5
			bit 9: Multi-Function Input 6
			bit A: Multi-Function Input 7
			bit B: Multi-Function Input 8
			bits C to F: Reserved

		Speed Reference 0002H
			Car speed in Frequency: Range 0.00 to 100.00%  Set by o1-03	= 1.

		Torque Limit 0004H
			0.1% units, signed

		Torque Compensation 0005H
			0.1% units, signed
				

   -----------------------------------------------------------------------------------------
*/

void send_L1000_drv_msg(int16 msg)
{
	uint16 len=0;
	int16 valid_msg = 0;

	switch (msg)
	{
		case 0:		// no message
			break;

		case 1:		// Send read drive status message
			len = 0;
			drv_trm_buf[len++] = 0x01;	// drive address
			drv_trm_buf[len++] = 0x03;	// Function for read register
			drv_trm_buf[len++] = 0x00;	// Address high byte
			drv_trm_buf[len++] = 0x20;	// Address low byte
			drv_trm_buf[len++] = 0x00;	// number of words high byte
			drv_trm_buf[len++] = 0x01;	// number of words low byte
			uwu.w = crc_chk(drv_trm_buf,len);
			drv_trm_buf[len++] = uwu.b[lbw];	// low byte of crc
			drv_trm_buf[len++] = uwu.b[hbw];	// high byte of crc
			drv_rstatus_addr = 0x0020;
			valid_msg = 1;
			break;

		case 2:		// Run message
			len = 0;
			drv_command = 0;
			

#if (Traction == 1)

	  #if (Simulator == 1)
		  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && 
	  #else
		  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && (lw_can_online == 1) &&
	  #endif
		  		((lw_status & c_lw_setup) != 0) && ((lw_status & c_lw_on) != 0) && 
		  								((lw_status & c_lw_error) == 0) && (lw_disable == 0))
		  	{						// load weigher setup and on
				if  ((rdoutp(o_RUN) == 0) || (pre_torque_set == 0))
					pre_torque_cnt = 0;
				
				if (pre_torque_set == 0)
				{
					if ((fvars[fvtrqrmptim] > fvars[fvpat_del]) && (fvars[fvpat_del] != 0))
						ramp_dly = fvars[fvpat_del]/2;
					else if (fvars[fvtrqrmptim] != 0)
						ramp_dly = fvars[fvtrqrmptim];
					else
						ramp_dly = 1;
					drv_trq_cmd = 0;
				}

				if (ramp_dly == 0)
					ramp_dly = 1;		// prevent divide by zero

				if ((torque != 0) && (drive_vel == 0) && (Dmd_Velocity == 0))
				{
					drv_trq = (int16)(((long)(torque) * 1000.0)/(1000 - fvars[fvbalance]));
					if (fvars[fvtrqrmptim] != 0)
					{
						if (abs(drv_trq_cmd + (drv_trq/ramp_dly)) > abs(drv_trq))
							drv_trq_cmd = drv_trq;
						else  
							drv_trq_cmd += drv_trq/ramp_dly;
					}
					else 
						drv_trq_cmd = drv_trq;

					if ((pre_torque_set == 1) && (pre_torque_cnt < 10))
						pre_torque_cnt ++;
					if (((rdoutp(o_UP) == 1) || (rdoutp(o_DNR) == 1))  &&
						((rdoutp(o_MCC) == 1) || (rdoutp(o_RUN) == 1)))
					{
						if ((pre_torque_cnt > 3) || (fvars[fvtrqrmptim] != 0))		   // delay the run signal to latch the pre-torque value
						{
							if (rdoutp(o_UP) == 1)
								drv_command |= 0x01;		// Up
							else 
								drv_command |= 0x02;		// Down
						}
					}
				}
				else
				{
					if ((pre_torque_set == 1) && (fvars[fvtrqrmptim] != 0))
					{
						if ((rdoutp(o_UP) == 0) && (rdoutp(o_DNR) == 0))
						{
							if (pre_torque_cnt > 0)
								pre_torque_cnt --;
							else
							{
								if ((abs(drv_trq_cmd) - abs(drv_trq/ramp_dly)) > 0)
									drv_trq_cmd -= drv_trq/ramp_dly;
								else
								{
									drv_trq_cmd = 0;
									pre_torque_set = 0;
								}
							}
						}	
					}
					else
					{
						pre_torque_set = 0;
						drv_trq_cmd = 0;
					}
					if ((rdoutp(o_MCC) == 1) || (rdoutp(o_RUN) == 1))
					{			
						if (rdoutp(o_UP) == 1)
							drv_command |= 0x01;		// Up
						else if (rdoutp(o_DNR) == 1)
							drv_command |= 0x02;		// Down
					}
				}
			}
			else 
			{
				drv_trq_cmd = 0;
				pre_torque_set = 0;
				if ((rdoutp(o_MCC) == 1) || (rdoutp(o_RUN) == 1))
				{			
					if (rdoutp(o_UP) == 1)
						drv_command |= 0x01;		// Up
					else if (rdoutp(o_DNR) == 1)
						drv_command |= 0x02;		// Down
				}
			}

#else
			drv_trq_cmd = 0;
			if ((rdoutp(o_MCC) == 1) || (rdoutp(o_RUN) == 1))
			{			
				if (rdoutp(o_UP) == 1)
					drv_command |= 0x01;		// Up
				else if (rdoutp(o_DNR) == 1)
					drv_command |= 0x02;		// Down
			}
#endif


#if (Traction == 1)
			if ((cons[loadw] & 2) != 0)
			{
				if (rdoutp(o_RUN) == 1)
				{
					if ((drive_vel == 0) && (Dmd_Velocity == 0))
						drv_pre_trq_seq ^= 0x01;		// have not started the run the alternate with pre-torque
					else
						drv_pre_trq_seq = 0;
				}
				else
					drv_pre_trq_seq = 0;
			}
			else
				drv_pre_trq_seq = 0;
#else
				drv_pre_trq_seq = 0;
#endif
			drv_trm_buf[len++] = 0x01;	// drive address
			drv_trm_buf[len++] = 0x5A;	// Special function for speed profile to get a special response
//			drv_trm_buf[len++] = 0x10;	// Function for multiple value write
			if ((drv_pre_trq_seq & 0x01) == 0)
			{			// send speed command
				drv_cmd_addr = 0x0001;		// runn command address
				uwu.w = (uint16)drv_cmd_addr;
				drv_trm_buf[len++] = uwu.b[hbw];	// Start Address high byte
				drv_trm_buf[len++] = uwu.b[lbw];	// Start Address low byte
	#if (Traction == 1)
				drv_trm_buf[len++] = 0x00;	// number of words high byte
				drv_trm_buf[len++] = 0x02;	// number of words low byte
				drv_trm_buf[len++] = 0x04;	// number of bytes
	#else
				drv_trm_buf[len++] = 0x00;	// number of words high byte
				drv_trm_buf[len++] = 0x01;	// number of words low byte
				drv_trm_buf[len++] = 0x02;	// number of bytes
	#endif

				uwu.w = (uint16)drv_command;
				drv_trm_buf[len++] = uwu.b[hbw];	// high byte of drive command
				drv_trm_buf[len++] = uwu.b[lbw];	// low byte of drive command

	#if (Traction == 1)
				drv_tmp = (long)((Drv_Velocity/Vel_Max) * 10000.0);
				drive_vel = (int16)(drv_tmp);
				uwu.w = (uint16)drive_vel;
				drv_trm_buf[len++] = uwu.b[hbw];	// high byte of drive velocity
				drv_trm_buf[len++] = uwu.b[lbw]; 	// low byte of drive velocity
	#endif
			}
			else		// send torque command
			{
				drv_cmd_addr = 0x0004;		// runn command address
				uwu.w = (uint16)drv_cmd_addr;
				drv_trm_buf[len++] = uwu.b[hbw];	// Start Address high byte
				drv_trm_buf[len++] = uwu.b[lbw];	// Start Address low byte
				drv_trm_buf[len++] = 0x00;	// number of words high byte
				drv_trm_buf[len++] = 0x02;	// number of words low byte
				drv_trm_buf[len++] = 0x04;	// number of bytes

				if (drv_trq_cmd >= 0)
					uwu.w = 1200;		 // torque limit
				else
					uwu.w = (int16)-1200;
				drv_trm_buf[len++] = uwu.b[hbw];	// high byte of torque limit
				drv_trm_buf[len++] = uwu.b[lbw];	// low byte of torque limit
				uwu.w = (uint16)drv_trq_cmd;
				drv_trm_buf[len++] = uwu.b[hbw];	// high byte of torque compensation
				drv_trm_buf[len++] = uwu.b[lbw];	// low byte of torque compensation
			}

			uwu.w = crc_chk(drv_trm_buf,len);
			drv_trm_buf[len++] = uwu.b[lbw];	// low byte of crc
			drv_trm_buf[len++] = uwu.b[hbw];	// high byte of crc
			valid_msg = 1;
			break;

		case 3:		// Send read request from parameter address and data
			len = 0;
			drv_trm_buf[len++] = 0x01;	// drive address
			drv_trm_buf[len++] = 0x03;	// Function for read register
			uwu.w = (uint16)drv_rpar_addr;
			drv_trm_buf[len++] = uwu.b[hbw];	// Address high byte
			drv_trm_buf[len++] = uwu.b[lbw];	// Address low byte
			drv_trm_buf[len++] = 0x00;	// number of words high byte
			drv_trm_buf[len++] = 0x01;	// number of words low byte
			uwu.w = crc_chk(drv_trm_buf,len);
			drv_trm_buf[len++] = uwu.b[lbw];	// low byte of crc
			drv_trm_buf[len++] = uwu.b[hbw];	// high byte of crc
			valid_msg = 1;
			break;

		case 6:		// Write One Drive Parameter
			len = 0;
/*
			drv_trm_buf[len++] = 0x01;	// drive address
			drv_trm_buf[len++] = 0x06;	// Function for multiple value write
			uwu.w = drv_wpar_addr;
			drv_trm_buf[len++] = uwu.b[hbw];	// Address high byte
			drv_trm_buf[len++] = uwu.b[lbw];	// Address low byte

			uwu.w = drv_wpar_data;
			drv_trm_buf[len++] = uwu.b[hbw];	// high byte of drive data
			drv_trm_buf[len++] = uwu.b[lbw];	// low byte of drive data
*/
			 // Use 0x10 if single write 0x06 is not allowed

			drv_trm_buf[len++] = 0x01;	// drive address
			drv_trm_buf[len++] = 0x10;	// Function for multiple value write
			uwu.w = (uint16)drv_wpar_addr;
			drv_trm_buf[len++] = uwu.b[hbw];	// Address high byte
			drv_trm_buf[len++] = uwu.b[lbw];	// Address low byte
			drv_trm_buf[len++] = 0x00;	// number of words high byte
			drv_trm_buf[len++] = 0x01;	// number of words low byte
			drv_trm_buf[len++] = 0x02;	// number of bytes
			uwu.w = (uint16)drv_wpar_data;
			drv_trm_buf[len++] = uwu.b[hbw];	// high byte of drive data
			drv_trm_buf[len++] = uwu.b[lbw];	// low byte of drive data


			uwu.w = crc_chk(drv_trm_buf,len);
			drv_trm_buf[len++] = uwu.b[lbw];	// low byte of crc
			drv_trm_buf[len++] = uwu.b[hbw];	// high byte of crc
			valid_msg = 1;
			break;
		
		case 8: // Loop Detection
			break;

		case 10: // Multiple write request
			break;

		default:
			break;
	}

	if (valid_msg == 1)
	{
//		for (i=0;i<len;i++)
//			drv_sendc(drv_trm_buf[i]);
//	 	drv_com_tx_start(len);
	 	drv_tx_len = len;

		if (drive_trm_cnt < 32000)
			drive_trm_cnt++;
		else
			drive_trm_cnt = 0;
	}
}
/*

	 Modbus protocol for Yaskawa L1000 AC drive
	 Receive routine
				
		Monitor Data
		------------	 
Register No. 		Contents
	0020H		Drive Status 1
					bit 0 During Run
					bit 1 During Reverse
					bit 2 Drive Ready
					bit 3 Fault
					bit 4 Data Setting Error
					bit 5 Multi-Function Relay Output (terminal M1-M2)
					bit 6 Multi-Function Relay Output (terminal M3-M4)
					bit 7 Multi-Function Relay Output (terminal M5-M6)
					bit 8 Multi-Function Photocoupler Output 1 (terminal P1-C1)
					bit 9 Multi-Function Photocoupler Output 2 (terminal P2-C2)
					bit A to bit D Reserved
					bit E ComRef status
					bit F ComCtrl status
	0021H		Fault Contents 1
					bit 0 Overcurrent (oC), Ground fault (GF)
					bit 1 Overvoltage (ov)
					bit 2 Drive Overload (oL2)
					bit 3 Overheat 1 (oH1)
					bit 4 Dynamic Braking Transistor Fault (rr)
					bit 5, 6 Reserved
					bit 7 EF, EF0, EF3 to EF8: External Fault
					bit 8 CPF....: Hardware Fault (includes oFx)
					bit 9 Motor Overload (oL1), Overtorque Detection 1/2 (oL3/oL4), Undertorque Detection 1/2 (UL3/UL4)
					bit A Encoder Disconnected (PGo), Option Card Hardware Fault (PGoH), Overspeed (oS), Excessive Speed Deviation (dEv)
					bit B Main Circuit Undervoltage (Uv)
					bit C Undervoltage (Uv1), Control Power Supply Undervoltage (Uv2), Soft Charge Circuit Fault (Uv3)
					bit D Output Phase Loss (LF), Input Phase Loss (PF)
					bit E MEMOBUS/Modbus Communication Error (CE), Option Communication Error (bUS)
					bit F Operator Connection Fault (oPr)
	0022H		Data Link Status
					bit 0 Writing data or switching motors
					bit 1 Reserved
					bit 2 Reserved
					bit 3 Upper or lower limit error
					bit 4 Data conformity error
					bit 5 Writing to EEPROM
					bit 6 to bit F Reserved
	0023H 		Speed Reference, <1>
	0024H 		Output Speed, <1>
	0025H 		Output Voltage Reference, 0.1 V units (units are determined by parameter H5-10)
	0026H 		Output Current, 0.1 A units
	0027H 		Output Power
	0028H 		Torque Reference
	0029H		Fault Contents 2
					bit 0 Reserved
					bit 1 Ground Fault (GF)
					bit 2 Input Phase Loss (PF)
					bit 3 Output Phase Loss (LF)
					bit 4 to bit F Reserved
	002AH		Alarm Contents1
					bit 0, 1 Reserved
					bit 2 Up/Down command Input Error (EF)
					bit 3 Drive Baseblock (bb)
					bit 4 Overtorque Detection 1 (oL3)
					bit 5 Heatsink Overheat (oH)
					bit 6 Overvoltage (ov)
					bit 7 Undervoltage (Uv)
					bit 8 Reserved
					bit 9 MEMOBUS/Modbus Communication Error (CE)
					bit A Option Communication Error (bUS)
					bit B Undertorque Detection 1/2 (UL3/UL4)
					bit C to E Reserved
					bit F Serial Communication Transmission Error (CALL)
	002BH		Input Terminal Status
					bit 0 Terminal S1 Closed
					bit 1 Terminal S2 Closed
					bit 2 Terminal S3 Closed
					bit 3 Terminal S4 Closed
					bit 4 Terminal S5 Closed
					bit 5 Terminal S6 Closed
					bit 6 Terminal S7 Closed
					bit 7 Terminal S8 Closed
					bit 8 to bit F Reserved
	002CH		Drive Status 2
					bit 0 During Run
					bit 1 Zero Speed
					bit 2 Speed Agree
					bit 3 User Speed Agree
					bit 4 Speed Detection 1
					bit 5 Speed Detection 2
					bit 6 Drive Ready
					bit 7 During Undervoltage
					bit 8 During Baseblock
					bit 9 Speed Reference from Operator Keypad
					bit A Up/Down command from Operator Keypad
					bit B Over/Undertorque 1, 2
					bit C Speed Reference Loss
					bit D During Fault Reset
					bit E Fault
					bit F Communication Timeout
	002DH		Output Terminal Status
					bit 0 Multi-Function Relay Output (terminal M1-M2)
					bit 1 Multi-Function Relay Output (terminal M3-M4)
					bit 2 Multi-Function Relay Output (terminal M5-M6)
					bit 3 Multi-Function Photocoupler Output 1 (terminal P1-C1)
					bit 4 Multi-Function Photocoupler Output 2 (terminal P2-C2)
					bit 3 to 6 Reserved
					bit 7 Fault Contact Output (terminal MA/MB-MC)
					bit 8 to F Reserved
	002EH-0030H Reserved
	0031H 		DC Bus Voltage, 1 Vdc units
	0032H 		Torque Reference (U1-09), 1% units
	0033H 		Reserved
	0034H 		Product Code 1 [ASCII], Product Type (LA for L1000A)
	0035H 		Product Code 2 [ASCII], Region Code
	0036H-003CH Reserved
	003DH		Communications Error Contents <3>
					bit 0 CRC Error
					bit 1 Data Length Error
					bit 2 Reserved
					bit 3 Parity Error
					bit 4 Overrun Error
					bit 5 Framing Error
					bit 6 Timeout
					bit 7 to bit F Reserved
	003EH		Output Speed r/min <4>
	003FH 		Output Speed 0.01% units
	0040H-004AH Used for various monitors U1-xx
	004BH		Drive status (U1-12)
					bit 0 During Run
					bit 1 During Zero Speed
					bit 2 During Reverse Run
					bit 3 During Fault Reset Signal Input
					bit 4 During Speed Agree
					bit 5 Drive Ready
					bit 6 Alarm
					bit 7 Fault
					bit 8 During Operation Error (oPE....)
					bit 9 During Momentary Power Loss
					bit A Motor 2 Selected
					bit B Reserved
					bit C Reserved
					bit D Reserved
					bit E ComRef status, NetRef status
					bit F ComCtrl status, NetCtrl status
	004CH-007EH Used for various monitors U1-...., U4-...., and U6-..... Refer to U: Monitors on page 347 for parameter details.
	007FH 		Alarm Code, Refer to Alarm Register Contents on page 381 for alarm codes.
	0080H-0097H Used for monitors U2-...., U3-..... Refer to U: Monitors 
	0098H 		High Word of Accumulated Operation Time Monitor, 10h units (U4-01)
	0099H 		Low Word of Accumulated Operation Time Monitor, 1h units (U4-01)
	009AH 		High Word of Cooling Fan Operation Time Monitor (U4-03)
	009BH 		Low Word of Cooling Fan Operation Time Monitor (U4-03)
	009CH-00AAH Reserved
	00ABH 		Drive Rated Current <2>
	00ACH		Speed Feedback (U1-05)	r/min units <4>
	00ADH 		Speed Feedback 0.01% units
	00AEH,00AFH Reserved
	00B0H 		Option Code Connected to CN5-A
				Register contains ASCII code of the option card.
					DI-A3 = 0x01
					DO-A3 = 0x02
					AO-A3 = 0x04
					PG-B3 = 0x11
					PG-X3 = 0x12
					PG-F3 = 0x21
					PG-E3 = 0x22
					Communication Option: Register contains ASCII code of 1st and 3rd digit of the option card type number.
					Example: Register value is 5353H for “SS” if a SI-S3 option card is installed.
	00B1H 		Reserved
	00B2H 		Option Code Connected to CN5-B
	00B3H 		Option Code Connected to CN5-C
	00B4H 		Reserved
	00B5H 		Output Speed After Soft Start	(U1-16) r/min units <4>
	00B6H 		Output Speed After Soft Start 0.01% units
	00B7H		Speed Reference	r/min units <4>
	00B8H 		Speed Reference 0.01% units
	00B9H-00BEH Reserved
	00BFH 		Lists the last to digits of operation error code oPE.....
	00C0H		Fault contents 3
					bit 1 Undervoltage (Uv1)
					bit 2 Control Power Supply Undervoltage (Uv2)
					bit 3 Soft Charge Circuit Fault (Uv3)
					bit 4 Reserved
					bit 5 Ground Fault (GF)
					bit 6 Overcurrent (oC)
					bit 7 Overvoltage (ov)
					bit 8 Heatsink Overheat (oH)
					bit 9 Heatsink Overheat (oH1)
					bit A Motor Overload (oL1)
					bit B Drive Overload (oL2)
					bit C Overtorque Detection 1 (oL3)
					bit D Overtorque Detection 2 (oL4)
					bit E Dynamic Braking Transistor Fault (rr)
					bit F Reserved
	00C1H		Fault contents 4
					bit 0 External Fault at input terminal S3 (EF3)
					bit 1 External Fault at input terminal S4 (EF4)
					bit 2 External Fault at input terminal S5 (EF5)
					bit 3 External Fault at input terminal S6 (EF6)
					bit 4 External Fault at input terminal S7 (EF7)
					bit 5 External Fault at input terminal S8 (EF8)
					bit 6 Reserved
					bit 7 Overspeed (os)
					bit 8 Excessive Speed Deviation (dEv)
					bit 9 Encoder Disconnected (PGo)
					bit A Input Phase Loss (PF)
					bit B Output Phase Loss (LF)
					bit C Reserved
					bit D Digital Operator Connection Fault (oPr)
					bit E EEPROM Write Error (Err)
					bit F Reserved
	00C2H		Fault contents 5
					bit 0 MEMOBUS/Modbus Communication Error (CE)
					bit 1 Option Communication Error (bUS)
					bit 2, 3 Reserved
					bit 4 Control Fault (CF)
					bit 5 Position Lock Error (SvE)
					bit 6 Option External Fault (EF0)
					bit 7 Reserved
					bit 8 Undertorque Detection 1 (UL3)
					bit 9 Undertorque Detection 2 (UL4)
					bit A to E Reserved
					bit F Hardware Fault (includes oFx)
	00C3H		Fault contents 6
					bit 0 Reserved
					bit 1 Z Pulse Fault Detection (dv1)
					bit 2 Z Pulse Noise Fault Detection (dv2)
					bit 3 Inversion Detection (dv3)
					bit 4 Inversion Prevention Detection (dv4)
					bit 5 Current Imbalance (LF2)
					bit 6 Pullout Detection (STo)
					bit 7 Option Card Hardware Fault (PGoH)
					bit 8 to F Reserved
	00C4H		Fault contents 7
					bit 0 to 4 Reserved
					bit 5 Current Offset Fault (CoF)
					bit 6 to B Reserved
					bit C Output Voltage Detection Fault (voF)
					bit D Reserved
					bit E Braking Transistor Overload Fault (boL)
					bit F Reserved
	00C5H
					bit 0 to 3 Reserved
					bit 4 Overacceleration (dv6)
					bit 5 to F Reserved
	00C6H,00C7H Reserved
	00C8H		Alarm contents 2
					bit 0 Undervoltage (Uv)
					bit 1 Overvoltage (ov)
					bit 2 Heatsink Overheat (oH)
					bit 3 Reserved
					bit 4 Overtorque 1 (oL3)
					bit 5 Overtorque 2 (oL4)
					bit 6 Up/Down commands Input Error (EF)
					bit 7 Drive Baseblock (bb)
					bit 8 External Fault 3, input terminal S3 (EF3)
					bit 9 External Fault 4, input terminal S4 (EF4)
					bit A External Fault 5, input terminal S5 (EF5)
					bit B External Fault 6, input terminal S6 (EF6)
					bit C External Fault 7, input terminal S7 (EF7)
					bit D External Fault 8, input terminal S8 (EF8)
					bit E Reserved
					bit F Overspeed (oS)

	00C9H		Alarm contents 3
					bit 0 Excessive Speed Deviation (dEv)
					bit 1 Encoder Disconnected (PGo)
					bit 2 Digital Operator Connection Fault (oPr)
					bit 3 MEMOBUS/Modbus Communication Error (CE)
					bit 4 Option Communication Error (bUS)
					bit 5 Serial Communication Transmission Error (CALL)
					bit 6 Motor Overload (oL1)
					bit 7 Drive Overload (oL2)
					bit 8 Reserved
					bit 9 Option Card External fault (EF0)
					bit A Motor 2 Switch command input during run (rUn)
					bit B Reserved
					bit C Serial Communication Transmission Error (CALL)
					bit D Undertorque Detection 1 (UL3)
					bit E Undertorque Detection 2 (UL4)
					bit F MEMOBUS/Modbus Test Mode Fault (SE)
	00CAH		Alarm contents 4
					bit 0 to 9 Reserved
					bit A Encoder Disconnected (PGo)
					bit B to F Reserved
	00CBH		Alarm Contents 5
					bit 0 to 2 Reserved
					bit 3 High Current Alarm (HCA)
					bit 4 Cooling Fan Maintenance Time (LT-1)
					bit 5 Soft Charge Bypass Relay Maintenance Time (LT-2)
					bit 6 Reserved
					bit 7 SI-S EEPROM Error (EEP)
					bit 8 to 9 Reserved
					bit A Safe Disable Input (HbbF)
					bit B Safe Disable Input (Hbb)
					bit C to F Reserved
	00CCH		Alarm Contents 6
					bit 0 Output Voltage Detection Fault (VoF)
					bit 1 Reserved
					bit 2 Capacitor Maintenance Time (LT-3)
					bit 3 IGBT Maintenance Time (50%) (LT-4)
					bit 4 Braking Transistor Overload Fault (boL)
					bit 5 to F Reserved
	00CDH-00CFH Reserved
	00D0H		CPF Contents 1
					bit 0, 1 Reserved
					bit 2 A/D Conversion Error (CPF02)
					bit 3 PWM Data Fault (CPF03)
					bit 4, 5 Reserved
					bit 6 EEPROM Memory Data Error (CPF06)
					bit 7 Terminal Board Connection Error (CPF07)
					bit 8 EEPROM Serial Communications Fault (CPF08)
					bit 9, A Reserved
					bit B RAM Fault (CPF11)
					bit C FLASH Memory Fault (CPF12)
					bit D Watchdog Circuit Exception (CPF13)
					bit E Control Circuit Fault (CPF14)
					bit F Reserved
	00D1H		CPF Contents 2
					bit 0 Clock Fault (CPF16)
					bit 1 Timing Fault (CPF17)
					bit 2 Control Circuit Fault (CPF18)
					bit 3 Control Circuit Fault (CPF19)
					bit 4 Hardware fault at power up (CPF20)
					bit 5 Hardware fault at communication start up (CPF21)
					bit 6 A/D Conversion Fault (CPF22)
					bit 7 PWM Feedback Fault (CPF23)
					bit 8 Drive Unit Signal Fault (CPF24)
					bit 9 Terminal board is not properly connected. (CPF25)
					bit A ASIC BB Circuit Error (CPF26)
					bit B ASIC PWM Setting Register Error (CPF27)
					bit C ASIC PWM Pattern Error (CPF28)
					bit D ASIC On-delay Error (CPF29)
					bit E ASIC BBON Error (CPF30)
					bit F ASIC Code Error (CPF31)
	00D2H
					bit 0 ASIC Start-up Error (CPF32)
					bit 1 Watch-dog Error (CPF33)
					bit 2 ASIC Power/Clock Error (CPF34)
					bit 3 External A/D Converter Error (CPF35)
					bit 4 to F Reserved
	00D3H-00D7H oFA0x Contents (CN5-A)
	00D8H		oFA0x Contents (CN5-A)
					bit 0 Option Compatibility Error (oFA00)
					bit 1 Option not properly connected (oFA01)
					bit 2 Same type of option card already connected (oFA02)
					bit 3, 4 Reserved
					bit 5 A/D Conversion Error (oFA05)
					bit 6 Option Response Error (oFA06)
					bit 7 to F Reserved
	00D9H		oFA1x Contents (CN5-A)
					bit 0 Option RAM Fault (oFA10)
					bit 1 Option Operation Mode Fault (SLMOD) (oFA11)
					bit 2 Drive Receive CRC Error (oFA12)
					bit 3 Drive Receive Frame Error (oFA13)
					bit 4 Drive Receive Abort Error (oFA14)
					bit 5 Option Receive CRC Error (oFA15)
					bit 6 Option Receive Frame Error (oFA16)
					bit 7 Option Receive Abort Error (oFA17)
					bit 8 to F Reserved
	00DAH-00DBH Reserved
	00DBH		oFA3x Contents (CN5-A)
					bit 0 Comm. ID Error (oFA30)
					bit 1 Model Code Error (oFA31)
					bit 2 Sumcheck Error (oFA32)
					bit 3 Comm. option timeout waiting for response (oFA33)
					bit 4 MEMOBUS Timeout (oFA34)
					bit 5 Drive timeout waiting for response (oFA35)
					bit 6 CI Check Error (oFA36)
					bit 7 Drive timeout waiting for response (oFA37)
					bit 8 Control Command Selection Error (oFA38)
					bit 9 Drive timeout waiting for response (oFA39)
					bit A Control Response Selection 1 Error (oFA40)
					bit B Drive timeout waiting for response (oFA41)
					bit C Control Response Selection 2 Error (oFA42)
					bit D Control Response Selection Error (oFA43)
					bit E, F Reserved
	00DCH		oFb0x Contents (CN5-B)
					bit 0 Option compatibility error (oFb00)
					bit 1 Option not properly connected (oFb01)
					bit 2 Same type of option card already connected (oFb02)
					bit 3, 4 Reserved
					bit 5 A/D Conversion Fault (oFb05)
					bit 6 Option Response Error (oFb06)
					bit 7 to F Reserved
	00DDH		oFb1x Contents (CN5-B)
					bit 0 Option RAM Fault (oFb10)
					bit 1 Option Operation Mode Fault (SLMOD) (oFb11)
					bit 2 Drive Receive CRC Error (oFb12)
					bit 3 Drive Receive Frame Error (oFb13)
					bit 4 Drive Receive Abort Error (oFb14)
					bit 5 Option Receive CRC Error (oFb15)
					bit 6 Option Receive Frame Error (oFb16)
					bit 7 Option Receive Abort Error (oFb17)
					bit 8 to F Reserved
	00DEH-00DFH Reserved
	00E0H		oFb3x Contents (CN5-B)
					bit 0 Comm. ID Error (oFb30)
					bit 1 Model Code Error (oFb31)
					bit 2 Sumcheck Error (oFb32)
					bit 3 Comm. option timeout waiting for response (oFb33)
					bit 4 MEMOBUS Timeout (oFb34)
					bit 5 Drive timeout waiting for response (oFb35)
					bit 6 CI Check Error (oFb36)
					bit 7 Drive timeout waiting for response (oFb37)
					bit 8 Control Command Selection Error (oFb38)
					bit 9 Drive timeout waiting for response (oFb39)
					bit A Control Response Selection 1 Error (oFb40)
					bit B Drive timeout waiting for response (oFb41)
					bit C Control Response Selection 2 Error (oFb42)
					bit D Control Response Selection Error (oFb43)
					bit E, F Reserved
	00E1H		oFC0x Contents (CN5-C)
					bit 0 Option compatibility error (oFC00)
					bit 1 Option not properly connected (oFC01)
					bit 2 Same type of option card already connected (oFC02)
					bit 3, 4 Reserved
					bit 5 A/D Conversion Fault (oFC05)
					bit 6 Option Response Error (oFC06)
					bit 7 to F Reserved
	00E2H		oFC1x Contents (CN5-C)
					bit 0 Option RAM Fault (oFC10)
					bit 1 Option Operation Mode Fault (SLMOD) (oFC11)
					bit 2 Drive Receive CRC Error (oFC12)
					bit 3 Drive Receive Frame Error (oFC13)
					bit 4 Drive Receive Abort Error (oFC14)
					bit 5 Option Receive CRC Error (oFC15)
					bit 6 Option Receive Frame Error (oFC16)
					bit 7 Option Receive Abort Error (oFC17)
					bit 8 to F Reserved
	00E3H,00E4H Reserved
	00E5H		oFC3x Contents (CN5-C)
					bit 0 Comm. ID Error (oFC30)
					bit 1 Model Code Error (oFC31)
					bit 2 Sumcheck Error (oFC32)
					bit 3 Comm. option timeout waiting for response (oFC33)
					bit 4 MEMOBUS Timeout (oFC34)
					bit 5 Drive timeout waiting for response (oFC35)
					bit 6 CI Check Error (oFC36)
					bit 7 Drive timeout waiting for response (oFC37)
					bit 8 Control Command Selection Error (oFC38)
					bit 9 Drive timeout waiting for response (oFC39)
					bit A Control Response Selection 1 Error (oFC40)
					bit B Drive timeout waiting for response (oFC41)
					bit C Control Response Selection 2 Error (oFC42)
					bit D Control Response Selection Error (oFC43)
					bit E, F Reserved
	00E6H-00FFH Reserved

Error Code		Error Name					Cause
01H				Function Code Error			• Attempted to set a function code from a PLC other than 03H, 08H, and 10H.
02H				Register Number Error		• A register number specified in the command message does not exist.
											• Attempted to send a broadcast message using other register numbers than 0001H or 0002H.
03H				Bit Count Error				• Read data or write data is greater than 16 bits. Invalid command message quantity.	
											• In a write message, the “Number of Data Items” contained within the message does not equal 
											  twice the amount of data words (i.e., the total of Data 1+ Data 2, etc.).
21H				Data Setting Error			• Control data or parameter write data is outside the allowable setting range.
											• Attempted to write a contradictory parameter setting.
22H				Write Mode Error			• During run, the user attempted to write a parameter that cannot be written to during run.
											• During an EEPROM memory data error (CPF06), the master attempted to write to a parameter 
											  other than A1-00 to A1-05, E1-03, or o2-04.
											• Attempted to write to read-only data.
23H				DC Bus Undervoltage 		• During an undervoltage situation, the master attempted to write to parameters that cannot 
				  Write Error				  be written to during undervoltage.
24H				Write Error During 			• Master attempted writing to the drive while the drive was processing parameter data.
				  Parameter Process

 ---------------------------------------------------------------
 
 */

void rcv_L1000_drv_com (void)
{
  	int16 i,j,rbempt=0,endmess=0;
	int16 msg;
	uint16 msg_len;
	int16 valid_msg;
	uint16 crc_reg = 0;

	if (Drv_Tranbuf_Empty == 1)
	{		// Wait for transmit to be finished

	  	rbempt = drv_isrxempty();
	  	while((rbempt == 0) && (endmess == 0))
	  	{
		  	drmbuf[dmpnt] = drv_asigetc();
		  	dmpnt++;
		  	rbempt = drv_isrxempty();
			valid_msg = ismsgvalid(drmbuf[1]);
		  	if (drmbuf[0] != 0x01) 
		  	  	dmpnt = 0;
		  	else if ((drmbuf[0] == 0x01) && (valid_msg == 1))
		  	{
				msg_len = get_msg_len(drmbuf[1],drmbuf[2]); 
		  	  	if ((dmpnt >= 200) || ((dmpnt > 1) &&
		  	  	 	(dmpnt > msg_len)))
		  	  	{
		  			rbempt = 1;
		  			dmpnt = 0;
		  	  	}
		  	  	if ((dmpnt > 1) && (valid_msg == 1) && (dmpnt == msg_len))
		  	  	{		// Correct length packet
					
					msg = drmbuf[1];
					for (i=0;i<=msg_len;i++)
						drv_rcv_buf[i] = drmbuf[i];
					crc_reg = crc_chk(drv_rcv_buf,msg_len-2);
					uwu.b[lbw] = drmbuf[msg_len-2];
					uwu.b[hbw] = drmbuf[msg_len-1];
		  	  	    if(crc_reg == uwu.w) // Good Packet
		  	  	    {
						switch(msg)
						{
							case 3:		// read register
								if (drv_rcv_buf[2] <= 2)
								{
									uiu.b[hb] = 0;
									uiu.b[mhb] = 0;
									uiu.b[mlb] = drv_rcv_buf[3];
									uiu.b[lb] = drv_rcv_buf[4];
									if (drv_rstatus_addr == 0x0020)
									{
										drv_status = uiu.dw;
										drv_rstatus_addr = 0;
									}
									else 
									{
										put_pkt_status(3,2,drive_port);
										drv_rpar_data = uiu.dw;	
									}
									rcv_drive_msg = 1;
								}
								break;
							case 6:		// response for write one register
								uwu.b[hbw] = drv_rcv_buf[2];
								uwu.b[lbw] = drv_rcv_buf[3];
								if (uwu.w == drv_wpar_addr)
									rcv_drive_msg = 1;
								break;
							case 0x10:	// response for write multiple registers
								uwu.b[hbw] = drv_rcv_buf[2];
								uwu.b[lbw] = drv_rcv_buf[3];
								if (uwu.w == drv_cmd_addr)
									rcv_drive_msg = 1;
								break;
							case 0x5A:	// response for special write multiple registers
								read_reg_status = drv_rcv_buf[2];
								drv_status = 0;
								if ((read_reg_status & 0x01) != 0)
								{
									uiu.b[hb] = 0;
									uiu.b[mhb] = 0;
									uiu.b[mlb] = drv_rcv_buf[3];
									uiu.b[lb] = drv_rcv_buf[4];
									drv_status = uiu.dw;
								}
								if ((read_reg_status & 0x02) != 0)
								{
									uiu.dw = drv_status;
									uiu.b[hb] = drv_rcv_buf[5];
									uiu.b[mhb] = drv_rcv_buf[6];
									drv_status = uiu.dw;
								}
								if ((read_reg_status & 0x04) != 0)
								{
									uwu.b[hbw] = drv_rcv_buf[7];
									uwu.b[lbw] = drv_rcv_buf[8];
									drv_motor_frequency = uwu.w;
								}
								if ((read_reg_status & 0x08) != 0)
								{
									uwu.b[hbw] = drv_rcv_buf[9];
									uwu.b[lbw] = drv_rcv_buf[10];
									drv_motor_current = uwu.w;
								}
								uwu.b[hbw] = drv_rcv_buf[11];
								uwu.b[lbw] = drv_rcv_buf[12];
								if (uwu.w == drv_cmd_addr)
									rcv_drive_msg = 1;
								break;
							case 0xDA:	// Error response for special write multiple registers
								read_reg_status = drv_rcv_buf[2];
								if ((read_reg_status & 0x01) != 0)
								{
									uiu.b[hb] = 0;
									uiu.b[mhb] = 0;
									uiu.b[mlb] = drv_rcv_buf[3];
									uiu.b[lb] = drv_rcv_buf[4];
									drv_status = uiu.dw;
								}
								if ((read_reg_status & 0x02) != 0)
								{
									uiu.dw = drv_status;
									uiu.b[hb] = drv_rcv_buf[5];
									uiu.b[mhb] = drv_rcv_buf[6];
									drv_status = uiu.dw;
								}
								if ((read_reg_status & 0x04) != 0)
								{
									uwu.b[hbw] = drv_rcv_buf[7];
									uwu.b[lbw] = drv_rcv_buf[8];
									drv_motor_frequency = uwu.w;
								}
								if ((read_reg_status & 0x08) != 0)
								{
									uwu.b[hbw] = drv_rcv_buf[9];
									uwu.b[lbw] = drv_rcv_buf[10];
									drv_motor_current = uwu.w;
								}
								drv_error = drv_rcv_buf[11];
								rcv_drive_msg = 1;
								break;
							case 0x83:
								drv_error = drv_rcv_buf[2];
								rcv_drive_msg = 1;
								break;
							case 0x86:
								drv_error = drv_rcv_buf[2];
								rcv_drive_msg = 1;
								break;
							case 0x90:
								drv_error = drv_rcv_buf[2];
								rcv_drive_msg = 1;
								break;
							default:
								break;
						}

						if (drive_rcv_cnt < 32000)
		  				    drive_rcv_cnt++;
						else
							drive_rcv_cnt = 0;
						drmbuf[0] = 0;
						drmbuf[1] = 0;
		  				drmbuf[msg_len-1] = 0;
		  				drmbuf[msg_len-2] = 0;
		  				dmpnt = 0;
						endmess = 1;
		  	  	    }
		  	  	    else                                // Bad Packet
		  	  	    {
						drive_pkt_err_cnt++;
		  				j = 1;
		  				while((drmbuf[j] != 0x01) && (j < dmpnt))
		  				{
		  					j++;
		  				}
		  				i = 0;
		  				while(j < dmpnt)        /* found another start character */
		  				{
		  					drmbuf[i] = drmbuf[j];
		  					i++;
		  					j++;
		  				}
		  				dmpnt = i;
		  	  	    }
		  	  	    rbempt = 1;
		  	  	}
				else if (dmpnt > msg_len)
					dmpnt = 0;
			}
			else if (dmpnt > 2)
				dmpnt = 0;
	  	}
	}
  	return;
}


// ----------------------
// Calculate CRC checksum
// ----------------------

uint16 crc_chk (unsigned char* cm_buff, uint16 length)
{
	uint16 crc_reg = 0xffff;
	int16 j;

	while(length > 0)
	{  	
		length--;
		crc_reg ^= *cm_buff++;
		for(j=0;j<8;j++)
		{
			if ((crc_reg & 0x01) != 0)			// bit0 == 1
				crc_reg = (crc_reg >> 1) ^0xA001;
			else
				crc_reg = crc_reg >> 1;

		}

	}
	return (crc_reg);

}


void CBV_F5_drv_tx_ctl (int16 drv_msg)
{
	if (drv_init_dly <= 0)
	{
		if ((drv_init_seq < 12) && (drv_nak == 1))
		{
			drv_init_seq = 0;
			drv_wpar_addr = 0;
			drv_wpar_data = 0;
			drv_wpar_set = 0;
			drv_msg = 'x';
			drv_init_dly = 50;
		}
		else
		{
			switch(drv_init_seq)
			{
				case 0:	// Wait for drive to be ready
					drv_init_seq = 0;
					drv_wpar_addr = 0;
					drv_wpar_data = 0;
					drv_wpar_set = 0;
					drv_msg = 'x';
					drv_init_dly = 50;
					break;

				case 1:	// Set drive to Run mode LF.3 = 1 (conf mode)
					drv_wpar_addr = 0x383;
					drv_wpar_data = 1;
					drv_wpar_set = 0xff;
					drv_msg = 'g';
					drv_init_seq++;
					drv_init_dly = 10;
					break;

				case 2: // turn auto reset to off  LF.5 = 0
					drv_wpar_addr = 0x385;
					drv_wpar_data = 0;
					drv_wpar_set = 0xff;
					drv_msg = 'g';
					drv_init_seq++;
					drv_init_dly = 5;
					break;

				case 3: // Set reference splitting	us.35 0x1A3
					drv_wpar_addr = 0x1A3;
					if (fvars[fvdrvupd] == 1)
						drv_wpar_data = 30;			// 15 msec set splitting to 30msec
					else if (fvars[fvdrvupd] == 2)
						drv_wpar_data = 40;			// 20 msec set splitting at 40 msec
					else
						drv_wpar_data = 20;			// 10 msec set splitting at 20 msec
					drv_wpar_set = 0xff;
					drv_msg = 'g';
					drv_init_seq++;
					drv_init_dly = 5;
					break;

				case 4: // Set E.Bus stop mode to Error	Pn.05 = 0
					drv_wpar_addr = 0x405;
					drv_wpar_data = 0;
					drv_wpar_set = 0xff;
					drv_msg = 'g';
					drv_init_seq++;
					drv_init_dly = 5;
					break;

				case 5: // Set Watchdog time to 0.200s just for power up US.29 = 20
					drv_wpar_addr = 0x019D;
					drv_wpar_data = 101; 		// Changed from 12 to 101 
					drv_wpar_set = 0xff;
					drv_msg = 'g';
					drv_init_seq++;
					drv_init_dly = 5;
					break;

				case 6:	// Set drive to Run mode LF.03 = 0
					drv_wpar_addr = 0x383;
					drv_wpar_data = 0;
					drv_wpar_set = 0xff;
					drv_msg = 'g';
					drv_init_seq++;
					drv_init_dly = 10;
					break;

				case 7: // Set Watchdog time to 0.05s Pn.06 = 10
					drv_wpar_addr = 0x406;
					drv_wpar_data = 50; 				// Changed from 10 to 50
					drv_wpar_set = 0xff;
					drv_msg = 'g';
					drv_init_seq++;
					drv_init_dly = 5;
					break;

			 	case 8:	// Set HSP5 Watchdog Time Sy.09 = .10s
					drv_wpar_addr = 0x9;
					drv_wpar_data = 50;	 				// Change from 10 to 50
					drv_wpar_set = 0xff;
					drv_msg = 'g';
					drv_init_seq++;
					drv_init_dly = 10;
					break;

				case 9:	// Read drive mode US.29 
					drv_rpar_addr = 0x019D;
					drv_rpar_data = 50;
					drv_rpar_set = 0x01;
					drv_msg = 'G';
					drv_init_seq++;
					drv_init_dly = 10;
					break;

				case 10:	// Read drive mode LF.04 
					if (drv_rpar_data != 101)
					{
						drv_init_seq = 0;
						drv_rpar_addr = 0;
						drv_rpar_data = 0;
						drv_rpar_set = 0;
						drv_wpar_addr = 0;
						drv_wpar_data = 0;
						drv_wpar_set = 0;
						drv_msg = 'x';
						drv_init_dly = 150;
					}
					else
					{
						drv_rpar_addr = 0x384;
						drv_rpar_data = 100;
						drv_rpar_set = 0x01;
						drv_msg = 'G';
						drv_mode = 100;
						drv_init_seq++;
						drv_init_dly = 10;
					}
					break;

				case 11: // Wait for drive to send drive mode
					if (drv_rpar_data != 100)
					{
						if((drv_rpar_data >= 1) && (drv_rpar_data <= 4))
						{
							drv_mode = (int16)drv_rpar_data;
//							set_KEB_par_min_max();		delete KEB parameters
							drv_init_seq++;
							drv_msg = 0;
							drv_wpar_addr = 0;
							drv_wpar_data = 0;
							drv_wpar_set = 0;
						}
					}
					else
					{
						if (drv_mode != 0)
						{
							drv_rpar_addr = 0x384;
							drv_rpar_data = 100;
							drv_rpar_set = 0x01;
							drv_msg = 'G';
							drv_mode = 0;
							drv_init_dly = 10;
						}
						else
						{
							drv_init_seq++;
						}
					}
					break;

		 		case 12:	// Init finished
					drv_wpar_addr = 0;
					drv_wpar_data = 0;
					drv_wpar_set = 0;
					drv_msg = 'x';
					drv_init_seq++;
					drv_init_dly = 0;
					drv_soft_reset = 1;		// reset e.bus error
					break;

				case 13: // Transmit run or data request message
					if ((drv_msg != 'x') && (drv_msg != 'G') && (drv_msg != 'g'))
					{									  //    Use g for writes
						drv_msg = 'x';
						drv_wpar_addr = 0;
						drv_wpar_data = 0;
						drv_wpar_set = 0;
					}
					drv_init_dly = 0;
					break;			

				default:
					drv_wpar_addr = 0;
					drv_wpar_data = 0;
					drv_wpar_set = 0;
					drv_msg = 'x';
					drv_init_seq = 12;
					drv_init_dly = 0;
					break;

			}
		}
	}
	else
	{
		drv_init_dly--;
		drv_wpar_addr = 0;
		drv_wpar_data = 0;
		drv_wpar_set = 0;
		drv_msg = 'x';
	}

	send_CBV_F5_drv_msg(drv_msg);					// KEB Combivert F5
}

int16 Addr_Tx_Cnt = 0;

// Comm to KEB Combivert F5 drive
//   Service 49 (49 + 47h = 'x')  
//           di02 = set digital input
//				bit 0: Control release
//				bit 1: Reset
//				bit 3: Forward
//				bit 4: Reverse
//           op03 = command speed (-4000 to +4000)
//           an17 = Analog2 Y offset (pre torque)	 (-100.0% to +100.0%)
//           ---- = not used

void send_CBV_F5_drv_msg (int16 service_msg)
{
	int16 i;
	uint32 chk_sum;
	int16 valid_msg;
	int16 chksum_start=1;
	int16 len=0;
	uint8 hex_to_ascii[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
#if (Traction == 1)
	float motor_rpm_mult;
#endif

	valid_msg = 0;

	
	switch (service_msg)
	{
		case 'g':		 // Write extended parameter  (use g for writes so the packet request can be placed in the queue and the read/write information is automatic)	 
			drv_nak = 0;
			drv_ack = 0;
			// Note: KEB drive uses capital G for both read and write
			i = 0;
			if (Addr_Tx_Cnt == 0)
			{
				drv_trm_buf[i++] = EOT;
				drv_trm_buf[i++] = '0';
				drv_trm_buf[i++] = '1';
				chksum_start = 4;
			}
			else
				chksum_start = 1;
			drv_trm_buf[i++] = STX;					// Start flag
			drv_trm_buf[i++] = 'G';				// Message 0 + 47h	  (lower case g only for GALaxy side)

 			if ((Invoke_ID >= 15) || (Invoke_ID == 0))
				Invoke_ID = 1;
			else
				Invoke_ID++;
			
			rx_Invoke_ID = hex_to_ascii[Invoke_ID & 0x0f];;
			tx_data_ID = rx_Invoke_ID;
			rx_data_ID = rx_Invoke_ID;
 			drv_trm_buf[i++] = rx_Invoke_ID; 

			uwu.w = (uint16)drv_wpar_addr;
			drv_trm_buf[i++] = hex_to_ascii[(uwu.b[hbw] >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[uwu.b[hbw] & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(uwu.b[lbw] >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[uwu.b[lbw] & 0x0f];

			
			uiu.dw = drv_wpar_data;

			drv_trm_buf[i++] = hex_to_ascii[(uiu.b[hb] >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[uiu.b[hb] & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(uiu.b[mhb] >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[uiu.b[mhb] & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(uiu.b[mlb] >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[uiu.b[mlb] & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(uiu.b[lb] >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[uiu.b[lb] & 0x0f];

			drv_trm_buf[i++] = hex_to_ascii[(drv_wpar_set >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[drv_wpar_set & 0x0f];

			drv_trm_buf[i++] = ETX;
			len = i;
			valid_msg = 1;
			 
			break;

		case 'G':		 // Read parameter data (upper case G use for read only because it is easier to queue)	 
			drv_eot = 0;
			// Note: KEB drive uses capital G for both read and write
			i = 0;
			if ((Addr_Tx_Cnt == 0) || (service_msg == 'G'))
			{
				drv_trm_buf[i++] = EOT;
				drv_trm_buf[i++] = '0';
				drv_trm_buf[i++] = '1';
				chksum_start = 3;
			}
			else
				chksum_start = 0;
			
//			drv_trm_buf[i++] = STX;					// Start flag
			drv_trm_buf[i++] = 'G';				// Message 0 + 47h

 			if ((Invoke_ID >= 15) || (Invoke_ID == 0))
				Invoke_ID = 1;
			else
				Invoke_ID++;
			
			rx_Invoke_ID = hex_to_ascii[Invoke_ID & 0x0f];;
			rx_data_ID = rx_Invoke_ID;
 			drv_trm_buf[i++] = rx_Invoke_ID; 

			uwu.w = (uint16)drv_rpar_addr;
			drv_trm_buf[i++] = hex_to_ascii[(uwu.b[hbw] >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[uwu.b[hbw] & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(uwu.b[lbw] >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[uwu.b[lbw] & 0x0f];

			drv_trm_buf[i++] = hex_to_ascii[(drv_rpar_set >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[drv_rpar_set & 0x0f];

			drv_trm_buf[i++] = ENQ;
			len = i;
			valid_msg = 1;
			 
			break;

		case 'x':	  	// Run message to drive (4x16 bit)
			drv_nak = 0;
			drv_ack = 0;
#if (Traction == 1)
				// 32000 = 4000 rpm for geared, 32000 = 500 rpm for gearless
			if ((fvars[fvmtrrpm] < 10) && (fvars[fvenctype] != 4))  // if tape feedback then use motor rpm for drive
			{		// use the encoder rpm
				if (fvars[fvencrpm] > 5000)
					motor_rpm_mult = 8.0;
				else
					motor_rpm_mult = 64.0;

				if (dirf == 1)
					drv_tmp = (long)((Drv_Velocity/Vel_Max) * (((float)fvars[fvencrpm]/10.0) * motor_rpm_mult * ((float)fvars[fvdrvmult]/1000.0)));
				else if (dirf == 2)
					drv_tmp = 0 - (long)((Drv_Velocity/Vel_Max) * (((float)fvars[fvencrpm]/10.0) * motor_rpm_mult * ((float)fvars[fvdrvmult]/1000.0)));
				else
			  		drv_tmp = 0;
			}
			else
			{
				if (fvars[fvmtrrpm] > 5000)
					motor_rpm_mult = 0.8;		   // multiplier of 10 already in the number
				else
					motor_rpm_mult = 6.4;

				if (dirf == 1)
				{
					if (fvars[fvkebdpytype] == 1)
						drv_tmp = (long)((Drv_Velocity * 50.0)/(Pulses_per_Inch));	// demand velocity to the drive in 1/10 fpm (vel/ppi)*((60*10)/12)
					else
						drv_tmp = (long)((Drv_Velocity/Vel_Max) * ((float)(fvars[fvmtrrpm]) * motor_rpm_mult * ((float)fvars[fvdrvmult]/1000.0)));
				}
				else if (dirf == 2)
				{
					if (fvars[fvkebdpytype] == 1)
						drv_tmp = 0 - (long)((Drv_Velocity * 50.0)/(Pulses_per_Inch));	// demand velocity to the drive in 1/10 fpm (vel/ppi)*((60*10)/12)
					else
						drv_tmp = 0 - (long)((Drv_Velocity/Vel_Max) * ((float)(fvars[fvmtrrpm]) * motor_rpm_mult * ((float)fvars[fvdrvmult]/1000.0)));
				}
				else
			  		drv_tmp = 0;
			}
#else
	  		drv_tmp = 0;
#endif		
			drive_vel = (int16)(drv_tmp);

			// need to place in temp buff and convert to ascii
	
 			if ((Invoke_ID >= 15) || (Invoke_ID == 0))
				Invoke_ID = 1;
			else
				Invoke_ID++;
			

			drv_command = 0;						// digital input bits

			if ((rdoutp(o_RST) == 1)|| (drv_soft_reset == 1))		// RESET drive
			{
				drv_soft_reset = 0;
				drv_command |= 2;				// set the reset bit
			}


			if  (rdoutp(o_RUN) == 1) 
				drv_command |= 1;				// set the run bit
			else
				drv_pre_trq_seq = 0;
			
			if ((procf == 23) && (bkt_test_seq == 3) && ((bkt_run_cmd == 3) || (bkt_run_cmd == 4)))
			{
				if ((rdoutp(o_UP) == 0) && (rdoutp(o_DNR) == 0))
					drv_command &= ~0x1;		// Turn off the run command for brake test
			}


#if (Traction == 1)

	  #if (Simulator == 1)
		  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && 
	  #else
		  	if (((cons[loadw] & 2) != 0) && (fvars[fvtorque] != 0) && (lw_can_online == 1) &&
	  #endif
		  		((lw_status & c_lw_setup) != 0) && ((lw_status & c_lw_on) != 0) && 
		  								((lw_status & c_lw_error) == 0) && (lw_disable == 0))
		  	{						// load weigher setup and on
				if  ((rdoutp(o_RUN) == 0) || (pre_torque_set == 0))
					pre_torque_cnt = 0;
				
				if (pre_torque_set == 0)
				{
//					if ((fvars[fvtrqrmptim] > fvars[fvpat_del]) && (fvars[fvpat_del] != 0))
//						ramp_dly = fvars[fvpat_del]/2;
//					else if (fvars[fvtrqrmptim] != 0)
//						ramp_dly = fvars[fvtrqrmptim];
//					else
//						ramp_dly = 1;
					drv_trq_cmd = 0;
				}

//				if (ramp_dly == 0)
//					ramp_dly = 1;		// prevent divide by zero

				if (/*(torque != 0) && */ (drv_pre_trq_seq == 0) && (drive_vel == 0) && (Dmd_Velocity == 0))
				{
					drv_trq = (int16)(((long)(torque) * 100.0)/(10.0 * (1000 - fvars[fvbalance])));
//					if (fvars[fvtrqrmptim] != 0)
//					{
//						if (abs(drv_trq_cmd + (drv_trq/ramp_dly)) > abs(drv_trq))
//							drv_trq_cmd = drv_trq;
//						else  
//							drv_trq_cmd += drv_trq/ramp_dly;
//					}
//					else 
						drv_trq_cmd = drv_trq;

					if ((pre_torque_set == 1) && (pre_torque_cnt <= 6))		// 10 
						pre_torque_cnt ++;
					
					if ((rdoutp(o_UP) == 1) || (rdoutp(o_DNR) == 1))
					{
						if ((pre_torque_cnt >= 6) /* || (fvars[fvtrqrmptim] != 0 */)		   // delay 8 the run signal to latch the pre-torque value
						{
							drv_pre_trq_seq = 1;
							if (rdoutp(o_UP) == 1)
								drv_command |= 4;				// set the FORWARD bit
							else
								drv_command |= 8;				// set the REVERSE bit
						}
					}
				}
				else
				{
//					if ((pre_torque_set == 1) && (fvars[fvtrqrmptim] != 0))
//					{
//						if ((rdoutp(o_UP) == 0) && (rdoutp(o_DNR) == 0))
//						{
//							if (pre_torque_cnt > 0)
//								pre_torque_cnt --;
//							else
//							{
//								if ((abs(drv_trq_cmd) - abs(drv_trq/ramp_dly)) > 0)
//									drv_trq_cmd -= drv_trq/ramp_dly;
//								else
//								{
//									drv_trq_cmd = 0;
//									pre_torque_set = 0;
//								}
//							}
//						}	
//					}
//					else 
					if (drive_vel != 0)
					{
						pre_torque_set = 0;
						drv_trq_cmd = 0;
					}
					if (rdoutp(o_UP) == 1)
						drv_command |= 4;				// set the FORWARD bit
					else if (rdoutp(o_DNR) == 1)
						drv_command |= 8;				// set the REVERSE bit
				}
			}
			else 
			{
				drv_trq_cmd = 0;
				pre_torque_set = 0;
				if (rdoutp(o_UP) == 1)
					drv_command |= 4;				// set the FORWARD bit
				else if (rdoutp(o_DNR) == 1)
					drv_command |= 8;				// set the REVERSE bit
			}

#else
			drv_trq_cmd = 0;
		  	if (rdoutp(o_UP) == 1)
		  		drv_command |= 4;				// set the FORWARD bit
		  	else if (rdoutp(o_DNR) == 1)
		  		drv_command |= 8;				// set the REVERSE bit
#endif
			
			i = 0;
			if (Addr_Tx_Cnt == 0) 
			{
				drv_trm_buf[i++] = EOT;
				drv_trm_buf[i++] = '0';
				drv_trm_buf[i++] = '1';
				chksum_start = 4;
			}
			else
				chksum_start = 1;
			
			drv_trm_buf[i++] = STX;					// Start flag
			drv_trm_buf[i++] = 'x';				// Message 49 + 47h

			rx_Invoke_ID = hex_to_ascii[Invoke_ID & 0x0f];;
 			drv_trm_buf[i++] = rx_Invoke_ID; 

			drv_trm_buf[i++] = hex_to_ascii[(drv_command >> 12) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(drv_command >> 8) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(drv_command >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[drv_command & 0x0f];

			drv_trm_buf[i++] = hex_to_ascii[(drive_vel >> 12) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(drive_vel >> 8) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(drive_vel >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[drive_vel & 0x0f];

			drv_trm_buf[i++] = hex_to_ascii[(drv_trq_cmd >> 12) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(drv_trq_cmd >> 8) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[(drv_trq_cmd >> 4) & 0x0f];
			drv_trm_buf[i++] = hex_to_ascii[drv_trq_cmd & 0x0f];

			drv_trm_buf[i++] = '0';
			drv_trm_buf[i++] = '0';
			drv_trm_buf[i++] = '0';
			drv_trm_buf[i++] = '0';

			drv_trm_buf[i++] = ETX;
			len = i;
			valid_msg = 1;
			break;
		default:
			valid_msg = 0;
			break;
	}
	
	if (valid_msg == 1)
	{	

		Addr_Tx_Cnt++;
		if (Addr_Tx_Cnt > 10)
			Addr_Tx_Cnt = 0;
//		for (i=0;i<chksum_start;i++)
//			drv_sendc(drv_trm_buf[i]);
		chk_sum = 0;
		for (i=chksum_start;i<len;i++)
		{
			chk_sum ^= drv_trm_buf[i];
//			drv_sendc(drv_trm_buf[i]);
		}
		chk_sum %= 256;
		if (chk_sum < 0x1f)
			chk_sum += 0x20;
		drv_trm_buf[len++] = (uint8)chk_sum;
//		drv_sendc((uint8)chk_sum);
//		drv_com_tx_start(len);
	 	drv_tx_len = len;

		if (drive_trm_cnt < 32000)
			drive_trm_cnt++;
		else
			drive_trm_cnt = 0;
	}
}

uint8 ascii_to_hex(uint8 value)
{
	uint8 x = 0;
	if ((value >= 'A') && (value <= 'F'))
		x = (value - 'A') + 10;
	else if ((value >= 'a') && (value <= 'f'))
		x = (value - 'a') + 10;
	else if ((value >= '0') && (value <= '9'))
		x = value - '0';

	return x;
}


// Comm from KEB Combivert F5 Drive
//   Service 49 (49 + 47h = 'x')
//           Sys51 = status word low
//				bit 0: 1=control release given
//              bit 1: 1=an error occured
//				bit 2: 0=stop, 1=run
//				bit 3: 0=forward, 1=reverse
// 				bit 4-6: Indication of Current parameter set
//				bit 7: Free
//              bit 8: 1=fast stop active
//				bit 9: Sercos bus synchronize reached
//				bit 10: homing procedure complete
//				bit 11: Position reached
//				bit 12-13: 0=Synchron running, 1=positioning, 3=coutouring mode
//				bit 14-15: reserved 
//           Sys42 = status word high (input and output status)
//           ru09 = encoder speed
//           ru15 = phase current

int16 invoke_id_ck = 0;

void rcv_CBV_F5_drv_com (void)
{
  	int16 i,j,rbempt=0,endmess=0;
	int16 len,msg;
	int16 msg_len[4] = {22,10,16,10};

	uiu.dw = 0;
  	rbempt = drv_isrxempty();
  	while((rbempt == 0) && (endmess == 0))
  	{
	  	drmbuf[dmpnt] = (uint8)(drv_asigetc() & 0x7F);
	  	dmpnt++;
	  	rbempt = drv_isrxempty();
	  	if (drmbuf[0] != STX)
		{
			if (drmbuf[0] == ACK)
			{
				if (drv_response_code[0] == rx_data_ID)	  // set from previous read
				{
					drv_ack = 1;
					put_pkt_status('g',2,drive_port);
					if (drive_rcv_cnt < 32000)
		  			    drive_rcv_cnt++;
					else
						drive_rcv_cnt = 0;
				}
			}
			else if (drmbuf[0] == NAK)
			{
				if (drv_response_code[0] == rx_data_ID)
				{
					drv_nak = 1;
					drv_error = ascii_to_hex(drv_response_code[1]);
				}
			}
			else if (drmbuf[0] == EOT)
			{
				if (drv_response_code[0] == rx_data_ID)
				{
					drv_eot = 1;
					drv_error = ascii_to_hex(drv_response_code[1]);
				}
			}
			else if (drmbuf[0] == rx_data_ID)
			{
				drv_response_code[0] = drmbuf[0];
//				if (drive_rcv_cnt < 32000)
//	  			    drive_rcv_cnt++;
//				else
//					drive_rcv_cnt = 0;
			}
			else
			{
				drv_response_code[1] = drmbuf[0];
			}
	  	  	dmpnt = 0;
		}
		else if ((drmbuf[1] == 'x') || (drmbuf[1] == 'G'))
		{
			if (drmbuf[1] == 'x')
				len = 22;
			else 
				len = 13;

			if (drmbuf[len - 2] == ETX)
		  	{
				if (invoke_id_ck == 0)
				{
					if (drmbuf[2] == rx_Invoke_ID)
						invoke_id_ck = 1;
				}
		  	  	if ((dmpnt >= 200) || ((dmpnt > 1) &&
		  	  	 	(dmpnt > len)))
		  	  	{
					invoke_id_ck = 0;
		  			rbempt = 1;
		  			dmpnt = 0;
		  	  	}
		  	  	if ((dmpnt > 1) && (dmpnt == len))
		  	  	{		// Correct length packet
					msg = drmbuf[1];
					chk_sum = 0;
					for (i=1;i<len-1;i++)
					{
						chk_sum ^= drmbuf[i];
						// need to convert to hex from ascii
						drv_rcv_buf[i] = drmbuf[i];
					}
					chk_sum %= 256;
					if (chk_sum < 0x1f)
						chk_sum += 0x20;
					drv_rcv_buf[i] = drmbuf[i];
		  	  	    if ((drmbuf[len-1] == chk_sum)/* && (invoke_id_ck == 1)*/) // Good Packet
		  	  	    {
						switch(msg)
						{
							case 'G':			// Receive parameter data
								uiu.b[hb] = ascii_to_hex(drv_rcv_buf[3]) << 4;
								uiu.b[hb] |= ascii_to_hex(drv_rcv_buf[4]);
								uiu.b[mhb] = ascii_to_hex(drv_rcv_buf[5]) << 4;
								uiu.b[mhb] |= ascii_to_hex(drv_rcv_buf[6]);
								uiu.b[mlb] = ascii_to_hex(drv_rcv_buf[7]) << 4;
								uiu.b[mlb] |= ascii_to_hex(drv_rcv_buf[8]);
								uiu.b[lb] = ascii_to_hex(drv_rcv_buf[9]) << 4;
								uiu.b[lb] |= ascii_to_hex(drv_rcv_buf[10]);
								if (rx_data_ID == drv_rcv_buf[2])
								{
									drv_rpar_data = uiu.dw;
									drive_com_error = 0;
									rcv_drive_msg = 1;
									put_pkt_status('G',2,drive_port);
									if (drv_half_duplex == 1)
									{	// hal duplex
										// Enable transmitter
										DRVtxEn;
									}
								}
								break;

							case 'x':
								
								if (fvars[fvkebdpytype] == 1)
								{
									uwu.b[hbw] = ascii_to_hex(drv_rcv_buf[3]) << 4;
									uwu.b[hbw] |= ascii_to_hex(drv_rcv_buf[4]);
									uwu.b[lbw] = ascii_to_hex(drv_rcv_buf[5]) << 4;
									uwu.b[lbw] |= ascii_to_hex(drv_rcv_buf[6]);

									drv_status = uwu.w;
									drive_fault = 0;

									uwu.b[hbw] = ascii_to_hex(drv_rcv_buf[7]) << 4;
									uwu.b[hbw] |= ascii_to_hex(drv_rcv_buf[8]);
									uwu.b[lbw] = ascii_to_hex(drv_rcv_buf[9]) << 4;
									uwu.b[lbw] |= ascii_to_hex(drv_rcv_buf[10]);
									drv_encoder_spd = uwu.w;


									uwu.b[hbw] = ascii_to_hex(drv_rcv_buf[11]) << 4;
									uwu.b[hbw] |= ascii_to_hex(drv_rcv_buf[12]);
									uwu.b[lbw] = ascii_to_hex(drv_rcv_buf[13]) << 4;
									uwu.b[lbw] |= ascii_to_hex(drv_rcv_buf[14]);
									drv_motor_torque = uwu.w;


									uwu.b[hbw] = ascii_to_hex(drv_rcv_buf[15]) << 4;
									uwu.b[hbw] |= ascii_to_hex(drv_rcv_buf[16]);
									uwu.b[lbw] = ascii_to_hex(drv_rcv_buf[17]) << 4;
									uwu.b[lbw] |= ascii_to_hex(drv_rcv_buf[18]);
									drv_motor_current = uwu.w;
								}
								else
								{
									uiu.b[mlb] = ascii_to_hex(drv_rcv_buf[3]) << 4;
									uiu.b[mlb] |= ascii_to_hex(drv_rcv_buf[4]);
									uiu.b[lb] = ascii_to_hex(drv_rcv_buf[5]) << 4;
									uiu.b[lb] |= ascii_to_hex(drv_rcv_buf[6]);

									uiu.b[hb] = ascii_to_hex(drv_rcv_buf[7]) << 4;
									uiu.b[hb] |= ascii_to_hex(drv_rcv_buf[8]);
									uiu.b[mhb] = ascii_to_hex(drv_rcv_buf[9]) << 4;
									uiu.b[mhb] |= ascii_to_hex(drv_rcv_buf[10]);
									drv_status = uiu.dw;

									if ((drv_status & 0x02) != 0)
										drive_fault = 1;
									else
										drive_fault = 0;

									uwu.b[hbw] = ascii_to_hex(drv_rcv_buf[11]) << 4;
									uwu.b[hbw] |= ascii_to_hex(drv_rcv_buf[12]);
									uwu.b[lbw] = ascii_to_hex(drv_rcv_buf[13]) << 4;
									uwu.b[lbw] |= ascii_to_hex(drv_rcv_buf[14]);
									drv_encoder_spd = uwu.w;


									uwu.b[hbw] = ascii_to_hex(drv_rcv_buf[15]) << 4;
									uwu.b[hbw] |= ascii_to_hex(drv_rcv_buf[16]);
									uwu.b[lbw] = ascii_to_hex(drv_rcv_buf[17]) << 4;
									uwu.b[lbw] |= ascii_to_hex(drv_rcv_buf[18]);
									drv_motor_current = uwu.w;
								}
		
								drive_com_error = 0;
								rcv_drive_msg = 1;
								if (drv_half_duplex == 1)
								{	// hal duplex
									// Enable transmitter
									DRVtxEn;
								}
								break;
							default:
								break;
						}


						if (drive_rcv_cnt < 32000)
		  				    drive_rcv_cnt++;
						else
							drive_rcv_cnt = 0;
						
						drmbuf[0] = 0;
						drmbuf[1] = 0;
						drmbuf[len-2] = 0;
		  				drmbuf[len-1] = 0;
		  				dmpnt = 0;
						endmess = 1;
		  	  	    }
		  	  	    else                                // Bad Packet
		  	  	    {
						drive_pkt_err_cnt++;
		  				j = 1;
		  				while((drmbuf[j] != STX) && (j < dmpnt))
		  				{
		  					j++;
		  				}
		  				i = 0;
		  				while(j < dmpnt)        /* found another start character */
		  				{
		  					drmbuf[i] = drmbuf[j];
		  					i++;
		  					j++;
		  				}
		  				dmpnt = i;
		  	  	    }
					invoke_id_ck = 0;
		  	  	    rbempt = 1;
		  	  	}
				else if (dmpnt > len)
				{
		  			rbempt = 1;
		  			dmpnt = 0;
					drmbuf[0] = 0;
					drmbuf[1] = 0;
					drmbuf[msg_len[0]-2] = 0;
	  				drmbuf[msg_len[0]-1] = 0;
				}
			}
			else if (dmpnt > 22)
			{
				dmpnt = 0;
			}
		}
		else if (dmpnt > 2)
		{
			dmpnt = 0;
		}
  	}
  	return;
}

//******************************************************
// Verify KEB Drive Run Mode
//******************************************************

void verify_run_mode (void)
{
static int time_out_cnt;
static int16 drv_par_req;
int i;

#if (Simulator == 1)
	keb_run_verify_seq = 3;
#endif

	if (keb_run_verify_seq == 0)
	{	// ready and waiting for command to verify run mode
	}
	else if (keb_run_verify_seq == 1)
	{
		drv_rpar_addr = 0x0383;
		drv_rpar_set = 1;
		put_pkt_req('G',2,DRVCOM);
		drv_nak = 0;
		drv_eot = 0;
		drv_error = 0;
		keb_run_verify_seq = 2;
		time_out_cnt = 0;
	}
	else if (keb_run_verify_seq == 2)
	{
  		i = get_pkt_status('G', 2, DRVCOM, 20);
		if (i == 1)
		{			   // lf.03 data read in
			drv_par_req = 0;
			Up_Date_LCD = 1;
			time_out_cnt = 0;
			if (drv_rpar_data == 0)
			{
				keb_run_verify_seq = 3;
				statusf2 &= ~sf_KEBRUN;
			}
			else 
			{
				keb_run_verify_seq = 1;
	
				drv_wpar_addr = 0x0383;
				drv_wpar_set = 1;
				drv_wpar_data = 0;
				put_pkt_req('g',2,DRVCOM);	 // put drive in run mode
				drv_nak = 0;
				drv_eot = 0;
				drv_error = 0;
			}
		}
		else if ((i == 2) || (i == -1))
		{
			if (time_out_cnt < 3)
			{		   // packete timed out so try again
				time_out_cnt ++;
				drv_rpar_addr = 0x0383;
				drv_rpar_set = 1;
				put_pkt_req('G',2,DRVCOM);
				drv_nak = 0;
				drv_eot = 0;
				drv_error = 0;
				keb_run_verify_seq = 2;
			}
			else if (time_out_cnt < 4)
			{
				if ((statusf2 & sf_KEBRUN) == 0)
					record_fault(f_keb_no_run_mode);
				time_out_cnt++;
				keb_run_verify_seq = 4;
				timers[tkebrun] = 0;
			}
		}
	}
	else if (keb_run_verify_seq == 3)
	{		// run mode verified
		statusf2 &= ~sf_KEBRUN;
	}
	else if (keb_run_verify_seq == 4)
	{		// run mode failed
		statusf2 |= sf_KEBRUN;
		if (timers[tkebrun] > 10)
			keb_run_verify_seq = 1;
	}

}


#endif



/* Revision History	
	

  3/27/07  5.22  mhd	1. Seperated drive comm into seperate file.
  4/27/07  5.23  mhd	1. Added comm for KEB Combivert F5 drive.
						2. Run KEB drive at 57600 baud.
  9/28/097 5.37 mhd		1. Run KEB drive at 38400 baud and poll every 15 msec.
  2/22/08 v5.41 mhd		1. Prevent divide by zero with ramp_dly.
  6/13/08 v5.44.4 mhd	1. Added comm for Delta VDF-VL drive.
  6/18/08 v5.44.5 mhd	1. Mofified drive comm for half duplex RS-485.
						2. Changed check to get message for drive comm.  Even though get message would return a message value, the logic to allow
						   the message to be transmitted only when the car was stopped, was faulty. Changed ((i != 0) && (procf <= 3) && (procf >= 11)) 
						   to ((i != 0) && ((procf <= 3) || (procf >= 11))).
  7/29/08 V5.45.3 mhd	1. Set standing field bit in comm for quatro drive.
						2. Set drv_pre_trq_seq = 9 once velocity is non-zero incase pre-torque sequence is interrupted.
  4/2/09 v5.48.3 mhd	1. Deleted drv_par_wrt, used with KEB, drive.  Use lower case g for write instead of upper case G for both read and write. This
                           way the message can be placed in the queue and the read or write information is not lost.
  4/21/09 				2. Modified init sequence for KEB drive.
  						3. Modified check for an available packet to execute after a control packet is sent.
  5/18/09 v5.48.6 mhd	1. Set variable drv_command to display on video debug screen.
  7/23/09 v5.49.4 mhd	1. Modified drive pre-torque sequence for HPV and Quattro drive to not have a run signal on the comm in advance of
						   the pre-torque signal.
  11/5/09 v5.50.6 mhd	1. modified reference splitting
  12/8/09 v5.50.11 mhd	1. Added transmitting and receiving parameters to and from HPV-900 drive.
						2. Added 50msec delay after transmitting parameter request.
  6/29/10 v5.52.11 mhd	1. Drive has comm error Send drive reset after 1 drive_com_error but do not shut down the car until more than 20 occur
  7/2/10 v5.52.12 mhd	1. Corrected up and down direction bit in comm to DSD-412 Drive.
  10/6/10 V5.52.36 mhd	1. Added modbus comm to Yaskawa L1000 drive (drive_type[]=15).
  1/11/11 v5.52.56 mhd	1. Delete Delta Drive.
  1/24/12 V6.0 mhd		1. Added drv_pwr_up_delay to prevent nuisance drive com error.
  8/2/12 v5.54.99 mhd	1. Added motor rpm for keb drive.
  8/8/12 v5.55.00 hdl	1. Changed the initialization sequence on KEB drives to allow more time before declaring eBUS error.
  9/7/12 v6.00.21 mhd	1. Added changes for v5.54.99 and v5.55.00.
  2/20/13 v7.0.1 mhd	1. Added drive start timer mechanism to get exactly 10, 15 or 20 msec,
  9/3/13 v7.0.18 mhd	1. Added keb display type parameter to select new keb display instead of using a different drive number.
  11/19/13 v7.0.29 mhd	1. Use fvars[fvenctype] == 4 for tape selector pulse feedback.
  1/3/14 v7.0.31 mhd	1. Modified initialization sequence to change e-bus time-out and to verify watchdog timer setting.
  						2. Changed ack and nak verification with rx_data_ID instead of tx_data_ID.
  10/7/14 v7.1.12 mhd	1. When RUN is lost, set the drive velocity to the demand velocity.
  10/8/14 v7.1.15 mhd	1. When checking velocity step to set drv_velocity, added check for (procf == 20) || (procf == 21) || (procf == 23).
  						2. When RUN or UP or DNR is lost, set the drive velocity to the demand velocity.
  10/9/14 v7.1.16 mhd	1. Turn off serial RUN command for brake test when UP and DNR signals are off.
  11/20/14 v7.1.21 mhd	1. Added check for receive message pointer dmpnt being too large in rcv_DSD_drv_com() and rcv_HPV_drv_com().
  11/25/14 v7.1.21 mhd	1. Set statusf2 flag if velocity rate is clampped.
  12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
						 4/5/14 v6.1.43 hdl	1. Modified   set_drv_ep_mode and  reset_drv_ep_mode to allow ultiple attempts when packet requests failed.
											2. Added read_drive_from_MRAM() and save_drive_to_MRAM() routines to save
												drv_line_voltage drv_motor_torque drv_regen_torque to MRAM to preserve settings because newer magnetek software
												is saving the parameters all the time.
6/3/15 v7.1.32 mhd		1. Change multiplier for load weigher from 1000 to 100 on KEB drive.
7/24/15 v7.1.40 mhd		1. Remove check for Torque != 0 to set pre-torque in KEB transmit routine.
						2. In KEB, HPV-900 and L1000 drives, send torque of zero if load weigher is turned off or disabled (same as DSD-412).
						3. Changed the delay time for UP or DNR to be 70 msec (pre_trq_cnt > 6) and set the UP and DN regardless of the torque ramp timer.

11/23/15 v7.1.49 mhd	1. Setup HPV drive to read drv_encoder_spd and drv_motor_torque on powerup so that these values are read all the time.
*/
