#define d_DPYSPI 1
#include "global.h"

/* Display SPI port
*/

void init_dpyspi (void);
void init_DPYSPI_DMA(void);
void dpyspi_start(uint16 offset);
void dpyspi_dma_rx_int(void);
void LCD_Buffer_to_spi (void);
void set_contrast(int16 new_contrast);
void set_brightness(int16 new_bright);
int16 get_contrast(void);
int16 get_brightness(void);
void put_dpyspi_data(uint8 dpyspi_data);
uint16 get_dpyspi_data (void);
void write_dpyspi(int16 row);
void load_special_char (void);


#define DSPI_A_Port 0xFFF90000L
#define DPYSPIdr DSPI_A_Port + 0x0034
#define DPYSPIsr DSPI_A_Port + 0x003A		// Not reading the status (38-39)
#define DPYSPIdmaRxIntLoc eDMA_6_Int

#define DPYSPIdmaTX 0x92
#define DPYSPIdmaRX 0x93

#define c_dpyspi_dma_buf_len 20
uint32 dpyspi_tx_dma_buf[c_dpyspi_dma_buf_len * 4];
uint16 dpyspi_rx_dma_buf[c_dpyspi_dma_buf_len * 4];

uint8 dpyspi_transferred = 1;
uint8 dpyspi_req = 0;
uint8 put_dpyspi = 0;
int8 dpy_line = 0;
uint8 upd_l0 = 0;
uint8 upd_l1 = 0;
uint8 upd_l2 = 0;
uint8 upd_l3 = 0;
int16 contrast = 40;
int16 brightness = 8;

char LCD_Display_prev[4][20];

#define c_DPYSPI_START 'S'


