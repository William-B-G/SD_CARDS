//  Serial Expansion Comm Routines
#define Baud 0		// Set baud rate for comm 2: 0 = 57600, 1=38400, 2=19200
#define d_SERIALEX 1
#include "global.h"

#define BACKGROUND	0
#define FOREGROUND	1
#define BLACK		0
#define BLUE		1
#define GREEN		2
#define CYAN		3
#define RED			4
#define MAGENTA 	5
#define BROWN		6
#define LTGREY		7
#define DKGREY		8
#define LTBLUE		9
#define LTGREEN 	10
#define LTCYAN		11
#define LTRED		12
#define LTMAGENTA	13
#define YELLOW		14
#define WHITE		15

#define LF 0x0A

extern const uint16 bittbl[16];
//				  {2400,4800,9600,19200}		
int16 sx_baud_tbl[4] = {2832,1416, 708,  354};	// baud setting for 108.75 MHz
//				     {2400, 4800, 9600, 19.2, 38.4, 57.6, 115.2, baud}		
int16 sx2_baud_tbl[7] = {2832, 1416,  708,  354,  177,  118,    59};	// baud setting for 108.75 MHz
uint8 temp_char = 0; // remove after RS485 test is debugged TODO_wbg (wbg)

#define ComDebugA 1
#define ComDebugB 0

// character strings used for communications
#define sx_rbufsize 300
#define sx_tbufsize 100

int16 sx_com_baud[2];	// default baud rate for sx comm


// *************************************
//        SX Comm Variables
// *************************************

unsigned char sx_Tranbuf_Empty[2] = {1,1}; // Com 1-2 Transmiter buffer is empty if 1 has chars if 0
unsigned char sx_Tranbuf_Full[2] = {0,0};	// Com Transmit buffer is full if 1 and 0 if not
unsigned char sx_Start_Tx[2] = {0,0};
unsigned char sx_tranbuf[2][sx_tbufsize+1];  // com transmit buffer
unsigned char sx_recbuf[2][sx_rbufsize+1];   // com receive buffer

int16 sx_rbs[2]={0,0}; 			// receive buffer start pointer com 1-2
int16 sx_rbf[2]={0,0}; 			// receive buffer end pointer com 1-2
int16 sx_tbs[2]={0,0};          // trasmit buffer start pointer com 1-2
int16 sx_tbf[2]={0,0};          // trasmit buffer end pointer com 1-2

int16 rx_row[2] = {1,1};
int16 rx_col[2] = {5,5};
int16 tx_row[2] = {1,1};
int16 tx_col[2] = {5,5};
int16 video_sx_rbf[2] = {0,0};
int16 video_sx_tbf[2] = {0,0};
int16 sx_dma_start_cnt[2] = {0,0};
int16 sx_dma_end_cnt[2] = {0,0};


void sx_initcom(void);
void sx_sendc( int16 comnmb, int16 letter);
void sx_enable_tx(int16 comnmb);
void init_sx_com_port_1(void);
void init_sx_com_port_2(void);
void sx_com_1_int(void);
void sx_com_2_int(void);
void se1_tx_dma_start(int16 len);
void se1_dma_trm_finished_int(void);
void se2_tx_dma_start(int16 len);
void se2_dma_trm_finished_int(void);
int16 sx_isrxempty(int16 comnmb);
int16 sx_istxcomplete(int16 comnmb);
int16 sx_asigetc( int16 comnmb);
int16 sx_istxDMAcomplete(int16 comnmb);


extern void vratputf( int v_w, int row, int col, int attr, char *fmt,...);


// ********************************************************
// This is polled driven get char from the receive buffer
// ********************************************************
int16 sx_asigetc( int16 comnmb)
{
  	int16 i;
  	i = sx_recbuf[comnmb][sx_rbs[comnmb]];
  	sx_recbuf[comnmb][sx_rbs[comnmb]] = 0;
  	sx_rbs[comnmb]++;
  	if(sx_rbs[comnmb] >= sx_rbufsize)
		sx_rbs[comnmb] = 0;
  	return(i);
}

