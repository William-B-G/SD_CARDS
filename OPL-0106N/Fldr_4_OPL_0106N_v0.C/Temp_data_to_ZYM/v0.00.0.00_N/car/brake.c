// this file contains all the Brake routines

#define d_BRAKE 1
#include "global.h"

void pick_brake(int cmd);
void drop_brake(int cmd);
void brk_iotest(void);
void em_brake_control(int16 start_ck);
void em_brake_drop(void);
void em_brake_pick(void);

#define c_max_brake_trigger 38400

// Drop brake for each command: cmd 0: drop brake timed
//                              cmd 1: reset brake i/o
//								cmd 2: drop brake emergency stop

#define c_drop_brake_timed 0
#define c_drop_brake_reset 1
#define c_drop_brake_estop 2

// Electronic Brake control variables

unsigned int Brake_Voltage=0;					// Brake voltage
unsigned int Return_Brake_Voltage=0;			// Brake voltage command returned from the board
unsigned int Prev_Brake_Voltage = 0;			
unsigned int Brk_Trig_Tim=0;					// Brake trigger time
unsigned int Return_Brk_Trig_Tim=0;				// Brake trigger time reflected from the brake board
unsigned int Prev_Brk_Trig_Tim=0;
unsigned int Brake_Pick_Trigger_Time = 0;		// Brake Pick trigger time
unsigned int Brake_Pick_Start_Trigger = 0;		// Brake Pick start trigger time
unsigned int Brake_Hold_Trigger_Time = 0;		// Brake hold trigger time
unsigned int Brake_Relevel_Trigger_Time = 0;	// Brake relevel trigger time
unsigned int Brake_Relevel_Start_Trigger = 0;	// Brake relevel start trigger time
unsigned int Brake_Drop_Trigger_Time = 0;		// Brake drop trigger time
int ins_brakef = 0;								// Flag to pick brake on inspection
float pick_rate_step = 0;						// Brake pick rate step
float pick_trig_rate = 0;						// Brake pick trigger rate step
float drop_rate_step = 0;						// Brake drop rate step
float drop_trig_rate = 0;						// Brake drop trigger rate step
int prev_timer = 0;								// previous timer value to change brake value

unsigned char Brk_Ctl=0;			// Brake Control variable
						 			// 			Bit 0: 1=Turn on Brake Voltage
						 			// 			Bit 1,2,3:  Brake status request (Current, Voltage In, Voltage Out, I/O, etc)
									//			Bit 4,5: Unused
									//			Bit 6: 1 = Pick on Run
									// 			Bit 7: 1 = Pick on Relevel
#define c_Pick_Brake 0x01   		// Command to pick brake
#define c_Pick_Run 0x40
#define c_Pick_Relevel 0x80

unsigned char Return_Brk_Ctl=0;		// Brake control command reflected from the brake board
unsigned char Prev_Brk_Ctl=0;
unsigned char brake_data_req = 0;	// data request number for brake data (see Brake Status)

unsigned char Brk_Stat=0;			// Brake Status
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

unsigned int Brk_Amp=0;				// Brake Current
unsigned int Brk_Volt_In=0;			// Brake Voltage input
unsigned int Brk_Volt_Out=0;		// Brake Voltage Output
unsigned int Brk_Carrier_Freq = 0;	// Brake Carrier Frequency
unsigned int Brk_Jumpers = 0;		// Brake Jumper Setting
unsigned int Brk_Fault=0;			// Brake Fault Code
unsigned int Prev_Brk_Fault=0;		// Previous brake fault code
unsigned char Brk_Sw_1=0;			// Brake Switch 1
unsigned char Brk_Sw_2=0;			// Brake Switch 2
unsigned char Brk_Relay_Byp=0;		// Brake Relay Bypass Switch

float fbrk_volt=0;					// floating point brake voltage
float fbrk_trg_tim=0;				// floating point brake trigger time


// Emergency brake variables

unsigned int Em_Brake_Voltage=0;					// Brake voltage
unsigned int Return_Em_Brake_Voltage=0;				// Brake voltage command returned from the board
unsigned int Prev_Em_Brake_Voltage = 0;			
unsigned int Em_Brk_Trig_Tim=0;						// Brake trigger time
unsigned int Return_Em_Brk_Trig_Tim=0;				// Brake trigger time reflected from the brake board
unsigned int Prev_Em_Brk_Trig_Tim=0;
unsigned int Em_Brake_Pick_Trigger_Time = 0;		// Brake Pick trigger time
unsigned int Em_Brake_Hold_Trigger_Time = 0;		// Brake hold trigger time
float ebk_pick_rate_step = 0;						// Brake pick rate step
float ebk_pick_trig_rate = 0;						// Brake pick trigger rate step
float ebk_drop_rate_step = 0;						// Brake drop rate step
float ebk_drop_trig_rate = 0;						// Brake drop trigger rate step
float febrk_volt=0;									// floating point brake voltage
float febrk_trg_tim=0;								// floating point brake trigger time
unsigned char Em_Brk_Ctl=0;			// Brake Control variable
						 			// 			Bit 0: 1=Turn on Brake Voltage
						 			// 			Bit 1,2,3:  Brake status request (Current, Voltage In, Voltage Out, I/O, etc)
									//			Bit 4,5: Unused
									//			Bit 6: 1 = Pick on Run
									// 			Bit 7: 1 = Pick on Relevel
unsigned char Return_Em_Brk_Ctl=0;		// Brake control command reflected from the brake board
unsigned char Prev_Em_Brk_Ctl=0;
unsigned char em_brake_data_req = 0;	// data request number for brake data (see Brake Status)

unsigned char Em_Brk_Stat=0;			// Brake Status
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

