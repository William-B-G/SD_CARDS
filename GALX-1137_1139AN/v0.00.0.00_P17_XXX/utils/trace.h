extern uint32 *trace_ptr[10];
extern char trace_name[10][11];
extern uint8 trace_record_fault[10];
extern int16 trace_status[10];
extern int16 trace_format[10];
extern int16 trace_auto_start; 		// variable to start trace automatically on power up
extern uint32 trace_offset[10];
extern uint32 trace_mask[10];

#define max_trace_size 100
extern uint32 Trace_Data[max_trace_size][6]; 
extern uint8 trace_update;
extern uint8 run_trace;
extern uint8 trace_complete;
extern uint8 trace_routine[max_trace_size];
extern uint16 trace_clock[max_trace_size];

extern uint32 tr_data[6];
extern uint32 Trace_pix;
extern uint32 Trace_gix;

enum 
{
	TR_NOTRACE,
	TR_ATFLOOR1,
	TR_ATFLOOR2,
	TR_ATFLOOR3,
	TR_ATFLOOR4,
	TR_CHKRUN1,
	TR_CHKRUN2,
	TR_DIRSEL,
	TR_HYDRO_CONTROL,
	TR_STATUS_CONTROL,
	TR_OUTPUT_CONTROL,
	TR_SET_GRP_IO,
	TR_COMM_CONTROL,
	TR_DOOR_CONTROL,
	TR_LIGHT_BUZZER_CONTROL,
	TR_CHK_MOTION_FAULT1,
	TR_CHK_MOTION_FAULT2,
	TR_CHK_MOTION_FAULT3,
	TR_CHK_MOTION_FAULT4,
	TR_SECURITY_CONTROL,
	TR_HOISTWAY_CONTROL,
	TR_INCTIME1,
	TR_INCTIME2,
	TR_INCTIME3,
	TR_INCTIME4,
	TR_MOTION1,
	TR_MOTION2,
	TR_MOTION3,
	TR_MOTION4,
	TR_MOTION5,
	TR_MOTION6,
	TR_MOTION7,
	TR_MOTION8,
	TR_MOTION9,
	TR_MOTION10,
	TR_MOTION11,
	TR_MOTION12,
	TR_MOTION13,
	TR_MOTION14,
	TR_NEEDSTOP,
	TR_RESET1,
	TR_RESET2,
	TR_RESET3,
	TR_RESET4,
	TR_RESET5,
	TR_SSTRING1,
	TR_SSTRING2,
	TR_SET_UL_DL,
	TR_CHKDOOR,
	TR_DOORS,
	TR_RDOORS,
	TR_INSPECTION1,
	TR_INSPECTION2,
	TR_INSPECTION3,
	TR_LIMIT_VELOCITY,
	TR_UPDATE_DPP,
	TR_LAST
};

extern unsigned char const trace_display[TR_LAST][9]; 

#define max_trace_ix 500

extern uint16 Trace_ix;
extern uint8 Trace_Stop;
extern uint8 Trace_Trigger;
extern uint8 trace_stop_cnt;
extern uint16 trace_stop_ix;
extern uint16 trace_cnt;

extern uint32 trigger_logic;
extern uint32 and_trigger_logic;
extern uint32 trigger_enable;

#define 	DELTA_FAULT		0x00000001		// fault change
#define		MATCH_FAULT		0x00000002		// fault match
#define		DELTA_SERVF		0x00000004		// servf change
#define		MATCH_SERVF		0x00000008		// servf match
#define		DELTA_PROCF 	0x00000010		// procf change
#define		MATCH_PROCF 	0x00000020		// procf match
#define		DELTA_RUN_STF 	0x00000040		// run_statusf change
#define		MATCH_RUN_STF 	0x00000080		// run_statusf match
#define		DELTA_SD		0x00000100		// slowdown change
#define		MATCH_SD		0x00000200		// slowdown match
#define		DELTA_RSD		0x00000400		// rear slowdown change
#define		MATCH_RSD		0x00000800		// rear slowdown match
#define		DELTA_STATUSF 	0x00001000		// statusf change
#define		MATCH_STATUSF 	0x00002000		// statusf match
#define		DELTA_STATUSF2 	0x00004000		// statusf2 change
#define		MATCH_STATUSF2 	0x00008000		// statusf2 match
#define 	DELTA_FLTB0		0x00010000		// fault bits 0 change
#define		MATCH_FLTB0		0x00020000		// fault bits 0 match
#define 	DELTA_FLTB1		0x00040000		// fault bits 1 change
#define		MATCH_FLTB1		0x00080000		// fault bits 1 match
#define 	DELTA_FLTB2		0x00100000		// fault bits 2 change
#define		MATCH_FLTB2		0x00200000		// fault bits 2 match
#define 	DELTA_FLTB3		0x00400000		// fault bits 3 change
#define		MATCH_FLTB3		0x00800000		// fault bits 3 match
#define		DELTA_NTSSTAT1	0x01000000		// nts status1 change
#define		DELTA_NTSSTAT2	0x02000000		// nts status2 change
#define		DELTA_NTSCMD1	0x04000000		// nts command1 change
#define		DELTA_NTSCMD2	0x08000000		// nts command2 change
#define		DELTA_NTSDZ		0x10000000		// nts doorzone change
#define		DELTA_NTSLIM	0x20000000		// nts limits change
#define		DELTA_STATUSF3 	0x40000000		// statusf change
#define		DELTA_STATUSF4 	0x80000000		// statusf change

extern uint16 trigger_arm;
extern uint16 trigger_arm_state;

#define 	PWRUP_RESET		0x0001			// Power up reset
#define		INIT_ATFLOOR	0x0002			// Initial Atfloor
#define		MOTION_START	0x0004			// Start of motion
#define		INIT_STOP		0x0008			// Initial Stop
#define 	RELEV_START		0x0010			// Relevel Start
#define		DO_START		0x0020			// Door open start
#define		DD_START		0x0040			// Door dwell start
#define		DC_START		0x0080			// Door close start
#define		RDO_START		0x0100			// Rear Door open start
#define		RDD_START		0x0200			// Rear Door dwell start
#define		RDC_START		0x0400			// Rear Door close start
#define 	INS_START		0x0800			// Inspection Start
#define 	SS_START		0x1000			// Safety String Start


extern int16 trigger_window;
extern int16 trace_interval;

extern uint16 trigger_fault;
extern uint16 trigger_servf;
extern uint16 trigger_procf;
extern uint32 trigger_run_statusf;
extern uint16 trigger_front_sd;
extern uint16 trigger_rear_sd;
extern uint32 trigger_statusf;
extern uint32 trigger_statusf2;
extern uint32 trigger_statusf3;
extern uint32 trigger_statusf4;
extern uint8 trigger_fault_bits[];	

extern void Write_System_Trace(void);
extern void init_debug_trace (void);
extern void start_debug_trace (void);
extern void clear_debug_trace(void);
extern void write_trace (uint8 routine);
extern void set_mask (int16 num);
extern void clear_trigger_logic (void);
void start_trace (void);

