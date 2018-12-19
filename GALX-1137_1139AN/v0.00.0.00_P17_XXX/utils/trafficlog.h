#define LOG_IX_SIZE_WORDS 3			// Number of words used per log index

#define MAX_NMB_LOG_IXS 21845		// Maximum number of log indexes in MRAM
									// 1 Page of MRAM = 128KB = 131072Bytes = 65536(16Bit)Words
									// Each logIx uses 3 (16bit)words of MRAM
									// 65536Words/3Words = 21845 Total log ixs

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
	
}Log_Ix_Type;		// traffic log index

extern Log_Ix_Type t_Log_Ix;

extern void trafficLogControl(void);
extern void init_MRAM_Traffic_Log(void);
extern uint8 findLogForDate(uint8 mm, uint8 dd, uint8 yy, uint16* ptr);
extern void readLogIxFromMRAM(uint16 log_ix, Log_Ix_Type* ptr);
extern void getNextLogIx(Log_Ix_Type* ptr);

extern uint16 logGetIx;
extern uint16 nmbLoggedCallsToday;
extern uint8 last_uhc_cancelled[car_size];
extern uint8 last_dhc_cancelled[car_size];
extern uint8 last_rdhc_cancelled[car_size];
extern uint8 last_ruhc_cancelled[car_size];
