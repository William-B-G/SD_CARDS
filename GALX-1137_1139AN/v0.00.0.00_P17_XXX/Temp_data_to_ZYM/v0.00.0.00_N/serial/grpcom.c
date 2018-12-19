#define Baud 0		// Set baud rate for comm 2: 0 = 57600, 1=38400, 2=19200
#define d_GRPCOM 1
#include "global.h"

void Small_init_grp_DMA(void);
extern int16 Add_msg_to_array_B(char *msg, int16 index);
extern int16 Log_an_event(char *msg);

/*		Group Comm Description
		Rs-485 half duplex
		8 data bits
		1 start bit
		1 stop bit
		Even parity sent on address
		Odd parity sent for all data

		Receiver set for odd parity only and set to interrupt on error
		When even parity address sent, receiver gets interrupted and verifies address

		This protocol us a modified Modbus RTU protocol. Instead of using a wait time delay to 
		verify the start of a packet, a ':' is transmitted similar to the ascii protocol.  Also, for
		the end of packet, an '@' symbol is sent instead of wait times.	 The BYTE COUNT is always
		sent in the first data byte of the function.  An LRC (Longitudinal Redundancy Check)
		is used instead of a CRC (Cyclical Redundancy Check) and includes the address, function 
		and data.
		
		Comm Protocol

		Master  						Slave             Parity
		START ':'                		':'					SET
		TO ADDRESS						FROM ADDRESS		CLR
		FUNCTION #						FUNCTION #			CLR
		Length							Length				CLR
		Compress zero flag				Compress zero Flag	CLR	  (FOR specific packets)
		DATA 1                  		DATA 1   	        CLR
		DATA 2                  		DATA 2            	CLR
			...                    			...
		DATA LAST               		DATA LAST         	CLR
		LRC CHECKSUM               		LRC CHECKSUM        CLR
		END	'@'							'@'					CLR

		
		Transmit buffer is packed exactly as data is transmitted so the 
		byte count is the number of bytes placed in the gc_trm_buf[]

		Receive buffer is packed from receive data plus the actual length
			and the actual checksum as follows:

			ADDRESS
			FUNCTION/PACKET
			BYTE COUNT
			DATA
			...
			CHECKSUM

		Packets are not tested for a fixed length but for the transmitted
		length being equal to the actual length.


		GROUP BUS
	CAR 1    		  1
	CAR 2             2
	CAR 3             3
	CAR 4             4
	CAR 5             5
	CAR 6             6
	CAR 7             7
	CAR 8             8
	MR_DPY            9
	LBY_DPY           10
	HC1 			  11
	HC2 			  12
	HC3 			  13	
	RHC1	 		  14
	RHC2	 		  15
	RHC3	 		  13	
	IRHC1   		  16
	IRHC2   		  17				
	IRHC3   		  18          
	RIRHC1		 	  19
	RIRHC2		 	  20
	RIRHC3		 	  18

	Packet/Function DESCRIPTION

	PACKET# 	TO 			FROM		BC			DESCRIPTION
	0           xxx         xxx          x          Packet #0: Invalid
	1			CAR			GRP					   	Packet #1: Car Data to Group
	2			GRP			CAR					   	Packet #2: Group Data to Cars
	3 			GRP			CAR						Packet #3: Set a front car call on a slave car
	4 			GRP			CAR						Packet #4: Set a rear car call on a slave car
	5 			GRP			CAR			 5			Packet #5: add a security code
													  Byte Count
													  Floor #	0=invalid, 255=All floors
													  Code 1st byte
													  Code 2nd byte
													  Code 3rd byte
													  Code 4th byte
	6 			GRP			CAR			 5			Packet #6: delete security code
													  Byte Count
													  Floor #	0=invalid, 255=All floors
													  Code 1st byte
													  Code 2nd byte
													  Code 3rd byte
													  Code 4th byte
	7 												Packet #7: Request Security Code Table for Floor
													  Byte Count
													  Floor Table Request
	8 												Packet #8: Request Fault informatin from Car
	9 												Packet #9: Transmit error codes to group
	10												Packet #10: Clear Fault information on Car
	11			ALL			ALL			2			Packet #11: Request Packet
													  Byte Count
	  												  Packet Number (to respond with version, init, etc.)
	12			ALL 		ALL			3 			Packet #12: Send Version and Revision Number
													  Byte Count
													  Device Version
													  Device Revision
	13			GRP			CAR			9-33		Packet #13: Send Security Car Call Lockout data to car
													  Byte Count
													  Security Flag
													  Floors 1 - 8
													  Floors 9 - 16
													  Floors 17 - 24
													  Floors 25 - 32
													  Rear Floors 1 - 8
													  Rear Floors 9 - 16
													  Rear Floors 17 - 24
													  Rear Floors 25 - 29
													  ids Floors 1 - 8
													  ids Floors 9 - 16
													  ids Floors 17 - 24
													  ids Floors 25 - 32
													  ids Rear Floors 1 - 8
													  ids Rear Floors 9 - 16
													  ids Rear Floors 17 - 24
													  ids Rear Floors 25 - 32
														if (high rise car)
														  Floors 33 - 40
														  Floors 41 - 48
														  Floors 49 - 56
														  Floors 57 - 60
														  Rear Floors 33 - 40
														  Rear Floors 41 - 48
														  Rear Floors 49 - 56
														  Rear Floors 57 - 60
														  ids Floors 33 - 40
														  ids Floors 41 - 48
														  ids Floors 49 - 56
														  ids Floors 57 - 60
														  ids Rear Floors 33 - 40
														  ids Rear Floors 41 - 48
														  ids Rear Floors 49 - 56
														  ids Rear Floors 57 - 60
	14			GRP			CAR			 2			Packet #14: Send Code Blue floor to Car
													  Byte Count
													  Front Code blue floor
													  Rear Code Blue floor
	15			GRP			CAR			9-17		Packet #15: Send Security Car Call Lockout Override data to car
													  Byte Count
													  Security Override Flag
													  Floors 1 - 8
													  Floors 9 - 16
													  Floors 17 - 24
													  Floors 25 - 32
													  Rear Floors 1 - 8
													  Rear Floors 9 - 16
													  Rear Floors 17 - 24
													  Rear Floors 25 - 32
														if (high rise car)
														  Floors 33 - 40
														  Floors 41 - 48
														  Floors 49 - 56
														  Floors 57 - 60
														  Rear Floors 33 - 40
														  Rear Floors 41 - 48
														  Rear Floors 49 - 56
														  Rear Floors 57 - 60
	16												Unused
	17			HC 			CAR			4			Packet #17: Hall call device initialization
													  Byte Count
										 			  Valid Floors 1-8 Down, 13-20 Down,
										 			  Valid Floors 9-12 Both, 21-24 Both (Down calls are lower bits)
										 			  Valid Floors 1-8 Up, 13-20 Up,
										 			  Master Version Number
													  Response: Packet 12
	18			HC			GRP			6			Packet #18: Set AND cancel Hall Call (bit set for each floor)
													  Byte Count
									 				  Set Floors 1-8 Down, 13-20 Down,
									 				  Set Floors 9-12 Both, 21-24 Both (Down calls are lower bits)
									 				  Set Floors 1-8 Up, 13-20 Up,
									 			   	  Cancel Floors 1-8 Down, 13-20 Down,
									 			   	  Cancel Floors 9-12 Both, 21-24 Both (Down calls are lower bits)
									 			   	  Cancel Floors 1-8 Up, 13-20 Up,
													  Response: Packet 19 or 11 (if not initialized)
	19			GRP			HC	  	    3		   	Packet #19: Hall Call input (bit set for each floor)
												   	  Byte Count
												   	  Floors 1-8 Up, 13-20 Up,
												   	  Floors 1-8 Down, 13-20 Down,
												   	  Floors 9-12 Both, 21-24 Both (Up calls are lower bits)
	20	 		GRP 		SEC			1			Packet #20: Request security data
												   	  Byte Count
													  Response: Packet 21
	21			SEC			GRP			3			Packet #21:	Security data from hall serial board
												   	  Byte Count
													  Floors 1-8,	Floors 25-29
													  Floors 9-16,	Rear Floors 25-28 
													  Floors 17-24,	Rear Floor 29
	22			GRP			CAR			16			Packet #22: Send gpi input data to car
													  Byte Count
													  gpi[0] - gpi[11]
	23			CAR			GRP			16			Packet #23: Send gpo output data to group
													  Byte Count
													  gpo[0] - gpo[11]
	24			GRP			CAR						Packet #24: Request cgpo output data to group from car 
	25			GRP			ALL						Packet #25: PI lantern message for simple hydro
	26	 		ALL			CAR			 2	   		Packet #26: Send/Request version and revision
													  Byte Count
													  Version Number
													  revision	  

	27			HC 			CAR			22			Packet #27: Hall call driver device initialization
													  Byte Count      
													  				  1st Driver Brd,    2nd Driver Brd,    3rd Driver Brd		  2nd Driver Brd < 30 floors
										 			  Valid Floors   2-8 Down(or RD), 25-32 Down(or RD), 49-56 Down				   25-30 Down
										 			  Valid Floors  9-16 Down(or RD), 33-40 Down(or RD), 57-60 RDown/57-60 Down	   25-28 RDown
										 			  Valid Floors 17-24 Down(or RD), 41-48 Down(or RD), 49-56 RDown			   29-30 RDown
										 			  Valid Floors     1-8 Up(or RU),   25-32 Up(or RU), 49-56 Up				   25-30 Up
										 			  Valid Floors    9-16 Up(or RU),   33-40 Up(or RU), 57-60 RUp/57-60 Up		   25-28 RUp
										 			  Valid Floors   17-24 Up(or RU),   41-48 Up(or RU), 49-56 RUp				   29-30 RUp
										 			  Master Version Number
													  Master Revision Number
										 			  Security Floor Masks     	 1-8,   		  25-32, 49-56 					   25-30 
										 			  Security Floor Masks   	9-16,   		  33-40, 57-60 					   25-28 
										 			  Security Floor Masks     17-24,   		  41-48, 49-56 					   29-30 
													  HC On Brightness	 (to remain backward compatible)
													  HC Off Brightness	 (to remain backward compatible)
		
													  Red Up On Time	  (up hc or up ir)
													  Red Up Off Time
													  Green Up On Time
													  Green Up Off Time
													  Blue Up On Time
													  Blue Up Off Time

													  Sec Red On Time
													  Sec Green On Time
													  Sec Blue On Time
													  Special Control
														bit 0: 1 = all boards non-latching
														bit 1-7: unused

													  if (version greater than 3)
												 	  	  cb/vip Floor Masks     	1-8,   		  	  25-32, 49-56 					   25-30 
												 	  	  cb/vip Floor Masks   		9-16,   		  33-40, 57-60 					   25-28 
												 	  	  cb/vip Floor Masks     	17-24,   		  41-48, 49-56 					   29-30 

													  	  Red Down On Time		 (dn hc or dn ir)
													  	  Red Down Off Time
													  	  Green Down On Time
													  	  Green Down Off Time
													  	  Blue Down On Time
													  	  Blue Down Off Time

													  	  cb/vip Red On Time	   (cb or vip)
													  	  cb/vip Green On Time
													  	  cb/vip Blue On Time

													  Response: Packet 12

	28			HC			GRP			23			Packet #28: Hall Call Driver Set AND cancel Hall Call (bit set for each floor)
													  Byte Count    
													  			    1st Driver Brd,    2nd Driver Brd,    3rd Driver Brd		   2nd Driver Brd < 30 floors
									 				  Set Floors   2-8 Down(or RD), 25-32 Down(or RD), 49-56 Down				    25-30 Down
									 				  Set Floors  9-16 Down(or RD), 33-40 Down(or RD), 57-60 RDown/57-60 Down	    25-28 RDown
									 				  Set Floors 17-24 Down(or RD), 41-48 Down(or RD), 49-56 RDown				    29-30 RDown
									 				  Set Floors     1-8 Up(or RU),   25-32 Up(or RU), 49-56 Up					    25-30 Up
									 				  Set Floors    9-16 Up(or RU),   33-40 Up(or RU), 57-60 RUp/57-60 Up		    25-28 RUp
									 				  Set Floors   17-24 Up(or RU),   41-48 Up(or RU), 49-56 RUp				    29-30 RUp
													  				   1st Driver Brd,    2nd Driver Brd,    3rd Driver Brd		   2nd Driver Brd < 30 floors
									 			   	  Cancel Floors   2-8 Down(or RD), 25-32 Down(or RD), 49-56 Down			    25-30 Down
									 			   	  Cancel Floors  9-16 Down(or RD), 33-40 Down(or RD), 57-60 RDown/57-60 Down    25-28 RDown
									 			   	  Cancel Floors 17-24 Down(or RD), 41-48 Down(or RD), 49-56 RDown			    29-30 RDown
									 			   	  Cancel Floors     1-8 Up(or RU),   25-32 Up(or RU), 49-56 Up				    25-30 Up
									 			   	  Cancel Floors    9-16 Up(or RU),   33-40 Up(or RU), 57-60 RUp/57-60 Up	    25-28 RUp
									 			   	  Cancel Floors   17-24 Up(or RU),   41-48 Up(or RU), 49-56 RUp				    29-30 RUp
                                                      HCB Control
															bit0: 1 = Invert Security
								                          	bit1: 1 = Flash Ssecurity
															bit2: 1 = Master Sec Enable	
															bit3: 1 = Master Hall Call Security
															bit4: 1 = Flash CodeBlue/Vip
															bits 5,6 = unused
															bit7 = Reset and init hall staions boards

															Checksum for on/off brightness, rgb up and rgb sec
													  Checksum of Light PWM Data low byte		 (up hc or up ir)  and (sec)
													  Checksum of Light PWM Data high byte	
													  if version > 3
										 			   	  cb/vip Set Floors     1-8 cb/vip,   25-32 Up(or RU), 49-56 Up				    25-30 Up
										 			   	  cb/vip Set Floors    9-16 cb/vip,   33-40 Up(or RU), 57-60 RUp/57-60 Up	    25-28 RUp
										 			   	  cb/vip Set Floors   17-24 cb/vip,   41-48 Up(or RU), 49-56 RUp				    29-30 RUp
										 			   	  cb/vip Cancel Floors     1-8 cb/vip,   25-32 Up(or RU), 49-56 Up				    25-30 Up
										 			   	  cb/vip Cancel Floors    9-16 cb/vip,   33-40 Up(or RU), 57-60 RUp/57-60 Up	    25-28 RUp
										 			   	  cb/vip Cancel Floors   17-24 cb/vip,   41-48 Up(or RU), 49-56 RUp				    29-30 RUp
																Checksum for rgb down and rgb cb/vip
														  Checksum of Light PWM Data low byte	   (dn hc or dn ir) and (cb or vip)
														  Checksum of Light PWM Data high byte	

													  Response: Packet 29 or 11 (if not initialized)

	29			GRP			HC	  	    14-20		   	Packet #29: Hall Call Driver input (bit set for each floor)
												   	  Byte Count  
													  			1st Driver Brd,    2nd Driver Brd,    3rd Driver Brd			  2nd Driver Brd < 30 floors
												   	  Floors   2-8 Down(or RD), 25-32 Down(or RD), 49-56 Down					   25-30 Down
												   	  Floors  9-16 Down(or RD), 33-40 Down(or RD), 57-60 RDown/57-60 Down		   25-28 RDown
												   	  Floors 17-24 Down(or RD), 41-48 Down(or RD), 49-56 RDown					   29-30 RDown
												   	  Floors     1-8 Up(or RU),   25-32 Up(or RU), 49-56 Up						   25-30 Up
												   	  Floors    9-16 Up(or RU),   33-40 Up(or RU), 57-60 RUp/57-60 Up			   25-28 RUp
												   	  Floors   17-24 Up(or RU),   41-48 Up(or RU), 49-56 RUp					   29-30 RUp
													  Driver Board Status 1:
																Bit 0: 1 = Rx from top HC board fault
																Bit 1: 1 = Tx to top HC board fault
																Bit 2: 1 = Too few stations on bus
																Bit 3: 1 = Too many stations on bus
																Bit 4: 1 = Rx from bottom HC board fault
																Bit 5: 1 = Tx to bottom HC board fault
													  Driver Board Status 2:
														Bit 1,0:	Board Fault Index  0=floors 1-6,1=floors 7-12, 2=floors 13-18, 3=floors 19-24
														Bit 2:		Security inputs returned, 0=faults, 1=Sec Inputs
														Bit 3:		Send both faults and security inputs 1 = both
														Bit 4:		Request Light Timer settings
														Bits 5-7:	Unused    
													  
													  if ((Bit2 == 0) || (Bit3 == 1))
														HC+1 Board Fault
													  	HC+2 Board Fault
													  	HC+3 Board Fault
													  	HC+4 Board Fault
													  	HC+5 Board Fault
													  	HC+6 Board Fault
														   fault codes :
															   0  - none
															   1  - tx down
															   2  - tx up
															   3  - rx down
															   4  - rx up
															   5  - stuck button down
															   6  - stuck button up
															   7  - led open down
															   8  - led open up
															   9  - led short down
															   10 - led short up
															   11 - fet open down
															   12 - fet open up
															   13 - fet short down
															   14 - fet short up
															   15 - Invalid Floor
															   16 - No Comm
															   17 - Device reset
													 
													  else if ((Bit2 == 1) || (Bit3 == 1))
																			1st Driver Brd,     2nd Driver Brd,    3rd Driver Brd			  2nd Driver Brd < 30 floors
												   	  	Aux Floor Input   1-8 Aux (or RAux), 25-32 Aux(or RAux),	 	49-56 Aux					   25-30 Aux	
													  	Aux Floor Input   9-16 Aux(or RAux), 33-40 Aux(or RAux), 		57-60 RAux/57-60 Aux		   25-28 RAux 
												   	  	Aux Floor Input  17-24 Aux(or RAux), 41-48 Aux(or RAux), 		49-56 RAux					   29-30 RAux

													    Sec Floor Input    1-8 Sec(or RSec), 25-32 Sec(or RSec), 		49-56 Sec					   25-30 Sec	
													    Sec Floor Input  9-16 Sec (or RSec), 33-40 Sec(or RSec), 		57-60 RSec/57-60 Sec		   25-28 RSec
													    Sec Floor Input 17-24 Sec (or RSec), 41-48 Sec(or RSec), 		49-56 Sec					   29-30 RSec


	30			GRP			HC	  	    50		   	Packet #30: Hall Call Driver and serial board version
														Byte Count
														HC 1 board version
														HC 1 board revision
														HC 2 board version
														HC 2 board revision
														HC 3 board version
														HC 4 board revision
														HC 4 board version
															.
															.
															.
														HC 23 board revision
														HC 23 board version
														HC 24 board revision
														HC 24 board version

	31			GRP			CARS  	    8-120		   	Packet #31: Send Security Code Checksum and Number of Codes for floors 1-30
														Byte Count
														Floor Start
														Number of Codes low byte Floor 1
														Number of Codes high byte Floor 1
														Checksum low byte Floor 1
														Checksum high byte Floor 1
															   .
															   .
															   .
														Number of Codes low byte Floor n (max n = 30)
														Number of Codes high byte Floor n
														Checksum low byte Floor n
														Checksum high byte Floor n

	32			GRP			CARS		8-120			Packet #32: Send Floor Security Codes (50 codes at a time) (each floor)
														Byte Count
														Floor #
														Code Index	low byte
														Code Index 	high byte
														Code 1 1st byte
														Code 1 2nd byte
														Code 1 3rd byte
														Code 1 4th byte 
																.
																.
																.
														Code n 1st byte		(max n = 50)
														Code n 2nd byte
														Code n 3rd byte
														Code n 4th byte 

	33 			GRP			CAR			 5			Packet #33: delete security code
													  Byte Count
													  Floor #	0=invalid, 255=All floors

	34			HC			GRP			27			Packet #34: Hall Call Station Security Mask, Color and Brightness 
														Byte Count
										 			  Security Floor Masks     	 1-8,   		  25-32, 49-56 					   25-30 
										 			  Security Floor Masks   	9-16,   		  33-40, 57-60 					   25-28 
										 			  Security Floor Masks     17-24,   		  41-48, 49-56 					   29-30 
													  HC On Brightness
													  HC Off Brightness
													  Red On Time  (up hc or up ir)
													  Red Off Time (up)
													  Green On Time	(up)
													  Green Off Time (up)
													  Blue On Time (up)
													  Blue Off Time (up)

													  Sec Red On Time
													  Sec Green On Time
													  Sec Blue On Time
													  if (version > 3)
														  Red Down On Time (dn hc or dn ir)
														  Red Down Off Time
														  Green Down On Time
														  Green Down Off Time
														  Blue Down On Time
														  Blue Down Off Time

														  cb/vip Red On Time  (cb or vip)
														  cb/vip Green On Time
														  cb/vip Blue On Time

	35			CAR			GRP			12			Packet #35: Line and Door Voltage from Car 
														Byte Count
										 			  L1 Voltage low byte
													  L1 Voltage high byte
										 			  L2 Voltage low byte
													  L2 Voltage high byte
										 			  L3 Voltage low byte
													  L4 Voltage high byte
										 			  Door Voltage low byte
													  Door Voltage high byte
													  
	36			GRP			CAR			4			Packet #36: Send confirmation that all faults have been sent 
														 Byte Count

	37			CAR			GRP			12			Packet #37:   Send Car  floor lockout  info to other Car
 														Byte Count
  													low byte ui_cc_sec[0]
													low middle byte ui_cc_sec[0]
													high middle byte ui_cc_ssec[0]
													high byte ui_cc_sec[0]
													low byte ui_cc_sec[1]
													low middle byte ui_cc_sec[1]
													high middle byte ui_cc_ssec[1]
													high byte ui_cc_sec[1]

	38			CAR 		GRP		   125			Packet #38:  Group parameter update to cars
														Byte Count	 
													Table number (Paremeter Menu)
														0 = no request
													    1 =	Group Dispatch
														2 = Group Options
														3 = CC/COP PB lights
														4 = HC/IR Pb Lights
														5 = CB/VIP/SEC PB Lights
													parameter 0 value low byte
													parameter 0 value high byte
														.
														.
														.
													parameter 59 value low byte
													parameter 59 value high byte

	39			GRP 		CAR		   5			Packet #39: Request Group parameter/service timer update
														Byte Count
													Table Number (parameter menu or service timer)
														values 0 to 20 are for parameter table
														values 21 to 31 are for service timers 0 to 10
														0 = no request

	40			CAR 		GRP		   26			Packet #40: Group parameter table checksums
														Byte Count

													byte parameter checksum indes 
														(0= grp_dispatch... cb/vip/sec)

												    low byte Group Dispatch	checksum
												    high byte Group Dispatch checksum
													low byte Group Options checksum
													high byte Group Options checksum
													low byte CC/COP PB lights checksum
													high byte CC/COP PB lights checksum
													low byte HC/IR Pb Lights checksum
													high byte HC/IR Pb Lights checksum
													low byte CB/VIP/SEC PB Lights checksum
													high byte CB/VIP/SEC PB Lights checksum

													byte service timer checksum index 
														(0 = service timers 0-4)
														(1 = service timers 5-9)
												    low byte Service Timer 1 or 5 checksum
												    high byte Service Timer 1 or 5 checksum
												    low byte Service Timer 2 or 6 checksum
												    high byte Service Timer 2 or 6 checksum
												    low byte Service Timer 3 or 7 checksum
												    high byte Service Timer 3 or 7 checksum
												    low byte Service Timer 4 or 8 checksum
												    high byte Service Timer 4 or 8 checksum
												    low byte Service Timer 5 or 9 checksum
												    high byte Service Timer 5 or 9 checksum
													

	41			CAR 		GRP		   	55		Packet #41:  Group service timer update to cars
														Byte Count	 
													Service timer number + 1
														0 = no request
													    1 =	Service timer 0
														2 = service timer 1
															.
															.
															.
														10 = service timer 9
													service timer n value byte 0
														.
														.
														.
													service timer n value byte 49
													
	60			CAR			GRP			XX		Packet #60: Send car iodata to group
												 SEE pack_car_iodata();
*/



//*********************************
// Group Comm Definitions
//*********************************

#define e_ok 0
#define e_inv_address 0x01
#define e_inv_pkt_st 0x02
#define e_inv_len 0x04
#define e_inv_ck_sum 0x08
#define e_inv_pkt_end 0x10
#define e_inv_parity 0x20
#define e_inv_case 0x40
#define e_FE_OE 0x80


// Device number declaratons

#define c_CAR1_addr 1
#define c_CAR2_addr 2
#define c_CAR3_addr 3
#define c_CAR4_addr 4
#define c_CAR5_addr 5
#define c_CAR6_addr 6
#define c_CAR7_addr 7
#define c_CAR8_addr 8
#define c_MR_DPY_addr 9
#define c_LBY_DPY_addr 10
#define g_invalid_io_loc 0
#define g_invalid_hc1_addr 11

#define c_com_no_dev 15			// Max of 15 device address


#define c_trm_buf_len 250
#define c_trm_buf_size 400
#define c_rcv_dma_buf_len 300
#define c_rcv_dma_buf_size 400
#define c_rcv_buf_len 250
#define c_rcv_buf_size 400


#if (Baud == 0)		  // Higher baud rate (57600)
 #define c_slave_min_time_out 2			// 30 Msec
 #define c_slave_max_time_out 7			// 70 Msec
 #define c_master_packet_time 10
 #define c_master_time_out 30
 #define c_full_update_count 30		// roughly 1.5 seconds
 int16 const min_response_wait[3] = {4,2,0};
 int16 const master_count[9] = {200,200,150,130,120,110,100,90,80};		 // roughly 1-5 seconds
 int16 const com_master_time_out[9]= {3,3,3,4,4,5,5,6,6};
//  int16 const com_master_time_out[car_size]= {2,2,2,3,3,4,4};
#else
 #if(Baud == 1)
  //  Baud rate at 38400
  #define c_slave_min_time_out 4			// 40 Msec
  #define c_slave_max_time_out 8
  #define c_master_packet_time 15
  #define c_master_time_out 35
  #define c_full_update_count 35	// roughly 1.5 seconds
  int16 const min_response_wait[3] = {5,3,1};
  int16 const master_count[9] = {150,150,130,120,110,100,90,80,70};		 // roughly 1-5 seconds
  int16 const com_master_time_out[9]= {3,3,3,4,4,5,5,6,6};
 #else
  #define c_slave_min_time_out 10			// 100 Msec
  #define c_slave_max_time_out 16
  #define c_master_packet_time 20
  #define c_master_time_out 40
  #define c_full_update_count 15	// roughly 1.5 seconds
  int16 const min_response_wait[3] = {8,4,2};
  int16 const master_count[9] = {100,100,75,68,60,55,50,40,30};		 // roughly 1-5 seconds
  int16 const com_master_time_out[9]= {3,3,3,4,4,5,5,6,6};
 #endif
#endif

								
int16 Dispatcher = 99;		// Dispatcher for this car
int16 prev_Packet = 0;
int16 gc_diag_Car[9] = {0,0,0,0,0,0,0,0,0};		// car to send diagnostic data	
bool long_door_time = 0;
bool set_long_door_time = 0;

int32 gc_source=0;						// Group comm interrupt source
int16 gc_count=0;						// Group comm interrput count
int16 gc_hc_dev=0;						// Set to 1 when hall call serial board used
int16 gc_hall_io_dev=0;					// Set to 1 when using any serial hall i/o device
int16 gc_restart_req=0;					// restart request
int16 gc_no_response = 0;				// no response expected on comm
int16 gc_unpack_error = 0;				// Unpack flag set but no data unpacked
int16 gc_slave_tx_start = 0;			// Flag to start response transmit routine from slave car
int16 gc_ix = 0;							// index for elevator or device to communicate with
int16 gc_addr = 0;            			// Address sent or received on comm
int16 gc_port = 0;						// Group Comm port
int16 gc_master = 0;					// Set to 1 if this car is master
int16 gc_master_cnt = 0;				// Counter to check for car 1
int16 gc_car = 1;						// Group Comm car 
int16 gc_tx_grp_car_data = 0;
				// List of devices on group comm
int16 gc_dev_list[c_com_no_dev];		// device numbers are placed sequentially in list
				// Version number for each device
int16 gc_dev_ver[c_com_no_dev];			// Device number is used as index
				// Revision number for each device
int16 gc_dev_rev[c_com_no_dev];			// Device number is used as index
int16 gc_dev_online[c_com_no_dev];		// Device number is used as index
				// Transmit packet count
uint16 gc_dev_tx_cnt[c_com_no_dev];		// Device number is used as index
				// Receive packet count
