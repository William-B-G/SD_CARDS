#define d_RTCSPI 1
#include "global.h"

/*  Real Time Calender Clock/MRAM - SPI interface

*/

void local_gettime(void);
void local_settime(void);
void local_getdate(void);
void local_setdate(void);
void init_rtc_mram(void);
void local_init_rtc(void);
void put_rtc_data(uint8 addr, uint8 data);
uint8 get_rtc_data (uint8 addr);
void Write_MRAM_Data(uint32 addr, uint16 data, uint8 dev);
uint16 Read_MRAM_Data (uint32 addr, uint8 dev);
void Read_MRAM_Array (uint32 addr, uint16 *data, uint16 len, uint8 dev);
void Write_MRAM_Array(uint32 addr, uint16 *data, uint16 len, uint8 dev);
void Write_MRAM_SR(uint8 status, uint8 dev);
uint8 Read_MRAM_SR (uint8 dev);
void Write_MRAM_Command(uint8 cmd, uint8 dev);

struct galtime {
    int8 hour;         /* Hours */
    int8 minute;       /* Minutes */
    int8 second;       /* Seconds */
    int8 hsecond;      /* Hundredths of seconds */
};

struct galdate {
    int8 day;      /* 1-31 */
    int8 month;    /* 1-12 */
    int16  year;     /* 0000 - 9999 */
    int8 dayofweek;/* 1 - 7 (1=Sunday) */
};

struct galtime t;
struct galdate d, prev_d;

uint8 mram_status;

