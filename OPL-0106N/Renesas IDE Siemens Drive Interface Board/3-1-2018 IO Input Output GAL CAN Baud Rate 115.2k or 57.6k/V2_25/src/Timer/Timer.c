/*
 * Timer.c
 *
 *  Created on: Apr 3, 2014
 *      Author: williec
 */


#include "../StarterCodes/iodefine.h"
#include "../Movfr.h"

void InitTimer(void);
void IncTimer(void);


//create the "10ms" time base
#pragma interrupt (TimerInterrupt(vect=28))
void TimerInterrupt(void);


//Modify
//-#define DIA_LED		PORTA.DR.BIT.B2
#define DIA_LED			PORTB.DR.BIT.B2
char can_delay_flag=0;
unsigned can_delay_time=0;

//30*100ms=3000ms
#define can_dt_limit	30



int TenMs = 0;
int LedTimer = 0;
int CanAckTimer = 0;
int CanTxTimer = 0;
int CanRxTimer = 0;
int CanOfflineTimer = 0;







/**********************************************/
/**              Init Timer                  **/
/**********************************************/
void InitTimer(void)
{
	TenMs = 0;

	//The Compare Match Timer (Unit 0) Module stop state is canceled
	SYSTEM.MSTPCRA.BIT.MSTPA15=0;

	/*  MainCLock = 9,216,000 MHz */
	CMT0.CMCR.BIT.CKS = 0; // PCLK/8  9,216,000/8 = 1,152,000
	CMT0.CMCR.BIT.CMIE = 1; // Enable Interrupt
	CMT0.CMCOR = 11520;


	IPR(CMT0, CMI0) = 6;
	IEN(CMT0, CMI0) = 1;

	// Start Timer  //
	CMT.CMSTR0.BIT.STR0 = 1;

}
/**********************************************/
/**            Increment Timer               **/
/**********************************************/
void IncTimer(void)
{
	//"100ms" Cycle
	while(TenMs>=10)
	{
		TenMs -= 10;

		if(LedTimer < 32000)
			LedTimer++;
		if(CanAckTimer <32000)
			CanAckTimer++;
		if(CanTxTimer < 32000 )
			CanTxTimer++;
		if(CanRxTimer < 32000 )
			CanRxTimer++;
		if(CanOfflineTimer < 32000)
			CanOfflineTimer++;

//Modify
		if(can_delay_flag==0)
		{
			can_delay_time++;
			if(can_delay_time>=can_dt_limit)
			{
/*//-
//ZYM 3-1-2018
				//GAL CAN Baud Rate flag 1 -->  57.6k    0 --> 115.2k
				if(CAN_BaudRate_flag)
				{
					CAN0.BCR.BIT.BRP = 9; // 57600=115200/2 =  9,216,000/((BRP + 1) * 16)
				}
				else
				{
					CAN0.BCR.BIT.BRP = 4; // 115200 =  9,216,000/((BRP + 1) * 16)
				}
*/
				Init_CAN();
				can_delay_flag=1;
			}
		}
	}
}

/**********************************************/
/**             Timer  Interrupt             **/
/**********************************************/
//Each "10ms" Cycle
void TimerInterrupt(void)
{
	TenMs++;
	CheckInput();
	IR(CMT0,CMI0) = 0; // clear IR  flag
}
