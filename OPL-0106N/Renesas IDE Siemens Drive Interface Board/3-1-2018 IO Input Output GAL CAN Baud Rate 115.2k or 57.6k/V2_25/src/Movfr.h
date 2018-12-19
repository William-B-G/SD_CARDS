/*
 * Movfr.h
 *
 *  Created on: Apr 24, 2014
 *      Author: williec
 */

#ifndef MOVFR_H_
#define MOVFR_H_

#define ICLK 73728000
#define PCLK 9216000

extern unsigned char  Version;
extern unsigned char  Revision;

/*extern char DOL;
extern char DCL;
extern char GSM;
extern char REOPEN;*/


extern char RUN_PB;
extern char CAM_SETUP_PB;
extern char AUTO_PB;
extern char MANUAL_PB;
extern char CLOSE_PB;
extern char OPEN_PB;
extern char NUDGE_PB;
extern char HEAVY_PB;
extern char CLOSE;
extern char OPEN;
extern char NUDGE;
extern char HEAVY;
extern char GSM;
extern char AUX;
extern char SSFO_IN;
extern char SDO_IN;
extern char SDC_IN;
extern char DOL;
extern char DCL;
extern char REOPEN;
extern char REOPEN_LCSE;
extern char FR_Jumper;
extern char CAN_BaudRate_flag;

void CheckInput(void);


extern unsigned char  FlashFaultCode;


#endif /* MOVFR_H_ */
