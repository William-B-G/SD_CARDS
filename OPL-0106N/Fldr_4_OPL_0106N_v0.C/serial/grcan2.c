#define d_GRCAN 1

#include "global.h"

/*

	Group Can for GALaxy 4

	This protocol is setup with the main CPU of the elevator as the master device and all 
	other can devices as slave devices.  The identifier consist of a message number and 
	a device address number.  When a messaged is sent to a slave device, it will include 
	the address of that particular slave device.  When a slave device responds, the address 
	is its own address showing where the message is comming from.  In short all data
	on the bus is being transmitted between master and slave and not from slave to slave.  
 	
	The baud rate is set to 115200 baud. Device address assignments and message information 
	is listed below:

		Can Port Device Map

	Device		  Number
	 Car			1
     VS		 		10		  Voltage Sensor
     SEB_1	 		21		  Serial I/O Expansion board 1	(io brd 7)
     SEB_2	 		22		  Serial I/O Expansion board 2	(io brd 8)
     SEB_3	 		23		  Serial I/O Expansion board 3	(io brd 9)
     SEB_4	 		24		  Serial I/O Expansion board 4	(io brd 10)
     SEB_5	 		25		  Serial I/O Expansion board 5	(io brd 11)
     SEB_6	 		26		  Serial I/O Expansion board 6	(io brd 12)
     SEB_7	 		27		  Serial I/O Expansion board 7	(io brd 13)
     SEB_8	 		28		  Serial I/O Expansion board 8   (io brd 14)
     SEB_9	 		29		  Serial I/O Expansion board 9   (io brd 15)
     SEB_10	 		30		  Serial I/O Expansion board 10  (io brd 16)
     SEB_11	 		31		  Serial I/O Expansion board 11  (io brd 17)
     SEB_12	 		32		  Serial I/O Expansion board 12  (io brd 18) 
     SEB_13	 		33		  Serial I/O Expansion board 13  (io brd 19) 
     SEB_14	 		34		  Serial I/O Expansion board 14  (io brd 20) 
     SEB_15	 		35		  Serial I/O Expansion board 15  (io brd 21) 
     SEB_16	 		36		  Serial I/O Expansion board 16  (io brd 22) 
     SEB_17	 		37		  Serial I/O Expansion board 17  (io brd 23) 
     SEB_18	 		38		  Serial I/O Expansion board 18  (io brd 24) 
     SEB_19	 		39		  Serial I/O Expansion board 19  (io brd 25) 
     SEB_20	 		40		  Serial I/O Expansion board 20  (io brd 26)
     SEB_21	 		41		  Serial I/O Expansion board 21  (io brd 27)
     SEB_22	 		42		  Serial I/O Expansion board 22  (io brd 28)
     SEB_23	 		43		  Serial I/O Expansion board 23  (io brd 29) 
     SEB_24	 		44		  Serial I/O Expansion board 24  (io brd 30) 
     SEB_25	 		45		  Serial I/O Expansion board 25  (io brd 31) 
     SEB_26	 		46		  Serial I/O Expansion board 26  (io brd 32) 
     SEB_27	 		47		  Serial I/O Expansion board 27  (io brd 33) 
     SEB_28	 		48		  Serial I/O Expansion board 28  (io brd 34) 
     SEB_29	 		49		  Serial I/O Expansion board 29  (io brd 35) 
     SEB_30	 		50		  Serial I/O Expansion board 30  (io brd 36)
     SEB_31	 		51		  Serial I/O Expansion board 31  (io brd 37)
     SEB_32	 		52		  Serial I/O Expansion board 32  (io brd 38)
     SEB_33	 		53		  Serial I/O Expansion board 33  (io brd 39) 
     SEB_34	 		54		  Serial I/O Expansion board 34  (io brd 40) 
     SEB_35	 		55		  Serial I/O Expansion board 35  (io brd 41) 
     SEB_36	 		56		  Serial I/O Expansion board 36  (io brd 42) 
     SEB_37	 		57		  Serial I/O Expansion board 37  (io brd 43) 
     SEB_38	 		58		  Serial I/O Expansion board 38  (io brd 44) 
     SEB_39	 		59		  Serial I/O Expansion board 39  (io brd 45) 
     SEB_40	 		60		  Serial I/O Expansion board 40  (io brd 46)
     END	 		60		  Last can device
     GLOBAL			255		  Global can address


	
	CAN PROTOCOL  (Using extended address bits)

	Frame/Length     
	Identifier 1  (Message Number:Bits 28-21)	 	(Message number)
	Identifier 2  (Device Address:Bits 20-13)	 	(TO/FROM ADDRESS)
	Identifier 3  (Unused:Bits 12-5)	 			
	Identifier 4  (Unused:Bits 4-0)
	Data Byte 1
	Data Byte 2
	Data Byte 3 
	Data Byte 4
	Data Byte 5
	Data Byte 6
	Data Byte 7
	Data Byte 8

	CAN PROTOCOL  (Using standard address bits)

	Frame/Length     
	Identifier 1 (6bits) (Message Bits: 10-5)	
	Identifier 2 (5bits) (Device Address:Bits 4-0)	 	
	Data Byte 1
	Data Byte 2
	Data Byte 3 
	Data Byte 4
	Data Byte 5
	Data Byte 6
	Data Byte 7
	Data Byte 8

	msg 	 To			From			BYTES

	1		CAR			CAR				2		Message #1: Permission to be Dispatcher
													Car number this car		if (car number < requesting Car) then permission denied
													Requesting Car

	2		CAR			CAR				1		Message #2: Request to Be Dispatcher
													Requesting Car number (every car on line must respond)
														
	3		CAR			CAR				1		Message #3: I am dispatcher
													Car number (no car responds)
														
	10		ALL			CPU				1		Message #10: Can device error
													can error byte

	11		SEB1-40		CPU				5		Message #11: Init data with software version
													master version
													master revision
													call latch mask
													call latch mask
													call latch mask

	12		CPU			ALL				2		Message #12: Software version
													Device version
													Device revision

	13		SEB1-40		CPU				3-5		Message #13: Set/Clear Output Serial Expansion Board 1-9 (SEB1-9) (bit set for each output)
												  	Output 1-8
												  	Output 9-16
												  	Output 17-24
													checksum low byte			// Car call color and brightness checksum
													checksum high byte			
													checksum low byte			// Label Checksum
													checksum high byte
													checksum byte				// Color index checksum

	14		CPU			SEB1-40			3-4		Message #14: Read Input Serial Expansion Board 1-9 (SEB1-9) (bit set for each input)
												  	Input 1-8
												  	Input 9-16
												  	Input 17-24

	15		SEB1-40		CPU				3		Message #15: Transmit back the inputs that were sent for verification
												  	Input 1-8
												  	Input 9-16
												  	Input 17-24

	16		CPU			SEB1-40			3		Message #16: Read back each input sent to device for verification
												  	Output 1-8
												  	Output 9-16
												  	Output 17-24

	44		CPU			TSVS				8		Message #44:	Voltage Input 
													low byte L1 Voltage
													high byte L1 Voltage
													low byte L2 Voltage
													high byte L2 Voltage
													low byte L3 Voltage
													high byte L3 Voltage
													low byte Door Voltage
													high byte Door Voltage

	45		CPU			TSVS				4 		Message #45:	Temperature 
													low byte external temperature
													high byte external temperature

	46 		CPU			RGBCC				1		Message 46: Request Board I/O Labels
													Byte 0:  Request flag must be set to 1

	47 		RGBCC		CPU					6		Message 47: Init Board I/O Labels
													Byte 0: I/O Location Index (0-23) Will cycle throug 0 to 23 on one request.
													Byte 1: Ascii byte 1 i/o name left to right
													Byte 2: Ascii byte 2
													Byte 3: Ascii byte 3
													Byte 4: Ascii byte 4
													Byte 5: Ascii byte 5
													Byte 6: On Color Index (not used on this bus so set to zero)
													Byte 7: Off Color Index	(not used on this bus so set to zero)

																	
	48 		CPU			any DEV				2		Message 48: Device error
													Byte 0: Error Code	(Specific code for device)
														Fault Code from RGB_CC board:
															0 = no fault
															1 = LED short red
															2 = LED short green
															3 = LED short blue
															4 = LED open red
															5 = LED open green
															6 = LED open blue
															7 = FET short red
															8 = FET short green
															9 = FET short blue
															10 = FET open red
															11 = FET open green
															12 = FET open blue
															13 = NO LED board
															14 = Stuck Button


	49		any DEV		CPU					1		Message 49: Reset Device Error
													Byte 0: Control bit 0: 1 = reset error condition
																	bit 1: 1 = clear error code buffer


	45		CPU			TSVS				2 		Message #45:	Temperature 
													low byte external temperature
													high byte external temperature


  	72			HC			CPU			7			Packet #72:  Driver board control data						 			   	  
                                                      HCB Control
															bit0: 1 = Invert Security
								                          	bit1: 1 = Flash Ssecurity
															bit2: 1 = Master Sec Enable	
															bit3: 1 = Master Hall Call Security
															bit4: 1 = Flash CodeBlue/Vip
															bits 5,6 = unused
															bit7 = Reset and init hall staions boards

													  Checksum of Light PWM Data low byte		 (up hc or up ir)  and (sec)
													  Checksum of Light PWM Data high byte	
													  Checksum of Light PWM Data low byte	   (dn hc or dn ir) and (cb or vip)
													  Checksum of Light PWM Data high byte	

													  Response: Packet 74 or request packet 77 (if not initialized)


	73			HC			CPU			7			Packet #73: Hall Call Driver Outputs Hall Call (bit set for each floor)
													  			    1st Driver Brd,    2nd Driver Brd,    3rd Driver Brd		   
									 				  Output Floors   1-4 Down/Up, 	  25-38 Down/Up, 	49-52 Down/Up	   
									 				  Output Floors   5-8 Down/Up, 	  39-32 Down/Up, 	53-56 Down/Up	   
									 				  Output Floors  9-12 Down/Up, 	  33-36 Down/Up, 	47-60 Down/Up		   
									 				  Output Floors  13-16 Down/Up,   37-40 Down/Up, 					   
									 				  Output Floors  17-20 Down/Up,   41-44 Down/Up, 			   
									 				  Output Floors  21-24 Down/Up,   45-48 Down/Up, 	
									 				  Response: Packet 74				   
													

	74			CPU			HC	  	    7		   	Packet #74: Hall Call Driver input (bit set for each floor)
													  			1st Driver Brd,    2nd Driver Brd,    3rd Driver Brd			  
									 				  Input Floors   1-4 Down/Up, 	  25-38 Down/Up, 	49-52 Down/Up	   
									 				  Input Floors   5-8 Down/Up, 	  39-32 Down/Up, 	53-56 Down/Up	   
									 				  Input Floors  9-12 Down/Up, 	  33-36 Down/Up, 	47-60 Down/Up		   
									 				  Input Floors  13-16 Down/Up,    37-40 Down/Up, 					   
									 				  Input Floors  17-20 Down/Up,    41-44 Down/Up, 			   
									 				  Input Floors  21-24 Down/Up,    45-48 Down/Up, 	
												   	  Response: Packet 75			
													  
 	75			CPU			HC	  	    8		   	Packet #75: Serial Hall Call Board Faults
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
													 

	76			GRP			HC	  	    7		   	Packet #76: Hall Call Driver and serial board version
													Command 0:
														HC 1 board version
														HC 1 board revision
														HC 2 board version
														HC 2 board revision
														HC 3 board version
														HC 3 board revision
														
													Command 1:
														HC 4 board version
														HC 4 board revision
														HC 5 board version
														HC 5 board revision
														HC 6 board version
														HC 6 board revision

													Command 2:
														HC 7 board version
														HC 7 board revision
														HC 8 board version
														HC 8 board revision
														HC 9 board version
														HC 0 board revision

													Command 3:
														HC 10 board version
														HC 10 board revision
														HC 11 board version
														HC 11 board revision
														HC 12 board version
														HC 12 board revision

													Command 4:
														HC 13 board version
														HC 13 board revision
														HC 14 board version
														HC 14 board revision
														HC 15 board version
														HC 15 board revision

													Command 5:
														HC 16 board version
														HC 16 board revision
														HC 17 board version
														HC 17 board revision
														HC 18 board version
														HC 18 board revision

													Command 6:
														HC 19 board version
														HC 19 board revision
														HC 20 board version
														HC 20 board revision
														HC 21 board version
														HC 21 board revision

													Command 7:
														HC 22 board version
														HC 22 board revision
														HC 23 board version
														HC 23 board revision
														HC 24 board version
														HC 24 board revision

 	77		HC 			CPU					8		Packet #77: Hall call driver device initialization
													       
													Init Command 0:	  (front/rear hc or front/rear ir)
													  				  1st Driver Brd,    2nd Driver Brd,    3rd Driver Brd		  
									 				  Valid Floors   1-4 Down/Up, 	  25-38 Down/Up, 	49-52 Down/Up	   
									 				  Valid Floors   5-8 Down/Up, 	  39-32 Down/Up, 	53-56 Down/Up	   
									 				  Valid Floors  9-12 Down/Up, 	  33-36 Down/Up, 	47-60 Down/Up		   
									 				  Valid Floors  13-16 Down/Up,    37-40 Down/Up, 					   
									 				  Valid Floors  17-20 Down/Up,    41-44 Down/Up, 			   
									 				  Valid Floors  21-24 Down/Up,    45-48 Down/Up, 	
										 			  			  
													Init Command 1:	  Latching bits
													  				  1st Driver Brd,    2nd Driver Brd,    3rd Driver Brd		  
									 				  Valid Floors   1-4 Down/Up, 	  25-38 Down/Up, 	49-52 Down/Up	   
									 				  Valid Floors   5-8 Down/Up, 	  39-32 Down/Up, 	53-56 Down/Up	   
									 				  Valid Floors  9-12 Down/Up, 	  33-36 Down/Up, 	47-60 Down/Up		   
									 				  Valid Floors  13-16 Down/Up,    37-40 Down/Up, 					   
									 				  Valid Floors  17-20 Down/Up,    41-44 Down/Up, 			   
									 				  Valid Floors  21-24 Down/Up,    45-48 Down/Up, 	
										 			  
										 			Init Command 2:
										 			  Master Version Number
													  Master Revision Number
													  HC On Brightness	 (to remain backward compatible)
													  HC Off Brightness	 (to remain backward compatible)
													  Special Control
														bit 0: 1 = all boards non-latching
														bit 1-7: unused
		
													Init Command 3:
													  Red Up On Time	  (up hc or up ir)
													  Red Up Off Time
													  Green Up On Time
													  Green Up Off Time
													  Blue Up On Time
													  Blue Up Off Time

													Init Command 4:
													  Red Down On Time		 (dn hc or dn ir)
												  	  Red Down Off Time
												  	  Green Down On Time
												  	  Green Down Off Time
												  	  Blue Down On Time
												  	  Blue Down Off Time
												  	  
													Init Command 5:
										 			  Security Floor Masks     	 1-8,   		  25-32, 49-56 					   
										 			  Security Floor Masks   	9-16,   		  33-40, 57-60 					   
										 			  Security Floor Masks     17-24,   		  41-48, 49-56 					   
													  Sec Red On Time
													  Sec Green On Time
													  Sec Blue On Time

													Init Command 6:
											 	  	  cb/vip Floor Masks     	1-8,   		  	  25-32, 49-56 					   
											 	  	  cb/vip Floor Masks   		9-16,   		  33-40, 57-60 					   
											 	  	  cb/vip Floor Masks     	17-24,   		  41-48, 49-56 					   
												  	  cb/vip Red On Time	   (cb or vip)
												  	  cb/vip Green On Time
												  	  cb/vip Blue On Time

													  Response: Packet 78
													  
	78			CPU			HC				8		Packet #78: Hall call driver device initialization response packet
 													
 													Init Command 0:
 														Response Byte: 30H
 													
 													Init Command 1:
  														Response Byte:	31H
 														
 													Init Command 2:
  														Response Byte:	32H
														Device Version
 														Device Revision
 														
 													Init Command 3:
 														Response Byte:	33H

 													Init Command 4:
 														Response Byte:	34H

 													Init Command 5:
 														Response Byte:	35H
 														
 													Init Command 6:
 														Response Byte:	36H

 													Init Command 7:
 														Response Byte:	37H
 														All Commands Received: 62H
 														
 													Init Command 8:
 														Response Byte: 38H
 														Init Request Byte: 59H = init, else 0 = not init request
 													
													All unused bytes set to 0
													

	79			HC			GRP			8			Packet #79: Hall Call Station Security Mask, Color and Brightness 
													 	Command 0:
											 			  Security Floor Masks     	 1-8,   		  25-32, 49-56 					   25-30 
											 			  Security Floor Masks   	9-16,   		  33-40, 57-60 					   25-28 
											 			  Security Floor Masks     17-24,   		  41-48, 49-56 					   29-30 
														  HC On Brightness
														  HC Off Brightness

													 	Command 1:
														  Red On Time  (up hc or up ir)
														  Red Off Time (up)
														  Green On Time	(up)
														  Green Off Time (up)
														  Blue On Time (up)
														  Blue Off Time (up)

													 	Command 2:
														  Red Down On Time (dn hc or dn ir)
														  Red Down Off Time
														  Green Down On Time
														  Green Down Off Time
														  Blue Down On Time
														  Blue Down Off Time

													 	Command 3:
														  Sec Red On Time
														  Sec Green On Time					
														  Sec Blue On Time
														  cb/vip Red On Time  (cb or vip)
														  cb/vip Green On Time
														  cb/vip Blue On Time
														  
														Unused bytes are set to zero
								


	90			HC			CPU			7			Packet #90: Hall Call Driver Aux Outputs Hall Call (bit set for each floor)
																		   1st Driver Brd,    2nd Driver Brd, 	3rd Driver Brd			  
									 			   	  	cb/vip Output Floors     1-8 cb/vip,   25-32 Up(or RU), 	49-56 Up				   
									 			   	  	cb/vip Output Floors    9-16 cb/vip,   33-40 Up(or RU), 	57-60 RUp/57-60 Up	   
									 			   	  	cb/vip Output Floors   17-24 cb/vip,   41-48 Up(or RU), 	49-56 RUp			   
			

	91			CPU			HC	  	    6		   	Packet #91: Hall Call Driver Aux input (bit set for each floor)
																			1st Driver Brd,     2nd Driver Brd,    	3rd Driver Brd			  
												   	  	Aux Floor Input   1-8 Aux (or RAux), 25-32 Aux(or RAux),	49-56 Aux			
													  	Aux Floor Input   9-16 Aux(or RAux), 33-40 Aux(or RAux), 	57-60 RAux/57-60 Aux
												   	  	Aux Floor Input  17-24 Aux(or RAux), 41-48 Aux(or RAux), 	49-56 RAux			


	80-87 Reserved for file transfer on can bus	
	110-115 ANY			CPU				Packets reserved for testing.

*/														


#define c_max_grcan_dev 65	// 55 devices 
// #define c_max_grcan_io_brd 40	// 40 i/o boards *** use nmb_hcio_brds instead
#define c_max_grcan_ser_array (nmb_hcio_brds * 3)

#define c_nmb_hcb 48


int16 grcan_addr[c_max_grcan_dev] = 
{ 0,10,20,
 11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,30,31,
 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
 52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,
 255,72};