unsigned int Em_Brk_Amp=0;				// Brake Current
unsigned int Em_Brk_Volt_In=0;			// Brake Voltage input
unsigned int Em_Brk_Volt_Out=0;		// Brake Voltage Output
unsigned int Em_Brk_Carrier_Freq = 0;	// Brake Carrier Frequency
unsigned int Em_Brk_Jumpers = 0;		// Brake Jumper Setting
unsigned int Em_Brk_Fault=0;			// Brake Fault Code
unsigned int Prev_Em_Brk_Fault=0;		// Previous brake fault code




void drop_brake (int cmd)
{
	switch (cmd)
	{
		case 0:		// Drop brake for normal stop
     		if(timers[tsoftstp] <= fvars[fvbrkdropdly])  // delay drop of brake
     		{						  // Note that drop delay is the delay for the relay to drop. The drop rate must be done before the contactor drops.
 				setoutp(o_BRK); // set the brake output
 				clroutp(o_BRKC);// set the brake cooling output

				if (timers[tbkdroprate] > 0)
				{
					timers[tbkdroprate] = 0;
					if ((Brake_Voltage > (unsigned)fvars[fvbkdrop]) && (fvars[fvbkdroprate] > 0))
					{
						if (fvars[fvbkhold] > fvars[fvbkdrop])
						{
							drop_rate_step = ((float)(fvars[fvbkhold] - fvars[fvbkdrop]))/((float)fvars[fvbkdroprate]);
							fbrk_volt = fbrk_volt - drop_rate_step;
//							Brake_Voltage = Brake_Voltage - drop_rate_step;
							Brake_Voltage = (int)fbrk_volt;
							if (Brake_Voltage < (unsigned)fvars[fvbkdrop])
								Brake_Voltage = (unsigned)fvars[fvbkdrop];
						}
						else
						{
							Brake_Voltage = (unsigned)fvars[fvbkdrop];
							fbrk_volt = (float)Brake_Voltage;
						}
					}
					else
					{
						if (fvars[fvbkdroprate] > 0)
							Brake_Voltage = (unsigned)fvars[fvbkdrop];
						else
						{
							if ((Brk_Ctl & c_Pick_Relevel) != 0)
								Brake_Voltage = (unsigned)fvars[fvbkrelv];
							else
								Brake_Voltage = (unsigned)fvars[fvbkhold];
						}

						fbrk_volt = (float)Brake_Voltage;
					}

					if ((Brk_Trig_Tim < Brake_Drop_Trigger_Time) && (fvars[fvbkdroprate] > 0))
					{
						if (Brake_Hold_Trigger_Time < Brake_Drop_Trigger_Time)
						{
							drop_trig_rate = ((float)(Brake_Drop_Trigger_Time - Brake_Hold_Trigger_Time))/((float)fvars[fvbkdroprate]);
							fbrk_trg_tim = fbrk_trg_tim + drop_trig_rate;

//							Brk_Trig_Tim = Brk_Trig_Tim + drop_trig_rate;
							Brk_Trig_Tim = (int)fbrk_trg_tim;
							if (Brk_Trig_Tim > Brake_Drop_Trigger_Time)
							{
								Brk_Trig_Tim = Brake_Drop_Trigger_Time;
								fbrk_trg_tim = (float)Brk_Trig_Tim;
							}
						}
						else
						{	
							Brk_Trig_Tim = Brake_Drop_Trigger_Time;
							fbrk_trg_tim = (float)Brk_Trig_Tim;
						}
					}
					else	
					{
						if (fvars[fvbkdroprate] > 0)
							Brk_Trig_Tim = Brake_Drop_Trigger_Time;
						else 
						{
							if ((Brk_Ctl & c_Pick_Relevel) != 0)
								Brk_Trig_Tim = Brake_Relevel_Trigger_Time;
							else
								Brk_Trig_Tim = Brake_Hold_Trigger_Time;
						}

						fbrk_trg_tim = (float)Brk_Trig_Tim;
					}
				}
     		}
     		else
     		{
	     		if(timers[tsoftstp] > (fvars[fvbrkdropdly] + 2))  // delay drop of brake
	     		{	   // Allow trigger to turn off before opening contactor
					Brk_Ctl &= ~(c_Pick_Brake | c_Pick_Run | c_Pick_Relevel);
					Brk_Trig_Tim = 0;
					Brake_Voltage = 0;
					fbrk_volt = 0;
					fbrk_trg_tim = 0;
	 				clroutp(o_BRK); // clear the brake output
	 				setoutp(o_BRKC);// clear the brake cooling output
				}
	     		else if(timers[tsoftstp] > (fvars[fvbrkdropdly] + 1))  // delay drop of brake
				{	   // Turn off triggering
					Brk_Ctl &= ~(c_Pick_Brake | c_Pick_Run | c_Pick_Relevel);
					Brk_Trig_Tim = 0;
					Brake_Voltage = 0;
					fbrk_volt = 0;
					fbrk_trg_tim = 0;
				}
				else
				{	  // Set the drop brake voltage	 (off voltage)
					Brk_Trig_Tim = Brake_Drop_Trigger_Time;
					Brake_Voltage = (unsigned)fvars[fvbkdrop];
					fbrk_volt = (float)Brake_Voltage;
					fbrk_trg_tim = (float)Brk_Trig_Tim;
				}
     		}

			if ((Brk_Ctl & c_Pick_Relevel) != 0)
			{
				if (Brake_Voltage > (unsigned)fvars[fvbkrelv])
				{
					Brake_Voltage = fvars[fvbkrelv];
					fbrk_volt = (float)Brake_Voltage;
				}

				if (Brk_Trig_Tim < Brake_Relevel_Trigger_Time)
				{
					Brk_Trig_Tim = Brake_Relevel_Trigger_Time;
					fbrk_trg_tim = (float)Brk_Trig_Tim;
				}
			}
			else
			{
				if (Brake_Voltage > (unsigned)fvars[fvbkhold])
				{
					Brake_Voltage = fvars[fvbkhold];
					fbrk_volt = (float)Brake_Voltage;
				}

				if ((Brk_Trig_Tim < Brake_Hold_Trigger_Time) && (Brk_Trig_Tim != 0))
				{
					Brk_Trig_Tim = Brake_Hold_Trigger_Time;
					fbrk_trg_tim = (float)Brk_Trig_Tim;
				}
			}
			break;
		case 1:		// Rese brake i/o to initial condition
			clroutp(o_BRK);   // Clear the Brake Output
			setoutp(o_BRKC);// Clear the Brake Cooling Output
			Brk_Ctl &= ~(c_Pick_Brake | c_Pick_Run | c_Pick_Relevel);
			Brk_Trig_Tim = 0;
			Brake_Voltage = 0;
			fbrk_volt = 0;
			fbrk_trg_tim = 0;
			break;
		case 2:		// Drop brake on emergency stop
			clroutp(o_BRK);
			clroutp(o_BRKC);	// cause the brake to set quicker
			Brk_Ctl &= ~(c_Pick_Brake | c_Pick_Run | c_Pick_Relevel);
			Brk_Trig_Tim = 0;
			Brake_Voltage = 0;
			fbrk_volt = 0;
			fbrk_trg_tim = 0;
			break;
		default:
			clroutp(o_BRK);   // Clear the Brake Output
			setoutp(o_BRKC);// Clear the Brake Cooling Output
			Brk_Ctl &= ~(c_Pick_Brake | c_Pick_Run | c_Pick_Relevel);
			Brk_Trig_Tim = 0;
			Brake_Voltage = 0;
			fbrk_volt = 0;
			fbrk_trg_tim = 0;
			break;
	}
}