uint16 gc_dev_rx_cnt[c_com_no_dev];		// Device number is used as index
uint16 gc_loss_cnt[c_com_no_dev];		// Device number is used as index
int16 gc_pkt_11_req_pkt[5] = {0,0,0,0,0};					// Used to request a packet from packet 11
int16 pkt_11_gix = 0;					// packet 11 get index
int16 pkt_11_pix = 0;					// packet 11 put index
int16 gc_num_dev = 0;					// Number of devices on comm
int16 gc_num_hc_dev = 0;				// Number of hall call devices on serial comm
int16 gc_num_car_dev = 0;				// Number of cars on serial comm
int16 gc_trm_int = 0;    				// Car comunication port transmit interupt running
int16 gc_trm_buf_empty = 0;				// Communications port transmit buffer is empty
int16 gc_response_wait = 0;				// Wait for response when set before transmitting again
int16 slave_max_time_out = 7;			// Slave maximum time out
int16 response_wait_time = 5;			// Response wait time (depends on the number of devices)
int16 gc_trm_case = 0;        			// Car switch case index for transmit state machine
int16 gc_trm_ix = 0;          			// trasmit buffer index
int16 gc_trm_len = 0;  			    	// trasmit buffer length

int16 gc_valid_start = 0;				// car received a valid start flag
int16 gc_valid_addr = 0;				// car received a valid address to any device
int16 gc_rcv_int = 0;     				// Car comunication port receive interupt running
int16 gc_rcv_case = 0;        			// Car switch case index for receive state machine
int16 gc_rcv_com_status = 0;			// Receive comm status bits
int16 gc_unpack = 0;          			// Set to 1 to unpack Car receive buffer
int16 gc_rcv_pix = 0;          			// Car receive buffer put index
int16 gc_rcv_dma_gix = 0;          		// Car receive buffer dma get index
int16 gc_rcv_gix = 0;          			// Car receive buffer get index
uint8 gc_rcv_len = 0;					// Car receive length sent in Packet
uint8 gc_rcv_act_len = 0;				// Car actual receive length of Packet
uint8 gc_rcv_ck_sum = 0;				// Car receive comm checksum
uint16 gc_trm_buf[c_trm_buf_size];  	// transmit buffer
uint8 gc_rcv_buf[c_rcv_buf_size];  		// receive buffer
uint16 gc_rcv_dma_buf[c_rcv_dma_buf_size];

bool Car_Not_Talking[car_size]; // car not communication one 484 port
bool full_update_req[car_size];	// Request by group for car to send a full update

int16 full_update_count = 0;
int16 full_update_car = 1;
bool car_full_update_req = 1;			// Car flag to send a full update to the group
bool car_iodata = 0;			
bool alternate_io_data = 0;					
bool iodata_full_update_req = 1;		// Car iodata flag to send a full update to the group
bool update_flag=0;
bool Group_Comm = 0;					// Group comm with car
uint16 parity_status = 0;

bool std_pkt_tx[11] = {0,0,0,0,0,0,0,0,0,0,0};		// standard packet 2 was transmitted when == 1

// Data arrays to compress transmission from car to group

#define c_buf_size 512
uint8 z_trm_buf[c_buf_size];		// temporary transmit hold buffer to remove zero's
uint8 z_rcv_buf[c_buf_size];		// temporary receive hold buffer to remove zero's
uint8 pack_buf[c_buf_size];			// Car data packed into this buffer first
uint8 hold_buf[c_buf_size];			// Buffer used to compare packed data with data transmitted previously to send data that has changed
uint8 iodata_hold_buf[c_buf_size];		// Buffer used to compare packed data with data transmitted previously to send data that has changed
uint8 unpack_buf[8][c_buf_size];	// Group unpacks the car data that has changed into a buffer for each car
uint8 iodata_unpack_buf[8][c_buf_size];	// Group unpacks the car data that has changed into a buffer for each car


// Hall call comm I/O's and variables

bool gc_luc = 0;					// Lobby Up request for seperate i/o board
bool gc_luc_canc = 0;				// Lobby Up request cancel for seperate i/o board

uint32 gc_uc[2]={0,0};				// Up calls from comm
uint32 gc_uc_set[2]={0,0};			// Set up calls on comm
uint32 gc_uc_canc[2]={0,0};			// Cancel up calls on comm

uint32 gc_dc[2]={0,0};				// Down calls from comm
uint32 gc_dc_set[2]={0,0};			// Set down calls on comm
uint32 gc_dc_canc[2]={0,0};			// Cancel down calls on comm

uint32 gc_ruc[2]={0,0};				// Up calls from comm
uint32 gc_ruc_set[2]={0,0};			// Set up calls on comm
uint32 gc_ruc_canc[2]={0,0};		// Cancel up calls on comm

uint32 gc_rdc[2]={0,0};				// Down calls from comm
uint32 gc_rdc_set[2]={0,0};			// Set down calls on comm
uint32 gc_rdc_canc[2]={0,0};		// Cancel down calls on comm

uint32 gc_uc_dsr[2]={0,0};  		// Up calls from dual serial riser comm
uint32 gc_dc_dsr[2]={0,0};  		// Down calls from  dual serial riser comm
uint32 gc_ruc_dsr[2]={0,0}; 		// Up calls from  dual serial riser comm
uint32 gc_rdc_dsr[2]={0,0}; 		// Down calls from  dual serial riser comm

uint8 gc_ir=0;						// Inconspicious riser switch activation (last floor down call + 1)
uint8 gc_ir_2=0;						// Inconspicious riser switch activation for second car (last floor down call + 2)
uint8 ir_hc=0;						// Use ir for std hall call if set
uint8 rir_hc=0;						// Use rir for std hall call if set
uint8 gc_tr=0;						// Third riser switch activation (last floor down call + 1)

uint32 gc_iruc[2]={0,0};			// Up calls from comm
uint32 gc_iruc_set[2]={0,0};		// Set up calls on comm
uint32 gc_iruc_canc[2]={0,0};		// Cancel up calls on comm

uint32 gc_irdc[2]={0,0};			// Down calls from comm
uint32 gc_irdc_set[2]={0,0};		// Set down calls on comm
uint32 gc_irdc_canc[2]={0,0};		// Cancel down calls on comm

uint32 gc_riruc[2]={0,0};			// Up calls from comm
uint32 gc_riruc_set[2]={0,0};		// Set up calls on comm
uint32 gc_riruc_canc[2]={0,0};		// Cancel up calls on comm

uint8 gc_rir=0;						// IRear nconspicious riser switch activation (last floor rear down call + 1)
uint32 gc_rirdc[2]={0,0};			// Down calls from comm
uint32 gc_rirdc_set[2]={0,0};		// Set down calls on comm
uint32 gc_rirdc_canc[2]={0,0};		// Cancel down calls on comm

uint8 gc_cb_sr=0;					// Code Blue second riser switch activation (last floor down call + 1)
uint8 gc_rcb_sr=0;			 	 	// Code Blue rear second riser switch activation (last floor rear down call + 1)
uint8 cb_dr=0;						// Use cb dual riser for std hall call if set
uint8 rcb_dr=0;						// Use rcb dual riser for std hall call if set

uint32 gc_cb[2]={0,0};				// Code Blue floors from comm
uint32 gc_cb_set[2]={0,0};			// Set code blue call
uint32 gc_cb_canc[2]={0,0};			// Cancel code blue call
uint32 gc_rcb[2]={0,0};				// Rear Code Blue floors from comm
uint32 gc_rcb_set[2]={0,0};			// Set rear code blue call
uint32 gc_rcb_canc[2]={0,0};		// Cancel rear code blue call

uint32 gc_cbsr[2]={0,0};			// Code Blue second riser floors from comm
uint32 gc_cbsr_set[2]={0,0};		// Set code blue second riser call   
uint32 gc_cbsr_canc[2]={0,0};		// Cancel code blue second riser call
uint32 gc_rcbsr[2]={0,0};			// Rear Code Blue second riser floors from comm
uint32 gc_rcbsr_set[2]={0,0};		// Set rear code blue second riser call   
uint32 gc_rcbsr_canc[2]={0,0};		// Cancel rear code blue second riser call

uint32 gc_xuc[2] = {0,0};	  		// Group cross assignment up call input
uint32 gc_xuct[2] = {0,0};			// Group cross assignment up call transfer flag
uint32 gc_xucl[2] = {0,0};			// Group cross assignment up call latch
uint32 gc_xuc_set[2] = {0,0};		// Group cross assignment up call set output
uint32 gc_xdc[2] = {0,0};		  	// Group cross assignment down call	input
uint32 gc_xdct[2] = {0,0};			// Group cross assignment down call transfer flag
uint32 gc_xdcl[2] = {0,0};			// Group cross assignment down call latch
uint32 gc_xdc_set[2] = {0,0};		// Group cross assignment down call set output

uint32 gc_xruc[2] = {0,0};	  		// Group cross assignment rear up call input
uint32 gc_xruct[2] = {0,0};			// Group cross assignment rear up call transfer flag
uint32 gc_xrucl[2] = {0,0};			// Group cross assignment rear up call latch
uint32 gc_xruc_set[2] = {0,0};		// Group cross assignment rear up call set output
uint32 gc_xrdc[2] = {0,0};		  	// Group cross assignment rear down call	input
uint32 gc_xrdct[2] = {0,0};			// Group cross assignment rear down call transfer flag
uint32 gc_xrdcl[2] = {0,0};			// Group cross assignment rear down call latch
uint32 gc_xrdc_set[2] = {0,0};		// Group cross assignment rear down call set output

uint32 gc_hcb_sec[2] = {0,0};		// hcb security inputs
uint32 gc_hcb_aux[2] = {0,0};		// hcb aux inputs
uint32 gc_hcb_rsec[2] = {0,0};		// hcb rear security inputs
uint32 gc_hcb_raux[2] = {0,0};		// hcb rear aux inputs

int32 tg_dcmsk[2] = {0,0};			// Temporary mask variables
int32 tg_ucmsk[2] = {0,0};
int32 tg_rdcmsk[2] = {0,0};
int32 tg_rucmsk[2] = {0,0};

uint32 gc_vip[2]={0,0};				// VIP floors from comm
uint32 gc_vip_set[2]={0,0};			// Set VIP call
uint32 gc_vip_canc[2]={0,0};		// Cancel VIP call
uint32 gc_rvip[2]={0,0};			// Rear VIP floors from comm
uint32 gc_rvip_set[2]={0,0};		// Set rear VIP call
uint32 gc_rvip_canc[2]={0,0};		// Cancel rear VIP call

uint32 gc_truc[2]={0,0};			// Third Riser up calls from comm
uint32 gc_truc_set[2]={0,0};		// Third Riser set up calls on comm
uint32 gc_truc_canc[2]={0,0};		// Third Riser cancel calls on comm
uint32 gc_trdc[2]={0,0};			// Third Riser down calls from comm
uint32 gc_trdc_set[2]={0,0};		// Third Riser set down calls on comm
uint32 gc_trdc_canc[2]={0,0};		// Third Riser cancel down calls on comm

uint32 g_trdcmsk[2] = {0,0};		// Bit mask for valid Third Riser up hall call floors
uint32 g_trucmsk[2] = {0,0};		// Bit mask for valid Third Riser dn hall call floors


// Dual riser mask
int32 pfdsr_msk[2]={0,0};		// Primary front dual serial riser mask
int32 sfdsr_msk[2]={0,0};		// Secondary front dual serial riser mask
int32 prdsr_msk[2]={0,0};		// Primary rear dual serial riser mask
int32 srdsr_msk[2]={0,0};		// Secondary rear dual serial riser mask



//Index used to transmit board io labels over comm

int16 pkt11_command = 0;//we may need to make this part of the queue
int16 io_brd_ix = 0;
int16 reqrd_labels_ix = 0; // index for sending dpy messages for io screen

/**************************************/
/****** Packet 11 data Structures *****/
/**************************************/

#define c_que_len 40

struct packet_11_struct
{
	int16 nmb;
	int16 cmd;	
};

typedef struct packet_11_struct pkt_type;

struct queue_packet_11 {
	pkt_type packet[c_que_len];		
	int16 used;
	int16 pix;
	int16 gix;
} pkt_11_que;

/**************************************/
/****** Packet 11 data Functions  *****/
/**************************************/

//lint -esym(534,put_11_que)
int16 put_11_que (int16 nmb, int16 cmd);
pkt_type get_11_que (void);
bool empty_11_que(void);

/**************************************/
/**************************************/
/**************************************/


int16 flt_tx_ix=0;
bool comm_loss=0;

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		
/*	
#if ((Traction == 0) && (Tract_OL == 0))
	int16 no_wheelchair_cap = 0;		// load high so car has no room for wheelchair	 (not used in hydro but must be declared)
#endif 
*/
// Group Comm security variables

uint8 gc_Add_Sec_Fl_Req = 0;			// Send security floor to add code
uint8 gc_Add_Sec_Code[4] = {0,0,0,0};	// Security Code to Add a code
uint8 gc_Del_Sec_Fl_Req = 0;			// Send Security floor to delete code
uint8 gc_Del_Sec_Code[4] = {0,0,0,0};	// Security code to delete a code
uint8 gc_Del_All_Codes_Fl_Req = 0;		// Delete all codes for one floor or all floors
uint8 gc_Sec_Fl_Tbl_Req[9] = {0,0,0,0,0,0,0,0,0};			// Floor request for security table
int16 gc_LookUpTable[NumMaxFl];
int16 gc_Sec_Req_Car = 0;							// Current Security Table Request Car
int16 gc_Sec_Req_Seq = 0;							// Security Request Sequence to send codes to car
int16 gc_Sec_Chksum_Fl = 0;						// Floor start for Security Checksum
int16 gc_Sec_Code_Index = 0;						// Code Index pointer
int16 gc_Code_Xfer_Comp = 0;						// Code table transfer complete

struct CdTable gc_CodeTable;
struct Cd_Tbl_Chk gc_Code_Tbl_Chk[fl_size];



// Group parameter checksum variables

uint8 gc_Var_Tbl_Req[13] = {0,0,0,0,0,0,0,0,0,0,0};			// Car request for group variable table
int16 gc_Var_Req_Seq = 0;							// group variable request sequence
int16 gc_Var_Req_Car = 0;							// group variable request car
uint16 t_svc_tmr_chksum[nmbsvct];
uint16 t_grp_fvars_chksum[Num_Fvars_Menus];
uint8 svc_tmr_chksum_upd = 0;



// Local Function Declaration

int16 put_pkt_req(int16 pkt_nmb, int16 addr, int16 com_nmb);
void put_pkt_req_all_cars(int16 pkt_nmb);
int16 get_pkt_req(int16 addr, int16 com_nmb);
int16 get_pkt_11_req (void);
int16 get_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb, int16 time);
void put_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb);
void put_pkt_11_req (int16 pkt_nmb);
void uncompress(int16 dev, int16 ix, int16 iodat);				
int16 put_11_que (int16 nmb, int16 cmd);
void zero_req_que(void); 
void zero_status_que(void);
void Req_Car_Voltage (void);
void Send_CC_Codes(void);
void Send_GRP_Vars(void);

void grp_com_int(void);
int16 receive(void);
void transmit(void);
void grp_com_tx_start(void);
void grp_dma_trm_int(void);
void grp_com_timer_int(void);
void verify_packet_in_buffer (void);
void local_car_to_group(void);
void local_group_to_car(void);
void Group_Com(void );
void pack_slave_grp_com(void);
//lint -esym(534,unpack_grp_com)
uint8 unpack_grp_com(void);
void pack_grp_com(uint16 Packet,uint16 to_addr);
void init_grp_com(void);
void init_grp_port (void);
void reinit_grp_com (void);
void init_grp_DMA(void);
void init_grp_com_variables(void);
int16 compress_zero(int16 buf_len, int16 max_len,int16 compress_flag);
int16 compress(uint8 *h_buf,int16 buf_len, int16 max_len);

void unpack_car_iodata(int16 dev, int16 ix);
int16 pack_car_iodata(int16 dev);
int16 pack_car_data_to_group(int16 dev);
void unpack_car_data_to_group(int16 dev, int16 ix);
int16 pack_group_data_to_car(int16 dev);
//lint -esym(534,uncompress_zero)
int16 uncompress_zero(int16 ix);			// return uncompressed length
void unpack_group_data_to_car(int16 dev, int16 ix, int16 bc);

uint8 c_rtc_sync = 0;	// Flag to sync car's time and date with dispatcher
						// c_rtc_sync = 0, Either car is dispatcher or it requests dispatcher (if online) for sending pkt 109
						// c_rtc_sync = 1, Car waiting for dispatcher's response to pkt 109 request
						// c_rtc_sync = 2, Car date and time successfully synced with dispatcher
uint8 c_rtc_sync_fail_cnt = 0;


void init_grp_com (void)
{

	_disable();

	init_grp_com_variables();

	init_grp_port();
	init_grp_DMA();

	PIT.TCTRL2.B.TEN = 0;		// Stop Timer
	INTC_InstallINTCInterruptHandler(grp_com_timer_int, 150, 2);  // Set interrupt vector for PIT2


	GRPport.CR1.B.RE = 1;					// Receiver Enable
  	EDMA.SERQR.R = GRPdmaChRx;				// Enable group coomm DMA channel
	GRPport.CR2.B.RXDMA = 1;				// Receive DMA Control: 0 = disabled, 1 = enabled
	
	_enable();		
	
}

void reinit_grp_com (void)
{
	_disable();

	gc_port = GRPCOM;
	gc_addr = cons[carnmb];
	gc_master = 0;
	gc_trm_case = 0;
	gc_rcv_case = 0;
	gc_rcv_com_status = 0;
	gc_rcv_pix = 0;
	gc_rcv_gix = 0;
	gc_trm_ix = 0;
	gc_trm_buf_empty = 1;
	gc_unpack_error = 0;

	init_grp_port();
	init_grp_DMA();
	
	gc_trm_buf_empty = 1;
	gc_trm_case = 0;

	PIT.TCTRL2.B.TEN = 0;		// Stop Timer
	INTC_InstallINTCInterruptHandler(grp_com_timer_int, 150, 2);  // Set interrupt vector for PIT2

	GRPport.CR1.B.RE = 1;					// Receiver Enable
  	EDMA.SERQR.R = GRPdmaChRx;				// Enable group coomm DMA channel
	GRPport.CR2.B.RXDMA = 1;				// Receive DMA Control: 0 = disabled, 1 = enabled

	_enable();		
}

void init_grp_port (void)
{		// Group comm
	//lint -esym(438,data_in)
	//lint -esym(550,data_in)
	uint16 data_in;
	
	GRPtxDis; 						// Turn off Transmit driver

	GRPport.CR1.B.TE = 0;			// Transmitter Disable
	GRPport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable

	// Control register 1	  
	GRPport.LCR.B.LIN = 0;			// Place in SCI mode	
	GRPport.CR1.B.SBR = 118;		// 57,600 BAUD
	
	GRPport.CR1.B.LOOPS = 0;		// Receiver source mode LOOPS:RSRC 00 = Dual wire mode
	GRPport.CR1.B.RSRC = 0;
	
	GRPport.CR1.B.M = 1;			// Frame Format Mode: set to 1 for nine bits
	GRPport.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
	GRPport.CR1.B.PE = 0;			// Parity Enable: 0 = disabled, 1 = enabled 
	GRPport.CR3.B.M2 = 0;
	GRPport.CR3.B.EROE = 1;			// Set error bit in receive data register 
	GRPport.CR3.B.ERFE = 1;			// Set error bit in receive data register
	GRPport.CR3.B.ERPE = 1;			// Set error bit in receive data register
		
	GRPport.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd

	GRPport.CR1.B.ILT = 0;			// Idle Line Type: 0 = idle line detection after low bit
	GRPport.CR1.B.TIE = 0;			// Transmit Interrupt Enable (data tranferred to shift register)
	GRPport.CR1.B.TCIE = 0;			// Transmit Complete Interrupt Enable (data shifted out of shift register)
	GRPport.CR1.B.RIE = 0;			// Receive Interrupt Enable
	GRPport.CR1.B.ILIE = 0;			// Idle line interrupt enable
	GRPport.CR1.B.RWU = 0;			// Receiver Wake Up mode: 0 = normal, 1 = wakeup on 9th bit address
	GRPport.CR1.B.SBK = 0;			// Send Break Character
	
	// Control Register 2
	GRPport.CR2.B.MDIS = 0;			// Module Disable
	GRPport.CR2.B.FBR = 0;			// Fast Bit Error Detect: 0 = normal, 1 = use in LIN mode	
	GRPport.CR2.B.BSTP = 0;			// DMA Stop on Bit Error: 0 = tx dma regardless of bit errors
	GRPport.CR2.B.IEBERR = 0;		// Bit Error Interrupt Enable
	GRPport.CR2.B.RXDMA = 0;		// Receive DMA Control: 0 = disabled, 1 = enabled
	GRPport.CR2.B.TXDMA = 0;		// Transmit DMA Control: 0 = disabled, 1 = enabled
	GRPport.CR2.B.BRK13 = 0;		// Break Character Length
	GRPport.CR2.B.TXDIR = 0;		// TXD pin output enable: Used in signel wire mode
	GRPport.CR2.B.BESM13 = 0;		// Fast Bit Error Detection Sample Mode: 0=RS9,1=RS10 LIN mode only
	GRPport.CR2.B.SBSTP = 0;		// Transmit Stop on Bit Error
	GRPport.CR2.B.RXPOL = 0;		// RXD Pin polarity: 0 = normal, 1 = inverted
	GRPport.CR2.B.PMSK = 0;			// Parity Bit Masking: 0 = in bit position, 1 = 0 in parity position	
	GRPport.CR2.B.ORIE = 0;			// Overrun Interrupt Enable
	GRPport.CR2.B.NFIE = 0;			// Noise Interrupt Enable
	GRPport.CR2.B.FEIE = 0;			// Frame Error Interrupt Enable
	GRPport.CR2.B.PFIE = 0;			// Parity Error Interrupt Enable
	
	
	// Enable must be last
	GRPport.CR1.B.TE = 1;			// Transmitter Enable
	GRPport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable

	data_in = GRPport.SDR.R;
	data_in = GRPport.SDR.R;

}

//*****************************************
// DMA Initialization for group Comm
//*****************************************

void Small_init_grp_DMA(void)
{	
 	// Initialize Receive Comm DMA

    EDMA.TCD[GRPdmaChRx].DADDR =  (uint32) &gc_rcv_dma_buf[0]; 	// Load address of destination
//    EDMA.TCD[GRPdmaChRx].DLAST_SGA = - (c_rcv_dma_buf_len * 2); // After major loop reset destination address    
    
  	EDMA.CEEIR.R = GRPdmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = GRPdmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = GRPdmaChRx;           	// DMA Clear error Register   			    
}

/* Mark Duckworth version of Nov 15, 2017
void Small_init_grp_DMA(void)
{	
 	// Initialize Receive Comm DMA

 	GRPport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable
	GRPport.CR2.B.RXDMA = 0;		// Receive DMA Control: 0 = disabled, 1 = enabled
	_disable();
   	EDMA.TCD[GRPdmaChRx].DADDR =  (uint32) &gc_rcv_dma_buf[0]; 	// Load address of destination 
  	EDMA.TCD[GRPdmaChRx].DSIZE = 1;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChRx].DOFF = 2;                         		// Increment destination addr by 2 
  	EDMA.TCD[GRPdmaChRx].NBYTES= 2;                    			// Transfer 1 byte per major loop 
 	EDMA.CEEIR.R = GRPdmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = GRPdmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = GRPdmaChRx;           	// DMA Clear error Register   			    
	GRPtxDis; 			// Turn off Transmit driver
	GRPport.CR1.B.RE = 1;					// Receiver Enable
  	EDMA.SERQR.R = GRPdmaChRx;				// Enable group coomm DMA channel
	GRPport.CR2.B.RXDMA = 1;				// Receive DMA Control: 0 = disabled, 1 = enabled
	_enable();
}
*/

