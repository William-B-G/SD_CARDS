#define d_SDSPI 1
#include "global.h"
#include "fat_opts.h"

/*
	Routines to access the Secure Digital memory device.  
	
	The following guidelines are used:
	
		1. All data is sent in 8 bit blocks.  
	
		2. When receiving data, host sends 0xFF and reads response on data in.
	
		3. Commands are sent without the use of DMA and the host waits for the response.
	
		4. Block data uses DMA for data transfer.
		
	The initialization sequence is as follows:
	
		1. Detect Card
		2. Set clock rate to 400kHz
		3. Send at least 74 clock cycles without CS asserted
		4. Delay 16 clock cycles?
		5. Send CMD0 with CS asserted
		6. Send CMD8
		7. Detect Version
		8. Send CMD58 (read OCR)
		9. Send ACMD41 (Argument 0 if ver 1, else HCS = 0 or 1 for SDHC or SDXC support)
		10. Send CMD58 (if ver 2 or higher to read CCS)
		
	
	

*/

#define SECTOR_SIZE 512

#define DSPI_B_Port 0xFFF94000L
#define SDSPIdr DSPI_B_Port + 0x0034
#define SDSPIsr DSPI_B_Port + 0x003B			// 3B instead of 3A since receiving only one byte
#define SDSPIdmaRxIntLoc eDMA_8_Int

#define SDSPIdmaChTx 7
#define SDSPIdmaChRx 8
#define SDSPIdmaTX 0x94
#define SDSPIdmaRX 0x95

#define c_sdspi_dma_buf_len 514
union
{
	uint32 buf[c_sdspi_dma_buf_len+4];
	union 
	{
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}un[c_sdspi_dma_buf_len+4];
}sdspi_tx_dma;


uint8 sdspi_rx_dma_buf[c_sdspi_dma_buf_len];
int32 sdspi_tx_dma_FF = 0x000100FF;

uint8 sdspi_transferred = 0;
uint8 sdspi_read = 0;
uint8 sdspi_write = 0;
uint8 sdspi_req = 0;
uint8 put_sdspi = 0;


void init_sdspi (void);
void init_SDSPI_DMA(void);
void sdspi_dma_rx_int(void); 
int16 detect_sd(void);
int16 det_sd_wr_prot(void);
uint8 sd_initialization(void);
uint16 crc_16 (unsigned char* chr, int16 cnt);
uint8 crc_7(unsigned char * chr, int cnt); 
uint8 send_sd_cmd(uint8 cmd, uint32 arg);
uint8 get_sd_data (uint32 spi_cmd);
void put_sd_data(uint32 spi_cmd, uint8 spi_data);
void copy_read_data_from_dma_buff(uint8 *addr);
uint8 sd_read_block(uint32 block_addr);
void copy_write_data_to_dma_buff(uint8 *addr);
uint8 sd_write_block(uint32 block_addr);
uint8 get_read_block_status(void);
uint8 get_write_block_status(void);
void sdspi_read_start(void);
void sdspi_write_start(void);
int32 media_read(uint32 sector, uint8 *buffer, uint32 sector_count);
int32 media_write(uint32 sector, uint8 *buffer, uint32 sector_count);

uint8 get_csd(void);
uint8 sd_read_status(void);
int32 get_sd_status(void);
void clock_8_delay (void);

// SD Global Variables

uint8 sdhc = 0;		// SD High Capacity = 1
uint8 sdvs = 0;		// SD voltage status 1 = okay
uint32 sd_voltage = 0;
uint8 sd_init = 0;		// secure digital init flag
uint8 csd[18];

#define R1 	1
#define R1b 2
#define R2	3
#define R3 	4
#define R7  5


// Secure Digital Packet Commands

#define CMD0_GO_IDLE_STATE 			0
#define CMD1_SEND_OP_COND			1
#define CMD8_SEND_IF_COND			8
#define CMD9_SEND_CSD				9
#define CMD10_SEND_CID				10
#define CMD12_STOP_TRANSMISSION		12
#define CMD13_SEND_STATUS			13
#define CMD16_SET_BLOCKLEN			16
#define CMD17_READ_SINGLE_BLOCK 	17
#define CMD18_READ_MULTIPLE_BLOCK 	18
#define CMD24_WRITE_SINGLE_BLOCK	24
#define CMD25_WRITE_MULTIPLE_BLOCK	25
#define CMD27_PROGRAM_CSD			27
#define CMD28_SET_WRITE_PROT		28
#define CMD29_CLR_WRITE_PROT		29
#define CMD30_SEND_WRITE_PROT		30
#define CMD32_ERASE_WR_BLK_STR_ADDR 32
#define CMD33_ERASE_WR_BLK_END_ADDR	33
#define CMD38_ERASE					38
#define CMD42_LOCK_UNLOCK			42
#define CMD55_APP_CMD				55
#define CMD56_GEN_CMD				56
#define CMD58_READ_OCR				58
#define CMD59_CRC_ON_OFF			59

// Application Packet Commands
#define ACMD13_SD_STATUS			(13 | 0x80)
#define ACMD22_SEND_NUM_WR_BLOCKS	(22 | 0x80)
#define ACMD23_SET_WR_BLK_ERASE_CNT	(23 | 0x80)
#define ACMD41_SD_SEND_OP_COND		(41 | 0x80)
#define ACMD42_SET_CLR_CARD_DETECT	(42 | 0x80)
#define ACMD51_SEND_SCR				(51 | 0x80)

// Byte times for time-out
#define CMD_RESP_TIMEOUT 	20		// must be at least 8 byte times
#define READ_RESP_TIMEOUT	21241  //SPI byte times for read operations (should be at least 100ms for SD cards)
#define WRITE_RESP_TIMEOUT	53103  //SPI byte times for write operation (should be at least 250ms for SD cards).

#define DATA_RESPONSE_TOKEN 		0x05
#define START_SINGLE_BLOCK_TOKEN	0xFE
#define START_MULTI_BLOCK_TOKEN		0xFC


// spi commands for put_sd_data() and get_sd_data()
#define NO_CS_SLOW_CLK	0x30000000
#define CS2_SLOW_CLK	0x30010000
#define CS2_3_3Mhz_CLK	0x00010000
#define CS2_1_6Mhz_CLK	0x10010000
#define CS2_0_8Mhz_CLK	0x20010000
uint32 spi_clock = NO_CS_SLOW_CLK;
uint8 sd_class = 0;
uint8 sd_pm = 0;


// R1 Bits
#define START_BIT_0			0x80
#define PAR_ERR				0x40		
#define ADDR_ERR			0x20
#define ERASE_SEQ_ERR		0x10
#define CRC_ERROR			0x08
#define ILLEGAL_CMD			0x04
#define ERASE_RESET			0x02
#define IN_IDLE_STATE		0x01

// R2 Bits
#define OUT_OF_RANGE		0x80
#define ERASE_PAR			0x40
#define WR_PROT_VIOLATION 	0x20
#define CARD_ECC_FAIL		0x10
#define CARD_CNTRL_ERR		0x08
#define UNSPECIFIED_ERR		0x04
#define WR_PROT_ERASE_SKIP 	0x02
#define CARD_LOCKED			0x01