#define  c_grcan_START	  1		  // Can start index
#define  c_grcan_VS		  1		  // Voltage Sensor
#define  c_grcan_FI_DPY   2		  // Serial PI and Lantern (global address for all pi and lantern devices)
#define	 c_grcan_SEB_1	  3		  // Serial I/O Expansion board 1	
#define	 c_grcan_SEB_2	  4		  // Serial I/O Expansion board 2	
#define	 c_grcan_SEB_3	  5		  // Serial I/O Expansion board 3	
#define	 c_grcan_SEB_4	  6		  // Serial I/O Expansion board 4	
#define	 c_grcan_SEB_5	  7		  // Serial I/O Expansion board 5	
#define	 c_grcan_SEB_6	  8		  // Serial I/O Expansion board 6	
#define	 c_grcan_SEB_7	  9		  // Serial I/O Expansion board 7	
#define  c_grcan_SEB_8	 10		  // Serial I/O Expansion board 8   
#define  c_grcan_SEB_9	 11		  // Serial I/O Expansion board 9   
#define  c_grcan_SEB_10	 12		  // Serial I/O Expansion board 10  
#define  c_grcan_SEB_11	 13		  // Serial I/O Expansion board 11  
#define  c_grcan_SEB_12	 14		  // Serial I/O Expansion board 12  
#define  c_grcan_SEB_13	 15		  // Serial I/O Expansion board 13  
#define  c_grcan_SEB_14	 16		  // Serial I/O Expansion board 14  
#define  c_grcan_SEB_15	 17		  // Serial I/O Expansion board 15  
#define  c_grcan_SEB_16	 18		  // Serial I/O Expansion board 16  
#define  c_grcan_SEB_17	 19		  // Serial I/O Expansion board 17  
#define  c_grcan_SEB_18	 20		  // Serial I/O Expansion board 18  
#define  c_grcan_SEB_19	 21		  // Serial I/O Expansion board 19  
#define  c_grcan_SEB_20	 22		  // Serial I/O Expansion board 20  
#define  c_grcan_SEB_21	 23		  // Serial I/O Expansion board 21  
#define  c_grcan_SEB_22	 24		  // Serial I/O Expansion board 22  
#define  c_grcan_SEB_23	 25		  // Serial I/O Expansion board 23  
#define  c_grcan_SEB_24	 26		  // Serial I/O Expansion board 24  
#define  c_grcan_SEB_25	 27		  // Serial I/O Expansion board 25  
#define  c_grcan_SEB_26	 28		  // Serial I/O Expansion board 26  
#define  c_grcan_SEB_27	 29		  // Serial I/O Expansion board 27  
#define  c_grcan_SEB_28	 30		  // Serial I/O Expansion board 28  
#define  c_grcan_SEB_29	 31		  // Serial I/O Expansion board 29  
#define  c_grcan_SEB_30	 32		  // Serial I/O Expansion board 30  
#define  c_grcan_SEB_31	 33		  // Serial I/O Expansion board 31  
#define  c_grcan_SEB_32	 34		  // Serial I/O Expansion board 32  
#define  c_grcan_SEB_33	 35		  // Serial I/O Expansion board 33  
#define  c_grcan_SEB_34	 36		  // Serial I/O Expansion board 34  
#define  c_grcan_SEB_35	 37		  // Serial I/O Expansion board 35  
#define  c_grcan_SEB_36	 38		  // Serial I/O Expansion board 36  
#define  c_grcan_SEB_37	 39		  // Serial I/O Expansion board 37  
#define  c_grcan_SEB_38	 40		  // Serial I/O Expansion board 38  
#define  c_grcan_SEB_39	 41		  // Serial I/O Expansion board 39  
#define  c_grcan_SEB_40	 42		  // Serial I/O Expansion board 40  
#define  c_grcan_HC_1	 43		  // Hall Call Driver board 1
#define  c_grcan_HC_2	 44		  // Hall Call Driver board 1
#define  c_grcan_HC_3	 45		  // Hall Call Driver board 1
#define  c_grcan_HC_4	 46		  // Hall Call Driver board 1
#define  c_grcan_HC_5	 47		  // Hall Call Driver board 1
#define  c_grcan_HC_6	 48		  // Hall Call Driver board 1
#define  c_grcan_HC_7	 49		  // Hall Call Driver board 1
#define  c_grcan_HC_8	 50		  // Hall Call Driver board 1
#define  c_grcan_HC_9	 51		  // Hall Call Driver board 1
#define  c_grcan_HC_10	 52		  // Hall Call Driver board 1
#define  c_grcan_HCB_1	 15		  // Hall Call Board 1
#define  c_grcan_HCB_2	 16		  // Hall Call Board 2
#define  c_grcan_HCB_3	 17		  // Hall Call Board 3
#define  c_grcan_HCB_4	 18		  // Hall Call Board 4
#define  c_grcan_HCB_5	 19		  // Hall Call Board 5
#define  c_grcan_HCB_6	 20		  // Hall Call Board 6
#define  c_grcan_HCB_7	 21		  // Hall Call Board 7
#define  c_grcan_HCB_8	 22		  // Hall Call Board 8
#define  c_grcan_HCB_9	 23		  // Hall Call Board 9
#define  c_grcan_HCB_10	 24		  // Hall Call Board 10
#define  c_grcan_HCB_11	 25		  // Hall Call Board 11
#define  c_grcan_HCB_12	 26		  // Hall Call Board 12
#define  c_grcan_HCB_13	 27		  // Hall Call Board 13
#define  c_grcan_HCB_14	 28		  // Hall Call Board 14
#define  c_grcan_HCB_15	 29		  // Hall Call Board 15
#define  c_grcan_HCB_16	 30		  // Hall Call Board 16
#define  c_grcan_HCB_17	 31		  // Hall Call Board 17
#define  c_grcan_HCB_18	 32		  // Hall Call Board 18
#define  c_grcan_HCB_19	 33		  // Hall Call Board 19
#define  c_grcan_HCB_20	 34		  // Hall Call Board 20
#define  c_grcan_HCB_21	 35		  // Hall Call Board 21
#define  c_grcan_HCB_22	 36		  // Hall Call Board 22
#define  c_grcan_HCB_23	 37		  // Hall Call Board 23
#define  c_grcan_HCB_24	 38		  // Hall Call Board 24
#define  c_grcan_HCB_25	 39		  // Hall Call Board 25
#define  c_grcan_HCB_26	 40		  // Hall Call Board 26
#define  c_grcan_HCB_27	 41		  // Hall Call Board 27
#define  c_grcan_HCB_28	 42		  // Hall Call Board 28
#define  c_grcan_HCB_29	 43		  // Hall Call Board 29
#define  c_grcan_HCB_30	 44		  // Hall Call Board 30
#define  c_grcan_HCB_31	 45		  // Hall Call Board 31
#define  c_grcan_HCB_32	 46		  // Hall Call Board 32
#define  c_grcan_HCB_33	 47		  // Hall Call Board 33
#define  c_grcan_HCB_34	 48		  // Hall Call Board 34
#define  c_grcan_HCB_35	 49		  // Hall Call Board 35
#define  c_grcan_HCB_36	 50		  // Hall Call Board 36
#define  c_grcan_HCB_37	 51		  // Hall Call Board 37
#define  c_grcan_HCB_38	 52		  // Hall Call Board 38
#define  c_grcan_HCB_39	 53		  // Hall Call Board 39
#define  c_grcan_HCB_40	 54		  // Hall Call Board 40
#define  c_grcan_HCB_41	 55		  // Hall Call Board 41
#define  c_grcan_HCB_42	 56		  // Hall Call Board 42
#define  c_grcan_HCB_43	 57		  // Hall Call Board 43
#define  c_grcan_HCB_44	 58		  // Hall Call Board 44
#define  c_grcan_HCB_45	 59		  // Hall Call Board 45
#define  c_grcan_HCB_46	 60		  // Hall Call Board 46
#define  c_grcan_HCB_47	 61		  // Hall Call Board 47
#define  c_grcan_HCB_48	 62		  // Hall Call Board 48
#define  c_grcan_END	 62		  // Last can device
#define  c_grcan_GLOBAL	 63		  // Global can address


#define IODATA_GRCAN_START 0		// 1 - 1
#define HCB_IODATA_GRCAN_START 12 * 3

int32 grcan_debug_1;
int32 grcan_debug_2;
int32 grcan_debug_3;
int32 grcan_debug_4;
int32 grcan_debug_5;
int32 grcan_debug_6;

#define c_Freeze_Start		0x01	// put chip in freeze mode
#define c_Freeze_Stop		0x00	// Takes chip out of freeze mode

/*
// write only addresses
#define c_Tx_Frame_Reg 16			// Transmit Frame Register
#define c_Tx_Id_Data 17				// Start of Trasmit id and data
#define c_Tx_Id_1 17				// Transmit ID
#define c_Tx_Id_2 18				// Transmit ID
#define c_Tx_Id_3 19				// Transmit ID
#define c_Tx_Id_4 20				// Transmit ID
#define c_Tx_Data_1 21				// Transmit Data
#define c_Tx_Data_2 22				// Transmit Data
#define c_Tx_Data_3 23				// Transmit Data
#define c_Tx_Data_4 24				// Transmit Data
#define c_Tx_Data_5 25				// Transmit Data
#define c_Tx_Data_6 26				// Transmit Data
#define c_Tx_Data_7 27				// Transmit Data
#define c_Tx_Data_8 28				// Transmit Data
#define c_EFF 0x80					// Extended Frame Format
#define c_RTR 0x40					// Remote Transmission Request

#define c_Rx_Fifo 32				// Receive Fifo Start
#define c_Tx_Buff 96				// Transmit Buffer Start
#define c_Free_Ram 110				// Free Rame Start
#define c_Accept_Code_0 16			// Acceptance Code
#define c_Accept_Code_1 17
#define c_Accept_Code_2 18
#define c_Accept_Code_3 19
#define c_Accept_Mask_0 20			// Acceptance Mask
#define c_Accept_Mask_1 21
#define c_Accept_Mask_2 22
#define c_Accept_Mask_3 23

*/
int16 grcan_put_dev_msg_req (int16 dev, int8 msg);
int8 grcan_get_dev_msg_req (int16 dev);
void init_grcan_bus (void);
int16 get_comm_status (int16 board_nmb);
void clr_grcan_dev_comm_loss (void);
uint16 calc_ioname_can_cksum (unsigned char devnmb);
void send_hc_dvr_req (int16 cmd);
void wrong_grcan_int (void);
void grcan_bus_off_and_warning_int (void);
void grcan_error_int (void);
void grcan_rcv_int (void);
void grcan_rcv_fifo_warning_int (void);
void grcan_rcv_fifo_overflow_int (void);
void grcan_trm_int_b8 (void);
void grcan_trm_int_b9 (void);
void grcan_trm_int_b10 (void);
void grcan_trm_int_b11 (void);
void grcan_control (void);
void receive_grcan_in (int16 BufIx);
void unpack_grcan_in (void);
int16 Grcan_ChipFreezeMode(unsigned char set);
void tx_grcan_msg(int16 TxBufIx);
void grcan_transmit (int16 TxBufIx);
uint16 calc_ioname_grcan_cksum (unsigned char devnmb);
int16 IsGrcTxBufFul(void);
void pack_grcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req);

// Declarations for Serial I/O Boards

#define c_io_ix_mult 18

union tag_ser_grc {

	union 
	{
		uint8 iob[3];
		struct
		{
			uint8 io8:1;
			uint8 io7:1;
			uint8 io6:1;
			uint8 io5:1;
			uint8 io4:1;
			uint8 io3:1;
			uint8 io2:1;
			uint8 io1:1;
		}B[3];
		struct
		{
			uint8 nib2:4;
			uint8 nib1:4;
		}N[3];
	} dev [nmb_hcio_brds];	
	
	struct
	{
		uint8 ihc[6];
		uint8 ohc[6];
		uint8 axihc[3];
		uint8 axohc[3];
	} devhc [nmb_hcio_brds/6];
	
	unsigned char brd_io [c_max_grcan_ser_array];
}serial_grc;


union tag_sync_grc {
	struct
	{
		unsigned char iob[3];
		unsigned char reflect_data;
		unsigned char rtx_cnt;
	} dev[nmb_hcio_brds];
	
	struct
	{
		unsigned char ihc[6];
		unsigned char ohc[6];
		uint8 axihc[3];
		uint8 axohc[3];
		unsigned char reflect_hc_data;
		unsigned char rtx_hc_cnt;
	} devhc [nmb_hcio_brds/6];
	
} sync_grc;


union tag_hcb
{
	uint8 iob;
	struct
	{
		uint8 canc_dn:1;
		uint8 canc_up:1;
		uint8 set_dn:1;
		uint8 set_up:1;
		uint8 out_dn:1;
		uint8 out_up:1;
		uint8 in_dn:1;
		uint8 in_up:1;
	}B;
} hcb[c_nmb_hcb]; 


struct 
{
	union 
	{
		uint8 iob;
		struct
		{
			uint8 canc_dn:1;
			uint8 canc_up:1;
			uint8 set_dn:1;
			uint8 set_up:1;
			uint8 out_dn:1;
			uint8 out_up:1;
			uint8 in_dn:1;
			uint8 in_up:1;
		}B;
	} io_stat;
	uint8 reflect_data;
	uint8 rtx_cnt;
} sync_hcb[c_nmb_hcb];


// Can fault codes from devices
/*
#define e_txb2_timeout 1
#define e_txb1_timeout 2
#define e_txb0_timeout 3
#define e_inv_pack 4
#define e_txb2_inv_len 5
#define e_txb1_inv_len 6
#define e_txb0_inv_len 7
#define e_error_int16 8
#define e_txb2_int16 9
#define e_txb1_int16 10
#define e_txb0_int16 11
#define e_inv_unpk 12
#define e_inv_msg 13
*/

#define grcan_que_len 3

struct tag_grcan_dev {
		unsigned char online;
		unsigned char online_cnt;
		unsigned char version;
		unsigned char revision;
		unsigned char msg_req[grcan_que_len];
		unsigned char can_error;
		unsigned char can_error_data;
		unsigned char label_ix;
		unsigned char label_req;
		unsigned char dev_error;
		unsigned char dev_error_data[2];
		unsigned char local_dev_comm_loss;
		unsigned char color_ix;
		unsigned char color_ix_req;
		unsigned char io_color_ix_chksum; 
		uint16 tx_cnt;
		uint16 rx_cnt;
		uint16 ioname_chksum;
} grcan_dev[c_max_grcan_dev];

int16 grcan_dev_req = 0;
int16 grcan_msg_req = 0;
int16 grcan_msg_cmd = 0;
int16 grcan_msg_cnt = 0;

int16 grcan_color_send_ix = 0;


bool grcan_car_online[car_size];
bool car_on_grcan = 0;

unsigned char grc_ioset[c_max_grcan_ser_array];
unsigned char grc_ioclr[c_max_grcan_ser_array];

int16 ver_dev_ix = 0;

unsigned char grcan_dev_list [c_max_grcan_dev];	// This is the list of can devices used 
int16 nmb_grcan_dev;
int16 nmb_dvr_brds = 0;
int16 grcan_ram_seq = 0;
int16 grcan_init_count = 0;
int16 grcan_init_mode_cnt = 0;
bool grcan_in_chip_reset = false;
int32 grcan_status = 0;
int16 grcan_mode = 0;
int16 grcan_int_status = 0;
int16 grcan_error = 0;
/*
	Can Error  
	Bit			Description
	00000001	Chip would not come out of reset mode during initialization
	00000010	Chip would not go into reset mode during initialization
	00000100	Can Bus off error
	00001000	Receive Buffer not released
	00010000	Transmit Buffer not released
	00100000	Transmit length error
*/
int16 grcan_int_count = 0;
int16 re_init_grcan = 0;
int16 grcan_arb_lost_cap=0;
int16 grcan_err_code_cap=0;
int16 grcan_tx_err_cnt=0;
int16 grcan_rx_err_cnt = 0;
int16 grcan_rcv_fifo_cnt = 0;
int16 grcan_rcv_int_fail_cnt = 0;
int16 grcan_WCI_cnt=0;	// Wrong can interrupt
int16 grcan_BOI_cnt=0;	// Bus Off and tx/rx warning interrupt count
int16 grcan_ERI_cnt=0;	// Error Interrupt count
int16 grcan_RI_cnt=0;		// Receive fifo interrupt count
int16 grcan_FWI_cnt=0;	// Fifo warning interrupt count
int16 grcan_FOI_cnt=0;	// Fifo Overflow interrupt count
int16 grcan_TI8_cnt=0;	// Transmit buffer 8 interrupt count
int16 grcan_TI9_cnt=0;	// Transmit buffer 9 interrupt count
int16 grcan_TI10_cnt=0;	// Transmit buffer 10 interrupt count
int16 grcan_TI11_cnt=0;	// Transmit buffer 11 interrupt count
int16 grcan_ack_error_cnt = 0;
int16 grcan_bus_off_cnt = 0;
int16 grcan_bus_idle_cnt = 0;

int16 grcan_tx_B08_fail_cnt = 0;
int16 grcan_tx_B09_fail_cnt = 0;
int16 grcan_tx_B10_fail_cnt = 0;
int16 grcan_tx_B11_fail_cnt = 0;


int16 grcan_data_req = 0;				// Flag to signal that data has been requested by a device
int16 grcan_data_req_cnt = 0;

#define c_grcan_tx_buf_siz 30

struct tag_grcan_tx {
		unsigned char len;
		unsigned char ex_id;	// flag for extended id mode
		unsigned char one_tx;	// Transmit one packet (no retransmit)
		unsigned char rtr;		// Remote Transmission Request
		unsigned char srr;		// Substitute Remote Request
		unsigned char msg;		// Message
		unsigned char addr;		// Address
		unsigned char id2;
		unsigned char id3;
		unsigned char data [8];
		} grcan_tx[c_grcan_tx_buf_siz+1];

int16 grc_tpix = 0;					// can transmit buffer put index
int16 grc_tgix = 0;					// can transmit buffer get index
int16 grc_availbuf = 0;				// available can transmit buffer
bool grcan_tx_complete[16] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int16 GrcanTxBufIx = 8;
		
		
#define c_grcan_rx_buf_siz 30

struct tag_grcan_rx {
		unsigned char valid_msg;	// 1 when valid message in buffer
		unsigned char len;
		unsigned char ex_id;	// flag for extended id mode
		unsigned char rtr;		// Remote Transmission Request
		unsigned char msg;		// Message
		unsigned char addr;		// Address
		unsigned char id2;
		unsigned char id3;
		unsigned char data [8];
		uint16 time;
		} grcan_rx[c_grcan_rx_buf_siz + 1];
		
int16 grc_rpix = 0;					// can transmit buffer put index
int16 grc_rgix = 0;					// can transmit buffer get index

#define c_hc_brds_per_dvr 24			// Number of hall call boards per driver board

// hall call driver board structure 

int16 hcbd_flt_ix = 0;
int16 hcb_flt_tbl[63] = {0, f_hcb_tx_dn,		// 1
							f_hcb_tx_up,	   	// 2
							f_hcb_rx_dn,	   	// 3
							f_hcb_rx_up,	   	// 4
							f_hcb_stuck_up,	   	// 5
							f_hcb_stuck_dn,	   	// 6
							f_hcb_led_open_dn, 	// 7
							f_hcb_led_open_up, 	// 8
							f_hcb_led_short_dn,	// 9
							f_hcb_led_short_up,	// 10
							f_hcb_fet_open_dn, 	// 11
							f_hcb_fet_open_up, 	// 12
							f_hcb_fet_short_dn,	// 13
							f_hcb_fet_short_up,	// 14
							f_hcb_inv_fl,		// 15
							f_hcb_no_com,		// 16
							f_hcb_dev_reset,	// 17
							f_hcb_dn_led_brd,	// 18
							f_hcb_up_led_brd,	// 19
							f_hcb_led_short_red_up,		// 20
							f_hcb_led_short_green_up,	// 21
							f_hcb_led_short_blue_up,	// 22
							f_hcb_led_short_blue_dn	,	// 23
							f_hcb_led_short_green_dn,	// 24
							f_hcb_led_short_red_dn	,	// 25
							f_hcb_led_open_red_up	,	// 26
							f_hcb_led_open_green_up ,	// 27
							f_hcb_led_open_blue_up	,	// 28
							f_hcb_led_open_blue_dn	,	// 29
							f_hcb_led_open_green_dn ,	// 30
							f_hcb_led_open_red_dn	,	// 31
							f_hcb_fet_short_red_up	,	// 32
							f_hcb_fet_short_green_up,	// 33
							f_hcb_fet_short_blue_up	,	// 34
							f_hcb_fet_short_blue_dn	,	// 35
							f_hcb_fet_short_green_dn,	// 36
							f_hcb_fet_short_red_dn	,	// 37
							f_hcb_fet_open_red_up	,	// 38
							f_hcb_fet_open_green_up ,	// 39
							f_hcb_fet_open_blue_up	,	// 40
							f_hcb_fet_open_blue_dn	,	// 41
							f_hcb_fet_open_green_dn ,	// 42
							f_hcb_fet_open_red_dn,		// 43
							f_hcb_low_supply_voltage,	// 44
							f_hcb_up_input_overload,	// 45
							f_hcb_dn_input_overload,	// 46
							f_hcb_xu_input_overload,	// 47
							f_hcb_xd_input_overload,	// 48
							f_inv_fault,				// 49
							f_inv_fault,				// 50
							f_inv_fault,				// 51
							f_inv_fault,				// 52
							f_inv_fault,				// 53
							f_inv_fault,				// 54
							f_inv_fault,				// 55
							f_inv_fault,				// 56
							f_inv_fault,				// 57
							f_inv_fault,				// 58
							f_inv_fault,				// 59
							f_inv_fault,				// 60
							f_inv_fault,				// 61
							f_inv_fault,				// 62
							};  				