// pick brake for each command: cmd 0: pick brake during run
//                              cmd 1: pick brake during relevel

#define c_pick_brake_run 0
#define c_pick_brake_relevel 1

void pick_brake (int cmd)			
{
	switch (cmd)
	{
		case 0:		// pick brake for a run sequence
			if(timers[tpatdly] >= (((fvars[fvbkpktm]+fvars[fvbrkpickdly]) * 10) + don_delay))
				clroutp(o_BRKC);
			else
				setoutp(o_BRKC);
			if(timers[tpatdly] >= ((fvars[fvbrkpickdly]* 10) + don_delay))
			{
				setoutp(o_BRK);
			}
			else
				clroutp(o_BRK);

			// Control Electronic Brake
			if(timers[tpatdly] >= ((fvars[fvbrkpickdly]* 10) + don_delay + 10))
			{							 // allow 100 msec for contactor to pick
				Brk_Ctl |= (c_Pick_Brake | c_Pick_Run);
				if(timers[tpatdly] >= (((fvars[fvbkpktm]+fvars[fvbrkpickdly]) * 10) + don_delay))
				{
					Brk_Trig_Tim = Brake_Hold_Trigger_Time;
					Brake_Voltage = fvars[fvbkhold];
					fbrk_volt = (float)Brake_Voltage;
					fbrk_trg_tim = (float)Brk_Trig_Tim;
				}
				else
				{

					if (timers[tbkpkrate] > 0)
					{
						timers[tbkpkrate] = 0;
						

						if ((Brake_Voltage < (unsigned)fvars[fvbkpick]) && (fvars[fvbkpkrate] > 0))
						{
							if (fvars[fvbkpick] > fvars[fvbkpkst])
								pick_rate_step = ((float)(fvars[fvbkpick] - fvars[fvbkpkst]))/((float)fvars[fvbkpkrate]);
							else
								pick_rate_step = ((float)fvars[fvbkpick])/((float)fvars[fvbkpkrate]);

							fbrk_volt = fbrk_volt + pick_rate_step;
//							Brake_Voltage = Brake_Voltage + pick_rate_step;
							Brake_Voltage = (int)fbrk_volt;

						}
						else
						{
							Brake_Voltage = fvars[fvbkpick];
							fbrk_volt = (float)Brake_Voltage;
						}

						if ((Brk_Trig_Tim > Brake_Pick_Trigger_Time) && (fvars[fvbkpkrate] > 0))
						{
							if (Brake_Pick_Trigger_Time < Brake_Pick_Start_Trigger)
								pick_trig_rate = ((float)(Brake_Pick_Start_Trigger - Brake_Pick_Trigger_Time))/((float)fvars[fvbkpkrate]);
							else
								pick_trig_rate = ((float)Brake_Pick_Trigger_Time)/((float)fvars[fvbkpkrate]);

//							Brk_Trig_Tim = Brk_Trig_Tim - pick_trig_rate;
							fbrk_trg_tim = fbrk_trg_tim - pick_trig_rate;
							Brk_Trig_Tim = (int)fbrk_trg_tim;
						}
						else
						{
							Brk_Trig_Tim = Brake_Pick_Trigger_Time;
							fbrk_trg_tim = (float)Brk_Trig_Tim;
						}
					}

					if (Brake_Voltage > (unsigned)fvars[fvbkpick])
					{
						Brake_Voltage = fvars[fvbkpick];
						fbrk_volt = (float)Brake_Voltage;
					}

					if (Brk_Trig_Tim < Brake_Pick_Trigger_Time)
					{
						Brk_Trig_Tim = Brake_Pick_Trigger_Time;
						fbrk_trg_tim = (float)Brk_Trig_Tim;
					}
				}
			}
			else
			{
				Brk_Ctl &= ~(c_Pick_Brake | c_Pick_Run | c_Pick_Relevel);
				Brk_Trig_Tim = Brake_Pick_Start_Trigger;
				Brake_Voltage = fvars[fvbkpkst];
				fbrk_volt = (float)Brake_Voltage;
				fbrk_trg_tim = (float)Brk_Trig_Tim;
			}

			break;

		case 1:		// Pick brake for a relevel sequence
			if(timers[tpatdly] <= ((fvars[fvbksrel] * 10) + don_delay))
			    clroutp(o_BRK);
			else
			    setoutp(o_BRK); // Turn on the Brake output

			if ((timers[tmotion] > (((fvars[fvbkpktm]+fvars[fvbrkpickdly]) * 10) + don_delay)) || (fvars[fvbkcrel] == 1))
			    clroutp(o_BRKC);   // Turn on the brake cooling output (Reduces brake voltage)
			else
			    setoutp(o_BRKC);   // Turn off the brake cooling output

			// Control Electronic Brake
			if(timers[tpatdly] <= ((fvars[fvbksrel] * 10) + don_delay + 10))
			{							 // allow 50 msec for contactor to pick
				Brk_Ctl &= ~(c_Pick_Brake | c_Pick_Run | c_Pick_Relevel);
				Brk_Trig_Tim = Brake_Relevel_Start_Trigger;
				Brake_Voltage = fvars[fvbkrvst];
				fbrk_volt = (float)Brake_Voltage;
				fbrk_trg_tim = (float)Brk_Trig_Tim;
			}
			else
			{
				Brk_Ctl |= (c_Pick_Brake | c_Pick_Relevel);

				if (timers[tbkpkrate] > 0)
				{
					timers[tbkpkrate] = 0;
					if ((Brake_Voltage < (unsigned)fvars[fvbkrelv]) && (fvars[fvbkrvrate] > 0))
					{
						if (fvars[fvbkrelv] > fvars[fvbkrvst])
							pick_rate_step = ((float)(fvars[fvbkrelv] - fvars[fvbkrvst]))/((float)fvars[fvbkrvrate]);
						else
							pick_rate_step = ((float)fvars[fvbkrelv])/((float)fvars[fvbkrvrate]);

						fbrk_volt = fbrk_volt + pick_rate_step;
//						Brake_Voltage = Brake_Voltage + pick_rate_step;
						Brake_Voltage = (int)fbrk_volt;
					}
					else
					{
						Brake_Voltage = fvars[fvbkrelv];
						fbrk_volt = (float)Brake_Voltage;
					}

					if ((Brk_Trig_Tim > Brake_Relevel_Trigger_Time) && (fvars[fvbkrvrate] > 0))
					{
						if (Brake_Relevel_Trigger_Time < Brake_Relevel_Start_Trigger)
							pick_trig_rate = (Brake_Relevel_Start_Trigger - Brake_Relevel_Trigger_Time)/fvars[fvbkrvrate];
						else
							pick_trig_rate = Brake_Relevel_Trigger_Time/fvars[fvbkrvrate];

						fbrk_trg_tim = fbrk_trg_tim - pick_trig_rate;

//						Brk_Trig_Tim = Brk_Trig_Tim - pick_trig_rate;
						Brk_Trig_Tim = (int)fbrk_trg_tim;
					}
					else
					{
						Brk_Trig_Tim = Brake_Relevel_Trigger_Time;
						fbrk_trg_tim = (float)Brk_Trig_Tim;
					 }
				}

				if (Brake_Voltage > (unsigned)fvars[fvbkrelv])
				{
					Brake_Voltage = fvars[fvbkrelv];
					fbrk_volt = (float)Brake_Voltage;
				}

				if (Brk_Trig_Tim < Brake_Relevel_Trigger_Time)
				{
					Brk_Trig_Tim = Brake_Relevel_Trigger_Time;
					fbrk_trg_tim = (float)Brk_Trig_Tim;
				}
			}

			break;

		default:
			break;
	}
}
//************************************************
// This routine test the brake and drive on inputs
//************************************************

