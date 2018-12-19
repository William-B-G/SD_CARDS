#define d_TRAFFICLOG 1
#include "global.h"

#define LOG_IX_SIZE_WORDS 3			// Number of words used per log index

#define MAX_NMB_LOG_IXS 21845		// Maximum number of log indexes in MRAM

/*

Q - How do we want to store traffic data on MRAM?
A - We want to use only one page of memory for saving traffic log data. 

	1 Page of MRAM = 128KB = 131072Bytes = 65536(16Bit)Words
	Each logIx uses 3 (16bit)words of MRAM
	65536Words/3Words = 21845 Total log ixs

We want to store the calls on MRAM by dynamically allocating memory, if MRAM buffer is full, erase a oldest day, 
if only 1 day of data is stored and the buffer is full then remove oldest 500 calls and use that space for logging new calls.

Example usage of MRAM for two days of data with 3 calls each day and 2 headers.
Number of calls on each day is dynamic (as many as come into the system during that day.).

					 LOG
	 _____________   IX#	BYTES USED (6bytes per ix)
	|    Day 1    |
	|  Header ix1 |   0		0-5
	|_____________|
	|             |
	|  Header ix2 |   1		6-11
	|_____________|
	|             |
	|   Call 1    |   2		12-17
	|_____________|
	|             |
	|   Call 2    |   3		18-23
	|_____________|
	|             |
	|   Call 3    |   4		24-29
	|_____________|
	|    Day 2    |
	|  Header ix1 |   5		30-35
	|_____________|
	|             |
	|  Header ix2 |   6		36-41
	|_____________|
	|             |
	|   Call 1    |   7		42-47
	|_____________|
	|             |
	|   Call 2    |   8		48-53
	|_____________|
	|             |
	|   Call 3    |   9		54-59
	|_____________|

					  .		.
					  .		.
					  .		.
					  
					  n     ((n*6)-((n*6)+5)), 
					  		
					  		n max value = 21845
					  
					  MRAM Log buffer is used as a circular buffer.

	Each new day starts with header containing the following 6 words of data.

	Header is divided into two log indexes.
	
	Header1 description:
	
	  Word#
		1			BIT0-4:  Day of the month, Dec Values: 1-31
					BIT5-8:	 Month of the year, Dec Values: 1-12
					BIT9-15: Year count, Dec Value: 0-99
					
		2			BIT0-15: Previous day log start ix

		3			BIT0-15: Next day log start ix

	Header2 description:
	
	  Word#
		1			BIT0-15: Total number of calls logged for the day
		
		2			BIT0-15: 2 Spare bytes
		
		3			BIT0-15: 2 Spare bytes


	Hall/Car Call log description:
	
	  Word#
	  	1			BIT0:     0=Invalid, 1=Valid log saved
	  				BIT1:     0=Car call logged, 1=Hall call logged
	  				BIT2-8:   Hall call floor num, Dec Values: 1-60
	  				          IF (its a HC logged) THEN it is the position when HC was latched.
	  				          ELSEIF (its a CC logged) THEN it is position where the CC was latched from.
	  				BIT9-15:  Only used when its a car call
	  				          Car call floor num, Dec values: 1-60

	  	2			BIT0-3:   Car number,        Dec Values: 1-8
	  				          Car number that served the hc/cc logged
	  				BIT4-9:   Call Entry TS Sec,	Dec Values: 0-59
					BIT10-15: Call Entry TS Min, Dec Values: 0-59

		3			BIT0-4:  Call Entry TS Hour, Dec Values: 0-23
					BIT5-12: Call completion time in seconds, Dec Values 0-255
					BIT13:   0=Front Door, 1=Rear Door Call (door type for the hc or cc logged)
					BIT14:   Only used when its a hall call logged
					         0=Up Dir HC, 1=Dn Dir HC 
					BIT15:   0=Spare bit


	Other necessay traffic log variables are kept on separate page of MRAM.
		logPutIx				+2 bytes
		logStartIx				+2 bytes
		logTodayStartIx			+2 bytes
		nmbLoggedDays			+2 bytes
		nmbLoggedCallsToday		+2 bytes
		old_day					+1 bytes
		old_month				+1 bytes
		old_year				+2 bytes
						Total = 14 bytes = 7 words
*/

typedef union Traffic_Log_Index
{
	// Access log ix in 3 words format
	uint16 dataW[LOG_IX_SIZE_WORDS];
	
	// Access traffic log index in car/hall call format
	struct
	{
		struct
		{
			uint16 valid_log   :  1;		// BIT0:     0=Invalid, 1=Valid log saved
			uint16 call_type   :  1;		// BIT1:     0=Car call logged, 1=Hall call logged
			uint16 hc_fl       :  7;		// BIT2-8:   Hall call floor num, Dec Values: 1-60
											//           IF (its a HC logged) THEN it is the position when HC was latched.
											//           ELSEIF (its a CC logged) THEN it is position where the CC was latched from.
			uint16 cc_fl       :  7;		// BIT9-15:  Only used when its a car call
											//           Car call floor num, Dec values: 1-60
		}W1;
		
		struct
		{
			uint16 car         :  4;		// BIT0-3:   Car number,        Dec Values: 1-8
											//           Car number that served the hc/cc logged
			// Entry timestamp indicating the time when the call was latched
			uint16 entry_sec   :  6;		// BIT4-9:   Call Entry TS Sec,	Dec Values: 0-59
			uint16 entry_min   :  6;		// BIT10-15: Call Entry TS Min, Dec Values: 0-59
		}W2;
		
		struct
		{
			uint16 entry_hour  :  5;		// BIT0-4:  Call Entry TS Hour, Dec Values: 0-23
			uint16 t_completed :  8;		// BIT5-12: Call completion time in seconds, Dec Values 0-255
			uint16 call_dtype  :  1;		// BIT13:   0=Front Door, 1=Rear Door Call (door type for the hc or cc logged)
			uint16 hcdir       :  1;		// BIT14:   Only used when its a hall call logged
											//          0=Up Dir HC, 1=Dn Dir HC 
			uint16 spare_bit   :  1;		// BIT15:   0=Spare bit
		}W3;
	}call_log;
	
	// Access traffic log index in header 1 format
	struct 
	{
		struct
		{	// Date is saved in the following format: DD-MM-YY
			uint16 day         :  5;        // BIT0-4:  Day of the month, Dec Values: 1-31
			uint16 month       :  4;        // BIT5-8:	Month of the year, Dec Values: 1-12
			uint16 year        :  7;        // BIT9-15:	Year count, Dec Value: 0-99
		}W1;	// word 1 (16bits)

		struct
		{
			uint16 prev_day_ix :  16;       // BIT0-15: Previous day log start ix
		}W2;	// word 2 (16bits)
		
		struct
		{
			uint16 next_day_ix :  16;       // BIT0-15: Next day log start ix
		}W3;	// word 3 (16bits)
	}header1;
	
	// Access traffic log index in header 2 format
	struct 
	{
		struct
		{
			uint16 nmb_calls   :  16;       // BIT0-15: Total number of calls logged for the day
		}W1;	// word 1 (16bits)
		
		struct
		{
			uint16 spare_bits  :  16;       // BIT0-15: 2 Spare bytes
		}W2;	// word 2 (16bits)
		
		struct
		{
			uint16 spare_bits  :  16;       // BIT0-15: 2 Spare bytes
		}W3;	// word 3 (16bits)
	}header2;
	
}Log_Ix_Type;		// traffic log index type