void init_grp_DMA(void) {

	// Initialize Transmit Comm DMA
  
  	EDMA.CERQR.R = GRPdmaChTx;      	// Disable Group transmit EDMA channel   

	INTC_InstallINTCInterruptHandler(grp_dma_trm_int, GRPdmaTxIntLoc, 3);  

  	EDMA.CPR[GRPdmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[GRPdmaChTx].R = GRPdmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[GRPdmaChTx].SADDR = (uint32) &gc_trm_buf[0];  	// Load address of source data 
  	EDMA.TCD[GRPdmaChTx].SSIZE = 1;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChTx].SOFF = 2;                         		// After transfer, add 2 to src addr
  	
  	EDMA.TCD[GRPdmaChTx].SLAST = -20;                      		// After major loop, reset src addr 
  	EDMA.TCD[GRPdmaChTx].SMOD = 0;                         		// Source modulo feature not used 

  	EDMA.TCD[GRPdmaChTx].DADDR =  GRPdr; 						// Load address of destination 
  	EDMA.TCD[GRPdmaChTx].DSIZE = 1;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChTx].DOFF = 0;                         		// Do not increment destination addr 
  	EDMA.TCD[GRPdmaChTx].DLAST_SGA = 0;                    		// After major loop, no dest addr change 
  	EDMA.TCD[GRPdmaChTx].DMOD = 0;                         		// Destination modulo feature not used 
  
  	EDMA.TCD[GRPdmaChTx].NBYTES= 2;                    			// Transfer 1 byte per minor loop 
  	EDMA.TCD[GRPdmaChTx].BITER = 10;                       		// 1 major loop iterations 
  	EDMA.TCD[GRPdmaChTx].CITER = 10;                       		// Initialize current iteraction count 
  	EDMA.TCD[GRPdmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[GRPdmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[GRPdmaChTx].INT_MAJ = 1;
  	EDMA.TCD[GRPdmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[GRPdmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[GRPdmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[GRPdmaChTx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[GRPdmaChTx].E_SG = 0; 
  	EDMA.TCD[GRPdmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[GRPdmaChTx].DONE = 0;
  	EDMA.TCD[GRPdmaChTx].ACTIVE = 0;
  	EDMA.TCD[GRPdmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = GRPdmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = GRPdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = GRPdmaChTx;           	// DMA Clear error Register 
  
  	
  	// Initialize Receive Comm DMA

 	EDMA.CERQR.R = GRPdmaChRx;      		// Disable group EDMA channel   
  
  	EDMA.CPR[GRPdmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[GRPdmaChRx].R = GRPdmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[GRPdmaChRx].SADDR = GRPdr;  						// Load address of source data 
  	EDMA.TCD[GRPdmaChRx].SSIZE = 1;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChRx].SOFF = 0;                         		// Do not increment source address
  	
  	EDMA.TCD[GRPdmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
  	EDMA.TCD[GRPdmaChRx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[GRPdmaChRx].DADDR =  (uint32) &gc_rcv_dma_buf[0]; 	// Load address of destination 
  	EDMA.TCD[GRPdmaChRx].DSIZE = 1;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChRx].DOFF = 2;                         		// Increment destination addr by 2 
  	EDMA.TCD[GRPdmaChRx].DLAST_SGA = - (c_rcv_dma_buf_len * 2); // After major loop reset destination address
  	EDMA.TCD[GRPdmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
  
  	EDMA.TCD[GRPdmaChRx].NBYTES= 2;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[GRPdmaChRx].BITER = c_rcv_dma_buf_len;             // Major loop iterations 
  	EDMA.TCD[GRPdmaChRx].CITER = c_rcv_dma_buf_len;             // Initialize current iteraction count 
  	EDMA.TCD[GRPdmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
  	EDMA.TCD[GRPdmaChRx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[GRPdmaChRx].INT_MAJ = 0;
  	EDMA.TCD[GRPdmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[GRPdmaChRx].BITERE_LINK = 0;
  	EDMA.TCD[GRPdmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[GRPdmaChRx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[GRPdmaChRx].E_SG = 0; 
  	EDMA.TCD[GRPdmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[GRPdmaChRx].DONE = 0;
  	EDMA.TCD[GRPdmaChRx].ACTIVE = 0;
  	EDMA.TCD[GRPdmaChRx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = GRPdmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = GRPdmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = GRPdmaChRx;           	// DMA Clear error Register 
  
}

//****************************************
// Initialize group comm variables
//****************************************

void init_grp_com_variables(void)
{
	int16 i,el;				



	for (i=0;i<car_size;i++)
	{
		grp_Car_Stat[i].g_cc[0] = 0;
		grp_Car_Stat[i].g_cc[1] = 0;
		grp_Car_Stat[i].g_rcc[0] = 0;
		grp_Car_Stat[i].g_rcc[1] = 0;
/*
		g_cc[i][1] = 0;
		g_cc[i][1] = 0;
		g_rcc[i][0] = 0;
		g_rcc[i][1] = 0;
*/
	}

	gc_port = GRPCOM;
	
	// note that if this car is group, addr will be changed to the car the
	// group is talking to.
	gc_addr = cons[carnmb];
	gc_master = 0;
	gc_trm_case = 0;
	gc_rcv_case = 0;
	gc_rcv_com_status = 0;
	gc_rcv_pix = 0;
	gc_rcv_gix = 0;
	gc_trm_ix = 0;
	gc_trm_buf_empty = 1;
	gc_unpack_error = 0;
	for(i=0;i<c_com_no_dev;i++)
	{
		gc_dev_list[i] = 0;
		gc_dev_ver[i] = 0;
		gc_dev_rev[i] = 0;
		gc_dev_online[i] = 0;
		gc_dev_tx_cnt[i] = 0;
		gc_dev_rx_cnt[i] = 0;
		gc_loss_cnt[i] = 0;
	}
	for(el=0;el<=cons[nmbcars];el++)
	{
		full_update_req[el] = 1;
		timers[tgcom_car + el] = 0;
		for(i=0;i<c_buf_size;i++)
			unpack_buf[el-1][i] = 0;
	}
	timers[tgc_tran] = 0;
	timers[tgc_recv] = 0;
	timers[tcom_response] = 0;	

	for(i=0;i<c_buf_size;i++)
	{
		pack_buf[i] = 0;
		hold_buf[i] = 0;
	}
	pack_grp_com(1,cons[carnmb]);
	gc_num_dev = 0;
	gc_num_hc_dev = 0;
	gc_num_car_dev = 0;
	for (i=1;i <= cons[nmbcars];i++)
		gc_dev_list[gc_num_dev++] = i;
		
	gc_dev_list[gc_num_dev++] = c_MR_DPY_addr;
	gc_num_car_dev = gc_num_dev;
	gc_ix = 0;

	if (gc_num_dev != 0)
	{
		gc_num_hc_dev = gc_num_dev - gc_num_car_dev;
		if (gc_num_hc_dev < 0)
			gc_num_hc_dev = 0;
	}

//	if (cons[naec] == 1)
//	{
//		slave_max_time_out = 12;
//		response_wait_time = 6;
//	}
//	else 
	{
		slave_max_time_out = c_slave_max_time_out;
		if (gc_num_dev <= 2)
		{
			response_wait_time = min_response_wait[0];
		}
		else if (gc_num_dev <= 4)
		{
			response_wait_time = min_response_wait[1];
		}
		else 
			response_wait_time = min_response_wait[2];
	}
}


//**************************************************
// This is start the comm transmission on a COM Port
//**************************************************

void grp_com_tx_start(void)
{
	int16 len;
	int32 slen;
	
	gc_trm_case = 0;
	gc_trm_buf_empty = 0;

	len = (int16)(gc_trm_buf[3] + 2);
	slen = len * 2;

	_disable();	   
	// Turn off the receiver
	GRPport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable
	
	// Enable transmitter
	EDMA.CER.R = GRPdmaChTx;           	// DMA Clear error Register 
	GRPtxEn; 			// Turn on Transmit driver
 	GRPport.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
    EDMA.TCD[GRPdmaChTx].SADDR = (uint32) &gc_trm_buf[0];  	// Load address of source data 
  	EDMA.TCD[GRPdmaChTx].SLAST = (0-slen);        // After major loop, reset src addr 
	EDMA.TCD[GRPdmaChTx].NBYTES = 2;	// Transfer len number byte per minor loop 
 	EDMA.TCD[GRPdmaChTx].BITER = len;   // 10 major loop iterations 
  	EDMA.TCD[GRPdmaChTx].CITER = len;   // Initialize current iteraction count 
  	EDMA.SERQR.R = GRPdmaChTx;      	// Enable Group EDMA channel   
	GRPport.CR2.B.TXDMA = 1;			// Transmit DMA Control: 0 = disabled, 1 = enabled
 //   sprintf(src,"%d %d", gc_trm_buf[0], gc_trm_buf[1]);	
 //   debug_msg(src, 2);	
	_enable();
}

//*********************************************
// Group DMA transmit finished interrupt
//*********************************************

void grp_dma_trm_int(void)
{
	gc_trm_int = 1;
	
  	EDMA.CIRQR.R = GRPdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = GRPdmaChTx;      	// Disable Drive EDMA channel   
	GRPport.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled

	// Transmit end timer - PIT 2
	PIT.TCTRL2.B.TEN = 0;		// Reset Timer
	if ((GRPport.SR.B.TC == 1) && (GRPport.SR.B.TACT == 0))
		PIT.LDVAL2.R = 10875 - 1;		// setup timer 1 for (100 usec) * incremental amount with 108.75 MHz fsys
	else
		PIT.LDVAL2.R = (10875 * 4) - 1;		// setup timer 1 for (400 usec) * incremental amount with 108.75 MHz fsys
	
	PIT.TCTRL2.B.TEN = 1;		// Start Timer
	PIT.TFLG2.B.TIF = 1;			// Clear interrupt flag so an interrupt does not occur right away
	PIT.TCTRL2.B.TIE = 1;		// Enable interrupts
}

//*********************************************
// Group Transmit Finished Timer Interrupt
//*********************************************
void grp_com_timer_int(void)
{
	static int16 tmr_count;
	
	PIT.TFLG2.B.TIF = 1;		// Clear interrupt flag 
	PIT.TCTRL2.B.TIE = 0;		// Disable interrupts
	PIT.TCTRL2.B.TEN = 0;		// Stop Timer
		
	tmr_count = 1000;
	while ((tmr_count > 0) && (GRPport.SR.B.TC == 0))
	{
		tmr_count--;
	}
	GRPtxDis; 				// Turn off Transmit driver
	gc_trm_buf_empty = 1;
	gc_trm_case = 0;
	gc_dev_tx_cnt[gc_addr]++;
	// Turn on the receiver
	if (gc_no_response == 0)
		GRPport.CR1.B.RE = 1;			// Receiver Enable: 1 = Enable, 0 = Disable

}

// ******************************************************
//		Verify Packet Data in buffer
//*******************************************************

void verify_packet_in_buffer (void)
{
	static int16 ite_count, beg_count, dma_count, status;

	beg_count = (int16)EDMA.TCD[GRPdmaChRx].BITER;
	ite_count = (int16)EDMA.TCD[GRPdmaChRx].CITER;
	dma_count = (int16)(beg_count - ite_count);
	
	status = e_ok;
	
	while ((gc_rcv_dma_gix != dma_count) && (status == e_ok) && (gc_rcv_case != 6))
	{
		status = receive();

		gc_rcv_dma_gix++;
		if (gc_rcv_dma_gix >= c_rcv_dma_buf_len)
			gc_rcv_dma_gix = 0;
	}
	if ((gc_unpack == 1) && (gc_rcv_case == 6))
		gc_rcv_case = 0;

}

// ******************************************************
//		Empty data from receive fifo
//*******************************************************

int16 receive (void)
{

	int16 exception;

	exception = e_ok;
	
			    sprintf(src,"receive"); 
                Add_msg_to_array_B(src, 1);	
	
	if (((gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x2000) != 0) && (gc_rcv_case > 0))
	{
		exception = e_FE_OE;
	}
	else
	{
		 
		switch (gc_rcv_case)
		{
			case 0:						// Get Start flag
				if ( (int16)(gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x20FF) == (int16)(':'))   		// Enable receive interrupt
				{
					if ((gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x8000) != 0)
					{		  // Must have the nineth bit set for the start flag (shows up in the RN flag)
						gc_rcv_int = 1;
						gc_rcv_case = 1;
						gc_rcv_len = 0;
						gc_rcv_act_len = 1;
						gc_rcv_ck_sum = 0;
						gc_rcv_gix = 0;
						gc_rcv_pix = 0;
						gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
						gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
						gc_rcv_pix++;
						gc_valid_start = 1;
					}
					else
					{
						gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
						gc_rcv_case = 0;
					}
				}
				else
				{
					gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
					gc_rcv_case = 0;
				}
				
				break;

			case 1:					// Get address
				if ((int16)(gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x20FF) == gc_addr)
				{					// Valid address
					gc_valid_addr = 1;
					if (((gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x8000) != 0) || (gc_addr > 8))
					{		  // Must have the nineth bit set for the ADDRESS (shows up in the RN flag)
						gc_rcv_int = 1;
						gc_rcv_case = 2;		
						gc_rcv_len = 0;
						gc_rcv_act_len++;
						gc_rcv_ck_sum += (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
						gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
						gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
						gc_rcv_pix++;
					}
					else
					{
						gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
						exception = e_inv_address;
					}
				}
				else
				{
					gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
					exception = 0;		 // No exception since this is a normal exit (not my address)
				}
				break;				

			case 2:						// get Packet number
				gc_rcv_int = 1;
				gc_rcv_ck_sum += (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
				gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
				gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
				gc_rcv_pix++;
				gc_rcv_act_len++;
				gc_rcv_case = 3;
				break;

			case 3:     				// get comm length
				gc_rcv_int = 1;
				gc_rcv_len = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
				gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
				gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
				gc_rcv_ck_sum += gc_rcv_len;
				gc_rcv_pix++;
				if ((gc_rcv_pix + gc_rcv_len) >= c_rcv_buf_size)
				{ 		// Unpack is not working so ingnore packet
					exception = e_inv_len;
				}
				else
				{
					gc_rcv_act_len++;
					gc_rcv_case = 4;
				}
				break;

			case 4:					// receive data
				gc_rcv_int = 1;
				if (gc_rcv_act_len >= gc_rcv_len)
				{  			// finished receiving packet
					gc_rcv_ck_sum = (0xFF - gc_rcv_ck_sum) + 1;
					if (gc_rcv_ck_sum == (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix])
					{
						gc_rcv_case = 5;
						gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
						gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
						gc_rcv_pix++;
					}
					else
					{	
						exception = e_inv_ck_sum;
					}
				}
				else
				{           	// Get packet data
					gc_rcv_ck_sum += (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
					gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
					gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
					gc_rcv_act_len ++;
					gc_rcv_pix++;
					if (gc_rcv_pix >= c_rcv_buf_size)
						gc_rcv_case = 0;					// error
				}
				break;

			case 5:					// receive end
				gc_rcv_int = 1;
				if ((uint8)(gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x00FF) == (uint8)('@'))
				{		// valid end character
					gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
					gc_rcv_dma_buf[gc_rcv_dma_gix] = 0;
					gc_rcv_pix++;
					gc_rcv_buf[gc_rcv_pix] = gc_rcv_act_len;	// store actual receive length
					gc_rcv_pix++;
					gc_rcv_buf[gc_rcv_pix] = gc_rcv_ck_sum;
					gc_unpack = 1;			// Packet has has been received (master unpacks right away)
					gc_rcv_case = 6;
				}
				else
				{
					exception = e_inv_pkt_end;
				}
				break;				
			
			case 6:			// finished
				gc_rcv_case = 6;
				break;

			default:    // Trun on receiver and line status interrupts
				exception = e_inv_case;
				break;
		}
	}

	
	if (exception != e_ok) 
	{
		gc_rcv_case = 0;
		gc_rcv_len = 0;
		gc_rcv_gix = 0;
		gc_rcv_pix = 0;
		gc_rcv_act_len = 0;
		gc_rcv_com_status |= exception;
	}
	return(exception);
}


//*********************************************
// Transfer comm data from local car to group
//*********************************************

void local_car_to_group(void)
{
	int16 emp_bits;
	int16 i;
	int16 fl_ix;
	
#if (Traction == 1)
	float tempd;
#endif

	grp_Car_Stat[cons[carnmb]].g_pos = position;
	grp_Car_Stat[cons[carnmb]].g_procf = procf;
	grp_Car_Stat[cons[carnmb]].g_servf = servf;
	grp_Car_Stat[cons[carnmb]].g_nxt_stp = next_stop;

	emp_bits = 0;
	if (rdinp(i_EPS) == 1)
		emp_bits |= 0x08;
	else
		emp_bits &= ~0x08;

	if (rdinp(i_EMP) == 1)
		emp_bits |= 0x10;
	else
		emp_bits &= ~0x10;

	if ((rdinp(i_EPT) == 1) && ((cons[EMPop] & 0x01) != 0))
		emp_bits |= 0x20;
	else
		emp_bits &= ~0x20;


	grp_Car_Stat[cons[carnmb]].g_empf = (int16)((empf & 0x07) | emp_bits);		// clear ept input to send to group

	grp_Car_Stat[cons[carnmb]].g_firef = firef;
	grp_Car_Stat[cons[carnmb]].g_doorf = doorf;
	grp_Car_Stat[cons[carnmb]].g_rdoorf = rdoorf;
	grp_Car_Stat[cons[carnmb]].g_dirf = dirf;
	grp_Car_Stat[cons[carnmb]].g_dpref = dpref;
	grp_Car_Stat[cons[carnmb]].g_codebf = codebf;
	grp_Car_Stat[cons[carnmb]].g_dcalls = dcalls;
  	grp_Car_Stat[cons[carnmb]].g_cur_flt = Current_Fault;
  	grp_Car_Stat[cons[carnmb]].g_statusf1 = statusf;
	grp_Car_Stat[cons[carnmb]].g_control = (int16)((up_peak_trigger & 0x1) | (((onward_calls != 0) & 0x1) << 1) |
											(((no_wheelchair_cap  != 0) & 0x01) << 2));	  // Set control bits (15 bits unused)
	grp_Car_Stat[cons[carnmb]].g_ss_stat = safety_string_status;
	grp_Car_Stat[cons[carnmb]].g_vipf = vipf;
	grp_Car_Stat[cons[carnmb]].g_nmbcc = nmb_cc;
	grp_Car_Stat[cons[carnmb]].g_topcc = top_cc;
	grp_Car_Stat[cons[carnmb]].g_botcc = bot_cc;
	grp_Car_Stat[cons[carnmb]].g_fire_status = fire_status;
	grp_Car_Stat[cons[carnmb]].g_ins_status = ins_status;
  	grp_Car_Stat[cons[carnmb]].g_statusf2 = statusf2;
  	grp_Car_Stat[cons[carnmb]].g_statusf3 = statusf3;
  	grp_Car_Stat[cons[carnmb]].g_statusf4 = statusf4;
	grp_Car_Stat[cons[carnmb]].g_pwr_status = power_status;
	
	
	grp_Car_Stat[cons[carnmb]].g_cancel_up = cancel_up;
	grp_Car_Stat[cons[carnmb]].g_cancel_dn = cancel_dn;
	grp_Car_Stat[cons[carnmb]].g_cancel_upr = cancel_upr;
	grp_Car_Stat[cons[carnmb]].g_cancel_dnr = cancel_dnr;
	
	grp_Car_Stat[cons[carnmb]].g_cancel_irup = cancel_irup;
	grp_Car_Stat[cons[carnmb]].g_cancel_irdn = cancel_irdn;
	grp_Car_Stat[cons[carnmb]].g_cancel_rirup = cancel_rirup;
	grp_Car_Stat[cons[carnmb]].g_cancel_rirdn = cancel_rirdn;
	
	grp_Car_Stat[cons[carnmb]].g_cancel_trup = cancel_trup;
	grp_Car_Stat[cons[carnmb]].g_cancel_trdn = cancel_trdn;
	grp_Car_Stat[cons[carnmb]].g_cancel_rtrup = cancel_rtrup;
	grp_Car_Stat[cons[carnmb]].g_cancel_rtrdn = cancel_rtrdn;
	

	if (f.Flts[Flt_ix].code != 0)
		car_has_fault = true;
	else
		car_has_fault = false;		// no fault
		
	grp_Car_Stat[cons[carnmb]].g_car_has_fault = car_has_fault;

#if (Traction == 1)
  	grp_Car_Stat[cons[carnmb]].g_enc_dir = Enc_dir;
	tempd = (float)((Dmd_Velocity * 5.0)/Pulses_per_Inch);	// has to be two steps due to conversion error
  	grp_Car_Stat[cons[carnmb]].g_dmd_vel = (int16)(tempd + 0.5);
	tempd = (float)((Enc_vel * 5.0)/Pulses_per_Inch);
  	grp_Car_Stat[cons[carnmb]].g_enc_vel = (int16)(tempd + 0.5);
  	grp_Car_Stat[cons[carnmb]].g_flr_cnt = DPP_Floor_Pos[position];
  	grp_Car_Stat[cons[carnmb]].g_pls_cnt = DPP_Count;
#else
  	grp_Car_Stat[cons[carnmb]].g_enc_dir = 0;
  	grp_Car_Stat[cons[carnmb]].g_dmd_vel = 0;
  	grp_Car_Stat[cons[carnmb]].g_enc_vel = 0;
  	grp_Car_Stat[cons[carnmb]].g_flr_cnt = 0;
  	grp_Car_Stat[cons[carnmb]].g_pls_cnt = 0;
#endif

	grp_Car_Stat[cons[carnmb]].g_L1_Voltage = L1_Voltage;
	grp_Car_Stat[cons[carnmb]].g_L2_Voltage = L2_Voltage;
	grp_Car_Stat[cons[carnmb]].g_L3_Voltage = L3_Voltage;
	grp_Car_Stat[cons[carnmb]].g_Door_Voltage = Door_Voltage;
	grp_Car_Stat[cons[carnmb]].g_car_alrm = car_alrm;			


	grp_Car_Stat[cons[carnmb]].g_run_statusf = run_statusf;
	grp_Car_Stat[cons[carnmb]].g_ins_servf = ins_servf;
	grp_Car_Stat[cons[carnmb]].g_non_simult_doorf = (uint8)fvars[fvno_simul_do];

	grp_Car_Stat[cons[carnmb]].g_ccpbs_fl_access_req = ccpbs_fl_access_req;

	for (i=0;i<4;i++)
	{
		grp_Car_Stat[cons[carnmb]].g_ccpbs_codes[i] = ccpbs_codes[i];
	}

	grp_Car_Stat[cons[carnmb]].g_dwellt_reset = (uint8)car_Grp_Stat.c_grp_rst_doort;
	grp_Car_Stat[cons[carnmb]].g_rdwellt_reset = (uint8)car_Grp_Stat.c_grp_rst_rdoort;
	
	// Get Car Call Data	
	for (i=cons[bottomf];i<=cons[topf];i++)
	{
		fl_ix = (i-1)/32;
		if (carcb[i] == 1)
			grp_Car_Stat[cons[carnmb]].g_cc[fl_ix] |= fl_mask[i];
		else
			grp_Car_Stat[cons[carnmb]].g_cc[fl_ix] &= ~fl_mask[i];
		
		if (rcarcb[i] == 1)
			grp_Car_Stat[cons[carnmb]].g_rcc[fl_ix] |= fl_mask[i];
		else
			grp_Car_Stat[cons[carnmb]].g_rcc[fl_ix] &= ~fl_mask[i];
	}
}


//*********************************************
// Transfer comm data from group to local car
//*********************************************

void local_group_to_car(void)
{

	c_up_hall_call[0] = g_up_hall_call[0][cons[carnmb]];		
	c_dn_hall_call[0] = g_dn_hall_call[0][cons[carnmb]];		
	c_upr_hall_call[0] = g_upr_hall_call[0][cons[carnmb]];		
	c_dnr_hall_call[0] = g_dnr_hall_call[0][cons[carnmb]];		

	c_irup_hall_call[0] = g_irup_hall_call[0][cons[carnmb]];		
	c_irdn_hall_call[0] = g_irdn_hall_call[0][cons[carnmb]];		
	c_rirup_hall_call[0] = g_rirup_hall_call[0][cons[carnmb]];		
	c_rirdn_hall_call[0] = g_rirdn_hall_call[0][cons[carnmb]];		

	if (cons[grtopf] > 32)
	{
		c_up_hall_call[1] = g_up_hall_call[1][cons[carnmb]];		
		c_dn_hall_call[1] = g_dn_hall_call[1][cons[carnmb]];		
		c_upr_hall_call[1] = g_upr_hall_call[1][cons[carnmb]];		
		c_dnr_hall_call[1] = g_dnr_hall_call[1][cons[carnmb]];		

		c_irup_hall_call[1] = g_irup_hall_call[1][cons[carnmb]];		
		c_irdn_hall_call[1] = g_irdn_hall_call[1][cons[carnmb]];		
		c_rirup_hall_call[1] = g_rirup_hall_call[1][cons[carnmb]];		
		c_rirdn_hall_call[1] = g_rirdn_hall_call[1][cons[carnmb]];		
	}


	c_cc_req[0] = g_cc_req[0][cons[carnmb]];

	if (grtop_rfl != 0)
	{
		c_rcc_req[0] = g_rcc_req[0][cons[carnmb]];
	}

	if (cons[grtopf] > 32)
	{
		c_cc_req[1] = g_cc_req[1][cons[carnmb]];

		if (grtop_rfl != 0)
		{
			c_rcc_req[1] = g_rcc_req[1][cons[carnmb]];
		}
	}

	car_Grp_Stat.c_park_fl = g_park[cons[carnmb]];
	car_Grp_Stat.c_grp_car = cons[carnmb];
	car_Grp_Stat.c_alt_lobby = alt_lobby;
	car_Grp_Stat.c_lby_req_fl = lby_req[cons[carnmb]];
	
	 // Note: leave a bit for rear next up
	car_Grp_Stat.c_next_up = next_up;
	car_Grp_Stat.c_ncu_door = ncu_door[cons[carnmb]]; 
	car_Grp_Stat.c_ncu_que = ncu_que[cons[carnmb]];
	car_Grp_Stat.c_local_up_peak = up_peak;

	car_Grp_Stat.c_cb_fl = g_cb_fl[cons[carnmb]];
	car_Grp_Stat.c_rcb_fl = g_rcb_fl[cons[carnmb]];
	car_Grp_Stat.c_grp_codebf = grp_codebf[cons[carnmb]];

	car_Grp_Stat.c_vip_fl = g_vip_fl[cons[carnmb]];
	car_Grp_Stat.c_rvip_fl = g_rvip_fl[cons[carnmb]] & 0x7f;

	car_Grp_Stat.c_gempf = grp_empf[cons[carnmb]];

	car_Grp_Stat.c_ir_active = ir_active;
	car_Grp_Stat.c_in_grp_sabbath = in_grp_sabbath;
	
	car_Grp_Stat.c_ids_ind = ids_ind[cons[carnmb]];
	car_Grp_Stat.c_ids_rtl = ids_rtl[cons[carnmb]] ;
	car_Grp_Stat.c_ids_heof = ids_heof[cons[carnmb]];
	car_Grp_Stat.c_ids_ad_off = ids_ad_off[cons[carnmb]];
	car_Grp_Stat.c_ids_allw_door = ids_allw_door[cons[carnmb]];
	car_Grp_Stat.c_ids_return_typ = ids_return_typ;
	car_Grp_Stat.c_ids_return_fl  = ids_return_flr;

	car_Grp_Stat.c_ui_ind = ui_ind[cons[carnmb]];
	car_Grp_Stat.c_ui_rtl = ui_rtl[cons[carnmb]];
	car_Grp_Stat.c_ui_heof = ui_heof[cons[carnmb]];
	car_Grp_Stat.c_ui_ad_off = ui_ad_off[cons[carnmb]];

	car_Grp_Stat.c_asvto_pos = asvto_pos[cons[carnmb]];	 			// Auto service time-out position
	car_Grp_Stat.c_car_asgn_sec = car_asgn_sec;				// security flag from group
	car_Grp_Stat.c_sec_fl_ctl = sec_fl_ctrl[cons[carnmb]];
	car_Grp_Stat.c_sr_cc_ctrl = sr_cc_ctrl[cons[carnmb]];

	car_Grp_Stat.c_any_hall_call = any_hall_call;
	car_Grp_Stat.c_any_ir_hall_call = any_ir_hall_call;
	car_Grp_Stat.c_ir_car = ir_car;

	c_tmr_svc[s_cc_lockouts] = tmr_svc[s_cc_lockouts];
	c_tmr_svc[s_ccl_override] = tmr_svc[s_ccl_override];
	c_tmr_svc[s_sabbath] = tmr_svc[s_sabbath];
	c_tmr_svc[s_cc_pb_sec] = tmr_svc[s_cc_pb_sec];
	c_tmr_svc[s_gccl_override] = tmr_svc[s_gccl_override];
	c_tmr_svc[s_gcc_lockouts] = tmr_svc[s_gcc_lockouts];
	c_tmr_svc[s_alt_fl_sec] = tmr_svc[s_alt_fl_sec];

	c_tmr_svc_en[s_cc_lockouts] = tmr_svc_en[s_cc_lockouts];
	c_tmr_svc_en[s_ccl_override] = tmr_svc_en[s_ccl_override];
	c_tmr_svc_en[s_sabbath] = tmr_svc_en[s_sabbath];
	c_tmr_svc_en[s_cc_pb_sec] = tmr_svc_en[s_cc_pb_sec];
	c_tmr_svc_en[s_gccl_override] = tmr_svc_en[s_gccl_override];
	c_tmr_svc_en[s_gcc_lockouts] = tmr_svc_en[s_gcc_lockouts];
	c_tmr_svc_en[s_alt_fl_sec] = tmr_svc_en[s_alt_fl_sec];

	c_tmr_svc_logic[s_cc_lockouts] = tmr_svc_logic[s_cc_lockouts];
	c_tmr_svc_logic[s_ccl_override] = tmr_svc_logic[s_ccl_override];
	c_tmr_svc_logic[s_sabbath] = tmr_svc_logic[s_sabbath];
	c_tmr_svc_logic[s_cc_pb_sec] = tmr_svc_logic[s_cc_pb_sec];
	c_tmr_svc_logic[s_gccl_override] = tmr_svc_logic[s_gccl_override];
	c_tmr_svc_logic[s_gcc_lockouts] = tmr_svc_logic[s_gcc_lockouts];
	c_tmr_svc_logic[s_alt_fl_sec] = tmr_svc_logic[s_alt_fl_sec];

	car_Grp_Stat.c_grp_fire_ctrl = g_fire_ctrl;
	car_Grp_Stat.c_car_hc_fire_door_open = grp_hc_fire_door_open;
	car_Grp_Stat.c_fvfiresw_loc = fvars[fvfiresw];
	car_Grp_Stat.c_ids_fsx = g_ids_fsx;

	lby_req_fl = car_Grp_Stat.c_lby_req_fl;
	park_fl = car_Grp_Stat.c_park_fl;	


	if (cons[Dest_Disp] == 1)
	{
		c_cc_dpy[0] = g_cc_dpy[0][cons[carnmb]];

		if (grtop_rfl != 0)
		{
			c_rcc_dpy[0] = g_rcc_dpy[0][cons[carnmb]];
		}

		if (cons[grtopf] > 32)
		{
			c_cc_dpy[1] = g_cc_dpy[1][cons[carnmb]];

			if (grtop_rfl != 0)
			{
				c_rcc_dpy[1] = g_rcc_dpy[1][cons[carnmb]];
			}
		}
	}
}

//********************************
// group comunications procedure
//********************************
void Group_Com()
{
	int16 i,j,Packet,comm_time_out;
//	int16 dev_ix;

	static int16 grp_Car_Statm_flt_tbl[9] = {f_Car1_Comm,f_Car1_Comm,f_Car2_Comm,f_Car3_Comm,
									f_Car4_Comm,f_Car5_Comm,f_Car6_Comm,f_Car7_Comm,f_Car8_Comm};

/*
	if (cons[nmbcars] == 1)
		Dispatcher = 1;
	else if (cons[nmbcars] >= 2)
	{
		if (cons[carnmb] == 1)
			Dispatcher = 1;
		else if (cons[carnmb] == 2)
		{
			if (timers[tgcom_car1] > 30)
			{
#if ((Traction == 1) || (Tract_OL == 1))
				if ((cons[Ser_IGEP] == 1) && (igep_com_online == 1))
					Dispatcher = 2;		// Car 2 can be dispatcher since IGEP master is on line
				else if ((Drive_Type[cons[carnmb]] != 1) && (Drive_Type[cons[carnmb]] != 9) && (rdinp(i_EMP) == 1))
#else
				if ((Drive_Type[cons[carnmb]] != 1) && (Drive_Type[cons[carnmb]] != 9) && (rdinp(i_EMP) == 1))
#endif
					Dispatcher = 1;		// Car 2 cannot be dispatcher on emergency power and not a hydro
				else
				{
					if (Dispatcher != 2)
						comm_loss = 1;
					Dispatcher = 2;
				}
			}
			else
				Dispatcher = 1;
		}
		else
		{ 					   // all other cars
			if (timers[tgcom_car1] < 30)
				Dispatcher = 1;		// comm to car 1
			else if (timers[tgcom_car2] < 30)
				Dispatcher = 2;			// Comm to car 2
			else if ((fvars[fvcmaster] >= cons[carnmb]) && (fvars[fvcmaster] <= cons[carnmb]))
			{
				if ((Drive_Type[cons[carnmb]] != 1) && (Drive_Type[cons[carnmb]] != 9) && (rdinp(i_EMP) == 1))
					Dispatcher = 1;		// Cars above 2 cannot be dispatcher on emergency power and not a hydro
				else
	 				Dispatcher = cons[carnmb];			// Select another car to be the dispatcher other than 1 or 2
			}
			else
				Dispatcher = 1;			// Default to car 1
		}
	}
*/	
	
	local_car_to_group();		// run this even if the car is not the dispatcher

	if (cons[carnmb] == Dispatcher)
	{
		gc_dev_ver[cons[carnmb]] = version;
		gc_dev_rev[cons[carnmb]] = revision;
		gc_dev_online[cons[carnmb]] = 1;
		
		local_group_to_car();

	}
	else
	{
		gc_dev_ver[cons[carnmb]] = 0;
		gc_dev_rev[cons[carnmb]] = 0;
		gc_dev_online[cons[carnmb]] = 0;
	}
	
	if (cons[loc_svc_tmr] != 0)
	{
		c_tmr_svc[s_cc_lockouts] = tmr_svc[s_cc_lockouts];
		c_tmr_svc[s_ccl_override] = tmr_svc[s_ccl_override];
		c_tmr_svc[s_sabbath] = tmr_svc[s_sabbath];
		c_tmr_svc[s_cc_pb_sec] = tmr_svc[s_cc_pb_sec];
		
		c_tmr_svc_en[s_cc_lockouts] = tmr_svc_en[s_cc_lockouts];
		c_tmr_svc_en[s_ccl_override] = tmr_svc_en[s_ccl_override];
		c_tmr_svc_en[s_sabbath] = tmr_svc_en[s_sabbath];
		c_tmr_svc_en[s_cc_pb_sec] = tmr_svc_en[s_cc_pb_sec];
		
		c_tmr_svc_logic[s_cc_lockouts] = tmr_svc_logic[s_cc_lockouts];
		c_tmr_svc_logic[s_ccl_override] = tmr_svc_logic[s_ccl_override];
		c_tmr_svc_logic[s_sabbath] = tmr_svc_logic[s_sabbath];
		c_tmr_svc_logic[s_cc_pb_sec] = tmr_svc_logic[s_cc_pb_sec];
	}

	if (comm_loss == 1)
	{		  // clear variables from the group on comm loss
		comm_loss = 0;
		park_fl = 0;
		lby_req_fl = 0;
		car_Grp_Stat.c_park_fl = 0;
		car_Grp_Stat.c_gempf = 0;
		car_Grp_Stat.c_lby_req_fl = 0;
		car_Grp_Stat.c_ncu_door = 0; 
		car_Grp_Stat.c_ncu_que = 0;
		car_Grp_Stat.c_cb_fl = 0;
		car_Grp_Stat.c_rcb_fl = 0;
		car_Grp_Stat.c_grp_codebf = 0;
		car_Grp_Stat.c_asvto_pos = 0;
		car_Grp_Stat.c_car_asgn_sec = 0;
		car_Grp_Stat.c_alt_lobby = 0;
		car_Grp_Stat.c_local_up_peak = 0;
	}
	
	if (gc_trm_buf_empty == 1)
		verify_packet_in_buffer();	// not transmitting so check receive buffer


	if ((cons[carnmb] == Dispatcher) && (gc_master == 1))
	{  			// This car is the group

 		if (gc_trm_buf_empty == 0)
 			timers[tcom_response] = 0;			// Start timing after the packet is sent to the slave

		if (timers[tcom_response] >= slave_max_time_out)
			comm_time_out = 1;
		else if ((gc_rcv_case < 2) && (timers[tcom_response] >= c_slave_min_time_out))
			comm_time_out = 1;
		else
			comm_time_out = 0;

		if (timers[tcom_response] >= response_wait_time)
			gc_response_wait = 1;
		
		if ((gc_trm_buf_empty == 1) && (gc_num_dev > 0) && 
			(gc_response_wait == 1) && ((gc_unpack == 1) || (comm_time_out == 1) || (gc_no_response == 1)))
		{			// unpack data when available and minimum delay has passed

			if (gc_unpack == 1)
			{		   // data is ready to be unpacked
				gc_unpack = 0;
				if (unpack_grp_com() == 1)
				{
					if ((gc_dev_ver[gc_dev_list[gc_ix]] == 0) && (gc_dev_rev[gc_dev_list[gc_ix]] == 0))
					{
						if ((gc_dev_list[gc_ix] >= c_CAR1_addr) && (gc_dev_list[gc_ix] <= c_CAR8_addr))
						{		// use packet 26 only for cars
							put_pkt_req(26,gc_dev_list[gc_ix],gc_port);
						}
					}
							
					gc_dev_online[gc_dev_list[gc_ix]] = 1;
					gc_loss_cnt[gc_dev_list[gc_ix]] = 0;		// restart count

					if (cons[cc_pb_sec] == 1)
						Send_CC_Codes();

					if (cons[Voltage_Sensor] == 1)
						Req_Car_Voltage();
					
					Send_GRP_Vars();
					
				}
				else
					gc_unpack_error ++;
			}
			else if (gc_no_response == 0)	// response time-out and no packet
			{
				gc_loss_cnt[gc_dev_list[gc_ix]]++;
				if (gc_loss_cnt[gc_dev_list[gc_ix]] > 9)
				{
					gc_loss_cnt[gc_dev_list[gc_ix]] = 1;
					gc_dev_online[gc_dev_list[gc_ix]] = 0;
				}
			}

			if (gc_restart_req == 1)
				reinit_grp_com();
			
			gc_restart_req = 0;
			if (gc_tx_grp_car_data == 0)
			{
				
				//if (gc_ix < (cons[nmbcars] - 1))	//does all cars
				if (gc_ix < (cons[nmbcars]))	//does all cars plus MR_DPY
					gc_ix++;
				else
					gc_ix = 0;	// finished the cars so start over
				
/*				if ((Car_IO_Select == gc_dev_list[gc_ix]) || (Car_Select_MRdpy == gc_dev_list[gc_ix]))
				{
					gc_diag_Car[gc_dev_list[gc_ix]] = 1;
				}
	
				if(gc_dev_online[c_MR_DPY_addr] == 0)
					Car_Select_MRdpy = 0;
*/
				if (gc_diag_Car[cons[carnmb]] == 1)
				{
					car_iodata = 1;
					gc_diag_Car[cons[carnmb]] = 0;
				}
				
			}
			
			if (gc_dev_list[gc_ix] == cons[carnmb])
			{
				if (gc_tx_grp_car_data == 0)
					gc_tx_grp_car_data = 1;
				else
					gc_tx_grp_car_data = 0;
			}
				
			
			if ((cons[nmbcars] > 1) && (car_on_grcan == 0))
			{
				if(gc_master_cnt > master_count[cons[nmbcars]])
				{	// every 50 to 100 times (3 to 5sec) through the loop release the comm to look for car 1 as master
					gc_master_cnt = 0;
					gc_master = 0;	   // cleared to allow car 1 to pick up as master

					// Setup car to receive from master 
					pack_slave_grp_com();
					gc_unpack = 0;
				}
				else
					gc_master_cnt++;
			}
			
			if (gc_master == 1)
			{     
			
				
				if (gc_dev_list[gc_ix] == cons[carnmb])
				{
					Packet = 0;
					gc_no_response = 1;
				}
				else 
				{
					gc_no_response = 0;
				}
				
				
				if ((gc_dev_list[gc_ix] >= c_CAR1_addr) && (gc_dev_list[gc_ix] <= c_CAR8_addr))
				{
					if (std_pkt_tx[gc_dev_list[gc_ix]] == 0)				// do not send consecutive request packets to cars
					{
						std_pkt_tx[gc_dev_list[gc_ix]] = 1;
						Packet = get_pkt_req(gc_dev_list[gc_ix],gc_port);	// get a Packet from the queue
					}
					else
						Packet = 0;
				}
				else
					Packet = get_pkt_req(gc_dev_list[gc_ix],gc_port);	// get a Packet from the queue

				
				if (Packet == 0)
				{
					if ((gc_dev_list[gc_ix] >= c_CAR1_addr) && (gc_dev_list[gc_ix] <= c_CAR8_addr))
					{
						
						std_pkt_tx[gc_dev_list[gc_ix]] = 0;
					
						if (gc_tx_grp_car_data == 1)
						{
							if (gc_dev_list[gc_ix] == cons[carnmb])
							{
								if (alternate_io_data == 1)
								{
									
									alternate_io_data = 0;
									
									if (car_iodata == 1) 
									{
										car_iodata = 0;
										Packet = 60;		// alternate io data
									}
									else
										Packet = 1;
								}
								else
								{
									Packet = 1;
									alternate_io_data = 1;
								}
							}
							else
								Packet = 1;			// send this car's data on the bus
						}
						else
						{
							full_update_count++;
							if (full_update_count > (c_full_update_count/2)) // get update every 1.5 second
							{					
								full_update_count = 0;
								if (full_update_car < cons[nmbcars])
									full_update_car ++;
								else 
									full_update_car = 1;
								full_update_req[full_update_car] = 1;					// set update flag for all cars
								if(cons[carnmb] == full_update_car)
									car_full_update_req = 1;
							}
							Packet = 2;       // send the standard Packet
						}
					}
					else if(gc_dev_list[gc_ix] == c_MR_DPY_addr)
					{
						std_pkt_tx[gc_dev_list[gc_ix]] = 1;
						Packet = 101;       //send standard packet
					}
					else
					{
						Packet = 26;
					}
					
				}
				pack_grp_com(Packet,gc_dev_list[gc_ix]);  // transmit group data to car
				gc_addr = gc_dev_list[gc_ix];
				grp_com_tx_start();
				timers[tcom_response] = 0;				// restart timer
				gc_response_wait = 0;
			}
		}
	}
	else
	{    // This car is not the group
/*		if ((gc_slave_tx_start == 1) && (gc_unpack == 0))
		{
			gc_slave_tx_start = 0;
			grp_com_tx_start();
		}
		else 
*/

		if ((gc_unpack != 0) || (timers[tcom_response] > c_master_packet_time))
		{							// tcom_response is a 10 msec timer
			if (gc_unpack == 1)
			{
				unpack_grp_com();
				gc_unpack = 2;
			}
			// pack data to transmit back to the group next time
			if ((gc_trm_buf_empty == 1) && ((gc_unpack == 2) || (timers[tcom_response] > c_master_time_out)))
			{
				pack_slave_grp_com();
				if (gc_unpack == 2)		// data received so respond
					grp_com_tx_start();
//					gc_slave_tx_start = 1;
				gc_unpack = 0;
			}
		}
	}

	if(gc_num_dev > 0)
	{
		if(gc_trm_int == 1)
		{
			timers[tgc_tran] = 0;
			gc_trm_int = 0;
		}
		if(gc_rcv_int == 1)
		{
			timers[tgc_recv] = 0;
			gc_rcv_int = 0;
			if (gc_master == 0) 
				timers[tgcom_master] = 0;
		}

		if (cons[carnmb] == Dispatcher) 
		{				// I'm the dispatcher
			if (gc_master == 1) 	// Car 1 is always the master when it is on line
			{		   
				timers[tgcom_master] = 0;
				gc_master = 1;
			}
			else if (timers[tgcom_master] > com_master_time_out[cons[carnmb]])
			{			  // check to become master
				gc_master = 1;
				timers[tgc_tran] = 0;
				timers[tgc_recv] = 0;
				timers[tgcom_master] = 0;
			}
		}
		else
		{
			gc_no_response = 0;
			gc_master = 0;				// If I'm not the dispatcher, I can't be the comm master.
			timers[tgcom_master] = 0;
		}

/*		if (cons[carnmb] == Dispatcher) // I'm the dispatcher
			gc_master = 1;
		else
			gc_master = 0;				// If I'm not the dispatcher, I can't be the comm master.
*/
		if (((timers[tgc_recv] > 50) || (timers[tgc_tran] > 50)) && (gc_trm_buf_empty == 1)) 
		{		 // No interrupts for too long and not transmitting
			reinit_grp_com();
			timers[tgc_tran] = 0;
			timers[tgc_recv] = 0;
			gc_trm_buf_empty = 1;
		}
	}

	if (cons[carnmb] == Dispatcher)
	{
 		Group_Comm = 1;			// if I'm the dispatcher, I'll always have comm with myself.
		Car_Not_Talking[cons[carnmb]] = 0;
		j = 1;
		for (i=1;i<=cons[nmbcars];i++)
		{	
			if (i != cons[carnmb])
			{
				if (timers[tgcom_car+i] > 20) 
				{		// comm loss with external car
					if (Car_Not_Talking[i] == 0)		// first time car is not talking
					{
					  	record_fault(grp_Car_Statm_flt_tbl[i]);	  // set car comm fault
					}
					Car_Not_Talking[i] = 1;
					j++;

					if (timers[tgcom_car+i] > 50)
					{
						timers[tgcom_car+i] = 31;
						if (j >= cons[nmbcars])		// all the cars must have timed out
							gc_restart_req = 1;
					}
				}
			}
		}
	}	 
	else
	{
		if ((Dispatcher != 0) && (c_rtc_sync != 2))			// Sync car's date and time with dispatcher
		{
			if (c_rtc_sync == 0)	
			{
				c_rtc_sync = 1;
				timer_sec[ts_crtcsync] = 0;
				if (put_11_que(109,0) != 0)					// Request dispatcher for pkt 109
					put_pkt_req(11,cons[carnmb],gc_port);
			}
			else
			{
				if (c_rtc_sync == 1)			// Car waiting for dispatcher's response
				{
					if (timer_sec[ts_crtcsync] > 1)
					{
						if (c_rtc_sync_fail_cnt <= 5)
						{
							c_rtc_sync = 0;
							c_rtc_sync_fail_cnt++;
						}
					}
				}
			}
		}
		
		j = 1;
		for (i=1;i<=cons[nmbcars];i++)
		{	
			if (i != cons[carnmb])
			{
				if (timers[tgcom_car+i] > 20) 
				{		// comm loss with external car
					j++;
				}
			}
		}
		if ((j >= cons[nmbcars]) && (Dispatcher != 0))		// all the cars must have timed out
		{									   // dispatcher == 0 is a special case for car 0 as group
			if (timers[tgcom_car+cons[carnmb]] > 50) 
			{								 // no comm with group
				if (gc_loss_cnt[cons[carnmb]] > 9)
					gc_loss_cnt[cons[carnmb]] = 1;
				else
					gc_loss_cnt[cons[carnmb]]++;
				gc_dev_online[cons[carnmb]] = 0;
				reinit_grp_com();
				timers[tgcom_car+cons[carnmb]] = 31;
			}
			if (Group_Comm == 1)
			{	
			  	record_fault(f_Group_Comm);
				Restart_Com(gc_port);
			}
			Group_Comm = 0;		// No comm with any car
			if ( ((Security_Type[cons[carnmb]] & 0x02) != 0) || ((Security_Type[cons[carnmb]] & 0x08) != 0) ||
				 ((Security_Type[cons[carnmb]] & 0x04) != 0))
			{
				gc_sec = 0;				// Comm lost so no security from group
				gc_rsec = 0;
			}
			if (cons[loc_svc_tmr] == 0)
			{		// No service activation timers if group loss and no local activation
				c_tmr_svc[s_cc_pb_sec] = 0;
				c_tmr_svc[s_sabbath] = 0;
				c_tmr_svc[s_ccl_override] = 0;
				c_tmr_svc[s_cc_lockouts] = 0;
		    }
		}
		else
		{
			gc_dev_online[cons[carnmb]] = 1;
			gc_loss_cnt[cons[carnmb]] = 0;
			timers[tgcom_car+cons[carnmb]] = 0;
		}
	}


	return;
}

//***************************************************
// Pack the slave com buffer and set to receive	data
//***************************************************

void pack_slave_grp_com(void)
{
	int16 Packet;
	
	if (prev_Packet == 1)
	{
		Packet = get_pkt_req(cons[carnmb],gc_port);	// get a Packet from the queue
		if (Packet == 0)
		{
			if (car_iodata == 1)
			{
				Packet = 60;
			}
			else
				Packet = 1;       // else send the standard Packet
		}
	}
	else
		Packet = 1;
	

	prev_Packet = Packet;
	

	gc_no_response = 0;
	gc_addr = cons[carnmb];
	pack_grp_com(Packet,cons[carnmb]);
	// set receiver on and line status interrupt 
	_disable();
	GRPtxDis; 			// Turn off Transmit driver
	// Turn off the receiver
	GRPport.CR1.B.RE = 1;			// Receiver Enable: 1 = Enable, 0 = Disable
	_enable();
	timers[tcom_response] = 0;				// restart timer
}

//*******************************
// Pack car to group comm packet
//*******************************

int16 pack_car_data_to_group(int16 dev)
{
	int16 ix,i,bc;
	static uint8 count;
	
 	pack_buf[0] = ':';      	// Start of Packet		// 0
 	pack_buf[1] = (uint8)dev; 			// From address	// 1
 	pack_buf[2] = 1; 			// Packet number		// 2
 	pack_buf[3] = 0;			// Packet length stuffed later
 	if (car_full_update_req == 1)
	{
		pack_buf[4] = 1;			// Set for full update
 		car_full_update_req = 0;
 	}
 	else
		pack_buf[4] = 0;


	// Pack Car data to group

 	ix = 5;													// index
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_pos;				// 5
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_nxt_stp;			// 6
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_up;		// 7
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_dn;		// 8
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_procf;			// 9
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_servf;			// 10
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_empf;			// 11
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_firef; 			// 12
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_doorf; 			// 13
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_rdoorf;			// 14
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_dirf;  			// 15
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_dpref; 			// 16
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_codebf;			// 17
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_dcalls;			// 18
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_upr;		// 19
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_dnr;		// 20
	
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_irup;	// 21
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_irdn;	// 22
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_rirup;	// 23
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_rirdn;	// 24
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_vipf;			// 25
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_nmbcc;			// 26
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_topcc;			// 27
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_botcc;			// 28
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_ins_status;		// 29
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_pwr_status;		// 30
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_car_alrm;		// 31
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_car_has_fault;	// 32
 	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_enc_dir;		// 33

 	uwu.w = grp_Car_Stat[dev].g_fire_status;		 	
 	pack_buf[ix++] = uwu.b[lbw];							//  34
 	pack_buf[ix++] = uwu.b[hbw];							//  35

	uwu.w = grp_Car_Stat[dev].g_dmd_vel;
 	pack_buf[ix++] = uwu.b[lbw];							//  36
 	pack_buf[ix++] = uwu.b[hbw];							//  37
 	
 	uwu.w = grp_Car_Stat[dev].g_enc_vel;
 	pack_buf[ix++] = uwu.b[lbw];							//  38
 	pack_buf[ix++] = uwu.b[hbw];							//  39
 	
 	uwu.w = grp_Car_Stat[dev].g_cur_flt;
 	pack_buf[ix++] = uwu.b[lbw];							//  40
 	pack_buf[ix++] = uwu.b[hbw];							//  41
 	
 	uwu.w = grp_Car_Stat[dev].g_control;	
 	pack_buf[ix++] = uwu.b[lbw];							//  42
 	pack_buf[ix++] = uwu.b[hbw];							//  43
 	
 	uwu.w = grp_Car_Stat[dev].g_ss_stat;
 	pack_buf[ix++] = uwu.b[lbw];							//  44
 	pack_buf[ix++] = uwu.b[hbw];							//  45
 	
 	uiu.dw = grp_Car_Stat[dev].g_run_statusf;
  	pack_buf[ix++] = uiu.b[lb];								//  46
 	pack_buf[ix++] = uiu.b[mlb];							//  47
 	pack_buf[ix++] = uiu.b[mhb];							//  48
 	pack_buf[ix++] = uiu.b[hb];								//  49

 	uiu.dw = grp_Car_Stat[dev].g_statusf1;
  	pack_buf[ix++] = uiu.b[lb];								//  50
 	pack_buf[ix++] = uiu.b[mlb];							//  51
 	pack_buf[ix++] = uiu.b[mhb];							//  52
 	pack_buf[ix++] = uiu.b[hb];								//  53
 	
	uiu.dw = grp_Car_Stat[dev].g_statusf2;
 	pack_buf[ix++] = uiu.b[lb];								//  54
 	pack_buf[ix++] = uiu.b[mlb];							//  55
 	pack_buf[ix++] = uiu.b[mhb];							//  56
 	pack_buf[ix++] = uiu.b[hb];								//  57
 	
 	uiu.dw = grp_Car_Stat[dev].g_flr_cnt;					
 	pack_buf[ix++] = uiu.b[lb];								//  58
 	pack_buf[ix++] = uiu.b[mlb];							//  59
 	pack_buf[ix++] = uiu.b[mhb];							//  60
 	pack_buf[ix++] = uiu.b[hb];								//  61
 	
 	uiu.dw = grp_Car_Stat[dev].g_pls_cnt;
 	pack_buf[ix++] = uiu.b[lb];								//  62
 	pack_buf[ix++] = uiu.b[mlb];							//  63
 	pack_buf[ix++] = uiu.b[mhb];							//  64
 	pack_buf[ix++] = uiu.b[hb];								//  65

 	uiu.dw = grp_Car_Stat[dev].g_cc[0];
  	pack_buf[ix++] = uiu.b[lb];								//  66
 	pack_buf[ix++] = uiu.b[mlb];							//  67
 	pack_buf[ix++] = uiu.b[mhb];							//  68
 	pack_buf[ix++] = uiu.b[hb];								//  69
 	
	uiu.dw = grp_Car_Stat[dev].g_cc[1];
 	pack_buf[ix++] = uiu.b[lb];								//  70
 	pack_buf[ix++] = uiu.b[mlb];							//  71
 	pack_buf[ix++] = uiu.b[mhb];							//  72
 	pack_buf[ix++] = uiu.b[hb];								//  73
 	
 	uiu.dw = grp_Car_Stat[dev].g_rcc[0];
 	pack_buf[ix++] = uiu.b[lb];								//  74
 	pack_buf[ix++] = uiu.b[mlb];							//  75
 	pack_buf[ix++] = uiu.b[mhb];							//  76
 	pack_buf[ix++] = uiu.b[hb];								//  77
 
 	uiu.dw = grp_Car_Stat[dev].g_rcc[1];
 	pack_buf[ix++] = uiu.b[lb];								//  78
 	pack_buf[ix++] = uiu.b[mlb];							//  79
 	pack_buf[ix++] = uiu.b[mhb];							//  80
 	pack_buf[ix++] = uiu.b[hb];								//  81
 	

														    // 81 + 118 max = 199
 	for(i=2; i<=cons[grtopf]; i++)
 	{
 		pack_buf[ix++] = (uint8)car_dn_eta[dev][i];
 	}
 	for(i=1; i<cons[grtopf]; i++)
 	{
 		pack_buf[ix++] = (uint8)car_up_eta[dev][i];
 	}
 	
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_trup;	//  MAX = 200
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_trdn;	//  max = 201
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_rtrup;	//  	  202
	pack_buf[ix++] = (uint8)grp_Car_Stat[dev].g_cancel_rtrdn;	//    	  203
	
	pack_buf[ix++] = grp_Car_Stat[dev].g_ins_servf;						//  	  204
	pack_buf[ix++] = grp_Car_Stat[dev].g_non_simult_doorf;				//  	  205
	
	pack_buf[ix++] = grp_Car_Stat[dev].g_ccpbs_fl_access_req;	//  	  206
	
	for(i=0; i<4; i++)											// 		  207-210	
	{
		pack_buf[ix++] = grp_Car_Stat[dev].g_ccpbs_codes[i];
	}
	
	pack_buf[ix++] = grp_Car_Stat[dev].g_dwellt_reset;	//		  211
	pack_buf[ix++] = grp_Car_Stat[dev].g_rdwellt_reset;//		  212

 	uiu.dw = grp_Car_Stat[dev].g_statusf3;
  	pack_buf[ix++] = uiu.b[lb];								//  213
 	pack_buf[ix++] = uiu.b[mlb];							//  214
 	pack_buf[ix++] = uiu.b[mhb];							//  215
 	pack_buf[ix++] = uiu.b[hb];								//  216
 	
	uiu.dw = grp_Car_Stat[dev].g_statusf4;
 	pack_buf[ix++] = uiu.b[lb];								//  217
 	pack_buf[ix++] = uiu.b[mlb];							//  218
 	pack_buf[ix++] = uiu.b[mhb];							//  219
 	pack_buf[ix++] = uiu.b[hb];								//  220

//	bc = compress(hold_buf, ix,160);
  	bc = 0;
//	if (bc == 0)
	{			// compression did not work or full update flag set so send full buffer
		if (pack_buf[4] == 0)
			pack_buf[4] = 1;					// full update

		ix = 0;
		for(i=0;i<=81;i++)
		{										//  bytes
			z_trm_buf[ix] = pack_buf[ix];	  	 
			hold_buf[ix] = pack_buf[ix];
			ix++;
		}
		
	 	for(i=0; i<(cons[grtopf]-1); i++)
		{						  // number of floors-1 in bytes
			z_trm_buf[ix] = pack_buf[ix];	  
			hold_buf[ix] = pack_buf[ix];
			ix++;
		}

	 	for(i=0; i<(cons[grtopf]-1); i++)
		{						  // number of floors-1 in bytes
			z_trm_buf[ix] = pack_buf[ix];	  
			hold_buf[ix] = pack_buf[ix];
			ix++;
		}

		for(i=0;i<=9;i++)
		{										//  bytes
			z_trm_buf[ix] = pack_buf[ix];	  	 
			hold_buf[ix] = pack_buf[ix];
			ix++;
		}
		// ix is the current byte count

	 	pack_buf[3] = (uint8)ix;      	// byte count
		z_trm_buf[3] = (uint8)ix;
		bc = compress_zero(ix,200,1);
		gc_trm_buf[3] = (uint8)bc;      	// byte count
	}
	return bc;
}


//*******************************************
// Pack the car iodata to group
//*******************************************


int16 pack_car_iodata(int16 dev)
{
	int16 ix,i,bc,j;
 	pack_buf[0] = ':';      	// Start of Packet		// 1
 	pack_buf[1] = (uint8)dev; 			// From address	// 2
 	pack_buf[2] = 60; 			// Packet number		// 3
 	
 	if(pkt11_command == 1)
 	{
 		iodata_full_update_req = 1;
 	}
/* 	
 	if (iodata_full_update_req == 1)
 	{
		pack_buf[4] = 1;			// Set for full update
 		full_update_count = 0;
 		iodata_full_update_req = 0;
 		update_flag = 1;
 	}
 	else
 	{
 		full_update_count++;
		if ((update_flag == 1) && (full_update_count > 3))
		{
 			full_update_count = 0;
 			pack_buf[4] = 2;		// Continue update
			update_flag = 0;
		}
 		else if (full_update_count > 15)
 		{
 			full_update_count = 0;
 			pack_buf[4] = 1;		// Group has not requested an update
 			update_flag = 1;
 		}
 		else
 			pack_buf[4] = 0;
 	}

*/
	// Pack Car iodata
	pack_buf[4] = 0;

 	ix = 5;
 	
 		// Pack iodata
 	for(i=0; i<258; i++)
 	{                      
 		pack_buf[ix++] = iodata[cons[carnmb]][i];
 	}

//	bc = compress(iodata_hold_buf,ix,160);
	bc = 0;	
//	if (bc == 0)
	{			// compression did not work or full update flag set so send full buffer
		if (pack_buf[4] == 0)
		{
			if (update_flag == 0)
			{
				update_flag = 1;
				pack_buf[4] = 1;					// full update
			}
			else
			{
				pack_buf[4] = 2;					// full update
				update_flag = 0;
			}

		}

		j = 0;

		for(i=0;i<5;i++)				 // first 5 bytes
		{
			z_trm_buf[j++] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
			iodata_hold_buf[i] = pack_buf[i];
		}
		
		ix = 5;
		if (pack_buf[4] == 1)
		{		 // send the iodata

			for(i=ix;i<(ix+129);i++)				 // 129 bytes
			{
				z_trm_buf[j++] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
				iodata_hold_buf[i] = pack_buf[i];
			}
		}
		else
		{										 // 129 bytes
			for(i=(ix+129);i<(ix+258);i++)
			{
				z_trm_buf[j++] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
				iodata_hold_buf[i] = pack_buf[i];
			}
		}
		// j is the current byte count

	 	pack_buf[3] = (uint8)j;      	// byte count
		z_trm_buf[3] = (uint8)j;
		bc = compress_zero(j,200,1);
		gc_trm_buf[3] = (uint8)bc;      	// byte count
	}
	return bc;

}


//*******************************************
// Pack the group data to the car
//*******************************************

int16 pack_group_data_to_car(int16 dev)
{
	int16 bc;
	int16 j;
	static int8 count;
	uint8 update_req;

	z_trm_buf[0] = ':';      	// Start of Packet
	z_trm_buf[1] = (uint8)dev; 		// To address
	z_trm_buf[2] = 2;			// Packet number
	j=4;
	uiu.dw = g_up_hall_call[0][dev];
	z_trm_buf[j++] = uiu.b[lb];
	z_trm_buf[j++] = uiu.b[mlb];
	z_trm_buf[j++] = uiu.b[mhb];
	z_trm_buf[j++] = uiu.b[hb];
	uiu.dw = g_dn_hall_call[0][dev]; 
	z_trm_buf[j++] = uiu.b[lb];
	z_trm_buf[j++] = uiu.b[mlb];
	z_trm_buf[j++] = uiu.b[mhb];
	z_trm_buf[j++] = uiu.b[hb];
	if (grtop_rfl != 0)
	{
		uiu.dw = g_upr_hall_call[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		uiu.dw = g_dnr_hall_call[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
	}
	if (cons[grtopf] > 32)
	{
		uiu.dw = g_up_hall_call[1][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		uiu.dw = g_dn_hall_call[1][dev]; 
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		if (grtop_rfl != 0)
		{
			uiu.dw = g_upr_hall_call[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			uiu.dw = g_dnr_hall_call[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
		}
	}
	if (cons[Second_Riser] != 0)
	{
		uiu.dw = g_irup_hall_call[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		uiu.dw = g_irdn_hall_call[0][dev]; 
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		if (grtop_rfl != 0)
		{
			uiu.dw = g_rirup_hall_call[0][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			uiu.dw = g_rirdn_hall_call[0][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
		}
		if (cons[grtopf] > 32)
		{
			uiu.dw = g_irup_hall_call[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			uiu.dw = g_irdn_hall_call[1][dev]; 
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			if (grtop_rfl != 0)
			{
				uiu.dw = g_rirup_hall_call[1][dev];
				z_trm_buf[j++] = uiu.b[lb];
				z_trm_buf[j++] = uiu.b[mlb];
				z_trm_buf[j++] = uiu.b[mhb];
				z_trm_buf[j++] = uiu.b[hb];
				uiu.dw = g_rirdn_hall_call[1][dev];
				z_trm_buf[j++] = uiu.b[lb];
				z_trm_buf[j++] = uiu.b[mlb];
				z_trm_buf[j++] = uiu.b[mhb];
				z_trm_buf[j++] = uiu.b[hb];
			}
		}
	}

	uiu.dw = g_cc_req[0][dev];
	z_trm_buf[j++] = uiu.b[lb];
	z_trm_buf[j++] = uiu.b[mlb];
	z_trm_buf[j++] = uiu.b[mhb];
	z_trm_buf[j++] = uiu.b[hb];
	if (grtop_rfl != 0)
	{
		uiu.dw = g_rcc_req[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
	}
	if (cons[grtopf] > 32)
	{
		uiu.dw = g_cc_req[1][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		if (grtop_rfl != 0)
		{
			uiu.dw = g_rcc_req[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
		}
	}

	z_trm_buf[j++] = (uint8)g_park[dev];
	z_trm_buf[j++] = (uint8)grp_empf[dev];
	z_trm_buf[j++] = (uint8)Dispatcher;		// send group number to car

	if(full_update_req[dev] == 1)
	{
		full_update_req[dev] = 0;	
		update_req = 1;					// request full update from car	 (set to request full update)
	}
	else
		update_req = 0;
	
	if (gc_diag_Car[dev] == 1)
	{
		update_req |= 0x20;
		gc_diag_Car[dev] = 0;
	}
	
	z_trm_buf[j++] = update_req;
	
	z_trm_buf[j++] = (uint8)ncu_door[dev];
	z_trm_buf[j++] = (uint8)lby_req[dev];
	z_trm_buf[j++] = (uint8)ncu_que[dev];
	z_trm_buf[j++] = (uint8)g_cb_fl[dev];
	z_trm_buf[j++] = (uint8)g_rcb_fl[dev];
	z_trm_buf[j++] = (uint8)grp_codebf[dev];

	z_trm_buf[j++] = (uint8)((next_up & 0x01) | ((up_peak & 0x01) << 1) | ((ir_active & 0x01) << 2) | ((in_grp_sabbath & 0x01) << 3));
	z_trm_buf[j++] = (uint8)((any_hall_call & 0x01) | ((alt_lobby & 0x01) << 1) | ((any_ir_hall_call & 0x01) << 2)); 
		 
	z_trm_buf[j++] = (uint8)ir_car;
	z_trm_buf[j++] = (uint8)g_vip_fl[dev];
	z_trm_buf[j++] = (uint8)g_rvip_fl[dev];
	
	z_trm_buf[j++] = (uint8)g_fire_ctrl;
	z_trm_buf[j++] = (uint8)g_ids_fsx;
	z_trm_buf[j++] = (uint8)(grp_hc_fire_door_open & 0x01);

	z_trm_buf[j++] = (uint8)asvto_pos[dev];		// Auto service time-out position: group set pos of car when time-out occurs
	z_trm_buf[j++] = (uint8)car_asgn_sec;		// security flag to car
	z_trm_buf[j++] = (uint8)sr_cc_ctrl[dev];
	z_trm_buf[j++] = (uint8)sec_fl_ctrl[dev];
	

	z_trm_buf[j++] = (uint8)((ids_ad_off[dev] & 0x01) | ((ids_heof[dev] & 0x01) << 1) | ((ids_rtl[dev] & 0x01) << 2) | ((ids_ind[dev] & 0x01) << 3) |
					 ((ids_allw_door[dev] & 0x01) << 4));
	z_trm_buf[j++] = (uint8)ids_return_flr;
	z_trm_buf[j++] = (uint8)ids_return_typ; 

	z_trm_buf[j++] = (uint8)((ui_ad_off[dev] & 0x01) | ((ui_heof[dev] & 0x01) << 1) | ((ui_rtl[dev] & 0x01) << 2) | ((ui_ind[dev] & 0x01) << 3));

	z_trm_buf[j++] = (uint8)(((tmr_svc[s_cc_lockouts] != 0) & 0x01) | (((tmr_svc[s_ccl_override] != 0) & 0x01) << 1) | 
					 (((tmr_svc[s_sabbath] != 0) & 0x01) << 2) | (((tmr_svc[s_cc_pb_sec] != 0) & 0x01) << 3) |
					 (((tmr_svc_en[s_cc_lockouts] != 0) & 0x01) << 4) | (((tmr_svc_en[s_ccl_override] != 0) & 0x01) << 5) |
					 (((tmr_svc_en[s_sabbath] != 0) & 0x01) << 6) | (((tmr_svc_en[s_cc_pb_sec] != 0) & 0x01) << 7));		// timer data

	z_trm_buf[j++] = (uint8)(((tmr_svc_logic[s_cc_lockouts] != 0) & 0x0F) | (((tmr_svc_logic[s_ccl_override] != 0) & 0xF0) << 4));
	
	z_trm_buf[j++] = (uint8)(((tmr_svc_logic[s_sabbath] != 0) & 0x0F) | (((tmr_svc_logic[s_cc_pb_sec] != 0) & 0xF0) << 4));

	z_trm_buf[j++] = (uint8)(((tmr_svc[s_gccl_override] != 0) & 0x01) | (((tmr_svc[s_gcc_lockouts] != 0) & 0x01) << 1) | 
					 (((tmr_svc[s_alt_fl_sec] != 0) & 0x01) << 2) |
					 (((tmr_svc_en[s_gccl_override] != 0) & 0x01) << 4) | (((tmr_svc_en[s_gcc_lockouts] != 0) & 0x01) << 5) |
					 (((tmr_svc_en[s_alt_fl_sec] != 0) & 0x01) << 6));		// timer data (space for one extra timer here)

	z_trm_buf[j++] = (uint8)(((tmr_svc_logic[s_gccl_override] != 0) & 0x0F) | (((tmr_svc_logic[s_gcc_lockouts] != 0) & 0xF0) << 4));

	z_trm_buf[j++] = (uint8)((tmr_svc[s_alt_fl_sec] != 0) & 0x0F);  // space for one extra timer here

	if (cons[Third_Riser] != 0)
	{
		uiu.dw = g_trup_hall_call[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		uiu.dw = g_trdn_hall_call[0][dev]; 
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		if (cons[grtopf] > 32)
		{
			uiu.dw = g_trup_hall_call[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			uiu.dw = g_trdn_hall_call[1][dev]; 
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
		}
		z_trm_buf[j++] = (uint8)tr_cc_ctrl[dev];
	}

	if (cons[Dest_Disp] == 1)
	{
		
		uiu.dw = g_cc_dpy[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		if (grtop_rfl != 0)
		{
			uiu.dw = g_rcc_dpy[0][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
		}
		if (cons[grtopf] > 32)
		{
			uiu.dw = g_cc_dpy[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			if (grtop_rfl != 0)
			{
				uiu.dw = g_rcc_dpy[1][dev];
				z_trm_buf[j++] = uiu.b[lb];
				z_trm_buf[j++] = uiu.b[mlb];
				z_trm_buf[j++] = uiu.b[mhb];
				z_trm_buf[j++] = uiu.b[hb];
			}
		}
	}

		
	// space for three extra timers here		
	z_trm_buf[j++] = 0;
	z_trm_buf[j++] = 0;
	z_trm_buf[j++] = 0;
	
	z_trm_buf[j++] = 0;				// this byte used for long door time with destination dispatch group
	z_trm_buf[j++] = (uint8)fvars[fvfiresw];		// send group's car fire parameter to car (incase car swings to other group)

	// spare bytes	
	z_trm_buf[j++] = 0;
	z_trm_buf[j++] = 0;
	z_trm_buf[j++] = count++;
		
	bc = compress_zero(j,(j*2)/3,1);
	gc_trm_buf[3] = (uint8)bc;      	// byte count
	return bc;
}


//*******************************************
// Pack data Packet for comm port
//*******************************************

void pack_grp_com(uint16 Packet,uint16 dev)
{
	int16 ix,i,j,k,l,bc,v_ix;
	char unsigned ck_sum;
	pkt_type pkt;
	int16 last_par;
	int16 len;
	
	ck_sum = 0;
	ix = 0;
	bc=0;
	switch (Packet)
	{
		case 0:			// Invalid packet
			break;
		case 1:      // Transmit car data to group
			bc = pack_car_data_to_group(dev);
			break;
		case 2:  	// Transmit group data to car
			bc = pack_group_data_to_car(dev);
			break;
		case 3:   // set a front car call on a slave car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 3;					// Packet number
			gc_trm_buf[3] = 5;					// byte count
			if (cc_req_fl[0] != 0)
			{
				gc_trm_buf[4] = cc_req_fl[0];			// car call floor
				cc_req_fl[0] = 0;
			}
			else if (cc_req_fl[1] != 0)
			{
				gc_trm_buf[4] = cc_req_fl[1];			// car call floor
				cc_req_fl[1] = 0;
			}
			else if (cc_req_fl[2] != 0)
			{
				gc_trm_buf[4] = cc_req_fl[2];			// car call floor
				cc_req_fl[2] = 0;
			}

			bc = 5;
			break;
		case 4:  // Set a rear car call on a slave car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 4;					// Packet number
			gc_trm_buf[3] = 5;					// packet byte count
			if (rcc_req_fl[0] != 0)
			{
				gc_trm_buf[4] = rcc_req_fl[0];		// car call floor
				rcc_req_fl[0] = 0;
			}
			else if (rcc_req_fl[1] != 0)
			{
				gc_trm_buf[4] = rcc_req_fl[1];		// car call floor
				rcc_req_fl[1] = 0;
			}
			else if (rcc_req_fl[2] != 0)
			{
				gc_trm_buf[4] = rcc_req_fl[2];		// car call floor
				rcc_req_fl[2] = 0;
			}
			bc = 5;
			break;
		case 5:	 // Add a Security Code
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 5;		// Packet number
			gc_trm_buf[3] = 9;		// packet byte count
			gc_trm_buf[4] = gc_Add_Sec_Fl_Req;
			gc_trm_buf[5] = gc_Add_Sec_Code[0];
			gc_trm_buf[6] = gc_Add_Sec_Code[1];
			gc_trm_buf[7] = gc_Add_Sec_Code[2];
			gc_trm_buf[8] = gc_Add_Sec_Code[3];
			bc = 9;
			break;
		case 6:	 // Delete a Security Code
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 6;		// Packet number
			gc_trm_buf[3] = 9;		// packet byte count
			gc_trm_buf[4] = gc_Del_Sec_Fl_Req;
			gc_trm_buf[5] = gc_Del_Sec_Code[0];
			gc_trm_buf[6] = gc_Del_Sec_Code[1];
			gc_trm_buf[7] = gc_Del_Sec_Code[2];
			gc_trm_buf[8] = gc_Del_Sec_Code[3];
			bc = 9;
			break;
		case 7:	 // Request a Security Code Table
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 7;		// Packet number
			gc_trm_buf[3] = 5;		// packet byte count
			gc_trm_buf[4] = gc_Sec_Fl_Tbl_Req[cons[carnmb]];
			bc = 5;
			break;
		case 8:			// Request Fault informatin from Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 8;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
		    for(i=0; i<max_flt_ix; i++)
		    {
				cf.Flts[i].code = 0;
				cf.Flts[i].count = 0;
		    }
			Car_Flt_ix = 0;
			break;
		case 9:			// Transmit error codes to group
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	 // From Address
			gc_trm_buf[2] = 9;		// Packet number
			gc_trm_buf[4] = Flt_ix;
			ix = 5;

			if (f.Flts[flt_tx_ix].code != 0)
			{
				gc_trm_buf[ix++] = flt_tx_ix;
				for (j=0;j<(int)sizeof(Fault_type); j++)
					gc_trm_buf[ix++] = f.fltb[flt_tx_ix][j];
			}

			if (flt_tx_ix != Flt_ix)
			{			 // Flt_ix will be the last one transmitted
				flt_tx_ix++;
				if (flt_tx_ix >= max_flt_ix)
					flt_tx_ix = 0;
//				put_pkt_req(1,cons[carnmb],gc_port);	// Request a standard packet first
				put_pkt_req(9,cons[carnmb],gc_port);	// Send more fault data
			}	
			else
				put_pkt_req(36,cons[carnmb],gc_port);
			bc = ix;
			gc_trm_buf[3] = ix;	  // packet byte count
			break;

		case 10:		// Clear Fault information on Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 10;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
			break;
		case 11:			// Request Packet from device
			pkt = get_11_que();
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 11;		// Packet number
			gc_trm_buf[3] = 6;		// packet byte count
			//gc_trm_buf[4] = get_pkt_11_req();		// request packet
			gc_trm_buf[4] = pkt.nmb;
			gc_trm_buf[5] = pkt.cmd;			
			bc = 6;
			break;
		case 12:
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 12;		// Packet number
			gc_trm_buf[3] = 6;		// packet byte count
			gc_trm_buf[4] = version;		// version number
			gc_trm_buf[5] = revision;		//revision number
			bc = 6;
			break;
		case 13:			// Send Security Car Call Lockout data to car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 13;					// Packet number
			gc_trm_buf[3] = 25;					// packet byte count
			gc_trm_buf[4] = (uint16)((gc_rsec << 1) | (gc_sec & 0x01));				// car call lockout security input
			uiu.dw = gc_cc_sec[0];
			gc_trm_buf[5] = uiu.b[lb];
			gc_trm_buf[6] = uiu.b[mlb];
			gc_trm_buf[7] = uiu.b[mhb];
			gc_trm_buf[8] = uiu.b[hb];
			uiu.dw = gc_rcc_sec[0];
			gc_trm_buf[9] = uiu.b[lb];
			gc_trm_buf[10] = uiu.b[mlb];
			gc_trm_buf[11] = uiu.b[mhb];
			gc_trm_buf[12] = uiu.b[hb];
			uiu.dw = ids_cc_sec[0][dev];
			gc_trm_buf[13] = uiu.b[lb];
			gc_trm_buf[14] = uiu.b[mlb];
			gc_trm_buf[15] = uiu.b[mhb];
			gc_trm_buf[16] = uiu.b[hb];
			uiu.dw = ids_rcc_sec[0][dev];
			gc_trm_buf[17] = uiu.b[lb];
			gc_trm_buf[18] = uiu.b[mlb];
			gc_trm_buf[19] = uiu.b[mhb];
			gc_trm_buf[20] = uiu.b[hb];
			uiu.dw = gc_ccs_per_fl[0][dev];
			gc_trm_buf[21] = uiu.b[lb];
			gc_trm_buf[22] = uiu.b[mlb];
			gc_trm_buf[23] = uiu.b[mhb];
			gc_trm_buf[24] = uiu.b[hb];

#if (Tract_HR == 1)
			gc_trm_buf[3] = 45;					// packet byte count
			uiu.dw = gc_cc_sec[1];
			gc_trm_buf[25] = uiu.b[lb];
			gc_trm_buf[26] = uiu.b[mlb];
			gc_trm_buf[27] = uiu.b[mhb];
			gc_trm_buf[28] = uiu.b[hb];

			uiu.dw = gc_rcc_sec[1];
			gc_trm_buf[29] = uiu.b[lb];
			gc_trm_buf[30] = uiu.b[mlb];
			gc_trm_buf[31] = uiu.b[mhb];
			gc_trm_buf[32] = uiu.b[hb];

			uiu.dw = ids_cc_sec[1][dev];
			gc_trm_buf[33] = uiu.b[lb];
			gc_trm_buf[34] = uiu.b[mlb];
			gc_trm_buf[35] = uiu.b[mhb];
			gc_trm_buf[36] = uiu.b[hb];

			uiu.dw = ids_rcc_sec[1][dev];
			gc_trm_buf[37] = uiu.b[lb];
			gc_trm_buf[38] = uiu.b[mlb];
			gc_trm_buf[39] = uiu.b[mhb];
			gc_trm_buf[40] = uiu.b[hb];
			uiu.dw = gc_ccs_per_fl[1][dev];
			gc_trm_buf[41] = uiu.b[lb];
			gc_trm_buf[42] = uiu.b[mlb];
			gc_trm_buf[43] = uiu.b[mhb];
			gc_trm_buf[44] = uiu.b[hb];
			bc = 45;
#endif		
			break;
		case 14:
			break;
		case 15:			// Send Security Lockout Override data to car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 15;					// Packet number
			gc_trm_buf[3] = 21;					// packet byte count
			gc_trm_buf[4] = (uint16)((gc_rsec_ovr << 1) | (gc_sec_ovr & 0x01));				// car call lockout security input
			uiu.dw = gc_cc_sec_ovr_set[0];
			gc_trm_buf[5] = uiu.b[lb];
			gc_trm_buf[6] = uiu.b[mlb];
			gc_trm_buf[7] = uiu.b[mhb];
			gc_trm_buf[8] = uiu.b[hb];
			uiu.dw = gc_rcc_sec_ovr_set[0];
			gc_trm_buf[9] = uiu.b[lb];
			gc_trm_buf[10] = uiu.b[mlb];
			gc_trm_buf[11] = uiu.b[mhb];
			gc_trm_buf[12] = uiu.b[hb];
			uiu.dw = gc_cc_sec_ovr_panic[0];
			gc_trm_buf[13] = uiu.b[lb];
			gc_trm_buf[14] = uiu.b[mlb];
			gc_trm_buf[15] = uiu.b[mhb];
			gc_trm_buf[16] = uiu.b[hb];
			uiu.dw = gc_rcc_sec_ovr_panic[0];
			gc_trm_buf[17] = uiu.b[lb];
			gc_trm_buf[18] = uiu.b[mlb];
			gc_trm_buf[19] = uiu.b[mhb];
			gc_trm_buf[20] = uiu.b[hb];
			bc = 21;
#if (Tract_HR == 1)
			gc_trm_buf[3] = 37;					// packet byte count
			uiu.dw = gc_cc_sec_ovr_set[1];
			gc_trm_buf[21] = uiu.b[lb];
			gc_trm_buf[22] = uiu.b[mlb];
			gc_trm_buf[23] = uiu.b[mhb];
			gc_trm_buf[24] = uiu.b[hb];
			uiu.dw = gc_rcc_sec_ovr_set[1];
			gc_trm_buf[25] = uiu.b[lb];
			gc_trm_buf[26] = uiu.b[mlb];
			gc_trm_buf[27] = uiu.b[mhb];
			gc_trm_buf[28] = uiu.b[hb];
			uiu.dw = gc_cc_sec_ovr_panic[1];
			gc_trm_buf[29] = uiu.b[lb];
			gc_trm_buf[30] = uiu.b[mlb];
			gc_trm_buf[31] = uiu.b[mhb];
			gc_trm_buf[32] = uiu.b[hb];
			uiu.dw = gc_rcc_sec_ovr_panic[1];
			gc_trm_buf[33] = uiu.b[lb];
			gc_trm_buf[34] = uiu.b[mlb];
			gc_trm_buf[35] = uiu.b[mhb];
			gc_trm_buf[36] = uiu.b[hb];
			bc = 37;
#endif		
			break;
		case 16:
			break;
		case 17:		// Hall call device initialization
			break;
		case 18:		// Set and cancel hall calls
			break;
		case 20:		// Request car call lockout security information
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 20;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
			break;
		case 22:			// Send group io data to car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 22;					// Packet number
			gc_trm_buf[4] = (uint16)((gc_sab_rfl << 1) | (gc_sab_fl & 0x01));				// car call lockout security input
			uiu.dw = gc_sab_fl_msk[0];
			gc_trm_buf[5] = uiu.b[lb];
			gc_trm_buf[6] = uiu.b[mlb];
			gc_trm_buf[7] = uiu.b[mhb];
			gc_trm_buf[8] = uiu.b[hb];
			uiu.dw = gc_sab_fl_msk[1];
			gc_trm_buf[9] = uiu.b[lb];
			gc_trm_buf[10] = uiu.b[mlb];
			gc_trm_buf[11] = uiu.b[mhb];
			gc_trm_buf[12] = uiu.b[hb];
			uiu.dw = gc_sab_rfl_msk[0];
			gc_trm_buf[13] = uiu.b[lb];
			gc_trm_buf[14] = uiu.b[mlb];
			gc_trm_buf[15] = uiu.b[mhb];
			gc_trm_buf[16] = uiu.b[hb];
			uiu.dw = gc_sab_rfl_msk[1];
			gc_trm_buf[17] = uiu.b[lb];
			gc_trm_buf[18] = uiu.b[mlb];
			gc_trm_buf[19] = uiu.b[mhb];
			gc_trm_buf[20] = uiu.b[hb];
			gc_trm_buf[21] = (uint16)((gc_rhugsec << 1) | (gc_hugsec & 0x01));				// car call lockout security input
			uiu.dw = gc_hugs_sec[0];
			gc_trm_buf[22] = uiu.b[lb];
			gc_trm_buf[23] = uiu.b[mlb];
			gc_trm_buf[24] = uiu.b[mhb];
			gc_trm_buf[25] = uiu.b[hb];
			uiu.dw = gc_hugs_sec[0];
			gc_trm_buf[26] = uiu.b[lb];
			gc_trm_buf[27] = uiu.b[mlb];
			gc_trm_buf[28] = uiu.b[mhb];
			gc_trm_buf[29] = uiu.b[hb];
			uiu.dw = gc_rhugs_sec[1];
			gc_trm_buf[30] = uiu.b[lb];
			gc_trm_buf[31] = uiu.b[mlb];
			gc_trm_buf[32] = uiu.b[mhb];
			gc_trm_buf[33] = uiu.b[hb];
			uiu.dw = gc_rhugs_sec[1];
			gc_trm_buf[34] = uiu.b[lb];
			gc_trm_buf[35] = uiu.b[mlb];
			gc_trm_buf[36] = uiu.b[mhb];
			gc_trm_buf[37] = uiu.b[hb];
			gc_trm_buf[3] = 38;					// packet byte count
			bc = 38;
			break;
		case 23:			// Send gpo data to group
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 22;					// Packet number
			gc_trm_buf[4] = cgpo[0];
			gc_trm_buf[5] = cgpo[1];
			gc_trm_buf[6] = cgpo[2];
			gc_trm_buf[7] = cgpo[3];
			gc_trm_buf[8] = cgpo[4];
			gc_trm_buf[9] = cgpo[5];
			gc_trm_buf[10] = cgpo[6];
			gc_trm_buf[11] = cgpo[7];
			gc_trm_buf[12] = cgpo[8];
			gc_trm_buf[13] = cgpo[9];
			gc_trm_buf[14] = cgpo[10];
			gc_trm_buf[15] = cgpo[11];
#if (Tract_HR == 1)
			gc_trm_buf[15] = cgpo[12];
			gc_trm_buf[16] = cgpo[13];
			gc_trm_buf[17] = cgpo[14];
			gc_trm_buf[18] = cgpo[15];
			gc_trm_buf[19] = cgpo[16];
			gc_trm_buf[20] = cgpo[17];
			gc_trm_buf[21] = cgpo[18];
			gc_trm_buf[22] = cgpo[19];
			gc_trm_buf[23] = cgpo[20];
			gc_trm_buf[24] = cgpo[21];
			gc_trm_buf[25] = cgpo[22];
			gc_trm_buf[26] = cgpo[23];
			gc_trm_buf[27] = cgpo[24];
			gc_trm_buf[28] = cgpo[25];
			gc_trm_buf[29] = cgpo[26];
			gc_trm_buf[3] = 31;					// packet byte count
			bc = 31;
#else
			gc_trm_buf[3] = 16;					// packet byte count
			bc = 16;
#endif
			break;
		case 24:			// Request CGPO (gpo) data from Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 24;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
			break;
		case 25:			// PI lantern packet for simple hydro
			break;	
		case 26:			// Send and request version and revision
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 26;		// Packet number
			gc_trm_buf[3] = 6;		// packet byte count
			gc_trm_buf[4] = version;	// version number
			gc_trm_buf[5] = revision;
			bc = 6;
			break;

		case 27:		// Hall call device initialization
			break;

		case 28:		// Set and cancel hall calls
			break;

		case 31:		// Send Security Code Checksum and Number of Codes for floors 1-60 (only send 30 at a time)
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 31;					// Packet number
			gc_trm_buf[4] = gc_Sec_Chksum_Fl;
			if (gc_Sec_Chksum_Fl <= 0)
				gc_Sec_Chksum_Fl = 1;
			if (cons[grtopf] > 30)
			{
				if ((gc_Sec_Chksum_Fl > 30) && (gc_Sec_Chksum_Fl <= cons[grtopf]))
					k = cons[grtopf];
				else
				{
					gc_Sec_Chksum_Fl = 1;
					k = 30;
				}
			}
			else
			{
				gc_Sec_Chksum_Fl = 1;
				k = cons[grtopf];
			}

			j = 0;
			for (i = gc_Sec_Chksum_Fl;i <= k;i++)
			{
				uwu.w = Code_Tbl_Chk[i].NumCode;
				gc_trm_buf[5 + j] = uwu.b[lbw];
				gc_trm_buf[6 + j] = uwu.b[hbw];
				uwu.w = Code_Tbl_Chk[i].CheckSum;
				gc_trm_buf[7 + j] = uwu.b[lbw];
				gc_trm_buf[8 + j] = uwu.b[hbw];
				j+=4;
			}
			bc = j + 5;		// packet byte count
			gc_trm_buf[3] = bc;		// packet byte count
			break;

		case 32:		// Send Codes to Car for one floor
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 32;					// Packet number
			gc_trm_buf[4] = gc_Sec_Fl_Tbl_Req[dev];
			gc_trm_buf[5] = gc_Sec_Code_Index;
			gc_trm_buf[6] = gc_CodeTable.NumCode;

			if (gc_Sec_Code_Index < 0)
				gc_Sec_Code_Index = 1;

			if (gc_CodeTable.NumCode < 50)
				k = gc_CodeTable.NumCode;
			else
				k = 50;

			j = 0;
			i = 0;
			while (i < k)
			{
				gc_trm_buf[7 + j] = gc_CodeTable.Codes[gc_Sec_Code_Index+i][0];
				gc_trm_buf[8 + j] = gc_CodeTable.Codes[gc_Sec_Code_Index+i][1];
				gc_trm_buf[9 + j] = gc_CodeTable.Codes[gc_Sec_Code_Index+i][2];
				gc_trm_buf[10 + j] = gc_CodeTable.Codes[gc_Sec_Code_Index+i][3];
				i++;
				j+=4;
			}
			if ((gc_Sec_Code_Index + i) >= gc_CodeTable.NumCode)
				gc_Code_Xfer_Comp = 1;
			gc_Sec_Code_Index += k;
			bc = j + 7;		// packet byte count
			gc_trm_buf[3] = bc;		// packet byte count
			break;

		case 33:		// Send Command to delete all security codes for one or all floors
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 33;					// Packet number
			gc_trm_buf[3] = 5;		// packet byte count
			gc_trm_buf[4] = gc_Del_All_Codes_Fl_Req;		// fl_req == 255 for all floors
			bc = 5;		// packet byte count
			break;
			
		case 34:		// Send Serial hall driver board security mask and light on and off times
			break;

		case 35:		// Send group voltage inputs
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 35;					// Packet number
			gc_trm_buf[3] = 13;		// packet byte count
			uwu.w = L1_Voltage;
			gc_trm_buf[4] = uwu.b[lbw];
			gc_trm_buf[5] = uwu.b[hbw];
			uwu.w = L2_Voltage;
			gc_trm_buf[6] = uwu.b[lbw];
			gc_trm_buf[7] = uwu.b[hbw];
			uwu.w = L3_Voltage;
			gc_trm_buf[8] = uwu.b[lbw];
			gc_trm_buf[9] = uwu.b[hbw];
			uwu.w = Door_Voltage;
			gc_trm_buf[10] = uwu.b[lbw];
			gc_trm_buf[11] = uwu.b[hbw];
			gc_trm_buf[12] = vs_can_online;
			bc = 13;		// packet byte count
			break;

		case 36:		// Send confirmation that all faults have been sent
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 36;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
			break;
			
		case 37:		//  Send Car  floor lockout  info to other Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 37;		// Packet number
			gc_trm_buf[3] = 12;		// packet byte count
			uiu.dw = ui_cc_sec[0][dev];
			gc_trm_buf[4] = uiu.b[lb];
			gc_trm_buf[5] = uiu.b[mlb];
			gc_trm_buf[6] = uiu.b[mhb];
			gc_trm_buf[7] = uiu.b[hb];
		   	uiu.dw = ui_cc_sec[1][dev]; 
			gc_trm_buf[8] =  uiu.b[lb];	   
			gc_trm_buf[9] =  uiu.b[mlb];	   
			gc_trm_buf[10] = uiu.b[mhb];	   
			gc_trm_buf[11] = uiu.b[hb];	   
			bc = 12;			
			break;			
			
		case 38:		// Send Parameter table to Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 38;		// Packet number
			if ((gc_Var_Tbl_Req[dev] < 1) || (gc_Var_Tbl_Req[dev] > 5))
				gc_Var_Tbl_Req[dev] = 1;
			v_ix = (int16)gc_Var_Tbl_Req[dev] + Group_Dispatch_Menu - 1;
			gc_trm_buf[4] = v_ix;				// parameter index
			last_par = (int16)last_var[v_ix] + 1;
			len = (last_par * 2) + 5;
			j = 5;
			i = 0;
			while ((i<last_par) && (i < Fvars_Per_Menu))
			{
				if (var_menu_tbl[v_ix][i] < nmbfvar)
					uwu.w = fvars[var_menu_tbl[v_ix][i]];
				else
					uwu.w = 0;
				gc_trm_buf[j++] = uwu.b[lbw];
				gc_trm_buf[j++] = uwu.b[hbw];
				i++;
			}
			gc_trm_buf[3] = len;		// packet byte count
			bc = len;
			gc_Var_Tbl_Req[gc_Var_Req_Car] = 0;
			break;

		case 39:		// Send Parameter request to group
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 39;		// Packet number
			gc_trm_buf[3] = 5;		// packet byte count
			gc_trm_buf[4] = gc_Var_Tbl_Req[cons[carnmb]];
			gc_Var_Tbl_Req[cons[carnmb]] = 0;
			bc = 5; 
			break;

		case 40:		// send parameter table checksum
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 40;		// Packet number
			ix = 0;					// index value * 5
			k = 4;
			gc_trm_buf[k++] = ix;		// if parameters are added
			for (i=0;i<5;i++)
			{
				uwu.w = grp_fvars_chksum[i+ix+Group_Dispatch_Menu];
				gc_trm_buf[k++] = uwu.b[lbw];
				gc_trm_buf[k++] = uwu.b[hbw];
			}
			ix = ((int16)svc_tmr_chksum_upd) * 5;
			gc_trm_buf[k++] = svc_tmr_chksum_upd;
			for (i=0;i<5;i++)
			{
				uwu.w = svc_tmr_chksum[i+ix];
				gc_trm_buf[k++] = uwu.b[lbw];
				gc_trm_buf[k++] = uwu.b[hbw];
			}
 			bc = k;		
			gc_trm_buf[3] = bc;		// packet byte count
			break;			

		case 41:		// send group service timer
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 41;		// Packet number

			if ((gc_Var_Tbl_Req[dev] < 21) || (gc_Var_Tbl_Req[dev] > 30))
				gc_Var_Tbl_Req[dev] = 21;
	
			ix = ((int16)gc_Var_Tbl_Req[dev]) - 21;
			k = 4;
			gc_trm_buf[k++] = (uint16)ix + 1;		// Index of timer 
			gc_trm_buf[k++] = svc_t[ix].service;
			gc_trm_buf[k++] = svc_t[ix].status;
			for (i=0;i<=2;i++)
			{
			  	gc_trm_buf[k++] = svc_t[ix].month[i];
			  	gc_trm_buf[k++] = svc_t[ix].day[i];
			  	gc_trm_buf[k++] = svc_t[ix].md_on_hour[i];
			  	gc_trm_buf[k++] = svc_t[ix].md_off_hour[i];
			  	gc_trm_buf[k++] = svc_t[ix].md_on_minute[i];
			  	gc_trm_buf[k++] = svc_t[ix].md_off_minute[i];
			}
			for (i=0;i<=6;i++)
			{
			  	gc_trm_buf[k++] = svc_t[ix].on_hour[i];
			  	gc_trm_buf[k++] = svc_t[ix].off_hour[i];
			  	gc_trm_buf[k++] = svc_t[ix].on_minute[i];
			  	gc_trm_buf[k++] = svc_t[ix].off_minute[i];
			}
			gc_trm_buf[k++] = svc_t[ix].logic;
 			bc = k;			
			gc_trm_buf[3] = bc;		// packet byte count
			gc_Var_Tbl_Req[gc_Var_Req_Car] = 0;
			break;			
	
		case 60:		// Send Car iodata to group
			bc = pack_car_iodata(dev);
			break;

			
		case 101:		// Heart beat
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 101;					// Packet number
			gc_trm_buf[3] = 9;					// packet byte count
			gc_trm_buf[4] = (uint8)(cons[carnmb] & 0xff);
			gc_trm_buf[5] = (uint8)(cons[grtopf] & 0xff);
			gc_trm_buf[6] = (uint8)(cons[nmbcars] & 0xff);
			gc_trm_buf[7] = 'G';
			gc_trm_buf[8] = '4';
			bc = 9;
			break;
			
		case 102:		//initialization byte
			gc_trm_buf[0] = ':';	// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 102;	// Packet number
			gc_trm_buf[3] = 1;		// packet byte count - stuffed later
			gc_trm_buf[4] = (uint8)(cons[carnmb] & 0xff);
			gc_trm_buf[5] = (uint8)(cons[grtopf] & 0xff);
			gc_trm_buf[6] = (uint8)(cons[nmbcars] & 0xff);
			j=6;
			for(i=1; i<=cons[grtopf]; i++)
			{
				gc_trm_buf[++j] = ' ';	 // Reserved for third byte of floor marking
				gc_trm_buf[++j] = flmrk[i][0];
				gc_trm_buf[++j] = flmrk[i][1];
			}
			for(i=1; i<=cons[nmbcars]; i++)
			{
				gc_trm_buf[++j] = car_bld_no[i][0];
				gc_trm_buf[++j] = car_bld_no[i][1];
			}
			bc = ++j;
			gc_trm_buf[3] = bc;
			break;
			
		case 103:
			gc_trm_buf[0] = ':';	// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 103;	// Packet number
			gc_trm_buf[3] = 1;		// packet byte count - stuffed later
			j = 3;
			for(i=1; i<=cons[grtopf]; i++)
			{
				gc_trm_buf[++j] = g_asign_up[i];	 // Reserved for third byte of floor marking
				gc_trm_buf[++j] = g_asign_dn[i];
				gc_trm_buf[++j] = g_asign_upr[i];
				gc_trm_buf[++j] = g_asign_dnr[i];
			}
			bc = ++j;
			gc_trm_buf[3] = bc;			
			break;
			
		case 104:
			gc_trm_buf[0] = ':';	// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 104;	// Packet number
			gc_trm_buf[3] = 1;		// packet byte count - stuffed later
			j = 3;
			gc_trm_buf[++j] = nmb_io_brds;
			
			for(i=0; i<nmb_io_brds; i++)
			{
				gc_trm_buf[++j] = ionamechksum[i];	 // Reserved for third byte of floor marking
			}
			bc = ++j;
			gc_trm_buf[3] = bc;			

			break;
			
		case 105:		//	IO labels
			gc_trm_buf[0] = ':';	// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 105;	// Packet number
			gc_trm_buf[3] = 1;		// packet byte count - stuffed later
			l = 3;
			io_brd_ix = pkt11_command;
			gc_trm_buf[++l] = io_brd_ix;//req_brd_id;.
			
			for(i = 0;i<3;i++)	
			{
				gc_trm_buf[++l] = io_valid[3*io_brd_ix + i];	
			}
			for(i = 0;i<3;i++)	
			{
				gc_trm_buf[++l] = IO_In_Out[3*io_brd_ix + i];	
			}
			for(i = 0;i<3;i++)
			{
				for(j=0;j<=7;j++)
				{
					if((io_valid[3*io_brd_ix + i] & (unsigned char)(fl_mask[j+1]))!=0)
					{
						for(k=0;k<5;k++)
						{
							gc_trm_buf[++l] = ioname[8*(3*io_brd_ix + i)+(j)][k];
						}
					}
				}
				
			}			
			bc = ++l;
			io_brd_ix = 0;
			pkt11_command = 0;
			gc_trm_buf[3] = bc;				
			break;
/*			
		case 106:		//	LCD messages checksums
			gc_trm_buf[0] = ':';	// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 106;	// Packet number
			gc_trm_buf[3] = 1;		// packet byte count - stuffed later
			j = 3;
			for(i = 0;i<5;i++)
			{
				gc_trm_buf[++j] = IO_scn_lcD.line1.chksum[i];
			}
			for(i = 0;i<30;i++)
			{
				gc_trm_buf[++j] = IO_scn_lcD.line2.chksum[i];
			}
			for(i = 0;i<60;i++)
			{
				gc_trm_buf[++j] = IO_scn_lcD.line3.chksum[i];
			}
			bc = ++j;
			gc_trm_buf[3] = bc;	
			break;
			
		case 107:		 //	LCD messages
			gc_trm_buf[0] = ':';	// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 107;	// Packet number
			gc_trm_buf[3] = 1;		// packet byte count - stuffed later
			l = 3;
			reqrd_labels_ix = pkt11_command;
			gc_trm_buf[++l] = reqrd_labels_ix;//required index for 5 lines of dpy
			if(reqrd_labels_ix < 5)
			{
				k = reqrd_labels_ix;
				for(i = 0;i<10;i++)
					for(j=0;j<25;j++)
						gc_trm_buf[++l] = IO_scn_lcD.line1.dpy[10 * k+i][j];
			}
			else if(reqrd_labels_ix < 35)
			{
				k = reqrd_labels_ix - 5;
				for(i = 0;i<10;i++)
					for(j=0;j<25;j++)
						gc_trm_buf[++l] = IO_scn_lcD.line2.dpy[10 * k+i][j];
			}
			else if(reqrd_labels_ix < 95)
			{
				k = reqrd_labels_ix - 35;
				for(i = 0;i<10;i++)
					for(j=0;j<25;j++)
						gc_trm_buf[++l] = IO_scn_lcD.line3.dpy[10 * k+i][j];
			}
			
			bc = ++l;
			io_brd_ix = 0;
			pkt11_command = 0;
			gc_trm_buf[3] = bc;				
			break;
			
		case 108:
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 108;					// Packet number
			gc_trm_buf[3] = 12;					// packet byte count
			gc_trm_buf[4] = 0;
			gc_trm_buf[5] = 0;
			gc_trm_buf[6] = 0;
			gc_trm_buf[7] = 0;
			gc_trm_buf[8] = 0;
			gc_trm_buf[9] = 0;
			gc_trm_buf[4] = line1_ix;
			gc_trm_buf[5] = line2_ix;
			gc_trm_buf[6] = line3_ix;
			gc_trm_buf[7] = line1_attr;
			gc_trm_buf[8] = line2_attr;
			gc_trm_buf[9] = line3_attr;

			gc_trm_buf[10] = 'G';
			
			gc_trm_buf[11] = '4';
			bc = 12;
			break;
*/

		case 109:		// Send dispatcher date and time to other cars
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 109;				// Packet number
			j = 3;
			local_gettime();
			gc_trm_buf[++j] = t.minute;
			gc_trm_buf[++j] = t.second;
			gc_trm_buf[++j] = t.hour;
			local_getdate();
			gc_trm_buf[++j] = d.day;
			gc_trm_buf[++j] = d.month;
			uwu.w = d.year;
			gc_trm_buf[++j] = uwu.b[lbw];
			gc_trm_buf[++j] = uwu.b[hbw];
			
			bc=++j;
			gc_trm_buf[3] = bc;
			break;

		default:
			break;
	}
	for (i=1; i < bc; i++)
		ck_sum += (uint8)gc_trm_buf[i];

	gc_trm_buf[i++] = (uint8)((0xFF - ck_sum) + 1); 			      // Set checksum in buffer
	gc_trm_buf[i++] = '@';						// packet end
	gc_trm_buf[i++] = 0xF0;						// Dummy byte
	gc_trm_buf[i] = 0xFF;						// Dummy byte
	gc_trm_buf[0] |= 0x4000;					// set ninth bit
	gc_trm_buf[1] |= 0x4000;					// set ninth bit
}

// ***************************************************************
// ******** Compress Zero's in comm  *****************************
// ***************************************************************

int16 compress_zero(int16 buf_len, int16 max_len,int16 compress_flag)
{
	// Data is in z_trm_buf[] and is checked for consecutive zeros.
	// For one zero you get two bytes of 0 followed by 1 and for n zeros
	// you get 0 followed by the number of zeros (including the first one).
	// You can never have a zero followed by zero. Any time there are more than
	// two consecutive zero's, you save at least 1 byte.  If the buffer is greater
	// than the max_len, then the compression is discarded and the full buffer 
	// is packed including the compress flag set to 8. If the copression worked, the compress
	// flag is set to 4.  The packet length is returned. 

	int16 j=0;
	int16 i=0;
	int16 zero_count=0;


	j = 0;
	for(i=0;i<=3;i++)
	{
		gc_trm_buf[j++] = z_trm_buf[i];	  // copy over the first four bytes (must be sent all the time
	}
	j=5;								// skip the compress flag
	zero_count = 0;
	for (i=4;i<buf_len;i++)
	{
		if (z_trm_buf[i] == 0)
		{
			zero_count++;
			if (i == (buf_len-1))
			{	 // last byte is a zero
				gc_trm_buf[j++] = 0;
				gc_trm_buf[j++] = zero_count;
				zero_count = 0;
			}
		}
		else 
		{
			if (zero_count != 0)
			{
				gc_trm_buf[j++] = 0;
				gc_trm_buf[j++] = zero_count;
				zero_count = 0;
			}
			gc_trm_buf[j++] = z_trm_buf[i];
		}
	}
	if ((j < max_len) && (compress_flag == 1))
	{
		gc_trm_buf[4] = 4;		// compress flag
		return j;
	}
	else 
	{
		gc_trm_buf[4] = 8;	// compress flag
		j = 5;					// skip the compress flag
		for (i=4;i<buf_len;i++)
		{				  // repack the buffer with the compress flag in it
			gc_trm_buf[j++] = z_trm_buf[i];	  // copy over the first four bytes (must be sent all the time
		}
		return j;		 
	}
}

// ***************************************************************
// ********** Compress the car to group comm buffer **************
// ***************************************************************

int16 compress(uint8 *h_buf,int16 buf_len, int16 max_len)
{
	// Data is in pack_buf[] and is compared with hold_buf[], if the 
	// data is different send the buffer index, number of consecutive
	// bytes that changed, and then the bytes that changed.  Do this
	// multiple times until at the end of the buffer or until the size of
	// buffer is greater then max_len. If greater than max_len return 0 else
	// return byte count to show that buffer is compressed and ready to transmit
	
	int16 ix_flag, count, ix_t, i;

	if(pack_buf[4] == 0)
	{		  // compress the buffer with update information only

		for(i=0;i<=4;i++)
		{
			gc_trm_buf[i] = pack_buf[i];	  // copy over the first FIVE bytes (must be sent all the time
			h_buf[i] = pack_buf[i];
		}
		ix_flag = 0;
		count = 0;
		ix_t = i;									// i is where we need to start
		while ((i<=buf_len) && (ix_t <= max_len))
		{
			if (pack_buf[i] != h_buf[i])
			{
				if (ix_flag == 0)
				{		// need to set the index
					ix_flag = 1;
					if (i >= 256)
					{	// since index cannot be a 1, us == 1 for 2 byte value of 256 + index
						gc_trm_buf[ix_t++] = 1;
						gc_trm_buf[ix_t++] = (uint16)(i-256);
					}
					else
						gc_trm_buf[ix_t++] = i;		// store index into buffer
					count = 0;
					gc_trm_buf[ix_t] = count;		// start with no consecutive bytes that have changed
					gc_trm_buf[ix_t+count+1] = pack_buf[i];	// store data byte
					h_buf[ix_t+count+1] = pack_buf[i];		// update the hold buffer
				}
				else
				{
					count++;
					gc_trm_buf[ix_t] = count;				// update the count
					gc_trm_buf[ix_t+count+1] = pack_buf[i];	// store next byte
					h_buf[ix_t+count+1] = pack_buf[i];		// update the hold buffer
				}
			}
			else
			{
				if (ix_flag == 1)
				{	// have to increment index by count amount + 1
					ix_flag = 0;
					ix_t = ix_t + count + 2;
				}
			}
			i++;					// next buffer location				
		}

		if (ix_flag == 1)
		{	// have to increment index by count amount + 1
			ix_flag = 0;
			ix_t = ix_t + count + 2;
		}

		if (ix_t >= max_len)
			return 0;		// compression not that good 
		else
		{
			gc_trm_buf[3] = ix_t;			// update byte count
			h_buf[3] = (uint8)ix_t;	
			return ix_t;					// compressed
		}
	}
	else
		return 0;		// full update - no compression

}


// ***********************************************************
// Get the packet number to be sent out with packet 11
// ***********************************************************

int16 get_pkt_11_req (void)
{
	int16 i = 0;
	int16 pkt_11_req = 0;
	while (i<5)
	{
		if (gc_pkt_11_req_pkt[pkt_11_gix] != 0)
		{
			pkt_11_req = gc_pkt_11_req_pkt[pkt_11_gix];
			gc_pkt_11_req_pkt[pkt_11_gix] = 0;
			return (pkt_11_req);
		}
		else
		{
			pkt_11_gix++;
			if (pkt_11_gix >= 5)
				pkt_11_gix = 0;
		}
		i++;
	}
	return (pkt_11_req);
}

 


// ***********************************************************
// Put the packet number in the queue for a packet 11 request
// ***********************************************************

void put_pkt_11_req (int16 pkt_nmb)
{
	int16 i = 0;
	while (i<5)
	{
		if (gc_pkt_11_req_pkt[pkt_11_pix] == 0)
		{
			gc_pkt_11_req_pkt[pkt_11_pix] = pkt_nmb;
			break;
		}
		else
		{
			pkt_11_pix ++;
			if (pkt_11_pix >= 5)
				pkt_11_pix = 0;
		}
		i++;
	}
}


//*************************************************
// Request the Line and Door voltage from each car
//*************************************************

void Req_Car_Voltage (void)
{
	static int16 second_cnt;
	static int16 volt_car;

	if (cons[nmbcars] > 1)
	{		 // only send Code Check if there are cars to send it to
		if (timers[tsec] != 0) 
		{
			if ((second_cnt & 0x01) == 0)
				second_cnt ++;
		}
		else
		{
			if ((second_cnt & 0x01) != 0)
				second_cnt ++;
		}

		if (second_cnt >= 5)
		{
			second_cnt = 0;
			volt_car++;
			if ((volt_car > cons[nmbcars]) || (volt_car <= 1))
				volt_car = 2;

			if (volt_car != cons[carnmb])
			{

				put_pkt_11_req(35);
				put_pkt_req(11,volt_car,gc_port);
			}
		}
	}
}

//********************************
// Send Car Call Codes to each car
//********************************

void Send_CC_Codes(void)
{
	static int16 second_cnt;
	static int16 prev_sec_cnt;
	static int16 cc_car;
	int16 i;

	if ((cons[nmbcars] > 1) && (gc_Var_Req_Seq == 0))
	{		 // only send Code Check if there are cars to send it to
		if (timers[tsec] != 0) 
		{
			if ((second_cnt & 0x01) == 0)
				second_cnt ++;
		}
		else
		{
			if ((second_cnt & 0x01) != 0)
				second_cnt ++;
		}

		if (second_cnt >= 10)
		{
			second_cnt = 0;
			if (cons[grtopf] > 30)
			{
				if (gc_Sec_Chksum_Fl == 1)
					gc_Sec_Chksum_Fl = 31;
				else
					gc_Sec_Chksum_Fl = 1;
			}
			else
				gc_Sec_Chksum_Fl = 1;

			if (gc_Sec_Chksum_Fl == 1)
				cc_car++;

			if (cc_car > 8)					// each car receives the packet every 80 seconds
				cc_car = 1;

			if (cc_car <= cons[nmbcars])
			{
				if (cc_car != cons[carnmb])
				{	   // Send Code Checksum for each car to verify
					put_pkt_req(31,cc_car,gc_port);
				}
			}
		}
		if (((second_cnt & 0x01) != 0) && (prev_sec_cnt != second_cnt))
		{
			if (gc_Sec_Req_Car == 0)
			{
				for (i=1;i<=cons[nmbcars];i++)
				{
					if ((i != cons[carnmb]) && (gc_Sec_Fl_Tbl_Req[i] > 0) && (gc_Sec_Fl_Tbl_Req[i] <= cons[grtopf]))
					{
						gc_Sec_Req_Car = i;
						gc_Sec_Req_Seq = 1;
						break;
					}
				} 
			}
			else if ((gc_Sec_Req_Car != cons[carnmb]) && (gc_Sec_Req_Car >= 1) && (gc_Sec_Req_Car <= cons[nmbcars]))
			{
				if (gc_dev_online[gc_Sec_Req_Car] == 1)
				{
					if (gc_Sec_Req_Seq == 1)
					{
						gc_Code_Xfer_Comp = 0;
						gc_Sec_Code_Index = 0;
						Read_CodeTbl(gc_Sec_Fl_Tbl_Req[gc_Sec_Req_Car], &gc_CodeTable, gc_LookUpTable);			
						put_pkt_req(32,gc_Sec_Req_Car,gc_port);
						gc_Sec_Req_Seq = 2;
					}
					else if (gc_Sec_Req_Seq == 2)
					{
						if (gc_Code_Xfer_Comp == 1)
						{
							gc_Sec_Fl_Tbl_Req[gc_Sec_Req_Car] = 0;
							gc_Sec_Req_Car = 0;
							gc_Sec_Req_Seq = 0;
						}
						else
						{		// send next packet
							second_cnt = 0;
							put_pkt_req(32,gc_Sec_Req_Car,gc_port);
						}
					}
				}
				else
				{
					gc_Sec_Fl_Tbl_Req[gc_Sec_Req_Car] = 0;
					gc_Sec_Req_Car = 0;
					gc_Sec_Req_Seq = 0;
				}
			}
			else
			{
				gc_Sec_Req_Car = 0;
				gc_Sec_Req_Seq = 0;
			}  
		}
		prev_sec_cnt = second_cnt;
	}
}



//*******************************************
// Send group vars and svc timers to each car
//*******************************************

void Send_GRP_Vars(void)
{
	static int16 second_cnt;
	static int16 prev_sec_cnt;
	static int16 gv_car;
	static int16 seq_cnt;
	int16 i;

	if ((cons[nmbcars] > 1) && (gc_Sec_Req_Seq == 0))
	{		 // only send Code Check if there are cars to send it to
		if (timers[tsec] != 0) 
		{
			if ((second_cnt & 0x01) == 0)
				second_cnt ++;
		}
		else
		{
			if ((second_cnt & 0x01) != 0)
				second_cnt ++;
		}

		if (second_cnt > 3)
		{
			second_cnt = 0;
			gv_car++;

			if (gv_car > 8)					// each car receives the packet every 24 seconds
			{
				gv_car = 1;
				svc_tmr_chksum_upd = (svc_tmr_chksum_upd + 1) & 0x01;
			}
			if (gv_car <= cons[nmbcars])
			{
				if (gv_car != cons[carnmb])
				{	   // Variable Checksum for each car to verify
					put_pkt_req(40,gv_car,gc_port);
				}
			}
		}
		if (((second_cnt & 0x01) == 0) && (prev_sec_cnt != second_cnt))
		{
			if (gc_Var_Req_Car == 0)
			{
				for (i=1;i<=cons[nmbcars];i++)
				{
					if ((i != cons[carnmb]) && (gc_Var_Tbl_Req[i] > 0) && (gc_Var_Tbl_Req[i] < Num_Fvars_Menus))
					{
						gc_Var_Req_Car = i;
						gc_Var_Req_Seq = 1;
						seq_cnt = 0;
						break;
					}
				} 
				if (gc_Var_Req_Seq == 0)
				{
					for (i=1;i<=cons[nmbcars];i++)
					{
						if ((i != cons[carnmb]) && (gc_Var_Tbl_Req[i] > 20) && (gc_Var_Tbl_Req[i] <= (20 + nmbsvct)))
						{
							gc_Var_Req_Car = i;
							gc_Var_Req_Seq = 3;
							seq_cnt = 0;
							break;
						}
					} 
				}
			}			 
			else if ((gc_Var_Req_Car != cons[carnmb]) && (gc_Var_Req_Car >= 1) && (gc_Var_Req_Car <= cons[nmbcars]))
			{
				if (gc_dev_online[gc_Var_Req_Car] == 1)
				{
					seq_cnt++;
					if (gc_Var_Req_Seq == 1)
					{
						gc_Var_Req_Seq = 2;
						second_cnt = 0;
						put_pkt_req(38,gc_Var_Req_Car,gc_port);
					}
					else if (gc_Var_Req_Seq == 3)
					{
						gc_Var_Req_Seq = 4;
						second_cnt = 0;
						put_pkt_req(41,gc_Var_Req_Car,gc_port);
					}
					else if (gc_Var_Tbl_Req[gc_Var_Req_Car] == 0)
					{
	  					gc_Var_Req_Seq = 0;
						gc_Var_Req_Car = 0;
					}
					else if (seq_cnt > 20)
					{
						gc_Var_Tbl_Req[gc_Var_Req_Car] = 0;
	  					gc_Var_Req_Seq = 0;
						gc_Var_Req_Car = 0;
						seq_cnt = 0;
					}
 				}
				else
				{
					gc_Var_Tbl_Req[gc_Var_Req_Car] = 0;
  					gc_Var_Req_Seq = 0;
					gc_Var_Req_Car = 0;
				}
			}
			else
			{
  				gc_Var_Req_Seq = 0;
  				gc_Var_Req_Car = 0;
			}  
		}
		prev_sec_cnt = second_cnt;
	}
}

//*********************************************************
//**** Uncompress zero's from the receive buffer  *********
//*********************************************************

int16 uncompress_zero(int16 ix)			// return uncompressed length
{
	int16 len, i,j,k;
	int16 zero_flag = 0;
	int16 zero_count = 0;

	len = (uint8)gc_rcv_buf[ix+3];

	j = 0;
	zero_flag = 0;
	zero_count = 0;
	for(i=0;i<=3;i++)
	{
		z_rcv_buf[j++] = (uint8)gc_rcv_buf[ix+i];
	}
	if ((uint8)gc_rcv_buf[ix+i] == 4)
	{								// zero's are compressed
		i++;						// skip the compress flag
		while(i < len)		// i starts at 5
		{
			if ((uint8)gc_rcv_buf[ix+i] == 0)
				zero_flag = 1;
			else
			{
				if (zero_flag == 1)
				{
					zero_count = (uint8)gc_rcv_buf[ix+i];
					for(k=0;k<zero_count;k++)
						z_rcv_buf[j++] = 0;
					zero_flag = 0;
				}
				else
					z_rcv_buf[j++] =  (uint8)gc_rcv_buf[ix+i];
			}
			i++;
		}
	}
	else
	{
		i++;				// skip the compress flag			
		while(i < len)		// i starts at 5
		{
			z_rcv_buf[j++] =  (uint8)gc_rcv_buf[ix+i];
			i++;
		}
	}
	return (j);
}
						


//*********************************************
//**** Uncompress the receive buffer  *********
//*********************************************

void uncompress(int16 dev, int16 ix, int16 iodat)				
{
	int16 len, count, ix_r,i,j;
	uint8 *upk_buf;
	
	if (iodat == 0)
		upk_buf = &unpack_buf[dev-1][0];
	else
		upk_buf = &iodata_unpack_buf[dev-1][0];
	

	len = (uint8)gc_rcv_buf[ix+3];
	if ((uint8)gc_rcv_buf[ix+4] == 0)
	{	// Data is compressed

		for(i=0;i<=4;i++)
		{
			upk_buf[i] = (uint8)gc_rcv_buf[ix+i];
		}
		while(i < (len-2))		// i starts at 5
		{
			ix_r = (uint8)gc_rcv_buf[ix+i];	   // get the index
			if (ix_r == 1)
			{		  // since 1 is not a valid index number use it to signal values greater than 255
				i++;
				ix_r = (uint16)((uint8)gc_rcv_buf[ix+i] + 256);
			}
			i++;
			count = (uint8)gc_rcv_buf[ix+i];		// get the count
			i++;
			for(j=0;j<=count;j++)
			{
				upk_buf[ix_r++] = (uint8)gc_rcv_buf[ix+i];		// store the data in the unpack buffer
				i++; 
			}
		}
	}
	else if (iodat == 0)
	{		// full packet update
		len = uncompress_zero(ix);

		// full packet update 
		i = 0;
		for(j=0;j<=len;j++)
		{										// All bytes
			upk_buf[j] = z_rcv_buf[i];
			i++;
		}
	}
	else
	{
		len = uncompress_zero(ix);
		for(i=0;i<=4;i++)
		{
			upk_buf[i] = (uint8)z_rcv_buf[ix+i];
		}
		if (z_rcv_buf[4] == 1)
			j = 0;	// full packet update type 1
		else
			j = 129;
			
		while(i < len)		// i starts at 5
		{
			upk_buf[j+i] = (uint8)z_rcv_buf[ix+i];
			i++;
		}
	}
		
}
//*********************************
// Unpack the car data to the group
//*********************************

void unpack_car_data_to_group(int16 dev, int16 ix)
{
	int16 i,j;

	if ((dev > 0) && (dev <= cons[nmbcars]))
	{
		uncompress(dev,ix,0);
		ix = 0;
		if (unpack_buf[dev-1][ix+4] == 1)
			full_update_req[dev] = 0;		// update request has been met for this car


		// Unpack Car data to group
																	// index
		grp_Car_Stat[dev].g_pos = unpack_buf[dev-1][ix+5];				// 5
		grp_Car_Stat[dev].g_nxt_stp = unpack_buf[dev-1][ix+6];			// 6
		grp_Car_Stat[dev].g_cancel_up = unpack_buf[dev-1][ix+7];		// 7
		grp_Car_Stat[dev].g_cancel_dn = unpack_buf[dev-1][ix+8];		// 8
		grp_Car_Stat[dev].g_procf = unpack_buf[dev-1][ix+9];			// 9
		grp_Car_Stat[dev].g_servf = unpack_buf[dev-1][ix+10];			// 10
		grp_Car_Stat[dev].g_empf = unpack_buf[dev-1][ix+11];			// 11
		grp_Car_Stat[dev].g_firef = unpack_buf[dev-1][ix+12]; 			// 12
		grp_Car_Stat[dev].g_doorf = unpack_buf[dev-1][ix+13]; 			// 13
		grp_Car_Stat[dev].g_rdoorf = unpack_buf[dev-1][ix+14];			// 14
		grp_Car_Stat[dev].g_dirf = unpack_buf[dev-1][ix+15];  			// 15
		grp_Car_Stat[dev].g_dpref = unpack_buf[dev-1][ix+16]; 			// 16
		grp_Car_Stat[dev].g_codebf = unpack_buf[dev-1][ix+17];			// 17
		grp_Car_Stat[dev].g_dcalls = unpack_buf[dev-1][ix+18];			// 18
		grp_Car_Stat[dev].g_cancel_upr = unpack_buf[dev-1][ix+19];		// 19
		grp_Car_Stat[dev].g_cancel_dnr = unpack_buf[dev-1][ix+20];		// 20
		
		grp_Car_Stat[dev].g_cancel_irup = unpack_buf[dev-1][ix+21];		// 21
		grp_Car_Stat[dev].g_cancel_irdn = unpack_buf[dev-1][ix+22];		// 22
		grp_Car_Stat[dev].g_cancel_rirup = unpack_buf[dev-1][ix+23];	// 23
		grp_Car_Stat[dev].g_cancel_rirdn = unpack_buf[dev-1][ix+24];	// 24
		grp_Car_Stat[dev].g_vipf = unpack_buf[dev-1][ix+25];			// 25
		grp_Car_Stat[dev].g_nmbcc = unpack_buf[dev-1][ix+26];			// 26
		grp_Car_Stat[dev].g_topcc = unpack_buf[dev-1][ix+27];			// 27
		grp_Car_Stat[dev].g_botcc = unpack_buf[dev-1][ix+28];			// 28
		grp_Car_Stat[dev].g_ins_status = unpack_buf[dev-1][ix+29];		// 29
		grp_Car_Stat[dev].g_pwr_status = unpack_buf[dev-1][ix+30];		// 30
		grp_Car_Stat[dev].g_car_alrm = unpack_buf[dev-1][ix+31];		// 31
		grp_Car_Stat[dev].g_car_has_fault = unpack_buf[dev-1][ix+32];	// 32
	 	grp_Car_Stat[dev].g_enc_dir = unpack_buf[dev-1][ix+33];			// 33

	 	uwu.b[lbw] = unpack_buf[dev-1][ix+34];						// 34
	 	uwu.b[hbw] = unpack_buf[dev-1][ix+35];						// 35
		grp_Car_Stat[dev].g_fire_status = uwu.w;		

	 	uwu.b[lbw] = unpack_buf[dev-1][ix+36];						// 36
	 	uwu.b[hbw] = unpack_buf[dev-1][ix+37];						// 37
	 	grp_Car_Stat[dev].g_dmd_vel = uwu.w;
	 	
	 	uwu.b[lbw] = unpack_buf[dev-1][ix+38];						// 38
	 	uwu.b[hbw] = unpack_buf[dev-1][ix+39];						// 39
	 	grp_Car_Stat[dev].g_enc_vel = uwu.w;
	 	
	 	uwu.b[lbw] = unpack_buf[dev-1][ix+40];						// 40
	 	uwu.b[hbw] = unpack_buf[dev-1][ix+41];						// 41
	 	grp_Car_Stat[dev].g_cur_flt = uwu.w;
	 	
	 	uwu.b[lbw] = unpack_buf[dev-1][ix+42];						// 42
	 	uwu.b[hbw] = unpack_buf[dev-1][ix+43];						// 43
	 	grp_Car_Stat[dev].g_control = uwu.w;	
	 	
	 	uwu.b[lbw] = unpack_buf[dev-1][ix+44];						// 44
	 	uwu.b[hbw] = unpack_buf[dev-1][ix+45];						// 45
	 	grp_Car_Stat[dev].g_ss_stat = uwu.w;
	 	
	  	uiu.b[lb] = unpack_buf[dev-1][ix+46];						// 46
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+47];						// 47
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+48];						// 48
	 	uiu.b[hb] = unpack_buf[dev-1][ix+49];						// 49
	 	grp_Car_Stat[dev].g_run_statusf = uiu.dw;

	  	uiu.b[lb] = unpack_buf[dev-1][ix+50];						// 50
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+51];						// 51
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+52];						// 52
	 	uiu.b[hb] = unpack_buf[dev-1][ix+53];						// 53
	 	grp_Car_Stat[dev].g_statusf1 = uiu.dw;
	 	
	 	uiu.b[lb] = unpack_buf[dev-1][ix+54];						// 54
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+55];						// 55
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+56];						// 56
	 	uiu.b[hb] = unpack_buf[dev-1][ix+57];						// 57
		grp_Car_Stat[dev].g_statusf2 = uiu.dw;
		
	 	uiu.b[lb] = unpack_buf[dev-1][ix+58];						// 58
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+59];						// 59
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+60];						// 60
	 	uiu.b[hb] = unpack_buf[dev-1][ix+61];						// 61
	  	grp_Car_Stat[dev].g_flr_cnt = uiu.dw;
	  							
	 	uiu.b[lb] = unpack_buf[dev-1][ix+62];						// 62
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+63];						// 63
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+64];						// 64
	 	uiu.b[hb] = unpack_buf[dev-1][ix+65];						// 65
		grp_Car_Stat[dev].g_pls_cnt = uiu.dw;

	  	uiu.b[lb] = unpack_buf[dev-1][ix+66];						// 66
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+67];						// 67
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+68];						// 68
	 	uiu.b[hb] = unpack_buf[dev-1][ix+69];						// 69
	 	grp_Car_Stat[dev].g_cc[0] = uiu.dw;
	 	
	 	uiu.b[lb] = unpack_buf[dev-1][ix+70];						// 70
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+71];						// 71
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+72];						// 72
	 	uiu.b[hb] = unpack_buf[dev-1][ix+73];						// 73
		grp_Car_Stat[dev].g_cc[1] = uiu.dw;
		
	 	uiu.b[lb] = unpack_buf[dev-1][ix+74];						// 74
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+75];						// 75
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+76];						// 76
	 	uiu.b[hb] = unpack_buf[dev-1][ix+77];						// 77
	 	grp_Car_Stat[dev].g_rcc[0] = uiu.dw;
	 	
	 	uiu.b[lb] = unpack_buf[dev-1][ix+78];						// 78
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+79];						// 79
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+80];						// 80
	 	uiu.b[hb] = unpack_buf[dev-1][ix+81];						// 81
	 	grp_Car_Stat[dev].g_rcc[1] = uiu.dw;
	 	

		j = 82;
		
	 	for(i=2; i<=cons[grtopf]; i++)
	 	{
	 		car_dn_eta[dev][i] = unpack_buf[dev-1][ix+j];
	 		j++;
	 	}
	 	for(i=1; i<cons[grtopf]; i++)
	 	{
	 		car_up_eta[dev][i] = unpack_buf[dev-1][ix+j];
	 		j++;
	 	}

		if (cons[Third_Riser] != 0)
		{
			grp_Car_Stat[dev].g_cancel_trup = unpack_buf[dev-1][ix+j];
			grp_Car_Stat[dev].g_cancel_trdn = unpack_buf[dev-1][ix+j+1];
			grp_Car_Stat[dev].g_cancel_rtrup = unpack_buf[dev-1][ix+j+2];
			grp_Car_Stat[dev].g_cancel_rtrdn = unpack_buf[dev-1][ix+j+3];
			j+=4;
		}

		grp_Car_Stat[dev].g_ins_servf = unpack_buf[dev-1][ix+j]; 
		j++;
		grp_Car_Stat[dev].g_non_simult_doorf = unpack_buf[dev-1][ix+j]; 
		j++;
		
		grp_Car_Stat[dev].g_ccpbs_fl_access_req = unpack_buf[dev-1][ix+j];
		j++;
		
		for(i=0; i<4; i++)
		{
			grp_Car_Stat[dev].g_ccpbs_codes[i] = unpack_buf[dev-1][ix+j];
			j++;
		}
		
		grp_Car_Stat[dev].g_dwellt_reset = unpack_buf[dev-1][ix+j];
		j++;
		grp_Car_Stat[dev].g_rdwellt_reset = unpack_buf[dev-1][ix+j];
		j++;

	  	uiu.b[lb] = unpack_buf[dev-1][ix+j];						// 50
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+j+1];						// 51
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+j+2];						// 52
	 	uiu.b[hb] = unpack_buf[dev-1][ix+j+3];						// 53
	 	grp_Car_Stat[dev].g_statusf3 = uiu.dw;
	 	j+=4;
	 	
	  	uiu.b[lb] = unpack_buf[dev-1][ix+j];						// 50
	 	uiu.b[mlb] = unpack_buf[dev-1][ix+j+1];						// 51
	 	uiu.b[mhb] = unpack_buf[dev-1][ix+j+2];						// 52
	 	uiu.b[hb] = unpack_buf[dev-1][ix+j+3];						// 53
	 	grp_Car_Stat[dev].g_statusf4 = uiu.dw;
		
		Car_Not_Talking[dev] = 0;
		timers[tgcom_car+dev] = 0;
	}
}


void unpack_car_iodata(int16 dev, int16 ix)
{
	int16 i;

	if ((dev > 0) && (dev <= cons[nmbcars]))
	{
		uncompress(dev,ix,1);
		// Unpack Car iodata 

	 	for(i=0; i<258; i++)
			iodata[dev][i] = iodata_unpack_buf[dev-1][i+ix+5];

		Car_Not_Talking[dev] = 0;
		timers[tgcom_car+dev] = 0;
	}
}

//**************************************************
// Unpack the group data to the car
//**************************************************

void unpack_group_data_to_car(int16 dev, int16 ix, int16 bc)
{
	int16 j;
	//lint -esym(438,spare_byte)
	//lint -esym(550,spare_byte)
	//lint -esym(438,uc_len)
	//lint -esym(550,uc_len)
	int16 spare_byte;
	int16 uc_len;
	uint8 update_req;
	static int16 com_gempf;

	if (dev != cons[carnmb])
		return;

	j = bc;		// to get rid of warning

	uc_len = uncompress_zero(ix);

	ix = 0;
	j = ix + 4;

	uiu.b[lb] = z_rcv_buf[j++];
	uiu.b[mlb] = z_rcv_buf[j++];
	uiu.b[mhb] = z_rcv_buf[j++];
	uiu.b[hb] = z_rcv_buf[j++];
	c_up_hall_call[0] = uiu.dw;

	uiu.b[lb] = z_rcv_buf[j++];
	uiu.b[mlb] = z_rcv_buf[j++];
	uiu.b[mhb] = z_rcv_buf[j++];
	uiu.b[hb] = z_rcv_buf[j++];
	c_dn_hall_call[0] = uiu.dw;

	if (grtop_rfl != 0)
	{

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_upr_hall_call[0] = uiu.dw;

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_dnr_hall_call[0] = uiu.dw;
	}

	if (cons[grtopf] > 32)
	{
		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_up_hall_call[1] = uiu.dw;

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_dn_hall_call[1] = uiu.dw;

		if (grtop_rfl != 0)
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_upr_hall_call[1] = uiu.dw;

			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_dnr_hall_call[1] = uiu.dw;
		}
	}

	if (cons[Second_Riser] != 0)
	{
		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_irup_hall_call[0] = uiu.dw;

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_irdn_hall_call[0] = uiu.dw;

		if (grtop_rfl != 0)
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_rirup_hall_call[0] = uiu.dw;

			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_rirdn_hall_call[0] = uiu.dw;
		}

		if (cons[grtopf] > 32)
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_irup_hall_call[1] = uiu.dw;

			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_irdn_hall_call[1] = uiu.dw;

			if (grtop_rfl != 0)
			{
				uiu.b[lb] = z_rcv_buf[j++];
				uiu.b[mlb] = z_rcv_buf[j++];
				uiu.b[mhb] = z_rcv_buf[j++];
				uiu.b[hb] = z_rcv_buf[j++];
				c_rirup_hall_call[1] = uiu.dw;

				uiu.b[lb] = z_rcv_buf[j++];
				uiu.b[mlb] = z_rcv_buf[j++];
				uiu.b[mhb] = z_rcv_buf[j++];
				uiu.b[hb] = z_rcv_buf[j++];
				c_rirdn_hall_call[1] = uiu.dw;
			}
		}
	}
	
	uiu.b[lb] = z_rcv_buf[j++];
	uiu.b[mlb] = z_rcv_buf[j++];
	uiu.b[mhb] = z_rcv_buf[j++];
	uiu.b[hb] = z_rcv_buf[j++];
	c_cc_req[0] = uiu.dw;
	
	if (grtop_rfl != 0)
	{

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_rcc_req[0] = uiu.dw;
	}

	if (cons[grtopf] > 32)
	{
		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_cc_req[1] = uiu.dw;

		if (grtop_rfl != 0)
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_rcc_req[1] = uiu.dw;
		}
	}

	
	car_Grp_Stat.c_park_fl = z_rcv_buf[j++];
	com_gempf = z_rcv_buf[j++];
	car_Grp_Stat.c_grp_car = z_rcv_buf[j++];
	
	if ((car_Grp_Stat.c_grp_car >= 0) && (car_Grp_Stat.c_grp_car <= cons[nmbcars]))
	{
		timers[tgrcan_master] = 0;			// allow current car to stay master
		Dispatcher = car_Grp_Stat.c_grp_car;
	}
	
	update_req = z_rcv_buf[j++];
	car_iodata = (update_req & 0x20) != 0;
	car_full_update_req = update_req & 0x01;
	car_Grp_Stat.c_ncu_door = z_rcv_buf[j++]; 
	car_Grp_Stat.c_lby_req_fl = z_rcv_buf[j++];
	car_Grp_Stat.c_ncu_que = z_rcv_buf[j++];

	car_Grp_Stat.c_cb_fl = z_rcv_buf[j++];
	car_Grp_Stat.c_rcb_fl = z_rcv_buf[j++];
	car_Grp_Stat.c_grp_codebf = z_rcv_buf[j++];

	car_Grp_Stat.c_next_up = (uint16)(z_rcv_buf[j] & 0x01);
	car_Grp_Stat.c_local_up_peak = (uint16)((z_rcv_buf[j] & 0x02) != 0);
	car_Grp_Stat.c_ir_active = (uint16)((z_rcv_buf[j] & 0x04) != 0);
	car_Grp_Stat.c_in_grp_sabbath = (uint16)((z_rcv_buf[j] & 0x08) != 0);
	j++;
	
	car_Grp_Stat.c_any_hall_call = (uint16)(z_rcv_buf[j] & 0x01);
	car_Grp_Stat.c_alt_lobby = (uint16)((z_rcv_buf[j] & 0x02) != 0);
	car_Grp_Stat.c_any_ir_hall_call = (uint16)((z_rcv_buf[j] & 0x04) != 0);
	j++;
	
	car_Grp_Stat.c_ir_car = z_rcv_buf[j++];
	car_Grp_Stat.c_vip_fl = z_rcv_buf[j++];
	car_Grp_Stat.c_rvip_fl = z_rcv_buf[j++];

	
	car_Grp_Stat.c_grp_fire_ctrl = z_rcv_buf[j++];
	car_Grp_Stat.c_ids_fsx = z_rcv_buf[j++];
	car_Grp_Stat.c_car_hc_fire_door_open = (uint16)(z_rcv_buf[j] & 0x01);
	j++;
	
	
	car_Grp_Stat.c_asvto_pos = z_rcv_buf[j++];	 			// Auto service time-out position
	car_Grp_Stat.c_car_asgn_sec = z_rcv_buf[j++];		// security flag from group
	car_Grp_Stat.c_sr_cc_ctrl = z_rcv_buf[j++];
	car_Grp_Stat.c_sec_fl_ctl = z_rcv_buf[j++];


	car_Grp_Stat.c_ids_ad_off = (uint16)(z_rcv_buf[j] & 0x01);
	car_Grp_Stat.c_ids_heof = (uint16)((z_rcv_buf[j] >> 1) & 0x01);
	car_Grp_Stat.c_ids_rtl = (uint16)((z_rcv_buf[j] >> 2) & 0x01);
	car_Grp_Stat.c_ids_ind = (uint16)((z_rcv_buf[j] >> 3) & 0x01);
	car_Grp_Stat.c_ids_allw_door = (uint16)((z_rcv_buf[j] >> 4) & 0x01); 
	j++;
	
	car_Grp_Stat.c_ids_return_fl  = z_rcv_buf[j++];
	car_Grp_Stat.c_ids_return_typ = z_rcv_buf[j++];

	
	car_Grp_Stat.c_ui_ad_off = (uint16)(z_rcv_buf[j] & 0x01);
	car_Grp_Stat.c_ui_heof = (uint16)( (z_rcv_buf[j] >> 1) & 0x01);
	car_Grp_Stat.c_ui_rtl = (uint16)((z_rcv_buf[j] >> 2) & 0x01);
	car_Grp_Stat.c_ui_ind = (uint16)((z_rcv_buf[j] >> 3) & 0x01);
	j++;

	if (cons[loc_svc_tmr] == 0)
	{
		c_tmr_svc[s_cc_lockouts] = (uint16)(z_rcv_buf[j] & 0x01);
		c_tmr_svc[s_ccl_override] = (uint16)((z_rcv_buf[j] >> 1) & 0x01);
		c_tmr_svc[s_sabbath] = (uint16)((z_rcv_buf[j] >> 2) & 0x01);
		c_tmr_svc[s_cc_pb_sec] = (uint16)((z_rcv_buf[j] >> 3) & 0x01);
		
		c_tmr_svc_en[s_cc_lockouts] = (uint16)((z_rcv_buf[j] >> 4) & 0x01);
		c_tmr_svc_en[s_ccl_override] = (uint16)((z_rcv_buf[j] >> 5) & 0x01);
		c_tmr_svc_en[s_sabbath] = (uint16)((z_rcv_buf[j] >> 6) & 0x01);
		c_tmr_svc_en[s_cc_pb_sec] = (uint16)((z_rcv_buf[j] >> 7) & 0x01);
		j++;
		
		c_tmr_svc_logic[s_cc_lockouts] = (uint16)(z_rcv_buf[j] & 0x0F);
		c_tmr_svc_logic[s_ccl_override] = (uint16)((z_rcv_buf[j] & 0xF0) >> 4);
		j++;
		
		c_tmr_svc_logic[s_sabbath] = (uint16)(z_rcv_buf[j] & 0x0F);
		c_tmr_svc_logic[s_cc_pb_sec] = (uint16)((z_rcv_buf[j] & 0xF0) >> 4);
		j++;
		
	}
	else
		j+=3;

	c_tmr_svc[s_gccl_override] = (uint16)(z_rcv_buf[j] & 0x01);
	c_tmr_svc[s_gcc_lockouts] = (uint16)((z_rcv_buf[j] >> 1) & 0x01);
	c_tmr_svc[s_alt_fl_sec] = (uint16)((z_rcv_buf[j] >> 2) & 0x01);
		
	c_tmr_svc_en[s_gccl_override] = (uint16)((z_rcv_buf[j] >> 4) & 0x01);
	c_tmr_svc_en[s_gcc_lockouts] = (uint16)((z_rcv_buf[j] >> 5) & 0x01);
	c_tmr_svc_en[s_alt_fl_sec] = (uint16)((z_rcv_buf[j] >> 6) & 0x01);
	j++;
		
	c_tmr_svc_logic[s_gccl_override] = (uint16)(z_rcv_buf[j] & 0x0F);
	c_tmr_svc_logic[s_gcc_lockouts] = (uint16)((z_rcv_buf[j] & 0xF0) >> 4);
	j++;
		
	c_tmr_svc_logic[s_alt_fl_sec] = (uint16)(z_rcv_buf[j] & 0x0F);
	j++;
		

	if (cons[Third_Riser] != 0)
	{
		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		g_trup_hall_call[0][dev] = uiu.dw;
		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		g_trdn_hall_call[0][dev] = uiu.dw;

		if (cons[grtopf] > 32)
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			g_trup_hall_call[1][dev] = uiu.dw;
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			g_trdn_hall_call[1][dev] = uiu.dw;
		}
		tr_cc_ctrl[dev] = (uint8)z_rcv_buf[j++];
	}

	if (cons[Dest_Disp] == 1)
	{
		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		c_cc_dpy[0] = uiu.dw;

		if (grtop_rfl != 0)
		{

			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_rcc_dpy[0] = uiu.dw;
		}

		if (cons[grtopf] > 32)
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			c_cc_dpy[1] = uiu.dw;

			if (grtop_rfl != 0)
			{
				uiu.b[lb] = z_rcv_buf[j++];
				uiu.b[mlb] = z_rcv_buf[j++];
				uiu.b[mhb] = z_rcv_buf[j++];
				uiu.b[hb] = z_rcv_buf[j++];
				c_rcc_dpy[1] = uiu.dw;
			}
		}
	}

	// space for three extra timers here		
	spare_byte = z_rcv_buf[j++];
	spare_byte = z_rcv_buf[j++];
	spare_byte = z_rcv_buf[j++];

		// spare bytes
	if (cons[Dest_Disp] == 1)
		long_door_time = z_rcv_buf[j++];
	else
	{
		long_door_time = 0;
		spare_byte = z_rcv_buf[j++];
	}
	
	if (long_door_time == 1)
		set_long_door_time = 1;
	
	car_Grp_Stat.c_fvfiresw_loc = z_rcv_buf[j++];
	spare_byte = z_rcv_buf[j++];
	spare_byte = z_rcv_buf[j++];
	spare_byte = z_rcv_buf[j++];

	Group_Comm = 1;
	timers[tgcom_car+car_Grp_Stat.c_grp_car] = 0;  // timer gets cleared for the car that is the group (car 1 or 2)
	if(com_gempf != 0)
	{
		if(timers[tgempf]>10)
		{
			car_Grp_Stat.c_gempf = com_gempf;
		}
	}
	else
	{
		timers[tgempf] = 0;
		car_Grp_Stat.c_gempf = 0;
	}
	
	// variables that get changed in the car and the group
	
	park_fl = car_Grp_Stat.c_park_fl; 
	lby_req_fl = car_Grp_Stat.c_lby_req_fl;

}


//*********************************************
// Unpack data Packet for comm port
//*********************************************

uint8 unpack_grp_com(void)
{
	static int16 dev, Packet,exception;
	uint8 found;
	int16 i,j,k,ix,hix,v_ix;
	static unsigned char pkt_end, pkt_start, bc, act_len, ck_sum, act_ck_sum;
	int16 flt_ix;
//	int16 dev_ix;
	int16 dev_temp, packet_temp, cmd_temp;	

	int16 last_par;
	int16 last_par_sent;

	exception = e_ok;
	found = 0;
	ix = gc_rcv_gix;
	hix = gc_rcv_pix;
	
	// This is not a circular buffer: if the buffer gets full there is a problem
	// It should never get full!!!

	while ((ix < hix) && (found==0))
	{
		exception = e_ok;
		pkt_start = (uint8)gc_rcv_buf[ix];
		dev = (uint8)gc_rcv_buf[ix+1];
		Packet = (uint8)gc_rcv_buf[ix+2];
		bc = (uint8)gc_rcv_buf[ix+3];		   // byte count

		if ((dev > c_LBY_DPY_addr) || (dev == 0))
			exception = e_inv_address;
		if (pkt_start != ':')
			exception = e_inv_pkt_st;
		if (bc > c_rcv_buf_len)
			exception = e_inv_len;

		if (exception == e_ok)
		{			// Could be a valid packet
			ck_sum = (uint8)gc_rcv_buf[ix+bc];
			pkt_end = (uint8)gc_rcv_buf[(ix+bc)+1];
			act_len = (uint8)gc_rcv_buf[ix+bc+2];
			act_ck_sum = (uint8)gc_rcv_buf[(ix+bc)+3];
			if (act_len != bc)
				exception = e_inv_len;
			if (act_ck_sum != ck_sum)
				exception = e_inv_ck_sum;
			if (pkt_end != '@')
				exception = e_inv_pkt_end;
		}

		if (exception == e_ok)
		{		// valid packet
			found = 1;
			if (dev == gc_addr)
			{
				gc_dev_rx_cnt[dev]++;
			}
			
			
			switch (Packet)
			{
				case 0:		// Invalid
					break;
				case 1:     // Unpack car data to group
					if (dev == 0)
					{
						if ((gc_rcv_buf[ix+5] == 1) && (gc_rcv_buf[ix+6]  == 9) &&
							(gc_rcv_buf[ix+7] == 2) && (gc_rcv_buf[ix+8] == 7))
						{		// valid car 0 group request
							timers[tgrcan_master] = 0;
							Dispatcher = 0;
						}
					}
					else
						unpack_car_data_to_group(dev,ix);
					break;
				case 2:		// Unpack group data to car
					if (dev == cons[carnmb]) // data is for this car
						unpack_group_data_to_car(dev,ix,bc);
					break;
				case 3:		  // set car call from group
					if (dev <= cons[nmbcars])
					{
						set_carcall(cons[carnmb],(uint8)(gc_rcv_buf[ix+4] & 0xff));
					}
					break;
				case 4:		  // Set rear car call from group
					if (dev <= cons[nmbcars])
					{
						set_rcarcall(cons[carnmb],(uint8)(gc_rcv_buf[ix+4] & 0xff));
					}
					break;
				case 5:         // Add a Security Code
					if (LCD_Menu == 11)
						LCD_Init = 1;	// In security display: reset so index is not corrupted

					gc_Add_Sec_Fl_Req = (uint8)gc_rcv_buf[ix+4];
					gc_Add_Sec_Code[0] = (uint8)gc_rcv_buf[ix+5];
					gc_Add_Sec_Code[1] = (uint8)gc_rcv_buf[ix+6];
					gc_Add_Sec_Code[2] = (uint8)gc_rcv_buf[ix+7];
					gc_Add_Sec_Code[3] = (uint8)gc_rcv_buf[ix+8];
					if ((gc_Add_Sec_Fl_Req > 0) && (gc_Add_Sec_Fl_Req <= cons[grtopf]))
					{
						Read_CodeTbl(gc_Add_Sec_Fl_Req,&gc_CodeTable, gc_LookUpTable);
						Add_Security_Code(gc_Add_Sec_Fl_Req, &gc_CodeTable, gc_LookUpTable, gc_Add_Sec_Code);
					}
					else if (gc_Add_Sec_Fl_Req == 0xFF)
					{
						Add_One_Code_All_Floors(1,1,gc_Add_Sec_Code);
					}
					break;
				case 6:     	// Delete a Security Code
					if (LCD_Menu == 11)
						LCD_Init = 1;	// In security display: reset so index is not corrupted

					gc_Del_Sec_Fl_Req = (uint8)gc_rcv_buf[ix+4];
					gc_Del_Sec_Code[0] = (uint8)gc_rcv_buf[ix+5];
					gc_Del_Sec_Code[1] = (uint8)gc_rcv_buf[ix+6];
					gc_Del_Sec_Code[2] = (uint8)gc_rcv_buf[ix+7];
					gc_Del_Sec_Code[3] = (uint8)gc_rcv_buf[ix+8];
					if ((gc_Del_Sec_Fl_Req > 0) && (gc_Del_Sec_Fl_Req <= cons[grtopf]))
					{
						Read_CodeTbl(gc_Add_Sec_Fl_Req,&gc_CodeTable, gc_LookUpTable);
						Delete_Security_Code(gc_Del_Sec_Fl_Req, &gc_CodeTable, gc_LookUpTable, gc_Del_Sec_Code);
					}
					else if (gc_Del_Sec_Fl_Req == 0xFF)
					{
						Delete_One_Code_All_Floors(1,1,gc_Del_Sec_Code);
					}
					break;
				case 7:     	// Request a Security Code Table
					gc_Sec_Fl_Tbl_Req[dev] = (uint8)gc_rcv_buf[ix+4];
					break;
				case 8:		// Request for fault information from group
					put_pkt_req(9,cons[carnmb],gc_port);
					flt_tx_ix = Flt_ix + 1;
					break;
				case 9:		// Fault data from car
					if (dev <= cons[nmbcars])
					{
						// put_pkt_status(9,dev,gc_port);
						if (bc > 5)
						{			// have data to unpack
							Car_Flt_ix = (uint8)gc_rcv_buf[ix+4];
							i = 5;
							while (i < bc)
							{
								flt_ix = (uint8)gc_rcv_buf[ix+i];
								if (flt_ix < max_flt_ix)
								{
									i++;
									for(j=0;j<(int)sizeof(Fault_type);j++) 
									{
										cf.fltb[flt_ix][j] = (uint8)gc_rcv_buf[ix+i];
										i++;
									}
								}
								else
									i+=sizeof(Fault_type);
							}
						}
					}
					break;	
				case 10:		// Clear fault information
					if (dev == cons[carnmb])
					{
						Flt_ix = 0;
				      	for(i=0; i<max_flt_ix; i++)
				      	{
							f.Flts[i].code = 0;
							f.Flts[i].count = 0;
				      	}
						Flt_ix = 0;
					  	Update_All_Flt();
					}
					break;
				case 11:		// Packet request
					pkt11_command = (uint8)gc_rcv_buf[ix+5];
					put_pkt_req((uint8)gc_rcv_buf[ix+4],dev,gc_port);
					break;
				case 12:		// Receive device version number
					if ((gc_dev_ver[dev] != 0) || (gc_dev_rev[dev] != 0))
					{
						fault_device = dev;
					  	record_fault(f_hc_dev_reset);
					}
					gc_dev_ver[dev] = (uint8)gc_rcv_buf[ix+4];			// Store device version number
					gc_dev_rev[dev] = (uint8)gc_rcv_buf[ix+5];			// Store device revision number
					break;
				case 13:	// Receive security car call lockout data from the group
					gc_sec = (uint8)(gc_rcv_buf[ix+4] & 0x01);		// Security switch input
					gc_rsec = (uint8)((gc_rcv_buf[ix+4] & 0x02) != 0);
					uiu.b[lb] =(uint8) gc_rcv_buf[ix+5];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+6];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+7];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+8];
					gc_cc_sec[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+9];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+10];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+11];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+12];
					gc_rcc_sec[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+13];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+14];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+15];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+16];
					ids_cc_sec[0][cons[carnmb]] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+17];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+18];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+19];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+20];
					ids_rcc_sec[0][cons[carnmb]] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+21];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+22];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+23];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+24];
					gc_ccs_per_fl[0][cons[carnmb]] = uiu.dw;
					