void brk_iotest(void)
{
	
	if (io_fault == 0)
	{
		if ( (rdoutp(o_RUNA) == 1) && (rdinp(i_RUNA) == 1)
			&& (rdoutp(o_MCA) == 1) && (rdinp(i_MCA) == 1) 
			&& (rdinp(i_MCX) == 1))
		{		// Run output turned on	AND MC is on

			if (rdinp(i_DON) == 1)
			{
				timers[tdonoff] = 0;

				// Check the BRK input and output

				if (rdoutp(o_BRK) == 1)
				{		// BRK output is on 
					timers[tbrkon] = 0;
					if (rdinp(i_BRK) == 0)
					{		 // Input is off
						if (timers[tbrkoff] > 8)		
						{
							record_fault(f_brk_off);
							io_fault = 1;
						}
					}
					else
						timers[tbrkoff] = 0;
				}
				else
				{		// BRK output is off
					timers[tbrkoff] = 0;
					if (rdinp(i_BRK) == 1)
					{	 // Input is still on
						if (timers[tbrkon] > 8)
						{
							record_fault(f_brk_on);
							io_fault = 1;
						}
					}
					else
						timers[tbrkon] = 0;
				}
			}
			else	// Test the drive on input
			{		 // Input is off
				timers[tbrkon] = 0;
				timers[tbrkoff] = 0;
				if (timers[tdonoff] > 30)		
				{
					record_fault(f_don_off);
					io_fault = 1;
				}
			}
			timers[tdonon] = 0;
		}
		else 
		{
			timers[tdonoff] = 0;
			timers[tbrkoff] = 0;

			if (rdinp(i_MCA) == 0)
			{		   
				// Test the drive on input
				if (rdinp(i_DON) == 1)
				{		 // Input is off
					if (timers[tdonon] > 8)		
					{
						record_fault(f_don_on);
						io_fault = 1;
					}
				}
				else
					timers[tdonon] = 0;

				if (rdinp(i_BRK) == 1)	// Brake input should be off if "drive on" is off
				{	 // Input is still on
					if (timers[tbrkon] > 8)
					{
						record_fault(f_brk_on);
						io_fault = 1;
					}
				}
				else
					timers[tbrkon] = 0;
			}
		}
	}
	else
	{
		timers[tdonon] = 0;
		timers[tdonoff] = 0;
		timers[tbrkoff] = 0;
		timers[tbrkon] = 0;
	}
}

