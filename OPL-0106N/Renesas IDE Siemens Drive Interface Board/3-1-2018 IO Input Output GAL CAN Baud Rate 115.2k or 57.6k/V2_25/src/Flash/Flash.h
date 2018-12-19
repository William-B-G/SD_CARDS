/*
 * Flash.h
 *
 *  Created on: May 16, 2014
 *      Author: williec
 */

#ifndef FLASH_H_
#define FLASH_H_

extern int WriteToFlash(int DataAddr);
extern void ReadFromFlash(int DataAddr);
extern void InitFlash(void);

#endif /* FLASH_H_ */