//**************************************************
// serial control for com port #SE1
//**************************************************
void sx_com_1_int(void)
{
	int32 SE1_source;			

	SE1_source = SE1port.SR.R;
	if ((SE1_source & 0x0F000000) != 0)		// Overrun, Noise, Framing or Parity error
	{
		SE1port.SR.R = 0x0F000000;	// Clear error flags by writing ones.
	}
	
  //////////  wbg :  2017-10-19 @ 11:30 /////// ver 0.0.36_C  // uncomment the ISR for Rx    
	if (SE1port.SR.B.RDRF == 1)		//Interupt source is a recieve data ready
	{	    
//-	    SIU.GPDO[Z6_LED].R = ~SIU.GPDO[Z6_LED].R;	// toggle led (wbg)
	    temp_char = (uint8)SE1port.SDR.B.D ; // Read data byte from buffer (wbg)
	    if(temp_char != 0) // (wbg)
	    {	 
	        if(sx_rbf[SE1COM] < 30)tempBuff[sx_rbf[SE1COM]] = temp_char; // save the char      	    
    		sx_recbuf[SE1COM][sx_rbf[SE1COM]] = temp_char;  // save the char	
    		SE1port.SR.R = 0x20000000;   // Clear RDRF flag 
    		sx_rbf[SE1COM]++;
    		if(sx_rbf[SE1COM] >= sx_rbufsize)
    			sx_rbf[SE1COM] = 0;
	    }
	    else SE1port.SR.R = 0x20000000;   // Clear RDRF flag
	}
  //////////  wbg :  2017-10-19 @ 11:30 /////// ver 0.0.36_C 
  
	if (sx_Start_Tx[SE1COM] == 1)
	{
		if (SE1port.SR.B.TDRE == 1)  // Interupt source is transmiter hold reg empty
		{
			SE1port.SR.R = 0x80000000;           /* Clear TDRE flag */
			SE1port.SDR.B.D = sx_tranbuf[SE1COM][sx_tbf[SE1COM]];
		
			sx_tbf[SE1COM]++;
			if(sx_tbf[SE1COM] >= sx_tbufsize)
				sx_tbf[SE1COM] = 0;
			if (sx_tbs[SE1COM] >= sx_tbf[SE1COM])
			{
				if ((sx_tbf[SE1COM] + (sx_tbufsize - sx_tbs[SE1COM])) >= 50)
					sx_Tranbuf_Full[SE1COM] = 0;
			}
			else
			{
				if ((sx_tbf[SE1COM] - sx_tbs[SE1COM]) >= 50)
					sx_Tranbuf_Full[SE1COM] = 0;
			}
			if(sx_tbs[SE1COM] == sx_tbf[SE1COM])   // disable transmiter empty interupt
			{
				sx_Tranbuf_Empty[SE1COM] = 1;
				sx_Tranbuf_Full[SE1COM] = 0;
				sx_Start_Tx[SE1COM] = 0;
			}
					
		}
	}
}

//**************************************************
// serial control for com port #SE2
//**************************************************
void sx_com_2_int(void)
{
	int32 SE2_source;

	SE2_source = SE2port.SR.R;
	if ((SE2_source & 0x0F000000) != 0)		// Overrun, Noise, Framing or Parity error
	{
		SE2port.SR.R = 0x0F000000;	// Clear error flags by writing ones.
	}
/*	if (SE2port.SR.B.RDRF == 1)		//Interupt source is a recieve data ready
	{
		sx_recbuf[SE2COM][sx_rbf[SE2COM]] = (uint8)SE2port.SDR.B.D;	// Read data byte from buffer
		SE2port.SR.R = 0x20000000;           // Clear RDRF flag 
		sx_rbf[SE2COM]++;
		if(sx_rbf[SE2COM] >= sx_rbufsize)
			sx_rbf[SE2COM] = 0;
	}
*/
	if (sx_Start_Tx[SE2COM] == 1)
	{
		if (SE2port.SR.B.TDRE == 1)   // Interupt source is transmiter hold reg empty
		{
		
			SE2port.SR.R = 0x80000000;           /* Clear TDRE flag */
			SE2port.SDR.B.D = sx_tranbuf[SE2COM][sx_tbf[SE2COM]];
		
			sx_tbf[SE2COM]++;
			if(sx_tbf[SE2COM] >= sx_tbufsize)
				sx_tbf[SE2COM] = 0;
			if (sx_tbs[SE2COM] >= sx_tbf[SE2COM])
			{
				if ((sx_tbf[SE2COM] + (sx_tbufsize - sx_tbs[SE2COM])) >= 50)
					sx_Tranbuf_Full[SE2COM] = 0;
			}
			else
			{
				if ((sx_tbf[SE2COM] - sx_tbs[SE2COM]) >= 50)
					sx_Tranbuf_Full[SE2COM] = 0;
			}
			if(sx_tbs[SE2COM] == sx_tbf[SE2COM])   // disable transmiter empty interupt
			{
				sx_Tranbuf_Empty[SE2COM] = 1;
				sx_Tranbuf_Full[SE2COM] = 0;
				sx_Start_Tx[SE2COM] = 0;
			}
					
		}
	}
}


