#define Baud 0		// Set baud rate for comm 2: 0 = 57600, 1=38400, 2=19200
#define d_HLCOM 1
#include "global.h"

/*		Hall Lantern Comm Description
		Rs-485 half duplex
		8 data bits
		1 start bit
		1 stop bit
		No parity sent on address

		Comm Protocol
		Master  						Hall Lantern      
		START '@'                		'@'				
		TO ADDRESS						FROM ADDRESS
		Packet Marker ':'				':'	
		Packet #						Packet #		
		Length							Length			
		DATA 1                  		DATA 1   	      
		DATA 2                  		DATA 2            
			...                    			...
		DATA LAST               		DATA LAST         
		LRC CHECKSUM               		LRC CHECKSUM      
		END	'!'							'!'				

		
		Transmit buffer is packed exactly as data is transmitted so the byte count is the number of bytes placed in the hc_trm_buf[]

		There is no receive data.

		Packets are not tested for a fixed length but for the transmitted
		length being equal to the actual length.


	Packet/Function DESCRIPTION

	PACKET# 	TO 			FROM		BC			DESCRIPTION
		25		HL			CPU			8	   		Message #25: PI and Lantern Data
												  	Car position		Binary position
												  	Ascii PI 0			Ascii Position low byte
												  	Ascii PI 1			Ascii Position High byte

													Lantern Status		Bit 7: LUP (lantern up)
																		Bit 6: LDN (lantern down)
																		Bit 5: RLUP (rear lantern up)
																		Bit 4: RLDN (rear lantern down)
																		Bit 3: UDA (up direction arrow)
																		Bit 2: DDA (down direction arrow)
																		Bit 1: User Message Bit 1
																		Bit 0: User Message Bit 0

												  	Motion Status		Bit 7: CULR UPREF (up preference)
												  						Bit 6: CDLR DNPREF (down preference)
												  						Bit 5: CUL run (car running)
												  						Bit 4: CDL leveling (car leveling)
												  						Bit 3: Out of Service
																		Bit 2: Inspection
																		Bit 1: Independent
																		Bit 0: unused

													Fire Status			Bit 7: PH2 (fire phase 2)
																		Bit 6: PH1 Complete (fire phase 1 complete)
																		Bit 5: PH1 Recall Main (fire phase 1 recall active)
																		Bit 4: EMP Recall (emergency power recall active)
												  						Bit 3: EMP Recall Complete (emergency power recall complete)
												  						Bit 2: Power Loss (Normal power is out)
												  						Bit 1: Imminent Shutdown
																		Bit 0: PH1 Recall Alternate (fire phase 1 recall active)

													Door Status 		Bits 3,2,1,0
												  						doorf = 0 doors closed no action
												  						doorf = 1 doors are opening
												  						doorf = 2 doors are dwellingse)
																		doorf = 3 doors are closing
																		doorf = 4 doors are nudging closed

																		Bits 7,6,5,4
													  					rdoorf = 0 doors closed no action
													  					rdoorf = 1 doors are opening
													  					rdoorf = 2 doors are dwelling
													  					rdoorf = 3 doors are closing
													  					rdoorf = 4 doors are nudging closed

													Misc Status			Bit 7: Activate speech
																		Bit 6: At Lobby (car at lobby floor)
																		Bit 5: Overload (load overload)
																		Bit 4: Load Bypass
																		Bit 3: EMT (Emergency Medical Technician Service)
																		Bit 2: Code Blue
																		Bit 1: Seismic
																		Bit 0: User Message Bit 2 

																		User Message Bits 2,1,0
																						  0 0 0	No message
																						  0 0 1 Message 1
																						  0 1 0 Message 2
																						  0 1 1 Message 3
																						  1 0 0 Message 4
																						  1 0 1 Message 5
																						  1 1 0 Message 6
																						  1 1 1 Message 7
																						   
*/



//*********************************
// Hall Lantern Comm Variables
//*********************************

// Device number declaratons

#define c_HL_addr 20

#define c_hl_buf_size 30
unsigned char hl_trm_buf[c_hl_buf_size];
uint16 hl_tx_len = 0;
int16 hl_port = 0;
int16 run_hl_com = 0;		// set to 1 to run hall lantern com
int16 hl_trm_cnt=0;

