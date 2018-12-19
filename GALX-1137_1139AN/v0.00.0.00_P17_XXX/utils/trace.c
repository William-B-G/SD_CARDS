#define d_TRACE 1
#include "global.h"
/*  This is the trace routine.  Trace data is written to a circular ram buffer and can be displayed to the HI terminal.
*/

uint32 *trace_ptr[10];
char trace_name[10][11];
uint8 trace_record_fault[10];
int16 trace_status[10];
int16 trace_format[10];
int16 trace_auto_start; 		// variable to start trace automatically on power up
uint32 trace_offset[10];
uint32 trace_mask[10];

uint32 tr_data[6] = {0,0,0,0,0,0};
uint32 Trace_pix = 0;
uint32 Trace_gix = 0;

#define max_trace_size 100
//uint32 Trace_Data[max_trace_size][6]; 
uint8 trace_update = 0;
uint8 run_trace = 0;
uint8 trace_complete = 0;
uint8 trace_routine[max_trace_size];
uint16 trace_clock[max_trace_size];

void write_trace (uint8 routine);
void set_mask (int16 num);
void Write_System_Trace(void);		   // Write to next location in memory	
void init_debug_trace (void);
void start_debug_trace (void);
void clear_debug_trace(void);
void clear_trigger_logic (void);
void start_trace (void);

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


unsigned char const trace_display[TR_LAST][9] = 
{
	"NO_TRACE",
	"ATFLOOR1",
	"ATFLOOR2",
	"ATFLOOR3",
	"ATFLOOR4",
	"CHKRUN1 ",
	"CHKRUN2 ",
	"DIRSEL  ",
	"HYDR_CTL",
	"STAT_CTL",
	"OUTP_CTL",
	"SETGRPIO",
	"COMM_CTL",
	"DOOR_CTL",
	"LHTBZCTL",
	"CKMTFLT1",
	"CKMTFLT2",
	"CKMTFLT3",
	"CKMTFLT4",
	"SEC_CTRL",
	"ENC_CTRL",
	"INCTIME1",
	"INCTIME2",
	"INCTIME3",
	"INCTIME4",
	"MOTION1 ",
	"MOTION2 ",
	"MOTION3 ",
	"MOTION4 ",
	"MOTION5 ",
	"MOTION6 ",
	"MOTION7 ",
	"MOTION8 ",
	"MOTION9 ",
	"MOTION10",
	"MOTION11",
	"MOTION12",
	"MOTION13",
	"MOTION14",
	"NEEDSTOP",
	"RESET1  ",
	"RESET2  ",
	"RESET3  ",
	"RESET4  ",
	"RESET5  ",
	"SSTRING1",
	"SSTRING2",
	"SET_ULDL",
	"CHKDOOR ",
	"DOORS   ",
	"RDOORS  ",
	"INSPECT1",
	"INSPECT2",
	"INSPECT3",
	"LMT_VEL ",
	"UPD_DPP ",
};
	
void write_trace (uint8 routine)
{
/*	if ((run_trace == 1) && (trace_complete == 0))
	{
		
		if ((trace_status[0] == 1) && ((((uint32) *trace_ptr[0]) & trace_mask[0]) != (tr_data[0] & trace_mask[0])))
			trace_update = 1;
		if ((trace_status[1] == 1) && ((((uint32) *trace_ptr[1]) & trace_mask[1]) != (tr_data[1] & trace_mask[1])))
			trace_update = 1;
		if ((trace_status[2] == 1) && ((((uint32) *trace_ptr[2]) & trace_mask[2]) != (tr_data[2] & trace_mask[2])))
			trace_update = 1;
		if ((trace_status[3] == 1) && ((((uint32) *trace_ptr[3]) & trace_mask[3]) != (tr_data[3] & trace_mask[3])))
			trace_update = 1;
	
		if (trace_update == 1)
		{
			trace_update = 0;
			trace_routine[Trace_pix] = routine;
			trace_clock[Trace_pix] = (uint16)trace_tmr;
		
			if (trace_status[0] == 1)
			{
				Trace_Data[Trace_pix][0] = (uint32) *trace_ptr[0] & trace_mask[0];
				tr_data[0] = ((uint32) *trace_ptr[0]) & trace_mask[0];
				if (trace_record_fault[0] == 1)
				{
					fault_prog_flag1 = trace_routine[Trace_pix];
					fault_prog_flag2 = (uint8)trace_clock[Trace_pix];
					fault_device_2 = 1;
					record_fault(f_trace);
				}
			}

			if (trace_status[1] == 1)
			{
				Trace_Data[Trace_pix][1] = (uint32) *trace_ptr[1] & trace_mask[1];
				tr_data[1] = ((uint32) *trace_ptr[1]) & trace_mask[1];
				if (trace_record_fault[1] == 1)
				{
					fault_prog_flag1 = trace_routine[Trace_pix];
					fault_prog_flag2 = (uint8)trace_clock[Trace_pix];
					fault_device_2 = 2;
					record_fault(f_trace);
				}
			}

			if (trace_status[2] == 1)
			{
				Trace_Data[Trace_pix][2] = (uint32) *trace_ptr[2] & trace_mask[2];
				tr_data[2] = ((uint32) *trace_ptr[2]) & trace_mask[2];
				if (trace_record_fault[2] == 1)
				{
					fault_prog_flag1 = trace_routine[Trace_pix];
					fault_prog_flag2 = (uint8)trace_clock[Trace_pix];
					fault_device_2 = 3;
					record_fault(f_trace);
				}
			}

			if (trace_status[3] == 1)
			{
				Trace_Data[Trace_pix][3] = (uint32) *trace_ptr[3] & trace_mask[3];
				tr_data[3] = ((uint32) *trace_ptr[3]) & trace_mask[3];
				if (trace_record_fault[3] == 1)
				{
					fault_prog_flag1 = trace_routine[Trace_pix];
					fault_prog_flag2 = (uint8)trace_clock[Trace_pix];
					fault_device_2 = 4;
					record_fault(f_trace);
				}
			}

			if (trace_status[4] == 1)
			{
				Trace_Data[Trace_pix][4] = (uint32) *trace_ptr[4] & trace_mask[4];
				tr_data[4] = ((uint32) *trace_ptr[4]) & trace_mask[4];
			}

			if (trace_status[5] == 1)
			{
				Trace_Data[Trace_pix][5] = ((uint32) *trace_ptr[5]) & trace_mask[5];
				tr_data[5] = ((uint32) *trace_ptr[5]) & trace_mask[5];
			}
			
			if (Trace_pix < (max_trace_size - 1))
				Trace_pix++;
			else
				trace_complete = 1;
		}
	}
*/	
}

