#define d_CTCAN 1
#include "global.h"
#define Door_Test 0

extern int16 Add_msg_to_array_B(char *msg, int16 index);
extern int16 Log_an_event(char *msg);

/*
	Car Top Can for GALaxy 4

	This protocol is setup with the main CPU of the elevator as the master device and all 
	other can devices as slave devices.  The identifier consist of a message number and 
	a device address number.  When a messaged is sent to a slave device, it will include 
	the address of that particular slave device.  When a slave device responds, the address 
	is its own address showing where the message is comming from.  In short all data
	on the bus is being transmitted between master and slave and not from slave to slave.  
	
	The baud rate is set to 115200 baud using an 18.432 MHz crystal. Device address assignments
	and message information is listed below.

		Can Port Device Map

	Device		  Number
	 Car			1
	 SEL1			2		  Primary Selector
     SPB	 		5		  Safety processor board
	 SEL2			6		  Secondary Selector
	 DOOR		    7		  Front Door Operator
	 RDOOR			8		  Rear Door Operator
	 LW				9		  Load Weigher
	 TSVS			10		  Temperature Sensor/Voltage Sensor
     SEB_1	 		11		  Serial I/O Expansion board 1	(io brd 7)
     SEB_2	 		12		  Serial I/O Expansion board 2	(io brd 8)
     SEB_3	 		13		  Serial I/O Expansion board 3	(io brd 9)
     SEB_4	 		14		  Serial I/O Expansion board 4	(io brd 10)
     SEB_5	 		15		  Serial I/O Expansion board 5	(io brd 11)
     SEB_6	 		16		  Serial I/O Expansion board 6	(io brd 12)
     SEB_7	 		17		  Serial I/O Expansion board 7	(io brd 13)
     SEB_8	 		18		  Serial I/O Expansion board 8   (io brd 14)
     SEB_9	 		19		  Serial I/O Expansion board 9   (io brd 15)
     PI_LANT 		20		  Serial PI and Lantern (global address for all pi and lantern devices)
     SEB_10	 		21		  Serial I/O Expansion board 10  (io brd 16)
     SEB_11	 		22		  Serial I/O Expansion board 11  (io brd 17)
     SEB_12	 		23		  Serial I/O Expansion board 12  (io brd 18) 
     SEB_13	 		24		  Serial I/O Expansion board 13  (io brd 19) 
     SEB_14	 		25		  Serial I/O Expansion board 14  (io brd 20) 
     SEB_15	 		26		  Serial I/O Expansion board 15  (io brd 21) 
     SEB_16	 		27		  Serial I/O Expansion board 16  (io brd 22) 
     SEB_17	 		28		  Serial I/O Expansion board 17  (io brd 23) 
     SEB_18	 		29		  Serial I/O Expansion board 18  (io brd 24) 
     SEB_19	 		30		  Serial I/O Expansion board 19  (io brd 25) 
     SEB_20	 		31		  Serial I/O Expansion board 20  (io brd 26)
     SEB_21	 		32		  Serial I/O Expansion board 21  (io brd 27)
     SEB_22	 		33		  Serial I/O Expansion board 22  (io brd 28)
     SEB_23	 		34		  Serial I/O Expansion board 23  (io brd 29) 
     SEB_24	 		35		  Serial I/O Expansion board 24  (io brd 30) 
     SEB_25	 		36		  Serial I/O Expansion board 25  (io brd 31) 
     SEB_26	 		37		  Serial I/O Expansion board 26  (io brd 32) 
     SEB_27	 		38		  Serial I/O Expansion board 27  (io brd 33) 
     SEB_28	 		39		  Serial I/O Expansion board 28  (io brd 34) 
     SEB_29	 		40		  Serial I/O Expansion board 29  (io brd 35) 
     SEB_30	 		41		  Serial I/O Expansion board 30  (io brd 36)
     SEB_31	 		42		  Serial I/O Expansion board 31  (io brd 37)
     SEB_32	 		43		  Serial I/O Expansion board 32  (io brd 38)
     SEB_33	 		44		  Serial I/O Expansion board 33  (io brd 39) 
     SEB_34	 		45		  Serial I/O Expansion board 34  (io brd 40) 
     SEB_35	 		46		  Serial I/O Expansion board 35  (io brd 41) 
     SEB_36	 		47		  Serial I/O Expansion board 36  (io brd 42) 
     SEB_37	 		48		  Serial I/O Expansion board 37  (io brd 43) 
     SEB_38	 		49		  Serial I/O Expansion board 38  (io brd 44) 
     SEB_39	 		50		  Serial I/O Expansion board 39  (io brd 45) 
     SEB_40	 		51		  Serial I/O Expansion board 40  (io brd 46)
     END	 		51		  Last can device
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
	3		CPU			SPB				4		Message #3:	Temperature 
													low byte external temperature
													high byte external temperature
													low byte board temperature
													high byte board temperature
													
	10		ALL			CPU				1		Message #10: Can device error
													can error byte

	11		SEB1-9		CPU				5		Message #11: Init data with software version
													master version
													master revision
													call latch mask
													call latch mask
													call latch mask

	12		CPU			ALL				2		Message #12: Software version
													Device version
													Device revision

	13		SEB1-40		CPU				3-5		Message #13: Set/Clear Output Serial Expansion Board 1-40 (SEB1-40) (bit set for each output)
												  	Output 1-8   or Unused
												  	Output 9-16  or Output 1-4 bits 4-8
												  	Output 17-24 or Output 5-12 bits 0-8
													checksum low byte			// Car call color and brightness checksum
													checksum high byte			
													checksum low byte			// Label Checksum
													checksum high byte
													checksum byte				// Color index checksum

	14		CPU			SEB1-7			3-4		Message #14: Read Input Serial Expansion Board 1-40 (SEB1-40) (bit set for each input)
												  	Input 1-8	or Input 1-8 bits 0-8
												  	Input 9-16	or Input 9-12 bits 0-3
												  	Input 17-24 or Unused
													Local Comm Bit0: 1st board or Unused
															   Bit1: 2nd board
															   Bit2: Altarnate car station 1st board
															   Bit3: Alternate car station 2nd board

	15		SEB1-7		CPU				3		Message #15: Transmit back of inputs that were sent for verification
												  	Input 1-8	or Input 1-8 bits 0-8
												  	Input 9-16	or Input 9-12 bits 0-3
												  	Input 17-24 or Unused

	16		CPU			SEB1-7			3		Message #16: Read back each input sent to device for verification
												  	Output 1-8   or Unused
												  	Output 9-16  or Output 1-4 bits 4-8
												  	Output 17-24 or Output 5-12 bits 0-8

	21		LW			CPU				1		Message #21: Enable Load Weigher Reading
													Byte 0: Bit0: 1 = Read load, 0 = Disable reading
													 		Bit1: 1 = Re-calibrate
															Bit2-7: Unused
													Byte 1-7: Unused

	22		CPU			LW				2		Message #22: Load value
													Byte 0: Load weigher low byte
													Byte 1: Load Weigher high byte
													Byte 2: Return command byte
													Byte 3: Error Code
													Byte 4-7: Unused

	23		SEL,SPB	 	CPU				5		Message #23: Selector Initialization
													Byte 0: Command
															Bit 0: 1 = Preset position Count 
													Byte 1: Low Byte of preset Position
													Byte 2: Middle Low Byte of preset Position
													Byte 3: Middle High Byte of presset Position
													Byte 4: High Byte of preset Position
													Byte 5-7: Unused

	24 		CPU			SEL				5		Message #24: Selector Position
													Byte 0: Low Byte of Position
													Byte 1: Middle Low Byte of Position
													Byte 2: Middle High Byte of Position
													Byte 3: High Byte of Position
													Byte 4: Status
														Bit0: Hoistway Learned
														Bit1: In Hw Learn
														Bit2: Fault
														Bits3-7: Unused
													Byte 5-7: Unused

	25		PI			CPU				8	   	Message #25: PI and Lantern Data
												  	Car position		Binary position
												  	Ascii PI 0			Ascii Position low byte
												  	Ascii PI 1			Ascii Position High byte

													Lantern Status		Bit 7: LUP (lantern up)
																		Bit 6: LDN (lantern down)
																		Bit 5: RLUP (rear lantern up)
																		Bit 4: RLDN (rear lantern down)
																		Bit 3: UDA (up direction arrow)
																		Bit 2: DDA (down direction arrow)
																		Bit 1: User Message Bit 1
																		Bit 0: User Message Bit 0

												  	Motion Status		Bit 7: CULR UPREF (up preference)
												  						Bit 6: CDLR DNPREF (down preference)
												  						Bit 5: CUL run (car running)
												  						Bit 4: CDL leveling (car leveling)
												  						Bit 3: Out of Service
																		Bit 2: Inspection
																		Bit 1: Independent
																		Bit 0: unused

													Fire Status			Bit 7: PH2 (fire phase 2)
																		Bit 6: PH1 Complete (fire phase 1 complete)
																		Bit 5: PH1 Recall Main (fire phase 1 recall active)
																		Bit 4: EMP Recall (emergency power recall active)
												  						Bit 3: EMP Recall Complete (emergency power recall complete)
												  						Bit 2: Power Loss (Normal power is out)
												  						Bit 1: Imminent Shutdown
																		Bit 0: PH1 Recall Alternate (fire phase 1 recall active)

													Door Status 		Bits 3,2,1,0
												  						doorf = 0 doors closed no action
												  						doorf = 1 doors are opening
												  						doorf = 2 doors are dwellingse)
																		doorf = 3 doors are closing
																		doorf = 4 doors are nudging closed

																		Bits 7,6,5,4
													  					rdoorf = 0 doors closed no action
													  					rdoorf = 1 doors are opening
													  					rdoorf = 2 doors are dwelling
													  					rdoorf = 3 doors are closing
													  					rdoorf = 4 doors are nudging closed

													Misc Status			Bit 7: Activate speech
																		Bit 6: At Lobby (car at lobby floor)
																		Bit 5: Overload (load overload)
																		Bit 4: Load Bypass
																		Bit 3: EMT (Emergency Medical Technician Service)
																		Bit 2: Code Blue
																		Bit 1: Seismic
																		Bit 0: User Message Bit 2 

																		User Message Bits 2,1,0
																						  0 0 0	No message
																						  0 0 1 Message 1
																						  0 1 0 Message 2
																						  0 1 1 Message 3
																						  1 0 0 Message 4
																						  1 0 1 Message 5
																						  1 1 0 Message 6
																						  1 1 1 Message 7
																						   
	26		AHA			CPU				8	   	Message #26: Attendant Hall Annunciator
												  	Byte 0:				Up hall call assignment 1-8
													Byte 1:				Up hall call assignment 9-16
													Byte 2: 			Up hall call assignment 17-24
													Byte 3: 			Up hall call assignment 25-32
												  	Byte 4:				Down hall call assignment 1-8
													Byte 5:				Down hall call assignment 9-16
													Byte 6: 			Down hall call assignment 17-24
													Byte 7: 			Down hall call assignment 25-32

	27		AHA			CPU				8	   	Message #27: Rear Attendant Hall Annunciator
												  	Byte 0:				Rear Up hall call assignment 1-8
													Byte 1:				Rear Up hall call assignment 9-16
													Byte 2: 			Rear Up hall call assignment 17-24
													Byte 3: 			Rear Up hall call assignment 25-32
												  	Byte 4:				Rear Down hall call assignment 1-8
													Byte 5:				Rear Down hall call assignment 9-16
													Byte 6: 			Rear Down hall call assignment 17-24
													Byte 7: 			Rear Down hall call assignment 25-32

	28		DOOR		CPU				3		Message # 28: Door Control
													Byte 0:
													    Bit 0: Open command   1 = Open
													    Bit 1: Close Command 1 = Close
													    Bit 2: Nudge Close	      1 = Close at nudging speed
													    Bit 3: Door Zone Status   1 = on dz
													    Bit 4: Unused
													    Bit 5: Heavy Door 1=Use heavy door parameters
													    Bit 6: Narrow Door 1 = Use narrow door limit
													    Bit 7: Unused
													Byte 1: Return verification for Byte 0 Msg# 29
														Bit 0: Door Open Limit Return Value
														Bit 1: Door Close Limit Return Value
														Bit 2: DPM limit return value
														Bit 3: Unused
														Bit 4: Reopen overspeed return value
														Bit 5: Reopen over torque return value
														Bit 6: Reopen EE/Detector Edge return value
														Bit 7: Unused
													Byte 2: Return Value display Request (max of 16 values)
														0 = Fault Code
														1 = Output Voltage
														2 = Output Current
														3 = Output Frequency
														4-15 not defined at this time
																
	29		CPU			DOOR			5		Message # 29: Door Status
													Byte 0:	 Status 1 Bits
													    Bit 0: Door Open Limit
													    Bit 1: Door Close Limit
													    Bit 2: DPM Limit
													    Bit 3: Unused 
													    Bit 4: Reopen overspeed 1=over speed detected so reopen
													    Bit 5: Reopen over torque 1=over torque detected so reopen
													    Bit 6: Reopen ee/de 1=ee or de detected
													    Bit 7: Unused
													Byte 1: Status 2 Bits
													    Bit 0: Fault
					 								    Bit 1: Door opening 1 = door moving in the open direction
													    Bit 2: Door closing 1 = door moving in the close direction
													    Bit 3: Unused
													    Bit 7,6,5,4: Monitor Value Returned  0-15 
														0 = Fault Code
														1 = Output Voltage
														2 = Output Current
														3 = Output Frequency
														4-15 not defined at this time
													Byte 2: Monitor Value High Byte 
													Byte 3:	Monitor Value Low Byte		
													Byte 4: Return Verification Data for Byte 0 MSG #28						    
														Bit 0: Door Open Return (Follows the state of Door Open Command)
													    Bit 1: Door Close Return (Follows the state of Door lose Command)
													    Bit 2: Door Nudging Return (Set = 1 when door running nudging speed)
										    			Bit 3: DZ Return (Follows the state of DZ status)	
													    Bit 4: Unused
												 	    Bit 5: Heavy Door Return (Follows the state of the Heavy Door Command) 
													    Bit 6: Narrow Door Return (Follows the state of the Narrow Door Command)
													    Bit 7: Unused

	30		DOOR		CPU				5		Message # 30: Parameter read/write request
													Byte 0:
													    Bit 0: Parameter Read/Write 1 = write
													    Bit 1-7: Unused
													Byte 1:	 Parameter Address high byte
													Byte 2: Parameter Address low byte
													Byte 3:	 Parameter Data high byte
													Byte 4:	 Parameter Data low byte
																
	31		CPU			DOOR			8		Message # 31: Parameter Return Data
													Byte 0:	 Error Flags
														0 = Okay
														1 = Illegal command code
														2 = Illegal data address
														3 = Illegal data value
													Byte 1:	Parameter Address high byte (data in 6 and 7)
													Byte 2: Parameter Address low byte (data in 6 and 7)						
													Byte 3:	Parameter High Byte
													Byte 4:	Parameter Low byte


	34 		CPU			SEL				6		Message #34: Selector Position
													Byte 0: Status
														Bit0: Hoistway Learned
														Bit1: In Hw Learn
														Bit2: Fault
														Bits3-7: Unused
													Byte 1: Floor position
														Bit0-5: floor position
														Bit6: Unused
														Bit7: Unused	
													Byte 2: door zone inputs 
														Bit0: UL
														Bit1: DZ
														Bit2: DZA
														Bit3: DL
														Bit4-7: Unused
													Byte 3: Up Slowdown Inputs
														Bit0: UN
														Bit1: UT
														Bit2: UT1
														Bit3: UT2
														Bit4: UT3
														Bit5: UT4
														Bit6: UT5
														Bit7: UT6
													Byte 4: Down Slowdown Inputs
														Bit0: DN
														Bit1: DT
														Bit2: DT1
														Bit3: DT2
														Bit4: DT3
														Bit5: DT4
														Bit6: DT5
														Bit7: DT6
													Byte 5: Terminal Limit Inputs
														Bit0: DTS
														Bit1: UTS
														Bit2: Unused
														Bit3: Unused
														Bit4: Unused
														Bit5: Unused
														Bit6: Unused
														Bit7: Unused

	35 		SEL			CPU				6		Message #35: Selector Position RETURN packet
													Byte 0: Status
														Bit0: Hoistway Learned
														Bit1: In Hw Learn
														Bit2: Fault
														Bits3-7: Unused
													Byte 1: Floor position
														Bit0-5: floor position
														Bit6: Unused
														Bit7: Unused	
													Byte 2: door zone inputs 
														Bit0: UL
														Bit1: DZ
														Bit2: DZA
														Bit3: DL
														Bit4-7: Unused
													Byte 3: Up Slowdown Inputs
														Bit0: UN
														Bit1: UT
														Bit2: UT1
														Bit3: UT2
														Bit4: UT3
														Bit5: UT4
														Bit6: UT5
														Bit7: UT6
													Byte 4: Down Slowdown Inputs
														Bit0: DN
														Bit1: DT
														Bit2: DT1
														Bit3: DT2
														Bit4: DT3
														Bit5: DT4
														Bit6: DT5
														Bit7: DT6
													Byte 5: Terminal Limit Inputs
														Bit0: DTS
														Bit1: UTS
														Bit2: Unused
														Bit3: Unused
														Bit4: Unused
														Bit5: Unused
														Bit6: Unused
														Bit7: Unused

	36		SEL		 	CPU				6		Message #36: Selector Command
													Byte 0: Command
															Bit 0: 1 = Learn Floor position 
															Bit 1: 1 = reset fault
															Bit 2: 1 = clear fault table
															Bit 3: 1 = Preset position Count
													Byte 1: Floor Learn position
													Byte 2: Number of floors
													Byte 3: low byte position count
													Byte 4: mid byte position count
													Byte 5: high byte position count
													Byte 6-7: Unused

	37		CPU		 	SEL				6		Message #37: Selector Command RETURN
													Byte 0: Command
															Bit 0: 1 = Learn Floor position 
															Bit 1: 1 = reset fault
															Bit 2: 1 = clear fault table
															Bit 3: 1 = Preset position Count
													Byte 1: Floor Learn position
													Byte 2: Number of floors
													Byte 3: low byte position count
													Byte 4: mid byte position count
													Byte 5: high byte position count
													Byte 6-7: Unused

	38 		SEL			SEL				4		Message #38: Selector Learn Status (selector to selector)
													Byte 0: Command
														Bit 0: 1 = Learn Floor position 
														Bit 1: 1 = reset fault
														Bit 2: 1 = clear fault table
													Byte 1: Floor Learn Position
													Byte 2: Number of Floors
													Byte 3: Status
														Bit 0: 1 = Hoistway Learned
														Bit 1: 1 = In Hw Learn
														Bit 2: 1 = Fault

	39 		SEL			SEL				4		Message #39: Selector Learn Status RETURN (selector to selector)
													Byte 0: Command
														Bit 0: 1 = Learn Floor position 
														Bit 1: 1 = reset fault
														Bit 2: 1 = clear fault table
													Byte 1: Floor Learn Position
													Byte 2: Number of Floors
													Byte 3: Status
														Bit 0: 1 = Hoistway Learned
														Bit 1: 1 = In Hw Learn
														Bit 2: 1 = Fault

	40 		CPU			SEL				8		Message #40: Selector Faults
													Byte 0: Fault Index
													Byte 1: fault[index]
													Byte 2: fault[index+1]
													Byte 3: fault[index+2]
													Byte 4: fault[index+3]
													Byte 5: fault[index+4]
													Byte 6: fault[index+5]
													Byte 7: fault[index+6]

	41 		SEB			SEL				6		Message #41: Selector Position
													Byte 0: Status
														Bit0: Hoistway Learned
														Bit1: In Hw Learn
														Bit2: Fault
														Bits3-7: Unused
													Byte 1: Floor position
														Bit0-5: floor position
														Bit6: Unused
														Bit7: Unused	
													Byte 2: door zone outputs 
														Bit0: UL
														Bit1: DZ
														Bit2: DZA
														Bit3: DL
														Bit4-7: Unused
													Byte 3: Up Slowdown Inputs
														Bit0: UN
														Bit1: UT
														Bit2: UT1
														Bit3: UT2
														Bit4: UT3
														Bit5: UT4
														Bit6: UT5
														Bit7: UT6
													Byte 4: Down Slowdown Inputs
														Bit0: DN
														Bit1: DT
														Bit2: DT1
														Bit3: DT2
														Bit4: DT3
														Bit5: DT4
														Bit6: DT5
														Bit7: DT6
													Byte 5: Terminal Limit Inputs
														Bit0: DTS
														Bit1: UTS
														Bit2: Unused
														Bit3: Unused
														Bit4: Unused
														Bit5: Unused
														Bit6: Unused
														Bit7: Unused

	42 		SEL			SEB				6		Message #42: Selector Position RETURN packet
													Byte 0: Status
														Bit0: Hoistway Learned
														Bit1: In Hw Learn
														Bit2: Fault
														Bits3-7: Unused
													Byte 1: Floor position
														Bit0-5: floor position
														Bit6: Unused
														Bit7: Unused	
													Byte 2: door zone inputs 
														Bit0: UL
														Bit1: DZ
														Bit2: DZA
														Bit3: DL
														Bit4-7: Unused
													Byte 3: Up Slowdown Inputs
														Bit0: UN
														Bit1: UT
														Bit2: UT1
														Bit3: UT2
														Bit4: UT3
														Bit5: UT4
														Bit6: UT5
														Bit7: UT6
													Byte 4: Down Slowdown Inputs
														Bit0: DN
														Bit1: DT
														Bit2: DT1
														Bit3: DT2
														Bit4: DT3
														Bit5: DT4
														Bit6: DT5
														Bit7: DT6
													Byte 5: Terminal Limit Inputs
														Bit0: DTS
														Bit1: UTS
														Bit2: Unused
														Bit3: Unused
														Bit4: Unused
														Bit5: Unused
														Bit6: Unused
														Bit7: Unused

	43		ALL			CPU				8	   	Message #43: Car Call Light Brightness and Color
												  	Byte 0:	Location Index = 0 - Brightness/Security   (will cycle 0 through max color index on one request)
														Byte 1:	Max Color Index	 (Up to OTS = 9)
														Byte 2: Off Brightness
														Byte 3: On Brightness
													  	Byte 4:	Security Red Intensity		 		// Color index 1
														Byte 5:	Security Green Intensity
														Byte 6: Security Blue Intensity
														Byte 7: 0
												  	Byte 0:	Location Index = 1 - Attendant Up/Down
														Byte 1:	Attendant Up Red Intensity			// Color index 2
														Byte 2: Attendant Up Green Intensity
														Byte 3: Attendant Up Blue Intensity
													  	Byte 4:	Attendant Down Green Intensity		// Color index 3
														Byte 5:	Attendant Down Blue Intensity
														Byte 6: Attendant Down Blue Intensity
														Byte 7: 0
												  	Byte 0:	Location Index = 2 - On/Off
														Byte 1:	Off Red Intensit					// Color index 4
														Byte 2: Off Green Intensity
														Byte 3: Off Blue Intensity
													  	Byte 4:	On Red Intensity					// Color index 5
														Byte 5:	On Green Intensity
														Byte 6: On Blue Intensity
														Byte 7: 0
												  	Byte 0:	Location Index = 3 - Fire/Medical Service
														Byte 1:	Fire Red Intensit					// Color index 6
														Byte 2: Fire Green Intensity
														Byte 3: Fire Blue Intensity
													  	Byte 4:	Medical Red Intensit				// Color index 7
														Byte 5:	Medical Green Intensity
														Byte 6: Medical Blue Intensity
														Byte 7: 0
												  	Byte 0:	Location Index = 4 - Emergency/OTS
														Byte 1:	Emergency Red Intensity				// Color index 8
														Byte 2: Emergency Green Intensity
														Byte 3: Emergency Blue Intensity
													  	Byte 4:	OTS Red Intensity					// Color index 9
														Byte 5:	OTS Green Intensity
														Byte 6: OTS Blue Intensity
														Byte 7: 0

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
													Byte 6: On Color Index		(If zero then i/o has no color association)
													Byte 7: Off Color Index		
																Logic:
																	if zero then i/o turned completely off when off)
																		On Color Index  then  Off Color Index
																			0						0
																			1 Security Color		4 Off Color 
																			2 Att UP				4
																			3 Att DN				4
																			4 Off Color				4
																			5 On Color				4

																											Backlight On
																			6 Fire 					fvbklton	bit0: 0 = 0 Full off, 1 = 4 Off Color
																			7 Medical				fvbklton	bit1: 0 = 0 Full off, 1 = 4 Off Color
																			8 Emergency				fvbklton	bit2: 0 = 0 Full off, 1 = 4 Off Color 
																			9 OTS					fvbklton	bit3: 0 = 0 Full off, 1 = 4 Off Color
																										Special Case for Att Up/Dn Lights
																			2 or 3 Att Up/Dn		fvbklton	bit4: 0 = 0 Full off, 1 = 4 Off Color
																			4 Non-CC (on color)		fvbklton	bit5: 0 = 0 Full off, 1 = 4 Off Color

																	
	48 		CPU			any DEV				2		Message 48: Device error
													Byte 0: Fault Code	(Specific code for device)
													Byte 1: Fault Count


	49		any DEV		CPU					1		Message 49: Reset Device Error
													Byte 0: Control bit 0: 1 = reset error condition
																	bit 1: 1 = clear error code buffer


	50		AHA			CPU				8	   	Message #49: Attendant Hall Annunciator
												  	Byte 0:				Up hall call assignment 33-40
													Byte 1:				Up hall call assignment 41-48
													Byte 2: 			Up hall call assignment 49-56
													Byte 3: 			Up hall call assignment 57-64
												  	Byte 4:				Down hall call assignment 33-40
													Byte 5:				Down hall call assignment 41-48
													Byte 6: 			Down hall call assignment 49-56
													Byte 7: 			Down hall call assignment 57-64

	51		AHA			CPU				8	   	Message #50: Rear Attendant Hall Annunciator
												  	Byte 0:				Rear Up hall call assignment 33-40
													Byte 1:				Rear Up hall call assignment 41-48
													Byte 2: 			Rear Up hall call assignment 49-56
													Byte 3: 			Rear Up hall call assignment 57-64
												  	Byte 4:				Rear Down hall call assignment 33-40
													Byte 5:				Rear Down hall call assignment 41-48
													Byte 6: 			Rear Down hall call assignment 49-56
													Byte 7: 			Rear Down hall call assignment 57-64

	52		ALL			CPU				1		Message #52: Time Clock Update
													Byte 0: Time increment counter
													Byte 1: fvcansynccnt Can sync count, 0 = no sync
													Byte 2: Cop Light Status: 1=Attup, 2=Attdn, 4=Sec Off

	53		SEB 		CPU				6		Message #53: Set/Clear Attendant Mask RGB Boards
												  	Attendant Up Call (match car call location)
												  	Attendant Up Call (match car call location)
												  	Attendant Up Call (match car call location)
												  	Attendant Down Call (match car call location)
												  	Attendant Down Call (match car call location)
												  	Attendant Down Call (match car call location)

	54		CPU			SEB1-7			6		Message #54: Return Attendant Mask RGB Boards
												  	Attendant Up Call (match car call location)
												  	Attendant Up Call (match car call location)
												  	Attendant Up Call (match car call location)
												  	Attendant Down Call (match car call location)
												  	Attendant Down Call (match car call location)
												  	Attendant Down Call (match car call location)

	55		SEB 		CPU				3		Message #55: Set/Clear Security Mask RGB Boards
												  	Security Call (match car call location)
												  	Security Call (match car call location)
												  	Security Call (match car call location)

	56		CPU			SEB1-7			3		Message #56: Return Security Mask RGB Boards
												  	Security Call (match car call location)
												  	Security Call (match car call location)
												  	Security Call (match car call location)

	57 		CPU			RGBCC			1		Message 57: Request Board I/O Color Indexes
													Byte 0:  Request flag must be set to 1

	58 		RGBCC		CPU				7		Message 58: Color Index for RGB board I/Os
													Byte 0: I/O Location Index    0   1   2   3     4     5     6     7
													Byte 1: On Color Index	  i/o 1	  4	  7	  10	13    16    19	  22
													Byte 2: Off Color Index		  1	  4	  7	  10	13	  16	19	  22
													Byte 3: On Color Index		  2	  5	  8	  11	14	  17	20	  23
													Byte 4: Off Color Index		  2	  5	  8	  11	14	  17	20	  23
													Byte 5: On Color Index		  3	  6	  9	  12	15	  18	21	  24
													Byte 6: Off Color Index		  3	  6	  9	  12	15	  18	21	  24
	59		CPU			SEL				1		Message 59: Dead Zone Selection
													Byte 0:	UL Dead zone parameter low byte
													Byte 1:	DL Dead zone parameter low byte
													
	60		SEL			CPU				1		Message 60: Dead Zone Selection
													Byte 0:	UL Dead zone parameter low byte
													Byte 1: Spb debug
													Byte 2:	DL Dead Zone low byte
													
													
	80-87 Reserved for file transfer on can bus	
	110-115 ANY			CPU				Packets reserved for testing.
												    
													
													

*/