#if (Tract_HR == 1) 
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+25];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+26];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+27];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+28];
					gc_cc_sec[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+29];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+30];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+31];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+32];
					gc_rcc_sec[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+33];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+34];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+35];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+36];
					ids_cc_sec[1][cons[carnmb]] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+37];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+38];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+39];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+40];
					ids_rcc_sec[1][cons[carnmb]] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+41];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+42];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+43];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+44];
					gc_ccs_per_fl[1][cons[carnmb]] = uiu.dw;

#endif
					break;
				case 14:		
					break;
				case 15:	// Receive Security lockout override data from group	
					gc_sec_ovr = (uint8)(gc_rcv_buf[ix+4] & 0x01);		// Security switch input
					gc_rsec_ovr = (uint8)((gc_rcv_buf[ix+4] & 0x02) != 0);
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+5];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+6];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+7];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+8];
					gc_cc_sec_ovr_set[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+9];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+10];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+11];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+12];
					gc_rcc_sec_ovr_set[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+13];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+14];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+15];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+16];
					gc_cc_sec_ovr_panic[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+17];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+18];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+19];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+20];
					gc_rcc_sec_ovr_panic[0] = uiu.dw;
#if (Tract_HR == 1) 
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+21];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+22];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+23];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+24];
					gc_cc_sec_ovr_set[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+25];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+26];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+27];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+28];
					gc_rcc_sec_ovr_set[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+29];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+30];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+31];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+32];
					gc_cc_sec_ovr_panic[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+33];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+34];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+35];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+36];
					gc_rcc_sec_ovr_panic[1] = uiu.dw;