void init_rtc_mram (void)
{
 	int16 i = 0;
 	int32 dontcare;
	
	_disable();
	
	DSPI_C.MCR.B.HALT = 1;		// Stop SPI from running
	DSPI_C.MCR.B.MSTR = 1;		// Master Mode
	DSPI_C.MCR.B.CONT_SCKE = 0;	// Continuous SCK disabled
	DSPI_C.MCR.B.DCONF = 0;		// SPI mode
	DSPI_C.MCR.B.FRZ = 0;		// Do not freeze SPI during debug
	DSPI_C.MCR.B.MTFE = 0;		// No modified timing format
	DSPI_C.MCR.B.PCSSE = 0;		// No Peripheral Chip Select Strobe 
	DSPI_C.MCR.B.ROOE = 0; 		// Ignore overflow incomming data
	DSPI_C.MCR.B.PCSIS0 = 1;	// Peripheral Chip select active low state
	DSPI_C.MCR.B.PCSIS1 = 1;	// Peripheral Chip select active low state
	DSPI_C.MCR.B.PCSIS2 = 1;	// Peripheral Chip select active low state
	DSPI_C.MCR.B.PCSIS3 = 1;	// Peripheral Chip select active low state
	DSPI_C.MCR.B.PCSIS4 = 1;	// Peripheral Chip select active low state
	DSPI_C.MCR.B.PCSIS5 = 1;	// Peripheral Chip select active low state
	DSPI_C.MCR.B.DIS_TXF = 0;	// Allow Transmit Fifo
	DSPI_C.MCR.B.DIS_RXF = 0;	// Allow Receive Fifo
	DSPI_C.MCR.B.CLR_TXF = 1;	// Clear transmit fifo
	DSPI_C.MCR.B.CLR_RXF = 1;	// Clear receive fifo
	DSPI_C.MCR.B.SMPL_PT = 0;	// Sample point 0 - Not using modified timing format

	
	DSPI_C.TCR.B.SPI_TCNT = 0;	// Reset SPI transfer Count
	
	//***********************************
	// Command Transfer to MRAM CTAR0
	//***********************************

	DSPI_C.CTAR[0].B.FMSZ = 7;		// Transfer 8 bits
	DSPI_C.CTAR[0].B.CPOL = 1;		// Clock polarity is high
	DSPI_C.CTAR[0].B.CPHA = 1;		// Clock phase is leading edge of SCK
	DSPI_C.CTAR[0].B.LSBFE = 0;		// MSB first 
		
	// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 4 = 36.78 nsec (use one clock width)
	DSPI_C.CTAR[0].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
	DSPI_C.CTAR[0].B.CSSCK = 1;		// PCS to SCK  set to  4 (tCSS on MRAM chip = 10 ns) 
	
	// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 4 = 36.78 nsec (use 1 clock width) 
	DSPI_C.CTAR[0].B.PASC = 0;		// Prescaler = 1				
	DSPI_C.CTAR[0].B.ASC = 1;		// After SCK delay set to 4 (tCSH on MRAM chip = 10 ns)
	
	// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 8 = 73.56 nsec (use 2 clock widths)
	DSPI_C.CTAR[0].B.PDT = 0;		// Prescaler = 1
	DSPI_C.CTAR[0].B._DT = 2;		// CS Delay Time set to 8 (tDT = 400 ns) (Could find no timing requirement on MRAM chip)
																				 
	// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 2 = 27.1875 MHz
	DSPI_C.CTAR[0].B.DBR = 0;		// Double baud rate
	DSPI_C.CTAR[0].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
	DSPI_C.CTAR[0].B.BR = 0;		// Baud rate prescaler (divide by 2)


	//***********************************
	// DATA Transfer to MRAM CTAR0
	//***********************************
		
	DSPI_C.CTAR[1].B.FMSZ = 15;		// Transfer 15 bits
	DSPI_C.CTAR[1].B.CPOL = 1;		// Clock polarity is high
	DSPI_C.CTAR[1].B.CPHA = 1;		// Clock phase is leading edge of SCK
	DSPI_C.CTAR[1].B.LSBFE = 0;		// MSB first 
		
	// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 4 = 36.78 nsec (use one clock width)
	DSPI_C.CTAR[1].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
	DSPI_C.CTAR[1].B.CSSCK = 1;		// PCS to SCK  set to  4 (tCSS on MRAM chip = 10 ns) 
	
	// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 4 = 36.78 nsec (use 1 clock width) 
	DSPI_C.CTAR[1].B.PASC = 0;		// Prescaler = 1				
	DSPI_C.CTAR[1].B.ASC = 1;		// After SCK delay set to 4 (tCSH on MRAM chip = 10 ns)
	
	// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 8 = 73.56 nsec (use 2 clock widths)
	DSPI_C.CTAR[1].B.PDT = 0;		// Prescaler = 1
	DSPI_C.CTAR[1].B._DT = 2;		// CS Delay Time set to 8 (tDT = 400 ns) (Could find no timing requirement on MRAM chip)
																				 
	// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 2 = 27.1875 MHz
	DSPI_C.CTAR[1].B.DBR = 0;		// Double baud rate
	DSPI_C.CTAR[1].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
	DSPI_C.CTAR[1].B.BR = 0;		// Baud rate prescaler (divide by 2)

	for (i=2;i<=7;i++)
	{
		DSPI_C.CTAR[i].B.FMSZ = 15;		// Transfer 16 bits
		DSPI_C.CTAR[i].B.CPOL = 1;		// Clock polarity is high
		DSPI_C.CTAR[i].B.CPHA = 1;		// Clock phase is leading edge of SCK
		DSPI_C.CTAR[i].B.LSBFE = 0;		// MSB first 
		
		// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 64 = 588 nsec (use one clock width)
		DSPI_C.CTAR[i].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
		DSPI_C.CTAR[i].B.CSSCK = 5;		// PCS to SCK  set to  1024 (tCC on RTC chip = 400 ns) 
		
		// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 32 = 294 nsec (use 1/2 clock width) 
		DSPI_C.CTAR[i].B.PASC = 0;		// Prescaler = 1				
		DSPI_C.CTAR[i].B.ASC = 4;		// After SCK delay set to 512 (tCCH on RTC chip = 100 ns)
		
		// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 128 = 1.177 usec (use 2 clock widths)
		DSPI_C.CTAR[i].B.PDT = 0;		// Prescaler = 1
		DSPI_C.CTAR[i].B._DT = 6;			// CS Delay Time (tDT = 400 ns) (tCWH on RTC chip)
																				 
		// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 32 = 1.69921875 MHz
		DSPI_C.CTAR[i].B.DBR = 0;		// No double baud rate
		DSPI_C.CTAR[i].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
		DSPI_C.CTAR[i].B.BR = 5;		// Baud rate prescaler (divide by 32)
	}
	
	// Clear the status register bits
	DSPI_C.SR.B.TCF = 1;			// Clear transfer complete flag
	DSPI_C.SR.B.TFUF = 1;			// Clear transmit underflow flag
	DSPI_C.SR.B.TFFF = 1; 			// Clear transmit fifo fill flag
	DSPI_C.SR.B.RFOF = 1;			// Clear receive fifo overflow flag
	DSPI_C.SR.B.RFDF = 1;			// Clear receive fifo drain flag

	
	// Set up DMA request for RTC SPI
	DSPI_C.RSER.B.TCF_RE = 0;		// Disable transmission request interrupt
	DSPI_C.RSER.B.EOQF_RE = 0;		// Disable DSPI finished interrupt
	DSPI_C.RSER.B.TFUF_RE = 0;		// Disable Transmit fifo underflow interrupt
	DSPI_C.RSER.B.TFFF_RE = 0;		// Enable Transit Fifo Fill
	DSPI_C.RSER.B.TFFF_DIRS = 0; 	// Set to generate DMA request for transmit fifo
	DSPI_C.RSER.B.RFOF_RE = 0;		// Disable Receive Fifo Overflow interrupt
	DSPI_C.RSER.B.RFDF_RE = 0;		// Enable Receive FIFO drain request
	DSPI_C.RSER.B.RFDF_DIRS = 0;	// Set to generate DMA request for receive fifo
	
	DSPI_C.MCR.B.HALT = 0;		// Allow SPI to run
	DSPI_C.MCR.B.MDIS = 0;		// No Module disable

	SIU.PCR[RTC_MRAM_CLK].R = 0x60C;
	SIU.PCR[RTC_MRAM_DOUT].R = 0x60C;
	SIU.PCR[RTC_MRAM_DIN].R = 0x500;
	SIU.PCR[RTC_CS].R = 0x60C;		// Chip select
	SIU.PCR[MRAM_CSn].R = 0x60C;		// Chip select
	SIU.PCR[MRAM_CSn_2].R = 0x60C;		// Chip select
	
	SIU.GPDO[MRAM_HOLDn].R = 0x01;
	SIU.GPDO[MRAM_HOLDn_2].R = 0x01;
	SIU.GPDO[MRAM_WPn].R = 0x01;
	SIU.GPDO[MRAM_WPn_2].R = 0x01;
  	
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 

	_enable();

	Write_MRAM_Command(6,0);
	mram_status = Read_MRAM_SR (0);

	local_init_rtc();			// check real time clock	

}