Log_Ix_Type t_Log_Ix;				// +6 bytes		Traffic log index, used throughout trafficlog.c routine as temp buffer 
									// 				for reading 6 words of log ix from MRAM to RAM, and writing log index
									//				of 6 words from RAM to MRAM.

void logNewDateHeader(void);
void trafficLogControl(void);
void init_MRAM_Traffic_Log(void);
void update_TLogVars_on_MRAM(void);
void clearLogIxBuffer(Log_Ix_Type* ptr);
uint16 writeNewCallToMRAM(Log_Ix_Type* ptr);
void writeLogIxToMRAM(uint16 log_ix, Log_Ix_Type* ptr);
void readLogIxFromMRAM(uint16 log_ix, Log_Ix_Type* ptr);
uint8 findLogForDate(uint8 mm, uint8 dd, uint8 yy, uint16* ptr);
void getNextLogIx(Log_Ix_Type* ptr);

/* 
	Variables used for storing previous copy of the car 
	calls and hall calls data to be able to detect change.
*/

/* Start prev vars declarations */

uint32 prev_guhc[2];				// +8 bytes
uint32 prev_gdhc[2];				// +8 bytes
uint32 prev_gruhc[2];				// +8 bytes
uint32 prev_grdhc[2];				// +8 bytes
uint32 prev_gcc[car_size][2];		// +72 bytes
uint32 prev_grcc[car_size][2];		// +72 bytes

/* End prev vars declarations */

// Per car store what floor last up/down hc was cancelled on
uint8 last_uhc_cancelled[car_size];		// +72 bytes
uint8 last_dhc_cancelled[car_size];		// +72 bytes
uint8 last_rdhc_cancelled[car_size];	// +72 bytes
uint8 last_ruhc_cancelled[car_size];	// +72 bytes

// Indexes to track circular MRAM log buffer
uint16 logPutIx = 0;
uint16 logGetIx	= 0;
uint16 logStartIx = 0;
uint16 nmbLoggedDays = 0;
uint16 nmbLoggedCallsToday = 0;
uint16 logTodayStartIx = 0;
uint16 lastLogIxWritten = 0;

void allocateMoreMRAM(void);
uint8 isTLogBufferFull(void);

/* 
	Below variables are used for tracking log indexes of 
	the currently latched calls, so when the call drops
*/

/* Start declaration log indexes tracking variables 	*/

uint16 ucTLogIx[fl_size];				// +122  bytes
uint16 dcTLogIx[fl_size];				// +122  bytes
uint16 rucTLogIx[fl_size];				// +122  bytes
uint16 rdcTLogIx[fl_size];				// +122  bytes
uint16 ccTLogIx[car_size][fl_size];		// +1098 bytes
uint16 rccTLogIx[car_size][fl_size];	// +1098 bytes

/* End declaration log indexes tracking variables 		*/


/*
	Copy HC and CC group data to car local core
	To make sure group data doesn't change in group core
	while reading and processing the data
*/

/* Start declaration log indexes tracking routine & variables */

void copy_group_data_to_local(void);

uint32 local_g_up_hall_call[2];			// +8  bytes
uint32 local_g_dn_hall_call[2];			// +8  bytes
uint32 local_g_upr_hall_call[2];		// +8  bytes
uint32 local_g_dnr_hall_call[2];		// +8  bytes
uint32 local_g_cc[car_size][2];			// +72 bytes
uint32 local_g_rcc[car_size][2];		// +72 bytes

/* End declaration log indexes tracking routine & variables */


// Data change detection vars and routine
uint8 chkDateChanged(void);
int16 old_year;
uint8 old_day, old_month;


// Routine to get time difference between two time stamps
uint8 time_difference(void *ptr1, void *ptr2);
struct timestamp
{
	uint8 sec;
	uint8 min;
	uint8 hour;
};
struct timestamp ts1;		// +3 bytes
struct timestamp ts2;		// +3 bytes


static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;

//*****************************************
//	Init MRAM for traffic log
//*****************************************

