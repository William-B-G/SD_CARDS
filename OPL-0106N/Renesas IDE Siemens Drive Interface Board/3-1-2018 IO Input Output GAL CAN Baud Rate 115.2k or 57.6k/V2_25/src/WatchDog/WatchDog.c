/*
 * WatchDog.c
 *
 *  Created on: Oct 21, 2015
 *      Author: williec
 */
#include "../StarterCodes/iodefine.h"
#include "WatchDog.h"


/************************************************/
/**              Init WatchDog                 **/
/************************************************/
void InitWatchDog(void)
{
	// roughly 1 second timeout  must  petdog before  timeout
	IWDT.IWDTCR.BIT.CKS = 5;
	IWDT.IWDTCR.BIT.TOPS = 1;
	PetDog();
}


/************************************************/
/**              Pet the WatchDog              **/
/************************************************/
void PetDog(void)
{
	IWDT.IWDTRR = 0x00;
	IWDT.IWDTRR = 0xff;
}