#define c_max_ctcan_dev 55	// 55 devices 
#define c_max_ctcan_io_brd 40	// 40 i/o boards 
#define c_max_ctcan_ser_array (c_max_ctcan_io_brd * 3)

int16 ctcan_addr[c_max_ctcan_dev] = 
{ 0, 2, 6, 7, 8, 9,10,20,
 11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,30,31,
 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
 255,52,53,54,55,56,57};

#define  c_ctcan_START	  1		  // Can start index
#define  c_ctcan_ENC_SEL  1		  // Encoded Tape Selector
#define  c_ctcan_SEL	  2		  // Selector board
#define  c_ctcan_DOOR	  3		  // Front Door board
#define  c_ctcan_RDOOR	  4		  // Rear Door Board
#define  c_ctcan_LW		  5		  // Load weigher
#define  c_ctcan_VS		  6		  // Voltage Sensor
#define  c_ctcan_PI_LANT  7		  // Serial PI and Lantern (global address for all pi and lantern devices)
#define	 c_ctcan_SEB_1	  8		  // Serial I/O Expansion board 1	(io brd 7)
#define	 c_ctcan_SEB_2	  9		  // Serial I/O Expansion board 2	(io brd 8)
#define	 c_ctcan_SEB_3	 10		  // Serial I/O Expansion board 3	(io brd 9)
#define	 c_ctcan_SEB_4	 11		  // Serial I/O Expansion board 4	(io brd 10)
#define	 c_ctcan_SEB_5	 12		  // Serial I/O Expansion board 5	(io brd 11)
#define	 c_ctcan_SEB_6	 13		  // Serial I/O Expansion board 6	(io brd 12)
#define	 c_ctcan_SEB_7	 14		  // Serial I/O Expansion board 7	(io brd 13)
#define  c_ctcan_SEB_8	 15		  // Serial I/O Expansion board 8   (io brd 14)
#define  c_ctcan_SEB_9	 16		  // Serial I/O Expansion board 9   (io brd 15)
#define  c_ctcan_SEB_10	 17		  // Serial I/O Expansion board 10  (io brd 16)
#define  c_ctcan_SEB_11	 18		  // Serial I/O Expansion board 11  (io brd 17)
#define  c_ctcan_SEB_12	 19		  // Serial I/O Expansion board 12  (io brd 18) 
#define  c_ctcan_SEB_13	 20		  // Serial I/O Expansion board 13  (io brd 19) 
#define  c_ctcan_SEB_14	 21		  // Serial I/O Expansion board 14  (io brd 20) 
#define  c_ctcan_SEB_15	 22		  // Serial I/O Expansion board 15  (io brd 21) 
#define  c_ctcan_SEB_16	 23		  // Serial I/O Expansion board 16  (io brd 22) 
#define  c_ctcan_SEB_17	 24		  // Serial I/O Expansion board 17  (io brd 23) 
#define  c_ctcan_SEB_18	 25		  // Serial I/O Expansion board 18  (io brd 24) 
#define  c_ctcan_SEB_19	 26		  // Serial I/O Expansion board 19  (io brd 25) 
#define  c_ctcan_SEB_20	 27		  // Serial I/O Expansion board 20  (io brd 26)
#define  c_ctcan_SEB_21	 28		  // Serial I/O Expansion board 21  (io brd 27)
#define  c_ctcan_SEB_22	 29		  // Serial I/O Expansion board 22  (io brd 28)
#define  c_ctcan_SEB_23	 30		  // Serial I/O Expansion board 23  (io brd 29) 
#define  c_ctcan_SEB_24	 31		  // Serial I/O Expansion board 24  (io brd 30) 
#define  c_ctcan_SEB_25	 32		  // Serial I/O Expansion board 25  (io brd 31) 
#define  c_ctcan_SEB_26	 33		  // Serial I/O Expansion board 26  (io brd 32) 
#define  c_ctcan_SEB_27	 34		  // Serial I/O Expansion board 27  (io brd 33) 
#define  c_ctcan_SEB_28	 35		  // Serial I/O Expansion board 28  (io brd 34) 
#define  c_ctcan_SEB_29	 36		  // Serial I/O Expansion board 29  (io brd 35) 
#define  c_ctcan_SEB_30	 37		  // Serial I/O Expansion board 30  (io brd 36)
#define  c_ctcan_SEB_31	 38		  // Serial I/O Expansion board 31  (io brd 37)
#define  c_ctcan_SEB_32	 39		  // Serial I/O Expansion board 32  (io brd 38)
#define  c_ctcan_SEB_33	 40		  // Serial I/O Expansion board 33  (io brd 39) 
#define  c_ctcan_SEB_34	 41		  // Serial I/O Expansion board 34  (io brd 40) 
#define  c_ctcan_SEB_35	 42		  // Serial I/O Expansion board 35  (io brd 41) 
#define  c_ctcan_SEB_36	 43		  // Serial I/O Expansion board 36  (io brd 42) 
#define  c_ctcan_SEB_37	 44		  // Serial I/O Expansion board 37  (io brd 43) 
#define  c_ctcan_SEB_38	 45		  // Serial I/O Expansion board 38  (io brd 44) 
#define  c_ctcan_SEB_39	 46		  // Serial I/O Expansion board 39  (io brd 45) 
#define  c_ctcan_SEB_40	 47		  // Serial I/O Expansion board 40  (io brd 46)
#define  c_ctcan_END	 47		  // Last can device
#define  c_ctcan_GLOBAL	 48		  // Global can address

#define IODATA_CTCAN_START 46		// 47 - 1


#define c_ctcan_COP c_ctcan_SEB_1
#define c_ctcan_SEL_DZ c_ctcan_SEB_2
#define c_ctcan_SEL_LIM c_ctcan_SEB_3
#define c_ctcan_DCL_DOL c_ctcan_SEB_4
#define c_ctcan_DCLR_DOLR c_ctcan_SEB_5

#define c_Freeze_Start		0x01	// put chip in freeze mode
#define c_Freeze_Stop		0x00	// Takes chip out of freeze mode


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

int16 ctcan_put_dev_msg_req (int16 dev, int8 msg);
int8 ctcan_get_dev_msg_req (int16 dev);
void init_ctcan_bus (void);
//void write_can (uint8 addr, uint8 data);
//uint8 read_can (uint8 addr);
void clr_ctcan_dev_comm_loss (void);
uint16 calc_ioname_can_cksum (unsigned char devnmb);
void wrong_ctcan_int (void);
void ctcan_bus_off_and_warning_int (void);
void ctcan_error_int (void);
void ctcan_rcv_int (void);
void ctcan_rcv_fifo_warning_int (void);
void ctcan_rcv_fifo_overflow_int (void);
void ctcan_trm_int_b8 (void);
void ctcan_trm_int_b9 (void);
void ctcan_trm_int_b10 (void);
void ctcan_trm_int_b11 (void);
void ctcan_control (void);
void receive_ctcan_in (int16 BufIx);
void unpack_ctcan_in (void);
int16 ChipFreezeMode(unsigned char set);
void calc_ctcan_devices_color_ix_chksum(void);
void tx_ctcan_msg(int16 TxBufIx);
void transmit (int16 TxBufIx);
void set_pi_lant_status (void);
void set_pi_cc_bits(void);
void set_dd_cc_pi_dpy(void);
void cc_sec_att_sync (void);
void update_cc_sec(void);
void update_att_hc_ann(void);
void set_door_command (void);
void set_rdoor_command (void);
int16 blind(void);
int16 valid_second_board (unsigned char addr);
unsigned char calc_io_color_ix_cksum (unsigned char devnmb);
int16 IsTxBufFul(void);
void pack_ctcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req);


unsigned char ctcan_sync_count = 0;
unsigned char cop_lt_status = 0;		// cop light status 1=AttUP, 2=AttDn, 4=Sec
unsigned char lt_status_count = 0;

int16 sel_can_error = 0;		// Prevent running the car when the selector is not communicating
int16 selector_ul_deadzone = 0;
int16 selector_dl_deadzone = 0;
int16 sel_can_online = 0;
int16 lw_can_online = 0;
int16 cop_can_error = 0;		// Prevent movement of the car on automatic when a cop device error
int16 cop_can_online = 0;
int16 vs_can_online = 0;

// Declarations for Serial I/O Boards

union tag_ser_ct {

	union 
	{
		uint8 iob[3];
		struct
		{
			uint8 io1:1;
			uint8 io2:1;
			uint8 io3:1;
			uint8 io4:1;
			uint8 io5:1;
			uint8 io6:1;
			uint8 io7:1;
			uint8 io8:1;
		}B[3];
		struct
		{
			uint8 nib1:4;
			uint8 nib2:4;
		}N[3];
	} dev [c_max_ctcan_io_brd];	
	
	unsigned char brd_io [c_max_ctcan_ser_array];
}serial_ctc;


struct tag_sync {
		unsigned char iob[3];
		unsigned char reflect_data;
		unsigned char rtx_cnt;
} sync_dev_ctc [c_max_ctcan_io_brd];


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

#define ctcan_que_len 3

struct tag_ctcan_dev {
		unsigned char online;
		unsigned char online_cnt;
		unsigned char version;
		unsigned char revision;
		unsigned char msg_req[ctcan_que_len];
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
} ctcan_dev[c_max_ctcan_dev];

int16 ctcan_dev_req = 0;
int16 ctcan_msg_req = 0;
int16 ctcan_msg_cmd = 0;
int16 ctcan_msg_cnt = 0;

int16 color_send_ix = 0;

unsigned char ctc_ioset[c_max_ctcan_ser_array];
unsigned char ctc_ioclr[c_max_ctcan_ser_array];

unsigned char ctcan_dev_list [c_max_ctcan_dev];	// This is the list of can devices used 
int16 nmb_ctcan_dev;
int16 ram_seq = 0;
int16 ctcan_init_count = 0;
int16 init_mode_cnt = 0;
bool in_chip_reset = false;
int32 ctcan_status = 0;
int16 ctcan_mode = 0;
int16 ctcan_int_status = 0;
int16 ctcan_error = 0;
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
int16 ctcan_int_count = 0;
int16 re_init_ctcan = 0;
int16 ctcan_arb_lost_cap=0;
int16 ctcan_err_code_cap=0;
int16 ctcan_tx_err_cnt=0;
int16 ctcan_rx_err_cnt = 0;
int16 rcv_fifo_cnt = 0;
int16 rcv_int_fail_cnt = 0;
int16 WCI_cnt=0;	// Wrong can interrupt
int16 BOI_cnt=0;	// Bus Off and tx/rx warning interrupt count
int16 ERI_cnt=0;	// Error Interrupt count
int16 RI_cnt=0;		// Receive fifo interrupt count
int16 FWI_cnt=0;	// Fifo warning interrupt count
int16 FOI_cnt=0;	// Fifo Overflow interrupt count
int16 TI8_cnt=0;	// Transmit buffer 8 interrupt count
int16 TI9_cnt=0;	// Transmit buffer 9 interrupt count
int16 TI10_cnt=0;	// Transmit buffer 10 interrupt count
int16 TI11_cnt=0;	// Transmit buffer 11 interrupt count
int16 ack_error_cnt = 0;
int16 bus_off_cnt = 0;
int16 bus_idle_cnt = 0;

int16 tx_B08_fail_cnt = 0;
int16 tx_B09_fail_cnt = 0;
int16 tx_B10_fail_cnt = 0;
int16 tx_B11_fail_cnt = 0;

int32 ctcan_debug_1=0;
int32 ctcan_debug_2=0;
int32 ctcan_debug_3=0;
int32 ctcan_debug_4=0;
int32 ctcan_debug_5=0;
int32 ctcan_debug_6=0;

int16 data_req = 0;				// Flag to signal that data has been requested by a device
int16 data_req_cnt = 0;

#define c_tx_buf_siz 30

struct tag_tx {
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
		} ctcan_tx[c_tx_buf_siz+1];

int16 ctc_tpix = 0;					// can transmit buffer put index
int16 ctc_tgix = 0;					// can transmit buffer get index
int16 availbuf = 0;				// available can transmit buffer
bool ctcan_tx_complete[16] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int16 CtcanTxBufIx = 8;
		
#define c_rx_buf_siz 30

struct tag_rx {
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
		} ctcan_rx[c_rx_buf_siz + 1];
		
int16 ctc_rpix = 0;					// can transmit buffer put index
int16 ctc_rgix = 0;					// can transmit buffer get index



// Declarations for Door Operator Boards

#define b_door_open 0x01
#define b_door_close 0x02
#define b_door_nudge 0x04
#define b_door_zone 0x08
#define b_door_heavy 0x20
#define b_door_narrow 0x40
#define b_door_par_wr 0x01
#define b_door_dol 0x01
#define b_door_dcl 0x02
#define b_door_dpm 0x04
#define b_door_opening 0x10
#define b_door_closing 0x20
#define b_door_reopening 0x40
#define b_door_det_edge 0x80
#define b_door_fault 0x01

// Front Door Operator Data

struct tag_DOOR {
		// Data Transmitted
		unsigned char command;
									// Bit 0: Open command   1 = Open
								 	// Bit 1: Close Command 1 = Close
								 	// Bit 2: Nudge Close	      1 = Close at nudging speed
								 	// Bit 3: Door Zone Status   1 = on dz
								 	// Bit 4: Unused
								 	// Bit 5: Heavy Door 1=use heavy door parameters
								 	// Bit 6: Narrow Door 1=use narrow door limit
								 	// Bit 7: Unused
		unsigned char prev_command;
		unsigned char status_ret;	/*	Status return bits
											Bit 0: Door Open Limit
											Bit 1: Door Close Limit
											Bit 2: DPM Limit
											Bit 3: Unused 
											Bit 4: Door reopen overspeed
											Bit 5: Door reopen over torque
											Bit 6: Door reopen ee/de
											Bit 7: Unused
									*/
		unsigned char reflect_data;	// Set true to send back status return data
		unsigned char par_cmd;		// Parameter Command
									/*  Bit 0: Read/Write Command 1 = write
										Bits 1-7 unused
									*/
		unsigned char prev_par_cmd;	// previous parameter command
		uint16 par_add_req;			// Parameter Address request 
		uint16 par_dat_wrt;			// Parameter Data wrt
		unsigned char dpy_req;		/* Display Request
										0 = Fault Code									
										1 = Output Voltage
										2 = Output Current
										3 = Output Frequency
										4-15 not defined at this time
									*/
		// Data Received
		unsigned char status_1;		/*	Status 1 bits
											Bit 0: Door Open Limit
											Bit 1: Door Close Limit
											Bit 2: DPM Limit
											Bit 3: Unused 
											Bit 4: Door reopen overspeed
											Bit 5: Door reopen over torque
											Bit 6: Door reopen ee/de
											Bit 7: Unused
									*/
		unsigned char status_2;			/*	Status 2 bits
											Bit 0: Fault
											Bit 1: Door Opening
											Bit 2: Door Closing
											Bit 3: Unused
											Bit 7,6,5,4: Monitor Value Returned  0-15 
											0 = Fault Code
											1 = Output Voltage
											2 = Output Current
											3 = Output Frequency
											4-15 not defined at this time
									*/
		unsigned char command_ret;		// Command byte return value (see command bits)
		unsigned char par_err;			// Parameter error
										/* 	0 = Okay
											1 = Illegal command code
											2 = Illegal data address
											3 = Illegal data value
										*/
		uint16 dpy_ret;				// Monitor value display return 
		uint16 fault;				// door fault code
		uint16 voltage;				// door output voltage
		uint16 current;				// door output current
		uint16 frequency;			// door output frequency
		uint16 par_add_ret;			// Parameter address return
		uint16 par_dat_rd;			// Parameter data read
		} DOOR_can = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

// Rear Door Operator Data

struct tag_RDOOR {
		// Data Transmitted
		unsigned char command;
									// Bit 0: Open command   1 = Open
								 	// Bit 1: Close Command 1 = Close
								 	// Bit 2: Nudge Close	      1 = Close at nudging speed
								 	// Bit 3: Door Zone Status   1 = on dz
								 	// Bit 4: Unused
								 	// Bit 5: Heavy Door 1=use heavy door parameters
								 	// Bit 6: Narrow Door 1=use narrow door limit
								 	// Bit 7: Unused
		unsigned char prev_command;
		unsigned char status_ret;	/*	Status return bits
											Bit 0: Door Open Limit
											Bit 1: Door Close Limit
											Bit 2: DPM Limit
											Bit 3: Unused 
											Bit 4: Door reopen overspeed
											Bit 5: Door reopen over torque
											Bit 6: Door reopen ee/de
											Bit 7: Unused
									*/
		unsigned char reflect_data;	// Set true to send back status return data
		unsigned char par_cmd;		// Parameter Command
									/*  Bit 0: Read/Write Command 1 = write
										Bits 1-7 unused
									*/
		unsigned char prev_par_cmd;	// previous parameter command
		uint16 par_add_req;			// Parameter Address request 
		uint16 par_dat_wrt;			// Parameter Data wrt
		unsigned char dpy_req;		/* Display Request
										0 = Fault Code									
										1 = Output Voltage
										2 = Output Current
										3 = Output Frequency
										4-15 not defined at this time
									*/
		// Data Received
		unsigned char status_1;		/*	Status 1 bits
											Bit 0: Door Open Limit
											Bit 1: Door Close Limit
											Bit 2: DPM Limit
											Bit 3: Unused 
											Bit 4: Door reopen overspeed
											Bit 5: Door reopen over torque
											Bit 6: Door reopen ee/de
											Bit 7: Unused
									*/
		unsigned char status_2;			/*	Status 2 bits
											Bit 0: Fault
											Bit 1: Door Opening
											Bit 2: Door Closing
											Bit 3: Unused
											Bit 7,6,5,4: Monitor Value Returned  0-15 
											0 = Fault Code
											1 = Output Voltage
											2 = Output Current
											3 = Output Frequency
											4-15 not defined at this time
									*/
		unsigned char command_ret;		// Command byte return value (see command bits)
		unsigned char par_err;			// Parameter error
										/* 	0 = Okay
											1 = Illegal command code
											2 = Illegal data address
											3 = Illegal data value
										*/
		uint16 dpy_ret;				// Monitor value display return 
		uint16 fault;				// door fault code
		uint16 voltage;				// door output voltage
		uint16 current;				// door output current
		uint16 frequency;			// door output frequency
		uint16 par_add_ret;			// Parameter address return
		uint16 par_dat_rd;			// Parameter data read
		} RDOOR_can = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

// Declarations for Selector Board
struct tag_SEL {
			unsigned char command;			// Selector Command
											//  Bit0: Preset Position count
											//  Bit1: Learn Floor Position
											//  Bit2: Reset Fault
											//  Bit3: Clear Fault table
			int32 poscnt;					// Car Position Count
			unsigned char position;			// Floor position	
			unsigned char doorzone;			// door zone inputs 
											//  Bit0: UL
											//	Bit1: DZ
											//  Bit2: DZA
											//  Bit3: DL
			unsigned char up_normal_limits;	// Up Slowdown Inputs
											//  Bit0: UN
											//  Bit1: UT
											//  Bit2: UT1
											//  Bit3: UT2
											//  Bit4: UT3
											//  Bit5: UT4
											//  Bit6: UT5
											//  Bit7: UT6
			unsigned char dn_normal_limits;	// Down Slowdown Inputs
											//  Bit0: DN
											//  Bit1: DT
											//  Bit2: DT1
											//  Bit3: DT2
											//  Bit4: DT3
											//  Bit5: DT4
											//  Bit6: DT5
											//  Bit7: DT6
			unsigned char em_terminal_limits; // Emergency Slowdown Inputs
											//  Bit0: DTS
											//  Bit1: UTS
			unsigned char status;			// Selector Status
											//  Bit0: Hoistway Learned
											//  Bit1: In Hw Learn
											//  Bit2: Fault
			unsigned char reflect_data;		// Return data verification
			unsigned char fault[10];		// Selector Faults
											//  0 = no fault
											//	1 = Sensor fault
		} SEL_can;


// Faults for RGB Car Call board
int16 ccb_flt_tbl[20] = {0, f_ccb_led_short_red,		// 1
							f_ccb_led_short_green,	   	// 2
							f_ccb_led_short_blue,	   	// 3
							f_ccb_led_open_red,		   	// 4
							f_ccb_led_open_green,	   	// 5
							f_ccb_led_open_blue,	   	// 6
							f_ccb_fet_short_red,	 	// 7
							f_ccb_fet_short_green, 		// 8
							f_ccb_fet_short_blue,		// 9
							f_ccb_fet_open_red,			// 10
							f_ccb_fet_open_green, 		// 11
							f_ccb_fet_open_blue,	 	// 12
							f_ccb_led_board,			// 13
							f_ccb_stuck_button,			// 14
							f_inv_fault,				// 15
							f_inv_fault,				// 16
							f_inv_fault,				// 17
							f_inv_fault,				// 18
							f_inv_fault,				// 19
							};  				


// Declarations for can bus pi and lantern devices

int16 alrm_pi_stat = 0;
unsigned char pi_pos;
unsigned char lant_stat;
unsigned char motion_stat;
unsigned char fire_stat;
unsigned char door_stat;
unsigned char misc_stat;
unsigned char pi_lant_stat;
unsigned char pi_motion_stat;
unsigned char pi_fire_stat;
unsigned char pi_door_stat;
unsigned char pi_misc_stat;
unsigned char update_pi_lant;

uint32 pi_cc[2] = {0,0};
uint32 pi_rcc[2] = {0,0};
uint32 ct_pi_cc[2] = {0,0};
uint32 ct_pi_rcc[2] = {0,0};

int32 c_cc_dpy[2] = {0,0};
int32 c_rcc_dpy[2] = {0,0};


uint8 pi_cc_command = 0;
uint8 pi_cc_floor = 0;
uint8 pi_location = 0;
uint8 pi_cc_ctcan_cnt = 0;
uint8 pi_cc_update = 0;



// Declarations for attendant hall call annuciator
int32 prev_up_hc[2] = {0,0};
int32 prev_dn_hc[2] = {0,0};
int32 prev_upr_hc[2] = {0,0};
int32 prev_dnr_hc[2] = {0,0};
int32 prev_irup_hc[2] = {0,0};
int32 prev_irdn_hc[2] = {0,0};
int32 prev_rirup_hc[2] = {0,0};
int32 prev_rirdn_hc[2] = {0,0};
int32 return_att_uhc[2] = {0,0};
int32 return_att_dhc[2] = {0,0};
int32 return_att_ruhc[2] = {0,0};
int32 return_att_rdhc[2] = {0,0};
int16 att_toggle = 0;
int16 update_att_fhc = 0;
int16 update_att_rhc = 0;
int16 update_att_fhc_hr = 0;
int16 update_att_rhc_hr = 0;
int16 update_att_fcc_18 = 0;
int16 update_att_fcc_5 = 0;
int16 update_att_fcc_11 = 0;
int16 update_att_rcc_6 = 0;
int16 update_att_rcc_7 = 0;
int16 update_att_rcc_12 = 0;

int16 cc_sec_flag_18 = 0;
int16 cc_sec_flag_5 = 0;
int16 cc_sec_flag_11 = 0;
int16 rcc_sec_flag_6 = 0;
int16 rcc_sec_flag_7 = 0;
int16 rcc_sec_flag_12 = 0;
int32 return_master_cc_sec[2] = {0,0};		// Car call security mask
int32 return_master_rcc_sec[2] = {0,0};   	// Rear Car call security mask

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu_2;					// unsigned integer union
	
static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		
	
// CAN_A Int Base = 127
// CAN_B Int Base = 157 
// CAN_C Int Base = 178
// CAN_D Int Base = 199

#define CT_CAN CAN_A				// Use CAN A
#define CT_CAN_INT_BASE 127
//#define CT_CAN CAN_B				// Use CAN B
//#define CT_CAN_INT_BASE 157