void init_MRAM_Traffic_Log(void)
{
	int16 k;
	k=(int16)MRAM_Traffic_Log_Vars_Offset;
	
	if (rd_mram(MRAM_Traffic_Log_Vars_Page,k) != (int16)0x190F)		
	{
		wr_mram(MRAM_Traffic_Log_Vars_Page,k,0x190F);
		k+=2;
										// First time start traffic log
		logPutIx = 0;
		logStartIx = 0;
		nmbLoggedDays = 0;
		logTodayStartIx = 0;
		
		// Log 1st day's header.
		
		// Write header1 data
		clearLogIxBuffer(&t_Log_Ix);
		local_getdate();
		t_Log_Ix.header1.W1.day = d.day;
		t_Log_Ix.header1.W1.month = d.month;
		t_Log_Ix.header1.W1.year = (d.year%100);
		t_Log_Ix.header1.W2.prev_day_ix = 0;
		t_Log_Ix.header1.W3.next_day_ix = 0;
		writeLogIxToMRAM(logPutIx++,&t_Log_Ix);
		
		// Write header2 data
		clearLogIxBuffer(&t_Log_Ix);
		t_Log_Ix.header2.W1.nmb_calls = 0;
		writeLogIxToMRAM(logPutIx++,&t_Log_Ix);
		
		old_day = d.day;
		old_month = d.month;
		old_year = d.year;
		
		nmbLoggedDays++;
		
		// Save the above initialized Log vars to MRAM
		update_TLogVars_on_MRAM();
	}
	else
	{
		k+=2;
		logPutIx = (uint16)rd_mram(MRAM_Traffic_Log_Vars_Page,k);
		k+=2;
		logStartIx = (uint16)rd_mram(MRAM_Traffic_Log_Vars_Page,k);
		k+=2;
		logTodayStartIx = (uint16)rd_mram(MRAM_Traffic_Log_Vars_Page,k);
		k+=2;
		nmbLoggedDays = (uint16)rd_mram(MRAM_Traffic_Log_Vars_Page,k);
		k+=2;
		nmbLoggedCallsToday = (uint16)rd_mram(MRAM_Traffic_Log_Vars_Page,k);
		k+=2;
		
		uwu.w = rd_mram(MRAM_Traffic_Log_Vars_Page,k);
		k+=2;
		old_day = uwu.b[0];
		old_month = uwu.b[1];
		old_year = rd_mram(MRAM_Traffic_Log_Vars_Page,k);
		k+=2;
	}
}

//*****************************************
//	Update traffic log variables on MRAM.
//	Traffic log variables are kept on
//	separate page.
//*****************************************

void update_TLogVars_on_MRAM(void)
{
	int16 k;
	k=(int16)(MRAM_Traffic_Log_Vars_Offset+2);
	
	wr_mram(MRAM_Traffic_Log_Vars_Page,k,logPutIx);
	k+=2;
	wr_mram(MRAM_Traffic_Log_Vars_Page,k,logStartIx);
	k+=2;
	wr_mram(MRAM_Traffic_Log_Vars_Page,k,logTodayStartIx);
	k+=2;
	wr_mram(MRAM_Traffic_Log_Vars_Page,k,nmbLoggedDays);
	k+=2;
	wr_mram(MRAM_Traffic_Log_Vars_Page,k,nmbLoggedCallsToday);
	k+=2;
	uwu.b[0] = old_day;
	uwu.b[1] = old_month;
	wr_mram(MRAM_Traffic_Log_Vars_Page,k,uwu.w);
	k+=2;
	wr_mram(MRAM_Traffic_Log_Vars_Page,k,old_year);
	k+=2;
}

//*****************************************
//	Traffic log manager
//	Main routine that drives traffic log.
//	Detects latching OR dropping in the Hall 
//	Calls and Car Call signals and updates
//	log on MRAM .
//*****************************************