//*********************************
// Function Declaration
//*********************************

void init_hl_com (void);
void init_hl_pit (void);
void hl_dma_trm_int(void);
void hl_com_timer_int(void);
void hl_com_start_timer_int (void);
void hl_com (void);
void pack_hl_com(uint16 Packet,uint16 to_addr);
void init_hl_DMA(void);
void hl_com_tx_start (int16 len);
		

// ***************************************************************
// ***************************************************************
// Initialize data for Hall Lanter comm
// ***************************************************************
// ***************************************************************


void init_hl_com (void)
{
	uint16 data_in;
	
	_disable();

	hl_port = HLCOM;
	hl_tx_len = 0;
		
	HLtxDis; 						// Turn off Transmit driver

	HLport.CR1.B.TE = 0;			// Transmitter Disable
	HLport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable


	// Control register 1	  
	HLport.LCR.B.LIN = 0;			// Place in SCI mode	
	HLport.CR1.B.SBR = 118;			// 57.6 K baud
	
	HLport.CR1.B.LOOPS = 0;			// Receiver source mode LOOPS:RSRC 00 = Dual wire mode
	HLport.CR1.B.RSRC = 0;
	
	HLport.CR1.B.M = 0;				// Frame Format Mode: set to 1 for nine bits
	HLport.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
	HLport.CR1.B.PE = 0;			// Parity Enable: 0 = disabled, 1 = enabled 
	HLport.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd
	HLport.CR3.B.M2 = 0;

	HLport.CR3.B.EROE = 0;			// Set error bit in receive data register 
	HLport.CR3.B.ERFE = 0;			// Set error bit in receive data register
	HLport.CR3.B.ERPE = 0;			// Set error bit in receive data register


	HLport.CR1.B.ILT = 0;			// Idle Line Type: 0 = idle line detection after low bit
	HLport.CR1.B.TIE = 0;			// Transmit Interrupt Enable (data tranferred to shift register)
	HLport.CR1.B.TCIE = 0;			// Transmit Complete Interrupt Enable (data shifted out of shift register)
	HLport.CR1.B.RIE = 0;			// Receive Interrupt Enable
	HLport.CR1.B.ILIE = 0;			// Idle line interrupt enable
	HLport.CR1.B.RWU = 0;			// Receiver Wake Up mode: 0 = normal, 1 = wakeup on 9th bit address
	HLport.CR1.B.SBK = 0;			// Send Break Character
	
	// Control Register 2
	HLport.CR2.B.MDIS = 0;			// Module Disable
	HLport.CR2.B.FBR = 0;			// Fast Bit Error Detect: 0 = normal, 1 = use in LIN mode	
	HLport.CR2.B.BSTP = 0;			// DMA Stop on Bit Error: 0 = tx dma regardless of bit errors
	HLport.CR2.B.IEBERR = 0;		// Bit Error Interrupt Enable
	HLport.CR2.B.RXDMA = 1;			// Receive DMA Control: 0 = disabled, 1 = enabled
	HLport.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	HLport.CR2.B.BRK13 = 0;			// Break Character Length
	HLport.CR2.B.TXDIR = 0;			// TXD pin output enable: Used in signel wire mode
	HLport.CR2.B.BESM13 = 0;		// Fast Bit Error Detection Sample Mode: 0=RS9,1=RS10 LIN mode only
	HLport.CR2.B.SBSTP = 0;			// Transmit Stop on Bit Error
	HLport.CR2.B.RXPOL = 0;			// RXD Pin polarity: 0 = normal, 1 = inverted
	HLport.CR2.B.PMSK = 0;			// Parity Bit Masking: 0 = in bit position, 1 = 0 in parity position	
	HLport.CR2.B.ORIE = 0;			// Overrun Interrupt Enable
	HLport.CR2.B.NFIE = 0;			// Noise Interrupt Enable
	HLport.CR2.B.FEIE = 0;			// Frame Error Interrupt Enable
	HLport.CR2.B.PFIE = 0;			// Parity Error Interrupt Enable
	

	data_in = HLport.SDR.R;
	data_in = HLport.SDR.R;

	init_drv_DMA();

  	HL_Tranbuf_Empty = 1;
	HL_Tranbuf_Full = 0;

	// Enable must be last
	HLport.CR1.B.TE = 1;			// Transmitter Enable
	
	HLport.CR1.B.RE = 0;			// Receiver Disable

	init_drv_pit();

	HLtxEn;
		
	_enable();
}