#define c_hc_brds_per_dvr 24			// Number of hall call boards per driver board

struct tag_hc_dvr_brd {
		unsigned char status_1;
		unsigned char prev_status_1;			// memory of previous fault
		unsigned char status_2;	
		unsigned char online;						// set to 1 if one line	
		unsigned char fault[c_hc_brds_per_dvr];
		unsigned char prev_fault[c_hc_brds_per_dvr];		// memory of previous faults
		unsigned char version[c_hc_brds_per_dvr];
		unsigned char revision[c_hc_brds_per_dvr];	
		unsigned char init_seq;								// serial hall call driver board init sequence
		unsigned char init_count;							// try to init hall call board for 5 tries
		unsigned char upd_seq;								// update sequence
		unsigned char color_seq;							// color update sequence
		unsigned char riser;								// riser: 0 = std, 1 = ir, 2= cb, 3 = vip  
		} hc_dvr_brd[nmb_hc_dvr_brds];
		
/* 													Status 1:
																	Bits:
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
																Bits 4-7:	Unused    

													fault codes :
																		0  - none
																		1  - tx fault down to below floor
																		2  - tx fault up to above floor
																		3  - rx fault down from below floor
																		4  - rx fault up to above floor
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
*/

int16 HcbControl[nmb_hc_dvr_brds];			// Hall Call Button Control 
								//							bit0: 1 = flash Security
								//                          bit1: 1 = Reset HCB Stations
								//							bit2: 1 = Master Sec Enable	
								//							bit3: 1 = Master Hall Call Security
								//							bits 4-6 = unused
								//							bit7 = Reset and init hall staions boards
								


int16 dispatcher_count = 0;			// dispatcher counter to determine dispatcher

	
static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		
uint8 shift_mask[2] = {0x0F,0xF0};

	
	
// CAN_A Int Base = 127
// CAN_B Int Base = 157 
// CAN_C Int Base = 178
// CAN_D Int Base = 199

#define GR_CAN CAN_C				// Use C for Group CAN
#define GR_CAN_INT_BASE 178


// Simple que with length of 3 for each device
//--------------------------------------
// Put device message request on que
//--------------------------------------
int16 grcan_put_dev_msg_req (int16 dev, int8 msg)
{
	int16 ix = 0;

	while(ix < grcan_que_len)
	{      // look for and open spot on the que
		if (grcan_dev[dev].msg_req[ix] == 0)
		{         // found an open spot so place Packet on que
			grcan_dev[dev].msg_req[ix] = msg;
			return 1;	// data on the que
		}
		else     // index to next spot (if non found, throw the data away)
			ix++;
	}
	return 0;		// que is full
}

//--------------------------------------
// Get device message request from que
//--------------------------------------
int8 grcan_get_dev_msg_req (int16 dev)
{
	int16 ix = 0;
	int8 msg = 0;			// return 0 if no Packet on queue

	while (ix < grcan_que_len)
	{
		if (grcan_dev[dev].msg_req[ix] != 0) 
		{      	// Packet for this device found
			msg = grcan_dev[dev].msg_req[ix];
			grcan_dev[dev].msg_req[ix] = 0;
			break;
		}
		else
			ix++;
	}

	// shift que data to the top (start where ix left off)

	while (ix < (grcan_que_len-1))
	{
		if (grcan_dev[dev].msg_req[ix+1] != 0)
		{   // Packet was taken from the middle of the que so shift data up
			grcan_dev[dev].msg_req[ix] = grcan_dev[dev].msg_req[ix+1];
			grcan_dev[dev].msg_req[ix+1] = 0;
			ix++;
		}
		else
			break;
	}
	return (msg);
}

// ---------------------------------------------------------------------
// Get Communications Status
//       0 = no comm, 1 = comm with board,  2 = comm on previous board
// ---------------------------------------------------------------------
int16 get_comm_status (int16 board_nmb)
{
	int16 comm_status = 0;
	int16 devnmb = 0;
		
	if ((HC_IO_board[board_nmb] & 0x40) != 0)
	{			// Individual Serial Hall call boards
		devnmb = board_nmb + c_grcan_SEB_1;
		if ((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_END))
			comm_status = (grcan_dev[devnmb].online == 1);
	}
	else if ((HC_IO_board[board_nmb] & 0x02) != 0)
	{			// Serial Hall call driver boards
		if ((board_nmb%6) == 0) 	// using a driver board
		{
			devnmb = (board_nmb/6) + c_grcan_HC_1;		
			comm_status = (grcan_dev[devnmb].online == 1);
		}
		else
			comm_status = 2;
	}
	else
	{
		devnmb = board_nmb + c_grcan_SEB_1;
		if ((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_END))
			comm_status = (grcan_dev[devnmb].online == 1);
	}
	return(comm_status);
}



// **************************************************
// ** Clear can device comm loss for local devices
// **************************************************

// clear comm loss error latch for RGB Car call driver board local devices
void clr_grcan_dev_comm_loss (void)
{
	int16 i;
	for (i = 0;i<c_max_grcan_dev; i++)
	{
		grcan_dev[i].local_dev_comm_loss = 0;
	}
}

//*******************************************************************
// Sends the freeze bit to the chip
// Value of 1 puts the chip in freeze mode
// Value of 0 takes is out of freeze mode
// Have to see the freeze mode acknowledge bit gets set
//*******************************************************************
int16 Grcan_ChipFreezeMode(unsigned char set)
{
	
	if (grcan_in_chip_reset == true)
		return 0;

	grcan_in_chip_reset = true;

	grcan_init_mode_cnt = 0;

	GR_CAN.MCR.B.MDIS = 0;			// Enable module after reset
	GR_CAN.MCR.B.FRZ = 1;			// Set or clear the freeze bit
	GR_CAN.MCR.B.HALT = set;			// Clear halt mode

	
	while (grcan_init_mode_cnt < 200)
	{
		if (GR_CAN.MCR.B.FRZACK == set)
		{
			grcan_in_chip_reset = false;
			return 1;
		}
		grcan_init_mode_cnt++;
	}
	grcan_in_chip_reset = false;
	return 0;		// Failed to initialize
}

// **************************************************
// ********* Send hall call driver request **********
// **************************************************

void send_hc_dvr_req (int16 cmd)
{
	int16 i;
	uint8 dev_ix = 0;
	
	i = 0;
	while (i < nmb_grcan_dev) 
	{
		if ((grcan_dev_list[i] >= c_grcan_HC_1) && (grcan_dev_list[i] <= c_grcan_HC_10))
		{
			dev_ix = grcan_dev_list[i] - c_grcan_HC_1;
			if (cmd == 0)
				HcbControl[dev_ix] |= 0x80;
			else
				put_pkt_req(79,grcan_dev_list[i],GRCAN);
		}
		i++;
	}
}



// **************************************************
// ****** Calculate ioname can device checksum ******
// **************************************************
uint16 calc_ioname_grcan_cksum (unsigned char devnmb)
{
	uint16 chksum = 0;
	int16 h,i,j;
	int16 array_start, io_name_start;
	int16 iodev_ix = 0;
	int16 nmb_io_labels = 0;

	iodev_ix = (int16)(devnmb-c_grcan_SEB_1);

	array_start = (int16)((IODATA_GRCAN_START + iodev_ix) * 3); 		// input array
	for (i=0;i<=2;i++)
	{
		for(j=0;j<=7;j++)
		{
			if ((gio_send_name[array_start+i] & bittbl[j & 0x07]) != 0)
			{
				io_name_start = (array_start + i) * 8;
				for (h=0;h<=4;h++)
					chksum += hcioname[io_name_start + j][h];
				nmb_io_labels++;
			}
		}
	}
	for(i=nmb_io_labels;i<12;i++)
		chksum += 0x00A0;			// all spaces (0x20) times 5
	return(chksum);
}

// **************************************************
// ****  Initialize the ECAN 1000 CAN interface Board
// **************************************************


void init_grcan_bus (void)
{	 
	int16 h,i,j,k;
	int16 reset_count;
	nmb_grcan_dev = 0;
	nmb_dvr_brds = 0;
	grc_tpix = 0;
	grc_tgix = 0;
	update_pi_lant = 0;


	if (cons[Voltage_Sensor] == 3)
	{
		grcan_dev_list[nmb_grcan_dev] = c_grcan_VS;		
		nmb_grcan_dev++;
	}

//	grcan_dev_list[nmb_grcan_dev] = c_grcan_FI_DPY;		
//	nmb_grcan_dev++;

	j = 0;
	h = 0;
	for (i=0;i<42;i++)			
	{
		if (HC_IO_board[i] != 0)
		{		 // Set device in device list
			if ((HC_IO_board[i] & 0x40) != 0)
			{   	// Individual Serial Board for each floor
				if (j >= 36)
				{
					
					if ((gio_valid[i] & 0x0F) != 0)
					{
						grcan_dev_list[nmb_grcan_dev] = (uint8)(c_grcan_HCB_1 + (j - 36));
						nmb_grcan_dev ++;					  // ^^starts after c_grcan_SEB_12 so must index from c_grcan_SEB_1, IO array 36
					}
					j++;
					if ((gio_valid[i] & 0xF0) != 0)
					{
						grcan_dev_list[nmb_grcan_dev] = (uint8)(c_grcan_HCB_1 + (j - 36));
						nmb_grcan_dev ++;
					}
					j++;
					k = ((c_grcan_HCB_1 + (j - 36))/2);
					serial_grc.brd_io[k] = GIO_In_Out[k];
					debug3++;
				}
				else
					j++;
			}
			else if ((HC_IO_board[i] & 0x02) != 0)
			{			// Serial Hall call driver boards
				if ((i%6) == 0) 	// using a driver board
				{
							
					grcan_dev_list[nmb_grcan_dev] = (uint8)(c_grcan_HC_1 + h);
					nmb_grcan_dev ++;
					nmb_dvr_brds++;
					k=h*c_io_ix_mult;
					serial_grc.brd_io[k] = GIO_In_Out[k];
					serial_grc.brd_io[k+1] = GIO_In_Out[k+1];
					serial_grc.brd_io[k+2] = GIO_In_Out[k+2];
					serial_grc.brd_io[k+3] = GIO_In_Out[k+3];
					serial_grc.brd_io[k+4] = GIO_In_Out[k+4];
					serial_grc.brd_io[k+5] = GIO_In_Out[k+5];
					serial_grc.brd_io[k+6] = GIO_In_Out[k+6];
					serial_grc.brd_io[k+7] = GIO_In_Out[k+7];
					serial_grc.brd_io[k+8] = GIO_In_Out[k+8];
					serial_grc.brd_io[k+9] = GIO_In_Out[k+9];
					serial_grc.brd_io[k+10] = GIO_In_Out[k+10];
					serial_grc.brd_io[k+11] = GIO_In_Out[k+11];
					serial_grc.brd_io[k+12] = GIO_In_Out[k+12];
					serial_grc.brd_io[k+13] = GIO_In_Out[k+13];
					serial_grc.brd_io[k+14] = GIO_In_Out[k+14];
					serial_grc.brd_io[k+15] = GIO_In_Out[k+15];
					serial_grc.brd_io[k+16] = GIO_In_Out[k+16];
					serial_grc.brd_io[k+17] = GIO_In_Out[k+17];
					hc_dvr_brd[h].riser = 0;				// default to standard riser
					if ((HC_IO_board[i] & 0x04) != 0)
						hc_dvr_brd[h].riser = 1;			// ir
					else if ((HC_IO_board[i] & 0x08) != 0)	
						hc_dvr_brd[h].riser = 2;			// code blue
					else if ((HC_IO_board[i] & 0x20) != 0)
						hc_dvr_brd[h].riser = 3;			// vip
					h++;
					j+=6;
				}
			}
			else
			{		// Standard IO boards
				grcan_dev_list[nmb_grcan_dev] = (uint8)(c_grcan_SEB_1 + j);
				grcan_dev[(c_grcan_SEB_1 + j)].ioname_chksum = calc_ioname_grcan_cksum((uint8)(c_grcan_SEB_1 + j));
				grcan_dev[(c_grcan_SEB_1 + j)].io_color_ix_chksum = 0;
				nmb_grcan_dev ++;
				k = (j*3);
				serial_grc.brd_io[k] = GIO_In_Out[k];
				serial_grc.brd_io[k+1] = GIO_In_Out[k+1];
				serial_grc.brd_io[k+2] = GIO_In_Out[k+2];
				j++;
				debug4++;
			}
		}
		else 
			j++;
		
	}
 /*
	for (i=0;i<c_max_grcan_io_brd;i++)
	{
		serial_grc.dev[0].iob[0] = 0;
		serial_grc.dev[0].iob[1] = 0xf0;
		serial_grc.dev[0].iob[2] = 0xff;
		j++;
	}
*/		
	for (i=0;i<c_max_grcan_dev;i++)
	{
		grcan_dev[i].version = 0;
		grcan_dev[i].revision = 0;
		grcan_dev[i].online = 0;
	}
	// See if the board is there
	grcan_init_count++;
	
	for (i=0;i<nmb_hc_dvr_brds;i++)
	{
		hc_dvr_brd[i].init_seq = 0;
		hc_dvr_brd[i].init_count = 0;
		hc_dvr_brd[i].upd_seq = 0;
	}
    
 	grcan_tx_complete[8] = 1;
	grcan_tx_complete[9] = 1;
	grcan_tx_complete[10] = 1;
	grcan_tx_complete[11] = 1;
    
	_disable();
	
	/*  Can Interrupt vectors
	
			CAN_INT_BASE = Can Bus Of and Warning
			CAN_INT_BASE + 1 = Can Error
			CAN_INT_BASE + 2 = Can Wake Up
			CAN_INT_BASE + 3 = Can Buffer 0
			CAN_INT_BASE + 4 = Can Buffer 1
			CAN_INT_BASE + 5 = Can Buffer 2
			CAN_INT_BASE + 6 = Can Buffer 3
			CAN_INT_BASE + 7 = Can Buffer 4
			CAN_INT_BASE + 8 = Can Buffer 5
			CAN_INT_BASE + 9 = Can Buffer 6
			CAN_INT_BASE + 10 = Can Buffer 7
			CAN_INT_BASE + 11 = Can Buffer 8
			CAN_INT_BASE + 12 = Can Buffer 9
			CAN_INT_BASE + 13 = Can Buffer 10
			CAN_INT_BASE + 14 = Can Buffer 11
			CAN_INT_BASE + 15 = Can Buffer 12
			CAN_INT_BASE + 16 = Can Buffer 13
			CAN_INT_BASE + 17 = Can Buffer 14
			CAN_INT_BASE + 18 = Can Buffer 15
			CAN_INT_BASE + 19 = Can Buffer 16-31
			CAN_INT_BASE + 20 = Can Buffer 32-63
			
	
	
	*/

	// Setup up interrupt routine vector
	INTC_InstallINTCInterruptHandler(grcan_bus_off_and_warning_int, GR_CAN_INT_BASE, 1);  
	INTC_InstallINTCInterruptHandler(grcan_error_int, GR_CAN_INT_BASE+1, 1);  
	INTC_InstallINTCInterruptHandler(wrong_grcan_int, GR_CAN_INT_BASE+3, 1);  
	INTC_InstallINTCInterruptHandler(wrong_grcan_int, GR_CAN_INT_BASE+4, 1);  
	INTC_InstallINTCInterruptHandler(wrong_grcan_int, GR_CAN_INT_BASE+5, 1);  
	INTC_InstallINTCInterruptHandler(wrong_grcan_int, GR_CAN_INT_BASE+6, 1);  
	INTC_InstallINTCInterruptHandler(wrong_grcan_int, GR_CAN_INT_BASE+7, 1);  
	INTC_InstallINTCInterruptHandler(grcan_rcv_int, GR_CAN_INT_BASE+8, 1);  
	INTC_InstallINTCInterruptHandler(grcan_rcv_fifo_warning_int, GR_CAN_INT_BASE+9, 1);  
	INTC_InstallINTCInterruptHandler(grcan_rcv_fifo_overflow_int, GR_CAN_INT_BASE+10, 1);  
	INTC_InstallINTCInterruptHandler(grcan_trm_int_b8, GR_CAN_INT_BASE+11, 1);  
	INTC_InstallINTCInterruptHandler(grcan_trm_int_b9, GR_CAN_INT_BASE+12, 1);  
	INTC_InstallINTCInterruptHandler(grcan_trm_int_b10, GR_CAN_INT_BASE+13, 1);  
	INTC_InstallINTCInterruptHandler(grcan_trm_int_b11, GR_CAN_INT_BASE+14, 1);  

	
	// Send init to can board
	if (Grcan_ChipFreezeMode(c_Freeze_Start) == 1)
	{
		grcan_init_count++;
		GR_CAN.MCR.B.SOFTRST = 1;		// Soft Reset
		reset_count = 0;
		while ((GR_CAN.MCR.B.SOFTRST == 1) && (reset_count < 1000))
			reset_count++;
			
		GR_CAN.MCR.B.MDIS = 0;			// Enable module after reset

	    // Clear of all interrupts
	    GR_CAN.IFLAG1.R = 0xFFFFFFFF;
	    GR_CAN.IFLAG2.R = 0xFFFFFFFF;
	    
	    // Disable of Interrupts
		GR_CAN.IMASK1.R = 0;
		GR_CAN.IMASK2.R = 0;
		
		GR_CAN.MCR.B.BCC = 1;			// Allow backward compatibility since all mask will be set to 0xffff
		GR_CAN.MCR.B.MAXMB = 0x0B;		// Set to 12 (0xC - 1)
		GR_CAN.MCR.B.WRNEN = 0;			// Warning interrupts
		GR_CAN.MCR.B.SRXDIS = 1;			// Self Reception disabled
		GR_CAN.MCR.B.FEN = 1;			// Enable RX fifo
		GR_CAN.MCR.B.ABTEN = 1;			// Abort enable
		GR_CAN.MCR.B.LPRIOEN = 0;		// No local priority mode
		GR_CAN.MCR.B.IDAM = 0;			// ID Acceptance mode (one full id per filter element)
		
		
	    // Configure clock AND baud rate timer for 115200 baud
	    
	    /*	
	    	fTq = Bit Rate * Number of time Quanta = fCANCLK / Prescale Value
	    	Prescale Value = fCANCLK/(Bit Rate * Number of Time Quanta)
	    	Prescale Value = 108.75 Mhz / (115200 * 16) = 59.00065
	    */
	    
	    
	    GR_CAN.CTRL.B.CLKSRC = 1; 		// System Clock 108.75 Mhz
		if (fvars[fvcanbaud] == 0)	  // Can Baud Rate, 0=115.2K, 1=57.6K
		    GR_CAN.CTRL.B.PRESDIV = 58;		// Prescale Devision Factor + 1 = Prescale Value for 115200
		else
		    GR_CAN.CTRL.B.PRESDIV = 117;		// Prescale Devision Factor + 1 = Prescale Value for 57600
		
		GR_CAN.CTRL.B.RJW = 2;			// Resync Jump Width = RJW + 1 = 3
		// Total Tq = 16
		GR_CAN.CTRL.B.PSEG1 = 7;			// Phase Segment 1 = (PSEG1 + 1) x Tq = 8 (also sync seg = 1)
		GR_CAN.CTRL.B.PSEG2 = 5;			// Phase Segment 2 = (PSEG2 + 1) x Tq = 6
		GR_CAN.CTRL.B.PROPSEG = 0;		// Propagation Segment time = (PROPSEG + 1) x Tq = 1
		GR_CAN.CTRL.B.SMP = 1;			// Three samples
		
		GR_CAN.CTRL.B.BOFFREC = 0;		// Automamtic recovery
		GR_CAN.CTRL.B.TSYN = 0;			// Timer sync disabled
		GR_CAN.CTRL.B.LBUF = 1;			// Lowest buffer transmitted first
		GR_CAN.CTRL.B.LOM = 0;			// Listen only mode disabled
		
		GR_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
		GR_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
		GR_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
		GR_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled


		// Set Acceptance Mask
		
		GR_CAN.RXIMR[0].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[1].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[2].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[3].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[4].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[5].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[6].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[7].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[8].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[9].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[10].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[11].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[12].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[13].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[14].R = 0x00000000;		// Enable all packets
		GR_CAN.RXIMR[15].R = 0x00000000;		// Enable all packets

		// Set Receive ID
		GR_CAN.MB[6].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		GR_CAN.MB[6].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		GR_CAN.MB[6].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		GR_CAN.MB[6].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID
		GR_CAN.MB[7].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		GR_CAN.MB[7].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		GR_CAN.MB[7].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		GR_CAN.MB[7].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID

		GR_CAN.MB[8].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		GR_CAN.MB[9].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		GR_CAN.MB[10].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		GR_CAN.MB[11].MSG.MCS.B.CODE = 0x08;		// write inactive mode


		if (Grcan_ChipFreezeMode(c_Freeze_Stop) == 1)
		{
			grcan_init_count++;
			if (nmb_grcan_dev != 0)
			{	// initialize can board

			    // Enable interrupts 
				GR_CAN.IMASK1.R = 0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
				GR_CAN.IMASK2.R = 0x00;		// MB 32-63 NOT Enabled
				GR_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
				GR_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
				GR_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
				GR_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled

			}
		}
		else
		{
			// Set error code
			grcan_error = 0x01;
		}		
	}
	else
	{
		// Set error code
		grcan_error = 0x02;
	} 
	_enable();
}

