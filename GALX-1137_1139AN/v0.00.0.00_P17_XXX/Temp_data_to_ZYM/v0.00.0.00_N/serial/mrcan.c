#define d_MRCAN 1

#include "global.h"

extern int16 Add_msg_to_array_B(char *msg, int16 index);
extern int16 Log_an_event(char *msg);

/*

	Machine Room Can for GALaxy 4

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
	 SEL1			2		  Primary Selector
	 NTS			3		  NTS CPU
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
	1		SPB			CPU				3		Message #1: Reset SPB, TOC or BRK Board
													master version
													master revision
													Control Byte
														bit 0: Error Flag Reset
														bit 1: Clear Fault Code Flag

	2		CPU			SPB				8		Message #2: Velocity, Position and service
													low byte velocity
													high byte velocity
													low byte 1 position count
													low byte 2 position count
													high byte 1 position count
													high byte 2 position count
													service
													Status flags	 
														bit 0: SFCO	
														bit 1: PICEN
														bit 2: LSCS
														bit 3: Com Fault
														bit 4: Velocity Fault
														bit 5: Position Init
														bit 6: Active Fault
														bit 7: Position Updated

	3		CPU			SPB				4		Message #3:	Temperature 
													low byte external temperature
													high byte external temperature
													low byte board temperature
													high byte board temperature
													
	4		CPU			SPB				8		Message #4: Limit Velocity
													low byte uts velocity
													high byte uts velocity
													low byte dts velocity
													high byte dts velocity
													low byte ut velocity
													high byte ut velocity
													low byte dt velocity
													high byte dt velocity

	5		CPU			SPB,BRK			2-8		Message #5: Fault Data
													fault start index/pointer   
														start index = low nibble
														pointer = high nibble
													# of Faults	Transmitted
													fault[index]
													fault[index+1]
													fault[index+2]
													fault[index+3]
													fault[index+4]
													fault[index+(# of Faults - 1)]

	6		CPU			SPB				2-8		Message #6: EEprom Data
													EEprom Data index
													# Bytes
													low byte eep[index]
													high byte eep[index]
													low byte eep[index+1]
													high byte eep[index+1]
													low byte eep[index (# Bytes - 1)]
													high byte eep[index (# Bytes - 1)]

	7		SPB			CPU				2-8		Message #7: Set EEprom Data
													EEprom Data index
													# Bytes
													low byte eep[index]
													high byte eep[index]
													low byte eep[index+1]
													high byte eep[index+1]
													low byte eep[index (# Bytes - 1)]
													high byte eep[index (# Bytes - 1)]

	8		BRK			CPU				7		Message #8: Brake Control command
													Byte 0:	Command
															Bit0: 	0=Off, 1=Fire Brake @ Voltage or Trigger
															Bit 3,2,1:	Byte 1,2 Select: 
															    0,0,0	- Current
															    0,0,1	- Output Voltage
															    0,1,0 	- Input Voltage
																0,1,1   - Fault Data
																1,0,0   - Jumper Setting
															Bit4:	Unused
															Bit5:	Unused
															Bit6:	Unused
															Bit7:	Unused
													Byte 1: Low Byte Trigger Time
													Byte 2: High Byte Trigger Time
													Byte 3: Low Byte Brake Voltage
													Byte 4: High Byte Brake Voltage
													Byte 5: Low Byte Line Voltage
													Byte 6: High Byte Line Voltage

	9		CPU			BRK				8		Message #9: Brake Data
													Byte 0: Status
															Bit0:	0=Okay, 1=Fault
															Bit 3,2,1:	Byte 1,2 Select: 
															    0,0,0	- Current
															    0,0,1	- Output Voltage
															    0,1,0 	- Input Voltage
															    0,1,1	- Fault Data
																1,0,0   - Jumper Setting
															Bit4:	Brake Switch 1
															Bit5:	Brake Switch 2
															Bit6:	Brake Relay Bypass
															Bit7:	0 = single phase, 1 = 3 phase
													Byte 1: Low Byte Brake Current,Volt,Freq,Jumper,Fault (determined by Status Bits 3,2,1)
													Byte 2: High Byte Brake Current,Volt,Freq,Jumper,Fault	(determined by Status Bits 3,2,1)
													Byte 3: Return Command Byte
													Byte 4: Return Low Byte Trigger Time
													Byte 5: Return High Byte Trigger Time
													Byte 6: Return Low Byte Brake Volage 
													Byte 7: Return High Byte Brake Voltage

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

	13		SEB1-40		CPU				3-5		Message #13: Set/Clear Output Serial Expansion Board 1-40 (SEB1-40) (bit set for each output)
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

	24 		CPU			SEL,SPB				5		Message #24: Selector Position
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
																						   

	32		SPB			CPU				1		Message #32: SPB special command packet
													Byte 0:	Command
														Bit 0-1: used in message 23
														Bit 2: Reset Unintended Motion
														Bit 3: Set Unintended Motion
														Bit 4: Test SFC
														Bit 5: Test GR1
													Byte 1:	Brake Command
															Bit0: 	0=Off, 1=Fire Brake @ Voltage 
															Bit1: 	0=Off, 1=Fire Em Brake @ Voltage
															Bit2:	0 = BRK Line Volts, 1= EBK Line Volts
													Byte 2: Low Byte Brake Voltage
													Byte 3: High Byte Brake Voltage
													Byte 4: Low Byte Em Brake Voltage
													Byte 5: High Byte Em Brake Voltage
													Byte 6: Low Byte BRK/EBK Line Voltage (Alternates)
													Byte 7: High Byte BRK/EBK Line Voltage

	33		CPU			SPB				1		Message #33: SPB command return from message 32
													Byte 0: Return command 
													Byte 1: Return Brake Command
															Bit0: 	0=Off, 1=Fire Brake @ Voltage 
															Bit1: 	0=Off, 1=Fire Em Brake @ Voltage
															Bit2:	0 = BRK Output Volts, 1= EBK Output Volts
													Byte 2: Low Byte Return Brake Voltage
													Byte 3: High Byte Return Brake Voltage
													Byte 4: Low Byte Return Em Brake Voltage
													Byte 5: High Byte Return Em Brake Voltage
													Byte 6: Low Byte BRK/EBK Output Voltage
													Byte 7: High Byte BRK/EBK Output Voltage

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
													Byte 0: Fault Code	(Specific code for device)
													Byte 1: Fault Count


	49		any DEV		CPU					1		Message 49: Reset Device Error
													Byte 0: Control bit 0: 1 = reset error condition
																	bit 1: 1 = clear error code buffer

	61		CPU			NTS  				2-8		Message #61: EEprom Data
													EEprom Data index
													# Bytes
													low byte eep[index]
													high byte eep[index]
													low byte eep[index+1]
													high byte eep[index+1]
													low byte eep[index (# Bytes - 1)]
													high byte eep[index (# Bytes - 1)]

	62		NTS			CPU					4		Message #62: Set EEprom Data
													EEprom Data index
													# Bytes = 2
													low byte eep[index]
													high byte eep[index]
													
	63		CPU			NTS					8		Message #63: Limit Velocity
													low byte ut velocity
													high byte ut velocity
													low byte dt velocity
													high byte dt velocity
													low byte ut1 velocity
													high byte ut1 velocity
													low byte dt1 velocity
													high byte dt1 velocity

	64		CPU			NTS					8		Message #64: Limit Velocity
													low byte ut2 velocity
													high byte ut2 velocity
													low byte dt2 velocity
													high byte dt2 velocity
													low byte ut3 velocity
													high byte ut3 velocity
													low byte dt3 velocity
													high byte dt3 velocity

	65		CPU			NTS					8		Message #65: Limit Velocity
													low byte ut4 velocity
													high byte ut4 velocity
													low byte dt4 velocity
													high byte dt4 velocity
													low byte ut5 velocity
													high byte ut5 velocity
													low byte dt5 velocity
													high byte dt5 velocity

	66		CPU			NTS					4		Message #66: Limit Velocity
													low byte ut6 velocity
													high byte ut6 velocity
													low byte dt6 velocity
													high byte dt6 velocity

	67 		SPB			NTSD				8		Message #67: NTSD Status and command return
													status:		bit 0: 1=direction up, 
																bit 1: 1=direction down
																bit 2: 1=sel comm okay
																bit 3: 1=spb comm okay
																bit 4: unused
																bit 5: 1=direction fault
																bit 6: 1=limit fault (em_sd)
																bit 7: 1=velocity fault
													Service:	0= no service
																1 = normal
										  						2 = EM Slowdown
																3 = UN Limit Stop
										 						4 = DN Limit Stop
													limits fault byte 0: 1=limit with error (see limit structure)
													limits fault byte 1:
													limits fault byte 2:
													velocity low byte
													velocity high byte
													Command Return: 
														bit 0: 1=Reset Emergency Slowdown (only if velocity zero)
														

	68 		NTSD		SPB					6		Message #68: NTSD Status return and command
													status return:		bit 0: 1=direction up, 
																bit 1: 1=direction down
																bit 2: 1=sel comm okay
																bit 3: 1=spb comm okay
																bit 4: unused
																bit 5: 1=direction fault
																bit 6: 1=limit fault (em_sd)
																bit 7: 1=velocity fault
													service return
													limits fault return byte 0: 1=limit with error (see limit structure)
													limits fault return byte 1:
													limits fault return byte 2:
													Command: 
														bit 0: 1=Reset Emergency Slowdown (only if velocity zero)


	69		NTS			SPB					3		Message #69: NTS IO data
													io[0]		bit 0: UT
																bit 1: UT1
																bit 2: UT2
																bit 3: UT3
																bit 4: UT4
																bit 5: UT5
																bit 6: UT6
																bit 7: UN
																
													io[1]		bit 0: DT
																bit 1: DT1
																bit 2: DT2
																bit 3: DT3
																bit 4: DT4
																bit 5: DT5
																bit 6: DT6
																bit 7: DN
																
													io[2]		bit 0: UP
																bit 1: DNR
																bit 2: NTSD
																bit 3: NTSD1
																
	70		CPU			SPB					6		Message #70: SPB can 2 status
													Byte 0: txerrcnt
													Byte 1: rxerrcnt
													Byte 2: low byte spbtxcnt
													Byte 3: high byte spbrxcnt
													Byte 4: low byte ntsrxcnt
													Byte 5: high byte ntsrxcnt
													Byte 6: low byte selrxcnt
													Byte 7: high byte selrxcnt
	80-87 Reserved for file transfer on can bus	
	110-115 ANY			CPU				Packets reserved for testing.


*/


#define c_max_mrcan_dev 55	// 55 devices 
#define c_max_mrcan_io_brd 40	// 40 i/o boards 
#define c_max_mrcan_ser_array (c_max_mrcan_io_brd * 3)


int16 mrcan_addr[c_max_mrcan_dev] = 
{ 0, 3, 5, 9,10,20,
 11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,30,31,
 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
 255,52,53,54,55,56,57,58,59};