void trafficLogControl(void)
{
	uint8 el, fl, fl_ix;
	
	if (Dispatcher == cons[carnmb])
	{
		if (chkDateChanged() != 0)
		{
			logNewDateHeader();		// Write header bytes on date change
		}
		
		copy_group_data_to_local();
		
		// Check for car call latch/drop
		
		for (el=1;el<=cons[nmbcars];el++)
		{
			if ((local_g_cc[el][0] != prev_gcc[el][0]) || (local_g_cc[el][0] != prev_gcc[el][1]))
			{
				for (fl=1;fl<=top_fl[el];fl++)
				{
					fl_ix = (fl-1)/32;
					if ((local_g_cc[el][fl_ix] & fl_mask[fl]) != (prev_gcc[el][fl_ix] & fl_mask[fl]))
					{
						if ((local_g_cc[el][fl_ix] & fl_mask[fl]) != 0)		// Car call latched
						{
							// Car must not be at the position where the car call input is ON
							// This check ensures bad CC input doesn't trigger unncessary calls to get logged
							if ((fl != grp_Car_Stat[el].g_pos) || 
								(((grp_Car_Stat[el].g_run_statusf & rs_DZ) == 0) || ((grp_Car_Stat[el].g_run_statusf & rs_HSF) != 0)))
							{
								clearLogIxBuffer(&t_Log_Ix);
								
								t_Log_Ix.call_log.W1.call_type = 0;						// Car Call type
								t_Log_Ix.call_log.W1.cc_fl = fl;						// Floor# where car call latched to
								t_Log_Ix.call_log.W1.hc_fl = grp_Car_Stat[el].g_pos;	// Position where car call latched from
								t_Log_Ix.call_log.W3.call_dtype = 0;					// Front door CC
								t_Log_Ix.call_log.W2.car = el;							// Car # where car call latched
								local_gettime();										// Refresh time structure
								t_Log_Ix.call_log.W2.entry_sec = t.second;				// Entry TS sec count
								t_Log_Ix.call_log.W2.entry_min = t.minute;				// Entry TS min count
								t_Log_Ix.call_log.W3.entry_hour = t.hour;				// Entry TS hour count
								
								ccTLogIx[el][fl] = writeNewCallToMRAM(&t_Log_Ix);		// Write the new call log to MRAM,
																						// save the log index location where the call was written
							}
						}
						else												// Car call dropped
						{
							readLogIxFromMRAM(ccTLogIx[el][fl],&t_Log_Ix);				// Read the call_log from MRAM
							
							// Verify the location of the car and set the valid_log status bit on the log index
							if ((t_Log_Ix.call_log.W1.valid_log == 0) && (grp_Car_Stat[el].g_pos == (int16)t_Log_Ix.call_log.W1.cc_fl))
							{
								t_Log_Ix.call_log.W1.valid_log = 1;
								ts1.sec = (uint8)t_Log_Ix.call_log.W2.entry_sec;
								ts1.min = (uint8)t_Log_Ix.call_log.W2.entry_min;
								ts1.hour = (uint8)t_Log_Ix.call_log.W3.entry_hour;
								
								local_gettime();
								ts2.sec = (uint8)t.second;
								ts2.min = (uint8)t.minute;
								ts2.hour = (uint8)t.hour;
								
								// Set the amount of time it took for completion the call
								t_Log_Ix.call_log.W3.t_completed = time_difference(&ts1, &ts2);
								
								writeLogIxToMRAM(ccTLogIx[el][fl],&t_Log_Ix);			// Update newly set infomation on MRAM
							}
						}
					}
				}

				prev_gcc[el][0] = local_g_cc[el][0];
				prev_gcc[el][1] = local_g_cc[el][1];
			}

			if ((grtop_rfl != 0) && (top_rfl[el] != 0))		// rear door car call
			{
				if ((local_g_rcc[el][0] != prev_grcc[el][0]) || (local_g_rcc[el][1] != prev_grcc[el][1]))
				{
					for (fl=1;fl<=top_rfl[el];fl++)
					{
						fl_ix = (fl-1)/32;
						if ((local_g_rcc[el][fl_ix] & fl_mask[fl]) != (prev_grcc[el][fl_ix] & fl_mask[fl]))
						{
							if ((local_g_rcc[el][fl_ix] & fl_mask[fl]) != 0)
							{
								// Car must not be at the position where the rear car call input is ON
								// This check ensures bad CC input doesn't trigger unncessary calls to get logged
								if ((fl != grp_Car_Stat[el].g_pos) || 
									(((grp_Car_Stat[el].g_run_statusf & rs_DZ) == 0) || ((grp_Car_Stat[el].g_run_statusf & rs_HSF) != 0)))
								{
									clearLogIxBuffer(&t_Log_Ix);
									
									t_Log_Ix.call_log.W1.call_type = 0;						// Car Call type
									t_Log_Ix.call_log.W1.cc_fl = fl;						// Floor# where car call latched to
									t_Log_Ix.call_log.W1.hc_fl = grp_Car_Stat[el].g_pos;	// Position where car call latched from
									t_Log_Ix.call_log.W3.call_dtype = 1;					// Rear door CC
									t_Log_Ix.call_log.W2.car = el;							// Car # where car call latched
									local_gettime();										// Refresh time structure
									t_Log_Ix.call_log.W2.entry_sec = t.second;				// Entry TS sec count
									t_Log_Ix.call_log.W2.entry_min = t.minute;				// Entry TS min count
									t_Log_Ix.call_log.W3.entry_hour = t.hour;				// Entry TS hour count
									
									rccTLogIx[el][fl] = writeNewCallToMRAM(&t_Log_Ix);		// Write the new call log to MRAM,
																							// save the log index location where the call was written
								}
							}
							else
							{
								readLogIxFromMRAM(rccTLogIx[el][fl],&t_Log_Ix);				// Read the call_log from MRAM
								
								// Verify the location of the car and set the valid_log status bit on the log index
								if ((t_Log_Ix.call_log.W1.valid_log == 0) && (grp_Car_Stat[el].g_pos == (int16)t_Log_Ix.call_log.W1.cc_fl))
								{
									t_Log_Ix.call_log.W1.valid_log = 1;
									ts1.sec = (uint8)t_Log_Ix.call_log.W2.entry_sec;
									ts1.min = (uint8)t_Log_Ix.call_log.W2.entry_min;
									ts1.hour = (uint8)t_Log_Ix.call_log.W3.entry_hour;
									
									local_gettime();
									ts2.sec = (uint8)t.second;
									ts2.min = (uint8)t.minute;
									ts2.hour = (uint8)t.hour;
									
									// Set the amount of time it took for completion the call
									t_Log_Ix.call_log.W3.t_completed = time_difference(&ts1, &ts2);
									
									writeLogIxToMRAM(rccTLogIx[el][fl],&t_Log_Ix);			// Update newly set infomation on MRAM
								}
							}
						}
					}
					
					prev_grcc[el][0] = local_g_rcc[el][0];
					prev_grcc[el][1] = local_g_rcc[el][1];
				}
			}
		}
		
		// Check for hall call latch/drop
		
		if ((local_g_up_hall_call[0] != prev_guhc[0]) || (local_g_up_hall_call[1] != prev_guhc[1]))
		{
			for (fl=1;fl<=cons[grtopf];fl++)
			{
				fl_ix = (fl-1)/32;
				if ((local_g_up_hall_call[fl_ix] & fl_mask[fl]) != (prev_guhc[fl_ix] & fl_mask[fl]))
				{
					if ((local_g_up_hall_call[fl_ix] & fl_mask[fl]) != 0)		// Hall call latched
					{
						clearLogIxBuffer(&t_Log_Ix);					// Clear log index buffer
						
						t_Log_Ix.call_log.W1.call_type = 1;				// Hall Call Type
						t_Log_Ix.call_log.W1.hc_fl = fl;				// Floor# where hall call latched
						t_Log_Ix.call_log.W3.hcdir = 0;					// Up direction hall call
						t_Log_Ix.call_log.W3.call_dtype = 0;			// Front door hall call
						local_gettime();								// Refresh time structure
						t_Log_Ix.call_log.W2.entry_sec = t.second;		// Entry TS sec count
						t_Log_Ix.call_log.W2.entry_min = t.minute;		// Entry TS min count
						t_Log_Ix.call_log.W3.entry_hour = t.hour;		// Entry TS hour count
						
						ucTLogIx[fl] = writeNewCallToMRAM(&t_Log_Ix);	// Write the new call log to MRAM,
																		// save the log index location where the call was written
					}
					else									// Verify hall call dropped
					{
						readLogIxFromMRAM(ucTLogIx[fl],&t_Log_Ix);		// Read the call_log from MRAM
	
						// Validate if any one of the elevators in the group cancelled this hall call
						for (el=1;el<=cons[nmbcars];el++)
						{
							if (last_uhc_cancelled[el] == fl)
							{
								// set the car that served hc and set the valid call flag
								t_Log_Ix.call_log.W2.car = el;
								t_Log_Ix.call_log.W1.valid_log = 1;
								
								ts1.sec = (uint8)t_Log_Ix.call_log.W2.entry_sec;
								ts1.min = (uint8)t_Log_Ix.call_log.W2.entry_min;
								ts1.hour = (uint8)t_Log_Ix.call_log.W3.entry_hour;
								
								local_gettime();
								ts2.sec = (uint8)t.second;
								ts2.min = (uint8)t.minute;
								ts2.hour = (uint8)t.hour;
								
								t_Log_Ix.call_log.W3.t_completed = time_difference(&ts1, &ts2);
								
								// write back to MRAM
								writeLogIxToMRAM(ucTLogIx[fl],&t_Log_Ix);
								last_uhc_cancelled[el] = 0;
								break;
							}
						}
					}
				}
			}
			
			prev_guhc[0] = local_g_up_hall_call[0];
			prev_guhc[1] = local_g_up_hall_call[1];
		}
		
		if ((local_g_dn_hall_call[0] != prev_gdhc[0]) || (local_g_dn_hall_call[1] != prev_gdhc[1]))
		{
			for (fl=1;fl<=cons[grtopf];fl++)
			{
				fl_ix = (fl-1)/32;
				if ((local_g_dn_hall_call[fl_ix] & fl_mask[fl]) != (prev_gdhc[fl_ix] & fl_mask[fl]))
				{
					if ((local_g_dn_hall_call[fl_ix] & fl_mask[fl]) != 0)
					{
						clearLogIxBuffer(&t_Log_Ix);					// Clear log index buffer
						
						t_Log_Ix.call_log.W1.call_type = 1;				// Hall Call Type
						t_Log_Ix.call_log.W1.hc_fl = fl;				// Floor# where hall call latched
						t_Log_Ix.call_log.W3.hcdir = 1;					// Dn direction hall call
						t_Log_Ix.call_log.W3.call_dtype = 0;			// Front door hall call
						local_gettime();								// Refresh time structure
						t_Log_Ix.call_log.W2.entry_sec = t.second;		// Entry TS sec count
						t_Log_Ix.call_log.W2.entry_min = t.minute;		// Entry TS min count
						t_Log_Ix.call_log.W3.entry_hour = t.hour;		// Entry TS hour count
					
						dcTLogIx[fl] = writeNewCallToMRAM(&t_Log_Ix);	// Write the new call log to MRAM,
																		// save the log index location where the call was written
					}
					else
					{
						readLogIxFromMRAM(dcTLogIx[fl],&t_Log_Ix);		// Read the call_log from MRAM
	
						// Validate if any one of the elevators in the group cancelled this hall call
						for (el=1;el<=cons[nmbcars];el++)
						{
							if (last_dhc_cancelled[el] == fl)
							{
								// set the car that served hc and set the valid call flag
								t_Log_Ix.call_log.W2.car = el;
								t_Log_Ix.call_log.W1.valid_log = 1;
								
								ts1.sec = (uint8)t_Log_Ix.call_log.W2.entry_sec;
								ts1.min = (uint8)t_Log_Ix.call_log.W2.entry_min;
								ts1.hour = (uint8)t_Log_Ix.call_log.W3.entry_hour;
								
								local_gettime();
								ts2.sec = (uint8)t.second;
								ts2.min = (uint8)t.minute;
								ts2.hour = (uint8)t.hour;
								
								t_Log_Ix.call_log.W3.t_completed = time_difference(&ts1, &ts2);
								
								// write back to MRAM
								writeLogIxToMRAM(dcTLogIx[fl],&t_Log_Ix);
								last_dhc_cancelled[el] = 0;
								break;
							}
						}
					}
				}
			}

			prev_gdhc[0] = local_g_dn_hall_call[0];
			prev_gdhc[1] = local_g_dn_hall_call[1];
		}
		
		// check for hall call latch/drop
		if (grtop_rfl != 0)
		{
			if ((local_g_upr_hall_call[0] != prev_gruhc[0]) || (local_g_upr_hall_call[1] != prev_gruhc[1]))
			{
				for (fl=1;fl<=grtop_rfl;fl++)
				{
					fl_ix = (fl-1)/32;
					if ((local_g_upr_hall_call[fl_ix] & fl_mask[fl]) != (prev_gruhc[fl_ix] & fl_mask[fl]))
					{
						if ((local_g_upr_hall_call[fl_ix] & fl_mask[fl]) != 0)
						{
							clearLogIxBuffer(&t_Log_Ix);					// Clear log index buffer
							
							t_Log_Ix.call_log.W1.call_type = 1;				// Hall Call Type
							t_Log_Ix.call_log.W1.hc_fl = fl;				// Floor# where hall call latched
							t_Log_Ix.call_log.W3.hcdir = 0;					// Up direction hall call
							t_Log_Ix.call_log.W3.call_dtype = 1;			// Rear door hall call
							local_gettime();								// Refresh time structure
							t_Log_Ix.call_log.W2.entry_sec = t.second;		// Entry TS sec count
							t_Log_Ix.call_log.W2.entry_min = t.minute;		// Entry TS min count
							t_Log_Ix.call_log.W3.entry_hour = t.hour;		// Entry TS hour count
							
							rucTLogIx[fl] = writeNewCallToMRAM(&t_Log_Ix);	// Write the new call log to MRAM,
																			// save the log index location where the call was written
						}
						else
						{
							readLogIxFromMRAM(rucTLogIx[fl],&t_Log_Ix);		// Read the call_log from MRAM
		
							// Validate if any one of the elevators in the group cancelled this hall call
							for (el=1;el<=cons[nmbcars];el++)
							{
								if (last_ruhc_cancelled[el] == fl)
								{
									// set the car that served hc and set the valid call flag
									t_Log_Ix.call_log.W2.car = el;
									t_Log_Ix.call_log.W1.valid_log = 1;
									
									ts1.sec = (uint8)t_Log_Ix.call_log.W2.entry_sec;
									ts1.min = (uint8)t_Log_Ix.call_log.W2.entry_min;
									ts1.hour = (uint8)t_Log_Ix.call_log.W3.entry_hour;
									
									local_gettime();
									ts2.sec = (uint8)t.second;
									ts2.min = (uint8)t.minute;
									ts2.hour = (uint8)t.hour;
									
									t_Log_Ix.call_log.W3.t_completed = time_difference(&ts1, &ts2);
									
									// write back to MRAM
									writeLogIxToMRAM(rucTLogIx[fl],&t_Log_Ix);
									last_ruhc_cancelled[el] = 0;
									break;
								}
							}
						}
					}
				}
				
				prev_gruhc[0] = local_g_upr_hall_call[0];
				prev_gruhc[1] = local_g_upr_hall_call[1];
			}
			
			if ((local_g_dnr_hall_call[0] != prev_grdhc[0]) || (local_g_dnr_hall_call[1] != prev_grdhc[1]))
			{
				for (fl=1;fl<=grtop_rfl;fl++)
				{
					fl_ix = (fl-1)/32;
					if ((local_g_dnr_hall_call[fl_ix] & fl_mask[fl]) != (prev_grdhc[fl_ix] & fl_mask[fl]))
					{
						if ((local_g_dnr_hall_call[fl_ix] & fl_mask[fl]) != 0)
						{
							clearLogIxBuffer(&t_Log_Ix);					// Clear log index buffer
							
							t_Log_Ix.call_log.W1.call_type = 1;				// Hall Call Type
							t_Log_Ix.call_log.W1.hc_fl = fl;				// Floor# where hall call latched
							t_Log_Ix.call_log.W3.hcdir = 1;					// Dn direction hall call
							t_Log_Ix.call_log.W3.call_dtype = 1;			// Rear door hall call
							local_gettime();								// Refresh time structure
							t_Log_Ix.call_log.W2.entry_sec = t.second;		// Entry TS sec count
							t_Log_Ix.call_log.W2.entry_min = t.minute;		// Entry TS min count
							t_Log_Ix.call_log.W3.entry_hour = t.hour;		// Entry TS hour count
						
							rdcTLogIx[fl] = writeNewCallToMRAM(&t_Log_Ix);	// Write the new call log to MRAM,
																			// save the log index location where the call was written
						}
						else
						{
							readLogIxFromMRAM(rdcTLogIx[fl],&t_Log_Ix);		// Read the call_log from MRAM
		
							// Validate if any one of the elevators in the group cancelled this hall call
							for (el=1;el<=cons[nmbcars];el++)
							{
								if (last_rdhc_cancelled[el] == fl)
								{
									// set the car that served hc and set the valid call flag
									t_Log_Ix.call_log.W2.car = el;
									t_Log_Ix.call_log.W1.valid_log = 1;
									
									ts1.sec = (uint8)t_Log_Ix.call_log.W2.entry_sec;
									ts1.min = (uint8)t_Log_Ix.call_log.W2.entry_min;
									ts1.hour = (uint8)t_Log_Ix.call_log.W3.entry_hour;
									
									local_gettime();
									ts2.sec = (uint8)t.second;
									ts2.min = (uint8)t.minute;
									ts2.hour = (uint8)t.hour;
									
									t_Log_Ix.call_log.W3.t_completed = time_difference(&ts1, &ts2);
									
									// write back to MRAM
									writeLogIxToMRAM(rdcTLogIx[fl],&t_Log_Ix);
									last_rdhc_cancelled[el] = 0;
									break;
								}
							}
						}
					}
				}
				
				prev_grdhc[0] = local_g_dnr_hall_call[0];
				prev_grdhc[1] = local_g_dnr_hall_call[1];
			}
		}
	}
}

