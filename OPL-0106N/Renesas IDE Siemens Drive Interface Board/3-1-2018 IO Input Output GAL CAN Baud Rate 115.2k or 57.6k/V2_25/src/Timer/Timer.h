/*
 * Timer.h
 *
 *  Created on: Apr 3, 2014
 *      Author: williec
 */

#ifndef TIMER_H_
#define TIMER_H_

extern void InitTimer(void);
extern void IncTimer(void);

extern int TenMs;
extern int LedTimer;
extern int CanAckTimer;
extern int CanTxTimer;
extern int CanRxTimer;
extern int CanOfflineTimer;

#endif /* TIMER_H_ */