// Simple que with length of 3 for each device
//--------------------------------------
// Put device message request on que
//--------------------------------------
int16 ctcan_put_dev_msg_req (int16 dev, int8 msg)
{
	int16 ix = 0;

	while(ix < ctcan_que_len)
	{      // look for and open spot on the que
		if (ctcan_dev[dev].msg_req[ix] == 0)
		{         // found an open spot so place Packet on que
			ctcan_dev[dev].msg_req[ix] = msg;
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
int8 ctcan_get_dev_msg_req (int16 dev)
{
	int16 ix = 0;
	int8 msg = 0;			// return 0 if no Packet on queue

	while (ix < ctcan_que_len)
	{
		if (ctcan_dev[dev].msg_req[ix] != 0) 
		{      	// Packet for this device found
			msg = ctcan_dev[dev].msg_req[ix];
			ctcan_dev[dev].msg_req[ix] = 0;
			break;
		}
		else
			ix++;
	}

	// shift que data to the top (start where ix left off)

	while (ix < (ctcan_que_len-1))
	{
		if (ctcan_dev[dev].msg_req[ix+1] != 0)
		{   // Packet was taken from the middle of the que so shift data up
			ctcan_dev[dev].msg_req[ix] = ctcan_dev[dev].msg_req[ix+1];
			ctcan_dev[dev].msg_req[ix+1] = 0;
			ix++;
		}
		else
			break;
	}
	return (msg);
}


// clear comm loss error latch for RGB Car call driver board local devices
void clr_ctcan_dev_comm_loss (void)
{
	int16 i;
	for (i = 0;i<c_max_ctcan_dev; i++)
	{
		ctcan_dev[i].local_dev_comm_loss = 0;
	}
}

//*******************************************************************
// Sends the freeze bit to the chip
// Value of 1 puts the chip in freeze mode
// Value of 0 takes is out of freeze mode
// Have to see the freeze mode acknowledge bit gets set
//*******************************************************************
int16 ChipFreezeMode(unsigned char set)
{
	
	if (in_chip_reset == true)
		return 0;

	in_chip_reset = true;

	init_mode_cnt = 0;

	CT_CAN.MCR.B.MDIS = 0;			// Enable module after reset
	CT_CAN.MCR.B.FRZ = 1;			// Set or clear the freeze bit
	CT_CAN.MCR.B.HALT = set;			// Clear halt mode

	
	while (init_mode_cnt < 200)
	{
		if (CT_CAN.MCR.B.FRZACK == set)
		{
			in_chip_reset = false;
			return 1;
		}
		init_mode_cnt++;
	}
	in_chip_reset = false;
	return 0;		// Failed to initialize
}




// **************************************************
// ****** Calculate ioname can device checksum ******
// **************************************************

uint16 calc_ioname_can_cksum (unsigned char devnmb)
{
	uint16 chksum = 0;
	int16 h,i,j;
	int16 array_start, io_name_start;
	int16 iodev_ix = 0;
	int16 nmb_io_labels = 0;

	iodev_ix = (int16)(devnmb-c_ctcan_SEB_1);

	array_start = (int16)((IODATA_CTCAN_START + iodev_ix) * 3); 		// input array

	for (i=0;i<=2;i++)
	{
		for(j=0;j<=7;j++)
		{
			if ((io_send_name[array_start+i] & bittbl[j & 0x07]) != 0)
			{
				io_name_start = (array_start + i) * 8;
				for (h=0;h<=4;h++)
					chksum += ioname[io_name_start + j][h];
				nmb_io_labels++;
			}
		}
	}
	for(i=nmb_io_labels;i<12;i++)
		chksum += 0x00A0;			// all spaces (0x20) times 5
	return(chksum);
}
// **********************************************************
// ****** Calculate io color index can device checksum ******
// **********************************************************

unsigned char calc_io_color_ix_cksum (unsigned char devnmb)
{
	unsigned char chksum = 0;
	int16 i,j;
	unsigned char color_off_ix = 0;
	unsigned char color_on_ix = 0;
	int16 array_start, io_start;
	int16 iodev_ix = 0;

	iodev_ix = (int16)(devnmb-c_ctcan_SEB_1);

	array_start = (int16)((IODATA_CTCAN_START + iodev_ix) * 3); 		// input array

	for (i=0;i<=2;i++)
	{
		for(j=0;j<=7;j++)
		{
			if ((io_send_name[array_start+i] & bittbl[j & 0x07]) != 0)
			{
				io_start = (array_start + i) * 8;

				color_on_ix = io_on_color_ix[io_start + j];

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

				chksum += color_on_ix;
				chksum += color_off_ix;
			}
		}
	}
	return(chksum);
}


//*****************************************************************
// ***  Search device list for boards that need checksum updated
//*****************************************************************

void calc_can_devices_color_ix_chksum(void)
{
	int16 i;
	unsigned char chksum=0;
	uint16 iodev_ix = 0;
	uint16 board = 0;

	if (cons[RGB_CC] == 1) 
	{
		for (i=0;i<=nmb_ctcan_dev;i++)
		{
			iodev_ix = (int16)(ctcan_dev_list[i]-c_ctcan_SEB_1);
			board = (uint16)(IODATA_CTCAN_START + iodev_ix); 		// input board

			if ((IO_board[cons[carnmb]][board] & 0x02) != 0)
			{
				chksum = calc_io_color_ix_cksum(ctcan_dev_list[i]);
				if (ctcan_dev[ctcan_dev_list[i]].io_color_ix_chksum != chksum)
				{
					if (ctcan_put_dev_msg_req (ctcan_dev_list[i],58) == 1)
						ctcan_dev[ctcan_dev_list[i] ].color_ix_req = 1;
				}
			}
			else
				chksum = 0;
			ctcan_dev[ctcan_dev_list[i]].io_color_ix_chksum = chksum;	
		}
	}
}


// **************************************************
// ****  Initialize the CAN interface
// **************************************************


void init_ctcan_bus (void)
{	 
	int16 i,j;
	int16 reset_count;

	nmb_ctcan_dev = 0;
	ctc_tpix = 0;
	ctc_tgix = 0;
	update_pi_lant = 0;

#if ((Traction == 1) || (Tract_OL == 1))

	if (cons[sel_type] == 2)
	{
		ctcan_dev_list[nmb_ctcan_dev] = c_ctcan_SEL;		
		nmb_ctcan_dev++;
	}
	
#endif

 	if ((cons[sercom] & 0x10) != 0)
	{
		ctcan_dev_list[nmb_ctcan_dev] = c_ctcan_DOOR;		
		nmb_ctcan_dev++;
		if (cons[rear] != 0)
		{
			ctcan_dev_list[nmb_ctcan_dev] = c_ctcan_RDOOR;		
			nmb_ctcan_dev++;
		}
		DOOR_can.command = 0;
		RDOOR_can.command = 0;
	}

#if ((Traction == 1) || (Tract_OL == 1))

	if (cons[loadw] == 2)
	{		
		ctcan_dev_list[nmb_ctcan_dev] = c_ctcan_LW;		
		nmb_ctcan_dev++;
	}

	if (cons[Voltage_Sensor] == 1)
	{
		ctcan_dev_list[nmb_ctcan_dev] = c_ctcan_VS;		
		nmb_ctcan_dev++;
	}

#endif

	j = 0;
	for (i=46;i<86;i++)			
	{
		if ((IO_board[cons[carnmb]][i] != 0) && (((cons[sercom] & 0x10) == 0) || ((i != 49) && (i != 50))))
		{		 // Set device in device list                     ^^^^ no io board for the door if movfr door operator
			ctcan_dev_list[nmb_ctcan_dev] = (uint8)(c_ctcan_SEB_1 + j);
			ctcan_dev[(c_ctcan_SEB_1 + j)].ioname_chksum = calc_ioname_can_cksum((uint8)(c_ctcan_SEB_1 + j));
			if ((cons[RGB_CC] == 1) && ((IO_board[cons[carnmb]][i] & 0x02) != 0))
				ctcan_dev[(c_ctcan_SEB_1 + j)].io_color_ix_chksum = calc_io_color_ix_cksum((uint8)(c_ctcan_SEB_1 + j));
			else
				ctcan_dev[(c_ctcan_SEB_1 + j)].io_color_ix_chksum = 0;
			nmb_ctcan_dev ++;
			serial_ctc.brd_io[j] = IO_In_Out[j+138];
		}
		j++;
	}
/*
	for (i=0;i<c_max_ctcan_io_brd;i++)
	{
		serial_ctc.dev[0].iob[0] = 0;
		serial_ctc.dev[0].iob[1] = 0xf0;
		serial_ctc.dev[0].iob[2] = 0xff;
		j++;
	}
*/		
	for (i=0;i<c_max_ctcan_dev;i++)
	{
		ctcan_dev[i].version = 0;
		ctcan_dev[i].revision = 0;
		ctcan_dev[i].online = 0;
	}
	// See if the board is there
	ctcan_init_count++;

 	ctcan_tx_complete[8] = 1;
	ctcan_tx_complete[9] = 1;
	ctcan_tx_complete[10] = 1;
	ctcan_tx_complete[11] = 1;

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
	INTC_InstallINTCInterruptHandler(ctcan_bus_off_and_warning_int, CT_CAN_INT_BASE, 1);  
	INTC_InstallINTCInterruptHandler(ctcan_error_int, CT_CAN_INT_BASE+1, 1);  
	INTC_InstallINTCInterruptHandler(wrong_ctcan_int, CT_CAN_INT_BASE+3, 1);  
	INTC_InstallINTCInterruptHandler(wrong_ctcan_int, CT_CAN_INT_BASE+4, 1);  
	INTC_InstallINTCInterruptHandler(wrong_ctcan_int, CT_CAN_INT_BASE+5, 1);  
	INTC_InstallINTCInterruptHandler(wrong_ctcan_int, CT_CAN_INT_BASE+6, 1);  
	INTC_InstallINTCInterruptHandler(wrong_ctcan_int, CT_CAN_INT_BASE+7, 1);  
	INTC_InstallINTCInterruptHandler(ctcan_rcv_int, CT_CAN_INT_BASE+8, 1);  
	INTC_InstallINTCInterruptHandler(ctcan_rcv_fifo_warning_int, CT_CAN_INT_BASE+9, 1);  
	INTC_InstallINTCInterruptHandler(ctcan_rcv_fifo_overflow_int, CT_CAN_INT_BASE+10, 1);  
	INTC_InstallINTCInterruptHandler(ctcan_trm_int_b8, CT_CAN_INT_BASE+11, 1);  
	INTC_InstallINTCInterruptHandler(ctcan_trm_int_b9, CT_CAN_INT_BASE+12, 1);  
	INTC_InstallINTCInterruptHandler(ctcan_trm_int_b10, CT_CAN_INT_BASE+13, 1);  
	INTC_InstallINTCInterruptHandler(ctcan_trm_int_b11, CT_CAN_INT_BASE+14, 1);  


	
	// Send init to can board
	if (ChipFreezeMode(c_Freeze_Start) == 1)
	{
		ctcan_init_count++;
		CT_CAN.MCR.B.SOFTRST = 1;		// Soft Reset
		reset_count = 0;
		while ((CT_CAN.MCR.B.SOFTRST == 1) && (reset_count < 1000))
			reset_count++;
			
		CT_CAN.MCR.B.MDIS = 0;			// Enable module after reset

	    // Clear of all interrupts
	    CT_CAN.IFLAG1.R = 0xFFFFFFFF;
	    CT_CAN.IFLAG2.R = 0xFFFFFFFF;
	    
	    // Disable of Interrupts
		CT_CAN.IMASK1.R = 0;
		CT_CAN.IMASK2.R = 0;
		
		CT_CAN.MCR.B.BCC = 1;			// Allow backward compatibility since all mask will be set to 0xffff
		CT_CAN.MCR.B.MAXMB = 0x0B;		// Set to 12 (0xC - 1)
		CT_CAN.MCR.B.WRNEN = 0;			// Warning interrupts
		CT_CAN.MCR.B.SRXDIS = 1;			// Self Reception disabled
		CT_CAN.MCR.B.FEN = 1;			// Enable RX fifo
		CT_CAN.MCR.B.ABTEN = 1;			// Abort enable
		CT_CAN.MCR.B.LPRIOEN = 0;		// No local priority mode
		CT_CAN.MCR.B.IDAM = 0;			// ID Acceptance mode (one full id per filter element)
		
		
	    // Configure clock AND baud rate timer for 115200 baud
	    
	    /*	
	    	fTq = Bit Rate * Number of time Quanta = fCANCLK / Prescale Value
	    	Prescale Value = fCANCLK/(Bit Rate * Number of Time Quanta)
	    	Prescale Value = 108.75 Mhz / (115200 * 16) = 59.00065
	    */
	    
	    
	    CT_CAN.CTRL.B.CLKSRC = 1; 		// System Clock 108.75 Mhz
		if (fvars[fvcanbaud] == 0)	  // Can Baud Rate, 0=115.2K, 1=57.6K
		    CT_CAN.CTRL.B.PRESDIV = 58;		// Prescale Devision Factor + 1 = Prescale Value for 115200
		else
		    CT_CAN.CTRL.B.PRESDIV = 117;		// Prescale Devision Factor + 1 = Prescale Value for 57600
		
		CT_CAN.CTRL.B.RJW = 2;			// Resync Jump Width = RJW + 1 = 3
		// Total Tq = 16
		CT_CAN.CTRL.B.PSEG1 = 7;			// Phase Segment 1 = (PSEG1 + 1) x Tq = 8 (also sync seg = 1)
		CT_CAN.CTRL.B.PSEG2 = 5;			// Phase Segment 2 = (PSEG2 + 1) x Tq = 6
		CT_CAN.CTRL.B.PROPSEG = 0;		// Propagation Segment time = (PROPSEG + 1) x Tq = 1
		CT_CAN.CTRL.B.SMP = 1;			// Three samples
		
		CT_CAN.CTRL.B.BOFFREC = 0;		// Automamtic recovery
		CT_CAN.CTRL.B.TSYN = 0;			// Timer sync disabled
		CT_CAN.CTRL.B.LBUF = 1;			// Lowest buffer transmitted first
		CT_CAN.CTRL.B.LOM = 0;			// Listen only mode disabled
		
		CT_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
		CT_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
		CT_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
		CT_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled


		// Set Acceptance Mask
		
		CT_CAN.RXIMR[0].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[1].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[2].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[3].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[4].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[5].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[6].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[7].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[8].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[9].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[10].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[11].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[12].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[13].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[14].R = 0x00000000;		// Enable all packets
		CT_CAN.RXIMR[15].R = 0x00000000;		// Enable all packets

		// Set Receive ID
		CT_CAN.MB[6].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		CT_CAN.MB[6].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		CT_CAN.MB[6].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		CT_CAN.MB[6].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID
		CT_CAN.MB[7].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		CT_CAN.MB[7].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		CT_CAN.MB[7].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		CT_CAN.MB[7].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID

		CT_CAN.MB[8].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		CT_CAN.MB[9].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		CT_CAN.MB[10].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		CT_CAN.MB[11].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		
		if (ChipFreezeMode(c_Freeze_Stop) == 1)
		{
			ctcan_init_count++;
			if (nmb_ctcan_dev != 0)
			{	// initialize can board

			    // Enable interrupts 
				CT_CAN.IMASK1.R = 0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
				CT_CAN.IMASK2.R = 0x00;		// MB 32-63 NOT Enabled
				CT_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
				CT_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
				CT_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
				CT_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled

			}
		}
		else
		{
			// Set error code
			ctcan_error = 0x01;
		}		
	}
	else
	{
		// Set error code
		ctcan_error = 0x02;
	} 
	_enable();
}

// **************************************************
// ********  Can Board Interrupt Routines  **********
// **************************************************

void wrong_ctcan_int (void)
{
	CT_CAN.IFLAG1.B.BUF00I = 1;
	CT_CAN.IFLAG1.B.BUF01I = 1;
	CT_CAN.IFLAG1.B.BUF02I = 1;
	CT_CAN.IFLAG1.B.BUF03I = 1;
	CT_CAN.IFLAG1.B.BUF04I = 1;
	WCI_cnt++;
}
void ctcan_bus_off_and_warning_int (void)
{		// Bus off, Transmit count warning, Receive Count warning
	CT_CAN.ESR.B.BOFFINT = 1;
	CT_CAN.ESR.B.TWRNINT = 1;
	CT_CAN.ESR.B.RWRNINT = 1;
	BOI_cnt++;
}

void ctcan_error_int (void)
{		// Bit 0, Bit 1, Acknowledge, CRC, Form or Stuffing Errors
	CT_CAN.ESR.B.ERRINT = 1;
	ERI_cnt++;
}

void ctcan_rcv_int (void)
{	// Receive Interrupt (MB5)
	RI_cnt++;

	rcv_fifo_cnt = 0;
	while ((CT_CAN.IFLAG1.B.BUF05I == 1) && (rcv_fifo_cnt < 9))
	{			// Have a can message
		receive_ctcan_in (0);
		CT_CAN.IFLAG1.B.BUF05I = 1;				// Clears the bit
		rcv_fifo_cnt++;
	}
	if (rcv_fifo_cnt > 1)
		ctcan_debug_5 = rcv_fifo_cnt;
}

void ctcan_rcv_fifo_warning_int (void)
{	// Receive fifo warning (MB6)
	CT_CAN.IFLAG1.B.BUF06I = 1;
	FWI_cnt++;
}

void ctcan_rcv_fifo_overflow_int (void)
{	// Receive Fifo Overrun Interrupt (MB7)
	CT_CAN.IFLAG1.B.BUF07I = 1;
	FOI_cnt++;
}

void ctcan_trm_int_b8 (void)
{	// Transmit Interrupt
	TI8_cnt++;
	CT_CAN.IFLAG1.B.BUF08I = 1;
	transmit(8);
	ctcan_tx_complete[8] = 1;
}

void ctcan_trm_int_b9 (void)
{	// Transmit Interrupt
	TI9_cnt++;
	CT_CAN.IFLAG1.B.BUF09I = 1;
	transmit(9);
	ctcan_tx_complete[9] = 1;
}

void ctcan_trm_int_b10 (void)
{	// Transmit Interrupt
	TI10_cnt++;
	CT_CAN.IFLAG1.B.BUF10I = 1;
	transmit(10);
	ctcan_tx_complete[10] = 1;
}

void ctcan_trm_int_b11 (void)
{	// Transmit Interrupt
	TI11_cnt++;
	CT_CAN.IFLAG1.B.BUF11I = 1;
	transmit(11);
	ctcan_tx_complete[11] = 1;
}

//************************************************************
// Receive the can input
//************************************************************

void receive_ctcan_in (int16 BufIx)
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
	while (((CT_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) == 0x01) && (read_dly_cnt < 100))
		read_dly_cnt ++;
		
	if (read_dly_cnt > 1)
		ctcan_debug_6 = read_dly_cnt;

	if ((CT_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) != 0x01)
	{			// read of CS locks the receive buffer
		if (ctcan_rx[ctc_rpix].valid_msg == 0)
		{
			ctcan_rx[ctc_rpix].ex_id = (uint8)CT_CAN.MB[BufIx].MSG.MCS.B.IDE;
			ctcan_rx[ctc_rpix].len = (uint8)CT_CAN.MB[BufIx].MSG.MCS.B.LENGTH;
			ctcan_rx[ctc_rpix].rtr = (uint8)CT_CAN.MB[BufIx].MSG.MCS.B.RTR;

			if (ctcan_rx[ctc_rpix].len > 8)
				ctcan_rx[ctc_rpix].len = 8;
			if (ctcan_rx[ctc_rpix].ex_id == 1)		// Extended frame
			{
				uiu.dw = (CT_CAN.MB[BufIx].MSG.MID.R << 3);
				ctcan_rx[ctc_rpix].msg = uiu.b[hb];	// Message
				ctcan_rx[ctc_rpix].addr = uiu.b[mhb];	// Address
				ctcan_rx[ctc_rpix].id2 = uiu.b[mlb];
				ctcan_rx[ctc_rpix].id3 = uiu.b[lb];
			}
			else
			{
				uiu.dw = ((CT_CAN.MB[BufIx].MSG.MID.R & 0x1FFC0000) << 1) ;
				uiu_2.dw = ((CT_CAN.MB[BufIx].MSG.MID.R & 0x1FFC0000) >> 2);
				ctcan_rx[ctc_rpix].msg = uiu.b[hb];	// Message
				ctcan_rx[ctc_rpix].addr = uiu_2.b[mhb];    // Address
			}

			i = 0;
			while ((i < ctcan_rx[ctc_rpix].len) && (i < 8))
			{
				ctcan_rx[ctc_rpix].data[i] = CT_CAN.MB[BufIx].MSG.MDATA.B[i];
				i++;
			}
			ctcan_rx[ctc_rpix].valid_msg = 1;
		}
	
	}
	else
	{
		ctcan_debug_4 = CT_CAN.MB[BufIx].MSG.MCS.B.CODE;
		rcv_int_fail_cnt++;	
	}
		
	ctcan_rx[ctc_rpix].time = (uint16)CT_CAN.MB[BufIx].MSG.MCS.B.TIMESTAMP;		// Unlocks the buffer

	ctc_rpix++;
	if (ctc_rpix >= c_rx_buf_siz)
		ctc_rpix = 0;
			
}


//************************************************************
// Unpack the can receive buffer
//************************************************************

void unpack_ctcan_in (void)
{
	int16 i;
	int16 devnmb = 0;
	int16 devix = 0;
	int16 loop_cnt = 0;
	int16 iodev_ix = 0;
	uint16 iodataix;

  //  SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25 
	while ((loop_cnt <= 4)  && (ctc_rgix != ctc_rpix))
	{
		if (ctcan_rx[ctc_rgix].valid_msg == 1)
		{		
			devnmb = findmatch(ctcan_rx[ctc_rgix].addr, ctcan_addr, sizeof(ctcan_addr));

			if ((ctcan_rx[ctc_rgix].rtr == 0) && (devnmb > 0))
			{		// OK to unpack the data 
			 //////////////// wbg ///////////////////////////   
			    ctcan_dev[devnmb].online = 1;
			    sprintf(src,"unpk ct %d; %d: %d", 
			        ctcan_rx[ctc_rgix].msg, devnmb, 
			            ctcan_dev[devnmb].online);
//-               Add_msg_to_array_B(src, devnmb);
               // SIU.GPDO[Z6_LED].R = 0;
              ////////////////////////////////////////			
			
			
	 			switch (ctcan_rx[ctc_rgix].msg)
				{
					case 10:	  // Device Can Error Message
						if ((devnmb >= c_ctcan_START) && (devnmb <= c_ctcan_END))
						{	  // id1 is the address
							ctcan_dev[devnmb].can_error = ctcan_rx[ctc_rgix].data[0];
							ctcan_dev[devnmb].can_error_data = ctcan_rx[ctc_rgix].data[1];
						}
						break;

					case 11:	// Invalid request for initialization
						if ((devnmb >= c_ctcan_START) && (devnmb <= c_ctcan_END))
						{	  // id1 is the address
							ctcan_put_dev_msg_req (devnmb,10);
						}
						break;

					case 12:	// message 12: device version and revision
						if ((devnmb >= c_ctcan_START) && (devnmb <= c_ctcan_VS))
						{	  // id1 is the address
							if (ctcan_dev[devnmb].version != 0) 
							{
								fault_prog_flag1 = 0;		// must set to zero on this fault for galcom
								fault_device = devnmb;
								record_fault(f_CTCAN_device_reset);
							}
							ctcan_dev[devnmb].version = ctcan_rx[ctc_rgix].data[0];
							ctcan_dev[devnmb].revision = ctcan_rx[ctc_rgix].data[1];
							ctcan_dev[devnmb].online_cnt = 5;
							ctcan_dev[devnmb].rx_cnt++;
						}
						else if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_END))
						{	  // id1 is the address
							if (ctcan_dev[devnmb].version != 0) 
							{
								fault_device = devnmb;
								record_fault(f_CTCAN_device_reset);
							}
							ctcan_dev[devnmb].version = ctcan_rx[ctc_rgix].data[0];
							ctcan_dev[devnmb].revision = ctcan_rx[ctc_rgix].data[1];
							timers[tcanrx + (devnmb-c_ctcan_SEB_1)] = 0;
							ctcan_dev[devnmb].online_cnt = 5;
							ctcan_dev[devnmb].rx_cnt++;
				
							if (devnmb == c_ctcan_SEB_2)
								ctcan_put_dev_msg_req (devnmb,59);

						}
						break;
					case 14:	// message 14: device inputs
						if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
						{	  // id1 is the address
//						  SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25 
							iodev_ix = (int16)(devnmb-c_ctcan_SEB_1);
							iodataix = (int16)((IODATA_CTCAN_START + iodev_ix) * 3); 		// input array
							serial_ctc.dev[iodev_ix].iob[0] = (uint8)((serial_ctc.dev[iodev_ix].iob[0] & IO_In_Out[iodataix]) | (ctcan_rx[ctc_rgix].data[0] & ~IO_In_Out[iodataix]));
							serial_ctc.dev[iodev_ix].iob[1] = (uint8)((serial_ctc.dev[iodev_ix].iob[1] & IO_In_Out[iodataix+1]) | (ctcan_rx[ctc_rgix].data[1] & ~IO_In_Out[iodataix+1]));
							serial_ctc.dev[iodev_ix].iob[2] = (uint8)((serial_ctc.dev[iodev_ix].iob[2] & IO_In_Out[iodataix+2]) | (ctcan_rx[ctc_rgix].data[2] & ~IO_In_Out[iodataix+2]));
/*							if ((cons[RGB_CC] == 1) && (ctcan_rx[ctc_rgix].len > 3))
							{

								if (ctcan_dev[devnmb].local_dev_comm_loss != (ctcan_rx[ctc_rgix].data[3] & 0x0F))
								{
									if (((ctcan_rx[ctc_rgix].data[3] & 0x01) != 0) && ((ctcan_dev[devnmb].local_dev_comm_loss & 0x01) == 0))
									{
										fault_device = ctcan_addr[devnmb];
										record_fault(f_ccb_comm_bd1);
									}
									i = valid_second_board(devnmb);
									if (i == 1)
									{
										if (((ctcan_rx[ctc_rgix].data[3] & 0x02) != 0) && ((ctcan_dev[devnmb].local_dev_comm_loss & 0x02) == 0))
										{
											fault_device = ctcan_addr[devnmb];
											record_fault(f_ccb_comm_bd2);
										}
									}
									if ((cons[aux_COP] != 0) && (devnmb != c_ctcan_SEB_19) && (devnmb != c_ctcan_SEB_22))
									{
										if (((ctcan_rx[ctc_rgix].data[3] & 0x04) != 0) && ((ctcan_dev[devnmb].local_dev_comm_loss & 0x04) == 0))
										{
											fault_device = ctcan_addr[devnmb];
											record_fault(f_ccb_comm_xbd1);
										}
										if (i == 1)
										{
											if (((ctcan_rx[ctc_rgix].data[3] & 0x08) != 0) && ((ctcan_dev[devnmb].local_dev_comm_loss & 0x08) == 0))
											{
												fault_device = ctcan_addr[devnmb];
												record_fault(f_ccb_comm_xbd2);
											}
										}
									}
								}
								ctcan_dev[devnmb].local_dev_comm_loss = ctcan_rx[ctc_rgix].data[3] & 0x0F;
							}
*/
							sync_dev_ctc[iodev_ix].reflect_data = 1;
							ctcan_debug_2++;
							timers[tcanrx + (devnmb-c_ctcan_SEB_1)] = 0;
							ctcan_dev[devnmb].online_cnt = 5;
							ctcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 16:	// message 16: return of output data
						if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
						{	  // id1 is the address
							iodev_ix = (int16)(devnmb-c_ctcan_SEB_1);
							iodataix = (int16)((IODATA_CTCAN_START + iodev_ix) * 3); 		// input array
							sync_dev_ctc[iodev_ix].iob[0] = (uint8)((sync_dev_ctc[iodev_ix].iob[0] & ~IO_In_Out[iodataix]) | (~ctcan_rx[ctc_rgix].data[0] & IO_In_Out[iodataix]));
							sync_dev_ctc[iodev_ix].iob[1] = (uint8)((sync_dev_ctc[iodev_ix].iob[1] & ~IO_In_Out[iodataix+1]) | (~ctcan_rx[ctc_rgix].data[1] & IO_In_Out[iodataix+1]));
							sync_dev_ctc[iodev_ix].iob[2] = (uint8)((sync_dev_ctc[iodev_ix].iob[2] & ~IO_In_Out[iodataix+2]) | (~ctcan_rx[ctc_rgix].data[2] & IO_In_Out[iodataix+2]));
							ctcan_dev[devnmb].online_cnt = 5;
							ctcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 22: // message 22: load weigher data
						if (devnmb == c_ctcan_LW)
						{
#if ((Traction == 1) || (Tract_OL == 1))
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[0];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[1];
							lw_data[lw_ix++] = uwu.w;
							if (lw_ix > 9)
								lw_ix = 0;
							for (i=0;i<=9;i++)
								avg_load += lw_data[i];
							avg_load /= 10;
							ret_lw_command = ctcan_rx[ctc_rgix].data[2];	// not used
							lw_error = ctcan_rx[ctc_rgix].data[3];
							ctcan_dev[devnmb].online_cnt = 6;
							ctcan_dev[devnmb].rx_cnt++;
#endif
						}
						break;
					case 24:	// message 24: Selector data
						if (devnmb == c_ctcan_SEL)
						{	  // id1 is the address
#if ((Traction == 1) || (Tract_OL == 1))
							uiu.dw = 0;
							uiu.b[lb] = ctcan_rx[ctc_rgix].data[0];
							uiu.b[mlb] = ctcan_rx[ctc_rgix].data[1];
							uiu.b[mhb] = ctcan_rx[ctc_rgix].data[2];
							uiu.b[hb] = ctcan_rx[ctc_rgix].data[3];
							SEL_can.poscnt = uiu.dw;
							SEL_can.status = ctcan_rx[ctc_rgix].data[4];
							ctcan_dev[devnmb].online_cnt = 3;
							ctcan_dev[devnmb].rx_cnt++;
#endif
						}
						break;
					case 29:		// Door Statue Data
						if (devnmb == c_ctcan_DOOR)
						{
							DOOR_can.status_1 = ctcan_rx[ctc_rgix].data[0];
							if ((DOOR_can.status_1 & 0x01) == 0)
								setinp(i_DOL);
							else
								clrinp(i_DOL);

							if ((DOOR_can.status_1 & 0x02) == 0)
								setinp(i_DCL);
							else
								clrinp(i_DCL);

							if ((DOOR_can.status_1 & 0x04) == 0)
								setinp(i_DPM);
							else
								clrinp(i_DPM);

							if ((DOOR_can.status_1 & 0x70) != 0)	  // ee, re-open over torque, re-open over speed
								setinp(i_EE);
							else
								clrinp(i_EE);
							
							if (DOOR_can.status_ret != DOOR_can.status_1)
								DOOR_can.reflect_data = 1;

							DOOR_can.status_ret = DOOR_can.status_1;

							DOOR_can.status_2 = ctcan_rx[ctc_rgix].data[1];
							i = (int16)(DOOR_can.status_2 >> 4);
							if (i == 0)
							{
								uwu.b[lbw] = ctcan_rx[ctc_rgix].data[3];
								uwu.b[hbw] = ctcan_rx[ctc_rgix].data[2];
								DOOR_can.fault = uwu.w;
							}
							else if (i == 1)
							{
								uwu.b[lbw] = ctcan_rx[ctc_rgix].data[3];
								uwu.b[hbw] = ctcan_rx[ctc_rgix].data[2];
								DOOR_can.voltage = uwu.w;
							}
							else if (i == 2)
							{
								uwu.b[lbw] = ctcan_rx[ctc_rgix].data[3];
								uwu.b[hbw] = ctcan_rx[ctc_rgix].data[2];
								DOOR_can.current = uwu.w;
							}
							else if (i == 3)
							{
								uwu.b[lbw] = ctcan_rx[ctc_rgix].data[3];
								uwu.b[hbw] = ctcan_rx[ctc_rgix].data[2];
								DOOR_can.frequency = uwu.w;
							}
							DOOR_can.command_ret = ctcan_rx[ctc_rgix].data[4];
							ctcan_dev[devnmb].online_cnt = 6;
							ctcan_dev[devnmb].rx_cnt++;
						}
						if (devnmb == c_ctcan_RDOOR)
						{
							RDOOR_can.status_1 = ctcan_rx[ctc_rgix].data[0];
							if ((RDOOR_can.status_1 & 0x01) == 0)
								setinp(i_DOLR);
							else
								clrinp(i_DOLR);

							if ((RDOOR_can.status_1 & 0x02) == 0)
								setinp(i_DCLR);
							else
								clrinp(i_DCLR);

							if ((RDOOR_can.status_1 & 0x04) == 0)
								setinp(i_RPM);
							else
								clrinp(i_RPM);

							if ((RDOOR_can.status_1 & 0x70) != 0)			// ee, re-open over torque, re-open over speed
								setinp(i_EER);
							else
								clrinp(i_EER);

							if (RDOOR_can.status_ret != RDOOR_can.status_1)
								RDOOR_can.reflect_data = 1;

							RDOOR_can.status_ret = RDOOR_can.status_1;

							RDOOR_can.status_2 = ctcan_rx[ctc_rgix].data[1];
							i = (int16)(RDOOR_can.status_2 >> 4);
							if (i == 0)
							{
								uwu.b[lbw] = ctcan_rx[ctc_rgix].data[3];
								uwu.b[hbw] = ctcan_rx[ctc_rgix].data[2];
								RDOOR_can.fault = uwu.w;
							}
							else if (i == 1)
							{
								uwu.b[lbw] = ctcan_rx[ctc_rgix].data[3];
								uwu.b[hbw] = ctcan_rx[ctc_rgix].data[2];
								RDOOR_can.voltage = uwu.w;
							}
							else if (i == 2)
							{
								uwu.b[lbw] = ctcan_rx[ctc_rgix].data[3];
								uwu.b[hbw] = ctcan_rx[ctc_rgix].data[2];
								RDOOR_can.current = uwu.w;
							}
							else if (i == 3)
							{
								uwu.b[lbw] = ctcan_rx[ctc_rgix].data[3];
								uwu.b[hbw] = ctcan_rx[ctc_rgix].data[2];
								RDOOR_can.frequency = uwu.w;
							}
							RDOOR_can.command_ret = ctcan_rx[ctc_rgix].data[4];
							ctcan_dev[devnmb].online_cnt = 6;
							ctcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 31:		// Door Parameter Return Data
						if (devnmb == c_ctcan_DOOR)
						{
							DOOR_can.par_err = ctcan_rx[ctc_rgix].data[0];
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[2];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[1];
							DOOR_can.par_add_ret = uwu.w;
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[4];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[3];
							DOOR_can.par_dat_rd = uwu.w;
							if (DOOR_can.par_add_req == DOOR_can.par_add_ret)
							{
								DOOR_can.par_cmd &= ~b_door_par_wr;
								DOOR_can.par_add_req = 0;
								DOOR_can.par_dat_wrt = 0;
							}
							ctcan_dev[devnmb].rx_cnt++;
							ctcan_dev[devnmb].online_cnt = 6;
						}
						if (devnmb == c_ctcan_RDOOR)
						{
							RDOOR_can.par_err = ctcan_rx[ctc_rgix].data[0];
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[2];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[1];
							RDOOR_can.par_add_ret = uwu.w;
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[4];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[3];
							RDOOR_can.par_dat_rd = uwu.w;
							if (RDOOR_can.par_add_req == RDOOR_can.par_add_ret)
							{
								RDOOR_can.par_cmd &= ~b_door_par_wr;
								RDOOR_can.par_add_req = 0;
								RDOOR_can.par_dat_wrt = 0;
							}
							ctcan_dev[devnmb].rx_cnt++;
							ctcan_dev[devnmb].online_cnt = 6;
						}
						break;
					case 34:	// message 34: Selector data
						if (devnmb == c_ctcan_SEL)
						{	  // id1 is the address
							SEL_can.status = ctcan_rx[ctc_rgix].data[0];
							SEL_can.position = ctcan_rx[ctc_rgix].data[1];
							SEL_can.doorzone = ctcan_rx[ctc_rgix].data[2];
							SEL_can.up_normal_limits = ctcan_rx[ctc_rgix].data[3];
							SEL_can.dn_normal_limits = ctcan_rx[ctc_rgix].data[4];
							SEL_can.em_terminal_limits = ctcan_rx[ctc_rgix].data[5];
							SEL_can.reflect_data = 1;
							ctcan_dev[devnmb].online_cnt = 3;
							ctcan_dev[devnmb].rx_cnt++;
						}
						break;

					case 44:		// message 44: Voltage Sensor Input	 (value x 10) = xxx.x Vrms
						if (devnmb == c_ctcan_VS)
						{
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[0];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[1];
							L1_Voltage = uwu.w;
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[2];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[3];
							L2_Voltage = uwu.w;
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[4];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[5];
							L3_Voltage = uwu.w;
							uwu.b[lbw] = ctcan_rx[ctc_rgix].data[6];
							uwu.b[hbw] = ctcan_rx[ctc_rgix].data[7];
							Door_Voltage = uwu.w;
							ctcan_dev[devnmb].online_cnt = 3;
							ctcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 46:	// message 46: Request for i/o labels
						if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
						{	  
							timers[tcanrx + (devnmb-c_ctcan_SEB_1)] = 0;
							ctcan_dev[devnmb].label_req = ctcan_rx[ctc_rgix].data[0];
							ctcan_dev[devnmb].label_ix = 0;
							ctcan_dev[devnmb].online_cnt = 5;
							ctcan_dev[devnmb].rx_cnt++;
							ctcan_put_dev_msg_req (devnmb,47);
						}
						break;
					case 48:	  // Message 48: Device Error Message
						if ((devnmb >= c_ctcan_START) && (devnmb <= c_ctcan_END))
						{	  
							ctcan_dev[devnmb].dev_error = ctcan_rx[ctc_rgix].data[0];
							ctcan_dev[devnmb].dev_error_data[0] = ctcan_rx[ctc_rgix].data[1];
							ctcan_dev[devnmb].dev_error_data[1] = ctcan_rx[ctc_rgix].data[2];
							if ((cons[RGB_CC] == 1) && ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_22)))
							{
								if (ctcan_dev[devnmb].dev_error <  (unsigned char) sizeof(ccb_flt_tbl))
								{
									fault_device = ctcan_addr[devnmb];				// device address
									fault_prog_flag1 = ctcan_dev[devnmb].dev_error_data[0];	  // local address
									fault_prog_flag2 = ctcan_dev[devnmb].dev_error_data[1];	  // io number 0-11	
									record_fault(ccb_flt_tbl[ctcan_dev[devnmb].dev_error]);
								}
							}
							else
							{
								fault_device = devnmb;
								fault_prog_flag1 = ctcan_dev[devnmb].dev_error;				// fault code
								fault_prog_flag2 = ctcan_dev[devnmb].dev_error_data[0];		// fault count
								fault_device_2 = ctcan_dev[devnmb].dev_error_data[1];	// speial code
								record_fault(f_CTCAN_device_fault);
							}
						}
 						break;
					case 54:		// Return Attendant Up and Down data
						if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_END))
						{
							if (devnmb == c_ctcan_SEB_4)
							{
								return_att_uhc[0] &= 0xfffff000L;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[1] & 0x0f;
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_att_uhc[0] |= uiu.dw;

								return_att_dhc[0] &= 0xfffff000L;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[3];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[4] & 0x0f;
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_att_dhc[0] |= uiu.dw;
							}
							else if (devnmb == c_ctcan_SEB_5) 
							{
								return_att_uhc[0] &= 0x00000fffL;
								return_att_uhc[1] &= 0xfffffff0L;
								uiu.b[lb] = 0;
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[2];
								return_att_uhc[0] |= (uiu.dw << 4);
	#if (Tract_HR == 1)
								return_att_uhc[1] |= (uiu.b[hb] >> 4);
	#endif
								return_att_dhc[0] &= 0x00000fffL;
								return_att_dhc[1] &= 0xfffffff0L;
								uiu.b[lb] = 0;
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[3];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[4];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[5];
								return_att_dhc[0] |= (uiu.dw << 4);
	#if (Tract_HR == 1)
								return_att_uhc[1] |= (uiu.b[hb] >> 4);
	#endif
							}
							else if (devnmb == c_ctcan_SEB_6) 
							{
								return_att_ruhc[0] &= 0xfffe0000L;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[2] & 0x01;
								uiu.b[hb] = 0;
								return_att_ruhc[0] |= uiu.dw;

								return_att_rdhc[0] &= 0xfffe0000L;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[3];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[4];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[5] & 0x01;
								uiu.b[hb] = 0;
								return_att_rdhc[0] |= uiu.dw;
							}
							else if (devnmb == c_ctcan_SEB_7)
							{
								return_att_ruhc[0] &= 0x0001ffffL;
								return_att_ruhc[1] &= 0xfffffe00L;
								uiu.b[lb] = 0;
								uiu.b[mlb] = 0;
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[1];
								return_att_ruhc[0] |= (uiu.dw << 1);
	#if (Tract_HR == 1)
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[2];
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_att_ruhc[1] |= (uiu.b[hb] >> 7);
	#endif

								return_att_rdhc[0] &= 0x0001ffffL;
								return_att_rdhc[1] &= 0xfffffe00L;
								uiu.b[lb] = 0;
								uiu.b[mlb] = 0;
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[3];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[4];
								return_att_rdhc[0] |= (uiu.dw << 1);
	#if (Tract_HR == 1)
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[4];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[5];
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_att_rdhc[1] |= (uiu.b[hb] >> 7);
	#endif
							}
	#if (Tract_HR == 1)
							else if (devnmb == c_ctcan_SEB_11)
							{
								return_att_uhc[1] &= 0x0000000fL;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[2];
								uiu.b[hb] = 0;
								return_att_uhc[1] |= (uiu.dw << 4);

								return_att_dhc[1] &= 0x0000000fL;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[3];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[4];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[5];
								uiu.b[hb] = 0;
								return_att_dhc[1] |= (uiu.dw << 4);
							}
							else if (devnmb == c_ctcan_SEB_12)
							{
								return_att_ruhc[1] &= 0x000001ffL;
								uiu.b[lb] = 0;
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[2];
								return_att_ruhc[1] |= (uiu.dw << 1);

								return_att_rdhc[1] &= 0x000001ffL;
								uiu.b[lb] = 0;
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[3];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[4];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[5];
								return_att_rdhc[1] |= (uiu.dw << 1);
							}
							else if ((cons[RGB_CC] == 1) && (devnmb == c_ctcan_SEB_18))
							{
								return_att_uhc[0] &= 0xfffff000L;
								uiu.b[lb] = (ctcan_rx[ctc_rgix].data[0] & 0x3f) | (ctcan_rx[ctc_rgix].data[1] << 2);
								uiu.b[mlb] = (ctcan_rx[ctc_rgix].data[1] >> 6) | ((ctcan_rx[ctc_rgix].data[2] & 0x03) << 2);
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_att_uhc[0] |= uiu.dw;

								return_att_dhc[0] &= 0xfffff000L;
								uiu.b[lb] = (ctcan_rx[ctc_rgix].data[3] & 0x3f) | (ctcan_rx[ctc_rgix].data[4] << 2);
								uiu.b[mlb] = (ctcan_rx[ctc_rgix].data[4] >> 6) | ((ctcan_rx[ctc_rgix].data[5] & 0x03) << 2);
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_att_dhc[0] |= uiu.dw;
							}
	#endif
							ctcan_dev[devnmb].online_cnt = 5;
							ctcan_dev[devnmb].rx_cnt++;

						}
						break;
					case 56:		// Return Car Call Security data
						if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_END))
						{

							if (devnmb == c_ctcan_SEB_4)
							{
								return_master_cc_sec[0] &= 0xfffff000L;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[1] & 0x0f;
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_master_cc_sec[0] |= uiu.dw;
							}
							else if (devnmb == c_ctcan_SEB_5) 
							{
								return_master_cc_sec[0] &= 0x00000fffL;
								return_master_cc_sec[1] &= 0xfffffff0L;
								uiu.b[lb] = 0;
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[2];
								return_master_cc_sec[0] |= (uiu.dw << 4);
	#if (Tract_HR == 1)
								return_master_cc_sec[1] |= (uiu.b[hb] >> 4);
	#endif
							}
							else if (devnmb == c_ctcan_SEB_6) 
							{
								return_master_rcc_sec[0] &= 0xfffe0000L;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[2] & 0x01;
								uiu.b[hb] = 0;

								return_master_cc_sec[0] |= uiu.dw;
							}
							else if (devnmb == c_ctcan_SEB_7)
							{
								return_master_rcc_sec[0] &= 0x0001ffffL;
								return_master_rcc_sec[1] &= 0xfffffe00L;
								uiu.b[lb] = 0;
								uiu.b[mlb] = 0;
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[1];
								return_master_rcc_sec[0] |= (uiu.dw << 1);
	#if (Tract_HR == 1)
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[2];
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_master_rcc_sec[1] |= (uiu.b[hb] >> 7);
	#endif
							}
	#if (Tract_HR == 1)
							else if (devnmb == c_ctcan_SEB_11)
							{
								return_master_cc_sec[1] &= 0x0000000fL;
								uiu.b[lb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[2];
								uiu.b[hb] = 0;
								return_master_cc_sec[1] |= (uiu.dw << 4);
							}
							else if (devnmb == c_ctcan_SEB_12)
							{
								return_master_rcc_sec[1] &= 0x000001ffL;
								uiu.b[lb] = 0;
								uiu.b[mlb] = ctcan_rx[ctc_rgix].data[0];
								uiu.b[mhb] = ctcan_rx[ctc_rgix].data[1];
								uiu.b[hb] = ctcan_rx[ctc_rgix].data[2];
								return_master_rcc_sec[1] |= (uiu.dw << 1);
							}
							else if ((cons[RGB_CC] == 1) && (devnmb == c_ctcan_SEB_18))
							{
								return_master_cc_sec[0] &= 0xfffff000L;
								uiu.b[lb] = (ctcan_rx[ctc_rgix].data[0] & 0x3f) | (ctcan_rx[ctc_rgix].data[1] << 2);
								uiu.b[mlb] = (ctcan_rx[ctc_rgix].data[1] >> 6) | ((ctcan_rx[ctc_rgix].data[2] & 0x03) << 2);
								uiu.b[mhb] = 0;
								uiu.b[hb] = 0;
								return_master_cc_sec[0] |= uiu.dw;
							}
	#endif
							ctcan_dev[devnmb].online_cnt = 5;
							ctcan_dev[devnmb].rx_cnt++;
						}
						break;

					case 57:	// message 57: Request for i/o color indexes
						if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
						{	  
							timers[tcanrx + (devnmb-c_ctcan_SEB_1)] = 0;
							ctcan_dev[devnmb].color_ix_req = ctcan_rx[ctc_rgix].data[0];
							ctcan_dev[devnmb].color_ix = 0;
							ctcan_dev[devnmb].online_cnt = 5;
							ctcan_dev[devnmb].rx_cnt++;
							ctcan_put_dev_msg_req (devnmb,58);
						}
						break;
						
					case 60:	// message 60: selector deadzone
						if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
						{	  // id1 is the address
							selector_ul_deadzone = ctcan_rx[ctc_rgix].data[0];
							selector_dl_deadzone = ctcan_rx[ctc_rgix].data[1];
							ctcan_dev[devnmb].rx_cnt++;
						}
						break;

					default:
						break;
				}
			}
			else 
			{
				if ((devnmb >= c_ctcan_START) && (devnmb <= c_ctcan_END))
				{
					ctcan_put_dev_msg_req (devnmb,ctcan_rx[ctc_rgix].msg);
				}
			}
			ctcan_rx[ctc_rgix].valid_msg = 0;
		}
		ctc_rgix++;
		if (ctc_rgix > c_rx_buf_siz)
			ctc_rgix = 0; 
		loop_cnt++;
	}
}


