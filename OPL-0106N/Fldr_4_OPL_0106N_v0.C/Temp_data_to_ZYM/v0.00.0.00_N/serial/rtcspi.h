extern void init_rtc_mram(void);
extern void local_gettime(void);
extern void local_settime(void);
extern void local_getdate(void);
extern void local_setdate(void);
extern void Write_MRAM_Data(uint32 addr, uint16 data, uint8 dev);
extern uint16 Read_MRAM_Data (uint32 addr, uint8 dev);
extern void Write_MRAM_SR(uint8 status, uint8 dev);
extern uint8 Read_MRAM_SR (uint8 dev);
extern void Write_MRAM_Command(uint8 cmd, uint8 dev);
extern void Read_MRAM_Array (uint32 addr, uint16 *data, uint16 len, uint8 dev);
extern void Write_MRAM_Array(uint32 addr, uint16 *data, uint16 len, uint8 dev);

#define MRWRSR 1 		// write_status_reg 1
#define MRRDSR 5 		// read_status_reg 3
#define MRWREN 6 		// write_enable 6
#define MRWRDI 4 		// write_disable 4
#define MRREAD 3 		// read memory 3
#define MRWRITE 2 		// write memory 2
#define MRSLEEP 0xB9	// sleep mode
#define MRWAKE 0xAB		// wake from sleep mode

struct galtime {
    int8 hour;         /* Hours */
    int8 minute;       /* Minutes */
    int8 second;       /* Seconds */
    int8 hsecond;      /* Hundredths of seconds */
};

struct galdate {
    int8 day;      /* 1-31 */
    int8 month;    /* 1-12 */
    int16  year;     /* 0000 - 9999 */
    int8 dayofweek;/* 1 - 7 (1=Sunday) */
};

extern struct galtime t;
extern struct galdate d, prev_d;