//****************************************************
// Initialize serial expansion comm ports
//****************************************************

void init_sx_com_port_1(void)
{
	uint16 data_in;

	_disable();

	// Serial Expansion Comm
		
  	/* Install our handlers */  
  	INTC_InstallINTCInterruptHandler(sx_com_1_int, SE1intLoc, 1);  


	SE1txDis; 						// Turn off Transmit driver

	SE1port.CR1.B.TE = 0;			// Transmitter Disable
	SE1port.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable

	// Control register 1	  
	SE1port.LCR.B.LIN = 0;			// Place in SCI mode	
	SE1port.CR1.B.SBR = sx_com_baud[SE1COM];
	
	SE1port.CR1.B.LOOPS = 0;		// Receiver source mode LOOPS:RSRC 00 = Dual wire mode
	SE1port.CR1.B.RSRC = 0;
	

	SE1port.CR1.B.M = 0;			// Frame Format Mode: set to 1 for nine bits
	SE1port.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
	SE1port.CR1.B.PE = 0;			// Parity Enable: 0 = disabled, 1 = enabled 
	SE1port.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd
	SE1port.CR3.B.M2 = 0;
 
 	SE1port.CR3.B.EROE = 0;			// Set error bit in receive data register 
	SE1port.CR3.B.ERFE = 0;			// Set error bit in receive data register
	SE1port.CR3.B.ERPE = 0;			// Set error bit in receive data register


	SE1port.CR1.B.ILT = 0;			// Idle Line Type: 0 = idle line detection after low bit
	SE1port.CR1.B.TIE = 0;			// Transmit Interrupt Enable (data tranferred to shift register)
	SE1port.CR1.B.TCIE = 0;			// Transmit Complete Interrupt Enable (data shifted out of shift register)
	SE1port.CR1.B.RIE = 0;			// Receive Interrupt Enable
	SE1port.CR1.B.ILIE = 0;			// Idle line interrupt enable
	SE1port.CR1.B.RWU = 0;			// Receiver Wake Up mode: 0 = normal, 1 = wakeup on 9th bit address
	SE1port.CR1.B.SBK = 0;			// Send Break Character
	
	// Control Register 2
	SE1port.CR2.B.MDIS = 0;			// Module Disable
	SE1port.CR2.B.FBR = 0;			// Fast Bit Error Detect: 0 = normal, 1 = use in LIN mode	
	SE1port.CR2.B.BSTP = 0;			// DMA Stop on Bit Error: 0 = tx dma regardless of bit errors
	SE1port.CR2.B.IEBERR = 0;		// Bit Error Interrupt Enable
	SE1port.CR2.B.RXDMA = 1;		// Receive DMA Control: 0 = disabled, 1 = enabled
	SE1port.CR2.B.TXDMA = 0;		// Transmit DMA Control: 0 = disabled, 1 = enabled
	SE1port.CR2.B.BRK13 = 0;		// Break Character Length
	SE1port.CR2.B.TXDIR = 0;		// TXD pin output enable: Used in signel wire mode
	SE1port.CR2.B.BESM13 = 0;		// Fast Bit Error Detection Sample Mode: 0=RS9,1=RS10 LIN mode only
	SE1port.CR2.B.SBSTP = 0;		// Transmit Stop on Bit Error
	SE1port.CR2.B.RXPOL = 0;		// RXD Pin polarity: 0 = normal, 1 = inverted
	SE1port.CR2.B.PMSK = 0;			// Parity Bit Masking: 0 = in bit position, 1 = 0 in parity position	
	SE1port.CR2.B.ORIE = 1;			// Overrun Interrupt Enable
	SE1port.CR2.B.NFIE = 1;			// Noise Interrupt Enable
	SE1port.CR2.B.FEIE = 1;			// Frame Error Interrupt Enable
	SE1port.CR2.B.PFIE = 1;			// Parity Error Interrupt Enable
	
	data_in = SE1port.SDR.R;
	data_in = SE1port.SDR.R;

	// Set RTS line high or low here when pin defined *******
	SE1txEn;

	sx_Tranbuf_Empty[SE1COM] = 1;
	sx_Tranbuf_Full[SE1COM] = 0;
	sx_Start_Tx[SE1COM] = 0;
	sx_tbs[SE1COM] = 0;
	sx_tbf[SE1COM] = 0;
	sx_rbs[SE1COM] = 0;
	sx_rbf[SE1COM] = 0;
	video_sx_rbf[SE1COM] = 0;
	video_sx_tbf[SE1COM] = 0;

  	// Initialize Transmit Comm DMA

 	EDMA.CERQR.R = SE1dmaChTx;      		// Disable group EDMA channel   
  
	INTC_InstallINTCInterruptHandler(se1_dma_trm_finished_int, SE1dmaTxIntLoc, 1);  

  	EDMA.CPR[SE1dmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[SE1dmaChTx].R = SE1dmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[SE1dmaChTx].SADDR = (uint32) &sx_tranbuf[SE1COM][0];  						// Load address of source data 
  	EDMA.TCD[SE1dmaChTx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[SE1dmaChTx].SOFF = 1;                         		// Increment source address by 1
  	
  	EDMA.TCD[SE1dmaChTx].SLAST = -10;                      		// After major loop, reset src addr 
  	EDMA.TCD[SE1dmaChTx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[SE1dmaChTx].DADDR =  SE1dr; 						// Load address of destination 
  	EDMA.TCD[SE1dmaChTx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[SE1dmaChTx].DOFF = 0;                         		// Do not increment destination 
  	EDMA.TCD[SE1dmaChTx].DLAST_SGA = 0; 				          	// After major loop reset destination address
  	EDMA.TCD[SE1dmaChTx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
  
  	EDMA.TCD[SE1dmaChTx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[SE1dmaChTx].BITER = 10;		        	     		// Major loop iterations 
  	EDMA.TCD[SE1dmaChTx].CITER = 10;       		    	  		// Initialize current iteraction count 
  	EDMA.TCD[SE1dmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[SE1dmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[SE1dmaChTx].INT_MAJ = 1;
  	EDMA.TCD[SE1dmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[SE1dmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[SE1dmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[SE1dmaChTx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[SE1dmaChTx].E_SG = 0; 
  	EDMA.TCD[SE1dmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[SE1dmaChTx].DONE = 0;
  	EDMA.TCD[SE1dmaChTx].ACTIVE = 0;
  	EDMA.TCD[SE1dmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = SE1dmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = SE1dmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = SE1dmaChTx;           	// DMA Clear error Register 


  	// Initialize Receive Comm DMA

 	EDMA.CERQR.R = SE1dmaChRx;      		// Disable group EDMA channel   
	  
  	EDMA.CPR[SE1dmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[SE1dmaChRx].R = SE1dmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[SE1dmaChRx].SADDR = SE1dr;  						// Load address of source data 
  	EDMA.TCD[SE1dmaChRx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[SE1dmaChRx].SOFF = 0;                         		// Do not increment source address
	  	
  	EDMA.TCD[SE1dmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
  	EDMA.TCD[SE1dmaChRx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[SE1dmaChRx].DADDR =  (uint32) &sx_recbuf[SE1COM][0]; 			// Load address of destination 
  	EDMA.TCD[SE1dmaChRx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[SE1dmaChRx].DOFF = 1;                         		// Increment destination addr by 1 
  	EDMA.TCD[SE1dmaChRx].DLAST_SGA = (0 - sx_rbufsize);           // After major loop reset destination address
  	EDMA.TCD[SE1dmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
	  
  	EDMA.TCD[SE1dmaChRx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[SE1dmaChRx].BITER = sx_rbufsize;             		// Major loop iterations 
  	EDMA.TCD[SE1dmaChRx].CITER = sx_rbufsize;             		// Initialize current iteraction count 
  	EDMA.TCD[SE1dmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
  	EDMA.TCD[SE1dmaChRx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[SE1dmaChRx].INT_MAJ = 0;
  	EDMA.TCD[SE1dmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[SE1dmaChRx].BITERE_LINK = 0;
  	EDMA.TCD[SE1dmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[SE1dmaChRx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[SE1dmaChRx].E_SG = 0; 
  	EDMA.TCD[SE1dmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[SE1dmaChRx].DONE = 0;
  	EDMA.TCD[SE1dmaChRx].ACTIVE = 0;
  	EDMA.TCD[SE1dmaChRx].START = 0;                        		// Initialize status flags 
	  
  	EDMA.CEEIR.R = SE1dmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = SE1dmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = SE1dmaChRx;           	// DMA Clear error Register 
  	EDMA.SERQR.R = SE1dmaChRx;				// Enable group coomm DMA channel
  

	// Enable must be last
	SE1port.CR1.B.TE = 1;			// Transmitter Enable
	SE1port.CR1.B.RE = 1;			// Receive Enable
	
	_enable();
}


//****************************************************
// Initialize serial expansion comm ports
//****************************************************

void init_sx_com_port_2(void)
{
	uint16 data_in;
	
	_disable();

	// Serial Expansion Comm
		
  	/* Install our handlers */  
  	INTC_InstallINTCInterruptHandler(sx_com_2_int, SE2intLoc, 1);  

	SE2txDis; 						// Turn off Transmit driver

	SE2port.CR1.B.TE = 0;			// Transmitter Disable
	SE2port.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable

	// Control register 1	  
	SE2port.LCR.B.LIN = 0;			// Place in SCI mode	
	SE2port.CR1.B.SBR = sx_com_baud[SE2COM];
	
	SE2port.CR1.B.LOOPS = 0;		// Receiver source mode LOOPS:RSRC 00 = Dual wire mode
	SE2port.CR1.B.RSRC = 0;
	

	SE2port.CR1.B.M = 0;			// Frame Format Mode: set to 1 for nine bits
	SE2port.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
	SE2port.CR1.B.PE = 0;			// Parity Enable: 0 = disabled, 1 = enabled 
	SE2port.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd
	SE2port.CR3.B.M2 = 0;
 
 	SE2port.CR3.B.EROE = 0;			// Set error bit in receive data register 
	SE2port.CR3.B.ERFE = 0;			// Set error bit in receive data register
	SE2port.CR3.B.ERPE = 0;			// Set error bit in receive data register


	SE2port.CR1.B.ILT = 0;			// Idle Line Type: 0 = idle line detection after low bit
	SE2port.CR1.B.TIE = 0;			// Transmit Interrupt Enable (data tranferred to shift register)
	SE2port.CR1.B.TCIE = 0;			// Transmit Complete Interrupt Enable (data shifted out of shift register)
	SE2port.CR1.B.RIE = 0;			// Receive Interrupt Enable
	SE2port.CR1.B.ILIE = 0;			// Idle line interrupt enable
	SE2port.CR1.B.RWU = 0;			// Receiver Wake Up mode: 0 = normal, 1 = wakeup on 9th bit address
	SE2port.CR1.B.SBK = 0;			// Send Break Character
	
	// Control Register 2
	SE2port.CR2.B.MDIS = 0;			// Module Disable
	SE2port.CR2.B.FBR = 0;			// Fast Bit Error Detect: 0 = normal, 1 = use in LIN mode	
	SE2port.CR2.B.BSTP = 0;			// DMA Stop on Bit Error: 0 = tx dma regardless of bit errors
	SE2port.CR2.B.IEBERR = 0;		// Bit Error Interrupt Enable
	SE2port.CR2.B.RXDMA = 1;		// Receive DMA Control: 0 = disabled, 1 = enabled
	SE2port.CR2.B.TXDMA = 0;		// Transmit DMA Control: 0 = disabled, 1 = enabled
	SE2port.CR2.B.BRK13 = 0;		// Break Character Length
	SE2port.CR2.B.TXDIR = 0;		// TXD pin output enable: Used in signel wire mode
	SE2port.CR2.B.BESM13 = 0;		// Fast Bit Error Detection Sample Mode: 0=RS9,1=RS10 LIN mode only
	SE2port.CR2.B.SBSTP = 0;		// Transmit Stop on Bit Error
	SE2port.CR2.B.RXPOL = 0;		// RXD Pin polarity: 0 = normal, 1 = inverted
	SE2port.CR2.B.PMSK = 0;			// Parity Bit Masking: 0 = in bit position, 1 = 0 in parity position	
	SE2port.CR2.B.ORIE = 0;			// Overrun Interrupt Enable
	SE2port.CR2.B.NFIE = 0;			// Noise Interrupt Enable
	SE2port.CR2.B.FEIE = 0;			// Frame Error Interrupt Enable
	SE2port.CR2.B.PFIE = 0;			// Parity Error Interrupt Enable
	
	data_in = SE2port.SDR.R;
	data_in = SE2port.SDR.R;

	// Set RTS line high or low here when pin defined *******
	SE2txEn;

	sx_Tranbuf_Empty[SE2COM] = 1;
	sx_Tranbuf_Full[SE2COM] = 0;
	sx_Start_Tx[SE2COM] = 0;
	sx_tbs[SE2COM] = 0;
	sx_tbf[SE2COM] = 0;
	sx_rbs[SE2COM] = 0;
	sx_rbf[SE2COM] = 0;
	video_sx_rbf[SE2COM] = 0;
	video_sx_tbf[SE2COM] = 0;

  	// Initialize Transmit Comm DMA

 	EDMA.CERQR.R = SE2dmaChTx;      		// Disable group EDMA channel   
  
	INTC_InstallINTCInterruptHandler(se2_dma_trm_finished_int, SE2dmaTxIntLoc, 1);  

  	EDMA.CPR[SE2dmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[SE2dmaChTx].R = SE2dmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[SE2dmaChTx].SADDR = (uint32) &sx_tranbuf[SE2COM][0];  						// Load address of source data 
  	EDMA.TCD[SE2dmaChTx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[SE2dmaChTx].SOFF = 1;                         		// Increment source address by 1
  	
  	EDMA.TCD[SE2dmaChTx].SLAST = -10;                     		// After major loop, reset src addr 
  	EDMA.TCD[SE2dmaChTx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[SE2dmaChTx].DADDR =  SE2dr; 						// Load address of destination 
  	EDMA.TCD[SE2dmaChTx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[SE2dmaChTx].DOFF = 0;                         		// Do not increment destination 
  	EDMA.TCD[SE2dmaChTx].DLAST_SGA = 0; 				          	// After major loop reset destination address
  	EDMA.TCD[SE2dmaChTx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
  
  	EDMA.TCD[SE2dmaChTx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[SE2dmaChTx].BITER = 10;		        	     		// Major loop iterations 
  	EDMA.TCD[SE2dmaChTx].CITER = 10;       		    	  		// Initialize current iteraction count 
  	EDMA.TCD[SE2dmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[SE2dmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[SE2dmaChTx].INT_MAJ = 1;
  	EDMA.TCD[SE2dmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[SE2dmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[SE2dmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[SE2dmaChTx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[SE2dmaChTx].E_SG = 0; 
  	EDMA.TCD[SE2dmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[SE2dmaChTx].DONE = 0;
  	EDMA.TCD[SE2dmaChTx].ACTIVE = 0;
  	EDMA.TCD[SE2dmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = SE2dmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = SE2dmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = SE2dmaChTx;           	// DMA Clear error Register 


  	// Initialize Receive Comm DMA

 	EDMA.CERQR.R = SE2dmaChRx;      		// Disable group EDMA channel   
	  
  	EDMA.CPR[SE2dmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[SE2dmaChRx].R = SE2dmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[SE2dmaChRx].SADDR = SE2dr;  						// Load address of source data 
  	EDMA.TCD[SE2dmaChRx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[SE2dmaChRx].SOFF = 0;                         		// Do not increment source address
	  	
  	EDMA.TCD[SE2dmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
  	EDMA.TCD[SE2dmaChRx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[SE2dmaChRx].DADDR =  (uint32) &sx_recbuf[SE2COM][0]; 			// Load address of destination 
  	EDMA.TCD[SE2dmaChRx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[SE2dmaChRx].DOFF = 1;                         		// Increment destination addr by 1 
  	EDMA.TCD[SE2dmaChRx].DLAST_SGA = (0 - sx_rbufsize);           // After major loop reset destination address
  	EDMA.TCD[SE2dmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
	  
  	EDMA.TCD[SE2dmaChRx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[SE2dmaChRx].BITER = sx_rbufsize;             		// Major loop iterations 
  	EDMA.TCD[SE2dmaChRx].CITER = sx_rbufsize;             		// Initialize current iteraction count 
  	EDMA.TCD[SE2dmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
  	EDMA.TCD[SE2dmaChRx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[SE2dmaChRx].INT_MAJ = 0;
  	EDMA.TCD[SE2dmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[SE2dmaChRx].BITERE_LINK = 0;
  	EDMA.TCD[SE2dmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[SE2dmaChRx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[SE2dmaChRx].E_SG = 0; 
  	EDMA.TCD[SE2dmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[SE2dmaChRx].DONE = 0;
  	EDMA.TCD[SE2dmaChRx].ACTIVE = 0;
  	EDMA.TCD[SE2dmaChRx].START = 0;                        		// Initialize status flags 
	  
  	EDMA.CEEIR.R = SE2dmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = SE2dmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = SE2dmaChRx;           	// DMA Clear error Register 
  	EDMA.SERQR.R = SE2dmaChRx;				// Enable group coomm DMA channel
 

	// Enable must be last
	SE2port.CR1.B.TE = 1;			// Transmitter Enable
	SE2port.CR1.B.RE = 1;			// Receive Enable
	
	_enable();
}

//**************************************************
// This is start the SE1COM transmission on a COM Port
//**************************************************

void se1_tx_dma_start(int16 len)
{
	
	_disable();	   
	
	// Enable transmitter
	EDMA.CER.R = SE1dmaChTx;           	// DMA Clear error Register 
 	SE1port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	sx_Tranbuf_Empty[SE1COM] = 0;
    EDMA.TCD[SE1dmaChTx].SADDR = (uint32) &sx_tranbuf[SE1COM][0];  	// Load address of source data 
  	EDMA.TCD[SE1dmaChTx].SLAST = (0-len);        // After major loop, reset src addr 
	EDMA.TCD[SE1dmaChTx].NBYTES = 1;	// Transfer len number byte per minor loop 
 	EDMA.TCD[SE1dmaChTx].BITER = len;   // 10 major loop iterations 
  	EDMA.TCD[SE1dmaChTx].CITER = len;   // Initialize current iteration count 
  	EDMA.SERQR.R = SE1dmaChTx;      	// Enable Group EDMA channel   
	SE1port.CR2.B.TXDMA = 1;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	sx_dma_start_cnt[0]++;
//	SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25
	_enable();
}

//*********************************************
// SE1COM DMA transmit finished interrupt
//*********************************************

void se1_dma_trm_finished_int(void)
{
	
  	EDMA.CIRQR.R = SE1dmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = SE1dmaChTx;      	// Disable Drive EDMA channel   
	SE1port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	sx_Tranbuf_Empty[SE1COM] = 1;
	sx_dma_end_cnt[0]++;
}

//**************************************************
// This is start the SE2COM transmission on a COM Port
//**************************************************

void se2_tx_dma_start(int16 len)
{
	
	_disable();	   
	
	// Enable transmitter
	EDMA.CER.R = SE2dmaChTx;           	// DMA Clear error Register 
 	SE2port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	sx_Tranbuf_Empty[SE2COM] = 0;
    EDMA.TCD[SE2dmaChTx].SADDR = (uint32) &sx_tranbuf[SE2COM][0];  	// Load address of source data 
  	EDMA.TCD[SE2dmaChTx].SLAST = (0-len);        // After major loop, reset src addr 
	EDMA.TCD[SE2dmaChTx].NBYTES = 1;	// Transfer len number byte per minor loop 
 	EDMA.TCD[SE2dmaChTx].BITER = len;   // 10 major loop iterations 
  	EDMA.TCD[SE2dmaChTx].CITER = len;   // Initialize current iteraction count 
  	EDMA.SERQR.R = SE2dmaChTx;      	// Enable Group EDMA channel   
	SE2port.CR2.B.TXDMA = 1;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	sx_dma_start_cnt[1]++;
	_enable();
}

//*********************************************
// SE2COM DMA transmit finished interrupt
//*********************************************

void se2_dma_trm_finished_int(void)
{
	
  	EDMA.CIRQR.R = SE2dmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = SE2dmaChTx;      	// Disable Drive EDMA channel   
	SE2port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	sx_Tranbuf_Empty[SE2COM] = 1;
	sx_dma_end_cnt[1]++;
}


/************************************************
 * initialize the serial communications port
 * installs interrupt, sets speed, etc
 ************************************************/
void sx_initcom(void)
{
	// init comm A

	sx_com_baud[SE1COM] = sx_baud_tbl[fvars[fvupsbaud]];
	init_sx_com_port_1();

	timers[tsx_txA] = 0;
	timers[tsx_rxA] = 0;

	// init comm B
	
	sx_com_baud[SE2COM] = sx2_baud_tbl[fvars[fv485baud]];
	init_sx_com_port_2();

	timers[tsx_txB] = 0;
	timers[tsx_rxB] = 0;
}


//****************************************
// Test to see if receive buffer is empty
//****************************************
int16 sx_isrxempty(int16 comnmb)
{
	static int16 ite_count, beg_count, dma_count;

	if (comnmb == SE1COM)
	{
		beg_count = (int16)EDMA.TCD[SE1dmaChRx].BITER;
		ite_count = (int16)EDMA.TCD[SE1dmaChRx].CITER;
		dma_count = (int16)(beg_count - ite_count);
		
		if (sx_rbs[comnmb] == dma_count) 
	      	return(1);
	    else
	      	return(0);
	}
	else if (comnmb == SE2COM)
	{
		beg_count = (int16)EDMA.TCD[SE2dmaChRx].BITER;
		ite_count = (int16)EDMA.TCD[SE2dmaChRx].CITER;
		dma_count = (int16)(beg_count - ite_count);
		
		if (sx_rbs[comnmb] == dma_count) 
	      	return(1);
	    else
	      	return(0);
	}
	return(0);
}


//*********************************************
// Test to see if the data transmit is complete
//*********************************************
int16 sx_istxcomplete(int16 comnmb)
{
	if ((sx_Start_Tx[comnmb] == 0) && (sx_tbs[comnmb] == sx_tbf[comnmb]))
		return(1);
	else
		return(0);
}


//*********************************************
// Test to see if the DMA data transmit is complete
//*********************************************
int16 sx_istxDMAcomplete(int16 comnmb)
{
	if (comnmb == SE1COM)
	{
		if (EDMA.TCD[SE1dmaChTx].ACTIVE == 0)
			return(1);
	}
	else if (comnmb == SE2COM)
	{
		if (EDMA.TCD[SE2dmaChTx].ACTIVE == 0)
			return(1);
	}
	
	return(0);
}

//*********************************************
// This is send a character out on a COM Port
//*********************************************
void sx_sendc( int16 comnmb, int16 letter)
{
    if(comnmb <= SE2COM) 
    {
		_disable();
		sx_tranbuf[comnmb][sx_tbs[comnmb]] = (uint8)letter;
		sx_tbs[comnmb]++;
		if(sx_tbs[comnmb] >= sx_tbufsize)
			sx_tbs[comnmb] = 0;
		if (sx_tbs[comnmb] >= sx_tbf[comnmb])
		{
			if ((sx_tbf[comnmb] + (sx_tbufsize - sx_tbs[comnmb])) < 50)
				sx_Tranbuf_Full[comnmb] = 1;
		}
		else
		{
			if ((sx_tbf[comnmb] - sx_tbs[comnmb]) < 50)
				sx_Tranbuf_Full[comnmb] = 1;
		}
		
		sx_Tranbuf_Empty[comnmb] = 0;
		if (comnmb == SE1COM)
			SE1port.CR1.B.TIE = 1;			// Transmit Interrupt Enabled
		else if (comnmb == SE2COM)
			SE2port.CR1.B.TIE = 1;			// Transmit Interrupt Enabled
		
		_enable();
    }
}

void sx_enable_tx(int16 comnmb)
{
	if (comnmb == SE1COM)
		SE1txEn; 						// Turn on Transmit driver
	else if (comnmb == SE2COM)
		SE2txEn; 						// Turn on Transmit driver
	
	sx_Start_Tx[comnmb] = 1;
}


/* Revision History
  5/18/09 v5.48.6 mhd		1. Initial release.
  4/27/10 v5.52.4 mhd		1. Use sx_isrxempty() instead of isrxempty().
  7/26/10 v5.52.15 mhd		1. Don't set INT pin active in sx_enable_tx().
  							2. Call init_sx_com_port() in unpack_apc_ups_com() when comm times out.	  '
							3. Clear sx_Start_Tx[] flag in init_sx_com_port().
							4. Clear ups_packet_req in unpack_eaton_ups_com().
  7/28/10 v5.52.17 mhd		1. Reset the comm 5 times before declaring the ups off line.
  
  8/11/17 v8.0.6 mhd		1. Moved IGEP, UPS and DL20 comm routines into seperate files. 
  8/14/17 v8.0.7 mhd		1. Changed transmit to use interrupts or DMA.
*/
