/*
 * CAN.h
 *
 *  Created on: Apr 24, 2014
 *      Author: williec
 */

#ifndef CAN_H_
#define CAN_H_

extern void SetCAN_ADDR(unsigned char NewAddr);
extern void Init_CAN(void);
extern void CanControl(void);
extern void UpdateCanStatus(void);
extern void PackCan(unsigned char Packet);

extern unsigned char CAN_DOOR_HEAVY;
extern unsigned char CAN_DOOR_CLOSE;
extern unsigned char CAN_DOOR_NUDGE;
extern unsigned char CAN_DOOR_OPEN;

extern unsigned char CanAddr;


extern unsigned char MovfrOnline;

#define RearDoorAddr 8

#endif /* CAN_H_ */