void init_debug_trace (void)
{
/*
#if (Traction == 1)
	trace_ptr[0] = (uint32 *)(&DPP_UT[0]); 
    trace_ptr[1] = (uint32 *)(&DPP_UT[1]); 
    trace_ptr[2] = (uint32 *)(&DPP_DT[0]); 
    trace_ptr[3] = (uint32 *)(&DPP_DT[1]); 
    strncpy(trace_name[0],"UT \0",4);
    strncpy(trace_name[1],"UT1\0",4);
    strncpy(trace_name[2],"DT \0",4);
    strncpy(trace_name[3],"DT1\0",4);
    trace_format[0] = 'I';
    trace_mask[0] = 0xFFFFFFFF;
	trace_status[0] = 1;
 	trace_record_fault[0] = 0;
    trace_format[1] = 'I';
    trace_mask[1] = 0xFFFFFFFF;
	trace_status[1] = 1;
 	trace_record_fault[1] = 0;
    trace_format[2] = 'I';
    trace_mask[2] = 0xFFFFFFFF;
	trace_status[2] = 1;
 	trace_record_fault[2] = 0;
    trace_format[3] = 'I';
    trace_mask[3] = 0xFFFFFFFF;
	trace_status[3] = 1;
 	trace_record_fault[3] = 0;
#endif
*/
}

void start_debug_trace (void)
{
/*
	Trace_pix = 0;
	Trace_gix = 0;
	run_trace = 1;
	trace_update = 1;
	trace_complete = 0;
	trace_tmr = 0;
*/	
}

void clear_debug_trace(void)
{
/*
	int i = 0;
	int j = 0;
	for (j=0;j<=5;j++)
	{
		trace_status[j] = 0;
 		trace_record_fault[j] = 0;
		trace_mask[j] = 0;
		tr_data[j] = 0;
		for (i=0;i<max_trace_size;i++)
			Trace_Data[i][j] = 0;
	}
	Trace_gix = 0;
	Trace_pix = 0;
	trace_complete = 0;
	run_trace = 0;
	trace_auto_start = 0;
*/	
}

void set_mask (int16 num)
{
/*
	switch (trace_format[num])
	{
		case 'D':
			trace_mask[num] = 0xFFFFFFFF;
			break;
		case 'B':
			trace_mask[num] = 0xFF000000;
			break;
		case 'H':
			trace_mask[num] = 0xFF000000;
			break;
		case 'W':
			trace_mask[num] = 0xFFFF0000;
			break;
		case 'S':		// Short Integer 16 bits
			trace_mask[num] = 0xFFFF0000;
			break;
		case 'X':
			trace_mask[num] = 0xFFFFFFFF;
			break;
		case 'I':	// Integer 32 bits
			trace_mask[num] = 0xFFFFFFFF;
			break;
		case 'U':
			trace_mask[num] = 0xFFFFFFFF;
			break;
		case 'C':
			trace_mask[num] = 0xFF000000;
			break;
		case 'L':			  
			trace_mask[num] = 0xFFFFFFFF;
			break;
		case 'E':
			trace_mask[num] = 0xFFFFFFFF;
			break;
		case 'F':
			trace_mask[num] = 0xFFFFFFFF;
			break;
		default:
			trace_mask[num] = 0xFFFFFFFF;
			break;
	}  
*/	
}

