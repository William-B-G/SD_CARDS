/*
 * Flash.c
 *
 *  Created on: May 16, 2014
 *      Author: williec
 */
#include "../StarterCodes/iodefine.h"
#include "../Movfr.h"
#include "FlashError.h"

typedef  __evenaccess volatile unsigned char  * FCU_BYTE_PTR;
typedef  __evenaccess volatile unsigned short * FCU_WORD_PTR;
#define FLASH_ADDRESS  0x00100000

#define FLASH_FENTRYR_TIMEOUT (4)
#define WAIT_TRESW    (35*(ICLK/1000000))

#define FCU_RAM_SIZE 0x2000

#define FLASH_CLOCK_HZ PCLK

/*  According to HW Manual the Max Programming Time for 256 bytes (ROM)
    is 12ms.  This is with a PCLK of 50MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 12ms number is adjusted linearly depending on the PCLK frequency.
*/
#define WAIT_MAX_ROM_WRITE \
        ((int)(12000 * (50.0/(FLASH_CLOCK_HZ/1000000)))*(ICLK/1000000))

/*  According to HW Manual the Max Programming Time for 128 bytes
    (Data Flash) is 5ms.  This is with a PCLK of 50MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 5ms number is adjusted linearly depending on the PCLK frequency.
*/
#define WAIT_MAX_DF_WRITE \
        ((int)(5000 * (50.0/(FLASH_CLOCK_HZ/1000000)))*(ICLK/1000000))

/*  According to HW Manual the Max Blank Check time for 2k bytes
    (Data Flash) is 0.7ms.  This is with a PCLK of 50MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 0.7ms number is adjusted linearly depending on the PCLK frequency.
*/
#define WAIT_MAX_BLANK_CHECK \
        ((int)(700 * (50.0/(FLASH_CLOCK_HZ/1000000)))*(ICLK/1000000))

/*  According to HW Manual the max timeout value when using the peripheral
    clock notification command is 60us. This is with a PCLK of 50MHz. The
    calculation below calculates the number of ICLK ticks needed for the
    timeout delay. The 10us number is adjusted linearly depending on
    the PCLK frequency.
*/
#define WAIT_MAX_NOTIFY_FCU_CLOCK \
        ((int)(60 * (50.0/(FLASH_CLOCK_HZ/1000000)))*(ICLK/1000000))

/*  According to HW Manual the Max Erasure Time for a 16kB block is
    around 300ms.  This is with a PCLK of 50MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 300ms number is adjusted linearly depending on the PCLK frequency.
*/
#define WAIT_MAX_ERASE \
        ((int)(300000 * (50.0/(FLASH_CLOCK_HZ/1000000)))*(ICLK/1000000))



int FlashClockStart = 0;

static int Enter_PE_MODE(void);
static void CLearFlashStatus(void);
static int  FlashClockSetup(void);
static void  FlashReset(void);
static void  Exit_PE_MODE(void);
int WriteToFlash(int DataAddr);
void ReadFromFlash(int DataAddr);
void InitFlash(void);


/**********************************************/
/**              Enter PE Mode               **/
/**********************************************/
static int Enter_PE_MODE(void)
{
	volatile int DelayCnt;
	int ReturnValue;

	/* FENTRYR must be 0x0000 before bit FENTRY0 or FENTRYD can be set to 1 */
	FLASH.FENTRYR.WORD = 0xAA00;

	DelayCnt = FLASH_FENTRYR_TIMEOUT;

	while(FLASH.FENTRYR.WORD != 0x0000)
	{
		if(DelayCnt-- <= 0)
		{
			return FENTRYR_FAILED_OFF;
		}
	}

	FLASH.FENTRYR.WORD = 0xAA80;

	CLearFlashStatus();

	FLASH.FWEPROR.BYTE = 0x01;

	/* Check for FCU error */
	if( (FLASH.FSTATR0.BIT.ILGLERR == 1)
	    ||  (FLASH.FSTATR0.BIT.ERSERR  == 1)
	    ||  (FLASH.FSTATR0.BIT.PRGERR  == 1)
	    ||  (FLASH.FSTATR1.BIT.FCUERR  == 1))
	{
	   /* Return FLASH_FAILURE, operation failure */
		return FWEPROR_FAILED;
	}

	if(FlashClockStart == 0)
	{
		ReturnValue = FlashClockSetup();
		if(ReturnValue != 0)
			return ReturnValue;

		FlashClockStart = 1;
	}

	return 0 ;

}