//*****************************************
//	chkDateChanged()
//	Checks if date has changed.
//*****************************************

uint8 chkDateChanged(void)
{
	static uint8 prev_min_cnt;
	static uint8 timer_synced_up;		// Sync up date check timer with rtc min count change, becuase min count changes every 60 secs
	static uint8 init_chk_date_vars = 0;
	static uint16 prev_ts_datechk;
	
	if (init_chk_date_vars == 0)
	{
		local_gettime();
		prev_min_cnt = t.minute;
		prev_ts_datechk = 0;
		timer_synced_up = 0;
		init_chk_date_vars = 1;
	}
	
	if (timer_synced_up == 0)
	{
		// Sync up 60 sec timer with minute count change
	
		if (timer_sec[ts_datechk] != prev_ts_datechk)
		{
			local_gettime();
			if (t.minute != prev_min_cnt)
			{
				timer_synced_up = 1;
				timer_sec[ts_datechk] = 0;
			}
			prev_ts_datechk = timer_sec[ts_datechk];
		}
	}
	else
	{
		if (timer_sec[ts_datechk] >= 60)	// Every minute check for date change
		{
			timer_sec[ts_datechk] = 0;
			local_getdate();
			if ((d.day != old_day) || (d.month != old_month) || (d.year != old_year))
			{
				old_day = d.day;
				old_month = d.month;
				old_year = d.year;
				init_chk_date_vars = 0;			// this will resync 60 second count with rtc minute count change on every date change
				update_TLogVars_on_MRAM();
				return(1);
			}
		}
	}
	return(0);
}