/*

typedef struct System_Trace {
	uint16 code;			// 0,1
	uint8 count;			// 2
	uint8 day;				// 3
	uint8 mon;				// 4
	int16 year;				// 5,6
	uint8 min;				// 7
	uint8 hour;				// 8
	uint8 sec;				// 9
	uint8 pos;				// 10
	uint8 servf;			// 11	  
	uint8 procf;			// 12	  
	uint8 doorf;			// 13
	uint8 rdoorf;			// 14
	uint8 dpref;			// 15
	uint8 dirf;				// 16
	uint8 empf;				// 17
	uint8 medf;				// 18
	uint8 codebf;			// 19
	uint8 eqf;				// 20
	uint8 firef;			// 21	   
	uint8 rfiref;			// 22
	uint8 hsf;				// 23
	uint8 startf;		   	// 24
	uint8 dcalls;   		// 25
	uint8 estop;			// 26
	uint8 nstopf;			// 27
	uint8 relevel;			// 28
	uint8 stepf;			// 29
	uint8 door_seqf;		// 30
	uint8 next_stop;		// 31   
	uint8 ins_status;		// 32  
	uint8 ncu_door_seq;		// 33
	uint8 device;			// 34
	uint8 prog_flag1;		// 35
	uint8 prog_flag2;		// 36
	uint8 device_2;			// 37
	uint8 iodata [30];		// 38-67
	int32 statusf;			// 68-71
	int32 dpp_count;	 	// 72-75
	int32 sd_count;		// 76-79
	int16 vel_fpm;	 		// 80-81
	int16 dmd_vel_fpm;		// 82-83
	int16 drv_vel_fpm;		// 84-85
	int16 enc_vel_fpm;		// 86-87
	uint8 SPB_service;		// 88
	uint8 SPB_status;		// 89
	int16 SPB_vel;			// 90-91
	int32 SPB_poscnt;		// 92-95
	uint16 SPB_command;		// 96-97
	uint8 SPB_ios[5];		// 98-102

	uint8 fault_bits[4];	// 103-106
	int32 statusf2;			// 107-110
	int16 ss_status;		// 111-112
	int16 power_status;		// 113-114
	int16 NTS_vel;			// 115-116
	uint8 NTS_service;		// 117
	uint8 NTS_status;		// 118
	uint8 NTS_command;		// 119
	uint8 NTS_limit_flt[3];	// 120-122
	uint8 NTS_ios[3];		// 123-125	 keep an even number
	uint32 run_statusf;		// 126-129
 	uint8 nudgst_flags;		// 130		Nudging_Flags = rnudgst, nudgst
	uint8 door_req_flags;	// 131 		Door Request = rdoor_en, rcc_req, rdob_req, rdoor_req, fdoor_en, fcc_req, fdob_req, fdoor_req
	uint8 call_flags;		// 132 		Call_Flags = onward_hc, onward_cc, onward_calls, dcalls 
	uint8 chkrun_st;		// 133
	uint8 chkstart_st;		// 134
	uint8 chklevel_st;		// 135
	uint8 chkdoor_st;		// 136
	uint16 front_sd;		// 137-138
	uint16 rear_sd;			// 139-140
	uint16 motion_tmr;		// 141-142
	int16 PAL_vel;			// 143-144
	uint8 PAL_stat;			// 145
	uint8 ins_servf; 		// 146
	uint8 enc_dir;			// 147
	uint16 drv_cmd;			// 148-149
	uint16 drv_status;		// 150-151
	uint16 drv_mtr_torque;	// 152-153
	uint16 drv_torque_cmd;	// 154-155
	uint16 percent_load;	// 156-157
	uint8 lev_startf;		// 158
	uint8 spare;			// 159
} System_Trace_type;		// 160 * 450 = 72000 bytes




	uint8 enc_dir;			// 148
*/

#define max_trace_ix 500

struct Fault S_Trace[max_trace_ix + 1];

uint16 Trace_ix = 0;
uint8 Trace_Stop = 0;
uint8 Trace_Trigger = 0;
uint8 trace_stop_cnt = 0;
uint16 trace_cnt = 0;
uint16 trace_stop_ix = 0;


uint16 st_cur_flt = 0;
uint16 st_servf = 0;
uint16 st_procf = 0;
uint16 st_front_sd = 0;		
uint16 st_rear_sd = 0;	
uint32 st_run_statusf = 0;		
uint32 st_statusf = 0;
uint32 st_statusf2 = 0;
uint32 st_statusf3 = 0;
uint32 st_statusf4 = 0;
uint8  st_nts_status1 = 0;
uint8  st_nts_status2 = 0;
uint16  st_nts_command1 = 0;
uint8  st_nts_command2 = 0;
uint8  st_nts_doorzone = 0;
uint8  st_nts_limits = 0;
uint8 st_fault_bits[4] = {0,0,0,0};

