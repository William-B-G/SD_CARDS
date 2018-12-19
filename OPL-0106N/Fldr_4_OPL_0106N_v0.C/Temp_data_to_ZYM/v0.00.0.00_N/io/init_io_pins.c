/*******************************************************************************
Function Name : InitGPIO
Engineer      : b08110
Date          : Feb-08-2010
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : 
Issues        : NONE
*******************************************************************************/

#define d_GPIO 1
#include "global.h"

void InitGpio (void);
void InitGPIO_p1(void);

void InitGPIO(void)
{    
	int16 i;
		 	
	// I/O Pin Type: I-input only, A-Analog, H-High Voltage, F-Fast, M-Medium, S-Slow

	// Init LED output on PF8
 	SIU.GPDO[Z6_LED].R = 0;
   	SIU.PCR[Z6_LED].R = 0x20C;
 	SIU.GPDO[TST3].R = 0;
	SIU.PCR[TST3].R = 0x20C;
 	SIU.GPDO[TST4].R = 0;
	SIU.PCR[TST4].R = 0x20C;
 	SIU.GPDO[TST5].R = 0;
	SIU.PCR[TST5].R = 0x20C;
 	SIU.GPDO[TST6].R = 0;
	SIU.PCR[TST6].R = 0x20C;

  

		// Set the Transmit and Receive pins for all UART ports
		
	SIU.PCR[TX0].R = 0x400;
	SIU.PCR[RX0].R = 0x400;
	SIU.PCR[TX1].R = 0x400;
	SIU.PCR[RX1].R = 0x400;
	SIU.PCR[TX2].R = 0x400;
	SIU.PCR[RX2].R = 0x400;
	SIU.PCR[TX3].R = 0x400;
	SIU.PCR[RX3].R = 0x400;
	SIU.PCR[TX4].R = 0x400;
	SIU.PCR[RX4].R = 0x400;
	SIU.PCR[TX5].R = 0x400;
	SIU.PCR[RX5].R = 0x400;
	
		// Set RTS lines to outputs 
	SIU.PCR[RTS0].R = 0x20C;
	SIU.PCR[RTS1].R = 0x20C;
	SIU.PCR[RTS2].R = 0x20C;
	SIU.PCR[RTS3].R = 0x20C;
	SIU.PCR[RTS4].R = 0x20C;
	SIU.PCR[RTS5].R = 0x20C;
	
		// Turn off all RTS lines
	SIU.GPDO[RTS0].R = 1;
	SIU.GPDO[RTS1].R = 1;
	SIU.GPDO[RTS2].R = 1;
	SIU.GPDO[RTS3].R = 1;
	SIU.GPDO[RTS4].R = 1;
	SIU.GPDO[RTS5].R = 1;
	
	
		// Set CTS linse to inputs
		
	SIU.PCR[CTS2].R = 0x100;
	SIU.PCR[CTS4].R = 0x100;
	SIU.PCR[CTS5].R = 0x100;
	
		// Set Can port pins
	SIU.PCR[CAN0_TX].R = 0x400;		
	SIU.PCR[CAN0_RX].R = 0x400;		
	SIU.PCR[CAN1_TX].R = 0x400;
	SIU.PCR[CAN1_RX].R = 0x400;
	SIU.PCR[CAN2_TX].R = 0x400;
	SIU.PCR[CAN2_RX].R = 0x400;
	SIU.PCR[CAN3_TX].R = 0x400;
	SIU.PCR[CAN3_RX].R = 0x400;
	
	
		// Set SPI port F
//	SIU.PCR[SPI_CLK].R = 0x600;		
//	SIU.PCR[SPI_OUT].R = 0x600;		
//	SIU.PCR[SPI_IN].R = 0x500;
//	SIU.PCR[SPI_CS].R = 0x600;		// Chip select
//  	SIU.GPDO[SPI_CS].R = 1;			// disable

		// Set RTC SPI port F
//	SIU.PCR[RTC_CLK].R = 0x600;
//	SIU.PCR[RTC_DOUT].R = 0x600;
//	SIU.PCR[RTC_DIN].R = 0x500;
//	SIU.PCR[RTC_CS].R = 0x600;		// Chip select

//  	SIU.GPDO[RTC_CS].R = 1;			// disable
	


	// Port G (pins mixed type MH and SH)
	SIU.PCR[PG0].R = 0x100;	// input
	SIU.PCR[PG1].R = 0x100;
	SIU.PCR[IOBD_CS2n].R = 0x20C;	// output
	SIU.PCR[PG3].R = 0x100;
	
	// Port G inputs (pins mixed type MH and SH)
	for (i=PG8;i<=PG15;i++)
	{
		SIU.PCR[i].R = 0x100;
	}

	// Port H  pin 0 for DBR Temp input (pin type SH)	
	SIU.PCR[DBR_TEMP].R = 0x100;
		
		
	// Port H (all pin type SH except PH2 is MH)
	for (i=PH1;i<=PH7;i++)
	{
		SIU.PCR[i].R = 0x100;
	}
	for (i=PH10;i<=PH15;i++)
	{
		SIU.PCR[i].R = 0x100;
	}
	
	// Port H  pin 8 for Encoder A (pin type SH)
	SIU.PCR[EN1_A].R = 0x500;
	// Port H  pin 9 for Encoder B (pin type SH)
	SIU.PCR[EN1_B].R = 0x500;

	// Port J  INPUTS
	for (i=PJ0;i<=PJ10;i++)
	{
		SIU.PCR[i].R = 0x100;
	}

	SIU.PCR[MRAM_HOLDn].R = 0x20C;	// output
	SIU.PCR[MRAM_HOLDn_2].R = 0x20C;	// output
	SIU.PCR[MRAM_WPn].R = 0x20C;	// output
	SIU.PCR[MRAM_WPn_2].R = 0x20C;	// output
  	
	
	// Secured Digital Port
//	SIU.PCR[SD_CLK].R = 0x600;		// 0x0A00		
//	SIU.PCR[SD_SO].R = 0x600;		// 0x0A00
//	SIU.PCR[SD_SI].R = 0x500;		// 0x0900
//	SIU.PCR[SD_CS].R = 0x600;		// Chip select 0x0A00
//	SIU.PCR[DETECT_SW].R = 0x100;
//	SIU.PCR[PROTECT_SW].R = 0x100;
//  	SIU.GPDO[SD_CS].R = 1;			// disable


}