//************************************************************
//*********  Check to see if the Transmit buffer is full *****
//************************************************************

int16 IsTxBufFul(void)
{

	_disable();
	if (ctc_tpix == ctc_tgix)
		availbuf = c_tx_buf_siz + 1;
	else if (ctc_tpix > ctc_tgix)
		availbuf = (c_tx_buf_siz - ctc_tpix) + ctc_tgix + 1;
	else 
		availbuf = ctc_tgix - ctc_tpix;

	_enable();
	if (availbuf > 9)
		return 0;
	else
		return 1;
}


//************************************************************
//*********  Pack data into the Can Structure ****************
//************************************************************


void pack_ctcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req)
{

	int16 iodev_ix = 0;
	uint16 iodataix;
	uint16 io_board = 0;
	unsigned char label_ix = 0;
	unsigned char color_ix = 0;
	unsigned char color_on_ix = 0;
	unsigned char color_off_ix = 0;
	bool valid_name = 0;
	int16 array_start, io_name_start;
	int16 i, j, k, m;

#if (Traction == 1)
	int16 t_blind = 0;
#endif

	_disable();
	if (ctc_tpix < c_tx_buf_siz)
		ctc_tpix++;
	else
		ctc_tpix = 0;
	_enable();
				
	if (tx_req == 1)
	{		 // Used to request a packet from a device
		ctcan_tx[ctc_tpix].rtr = 1;
		ctcan_tx[ctc_tpix].srr = 1;
		ctcan_tx[ctc_tpix].len = 0;
		ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
		ctcan_tx[ctc_tpix].one_tx = 0;
		ctcan_tx[ctc_tpix].msg = msg;
		ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
		ctcan_tx[ctc_tpix].id2 = 0;
		ctcan_tx[ctc_tpix].id3 = 0;
		ctcan_tx[ctc_tpix].data[0] = 0;
		ctcan_tx[ctc_tpix].data[1] = 0;
		ctcan_tx[ctc_tpix].data[2] = 0;
		ctcan_tx[ctc_tpix].data[3] = 0;
		ctcan_tx[ctc_tpix].data[4] = 0;
		ctcan_tx[ctc_tpix].data[5] = 0;
		ctcan_tx[ctc_tpix].data[6] = 0;
		ctcan_tx[ctc_tpix].data[7] = 0;
		if ((devnmb >= c_ctcan_START) && (devnmb <= c_ctcan_END))
		 	ctcan_dev[devnmb].tx_cnt++;
	}
	else switch (msg)
	{
		case 1:			// Init Safety processor Board OR load weigher
			if ((devnmb == c_ctcan_LW) || (devnmb == c_ctcan_VS) || 
				(devnmb == c_ctcan_SEL) || (devnmb == c_ctcan_DOOR) || (devnmb == c_ctcan_RDOOR) )
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 3;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit 
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = version;
				ctcan_tx[ctc_tpix].data[1] = revision;
				ctcan_tx[ctc_tpix].data[2] = 0;	 		// Control byte

				ctcan_tx[ctc_tpix].data[3] = 0;	 
				ctcan_tx[ctc_tpix].data[4] = 0;	 
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;

		case 11:		// Init data
			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_END))
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 5;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				if (ctcan_dev[devnmb].online == 1)
					ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					ctcan_tx[ctc_tpix].one_tx = 1;
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = version;
				ctcan_tx[ctc_tpix].data[1] = revision;
				
				if (cons[housing] == 1) 
				{
/*
					if (devnmb == c_ctcan_SEB_4)
					{
						uiu.dw = cc_mask[cons[carnmb]][0];
						ctcan_tx[ctc_tpix].data[2] = uiu.b[lb];	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[3] = (uiu.b[mlb] & 0x0f);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = 0;	 // Set board call latching mask
					}
					else if (devnmb == c_ctcan_SEB_5) 
					{
						uiu.dw = cc_mask[cons[carnmb]][0];
						ctcan_tx[ctc_tpix].data[2] = ((uiu.b[mlb] >> 4) | (uiu.b[mhb] << 4));	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[3] = ((uiu.b[mhb] >> 4) | (uiu.b[hb] << 4));	 // Set board call latching mask
#if (Tract_HR == 1)
						ctcan_tx[ctc_tpix].data[4] = ((uiu.b[hb] >> 4) & 0x0f);	 // Set board call latching mask
						uiu.dw = cc_mask[cons[carnmb]][1];
						ctcan_tx[ctc_tpix].data[4] |= ((uiu.b[lb] << 4) & 0xf0);	 // Set board call latching mask
#else
						ctcan_tx[ctc_tpix].data[4] = ((uiu.b[hb] >> 4) & 0x01);	 // Set board call latching mask
#endif
					}
					else if (devnmb == c_ctcan_SEB_6) 
					{
						uiu.dw = rcc_mask[cons[carnmb]][0];
						ctcan_tx[ctc_tpix].data[2] = uiu.b[lb];	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[3] = uiu.b[mlb];	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = uiu.b[mhb] & 0x01;	 // Set board call latching mask
					}
				   	else if (devnmb == c_ctcan_SEB_7)
					{
						uiu.dw = rcc_mask[cons[carnmb]][0];
						ctcan_tx[ctc_tpix].data[2] = ((uiu.b[mhb] >> 1) | (uiu.b[hb] << 7));	 // Set board call latching mask
#if (Tract_HR == 1)
						ctcan_tx[ctc_tpix].data[3] = (uiu.b[hb] >> 1);	 // Set board call latching mask
						uiu.dw = rcc_mask[cons[carnmb]][1];
						ctcan_tx[ctc_tpix].data[3] |= (uiu.b[lb] << 7);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = (uiu.b[lb] >> 1) | (uiu.b[mlb] << 7);	 // Set board call latching mask
#else
						ctcan_tx[ctc_tpix].data[3] = ((uiu.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = 0;	 // Set board call latching mask
#endif
					}
#if (Tract_HR == 1)
					else if (devnmb == c_ctcan_SEB_11)
					{
						uiu.dw = cc_mask[cons[carnmb]][1];
						ctcan_tx[ctc_tpix].data[2] = ((uiu.b[lb] >> 4) | (uiu.b[mlb] << 4));	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[3] = ((uiu.b[mlb] >> 4) | (uiu.b[mhb] << 4));	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = ((uiu.b[mhb] >> 4) | (uiu.b[hb] << 4));	 // Set board call latching mask
					}
					else if (devnmb == c_ctcan_SEB_12)
					{
						uiu.dw = rcc_mask[cons[carnmb]][1];
						ctcan_tx[ctc_tpix].data[2] = ((uiu.b[mlb] >> 1) | (uiu.b[mhb] << 7));	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[3] = ((uiu.b[mhb] >> 1) | (uiu.b[hb] << 7));	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = ((uiu.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
					}
#endif
*/
				}
				else
				{
					ctcan_tx[ctc_tpix].data[2] = 0;	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[3] = 0;	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[4] = 0;	 // Set board call latching mask
				}

				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 13:		// I/O Board outputs
			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 8;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				if (ctcan_dev[devnmb].online == 1)
					ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					ctcan_tx[ctc_tpix].one_tx = 1;
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_ctcan_SEB_1);
				iodataix = (int16)((IODATA_CTCAN_START + iodev_ix) * 3); 		// input array
				ctcan_tx[ctc_tpix].data[0] = (uint8)((~serial_ctc.dev[iodev_ix].iob[0] & IO_In_Out[iodataix]));
				ctcan_tx[ctc_tpix].data[1] = (uint8)((~serial_ctc.dev[iodev_ix].iob[1] & IO_In_Out[iodataix+1]));
				ctcan_tx[ctc_tpix].data[2] = (uint8)((~serial_ctc.dev[iodev_ix].iob[2] & IO_In_Out[iodataix+2]));
				if (cons[RGB_CC] == 1)
				{
					uwu.w = CC_ColorOnOffChecksum;
					ctcan_tx[ctc_tpix].data[3] = uwu.b[lbw];
					ctcan_tx[ctc_tpix].data[4] = uwu.b[hbw];
					uwu.w = ctcan_dev[devnmb].ioname_chksum;
					ctcan_tx[ctc_tpix].data[5] = uwu.b[lbw];
					ctcan_tx[ctc_tpix].data[6] = uwu.b[hbw];
					ctcan_tx[ctc_tpix].data[7] = ctcan_dev[devnmb].io_color_ix_chksum;
				}
				else
				{
					ctcan_tx[ctc_tpix].data[3] = 0;
					ctcan_tx[ctc_tpix].data[4] = 0;
					uwu.w = ctcan_dev[devnmb].ioname_chksum;
					ctcan_tx[ctc_tpix].data[5] = uwu.b[lbw];
					ctcan_tx[ctc_tpix].data[6] = uwu.b[hbw];
					ctcan_tx[ctc_tpix].data[7] = 0;
				}
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 15:		// I/O Board inputs sent back to the can serial i/o board
			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 3;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				if (ctcan_dev[devnmb].online == 1)
					ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					ctcan_tx[ctc_tpix].one_tx = 1;
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_ctcan_SEB_1);
				iodataix = (int16)((IODATA_CTCAN_START + iodev_ix) * 3); 		// input array
				ctcan_tx[ctc_tpix].data[0] = (uint8)(serial_ctc.dev[iodev_ix].iob[0] & ~IO_In_Out[iodataix]);
				ctcan_tx[ctc_tpix].data[1] = (uint8)(serial_ctc.dev[iodev_ix].iob[1] & ~IO_In_Out[iodataix+1]);
				ctcan_tx[ctc_tpix].data[2] = (uint8)(serial_ctc.dev[iodev_ix].iob[2] & ~IO_In_Out[iodataix+2]);
				ctcan_tx[ctc_tpix].data[3] = 0;
				ctcan_tx[ctc_tpix].data[4] = 0;
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 21:		// Load weigher command
			if (devnmb == c_ctcan_LW)
			{
#if ((Traction == 1) || (Tract_OL == 1))
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 1;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				if (ctcan_dev[devnmb].online == 1)
					ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					ctcan_tx[ctc_tpix].one_tx = 1;
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = lw_command;
				ctcan_tx[ctc_tpix].data[1] = 0;
				ctcan_tx[ctc_tpix].data[2] = 0;
				ctcan_tx[ctc_tpix].data[3] = 0;
				ctcan_tx[ctc_tpix].data[4] = 0;
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
#endif
			}
			break;
		case 23:		// Selector/SPB position init command
#if (Traction == 1) 
			if (devnmb == c_ctcan_SEL)
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 5;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				if (ctcan_dev[devnmb].online == 1)
					ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					ctcan_tx[ctc_tpix].one_tx = 1;
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				uiu.dw = SEL_Preset_Count;
				ctcan_tx[ctc_tpix].data[0] = 1;		// preset count flag
				ctcan_tx[ctc_tpix].data[1] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[2] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[3] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[4] = uiu.b[hb];
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
			}
#endif
			break;
		case 25:		// PI and Lantern message
			if (devnmb == c_ctcan_PI_LANT)
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 8;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[c_ctcan_PI_LANT];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
#if (Traction == 1)
				t_blind = blind();
#endif
				if (cons[Xref_PI_Pos] == 1)
					ctcan_tx[ctc_tpix].data[0] = xpi_pos[pi_pos];