/**********************************************/
/**              Enter PE Mode               **/
/**********************************************/
static void CLearFlashStatus(void)
{
	FCU_BYTE_PTR ptrb;
	ptrb = (FCU_BYTE_PTR)(FLASH_ADDRESS);

	if(FLASH.FSTATR0.BIT.ILGLERR == 1)
	{
		if(FLASH.FASTAT.BYTE != 0x10)
		{
			FLASH.FASTAT.BYTE = 0x10;
		}
	}
	*ptrb = 0x50;
}



/**********************************************/
/**              Flash Clock Setup           **/
/**********************************************/
static int  FlashClockSetup(void)
{
	volatile int DelayCnt;

	FLASH.PCKAR.BIT.PCKA = 9;



	DelayCnt = 1000;
	while(DelayCnt > 0)
		DelayCnt--;


	/* Execute Peripheral Clock Notification Commands */
	*(FCU_BYTE_PTR)FLASH_ADDRESS = 0xE9;
	*(FCU_BYTE_PTR)FLASH_ADDRESS = 0x03;
	*(FCU_WORD_PTR)FLASH_ADDRESS = 0x0F0F;
	*(FCU_WORD_PTR)FLASH_ADDRESS = 0x0F0F;
	*(FCU_WORD_PTR)FLASH_ADDRESS = 0x0F0F;
	*(FCU_BYTE_PTR)FLASH_ADDRESS = 0xD0;

	DelayCnt = WAIT_MAX_NOTIFY_FCU_CLOCK;

	while(FLASH.FSTATR0.BIT.FRDY == 0)
	{
		if(DelayCnt-- <= 0)
		{
			FlashReset();
			return PCKAR_FRDY_FAILED_OFF;
		}
	}

	if(FLASH.FSTATR0.BIT.ILGLERR == 1)
	{
		return PCKAR_ILGLERR_FAILED;
	}

	return 0;
}


/**********************************************/
/**               Flash Reset                **/
/**********************************************/
static void  FlashReset(void)
{
	volatile int DelayCnt;
	FLASH.FRESETR.WORD = 0xCC01;

	DelayCnt = WAIT_TRESW;
	while (DelayCnt > 0)
	{
		DelayCnt--;
	}

	FLASH.FRESETR.WORD = 0xCC00;

	FLASH.FENTRYR.WORD = 0xAA00;

	DelayCnt = FLASH_FENTRYR_TIMEOUT;

	while(FLASH.FENTRYR.WORD != 0x0000)
	{
		if(DelayCnt-- <= 0)
		{
			break;
		}
	}

	FLASH.DFLRE0.WORD = 0x2D01;  //  Enable Block 0  Read
	FLASH.DFLWE0.WORD = 0x1E01; // Enable Block 0  Write
	FLASH.FWEPROR.BYTE =0x02;

}

/**********************************************/
/**              Exit PE Mode                **/
/**********************************************/
static void  Exit_PE_MODE(void)
{
	volatile int DelayCnt;

	DelayCnt = WAIT_MAX_ERASE;

	while(FLASH.FSTATR0.BIT.FRDY == 0)
	{
		DelayCnt--;
		if(DelayCnt <= 0)
		{
			FlashReset();
			break;
		}
	}

	if(     (FLASH.FSTATR0.BIT.ILGLERR == 1)
	    ||  (FLASH.FSTATR0.BIT.ERSERR  == 1)
	    ||  (FLASH.FSTATR0.BIT.PRGERR  == 1))
	{

		CLearFlashStatus();
	}

	FLASH.FENTRYR.WORD = 0xAA00;

	DelayCnt = FLASH_FENTRYR_TIMEOUT;

	while(FLASH.FENTRYR.WORD != 0x0000)
	{
		if(DelayCnt-- <= 0)
		{
			break;
		}
	}

	FLASH.FWEPROR.BYTE = 0x02;

}