/*******************************************************************************
Function Name : InitGPIO
Engineer      : b08110
Date          : Feb-08-2010
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : 
Issues        : NONE
*******************************************************************************/
void InitGPIO_p1(void)
{    
	int16 i;
		 	
	// I/O Pin Type: I-input only, A-Analog, H-High Voltage, F-Fast, M-Medium, S-Slow
		 	
     // Init Z0 TST2 LED output on PE15
  	SIU.PCR[Z0_LED].R = 0x20C;
  	SIU.GPDO[Z0_LED].R = 0;
  	SIU.GPDO[Z0_LED].R = 1;

 	// Port A for LCD, M-Ram and Video port input data (all pin type IHA)
	for (i=PA0;i<=PA15;i++)
	{
		SIU.PCR[i].R = 0x0100;		// Input with IBE=1,HYS=0, WPE,WPS = 0
	}
	SIU.PCR[PA14].R = 0x0100;		// Input with IBE=1,HYS=0, WPE,WPS = 0
	SIU.PCR[PA15].R = 0x0100;		// Input with IBE=1,HYS=0, WPE,WPS = 0

 	// Port B for LCD, M-Ram and Video port output data (all pin type SHA)
	for (i=PB0;i<=PB10;i++)
	{
		SIU.PCR[i].R = 0x0100;
	}

 	// Port C for LCD, M-Ram and Video port Address (all pin type SHA)
	for (i=PC0;i<=PC10;i++)
	{
		SIU.PCR[i].R = 0x0100;
	}
	SIU.PCR[PC14].R = 0x0100;		// Input with IBE=1,HYS=0, WPE,WPS = 0
	SIU.PCR[PC15].R = 0x0100;		// Input with IBE=1,HYS=0, WPE,WPS = 0
	

 	// Port K for LCD Push Button Inputs 
	for (i=PK4;i<=PK7;i++)
	{
		SIU.PCR[i].R = 0x0100;		// Input with IBE=1,HYS=0, WPE,WPS = 0
	}
	
	//ZYM 10/26/2017
	SIU.PCR[PB1].R = 0x20C; // pin is output with slew rate set to 'fastest' (wbg)
    SIU.GPDO[PB1].R = 0; // set = 0 when hall com is to Tx, and set = 1 when hall com is set to Rx (as 
        // when testing the RS485 with hall com set to receive a msg from group com).           
}


/*
	Revision History
	
11/21/13 v1.0.5 mhd		1. Turn off all RTS lines to prevent group comm from being shut down on other cars.
8/15/17 v8.0.8 mhd		1. Changed ouput for SIU.PCR[Z6_LED].R and SIU.PCR[Z0_LED].R from 0x22C to 0x20C to make outputs push pull instead of open drain.

*/
