
// *****  Real time clock variables 

#define c_rtc_address 0x70
#define c_rtc_data 0x71
#define c_rtc_sec 0x0
#define c_rtc_min 0x2
#define c_rtc_hour 0x4
#define c_rtc_rega 0xA
#define c_rtc_uip 0x80

#define nmbsvct 10

struct svc_timer {
	unsigned char service;			/* Service use 
										0 = no function
										1 = Security
										2 = Up Peak
										3 = Down Peak
										4 = Next Up
										5 = Parking
										6 = Alternate Parking
									*/
	unsigned char status;			// Bit 0: 0=off,1=on
									// Bit 1: 0=not enabled, 1=enabled
	unsigned char logic;			// Input logic:
									//  0 = input only
									//  1 = input 1 or timer 1
									//  2 = input 1 AND timer 0
									//  3 = input 1 AND timer 1
	unsigned char option;			// future use
    unsigned char month[3];   		/* 1-12 */
    unsigned char day[3];      		/* 1-31 */
	unsigned char md_on_hour[3];	// month/day on hour
	unsigned char md_off_hour[3];	// month/day off hour
	unsigned char md_on_minute[3];	// month/day on minute
	unsigned char md_off_minute[3];	// month/day off minute
    unsigned char on_hour[7];       /* dayofweek on Hours */
    unsigned char off_hour[7];      /* dayofweek off Hours */
    unsigned char on_minute[7];     /* dayofweek on Minutes */
    unsigned char off_minute[7];    /* dayofweek off Minutes */
};
extern struct svc_timer svc_t[];
extern uint16 svc_tmr_chksum[];
typedef struct svc_timer svc_timer_size; // for getting size of svc_timer

extern int16 LT_Flt_ix;

// setup data for destination dispatch hall device locations

extern int8 dd_hd[fl_size];
extern int8 dd_rhd[fl_size];

// Constants that taylor the program to a specific job
extern unsigned char car_bld_no[car_size][2];	// car data	building number
extern int16 vidpos[car_size];          			// video car screen position
extern int16 vidcar[car_size];					// video screen position car


extern uint32 cc_mask[car_size][2];					// car call mask to for special setup on car call latching
extern uint32 rcc_mask[car_size][2];					// rear car call mask to for special setup on car call latching
extern uint32 g_cc_mask[2];					// car call mask to for special setup on car call latching
extern uint32 g_rcc_mask[2];					// rear car call mask to for special setup on car call latching

extern uint32 g_ucmsk[2];			// Bit mask for valid up hall call floors
extern uint32 g_dcmsk[2];			// Bit mask for valid down hall call floors
extern uint32 g_rucmsk[2];			// Bit mask for valid up hall call floors
extern uint32 g_rdcmsk[2];			// Bit mask for valid down hall call floors

extern uint32 g_irucmsk[2];			// Bit mask for valid ir up hall call floors
extern uint32 g_irdcmsk[2];			// Bit mask for valid ir down hall call floors
extern uint32 g_rirucmsk[2];			// Bit mask for valid rear ir up hall call floors
extern uint32 g_rirdcmsk[2];			// Bit mask for valid rear ir down hall call floors

extern int16 front_op[car_size];
extern int16 rear_op[car_size];
extern int16 top_fl[car_size];
extern int16 top_rfl[car_size];

extern int16 bot_rfl;
extern int16 grtop_rfl;
extern int16 grbot_rfl;

extern void test_pins (void);

extern void wr_mram (int16 page, int16 addr, int16 value);
extern int16 rd_mram (int16 page, int16 addr);

// extern uint8 inp(int16 addr);
// extern void outp(int16 addr, uint8 value);
extern void itoa(int32 n, char a[]);
extern void _enable (void);
extern void _disable (void);
extern void delay(uint32 nCount);

extern int16 bcd_to_hex(uint16 value);
extern uint16 hex_to_bcd (uint16 value);

extern int16 findmatch (int16 value, int16 s[], int16 len);


extern void clr_LT_flts(void);
extern void init_LT_flts_ix(void);
extern void rd_LT_flts(int16 LT_Flt_ix, uint16 *f);
extern void Update_LT_Flt(void);

extern int16 calc_floor_to_Top_Speed (int16 dir);
extern int32 atoh(char s[]);
extern unsigned char calc_dayofweek (void);
extern void calculate_button_color (int16 pwrup);
extern void clear_call_data(void);
extern void clear_job_statistics (void);
extern void init_call_data(void);
extern void init_flight_times (int16 fl_run_time);
extern void Read_Grp_Svc_Timer (int16 tmrnmb);
extern void rdcons(void );
extern void rdhoistway(void);
#if (Simulator == 1)
extern void Reset_Hoistway_Init (void);
#endif
extern void Reset_Service_Timers (void);
extern void reset_lwvars(void);
extern void reset_uvars(void);
extern void reset_fvars(void);
extern void rdfvar();
extern void test_hardware(void);
extern void Update_Flt(void);
extern void Update_All_Flt(void);
extern void wrlwvar(void);
extern uint8 verify_lw_data (void);
extern void wruvar(void);
extern void wr_one_fvar(int16 var_offset);
extern void wrfvar(void);
extern void Up_Date_Calls(void );
extern void Update_Control_Flags(void);
extern void Update_Enc_Status_Flag (void);
extern void Update_Flight_Times(void);
extern void Update_Job_Statistics (void);
extern void Update_Pos (void);
extern void Update_scnd_Pos(void);
extern void Wrt_Grp_Svc_Timer (int16 tmrnmb);
#if(Traction == 1)
extern void Update_SEL_Offset_Count (void);
extern void read_drive_from_MRAM(void);
extern void save_drive_to_MRAM(void);
#endif
extern void Update_DPP_EE (void);
extern void Update_Enc_Base_Count (void);
extern void Update_Contrast_Brightness (void);
extern void Wrt_Hoistway(void);
extern void Init_Hoistway(void );
extern void petdog();
extern void disable_dog(void);
extern void enable_dog(void);
extern void rdflts(void );
extern void write_MRAM_code (uint16 code);
extern uint16 get_MRAM_code (void);
extern void calculate_brake(void);
extern void calculate_em_brake(void);
extern void calculate_profile(void);
extern void set_job_min_max_fvars(void);
#if (GALaxy_4 == 1)
extern void set_spb_min_max_fvars(void);
extern void reset_spb_fvars(void);
extern void wr_spb_fvar(void);
extern void rd_spb_fvar(void);
extern void set_nts_min_max_fvars(void);
extern void reset_nts_fvars(void);
extern void wr_nts_fvar(void);
extern void rd_nts_fvar(void);
#endif
extern void read_trace_vars(void);
extern void write_trace_vars(void);
extern void read_spare_ios(void);
extern void write_spare_ios(void);

extern int16 verify_fvars(int16 Table);
extern int16 get_fvars_chksum (int16 Table);
extern int16 chksum_fvars(int16 Table);