// **************************************************
// ********  Can Board Interrupt Routines  **********
// **************************************************

void wrong_grcan_int (void)
{
	GR_CAN.IFLAG1.B.BUF00I = 1;
	GR_CAN.IFLAG1.B.BUF01I = 1;
	GR_CAN.IFLAG1.B.BUF02I = 1;
	GR_CAN.IFLAG1.B.BUF03I = 1;
	GR_CAN.IFLAG1.B.BUF04I = 1;
	grcan_WCI_cnt++;
}
void grcan_bus_off_and_warning_int (void)
{		// Bus off, Transmit count warning, Receive Count warning
	GR_CAN.ESR.B.BOFFINT = 1;
	GR_CAN.ESR.B.TWRNINT = 1;
	GR_CAN.ESR.B.RWRNINT = 1;
	grcan_BOI_cnt++;
}

void grcan_error_int (void)
{		// Bit 0, Bit 1, Acknowledge, CRC, Form or Stuffing Errors
	GR_CAN.ESR.B.ERRINT = 1;
	grcan_ERI_cnt++;
}

void grcan_rcv_int (void)
{	// Receive Interrupt (MB5)
	grcan_RI_cnt++;

	grcan_rcv_fifo_cnt = 0;
	while ((GR_CAN.IFLAG1.B.BUF05I == 1) && (grcan_rcv_fifo_cnt < 9))
	{			// Have a can message
		receive_grcan_in (0);
		GR_CAN.IFLAG1.B.BUF05I = 1;				// Clears the bit
		grcan_rcv_fifo_cnt++;
	}
	if (grcan_rcv_fifo_cnt > 1)
		grcan_debug_5 = grcan_rcv_fifo_cnt;
}

void grcan_rcv_fifo_warning_int (void)
{	// Receive fifo warning (MB6)
	GR_CAN.IFLAG1.B.BUF06I = 1;
	grcan_FWI_cnt++;
}

void grcan_rcv_fifo_overflow_int (void)
{	// Receive Fifo Overrun Interrupt (MB7)
	GR_CAN.IFLAG1.B.BUF07I = 1;
	grcan_FOI_cnt++;
}

void grcan_trm_int_b8 (void)
{	// Transmit Interrupt
	grcan_TI8_cnt++;
	GR_CAN.IFLAG1.B.BUF08I = 1;
	grcan_transmit(8);
	grcan_tx_complete[8] = 1;
}

void grcan_trm_int_b9 (void)
{	// Transmit Interrupt
	grcan_TI9_cnt++;
	GR_CAN.IFLAG1.B.BUF09I = 1;
	grcan_transmit(9);
	grcan_tx_complete[9] = 1;
}

void grcan_trm_int_b10 (void)
{	// Transmit Interrupt
	grcan_TI10_cnt++;
	GR_CAN.IFLAG1.B.BUF10I = 1;
	grcan_transmit(10);
	grcan_tx_complete[10] = 1;
}

void grcan_trm_int_b11 (void)
{	// Transmit Interrupt
	grcan_TI11_cnt++;
	GR_CAN.IFLAG1.B.BUF11I = 1;
	grcan_transmit(11);
	grcan_tx_complete[11] = 1;
}

//************************************************************
// Receive the can input
//************************************************************

void receive_grcan_in (int16 BufIx)
{
	int16 i;
	int16 read_dly_cnt;
 	union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu_2;					// unsigned integer union
	
	read_dly_cnt = 0;
	while (((GR_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) == 0x01) && (read_dly_cnt < 100))
		read_dly_cnt ++;
		
	if (read_dly_cnt > 1)
		grcan_debug_6 = read_dly_cnt;

	if ((GR_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) != 0x01)
	{			// read of CS locks the receive buffer
		if (grcan_rx[grc_rpix].valid_msg == 0)
		{
			grcan_rx[grc_rpix].ex_id = (uint8)GR_CAN.MB[BufIx].MSG.MCS.B.IDE;
			grcan_rx[grc_rpix].len = (uint8)GR_CAN.MB[BufIx].MSG.MCS.B.LENGTH;
			grcan_rx[grc_rpix].rtr = (uint8)GR_CAN.MB[BufIx].MSG.MCS.B.RTR;

			if (grcan_rx[grc_rpix].len > 8)
				grcan_rx[grc_rpix].len = 8;
			if (grcan_rx[grc_rpix].ex_id == 1)		// Extended frame
			{
				uiu.dw = (GR_CAN.MB[BufIx].MSG.MID.R << 3);
				grcan_rx[grc_rpix].msg = uiu.b[hb];	// Message
				grcan_rx[grc_rpix].addr = uiu.b[mhb];	// Address
				grcan_rx[grc_rpix].id2 = uiu.b[mlb];
				grcan_rx[grc_rpix].id3 = uiu.b[lb];
			}
			else
			{
				uiu.dw = ((GR_CAN.MB[BufIx].MSG.MID.R & 0x1FFC0000) << 1) ;
				uiu_2.dw = ((GR_CAN.MB[BufIx].MSG.MID.R & 0x1FFC0000) >> 2);
				grcan_rx[grc_rpix].msg = uiu.b[hb];	// Message
				grcan_rx[grc_rpix].addr = uiu_2.b[mhb];    // Address
			}

			i = 0;
			while ((i < grcan_rx[grc_rpix].len) && (i < 8))
			{
				grcan_rx[grc_rpix].data[i] = GR_CAN.MB[BufIx].MSG.MDATA.B[i];
				i++;
			}
			grcan_rx[grc_rpix].valid_msg = 1;
		}
	
	}
	else
	{
		grcan_debug_4 = GR_CAN.MB[BufIx].MSG.MCS.B.CODE;
		grcan_rcv_int_fail_cnt++;	
	}
		
	grcan_rx[grc_rpix].time = (uint16)GR_CAN.MB[BufIx].MSG.MCS.B.TIMESTAMP;		// Unlocks the buffer

	grc_rpix++;
	if (grc_rpix >= c_grcan_rx_buf_siz)
		grc_rpix = 0;
			
}

//************************************************************
// Unpack the can receive buffer
//************************************************************

void unpack_grcan_in (void)
{
	int16 loop_cnt = 0;
	int16 devnmb = 0;
	int16 iodev_ix = 0;
	int16 dvr_ix = 0;
	uint16 iodataix = 0;
	uint8 io_mask = 0;
	uint8 shift_amt = 0;
	uint8 shift_ix = 0;
	int16 j = 0;
	int16 i = 0;
	int16 k = 0;

	while ((loop_cnt <= 4)  && (grc_rgix != grc_rpix))
	{
		if (grcan_rx[grc_rgix].valid_msg == 1)
		{		
			if (grcan_rx[grc_rgix].addr > cons[nmbcars])
				devnmb = findmatch(grcan_rx[grc_rgix].addr, grcan_addr, sizeof(grcan_addr));
			else 
				devnmb = grcan_rx[grc_rgix].addr;
			if (grcan_rx[grc_rgix].rtr == 0)
			{		// OK to unpack the data 
	 			switch (grcan_rx[grc_rgix].msg)
				{

					case 1:	// Receive Request for Dispatcher
						if ((devnmb >= 0) && (devnmb <= cons[nmbcars]))
						{	  // id1 is the address
							if (grcan_rx[grc_rgix].data[0] < cons[carnmb])
							{
								timers[tgrcan_master] = 0;
								dispatcher_count = 0;
								Dispatcher = grcan_rx[grc_rgix].data[0];
							}
							grcan_car_online[devnmb] = 1;
							timers[tgrcan_car+devnmb] = 0;
						}
						break;

					case 10:	  // Device Can Error Message
						if ((devnmb >= c_grcan_START) && (devnmb <= c_grcan_END))
						{	  // id1 is the address
							grcan_dev[devnmb].can_error = grcan_rx[grc_rgix].data[0];
							grcan_dev[devnmb].can_error_data = grcan_rx[grc_rgix].data[1];
						}
						break;

					case 11:	// Invalid request for initialization
						if ((devnmb >= c_grcan_START) && (devnmb <= c_grcan_END))
						{	  // id1 is the address
							grcan_put_dev_msg_req(devnmb, 10);
						}
						break;

					case 12:	// message 12: device version and revision
						if ((devnmb >= c_grcan_START) && (devnmb <= c_grcan_VS))
						{	  // id1 is the address
							if (grcan_dev[devnmb].version != 0) 
							{
								fault_prog_flag1 = 0;		// must set to zero on this fault for galcom
								fault_device = devnmb;
								record_fault(f_GRCAN_device_reset);
							}
							grcan_dev[devnmb].version = grcan_rx[grc_rgix].data[0];
							grcan_dev[devnmb].revision = grcan_rx[grc_rgix].data[1];
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
						}
						else if ((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_END))
						{	  // id1 is the address
							if (grcan_dev[devnmb].version != 0) 
							{
								fault_device = devnmb;
								record_fault(f_GRCAN_device_reset);		// Need to change to grcan device reset error
							}
							grcan_dev[devnmb].version = grcan_rx[grc_rgix].data[0];
							grcan_dev[devnmb].revision = grcan_rx[grc_rgix].data[1];
							timers[tgrcanrx + (devnmb-c_grcan_SEB_1)] = 0;
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 14:	// message 14: device inputs
						if (((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_40) && ((cons[sercom] & 0x01) == 0)) ||
						 	((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_12) && ((cons[sercom] & 0x01) != 0)))
						{	  // id1 is the address
							iodev_ix = (int16)(devnmb-c_grcan_SEB_1);
							iodataix = (int16)((IODATA_GRCAN_START + iodev_ix) * 3); 		// input array
							serial_grc.dev[iodev_ix].iob[0] = (uint8)((serial_grc.dev[iodev_ix].iob[0] & GIO_In_Out[iodataix]) | (grcan_rx[grc_rgix].data[0] & ~GIO_In_Out[iodataix]));
							serial_grc.dev[iodev_ix].iob[1] = (uint8)((serial_grc.dev[iodev_ix].iob[1] & GIO_In_Out[iodataix+1]) | (grcan_rx[grc_rgix].data[1] & ~GIO_In_Out[iodataix+1]));
							serial_grc.dev[iodev_ix].iob[2] = (uint8)((serial_grc.dev[iodev_ix].iob[2] & GIO_In_Out[iodataix+2]) | (grcan_rx[grc_rgix].data[2] & ~GIO_In_Out[iodataix+2]));
							sync_grc.dev[iodev_ix].reflect_data = 1;
							grcan_debug_2++;
							timers[tgrcanrx + (devnmb-c_grcan_SEB_1)] = 0;
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
						}
						else if ((devnmb >= c_grcan_HCB_1) && (devnmb <= c_grcan_HCB_40) && ((cons[sercom] & 0x01) != 0))
						{	  // Individual hall call board
							fl = (debnmb - c_grcan_HCB_1) + 1;
							fl_ix = (fl - 1)/32;
							hcb[fl-1].R = grcan_rx[grc_rgix].data[0];
							
							if (fl != cons[topf])
							{
								if (hcb[fl-1].B.in_up == 1)
								{
									gc_uc[fl_ix] |= fl_mask[fl];
									setginp(i_UC(fl)) = 1;
								}
								else
								{
									gc_uc[fl_ix] &= ~fl_mask[fl];
									clrginp(i_UC(fl)) = 1;
								}
								if (hcb[fl-1].B.out_up == 1)
									setginp(o_UCL(fl)) = 1;
								else
									clrginp(o_UCL(fl)) = 1;
							}
							
							if (hcb[fl-1].B.in_dn == 1)
							{
								if (fl == cons[bottomf])
									setginp[i_HC];
								else
								{
									gc_dc[fl_ix] |= fl_mask[fl];
									setginp(i_DC(fl)) = 1;
								}
							}
							else
							{
								if (fl == cons[bottomf])
									clrginp[i_HC];
								else
								{
									gc_dc[fl_ix] &= ~fl_mask[fl];
									clrginp(i_DC(fl)) = 1;
								}
							}
							
							if (fl != cons[bottomf])
							{
									
								if (hcb[fl-1].B.out_dn == 1)
									setginp(o_DCL(fl)) = 1;
								else
									clrginp(o_DCL(fl)) = 1;
							}
							
							sync_hcb[fl-1].reflect_data = 1;

							timers[tgrcanrx + (devnmb-c_grcan_SEB_1)] = 0;
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 16:	// message 16: return of output data
						if (((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_40) && ((cons[sercom] & 0x01) == 0)) ||
						 	((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_12) && ((cons[sercom] & 0x01) != 0)))
						{	  // id1 is the address
							iodev_ix = (int16)(devnmb-c_grcan_SEB_1);
							iodataix = (int16)((IODATA_GRCAN_START + iodev_ix) * 3); 		// input array
							sync_grc.dev[iodev_ix].iob[0] = (uint8)((sync_grc.dev[iodev_ix].iob[0] & ~GIO_In_Out[iodataix]) | (~grcan_rx[grc_rgix].data[0] & GIO_In_Out[iodataix]));
							sync_grc.dev[iodev_ix].iob[1] = (uint8)((sync_grc.dev[iodev_ix].iob[1] & ~GIO_In_Out[iodataix+1]) | (~grcan_rx[grc_rgix].data[1] & GIO_In_Out[iodataix+1]));
							sync_grc.dev[iodev_ix].iob[2] = (uint8)((sync_grc.dev[iodev_ix].iob[2] & ~GIO_In_Out[iodataix+2]) | (~grcan_rx[grc_rgix].data[2] & GIO_In_Out[iodataix+2]));
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
						}
						else if ((devnmb >= c_grcan_HCB_1) && (devnmb <= c_grcan_HCB_40) && ((cons[sercom] & 0x01) != 0))
						{	  // Individual hall call board
							fl = (debnmb - c_grcan_HCB_1) + 1;
							sync_hcb[fl-1].iostat.iob = grcan_rx[grc_rgix].data[0];
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 44:		// message 44: Voltage Sensor Input	 (value x 10) = xxx.x Vrms
						if (devnmb == c_grcan_VS)
						{
							uwu.b[lbw] = grcan_rx[grc_rgix].data[0];
							uwu.b[hbw] = grcan_rx[grc_rgix].data[1];
							L1_Voltage = uwu.w;
							uwu.b[lbw] = grcan_rx[grc_rgix].data[2];
							uwu.b[hbw] = grcan_rx[grc_rgix].data[3];
							L2_Voltage = uwu.w;
							uwu.b[lbw] = grcan_rx[grc_rgix].data[4];
							uwu.b[hbw] = grcan_rx[grc_rgix].data[5];
							L3_Voltage = uwu.w;
							uwu.b[lbw] = grcan_rx[grc_rgix].data[6];
							uwu.b[hbw] = grcan_rx[grc_rgix].data[7];
							Door_Voltage = uwu.w;
							grcan_dev[devnmb].online_cnt = 3;
							grcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 46:	// message 46: Request for i/o labels
						if ((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_40))
						{	  
							timers[tgrcanrx + (devnmb-c_grcan_SEB_1)] = 0;
							grcan_dev[devnmb].label_req = grcan_rx[grc_rgix].data[0];
							grcan_dev[devnmb].label_ix = 0;
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
							grcan_put_dev_msg_req(devnmb, 47);
						}
						break;
					case 48:	  // Message 48: Device Error Message
						if ((devnmb >= c_grcan_START) && (devnmb <= c_grcan_END))
						{	  
							grcan_dev[devnmb].dev_error = grcan_rx[grc_rgix].data[0];
							grcan_dev[devnmb].dev_error_data[0] = grcan_rx[grc_rgix].data[1];
							grcan_dev[devnmb].dev_error_data[1] = grcan_rx[grc_rgix].data[2];
							fault_device = devnmb;
							fault_prog_flag1 = grcan_dev[devnmb].dev_error;				// fault code
							fault_prog_flag2 = grcan_dev[devnmb].dev_error_data[0];		// fault count
							fault_device_2 = grcan_dev[devnmb].dev_error_data[1];	// speial code
							record_fault(f_GRCAN_device_fault);
/*							if ((cons[RGB_CC] == 1) && ((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_40)))
							{
								if (grcan_dev[devnmb].dev_error <  (unsigned char) sizeof(ccb_flt_tbl))
								{
									fault_device = grcan_addr[devnmb];				// device address
									fault_prog_flag1 = grcan_dev[devnmb].dev_error_data[0];	  // local address
									fault_prog_flag2 = grcan_dev[devnmb].dev_error_data[1];	  // io number 0-11	
									record_fault(ccb_flt_tbl[grcan_dev[devnmb].dev_error]);
								}
							}
*/
						}
 						break;

					case 74:	// message 74: device inputs
						if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10))
						{	  // id1 is the address
							iodev_ix = (int16)(devnmb-c_grcan_HC_1);
							iodataix = (int16)((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult); 		// input or output array	 starts at array 0 (HCP,1U,2D,2U,3D,3U,4D,...etc)
							serial_grc.devhc[iodev_ix].ihc[0] = (uint8)((serial_grc.devhc[iodev_ix].ihc[0] & GIO_In_Out[iodataix+0]) | (grcan_rx[grc_rgix].data[0] & ~GIO_In_Out[iodataix+0]));
							serial_grc.devhc[iodev_ix].ihc[1] = (uint8)((serial_grc.devhc[iodev_ix].ihc[1] & GIO_In_Out[iodataix+1]) | (grcan_rx[grc_rgix].data[1] & ~GIO_In_Out[iodataix+1]));
							serial_grc.devhc[iodev_ix].ihc[2] = (uint8)((serial_grc.devhc[iodev_ix].ihc[2] & GIO_In_Out[iodataix+2]) | (grcan_rx[grc_rgix].data[2] & ~GIO_In_Out[iodataix+2]));
							serial_grc.devhc[iodev_ix].ihc[3] = (uint8)((serial_grc.devhc[iodev_ix].ihc[3] & GIO_In_Out[iodataix+3]) | (grcan_rx[grc_rgix].data[3] & ~GIO_In_Out[iodataix+3]));
							serial_grc.devhc[iodev_ix].ihc[4] = (uint8)((serial_grc.devhc[iodev_ix].ihc[4] & GIO_In_Out[iodataix+4]) | (grcan_rx[grc_rgix].data[4] & ~GIO_In_Out[iodataix+4]));
							serial_grc.devhc[iodev_ix].ihc[5] = (uint8)((serial_grc.devhc[iodev_ix].ihc[5] & GIO_In_Out[iodataix+5]) | (grcan_rx[grc_rgix].data[5] & ~GIO_In_Out[iodataix+5]));
							sync_grc.dev[iodev_ix].reflect_data = 1;
							grcan_debug_2++;
							timers[tgrcanrx + (devnmb-c_grcan_HC_1)] = 0;
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
						}
						break;
						
					case 75:	// message 75: Hall call driver board status and faults
						if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10))
						{	  // id1 is the address
							dvr_ix = devnmb-c_grcan_HC_1;
							put_pkt_status(75,devnmb,GCANCOM);
							hc_dvr_brd[dvr_ix].status_1 = (uint8)grcan_rx[grc_rgix].data[0];
							if ((hc_dvr_brd[dvr_ix].prev_status_1 != hc_dvr_brd[dvr_ix].status_1) && (hc_dvr_brd[dvr_ix].status_1 != 0))
							{
								// look at only the faults bits that have changed from 0 to 1.
								j = (uint16)((hc_dvr_brd[dvr_ix].prev_status_1 ^ hc_dvr_brd[dvr_ix].status_1) & hc_dvr_brd[dvr_ix].status_1);
								for (i=0;i<=5;i++)
								{
									if ((j & bittbl[i]) != 0)
									{
										fault_device = devnmb;
										record_fault((uint16)(f_hc_rx_top + i));
									}
								}
							} 
							hc_dvr_brd[dvr_ix].prev_status_1 = hc_dvr_brd[dvr_ix].status_1;
							hc_dvr_brd[dvr_ix].status_2 = (uint8)grcan_rx[grc_rgix].data[1];
							k = 0;
							if (((hc_dvr_brd[dvr_ix].status_2 & 0x04) == 0) || ((hc_dvr_brd[dvr_ix].status_2 & 0x08) != 0))
							{
								hcbd_flt_ix = (uint16)((grcan_rx[grc_rgix].data[1] & 0x03) * 6);
								hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix] = (uint8)grcan_rx[grc_rgix].data[2];
								hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+1] = (uint8)grcan_rx[grc_rgix].data[3];
								hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+2] = (uint8)grcan_rx[grc_rgix].data[4];
								hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+3] = (uint8)grcan_rx[grc_rgix].data[5];
								hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+4] = (uint8)grcan_rx[grc_rgix].data[6];
								hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+5] = (uint8)grcan_rx[grc_rgix].data[7];
								for (i=0;i<=5;i++)
								{
									if ((hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+i] != hc_dvr_brd[dvr_ix].prev_fault[hcbd_flt_ix+i]) &&
										(hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+i] != 0) && (hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+i] != 15))
									{							   // fault cannot be 0:(no fault) or 15:(Invalid floor)
										fault_device = devnmb;
										fault_device_2 = hcbd_flt_ix+i+1;
										record_fault(hcb_flt_tbl[hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+i] & 0x3f]);
									}
									hc_dvr_brd[dvr_ix].prev_fault[hcbd_flt_ix+i] = hc_dvr_brd[dvr_ix].fault[hcbd_flt_ix+i];
								}
								k = 6;
							}
							else if (((hc_dvr_brd[iodev_ix].riser != 2) || (cons[cblue] != 2)) && ((hc_dvr_brd[iodev_ix].riser != 3) || (cons[VipSvc] != 4)))
							{		// inputs not being used for code blue or vip
								iodev_ix = (int16)(devnmb-c_grcan_HC_1);
								iodataix = (int16)(((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult) + 12); 		// inputs only	 starts at array 0 (1HSC,2HSC,3HSC,4HSC,..etc)
								serial_grc.devhc[iodev_ix].axihc[0] = (uint8)((serial_grc.devhc[iodev_ix].axihc[0] & GIO_In_Out[iodataix+0]) | (grcan_rx[grc_rgix].data[2] & ~GIO_In_Out[iodataix+0]));
								serial_grc.devhc[iodev_ix].axihc[1] = (uint8)((serial_grc.devhc[iodev_ix].axihc[1] & GIO_In_Out[iodataix+1]) | (grcan_rx[grc_rgix].data[3] & ~GIO_In_Out[iodataix+1]));
								serial_grc.devhc[iodev_ix].axihc[2] = (uint8)((serial_grc.devhc[iodev_ix].axihc[2] & GIO_In_Out[iodataix+2]) | (grcan_rx[grc_rgix].data[4] & ~GIO_In_Out[iodataix+2]));
							}
							timers[tgrcanrx + (devnmb-c_grcan_HC_1)] = 0;
							if ((hc_dvr_brd[dvr_ix].status_2 & 0x10) != 0)
							{
								if (grcan_put_dev_msg_req(devnmb, 79) == 1)
								{
									hc_dvr_brd[iodev_ix].color_seq = 0;
								}
							}
						}
						break;
						
					case 76:	// message 76: Hall Call station board version and revision
						if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10))
						{	  // id1 is the address
							dvr_ix = devnmb-c_grcan_HC_1;
							ver_dev_ix = (uint8)grcan_rx[grc_rgix].data[0];
							if (ver_dev_ix < 24)
							{
								hc_dvr_brd[dvr_ix].version[ver_dev_ix] = (uint8)grcan_rx[grc_rgix].data[1];		// Store device version number
								hc_dvr_brd[dvr_ix].revision[ver_dev_ix] = (uint8)grcan_rx[grc_rgix].data[2];	// Store device revision number
							}
							ver_dev_ix++;
							if (ver_dev_ix < 24)
							{
								hc_dvr_brd[dvr_ix].version[ver_dev_ix] = (uint8)grcan_rx[grc_rgix].data[3];		// Store device version number
								hc_dvr_brd[dvr_ix].revision[ver_dev_ix] = (uint8)grcan_rx[grc_rgix].data[4];	// Store device revision number
							}
							ver_dev_ix++;
							if (ver_dev_ix < 24)
							{
								hc_dvr_brd[dvr_ix].version[ver_dev_ix] = (uint8)grcan_rx[grc_rgix].data[5];		// Store device version number
								hc_dvr_brd[dvr_ix].revision[ver_dev_ix] = (uint8)grcan_rx[grc_rgix].data[6];	// Store device revision number
							}
							ver_dev_ix++;
							if (ver_dev_ix < 24)
								put_pkt_req(76,devnmb,GCANCOM);
							timers[tgrcanrx + (devnmb-c_grcan_HC_1)] = 0;
						}
						break;

					case 78:	// message 78: HC Driver board init status
						if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10))
						{	  // id1 is the address
							iodev_ix = (int16)(devnmb-c_grcan_HC_1);
							iodataix = (int16)(((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult) + 6); 		// input or output array
							if (grcan_rx[grc_rgix].data[0] == 0x30)
								hc_dvr_brd[iodev_ix].init_seq = 1;
							if (grcan_rx[grc_rgix].data[0] == 0x31)
								hc_dvr_brd[iodev_ix].init_seq = 2;
							else if (grcan_rx[grc_rgix].data[0] == 0x32)
							{
								grcan_dev[devnmb].version = grcan_rx[grc_rgix].data[1];
								grcan_dev[devnmb].revision = grcan_rx[grc_rgix].data[2];
								hc_dvr_brd[iodev_ix].init_seq = 3;	
							}
							else if (grcan_rx[grc_rgix].data[0] == 0x33)
							{
								hc_dvr_brd[iodev_ix].init_seq = 4;	
							}
							else if (grcan_rx[grc_rgix].data[0] == 0x34)
							{
								hc_dvr_brd[iodev_ix].init_seq = 5;	
							}
							else if (grcan_rx[grc_rgix].data[0] == 0x35)
							{
								hc_dvr_brd[iodev_ix].init_seq = 6;	
							}
							else if (grcan_rx[grc_rgix].data[0] == 0x36)
							{
								hc_dvr_brd[iodev_ix].init_seq = 7;	
							}
							else if (grcan_rx[grc_rgix].data[0] == 0x37)
							{											   			
								if (grcan_rx[grc_rgix].data[1] == 0x59)
									hc_dvr_brd[iodev_ix].init_seq = 9;	// Init sequence complete
								else
									hc_dvr_brd[iodev_ix].init_seq = 10;		// init fault
									
							}
							else if (grcan_rx[grc_rgix].data[0] == 0x38)
							{
								if (grcan_rx[grc_rgix].data[1] == 0x62)					 			// restart init sequence
									hc_dvr_brd[iodev_ix].init_seq = 0;
								else
									hc_dvr_brd[iodev_ix].init_seq = 10;		// init fault
							}
							
							
							if (hc_dvr_brd[iodev_ix].init_seq != 9)
							{
								if (hc_dvr_brd[iodev_ix].init_seq == 10)
								{
									if (hc_dvr_brd[iodev_ix].init_count < 5)
									{
										hc_dvr_brd[iodev_ix].init_seq = 0;
										hc_dvr_brd[iodev_ix].init_count++;
										grcan_put_dev_msg_req(devnmb, 77);
									}
								}
								else
									grcan_put_dev_msg_req(devnmb, 77);
							}
							
							grcan_dev[devnmb].online_cnt = 5;
							grcan_dev[devnmb].rx_cnt++;
							timers[tgrcanrx + (devnmb-c_grcan_HC_1)] = 0;
						}
						break;
						
					case 91:	// Unpack code blue or VIP inputs from serial hall call boards
						if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10))
						{	  // id1 is the address
							if (((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] == 2)) || 
								((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] == 4)))
							{
									
								iodev_ix = (int16)(devnmb-c_grcan_HC_1);
								iodataix = (int16)(((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult) + 12); 		// inputs only	 starts at array 0 (1CB,2CB,3CB,4CB,..etc)
								serial_grc.devhc[iodev_ix].axihc[0] = (uint8)((serial_grc.devhc[iodev_ix].axihc[0] & GIO_In_Out[iodataix+0]) | (grcan_rx[grc_rgix].data[0] & ~GIO_In_Out[iodataix+0]));
								serial_grc.devhc[iodev_ix].axihc[1] = (uint8)((serial_grc.devhc[iodev_ix].axihc[1] & GIO_In_Out[iodataix+1]) | (grcan_rx[grc_rgix].data[1] & ~GIO_In_Out[iodataix+1]));
								serial_grc.devhc[iodev_ix].axihc[2] = (uint8)((serial_grc.devhc[iodev_ix].axihc[2] & GIO_In_Out[iodataix+2]) | (grcan_rx[grc_rgix].data[2] & ~GIO_In_Out[iodataix+2]));
								timers[tgrcanrx + (devnmb-c_grcan_HC_1)] = 0;
							}
						}
						break;
						

					default:
						break;
				}
			}
			else 
			{
				if ((devnmb >= c_grcan_START) && (devnmb <= c_grcan_END))
				{
					grcan_put_dev_msg_req(devnmb, grcan_rx[grc_rgix].msg);
				}
			}
			grcan_rx[grc_rgix].valid_msg = 0;
		}
		grc_rgix++;
		if (grc_rgix > c_grcan_rx_buf_siz)
			grc_rgix = 0; 
		loop_cnt++;
	}
}