void init_dpyspi (void)
{
	int16 i;
	
	_disable();
	
	DSPI_A.MCR.B.HALT = 1;		// Stop SPI from running
	DSPI_A.MCR.B.MSTR = 1;		// Master Mode
	DSPI_A.MCR.B.CONT_SCKE = 0;	// Continuous SCK disabled
	DSPI_A.MCR.B.DCONF = 0;		// SPI mode
	DSPI_A.MCR.B.FRZ = 0;		// Do not freeze SPI during debug
	DSPI_A.MCR.B.MTFE = 0;		// No modified timing format
	DSPI_A.MCR.B.PCSSE = 0;		// No Peripheral Chip Select Strobe 
	DSPI_A.MCR.B.ROOE = 0; 		// Ignore overflow incomming data
	DSPI_A.MCR.B.PCSIS0 = 1;	// Peripheral Chip select active low state
	DSPI_A.MCR.B.PCSIS1 = 1;	// Peripheral Chip select active low state
	DSPI_A.MCR.B.PCSIS2 = 1;	// Peripheral Chip select active low state
	DSPI_A.MCR.B.PCSIS3 = 1;	// Peripheral Chip select active low state
	DSPI_A.MCR.B.PCSIS4 = 1;	// Peripheral Chip select active low state
	DSPI_A.MCR.B.PCSIS5 = 1;	// Peripheral Chip select active low state
	DSPI_A.MCR.B.DIS_TXF = 0;	// Allow Transmit Fifo
	DSPI_A.MCR.B.DIS_RXF = 0;	// Allow Receive Fifo
	DSPI_A.MCR.B.CLR_TXF = 1;	// Clear transmit fifo
	DSPI_A.MCR.B.CLR_RXF = 1;	// Clear receive fifo
	DSPI_A.MCR.B.SMPL_PT = 0;	// Sample point 0 - Not using modified timing format
	
	DSPI_A.TCR.B.SPI_TCNT = 0;	// Reset SPI transfer Count
	
	for (i=0;i<=7;i++)
	{
		DSPI_A.CTAR[i].B.FMSZ = 7;		// Transfer 8 bits
		DSPI_A.CTAR[i].B.CPOL = 1;		// Clock polarity is low
		DSPI_A.CTAR[i].B.CPHA = 1;		// Clock phase is leading edge of SCK
		DSPI_A.CTAR[i].B.LSBFE = 0;		// MSB first 
		
		// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 1024 = 9.416 usec (use one clock width)
		DSPI_A.CTAR[i].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
		DSPI_A.CTAR[i].B.CSSCK = 9;		// PCS to SCK  set to  1024  
		
		// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 512 = 4.708 usec (use 1/2 clock width) 
		DSPI_A.CTAR[i].B.PASC = 0;		// Prescaler = 1				
		DSPI_A.CTAR[i].B.ASC = 8;		// After SCK delay set to 512 
		
		// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 2048 = 18.832 usec (use 2 clock widths)
		DSPI_A.CTAR[i].B.PDT = 0;		// Prescaler = 1
		DSPI_A.CTAR[i].B._DT = 10;			// CS Delay Time  
																				 
		// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 512 = 106.20117875 KHz
		DSPI_A.CTAR[i].B.DBR = 0;		// No double baud rate
		DSPI_A.CTAR[i].B.PBR = 1;		// Baud rate clock prescale (divide by 2) ** 1 = DEVICE BY 3 FOR TEST 70.8 KHz
		DSPI_A.CTAR[i].B.BR = 9;		// Baud rate prescaler (divide by 512)
	}
	
	// Clear the status register bits
	DSPI_A.SR.B.TCF = 1;			// Clear transfer complete flag
	DSPI_A.SR.B.TFUF = 1;			// Clear transmit underflow flag
	DSPI_A.SR.B.TFFF = 1; 			// Clear transmit fifo fill flag
	DSPI_A.SR.B.RFOF = 1;			// Clear receive fifo overflow flag
	DSPI_A.SR.B.RFDF = 1;			// Clear receive fifo drain flag

	
	// Set up DMA request for RTC SPI
	DSPI_A.RSER.B.TCF_RE = 0;		// Disable transmission request interrupt
	DSPI_A.RSER.B.EOQF_RE = 0;		// Disable DSPI finished interrupt
	DSPI_A.RSER.B.TFUF_RE = 0;		// Disable Transmit fifo underflow interrupt
	DSPI_A.RSER.B.RFOF_RE = 0;		// Disable Receive Fifo Overflow interrupt

//	DSPI_A.RSER.B.TFFF_RE = 0;		// Leave Disabled for now: Enable Transit Fifo Fill
//	DSPI_A.RSER.B.RFDF_RE = 0;		// Leave Disabled for not: Enable Receive FIFO drain request

	DSPI_A.RSER.B.TFFF_RE = 0;		// Enable Transit Fifo Fill
	DSPI_A.RSER.B.TFFF_DIRS = 0; 	// Set to generate DMA request for transmit fifo
	DSPI_A.RSER.B.RFOF_RE = 0;		// Disable Receive Fifo Overflow interrupt
	DSPI_A.RSER.B.RFDF_RE = 0;		// Enable Receive FIFO drain request
	DSPI_A.RSER.B.RFDF_DIRS = 0;	// Set to generate DMA request for receive fifo
	
	
	DSPI_A.MCR.B.HALT = 0;		// Allow SPI to run
	DSPI_A.MCR.B.MDIS = 0;		// No Module disable

	SIU.PCR[DPY_CLK].R = 0x60C;		
	SIU.PCR[DPY_OUT].R = 0x60C;		
	SIU.PCR[DPY_IN].R = 0x500;
	SIU.PCR[DPY_CS].R = 0x60C;		// Chip select

	init_DPYSPI_DMA();
	
	// preload the command word
    for(i=0; i<(c_dpyspi_dma_buf_len * 4); i++)
    {
    	dpyspi_tx_dma_buf[i] = 0x00010000;
    }

//	DSPI_A.RSER.B.TFFF_DIRS = 1; 	// Set to generate DMA request for transmit fifo
//	DSPI_A.RSER.B.RFDF_DIRS = 1;	// Set to generate DMA request for receive fifo
//	DSPI_A.RSER.B.TFFF_RE = 1;		// Enable Transit Fifo Fill
//	DSPI_A.RSER.B.RFDF_RE = 1;		// Enable Receive FIFO drain request
	
	delay(1000);			// delay to allow LCD to reset

	put_dpyspi_data(0xFE);
	put_dpyspi_data(0x41);		// Turn on display
	delay(1000);			// delay to allow LCD to reset
	put_dpyspi_data(0xFE);
	put_dpyspi_data(0x51);		// Clear screen
	delay(12000);			// delay to allow LCD to reset
	put_dpyspi_data(0xFE);
	put_dpyspi_data(0x53);		// Set Backlight Brightness
	put_dpyspi_data(0x08);		// value between 1 and 8

	
	load_special_char();
	
	if (contrast < 25)
	{
		set_contrast(25);
	}
	
	_enable();
}