void em_brake_pick(void)
{
	Em_Brk_Ctl |= c_Pick_Brake;
	if (timers[tebkpick] > 1)
	{			  // Allow 100 msec for MC to pick
		if(timers[tebkpick] >= fvars[fvebkpktim])
		{							 // Brake Cool needs to be energized
			Em_Brk_Trig_Tim = Em_Brake_Hold_Trigger_Time;
			Em_Brake_Voltage = fvars[fvebkhold];
			febrk_volt = (float)Em_Brake_Voltage;
			febrk_trg_tim = (float)Em_Brk_Trig_Tim;
		}
		else
		{
			if (timers[tebkpkrate] > 0)
			{
				timers[tebkpkrate] = 0;

				if ((Em_Brake_Voltage < (unsigned)fvars[fvebkpick]) && (fvars[fvebkpkrate] > 0))
				{
					ebk_pick_rate_step = ((float)fvars[fvebkpick])/((float)fvars[fvebkpkrate]);

					febrk_volt = febrk_volt + ebk_pick_rate_step;
					Em_Brake_Voltage = (int)febrk_volt;

				}
				else
				{
					Em_Brake_Voltage = fvars[fvebkpick];
					febrk_volt = (float)Em_Brake_Voltage;
				}

				if ((Em_Brk_Trig_Tim > Em_Brake_Pick_Trigger_Time) && (fvars[fvebkpkrate] > 0))
				{
					if (Em_Brake_Pick_Trigger_Time < c_max_brake_trigger)
						ebk_pick_trig_rate = ((float)c_max_brake_trigger - (float)Em_Brake_Pick_Trigger_Time)/((float)fvars[fvebkpkrate]);
					else
						ebk_pick_trig_rate = ((float)Em_Brake_Pick_Trigger_Time)/((float)fvars[fvebkpkrate]);

					febrk_trg_tim = febrk_trg_tim - ebk_pick_trig_rate;
					Em_Brk_Trig_Tim = (int)febrk_trg_tim;
				}
				else
				{
					Em_Brk_Trig_Tim = Em_Brake_Pick_Trigger_Time;
					febrk_trg_tim = (float)Em_Brk_Trig_Tim;
				}
			}
		}
	}
	else
	{		 // Prepare to pick brake
		Em_Brk_Ctl &= ~c_Pick_Brake;
		Em_Brake_Voltage = 0;
		febrk_volt = (float)Em_Brake_Voltage;
		Em_Brk_Trig_Tim = c_max_brake_trigger;
		febrk_trg_tim = (float)Em_Brk_Trig_Tim;
	}
		
	if (Em_Brake_Voltage > (unsigned)fvars[fvebkpick])
	{
		Em_Brake_Voltage = fvars[fvebkpick];
		febrk_volt = (float)Em_Brake_Voltage;
	}

	if (Em_Brk_Trig_Tim < Em_Brake_Pick_Trigger_Time)
	{
		Em_Brk_Trig_Tim = Em_Brake_Pick_Trigger_Time;
		febrk_trg_tim = (float)Em_Brk_Trig_Tim;
	}
	timers[tebkdroprate] = 0;
	if((cons[EM_BRK] != 8) && (cons[EM_BRK] != 7))
		timers[tebkdrop] = 0;
}


void em_brake_drop(void)
{
	if (rdoutp(o_MCA) == 0)
	{						// MC is being commanded to drop ... drop brake
		Em_Brk_Ctl &= ~c_Pick_Brake;
		Em_Brake_Voltage = 0;
		febrk_volt = (float)Em_Brake_Voltage;
		Em_Brk_Trig_Tim = 0;
		febrk_trg_tim = (float)Em_Brk_Trig_Tim;
	}
	else if (Em_Brake_Voltage > 0)
	{ 			   // Note that the drop rate must be done before the contactor drops.
		if (timers[tebkdroprate] > 0)
		{				// drop brake according to drop rate
			timers[tebkdroprate] = 0;
			if ((Em_Brake_Voltage > 0) && (fvars[fvebkdroprate] > 0))
			{
				ebk_drop_rate_step = ((float)fvars[fvebkhold])/((float)fvars[fvebkdroprate]);
				febrk_volt = febrk_volt - ebk_drop_rate_step;
				if (febrk_volt > 0)
					Em_Brake_Voltage = (int)febrk_volt;
				else
					Em_Brake_Voltage = 0;
			}
			else
			{
				Em_Brk_Ctl &= ~c_Pick_Brake;
				Em_Brake_Voltage = 0;
				febrk_volt = (float)Em_Brake_Voltage;
			}

			if ((Em_Brk_Trig_Tim < c_max_brake_trigger) && (fvars[fvebkdroprate] > 0))
			{
				ebk_drop_trig_rate = ((float)Em_Brake_Hold_Trigger_Time)/((float)fvars[fvebkdroprate]);
				febrk_trg_tim = febrk_trg_tim + ebk_drop_trig_rate;
				if ((int)febrk_trg_tim > c_max_brake_trigger)
					Em_Brk_Trig_Tim = c_max_brake_trigger;
				else
					Em_Brk_Trig_Tim = (int)febrk_trg_tim;
			}
			else	
			{
				Em_Brk_Ctl &= ~c_Pick_Brake;
				Em_Brk_Trig_Tim = c_max_brake_trigger;
				febrk_trg_tim = (float)Em_Brk_Trig_Tim;
			}

	    }
		if (Em_Brake_Voltage > (unsigned)fvars[fvebkhold])	// always drop from the hold voltage
			Em_Brake_Voltage = fvars[fvebkhold];

		if (Em_Brk_Trig_Tim < Em_Brake_Hold_Trigger_Time)
			Em_Brk_Trig_Tim = Em_Brake_Hold_Trigger_Time;
	}
	else
	{		 // brake has timed out and dropped
		Em_Brk_Ctl &= ~c_Pick_Brake;
		Em_Brake_Voltage = 0;
		febrk_volt = (float)Em_Brake_Voltage;
		Em_Brk_Trig_Tim = 0;
		febrk_trg_tim = (float)Em_Brk_Trig_Tim;
	}
	timers[tebkpick] = 0;
	timers[tebkpkrate] = 0;
}