uint32 trigger_logic = 0;					// adjustable variable
uint32 and_trigger_logic = 0;				// adjustable variable
uint32 trigger_enable = 0;					// adjustable variable

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

uint16 trigger_arm = 0;						// adjustable variable
uint16 trigger_arm_state = 0;

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

int16 trigger_window = 0;					// adjustable variable
int16 trace_interval = 0;					// adjustable variable

uint16 trigger_fault = 0;					// adjustable variable
uint16 trigger_servf = 0;					// adjustable variable
uint16 trigger_procf = 0;					// adjustable variable
uint32 trigger_run_statusf = 0;				// adjustable variable
uint16 trigger_front_sd = 0;				// adjustable variable
uint16 trigger_rear_sd = 0;					// adjustable variable
uint32 trigger_statusf = 0;					// adjustable variable
uint32 trigger_statusf2 = 0;				// adjustable variable
uint32 trigger_statusf3 = 0;				// adjustable variable
uint32 trigger_statusf4 = 0;				// adjustable variable
uint8 trigger_fault_bits[4] = {0,0,0,0};	// adjustable variable

uint32 trigger_set = 0;
int16 trigger_window_count = 0;


//**********************************************
// Write System Trace to ram buffer
//**********************************************
void Write_System_Trace(void)		   // Write to next location in memory	
{
		//  Status data is stored in a circular buffer of Sys_Stat_Size (block)
		//  The index is stored in the first word.  Each status is stored as a multiple bytes
		//  in consecutive order after the index.  The index is incremented after each
		//  write.



/* Trigger Conditions
					note: ^ refers to change of state
	^ Fault
	= Fault
	^ servf
	= servf
	^ procf
	= procf
	^ run_statusf
	= run_statusf
	^ slowndown
	= slowndown
	^ rear slowdown
	= rear slowdown
	^ statusf
	= statusf
	^ statusf2
	= statusf2
	^ Fault Bits 0
	= Fault Bits 0
	^ Fault Bits 1
	= Fault Bits 1
	^ Fault Bits 2
	= Fault Bits 2
	^ Fault Bits 3
	= Fault Bits 3
	^ SPB service
	= SPB status
	^ SPB command
	= NTS service
	^ NTS status
	= NTS command
	
	Trigger_Fault: xxxx = fault
	Trigger_servf: xx = servf
	Trigger_procf: xx = procf
	Trigger_run_statusf: xxxx xxxx = run_statusf
	Trigger_active:  0 = ignore, 1 = active logic
			bit 0: Fault change
			bit 1: Fault match
			bit 2: servf change
			bit 3: servf match
			bit 4: procf change
			bit 5: procf match
			bit 6: run_statusf change
			bit 7: run_statusf match
			bit 8: slowdown change
			bit 9: slowdown match
			bit 10: rear slowdown change
			bit 11: rear slowdown match
			bit 12: statusf change
			bit 13: statusf match
			bit 14: statusf2 change
			bit 15: statusf2 match
			bit 16: Fault Bits 0 change
			bit 17: Fault Bits 0 match
			bit 18: Fault Bits 1 change
			bit 19: Fault Bits 1 match
			bit 20: Fault Bits 2 change
			bit 21: Fault Bits 2 match
			bit 22: Fault Bits 3 change
			bit 23: Fault Bits 3 match
			bit 24: SPB service change
			bit 25: SPB status change
			bit 26: SPB command change
			bit 27: NTS service change
			bit 28: NTS status change
			bit 29: NTS command change
			bit 30: statusf3 change
			bit 31: statusf4 change
			
	Trigger_Logic:  0 = And, 1 = Or
			bit 0: Fault change
			bit 1: Fault match
			bit 2: servf change
			bit 3: servf match
			bit 4: procf change
			bit 5: procf match
			bit 6: run_statusf change
			bit 7: run_statusf match
			bit 8: slowdown change
			bit 9: slowdown match
			bit 10: rear slowdown change
			bit 11: rear slowdown match
			bit 12: statusf change
			bit 13: statusf match
			bit 14: statusf2 change
			bit 15: statusf2 match
			bit 16: Fault Bits 0 change
			bit 17: Fault Bits 0 match
			bit 18: Fault Bits 1 change
			bit 19: Fault Bits 1 match
			bit 20: Fault Bits 2 change
			bit 21: Fault Bits 2 match
			bit 22: Fault Bits 3 change
			bit 23: Fault Bits 3 match
			bit 24: SPB service change
			bit 25: SPB status change
			bit 26: SPB command change
			bit 27: NTS service change
			bit 28: NTS status change
			bit 29: NTS command change
			bit 30: statusf3 change
			bit 31: statusf4 change
*/			
			
			
/*			
	if (((trigger_enable & DELTA_FAULT) != 0) && (Current_Fault != st_cur_flt) && (Current_Fault != 0))
		trigger_set |= DELTA_FAULT;
	if (((trigger_enable & MATCH_FAULT) != 0) && (trigger_fault == Current_Fault) && (Current_Fault != 0))
		trigger_set |= MATCH_FAULT;
		
	if (((trigger_enable & DELTA_SERVF) != 0) && (servf != st_servf))
		trigger_set |= DELTA_SERVF;
	if (((trigger_enable & MATCH_SERVF) != 0) && (trigger_servf == servf))
		trigger_set |= MATCH_SERVF;
		
	if (((trigger_enable & DELTA_PROCF) != 0) && (procf != st_procf))
		trigger_set |= DELTA_PROCF;
	if (((trigger_enable & MATCH_PROCF) != 0) && (trigger_procf == procf))
		trigger_set |= MATCH_PROCF;
		
	if (((trigger_enable & DELTA_RUN_STF) != 0) && (run_statusf != st_run_statusf))
		trigger_set |= DELTA_RUN_STF;
	if (((trigger_enable & MATCH_RUN_STF) != 0) && (trigger_run_statusf == run_statusf))
		trigger_set |= MATCH_RUN_STF;
		
	if (((trigger_enable & DELTA_SD) != 0) && (front_slowdown != st_front_sd))
		trigger_set |= DELTA_SD;
	if (((trigger_enable & MATCH_SD) != 0) && (trigger_front_sd == front_slowdown))
		trigger_set |= MATCH_SD;
		
	if (((trigger_enable & DELTA_RSD) != 0) && (rear_slowdown != st_rear_sd))
		trigger_set |= DELTA_RSD;
	if (((trigger_enable & MATCH_RSD) != 0) && (trigger_rear_sd == rear_slowdown))
		trigger_set |= MATCH_RSD;
		
	if (((trigger_enable & DELTA_STATUSF) != 0) && (statusf != st_statusf))
		trigger_set |= DELTA_STATUSF;
	if (((trigger_enable & MATCH_STATUSF) != 0) && (trigger_statusf == statusf))
		trigger_set |= MATCH_STATUSF;
		
	if (((trigger_enable & DELTA_STATUSF2) != 0) && (statusf2 != st_statusf2))
		trigger_set |= DELTA_STATUSF2;
	if (((trigger_enable & MATCH_STATUSF2) != 0) && (trigger_statusf2 == statusf2))
		trigger_set |= MATCH_STATUSF2;
		
	if (((trigger_enable & DELTA_FLTB0) != 0) && (Fault_Bits[0] != st_fault_bits[0]))
		trigger_set |= DELTA_FLTB0;
	if (((trigger_enable & MATCH_FLTB0) != 0) && (trigger_fault_bits[0] == Fault_Bits[0]))
		trigger_set |= MATCH_FLTB0;
		
	if (((trigger_enable & DELTA_FLTB1) != 0) && (Fault_Bits[1] != st_fault_bits[1]))
		trigger_set |= DELTA_FLTB1;
	if (((trigger_enable & MATCH_FLTB1) != 0) && (trigger_fault_bits[1] == Fault_Bits[1]))
		trigger_set |= MATCH_FLTB1;
		
	if (((trigger_enable & DELTA_FLTB2) != 0) && (Fault_Bits[2] != st_fault_bits[2]))
		trigger_set |= DELTA_FLTB2;
	if (((trigger_enable & MATCH_FLTB2) != 0) && (trigger_fault_bits[2] == Fault_Bits[2]))
		trigger_set |= MATCH_FLTB2;
		
	if (((trigger_enable & DELTA_FLTB3) != 0) && (Fault_Bits[3] != st_fault_bits[3]))
		trigger_set |= DELTA_FLTB3;
	if (((trigger_enable & MATCH_FLTB3) != 0) && (trigger_fault_bits[3] == Fault_Bits[3]))
		trigger_set |= MATCH_FLTB3;
		
	if (((trigger_enable & DELTA_NTSSTAT1) != 0) && (NTS_spi.Status1.R != st_nts_status1))
		trigger_set |= DELTA_NTSSTAT1;
	if (((trigger_enable & DELTA_NTSSTAT2) != 0) && (NTS_spi.Status2.R != st_nts_status2))
		trigger_set |= DELTA_NTSSTAT2;
	if (((trigger_enable & DELTA_NTSCMD1) != 0) && (NTS_spi.Command1.R != st_nts_command1))
		trigger_set |= DELTA_NTSCMD1;
	if (((trigger_enable & DELTA_NTSCMD2) != 0) && (NTS_spi.Command2.R != st_nts_command2))
		trigger_set |= DELTA_NTSCMD2;
	if (((trigger_enable & DELTA_NTSDZ) != 0) && (NTS_spi.doorzone != st_nts_doorzone))
		trigger_set |= DELTA_NTSDZ;
	if (((trigger_enable & DELTA_NTSLIM) != 0) && (NTS_spi.limits != st_nts_limits))
		trigger_set |= DELTA_NTSLIM;
		
	if (((trigger_enable & DELTA_STATUSF3) != 0) && (statusf3 != st_statusf3))
		trigger_set |= DELTA_STATUSF3;

	if (((trigger_enable & DELTA_STATUSF4) != 0) && (statusf4 != st_statusf4))
		trigger_set |= DELTA_STATUSF4;
	
	if (((trigger_arm_state & trigger_arm) != 0) || (trigger_arm == 0))
	{	// trigger is armed
		if ( (((trigger_set & and_trigger_logic) != 0) && ((trigger_set & and_trigger_logic) == and_trigger_logic)) ||
			  		((trigger_set & trigger_logic) != 0))
		{
			if ((Trace_Stop == 0) && (Trace_Trigger == 0))
				Trace_Trigger = 1;
		}
	}


	if (Trace_Stop == 1)
	{
		if (Trace_Trigger == 1)
		{
			Trace_Trigger = 0;			 
			trace_stop_cnt = 0;
			trigger_arm_state = 0;
		}
	}
	
		

	if ((tmr_10_msec > trace_interval) && (Trace_Stop == 0))
	{
		tmr_10_msec = 0;
	
		if (Trace_ix < max_trace_ix)
			Trace_ix ++;	  		// Get the next index location
		else
			Trace_ix = 0;	  // Restart the index	
			
	 	if (Trace_Trigger == 1)
		{
			if (trace_stop_cnt == 0) 
				trace_stop_ix = Trace_ix;
			
			if (trace_stop_cnt > 34)
				Trace_Stop = 1;
			else
				trace_stop_cnt++;
			
		}
  		else
  		{
  			trace_stop_ix = 0;
  		}

		S_Trace[Trace_ix].count = (uint8)trace_cnt;
		S_Trace[Trace_ix].code = Current_Fault;
		S_Trace[Trace_ix].day = d.day;
		S_Trace[Trace_ix].mon = d.month;
		S_Trace[Trace_ix].year = d.year;
		S_Trace[Trace_ix].min = t.minute;
		S_Trace[Trace_ix].hour = t.hour;
		S_Trace[Trace_ix].sec = t.second;
		S_Trace[Trace_ix].pos = (uint8)position;
		S_Trace[Trace_ix].servf = (uint8)servf;
		S_Trace[Trace_ix].procf = (uint8)procf;
		S_Trace[Trace_ix].doorf = (uint8)doorf;
		S_Trace[Trace_ix].rdoorf = (uint8)rdoorf;
		S_Trace[Trace_ix].dpref = (uint8)dpref;
		S_Trace[Trace_ix].dirf = (uint8)dirf;
		S_Trace[Trace_ix].empf = (uint8)empf;
		S_Trace[Trace_ix].medf = (uint8)medf;
		S_Trace[Trace_ix].codebf = (uint8)codebf;
		S_Trace[Trace_ix].eqf = (uint8)eqf;
		S_Trace[Trace_ix].firef = (uint8)firef;
		S_Trace[Trace_ix].rfiref = (uint8)rfiref;
		S_Trace[Trace_ix].hsf = (uint8)hsf;
		S_Trace[Trace_ix].startf = (uint8)startf;
		S_Trace[Trace_ix].dcalls = (uint8)dcalls;
		S_Trace[Trace_ix].estop = (uint8)estop;
		S_Trace[Trace_ix].nstopf = (uint8)nstopf;
		S_Trace[Trace_ix].relevel = (uint8)relevel;
		S_Trace[Trace_ix].stepf = (uint8)stepf;
		S_Trace[Trace_ix].door_seqf = (uint8)Door_Seq_Flags;
		S_Trace[Trace_ix].next_stop = (uint8)next_stop;
		S_Trace[Trace_ix].ins_status = (uint8)ins_status;
		S_Trace[Trace_ix].ncu_door_seq = (uint8)ncu_door_seq;

		if (fault_device != 0)
		{
			S_Trace[Trace_ix].device = (uint8)fault_device;
			fault_device = 0;
		}
		else
			S_Trace[Trace_ix].device = 0;

		if (fault_prog_flag1 != 0)
		{
			S_Trace[Trace_ix].prog_flag1 = (uint8)fault_prog_flag1;
			fault_prog_flag1 = 0;
		}
		else
			S_Trace[Trace_ix].prog_flag1 = 0;

		if (fault_prog_flag2 != 0)
		{
			S_Trace[Trace_ix].prog_flag2 = (uint8)fault_prog_flag2;
			fault_prog_flag2 = 0;
		}
		else
			S_Trace[Trace_ix].prog_flag2 = 0;

		if (fault_device_2 != 0)
		{
			S_Trace[Trace_ix].device_2 = (uint8)fault_device_2;
			fault_device_2 = 0;
		}
		else
			S_Trace[Trace_ix].device_2 = 0;
		
		

		S_Trace[Trace_ix].iodata[0] = iodata[cons[carnmb]][0];
		S_Trace[Trace_ix].iodata[1] = iodata[cons[carnmb]][1];
		S_Trace[Trace_ix].iodata[2] = iodata[cons[carnmb]][2];
		S_Trace[Trace_ix].iodata[3] = iodata[cons[carnmb]][3];
		S_Trace[Trace_ix].iodata[4] = iodata[cons[carnmb]][4];
		S_Trace[Trace_ix].iodata[5] = iodata[cons[carnmb]][5];
		S_Trace[Trace_ix].iodata[6] = iodata[cons[carnmb]][6];
		S_Trace[Trace_ix].iodata[7] = iodata[cons[carnmb]][7];
		S_Trace[Trace_ix].iodata[8] = iodata[cons[carnmb]][8];
		S_Trace[Trace_ix].iodata[9] = iodata[cons[carnmb]][9];
		S_Trace[Trace_ix].iodata[10] = iodata[cons[carnmb]][10];
		S_Trace[Trace_ix].iodata[11] = iodata[cons[carnmb]][11];
		S_Trace[Trace_ix].iodata[12] = iodata[cons[carnmb]][12];
		S_Trace[Trace_ix].iodata[13] = iodata[cons[carnmb]][13];

		S_Trace[Trace_ix].iodata[14] = iodata[cons[carnmb]][18];
		S_Trace[Trace_ix].iodata[15] = iodata[cons[carnmb]][19];
		S_Trace[Trace_ix].iodata[16] = iodata[cons[carnmb]][138];
		S_Trace[Trace_ix].iodata[17] = iodata[cons[carnmb]][140];
		S_Trace[Trace_ix].iodata[18] = iodata[cons[carnmb]][141];
		S_Trace[Trace_ix].iodata[19] = iodata[cons[carnmb]][142];
		S_Trace[Trace_ix].iodata[20] = iodata[cons[carnmb]][143];
		S_Trace[Trace_ix].iodata[21] = iodata[cons[carnmb]][144];
		S_Trace[Trace_ix].iodata[22] = iodata[cons[carnmb]][145];
		S_Trace[Trace_ix].iodata[23] = iodata[cons[carnmb]][146];
		S_Trace[Trace_ix].iodata[24] = iodata[cons[carnmb]][147];
		S_Trace[Trace_ix].iodata[25] = iodata[cons[carnmb]][149];
		S_Trace[Trace_ix].iodata[26] = iodata[cons[carnmb]][153];
		S_Trace[Trace_ix].iodata[27] = iodata[cons[carnmb]][156];
 		S_Trace[Trace_ix].iodata[28] = iodata[cons[carnmb]][150];
		S_Trace[Trace_ix].iodata[29] = iodata[cons[carnmb]][152];

		S_Trace[Trace_ix].statusf = statusf;

		S_Trace[Trace_ix].dpp_count = DPP_Count;
		S_Trace[Trace_ix].sd_count = SD_Count;
		S_Trace[Trace_ix].vel_fpm = vel_fpm;
		S_Trace[Trace_ix].drv_vel_fpm = 0;
		S_Trace[Trace_ix].dmd_vel_fpm = dmd_vel_fpm;
		S_Trace[Trace_ix].enc_vel_fpm = enc_vel_fpm;
		
		S_Trace[Trace_ix].NTS_status1 = NTS_spi.Status1.R;
		S_Trace[Trace_ix].NTS_status2 = NTS_spi.Status1.R;
		S_Trace[Trace_ix].NTS_vel = NTS_spi.velocity;
		S_Trace[Trace_ix].NTS_poscnt= NTS_spi.pos_cnt;
		S_Trace[Trace_ix].NTS_command1 = NTS_spi.Command1.R;
		S_Trace[Trace_ix].NTS_command2 = NTS_spi.Command2.R;
		S_Trace[Trace_ix].NTS_doorzone = NTS_spi.doorzone;
		S_Trace[Trace_ix].NTS_limits = NTS_spi.limits;
		S_Trace[Trace_ix].NTS_vel_dir = NTS_spi.vel_dir;
		
		S_Trace[Trace_ix].unused1 = 0;
		S_Trace[Trace_ix].unused2 = 0;
		

		S_Trace[Trace_ix].fault_bits[0] = Fault_Bits[0];

		S_Trace[Trace_ix].fault_bits[1] = Fault_Bits[1];

		S_Trace[Trace_ix].fault_bits[2] = Fault_Bits[2];
		S_Trace[Trace_ix].fault_bits[3] = Fault_Bits[3];

		S_Trace[Trace_ix].statusf2 = statusf2;
		S_Trace[Trace_ix].ss_status = safety_string_status;
		S_Trace[Trace_ix].power_status = power_status;
		
		S_Trace[Trace_ix].fl_dpp_count = DPP_Floor_Pos[position];
		S_Trace[Trace_ix].unused7 = 0;
		S_Trace[Trace_ix].unused8 = 0;
		S_Trace[Trace_ix].unused9 = 0;
		S_Trace[Trace_ix].unused10 = 0;
		S_Trace[Trace_ix].unused11 = 0;
		S_Trace[Trace_ix].unused12 = 0;
		S_Trace[Trace_ix].unused13 = 0;
		
  		S_Trace[Trace_ix].run_statusf = run_statusf;
		
		S_Trace[Trace_ix].nudgst_flags = Nudgst_Flags;
		S_Trace[Trace_ix].door_req_flags = Door_Req_Flags;
		S_Trace[Trace_ix].call_flags = Call_Flags;
  		S_Trace[Trace_ix].chkrun_st = chkrun_state;
  		S_Trace[Trace_ix].chkstart_st = chkstart_state;
  		S_Trace[Trace_ix].chklevel_st = chklevel_state;
  		S_Trace[Trace_ix].chkdoor_st = chkdoor_state;
		S_Trace[Trace_ix].front_sd = front_slowdown;
		S_Trace[Trace_ix].rear_sd = rear_slowdown;
		S_Trace[Trace_ix].motion_tmr = timers[tmotion];

		S_Trace[Trace_ix].PAL_vel = 0;
		S_Trace[Trace_ix].PAL_stat = 0;
		S_Trace[Trace_ix].ins_servf = ins_servf;
		S_Trace[Trace_ix].enc_dir = (uint8)Enc_dir;
#if (Traction == 1)		
		S_Trace[Trace_ix].drv_cmd = (uint16)(drv_command & 0xffff);
		S_Trace[Trace_ix].drv_status = (uint16)drv_status;
		S_Trace[Trace_ix].drv_mtr_torque = (uint16)drv_motor_torque;
		S_Trace[Trace_ix].drv_torque_cmd = drv_trq_cmd;
#else
		S_Trace[Trace_ix].drv_cmd = 0;
		S_Trace[Trace_ix].drv_status = 0;
		S_Trace[Trace_ix].drv_mtr_torque = 0;
		S_Trace[Trace_ix].drv_torque_cmd = 0;
#endif		
		S_Trace[Trace_ix].percent_load = (uint8)(percent_load/10);

		S_Trace[Trace_ix].lev_startf = (uint8)lev_startf;
		S_Trace[Trace_ix].statusf3 = statusf3;
		S_Trace[Trace_ix].statusf4 = statusf4;
		S_Trace[Trace_ix].spare = 0;


		trace_cnt++;
			
		if (trigger_set != 0)
		{
			if (trigger_window_count >= trigger_window)
				trigger_set = 0;
			trigger_window_count++;
		}
		else
			trigger_window_count = 0;

	}

	st_cur_flt = Current_Fault;
	st_servf = servf;
	st_procf = procf;
	st_run_statusf = run_statusf;
	st_front_sd = front_slowdown;		
	st_rear_sd = rear_slowdown;			
	st_statusf = statusf;
	st_statusf2 = statusf2;
	st_statusf3 = statusf3;
	st_statusf4 = statusf4;
	st_nts_status1 = NTS_spi.Status1.R;
	st_nts_status2 = NTS_spi.Status2.R;
	st_nts_command1 = NTS_spi.Command1.R;
	st_nts_command2 = NTS_spi.Command2.R;
	st_nts_doorzone = NTS_spi.doorzone;
	st_nts_limits = NTS_spi.limits;
	

	st_fault_bits[0] = Fault_Bits[0];
	st_fault_bits[1] = Fault_Bits[1];
	st_fault_bits[2] = Fault_Bits[2];
	st_fault_bits[3] = Fault_Bits[3];
*/
}