//****************************
// Put RTC Data
//****************************

void put_rtc_data(uint8 addr, uint8 data)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	uint32 dontcare;
	uint16 count = 0;
	
 	// Clear the receive fifo
	
	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;

	uiu.w[hw] = 0x2001;
	uiu.b[mlb] = addr | 0x80;
	uiu.b[lb] = data; 
	DSPI_C.SR.B.TCF = 1;		// clear bit
	DSPI_C.PUSHR.R = uiu.dw; // Send data to rtc
  	while ((DSPI_C.SR.B.TCF != 1) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
}

//****************************
// Get RTC data
//****************************

uint8 get_rtc_data (uint8 addr)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	uint32 RecDataSlave;
	uint16 count = 0;

 	// Clear the receive fifo
	
	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;

	uiu.w[hw] = 0x2001;
	uiu.b[mlb] = addr & 0x7F;
	uiu.b[lb] = 0; 

	DSPI_C.SR.B.TCF = 1;		// clear bit
  	DSPI_C.PUSHR.R = uiu.dw; 	// send data request
  	while ((DSPI_C.SR.B.TCF != 1) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
  	return ((uint8)RecDataSlave);
}


#define MRWRSR 1 		// write_status_reg 1
#define MRRDSR 5 		// read_status_reg 3
#define MRWREN 6 		// write_enable 6
#define MRWRDI 4 		// write_disable 4
#define MRREAD 3 		// read memory 3
#define MRWRITE 2 		// write memory 2
#define MRSLEEP 0xB9	// sleep mode
#define MRWAKE 0xAB		// wake from sleep mode

//****************************
// Write Command to MRAM
//****************************