// SD Response Structure 

struct {
	
    uint8 r1;
    uint8 r2;
    union 
    {
    	uint8 b[4];
    	uint32 dw;
    }r3;
    union 
    {
    	uint8 b[4];
    	uint32 dw;
    }r7;
    uint8 time_out;			// time-out flag
} Resp;                  // R1, R2, R3 and R7 response


							//  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
uint8 sd_cmd_resp_tbl[64] =  { R1, R1,  0,  0,  0,  0, R1,  0, R7, R1,	// 0-9
							   R1,  0,R1b, R2,  0,  0, R1, R1, R1,  0,	// 10-19
							    0,  0,  0,  0, R1, R1,  0, R1,R1b,R1b,	// 20-29
							   R1,  0, R1, R1,  0,  0,  0,  0,R1b,  0,	// 30-39
							    0,  0, R1,  0,  0,  0,  0,  0,  0,  0,  // 40-49
							    0,  0,  0,  0,  0, R1, R1,  0, R3, R1,	// 50-59
							    0,  0,  0,  0};							// 60-63
							    
							//  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
uint8 sd_acmd_resp_tbl[64] =  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	// 0-9
							    0,  0,  0, R2,  0,  0,  0,  0,  0,  0,	// 10-19
							    0,  0, R1, R1,  0,  0,  0,  0,  0,  0,	// 20-29
							    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	// 30-39
							    0, R1, R1,  0,  0,  0,  0,  0,  0,  0,	// 40-49
							    0, R1,  0,  0,  0,  0,  0,  0,  0,  0,	// 50-59
							    0,  0,  0,  0};							// 60-63
							     

// SD Errors

#define No_SD_Error 		0
#define SD_Not_Detected 	1
#define SD_Write_Protect 	2
#define Idle_State_Fail		3
#define CMD0_Time_Out 		4		// Idle command time-out
#define CMD16_Time_out 		5		// Set block length command time-out
#define CMD17_Time_out 		6		// read coammand time-out
#define Read_Cmd_Fail 		7	
#define Read_resp_fail		8
#define CMD24_Time_out		9		// write command time-out
#define Write_Cmd_Fail		10
#define Invalid_Voltage 	11
#define ACMD41_Time_out 	12		// Capacity Support command time-out
#define ACMD41_Response_Error 13	// Capacity Support command response failure
#define CMD55_Time_Out 		14		// Application Cmd time-out
#define CMD58_Time_Out 		15		// OCR Request time-out
#define CMD59_Time_Out 		16		// CRC off Command time-out
#define ACMD13_Time_Out		17
#define Read_Status_Error 	18

	
	
// SD Command Structure
	
struct 
{
	uint8 cmd;			// command number
	union 
	{		
		uint8 b[4];
		uint32 dw;
	}arg;				// argument
	uint8 crc;			// crc check sum
	uint8 resp;			// responce expected
	uint8 ex;
}sd_pkt;					// sd packet structure