//**********************************************
// Clear Trace Logic
//**********************************************
void clear_trigger_logic (void)
{
/*
	trigger_logic = 0;
	trigger_set = 0;
	and_trigger_logic = 0;
	trigger_enable = 0;
	trigger_fault = 0;
	trigger_servf = 0;
	trigger_procf = 0;
	trigger_run_statusf = 0;
	trigger_front_sd = 0;
	trigger_rear_sd = 0;
	trigger_statusf = 0;
	trigger_statusf2 = 0;
	trigger_statusf3 = 0;
	trigger_statusf4 = 0;
	trigger_fault_bits[0] = 0;
	trigger_fault_bits[1] = 0;
	trigger_fault_bits[2] = 0;
	trigger_fault_bits[3] = 0;
	trigger_arm_state = 0;
	trigger_window_count = 0;
*/		
}

//**********************************************
// Start Trace
//**********************************************
void start_trace (void)
{
/*
	Trace_Trigger = 0;
	Trace_Stop = 0;
	trigger_set = 0;
	trigger_arm_state = 0;
	trigger_window_count = 0;
*/		
}

/*

Revision History:

10/6/14 v7.1.12 mhd		1. Changed fault_type variable stops[0] to next_stop because that is what it is.
12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
							12/1/13 v6.1.16 mhd		1. Setup a special trace mechanism to help debugging.  
3/9/17 v8.0.1 mhd		1. Added statusf3 and statusf4 into trace buffer. 

*/