//**************************************************
// Init Hall Lantern Com Programmable Interval Timer
//**************************************************
void init_hl_pit (void)
{
	PIT.TCTRL7.B.TEN = 0;				// Stop Timer
	INTC_InstallINTCInterruptHandler(hl_com_start_timer_int, 155, 1);  // Set interrupt vector for PIT7
	PIT.LDVAL7.R = 10875000 - 1;		// setup timer 1 for 10 msec with 108.75 MHz fsys
	PIT.TFLG7.B.TIF = 1;				// Clear interrupt flag so an interrupt does not occur right away
	PIT.TCTRL7.B.TIE = 1;				// Enable interrupts
	PIT.TCTRL7.B.TEN = 1;				// Start Timer

}

//*****************************************
// DMA Initialization for Hall Lantern Comm
//*****************************************


void init_hl_DMA(void) {

	// Initialize Transmit Comm DMA
  
  	EDMA.CERQR.R = HLdmaChTx;      	// Disable Group transmit EDMA channel   

	INTC_InstallINTCInterruptHandler(hl_dma_trm_int, HLdmaTxIntLoc, 1);  

  	EDMA.CPR[HLdmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[HLdmaChTx].R = HLdmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[HLdmaChTx].SADDR = (uint32) &hl_trm_buf[0];  		// Load address of source data 
  	EDMA.TCD[HLdmaChTx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[HLdmaChTx].SOFF = 1;                         		// After transfer, add 1 to src addr
  	
  	// init to ten bytes for now
  	EDMA.TCD[HLdmaChTx].SLAST = -10;                      		// After major loop, reset src addr 
  	EDMA.TCD[HLdmaChTx].SMOD = 0;                         		// Source modulo feature not used 

  	EDMA.TCD[HLdmaChTx].DADDR =  HLdr;	 						// Load address of destination 
  	EDMA.TCD[HLdmaChTx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[HLdmaChTx].DOFF = 0;                         		// Do not increment destination addr 
  	EDMA.TCD[HLdmaChTx].DLAST_SGA = 0;                    		// After major loop, no dest addr change 
  	EDMA.TCD[HLdmaChTx].DMOD = 0;                         		// Destination modulo feature not used 
  
  	EDMA.TCD[HLdmaChTx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[HLdmaChTx].BITER = 10;                       		// 10 major loop iterations 
  	EDMA.TCD[HLdmaChTx].CITER = 10;                       		// Initialize current iteraction count 
  	EDMA.TCD[HLdmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[HLdmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[HLdmaChTx].INT_MAJ = 1;
  	EDMA.TCD[HLdmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[HLdmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[HLdmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[HLdmaChTx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[HLdmaChTx].E_SG = 0; 
  	EDMA.TCD[HLdmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[HLdmaChTx].DONE = 0;
  	EDMA.TCD[HLdmaChTx].ACTIVE = 0;
  	EDMA.TCD[HLdmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = HLdmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = HLdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = HLdmaChTx;           	// DMA Clear error Register 
  
}



//*********************************************
// Hall Lantern DMA transmit finished interrupt
//*********************************************

void hl_dma_trm_int(void)
{

  	EDMA.CIRQR.R = HLdmaChTx;       // DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = HLdmaChTx;      	// Disable Drive EDMA channel   
	HLport.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	hl_tx_len = 0;
}


//*********************************************
// HL Transmit Start 100 msec Timer Interrupt
//*********************************************
void hl_com_start_timer_int (void)
{
	PIT.TFLG7.B.TIF = 1;		// Clear interrupt flag 
	run_hl_com = 1;				// allow to pack next message
	if (hl_tx_len != 0)
	{				// message has been packed
		hl_com_tx_start(hl_tx_len);
	}
}

// ***************************************************************
// ******** Start Drive Comm *************************************
// ***************************************************************

void hl_com_tx_start (int16 len)
{
	_disable();	
	
	// Turn off the receiver
	HLport.CR1.B.RE = 0;					// Receiver Enable: 1 = Enable, 0 = Disable
	
	// Enable transmitter
	HLtxEn;
	// Start DMA transfer for com
	EDMA.CER.R = HLdmaChTx;           		// DMA Clear error Register 
	HLport.CR2.B.TXDMA = 0;					// Transmit DMA Control: 0 = disabled, 1 = enabled
    EDMA.TCD[HLdmaChTx].SADDR = (uint32) &hl_trm_buf[0];  	// Load address of source data 
 	EDMA.TCD[HLdmaChTx].SLAST = (0-len);    // After major loop, reset src addr 
 	EDMA.TCD[HLdmaChTx].NBYTES = 1;			// Transfer len number byte per minor loop 
  	EDMA.TCD[HLdmaChTx].BITER = len;	   	// 10 major loop iterations 
  	EDMA.TCD[HLdmaChTx].CITER = len;   		// Initialize current iteraction count 
  	EDMA.SERQR.R = HLdmaChTx;      			// Enable Drive EDMA channel   
	HLport.CR2.B.TXDMA = 1;					// Transmit DMA Control: 0 = disabled, 1 = enabled
	_enable();
}



//********************************
// Hall Lanterm comunications procedure
//********************************
void hl_com()
{
	uint16 Packet;

	if ((run_hl_com == 1) && (hl_tx_len == 0))
	{
		
		if (EDMA.TCD[HLdmaChTx].ACTIVE == 0)
		{
			Packet = get_pkt_req(20,hl_port);	// get a Packet from the queue

			if (Packet == 0)
				Packet = 25;
					
			pack_hl_com(Packet,20);  // transmit hall lantern data
			
			run_hl_com = 0;
		}
	}
}

//*******************************************
// Pack hall lanter data Packet for comm port
//*******************************************

void pack_hl_com(uint16 Packet,uint16 dev)
{
	int16 i;
	char unsigned ck_sum;
	int16 len = 0;
	bool valid_msg = 0;
	
	ck_sum = 0;
	ix = 0;
	bc=0;
	switch (Packet)
	{
		case 0:			// Invalid packet
			break;
		case 25:			// PI / Hall lantern packet for hydro
			hl_trm_buf[0] = '@';	// Start Character
			hl_trm_buf[1] = dev;	// To Address
			hl_trm_buf[2] = ':'		// Packet marker
			hl_trm_buf[3] = 25;		// packet number
			hl_trm_buf[4] = 13;		// length
			if (cons[Xref_PI_Pos] == 1)
				hl_trm_buf[5] = xpi_pos[pi_pos];
			else
				hl_trm_buf[5] = pi_pos;
			if ((servf == 25) && ((fvars[fvsabbdis] & 0x01) != 0))
			{
				hl_trm_buf[6] = 'S';
				hl_trm_buf[7] = 'B';
			}
			else 
			{
				hl_trm_buf[6] = flmrk[pi_pos][0];
				hl_trm_buf[7] = flmrk[pi_pos][1];
			}
			if((alrm_pi_stat == 1) && (timers[tsec] != 0))
			{
				hl_trm_buf[6] = 'A';
				hl_trm_buf[7] = 'L';
			}
			
			hl_trm_buf[8] = pi_lant_stat;
			hl_trm_buf[9] = pi_motion_stat;
			hl_trm_buf[10] = pi_fire_stat;
			hl_trm_buf[11] = pi_door_stat;
			hl_trm_buf[12] = pi_misc_stat;
			len = 13;
			valid_msg = 1;
			break;	

		default:
			break;
	}
	
	if (valid_msg == 1)
	{
		for (i=0; i < len; i++)
			ck_sum += (uint8)hl_trm_buf[i];

		hl_trm_buf[len++] = (uint8)((0xFF - ck_sum) + 1); 			      // Set checksum in buffer
		hl_trm_buf[len++] = '!';						// packet end
		hl_trm_buf[len++] = 0xF0;						// Dummy byte
		hl_trm_buf[len++] = 0xFF;						// Dummy byte
		chk_sum = 0;

	 	hl_tx_len = len;

		if (hl_trm_cnt < 32000)
			hl_trm_cnt++;
		else
			hl_trm_cnt = 0;
	}
}




/* Revision History


*/