/**********************************************/
/**              Write To Flash              **/
/**********************************************/

int WriteToFlash(int DataAddr)
{
	int ReturnValue;
	volatile int DelayCnt;
	volatile int NumOfWord;

	ReturnValue = Enter_PE_MODE();
	if(ReturnValue!= 0)
		return ReturnValue;

	FLASH.FPROTR.WORD = 0x5501;

	// Erase Previous data
	 *(FCU_BYTE_PTR)FLASH_ADDRESS = 0x20;
	 *(FCU_BYTE_PTR)FLASH_ADDRESS = 0xD0;

	 DelayCnt = WAIT_MAX_ERASE;

	while(FLASH.FSTATR0.BIT.FRDY == 0)
	{
	  	if (DelayCnt-- <= 0)
		{
	  		break;
		}
	}
	*(FCU_BYTE_PTR)FLASH_ADDRESS = 0xFF;

	// Send command to data flash area
		*(FCU_BYTE_PTR)FLASH_ADDRESS = 0xE8;
	   // Specify data transfer size to data flash area
	    *(FCU_BYTE_PTR)FLASH_ADDRESS = 0x04;

	    NumOfWord = 0;
	    while(NumOfWord++ < 4)
	    {
	    	 // Copy data from source address to destination area
	    	  *(FCU_WORD_PTR)FLASH_ADDRESS = *(unsigned short *) DataAddr;

	    	   // Increment data address by two bytes
	    	  DataAddr += 2;
	    }

	    *(FCU_BYTE_PTR)FLASH_ADDRESS = 0xD0;

	    DelayCnt = WAIT_MAX_DF_WRITE;

	    while(FLASH.FSTATR0.BIT.FRDY == 0)
	    {
	    	if (DelayCnt-- <= 0)
	    	{
	    		FlashReset();
	    	    return WRITE_FAILED_NO_FRDY;
	   		}
	   	}


	    if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.PRGERR == 1))
		{
	    	return WRITE_FAILED_ILGLERR;
		}

	    Exit_PE_MODE();
	    return 0;
}



/**********************************************/
/**              Write To Flash              **/
/**********************************************/
void ReadFromFlash(int DataAddr)
{
	int i;
	for(i = 0;i<8;i++)
	{
		*(unsigned char *)DataAddr = *(unsigned char *)(FLASH_ADDRESS + i);
		DataAddr+=1;
	}
}


/**********************************************/
/**                 InitFlash                **/
/**********************************************/
void InitFlash(void)
{
	volatile int DelayCnt;
	volatile int *Src,*Dst;
	int i;

	FlashClockStart  = 0;

	FLASH.FAEINT.BYTE = 0x00;
	FLASH.FRDYIE.BYTE = 0x00;

	 /* Disable flash interface error (FIFERR) */
	IPR(FCU, FIFERR) = 0;
	IEN(FCU, FIFERR) = 0;

	/* Enable the FCU RAM */
	FLASH.FCURAME.WORD = 0xC401;

	Src = (int *)0xFEFFE000;
	Dst = (int *)0x007F8000;
	for( i=0; i<(FCU_RAM_SIZE/4); i++)
	{
		/* Copy data from the source to the destination pointer */
		*Dst = *Src;
		 /* Increment the source and destination pointers */
		 Src++;
		 Dst++;
	}

	FLASH.DFLRE0.WORD = 0x2D01;  //  Enable Block 0  Read
	FLASH.DFLWE0.WORD = 0x1E01; // Enable Block 0  Write

}