void em_brake_control(int16 start_ck)
{
	// Sheave brake cons[EM_BRK] == 2 or Electronic brake with delayed drop cons[EM_BRK] == 7

	if ((cons[EM_BRK] == 2) && (gripper_fault == 0))
	{
		if ((ins_brakef == 1) && ((safe() == 1) || (safe_override_seq == 2)) && (chk_ins_sw() == 0) && (ins_sw_err == 0)) // pick brake on inspection
			timers[tebkdrop] = 0;
		else if (((chk_ins() != 0) && (chk_ins_sw() != 0)) || (dpref != 0) || (dirf != 0)) // when to pick the brake
			timers[tebkdrop] = 0;

		// Control the Emergency Brake output

		if (timers[tebkdrop] <= fvars[fvebkdropdly])	// TURN ON EMERGENCY BRAKE output
		{
			if ((rdoutp(o_EBK) == 0) || (rdinp(i_EBKS) == 0))	 // output is currently off OR brake did not pick
				timers[tebkpick] = 0;			 // reset the timer on first pick
			if (rdoutp(o_EBK) == 0)
				timers[tebrkio] = 0;
			setoutp(o_EBK);
		}
		else
		{
			if (rdoutp(o_EBK) == 1)
				timers[tebrkio] = 0;
			clroutp(o_EBK);	// turn off EMERGENCY BRAKE OUTPUT
		}

		// Control the Emergency Brake Cool relay output and test for i/o failure

		if (rdoutp(o_EBK) == 1)		// Brake is being picked
		{
			if (timers[tebkpick] > fvars[fvebkpktim])
				clroutp(o_EBKC);
			else
				setoutp(o_EBKC);
			
			if ((rdinp(i_EBK) == 0) && (rdinp(i_EBKS) == 1))
				timers[tebrkio] = 0;
			else 
			{
				if ((timers[tebrkio] > 10) && (rdinp(i_EBK) == 1) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_on);
					io_fault = 1;
					timers[tebrkio] = 0;
				}
				if ((timers[tebrkio] > 50) && (rdinp(i_EBKS) == 0) && (rdinp(i_MRSW) == 1) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_sw_off);
					io_fault = 1;
					timers[tebrkio] = 0;
					procf = 11;
					if (fault_run == 0)
						fault_run_cnt++;
					fault_run = 1;
				}
			}
		}
		else
		{
			setoutp(o_EBKC);

			if ((rdinp(i_EBK) == 1) && (rdinp(i_EBKS) == 0))
				timers[tebrkio] = 0;
			else 
			{
				if ((timers[tebrkio] > 10) && (rdinp(i_EBK) == 0) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_off);
					io_fault = 1;
					timers[tebrkio] = 0;
				}
				if ((timers[tebrkio] > 50) && (rdinp(i_EBKS) == 1) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_sw_on);
					io_fault = 1;
					timers[tebrkio] = 0;
				}
			}
		}
	}

	// Sheave brake 2007 compliance

	if (((cons[EM_BRK] == 3) || (cons[EM_BRK] == 8)) && (gripper_fault == 0) && ((gripper_test >= 10) || (gripper_test == 0) || (chk_ins() != 0)))
	{
		if (((chk_ins() != 0) && (chk_ins_sw() != 0)) || (dpref != 0) || (dirf != 0)) 
			timers[tebkdrop] = 0;

		// Control the Emergency Brake output

		if (timers[tebkdrop] <= fvars[fvebkdropdly])	// TURN ON EMERGENCY BRAKE output
		{
			if (rdoutp(o_EBA1) == 0) 	 // output is currently off OR brake did not pick
				timers[tebkpick] = 0;			 // reset the timer on first pick
			if (rdoutp(o_EBA1) == 0)
				timers[tebrkio] = 0;
			setoutp(o_EBA1);
			setoutp(o_EBA2);
			clroutp(o_EBB1);
			clroutp(o_EBB2);
		}
		else
		{
			if (rdoutp(o_EBK) == 1)
				timers[tebrkio] = 0;
			clroutp(o_EBA1);	// turn off EMERGENCY BRAKE OUTPUT
			clroutp(o_EBA2);	// turn off EMERGENCY BRAKE OUTPUT
			clroutp(o_EBB1);
			clroutp(o_EBB2);
		}

		// Control the Emergency Brake Cool relay output and test for i/o failure

		if (rdoutp(o_EBA1) == 1)		// Brake is being picked
		{
			if(cons[EM_BRK] == 3)
			{
				if (timers[tebkpick] > fvars[fvebkpktim])
					clroutp(o_EBC);
				else
					setoutp(o_EBC);
			}

			if ((rdinp(i_EBA) == 0) && (rdinp(i_EBKS) == 1))
				timers[tebrkio] = 0;
			else 
			{
				if ((timers[tebrkio] > 10) && (rdinp(i_EBA) == 1) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_on);
					io_fault = 1;
					timers[tebrkio] = 0;
				}
				if ((timers[tebrkio] > 50) && (rdinp(i_EBKS) == 0) && (rdinp(i_MRSW) == 1) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_sw_off);
					io_fault = 1;
					timers[tebrkio] = 0;
					procf = 11;
					if (fault_run == 0)
						fault_run_cnt++;
					fault_run = 1;
				}
			}
		}
		else
		{
			if(cons[EM_BRK] == 3)
			{
				setoutp(o_EBC);
			}

			if ((rdinp(i_EBA) == 1) && (rdinp(i_EBKS) == 0))
				timers[tebrkio] = 0;
			else 
			{
				if ((timers[tebrkio] > 10) && (rdinp(i_EBA) == 0) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_off);
					io_fault = 1;
					timers[tebrkio] = 0;
				}
				if ((timers[tebrkio] > 50) && (rdinp(i_EBKS) == 1) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_sw_on);
					io_fault = 1;
					timers[tebrkio] = 0;
				}
			}
		}
	}
	
	if ((cons[EM_BRK] == 8) && (gripper_fault == 0))
	{
		if ((rdinp(i_EBA)== 1) && (rdinp(i_EBB) == 1))	 // output is currently off
		{
			timers[tebkpick] = 0;		
		}
		if(((rdoutp(o_EBA1) == 1) && (rdoutp(o_EBA2) == 1)) ||
			(((rdoutp(o_EBB1) == 1) && (rdoutp(o_EBB2) == 1))))
		{
			em_brake_pick();				
		}
		else
		{
			em_brake_drop();
		}
		
	}
	

	if ((cons[EM_BRK] == 5) && (gripper_fault == 0))
	{
		// Control the Emergency Brake Cool relay output and test for i/o failure

		if ((rdinp(i_MCA) == 1) && (rdinp(i_P) == 0))		// Brake Cool needs to be energized
		{
			if (timers[tebkpick] > fvars[fvebkpktim])
				clroutp(o_EBC);
			else
				setoutp(o_EBC);
		}
		else
		{
			setoutp(o_EBC);
			timers[tebkpick] = 0;			 // reset the timer
		}
	}

	// Monitor the brake switch so it can be taken out of the DEL circuit.

	if ((cons[EM_BKS] != 0) && ((cons[EM_BRK] == 1) || (cons[EM_BRK] == 4) || (cons[EM_BRK] == 5)))
	{
		// Test for emergency brake switch

		if ((rdinp(i_MCA) == 1) && (rdinp(i_P) == 0))		// The emergency brake should be picked
		{
			if (rdinp(i_EBKS) == 1)
				timers[tebrkio] = 0;
			else 
			{
				if ((timers[tebrkio] > 50) && (rdinp(i_EBKS) == 0) && (rdinp(i_MRSW) == 1) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_sw_off);
					io_fault = 1;
					timers[tebrkio] = 0;
					procf = 11;
					if (fault_run == 0)
						fault_run_cnt++;
					fault_run = 1;
				}
			}
		}
		else
		{
			if (rdinp(i_EBKS) == 0)
				timers[tebrkio] = 0;
			else 
			{
				if ((timers[tebrkio] > 50) && (rdinp(i_EBKS) == 1) && (io_fault == 0))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_sw_on);
					io_fault = 1;
					timers[tebrkio] = 0;
				}
			}
		}
	}



	if (cons[EM_BRK] == 6)
	{
		// Control Electronic Emergency Brake when no unintended motion fault
		if ((rdinp(i_MCA) == 1) && (rdoutp(o_MCA) == 1) && (rdinp(i_P) == 0) && (gripper_fault == 0))
		{				 // mc is being energized
			if ((rdoutp(o_UP) == 1) || (rdoutp(o_DNR) == 1))
			{		// Run condition
				if (rdinp(i_XBKS1) == 1)
					timers[tebrkio] = 0;
				else if ((rdinp(i_XBKS1) == 0) && (timers[tebrkio] > 50))
				{
					if (io_fault == 0)
						record_fault(f_em_brk_sw_off);
					io_fault = 1;
					procf = 11;
					if (fault_run == 0)
						fault_run_cnt++;
					fault_run = 1;
				}
				em_brake_pick();
			}
			else if ((ins_brakef == 1) && ((safe() == 1) || (safe_override_seq == 2)) && (chk_ins_sw() == 0) && (ins_sw_err == 0))
			{		// Lift brake on inspection
				em_brake_pick();
			}
			else
			{		// Drop the emergency brake
				em_brake_drop();
				timers[tebrkio] = 0;
			}
		}
		else
		{		// Drop the emergency brake
			if (rdinp(i_XBKS1) == 0)
				timers[tebrkio] = 0;
			else if ((rdinp(i_XBKS1) == 1) && (timers[tebrkio] > 50))
			{
				if (io_fault == 0)
					record_fault(f_em_brk_sw_on);
				io_fault = 1;
			}
			em_brake_drop();
		}
	}

	if ((cons[EM_BRK] == 7) && ((cons[brake] == 2) || (cons[brake] == 3)))
	{
		
		if ((gripper_fault == 0) && (in_pfc_sfc_test == 0) && (in_gripper_test == 0) && 
					((rdoutp(o_GR2R) == 1) || (rdoutp(o_LE) == 1)))
		{
			if ((rdinp(i_MCA) == 1) 	||				// normal brake pick
				((chk_ins() == 1) && (ins_brakef == 1) && (chk_ins_sw() == 0)) ||
				((rdinp(i_SFC) == 0) && (rdinp(i_EMP) == 1) && (pwl_brakef == 1) && (cons[PWL_Brake] == 1)))  // pick brake on inspection
			{
				if (start_ck == 0)
				{		// allow control to pick contactors on start (contactors will bick here on recovery and inspection brake pick)
					setoutp(o_EBK2);		// close the dc side first
					setoutp(o_EBK1);
				}
				timers[tebkdrop] = 0;
			}


			// Control the Emergency Brake

			if (timers[tebkdrop] <= fvars[fvebkdropdly])	// TURN ON EMERGENCY BRAKE output
			{
				if (rdoutp(o_EBK1) == 0)	 // output is currently off
				{
					timers[tebkpick] = 0;			 // reset the timer on first pick
					timers[tebrkio] = 0;
				}
				if ((rdinp(i_EBK1) == 1) && (rdinp(i_EBK2) == 1))
					em_brake_pick();		// Trigger board after all contactors picked
			}
			else 
			{			// Drop the brake
				
				if (timers[tebkdrop] <= (fvars[fvebkdropdly] + 1))
				{	// allow 100 msec for brake to drop voltage before opening contactor
					em_brake_drop();
				}
				else if (timers[tebkdrop] <= (fvars[fvebkdropdly] + 2))
				{	// Open AC contactor after 200 msec 
					clroutp(o_EBK1);	// turn off EMERGENCY BRAKE AC Contactor
					em_brake_drop();
				}
				else if (timers[tebkdrop] <= (fvars[fvebkdropdly] + 3))
				{	// Open DC contactor after 300 msec 
					clroutp(o_EBK1);	// turn off EMERGENCY BRAKE AC Contactor
					clroutp(o_EBK2);	// turn off EMERGENCY BRAKE DC Contactor
					em_brake_drop();
				}
				else
				{		// open brake contactor
					clroutp(o_EBK1);	// turn off EMERGENCY BRAKE AC Contactor
					clroutp(o_EBK2);	// turn off EMERGENCY BRAKE DC Contactor
					if (rdoutp(o_EBK) == 1)
						timers[tebrkio] = 0;
					em_brake_drop();
				}
			}


			if (rdoutp(o_EBK1) == 1)		// Brake is being picked
			{
				
				if (rdinp(i_EBKS) == 1)
					timers[tebrkio] = 0;
				else 
				{
					if ((timers[tebrkio] > 50) && (rdinp(i_EBKS) == 0) && (rdinp(i_MRSW) == 1) && (io_fault == 0))
					{
						if (io_fault == 0)
							record_fault(f_em_brk_sw_off);
						io_fault = 1;
						timers[tebrkio] = 0;
						procf = 11;
						if (fault_run == 0)
							fault_run_cnt++;
						fault_run = 1;
					}
				}
			}
			else
			{

				if (rdinp(i_EBKS) == 0)
					timers[tebrkio] = 0;
				else 
				{
					if ((timers[tebrkio] > 50) && (rdinp(i_EBKS) == 1) && (io_fault == 0))
					{
						if (io_fault == 0)
							record_fault(f_em_brk_sw_on);
						io_fault = 1;
						timers[tebrkio] = 0;
					}
				}
			}
		}
		else
		{
			rdinp(i_EBK1);		// make sure i/o's are being updated on the screen
			rdinp(i_EBK2);
			rdinp(i_EBKS);
			// open brake contactor
			clroutp(o_EBK1);	// turn off EMERGENCY BRAKE AC Contactor
			clroutp(o_EBK2);	// turn off EMERGENCY BRAKE DC Contactor
			if (rdoutp(o_EBK) == 1)
				timers[tebrkio] = 0;
			em_brake_drop();
			timers[tebrkio] = 0;
		}
	}
	
	if (cons[Aux_Brake] == 1)
	{		// Follow regular brake
		Em_Brk_Ctl = Brk_Ctl;
		Em_Brake_Voltage = Brake_Voltage;
		Em_Brk_Trig_Tim = Brk_Trig_Tim;
	}
	

}