//*****************************************
//	Adds header for new data
//	When date change is detected and to be
//	able to detect new day's log add the
//	header data of 6 words to the log buffer.
//*****************************************

void logNewDateHeader(void)
{
	uint16 prevDayStartIx = 0;
	
	// Write header1 bytes to MRAM
	if (isTLogBufferFull() != 0)
		allocateMoreMRAM();
	
	prevDayStartIx = logTodayStartIx;
	logTodayStartIx = logPutIx;
	
	clearLogIxBuffer(&t_Log_Ix);
	local_getdate();
	t_Log_Ix.header1.W1.day = d.day;
	t_Log_Ix.header1.W1.month = d.month;
	t_Log_Ix.header1.W1.year = (d.year%100);
	t_Log_Ix.header1.W2.prev_day_ix = prevDayStartIx;
	t_Log_Ix.header1.W3.next_day_ix = 0;
	writeLogIxToMRAM(logPutIx++,&t_Log_Ix);
	
	// Write header2 bytes to MRAM
	if (isTLogBufferFull() != 0)
		allocateMoreMRAM();
	
	clearLogIxBuffer(&t_Log_Ix);
	t_Log_Ix.header2.W1.nmb_calls = 0;
	writeLogIxToMRAM(logPutIx++,&t_Log_Ix);
	
	// Update previous day's next link to today's log start ix
	readLogIxFromMRAM(prevDayStartIx,&t_Log_Ix);
	t_Log_Ix.header1.W3.next_day_ix = logTodayStartIx;
	writeLogIxToMRAM(prevDayStartIx++,&t_Log_Ix);
	
	// Save and reset number of calls saved today.
	if (prevDayStartIx == MAX_NMB_LOG_IXS)
		prevDayStartIx = 0;
	readLogIxFromMRAM(prevDayStartIx,&t_Log_Ix);
	t_Log_Ix.header2.W1.nmb_calls = nmbLoggedCallsToday;
	writeLogIxToMRAM(prevDayStartIx++,&t_Log_Ix);
	nmbLoggedCallsToday = 0;
	
	nmbLoggedDays++;
	update_TLogVars_on_MRAM();
}