#endif
					break;
				case 16:		
					break;
				case 17:		// Hall Call device initialization
					break;
				case 18:		// Set and cancel hall call
					break;
				case 19:		// Hall Call input
					break;

				case 21:		// Security car call lockouts
					break;

				case 22:	// Receive group input data 
					gc_sab_fl = (uint8)(gc_rcv_buf[ix+4] & 0x01);		// Security switch input
					gc_sab_rfl = (uint8)((gc_rcv_buf[ix+4] & 0x02) != 0);
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+5];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+6];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+7];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+8];
					gc_sab_fl_msk[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+9];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+10];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+11];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+12];
					gc_sab_fl_msk[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+13];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+14];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+15];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+16];
					gc_sab_rfl_msk[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+17];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+18];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+19];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+20];
					gc_sab_rfl_msk[1] = uiu.dw;
					
					gc_hugsec = (uint8)(gc_rcv_buf[ix+21] & 0x01);		// Security switch input
					gc_rhugsec = (uint8)((gc_rcv_buf[ix+21] & 0x02) != 0);
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+22];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+23];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+24];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+25];
					gc_hugs_sec[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+26];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+27];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+28];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+29];
					gc_hugs_sec[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+30];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+31];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+32];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+33];
					gc_rhugs_sec[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+34];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+35];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+36];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+37];
					gc_rhugs_sec[1] = uiu.dw;
					break;
				case 23:	// Receive gpo output data from car
					cgpo[0] = (uint8)gc_rcv_buf[ix+4];
					cgpo[1] = (uint8)gc_rcv_buf[ix+5];
					cgpo[2] = (uint8)gc_rcv_buf[ix+6];
					cgpo[3] = (uint8)gc_rcv_buf[ix+7];
					cgpo[4] = (uint8)gc_rcv_buf[ix+8];
					cgpo[5] = (uint8)gc_rcv_buf[ix+9];
					cgpo[6] = (uint8)gc_rcv_buf[ix+10];
					cgpo[7] = (uint8)gc_rcv_buf[ix+11];
					cgpo[8] = (uint8)gc_rcv_buf[ix+12];
					cgpo[9] = (uint8)gc_rcv_buf[ix+13];
					cgpo[10] = (uint8)gc_rcv_buf[ix+14];
					cgpo[11] = (uint8)gc_rcv_buf[ix+15];
