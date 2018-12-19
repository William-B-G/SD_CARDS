// This file contains utility files	to read and write to external program memory
#define d_UTILS 1
#include "global.h"
#include "fat_filelib.h"

// Variables used in this program for velocity calculations

int32 Dist_Mode1 = 0;
int32 Dist_Mode2 = 0;
int32 Dist_Mode3_Inc = 0;
int32 Dist_Mode3 = 0;
int32 Dist_to_TS = 0;

float Vel_Mode1 = 0.0;
float Vel_Mode1_Prev = 0.;
float Vel_Mode3_Inc_Prev = 0.0;
float Delta_Vel = 0.0;
float Vel_Mode3 = 0.0;
  	
// setup data for destination dispatch hall device locations

int8 dd_hd[fl_size];
int8 dd_rhd[fl_size];
uint32 g_ddmsk[2]={0,0};			// Bit mask for destination dispatch call enabled floors
uint32 g_rddmsk[2]={0,0};			// Bit mask for destination dispatch call enabled floors


// Constants that taylor the program to a specific job
unsigned char car_bld_no[car_size][2];	// car building number
int16 vidpos[car_size];          		// video car screen position
int16 vidcar[car_size];					// video screen position car


uint32 cc_mask[car_size][2];				// car call mask to for special setup on car call latching
uint32 rcc_mask[car_size][2];				// rear car call mask to for special setup on car call latching

uint32 g_cc_mask[2] = {0,0};				// car call mask to for special setup on car call latching
uint32 g_rcc_mask[2] = {0,0};				// rear car call mask to for special setup on car call latching

uint32 g_ucmsk[2]={0,0};			// Bit mask for valid up hall call floors
uint32 g_dcmsk[2]={0,0};			// Bit mask for valid down hall call floors
uint32 g_rucmsk[2]={0,0};		// Bit mask for valid up hall call floors
uint32 g_rdcmsk[2]={0,0};		// Bit mask for valid down hall call floors

uint32 g_irucmsk[2]={0,0};		// Bit mask for valid ir up hall call floors
uint32 g_irdcmsk[2]={0,0};		// Bit mask for valid ir down hall call floors
uint32 g_rirucmsk[2]={0,0};		// Bit mask for valid rear ir up hall call floors
uint32 g_rirdcmsk[2]={0,0};		// Bit mask for valid rear ir down hall call floors

int16 front_op[car_size];
int16 rear_op[car_size];
int16 top_fl[car_size];
int16 top_rfl[car_size];

int16 bot_rfl = 1;
int16 grtop_rfl=0;
int16 grbot_rfl=0;

int8 sema4_gate0_flt = 0;

#define c_floor_height 144.0


int16 LT_Flt_ix = 0;
int16 var_num = 0;


void test_pins (void);

extern void enable_dog (void);
extern void disable_dog (void);

void clr_LT_flts(void);
void init_LT_flts_ix(void);
void rd_LT_flts(int16 LT_Flt_ix, uint16 *f);
void Update_LT_Flt(void);

int16 rd_keybrd (void);
void wr_keybrd (int16 value);
void wr_vga (int32 addr, int16 value);
int16 rd_vga (int32 addr);
void wr_vga_reg (int32 addr, int16 value);
int16 rd_vga_reg (int32 addr);

void wr_mram_array (int16 page, int16 addr, int16 *value, int16 len);
void wr_mram (int16 page, int16 addr, int16 value);
void rd_mram_array (int16 page, int16 addr, int16 *data, int16 len);
int16 rd_mram (int16 page, int16 addr);
void wr_lcd (int16 value, int16 cmd_data);
int16 rd_lcd (int16 cmd_data);

// uint8 inp(int16 addr);
// void outp(int16 addr, uint8 value);
void itoa(int32 n, char a[]);
void _enable (void);
void _disable (void);
void delay(uint32 nCount);

void calculate_brake(void);
void calculate_em_brake(void);
int16 calc_floor_to_Top_Speed (int16 dir);
void calculate_dist_mode1 (void);
void calculate_profile(void);
void calculate_button_color (int16 pwrup);
void calculate_hall_button_color (void);
void calculate_car_button_color (int16 pwrup);
void calculate_hoistway(void);
extern uint16 crc_16(unsigned char *data, int16 cnt);
void calculate_grp_fvars_chksum(void);
void calculate_srv_tmr_chksum(int16 tmrnmb);
unsigned char calc_dayofweek (void);
uint16 hex_to_bcd (uint16 value);
int32 atoh(char s[]);
int16 bcd_to_hex(uint16 value);
int16 findmatch (int16 value, int16 s[], int16 len);
void clear_call_data(void);
void clear_job_statistics (void);
void init_call_data(void);
void init_flight_times (int16 fl_run_time);
void Init_Hoistway(void);
void rdhoistway(void);
void rdcons_std(void );
void rdconsB_std(void );
void rdcons_hr(void );
void rdcons_empty(void );
void rdcons(void );
void rdflts(void );
void reset_lwvars(void);
void reset_uvars(void);
void reset_fvars(void);
void rdfvar();
void read_fvars (void);
void test_hardware (void);
int16 test_MRAM_present(int16 WhichChip);
int16 test_MRAM_present_chip2(void);
int16 mem_test(uint16 Dat_Page, uint16 Dat_Offset);
int16 test_pages(uint16 Dat_Page1, uint16 Dat_Page2);
#if (Simulator == 1)
void Reset_Hoistway_Init (void);
#endif
void Reset_Service_Timers (void);
void Read_Grp_Svc_Timer (int16 tmrnmb);
void set_job_default_fvars(void);
void set_job_min_max_one_fvars(int16 var_offset);
void set_job_min_max_fvars(void);
void Up_Date_Calls(void );
void Update_Control_Flags(void);
void Update_Enc_Status_Flag (void);
void Update_Flight_Times(void);
void Update_Job_Statistics (void);
void Update_Pos(void);
void Update_scnd_Pos(void);
void Update_Flt(void);
void Update_All_Flt(void);
int16 verify_dat(void);
int16 verifyB_dat(void);
void wrlwvar(void);
void wruvar(void);
void wr_one_fvar(int16 var_offset);
void wrfvar(void);
#if (Traction == 1)
void read_drive_from_MRAM(void);
void save_drive_to_MRAM(void);
void Update_SEL_Offset_Count (void);
#endif

void Update_DPP_EE (void);
void Update_Enc_Base_Count (void);
void Update_Contrast_Brightness (void);

void Wrt_Grp_Svc_Timer (int16 tmrnmb);
void Wrt_Hoistway(void);

void write_MRAM_code (uint16 code);
uint16 get_MRAM_code (void);

#if (GALaxy_4 == 1)
void set_spb_min_max_fvars(void);
void reset_spb_fvars(void);
void wr_spb_fvar(void);
void rd_spb_fvar(void);
void set_nts_min_max_fvars(void);
void reset_nts_fvars(void);
void wr_nts_fvar(void);
void rd_nts_fvar(void);
#endif

void reset_mem_bus (void);
void reverse (char s[]);
int16 verify_fvars(int16 Table);
uint8 verify_lw_data (void);
void compare_fvars_chksum(void);
int16 chksum_fvars(int16 Table);
void read_backup_fvars (void);
void backup_fvars(void);
void write_fvars_chksum (uint16 cksum,int16 Table);
int16 get_fvars_chksum (int16 Table);
void read_trace_vars(void);
void write_trace_vars(void);
void read_spare_ios(void);
void write_spare_ios(void);
void wr_mram_cmd(uint8 cmd, uint8 page);



/*
#if ((Traction == 0) && (Tract_OL == 0))
int16 lw_empty[fl_size] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int16 lw_full[fl_size] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char lw_status = 0;	// load weigher status
#endif
*/

// Must keep an even number in this structure for the MRAM read and write
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

struct svc_timer svc_t[nmbsvct];
uint16 svc_tmr_chksum[nmbsvct];
typedef struct svc_timer svc_timer_size; // for getting size of svc_timer


int16 cons_init = 0;
int16 run_calc = 0;


float Volt_ac = 0;
float acosret = 0;
int32 opto_trig_tim = 0;

char datbuf[181], datbuf1[181], datbuf2[181];
char datbuf3[181], datbuf4[181], datbuf5[181];
char charbuf[1]; // single sized string 

int16 tdata[1000];


uint16 MRAM_Day_Dat_Page[7] = {MRAM_SUN_Dat_Page,MRAM_MON_Dat_Page,MRAM_TUE_Dat_Page,MRAM_WED_Dat_Page,MRAM_THR_Dat_Page,MRAM_FRI_Dat_Page,MRAM_SAT_Dat_Page};
uint16 MRAM_Day_Dat_Offset[7] = {MRAM_SUN_Dat_Offset,MRAM_MON_Dat_Offset,MRAM_TUE_Dat_Offset,MRAM_WED_Dat_Offset,MRAM_THR_Dat_Offset,MRAM_FRI_Dat_Offset,MRAM_SAT_Dat_Offset};

int16 t_DPP_Fl_Offset = 0;


// *****  Real time clock variables 

#define c_rtc_address 0x70
#define c_rtc_data 0x71
#define c_rtc_sec 0x0
#define c_rtc_min 0x2
#define c_rtc_hour 0x4
#define c_rtc_dayofweek 0x6
#define c_rtc_day 0x7
#define c_rtc_month 0x8
#define c_rtc_year 0x9
 
// Register A
#define c_rtc_rega 0xA
#define c_rtc_uip 0x80		// Update flag: 1=time is being updated

// Register B
#define c_rtc_regb 0xB
#define c_rtc_set 0x80		// Set flag 1=set data/time
#define c_rtc_dm  0x04		// Data mode 0 = BCD, 1 = BINARY
#define c_rtc_24_12 0x02	// 24/12 hour clock 1=24hr
#define c_rtc_dse 0x01		// Daylight savings time enabled

#define c_rtc_regc 0xC
#define c_rtc_regd 0xD

#define LF 0x0A

//#if (optimization_level <= 2)
//	#define short_delay_1 20
//	#define short_delay_2 40
//	#define delay_1_usec 1
//	#define delay_5_usec 5
//#else
	#define short_delay_1 30	
	#define short_delay_2 70	
	#define delay_1_usec 2
	#define delay_5_usec 10
//#endif

int16 temp = 0;
int16 temp1 = 0;

#define delay1(x)			\
{							\
	for (i=0;i<=x;i++)		\
	{						\
		temp++;				\
		if ((temp & 3) == 3)		\
			temp1++;		\
	}						\
}

// System call declare for 5668 in folder C:\Program Files\Freescale\CW for MPC55xx and MPC56xx 2.7\PowerPC_EABI_Support\MSL\MSL_C\MSL_Common\Include\cstdio.h

void test_pins (void)
{
	uint16 test_in;

	
	SIU.GPDO[RTS2].R = 0;
	SIU.GPDO[RTS4].R = 0;
	SIU.GPDO[RTS5].R = 0;
	test_in = SIU.GPDI[CTS2].B.PDI;
	test_in = SIU.GPDI[CTS4].B.PDI;
	test_in = SIU.GPDI[CTS5].B.PDI;
	
	SIU.GPDO[RTS2].R = 1;
	test_in = SIU.GPDI[CTS2].B.PDI;
	SIU.GPDO[RTS2].R = 0;
	test_in = SIU.GPDI[CTS2].B.PDI;
	
	SIU.GPDO[RTS4].R = 1;
	test_in = SIU.GPDI[CTS4].B.PDI;
	SIU.GPDO[RTS4].R = 0;
	test_in = SIU.GPDI[CTS4].B.PDI;

	SIU.GPDO[RTS5].R = 1;
	test_in = SIU.GPDI[CTS5].B.PDI;
	SIU.GPDO[RTS5].R = 0;
	test_in = SIU.GPDI[CTS5].B.PDI;
	
	
}

//*********************************************************************************
// Write to MRAM Array
// 		addr = Address of memory (byte wide)
//		value = word or byte value to be written
//*********************************************************************************

void wr_mram_array (int16 page, int16 addr, int16 *value, int16 len)
{			
	static union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}adl;					// unsigned integer union
	int16 i = 0;
		

	// Write address to port
	if (page <= 3)
	{
		adl.w[lw] = addr;
		adl.w[hw] = page;
		Write_MRAM_Array(adl.dw, value, len, 0);
	}
	else
	{
		adl.w[lw] = addr;
		adl.w[hw] = (int16)(page - 4);
		Write_MRAM_Array(adl.dw, value, len, 1);
	}
 }

//*********************************************************************************
// Read to MRAM Array
// 		addr = Address of memory (byte wide)
//		value = word or byte value to be written
//*********************************************************************************

void rd_mram_array (int16 page, int16 addr, int16 *data, int16 len)
{
	static union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}adl;					// unsigned integer union
	int16 i = 0;
		

	// Write address to port
	if (page <= 3)
	{
		adl.w[lw] = addr;
		adl.w[hw] = page;
		Read_MRAM_Array(adl.dw, data, len, 0);
	}
	else
	{
		adl.w[lw] = addr;
		adl.w[hw] = (int16)(page - 4);
		Read_MRAM_Array(adl.dw, data, len, 1);
	}
}


//*********************************************************************************
// Write to MRAM
// 		addr = Address of memory
//		value = word or byte value to be written
//*********************************************************************************

void wr_mram (int16 page, int16 addr, int16 value)
{
	
	static union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}adl;					// unsigned integer union
	int16 i = 0;
		
	// Write address to port
	if (page <= 3)
	{
		adl.w[lw] = addr;
		adl.w[hw] = page;
		Write_MRAM_Data(adl.dw, value, 0);
	}
	else
	{
		adl.w[lw] = addr;
		adl.w[hw] = (int16)(page - 4);
		Write_MRAM_Data(adl.dw, value, 1);
	}

}

//*********************************************************************************
// Read to MRAM
// 		addr = Address of memory
//      Return (read value)
//*********************************************************************************

int16 rd_mram (int16 page, int16 addr)
{
	int16 value = 0;
	static union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}adl;					// unsigned integer union
		

	if (page <= 3)
	{
		adl.w[lw] = addr;
		adl.w[hw] = page;
		value =	Read_MRAM_Data (adl.dw, 0);
	}
	else
	{
		adl.w[lw] = addr;
		adl.w[hw] = (int16)(page - 4);
		value =	Read_MRAM_Data (adl.dw, 1);
	}
		
	return(value);
}

//*********************************************************************************
// Write command to MRAM
//      cmd = mram command
//      page = hardware page address
//*********************************************************************************

void wr_mram_cmd(uint8 cmd, uint8 page)
{
	if (page <= 3)
	{
        Write_MRAM_Command(cmd,0);
	}
	else
	{
	    Write_MRAM_Command(cmd,1);
	}
   
}



// ************************************
// PC-104 Bus I/O read
// ************************************
/*                    
uint8 inp(int16 addr)
{
	uint8 value = 0;
	int16 i;
	
	delay1(delay_5_usec);				// 1 usec
	value = (uint8)(0);		// input from port G

    return (value);
}
   
// ************************************
// PC-104 Bus I/O write
// ************************************
                    
void outp(int16 addr, uint8 value)
{
	int16 i = 0;
	
	// Turn off control pins
	delay1(delay_5_usec);				// 1 usec
	
    return;
}

*/
asm void _enable (void)
{
  	wrteei 1;       	/* Make sure external interrupts are enabled  */
}

asm void _disable (void)
{
	wrteei 0			/* Make sure external interrupts are disabled  */
}


/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void delay(uint32 nCount)
{
  	uint32 j;

  	for(j = 0; j <= nCount; j++)
	{
		temp++;		// delay
		if ((temp & 3) == 3)
			temp1++;
	}
}


#if ((Traction == 1) || (Tract_OL == 1))

void calculate_brake(void)
{
		// Brake calculation from Vdc = ((2*sqr(2)/pi)*Vrms cos a
		//     Vdc = (2*sqr(2)/pi) * Vac cos a				 
		//     Vdc = .90031631615706069 * Vac cos a
		//     a = arc cos (Vdc/(.9003163 * Vac))
		// The answer is a value between 0 and pi/2. To get value between
		// 0 and pi, use a = arc cos (((Vdc/(.9003163 * Vac)) - 0.5) * 2) 
		// Divide this number by pi and multiply by 38400 (our time value for a half wave)
		// 18.421x10^6/4 = 4608000 (1/4608000) * 384000 = 0.0083333333333 sec (one half wave timing on the brake board)
		// (38400)/pi = 12223.099629457561787

	Volt_ac = (float)((float)fvars[fvbkaclv] * 0.900316316157);
	Brake_Pick_Trigger_Time = 1;
	Brake_Pick_Start_Trigger = c_max_brake_trigger;
	Brake_Hold_Trigger_Time = 1;
	Brake_Relevel_Trigger_Time = 1;
	Brake_Relevel_Start_Trigger = c_max_brake_trigger;
	Brake_Drop_Trigger_Time = c_max_brake_trigger;
//	opto_trig_tim = (int32)(442386.0/fvars[fvbkaclv]);			// (3686.4 * 120)	 (800usec @ 120vac)
	opto_trig_tim = (int32)((fvars[fvoptodly] * 120)/fvars[fvbkaclv]);
	if (Volt_ac > 0)
	{
		if (Volt_ac >= (float)fvars[fvbkpick])
		{
			acosret = (float)acos((((float)fvars[fvbkpick]/Volt_ac)-0.5) * 2.0);
			if ((acosret >= 0) && (acosret <= 3.141502654))
				Brake_Pick_Trigger_Time = (int32)((double)acosret  * 12223.099629457561787) + opto_trig_tim;		// Brake Pick trigger time
		}
		else 
			Brake_Pick_Trigger_Time = 1;
		if (Volt_ac >= (float)fvars[fvbkpkst])
		{
			acosret = (float)acos((((float)fvars[fvbkpkst]/Volt_ac)-0.5) * 2.0);
			if ((acosret >= 0) && (acosret <= 3.141502654))
				Brake_Pick_Start_Trigger = (int32)(acosret  * 12223.099629457561787) + opto_trig_tim;		// Brake Pick trigger time
		}
		else 
			Brake_Pick_Start_Trigger = c_max_brake_trigger;
		if (Volt_ac >= (float)fvars[fvbkhold])
		{
			acosret = (float)acos((((float)fvars[fvbkhold]/Volt_ac)-0.5) * 2.0);
			if ((acosret >= 0) && (acosret <= 3.141502654))
				Brake_Hold_Trigger_Time = (int32)(acosret * 12223.099629457561787) + opto_trig_tim;		// Brake hold trigger time
		}
		else 
			Brake_Hold_Trigger_Time = 1;
		if (Volt_ac >= (float)fvars[fvbkrelv])
		{
			acosret = (float)acos((((float)fvars[fvbkrelv]/Volt_ac)-0.5) * 2.0);
			if ((acosret >= 0) && (acosret <= 3.141502654))
				Brake_Relevel_Trigger_Time = (int32)(acosret * 12223.099629457561787) + opto_trig_tim;	// Brake relevel trigger time
		}
		else 
			Brake_Relevel_Trigger_Time = 1;
		if (Volt_ac >= (float)fvars[fvbkrvst])
		{
			acosret = (float)acos((((float)fvars[fvbkrvst]/Volt_ac)-0.5) * 2.0);
			if ((acosret >= 0) && (acosret <= 3.141502654))
				Brake_Relevel_Start_Trigger = (int32)(acosret * 12223.099629457561787) + opto_trig_tim;	// Brake relevel trigger time
		}
		else 
			Brake_Relevel_Start_Trigger = c_max_brake_trigger;

		if (Volt_ac >= (float)fvars[fvbkdrop])
		{
			acosret = (float)acos((((float)fvars[fvbkdrop]/Volt_ac)-0.5) * 2.0);
			if ((acosret >= 0) && (acosret <= 3.141502654))
				Brake_Drop_Trigger_Time = (int32)(acosret * 12223.099629457561787) + opto_trig_tim;		// Brake hold trigger time
		}
		else 
			Brake_Drop_Trigger_Time = c_max_brake_trigger;
	}
}

void calculate_em_brake(void)
{
		// Brake calculation from Vdc = ((2*sqr(2)/pi)*Vrms cos a
		//     Vdc = (2*sqr(2)/pi) * Vac cos a				 
		//     Vdc = .90031631615706069 * Vac cos a
		//     a = arc cos (Vdc/(.9003163 * Vac))
		// The answer is a value between 0 and pi/2. To get value between
		// 0 and pi, use a = arc cos (((Vdc/(.9003163 * Vac)) - 0.5) * 2) 
		// Divide this number by pi and multiply by 38400 (our time value for a half wave)
		// 18.421x10^6/4 = 4608000 (1/4608000) * 384000 = 0.0083333333333 sec (one half wave timing on the brake board)
		// (38400)/pi = 12223.099629457561787

	Volt_ac = (float)((float)fvars[fvebkaclv] * 0.900316316157);
	Em_Brake_Pick_Trigger_Time = 1;
	Em_Brake_Hold_Trigger_Time = 1;
//	opto_trig_tim = (int32)(442386.0/fvars[fvbkaclv]);			// (3686.4 * 120)	 (800usec @ 120vac)
	opto_trig_tim = (int32)((fvars[fvebkoptodly] * 120)/fvars[fvebkaclv]);
	if (Volt_ac > 0)
	{
		if (Volt_ac >= (float)fvars[fvebkpick])
		{
			acosret = (float)acos((((float)fvars[fvebkpick]/Volt_ac)-0.5) * 2.0);
			if ((acosret >= 0) && (acosret <= 3.141502654))
				Em_Brake_Pick_Trigger_Time = (int32)(acosret  * 12223.099629457561787) + opto_trig_tim;		// Brake Pick trigger time
		}
		else 
			Em_Brake_Pick_Trigger_Time = 1;
		if (Volt_ac >= (float)fvars[fvebkhold])
		{
			acosret = (float)acos((((float)fvars[fvebkhold]/Volt_ac)-0.5) * 2.0);
			if ((acosret >= 0) && (acosret <= 3.141502654))
				Em_Brake_Hold_Trigger_Time = (int32)(acosret * 12223.099629457561787) + opto_trig_tim;		// Brake hold trigger time
		}
		else 
			Em_Brake_Hold_Trigger_Time = 1;
	}
}

#endif


#if (Traction == 1)

int16 calc_floor_to_Top_Speed (int16 dir)
{
	int16 i;
	int16 tpos;
	int16 last_limit;
	
	last_limit = 0;
	if (cons[speed] > 250)
		last_limit = 1;
	if (cons[speed] > 500)
		last_limit = 2;
	if (cons[speed] > 700)
		last_limit = 3;
	if (cons[speed] > 900)
		last_limit = 4;
	if (cons[speed] > 1100)
		last_limit = 5;
	if (cons[speed] > 1300)
		last_limit = 6;

	
	
	if (dir == 1)
	{
	
		for(i=(cons[topf]-1);i>cons[bottomf];i--)
		{
			if (DPP_Floor_Pos[i] <= (DPP_Floor_Pos[cons[topf]] - (Dist_to_TS + (DPP_UN - DPP_UT[last_limit]))))
			{
				tpos = i;
				break;
			}
		}
		if (i > cons[bottomf])
			return(tpos);
	    else
			return (0);
	}
	else if (dir == 2)
	{
		
		for(i=(cons[bottomf]+1);i<cons[topf];i++)
		{
			if (DPP_Floor_Pos[i] >= (DPP_Floor_Pos[cons[bottomf]] + (Dist_to_TS + (DPP_DT[last_limit] - DPP_DN))))
			{
				tpos = i;
				break;
			}
		}
		if (i < cons[topf])
			return(tpos);
	    else
			return (0);
	}
	else
		return (0);
}

void calculate_dist_mode1 (void)
{
  	int32 i,nmb_steps;
  	float Time_In_Mode1;
	
	Dist_Mode1 = 0;
	Vel_Mode1 = 0.0;
	Vel_Mode1_Prev = 0.0;
	Vel_Step = Vel_Step_Inc;
  	Time_In_Mode1 = ((float)fvars[fvacc] / (float)fvars[fvjrk]);
	nmb_steps = (int32)(Time_In_Mode1 * (float)100.00);
  	for(i=1; i<=nmb_steps; i++)
  	{
  		Vel_Mode1_Prev = Vel_Mode1;
  	    Vel_Mode1 += Vel_Step;
  	    Vel_Step += Vel_Step_Inc;
  	    Dist_Mode1 += (Vel_Mode1 - Vel_Mode1_Prev)/(float)2.0;
  	}
	
}