//ZYM 2-2-2018		   "devix"(this is the devix number, example: devix=6 is for Mechine Room CAN SEB Board #1 CAN ADD is 11)
//									   the devix number meaning the INDEX of "int16 mrcan_addr[c_max_mrcan_dev]"
#define  c_mrcan_START	  1		  // Can start index
#define  c_mrcan_NTS	  1		  // NTS Normal Terminal Slowdown CPU
#define  c_mrcan_SPB	  2		  // Safety processor board
#define  c_mrcan_LW		  3		  // Load weigher
#define  c_mrcan_VS		  4		  // Voltage Sensor
#define  c_mrcan_PI_LANT  5		  // Serial PI and Lantern (global address for all pi and lantern devices)
#define	 c_mrcan_SEB_1	  6		  // Serial I/O Expansion board 1	(io brd 7)
#define	 c_mrcan_SEB_2	  7		  // Serial I/O Expansion board 2	(io brd 8)
#define	 c_mrcan_SEB_3	  8		  // Serial I/O Expansion board 3	(io brd 9)
#define	 c_mrcan_SEB_4	  9		  // Serial I/O Expansion board 4	(io brd 10)
#define	 c_mrcan_SEB_5	 10		  // Serial I/O Expansion board 5	(io brd 11)
#define	 c_mrcan_SEB_6	 11		  // Serial I/O Expansion board 6	(io brd 12)
#define	 c_mrcan_SEB_7	 12		  // Serial I/O Expansion board 7	(io brd 13)
#define  c_mrcan_SEB_8	 13		  // Serial I/O Expansion board 8   (io brd 14)
#define  c_mrcan_SEB_9	 14		  // Serial I/O Expansion board 9   (io brd 15)
#define  c_mrcan_SEB_10	 15		  // Serial I/O Expansion board 10  (io brd 16)
#define  c_mrcan_SEB_11	 16		  // Serial I/O Expansion board 11  (io brd 17)
#define  c_mrcan_SEB_12	 17		  // Serial I/O Expansion board 12  (io brd 18) 
#define  c_mrcan_SEB_13	 18		  // Serial I/O Expansion board 13  (io brd 19) 
#define  c_mrcan_SEB_14	 19		  // Serial I/O Expansion board 14  (io brd 20) 
#define  c_mrcan_SEB_15	 20		  // Serial I/O Expansion board 15  (io brd 21) 
#define  c_mrcan_SEB_16	 21		  // Serial I/O Expansion board 16  (io brd 22) 
#define  c_mrcan_SEB_17	 22		  // Serial I/O Expansion board 17  (io brd 23) 
#define  c_mrcan_SEB_18	 23		  // Serial I/O Expansion board 18  (io brd 24) 
#define  c_mrcan_SEB_19	 24		  // Serial I/O Expansion board 19  (io brd 25) 
#define  c_mrcan_SEB_20	 25		  // Serial I/O Expansion board 20  (io brd 26)
#define  c_mrcan_SEB_21	 26		  // Serial I/O Expansion board 21  (io brd 27)
#define  c_mrcan_SEB_22	 27		  // Serial I/O Expansion board 22  (io brd 28)
#define  c_mrcan_SEB_23	 28		  // Serial I/O Expansion board 23  (io brd 29) 
#define  c_mrcan_SEB_24	 29		  // Serial I/O Expansion board 24  (io brd 30) 
#define  c_mrcan_SEB_25	 30		  // Serial I/O Expansion board 25  (io brd 31) 
#define  c_mrcan_SEB_26	 31		  // Serial I/O Expansion board 26  (io brd 32) 
#define  c_mrcan_SEB_27	 32		  // Serial I/O Expansion board 27  (io brd 33) 
#define  c_mrcan_SEB_28	 33		  // Serial I/O Expansion board 28  (io brd 34) 
#define  c_mrcan_SEB_29	 34		  // Serial I/O Expansion board 29  (io brd 35) 
#define  c_mrcan_SEB_30	 35		  // Serial I/O Expansion board 30  (io brd 36)
#define  c_mrcan_SEB_31	 36		  // Serial I/O Expansion board 31  (io brd 37)
#define  c_mrcan_SEB_32	 37		  // Serial I/O Expansion board 32  (io brd 38)
#define  c_mrcan_SEB_33	 38		  // Serial I/O Expansion board 33  (io brd 39) 
#define  c_mrcan_SEB_34	 39		  // Serial I/O Expansion board 34  (io brd 40) 
#define  c_mrcan_SEB_35	 40		  // Serial I/O Expansion board 35  (io brd 41) 
#define  c_mrcan_SEB_36	 41		  // Serial I/O Expansion board 36  (io brd 42) 
#define  c_mrcan_SEB_37	 42		  // Serial I/O Expansion board 37  (io brd 43) 
#define  c_mrcan_SEB_38	 43		  // Serial I/O Expansion board 38  (io brd 44) 
#define  c_mrcan_SEB_39	 44		  // Serial I/O Expansion board 39  (io brd 45) 
#define  c_mrcan_SEB_40	 45		  // Serial I/O Expansion board 40  (io brd 46)
#define  c_mrcan_END	 45		  // Last can device
#define  c_mrcan_GLOBAL	 46		  // Global can address

#define IODATA_MRCAN_START 6		// 7 - 1


int32 mrcan_debug_1;
int32 mrcan_debug_2;
int32 mrcan_debug_3;
int32 mrcan_debug_4;
int32 mrcan_debug_5;
int32 mrcan_debug_6;

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


int16 mrcan_put_dev_msg_req (int16 dev, int8 msg);
int8 mrcan_get_dev_msg_req (int16 dev);
void init_mrcan_bus (void);
//void write_can (uint8 addr, uint8 data);
//uint8 read_can (uint8 addr);
void clr_mrcan_dev_comm_loss (void);
unsigned char calc_io_color_ix_mrcan_cksum (unsigned char devnmb);
uint16 calc_ioname_mrcan_cksum (unsigned char devnmb);
void wrong_mrcan_int (void);
void mrcan_bus_off_and_warning_int (void);
void mrcan_error_int (void);
void mrcan_rcv_int (void);
void mrcan_rcv_fifo_warning_int (void);
void mrcan_rcv_fifo_overflow_int (void);
void mrcan_trm_int_b8 (void);
void mrcan_trm_int_b9 (void);
void mrcan_trm_int_b10 (void);
void mrcan_trm_int_b11 (void);
void mrcan_control (void);
void receive_mrcan_in (int16 BufIx);
void unpack_mrcan_in (void);
int16 Mrcan_ChipFreezeMode(unsigned char set);
void tx_mrcan_msg(int16 TxBufIx);
void mrcan_transmit (int16 TxBufIx);
int16 IsMrcTxBufFul(void);
void pack_mrcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req);
void set_dd_cc_mrcan_pi_dpy(void);



// Declarations for Serial I/O Boards


union tag_ser_mr {

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
	} dev [c_max_mrcan_io_brd];	
	
	unsigned char brd_io [c_max_mrcan_ser_array];
}serial_mrc;

struct tag_sync_mrc {
		unsigned char iob[3];
		unsigned char reflect_data;
		unsigned char rtx_cnt;
} sync_dev_mrc [c_max_mrcan_io_brd];


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

#define mrcan_que_len 3

struct tag_mrcan_dev {
		unsigned char online;
		unsigned char online_cnt;
		unsigned char version;
		unsigned char revision;
		unsigned char msg_req[mrcan_que_len];
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
} mrcan_dev[c_max_mrcan_dev];

int16 mrcan_dev_req = 0;
int16 mrcan_msg_req = 0;
int16 mrcan_msg_cmd = 0;
int16 mrcan_msg_cnt = 0;

int16 mrcan_color_send_ix = 0;

unsigned char mrc_ioset[c_max_mrcan_ser_array];
unsigned char mrc_ioclr[c_max_mrcan_ser_array];

unsigned char mrcan_dev_list [c_max_mrcan_dev];	// This is the list of can devices used 
int16 nmb_mrcan_dev;
int16 mrcan_ram_seq = 0;
int16 mrcan_init_count = 0;
int16 mrcan_init_mode_cnt = 0;
bool mrcan_in_chip_reset = false;
int32 mrcan_status = 0;
int16 mrcan_mode = 0;
int16 mrcan_int_status = 0;
int16 mrcan_error = 0;
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
int16 mrcan_int_count = 0;
int16 re_init_mrcan = 0;
int16 mrcan_arb_lost_cap=0;
int16 mrcan_err_code_cap=0;
int16 mrcan_tx_err_cnt=0;
int16 mrcan_rx_err_cnt = 0;
int16 mrcan_rcv_fifo_cnt = 0;
int16 mrcan_rcv_int_fail_cnt = 0;
int16 mrcan_WCI_cnt=0;	// Wrong can interrupt
int16 mrcan_BOI_cnt=0;	// Bus Off and tx/rx warning interrupt count
int16 mrcan_ERI_cnt=0;	// Error Interrupt count
int16 mrcan_RI_cnt=0;		// Receive fifo interrupt count
int16 mrcan_FWI_cnt=0;	// Fifo warning interrupt count
int16 mrcan_FOI_cnt=0;	// Fifo Overflow interrupt count
int16 mrcan_TI8_cnt=0;	// Transmit buffer 8 interrupt count
int16 mrcan_TI9_cnt=0;	// Transmit buffer 9 interrupt count
int16 mrcan_TI10_cnt=0;	// Transmit buffer 10 interrupt count
int16 mrcan_TI11_cnt=0;	// Transmit buffer 11 interrupt count
int16 mrcan_ack_error_cnt = 0;
int16 mrcan_bus_off_cnt = 0;
int16 mrcan_bus_idle_cnt = 0;

int16 mrcan_tx_B08_fail_cnt = 0;
int16 mrcan_tx_B09_fail_cnt = 0;
int16 mrcan_tx_B10_fail_cnt = 0;
int16 mrcan_tx_B11_fail_cnt = 0;


int16 mrcan_data_req = 0;				// Flag to signal that data has been requested by a device
int16 mrcan_data_req_cnt = 0;

#define c_mrcan_tx_buf_siz 30

struct tag_mrcan_tx {
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
		} mrcan_tx[c_mrcan_tx_buf_siz+1];

int16 mrc_tpix = 0;					// can transmit buffer put index
int16 mrc_tgix = 0;					// can transmit buffer get index
int16 mrc_availbuf = 0;				// available can transmit buffer
bool mrcan_tx_complete[16] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int16 MrcanTxBufIx = 8;
		
#define c_mrcan_rx_buf_siz 30

struct tag_mrcan_rx {
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
		} mrcan_rx[c_mrcan_rx_buf_siz + 1];
		
int16 mrc_rpix = 0;					// can transmit buffer put index
int16 mrc_rgix = 0;					// can transmit buffer get index



uint8 upd_pi_lant_mrcan;
uint32 mr_pi_cc[2] = {0,0};
uint32 mr_pi_rcc[2] = {0,0};

uint8 pi_cc_mrcan_command = 0;
uint8 pi_cc_mrcan_floor = 0;
uint8 pi_mrcan_location = 0;
uint8 pi_cc_mrcan_cnt = 0;
uint8 pi_cc_mrcan_update = 0;

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


//#if (Simulator == 0)
	#define MR_CAN CAN_D				// Use C for machine room
	#define MR_CAN_INT_BASE 199
//#else
//	#define MR_CAN CAN_B				// Use C for machine room
//	#define MR_CAN_INT_BASE 157
//#endif