//*************************************************
//	Traverse thorough the Log Headers
//	to find log for the date specified
//	in the parameter.
//	Argument:	@mm:	Month
//				@dd:	Day
//				@yy:	Year
//				@ptr:	shor int variable pointer
//						sets to value of starting 
//						log ix on MRAM where data's
//						data starts
//	Retuns: 1 when found, otherwise 0
//*************************************************

uint8 findLogForDate(uint8 mm, uint8 dd, uint8 yy, uint16* ptr)
{
	uint16 i=0;
	uint16 prevDayLogIx;
	
	if (nmbLoggedDays != 0)
	{
		readLogIxFromMRAM(logTodayStartIx,&t_Log_Ix);
		prevDayLogIx = logTodayStartIx;
		while (i < nmbLoggedDays)
		{
			if ((t_Log_Ix.header1.W1.month != mm) || (t_Log_Ix.header1.W1.day != dd) || (t_Log_Ix.header1.W1.year != yy))
			{
				prevDayLogIx = t_Log_Ix.header1.W2.prev_day_ix;
				readLogIxFromMRAM(prevDayLogIx,&t_Log_Ix);
			}
			else
			{
				*ptr = prevDayLogIx;
				return(1);
			}
			i++;
		}
	}
	return(0);
}

//*****************************************
//	Reads next log index from MRAM
//	Makes it easier when api is used by
//	dev.
//*****************************************

void getNextLogIx(Log_Ix_Type* ptr)
{
	logGetIx++;
	if (logGetIx == MAX_NMB_LOG_IXS)
		logGetIx = 0;
	readLogIxFromMRAM(logGetIx,ptr);
}

//*****************************************
//	Writes a new call to MRAM
//	Returns traffic log index where
//	new call was written.
//	Arguments:	@ptr: Pointer points to traffic
//			          log index union structure.
//*****************************************

uint16 writeNewCallToMRAM(Log_Ix_Type* ptr)
{
	if (isTLogBufferFull() != 0)
		allocateMoreMRAM();
	writeLogIxToMRAM(logPutIx++,ptr);
	nmbLoggedCallsToday++;
	update_TLogVars_on_MRAM();
	return(lastLogIxWritten);
}

//*****************************************
//	isTLogBufferFull() - Is traffic log full
//	Returns: 1 when log buffer is full,
//			otherwise, 0.
//*****************************************

uint8 isTLogBufferFull(void)
{
	return(((logPutIx == MAX_NMB_LOG_IXS) || (logPutIx == logStartIx)) != 0);
}

//*****************************************
//	Allocate more MRAM
//	When MRAM buffer is full, allocates
//	more memory for logging new calls,
//	if (only one day of data)
//		delete only oldest 500 calls
//	else
//		delete oldest day's data
//*****************************************

void allocateMoreMRAM(void)
{
	uint16 newLogPlaceIx = 0;
	uint16 prevlogStartIx = 0;

	if (logPutIx == MAX_NMB_LOG_IXS)
		logPutIx = 0;
	
	if (logPutIx == logStartIx)		// ? buffer full
	{
		if (nmbLoggedDays == 1)		// Special case when I have history for only one day but I ran out of MRAM.
		{
		
			// Do not delete entire day of history, only free up space for 500 calls, remove oldest 500 calls logged
		
			prevlogStartIx = logStartIx;			// used for move the header over to new the new space
			
			if ((MAX_NMB_LOG_IXS-logPutIx) > 500)	// add space for 500 more calls to be logged
				logStartIx = logStartIx+500;
			else
				logStartIx = 500-(MAX_NMB_LOG_IXS-logPutIx);
			
			logTodayStartIx = logStartIx;
			nmbLoggedCallsToday -= 500;
			
			newLogPlaceIx = logTodayStartIx;
			
			// Move the old 6 words of header to new location
			readLogIxFromMRAM(prevlogStartIx++,&t_Log_Ix);
			writeLogIxToMRAM(newLogPlaceIx++,&t_Log_Ix);
			
			if (newLogPlaceIx == MAX_NMB_LOG_IXS)
				newLogPlaceIx = 0;
			
			readLogIxFromMRAM(prevlogStartIx++,&t_Log_Ix);
			writeLogIxToMRAM(newLogPlaceIx++,&t_Log_Ix);
		}
		else
		{
			readLogIxFromMRAM(logStartIx,&t_Log_Ix);
			logStartIx = t_Log_Ix.header1.W3.next_day_ix;
			nmbLoggedDays--;
		}
	}
}

//*****************************************
//	Write a log index to MRAM
//	Argument: @log_ix: Location on MRAM in
//					   the 3 words format.
//			  @ptr: Pointer points to traffic
//			        log index union structure.
//*****************************************

void writeLogIxToMRAM(uint16 log_ix, Log_Ix_Type* ptr)
{
	uint8 i;
	uint16 k=(log_ix*LOG_IX_SIZE_WORDS);
	
	lastLogIxWritten = log_ix;
	
	for (i=0;i<LOG_IX_SIZE_WORDS;i++)
	{
		wr_mram(MRAM_Traffic_Log_Page,k,ptr->dataW[i]);
		k+=2;
	}
}

//*****************************************
//	Read a log index from MRAM
//	Argument: @log_ix: Location on MRAM in
//					   the 3 words format.
//			  @ptr: Pointer points to traffic
//			        log index union structure.
//*****************************************