void Write_MRAM_Command(uint8 cmd, uint8 dev)
{		
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	uint32 dontcare;
	uint16 count = 0;
	
 	// Clear the receive fifo
	
	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;

	if (dev == 0)
		uiu.w[hw] = 0x0002;
	else 
		uiu.w[hw] = 0x0004;
	uiu.b[mlb] = 0;
	uiu.b[lb] = cmd; 		// 8-bit write
	DSPI_C.SR.B.TCF = 1;		// clear bit
	DSPI_C.PUSHR.R = uiu.dw; // Send data to rtc
  	while ((DSPI_C.SR.B.TCF != 1) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
}

//********************************
// Read Status Register from MRAM
//********************************

uint8 Read_MRAM_SR (uint8 dev)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	uint32 RecDataSlave;
	uint16 count = 0;

	// Clear the receive fifo
	
 	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;

	if (dev == 0)
		uiu.w[hw] = 0x1002;
	else 
		uiu.w[hw] = 0x1004;
	uiu.b[mlb] = MRRDSR;		// 8-bit write/8-bit read
	uiu.b[lb] = 0; 

	DSPI_C.SR.B.TCF = 1;		// clear bit
  	DSPI_C.PUSHR.R = uiu.dw; 	// send data request
  	while ((DSPI_C.SR.B.TCF != 1) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
  	return ((uint8)RecDataSlave);
}

//*******************************
// Write Status Register to MRAM
//*******************************

void Write_MRAM_SR(uint8 status, uint8 dev)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	uint32 dontcare;
	uint16 count = 0;
	
	// Clear the receive fifo
	
 	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;

	if (dev == 0)
		uiu.w[hw] = 0x1002;
	else 
		uiu.w[hw] = 0x1004;
	uiu.b[mlb] = MRWRSR;			// 16 bit write
	uiu.b[lb] = status; 
	DSPI_C.SR.B.TCF = 1;		// clear bit
	DSPI_C.PUSHR.R = uiu.dw; // Send data to rtc
  	while ((DSPI_C.SR.B.TCF != 1) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
}



//********************************
// Read Data from MRAM
//********************************

uint16 Read_MRAM_Data (uint32 addr, uint8 dev)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu_2;					// unsigned integer union
	uint32 RecDataSlave;
	uint8 dev_sel;
	uint16 count = 0;

	if (dev == 0)
		dev_sel = 0x02;
	else
		dev_sel = 0x04;
	
	// clear the fifo
 	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;
	
	// write command and middle high byte of address
	uiu_2.dw = addr;
	uiu.b[hb] = 0x90;			// Continuous chip select
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.b[mlb] = MRREAD;		
	uiu.b[lb] = uiu_2.b[mhb];	 // on 24 bits of address used 

  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
  	DSPI_C.PUSHR.R = uiu.dw; 	// send data request

	// write low word of address
	uiu.b[hb] = 0x90;			// Continuous chip select
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.w[lw] = uiu_2.w[lw];		
 	DSPI_C.PUSHR.R = uiu.dw; 	// send data request

	// write FFFF to receive data
	uiu.b[hb] = 0x10;			// Allow chip select to turn off
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.w[lw] = 0xFFFF;		
 	DSPI_C.PUSHR.R = uiu.dw; 	// send data request
 	
 	while ((DSPI_C.SR.B.RXCTR != 3) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
  	
  	return ((uint16)RecDataSlave);
}

//*******************************
// Write Data to MRAM
//*******************************

void Write_MRAM_Data(uint32 addr, uint16 data, uint8 dev)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu_2;					// unsigned integer union
	uint32 dontcare;
	uint8 dev_sel;
	uint16 count = 0;

	if (dev == 0)
		dev_sel = 0x02;
	else
		dev_sel = 0x04;
	
	// Clear the receive fifo
	
 	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;

	// write command and high byte of address
	uiu_2.dw = addr;
	uiu.b[hb] = 0x90;			// Continuous chip select
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.b[mlb] = MRWRITE;		
	uiu.b[lb] = uiu_2.b[mhb];	 // on 24 bits of address used 

  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
	DSPI_C.PUSHR.R = uiu.dw; // Send data to rtc

	// write low word of address
	uiu.b[hb] = 0x90;			// Continuous chip select
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.w[lw] = uiu_2.w[lw];		
  	DSPI_C.PUSHR.R = uiu.dw; 	// send data request

	// write data
	uiu.b[hb] = 0x10;			// Allow chip select to turn off
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.w[lw] = data;		
 	DSPI_C.PUSHR.R = uiu.dw; 	// send data request
	
  	while ((DSPI_C.SR.B.RXCTR != 3) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
 	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
}

//********************************
// Read Array from MRAM
//********************************

void Read_MRAM_Array (uint32 addr, uint16 *data, uint16 len, uint8 dev)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu_2;					// unsigned integer union
	uint32 RecDataSlave;
	uint8 dev_sel;
	uint16 count = 0;
	uint16 i;

	if (dev == 0)
		dev_sel = 0x02;
	else
		dev_sel = 0x04;
	
	// clear the fifo
 	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;
	
	// write command and middle high byte of address
	uiu_2.dw = addr;
	uiu.b[hb] = 0x90;			// Continuous chip select
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.b[mlb] = MRREAD;		
	uiu.b[lb] = uiu_2.b[mhb];	 // on 24 bits of address used 

  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
  	DSPI_C.PUSHR.R = uiu.dw; 	// send data request

	// write low word of address
	uiu.b[hb] = 0x90;			// Continuous chip select
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.w[lw] = uiu_2.w[lw];		
 	DSPI_C.PUSHR.R = uiu.dw; 	// send data request
 	while ((DSPI_C.SR.B.RXCTR != 2) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	RecDataSlave = DSPI_C.POPR.R;    // Read data received by slave SPI 

	for (i=0;i<len;i++)
	{
		// write FFFF to receive data
		if (i == (len-1))
			uiu.b[hb] = 0x10;			// Allow chip select to turn off
		else
			uiu.b[hb] = 0x90;			// Continuous chip select
		
		uiu.b[mhb] = dev_sel;		// select which device to use
		uiu.w[lw] = 0xFFFF;		
	 	DSPI_C.PUSHR.R = uiu.dw; 	// send data request
	 	
	 	while ((DSPI_C.SR.B.RXCTR != 1) && (count < 1000))
	  	{		// Wait for Transmit Complete Flag = 1
	  		count++;
	  	}   
	  	data[i] = (uint16)DSPI_C.POPR.R;    // Read data received by slave SPI 
	}
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
  	
}

//*******************************
// Write Array to MRAM
//*******************************

void Write_MRAM_Array(uint32 addr, uint16 *data, uint16 len, uint8 dev)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu_2;					// unsigned integer union
	uint32 dontcare;
	uint8 dev_sel;
	uint16 count = 0;
	uint16 i;

	if (dev == 0)
		dev_sel = 0x02;
	else
		dev_sel = 0x04;
	
	// Clear the receive fifo
	
 	while ((DSPI_C.SR.B.RXCTR != 0) && (count <= 5))
  	{		// Wait for Transmit Complete Flag = 1
 	  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
 		count++;
  	}   
  	
	count = 0;

	// write command and high byte of address
	uiu_2.dw = addr;
	uiu.b[hb] = 0x90;			// Continuous chip select
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.b[mlb] = MRWRITE;		
	uiu.b[lb] = uiu_2.b[mhb];	 // on 24 bits of address used 

  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
	DSPI_C.PUSHR.R = uiu.dw; // Send data to rtc

	// write low word of address
	uiu.b[hb] = 0x90;			// Continuous chip select
	uiu.b[mhb] = dev_sel;		// select which device to use
	uiu.w[lw] = uiu_2.w[lw];		
  	DSPI_C.PUSHR.R = uiu.dw; 	// send data request
  	while ((DSPI_C.SR.B.RXCTR != 2) && (count < 1000))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
 	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 

	for (i=0;i<len;i++)
	{
			
		// write data
		if (i == (len-1))
			uiu.b[hb] = 0x10;			// Allow chip select to turn off
		else
			uiu.b[hb] = 0x90;			// Continuous chip select
		uiu.b[mhb] = dev_sel;		// select which device to use
		uiu.w[lw] = data[i];		
	 	DSPI_C.PUSHR.R = uiu.dw; 	// send data request
		
	  	while ((DSPI_C.SR.B.RXCTR != 1) && (count < 1000))
	  	{		// Wait for Transmit Complete Flag = 1
	  		count++;
	  	}   
	  	dontcare = DSPI_C.POPR.R;    // Read data received by slave SPI 
	}
  	DSPI_C.SR.R = 0x9A0A0000;        // Clear TCF, EOQF, TFUF, TFFF, RFOF and RFDF flags by writing 1 to them 
}

