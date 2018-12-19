/*
 * Macro.h
 *
 *  Created on: Apr 3, 2014
 *      Author: williec
 */

#ifndef MACRO_H_
#define MACRO_H_
#include"StarterCodes/iodefine.h"


/*  IO Direction */
#define IO_INPUT  0
#define IO_OUTPUT 1


#define ON 1
#define OFF 0




/*//    CAM Signal Input True Table

     FSC Opto       Speed                   SSO/FSO Opto        MSO Opto        Speed
        1            FSC                          1                0             SSO
        0            HSC                          0                0             HSO
                                                  0                1             MSO
                                                  1                1             FSO
 */


/*//   Signal Output to Drive True Table

     H       M       L          Speed
     0       0       0         Holding
     0       0       1         Nudging
     0       1       0           FSC
     0       1       1           SSO
     1       0       0           HSC
     1       0       1           HSO
     1       1       0           MSO
     1       1       1           FSO
 */




//-------------------------------------------------------------
//PORT4,5,6 have no "DDR" setting, so default as Input PORT
//Push Button
/* Push Button signal  */
//ZYM 2-21-2018
//Close Push Button
//-#define CLOSE_PB_HOLD		PORT4.PORT.BIT.B0
#define CLOSE_PB_HOLD		PORT4.PORT.BIT.B5

//Open Push Button
//-#define OPEN_PB_HOLD		    PORT4.PORT.BIT.B1
#define OPEN_PB_HOLD		PORT4.PORT.BIT.B4

//Nudging Push Button
//-#define NUDGE_PB_HOLD		PORT4.PORT.BIT.B2
#define NUDGE_PB_HOLD		PORT4.PORT.BIT.B6

//Heavy Push Button
//-#define HEAVY_PB_HOLD		PORT4.PORT.BIT.B3
#define HEAVY_PB_HOLD		PORT4.PORT.BIT.B7





//-------------------------------------------------------------
//Switch
//Setup Switch
//-#define CAM_SETUP_PB_HOLD	PORT4.PORT.BIT.B5
#define CAM_SETUP_PB_HOLD	PORT4.PORT.BIT.B1

//-#define RUN_PB_HOLD         PORT4.PORT.BIT.B4
//Run Switch
#define RUN_PB_HOLD         PORT4.PORT.BIT.B0

//Auto Switch
//-#define AUTO_PB_HOLD			PORT4.PORT.BIT.B6
#define AUTO_PB_HOLD		PORT4.PORT.BIT.B3

//Manual Switch
//-#define MANUAL_PB_HOLD		PORT4.PORT.BIT.B7
#define MANUAL_PB_HOLD		PORT4.PORT.BIT.B2





//-------------------------------------------------------------
//Setting Pin as Input/Output I/O
//Front Rear Door I/O Direction
//-#define FR_Signal_DIR		PORTB.DDR.BIT.B7
#define FR_Signal_DIR		PORTA.DDR.BIT.B3


// Determines if  its  Front  or Rear
//-#define FR_Signal			PORTB.PORT.BIT.B7
#define FR_Signal			PORTA.PORT.BIT.B3





//-------------------------------------------------------------
//Input Card
//Input Card I/O Direction
//Close Input Card I/O Direction
//-#define CLOSE_DIR   			PORT1.DDR.BIT.B1
#define CLOSE_DIR   		PORT9.DDR.BIT.B4

//Open Input Card I/O Direction
//-#define OPEN_DIR				PORT1.DDR.BIT.B0
#define OPEN_DIR			PORT9.DDR.BIT.B3

//Nudging Input Card I/O Direction
//-#define NUDGE_DIR			PORTA.DDR.BIT.B5
#define NUDGE_DIR			PORT9.DDR.BIT.B2

//Heavy Input Card I/O Direction
//-#define HEAVY_DIR			PORTA.DDR.BIT.B4
#define HEAVY_DIR			PORT9.DDR.BIT.B1



//Close Input Card
//-#define CLOSE_HOLD   		PORT1.PORT.BIT.B1
#define CLOSE_HOLD   		PORT9.PORT.BIT.B4