void readLogIxFromMRAM(uint16 log_ix, Log_Ix_Type* ptr)
{
	uint8 i;
	uint16 k=(log_ix*LOG_IX_SIZE_WORDS);
	
	for (i=0;i<LOG_IX_SIZE_WORDS;i++)
	{
		ptr->dataW[i] = rd_mram(MRAM_Traffic_Log_Page,k);
		k+=2;
	}
}

//*****************************************
//	Clear traffic log ix buffer
//	Argument: Pointer points to traffic
//			  log index union structure.
//*****************************************

void clearLogIxBuffer(Log_Ix_Type* ptr)
{
	uint8 i;
	for (i=0;i<LOG_IX_SIZE_WORDS;i++)
		ptr->dataW[i] = 0;
}

//*****************************************
//	Copy group data to local
//	To avoid RAM read inconsistancy of vars 
//	that are being set/cleared in different core, 
//	copy data to local core variables.
//*****************************************

void copy_group_data_to_local(void)
{
	uint8 el;
	
	for (el=1;el<=cons[nmbcars];el++)
	{
		if ((grp_Car_Stat[el].g_cc[0] != local_g_cc[el][0]) || (grp_Car_Stat[el].g_cc[1] != local_g_cc[el][1]))
		{
			local_g_cc[el][0] = grp_Car_Stat[el].g_cc[0];
			local_g_cc[el][1] = grp_Car_Stat[el].g_cc[1];
		}
		
		if ((grp_Car_Stat[el].g_rcc[0] != local_g_rcc[el][0]) || (grp_Car_Stat[el].g_rcc[1] != local_g_rcc[el][1]))
		{
			local_g_rcc[el][0] = grp_Car_Stat[el].g_rcc[0];
			local_g_rcc[el][1] = grp_Car_Stat[el].g_rcc[1];
		}
		
		if ((g_up_hall_call[0][0] != local_g_up_hall_call[0]) || (g_up_hall_call[1][0] != local_g_up_hall_call[1]))
		{
			local_g_up_hall_call[0] = g_up_hall_call[0][0];
			local_g_up_hall_call[1] = g_up_hall_call[1][0];
		}
		
		if ((g_dn_hall_call[0][0] != local_g_dn_hall_call[0]) || (g_dn_hall_call[1][0] != local_g_dn_hall_call[1]))
		{
			local_g_dn_hall_call[0] = g_dn_hall_call[0][0];
			local_g_dn_hall_call[1] = g_dn_hall_call[1][0];
		}
		
		if ((g_upr_hall_call[0][0] != local_g_upr_hall_call[0]) || (g_upr_hall_call[1][0] != local_g_upr_hall_call[1]))
		{
			local_g_upr_hall_call[0] = g_upr_hall_call[0][0];
			local_g_upr_hall_call[1] = g_upr_hall_call[1][0];
		}
		
		if ((g_dnr_hall_call[0][0] != local_g_dnr_hall_call[0]) || (g_dnr_hall_call[1][0] != local_g_dnr_hall_call[1]))
		{
			local_g_dnr_hall_call[0] = g_dnr_hall_call[0][0];
			local_g_dnr_hall_call[1] = g_dnr_hall_call[1][0];
		}
	}
}

//*****************************************************
//	Takes two time stamps in HH MM SS format and returns
//	time difference between them in seconds.
//
//	Precondition, 1st ts should always be older of two ts entered
//
//	Argumets:	@ptr1	Pointer to first time stamp structure
//				@ptr2	Pointer to second time stamp structure
//	Returns:	time difference in seconds.
//*****************************************************

uint8 time_difference(void *ptr1, void *ptr2)
{
	uint8 i=0;
	int16 difference=0;
	
	struct timestamp *first_ts = (struct timestamp *)ptr1;
	struct timestamp *second_ts = (struct timestamp *)ptr2;
	
	if ((first_ts->hour == second_ts->hour) && 					// same time stamp
		(first_ts->min == second_ts->min) &&
		(first_ts->sec == second_ts->sec))
		difference = 0;
	else if (first_ts->hour == second_ts->hour)
	{
		if (first_ts->min == second_ts->min)					// Only seconds count changed
			difference = (int16)(second_ts->sec - first_ts->sec);
		else		// Minutes and seconds changed
		{
			if (first_ts->min < second_ts->min)			// First ts should always be older of two time-stamps
			{
				difference += (int16)(60 - first_ts->sec);
				for (i=first_ts->min+1; i<second_ts->min; i++)
					difference += 60;
				difference += (int16)(second_ts->sec);
			}
		}
	}
	else
	{
        if (first_ts->hour < second_ts->hour)	// Same day first hour must always be older
        {
            // first add up seconds within this hour
            difference += 60 - (int16)(first_ts->sec);
            for (i=first_ts->min+1; i<=59; i++)
				difference += 60;   // every minute has 60 seconds

            // add up 3600 every hour in the middle if any
            for (i=first_ts->hour+1; i<second_ts->hour; i++)
                difference += 3600;     // every hour has 3600 seconds

			// in the last add up seconds in the last hour
			for (i=0; i<second_ts->min; i++)
				difference += 60;   // every minute has 60 seconds

			difference += (int16)(second_ts->sec);
        }
        else        							// Transition happened to new day
        {
            // first add up remaining time in the previous day
            difference += 60 - (int16)first_ts->sec;
            for (i=first_ts->min+1; i<=59; i++)
				difference += 60;   // every minute has 60 seconds

            // add up 3600 every hour in the middle if any
            for (i=first_ts->hour+1; i<=23; i++)
                difference += 3600;     // every hour has 3600 seconds

            for (i=0; i<second_ts->hour; i++)
                difference += 3600;     // every hour has 3600 seconds

            for (i=0; i<second_ts->min; i++)
				difference += 60;   // every minute has 60 seconds

			difference += (int16)second_ts->sec;
        }
	}
	
	if (difference > 255)
		difference = 255;
	
	return ((uint8)difference);
}

/* 	Revision History
11/03/15 v7.1.45 as  1. Added routines for logging standard hall calls and car calls traffic data.

*/