//****************************
//  Calculate Day of the Week
//****************************

int16 month_c[13] = {0,13,14,3,4,5,6,7,8,9,10,11,12};

unsigned char calc_dayofweek (void)
{
	int16 low_year;
	int16 high_year;
	int16 bcd_year;
	int16 dow_calc;
	int16 year_calc;

	unsigned char dow;

	bcd_year = hex_to_bcd(d.year);
	low_year = bcd_to_hex((uint16)(bcd_year & 0x00FF));
	high_year = bcd_to_hex((uint16)((bcd_year >> 8) & 0x00FF));
	if (month_c[d.month] > 12)
		low_year--;		// months 13 and 14 are considered in the previous year

	year_calc = (2 * high_year) % 7;
	dow_calc = ((unsigned char) ((int)d.day + ((26*(month_c[(int)d.month]+1))/10) + ((5*low_year)/4)  + (high_year/4) - (year_calc)));

	dow = (uint8)(dow_calc % 7);

	if (dow > 6)
		dow = 0;

	return dow;

}


//***************************************
// Initialize Real Time Calendar Clock
//***************************************

void local_init_rtc(void)
{
	uint8 hour24;
	uint8 oscfail;
	uint8 cap_charge;
	
	 hour24 = get_rtc_data(3);
	 oscfail = get_rtc_data(14) ;
	 cap_charge = get_rtc_data(15);
	 if ((cap_charge != 0xAA) || ((oscfail & 0x80) != 0) || ((hour24 & 0x40) != 0))
	 {	// Initialize real time calendar clock

		put_rtc_data(13,0x18);		// make sure oscillator is enabled
		put_rtc_data(14,0);			// clear oscfail bit
		put_rtc_data(15,0xAA);		// set 2k resistor with diode
		put_rtc_data(3,0);			// 24 hour bit gets cleared for 24 hour clock
		
		hour24 = get_rtc_data(3);
	 	oscfail = get_rtc_data(14) ;
		cap_charge = get_rtc_data(15);
	 }
	 
	 if ((cap_charge != 0xAA) || ((oscfail & 0x80) != 0) || ((hour24 & 0x40) != 0))
	 {	// Initialize real time calendar clock

		put_rtc_data(13,0x18);		// make sure oscillator is enabled
		put_rtc_data(14,0);			// clear oscfail bit
		put_rtc_data(15,0xAA);		// set 2k resistor with diode
		put_rtc_data(3,0);			// 24 hour bit gets cleared for 24 hour clock
		
		
		t.hour = 0;
		t.second = 0;
		t.minute = 0;
		d.day = 1;
		d.month = 1;
		d.year = 2000;
		d.dayofweek = calc_dayofweek();
		local_settime();
		local_setdate();
	 }

}					
//***************************************
// Get time from real time calendar clock
//***************************************