#if (Tract_HR == 1)
					cgpo[12] = (uint8)gc_rcv_buf[ix+16];
					cgpo[13] = (uint8)gc_rcv_buf[ix+17];
					cgpo[14] = (uint8)gc_rcv_buf[ix+18];
					cgpo[15] = (uint8)gc_rcv_buf[ix+19];
					cgpo[16] = (uint8)gc_rcv_buf[ix+20];
					cgpo[17] = (uint8)gc_rcv_buf[ix+21];
					cgpo[18] = (uint8)gc_rcv_buf[ix+22];
					cgpo[19] = (uint8)gc_rcv_buf[ix+23];
					cgpo[20] = (uint8)gc_rcv_buf[ix+24];
					cgpo[21] = (uint8)gc_rcv_buf[ix+25];
					cgpo[22] = (uint8)gc_rcv_buf[ix+26];
					cgpo[23] = (uint8)gc_rcv_buf[ix+27];
					cgpo[24] = (uint8)gc_rcv_buf[ix+28];
					cgpo[25] = (uint8)gc_rcv_buf[ix+29];
					cgpo[26] = (uint8)gc_rcv_buf[ix+30];
#endif
					break;
				case 24:		// Request for fault information from group
					put_pkt_req(23,cons[carnmb],gc_port);
					break;
				case 25:		// PI lantern packet for simple hydro
					break;
				case 26:
					if (dev == cons[carnmb]) // data is for this car
					{
						put_pkt_req(12,dev,gc_port);					
						gc_dev_ver[gc_car] = (uint8)gc_rcv_buf[ix+4];			// Store device version number
						gc_dev_rev[gc_car] = (uint8)gc_rcv_buf[ix+5];			// Store device revision number
					}
					break;
				case 29:		// Hall Call input
					break;

				case 30:		// Hall Call version and revision
					break;
				case 31:		// Receive Security Code Checksum and number of codes for floor
					gc_Sec_Chksum_Fl = gc_rcv_buf[ix + 4];
					if (gc_Sec_Chksum_Fl <= 0)
						gc_Sec_Chksum_Fl = 1;
					if (cons[grtopf] > 30)
					{
						if ((gc_Sec_Chksum_Fl > 30) && (gc_Sec_Chksum_Fl <= cons[grtopf]))
							k = cons[grtopf];
						else
						{
							gc_Sec_Chksum_Fl = 1;
							k = 30;
						}
					}
					else
					{
						gc_Sec_Chksum_Fl = 1;
						k = cons[grtopf];
					}

					j = 0;
					for (i = gc_Sec_Chksum_Fl; i <= k; i++)
					{	
						uwu.b[lbw] = (uint8)gc_rcv_buf[ix+5+j];
						uwu.b[hbw] = (uint8)gc_rcv_buf[ix+6+j];
						gc_Code_Tbl_Chk[i].NumCode = uwu.w;
						uwu.b[lbw] = (uint8)gc_rcv_buf[ix+7+j];
						uwu.b[hbw] = (uint8)gc_rcv_buf[ix+8+j];
						gc_Code_Tbl_Chk[i].CheckSum = uwu.w;
						j += 4;
					}
					if ((cons[grtopf] <= 30) || ((gc_Sec_Chksum_Fl > 30) && (cons[grtopf] > 30)))
						gc_Sec_Fl_Tbl_Req[dev] = (uint8)Verify_Code_Check_Table(&gc_Code_Tbl_Chk[gc_Sec_Chksum_Fl]);	  // wait for all the data
					else
						gc_Sec_Fl_Tbl_Req[dev] = 0;
						
					if (gc_Sec_Fl_Tbl_Req[dev] != 0)
					{
						put_pkt_req(7,dev,gc_port);
					}
					break;

				case 32:		// Receive Codes to Car for one floor
					if (gc_Sec_Fl_Tbl_Req[cons[carnmb]] == gc_rcv_buf[ix+4])
					{
						gc_Sec_Code_Index = gc_rcv_buf[ix+5];
						gc_CodeTable.NumCode = gc_rcv_buf[ix+6];

						if (gc_Sec_Code_Index < 0)
							gc_Sec_Code_Index = 1;

						if (gc_CodeTable.NumCode < 50)
							k = gc_CodeTable.NumCode;
						else
							k = 50;

						j = 0;
						i = 0;
						while (i < k)
						{
							gc_CodeTable.Codes[gc_Sec_Code_Index+i][0] = (uint8)gc_rcv_buf[ix+7+j];
							gc_CodeTable.Codes[gc_Sec_Code_Index+i][1] = (uint8)gc_rcv_buf[ix+8+j];
							gc_CodeTable.Codes[gc_Sec_Code_Index+i][2] = (uint8)gc_rcv_buf[ix+9+j];
							gc_CodeTable.Codes[gc_Sec_Code_Index+i][3] = (uint8)gc_rcv_buf[ix+10+j];
							i++;
							j+=4;
						}
						if ((gc_Sec_Code_Index + i) >= gc_CodeTable.NumCode)
						{
							if (LCD_Menu == 11)
								LCD_Init = 1;	// In security display: reset so index is not corrupted

							Write_CodeTbl(gc_Sec_Fl_Tbl_Req[cons[carnmb]], &gc_CodeTable, gc_LookUpTable);
						}
					}
					break;

				case 33:		// Receive Command to delete all security codes for one or all floors
					gc_Del_All_Codes_Fl_Req = (uint8)gc_rcv_buf[ix+4];
					if (gc_Del_All_Codes_Fl_Req != 0)
					{
						if ((gc_Del_All_Codes_Fl_Req >= cons[bottomf]) && (gc_Del_All_Codes_Fl_Req <= cons[topf]))
						{
							if (LCD_Menu == 11)
								LCD_Init = 1;	// In security display: reset so index is not corrupted

							Clear_All_Codes_One_Floor(gc_Del_All_Codes_Fl_Req, &gc_CodeTable, gc_LookUpTable);
						}
						else if (gc_Del_All_Codes_Fl_Req == 0xFF)
						{
							if (LCD_Menu == 11)
								LCD_Init = 1;	// In security display: reset so index is not corrupted

							Clear_All_Codes_All_Floors(1,1);
						}
					}
					break;
				case 34:		  // Set up call
					set_upcall((uint16)(gc_rcv_buf[ix+4] & 0xff));
					break;						
					

				case 35:         // Receive voltage sensor input
					uwu.b[lbw] = (uint8)gc_rcv_buf[ix+4];
					uwu.b[hbw] = (uint8)gc_rcv_buf[ix+5];
					grp_Car_Stat[dev].g_L1_Voltage = uwu.w;
					uwu.b[lbw] = (uint8)gc_rcv_buf[ix+6];
					uwu.b[hbw] = (uint8)gc_rcv_buf[ix+7];
					grp_Car_Stat[dev].g_L2_Voltage = uwu.w;
					uwu.b[lbw] = (uint8)gc_rcv_buf[ix+8];
					uwu.b[hbw] = (uint8)gc_rcv_buf[ix+9];
					grp_Car_Stat[dev].g_L3_Voltage = uwu.w;
					uwu.b[lbw] = (uint8)gc_rcv_buf[ix+10];
					uwu.b[hbw] = (uint8)gc_rcv_buf[ix+11];
					grp_Car_Stat[dev].g_Door_Voltage = uwu.w;
					grp_Car_Stat[dev].g_Voltage_Dev_Online = (uint8)gc_rcv_buf[ix+12];
					break;

				case 36:
					put_pkt_status(36,dev,gc_port);
					break;
					
				case 37:		
				 	if(dev == cons[carnmb])
					{
					   	uiu.b[lb] = gc_rcv_buf[ix+4];
						uiu.b[mlb] = gc_rcv_buf[ix+5];
						uiu.b[mhb] = gc_rcv_buf[ix+6];
						uiu.b[hb] = gc_rcv_buf[ix+7];
						ui_cc_sec[0][cons[carnmb]] = uiu.dw;						   						
						uiu.b[lb] = gc_rcv_buf[ix+8];
						uiu.b[mlb] = gc_rcv_buf[ix+9];
						uiu.b[mhb] = gc_rcv_buf[ix+10];
						uiu.b[hb] = gc_rcv_buf[ix+11];
					   	ui_cc_sec[1][cons[carnmb]] = uiu.dw;			 
					} 				 
					break;
					
				case 38:		// Receive group parameters
				 	if(dev == cons[carnmb])
					{
						v_ix = gc_rcv_buf[ix+4];
						if (v_ix < Num_Fvars_Menus)
						{
							last_par = ((int16)last_var[v_ix]) + 1;
							last_par_sent = (((int16)bc) - 5)/2; 
							if (last_par_sent < last_par)
								last_par = last_par_sent;
							i = 0;
							j = 5;
							while ((j < bc) && (i < last_par) && (i < Fvars_Per_Menu))
							{
								uwu.b[lbw] = gc_rcv_buf[ix+j];
								j++;
								uwu.b[hbw] = gc_rcv_buf[ix+j];
								j++;
								if (var_menu_tbl[v_ix][i] < nmbfvar)
									fvars[var_menu_tbl[v_ix][i]] = uwu.w;
								i++;
							}
							wrfvar();
						}
					} 				 
					break;

				case 39:		// Receive group parameters	request
 					gc_Var_Tbl_Req[dev] = gc_rcv_buf[ix+4];
					break;					
					
				case 40:		// receive parameter checksums
				 	if(dev == cons[carnmb])
					{
						j = gc_rcv_buf[ix+4];			// checksum index
						if (j > 0)
							j = 0;
						k = 5;
						for (i=0;i<5;i++)
						{
							uwu.b[lbw] = gc_rcv_buf[ix+k];
							k++;
							uwu.b[hbw] = gc_rcv_buf[ix+k];
							k++;
							t_grp_fvars_chksum[i+j+Group_Dispatch_Menu] = uwu.w; 
						}
						svc_tmr_chksum_upd = gc_rcv_buf[ix+k];			// checksum index
						k++;
						if (svc_tmr_chksum_upd <= 1)
						{
							j = ((int16)svc_tmr_chksum_upd) * 5;
							for (i=0;i<5;i++)
							{
								uwu.b[lbw] = gc_rcv_buf[ix+k];
								k++;
								uwu.b[hbw] = gc_rcv_buf[ix+k];
								k++;
								t_svc_tmr_chksum[i+j] = uwu.w; 
							}
						}
						
 						gc_Var_Tbl_Req[cons[carnmb]] = 0;
						
						for (i=0;i<5;i++)
						{		   // check all of the group parameters before checking the service timers
							if (grp_fvars_chksum[Group_Dispatch_Menu + i] != t_grp_fvars_chksum[Group_Dispatch_Menu + i]) 
							{
								gc_Var_Tbl_Req[cons[carnmb]] = 1 + ((uint8)i);
								break;
							}	
						}
						if ((gc_Var_Tbl_Req[cons[carnmb]] == 0) && (cons[loc_svc_tmr] == 0))
						{				// no other request being demanded and theis car does not have a local service timer
							j = ((int16)svc_tmr_chksum_upd) * 5;
							for (i=0;i<5;i++)
							{
								if (svc_tmr_chksum[j+i] != t_svc_tmr_chksum[j+i]) 
								{
									gc_Var_Tbl_Req[cons[carnmb]] = 21 + ((uint8)i) + ((uint8)j);
									break;
								}	
							}
						}						
						if (gc_Var_Tbl_Req[cons[carnmb]] != 0)
		 					put_pkt_req(39,cons[carnmb],gc_port);
					}
					break;

				case 41:		// Receive group service timers
				 	if(dev == cons[carnmb])
					{
						i = gc_rcv_buf[ix+4];		// timer number + 1;
						if ((i >= 1) && (i <= 10))
						{
							i -= 1;
					
							if (i < nmbsvct)
							{
								k = 5;
								svc_t[i].service = gc_rcv_buf[ix+k];
								k++;
								svc_t[i].status = gc_rcv_buf[ix+k];
								k++;
								for (j=0;j<=2;j++)
								{
									svc_t[i].month[j] = gc_rcv_buf[ix+k];
									k++;
									svc_t[i].day[j] = gc_rcv_buf[ix+k];
									k++;
								  	svc_t[i].md_on_hour[j] = gc_rcv_buf[ix+k];
									k++;
								  	svc_t[i].md_off_hour[j] = gc_rcv_buf[ix+k];
									k++;
								  	svc_t[i].md_on_minute[j] = gc_rcv_buf[ix+k];
									k++;
								  	svc_t[i].md_off_minute[j] = gc_rcv_buf[ix+k];
									k++;
								}
								for (j=0;j<=6;j++)
								{
								  	svc_t[i].on_hour[j] = gc_rcv_buf[ix+k];
									k++;
								  	svc_t[i].off_hour[j] = gc_rcv_buf[ix+k];
									k++;
								  	svc_t[i].on_minute[j] = gc_rcv_buf[ix+k];
									k++;
								  	svc_t[i].off_minute[j] = gc_rcv_buf[ix+k];
									k++;
								}
								svc_t[i].logic = gc_rcv_buf[ix+k];
								k++;
								Wrt_Grp_Svc_Timer(i);
							}
						}
					} 				 
					break;

					
				case 60:		// Unpack iodata from car
					unpack_car_iodata(dev, ix);
					break;