//*****************************************
// DMA Initialization for Display SPI Port
//*****************************************


void init_DPYSPI_DMA(void) 
{

	// Initialize Transmit Comm DMA
  
  	EDMA.CERQR.R = DPYSPIdmaChTx;      	// Disable Display SPI transmit EDMA channel   


  	EDMA.CPR[DPYSPIdmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[DPYSPIdmaChTx].R = DPYSPIdmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[DPYSPIdmaChTx].SADDR = (uint32) &dpyspi_tx_dma_buf[0]; // Load address of source data 
  	EDMA.TCD[DPYSPIdmaChTx].SSIZE = 2;                        		// Read 2**0 = 4 byte per transfer 
  	EDMA.TCD[DPYSPIdmaChTx].SOFF = 4;                         		// After transfer, add 4 to src addr
  	
  	EDMA.TCD[DPYSPIdmaChTx].SLAST = - (c_dpyspi_dma_buf_len * 4); 	// After major loop, reset src addr 
  	EDMA.TCD[DPYSPIdmaChTx].SMOD = 0;                         		// Source modulo feature not used 

  	EDMA.TCD[DPYSPIdmaChTx].DADDR =  DPYSPIdr; 						// Load address of destination register
  	EDMA.TCD[DPYSPIdmaChTx].DSIZE = 2;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[DPYSPIdmaChTx].DOFF = 0;                         		// Do not increment destination addr 
  	EDMA.TCD[DPYSPIdmaChTx].DLAST_SGA = 0;                    		// After major loop, no dest addr change 
  	EDMA.TCD[DPYSPIdmaChTx].DMOD = 0;                         		// Destination modulo feature not used 
  
  	EDMA.TCD[DPYSPIdmaChTx].NBYTES= 4;                    			// Transfer 1 byte per minor loop 
  	EDMA.TCD[DPYSPIdmaChTx].BITER = c_dpyspi_dma_buf_len;           // 1 major loop iterations 
  	EDMA.TCD[DPYSPIdmaChTx].CITER = c_dpyspi_dma_buf_len;           // Initialize current iteraction count 
  	EDMA.TCD[DPYSPIdmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[DPYSPIdmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[DPYSPIdmaChTx].INT_MAJ = 0;
  	EDMA.TCD[DPYSPIdmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[DPYSPIdmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[DPYSPIdmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[DPYSPIdmaChTx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[DPYSPIdmaChTx].E_SG = 0; 
  	EDMA.TCD[DPYSPIdmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[DPYSPIdmaChTx].DONE = 0;
  	EDMA.TCD[DPYSPIdmaChTx].ACTIVE = 0;
  	EDMA.TCD[DPYSPIdmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = DPYSPIdmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = DPYSPIdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = DPYSPIdmaChTx;           	// DMA Clear error Register 
  
  	
  	// Initialize Receive Comm DMA

 	EDMA.CERQR.R = DPYSPIdmaChRx;      		// Disable group EDMA channel   
  
	INTC_InstallINTCInterruptHandler(dpyspi_dma_rx_int, DPYSPIdmaRxIntLoc, 1);  

  	EDMA.CPR[DPYSPIdmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[DPYSPIdmaChRx].R = DPYSPIdmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[DPYSPIdmaChRx].SADDR = DPYSPIsr;  						// Load address of source register data 
  	EDMA.TCD[DPYSPIdmaChRx].SSIZE = 1;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[DPYSPIdmaChRx].SOFF = 0;                         		// Do not increment source address
  	
  	EDMA.TCD[DPYSPIdmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
  	EDMA.TCD[DPYSPIdmaChRx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[DPYSPIdmaChRx].DADDR =  (uint32) &dpyspi_rx_dma_buf[0]; 						// Load address of destination 
  	EDMA.TCD[DPYSPIdmaChRx].DSIZE = 1;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[DPYSPIdmaChRx].DOFF = 2;                         		// Increment destination addr by 2 
  	EDMA.TCD[DPYSPIdmaChRx].DLAST_SGA = - (c_dpyspi_dma_buf_len * 2);                    		// After major loop reset destination address
  	EDMA.TCD[DPYSPIdmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
  
  	EDMA.TCD[DPYSPIdmaChRx].NBYTES= 2;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[DPYSPIdmaChRx].BITER = c_dpyspi_dma_buf_len;            // Major loop iterations 
  	EDMA.TCD[DPYSPIdmaChRx].CITER = c_dpyspi_dma_buf_len;                       	// Initialize current iteraction count 
  	EDMA.TCD[DPYSPIdmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
  	EDMA.TCD[DPYSPIdmaChRx].INT_HALF = 0;                     		
  	EDMA.TCD[DPYSPIdmaChRx].INT_MAJ = 1;							// Interrupt after major loop
  	EDMA.TCD[DPYSPIdmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[DPYSPIdmaChRx].BITERE_LINK = 0;
  	EDMA.TCD[DPYSPIdmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[DPYSPIdmaChRx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[DPYSPIdmaChRx].E_SG = 0; 
  	EDMA.TCD[DPYSPIdmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[DPYSPIdmaChRx].DONE = 0;
  	EDMA.TCD[DPYSPIdmaChRx].ACTIVE = 0;
  	EDMA.TCD[DPYSPIdmaChRx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = DPYSPIdmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = DPYSPIdmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = DPYSPIdmaChRx;           	// DMA Clear error Register 
  
}

//**************************************************
// This is start the comm transmission on a COM Port
//**************************************************

void dpyspi_start(uint16 offset)
{
	dpyspi_transferred = 0;
	
	_disable();	   
	// Turn off the SPI transmit and receive fifo
	DSPI_A.RSER.B.TFFF_RE = 0;		// Disable Transit Fifo Fill
	DSPI_A.RSER.B.RFDF_RE = 0;		// Dosable Receive FIFO drain request
	DSPI_A.RSER.B.TFFF_DIRS = 0; 	// Clear generate DMA request for transmit fifo
	DSPI_A.RSER.B.RFDF_DIRS = 0;	// Clear generate DMA request for receive fifo
	
	// Enable transmitter
	EDMA.CER.R = DPYSPIdmaChTx;           	// DMA Clear error Register 
	EDMA.CER.R = DPYSPIdmaChRx;           	// DMA Clear error Register 

    EDMA.TCD[DPYSPIdmaChTx].SADDR = (uint32) &dpyspi_tx_dma_buf[offset];  	// Load address of source data 
  	EDMA.TCD[DPYSPIdmaChTx].BITER = c_dpyspi_dma_buf_len;                       		// 1 major loop iterations 
  	EDMA.TCD[DPYSPIdmaChTx].CITER = c_dpyspi_dma_buf_len;                       		// Initialize current iteraction count 

  	EDMA.TCD[DPYSPIdmaChRx].DADDR =  (uint32) &dpyspi_rx_dma_buf[0]; 						// Load address of destination 
  	EDMA.TCD[DPYSPIdmaChRx].BITER = c_dpyspi_dma_buf_len;            // Major loop iterations 
  	EDMA.TCD[DPYSPIdmaChRx].CITER = c_dpyspi_dma_buf_len;                       	// Initialize current iteraction count 

  	EDMA.SERQR.R = DPYSPIdmaChTx;      	// Enable Display SPI EDMA channel 
  	EDMA.SERQR.R = DPYSPIdmaChRx;      	// Enable Display SPI EDMA channel 
  	  
	DSPI_A.RSER.B.TFFF_DIRS = 1; 	// Set to generate DMA request for transmit fifo
	DSPI_A.RSER.B.RFDF_DIRS = 1;	// Set to generate DMA request for receive fifo
	DSPI_A.RSER.B.TFFF_RE = 1;		// Enable Transit Fifo Fill
	DSPI_A.RSER.B.RFDF_RE = 1;		// Enable Receive FIFO drain request
	_enable();
}

//*********************************************
// Display SPI DMA transmit finished interrupt
//*********************************************

void dpyspi_dma_rx_int(void)
{
	
  	EDMA.CIRQR.R = DPYSPIdmaChRx;      	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = DPYSPIdmaChRx;      	// Disable Display SPI EDMA channel   
	DSPI_A.RSER.B.TFFF_RE = 0;		// Disable Transit Fifo Fill
	DSPI_A.RSER.B.RFDF_RE = 0;		// Disable Receive FIFO drain request
	DSPI_A.RSER.B.TFFF_DIRS = 0; 	// Clear generate DMA request for transmit fifo
	DSPI_A.RSER.B.RFDF_DIRS = 0;	// Clear generate DMA request for receive fifo
	
	dpyspi_transferred = 1;

}

#define CS_1_DPY_SPI	0x00010000


void put_dpyspi_data(uint8 dpyspi_data)
{
	uint32 dontcare;
	uint16 count = 0;

	DSPI_A.SR.B.TCF = 1; 		// clear bit
	DSPI_A.PUSHR.R = CS_1_DPY_SPI | dpyspi_data; // Send data with fast clock to sd
  	while ((DSPI_A.SR.B.TCF != 1) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	dontcare = DSPI_A.POPR.R;    // Read data received by slave SPI 
  	DSPI_A.SR.R = 0x80020000;        // Clear TCF, RDRF flags by writing 1 to them 
}

uint16 get_dpyspi_data (void)
{
	uint32 RecDataSlave;
	uint16 count = 0;

	DSPI_A.SR.B.TCF = 1; 		// clear bit
	DSPI_A.PUSHR.R = 0x0001AA55; // Send data to rtc
  	while ((DSPI_A.SR.B.TCF != 1) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	RecDataSlave = DSPI_A.POPR.R;    // Read data received by slave SPI 
  	DSPI_A.SR.R = 0x80020000;        // Clear TCF, RDRF flags by writing 1 to them 
  	return ((uint16)RecDataSlave);
}


/*

Line 0 0x00 to 0x13
Line 1 0x40 to 0x53
Line 2 0x14 to 0x27
Line 3 0x54 to 0x67

*/

#define l0_off 0
#define l1_off 64
#define l2_off 20
#define l3_off 84

void LCD_Buffer_to_spi (void)
{  // tagwbg 05 LCD_Buffer_to_spi()
	uint16 i;

	if (dpyspi_timer > 2)		// ten msec timer
	{
		
		for(i=0; i<=19; i++)
		{
			if (LCD_Display[0][i] != LCD_Display_prev[0][i])
			{
				LCD_Display_prev[0][i] = LCD_Display[0][i];
				upd_l0 = 1;
			}
			if (LCD_Display[1][i] != LCD_Display_prev[1][i])
			{
				LCD_Display_prev[1][i] = LCD_Display[1][i];
				upd_l1 = 1;
			}
			if (LCD_Display[2][i] != LCD_Display_prev[2][i])
			{
				LCD_Display_prev[2][i] = LCD_Display[2][i];
				upd_l2 = 1;
			}
			if (LCD_Display[3][i] != LCD_Display_prev[3][i])
			{
				LCD_Display_prev[3][i] = LCD_Display[3][i];
				upd_l3 = 1;
			}
			
		}

		if ((dpyspi_transferred == 1) || (dpyspi_timer > 20))
		{
			// Transmit to display again
			dpyspi_timer = 0;
			
			
			switch (dpy_line)
			{
				case 0:
					if (upd_l0 == 1)
					{
						upd_l0 = 0;
						for (i=0;i<=19;i++)
						{
							dpyspi_tx_dma_buf[l0_off+i] = CS_1_DPY_SPI | LCD_Display[0][i];
						}
						put_dpyspi_data(0xFE);
						put_dpyspi_data(0x45);		// Locate Cursor at address
						put_dpyspi_data(0x00);		// Line 0 location @0x00
						dpyspi_start(l0_off);
						break;
					}
					
				case 1:
									
					if (upd_l1 == 1)
					{
						upd_l1 = 0;
						for (i=0;i<=19;i++)
						{
							dpyspi_tx_dma_buf[l1_off+i] = CS_1_DPY_SPI | LCD_Display[1][i];
						}
						put_dpyspi_data(0xFE);
						put_dpyspi_data(0x45);		// Locate Cursor at address
						put_dpyspi_data(0x40);		// Line 1 location @0x40
						dpyspi_start(l1_off);
						break;
					}
					
				case 2:
					if (upd_l2 == 1)
					{
						upd_l2 = 0;
						for (i=0;i<=19;i++)
						{
							dpyspi_tx_dma_buf[l2_off+i] = CS_1_DPY_SPI | LCD_Display[2][i];
						}
						put_dpyspi_data(0xFE);
						put_dpyspi_data(0x45);		// Locate Cursor at address
						put_dpyspi_data(0x14);		// Line 2 location @0x14
						dpyspi_start(l2_off);
						break;
					}
				
				case 3:
					if (upd_l3 == 1)
					{
						upd_l3 = 0;
						for (i=0;i<=19;i++)
						{
							dpyspi_tx_dma_buf[l3_off+i] = CS_1_DPY_SPI | LCD_Display[3][i];
						}
						put_dpyspi_data(0xFE);
						put_dpyspi_data(0x45);		// Locate Cursor at address
						put_dpyspi_data(0x54);		// Line 3 location @0x54
						dpyspi_start(l3_off);
						break;
					}
					
				
				default:
					break;
			}
			
			dpy_line++;
			if (dpy_line > 3)
				dpy_line = 0;
			
			
		}
	}
}



//******************************************
//*** Set the Contrast
//******************************************

void set_contrast(int16 new_contrast)
{
	dpyspi_timer = 0;				// Allow time for contrast to be updated
	if ((new_contrast >= 1) && (new_contrast <= 50))
	{
		contrast = new_contrast;
		put_dpyspi_data(0xFE);
		put_dpyspi_data(0x52);			// Set Contrast command
		put_dpyspi_data((uint8)contrast);		// Send Contrast value between 1 and 50, default = 40
		Update_Contrast_Brightness();
	}
}

int16 get_contrast(void)
{
	return contrast;
}


//******************************************
//*** Set the Brightness
//******************************************

void set_brightness(int16 new_bright)
{
	dpyspi_timer = 0;				// Allow time for brightness to be updated
	if ((new_bright >= 1) && (new_bright <= 8))
	{
		brightness = new_bright;
		put_dpyspi_data(0xFE);
		put_dpyspi_data(0x53);			// Set Contrast command
		put_dpyspi_data((uint8)brightness);	// Send Brightness value between 1 and 8, default = 8
		Update_Contrast_Brightness();
	}
}

int16 get_brightness(void)
{
	return brightness;
}


//******************************************
//*** Write Display SPI
//******************************************

void write_dpyspi(int16 row)
{  // tagwbg 03 write_dpyspi
	int16 i;
	int16 count=0;
	
	dpyspi_timer = 0;
	while ((dpyspi_transferred == 0) && (count < 10000))
	{
		count++;
		if (dpyspi_timer > 10)
			break;
	}
	
	if (row == 0)
	{
		for (i=0;i<=19;i++)
		{
			dpyspi_tx_dma_buf[l0_off+i] = CS_1_DPY_SPI | LCD_Display[0][i];
		}
		put_dpyspi_data(0xFE);
		put_dpyspi_data(0x45);		// Locate Cursor at address
		put_dpyspi_data(0x00);		// Line 0 location @0x00
		dpyspi_start(l0_off);
	}
	else if (row == 1)
	{
		for (i=0;i<=19;i++)
		{
			dpyspi_tx_dma_buf[l1_off+i] = CS_1_DPY_SPI | LCD_Display[row][i];
		}
		put_dpyspi_data(0xFE);
		put_dpyspi_data(0x45);		// Locate Cursor at address
		put_dpyspi_data(0x40);		// Line 1 location @0x40
		dpyspi_start(l1_off);
	}
	else if (row == 2)
	{
		for (i=0;i<=19;i++)
		{
			dpyspi_tx_dma_buf[l2_off+i] = CS_1_DPY_SPI | LCD_Display[row][i];
		}
		put_dpyspi_data(0xFE);
		put_dpyspi_data(0x45);		// Locate Cursor at address
		put_dpyspi_data(0x14);		// Line 2 location @0x14
		dpyspi_start(l2_off);
	}
	else if (row == 3)
	{
		for (i=0;i<=19;i++)
		{
			dpyspi_tx_dma_buf[l3_off+i] = CS_1_DPY_SPI | LCD_Display[row][i];
		}
		put_dpyspi_data(0xFE);
		put_dpyspi_data(0x45);		// Locate Cursor at address
		put_dpyspi_data(0x54);		// Line 3 location @0x54
		dpyspi_start(l3_off);
	}
}

#define io_off 			0x00
#define io_on 			0x01
#define motor_on		0x02
#define valve_on		0x03
#define up_arrow 		0x04
#define dn_arrow		0x05
#define doors_closed 	0x06
#define fdoor_open		0x07
#define rdoor_open		'_'

void load_special_char (void)
{
// tagwbg 17
	delay(1500);			// delay to allow LCD to update
	
	put_dpyspi_data(0xFE);		// (IO OFF)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x00);		// character address 
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00100 - 0x04
	put_dpyspi_data(0x00);		// 000 01010 - 0x0A
	put_dpyspi_data(0x00);		// 000 10001 - 0x11
	put_dpyspi_data(0x00);		// 000 10001 - 0x11
	put_dpyspi_data(0x00);		// 000 01010 - 0x0A
	put_dpyspi_data(0x00);		// 000 00100 - 0x04
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
		

	put_dpyspi_data(0xFE);		// (ALL SEGS ON - Button pressed)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x01);		// character address 
	put_dpyspi_data(0xFF);		// 000 00000 - 0xFF
	put_dpyspi_data(0xFF);		// 000 00100 - 0xFF
	put_dpyspi_data(0xFF);		// 000 01010 - 0xFF
	put_dpyspi_data(0xFF);		// 000 10001 - 0xFF
	put_dpyspi_data(0xFF);		// 000 10001 - 0xFF
	put_dpyspi_data(0xFF);		// 000 01010 - 0xFF
	put_dpyspi_data(0xFF);		// 000 00100 - 0xFF
	put_dpyspi_data(0xFF);		// 000 00000 - 0xFF

	delay(1500);			// delay to allow LCD to update
/*
	put_dpyspi_data(0xFE);		// (IO ON)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x01);		// character address 
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x04);		// 000 00100 - 0x04
	put_dpyspi_data(0x0E);		// 000 01110 - 0x0E
	put_dpyspi_data(0x1F);		// 000 11111 - 0x1F
	put_dpyspi_data(0x1F);		// 000 11111 - 0x1F
	put_dpyspi_data(0x0E);		// 000 01110 - 0x0E
	put_dpyspi_data(0x04);		// 000 00100 - 0x04
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
*/
	delay(1500);			// delay to allow LCD to update
	
	put_dpyspi_data(0xFE);		// (Motor On)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x02);		// character address 
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x1E);		// 000 11110 - 0x1E
	put_dpyspi_data(0x13);		// 000 10011 - 0x13
	put_dpyspi_data(0x13);		// 000 10011 - 0x13
	put_dpyspi_data(0x13);		// 000 10011 - 0x13
	put_dpyspi_data(0x1E);		// 000 11110 - 0x1E
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
		
	delay(1500);			// delay to allow LCD to update
	
	put_dpyspi_data(0xFE);		// (Valve ON)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x03);		// character address 
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x1B);		// 000 11011 - 0x1B
	put_dpyspi_data(0x15);		// 000 10101 - 0x15
	put_dpyspi_data(0x15);		// 000 10101 - 0x15
	put_dpyspi_data(0x15);		// 000 10101 - 0x15
	put_dpyspi_data(0x1B);		// 000 11011 - 0x1B
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
		
	delay(1500);			// delay to allow LCD to update
	
	put_dpyspi_data(0xFE);		// (Up Arror)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x04);		// character address 
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x04);		// 000 00100 - 0x04
	put_dpyspi_data(0x0E);		// 000 01110 - 0x0E
	put_dpyspi_data(0x1F);		// 000 11111 - 0x1F
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
		
	delay(1500);			// delay to allow LCD to update
	
	put_dpyspi_data(0xFE);		// (Dn Arrow)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x05);		// character address 
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x1F);		// 000 11111 - 0x1F
	put_dpyspi_data(0x0E);		// 000 01110 - 0x0E
	put_dpyspi_data(0x04);		// 000 00100 - 0x04
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
		
	delay(1500);			// delay to allow LCD to update
	
	put_dpyspi_data(0xFE);		// (Doors Closed)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x06);		// character address 
	put_dpyspi_data(0x1F);		// 000 11111 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
	put_dpyspi_data(0x1F);		// 000 11111 - 0x1F
	put_dpyspi_data(0x00);		// 000 00000 - 0x00
		
	delay(1500);			// delay to allow LCD to update
	
	put_dpyspi_data(0xFE);		// (Button Not pressed)
	put_dpyspi_data(0x54);		// Load Special Character Set command
	put_dpyspi_data(0x07);		// character address 
	put_dpyspi_data(0x1F);		// 
	put_dpyspi_data(0x11);		// 
	put_dpyspi_data(0x11);		// 
	put_dpyspi_data(0x11);		// 
	put_dpyspi_data(0x11);		// 
	put_dpyspi_data(0x11);		// 
	put_dpyspi_data(0x11);		// 
	put_dpyspi_data(0x1F);		// 	
}

/* 
Revision History


*/