//Open Input Card
//-#define OPEN_HOLD			PORT1.PORT.BIT.B0
#define OPEN_HOLD			PORT9.PORT.BIT.B3

//Nudging Input Card
//-#define NUDGE_HOLD			PORTA.PORT.BIT.B5
#define NUDGE_HOLD			PORT9.PORT.BIT.B2

//Heavy Input Card
//-#define HEAVY_HOLD			PORTA.PORT.BIT.B4
#define HEAVY_HOLD			PORT9.PORT.BIT.B1






//-------------------------------------------------------------
//CAM
//CAM I/O Direction
//DPM CAM I/O Direction
//-#define GSM_DIR				PORTB.DDR.BIT.B1

//AUX CAM I/O Direction
//-#define AUX_DIR				PORTB.DDR.BIT.B0

//SSO/FSO CAM I/O Direction
//-#define SSFO_IN_DIR			PORT2.DDR.BIT.B2

//MSO CAM I/O Direction
//-#define SDO_IN_DIR			PORT2.DDR.BIT.B3

//FSC CAM I/O Direction
//-#define SDC_IN_DIR			PORT2.DDR.BIT.B4

//DOL CAM I/O Direction
//-#define DOL_DIR 				PORT7.DDR.BIT.B0
#define DOL_DIR 			PORT2.DDR.BIT.B4

//DCL CAM I/O Direction
//-#define DCL_DIR				PORTB.DDR.BIT.B4
#define DCL_DIR			    PORT2.DDR.BIT.B2




//DPM CAM
//-#define GSM_HOLD			PORTB.PORT.BIT.B1

//AUX CAM
//-#define AUX_HOLD			PORTB.PORT.BIT.B0

//SSO/FSO CAM
//-#define SSFO_IN_HOLD		PORT2.PORT.BIT.B2

//MSO CAM
//-#define SDO_IN_HOLD			PORT2.PORT.BIT.B3

//FSC CAM
//-#define SDC_IN_HOLD			PORT2.PORT.BIT.B4

//DOL CAM
//-#define DOL_HOLD 			PORT7.PORT.BIT.B0
#define DOL_HOLD			PORT2.PORT.BIT.B4

//DCL CAM
//-#define DCL_HOLD				PORTB.PORT.BIT.B4
#define DCL_HOLD			PORT2.PORT.BIT.B2





//-------------------------------------------------------------
//Reopen
//Reopen I/O Direction
//From Siemens Drive
//-#define REOPEN_DIR			PORTB.DDR.BIT.B3
#define REOPEN_DIR			PORT2.DDR.BIT.B3

//Reopen
//-#define REOPEN_HOLD			PORTB.PORT.BIT.B3
#define REOPEN_HOLD			PORT2.PORT.BIT.B3




//From Scan Guard
#define REOPEN_LCSE_DIR		PORTA.DDR.BIT.B2
#define REOPEN_LCSE_HOLD	PORTA.PORT.BIT.B2





//-------------------------------------------------------------
/* LED  Signal  */
//Holding LED Output Signal
//Holding LED I/O Direction
//-#define HOLD_LED_DIR		PORT3.DDR.BIT.B0

//MCU Heart Beep I/O Direction
//-#define DIA_LED_DIR			PORTA.DDR.BIT.B2
#define DIA_LED_DIR			PORT3.DDR.BIT.B2

//CAN Communication LED I/O Direction
//-#define COMM_LED_DIR			PORTB.DDR.BIT.B2
#define COMM_LED_DIR		PORT3.DDR.BIT.B3



//-#define HOLD_LED			PORT3.DR.BIT.B0

//Modify
//MCU Heart Beep Output Signal
//-#define DIA_LED				PORTA.DR.BIT.B2
#define DIA_LED				PORT3.DR.BIT.B2

//CAN Communication LED Output Signal
//-#define COMM_LED				PORTB.DR.BIT.B2
#define COMM_LED			PORT3.DR.BIT.B3