/*
				case 101:	   // Log on  user
					Car_Select_MRdpy = (uint8)gc_rcv_buf[ix+6];
					put_pkt_status(101,dev,gc_port);
					break;
*/
				case 102:	   //Group initializaton data
					put_pkt_req(102,dev,gc_port);
					break;
				
				case 103:	   // password
					put_pkt_req(103,dev,gc_port);
					break;
				
				case 104:		// write new user name
					put_pkt_req(104,cons[carnmb],gc_port);
					break;
				
				case 105:
					put_pkt_req(105,cons[carnmb],gc_port);
					break;
					
				case 106:	// set date and time
			    	d.year = gc_rcv_buf[ix+4];
			    	d.year = (uint16)((gc_rcv_buf[ix+5] << 4) | d.year);
			    	d.month = gc_rcv_buf[ix+6];
			    	d.day = gc_rcv_buf[ix+7];
			    	t.hour = gc_rcv_buf[ix+8];
			    	t.minute = gc_rcv_buf[ix+9];
					local_setdate();
					local_settime();
					break;
					
				case 107:	// Get galcom version
					
					break;
					
				case 108:	//restart comm
					break;
					
				case 109:
					t.minute = gc_rcv_buf[ix+4];
					t.second = gc_rcv_buf[ix+5];
					t.hour = gc_rcv_buf[ix+6];
					local_settime();
					d.day = gc_rcv_buf[ix+7];
					d.month = gc_rcv_buf[ix+8];
					uwu.b[lbw] = gc_rcv_buf[ix+9];
					uwu.b[hbw] = gc_rcv_buf[ix+10];
					d.year = uwu.w;
					local_setdate();
					c_rtc_sync = 2;		// Car's rtc successfully synced with dispatcher
					break;
					
				case 110:	//request group initialization data
					break;
					
				case 111:		// Packet request from c_MR_DPY_addr
					dev_temp = (uint8)gc_rcv_buf[ix+4];
					packet_temp = (uint8)gc_rcv_buf[ix+5];
					cmd_temp = (uint8)gc_rcv_buf[ix+6];
					if(dev_temp == cons[carnmb])
					{
						put_pkt_req(packet_temp,dev_temp,gc_port);
						pkt11_command = cmd_temp;
					}
					else if((dev_temp > c_CAR1_addr) && (dev_temp <= c_CAR8_addr))
					{
						//put_pkt_11_req(packet2);
						if (put_11_que(packet_temp,cmd_temp))
							put_pkt_req(11,dev_temp,gc_port);
					}
						
					break;
					
				case 112:	//Extended group initialization
					break;
					
				case 113:	//send io board status
					break;

					
				case 200:
					
					break;

				default:
					break;
			}
			ix = (int16)(ix + bc + 3);
		}
		else
			ix++;
		gc_rcv_gix = ix;
	}
	return found;
}




