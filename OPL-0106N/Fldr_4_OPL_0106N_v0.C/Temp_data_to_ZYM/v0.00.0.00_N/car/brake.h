// Prototypes for brake.cpp
// pick brake for each command: cmd 0: pick brake during run
//                              cmd 1: pick brake during relevel

#define c_max_brake_trigger 38400


#define c_pick_brake_run 0
#define c_pick_brake_relevel 1
extern void pick_brake(int cmd);

// Drop brake for each command: cmd 0: drop brake timed stop
//                              cmd 1: reset brake i/o
//								cmd 2: drop brake emergency stop

#define c_drop_brake_timed 0
#define c_drop_brake_reset 1
#define c_drop_brake_estop 2
extern void drop_brake(int cmd);

extern void brk_iotest(void);

extern void em_brake_control(int16 start_ck);

// Electronic Brake control variables

extern unsigned int Brake_Voltage;					// Brake voltage
extern unsigned int Return_Brake_Voltage;			// Brake voltage command returned from the board
extern unsigned int Prev_Brake_Voltage;			
extern unsigned int Brk_Trig_Tim;					// Brake trigger time
extern unsigned int Return_Brk_Trig_Tim;			// Brake trigger time reflected from the brake board
extern unsigned int Prev_Brk_Trig_Tim; 				// Brake trigger time
extern unsigned int Brake_Pick_Trigger_Time;		// Brake Pick trigger time
extern unsigned int Brake_Pick_Start_Trigger;		// Brake Pick start trigger time
extern unsigned int Brake_Hold_Trigger_Time;		// Brake hold trigger time
extern unsigned int Brake_Relevel_Trigger_Time;		// Brake relevel trigger time
extern unsigned int Brake_Relevel_Start_Trigger;	// Brake relevel start trigger time
extern unsigned int Brake_Drop_Trigger_Time;		// Brake drop trigger time
extern int ins_brakef;								// Flag to pick brake on inspection
extern unsigned char Brk_Ctl;			// Brake Control variable
						 			// 			Bit 0: 1=Turn on Brake Voltage
						 			// 			Bit 1,2,3:  Brake status request (Current, Voltage In, Voltage Out, I/O, etc)
									//			Bit 4,5: Unused
									//			Bit 6: 1 = Pick on Run
									// 			Bit 7: 1 = Pick on Relevel
#define c_Pick_Brake 0x01   		// Command to pick brake
#define c_Pick_Run 0x40
#define c_Pick_Relevel 0x80

extern unsigned char Return_Brk_Ctl;	// Brake control reflected from the brake board
extern unsigned char Prev_Brk_Ctl;		// Brake Control variable

extern unsigned char brake_data_req;	// data request number for brake data (see Brake Status)

extern unsigned char Brk_Stat;			// Brake Status
						 				// 			Bit 0: 1=Brake Fault
							 			// 			Bits 3,2,1:
							 			// 			  	 0,0,0	- Current
										//				 0,0,1	- Output Voltage
										//				 0,1,0 	- Input Voltage
										//				 0,1,1	- Fault Data
										//				 1,0,0  - Jumper Setting
										//			Bit3:	Brake Switch 1
										//			Bit4:	Brake Switch 2
										//			Bit5:	Brake Relay Bypass
										//			Bit6:	Unused
										//			Bit7:	Unused

extern unsigned int Brk_Amp;				// Brake Current
extern unsigned int Brk_Volt_In;			// Brake Voltage input
extern unsigned int Brk_Volt_Out;			// Brake Voltage Output
extern unsigned int Brk_Carrier_Freq;		// Brake Carrier Frequency
extern unsigned int Brk_Jumpers;			// Brake Jumper Setting
extern unsigned int Brk_Fault;				// Brake Fault Code
extern unsigned int Prev_Brk_Fault;			// Previous brake fault code
extern unsigned char Brk_Sw_1;				// Brake Switch 1
extern unsigned char Brk_Sw_2;				// Brake Switch 2
extern unsigned char Brk_Relay_Byp;			// Brake Relay Bypass Switch


// Emergency brake variables

extern unsigned int Em_Brake_Voltage;		  		// Brake voltage
extern unsigned int Return_Em_Brake_Voltage;  		// Brake voltage command returned from the board
extern unsigned int Prev_Em_Brake_Voltage;	  	
extern unsigned int Em_Brk_Trig_Tim;		  		// Brake trigger time
extern unsigned int Return_Em_Brk_Trig_Tim;	  		// Brake trigger time reflected from the brake board
extern unsigned int Prev_Em_Brk_Trig_Tim;
extern unsigned int Em_Brake_Pick_Trigger_Time;		// Brake Pick trigger time
extern unsigned int Em_Brake_Hold_Trigger_Time;		// Brake hold trigger time
extern unsigned char Em_Brk_Ctl;			// Brake Control variable
						 			// 			Bit 0: 1=Turn on Brake Voltage
						 			// 			Bit 1,2,3:  Brake status request (Current, Voltage In, Voltage Out, I/O, etc)
									//			Bit 4,5: Unused
									//			Bit 6: 1 = Pick on Run
									// 			Bit 7: 1 = Pick on Relevel
extern unsigned char Return_Em_Brk_Ctl;		// Brake control command reflected from the brake board
extern unsigned char Prev_Em_Brk_Ctl;
extern unsigned char em_brake_data_req;	// data request number for brake data (see Brake Status)

extern unsigned char Em_Brk_Stat;			// Brake Status
						 			// 			Bit 0: 1=Brake Fault
						 			// 			Bits 3,2,1:
						 			// 			  	 0,0,0	- Current
									//				 0,0,1	- Output Voltage
									//				 0,1,0 	- Input Voltage
									//				 0,1,1	- Fault Data
									//				 1,0,0  - Jumper Setting
									//			Bit3:	Brake Switch 1
									//			Bit4:	Brake Switch 2
									//			Bit5:	Brake Relay Bypass
									//			Bit6:	Unused
									//			Bit7:	Unused

extern unsigned int Em_Brk_Amp;				// Brake Current
extern unsigned int Em_Brk_Volt_In;			// Brake Voltage input
extern unsigned int Em_Brk_Volt_Out;			// Brake Voltage Output
extern unsigned int Em_Brk_Carrier_Freq;		// Brake Carrier Frequency
extern unsigned int Em_Brk_Jumpers;			// Brake Jumper Setting
extern unsigned int Em_Brk_Fault;			// Brake Fault Code
extern unsigned int Prev_Em_Brk_Fault;		// Previous brake fault code