//************************************************************
//*********  Check to see if the Transmit buffer is full *****
//************************************************************

int16 IsGrcTxBufFul(void)
{

	_disable();
	if (grc_tpix == grc_tgix)
		grc_availbuf = c_grcan_tx_buf_siz + 1;
	else if (grc_tpix > grc_tgix)
		grc_availbuf = (c_grcan_tx_buf_siz - grc_tpix) + grc_tgix + 1;
	else 
		grc_availbuf = grc_tgix - grc_tpix;

	_enable();
	if (grc_availbuf > 9)
		return 0;
	else
		return 1;
}


//************************************************************
//*********  Pack data into the Can Structure ****************
//************************************************************


void pack_grcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req)
{

	int16 iodev_ix = 0;
	uint16 iodataix = 0;
	uint8 io_mask = 0;
	uint8 shift_amt = 0;
	uint8 shift_ix = 0;
	unsigned char label_ix = 0;
	unsigned char color_ix = 0;
	unsigned char color_on_ix = 0;
	unsigned char color_off_ix = 0;
	bool valid_name = 0;
	int16 array_start, io_name_start;
	int16 i, j, k;

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union

#if (Traction == 1)
	int16 t_blind = 0;
#endif

	_disable();
	if (grc_tpix < c_grcan_tx_buf_siz)
		grc_tpix++;
	else
		grc_tpix = 0;
	_enable();
				
	if (tx_req == 1)
	{		 // Used to request a packet from a device
		grcan_tx[grc_tpix].rtr = 1;
		grcan_tx[grc_tpix].srr = 1;
		grcan_tx[grc_tpix].len = 0;
		grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
		grcan_tx[grc_tpix].one_tx = 0;
		grcan_tx[grc_tpix].msg = msg;
		grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
		grcan_tx[grc_tpix].id2 = 0;
		grcan_tx[grc_tpix].id3 = 0;
		grcan_tx[grc_tpix].data[0] = 0;
		grcan_tx[grc_tpix].data[1] = 0;
		grcan_tx[grc_tpix].data[2] = 0;
		grcan_tx[grc_tpix].data[3] = 0;
		grcan_tx[grc_tpix].data[4] = 0;
		grcan_tx[grc_tpix].data[5] = 0;
		grcan_tx[grc_tpix].data[6] = 0;
		grcan_tx[grc_tpix].data[7] = 0;
		if ((devnmb >= c_grcan_START) && (devnmb <= c_grcan_END))
		 	grcan_dev[devnmb].tx_cnt++;
	}
	else switch (msg)
	{
		case 1:		// Request to be dispatcher
			if (devnmb == cons[carnmb]) 
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 1;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)devnmb;
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				grcan_tx[grc_tpix].data[0] = (uint8)cons[carnmb];
				grcan_tx[grc_tpix].data[1] = 0;
				
				grcan_tx[grc_tpix].data[2] = 0;	 
				grcan_tx[grc_tpix].data[3] = 0;	 
				grcan_tx[grc_tpix].data[4] = 0;	 

				grcan_tx[grc_tpix].data[5] = 0;
				grcan_tx[grc_tpix].data[6] = 0;
				grcan_tx[grc_tpix].data[7] = 0;
				if (dispatcher_count < 20)
					dispatcher_count++;				
			}
			break;
			
			
		case 11:		// Init data
			if ((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_END))
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 5;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				grcan_tx[grc_tpix].data[0] = version;
				grcan_tx[grc_tpix].data[1] = revision;
				
				grcan_tx[grc_tpix].data[2] = 0;	 // Set board call latching mask
				grcan_tx[grc_tpix].data[3] = 0;	 // Set board call latching mask
				grcan_tx[grc_tpix].data[4] = 0;	 // Set board call latching mask

				grcan_tx[grc_tpix].data[5] = 0;
				grcan_tx[grc_tpix].data[6] = 0;
				grcan_tx[grc_tpix].data[7] = 0;
				grcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 13:		// I/O Board outputs
			if (((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_40) && ((cons[sercom] & 0x01) == 0)) ||
			 	((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_12) && ((cons[sercom] & 0x01) != 0)))
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 8;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_grcan_SEB_1);
				iodataix = (int16)((IODATA_GRCAN_START + iodev_ix) * 3); 		// input array
				grcan_tx[grc_tpix].data[0] = (uint8)((~serial_grc.dev[iodev_ix].iob[0] & GIO_In_Out[iodataix]));
				grcan_tx[grc_tpix].data[1] = (uint8)((~serial_grc.dev[iodev_ix].iob[1] & GIO_In_Out[iodataix+1]));
				grcan_tx[grc_tpix].data[2] = (uint8)((~serial_grc.dev[iodev_ix].iob[2] & GIO_In_Out[iodataix+2]));
				grcan_tx[grc_tpix].data[3] = 0;
				grcan_tx[grc_tpix].data[4] = 0;
				uwu.w = grcan_dev[devnmb].ioname_chksum;
				grcan_tx[grc_tpix].data[5] = uwu.b[lbw];
				grcan_tx[grc_tpix].data[6] = uwu.b[hbw];
				grcan_tx[grc_tpix].data[7] = 0;
				grcan_dev[devnmb].tx_cnt++;
			}
			else if ((devnmb >= c_grcan_HCB_1) && (devnmb <= c_grcan_HCB_40) && ((cons[sercom] & 0x01) != 0))
			{	  // Individual hall call board
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 1;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				fl = (int16)((devnmb-c_grcan_HCB_1) + 1);
				fl_ix = (fl-1)/32;
				if (fl != cons[topf])
				{
					if ((gc_uc_set[fl_ix] & fl_mask[fl]) != 0)
						hcb[fl-1].B.set_up = 1;
					else
						hcb[fl-1].B.set_up = 0;
					if ((gc_uc_canc[fl_ix] & fl_mask[fl]) != 0)
						hcb[fl-1].B.canc_up = 1;
					else
						hcb[fl-1].B.canc_up = 0;
				}
				else
				{
					hcb[fl-1].B.set_up = 0;
					hcb[fl-1].B.canc_up = 0;
				}

				if (fl != cons[bottomf])
				{
					if ((gc_dc_set[fl_ix] & fl_mask[fl]) != 0)
						hcb[fl-1].B.set_dn = 1;
					else
						hcb[fl-1].B.set_dn = 0;
					
					if ((gc_dc_canc[fl_ix] & fl_mask[fl]) != 0)
						hcb[fl-1].B.canc_dn = 1;
					else
						hcb[fl-1].B.canc_dn = 0;
				}
				else
				{
					hcb[fl-1].B.set_dn = 0;
					hcb[fl-1].B.canc_dn = 0;
				}
				
				// Set input data							
				grcan_tx[grc_tpix].data[0] = hcb[fl-1].R;
				grcan_tx[grc_tpix].data[1] = 0;
				grcan_tx[grc_tpix].data[2] = 0;
				grcan_tx[grc_tpix].data[3] = 0;
				grcan_tx[grc_tpix].data[4] = 0;
				grcan_tx[grc_tpix].data[5] = 0;
				grcan_tx[grc_tpix].data[6] = 0;
				grcan_tx[grc_tpix].data[7] = 0;
				grcan_dev[devnmb].tx_cnt++;
			}
			break;
			
		case 15:		// I/O Board inputs sent back to the can serial i/o board
			if (((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_40) && ((cons[sercom] & 0x01) == 0)) ||
			 	((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_12) && ((cons[sercom] & 0x01) != 0)))
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 3;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_grcan_SEB_1);
				iodataix = (int16)((IODATA_GRCAN_START + iodev_ix) * 3); 		// input array
				grcan_tx[grc_tpix].data[0] = (uint8)(serial_grc.dev[iodev_ix].iob[0] & ~GIO_In_Out[iodataix]);
				grcan_tx[grc_tpix].data[1] = (uint8)(serial_grc.dev[iodev_ix].iob[1] & ~GIO_In_Out[iodataix+1]);
				grcan_tx[grc_tpix].data[2] = (uint8)(serial_grc.dev[iodev_ix].iob[2] & ~GIO_In_Out[iodataix+2]);
				grcan_tx[grc_tpix].data[3] = 0;
				grcan_tx[grc_tpix].data[4] = 0;
				grcan_tx[grc_tpix].data[5] = 0;
				grcan_tx[grc_tpix].data[6] = 0;
				grcan_tx[grc_tpix].data[7] = 0;
				grcan_dev[devnmb].tx_cnt++;
			}
			else if ((devnmb >= c_grcan_HCB_1) && (devnmb <= c_grcan_HCB_40) && ((cons[sercom] & 0x01) != 0))
			{	  // Individual hall call board
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 1;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				iodev_ix = (int16)devnmb-c_grcan_HCB_1;
				shift_ix = ((iodev_ix & 0x01) != 0);
				shift_amt = shift_ix * 4;
				iodataix = (int16)(HCB_IODATA_GRCAN_START + iodev_ix/2); 		// io array
				io_mask =  GIO_In_Out[iodataix] & shift_mask[shift_ix];
				// Set input data							
				grcan_tx[grc_tpix].data[0] = (uint8)((serial_grc.brd_io[iodataix] & ~io_mask) << shift_amt);
				grcan_tx[grc_tpix].data[1] = 0;
				grcan_tx[grc_tpix].data[2] = 0;
				grcan_tx[grc_tpix].data[3] = 0;
				grcan_tx[grc_tpix].data[4] = 0;
				grcan_tx[grc_tpix].data[5] = 0;
				grcan_tx[grc_tpix].data[6] = 0;
				grcan_tx[grc_tpix].data[7] = 0;
				grcan_dev[devnmb].tx_cnt++;
			}
			break;
			
		case 47:		// Send I/O Board Label
			if ((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_40))
			{
				if (grcan_dev[devnmb].label_req != 0)
				{
					iodev_ix = (int16)(devnmb-c_grcan_SEB_1);
					label_ix = grcan_dev[devnmb].label_ix;
					array_start = (int16)((IODATA_GRCAN_START + iodev_ix) * 3); 		// input array
					k = 0;		// default to input name (even if all spaces)
					i = (int16)((label_ix + 12)/8);		   
					j = (int16)((label_ix  + 12)% 8);
					if ((gio_send_name[array_start+i] & bittbl[j & 0x07]) != 0)
					{
						valid_name = 1;							// valid output name
						k = 12;
					}
					else 
					{
						i = (int16)(label_ix/8);		   
						j = (int16)(label_ix % 8);
						if ((gio_send_name[array_start + i] & bittbl[j & 0x07]) != 0)
							valid_name = 1;							// valid input name

					}

					grcan_tx[grc_tpix].rtr = 0;
					grcan_tx[grc_tpix].srr = 1;
					grcan_tx[grc_tpix].len = 8;
					grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
					if (grcan_dev[devnmb].online == 1)
						grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
					else
						grcan_tx[grc_tpix].one_tx = 1;
					grcan_tx[grc_tpix].msg = msg;
					grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
					grcan_tx[grc_tpix].id2 = 0;
					grcan_tx[grc_tpix].id3 = 0;
					grcan_tx[grc_tpix].data[0] = label_ix;
					io_name_start = (array_start * 8) + k;
					if(valid_name == 1)
					{
						grcan_tx[grc_tpix].data[1] = hcioname[io_name_start + label_ix][0];
						grcan_tx[grc_tpix].data[2] = hcioname[io_name_start + label_ix][1];
						grcan_tx[grc_tpix].data[3] = hcioname[io_name_start + label_ix][2];
						grcan_tx[grc_tpix].data[4] = hcioname[io_name_start + label_ix][3];
						grcan_tx[grc_tpix].data[5] = hcioname[io_name_start + label_ix][4];
					}
					else
					{
						grcan_tx[grc_tpix].data[1] = ' ';				
						grcan_tx[grc_tpix].data[2] = ' ';				
						grcan_tx[grc_tpix].data[3] = ' ';				
						grcan_tx[grc_tpix].data[4] = ' ';				
						grcan_tx[grc_tpix].data[5] = ' ';				
					}

					color_on_ix = io_on_color_ix[io_name_start + label_ix];

					if (color_on_ix == 0)
						color_off_ix = 0;
					else
						color_off_ix = 4;

					if (((color_on_ix == 6) && ((fvars[fvbklton] & 0x01) == 0)) ||
						((color_on_ix == 7) && ((fvars[fvbklton] & 0x02) == 0)) ||
						((color_on_ix == 8) && ((fvars[fvbklton] & 0x04) == 0)) ||
						((color_on_ix == 9) && ((fvars[fvbklton] & 0x08) == 0)) ||
						((color_on_ix == 2) && ((fvars[fvbklton] & 0x10) == 0)) ||
						((color_on_ix == 3) && ((fvars[fvbklton] & 0x10) == 0)) ||
						((color_on_ix == 4) && ((fvars[fvbklton] & 0x20) == 0)))
						color_off_ix = 0;

					grcan_tx[grc_tpix].data[6] = color_on_ix;		// On State Color
					grcan_tx[grc_tpix].data[7] = color_off_ix;	    // Off State Color

					label_ix ++;
					grcan_dev[devnmb].label_ix = label_ix;
					if (label_ix <= 11)
					{
						grcan_put_dev_msg_req(devnmb, 47);
					}
					else
					{
						grcan_dev[devnmb].label_ix = 0;
						grcan_dev[devnmb].label_req = 0;
					}
					grcan_dev[devnmb].tx_cnt++;
				}
			}
			break;
			
		case 49:			// Message 49: Reset Error Message
			grcan_tx[grc_tpix].rtr = 0;
			grcan_tx[grc_tpix].srr = 1;
			grcan_tx[grc_tpix].len = 1;
			grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
			grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit 
			grcan_tx[grc_tpix].msg = msg;
			grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
			grcan_tx[grc_tpix].id2 = 0;
			grcan_tx[grc_tpix].id3 = 0;
			grcan_tx[grc_tpix].data[0] = 0x03;		// Control byte - bit0: reset error , bit 1: clear error buffer
			grcan_tx[grc_tpix].data[1] = 0;
			grcan_tx[grc_tpix].data[2] = 0;	 		 
			grcan_tx[grc_tpix].data[3] = 0;	 
			grcan_tx[grc_tpix].data[4] = 0;	 
			grcan_tx[grc_tpix].data[5] = 0;
			grcan_tx[grc_tpix].data[6] = 0;
			grcan_tx[grc_tpix].data[7] = 0;
			grcan_dev[devnmb].tx_cnt++;
			break;

		case 72:		// Hall Call Driver Board control data
			if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10))
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 8;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_grcan_HC_1);
				HcbControl[iodev_ix] &= 0xF0;
				HcbControl[iodev_ix] |= fvars[fvhcbseclt] & 0x07;
			
				if ((gc_hsec != 0) || (gc_rhsec != 0) || (gc_shsec != 0) || (gc_rshsec != 0))
					HcbControl[iodev_ix] |= 0x08;
				 
				grcan_tx[grc_tpix].data[0] = (uint8)HcbControl[iodev_ix];
				HcbControl[iodev_ix] &= 0x7F;			// CLEAR reset (send only once)	
				
				// normal hall call
				uwu.w = (uint16)(ColorBrightChecksum + ColorUpHCChecksum + ColorDnHCChecksum + ColorSecChecksum);

				if (fvars[fvhcbcbloc] == 1)
					uwu.w += (uint16)ColorCBOnChecksum;	   
				else if (fvars[fvhcbviploc] == 1)
					uwu.w += (uint16)ColorVipOnChecksum;
				else
					uwu.w += (uint16)(uint16)ColorCBOnChecksum;
				
				if (((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] == 2)) || 
					((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] == 4)))
				{		// aux outputs being used for cb or vip
					grcan_tx[grc_tpix].data[1] = 0;
					grcan_tx[grc_tpix].data[2] = 0;
					grcan_tx[grc_tpix].data[3] = 0;
				}
				else if (((Security_Type[cons[carnmb]] & 0x100) != 0) && ((HcbControl[iodev_ix] & 0x08) != 0))
				{		// if not used for cb or vip then send security data
					iodataix = (int16)((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult); 		// hall call security output data
					grcan_tx[grc_tpix].data[1] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[0] & GIO_In_Out[iodataix+15]));
					grcan_tx[grc_tpix].data[2] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[1] & GIO_In_Out[iodataix+16]));
					grcan_tx[grc_tpix].data[3] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[2] & GIO_In_Out[iodataix+17]));
				}
				else
				{		// aux outputs not used 
					grcan_tx[grc_tpix].data[1] = 0;
					grcan_tx[grc_tpix].data[2] = 0;
					grcan_tx[grc_tpix].data[3] = 0;
				}