#if (Traction == 1)
				else if (t_blind == 1)
					ctcan_tx[ctc_tpix].data[0] = (uint8)(cons[topf] + 1);
				else if (t_blind == 2)
					ctcan_tx[ctc_tpix].data[0] = (uint8)(cons[topf] + 2);
#endif
				else
					ctcan_tx[ctc_tpix].data[0] = pi_pos;

				if ((servf == s_SABBATH) && ((fvars[fvsabbdis] & 0x01) != 0))
				{
					ctcan_tx[ctc_tpix].data[1] = 'S';
					ctcan_tx[ctc_tpix].data[2] = 'B';
				}
				else if (cons[Alt_Ascii_pi] == 1)
				{
					if ( ((alt_flmrk[pi_pos][0]) != (flmrk[pi_pos][0])) ||
						((alt_flmrk[pi_pos][1]) != (flmrk[pi_pos][1])) )
					{
						if ((timers[taltpi_on]) > 20)
						{
							if ((timers[taltpi_off]) > 20)
								timers[taltpi_on] = 0;
							ctcan_tx[ctc_tpix].data[1] = alt_flmrk[pi_pos][0];
							ctcan_tx[ctc_tpix].data[2] = alt_flmrk[pi_pos][1];
						}
						else
						{
							timers[taltpi_off] = 0;
							ctcan_tx[ctc_tpix].data[1] = flmrk[pi_pos][0];
					  		ctcan_tx[ctc_tpix].data[2] = flmrk[pi_pos][1];
						}
					}
					else {
							timers[taltpi_off] = 0;
							timers[taltpi_on] = 0;
#if (Traction == 1)
							if ((t_blind == 1) && (cons[Blind_Floor] != 0))
							{
								ctcan_tx[ctc_tpix].data[1] = Blind_flmrk[0][0];
								ctcan_tx[ctc_tpix].data[2] = Blind_flmrk[0][1];
							}
							else if ((t_blind == 2) && (cons[Blind_Floor_2] != 0))
							{
								ctcan_tx[ctc_tpix].data[1] = Blind_flmrk[1][0];
								ctcan_tx[ctc_tpix].data[2] = Blind_flmrk[1][1];
							}
							else
#endif
							{
							
								ctcan_tx[ctc_tpix].data[1] = flmrk[pi_pos][0];
								ctcan_tx[ctc_tpix].data[2] = flmrk[pi_pos][1];
							}
					}
				}
				else if ((cons[Alt_Ascii_pi] == 2) && (rdinp(i_ALTPI)) && (firef == 0) && (rfiref == 0))
				{
					ctcan_tx[ctc_tpix].data[1] = alt_flmrk[pi_pos][0];
					ctcan_tx[ctc_tpix].data[2] = alt_flmrk[pi_pos][1];
				}
				else 
				{
#if (Traction == 1)
					if ((t_blind == 1) && (cons[Blind_Floor] != 0))
					{
						ctcan_tx[ctc_tpix].data[1] = Blind_flmrk[0][0];
						ctcan_tx[ctc_tpix].data[2] = Blind_flmrk[0][1];
					}
					else if ((t_blind == 2) && (cons[Blind_Floor_2] != 0))
					{
						ctcan_tx[ctc_tpix].data[1] = Blind_flmrk[1][0];
						ctcan_tx[ctc_tpix].data[2] = Blind_flmrk[1][1];
					}
					else
#endif
					{
						ctcan_tx[ctc_tpix].data[1] = flmrk[pi_pos][0];
						ctcan_tx[ctc_tpix].data[2] = flmrk[pi_pos][1];
					}
				}
				if((alrm_pi_stat == 1) && (timers[tsec] != 0))
				{
					ctcan_tx[ctc_tpix].data[1] = 'A';
					ctcan_tx[ctc_tpix].data[2] = 'L';
				}
				
				

				ctcan_tx[ctc_tpix].data[3] = pi_lant_stat;
				ctcan_tx[ctc_tpix].data[4] = pi_motion_stat;
				ctcan_tx[ctc_tpix].data[5] = pi_fire_stat;
				ctcan_tx[ctc_tpix].data[6] = pi_door_stat;
				ctcan_tx[ctc_tpix].data[7] = pi_misc_stat;
			}
			break;