void local_gettime(void)
{
	t.second = (uint8)bcd_to_hex((uint16) get_rtc_data(1));
	t.minute = (uint8)bcd_to_hex((uint16) get_rtc_data(2));
	t.hour = (uint8)bcd_to_hex((uint16) get_rtc_data(3) & 0x3F);
}

//***************************************
// Set time in real time calendar clock
//***************************************

void local_settime(void)
{
	put_rtc_data(1,(uint8)hex_to_bcd(t.second));
	put_rtc_data(2,(uint8)hex_to_bcd(t.minute));
	put_rtc_data(3,(uint8)hex_to_bcd((uint16)(t.hour & 0x3F)));
}

//***************************************
// Get date from real time calendar clock
//***************************************

void local_getdate(void)
{
	int8 dayofweek;
	uint8 century_month;
	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
	
	dayofweek = (uint8)bcd_to_hex((uint16) get_rtc_data(4));
	if ((dayofweek < 0) || (dayofweek > 6))
		d.dayofweek = 0;
	else
		d.dayofweek = dayofweek;
	d.day = (uint8)bcd_to_hex((uint16) get_rtc_data(5));
	century_month = get_rtc_data(6);
	d.month = (uint8)bcd_to_hex((uint16) (century_month & 0x7F));
	uwu.b[lbw] = (uint8)get_rtc_data(7);
	uwu.b[hbw] = (uint8)rd_mram(MRAM_RTC_Dat_Page,MRAM_RTC_Dat_Offset);
	d.year = bcd_to_hex(uwu.w);
	if ((century_month & 0x80) != 0)
	{
		d.year += 100;
		local_setdate();
	}
}

//***************************************
// Set date in real time calendar clock
//***************************************

void local_setdate(void)
{
	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union


	d.dayofweek = calc_dayofweek();
	if (d.dayofweek > 6)
		d.dayofweek = 0;
	put_rtc_data(4,(uint8)hex_to_bcd(d.dayofweek));
	put_rtc_data(5,(uint8)hex_to_bcd(d.day));
	put_rtc_data(6,(uint8)(hex_to_bcd(d.month) & 0x1F));  // Century flag gets cleared
	uwu.w = hex_to_bcd(d.year);
	put_rtc_data(7,uwu.b[lbw]);
  	wr_mram(MRAM_RTC_Dat_Page,MRAM_RTC_Dat_Offset,uwu.b[hbw]);
}