//				grcan_tx[grc_tpix].data[1] = (uint8)(gio_valid[iodataix+12]); // hall call security data
//				grcan_tx[grc_tpix].data[2] = (uint8)(gio_valid[iodataix+13]);
//				grcan_tx[grc_tpix].data[3] = (uint8)(gio_valid[iodataix+14]);

				grcan_tx[grc_tpix].data[4] = (uint8)(HcbOnBright);
				grcan_tx[grc_tpix].data[5] = (uint8)(HcbOffBright);
				grcan_tx[grc_tpix].data[6] = uwu.b[lbw];
				grcan_tx[grc_tpix].data[7] = uwu.b[hbw];
						
/*						
				// IR hall call
				if ((fvars[fvhcbirlt] == 0) || ((fvars[fvhcbirlt] == 1) && (ir_call_enable == 1)))
					chksum = ColorBrightChecksum + ColorIRUpChecksum + ColorSecChecksum;
				else
					chksum = ColorBrightChecksum + ColorUpHCChecksum + ColorSecChecksum;
				
				if ((fvars[fvhcbirlt] == 0) || ((fvars[fvhcbirlt] == 1) && (ir_call_enable == 1)))
					chksum = ColorIRDnChecksum;
				else
					chksum = ColorDnHCChecksum;
			
				if (fvars[fvhcbcbloc] == 2)
					chksum += ColorCBOnChecksum;
				else if (fvars[fvhcbviploc] == 2)
					chksum += ColorVipOnChecksum;
				else
					chksum += ColorCBOnChecksum;
				
				
				// Code blue or Vip hall call
				chksum = ColorBrightChecksum + ColorVipOnChecksum + ColorVipOffChecksum + ColorSecChecksum;
				chksum = ColorCBOnChecksum + ColorCBOffChecksum;
*/
				 
			}
			break;
			
		case 73:		// I/O Board outputs
			if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10))
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 8;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_grcan_HC_1);
				iodataix = (int16)(((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult) + 6); 		// OUTPUT array
				grcan_tx[grc_tpix].data[0] = (uint8)((~serial_grc.devhc[iodev_ix].ohc[0] & GIO_In_Out[iodataix+0]));
				grcan_tx[grc_tpix].data[1] = (uint8)((~serial_grc.devhc[iodev_ix].ohc[1] & GIO_In_Out[iodataix+1]));
				grcan_tx[grc_tpix].data[2] = (uint8)((~serial_grc.devhc[iodev_ix].ohc[2] & GIO_In_Out[iodataix+2]));
				grcan_tx[grc_tpix].data[3] = (uint8)((~serial_grc.devhc[iodev_ix].ohc[3] & GIO_In_Out[iodataix+3]));
				grcan_tx[grc_tpix].data[4] = (uint8)((~serial_grc.devhc[iodev_ix].ohc[4] & GIO_In_Out[iodataix+4]));
				grcan_tx[grc_tpix].data[5] = (uint8)((~serial_grc.devhc[iodev_ix].ohc[5] & GIO_In_Out[iodataix+5]));
				grcan_tx[grc_tpix].data[6] = 0;
				grcan_tx[grc_tpix].data[7] = 0;
				grcan_dev[devnmb].tx_cnt++;
			}
			break;
			
		case 77:		// I/O Board Initialization
			if (((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10)))
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 8;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;

				iodev_ix = (int16)(devnmb-c_grcan_HC_1);
				iodataix = (int16)((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult); 		// input array
				if (hc_dvr_brd[iodev_ix].init_seq == 0)
				{		// Standard IO mask
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].init_seq;
					grcan_tx[grc_tpix].data[1] = 1;								// mask decode flag 1=u/d, 2=cb/vip
					grcan_tx[grc_tpix].data[2] = (uint8)(gio_valid[iodataix+0]);
					grcan_tx[grc_tpix].data[3] = (uint8)(gio_valid[iodataix+1]);
					grcan_tx[grc_tpix].data[4] = (uint8)(gio_valid[iodataix+2]);
					grcan_tx[grc_tpix].data[5] = (uint8)(gio_valid[iodataix+3]);
					grcan_tx[grc_tpix].data[6] = (uint8)(gio_valid[iodataix+4]);
					grcan_tx[grc_tpix].data[7] = (uint8)(gio_valid[iodataix+5]);
				}
				else if (hc_dvr_brd[iodev_ix].init_seq == 1)
				{		// standard IO mask
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].init_seq;
					grcan_tx[grc_tpix].data[1] = 1;								// mask decode flag 1=u/d, 2=cb/vip
					grcan_tx[grc_tpix].data[2] = (uint8)(gio_valid[iodataix+6]);
					grcan_tx[grc_tpix].data[3] = (uint8)(gio_valid[iodataix+7]);
					grcan_tx[grc_tpix].data[4] = (uint8)(gio_valid[iodataix+8]);
					grcan_tx[grc_tpix].data[5] = (uint8)(gio_valid[iodataix+9]);
					grcan_tx[grc_tpix].data[6] = (uint8)(gio_valid[iodataix+10]);
					grcan_tx[grc_tpix].data[7] = (uint8)(gio_valid[iodataix+11]);
				}
				else if (hc_dvr_brd[iodev_ix].init_seq == 2)
				{
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].init_seq;
					grcan_tx[grc_tpix].data[1] = version;
					grcan_tx[grc_tpix].data[2] = revision;
					grcan_tx[grc_tpix].data[3] = (uint8)(HcbOnBright);
					grcan_tx[grc_tpix].data[4] = (uint8)(HcbOffBright);
					if ((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] == 2))		// code blue on this board and on the aux inputs
						grcan_tx[grc_tpix].data[5] = 1;								// 1=cb on aux inputs
					else if ((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] == 4))
						grcan_tx[grc_tpix].data[5] = 2;								// 2=vip on aux inputs
					else
						grcan_tx[grc_tpix].data[5] = 0; // special control byte
					grcan_tx[grc_tpix].data[6] = 0;
					grcan_tx[grc_tpix].data[7] = 0;
				}
				else if (hc_dvr_brd[iodev_ix].init_seq == 3)
				{
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].init_seq;
					if (hc_dvr_brd[iodev_ix].riser == 1)
					{		// ir buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbIRUpOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbIRUpOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbIRUpOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbIRUpOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbIRUpOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbIRUpOffBlue & 0x00ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] != 2) && (cons[cblue] != 0))
					{	// cb buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbCBOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbCBOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbCBOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbCBOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbCBOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbCBOffBlue & 0x00ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] != 4) && (cons[VipSvc] != 0))
					{	// Vip buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbVipOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbVipOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbVipOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbVipOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbVipOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbVipOffBlue & 0x00ff);
					}
					else
					{	// std buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbUpOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbUpOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbUpOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbUpOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbUpOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbUpOffBlue & 0x00ff);
						
					}
					grcan_tx[grc_tpix].data[7] = 0;
				}
				else if (hc_dvr_brd[iodev_ix].init_seq == 4)
				{
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].init_seq;
					if (hc_dvr_brd[iodev_ix].riser == 1)
					{	// ir buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbIRDnOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbIRDnOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbIRDnOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbIRDnOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbIRDnOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbIRDnOffBlue & 0x00ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] != 2) && (cons[cblue] != 0))
					{	// cb buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbCBOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbCBOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbCBOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbCBOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbCBOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbCBOffBlue & 0x00ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] != 4) && (cons[VipSvc] != 0))
					{	// Vip buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbVipOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbVipOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbVipOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbVipOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbVipOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbVipOffBlue & 0x00ff);
					}
					else
					{	// std buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbDnOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbDnOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbDnOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbDnOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbDnOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbDnOffBlue & 0x00ff);
					}
						
					grcan_tx[grc_tpix].data[7] = 0;
				}
				else if (hc_dvr_brd[iodev_ix].init_seq == 5)
				{
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].init_seq;
					if (((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] == 2)) || 
						((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] == 4)))
					{		// aux input being used for cb or vip
						grcan_tx[grc_tpix].data[1] = 0;
						grcan_tx[grc_tpix].data[2] = 0;
						grcan_tx[grc_tpix].data[3] = 0;
					}
					else
					{
						grcan_tx[grc_tpix].data[1] = (uint8)(gio_valid[iodataix+12]);
						grcan_tx[grc_tpix].data[2] = (uint8)(gio_valid[iodataix+13]);
						grcan_tx[grc_tpix].data[3] = (uint8)(gio_valid[iodataix+14]);
					}
					grcan_tx[grc_tpix].data[4] = (uint8)(HcbSecOnRed & 0x00ff);
					grcan_tx[grc_tpix].data[5] = (uint8)(HcbSecOnGreen & 0x00ff);
					grcan_tx[grc_tpix].data[6] = (uint8)(HcbSecOnBlue & 0x00ff);
					grcan_tx[grc_tpix].data[7] = 0;
				}
				else if (hc_dvr_brd[iodev_ix].init_seq == 6)
				{
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].init_seq;
					grcan_tx[grc_tpix].data[1] = (uint8)(gio_valid[iodataix+12]);
					grcan_tx[grc_tpix].data[2] = (uint8)(gio_valid[iodataix+13]);
					grcan_tx[grc_tpix].data[3] = (uint8)(gio_valid[iodataix+14]);
					if ((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] == 2))
					{
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbCBOnRed & 0x0ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbCBOnGreen & 0x0ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbCBOnBlue & 0x0ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] == 4))
					{
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbVipOnRed & 0x0ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbVipOnGreen & 0x0ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbVipOnBlue & 0x0ff);
					}
					else
					{
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbCBOnRed & 0x0ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbCBOnGreen & 0x0ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbCBOnBlue & 0x0ff);
					}
					grcan_tx[grc_tpix].data[7] = 0;
				}
				else if (hc_dvr_brd[iodev_ix].init_seq == 7)
				{							  // init sequence is finished
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].init_seq;
					grcan_tx[grc_tpix].data[1] = 0;
					grcan_tx[grc_tpix].data[2] = 0;
					grcan_tx[grc_tpix].data[3] = 0;
					grcan_tx[grc_tpix].data[4] = 0;
					grcan_tx[grc_tpix].data[5] = 0;
					grcan_tx[grc_tpix].data[6] = 0;
					grcan_tx[grc_tpix].data[7] = 0;
				}
				grcan_dev[devnmb].tx_cnt++;
			}
			break;
			
		case 79:		// Send Security Mask and Color table
			if (((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10)))
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 7;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;

				iodev_ix = (int16)(devnmb-c_grcan_HC_1);
				iodataix = (int16)((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult); 		// OUTPUT array
				if (hc_dvr_brd[iodev_ix].color_seq == 0)
				{
					if (((Security_Type[cons[carnmb]] & 0x100) != 0) && ((HcbControl[iodev_ix] & 0x08) != 0))
					{		// security is enabled
						grcan_tx[grc_tpix].data[1] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[0] & GIO_In_Out[iodataix+15]));
						grcan_tx[grc_tpix].data[2] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[1] & GIO_In_Out[iodataix+16]));
						grcan_tx[grc_tpix].data[3] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[2] & GIO_In_Out[iodataix+17]));
					}
					else 
					{		// not on security
						grcan_tx[grc_tpix].data[1] = 0;
						grcan_tx[grc_tpix].data[2] = 0;
						grcan_tx[grc_tpix].data[3] = 0;
					}
					grcan_tx[grc_tpix].data[4] = (uint8)(HcbOnBright);
					grcan_tx[grc_tpix].data[5] = (uint8)(HcbOffBright);
					grcan_tx[grc_tpix].data[6] = 0; // special control byte
				}
				else if (hc_dvr_brd[iodev_ix].color_seq == 1)
				{			// standard IO up color
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].color_seq;
					if (hc_dvr_brd[iodev_ix].riser == 1)
					{		// ir buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbIRUpOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbIRUpOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbIRUpOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbIRUpOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbIRUpOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbIRUpOffBlue & 0x00ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] != 2) && (cons[cblue] != 0))
					{	// cb buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbCBOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbCBOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbCBOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbCBOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbCBOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbCBOffBlue & 0x00ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] != 4) && (cons[VipSvc] != 0))
					{	// Vip buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbVipOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbVipOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbVipOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbVipOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbVipOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbVipOffBlue & 0x00ff);
					}
					else
					{	// std buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbUpOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbUpOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbUpOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbUpOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbUpOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbUpOffBlue & 0x00ff);
						
					}
				}
				else if (hc_dvr_brd[iodev_ix].color_seq == 2)
				{		// standard IO down color
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].color_seq;
					if (hc_dvr_brd[iodev_ix].riser == 1)
					{	// ir buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbIRDnOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbIRDnOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbIRDnOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbIRDnOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbIRDnOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbIRDnOffBlue & 0x00ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] != 2) && (cons[cblue] != 0))
					{	// cb buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbCBOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbCBOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbCBOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbCBOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbCBOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbCBOffBlue & 0x00ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] != 4) && (cons[VipSvc] != 0))
					{	// Vip buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbVipOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbVipOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbVipOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbVipOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbVipOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbVipOffBlue & 0x00ff);
					}
					else
					{	// std buttons
						grcan_tx[grc_tpix].data[1] = (uint8)(HcbDnOnRed & 0x00ff);
						grcan_tx[grc_tpix].data[2] = (uint8)(HcbDnOffRed & 0x00ff);
						grcan_tx[grc_tpix].data[3] = (uint8)(HcbDnOnGreen  & 0x00ff);
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbDnOffGreen & 0x00ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbDnOnBlue & 0x00ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbDnOffBlue & 0x00ff);
					}
				}
				else if (hc_dvr_brd[iodev_ix].color_seq == 3)
				{		// Aux IO color
					grcan_tx[grc_tpix].data[0] = hc_dvr_brd[iodev_ix].color_seq;
					grcan_tx[grc_tpix].data[1] = (uint8)(HcbSecOnRed & 0x00ff);
					grcan_tx[grc_tpix].data[2] = (uint8)(HcbSecOnGreen & 0x00ff);
					grcan_tx[grc_tpix].data[3] = (uint8)(HcbSecOnBlue & 0x00ff);
					if ((hc_dvr_brd[iodev_ix].riser == 2) && (cons[cblue] == 2))
					{
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbCBOnRed & 0x0ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbCBOnGreen & 0x0ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbCBOnBlue & 0x0ff);
					}
					else if ((hc_dvr_brd[iodev_ix].riser == 3) && (cons[VipSvc] == 4))
					{
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbVipOnRed & 0x0ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbVipOnGreen & 0x0ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbVipOnBlue & 0x0ff);
					}
					else
					{
						grcan_tx[grc_tpix].data[4] = (uint8)(HcbCBOnRed & 0x0ff);
						grcan_tx[grc_tpix].data[5] = (uint8)(HcbCBOnGreen & 0x0ff);
						grcan_tx[grc_tpix].data[6] = (uint8)(HcbCBOnBlue & 0x0ff);
					}
				}
				grcan_tx[grc_tpix].data[7] = 0;
				grcan_dev[devnmb].tx_cnt++;
				hc_dvr_brd[iodev_ix].color_seq++;
				if (hc_dvr_brd[iodev_ix].color_seq <= 3)
					grcan_put_dev_msg_req(devnmb, 79);
			}
			break;
			
		case 90:		// Sent code blue or vip output to serial hall driver board
			if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10))
			{
				grcan_tx[grc_tpix].rtr = 0;
				grcan_tx[grc_tpix].srr = 1;
				grcan_tx[grc_tpix].len = 3;
				grcan_tx[grc_tpix].ex_id = 1;		// Extended Id
				if (grcan_dev[devnmb].online == 1)
					grcan_tx[grc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					grcan_tx[grc_tpix].one_tx = 1;
				grcan_tx[grc_tpix].msg = msg;
				grcan_tx[grc_tpix].addr = (uint8)grcan_addr[devnmb];
				grcan_tx[grc_tpix].id2 = 0;
				grcan_tx[grc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_grcan_HC_1);
				iodataix = (int16)(((IODATA_GRCAN_START + iodev_ix) * c_io_ix_mult) + 12); 		// OUTPUT array
				grcan_tx[grc_tpix].data[0] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[0] & GIO_In_Out[iodataix+3]));
				grcan_tx[grc_tpix].data[1] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[1] & GIO_In_Out[iodataix+4]));
				grcan_tx[grc_tpix].data[2] = (uint8)((~serial_grc.devhc[iodev_ix].axohc[2] & GIO_In_Out[iodataix+5]));
				grcan_tx[grc_tpix].data[3] = 0;
				grcan_tx[grc_tpix].data[4] = 0;
				grcan_tx[grc_tpix].data[5] = 0;
				grcan_tx[grc_tpix].data[6] = 0;
				grcan_tx[grc_tpix].data[7] = 0;
				grcan_dev[devnmb].tx_cnt++;
			}
			break;

   		default:
			break;
	}
}