/*		case 26:		// Hall Assignment Annunciator
			if (devnmb == c_ctcan_PI_LANT)
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 8;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[c_ctcan_PI_LANT];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;

				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
					((cons[Car_Sw] & 1) != 0))
				{
					if (cons[Second_Riser] != 0)
						uiu.dw = (c_up_hall_call[0] | c_irup_hall_call[0]);
					else
						uiu.dw = c_up_hall_call[0];
				}
				else
					uiu.dw = 0;

				ctcan_tx[ctc_tpix].data[0] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[1] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[2] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[3] = uiu.b[hb];

				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
					((cons[Car_Sw] & 1) != 0))
				{
					if (cons[Second_Riser] != 0)
						uiu.dw = (c_dn_hall_call[0] | c_irdn_hall_call[0]);
					else
						uiu.dw = c_dn_hall_call[0];
				}
				else
					uiu.dw = 0;

				ctcan_tx[ctc_tpix].data[4] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[5] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[6] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[7] = uiu.b[hb];
			}
			break;
		case 27:		// Rear Hall Assignment Annunciator
			if (devnmb == c_ctcan_PI_LANT)
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 8;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[c_ctcan_PI_LANT];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;

				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
					((cons[Car_Sw] & 1) != 0))
				{
					if (cons[Second_Riser] != 0)
						uiu.dw = (c_upr_hall_call[0] | c_rirup_hall_call[0]);
					else
						uiu.dw = c_upr_hall_call[0];
				}
				else
					uiu.dw = 0;

				ctcan_tx[ctc_tpix].data[0] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[1] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[2] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[3] = uiu.b[hb];

				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
					((cons[Car_Sw] & 1) != 0))
				{
					if (cons[Second_Riser] != 0)
						uiu.dw = (c_dnr_hall_call[0] | c_rirdn_hall_call[0]);
					else
						uiu.dw = c_dnr_hall_call[0];
				}
				else
					uiu.dw = 0;

				ctcan_tx[ctc_tpix].data[4] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[5] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[6] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[7] = uiu.b[hb];
			}
			break;
*/	
		case 28:		// Door Control message
			if (devnmb == c_ctcan_DOOR)
			{			  // Front Door
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 3;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = DOOR_can.command;
				ctcan_tx[ctc_tpix].data[1] = DOOR_can.status_ret;
				ctcan_tx[ctc_tpix].data[2] = DOOR_can.dpy_req;
				ctcan_tx[ctc_tpix].data[3] = 0;
				ctcan_tx[ctc_tpix].data[4] = 0;
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				DOOR_can.prev_command = DOOR_can.command;
				ctcan_dev[devnmb].tx_cnt++;
			}
			if (devnmb == c_ctcan_RDOOR)
			{			// Rear Door
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 3;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = RDOOR_can.command;
				ctcan_tx[ctc_tpix].data[1] = RDOOR_can.status_ret;
				ctcan_tx[ctc_tpix].data[2] = RDOOR_can.dpy_req;
				ctcan_tx[ctc_tpix].data[3] = 0;
				ctcan_tx[ctc_tpix].data[4] = 0;
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				RDOOR_can.prev_command = RDOOR_can.command;
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 30:		// Door Parameter Read/Write request
			if (devnmb == c_ctcan_DOOR)
			{			  // Front Door
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 5;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = DOOR_can.par_cmd;
				uwu.w = DOOR_can.par_add_req;
				ctcan_tx[ctc_tpix].data[1] = uwu.b[hbw];
				ctcan_tx[ctc_tpix].data[2] = uwu.b[lbw];
				uwu.w = DOOR_can.par_dat_wrt;
				ctcan_tx[ctc_tpix].data[3] = uwu.b[hbw];
				ctcan_tx[ctc_tpix].data[4] = uwu.b[lbw];
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				DOOR_can.par_cmd &= ~b_door_par_wr;
				DOOR_can.prev_command = DOOR_can.command;
				ctcan_dev[devnmb].tx_cnt++;
			}
			if (devnmb == c_ctcan_RDOOR)
			{			// Rear Door
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 5;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = RDOOR_can.par_cmd;
				uwu.w = RDOOR_can.par_add_req;
				ctcan_tx[ctc_tpix].data[1] = uwu.b[hbw];
				ctcan_tx[ctc_tpix].data[2] = uwu.b[lbw];
				uwu.w = RDOOR_can.par_dat_wrt;
				ctcan_tx[ctc_tpix].data[3] = uwu.b[hbw];
				ctcan_tx[ctc_tpix].data[4] = uwu.b[lbw];
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				RDOOR_can.par_cmd &= ~b_door_par_wr;
				RDOOR_can.prev_par_cmd = RDOOR_can.par_cmd;
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 35:		// Selector RETURN data
			if (devnmb == c_ctcan_SEL)
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 6;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				if (ctcan_dev[devnmb].online == 1)
					ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					ctcan_tx[ctc_tpix].one_tx = 1;
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = SEL_can.status;
				ctcan_tx[ctc_tpix].data[1] = SEL_can.position;
				ctcan_tx[ctc_tpix].data[2] = SEL_can.doorzone;
				ctcan_tx[ctc_tpix].data[3] = SEL_can.up_normal_limits;
				ctcan_tx[ctc_tpix].data[4] = SEL_can.dn_normal_limits;
				ctcan_tx[ctc_tpix].data[5] = SEL_can.em_terminal_limits;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 43:		// Global Car Call Light Brightness and Color	 (any one can request a 43)
			ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
			ctcan_tx[ctc_tpix].len = 8;
			ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
			ctcan_tx[ctc_tpix].one_tx = 0;		
			ctcan_tx[ctc_tpix].msg = msg;
			ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[c_ctcan_GLOBAL];
			ctcan_tx[ctc_tpix].id2 = 0;
			ctcan_tx[ctc_tpix].id3 = 0;
			ctcan_tx[ctc_tpix].data[0] =	(uint8)color_send_ix;
			if (color_send_ix == 0)
			{
				ctcan_tx[ctc_tpix].data[1] = 9;
				ctcan_tx[ctc_tpix].data[2] = (uint8)ccOnBright;			// Call On Brightness
				ctcan_tx[ctc_tpix].data[3] = (uint8)ccOffBright;		// Call Off Brightness
				ctcan_tx[ctc_tpix].data[4] = (uint8)ccSecOnRed;			// Security On Red
				ctcan_tx[ctc_tpix].data[5] = (uint8)ccSecOnGreen; 		// Security On Green
				ctcan_tx[ctc_tpix].data[6] = (uint8)ccSecOnBlue; 		// Security Blue
			}
			else if (color_send_ix == 1)
			{
				ctcan_tx[ctc_tpix].data[1] = (uint8)ccAttUpOnRed;			// Attendant Up On Red
				ctcan_tx[ctc_tpix].data[2] = (uint8)ccAttUpOnGreen;			// Attendant Up On Green
				ctcan_tx[ctc_tpix].data[3] = (uint8)ccAttUpOnBlue;			// Attendant Up On Blue
				ctcan_tx[ctc_tpix].data[4] = (uint8)ccAttDnOnRed;			// Attendant Dn On Red
				ctcan_tx[ctc_tpix].data[5] = (uint8)ccAttDnOnGreen; 		// Attendant Dn On Green
				ctcan_tx[ctc_tpix].data[6] = (uint8)ccAttDnOnBlue;			// Attendant Dn On Blue
			}
			else if (color_send_ix == 2)
			{
				ctcan_tx[ctc_tpix].data[1] = (uint8)ccOffRed;			// Call Off Red
				ctcan_tx[ctc_tpix].data[2] = (uint8)ccOffGreen;			// Call Off Green
				ctcan_tx[ctc_tpix].data[3] = (uint8)ccOffBlue;			// Call Off Blue
				ctcan_tx[ctc_tpix].data[4] = (uint8)ccOnRed;			// Call On Red
				ctcan_tx[ctc_tpix].data[5] = (uint8)ccOnGreen;			// Call On Green
				ctcan_tx[ctc_tpix].data[6] = (uint8)ccOnBlue;			// Call On Blue
			}
			else if (color_send_ix == 3)
			{
				ctcan_tx[ctc_tpix].data[1] = (uint8)copFireOnRed;			// Fire On Red
				ctcan_tx[ctc_tpix].data[2] = (uint8)copFireOnGreen;			// Fire On Green
				ctcan_tx[ctc_tpix].data[3] = (uint8)copFireOnBlue;			// Fire On Blue
				ctcan_tx[ctc_tpix].data[4] = (uint8)copMedOnRed;			// Medical On Red
				ctcan_tx[ctc_tpix].data[5] = (uint8)copMedOnGreen; 			// Medical On Green
				ctcan_tx[ctc_tpix].data[6] = (uint8)copMedOnBlue;			// Medical On Blue
			}
			else if (color_send_ix == 4)
			{
				ctcan_tx[ctc_tpix].data[1] = (uint8)copEmOnRed;				// Emergency On Red
				ctcan_tx[ctc_tpix].data[2] = (uint8)copEmOnGreen;			// Emergency On Green
				ctcan_tx[ctc_tpix].data[3] = (uint8)copEmOnBlue;			// Emergency On Blue
				ctcan_tx[ctc_tpix].data[4] = (uint8)copOTSOnRed;			// OTS On Red
				ctcan_tx[ctc_tpix].data[5] = (uint8)copOTSOnGreen; 			// OTS On Green
				ctcan_tx[ctc_tpix].data[6] = (uint8)copOTSOnBlue;			// OTS On Blue
			}
			color_send_ix++;
			ctcan_tx[ctc_tpix].data[7] = 0;
			if (color_send_ix <= 4)
			{
				ctcan_put_dev_msg_req (devnmb,43);
			}
			else
				color_send_ix = 0;	
			break;

		case 47:		// Send I/O Board Label
			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
			{
				if (ctcan_dev[devnmb].label_req != 0)
				{
					iodev_ix = (int16)(devnmb-c_ctcan_SEB_1);
					label_ix = ctcan_dev[devnmb].label_ix;
					array_start = (int16)((IODATA_CTCAN_START + iodev_ix) * 3); 		// input array
					io_board = (int16)(IODATA_CTCAN_START + iodev_ix);
					valid_name = 0;
					k = 0;		// default to input name (even if all spaces)
					i = (int16)((label_ix + 12)/8);		   
					j = (int16)((label_ix  + 12)% 8);
					if ((io_send_name[array_start+i] & bittbl[j & 0x07]) != 0)
					{
						valid_name = 1;							// valid output name
						k = 12;
					}
					else 
					{
						i = (int16)(label_ix/8);		   
						j = (int16)(label_ix % 8);
						if ((io_send_name[array_start + i] & bittbl[j & 0x07]) != 0)
							valid_name = 1;							// valid input name

					}
						
					ctcan_tx[ctc_tpix].rtr = 0;
					ctcan_tx[ctc_tpix].srr = 1;
					ctcan_tx[ctc_tpix].len = 8;
					ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
					if (ctcan_dev[devnmb].online == 1)
						ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
					else
						ctcan_tx[ctc_tpix].one_tx = 1;
					ctcan_tx[ctc_tpix].msg = msg;
					ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
					ctcan_tx[ctc_tpix].id2 = 0;
					ctcan_tx[ctc_tpix].id3 = 0;
					ctcan_tx[ctc_tpix].data[0] = label_ix;
					io_name_start = (array_start * 8) + k;
					if(valid_name == 1)
					{
						ctcan_tx[ctc_tpix].data[1] = ioname[io_name_start + label_ix][0];
						ctcan_tx[ctc_tpix].data[2] = ioname[io_name_start + label_ix][1];
						ctcan_tx[ctc_tpix].data[3] = ioname[io_name_start + label_ix][2];
						ctcan_tx[ctc_tpix].data[4] = ioname[io_name_start + label_ix][3];
						ctcan_tx[ctc_tpix].data[5] = ioname[io_name_start + label_ix][4];
					}
					else
					{
						ctcan_tx[ctc_tpix].data[1] = ' ';
						ctcan_tx[ctc_tpix].data[2] = ' ';
						ctcan_tx[ctc_tpix].data[3] = ' ';
						ctcan_tx[ctc_tpix].data[4] = ' ';
						ctcan_tx[ctc_tpix].data[5] = ' ';
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

					if ((cons[RGB_CC] != 0) && ((IO_board[cons[carnmb]][io_board] & 0x02) != 0))
					{
						ctcan_tx[ctc_tpix].data[6] = color_on_ix;		// On State Color
						ctcan_tx[ctc_tpix].data[7] = color_off_ix;	    // Off State Color
					}
					else
					{
						ctcan_tx[ctc_tpix].data[6] = 0;		// On State Color
						ctcan_tx[ctc_tpix].data[7] = 0;	    // Off State Color
					}
					label_ix ++;
					ctcan_dev[devnmb].label_ix = label_ix;
					if (label_ix <= 11) 
					{
						ctcan_put_dev_msg_req (devnmb,47);
					}
					else
					{
						ctcan_dev[devnmb].label_ix = 0;
						ctcan_dev[devnmb].label_req = 0;
					}
					ctcan_dev[devnmb].tx_cnt++;
				}
			}
			break;
		case 49:			// Message 49: Reset Error Message
			ctcan_tx[ctc_tpix].rtr = 0;
			ctcan_tx[ctc_tpix].srr = 1;
			ctcan_tx[ctc_tpix].len = 1;
			ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
			ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit 
			ctcan_tx[ctc_tpix].msg = msg;
			ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
			ctcan_tx[ctc_tpix].id2 = 0;
			ctcan_tx[ctc_tpix].id3 = 0;
			ctcan_tx[ctc_tpix].data[0] = 0x03;		// Control byte - bit0: reset error , bit 1: clear error buffer
			ctcan_tx[ctc_tpix].data[1] = 0;
			ctcan_tx[ctc_tpix].data[2] = 0;	 		 
			ctcan_tx[ctc_tpix].data[3] = 0;	 
			ctcan_tx[ctc_tpix].data[4] = 0;	 
			ctcan_tx[ctc_tpix].data[5] = 0;
			ctcan_tx[ctc_tpix].data[6] = 0;
			ctcan_tx[ctc_tpix].data[7] = 0;
			ctcan_dev[devnmb].tx_cnt++;
			break;
/*		case 50:		// Hall Assignment Annunciator high rise floors
			if (devnmb == c_ctcan_PI_LANT)
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 8;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[c_ctcan_PI_LANT];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;

				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
					((cons[Car_Sw] & 1) != 0))
				{
					if (cons[Second_Riser] != 0)
						uiu.dw = (c_up_hall_call[1] | c_irup_hall_call[1]);
					else
						uiu.dw = c_up_hall_call[1];
				}
				else
					uiu.dw = 0;

				ctcan_tx[ctc_tpix].data[0] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[1] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[2] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[3] = uiu.b[hb];

				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
					((cons[Car_Sw] & 1) != 0))
				{
					if (cons[Second_Riser] != 0)
						uiu.dw = (c_dn_hall_call[1] | c_irdn_hall_call[1]);
					else
						uiu.dw = c_dn_hall_call[1];
				}
				else
					uiu.dw = 0;

				ctcan_tx[ctc_tpix].data[4] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[5] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[6] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[7] = uiu.b[hb];
			}
			break;
		case 51:		// Rear Hall Assignment Annunciator high rise floors
			if (devnmb == c_ctcan_PI_LANT)
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 8;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;		
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[c_ctcan_PI_LANT];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;

				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
					((cons[Car_Sw] & 1) != 0))
				{
					if (cons[Second_Riser] != 0)
						uiu.dw = (c_upr_hall_call[1] | c_rirup_hall_call[1]);
					else
						uiu.dw = c_upr_hall_call[1];
				}
				else
					uiu.dw = 0;

				ctcan_tx[ctc_tpix].data[0] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[1] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[2] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[3] = uiu.b[hb];

				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
					((cons[Car_Sw] & 1) != 0))
				{
					if (cons[Second_Riser] != 0)
						uiu.dw = (c_dnr_hall_call[1] | c_rirdn_hall_call[1]);
					else
						uiu.dw = c_dnr_hall_call[1];
				}
				else
					uiu.dw = 0;

				ctcan_tx[ctc_tpix].data[4] = uiu.b[lb];
				ctcan_tx[ctc_tpix].data[5] = uiu.b[mlb];
				ctcan_tx[ctc_tpix].data[6] = uiu.b[mhb];
				ctcan_tx[ctc_tpix].data[7] = uiu.b[hb];
			}
			break;
*/
		case 52:			// Message 52: Sync Timer
			ctcan_tx[ctc_tpix].rtr = 0;
			ctcan_tx[ctc_tpix].srr = 1;
			ctcan_tx[ctc_tpix].len = 3;
			ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
			ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit 
			ctcan_tx[ctc_tpix].msg = msg;
			ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
			ctcan_tx[ctc_tpix].id2 = 0;
			ctcan_tx[ctc_tpix].id3 = 0;
			if (lt_status_count == 3)
				cop_lt_status  = 0x04;			// turn off security light
			else if (lt_status_count == 1)
				cop_lt_status = 0x01;			// turn on up attendant
			else if (lt_status_count == 2)
				cop_lt_status = 0x02;			// turn on down attendant
			else
				cop_lt_status = 0;

			if ((fvars[fvccltctl] & 0x02) != 0)
			{
				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) || ((cons[Car_Sw] & 1) != 0))
					cop_lt_status |= 0x80;
			}
							
			ctcan_tx[ctc_tpix].data[0] = ctcan_sync_count;		
			ctcan_tx[ctc_tpix].data[1] = (uint8)fvars[fvcansynccnt];
			ctcan_tx[ctc_tpix].data[2] = cop_lt_status;	 		 
			ctcan_tx[ctc_tpix].data[3] = 0;	 
			ctcan_tx[ctc_tpix].data[4] = 0;	 
			ctcan_tx[ctc_tpix].data[5] = 0;
			ctcan_tx[ctc_tpix].data[6] = 0;
			ctcan_tx[ctc_tpix].data[7] = 0;
			ctcan_sync_count++;
			if (ctcan_sync_count > fvars[fvcansynccnt])
			{
				ctcan_sync_count = 0;
				lt_status_count ++;
				if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
 					((cons[Car_Sw] & 1) != 0))
 				{
 			 		if ((c_up_hall_call[0] == 0) && (c_up_hall_call[1] == 0))
 					{
 						if (lt_status_count == 1)
 							lt_status_count++;
 					}
 			 		if ((c_dn_hall_call[0] == 0) && (c_dn_hall_call[1] == 0))
 					{
 						if (lt_status_count == 2)
 							lt_status_count++;
 					}
					if (lt_status_count > 2)
						lt_status_count = 0;
 				}

 				else if ((master_cc_sec[0] != 0) || (master_cc_sec[0] != 0))
				{
					if ((fvars[fvccltctl] & 0x01) != 0)
					{
						if (lt_status_count == 1)
							lt_status_count = 3;

						if (lt_status_count > 3)
							lt_status_count = 0;
					}
					else
						lt_status_count = 0;
				}
			}
			
			break;
		case 53:		// Send Attendant Up and Down data
			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_END))
			{		// flash lights on RGB boards
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 6;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				if (ctcan_dev[devnmb].online == 1)
					ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					ctcan_tx[ctc_tpix].one_tx = 1;
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;

				if (devnmb == c_ctcan_SEB_4)
				{
					if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
						((cons[Car_Sw] & 1) != 0))
					{
						uiu.dw = c_up_hall_call[0];
						uiu_2.dw = c_dn_hall_call[0];
					}
					else
					{
						uiu.dw = 0;
						uiu_2.dw = 0;
					}
					ctcan_tx[ctc_tpix].data[0] = uiu.b[lb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = (uiu.b[mlb] & 0x0f);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = 0;	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[3] = uiu_2.b[lb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[4] = (uiu_2.b[mlb] & 0x0f);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[5] = 0;	 // Set board call latching mask
				}
				else if (devnmb == c_ctcan_SEB_5) 
				{
					if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
						((cons[Car_Sw] & 1) != 0))
					{
						uiu.dw = c_up_hall_call[0];
						uiu_2.dw = c_dn_hall_call[0];
						ctcan_tx[ctc_tpix].data[0] = ((uiu.b[mlb] >> 4) | (uiu.b[mhb] << 4));	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[1] = ((uiu.b[mhb] >> 4) | (uiu.b[hb] << 4));	 // Set board call latching mask
		
						ctcan_tx[ctc_tpix].data[3] = ((uiu_2.b[mlb] >> 4) | (uiu_2.b[mhb] << 4));	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = ((uiu_2.b[mhb] >> 4) | (uiu_2.b[hb] << 4));	 // Set board call latching mask
#if (Tract_HR == 1)
				   		ctcan_tx[ctc_tpix].data[2] = ((uiu.b[hb] >> 4) & 0x0f);	 // Set board call latching mask
				   		ctcan_tx[ctc_tpix].data[5] = ((uiu_2.b[hb] >> 4) & 0x0f);	 // Set board call latching mask

						uiu.dw = c_up_hall_call[0];
						uiu_2.dw = c_dn_hall_call[0];
				   		ctcan_tx[ctc_tpix].data[2] |= ((uiu.b[lb] << 4) & 0xf0);	 // Set board call latching mask
				   		ctcan_tx[ctc_tpix].data[5] |= ((uiu_2.b[lb] << 4) & 0xf0);	 // Set board call latching mask
#else
						ctcan_tx[ctc_tpix].data[2] = ((uiu.b[hb] >> 4) & 0x01);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[5] = ((uiu_2.b[hb] >> 4) & 0x01);	 // Set board call latching mask
#endif
					}
					else
					{
						ctcan_tx[ctc_tpix].data[0] = 0;
						ctcan_tx[ctc_tpix].data[1] = 0;
						ctcan_tx[ctc_tpix].data[2] = 0;
						ctcan_tx[ctc_tpix].data[3] = 0;
						ctcan_tx[ctc_tpix].data[4] = 0;
						ctcan_tx[ctc_tpix].data[5] = 0;
					}
				}
				else if (devnmb == c_ctcan_SEB_6) 
				{
					if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
						((cons[Car_Sw] & 1) != 0))
					{
						uiu.dw = c_upr_hall_call[0];
						uiu_2.dw = c_dnr_hall_call[0];
					}
					else
					{
						uiu.dw = 0;
						uiu_2.dw = 0;
					}
					ctcan_tx[ctc_tpix].data[0] = uiu.b[lb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = uiu.b[mlb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = uiu.b[mhb] & 0x01;	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[3] = uiu_2.b[lb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[4] = uiu_2.b[mlb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[5] = uiu_2.b[mhb] & 0x01;	 // Set board call latching mask
				}
				else if (devnmb == c_ctcan_SEB_7)
				{
					if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
						((cons[Car_Sw] & 1) != 0))
					{
						uiu.dw = c_upr_hall_call[0];
						uiu_2.dw = c_dnr_hall_call[0];

						ctcan_tx[ctc_tpix].data[0] = ((uiu.b[mhb] >> 1) | (uiu.b[hb] << 7));	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[3] = ((uiu_2.b[mhb] >> 1) | (uiu_2.b[hb] << 7));	 // Set board call latching mask
#if (Tract_HR == 1)
						ctcan_tx[ctc_tpix].data[1] = (uiu.b[hb] >> 1);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = (uiu_2.b[hb] >> 1);	 // Set board call latching mask
						uiu.dw = c_upr_hall_call[1];
						uiu_2.dw = c_dnr_hall_call[1];
						ctcan_tx[ctc_tpix].data[1] |= (uiu.b[lb] << 7);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[2] = (uiu.b[lb] >> 1) | (uiu.b[mlb] << 7);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] |= (uiu_2.b[lb] << 7);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[5] = (uiu_2.b[lb] >> 1) | (uiu_2.b[mlb] << 7);	 // Set board call latching mask
#else
						ctcan_tx[ctc_tpix].data[1] = ((uiu.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[2] = 0;	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[4] = ((uiu_2.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
						ctcan_tx[ctc_tpix].data[5] = 0;	 // Set board call latching mask
#endif
					}
					else
					{
						ctcan_tx[ctc_tpix].data[0] = 0;
						ctcan_tx[ctc_tpix].data[1] = 0;
						ctcan_tx[ctc_tpix].data[2] = 0;
						ctcan_tx[ctc_tpix].data[3] = 0;
						ctcan_tx[ctc_tpix].data[4] = 0;
						ctcan_tx[ctc_tpix].data[5] = 0;
					}
				}
#if (Tract_HR == 1)
				else if (devnmb == c_ctcan_SEB_11)
				{
					if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
						((cons[Car_Sw] & 1) != 0))
					{
						uiu.dw = c_up_hall_call[1];
						uiu_2.dw = c_dn_hall_call[1];
					}
					else
					{
						uiu.dw = 0;
						uiu_2.dw = 0;
					}
					ctcan_tx[ctc_tpix].data[0] = ((uiu.b[lb] >> 4) | (uiu.b[mlb] << 4));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = ((uiu.b[mlb] >> 4) | (uiu.b[mhb] << 4));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = ((uiu.b[mhb] >> 4) | (uiu.b[hb] << 4));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[3] = ((uiu_2.b[lb] >> 4) | (uiu_2.b[mlb] << 4));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[4] = ((uiu_2.b[mlb] >> 4) | (uiu_2.b[mhb] << 4));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[5] = ((uiu_2.b[mhb] >> 4) | (uiu_2.b[hb] << 4));	 // Set board call latching mask
				}
				else if (devnmb == c_ctcan_SEB_12)
				{
					if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
						((cons[Car_Sw] & 1) != 0))
					{
						uiu.dw = c_upr_hall_call[1];
						uiu_2.dw = c_dnr_hall_call[1];
					}
					else
					{
						uiu.dw = 0;
						uiu_2.dw = 0;
					}
					ctcan_tx[ctc_tpix].data[0] = ((uiu.b[mlb] >> 1) | (uiu.b[mhb] << 7));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = ((uiu.b[mhb] >> 1) | (uiu.b[hb] << 7));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = ((uiu.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[3] = ((uiu_2.b[mlb] >> 1) | (uiu_2.b[mhb] << 7));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[4] = ((uiu_2.b[mhb] >> 1) | (uiu_2.b[hb] << 7));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[5] = ((uiu_2.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
				}
				else if ((cons[RGB_CC] == 1) && (devnmb == c_ctcan_SEB_18))
				{
					if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
						((cons[Car_Sw] & 1) != 0))
					{
						uiu.dw = c_up_hall_call[0];
						uiu_2.dw = c_dn_hall_call[0];
					}
					else
					{
						uiu.dw = 0;
						uiu_2.dw = 0;
					}
					ctcan_tx[ctc_tpix].data[0] = uiu.b[lb] & 0x3f;	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = ((uiu.b[lb] & 0xc0) >> 2) | (uiu.b[mlb] << 6);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = (uiu.b[mlb] >> 2) & 0x02;	 // Set board call latching mask

					ctcan_tx[ctc_tpix].data[3] = uiu_2.b[lb] & 0x3f;	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[4] = ((uiu_2.b[lb] & 0xc0) >> 2) | (uiu_2.b[mlb] << 6);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[5] = (uiu_2.b[mlb] >> 2) & 0x02;	 // Set board call latching mask
				}
#endif
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 55:		// Send Car Call Security data
			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_END))
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 3;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				if (ctcan_dev[devnmb].online == 1)
					ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					ctcan_tx[ctc_tpix].one_tx = 1;
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;

				if (devnmb == c_ctcan_SEB_4)
				{
					uiu.dw = master_cc_sec[0];
					ctcan_tx[ctc_tpix].data[0] = uiu.b[lb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = (uiu.b[mlb] & 0x0f);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = 0;	 // Set board call latching mask
				}
				else if (devnmb == c_ctcan_SEB_5) 
				{
					uiu.dw = master_cc_sec[0];
					ctcan_tx[ctc_tpix].data[0] = ((uiu.b[mlb] >> 4) | (uiu.b[mhb] << 4));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = ((uiu.b[mhb] >> 4) | (uiu.b[hb] << 4));	 // Set board call latching mask
		
#if (Tract_HR == 1)
			   		ctcan_tx[ctc_tpix].data[2] = ((uiu.b[hb] >> 4) & 0x0f);	 // Set board call latching mask

					uiu.dw = master_cc_sec[0];
			   		ctcan_tx[ctc_tpix].data[2] |= ((uiu.b[lb] << 4) & 0xf0);	 // Set board call latching mask
#else
					ctcan_tx[ctc_tpix].data[2] = ((uiu.b[hb] >> 4) & 0x01);	 // Set board call latching mask
#endif
				}
				else if (devnmb == c_ctcan_SEB_6) 
				{
					uiu.dw = master_rcc_sec[0];
					ctcan_tx[ctc_tpix].data[0] = uiu.b[lb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = uiu.b[mlb];	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = uiu.b[mhb] & 0x01;	 // Set board call latching mask
				}
				else if (devnmb == c_ctcan_SEB_7)
				{
					uiu.dw = master_rcc_sec[0];

					ctcan_tx[ctc_tpix].data[0] = ((uiu.b[mhb] >> 1) | (uiu.b[hb] << 7));	 // Set board call latching mask
#if (Tract_HR == 1)
					ctcan_tx[ctc_tpix].data[1] = (uiu.b[hb] >> 1);	 // Set board call latching mask
					uiu.dw = master_rcc_sec[1];
					ctcan_tx[ctc_tpix].data[1] |= (uiu.b[lb] << 7);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = (uiu.b[lb] >> 1) | (uiu.b[mlb] << 7);	 // Set board call latching mask
#else
					ctcan_tx[ctc_tpix].data[1] = ((uiu.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = 0;	 // Set board call latching mask
#endif
				}
#if (Tract_HR == 1)
				else if (devnmb == c_ctcan_SEB_11)
				{
					uiu.dw = master_cc_sec[1];
					ctcan_tx[ctc_tpix].data[0] = ((uiu.b[lb] >> 4) | (uiu.b[mlb] << 4));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = ((uiu.b[mlb] >> 4) | (uiu.b[mhb] << 4));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = ((uiu.b[mhb] >> 4) | (uiu.b[hb] << 4));	 // Set board call latching mask
				}
				else if (devnmb == c_ctcan_SEB_12)
				{
					uiu.dw = master_cc_sec[1];
					ctcan_tx[ctc_tpix].data[0] = ((uiu.b[mlb] >> 1) | (uiu.b[mhb] << 7));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = ((uiu.b[mhb] >> 1) | (uiu.b[hb] << 7));	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = ((uiu.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
				}
				else if ((cons[RGB_CC] == 1) && (devnmb == c_ctcan_SEB_18))
				{
					uiu.dw = master_cc_sec[0];
					ctcan_tx[ctc_tpix].data[0] = uiu.b[lb] & 0x3f;	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[1] = ((uiu.b[lb] & 0xc0) >> 2) | (uiu.b[mlb] << 6);	 // Set board call latching mask
					ctcan_tx[ctc_tpix].data[2] = (uiu.b[mlb] >> 2) & 0x02;	 // Set board call latching mask
				}
#endif
				ctcan_tx[ctc_tpix].data[3] = 0;
				ctcan_tx[ctc_tpix].data[4] = 0;
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
			}
			break;

		case 58:		// Send I/O Board i/o Color index
			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
			{
				if (ctcan_dev[devnmb].color_ix_req != 0)
				{
					iodev_ix = (int16)(devnmb-c_ctcan_SEB_1);
					color_ix = ctcan_dev[devnmb].color_ix;	
					array_start = (int16)((IODATA_CTCAN_START + iodev_ix) * 3); 		// input array
					//m = color_ix * 3;

					ctcan_tx[ctc_tpix].rtr = 0;
					ctcan_tx[ctc_tpix].srr = 1;
					ctcan_tx[ctc_tpix].len = 7;
					ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
					if (ctcan_dev[devnmb].online == 1)
						ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
					else
						ctcan_tx[ctc_tpix].one_tx = 1;
					ctcan_tx[ctc_tpix].msg = msg;
					ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
					ctcan_tx[ctc_tpix].id2 = 0;
					ctcan_tx[ctc_tpix].id3 = 0;
					ctcan_tx[ctc_tpix].data[0] = color_ix;	 //changed mhd

					for(m=0;m<3;m++)
					{
						i = (int16)(color_ix/8);		   // io_ix = 0 to 23
						j = (int16)(color_ix % 8);

						if ((io_send_name[array_start+i] & bittbl[j & 0x07]) != 0)
							k = 0;	   // input label is set
						else if ((io_send_name[array_start+3+i] & bittbl[j & 0x07]) != 0)
							k = 3;		// output label is set
						else
							k = 0; // default to input label if not set

						io_name_start = (array_start + k) * 8;
						//color_on_ix = io_on_color_ix[io_name_start + m];	   //changed drw

						color_on_ix = io_on_color_ix[io_name_start + color_ix];	   //changed drw
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

						ctcan_tx[ctc_tpix].data[(m*2)+1] = color_on_ix;		// On State Color
						ctcan_tx[ctc_tpix].data[(m*2)+2] = color_off_ix;	    // Off State Color
	 					color_ix++;
 					}
 					ctcan_dev[devnmb].color_ix = color_ix;
					if (color_ix <= 11) 
					{
						ctcan_put_dev_msg_req (devnmb,58);
					}
					else
					{
						ctcan_dev[devnmb].color_ix = 0;
						ctcan_dev[devnmb].color_ix_req = 0;
					}
					ctcan_dev[devnmb].tx_cnt++;
				}
			}
			break;

		case 59:		// Dead Zone Parameter
/*
			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
			{
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 3;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit 
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[devnmb];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = (uint8)fvars[fvuldeadzone];
				ctcan_tx[ctc_tpix].data[1] = (uint8)spb_fvars[spb_setup_mode];
				ctcan_tx[ctc_tpix].data[2] = (uint8)fvars[fvdldeadzone];	 		

				ctcan_tx[ctc_tpix].data[3] = 0;	 
				ctcan_tx[ctc_tpix].data[4] = 0;	 
				ctcan_tx[ctc_tpix].data[5] = 0;
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				ctcan_dev[devnmb].tx_cnt++;
			}
*/
			break;
			
		case 100:		// Destination Car Call display data
			if (devnmb == c_ctcan_PI_LANT)
			{
				pi_location = 0;
				ctcan_tx[ctc_tpix].rtr = 0;
				ctcan_tx[ctc_tpix].srr = 1;
				ctcan_tx[ctc_tpix].len = 6;
				ctcan_tx[ctc_tpix].ex_id = 1;		// Extended Id
				ctcan_tx[ctc_tpix].one_tx = 0;	   // ok to retransmit 
				ctcan_tx[ctc_tpix].msg = msg;
				ctcan_tx[ctc_tpix].addr = (uint8)ctcan_addr[c_ctcan_PI_LANT];
				ctcan_tx[ctc_tpix].id2 = 0;
				ctcan_tx[ctc_tpix].id3 = 0;
				ctcan_tx[ctc_tpix].data[0] = pi_cc_command;
				ctcan_tx[ctc_tpix].data[1] = pi_cc_floor;
				if ((pi_cc_command & 0x20) != 0)
				{						   // Extra ascii used for rear floor
					ctcan_tx[ctc_tpix].data[2] = 'R';
					ctcan_tx[ctc_tpix].data[3] = flmrk[pi_cc_floor][0];
					ctcan_tx[ctc_tpix].data[4] = flmrk[pi_cc_floor][1];
				}
				else
				{
					ctcan_tx[ctc_tpix].data[2] = flmrk[pi_cc_floor][0];
					ctcan_tx[ctc_tpix].data[3] = flmrk[pi_cc_floor][1];
					ctcan_tx[ctc_tpix].data[4] = ' ';	// send a space if using two ascii floor numbers
				}
							
				ctcan_tx[ctc_tpix].data[5] = pi_location;	 
				ctcan_tx[ctc_tpix].data[6] = 0;
				ctcan_tx[ctc_tpix].data[7] = 0;
				pi_cc_ctcan_cnt ++;;
				ctcan_dev[devnmb].tx_cnt++;

			}
			break;
			
   		default:
			break;
	}
}

//************************************************************
//*********  Transmit Can Message  ***************************
//************************************************************

void tx_ctcan_msg(int16 TxBufIx)
{
	
	_disable();
	if (ctc_tgix != ctc_tpix)
	{
		
		switch (TxBufIx)
		{
			
			case 8:
				if (((CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (ctcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (CT_CAN.IFLAG1.B.BUF08I == 1)
						CT_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
					transmit(TxBufIx);
					CT_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
					tx_B08_fail_cnt = 0;
					break;
				}
				else
				{
					tx_B08_fail_cnt++;
					if (tx_B08_fail_cnt > 5)
					{
						ctcan_tx_complete[TxBufIx] = 1;
						tx_B08_fail_cnt = 0;
						CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
					 
			//lint -fallthrough
			case 9:
				if (((CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (ctcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (CT_CAN.IFLAG1.B.BUF09I == 1)
						CT_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
					transmit(TxBufIx);
					CT_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
					tx_B09_fail_cnt = 0;
					break;
				}
				else
				{
					tx_B09_fail_cnt++;
					if (tx_B09_fail_cnt > 5)
					{
						ctcan_tx_complete[TxBufIx] = 1;
						tx_B09_fail_cnt = 0;
						CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			//lint -fallthrough
			case 10:
				if (((CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (ctcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (CT_CAN.IFLAG1.B.BUF10I == 1)
						CT_CAN.IFLAG1.B.BUF10I = 1;		// Reset IF
					transmit(TxBufIx);
					CT_CAN.IFLAG1.B.BUF10I = 1;		// Reset IF
					tx_B10_fail_cnt = 0;
					break;
				}
				else
				{
					tx_B10_fail_cnt++;
					if (tx_B10_fail_cnt > 5)
					{
						ctcan_tx_complete[TxBufIx] = 1;
						tx_B10_fail_cnt = 0;
						CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			//lint -fallthrough
			case 11:
				if (((CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (ctcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (CT_CAN.IFLAG1.B.BUF11I == 1)
						CT_CAN.IFLAG1.B.BUF11I = 1;		// Reset IF
					transmit(TxBufIx);
					CT_CAN.IFLAG1.B.BUF11I = 1;		// Reset IF
					tx_B11_fail_cnt = 0;
					break;
				}
				else
				{
					tx_B11_fail_cnt++;
					if (tx_B11_fail_cnt > 5)
					{
						ctcan_tx_complete[TxBufIx] = 1;
						tx_B11_fail_cnt = 0;
						CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
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

void transmit (int16 TxBufIx)
{
	int16 i;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
	if (ctc_tgix != ctc_tpix)
	{		// There is data in the buffer 

		ctcan_tx_complete[TxBufIx] = 0;
		
		if (ctc_tgix < c_tx_buf_siz)
			ctc_tgix++;
		else
			ctc_tgix = 0;

		if (ctcan_tx[ctc_tgix].len > 8)
		{
			ctcan_tx[ctc_tgix].len = 8;
			ctcan_error |= 0x40;
		}
			
		if (ctcan_tx[ctc_tgix].ex_id == 1)
		{
			uiu.b[hb] = ctcan_tx[ctc_tgix].msg;		// Message
			uiu.b[mhb] = ctcan_tx[ctc_tgix].addr;	// Address
			uiu.b[mlb] = 0;
			uiu.b[lb] = 0;
		 	CT_CAN.MB[TxBufIx].MSG.MID.R = uiu.dw >> 3;		// priority bits get set to zero on shift
		}
		else
		{
			uiu.b[hb] = (uint8)(ctcan_tx[ctc_tgix].msg & 0x3F);		// Message
			uiu.b[mhb] = (uint8)(ctcan_tx[ctc_tgix].addr << 3);	// Address
			uiu.b[mlb] = 0;
			uiu.b[lb] = 0;
		 	CT_CAN.MB[TxBufIx].MSG.MID.R = uiu.dw >> 3;		// priority bits get set to zero on shift
		}


		i = 0;
		while ((i<ctcan_tx[ctc_tgix].len) && (i < 8))
		{
		 	CT_CAN.MB[TxBufIx].MSG.MDATA.B[i++] = ctcan_tx[ctc_tgix].data[i];
		}
		

		CT_CAN.MB[TxBufIx].MSG.MCS.B.LENGTH = ctcan_tx[ctc_tgix].len;
	 	
		if (ctcan_tx[ctc_tgix].rtr == 1)
			CT_CAN.MB[TxBufIx].MSG.MCS.B.RTR = 1;
	 	else
			CT_CAN.MB[TxBufIx].MSG.MCS.B.RTR = 0;

		if (ctcan_tx[ctc_tgix].ex_id == 1)
			CT_CAN.MB[TxBufIx].MSG.MCS.B.IDE = 1;
	 	else
			CT_CAN.MB[TxBufIx].MSG.MCS.B.IDE = 0;

		if (ctcan_tx[ctc_tgix].srr == 1)
			CT_CAN.MB[TxBufIx].MSG.MCS.B.SRR = 1;
	 	else
			CT_CAN.MB[TxBufIx].MSG.MCS.B.SRR = 0;
	 	
	 	
		CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x0C;		// Initiate transmit
		ctcan_debug_1 = CT_CAN.MB[TxBufIx].MSG.MCS.B.CODE;
	}
}

//************************************************************
//*********  Process the Can Serial Data *********************
//************************************************************

void ctcan_control (void)
{
	static int16 ix;
	int16 i;
	unsigned char msg_req = 0;

	unsigned char devix,iodevix,devnmb,tdevix;
	uint16 iodataix;

	if (cons[RGB_CC] == 1)
		cc_sec_att_sync();

	if (nmb_ctcan_dev > 0)
	{
		unpack_ctcan_in();

//		if (re_init_ctcan == 1)
//		{
//			re_init_ctcan = 0;
//			record_fault(f_can_init_error);
//			init_ctcan_bus();
//		}

		ctcan_status = CT_CAN.ESR.R;
		if (CT_CAN.ESR.B.ERRINT == 1)
		{
			ctcan_status = CT_CAN.ESR.R;
			CT_CAN.ESR.B.ERRINT = 1;
		}
		else
			ctcan_status = 0;
		
		ctcan_tx_err_cnt = (uint8)CT_CAN.ECR.B.TXECNT;
		ctcan_rx_err_cnt = (uint8)CT_CAN.ECR.B.RXECNT;

		if (CT_CAN.ESR.B.BOFFINT == 1)
		{		// The CAN bus is off
			CT_CAN.ESR.B.BOFFINT = 1;
			bus_off_cnt++;
			ctcan_error |= 0x04;
			init_ctcan_bus();
			record_fault(f_can_bus_off);
		}
		else if (bus_off_cnt > 0)
			bus_off_cnt--;

#if (Simulator == 0)
		if ((ctcan_status & 0x00000080) != 0)
		{		// Bus is idle
			bus_idle_cnt++;
//			ctcan_error |= 0x08;
//			record_fault(f_ctcan_bus_idle);		// bus can go idle while car sitting at the floor
		}
		else if (bus_idle_cnt > 0)
			bus_idle_cnt--;

		if ((ctcan_status & 0x00002000) != 0)
		{		// Transmit acknowledge error
		
			ack_error_cnt++;
			ctcan_error |= 0x10;
//			record_fault(f_ctcan_ack_err);	
		}
		else if (ack_error_cnt > 0)
			ack_error_cnt--;
#endif

		if (IsTxBufFul() == 0)
		{	// buffer is not full	  OK to write to buffer
/*			if (((rdinp(i_ATT) == 1) && (cons[att] == 1)) || (cons[att] == 2) ||
				((cons[Car_Sw] & 1) != 0))
			{	// has annunciator board
				update_att_hc_ann();
#if (Tract_HR == 1)
				if (cons[grtopf] <= 32)
				{			// less than 32 floors
					if (cons[rear] == 0)
					{		  // no rear doors
						if (timers[tatthc] > 8)
						{
							timers[tatthc] = 0;
							att_toggle = 0;
							update_att_fhc = 1;
						}
					}
					else  // less than 32 floor and rear doors
					{
						if (timers[tatthc] > 4)
						{
							timers[tatthc] = 0;
							if (att_toggle > 1)
								att_toggle = 0;
							if (att_toggle == 0)
								update_att_fhc = 1;
							else
								update_att_rhc = 1;
							att_toggle++;
						}
					}
				}
				else 
				{		// else greater than 32 floors

					if (cons[rear] == 0)
					{		// no rear floors
						if (timers[tatthc] > 4)
						{
							timers[tatthc] = 0;
							if (att_toggle > 1)
								att_toggle = 0;
							if (att_toggle == 0)
								update_att_fhc = 1;
							else
								update_att_fhc_hr = 1;
							att_toggle++;
						}
					}
					else
					{		// greater than 32 floors with rear
						if (timers[tatthc] > 3)
						{
							timers[tatthc] = 0;
							if (att_toggle > 3)
								att_toggle = 0;
							if (att_toggle == 0)
								update_att_fhc = 1;
							else if (att_toggle == 1)
								update_att_fhc_hr = 1;
							else if (att_toggle == 2)
								update_att_rhc = 1;
							else if (att_toggle == 3)
								update_att_rhc_hr = 1;
							att_toggle++;
						}
					}
				}
#else
				if (cons[rear] == 0)
				{		  // no rear doors
					if (timers[tatthc] > 8)
					{
						timers[tatthc] = 0;
						att_toggle = 0;
						update_att_fhc = 1;
					}
				}
				else  // rear doors
				{
					if (timers[tatthc] > 4)
					{
						timers[tatthc] = 0;
						if (att_toggle > 1)
							att_toggle = 0;
						if (att_toggle == 0)
							update_att_fhc = 1;
						else
							update_att_rhc = 1;
						att_toggle++;
					}
				}
#endif												
				if (update_att_fhc == 1)
				{
				    update_att_fhc = 0;	
					if (cons[RGB_CC] == 1)
					{
						update_att_fcc_18 = 1;
						update_att_fcc_5 = 1;
					}
					pack_ctcan_out(26,c_ctcan_PI_LANT,0);
				}
#if (Tract_HR == 1)
				if (update_att_fhc_hr == 1)
				{
					update_att_fhc_hr = 0;
					if (cons[RGB_CC] == 1)
					{
						update_att_fcc_5 = 1;
						update_att_fcc_11 = 1;
					}
					pack_ctcan_out(50,c_ctcan_PI_LANT,0);
				}
#endif

				if (update_att_rhc == 1)
				{
					update_att_rhc = 0;
					if (cons[RGB_CC] == 1)
					{
						update_att_rcc_6 = 1;
						update_att_rcc_7 = 1;
					}
					pack_ctcan_out(27,c_ctcan_PI_LANT,0);
				}
#if (Tract_HR == 1)
				if (update_att_rhc_hr == 1)
				{
					update_att_rhc_hr = 0;
					if (cons[RGB_CC] == 1)
					{
						update_att_rcc_7 = 1;
						update_att_rcc_12 = 1;
					}
					pack_ctcan_out(51,c_ctcan_PI_LANT,0);
				}
#endif					
			}
			*/
			
			set_pi_lant_status();
			upd_pi_lant_mrcan = update_pi_lant;
			set_pi_cc_bits();
			set_dd_cc_pi_dpy();
			
			if (((timers[tpilant_ctcan] > 2) || (update_pi_lant == 1)) && (data_req == 0))	// 300 msec
			{
				data_req = 1;
				update_pi_lant = 0;
				pack_ctcan_out(25,c_ctcan_PI_LANT,0);		// Pack the pi and lantern packet 
				timers[tpilant_ctcan] = 0;
			}
			if ((timers[tpicc_ctcan] > 2) && (pi_cc_update == 1) && (data_req == 0))	// 300 msec
			{
				if (pi_cc_ctcan_cnt < 2)
				{
					data_req = 1;
					pack_ctcan_out(100,c_ctcan_PI_LANT,0);		// Pack the pi and lantern packet 
					timers[tpicc_ctcan] = 0;
				}
				else
				{
					pi_cc_ctcan_cnt = 0;
					pi_cc_update = 0;
				}
			}
		}

		if (IsTxBufFul() == 0)
		{	// buffer is not full	  OK to write to buffer
			if ((ctcan_msg_req != 0) && (ctcan_dev_req != 0) && (ctcan_msg_cmd != 0) && (data_req == 0))
			{
				data_req = 1;
				if ((((ctcan_dev_req >= c_ctcan_START) && (ctcan_dev_req <= c_ctcan_END)) || (ctcan_dev_req == c_ctcan_GLOBAL)) && (ctcan_msg_cmd <= 2))
				{
					pack_ctcan_out((uint8)ctcan_msg_req,(uint8)ctcan_dev_req,(uint8)ctcan_msg_cmd-1);		// Pack Message request 
				}
				if (ctcan_msg_cnt > 0)
					ctcan_msg_cnt--;
				if (ctcan_msg_cnt == 0)
				{
					ctcan_msg_req = 0;
					ctcan_dev_req = 0;
					ctcan_msg_cmd = 0;
				}
			}
		}	 
			
		// Send out can sync timer
		if (cons[RGB_CC] == 1)
		{
			if (IsTxBufFul() == 0)
			{	// buffer is not full	  OK to write to buffer
				if (can_sync_timer >= 25)
				{
					can_sync_timer = 0;
					pack_ctcan_out(52,c_ctcan_GLOBAL,0);
				}
			}
		}

		// Communicate with Serial Devices and Expansion I/O boards
		i = 0;
		while ((i < nmb_ctcan_dev) && (IsTxBufFul() == 0))
		{
			if (ix < (nmb_ctcan_dev-1))
				ix++;
			else
				ix = 0;

			devnmb = ctcan_dev_list[ix];	// get device number from list
			devix = devnmb;


			// Communicate with Selector Device
			if (cons[sel_type] == 2)
			{
				if (devnmb == c_ctcan_SEL)
				{
					msg_req = ctcan_get_dev_msg_req(devnmb);
					if (msg_req != 0)
					{
						pack_ctcan_out(msg_req,c_ctcan_SEL,0);
					}
					else if (SEL_can.reflect_data == 1)
					{
						SEL_can.reflect_data = 0;
						pack_ctcan_out(35,c_ctcan_SEL,0);	// Request a data packet	
					}
					else if (can_sel_timer >= 100)
					{						   // every 1000 msec
						if (data_req == 0) 
						{
							data_req = 1;
							if (ctcan_dev[devix].version == 0)
							{
								can_sel_timer = 0;
								pack_ctcan_out(1,c_ctcan_SEL,0);	// send init packet
	 						}
	 						else 
	 						{
								can_sel_timer = 0;
								pack_ctcan_out(34,c_ctcan_SEL,1);	// Request a data packet	
							}

							if (can_sel_timer  == 0)
							{
								if (ctcan_dev[devix].online_cnt > 0)
								{
									sel_can_error = 0;
									sel_can_online = 1;
									statusf &= ~sf_SELCAN;		// SEL can comm error cleared
									ctcan_dev[devix].online = 1;
								}
								else
								{
#if (Simulator == 0)
									sel_can_error = 1;		// prevent car from running
									statusf |= sf_SELCAN;		// SEL can comm error
									if (ctcan_dev[devix].online == 1)
										record_fault(f_sel_can_error);
#endif
									sel_can_online = 0;
									ctcan_dev[devix].online = 0;
								}
							}
						}
					}
				}
			}
			else
			{
				sel_can_online = 0;
				sel_can_error = 0;
			}
			
			if (cons[loadw] == 2)
			{	// Request load weigher data when at floor and door is open
				if (devnmb == c_ctcan_LW)
				{
					msg_req = ctcan_get_dev_msg_req(devnmb);
					if (msg_req != 0)
					{
						pack_ctcan_out(msg_req,c_ctcan_LW,0);
						timers[tlwupd] = 0;
					}
					else if (((timers[tlwupd] >= 2) && (procf >= 14) && (procf <= 16)) ||	// 200 msec at the floor
							 ((timers[tlwupd] >= 5) && ((procf < 14) || (procf > 16))))		// Every 500 msec not at floor
					{		
						if (data_req == 0) 
						{
							data_req = 1;
							timers[tlwupd] = 0;

							if (ctcan_dev[devix].version == 0)
								pack_ctcan_out(1,c_ctcan_LW,0);	// send init packet
							else
								pack_ctcan_out(21,c_ctcan_LW,0);	// request data read from load weigher					

							if (ctcan_dev[devix].online_cnt > 0)
								ctcan_dev[devix].online_cnt--;
							if (ctcan_dev[devix].online_cnt > 0)
							{
								lw_can_online = 1;
								ctcan_dev[devix].online = 1;
							}
							else
							{
								lw_can_online = 0;
								ctcan_dev[devix].online = 0;
							}
						}
					}
				}
			}
			else if (cons[loadw] != 3)
				lw_can_online = 0;
				
			if ((cons[sercom] & 0x10) != 0)
			{	// Serial Door operators
				if (devnmb == c_ctcan_DOOR)
				{
					set_door_command();
					msg_req = ctcan_get_dev_msg_req(devnmb);
					if (msg_req != 0)
					{
						pack_ctcan_out(msg_req,c_ctcan_DOOR,0);
						timers[tdoorupd] = 0;
					}
					else if ((DOOR_can.command  != DOOR_can.prev_command) || (DOOR_can.reflect_data == 1))
					{
						DOOR_can.reflect_data = 0;
						pack_ctcan_out(28,c_ctcan_DOOR,0);	// send data to door
					}
					else if (timers[tdoorupd] >= 5)		// Every 500 msec
					{		
						if (data_req == 0) 
						{
							data_req = 1;

							if ((ctcan_dev[devix].version == 0) && (ctcan_dev[devix].revision == 0))
							{
								timers[tdoorupd] = 0;
								pack_ctcan_out(12,c_ctcan_DOOR,1);	// request version
							}
	 						else 
	 						{
									
								timers[tdoorupd] = 0;

								DOOR_can.dpy_req++;

								if (DOOR_can.dpy_req > 3)
									DOOR_can.dpy_req = 0;
								
								pack_ctcan_out(28,c_ctcan_DOOR,0);	// send door command					

							}

							if (timers[tdoorupd] == 0)
							{
								if (ctcan_dev[devix].online_cnt > 0)
									ctcan_dev[devix].online_cnt--;
								if (ctcan_dev[devix].online_cnt > 0)
									ctcan_dev[devix].online = 1;
								else
								{
#if (Simulator == 1)
									DOOR_can.command_ret = DOOR_can.command;
#endif
									ctcan_dev[devix].online = 0;
								}
							}
						}
					}
					else if ((DOOR_can.command != DOOR_can.command_ret) && (ctcan_dev[devix].online == 1))
					{
						if ((timers[tdoorupd] & 0x01) == 0x01)		   // Every 100 msec
						{
						 	pack_ctcan_out(28,c_ctcan_DOOR,0);	// send data to door
						}
					}
				}
				if ((devnmb == c_ctcan_RDOOR) && (cons[rear] != 0))
				{
					set_rdoor_command();
					msg_req = ctcan_get_dev_msg_req(devnmb);
					if (msg_req != 0)
					{
						pack_ctcan_out(msg_req,c_ctcan_RDOOR,0);
						timers[trdoorupd] = 0;
					}
					else if ((RDOOR_can.command  != RDOOR_can.prev_command) || (RDOOR_can.reflect_data == 1))
					{
						RDOOR_can.reflect_data = 0;
						pack_ctcan_out(28,c_ctcan_RDOOR,0);	// send data to door
					}
					else if (timers[trdoorupd] >= 5)		// Every 500 msec
					{		
						if (data_req == 0)
						{
							data_req = 1;
							if ((ctcan_dev[devix].version == 0) && (ctcan_dev[devix].revision == 0))
							{
								timers[trdoorupd] = 0;
								pack_ctcan_out(12,c_ctcan_RDOOR,1);	// request version
	 						}
	 						else  
	 						{

								timers[trdoorupd] = 0;

								RDOOR_can.dpy_req++;

								if (RDOOR_can.dpy_req > 3)
									RDOOR_can.dpy_req = 0;
								
								pack_ctcan_out(28,c_ctcan_RDOOR,0);	// send door command					
							}
							if (timers[trdoorupd] == 0)
							{
								if (ctcan_dev[devix].online_cnt > 0)
									ctcan_dev[devix].online_cnt--;
								if (ctcan_dev[devix].online_cnt > 0)
									ctcan_dev[devix].online = 1;
								else
								{
#if (Simulator == 1)
									RDOOR_can.command_ret = RDOOR_can.command;
#endif
									ctcan_dev[devix].online = 0;
								}
							}
						}
					}
					else if (RDOOR_can.command  != RDOOR_can.command_ret)
					{
						if ((timers[trdoorupd] & 0x01) == 0)		// Every 100 msec 
							pack_ctcan_out(28,c_ctcan_RDOOR,0);	// send data to door
					}
				}
			}

			if (cons[Voltage_Sensor] == 1)
			{
				if ((timers[tvsupd] >= 30) && (devnmb == c_ctcan_VS)) // every 3 seconds
				{	  // Request Voltage from voltage board
					if (data_req == 0)
					{
						data_req = 1;
						msg_req = ctcan_get_dev_msg_req(devnmb);
						if (msg_req != 0)
						{
							pack_ctcan_out(msg_req,c_ctcan_VS,0);
						}
						if (ctcan_dev[devix].version == 0)
							pack_ctcan_out(1,c_ctcan_VS,0);	// send init packet
						else
							pack_ctcan_out(44,c_ctcan_VS,1);	// data request controller voltage
						timers[tvsupd] = 0;;

#if (Simulator == 1)
						ctcan_dev[devix].online = 1;
						vs_can_online = 1;
#else
						if (ctcan_dev[devix].online_cnt > 0)
							ctcan_dev[devix].online_cnt--;
						if (ctcan_dev[devix].online_cnt > 0)
						{
							vs_can_online = 1;
							ctcan_dev[devix].online = 1;
						}
						else
						{
							vs_can_online = 0;
							ctcan_dev[devix].online = 0;
						}
#endif
					}
				}
			}
			else if (cons[Voltage_Sensor] == 0)
				vs_can_online = 0;
			

			// I/O Board Data

			if ((devnmb >= c_ctcan_SEB_1) && (devnmb <= c_ctcan_SEB_40))
			{		 // Valid device number
				iodevix = devnmb-c_ctcan_SEB_1;
				tdevix = devnmb - c_ctcan_SEB_1;
				iodataix = (uint16)((IODATA_CTCAN_START + iodevix) * 3); 		// input array

				// *********************** Output device *********************
					 
				if (((iodata[cons[carnmb]][iodataix] & IO_In_Out[iodataix]) != (serial_ctc.dev[iodevix].iob[0] & IO_In_Out[iodataix])) ||
					((iodata[cons[carnmb]][iodataix+1] & IO_In_Out[iodataix+1]) != (serial_ctc.dev[iodevix].iob[1] & IO_In_Out[iodataix+1])) ||
					((iodata[cons[carnmb]][iodataix+2] & IO_In_Out[iodataix+2]) != (serial_ctc.dev[iodevix].iob[2] & IO_In_Out[iodataix+2]))) 
				{		// io data for the output has changed
					serial_ctc.dev[iodevix].iob[0] = (uint8)((serial_ctc.dev[iodevix].iob[0] & ~IO_In_Out[iodataix]) | (iodata[cons[carnmb]][iodataix] & IO_In_Out[iodataix]));
					serial_ctc.dev[iodevix].iob[1] = (uint8)((serial_ctc.dev[iodevix].iob[1] & ~IO_In_Out[iodataix+1]) | (iodata[cons[carnmb]][iodataix+1] & IO_In_Out[iodataix+1]));
					serial_ctc.dev[iodevix].iob[2] = (uint8)((serial_ctc.dev[iodevix].iob[2] & ~IO_In_Out[iodataix+2]) | (iodata[cons[carnmb]][iodataix+2] & IO_In_Out[iodataix+2]));
					pack_ctcan_out(13,devnmb,0);	// Data has changed so pack the data
					timers[tcantx + tdevix] = 0;
					sync_dev_ctc[iodevix].rtx_cnt = 0;
				}
				else if (((sync_dev_ctc[iodevix].iob[0] & IO_In_Out[iodataix]) != (serial_ctc.dev[iodevix].iob[0] & IO_In_Out[iodataix])) ||
					((sync_dev_ctc[iodevix].iob[1] & IO_In_Out[iodataix+1]) != (serial_ctc.dev[iodevix].iob[1] & IO_In_Out[iodataix+1])) ||
					((sync_dev_ctc[iodevix].iob[2] & IO_In_Out[iodataix+2]) != (serial_ctc.dev[iodevix].iob[2] & IO_In_Out[iodataix+2]))) 
				{		// device has not updated it's outputs yet
					if (timers[tcantx + tdevix] >= 2)
					{
						pack_ctcan_out(13,devnmb,0);	// Data has changed so pack the data
						timers[tcantx + tdevix] = 0;
						if (sync_dev_ctc[iodevix].rtx_cnt > 10)
						{
							sync_dev_ctc[iodevix].iob[0] = (uint8)((sync_dev_ctc[iodevix].iob[0] & ~IO_In_Out[iodataix]) | (serial_ctc.dev[iodevix].iob[0] & IO_In_Out[iodataix]));
							sync_dev_ctc[iodevix].iob[1] = (uint8)((sync_dev_ctc[iodevix].iob[1] & ~IO_In_Out[iodataix+1]) | (serial_ctc.dev[iodevix].iob[1] & IO_In_Out[iodataix+1]));
							sync_dev_ctc[iodevix].iob[2] = (uint8)((sync_dev_ctc[iodevix].iob[2] & ~IO_In_Out[iodataix+2]) | (serial_ctc.dev[iodevix].iob[2] & IO_In_Out[iodataix+2]));
							sync_dev_ctc[iodevix].rtx_cnt = 0;
						}
						else
							sync_dev_ctc[iodevix].rtx_cnt++;
					}
				}
				else if ((timers[tcantx + tdevix] > 6) && (data_req == 0))
				{		 // Update outputs
					data_req = 1;
					serial_ctc.dev[iodevix].iob[0] = (uint8)((serial_ctc.dev[iodevix].iob[0] & ~IO_In_Out[iodataix]) | (iodata[cons[carnmb]][iodataix] & IO_In_Out[iodataix]));
					serial_ctc.dev[iodevix].iob[1] = (uint8)((serial_ctc.dev[iodevix].iob[1] & ~IO_In_Out[iodataix+1]) | (iodata[cons[carnmb]][iodataix+1] & IO_In_Out[iodataix+1]));
					serial_ctc.dev[iodevix].iob[2] = (uint8)((serial_ctc.dev[iodevix].iob[2] & ~IO_In_Out[iodataix+2]) | (iodata[cons[carnmb]][iodataix+2] & IO_In_Out[iodataix+2]));
					pack_ctcan_out(13,devnmb,0);	// Data has changed so pack the data
					timers[tcantx + tdevix] = 0;
					sync_dev_ctc[iodevix].rtx_cnt = 0;
				}
				else
				{
					
/*					if (cons[RGB_CC] == 1)
					{
						update_cc_sec();
						if ((cc_sec_flag_18 == 1) && (devnmb == c_ctcan_SEB_18))
						{
							cc_sec_flag_18 = 0;
							pack_ctcan_out(55,devnmb,0);
						}
						else if ((cc_sec_flag_5 == 1) && (devnmb == c_ctcan_SEB_5))
						{
							cc_sec_flag_5 = 0;
							pack_ctcan_out(55,devnmb,0);
						}
						else if ((cc_sec_flag_11 == 1) && (devnmb == c_ctcan_SEB_11))
						{
							cc_sec_flag_11 = 0;
							pack_ctcan_out(55,devnmb,0);
						}
						
						if ((update_att_fcc_18 == 1) && (devnmb == c_ctcan_SEB_18)) 
						{
							update_att_fcc_18 = 0;
							pack_ctcan_out(53,devnmb,0);
						}									
						else if ((update_att_fcc_5 == 1) && (devnmb == c_ctcan_SEB_5)) 
						{
							update_att_fcc_5 = 0;
							pack_ctcan_out(53,devnmb,0);
						}									
						else if ((update_att_fcc_11 == 1) && (devnmb == c_ctcan_SEB_11)) 
						{
							update_att_fcc_11 = 0;
							pack_ctcan_out(53,devnmb,0);
						}	
						
						if (cons[rear] != 0)
						{								
							if ((rcc_sec_flag_6 == 1) && (devnmb == c_ctcan_SEB_6))
							{
								rcc_sec_flag_6 = 0;
								pack_ctcan_out(55,devnmb,0);
							}
							else if ((rcc_sec_flag_7 == 1) && (devnmb == c_ctcan_SEB_7))
							{
								rcc_sec_flag_7 = 0;
								pack_ctcan_out(55,devnmb,0);
							}
							else if ((rcc_sec_flag_12 == 1) && (devnmb == c_ctcan_SEB_12))
							{
								rcc_sec_flag_12 = 0;
								pack_ctcan_out(55,devnmb,0);
							}

							if ((update_att_rcc_6 == 1) && (devnmb == c_ctcan_SEB_6)) 
							{
								update_att_rcc_6 = 0;
								pack_ctcan_out(53,devnmb,0);
							}									
							else if ((update_att_rcc_7 == 1) && (devnmb == c_ctcan_SEB_7)) 
							{
								update_att_rcc_7 = 0;
								pack_ctcan_out(53,devnmb,0);
							}									
							else if ((update_att_rcc_12 == 1) && (devnmb == c_ctcan_SEB_12)) 
							{
								update_att_rcc_12 = 0;
								pack_ctcan_out(53,devnmb,0);
							}									
						}
					}
*/
				}

					
				// *****************  Input check **********************

				if (sync_dev_ctc[iodevix].reflect_data == 1)
				{		 // Send back what was sent so the device can verify it was sent ok
					sync_dev_ctc[iodevix].reflect_data = 0;	// Must be cleared before data is packed - if new data comes in during
					ctcan_debug_3++;
					pack_ctcan_out(15,devnmb,0);			//  the pack, it must be re-sent
				}
				else 
				{
					msg_req = ctcan_get_dev_msg_req(devnmb);
					if (msg_req != 0)
					{
						pack_ctcan_out(msg_req,devnmb,0);
					}
					else if (timers[tcanrx + tdevix] > 3)
					{
						if (data_req == 0)
						{
							data_req = 1;
							if (ctcan_dev[devix].version == 0)
							{
								pack_ctcan_out(11,devnmb,0);	// send init packet
								timers[tcanrx + tdevix] = 0;
								if (ctcan_dev[devix].online_cnt > 0)
									ctcan_dev[devix].online_cnt--;
							}
							else 
							{
								pack_ctcan_out(14,devnmb,1);	// data request
								timers[tcanrx + tdevix] = 0;
								if (ctcan_dev[devix].online_cnt > 0)
									ctcan_dev[devix].online_cnt--;
							}
						}
					}
				}
					
				if (ctcan_dev[devix].online_cnt > 0)
				{
					ctcan_dev[devix].online = 1;
					if (devnmb == c_ctcan_SEB_1)
					{
						cop_can_online = 1;
						cop_can_error = 0;
						statusf2 &= ~sf_COPCAN;		// COP can comm error
					}
					else if ((devnmb == c_ctcan_SEB_3) && (ctcan_dev[c_ctcan_SEB_2].online == 1))
					{
						sel_can_online = 1;
						sel_can_error = 0;
						statusf &= ~sf_SELCAN;		// SEL can comm error
					}
				}
				else
				{

#if (Simulator == 0)
					if (ctcan_dev[devix].online == 1)
					{
						fault_device = ctcan_addr[devnmb];
//						record_fault(f_seb_ctcan_error);
						
					}
					if (devnmb == c_ctcan_SEB_1)
					{
						cop_can_online = 0;
						cop_can_error = 1;
						statusf2 |= sf_COPCAN;		// COP can comm error
						if (ctcan_dev[devix].online == 1)
							record_fault(f_cop_can_error);
					}
					else if ((devnmb == c_ctcan_SEB_2) || (devnmb == c_ctcan_SEB_3))
					{
						sel_can_online = 0;
						sel_can_error = 1;
						statusf |= sf_SELCAN;		// SEL can comm error
						if (ctcan_dev[devix].online == 1)
							record_fault(f_sel_can_error);
					}
#endif
					ctcan_dev[devix].online = 0;
#if (Simulator == 0)
		 			serial_ctc.dev[iodevix].iob[0] &= IO_In_Out[iodataix];	  // clear input data since device is not on-line
		 			serial_ctc.dev[iodevix].iob[1] &= IO_In_Out[iodataix+1];
		 			serial_ctc.dev[iodevix].iob[2] &= IO_In_Out[iodataix+2];
					if (devnmb == c_ctcan_DCL_DOL)
					{
						clrinp(i_DCL);
						clrinp(i_DOL);
					}
					if ((cons[rear] != 0) && (devnmb == c_ctcan_DCLR_DOLR)) 
					{			 // serial to the cop and rear doors
						clrinp(i_DCLR);
						clrinp(i_DOLR);
					}
#endif
				}
			}
			i++;		
		}
		CtcanTxBufIx ++;
		if (CtcanTxBufIx > 11)
			CtcanTxBufIx = 8;
		tx_ctcan_msg(CtcanTxBufIx);		// Check if data waiting and transmit
		if (data_req == 0)
			data_req_cnt = 0;
		else if (data_req_cnt >= 1)
		{						// Cause a delay between sending packet request instead of sending all at once
			data_req_cnt = 0;
			data_req = 0;
		}
		else
			data_req_cnt++;
	}
}


//************************************************************
//*********  Set the PI and Lantern Status Flags *************
//************************************************************


void set_pi_lant_status (void)
{
/*			Packet 25 for PI, Lantern and Speech devices.

	25		PI			CPU				8	   	Message #25: PI and Lantern Data
												  	Car position		Binary position
												  	Ascii PI 0			Ascii Position low byte
												  	Ascii PI 1			Ascii Position High byte

													Lantern Status		Bit 7: LUP (lantern up)
																		Bit 6: LDN (lantern down)
																		Bit 5: RLUP (rear lantern up)
																		Bit 4: RLDN (rear lantern down)
																		Bit 3: UDA (up direction arrow)
																		Bit 2: DDA (down direction arrow)
																		Bit 1: User Message Bit 1
																		Bit 0: User Message Bit 0

												  	Motion Status		Bit 7: CULR previously UPREF (up preference)
												  						Bit 6: CDLR previously DNPREF (down preference)
												  						Bit 5: CUL previously run (car running)
												  						Bit 4: CDL previously leveling (car leveling)
												  						Bit 3: Out of Service
																		Bit 2: Inspection
																		Bit 1: Independent
																		Bit 0: unused

													Fire Status			Bit 7: PH2 (fire phase 2)
																		Bit 6: PH1 Complete (fire phase 1 complete)
																		Bit 5: PH1 Recall Main (fire phase 1 recall active)
																		Bit 4: EMP Recall (emergency power recall active)
												  						Bit 3: EMP Recall Complete (emergency power recall complete)
												  						Bit 2: Power Loss (Normal power is out)
												  						Bit 1: Imminent Shutdown
																		Bit 0: PH1 Recall Alternate (fire phase 1 recall active)

													Door Status 		Bits 3,2,1,0
												  						doorf = 0 doors closed no action
												  						doorf = 1 doors are opening
												  						doorf = 2 doors are dwellingse)
																		doorf = 3 doors are closing
																		doorf = 4 doors are nudging closed

																		Bits 7,6,5,4
													  					rdoorf = 0 doors closed no action
													  					rdoorf = 1 doors are opening
													  					rdoorf = 2 doors are dwelling
													  					rdoorf = 3 doors are closing
													  					rdoorf = 4 doors are nudging closed

													Misc Status			Bit 7: Activate speech
																		Bit 6: At Lobby (car at lobby floor)
																		Bit 5: Overload (load overload)
																		Bit 4: Load Bypass
																		Bit 3: EMT (Emergency Medical Technician Service)
																		Bit 2: Code Blue
																		Bit 1: Seismic
																		Bit 0: User Message Bit 2 

																		User Message Bits 2,1,0
																						  0 0 0	No message
																						  0 0 1 Message 1
																						  0 1 0 Message 2
																						  0 1 1 Message 3
																						  1 0 0 Message 4
																						  1 0 1 Message 5
																						  1 1 0 Message 6
																						  1 1 1 Message 7
*/



	if (position != pi_pos)
		update_pi_lant = 1;
	if ((valid_fl[position] != 0) || (fvars[fvhbz_nvfl] == 0))
		pi_pos = (uint8)position;				// update only when valid posiuwu.won

	lant_stat = 0;
	if (lant_up == 1)
		lant_stat |= 0x80;
	if (lant_dn == 1)
		lant_stat |= 0x40;
	if (rlant_up == 1)
		lant_stat |= 0x20;
	if (rlant_dn == 1)
		lant_stat |= 0x10;
	if (up_dir_arrow == 1)
		lant_stat |= 0x08;
	if (dn_dir_arrow == 1)
		lant_stat |= 0x04;

	if ((cons[loadw] != 1)&&(cons[housing] == 3))
	{
		if ((rd_both_inps(i_ALRM,i_XALRM) == 1) || ((cons[rear] != 0) && (rd_both_inps(i_ALRMR,i_XALRMR) == 1)))
		{
			alrm_pi_stat = 1;
			timers[talrm_pi] = 0;
		}
		else if(timers[talrm_pi] > 200)				  
		{
			alrm_pi_stat = 0;
		}
	}


	if ((fvars[fvsvcmsg1] != 0) && (servf == fvars[fvsvcmsg1])) 
		lant_stat |= 0x01;
	else if ((fvars[fvsvcmsg2] != 0) && (servf == fvars[fvsvcmsg2]))
		lant_stat |= 0x02;
	else if ((fvars[fvextdoormsg] == 1) && (extended_door_time == 1))		
		lant_stat |= 0x02;
	else if ((fvars[fvsvcmsg3] != 0) && (servf == fvars[fvsvcmsg3]))
		lant_stat |= 0x03;

	if (pi_lant_stat != lant_stat)
		update_pi_lant = 1;

	pi_lant_stat = lant_stat;

	door_stat = 0;
	if (rdoutp(o_NUD) == 1)
		door_stat = 4;
	else if (rdoutp(o_DO) == 1)
		door_stat = 1;
	else if (rdoutp(o_DC) == 1)
		door_stat = 3;
	else if (rdinp(i_DCL) == 0)
		door_stat = 0;
	else if (rdinp(i_DOL) == 0)
		door_stat = 2;

	if (rdoutp(o_NUDR) == 1)
		door_stat |= 0x40;
	else if (rdoutp(o_DOR) == 1)
		door_stat |= 0x10;
	else if (rdoutp(o_DCR) == 1)
		door_stat |= 0x30;
	else if (rdinp(i_DCLR) == 0)
		door_stat &= 0x0F;
	else if (rdinp(i_DOLR) == 0)
		door_stat |= 0x20;

	if (pi_door_stat != door_stat)
		update_pi_lant = 1;

	pi_door_stat = door_stat;

	fire_stat = 0;
	if (firef > 3)
		fire_stat |= 0x80;
	if ((firef == 1) || (firef == 2) || (rfiref == 1) || (rfiref == 2) || (firef == 3) || (rfiref == 3))
	{
		if ((firef == 3) || (rfiref == 3))
			fire_stat |= 0x40;
		if ((firef == 1) || (rfiref == 1) || (((firef == 3) || (rfiref == 3)) && (fire_floor == fvars[fvfs1f])))
			fire_stat = 0x20;
		if ((firef == 2) || (rfiref == 2) || (((firef == 3) || (rfiref == 3)) && (fire_floor == fvars[fvfs1af])))
			fire_stat = 0x01;
	}

	if (empf != 0)
	{
		fire_stat |= 0x04;
		if ((empf == 4) || (empf == 5) || (empf == 3))
		{
			fire_stat |= 0x10;				// Recall sequence
			if ((empf == 4) || (empf == 5))
				fire_stat |= 0x08;				// Recall complete
		}
	}


	// Fire stat bit 1 reserved for imminent shutdown

	if (pi_fire_stat != fire_stat)
		update_pi_lant = 1;

	pi_fire_stat = fire_stat;


	// Motion Status
	motion_stat = 0;

	if (cab_rlant_up == 1)
		motion_stat |= 0x80;
	if (cab_rlant_dn == 1)
		motion_stat |= 0x40;
	if (cab_lant_up == 1)
		motion_stat |= 0x20;
	if (cab_lant_dn == 1)
		motion_stat |= 0x10;
/*
	if (cab_lant_up == 1)
		motion_stat |= 0x80;
	if (cab_lant_dn == 1)
		motion_stat |= 0x40;
	if (cab_rlant_up == 1)
		motion_stat |= 0x20;
	if (cab_rlant_dn == 1)
		motion_stat |= 0x10;

	if (dpref == 1)
		motion_stat |= 0x80;
	else if (dpref == 2)
		motion_stat |= 0x40;

	if (rdoutp(o_RUN) == 1)
		motion_stat |= 0x20;

	if (relevel == 1)
		motion_stat |= 0x10;
*/

	if (servf < s_IND)
		motion_stat |= 0x08; 

	if ((chk_ins() == 1) && (safeties() == 1))
		motion_stat |= 0x04; 

	if (ind() == 1)
		motion_stat |= 0x02; 

	if (pi_motion_stat != motion_stat)
		update_pi_lant = 1;

	pi_motion_stat = motion_stat;

	
	// Misc. Status Flags
	misc_stat = 0;
//	if (position == lobby_fl)
//		misc_stat |= 0x40;

	
//change this statement
//	if (rdinp(i_OLV) == 1)
// to if(in_overload == 1)

	if(in_overload == 1)
		misc_stat |= 0x20;
	if (rdinp(i_LWB) == 1)
		misc_stat |= 0x10;	
	if ((medf == 1) || (medf == 2))
		misc_stat |= 0x08;
	if ((codebf == 1) || (codebf == 2) || (codebf == 3))
		misc_stat |= 0x04;
	if (eqf != 0)
		misc_stat |= 0x02;

	if (pi_misc_stat != misc_stat)
		update_pi_lant = 1;

	pi_misc_stat = misc_stat;
}

// Set Car Call Data for pi	
void set_pi_cc_bits(void)
{
	int16 i;
	int16 fl_ix;
	
	for (i=cons[bottomf];i<=cons[topf];i++)
	{
		fl_ix = (i-1)/32;
		if (carcb[i] == 1)
			pi_cc[fl_ix] |= fl_mask[i];
		else
			pi_cc[fl_ix] &= ~fl_mask[i];
		
		if (rcarcb[i] == 1)
			pi_rcc[fl_ix] |= fl_mask[i];
		else
			pi_rcc[fl_ix] &= ~fl_mask[i];
	}
	
}


// set flag to send cc data to dd pi 

void set_dd_cc_pi_dpy(void)
{
	int16 i;
	int16 fl_ix;
	
	
	if ((firef == 0) && (rfiref == 0) && (servf == s_AUTO))
	{  		// automatic service only
		
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			fl_ix = (i-1)/32;
			if ((pi_cc[fl_ix] & fl_mask[i]) != (ct_pi_cc[fl_ix] & fl_mask[i]))
			{
				if (pi_cc_update == 0)
				{
				 	pi_cc_floor = (uint8)i;
				 	if ((pi_cc[fl_ix] & fl_mask[i]) != 0)
						pi_cc_command = 0x01;	   			// set car call
				 	else
				 		pi_cc_command = 0x02;				// clear car call
					pi_cc_update = 1;
					ct_pi_cc[fl_ix] &= ~fl_mask[i];
					ct_pi_cc[fl_ix] |= pi_cc[fl_ix] & fl_mask[i];
					break;
				}
			}
			if ((pi_rcc[fl_ix] & fl_mask[i]) != (ct_pi_rcc[fl_ix] & fl_mask[i]))
			{
				if (pi_cc_update == 0)
				{
				 	pi_cc_floor = (uint8)i;
				 	if ((pi_rcc[fl_ix] & fl_mask[i]) != 0)
						pi_cc_command = 0x11;				// set rear car call
				 	else
				 		pi_cc_command = 0x12;				// clear rear car call
					pi_cc_update = 1;
					ct_pi_rcc[fl_ix] &= ~fl_mask[i];
					ct_pi_rcc[fl_ix] |= pi_rcc[fl_ix] & fl_mask[i];
					break;
				}
			}
		}
	}
}

//********************************************
// Car Call Security and Att Annunciator Sync
//********************************************

void cc_sec_att_sync (void)
{
	if (timers[tccsync] > 3)
	{
		if (return_master_cc_sec[0] != master_cc_sec_prev[0])
		{
			cc_sec_flag_18 = 1;
			cc_sec_flag_5 = 1;
		}
		if (return_master_cc_sec[1] != master_cc_sec_prev[1])
		{
			cc_sec_flag_5 = 1;
			cc_sec_flag_11 = 1;
		}
		if (cons[rear] != 0)
		{								
			if (return_master_rcc_sec[0] != master_rcc_sec_prev[0])
			{
				rcc_sec_flag_6 = 1;
				rcc_sec_flag_7 = 1;
			}
			if (return_master_rcc_sec[1] != master_rcc_sec_prev[1])
			{
				rcc_sec_flag_7 = 1;
				rcc_sec_flag_12 = 1;
			}
		}
	}
	if (cons[att] != 0)
	{
		if (timers[tattsync] > 5)
		{
			if ((return_att_uhc[0] != c_up_hall_call[0]) ||
				(return_att_dhc[0] != c_dn_hall_call[0]))
			{
				update_att_fcc_18 = 1;
				update_att_fcc_5 = 1;
			}
			if ((return_att_ruhc[0] != c_upr_hall_call[0]) ||
				(return_att_rdhc[0] != c_dnr_hall_call[0]))
			{
				update_att_rcc_6 = 1;
				update_att_rcc_7 = 1;
			}
#if (Tract_HR == 1)
			if (cons[grtopf] > 32)
			{
				// Check for attendant hall call annuciator update
				if ((return_att_uhc[1] != c_up_hall_call[1]) ||
					(return_att_dhc[1] != c_dn_hall_call[1]))
				{
					update_att_fcc_5 = 1;
					update_att_fcc_11 = 1;
				}
				if ((return_att_ruhc[1] != c_upr_hall_call[1]) ||
					(return_att_rdhc[1] != c_dnr_hall_call[1]))
				{
					update_att_rcc_7 = 1;
					update_att_rcc_12 = 1;
				}
			} 
#endif
		}
	}
}

//****************************************
// Update attendant hall call annunciator
//****************************************

void update_cc_sec(void)
{
   	if (master_cc_sec[0] != master_cc_sec_prev[0])
   	{
   		cc_sec_flag_18 = 1;
   		cc_sec_flag_5 = 1;
   		master_cc_sec_prev[0] = master_cc_sec[0];
   	}
   	if (master_cc_sec[1] != master_cc_sec_prev[1])
   	{
   		cc_sec_flag_5 = 1;
   		cc_sec_flag_11 = 1;
   		master_cc_sec_prev[1] = master_cc_sec[1];
   	}
   	if (cons[rear] != 0)
   	{								
   		if (master_rcc_sec[0] != master_rcc_sec_prev[0])
   		{
   			rcc_sec_flag_6 = 1;
   			rcc_sec_flag_7 = 1;
   			master_rcc_sec_prev[0] = master_rcc_sec[0];
   		}
   		if (master_rcc_sec[1] != master_rcc_sec_prev[1])
   		{
   			rcc_sec_flag_7 = 1;
   			rcc_sec_flag_12 = 1;
   			master_rcc_sec_prev[1] = master_rcc_sec[1];
   		}
	}
}

//****************************************
// Update attendant hall call annunciator
//****************************************

void update_att_hc_ann(void)
{

	int16 temp = 0;
	// Check for attendant hall call annuciator update
	if (prev_up_hc[0] != c_up_hall_call[0])
	{
		update_att_fhc = 1;
		update_att_nmb_hc = 1;
	}
	if (prev_dn_hc[0] != c_dn_hall_call[0])
	{
		update_att_fhc = 1;
		update_att_nmb_hc = 1;
	}
	if (prev_upr_hc[0] != c_upr_hall_call[0])
	{
		update_att_rhc = 1;
		update_att_nmb_hc = 1;
	}
	if (prev_dnr_hc[0] != c_dnr_hall_call[0])
	{
		update_att_rhc = 1;
		update_att_nmb_hc = 1;
	}
	prev_up_hc[0] = c_up_hall_call[0];
	prev_dn_hc[0] = c_dn_hall_call[0];
	prev_upr_hc[0] = c_upr_hall_call[0];
	prev_dnr_hc[0] = c_dnr_hall_call[0];


	if (cons[Second_Riser] != 0)
	{
		if (prev_irup_hc[0] != c_irup_hall_call[0])
		{
			update_att_fhc = 1;
			update_att_nmb_hc = 1;
		}
		if (prev_irdn_hc[0] != c_irdn_hall_call[0])
		{
			update_att_fhc = 1;
			update_att_nmb_hc = 1;
		}
		if (prev_rirup_hc[0] != c_rirup_hall_call[0])
		{
			update_att_rhc = 1;
			update_att_nmb_hc = 1;
		}
		if (prev_rirdn_hc[0] != c_rirdn_hall_call[0])
		{
			update_att_rhc = 1;
			update_att_nmb_hc = 1;
		}
		prev_irup_hc[0] = c_irup_hall_call[0];
		prev_irdn_hc[0] = c_irdn_hall_call[0];
		prev_rirup_hc[0] = c_rirup_hall_call[0];
		prev_rirdn_hc[0] = c_rirdn_hall_call[0];
	}

	
#if (Tract_HR == 1)
	if (cons[grtopf] > 32)
	{
		// Check for attendant hall call annuciator update
		if (prev_up_hc[1] != c_up_hall_call[1])
		{
			update_att_fhc_hr = 1;
			update_att_nmb_hc = 1;
		}
		if (prev_dn_hc[1] != c_dn_hall_call[1])
		{
			update_att_fhc_hr = 1;
			update_att_nmb_hc = 1;
		}
		if (prev_upr_hc[1] != c_upr_hall_call[1])
		{
			update_att_rhc_hr = 1;
			update_att_nmb_hc = 1;
		}
		if (prev_dnr_hc[1] != c_dnr_hall_call[1])
		{
			update_att_rhc_hr = 1;
			update_att_nmb_hc = 1;
		}
		prev_up_hc[1] = c_up_hall_call[1];
		prev_dn_hc[1] = c_dn_hall_call[1];
		prev_upr_hc[1] = c_upr_hall_call[1];
		prev_dnr_hc[1] = c_dnr_hall_call[1];
		
		if (cons[Second_Riser] != 0)
		{
			// Check for attendant hall call annuciator update
			if (prev_irup_hc[1] != c_irup_hall_call[1])
			{
				update_att_fhc_hr = 1;
				update_att_nmb_hc = 1;
			}
			if (prev_irdn_hc[1] != c_irdn_hall_call[1])
			{
				update_att_fhc_hr = 1;
				update_att_nmb_hc = 1;
			}
			if (prev_rirup_hc[1] != c_rirup_hall_call[1])
			{
				update_att_rhc_hr = 1;
				update_att_nmb_hc = 1;
			}
			if (prev_rirdn_hc[1] != c_rirdn_hall_call[1])
			{
				update_att_rhc_hr = 1;
				update_att_nmb_hc = 1;
			}
			prev_irup_hc[1] = c_irup_hall_call[1];
			prev_irdn_hc[1] = c_irdn_hall_call[1];
			prev_rirup_hc[1] = c_rirup_hall_call[1];
			prev_rirdn_hc[1] = c_rirdn_hall_call[1];
		}
	} 
#endif

	temp = att_nmb_hc; 
	if(update_att_nmb_hc == 1)
	{
		att_nmb_hc = nmb_asg_uc + nmb_asg_dc;
		update_att_nmb_hc = 0;
	}
	if(att_nmb_hc > temp)
	{
		att_nmb_hc2buz++;
	}
}		

void set_door_command (void)
{
	if (rdoutp(o_DO) == 1)
	{
		DOOR_can.command |= b_door_open;
	}
	else
	{
		DOOR_can.command &= ~b_door_open;
	}

	if (rdoutp(o_DC) == 1)
	{
		DOOR_can.command |= b_door_close;
	}
	else
	{
		DOOR_can.command &= ~b_door_close;
	}

	if (rdoutp(o_NUD) == 1)
	{
		DOOR_can.command |= b_door_nudge;
	}
	else
	{
		DOOR_can.command &= ~b_door_nudge;	
	}

	if (cons[heavy_door] == 1)
	{
		
		if (rdoutp(o_HVD) == 1)
			DOOR_can.command |= b_door_heavy;
		else
			DOOR_can.command &= ~b_door_heavy;	
	}
	
#if (Door_Test == 1)
	if (rdoutp(o_DTR) == 1)
		DOOR_can.command |= b_door_narrow;
	else
		DOOR_can.command &= ~b_door_narrow;	
#endif 

	if (rdinp(i_DZ) == 1)
		DOOR_can.command |= b_door_zone;
	else
		DOOR_can.command &= ~b_door_zone;

	if ((cons[dflt_MOVFR] & 0x02) != 0)
	{

		if (rdoutp(o_DO) == 1)
			setoutp(o_std_DO);
		else
			clroutp(o_std_DO);
		if (rdoutp(o_DC) == 1)
			setoutp(o_std_DC);
		else
			clroutp(o_std_DC);
		if (rdoutp(o_NUD) == 1)
			setoutp(o_std_NUD);
		else
			clroutp(o_std_NUD);
		
	}
}

void set_rdoor_command (void)
{
	if (rdoutp(o_DOR) == 1)
		RDOOR_can.command |= b_door_open;
	else
		RDOOR_can.command &= ~b_door_open;

	if (rdoutp(o_DCR) == 1)
		RDOOR_can.command |= b_door_close;
	else
		RDOOR_can.command &= ~b_door_close;

	if (rdoutp(o_NUDR) == 1)
		RDOOR_can.command |= b_door_nudge;
	else
		RDOOR_can.command &= ~b_door_nudge;

	if (cons[heavy_door] == 1)
	{
		
		if (rdoutp(o_HVDR) == 1)
			RDOOR_can.command |= b_door_heavy;
		else
			RDOOR_can.command &= ~b_door_heavy;
	}
	
	if (rdinp(i_DZ) == 1)
		RDOOR_can.command |= b_door_zone;
	else
		RDOOR_can.command &= ~b_door_zone;

	if ((cons[dflt_MOVFR] & 0x02) != 0)
	{

		if (rdoutp(o_DOR) == 1)
			setoutp(o_std_DOR);
		else
			clroutp(o_std_DOR);
		if (rdoutp(o_DCR) == 1)
			setoutp(o_std_DCR);
		else
			clroutp(o_std_DCR);
		if (rdoutp(o_NUDR) == 1)
			setoutp(o_std_NUDR);
		else
			clroutp(o_std_NUDR);
	}
}

#if (Traction == 1)
int16 blind (void)
{

	int16 blind_shaft = 0;
	if (cons[Blind_Floor] != 0)
	{								/* Floor below the Blind Hoistway	*/
		if ((front_slowdown != 0) || (rear_slowdown != 0)) 
			blind_shaft = 0;
		else 
		{
			if ( (Velocity >= 100) && (dirf == 1))
			{
				if ( (position <= cons[Blind_Floor]) && (DPP_Count > (DPP_Floor_Pos[cons[Blind_Floor]] + (int32)(24.0*Pulses_per_Inch))) )
					blind_shaft = 1;
				else if (position > cons[Blind_Floor])
					blind_shaft = 0;
			}
			else if ((Velocity >= 100) && (dirf == 2))
			{
				if ( (position > cons[Blind_Floor]) && (DPP_Count < (DPP_Floor_Pos[cons[Blind_Floor] + 1] - (int32)(24.0*Pulses_per_Inch))) )
					blind_shaft = 1;
				else if	(position <= cons[Blind_Floor])
					blind_shaft = 0;
			}
		}
	}
	else if (cons[Blind_Floor_2] != 0)
	{								/* Floor below the Blind Hoistway	*/
		if ((front_slowdown != 0) || (rear_slowdown != 0)) 
			blind_shaft = 0;
		else 
		{
			if ( (Velocity >= 100) && (dirf == 1))
			{
				if ( (position <= cons[Blind_Floor_2]) && (DPP_Count > (DPP_Floor_Pos[cons[Blind_Floor_2]] + (int32)(24.0*Pulses_per_Inch))) )
					blind_shaft = 2;
				else if (position > cons[Blind_Floor_2])
					blind_shaft = 0;
			}
			else if ((Velocity >= 100) && (dirf == 2))
			{
				if ( (position > cons[Blind_Floor_2]) && (DPP_Count < (DPP_Floor_Pos[cons[Blind_Floor_2] + 1] - (int32)(24.0*Pulses_per_Inch))) )
					blind_shaft = 2;
				else if	(position <= cons[Blind_Floor_2])
					blind_shaft = 0;
			}
		}
	}

	return (blind_shaft);
}
#endif

//***************************************************************
// Check for valid second board when using RGB car call boards
//***************************************************************

int16 valid_second_board (unsigned char addr)
{
	if ((addr == c_ctcan_SEB_5) && (cons[topf] >= 25))
		return(1);
	if ((addr == c_ctcan_SEB_6) && (top_rfl[cons[carnmb]] >= 11))
		return(1);
	if ((addr == c_ctcan_SEB_7)  && (top_rfl[cons[carnmb]] >= 30))
		return(1);
	if ((addr == c_ctcan_SEB_11) && (cons[topf] >= 49))
		return(1);
	if ((addr == c_ctcan_SEB_12)  && (top_rfl[cons[carnmb]] >= 54))
		return(1);
	if ((addr == c_ctcan_SEB_18) && (cons[topf] >= 7))
		return(1);
	if ((addr == c_ctcan_SEB_19) && ((cons[ansi] >= 2004) || (cons[rear] != 0)))
		return(1);
	if ((addr == c_ctcan_SEB_22) && ((cons[dispis] & 0x20) != 0))
		return(1);
	 
	return(0);
}


/* Revision History

  8/14/03 v3.06 mhd		1. Zero serial_ctc.dev[].inpb and out arrays during initialization.
8/29/03 3.07 mhd		1. Changed lwb to an array.
3/3/04 4.00 mhd			1. Added comm to Top of Car board.
						2. Added TOC can comm error.
11/22/04 4.03 mhd		1. Updated pi/lantern routine
						2. Corrected buffer size calculation in IsTxBufFul routine.
12/30/04 4.04 mhd		1. Added communications for Safety Processor Board and Load weigher.
2/2/05 V4.06 MHD		1. Added comm fault for SEB devices.  Also set fault_device in fault buffer when comm fault occurs.
4/9/05 v4.09 mhd		1. Split function of unpack_ctcan_in into receive_ctcan_in and unpack_ctcan_in.  Call receive_ctcan_in from
						   the can interrupt and unpack the buffer in unpack_ctcan_in from ctcan_control. 
						2. In receive_ctcan_in, when reading data make sure i stops at 8.
5/5/05 v4/17 mhd		1. Change online_cnt in packet 22 (load weigher) to 6.
7/8/05 v4.24 mhd		1. Deleted checking for revision to detect a device reset error.
7/14/04 v4.25 mhd		1. Added default statement at the end of all switch case commands is there were none.
						2. Added request and unpack message 10 to request and receive device error messages.
						3. Added unpack message 11 to detect a comm error.
						4. Check that message request from device is zero before setting message.
						5. Disable interrupts in IsTxBufFul when checking buffer pointers ctc_tpix and ctc_tgix.
						6. Place the check for messages for both SPB and LW devices first then check for version == 0.  This was
						   to fix faulty device reset error on power up.
						7. Corrected mistake in upack_ctcan_in message #11 to set timers[tcanrx + (ctcan_rx[ctc_rgix].addr-c_ctcan_SEB_1)] = 0
						   when it is a SPB board.  This creates an invalid index.  Check for correct SEB board to zero time.
8/22/05 v4.29 mhd		1. Changed start index for can from c_ctcan_SPB to c_ctcan_START so we can add the brake board c_ctcan_BRK.
						2. Added brake board messages.
10/05/05 v4.35 pn		1. Corrected in unpack_ctcan_in routine case 14 and case 16 to include serial expansion board 8.  It was only looking
						   for up to serial expansion board 7.
10/19/05 v4.36 mhd		2. Added attendant annuciator packet 26 and 27 outputs.
1/9/06 v4.46 mhd		1. Set mask for car calls to can boards if housing job.
3/9/06 v4.58 mhd		1. Added selector can interface and modified SPB, BRK and LW message check.
3/30/06 v4.61 pn		1. Clear hall call anunciator outputs when not on attendant service.
4/28/06 v4.65 mhd		1. Send attendant annunciator outputs when using a car switch (cons[car_sw] != 0).
5/22/06 v4.67 mhd		1. Check data_req flag to send update brake data.
8/14/06 v5.01 mhd		1. Only send brake packet when brake packet has changed also continue to transimt until command
						   and trigger value matches values returned from the brake board.
						2. Use individual timers for safety processor, brake, load weigher and temperature input.
						3. Change selector reading to every 50 msec.
8/23/06 v5.03 mhd		1. Must decrement online count for brake.
						2. Change temporary variables ii and ll to uwu.w, uiu.dw for transmit and uwu.w, uiu.dw for receive.
						3. Added prev_brk_ctl and prev_brk_trig_tim to send brake data, also if return data not received,
						   wait for timer to expire.
						4. Read load weigher every 200 msec when car at floor and record 10 samples instead of 5.
9/17/06 v5.04 mhd		1. Move online check for brake within packet transmit if statement.
11/6/06 v5.08 mhd		1. Added Cab lanterns to set_pi_lant_status routine.
2/20/07 v5.18 mhd		1. Added additional i/o boards for 60 floors.
4/29/07 v5.23 mhd		1. Anded (chk_ins() == 1) with (safetie() == 1) to set pi data for inspection flag so that if the car is on safeties
						   it won't show inspection	on the message display.
4/30/07 	mhd			2. Added packet 23 to preset selector position count.
5/30/07	v5.24 mhd		3. Modified packets 8 and 9 and added control software for new brake board (cons[brake] == 3)
6/12/07 v5.26 mhd		1. Added toc_ctcan_error_off_cnt to delay clearing the toc_ctcan_error flag so that a DZA fault can be reset if
                           it were caused by a comm fault.
8/21/07 v5.35 mhd		1. Correct setting of DOLR and DCLR when comm lost for seperate rear door board cons[rearsel] == 1. 
10/30/07 v5.38.4 mhd	1. No PI or handicap buzzer if not a valid_fl[].
11/28/07 v5.39.4 mhd	1. Use lobby_fl variable instead of fvars[fvlob] so that lobby_fl can be changed to alternate lobby fvars[fvaltlob]. 	
3/21/08 v5.42.3 mhd		1. Added service message variable to display service message to PI if variable matches servf.
3/27/08 v5.42.5 mhd		1. Added can service for door operators.
4/21/08 v5.43.7 mhd		1. Addes specail can board SEB_10. Changed the locations of which io was used. see hmmap.h for array locations.
4/24/08 v5.43.8 mhd		1. Changed SEB_10 to SEB_10_io and SEB10 to SEB_10 so that the address label is the address. Also did the same for PI_LANT
                           and PILANT to PI_LAND and PI_LANT_id.
5/29/08 v5.44.1 mhd		1. Modified messages 28 and 29, added messages 30 and 31 for door operator.
7/3/08 v5.45 mhd		1. Send reset unintended motion fault command to safety processor board in packet 1.
7/17/08 v5.45.1 mhd		1. Send gr1 test and SFC test commands to safety processor board in packet 32.  Moved rest unintended motion fault command to packet 32.
8/4/08 v5.45.4 mhd		1. Changed SPB version check from ((SPB_can.version >= 5) && (SPB_can.revision >= 8)) to 
						   ((SPB_can.version >= 5) || ((SPB_can.version == 5) && (SPB_can.revision >= 8)).
8/29/08 v5.45.7 mhd		1. Changed SPB version check from ((SPB_can.version >= 5) to ((SPB_can.version > 5).
12/16/08 v5.46.8 hdl	1. Added check for cons[att] == 2 to set attendant hall annunciator lights.
12/22/08 v5.46.9 mhd	1. Change the location of cab lantern bits for pi output to match spec sent to CE.
01/06/09 v5.47 pn		1. Added alternate ascii pi's to alternate between regular 
						   ascii pi's and alternate ascii pi's.
03/23/09 v5.47.11 mhd	1. Added PI cross reference position output.
4/14/09 v5.48.2 mhd		1. Set online_cnt for can door board when receiving data.
4/22/09 V5.48.4 mhd		1. Added DPM and RPM for serial MOVFR.
						2. Corrected index for RPM[] from 1 to 2 for serial MOVFR.
5/18/09 v5.48.6 mhd		1. Added c_ctcan_DOOR and c_ctcan_RDOOR devices to init packet 1.
7/2/09 v5.49 mhd		1. Deleted SEL_can.diagnostic and added other variable to structure.
						2. Modified packet 24 for selector and added packets 34 - 40. 
8/28/09 v5.49.11 mhd	1. Added packet 43 to send cc board light color and brightness.
						2. Send car call board color and brightness checksum in packet 13 so board can request a packet 43 if the checksum does not match.
10/02/09 v5.50.4 pn/mhd	1. In pack_ctcan_out() packet 25 where (cons[Alt_Ascii_pi] == 1), change check for alternate pi from AND to OR.

1/25/10  v5.51.8		1. Remove line ctcan_dev[devnmb].tx_cnt++;
							from packet 43. It was writing data to an index outside the ctcan_dev structure.
2/25/10 v5.51.12 pn		1. Added blind() routine for special pi output for blind shaft.
3/24/10 v5.52 mhd		1. Added packets 46 and 47 to transmit labels to RGB car call board.
						2. Added packets 48 and 49 to receive error codes from RGB car call boards. 
						3. Added fault messages for RGB Car Call Board.
						4. Send checksum for labels in packet 13. 
						5. Receive local board comm status from RBG car call boards in packet 14.
						6. Added clr_ctcan_dev_comm_loss() to clear comm loss error latch for RGB Car call driver board local devices.
5/7/10 v5.52.8 mhd		1. Added can packets 52-58 for RGB Car call boards to be able to change the color of fire and emergency outputs, to synchronize and set the
						   security and attendant flashing on the car calls.
						2. Send sync every 250 msec.
6/5/10 	mhd				3. Corrected brake packet #8 to load Return_Brake_Voltage if cons[brake] == 3. Also corrected return check
						   to repeat packet 9 for Return_Brake_Voltage if brake == 3 and Return_Brk_Trig_Tim if brake == 2. 
						4. Modified timing on attendant annunciator packets.
						5. Added attendant annunciator update flag high rise if floors greater then 32.
6/7/10 v5.52.9 mhd		1. Added packets 54 and 56 for annunciator and security.
						2. Use master_cc_sec[] variable for any secured car call.
						3. Added mechanism to update hall call annunciator and car call security and modified changed of state packet transmission.
						4. Place rgb_cc local address in fault_device_2 and io location in fault_prog_flag1.
						5. Modified cop_light_st in packet 52 to alternate attendant up and down, and car call security.
						6. Modified transmit timing for annunciator packets.
6/25/10 v5.52.11 mhd	1. Changed Tq for can bus timing from 8 to 16.
7/1/10 v5.52.12 mhd		1. Added Electronic Emergency Brake.
						2. Added Can baud rate for 57.6K from parameter fvcanbaud.
10/14/10 V5.52.37 mhd 	1. Changed ((SPB_can.version > 5) || ((SPB_can.version == 5) && (SPB_can.revision >= 8)) to
						   (SPB_can.version > 5)
10/25/10 v5.52.39 pn/hdl 1. Changed (SPB_can.version > 5)  back to (see above) ((SPB_can.version > 5) || ((SPB_can.version == 5) && (SPB_can.revision >= 8))
						The changed implemented in version 5.52.37 made the controller fail the rope gripper test for 2007.	
						The CPU will not send the command over the CAN bus to the safety processor to test the GR1 output
10/26/10 v5.52.41 mhd	1. Added two additional I/O board on can bus.
1/7/11 v5.52.54 hdl		1. Added code to flash Alarm status on PI.
1/11/11 v5.52.56 mhd	1. When receiving spb packet for velocity, set direction whenever sel_type != 0 to allow for sel_type == 3.
4/29/11 5.52.16 hdl		1. Corrected the logic for the hall call buzzer when   fvhcattbuz == 1. It was deleted at one point. It is operational once again
5/31/11 v5.54.02 mhd	1. Set SPB_can.vel_dir from SPB if version greater than or equal to 5.17 and 6.5.
7/14/11 v5.54.15 mhd	1. Set hall call annunciator outputs from the standard hall call and ir hall calls when on second riser and att.
						2. Added missing "break;" command after packet 51 in pack_ctcan_out().
7/26/11 v5.54.17 mhd	1. Removed code to set the at lobby flag in the set pi routine.

8/16/11	v5.54.22 hdl	1. Added option to have the PI dispaly Door Hold when EDL output is turned on.

10/5/11 v5.54.28 hdl	1. change this statement if (rdinp(i_OLV) == 1) to if(in_overload == 1) in the can bus data do PI driver for it to send the overload bit
							when using any loadweigher

11/29/11 v5.54.41 hdl	1. RDOOR_can.command was fixed as well as IO select for can bus rear door operator
1/24/12 v6.0 mhd		1. Added seperate error codes for aux or em brake.
						2, On packets 16 and 20, do not clear the receive timing because it is return data for the output write command. If you zero the timer
						   it screws up the timing for the input timing.

5/1/12 v6.0.7 mhd		1. Deleted clearing spbc_GR1_pick in can control routine.  This flag must remain set to keep GR1 picked.
6/6/12 v6.09 mhd		1. Added SPB_can.prev_cmd, SPB_can.cmd_req and SPB_can.cmd_rcvd to prevent a race condition and leaving SFC off.
6/21/13 v7.0.9 mhd		1. Deleted hall lanterns and Attendant hall call annunciation.						
7//25/13 v7.0.15 mhd	1. Added o_std_DO, o_std_DOR ... etc for additional door oputput with movfr
11/11/13 V7.0.28 mhd	1. Send message 59 to SEB_2 after init response message 12.  This will send the dead zone parameter to the selector.
						2. Replaced iodata[1] with iodata[cons[carnmb]] where necessary.
12/30/13 v7.0.31 mhd	1. Changed availbuf check in IsTxBufFul() from > 4 to > 9, Also changed c_tx_buf_siz and c_rx_buf_siz from 20 to 30.
						2. Moved _disable() and _enable() routines in tx_can_msg() to have interrupts disable during the entire routine. If the ctgix and ctpix indexes are checked or the 
						   (CAN_A.MB[TxBufIx].MSG.MCS.B.CODE) transmit status is checked and then a transmit interrupt occurs, a transmit() routine can be called when the transmitter is not ready
						   causing an invalid packet. 
						3. Added ctcan_tx_complete flag to prevent the tx_can_msg() routine from writing over the buffer of a partially transmitted packet.
						4. Added data_req check for pi/lantern packet. 
						5. Added data_req check for I/O board timer update outputs.
						6. Rotate buffer index when calling tx_ctcan_msg(index).
						7, In packet 47, do not send label if name is not valid.
6/24/14 v7.1.4 mhd		1. Corrected setting lw_can_online to zero when cons[loadw] is not equal to 2.  Moved the else if (cons[loadw] != 3) to the else part of (cons[loadw] == 2).
7/17/15 V7.1.39 mhd		1. Corrected c_ctcan_GLOBAL from 49 to 48.
10/9/15 v7.1.43 mhd		1. Added can message request que.
12/3/15 v7.1.51 mhd		1. Change the data_req_cnt check from 4 to 1.
*/