/* Revision History

3/8/06 v4.58 mhd 		1. Correct brake pick electronic trigger control.
5/11/07 v5.23 mhd		1. Correct brake drop timming to have BK drop after trigger is turned off <= should have been >.
						2. Added 50 msec on BK pick delay after trigger starts.
2/7/08 v5.41 mhd		1. Set brake voltage to zero in brake drop routine.
3/12/08 v5.42 mhd		1. Added Brake start and brake pick rate for pick and relevel.
8/22/08 v5.45.6 mhd		1. Added em_brake_control() (moved from control.cc).
3/18/09 v5.47.11 mhd 	1. Added Brake voltage drop ramp down on stop using: fvbkdrop (Brake Drop Voltage), fvbkdroprate (Brake Voltage Drop Rate) and
						   fvbkdroplev (Brake Drop timing during Leveling flag).
5/29/09 v5.48.8 mhd		1. Modified calculation for brake pick and drop rate to use floating point. 
6/4/09		 mhd		2. Set brake voltage to brake hold voltage during drop time if drop rate equals zero.
						3. Corrected brake pick voltage.
10/12/09 v5.50.4 mhd	1. Set flag for brake relevel and brake run.
12/22/09 v5.51.1 mhd	1. Check for (Brk_Trig_Tim != 0) when dropping brake before setting trigger time to hold trigger value.
12/8/11 v5.54.42 mhd	1. Corrected emergency brake pick to set Em_Brake_Voltage instead of Brake_Voltage and fvars[fvbkpick] to fvars[fvebkpick].
3/1/12 v5.54.60 mhd		1. Corrected emergency brake pick to check (ins_sw_error == 0) instead of (ins_sw_error != 0).
4/18/12 v6.0.06 mhd		1. Made EBKS input an array.
						2. Added check for brake switch input when cons[EM_BKS] is set.
						3. Initiate an emergency slowdown if the brake swtich does not turn on and also increment the fault run counter.
5/1/12 v6.0.7 mhd		1. Added new brake control cons[EM_BRK] == 7.  This is to have a controled drop of the emergency brake even after MC drops.
9/7/12 v6.0.21	mhd		1. In cons[em_brk] == 7, corrected brake timer to  if (timers[tebkdrop] <= fvars[fvebkdropdly])  --- added the <= instead <.
8/13/13 v7.0.16 mhd		1. Added em_brk type 8 from version 6.0.49.
11/12/13 v7.0.8	mhd		1. Em Brake = 7, change rdoutp(o_MCA) to rdinp(i_MCA) because brake has to pick after the run relay picks on a recovery run.
12/8/15 v7.1.53 mhd		1. Em Brake = 7, Since power for the em brake comes from the gripper circuit, only pick the brake when the 
						   gripper circuit is providing power i.e., ((rdoutp(o_GR2R) == 1) || (rdoutp(o_LE) == 1)).
*/
