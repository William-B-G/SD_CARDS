#include "MPC5668G.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "IntcInterrupts.h"
#if (GALaxy_4 == 1)
	#include "G4_hmmap.h"
#else 
	#include "hmmap.h"
#endif
#include "mrammap.h"
#include "io.h"
#include "car.h"
#include "display.h"
#include "group.h"
#include "serial.h"
#include "ut.h"

#pragma opt_usedef_mem_limit 580

extern int16 inctime_count;
extern int wbg_02;  // for debug only
extern int wbg_04;  // for debug only
extern unsigned char tempBuff[];
extern void DebouncePushButtons(void);
extern void debug_msg(char *msg, int line);
extern void com_dbg_wbg_dump(int16 comnmb);
extern void Clear_dbg_com_buf(int16 comnmb);

extern int16 DownButton;
extern int16 UpButton;
extern int16 EnterButton;
extern int16 ModeButton;

extern char src[50];
extern char src_B[25];
extern char src_C[25];
extern char dbg_com_buf[3][300];

extern int16 I_WD_index;
extern int16 J_WD_index;

extern uint8 cpld1_B_mlb;  // address for CPLD1 (safety PAL)
extern uint8 cpld1_B_lb; 
extern uint8 cpld2_B_mlb;  // address for CPLD2 (read IO PAL)
extern uint8 cpld2_B_lb;

/**  Button Case   **/
#define ButtonRelease    0
#define ButtonPress     1
#define ButtonPressAck  2
#define ButtonEnable   0
#define ButtonDisable  1

/**  Special Characters  **/

enum
{
  All_Segs_OFF,  //
  All_Segs_On, // also - button down
  Show_Button_Down = 1,
  Show_IO_On,
  Show_Motor_On,
  Show_Valve_On,
  Show_Up_Arrow,
  Show_Down_Arrow,
  Show_Button_Up	
};

/***  FOR CAN TEST with board test SW (wbg) ***/
extern uint8 Group_CAN_Test_OK;
extern uint8 Mr_CAN_Test_OK;
extern uint8 Car_CAN_Test_OK;
extern void comm_control (void);

/***  FOR FPGA TEST with board test SW (wbg) ***/
extern int16 ver_rev;
extern int16 ioUnpkOK; 
extern int16 i1UnpkOK;
extern int16 ntsUnpkOK;