//************************************************************
//*********  Transmit Can Message  ***************************
//************************************************************

void tx_grcan_msg(int16 TxBufIx)
{
	_disable();
	if (grc_tgix != grc_tpix)
	{
		
		switch (TxBufIx)
		{
			
			case 8:
				if (((GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (grcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (GR_CAN.IFLAG1.B.BUF08I == 1)
						GR_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
					grcan_transmit(TxBufIx);
					GR_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
					grcan_tx_B08_fail_cnt = 0;
					break;
				}
				else
				{
					grcan_tx_B08_fail_cnt++;
					if (grcan_tx_B08_fail_cnt > 5)
					{
						grcan_tx_complete[TxBufIx] = 1;
						grcan_tx_B08_fail_cnt = 0;
						GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
					 
			//lint -fallthrough
			case 9:
				if (((GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (grcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (GR_CAN.IFLAG1.B.BUF09I == 1)
						GR_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
					grcan_transmit(TxBufIx);
					GR_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
					grcan_tx_B09_fail_cnt = 0;
					break;
				}
				else
				{
					grcan_tx_B09_fail_cnt++;
					if (grcan_tx_B09_fail_cnt > 5)
					{
						grcan_tx_complete[TxBufIx] = 1;
						grcan_tx_B09_fail_cnt = 0;
						GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			//lint -fallthrough
			case 10:
				if (((GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (grcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (GR_CAN.IFLAG1.B.BUF10I == 1)
						GR_CAN.IFLAG1.B.BUF10I = 1;		// Reset IF
					grcan_transmit(TxBufIx);
					GR_CAN.IFLAG1.B.BUF10I = 1;		// Reset IF
					grcan_tx_B10_fail_cnt = 0;
					break;
				}
				else
				{
					grcan_tx_B10_fail_cnt++;
					if (grcan_tx_B10_fail_cnt > 5)
					{
						grcan_tx_complete[TxBufIx] = 1;
						grcan_tx_B10_fail_cnt = 0;
						GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			//lint -fallthrough
			case 11:
				if (((GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (grcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (GR_CAN.IFLAG1.B.BUF11I == 1)
						GR_CAN.IFLAG1.B.BUF11I = 1;		// Reset IF
					grcan_transmit(TxBufIx);
					GR_CAN.IFLAG1.B.BUF11I = 1;		// Reset IF
					grcan_tx_B11_fail_cnt = 0;
					break;
				}
				else
				{
					grcan_tx_B11_fail_cnt++;
					if (grcan_tx_B11_fail_cnt > 5)
					{
						grcan_tx_complete[TxBufIx] = 1;
						grcan_tx_B11_fail_cnt = 0;
						GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
				}
				break;
			default:
				break;
		}
	}
	_enable();
}


//************************************************************
//*********  Transmit Buffer  ********************************
//************************************************************

void grcan_transmit (int16 TxBufIx)
{
	int16 i;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
	if (grc_tgix != grc_tpix)
	{		// There is data in the buffer 

		grcan_tx_complete[TxBufIx] = 0;

		if (grc_tgix < c_grcan_tx_buf_siz)
			grc_tgix++;
		else
			grc_tgix = 0;

		if (grcan_tx[grc_tgix].len > 8)
		{
			grcan_tx[grc_tgix].len = 8;
			grcan_error |= 0x40;
		}
			

		if (grcan_tx[grc_tgix].ex_id == 1)
		{
			uiu.b[hb] = grcan_tx[grc_tgix].msg;		// Message
			uiu.b[mhb] = grcan_tx[grc_tgix].addr;	// Address
			uiu.b[mlb] = 0;
			uiu.b[lb] = 0;
		 	GR_CAN.MB[TxBufIx].MSG.MID.R = uiu.dw >> 3;		// priority bits get set to zero on shift
		}
		else
		{
			uiu.b[hb] = (uint8)(grcan_tx[grc_tgix].msg & 0x3F);		// Message
			uiu.b[mhb] = (uint8)(grcan_tx[grc_tgix].addr << 3);	// Address
			uiu.b[mlb] = 0;
			uiu.b[lb] = 0;
		 	GR_CAN.MB[TxBufIx].MSG.MID.R = uiu.dw >> 3;		// priority bits get set to zero on shift
		}


		i = 0;
		while ((i<grcan_tx[grc_tgix].len) && (i < 8))
		{
		 	GR_CAN.MB[TxBufIx].MSG.MDATA.B[i++] = grcan_tx[grc_tgix].data[i];
		}
		

		GR_CAN.MB[TxBufIx].MSG.MCS.B.LENGTH = grcan_tx[grc_tgix].len;
	 	
		if (grcan_tx[grc_tgix].rtr == 1)
			GR_CAN.MB[TxBufIx].MSG.MCS.B.RTR = 1;
	 	else
			GR_CAN.MB[TxBufIx].MSG.MCS.B.RTR = 0;

		if (grcan_tx[grc_tgix].ex_id == 1)
			GR_CAN.MB[TxBufIx].MSG.MCS.B.IDE = 1;
	 	else
			GR_CAN.MB[TxBufIx].MSG.MCS.B.IDE = 0;

		if (grcan_tx[grc_tgix].srr == 1)
			GR_CAN.MB[TxBufIx].MSG.MCS.B.SRR = 1;
	 	else
			GR_CAN.MB[TxBufIx].MSG.MCS.B.SRR = 0;
	 	
	 	
		GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x0C;		// Initiate transmit
		grcan_debug_1 = GR_CAN.MB[TxBufIx].MSG.MCS.B.CODE;
	}
}

//************************************************************
//*********  Process the Can Serial Data *********************
//************************************************************

void grcan_control (void)
{
	static int16 ix;
	int16 i;
	unsigned char msg_req = 0;

	unsigned char devix,iodevix,devnmb,tdevix;
	uint16 iodataix;
	uint8 io_mask = 0;
	uint8 shift_amt = 0;
	uint8 shift_ix = 0;

	if (nmb_grcan_dev > 0)
	{
		unpack_grcan_in();

		
		if (re_init_grcan == 1)
		{
			re_init_grcan = 0;
//			record_fault(f_grcan_init_error);
//			init_grcan_bus();
		}

		grcan_status = GR_CAN.ESR.R;
		if (GR_CAN.ESR.B.ERRINT == 1)
		{
			grcan_status = GR_CAN.ESR.R;
			GR_CAN.ESR.B.ERRINT = 1;
		}
		else
			grcan_status = 0;
		
		grcan_tx_err_cnt = (uint8)GR_CAN.ECR.B.TXECNT;
		grcan_rx_err_cnt = (uint8)GR_CAN.ECR.B.RXECNT;

		if (GR_CAN.ESR.B.BOFFINT == 1)
		{		// The CAN bus is off
			GR_CAN.ESR.B.BOFFINT = 1;
			grcan_bus_off_cnt++;
			grcan_error |= 0x04;
			init_grcan_bus();
//			record_fault(f_grcan_bus_off);
		}
		else if (grcan_bus_off_cnt > 0)
			grcan_bus_off_cnt--;

#if (Simulator == 0)
		if ((grcan_status & 0x00000080) != 0)
		{		// Bus is idle
			grcan_bus_idle_cnt++;
//			grcan_error |= 0x08;
//			record_fault(f_grcan_bus_idle);		// bus can go idle while car sitting at the floor
		}
		else if (grcan_bus_idle_cnt > 0)
			grcan_bus_idle_cnt--;

		if ((grcan_status & 0x00002000) != 0)
		{		// Transmit acknowledge error
		
			grcan_ack_error_cnt++;
			grcan_error |= 0x10;
//			record_fault(f_grcan_ack_err);	
		}
		else if (grcan_ack_error_cnt > 0)
			grcan_ack_error_cnt--;
#endif


		if (IsGrcTxBufFul() == 0)
		{	// buffer is not full	  OK to write to buffer
			if ((grcan_msg_req != 0) && (grcan_dev_req != 0) && (grcan_msg_cmd != 0) && (grcan_data_req == 0))
			{
				grcan_data_req = 1;
				if ((((grcan_dev_req >= c_grcan_START) && (grcan_dev_req <= c_grcan_END)) || (grcan_dev_req == c_grcan_GLOBAL)) && (grcan_msg_cmd <= 2))
				{
					pack_grcan_out((uint8)grcan_msg_req,(uint8)grcan_dev_req,(uint8)grcan_msg_cmd-1);		// Pack Message request 
				}
				if (grcan_msg_cnt > 0)
					grcan_msg_cnt--;
				if (grcan_msg_cnt == 0)
				{
					grcan_msg_req = 0;
					grcan_dev_req = 0;
					grcan_msg_cmd = 0;
				}
			}
		}	 

		if (IsGrcTxBufFul() == 0) 
		{			// Decide who is the Dispatcher
			
			if (Dispatcher == 99)
			{
				if (timers[tgrcan_master] >= (uint16)(cons[carnmb] + 3))
				{
					timers[tgrcan_master] = 0;
					pack_grcan_out( 1,(uint8)cons[carnmb],0);		// Request Dispatcher 

					if ((dispatcher_count > 8) && (Group_Comm == 0))		// Don't switch if someone else is already master on the group 485 bus
					{
						if ((gc_valid_start == 1) || (gc_valid_addr == 1))	// receiving some valid data on the group comm		
						{
							gc_valid_start = 0;
							gc_valid_addr = 0;
						}
						else
						{
							Dispatcher = cons[carnmb];
							for (i=1;i<=cons[nmbcars];i++)
							{	
								timers[tgcom_car+i] = 0;    // restart car timers
							}
						}
					}
					
				}
				timers[tgrcan_master_req] = 0;		
			}
			else if (Dispatcher == cons[carnmb])
			{
				if (timers[tgrcan_master] >= 5)
				{
					timers[tgrcan_master] = 0;
					pack_grcan_out( 1,(uint8)cons[carnmb],0);		// Request Dispatcher 
				}
				timers[tgrcan_master_req] = 0;		
			}
			else
			{
				if (timers[tgrcan_master_req] >= 20)
				{		// do not update when timer is zero
					
					timers[tgrcan_master_req] = 0;		// request for dispatcher every 2 seconds
					pack_grcan_out( 1,(uint8)cons[carnmb],0);		// Request Dispatcher 
				}
				
				if (timers[tgrcan_master] >= 50)		// no request for dispatcher for 10 seconds	   (if someone else is dispatcher this time-out will occur twice)
				{
					timers[tgrcan_master] = 0;
					if ((gc_valid_start == 1) || (gc_valid_addr == 1))	// receiving some valid data on the group comm		
					{
						gc_valid_start = 0;
						gc_valid_addr = 0;
					}
					else
						Dispatcher = 99;
				}
			}

		}
		
		car_on_grcan = 0;
		
		for (i=1;i<= cons[nmbcars];i++)
		{
			if (i != cons[carnmb])
			{
				if (timers[tgrcan_car+i] >= 40)
				{
					timers[tgrcan_car+i] = 0;
					grcan_car_online[i] = 0;
				}
				else if (grcan_car_online[i] == 1)
					car_on_grcan = 1;
			}
		}
				
		
		if (Dispatcher == cons[carnmb])
		{
			
			// Communicate with Serial Devices and Expansion I/O boards
			i = 0;
			while ((i < nmb_grcan_dev) && (IsGrcTxBufFul() == 0))
			{
				if (ix < (nmb_grcan_dev-1))
					ix++;
				else
					ix = 0;
				
				devnmb = grcan_dev_list[ix];	// get device number from list
				devix = devnmb;
				
				if (cons[Voltage_Sensor] == 3)
				{
					if ((timers[tvsupd] >= 30) && (devnmb == c_grcan_VS)) // every 3 seconds
					{	  // Request Voltage from voltage board
						if (grcan_data_req == 0)
						{
							grcan_data_req = 1;
							
							msg_req = grcan_get_dev_msg_req(devnmb);
							if (msg_req != 0)
							{
								pack_grcan_out(msg_req,c_grcan_VS,0);
							}
							if (grcan_dev[devix].version == 0)
								pack_grcan_out(1,c_grcan_VS,0);	// send init packet
							else
								pack_grcan_out(44,c_grcan_VS,1);	// data request controller voltage
							timers[tvsupd] = 0;;

#if (Simulator == 1)
							grcan_dev[devix].online = 1;
							vs_can_online = 1;
#else
							if (grcan_dev[devix].online_cnt > 0)
								grcan_dev[devix].online_cnt--;
							if (grcan_dev[devix].online_cnt > 0)
							{
								vs_can_online = 1;
								grcan_dev[devix].online = 1;
							}
							else
							{
								vs_can_online = 0;
								grcan_dev[devix].online = 0;
							}
#endif
						}
					}
				}
				else if (cons[Voltage_Sensor] == 0)
					vs_can_online = 0;
				

				// I/O Board Data

				if (((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_40) && ((cons[sercom] & 0x01) == 0)) ||
					((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_SEB_12) && ((cons[sercom] & 0x01) != 0)))
				{		 // Valid device number
					iodevix = devnmb-c_grcan_SEB_1;
					tdevix = devnmb - c_grcan_SEB_1;
					iodataix = (int16)((IODATA_GRCAN_START + iodevix) * 3); 		// input array

					// *********************** Output device *********************
						 
					if (((giodata(iodataix+0) & GIO_In_Out[iodataix+0]) != (serial_grc.dev[iodevix].iob[0] & GIO_In_Out[iodataix+0])) ||
						((giodata(iodataix+1) & GIO_In_Out[iodataix+1]) != (serial_grc.dev[iodevix].iob[1] & GIO_In_Out[iodataix+1])) ||
						((giodata(iodataix+2) & GIO_In_Out[iodataix+2]) != (serial_grc.dev[iodevix].iob[2] & GIO_In_Out[iodataix+2]))) 
					{		// io data for the output has changed
						serial_grc.dev[iodevix].iob[0] = (uint8)((serial_grc.dev[iodevix].iob[0] & ~GIO_In_Out[iodataix+0]) | (giodata(iodataix+0) & GIO_In_Out[iodataix+0]));
						serial_grc.dev[iodevix].iob[1] = (uint8)((serial_grc.dev[iodevix].iob[1] & ~GIO_In_Out[iodataix+1]) | (giodata(iodataix+1) & GIO_In_Out[iodataix+1]));
						serial_grc.dev[iodevix].iob[2] = (uint8)((serial_grc.dev[iodevix].iob[2] & ~GIO_In_Out[iodataix+2]) | (giodata(iodataix+2) & GIO_In_Out[iodataix+2]));
						pack_grcan_out(13,devnmb,0);	// Data has changed so pack the data
						timers[tgrcantx + tdevix] = 0;
						sync_grc.dev[iodevix].rtx_cnt = 0;
					}
					else if (((sync_grc.dev[iodevix].iob[0] & GIO_In_Out[iodataix+0]) != (serial_grc.dev[iodevix].iob[0] & GIO_In_Out[iodataix+0])) ||
							 ((sync_grc.dev[iodevix].iob[1] & GIO_In_Out[iodataix+1]) != (serial_grc.dev[iodevix].iob[1] & GIO_In_Out[iodataix+1])) ||
							 ((sync_grc.dev[iodevix].iob[2] & GIO_In_Out[iodataix+2]) != (serial_grc.dev[iodevix].iob[2] & GIO_In_Out[iodataix+2]))) 
					{		// device has not updated it's outputs yet
						if (timers[tgrcantx + tdevix] >= 2)
						{
							pack_grcan_out(13,devnmb,0);	// Data has changed so pack the data
							timers[tgrcantx + tdevix] = 0;
							if (sync_grc.dev[iodevix].rtx_cnt > 10)
							{
								sync_grc.dev[iodevix].iob[0] = (uint8)((sync_grc.dev[iodevix].iob[0] & ~GIO_In_Out[iodataix+0]) | (serial_grc.dev[iodevix].iob[0] & GIO_In_Out[iodataix+0]));
								sync_grc.dev[iodevix].iob[1] = (uint8)((sync_grc.dev[iodevix].iob[1] & ~GIO_In_Out[iodataix+1]) | (serial_grc.dev[iodevix].iob[1] & GIO_In_Out[iodataix+1]));
								sync_grc.dev[iodevix].iob[2] = (uint8)((sync_grc.dev[iodevix].iob[2] & ~GIO_In_Out[iodataix+2]) | (serial_grc.dev[iodevix].iob[2] & GIO_In_Out[iodataix+2]));
								sync_grc.dev[iodevix].rtx_cnt = 0;
							}
							else
								sync_grc.dev[iodevix].rtx_cnt++;
						}
					}
					else if ((timers[tgrcantx + tdevix] > 5) && (grcan_data_req == 0))
					{		 // Update outputs
						grcan_data_req = 1;
						serial_grc.dev[iodevix].iob[0] = (uint8)((serial_grc.dev[iodevix].iob[0] & ~GIO_In_Out[iodataix+0]) | (giodata(iodataix+0) & GIO_In_Out[iodataix+0]));
						serial_grc.dev[iodevix].iob[1] = (uint8)((serial_grc.dev[iodevix].iob[1] & ~GIO_In_Out[iodataix+1]) | (giodata(iodataix+1) & GIO_In_Out[iodataix+1]));
						serial_grc.dev[iodevix].iob[2] = (uint8)((serial_grc.dev[iodevix].iob[2] & ~GIO_In_Out[iodataix+2]) | (giodata(iodataix+2) & GIO_In_Out[iodataix+2]));
						pack_grcan_out(13,devnmb,0);	// Data has changed so pack the data
						timers[tgrcantx + tdevix] = 0;
						sync_grc.dev[iodevix].rtx_cnt = 0;
					}
					
					// *****************  Input check **********************

					if (sync_grc.dev[iodevix].reflect_data == 1)
					{		 // Send back what was sent so the device can verify it was sent ok
						sync_grc.dev[iodevix].reflect_data = 0;	// Must be cleared before data is packed - if new data comes in during
						grcan_debug_3++;
						pack_grcan_out(15,devnmb,0);			//  the pack, it must be re-sent
					}
					else 
					{
						msg_req = grcan_get_dev_msg_req(devnmb);
						if (msg_req != 0)
						{
							pack_grcan_out(msg_req,devnmb,0);
						}
						else if (timers[tgrcanrx + tdevix] > 7)
						{
							if (grcan_dev[devix].version == 0)
							{
								pack_grcan_out(11,devnmb,0);	// send init packet
								timers[tgrcanrx + tdevix] = 0;
								if (grcan_dev[devix].online_cnt > 0)
									grcan_dev[devix].online_cnt--;
							}
							else if (grcan_data_req == 0)
							{
								grcan_data_req = 1;
								pack_grcan_out(14,devnmb,1);	// data request
								timers[tgrcanrx + tdevix] = 0;
								if (grcan_dev[devix].online_cnt > 0)
									grcan_dev[devix].online_cnt--;
							}
						}
					}
					
					if (grcan_dev[devix].online_cnt > 0)
					{
						grcan_dev[devix].online = 1;
						
					}
					else
					{

#if (Simulator == 0)
						if (grcan_dev[devix].online == 1)
						{
							fault_device = grcan_addr[devnmb];
	//						record_fault(f_seb_grcan_error);
						}
#endif
						grcan_dev[devix].online = 0;
#if (Simulator == 0)
			 			serial_grc.dev[iodevix].iob[0] &= GIO_In_Out[iodataix+0];	  // clear input data since device is not on-line
			 			serial_grc.dev[iodevix].iob[1] &= GIO_In_Out[iodataix+1];
			 			serial_grc.dev[iodevix].iob[2] &= GIO_In_Out[iodataix+2];
#endif
					}
				}
				
				// Serial Hall Call Driver Data
				
				if ((devnmb >= c_grcan_HC_1) && (devnmb <= c_grcan_HC_10) && ((cons[sercom] & 0x01) == 0))
				{		 // Valid device number
					iodevix = devnmb-c_grcan_HC_1;
					tdevix = devnmb - c_grcan_HC_1;
					iodataix = (int16)((IODATA_GRCAN_START + iodevix) * c_io_ix_mult); 		// input or output array

					// *********************** Output device *********************
						 
					if (((giodata(iodataix+6) & GIO_In_Out[iodataix+6]) != (serial_grc.devhc[iodevix].ohc[0] & GIO_In_Out[iodataix+6])) ||
						((giodata(iodataix+7) & GIO_In_Out[iodataix+7]) != (serial_grc.devhc[iodevix].ohc[1] & GIO_In_Out[iodataix+7])) ||
						((giodata(iodataix+8) & GIO_In_Out[iodataix+8]) != (serial_grc.devhc[iodevix].ohc[2] & GIO_In_Out[iodataix+8])) ||
						((giodata(iodataix+9) & GIO_In_Out[iodataix+9]) != (serial_grc.devhc[iodevix].ohc[3] & GIO_In_Out[iodataix+9])) ||
						((giodata(iodataix+10) & GIO_In_Out[iodataix+10]) != (serial_grc.devhc[iodevix].ohc[4] & GIO_In_Out[iodataix+10])) ||
						((giodata(iodataix+11) & GIO_In_Out[iodataix+11]) != (serial_grc.devhc[iodevix].ohc[5] & GIO_In_Out[iodataix+11])))
					{		// io data for the output has changed
						serial_grc.devhc[iodevix].ohc[0] = (uint8)((serial_grc.devhc[iodevix].ohc[0] & ~GIO_In_Out[iodataix+6]) | (giodata(iodataix+6) & GIO_In_Out[iodataix+6]));
						serial_grc.devhc[iodevix].ohc[1] = (uint8)((serial_grc.devhc[iodevix].ohc[1] & ~GIO_In_Out[iodataix+7]) | (giodata(iodataix+7) & GIO_In_Out[iodataix+7]));
						serial_grc.devhc[iodevix].ohc[2] = (uint8)((serial_grc.devhc[iodevix].ohc[2] & ~GIO_In_Out[iodataix+8]) | (giodata(iodataix+8) & GIO_In_Out[iodataix+8]));
						serial_grc.devhc[iodevix].ohc[3] = (uint8)((serial_grc.devhc[iodevix].ohc[3] & ~GIO_In_Out[iodataix+9]) | (giodata(iodataix+9) & GIO_In_Out[iodataix+9]));
						serial_grc.devhc[iodevix].ohc[4] = (uint8)((serial_grc.devhc[iodevix].ohc[4] & ~GIO_In_Out[iodataix+10]) | (giodata(iodataix+10) & GIO_In_Out[iodataix+10]));
						serial_grc.devhc[iodevix].ohc[5] = (uint8)((serial_grc.devhc[iodevix].ohc[5] & ~GIO_In_Out[iodataix+11]) | (giodata(iodataix+11) & GIO_In_Out[iodataix+11]));
						pack_grcan_out(73,devnmb,0);	// Data has changed so pack the data
						timers[tgrcantx + tdevix] = 0;
						sync_grc.devhc[iodevix].rtx_hc_cnt = 0;
					}
					
					if (((giodata(iodataix+15) & GIO_In_Out[iodataix+15]) != (serial_grc.devhc[iodevix].axohc[0] & GIO_In_Out[iodataix+15])) ||
						((giodata(iodataix+16) & GIO_In_Out[iodataix+16]) != (serial_grc.devhc[iodevix].axohc[1] & GIO_In_Out[iodataix+16])) ||
						((giodata(iodataix+17) & GIO_In_Out[iodataix+17]) != (serial_grc.devhc[iodevix].axohc[2] & GIO_In_Out[iodataix+17])))
					{		// io data for the output has changed
						serial_grc.devhc[iodevix].axohc[0] = (uint8)((serial_grc.devhc[iodevix].axohc[0] & ~GIO_In_Out[iodataix+15]) | (giodata(iodataix+15) & GIO_In_Out[iodataix+15]));
						serial_grc.devhc[iodevix].axohc[1] = (uint8)((serial_grc.devhc[iodevix].axohc[1] & ~GIO_In_Out[iodataix+16]) | (giodata(iodataix+16) & GIO_In_Out[iodataix+16]));
						serial_grc.devhc[iodevix].axohc[2] = (uint8)((serial_grc.devhc[iodevix].axohc[2] & ~GIO_In_Out[iodataix+17]) | (giodata(iodataix+17) & GIO_In_Out[iodataix+17]));
						if (((hc_dvr_brd[iodevix].riser == 2) && (cons[cblue] == 2)) || 
							((hc_dvr_brd[iodevix].riser == 3) && (cons[VipSvc] == 4)))
						{		// code blue or vip using aux i/o
							pack_grcan_out(90,devnmb,0);	// Data has changed so pack the data
						}
						timers[tgrcantx + tdevix] = 0;
						sync_grc.devhc[iodevix].rtx_hc_cnt = 0;
					}
  
					if ((timers[tgrcantx + tdevix] > 5) && (grcan_data_req == 0))
					{		 // Update outputs
						grcan_data_req = 1;
							
						if (hc_dvr_brd[iodevix].upd_seq == 0)
						{
							hc_dvr_brd[iodevix].upd_seq = 1;
							serial_grc.devhc[iodevix].ohc[0] = (uint8)((serial_grc.devhc[iodevix].ohc[0] & ~GIO_In_Out[iodataix+6]) | (giodata(iodataix+6) & GIO_In_Out[iodataix+6]));
							serial_grc.devhc[iodevix].ohc[1] = (uint8)((serial_grc.devhc[iodevix].ohc[1] & ~GIO_In_Out[iodataix+7]) | (giodata(iodataix+7) & GIO_In_Out[iodataix+7]));
							serial_grc.devhc[iodevix].ohc[2] = (uint8)((serial_grc.devhc[iodevix].ohc[2] & ~GIO_In_Out[iodataix+8]) | (giodata(iodataix+8) & GIO_In_Out[iodataix+8]));
							serial_grc.devhc[iodevix].ohc[3] = (uint8)((serial_grc.devhc[iodevix].ohc[3] & ~GIO_In_Out[iodataix+9]) | (giodata(iodataix+9) & GIO_In_Out[iodataix+9]));
							serial_grc.devhc[iodevix].ohc[4] = (uint8)((serial_grc.devhc[iodevix].ohc[4] & ~GIO_In_Out[iodataix+10]) | (giodata(iodataix+10) & GIO_In_Out[iodataix+10]));
							serial_grc.devhc[iodevix].ohc[5] = (uint8)((serial_grc.devhc[iodevix].ohc[5] & ~GIO_In_Out[iodataix+11]) | (giodata(iodataix+11) & GIO_In_Out[iodataix+11]));
							pack_grcan_out(73,devnmb,0);	// Data has changed so pack the data
							
						}
						else if (hc_dvr_brd[iodevix].upd_seq == 1)
						{
							hc_dvr_brd[iodevix].upd_seq = 2;
							pack_grcan_out(72,devnmb,0);	// control update
							
							if ((cons[cblue] == 0) && (cons[VipSvc] == 0))
							{
								hc_dvr_brd[iodevix].upd_seq = 0;
								timers[tgrcantx + tdevix] = 0;
							}
						}
						else if (hc_dvr_brd[iodevix].upd_seq == 2)
						{
							hc_dvr_brd[iodevix].upd_seq = 0;
							if (((hc_dvr_brd[iodevix].riser == 2) && (cons[cblue] == 2)) || 
								((hc_dvr_brd[iodevix].riser == 3) && (cons[VipSvc] == 4)))
							{		// code blue or vip using aux i/o
								serial_grc.devhc[iodevix].axohc[0] = (uint8)((serial_grc.devhc[iodevix].axohc[0] & ~GIO_In_Out[iodataix+15]) | (giodata(iodataix+15) & GIO_In_Out[iodataix+15]));
								serial_grc.devhc[iodevix].axohc[1] = (uint8)((serial_grc.devhc[iodevix].axohc[1] & ~GIO_In_Out[iodataix+16]) | (giodata(iodataix+16) & GIO_In_Out[iodataix+16]));
								serial_grc.devhc[iodevix].axohc[2] = (uint8)((serial_grc.devhc[iodevix].axohc[2] & ~GIO_In_Out[iodataix+17]) | (giodata(iodataix+17) & GIO_In_Out[iodataix+17]));
		  						pack_grcan_out(90,devnmb,0);	// Data has changed so pack the data
							}
							timers[tgrcantx + tdevix] = 0;

						}
						sync_grc.devhc[iodevix].rtx_hc_cnt = 0;
					}
					
					// *****************  Input check **********************

/*					if (sync_grc.devhc[iodevix].reflect_hc_data == 1)
					{		 // Send back what was sent so the device can verify it was sent ok
						sync_grc.devhc[iodevix].reflect_hc_data = 0;	// Must be cleared before data is packed - if new data comes in during
						grcan_debug_3++;
						pack_grcan_out(75,devnmb,0);			//  the pack, it must be re-sent
					}
					else  */
					msg_req = grcan_get_dev_msg_req(devnmb);
					if (msg_req != 0)
					{
						pack_grcan_out(msg_req,devnmb,0);
					}
					else if ((msg_req = (uint8)get_pkt_req(devnmb,GCANCOM)) != 0)
					{
						pack_grcan_out(msg_req,devnmb,1);
					}
					else if (timers[tgrcanrx + tdevix] > 7)
					{
						if (grcan_dev[devix].version == 0)
						{
							pack_grcan_out(77,devnmb,0);	// send init packet
							timers[tgrcanrx + tdevix] = 0;
							if (grcan_dev[devix].online_cnt > 0)
								grcan_dev[devix].online_cnt--;
						}
						else if (grcan_data_req == 0)
						{
							grcan_data_req = 1;
							pack_grcan_out(74,devnmb,1);	// request inputs
							timers[tgrcanrx + tdevix] = 0;
							if (grcan_dev[devix].online_cnt > 0)
								grcan_dev[devix].online_cnt--;
						}
					}
						
					if (grcan_dev[devix].online_cnt > 0)
					{
						grcan_dev[devix].online = 1;
						hc_dvr_brd[iodevix].online = 1;
					}
					else
					{

#if (Simulator == 0)
						if (grcan_dev[devix].online == 1)
						{
							fault_device = grcan_addr[devnmb];
	//						record_fault(f_seb_grcan_error);
						}
#endif
						grcan_dev[devix].online = 0;
						hc_dvr_brd[iodevix].init_seq = 0;
						hc_dvr_brd[iodevix].init_count = 0;
						hc_dvr_brd[iodevix].online = 0;
#if (Simulator == 0)
						iodataix = (int16)((IODATA_GRCAN_START + iodevix) * c_io_ix_mult); 		// input or output array
			 			serial_grc.devhc[iodevix].ihc[0] &= GIO_In_Out[iodataix+0];	  // clear input data since device is not on-line
			 			serial_grc.devhc[iodevix].ihc[1] &= GIO_In_Out[iodataix+1];
			 			serial_grc.devhc[iodevix].ihc[2] &= GIO_In_Out[iodataix+2];
			 			serial_grc.devhc[iodevix].ihc[3] &= GIO_In_Out[iodataix+3];
			 			serial_grc.devhc[iodevix].ihc[4] &= GIO_In_Out[iodataix+4];
			 			serial_grc.devhc[iodevix].ihc[5] &= GIO_In_Out[iodataix+5];
			 			serial_grc.devhc[iodevix].axihc[0] &= GIO_In_Out[iodataix+12];
			 			serial_grc.devhc[iodevix].axihc[1] &= GIO_In_Out[iodataix+13];
			 			serial_grc.devhc[iodevix].axihc[2] &= GIO_In_Out[iodataix+14];
#endif
					}
				}
				
				
				if ((devnmb >= c_grcan_HCB_1) && (devnmb <= c_grcan_HCB_48) && ((cons[sercom] & 0x01) != 0))
				{		 // Valid device number
					iodevix = devnmb-c_grcan_HCB_1;
					tdevix = devnmb - c_grcan_SEB_1;
					shift_ix = ((iodevix & 0x01) != 0);
					shift_amt = shift_ix * 4;
					iodataix = (int16)(HCB_IODATA_GRCAN_START + iodevix/2); 		// input array
					io_mask = GIO_In_Out[iodataix] & shift_mask[shift_ix];
					
					// *********************** Output device *********************
					
					if ((giodata(iodataix) & io_mask) != (serial_grc.brd_io[iodataix] & io_mask))
					{		// pack data out
					   	serial_grc.brd_io[iodataix] = (uint8)((serial_grc.brd_io[iodataix] & ~io_mask ) | (giodata(iodataix) & io_mask));
						pack_grcan_out(13,devnmb,0);	// Data has changed so pack the data
						timers[tgrcantx + tdevix] = 0;
						sync_hcb[iodevix].rtx_cnt = 0;
					}
					else if ((sync_hcb[iodevix].iob & io_mask) != (serial_grc.brd_io[iodataix] & io_mask))
					{		// device has not updated it's outputs yet
						if (timers[tgrcantx + tdevix] >= 2)
						{
							pack_grcan_out(13,devnmb,0);	// Data has changed so pack the data
							timers[tgrcantx + tdevix] = 0;
							if (sync_hcb[iodevix].rtx_cnt > 10)
							{
								sync_hcb[iodevix].iob = (uint8)((sync_hcb[iodevix].iob & ~io_mask) | (serial_grc.brd_io[iodataix] & io_mask));
								sync_hcb[iodevix].rtx_cnt = 0;
							}
							else
								sync_hcb[iodevix].rtx_cnt++;
						}
					}
					else if ((timers[tgrcantx + tdevix] > 5) && (grcan_data_req == 0))
					{		 // Update outputs
						grcan_data_req = 1;
					   	serial_grc.brd_io[iodataix] = (uint8)((serial_grc.brd_io[iodataix] & ~io_mask) | (giodata(iodataix) & io_mask));
						pack_grcan_out(13,devnmb,0);	// Data has changed so pack the data
						timers[tgrcantx + tdevix] = 0;
						sync_hcb[iodevix].rtx_cnt = 0;
					}
					
					// *****************  Input check **********************

					if (sync_hcb[iodevix].reflect_data == 1)
					{		 // Send back what was sent so the device can verify it was sent ok
						sync_hcb[iodevix].reflect_data = 0;	// Must be cleared before data is packed - if new data comes in during
						grcan_debug_3++;
						pack_grcan_out(15,devnmb,0);			//  the pack, it must be re-sent
					}
					else 
					{
						msg_req = grcan_get_dev_msg_req(devnmb);
						if (msg_req != 0)
						{
							pack_grcan_out(msg_req,devnmb,0);
						}
						else if (timers[tgrcanrx + tdevix] > 7)
						{
							if (grcan_dev[devix].version == 0)
							{
								pack_grcan_out(11,devnmb,0);	// send init packet
								timers[tgrcanrx + tdevix] = 0;
								if (grcan_dev[devix].online_cnt > 0)
									grcan_dev[devix].online_cnt--;
							}
							else if (grcan_data_req == 0)
							{
								grcan_data_req = 1;
								pack_grcan_out(14,devnmb,1);	// data request
								timers[tgrcanrx + tdevix] = 0;
								if (grcan_dev[devix].online_cnt > 0)
									grcan_dev[devix].online_cnt--;
							}
						}
					}
				}
				
				i++;		
			}
		}
		GrcanTxBufIx ++;
		if (GrcanTxBufIx > 11)
			GrcanTxBufIx = 8;
		tx_grcan_msg(GrcanTxBufIx);		// Check if data waiting and transmit
		if (grcan_data_req == 0)
			grcan_data_req_cnt = 0;
		else if (grcan_data_req_cnt >= 1)
		{						// Cause a delay between sending packet request instead of sending all at once
			grcan_data_req_cnt = 0;
			grcan_data_req = 0;
		}
		else
			grcan_data_req_cnt++;
	}
}


/* Revision History
6/21/12 v6.0.9 mhd	1. Created new can bus
12/30/13 v7.0.31 mhd	1. Changed availbuf check in IsGrcTxBufFul() from > 4 to > 9, Also changed c_grcan_tx_buf_siz and c_grcan_rx_buf_siz from 20 to 30.
						2. Moved _disable() and _enable() routines in tx_can_msg() to have interrupts disable during the entire routine. If the grc_tgix and grc_tpix indexes are checked or the 
						   (CAN_A.MB[TxBufIx].MSG.MCS.B.CODE) transmit status is checked and then a transmit interrupt occurs, a transmit() routine can be called when the transmitter is not ready
						   causing an invalid packet. 
						3. Added grcan_tx_complete flag to prevent the tx_grcan_msg() routine from writing over the buffer of a partially transmitted packet.
						4. Added data_req check for Voltage Sensor device packet. 
						5. Added data_req check for I/O board timer update outputs.
						6. Rotate buffer index when calling tx_grcan_msg(index).
						7, In packet 47, do not send label if name is not valid.
4/1/14 v7.0.39 mhd		1. In tx_grcan_msg() added _enable() at end of routine.  It was mistakenly changes to _disable() somehow?
5/27/14 v7.1.0 mhd		1. Change timers[tgrcan_master] time-out from 100 to 50 so time-out is 10 seconds instead of 20 (time-out can occur twice).
7/21/15 v7.1.40 mhd		1. Changed hall call driver board structure to use 18 bytes instead of 12.  Added 6 bytes for aux inputs and outputs.
9/21/15 v7.1.41 mhd		1. Corrected hall call driver board initialization for code blue and vip boards.
10/9/15 v7.1.43 mhd		1. Added can message request que.
12/3/15 v7.1.51 mhd		1. Change the grcan_data_req_cnt check from 4 to 1.
*/