//--------------------------------
/* Door Speed Mode  Status */
//FSO Speed LED I/O Direction
/*//-
#define FSO_DIR				PORT9.DDR.BIT.B1

//MSO Speed LED I/O Direction
#define MSO_DIR				PORT9.DDR.BIT.B2

//HSO Speed LED I/O Direction
#define HSO_DIR				PORT9.DDR.BIT.B3

//SSO Speed LED I/O Direction
#define SSO_DIR				PORT9.DDR.BIT.B4

//SDC Speed LED I/O Direction
#define SDC_DIR				PORT3.DDR.BIT.B1

//HSC Speed LED I/O Direction
#define HSC_DIR				PORT3.DDR.BIT.B2
*/
//Nudging Speed Signal and Indicate LED Output I/O Direction
//-#define NSC_DIR				PORT3.DDR.BIT.B3
#define NSC_DIR				PORT7.DDR.BIT.B2

//the I/O Direction of Nudging Bypass the Reopen Hardware Circuit
//-#define NUD_BP_DIR			PORTA.DDR.BIT.B3




/*//-
//FSO Speed LED Output
#define FSO					PORT9.DR.BIT.B1

//MSO Speed LED Output
#define MSO					PORT9.DR.BIT.B2

//HSO Speed LED Output
#define HSO					PORT9.DR.BIT.B3

//SSO Speed LED Output
#define SSO					PORT9.DR.BIT.B4

//SDC Speed LED Output
#define SDC					PORT3.DR.BIT.B1

//HSC Speed LED Output
#define HSC					PORT3.DR.BIT.B2
*/
//Nudging Speed Signal and Indicate LED Output
//-#define NSC					PORT3.DR.BIT.B3
#define NSC					PORT7.DR.BIT.B2

//Nudging Bypass the Reopen Hardware Circuit
//-#define NUD_BP				PORTA.DR.BIT.B3










//-------------------------------------------------------------
/* Drive Signal  */
//Speed Control I/O Direction
/*//-
#define H_DIR				PORT7.DDR.BIT.B1
#define M_DIR   			PORT7.DDR.BIT.B2
#define L_DIR				PORT7.DDR.BIT.B3
*/

//Close Direction
//-#define STF_DIR				PORT7.DDR.BIT.B4
#define STF_DIR				PORT7.DDR.BIT.B0
//-#define STR_DIR				PORT7.DDR.BIT.B5

//Open Direction
#define STR_DIR				PORT7.DDR.BIT.B1
//-#define AUX_DRIVE_DIR		PORT7.DDR.BIT.B6

//Heavy Door
#define AUX_DRIVE_DIR		PORT7.DDR.BIT.B3

//REOPEN RELAY DIR
#define RLY_REOPEN_DIR		PORT7.DDR.BIT.B4

//DCL RELAY DIR
#define RLY_DCL_DIR			PORT7.DDR.BIT.B5

//DOL RELAY DIR
#define RLY_DOL_DIR			PORT7.DDR.BIT.B6




/*//-
//Speed Control I/O Signal Output
#define H					PORT7.DR.BIT.B1
#define M   				PORT7.DR.BIT.B2
#define L					PORT7.DR.BIT.B3
*/
//Close Direction MUST require "STR" Signal for Holding and Running
//-#define STF					PORT7.DR.BIT.B4
#define STF					PORT7.DR.BIT.B0

//Open Direction MUST require "STR" Signal for Holding and Running
//-#define STR					PORT7.DR.BIT.B5
#define STR					PORT7.DR.BIT.B1

//Heavy Door
//-#define AUX_DRIVE			PORT7.DR.BIT.B6
#define AUX_DRIVE			PORT7.DR.BIT.B3

//REOPEN RELAY
#define RLY_REOPEN			PORT7.DR.BIT.B4

//DCL RELAY
#define RLY_DCL				PORT7.DR.BIT.B5

//DOL RELAY
#define RLY_DOL				PORT7.DR.BIT.B6



//-------------------------------------------------------------
//GAL CAN Baud Rate flag 1 -->  57.6k    0 --> 115.2k
#define CAN_BaudRate_HOLD_DIR		PORTB.DDR.BIT.B0

#define CAN_BaudRate_HOLD			PORTB.PORT.BIT.B0

#endif /* MACRO_H_ */