void calculate_profile(void)
{
  	int32 li,nmb_steps;
  	float Time_In_Mode1, Time_In_Mode1_SF, Time_In_Mode1_EP; 
  	float Accel, Accel_SF, Accel_EP; 
  	float Jerk_M5, Jerk_M5_SF, Jerk_M5_EP;
  	
  	if (fvars[fvenctype] == 4)
  		Vel_Max = ((float)64.0 * (float)(cons[speed])) / (float)60.0;		// pulses per second  = 64 p/ft * ft/min * 1 min/60sec
  	else
  	{
		if (cons[sel_type] == 3)
		 	Vel_Max = (float)(((float)fvars[fvencrpm] * (float)fvars[fvencppr]) / 600.0);
		else
		 	Vel_Max = (float)(((float)fvars[fvencrpm] * (float)fvars[fvencppr] * 4.0) / 600.0);		// previously rpm * ppr * 0.0666666
  	}
  	
  	Pulses_per_Inch = (Vel_Max / ((float)cons[speed] * (float)(0.200)));
  	Em_Decel = ((float)fvars[fvemdec] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
  	Em_Lev_Decel = ((float)fvars[fvemdeclev] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
  	Vel_Level = ((float)fvars[fvlvs] * Pulses_per_Inch / (float)50.000);  // Pules/sec^2
  	Vel_Relevel = ((float)fvars[fvrlvs] * Pulses_per_Inch / (float)50.000);  // Pules/sec^2
  	Vel_Relev_St = ((float)fvars[fvrlvst] * Pulses_per_Inch / (float)50.000);  // Pules/sec^2
	Recovery_Vel = ((float)fvars[fvrecvel] * Pulses_per_Inch) / (float)5.000; 
	Ep_Recov_Vel = ((float)fvars[fveprecvel] * Pulses_per_Inch) / (float)5.000;
	Ep_Top_Speed = ((float)fvars[fvts_ep] * Pulses_per_Inch) / (float)5.000;
	Top_Speed = ((float)fvars[fvtopspeed] * Pulses_per_Inch) / (float)5.000;

	eight_inches = (int32)Pulses_per_Inch * 8;
	five_inches = (int32)Pulses_per_Inch * 5;
	three_inches = (int32)Pulses_per_Inch * 3;
	two_inches = (int32)Pulses_per_Inch * 2;
	dead_zone = (int32)((Pulses_per_Inch * 3.0)/8.0);


	// Normal Floor height speed profile parameters

  	Accel = ((float)fvars[fvacc] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
  	Time_In_Mode1 = ((float)fvars[fvacc] / (float)fvars[fvjrk]);
  	Time_In_Mode3 = ((float)fvars[fvacc] / (float)fvars[fvjrk3]);
  	Time_In_Mode5 = ((float)fvars[fvdec] / (float)fvars[fvdjrk]);
  	Jerk_M5 = ((float)fvars[fvdjrk] * Pulses_per_Inch / (float)5.000);  // Pules/sec^3
  	Deceleration = ((float)fvars[fvdec] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
  	Level_Dist = ((float)fvars[fvm7dst] * Pulses_per_Inch / (float)10.0);
  	Vel_Step_Const = (Accel / (float)100.00);
  	Vel_Step_Inc = (Vel_Step_Const / (Time_In_Mode1 * (float)100.00));
  	Vel_Step_Dec = (Vel_Step_Const / (Time_In_Mode3 * (float)100.00));
  	
  	// Calculate Distance traveled in Mode 1
  	calculate_dist_mode1();
  	
  	// Calculate Velocity Increment and Distance Increment in Mode 3
  	
  	Vel_Mode3_Inc_Prev = 0.0;
  	Vel_Mode3_Inc = 0.0;
  	Vel_Step = Vel_Step_Const;
  	Dist_Mode3_Inc = 0;
  	nmb_steps = (int32)(Time_In_Mode3 * (float)100.00);
  	for(li=1; li<=nmb_steps; li++)
  	{
  		Vel_Mode3_Inc_Prev = Vel_Mode3_Inc;
  	    Vel_Mode3_Inc += Vel_Step;
  	    Vel_Step -= Vel_Step_Dec;
  	    Dist_Mode3_Inc += (Vel_Mode3_Inc - Vel_Mode3_Inc_Prev)/(float)2.0;
  	}
  	
  	Vel_Mode3 = Vel_Max - Vel_Mode3_Inc;
	Delta_Vel = Vel_Mode3 - Vel_Mode1;
	
	// Calculate Distance in Mode 2
	Dist_Mode2 = ((Delta_Vel * Delta_Vel)/(2.0 * Accel)) + ((Delta_Vel * Vel_Mode1)/Accel);
	// Calculate Distance in Mode 3
	Dist_Mode3 = (Vel_Mode3 * Time_In_Mode3) + Dist_Mode3_Inc;  	
	// Calculate Distance to top speed in pulses
  	Dist_to_TS = (Dist_Mode1 + Dist_Mode2 + Dist_Mode3);
  	
  	Vel_Mode5_Dec = ((Deceleration * Time_In_Mode5) -
			   (Jerk_M5 / (float)2.0 * Time_In_Mode5 * Time_In_Mode5));
  	Const_Dist_Mode5 = ((Deceleration /(float) 2.0 * Time_In_Mode5 * Time_In_Mode5) -
		(Jerk_M5 / (float)6.0 * Time_In_Mode5 * Time_In_Mode5 * Time_In_Mode5));


	
	
	
	// Short Floor height speed profile parameters

  	Accel_SF = ((float)fvars[fvacc_sf] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
  	Time_In_Mode1_SF = ((float)fvars[fvacc_sf] / (float)fvars[fvjrk_sf]);
  	Time_In_Mode3_SF = ((float)fvars[fvacc_sf] / (float)fvars[fvjrk3_sf]);
  	Time_In_Mode5_SF = ((float)fvars[fvdec_sf] / (float)fvars[fvdjrk_sf]);
  	Jerk_M5_SF = ((float)fvars[fvdjrk_sf] * Pulses_per_Inch / (float)5.000);  // Pules/sec^3
  	Deceleration_SF = ((float)fvars[fvdec_sf] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
  	Level_Dist_SF = ((float)fvars[fvm7dst_sf] * Pulses_per_Inch / (float)10.0);
  	Vel_Step_Const_SF = (Accel_SF / (float)100.00);
  	Vel_Step_Inc_SF = (Vel_Step_Const_SF / (Time_In_Mode1_SF * (float)100.00));
  	Vel_Step_Dec_SF = (Vel_Step_Const_SF / (Time_In_Mode3_SF * (float)100.00));
  	Vel_Mode3_Inc_SF = 0.0;
  	Vel_Step = Vel_Step_Const_SF;
  	nmb_steps = (int32)(Time_In_Mode3_SF * (float)100.00);
  	for(li=1; li<=nmb_steps; li++)
  	{
  	    Vel_Mode3_Inc_SF += Vel_Step;
  	    Vel_Step -= Vel_Step_Dec_SF;
  	}
  	Vel_Mode5_Dec_SF = ((Deceleration_SF * Time_In_Mode5_SF) -
			   (Jerk_M5_SF / (float)2.0 * Time_In_Mode5_SF * Time_In_Mode5_SF));
  	Const_Dist_Mode5_SF = ((Deceleration_SF / (float)2.0 * Time_In_Mode5_SF * Time_In_Mode5_SF) -
		(Jerk_M5_SF / (float)6.0 * Time_In_Mode5_SF * Time_In_Mode5_SF * Time_In_Mode5_SF));


	// Emergency Power speed profile parameters

  	Accel_EP = ((float)fvars[fvacc_ep] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
  	Time_In_Mode1_EP = ((float)fvars[fvacc_ep] / (float)fvars[fvjrk_ep]);
  	Time_In_Mode3_EP = ((float)fvars[fvacc_ep] / (float)fvars[fvjrk3_ep]);
  	Time_In_Mode5_EP = ((float)fvars[fvdec_ep] / (float)fvars[fvdjrk_ep]);
  	Jerk_M5_EP = ((float)fvars[fvdjrk_ep] * Pulses_per_Inch / (float)5.000);  // Pules/sec^3
  	Deceleration_EP = ((float)fvars[fvdec_ep] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
  	Level_Dist_EP = ((float)fvars[fvm7dst_ep] * Pulses_per_Inch / (float)10.0);
  	Vel_Step_Const_EP = (Accel_EP / (float)100.00);
  	Vel_Step_Inc_EP = (Vel_Step_Const_EP / (Time_In_Mode1_EP * (float)100.00));
  	Vel_Step_Dec_EP = (Vel_Step_Const_EP / (Time_In_Mode3_EP * (float)100.00));
  	Vel_Mode3_Inc_EP = 0.0;
  	Vel_Step = Vel_Step_Const_EP;
  	nmb_steps = (int32)(Time_In_Mode3_EP * (float)100.00);
  	for(li=1; li<=nmb_steps; li++)
  	{
  	    Vel_Mode3_Inc_EP += Vel_Step;
  	    Vel_Step -= Vel_Step_Dec_EP;
  	}
  	Vel_Mode5_Dec_EP = ((Deceleration_EP * Time_In_Mode5_EP) -
			   (Jerk_M5_EP / (float)2.0 * Time_In_Mode5_EP * Time_In_Mode5_EP));
  	Const_Dist_Mode5_EP = ((Deceleration_EP / (float)2.0 * Time_In_Mode5_EP * Time_In_Mode5_EP) -
		(Jerk_M5_EP / (float)6.0 * Time_In_Mode5_EP * Time_In_Mode5_EP * Time_In_Mode5_EP));

  	Accel = ((float)fvars[fvacclim] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
	Vel_Step_FS = (Accel / (float)100.00);
	Vel_Step_Clamp = (Deceleration / (float)100.00);

  	Deceleration_Ins = ((float)fvars[fvinsdecstp] * Pulses_per_Inch / (float)5.000);  // Pules/sec^2
	Vel_Step_Ins = (Deceleration_Ins / (float)100.00);	
	Hoistway_Init_Count = (int32)(Pulses_per_Inch * (float)156.0);			// 13 feet for error	
}
#endif

void calculate_hoistway(void)
{
  	
  	Pulses_per_Inch = 50.8;
	eight_inches = (int32)Pulses_per_Inch * 8;
	five_inches = (int32)Pulses_per_Inch * 5;
	three_inches = (int32)Pulses_per_Inch * 3;
	two_inches = (int32)Pulses_per_Inch * 2;
	dead_zone = (int32)((Pulses_per_Inch * 3.0)/8.0);
	Hoistway_Init_Count = (int32)(Pulses_per_Inch * (float)156.0);			// 13 feet for error	

}

//************************************************
// Calculate Button color for Serial Call buttons
//************************************************

						//  New color table
						//   0,   1,      2,      3,          4,     5,          6,    7,     8,    9,     10,      11,   12,         13,         14,         15
						//    , red, orange, yellow, chartreuse, green, aquamarine, cyan, azure, blue, violet, magenta, rose, rose white, warm white, cool white
	int16 red_tbl[16] =   {  0,1000,   1000,    750,        325,     0,          0,    0,     0,    0,    500,    1000, 1000,       1000,        850,        625};
	int16 green_tbl[16] = {  0,   0,    400,   1000,       1000,  1000,       1000, 1000,   500,    0,      0,       0,    0,       1000,       1000,       1000};
	int16 blue_tbl[16] =  {  0,   0,      0,      0,          0,     0,        400, 1000,  1000, 1000,   1000,    1000,  500,        500,        325,        500};

						//   Previous color table
						//    , red, rose, magenta, violet, blue, azure, cyan, aquamarine, green, chartreuse, yellow, orange, cool white, warm white, rose white
//	int16 red_tbl[16] =   {  0,1000, 1000,    1000,    500,    0,     0,    0,          0,     0,        325,    750,   1000,        625,        850,       1000};
//	int16 green_tbl[16] = {  0,   0,    0,       0,      0,    0,   500, 1000,       1000,  1000,       1000,   1000,    400,       1000,       1000,       1000};
//	int16 blue_tbl[16] =  {  0,   0,  500,    1000,   1000, 1000,  1000, 1000,        400,     0,          0,      0,      0,        500,        325,        500};

void calculate_button_color (int16 pwrup)
{
	calculate_hall_button_color();
	calculate_car_button_color(pwrup);
}


void calculate_hall_button_color (void)
{
	int16 hcbred = 0;
	int16 hcbgreen = 0;
	int16 hcbblue = 0;
	int16 hc_dvr_req = 0;					 
				 
	HcbOnBright = (fvars[fvhcbonbright] * 255)/1000;	   // leave brightness for backward compatibility	
	HcbOffBright = (fvars[fvhcboffbright] * 255)/1000;	

	// Set Up On color for hall call buttons
	if (fvars[fvhcbuponcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbuponcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbuponcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbuponcolor] & 0x0f];

		HcbUpOnRed = (((hcbred * 255)/1000) * fvars[fvhcbuponbright])/1000;		
		HcbUpOnGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbuponbright])/1000;	
		HcbUpOnBlue = (((hcbblue * 255)/1000) * fvars[fvhcbuponbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbUpOnRed = (((fvars[fvhcbuponred] * 255)/1000) * fvars[fvhcbuponbright])/1000;		
		HcbUpOnGreen = (((fvars[fvhcbupongreen] * 255)/1000) * fvars[fvhcbuponbright])/1000;		
		HcbUpOnBlue = (((fvars[fvhcbuponblue] * 255)/1000) * fvars[fvhcbuponbright])/1000;		
	}

	// Set Up Off color for hall call buttons
	if (fvars[fvhcbupoffcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbupoffcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbupoffcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbupoffcolor] & 0x0f];

		HcbUpOffRed = (((hcbred * 255)/1000) * fvars[fvhcbupoffbright])/1000;		
		HcbUpOffGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbupoffbright])/1000;		
		HcbUpOffBlue = (((hcbblue * 255)/1000) * fvars[fvhcbupoffbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbUpOffRed = (((fvars[fvhcbupoffred] * 255)/1000) * fvars[fvhcbupoffbright])/1000;		
		HcbUpOffGreen = (((fvars[fvhcbupoffgreen] * 255)/1000) * fvars[fvhcbupoffbright])/1000;		
		HcbUpOffBlue = (((fvars[fvhcbupoffblue] * 255)/1000) * fvars[fvhcbupoffbright])/1000;		
	}

	// Set color for button on security
	if (fvars[fvhcbseccolor] != 0)
	{	 // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbseccolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbseccolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbseccolor] & 0x0f];

		HcbSecOnRed = (((hcbred * 255)/1000) * fvars[fvhcbsecbright])/1000;		
		HcbSecOnGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbsecbright])/1000;	
		HcbSecOnBlue = (((hcbblue * 255)/1000) * fvars[fvhcbsecbright])/1000;	
	}
	else
	{	// Adjust individual custom colors
		HcbSecOnRed = (((fvars[fvhcbsecred] * 255)/1000) * fvars[fvhcbsecbright])/1000;		
		HcbSecOnGreen = (((fvars[fvhcbsecgreen] * 255)/1000) * fvars[fvhcbsecbright])/1000;	
		HcbSecOnBlue = (((fvars[fvhcbsecblue] * 255)/1000) * fvars[fvhcbsecbright])/1000;	
	}

	if ((HcbUpOnRed != HcbUpOnRed_prev) || (HcbUpOnGreen != HcbUpOnGreen_prev) || (HcbUpOnBlue != HcbUpOnBlue_prev) ||
	   (HcbUpOffRed != HcbUpOffRed_prev) || (HcbUpOffGreen != HcbUpOffGreen_prev) || (HcbUpOffBlue != HcbUpOffBlue_prev) ||
	   (HcbSecOnRed != HcbSecOnRed_prev) || (HcbSecOnGreen != HcbSecOnGreen_prev) || (HcbSecOnBlue != HcbSecOnBlue_prev) ||
	   (HcbOnBright != HcbOnBright_prev) || (HcbOffBright != HcbOffBright_prev))
		hc_dvr_req = 1;

	HcbUpOnRed_prev = HcbUpOnRed;
	HcbUpOnGreen_prev = HcbUpOnGreen;
	HcbUpOnBlue_prev = HcbUpOnBlue;
	HcbUpOffRed_prev = HcbUpOffRed;
	HcbUpOffGreen_prev = HcbUpOffGreen;
	HcbUpOffBlue_prev = HcbUpOffBlue;
	HcbSecOnRed_prev = HcbSecOnRed;
	HcbSecOnGreen_prev = HcbSecOnGreen;
	HcbSecOnBlue_prev = HcbSecOnBlue;

	HcbOnBright_prev = HcbOnBright; 		   // leave brightness for backward compatibility
	HcbOffBright_prev = HcbOffBright; 
	
	 
	ColorUpHCChecksum = 0;
	ColorUpHCChecksum += HcbUpOnRed;
	ColorUpHCChecksum += HcbUpOnGreen;
	ColorUpHCChecksum += HcbUpOnBlue;
	ColorUpHCChecksum += HcbUpOffRed;
	ColorUpHCChecksum += HcbUpOffGreen;
	ColorUpHCChecksum += HcbUpOffBlue;

	ColorSecChecksum = 0;
	ColorSecChecksum += HcbSecOnRed;
	ColorSecChecksum += HcbSecOnGreen;
	ColorSecChecksum += HcbSecOnBlue;

	ColorBrightChecksum = 0;
	ColorBrightChecksum += HcbOnBright;		// leave brightness in checksum for backward compatibility
	ColorBrightChecksum += HcbOffBright;


	// Set Down On color for hall call buttons
	if (fvars[fvhcbdnoncolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbdnoncolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbdnoncolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbdnoncolor] & 0x0f];

		HcbDnOnRed = (((hcbred * 255)/1000) * fvars[fvhcbdnonbright])/1000;		
		HcbDnOnGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbdnonbright])/1000;	
		HcbDnOnBlue = (((hcbblue * 255)/1000) * fvars[fvhcbdnonbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbDnOnRed = (((fvars[fvhcbdnonred] * 255)/1000) * fvars[fvhcbdnonbright])/1000;		
		HcbDnOnGreen = (((fvars[fvhcbdnongreen] * 255)/1000) * fvars[fvhcbdnonbright])/1000;		
		HcbDnOnBlue = (((fvars[fvhcbdnonblue] * 255)/1000) * fvars[fvhcbdnonbright])/1000;		
	}

	// Set Down Off color for hall call buttons
	if (fvars[fvhcbdnoffcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbdnoffcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbdnoffcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbdnoffcolor] & 0x0f];

		HcbDnOffRed = (((hcbred * 255)/1000) * fvars[fvhcbdnoffbright])/1000;		
		HcbDnOffGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbdnoffbright])/1000;		
		HcbDnOffBlue = (((hcbblue * 255)/1000) * fvars[fvhcbdnoffbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbDnOffRed = (((fvars[fvhcbdnoffred] * 255)/1000) * fvars[fvhcbdnoffbright])/1000;		
		HcbDnOffGreen = (((fvars[fvhcbdnoffgreen] * 255)/1000) * fvars[fvhcbdnoffbright])/1000;		
		HcbDnOffBlue = (((fvars[fvhcbdnoffblue] * 255)/1000) * fvars[fvhcbdnoffbright])/1000;		
	}

	if ((HcbDnOnRed != HcbDnOnRed_prev) || (HcbDnOnGreen != HcbDnOnGreen_prev) || (HcbDnOnBlue != HcbDnOnBlue_prev) ||
	   (HcbDnOffRed != HcbDnOffRed_prev) || (HcbDnOffGreen != HcbDnOffGreen_prev) || (HcbDnOffBlue != HcbDnOffBlue_prev))
		hc_dvr_req = 1;

	HcbDnOnRed_prev = HcbDnOnRed;
	HcbDnOnGreen_prev = HcbDnOnGreen;
	HcbDnOnBlue_prev = HcbDnOnBlue;
	HcbDnOffRed_prev = HcbDnOffRed;
	HcbDnOffGreen_prev = HcbDnOffGreen;
	HcbDnOffBlue_prev = HcbDnOffBlue;
	 
	ColorDnHCChecksum = 0;
	ColorDnHCChecksum += HcbDnOnRed;
	ColorDnHCChecksum += HcbDnOnGreen;
	ColorDnHCChecksum += HcbDnOnBlue;
	ColorDnHCChecksum += HcbDnOffRed;
	ColorDnHCChecksum += HcbDnOffGreen;
	ColorDnHCChecksum += HcbDnOffBlue;

	// Set Code Blue On color for hall call buttons
	if (fvars[fvhcbcboncolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbcboncolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbcboncolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbcboncolor] & 0x0f];

		HcbCBOnRed = (((hcbred * 255)/1000) * fvars[fvhcbcbonbright])/1000;		
		HcbCBOnGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbcbonbright])/1000;	
		HcbCBOnBlue = (((hcbblue * 255)/1000) * fvars[fvhcbcbonbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbCBOnRed = (((fvars[fvhcbcbonred] * 255)/1000) * fvars[fvhcbcbonbright])/1000;		
		HcbCBOnGreen = (((fvars[fvhcbcbongreen] * 255)/1000) * fvars[fvhcbcbonbright])/1000;		
		HcbCBOnBlue = (((fvars[fvhcbcbonblue] * 255)/1000) * fvars[fvhcbcbonbright])/1000;		
	}

	// Set Code Blue Off color for hall call buttons
	if (fvars[fvhcbcboffcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbcboffcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbcboffcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbcboffcolor] & 0x0f];

		HcbCBOffRed = (((hcbred * 255)/1000) * fvars[fvhcbcboffbright])/1000;		
		HcbCBOffGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbcboffbright])/1000;	
		HcbCBOffBlue = (((hcbblue * 255)/1000) * fvars[fvhcbcboffbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbCBOffRed = (((fvars[fvhcbcboffred] * 255)/1000) * fvars[fvhcbcboffbright])/1000;		
		HcbCBOffGreen = (((fvars[fvhcbcboffgreen] * 255)/1000) * fvars[fvhcbcboffbright])/1000;		
		HcbCBOffBlue = (((fvars[fvhcbcboffblue] * 255)/1000) * fvars[fvhcbcboffbright])/1000;		
	}

	if ((HcbCBOnRed != HcbCBOnRed_prev) || (HcbCBOnGreen != HcbCBOnGreen_prev) || (HcbCBOnBlue != HcbCBOnBlue_prev) ||
	   (HcbCBOffRed != HcbCBOffRed_prev) || (HcbCBOffGreen != HcbCBOffGreen_prev) || (HcbCBOffBlue != HcbCBOffBlue_prev))
		hc_dvr_req = 1;

	HcbCBOnRed_prev = HcbCBOnRed;
	HcbCBOnGreen_prev = HcbCBOnGreen;
	HcbCBOnBlue_prev = HcbCBOnBlue;
	HcbCBOffRed_prev = HcbCBOffRed;
	HcbCBOffGreen_prev = HcbCBOffGreen;
	HcbCBOffBlue_prev = HcbCBOffBlue;

	ColorCBOnChecksum = 0;
	ColorCBOnChecksum += HcbCBOnRed;
	ColorCBOnChecksum += HcbCBOnGreen;
	ColorCBOnChecksum += HcbCBOnBlue;

	ColorCBOffChecksum = 0;
	ColorCBOffChecksum += HcbCBOffRed;
	ColorCBOffChecksum += HcbCBOffGreen;
	ColorCBOffChecksum += HcbCBOffBlue;


	// Set Vip On color for hall call buttons
	if (fvars[fvhcbviponcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbviponcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbviponcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbviponcolor] & 0x0f];

		HcbVipOnRed =  (((hcbred * 255)/1000) * fvars[fvhcbviponbright])/1000;		
		HcbVipOnGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbviponbright])/1000;	
		HcbVipOnBlue = (((hcbblue * 255)/1000) * fvars[fvhcbviponbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbVipOnRed = (((fvars[fvhcbviponred] * 255)/1000) * fvars[fvhcbviponbright])/1000;		
		HcbVipOnGreen = (((fvars[fvhcbvipongreen] * 255)/1000) * fvars[fvhcbviponbright])/1000;		
		HcbVipOnBlue = (((fvars[fvhcbviponblue] * 255)/1000) * fvars[fvhcbviponbright])/1000;		
	}

	// Set Vip Off color for hall call buttons
	if (fvars[fvhcbvipoffcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbvipoffcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbvipoffcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbvipoffcolor] & 0x0f];

		HcbVipOffRed = (((hcbred * 255)/1000) * fvars[fvhcbvipoffbright])/1000;		
		HcbVipOffGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbvipoffbright])/1000;	
		HcbVipOffBlue = (((hcbblue * 255)/1000) * fvars[fvhcbvipoffbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbVipOffRed = (((fvars[fvhcbvipoffred] * 255)/1000) * fvars[fvhcbvipoffbright])/1000;		
		HcbVipOffGreen = (((fvars[fvhcbvipoffgreen] * 255)/1000) * fvars[fvhcbvipoffbright])/1000;		
		HcbVipOffBlue = (((fvars[fvhcbvipoffblue] * 255)/1000) * fvars[fvhcbvipoffbright])/1000;		
	}

	if ((HcbVipOnRed != HcbVipOnRed_prev) || (HcbVipOnGreen != HcbVipOnGreen_prev) || (HcbVipOnBlue != HcbVipOnBlue_prev) ||
	   (HcbVipOffRed != HcbVipOffRed_prev) || (HcbVipOffGreen != HcbVipOffGreen_prev) || (HcbVipOffBlue != HcbVipOffBlue_prev))
		hc_dvr_req = 1;

	HcbVipOnRed_prev = HcbVipOnRed;
	HcbVipOnGreen_prev = HcbVipOnGreen;
	HcbVipOnBlue_prev = HcbVipOnBlue;
	HcbVipOffRed_prev = HcbVipOffRed;
	HcbVipOffGreen_prev = HcbVipOffGreen;
	HcbVipOffBlue_prev = HcbVipOffBlue;
	 
	ColorVipOnChecksum = 0;
	ColorVipOnChecksum += HcbVipOnRed;
	ColorVipOnChecksum += HcbVipOnGreen;
	ColorVipOnChecksum += HcbVipOnBlue;

	ColorVipOffChecksum = 0;
	ColorVipOffChecksum += HcbVipOffRed;
	ColorVipOffChecksum += HcbVipOffGreen;
	ColorVipOffChecksum += HcbVipOffBlue;


	// Set IRUP On color for hall call buttons
	if (fvars[fvhcbiruponcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbiruponcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbiruponcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbiruponcolor] & 0x0f];

		HcbIRUpOnRed = (((hcbred * 255)/1000) * fvars[fvhcbiruponbright])/1000;		
		HcbIRUpOnGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbiruponbright])/1000;	
		HcbIRUpOnBlue = (((hcbblue * 255)/1000) * fvars[fvhcbiruponbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbIRUpOnRed = (((fvars[fvhcbiruponred] * 255)/1000) * fvars[fvhcbiruponbright])/1000;		
		HcbIRUpOnGreen = (((fvars[fvhcbirupongreen] * 255)/1000) * fvars[fvhcbiruponbright])/1000;		
		HcbIRUpOnBlue = (((fvars[fvhcbiruponblue] * 255)/1000) * fvars[fvhcbiruponbright])/1000;		
	}

	// Set IRUp Off color for hall call buttons
	if (fvars[fvhcbirupoffcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbirupoffcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbirupoffcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbirupoffcolor] & 0x0f];

		HcbIRUpOffRed = (((hcbred * 255)/1000) * fvars[fvhcbirupoffbright])/1000;		
		HcbIRUpOffGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbirupoffbright])/1000;		
		HcbIRUpOffBlue = (((hcbblue * 255)/1000) * fvars[fvhcbirupoffbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbIRUpOffRed = (((fvars[fvhcbirupoffred] * 255)/1000) * fvars[fvhcbirupoffbright])/1000;		
		HcbIRUpOffGreen = (((fvars[fvhcbirupoffgreen] * 255)/1000) * fvars[fvhcbirupoffbright])/1000;		
		HcbIRUpOffBlue = (((fvars[fvhcbirupoffblue] * 255)/1000) * fvars[fvhcbirupoffbright])/1000;		
	}

	// Set IRDown On color for hall call buttons
	if (fvars[fvhcbirdnoncolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbirdnoncolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbirdnoncolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbirdnoncolor] & 0x0f];

		HcbIRDnOnRed = (((hcbred * 255)/1000) * fvars[fvhcbirdnonbright])/1000;		
		HcbIRDnOnGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbirdnonbright])/1000;	
		HcbIRDnOnBlue = (((hcbblue * 255)/1000) * fvars[fvhcbirdnonbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbIRDnOnRed = (((fvars[fvhcbirdnonred] * 255)/1000) * fvars[fvhcbirdnonbright])/1000;		
		HcbIRDnOnGreen = (((fvars[fvhcbirdnongreen] * 255)/1000) * fvars[fvhcbirdnonbright])/1000;		
		HcbIRDnOnBlue = (((fvars[fvhcbirdnonblue] * 255)/1000) * fvars[fvhcbirdnonbright])/1000;		
	}

	// Set IRDown Off color for hall call buttons
	if (fvars[fvhcbirdnoffcolor] != 0)
	{	   // Use standard colors from table
		hcbred = red_tbl[fvars[fvhcbirdnoffcolor] & 0x0f];
		hcbgreen = green_tbl[fvars[fvhcbirdnoffcolor] & 0x0f];
		hcbblue = blue_tbl[fvars[fvhcbirdnoffcolor] & 0x0f];

		HcbIRDnOffRed = (((hcbred * 255)/1000) * fvars[fvhcbirdnoffbright])/1000;		
		HcbIRDnOffGreen = (((hcbgreen * 255)/1000) * fvars[fvhcbirdnoffbright])/1000;		
		HcbIRDnOffBlue = (((hcbblue * 255)/1000) * fvars[fvhcbirdnoffbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		HcbIRDnOffRed = (((fvars[fvhcbirdnoffred] * 255)/1000) * fvars[fvhcbirdnoffbright])/1000;		
		HcbIRDnOffGreen = (((fvars[fvhcbirdnoffgreen] * 255)/1000) * fvars[fvhcbirdnoffbright])/1000;		
		HcbIRDnOffBlue = (((fvars[fvhcbirdnoffblue] * 255)/1000) * fvars[fvhcbirdnoffbright])/1000;		
	}

	if ((HcbIRUpOnRed != HcbIRUpOnRed_prev) || (HcbIRUpOnGreen != HcbIRUpOnGreen_prev) || (HcbIRUpOnBlue != HcbIRUpOnBlue_prev) ||
	   (HcbIRUpOffRed != HcbIRUpOffRed_prev) || (HcbIRUpOffGreen != HcbIRUpOffGreen_prev) || (HcbIRUpOffBlue != HcbIRUpOffBlue_prev) ||
	   (HcbIRDnOnRed != HcbIRDnOnRed_prev) || (HcbIRDnOnGreen != HcbIRDnOnGreen_prev) || (HcbIRDnOnBlue != HcbIRDnOnBlue_prev) ||
	   (HcbIRDnOffRed != HcbIRDnOffRed_prev) || (HcbIRDnOffGreen != HcbIRDnOffGreen_prev) || (HcbIRDnOffBlue != HcbIRDnOffBlue_prev))
		hc_dvr_req = 1;

	HcbIRUpOnRed_prev = HcbIRUpOnRed;
	HcbIRUpOnGreen_prev = HcbIRUpOnGreen;
	HcbIRUpOnBlue_prev = HcbIRUpOnBlue;
	HcbIRUpOffRed_prev = HcbIRUpOffRed;
	HcbIRUpOffGreen_prev = HcbIRUpOffGreen;
	HcbIRUpOffBlue_prev = HcbIRUpOffBlue;

	HcbIRDnOnRed_prev = HcbIRDnOnRed;
	HcbIRDnOnGreen_prev = HcbIRDnOnGreen;
	HcbIRDnOnBlue_prev = HcbIRDnOnBlue;
	HcbIRDnOffRed_prev = HcbIRDnOffRed;
	HcbIRDnOffGreen_prev = HcbIRDnOffGreen;
	HcbIRDnOffBlue_prev = HcbIRDnOffBlue;

	ColorIRDnChecksum = 0;
	ColorIRDnChecksum += HcbIRDnOnRed;
	ColorIRDnChecksum += HcbIRDnOnGreen;
	ColorIRDnChecksum += HcbIRDnOnBlue;
	ColorIRDnChecksum += HcbIRDnOffRed;
	ColorIRDnChecksum += HcbIRDnOffGreen;
	ColorIRDnChecksum += HcbIRDnOffBlue;

	ColorIRUpChecksum = 0;
	ColorIRUpChecksum += HcbIRUpOnRed;
	ColorIRUpChecksum += HcbIRUpOnGreen;
	ColorIRUpChecksum += HcbIRUpOnBlue;
	ColorIRUpChecksum += HcbIRUpOffRed;
	ColorIRUpChecksum += HcbIRUpOffGreen;
	ColorIRUpChecksum += HcbIRUpOffBlue;


	if (hc_dvr_req == 1)
		send_hc_dvr_req(1);
	hc_dvr_req = 0;

}

void calculate_car_button_color (int16 pwrup)
{
	int16 ccred = 0;
	int16 ccgreen = 0;
	int16 ccblue = 0;		

	// ********************************
	// Car Call Colors
	// ********************************

	ccOnBright = (fvars[fvcconbright] * 255)/1000;		
	ccOffBright = (fvars[fvccoffbright] * 255)/1000;	

	// Set On color for hall call buttons
	if (fvars[fvcconcolor] != 0)
	{	   // Use standard colors from table
		ccred = red_tbl[fvars[fvcconcolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvcconcolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvcconcolor] & 0x0f];

		ccOnRed = (((ccred * 255)/1000) * fvars[fvcconbright])/1000;		
		ccOnGreen = (((ccgreen * 255)/1000) * fvars[fvcconbright])/1000;	
		ccOnBlue = (((ccblue * 255)/1000) * fvars[fvcconbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		ccOnRed = (((fvars[fvcconred] * 255)/1000) * fvars[fvcconbright])/1000;		
		ccOnGreen = (((fvars[fvccongreen] * 255)/1000) * fvars[fvcconbright])/1000;		
		ccOnBlue = (((fvars[fvcconblue] * 255)/1000) * fvars[fvcconbright])/1000;		
	}

	// Set Off color for hall call buttons
	if (fvars[fvccoffcolor] != 0)
	{	   // Use standard colors from table
		ccred = red_tbl[fvars[fvccoffcolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvccoffcolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvccoffcolor] & 0x0f];

		ccOffRed = (((ccred * 255)/1000) * fvars[fvccoffbright])/1000;		
		ccOffGreen = (((ccgreen * 255)/1000) * fvars[fvccoffbright])/1000;		
		ccOffBlue = (((ccblue * 255)/1000) * fvars[fvccoffbright])/1000;		
	}
	else
	{	// Adjust individual custom colors
		ccOffRed = (((fvars[fvccoffred] * 255)/1000) * fvars[fvccoffbright])/1000;		
		ccOffGreen = (((fvars[fvccoffgreen] * 255)/1000) * fvars[fvccoffbright])/1000;		
		ccOffBlue = (((fvars[fvccoffblue] * 255)/1000) * fvars[fvccoffbright])/1000;		
	}

	// Set color for button on security
	if (fvars[fvccseccolor] != 0)
	{	 // Use standard colors from table
		ccred = red_tbl[fvars[fvccseccolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvccseccolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvccseccolor] & 0x0f];

		ccSecOnRed = (((ccred * 255)/1000) * fvars[fvccsecbright])/1000;		
		ccSecOnGreen = (((ccgreen * 255)/1000) * fvars[fvccsecbright])/1000;	
		ccSecOnBlue = (((ccblue * 255)/1000) * fvars[fvccsecbright])/1000;	
	}
	else
	{	// Adjust individual custom colors
		ccSecOnRed = (((fvars[fvccsecred] * 255)/1000) * fvars[fvccsecbright])/1000;		
		ccSecOnGreen = (((fvars[fvccsecgreen] * 255)/1000) * fvars[fvccsecbright])/1000;	
		ccSecOnBlue = (((fvars[fvccsecblue] * 255)/1000) * fvars[fvccsecbright])/1000;	
	}

	// Set color for button on attendant up
	if (fvars[fvccattupcolor] != 0)
	{	 // Use standard colors from table
		ccred = red_tbl[fvars[fvccattupcolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvccattupcolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvccattupcolor] & 0x0f];

		ccAttUpOnRed = (((ccred * 255)/1000) * fvars[fvccattupbright])/1000;		
		ccAttUpOnGreen = (((ccgreen * 255)/1000) * fvars[fvccattupbright])/1000;	
		ccAttUpOnBlue = (((ccblue * 255)/1000) * fvars[fvccattupbright])/1000;	
	}
	else
	{	// Adjust individual custom colors
		ccAttUpOnRed = (((fvars[fvccattupred] * 255)/1000) * fvars[fvccattupbright])/1000;		
		ccAttUpOnGreen = (((fvars[fvccattupgreen] * 255)/1000) * fvars[fvccattupbright])/1000;	
		ccAttUpOnBlue = (((fvars[fvccattupblue] * 255)/1000) * fvars[fvccattupbright])/1000;	
	}

	// Set color for button on attendant Dn
	if (fvars[fvccattdncolor] != 0)
	{	 // Use standard colors from table
		ccred = red_tbl[fvars[fvccattdncolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvccattdncolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvccattdncolor] & 0x0f];

		ccAttDnOnRed = (((ccred * 255)/1000) * fvars[fvccattdnbright])/1000;		
		ccAttDnOnGreen = (((ccgreen * 255)/1000) * fvars[fvccattdnbright])/1000;	
		ccAttDnOnBlue = (((ccblue * 255)/1000) * fvars[fvccattdnbright])/1000;	
	}
	else
	{	// Adjust individual custom colors
		ccAttDnOnRed = (((fvars[fvccattdnred] * 255)/1000) * fvars[fvccattdnbright])/1000;		
		ccAttDnOnGreen = (((fvars[fvccattdngreen] * 255)/1000) * fvars[fvccattdnbright])/1000;	
		ccAttDnOnBlue = (((fvars[fvccattdnblue] * 255)/1000) * fvars[fvccattdnbright])/1000;	
	}

	// Set color for button on fire
	if (fvars[fvcopfirecolor] != 0)
	{	 // Use standard colors from table
		ccred = red_tbl[fvars[fvcopfirecolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvcopfirecolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvcopfirecolor] & 0x0f];

		copFireOnRed = (((ccred * 255)/1000) * fvars[fvcopfirebright])/1000;		
		copFireOnGreen = (((ccgreen * 255)/1000) * fvars[fvcopfirebright])/1000;	
		copFireOnBlue = (((ccblue * 255)/1000) * fvars[fvcopfirebright])/1000;	
	}
	else
	{	// Adjust individual custom colors
		copFireOnRed = (((fvars[fvcopfirered] * 255)/1000) * fvars[fvcopfirebright])/1000;		
		copFireOnGreen = (((fvars[fvcopfiregreen] * 255)/1000) * fvars[fvcopfirebright])/1000;	
		copFireOnBlue = (((fvars[fvcopfireblue] * 255)/1000) * fvars[fvcopfirebright])/1000;	
	}

	// Set color for button on Medical
	if (fvars[fvcopmedcolor] != 0)
	{	 // Use standard colors from table
		ccred = red_tbl[fvars[fvcopmedcolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvcopmedcolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvcopmedcolor] & 0x0f];

		copMedOnRed = (((ccred * 255)/1000) * fvars[fvcopmedbright])/1000;		
		copMedOnGreen = (((ccgreen * 255)/1000) * fvars[fvcopmedbright])/1000;	
		copMedOnBlue = (((ccblue * 255)/1000) * fvars[fvcopmedbright])/1000;	
	}
	else
	{	// Adjust individual custom colors
		copMedOnRed = (((fvars[fvcopmedred] * 255)/1000) * fvars[fvcopmedbright])/1000;		
		copMedOnGreen = (((fvars[fvcopmedgreen] * 255)/1000) * fvars[fvcopmedbright])/1000;	
		copMedOnBlue = (((fvars[fvcopmedblue] * 255)/1000) * fvars[fvcopmedbright])/1000;	
	}

	// Set color for button on Emergency
	if (fvars[fvcopemcolor] != 0)
	{	 // Use standard colors from table
		ccred = red_tbl[fvars[fvcopemcolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvcopemcolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvcopemcolor] & 0x0f];

		copEmOnRed = (((ccred * 255)/1000) * fvars[fvcopembright])/1000;		
		copEmOnGreen = (((ccgreen * 255)/1000) * fvars[fvcopembright])/1000;	
		copEmOnBlue = (((ccblue * 255)/1000) * fvars[fvcopembright])/1000;	
	}
	else
	{	// Adjust individual custom colors
		copEmOnRed = (((fvars[fvcopemred] * 255)/1000) * fvars[fvcopembright])/1000;		
		copEmOnGreen = (((fvars[fvcopemgreen] * 255)/1000) * fvars[fvcopembright])/1000;	
		copEmOnBlue = (((fvars[fvcopemblue] * 255)/1000) * fvars[fvcopembright])/1000;	
	}

	// Set color for button on OTS
	if (fvars[fvcopotscolor] != 0)
	{	 // Use standard colors from table
		ccred = red_tbl[fvars[fvcopotscolor] & 0x0f];
		ccgreen = green_tbl[fvars[fvcopotscolor] & 0x0f];
		ccblue = blue_tbl[fvars[fvcopotscolor] & 0x0f];

		copOTSOnRed = (((ccred * 255)/1000) * fvars[fvcopotsbright])/1000;		
		copOTSOnGreen = (((ccgreen * 255)/1000) * fvars[fvcopotsbright])/1000;	
		copOTSOnBlue = (((ccblue * 255)/1000) * fvars[fvcopotsbright])/1000;	
	}
	else
	{	// Adjust individual custom colors
		copOTSOnRed = (((fvars[fvcopotsred] * 255)/1000) * fvars[fvcopotsbright])/1000;		
		copOTSOnGreen = (((fvars[fvcopotsgreen] * 255)/1000) * fvars[fvcopotsbright])/1000;	
		copOTSOnBlue = (((fvars[fvcopotsblue] * 255)/1000) * fvars[fvcopotsbright])/1000;	
	}


	if ((ccOnRed != ccOnRed_prev) || (ccOnGreen != ccOnGreen_prev) || (ccOnBlue != ccOnBlue_prev) ||
	   (ccOffRed != ccOffRed_prev) || (ccOffGreen != ccOffGreen_prev) || (ccOffBlue != ccOffBlue_prev) ||
	   (ccSecOnRed != ccSecOnRed_prev) || (ccSecOnGreen != ccSecOnGreen_prev) || (ccSecOnBlue != ccSecOnBlue_prev) ||
	   (ccAttUpOnRed != ccAttUpOnRed_prev) || (ccAttUpOnGreen != ccAttUpOnGreen_prev) || (ccAttUpOnBlue != ccAttUpOnBlue_prev) ||
	   (ccAttDnOnRed != ccAttDnOnRed_prev) || (ccAttDnOnGreen != ccAttDnOnGreen_prev) || (ccAttDnOnBlue != ccAttDnOnBlue_prev) ||
	   (copFireOnRed != copFireOnRed_prev) || (copFireOnGreen != copFireOnGreen_prev) || (copFireOnBlue != copFireOnBlue_prev) ||
	   (copMedOnRed != copMedOnRed_prev) || (copMedOnGreen != copMedOnGreen_prev) || (copMedOnBlue != copMedOnBlue_prev) ||
	   (copEmOnRed != copEmOnRed_prev) || (copEmOnGreen != copEmOnGreen_prev) || (copEmOnBlue != copEmOnBlue_prev) ||
	   (copOTSOnRed != copOTSOnRed_prev) || (copOTSOnGreen != copOTSOnGreen_prev) || (copOTSOnBlue != copOTSOnBlue_prev) ||
	   (ccOnBright != ccOnBright_prev) || (ccOffBright != ccOffBright_prev))
	{
#if (GALaxy_4 == 1)
		ctcan_dev_req = c_ctcan_GLOBAL;
		ctcan_msg_req = 43;
		ctcan_msg_cmd = 1;
		if (pwrup == 1)
			ctcan_msg_cnt = 3;
		else
			ctcan_msg_cnt = 1;
#else
		can_dev_req = c_can_GLOBAL;
		can_msg_req = 43;
		can_msg_cmd = 1;
		if (pwrup == 1)
			can_msg_cnt = 3;
		else
			can_msg_cnt = 1;
#endif
	}

	ccOnBright_prev = ccOnBright; 		   // leave brightness for backward compatibility
	ccOffBright_prev = ccOffBright; 

	ccOnRed_prev = ccOnRed;
	ccOnGreen_prev = ccOnGreen;
	ccOnBlue_prev = ccOnBlue;

	ccOffRed_prev = ccOffRed;
	ccOffGreen_prev = ccOffGreen;
	ccOffBlue_prev = ccOffBlue;

	ccSecOnRed_prev = ccSecOnRed;
	ccSecOnGreen_prev = ccSecOnGreen;
	ccSecOnBlue_prev = ccSecOnBlue;
	
	ccAttUpOnRed_prev = ccAttUpOnRed;
	ccAttUpOnGreen_prev = ccAttUpOnGreen;
	ccAttUpOnBlue_prev = ccAttUpOnBlue;

	ccAttDnOnRed_prev = ccAttDnOnRed;
	ccAttDnOnGreen_prev = ccAttDnOnGreen;
	ccAttDnOnBlue_prev = ccAttDnOnBlue;

	copFireOnRed_prev = copFireOnRed;
	copFireOnGreen_prev = copFireOnGreen;
	copFireOnBlue_prev = copFireOnBlue;

	copMedOnRed_prev = copMedOnRed;
	copMedOnGreen_prev = copMedOnGreen;
	copMedOnBlue_prev = copMedOnBlue;

	copEmOnRed_prev = copEmOnRed;
	copEmOnGreen_prev = copEmOnGreen;
	copEmOnBlue_prev = copEmOnBlue;

	copOTSOnRed_prev = copOTSOnRed;
	copOTSOnGreen_prev = copOTSOnGreen;
	copOTSOnBlue_prev = copOTSOnBlue;

	// Calculate checksum for car 
	CC_ColorOnOffChecksum = 0;
	CC_ColorOnOffChecksum += ccOnBright;
	CC_ColorOnOffChecksum += ccOffBright;
	CC_ColorOnOffChecksum += ccOnRed;
	CC_ColorOnOffChecksum += ccOnGreen;
	CC_ColorOnOffChecksum += ccOnBlue;
	CC_ColorOnOffChecksum += ccOffRed;
	CC_ColorOnOffChecksum += ccOffGreen;
	CC_ColorOnOffChecksum += ccOffBlue;
	CC_ColorOnOffChecksum += ccSecOnRed;
	CC_ColorOnOffChecksum += ccSecOnGreen;
	CC_ColorOnOffChecksum += ccSecOnBlue;
	CC_ColorOnOffChecksum += ccAttUpOnRed;
	CC_ColorOnOffChecksum += ccAttUpOnGreen;
	CC_ColorOnOffChecksum += ccAttUpOnBlue;
	CC_ColorOnOffChecksum += ccAttDnOnRed;
	CC_ColorOnOffChecksum += ccAttDnOnGreen;
	CC_ColorOnOffChecksum += ccAttDnOnBlue;
	CC_ColorOnOffChecksum += copFireOnRed;
	CC_ColorOnOffChecksum += copFireOnGreen;
	CC_ColorOnOffChecksum += copFireOnBlue;
	CC_ColorOnOffChecksum += copMedOnRed;
	CC_ColorOnOffChecksum += copMedOnGreen;
	CC_ColorOnOffChecksum += copMedOnBlue;
	CC_ColorOnOffChecksum += copEmOnRed;
	CC_ColorOnOffChecksum += copEmOnGreen;
	CC_ColorOnOffChecksum += copEmOnBlue;
	CC_ColorOnOffChecksum += copOTSOnRed;
	CC_ColorOnOffChecksum += copOTSOnGreen;
	CC_ColorOnOffChecksum += copOTSOnBlue;

	calc_can_devices_color_ix_chksum();
				 
}

//**************************************************
// **** Calculate group field variable checksum ****
//**************************************************

void calculate_grp_fvars_chksum(void)
{
	int16 ix;
	int16 last_par;
	uint16 crc;
	int16 i;
	int16 j;

 //	void* vptr;

	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union

	for (i=0;i<=4;i++)
	{		// do all five tables
		ix = i + Group_Dispatch_Menu;
		last_par = (int16)(last_var[ix] + 1);
		crc = 0;
		for (j=0;j<last_par;j++)
		{
			uwu.w = fvars[var_menu_tbl[ix][j]];
	    	crc  = (uint16)((uint8)(crc >> 8) | (uint16)(crc << 8));
	    	crc ^= uwu.b[lbw];
	    	crc ^= (uint8)(crc & 0xff) >> 4;
	    	crc ^= (crc << 8) << 4;
	    	crc ^= ((crc & 0xff) << 4) << 1;

	    	crc  = (uint16)((uint8)(crc >> 8) | (uint16)(crc << 8));
	    	crc ^= uwu.b[hbw];
	    	crc ^= (uint8)(crc & 0xff) >> 4;
	    	crc ^= (crc << 8) << 4;
	    	crc ^= ((crc & 0xff) << 4) << 1;
		}
//		crc &= 0xffff;
		grp_fvars_chksum[ix] = crc;
	}	
}


//****************************************************
// **** Calculate Service Timer variable checksum ****
//****************************************************

void calculate_srv_tmr_chksum(int16 tmrnmb)
{
	void* vptr;
	if (tmrnmb < nmbsvct)
	{
		vptr = &svc_t[tmrnmb];
		svc_tmr_chksum[tmrnmb] = crc_16((unsigned char *)vptr,sizeof(svc_timer_size));
	}
}


//**************************
// Ascii to hex conversion
//**************************


int32 atoh(char s[])
{
	int32 i,n,x;
	bool x_instead_of_h = 0;
	i = 0;
	x = 0;
	n = 0;
	
	while ((s[i] != 0) && (i<8))
	{
		x = 0;
		if ((s[i] >= 'A') && (s[i] <= 'F'))
			x = (s[i] - 'A') + 10;
		else if ((s[i] >= 'a') && (s[i] <= 'f'))
			x = (s[i] - 'a') + 10;
		else if ((s[i] >= '0') && (s[i] <= '9'))
			x = s[i] - '0';
		else if (s[i] == ' ')
		{
			if (n == 0)
				x = 0;
			else
				return n;
		} 
		else if (((s[i] == 'x') || (s[i] == 'X')) && (n == 0))
			x_instead_of_h = 1;			 // leading x instead of followed by h
		else if ((s[i] == 'h') || (s[i] == 'H') || ((x_instead_of_h == 1) && (s[i] == CR)))
			return n;
		else
			return 0;
		n = (n * 16) + x;
		i++;
	}
	return n;
}

void reverse (char s[])
{
	char c;
  	int16 i, j;
	for (i = 0, j = (int16)(strlen(s) - 1); i < j; i++, j--)
	{
	  	c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

//*****************************
// Integer to Ascii Conversion
//*****************************
void itoa(int32 n, char s[])
{
	int32 i;
	int32 sign;
	
	sign = n;
	if (sign < 0)	// record sign
        n = -n;     // make n positive
    i = 0;
    do {    // generate digits in reverse order
        s[i++] = (char)(n % 10 + '0');  // get next digit
    } while ((n /= 10) > 0);    // delete it
    
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
        


//**************************
// BCD to hex conversion
//**************************

int16 bcd_to_hex(uint16 value)
{
	return (int16)((((value >> 12) & 0x0f) * 1000) + (((value >> 8) & 0x0f) * 100) + (((value >> 4) & 0x0f) * 10) + (value & 0x0f));
}

//***********************
// Hex to BCD conversion
//***********************

uint16 hex_to_bcd (uint16 value)
{
	int16 hunds,tens;
	hunds = (int16)(value % 1000);
	tens = (int16)(value % 100);
	return (uint16)( ((value/1000) << 12) + ((hunds/100) << 8) + ((tens/10) << 4) + (tens % 10));
}


//********************************
// Find short int match in array
//********************************

int16 findmatch (int16 value, int16 s[], int16 len)
{
	int16 i=0;
	while (i<len)
	{
		if (value == s[i])
			return i;
		i++;
	}
	return 0;
}

// ********************************************
// This is the clear hall call data	routine
// ********************************************
void clear_call_data(void)
{
	int16 i,j,k,m;

	local_getdate();
  	day_of_week = d.dayofweek;
	if ((day_of_week > 6) || (day_of_week < 0))
		day_of_week = 0;

	Write_MRAM_Command(MRWREN,0);

	// Verify Call data is valid if not zero the data
	for (k=0;k<=6;k++)
	{
		m = MRAM_Day_Dat_Offset[k];
		wr_mram(MRAM_Day_Dat_Page[k],m,0x0076);
		m+=2;
	    for(i=0; i<=96; i++)
	    {
			for(j=0; j<=fl_size; j++)
			{
				nmb_up_call[i][j] = 0;
				nmb_dn_call[i][j] = 0;
			}
			wr_mram_array(MRAM_Day_Dat_Page[k],m,nmb_up_call[i],fl_size);
			m += (int16)(fl_size * 2);
			wr_mram_array(MRAM_Day_Dat_Page[k],m,nmb_dn_call[i],fl_size);
			m += (int16)(fl_size * 2);
  	    }

//		wr_mram(MRAM_Day_Dat_Page[k],m,0x0076);
//		m+=2;
//	    for(i=0; i<=96; i++)
//  	{
//			  for(j=0; j<=fl_size; j++)
//			  {
//					wr_mram(MRAM_Day_Dat_Page[k],m,0x0000);
//					m+=2;
//					wr_mram(MRAM_Day_Dat_Page[k],m,0x0000);
//					m+=2;
//			  }
//	    }
	}
	Write_MRAM_Command(MRWRDI,0);
}

//***********************************************
//********    Clear Job Statistics    ***********
//***********************************************

void clear_job_statistics (void)
{
	int16 i,k;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		

	for(i=0; i<nmblvar; i++)
		 lvars[i] = 0;

	Write_MRAM_Command(MRWREN,0);
	k = MRAM_Stats_Dat_Offset;
	wr_mram(MRAM_Stats_Dat_Page,k,0x0079);
	k+=2;
	wr_mram_array(MRAM_Stats_Dat_Page,k,(int16 *)lvars,(nmblvar * 2));
	Write_MRAM_Command(MRWRDI,0);
}


// ********************************************
// This is the initalize hall call data
// ********************************************
void init_call_data(void)
{
	int16 i,j,k,m;


	local_getdate();
  	day_of_week = d.dayofweek;
	if ((day_of_week > 6) || (day_of_week < 0))
		day_of_week = 0;

	Write_MRAM_Command(MRWREN,0);
	// Verify Call data is valid if not zero the data
	for (k=0;k<=6;k++)
	{
		m = MRAM_Day_Dat_Offset[k];
	  	if(rd_mram(MRAM_Day_Dat_Page[k],m) != 0x0076)
	  	{
			wr_mram(MRAM_Day_Dat_Page[k],m,0x0076);
			m+=2;
	  	    for(i=0; i<=96; i++)
	  	    {
				for(j=0; j<=fl_size; j++)
				{
					nmb_up_call[i][j] = 0;
					nmb_dn_call[i][j] = 0;
				}
				wr_mram_array(MRAM_Day_Dat_Page[k],m,nmb_up_call[i],fl_size);
				m += (int16)(fl_size * 2);
				wr_mram_array(MRAM_Day_Dat_Page[k],m,nmb_dn_call[i],fl_size);
				m += (int16)(fl_size * 2);
	  	    }
		}
	}

	Write_MRAM_Command(MRWRDI,0);
	petdog();

	// Initialize today's call data
	m = (int16)(MRAM_Day_Dat_Offset[day_of_week]+2);
  	for(i=0; i<=96; i++)
  	{
		rd_mram_array(MRAM_Day_Dat_Page[day_of_week],m,nmb_up_call[i],fl_size);
		m += (int16)(fl_size * 2);
		rd_mram_array(MRAM_Day_Dat_Page[day_of_week],m,nmb_dn_call[i],fl_size);
		m += (int16)(fl_size * 2);
  	}
  	return;
}

//***************************************************
// Initialize Flight times
//***************************************************

void init_flight_times (int16 fl_run_time)
{
	int16 i,j,k;

	if ((fl_run_time >= 3) && (fl_run_time <= 10))
		run_calc = (fl_run_time - 2) * cons[speed];
	else
		run_calc = 720;	 
	
   	for(i=1; i<=cons[grtopf]; i++)
   	{
		for(j=1; j<=cons[grtopf]; j++)
   		{
   		    if(i == j)
   		    {
				flight_times[i][j] = 0;
   		    }
   		    else
   		    {
   		    	k = (int16)abs((j-i));
   				flight_times[i][j] = ((run_calc*k)/cons[speed])+2;
   		    }
  		}
   	}
	Write_MRAM_Command(MRWREN,0);
	k = MRAM_Flight_Dat_Offset;
	wr_mram(MRAM_Flight_Dat_Page,k,0x0047);
	k+=2;
	wr_mram_array(MRAM_Flight_Dat_Page,k,(int16 *)flight_times,(fl_size * fl_size));
	Write_MRAM_Command(MRWRDI,0);
}

//***************************************************
//     Initialize Hoistway Digital Pulse Tables
//***************************************************
void Init_Hoistway(void)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
	float utdt_dist = 0.0;
  	int16 i,k;
  	int32 b;
	int16 init_hw_fault = 0;

	// Always use tract_hr for new cpu

	k = MRAM_Hoist_Dat_Offset;
  	if(rd_mram(MRAM_Hoist_Dat_Page,k) != 0x00D7)
  	{		// Hoistway data not valid so initialize it
		Write_MRAM_Command(MRWREN,0);
  		wr_mram(MRAM_Hoist_Dat_Page,k,0x00D7);
		k+=2;
		init_hw_fault = f_hw_init;

  	  	b = ((int32)(Hoistway_Init_Count + (2.0 * Pulses_per_Inch)));
  	  	for(i = 1; i <= cons[topf]; i++)
  	  	{
			if (i < cons[bottomf])
				b = (uint32)1000.0;
			else if	(i == cons[bottomf])
			    b = ((int32)(Hoistway_Init_Count + (2.0 * Pulses_per_Inch)));
			else if ((cons[shortfl] != 0) && (i == (cons[shortfl] + 1)))
			{
				if (i == cons[vshort_tfl])
				{
					if ((fvars[fvshrtflctl] & 0x01) != 0)
						b += ((int32)(10.5 * Pulses_per_Inch));
					else
						b += ((int32)(2.5 * Pulses_per_Inch));
				}
				else if ((fvars[fvshrtflctl] & 0x02) != 0)
					b += ((int32)(50.0 * Pulses_per_Inch));
				else
					b += ((int32)(18.0 * Pulses_per_Inch));
			}
			else if ((cons[shortflmid] != 0) && (i == (cons[shortflmid] + 1)))
			{
				if ((fvars[fvshrtflctl] & 0x04) != 0)
					b += ((int32)(50.0 * Pulses_per_Inch));
				else
					b += ((int32)(18.0 * Pulses_per_Inch));
			}
			else 
			    b += ((int32)(c_floor_height * Pulses_per_Inch));

			uiu.dw = b;
	  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
			k+=2;
	  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
 			k+=2;
 	  	}

  	  	// down normal position -2 inches
  	  	uiu.dw = (Hoistway_Init_Count);
  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
		k+=2;
  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
		k+=2;

		Set_limit_ix();
		
		utdt_dist = (float)limit_dist[limit_ix];

  	  	// Down termianl position 50 inches
  	  	uiu.dw = ((int32)(Hoistway_Init_Count + (utdt_dist * Pulses_per_Inch)));
  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
		k+=2;
  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
		k+=2;

  	  	// Up terminal position 50 inches
  	  	b = ((int32)(((((float)(cons[topf]-cons[bottomf]) * c_floor_height) + 2.0) * Pulses_per_Inch) + Hoistway_Init_Count));
  	  	uiu.dw = ((int32)(b - (utdt_dist * Pulses_per_Inch)));
  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
		k+=2;
  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
		k+=2;

  	  	// Up normal position +2 inches
  	  	uiu.dw = ((int32)(b + (2.0 * Pulses_per_Inch)));
  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
		k+=2;
  		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
		k+=2;

  		wr_mram(MRAM_Hoist_Dat_Page,k,0x0000);	// Learned hoistway flag is zeroed
		k+=2;
	 	Write_MRAM_Command(MRWRDI,0);
 	}

	// Read Hoistway data into ram
 	k = MRAM_Hoist_Dat_Offset+2;	  // Skip initialization flag byte
  	for(i= 1; i<= cons[topf]; i++)             /* initialize floor positions */
  	{
		uiu.w[lw] = rd_mram(MRAM_Hoist_Dat_Page,k);
		k+=2;
		uiu.w[hw] = rd_mram(MRAM_Hoist_Dat_Page,k);
		k+=2;
		DPP_Floor_Pos[i] = uiu.dw;
  	}

	uiu.w[lw] = rd_mram(MRAM_Hoist_Dat_Page,k);
	k+=2;
	uiu.w[hw] = rd_mram(MRAM_Hoist_Dat_Page,k);
	k+=2;
	DPP_DN = uiu.dw;

	uiu.w[lw] = rd_mram(MRAM_Hoist_Dat_Page,k);
	k+=2;
	uiu.w[hw] = rd_mram(MRAM_Hoist_Dat_Page,k);
	k+=2;
	DPP_DT = uiu.dw;

	uiu.w[lw] = rd_mram(MRAM_Hoist_Dat_Page,k);
	k+=2;
	uiu.w[hw] = rd_mram(MRAM_Hoist_Dat_Page,k);
	k+=2;
	DPP_UT = uiu.dw;

	uiu.w[lw] = rd_mram(MRAM_Hoist_Dat_Page,k);
	k+=2;
	uiu.w[hw] = rd_mram(MRAM_Hoist_Dat_Page,k);
	k+=2;
 	DPP_UN = uiu.dw;

 	Hoistway_Learned = rd_mram(MRAM_Hoist_Dat_Page,k);

	k = MRAM_Fl_Update_Adjust_Offset;
  	if(rd_mram(MRAM_Fl_Update_Adjust_Page,k) != 0x00DA)
  	{		// Hoistway update and offset data not valid so initialize it
		Write_MRAM_Command(MRWREN,0);
  		wr_mram(MRAM_Fl_Update_Adjust_Page,k,0x00DA);
		k+=2;
		init_hw_fault = f_hw_upd_init;

		// Up Level Distance floor level 
  		wr_mram(MRAM_Fl_Update_Adjust_Page,k,(int16)(Pulses_per_Inch/8.0));
		k+=2;

		// Down Level Distance floor level 
  		wr_mram(MRAM_Fl_Update_Adjust_Page,k,(int16)(Pulses_per_Inch/8.0));
		k+=2;

		Set_limit_ix();
	  		
  	  	for(i = 1; i <= cons[topf]; i++)
  	  	{
			// floor up slowdown 
	  		wr_mram(MRAM_Fl_Update_Adjust_Page,k,(uint16)((float)limit_dist[limit_ix] * Pulses_per_Inch));
			k+=2;
			// floor down slowdown 
	  		wr_mram(MRAM_Fl_Update_Adjust_Page,k,(uint16)((float)limit_dist[limit_ix] * Pulses_per_Inch));
			k+=2;
			
  	  	}
  	  	
  	  	for(i = 1; i <= cons[topf]; i++)
  	  	{
			// floor up slowdown 
	  		wr_mram(MRAM_Fl_Update_Adjust_Page,k,(uint16)(2.0 * Pulses_per_Inch));
			k+=2;
			// floor down slowdown 
	  		wr_mram(MRAM_Fl_Update_Adjust_Page,k,(uint16)(2.0 * Pulses_per_Inch));
			k+=2;
			
  	  	}
		Write_MRAM_Command(MRWRDI,0);
  	}

 	k = MRAM_Fl_Update_Adjust_Offset+2;	// Skip initialization flag byte
			

	Up_fl_level_dist = rd_mram(MRAM_Fl_Update_Adjust_Page,k);	
	k+=2;
					
	Dn_fl_level_dist = rd_mram(MRAM_Fl_Update_Adjust_Page,k);
	k+=2;
	


	for(i= 1; i<= cons[topf]; i++)             /* initialize floor offsets */
	{
		DPP_Fl_Up_SD[i] = rd_mram(MRAM_Fl_Update_Adjust_Page,k);
		k+=2;
		DPP_Fl_Dn_SD[i] = rd_mram(MRAM_Fl_Update_Adjust_Page,k);
		k+=2;
	}

	for(i= 1; i<= cons[topf]; i++)             /* initialize floor offsets */
	{
		DPP_SFl_Up_SD[i] = rd_mram(MRAM_Fl_Update_Adjust_Page,k);
		k+=2;
		DPP_SFl_Dn_SD[i] = rd_mram(MRAM_Fl_Update_Adjust_Page,k);
		k+=2;
	}


	mid_hoistway = DPP_Floor_Pos[cons[bottomf]] + (DPP_Floor_Pos[cons[topf]] - DPP_Floor_Pos[cons[bottomf]])/2;

	i = verify_hoistway();
	if (i != 0)
	{
		Hoistway_Learned = 0;
		init_hw_fault = i;
	}

	if (init_hw_fault != 0)
		record_fault(init_hw_fault);
}


// ***********************************************
// Read the correct cons file
// ***********************************************
void rdcons(void)
{
	int16 i,j;
  	int32 uc_msk;
  	int32 dc_msk;
	int32 ir_uc_msk;
	int32 ir_dc_msk;
  	int16 fl_ix;
	uint16 sd_error = 0;

  	disable_dog();

	sd_error = sd_initialization();
	// Initialise File IO Library
	if (sd_error == 0)
	{
		fl_init();
		// Attach media access functions to library
		if (fl_attach_media(media_read, media_write) != FAT_INIT_OK)
		{
			print_com1("ERROR: Media attach failed\n");
			sd_error = 0x55; 
		}
	}
	
	// Zero cons table
	for(i=0; i<connmb; i++)
		cons[i] = 0;

	// Clear array especially if no security being used
	car_asgn_sec = 0;
	car_Grp_Stat.c_car_asgn_sec = 0;
	for (i=1; i<fl_size; i++)
	{
		car_asgn_cc[i] = 0;
		car_asgn_uc[i] = 0;
		car_asgn_dc[i] = 0;
 		car_asgn_rcc[i] = 0;
		car_asgn_ruc[i] = 0;
		car_asgn_rdc[i] = 0;
	}

	grbot_rfl = 0;
	i = 0;
   	for(j=0; j<fl_size; j++)
	{
		ccnmsk[i][j] = 0;
		ucnmsk[i][j] = 0;
		dcnmsk[i][j] = 0;
		rccnmsk[i][j] = 0;
		rucnmsk[i][j] = 0;
		rdcnmsk[i][j] = 0;

		ccsmsk[i][j] = 0;
		ucsmsk[i][j] = 0;
		dcsmsk[i][j] = 0;
		rccsmsk[i][j] = 0;
		rucsmsk[i][j] = 0;
		rdcsmsk[i][j] = 0;

		ccs2msk[i][j] = 0;
		ucs2msk[i][j] = 0;
		dcs2msk[i][j] = 0;
		rccs2msk[i][j] = 0;
		rucs2msk[i][j] = 0;
		rdcs2msk[i][j] = 0;

		ccs3msk[i][j] = 0;
		ucs3msk[i][j] = 0;
		dcs3msk[i][j] = 0;
		rccs3msk[i][j] = 0;
		rucs3msk[i][j] = 0;
		rdcs3msk[i][j] = 0;

		ccs4msk[i][j] = 0;
		ucs4msk[i][j] = 0;
		dcs4msk[i][j] = 0;
		rccs4msk[i][j] = 0;
		rucs4msk[i][j] = 0;
		rdcs4msk[i][j] = 0;

		irccmsk[i][j] = 0;
		irucmsk[i][j] = 0;
		irdcmsk[i][j] = 0;
		rirccmsk[i][j] = 0;
		rirucmsk[i][j] = 0;
		rirdcmsk[i][j] = 0;

		sab_ccmsk[j] = 0;
		sab_ucmsk[j] = 0;
		sab_ucmsk[j] = 0;
		sab_rccmsk[j] = 0;
		sab_rucmsk[j] = 0;
		sab_rdcmsk[j] = 0;

		xa_uc_msk[j] = 0;
		xa_dc_msk[j] = 0;
		xpi_pos[j] = (uint8)j;

	}
	ccmsk[0] = &ccnmsk[0][0];
	ucmsk[0] = &ucnmsk[0][0];
	dcmsk[0] = &dcnmsk[0][0];
	rccmsk[0] = &rccnmsk[0][0];
	rucmsk[0] = &rucnmsk[0][0];
	rdcmsk[0] = &rdcnmsk[0][0];

	g_cbmsk[0] = 0;
	g_cbmsk[1] = 0;
	g_rcbmsk[0] = 0;
	g_rcbmsk[1] = 0;

	g_vipmsk[0] = 0;
	g_vipmsk[1] = 0;
	g_rvipmsk[0] = 0;
	g_rvipmsk[1] = 0;

	for (j=1;j<car_size;j++)
	{
		Valid_SR_Car[j] = 0;
 	  	valid_ep_car[j] = 1;
 	  	front_op[j] = 0;
 	  	rear_op[j] = 0;
		cc_mask[j][0] = 0;
		cc_mask[j][1] = 0;
		rcc_mask[j][0] = 0;
		rcc_mask[j][1] = 0;
	}

	if (sd_error == 0)
	{
		if (verifyB_dat() == 0)
			rdconsB_std();		
#if (Tract_HR == 1)
		if (verify_dat() == 4)
			rdcons_hr();			
#else
		if (verify_dat() == 0)
			rdcons_std();
#endif
		else
			rdcons_empty();
	}
	else
		rdcons_empty();

	grtop_rfl = 0;
	for (j=1;j<=cons[nmbcars];j++)
	{
		if (top_rfl[j] > grtop_rfl)
			grtop_rfl = top_rfl[j];
	}

	if (grbot_rfl > grtop_rfl)
		grbot_rfl = grtop_rfl;
	
	if (cons[rear] == 0)
		bot_rfl = 1;
	
	g_cc_mask[0] = 0;
	g_cc_mask[1] = 0;
	for (j=1; j<=cons[grtopf]; j++)
	{
		fl_ix = (j-1)/32;

		uc_msk = 0;
		dc_msk = 0;
		ir_uc_msk = 0;
		ir_dc_msk = 0;
		for (i=1; i<=cons[nmbcars]; i++)
		{
			uc_msk |= ucnmsk[i][j];
			dc_msk |= dcnmsk[i][j];
			if ((cons[SecFlCfg] == 1) || (cons[SecFlCfg] == 5))
			{ 					  // add in security mask only if selected
				uc_msk |= ucsmsk[i][j];
				dc_msk |= dcsmsk[i][j];
			}
			else if (cons[SecFlCfg] == 2)
			{ 					  // add in security mask only if selected
				uc_msk |= ucsmsk[i][j];
				dc_msk |= dcsmsk[i][j];
				if (cons[SecFlCfg_2] == 1)
				{ 					  // add in security mask only if selected
					uc_msk |= ucs2msk[i][j];
					dc_msk |= dcs2msk[i][j];
				}
				if (cons[SecFlCfg_3] == 1)
				{ 					  // add in security mask only if selected
					uc_msk |= ucs3msk[i][j];
					dc_msk |= dcs3msk[i][j];
				}
				if (cons[SecFlCfg_4] == 1)
				{ 					  // add in security mask only if selected
					uc_msk |= ucs4msk[i][j];
					dc_msk |= dcs4msk[i][j];
				}
			}
			else if ((cons[SecFlCfg] == 3) || (cons[SecFlCfg] == 7))
			{
				ucs4msk[i][j] = ucnmsk[i][j];
				dcs4msk[i][j] = dcnmsk[i][j];
			}
			else if (cons[SecFlCfg] == 4)
			{
				uc_msk |= ucsmsk[i][j];
				dc_msk |= dcsmsk[i][j];
				if (ucsmsk[i][j] == 0)
					flcfg_uhc_carsec[fl_ix][i] |= fl_mask[j];	// set the security bit
				else
					flcfg_uhc_carsec[fl_ix][i] &= ~fl_mask[j];	// clear the security bit

				if (dcsmsk[i][j] == 0)
					flcfg_dhc_carsec[fl_ix][i] |= fl_mask[j];	// set the security bit
				else
					flcfg_dhc_carsec[fl_ix][i] &= ~fl_mask[j];	// clear the security bit

			}
			ir_uc_msk |= irucmsk[i][j];
			ir_dc_msk |= irdcmsk[i][j];

			if (ccmsk[i][j] == 1)
			{
				cc_mask[i][fl_ix] |= fl_mask[j];
				g_cc_mask[fl_ix] |= fl_mask[j];
			}
		}
		if (uc_msk != 0)
		{
			g_ucmsk[fl_ix] |= fl_mask[j];
			if (cons[incrs] != 0)
				g_irucmsk[fl_ix] |= fl_mask[j];
		}
		if (dc_msk != 0)
		{
			g_dcmsk[fl_ix] |= fl_mask[j];
			if (cons[incrs] != 0)
				g_irdcmsk[fl_ix] |= fl_mask[j];
		}

		if (cons[cblue] != 0)
		{
			if (cons[CB_msk] == 0)
			{
				if ((uc_msk != 0) || (dc_msk != 0))
					g_cbmsk[fl_ix] |= fl_mask[j];
			}
		}
		if (cons[VipSvc] != 0)
		{
			if (cons[VIP_msk] == 0)
			{
				if ((uc_msk != 0) || (dc_msk != 0))
					g_vipmsk[fl_ix] |= fl_mask[j];
			}
		}

		if (cons[Second_Riser] != 0) 
		{
			if (ir_uc_msk != 0)
				g_irucmsk[fl_ix] |= fl_mask[j];
			if (ir_dc_msk != 0)
				g_irdcmsk[fl_ix] |= fl_mask[j];
		}
	}

	g_rcc_mask[0] = 0;
	g_rcc_mask[1] = 0;
	for (j=1; j<=cons[grtopf]; j++)
	{
		fl_ix = (j-1)/32;
		uc_msk = 0;
		dc_msk = 0;
		ir_uc_msk = 0;
		ir_dc_msk = 0;
		for (i=1; i<=cons[nmbcars]; i++)
		{
			uc_msk |= rucnmsk[i][j];
			dc_msk |= rdcnmsk[i][j];
			if ((cons[SecFlCfg] == 1) || (cons[SecFlCfg] == 5))
			{ 					  // add in security mask only if selected
				uc_msk |= rucsmsk[i][j];
				dc_msk |= rdcsmsk[i][j];
			}
			else if (cons[SecFlCfg] == 2)
			{ 					  // add in security mask only if selected
				uc_msk |= rucsmsk[i][j];
				dc_msk |= rdcsmsk[i][j];
				if (cons[SecFlCfg_2] == 1)
				{ 					  // add in security mask only if selected
					uc_msk |= rucs2msk[i][j];
					dc_msk |= rdcs2msk[i][j];
				}
				if (cons[SecFlCfg_3] == 1)
				{ 					  // add in security mask only if selected
					uc_msk |= rucs3msk[i][j];
					dc_msk |= rdcs3msk[i][j];
				}
				if (cons[SecFlCfg_4] == 1)
				{ 					  // add in security mask only if selected
					uc_msk |= rucs4msk[i][j];
					dc_msk |= rdcs4msk[i][j];
				}
			}
			else if ((cons[SecFlCfg] == 3) || (cons[SecFlCfg] == 7))
			{
				rucs4msk[i][j] = rucnmsk[i][j];
				rdcs4msk[i][j] = rdcnmsk[i][j];
			}
			else if (cons[SecFlCfg] == 4)
			{
				uc_msk |= rucsmsk[i][j];
				dc_msk |= rdcsmsk[i][j];
				if (rucsmsk[i][j] == 0)
					flcfg_ruhc_carsec[fl_ix][i] |= fl_mask[j];	// set the security bit
				else
					flcfg_ruhc_carsec[fl_ix][i] &= ~fl_mask[j];	// clear the security bit

				if (rdcsmsk[i][j] == 0)
					flcfg_rdhc_carsec[fl_ix][i] |= fl_mask[j];	// set the security bit
				else
					flcfg_rdhc_carsec[fl_ix][i] &= ~fl_mask[j];	// clear the security bit

			}
			ir_uc_msk |= rirucmsk[i][j];
			ir_dc_msk |= rirdcmsk[i][j];
			if (rccmsk[i][j] == 1)
			{
				rcc_mask[i][fl_ix] |= fl_mask[j];
				g_rcc_mask[fl_ix] |= fl_mask[j];
			}
		}
		if (uc_msk != 0)
		{
			g_rucmsk[fl_ix] |= fl_mask[j];
			if (cons[incrs] != 0)
				g_rirucmsk[fl_ix] |= fl_mask[j];
		}
		if (dc_msk != 0)
		{
			g_rdcmsk[fl_ix] |= fl_mask[j];
			if (cons[incrs] != 0)
				g_rirdcmsk[fl_ix] |= fl_mask[j];
		}
		if (cons[cblue] != 0)
		{
			if (cons[CB_msk] == 0)
			{
				if ((uc_msk != 0) || (dc_msk != 0))
					g_rcbmsk[fl_ix] |= fl_mask[j];
			}
		}
		else if (cons[VipSvc] != 0)
		{				// set the rear vip mask
			if (cons[VIP_msk] == 0)
			{
				if ((uc_msk != 0) || (dc_msk != 0))
					g_rvipmsk[fl_ix] |= fl_mask[j];
			}
		}

		if (cons[Second_Riser] != 0) 
		{
			if (ir_uc_msk != 0)
				g_rirucmsk[fl_ix] |= fl_mask[j];
			if (ir_dc_msk != 0)
				g_rirdcmsk[fl_ix] |= fl_mask[j];
		}
	}

	if (cons[LbyUpReq] != 0)
	{
		if ((cons[sercom] & 0x20) == 0)
		{				 // if serial hc boards then mask is set in packet 17 in comm.cc
			fl_ix = cons[grtopf]/32;
			if (cons[epgrp] != 0)
				g_ucmsk[fl_ix] |= fl_mask[cons[grtopf]+1];
			else
				g_ucmsk[fl_ix] |= fl_mask[cons[grtopf]];
		}
	}

	front_vip = 0;
	rear_vip = 0;
	if (cons[VipSvc] != 0)
	{
		if ((g_vipmsk[0] != 0) || (g_vipmsk[1] != 0))
			front_vip = 1;
		if ((g_rvipmsk[0] != 0) || (g_rvipmsk[1] != 0))
			rear_vip = 1;
	}
	
	if (cons[Third_Riser] != 0)
	{
		// enable the mask so that the calls can be made on the board
	
		for (i=0;i<=cons[nmbcars];i++)
		{
			for (j=1; j<=cons[grtopf]; j++)
			{	// set TR2 valid up/dn call mask same as normal up/down mask
				fl_ix = (j-1)/32;
				if (trucmsk[i][j] == 1)
					g_trucmsk[fl_ix] |= fl_mask[j];
				if (trdcmsk[i][j] == 1)
					g_trdcmsk[fl_ix] |= fl_mask[j];
			}
		}
	}
	
	
	if ((Security_Type[cons[carnmb]] & 0x01) != 0)
		Car_Call_Lockouts = 1;
	else if ((Security_Type[cons[carnmb]] & 0x10) != 0)
		Lockout_Override = 1;

	if ((Security_Type[cons[carnmb]] & 0x02) != 0)
		Grp_CC_Lockouts = 1;
	if ((Security_Type[cons[carnmb]] & 0x08) != 0)
		Grp_FL_Lockouts = 1;
	if ((Security_Type[cons[carnmb]] & 0x20) != 0)
		Grp_HC_Lockouts = 1;
	if ((Security_Type[cons[carnmb]] & 0x80) != 0)
		Grp_CC_Override = 1;

	if ((cons[sercom] & 0x04) == 0)		// no 1040 board
		cons[F1_CT] = 0;				// Option only valid if 1040 top of car board used

#if (Traction == 1)
	ETS_Dist = 10.0;
	if (cons[speed] > 500)
		ETS_Dist = 14.0;
	if (cons[speed] > 700)
		ETS_Dist = 18.0;
	if (cons[speed] > 900)
		ETS_Dist = 24.0;
	if (cons[speed] > 1100)
		ETS_Dist = 32.0;
	if (cons[speed] > 1300)
		ETS_Dist = 42.0;

#endif
	enable_dog();
}


#if (Tract_HR == 1)
// *******************************************************
// This is read in constants that define the high rise job
// *******************************************************
void rdcons_hr()
{
  	int16 i,j,k;
  	int16 fl_ix;

	FL_FILE *fp1;

  	disable_dog();

   	fp1 = fl_fopen("/conshr.dat","r");
	if (fp1 == NULL)
	   	fp1 = fl_fopen("/CONSHR.DAT","r");

    for(i=0; i<31; i++)             // reading job constants
    {			// 31 max to keep cons file the same
      	fl_fgets(datbuf,180,fp1);
      	cons[i] = (int16)atoi(datbuf);
    }

	if (cons[speed] == 0)
		cons[speed] = 100;
	
	if ((cons[frghtd] == 10) || (cons[frghtd] == 12))
		eef2_enabled = 1;

	if ((cons[rear] != 0) &&  ((cons[frghtd] == 11) || (cons[frghtd] == 12)))
		eer2_enabled = 1;
	


	// Read Drive Type for each car
    for(i=1; i<car_size; i++)             // reading job constants
    {
      	fl_fgets(datbuf,180,fp1);
      	Drive_Type[i] = (int8)atoi(datbuf);
    }

	bot_rfl = cons[topf];
    for(i=1; i<=8; i++)
    {
      	fl_fgets(datbuf,180,fp1);             // reading video position
		j = 0;
		k = 0;
		car_bld_no[i][0] = ' ';
		car_bld_no[i][1] = ' ';
		while ((j <= 2) && (k <= 1))
		{
			if ((datbuf[j] >= '0') && (datbuf[j] <= 'z'))  // does not include "/" (0x2f) or ' ' (0x20)
			{
				car_bld_no[i][k] = datbuf[j];
				k++;
			}
			j++;
		}
      	fl_fgets(datbuf,180,fp1);             // reading car call mask
      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
	  	front_op[i] = 0;
	  	rear_op[i] = 0;
	  	top_fl[i] = 0;
      	top_rfl[i] = 0;
      	for(j=1; j<fl_size; j++)
      	{		//     ^ Changed from 31 to 32 and back to 31
			if(datbuf[j] == '1')    // seting car call masks
			{
			  	ccnmsk[i][j] = 1;
			  	cons_init = 1;
			}
			else
			  	ccnmsk[i][j] = 0;
			if(datbuf1[j] == '1')   // seting up call masks
			  	ucnmsk[i][j] = 1;
			else
			  	ucnmsk[i][j] = 0;
			if(datbuf2[j] == '1')   // seting dn call masks
			  	dcnmsk[i][j] = 1;
			else
			  	dcnmsk[i][j] = 0;
			if(datbuf3[j] == '1')   // seting rear car call masks
			  	rccnmsk[i][j] = 1;
			else
			  	rccnmsk[i][j] = 0;
			if(datbuf4[j] == '1')   // setting rear up call masks
			  	rucnmsk[i][j] = 1;
			else
			  	rucnmsk[i][j] = 0;
			if(datbuf5[j] == '1')   // setting rear dn call masks
			  	rdcnmsk[i][j] = 1;
			else
			  	rdcnmsk[i][j] = 0;
			if (i == cons[carnmb])
				valid_fl[j] = 0;
			
			if ((ccnmsk[i][j] == 1) || (ucnmsk[i][j] == 1) || (dcnmsk[i][j] == 1))
			{
				if (i == cons[carnmb])
					valid_fl[j] = 1;
				front_op[i] = 1;	 // set if the car has front doors
				top_fl[i] = j;		 // set the highest top floor for each car 
			}
			if ((rccnmsk[i][j] == 1) || (rucnmsk[i][j] == 1) || (rdcnmsk[i][j] == 1))
			{
				if (i == cons[carnmb])
				{
					valid_fl[j] = 1;
					if (j < bot_rfl)
						bot_rfl = j;
				}
				if ((j < grbot_rfl) || (grbot_rfl == 0))
					grbot_rfl = j;
				
				rear_op[i] = 1;		// set if the car has rear doors
				top_rfl[i] = j;		// set the highest rear top floor for each car
			}
      	}
		ccmsk[i] = &ccnmsk[i][0];
		ucmsk[i] = &ucnmsk[i][0];
		dcmsk[i] = &dcnmsk[i][0];
		rccmsk[i] = &rccnmsk[i][0];
		rucmsk[i] = &rucnmsk[i][0];
		rdcmsk[i] = &rdcnmsk[i][0];
    }
	
    fl_fgets(datbuf,180,fp1);       // reading in the floor markings
    for(i=1; i<=60; i++)
    {	  //     ^ Changed from 31 to 60
      	fl_fgets(datbuf,180,fp1);
      	flmrk[i][0] = datbuf[0];
      	flmrk[i][1] = datbuf[1];
    }
    fl_fgets(datbuf,180,fp1);       //reading in the job name
    j=0;
    while((datbuf[j] != '/') && (j < 25))
    {
      	jobnm[j] = datbuf[j];
      	j++;
    }
    jobnm[j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading address line number 1
    while((datbuf[j] != '/') && (j < 25))
    {
      	add1[j] = datbuf[j];
      	j++;
    }
    add1[j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading address line number 2
    while((datbuf[j] != '/') && (j < 25))
    {
      	add2[j] = datbuf[j];
      	j++;
    }
    add2[j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading company name
    while((datbuf[j] != '/') && (j < 25))
    {
      	cmp[j] = datbuf[j];
      	j++;
    }
    cmp[j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading IVT password
    while((datbuf[j] != '/') && (j < 25))
    {
      	pass[0][j] = datbuf[j];
      	j++;
    }
    pass[0][j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading IVT user name
    while((datbuf[j] != '/') && (j < 25))
    {
      	user[0][j] = datbuf[j];
      	j++;
    }
    user[0][j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading company pass word
    while((datbuf[j] != '/') && (j < 25))
    {
      	pass[1][j] = datbuf[j];
      	j++;
    }
    pass[1][j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading company user name
    while((datbuf[j] != '/') && (j < 25))
    {
      	user[1][j] = datbuf[j];
      	j++;
    }
    user[1][j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading video message line #1
    while((datbuf[j] != '/') && (j < 25))
    {
      	vidmes[0][j] = datbuf[j];
      	j++;
    }
    vidmes[0][j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading video message line #2
    while((datbuf[j] != '/') && (j < 25))
    {
      	vidmes[1][j] = datbuf[j];
      	j++;
    }
    vidmes[1][j] = 13;
    j=0;
    fl_fgets(datbuf,180,fp1);       //reading video message line #3
    while((datbuf[j] != '/') && (j < 25))
    {
      	vidmes[2][j] = datbuf[j];
      	j++;
    }
    vidmes[2][j] = 13;

	if (fl_fgets(datbuf,180,fp1) == 0)		// look for file continuation
		goto Exit_Gracefully;
	if (strstr(datbuf,"Continue-S1") != 0)
	{
		// Read Security Type for each car
	    for(i=1; i<=8; i++)             // reading job constants
	    {
	      	fl_fgets(datbuf,180,fp1);
	      	Security_Type[i] = (int16)atoi(datbuf);
	    }

		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;

	} 	 // else security defaults to zero

	if (strstr(datbuf,"Continue-S2") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[SecFlCfg] = (int16)atoi(datbuf);

		// Read Security mask for each car
	    for(i=1; i<=8; i++)
	    {
	      	fl_fgets(datbuf,180,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,180,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	ccsmsk[i][j] = 1;
				else
				  	ccsmsk[i][j] = 0;
				if(datbuf1[j] == '1')   // seting up call masks
				  	ucsmsk[i][j] = 1;
				else
				  	ucsmsk[i][j] = 0;
				if(datbuf2[j] == '1')   // seting dn call masks
				  	dcsmsk[i][j] = 1;
				else
				  	dcsmsk[i][j] = 0;
				if(datbuf3[j] == '1')   // seting rear car call masks
				  	rccsmsk[i][j] = 1;
				else
				  	rccsmsk[i][j] = 0;
				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rucsmsk[i][j] = 1;
				else
				  	rucsmsk[i][j] = 0;
				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rdcsmsk[i][j] = 1;
				else
				  	rdcsmsk[i][j] = 0;
	      	}
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C2") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);			// ascii date code is yyyymmdd
      	cons_date_code = atol(datbuf);		// converts to a long integer such as 20031204
	    for(i=32; i<=49; i++)             // reading ADDED job constants
	    {			// 32 to 50 max to keep cons file the same
	      	fl_fgets(datbuf,180,fp1);
	      	cons[i] = (int16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C3") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[StagRear] = (int16)atoi(datbuf);		// get staggard rear option flag
      	fl_fgets(datbuf,180,fp1);			// get staggard rear floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set staggard rear floor masks
			  	stag_rmsk[j] = 1;
			else
			  	stag_rmsk[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C4") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[LbyUpReq] = (int16)atoi(datbuf);		// get Lobby Up Request option flag
      	fl_fgets(datbuf,180,fp1);			// get mask for cars that service this call
	    for(j=1; j<=8; j++)
	    {	
			if(datbuf[j] == '1')    // set lobby up request floor masks
			  	lby_up_req_car[j] = 1;
			else if (datbuf[j] == '3')
			  	lby_up_req_car[j] = 3;
			else if (datbuf[j] == '5')
			  	lby_up_req_car[j] = 5;
			else
			  	lby_up_req_car[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C5") != 0)	// look for next file continuation
	{
	    for(i=52; i<=72; i++)             // reading ADDED job constants
	    {			// 52 to 72 max to keep cons file the same
	      	fl_fgets(datbuf,180,fp1);
	      	cons[i] = (int16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C6") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[heavy_door] = (int16)atoi(datbuf);		// get heavy door option flag
      	fl_fgets(datbuf,180,fp1);			// get heavy door front floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	heavy_door_fmsk[j] = 1;
			else
			  	heavy_door_fmsk[j] = 0;
		}
      	fl_fgets(datbuf,180,fp1);			// get heavy door rear floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set rear floor masks
			  	heavy_door_rmsk[j] = 1;
			else
			  	heavy_door_rmsk[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C7") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[stag_auto_door] = (int16)atoi(datbuf);		// get heavy door option flag
      	fl_fgets(datbuf,180,fp1);			// get staggerd auto/swing door front floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	stag_ado_msk[j] = 1;
			else
			  	stag_ado_msk[j] = 0;
		}
      	fl_fgets(datbuf,180,fp1);			// get staggerd auto/swing door rear floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set rear floor masks
			  	stag_ado_rmsk[j] = 1;
			else
			  	stag_ado_rmsk[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-CB") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[CBCar] = (int16)atoi(datbuf);		// get code blue car option flag
      	fl_fgets(datbuf,180,fp1);			// get code blue car mask
	    for(j=1; j<=8; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	valid_cb_car[j] = 1;
			else
			  	valid_cb_car[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-S3") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[Car_Asgn_Fl] = (int16)atoi(datbuf);		// get security option flag
      	fl_fgets(datbuf,180,fp1);             // reading car call mask
      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
	    for(j=1; j<fl_size; j++)
	    {	
			if ((datbuf[j] >= '0') && (datbuf[j] <= '8'))
				car_asgn_cc[j] = (int16)(datbuf[j] - '0');	  // set front floor car
			else
			  	car_asgn_cc[j] = 0;

			if ((datbuf1[j] >= '0') && (datbuf1[j] <= '8'))
				car_asgn_uc[j] = (int16)(datbuf1[j] - '0');	  // set front floor car
			else
			  	car_asgn_uc[j] = 0;

			if ((datbuf2[j] >= '0') && (datbuf2[j] <= '8'))
				car_asgn_dc[j] = (int16)(datbuf2[j] - '0');	  // set front floor car
			else
			  	car_asgn_dc[j] = 0;

			if ((datbuf3[j] >= '0') && (datbuf3[j] <= '8'))
				car_asgn_rcc[j] = (int16)(datbuf3[j] - '0');	  // set rear floor car
			else
			  	car_asgn_rcc[j] = 0;

			if ((datbuf4[j] >= '0') && (datbuf4[j] <= '8'))
				car_asgn_ruc[j] = (int16)(datbuf4[j] - '0');	  // set rear floor car
			else
			  	car_asgn_ruc[j] = 0;

			if ((datbuf5[j] >= '0') && (datbuf5[j] <= '8'))
				car_asgn_rdc[j] = (int16)(datbuf5[j] - '0');	  // set rear floor car
			else
			  	car_asgn_rdc[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
	if (strstr(datbuf,"Continue-EP") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[EPCarRun] = (int16)atoi(datbuf);		// get Emergency Power car option flag
      	fl_fgets(datbuf,180,fp1);			// get Emergency Power car mask
		if (cons[EPCarRun] == 1)
		{
		    for(j=1; j<=8; j++)
		    {	
				if(datbuf[j] == '1')    // set front floor masks
				  	valid_ep_car[j] = 1;
				else
				  	valid_ep_car[j] = 0;
			}
		}
		else
		{
		    for(j=1; j<=8; j++)
			  	valid_ep_car[j] = 1;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SR") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[Second_Riser] = (int16)atoi(datbuf);

		// Read Second Riser mask for each car
	    for(i=1; i<=8; i++)
	    {
	      	fl_fgets(datbuf,180,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,180,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	irccmsk[i][j] = 1;
				else
				  	irccmsk[i][j] = 0;

				if(datbuf1[j] == '1')   // seting up call masks
				  	irucmsk[i][j] = 1;
				else
				  	irucmsk[i][j] = 0;

				if(datbuf2[j] == '1')   // seting dn call masks
				  	irdcmsk[i][j] = 1;
				else
				  	irdcmsk[i][j] = 0;

				if(datbuf3[j] == '1')    // seting car call masks
				  	rirccmsk[i][j] = 1;
				else
				  	rirccmsk[i][j] = 0;

				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rirucmsk[i][j] = 1;
				else
				  	rirucmsk[i][j] = 0;

				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rirdcmsk[i][j] = 1;
				else
				  	rirdcmsk[i][j] = 0;

				if ((irucmsk[i][j] == 1) || (irdcmsk[i][j] == 1) || (rirucmsk[i][j] == 1) || (rirdcmsk[i][j] == 1))
				{
					if (i == cons[carnmb])
						valid_fl[j] = 1;
					Valid_SR_Car[i] = 1;
				}
				if ((rirucmsk[i][j] == 1) || (rirdcmsk[i][j] == 1))
				{
					if ((j < grbot_rfl) || (grbot_rfl == 0))
						grbot_rfl = j;
					rear_op[i] = 1;		// set if the car has rear doors
					top_rfl[i] = j;		// set the highest rear top floor for each car
				}
	      	}
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-VIP") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[VipSvc] = (int16)atoi(datbuf);		// get vip car option flag
      	fl_fgets(datbuf,180,fp1);			// get vip car mask
	    for(j=1; j<=8; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	valid_vip_car[j] = 1;
			else
			  	valid_vip_car[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C8") != 0)	// look for next file continuation
	{
	    for(i=80; i<=99; i++)             // reading ADDED job constants
	    {			// 80 to 99 max to keep cons file the same
	      	fl_fgets(datbuf,180,fp1);
	      	cons[i] = (int16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SAB") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[SAB_Fl] = (int16)atoi(datbuf);

		// Read Sabbath floor mask
      	fl_fgets(datbuf,180,fp1);             // reading car call mask
      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
      	for(j=1; j<fl_size; j++)
      	{	
			if(datbuf[j] == '1')    // seting car call masks
			{
			  	sab_ccmsk[j] = 1;
				sab_cc_topff = j;
				if(sab_cc_botff == 0)
					sab_cc_botff = j;
			}
			else
			  	sab_ccmsk[j] = 0;
	
			if(datbuf1[j] == '1')   // seting up call masks
			  	sab_ucmsk[j] = 1;
			else
			  	sab_ucmsk[j] = 0;

			if(datbuf2[j] == '1')   // seting dn call masks
			  	sab_dcmsk[j] = 1;
			else
			  	sab_dcmsk[j] = 0;

			if(datbuf3[j] == '1')    // seting car call masks
			{
			  	sab_rccmsk[j] = 1;
				sab_cc_topfr = j;
				if(sab_cc_botfr == 0)
					sab_cc_botfr = j;
			}
			else
			  	sab_rccmsk[j] = 0;

			if(datbuf4[j] == '1')   // setting rear up call masks
			  	sab_rucmsk[j] = 1;
			else
			  	sab_rucmsk[j] = 0;

			if(datbuf5[j] == '1')   // setting rear dn call masks
			  	sab_rdcmsk[j] = 1;
			else
			  	sab_rdcmsk[j] = 0;

	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
	if (strstr(datbuf,"Continue-XA") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[XA_Mask] = (int16)atoi(datbuf);

		// Read cross assignment floor mask
      	fl_fgets(datbuf,180,fp1);            // reading up call mask
      	fl_fgets(datbuf1,180,fp1);            // reading dn call mask
      	for(j=1; j<fl_size; j++)
      	{	
			if(datbuf[j] == '1')    // seting up call masks
			  	xa_uc_msk[j] = 1;
			else if (datbuf[j] == '2')
			  	xa_uc_msk[j] = 2;
			else if (datbuf[j] == '3')
			  	xa_uc_msk[j] = 3;
			else
			  	xa_uc_msk[j] = 0;
	
			if(datbuf1[j] == '1')   // seting down call masks
			  	xa_dc_msk[j] = 1;
			else if (datbuf1[j] == '2')
			  	xa_dc_msk[j] = 2;
			else if (datbuf1[j] == '3')
			  	xa_dc_msk[j] = 3;
			else
			  	xa_dc_msk[j] = 0;
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-API") != 0)	// look for next file continuation
	{

      	fl_fgets(datbuf,180,fp1);
      	cons[Alt_Ascii_pi] = (int16)atoi(datbuf);

	    for(i=1; i<=60; i++)
    	{	  //     ^ Changed from 31 to 60 
      		fl_fgets(datbuf,180,fp1);
	      	alt_flmrk[i][0] = datbuf[0];
    	  	alt_flmrk[i][1] = datbuf[1];
	    }
		
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
		
	if (strstr(datbuf,"Continue-XPI") != 0)	// look for next file continuation
	{

      	fl_fgets(datbuf,180,fp1);
      	cons[Xref_PI_Pos] = (int16)atoi(datbuf);

	    for(i=1; i<=60; i++)
    	{	  //     ^ Changed from 31 to 60 
      		fl_fgets(datbuf,180,fp1);
      		xpi_pos[i] = (int8)atoi(datbuf);
	    }
		
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-DSR") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[Dual_Ser_Riser] = (int16)atoi(datbuf);

		// Read serial riser floor mask
      	fl_fgets(datbuf,180,fp1);            // reading primary front riser mask
      	fl_fgets(datbuf1,180,fp1);            // reading secondary front riser mask
      	fl_fgets(datbuf2,180,fp1);            // reading primary rear riser mask
      	fl_fgets(datbuf3,180,fp1);            // reading secondary rear or third front riser mask
		pfdsr_msk[0] = 0;
		pfdsr_msk[1] = 0;
		sfdsr_msk[0] = 0;
		sfdsr_msk[1] = 0;
		prdsr_msk[0] = 0;
		prdsr_msk[1] = 0;
		srdsr_msk[0] = 0;
		srdsr_msk[1] = 0;
      	for(j=1; j<fl_size; j++)
      	{	
			fl_ix = (j-1)/32;
			if(datbuf[j] == '1')    // seting primary front riser masks
			  	pfdsr_msk[fl_ix] |= fl_mask[j];

			if(datbuf1[j] == '1')   // seting secondary front riser masks
			  	sfdsr_msk[fl_ix] |= fl_mask[j];

			if(datbuf2[j] == '1')    // seting primary rear riser masks
			  	prdsr_msk[fl_ix] |= fl_mask[j];
	
			if(datbuf3[j] == '1')   // seting secondary rear or third front riser masks
			  	srdsr_msk[fl_ix] |= fl_mask[j];
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-BPI") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[Blind_Floor] = (int16)atoi(datbuf);

   		fl_fgets(datbuf,180,fp1);
      	Blind_flmrk[0][0] = datbuf[0];
   	  	Blind_flmrk[0][1] = datbuf[1];

      	fl_fgets(datbuf,180,fp1);
      	cons[Blind_Floor_2] = (int16)atoi(datbuf);

   		fl_fgets(datbuf,180,fp1);
      	Blind_flmrk[1][0] = datbuf[0];
   	  	Blind_flmrk[1][1] = datbuf[1];
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SF2") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[SecFlCfg_2] = (int16)atoi(datbuf);

		// Read Security 2 mask for each car
	    for(i=1; i<=8; i++)
	    {
	      	fl_fgets(datbuf,180,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,180,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	ccs2msk[i][j] = 1;
				else
				  	ccs2msk[i][j] = 0;
				if(datbuf1[j] == '1')   // seting up call masks
				  	ucs2msk[i][j] = 1;
				else
				  	ucs2msk[i][j] = 0;
				if(datbuf2[j] == '1')   // seting dn call masks
				  	dcs2msk[i][j] = 1;
				else
				  	dcs2msk[i][j] = 0;
				if(datbuf3[j] == '1')   // seting rear car call masks
				  	rccs2msk[i][j] = 1;
				else
				  	rccs2msk[i][j] = 0;
				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rucs2msk[i][j] = 1;
				else
				  	rucs2msk[i][j] = 0;
				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rdcs2msk[i][j] = 1;
				else
				  	rdcs2msk[i][j] = 0;
	      	}
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SF3") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[SecFlCfg_3] = (int16)atoi(datbuf);

		// Read Security 3 mask for each car
	    for(i=1; i<=8; i++)
	    {
	      	fl_fgets(datbuf,180,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,180,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	ccs3msk[i][j] = 1;
				else
				  	ccs3msk[i][j] = 0;
				if(datbuf1[j] == '1')   // seting up call masks
				  	ucs3msk[i][j] = 1;
				else
				  	ucs3msk[i][j] = 0;
				if(datbuf2[j] == '1')   // seting dn call masks
				  	dcs3msk[i][j] = 1;
				else
				  	dcs3msk[i][j] = 0;
				if(datbuf3[j] == '1')   // seting rear car call masks
				  	rccs3msk[i][j] = 1;
				else
				  	rccs3msk[i][j] = 0;
				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rucs3msk[i][j] = 1;
				else
				  	rucs3msk[i][j] = 0;
				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rdcs3msk[i][j] = 1;
				else
				  	rdcs3msk[i][j] = 0;
	      	}
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SF4") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[SecFlCfg_4] = (int16)atoi(datbuf);

		// Read Security 4 mask for each car
	    for(i=1; i<=8; i++)
	    {
	      	fl_fgets(datbuf,180,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,180,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	ccs4msk[i][j] = 1;
				else
				  	ccs4msk[i][j] = 0;
				if(datbuf1[j] == '1')   // seting up call masks
				  	ucs4msk[i][j] = 1;
				else
				  	ucs4msk[i][j] = 0;
				if(datbuf2[j] == '1')   // seting dn call masks
				  	dcs4msk[i][j] = 1;
				else
				  	dcs4msk[i][j] = 0;
				if(datbuf3[j] == '1')   // seting rear car call masks
				  	rccs4msk[i][j] = 1;
				else
				  	rccs4msk[i][j] = 0;
				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rucs4msk[i][j] = 1;
				else
				  	rucs4msk[i][j] = 0;
				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rdcs4msk[i][j] = 1;
				else
				  	rdcs4msk[i][j] = 0;
	      	}
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-CBVIP") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[CB_msk] = (int16)atoi(datbuf);

		// Read code blue assignment floor mask
      	fl_fgets(datbuf,180,fp1);            // reading up call mask
      	fl_fgets(datbuf1,180,fp1);            // reading dn call mask
		g_cbmsk[0] = 0;
		g_cbmsk[1] = 0;
		g_rcbmsk[0] = 0;
		g_rcbmsk[1] = 0;
      	for(j=1; j<fl_size; j++)
      	{	
			fl_ix = (j-1)/32;
			if(datbuf[j] == '1')    // seting up call masks
			  	g_cbmsk[fl_ix] |= fl_mask[j];
	
			if(datbuf1[j] == '1')    // seting up call masks
			  	g_rcbmsk[fl_ix] |= fl_mask[j];
	
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C9") != 0)	// look for next file continuation
	{
	    for(i=111; i<=130; i++)             // reading ADDED job constants
	    {			// 111 to 130 max to keep cons file the same
	      	fl_fgets(datbuf,180,fp1);
	      	cons[i] = (int16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-CBSR") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[CB_SR] = (int16)atoi(datbuf);		// get code blue car option flag
      	fl_fgets(datbuf,180,fp1);			// get code blue car mask
      	fl_fgets(datbuf1,180,fp1);			// get code blue car mask
	    for(j=1; j<=8; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	valid_cbpr_car[j] = 1;
			else
			  	valid_cbpr_car[j] = 0;
			if(datbuf1[j] == '1')    // set front floor masks
			  	valid_cbsr_car[j] = 1;
			else
			  	valid_cbsr_car[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-EPSF") != 0)	// look for next file continuation
	{

      	fl_fgets(datbuf,180,fp1);
      	cons[EPSF] = (int16)atoi(datbuf);		// get Emergency Power Split Feeder option flag
      	fl_fgets(datbuf,180,fp1);			// get Power Feeder car mask
		if (cons[EPSF] == 1)
		{
		    for(j=1; j<=8; j++)
		    {	
				if(datbuf[j] == '1')    // set front floor masks
				  	power_feeder[j] = 1;
				else
				  	power_feeder[j] = 0;
			}
		}
		else
		{
		    for(j=1; j<=8; j++)
			  	power_feeder[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-VIPMSK") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[VIP_msk] = (int16)atoi(datbuf);

		// Read vip assignment floor mask
      	fl_fgets(datbuf,180,fp1);            // reading up call mask
      	fl_fgets(datbuf1,180,fp1);            // reading dn call mask
		g_vipmsk[0] = 0;
		g_vipmsk[1] = 0;
		g_rvipmsk[0] = 0;
		g_rvipmsk[1] = 0;
      	for(j=1; j<fl_size; j++)
      	{	
			fl_ix = (j-1)/32;
			if(datbuf[j] == '1')    // seting up call masks
			  	g_vipmsk[fl_ix] |= fl_mask[j];
	
			if(datbuf1[j] == '1')    // seting up call masks
			  	g_rvipmsk[fl_ix] |= fl_mask[j];
	
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C10") != 0)	// look for next file continuation
	{
	    for(i=134; i<=153; i++)             // reading ADDED job constants
	    {			// 134 to 153 max to keep cons file the same
	      	fl_fgets(datbuf,180,fp1);
	      	cons[i] = (int16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-TR") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[Third_Riser] = (int16)atoi(datbuf);

		// Read Security 4 mask for each car
	    for(i=1; i<=8; i++)
	    {
	      	fl_fgets(datbuf,180,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,180,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,180,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,180,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,180,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,180,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,180,fp1);            // reading rear dn call mask
	      	for (j=1; j<fl_size; j++)
	      	{
				if (datbuf[j] == '1')    // seting car call masks
				  	trccmsk[i][j] = 1;
				else
				  	trccmsk[i][j] = 0;
				if (datbuf1[j] == '1')   // seting up call masks
				  	trucmsk[i][j] = 1;
				else
				  	trucmsk[i][j] = 0;
				if (datbuf2[j] == '1')   // seting dn call masks
				  	trdcmsk[i][j] = 1;
				else
				  	trdcmsk[i][j] = 0;
				if (datbuf3[j] == '1')    // seting car call masks
				  	rtrccmsk[i][j] = 1;
				else
				  	rtrccmsk[i][j] = 0;
				if (datbuf4[j] == '1')   // setting rear up call masks
				  	rtrucmsk[i][j] = 1;
				else
				  	rtrucmsk[i][j] = 0;
				if (datbuf5[j] == '1')   // setting rear dn call masks
				  	rtrdcmsk[i][j] = 1;
				else
				  	rtrdcmsk[i][j] = 0;
				if ((trucmsk[i][j] == 1) || (trdcmsk[i][j] == 1) || (rtrucmsk[i][j] == 1) || (rtrdcmsk[i][j] == 1))
				{
					if (i == cons[carnmb])
						valid_fl[j] = 1;
					Valid_TR_Car[i] = 1;
				}
	      	}
	    }
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
	if (strstr(datbuf,"Continue-EPSEL") != 0)	// look for next file continuation
	{	
      	fl_fgets(datbuf,180,fp1);
      	cons[EPSEL_tables] = (int16)atoi(datbuf);	
      	fl_fgets(datbuf,180,fp1);
      	fl_fgets(datbuf1,180,fp1);
      	fl_fgets(datbuf2,180,fp1);
      	fl_fgets(datbuf3,180,fp1);  				
		if (cons[EPSEL_tables] == 1)
		{
		    for(j=1; j<=8; j++)
		    {	
		    	charbuf[0] = datbuf[j];
		    	k = (int16)atoi(charbuf);
		    	if((k>=1) && (k<=8))
		    		car_recall_order_fixed[0][j] = k;
		    	else
		    		car_recall_order_fixed[0][j] = 0;
		    }
			for(j=1; j<=8; j++)
		    {	
		    	charbuf[0] = datbuf1[j];
		    	k = (int16)atoi(charbuf);
		    	if((k>=1) && (k<=8))
		    		car_select_order_fixed[0][j] = k;
		    	else
		    		car_select_order_fixed[0][j] = 0;
		    }
			for(j=1; j<=8; j++)
		    {	
		    	charbuf[0] = datbuf2[j];
		    	k = (int16)atoi(charbuf);
		    	if((k>=1) && (k<=8))
		    		car_recall_order_fixed[1][j] = k;
		    	else
		    		car_recall_order_fixed[1][j] = 0;
		    }
			for(j=1; j<=8; j++)
		    {	
		    	charbuf[0] = datbuf3[j];
		    	k = (int16)atoi(charbuf);
		    	if((k>=1) && (k<=8))
		    		car_select_order_fixed[1][j] = k;
		    	else
		    		car_select_order_fixed[1][j] = 0;
		    }
		}
		else
		{
			for(j=1; j<=8; j++)
		    {	
	    		car_recall_order_fixed[0][j] = 0;
	    		car_select_order_fixed[0][j] = 0;
	    		car_recall_order_fixed[1][j] = 0;
	    		car_select_order_fixed[1][j] = 0;
		    }		

		}		
			
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;	
	}
	
	if (strstr(datbuf,"Continue-DD") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[Dest_Disp] = (int16)atoi(datbuf);

		g_ddmsk[0] = 0;
		g_ddmsk[1] = 0;
		g_rddmsk[0] = 0;
		g_rddmsk[1] = 0;
		// Get Kiosk locations and set floor mask
	    for(i=1; i<=60; i++)
    	{	   
      		fl_fgets(datbuf,180,fp1);
      		if (i < fl_size)
      		{
	      		dd_hd[i] = (int8)atoi(datbuf);
				fl_ix = (i-1)/32;
	      		if (dd_hd[i] != 0)
				  	g_ddmsk[fl_ix] |= fl_mask[i];
      		}
	    }
	    for(i=1; i<=60; i++)
    	{	        
      		fl_fgets(datbuf,180,fp1);
      		if (i < fl_size)
      		{
	      		dd_rhd[i] = (int8)atoi(datbuf);
				fl_ix = (i-1)/32;
	      		if (dd_rhd[i] != 0)
				  	g_rddmsk[fl_ix] |= fl_mask[i];
      		}
	    }
		
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
		
	}

	if (strstr(datbuf,"Continue-C11") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,180,fp1);
      	cons[pbdoormsk] = (int16)atoi(datbuf);		// pb door option flag
      	fl_fgets(datbuf,180,fp1);			// get pb door front floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	pbdoor_fmsk[j] = 1;
			else
			  	pbdoor_fmsk[j] = 0;
		}
      	fl_fgets(datbuf,180,fp1);			// get pb door rear floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set rear floor masks
			  	pbdoor_rmsk[j] = 1;
			else
			  	pbdoor_rmsk[j] = 0;
		}
		if (fl_fgets(datbuf,180,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}


Exit_Gracefully:

    fl_fclose(fp1);
}

#else

///////////////////////

// **********************************************
// This is read in constants that set up the board test program 
// tagwbg 19
// **********************************************
void rdconsB_std()
{
  	int16 i,j,k;
  	int16 fl_ix;

  	FL_FILE *fp1;
  
  	disable_dog();
  	
  	fp1 = fl_fopen("/consb.dat","r");
	if (fp1 == NULL)
  	 	fp1 = fl_fopen("/CONSB.DAT","r");		

	if (fp1 != NULL)
	{  
        for(i=0; i<31; i++)             // reading board test program data
        {			// 31 max to keep consB file the same
          	fl_fgets(datbuf,120,fp1);
          	consB[i] = (uint16)(atoi(datbuf));
        }	    
	}	
	else
	{
        for(i=0; i<31; i++)             // set all to fixed value
        {		          	
          	consB[i] = 0;
        }	    
	}
		fl_fclose(fp1);
}

/////////////////////

// **********************************************
// This is read in constants that define the job
// **********************************************
void rdcons_std()
{
  	int16 i,j,k;
  	int16 fl_ix;

  	FL_FILE *fp1;
  
  	disable_dog();

    fp1 = fl_fopen("/cons.dat","r");
	if (fp1 == NULL)
  	 	fp1 = fl_fopen("/CONS.DAT","r");

    for(i=0; i<31; i++)             // reading job constants
    {			// 31 max to keep cons file the same
      	fl_fgets(datbuf,120,fp1);
      	cons[i] = (uint16)(atoi(datbuf));
    }

	if (cons[speed] == 0)
		cons[speed] = 100;

	// Read Drive Type for each car
    for(i=1; i<=6; i++)             // reading job constants
    {
      	fl_fgets(datbuf,120,fp1);
      	Drive_Type[i] = (uint8)atoi(datbuf);
    }


	bot_rfl = cons[topf];

    for(i=1; i<=6; i++)
    {
      	fl_fgets(datbuf,120,fp1);             // reading video position
		j = 0;
		k = 0;
		car_bld_no[i][0] = ' ';
		car_bld_no[i][1] = ' ';
		while ((j <= 2) && (k <= 1))
		{
			if ((datbuf[j] >= '0') && (datbuf[j] <= 'z'))  // does not include "/" (0x2f) or ' ' (0x20)
			{
				car_bld_no[i][k] = datbuf[j];
				k++;
			}
			j++;
		}
      	fl_fgets(datbuf,120,fp1);             // reading car call mask
      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
	  	front_op[i] = 0;
	  	rear_op[i] = 0;
	  	top_fl[i] = 0;
      	top_rfl[i] = 0;
      	for(j=1; j<=fl_size; j++)
      	{		//     ^ Changed from 31 to 32 and back to 31
			if(datbuf[j] == '1')    // seting car call masks
			{
			  	ccnmsk[i][j] = 1;
			  	cons_init = 1;
			}
			else
			  	ccnmsk[i][j] = 0;
			if(datbuf1[j] == '1')   // seting up call masks
			  	ucnmsk[i][j] = 1;
			else
			  	ucnmsk[i][j] = 0;
			if(datbuf2[j] == '1')   // seting dn call masks
			  	dcnmsk[i][j] = 1;
			else
			  	dcnmsk[i][j] = 0;
			if(datbuf3[j] == '1')   // seting rear car call masks
			  	rccnmsk[i][j] = 1;
			else
			  	rccnmsk[i][j] = 0;
			if(datbuf4[j] == '1')   // setting rear up call masks
			  	rucnmsk[i][j] = 1;
			else
			  	rucnmsk[i][j] = 0;
			if(datbuf5[j] == '1')   // setting rear dn call masks
			  	rdcnmsk[i][j] = 1;
			else
			  	rdcnmsk[i][j] = 0;
			if (i == cons[carnmb])
				valid_fl[j] = 0;
			if ((ccnmsk[i][j] == 1) || (ucnmsk[i][j] == 1) || (dcnmsk[i][j] == 1))
			{
				if (i == cons[carnmb])
					valid_fl[j] = 1;
				front_op[i] = 1;	 // set if the car has front doors
				top_fl[i] = j;		 // set the highest top floor for each car 
			}
			if ((rccnmsk[i][j] == 1) || (rucnmsk[i][j] == 1) || (rdcnmsk[i][j] == 1))
			{
				if (i == cons[carnmb])
				{
					valid_fl[j] = 1;
					if (j < bot_rfl)
						bot_rfl = j;
				}
				if ((j < grbot_rfl) || (grbot_rfl == 0))
					grbot_rfl = j;
				rear_op[i] = 1;		// set if the car has rear doors
				top_rfl[i] = j;		// set the highest rear top floor for each car
			}
      	}
		ccmsk[i] = &ccnmsk[i][0];
		ucmsk[i] = &ucnmsk[i][0];
		dcmsk[i] = &dcnmsk[i][0];
		rccmsk[i] = &rccnmsk[i][0];
		rucmsk[i] = &rucnmsk[i][0];
		rdcmsk[i] = &rdcnmsk[i][0];
    }
	

    fl_fgets(datbuf,120,fp1);       // reading in the floor markings
    for(i=1; i<=32; i++)
    {	  //     ^ Changed from 31 to 32
      	fl_fgets(datbuf,120,fp1);
      	if (i < fl_size)
      	{
      		flmrk[i][0] = datbuf[0];
      		flmrk[i][1] = datbuf[1];
      	}
    }
    fl_fgets(datbuf,120,fp1);       //reading in the job name
    j=0;
    while((datbuf[j] != '/') && (j < 25))
    {
      	jobnm[j] = datbuf[j];
      	j++;
    }
    jobnm[j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading address line number 1
    while((datbuf[j] != '/') && (j < 25))
    {
      	add1[j] = datbuf[j];
      	j++;
    }
    add1[j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading address line number 2
    while((datbuf[j] != '/') && (j < 25))
    {
      	add2[j] = datbuf[j];
      	j++;
    }
    add2[j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading company name
    while((datbuf[j] != '/') && (j < 25))
    {
      	cmp[j] = datbuf[j];
      	j++;
    }
    cmp[j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading IVT password
    while((datbuf[j] != '/') && (j < 25))
    {
      	pass[0][j] = datbuf[j];
      	j++;
    }
    pass[0][j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading IVT user name
    while((datbuf[j] != '/') && (j < 25))
    {
      	user[0][j] = datbuf[j];
      	j++;
    }
    user[0][j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading company pass word
    while((datbuf[j] != '/') && (j < 25))
    {
      	pass[1][j] = datbuf[j];
      	j++;
    }
    pass[1][j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading company user name
    while((datbuf[j] != '/') && (j < 25))
    {
      	user[1][j] = datbuf[j];
      	j++;
    }
    user[1][j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading video message line #1
    while((datbuf[j] != '/') && (j < 25))
    {
      	vidmes[0][j] = datbuf[j];
      	j++;
    }
    vidmes[0][j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading video message line #2
    while((datbuf[j] != '/') && (j < 25))
    {
      	vidmes[1][j] = datbuf[j];
      	j++;
    }
    vidmes[1][j] = 13;
    j=0;
    fl_fgets(datbuf,120,fp1);       //reading video message line #3
    while((datbuf[j] != '/') && (j < 25))
    {
      	vidmes[2][j] = datbuf[j];
      	j++;
    }
    vidmes[2][j] = 13;

	if (fl_fgets(datbuf,120,fp1) == 0)		// look for file continuation
		goto Exit_Gracefully;
	
	if (strstr(datbuf,"Continue-S1") != 0)
	{
		// Read Security Type for each car
	    for(i=1; i<=6; i++)             // reading job constants
	    {
	      	fl_fgets(datbuf,120,fp1);
	      	Security_Type[i] = (uint16)atoi(datbuf);
	    }

		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;

	} 	 // else security defaults to zero

	if (strstr(datbuf,"Continue-S2") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[SecFlCfg] = (uint16)atoi(datbuf);

		// Read Security mask for each car
	    for(i=1; i<=6; i++)
	    {
	      	fl_fgets(datbuf,120,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,120,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	ccsmsk[i][j] = 1;
				else
				  	ccsmsk[i][j] = 0;
				if(datbuf1[j] == '1')   // seting up call masks
				  	ucsmsk[i][j] = 1;
				else
				  	ucsmsk[i][j] = 0;
				if(datbuf2[j] == '1')   // seting dn call masks
				  	dcsmsk[i][j] = 1;
				else
				  	dcsmsk[i][j] = 0;
				if(datbuf3[j] == '1')   // seting rear car call masks
				  	rccsmsk[i][j] = 1;
				else
				  	rccsmsk[i][j] = 0;
				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rucsmsk[i][j] = 1;
				else
				  	rucsmsk[i][j] = 0;
				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rdcsmsk[i][j] = 1;
				else
				  	rdcsmsk[i][j] = 0;
	      	}
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C2") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);			// ascii date code is yyyymmdd
      	cons_date_code = atol(datbuf);		// converts to a long integer such as 20031204
	    for(i=32; i<=49; i++)             // reading ADDED job constants
	    {			// 32 to 50 max to keep cons file the same
	      	fl_fgets(datbuf,120,fp1);
	      	cons[i] = (uint16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C3") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
     	cons[StagRear] = (uint16)atoi(datbuf);		// get staggard rear option flag
      	fl_fgets(datbuf,120,fp1);			// get staggard rear floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set staggard rear floor masks
			  	stag_rmsk[j] = 1;
			else
			  	stag_rmsk[j] = 0;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C4") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[LbyUpReq] = (uint16)atoi(datbuf);		// get Lobby Up Request option flag
      	fl_fgets(datbuf,120,fp1);			// get mask for cars that service this call
	    for(j=1; j<=6; j++)
	    {	
			if(datbuf[j] == '1')    // set lobby up request floor masks
			  	lby_up_req_car[j] = 1;
			else if (datbuf[j] == '3')
			  	lby_up_req_car[j] = 3;
			else if (datbuf[j] == '5')
			  	lby_up_req_car[j] = 5;
			else
			  	lby_up_req_car[j] = 0;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C5") != 0)	// look for next file continuation
	{
	    for(i=52; i<=72; i++)             // reading ADDED job constants
	    {			// 52 to 72 max to keep cons file the same
	      	fl_fgets(datbuf,120,fp1);
	      	cons[i] = (uint16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C6") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[heavy_door] = (uint16)atoi(datbuf);		// get heavy door option flag
      	fl_fgets(datbuf,120,fp1);			// get heavy door front floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	heavy_door_fmsk[j] = 1;
			else
			  	heavy_door_fmsk[j] = 0;
		}
      	fl_fgets(datbuf,120,fp1);			// get heavy door rear floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set rear floor masks
			  	heavy_door_rmsk[j] = 1;
			else
			  	heavy_door_rmsk[j] = 0;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
	if (strstr(datbuf,"Continue-C7") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[stag_auto_door] = (uint16)atoi(datbuf);		// get heavy door option flag
      	fl_fgets(datbuf,120,fp1);			// get staggerd auto/swing door front floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	stag_ado_msk[j] = 1;
			else
			  	stag_ado_msk[j] = 0;
		}
      	fl_fgets(datbuf,120,fp1);			// get staggerd auto/swing door rear floor mask
	    for(j=1; j<fl_size; j++)
	    {	
			if(datbuf[j] == '1')    // set rear floor masks
			  	stag_ado_rmsk[j] = 1;
			else
			  	stag_ado_rmsk[j] = 0;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-CB") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[CBCar] = (uint16)atoi(datbuf);		// get code blue car option flag
      	fl_fgets(datbuf,120,fp1);			// get code blue car mask
	    for(j=1; j<=6; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	valid_cb_car[j] = 1;
			else
			  	valid_cb_car[j] = 0;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-S3") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[Car_Asgn_Fl] = (uint16)atoi(datbuf);		// get security option flag
      	fl_fgets(datbuf,120,fp1);             // reading car call mask
      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
	    for(j=1; j<fl_size; j++)
	    {	
			if ((datbuf[j] >= '0') && (datbuf[j] <= '8'))
				car_asgn_cc[j] = (uint16)(datbuf[j] - '0');	  // set front floor car
			else
			  	car_asgn_cc[j] = 0;

			if ((datbuf1[j] >= '0') && (datbuf1[j] <= '8'))
				car_asgn_uc[j] = (uint16)(datbuf1[j] - '0');	  // set front floor car
			else
			  	car_asgn_uc[j] = 0;

			if ((datbuf2[j] >= '0') && (datbuf2[j] <= '8'))
				car_asgn_dc[j] = (uint16)(datbuf2[j] - '0');	  // set front floor car
			else
			  	car_asgn_dc[j] = 0;

			if ((datbuf3[j] >= '0') && (datbuf3[j] <= '8'))
				car_asgn_rcc[j] = (uint16)(datbuf3[j] - '0');	  // set rear floor car
			else
			  	car_asgn_rcc[j] = 0;

			if ((datbuf4[j] >= '0') && (datbuf4[j] <= '8'))
				car_asgn_ruc[j] = (uint16)(datbuf4[j] - '0');	  // set rear floor car
			else
			  	car_asgn_ruc[j] = 0;

			if ((datbuf5[j] >= '0') && (datbuf5[j] <= '8'))
				car_asgn_rdc[j] = (uint16)(datbuf5[j] - '0');	  // set rear floor car
			else
			  	car_asgn_rdc[j] = 0;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-EP") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[EPCarRun] = (uint16)atoi(datbuf);		// get Emergency Power car option flag
      	fl_fgets(datbuf,120,fp1);			// get Emergency Power car mask
		if (cons[EPCarRun] == 1)
		{
		    for(j=1; j<=6; j++)
		    {	
				if(datbuf[j] == '1')    // set front floor masks
				  	valid_ep_car[j] = 1;
				else
				  	valid_ep_car[j] = 0;
			}
		}
		else
		{
		    for(j=1; j<=6; j++)
			  	valid_ep_car[j] = 1;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SR") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[Second_Riser] = (uint16)atoi(datbuf);

		// Read Second Riser mask for each car
	    for(i=1; i<=6; i++)
	    {
	      	fl_fgets(datbuf,120,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,120,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	irccmsk[i][j] = 1;
				else
				  	irccmsk[i][j] = 0;

				if(datbuf1[j] == '1')   // seting up call masks
				  	irucmsk[i][j] = 1;
				else
				  	irucmsk[i][j] = 0;

				if(datbuf2[j] == '1')   // seting dn call masks
				  	irdcmsk[i][j] = 1;
				else
				  	irdcmsk[i][j] = 0;

				if(datbuf3[j] == '1')    // seting car call masks
				  	rirccmsk[i][j] = 1;
				else
				  	rirccmsk[i][j] = 0;

				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rirucmsk[i][j] = 1;
				else
				  	rirucmsk[i][j] = 0;

				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rirdcmsk[i][j] = 1;
				else
				  	rirdcmsk[i][j] = 0;

				if ((irucmsk[i][j] == 1) || (irdcmsk[i][j] == 1) || (rirucmsk[i][j] == 1) || (rirdcmsk[i][j] == 1))
				{
					if (i == cons[carnmb])
						valid_fl[j] = 1;
					Valid_SR_Car[i] = 1;
				}
				if ((rirucmsk[i][j] == 1) || (rirdcmsk[i][j] == 1))
				{
					if ((j < grbot_rfl) || (grbot_rfl == 0))
						grbot_rfl = j;
					rear_op[i] = 1;		// set if the car has rear doors
					top_rfl[i] = j;		// set the highest rear top floor for each car
				}
	      	}
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-VIP") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[VipSvc] = (uint16)atoi(datbuf);		// get vip car option flag
      	fl_fgets(datbuf,120,fp1);			// get vip car mask
	    for(j=1; j<=6; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	valid_vip_car[j] = 1;
			else
			  	valid_vip_car[j] = 0;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
		
	if (strstr(datbuf,"Continue-C8") != 0)	// look for next file continuation
	{
	    for(i=80; i<=99; i++)             // reading ADDED job constants
	    {			// 80 to 99 max to keep cons file the same
	      	fl_fgets(datbuf,120,fp1);
	      	cons[i] = (uint16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SAB") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[SAB_Fl] = (uint16)atoi(datbuf);

		// Read Sabbath floor mask
      	fl_fgets(datbuf,120,fp1);             // reading car call mask
      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
      	for(j=1; j<fl_size; j++)
      	{	
			if(datbuf[j] == '1')    // seting car call masks
			{
			  	sab_ccmsk[j] = 1;
				sab_cc_topff = j;
				if(sab_cc_botff == 0)
					sab_cc_botff = j;
			}
			else
			  	sab_ccmsk[j] = 0;
	
			if(datbuf1[j] == '1')   // seting up call masks
			  	sab_ucmsk[j] = 1;
			else
			  	sab_ucmsk[j] = 0;

			if(datbuf2[j] == '1')   // seting dn call masks
			  	sab_dcmsk[j] = 1;
			else
			  	sab_dcmsk[j] = 0;

			if(datbuf3[j] == '1')    // seting car call masks
			{
			  	sab_rccmsk[j] = 1;
				sab_cc_topfr = j;
				if(sab_cc_botfr == 0)
					sab_cc_botfr = j;
			}
			else
			  	sab_rccmsk[j] = 0;

			if(datbuf4[j] == '1')   // setting rear up call masks
			  	sab_rucmsk[j] = 1;
			else
			  	sab_rucmsk[j] = 0;

			if(datbuf5[j] == '1')   // setting rear dn call masks
			  	sab_rdcmsk[j] = 1;
			else
			  	sab_rdcmsk[j] = 0;

	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-XA") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[XA_Mask] = (uint16)atoi(datbuf);

		// Read cross assignment floor mask
      	fl_fgets(datbuf,120,fp1);            // reading up call mask
      	fl_fgets(datbuf1,120,fp1);            // reading dn call mask
      	for(j=1; j<fl_size; j++)
      	{	
			if(datbuf[j] == '1')    // seting up call masks
			  	xa_uc_msk[j] = 1;
			else if (datbuf[j] == '2')
			  	xa_uc_msk[j] = 2;
			else if (datbuf[j] == '3')
			  	xa_uc_msk[j] = 3;
			else
			  	xa_uc_msk[j] = 0;
	
			if(datbuf1[j] == '1')   // seting down call masks
			  	xa_dc_msk[j] = 1;
			else if (datbuf1[j] == '2')
			  	xa_dc_msk[j] = 2;
			else if (datbuf1[j] == '3')
			  	xa_dc_msk[j] = 3;
			else
			  	xa_dc_msk[j] = 0;

	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
	

	if (strstr(datbuf,"Continue-API") != 0)	// look for next file continuation
	{

      	fl_fgets(datbuf,120,fp1);
      	cons[Alt_Ascii_pi] = (uint16)atoi(datbuf);

	    for(i=1; i<=32; i++)
    	{	       
      		fl_fgets(datbuf,120,fp1);
      		if (i < fl_size)
      		{
		      	alt_flmrk[i][0] = datbuf[0];
	    	  	alt_flmrk[i][1] = datbuf[1];
      		}
	    }
		
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-XPI") != 0)	// look for next file continuation
	{

      	fl_fgets(datbuf,120,fp1);
      	cons[Xref_PI_Pos] = (uint16)atoi(datbuf);
	    for(i=1; i<=32; i++)
    	{	       
      		fl_fgets(datbuf,120,fp1);
      		if (i < fl_size)
	      		xpi_pos[i] = (uint8)atoi(datbuf);
	    }
		
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-DSR") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[Dual_Ser_Riser] = (uint16)atoi(datbuf);

		// Read serial riser floor mask
      	fl_fgets(datbuf,120,fp1);            // reading primary front riser mask
      	fl_fgets(datbuf1,120,fp1);            // reading secondary front riser mask
      	fl_fgets(datbuf2,120,fp1);            // reading primary rear riser mask
      	fl_fgets(datbuf3,120,fp1);            // reading secondary rear or third front riser mask
		pfdsr_msk[0] = 0;
		pfdsr_msk[1] = 0;
		sfdsr_msk[0] = 0;
		sfdsr_msk[1] = 0;
		prdsr_msk[0] = 0;
		prdsr_msk[1] = 0;
		srdsr_msk[0] = 0;
		srdsr_msk[1] = 0;
      	for(j=1; j<fl_size; j++)
      	{	
			fl_ix = (j-1)/32;
			if(datbuf[j] == '1')    // seting primary front riser masks
			  	pfdsr_msk[fl_ix] |= fl_mask[j];
	
			if(datbuf1[j] == '1')   // seting secondary front riser masks
			  	sfdsr_msk[fl_ix] |= fl_mask[j];

			if(datbuf2[j] == '1')    // seting primary rear riser masks
			  	prdsr_msk[fl_ix] |= fl_mask[j];
	
			if(datbuf3[j] == '1')   // seting secondary rear or third front riser masks
			  	srdsr_msk[fl_ix] |= fl_mask[j];
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-BPI") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[Blind_Floor] = (uint16)atoi(datbuf);

   		fl_fgets(datbuf,120,fp1);
      	Blind_flmrk[0][0] = datbuf[0];
   	  	Blind_flmrk[0][1] = datbuf[1];

      	fl_fgets(datbuf,120,fp1);
      	cons[Blind_Floor_2] = (uint16)atoi(datbuf);

   		fl_fgets(datbuf,120,fp1);
      	Blind_flmrk[1][0] = datbuf[0];
   	  	Blind_flmrk[1][1] = datbuf[1];
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SF2") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[SecFlCfg_2] = (uint16)atoi(datbuf);

		// Read Security 2 mask for each car
	    for(i=1; i<=6; i++)
	    {
	      	fl_fgets(datbuf,120,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,120,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	ccs2msk[i][j] = 1;
				else
				  	ccs2msk[i][j] = 0;
				if(datbuf1[j] == '1')   // seting up call masks
				  	ucs2msk[i][j] = 1;
				else
				  	ucs2msk[i][j] = 0;
				if(datbuf2[j] == '1')   // seting dn call masks
				  	dcs2msk[i][j] = 1;
				else
				  	dcs2msk[i][j] = 0;
				if(datbuf3[j] == '1')   // seting rear car call masks
				  	rccs2msk[i][j] = 1;
				else
				  	rccs2msk[i][j] = 0;
				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rucs2msk[i][j] = 1;
				else
				  	rucs2msk[i][j] = 0;
				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rdcs2msk[i][j] = 1;
				else
				  	rdcs2msk[i][j] = 0;
	      	}
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SF3") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[SecFlCfg_3] = (uint16)atoi(datbuf);

		// Read Security 3 mask for each car
	    for(i=1; i<=6; i++)
	    {
	      	fl_fgets(datbuf,120,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,120,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	ccs3msk[i][j] = 1;
				else
				  	ccs3msk[i][j] = 0;
				if(datbuf1[j] == '1')   // seting up call masks
				  	ucs3msk[i][j] = 1;
				else
				  	ucs3msk[i][j] = 0;
				if(datbuf2[j] == '1')   // seting dn call masks
				  	dcs3msk[i][j] = 1;
				else
				  	dcs3msk[i][j] = 0;
				if(datbuf3[j] == '1')   // seting rear car call masks
				  	rccs3msk[i][j] = 1;
				else
				  	rccs3msk[i][j] = 0;
				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rucs3msk[i][j] = 1;
				else
				  	rucs3msk[i][j] = 0;
				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rdcs3msk[i][j] = 1;
				else
				  	rdcs3msk[i][j] = 0;
	      	}
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-SF4") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[SecFlCfg_4] = (uint16)atoi(datbuf);

		// Read Security 4 mask for each car
	    for(i=1; i<=6; i++)
	    {
	      	fl_fgets(datbuf,120,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,120,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
	      	for(j=1; j<fl_size; j++)
	      	{	
				if(datbuf[j] == '1')    // seting car call masks
				  	ccs4msk[i][j] = 1;
				else
				  	ccs4msk[i][j] = 0;
				if(datbuf1[j] == '1')   // seting up call masks
				  	ucs4msk[i][j] = 1;
				else
				  	ucs4msk[i][j] = 0;
				if(datbuf2[j] == '1')   // seting dn call masks
				  	dcs4msk[i][j] = 1;
				else
				  	dcs4msk[i][j] = 0;
				if(datbuf3[j] == '1')   // seting rear car call masks
				  	rccs4msk[i][j] = 1;
				else
				  	rccs4msk[i][j] = 0;
				if(datbuf4[j] == '1')   // setting rear up call masks
				  	rucs4msk[i][j] = 1;
				else
				  	rucs4msk[i][j] = 0;
				if(datbuf5[j] == '1')   // setting rear dn call masks
				  	rdcs4msk[i][j] = 1;
				else
				  	rdcs4msk[i][j] = 0;
	      	}
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-CBVIP") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[CB_msk] = (uint16)atoi(datbuf);

		// Read code blue assignment floor mask
      	fl_fgets(datbuf,120,fp1);            // reading up call mask
      	fl_fgets(datbuf1,120,fp1);            // reading dn call mask
		g_cbmsk[0] = 0;
		g_cbmsk[1] = 0;
		g_rcbmsk[0] = 0;
		g_rcbmsk[1] = 0;
      	for(j=1; j<fl_size; j++)
      	{	
			fl_ix = (j-1)/32;
			if(datbuf[j] == '1')    // seting up call masks
			  	g_cbmsk[fl_ix] |= fl_mask[j];
	
			if(datbuf1[j] == '1')    // seting up call masks
			  	g_rcbmsk[fl_ix] |= fl_mask[j];
	
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C9") != 0)	// look for next file continuation
	{
	    for(i=111; i<=130; i++)             // reading ADDED job constants
	    {			// 111 to 130 max to keep cons file the same
	      	fl_fgets(datbuf,120,fp1);
	      	cons[i] = (uint16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-CBSR") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[CB_SR] = (uint16)atoi(datbuf);		// get code blue car option flag
      	fl_fgets(datbuf,120,fp1);			// get code blue car mask
      	fl_fgets(datbuf1,120,fp1);			// get code blue car mask
	    for(j=1; j<=6; j++)
	    {	
			if(datbuf[j] == '1')    // set front floor masks
			  	valid_cbpr_car[j] = 1;
			else
			  	valid_cbpr_car[j] = 0;
			if(datbuf1[j] == '1')    // set front floor masks
			  	valid_cbsr_car[j] = 1;
			else
			  	valid_cbsr_car[j] = 0;
		}
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

 	if (strstr(datbuf,"Continue-EPSF") != 0)	// look for next file continuation
	{
     	fl_fgets(datbuf,120,fp1);
      	cons[EPSF] = (uint16)atoi(datbuf);		// get Emergency Power Split Feeder option flag
      	fl_fgets(datbuf,120,fp1);			// get Power Feeder car mask
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
	
	if (strstr(datbuf,"Continue-VIPMSK") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[VIP_msk] = (int16)atoi(datbuf);

		// Read vip assignment floor mask
      	fl_fgets(datbuf,120,fp1);            // reading up call mask
      	fl_fgets(datbuf1,120,fp1);            // reading dn call mask
		g_vipmsk[0] = 0;
		g_vipmsk[1] = 0;
		g_rvipmsk[0] = 0;
		g_rvipmsk[1] = 0;
      	for(j=1; j<fl_size; j++)
      	{	
			fl_ix = (j-1)/32;
			if(datbuf[j] == '1')    // seting up call masks
			  	g_vipmsk[fl_ix] |= fl_mask[j];
	
			if(datbuf1[j] == '1')    // seting up call masks
			  	g_rvipmsk[fl_ix] |= fl_mask[j];
	
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}

	if (strstr(datbuf,"Continue-C10") != 0)	// look for next file continuation
	{
	    for(i=134; i<=153; i++)             // reading ADDED job constants
	    {			// 134 to 153 max to keep cons file the same
	      	fl_fgets(datbuf,120,fp1);
	      	cons[i] = (uint16)atoi(datbuf);
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
	
	if (strstr(datbuf,"Continue-TR") != 0)	// look for next file continuation
	{
      	fl_fgets(datbuf,120,fp1);
      	cons[Third_Riser] = (int16)atoi(datbuf);

		// Read Security 4 mask for each car
	    for(i=1; i<=6; i++)
	    {
	      	fl_fgets(datbuf,120,fp1);             // reading car number comment	(throw it away)
	      	fl_fgets(datbuf,120,fp1);             // reading car call mask
	      	fl_fgets(datbuf1,120,fp1);            // reading up call mask
	      	fl_fgets(datbuf2,120,fp1);            // reading dn call mask
	      	fl_fgets(datbuf3,120,fp1);            // reading rear car call mask
	      	fl_fgets(datbuf4,120,fp1);            // reading rear up call mask
	      	fl_fgets(datbuf5,120,fp1);            // reading rear dn call mask
	      	for (j=1; j<fl_size; j++)
	      	{
				if (datbuf[j] == '1')    // seting car call masks
				  	trccmsk[i][j] = 1;
				else
				  	trccmsk[i][j] = 0;
				if (datbuf1[j] == '1')   // seting up call masks
				  	trucmsk[i][j] = 1;
				else
				  	trucmsk[i][j] = 0;
				if (datbuf2[j] == '1')   // seting dn call masks
				  	trdcmsk[i][j] = 1;
				else
				  	trdcmsk[i][j] = 0;
				if (datbuf3[j] == '1')    // seting car call masks
				  	rtrccmsk[i][j] = 1;
				else
				  	rtrccmsk[i][j] = 0;
				if (datbuf4[j] == '1')   // setting rear up call masks
				  	rtrucmsk[i][j] = 1;
				else
				  	rtrucmsk[i][j] = 0;
				if (datbuf5[j] == '1')   // setting rear dn call masks
				  	rtrdcmsk[i][j] = 1;
				else
				  	rtrdcmsk[i][j] = 0;
				if ((trucmsk[i][j] == 1) || (trdcmsk[i][j] == 1) || (rtrucmsk[i][j] == 1) || (rtrdcmsk[i][j] == 1))
				{
					if (i == cons[carnmb])
						valid_fl[j] = 1;
					Valid_TR_Car[i] = 1;
				}
	      	}
	    }
		if (fl_fgets(datbuf,120,fp1) == 0)		// 	load next line
			goto Exit_Gracefully;
	}
	

Exit_Gracefully:

    fl_fclose(fp1);

}
#endif
// **********************************************
// Default the constants for an invalid cons file
// **********************************************
void rdcons_empty()
{
  int16 i,j,k;

  	disable_dog();

	
	cons[loadw] = 0;        // cons[loadw]=1 if load weighing switches, 2 = analog load weighing
	cons[arst] = 1;      	// cons[arst]=1 if automatic reset is enabled 0=no
	cons[att] = 0;       	// cons[att]=1 if attendant operation, 0=no, 2=annuciator in att + auto
	cons[behindc] = 0;      // cons[behindc]=1 if behind the car call cancel 0=no
	cons[bipolar] = 1;      // cons[bipolar]=1 if bipolar scurve 0 if unipolar
	cons[bottomf] = 1;      // cons[bottomf]= position of the bottom floor
	cons[carnmb] = 1;       // cons[carnmb] is the car number in the group
	cons[cblue] = 0;        // cons[cblue]=1 if code blue operation 0=no
	cons[dcpo] = 1;         // cons[dcpo]=1 if run with door close power 0=no
	cons[sercom] = 15;   	// cons[sercom] 1= Serial Hall Call, 2=Serial Car, 4=CC in COP, 8=drive, 16=door, 32=hc drivers
	cons[dispis] = 0;       // cons[dispis]= 0 = Binary, 1 if discrete position indicators, 
							//               +2 = Lobby lantern instead of ISER or RCM
							//               4 = Binary with OS flash Out of Service
							//               6 = Binary with Lobby Lanter and OS flash Out of Service
							//               8 = Use UDA and DDA on car for Att Up and Dn light
							//				16 = Use UDA and DDA on ATT but not on automatic
							//				32 = PI I/Os on 2nd car call board (used with RGB Car Call boards)
	cons[equake] = 0;       // cons[equake]=1 if california earth quake 0=no
	cons[fax] = 0;          // cons[fax]=1 if fax modem installed
	cons[frghtd] = 0;       // cons[frghtd]: 0=no
							//				 1=automatic power freight door  
							//				 2=peele
							//				 3=courion model E						
							//				 4=ems,											
							//				 5=courion model D						
							//				 6=front automatic power freight door 						
							//				 7=rear automatic power freight door 						
							//				 8=front and rear  automatic power freight door 						
							//				 9=Guilbert						
	cons[home] = 0;         // cons[home] is the floor number for simplex homing
	cons[ansi] = 2004;     	// cons[ansi] = Ansi code compliance, 1996, 2000, etc --- Previously = cons[hydro] 1=hydro 2=invert 3=vector 4=gen 5=scr 6=vector open loop
	cons[incrs] = 0;        // cons[incrs]=1 if inconscpicous riser, 2=ir or'ed with std hall calls, 3=special ir for car op in group
	cons[mand] = 0;         // cons[mand]: 	1 = if manual doors 0 if automatic doors, 
							// 				2 = swing door w/power gate, 
							// 				3 = fixed cam special (deleted), 
							// 				4 = manual staggered rear/automatic front,
							// 				5 = swing w/pwr gate stggered rear/auto front
							// 				6 = Swing rear door with automatic front.
							//				7 = Automatic front/swing front and manual rear
							//				8 = Swing front/manual doors at staggered floor with manual rear
							//				9 = Guilbert Door Operator
	cons[modem] = 0;        // cons[modem]=1 if modem installed
	cons[nmbcars] = 1;      // cons[nmbcars] is the number of cars in the group
	cons[nodzdc] = 0;      	// cons[nodzdc]=1 if canadian no door zone close door
	cons[firesv] = 0;       // cons[firesv]= fire service type: 0=Ansi 1996, 1=New York, 2=Ansi2000, 3=Chicago
	cons[otslbp] = 0;      	// cons[otslbp]=1 out of service lobby park door open, 2 = cycle door closed, 4=Sec Em Recall (RTLDC) and (RTL)
	cons[predo] = 0;        // cons[predo]=1 if preopen doors on stop 0=no
	cons[rear] = 0;         // cons[rear]=1 if rear doors 0=no
	cons[epgrp] = 0; 		// cons[epgrp]=car number if inter-group emergency operation; previously = cons[Encoder_PPM]
	cons[speed] = 150;      // cons[speed]= elevator speed in fpm
	cons[topf] = 8;         // cons[topf]= position of the top floor
	cons[brake] = 3;        // cons[brake] = 2 for electronic brake board, 3=3phase brake
	cons[grtopf] = 8;       // cons[grtopf]= the top floor of the group
	cons[BinPiIn] = 1;      // cons[BinPiIn] = 1 for binary input floor confirm
	cons[SecFlCfg] = 0;		// cons[SecFlCfg] = 1 to set masks for security floor configuration, 2 = multiple floor mask security, 3=secure hall call per floor per car from keyswitch 
	cons[hben] = 0;			// cons[hben] = 1 to use enable button on car to hear handicap buzzer, 2 = beep when car call hit 4== beep on car call ONLY
	cons[exDHwL] = 0;		// cons[exDHwL] = 1 extended door hold w/light and car call cancel door hold time      
	cons[medEm] = 0;		// cons[medEm] = 1 Medical Emergency Service
	cons[rsbuffer] = 0;		// cons[rsbuffer] = 1 Reduced stroke buffer, 0 = normal
	cons[naec] = 0;			// cons[naec] = 1 Set for special show software
	cons[access_type] = 0; 	// cons[access_type] = 0 No access, 1 = front top access, 2 front bottom, 4 = rear top, 8 = rear bottom
	cons[shortfl] = 0;		// cons[shortfl] = 0 No short floor, # of bottom short floor
	cons[saf_test] = 0;		// cons[saf_test] = 1 Shutdown if annual safety test flag not updated
	cons[ids] = 0;			// cons[ids] = 1 Enable Lift-Net Protocol from IDS
	cons[LW_dis_sw] = 0; 	// cons[LW_dis_sw] = 1 Load weigher disable switch
	cons[Elev_off] = 0;		// cons[Elev_off]:	+1 = Elevator off switch in COP EOF, +2 = Elevator Switch in Hall HEOF, +4 = Elevator off reset EOR,
							//					+8 = Elevator Off Output ELOO,
	cons[Remote_CC] = 0;	// cons[Remote_CC] = 1 Remote car call stations (car calls, DOB and DCB) :=2 use ED instead of INDC
	cons[GLBP_LED] = 0;		// cons[GLBP_LED] = 1 Rudy rule: output status of gate or lock bypass to LED mounted exernally.
	cons[Low_Press] = 0;	// cons[Low_Press] = 1 Hydro Pressure Switch input (EMH)
	cons[Low_Oil] = 0;		// cons[Low_Oil] = 1 Low Oil Switch	(COL)
	cons[MtrBlower] = 0;	// cons[MtrBlower] = 1 Motor blower output (uses GEN)
	cons[Sabbath] = 0;		// cons[Sabbath] = 1 Sabbath car calls, 2=cc set from gio, 3=Sabbath Hall calls 4 = car calls  5 = ir to ovride sabbath
	cons[Car_Sw] = 0;		// cons[Car_Sw] = 1 Car Switch
	cons[StagRear] = 0;		// cons[StagRear] = 0, No staggered rear, 1 = staggered Rear floor flag
	cons[LbyUpReq] = 0;		// cons[LbyUpReq] = 1 Lobby Up Hall Call Request (UC grtopf + 1), 1 in option but 5 in array = Lobby Up hall call but secf decides on standard hc assignment.
	cons[door_type] = 0;		// cons[door_type] = 1 OVL/6970
	cons[housing] = 0;		// cons[housing] = 1, NJ Housing Authority Job, 2 = NYCHA, 3 = Toronto housing
	cons[rearsel] = 0;		// cons[rearsel] = 1, Rear door i/o on seperate board
	cons[EMPop] = 0;		// cons[EMPop]: +1=Emergency Power Transfer, +2 = Hydro Emp Seq, +4 = Hydro S EPS, +8 Emp Status Indicators 
	cons[SFC_2004_IO] = 1;		// cons[SFC_2004_IO] == 1, SFC test (v3.06+) and 2004 I/O change: SS to CWS, FFS to SS and CTS.
	cons[sel_type] = 0;		// cons[sel_type]: 1=Rail Target Selector/Governor Encoder, 2=Encoded Tape, 3=Rail Target/CPU Gov Enc
	cons[pbdoor] = 0;		// cons[pbdoor] = 1, Momentary Push Button door operation, 2=Constant pressure pbdoor. 
	cons[DZDoorEn] = 0;		// cons[DZDoorEn] = 1, Door Zone Door Enable output on EML
							//				  = 2, Rear Door Zone Door Open Enable Input (RDZE)
	cons[dpp_upd] = 0;		// cons[dpp_upd] = 1, Dpp update and offset control if using dz interrupt (1038 j26 to +DZ) (1036 j6 to Ri1)
	cons[FSX_rear] = 0;		// cons[FSX_rear] = 1, Special operation for FSX as the rear door fire key switch and FS is front
	cons[RCM_HWDoor] = 0;	// cons[RCM_HWDoor] = 1, Retiring Cam to Close Hoistway Door, or with auto door
	cons[vshort_tfl] = 0; 	// cons[vshort_tfl] = 0 No very short floor, # of top very short floor (DTR used to switch door zone inputs)
	cons[Reset_Jack] = 0;	// cons[Reset_Jack] = 1, home and reset jack once a day
	cons[Micro_Relev] = 0;	// cons[Micro_Relev] = 1, micro releveling using UPML and DNML outputs.
	cons[Auto_Swing_DO] = 0;  // cons[Auto_Swing_DO] = 1, automatic swing door w/manual gate
							 // cons[Auto_Swing_DO] = 2, automatic swing door pulse auto door output
	cons[Park_NCU_ALB] = 0;	// cons[Park_NCU_ALB] = 1, Inputs for APRK, ALBY, NCU 
	cons[Handicap_HPB] = 0;	// cons[Handicap_HPB] = 1, Input for handicap car request
	cons[F1_CT] = 0;		// cons[F1CT] = 1, FB, FL on 1040 Car Top
	cons[F1_ML_AL] = 0;		// cons[F1_ML_AL] = 1, F1ML replaces EQL, F1AL replaces EML
	cons[shortflmid] = 0;	// cons[shortflmid] = 1, Second short floor in middle of hoistway
	cons[EM_BRK] = 0;		// cons[EM_BRK] = 1 Secondary Emergency Brake, 2=Sheave brake, 3=Sheave brake 2007, 
							// 4=Secondary Em Brake OR Sheave brake 2007 pick and drop each run, 5=Sheave Brake seperate BC and UTM.
							// 6=Electronic emergency brake
	cons[heavy_door] = 0;	// cons[heavy_door] = 1 heavy door operation.
	cons[stag_auto_door] = 0;	// cons[stag_auto_door] = 1 for staggered auto/swing door (cons[mand] == 7)
	cons[CBCar] = 0;		// cons[CBCar] = 1, Valid code blue car set in array in cons file.
	cons[Car_Asgn_Fl] = 0;	// cons[Car_Asgn_Fl] = 1, Car assigned to particular floor in cons floor array.
	cons[EPCarRun] = 0;		// cons[EPCarRun] = 1, Mask defines particular cars to run on EP.
	cons[Second_Riser] = 0;		// cons[Second_Riser] = 1, Dispatch call from Second Riser to multiple cars
	cons[VipSvc] = 0;		// cons[VipSvc] = 1 Priority Service (vip) flag
	cons[IndSec] = 0;		// cons[IndSec] = 1 Independent Security Service
	cons[hugs] = 0;			// cons[hugs] == 1, Hugs Security Service in HCS; cons[hugs] == 2, Hugs Security Service in GCS
	cons[GOV_Reset] = 0;	// cons[GOV_Reset] == 1, Electronic governor reset
	cons[cc_pb_sec] = 0;	// cons[cc_pb_sec] == 1, car call push button security
	cons[loc_svc_tmr] = 0;	// cons[loc_svc_tmr] == 1, Run service timer for non dispatcher car
	cons[Emp_Recover] = 0;	// cons[Emp_Recover] == 1, Traction with battery recovery, 2 = single phase ups
	cons[Cycle_Run] = 0;	// cons[Cycle_Run] == 1, Cycle car after sitting for cycle run hours (adjustable timer)
	cons[HCP_Fuse] = 0;		// cons[HCP_Fuse] == 1, Seperate hall call fuse split from fire service hall power
	cons[Smoke_2004] = 0;	// cons[Smoke_2004] == 1, for HWS2 Input, previously cons[Dual_Ser_Riser].
	cons[PWL_Brake ] = 0;	// cons[PWL_Brake] == 1, Manual Power Loss Brake, 2=MBC control only
	cons[INS_EN_SFC] = 0;	// cons[INS_EN_SFC] == 1, Inspection switch enable and read SFC input
	cons[FS2_SW] = 0;		// cons[FS2_SW] == 1, Reverse FS2 switch logic
	cons[Voltage_Sensor] = 0;	// Cons[Voltage_Sensor] == 1, Analog voltage sensor to detect phase loss and low voltage (3-phase + door)
	cons[Galileo] = 1;		// cons[Galileo] == 1, Enable control function of Galileo
	cons[EP_Profile] = 0;	// cons[EP_Profile] +1, Use Emergency Power Speed profile parameters on EP
							// cons[EP_Profile] +2, Enables RSPD input to switch Speed profile parameters.
	cons[UPS_Type] = 0;		// cons[UPS_Type], 0 for APC, 1 for Eaton (used for serial protocol)
	cons[RGB_CC] = 0;		// cons[RGB_CC], 1 = RBG Car call boards
	cons[aux_COP] = 0;	// cons[aux_COP], 1 = Aux RGB car call station
	cons[FSautoRST] = 0;	// cons[FSautoRST], 1 = FS Auto reset operation
	cons[Aux_Brake] = 1;	// cons[Aux_Brake], 1 = Aux Electronic Brake follows normal electronic brake 
	cons[SAB_Fl] = 0;		// cons[SAB_Fl] = 1, use floors from cons file table (if cons[Sabbath] == 1) use car call, (if cons[Sabbath] == 3) use hall calls
	cons[XA_Mask] = 0;		// Cross-Assignment Mask for which group floors are assigned.  1=GAL, 2=Other, 3=both
	cons[Alt_Ascii_pi] = 0; // enable alternate ascii pi's
	cons[Xref_PI_Pos] = 0;	// Cross Reference PI (Use a different number for the PI position at the floor)
	cons[Dual_Ser_Riser] = 0; // cons[Dual_Ser_Riser] == 1, Dual Serial Risers
	cons[Blind_Floor] = 0; 	// cons[Blind_Floor] == Floor, floor above the blind shaft to display blind shaft character on digital PI indicator.
	cons[Blind_Floor_2] = 0; // cons[Blind_Floor_2] == Floor, floor below the blind shaft to display blind shaft character on digital PI indicator.
	cons[SecFlCfg_2] = 0;	// cons[SecFlCfg_2] = 1 to set masks for security floor configuration, cons[SecFlCfg] must be set to 2 
	cons[SecFlCfg_3] = 0;	// cons[SecFlCfg_3] = 1 to set masks for security floor configuration, cons[SecFlCfg] must be set to 2 
	cons[SecFlCfg_4] = 0;	// cons[SecFlCfg_4] = 1 to set masks for security floor configuration, cons[SecFlCfg] must be set to 2 
	cons[CB_msk] = 0;		// cons[CB_msk] = 1 use floor mask for CB valid floors
	cons[FI_EP_Ext] = 0;	// cons[FI_EP_Ext] = 1 when 24 Volt Hall calls used	or for external Fire and Emp i/o	
	cons[Prison] = 0;		// cons[Prison] = 1. Special service for REMOTE COP as well as emergency recall
	cons[Super_Sabbath] = 0; // cons[Super_Sabbath] will enable the ouputs for SABUZ and SABEE for special Sabbath operation
	cons[incarvip] = 0;		// cons[incarvip] = 1. Special VIP feature, allows car to go into VIP service with an input called ICVIP located into COP 1st car call board 
	cons[exDHwLatFL] = 0;	// cons[exDHwLatFL] = 1 extended door hold w/light and car call cancel door hold time for one floor only (fvar) 
	cons[distress] = 0;		// cons[distress] = 1 enables osrL light output osrB buzzer output and osrA acknowledge input on external IO baord.
	cons[Australia] = 0;	// cons[Australia] = 1 enables special code and i/o's for Australia
	cons[TUG] = 0;			// cons[TUG] = 1 enables I/O interface and operation for TUG Hospital Robots
	cons[Ser_IGEP] = 0;		// cons[Ser_IGEP] = 1 Serial Intergroup Emergency Power
	cons[dflt_MOVFR] = 0;	// cons[dflt_MOVFR] = 1 EE to TOC with CAN MOVFR
	cons[MR_FBZ] = 0;		// cons[MR_FBZ] = 1, Fire buzzer in the machine room.
	cons[MR_INE] = 0;		// cons[MR_INE] = 1,2,3 Machine Room inspection enable input
	cons[EM_BKS] = 0;		// cons[EM_BKS] = 1,2,3 Seperate brake input
	cons[zoneSW] = 0;		// cons[zoneSW] = enables ZON input inside car station to allow the car to zone back to lobby when no calls are on
	cons[hotoilSW] = 0;		// cons[hotoilSW] = 1 enables hot oil switch. Feature will only work on hydros.
	cons[servOUT] = 0;		// cons[servOUT] = Service Message indicator +1= in COP +2 = Machine room, +4 = ISER output, +8 = OSERL, +16 Group Service output
	cons[enctype] = 0;		// cons[enctype] = 0-Turck, 1-Dynapar, 2-Wachendorff
	cons[vip_en] = 0;		// cons[vip_en] = +1. to have VIPEN input to enable making VIP calls (works similar to SECUR for ccs)
							// cons[vip_en] = +2. to have VPECC input in TOC to enable latching car calls when at vip floor doors open on group VIP service
	cons[svc_tmr_en] = 0;	// cons[svc_tmr_en] = 1 for enabling service timers enable input on the external io board
	cons[openlout ] = 0; 	// cons[openlout] = 1 for putting the OPENL output on the Fire Service board
	cons[CB_SR] = 0;		// cons[CB_SR] = 1 for code blue second riser
	cons[EPSF] = 0;			// cons[EPSF] = 1 for Emergency Power Split Feeder
	cons[VIP_msk] = 0;		// cons[VIP_msk] = 1 use floor mask for VIP valid floors

	cons[Hall_Lant] = 0;	// cons[Hall_Lant] = 1 add hall lanterns
	cons[CCS_BUS] = 0;		// cons[CCS_BUS]  0 = MRCAN, 1 = CTCAN
	cons[IC_INE] = 0;    	// cons[IC_INE] = 1, use DCB as enable input for In Car Inspection mode
	cons[smoke_interface] = 0;	// cons[smoke_interface], +1 = enables reading smoke sensor input per floor and provide ALTO and MESO outputs
							//					      +2 = disable hospital service over fire service if a call is made on the floor where smoke sensor was activated.
	cons[FiCntrlCentr] = 0; // cons[FiCntrlCentr] = 1 add outputs for fire control center
	cons[sec_dis] = 0;	  	// cons[sec_dis], +1 = disable car call security at lobby floor with a SCD pushbutton input for fvars[fvsecdist] amount of time
	cons[Intf_Voltage] = 0;	// cons[Intf_Voltage]: 0 = 24, 1 = 120Vac, bit 0: Lantern voltage, bit 1: HC Voltage, bit 2: FEP Voltage, bit 3: Car Call Voltage, bit 4: Discrete PI Volt
	cons[ep_auto_en] = 0;   // cons[ep_auto_en] = 1, allowing EM recalling with AUTEM enable input
	cons[Hardware_Config] = 0;	 // cons[Hardware_Config]: Bit 0 = 1 DET not on COP
	cons[fire_smoke_nc] = 0;  // cons[fire_smoke_nc] = 1, all smoke detector inputs are normally closed contacts
	cons[eetst_dis] = 0;		// cons[eetst_dis] = 1, disable ee test when using peele auto door interface
	cons[Car_Elev_off] = 0; 	// cons[Car_Elev_off]:	+1 = Elevator Switch in Car CEOF, 
								//						+2 = Elevator off reset CEOFR for CEOF,
								//						+4 = Elevator Off OutputCEOFL for CEOF,
	cons[Unused146] = 0;
	cons[Unused147] = 0;
	cons[Unused148] = 0;
	cons[Unused149] = 0;
	cons[Unused150] = 0;
	cons[Unused151] = 0;
	cons[Unused152] = 0;
	cons[Unused153] = 0;
	cons[Third_Riser] = 0;		// cons[Third_Riser] = 1, Dispatch calls from Third Riser to multiple cars, uses rear IR boards
	cons[EPSEL_tables] = 0;		// cons[EPSEL_tables] = 1 use these tables to decide order and number of cars to recall and select
	cons[Dest_Disp] = 0;	  	// cons[Dest_Disp] = 1, destination dispatch group
	cons[pbdoormsk] = 0;		// cons[pbdoormsk] = 1, enable constant pressure pbdoor for floors with the mask set. 

	// Read Drive Type for each car
    for(i=1; i<car_size; i++)             // reading job constants
      	Drive_Type[i] = 3;

    for(i=1; i<car_size; i++)
    {
		j = 0;
		k = 0;
		car_bld_no[i][0] = ' ';
		car_bld_no[i][1] = ' ';
		front_op[i] = 1;	 // set if the car has front doors
  		rear_op[i] = 0;
	  	top_fl[i] = cons[grtopf];
      	top_rfl[i] = 0;
      	for(j=1; j<fl_size; j++)
      	{		//     ^ Changed from 31 to 32 and back to 31
		  	cons_init = 0;
		  	if (j <= cons[topf])
		  	{
			  	ccnmsk[i][j] = 1;
			  	if (j < cons[topf])
			  		ucnmsk[i][j] = 1;
			  	else
			  		ucnmsk[i][j] = 0;
			  	if (j > cons[bottomf])
			  		dcnmsk[i][j] = 1;
			  	else
			  		dcnmsk[i][j] = 0;
		  	}
		  	else
		  	{
			  	ccnmsk[i][j] = 0;
			  	ucnmsk[i][j] = 0;
			  	dcnmsk[i][j] = 0;
		  		
		  	}
		  	rccnmsk[i][j] = 0;
		  	rucnmsk[i][j] = 0;
		  	rdcnmsk[i][j] = 0;
			if (i == cons[carnmb])
			{
				if ((ccnmsk[i][j] == 1) || (ucnmsk[i][j] == 1) || (dcnmsk[i][j] == 1))
					valid_fl[j] = 1;
				else
					valid_fl[j] = 0;
			}
      	}
		ccmsk[i] = &ccnmsk[i][0];
		ucmsk[i] = &ucnmsk[i][0];
		dcmsk[i] = &dcnmsk[i][0];
		rccmsk[i] = &rccnmsk[i][0];
		rucmsk[i] = &rucnmsk[i][0];
		rdcmsk[i] = &rdcnmsk[i][0];
    }
	

    for(i=1; i<fl_size; i++)
    {	  //     ^ Changed from 31 to 32
    	if (i < 10)
    	{
    		
      		flmrk[i][0] = ' ';
      		flmrk[i][1] = (int8)(i + '0');
    	}
    	else if (i < 20)
    	{
      		flmrk[i][0] = '1';
      		flmrk[i][1] = (int8)((i-10) + '0');
    		
    	}
    	else if (i < 30)
    	{
      		flmrk[i][0] = '2';
      		flmrk[i][1] = (int8)((i-20) + '0');
    	}
    	else if (i < 40)
    	{
      		flmrk[i][0] = '3';
      		flmrk[i][1] = (int8)((i-30) + '0');
    	}
    	else if (i < 50)
    	{
      		flmrk[i][0] = '4';
      		flmrk[i][1] = (int8)((i-40) + '0');
    	}
    	else if (i < 60)
    	{
      		flmrk[i][0] = '5';
      		flmrk[i][1] = (int8)((i-50) + '0');
    	}
    	else
    	{
      		flmrk[i][0] = '6';
      		flmrk[i][1] = (int8)((i-60) + '0');
    	}
    }

    for(j=1; j<10; j++)
      	jobnm[j] = ' ';
    jobnm[j] = 13;

    for(j=1; j<10; j++)
      	add1[j] = ' ';
    add1[j] = 13;

    for(j=1; j<10; j++)
      	add2[j] = ' ';
    add2[j] = 13;

    for(j=1; j<10; j++)
      	cmp[j] = ' ';
    cmp[j] = 13;

    for(j=1; j<10; j++)
      	pass[0][j] = ' ';
    pass[0][j] = 13;

    for(j=1; j<10; j++)
      	user[0][j] = ' ';
    user[0][j] = 13;

    for(j=1; j<10; j++)
      	pass[1][j] = ' ';
    pass[1][j] = 13;

    for(j=1; j<10; j++)
      	user[1][j] = ' ';
    user[1][j] = 13;

    for(j=1; j<10; j++)
      	vidmes[0][j] = ' ';
    vidmes[0][j] = 13;

    for(j=1; j<10; j++)
      	vidmes[1][j] = ' ';
    vidmes[1][j] = 13;

    for(j=1; j<10; j++)
      	vidmes[2][j] = ' ';
    vidmes[2][j] = 13;

}

// *******************************************
// This is read in hoistway tables for 
// simulator - Gives capability of fixing 
// floor positions basd on hoistway.dat file
// *******************************************

void rdhoistway(void)
{

#if(Simulator == 1)

	int16 i,j,k;
  	int16 fl_ix;

  	FL_FILE *fp1;
  
  	disable_dog();

    fp1 = fl_fopen("/hoistway.dat","r");
	
	if (fp1 != NULL)
	{
		hw_tbl_file = 1;	
		
      	fl_fgets(datbuf,120,fp1);
      	hstw.dnl = atoi(datbuf);	

      	fl_fgets(datbuf,120,fp1);
      	hstw.unl = atoi(datbuf);
      	

      	fl_fgets(datbuf,120,fp1);
      	hstw.dtl = atoi(datbuf);

      	fl_fgets(datbuf,120,fp1);
      	hstw.utl = atoi(datbuf);

		hstw.pps[0] = 0;

		for (i = 1; i < 61; ++i)
		{
	      	fl_fgets(datbuf,120,fp1);
	      	hstw.pps[i] = atoi(datbuf);
		}      	

 		fl_fclose(fp1);
	}


	enable_dog();
	

#endif
}


// *******************************************
// This is read in faults
// *******************************************
void rdflts(void)
{
  	int16 i,j,k;
	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		

	k = MRAM_Flts_Dat_Offset;
    if(rd_mram(MRAM_Flts_Dat_Page,k) != 0x009E)
    {
		Write_MRAM_Command(MRWREN,0);
	  	wr_mram(MRAM_Flts_Dat_Page,k,0x009E);
		k+=2;
		Flt_ix = 0;
	  	wr_mram(MRAM_Flts_Dat_Page,k,0x0000);	// fault index
	  	for(i=0; i<max_flt_ix; i++)
	  	{
			for(j=0; j<(int16)sizeof(Fault_type); j+=2)
			{
				f.fltw[i][j] = 0x0000;
			}
			k = MRAM_Flts_Dat_Offset + 4 + (i * sizeof(Fault_type));
			wr_mram_array(MRAM_Flts_Dat_Page,k,(int16 *)(&f.fltw[i]),(int16)(sizeof(Fault_type))/2); 
	  	}
		Write_MRAM_Command(MRWRDI,0);
    }
	k = MRAM_Flts_Dat_Offset+2;		  // Skip initialization flag byte
	Flt_ix = rd_mram(MRAM_Flts_Dat_Page,k);
    for(i=0; i<max_flt_ix; i++) // read in faults from ram disk
    {
		k = MRAM_Flts_Dat_Offset + 4 + (i * sizeof(Fault_type));
		rd_mram_array(MRAM_Flts_Dat_Page,k,(int16 *)(&f.fltw[i]),(int16)(sizeof(Fault_type))/2); 
    }
  	return;
}

// *******************************************
// Clear Long Term faults
// *******************************************
void clr_LT_flts(void)
{
	int16 i,j,k;
	
	Write_MRAM_Command(MRWREN,0);
	k = MRAM_LT_Flts_Dat_Offset;
  	wr_mram(MRAM_LT_Flts_Dat_Page,k,0x00AE);
	k+=2;
	LT_Flt_ix = 0;
  	wr_mram(MRAM_LT_Flts_Dat_Page,k,0x0000);	// fault index
	k+=2;
	for(j=0; j<(int16)sizeof(Fault_type); j+=2)
	{
		cf.fltw[0][j] = 0x0000;			// use externa car fault buffer [0] to zero lt faults
	}
  	for(i=0; i<max_lt_flt_ix; i++)
  	{
		k = MRAM_LT_Flts_Dat_Offset + 4 + (i * sizeof(Fault_type));
		wr_mram_array(MRAM_LT_Flts_Dat_Page,k,(int16 *)(&cf.fltw[0]),(int16)(sizeof(Fault_type))/2); 
    }
	Write_MRAM_Command(MRWRDI,0);
}

// *******************************************
// Init Long Term faults Index
// *******************************************

void init_LT_flts_ix(void)
{
	int16 k;
	
	k = MRAM_LT_Flts_Dat_Offset;
    if(rd_mram(MRAM_LT_Flts_Dat_Page,k) != 0x00AE)
    {
		clr_LT_flts();
    }
	k = MRAM_LT_Flts_Dat_Offset+2;		  // Skip initialization flag byte
	LT_Flt_ix = rd_mram(MRAM_LT_Flts_Dat_Page,k);
	if (LT_Flt_ix >= max_lt_flt_ix)		// make sure it is in a valid range
		LT_Flt_ix = 0;
}


// *******************************************
// This is read Long Term faults
// *******************************************
void rd_LT_flts(int16 LT_Flt_ix, uint16 *f)
{
  	int16 j,k;

	k = MRAM_LT_Flts_Dat_Offset + 4 + (LT_Flt_ix * sizeof(Fault_type));
	rd_mram_array(MRAM_LT_Flts_Dat_Page,k,(int16 *)(f),(int16)(sizeof(Fault_type))/2); 
  	return;
}


// *******************************************
// This is read in field adjustable variables
// *******************************************
#if (GALaxy_4 == 1)
/*
void rd_spb_fvar(void)
{
  	int16 i,k;
	uint16 fvars_fault = 0;

	if(rd_mram(MRAM_SPB_FVARS_Dat_Page,(uint16)MRAM_SPB_FVARS_Dat_Offset) != 0x0097)
  	{
		fvars_fault = 1;
		reset_spb_fvars();
	}
	else
	{		 // eeprom code matches
		k = (uint16)(MRAM_SPB_FVARS_Dat_Offset+2);	   // Skip initialization flag byte
		rd_mram_array(MRAM_SPB_FVARS_Dat_Page,k,spb_fvars,numspbvars); 
	}
}
*/
// *******************************************
// This is read in field adjustable variables
// *******************************************
void rd_nts_fvar(void)
{
  	int16 i,k;
	uint16 fvars_fault = 0;

	if(rd_mram(MRAM_NTS_FVARS_Dat_Page,(uint16)MRAM_NTS_FVARS_Dat_Offset) != 0x0071)
  	{
		fvars_fault = 1;
		reset_nts_fvars();
	}
	else
	{		 // eeprom code matches
		k = (uint16)(MRAM_NTS_FVARS_Dat_Offset+2);	   // Skip initialization flag byte
		rd_mram_array(MRAM_NTS_FVARS_Dat_Page,k,nts_fvars,numntsvars); 
	}		
}
#endif

// *******************************************
// This is read in field adjustable variables
// *******************************************
void rdfvar(void)
{
  	int16 i,j,k;
	int16 tmppos;
	uint16 fvars_fault = 0;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
	uint8 mram_status;
	
#define b_fvars_init 1
#define b_user_init 2
#define b_lw_init 4

	mram_status = Read_MRAM_SR (0);

	
  // updating battery back ram FLIGHT data
    if(rd_mram(MRAM_Flight_Dat_Page,MRAM_Flight_Dat_Offset) != 0x0047)
    {
		init_flight_times(0);
    }

	// Read the data from ram
	k = MRAM_Flight_Dat_Offset + 2;
	rd_mram_array(MRAM_Flight_Dat_Page,k,(int16 *)flight_times,(fl_size * fl_size));

  	// updating control and fire data
    if (rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset) != 0x0092)
    {
		Write_MRAM_Command(MRWREN,0);
		//initialize fire flag ram parameters
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset,0x0092);	// init code
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+2,0x0);		// fire flag = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+4,0x01);	// position = 1
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+6,0x0);		// FS_DOB_DIS = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+8,0x0);		// fs2_to_fs1 = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+10,0x0); 	// rfire flag = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+12,0x0); 	// gripper_fault = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+14,0x0);		// fs1_effect = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+16,0x0);		// fire_sensor = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+18,0x1);		// fire_floor = 1
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+20,0x0);	// earthquake flag = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+22,0x0);	// above counterweight flag = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+24,0x0);	// Blink_FSL = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+26,0x0);	// DPP_Count low word
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+28,0x0);	// DPP_Count high word
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+30,0x0);	// stallf flag = 0
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+32,0x0);	// Encoder Base Count low word
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+34,0x0);	// Encoder Base Count high word
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+36,0x0);	// Second Pos
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+38,0x0);	// Selector Offset Count low word
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+40,0x0);	// Selector Offset Count high word
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+42,0x0);	// Hot Oil flag
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+44,0x0);	// encoder can default initialization
 		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+46,0x0);    // Power up Auto Trace Flag
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+48,0x0);    // Drive line to line voltage
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+50,0x0);    // Drive motor torque limit
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+52,0x0);    // Drive regen torque limit
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+54,40);    // LCD Contrast
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+56,8);    // LCD Brightness
 		Write_MRAM_Command(MRWRDI,0);
 	}

    firef = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+2);
	tmppos = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+4);
	if ((tmppos > 0) && (tmppos <= cons[topf]))
    	position = tmppos; 
    FS_DOB_DIS = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+6);
    fs2_to_fs1 = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+8);
    rfiref = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+10);
    gripper_fault = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+12);
    fs1_effect = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+14);
    fire_sensor = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+16);
    fire_floor = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+18);
    eqf = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+20);
    above_cwt = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+22);
    Blink_FSL = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+24);
#if (Traction == 1)
    uiu.w[lw] = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+26);
    uiu.w[hw] = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+28);
	DPP_Count = uiu.dw;
	if(cons[sel_type] != 3)
	{
		preset_encoder_count(DPP_Count);
	}
	Prev_DPP_Count = DPP_Count;
	prev_Enc_Count = DPP_Count;
#endif

    stallf = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+30);

#if (Traction == 1)
	if(cons[sel_type] == 3)
	{
    	uiu.w[lw] = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+32);
	    uiu.w[hw] = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+34);
		Enc_Base_Count = uiu.dw;
	}
#endif

    scnd_pos = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+36);
	prev_scnd_pos = scnd_pos;

#if (Traction == 1)
	if(cons[sel_type] == 1)
	{
    	uiu.w[lw] = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+38);
	    uiu.w[hw] = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+40);
		SEL_Offset = uiu.dw;
	}
#endif

    hotoilf = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+42);
	old_hotoilf = hotoilf;
	
#if (Traction == 1)
	can_enc_statusf = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+44);
#endif

	trace_auto_start = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+46);
	old_trace_auto_start = trace_auto_start;
	
#if ((Traction == 0) && (Tract_OL == 0))
	if (stallf != 0)
		stallf = 1;			// go through stall process
#else
	stallf = 0;
#endif

	stallf = 0;
	old_stallf = stallf;
  	old_firef = firef;
  	old_rfiref = rfiref;
  	old_fs2_to_fs1 = fs2_to_fs1;


	contrast = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+54);
	brightness = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+56);

	// Read job statistics
	
    if(rd_mram(MRAM_Stats_Dat_Page,MRAM_Stats_Dat_Offset) != 0x0079)
    {
		Write_MRAM_Command(MRWREN,0);
		wr_mram(MRAM_Stats_Dat_Page,MRAM_Stats_Dat_Offset,0x0079);
		for(i=0; i<nmblvar; i++)
		{
			lvars[i] = 0;
		}
		k = MRAM_Stats_Dat_Offset+2;
		wr_mram_array(MRAM_Stats_Dat_Page,k,(int16 *)lvars,(nmblvar * 2)); 
		Write_MRAM_Command(MRWRDI,0);
    }

	k = MRAM_Stats_Dat_Offset+2;
	rd_mram_array(MRAM_Stats_Dat_Page,k,(int16 *)lvars,(nmblvar * 2)); 
	
	// set default variable that depend on job information

	set_job_default_fvars();
// not yet reset from 354


	// Read the field variables
	if(rd_mram(MRAM_FVARS_Dat_Page,MRAM_FVARS_Dat_Offset) != 0x0085)
  	{
		fvars_fault = b_fvars_init;
		reset_fvars();
	}
	else 
	{		 // eeprom code matches
		read_fvars();
		compare_fvars_chksum();
		i = verify_fvars(1);
	 	if (i != 0)
	 	{
	 		fault_device = i;
	 		fault_device_2 = var_num;
	 		record_fault(f_fvars_backup);
	 	}
	}
	
	if(rd_mram(MRAM_User_Dat_Page,MRAM_User_Dat_Offset) != 0x0025)
  	{
  		fvars_fault = b_user_init;
  		reset_uvars();
	}
	else
	{		 // eeprom code matches
	
		k = MRAM_User_Dat_Offset+2;   // Skip initialization flag byte
		rd_mram_array(MRAM_User_Dat_Page,k,tdata,19); 
	    // reading in the telephone number
	    i=0;
	    while(i < 18)
	    {
			telnmb[i] = (int8)tdata[i];
			
			if ((telnmb[i] < '0') || (telnmb[i] > '9'))
			{
				if ((telnmb[i] != CR) && (telnmb[i] != '.') && (telnmb[i] != '-'))
					telnmb[i] = CR;
			}	
			i++;
	    }
	    telnmb[i] = (int8)tdata[i];
		if (telnmb[i] != CR)
			telnmb[i] = CR;
		
		// setup the page register
		k = MRAM_PassW_Dat_Offset;  
		rd_mram_array(MRAM_User_Dat_Page,k,tdata,228); 		// 228 bytes (6 * 19 * 2)
		k=0;
	  	for(j=2; j<=7; j++)
	  	{
			i=0;
			while((tdata[i] != CR) && (i < 18))      // reading in user defineable passwords
			{
			    pass[j][i] = (int8)tdata[k];
			    i++;
			    k++;
			}
			pass[j][i] = CR;
			k++;
			i=0;
			while((tdata[i] != CR) && (i <18))       // reading in user defineable user names
			{
			    user[j][i] = (int8)tdata[k];
			    i++;
			    k++;
			}
			user[j][i] = CR;
			k++;
	  	}
	}

// comm_baud[0..1] still 354, 354


	if(rd_mram(MRAM_LW_Dat_Page,MRAM_LW_Dat_Offset) != 0x0096)
	{
		fvars_fault = b_lw_init;
		reset_lwvars();
	}
	else
	{		 // eeprom code matches
		// load weigher variables
		k = MRAM_LW_Dat_Offset+2;   // Skip initialization flag byte
		lw_status = (int8)rd_mram(MRAM_LW_Dat_Page,k);
		k+=2;
		rd_mram_array(MRAM_LW_Dat_Page,k,&lw_empty[1],cons[grtopf]); 
		k+=cons[grtopf];
		rd_mram_array(MRAM_LW_Dat_Page,k,&lw_full[1],cons[grtopf]); 

	}

	if (verify_lw_data() == 0)		// verify error
	{
		lw_status |= c_lw_error;		// device has error
		lw_status &= ~c_lw_on;			// load weigher is turned off
		lw_status &= ~c_lw_setup;		// error so clear setup flag
		record_fault(f_lw_load_table);
	}
	
// comm_baud[0..1] still 354, 354
    	
	read_trace_vars();
	
	set_job_min_max_fvars();
    // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15 debug15

// comm_baud[0..1] still 354, 354 but is about to be set to 2832, 354 
       
  	comm_baud[0] = baud_tbl[fvars[fvbaud]];
  	comm_baud[1] = baud_tbl[fvars[fvbaud2]];
// "now set comm_baud[0] back to 354"  	
 	fvars[fvbaud] = 3; // wbg
 	comm_baud[0] = baud_tbl[fvars[fvbaud]]; // wbg
  	
#if ((Traction == 1) || (Tract_OL == 1))
	if ((Drive_Type[cons[carnmb]] == 12) && (fvars[fvkebdpytype] == 1))	// keb with new keypad
		drv_baud = keb_drv_baud_tbl[fvars[fvdrvbaud]];
	else
		drv_baud = drv_baud_tbl[fvars[fvdrvbaud]];
	drv_modbus = drv_modbus_tbl[fvars[fvmodbus]];
#endif

//	sx_com_baud[0] = sx_baud_tbl[fvars[fvupsbaud]];
//	sx_com_baud[1] = sx2_baud_tbl[fvars[fv485baud]];

// comm_baud[0..1] now contains 2832, 354


#if (Traction == 1)
	can_enc_ppr = fvars[fvencppr];
	can_enc_dir = fvars[fvencdir];
#endif

	calculate_grp_fvars_chksum();
	calculate_button_color(1);

#if (Traction == 1)
	calculate_profile();
#else
	calculate_hoistway();
#endif
#if ((Traction == 1) || (Tract_OL == 1))
	calculate_brake();
	calculate_em_brake();
#endif
	if (fvars_fault != 0)
	{
		if ((fvars_fault & b_fvars_init) != 0)
			record_fault(f_fvars_init);
		if ((fvars_fault & b_user_init) != 0)
			record_fault(f_user_init);
		if ((fvars_fault & b_lw_init) != 0)
			record_fault(f_lw_init);
	}
	
	for(i=0;i<nmbsvct;i++)
		Read_Grp_Svc_Timer(i);


	if(((cons[sercom] & 0x20) != 0) && (fvars[fvxasgn] != 0))
	{
		cons[sercom] = cons[sercom] - 32;//eliminate the serial hall call for now
	}
	
#if (GALaxy_4 == 1)
//	rd_spb_fvar();
	rd_nts_fvar();
#endif

	read_spare_ios();	
}

//**************************************************
// Read field variables from memory
//**************************************************
void read_fvars (void)
{
  	int16 i,k;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union

	k = MRAM_FVARS_Dat_Offset+2;	   // Skip initialization flag byte
	rd_mram_array(MRAM_FVARS_Dat_Page,k,fvars,600); 
/*    for(i=0; i<600; i++)	// read in variables from ram disk	(including added ones)
    {
		fvars[i] = rd_mram(MRAM_FVARS_Dat_Page,k);
		k+=2;
    }
*/		
	k = MRAM_Speed_Clamp_Dat_Offset;
	rd_mram_array(MRAM_FVARS_Dat_Page,k,speed_clamp,9); 

	if (speed_clamp[0] > 140)
		speed_clamp[0] = 140;
	for (i=1;i<=8;i++)
	{
		if (speed_clamp[i] > cons[speed])
			speed_clamp[i] = cons[speed];
	}

	// Prevent clamping when limit not used
	if ((cons[speed] <= 200) && (cons[rsbuffer] != 1))
		speed_clamp[5] = cons[speed];
	if (cons[speed] <= 250)
		speed_clamp[2] = cons[speed];
	if (cons[speed] <= 500)
		speed_clamp[3] = cons[speed];
	if (cons[speed] <= 700)
		speed_clamp[4] = cons[speed];
	if (cons[speed] <= 900)
		speed_clamp[6] = cons[speed];
	if (cons[speed] <= 1100)
		speed_clamp[7] = cons[speed];
	if (cons[speed] <= 1300)
		speed_clamp[8] = cons[speed];

	k = MRAM_Sec_CC_Dat_Offset;
	rd_mram_array(MRAM_FVARS_Dat_Page,k,(int16 *)cc_sec_mask,4); 


	k = MRAM_Sec_CC_Dat_Offset + 8;
	rd_mram_array(MRAM_FVARS_Dat_Page,k,(int16 *)rcc_sec_mask,4); 
	
}


//************************************************//
// Backup the field variables in a seperate table 
//************************************************//

void backup_fvars(void)
{
	int16 i;
	int16 k;
	int16 chksum = 0;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
	Write_MRAM_Command(MRWREN,0);
	k = (uint16)MRAM_FVARS_Backup_Dat_Offset;
	wr_mram(MRAM_FVARS_Backup_Dat_Page,k,(int16)0x0099);    
	k+=2;
	wr_mram_array(MRAM_FVARS_Backup_Dat_Page,k,fvars,600); 
    for(i=0; i<600; i++)
    {
		chksum += fvars[i];
    }
    
	k = (uint16)MRAM_Speed_Clamp_Backup_Dat_Offset;
	wr_mram_array(MRAM_FVARS_Backup_Dat_Page,k,speed_clamp,9); 
    for(i=0; i<=8; i++)
    {
		chksum += speed_clamp[i];
    }

	k = (uint16)MRAM_Sec_CC_Backup_Dat_Offset;
	wr_mram_array(MRAM_FVARS_Backup_Dat_Page,k,(int16 *)cc_sec_mask,4); 
	uiu.dw = cc_sec_mask[0];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = cc_sec_mask[1];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	

	k = (uint16)(MRAM_Sec_CC_Backup_Dat_Offset + 8);
	wr_mram_array(MRAM_FVARS_Backup_Dat_Page,k,(int16 *)rcc_sec_mask,4); 
	uiu.dw = rcc_sec_mask[0];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = rcc_sec_mask[1];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	
	write_fvars_chksum(chksum,1);
	Write_MRAM_Command(MRWRDI,0);

}

//**************************************************
// Read Baackup field variables from memory
//**************************************************
void read_backup_fvars (void)
{
  	int16 i,k;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union

	k = (uint16)(MRAM_FVARS_Backup_Dat_Offset+2);	   // Skip initialization flag byte
	rd_mram_array(MRAM_FVARS_Backup_Dat_Page,k,fvars,600); 
		
	k = (uint16)MRAM_Speed_Clamp_Backup_Dat_Offset;
	rd_mram_array(MRAM_FVARS_Backup_Dat_Page,k,speed_clamp,9); 

	if (speed_clamp[0] > 140)
		speed_clamp[0] = 140;
	for (i=1;i<=8;i++)
	{
		if (speed_clamp[i] > cons[speed])
			speed_clamp[i] = cons[speed];
	}

	// Prevent clamping when limit not used
	if ((cons[speed] <= 200) && (cons[rsbuffer] != 1))
		speed_clamp[5] = cons[speed];
	if (cons[speed] <= 250)
		speed_clamp[2] = cons[speed];
	if (cons[speed] <= 500)
		speed_clamp[3] = cons[speed];
	if (cons[speed] <= 700)
		speed_clamp[4] = cons[speed];
	if (cons[speed] <= 900)
		speed_clamp[6] = cons[speed];
	if (cons[speed] <= 1100)
		speed_clamp[7] = cons[speed];
	if (cons[speed] <= 1300)
		speed_clamp[8] = cons[speed];

	k = (uint16)MRAM_Sec_CC_Backup_Dat_Offset;
	rd_mram_array(MRAM_FVARS_Backup_Dat_Page,k,(int16 *)cc_sec_mask,4); 

	k = (uint16)(MRAM_Sec_CC_Backup_Dat_Offset + 8);
	rd_mram_array(MRAM_FVARS_Backup_Dat_Page,k,(int16 *)rcc_sec_mask,4); 

}

//***********************************************//
// Compare fvars checksums
//***********************************************//

void compare_fvars_chksum(void)
{
	int16 chksum;
	int16 chksum1;
	int16 chksum_mem;
	int16 chksum1_mem;
	
	chksum = chksum_fvars(0);
	chksum_mem = get_fvars_chksum(0);
	chksum1 = chksum_fvars(1);
	chksum1_mem = get_fvars_chksum(1);
	if (chksum_mem != chksum)
	{
		if(rd_mram(MRAM_FVARS_Backup_Dat_Page,(uint16)MRAM_FVARS_Backup_Dat_Offset) == 0x0099)
		{
	 		if (chksum1_mem == chksum1)
	 		{
				record_fault(f_fvars_chksum);
				read_backup_fvars();
				wrfvar();
	 		}
	 		else
			{
				record_fault(f_fvars_chksum_both);
				wrfvar();		// rely on the fvars and correct the checksum 
			}
		}
		else
		{
			record_fault(f_fvars_chksum_both);
			wrfvar();		// rely on the fvars and correct the checksum 
		}
	}
	else
	{
		if(rd_mram(MRAM_FVARS_Backup_Dat_Page,(uint16)MRAM_FVARS_Backup_Dat_Offset) != 0x0099)
		{
			record_fault(f_fvars_backkup_init);
			backup_fvars();
		}
		else if (chksum1_mem != chksum)
		{
			record_fault(f_fvars_chksum_bkup);
			backup_fvars();
		}
	}
		
}



//*********************************************//
// Calculate checksum for fvars         
//   Table = 0; Main fvars table
//   Table = 1; Backup fvars table    
//*********************************************//

int16 chksum_fvars(int16 Table)
{	
	int16 chksum = 0;
	int16 i,j,k;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union

	if (Table == 0)
 		j = MRAM_FVARS_Dat_Page;	   // Main variable table
	else
 		j = MRAM_FVARS_Backup_Dat_Page;	   // Main variable table

	if (Table == 0)
	 	k = MRAM_FVARS_Dat_Offset+2;	   // Skip initialization flag byte
	else
	 	k = (uint16)(MRAM_FVARS_Backup_Dat_Offset+2);	   // Skip initialization flag byte
	rd_mram_array(j,k,tdata,600); 
    for(i=0; i<600; i++)	// read in variables from ram disk	(including added ones)
    {
    	chksum += tdata[i];
    }
    
	if (Table == 0)
		k = MRAM_Speed_Clamp_Dat_Offset;
	else
		k = (uint16)MRAM_Speed_Clamp_Backup_Dat_Offset;
	
	rd_mram_array(j,k,tdata,9); 
	for(i=0;i<=8;i++)
	{
		chksum += tdata[i];
	}


	if (Table == 0)
		k = MRAM_Sec_CC_Dat_Offset;
	else
		k = (uint16)MRAM_Sec_CC_Backup_Dat_Offset;
	rd_mram_array(j,k,tdata,8); 
	for(i=0;i<8;i++)
	{
		chksum += tdata[i];
	}

	return chksum;
}


//*********************************************//
// Verify Field Variable data matches ram
//   Table = 0; Main fvars table
//   Table = 1; Backup fvars table    
//********************************************//

int16 verify_fvars(int16 Table)
{
  	int16 i,j,k;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union

	if (Table == 0)
 		j = MRAM_FVARS_Dat_Page;	   // Main variable table
	else
 		j = MRAM_FVARS_Backup_Dat_Page;	   // Main variable table

	if (Table == 0)
		k = MRAM_FVARS_Dat_Offset+2;	   // Skip initialization flag byte
	else
		k = (uint16)(MRAM_FVARS_Backup_Dat_Offset+2);	   // Skip initialization flag byte
	
	rd_mram_array(j,k,tdata,600);

    for(i=0; i<600; i++)	// read in variables from ram disk	(including added ones)
    {
		if (fvars[i] != tdata[i])
		{
			var_num = i;
			return 1;
		}
    }
		
	if (Table == 0)
		k = MRAM_Speed_Clamp_Dat_Offset;
	else
		k = (uint16)MRAM_Speed_Clamp_Backup_Dat_Offset;
	rd_mram_array(j,k,tdata,9);
	for(i=0;i<=8;i++)
	{
		if (speed_clamp[i] != tdata[i])
		{
			var_num = i;
			return 2;
		}
	}

	if (Table == 0)
		k = MRAM_Sec_CC_Dat_Offset;
	else
		k = (uint16)MRAM_Sec_CC_Backup_Dat_Offset;
	rd_mram_array(j,k,tdata,8);
	uiu.w[hw] = tdata[0];
	uiu.w[lw] = tdata[1];
	if (cc_sec_mask[0] != uiu.dw)
	{
		var_num = 0;
		return 3;
	}
	uiu.w[hw] = tdata[2];
	uiu.w[lw] = tdata[3];
	if (cc_sec_mask[1] != uiu.dw)
	{
		var_num = 0;
		return 4;
	}
	uiu.w[hw] = tdata[4];
	uiu.w[lw] = tdata[5];
	if (rcc_sec_mask[0] != uiu.dw)
	{
		var_num = 0;
		return 5;
	}
	uiu.w[hw] = tdata[6];
	uiu.w[lw] = tdata[7];
	if (rcc_sec_mask[1] != uiu.dw)
	{
		var_num = 0;
		return 6;
	}
	
	return 0;
}

//*******************************************
// Verify the load weigher data (return 0 if error)
//*******************************************
uint8 verify_lw_data (void)
{
	uint8 okay = 1;
	int16 i;
	
	for(i=1; i<=cons[grtopf]; i++)
	{
		if ( ((ccnmsk[cons[carnmb]][i] == 1) || (rccnmsk[cons[carnmb]][i] == 1)) && 
			(lw_empty[i] > lw_full[i]) )
			okay = 0;
	}
	
	return (okay);
}

//*******************************************
// Read Trace Variables from MRAM
//*******************************************

void read_trace_vars(void)
{
  	int16 k;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
	if(rd_mram(MRAM_Trace_Dat_Page,(uint16)MRAM_Trace_Dat_Offset) != 0x0038)
	{
		trigger_logic = 0;
		and_trigger_logic = 0;
		trigger_enable = 0;
		trigger_arm = 0;
		trigger_window = 0;
		trace_interval = 0;
		trigger_fault = 0;
		trigger_servf = 0;
		trigger_procf = 0;
		trigger_run_statusf = 0;
		trigger_front_sd = 0;
		trigger_rear_sd = 0;
		trigger_statusf = 0;
		trigger_statusf2 = 0;
		trigger_fault_bits[0] = 0;	
		trigger_fault_bits[1] = 0;	
		trigger_fault_bits[2] = 0;	
		trigger_fault_bits[3] = 0;	
		
		write_trace_vars();
	}
	else
	{
		k = (uint16)MRAM_Trace_Dat_Offset;
		k+=2;		// skip validation code
		uiu.w[lw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[hw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	trigger_logic = uiu.dw;
		uiu.w[lw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[hw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	and_trigger_logic = uiu.dw;
	 	uiu.w[lw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[hw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		trigger_enable = uiu.dw;
		trigger_arm = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		trigger_window = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	trace_interval = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	trigger_fault = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	trigger_servf = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	trigger_procf = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[lw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[hw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	trigger_run_statusf = uiu.dw;
	 	trigger_front_sd = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	trigger_rear_sd = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[lw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[hw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
	 	trigger_statusf = uiu.dw;
	 	uiu.w[lw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[hw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		trigger_statusf2 = uiu.dw;
	 	uiu.w[lw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		uiu.w[hw] = rd_mram(MRAM_Trace_Dat_Page,k);
		k+=2;
		trigger_fault_bits[0] = uiu.b[lb];	
		trigger_fault_bits[1] = uiu.b[mlb];	
		trigger_fault_bits[2] = uiu.b[mhb];	
		trigger_fault_bits[3] = uiu.b[hb];	
	}
}

//*******************************************
// Write Trace Variables to MRAM
//*******************************************

void write_trace_vars(void)
{
  	int16 k;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;		// unsigned integer union
	
		
	Write_MRAM_Command(MRWREN,0);
	k = (uint16)MRAM_Trace_Dat_Offset;
	wr_mram(MRAM_Trace_Dat_Page,k,0x0038);
	k+=2;
	uiu.dw = trigger_logic;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[lw]);	
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[hw]);
	k+=2;
	uiu.dw = and_trigger_logic;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[lw]);	
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[hw]);
	k+=2;
	uiu.dw = trigger_enable;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[lw]);	
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[hw]);
	k+=2;

	wr_mram(MRAM_Trace_Dat_Page,k,trigger_arm);	
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,trigger_window);
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,trace_interval);
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,trigger_fault);
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,trigger_servf);
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,trigger_procf);
	k+=2;
	
	uiu.dw = trigger_run_statusf;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[lw]);	
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[hw]);
	k+=2;

	wr_mram(MRAM_Trace_Dat_Page,k,trigger_front_sd);
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,trigger_rear_sd);
	k+=2;

	uiu.dw = trigger_statusf;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[lw]);	
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[hw]);
	k+=2;
	uiu.dw = trigger_statusf2;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[lw]);	
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[hw]);
	k+=2;
	
	uiu.b[lb] = trigger_fault_bits[0]; 
	uiu.b[mlb] = trigger_fault_bits[1];
	uiu.b[mhb] = trigger_fault_bits[2];
	uiu.b[hb] = trigger_fault_bits[3];
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[lw]);	
	k+=2;
	wr_mram(MRAM_Trace_Dat_Page,k,uiu.w[hw]);
	k+=2;
	Write_MRAM_Command(MRWRDI,0);
}

//*******************************************
// Read Spare IOS from MRAM
//*******************************************

void read_spare_ios(void)
{
	int i;
  	int16 k;
	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
	
	if(rd_mram(MRAM_IO_Dat_Page,(uint16)MRAM_IO_Dat_Offset) != 0x00D9)
	{
		for (i=0;i<18;i++)
		{
			spare_ioname[i][0] = SPARE_IOS[i][0];
			spare_ioname[i][1] = SPARE_IOS[i][1];
			spare_ioname[i][2] = SPARE_IOS[i][2];
			spare_ioname[i][3] = SPARE_IOS[i][3];
			spare_ioname[i][4] = SPARE_IOS[i][4];
			spare_io_stat[i] = 0;
		}
		
		write_spare_ios();
	}
	else
	{
		k = (uint16)MRAM_IO_Dat_Offset;
		k+=2;		// skip validation code
		for (i=0;i<18;i++)
		{
			uwu.w = rd_mram(MRAM_IO_Dat_Page,k);
			k+=2;
			spare_ioname[i][0] = uwu.b[0];
			spare_ioname[i][1] = uwu.b[1];
			uwu.w = rd_mram(MRAM_IO_Dat_Page,k);
			k+=2;
			spare_ioname[i][2] = uwu.b[0];
			spare_ioname[i][3] = uwu.b[1];
			uwu.w = rd_mram(MRAM_IO_Dat_Page,k);
			k+=2;
			spare_ioname[i][4] = uwu.b[0];
			spare_io_stat[i] = uwu.b[1];
		}
		
	}
}

//*******************************************
// Write Spare IOS to MRAM
//*******************************************

void write_spare_ios(void)
{
	int16 i;
  	int16 k;
	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
	
		
	Write_MRAM_Command(MRWREN,0);
	k = (uint16)MRAM_IO_Dat_Offset;
	wr_mram(MRAM_IO_Dat_Page,k,0x00D9);
	k+=2;
	for (i=0;i<18;i++)
	{
		uwu.b[0] = spare_ioname[i][0];
		uwu.b[1] = spare_ioname[i][1];
		wr_mram(MRAM_IO_Dat_Page,k,uwu.w);	
		k+=2;
		uwu.b[0] = spare_ioname[i][2];
		uwu.b[1] = spare_ioname[i][3];
		wr_mram(MRAM_IO_Dat_Page,k,uwu.w);	
		k+=2;
		uwu.b[0] = spare_ioname[i][4];
		uwu.b[1] = spare_io_stat[i];
		wr_mram(MRAM_IO_Dat_Page,k,uwu.w);	
		k+=2;
	}
	Write_MRAM_Command(MRWRDI,0);

}


#if (Traction == 1)
	
//*******************************************
// Save Drive data to MRAM
//*******************************************
void save_drive_to_MRAM(void)
{
	Write_MRAM_Command(MRWREN,0);
	if ((drv_line_voltage != 0) && (drv_line_voltage != 140))
	{
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+48,drv_line_voltage);
	}
	if ((drv_motor_torque != 0) && (drv_motor_torque != 1000))
	{
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+50,drv_motor_torque);
	}
	if ((drv_regen_torque != 0) && (drv_regen_torque != 1000))
	{
		wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+52,drv_regen_torque);
	}
	Write_MRAM_Command(MRWRDI,0);
}

//*******************************************
// Read Drive Data from MRAM
//*******************************************
void read_drive_from_MRAM(void)
{
	int16 temp_line_volt = 0;
	int16 temp_motor_torque = 0;
	int16 temp_regen_torque = 0;
	
	temp_line_volt = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+48);	
	temp_motor_torque = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+50);
	temp_regen_torque = rd_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+52);
	
	if(temp_line_volt != 0)
	{
		drv_line_voltage = temp_line_volt;
	}
	if(temp_motor_torque != 0)
	{
		drv_motor_torque = temp_motor_torque;
	}
	if(temp_regen_torque != 0)
	{
		drv_regen_torque = temp_regen_torque;
	}
}

#endif

//**************************************************
// Reset load weigher variables
//**************************************************

void reset_lwvars(void)
{
	int16 i;
	
	lw_status = 0;
	for(i=1; i<=cons[grtopf]; i++)
	{
		lw_empty[i] = 0;
		lw_full[i] = 0;
	}
	wrlwvar();
}

//**************************************************
// Reset user variables
//**************************************************

void reset_uvars(void)
{
	int16 i;
	
	telnmb[0] = '1';      // telephone number of the shop
	telnmb[1] = '2';
	telnmb[2] = '3';
	telnmb[3] = '4';
	telnmb[4] = '5';
	telnmb[5] = '6';
	telnmb[6] = '7';
	telnmb[7] = CR;
	telnmb[8] = '\n';
	for(i=2; i<=7; i++)         // clearing all user definable passwords
	{
	  	pass[i][0] = CR;
	  	user[i][0] = CR;
	}
	wruvar();
}

#if (GALaxy_4 == 1)

//**************************************************
// Reset SPB field variables
//**************************************************
/*
void reset_spb_fvars(void)
{

  	int16 i;
	for (i=0;i<numspbvars;i++)
	{
		spb_fvars[i] = spb_fvars_default[i];
	}

	wr_spb_fvar();

}
*/
//**************************************************
// Reset NTS field variables
//**************************************************
void reset_nts_fvars(void)
{

  	int16 i;
	for (i=0;i<numntsvars;i++)
	{
		nts_fvars[i] = nts_fvars_default[i];
	}

	wr_nts_fvar();

}

#endif

//**************************************************
// Reset field variables
//**************************************************

void reset_fvars(void)
{

  	int16 i;
	for (i=0;i<nmbfvar;i++)
	{
		fvars[i] = fvars_default[i];
	}

	speed_clamp[0] = 140;
	for(i=1; i<=8; i++)
		speed_clamp[i] = cons[speed];

	cc_sec_mask[0] = 0;
	cc_sec_mask[1] = 0;
	rcc_sec_mask[0] = 0;
	rcc_sec_mask[1] = 0;

	wrfvar();

}

//**************************************************
// Reset Hoistway Init
//**************************************************

#if (Simulator == 1)
void Reset_Hoistway_Init (void)
{
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Hoist_Dat_Page,MRAM_Hoist_Dat_Offset,0x0000);	// Clear initialization flag
    wr_mram(MRAM_Fl_Update_Adjust_Page,MRAM_Fl_Update_Adjust_Offset,0x0000);	// Initialization flag byte
	Write_MRAM_Command(MRWRDI,0);
}
#endif


//**************************************************
// Reset Service Timers
//**************************************************

void Reset_Service_Timers (void)
{
	int16 i,j;
	
	for (i=0;i<nmbsvct;i++)
	{
		svc_t[i].service = 0;
		svc_t[i].status = 0;
		svc_t[i].logic = 0;
		svc_t[i].option = 0;
		for(j=0;j<=2;j++)
		{
			svc_t[i].month[j] = 0;
			svc_t[i].day[j] = 0;
			svc_t[i].md_on_hour[j] = 0;
			svc_t[i].md_off_hour[j] = 0;
			svc_t[i].md_on_minute[j] = 0;
			svc_t[i].md_off_minute[j] = 0;
		}
		for(j=0;j<=6;j++)
		{
			svc_t[i].on_hour[j] = 0;
			svc_t[i].off_hour[j] = 0;
			svc_t[i].on_minute[j] = 0;
			svc_t[i].off_minute[j] = 0;
		}
	}
	for (i = 0; i < nmbsvct; i++)
		Wrt_Grp_Svc_Timer(i);
}

//**********************************
//  Read Group Service Timers
//**********************************

void Read_Grp_Svc_Timer (int16 tmrnmb)
{
	void* vptr;
	int16 i, k;
	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union

	if (tmrnmb >= nmbsvct)
		return;

  	// setup the page register
	k = MRAM_Group_Timer_Offset + (tmrnmb * (sizeof(svc_timer_size) + 2));
    if(rd_mram(MRAM_Group_Timer_Page,k) != 0x0039)
    {
		svc_t[tmrnmb].service = 0;
		svc_t[tmrnmb].status = 0;
		svc_t[tmrnmb].logic = 0;
		svc_t[tmrnmb].option = 0;
		for (i=0;i<=2;i++)
		{
			svc_t[tmrnmb].month[i] = 0;
			svc_t[tmrnmb].day[i] = 0;
		  	svc_t[tmrnmb].md_on_hour[i] = 0;
		  	svc_t[tmrnmb].md_off_hour[i] = 0;
		  	svc_t[tmrnmb].md_on_minute[i] = 0;
		  	svc_t[tmrnmb].md_off_minute[i] = 0;
		}
		for (i=0;i<=6;i++)
		{
		  	svc_t[tmrnmb].on_hour[i] = 0;
		  	svc_t[tmrnmb].off_hour[i] = 0;
		  	svc_t[tmrnmb].on_minute[i] = 0;
		  	svc_t[tmrnmb].off_minute[i] = 0;
		}
		Wrt_Grp_Svc_Timer(tmrnmb);
  	}
    
	k+=2;
	vptr = &svc_t[tmrnmb];
	rd_mram_array(MRAM_Group_Timer_Page,k,(int16 *)(vptr),(int16)(sizeof(svc_timer_size))/2); 
	calculate_srv_tmr_chksum(tmrnmb);
}


//*****************************************
// Set SPB specific min, max field variables 
//***************************************** 
/*
void set_spb_min_max_fvars(void)
{
	int16 i;

	// make sure all variable are within a valid range
  	for(i=0; i<numspbvars; i++)
  	{		// verify min and max values for all parameters
		if ((spb_fvars[i] < spb_fvars_min[i]) || (spb_fvars[i] > spb_fvars_max[i]))
			spb_fvars[i] = spb_fvars_default[i];		// set to default if out of range
  	}
}
*/
//*****************************************
// Set NTS specific min, max field variables 
//***************************************** 
void set_nts_min_max_fvars(void)
{
	int16 i;

	// make sure all variable are within a valid range
  	for(i=0; i<numntsvars; i++)
  	{		// verify min and max values for all parameters
		if ((nts_fvars[i] < nts_fvars_min[i]) || (nts_fvars[i] > nts_fvars_max[i]))
			nts_fvars[i] = nts_fvars_default[i];		// set to default if out of range
  	}
}


//*****************************************
// Set job specific default field variables 
//***************************************** 
void set_job_default_fvars(void)
{
	fvars_default[fvbaud] = baud_default;		// past from pwrup routine
	fvars_default[fvacctopfl] = cons[topf];
	fvars_default[fvaccbotfl] = cons[bottomf];
}

//*****************************************
// Set one job specific min, max field variables 
//***************************************** 
void set_job_min_max_one_fvars(int16 var_offset)
{

	set_job_default_fvars();

	// make sure all variable are within a valid range
	if (fvars_max[var_offset] == -1)
		fvars_max[var_offset] = cons[nmbcars];
	else if (fvars_max[var_offset] == -2)
		fvars_max[var_offset] = cons[grtopf];
	else if (fvars_max[var_offset] == -3)
		fvars_max[var_offset] = cons[speed];
	
	if (fvars_min[var_offset] == -7)
		fvars_min[var_offset] = cons[carnmb];
	else if (fvars_min[var_offset] == -4)
		fvars_min[var_offset] = cons[bottomf];

	if ((fvars[var_offset] < fvars_min[var_offset]) || (fvars[var_offset] > fvars_max[var_offset]))
		fvars[var_offset] = fvars_default[var_offset];		// set to default if out of range

	if (var_offset == fvenctype)
	{
		
		if (fvars[fvenctype] == 1)			// Turck
		{
			if ((fvars[fvencnode] < 53) || (fvars[fvencnode] > 73))
				fvars[fvencnode] = 63;			// Node id
		}
		else if (fvars[fvenctype] == 2)		// Dynapar
		{
			if (fvars[fvencnode] > 21)
				fvars[fvencnode] = 1;			// Node id
		}
		else if (fvars[fvenctype] == 3)		// Wachendorff
		{
			if (fvars[fvencnode] < 107)
				fvars[fvencnode] = 127;			// Node id
		}
	}
	
	
	if ((var_offset == fvrecvel) && (fvars[fvrecvel] > cons[speed]))
		fvars[fvrecvel] = cons[speed];
	if ((var_offset == fvts_ep) && (fvars[fvts_ep] > cons[speed]))
		fvars[fvts_ep] = cons[speed];
	if ((var_offset == fvtopspeed) && ((fvars[fvtopspeed] > cons[speed]) || (fvars[fvtopspeed] == 0)))
		fvars[fvtopspeed] = cons[speed];

}

//*****************************************
// Set job specific min, max field variables 
//***************************************** 
void set_job_min_max_fvars(void)
{
	int16 i;

	set_job_default_fvars();

	// make sure all variable are within a valid range
  	for(i=0; i<nmbfvar; i++)
  	{		// verify min and max values for all parameters
		if (fvars_max[i] == -1)
			fvars_max[i] = cons[nmbcars];
		else if (fvars_max[i] == -2)
			fvars_max[i] = cons[grtopf];
		else if (fvars_max[i] == -3)
			fvars_max[i] = cons[speed];

		if (fvars_min[i] == -7)
			fvars_min[i] = cons[carnmb];
		else if (fvars_min[i] == -4)
			fvars_min[i] = cons[bottomf];

		if ((fvars[i] < fvars_min[i]) || (fvars[i] > fvars_max[i]))
			fvars[i] = fvars_default[i];		// set to default if out of range
  	}

	if (fvars[fvenctype] == 1)			// Turck
	{
		if ((fvars[fvencnode] < 53) || (fvars[fvencnode] > 73))
			fvars[fvencnode] = 63;			// Node id
	}
	else if (fvars[fvenctype] == 2)		// Dynapar
	{
		if (fvars[fvencnode] > 21)
			fvars[fvencnode] = 1;			// Node id
	}
	else if (fvars[fvenctype] == 3)		// Wachendorff
	{
		if (fvars[fvencnode] < 107)
			fvars[fvencnode] = 127;			// Node id
	}
	
	if (fvars[fvrecvel] > cons[speed])
		fvars[fvrecvel] = cons[speed];
	if (fvars[fvts_ep] > cons[speed])
		fvars[fvts_ep] = cons[speed];
	if ((fvars[fvtopspeed] > cons[speed]) || (fvars[fvtopspeed] == 0))
		fvars[fvtopspeed] = cons[speed];

}

// *******************************************************
// Test the system hardware  -- this can later be expanded
// *******************************************************

void test_hardware (void)
{
	int16 fault;

	fault = test_MRAM_present(1);
	if (fault != 0)
	{
		record_fault(fault);
		hardware_init_fault = 1;
	}
}

// *****************************************
// Test MRam for valid memory
// *****************************************
enum
{
    chip_0,
    chip_1,
    chip_2 
};

int16 test_MRAM_present(int16 WhichChip)
{
  switch(WhichChip)
  {
    case chip_1:
    	if (mem_test(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset) != 0)
    		return f_MRAM_fault;
    	if (mem_test(MRAM_FVARS_Dat_Page,MRAM_FVARS_Dat_Offset) != 0)
    		return f_MRAM_fault;
    	if (mem_test(MRAM_Hoist_Dat_Page,MRAM_Hoist_Dat_Offset) != 0)
    		return f_MRAM_fault;

    	if (mem_test(MRAM_Fl_Update_Adjust_Page,MRAM_Fl_Update_Adjust_Offset) != 0)
    		return f_MRAM_fault;

    	if (mem_test(MRAM_Flight_Dat_Page,MRAM_Flight_Dat_Offset) != 0)
    		return f_MRAM_fault;
    	if (mem_test(MRAM_MON_Dat_Page,MRAM_MON_Dat_Offset) != 0)
    		return f_MRAM_fault;
    	if (mem_test(MRAM_WED_Dat_Page,MRAM_WED_Dat_Offset) != 0)
    		return f_MRAM_fault;
    	if (mem_test(MRAM_FRI_Dat_Page,MRAM_FRI_Dat_Offset) != 0)
    		return f_MRAM_fault;

    	if (test_pages(0,1) != 0)
    	{
    		New_write_display("MRAM1_Page:0,1 FAIL ",3,0);
    		return f_MRAM_fault;
    	}
    		
    	if (test_pages(1,2) != 0)
    	{
    		New_write_display("MRAM1_Page:1,2 FAIL ",3,0);
    		return f_MRAM_fault;
    	}
    		
    	if (test_pages(2,3) != 0)
    	{
    		New_write_display("MRAM1_Page:2,3 FAIL ",3,0);
    		return f_MRAM_fault;
    	}
    		
    	if (test_pages(1,3) != 0)
    	{
    	  New_write_display("MRAM1_Page:1,3 FAIL ",3,0);
    	  return f_MRAM_fault;	  	    
    	}    
        break;
        
    case chip_2:
    //  Rev 'E' board only has one MRAM chip (wbg - 2017-10-23)	
    //  Consequently, it will fail the following tests.

    	if (test_pages(3,4) != 0)
    	{
    	  New_write_display("MRAM2_Page:3,4 FAIL ",3,0);
    	  return f_MRAM_fault;  
    	}

    	if (test_pages(4,5) != 0)
    	{
    	  New_write_display("MRAM2_Page:4,5 FAIL ",3,0);
    	  return f_MRAM_fault; 	    
    	}

    	if (test_pages(5,6) != 0)
    	{
    	  New_write_display("MRAM2_Page:5,6 FAIL ",3,0);
    	  return f_MRAM_fault; 	    
    	}

    	if (test_pages(0,5) != 0)
    	{
    	  New_write_display("MRAM2_Page:0,5 FAIL ",3,0);
    	  return f_MRAM_fault; 	    
    	}

    	if (test_pages(1,4) != 0)
    	{
    	  New_write_display("MRAM2_Page:1,4 FAIL ",3,0);
    	  return f_MRAM_fault; 	    
    	}
    
        break;
        
    default:
        return f_MRAM_fault;
        break;
  }	
	return 0;	
}

int16 test_MRAM_present_chip2(void)
{

//  Rev 'E' board only has one MRAM chip (wbg - 2017-10-23)	
//  Consequently, it will fail the following tests.

	if (test_pages(3,4) != 0)
	{
	  New_write_display("3,4 failed          ",3,0);
	  return f_MRAM_fault;  
	}

	if (test_pages(4,5) != 0)
	{
	  New_write_display("4,5 failed          ",3,0);
	  return f_MRAM_fault; 	    
	}

	if (test_pages(5,6) != 0)
	{
	  New_write_display("5,6 failed          ",3,0);
	  return f_MRAM_fault; 	    
	}

	if (test_pages(0,5) != 0)
	{
	  New_write_display("0,5 failed          ",3,0);
	  return f_MRAM_fault; 	    
	}

	if (test_pages(1,4) != 0)
	{
	  New_write_display("1,4 failed          ",3,0);
	  return f_MRAM_fault; 	    
	}	
	return 0;	
}


// ******************************************
// Test if write to memory wraps to next page
// ******************************************

int16 test_pages(uint16 Dat_Page1, uint16 Dat_Page2)
{
	int16 var1, var2;

	wr_mram_cmd(MRWREN,(uint8)Dat_Page1);
	var1 = rd_mram(Dat_Page1,0);
	wr_mram(Dat_Page1,0,0x00);	// set first page to zero

	wr_mram_cmd(MRWREN,(uint8)Dat_Page2);
	var2 = rd_mram(Dat_Page2,0);
	wr_mram(Dat_Page2,0,0x3751);	// write different value to second page
	wr_mram_cmd(MRWRDI,(uint8)Dat_Page1);
	wr_mram_cmd(MRWRDI,(uint8)Dat_Page2);

	if (rd_mram(Dat_Page2,0) != 0x3751)
		return 1;

	if (rd_mram(Dat_Page1,0) == 0x3751)		// Page wrap
		return 1;
	
	if (rd_mram(Dat_Page1,0) != 0x0000)		// Error
		return 1;

	wr_mram_cmd(MRWREN,(uint8)Dat_Page1);
	wr_mram(Dat_Page1,0,var1);	// restore first page

	wr_mram_cmd(MRWREN,(uint8)Dat_Page2);
	wr_mram(Dat_Page2,0,var2);	// restore second page

	wr_mram_cmd(MRWRDI,(uint8)Dat_Page1);
	wr_mram_cmd(MRWRDI,(uint8)Dat_Page2);
	return 0;
}

// *************************************************
// Test for read and write data retention to memory
// *************************************************

int16 mem_test(uint16 Dat_Page, uint16 Dat_Offset)
{
	uint16 var;

	wr_mram_cmd(MRWREN,(uint8)Dat_Page);
	var = rd_mram(Dat_Page,Dat_Offset);

	wr_mram(Dat_Page,Dat_Offset,0x2539);
	wr_mram_cmd(MRWRDI,(uint8)Dat_Page);
	if (rd_mram(Dat_Page,Dat_Offset) != 0x2539)
		return 1;

	wr_mram_cmd(MRWREN,(uint8)Dat_Page);
	wr_mram(Dat_Page,Dat_Offset,0x0000);
	wr_mram_cmd(MRWRDI,(uint8)Dat_Page);
	if (rd_mram(Dat_Page,Dat_Offset) != 0x0000)
		return 1;
	
	wr_mram_cmd(MRWREN,(uint8)Dat_Page);
	wr_mram(Dat_Page,Dat_Offset,var);
	wr_mram_cmd(MRWRDI,(uint8)Dat_Page);
	if (rd_mram(Dat_Page,Dat_Offset) != var)
		return 1;

	return 0;
} 


// ********************************************
// This is the save hall call data
// ********************************************
void Up_Date_Calls(void)
{
  	int16 i,j,k;

	if ((old_day_of_week > 6) || (old_day_of_week < 0))
		old_day_of_week = 0;

	Write_MRAM_Command(MRWREN,0);
//	k = MRAM_Day_Dat_Offset[old_day_of_week];
//	wr_mram(MRAM_Day_Dat_Page[old_day_of_week],k,0x0076);
//	k+=2;
//  for(i=0; i<=96; i++)
//  {
//		for(j=0; j<=fl_size; j++)
//		{
//			wr_mram(MRAM_Day_Dat_Page[old_day_of_week],k,0x0000);
//			k+=2;
//			wr_mram(MRAM_Day_Dat_Page[old_day_of_week],k,0x0000);
//			k+=2;
//		}
//  }

	// Store the previous calls for the day
	k = MRAM_Day_Dat_Offset[old_day_of_week];
	wr_mram(MRAM_Day_Dat_Page[old_day_of_week],k,0x0076);
	k+=2;
  	for(i=0; i<=96; i++)
  	{
//		for(j=0; j<=fl_size; j++)
//		{
//			wr_mram(MRAM_Day_Dat_Page[old_day_of_week],k,nmb_up_call[i][j]/2);
//			k+=2;
//			wr_mram(MRAM_Day_Dat_Page[old_day_of_week],k,nmb_dn_call[i][j]/2);
//			k+=2;
//		}
		wr_mram_array(MRAM_Day_Dat_Page[old_day_of_week],k,nmb_up_call[i],fl_size);
		k += (int16)(fl_size * 2);
		wr_mram_array(MRAM_Day_Dat_Page[old_day_of_week],k,nmb_dn_call[i],fl_size);
		k += (int16)(fl_size * 2);
  	}

	Write_MRAM_Command(MRWRDI,0);

	petdog();

	if ((day_of_week > 6) || (day_of_week < 0))
		day_of_week = 0;

	// Load the new day call data

	k = (int16)(MRAM_Day_Dat_Offset[day_of_week] + 2);
  	for(i=0; i<=96; i++)
  	{
//		for(j=0; j<=fl_size; j++)
//		{
//			nmb_up_call[i][j] = rd_mram(MRAM_Day_Dat_Page[day_of_week],k);
//			k+=2;
//			nmb_dn_call[i][j] = rd_mram(MRAM_Day_Dat_Page[day_of_week],k);;
//			k+=2;
//		}
		rd_mram_array(MRAM_Day_Dat_Page[day_of_week],k,nmb_up_call[i],fl_size);
		k += (int16)(fl_size * 2);
		rd_mram_array(MRAM_Day_Dat_Page[day_of_week],k,nmb_dn_call[i],fl_size);
		k += (int16)(fl_size * 2);
  	}
  	return;
}

//*******************************
// Update car control flags
//*******************************

void Update_Control_Flags(void)
{
	int16 k;
		
	Write_MRAM_Command(MRWREN,0);
	// setup the page register
	k = MRAM_Fire_Dat_Offset;

	// updating battery back ram fire data

	wr_mram(MRAM_Fire_Dat_Page,k+0,0x0092);	// control flag code number
	wr_mram(MRAM_Fire_Dat_Page,k+2,(firef & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+4,(position & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+6,(FS_DOB_DIS & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+8,(fs2_to_fs1 & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+10,(rfiref & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+12,(gripper_fault & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+14,(fs1_effect & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+16,(fire_sensor & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+18,(fire_floor & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+20,(eqf & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+22,(above_cwt & 0x00ff));
	wr_mram(MRAM_Fire_Dat_Page,k+24,(Blink_FSL & 0x00ff));

	if (stallf	!= 0)							
		wr_mram(MRAM_Fire_Dat_Page,k+30,1);				
	else
		wr_mram(MRAM_Fire_Dat_Page,k+30,0);				
	
	if (hotoilf != 0)
		wr_mram(MRAM_Fire_Dat_Page,k+42,1);
	else
		wr_mram(MRAM_Fire_Dat_Page,k+42,0);
	
	Write_MRAM_Command(MRWRDI,0);
}

//**********************************
// Update car flight Times routine
//**********************************

void Update_Flight_Times(void)
{
	int16 i,j,k;
	Write_MRAM_Command(MRWREN,0);
	k = MRAM_Flight_Dat_Offset;
	wr_mram(MRAM_Flight_Dat_Page,k,0x0047);
	k+=2;
	wr_mram_array(MRAM_Flight_Dat_Page,k,(int16 *)flight_times,(fl_size * fl_size));
	Write_MRAM_Command(MRWRDI,0);
}

//**********************************
// Update group job statistics
//**********************************

void Update_Job_Statistics (void)
{
  	int16 i,k;
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union

	Write_MRAM_Command(MRWREN,0);
	// Set data page register
	k = MRAM_Stats_Dat_Offset;
	wr_mram(MRAM_Stats_Dat_Page,k,0x0079);
	k+=2;
	wr_mram_array(MRAM_Stats_Dat_Page,k,(int16 *)lvars,(nmblvar * 2));
	Write_MRAM_Command(MRWRDI,0);
}

// *******************************************
// Update the car position
// *******************************************

void Update_Pos (void)
{
	// Update the position  
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+4,(position & 0xff));
	Write_MRAM_Command(MRWRDI,0);
}

// *******************************************
// Update the car position
// *******************************************

void Update_scnd_Pos (void)
{
	// Update the second position  
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+36,(scnd_pos & 0xff));
	Write_MRAM_Command(MRWRDI,0);
}

#if (Traction == 1)

// *******************************************
// Update Encoder Status Flag
// *******************************************

void Update_Enc_Status_Flag (void)
{
	// Update the second position  
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+44,(can_enc_statusf & 0xff));
	Write_MRAM_Command(MRWRDI,0);
}


 //*******************************************
 //Update Selector Offset Count
 //*******************************************

void Update_SEL_Offset_Count (void)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
	uiu.dw = SEL_Offset;
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+38,uiu.w[lw]);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+40,uiu.w[hw]);
	Write_MRAM_Command(MRWRDI,0);
}

#endif


 //*******************************************
 //Update the DPP Count
 //*******************************************

void Update_DPP_EE (void)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
	uiu.dw = DPP_Count;
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+26,uiu.w[lw]);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+28,uiu.w[hw]);
	Write_MRAM_Command(MRWRDI,0);
}

//*******************************************
//Update LCD Contrast and Brightness
//*******************************************

void Update_Contrast_Brightness (void)
{
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+54,contrast);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+56,brightness);
	Write_MRAM_Command(MRWRDI,0);
}

 //*******************************************
 //Update Encoder Base Count
 //*******************************************

void Update_Enc_Base_Count (void)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
	uiu.dw = Enc_Base_Count;
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+32,uiu.w[lw]);
	wr_mram(MRAM_Fire_Dat_Page,MRAM_Fire_Dat_Offset+34,uiu.w[hw]);
	Write_MRAM_Command(MRWRDI,0);
}


// ********************************************
// This is save the fault log
// ********************************************

void Update_Flt(void)
{
  	int16 i,j,k;

	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union


	Write_MRAM_Command(MRWREN,0);
	k = MRAM_Flts_Dat_Offset;
  	wr_mram(MRAM_Flts_Dat_Page,k,0x009E);
	k+=2;
  	wr_mram(MRAM_Flts_Dat_Page,k,Flt_ix);	// fault index

	k = MRAM_Flts_Dat_Offset + 4 + (Flt_ix * sizeof(Fault_type));
	wr_mram_array(MRAM_Flts_Dat_Page,k,(int16 *)(&f.fltw[Flt_ix]),(int16)(sizeof(Fault_type))/2); 

	Write_MRAM_Command(MRWRDI,0);
	return;
}


// ********************************************
// This is save the fault log
// ********************************************
void Update_All_Flt(void)
{
  	int16 i,j,k;

	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union

	Write_MRAM_Command(MRWREN,0);
	k = MRAM_Flts_Dat_Offset;
  	wr_mram(MRAM_Flts_Dat_Page,k,0x009E);
	k+=2;
  	wr_mram(MRAM_Flts_Dat_Page,k,Flt_ix);	// fault index
  	for(i=0; i<max_flt_ix; i++)
  	{
		k = MRAM_Flts_Dat_Offset + 4 + (i * sizeof(Fault_type));
		wr_mram_array(MRAM_Flts_Dat_Page,k,(int16 *)(&f.fltw[i]),(int16)(sizeof(Fault_type))/2); 
    }

	Write_MRAM_Command(MRWRDI,0);
 	return;
}

// ********************************************
// This is save the Long Term fault log
// ********************************************
void Update_LT_Flt(void)
{
  	int16 j,k;

	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
	
	if (LT_Flt_ix >= (max_lt_flt_ix-1))
		LT_Flt_ix = 0;
	else
		LT_Flt_ix++;

	
	Write_MRAM_Command(MRWREN,0);
	k = MRAM_LT_Flts_Dat_Offset;
  	wr_mram(MRAM_LT_Flts_Dat_Page,k,0x00AE);
	k+=2;
  	wr_mram(MRAM_LT_Flts_Dat_Page,k,LT_Flt_ix);	// fault index
	k+=2;
	k = MRAM_LT_Flts_Dat_Offset + 4 + (LT_Flt_ix * sizeof(Fault_type));
	wr_mram_array(MRAM_LT_Flts_Dat_Page,k,(int16 *)(&f.fltw[Flt_ix]),(int16)(sizeof(Fault_type))/2); 
	Write_MRAM_Command(MRWRDI,0);

 	return;
}

////////////////////////////

//*******************************************
// Verify consB.dat file  // tagwbg 19
//*******************************************

int16 verifyB_dat(void)
{
	int16 test,status;
  	FL_FILE *fp1;
//  	char *fp1;

  	disable_dog();
    status = 1;
   	fp1 = fl_fopen("/consB.dat","r");
	if (fp1 == NULL)
  	 	fp1 = fl_fopen("/CONSB.DAT","r");

	if (fp1 != NULL) status = 0;  // return 0; else return 1;
	fl_fclose(fp1);
	return status;
}

//////////////////////////

//*******************************************
// Verify .dat file
//*******************************************

int16 verify_dat(void)
{
	int16 test,status;
  	FL_FILE *fp1;
//  	char *fp1;

  	disable_dog();
	status = 1;			// Not found or no access
#if (Tract_HR == 1)
   	fp1 = fl_fopen("/conshr.dat","r");
	if (fp1 == NULL)
	   	fp1 = fl_fopen("/CONSHR.DAT","r");

#else
   	fp1 = fl_fopen("/cons.dat","r");
	if (fp1 == NULL)
  	 	fp1 = fl_fopen("/CONS.DAT","r");
#endif
	if (fp1 != NULL)
	{
		fl_fgets(datbuf,120,fp1);	// title on first line
      	test = (int16)atoi(datbuf);
		if ( (datbuf[0] != 'S') && (datbuf[1] != '0') && (test >= 0x0) && (test <= 0x1F) )
		{
			if (fl_filelength(fp1) <= (int32)(60000L))		
#if (Tract_HR == 1)
				status = 4;
#else
				status = 0;
#endif
			else
				status = 3;	// file too long
		}
		else
			status = 2;		// Not a DAT file
		fl_fclose(fp1);
	}
	enable_dog();
	return(status);
}

//***********************************************
// Write load weigher variables
//***********************************************

void wrlwvar(void)
{
	int16 i,k;
	
	Write_MRAM_Command(MRWREN,0);
	// load weigher variables
	k = MRAM_LW_Dat_Offset;   //  initialization flag byte
	wr_mram(MRAM_LW_Dat_Page,k,0x0096);
	k+=2;
	wr_mram(MRAM_LW_Dat_Page,k,lw_status);
	k+=2;
	wr_mram_array(MRAM_LW_Dat_Page,k,&lw_empty[1],cons[grtopf]); 
	k+=cons[grtopf];
	wr_mram_array(MRAM_LW_Dat_Page,k,&lw_full[1],cons[grtopf]); 

	Write_MRAM_Command(MRWRDI,0);
}

//***********************************************
// Write User variables
//***********************************************

void wruvar(void)
{
	int16 i, j, k;
	
	Write_MRAM_Command(MRWREN,0);
    // writing out the telephone number
	k = MRAM_User_Dat_Offset;   
	wr_mram(MRAM_User_Dat_Page,k,0x0025);
    i=0;
    while(i < 18)
    {
		tdata[i] = telnmb[i];
		i++;
    }
	
	tdata[i] = CR;

	k = MRAM_User_Dat_Offset+2;   // Skip initialization flag byte
	wr_mram_array(MRAM_User_Dat_Page,k,tdata,19); 

	k = 0;   
   	for(j=2; j<=7; j++)
   	{
   	  	i=0;
   	  	while((pass[j][i]!=CR) && (i < 18))      // writing out user defineable passwords
   	  	{
			tdata[k] = pass[j][i];
			k++;
 	  	    i++;
   	  	}
		tdata[k] = CR;
		k++;
   	  	i=0;
   	  	while((user[j][i]!=CR) && (i < 18))       // writing out user defineable user names
   	  	{
			tdata[k] = user[j][i];
			k++;
   	  	    i++;
   	  	}
		tdata[k] = CR;
		k++;
   	}
	k = MRAM_PassW_Dat_Offset;  
	wr_mram_array(MRAM_User_Dat_Page,k,tdata,228); 		// 228 bytes (6 * 19 * 2)
	Write_MRAM_Command(MRWRDI,0);
}

#if (GALaxy_4 == 1)
// ********************************************
// This is save the SPB field adjustable variables
// ********************************************
/*
void wr_spb_fvar(void)
{
		
  	int16 i,k;

	// set default variable that depend on job information

	set_spb_min_max_fvars();

	Write_MRAM_Command(MRWREN,0);
	// existing field variables
	k = (uint16)MRAM_SPB_FVARS_Dat_Offset;
	wr_mram(MRAM_SPB_FVARS_Dat_Page,k,0x0097);    
	k+=2;
	wr_mram_array(MRAM_SPB_FVARS_Dat_Page,k,spb_fvars,numspbvars); 
	Write_MRAM_Command(MRWRDI,0);
}
*/
// ********************************************
// This is save the NTS field adjustable variables
// ********************************************

void wr_nts_fvar(void)
{
		
  	int16 i,k;

	// set default variable that depend on job information

	set_nts_min_max_fvars();

	Write_MRAM_Command(MRWREN,0);
	// existing field variables
	k = (uint16)MRAM_NTS_FVARS_Dat_Offset;
	wr_mram(MRAM_NTS_FVARS_Dat_Page,k,0x0071);    
	k+=2;
	wr_mram_array(MRAM_NTS_FVARS_Dat_Page,k,nts_fvars,numntsvars); 
	Write_MRAM_Command(MRWRDI,0);
}
#endif


// ********************************************
// Write One field adjustable variables
// ********************************************

void wr_one_fvar(int16 var_offset)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union

  	int16 i,k;
	int16 chksum = 0;		

	// set default variable that depend on job information

	set_job_min_max_one_fvars(var_offset);
	Write_MRAM_Command(MRWREN,0);

	// existing field variables
	k = MRAM_FVARS_Dat_Offset;
	wr_mram(MRAM_FVARS_Dat_Page,k,(int16)0x0085);

	k=MRAM_FVARS_Dat_Offset+2+(var_offset * 2);
	wr_mram(MRAM_FVARS_Dat_Page,k,fvars[var_offset]);	// Field variable
	    
	chksum = 0;
    for(i=0; i<600; i++)
    {
		chksum += fvars[i];
    }

    for(i=0; i<=8; i++)
    {
		chksum += speed_clamp[i];
    }

	uiu.dw = cc_sec_mask[0];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = cc_sec_mask[1];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = rcc_sec_mask[0];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = rcc_sec_mask[1];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	

	write_fvars_chksum(chksum,0);
	
	k = (uint16)MRAM_FVARS_Backup_Dat_Offset;
	wr_mram(MRAM_FVARS_Backup_Dat_Page,k,(int16)0x0099);    

	k=MRAM_FVARS_Backup_Dat_Offset+2+(var_offset * 2);
	wr_mram(MRAM_FVARS_Backup_Dat_Page,k,fvars[var_offset]);	// Field variable
	
	chksum = 0;
    for(i=0; i<600; i++)
    {
		chksum += fvars[i];
    }
    
    for(i=0; i<=8; i++)
    {
		chksum += speed_clamp[i];
    }

	uiu.dw = cc_sec_mask[0];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = cc_sec_mask[1];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = rcc_sec_mask[0];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = rcc_sec_mask[1];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	

	write_fvars_chksum(chksum,1);

	Write_MRAM_Command(MRWRDI,0);
}

// ********************************************
// This is save the field adjustable variables
// ********************************************

void wrfvar(void)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union

  	int16 i,k;
#if ((Traction == 1) || (Tract_OL == 1))
  	int16 t_drv_baud;
#endif  	
	int16 chksum = 0;		

	
	// set default variable that depend on job information

	set_job_min_max_fvars();
	Write_MRAM_Command(MRWREN,0);

	// existing field variables
	k = MRAM_FVARS_Dat_Offset;
	wr_mram(MRAM_FVARS_Dat_Page,k,(int16)0x0085);
	k+=2;   
	wr_mram_array(MRAM_FVARS_Dat_Page,k,fvars,600); 
    for(i=0; i<600; i++)
    {
		chksum += fvars[i];
    }
	
	k = MRAM_Speed_Clamp_Dat_Offset;
	wr_mram_array(MRAM_FVARS_Dat_Page,k,speed_clamp,9); 
    for(i=0; i<=8; i++)
    {
		chksum += speed_clamp[i];
    }

	k = MRAM_Sec_CC_Dat_Offset;
	wr_mram_array(MRAM_FVARS_Dat_Page,k,(int16 *)cc_sec_mask,4); 
	uiu.dw = cc_sec_mask[0];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];
	uiu.dw = cc_sec_mask[1];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];
		
	k = MRAM_Sec_CC_Dat_Offset + 8;
	wr_mram_array(MRAM_FVARS_Dat_Page,k,(int16 *)rcc_sec_mask,4); 
	uiu.dw = rcc_sec_mask[0];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];	
	uiu.dw = rcc_sec_mask[1];
	chksum += uiu.w[lw];
	chksum += uiu.w[hw];
	
	write_fvars_chksum(chksum,0);

//	Write_MRAM_Command(MRWRDI,0);
	backup_fvars();

 	if(comm_baud[0] != baud_tbl[fvars[fvbaud]])	
  	{ // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15 debug15
	  	comm_baud[0] = baud_tbl[fvars[fvbaud]];
  		Restart_Com(USR1COM);		// Reset baud rate for user port
  	}
 	if(comm_baud[1] != baud_tbl[fvars[fvbaud2]])	
  	{ // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15 debug15
	  	comm_baud[1] = baud_tbl[fvars[fvbaud2]];
  		Restart_Com(USR2COM);		// Reset baud rate for user port
  	}

#if ((Traction == 1) || (Tract_OL == 1))
	if ((Drive_Type[cons[carnmb]] == 12) && (fvars[fvkebdpytype] == 1))	// keb with new keypad
		t_drv_baud = keb_drv_baud_tbl[fvars[fvdrvbaud]];
	else
		t_drv_baud = drv_baud_tbl[fvars[fvdrvbaud]];
	drv_modbus = drv_modbus_tbl[fvars[fvmodbus]];

 	if ((drv_baud != t_drv_baud) || (drv_modbus != drv_modbus_tbl[fvars[fvmodbus]]))
  	{
		drv_modbus = drv_modbus_tbl[fvars[fvmodbus]];
	  	drv_baud = t_drv_baud;
  		init_drv_com();		// Reset baud rate for drive port
  	}

#endif

 	if(sx_com_baud[SE1COM] != sx_baud_tbl[fvars[fvupsbaud]])	
  	{
	  	sx_com_baud[SE1COM] = sx_baud_tbl[fvars[fvupsbaud]];
		init_sx_com_port_1();
	}
 	if(sx_com_baud[SE2COM] != sx2_baud_tbl[fvars[fv485baud]])	
  	{
	  	sx_com_baud[SE2COM] = sx2_baud_tbl[fvars[fv485baud]];
		init_sx_com_port_2();
	}
 
#if (Traction == 1)
	if (can_enc_ppr != fvars[fvencppr]) 
	{
		can_enc_ppr = fvars[fvencppr];
		WriteEncPPR = 1;
	}
	if (can_enc_dir != fvars[fvencdir])
	{
		can_enc_dir = fvars[fvencdir];
		WriteEncDir = 1;
	}
#endif

	if ((selector_dl_deadzone != fvars[fvdldeadzone]) || (selector_ul_deadzone != fvars[fvuldeadzone]))
	{
		ctcan_put_dev_msg_req(c_ctcan_SEB_2, 59);
	}

/*REMOVED THE SETTING OF THE ENCODER FROM THIS LOCATION. iT WILL BE DONE TRHOUGH INCTIME()*/

	calculate_grp_fvars_chksum();
	calculate_button_color(0);

#if (Traction == 1)
	calculate_profile();
#endif
#if ((Traction == 1) || (Tract_OL == 1))
	calculate_brake();
	calculate_em_brake();
#endif

	i = verify_fvars(0);
	if (i != 0)
	{
		fault_device = i;
		fault_device_2 = var_num;
	 	record_fault(f_mram_write_er);
	}

	return;
}


//***************************************
//  Write Group Service Timers 
//***************************************

void Wrt_Grp_Svc_Timer (int16 tmrnmb)
{
	void* vptr;
	int16 i, k;

	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		

	if (tmrnmb >= nmbsvct)
		return;

	Write_MRAM_Command(MRWREN,0);
	k = MRAM_Group_Timer_Offset + (tmrnmb * (sizeof(svc_timer_size) + 2));	 // add initialization flag word
	wr_mram(MRAM_Group_Timer_Page,k,0x0039);
	k+=2;
	vptr = &svc_t[tmrnmb];
	wr_mram_array(MRAM_Group_Timer_Page,k,(int16 *)(vptr),(int16)(sizeof(svc_timer_size))/2); 

	calculate_srv_tmr_chksum(tmrnmb);
	Write_MRAM_Command(MRWRDI,0);
}

//***************************************************
//   Save Hoistway Digital Pulse Tables to Ram Disk
//***************************************************
void Wrt_Hoistway(void)
{
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
  	int16 i,k;
	Write_MRAM_Command(MRWREN,0);
 	k = MRAM_Hoist_Dat_Offset;	// Skip initialization flag byte
	wr_mram(MRAM_Hoist_Dat_Page,k,0x00D7);
	k+=2;
  	for(i= 1; i<= cons[topf]; i++)    // saving floor DPP position
  	{
		uiu.dw = DPP_Floor_Pos[i];
		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
		k+=2;
		wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
		k+=2;
  	}
	uiu.dw = DPP_DN;
	wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
	k+=2;
	wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
	k+=2;

	uiu.dw = DPP_DT;
	wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
	k+=2;
	wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
	k+=2;

	uiu.dw = DPP_UT;
	wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
	k+=2;
	wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
	k+=2;

	uiu.dw = DPP_UN;
	wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[lw]);
	k+=2;
	wr_mram(MRAM_Hoist_Dat_Page,k,uiu.w[hw]);
	k+=2;

	wr_mram(MRAM_Hoist_Dat_Page,k,Hoistway_Learned);
	k+=2;

	k = MRAM_Fl_Update_Adjust_Offset;
	wr_mram(MRAM_Fl_Update_Adjust_Page,k,0x00DA);
	k+=2;

	wr_mram(MRAM_Fl_Update_Adjust_Page,k,Up_fl_level_dist);
	k+=2;

	wr_mram(MRAM_Fl_Update_Adjust_Page,k,Dn_fl_level_dist);
	k+=2;

  	for(i= 1; i<= cons[topf]; i++)             /* store floor slowdowns */
  	{
		wr_mram(MRAM_Fl_Update_Adjust_Page,k,DPP_Fl_Up_SD[i]);
		k+=2;
		wr_mram(MRAM_Fl_Update_Adjust_Page,k,DPP_Fl_Dn_SD[i]);
		k+=2;
  	}
  	
  	for(i= 1; i<= cons[topf]; i++)             /* store floor slowdowns */
  	{
		wr_mram(MRAM_Fl_Update_Adjust_Page,k,DPP_SFl_Up_SD[i]);
		k+=2;
		wr_mram(MRAM_Fl_Update_Adjust_Page,k,DPP_SFl_Dn_SD[i]);
		k+=2;
  	}
	Write_MRAM_Command(MRWRDI,0);
}


// ********************************
// This is pet the watch dog timer
// ********************************
void petdog()
{
	SWT.SR.R = 0x0000A602;		// Send code to reset timer
	SWT.SR.R = 0x0000B480;
}

//************************************
// Turn off Watchdog
//************************************
void disable_dog(void)
{
    SWT._CR.R = 0xFF000002;			// Disable watch dog
}

//************************************
// Turn on Watchdog
//************************************
void enable_dog(void)
{
    SWT._CR.R = 0xFF000002;			// Disable watch dog
//    SWT.TO.R = 0x00186A00;			// 16 MHz * 0.100 (100 msec) = 1600000 = 0x00186A00
    SWT.TO.R = 0x00F42400;			// 16 MHz * 1.00 (1 sec) = 16000000 = 0x00F42400
    SWT._CR.R = 0x80000003;			// Z6 CPU only, Enable watch dog, disable in debug mode
}

//*********************************************
// Write MRAM code
//*********************************************
void write_MRAM_code (uint16 code)
{
	Write_MRAM_Command(MRWREN,0);
	wr_mram(MRAM_Pwrup_Dat_Page,MRAM_Pwrup_Dat_Offset+2,code);	// init code
	Write_MRAM_Command(MRWRDI,0);
}

//*********************************************
// Get MRAM code
//*********************************************
uint16 get_MRAM_code (void)
{
	return rd_mram(MRAM_Pwrup_Dat_Page,MRAM_Pwrup_Dat_Offset+2);
}
//*********************************************
// Write fvars checksum
//*********************************************
void write_fvars_chksum (uint16 cksum,int16 Table)
{
	wr_mram(MRAM_FVARS_Checksum_Page,MRAM_FVARS_Checksum_Offset + (Table * 2),cksum);	// init code
}

//*********************************************
// Get fvars checksum
//*********************************************
int16 get_fvars_chksum (int16 Table)
{
	return rd_mram(MRAM_FVARS_Checksum_Page,MRAM_FVARS_Checksum_Offset + (Table * 2));
}


/* Revision History

8/11/03 v3.06 mhd		1. Added default setting for field variables IR car select, Breke lift switch and load 
						   weighing anti-nuisance count.
8/27/03 v3.07. mhd		1. Added group hall call lockouts from car call lockout inputs   
9/22/03 v3.08 mhd  		1. Move call routines to calls.cpp file.
10/14/03 v3.10 mhd		1. Check cons[speed] to make sure it is not zero.  If zero set to 100.
12/2/03 v3.14 mhd		1. Made subroutines for Update_Control_Flags, Update_Flight_Times and Update_Job_Statistics.
12/22/03 v3.17 mhd		1. Added more job constants in cons file and a date code. see rdcon routine.
12/22/03 3.17 mhd		1. Add new security_type[] = 4 for seperate car call and hall call lockouts.  Use the rear car call 
						   lockouts for the hall calls.
2/25/04 4.00 mhd		1. Change read and writes to battery backed and eeprom memory for new comm memory board.
7/16/04 v4.02 mhd		1. Changed entire fault code mechanism.
11/2/04 V4.03 mhd		1. Store Blink_FSL in battery backed memory
2/24/05 v4.06 mhd		1. Added Staggard floor in cons file.
4/9/05 v4.09 mhd		1. Use temp variable so the index does not get trashed if an interrupt occurs during a fault write.
4/24/05 v4.11 mhd 		1. Changed Security_Type to allow both car call lockouts and group car call lockouts.
4/26/05 v4.14 mhd		1. Added extra lobby up call cons[LbyUpReq].
6/22/05 V4.23 MHD		1. Added local_gettime, local_settime, local_getdate, and local_setdate.  This was necessary in order to
                    	   disable the 18.2 msec DOS interrupt.
7/12/05 v4.24 mhd 		1. Added new Security_Type 0x10 for Lockout_Override to be used with group or ids floor lockouts.
9/14/05 V4.32 mhd		1. Added 21 extra cons variable including door_type. Also added heavy door mask.
11/10/05 v4.39 mhd		1. Moved the 60 new variables added in 4.38 after load weither variables to keep the eeprom data backward compatible.
11/17/05 				2. Calculate the ut position from cons[topf]-cons[bottomf] instead of cons[topf]-1 in Init hoistway.
1/9/06 v4.46 mhd		1. Set mask for car calls to can boards if housing job.
3/8/06 v4.58 mhd		1. Updated the brake trigger calculation.
3/20/06 V4.59 mhd 		1. Updated the brake trigger calculations again.
5/24/06 v4.67 mhd		1. Added offset for opto trigger time when calculating the brake trigger.
7/7/06 v4.75 mhd		1. Read car building characters into car_bld_no[7][2] instead of a car number into car_dat[7].
7/28/06 v4.77 mhd		1. Added floor position calculation for short floor	in init_hoistway routine.
8/16/06 v5.02 mhd		1. Added fvars[fvemdec] (Em_Decel) for the emergency decel rate.
9/8/06 v5.04 mhd		1. Set ccnmsk[0][0], ucnmsk[0][0], dcnmsk[0] etc. to zero then set ccmsk[0], ucmsk[0] and dcmsk[0] pointer
						   value to ccnmsk[0][0] etc. so that if car_no == zero the program won't crash from an invalid pointer. 
10/6/06 V5.05 mhd		1. Add seperate rdcons_hr routine for high rise car.
						2. Added update_pos to keep an updated position.
						3. Added staggerd auto/swing door option.
10/30/06 v5.06 mhd		1. Set lobby up request mask to 1 or 5.  When set to 1 that car is assigned the lobby up request or a standard
						   up hall call. When set to 5, the car is assigned the lobby up request but uses the secf input to determine
						   if the standard up hall call can be assigned to this car.  Bit 2 is set in software to disable the standard
						   up hall call assignment when SECF is on.  To use this feature, the secf feature must also be programmed. 
11/6/06 v5.08 mhd		1. Added UT6/DT6, UT5/DT5 and UT4/DT4 slowdown limits. 
						2. Moved analog load weigher table.  Changed field variable init value to detect and make table change.
12/19/06 v5.09 mhd		1. Correct calculation for brake_relevel_trigger_time in rdfvar. (opto_trig_tim was incorrectly added to 
                    	   acosret instead of brake_relevel_trigger_time
12/4/06 v5.10 mhd		1. Added test for valid hoistway data and for valid memory hardware.
1/3/07 v5.11 mhd		1. Use -1 for max number of cars and -2 for max number of floors for max adjustable variables and
						   use -1 for car number and -2 for bottom floor for the min variables.
						2. Changed all double data types to float for the djgpp compiler.
4/16/07 v5.22 mhd		1. Changed start index for verify_hoistway from 1 to cons[bottomf].
5/10/07 v5.23 mhd		1. Store SEL_Top_Count and SEL_Bottom_Count in EE ram memory.  
						2. Added better check for verify hoistway and set several more error codes.
						3. Store DPP_Count in EE RAM every 10 msec.
6/18/07 v5.27 mhd		1. Allow lby_up_req_car to be 1, 3 and 5 (added the 3).
7/2/07 v5.29 mhd		1. Read in code blue car mask to select valid code blue cars.
						2. Read in car assigned floor security mask.
7/20/07 v5.31 mhd		1. Allow option for cons[dpp_udp] != 0 instead of == 1 to allow for dz iterrupt operation.
8/9/07 v5.32 mhd		1. Added option for valid emergency power car to be selected to run.
10/1/07 V5.37 mhd		1. Increased number of variables from 180 to 360. 
10/4/07 v5.38 mhd		1. Added setup of second riser mask and Valid_SR_Car variable.
11/12/07 v5.39 mhd		1. Added Vip configuration for cb calls.
12/20/07 v5.39.8 mhd	1. Fix Vip index from 8 car to 6 cars.  Writes over load weigher option flag.
12/28/07 v5.39.9 mhd	1. Use -3 for cons[speed] in adjustable variables.
						2. Added short floor for test at 29 inches.
1/4/08 v5.40 mhd		1. Clear option cons[F1_CT] if 1040 TOC board not selected ((cons[sercom] & 0x04) == 0).
3/13/08 v5.42.1 mhd		1. Added mid short floor.
3/21/08 v5.42.3 mhd		1. Added check for top_rfl for secondary riser.
3/25/08 v5.42.4 mhd		1. Added seperate speed profile variables for short floor run.
4/9/08 V5.43.1 mhd		1. Disable bit for EOR input cons[Elev_Off] if earthquake used (EQR).
4/17/07 v5.43.6 mhd		1. Changed datbuf size from 60 to 120 for standard data and from 120 to 180 for high rise.
						2. Set variable Grp_CC_Override from Security_Type & 80h.
4/25/08 v5.43.8 mhd		1. Every time wrfvar is call you would get a drive comm error because comm_baud[2] was checked against baud_tbl[] instead of drv_baud_tbl[].
						2. In rdfvar(), clear the encoder count, outp(Encoder_Command, Encoder_CLR), when updating the DPP_Count from flash.
5/5/08 v5.44 mhd		1. Modified fault structure so that fault code is a word instead of a byte.  Deleted com1src, com3src, com3src, gcsrc and renamed tx_empty to device.
						   Added unused1, unused2 and unused3 to keep the structure the same size.
						2. Changed fault init code from C3 to 9D so that fault table will be erased.
						3. Or security mask with normal mask only if cons[SecFlCfg] != 0.
7/7/08 v5.45 mhd		1. Set default for fvacctopfl and fvaccbotfl from con[topf] and cons[bottomf] variables.
7/10/08 v5.45.1 mhd		1. Added cons variables 80 to 99 in Continue-C8.
						2. Added Up_fl_level_dist and Dn_fl_level_dist.
7/22/08 v5.45.2 mhd		1. Modified floor tables: deleted DPP_DZU_Count[], DPP_DZD_Count[], DPP_fl_offset[] and uset DZU_Dist, DZD_Dist, Up_fl_level_dist and Dn_fl_level_dist.
						2. Use (cons[dpp_upd] != 0) instead of looking at individual bits i.e. ((cons[dpp_upd] & 0x02) != 0) or ((cons[dpp_upd] & 0x04) != 0).
8/21/08 v5.45.6 mhd		1. Added Sabbath service floor mask from cons file.
9/3/08 v5.45.8 mhd		1. Added cons[hugs]==1 for hugs service. Uses (Security_type & 0x20).
9/29/08 v5.46 mhd		1. Store stallf in eeprom for hydro cars.
11/4/08 v5.46.5 mhd		1. Added group service timers.
12/01/08 v5.46.5 mhd/pn 1. In local_getdate check to see if day of week is greater 
						   or equal to 7 instead of just equal to 7.
12/15/08 v5.46.8 mhd	1. Added floor offset back into table.
01/06/09 v5.47 pn		1. Added alternate floor ascii pi's in the cons file
						   to switch between regular ascii pi and alternate 
						   ascii pi's.   
2/9/09 v5.47.5 mhd		1. Added "else" before if(inp(C1036_EERam_Add_NI) != 0xda) when checking for floor offset table to correct bug.
2/10/09 v5.47.6 mhd		1. Change floor index from j=0 to fl_size for call array data see init_call_data, clear_call_data and Up_Date_Calls.
						2. Corrected calc_dayofweek() to account months 13 and 14 in table.
3/9/09 v5.47.7 mhd		1. Corrected calc_dayofweek() for May and April (would be negative) (calculation from Zeller's method only showed one mod 7 
						   but should include two).	
						2. When reading local_getdate() verify that dayofweek is not < zero or > 6.
3/18/09 v5.47.11 mhd	1. Allow car to recover at Recovery_Vel (up to 80 fpm). Added calculation for fvrecvel parameter (Recovery Vel).
						2. Added calculations for Brake voltage drop ramp down parameters: fvbkdrop (Brake Drop Voltage), fvbkdroprate (Brake Voltage Drop Rate) and
						   fvbkdroplev (Brake Drop timing during Leveling flag). 
						3. Added Ep_Recov_Vel for emergency power recovery velocity.
						4. Added Em_Lev_Decel Emergency Deceleration to Leveling Rate.
						5. Added PI cross reference position table.
						6. Changed index for Continue-API in rdcons_std from 31 to 32 because there are 1-32 table values.
4/1/09 v5.48.3  mhd		1. Added routines set_job_default_fvars(void) and set_job_min_max_fvars(void) to set the fvars min, max and default that are job dependent.
5/29/09 v5.48.8 mhd		1. Added calculate button color routine.
						2. Read mask for serial hall button primary and secondary riser.

6/22/09 v5.48.12 hdl	1. created reset_fvars() function to take care of resetting all field variables back to the defaults
7/20/09 v5.49.1 mhd		1. Correct sign for floor offset when reading in.
10/21/09 V5.50.5 mhd	1. Added Emergency Power speed profile calculation and Ep_Top_Speed.						

11/11/09 v5.50.7 hdl	1. Allow EOR input to coexist with earthquake
						2. In rdcons, eliminate stament that will modify cons[elev_off]=~ 4

1/25/10 v5.51.8	hdl		1. added ccSecOnBright_prev = ccSecOnBright
							previously the variable never was utilized
2/25/10 v5.51.12 pn		1. Added blind floor ascii PI to cons file to indicate ascii characters when car is in blind shaft.
3/12/10 v5.51.14 mhd	1. Added security floor configuration tables for configuration 2, 3 and 4.
						2. Changed verify_dat() routine to allow cons.dat file to be 60000 bytes up from 30000.
3/19/10 v5.52 mhd		1. Added floor mask for code blue and vip riser.  Especially usefull for serial riser to use only the required number hall call boards.
4/27/10 v5.52.4 mhd		1. Added ETS_Dist to change the UTS/DTS Distance depending on car top speed.
5/17/10 v5.52.8 mhd		1. Changed color table to show red, green, blue then white.
6/7/10 v5.52.9 mhd		1. Added 60 additional field variables.	(from 360 to 420)
						2. Corrected number of service timers nmbsvct (needed to be a minimum of 17 - changed from 10 to 40).
						3. Modified calculate_button_color to add up, down, ir, codeblue and vip colors and added individual checksums for each.
8/1/10 v5.52.19 mhd		1. Added code blue second riser option input from cons.dat file.
8/18/10 v5.52.25 mhd	1. Added group hall call security "cons[SecFlCfg] == 3" to secure hall calls per floor per car from
							a key switch for each car and floor.

8/24/10 v5.52.26 mhd/hdl 1. Corrected HcbCBOn ON calculate hall_button_color(). It was set for VIP instead of CB.

10/18/10 v5.52.38 hdl	1. In rdfvar(), changed the value of cons[sercom] (RAM) to elliminate serial hall calls if Cross assigment or cross cancelation is enabled.

10/25/10 v5.52.39 hdl	1. Added the routine fvarstoCF() to read and write field variables from the flashcard
11/11/10 v5.52.46 mhd	1. Added software to set hall call mask that activates and de-activates special security for hall calls on individual cars.
10/29/11 v5.52.56 mhd 	1. Added cons[sel_type] = 3 for tapeless selector with absolute CAN encoder on governor going into the CPU encoder board.
						2. Replaced setting the encoder directly with preset_encoder_count() routine.
						3. Remove multiplier of 4 in Vel_Max calculation for can open encoder.

2/1/11	v5.52.58		1. correct the index for rcc_sec when being loaded from memory. It was using indexes outside the range
2/24/11 v5.53.4 mhd		1. Delete sending max pulse count: Set_MaxPulses_CanOpenEnc() so that encoder can modify the PPR.


4/14/11 v5.53.12 hdl/wc	1. Removed 	a section in the write field variable routine that will update the can open encoder. It is done in inctime now when the consitions are right (timer based)

5/23/11 v.5.53.21 hdl	1. Disable interrups in local_getdate to avoidtiming issue related to DALLAS real time clock on ts5300 CPU 
5/19/11 v5.54.02 mhd	1. Check parameter for small memory chip. If small chip then move FVARS_Dat_0ffset3 to FVARS_Dat_Offset3a so everything fits on small chip.
10/7/11 v5.54.25 mhd	1. Added Emergency Power Split Feeder cons file data.

10/13/11 v5.54.32 mhd	1. Initialize Hoistway_Init_Count in calculate_profile routine.
12/16/11 v5.54.45	hdl	1. Added scnd_pos and Update_scnd_Pos to help galaxy controller recognize position on power up to make the right recall decision in hydros (2007 code
6/25/12 V6.0.10 mhd		1. Corrected writing and reading word fault (previously a byte) to system status log.
9/4/12 v6.00.19 mhd		1. Added Update_SEL_Offset_Count() routine to save the SEL_Offset count in MRAM.
1/7/13 v7.0.0 mhd		1. Added seperate VIP mask.
7/25/13 v7.0.15 mhd		1. Added special baud for keb with new keypad
8/09/13 v6.0.50 as		1. Modified rdcons_hr(), rdcons_std() to read in the cons file continuations from Continue-VIPMSK to Continue-TR
8/15/13 v7.0.16 mhd		1. Added check for rs-485 comm baud rate.
10/28/13 v7.0.24 mhd	1. Corrected extern verify_hoistway() to have void parameters.
11/19/13 v7.0.29 mhd	1. Used fvar[fvenctype] for tape to set Vel_Max in pulses per second. 
11/27/13 V7.0.30 mhd	1. Replaced 2 instances of Sys_Stat_Size with MRAM_Sys_Stat_Size
						2. Replaced 1 instance of Stats_Dat_Offset with MRAM_Stats_Dat_Offset
						3. Replaced 1 instance of Fire_Dat_Offset with MRAM_Fire_Dat_Offset.
4/4/14 v7.0.40 mhd		1. Added cons file variable cons[fire_smoke_nc] = 1, all smoke detector inputs are normally closed contacts and modified fire service to operate as such.
4/22/14 v7.0.43 mhd		1. Modified atoh() routine to also use the format 0x01.
8/15/14 v7.1.8 mhd		1. Added cons file input for ep select and destination displatch.

11/10/14 v7.1.20 mhd	1. Set fvars[fvencnode] to node id from fvars[fvenctypd], i.e. Turck, Dynapar or Wachendorff
12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
							12/1/13 v6.1.16 mhd		1. Store flag in mram to run special trace software on power up automatically when set.
							4/5/2014 v6.1.43 hdl 	1. Added read_drive_from_MRAM() and save_drive_to_MRAM() routines to save
														drv_line_voltage drv_motor_torque drv_regen_torque to MRAM to preserve settings
							10/21/14 v6.1.91 as		1. Modified verify_dat() routine to allow first line to be <= 0x1F
1/29/15 v7.1.24 mhd		1. Changed minimum value in fvars table to -4 from -2 to match spread sheet for Wifi.  Also added -7 for car number. See set_job_min_max_fvars() routine.
6/3/15 v7.1.32 mhd		1. Changed calc_floor_to_Top_Speed() to add distance of last UT[n] or DT[n] limiti.  Try to reach topspeed before hitting the last limits.
8/5/15 V7.1.40 mhd		1. Changed load weigher to read values in both the up and down direction.  Renamed lw_empty[] and lw_full[] to lw_empty_up[] and lw_full_up[].
						2. Added variables lw_empty_dn[] and lw_full_dn[].  Copy lw_empty_up[] and lw_full_up[] to lw_empty_dn[] and lw_full_dn[] on first powerup if
						   load weigher setup with old version software.
						3. Chcek ut limits to be higher than the middle of the hoistway.
8/18/17 v8.0.9 mhd		1. Added front_op[i] for each car for changes in door lock and gate test.

*/