void init_sdspi (void)
{
	int16 i;
	
	_disable();
	
	DSPI_B.MCR.B.HALT = 1;		// Stop SPI from running
	DSPI_B.MCR.B.MSTR = 1;		// Master Mode
	DSPI_B.MCR.B.CONT_SCKE = 0;	// Continuous SCK disabled
	DSPI_B.MCR.B.DCONF = 0;		// SPI mode
	DSPI_B.MCR.B.FRZ = 0;		// Do not freeze SPI during debug
	DSPI_B.MCR.B.MTFE = 0;		// No modified timing format
	DSPI_B.MCR.B.PCSSE = 0;		// No Peripheral Chip Select Strobe 
	DSPI_B.MCR.B.ROOE = 0; 		// Ignore overflow incomming data
	DSPI_B.MCR.B.PCSIS0 = 1;	// Peripheral Chip select active low state
	DSPI_B.MCR.B.PCSIS1 = 1;	// Peripheral Chip select active low state
	DSPI_B.MCR.B.PCSIS2 = 1;	// Peripheral Chip select active low state
	DSPI_B.MCR.B.PCSIS3 = 1;	// Peripheral Chip select active low state
	DSPI_B.MCR.B.PCSIS4 = 1;	// Peripheral Chip select active low state
	DSPI_B.MCR.B.PCSIS5 = 1;	// Peripheral Chip select active low state
	DSPI_B.MCR.B.DIS_TXF = 0;	// Allow Transmit Fifo
	DSPI_B.MCR.B.DIS_RXF = 0;	// Allow Receive Fifo
	DSPI_B.MCR.B.CLR_TXF = 1;	// Clear transmit fifo
	DSPI_B.MCR.B.CLR_RXF = 1;	// Clear receive fifo
	DSPI_B.MCR.B.SMPL_PT = 0;	// Sample point 0 - Not using modified timing format
	
	DSPI_B.TCR.B.SPI_TCNT = 0;	// Reset SPI transfer Count
	
	//**************************************
	// Set 3.3Mhz clock for indes 0
	//**************************************
	DSPI_B.CTAR[0].B.FMSZ = 7;		// Transfer 8 bits
	DSPI_B.CTAR[0].B.CPOL = 0;		// Clock polarity is low
	DSPI_B.CTAR[0].B.CPHA = 0;		// Clock phase is leading edge of SCK
	DSPI_B.CTAR[0].B.LSBFE = 0;		// MSB first 
		
	// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 32 = 294 nsec (use one clock width)
	DSPI_B.CTAR[0].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
	DSPI_B.CTAR[0].B.CSSCK = 4;		// PCS to SCK  set to  1024 (tCC on RTC chip = 400 ns) 32
		
	// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 16 = 247 nsec (use 1/2 clock width) 
	DSPI_B.CTAR[0].B.PASC = 0;		// Prescaler = 1				
	DSPI_B.CTAR[0].B.ASC = 3;		// After SCK delay set to 512 (tCCH on RTC chip = 100 ns) 16
		
	// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 64 = 588 nsec (use 2 clock widths)
	DSPI_B.CTAR[0].B.PDT = 0;		// Prescaler = 1
	DSPI_B.CTAR[0].B._DT = 5;		// CS Delay Time (tDT = 400 ns) (tCWH on RTC chip) 64
																				 
	// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 16 = 3.3984375 MHz
	DSPI_B.CTAR[0].B.DBR = 0;		// No double baud rate
	DSPI_B.CTAR[0].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
	DSPI_B.CTAR[0].B.BR = 4;		// Baud rate prescaler (divide by 16)
	
	// Note that 53103 byte transfers = 250 msec
	//           21241 byte transfers = 100 msec
	
	//**************************************
	// Set 1.6 Mhz clock for indes 1
	//**************************************
	DSPI_B.CTAR[1].B.FMSZ = 7;		// Transfer 8 bits
	DSPI_B.CTAR[1].B.CPOL = 0;		// Clock polarity is low
	DSPI_B.CTAR[1].B.CPHA = 0;		// Clock phase is leading edge of SCK
	DSPI_B.CTAR[1].B.LSBFE = 0;		// MSB first 
		
	// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 64 = 588 nsec (use one clock width)
	DSPI_B.CTAR[1].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
	DSPI_B.CTAR[1].B.CSSCK = 5;		// PCS to SCK  set to  1024 (tCC on RTC chip = 400 ns) 
		
	// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 32 = 294 nsec (use 1/2 clock width) 
	DSPI_B.CTAR[1].B.PASC = 0;		// Prescaler = 1				
	DSPI_B.CTAR[1].B.ASC = 4;		// After SCK delay set to 512 (tCCH on RTC chip = 100 ns)
		
	// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 128 = 1.177 usec (use 2 clock widths)
	DSPI_B.CTAR[1].B.PDT = 0;		// Prescaler = 1
	DSPI_B.CTAR[1].B._DT = 6;		// CS Delay Time (tDT = 400 ns) (tCWH on RTC chip)
																				 
	// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 32 = 1.69921875 MHz
	DSPI_B.CTAR[1].B.DBR = 0;		// No double baud rate
	DSPI_B.CTAR[1].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
	DSPI_B.CTAR[1].B.BR = 5;		// Baud rate prescaler (divide by 32)
		

	//**************************************
	// Set 849.609 Khz clock for indes 2
	//**************************************
	DSPI_B.CTAR[2].B.FMSZ = 7;		// Transfer 8 bits
	DSPI_B.CTAR[2].B.CPOL = 0;		// Clock polarity is low
	DSPI_B.CTAR[2].B.CPHA = 0;		// Clock phase is leading edge of SCK
	DSPI_B.CTAR[2].B.LSBFE = 0;		// MSB first 
		
	// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 128 = 1.176 usec (use one clock width)
	DSPI_B.CTAR[2].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
	DSPI_B.CTAR[2].B.CSSCK = 6;		// PCS to SCK  set to  1024 (tCC on RTC chip = 400 ns) 128
		
	// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 64 = 588 nsec (use 1/2 clock width) 
	DSPI_B.CTAR[2].B.PASC = 0;		// Prescaler = 1				
	DSPI_B.CTAR[2].B.ASC = 5;		// After SCK delay set to 512 (tCCH on RTC chip = 100 ns) 64
		
	// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 257 = 3.54 usec (use 2 clock widths)
	DSPI_B.CTAR[2].B.PDT = 0;		// Prescaler = 1
	DSPI_B.CTAR[2].B._DT = 7;		// CS Delay Time (tDT = 400 ns) (tCWH on RTC chip) 256
																				 
	// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 64 = 0.849609375 MHz
	DSPI_B.CTAR[2].B.DBR = 0;		// No double baud rate
	DSPI_B.CTAR[2].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
	DSPI_B.CTAR[2].B.BR = 6;		// Baud rate prescaler (divide by 64)

	//**************************************
	// Set 356.557 Khz clock for indes 3
	//**************************************
	
	DSPI_B.CTAR[3].B.FMSZ = 7;		// Transfer 8 bits
	DSPI_B.CTAR[3].B.CPOL = 0;		// Clock polarity is low
	DSPI_B.CTAR[3].B.CPHA = 0;		// Clock phase is leading edge of SCK
	DSPI_B.CTAR[3].B.LSBFE = 0;		// MSB first 
			
	// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 5 x 64 = 2.8 usec (use one clock width)
	DSPI_B.CTAR[3].B.PCSSCK = 2;	// PCS to SCK Delay Prescaler = 5
	DSPI_B.CTAR[3].B.CSSCK = 5;		// PCS to SCK  set to  64  
			
	// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 5 x 32 = 1.4 usec (use 1/2 clock width) 
	DSPI_B.CTAR[3].B.PASC = 2;		// Prescaler = 5				
	DSPI_B.CTAR[3].B.ASC = 4;		// After SCK delay set to 32 
			
	// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 5 x 128 = 5.6 usec (use 2 clock widths)
	DSPI_B.CTAR[3].B.PDT = 2;		// Prescaler = 5
	DSPI_B.CTAR[3].B._DT = 6;		// CS Delay Time  
																					 
	// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 5 / 64 = 356557.3770 KHz
	DSPI_B.CTAR[3].B.DBR = 0;		// No double baud rate
	DSPI_B.CTAR[3].B.PBR = 2;		// Baud rate clock prescale (divide by 5)
	DSPI_B.CTAR[3].B.BR = 6;		// Baud rate prescaler (divide by 64)
	
	// Note that 11143 byte transfers = 250 msec
	//			  4457 byte transfers = 100 msec
	
	//**************************************



	// Clear the status register bits
	DSPI_B.SR.B.TCF = 1;			// Clear transfer complete flag
	DSPI_B.SR.B.TFUF = 1;			// Clear transmit underflow flag
	DSPI_B.SR.B.TFFF = 1; 			// Clear transmit fifo fill flag
	DSPI_B.SR.B.RFOF = 1;			// Clear receive fifo overflow flag
	DSPI_B.SR.B.RFDF = 1;			// Clear receive fifo drain flag

	
	// Set up DMA request for SD SPI
	DSPI_B.RSER.B.TCF_RE = 0;		// Disable transmission request interrupt
	DSPI_B.RSER.B.EOQF_RE = 0;		// Disable DSPI finished interrupt
	DSPI_B.RSER.B.TFUF_RE = 0;		// Disable Transmit fifo underflow interrupt
	
	DSPI_B.RSER.B.RFOF_RE = 0;		// Disable Receive Fifo Overflow interrupt
	DSPI_B.RSER.B.RFDF_RE = 0;		// Enable Receive FIFO drain request

	// Leave disabled for now
	DSPI_B.RSER.B.TFFF_DIRS = 0; 	// Set to generate DMA request for transmit fifo
	DSPI_B.RSER.B.RFDF_DIRS = 0;	// Set to generate DMA request for receive fifo
	DSPI_B.RSER.B.TFFF_RE = 0;		// Enable Transmit Fifo Fill
	DSPI_B.RSER.B.RFDF_RE = 0;		// Enable Receive FIFO drain request
	
	DSPI_B.MCR.B.HALT = 0;		// Allow SPI to run
	DSPI_B.MCR.B.MDIS = 0;		// No Module disable

	// Secured Digital Port
	SIU.PCR[SD_CLK].R = 0x600;		// 0x0A00		
	SIU.PCR[SD_SO].R = 0x600;		// 0x0A00
	SIU.PCR[SD_SI].R = 0x500;		// 0x0900
	SIU.PCR[SD_CS].R = 0x600;		// Chip select 0x0A00
	SIU.PCR[DETECT_SW].R = 0x100;
	SIU.PCR[PROTECT_SW].R = 0x100;

	init_SDSPI_DMA();			// init for block data transfers
	
	// preload the command word
    for(i=0; i < c_sdspi_dma_buf_len-1; i++)
    {
    	sdspi_tx_dma.buf[i] = 0x90010000;		// CTAS = 0 for FAST clock (commands will write slow clock mode during init CTAS = 1)
    }
   	sdspi_tx_dma.buf[i] = 0x10010000;			//	Turn off cs on last byte 

	_enable();
}