// ***************************************************************
// ******** put packet number and address in 111 que **********
// Put packet in pkt_111_que. data is added at the end of the
// from the location first and the index is advanced by one
// ********                                      *****************
// ***************************************************************

int16 put_11_que (int16 nmb, int16 cmd)
{
	pkt_type pkt;
	
	pkt.nmb = nmb;
	pkt.cmd = cmd;
	if (pkt_11_que.used != c_que_len)
	{
		pkt_11_que.pix++;
		if (pkt_11_que.pix >= c_que_len)
			pkt_11_que.pix = 0;
		pkt_11_que.packet[pkt_11_que.pix] = pkt;
		pkt_11_que.used++;		
		return(1);
	}
	return(0);
}

// ***************************************************************
// ******** Get packet number off of request que *****************
// Get packet out of que:  Data is taken off of the que from
// if first used is zero current and first match and we DONT_CARE
// for what data is saved in the array
//****************************************************************
// ***************************************************************


pkt_type get_11_que (void)
{
	pkt_type pkt;
	pkt.nmb = 0;
	pkt.cmd = 0;
	if (pkt_11_que.used != 0)
	{
		pkt_11_que.gix++;
		if (pkt_11_que.gix >= c_que_len)
			pkt_11_que.gix = 0;
		pkt = pkt_11_que.packet[pkt_11_que.gix];
		pkt_11_que.used--;
	}
	return pkt;
}

// ***************************************************************
// ******** Is My queue Empty*************************************
// ***************************************************************

bool empty_11_que(void)
{
	if(pkt_11_que.used == 0)
	{
		return 1;
	}	
	return 0;
	
}


/* Revision History

9/13/11 v5.54.24 hdl 	1. Added local_up_peak to the data sent over to each car

9/22/11 v5.54.25 mhd	1. Set EMP and EPT status in grp_Car_Stat[cons[carnmb]].g_empf for split power feeder operation.
						2. Use gempf instead of grp_empf[0] from group.
v6.0.0	04/08/11  mhd 		1. When gc_dev_ver and gc_dev_rev == 0, must initialize hall call board with packet 17 or 27 but not packet 26.

2/1/12 v5.54.53 mhd		1. Send bit from group to car for grp_hc_fire_door_open which get set as car_Grp_Stat.c_car_hc_fire_door_open. (Special for denver fire service)

2/27/11	v5.54.59 hdl	1. Send the information for hall calls security to both the main and second riser depending on field variable  fvhcsecctrl 

3/2/12 v5.54.60 mhd		1. Anded i_EPT with cons variable ((cons[EMPop] & 0x01) != 0) so the i_EPT is not set from BDC input. See emp_bits.
6/14/12 v5.54.87 hdl 	1. Added g_ids_fsx to control the fsx input being sent from liftnet from a master switch for all groups and all cars.
   ^^^ v6.0,12 mhd 
2/13/13 v7.0.1 mhd		1. Prevent any car greater than car 2 from being the dispatch car if on emergency power. Without group control the car can return from the EPS input.
67/31/13 v7.0.15 mhd	1. Change packet 9 to only send one fault at a time.
8/25/13 v7.0.16 mhd		1. Added the following changes from galaxy 3:
						   {
								v5.55.37 mhd	1. Added routines to calculate the group field variable and service timer checksum.
												4. Changed the number of service timers nmbsvct to 10 from 40. Don't know what I was thinking in version v5.52.9 but there are only 10 service timers. I probably confused 
												   it with the number of services available for the service timers.
												   
								v5.55.38 as		1. Modified pack_grp_com() and unpack_grp_com() case 13 to send/recieve group car call security masks gc_ccs_per_fl[2][9] from dispatcher to each car 

								v6.0.50 as		1. Added below Galaxy III changes made in v5.55.47 and v5.55.48
												2. Modified Group_Com() to allow car 2 and above to be the dispatcher in case if a car is manually selected to run on EMP, this will allow the hall calls assignment
												3. Modified pack_car_data_to_group() and unpack_car_data_to_group() routines to send third riser hall call cancel flags	from car to group
												4. Modified pack_group_data_to_car() and unpack_group_data_to_car() routines to send third riser hall call data from group to car
						   }	   
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.
						2. Replaced iodata[1] with iodata[cons[carnmb]] where necessary.
1/7/14 V7.0.31 mhd		1. Modified pack_slave_grp_com() to check the packet que before looking for car_iodata.  Also do not clear car io data, let the group car change it.
						2. Added Car_IO_Select to select the car to send io data instead of Car_Select because Car_Select has to be set to retreive fault data from the car.
						3. Deleted setting packet 1 in que in packet 9 since pack_slave_grp_com() already alternates the packets.
2/26/14 v7.0.35 mhd		1. Modified End of transmit timing and start of transmit for slave car.  See grp_dma_trm_int() and gc_slave_tx_start.
5/27/14 v7.1.0 mhd		1. Added car call request from group in group to car data.
						2. Added check to make car 0 group in packet 1.
5/29/14 v7.1.2 mhd		1. Set c_cc_req[] and c_rcc_req[] in local_group_to_car() from g_cc_req[][] and g_rcc_req[][].
6/20/14 v7.1.3 mhd		1. Removed standard compression routine from the car to group comm.  Fix bug in uncompress routine to use length from uncompress_zero.
8/15/14 v7.1.8 mhd		1. Added g_cc_dpy[][], g_rcc_dpy[][], c_cc_dpy[][] and c_rcc_dpy[][] to set destination car call display in car.  This data has also been added to the
						   group to car and car to group comm.
10/8/14 v7.1.14 mhd		1. Added non-simultaneous door flag parameter in car to group comm.
12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
								13 v6.1.14 mhd	1. Changed reinit_grp_comm() to set gc_trm_buf_empty, otherwise comm can get locked up.
								4/14/14 v6.1.50 hdl		1. Added car_Grp_Stat.c_fvfiresw_loc to determine fire switch location from the group. Special operation was required
11/17/15 v7.1.48 as		1. Found an issue with the pkt 11 queue implementation, decided to reimplement it with ease of readability with gix and pix's.
						2. Added logic for non dispatch car to request packet 109 from dispatcher for syncing its date and time with the dispatcher.
1/24/17 v7.2.50 mhd		1. Added statusf3 and statusf4 in car to gorup packet.
						2. Added the following change from G4 traction:
							9/16/16 v7.2.18 as		1. Modified pack_car_data_to_group() to add g_dwellt_reset and g_rdwellt_reset bytes to the end of the packet.

*/