// Simple que with length of 3 for each device
//--------------------------------------
// Put device message request on que
//--------------------------------------
int16 mrcan_put_dev_msg_req (int16 dev, int8 msg)
{
	int16 ix = 0;

	while(ix < mrcan_que_len)
	{      // look for and open spot on the que
		if (mrcan_dev[dev].msg_req[ix] == 0)
		{         // found an open spot so place Packet on que
			mrcan_dev[dev].msg_req[ix] = msg;
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
int8 mrcan_get_dev_msg_req (int16 dev)
{
	int16 ix = 0;
	int8 msg = 0;			// return 0 if no Packet on queue

	while (ix < mrcan_que_len)
	{
		if (mrcan_dev[dev].msg_req[ix] != 0) 
		{      	// Packet for this device found
			msg = mrcan_dev[dev].msg_req[ix];
			mrcan_dev[dev].msg_req[ix] = 0;
			break;
		}
		else
			ix++;
	}

	// shift que data to the top (start where ix left off)

	while (ix < (mrcan_que_len-1))
	{
		if (mrcan_dev[dev].msg_req[ix+1] != 0)
		{   // Packet was taken from the middle of the que so shift data up
			mrcan_dev[dev].msg_req[ix] = mrcan_dev[dev].msg_req[ix+1];
			mrcan_dev[dev].msg_req[ix+1] = 0;
			ix++;
		}
		else
			break;
	}
	return (msg);
}

// **************************************************
// ** Clear can device comm loss for local devices
// **************************************************

// clear comm loss error latch for RGB Car call driver board local devices
void clr_mrcan_dev_comm_loss (void)
{
	int16 i;
	for (i = 0;i<c_max_mrcan_dev; i++)
	{
		mrcan_dev[i].local_dev_comm_loss = 0;
	}
}

//*******************************************************************
// Sends the freeze bit to the chip
// Value of 1 puts the chip in freeze mode
// Value of 0 takes is out of freeze mode
// Have to see the freeze mode acknowledge bit gets set
//*******************************************************************
int16 Mrcan_ChipFreezeMode(unsigned char set)
{
	
	if (mrcan_in_chip_reset == true)
		return 0;

	mrcan_in_chip_reset = true;

	mrcan_init_mode_cnt = 0;

	MR_CAN.MCR.B.MDIS = 0;			// Enable module after reset
	MR_CAN.MCR.B.FRZ = 1;			// Set or clear the freeze bit
	MR_CAN.MCR.B.HALT = set;			// Clear halt mode

	
	while (mrcan_init_mode_cnt < 200)
	{
		if (MR_CAN.MCR.B.FRZACK == set)
		{
			mrcan_in_chip_reset = false;
			return 1;
		}
		mrcan_init_mode_cnt++;
	}
	mrcan_in_chip_reset = false;
	return 0;		// Failed to initialize
}




// **************************************************
// ****** Calculate ioname can device checksum ******
// **************************************************

uint16 calc_ioname_mrcan_cksum (unsigned char devnmb)
{
	uint16 chksum = 0;
	int16 h,i,j;
	int16 array_start, io_name_start;
	int16 iodev_ix = 0;
	int16 nmb_io_labels = 0;

	iodev_ix = (int16)(devnmb-c_mrcan_SEB_1);

	array_start = (int16)((IODATA_MRCAN_START + iodev_ix) * 3); 		// input array
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

unsigned char calc_io_color_ix_mrcan_cksum (unsigned char devnmb)
{
	unsigned char chksum = 0;
	int16 i,j;
	unsigned char color_off_ix = 0;
	unsigned char color_on_ix = 0;
	int16 array_start, io_start;
	int16 iodev_ix = 0;

	iodev_ix = (int16)(devnmb-c_mrcan_SEB_1);

	array_start = (int16)((IODATA_MRCAN_START + iodev_ix) * 3); 		// input array

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

// **************************************************
// ****  Initialize the ECAN 1000 CAN interface Board
// **************************************************


void init_mrcan_bus (void)
{	 
	int16 i,j;
	int16 reset_count;

	nmb_mrcan_dev = 0;
	mrc_tpix = 0;
	mrc_tgix = 0;
	update_pi_lant = 0;


#if ((Traction == 1) || (Tract_OL == 1))

	if (cons[loadw] == 3)
	{		
		mrcan_dev_list[nmb_mrcan_dev] = c_mrcan_LW;		
		nmb_mrcan_dev++;
	}

	if (cons[Voltage_Sensor] == 2)
	{
		mrcan_dev_list[nmb_mrcan_dev] = c_mrcan_VS;		
		nmb_mrcan_dev++;
	}

#endif

	j = 0;
	for (i=6;i<46;i++)			
	{
		if (IO_board[cons[carnmb]][i] != 0)
		{		 // Set device in device list
			mrcan_dev_list[nmb_mrcan_dev] = (uint8)(c_mrcan_SEB_1 + j);
			mrcan_dev[(c_mrcan_SEB_1 + j)].ioname_chksum = calc_ioname_mrcan_cksum((uint8)(c_mrcan_SEB_1 + j));
			mrcan_dev[(c_mrcan_SEB_1 + j)].io_color_ix_chksum = 0;
			nmb_mrcan_dev ++;
			serial_mrc.brd_io[j] = IO_In_Out[j+18];
		}
		j++;
	}
 /*
	for (i=0;i<c_max_mrcan_io_brd;i++)
	{
		serial_mrc.dev[0].iob[0] = 0;
		serial_mrc.dev[0].iob[1] = 0xf0;
		serial_mrc.dev[0].iob[2] = 0xff;
		j++;
	}
*/		
	for (i=0;i<c_max_mrcan_dev;i++)
	{
		mrcan_dev[i].version = 0;
		mrcan_dev[i].revision = 0;
		mrcan_dev[i].online = 0;
	}
	// See if the board is there
	mrcan_init_count++;

 	mrcan_tx_complete[8] = 1;
	mrcan_tx_complete[9] = 1;
	mrcan_tx_complete[10] = 1;
	mrcan_tx_complete[11] = 1;

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
	INTC_InstallINTCInterruptHandler(mrcan_bus_off_and_warning_int, MR_CAN_INT_BASE, 1);  
	INTC_InstallINTCInterruptHandler(mrcan_error_int, MR_CAN_INT_BASE+1, 1);  
	INTC_InstallINTCInterruptHandler(wrong_mrcan_int, MR_CAN_INT_BASE+3, 1);  
	INTC_InstallINTCInterruptHandler(wrong_mrcan_int, MR_CAN_INT_BASE+4, 1);  
	INTC_InstallINTCInterruptHandler(wrong_mrcan_int, MR_CAN_INT_BASE+5, 1);  
	INTC_InstallINTCInterruptHandler(wrong_mrcan_int, MR_CAN_INT_BASE+6, 1);  
	INTC_InstallINTCInterruptHandler(wrong_mrcan_int, MR_CAN_INT_BASE+7, 1);  
	INTC_InstallINTCInterruptHandler(mrcan_rcv_int, MR_CAN_INT_BASE+8, 1);  
	INTC_InstallINTCInterruptHandler(mrcan_rcv_fifo_warning_int, MR_CAN_INT_BASE+9, 1);  
	INTC_InstallINTCInterruptHandler(mrcan_rcv_fifo_overflow_int, MR_CAN_INT_BASE+10, 1);  
	INTC_InstallINTCInterruptHandler(mrcan_trm_int_b8, MR_CAN_INT_BASE+11, 1);  
	INTC_InstallINTCInterruptHandler(mrcan_trm_int_b9, MR_CAN_INT_BASE+12, 1);  
	INTC_InstallINTCInterruptHandler(mrcan_trm_int_b10, MR_CAN_INT_BASE+13, 1);  
	INTC_InstallINTCInterruptHandler(mrcan_trm_int_b11, MR_CAN_INT_BASE+14, 1);  

	
	// Send init to can board
	if (Mrcan_ChipFreezeMode(c_Freeze_Start) == 1)
	{
		mrcan_init_count++;
		MR_CAN.MCR.B.SOFTRST = 1;		// Soft Reset
		reset_count = 0;
		while ((MR_CAN.MCR.B.SOFTRST == 1) && (reset_count < 1000))
			reset_count++;
			
		MR_CAN.MCR.B.MDIS = 0;			// Enable module after reset

	    // Clear of all interrupts
	    MR_CAN.IFLAG1.R = 0xFFFFFFFF;
	    MR_CAN.IFLAG2.R = 0xFFFFFFFF;
	    
	    // Disable of Interrupts
		MR_CAN.IMASK1.R = 0;
		MR_CAN.IMASK2.R = 0;
		
		MR_CAN.MCR.B.BCC = 1;			// Allow backward compatibility since all mask will be set to 0xffff
		MR_CAN.MCR.B.MAXMB = 0x0B;		// Set to 12 (0xC - 1)
		MR_CAN.MCR.B.WRNEN = 0;			// Warning interrupts
		MR_CAN.MCR.B.SRXDIS = 1;			// Self Reception disabled
		MR_CAN.MCR.B.FEN = 1;			// Enable RX fifo
		MR_CAN.MCR.B.ABTEN = 1;			// Abort enable
		MR_CAN.MCR.B.LPRIOEN = 0;		// No local priority mode
		MR_CAN.MCR.B.IDAM = 0;			// ID Acceptance mode (one full id per filter element)
		
		
	    // Configure clock AND baud rate timer for 115200 baud
	    
	    /*	
	    	fTq = Bit Rate * Number of time Quanta = fCANCLK / Prescale Value
	    	Prescale Value = fCANCLK/(Bit Rate * Number of Time Quanta)
	    	Prescale Value = 108.75 Mhz / (115200 * 16) = 59.00065
	    */
	    
	    
	    MR_CAN.CTRL.B.CLKSRC = 1; 		// System Clock 108.75 Mhz
		if (fvars[fvcanbaud] == 0)	  // Can Baud Rate, 0=115.2K, 1=57.6K
		    MR_CAN.CTRL.B.PRESDIV = 58;		// Prescale Devision Factor + 1 = Prescale Value for 115200
		else
		    MR_CAN.CTRL.B.PRESDIV = 117;		// Prescale Devision Factor + 1 = Prescale Value for 57600
		
		MR_CAN.CTRL.B.RJW = 2;			// Resync Jump Width = RJW + 1 = 3
		// Total Tq = 16
		MR_CAN.CTRL.B.PSEG1 = 7;			// Phase Segment 1 = (PSEG1 + 1) x Tq = 8 (also sync seg = 1)
		MR_CAN.CTRL.B.PSEG2 = 5;			// Phase Segment 2 = (PSEG2 + 1) x Tq = 6
		MR_CAN.CTRL.B.PROPSEG = 0;		// Propagation Segment time = (PROPSEG + 1) x Tq = 1
		MR_CAN.CTRL.B.SMP = 1;			// Three samples
		
		MR_CAN.CTRL.B.BOFFREC = 0;		// Automamtic recovery
		MR_CAN.CTRL.B.TSYN = 0;			// Timer sync disabled
		MR_CAN.CTRL.B.LBUF = 1;			// Lowest buffer transmitted first
		MR_CAN.CTRL.B.LOM = 0;			// Listen only mode disabled
		
		MR_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
		MR_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
		MR_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
		MR_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled


		// Set Acceptance Mask
		
		MR_CAN.RXIMR[0].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[1].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[2].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[3].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[4].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[5].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[6].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[7].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[8].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[9].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[10].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[11].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[12].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[13].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[14].R = 0x00000000;		// Enable all packets
		MR_CAN.RXIMR[15].R = 0x00000000;		// Enable all packets

		// Set Receive ID
		MR_CAN.MB[6].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		MR_CAN.MB[6].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		MR_CAN.MB[6].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		MR_CAN.MB[6].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID
		MR_CAN.MB[7].RXIDA[0] = 0xC0000000;		// Allow Remote Request and Extended ID
		MR_CAN.MB[7].RXIDA[1] = 0xC0000000;		// Allow Remote Request and Extended ID
		MR_CAN.MB[7].RXIDA[2] = 0xC0000000;		// Allow Remote Request and Extended ID
		MR_CAN.MB[7].RXIDA[3] = 0xC0000000;		// Allow Remote Request and Extended ID

		MR_CAN.MB[8].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		MR_CAN.MB[9].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		MR_CAN.MB[10].MSG.MCS.B.CODE = 0x08;		// write inactive mode
		MR_CAN.MB[11].MSG.MCS.B.CODE = 0x08;		// write inactive mode


		if (Mrcan_ChipFreezeMode(c_Freeze_Stop) == 1)
		{
			mrcan_init_count++;
			if (nmb_mrcan_dev != 0)
			{	// initialize can board

			    // Enable interrupts 
				MR_CAN.IMASK1.R = 0x00000FE0;	// MB 8-11 for transmit and 5-7 for receive
				MR_CAN.IMASK2.R = 0x00;		// MB 32-63 NOT Enabled
				MR_CAN.CTRL.B.BOFFMSK = 0;		// Bus off interrup mask: 0 = disabled; 1 = enabled
				MR_CAN.CTRL.B.ERRMSK = 0;		// Error interrup mask: 0 = disabled; 1 = enabled
				MR_CAN.CTRL.B.TWRNMSK = 0;		// TX warning interrup mask: 0 = disabled; 1 = enabled
				MR_CAN.CTRL.B.RWRNMSK = 0;		// RX warning interrup mask: 0 = disabled; 1 = enabled

			}
		}
		else
		{
			// Set error code
			mrcan_error = 0x01;
		}		
	}
	else
	{
		// Set error code
		mrcan_error = 0x02;
	} 
	_enable();
}

// **************************************************
// ********  Can Board Interrupt Routines  **********
// **************************************************

void wrong_mrcan_int (void)
{
	MR_CAN.IFLAG1.B.BUF00I = 1;
	MR_CAN.IFLAG1.B.BUF01I = 1;
	MR_CAN.IFLAG1.B.BUF02I = 1;
	MR_CAN.IFLAG1.B.BUF03I = 1;
	MR_CAN.IFLAG1.B.BUF04I = 1;
	mrcan_WCI_cnt++;
}
void mrcan_bus_off_and_warning_int (void)
{		// Bus off, Transmit count warning, Receive Count warning
	MR_CAN.ESR.B.BOFFINT = 1;
	MR_CAN.ESR.B.TWRNINT = 1;
	MR_CAN.ESR.B.RWRNINT = 1;
	mrcan_BOI_cnt++;
}

void mrcan_error_int (void)
{		// Bit 0, Bit 1, Acknowledge, CRC, Form or Stuffing Errors
	MR_CAN.ESR.B.ERRINT = 1;
	mrcan_ERI_cnt++;
}

void mrcan_rcv_int (void)
{	// Receive Interrupt (MB5)
	mrcan_RI_cnt++;

	mrcan_rcv_fifo_cnt = 0;
	while ((MR_CAN.IFLAG1.B.BUF05I == 1) && (mrcan_rcv_fifo_cnt < 9))
	{			// Have a can message
		receive_mrcan_in (0);
		MR_CAN.IFLAG1.B.BUF05I = 1;				// Clears the bit
		mrcan_rcv_fifo_cnt++;
	}
	if (mrcan_rcv_fifo_cnt > 1)
		mrcan_debug_5 = mrcan_rcv_fifo_cnt;
}

void mrcan_rcv_fifo_warning_int (void)
{	// Receive fifo warning (MB6)
	MR_CAN.IFLAG1.B.BUF06I = 1;
	mrcan_FWI_cnt++;
}

void mrcan_rcv_fifo_overflow_int (void)
{	// Receive Fifo Overrun Interrupt (MB7)
	MR_CAN.IFLAG1.B.BUF07I = 1;
	mrcan_FOI_cnt++;
}

void mrcan_trm_int_b8 (void)
{	// Transmit Interrupt
	mrcan_TI8_cnt++;
	MR_CAN.IFLAG1.B.BUF08I = 1;
	mrcan_transmit(8);
	mrcan_tx_complete[8] = 1;
}

void mrcan_trm_int_b9 (void)
{	// Transmit Interrupt
	mrcan_TI9_cnt++;
	MR_CAN.IFLAG1.B.BUF09I = 1;
	mrcan_transmit(9);
	mrcan_tx_complete[9] = 1;
}

void mrcan_trm_int_b10 (void)
{	// Transmit Interrupt
	mrcan_TI10_cnt++;
	MR_CAN.IFLAG1.B.BUF10I = 1;
	mrcan_transmit(10);
	mrcan_tx_complete[10] = 1;
}

void mrcan_trm_int_b11 (void)
{	// Transmit Interrupt
	mrcan_TI11_cnt++;
	MR_CAN.IFLAG1.B.BUF11I = 1;
	mrcan_transmit(11);
	mrcan_tx_complete[11] = 1;
}

//************************************************************
// Receive the can input
//************************************************************

void receive_mrcan_in (int16 BufIx)
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
	while (((MR_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) == 0x01) && (read_dly_cnt < 100))
		read_dly_cnt ++;
		
	if (read_dly_cnt > 1)
		mrcan_debug_6 = read_dly_cnt;

	if ((MR_CAN.MB[BufIx].MSG.MCS.B.CODE & 0x09) != 0x01)
	{			// read of CS locks the receive buffer
		if (mrcan_rx[mrc_rpix].valid_msg == 0)
		{
			mrcan_rx[mrc_rpix].ex_id = (uint8)MR_CAN.MB[BufIx].MSG.MCS.B.IDE;
			mrcan_rx[mrc_rpix].len = (uint8)MR_CAN.MB[BufIx].MSG.MCS.B.LENGTH;
			mrcan_rx[mrc_rpix].rtr = (uint8)MR_CAN.MB[BufIx].MSG.MCS.B.RTR;

			if (mrcan_rx[mrc_rpix].len > 8)
				mrcan_rx[mrc_rpix].len = 8;
			if (mrcan_rx[mrc_rpix].ex_id == 1)		// Extended frame
			{
				uiu.dw = (MR_CAN.MB[BufIx].MSG.MID.R << 3);
				mrcan_rx[mrc_rpix].msg = uiu.b[hb];	// Message
				mrcan_rx[mrc_rpix].addr = uiu.b[mhb];	// Address
				mrcan_rx[mrc_rpix].id2 = uiu.b[mlb];
				mrcan_rx[mrc_rpix].id3 = uiu.b[lb];
			}
			else
			{
				uiu.dw = ((MR_CAN.MB[BufIx].MSG.MID.R & 0x1FFC0000) << 1) ;
				uiu_2.dw = ((MR_CAN.MB[BufIx].MSG.MID.R & 0x1FFC0000) >> 2);
				mrcan_rx[mrc_rpix].msg = uiu.b[hb];	// Message
				mrcan_rx[mrc_rpix].addr = uiu_2.b[mhb];    // Address
			}

			i = 0;
			while ((i < mrcan_rx[mrc_rpix].len) && (i < 8))
			{
				mrcan_rx[mrc_rpix].data[i] = MR_CAN.MB[BufIx].MSG.MDATA.B[i];
				i++;
			}
			mrcan_rx[mrc_rpix].valid_msg = 1;
		}
	
	}
	else
	{
		mrcan_debug_4 = MR_CAN.MB[BufIx].MSG.MCS.B.CODE;
		mrcan_rcv_int_fail_cnt++;	
	}
		
	mrcan_rx[mrc_rpix].time = (uint16)MR_CAN.MB[BufIx].MSG.MCS.B.TIMESTAMP;		// Unlocks the buffer

	mrc_rpix++;
	if (mrc_rpix >= c_mrcan_rx_buf_siz)
		mrc_rpix = 0;
			
}

//************************************************************
// Unpack the can receive buffer
//************************************************************

void unpack_mrcan_in (void)
{
	uint8 btemp = 0;
#if (Traction == 1)
	int16 i,j,k;
#endif
	int16 devnmb = 0;
	int16 loop_cnt = 0;
	int16 iodev_ix = 0;
	uint16 iodataix = 0;
	int16 SPB_velocity_temp = 0;
	int32 SPB_poscnt_temp = 0;

	while ((loop_cnt <= 4)  && (mrc_rgix != mrc_rpix))
	{
		if (mrcan_rx[mrc_rgix].valid_msg == 1)
		{		
			devnmb = findmatch(mrcan_rx[mrc_rgix].addr, mrcan_addr, sizeof(mrcan_addr));
			if (mrcan_rx[mrc_rgix].rtr == 0)
			{		// OK to unpack the data 
/*			
			 //////////////// wbg ///////////////////////////   
			    mrcan_dev[devnmb].online = 1;
			    sprintf(src,"unpk mr %d; %d: %d", 
			        mrcan_rx[mrc_rgix].msg, devnmb, 
			            mrcan_dev[devnmb].online);
//-               Add_msg_to_array_B(src, devnmb);
               // // SIU.GPDO[Z6_LED].R = 0;
              ////////////////////////////////////////			
*/			
	 			switch (mrcan_rx[mrc_rgix].msg)
				{
					case 10:	  // Device Can Error Message
						if ((devnmb >= c_mrcan_START) && (devnmb <= c_mrcan_END))
						{	  // id1 is the address
							mrcan_dev[devnmb].can_error = mrcan_rx[mrc_rgix].data[0];
							mrcan_dev[devnmb].can_error_data = mrcan_rx[mrc_rgix].data[1];
						}
						break;

					case 11:	// Invalid request for initialization
						if ((devnmb >= c_mrcan_START) && (devnmb <= c_mrcan_END))
						{	  // id1 is the address
							mrcan_put_dev_msg_req(devnmb, 10);
						}
						break;

					case 12:	// message 12: device version and revision
						if ((devnmb >= c_mrcan_START) && (devnmb <= c_mrcan_VS))
						{	  // id1 is the address
							if (mrcan_dev[devnmb].version != 0) 
							{
								fault_device = devnmb;
								record_fault(f_MRCAN_device_reset);
							}
							mrcan_dev[devnmb].version = mrcan_rx[mrc_rgix].data[0];
							mrcan_dev[devnmb].revision = mrcan_rx[mrc_rgix].data[1];
							mrcan_dev[devnmb].online_cnt = 5;
							mrcan_dev[devnmb].rx_cnt++;
						}
						else if ((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_END))
						{	  // id1 is the address
							if (mrcan_dev[devnmb].version != 0) 
							{
								fault_prog_flag1 = 0;		// must set to zero on this fault for galcom
								fault_device = devnmb;
								record_fault(f_MRCAN_device_reset);		// Need to change to mrcan device reset error
							}
							mrcan_dev[devnmb].version = mrcan_rx[mrc_rgix].data[0];
							mrcan_dev[devnmb].revision = mrcan_rx[mrc_rgix].data[1];
							timers[tmrcanrx + (devnmb-c_mrcan_SEB_1)] = 0;
							mrcan_dev[devnmb].online_cnt = 5;
							mrcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 14:	// message 14: device inputs
						if ((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_SEB_40))
						{	  // id1 is the address
							iodev_ix = (int16)(devnmb-c_mrcan_SEB_1);
							iodataix = (int16)((IODATA_MRCAN_START + iodev_ix) * 3); 		// input array
							serial_mrc.dev[iodev_ix].iob[0] = (uint8)((serial_mrc.dev[iodev_ix].iob[0] & IO_In_Out[iodataix]) | (mrcan_rx[mrc_rgix].data[0] & ~IO_In_Out[iodataix]));
							serial_mrc.dev[iodev_ix].iob[1] = (uint8)((serial_mrc.dev[iodev_ix].iob[1] & IO_In_Out[iodataix+1]) | (mrcan_rx[mrc_rgix].data[1] & ~IO_In_Out[iodataix+1]));
							serial_mrc.dev[iodev_ix].iob[2] = (uint8)((serial_mrc.dev[iodev_ix].iob[2] & IO_In_Out[iodataix+2]) | (mrcan_rx[mrc_rgix].data[2] & ~IO_In_Out[iodataix+2]));
							sync_dev_mrc[iodev_ix].reflect_data = 1;
							mrcan_debug_2++;
							timers[tmrcanrx + (devnmb-c_mrcan_SEB_1)] = 0;
							mrcan_dev[devnmb].online_cnt = 5;
							mrcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 16:	// message 16: return of output data
						if ((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_SEB_40))
						{	  // id1 is the address
							iodev_ix = (int16)(devnmb-c_mrcan_SEB_1);
							iodataix = (int16)((IODATA_MRCAN_START + iodev_ix) * 3); 		// input array
							sync_dev_mrc[iodev_ix].iob[0] = (uint8)((sync_dev_mrc[iodev_ix].iob[0] & ~IO_In_Out[iodataix]) | (~mrcan_rx[mrc_rgix].data[0] & IO_In_Out[iodataix]));
							sync_dev_mrc[iodev_ix].iob[1] = (uint8)((sync_dev_mrc[iodev_ix].iob[1] & ~IO_In_Out[iodataix+1]) | (~mrcan_rx[mrc_rgix].data[1] & IO_In_Out[iodataix+1]));
							sync_dev_mrc[iodev_ix].iob[2] = (uint8)((sync_dev_mrc[iodev_ix].iob[2] & ~IO_In_Out[iodataix+2]) | (~mrcan_rx[mrc_rgix].data[2] & IO_In_Out[iodataix+2]));
							mrcan_dev[devnmb].online_cnt = 5;
							mrcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 22: // message 22: load weigher data
						if (devnmb == c_mrcan_LW)
						{
#if ((Traction == 1) || (Tract_OL == 1))
							uwu.b[lbw] = mrcan_rx[mrc_rgix].data[0];
							uwu.b[hbw] = mrcan_rx[mrc_rgix].data[1];
							lw_data[lw_ix++] = uwu.w;
							if (lw_ix > 9)
								lw_ix = 0;
							for (i=0;i<=9;i++)
								avg_load += lw_data[i];
							avg_load /= 10;
							ret_lw_command = mrcan_rx[mrc_rgix].data[2];	// not used
//							if ((ret_lw_command & c_lw_calibrate) != 0)
//								lw_command &= ~ c_lw_calibrate;
							lw_error = mrcan_rx[mrc_rgix].data[3];
//	 					if (lw_error != 0)
// 							lw_status |= c_lw_error;
// 						else
// 							lw_status &= ~c_lw_error;

							mrcan_dev[devnmb].online_cnt = 6;
							mrcan_dev[devnmb].rx_cnt++;
#endif
						}
						break;
					case 44:		// message 44: Voltage Sensor Input	 (value x 10) = xxx.x Vrms
						if (devnmb == c_mrcan_VS)
						{
							uwu.b[lbw] = mrcan_rx[mrc_rgix].data[0];
							uwu.b[hbw] = mrcan_rx[mrc_rgix].data[1];
							L1_Voltage = uwu.w;
							uwu.b[lbw] = mrcan_rx[mrc_rgix].data[2];
							uwu.b[hbw] = mrcan_rx[mrc_rgix].data[3];
							L2_Voltage = uwu.w;
							uwu.b[lbw] = mrcan_rx[mrc_rgix].data[4];
							uwu.b[hbw] = mrcan_rx[mrc_rgix].data[5];
							L3_Voltage = uwu.w;
							uwu.b[lbw] = mrcan_rx[mrc_rgix].data[6];
							uwu.b[hbw] = mrcan_rx[mrc_rgix].data[7];
							Door_Voltage = uwu.w;
							mrcan_dev[devnmb].online_cnt = 5;
							mrcan_dev[devnmb].rx_cnt++;
						}
						break;
					case 46:	// message 46: Request for i/o labels
						if ((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_SEB_40))
						{	  
							timers[tmrcanrx + (devnmb-c_mrcan_SEB_1)] = 0;
							mrcan_dev[devnmb].label_req = mrcan_rx[mrc_rgix].data[0];
							mrcan_dev[devnmb].label_ix = 0;
							mrcan_dev[devnmb].online_cnt = 5;
							mrcan_dev[devnmb].rx_cnt++;
							mrcan_put_dev_msg_req(devnmb, 47);
							mrcan_dev[devnmb].online_cnt = 5;
							mrcan_dev[devnmb].rx_cnt++;
						}
						break;
						
					case 48:	  // Message 48: Device Error Message
						if ((devnmb >= c_mrcan_START) && (devnmb <= c_mrcan_END))
						{	  
							mrcan_dev[devnmb].dev_error = mrcan_rx[mrc_rgix].data[0];
							mrcan_dev[devnmb].dev_error_data[0] = mrcan_rx[mrc_rgix].data[1];
							mrcan_dev[devnmb].dev_error_data[1] = mrcan_rx[mrc_rgix].data[2];
							fault_device = devnmb;
							fault_prog_flag1 = mrcan_dev[devnmb].dev_error;				// fault code
							fault_prog_flag2 = mrcan_dev[devnmb].dev_error_data[0];		// fault count
							fault_device_2 = mrcan_dev[devnmb].dev_error_data[1];	// speial code
							record_fault(f_MRCAN_device_fault);
							mrcan_dev[devnmb].online_cnt = 5;
							mrcan_dev[devnmb].rx_cnt++;
						}
 						break;

					default:
						break;
				}
			}
			else 
			{
				if ((devnmb >= c_mrcan_START) && (devnmb <= c_mrcan_END))
				{
					mrcan_put_dev_msg_req(devnmb, mrcan_rx[mrc_rgix].msg);
				}
			}
			mrcan_rx[mrc_rgix].valid_msg = 0;
		}
		mrc_rgix++;
		if (mrc_rgix > c_mrcan_rx_buf_siz)
			mrc_rgix = 0; 
		loop_cnt++;
	}
}


//************************************************************
//*********  Check to see if the Transmit buffer is full *****
//************************************************************

int16 IsMrcTxBufFul(void)
{

	_disable();
	if (mrc_tpix == mrc_tgix)
		mrc_availbuf = c_mrcan_tx_buf_siz + 1;
	else if (mrc_tpix > mrc_tgix)
		mrc_availbuf = (c_mrcan_tx_buf_siz - mrc_tpix) + mrc_tgix + 1;
	else 
		mrc_availbuf = mrc_tgix - mrc_tpix;

	_enable();
	if (mrc_availbuf > 9)
		return 0;
	else
		return 1;
}


//************************************************************
//*********  Pack data into the Can Structure ****************
//************************************************************


void pack_mrcan_out(unsigned char msg, unsigned char devnmb,unsigned char tx_req)
{

	int16 iodev_ix = 0;
	uint16 iodataix = 0;
	uint16 io_board = 0;
	uint16 phase = 0;
	unsigned char label_ix = 0;
	unsigned char color_ix = 0;
	unsigned char color_on_ix = 0;
	unsigned char color_off_ix = 0;
	bool valid_name = 0;
#if (Traction == 1)
	uint8 Brk_ACLV;
	int16 t_blind = 0;
#endif
	int16 array_start, io_name_start;
	int16 i, j, k;

	_disable();
	if (mrc_tpix < c_mrcan_tx_buf_siz)
		mrc_tpix++;
	else
		mrc_tpix = 0;
	_enable();
				
	if (tx_req == 1)
	{		 // Used to request a packet from a device
		mrcan_tx[mrc_tpix].rtr = 1;
		mrcan_tx[mrc_tpix].srr = 1;
		mrcan_tx[mrc_tpix].len = 0;
		mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
		mrcan_tx[mrc_tpix].one_tx = 0;
		mrcan_tx[mrc_tpix].msg = msg;
		mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
		mrcan_tx[mrc_tpix].id2 = 0;
		mrcan_tx[mrc_tpix].id3 = 0;
		mrcan_tx[mrc_tpix].data[0] = 0;
		mrcan_tx[mrc_tpix].data[1] = 0;
		mrcan_tx[mrc_tpix].data[2] = 0;
		mrcan_tx[mrc_tpix].data[3] = 0;
		mrcan_tx[mrc_tpix].data[4] = 0;
		mrcan_tx[mrc_tpix].data[5] = 0;
		mrcan_tx[mrc_tpix].data[6] = 0;
		mrcan_tx[mrc_tpix].data[7] = 0;
		if ((devnmb >= c_mrcan_START) && (devnmb <= c_mrcan_END))
		 	mrcan_dev[devnmb].tx_cnt++;
	}
	else switch (msg)
	{
		case 1:			// Init Safety processor Board, NTS board OR load weigher
			if ((devnmb == c_mrcan_SPB) || (devnmb == c_mrcan_NTS) || (devnmb == c_mrcan_LW) || (devnmb == c_mrcan_VS))
			{
				mrcan_tx[mrc_tpix].rtr = 0;
				mrcan_tx[mrc_tpix].srr = 1;
				mrcan_tx[mrc_tpix].len = 4;
				mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
				mrcan_tx[mrc_tpix].one_tx = 0;	   // ok to retransmit 
				mrcan_tx[mrc_tpix].msg = msg;
				mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
				mrcan_tx[mrc_tpix].id2 = 0;
				mrcan_tx[mrc_tpix].id3 = 0;
				mrcan_tx[mrc_tpix].data[0] = version;
				mrcan_tx[mrc_tpix].data[1] = revision;
				mrcan_tx[mrc_tpix].data[2] = (((cons[rsbuffer] & 0x01) << 1) || (cons[rear] & 0x01));	 		// Control byte

				mrcan_tx[mrc_tpix].data[3] = 0x93;	 // 0x93 code = rsbuffer and rear cons data being sent
				mrcan_tx[mrc_tpix].data[4] = 0;	 
				mrcan_tx[mrc_tpix].data[5] = 0;
				mrcan_tx[mrc_tpix].data[6] = 0;
				mrcan_tx[mrc_tpix].data[7] = 0;
				mrcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 11:		// Init data
			if ((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_END))
			{
				mrcan_tx[mrc_tpix].rtr = 0;
				mrcan_tx[mrc_tpix].srr = 1;
				mrcan_tx[mrc_tpix].len = 5;
				mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
				if (mrcan_dev[devnmb].online == 1)
					mrcan_tx[mrc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					mrcan_tx[mrc_tpix].one_tx = 1;
				mrcan_tx[mrc_tpix].msg = msg;
				mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
				mrcan_tx[mrc_tpix].id2 = 0;
				mrcan_tx[mrc_tpix].id3 = 0;
				mrcan_tx[mrc_tpix].data[0] = version;
				mrcan_tx[mrc_tpix].data[1] = revision;
				
				if (cons[housing] == 1) 
				{
/*					if (devnmb == c_mrcan_SEB_4)
					{
						uiu.dw = cc_mask[cons[carnmb]][0];
						mrcan_tx[mrc_tpix].data[2] = uiu.b[lb];	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[3] = (uiu.b[mlb] & 0x0f);	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[4] = 0;	 // Set board call latching mask
					}
					else if (devnmb == c_mrcan_SEB_5) 
					{
						uiu.dw = cc_mask[cons[carnmb]][0];
						mrcan_tx[mrc_tpix].data[2] = ((uiu.b[mlb] >> 4) | (uiu.b[mhb] << 4));	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[3] = ((uiu.b[mhb] >> 4) | (uiu.b[hb] << 4));	 // Set board call latching mask
#if (Tract_HR == 1)
						mrcan_tx[mrc_tpix].data[4] = ((uiu.b[hb] >> 4) & 0x0f);	 // Set board call latching mask
						uiu.dw = cc_mask[cons[carnmb]][1];
						mrcan_tx[mrc_tpix].data[4] |= ((uiu.b[lb] << 4) & 0xf0);	 // Set board call latching mask
#else
						mrcan_tx[mrc_tpix].data[4] = ((uiu.b[hb] >> 4) & 0x01);	 // Set board call latching mask
#endif
					}
					else if (devnmb == c_mrcan_SEB_6) 
					{
						uiu.dw = rcc_mask[cons[carnmb]][0];
						mrcan_tx[mrc_tpix].data[2] = uiu.b[lb];	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[3] = uiu.b[mlb];	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[4] = uiu.b[mhb] & 0x01;	 // Set board call latching mask
					}
				   	else if (devnmb == c_mrcan_SEB_7)
					{
						uiu.dw = rcc_mask[cons[carnmb]][0];
						mrcan_tx[mrc_tpix].data[2] = ((uiu.b[mhb] >> 1) | (uiu.b[hb] << 7));	 // Set board call latching mask
#if (Tract_HR == 1)
						mrcan_tx[mrc_tpix].data[3] = (uiu.b[hb] >> 1);	 // Set board call latching mask
						uiu.dw = rcc_mask[cons[carnmb]][1];
						mrcan_tx[mrc_tpix].data[3] |= (uiu.b[lb] << 7);	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[4] = (uiu.b[lb] >> 1) | (uiu.b[mlb] << 7);	 // Set board call latching mask
#else
						mrcan_tx[mrc_tpix].data[3] = ((uiu.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[4] = 0;	 // Set board call latching mask
#endif
					}
#if (Tract_HR == 1)
					else if (devnmb == c_mrcan_SEB_11)
					{
						uiu.dw = cc_mask[cons[carnmb]][1];
						mrcan_tx[mrc_tpix].data[2] = ((uiu.b[lb] >> 4) | (uiu.b[mlb] << 4));	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[3] = ((uiu.b[mlb] >> 4) | (uiu.b[mhb] << 4));	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[4] = ((uiu.b[mhb] >> 4) | (uiu.b[hb] << 4));	 // Set board call latching mask
					}
					else if (devnmb == c_mrcan_SEB_12)
					{
						uiu.dw = rcc_mask[cons[carnmb]][1];
						mrcan_tx[mrc_tpix].data[2] = ((uiu.b[mlb] >> 1) | (uiu.b[mhb] << 7));	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[3] = ((uiu.b[mhb] >> 1) | (uiu.b[hb] << 7));	 // Set board call latching mask
						mrcan_tx[mrc_tpix].data[4] = ((uiu.b[hb] >> 1) & 0x0f);	 // Set board call latching mask
					}
#endif
*/
				}
				else
				{
					mrcan_tx[mrc_tpix].data[2] = 0;	 // Set board call latching mask
					mrcan_tx[mrc_tpix].data[3] = 0;	 // Set board call latching mask
					mrcan_tx[mrc_tpix].data[4] = 0;	 // Set board call latching mask
				}

				mrcan_tx[mrc_tpix].data[5] = 0;
				mrcan_tx[mrc_tpix].data[6] = 0;
				mrcan_tx[mrc_tpix].data[7] = 0;
				mrcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 13:		// I/O Board outputs
			if ((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_SEB_40))
			{
				mrcan_tx[mrc_tpix].rtr = 0;
				mrcan_tx[mrc_tpix].srr = 1;
				mrcan_tx[mrc_tpix].len = 8;
				mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
				if (mrcan_dev[devnmb].online == 1)
					mrcan_tx[mrc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					mrcan_tx[mrc_tpix].one_tx = 1;
				mrcan_tx[mrc_tpix].msg = msg;
				mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
				mrcan_tx[mrc_tpix].id2 = 0;
				mrcan_tx[mrc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_mrcan_SEB_1);
				iodataix = (int16)((IODATA_MRCAN_START + iodev_ix) * 3); 		// input array
				mrcan_tx[mrc_tpix].data[0] = (uint8)((~serial_mrc.dev[iodev_ix].iob[0] & IO_In_Out[iodataix]));
				mrcan_tx[mrc_tpix].data[1] = (uint8)((~serial_mrc.dev[iodev_ix].iob[1] & IO_In_Out[iodataix+1]));
				mrcan_tx[mrc_tpix].data[2] = (uint8)((~serial_mrc.dev[iodev_ix].iob[2] & IO_In_Out[iodataix+2]));
				if (cons[RGB_CC] == 1)
				{
					uwu.w = CC_ColorOnOffChecksum;
					mrcan_tx[mrc_tpix].data[3] = uwu.b[lbw];
					mrcan_tx[mrc_tpix].data[4] = uwu.b[hbw];
					uwu.w = mrcan_dev[devnmb].ioname_chksum;
					mrcan_tx[mrc_tpix].data[5] = uwu.b[lbw];
					mrcan_tx[mrc_tpix].data[6] = uwu.b[hbw];
					mrcan_tx[mrc_tpix].data[7] = mrcan_dev[devnmb].io_color_ix_chksum;
				}
				else
				{
					mrcan_tx[mrc_tpix].data[3] = 0;
					mrcan_tx[mrc_tpix].data[4] = 0;
					uwu.w = mrcan_dev[devnmb].ioname_chksum;
					mrcan_tx[mrc_tpix].data[5] = uwu.b[lbw];
					mrcan_tx[mrc_tpix].data[6] = uwu.b[hbw];
					mrcan_tx[mrc_tpix].data[7] = 0;
				}
				mrcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 15:		// I/O Board inputs sent back to the can serial i/o board
			if (((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_END)))
			{
				mrcan_tx[mrc_tpix].rtr = 0;
				mrcan_tx[mrc_tpix].srr = 1;
				mrcan_tx[mrc_tpix].len = 3;
				mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
				if (mrcan_dev[devnmb].online == 1)
					mrcan_tx[mrc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					mrcan_tx[mrc_tpix].one_tx = 1;
				mrcan_tx[mrc_tpix].msg = msg;
				mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
				mrcan_tx[mrc_tpix].id2 = 0;
				mrcan_tx[mrc_tpix].id3 = 0;
				iodev_ix = (int16)(devnmb-c_mrcan_SEB_1);
				iodataix = (int16)((IODATA_MRCAN_START + iodev_ix) * 3); 		// input array
				mrcan_tx[mrc_tpix].data[0] = (uint8)(serial_mrc.dev[iodev_ix].iob[0] & ~IO_In_Out[iodataix]);
				mrcan_tx[mrc_tpix].data[1] = (uint8)(serial_mrc.dev[iodev_ix].iob[1] & ~IO_In_Out[iodataix+1]);
				mrcan_tx[mrc_tpix].data[2] = (uint8)(serial_mrc.dev[iodev_ix].iob[2] & ~IO_In_Out[iodataix+2]);
				mrcan_tx[mrc_tpix].data[3] = 0;
				mrcan_tx[mrc_tpix].data[4] = 0;
				mrcan_tx[mrc_tpix].data[5] = 0;
				mrcan_tx[mrc_tpix].data[6] = 0;
				mrcan_tx[mrc_tpix].data[7] = 0;
				mrcan_dev[devnmb].tx_cnt++;
			}
			break;
		case 21:		// Load weigher command
			if (devnmb == c_mrcan_LW)
			{
#if ((Traction == 1) || (Tract_OL == 1))
				mrcan_tx[mrc_tpix].rtr = 0;
				mrcan_tx[mrc_tpix].srr = 1;
				mrcan_tx[mrc_tpix].len = 1;
				mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
				if (mrcan_dev[devnmb].online == 1)
					mrcan_tx[mrc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					mrcan_tx[mrc_tpix].one_tx = 1;
				mrcan_tx[mrc_tpix].msg = msg;
				mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
				mrcan_tx[mrc_tpix].id2 = 0;
				mrcan_tx[mrc_tpix].id3 = 0;
				mrcan_tx[mrc_tpix].data[0] = lw_command;
				mrcan_tx[mrc_tpix].data[1] = 0;
				mrcan_tx[mrc_tpix].data[2] = 0;
				mrcan_tx[mrc_tpix].data[3] = 0;
				mrcan_tx[mrc_tpix].data[4] = 0;
				mrcan_tx[mrc_tpix].data[5] = 0;
				mrcan_tx[mrc_tpix].data[6] = 0;
				mrcan_tx[mrc_tpix].data[7] = 0;
				mrcan_dev[devnmb].tx_cnt++;
#endif
			}
			break;
#if (Traction == 1) 
		case 23:		// Selector/SPB position init command
			if (devnmb == c_mrcan_SPB) 
			{
				mrcan_tx[mrc_tpix].rtr = 0;
				mrcan_tx[mrc_tpix].srr = 1;
				mrcan_tx[mrc_tpix].len = 5;
				mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
				if (mrcan_dev[devnmb].online == 1)
					mrcan_tx[mrc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
				else
					mrcan_tx[mrc_tpix].one_tx = 1;
				mrcan_tx[mrc_tpix].msg = msg;
				mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
				mrcan_tx[mrc_tpix].id2 = 0;
				mrcan_tx[mrc_tpix].id3 = 0;
				uiu.dw = SEL_Preset_Count;
				mrcan_tx[mrc_tpix].data[0] = 1;		// preset count flag
				mrcan_tx[mrc_tpix].data[1] = uiu.b[lb];
				mrcan_tx[mrc_tpix].data[2] = uiu.b[mlb];
				mrcan_tx[mrc_tpix].data[3] = uiu.b[mhb];
				mrcan_tx[mrc_tpix].data[4] = uiu.b[hb];
				mrcan_tx[mrc_tpix].data[5] = 0;
				mrcan_tx[mrc_tpix].data[6] = 0;
				mrcan_tx[mrc_tpix].data[7] = 0;
				mrcan_dev[devnmb].tx_cnt++;
			}
			break;
#endif
		case 25:		// PI and Lantern message
			if (devnmb == c_mrcan_PI_LANT)
			{
				mrcan_tx[mrc_tpix].rtr = 0;
				mrcan_tx[mrc_tpix].srr = 1;
				mrcan_tx[mrc_tpix].len = 8;
				mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
				mrcan_tx[mrc_tpix].one_tx = 0;		
				mrcan_tx[mrc_tpix].msg = msg;
				mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[c_mrcan_PI_LANT];
				mrcan_tx[mrc_tpix].id2 = 0;
				mrcan_tx[mrc_tpix].id3 = 0;
#if (Traction == 1)
				t_blind = blind();
#endif
				if (cons[Xref_PI_Pos] == 1)
					mrcan_tx[mrc_tpix].data[0] = xpi_pos[pi_pos];
#if (Traction == 1)
				else if (t_blind == 1)
					mrcan_tx[mrc_tpix].data[0] = (uint8)(cons[topf] + 1);
				else if (t_blind == 2)
					mrcan_tx[mrc_tpix].data[0] = (uint8)(cons[topf] + 2);
#endif
				else
					mrcan_tx[mrc_tpix].data[0] = pi_pos;

				if ((servf == s_SABBATH) && ((fvars[fvsabbdis] & 0x01) != 0))
				{
					mrcan_tx[mrc_tpix].data[1] = 'S';
					mrcan_tx[mrc_tpix].data[2] = 'B';
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
							mrcan_tx[mrc_tpix].data[1] = alt_flmrk[pi_pos][0];
							mrcan_tx[mrc_tpix].data[2] = alt_flmrk[pi_pos][1];
						}
						else
						{
							timers[taltpi_off] = 0;
							mrcan_tx[mrc_tpix].data[1] = flmrk[pi_pos][0];
					  		mrcan_tx[mrc_tpix].data[2] = flmrk[pi_pos][1];
						}
					}
					else {
							timers[taltpi_off] = 0;
							timers[taltpi_on] = 0;
#if (Traction == 1)
							if ((t_blind == 1) && (cons[Blind_Floor] != 0))
							{
								mrcan_tx[mrc_tpix].data[1] = Blind_flmrk[0][0];
								mrcan_tx[mrc_tpix].data[2] = Blind_flmrk[0][1];
							}
							else if ((t_blind == 2) && (cons[Blind_Floor_2] != 0))
							{
								mrcan_tx[mrc_tpix].data[1] = Blind_flmrk[1][0];
								mrcan_tx[mrc_tpix].data[2] = Blind_flmrk[1][1];
							}
							else
#endif
							{
							
								mrcan_tx[mrc_tpix].data[1] = flmrk[pi_pos][0];
								mrcan_tx[mrc_tpix].data[2] = flmrk[pi_pos][1];
							}
					}
				}
				else if ((cons[Alt_Ascii_pi] == 2) && (rdinp(i_ALTPI)) && (firef == 0) && (rfiref == 0))
				{
					mrcan_tx[mrc_tpix].data[1] = alt_flmrk[pi_pos][0];
					mrcan_tx[mrc_tpix].data[2] = alt_flmrk[pi_pos][1];
				}
				else 
				{
#if (Traction == 1)
					if ((t_blind == 1) && (cons[Blind_Floor] != 0))
					{
						mrcan_tx[mrc_tpix].data[1] = Blind_flmrk[0][0];
						mrcan_tx[mrc_tpix].data[2] = Blind_flmrk[0][1];
					}
					else if ((t_blind == 2) && (cons[Blind_Floor_2] != 0))
					{
						mrcan_tx[mrc_tpix].data[1] = Blind_flmrk[1][0];
						mrcan_tx[mrc_tpix].data[2] = Blind_flmrk[1][1];
					}
					else
#endif
					{
						mrcan_tx[mrc_tpix].data[1] = flmrk[pi_pos][0];
						mrcan_tx[mrc_tpix].data[2] = flmrk[pi_pos][1];
					}
				}
				if((alrm_pi_stat == 1) && (timers[tsec] != 0))
				{
					mrcan_tx[mrc_tpix].data[1] = 'A';
					mrcan_tx[mrc_tpix].data[2] = 'L';
				}
				
				

				mrcan_tx[mrc_tpix].data[3] = pi_lant_stat;
				mrcan_tx[mrc_tpix].data[4] = pi_motion_stat;
				mrcan_tx[mrc_tpix].data[5] = pi_fire_stat;
				mrcan_tx[mrc_tpix].data[6] = pi_door_stat;
				mrcan_tx[mrc_tpix].data[7] = pi_misc_stat;
			}
			break;

		case 47:		// Send I/O Board Label
			if ((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_SEB_40))
			{
				if (mrcan_dev[devnmb].label_req != 0)
				{
					iodev_ix = (int16)(devnmb-c_mrcan_SEB_1);
					label_ix = mrcan_dev[devnmb].label_ix;
					array_start = (int16)((IODATA_MRCAN_START + iodev_ix) * 3); 		// input array
					io_board = (int16)(IODATA_MRCAN_START + iodev_ix);
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
					mrcan_tx[mrc_tpix].rtr = 0;
					mrcan_tx[mrc_tpix].srr = 1;
					mrcan_tx[mrc_tpix].len = 8;
					mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
					if (mrcan_dev[devnmb].online == 1)
						mrcan_tx[mrc_tpix].one_tx = 0;	   // ok to retransmit since device is on line
					else
						mrcan_tx[mrc_tpix].one_tx = 1;
					mrcan_tx[mrc_tpix].msg = msg;
					mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
					mrcan_tx[mrc_tpix].id2 = 0;
					mrcan_tx[mrc_tpix].id3 = 0;
					mrcan_tx[mrc_tpix].data[0] = label_ix;
					io_name_start = (array_start * 8) + k;
					if(valid_name == 1)
					{
						mrcan_tx[mrc_tpix].data[1] = ioname[io_name_start + label_ix][0];
						mrcan_tx[mrc_tpix].data[2] = ioname[io_name_start + label_ix][1];
						mrcan_tx[mrc_tpix].data[3] = ioname[io_name_start + label_ix][2];
						mrcan_tx[mrc_tpix].data[4] = ioname[io_name_start + label_ix][3];
						mrcan_tx[mrc_tpix].data[5] = ioname[io_name_start + label_ix][4];
					}
					else
					{
						mrcan_tx[mrc_tpix].data[1] = ' ';				
						mrcan_tx[mrc_tpix].data[2] = ' ';				
						mrcan_tx[mrc_tpix].data[3] = ' ';				
						mrcan_tx[mrc_tpix].data[4] = ' ';				
						mrcan_tx[mrc_tpix].data[5] = ' ';				
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
						mrcan_tx[mrc_tpix].data[6] = color_on_ix;		// On State Color
						mrcan_tx[mrc_tpix].data[7] = color_off_ix;	    // Off State Color
					}
					else
					{
						mrcan_tx[mrc_tpix].data[6] = 0;		// On State Color
						mrcan_tx[mrc_tpix].data[7] = 0;	    // Off State Color
					}
					
					label_ix ++;
					mrcan_dev[devnmb].label_ix = label_ix;
					if (label_ix <= 11)
					{
						mrcan_put_dev_msg_req(devnmb, 47);
					}
					else
					{
						mrcan_dev[devnmb].label_ix = 0;
						mrcan_dev[devnmb].label_req = 0;
					}
					mrcan_dev[devnmb].tx_cnt++;
				}
			}
			break;
			
		case 49:			// Message 49: Reset Error Message
			mrcan_tx[mrc_tpix].rtr = 0;
			mrcan_tx[mrc_tpix].srr = 1;
			mrcan_tx[mrc_tpix].len = 1;
			mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
			mrcan_tx[mrc_tpix].one_tx = 0;	   // ok to retransmit 
			mrcan_tx[mrc_tpix].msg = msg;
			mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[devnmb];
			mrcan_tx[mrc_tpix].id2 = 0;
			mrcan_tx[mrc_tpix].id3 = 0;
			mrcan_tx[mrc_tpix].data[0] = 0x03;		// Control byte - bit0: reset error , bit 1: clear error buffer
			mrcan_tx[mrc_tpix].data[1] = 0;
			mrcan_tx[mrc_tpix].data[2] = 0;	 		 
			mrcan_tx[mrc_tpix].data[3] = 0;	 
			mrcan_tx[mrc_tpix].data[4] = 0;	 
			mrcan_tx[mrc_tpix].data[5] = 0;
			mrcan_tx[mrc_tpix].data[6] = 0;
			mrcan_tx[mrc_tpix].data[7] = 0;
			mrcan_dev[devnmb].tx_cnt++;
			break;

		case 100:		// Destination Car Call display data
			if (devnmb == c_mrcan_PI_LANT)
			{
				pi_mrcan_location = 1;
				mrcan_tx[mrc_tpix].rtr = 0;
				mrcan_tx[mrc_tpix].srr = 1;
				mrcan_tx[mrc_tpix].len = 6;
				mrcan_tx[mrc_tpix].ex_id = 1;		// Extended Id
				mrcan_tx[mrc_tpix].one_tx = 0;		
				mrcan_tx[mrc_tpix].msg = msg;
				mrcan_tx[mrc_tpix].addr = (uint8)mrcan_addr[c_mrcan_PI_LANT];
				mrcan_tx[mrc_tpix].id2 = 0;
				mrcan_tx[mrc_tpix].id3 = 0;
  				mrcan_tx[mrc_tpix].data[0] = pi_cc_mrcan_command;
				mrcan_tx[mrc_tpix].data[1] = pi_cc_mrcan_floor;
				if ((pi_cc_command & 0x20) != 0)
				{						   // Extra ascii used for rear floor
					mrcan_tx[mrc_tpix].data[2] = 'R';	   	// send an R for the rear doors
					mrcan_tx[mrc_tpix].data[3] = flmrk[pi_cc_mrcan_floor][0];
					mrcan_tx[mrc_tpix].data[4] = flmrk[pi_cc_mrcan_floor][1];
				}
				else
				{
					mrcan_tx[mrc_tpix].data[2] = flmrk[pi_cc_mrcan_floor][0];
					mrcan_tx[mrc_tpix].data[3] = flmrk[pi_cc_mrcan_floor][1];
					mrcan_tx[mrc_tpix].data[4] = ' ';  		// send a space if only using two ascii characters
				}
				mrcan_tx[mrc_tpix].data[5] = pi_mrcan_location;	 
				mrcan_tx[mrc_tpix].data[6] = 0;
				mrcan_tx[mrc_tpix].data[7] = 0;
				pi_cc_mrcan_cnt++;
				mrcan_dev[devnmb].tx_cnt++;
			}
			break;

   		default:
			break;
	}
}

//************************************************************
//*********  Transmit Can Message  ***************************
//************************************************************

void tx_mrcan_msg(int16 TxBufIx)
{
	_disable();
	if (mrc_tgix != mrc_tpix)
	{
		
		switch (TxBufIx)
		{
			
			case 8:
				if (((MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (mrcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (MR_CAN.IFLAG1.B.BUF08I == 1)
						MR_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
					mrcan_transmit(TxBufIx);
					MR_CAN.IFLAG1.B.BUF08I = 1;		// Reset IF
					mrcan_tx_B08_fail_cnt = 0;
					break;
				}
				else
				{
					mrcan_tx_B08_fail_cnt++;
					if (mrcan_tx_B08_fail_cnt > 5)
					{
						mrcan_tx_complete[TxBufIx] = 1;
						mrcan_tx_B08_fail_cnt = 0;
						MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
					 
			case 9:
				if (((MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (mrcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (MR_CAN.IFLAG1.B.BUF09I == 1)
						MR_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
					mrcan_transmit(TxBufIx);
					MR_CAN.IFLAG1.B.BUF09I = 1;		// Reset IF
					mrcan_tx_B09_fail_cnt = 0;
					break;
				}
				else
				{
					mrcan_tx_B09_fail_cnt++;
					if (mrcan_tx_B09_fail_cnt > 5)
					{
						mrcan_tx_complete[TxBufIx] = 1;
						mrcan_tx_B09_fail_cnt = 0;
						MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			case 10:
				if (((MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (mrcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (MR_CAN.IFLAG1.B.BUF10I == 1)
						MR_CAN.IFLAG1.B.BUF10I = 1;		// Reset IF
					mrcan_transmit(TxBufIx);
					MR_CAN.IFLAG1.B.BUF10I = 1;		// Reset IF
					mrcan_tx_B10_fail_cnt = 0;
					break;
				}
				else
				{
					mrcan_tx_B10_fail_cnt++;
					if (mrcan_tx_B10_fail_cnt > 5)
					{
						mrcan_tx_complete[TxBufIx] = 1;
						mrcan_tx_B10_fail_cnt = 0;
						MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
					TxBufIx++;		
				}
			case 11:
				if (((MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x08) || (MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x04) || 
					(MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x0A) || (MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x09) ||
					(MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE == 0x00)) && (mrcan_tx_complete[TxBufIx] == 1))
				{		// The transmitter is not busy
					if (MR_CAN.IFLAG1.B.BUF11I == 1)
						MR_CAN.IFLAG1.B.BUF11I = 1;		// Reset IF
					mrcan_transmit(TxBufIx);
					MR_CAN.IFLAG1.B.BUF11I = 1;		// Reset IF
					mrcan_tx_B11_fail_cnt = 0;
					break;
				}
				else
				{
					mrcan_tx_B11_fail_cnt++;
					if (mrcan_tx_B11_fail_cnt > 5)
					{
						mrcan_tx_complete[TxBufIx] = 1;
						mrcan_tx_B11_fail_cnt = 0;
						MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x09;		// write abort code
					}
				}
			default:
				break;
		}
	}
	_enable();
}


//************************************************************
//*********  Transmit Buffer  ********************************
//************************************************************

void mrcan_transmit (int16 TxBufIx)
{
	int16 i;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
	if (mrc_tgix != mrc_tpix)
	{		// There is data in the buffer 

		mrcan_tx_complete[TxBufIx] = 0;

		if (mrc_tgix < c_mrcan_tx_buf_siz)
			mrc_tgix++;
		else
			mrc_tgix = 0;

		if (mrcan_tx[mrc_tgix].len > 8)
		{
			mrcan_tx[mrc_tgix].len = 8;
			mrcan_error |= 0x40;
		}
			

		if (mrcan_tx[mrc_tgix].ex_id == 1)
		{
			uiu.b[hb] = mrcan_tx[mrc_tgix].msg;		// Message
			uiu.b[mhb] = mrcan_tx[mrc_tgix].addr;	// Address
			uiu.b[mlb] = 0;
			uiu.b[lb] = 0;
		 	MR_CAN.MB[TxBufIx].MSG.MID.R = uiu.dw >> 3;		// priority bits get set to zero on shift
		}
		else
		{
			uiu.b[hb] = (uint8)(mrcan_tx[mrc_tgix].msg & 0x3F);		// Message
			uiu.b[mhb] = (uint8)(mrcan_tx[mrc_tgix].addr << 3);	// Address
			uiu.b[mlb] = 0;
			uiu.b[lb] = 0;
		 	MR_CAN.MB[TxBufIx].MSG.MID.R = uiu.dw >> 3;		// priority bits get set to zero on shift
		}


		i = 0;
		while ((i<mrcan_tx[mrc_tgix].len) && (i < 8))
		{
		 	MR_CAN.MB[TxBufIx].MSG.MDATA.B[i++] = mrcan_tx[mrc_tgix].data[i];
		}
		

		MR_CAN.MB[TxBufIx].MSG.MCS.B.LENGTH = mrcan_tx[mrc_tgix].len;
	 	
		if (mrcan_tx[mrc_tgix].rtr == 1)
			MR_CAN.MB[TxBufIx].MSG.MCS.B.RTR = 1;
	 	else
			MR_CAN.MB[TxBufIx].MSG.MCS.B.RTR = 0;

		if (mrcan_tx[mrc_tgix].ex_id == 1)
			MR_CAN.MB[TxBufIx].MSG.MCS.B.IDE = 1;
	 	else
			MR_CAN.MB[TxBufIx].MSG.MCS.B.IDE = 0;

		if (mrcan_tx[mrc_tgix].srr == 1)
			MR_CAN.MB[TxBufIx].MSG.MCS.B.SRR = 1;
	 	else
			MR_CAN.MB[TxBufIx].MSG.MCS.B.SRR = 0;
	 	
	 	
		MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE = 0x0C;		// Initiate transmit
		mrcan_debug_1 = MR_CAN.MB[TxBufIx].MSG.MCS.B.CODE;
	}
}

//************************************************************
//*********  Process the Can Serial Data *********************
//************************************************************

void mrcan_control (void)
{
	static int16 ix;
	int16 i;
	unsigned char msg_req = 0;

	unsigned char devix,iodevix,devnmb,tdevix;
	uint16 iodataix;

	if (nmb_mrcan_dev > 0)
	{
		unpack_mrcan_in();


		if (re_init_mrcan == 1)
		{
			re_init_mrcan = 0;
//			record_fault(f_mrcan_init_error);
//			init_mrcan_bus();
		}

		mrcan_status = MR_CAN.ESR.R;
		if (MR_CAN.ESR.B.ERRINT == 1)
		{
			mrcan_status = MR_CAN.ESR.R;
			MR_CAN.ESR.B.ERRINT = 1;
		}
		else
			mrcan_status = 0;
		
		mrcan_tx_err_cnt = (uint8)MR_CAN.ECR.B.TXECNT;
		mrcan_rx_err_cnt = (uint8)MR_CAN.ECR.B.RXECNT;

		if (MR_CAN.ESR.B.BOFFINT == 1)
		{		// The CAN bus is off
			MR_CAN.ESR.B.BOFFINT = 1;
			mrcan_bus_off_cnt++;
			mrcan_error |= 0x04;
			init_mrcan_bus();
//			record_fault(f_mrcan_bus_off);
		}
		else if (mrcan_bus_off_cnt > 0)
			mrcan_bus_off_cnt--;

#if (Simulator == 0)
		if ((mrcan_status & 0x00000080) != 0)
		{		// Bus is idle
			mrcan_bus_idle_cnt++;
//			mrcan_error |= 0x08;
//			record_fault(f_mrcan_bus_idle);		// bus can go idle while car sitting at the floor
		}
		else if (mrcan_bus_idle_cnt > 0)
			mrcan_bus_idle_cnt--;

		if ((mrcan_status & 0x00002000) != 0)
		{		// Transmit acknowledge error
		
			mrcan_ack_error_cnt++;
			mrcan_error |= 0x10;
//			record_fault(f_mrcan_ack_err);	
		}
		else if (mrcan_ack_error_cnt > 0)
			mrcan_ack_error_cnt--;
#endif


		if (IsMrcTxBufFul() == 0)
		{	// buffer is not full	  OK to write to buffer
			set_pi_lant_status();
			set_dd_cc_mrcan_pi_dpy();
			if (((timers[tpilant_mrcan] > 2) || (upd_pi_lant_mrcan == 1)) && (mrcan_data_req == 0))	// 300 msec
			{
				mrcan_data_req = 1;
				upd_pi_lant_mrcan = 0;
				pack_mrcan_out(25,c_mrcan_PI_LANT,0);		// Pack the pi and lantern packet 
				timers[tpilant_mrcan] = 0;
			}

			if ((timers[tpicc_mrcan] > 2) && (pi_cc_mrcan_update == 1) && (mrcan_data_req == 0))	
			{
				if (pi_cc_mrcan_cnt < 2)
				{
					mrcan_data_req = 1;
					pack_mrcan_out(100,c_mrcan_PI_LANT,0);		// Pack the pi and lantern packet 
					timers[tpicc_mrcan] = 0;
				}
				else
				{
					pi_cc_mrcan_cnt = 0;
					pi_cc_mrcan_update = 0;
				}
			}
		}

		if (IsMrcTxBufFul() == 0)
		{	// buffer is not full	  OK to write to buffer
			if ((mrcan_msg_req != 0) && (mrcan_dev_req != 0) && (mrcan_msg_cmd != 0) && (mrcan_data_req == 0))
			{
				mrcan_data_req = 1;
				if ((((mrcan_dev_req >= c_mrcan_START) && (mrcan_dev_req <= c_mrcan_END)) || (mrcan_dev_req == c_mrcan_GLOBAL)) && (mrcan_msg_cmd <= 2))
				{
					pack_mrcan_out((uint8)mrcan_msg_req,(uint8)mrcan_dev_req,(uint8)mrcan_msg_cmd-1);		// Pack Message request 
				}
				if (mrcan_msg_cnt > 0)
					mrcan_msg_cnt--;
				if (mrcan_msg_cnt == 0)
				{
					mrcan_msg_req = 0;
					mrcan_dev_req = 0;
					mrcan_msg_cmd = 0;
				}
			}
		}	 
			
		// Communicate with Serial Devices and Expansion I/O boards
		i = 0;
		while ((i < nmb_mrcan_dev) && (IsMrcTxBufFul() == 0))
		{
			if (ix < (nmb_mrcan_dev-1))
				ix++;
			else
				ix = 0;
			
			devnmb = mrcan_dev_list[ix];	// get device number from list
			devix = devnmb;
			

			if (cons[loadw] == 3)
			{	// Request load weigher data when at floor and door is open
				if (devnmb == c_mrcan_LW)
				{
					msg_req = mrcan_get_dev_msg_req(devnmb);
					if (msg_req != 0)
					{
						pack_mrcan_out(msg_req,c_mrcan_LW,0);
						timers[tlwupd] = 0;
					}
					else if (((timers[tlwupd] >= 2) && (procf >= 14) && (procf <= 16)) ||	// 200 msec at the floor
							 ((timers[tlwupd] >= 5) && ((procf < 14) || (procf > 16))))		// Every 500 msec not at floor
					{		
						if (mrcan_data_req == 0)
						{
							mrcan_data_req = 1;
							timers[tlwupd] = 0;

							if (mrcan_dev[devix].version == 0)
								pack_mrcan_out(1,c_mrcan_LW,0);	// send init packet
							else
								pack_mrcan_out(21,c_mrcan_LW,0);	// request data read from load weigher					

							if (mrcan_dev[devix].online_cnt > 0)
								mrcan_dev[devix].online_cnt--;
							if (mrcan_dev[devix].online_cnt > 0)
							{
								lw_can_online = 1;
								mrcan_dev[devix].online = 1;
							}
							else
							{
								lw_can_online = 0;
								mrcan_dev[devix].online = 0;
							}
						}
					}
				}
			}
			else if (cons[loadw] != 2)
				lw_can_online = 0;
				
			if (cons[Voltage_Sensor] == 2)
			{
				if ((timers[tvsupd] >= 30) && (devnmb == c_mrcan_VS)) // every 3 seconds
				{	  // Request Voltage from voltage board
					if (mrcan_data_req == 0)
					{
						mrcan_data_req = 1;
						
						msg_req = mrcan_get_dev_msg_req(devnmb);
						if (msg_req != 0)
						{
							pack_mrcan_out(msg_req,c_mrcan_VS,0);
						}
						
						
						if (mrcan_dev[devix].version == 0)
							pack_mrcan_out(1,c_mrcan_VS,0);	// send init packet
						else
							pack_mrcan_out(44,c_mrcan_VS,1);	// data request controller voltage
						timers[tvsupd] = 0;;

#if (Simulator == 1)
						mrcan_dev[devix].online = 1;
						vs_can_online = 1;
#else
						if (mrcan_dev[devix].online_cnt > 0)
							mrcan_dev[devix].online_cnt--;
						if (mrcan_dev[devix].online_cnt > 0)
						{
							vs_can_online = 1;
							mrcan_dev[devix].online = 1;
						}
						else
						{
							vs_can_online = 0;
							mrcan_dev[devix].online = 0;
						}
#endif
					}
				}
			}
			else if (cons[Voltage_Sensor] == 0)
				vs_can_online = 0;
			

			// I/O Board Data

			if ((devnmb >= c_mrcan_SEB_1) && (devnmb <= c_mrcan_SEB_40))
			{		 // Valid device number
				iodevix = devnmb-c_mrcan_SEB_1;
				tdevix = devnmb - c_mrcan_SEB_1;
				iodataix = (uint16)((IODATA_MRCAN_START + iodevix) * 3); 		// input array

				// *********************** Output device *********************
					 
				if (((iodata[cons[carnmb]][iodataix] & IO_In_Out[iodataix]) != (serial_mrc.dev[iodevix].iob[0] & IO_In_Out[iodataix])) ||
					((iodata[cons[carnmb]][iodataix+1] & IO_In_Out[iodataix+1]) != (serial_mrc.dev[iodevix].iob[1] & IO_In_Out[iodataix+1])) ||
					((iodata[cons[carnmb]][iodataix+2] & IO_In_Out[iodataix+2]) != (serial_mrc.dev[iodevix].iob[2] & IO_In_Out[iodataix+2]))) 
				{		// io data for the output has changed
					serial_mrc.dev[iodevix].iob[0] = (uint8)((serial_mrc.dev[iodevix].iob[0] & ~IO_In_Out[iodataix]) | (iodata[cons[carnmb]][iodataix] & IO_In_Out[iodataix]));
					serial_mrc.dev[iodevix].iob[1] = (uint8)((serial_mrc.dev[iodevix].iob[1] & ~IO_In_Out[iodataix+1]) | (iodata[cons[carnmb]][iodataix+1] & IO_In_Out[iodataix+1]));
					serial_mrc.dev[iodevix].iob[2] = (uint8)((serial_mrc.dev[iodevix].iob[2] & ~IO_In_Out[iodataix+2]) | (iodata[cons[carnmb]][iodataix+2] & IO_In_Out[iodataix+2]));
					pack_mrcan_out(13,devnmb,0);	// Data has changed so pack the data
					timers[tmrcantx + tdevix] = 0;
					sync_dev_mrc[iodevix].rtx_cnt = 0;
				}
				else if (((sync_dev_mrc[iodevix].iob[0] & IO_In_Out[iodataix]) != (serial_mrc.dev[iodevix].iob[0] & IO_In_Out[iodataix])) ||
					((sync_dev_mrc[iodevix].iob[1] & IO_In_Out[iodataix+1]) != (serial_mrc.dev[iodevix].iob[1] & IO_In_Out[iodataix+1])) ||
					((sync_dev_mrc[iodevix].iob[2] & IO_In_Out[iodataix+2]) != (serial_mrc.dev[iodevix].iob[2] & IO_In_Out[iodataix+2]))) 
				{		// device has not updated it's outputs yet
					if (timers[tmrcantx + tdevix] >= 2)
					{
						pack_mrcan_out(13,devnmb,0);	// Data has changed so pack the data
						timers[tmrcantx + tdevix] = 0;
						if (sync_dev_mrc[iodevix].rtx_cnt > 10)
						{
							sync_dev_mrc[iodevix].iob[0] = (uint8)((sync_dev_mrc[iodevix].iob[0] & ~IO_In_Out[iodataix]) | (serial_mrc.dev[iodevix].iob[0] & IO_In_Out[iodataix]));
							sync_dev_mrc[iodevix].iob[1] = (uint8)((sync_dev_mrc[iodevix].iob[1] & ~IO_In_Out[iodataix+1]) | (serial_mrc.dev[iodevix].iob[1] & IO_In_Out[iodataix+1]));
							sync_dev_mrc[iodevix].iob[2] = (uint8)((sync_dev_mrc[iodevix].iob[2] & ~IO_In_Out[iodataix+2]) | (serial_mrc.dev[iodevix].iob[2] & IO_In_Out[iodataix+2]));
							sync_dev_mrc[iodevix].rtx_cnt = 0;
						}
						else
							sync_dev_mrc[iodevix].rtx_cnt++;
					}
				}
				else if ((timers[tmrcantx + tdevix] > 6) && (mrcan_data_req == 0))
				{		 // Update outputs
					mrcan_data_req = 1;
					serial_mrc.dev[iodevix].iob[0] = (uint8)((serial_mrc.dev[iodevix].iob[0] & ~IO_In_Out[iodataix]) | (iodata[cons[carnmb]][iodataix] & IO_In_Out[iodataix]));
					serial_mrc.dev[iodevix].iob[1] = (uint8)((serial_mrc.dev[iodevix].iob[1] & ~IO_In_Out[iodataix+1]) | (iodata[cons[carnmb]][iodataix+1] & IO_In_Out[iodataix+1]));
					serial_mrc.dev[iodevix].iob[2] = (uint8)((serial_mrc.dev[iodevix].iob[2] & ~IO_In_Out[iodataix+2]) | (iodata[cons[carnmb]][iodataix+2] & IO_In_Out[iodataix+2]));
					pack_mrcan_out(13,devnmb,0);	// Data has changed so pack the data
					timers[tmrcantx + tdevix] = 0;
					sync_dev_mrc[iodevix].rtx_cnt = 0;
				}
				
				// *****************  Input check **********************

				if (sync_dev_mrc[iodevix].reflect_data == 1)
				{		 // Send back what was sent so the device can verify it was sent ok
					sync_dev_mrc[iodevix].reflect_data = 0;	// Must be cleared before data is packed - if new data comes in during
					mrcan_debug_3++;
					pack_mrcan_out(15,devnmb,0);			//  the pack, it must be re-sent
				}
				else 
				{
					msg_req = mrcan_get_dev_msg_req(devnmb);
					if (msg_req != 0)
					{
						pack_mrcan_out(msg_req,devnmb,0);
					}
					else if (timers[tmrcanrx + tdevix] > 3)
					{
						if (mrcan_data_req == 0)
						{
							mrcan_data_req = 1;
							if (mrcan_dev[devix].version == 0)
							{
								pack_mrcan_out(11,devnmb,0);	// send init packet
								timers[tmrcanrx + tdevix] = 0;
								if (mrcan_dev[devix].online_cnt > 0)
									mrcan_dev[devix].online_cnt--;
							}
							else 
							{
								pack_mrcan_out(14,devnmb,1);	// data request
								timers[tmrcanrx + tdevix] = 0;
								if (mrcan_dev[devix].online_cnt > 0)
									mrcan_dev[devix].online_cnt--;
							}
						}
					}
				}
				if (mrcan_dev[devix].online_cnt > 0)
				{
					mrcan_dev[devix].online = 1;
				}
				else
				{

#if (Simulator == 0)
					if (mrcan_dev[devix].online == 1)
					{
						fault_device = mrcan_addr[devnmb];
//						record_fault(f_seb_mrcan_error);
					}
#endif
					mrcan_dev[devix].online = 0;
#if (Simulator == 0)
		 			serial_mrc.dev[iodevix].iob[0] &= IO_In_Out[iodataix];	  // clear input data since device is not on-line
		 			serial_mrc.dev[iodevix].iob[1] &= IO_In_Out[iodataix+1];
		 			serial_mrc.dev[iodevix].iob[2] &= IO_In_Out[iodataix+2];
#endif
				}
			}
			i++;		
		}
		MrcanTxBufIx ++;
		if (MrcanTxBufIx > 11)
			MrcanTxBufIx = 8;
		tx_mrcan_msg(MrcanTxBufIx);		// Check if data waiting and transmit
		if (mrcan_data_req == 0)
			mrcan_data_req_cnt = 0;
		else if (mrcan_data_req_cnt >= 1)
		{						// Cause a delay between sending packet request instead of sending all at once
			mrcan_data_req_cnt = 0;
			mrcan_data_req = 0;
		}
		else
			mrcan_data_req_cnt++;
	}
}


// set flag to send cc data to dd pi 

void set_dd_cc_mrcan_pi_dpy(void)
{

	int16 i;
	int16 fl_ix;
	
	if ((firef == 0) && (rfiref == 0) && (servf == s_AUTO))
	{  		// automatic service only
		
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			fl_ix = (i-1)/32;
			if ((c_cc_dpy[fl_ix] & fl_mask[i]) != (mr_pi_cc[fl_ix] & fl_mask[i]))
			{
				if (pi_cc_mrcan_update == 0)
				{
				 	pi_cc_mrcan_floor = (uint8)i;
				 	if ((c_cc_dpy[fl_ix] & fl_mask[i]) != 0)
						pi_cc_mrcan_command = 0x01;	   			// set car call
				 	else
				 		pi_cc_mrcan_command = 0x02;				// clear car call
					pi_cc_mrcan_update = 1;
					mr_pi_cc[fl_ix] &= ~fl_mask[i];
					mr_pi_cc[fl_ix] |= c_cc_dpy[fl_ix] & fl_mask[i];
					break;
				}
			}
			if ((c_rcc_dpy[fl_ix] & fl_mask[i]) != (mr_pi_rcc[fl_ix] & fl_mask[i]))
			{
				if (pi_cc_mrcan_update == 0)
				{
				 	pi_cc_mrcan_floor = (uint8)i;
				 	if ((c_rcc_dpy[fl_ix] & fl_mask[i]) != 0)
						pi_cc_mrcan_command = 0x11;				// set rear car call
				 	else
				 		pi_cc_mrcan_command = 0x12;				// clear rear car call
					pi_cc_mrcan_update = 1;
					mr_pi_rcc[fl_ix] &= ~fl_mask[i];
					mr_pi_rcc[fl_ix] |= c_rcc_dpy[fl_ix] & fl_mask[i];
					break;
				}
			}
		}
	}
}




/* Revision History
6/21/12 v6.0.9 mhd	1. Created new can bus
9/10/2013 v7.0.19 mhd	1. Send the voltage phase to the spb for the brake voltage in the msb of the line voltage.
9/30/13 v7.0.21 mhd		1. Modified pack routine to send multiple eeprom variables for both the SPB and NTS processors.
11/12/13 v7,0.28 mhd	1. Replaced iodata[1] with iodata[cons[carnmb]] where necessary.
12/3/13 v7,0,30 mhd		1. Update spb command after 700 msec all the time.  If command update occurs when timers[tspbupd] >= 4, reset the timers[tspbupd] = 2 to allow for a delay.
12/30/13 v7.0.31 mhd	1. Changed availbuf check in IsMrcTxBufFul() from > 4 to > 9, Also changed c_mrcan_tx_buf_siz and c_mrcan_rx_buf_siz from 20 to 30.
						2. Moved _disable() and _enable() routines in tx_can_msg() to have interrupts disable during the entire routine. If the mrc_tgix and mrc_tpix indexes are checked or the 
						   (CAN_A.MB[TxBufIx].MSG.MCS.B.CODE) transmit status is checked and then a transmit interrupt occurs, a transmit() routine can be called when the transmitter is not ready
						   causing an invalid packet. 
						3. Added mrcan_tx_complete flag to prevent the tx_mrcan_msg() routine from writing over the buffer of a partially transmitted packet.
						4. Added data_req check for pi/lantern packet. 
						5. Added data_req check for I/O board timer update outputs.
						6. Rotate buffer index when calling tx_mrcan_msg(index).
						7, In packet 47, do not send label if name is not valid.
4/8/15 v7.1.30 mhd		1. Clear NTS EM SD flag (NTS_can.command &= ~ntsc_Set_EM_SD) when command returned with the bit set in packet 16.
						2. Clear SPB update timer when packets 2 and 3 received because there is no need to request a packet that you just received.
10/9/15 v7.1.43 mhd		1. Added can message request que.
12/3/15 v7.1.51 mhd		1. Change the mrcan_data_req_cnt check from 4 to 1.
12/30/15 V7.1.56 mhd	1. Send reduced stroke buffer and rear door flags to SPB in packet 1.

*/