//*****************************************
// DMA Initialization for Display SPI Port
//*****************************************


void init_SDSPI_DMA(void) 
{

	// Initialize Transmit Comm DMA
  
  	EDMA.CERQR.R = SDSPIdmaChTx;      	// Disable Display SPI transmit EDMA channel   


  	EDMA.CPR[SDSPIdmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[SDSPIdmaChTx].R = SDSPIdmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[SDSPIdmaChTx].SADDR = (uint32) &sdspi_tx_dma.buf[0];  	// Load address of source data 
  	EDMA.TCD[SDSPIdmaChTx].SSIZE = 2;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[SDSPIdmaChTx].SOFF = 4;                         		// After transfer, add 2 to src addr
  	
  	EDMA.TCD[SDSPIdmaChTx].SLAST = - (c_sdspi_dma_buf_len * 4);                      		// After major loop, reset src addr 
  	EDMA.TCD[SDSPIdmaChTx].SMOD = 0;                         		// Source modulo feature not used 

  	EDMA.TCD[SDSPIdmaChTx].DADDR =  SDSPIdr; 						// Load address of destination register
  	EDMA.TCD[SDSPIdmaChTx].DSIZE = 2;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[SDSPIdmaChTx].DOFF = 0;                         		// Do not increment destination addr 
  	EDMA.TCD[SDSPIdmaChTx].DLAST_SGA = 0;                    		// After major loop, no dest addr change 
  	EDMA.TCD[SDSPIdmaChTx].DMOD = 0;                         		// Destination modulo feature not used 
  
  	EDMA.TCD[SDSPIdmaChTx].NBYTES= 4;                    			// Transfer 1 byte per minor loop 
  	EDMA.TCD[SDSPIdmaChTx].BITER = c_sdspi_dma_buf_len;                       		// 1 major loop iterations 
  	EDMA.TCD[SDSPIdmaChTx].CITER = c_sdspi_dma_buf_len;                       		// Initialize current iteraction count 
  	EDMA.TCD[SDSPIdmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[SDSPIdmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[SDSPIdmaChTx].INT_MAJ = 0;
  	EDMA.TCD[SDSPIdmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[SDSPIdmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[SDSPIdmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[SDSPIdmaChTx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[SDSPIdmaChTx].E_SG = 0; 
  	EDMA.TCD[SDSPIdmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[SDSPIdmaChTx].DONE = 0;
  	EDMA.TCD[SDSPIdmaChTx].ACTIVE = 0;
  	EDMA.TCD[SDSPIdmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = SDSPIdmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = SDSPIdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = SDSPIdmaChTx;           	// DMA Clear error Register 
  
  	
  	// Initialize Receive Comm DMA

 	EDMA.CERQR.R = SDSPIdmaChRx;      		// Disable group EDMA channel   
  
	INTC_InstallINTCInterruptHandler(sdspi_dma_rx_int, SDSPIdmaRxIntLoc, 1);  

  	EDMA.CPR[SDSPIdmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[SDSPIdmaChRx].R = SDSPIdmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[SDSPIdmaChRx].SADDR = SDSPIsr;  						// Load address of source register data 
  	EDMA.TCD[SDSPIdmaChRx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[SDSPIdmaChRx].SOFF = 0;                         		// Do not increment source address
  	
  	EDMA.TCD[SDSPIdmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
  	EDMA.TCD[SDSPIdmaChRx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[SDSPIdmaChRx].DADDR =  (uint32) &sdspi_rx_dma_buf[0]; 						// Load address of destination 
  	EDMA.TCD[SDSPIdmaChRx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[SDSPIdmaChRx].DOFF = 1;                         		// Increment destination addr by 2 
  	EDMA.TCD[SDSPIdmaChRx].DLAST_SGA = - (c_sdspi_dma_buf_len);                    		// After major loop reset destination address
  	EDMA.TCD[SDSPIdmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
  
  	EDMA.TCD[SDSPIdmaChRx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[SDSPIdmaChRx].BITER = c_sdspi_dma_buf_len;            // Major loop iterations 
  	EDMA.TCD[SDSPIdmaChRx].CITER = c_sdspi_dma_buf_len;                       	// Initialize current iteraction count 
  	EDMA.TCD[SDSPIdmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
  	EDMA.TCD[SDSPIdmaChRx].INT_HALF = 0;                     		
  	EDMA.TCD[SDSPIdmaChRx].INT_MAJ = 1;							// Interrupt after major loop
  	EDMA.TCD[SDSPIdmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[SDSPIdmaChRx].BITERE_LINK = 0;
  	EDMA.TCD[SDSPIdmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[SDSPIdmaChRx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[SDSPIdmaChRx].E_SG = 0; 
  	EDMA.TCD[SDSPIdmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[SDSPIdmaChRx].DONE = 0;
  	EDMA.TCD[SDSPIdmaChRx].ACTIVE = 0;
  	EDMA.TCD[SDSPIdmaChRx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = SDSPIdmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = SDSPIdmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = SDSPIdmaChRx;           	// DMA Clear error Register 
  
}

//*****************************************
// SD SPI DMA Receive Finnished Interrupt
//*****************************************

void sdspi_dma_rx_int(void)
{
	
  	EDMA.CIRQR.R = SDSPIdmaChRx;      	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = SDSPIdmaChRx;      	// Disable Display SPI EDMA channel   
	DSPI_B.RSER.B.TFFF_RE = 0;			// Enable Transit Fifo Fill
	DSPI_B.RSER.B.RFDF_RE = 0;			// Enable Receive FIFO drain request
	DSPI_B.RSER.B.TFFF_DIRS = 0; 	// Set to generate DMA request for transmit fifo
	DSPI_B.RSER.B.RFDF_DIRS = 0;	// Set to generate DMA request for receive fifo
	
	sdspi_transferred = 1;

}

//*****************************************
// Detect SD card present
//*****************************************

int16 detect_sd(void)
{
	
  	if(SIU.GPDI[DETECT_SW].B.PDI == 1)
 		return(0);
   	else
  		return(1);		// detected when input low
}

//*****************************************
// Detect SD card Write Protect
//*****************************************

int16 det_sd_wr_prot(void)
{
	
  	if(SIU.GPDI[PROTECT_SW].B.PDI == 1)
 		return(1);	// protected when input high
   	else
  		return(0);		
}

//*****************************************
// SD card Initialization
//*****************************************


uint8 sd_initialization(void)
{
	int16 i;
	int32 time_out;

	sdhc = 0;	// Normal Capacity Card
	sdvs = 0;
	sd_init = 0;
	
	if (detect_sd() == 0)
		return SD_Not_Detected;
	
	if (det_sd_wr_prot() == 1)
		return SD_Write_Protect;

	
	// SPI port already initialized (slow mode is set by using CTAR[1] values
	
	spi_clock = NO_CS_SLOW_CLK;		// slow clock with no chip select
	
	// must do at least 74 clock cycles so do 80 clocks
	for (i=0;i<=10;i++)
		put_sd_data(spi_clock,0xFF);

	spi_clock = CS2_SLOW_CLK;		// use the chip select 
	
	// Put sd card in idle state
	
	time_out = 0;
	do
	{
		spi_clock = NO_CS_SLOW_CLK;		// slow clock with no chip select
		put_sd_data(spi_clock,0xFF);	// send clock pulses incase card had not finished previous command
		
		spi_clock = CS2_SLOW_CLK;		// use the chip select 
			
		send_sd_cmd(CMD0_GO_IDLE_STATE, 0x00);			// send command 0 to go idle
		time_out++;
		
	}while ((Resp.r1 != IN_IDLE_STATE) && (time_out < 100));
	
	if (time_out >= 100)		// card timed out
	{
		// something failed so try again
		spi_clock = NO_CS_SLOW_CLK;		// slow clock with no chip select
		put_sd_data(spi_clock,0xFF);

		spi_clock = CS2_SLOW_CLK;		// use the chip select 
		
		send_sd_cmd(CMD12_STOP_TRANSMISSION,0x00);		// send command 12 to stop transmission

		if (send_sd_cmd(CMD0_GO_IDLE_STATE, 0x00) == 0)		// send command 0 to go idle
			return CMD0_Time_Out;					// command 0 timed out
		
		if (Resp.r1 != IN_IDLE_STATE)
			return Idle_State_Fail;					// still not in idle state
			
	}
	
	spi_clock = CS2_SLOW_CLK;		// use the chip select 

	// See if car supports command 8

	send_sd_cmd(CMD8_SEND_IF_COND,0x01AA);	// Send Interface Condition Command
	
	if (((Resp.r7.dw & 0xFFF) == 0x1AA) && (Resp.time_out == 0))
	{		// could be a high capacity card
		// Card supports command 8
		
		// See if voltage is correct
		if (send_sd_cmd(CMD58_READ_OCR, 0x00) == 0)				// send request for OCR
			return CMD58_Time_Out;
		
		sd_voltage = Resp.r3.dw;
		
		if ((Resp.r3.dw & 0x00780000) == 0)				// invalid voltage
			return Invalid_Voltage;
		else
			sdvs = 1;
		
		// Send Capacity information in cmd 41 and wait for chip to initialize

		time_out = 0;
		do
		{
			
			if(send_sd_cmd(CMD55_APP_CMD,0x00) == 0)		// send command 55 to signal next command is an application command
				return CMD55_Time_Out;			// bad response from 
			
			if (send_sd_cmd(ACMD41_SD_SEND_OP_COND,0x40000000) == 0)		// send Application command 41 to send host capacity support
				return ACMD41_Time_out;
			
			time_out++;
			
		} while ((Resp.r1 != 0) && (time_out < 1000));	// exit when card is not in idle state and has no errors

		if (time_out >= 1000)
			return ACMD41_Response_Error;
		
		if (send_sd_cmd(CMD58_READ_OCR, 0x00) == 0)				// send request for OCR
			return CMD58_Time_Out;
		
		if ((Resp.r3.dw & 0x40000000) != 0)
			sdhc = 1;	// High Capacity Card
		else
			sdhc = 0;	// Normal Capacity Card
			
	}
	else
	{		// card does not support CMD8 and not high capacity
		sdhc = 0;	// Normal Capacity Card

		// See if voltage is correct
		if (send_sd_cmd(CMD58_READ_OCR, 0x00) == 0)				// send request for OCR
			return CMD58_Time_Out;
		
		if ((Resp.r3.dw & 0x00780000) == 0)				// invalid voltage
			return Invalid_Voltage;
		else
			sdvs = 1;
		
		// Send Capacity information in cmd 41 and wait for chip to initialize

		time_out = 0;
		do
		{
			
			if(send_sd_cmd(CMD55_APP_CMD,0x00) == 0)		// send command 55 to signal next command is an application command
				return CMD55_Time_Out;			// bad response from 
			
			if (send_sd_cmd(ACMD41_SD_SEND_OP_COND,0x00) == 0)		// send Application command 41 to send host capacity support
				return ACMD41_Time_out;
			
			time_out++;
			
		} while ((Resp.r1 != 0) && (time_out < 1000));	// exit when card is not in idle state and has no errors

		if (time_out >= 1000)
			return ACMD41_Response_Error;
		
	}
	
//	spi_clock = CS2_SLOW_CLK;
	spi_clock = CS2_1_6Mhz_CLK;

	// Turn CRC off
	if(send_sd_cmd(CMD59_CRC_ON_OFF, 0x00) == 0)
		return CMD59_Time_Out;
	
	// Set block size to 512
	if(send_sd_cmd(CMD16_SET_BLOCKLEN, SECTOR_SIZE) == 0)
		return CMD16_Time_out;
	
/*	// Card has been initialized 
	if (get_csd() != 0)
	{
		debug1 = 50;
		return CMD0_Time_Out;
	}
	
	debug2 = csd[0];
	debug2 = csd[1];
	debug4 = csd[2];
	
	if (get_sd_status() == 0)
		return Read_Status_Error;
	
	if (sd_class == 0x04)		// class 10
		spi_clock = CS2_3_3Mhz_CLK;
	else if (sd_pm > 0x03)		// 3 Mhz
		spi_clock = CS2_3_3Mhz_CLK;
	else if (sd_pm > 0x02)		// 3 Mhz
		spi_clock = CS2_1_6Mhz_CLK;		// use the chip select with fast clock 
	else if (sd_pm > 0x01)		// 3 Mhz
		spi_clock = CS2_0_8Mhz_CLK;
	else
		spi_clock = CS2_SLOW_CLK;
*/	
	sd_init = 1;
	return 0;
}

//*****************************************
// Transfer One Byte to the SD Card
//*****************************************


void put_sd_data(uint32 spi_cmd, uint8 spi_data)
{
	uint32 dontcare;
	uint16 count = 0;

	DSPI_B.PUSHR.R = spi_cmd | spi_data; // Send data with fast clock to sd

  	while ((DSPI_B.SR.B.RFDF != 1) && (count < 1000))
  	{		// Wait for Receive FIFO Drain Flag = 1
  		count++;
  	}   
  	dontcare = DSPI_B.POPR.R;    // Read data received by slave SPI 
  	DSPI_B.SR.R = 0x80020000;        // Clear TCF, RDRF flags by writing 1 to them 
}


//*******************************************
// Receive One Byte from the SD Card
//*******************************************


uint8 get_sd_data (uint32 spi_cmd)
{
	uint32 RecDataSlave;
	uint16 count = 0;

	DSPI_B.PUSHR.R = spi_cmd | 0x000000FF; // Send dummy data 

  	while ((DSPI_B.SR.B.RFDF != 1) && (count < 1000))
  	{		// Wait for Receive FIFO Drain Flag = 1
  		count++;
  	}   
  	RecDataSlave = DSPI_B.POPR.R;    // Read data received by slave SPI 
  	DSPI_B.SR.R = 0x80020000;        // Clear TCF, RDRF flags by writing 1 to them 
  	return ((uint8)RecDataSlave);
}


//*******************************************
// Send SD Command and get response
//   return 1 for success and 0 for time-out
//*******************************************

uint8 send_sd_cmd(uint8 cmd, uint32 arg)
{
	int32 i;
	uint8 temp1, temp2;
	uint8 crc[5];
	
	sd_pkt.cmd = (cmd & 0x3f) | 0x40;
	crc[0] = sd_pkt.cmd;
	sd_pkt.arg.dw = arg;
	crc[1] = sd_pkt.arg.b[hb];
	crc[2] = sd_pkt.arg.b[mhb];
	crc[3] = sd_pkt.arg.b[mlb];
	crc[4] = sd_pkt.arg.b[lb];
	
	if ((cmd & 0x80) == 0)
		sd_pkt.resp = sd_cmd_resp_tbl[cmd & 0x3f];
	else
		sd_pkt.resp = sd_acmd_resp_tbl[cmd & 0x3f];
	
	temp1 = sd_pkt.resp;
	
	sd_pkt.crc = crc_7(crc,5);		// Should return 0x95 for CMD 0
	Resp.time_out = 0;							// clear time-out flaga

	temp2 = sd_pkt.crc;
	
	spi_clock |= 0x80000000;
	
	put_sd_data(spi_clock,sd_pkt.cmd);	
	put_sd_data(spi_clock,sd_pkt.arg.b[hb]);	
	put_sd_data(spi_clock,sd_pkt.arg.b[mhb]);	
	put_sd_data(spi_clock,sd_pkt.arg.b[mlb]);	
	put_sd_data(spi_clock,sd_pkt.arg.b[lb]);
	put_sd_data(spi_clock,sd_pkt.crc);
	
	spi_clock &= 0x7FFFFFFF;
	
	// Wait for a response
	i = 0;
	do
	{
		Resp.r1 = get_sd_data(spi_clock);
		i++;
	}while ((Resp.r1 == 0xFF) && (i < CMD_RESP_TIMEOUT));
	
	// Check for timeout failure	
	if (i >= CMD_RESP_TIMEOUT)
	{
		Resp.time_out = 1;
		return 0;
	}
	
	if (Resp.time_out == 0)
	{
		
		if (sd_pkt.resp == R2)
		{
			Resp.r2 = get_sd_data(spi_clock);
		}
		else if (sd_pkt.resp == R3)
		{
			Resp.r3.b[hb] = get_sd_data(spi_clock);
			Resp.r3.b[mhb] = get_sd_data(spi_clock);
			Resp.r3.b[mlb] = get_sd_data(spi_clock);
			Resp.r3.b[lb] = get_sd_data(spi_clock);
		}
		else if (sd_pkt.resp == R7) 
		{
			Resp.r7.b[hb] = get_sd_data(spi_clock);
			Resp.r7.b[mhb] = get_sd_data(spi_clock);
			Resp.r7.b[mlb] = get_sd_data(spi_clock);
			Resp.r7.b[lb] = get_sd_data(spi_clock);
		}
		else if (sd_pkt.resp == R1b)
		{
			i = 0;
			do
			{
				Resp.r1 = get_sd_data(spi_clock);
				i++;
			}while ((Resp.r1 == 0x00) && (i < WRITE_RESP_TIMEOUT));
			// Check for timeout failure	
			if (i >= WRITE_RESP_TIMEOUT)
			{
				Resp.time_out = 1;
				return 0;
			}
		}
	}
		
	spi_clock &= 0x7FFFFFFF;
	put_sd_data(spi_clock,0xFF);		// Delay 8 clock cycles
	return 1;
}



//***************************
// Eight clock pulse delay
//***************************

void clock_8_delay (void)
{
	spi_clock &= 0x7FFFFFFF;
	put_sd_data(spi_clock,0xFF);		// Delay 8 clock cycles
}

//*****************************************
// Calculate 7 bit CRC for command
//*****************************************

uint8 crc_7(unsigned char * chr, int cnt) 
{
	int16 i,a;
	uint8 crc,Data;

	crc=0;
	for (a=0;a<cnt;a++) 
	{
		Data=chr[a];
		for (i=0;i<8;i++) 
		{
			crc <<= 1;

			if (((Data ^ crc) & 0x80) != 0)
				crc ^=0x09;
			Data <<= 1; 
		}
	}
	crc=(crc<<1)|1;
	return(crc);
} 

//***************************************************************
// Params:
// 	 Sector: 32-bit sector number
// 	 Buffer: Target buffer to read n sectors of data into.
// 	 Sector_count: Number of sectors to read.
//
// Return: 
// 	 int, 1 = success, 0 = failure.
// 
// Description:
//    Application/target specific disk/media read function.
//    Sector number (sectors are usually 512 byte pages) to read.
//***************************************************************

int32 media_read(uint32 sector, uint8 *buffer, uint32 sector_count)
{
	int32 i;
	int32 time_out;
	uint8 status;
	uint16 j;
	
	
	j = 0;
	for (i=sector;i<(sector+sector_count);i++)
	{
		if (sd_read_block(i) != 0)
			return 0;
		
		status = 0;
		time_out = 0;
		do
		{
			status = get_read_block_status();
			time_out++;
		}while ((status == 0) && (time_out < 0x000FFFFF));
		clock_8_delay();
		if (status == 0xff)
			return 0;
		
		if (time_out > 0x000FFFFF)
			return 0;

		if (j >= FAT_BUFFERS)
			return 0;
		
		copy_read_data_from_dma_buff(&buffer[j*SECTOR_SIZE]);
		j++;
	}
	
	return 1;
}

//*****************************************
// Copy data from DMA buffer to User Ram
//*****************************************

void copy_read_data_from_dma_buff(uint8 *addr)
{
	uint16 i;
	
	
	for (i=0;i<SECTOR_SIZE;i++)
		addr[i] = sdspi_rx_dma_buf[i];
}


//*********************************************
// Read Block of Data from SD Card (512 bytes)
//*********************************************

uint8 sd_read_block(uint32 block_addr)
{
	int32 i;
	uint8 token;
	
	if (sdhc == 0)				// high capacity expects the block number wheras normal capacity expects a physical address
		block_addr <<= 9;
	
	if (send_sd_cmd(CMD17_READ_SINGLE_BLOCK, block_addr) == 0)		// multiply block address by 512 to get physical address
		return CMD17_Time_out;
	
	if (Resp.r1 != 0)
		return Read_Cmd_Fail;
	
		// Wait for a token
	i = 0;
	do
	{
		token = get_sd_data(spi_clock);
		i++;
	}while ((token == 0xFF) && (i < READ_RESP_TIMEOUT));

	if (token != START_SINGLE_BLOCK_TOKEN)
		return Read_resp_fail;
		
	// Token received so start data transfer

	sdspi_read_start();
	return 0;
	
}

//*********************************************
// Read the CSD Card status block (16 bytes)
//*********************************************

uint8 get_csd(void)
{
	int32 i;
	uint8 token;
	
	
	if (send_sd_cmd(CMD9_SEND_CSD, 0x00) == 0)		
		return ACMD13_Time_Out;
	
	if (Resp.r1 != 0)
		return Read_Cmd_Fail;
	
	// Wait for a token
	i = 0;
	do
	{
		token = get_sd_data(spi_clock);
		i++;
	}while ((token == 0xFF) && (i < READ_RESP_TIMEOUT));


	if (token != START_SINGLE_BLOCK_TOKEN)
		return Read_resp_fail;
	// Token received so start data transfer
	for (i=0;i<18;i++)
		csd[i] = get_sd_data(spi_clock);

	clock_8_delay();
	return 0;
	
}


//*********************************************
// Read the SD Card status block (512 bytes)
//*********************************************

uint8 sd_read_status(void)
{
	int32 i;
	uint8 token;
	
	
	if(send_sd_cmd(CMD55_APP_CMD,0x00) == 0)		// send command 55 to signal next command is an application command
		return CMD55_Time_Out;			// bad response from 

	if (send_sd_cmd(ACMD13_SD_STATUS, 0x00) == 0)		// multiply block address by 512 to get physical address
		return ACMD13_Time_Out;
	
	if ((Resp.r1 != 0) || (Resp.r2 != 0))
		return Read_Cmd_Fail;
	
	// Wait for a token
	i = 0;
	do
	{
		token = get_sd_data(spi_clock);
		i++;
	}while ((token == 0xFF) && (i < READ_RESP_TIMEOUT));

	if (token != START_SINGLE_BLOCK_TOKEN)
		return Read_resp_fail;
	// Token received so start data transfer

	sdspi_read_start();
	return 0;
	
}



//*********************************************
// Send command to read the sd car status
//*********************************************

int32 get_sd_status(void)
{
	int32 time_out;
	uint8 status;
	
	if (sd_read_status() != 0)
		return 0;
		
	status = 0;
	time_out = 0;
	do
	{
		status = get_read_block_status();
		time_out++;
	}while ((status == 0) && (time_out < 0x000FFFFF));
	if (status == 0xff)
		return 0;
	
	if (time_out > 0x000FFFFF)
		return 0;
	
	sd_class = sdspi_rx_dma_buf[440];
	sd_pm = sdspi_rx_dma_buf[432];

	return 1;
}


//***************************************************************
// Params:
//	 Sector: 32-bit sector number
//	 Buffer: Target buffer to write n sectors of data from.
//	 Sector_count: Number of sectors to write.
//
// Return: 
//	 int, 1 = success, 0 = failure.
//
// Description:
//   Application/target specific disk/media write function.
//   Sector number (sectors are usually 512 byte pages) to write to.
//***************************************************************

int32 media_write(uint32 sector, uint8 *buffer, uint32 sector_count)
{
	int32 i;
	int32 time_out;
	uint8 status;
	uint16 j;
	
	j = 0;
	
	for (i=sector;i<(sector+sector_count);i++)
	{
		if (j >= FAT_BUFFERS)
			return 0;
		
		copy_write_data_to_dma_buff(&buffer[j*SECTOR_SIZE]);
		if (sd_write_block(i) != 0)
			return 0;
		
		status = 0;
		time_out = 0;
		do
		{
			status = get_write_block_status();
			time_out++;
		}while ((status == 0) && (time_out < 0x000FFFFF));
		clock_8_delay();
		if (status == 0xff)
			return 0;
		if (time_out > 0x000FFFFF)
			return 0;
		j++;
	}
	return 1;
}

//*****************************************
// Copy Data from User Ram to DMA Buffer
//*****************************************

void copy_write_data_to_dma_buff(uint8 *addr)
{
	uint16 i;
	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
	
	for (i=0;i<SECTOR_SIZE;i++)
		sdspi_tx_dma.un[i].b[lb] = addr[i];

	uwu.w = crc_16(addr,SECTOR_SIZE);
	sdspi_tx_dma.un[i++].b[lb] = uwu.b[hbw];
	sdspi_tx_dma.un[i].b[lb] = uwu.b[lbw];
	
}

//*********************************************
// Write Block of Data to SD Card (512 bytes)
//*********************************************
	
uint8 sd_write_block(uint32 block_addr)
{
	if (sdhc == 0)				// high capacity expects the block number wheras normal capacity expects a physical address
		block_addr <<= 9;
	
	if (send_sd_cmd(CMD24_WRITE_SINGLE_BLOCK, block_addr) == 0)
		return CMD24_Time_out;
	
	if (Resp.r1 != 0)
		return Write_Cmd_Fail;
	
	put_sd_data(spi_clock,0xFF);		// Required timing for data transfer
	
	put_sd_data(spi_clock,START_SINGLE_BLOCK_TOKEN);
	
	sdspi_write_start();
	
	return 0;
	
}


//********************************************
// Get the Status of the Read Blockk Transfer
//********************************************

uint8 get_read_block_status(void)
{
	uint16 read_crc;
	
	if ((sdspi_transferred == 1) && (sdspi_read == 1))
	{
		sdspi_read = 0;
		read_crc = crc_16(sdspi_rx_dma_buf,512);

		spi_clock &= 0x7FFFFFFF;
		put_sd_data(spi_clock,0xFF);		// Delay 8 clock cycles
		return 1;
	}
	
	if (sdspi_read == 0)
		return 0xff;
	
	return 0;
}

//********************************************
// Get the Status of the Write Block Transfer
//********************************************

uint8 get_write_block_status(void)
{
	uint32 i;
	uint8 token;
	
	if ((sdspi_transferred == 1) && (sdspi_write == 1))
	{
		sdspi_write = 0;
		// Wait for a token
		i = 0;
		do
		{
			token = get_sd_data(spi_clock);
			i++;
		}while ((token != 0xFF) && (i < WRITE_RESP_TIMEOUT));

		spi_clock &= 0x7FFFFFFF;
		put_sd_data(spi_clock,0xFF);		// Delay 8 clock cycles
		if (i < WRITE_RESP_TIMEOUT)
			return 1;
	}
	
	if (sdspi_write == 0)
		return 0xff;
	
	return 0;
}

//**************************************************
// This is start the comm transmission on a COM Port
//**************************************************

void sdspi_read_start(void)
{
	sdspi_transferred = 0;
	sdspi_read = 1;
	
	_disable();	   
	// Turn off the SPI transmit and receive fifo
	DSPI_B.RSER.B.TFFF_RE = 0;		// Enable Transmit Fifo Fill
	DSPI_B.RSER.B.RFDF_RE = 0;		// Enable Receive FIFO drain request
	
	// Enable transmitter
	EDMA.CER.R = SDSPIdmaChTx;           	// DMA Clear error Register 
	EDMA.CER.R = SDSPIdmaChRx;           	// DMA Clear error Register 

    EDMA.TCD[SDSPIdmaChTx].SADDR = (uint32) &sdspi_tx_dma_FF;  	// Load address of source data 
  	EDMA.TCD[SDSPIdmaChTx].SOFF = 0;                         		// After transfer, add 2 to src addr
  	EDMA.TCD[SDSPIdmaChTx].SLAST = 0;                      			// After major loop, reset src addr 

  	EDMA.TCD[SDSPIdmaChTx].BITER = c_sdspi_dma_buf_len;                       		// 1 major loop iterations 
  	EDMA.TCD[SDSPIdmaChTx].CITER = c_sdspi_dma_buf_len;                       		// Initialize current iteraction count 

  	EDMA.TCD[SDSPIdmaChRx].DADDR = (uint32) &sdspi_rx_dma_buf[0]; 						// Load address of destination 
  	EDMA.TCD[SDSPIdmaChRx].DOFF = 1;                         		// Increment destination addr by 2 
  	EDMA.TCD[SDSPIdmaChRx].DLAST_SGA = - (c_sdspi_dma_buf_len);                    		// After major loop reset destination address
  	EDMA.TCD[SDSPIdmaChRx].BITER = c_sdspi_dma_buf_len;            // Major loop iterations 
  	EDMA.TCD[SDSPIdmaChRx].CITER = c_sdspi_dma_buf_len;                       	// Initialize current iteraction count 

  	EDMA.SERQR.R = SDSPIdmaChTx;      	// Enable Display SPI EDMA channel 
  	EDMA.SERQR.R = SDSPIdmaChRx;      	// Enable Display SPI EDMA channel 
  	  
	DSPI_B.RSER.B.TFFF_DIRS = 1; 	// Set to generate DMA request for transmit fifo
	DSPI_B.RSER.B.RFDF_DIRS = 1;	// Set to generate DMA request for receive fifo
	DSPI_B.RSER.B.TFFF_RE = 1;		// Enable Tranmsit Fifo Fill
	DSPI_B.RSER.B.RFDF_RE = 1;		// Enable Receive FIFO drain request
	_enable();
}


//**************************************************
// This is start the comm transmission on a COM Port
//**************************************************

void sdspi_write_start(void)
{
	sdspi_transferred = 0;
	sdspi_write = 1;
	
	_disable();	   
	// Turn off the SPI transmit and receive fifo
	DSPI_B.RSER.B.TFFF_RE = 0;		// Enable Transmit Fifo Fill
	DSPI_B.RSER.B.RFDF_RE = 0;		// Enable Receive FIFO drain request
	
	// Enable transmitter
	EDMA.CER.R = SDSPIdmaChTx;           	// DMA Clear error Register 
	EDMA.CER.R = SDSPIdmaChRx;           	// DMA Clear error Register 

    EDMA.TCD[SDSPIdmaChTx].SADDR = (uint32) &sdspi_tx_dma.buf[0];  	// Load address of source data 
  	EDMA.TCD[SDSPIdmaChTx].SOFF = 4;                         		// After transfer, add 2 to src addr
  	EDMA.TCD[SDSPIdmaChTx].SLAST = - (c_sdspi_dma_buf_len * 4);                      		// After major loop, reset src addr 

  	EDMA.TCD[SDSPIdmaChTx].BITER = c_sdspi_dma_buf_len;                       		// 1 major loop iterations 
  	EDMA.TCD[SDSPIdmaChTx].CITER = c_sdspi_dma_buf_len;                       		// Initialize current iteraction count 

  	EDMA.TCD[SDSPIdmaChRx].DADDR = (uint32) &sdspi_rx_dma_buf[0]; 						// Load address of destination 
  	EDMA.TCD[SDSPIdmaChRx].DOFF = 1;                         		// Increment destination addr by 2 
  	EDMA.TCD[SDSPIdmaChRx].DLAST_SGA = - (c_sdspi_dma_buf_len);                    		// After major loop reset destination address
  	EDMA.TCD[SDSPIdmaChRx].BITER = c_sdspi_dma_buf_len;            // Major loop iterations 
  	EDMA.TCD[SDSPIdmaChRx].CITER = c_sdspi_dma_buf_len;                       	// Initialize current iteraction count 

  	EDMA.SERQR.R = SDSPIdmaChTx;      	// Enable Display SPI EDMA channel 
  	EDMA.SERQR.R = SDSPIdmaChRx;      	// Enable Display SPI EDMA channel 
  	  
	DSPI_B.RSER.B.TFFF_DIRS = 1; 	// Set to generate DMA request for transmit fifo
	DSPI_B.RSER.B.RFDF_DIRS = 1;	// Set to generate DMA request for receive fifo
	DSPI_B.RSER.B.TFFF_RE = 1;		// Enable Tranmsit Fifo Fill
	DSPI_B.RSER.B.RFDF_RE = 1;		// Enable Receive FIFO drain request
	_enable();
}


//*****************************************
// Calculate 16 bit CRC for Data (Modbus)
//*****************************************

uint16 modbus_crc_16 (unsigned char* chr, int16 cnt)
{
	uint16 crc_reg = 0xffff;
	int16 j;

	while(cnt > 0)
	{  	
		cnt--;
		crc_reg ^= *chr++;
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

//*****************************************
// Calculate 16 bit CRC for Data (CRC-CCITT(0xFFFF))
//*****************************************

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

//*****************************************
// Calculate 16 bit CRC for Data (SD Card) (xmodem)
//*****************************************

uint16 crc_16(unsigned char *data, int16 cnt)
{
	uint16 crc = 0;
	uint16 i;
	
	for (i=0;i<cnt;i++)
	{
		
    	crc  = (uint16)((uint8)(crc >> 8) | (uint16)(crc << 8));
    	crc ^= data[i];
    	crc ^= (unsigned char)(crc & 0xff) >> 4;
    	crc ^= (crc << 8) << 4;
    	crc ^= ((crc & 0xff) << 4) << 1;
	}
	return (crc &= 0xffff);
}

//*****************************************
// Calculate 16 bit CRC for Data (CRC-16)
//*****************************************

uint16 calc_crc16(uint8 *pBuffer, int16 nLength)
{
	static const uint16 CRC16_TABLE[16] =
	{
	0x0000, 0xCC01, 0xD801, 0x1400,
	0xF001, 0x3C00, 0x2800, 0xE401,
	0xA001, 0x6C00, 0x7800, 0xB401,
	0x5000, 0x9C01, 0x8801, 0x4400
	};

	uint16 wCrc = 0;
	uint16 wTemp;
	int16 nIndex;

	for(nIndex = 0; nIndex < nLength; nIndex++, pBuffer++)
	{
		// lower 4 bits 
		wTemp = CRC16_TABLE[wCrc & 0x000F];
		wCrc = (wCrc >> 4) & 0x0FFF;
		wCrc = wCrc ^ wTemp ^ CRC16_TABLE[*pBuffer & 0x000F];

		// upper 4 bits 
		wTemp = CRC16_TABLE[wCrc & 0x000F];
		wCrc = (wCrc >> 4) & 0x0FFF;
		wCrc = wCrc ^ wTemp ^ CRC16_TABLE[(*pBuffer >> 4) & 0x000F];
	}

	return wCrc;
}

