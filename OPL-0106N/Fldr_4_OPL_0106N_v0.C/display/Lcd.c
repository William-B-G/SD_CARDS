
//  All LCD Files 1.14

#define d_LCDDPY 1
#include "global.h"
// #include "GALX_1132_TEST.h"
#include "SD_TEST.h"

#define ServiceTimer 1

// Prototypes of all functions in lcd.cpp
/**  Button Case   **/

int16 Check_for_Mode_Button(void);
int16 Check_for_Up_Button(void);
int16 Check_for_Down_Button(void);
int16 Check_for_Enter_Button(void);
void DebouncePushButtons(void);
void CheckDownButton(void);
void CheckUpButton(void);
void CheckEnterButton(void);
void CheckModeButton(void);
	int16 UpButtonStatus = ButtonEnable;
	int16 DownButtonStatus = ButtonEnable;
	int16 EnterButtonStatus = ButtonEnable;
	int16 ModeButtonStatus = ButtonEnable;
	int16 DownButton = ButtonRelease;
	int16 UpButton = ButtonRelease;
	int16 EnterButton = ButtonRelease;
	int16 ModeButton = ButtonRelease;
void init_LCD(void );
void reinit_LCD(void);
void Refresh_LCD(void );
void write_display(char *s, int16 row, int16 col);
void Fill_Entire_Line_One_Char(char *s, int16 row);
void New_write_display(char *s, int16 row, int16 col);
void Sprint_Lcd_SignalChar(int16 Row,int16 Col,unsigned char Ch);
void clrLCDdpy(void);
int16 set_date_and_time (int16 lcd_menu);
int16 CAN_Test(int16 lcd_menu);
int16 job_statistics(int16 lcd_menu);
int16 inputs_and_outputs(int16 lcd_menu);		  
int16 setup_car_and_hall_calls (int16 lcd_menu);
int16 run_elevator_status (int16 lcd_menu);
int16 view_fault_log (int16 lcd_menu);
int16 display_hoistway_floor_table (int16 lcd_menu);
int16 set_service_timers (int16 lcd_menu);
int16 cc_pb_security (int16 lcd_menu);
int16 adjust_lcd_display (int16 lcd_menu);
int16 software_utilities (int16 lcd_menu);
int16 password (int16 lcd_menu);
int16 display_immediate_fault(int16 lcd_menu);
void StoreLCDInfo(void);
void ReturnLCDMenu(void);
void write_LCD (void);
int16 display_hoistway(int16 lcd_menu);
int16 display_short_floor_sd(int16 lcd_menu);
int16 display_dz_off(int16 lcd_menu);
int16 display_cpu_limits(int16 lcd_menu);
int16 dpy_aps_valid_fl_table(int16 lcd_menu);
int16 count_one_bit_16(uint16 bitnmb);
void Display_Var_Label(int16 menu, int16 fv_index, int16 var);
void Elevator_Status_Init (int16 Init_Flag);
void Elevator_Service (struct Fault *ptr);
void Elevator_Status (int16 Status_Dpy, struct Fault *ptr);
char getcaps (char c);
void Service_Timers (void);
void clear_timer(int16 Timer);
void copy_timer(int16 timer_number, int16 Timer_Ix_1, int16 Timer_Ix_2);
void get_month_day_timer(int16 timer_number,int16 Timer_Ix);
void set_month_day_timer(int16 timer_number,int16 Timer_Ix);
void get_dow_timer(int16 timer_number,int16 Timer_Ix);
void set_dow_timer(int16 timer_number,int16 Timer_Ix);
void fault_display (int16 fault_line, int16 flt_ix, int16 dpy_ix);
int16 set_adj_digit (int32 max,int16 digit);
void adjust_variable (int16 *varptr, int16 max, int16 min, int16 inc, int16 digit);
void adjust_variable_32 (int32 *varptr, int32 max, int32 min, int32 inc, int16 digit);
void flash_digit (char dchar, int16 row, int16 col, int16 enable);
void display_variable(int16 row, int16 col, int16 digit, int16 lcd_par, int16 flash);
void display_variable_11_digits(int16 row, int16 col, int16 digit, int32 lcd_par, int16 flash);
int16 rdLCDio(int16 bitloc);
void display_io_name (int16 ioloc);
void display_io_value (int16 ioloc, struct Fault *ptr);

extern int16 safe(void);

extern void dog_ints_off(void);
extern void dog_ints_on(void);
extern void keybrd_restore (void);
extern void keybrd_init(void);
extern void rotate_io_bd (int16 dir);
extern struct Fault S_Trace[max_trace_ix + 1];
extern int16 JumpToPowerUp(int16 lcd_menu);

int16 test_data[20] = 
{
	0x1122,0x3344,0x5566,0x7788,0x7900,
	0x1234,0x5678,0x0910,0x1112,0x1314,
	0x1516,0x1718,0x1920,0x2122,0x2324,
	0x2526,0x2728,0x2930,0x3132,0x3334
};

int16 data_test[20];
int32 address;
bool fail;


int32 addr_ptr;
int32 address_code = 0;
int32 start_address = 0;

uint8 upd_row_0 = 0;
uint8 upd_row_1 = 0;

// Trace variables
int16 trace_dpy_ix = 0;
int16 next_trace = 0;
int8 set_dpy_ix = 0;
int8 trig_dpy_ix = 0;
int8 end_dpy_ix = 0;
int16 trace_dpy_offset = 0;
int16 dpy_offset_ix = 0;

// varaibales for the LCD Display

extern int16 LCD_Menu;
extern int16 LCD_Pointer;
extern int16 LCD_Upd_Dpy;
extern int16 LCD_Init;
extern int16 LCD_Sub_Init;


char LCD_Display[4][21];
char LCD_Display_old[4][21];
int16 LCD_Cur_Pos = 0;
int16 LCD_Line = 0;
int16 LCD_Flash = 1;
int16 LCD_Old_Flash = 1;
int8 LCD_Month;
int16 LCD_Year;
int8 LCD_Day;
int8 LCD_Minute;
int8 LCD_Hour;
char LCD_String[30];
char LCD_Time_Date[24];
char LCD_Flash_TMP[6];
int16 LCD_Dig_Point=0;
int16 LCD_Dig_Loc=0;
int16 LCD_ver = 0;
int16 LCD_rev = 0;

int16 sd_error = 0;		// secure digital return error code


unsigned char lcd_data[3]={0,0,0};
#define c_lcd_dbn_cnt 2

const uint16 bittbl[16] = {(uint16)0x01,(uint16)0x02,(uint16)0x04,(uint16)0x08,(uint16)0x10,(uint16)0x20,(uint16)0x40,(uint16)0x80,
							(uint16)0x100,(uint16)0x200,(uint16)0x400,(uint16)0x800,(uint16)0x1000,(uint16)0x2000,(uint16)0x4000,(uint16)0x8000};
const uint32 l_bit_mask[32] = 	
  {0x00000001L,0x00000002L,0x00000004L,0x00000008L,0x00000010L,0x00000020L,0x00000040L,0x00000080L,
   0x00000100L,0x00000200L,0x00000400L,0x00000800L,0x00001000L,0x00002000L,0x00004000L,0x00008000L,
   0x00010000L,0x00020000L,0x00040000L,0x00080000L,0x00100000L,0x00200000L,0x00400000L,0x00800000L,
   0x01000000L,0x02000000L,0x04000000L,0x08000000L,0x10000000L,0x20000000L,0x40000000L,0x80000000L
   };

static uint8 hex_to_ascii[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};



// fault display variables
static bool FaultInterrupt;

// Temp varibles && Password varibles for LCD menu
struct tag_LCD_State
	{
		int16 LCD_Menu;
		int16 LCD_Pointer;
		int16 LCD_Dig_Point;
		char LCD_Display[2][21];
	} LCD_State[3];

int16 LCD_State_ix;

int16 LCD_Pointer_Return;

int16 Password;
bool PasswordFlag;	   // a Flag that specified whether or not the password was enter correctly

int16 Timer_Number;
unsigned char Timer_Status;
char Timer_Service;
char Timer_Logic;

int16 date_time_var[6] = {0,0,0,0,0,0};
#define c_month 0
#define c_day 1
#define c_md_on_hour 2
#define c_md_on_minute 3
#define c_md_off_hour 4
#define c_md_off_minute 5

int16 dow_time_var[4] = {0,0,0,0};
#define c_on_hour 0
#define c_on_minute 1
#define c_off_hour 2
#define c_off_minute 3


// Security Code Varibles

int16 LCD_FloorNum = 1;
unsigned char Code_Num[4]={1,1,1,1};
int16 code_floor_access;
int16 CodeOption;
int16 Security_Dpy_Index;

int16 line_select;
int16 status_select;
int16 error_select;


#pragma section const_type

// tagwbg 12
const char xLCD_Main_Menu[15][21] = {
  "       GALaxy       ",	   // 0
  " CAN                ",	   // 1
  " RS-232             ",	   // 2
  " RS-485             ",	   // 3
  " WATCH DOG          ",	   // 4
  " MRAM               ",	   // 5
  " SD CARD            ",	   // 6 
  " WIFI               ",	   // 7
  " SOFTWARE VERSION   ",	   // 8
  " TEST X 1           ",	   // 9
  " TEST X 2           ",	   // 10
  " TEST X 3           ",	   // 11
  " ADJUST LCD DISPLAY ",	   // 12
  " GAL1100 IO BOARD   ",	   // 13
  " DIAGNOSTICS        ",	   // 14
};


#define Last_Menu 14
static const char Main_Menu_list[15] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};

char Menu_list[50];

static  const char LCD_Date_Time_Menu[2][21] = {
  " Set Date and Time  ",
  " View Day of Week   ",
};

static  const char LCD_DOW__Menu[8][21] = {
  "      Saturday      ",
  "       Sunday       ",
  "       Monday       ",
  "       Tuesday      ",
  "      Wednesday     ",
  "      Thursday      ",
  "       Friday       ",
  " Day of the Week:   ",
};

#define Car_Motion_Menu 0
#define Car_Brake_Menu 1
#define Modified_Motion_Menu 2
#define Car_Timers_Menu 3
#define Car_Options_Menu 4
#define Service_Options_Menu 5
#define Emergency_Service_Menu 6
#define Group_Dispatch_Menu 7
#define Group_Options_Menu 8
#define CC_COP_Lights_Menu 9
#define HC_IR_Call_Lights_Menu 10
#define CB_Vip_HSec_Call_Lights_Menu 11
#define System_Options_Menu 12
#define Drive_Parameters_Menu 13
#define Safety_Proc_Adj_Vars_Menu 14
#define NTS_Proc_Adj_Vars_Menu 15

const char CAN_Test_Items[16][21] = {
  " CAN A Test         ",	// menu 0
  " CAN B Test         ",	// menu 1
  " CAN C Test         ",	// menu 2
  " CAN x Test         ",	// menu 3
  " BLANK TEST 1       ",	// menu 4
  " BLANK TEST 2       ",	// menu 5
  " BLANK TEST 3       ",	// menu 6
  " BLANK TEST 4       ",  	// menu 7 
  " BLANK TEST 5       ",	// menu 8
  " BLANK TEST 6       ",	// menu 9
  " BLANK TEST 7       ",	// menu 10
  " BLANK TEST 8       ",	// menu 11
  " BLANK TEST 9       ",	// menu 12
  " BLANK TEST 10      ",	// menu 13
  " BLANK TEST 11      ",	// menu 14
  " BLANK TEST 12      ",	// menu 15
};


#define CAN_Test_Last_Menu 15				//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14
static const char CAN_Test_Menu_list[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,14,15,0};


static  const char LCD_Job_Statistics[2][21] = {
  " View Job Statistics",
  " Clear Job Statistic",
};

const char LCD_Input_Output[5][21] = {
  " Car Inputs & Output",
  " Grp Inputs & Output",
  " NTS Inputs & Output",
  " FPGA Input & Output",
  " Relocate IOs       ",
};

#define Last_IO_Menu 4				//0,1,2,3,4
static const char IO_Menu_list[5] = {0,1,2,3,4};



static  const char LCD_Fault_Log[2][21] = {
  "  View Fault Log    ",
  "  Clear Fault Log   ",
};

static  const char LCD_Detail_Fault[36][21] = {

  "Dpp Count=0000000000",   // 0  - 10
  "SD Count =0000000000",   // 1  - 10
  "EncV=0000,Enc Dir=No",   // 2  - 5, 18
  "%Load=000,          ",   // 3  - 6
  
  "NTS Count=0000000000",   // 4  - 6, 15
  "NTS Vel=      Dir=  ",   // 5  - 8, 15
  "UN*|UT*| DZ*|HWLrn=1",   // 6  - 4, 11, 18
  "DN*|DT*|FLT*|OnLin=1",   // 7  - 4, 11, 18

  "servf=   , procf=   ",	// 8  - 6, 17
  "doorf=   .rdpprf=   ",   // 9  - 6, 17
  "dpref=   ,  dirf=   ",   // 10 - 6, 17
  "firef=   ,rfiref=   ",   // 11 - 6, 17
  
  "codeb=   ,   eqf=   ",   // 12 - 6, 17
  " empf=   ,  medf=   ",	// 13 - 6, 17
  "  hsf=   ,startf=   ",   // 14 - 6, 17
  "stepf=   ,estopf=   ",   // 15 - 6, 17
  
  "callf=   ,dcalls=   ",   // 16 - 6, 17
  "nstop=   ,nxstop=   ",   // 17 - 6, 17
  "Relev=   ,LevStr=   ",   // 18 - 6, 17
  "NCUds=   ,DOseqf=   ",   // 19 - 6, 17
  
  "NudSt=   ,DoorRq=   ",   // 20 - 6, 17
  "FSd=     ,RSd=      ",   // 21 - 4, 14
  "InsSt=   ,InsSvc=   ",   // 22 - 6, 17
  "Motion Tmr =        ",   // 23 - 13
  
  " statusf =          ",   // 24 - 11
  "statusf3 =          ",   // 25 - 11
  "statusf4 =          ",   // 26 - 11
  "statusf2 =          ",   // 27 - 11
  
  "Run Stat =          ",   // 28 - 11
  "ssSt=    ,pwrSt=    ",   // 29 - 5, 16
  "HWLearn= ,Online=   ",   // 30 - 11
  "SelOk  = ,          ",   // 31 - 11

  "CkDrS =             ",	// 32 - 6, 17
  "CkStS =             ",	// 33 - 6, 17
  "CkRunS=             ",	// 34 - 6, 17
  "CkLevS=             ",	// 35 - 6, 17
  
};


// Only use 20 characters due to limited space on video display
const char LCD_device_Fault[62][21] = 
{
  "Dev:        Cnt:    ",	// 0
  "Invalid Fault       ",	// 1
  "EEprom Fault        ",	// 2
  "UTS Speed Fault     ", 	// 3
  "DTS Speed Fault     ",	// 4
  "UT Speed Fault      ",	// 5
  "DT Speed Fault      ",	// 6
  "INS Speed Fault     ",	// 7
  "LEV Speed Fault     ",	// 8
  "DL/GS Fault         ",	// 9
  "IO Fault            ",	// 10
  "INS DO Speed Fault  ",	// 11
  "No Pulses Fault     ",	// 12
  "Comm Fault          ",	// 13
  "PAL Error           ",	// 14
  "Address Error       ",	// 15
  "Encoder Vel Flt     ",	// 16
  "Encoder Comm Fault  ",	// 17
  "Encoder Init Fault  ",	// 18
  "Encoder Battery Flt ",	// 19
  "Encoder Fault       ",	// 20
  "Encoder Count Fault ",	// 21
  "Unintend Motion Flt ",	// 22
  "Reset Debug Trap    ",	// 23
  "Reset Illegal OpCode",	// 24
  "Reset MCLR Error    ",	// 25
  "Reset Software      ",	// 26
  "Reset W-Dog Timeout ",	// 27
  "Reset Sleep Wakeup  ",	// 28
  "Reset Idle Clock    ",	// 29
  "Reset Brown-out     ",	// 30
  "Reset Power-up      ",  	// 31  
  "NTSD Comm Fault     ",	// 32   
  "SEL Comm Fault      ",	// 33   
  "SPB Comm Fault      ",	// 34   
  "UT Limit Vel Fault  ",	// 35   
  "UT1 Limit Vel Fault ",	// 36   
  "UT2 Limit Vel Fault ",	// 37   
  "UT3 Limit Vel Fault ",	// 38   
  "UT4 Limit Vel Fault ",	// 39   
  "UT5 Limit Vel Fault ",	// 40   
  "UT6 Limit Vel Fault ",	// 41   
  "DT Limit Vel Fault  ",	// 42   
  "DT1 Limit Vel Fault ",	// 43   
  "DT2 Limit Vel Fault ",	// 44   
  "DT3 Limit Vel Fault ",	// 45   
  "DT4 Limit Vel Fault ",	// 46   
  "DT5 Limit Vel Fault ",	// 47   
  "DT6 Limit Vel Fault ",	// 48
  "UN Pulse Fault      ",   // 49
  "DN Pulse Fault      ",   // 50
  "Norm Lim Clk Pls Flt",   // 51
  "UT Pulse Fault      ",   // 52
  "DT Pulse Fault      ",   // 53
  "Term Lim Clk Pls Flt",   // 54
  "ETS Up Speed Fault  ",	// 55
  "ETS Down Speed Fault",	// 56
  "ETS Up Flt Set EBrk ",	// 57
  "ETS Dn Flt Set EBrk ",	// 58
  "PAL ETS Up Spd Flt  ",	// 59
  "PAL ETS Dn Spd Flt  ",	// 60
  "Parameter Setup Flt ",	// 61
};


const char LCD_device_Reset[2][21] = 
{
  "Device:             ",	// 0
  "Initialization Req  ",	// 1
};

static  const char LCD_Dbg_Status[4][21]={
  "Debug 1 =           ",
  "Debug 2 =           ",
  "Debug 3 =           ",
  "Debug 4 =           ",
};

static  const char LCD_Dbg_Status2[4][21]={
  "Debug 5 =           ",
  "Debug 6 =           ",
  "Debug 7 =           ",
  "Debug 8 =           ",
};

static  const char LCD_Fire_dpy_Status[3][21]={
  "f1ef =    ,frst =    ",  
  "gfct =    ,f2t1 =    ",
  "fsen =    ,fsts =    ",
};


			// medf = 1 recall car to medical emergency floor
			// medf = 2 car at medical emergency floor with door open (return complete)
			// medf = 3 On EMS car call service
			// codebf = 2 at code blue floor
			// codebf = 3 at code blue floor with door open
			// codebf = 4 finished code blue return 


static  const char LCD_ckdoor_tbl[41][14] = {
  "No Door Op   ", 	// 0 
  "Fire Door    ",	// 1
  "Med Em Svc   ",	// 2
  "EAQ Door Open",	// 3
  "EMP Wait DC  ",	// 4
  "EMP Home DO  ",	// 5
  "EMP Home DC  ",	// 6
  "EMP RCL Door ",	// 7
  "Stall Op Door",	// 8
  "Hot Oil Door ",	// 9
  "MedEm RCL @Fl",	// 10
  "MedEm RCL    ",	// 11
  "MedEm Wait Sw",	// 12
  "MedEm Svc Op ",	// 13
  "Hospital Svc ",	// 14
  "CB Ovr FS RCL",	// 15
  "CB Ovr FS Svc",	// 16
  "F1 Recall @FL",	// 17
  "F1 Recall    ",	// 18
  "F1 Complete  ",	// 19
  "F1 or F2     ",	// 20
  "At Floor Chk ",	// 21
  "Front DPM    ",	// 22
  "Rear DPM     ",	// 23
  "CodeBlue RCL ",	// 24
  "CodeBlue Svc ",  // 25
  "VIP Recall   ",  // 26
  "VIP Service  ",  // 27
  "Independent  ",  // 28
  "Overload     ",	// 29
  "Elevator Off ",	// 30
  "Prison Svc   ",	// 31
  "Push Button  ",	// 32
  "Attendant    ",	// 33
  "Extended Door",	// 34
  "Sabbath      ",	// 35
  "RTL Door Cl  ",	// 36
  "Lobby Recall ",	// 37
  "Car Elev Off ",	// 38
  "HW Elev Off  ",	// 39
  "Automatic    ",	// 40
};
static  const char LCD_ckstart_tbl[30][14] = {
  "No Start Op  ", 	// 0 
  "UN Limit Off ",	// 1
  "FSTU Fail On ",	// 2
  "SPD Failed On",	// 3
  "MC Failed On ",	// 4
  "MC Failed Off",	// 5
  "SPD Fail Off ",	// 6
  "RUNU Fail On ",	// 7
  "RUNU Fail Off",	// 8
  "SU Failed On ",	// 9
  "SUF Failed On",	// 10
  "SU Failed Off",	// 11
  "SUF Fail Off ",	// 12
  "SUF On/W SU  ",	// 13
  "RUN Up       ",	// 14
  "DN Limit Off ",	// 15
  "FSTD Fail On ",	// 16
  "RUND Fail On ",	// 17
  "RUND Fail Off",	// 18
  "SD Failed On ",	// 19
  "SDF Failed On",	// 20
  "SD Failed Off",	// 21
  "SDF Fail Off ",	// 22
  "SDF On/W SD  ",	// 23
  "RUN Down     ",	// 24
  "25           ",  // 25
  "26           ",  // 26
  "27           ",  // 27
  "28           ",  // 28
  "29           ",	// 29
};
static  const char LCD_ckrun_tbl[41][14] = {
  "No Run Op    ", 	// 0 
  "HW Learn Flt ",	// 1
  "PAL Fault    ",	// 2
  "Comm Error   ",	// 3
  "IO Test      ",	// 4
  "Norm Stop Tmr",	// 5
  "ATT No DC CC ",	// 6
  "ATT No UP/DN ",	// 7
  "ATT No DClose",	// 8
  "No ATT UP    ",	// 9
  "No ATT No DC ",	// 10
  "CarSw No DC  ",	// 11
  "CarSw DC Hold",	// 12
  "Man Door Time",	// 13
  "SafeTest Year",	// 14
  "SafeTst Month",	// 15
  "SafeTest Day ",	// 16
  "Door Request ",	// 17
  "Door Open    ",	// 18
  "Door Closing ",	// 19
  "PreTq NewPref",	// 20
  "DC No DO Seq ",	// 21
  "UC No DO Seq ",	// 22
  "LC No DO Seq ",	// 23
  "RDC No DO Seq",	// 24
  "RUC No DO Seq",  // 25
  "Viscosity    ",  // 26
  "Relevel Req  ",  // 27
  "No Calls     ",  // 28
  "Earthquake   ",	// 29
  "No DCl       ",	// 30
  "No Rear DCL  ",	// 31
  "No SAFE      ",	// 32
  "Running      ",	// 33
  "34           ",	// 34
  "35           ",	// 35
  "36           ",	// 36
  "37           ",	// 37
  "38           ",	// 38
  "39           ",	// 39
  "40           ",	// 40
};
static  const char LCD_cklevel_tbl[11][14] = {
  "No Level Op  ", 	// 0 
  "MC Failed Off",	// 1
  "SPD Fail Off ",	// 2
  "RUNU Fail Off",	// 3
  "SU Failed Off",	// 4
  "SUF Failed On",	// 5
  "Level Up     ",	// 6
  "RUND Fail Off",	// 7
  "SD Failed Off",	// 8
  "SDF Failed On",	// 9
  "Level Down   ",	// 10
};


static  const char LCD_Dpy_Hoistway[5][21] = {
  " Floor & SD Counts  ",
  " Short FL SD Counts ",
  " CPU Term Limit Dist",
  " UL & DL Distance   ",
  " Valid Floor & Clips",
};

#define Last_Hoistway_Menu 4			 //0,1,2,3,4
static const char Hoistway_Menu_list[5] = {0,1,2,3,4};


static  const char LCD_Set_Time_Date[2][21] = {
  " Press Mode for Next",
  " and Enter to Set   ",
};




#define MainMenu_Dpy_Offset 0
#define Function_Dpy_Offset 2
#define NO_YES_Dpy_Offset 6
#define Floor_Dpy_Offset 8
#define Code_Tbl_Empty_Offset 9


// Security Options
static  const char Security_dpy[14][21] = {
  " Sec Codes Per Floor", //0
  "Sec Codes All Floors", //1
  " View Security Codes", //2
  "  Add Security Code ", //3
  "Delete Security Code", //4
  "Del All Codes On Flr", //5
  "         NO         ", //6
  "         YES        ", //7 
  "    Floor #    1    ", //8
  "  Code Table Empty  ", //9
  "Add Code On All Flrs", //10
  "Delete Code All Flrs", //11
  " Del All Codes/Flrs ", //12
  "   Code Table Full  ", //13
};								  



static const char LCD_Par[lastfvar+1][21] ={
  "Min=0   Max=5.0  Sec",   // 000 - Y Delta Time
  "Min=0   Max=10.0 Sec",   // 001 - Fault Time
  "Min=0   Max=10.0 Sec",   // 002 - Reset Time
  "0=1 Gong,  1=2 Gongs",   // 003 - Double Stroke
  "Min=0   Max=2.0  Sec",   // 004 - Lant On Time
  "Min=0   Max=2.0  Sec",   // 005 - Lant Off Time
  "Min=.2  Max=2.0  Sec",   // 006 - Pas Chime Time
  "Mn=10  Mx=3200.0 Sec",   // 007 - Door Fail Time
  "Mn=20  Mx=3200.0 Sec",   // 008 - Nudging Time
  "Min=0  Mx=3200.0 Sec",   // 009 - Preopen Delay
  "Min=1.0 Max=60.0 Sec",   // 010 - Car Call Dwell
  "Min=1.0 Max=60.0 Sec",   // 011 - Hall Call Dwell
  "Min=1.0 Max=60.0 Sec",   // 012 - Lobby Dwell
  "Min=1.0 Max=120  Sec",   // 013 - Handicap Dwell
  "Min=1.0 Max=60.0 Sec",   // 014 - Non Interfer T
  "Min=Bot Max=Top Flr ",   // 015 - Lobby Floor
  "Min=Bot Max=Top Flr ",   // 016 - Fire Main Floor
  "Min=Bot Max=Top Flr ",   // 017 - ALT Fire Floor
  "Mn=20  Mx=3200.0 Sec",   // 018 - Stall Time
  "Min=0.2 Max=3.0  Sec",   // 019 - Soft Stop Time
  "Mn=30  Mx=3200.0 Sec",   // 020 - Gen/Lt/Fan Time
  "Min=0   Max=5.0  Sec",   // 021 - Brake Pick Del
  "Min=0   Max=5.0  Sec",   // 022 - Brake Drop Del
  "Min=50 Mx=480 fpm/ss",   // 023 - Soft Start Jerk
  "Min=50 Max=300 fpm/s",   // 024 - Acceleration
  "Min=50 Mx=480 fpm/ss",   // 025 - Decel Jerk
  "Min=50 Max=300 fpm/s",   // 026 - Decel Rate
  "Min=1.2 Max=25.0  in",   // 027 - Floor Targ Dis
  "Min=1.0 Max=15.0 fpm",   // 028 - Leveling Speed
  "Min=1.0 Max=15.0 fpm",   // 029 - Relevel Speed
  "Min=0   Max=150  fpm",   // 030 - Inspect Speed
  "Min=2000 Max=2999 yr",   // 031 - Year
  "Min=1   Max=12 month",   // 032 - Month
  "Min=1   Max=31   day",   // 033 - Day
  "Min=0   Max=23    hr",   // 034 - Hour
  "Min=0   Max=59   min",   // 035 - Minute
  "Mn=50  Mx=480 fpm/ss",   // 036 - Roll Over Jerk
  "0=STK16C88  1=16C68 ",   // 037 - EE Memory Type
  "Min=2000 Max=2999 yr",   // 038 - Safe Test Year
  "Min=1   Max=12 month",   // 039 - Safe Test Month
  "Min=1   Max=31   day",   // 040 - Safe Test Day
  "Min=0   Max=3.00 sec",   // 041 - Pattern Delay
  "Min=60 Max=110  %vel",   // 042 - Field Weaken
  "Min=60 Max=65535 ppr",   // 043 - Encoder PPR
  "Min=1 Max=3000.0 rpm",   // 044 - Encoder RPM
  "0-3=19.2,38.4,57.6, ",   // 045 - User Baud Rate
  "Min=0 Mx=#Cars #cars",   // 046 - Parking
  "Min=Bot Max=Top Flr ",   // 047 - Em Power Floor
  "Min=1 Mx=#Cars #cars",   // 048 - Em Power Cars
  "Min=0 Max=#Cars car#",   // 049 - 1st Recall Car
  "Min=0 Max=#Cars car#",   // 050 - 1st EP Run Car
  "Min=1 Max=600.0  Sec",   // 051 - Recall Timeout
  "0 = OFF    1 = ON   ",   // 052 - DOB Over Nudg
  "0=OFF,1=ON,2=Ft;4=Rr",   // 053 - Emerg Dispatch
  "Min=1 Mx=#Cars #cars",   // 054 - Lobby Request
  "0=OF,1=NU,2=NDC,4=Pk",   // 055 - Next Car Up
  "Min=0 Max=#Cars car#",   // 056 - Code Blue Car
  "Min=0 Max=#Cars car#",   // 057 - IR Car
  "0=OF,1=ON,2=ON W/EBK",   // 058 - Brake Lift Sw
  "0=OFF,  Max=50  #CCs",   // 059 - LW Anti-nuisan
  "Alt|M 0=ff,1=fr,2=rf",   // 060 - Fire Sw Loc
  "Min=0   Max=60   Sec",   // 061 - ETA Min Time
  "Min=0   Max=60   Sec",   // 062 - ETA Co CC Time
  "Min=1 Max=#Cars loc#",   // 063 - Vid Pos Car 1
  "Min=1 Max=#Cars loc#",   // 064 - Vid Pos Car 2
  "Min=1 Max=#Cars loc#",   // 065 - Vid Pos Car 3
  "Min=1 Max=#Cars loc#",   // 066 - Vid Pos Car 4
  "Min=1 Max=#Cars loc#",   // 067 - Vid Pos Car 5
  "Min=1 Max=#Cars loc#",   // 068 - Vid Pos Car 6
  "0=OFF,Mx=10 #of runs",   // 069 - No Psg Run Cnt
  "0=OFF. 1=CCS. 2=SECF",   // 070 - Ind Over Sec
  "Min=0   Max=5.0  Sec",   // 071 - Relev Brk Delay
  "0 = OFF    1 = ON   ",   // 072 - Relev Brk LowV
  "Min=0 Max=3200.0 Sec",   // 073 - Up Pk Trig Time
  "Min=1 Mx=40 #Pk Trig",   // 074 - Up Pk Trig Cnt
  "Min=1  Max=40   #CCs",   // 075 - Up Pk CC Count
  "Mn=0.0 Mx=3200.0 Sec",   // 076 - Up Peak Time
  "Mn=0.0 Mx=3200.0 Sec",   // 077 - Dn Pk Trig Time
  "Min=0  Mx=100 #Dn HC",   // 078 - Dn Pk Trig Cnt
  "Mn=0.0 Mx=3200.0 Sec",   // 079 - Down Peak Time
  "Mn=0.0  Mx=120.0 Sec",   // 080 - Park Delay Time
  "0=OF,1=XASGN,2=XCANC",   // 081 - HC X-Assign En
  "Min=0  Max=500   Sec",   // 082 - HC X-Assign ETA
  "Min=Bot Max=Top Flr ",   // 083 - Med Em Floor
  "0=Front, 1=Rear  Flr",   // 084 - Med Em Sw Loc
  "0=Main, 1=Alt, Floor",   // 085 - MachRm Fire Ret
  "0=Main, 1=Alt, Floor",   // 086 - Hoistw Fire Ret
  "AftRec,1=Imm,2=FS OF",   // 087 - Recall Reset
  "Min=5  Max=225 fpm/s",   // 088 - Stop Decel Rate
  "Min=0  Max=3.00  sec",   // 089 - Relevel Pattern Dly
  "0=Immediate,1=AftDON",   // 090 - DON Start Control
  "Mn=0 Mx=7372 3686/ms",   // 091 - Brake Opto Dly
  " 0 = N/C   1 = N/O  ",   // 092 - Invert Brake Lift SW
  "0=IS,1=Inv,2=IU,4=OS",   // 093 - Invert ISER
  "Group Dispatcher Car",   // 094 - Dispatcher car
  "Min=0  Mx=3200.0 Sec",   // 095 - Video time out
  "0 = OFF    1 = ON   ",   // 096 - Door Close Outp. Ins.
  "Min=1 Max=Top  Floor",   // 097 - Parking floor 1
  "Min=1 Max=Top  Floor",   // 098 - Parking floor 2
  "Min=1 Max=Top  Floor",   // 099 - Parking floor 3
  "Min=1 Max=Top  Floor",   // 100 - Parking floor 4
  "Min=1 Max=Top  Floor",   // 101 - Parking floor 5
  "Min=1 Max=Top  Floor",   // 102 - Parking width
  "Min=0 Mx=100.0 %Load",   // 103 - Percent balanced load
  "Mn=0 Mx=100.00  gain",   // 104 - Torque amount
  "Min=0 Max=100  %Load",   // 105 - Load Bypass percent
  "Min=0 Max=100  %Load",   // 106 - Load Antinuisance percent
  "Min=0 Max=100  %Load",   // 107 - Load Dispatch percent
  "Min=0 Max=125  %Load",   // 108 - Load Overload percent
  "IREn,2=AnCB4S,AnCB4F",   // 109 - IR ControlFinishing
  "Min=20 Max=400  Volt",   // 110 - Brake Pick Voltage
  "Min=0.1 Max=6.0  Sec",   // 111 - Brake Pick Time
  "Min=20 Max=400  Volt",   // 112 - Brake Hold Voltage
  "Min=10 Max=400  Volt",   // 113 - Brake Relevel Volt
  "Min=80 Max=300  Volt",   // 114 - Brake L-L Voltage
  "Mn=0.1 Mx=500.0  Ohm",   // 115 - Brake Resistance
  "Mn=0.0 Mx=900.0  Sec",   // 116 - Attendant Buzzer Delay
  "0=No FSX Sw,1=FSX Sw",   // 117 - Aux. Hall Fire Sw
  "0=F1&2,F1,BLK,4=FSL ",   // 118 - Hall fire light
  "0=Bo,1=Sep,C-R,4=R-C",   // 119 - COP/Remote CC Select
  "Min=0  Max=1.5   Sec",   // 120 - Door Open or close delay time
  "1=Rcl,fD,4=rD,AlwRcl",   // 121 - Security Recall
  "1=CCS,2=SECF,4=R CCS",   // 122 - DOB override securityCCS Rear Only.
  "Mn=0.0 Mx=900.0  Sec",   // 123 - Manual Door Buzzer Delay
  "Min=0  Max=Top   Flr",   // 124 - Security Floor
  "Min=0  Max=5.0   Sec",   // 125 - Retiring Cam pick delay timer
  "Min=0.0  Mx=60.0 Sec",   // 126 - Short dwell door time
  "Min=0 Mx=#Cars  car#",   // 127 - 2nd IR Car
  "1=Up,2=Dn,4=HC,8=RTL",   // 128 - Stop At lobby
  " 0 = N/O  1 = N/C   ",   // 129 - Invert CLF
  " 0 = N/O  1 = N/C   ",   // 130 - Invert TPL
  " 0 = N/O  1 = N/C   ",   // 131 - Invert LPS
  " 0 = N/O  1 = N/C   ",   // 132 - Invert LOS
  "0=Norm,1=Nud No Call",   // 133 - Nudge No Calls
  "F1:1=No Blk,2=BZ Off",   // 134 - Fire Option
  "  0 = OFF  1 = ON   ",   // 135 - Attendant HC sets car call
  "Mn=1.0 Mx=3200.0 Sec",   // 136 - Code blue door open time
  "  0 = OFF  1 = ON   ",   // 137 - CB Req Ind Car
  "  0 = OFF  1 = ON   ",   // 138 - Code blue light
  "  0 = OFF  1 = ON   ",   // 139 - HSV door close on Car call
  "Min=0  Max=20.0  Sec",   // 140 - Gripper Reset Time
  "  0 = OFF  1 = ON   ",   // 141 - Ind Door CL CC
  "Mn=0 Mx=50.00 offset",   // 142 - Torque offset when no comp ropes
  " 1=SPB Spd,2=OverSpd",   // 143 - Gripper Spb spd or overspd.
  "1=EN Aut,2=DIS,4=1CC",   // 144 - Single Auto PB
  "  0 = 0FF  1 = ON   ",   // 145 - Door Open when no DOL
  "                    ",   // 146 - Previously: fvdisglt - Disable Gate and Lock Test for New York City
  "Mn=0.0  Mx=15.0  fpm",   // 147 - Relevel Start speed
  "Min=80  Mx=360 fpm/s",   // 148 - Emergency Decel Rate
  "  0 = ON   1 = OFF  ",   // 149 - Disable DCB cancel dwell time
  "1=CyDo,2=CCC,4=RevDo",   // 150 - Return to lobby option
  "Min=1 Max=#Cars loc#",   // 151 - Vid Pos Car 7
  "Min=1 Max=#Cars loc#",   // 152 - Vid Pos Car 8
  "0=#HC/F,1=DivH,2=Var",   // 153 - Parking Type
  "Min=1  Max=Top   Flr",   // 154 - Parking floor 6
  "Min=1  Max=Top   Flr",   // 155 - Parking floor 7
  "1=Frnt 1st, 2=Rr 1st",   // 156 - Non-Simultaneous Doors
  "0=OFF,1=ON,2=Off SFl",   // 157 - Preopen Doors
  "Min=80  Max=125 %Spd",   // 158 - Percent Overspeed
  "1=ON,2=Dz Int,4=CVel",   // 159 - Position Count Update
  "Mi=1 Mx=10000  1/sec",   // 160 - Pulse Error Delay
  "  0 = 0FF  1 = ON   ",   // 161 - Gripper Trip Buzzer
  "Min=0.0 Max=2.0  Sec",   // 162 - Relevel Delay Time
  "1=Ph2RclDO,2=NoBlkF2",   // 163 - Fire Option  2
  "  0 = OFF  1 = ON   ",   // 164 - Close gate,swing door no pref
  "Mn=3 Mx=10 #Lev Flts",   // 165 - Leveling Fault Count
  "Mn=0 Mx=100.00  gain",   // 166 - Torque Down Amt
  "Mn=0.00 Mx=1.00  Sec",   // 167 - Torque Ramp Control
  "Mn=0.0 Mx=3200.0 Sec",   // 168 - Electric Eye Time-out Time
  "  0 = OFF  1 = ON   ",   // 169 - OTS No HC Canc
  "1=Dis,2=Reopen,4=Buz",   // 170 - Nudge Dis Ctrl
  "Mn=10.0 Mx=60.0  Sec",   // 171 - Phase 1 door close time-out
  "0=0FF +1=Alwys +2=Pk",   // 172 - No HC Door Reop
  "Mn=1 Mx=10 #Rdy Flts",   // 173 - Drive Rdy Faults
  "Mn=1.0  Mx=60.0  Sec",   // 174 - Return to Lobby Door Dwell Time
  "TimOut:+1=HUL,+2=CUL",   // 175 - Next Up Direction Lant Ctl
  "0=OnDCL,1=Aft NU Tim",   // 176 - Next Up Preference Ctl
  "1=No HC,2=Or SR/w HC",   // 177 - Second Riser Ctrl
  "1=Cab Lant          ",   // 178 - SR Lantern Ctl
  "Min=0   Max=60   Sec",   // 179 - Attendant ETA Pref
  "  0 = 0FF  1 = ON   ",   // 180 - HBZ/PI Dis at non-valid fl
  "Mn=1.0 Mx=3200.0 Sec",   // 181 - Vip Door Time
  "+1=Canc VIPC,+2=CCC ",   // 182 - Vip Operation Flags
  "Mn=0 Mx=#Cars  #cars",   // 183 - Number of Vip Cars
  "0=Op,Norm,2=EmP,Rcal",   // 184 - EP Op LED function
  " 0=Prkd,Rcld,2=epflr",   // 185 - EP Park LED funct
  "1=RC,2=DO,4=LFof8=DC",   // 186 - Hall Elevator Off options
  "0-3=192,384,576,115 ",   // 187 - Drive baud rate
  "0=10,1=15,2=20  msec",   // 188 - Drive update rate
  "Min=1  Max=Top   Flr",   // 189 - Alternate Lobby Fl
  "Min=1  Max=Top   Flr",   // 190 - Alt Parking Fl
  "0=Always,1=NCU/Up Pk",   // 191 - Lobby Req Cntrl
  "Min=0 Max=100  %Load",   // 192 - Handicap Capacity percent
  "Mn=1.0 Mx=3200.0 Sec",   // 193 - Freight door close time-out
  "Min=0   Max=255  Sec",   // 194 - Handicap Car Wait
  "Mn=10.0 Mx=72.0 Inch",   // 195 - Short floor distance in inches
  "0=Norm,+1=Hld,+2=Adv",   // 196 - Retiring Cam Control
  "0=Main+Aux Sw,1=Main",   // 197 - Recall from FS1 Alt Floor
  "Mn=1.0 Mx=3200.0 Sec",   // 198 - Hoistway Learn Stall Time
  "Min=0.00 Max=2.00 in",   // 199 - Position Pulse Count Error
  "0=On   Max=30.0  Sec",   // 200 - Attendant Buzzer Off Time
  "Min=1.0 Max=30.0 Sec",   // 201 - Attendant Buzzer On Time
  "Min=0.0 Max=2.0  Sec",   // 202 - Brake pick rate
  "Min=0.0 Max=2.0  Sec",   // 203 - Brake relevel rate
  "Min=0  Max=400  Volt",   // 204 - Brake Pick Start
  "Min=0  Max=400  Volt",   // 205 - Brake relevel Start
  "Min=1.0 Max=30.0 Sec",   // 206 - Short Fl Dn SD
  "Min=1.0 Max=30.0 Sec",   // 207 - Mid Shrt Fl Dn SD
  "Min=1.0 Max=30.0 Sec",   // 208 - Short Fl Up SD
  "Min=1.0 Max=30.0 Sec",   // 209 - Mid Shrt Fl Up SD
  "0=Lobby Max=Top  Flr",   // 210 - HS Elevator Off floor
  "Min=0 Max=30 Svc Num",   // 211 - Service msg 1 display
  "Min=0 Max=30 Svc Num",   // 212 - Service msg 2 display
  "0=GOV  1=GOV+Speed  ",   // 213 - GOV Gripper Trip Ctl
  "Mn=50  Mx=480 fpm/ss",   // 214 - Short Floor Soft Start Jerk
  "Mn=50  Mx=300  fpm/s",   // 215 - Short Floor Accel Rate
  "Mn=50  Mx=480 fpm/ss",   // 216 - Short Floor Roll Over Jerk
  "Mn=50  Mx=480 fpm/ss",   // 217 - Short Floor Decel Jerk
  "Mn=50  Mx=300  fpm/s",   // 218 - Short Floor Decel Rate
  "Min=1.2  Max=25.0 in",   // 219 - Short Floor Targ Dis
  "1=SFR,2=SDSF,4=SDMSF",   // 220 - Short Floor Control
  "0=1stRcl,1=All,2=no ",   // 221 - Security Recall 2group recalls
  "Mn=0.0 Mx=3200.0 Sec",   // 222 - EP Recall Delay
  "Min=1   Max=240  Sec",   // 223 - Grp cc sec ovr tim
  "Min=0.0 Mx=360.0 Sec",   // 224 - Em brake drop dly
  "Min=0.0 Max=10.0 Sec",   // 225 - Em brake pick tim
  "Min=0.0 Max=100.0  %",   // 226 - HC on brightness
  "Min=0.0 Max=100.0  %",   // 227 - HC off brightness
  "Min=Bot Max=Top  Flr",   // 228 - Access Top Floor
  "Min=Bot Max=Top  Flr",   // 229 - Access Bottom Floor
  "0=UL,DL, 1=PLS CNT  ",   // 230 - Stop on position pulse
  "Mn=0.0 Mx=3200.0 Sec",   // 231 - Sabbath restart
  "Mn=0.0 Max=30.0  Sec",   // 232 - Freight Power Door open time
  "  0 = 0FF  1 = ON   ",   // 233 - Automatically Display Fault
  "Min=0000  Max=9999  ",   // 234 - Password
  "Mn=0.0 Mx=3200.0 Sec",   // 235 - Password time-out
  "  0 = 0FF  1 = ON   ",   // 236 - Behind car call cancel
  "Min=0.0 Mx=240.0 Sec",   // 237 - Adv de time non-simult doors
  "Mn=0 Mx=#Cars  #cars",   // 238 - Grp Timer Parking
  "0=Recovery, 1=Always",   // 239 - Binary preset always
  "0=HS Out,1=No HS Out",   // 240 - Shrt fl hsf/no hsout
  "0=Norm UPk,1=Hvy UPk",   // 241 - Up peak control
  "Mn=0 Mx=#Cars  #cars",   // 242 - Up peak pool
  "0=Norm DPk,1=Hvy DPk",   // 243 - Down peak control
  "Min=0 Mx=#Cars #cars",   // 244 - Down peak pool
  "Min=0 Mx=#Cars #cars",   // 245 - HC X-Assign Cars
  "0-5N1,N2,E1,E2,O1,O2",   // 246 - Drive Modbus protocol
  "0=No,1=ALTof,3=ALTon",   // 247 - Fire alt rcl after fs off
  "Min=50 Max=480 fpm/s",   // 248 - Inspection Decel Stop Rate
  "0=Movement, 1=Load  ",   // 249 - Emergency Power Recovery Dir
  "Min=0  Max=200  Volt",   // 250 - Brake Drop Voltage
  "Min=0.0 Max=5.0  Sec",   // 251 - Brake Drop rate
  "0=DeadLevel,1=Levlng",   // 252 - Brake Drop Start
  "Min=15  Max=100  fpm",   // 253 - Recovery Speed fpm
  "Min=50 Max=300 fpm/s",   // 254 - Emergency Decel to Level Rate
  "Min=10  Max=100  fpm",   // 255 - Emergency Power Recovery Velocity
  "Min=0  Max=300  hour",   // 256 - Cycle Run time in hours
  "Min=0.0 Max=100.0  %",   // 257 - HCUp On red
  "Min=0.0 Max=100.0  %",   // 258 - HCUp Off red
  "Min=0.0 Max=100.0  %",   // 259 - HCUp On Green
  "Min=0.0 Max=100.0  %",   // 260 - HCUp Off Grn
  "Min=0.0 Max=100.0  %",   // 261 - HCUp On Blue
  "Min=0.0 Max=100.0  %",   // 262 - HCUp Off Blue
  "1fs1,rtl,emp,8@L,all",   // 263 - Door Open Light control
  "Min=0   Max=15    ix",   // 264 - HCUp On Color
  "Min=0   Max=15    ix",   // 265 - HCUp Off Color
  "Min=0   Max=15    ix",   // 266 - HC Sec Color
  "+1=Inv,+2=Flsh,+4=En",   // 267 - HC Sec Lt
  "Min=0.0 Max=100.0  %",   // 268 - HC Sec brightness
  "Min=0.0 Max=100.0  %",   // 269 - HC Sec On red
  "Min=0.0 Max=100.0  %",   // 270 - HC Sec On green
  "Min=0.0 Max=100.0  %",   // 271 - HC Sec On blue
  "0=Main, 1=Alt, Floor",   // 272 - Hoistway 2 fire ret
  "Min=0.250 Max=2.000 ",   // 273 - Drive Speed Multiplier
  "  0 = 0FF  1 = ON   ",   // 274 - Advanced Pre-Torque
  "Min=0   Max=15    ix",   // 275 - CC Button On Color
  "Min=0   Max=15    ix",   // 276 - CC Button Off Color
  "Min=0.0 Max=100.0  %",   // 277 - CC output on brightness
  "Min=0.0 Max=100.0  %",   // 278 - CC output off brightness
  "Min=0.0 Max=100.0  %",   // 279 - CC On red intensity
  "Min=0.0 Max=100.0  %",   // 280 - CC Off red intensity
  "Min=0.0 Max=100.0  %",   // 281 - CC On green intensity
  "Min=0.0 Max=100.0  %",   // 282 - CC Off green intensity
  "Min=0.0 Max=100.0  %",   // 283 - CC On blue intensity
  "Min=0.0 Max=100.0  %",   // 284 - CC Off blue intensity
  "1=Flsh Sec,2=ATT Seq",   // 285 - CC Button Light Control
  "Min=0   Max=15    ix",   // 286 - CC Button Security Color
  "Min=0.0 Max=100.0  %",   // 287 - CC Security brightness
  "Min=0.0 Max=100.0  %",   // 288 - CC Security red intensity
  "Min=0.0 Max=100.0  %",   // 289 - CC Security green intensity
  "Min=0.0 Max=100.0  %",   // 290 - CC Security blue intensity
  "1=IR,Lby Dwel,LbyOff",   // 291 - Sabbath Enable variabletimer, 4= Lobby off 
  "Mn=0.0 Mx=600.0 Vrms",   // 292 - Low Line Voltage (RMS value)
  "Mn=0.0 Mx=600.0 Vrms",   // 293 - Low Door Voltage (RMS value)
  "Min=10 Mx=TopSpd fpm",   // 294 - Emergency Power top speed
  "Min=50 Mx=480 fpm/ss",   // 295 - Emergency Power Soft Start Jerk Rate
  "Min=50 Max=300 fpm/s",   // 296 - Emergency Power Acceleration
  "Min=50 Mx=480 fpm/ss",   // 297 - Emergency Power Roll Over Jerk
  "Min=50 Mx=480 fpm/ss",   // 298 - Emergency Power Decel Jerk
  "Min=50 Max=300 fpm/s",   // 299 - Emergency Power Decel Rate
  "Min=1.2 Max=25.0  in",   // 300 - Emergency Power Targ Dis
  "Min=25 Mx=TopSpd fpm",   // 301 - Top Speed
  "0=ISER, 1=Ins Speed ",   // 302 - Inspection ISER
  "Min=1.0 Max=60.0 sec",   // 303 - EP Recover Timeout
  "1=FL Off Not Select ",   // 304 - Fire Light control during EP
  "0=Stop,1=Recll,2=Seq",   // 305 - EP Manual Sel En
  "1=FL Off Can't Recll",   // 306 - Fire Light control OTS
  "Min=10.0 Mx=60.0 Sec",   // 307 - Sabbath Door Dwell Time
  "Min=1.0 Max=10.0 Sec",   // 308 - Sabbath Door Buzzer timer b4 DC
  "Min=0.0 Max=101.0  %",   // 309 - Low Battery Capacity Level
  "1=PI,+2=Lant,4=Arrow",   // 310 - Sabbath disable variable
  "  0 = ON   1 = OFF  ",   // 311 - Electric Eye cancel door time
  "0=Immediate, 1=delay",   // 312 - EP @ Rcl FL W/DOtime-delay
  "0=Immediate, 1=delay",   // 313 - EP NOT @Rcl FL DOafter time-delay
  "0=DrClosed,1=DoorOpn",   // 314 - Asgn Park Fl DO
  "Min=1.0 Max=15.0 Sec",   // 315 - Hall Lantern Delay
  "1=LbyOn,2=CLD,4=NoSi",   // 316 - Sabbath Enable control 2
  " 1=DoorOpening,2=DPM",   // 317 - CLant Ctl
  "0=Dis,Max=3200.0 Sec",   // 318 - Max Door Hold time from ED
  "  0 = 0FF  1 = ON   ",   // 319 - Access run door close
  "Min=0   Max=15    ix",   // 320 - Car Call Button Att Up Color
  "Min=0.0 Max=100.0  %",   // 321 - Car Call Att Up brightness
  "Min=0.0 Max=100.0  %",   // 322 - Car Call Att Up red intensity
  "Min=0.0 Max=100.0  %",   // 323 - Car Call Att Up green intensity
  "Min=0.0 Max=100.0  %",   // 324 - Car Call Att Up blue intensity
  "Min=0   Max=15    ix",   // 325 - Car Call Button Att Dn Color
  "Min=0.0 Max=100.0  %",   // 326 - Car Call Att Dn brightness
  "Min=0.0 Max=100.0  %",   // 327 - Car Call Att Dn red intensity
  "Min=0.0 Max=100.0  %",   // 328 - Car Call Att Dn green intensity
  "Min=0.0 Max=100.0  %",   // 329 - Car Call Att Dn blue intensity
  "Min=0   Max=15    ix",   // 330 - Fire light Color
  "Min=0.0 Max=100.0  %",   // 331 - Fire light brightness
  "Min=0.0 Max=100.0  %",   // 332 - Fire light red intensity
  "Min=0.0 Max=100.0  %",   // 333 - Fire light green intensity
  "Min=0.0 Max=100.0  %",   // 334 - Fire light blue intensity
  "Min=0   Max=15    ix",   // 335 - Medical light Color
  "Min=0.0 Max=100.0  %",   // 336 - Medical light brightness
  "Min=0.0 Max=100.0  %",   // 337 - Medical light red intensity
  "Min=0.0 Max=100.0  %",   // 338 - Medical light green intensity
  "Min=0.0 Max=100.0  %",   // 339 - Medical light blue intensity
  "Min=0   Max=15    ix",   // 340 - Emergency light Color
  "Min=0.0 Max=100.0  %",   // 341 - Emergency light brightness
  "Min=0.0 Max=100.0  %",   // 342 - Emergency light red intensity
  "Min=0.0 Max=100.0  %",   // 343 - Emergency light green intensity
  "Min=0.0 Max=100.0  %",   // 344 - Emergency light blue intensity
  "Min=0   Max=15    ix",   // 345 - OTS light Color
  "Min=0.0 Max=100.0  %",   // 346 - OTS light brightness
  "Min=0.0 Max=100.0  %",   // 347 - OTS light red intensity
  "Min=0.0 Max=100.0  %",   // 348 - OTS light green intensity
  "Min=0.0 Max=100.0  %",   // 349 - OTS light blue intensity
  "Min=0   Max=63      ",   // 350 - Backlight Output lightsOTS,Bit4:Att,Bit5:non-cc
  "Min=0   Max=7 1/4sec",   // 351 - Can Sync Count 1/4sec
  "Min=0   Max=15    ix",   // 352 - HCDn On color
  "Min=0.0 Max=100.0  %",   // 353 - HCDn On brightness
  "Min=0.0 Max=100.0  %",   // 354 - HCDn On red intensity
  "Min=0.0 Max=100.0  %",   // 355 - HCDn On green intensity
  "Min=0.0 Max=100.0  %",   // 356 - HCDn On blue intensity
  "Min=0   Max=15    ix",   // 357 - HCDn Off color
  "Min=0.0 Max=100.0  %",   // 358 - HCDn Off brightness
  "Min=0.0 Max=100.0  %",   // 359 - HCDn Off red intensity
  "Min=0.0 Max=100.0  %",   // 360 - HCDn Off green intensity
  "Min=0.0 Max=100.0  %",   // 361 - HCDn Off blue intensity
  "Min=0   Max=15    ix",   // 362 - HC CB On color
  "Min=0.0 Max=100.0  %",   // 363 - HC CB On brightness
  "Min=0.0 Max=100.0  %",   // 364 - HC CB On red intensity
  "Min=0.0 Max=100.0  %",   // 365 - HC CB On green intensity
  "Min=0.0 Max=100.0  %",   // 366 - HC CB On blue intensity
  "Min=0   Max=15    ix",   // 367 - HC Vip On color
  "Min=0.0 Max=100.0  %",   // 368 - HC Vip On brightness
  "Min=0.0 Max=100.0  %",   // 369 - HC Vip On red intensity
  "Min=0.0 Max=100.0  %",   // 370 - HC Vip On green intensity
  "Min=0.0 Max=100.0  %",   // 371 - HC Vip On blue intensity
  "Min=0   Max=15    ix",   // 372 - HCIR Up On color
  "Min=0.0 Max=100.0  %",   // 373 - HCIR Up On brightness
  "Min=0.0 Max=100.0  %",   // 374 - HCIR Up On red intensity
  "Min=0.0 Max=100.0  %",   // 375 - HCIR Up On green intensity
  "Min=0.0 Max=100.0  %",   // 376 - HCIR Up On blue intensity
  "Min=0   Max=15    ix",   // 377 - HCIR Dn On color
  "Min=0.0 Max=100.0  %",   // 378 - HCIR Dn On brightness
  "Min=0.0 Max=100.0  %",   // 379 - HCIR Dn On red intensity
  "Min=0.0 Max=100.0  %",   // 380 - HCIR Dn On green intensity
  "Min=0.0 Max=100.0  %",   // 381 - HCIR Dn On blue intensity
  "Min=0   Max=15    ix",   // 382 - HCIR Up Off color
  "Min=0.0 Max=100.0  %",   // 383 - HCIR Up Off brightness
  "Min=0.0 Max=100.0  %",   // 384 - HCIR Up Off red intensity
  "Min=0.0 Max=100.0  %",   // 385 - HCIR Up Off green intensity
  "Min=0.0 Max=100.0  %",   // 386 - HCIR Up Off blue intensity
  "Min=0   Max=15    ix",   // 387 - HCIR Dn Off color
  "Min=0.0 Max=100.0  %",   // 388 - HCIR Dn Off brightness
  "Min=0.0 Max=100.0  %",   // 389 - HCIR Dn Off red intensity
  "Min=0.0 Max=100.0  %",   // 390 - HCIR Dn Off green intensity
  "Min=0.0 Max=100.0  %",   // 391 - HCIR Dn Off blue intensity
  "Min=0.0 Max=100.0  %",   // 392 - HCUp On brightness
  "Min=0.0 Max=100.0  %",   // 393 - HCUp On brightness
  " 1=On HC,  2=On IR  ",   // 394 - CB Button Loc
  " 1=On HC,  2=On IR  ",   // 395 - Vip Location
  "0=IR,1=HC IR Not On ",   // 396 - IR light color control
  "Min=0   Max=15    ix",   // 397 - HC CB Off color
  "Min=0.0 Max=100.0  %",   // 398 - HC CB Off brightness
  "Min=0.0 Max=100.0  %",   // 399 - HC CB Off red intensity
  "Min=0.0 Max=100.0  %",   // 400 - HC CB Off green intensity
  "Min=0.0 Max=100.0  %",   // 401 - HC CB Off blue intensity
  "Min=0   Max=15    ix",   // 402 - HC Vip Off color
  "Min=0.0 Max=100.0  %",   // 403 - HC Vip Off brightness
  "Min=0.0 Max=100.0  %",   // 404 - HC Vip Off red intensity
  "Min=0.0 Max=100.0  %",   // 405 - HC Vip Off green intensity
  "Min=0.0 Max=100.0  %",   // 406 - HC Vip Off blue intensity
  "0=No,+1=Cab,+2=Hall ",   // 407 - Vip lantern control
  "0=Specl Serv,1=KeySw",   // 408 - Group CC Override Ctrl
  "Min=0  Max=Last  Flt",   // 409 - Exclusion fault 1
  "Min=0  Max=Last  Flt",   // 410 - Exclusion fault 2
  "Min=0  Max=Last  Flt",   // 411 - Exclusion fault 3
  "Min=0  Max=Last  Flt",   // 412 - Exclusion fault 4
  "Min=0  Max=Last  Flt",   // 413 - Exclusion fault 5
  "Min=0  Max=Last  Flt",   // 414 - Exclusion fault 6
  "Min=20  Max=400 Volt",   // 415 - Em Bk Pick voltage
  "Min=20  Max=400 Volt",   // 416 - Em Bk hold voltage
  "Min=0.0  Max=2.0 Sec",   // 417 - Em Bk Pick Rate
  "Min=0.0  Max=1.5 Sec",   // 418 - Em Bk Drop Rate
  "Min=80 Max=300  Volt",   // 419 - Em Bk L-L Voltage
  "Mn=0 Mx=7372 3686/ms",   // 420 - Em Bk Opto Delay
  "0=115.2,1=57.6K Baud",   // 421 - Can Baud Rate
  "0=Norm,1=No Fwd Call",   // 422 - Arrival Lantern
  "Min=0 Mx=#Cars  car#",   // 423 - Code Blue SR Car
  "0=No, 1=IR Car CB En",   // 424 - Code blue over IR car
  "Min=0   Max=60   Sec",   // 425 - IR Car CB penalty
  "0=IN Sec,1=UNSecured",   // 426 - In security Output invert
  "1=ATU/D,+2=ATU,+4=CC",   // 427 - Attendent manual dir enable
  "Min=1   Max=5   10ms",   // 428 - Encoder Debounce Interval
  "Min=2   Max=10     #",   // 429 - Encoder number of samples
  "Min=.1  Max=2.0  Sec",   // 430 - Handicap buzzer on CC beep time
  "0=Norm,1=Service UPS",   // 431 - Service UPS mode
  "1=Dbg,2=Galcm,3=DL20",   // 432 - Com 1 Port sel
  "0-3=19.2,38.4,57.6, ",   // 433 - User 2 Baud Rate
  "Min=0 Mx=0xFFFF Bits",   // 434 - Cpu timing output control
  "Min=0 Max=30 Svc Num",   // 435 - Service msg 3 display
  "1=up,2=dn,4=ur,8=dnr",   // 436 - HC Asgn Secure Type
  "0=2.4,1=4.8,2=9.6,  ",   // 437 - UPS baud rate
  "0=Dis 1=Buzz Once/HC",   // 438 - HC Acknowledge ATT Buzzer
  "Min=0 Max=#Cars car#",   // 439 - Second Code Blue Car
  "1=DC,2=HEOFL,4=AuRst",   // 440 - Hall Elevator Off options 2timer 
  "  0 = OFF  1 = ON   ",   // 441 - Code blue Buzzer
  "  0 = OFF  1 = ON   ",   // 442 - Code Blue Override Independent
  "  0 = OFF  1 = ON   ",   // 443 - Code Blue Recall any
  "  +1=IND   +2=ATT   ",   // 444 - HEOF override independent
  "0=deflt,1=reOpn Stop",   // 445 - NYC FS2 Door Reopen on STP
  "Min=0.0  Max=5.0 Sec",   // 446 - Pwl brake on time
  "Min=0.0  Max=5.0 Sec",   // 447 - Pwl brake off time
  "  0 = OFF  1 = ON   ",   // 448 - Select/Prioritize ATT
  "  0 = CW   1 = CCW  ",   // 449 - Encoder Direction
  "0=OFF,1=ON (Cyc Pwr)",   // 450 - EMP recalled output en
  "0=OS,NCall,BTFL,4=SS",   // 451 - OSER control 1
  "Min=Bot  Max=Top Flr",   // 452 - Extra door time Location
  "0=OS,1=OSERL OutCtl1",   // 453 - OSER control 2
  "Min=0   Max=600  Sec",   // 454 - Timer for OSERL no asnwering call
  "Min=0   Max=600  Sec",   // 455 - Timer for OSERL car between floors
  "Min=0   Max=600  Sec",   // 456 - Timer for OSERL Safety String Open
  "0=same HW 1=diff HW ",   // 457 - Hoistway 2 fire loc
  "Min=0.5 Max=5.5  Sec",   // 458 - Retiring cam drop fail safe delay
  "Min=50  Max=300  fpm",   // 459 - Velocity difference
  "0=Enabled 1=Disabled",   // 460 - Velocity difference disable
  "0=Enabled 1=Disabled",   // 461 - SPB direction fault disable
  "0=Enabled 1=Disabled",   // 462 - Limit dir fault disable
  "0=Enabled 1=Disabled",   // 463 - Encoder dir fault disable
  "0=Enabled 1=Disabled",   // 464 - UL/DL dir fault disable
  "Min=0.0 Max=2.0  Sec",   // 465 - Relevel Start Velocity Hold Time
  "0=Stp,1=CPrs,2=Momen",   // 466 - Med door reopen
  "0=Imm,1=Delayed,2=No",   // 467 - Med Ind override
  "0=std  1= CB -> EMS ",   // 468 - Code Blue Bypass Disable
  "0=no   1= DH in PI  ",   // 469 - Extended Door PI Message
  "Min=0 Max=#Cars car#",   // 470 - 1st Recall Car
  "Min=0 Max=#Cars car#",   // 471 - 1st EP Run Car
  "0=dis, 1=enable RCF ",   // 472 - RCM Cam out for freight
  "Min=0.0 Max=3.0  Sec",   // 473 - Lantern preference change delay
  "Min=50  Max=300  fpm",   // 474 - Slip Velocity difference fault
  "  0 = Off  1 = ON   ",   // 475 - DOB overrides DCB on phase 2
  "  0 = Off  1 = ON   ",   // 476 - Denver-Cl door after f1 recl
  "Min=1.0 Max=90.0 Sec",   // 477 - Denver FS phase 1 dwell time
  " 0=std, 1=both, 2=SR",   // 478 - HC Security ctrl
  "0=Enabled,1=Disabled",   // 479 - Slip Velocity disable
  "0=Drop Run,1=Set EBK",   // 480 - Leveling fault trip
  "0=On Start, 1=On Run",   // 481 - Emergency Brake Switch Controlbrake switch on start
  "0=HC only,1=HC + CC ",   // 482 - Attendant Buzzer control
  "Min=6.0 Max=48.0  in",   // 483 - Radial position indicator
  " 0=Off    1=Enable  ",   // 484 - Recall in ind service
  "Min=10.0 Mx=60.0 Sec",   // 485 - Recall in ind service tmr
  "Mn=0.0 Mx=3000.0 rpm",   // 486 - Motor RPM
  "0=one,1=multi calls ",   // 487 - VIP Multiple Calls enable
  "Min=0 Max=30 Svc Num",   // 488 - Service Output Control
  "0=250k,1=125k   Baud",   // 489 - Can Encoder Baud Rate
  "Min=1  Max=127 Node#",   // 490 - Can Encoder Node ID
  "0=Disabled,1=Enabled",   // 491 - Code Blue allow single call
  "   0 = Dn  1 = Up   ",   // 492 - Sabbath collective mode
  "0=Frnt,1=Rear,2=Both",   // 493 - Return to lobby door open type
  "0=Not NCU/IR 1=Alwys",   // 494 - Ring the lantern in IR/NCU
  "0=dis  Max=Top Floor",   // 495 - High Priority Fl Number
  "Min=6.0 Mx=254.0 Sec",   // 496 - High Priority Fl Time
  "0=Run,1=RclRun,2=Rcl",   // 497 - EPS Sel
  "Min=0   Max=75   fpm",   // 498 - Access Speed
  "0=Disabled,1=Enabled",   // 499 - Auto service time-out
  " Mn=10 Mx=3200.0 Sec",   // 500 - Auto service time-out Time
  "0=Disabled,1=Enabled",   // 501 - Car call lockouts on sabbath
  "0=Disabled,1=Enabled",   // 502 - Med svc override car call sec
  "Min=0 Max=3200.0 Sec",   // 503 - HEOF Auto reset time
  "0=Disabled,1=Enabled",   // 504 - CCPBS on group car call sec
  "ANDed bits ul/dl    ",   // 505 - UL/DL bits anded 0011: ul = UL & UL1, dl = DL & DL1 (G4 Only)
  "Min=0.0 Max=120.0 in",   // 506 - Access UT distance in inches (G4 Only)
  "Min=0.0 Max=120.0 in",   // 507 - Access DT distance in inches (G4 Only)
  "1=Dbg,2=Galcm,3=DL20",   // 508 - Com 2 Port sel
  "0=dis,   1=No Std HC",   // 509 - Third Riser Control
  "0-3=19.2,38.4,57.6  ",   // 510 - 485 port baud rate
  " Min=0  Max=4    Mfg",   // 511 - Can Bus Encoder Mfg Type
  " 0=Standard, 1=LCD  ",   // 512 - KebDisplay type
  " 0=Single, 1=3 Phase",   // 513 - Emergency Brake Phase Input (G4 Only)
  " 0=Single, 1=3 Phase",   // 514 - Brake Phase Input (G4 Only)
  "0.00 to 1.300  %Dist",   // 515 - Distance Feed Forward (G4 Only)
  " 1=Down Dir,2=Up Dir",   // 516 - Second riser CC Security by dir
  "0=Disabled,1=Enabled",   // 517 - Minimum short door time enable
  "0=Any Flr,+1=Rcl Flr",   // 518 - Recall Reset 2
  "   0 = N/O  1 = N/C ",   // 519 - Invert hall call security input (G4 Only)
  " 0=DZ, 1=DZ and DZA ",   // 520 - DZ Gripper Trip Ctl
  "Min=10 Mx=3200.0 Sec",   // 521 - Security Disable time
  "ANDed bits ul/dl    ",   // 522 - Deadzone sensor bit selection (G4 Only)
  "Min=0.0 Max=2.0  Sec",   // 523 - EE Test Time
  "+1=F as R, +2=R as F",   // 524 - Reassign CC's when on security
  "1=F,+2=R,4=FIR,+8=RI",   // 525 - Disable opposite hc
  " Min=10 Mx=30.0  Sec",   // 526 - Disable opposite hc tim
  "+1=Activate On DorCl",   // 527 - Hall Elevator Off options 3
  "+1=RC+2=DO,+4=CLF=DC",   // 528 - Car  Elevator Off options
  "+1=DC, ELOO, Aut Rst",   // 529 - Car  Elevator Off options 2
  "1=CB before FS Recll",   // 530 - Code Blue over Fire Service
  "+1=U-C,D-C+4=U-R,D-R",   // 531 - COP/Remote Disable Select
  "1=wait for CC in FS2",   // 532 - Fire Service 2 with special dev
  "Min=0  Max=10.0  Sec",   // 533 - Auto Swing Door Open Delay Sec
  "Min=0  Max=320   Sec",   // 534 - CL Pulse Time
};

const char LCD_Field_Vars[lastfvar+1][21] = {
  " Y Delta Time=      ",   // 000 - Y Delta Time
  "   Fault Time=      ",	// 001 - Fault Time
  "   Reset Time=      ",	// 002 - Reset Time
  "Double Stroke=      ",	// 003 - Double Stroke
  " Lant On Time=      ",	// 004 - Lant On Time
  "Lant Off Time=      ",	// 005 - Lant Off Time
  "Pas Chime Tim=      ",	// 006 - Pas Chime Time
  "Door Fail Tim=      ",	// 007 - Door Fail Time
  "Nudging Time =      ",	// 008 - Nudging Time
  "Preopen Delay=      ",	// 009 - Preopen Delay
  "     CC Dwell=      ",	// 010 - Car Call Dwell
  "     HC Dwell=      ",	// 011 - Hall Call Dwell
  "  Lobby Dwell=      ",	// 012 - Lobby Dwell
  "Handicap Dwll=      ",	// 013 - Handicap Dwell
  "NonInterfer T=      ",	// 014 - Non Interfer T
  "  Lobby Floor=      ",	// 015 - Lobby Floor
  "Fire Main Flr=      ",	// 016 - Fire Main Floor
  " ALT Fire Flr=      ",	// 017 - ALT Fire Floor
  "   Stall Time=      ",	// 018 - Stall Time
  "Soft Stop Tim=      ",	// 019 - Soft Stop Time
  "  Lt/Fan Time=      ",	// 020 - Gen/Lt/Fan Time
  " Brk Pick Del=      ",	// 021 - Brake Pick Del
  " Brk Drop Del=      ",	// 022 - Brake Drop Del
  "SoftStart Jrk=      ",	// 023 - Soft Start Jerk
  " Acceleration=      ",	// 024 - Acceleration
  "   Decel Jerk=      ",	// 025 - Decel Jerk
  "   Decel Rate=      ",	// 026 - Decel Rate
  " Flr Targ Dis=      ",	// 027 - Floor Targ Dis
  " Leveling Spd=      ",	// 028 - Leveling Speed
  "Relevel Speed=      ",	// 029 - Relevel Speed
  "Inspect Speed=      ",	// 030 - Inspect Speed
  "         Year=      ",	// 031 - Year
  "        Month=      ",	// 032 - Month
  "          Day=      ",	// 033 - Day
  "         Hour=      ",	// 034 - Hour
  "       Minute=      ",	// 035 - Minute
  "RollOver Jerk=      ",	// 036 - Roll Over Jerk
  "EE MemoryType=      ",	// 037 - EE Memory Type
  "SafeTest Year=      ",	// 038 - Safe Test Year
  "SafeTst Month=      ",	// 039 - Safe Test Month
  "Safe Test Day=      ",	// 040 - Safe Test Day
  "Pattern Delay=      ",	// 041 - Pattern Delay
  " Field Weaken=      ",	// 042 - Field Weaken
  "  Encoder PPR=      ",	// 043 - Encoder PPR
  "  Encoder RPM=      ",	// 044 - Encoder RPM
  "    COM1 Baud=      ",	// 045 - User Baud Rate
  "      Parking=      ",	// 046 - Parking
  " Em Pwr Floor=      ",	// 047 - Em Power Floor
  "Em Power Cars=      ",	// 048 - Em Power Cars
  "1stRecall Car=      ",	// 049 - 1st Recall Car
  "1stEP Run Car=      ",	// 050 - 1st EP Run Car
  "Recall Timout=      ",	// 051 - Recall Timeout
  "DOB Over Nudg=      ",   // 052 - DOB Over Nudg
  "Emer Dispatch=      ",   // 053 - Emerg Dispatch
  "Lobby Request=      ",   // 054 - Lobby Request
  "  Next Car Up=      ",   // 055 - Next Car Up
  "Code Blue Car=      ",  	// 056 - Code Blue Car
  "       IR Car=      ",	// 057 - IR Car
  "Brake Lift Sw=      ",	// 058 - Brake Lift Sw
  "LW Anti-nuisn=      ",	// 059 - LW Anti-nuisan
  "  Fire Sw Loc=      ", 	// 060 - Fire Sw Loc
  " ETA Min Time=      ",	// 061 - ETA Min Time
  "ETA Co CC Tim=      ",	// 062 - ETA Co CC Time
  "Vid Pos Car 1=      ",	// 063 - Vid Pos Car 1
  "Vid Pos Car 2=      ",	// 064 - Vid Pos Car 2
  "Vid Pos Car 3=      ",	// 065 - Vid Pos Car 3
  "Vid Pos Car 4=      ",	// 066 - Vid Pos Car 4
  "Vid Pos Car 5=      ",	// 067 - Vid Pos Car 5
  "Vid Pos Car 6=      ",	// 068 - Vid Pos Car 6
  "No Psg RunCnt=      ",	// 069 - No Psg Run Cnt
  " Ind Over Sec=      ",	// 070 - Ind Over Sec
  "Relev Brk Dly=      ",	// 071 - Relev Brk Delay
  "RelevBrk LowV=      ",	// 072 - Relev Brk LowV
  "UpPk Trig Tim=      ",	// 073 - Up Pk Trig Time
  "UpPk Trig Cnt=      ",	// 074 - Up Pk Trig Cnt
  "UpPk CC Count=      ",	// 075 - Up Pk CC Count
  "Up Peak Time =      ",	// 076 - Up Peak Time
  "DnPk Trig Tim=      ",	// 077 - Dn Pk Trig Time
  "DnPk Trig Cnt=      ",	// 078 - Dn Pk Trig Cnt
  "DownPeak Time=      ",	// 079 - Down Peak Time
  "Park Dly Time=      ",	// 080 - Park Delay Time
  "HC X-AssignEn=      ",	// 081 - HC X-Assign En
  "HCX-AssignETA=      ",	// 082 - HC X-Assign ETA
  " Med Em Floor=      ",	// 083 - Med Em Floor
  "Med Em Sw Loc=      ",	// 084 - Med Em Sw Loc
  "MachRm FirRet=      ", 	// 085 - MachRm Fire Ret
  "Hoistw FirRet=      ", 	// 086 - Hoistw Fire Ret
  " Recall Reset=      ",	// 087 - Recall Reset
  "StopDecelRate=      ",	// 088 - Stop Decel Rate
  "Relev Pat Dly=      ",	// 089 - Relevel Pattern Dly
  "DON Start Ctl=      ", 	// 090 - DON Start Control
  " Brk Opto Dly=      ", 	// 091 - Brake Opto Dly
  "   Invert BLS=      ",   // 092 - Invert Brake Lift SW
  "ISER Outp Ctl=      ",	// 093 - Invert ISER
  " Dispatch Car=      ",   // 094 - Dispatcher car
  "Video Tim-out=      ",   // 095 - Video time out
  "Ins Door Clos=      ",   // 096 - Door Close Outp. Ins.
  "Parking Flr 1=      ",   // 097 - Parking floor 1
  "Parking Flr 2=      ",   // 098 - Parking floor 2
  "Parking Flr 3=      ",   // 099 - Parking floor 3
  "Parking Flr 4=      ",   // 100 - Parking floor 4
  "Parking Flr 5=      ",   // 101 - Parking floor 5
  "Parking Width=      ",   // 102 - Parking width
  "Balanced Load=      ", 	// 103 - Percent balanced load
  "Torque Amount=      ", 	// 104 - Torque amount
  "  Load Bypass=      ", 	// 105 - Load Bypass percent
  "Load Antinuis=      ", 	// 106 - Load Antinuisance percent
  "Load Dispatch=      ", 	// 107 - Load Dispatch percent
  "Load Overload=      ", 	// 108 - Load Overload percent
  "   IR Control=      ", 	// 109 - IR ControlFinishing
  "Brk Pick Volt=      ", 	// 110 - Brake Pick Voltage
  "Brk Pick Time=      ", 	// 111 - Brake Pick Time
  "Brk Hold Volt=      ", 	// 112 - Brake Hold Voltage
  "Bk Relev Volt=      ", 	// 113 - Brake Relevel Volt
  "BkAC L-L Volt=      ", 	// 114 - Brake L-L Voltage
  "Bk Resistance=      ", 	// 115 - Brake Resistance
  "Att Buz Delay=      ",   // 116 - Attendant Buzzer Delay
  " Aux. Fire Sw=      ",   // 117 - Aux. Hall Fire Sw
  " Hall Fire Lt=      ",	// 118 - Hall fire light
  "COP/Remote CC=      ",	// 119 - COP/Remote CC Select
  "Door Dly Time=      ",	// 120 - Door Open or close delay time
  " Security Rcl=      ",	// 121 - Security Recall
  " DOB Over Sec=      ",   // 122 - DOB override securityCCS Rear Only.
  "ManDo Buz Dly=      ",   // 123 - Manual Door Buzzer Delay
  " Security Flr=      ",	// 124 - Security Floor
  "RC Pick Delay=      ",   // 125 - Retiring Cam pick delay timer
  "Shrt Dwll Tim=      ",	// 126 - Short dwell door time
  "   2nd IR Car=      ",	// 127 - 2nd IR Car
  "Stop At Lobby=      ", 	// 128 - Stop At lobby
  "   Invert CLF=      ",	// 129 - Invert CLF
  "   Invert TPL=      ", 	// 130 - Invert TPL
  "   Invert LPS=      ",   // 131 - Invert LPS
  "   Invert LOS=      ",   // 132 - Invert LOS
  "Nudg No Calls=      ",   // 133 - Nudge No Calls
  "  Fire Option=      ",   // 134 - Fire Option
  "Att CC frm HC=      ",	// 135 - Attendant HC sets car call
  " CB Door Time=      ",	// 136 - Code blue door open time
  "CB Req IndCar=      ",	// 137 - CB Req Ind Car
  "Flash CBLight=      ",	// 138 - Code blue light
  "HSV DoorCl CC=      ",   // 139 - HSV door close on Car call
  "EBK Rset Time=      ",	// 140 - Gripper Reset Time
  "IND DoorCl CC=      ",   // 141 - Ind Door CL CC
  "RopeComp Torq=      ",	// 142 - Torque offset when no comp ropes
  "Grip/EBK Trip=      ",   // 143 - Gripper Spb spd or overspd.
  "Single AutoPB=      ",   // 144 - Single Auto PB
  "DO No ActvDOL=      ",	// 145 - Door Open when no DOL
  "                    ",   // 146 - Previously: fvdisglt - Disable Gate and Lock Test for New York City
  "RelevStrt Spd=      ",	// 147 - Relevel Start speed
  "Em Decel Rate=      ",	// 148 - Emergency Decel Rate
  "DCB Canc Dwll=      ",	// 149 - Disable DCB cancel dwell time
  "Retrn To Lbby=      ",	// 150 - Return to lobby option
  "Vid Pos Car 7=      ",	// 151 - Vid Pos Car 7
  "Vid Pos Car 8=      ",	// 152 - Vid Pos Car 8
  " Parking Type=      ",	// 153 - Parking Type
  "Parking Flr 6=      ",   // 154 - Parking floor 6
  "Parking Flr 7=      ",   // 155 - Parking floor 7
  "Non-Simul Dor=      ",	// 156 - Non-Simultaneous Doors
  "Preopen Doors=      ",	// 157 - Preopen Doors
  "Ovrspeed Trip=      ",	// 158 - Percent Overspeed
  "Pos Count Upd=      ",	// 159 - Position Count Update
  "Pls Err Delay=      ", 	// 160 - Pulse Error Delay
  "Gripr/EBK Buz=      ",	// 161 - Gripper Trip Buzzer
  "Relev Dly Tim=      ",	// 162 - Relevel Delay Time
  "Fire Option 2=      ",   // 163 - Fire Option  2
  "ClGate NoPref=      ",   // 164 - Close gate,swing door no pref
  "Level Flt Cnt=      ",	// 165 - Leveling Fault Count
  "Torq Down Amt=      ",   // 166 - Torque Down Amt
  "Torq Ramp Tim=      ",	// 167 - Torque Ramp Control
  "  EE Time-out=      ",	// 168 - Electric Eye Time-out Time
  "OTS No HCCanc=      ",	// 169 - OTS No HC Canc
  "Nudge Dis Ctl=      ",	// 170 - Nudge Dis Cgrl
  "F1 DC Tim-out=      ",	// 171 - Phase 1 door close time-out
  "NoHC Dor Reop=      ",   // 172 - No HC Door Reop
  " Drv Rdy Flts=      ",	// 173 - Drive Rdy Faults
  "RTL Dwell Tim=      ",	// 174 - Return to Lobby Door Dwell Time
  "NCU Lant Ctrl=      ",   // 175 - Next Up Direction Lant Ctl
  "NCU Pref Ctrl=      ",	// 176 - Next Up Preference Ctl
  "2nd Riser Ctl=      ",   // 177 - Second Riser Ctrl
  "2nd Risr Lant=      ",	// 178 - SR Lantern Ctl
  "ATT Pref Time=      ",   // 179 - Attendant ETA Pref
  "HB/PI DisNoFl=      ",   // 180 - HBZ/PI Dis at non-valid fl
  "VIP Door Time=      ",	// 181 - Vip Door Time
  "VIP Operation=      ",   // 182 - Vip Operation Flags
  "Nmbr Vip Cars=      ",   // 183 - Number of Vip Cars
  "EmPwr Op Outp=      ",   // 184 - EP Op LED function
  "EmPwr Pk Outp=      ",   // 185 - EP Park LED funct
  " HEOF Control=      ",   // 186 - Hall Elevator Off options
  "Drv Baud Rate=      ",	// 187 - Drive baud rate
  " Drive Update=      ",	// 188 - Drive update rate
  " Alt Lbby Flr=      ",   // 189 - Alternate Lobby Fl
  "Alt Parkin Fl=      ",	// 190 - Alt Parking Fl
  "Lobby Req Ctl=      ",	// 191 - Lobby Req Cntrl
  "Handicap Load=      ",	// 192 - Handicap Capacity percent
  "FR DC Tim-out=      ",	// 193 - Freight door close time-out
  "Handicap Wait=      ",	// 194 - Handicap Car Wait
  "Short Fl Dist=      ",	// 195 - Short floor distance in inches
  "  RCM Control=      ",   // 196 - Retiring Cam Control
  "Rcl frm F1Alt=      ",	// 197 - Recall from FS1 Alt Floor
  "Learn HWStall=      ",	// 198 - Hoistway Learn Stall Time
  "PlsCnt Upd Er=      ", 	// 199 - Position Pulse Count Error
  "AttBuzOff Tim=      ",	// 200 - Attendant Buzzer Off Time
  "AttBuz On Tim=      ",	// 201 - Attendant Buzzer On Time
  "Brk Pick Rate=      ",   // 202 - Brake pick rate
  "BrkRelev Rate=      ",   // 203 - Brake relevel rate
  "Brk Pick Strt=      ",	// 204 - Brake Pick Start
  "BrkRelev Strt=      ",	// 205 - Brake relevel Start
  "Shrt Fl Dn SD=      ",	// 206 - Short Fl Dn SD
  "ShrtMidF DnSD=      ",	// 207 - Mid Shrt Fl Dn SD
  "Shrt Fl Up SD=      ",	// 208 - Short Fl Up SD
  "ShrtMidF UpSD=      ",	// 209 - Mid Shrt Fl Up SD
  "ElevOff RetFl=      ",	// 210 - HS Elevator Off floor
  "PI Serv Msg 1=      ", 	// 211 - Service msg 1 display
  "PI Serv Msg 2=      ",  	// 212 - Service msg 2 display
  " GOV Grip/EBK=      ", 	// 213 - GOV Gripper Trip Ctl
  "ShF SoftStJrk=      ",	// 214 - Short Floor Soft Start Jerk
  "ShF AccelRate=      ",	// 215 - Short Floor Accel Rate
  "ShrtF RollJrk=      ",	// 216 - Short Floor Roll Over Jerk
  "ShrtF DeclJrk=      ",	// 217 - Short Floor Decel Jerk
  "ShF DecelRate=      ",	// 218 - Short Floor Decel Rate
  "ShrtF TargDis=      ",	// 219 - Short Floor Targ Dis
  "ShortFl Cntrl=      ",   // 220 - Short Floor Control
  " Sec Recall 2=      ",	// 221 - Security Recall 2group recalls
  "EP Recall Dly=      ",   // 222 - EP Recall Delay
  "GrpCC Sec OvT=      ",   // 223 - Grp cc sec override tim
  "EmBk Drop Dly=      ",   // 224 - Em brake drop dly
  "EmBk Pick Tim=      ",   // 225 - Em brake pick tim
  "  HC On Brght=      ",	// 226 - HC on brightness
  "HC Off Bright=      ",   // 227 - HC off brightness
  "Access Top Fl=      ", 	// 228 - Access Top Floor
  "Access Bot Fl=      ",	// 229 - Access Bottom Floor
  "StopOn PosCnt=      ",   // 230 - Stop on position pulse
  " Sabb Restart=      ",   // 231 - Sabbath restart
  "FR Pwr DO Tim=      ",   // 232 - Freight Power Door open time
  " Auto Flt Dpy=      ",   // 233 - Automatically Display Fault
  "     Password=      ",   // 234 - Password
  "Pword Tim-out=      ",   // 235 - Password time-out
  "Behnd CC Canc=      ",	// 236 - Behind car call cancel
  "AdvDoor EnTim=      ",	// 237 - Adv de time non-simult doors
  "Grp TimerPark=      ",	// 238 - Grp Timer Parking
  "Binary Preset=      ",	// 239 - Binary preset always
  " Short Fl hsf=      ",  	// 240 - Shrt fl hsf/no hsout
  " Up Pk Contrl=      ",	// 241 - Up peak control
  " Up Peak Pool=      ",	// 242 - Up peak pool
  "DnPeak Contrl=      ",	// 243 - Down peak control
  "DownPeak Pool=      ",	// 244 - Down peak pool
  "X-Assign Cars=      ",	// 245 - HC X-Assign Cars
  " Drive Modbus=      ",	// 246 - Drive Modbus protocol
  "AltRcl FS Off=      ",   // 247 - Fire alt rcl after fs off
  "Ins Decel Stp=      ",	// 248 - Inspection Decel Stop Rate
  "EP Recovr Dir=      ",	// 249 - Emergency Power Recovery Dir
  "Brk Drop Volt=      ",	// 250 - Brake Drop Voltage
  "Brk Drop Rate=      ",   // 251 - Brake Drop rate
  "Brk Drop Strt=      ",	// 252 - Brake Drop Start
  " Recovery Spd=      ",	// 253 - Recovery Speed fpm
  " EM Decel Lev=      ",	// 254 - Emergency Decel to Level Rate
  "EP Recovr Spd=      ",	// 255 - Emergency Power Recovery Velocity
  "Run Cycle Tim=      ",	// 256 - Cycle Run time in hours
  "  HCUp On Red=      ",	// 257 - HCUp On red
  " HCUp Off Red=      ",   // 258 - HCUp Off red
  "HCUp On Green=      ",	// 259 - HCUp On Green
  "HCU Off Green=      ",	// 260 - HCUp Off Grn
  " HCUp On Blue=      ",   // 261 - HCUp On Blue
  "HCUp Off Blue=      ",	// 262 - HCUp Off Blue
  "DoorOpenL Ctl=      ",	// 263 - Door Open Light control
  "HCUp On Color=      ",	// 264 - HCUp On Color
  "HCU Off Color=      ",	// 265 - HCUp Off Color
  " HC Sec Color=      ",   // 266 - HC Sec Color
  "   HC Sec Ctl=      ",   // 267 - HC Sec Lt
  "HC Sec Bright=      ",	// 268 - HC Sec brightness
  "   HC Sec Red=      ",	// 269 - HC Sec On red
  " HC Sec Green=      ",   // 270 - HC Sec On green
  "  HC Sec Blue=      ",	// 271 - HC Sec On blue
  "HWS2 Fire Ret=      ", 	// 272 - Hoistway 2 fire ret
  " Drv Spd Mult=      ",	// 273 - Drive Speed Multiplier
  " Adv Pre-Torq=      ",	// 274 - Advanced Pre-Torque
  "  CC On Color=      ",	// 275 - CC Button On Color
  " CC Off Color=      ",	// 276 - CC Button Off Color
  " CC On Bright=      ",	// 277 - CC output on brightness
  "CC Off Bright=      ",   // 278 - CC output off brightness
  "    CC On Red=      ",	// 279 - CC On red intensity
  "   CC Off Red=      ",   // 280 - CC Off red intensity
  "  CC On Green=      ",	// 281 - CC On green intensity
  " CC Off Green=      ",	// 282 - CC Off green intensity
  "   CC On Blue=      ",   // 283 - CC On blue intensity
  "  CC Off Blue=      ",	// 284 - CC Off blue intensity
  " CC Light Ctl=      ",   // 285 - CC Button Light Control
  " CC Sec Color=      ",   // 286 - CC Button Security Color
  "CC Sec Bright=      ",	// 287 - CC Security brightness
  "   CC Sec Red=      ",	// 288 - CC Security red intensity
  " CC Sec Green=      ",   // 289 - CC Security green intensity
  "  CC Sec Blue=      ",	// 290 - CC Security blue intensity
  "  Sabb En Ctl=      ",   // 291 - Sabbath Enable variabletimer, 4= Lobby off 
  "Low Line Volt=      ",	// 292 - Low Line Voltage (RMS value)
  "Low Door Volt=      ",	// 293 - Low Door Voltage (RMS value)
  "EP Top Speed =      ",	// 294 - Emergency Power top speed
  "EP SoftSt Jrk=      ",	// 295 - Emergency Power Soft Start Jerk Rate
  "EP Accel Rate=      ",	// 296 - Emergency Power Acceleration
  " EP Roll Jerk=      ",	// 297 - Emergency Power Roll Over Jerk
  "EP Decel Jerk=      ",	// 298 - Emergency Power Decel Jerk
  "EP Decel Rate=      ",	// 299 - Emergency Power Decel Rate
  "EP Target Dis=      ",	// 300 - Emergency Power Targ Dis
  "    Top Speed=      ", 	// 301 - Top Speed
  "Insp Spd ISER=      ",	// 302 - Inspection ISER
  "EP Recovr Tim=      ",	// 303 - EP Recover Timeout
  " FireL Em Pwr=      ",	// 304 - Fire Light control during EP
  "EP Man Sel En=      ",	// 305 - EP Manual Sel En
  "FireL OTS Ret=      ",   // 306 - Fire Light control OTS
  "Sabbath Dwell=      ",   // 307 - Sabbath Door Dwell Time
  "Sabb Buzz Dly=      ",   // 308 - Sabbath Door Buzzer timer b4 DC
  "LowBat CapLev=      ",	// 309 - Low Battery Capacity Level
  " Sabb Dis Ctl=      ",	// 310 - Sabbath disable variable
  "EE Canc Dwell=      ",	// 311 - Electric Eye cancel door time
  "SkpCar@RcFLDO=      ",	// 312 - EP @ Rcl FL W/DOtime-delay
  "SkpCarN@RcFDO=      ",	// 313 - EP NOT @Rcl FL DOafter time-delay
  "Asgn ParkF DO=      ",	// 314 - Asgn Park Fl DO
  "Hall Lant Dly=      ",	// 315 - Hall Lantern Delay
  " Sabb En Ctl2=      ",	// 316 - Sabbath Enable control 2
  "Cab Lant Ctrl=      ",	// 317 - CLant Ctl
  "Mx Door Hld T=      ",	// 318 - Max Door Hold time from ED
  " Acc Door Cls=      ",	// 319 - Access run door close
  "CCAttUp Color=      ",   // 320 - Car Call Button Att Up Color
  "CCAttUpBright=      ",	// 321 - Car Call Att Up brightness
  " CC AttUp Red=      ",	// 322 - Car Call Att Up red intensity
  "CCAttUp Green=      ",   // 323 - Car Call Att Up green intensity
  "CC AttUp Blue=      ",	// 324 - Car Call Att Up blue intensity
  "CCAttDn Color=      ",   // 325 - Car Call Button Att Dn Color
  "CCAttDnBright=      ",	// 326 - Car Call Att Dn brightness
  " CC AttDn Red=      ",	// 327 - Car Call Att Dn red intensity
  "CCAttDn Green=      ",   // 328 - Car Call Att Dn green intensity
  "CC AttDn Blue=      ",	// 329 - Car Call Att Dn blue intensity
  "Fire Lt Color=      ",   // 330 - Fire light Color
  "Fir Lt Bright=      ",	// 331 - Fire light brightness
  "  Fire Lt Red=      ",	// 332 - Fire light red intensity
  "Fire Lt Green=      ",   // 333 - Fire light green intensity
  " Fire Lt Blue=      ",	// 334 - Fire light blue intensity
  " Med Lt Color=      ",   // 335 - Medical light Color
  "Med Lt Bright=      ",	// 336 - Medical light brightness
  "   Med Lt Red=      ",	// 337 - Medical light red intensity
  " Med Lt Green=      ",   // 338 - Medical light green intensity
  "  Med Lt Blue=      ",	// 339 - Medical light blue intensity
  "Emer Lt Color=      ",   // 340 - Emergency light Color
  " Em Lt Bright=      ",	// 341 - Emergency light brightness
  "  Emer Lt Red=      ",	// 342 - Emergency light red intensity
  "Emer Lt Green=      ",   // 343 - Emergency light green intensity
  " Emer Lt Blue=      ",	// 344 - Emergency light blue intensity
  " OTS Lt Color=      ",   // 345 - OTS light Color
  "OTS Lt Bright=      ",	// 346 - OTS light brightness
  "   OTS Lt Red=      ",	// 347 - OTS light red intensity
  " OTS Lt Green=      ",   // 348 - OTS light green intensity
  "  OTS Lt Blue=      ",	// 349 - OTS light blue intensity
  " Backlight Lt=      ",   // 350 - Backlight Output lightsOTS,Bit4:Att,Bit5:non-cc
  " CAN Sync Cnt=      ",	// 351 - Can Sync Count 1/4sec
  "HCDn On Color=      ",   // 352 - HCDn On color
  "HCDn OnBright=      ",	// 353 - HCDn On brightness
  "  HCDn On Red=      ",	// 354 - HCDn On red intensity
  "HCDn On Green=      ",   // 355 - HCDn On green intensity
  " HCDn On Blue=      ",	// 356 - HCDn On blue intensity
  "HCD Off Color=      ",   // 357 - HCDn Off color
  "HCD Off Brght=      ",	// 358 - HCDn Off brightness
  " HCDn Off Red=      ",	// 359 - HCDn Off red intensity
  "HCD Off Green=      ",   // 360 - HCDn Off green intensity
  "HCDn Off Blue=      ",	// 361 - HCDn Off blue intensity
  "  CB On Color=      ",   // 362 - HC CB On color
  " CB On Bright=      ",	// 363 - HC CB On brightness
  "    CB On Red=      ",	// 364 - HC CB On red intensity
  "  CB On Green=      ",   // 365 - HC CB On green intensity
  "   CB On Blue=      ",	// 366 - HC CB On blue intensity
  " Vip On Color=      ",   // 367 - HC Vip On color
  "Vip On Bright=      ",	// 368 - HC Vip On brightness
  "   Vip On Red=      ",	// 369 - HC Vip On red intensity
  " Vip On Green=      ",   // 370 - HC Vip On green intensity
  "  Vip On Blue=      ",	// 371 - HC Vip On blue intensity
  "IRUp On Color=      ",   // 372 - HCIR Up On color
  "IRU On Bright=      ",	// 373 - HCIR Up On brightness
  "  IRUp On Red=      ",	// 374 - HCIR Up On red intensity
  "IRUp On Green=      ",   // 375 - HCIR Up On green intensity
  " IRUp On Blue=      ",	// 376 - HCIR Up On blue intensity
  "IRDn On Color=      ",   // 377 - HCIR Dn On color
  "IRD On Bright=      ",	// 378 - HCIR Dn On brightness
  "  IRDn On Red=      ",	// 379 - HCIR Dn On red intensity
  "IRDn On Green=      ",   // 380 - HCIR Dn On green intensity
  " IRDn On Blue=      ",	// 381 - HCIR Dn On blue intensity
  "IRU Off Color=      ",   // 382 - HCIR Up Off color
  "IRU Off Brght=      ",	// 383 - HCIR Up Off brightness
  " IRUp Off Red=      ",	// 384 - HCIR Up Off red intensity
  "IRU Off Green=      ",   // 385 - HCIR Up Off green intensity
  "IRUp Off Blue=      ",	// 386 - HCIR Up Off blue intensity
  "IRD Off Color=      ",   // 387 - HCIR Dn Off color
  "IRD Off Brght=      ",	// 388 - HCIR Dn Off brightness
  " IRDn Off Red=      ",	// 389 - HCIR Dn Off red intensity
  "IRD Off Green=      ",   // 390 - HCIR Dn Off green intensity
  "IRDn Off Blue=      ",	// 391 - HCIR Dn Off blue intensity
  "HCU On Bright=      ",	// 392 - HCUp On brightness
  "HCU Off Brght=      ",	// 393 - HCUp On brightness
  "CB Button Loc=      ",	// 394 - CB Button Loc
  "Vip Buttn Loc=      ",	// 395 - Vip Location
  "IR Color Ctrl=      ",	// 396 - IR light color control
  " CB Off Color=      ",   // 397 - HC CB Off color
  "CB Off Bright=      ",	// 398 - HC CB Off brightness
  "   CB Off Red=      ",	// 399 - HC CB Off red intensity
  " CB Off Green=      ",   // 400 - HC CB Off green intensity
  "  CB Off Blue=      ",	// 401 - HC CB Off blue intensity
  "Vip Off Color=      ",   // 402 - HC Vip Off color
  "Vip Off Brght=      ",	// 403 - HC Vip Off brightness
  "  Vip Off Red=      ",	// 404 - HC Vip Off red intensity
  "Vip Off Green=      ",   // 405 - HC Vip Off green intensity
  " Vip Off Blue=      ",	// 406 - HC Vip Off blue intensity
  "Vip Lant Ctrl=      ",	// 407 - Vip lantern control
  "GrpCC Ovrride=      ",	// 408 - Group CC Override Ctrl
  "ExclusionFLT1=      ",	// 409 - Exclusion fault 1
  "ExclusionFLT2=      ",	// 410 - Exclusion fault 2
  "ExclusionFLT3=      ",	// 411 - Exclusion fault 3
  "ExclusionFLT4=      ",	// 412 - Exclusion fault 4
  "ExclusionFLT5=      ",	// 413 - Exclusion fault 5
  "ExclusionFLT6=      ",	// 414 - Exclusion fault 6
  "EmBk PickVolt=      ",	// 415 - Em Bk Pick voltage
  "EmBk HoldVolt=      ",	// 416 - Em Bk hold voltage
  "EmBk PickRate=      ",	// 417 - Em Bk Pick Rate
  "EmBk DropRate=      ",	// 418 - Em Bk Drop Rate
  " EmBk L-L VAC=      ", 	// 419 - Em Bk L-L Voltage
  "EmBk Opto Dly=      ", 	// 420 - Em Bk Opto Delay
  "CAN Baud Rate=      ",	// 421 - Can Baud Rate
  " Arrival Lant=      ",   // 422 - Arrival Lantern
  "CB SRiser Car=      ",  	// 423 - Code Blue SR Car
  "CB Sel IR Car=      ",	// 424 - Code blue over IR car
  "CB IR Penalty=      ",   // 425 - IR Car CB penalty
  "INSEC Out Ctl=      ",   // 426 - In security Output invert
  "Manual Dir En=      ",   // 427 - Attendent manual dir enable
  "Encr Interval=      ",   // 428 - Encoder Debounce Interval
  "  Encr Sample=      ",	// 429 - Encoder number of samples
  "Chime on CC T=      ",	// 430 - Handicap buzzer on CC beep time
  "  Service UPS=      ",   // 431 - Service UPS mode
  "COM1 Port Sel=      ",	// 432 - Com 1 Port sel
  "    COM2 Baud=      ",	// 433 - User 2 Baud Rate
  "Cpu Timg Outp=      ",	// 434 - Cpu timing output control
  "PI Serv Msg 3=      ", 	// 435 - Service msg 3 display
  "HCasg SecType=      ",	// 436 - HC Asgn Secure Type
  "UPS/COM3 Baud=      ",	// 437 - UPS baud rate
  "   HC Ack Buz=      ",	// 438 - HC Acknowledge ATT Buzzer
  "     CB Car 2=      ",  	// 439 - Second Code Blue Car
  " HEOF Cntrl 2=      ",   // 440 - Hall Elevator Off options 2timer 
  " CB Buzz ctrl=      ",	// 441 - Code blue Buzzer
  "  CB Over Ind=      ",	// 442 - Code Blue Override Independent
  "CB Rcl anycar=      ",	// 443 - Code Blue Recall any
  "HEOF Override=      ",	// 444 - HEOF override
  "FS2 Do Reopen=      ",	// 445 - NYC FS2 Door Reopen on STP
  "Pwl Bk On Tim=      ",	// 446 - Pwl brake on time
  "Pwl Bk OffTim=      ",	// 447 - Pwl brake off time
  "EP ATTcar 1st=      ",	// 448 - Select/Prioritize ATT
  "  Encoder Dir=      ",	// 449 - Encoder Direction
  "EP Rcl Out En=      ",	// 450 - EMP recalled output en
  "OSERL OutCtl1=      ",	// 451 - OSER control 1
  "Handcap T Flr=      ",	// 452 - Extra door time Location
  "OSERL OutCtl2=      ",	// 453 - OSER control 2
  "OSER NoCall T=      ",	// 454 - Timer for OSERL no asnwering call
  "OSER BtwFlr T=      ",	// 455 - Timer for OSERL car between floors
  "OSER SSopen T=      ",	// 456 - Timer for OSERL Safety String Open
  "HWS2 Fire Loc=      ",	// 457 - Hoistway 2 fire loc
  "RCDrop Fail T=      ",	// 458 - Retiring cam drop fail safe delay
  "Velocity Diff=      ",	// 459 - Velocity difference
  " Vel Diff Dis=      ",	// 460 - Velocity difference disable
  "SP Dir FltDis=      ",	// 461 - SPB direction fault disable
  "LimDir FltDis=      ",	// 462 - Limit dir fault disable
  "EncDir FltDis=      ",	// 463 - Encoder dir fault disable
  "ULDLDirFltDis=      ",	// 464 - UL/DL dir fault disable
  "Relev St Time=      ",   // 465 - Relevel Start Velocity Hold Time
  "MedDoorReopen=      ",	// 466 - Med door reopen
  "MedInd Ovrrid=      ",	// 467 - Med Ind override
  "EMS/HSafterCB=      ",	// 468 - Code Blue Bypass Disable
  " DoorHold Msg=      ",	// 469 - Extended Door PI Message
  "1st Rcl EPSF2=      ",	// 470 - 1st Recall Car
  "1st Run EPSF2=      ",	// 471 - 1st EP Run Car
  "RCF Output En=      ",	// 472 - RCM Cam out for freight
  "Lant Pref Dly=      ", 	// 473 - Lantern preference change delay
  "Slip Vel Diff=      ",	// 474 - Slip Velocity difference fault
  "F2DOB ovr DCB=      ",	// 475 - DOB overrides DCB on phase 2
  "ClDoor F1 Rcl=      ",	// 476 - Denver-Cl door after f1 recl
  "F1 Door Dwell=      ",	// 477 - Denver FS phase 1 dwell time
  "HC Secur ctrl=      ", 	// 478 - HC Security ctrl
  " Slip Det Dis=      ",	// 479 - Slip Velocity disable
  " Leveling Flt=      ",	// 480 - Leveling fault trip
  "  EM Brake Sw=      ",	// 481 - Emergency Brake Switch Controlbrake switch on start
  " Att Buz ctrl=      ",	// 482 - Attendant Buzzer control
  "  Rad Pos Ind=      ",	// 483 - Radial position indicator
  "IND Rcl 2 Lby=      ",	// 484 - Recall in ind service
  "IND Rcl2Lby T=      ",	// 485 - Recall in ind service tmr
  "    Motor RPM=      ",	// 486 - Motor RPM
  "VIP multicall=      ",	// 487 - VIP Multiple Calls enable
  "Svc Light Ctl=      ",	// 488 - Service Output Control
  " Encoder Baud=      ",	// 489 - Can Encoder Baud Rate
  "   Enc NodeID=      ",	// 490 - Can Encoder Node ID
  "CB SingleCall=      ",	// 491 - Code Blue allow single call
  " Sabbath Mode=      ",	// 492 - Sabbath collective mode
  " RTL Door Sel=      ",	// 493 - Return to lobby door open type
  "LbyLan NCU/IR=      ",	// 494 - Ring the lantern in IR/NCU
  "Priorty Floor=      ",	// 495 - High Priority Fl Number
  "Priorty Flr T=      ",	// 496 - High Priority Fl Time
  "EPS Sel NoGrp=      ",	// 497 - EPS Sel
  " Access Speed=      ",	// 498 - Access Speed
  "AutoSVC T-out=      ",	// 499 - Auto service time-out
  "AutoSVC tot T=      ",	// 500 - Auto service time-out Time
  "  CCS on Sabb=      ",	// 501 - Car call lockouts on sabbath
  "Med CCS Ovrrd=      ",	// 502 - Med svc override car call sec
  "HEOF AutoRstT=      ",	// 503 - HEOF Auto reset time
  "CCPBS Grp Sec=      ",	// 504 - CCPBS on group car call sec
  "UL DeadZn Sel=      ",	// 505 - UL/DL bits anded 0011: ul = UL & UL1, dl = DL & DL1 (G4 Only)
  "  Acc UT Dist=      ",	// 506 - Access UT distance in inches (G4 Only)
  "  Acc DT Dist=      ",	// 507 - Access DT distance in inches (G4 Only)
  "COM2 Port Sel=      ",	// 508 - Com 2 Port sel
  " 3rd Risr Ctl=      ",   // 509 - Third Riser Control
  "RS485COM Baud=      ",	// 510 - 485 port baud rate
  " Encoder Type=      ",	// 511 - Can Bus Encoder Mfg Type
  " KEB Dpy Type=      ",	// 512 - KebDisplay type
  "Brk Phase Inp=      ", 	// 513 - Emergency Brake Phase Input (G4 Only)
  "EmBk Phase In=      ", 	// 514 - Distance Feed Forward (G4 Only)
  "Dist Feed Fwd=      ",	// 515 - Brake Phase Input (G4 Only)
  " SR CCSec Dir=      ",	// 516 - Second riser CC Security by dir
  "Min Door T En=      ",	// 517 - Minimum short door time enable
  "Recall Reset2=      ",	// 518 - Recall Reset 2
  "Invert HC Sec=      ",	// 519 - Invert hall call security input (G4 Only)
  "  DZ grip ctl=      ",	// 520 - DZ Gripper Trip Ctl
  " Sec Dis Time=      ",	// 521 - Security Disable time
  "DL DeadZn Sel=      ",	// 522 - Deadzone sensor bit selection (G4 Only)
  " EE Test Time=      ", 	// 523 - EE Test Time
  "Sec ReasignCC=      ", 	// 524 - Reassign CC's when on security
  "Disabl Opp HC=      ",	// 525 - Disable opposite hc
  " Dis Opp HC T=      ",	// 526 - Disable opposite hc tim
  " HEOF Cntrl 3=      ",   // 527 - Hall Elevator Off options 3
  " CEOF Control=      ",   // 528 - Car  Elevator Off options
  " CEOF Cntrl 2=      ",   // 529 - Car  Elevator Off options 2
  "   CB over FS=      ",   // 530 - Code Blue over Fire Service
  "   COP/RM Dis=      ",	// 531 - COP/Remote Disable Select
  "F2 STP Recovr=      ",	// 532 - Fire Service 2 with special dev
  "AutoSwg DODly=      ",	// 533 - Auto Swing Door Open Delay Sec
  "CL Pulse Time=      ",	// 534 - CL Pulse Time
};

#pragma section all_types

#define Num_Fvars_Menus 13
uint16 grp_fvars_chksum[Num_Fvars_Menus];


#if ((Traction == 1) || (Tract_OL == 1))
  #if (Traction == 1)
	#if (Tract_HR == 1)
/*
												 Car Motion
												 |   Car Brake
												 |	 |   Modified Motion
												 |	 |   |   Car Timers
												 |	 |	 |   |   Car Options
												 |	 |	 |	 |   |   Service Options
												 |	 |	 |	 |   |   |   Fire & Medical Services
												 |	 |	 |	 |   |   | 	 |   Group Dispatch
												 |	 |	 |	 |	 |	 |   |   |   Group Options
												 |	 |	 |	 |	 |	 |   |   |	 |   cc/cop pb
												 |	 |	 |	 |	 |	 |	 |   |   |   |   hc/ir pb
												 |	 |	 |	 |	 |	 |	 |   |   |   |   |   cb/vip/sec pb
												 |	 |	 |	 |	 |	 |	 |   |   |   |	 |	 |   System Options
											  	 |	 |	 |	 |	 |	 |	 |	 |	 |	 |	 |	 |	 |					*/
unsigned char const last_var [Num_Fvars_Menus] = {46, 29, 15, 48, 50, 50, 35, 37, 54, 46, 42, 25, 34};	// Traction last index value 
	#else													
unsigned char const last_var [Num_Fvars_Menus] = {46, 29, 15, 48, 50, 50, 35, 37, 54, 46, 42, 25, 34};	// Traction last index value 
	#endif
  #else
unsigned char const last_var [Num_Fvars_Menus] = {9, 29,  5, 48, 44, 50, 35, 37, 54, 46, 42, 25, 34};	// Traction NDF last index value
  #endif
#else
unsigned char const last_var [Num_Fvars_Menus] = {8,  0,  5, 46, 35, 45, 34, 37, 54, 46, 42, 25, 23};	// Hydro last index value  
#endif	

#define Fvars_Per_Menu 60
uint16 const var_menu_tbl[Num_Fvars_Menus][Fvars_Per_Menu] = {
#if ((Traction == 1) || (Tract_OL == 1))
  #if (Traction == 1)
// Car Speed Profile Variables for traction - Car Motion
{
30,  // 030 - Inspect Speed     					30  C	0
248, // 248 - Inspection Decel Stop Rate			248 C   1
498, // 498 - Access Speed                          498 C   2
41,  // 041 - Pattern Delay     					41  C	3
21,  // 021 - Brake Pick Del    					21  C	4
23,  // 023 - Soft Start Jerk   					23  C	5
24,  // 024 - Acceleration      					24  C	6
36,  // 036 - Roll Over Jerk    					36  C	7
301, // 301 - Top Speed								301 C   8
25,  // 025 - Decel Jerk        					25  C	9
26,  // 026 - Decel Rate        					26  C	10
27,  // 027 - Floor Targ Dis    					27  C	11
515, // 515 - Distance Feed Forward					515 C   12
28,  // 028 - Leveling Speed    					28  C	13
88,  // 088 - Stop Decel Rate   					88  C	14
19,  // 019 - Soft Stop Time    					19  C	15
22,  // 022 - Brake Drop Del    					22  C	16
89,  // 089 - Relevel Pattern Dly  					89  C   17
29,  // 029 - Relevel Speed     					29  C	18
147, // 147 - Relevel Start speed  					147 C	19
465, // 465 - Relevel Start Velocity Hold Time		465 C 	20
148, // 148 - Emergency Decel Rate 					148 C	21
253, // 253 - Recovery Speed fpm					253 C   22
254, // 254 - Emergency Decel to Level Rate			254 C   23
158, // 158 - Percent Overspeed    					158 C   24
90,  // 090 - DON Start Control						90  C	25
42,  // 042 - Field Weaken      					42  C	26
9,	 // 009 - Preopen Delay 	 					9   C	27
43,  // 043 - Encoder PPR       					43  C	28
44,  // 044 - Encoder RPM      						44  C	29
486, // 486 - Motor RPM      						486 C	30
449, // 449 - Encoder Direction						449 C	31
273, // 273 - Drive Speed Multiplier				273 C   32
103, // 103 - Percent balanced load					103 C   33
104, // 104 - Torque amount	   						104 C   34
166, // 166 - Torque Down Amt    					166 C   35
142, // 142 - Torque offset when no comp ropes     	142 C   36
167, // 167 - Torque Ramp Control 					167 C   37
274, // 274 - Advanced Pre-Torque					274 C	38
159, // 159 - Position Count Update 				159 C   39
160, // 160 - Pulse Error Delay 					160 C 	40
199, // 199 - Position Pulse Count Error			199 C   41
230, // 230 - Stop on position pulse				230 C   42
505, // 505 - UL Deadzone sensor bit selection		505 C 	43
522, // 522 - DL Deadzone sensor bit selection		522 C 	44
506, // 506 - Access UT distance in inches			506 C 	45
507, // 507 - Access DT distance in inches			507 C 	46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
  #else
// Car Speed Profile Variables for traction	NDF - Car Motion
{
30,  // 030 - Inspect Speed     					30  C	0
41,  // 041 - Pattern Delay     					41  C	1
21,  // 021 - Brake Pick Del    					21  C	2
19,  // 019 - Soft Stop Time    					19  C   3
22,  // 022 - Brake Drop Del    					22  C	4 
9,	 // 009 - Preopen Delay 	 					9   C	5
505, // 505 - UL Deadzone sensor bit selection		505 C 	6
522, // 522 - DL Deadzone sensor bit selection		522 C 	7
506, // 506 - Access UT distance in inches			506 C 	8
507, // 507 - Access DT distance in inches			507 C 	9
219, // Unused 10
219, // Unused 11
219, // Unused 12
219, // Unused 13
219, // Unused 14
219, // Unused 15
219, // Unused 16
219, // Unused 17
219, // Unused 18
219, // Unused 19
219, // Unused 20
219, // Unused 21
219, // Unused 22
219, // Unused 23
219, // Unused 24
219, // Unused 25
219, // Unused 26
219, // Unused 27
219, // Unused 28
219, // Unused 29
219, // Unused 30
219, // Unused 31
219, // Unused 32
219, // Unused 33
219, // Unused 34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
  #endif
#else
// Car Speed Profile Variables for Hydro - Car Motion
{
30,  // 030 - Inspect Speed     					30 	C	0
19,  // 019 - Soft Stop Time    					19 	C	1
9,	 // 009 - Preopen Delay 	 					9 	C	2
301, // 301 - Top Speed								301 C   3
505, // 505 - UL Deadzone sensor bit selection		505 C 	4
522, // 522 - DL Deadzone sensor bit selection		522 C 	5
506, // 506 - Access UT distance in inches			506 C 	6
507, // 507 - Access DT distance in inches			507 C 	7
230, // 230 - Stop on position pulse				230 C   8
219, // Unused 9 
219, // Unused 10
219, // Unused 11
219, // Unused 12
219, // Unused 13
219, // Unused 14
219, // Unused 15
219, // Unused 16
219, // Unused 17
219, // Unused 18
219, // Unused 19
219, // Unused 20
219, // Unused 21
219, // Unused 22
219, // Unused 23
219, // Unused 24
219, // Unused 25
219, // Unused 26
219, // Unused 27
219, // Unused 28
219, // Unused 29
219, // Unused 30
219, // Unused 31
219, // Unused 32
219, // Unused 33
219, // Unused 34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#endif

#if ((Traction == 1) || (Tract_OL == 1))
// Brake parameters
{
21,  // 021 - Brake Pick Del    	21  C	0
204, // 204 - Brake Pick Start 		204 C 	1
110, // 110 - Brake Pick Voltage  	110 C 	2
202, // 202 - Brake pick rate 		202 C 	3
111, // 111 - Brake Pick Time   	111 C 	4
112, // 112 - Brake Hold Voltage  	112 C 	5
22,  // 022 - Brake Drop Del    	22  C	6 
250, // 250 - Brake Drop Voltage  	250	C	7
251, // 251 - Brake Drop rate   	251	C	8
252, // 252 - Brake Drop Start		252 C   9
114, // 114 - Brake L-L Voltage		114 C	10
513, // 513 - BRK Phase Input		513 C   11
115, // 115 - Brake Resistance   	115 C 	12
71,  // 071 - Relev Brk Delay   	71  C 	13 
205, // 205 - Brake relevel Start 	205 C 	14
113, // 113 - Brake Relevel Volt  	113 C 	15
203, // 203 - Brake relevel rate 	203 C   16
72,  // 072 - Relev Brk LowV    	72  C 	17
91,  // 091 - Brake Opto Dly		91  C   18
415, // 415 - Em Bk Pick voltage 	415 C	19
417, // 417 - Em Bk Pick Rate	   	417 C	20
225, // 225 - Em brake pick tim  	225 C   21
416, // 416 - Em Bk hold voltage 	416 C   22
224, // 224 - Em brake drop dly  	224 C   23
418, // 418 - Em Bk Drop Rate	   	418 C	24
419, // 419 - Em Bk L-L Voltage  	419 C 	25
514, // 514 - EBK Phase Input		514 C   26
420, // 420 - Em Bk Opto Delay   	420 C 	27
446, // 446 - Pwl brake on time  	446	C   28
447, // 447 - Pwl brake off time 	447 C 	29
219, // Unused 30
219, // Unused 31
219, // Unused 32
219, // Unused 33
219, // Unused 34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#else
// Brake parameters (not used for hydro but must be in table)
{
219, // Unused 0  
219, // Unused 1 
219, // Unused 2 
219, // Unused 3  
219, // Unused 4 
219, // Unused 5 
219, // Unused 6 
219, // Unused 7 
219, // Unused 8 
219, // Unused 9 
219, // Unused 10
219, // Unused 11
219, // Unused 12
219, // Unused 13
219, // Unused 14
219, // Unused 15
219, // Unused 16
219, // Unused 17
219, // Unused 18
219, // Unused 19
219, // Unused 20
219, // Unused 21
219, // Unused 22
219, // Unused 23
219, // Unused 24
219, // Unused 25
219, // Unused 26
219, // Unused 27
219, // Unused 28
219, // Unused 29
219, // Unused 30
219, // Unused 31
219, // Unused 32
219, // Unused 33
219, // Unused 34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#endif

#if ((Traction == 1) || (Tract_OL == 1))
  #if (Traction == 1)
// Car Modified Motion Variables for traction
{
195, // 195 - Short floor distance in inches	 	195 C	0
214, // 214 - Short Floor Soft Start Jerk			214 C   1
215, // 215 - Short Floor Accel Rate				215 C   2
216, // 216 - Short Floor Roll Over Jerk  			216 C	3
217, // 217 - Short Floor Decel Jerk        		217 C	4
218, // 218 - Short Floor Decel Rate				218 C	5
219, // 219 - Short Floor Targ Dis 					219 C	6
220, // 220 - Short Floor Control					220 C   7
255, // 255 - Emergency Power Recovery Velocity		255 C   8
294, // 294 - Emergency Power top speed		   		294 C   9
295, // 295 - Emergency Power Soft Start Jerk Rate	295 C   10
296, // 296 - Emergency Power Acceleration 	   		296 C	11
297, // 297 - Emergency Power Roll Over Jerk   	   	297 C	12
298, // 298 - Emergency Power Decel Jerk	   		298 C	13
299, // 299 - Emergency Power Decel Rate	   		299 C	14
300, // 300 - Emergency Power Targ Dis				300 C 	15
219, // Unused 16				
219, // Unused 17				
219, // Unused 18			
219, // Unused 19
219, // Unused 20
219, // Unused 21
219, // Unused 22
219, // Unused 23
219, // Unused 24
219, // Unused 25
219, // Unused 26
219, // Unused 27
219, // Unused 28
219, // Unused 29		
219, // Unused 30		
219, // Unused 31		
219, // Unused 32		
219, // Unused 33		
219, // Unused 34		
219, // Unused 35		
219, // Unused 36		
219, // Unused 37		
219, // Unused 38		
219, // Unused 39   	
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
  #else
// Car Modified Motion Variables for traction	NDF
{
208, // 208 - Short Fl Up SD     		208 C   0
206, // 206 - Short Fl Dn SD     		206 C   1
209, // 209 - Mid Shrt Fl Up SD  		209 C   2
207, // 207 - Mid Shrt Fl Dn SD  		207 C   3
220, // 220 - Short Floor Ctrl   		220 C   4
240, // 240 - Shrt fl hsf/no hsout 		240 C   5
219, // Unused 6
219, // Unused 7
219, // Unused 8
219, // Unused 9
219, // Unused 10
219, // Unused 11
219, // Unused 12
219, // Unused 13
219, // Unused 14
219, // Unused 15
219, // Unused 16
219, // Unused 17
219, // Unused 18
219, // Unused 19
219, // Unused 20
219, // Unused 21
219, // Unused 22
219, // Unused 23
219, // Unused 24
219, // Unused 25
219, // Unused 26
219, // Unused 27
219, // Unused 28
219, // Unused 29
219, // Unused 30
219, // Unused 31
219, // Unused 32
219, // Unused 33
219, // Unused 34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
  #endif
#else
// Car Modified Motion Variables for Hydro
{
208, // 208 - Short Fl Up SD     		208 C   0
206, // 206 - Short Fl Dn SD     		206 C   1
209, // 209 - Mid Shrt Fl Up SD  		209 C   2
207, // 207 - Mid Shrt Fl Dn SD  		207 C   3
220, // 220 - Short Floor Ctl    		220 C   4
240, // 240 - Shrt fl hsf/no hsout 		240 C   5
219, // Unused 6 
219, // Unused 7
219, // Unused 8
219, // Unused 9 
219, // Unused 10
219, // Unused 11
219, // Unused 12
219, // Unused 13
219, // Unused 14
219, // Unused 15
219, // Unused 16
219, // Unused 17
219, // Unused 18
219, // Unused 19
219, // Unused 20
219, // Unused 21
219, // Unused 22
219, // Unused 23
219, // Unused 24
219, // Unused 25
219, // Unused 26
219, // Unused 27
219, // Unused 28
219, // Unused 29
219, // Unused 30
219, // Unused 31
219, // Unused 32
219, // Unused 33
219, // Unused 34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#endif

#if ((Traction == 1) || (Tract_OL == 1))

// Car Timers
{
0,	 // 000 - Y Delta Time  						0 C		 0
1,	 // 001 - Fault Time 	  						1 C		 1
2,	 // 002 - Reset Time 	  						2 C		 2
4,	 // 004 - Lant On Time 	  						4 C		 3
5,	 // 005 - Lant Off Time 	  					5 C		 4
6,	 // 006 - Pas Chime Time 	  					6 C		 5
534, // 534 - CL Pulse Time							534 C	 6
430, // 430 - Handicap buzzer on CC beep time		430 C	 7
7,	 // 007 - Door Fail Time 	  					7 C		 8
8,	 // 008 - Nudging Time 	  						8 C		 9
10,	 // 010 - Car Call Dwell 	  					10 C	 10 
11,	 // 011 - Hall Call Dwell	  					11 C	 11
12,	 // 012 - Lobby Dwell 	  						12 C	 12
13,	 // 013 - Handicap Dwell 	  					13 C	 13
126, // 126 - Short dwell door time 				126 C 	 14
174, // 174 - Return to Lobby Door Dwell Time		174 C 	 15
136, // 136 - Code blue door open time  			136 C 	 16
14,	 // 014 - Non Interfer T 	  					14 C	 17
18,  // 018 - Stall Time 							18 C	 18
198, // 198 - Hoistway Learn Stall Time				198 C	 19
20,  // 020 - Gen/Lt/Fan Time 						20 C	 20
116, // 116 - Attendant Buzzer Delay 				116 c 	 21
200, // 200 - Attendant Buzzer Off Time				200 C	 22
201, // 201 - Attendant Buzzer On Time				201 C	 23
120, // 120 - Door Open or close delay time 		120 C 	 24
123, // 123 - Manual Door Buzzer Delay  			123 C    25
125, // 125 - Retiring Cam pick delay timer 		125 C    26
458, // 458 - Retiring cam drop fail safe delay   	458 C	 27
140, // 140 - Gripper Reset Time 					140 C    28
162, // 162 - Relevel Delay Time 					162 C    29
168, // 168 - Electric Eye Time-out Time 			168 C    30
523, // 523 - EE Test Time							523 C	 31
171, // 171 - Phase 1 door close time-out		 	171 C    32
181, // 181 - Vip Door Time                       	181 C    33
193, // 193 - Freight door close time-out			193 C	 34
232, // 232 - Freight Power Door open time			232 C    35		
237, // 237 - Adv de time non-simult doors			237 C    36
256, // 256 - Cycle Run time in hours				256 C    37
307, // 307 - Sabbath Door Dwell Time				307 C	 38
308, // 308 - Sabbath Door Buzzer timer b4 DC		308 C	 39
315, // 315 - Hall Lantern Delay					315 C	 40
318, // 318 - Max Door Hold time from ED  0 = dis	318 C    41
454, // 454 - Timer for OSERL no asnwering call   	454 C	 42
455, // 455 - Timer for OSERL car between floors  	455 C	 43
456, // 456 - Timer for OSERL Safety String Open  	456 C	 44
503, // 503 - HEOF Auto reset time					503 C	 45
485, // 485 - Recall in ind service tmr				485 C 	 46
521, // 521 - Security disable time					521 C	 47
533, // 533 - Auto Swing Door Open Delay Sec		533 C	 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#else
// Car Timers
{
0,	 // 000 - Y Delta Time  						0   C	 0
1,	 // 001 - Fault Time 	  						1   C	 1
2,	 // 002 - Reset Time 	  						2   C	 2
4,	 // 004 - Lant On Time 	  						4   C	 3
5,	 // 005 - Lant Off Time 	  					5   C	 4
6,	 // 006 - Pas Chime Time 	  					6   C	 5
534, // 534 - CL Pulse Time							534 C	 6
430, // 430 - Handicap buzzer on CC beep time     	430	C	 7
7,	 // 007 - Door Fail Time 	  					7   C	 8
8,	 // 008 - Nudging Time 	  						8   C	 9
10,	 // 010 - Car Call Dwell 	  					10  C	 10 
11,	 // 011 - Hall Call Dwell	  					11  C	 11
12,	 // 012 - Lobby Dwell 	  						12  C	 12
13,	 // 013 - Handicap Dwell 	  					13  C	 13
126, // 126 - Short dwell door time 				126 C    14
174, // 174 - Return to Lobby Door Dwell Time		174 C    15
136, // 136 - Code blue door open time  			136 C    16
14,	 // 014 - Non Interfer T 	  					14  C	 17
18,  // 018 - Stall Time 							18  C	 18
20,  // 020 - Gen/Lt/Fan Time 						20  C	 19
116, // 116 - Attendant Buzzer Delay 				116 C    20
200, // 200 - Attendant Buzzer Off     				200 C	 21
201, // 201 - Attendant Buzzer On          			201 C	 22
120, // 120 - Door Open or close delay time 		120 C    23
123, // 123 - Manual Door Buzzer Delay  			123 C    24
125, // 125 - Retiring Cam pick delay timer 		125 C    25
458, // 458 - Retiring cam drop fail safe delay 	458 C    26
162, // 162 - Relevel Delay Time 					162 C    27
168, // 168 - Electric Eye Time-out Time 			168 C    28
523, // 523 - EE Test Time							523 C	 29
171, // 171 - Phase 1 door close time-out 			171 C    30
181, // 181 - Vip Door Time                     	181 C    31
193, // 193 - Freight door close time-out			193 C	 32
232, // 232 - Freight Power Door open time			232 C    33		
237, // 237 - Adv de time non-simult doors			237 C    34
256, // 256 - Cycle Run time in hours				256 C    35
307, // 307 - Sabbath Door Dwell Time				307 C	 36
308, // 308 - Sabbath Door Buzzer timer b4 DC		308 C	 37
315, // 315 - Hall Lantern Delay					315 C	 38
318, // 318 - Max Door Hold time from ED  0 = dis	318 C    39
454, // 454 - Timer for OSERL no asnwering call   	454 C	 40
455, // 455 - Timer for OSERL car between floors  	455 C	 41
456, // 456 - Timer for OSERL Safety String Open  	456 C	 42
503, // 503 - HEOF Auto reset time					503 C	 43
485, // 485 - Recall in ind service tmr				485 C	 44
521, // 521 - Security disable time					521 C	 45
533, // 533 - Auto Swing Door Open Delay Sec		533 C	 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#endif

#if ((Traction == 1) || (Tract_OL == 1))
  #if (Traction == 1)

// Car Option Variables for traction
{
58,  // 058 - Brake Lift Sw     				58  C	0
92,  // 092 - Invert Brake Lift SW 				92  C   1
481, // 481 - Emergency Brake Switch Control  	481 C   2
93,  // 093 - Invert ISER          				93  C   3
451, // 451 - OSER control 1					451 C	4
453, // 453 - OSER control 2					453 C	5
129, // 129 - Invert CLF						129	C   6
3,	 // 003 - Double Stroke 	  				3   C	7
175, // 175 - Next Up Direction Lant Ctl		175 C   8
176, // 176 - Next Up Preference Ctl			176 C   9
178, // 178 - SR Lantern Ctl, 1=SR Cab Lant   	178 C   10
317, // 317 - CLant Ctl 1=do,2=dpm        		317 C   11
422, // 422 - Arrival Lantern					422 C   12
473, // 473 - Lantern preference change delay 	473 C 	13					    
180, // 180 - HBZ/PI Dis at non-valid fl		180	C   14
52,  // 052 - DOB Over Nudg    					52  C   15
133, // 133 - Nudge No Calls     				133 C   16
170, // 170 - Nudge Dis Cgrl     				170 C   17
145, // 145 - Door Open when no DOL 			145 C   18
149, // 149 - Disable DCB cancel dwell			149 C   19
311, // 311 - Electric Eye cancel door time		311 C 	20
156, // 156 - Non-Simultaneous Doors 			156 C   21
157, // 157 - Preopen Doors 					157 C   22
164, // 164 - Close gate,swing door no pref 	164 C   23
172, // 172 - No HC Door Reop    				172 C   24  No hall call button door reopen with onward call
236, // 236 - Behind car call cancel			236 C   25
196, // 196 - Retiring Cam Control 				196 C   26
119, // 119 - COP/Remote CC Select   			119 C   27
531, // 531 - COP/Remote Disable				531 C	28
143, // 143 - Gripper Spb spd or overspd. 		143 C   29 
213, // 213 - GOV Gripper Trip Ctl				213 C 	30
520, // 520 - DZ gripper trip control			520 C	31
161, // 161 - Gripper Trip Buzzer 				161 C   32
165, // 165 - Leveling Fault Count 				165 C   33
480, // 480 - Leveling fault trip				480 C	34
239, // 239 - Binary preset always				239 C   35
173, // 173 - Drive Rdy Faults 					172 C   36 
263, // 263 - Door Open Light control			263 C   37
452, // 452 - Extra door time Location			452 C	38
459, // 459 - Velocity difference 				459 C 	39
460, // 460 - Velocity difference disable		460 C 	40
461, // 461 - SPB direction fault disable 		461 C 	41
474, // 474 - Slip Velocity difference fault	459 C   42
479, // 479 - Slip Velocity disable				479 C	43
462, // 462 - Limit dir fault disable 			462 C 	44
463, // 463 - Encoder dir fault disable			463 C 	45
464, // 464 - UL/DL dir fault disable			464 C 	46
472, // 472 - RCM Cam out for freight			472 C	47						    
483, // 483 - Radial position indicator			483 C   48
494, // 494 - Ring the lantern in IR/NCU		494 C   49
517, // 517 - Minimum short door time enable	517 C 	50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
 #else
// Car Option Variables for traction ndf
{
58,  // 058 - Brake Lift Sw     				58  C	0
92,  // 092 - Invert Brake Lift SW 				92  C   1
481, // 481 - Emergency Brake Switch Control  	481 C   2
93,  // 093 - Invert ISER          				93  C   3
451, // 451 - OSER control 1					451 C	4
453, // 453 - OSER control 2					453 C	5
129, // 129 - Invert CLF						129	C   6
3,	 // 003	- Double Stroke 	  				3   C	7
175, // 175 - Next Up Direction Lant Ctl		175 C   8
176, // 176 - Next Up Preference Ctl			176 C   9
178, // 178 - SR Lantern Ctl, 1=SR Cab Lant   	178 C   10
317, // 317 - CLant Ctl 1=do,2=dpm        		317 C   11
422, // 422 - Arrival Lantern					422 C   12
473, // 473 - Lantern preference change delay 	473 C 	13					    
180, // 180 - HBZ/PI Dis at non-valid fl		180	C   14
52,  // 052 - DOB Over Nudg    					52  C   15
133, // 133 - Nudge No Calls     				133 C   16
170, // 170 - Nudge Dis Cgrl     				170 C   17
145, // 145 - Door Open when no DOL 			145 C   18
149, // 149 - Disable DCB cancel dwell			149 C   19
311, // 311 - Electric Eye cancel door time		311 C 	20
156, // 156 - Non-Simultaneous Doors 			156 C   21
157, // 157 - Preopen Doors 					157 C   22
164, // 164 - Close gate,swing door no pref 	164 C   23
172, // 172 - No HC Door Reop    				172 C   24  No hall call button door reopen with onward call
236, // 236 - Behind car call cancel			236 C   25
196, // 196 - Retiring Cam Control 				196 C   26
119, // 119 - COP/Remote CC Select   			119 C   27
531, // 531 - COP/Remote Disable				531 C	28
143, // 143 - Gripper Spb spd or overspd. 		143 C   29 
213, // 213 - GOV Gripper Trip Ctl				213 C 	30
520, // 520 - DZ gripper trip control			520 C	31
161, // 161 - Gripper Trip Buzzer 				161 C   32
165, // 165 - Leveling Fault Count 				165 C   33
480, // 480 - Leveling fault trip				480 C	34
239, // 239 - Binary preset always				239 C   35
173, // 173 - Drive Rdy Faults 					172 C   36 
263, // 263 - Door Open Light control			263 C   37
452, // 452 - Extra door time Location			452 C	38
461, // 461 - SPB direction fault disable 		461 C 	39
462, // 462 - Limit dir fault disable 			462 C 	40
464, // 464 - UL/DL dir fault disable			464 C 	41
472, // 472 - RCM Cam out for freight			472 C	42
494, // 494 - Ring the lantern in IR/NCU	    494 C   43
517, // 517 - Minimum short door time enable	517 C	44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
  #endif
#else
// Car Option Variables for hydro
{
130, // 130 - Invert TPL						130 C   0
131, // 131 - Invert LPS          				131 C   1
132, // 132 - Invert LOS						132 C   2
93,  // 093 - Invert ISER          				93  C   3
451, // 451 - OSER control 1					451 C	4
453, // 453 - OSER control 2					453 C	5
129, // 129 - Invert CLF						129	C   6
3,	 // 003 - Double Stroke 	  				3   C	7
175, // 175 - Next Up Direction Lant Ctl		175 C   8
176, // 176 - Next Up Preference Ctl			176 C   9
178, // 178 - SR Lantern Ctl, 1=SR Cab Lant   	178 C   10
317, // 317 - CLant Ctl 1=do,2=dpm				317 C   11
422, // 422 - Arrival Lantern					422 C   12
473, // 473 - Lantern preference change delay 	473 C 	13
180, // 180 - HBZ/PI Dis at non-valid fl		180	C   14
52,  // 052 - DOB Over Nudg    					52  C   15
133, // 133 - Nudge No Calls      				133 C	16
170, // 170 - Nudge Dis Cgrl     				170 C   17
145, // 145 - Door Open when no DOL 			145 C   18
149, // 149 - Disable DCB cancel dwell time 	149 C   19
311, // 311 - Electric Eye cancel door time		311 C 	20
156, // 156 - Non-Simultaneous Doors 			156 C   21
157, // 157 - Preopen doors 					157 C   22
164, // 164 - Close gate,swing door,no pref   	164 C   23
172, // 172 - No HC Door Reop    				172 C   24  No hall call button door reopen with onward call
236, // 236 - Behind car call cancel			236 C   25
196, // 196 - Retiring Cam Control				196 C   26
119, // 119 - COP/Remote CC Select   			119 C   27
531, // 531 - COP/Remote Disable				531 C	28
239, // 239 - Binary preset always				239 C   29
173, // 173 - Drive Rdy Faults 					172 C   30 
263, // 263 - Door Open Light control			263 C   31
452, // 452 - Extra door time Location			452 C	32
472, // 472 - RCM Cam out for freight			472 C	33
494, // 494 - Ring the lantern in IR/NCU		494 C   34
517, // 517 - Minimum short door time enable	517 C	35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#endif

#if ((Traction == 1) || (Tract_OL == 1))
// Service Option Variables for traction
{
15,  // 015 - Lobby Floor 						15 C	0
150, // 150 - Return to lobby option 			150 C   1	  +1 = cycle door, +2 = cancel car calls
493, // 493 - Return to lobby door open type  	493 C	2
128, // 128 - Stop At lobby						128 C   3
53,  // 053 - Emerg Dispatch   					53 C	4
96,  // 096 - Door Close Outp. Ins.				96 C    5
70,  // 070 - Ind Over Sec     					70 C 	6
141, // 141 - Ind Door CL CC	   				141 C   7
484, // 484 - Recall in ind service				484 C	8
121, // 121 - Security Recall 					121 C   9
221, // 221 - Security Recall 2					221 C   10
124, // 124 - Security Floor 					124 C   11
426, // 426 - In security Output invert 		426 C   12
122, // 122 - DOB override security 			122 C   13
504, // 504 - CCPBS on group car call sec		504 C 	14
516, // 516 - Second riser CC Security by dir 	516 C 	15
524, // 524 - Reassign car calls on SECFM		524 C	16
135, // 135 - Attendant HC sets car call 		135 C   17
427, // 427 - Attendant manual dir enable		427 C	18
438, // 438 - HC Acknowledge ATT Buzzer			438 C	19
482, // 482 - Attendant Buzzer control			482 C   20
182, // 182 - Vip Operation Flags				182 C   21
407, // 407 - Vip lantern control				407 C 	22
487, // 487 - VIP Multiple Calls enable			487 C	23
210, // 210 - HS Elevator Off floor				210 C 	24
186, // 186 - Elevator Off options            	186 C   25
440, // 440 - Elevator Off options 2          	440 C   26
527, // 527 - Hall Elevator Off options 3		527 C	27
444, // 444 - HEOF override independent			444 C   28
528, // 528 - Car Elevator Off options			528 C	29
529, // 529 - Car Elevator Off options 2		529 C	30
228, // 228 - Access Top Floor					228 C   31
229, // 229 - Access Bottom Floor				229 C   32
319, // 319 - Access run door close				319 C   33
211, // 211 - Service msg 1 display				211 C 	34
212, // 212 - Service msg 2 display				212 C 	35
435, // 435 - Service msg 3 display				435 C 	36
469, // 469 -	Extended Door PI Message		469 C	37
488, // 488 - Service Output Control			488 C	38
501, // 501 - Car call lockouts on sabbath		501 C   39
291, // 291 - Sabbath Enable variable		    291 C 	40
316, // 316 - Sabbath Enable control 2 	    	316 C   41
310, // 310 - Sabbath disable variable			310 C   42
492, // 492 - Sabbath collective mode			492 C   43
59,  // 059 - LW Anti-nuisan   					59 C	44
69,  // 069 - No Psg Run Cnt   					69 C	45
105, // 105 - Load Bypass percent  				105 C   46
106, // 106 - Load Antinuisance percent  		106 C   47
107, // 107 - Load Dispatch percent  			106 C   48
108, // 108 - Load Overload percent  			108 C   49
192, // 192 - Handicap Capacity percent			192	C 	50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#else
// Service Option Variables for hydro
{
15,  // 015 - Lobby Floor 						15 C	0
150, // 150 - Return to lobby option			150 C   1	+1 = cycle door, +2 = cancel car calls
493, // 493 - Return to lobby door open type  	493 C	2
128, // 128 - Stop At lobby						128 C   3
53,  // 053 - Emerg Dispatch   					53 C	4
96,  // 096 - Door Close Outp. Ins.				96 C    5
70,  // 070 - Ind Over Sec     					70 C 	6
141, // 141 - Ind Door CL CC	   				141 C   7
484, // 484 - Recall in ind service				484 C	8
121, // 121 - Security Recall     				120 C   9
221, // 221 - Security Recall 2					221 C   10
124, // 124 - Security Floor 					124 C   11
426, // 426 - In security Output invert			426 C   12
122, // 122 - DOB override security 			122 C   13
504, // 504 - CCPBS on group car call sec		504 C 	14
516, // 516 - Second riser CC Security by dir 	516 C 	15
524, // 524 - Reassign car calls on SECFM		524 C	16
135, // 135 - Attendant HC sets car call 		135 C   17
427, // 427 - Attendent manual dir enable		427 C	18
438, // 438 - HC Acknowledge ATT Buzzer			438 C   19
482, // 482 - Attendant Buzzer control			482 C   20
182, // 182 - Vip Operation Flags				182 C   21
407, // 407 - Vip lantern control				407 C 	22
487, // 487 - VIP Multiple Calls enable			487 C	23
210, // 210 - HS Elevator Off floor				210 C 	24
186, // 186 - Elevator Off options          	186 C   25
440, // 440 - Elevator Off options 2         	440 C	26
527, // 527 - Hall Elevator Off options 3		527 C	27
444, // 444 - HEOF override independent			444 C   28
528, // 528 - Car  Elevator Off options			528 C	29
529, // 529 - Car  Elevator Off options 2		529 C	30
228, // 228 - Access Top Floor					228 C   31
229, // 229 - Access Bottom Floor				229 C   32
319, // 319 - Access run door close				319 C   33
211, // 211 - Service msg 1 display				211 C 	34
212, // 212 - Service msg 2 display				212 C 	35
435, // 435 - Service msg 3 display				212 C 	36
469, // 469 - Extended Door PI Message			469 C	37
488, // 488 - Service Output Control			488 C	38
501, // 501 - Car call lockouts on sabbath		501 C   39
291, // 291 - Sabbath Enable variable		    291 C 	40
316, // 316 - Sabbath Enable control 2 	    	316 C   41
310, // 310 - Sabbath disable variable			310 C   42
492, // 492 - Sabbath collective mode			492 C   43
59,  // 059 - LW Anti-nuisan   					59 C	44
69,  // 069 - No Psg Run Cnt   					69 C	45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#endif
 
#if ((Traction == 1) || (Tract_OL == 1))
// Emergency Services: Car Fire & Medical Services Variables
{
16,  // 016 - Fire Main Floor					16 C  	0
17,  // 017 - ALT Fire Floor					17 C	1
60,  // 060 - Fire Sw Loc 						60 C	2
117, // 117 - Aux. Hall Fire Sw   				117 C 	3
197, // 197 - Recall from FS1 Alt Floor			197 C	4
118, // 118 - Hall fire light     				118 C	5
85,  // 085 - MachRm Fire Ret					85 C	6
86,  // 086 - Hoistw Fire Ret					86 C	7
272, // 272 - Hoistway 2 fire ret				272 C   8
457, // 457 - Hoistway 2 fire loc				457	C	9
87,  // 087 - Recall Reset  					87 C 	10
518, // 518 - Recall Reset 2  					518 C 	11
134, // 134 - Fire Option         				134 C 	12
163, // 163 - Fire Option  2      				163 C 	13
171, // 171 - Phase 1 door close time-out 		171 C   14
247, // 247 - Fire alt rcl after fs off			247 C	15
304, // 304 - Fire Light control during EP		304 C   16
306, // 306 - Fire Light control OTS			306 C	17
475, // 475 - DOB overrides DCB on phase 2		475 C 	18
476, // 476 - Denver-Cl door after f1 recl		476 C 	19
477, // 477 - Denver FS phase 1 dwell time		477 C   20
47,  // 047 - Em Power Floor					47 G  	21	Emergency Power Return Floor
497, // 497 - EPS Sel 0=run,1=rcl/run,2=rcl		497 C   22
249, // 249 - Emergency Power Recovery Dir    	249 C	23
83,  // 083 - Med Em Floor  					83 C 	24
84,  // 084 - Med Em Sw Loc  					84 C 	25
466, // 466 - Med door reopen 					466 C   26
467, // 467 - Med Ind override					467 C   27
502, // 502 - Med svc override car call sec		502 C	28
138, // 138 - Code blue light	 				138 C   29
441, // 441 - Code blue Buzzer					441 C	30
442, // 442 - Code Blue Override Independent  	442 C   31
468, // 468 - Code Blue Bypass Disable 			468 C 	32
491, // 491 - Code Blue allow single call		491 C 	33
530, // 530 - Code Blue over Fire Service		530 C 	34
139, // 139 - HSV door close on Car call  		139 C   35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#else
// Car Fire & Medical Services Variables for hydro
{
16,  // 016 - Fire Main Floor					16 C  	0
17,  // 017 - ALT Fire Floor					17 C	1
60,  // 060 - Fire Sw Loc 						60 C	2
117, // 117 - Aux. Hall Fire Sw   				117 C 	3
197, // 197 - Recall from FS1 Alt Floor			197 C	4
118, // 118 - Hall fire light     				118 C	5
85,  // 085 - MachRm Fire Ret					85 C	6
86,  // 086 - Hoistw Fire Ret					86 C	7
272, // 272 - Hoistway 2 fire ret				272 C   8
457, // 457 - Hoistway 2 fire loc				457	C	9
87,  // 087 - Recall Reset  					87 C 	10
518, // 518 - Recall Reset 2  					518 C 	11
134, // 134 - Fire Option         				134 C 	12
163, // 163 - Fire Option  2      				163 C 	13
171, // 171 - Phase 1 door close time-out 		171 C   14
247, // 247 - Fire alt rcl after fs off			247 C	15
304, // 304 - Fire Light control during EP		304 C   16
306, // 306 - Fire Light control OTS			306 C	17
475, // 475 - DOB overrides DCB on phase 2		475 C 	18
476, // 476 - Denver-Cl door after f1 recl		476 C 	19
477, // 477 - Denver FS phase 1 dwell time		477 C   20
532, // 532 - Fire Service 2 with special dev	532 C 	21
47,  // 047 - Em Power Floor					47 G  	22	Emergency Power Return Floor
83,  // 083 - Med Em Floor  					83 C 	23
84,  // 084 - Med Em Sw Loc  					84 C 	24
466, // 466 - Med door reopen 					466 C   25
467, // 467 - Med Ind override					467 C   26
502, // 502 - Med svc override car call sec		502 C	27
138, // 138 - Code blue light	 				138 C   28
441, // 441 - Code blue Buzzer					441 C   29
442, // 442 - Code Blue Override Independent 	442 C	30
468, // 468 - Code Blue Bypass Disable 			468 C 	31
491, // 491 - Code Blue allow single call		491 C 	32
530, // 530 - Code Blue over Fire Service		530 C 	33
139, // 139 - HSV door close on Car call		139 C   34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#endif

// Group Dispatch Variables
{
46,  // 046 - Parking			 		 46 G	0
80,  // 080 - Park Delay Time			 80 G  	1 	Parking Delay time
97,  // 097 - Parking floor 1			 97 G	2 
98,  // 098 - Parking floor 2			 98 G	3 
99,  // 099 - Parking floor 3			 99 G	4 
100, // 100 - Parking floor 4	 	 	100 G	5 
101, // 101 - Parking floor 5	 	 	101 G	6 
154, // 154 - Parking floor 6	 	 	154 G	7 
155, // 155 - Parking floor 7	 	 	155 G	8 
102, // 102 - Parking width 	 	  	102 G	9 
153, // 153 - Parking Type     	  		153 G   10
190, // 190 - Alt Parking Fl    	 	190 G	11
238, // 238 - Grp Timer Parking  		238 G	12
314, // 314 - Asgn Park Fl DO    		314 G   13	Assign parking floor to car with door open
15,  // 015 - Lobby Floor 			 	15  G	14
189, // 189 - Alternate Lobby Fl 		189 G	15
54,  // 054 - Lobby Request			 	54  G	16	  
191, // 191 - Lobby Req Cntrl    		191 G	17
55,  // 055 - Next Car Up				55  G	18
73,  // 073 - Up Pk Trig Time			73  G  	19 Up peak trigger time
74,  // 074 - Up Pk Trig Cnt			74  G  	20	Up Peak trigger count
75,  // 075 - Up Pk CC Count			75  G  	21	Up Peak car call count
76,  // 076 - Up Peak Time			 	76  G  	22	Up Peak Duration time
241, // 241 - Up peak control	   		241 G	23
242, // 242 - Up peak pool       		242 G	24	Cars removed from up peak pool
77,  // 077 - Dn Pk Trig Time			 77 G  	25	Down Peak trigger time
78,  // 078 - Dn Pk Trig Cnt			 78 G  	26	Down Peak trigger count
79,  // 079 - Down Peak Time			 79 G  	27	Down Peak Duration time
243, // 243 - Down peak control  		243 G	28
244, // 244 - Down peak pool      		244 G	29	Cars removed from down peak pool
61,  // 061 - ETA Min Time			 	61  G	30
62,  // 062 - ETA Co CC Time			 62 G	31
495, // 495 - High Priority Fl Number	495	G	32
496, // 496 - High Priority Fl Time		496 G	33
499, // 499 - Auto service time-out		499 G   34
500, // 500 - Auto SVC time-out Time	500 G	35
525, // 525 - Disable opposite HC		525 G	36
526, // 526 - Disable op HC time 		526 G	37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},

// Group Option Variables
{
47,  // 047 - Em Power Floor			 47 G  	0	Emergency Power Return Floor
48,  // 048 - Em Power Cars			 	 48 G  	1 	Number of cars to run on Emergency Power
49,  // 049 - 1st Recall Car			 49 G  	2	First EP return Car
470, // 470 - 1st Recall Car			470 G 	3	First EP return Car
50,  // 050 - 1st EP Run Car			 50 G  	4	First EP Run Car 
471, // 471 - 1st EP Run Car			471 G 	5	First EP Run Car 
222, // 222 -EP Recall Delay 	   		222 G	6
51,  // 051 - Recall Timeout			 51 G  	7	Recall Car Time-out 
303, // 303 - EP Recover Timeout  		303 G   8
305, // 305 - EP Manual Sel En	   		305 G   9
312, // 312 - EP @ Rcl FL W/DO   		312 G   10	 
313, // 313 - EP NOT @Rcl FL DO  		313 G   11 
184, // 184 - EP Op LED function 		184 G   12
185, // 185 - EP Park LED funct  		185 G   13
448, // 448 - Select/Prioratize ATT		448 G	14
450, // 450 - EMP recalled output en  	450 G	15	
56,  // 056 - Code Blue Car			 	 56 G	16
439, // 439 - Second Code Blue Car		439 G	17
443, // 443 - Code Blue Recall any 		443 G	18	 
423, // 423 - Code Blue SR Car			423 G	19
137, // 137 - CB Req Ind Car     		137 G   20   Code blue request independent car 
394, // 394 - CB Button Loc 	   		394 G	21
424, // 424 - Code blue over IR car		424	G	22
425, // 425 - IR Car CB penalty 		425 G	23
57,  // 057 - IR Car 		 			 57 G	24	
127, // 127 - 2nd IR Car		   		127 G	25
109, // 109 - IR Control		   		109 G   26
63,  // 063 - Vid Pos Car 1 			 63 G	27
64,  // 064 - Vid Pos Car 2 			 64 G	28	
65,  // 065 - Vid Pos Car 3 			 65 G	29	
66,  // 066 - Vid Pos Car 4			  	 66 G	30
67,  // 067 - Vid Pos Car 5			 	 67 G	31
68,  // 068 - Vid Pos Car 6			 	 68 G	32
151, // 151 - Vid Pos Car 7	   			151 G	33
152, // 152 - Vid Pos Car 8	   			152 G	34
81,  // 081 - HC X-Assign En 			 81 G  	35	Hall call cross assignment enable flag
82,  // 082 - HC X-Assign ETA			 82 G  	36	Hall Call cross assignment ETA
245, // 245 - HC X-Assign Cars   		245 G	37  Number of cars in the old group to assign calls
94,  // 083 - Dispatcher car      		 83 G   38  Ability to change a car other than 1 or 2 to become the master
53,  // 053 - Emerg Dispatch   		 	 53 C	39
144, // 144 - Single Auto PB     		144 G   40
169, // 169 - OTS No HC Canc     		169 G   41
177, // 177 - Second Riser Ctrl  		177 G   42
179, // 179 - Attendant ETA Pref 		179 G   43
182, // 182 - Vip Op Flags 	   		 	182 G   44
183, // 183 - Number of Vip Cars 		183 G   45
395, // 395 - Vip Location       		395 G	46
194, // 194 - Handicap Car Wait  		194 G	47
223, // 223 - Grp cc sec ovr tim 		223 G	48
408, // 408 - Group CC Override Ctrl	408 G 	49
231, // 231 - Sabbath restart    		231 G	50
436, // 436 - HC Asgn Secure Type		436 G	51
478, // 478 - HC security ctrl			478 G	52
509, // 509 - Third Riser Control		509 G	53
519, // 519 - Invert HC Sec Input		519	G	54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59 
},

// Car/COP Push Button Options
{
275, // 275 - CC Button On Color				275 C 	0
277, // 277 - CC output on brightness			277 C 	1
279, // 279 - CC On red intensity				279 C 	2
281, // 281 - CC On green intensity				281 C 	3
283, // 283 - CC On blue intensity				283 C 	4
276, // 276 - CC Button Off Color				276 C 	5
278, // 278 - CC output off brightness			278 C 	6
280, // 280 - CC Off red intensity				280 C 	7
282, // 282 - CC Off green intensity			282 C 	8
284, // 284 - CC Off blue intensity				284 C 	9
286, // 286 - CC Button Security Color			286 C 	10
287, // 287 - CC Security brightness			287 C 	11
288, // 288 - CC Security red intensity			288 C 	12
289, // 289 - CC Security green intensity		289 C 	13
290, // 290 - CC Security blue intensity		290 C 	14
320, // 320 - Car Call Button Att Up Color		320 C 	15
321, // 321 - Car Call Att Up brightness	    321 C 	16
322, // 322 - Car Call Att Up red intensity		322 C 	17
323, // 323 - Car Call Att Up green intensity 	323 C 	18
324, // 324 - Car Call Att Up blue intensity  	324 C 	19
325, // 325 - Car Call Button Att Dn Color		325 C 	20
326, // 326 - Car Call Att Dn brightness	    326 C 	21
327, // 327 - Car Call Att Dn red intensity		327 C 	22
328, // 328 - Car Call Att Dn green intensity 	328 C 	23
329, // 329 - Car Call Att Dn blue intensity  	329 C 	24
285, // 285 - CC Button Light Control 			285 C 	25
330, // 330 - Fire light Color					330 C 	26
331, // 331 - Fire light brightness				331 C 	27
332, // 332 - Fire light red intensity			332 C 	28
333, // 333 - Fire light green intensity		333 C 	29
334, // 334 - Fire light blue intensity			334 C 	30
335, // 335 - Medical light Color				335 C 	31
336, // 336 - Medical light brightness			336 C 	32
337, // 337 - Medical light red intensity		337 C 	33
338, // 338 - Medical light green intensity		338 C 	34
339, // 339 - Medical light blue intensity		339 C 	35
340, // 340 - Emergency light Color				340 C 	36
341, // 341 - Emergency light brightness		341 C 	37
342, // 342 - Emergency light red intensity		342 C 	38
343, // 343 - Emergency light green intensity	343 C 	39
344, // 344 - Emergency light blue intensity	344 C 	40
345, // 345 - OTS light Color					345 C 	41
346, // 346 - OTS light brightness				346 C 	42
347, // 347 - OTS light red intensity			347 C 	43
348, // 348 - OTS light green intensity			348 C 	44
349, // 349 - OTS light blue intensity			349 C 	45
350, // 350 - Backlight Output lights  			350 C 	46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},

// HC/IR Push Button Options
{
226, // 226 - HC on brightness   				226 G   0
227, // 227 - HC off brightness  				227 G   1
264, // 264 - HCUp On Color 	   				264 G 	2
392, // 392 - HCUp On brightness				392 G 	3
257, // 257 - HCUp On red   					257 G 	4
259, // 259 - HCUp On Green 					259 G 	5
261, // 261 - HCUp On Blue  					261 G 	6
265, // 265 - HCUp Off Color 	   				265 G 	7
393, // 393 - HCUp On brightness				393 G 	8
258, // 258 - HCUp Off red  					258 G 	9
260, // 260 - HCUp Off Grn  					260 G 	10
262, // 262 - HCUp Off Blue 					262 G 	11
352, // 352 - HCDn On color	  					352 G 	12
353, // 353 - HCDn On brightness    			353 G 	13
354, // 354 - HCDn On red intensity				354 G 	14
355, // 355 - HCDn On green intensity 			355 G 	15
356, // 356 - HCDn On blue intensity			356 G 	16
357, // 357 - HCDn Off color					357	G	17
358, // 358 - HCDn Off brightness				358	G	18
359, // 359 - HCDn Off red intensity			359	G	19
360, // 360 - HCDn Off green intensity			360	G	20
361, // 361 - HCDn Off blue intensity			361	G	21
372, // 372 - HCIR Up On color					372 G 	22
373, // 373 - HCIR Up On brightness				373 G 	23
374, // 374 - HCIR Up On red intensity			374 G 	24
375, // 375 - HCIR Up On green intensity		375 G 	25
376, // 376 - HCIR Up On blue intensity			376 G 	26
382, // 382 - HCIR Up Off color					382 G   27
383, // 383 - HCIR Up Off brightness			383 G   28
384, // 384 - HCIR Up Off red intensity			384 G 	29
385, // 385 - HCIR Up Off green intensity		385 G 	30
386, // 386 - HCIR Up Off blue intensity		386 G 	31
377, // 377 - HCIR Dn On color					377 G 	32
378, // 378 - HCIR Dn On brightness				378 G 	33
379, // 379 - HCIR Dn On red intensity			379 G 	34
380, // 380 - HCIR Dn On green intensity		380 G 	35
381, // 381 - HCIR Dn On blue intensity			381 G 	36
387, // 387 - HCIR Dn Off color					387 G 	37
388, // 388 - HCIR Dn Off brightness			388 G 	38
389, // 389 - HCIR Dn Off red intensity			389 G 	39
390, // 390 - HCIR Dn Off green intensity		390 G 	40
391, // 391 - HCIR Dn Off blue intensity		391 G 	41
396, // 396 - IR light color control			396 G 	42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
// CB/VIP/SEC Push Button Options
{
362, // 362 - HC CB On color					362 G 	0
363, // 363 - HC CB On brightness				363 G 	1
364, // 364 - HC CB On red intensity			364 G 	2
365, // 365 - HC CB On green intensity			365 G 	3
366, // 366 - HC CB On blue intensity			366 G 	4
397, // 397 - HC CB Off color					397 G 	5
398, // 398 - HC CB Off brightness				398 G 	6
399, // 399 - HC CB Off red intensity			399 G 	7
400, // 400 - HC CB Off green intensity			400 G 	8
401, // 401 - HC CB Off blue intensity			401 G 	9
367, // 367 - HC Vip On color					367 G 	10
368, // 368 - HC Vip On brightness				368 G 	11
369, // 369 - HC Vip On red intensity			369 G 	12
370, // 370 - HC Vip On green intensity			370 G 	13
371, // 371 - HC Vip On blue intensity			371 G 	14
402, // 402 - HC Vip Off color					402 G 	15
403, // 403 - HC Vip Off brightness				403 G 	16
404, // 404 - HC Vip Off red intensity			404 G 	17
405, // 405 - HC Vip Off green intensity		405 G 	18
406, // 406 - HC Vip Off blue intensity			406 G 	19
266, // 266 - HC Sec Color	   					266 G 	20
268, // 268 - HC Sec brightness  				268 G   21
269, // 269 - HC Sec On red  	   				269 G 	22
270, // 270 - HC Sec On green    				270 G 	23
271, // 271 - HC Sec On blue     				271 G 	24
267, // 267 - HC Sec Lt 		   				267 G  	25
219, // Unused 26
219, // Unused 27
219, // Unused 28
219, // Unused 29
219, // Unused 30
219, // Unused 31
219, // Unused 32
219, // Unused 33
219, // Unused 34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},

#if ((Traction == 1) || (Tract_OL == 1))
// System Options
{
187, // 187 - Drive baud rate					187 C  	0
188, // 188 - Drive update rate					188 C   1 
246, // 246 - Drive Modbus protocol           	246 C   2
512, // 512 - KebDisplay type: 0=Std, 1=LCD   	512 C   3
421, // 421 - Can Baud Rate						421 C 	4
45,  // 045 - User Baud Rate   					 45 C	5
433, // 433 - User 2 Baud Rate					433 C   6
432, // 432 - Com Diag Port Sel 1				432 C   7
508, // 508 - Com Diag Port Sel 2				508 C   8
38,  // 038 - Safe Test Year   					 38 C	9
39,  // 039 - Safe Test Month  					 39 C	10
40,  // 040 - Safe Test Day   					 40 C	11
233, // 233 - Automatically Display Fault		233 C   12 
234, // 234 - Password							234 C	13
235, // 235 - Password time-out					235 C   14
95,  // 095 - Video time out					 95 C   15
309, // 309 - Low Battery Capacity Level		309 C   16
351, // 351 - Can Sync Count 1/4sec 			351 C   17
409, // 409 - Exclusion fault 1					409 C   18
410, // 410 - Exclusion fault 2					410 C   19
411, // 411 - Exclusion fault 3					411 C   20
412, // 412 - Exclusion fault 4					412 C   21
413, // 413 - Exclusion fault 5					413 C   22
414, // 414 - Exclusion fault 6					414 C   23
428, // 428 - Encoder Debounce Interval 		428 C   24
429, // 429 - Encoder number of samples			429 C   25
511, // 511 - Can Bus Encoder Mfg Type			511 C   26
489, // 489 - Can Encoder Baud Rate				489 C   27  
490, // 490 - Can Encoder Node ID             	490 C   28 
431, // 431 - Service UPS mode					431 C   29
437, // 437 - UPS baud rate						437 C   30
510, // 510 - 485 port baud rate				510 C	31
434, // 434 - Cpu timing output control			434 C   32
292, // 292 - Low Line Voltage (RMS value)		292 C 	33
293, // 293 - Low Door Voltage (RMS value)		293 C 	34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#else
// System Options
{
421, // 421 - Can Baud Rate					421 C 	0
45,  // 045 - User Baud Rate   				 45 C	1
433, // 433 - User 2 Baud Rate				433 C   2
432, // 432 - Com Diag Port					432 C   3
508, // 508 - Com Diag Port Sel 2			508 C   4
38,  // 038 - Safe Test Year   				 38 C	5
39,  // 039 - Safe Test Month  				 39 C	6
40,  // 040 - Safe Test Day      			 40 C	7 
233, // 233 - Automatically Display Fault	233 C   8
234, // 234 - Password						234 C	9
235, // 235 - Password time-out				235 C   10
95,  // 095 - Video time out				 95 C   11
351, // 351 - Can Sync Count 1/4sec 		351 C   12
409, // 409 - Exclusion fault 1				409 C   13
410, // 410 - Exclusion fault 2 			410 C   14
411, // 411 - Exclusion fault 3				411 C   15
412, // 412 - Exclusion fault 4 			412 C   16
413, // 413 - Exclusion fault 5				413 C   17
414, // 414 - Exclusion fault 6 			414 C   18
437, // 437 - UPS baud rate					437 C   19
510, // 510 - 485 port baud rate			510 C	20
434, // 434 - Cpu timing output control		434 C   21
292, // 292 - Low Line Voltage (RMS value)	292 C 	22
293, // 293 - Low Door Voltage (RMS value)	293 C 	23
219, // Unused 24
219, // Unused 25
219, // Unused 26
219, // Unused 27
219, // Unused 28
219, // Unused 29
219, // Unused 30
219, // Unused 31
219, // Unused 32
219, // Unused 33
219, // Unused 34
219, // Unused 35
219, // Unused 36
219, // Unused 37
219, // Unused 38
219, // Unused 39
219, // Unused 40
219, // Unused 41
219, // Unused 42
219, // Unused 43
219, // Unused 44
219, // Unused 45
219, // Unused 46
219, // Unused 47
219, // Unused 48
219, // Unused 49
219, // Unused 50
219, // Unused 51
219, // Unused 52
219, // Unused 53
219, // Unused 54
219, // Unused 55
219, // Unused 56
219, // Unused 57
219, // Unused 58
219, // Unused 59
},
#endif
};

#pragma section const_type

#if (GALaxy_4 == 1)
static const char SPB_Encoder_Type[5][21] = {
  "0=Standard Tape     ",
  "1=Governor Encoder  ",
  "2=Encoded Tape      ",
  "3=Governor Pulses   ",
  "4=Incremental Encode",
};
#endif

static const char Multi_Display[16][21] = {
  "0=Option 0          ",
  "1=Option 1          ",
  "2=Option 2          ",
  "3=Options 2 & 1     ",
  "4=Option 4          ",
  "5=Options 4 & 1     ",
  "6=Options 4 & 2     ",
  "7=Options 4, 2 & 1  ",
  "8=Option 8          ",
  "9=Options 8 & 1     ",
  "10=Options 8 & 2    ",
  "11=Options 8, 2 & 1 ",
  "12=Options 8 & 4    ",
  "13=Options 8, 4, & 1",
  "14=Options 8, 4, & 2",
  "15=Options 8,4,2 & 1",
};

static const char LCD_Var_Labels[16][21] = {
  "0=Use RGB Variables ",
  "       1=Red        ",
  "     2 = Orange     ",
  "     3 = Yellow     ",
  "   4 = Chartreuse   ", 
  "     5 = Green      ", 
  "   6 = Aquamarine   ",
  "      7 = Cyan      ", 
  "     8 = Azure      ",
  "      9 = Blue      ",
  "    10 = Violet     ", 
  "    11 = Magenta    ", 
  "     12 = Rose      ",  
  "  13 = Rose White   ", 
  "  14 = Warm White   ", 
  "  15 = Cool White   ",
};

static const char LCD_Button_Sec_Light[8][21] = {
  "  0=Light On Steady ",
  "  1=Invert Security ",
  "  2=Flash Security  ",
  " 3=Flash/Invert Sec ",
  " 4=Sec Master Enable",
  " 5=Master Sec En/Inv",
  "6=Mastr Sec En/Flash",
  "7=Mastr En/Flash/Inv",
};

static const char LCD_Backlight_Output[64][21] = {
  " 0=Backlight Off    ",
  " 1=Backlight Fire Lt",
  " 2=Backlight Med Lt ",
  " 3=Backlight Med/Fi ",
  " 4=Backlight Em Lt  ",
  " 5=Backlight Em/Fire",
  " 6=Backlight Emer/Md",
  " 7=Backlt Em/Med/Fi ",
  " 8=Backlight OTS Lt ",
  " 9=Backlight OTS/Fi ",
  "10=Backlight OTS/Med",
  "11=Backlt OTS/Med/Fi",
  "12=Backlight OTS/Em ",
  "13=Backlt OTS/Em/Fi ",
  "14=Backlt OTS/Em/Med",
  "15=Bcklt OS/Em/Md/Fi",
  "16=Backlight Att    ",
  "17=Backlt Att/Fire  ",
  "18=Backlt Att/Med   ",
  "19=Backlt Att/Med/Fi", 
  "20=Backlt Att/Emerg ",
  "21=Backlt Att/Em/Fi ",
  "22=Backlt Att/Em/Med",
  "23=Bklt Att/Em/Md/Fi",
  "24=Backlight Att/OTS",
  "25=Bklt Att/OTS/Fire",
  "26=Backlt Att/OS/Med",
  "27=Bklt Att/OS/Md/Fi",
  "28=Bklt Att/OTS/Emer",
  "29=Bklt Att/OS/Em/Fi",
  "30=Bklt Att/OS/Em/Md",
  "31=Att/OTS/Em/Md/Fi ",
  "32=Backlight Non-CC ",
  "33=Backlt Non-CC/Fi ",
  "34=Backlt Non-CC/Med",
  "35=Backlt NCC/Med/Fi", 
  "36=Backlt Non-CC/Em ",
  "37=Backlt NCC/Em/Fi ",
  "38=Backlt NCC/Em/Med",
  "39=Bklt NCC/Em/Md/Fi",
  "40=Backlt Non-CC/OTS",
  "41=Bklt NCC/OTS/Fire",
  "42=Backlt NCC/OS/Med",
  "43=Bklt NCC/OS/Md/Fi",
  "44=Bklt NCC/OTS/Emer",
  "45=Bklt NCC/OS/Em/Fi",
  "46=Bklt NCC/OS/Em/Md",
  "47=NCC/OTS/Em/Md/Fi ",
  "48=Backlt Non-CC/Att",
  "49=Backlt NCC/Att/Fi",
  "50=Backlt NCC/Att/Md",
  "51=Blt NCC/Att/Md/Fi", 
  "52=Backlt NCC/Att/Em",
  "53=Bklt NCC/At/Em/Fi",
  "54=Bklt NCC/At/Em/Md",
  "55=NCC/Att/Em/Md/Fi ",
  "56=Bklt NCC/Att/OTS ",
  "57=Blt NCC/Att/OS/Fi",
  "58=NCC/Att/OTS/Med  ",
  "59=NCC/Att/OTS/Md/Fi",
  "60=Blt NCC/Att/OS/Em",
  "61=NCC/Att/OTS/Em/Fi",
  "62=NCC/Att/OTS/Em/Md",
  "63=NC/At/OS/Em/Md/Fi",
};

static const char CAN_Encoder_Type[5][21] = {
  "0=Cons File Setting ",
  "1=Turck Encoder     ",
  "2=Dynapar Encoder   ",
  "3=Wachendorff Encodr",
  "4=Tape Selector Fdbk",
};

static const char UL_Dead_Zone_Sensor[16][21] = {
  " 0 =    ,   ,   ,UL1",
  " 1 =    ,   ,   ,UL1",
  " 2 =    ,   ,UL2,   ",
  " 3 =    ,   ,UL2,UL1",
  " 4 =    ,UL3,   ,   ",
  " 5 =    ,UL3,   ,UL1",
  " 6 =    ,UL3,UL2,   ",
  " 7 =    ,UL3,UL2,UL1",
  " 8 = UL4,   ,   ,   ",
  " 9 = UL4,   ,   ,UL1",
  "10 = UL4,   ,UL2,   ",
  "11 = UL4,   ,UL2,UL1",
  "12 = UL4,UL3,   ,   ",
  "13 = UL4,UL3,   ,UL1",
  "14 = UL4,UL3,UL2,   ",
  "15 = UL4,UL3,UL2,UL1",
};

static const char DL_Dead_Zone_Sensor[16][21] = {
  " 0 =    ,   ,   ,DL1",
  " 1 =    ,   ,   ,DL1",
  " 2 =    ,   ,DL2,   ",
  " 3 =    ,   ,DL2,DL1",
  " 4 =    ,DL3,   ,   ",
  " 5 =    ,DL3,   ,DL1",
  " 6 =    ,DL3,DL2,   ",
  " 7 =    ,DL3,DL2,DL1",
  " 8 = DL4,   ,   ,   ",
  " 9 = DL4,   ,   ,DL1",
  "10 = DL4,   ,DL2,   ",
  "11 = DL4,   ,DL2,DL1",
  "12 = DL4,DL3,   ,   ",
  "13 = DL4,DL3,   ,DL1",
  "14 = DL4,DL3,DL2,   ",
  "15 = DL4,DL3,DL2,DL1",
};


//				     {2400, 4800, 9600, 19.2, 38.4, 57.6, 115.2, 219254, 226562.5, 234375 baud}		

static const char Baud_Display[10][21] = {
  " 0 = 2400 Baud      ",
  " 1 = 4800 Baud      ",
  " 2 = 9600 Baud      ",
  " 3 = 19200 Baud     ",
  " 4 = 38400 Baud     ",
  " 5 = 57600 Baud     ",
  " 6 = 115200 Baud    ",
  " 7 = 219254 Baud    ",
  " 8 = 226562.5 Baud  ",
  " 9 = 234375 Baud    ",
};

static const char Drv_Baud_Display[4][21] = {
  " 0 = 19200 Baud     ",
  " 1 = 38400 Baud     ",
  " 2 = 57600 Baud     ",
  " 3 = 115200 Baud    ",
};

static const char Com_Sel_Display[8][21] = {
  "0=Comm Diag         ",
  "1=Comm Debug        ",
  "2=Galcom            ",
  "3=DL20              ",
  "4=Galcom Wireless   ",
  "5=Galcom Ethernet   ",
  "6=Galcom WiFiFlowCtl",
  "7=Galcom EthnFlowCtl",
};
static const char Xassign_Display[6][21] = {
  "0=No Cross Assign   ",
  "1=Front Cross Assign",
  "2=Cross Cancellation",
  "3=Invalid Selection ",
  "4=Rear Cross Assign ",
  "5=F & R X-Assignment",
};


static const char Fire_SW_Display[4][21] = {
  "0=Alt=Frnt,Main=Frnt",
  "1=Alt=Frnt,Main=Rear",
  "2=Alt=Rear,Main=Frnt",
  "3=Alt=Rear,Main=Rear",
};

static const char Fire_RST_Display[3][21] = {
  "0=Reset Aft Recall  ",
  "1=Reset Immediately ",
  "2=HFOff Rset FS Only",
};

static const char ISER_Display[6][21] = {
  "0=In Service Light  ",
  "1=Invert ISER output",
  "2=In Use Light      ",
  "3=Invert In Use Lt  ",
  "4=Car Shutdown      ",
  "5=Inv Car Shutdown  ",
};

static const char IR_CTL_Display[8][21] = {
  "0=IR Active ICR/RICR",
  "1=IR Active IR Calls",
  "2=Finish CCs then IR",
  "3=Both Options 2 & 1",
  "4=Fin CC then IR Off",
  "5=Both Options 4 & 1",
  "6=Both Options 4 & 2",
  "7=Options 4, 2 & 1  ",
};

static const char FSO_Display[5][21] = {
  "0=FLH Phase 1 or 2  ",
  "1=FLH Phase 1 only  ",
  "2=Flash FLH Ph1 or 2",
  "3=Flash FLH Phase 1 ",
  "4=FLH Follows FL    ",
};

static const char RMCC_Display[8][21] = {
  "0=COP & Remote CC   ",
  "1=COP or Remote CC  ",
  "2=COP sent to Remote",
  "3=COP->RM, COP or RM",
  "4=Remote sent to COP",
  "5=RM->COP, COP or RM",
  "6=COP->RM & RM->COP ",
  "7=COP->RM or RM->COP",
};

static const char SEC_RCL_Display[16][21] = {
  "0=No Recall         ",
  "1=On SEC Activation ",
  "2=Cycle Front Door  ",
  "3=On SEC,Cycle Front",
  "4=Cycle Rear Door   ",
  "5=On SEC,Cycle Rear ",
  "6=Cycle Front & Rear",
  "8=After Each Run    ",
  "9=Each Run & On SEC ",
  "10=Each Run,Cycle F ",
  "11=Each Run&SEC,CycF",
  "12=Each Run,Cycle R ",
  "13=Each Run&SEC,CycR",
  "14=Each Run,Cyc F&R ",
  "14=Ea Run&SEC,CycF&R",
};

static const char DOB_SEC_Display[6][21] = {
  "0=No DOB Override   ",
  "1=Any Secured Floor ",
  "2=Group CC Lockouts ",
  "3=Rear CC Lockouts  ",
  "4=Front CC Lockouts ",
  "5=Grp Lockout Ind Sv",
};

static const char OP_LED_Display[4][21] = {
  "0=EP Operational Car",
  "1=Normal Power Cars ",
  "2=Emerg. Power Cars ",
  "3=EP Recall Cars    ",
};

static const char PK_LED_Display[3][21] = {
  "0=Parked @ Recall FL",
  "1=Pkd@RclFL or Selec",
  "2=Pkd@RclFL & Select",
};

static const char HEOF_Display[5][21] = {
  "0=No Selection      ",
  "1=Recall Car Key Sw ",
  "2=Keep Door Open @FL",
  "4=Light/Fan Time-out",
  "8=Door Close Tim-out",
};

static const char SEC_RCL_2_Display[3][21] = {
  "0=OuTofGrp 1stRecall",
  "1=OuTofGrp All Recls",
  "2=In Grp All Recalls",
};

static const char OPENL_Display[6][21] = {
  "0=No OPENL Operation",
  "1=F1 Return DoorOpen",
  "2=Lby Retrn DoorOpen",
  "4=EP Recall DoorOpen",
  "8=Door Close Tim-out",
  "16=DoorOpen All Flrs",
};

static const char OSER_CTL_Display[4][21] = {
  "0=Out of Service Lt ",
  "1=Not Answering Call",
  "2=Stuck Between Flrs",
  "4=Safety String Open",
};

static const char SABB_EN_Display[4][21] = {
  "0=No Selectiion     ",
  "1=IR ovrride Sabbath",
  "2=Handicap Lby Dwell",
  "4=Turn Off Sabb @Lby",
};

static const char ELEV_OFF_Display[5][21] = {
  "0=No Selection      ",
  "1=Recall Car Key Sw ",
  "2=Keep Door Open @FL",
  "4=Light/Fan Time-out",
  "8=Door Close Tim-out",
};

static const char ELEV_OFF_2_Display[5][21] = {
  "0=No Selection      ",
  "1=DoorCl on Reversal",
  "2=No Flash ELO Light",
  "4=Auto Reset w/timer",
  "8=ELO Light Rcll Fin",
};

static const char RMCC_DIS_Display[5][21] = {
  "0=No Selection      ",
  "1=Up Dir Disable COP",
  "2=Dn Dir Disable COP",
  "4=Up Dir Dis RMCC   ",
  "8=Down Dir Dis RMCC ",
};

static  const char LCD_Statistics[18][21] = {
  "     Car Calls      ",
  "   Up Hall Calls    ",
  "  Down Hall Calls   ",
  " Up HCalls < 15 Sec ",
  " Up HCalls < 30 Sec ",
  " Up HCalls < 45 Sec ",
  " Up HCalls < 60 Sec ",
  " Up HCalls > 60 Sec ",
  " Dn HCalls < 15 Sec ",
  " Dn HCalls < 30 Sec ",
  " Dn HCalls < 45 Sec ",
  " Dn HCalls < 60 Sec ",
  " Dn HCalls > 60 Sec ",
  " Hall Calls < 15 Sec",
  " Hall Calls < 30 Sec",
  " Hall Calls < 45 Sec",
  " Hall Calls < 60 Sec",
  " Hall Calls > 60 Sec",
};

#define Num_Set_Calls_Menus 9
static  const char LCD_Setup_Calls[Num_Set_Calls_Menus][21] = {
  " Setup Car Calls    ",
  " Set Down Hall Calls",
  " Set Up Hall Calls  ",
  " Set Rear Car Calls ",
  " Set R Dn Hall Calls",
  " Set R Up Hall Calls",
  " Lockout F Car Calls",
  " Lockout R Car Calls",
  " Car Call Test      ",
};

static  const char LCD_Calls[8][21] = {
  "Hit Up/Dn to Change ",
  "or Enter to Set     ",
  "Car Call At Floor:  ",
  "Dn HC At Floor:     ",
  "Up HC At Floor:     ",
  "Rear CC At Floor:   ",
  "RDn HC At Floor:    ",
  "RUp HC At Floor:    ",
};

static  const char LCD_Secure_Calls[8][21] = {
  "Hit Up/Dn to Change ",
  "Floor.  * = Secured ",
  "Hit Enter to Modify.",
  "F CC Access At:     ",
  "R CC Access At:     ",
  "  Sure you want to  ",
  "secure car calls? No",
  "secure car calls?Yes",
};

static  const char LCD_Car_Call_Test[4][21] = {
  "Hit Up/Dn to change ",
  "floor.  * = Selected",
  "Hit Enter to Modify.",
  "Car Call At Flr:    ",
};

static  const char LCD_Enable_Car_Call_Test[6][21] = {
  "  Sure you want to  ",
  "enable CC test? No  ",
  "enable CC test? Yes ",
  "Do you want to cont.",
  "with prev CC test? Y",
  "with prev CC test? N",
};


const char LCD_Fire_status[9][21] = {
  "No Fire Service     ",   // firef == 0
  "Fire PH1 Main Return",   // firef == 1
  "Fire PH1 Alt Return ",   // firef == 2
  "FS1 Return Complete ",   // firef == 3
  "Fire PH2 Door Hold  ",   // firef == 4
  "F2 Constant Press DO",   // firef == 5
  "F2 Constant Press DC",   // firef == 6
  "Fire PH2 Door Hold  ",   // firef == 7
  "FS2 Momentary DClose",   // firef == 8
};

const char LCD_Fire_door[5][14] = {
  "Door Closed  ",	 // doorf == 0
  "Door Opening ",	 // doorf == 1
  "Door Open    ",	 // doorf == 2
  "Door Closing ",	 // doorf == 3
  "Door Nudging ",	 // doorf == 4
};

const char LCD_Fire_rdoor[5][14] = {
  "RDor Closed  ",	 // rdoorf == 0
  "RDor Opening ",	 // rdoorf == 1
  "RDor Open    ",	 // rdoorf == 2
  "RDor Closing ",	 // rdoorf == 3
  "RDor Nudging ",	 // rdoorf == 4
};

const char LCD_Procf[29][21] = {
  " No Procedure Flag  ",
  "     Reset Mode     ",	// procf == 1
  "  Inspection Mode   ",	// procf == 2
  "      Up Fast       ",   // procf == 3 hsf==1 && dirf==1
  "   Up Transition    ",   // procf == 4 hsf==0 && UL==0
  "    Leveling Up     ",   // procf == 5 hsf==0 && UL==1
  "     Down Fast      ",   // procf == 6 hsf==1 && dirf==2
  "  Down Transition   ",   // procf == 7 hsf==0 && DL==0
  "   Leveling Down    ",   // procf == 8 hsf==0 && DL==1
  "  Emergency Stop    ",   // procf == 9
  "     Not Used       ",   // procf == 10
  " Emergency Slowdown ",   // procf == 11
  " Safety String Open ",   // procf == 12
  " Elevator Off Line  ",   // procf == 13
  "  Elevator Parked   ",   // procf == 14
  "  Waiting At Floor  ",   // procf == 15
  "   Door Procedure   ",   // procf == 16
  "  Elevator Stalled  ",   // procf == 17
  "  Reset Hydro Jack  ",   // procf == 18
  " Low Pressure Mode  ",	// procf == 19
  " Auto Learn HW Mode ",	// procf == 20
  " Emp Recovery Mode  ",	// procf == 21
  "    Hot Oil Mode    ",	// procf == 22
  "  Brake Test Mode   ",	// procf == 23
};

const char LCD_Procf2[9][11] = {
  "Up Fast    ",   // procf == 3 hsf==1 && dirf==1
  "Up Slowdown",   // procf == 4 hsf==0 && UL==0
  "Leveling Up",   // procf == 5 hsf==0 && UL==1
  "Down Fast  ",   // procf == 6 hsf==1 && dirf==2
  "Dn Slowdown",   // procf == 7 hsf==0 && DL==0
  "Leveling Dn",   // procf == 8 hsf==0 && DL==1
  "Emerg. Stop",   // procf == 9
  "Not Used   ",   // procf == 10
  "Em Slowdown",   // procf == 11
};

const char LCD_Statusf[33][21] = {
  "     Status OK      ",
  "   S10 Input Off    ",	// statusf = 0x1
  "    HC Input Off    ",	// statusf = 0x2
  "    SS Input Off    ",	// statusf = 0x4
  "  READY Input Off   ",   // statusf = 0x8                   
  " Gripper/EBK Fault  ",   // statusf = 0x10  		** MODIFIED FOR EBK
  "     I/O Error      ",	// statusf = 0x20  
  " Insp or Byp Error  ",	// statusf = 0x40
  "  Bin. Pos. Error   ",   // statusf = 0x80                 
  "   Position Error   ",   // statusf = 0x100                
  "    AD Input Off    ",   // statusf = 0x200                
  "    CS Input Off    ",   // statusf = 0x400
  "  Door Zone Fault   ",	// statusf = 0x800
  "  Gate/Lock Fault   ",	// statusf = 0x1000
  "     P Input On     ",   // statusf = 0x2000
  "  Looking for DCL   ",   // statusf = 0x4000
  " Door Close Contact ",   // statusf = 0x8000
  " Brake Switch Fault ",	// statusf = 0x10000
  " TOC CAN Com Error  ",	// statusf = 0x20000
  "  DRIVE Com Error   ",	// statusf = 0x40000
  " Saf Proc Com Error ",   // statusf = 0x80000                   
  " DB Res. Temp. Trip ",   // statusf = 0x100000  
  " Run Fault:Shutdown ",	// statusf = 0x200000  
  " Annual Safety Test ",	// statusf = 0x400000
  "  Waiting for SAFE  ",   // statusf = 0x800000                 
  " Terminal Limit Flt ",   // statusf = 0x1000000                
  "   GTS Input Off    ",   // statusf = 0x2000000                
  " UL,DL,DZ Off at FL ",   // statusf = 0x4000000
  "  Brake CAN Error   ",	// statusf = 0x8000000
  " Fire Stop Sw. Off  ",	// statusf = 0x10000000
  " SEL CAN Com Error  ",   // statusf = 0x20000000
  "   UL or DL Fault   ",   // statusf = 0x40000000
  "   Leveling Fault   ",   // statusf = 0x80000000
};


const char LCD_Statusf2[33][21] = {
  "     Status OK      ",
  " Hardware Init Flt  ",	// statusf = 0x1
  " Front Door Cls Flt ",	// statusf = 0x2
  " Rear Door Clos Flt ",	// statusf = 0x4
  " Line Voltage Fault ",   // statusf = 0x8                   
  " Door Voltage Fault ",   // statusf = 0x10  		
  " BK Lift Sw Run Flt ",	// statusf = 0x20  
  "   Door Motor OVL   ",	// statusf = 0x40
  " Learn Hoistway Flt ",   // statusf = 0x80                 
  "     UPS Fault      ",   // statusf = 0x100                
  " Em Brake CAN Error ",   // statusf = 0x200                
  " KEB Drv Not In Run ",   // statusf = 0x400
  " At Floor Shutdown  ",	// statusf = 0x800
  " 1036 Auto Run Hold ",	// statusf = 0x1000
  " Reset Run Failure  ",   // statusf = 0x2000
  " Invalid SEL Count  ",   // statusf = 0x4000
  " COP CAN Com Error  ",   // statusf = 0x8000
  " Enc CAN Com Error  ",	// statusf = 0x10000
  " IO Redundancy Test ",	// statusf = 0x20000
  " Gripper Relay Test ",	// statusf = 0x40000
  " Drive Not Powered  ",   // statusf = 0x80000                   
  " Normal Stop Timer  ",   // statusf = 0x100000  
  " Door Open Request  ",	// statusf = 0x200000  
  "  Waiting For DPM   ",	// statusf = 0x400000
  "  Waiting For RPM   ",   // statusf = 0x800000                 
  " Viscosity Control  ",   // statusf = 0x1000000                
  "  Leveling Request  ",   // statusf = 0x2000000                
  "  Term Speed Clamp  ",   // statusf = 0x4000000
  "  Speed Rate Clamp  ",	// statusf = 0x8000000
  " Front EE Test Flt  ",	// statusf = 0x10000000
  "  Rear EE Test Flt  ",   // statusf = 0x20000000
  " Setup Mode No Auto ",   // statusf = 0x40000000
  "                    ",   // statusf = 0x80000000
};

const char LCD_Statusf3[33][25] = {
  "Status OK               ",
  "SPB Safety CAN Flt PI=  ",	// statusf = 0x1
  "APS Vel Verify Flt PI=  ",	// statusf = 0x2
  "APS Sel Board Flt  PI=  ",	// statusf = 0x4
  "APS Sel CAN Flt    PI=  ",   // statusf = 0x8                   
  "                   PI=  ",   // statusf = 0x10  		
  "                   PI=  ",	// statusf = 0x20  
  "                   PI=  ",	// statusf = 0x40
  "                   PI=  ",   // statusf = 0x80                 
  "                   PI=  ",   // statusf = 0x100                
  "                   PI=  ",   // statusf = 0x200                
  "                   PI=  ",   // statusf = 0x400
  "                   PI=  ",	// statusf = 0x800
  "                   PI=  ",	// statusf = 0x1000
  "                   PI=  ",   // statusf = 0x2000
  "                   PI=  ",   // statusf = 0x4000
  "                   PI=  ",   // statusf = 0x8000
  "                   PI=  ",	// statusf = 0x10000
  "                   PI=  ",	// statusf = 0x20000
  "                   PI=  ",	// statusf = 0x40000
  "                   PI=  ",   // statusf = 0x80000                   
  "                   PI=  ",   // statusf = 0x100000  
  "                   PI=  ",	// statusf = 0x200000  
  "                   PI=  ",	// statusf = 0x400000
  "                   PI=  ",   // statusf = 0x800000                 
  "                   PI=  ",   // statusf = 0x1000000                
  "                   PI=  ",   // statusf = 0x2000000                
  "                   PI=  ",   // statusf = 0x4000000
  "                   PI=  ",	// statusf = 0x8000000
  "                   PI=  ",	// statusf = 0x10000000
  "                   PI=  ",   // statusf = 0x20000000
  "                   PI=  ",   // statusf = 0x40000000
  "                   PI=  ",	// statusf = 0x80000000
};

const char LCD_Statusf4[33][25] = {
  "Status OK               ",
  "                   PI=  ",	// statusf = 0x1
  "                   PI=  ",	// statusf = 0x2
  "                   PI=  ",	// statusf = 0x4
  "                   PI=  ",   // statusf = 0x8                   
  "                   PI=  ",   // statusf = 0x10  		
  "                   PI=  ",	// statusf = 0x20  
  "                   PI=  ",	// statusf = 0x40
  "                   PI=  ",   // statusf = 0x80                 
  "                   PI=  ",   // statusf = 0x100                
  "                   PI=  ",   // statusf = 0x200                
  "                   PI=  ",   // statusf = 0x400
  "                   PI=  ",	// statusf = 0x800
  "                   PI=  ",	// statusf = 0x1000
  "                   PI=  ",   // statusf = 0x2000
  "                   PI=  ",   // statusf = 0x4000
  "                   PI=  ",   // statusf = 0x8000
  "                   PI=  ",	// statusf = 0x10000
  "                   PI=  ",	// statusf = 0x20000
  "                   PI=  ",	// statusf = 0x40000
  "                   PI=  ",   // statusf = 0x80000                   
  "                   PI=  ",   // statusf = 0x100000  
  "                   PI=  ",	// statusf = 0x200000  
  "                   PI=  ",	// statusf = 0x400000
  "                   PI=  ",   // statusf = 0x800000                 
  "                   PI=  ",   // statusf = 0x1000000                
  "                   PI=  ",   // statusf = 0x2000000                
  "                   PI=  ",   // statusf = 0x4000000
  "                   PI=  ",	// statusf = 0x8000000
  "                   PI=  ",	// statusf = 0x10000000
  "                   PI=  ",   // statusf = 0x20000000
  "                   PI=  ",   // statusf = 0x40000000
  "                   PI=  ",	// statusf = 0x80000000
};


const char LCD_SS_Status[17][21] = {
  "  Safety Status OK  ",
  "  Governor Switch   ",	// safety_string_status = 0x1
  "  Top Final Limit   ",	// safety_string_status = 0x2
  " Bottom Final Limit ",	// safety_string_status = 0x4
  "     Pit Switch     ",   // safety_string_status = 0x8                   
  "  Hoistway Safety   ",   // safety_string_status = 0x10  
  "  Car Top Stop Sw   ",	// safety_string_status = 0x20
  " Car Safety Switch  ",	// safety_string_status = 0x40
  "  Rail Lock Switch  ",   // safety_string_status = 0x80                 
  "   FF Stop Switch   ",   // safety_string_status = 0x100                
  "  Car Stop Switch   ",   // safety_string_status = 0x200  
  " Motor Room Stop Sw ",   // safety_string_status = 0x400
  "  Gripper Trip Sw   ",   // safety_string_status = 0x800
  "                    ",	// safety_string_status = 0x1000
  "                    ",   // safety_string_status = 0x2000
  "                    ",   // safety_string_status = 0x4000
  "                    ",   // safety_string_status = 0x8000
};

const char LCD_Doorf[6][21] = {
  "Door Closed    PI=  ",	// doorf == 0
  "Door Opening   PI=  ",   // doorf == 1
  "Door Open      PI=  ",   // doorf == 2
  "Door Closing   PI=  ",   // doorf == 3
  "Door Nudging   PI=  ",   // doorf == 4
  "Door Ajar      PI=  ",   // doorf == 5
};

const char LCD_Rdoorf[6][21] = {
  "R Door Closed  PI=  ",	// rdoorf == 0
  "R Door Opening PI=  ",   // rdoorf == 1
  "R Door Open    PI=  ",   // rdoorf == 2
  "R Door Closing PI=  ",   // rdoorf == 3
  "R Door Nudging PI=  ",   // rdoorf == 4
  "R Door Ajar    PI=  ",   // rdoorf == 5
};

const char LCD_Door_dwell[2][21] = {
  "Door Dwelling  PI=  ",   // doorf == 2
  "RDoor Dwelling PI=  ",   // rdoorf == 2
};

const char LCD_Servf[46][21]={            // service flag
  "    Safety Sting    ",	// servf = 0 Safty String
  " Inspection Service ",	// servf = 1 Inspection
  "   Reset Service    ",	// servf = 2 Reset Mode
  " Test Mode Service  ",	// servf = 3 Test Mode
  "Hoistway Setup Fault",	// servf = 4 Hoistway Setup 
  "Fire Fighters Stop S",	// servf = 5 Fire Fighters Stop Switch
  "Machine Room Stop SW",	// servf = 6 Machine Room Stop Switch
  "  At Floor Shutdown ",	// servf = 7 AT Floor Shutdown
  " Communications Fail",	// servf = 8 Comm Fail
  " Door Close Failure ",	// servf = 9 Door Close Fail
  " Door Protection Svc",	// servf = 10 Door Protection Service 
  "Stalled Out of Serv ",	// servf = 11 Stalled
  "   Low Oil Service  ",	// servf = 12 Low Oil
  " Earth Quake Service",	// servf = 13 Earthquake
  "Emergency Power Serv",	// servf = 14 Emergency Power
  "Fire Service Phase 2",	// servf = 15 Fire Phase 2
  "Fire Ph 1 Main Retrn",	// servf = 16 Fire Phase 1 Main
  "Fire Ph 1 Alt Return",	// servf = 17 Fire Phase 1 Alt
  "Emergency Pwr Recall", 	// servf = 18 Emergency Power Recall
  "  Hospital Service  ",	// servf = 19 Hospital Service
  " Medical Emergency  ",	// servf = 20 Medical emergency Service
  " Code Blue Service  ",	// servf = 21 Code Blue
  "    Low Pressure    ",	// servf = 22 Low Pressure
  "  Hot Oil Operation ",	// servf = 23 Hot Oil.
  "    Auto Door Off   ",	// servf = 24 Auto Door Off
  "    Riot Control    ",	// servf = 25 RIOT
  "Car Sw Elevator Off ",   // servf = 26 Elevator Off Car Switch
  "Hall Sw Elevator Off",	// servf = 27 Elevator Off Hall Switch
  "Car Sw Elevator Off2",	// servf = 28 Car Off
  "  Return to Lobby   ",	// servf = 29 Lobby Recall
  "Independent Service ",	// servf = 30 Independent
  "  Priority Service  ", 	// servf = 31 VIP Service
  "Calibrate Load Weigh", 	// servf = 32 Load Weigher Calibration
  " Reset Jack Service ", 	// servf = 33 Reset Jack
  "Load Weigh Overload ", 	// servf = 34 Load Weigh Overload
  "Load Weighing Bypass", 	// servf = 35 Load Bypass
  " Extended Door Time ", 	// servf = 36 Extended Door Time
  "   Reset Going Up   ", 	// servf = 37 Reset Up
  "  Reset Going Down  ", 	// servf = 38 Reset Down
  "  Security Recall   ", 	// servf = 39 Security Recall
  "    TUG Service     ", 	// servf = 40 TUGS
  "  Sabbath Service   ", 	// servf = 41 Sabbath Service
  " Attendant Service  ", 	// servf = 42 Attendant
  "       Homing       ", 	// servf = 43 Homing
  " Automatic Service  ", 	// servf = 44 Auto 
  "                    ", 	// servf = 45 Unused
};


const char LCD_Ins_Servf[9][21]={            // service flag
  "Invalid Insp Input  ",	// ins_servf = 0 invalid inspection input
  "      Car Top       ",	// ins_servf = 1 Car Top inspection
  "    Machine Room    ",	// ins_servf = 2 Machine Room Inspection
  "      Access        ",	// ins_servf = 3 Access Inspection
  "      In-Car        ",	// ins_servf = 4 In-Car inspection
  "Car Top Lock Bypass ",	// ins_servf = 5 Car Top Lock Bypass inspection
  "Car Top Gate Bypass ",	// ins_servf = 6 Car Top Gate Bypass inspection
  "Car Top G & L Bypass",	// ins_servf = 7 Car Top Gate and Lock Bypass inspection
  "COP HW Setup Jumper ",	// ins_servf = 8 Selector setup swtich is on
};

const char Service[45][20] = {            // service flag (Left Justified)
  "Safety Sting        ",	// servf = 0 Safty String
  "Inspection Service  ",	// servf = 1 Inspection
  "Reset Mode          ",	// servf = 2 Reset Mode
  "Test Mode Service   ",	// servf = 3 Test Mode
  "Hoistway Setup Fault",	// servf = 4 Hoistway Setup 
  "Fire Fighters Stop S",	// servf = 5 Fire Fighters Stop Switch
  "Machine Room Stop SW",	// servf = 6 Machine Room Stop Switch
  "At Floor Shutdown   ",	// servf = 7 AT Floor Shutdown
  "Communications Fail ",	// servf = 8 Comm Fail
  "Door Close Failure  ",	// servf = 9 Door Close Fail
  "Door Protection Svc ",	// servf = 10 Door Protection Service 
  "Stalled Out of Serv ",	// servf = 11 Stalled
  "Low Oil Service     ",	// servf = 12 Low Oil
  "Earth Quake Service ",	// servf = 13 Earthquake
  "Emergency Power Serv",	// servf = 14 Emergency Power
  "Fire Service Phase 2",	// servf = 15 Fire Phase 2
  "Fire Ph 1 Main Retrn",	// servf = 16 Fire Phase 1 Main
  "Fire Ph 1 Alt Return",	// servf = 17 Fire Phase 1 Alt
  "Hospital Service    ",	// servf = 18 Hospital Service
  "Medical Emergency   ",	// servf = 19 Medical emergency Service
  "Code Blue Service   ",	// servf = 20 Code Blue
  "Low Pressure        ",	// servf = 21 Low Pressure
  "Hot Oil Operation   ",	// servf = 22 Hot Oil.
  "Auto Door Off       ",	// servf = 23 Auto Door Off
  "Riot Control        ",	// servf = 24 RIOT
  "Car Sw Elevator Off ",   // servf = 25 Elevator Off Car Switch
  "Hall Sw Elevator Off",	// servf = 26 Elevator Off Hall Switch
  "Car Sw Elevator Off2",	// servf = 27 Car Off
  "Return to Lobby     ",	// servf = 28 Lobby Recall
  "Independent Service ",	// servf = 29 Independent
  "Priority Service    ", 	// servf = 30 VIP Service
  "Calibrate Load Weigh", 	// servf = 31 Load Weigher Calibration
  "Reset Jack Service  ", 	// servf = 32 Reset Jack
  "Load Weigh Overload ", 	// servf = 33 Load Weigh Overload
  "Load Weighing Bypass", 	// servf = 34 Load Bypass
  "Extended Door Time  ", 	// servf = 35 Extended Door Time
  "Reset Going Up      ", 	// servf = 36 Reset Up
  "Reset Going Down    ", 	// servf = 37 Reset Down
  "Security Recall     ", 	// servf = 38 Security Recall
  "TUG Service         ", 	// servf = 39 TUGS
  "Sabbath Service     ", 	// servf = 40 Sabbath Service
  "Attendant Service   ", 	// servf = 41 Attendant
  "Homing              ", 	// servf = 42 Homing
  "Automatic Service   ", 	// servf = 43 Auto 
  "                    ", 	// servf = 44 Unused
};

const char servf_tbl[46][4] = {
		"SAF",            //servf = 0 Safty String
		"INS",            //servf = 1 Inspection
		"RST",            //servf = 2 Reset Mode
		"TST",            //servf = 3 Test Mode
		"HSU",            //servf = 4 Hoistway Setup 
		"FFS",            //servf = 5 Fire Fighters Stop Switch
		"MRS",            //servf = 6 Machine Room Stop Switch
		"AFS",            //servf = 7 AT Floor Shutdown
		"CMF",            //servf = 8 Comm Fail
		"DCF",            //servf = 9 Door Close Fail
		"DPS",            //servf = 10 Door Protection Service 
		"STA",            //servf = 11 Stalled
		"LOL",            //servf = 12 Low Oil
		"EAQ",            //servf = 13 Earthquake
		"EMP",            //servf = 14 Emergency Power
		"FS2",            //servf = 15 Fire Phase 2
		"FSM",            //servf = 16 Fire Phase 1 Main
		"FSA",            //servf = 17 Fire Phase 1 Alt
		"EPR",			  //servf = 18 Emergency Power Recall
		"HSV",            //servf = 19 Hospital Service
		"MED",            //servf = 20 Medical emergency Service
		"CBL",            //servf = 21 Code Blue
		"LPR",            //servf = 22 Low Pressure
		"HOT",            //servf = 23 Hot Oil.
		"ADO",            //servf = 24 Auto Door Off
		"SEC",            //servf = 25 RIOT
        "EOF",            //servf = 26 Elevator Off Car Switch
		"HEO",            //servf = 27 Elevator Off Hall Switch
		"EO2",            //servf = 28 Car Off
		"RTL",            //servf = 29 Lobby Recall
		"IND",            //servf = 30 Independent
		"VIP",            //servf = 31 VIP Service
		"LWC",            //servf = 32 Load Weigher Calibration
		"JAK",            //servf = 33 Reset Jack
		"OVL",            //servf = 34 Load Weigh Overload
		"LBP",            //servf = 35 Load Bypass
		"EDT",            //servf = 36 Extended Door Time
		"RSU",            //servf = 37 Reset Up
		"RSD",            //servf = 38 Reset Down
		"SEC",            //servf = 39 Security Recall
		"TUG",            //servf = 40 TUGS
		"SAB",            //servf = 41 Sabbath Service
		"ATT",            //servf = 42 Attendant
		"HOM",            //servf = 43 Homing
		"AUT",            //servf = 44 Auto 
		"   ",            //servf = 45 Unused
		};


static  const char Set_Calls_Window[3][12] = {
  " --------- ",
  " Set CC  1 ",
  " --------- ",
};

static  const char Trace_Window[5][13] = {
  " ---Trace---",	// 0 - Border
  " Enter=Stop ",	// 1 - Stop Trace
  " Enter=Start",	// 2 - Start Trace
  " Mode=Exit  ",	// 3 - Exit
  " -----------",	// 4 - Border
};

static  const char LCD_Run_Status[3][21]={
  "Dmd=      Vel=      ",
  "Dif=      Dir=      ",
  "DP=            PI=  ",
};
#if ((Traction == 1) || (Tract_OL == 1))
static  const char LCD_Temp_Status[2][21]={
  "EXT Temp. = ----  F ",
  "SPB Temp. = ----  F ",
};
#endif
static  const char LCD_Run_Time[3][21]={
  "PTT=00.0  Run=000.0 ", 
  "SFl=0      PDO=00.0 ",
  "Lev=00.0   Roll=0   ",
};

static  const char LCD_Voltage[2][21]={
  "L1V=000.0  L2V=000.0",
  "L3V=000.0  DoV=000.0",
};

static const char LCD_Dpy_IO_Row[21] = {
  "      |      |      ",	// Seperate I/O
};

static  const char LCD_NTS_Status[4][21]={
  "NTS Count = 00000000",   // 4  - 12
  "Vel = 100 ^fpm      ",   // 5  - 6, 10, 11
  "UN*|UT*| DZ*| HWLrn*",   // 6  - 2,  6, 11, 19
  "DN*|DT*|OnL*| ClipF*",   // 7  - 2,  6, 11, 19
};
		

const char LCD_Fault[max_nmbflts+1][21]={
  "  No Current Fault  ",   // 000F - No Faults
  "    Reset Fault     ",   // 001F - Reset failure
  "   Position Fault   ",   // 002F - Elevator lost failure (position error)
  " Binary Input Fault ",   // 003F - Binary floor input failure
  "Safety String Fault ",   // 004F - Safety string failure or no P
  " Door Zone On Fault ",   // 005F - Door zone on failure
  "   Stalled Fault    ",   // 006F - Stall failure
  "  Door Open Fault   ",   // 007F - Door open failure
  "  Door Close Fault  ",   // 008F - Door close failure
  "Up Directional Fault",   // 009F - Up directional open
  "Dn Directional Fault",   // 010F - Down directional open
  "Hoistway Learn Fault",   // 011F - Hoistway not Learned
  " Stop Switch Fault  ",   // 012F - Stop switch open
  "  DPM Input Fault   ",   // 013F - Door lock Protection (DPM) on fault
  "S10 Fuse Blown Fault",   // 014F - LC fuse Blown
  "HC Fuse Blown Fault ",   // 015F - HC fuse Blown
  " Drive Ready Fault  ",   // 016F - Drive Ready off
  "'P' Input Off Fault ",   // 017F - Potential input off
  "   Car Safe Fault   ",   // 018F - Car Safe Fault
  "UL and DL Off Fault ",   // 019F - UL and DL off when expected on
  "  Delta off Fault   ",   // 020F - Delta off when expected on
  "   UT count Fault   ",   // 021F - Up Terminal limit count error
  "  UT1 count Fault   ",   // 022F - Up Terminal 1 limit count error
  "  UT2 count Fault   ",   // 023F - Up Terminal 2 limit count error
  "  UT3 count Fault   ",   // 024F - Up Terminal 3 limit count error
  "   DT count Fault   ",   // 025F - Down Terminal limit count error
  "  DT1 count Fault   ",   // 026F - Down Terminal 1 limit count error
  "  DT2 count Fault   ",   // 027F - Down Terminal 2 limit count error
  "  DT3 count Fault   ",   // 028F - Down Terminal 3 limit count error
  "Rear Door Open Fault",   // 029F - Door open failure
  "Rear Door Close Flt ",   // 030F - Door close failure
  "  Group Comm Loss   ",   // 031F - Group Comm loss 
  "  Car 1 Comm Loss   ",   // 032F - Car 1 Comm Loss (Cannot have a Car 1 comm loss)
  "  Car 2 Comm Loss   ",   // 033F - Car 2 Comm Loss
  "  Car 3 Comm Loss   ",   // 034F - Car 3 Comm Loss
  "  Car 4 Comm Loss   ",   // 035F - Car 4 Comm Loss
  "  Car 5 Comm Loss   ",   // 036F - Car 5 Comm Loss
  "  Car 6 Comm Loss   ",   // 037F - Car 6 Comm Loss
  " RUN I/O Failed On  ",   // 038F - RUN input or output failed on
  "RUN O/RUNA I Failed ",   // 039F - Run output or runa input failed
  " RUN I/O Failed Off ",   // 040F - RUN input or output failed off 
  " RUNA I/O Failed On ",   // 041F - RUNA input or output failed on
  "RUNA I/O Failed Off ",   // 042F - RUNA input or output failed off
  "  UP I/O Failed On  ",   // 043F - UP input or output failed on
  " UP I/O Failed Off  ",   // 044F - UP input or output failed off
  " DNR I/O Failed On  ",   // 045F - DNR input or output failed on
  " DNR I/O Failed Off ",   // 046F - DNR input or output failed off
  " UPF I/O Failed On  ",   // 047F - UP Fast input or output failed on
  " UPF I/O Failed Off ",   // 048F - UP Fast input or output failed off
  "  DF I/O Failed On  ",   // 049F - DN Fast input or output failed on
  " DF I/O Failed Off  ",   // 050F - DN Fast input or output failed off
  " MCC I/O Failed On  ",   // 051F - MCC input or output failed on
  " RUN 0/RUN I Failed ",   // 052F - Run output or run input failed
  " MCC I/O Failed Off ",   // 053F - MCC input or output failed off 
  " MCA I/O Failed On  ",   // 054F - MC input or output failed on
  " MCA I/O Failed Off ",   // 055F - MC input or output failed off
  " BRK I/O Failed On  ",   // 056F - BRK input or output failed on
  " BRK I/O Failed Off ",   // 057F - BRK input or output failed off
  " DON I/O Failed On  ",   // 058F - DON input failed on
  " DON I/O Failed Off ",   // 059F - DON input failed off
  " RUN, RUNA, UP Fail ",   // 060F - RUN input/output or UP output failed off
  "RUN, RUNA, DNR Fail ",   // 061F - RUN input/output or DNR output failed off
  "Top Door Lock Fault ",   // 062F - Door Lock Top failed on
  "Mid Door Lock Fault ",   // 063F - Door Lock Middle failed on
  "Bot Door Lock Fault ",   // 064F - Door Lock Bottom failed on
  " Gate Switch Fault  ",   // 065F - Gate Switch failed on 
  "Rear Top Lock Fault ",   // 066F - Rear Door Lock Top failed on
  "Rear Mid Lock Fault ",   // 067F - Rear Door Lock Middle failed on
  "Rear Bot Lock Fault ",   // 068F - Rear Door Lock Bottom failed on
  " Rear Gate Sw Fault ",   // 069F - Rear Gate Switch failed on
  " 'P' Input On Fault ",   // 070F - Potential input failed on
  "    Estop Fault     ",   // 071F - Three estops in a row
  "Inspection Input Flt",   // 072F - Inspection input error
  "Gate/Lock Byp Sw Flt",   // 073F - Gate or lock bypass error
  "GRT1 input On Fault ",   // 074F - Gripper input 1 failed on
  "GRT1 input Off Fault",   // 075F - Gripper input 1 failed off
  "GRT2 input On Fault ",   // 076F - Gripper input 2 failed on
  "GRT2 input Off Fault",   // 077F - Gripper input 2 failed off
  "Gripper did not Pick",   // 078F - Gripper did not pick
  " Gripper Trip Fault ",   // 079F - Gripper tripped from an un-safe condition
  "DLT & DLT-1 Opposite",   // 080F - Door Lock Top DLT and DLT_1 failed opposite
  "DLM & DLM-1 Opposite",   // 081F - Door Lock Middle DLM and DLM_1 failed opposite
  "DLB & DLB-1 Opposite",   // 082F - Door Lock Bottom DLB and DLB_1 failed opposite
  " GS & GS_1 Opposite ",   // 083F - Gate Switch GS and GS_1 failed opposite 
  "RLM & RLM-1 Opposite",   // 084F - Rear Door Lock Middle RLM and RLM_1 failed opposite
  "RGS & RGS-1 Opposite",   // 085F - Rear Gate Switch RGS and RGS_1 failed opposite
  "  Brake Pick Fault  ",   // 086F - Brake failed to pick 
  "  Brake Drop Fault  ",   // 087F - Brake failed to drop
  "Brk Flt Set Gripper ",   // 088F - Gripper set from brake failure
  "   Delta On Fault   ",   // 089F - DEL input failed on when should be off
  " CWT Sw Error at DT ",   // 090F - Above cwt flag set incorrectly at DT limit
  " CWT Sw Error at UT ",   // 091F - Above cwt flag set incorrectly at UT limit
  " Target Fault at UT ",   // 092F - Target error at UT slowdown limit
  "Target Fault at UT1 ",   // 093F - Target error at UT1 slowdown limit
  "Target Fault at UT2 ",   // 094F - Target error at UT2 slowdown limit
  "Target Fault at UT3 ",   // 095F - Target error at UT3 slowdown limit
  " Target Fault at DT ",   // 096F - Target error at DT slowdown limit
  "Target Fault at DT1 ",   // 097F - Target error at DT1 slowdown limit
  "Target Fault at DT2 ",   // 098F - Target error at DT2 slowdown limit
  "Target Fault at DT3 ",   // 099F - Target error at DT3 slowdown limit
  "  Overspeed Fault   ",   // 100F - Velocity Over speed
  "HC Com Device Reset ",   // 101F - Hall call device reset
  "Car Com Device Reset",   // 102F - Car comm device reset
  " TOC CAN Com Error  ",   // 103F - Top car device can comm error
  "Drive Com Rcv Error ",   // 104F - Drive comm receive error 
  "   Power Up Reset   ",   // 105F - Power Up Reset
  "Possible DRV/SPB Er ",   // 106F - Drive or Safety Processor Fault
  "Car Safe Fault Preop",   // 107F - Car Safe Fault during preopening
  "Car Safe Fault Start",   // 108F - Car Safe Fault during start (door possibly not closed)
  "Inspection Up/Dn Sw ",   // 109F - Up or down inspections switch error
  "Drive Com Trm Error ",   // 110F - Drive Comm transmit error
  "Drive Com Int Error ",   // 111F - Drive Comm interrupt enable error
  "Drive Com TxEmpty Er",   // 112F - Drive comm transmit empty error
  "  UTS count Fault   ",   // 113F - Up ETS Terminal limit count error
  "  DTS count Fault   ",   // 114F - Down ETS Terminal limit count error
  "Target Fault at UTS ",   // 115F - Target error at UTS slowdown limit
  "Target Fault at DTS ",   // 116F - Target error at DTS slowdown limit
  "Drive Com 8259 Error",   // 117F - Drive comm 8256 error
  "Drive Com Tx Stop Er",   // 118F - Drive transmitter stopped error
  " SPB CAN Com Error  ",   // 119F - Safety Processor board can communications error
  "DBR Temperature Flt ",   // 120F - Dynamic Brake Resistors are too hot fault
  " SEB CAN Com Error  ",	// 121F - Serial Expansion Board Can comm error
  "LW Calibration Error",	// 122F - Load weigher calibration error
  "CAN Chip Init Error ",   // 123F - Can bus initialization error (bus or timer failure)
  "Run Fault: Shutdown ",   // 124F - Too many consecutive runs with faults
  "Drive has Com Error ",	// 125F - Drive has a comm error
  "Encoder Count Error ",	// 126F - Encoder Fault
  " Can Bus Off Error  ",	// 127F - Can Bus Off error
  "Zero Vel Decel Roll ",   // 128F - Zero Velocity Error during roll into decel
  "  PC/104 Bus Error  ",	// 129F - bus error during encoder read
  " Low Pressure Fault ",	// 130F - low pressure switch error
  "UTS Failed On Fault ",	// 131F - UTS failed on
  "DTS Failed On Fault ",	// 132F - DTS failed on
  " UT Failed On Fault ",	// 133F - UT failed on
  " DT Failed On Fault ",	// 134F - DT failed on
  "  RPM Input Fault   ",	// 135F - Rear Door lock Protection (RPM) on fault
  "DPM Off/GS or DL On ",	// 136F - DPM off with gate or lock on
  "RPM Off/RGS or DL On",	// 137F - RPM off with rear gate or lock on 
  "Pulse Error > 75 fpm",	// 138F - Pulse count error, car > 75 fpm travel < 2in
  "Wrong Dir Pls Run Up",	// 139F - Pulse count wrong direction running up
  "Wrong Dir Pls Run Dn",	// 140F - Pulse count wrong direction running down
  "Pls Er 75% Top Speed",	// 141F - Pulse count error, 75% top speed not enough counts
  " BRK CAN Com Error  ",	// 142F - Brake board can error
  "Governor Switch Flt ",	// 143F - Governor switch
  "Top Final Limit Flt ",	// 144F - Top final limit
  "Bot Final Limit Flt ",	// 145F - Bottom final limit
  "  Pit Switch Fault  ",	// 146F - Pit switch
  "Pit Door Switch Flt ",	// 147F - Pit door switch
  "Buffer Switch Fault ",	// 148F - Buffer switch
  " Comp. Switch Fault ",	// 149F - Comp rope switch
  "Top Emerg. Exit Flt ",	// 150F - Top emergency exit switch
  "Side Emerg. Exit Flt",	// 151F - Side Emergency exit switch
  "Car Safety Sw. Fault",	// 152F - Car safety switch
  "Car Call Power Fuse ",	// 153F - Car call common fuse
  "Car Call Light Fuse ",	// 154F - Car call light common fuse
  "Hall Call Light Fuse",	// 155F - Hall call light common fuse
  "Fire Fighter Stop Sw",	// 156F - Fire Fighters Stop Switch
  "PFC Relay Failed On ", 	// 157F - PFC relay failed on 
  "PFC Relay Failed Off", 	// 158F - PFC relay failed off
  "SFC Relay Failed On ", 	// 159F - SFC relay failed on
  "SFC Relay Failed Off", 	// 160F - SFC relay failed off
  "Door Zone Aux On Flt",	// 161F - Door zone A on fault
  "Door Zone Off Fault ",	// 162F - Door zone off fault
  "DoorZone Aux Off Flt",	// 163F - Door zone A off fault
  "PFC-SFC Test Lost DZ",	// 164F - Lost DZ during PFC SFC test
  " FST I/O Failed On  ", 	// 165F - FST output failed on or FSTI input failed on
  " FST I/O Failed Off ", 	// 166F - FST output failed off or FSTI input failed off
  " FSTP I/O Failed On ", 	// 167F - FSTP output failed on
  "FSTP I/O Failed Off ", 	// 168F - FSTP output failed off
  " SEL CAN Com Error  ",  	// 169F - Selector board can error
  "RUNA O/RUN I Failed ",	// 170F - Runa or runi failed
  " Speed Control Exit ", 	// 171F - Emergency exit
  " UL Failed On Fault ", 	// 172F - UL failed on
  " DL Failed On Fault ", 	// 173F - DL failed on
  "UL,DL & DZ Off at FL", 	// 174F - UL DL and DZ off at floor
  "  UT4 count Fault   ",	// 175F - Up Terminal 4 limit count error
  "  UT5 count Fault   ",	// 176F - Up Terminal 5 limit count error
  "  UT6 count Fault   ",	// 177F - Up Terminal 6 limit count error
  "  DT4 count Fault   ",	// 178F - Down Terminal 4 limit count error
  "  DT5 count Fault   ",	// 179F - Down Terminal 5 limit count error
  "  DT6 count Fault   ",	// 180F - Down Terminal 6 limit count error
  "Target Fault at UT4 ",	// 181F - Target error at UT4 slowdown limit
  "Target Fault at UT5 ",	// 182F - Target error at UT5 slowdown limit
  "Target Fault at UT6 ",	// 183F - Target error at UT6 slowdown limit
  "Target Fault at DT4 ",	// 184F - Target error at DT4 slowdown limit
  "Target Fault at DT5 ",	// 185F - Target error at DT5 slowdown limit
  "Target Fault at DT6 ",	// 186F - Target error at DT6 slowdown limit
  "Field Vars Relocated",	// 187F - Field variables relocated
  "Field Vars Deflt Ini",	// 188F - Field variables initialized
  "Hoistway Default Ini",	// 189F - Hoistway initialized
  "  Car 7 Comm Loss   ",	// 190F - Car 7 Comm Loss
  "  Car 8 Comm Loss   ",   // 191F - Car 8 Comm Loss
  "   EE Ram Failed    ",	// 192F - EE Ram fault
  "   NV Ram Failed    ",	// 193F - NV Ram Fault
  "Hoistway Update Init",	// 194F - Hoistway update table initialized
  "Lowoil Switch Fault ",	// 195F - Low oil switch input fault
  "   Leveling Fault   ",	// 196F - Leveling fault
  "Selector Count Fault",	// 197F - Selector Count fault
  "Invalid Floor Count ",	// 198F - Invalid floor count
  "Invlid Top Floor Cnt",	// 199F - Invalid top floor count
  "Invalid DN or DT Cnt",	// 200F - Invalid DN or DT count
  "Invald DT or DT1 Cnt",	// 201F - Invalid DT or DT1 count
  "Invld DT1 or DT2 Cnt",	// 202F - Invalid DT1 or DT2 count
  "Invld DT2 or DT3 Cnt",	// 203F - Invalid DT2 or DT3 count
  "Invld DT3 or DT4 Cnt",	// 204F - Invalid DT3 or DT4 count
  "Invld DT4 or DT5 Cnt",	// 205F - Invalid DT4 or DT5 count
  "Invld DT5 or DT6 Cnt",	// 206F - Invalid DT5 or DT6 count
  "Invld UT6 or UT5 Cnt",	// 207F - Invalid UT6 or UT5 count
  "Invld UT5 or UT4 Cnt",	// 208F - Invalid UT5 or UT4 count
  "Invld UT4 or UT3 Cnt",	// 209F - Invalid UT4 or UT3 count
  "Invld UT3 or UT2 Cnt",	// 210F - Invalid UT3 or UT2 count
  "Invld UT2 or UT1 Cnt",	// 211F - Invalid UT2 or UT1 count
  "Invld UT1 or UT  Cnt",	// 212F - Invalid UT1 or UT  count
  "Invalid UT or UN Cnt",	// 213F - Invalid UT or UN count
  "Invalid SEL Bot Cnt ",	// 214F - Invalid Selector Bottom Count
  "Invalid SEL Top Cnt ",	// 215F - Invalid Selector Top Count
  "Invld DZU or DZD Cnt",	// 216F - Invalid DZU or DZD Count
  "Invald FL Offset Cnt",	// 217F - Invalid FL offset Count
  " ASV Time-out Car 1 ", 	// 218F - Auto Service Time-out Car 1
  " ASV Time-out Car 2 ", 	// 219F - Auto Service Time-out Car 2
  " ASV Time-out Car 3 ", 	// 220F - Auto Service Time-out Car 3
  " ASV Time-out Car 4 ", 	// 221F - Auto Service Time-out Car 4
  " ASV Time-out Car 5 ", 	// 222F - Auto Service Time-out Car 5
  " ASV Time-out Car 6 ", 	// 223F - Auto Service Time-out Car 6
  " ASV Time-out Car 7 ", 	// 224F - Auto Service Time-out Car 7
  " ASV Time-out Car 8 ", 	// 225F - Auto Service Time-out Car 8
  " BKS Fault Slowdown ",   // 226F - Brake switch fail slowdown
  "Leveling Tim-out Flt",	// 227F - Time-out leveling fault targeting floor
  "Hoistway Safe Fault ",   // 228F - Auto learn hoistway safe fault
  "Learn HW Stall Fault",	// 229F - Auto learn hoistway stall fault
  " EM Brake Input OFF ",	// 230F - Emergency Brake failed off fault
  " EM Brake Input ON  ",	// 231F - Emergency Brake failed on fault
  "EM Brake Switch OFF ",	// 232F - Emergency Brake failed off fault
  " EM Brake Switch ON ",	// 233F - Emergency Brake failed on fault
  "BRK Over Current Flt",   // 234F - Brake board over current fault (brake error 2)
  "BRK Over Voltage Flt",   // 235F - Brake board over voltage fault (3)
  "BRK No Currnt w/Volt",	// 236F - Brake board no current fault with voltage applied (4)
  "BRK No DC Bus Volts ",	// 237F - Brake board no dc bus voltage fault (5)
  "BRK No Output Volts ",	// 238F - Brake board not output voltage fault  (6)
  "BRK Low DC Bus Volts",	// 239F - Brake board low dc bus voltage fault   (7)
  "BRK Rq Volt > DC Bus",	// 240F - Brake board dmd voltage greater than dc bus voltage fault (8)
  "Grp Comm Config Err ",	// 241F - Group Comm configuration error (hall call i/o address configuration error)
  "HC DvrBd Rx from Top",	// 242F - Rx from top HC board fault  (nest 8 errors must be consecutive)
  " HC DvrBd Tx to Top ",	// 243F - Tx to top HC board fault
  "HC DvrBd Too Few Dev",   // 244F - Too few stations on bus
  "HC DvrBd TooMany Dev",	// 245F - Too many stations on bus
  "HC DrvBd Rx from Bot",	// 246F - Rx from bottom HC board fault
  " HC DrvBd Tx to Bot ",	// 247F - Tx to bottom HC board fault
  " HC DrvBd error 248 ",	// 248F - Reserved for hc board fault
  " HC DrvBd error 249 ",   // 249F - Reserved for hc board fault 
  "Level Stop Cnt Fault",   // 250F - Leveling stop fault occured from incorrect count
  " GTS Input On Fault ",   // 251F - GTS input failed on  
  "GTS Input Off Fault ",   // 252F - GTS input failed off
  " Invalid Fault Code ",   // 253F - Invalid fault code
  "SPB Unintend Motion ",   // 254F - SPB Unintended Motion
  "Fault# > IDS Flt Siz",   // 255F - Largest error IDS can do
  " HCB Tx to below fl ",	// 256F - HCB Tx fault down to below floor    (next 14 errors must be consecutive)
  " HCB Tx to above fl ",	// 257F - HCB tx fault up to above floor
  "HCB Rx from below fl",	// 258F - HCB rx fault down from below floor
  "HCB Rx from above fl",	// 259F - HCB rx fault up to above floor
  "HCB Stuck Dn Button ",   // 260F - HCB stuck button down
  "HCB Stuck Up Button ",	// 261F - HCB stuck button up
  "  HCB Dn LED Open   ",	// 262F - HCB led open down
  "  HCB Up LED Open   ",	// 263F - HCB led open up
  "  HCB Dn LED Short  ",   // 264F - HCB led short down
  "  HCB Up LED Short  ",   // 265F - HCB led short up
  "  HCB Dn FET Open   ",   // 266F - HCB fet open down
  "  HCB Up FET Open   ",   // 267F - HCB fet open up
  "  HCB Dn FET Short  ",   // 268F - HCB fet short down
  "  HCB Up FET Short  ",   // 269F - HCB fet short up
  "EBAi Input On Fault ",   // 270F - Emergency brake A1,A2 input failed on
  "EBAi Input Off Fault",   // 271F - Emergency brake A1,A2 input failed off
  "EBBi Input On Fault ",   // 272F - Emergency brake B1,B2 input failed on
  "EBBi Input Off Fault",   // 273F - Emergency brake B1,B2 input failed off
  " DL & DL-1 Opposite ",   // 274F - DL and DL1 do not match
  " UL & UL-1 Opposite ",	// 275F - UL and UL1 do not match
  " HCB Invalid Floor  ",   // 276F - HCB has invalid floor
  "HCB Device Comm Loss",	// 277F - HCB no comm to individual hc board
  "  HCB Device Reset  ",	// 278F - HCB device reset
  "HCB No Dn LED Board ",	// 279F - HCB No Down Led Board Detected
  "HCB No Up LED Board ",	// 280F - HCB No Up Led Board Detected
  "HCB LED Short Red Up", 	// 281F - HCB led short red up
  "HCB LED Short Grn Up", 	// 282F - HCB led short green up
  "HCB LED Short Blu Up", 	// 283F - HCB led short blue up
  "HCB LED Short Blu Dn", 	// 284F - HCB led short blue down
  "HCB LED Short Grn Dn", 	// 285F - HCB led short green down
  "HCB LED Short Red Dn", 	// 286F - HCB led short red down
  "HCB LED Open Red Up ", 	// 287F - HCB led short red up
  "HCB LED Open Grn Up ", 	// 288F - HCB led open green up
  "HCB LED Open Blue Up", 	// 289F - HCB led open blue up
  "HCB LED Open Blue Dn", 	// 290F - HCB led open blue down
  "HCB LED Open Grn Dn ", 	// 291F - HCB led open green down
  "HCB LED Open Red Dn ", 	// 292F - HCB led open red down
  "HCB FET Short Red Up", 	// 293F - HCB fet short red up
  "HCB FET Short Grn Up", 	// 294F - HCB fet short green up
  "HCB FET Short Blu Up", 	// 295F - HCB fet short blue up
  "HCB FET Short Blu Dn", 	// 296F - HCB fet short blue down
  "HCB FET Short Grn Dn", 	// 297F - HCB fet short green down
  "HCB FET Short Red Dn", 	// 298F - HCB fet short red down
  "HCB FET Open Red Up ", 	// 299F - HCB fet short red up
  "HCB FET Open Grn Up ", 	// 300F - HCB fet open green up
  "HCB FET Open Blue Up", 	// 301F - HCB fet open blue up
  "HCB FET Open Blue Dn", 	// 302F - HCB fet open blue down
  "HCB FET Open Grn Dn ", 	// 303F - HCB fet open green down
  "HCB FET Open Red Dn ", 	// 304F - HCB fet open red down
  "UPS Low Battery Flt ",	// 305F - power loss ups battery fault
  "UPS On Battery Power",	// 306F - power loss ups on battery power
  "   UPS Turned Off   ", 	// 307F - power loss ups turned off
  "   UPS Comm Fault   ",	// 308F - power loss ups online fault
  "UPS Low Bat Voltage ",	// 309F - power loss ups battery fault
  "UL,DL Learn Cnt Flt ",	// 310F - UL,DL setup count fault
  "EPRecall Car1 Tim-ot",	// 311F - Emergency Power Recall Time-out Car 1
  "EPRecall Car2 Tim-ot",	// 312F - Emergency Power Recall Time-out Car 2
  "EPRecall Car3 Tim-ot",	// 313F - Emergency Power Recall Time-out Car 3
  "EPRecall Car4 Tim-ot",	// 314F - Emergency Power Recall Time-out Car 4
  "EPRecall Car5 Tim-ot",	// 315F - Emergency Power Recall Time-out Car 5
  "EPRecall Car6 Tim-ot",	// 316F - Emergency Power Recall Time-out Car 6
  "EPRecall Car7 Tim-ot",	// 317F - Emergency Power Recall Time-out Car 7
  "EPRecall Car8 Tim-ot",	// 318F - Emergency Power Recall Time-out Car 8
  " SPB SFC Off Fault  ",   // 319F - Safety Processor Board SFC fault
  "EP Recall Car 1 OTS ",	// 320F - Emergency Power Recall Car Out of Service Car 1
  "EP Recall Car 2 OTS ",	// 321F - Emergency Power Recall Car Out of Service Car 2
  "EP Recall Car 3 OTS ",	// 322F - Emergency Power Recall Car Out of Service Car 3
  "EP Recall Car 4 OTS ",	// 323F - Emergency Power Recall Car Out of Service Car 4
  "EP Recall Car 5 OTS ",	// 324F - Emergency Power Recall Car Out of Service Car 5
  "EP Recall Car 6 OTS ",	// 325F - Emergency Power Recall Car Out of Service Car 6
  "EP Recall Car 7 OTS ",	// 326F - Emergency Power Recall Car Out of Service Car 7
  "EP Recall Car 8 OTS ",	// 327F - Emergency Power Recall Car Out of Service Car 8
  " Dn Normal SW Setup ",	// 328F - Down Normal must turn on before reaching bottom floor dead level
  " Up Normal SW Setup ",	// 329F - Up Normal must turn off after reaching the top floor dead level
  "Adv PreTrq Start Flt",	// 330F - Did not get safe on advance pre-torque start
  "Selector Preset Flt ",	// 331F - Selector preset position fault
  "Unintended Motion Ft",	// 332F - Unintended Motion fault
  "Emergency Brake Trip",	// 333F - Emergency Brake Trip fault
  "UTM Contact/GTS Flt ",	// 334F - UTS contact/GTS input pick fault
  "Brk Flt Set EM Brake", 	// 335F - Emergency brake set from brake fault
  "Front Det Edge Fault",	// 336F - Front Detector Edge Time-out
  "Rear Det Edge Fault ",	// 337F - Rear Detector Edge Time-out
  "L1 Low Line Voltage ",	// 338F - L1 Line Voltage Low
  "L2 Low Line Voltage ",	// 339F - L2 Line Voltage Low
  "L3 Low Line Voltage ",	// 340F - L3 Line Voltage Low
  "Door Low Voltage Flt",	// 341F - Door Line Voltage Low
  "Lobby Hall Call Fuse",	// 342F - Lobby Call common fuse
  "Door Motor Overload ",	// 343F - Door Motor Overload
  "Hoist Motor Overload",	// 344F - Hoist Motor Overload
  "Car Top Stop Switch ",	// 345F - Car top stop switch 
  "Door Lock Safe Fault",	// 346F - Door lock safe fault
  "Car Gate Safe Fault ",	// 347F - Car Gate safe fault
  "FDoor Close Cont Flt",   // 348F - Door Close Contact safe fault
  "RDoor Close Cont Flt",	// 349F - Rear Door Close Contact safe fault
  "UPS Low Bat Capacity",	// 350F - power loss ups battery capacity low fault
  "Motion Exit Ins Flt ",   // 351F - emergency motion exit from inspection 
  "Run Inhibit Rset Cnt",   // 352F - Run inhibit from reset count
  " At Floor Shutdown  ",   // 353F - At floor shutdown
  "   Hot Oil Fault    ",	// 354F - Hot Oil Fault
  "EPR Pwr Lost Moving ",   // 355F - EP Recovery power lost while moving
  " CCB LED Short Red  ",   // 356F - Car Call Board LED short red 
  "CCB LED Short Green ",   // 357F - Car Call Board LED short green
  " CCB LED Short Blue ",   // 358F - Car Call Board LED short blue
  "  CCB LED Open Red  ",   // 359F - Car Call Board LED open red
  " CCB LED Open Green ",   // 360F - Car Call Board LED open green
  " CCB LED Open Blue  ",   // 361F - Car Call Board LED open blue
  " CCB FET Short Red  ",   // 362F - Car Call Board FET short red 
  "CCB FET Short Green ",   // 363F - Car Call Board FET short green 
  " CCB FET Short Blue ",   // 364F - Car Call Board FET short blue 
  "  CCB FET Open Red  ",   // 365F - Car Call Board FET open red 
  " CCB FET Open Green ",   // 366F - Car Call Board FET open green 
  " CCB FET Open Blue  ",   // 367F - Car Call Board FET open blue
  "  CCB No LED Board  ",	// 368F - Car Call LED board missing
  "  CCB Stuck Button  ",	// 369F - Car Call Board stuck button
  "CCB No Comm Board 1 ",	// 370F - Car Call Board local board 1 comm loss
  "CCB No Comm Board 2 ",	// 371F - Car Call Board local board 2 comm loss
  "CCB No Comm Aux Bd 1",	// 372F - Car Call Board local aux board 1 comm loss
  "CCB No Comm Aux Bd 2",	// 373F - Car Call Board local aux board 2 comm loss
  "HCB Low Supply Volt ",	// 374F - Hall call board has low supply voltage
  "HCB Up Input Ovrload",   // 375F - HCB Up input overload
  "HCB Dn Input Ovrload",   // 376F - HCB Down input overload
  "HCB Ax Up Input Ovld",   // 377F - HCB Aux Up input overload
  "HCB Ax Dn Input Ovld",   // 378F - HCB Aux Down input overload
  "Fld Var Partial Init",   // 379F - Field variables partial table initialized
  "   RCM / Lock Flt   ",	// 380F - Retiring Cam/Lock fault
  "EM BRK CAN Com Error",	// 381F - Emergency Brake board can error
  " Encoder Preset Flt ",	// 382F - encoder Preset error
  " Encoder PPR Error  ", 	// 383F - encoder ppr error
  " Encoder Com Error  ", 	// 384F - encoder com error
  "    Encoder Init    ",	// 385F - encoder init
  "Velocity Diff Fault ",	// 386F - Velocity decel difference fault
  "SPB Enc Opp Dir Flt ",	// 387F - SPB encoder opposite direction fault
  " Limit Opp Dir Flt  ",	// 388F - Car hit limits in wrong direction fault
  " Encoder Dir Fault  ",	// 389F - Encoder direction fault
  "KEB Not In Run Mode ",	// 390F - Keb drive not in run mode
  "UL/DL Dir Seq Fault ",	// 391F - UL and DL Direction Fault
  " Can Bus Idle Error ",	// 392F - can bus idle error
  "Can Acknowledge Err ",	// 393F - can bus acknowledge error
  " Encoder Busy Error ",	// 394F - Encoder busy fault
  " Slip Detect Fault  ",	// 395F - SPB Velocity difference fault
  " User Variable Init ",	// 396F - User variable init
  "Load Weigh Var Init ",	// 397F - Load weigher init
  "MRAM Hardware Fault ",	// 398F - Mram Fault
  "Enc Can Bus Off Err ",	// 399F - Encoder can bus off
  "Enc Can Bus Idle Err",	// 400F - Encoder can bus idle
  "Enc Can Bus Ack Err ",	// 401F - Encoder can bus acknowledge error
  "Enc Can Packet Fault",	// 402F - Encoder can bus packet fault
  "XBK Over Current Flt",   // 403F - Aux Brake board over current fault (brake error 2)
  "XBK Over Voltage Flt",   // 404F - Aux Brake board over voltage fault (3)
  "XBK No Currnt w/Volt",	// 405F - Aux Brake board no current fault with voltage applied (4)
  "XBK No DC Bus Volts ",	// 406F - Aux Brake board no dc bus voltage fault (5)
  "XBK No Output Volts ",	// 407F - Aux Brake board not output voltage fault  (6)
  "XBK Low DC Bus Volts",	// 408F - Aux Brake board low dc bus voltage fault   (7)
  "XBK Rq Volt > DC Bus",	// 409F - Aux Brake board dmd voltage greater than dc bus voltage fault (8)
  "Motion Exit GTS Flt ",   // 410F - GTS Motion emergency exit
  "EBK Input Failed OFF",	// 411F - Emergency Brake EBK1 failed off fault
  "EBK Input Failed ON ",	// 412F - Emergency Brake EBK1 failed on fault
  "EBK1 Input Faild OFF",	// 413F - Emergency Brake EBK2 failed off fault
  "EBK1 Input Failed ON",	// 414F - Emergency Brake EBK2 failed on fault
  "Lev Flt Set Gripper ",   // 415F - Gripper set from leveling fault
  "Lev Flt Set EM Brake", 	// 416F - Emergency brake set from leveling fault
  "Selector DZ Off Flt ",	// 417F - Selector Door zone off fault
  "Rail Lock Switch Flt",	// 418F - Rail Lock Switch
  "Machine Room Stop Sw",	// 419F - Machine Room Stop Switch
  "SPB Count Update Err",	// 420F - Safety Processor count update error
  " MRCAN Device Fault ",	// 421F - Device fault (must look up device code
  " CTCAN Device Fault ",	// 422F - Device fault (must look up device code
  " GRCAN Device Fault ",	// 423F - Device fault (must look up device code
  " MRCAN Device Reset ",	// 424F - Car comm device reset
  " CTCAN Device Reset ",	// 425F - Car comm device reset
  " GRCAN Device Reset ",	// 426F - Car comm device reset
  " Hatch Safety Fault ",	// 427F - Hatch safety fault
  " COP CAN Com Error  ",	// 428F - Top car device can comm error
  "   MCX Off Fault    ",	// 429F - MCX off fault
  "    MCX On Fault    ", 	// 430F - MCX on fault
  "FEP Fuse Blown Fault",	// 431F - Fire/Emergency Fuse Blown
  "DL20Phone test fail ",	// 432F - phone test failed
  "CAN spb bad command ",	// 433F - wrong umotion command being sent to spb
  "   Trace Trigger    ",	// 434F - trace trigger occured
  "EE Tst EE1 Failed ON", 	// 435F - EE1 front input failed ON fault 
  "EE Tst EE1 Faild OFF", 	// 436F - EE1 front input failed OFF fault 
  "EE Tst EE2 Failed ON", 	// 437F - EE2 front input failed ON fault
  "EE Tst EE2 Faild OFF", 	// 438F - EE2 front input failed OFF fault
  "EE Tst EER1 Faild ON", 	// 439F - EER1 rear input failed ON fault 
  "EE Tst EER1 Fail OFF", 	// 440F - EER1 rear input failed OFF fault
  "EE Tst EER2 Faild ON", 	// 441F - EER2 rear input failed ON fault
  "EE Tst EER2 Fail OFF", 	// 442F - EER2 rear input failed OFF fault
  "   FETST ON Fault   ", 	// 443F - front edge test output ON fault
  "  FETST OFF Fault   ", 	// 444F - front edge test output OFF fault
  "   RETST ON Fault   ", 	// 445F - rear edge test output ON fault
  "  RETST OFF Fault   ", 	// 446F - rear edge test output OFF fault
  "Brake Test Stall Flt",	// 447F - Brake test stall fault
  "SPB Limit Vel Fault ",	// 448F - Safety Processor Limit Velocity Error
  "NTS Limit Vel Fault ",	// 449F - NTS Processor Limit Velocity Error
  "Brake Test UN/DN Flt",	// 450F - Brake Test UN, DN error
  "Brake Test UT/DT Flt",	// 451F - Brake Test UT, DT error
  "Brk Test UTS/DTS Flt",	// 452F - Brake Test UTS, DTS error
  "Brk Test Vel Tim-out",	// 453F - Brake Test Time-out before top speed
  "   RUNX Failed On   ",   // 454F - RUNX input or output failed on
  "  RUNX Failed Off   ",   // 455F - RUNX input or output failed off 
  " RUNX Off On Motion ",   // 456F - RUNX input or output failed off during motion 
  "  MRAM Write Error  ",	// 457F - Mram Write error
  "FVARS Backup Tbl Err",	// 458F - Field Variable backup table error
  "FVARS Both Tbl Cksum",	// 459F - Field Variable and backup table checksum error
  "FVARS Tbl Chksum Err",	// 460F - Field Variable checksum error
  "FVARS Bckup Tbl Cksm",	// 461F - Field Variable backup checksum error
  " FVARS Backup Init  ",	// 462F - Field variable backup init
  "Aut Swg Fr Door Open",	// 463F - Swing front hall door open flt
  "Aut Swg Rr Door Open",	// 464F - Swing rear hall door open flt
  "NTS Fault Up Dir Run",	// 465F - NTS fault during run in the up direction
  "NTS Fault Dn Dir Run",	// 466F - NTS fault during run in the down direction
  "Uncontrolld Leveling",	// 467F - Uncontrolled Leveling fault
  "Car Overspeed > 125%",	// 468F - Car overspeed greater than 125 %
  "UMotion Enc Velocity",	// 469F - Unintended motion detected from encoder velocity
  "UMotion SPB Velocity",	// 470F - Unintended motion detected from Safety processor velocity
  " GOV Overspeed Trip ",	// 471F - Governor overspeed trip
  "UMotion Ins DO No DZ",	// 472F - Unintended motion on inspection, door open and no door zone
  "UMotion Auto DO NoDZ",	// 473F - Unintended motion on automatic, door open and not door zone
  "ETS Failed On Top Fl",	// 474F - ETS failed on at the top floor
  "ETS Failed On Bot Fl",	// 475F - ETS failed on at the bottom floor
  "LW Load Table Fault ",	// 476F - Load Weigher load table fault
  "SPB Parameter Fault ",	// 477F - SPB configuration error - incorrect parameters loaded
  "   Shutdown Alarm   ",	// 478F - Car out of service and need mechanic
  "GOVRi Input On Fault",	// 479F - Governor Reset input GOVRi on fault
  "CED APS CAN Com Err ",	// 480F - Cedes Selector APS Can comm error
  "   CED APS Fault    ",	// 481F - Cedes Selector APS Fault
  "  CED Board Fault   ",	// 482F - Cedes Selector Interface board Fault
  "CED Brd Can Comm Err",	// 483F - Cedes Selector Interface board CAN comm fault
  " CEN I/O Failed_ On ",	// 484F - CEN Relay input/output failed on 
  " CEN I/O Failed Off ",	// 485F - CEN Relay input/output failed off
  "PALF Input Failed On",	// 486F - PALF input failed on 
  "PALF Input Faild Off",	// 487F - PALF input failed off	
  "CFLT/STE IO Fail Off",	// 488F = CFLT/STE C Contactor/input/output failed off
  "CFLT/STE IO Fail On ",	// 489F - CFLT/STE C Contactor/input/output failed on
  "DZ Off Redundancy Ck",	// 490F - DZ lost durning Redundancy Test
  "SUF I/O On Fault    ",	// 491F - SUF I/O on fault  
  "SDF I/O On Fault    ",	// 492F - SDF I/O on fault};
  "SPD I/O On Fault    ",	// 493F - SPD I/O on fault
  "SPD I/O Off Fault   ",	// 494F - SPD I/O off faultstatic  const char LCD_Fault_Xtra[2][21]={
  "CEN I/O Off Fault   ",	// 495F - CEN I/O off fault
  "HW Count Read Fault ",	// 496F - Hoistway Count Read fault
  "HW Slowdown Cnt Flt ",	// 497F - Hoistway Slowdown Count fault
  "CEN/TPH Failed Off  ",	// 498F - CEN/TPH failed off 
  "NTS Vars Setup Fault",	// 499F - NTS CPU parameter setup fault 
};

static  const char LCD_Fault_Xtra[2][21]={
  "ix=00,P= 1,F= G,#001",   // Line 1
  " AUT,  0:53:29  5/20",   // Line 2
};

static  const char LCD_Clear_Fault[5][21]={
  "   Press Enter to   ",
  " Clear the Fault Log",
  "      Fault Log     ",
  "       Cleared      ",
  "   No Occurrences   ",
};
static  const char LCD_Clear_Stats[4][21]={
  "   Press Enter to   ",
  "Clear Job Statistics",
  "   Job Statistics   ",
  "       Cleared      ",
};

static  const char LCD_Floor_SD[4][21]={
  "Floor  1 Count   ft ",
  " DS=                ",
  "P 1=                ",
  " US=                ",
};
static  const char LCD_Short_Floor_SD[4][21]={
  "Floor  1 Count   ft ",
  "SDS=                ",
  "P 1=                ",
  "SUS=                ",
};
static  const char LCD_Term_Limits[7][21]={
  "CPU Limit Count   ft",
  "UN =                ",
  "P 8=                ",
  "UT =                ",
  "DT =                ",
  "P 1=                ",
  "DN =                ",
};
static  const char LCD_lev_dist[4][21]={
  "  Leveling Distance ",
  "Dn Level =          ",
  "Up Level =          ",
  "PPI =               ",
};
static  const char LCD_final_stop[4][21]={
  "UL/DL Cnt=          ",
  "Offset Cnt=         ",
  "Vel=     EncVel=    ",
  "Pulses/Inch=        ",
};

static  const char LCD_aps_valid_fl_tbl[5][25]={
  "FL16....... 8......1",
  "   00000000 00000000",
  "CP16....... 8......1",
  "   00000000 00000000",
};


static  const char LCD_Timer_Menu[8][21] = {
  " Service Timer # 1  ",
  "Set Month/Day Timers",
  "Set Day of Week Tmrs",
  " Set Timer Service  ",
  "  Set Timer Logic   ",
  "View/Modify Tmr Stat",
  "Copy Day Of Week Tmr",
  "    Clear Timer     ",
};

static  const char LCD_Month_Day_Timer_Menu[4][21] = {
  "Mon/Day Tmr Date # 1",
  " Month/Day: 10/25   ",
  "   On Time: 06:53   ",
  "  Off Time: 07:53   ",
};

static  const char LCD_DOW_Timer_Menu[9][21] = {
  "DOW Timer: Saturday ",
  "DOW Timer: Sunday   ",
  "DOW Timer: Monday   ",
  "DOW Timer: Tuesday  ",
  "DOW Timer: Wednesday",
  "DOW Timer: Thursday ",
  "DOW Timer: Friday   ",
  "  On Time: 06:53    ",
  " Off Time: 07:53    ",
};

static  const char LCD_Timer_Status_Menu[4][21] = {
  "Timer Off / Disabled",
  "Timer On  / Disabled",
  "Timer Off / Enabled ",
  "Timer On  / Enabled ",
};

static  const char LCD_Timer_Service_Menu[17][21] = {
  "No Service Selected ",
  "      Parking       ",
  " Alternate Parking  ",
  "      Next Up       ",
  "      Up Peak       ",
  "     Down Peak      ",
  "  Alternate Lobby   ",
  " Alt Floor Security ",
  " Car Call Lockouts  ",
  " Group CC Lockouts  ",
  " Car Call Override  ",
  " Group CC Override  ",
  "      Sabbath       ",
  "   CC PB Security   ",
  " Floor Sec Table 2  ",
  " Floor Sec Table 3  ",
  " Floor Sec Table 4  ",
};

static  const char LCD_Timer_Copy_Menu[4][21] = {
  "Monday->Tues-Friday ",
  " Saturday -> Sunday ",
  " Mon->Tue-Fri Copied",
  "Sat -> Sunday Copied",
};

static  const char LCD_Timer_Clear_Menu[2][21] = {
  "Hit Ent to Clr Timer",
  "   Timer Cleared    ",
};

static  const char LCD_Timer_Logic[4][21] = {
  "     Input only     ",
  " Input=1 OR  Timer=1",
  " Input=1 AND Timer=0",
  " Input=1 AND Timer=1",
};


static  const char LCD_Version[4][21] = {
  "Car # 1 Car Type:   ",
  "   Controller # 1   ",
  " Software Version:  ",
  "      01.01.01      ",
};
// tagwbg 13
const char LCD_Software_Menu[9][21] = {
  " Software Version   ",
  " Run Power-Up Mode  ",
  " Test Watchdog Reset",
  " Reset Debug Vars   ",
  " SD Card Read/Write ",
  " SD Card Status     ",
  " WiFi Card Setup    ",
  " Preset Lim Velocity",
  " Trace Setup        ",
};

#if (Traction == 1)
#define Last_SW_Menu 8			   //0,1,2,3,4,5,6,7,8
static const char SW_Menu_list[9] = {0,1,2,3,4,5,6,7,8};
#else
#define Last_SW_Menu 7             //0,1,2,3,4,5,6,7,8
static const char SW_Menu_list[9] = {0,1,2,3,4,5,6,8,0};
#endif

static  const char Password_dpy[3][21] = {
  "Please Enter Correct",
  "      Password      ",
  "   Enter Password   ",
 };


static const char LCD_Jump_info[8][21] = {
  "Place car on Inspect",
  " and then hit Enter ",
  "Cannot Run Power-Up ",
  " Car Not on Inspect ",
  "Power-Up Jump Failed",
  " Invalid MRAM Code  ",
  "Invalid Start Addres",
  "Invalid Address Code",
};

static  const char LCD_Watchdog_info[4][21] = {
  "Place car on Inspect",
  " and then hit Enter ",
  " Cannot Run WD Test ",
  " Car Not on Inspect ",
};

static  const char LCD_SD_Read_Write[15][21] = {
  " Wrt Field Variable ",	 // 0
  " Read FieldVariables",	 // 1
  " Write NTS Proc Vars",	 // 2
  " Read NTS Proc Vars ",	 // 3
  " Wrt Job Setup Data ",	 // 4
  " Read Job Setup Data",	 // 5
  " Write Hoistway Data",	 // 6
  " Read Hoistway Data ",	 // 7
  " Wrt Service Timers ",	 // 8
  " Read Service timers",	 // 9
  " Write Security Data",	 // 10
  " Read Security Data ",	 // 11
  " Write Fault Log    ",	 // 12
  " Wrt LongTerm FltLog",	 // 13
  " Wrt Job Statistics ",	 // 14
};

#define Last_SD_Menu 16				//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
static const char SD_Menu_list[17] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};


static  const char LCD_Debug_Reset[2][21] = {
  "  Debug Variables   ",
  "       Reset        ",
};
static  const char LCD_Init_Vars[4][21] = {
  " Hit Enter to Preset",
  "Lim Vel/Mode to Exit",
  "  Limit Velocities  ",
  "Prset to Contract Sp",
};
  

static  const char LCD_SD_Read_Write_info[15][21] = {
  "    Place Car on    ",	// 0
  " Inspection to Run  ",   // 1
  " SD Card Read/Write ",	// 2
  "     Operation      ",	// 3
  "  Invalid Command   ",	// 4 - 0
  "Data Written to File",	// 5 - 1
  "Data Read from File ",	// 6 - 2
  "   File Not Found   ",	// 7 - 3
  "Cannot Read Car Data",	// 8 - 4
  "  Invalid Car Data  ",	// 9 - 5
  " Invalid Data Field ",	// 10 - 6
  " SD Card Init Error ",	// 11 - 7
  "  Media Init Error  ",	// 12 - 8
  " File Delete Error  ",	// 13 - 9
  " Overwrite File? Y  ",	// 14 - 10
};

static  const char LCD_SD_Card_Status[3][21] = {
  "Hit Enter for Status",
  "Init=0,HC=0,VStat=0 ",
  "Place Car on Inspect",
};

static  const char LCD_SD_Error[17][21] = {
  "    SD Card Error   ",
  "SD Card Not Detected",
  " SD Card Wrt Protect",
  " Idle State Failed  ",
  " Idle CMD0 Time Out ",
  "Block Len CMD TimOut",
  "Read CMD17 Time Out ",
  " Read Command Fail  ",
  "Read Response Error ",
  "Write CMD24 Time Out",
  " Write Command Fail ",
  "Invalid Card Voltage",
  " SDHC ACMD41 Tim-Out",
  " SDHC Response Error",
  "App ReqCMD55 Tim-Out",
  "OCR ReqCMD58 Tim-Out",
  "CRC offCMD59 Tim-Out",
};


static  const char LCD_Dpy_Adjust_Menu[4][21] = {
  " Set LCD Contrast   ",
  " Set LCD Brightness ",
  " -.............+ 40 ",
  " -........+  8      ",
};

	
#pragma section all_types

/*********************************************************/
/**        Debounce Push Buttons                        **/
/*********************************************************/
void DebouncePushButtons(void)
{
  CheckDownButton();
  CheckUpButton();
  CheckEnterButton();
  CheckModeButton();
}

/*********************************************************/
/**               Down Push Button Case                 **/
/*********************************************************/
void CheckDownButton(void)
{
	static int16 DownDebounceCnt = 0;	
	switch(DownButton)
	{
		case ButtonRelease:
			if(rdLCDio(Lcd_DN) == 1)  // =0 for not pressed and =1 for pressed. This is
			  // reverse of GALx1100BT
			{
				if(DownDebounceCnt< 2)
					DownDebounceCnt++;
				else
					DownButton = ButtonPress;				
			}
			else
				DownDebounceCnt = 0;			
			break;
		case ButtonPress:
			if(DownButtonStatus == ButtonDisable)
			{
				DownButton = ButtonRelease;
				DownDebounceCnt = 0;				
			}
			break;
		case ButtonPressAck:
			if(rdLCDio(Lcd_DN) == 0)
			{
				if(DownDebounceCnt > 0)
					DownDebounceCnt--;
				else
					DownButton = ButtonRelease;
				
			}
			else
				DownDebounceCnt = 2;			
			break;
	}
}


/*********************************************************/
/**                Up Push Button Case                  **/
/*********************************************************/
void CheckUpButton(void)
{

	static int16 UpDebounceCnt = 0;	
	switch(UpButton)
	{
		case ButtonRelease:
			if(rdLCDio(Lcd_UP) == 1) // =0 for not pressed and =1 for pressed. This is
			  // reverse of GALx1100BT
			{			      	        
				if(UpDebounceCnt< 2)
					UpDebounceCnt++;
				else
				{
					UpButton = ButtonPress;	
					wbg_04 = 11;					
				}
			
			}
			else   	       	             			
				UpDebounceCnt = 0;			
			break;
		case ButtonPress:
			if(UpButtonStatus == ButtonDisable)
			{
				UpButton = ButtonRelease;
				UpDebounceCnt = 0;	
				wbg_04 =  10;  // button released			
			}
			break;
		case ButtonPressAck:
			if(rdLCDio(Lcd_UP) == 0)
			{
				if(UpDebounceCnt > 0)
					UpDebounceCnt--;
				else
				{
					UpButton = ButtonRelease;					
					wbg_04 =  10;  // button released
				}				
			}
			else
				UpDebounceCnt = 2;			
			break;
	}

}

/*********************************************************/
/**             Enter Push Button Case                  **/
/*********************************************************/
void CheckEnterButton(void)
{

	static int16 EnterDebounceCnt = 0;	
	switch(EnterButton)
	{
		case ButtonRelease:
			if(rdLCDio(Lcd_ENTER) == 1)  // =0 for not pressed and =1 for pressed. This is
			  // reverse of GALx1100BT
			{
				if(EnterDebounceCnt< 2)
					EnterDebounceCnt++;
				else
					EnterButton = ButtonPress;				
			}
			else
				EnterDebounceCnt = 0;
			
			break;
		case ButtonPress:
			if(EnterButtonStatus == ButtonDisable)
			{
				EnterButton = ButtonRelease;
				EnterDebounceCnt = 0;				
			}
			break;
		case ButtonPressAck:
			if(rdLCDio(Lcd_ENTER) == 0)
			{
				if(EnterDebounceCnt > 0)
					EnterDebounceCnt--;
				else
					EnterButton = ButtonRelease;
				
			}
			else
				EnterDebounceCnt = 2;			
			break;
	}

}


/*********************************************************/
/**              Mode Push Button Case                  **/
/*********************************************************/
void CheckModeButton(void)
{

	static int16 ModeDebounceCnt = 0;	
	switch(ModeButton)
	{
		case ButtonRelease:
			if(rdLCDio(Lcd_MODE) == 1)  // =0 for not pressed and =1 for pressed. This is
			  // reverse of GALx1100BT
			{
				if(ModeDebounceCnt< 2)
					ModeDebounceCnt++;
				else
					ModeButton = ButtonPress;				
			}
			else
				ModeDebounceCnt = 0;
			break;
		case ButtonPress:
			if(ModeButtonStatus == ButtonDisable)
			{
				ModeButton = ButtonRelease;
				ModeDebounceCnt = 0;				
			}
			break;
		case ButtonPressAck:
			if(rdLCDio(Lcd_MODE) == 0)
			{
				if(ModeDebounceCnt > 0)
					ModeDebounceCnt--;
				else
					ModeButton = ButtonRelease;				
			}
			else
				ModeDebounceCnt = 2;			
			break;
	}

}


// *******************************************
// This is the read an input procedure
// If voltage return 1 if no voltage return 0
// *******************************************
int16 rdLCDio(int16 bitloc)
{
	
  	if(SIU.GPDI[bitloc].B.PDI == 1)
 		return(1);
   	else
  		return(0);
}

#define short_delay 100
#define short_delay_2 80
#define long_delay 120

// ******************************************************
// This is initialize the lcd procedure 2/9/2001
// ******************************************************
void init_LCD()
{
  //lint -esym(438,k)
  //lint -esym(550,k)
  int16 i,j,k;
  
	delay(10000);			// delay to allow LCD to reset

  	for(i=0; i<=19; i++)	// Clearing LCD Display Buffer
  	{
      	LCD_Display[0][i] = ' ';
      	LCD_Display[1][i] = ' ';
      	LCD_Display[2][i] = ' ';
      	LCD_Display[3][i] = ' ';
  	}
  	FaultInterrupt = false;
  	Password = 0;		    //  input password = 0;
  	PasswordFlag = false;
  	LCD_State_ix = 0;
  	LCD_Menu = 0;
  	
  	
	for (j=0;j<=20000;j++)
	{
		if ((j & 0x01) != 0)
			k = j+5;		// delay
	}
  	write_display("       GALAxy       ", 0,0);
	delay(10000);			// delay to allow LCD to reset
	delay(10000);			// delay to allow LCD to reset
 	write_display("   Initialization   ", 1,0);
	delay(10000);			// delay to allow LCD to reset

  	LCD_Pointer = 1;
    LCD_Upd_Dpy = 1;
}



// *****************************************************************
// This is re - initialize the lcd 10/30/2009 if sequence is pressed
// *****************************************************************

void reinit_LCD()
{

// This section checks for special sequence to restart the LCD interface
// Sequence: Mode + Up + Down + Up + Down
// Hitting enter resets the sequence

	static int16 LCD_sequence;
	int16 init = 0;


	if(EnterButton == 0)
		LCD_sequence = 0;
		
	switch(LCD_sequence)
	{
		case 0:	//check Mode
			if (ModeButton == 0)
				LCD_sequence = 1;
			break;
		case 1:	//check Up
			if ((ModeButton == 0) || (DownButton == 0) || (EnterButton == 0))
				LCD_sequence = 0;
			else if (UpButton == 0)
				LCD_sequence = 2;
			break;
		case 2:	//check Down
			if ((ModeButton == 0) || (UpButton == 0) || (EnterButton == 0))
				LCD_sequence = 0;
			else if (DownButton == 0)
				LCD_sequence = 3;
			break;
		case 3:	//check Up
			if ((ModeButton == 0) || (DownButton == 0) || (EnterButton == 0))
				LCD_sequence = 0;
			else if (UpButton == 0)
				LCD_sequence = 4;
			break;
		case 4:	//check Down
			if ((ModeButton == 0) || (UpButton == 0) || (EnterButton == 0))
				LCD_sequence = 0;
			else if (DownButton == 0)
				LCD_sequence = 5;
			break;
		case 5: // reinit LCD
			init_LCD();
			set_contrast(40);
			set_brightness(8);
			init = 1;
			LCD_sequence = 0;
			break;
		default:
			break;

	}
	
	if (init == 1)
	{
  		init = 0;
	  	LCD_Display[1][0] = ' ';
	  	LCD_Display[1][1] = ' ';
	  	LCD_Display[1][2] = ' ';
	  	LCD_Display[1][3] = ' ';
	  	LCD_Display[1][4] = ' ';
	  	LCD_Display[1][5] = 'C';
	  	LCD_Display[1][6] = 'O';
	  	LCD_Display[1][7] = 'N';
	  	LCD_Display[1][8] = 'T';
	  	LCD_Display[1][9] = 'R';
	  	LCD_Display[1][10] = 'O';
	  	LCD_Display[1][11] = 'L';
	  	LCD_Display[1][12] = 'L';
	  	LCD_Display[1][13] = 'E';
	  	LCD_Display[1][14] = 'R';
	  	LCD_Display[1][15] = ' ';
	  	LCD_Display[1][16] = ' ';
	  	LCD_Display[1][17] = ' ';
	  	LCD_Display[1][18] = ' ';
	  	LCD_Display[1][19] = ' ';
	}
}

// *****************************************************************
// Check if the mode button was pressed and released
// *****************************************************************

int16 Check_for_Mode_Button(void)
{
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == ButtonPress))
	{
	  ModeButton =  ButtonPressAck;
	  return 1;	
	}
	return 0;
}

// *****************************************************************
// Check if the Up button was pressed and released
// *****************************************************************
int16 Check_for_Up_Button(void)
{
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == ButtonPress))
	{
	  UpButton =  ButtonPressAck;
	  return 1;	
	}
	return 0;
}

// *****************************************************************
// Check if the Down button was pressed and released
// *****************************************************************
int16 Check_for_Down_Button(void)
{
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == ButtonPress))
	{
	  DownButton =  ButtonPressAck;
	  return 1;	
	}
	return 0;
}

// *****************************************************************
// Check if the Enter button was pressed and released
// *****************************************************************
int16 Check_for_Enter_Button(void)
{
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == ButtonPress))
	{
	  EnterButton =  ButtonPressAck;
	  return 1;	
	}
	return 0;
}

// *****************************************************************
// Check if Mode/Enter combo was pressed and released
// *****************************************************************
int16 Check_Mode_Enter_Combo(void)
{  // NOT RELIABLE  TODO_wbg
    if( Check_for_Mode_Button() && Check_for_Enter_Button())
    {
       // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return 1;
    }
    return 0;
}

//*************************************************
// This is refresh LCD Display
// it puts One character on the Display every cycle
//*************************************************
void Refresh_LCD()
{
  int16 i;
  int16 j = 0;
//  int16 fl_ix;
  int16 line_ptr;
  int16 start_ptr;
  static int16 grp_page_select;
  static int16 car_page_select;
  static int16 update_flt_status;
  static int16 up_dbn,dn_dbn,enter_dbn,mode_dbn;
  


  	petdog();
   	if(timers[tFlash] > 5)
	{
		LCD_Flash = 0;           
		if(timers[tFlash] > 10)
			timers[tFlash] = 0;
	}
  	else
		LCD_Flash = 1;

    DebouncePushButtons(); // tagwbg 02
// tagwbg 01 all debounce code that was here was removed and
// replaced with DebouncePushButtons()
 
	if (fvars[fvpassword] == 0)
		PasswordFlag = true;
	else if ((timers[tpassword] > (fvars[fvpwtime] + 100)) && (fvars[fvpwtime] > 0))
		PasswordFlag = false;			// shortest time-out is 10 seconds

 	if (statusf != prev_statusf)
	{
		prev_statusf = statusf;
	}
	if (Current_Fault != prev_fault)
	{
		if (Current_Fault !=0)
		{
	    	if ((fvars[fvfltdpy] == 1) && (LCD_Menu != 6))
	    	{				  // flag must be set and not in elevator status mode
				if (FaultInterrupt == false)
				{
					StoreLCDInfo();
	    			LCD_Menu = 24;
					LCD_Pointer = 24;
				}
				else
					LCD_Upd_Dpy = 1;
			}
		}
		prev_fault = Current_Fault;
	}
	if (procf != prev_procf)
	{
		prev_procf = procf;
	}
	if (servf != prev_servf)
	{
		prev_servf = servf;
	}


	reinit_LCD();

	if ((detect_sd() == 0) && (sd_init == 1))
	{
		sd_init = 0;
		sd_error = 0;
	}
	if ((det_sd_wr_prot() == 1) && (sd_init == 1))
	{
		sd_init = 0;
		sd_error = 0;
	}


//*********************************************************
//*********************************************************
//********************* MAIN MENU *************************
//*********************************************************
//*********************************************************

/*
if(Check_for_Up_Button())

if(Check_for_Down_Button())

if(Check_for_Mode_Button() )

if(Check_for_Enter_Button() )
*/

	switch(LCD_Menu)
  	{
    	case 0:	// main menu

		    if(Check_for_Mode_Button() )
		  	{  // rdLCDio(Lcd_MODE) =0 for not pressed 
		  	   // and =1 for pressed. This is
			   // reverse of GALx1100BT		  	    
		  	    LCD_Init = 1;		
				LCD_Menu = 6; // Jump to Elevator Status
				return;
		  	}
		  	if(Check_for_Enter_Button() )
		  	{
		  	    LCD_Menu = Main_Menu_list[LCD_Pointer];
		  	    LCD_Init = 1;		
		  	}
		  	if(Check_for_Up_Button())
		  	{    
		  	    LCD_Pointer--; // increment to the next menu item
		  	    if(LCD_Pointer < 1) LCD_Pointer = Last_Menu;
		  	    LCD_Upd_Dpy = 1;
		  	}
		  	if(Check_for_Down_Button())
		  	{
		  	    LCD_Pointer++; // increment to the next menu item
		  	    if(LCD_Pointer > Last_Menu) LCD_Pointer = 1;
		  	    LCD_Upd_Dpy = 1;
		  	}
		  	if(LCD_Upd_Dpy == 1)
		  	{
			  	LCD_Upd_Dpy = 0;
		  		line_ptr = (LCD_Pointer - 1) % 3;
		  		start_ptr = LCD_Pointer - line_ptr;
		  	    for(i=0; i<=19; i++)
		  	    {
					LCD_Display[0][i] = xLCD_Main_Menu[0][i];
					LCD_Display[1][i] = xLCD_Main_Menu[Main_Menu_list[start_ptr]][i];
					if ((start_ptr + 1) > Last_Menu)
				  	{
				  		LCD_Display[2][i] = ' ';
				  		LCD_Display[3][i] = ' ';
				  	}
				  	else
				  	{
						LCD_Display[2][i] = xLCD_Main_Menu[Main_Menu_list[start_ptr+1]][i];
						if ((start_ptr + 2) > Last_Menu)
					  		LCD_Display[3][i] = ' ';
						else
							LCD_Display[3][i] = xLCD_Main_Menu[Main_Menu_list[start_ptr+2]][i];
				  	}
		  	    }
				LCD_Display[line_ptr+1][0] = '>';		// show cursor
		  	}
		  	if(timers[tsec] != 0)
		  	{
		  	    LCD_Display[0][10] = 'A';
		  	    LCD_Display[0][11] = 'x';
		  	    LCD_Display[0][12] = 'y';
		  	}
		  	else
		  	{
		  	    LCD_Display[0][10] = ' ';
		  	    LCD_Display[0][11] = ' ';
		  	    LCD_Display[0][12] = ' ';
		  	}
	      	break;

		case 1:	// CAN
			LCD_Menu = CAN_Test(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 1;
			}
	      	break;

      	case 2:	// Adjustable Field Variables
      		LCD_Menu = CAN_Test(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 2;
			}
	      	break;
      	case 3:	// Job Statistics
      		LCD_Menu = job_statistics(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 3;
			}
	      	break;
      	case 4:	// Watchdog
            while(1)
            {
            	; // let watchdog timer reset system
            }
	      	break;
		case 5:		// Setup Car and Hall Calls
			LCD_Menu = setup_car_and_hall_calls(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
				LCD_Pointer = 5;
			}
	      	break;
      	case 6:	// SD Card test
      		LCD_Menu = TEST_SD(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 6;
			}
	      	break;

      	case 7:	// View Fault Log
      		LCD_Menu = view_fault_log(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 7;
			}
	      	break;

		case 8:		// Elevator Setup
			LCD_Menu = Elevator_Setup(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 8;
			}
			break;

		case 9:	// Display Hoistway and Floor Tables
			LCD_Menu = display_hoistway_floor_table(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 9;
			}
			break;

		case 10:		  // Set Service Timers
			LCD_Menu = set_service_timers(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 10;
			}
			break;

		case 11:	   //Security Option
			LCD_Menu = cc_pb_security(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 11;
			}
			break;

		case 12:	// Set Video Display
			LCD_Menu = adjust_lcd_display(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 12;
			}
			break;

		case 13:		// Software version and jump to bios for upload
			LCD_Menu = software_utilities(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 13;
			}
	      	break;

 		case 14:	// Diagnostics 
 			LCD_Menu = diagnostics(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 14;
			}
			break;

		case 24:		// Display Immediate fault
			LCD_Menu = display_immediate_fault(LCD_Menu);
			if (LCD_Menu == 0)
			{
				LCD_Upd_Dpy = 1;
//				LCD_Pointer = 15;
			}
			break;
				
		case 25:	  // Password
			password(LCD_Menu);	
			break;
		   
		default:
			break;

  	}
} // END OF Refresh_LCD

void write_LCD (void)
{
	int16 i;
	
	if ((upd_row_0 == 0) && (upd_row_1 == 0)) //switch pages
	{
		for(i=0; i<=19; i++)
		{
			if (LCD_Display[0][i] != LCD_Display_old[0][i])
			{
				LCD_Display_old[0][i] = LCD_Display[0][i];
				upd_row_0 = 1;
			}
			if (LCD_Display[1][i] != LCD_Display_old[1][i])
			{
				LCD_Display_old[1][i] = LCD_Display[1][i];
				upd_row_1 = 1;
			}
			
		}
		if ((upd_row_0 == 1) || (upd_row_1 == 1))
		{
			LCD_Cur_Pos = 0;
			for (i=0;i<=500;i++)
				;		// delay
			if (upd_row_0 == 1)
			{
				LCD_Line = 0;
			}
			else
			{
				LCD_Line = 1;
			}
		}
	}

	if ((upd_row_0 == 1) || (upd_row_1 == 1))
	{
		
		if(LCD_Line == 0)
	  	{
//			if (LCD_Display[0][LCD_Cur_Pos] == 0)		// Don't print a null character
//				LCD_Display[0][LCD_Cur_Pos] = ' ';			
			LCD_Cur_Pos++;
	  	}
	  	else
	  	{
//			if (LCD_Display[1][LCD_Cur_Pos] == 0)		// Don't print a null character
//				LCD_Display[1][LCD_Cur_Pos] = ' ';			
			LCD_Cur_Pos++;
	  	}
	  	

		if(LCD_Cur_Pos >= 20) //switch pages
		{

			for (i=0;i<=500;i++)
				;		// delay

		   	if(LCD_Line == 0)
		   	{
				
				upd_row_0 = 0;
				
			    LCD_Line = 1;
			    LCD_Cur_Pos = 0;
		   	}
		   	else
		   	{

				upd_row_1 = 0;

			    LCD_Line = 0;
			    LCD_Cur_Pos = 0;
		   	}

		}
	}
}


// ************************************************************
// Write Display Routine 
// Note: This routine should only be called when the car is on
// inspection or not alble to run 
//*************************************************************

void write_display(char *s, int16 row, int16 col)
{ // sets up buffer LCD_Display for all lines  // tagwbg 02
	int16 i;
	int16 j = 0;
	
	if ((row == 0) && (col < 20))
	{
		
		for (i=col;i<=19;i++)
		{
			if (s[j] == 0)
				break;
			else
				LCD_Display[0][i] = s[j++];
		}
	}
	
	if ((row == 1) && (col < 20))
	{
		for (i=col;i<=19;i++)
		{
			if (s[j] == 0)
				break;
			else
				LCD_Display[1][i] = s[j++];
		}
	}
	
	if ((row == 2) && (col < 20))
	{
		for (i=col;i<=19;i++)
		{
			if (s[j] == 0)
				break;
			else
				LCD_Display[2][i] = s[j++];
		}
	}

	if ((row == 3) && (col < 20))
	{
		for (i=col;i<=19;i++)
		{
			if (s[j] == 0)
				break;
			else
				LCD_Display[3][i] = s[j++];
		}
	}

	if (row <= 1)
	{
		
		upd_row_0 = 0;
		upd_row_1 = 0;
	
		for (i=0;i<=19;i++)
			write_LCD();  // tagwbg 04 write_display()
		
	}

	if (row <= 3)
		write_dpyspi(row);
}

///////////////////////

void Fill_Entire_Line_One_Char(char *s, int16 row)
{
    int16 i = 0;
    
    if(row > 3) return;
    if(row < 0) return;
    
	for(i = 0; i <= 19; i++)
	{
	  LCD_Display[row][i] = s[0];	
	}
}

void New_write_display(char *s, int16 row, int16 col)
{ // sets up buffer LCD_Display for all lines  // tagwbg 02
	int16 i;
	int16 j = 0;
	petdog();
	if ((row >= 0) && (row <= 3) && (col < 20) && (col >= 0))
	{
		for (i=col;i<=19;i++)
		{
			if (s[j] == NULL)
				break;
			else
				LCD_Display[row][i] = s[j++];
		}		
	}
}



//ZYM 1-15-2018
//--------------------------------------------------------
//Load Signal Char to LCD  Buffer                  
void Sprint_Lcd_SignalChar(int16 Row,int16 Col,unsigned char Ch)
{
	petdog();
	if((Row>=0) && (Row<4) && (Col<20) && (Col>=0))
	{
		LCD_Display[Row][Col] = Ch;	
	}
		
}



////////////////////////

void StoreLCDInfo(void)
{
	int16 i;
	
	if (LCD_State_ix < 2)
	{
		LCD_State[LCD_State_ix].LCD_Menu = LCD_Menu;
	    LCD_State[LCD_State_ix].LCD_Pointer = LCD_Pointer;
		LCD_State[LCD_State_ix].LCD_Dig_Point = LCD_Dig_Point;
	    
	    for(i=0; i<=19; i++)
	    {
		  LCD_State[LCD_State_ix].LCD_Display[0][i] = LCD_Display[0][i];
		  LCD_State[LCD_State_ix].LCD_Display[1][i] = LCD_Display[1][i];
		}
		LCD_State_ix++;
	}	 
}

void ReturnLCDMenu(void)
{
	int16 i;
	if (LCD_State_ix > 0)
	{
		LCD_State_ix--;
		LCD_Menu = LCD_State[LCD_State_ix].LCD_Menu;
    	LCD_Pointer = LCD_State[LCD_State_ix].LCD_Pointer;		
		LCD_Upd_Dpy = 1;
		LCD_Dig_Point = LCD_State[LCD_State_ix].LCD_Dig_Point;	
    
		for(i=0; i<=19; i++)
    	{
	  	 	LCD_Display[0][i] = LCD_State[LCD_State_ix].LCD_Display[0][i];
	     	LCD_Display[1][i] = LCD_State[LCD_State_ix].LCD_Display[1][i];
	    }
	}
	else 
	{
     	LCD_Menu = 0;  			
	  	LCD_Pointer = 0;
		for(i=0; i<=19; i++)
		{
			LCD_Display[0][i] = xLCD_Main_Menu[0][i];
			LCD_Display[1][i] = xLCD_Main_Menu[LCD_Pointer][i];
		}
		LCD_Upd_Dpy = 1;
	}
}

void clrLCDdpy(void)
{
	int16 j;
	
   for(j=0;j<20;j++)
   {
   	 LCD_Display[0][j] = ' ';
	 LCD_Display[1][j] = ' ';
   }
	 
}
//*********************************************************
//*********************************************************
//********************* MENU Subroutines ******************
//*********************************************************
//*********************************************************



//************************************
// Set Date and Time Routine - Menu 1
//************************************

int16 set_date_and_time (int16 lcd_menu)
{
	int16 i;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 mode_sel;

	if (LCD_Init == 1)
	{
		LCD_Init  = 0;
		menu_ptr = 0;
		menu_level = 0;
		mode_sel = 0;
  	    upd_dpy = 1;
	}

	if (menu_level == 0)
	{			// Select time menu

		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
				menu_ptr = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
	    	if(menu_ptr > 1)
				menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	menu_level = menu_ptr + 1;
		  	LCD_Sub_Init = 1;
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = xLCD_Main_Menu[1][i];
			  	LCD_Display[1][i] = LCD_Date_Time_Menu[0][i];
			  	LCD_Display[2][i] = LCD_Date_Time_Menu[1][i];
			  	LCD_Display[3][i] = ' ';
		    }
			LCD_Display[menu_ptr+1][0] = '>';		// show cursor
		}
	}
	else if (menu_level == 1)
	{
	  	if(LCD_Sub_Init == 1)	// first time through set time and date
	  	{
	  	    LCD_Sub_Init = 0;
	  	    upd_dpy = 1;
	  	    local_gettime();
	  	    local_getdate();
	  	    LCD_Year = d.year;
	  	    LCD_Month = d.month;
	  	    LCD_Day = d.day;
	  	    LCD_Hour = t.hour;
	  	    LCD_Minute = t.minute;
	  	    LCD_Time_Date[0] = ' ';
	  	    LCD_Time_Date[6] = ' ';
	  	    LCD_Time_Date[7] = ' ';
	  	    LCD_Time_Date[8] = ' ';
	  	    LCD_Time_Date[19] = ' ';
	  	    LCD_Time_Date[3] = ':';
	  	    LCD_Time_Date[11] = '/';
	  	    LCD_Time_Date[14] = '/';
	  	    mode_sel = 0;
	  	}
	  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	  	{
	  	    ModeButton = 1;  //
	  	    upd_dpy = 1;
	  	    mode_sel++;
	  	    if(mode_sel > 7)
			  	mode_sel = 0;
	  	}
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  	{
	  	    EnterButton = 1;
	  	    d.year = LCD_Year;
	  	    d.month = LCD_Month;
	  	    d.day = LCD_Day;
	  	    t.hour = LCD_Hour;
	  	    t.minute = LCD_Minute;
			local_setdate();
			local_settime();
			if (cons[carnmb] == Dispatcher)
				put_pkt_req_all_cars(109);		// Update time on all other cars
      		menu_level = 0;
			menu_ptr = 0;
	  	    upd_dpy = 1;
			return(lcd_menu);
	  	}
	  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  	{
	  	    UpButton = 1;  // incriment
	  	    upd_dpy = 1;
	  	    if(mode_sel == 0)	// incriment hours
	  	    {
				  LCD_Hour++;
				  if(LCD_Hour > 23)
				      LCD_Hour = 0;
	  	    }
	  	    if(mode_sel == 1)	// incriment Minutes
	  	    {
				  LCD_Minute++;
				  if(LCD_Minute > 59)
				      LCD_Minute = 0;
	  	    }
	  	    if(mode_sel == 2)	// incriment Months
	  	    {
				  LCD_Month++;
				  if(LCD_Month > 12)
				      LCD_Month = 1;
	  	    }
	  	    if(mode_sel == 3)	// incriment Days
	  	    {
				  LCD_Day++;
				  if(LCD_Day > 31)
				      LCD_Day = 1;
	  	    }
	  	    if(mode_sel == 4)	// incriment Year [1]
				  LCD_Year+=1;
	  	    if(mode_sel == 5)	// incriment Year [10]
				  LCD_Year+=10;
	  	    if(mode_sel == 6)	// incriment Year [100]
				  LCD_Year+=100;
	  	    if(mode_sel == 7)	// incriment Year [1000]
				  LCD_Year+=1000;
			if(LCD_Year > 9999) //corrects years above 10000
				  LCD_Year = 9999;
	  	}
	  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  	{
	  	    DownButton = 1;  // decriment
	  	    upd_dpy = 1;
	  	    if(mode_sel == 0)	// decriment hours
	  	    {
				  LCD_Hour--;
				  if(LCD_Hour < 0)
				      LCD_Hour = 23;
	  	    }
	  	    if(mode_sel == 1)	// decriment Minutes
	  	    {
				  LCD_Minute--;
				  if(LCD_Minute < 0)
				      LCD_Minute = 59;
	  	    }
	  	    if(mode_sel == 2)	// decriment Months
	  	    {
				  LCD_Month--;
				  if(LCD_Month < 1)
				      LCD_Month = 12;
	  	    }
	  	    if(mode_sel == 3)	// decriment Days
	  	    {
				  LCD_Day--;
				  if(LCD_Day < 1)
				      LCD_Day = 31;
	  	    }
	  	    if(mode_sel == 4)	// decriment Year [1]
				  LCD_Year-=1;
	  	    if(mode_sel == 5)	// decriment Year [10]
				  LCD_Year-=10;
	  	    if(mode_sel == 6)	// decriment Year [100]
				  LCD_Year-=100;
	  	    if(mode_sel == 7)	// decriment Year [1000]
				  LCD_Year-=1000;
			if(LCD_Year < 0) //corrects years below 0
			      LCD_Year = 0;
	  	}
	  	if(upd_dpy == 1)
	  	{
			upd_dpy = 0;
	  	    if(LCD_Hour < 10)
	  	    {
				  LCD_Time_Date[1] = ' ';
				  LCD_Time_Date[2] = (char)('0' + LCD_Hour);
	  	    }
	  	    else
	  	    {
				  itoa(LCD_Hour, LCD_String);
				  LCD_Time_Date[1] = LCD_String[0];
				  LCD_Time_Date[2] = LCD_String[1];
	  	    }
	  	    if(mode_sel == 0)	// flash hours
	  	    {
				  LCD_Flash_TMP[0] = LCD_Time_Date[1];
				  LCD_Flash_TMP[1] = LCD_Time_Date[2];
	  	    }
	  	    if(LCD_Minute < 10)
	  	    {
				  LCD_Time_Date[4] = '0';
				  LCD_Time_Date[5] = (char)('0' + LCD_Minute);
	  	    }
	  	    else
	  	    {
				  itoa(LCD_Minute, LCD_String);
				  LCD_Time_Date[4] = LCD_String[0];
				  LCD_Time_Date[5] = LCD_String[1];
	  	    }
	  	    if(mode_sel == 1)	// flash minutes
	  	    {
				  LCD_Flash_TMP[0] = LCD_Time_Date[4];
				  LCD_Flash_TMP[1] = LCD_Time_Date[5];
	  	    }
	  	    if(LCD_Month < 10)
	  	    {
				  LCD_Time_Date[9] = ' ';
				  LCD_Time_Date[10] = (char)('0' + LCD_Month);
	  	    }
	  	    else
	  	    {
				  itoa(LCD_Month, LCD_String);
				  LCD_Time_Date[9] = LCD_String[0];
				  LCD_Time_Date[10] = LCD_String[1];
	  	    }
	  	    if(mode_sel == 2)	// flash Months
	  	    {
				  LCD_Flash_TMP[0] = LCD_Time_Date[9];
				  LCD_Flash_TMP[1] = LCD_Time_Date[10];
	  	    }
	  	    if(LCD_Day < 10)
	  	    {
				  LCD_Time_Date[12] = '0';
				  LCD_Time_Date[13] = (char)('0' + LCD_Day);
	  	    }
	  	    else
	  	    {
				  itoa(LCD_Day, LCD_String);
				  LCD_Time_Date[12] = LCD_String[0];
				  LCD_Time_Date[13] = LCD_String[1];
	  	    }
	  	    if(mode_sel == 3)	// flash days
	  	    {
				  LCD_Flash_TMP[0] = LCD_Time_Date[12];
				  LCD_Flash_TMP[1] = LCD_Time_Date[13];
	  	    }
	  	    //itoa(LCD_Year, LCD_String);
	  	    sprintf(LCD_String,"%4d",LCD_Year);
			  for (i=0;i<=3;i++)
				if (LCD_String[i] == ' ')
					LCD_String[i] = '0';
									 
	  	    LCD_Time_Date[15] = LCD_String[0];
	  	    LCD_Time_Date[16] = LCD_String[1];
	  	    LCD_Time_Date[17] = LCD_String[2];
	  	    LCD_Time_Date[18] = LCD_String[3];

	  	    if(mode_sel == 4)	// flash year[1]
				  LCD_Flash_TMP[3] = LCD_Time_Date[18];

	  	    if(mode_sel == 5)	// flash year[10]
				  LCD_Flash_TMP[2] = LCD_Time_Date[17];

	  	    if(mode_sel == 6)	// flash year[100]
				  LCD_Flash_TMP[1] = LCD_Time_Date[16];

	  	    if(mode_sel == 7)	// flash year[1000]
				  LCD_Flash_TMP[0] = LCD_Time_Date[15];

	  	    for(i=0; i<=19; i++)
	  	    {
				  LCD_Display[1][i] = LCD_Set_Time_Date[0][i];
				  LCD_Display[2][i] = LCD_Set_Time_Date[1][i];
				  LCD_Display[3][i] = LCD_Time_Date[i];
	  	    }
	  	}
	  	else
	  	{
	  	    if(mode_sel == 0)	// flash hours
	  	    {
				  if(LCD_Flash == 0)
				  {
				      LCD_Display[3][1] = ' ';
				      LCD_Display[3][2] = ' ';
				  }
				  else
				  {
				      LCD_Display[3][1] = LCD_Flash_TMP[0];
				      LCD_Display[3][2] = LCD_Flash_TMP[1];
				  }
	  	    }
	  	    if(mode_sel == 1)	// flash minutes
	  	    {
				  if(LCD_Flash == 0)
				  {
				      LCD_Display[3][4] = ' ';
				      LCD_Display[3][5] = ' ';
				  }
				  else
				  {
				      LCD_Display[3][4] = LCD_Flash_TMP[0];
				      LCD_Display[3][5] = LCD_Flash_TMP[1];
				  }
	  	    }
	  	    if(mode_sel == 2)	// flash month
	  	    {
				  if(LCD_Flash == 0)
				  {
				      LCD_Display[3][9] = ' ';
				      LCD_Display[3][10] = ' ';
				  }
				  else
				  {
				      LCD_Display[3][9] = LCD_Flash_TMP[0];
				      LCD_Display[3][10] = LCD_Flash_TMP[1];
				  }
	  	    }
	  	    if(mode_sel == 3)	// flash day
	  	    {
				  if(LCD_Flash == 0)
				  {
				      LCD_Display[3][12] = ' ';
				      LCD_Display[3][13] = ' ';
				  }
				  else
				  {
				      LCD_Display[3][12] = LCD_Flash_TMP[0];
				      LCD_Display[3][13] = LCD_Flash_TMP[1];
				  }
	  	    }
			// flash years, units to ten thousands

			if(mode_sel == 4)	// flash years [1]
			{
				if(LCD_Flash == 0)
					LCD_Display[3][18] = ' ';
				else
					LCD_Display[3][18] = LCD_Flash_TMP[3];
			}
			if(mode_sel == 5)	// flash years [10]
			{
				if(LCD_Flash == 0)
					LCD_Display[3][17] = ' ';
				else
					LCD_Display[3][17] = LCD_Flash_TMP[2];
			}
					
			if(mode_sel == 6)	// flash years [100]
			{
				if(LCD_Flash == 0)
					LCD_Display[3][16] = ' ';
				else
					LCD_Display[3][16] = LCD_Flash_TMP[1];
			}
					
			if(mode_sel == 7)	// flash years [1000]
			{
				if(LCD_Flash == 0)
					LCD_Display[3][15] = ' ';
				else
				    LCD_Display[3][15] = LCD_Flash_TMP[0];
			}
	  	}
	}
	else if (menu_level == 2)
	{		// Display the day of week
  		if(LCD_Sub_Init == 1)	
  		{
	    	LCD_Sub_Init = 0;
			local_getdate();
	    	for(i=0; i<=19; i++)
	    	{
				LCD_Display[1][i] = LCD_DOW__Menu[7][i];
				LCD_Display[2][i] = LCD_DOW__Menu[d.dayofweek][i];
				LCD_Display[3][i] = ' ';
	    	}
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
      		ModeButton = 1;  //
      		menu_level = 0;
			menu_ptr = 1;
	  	    upd_dpy = 1;
			return(lcd_menu);
  		}
	}
	return(lcd_menu);
}


//****************************************
// CAN Test  - Menu 1
//****************************************

int16 CAN_Test (int16 lcd_menu)
{
	int16 i;
	int16 fv_line = 0;
	int16 start_ptr;
	int16 line_ptr;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 var_menu_ptr;
	static int16 var_ptr;
	static int16 mode_sel;
	
  	if(LCD_Init == 1)		// first time through procedure
 	{
 	    LCD_Init = 0;
 	    menu_ptr = 0;
 	    menu_level = 0;
 	    mode_sel = 0;
 	    upd_dpy = 1;
		var_menu_ptr = 0;
		var_ptr = 0;
 	}
	if (menu_level == 0)
	{			// Rotate through menu items
		if(Check_for_Mode_Button() )
		{	   // Exit to main menu
			lcd_menu = 0;
			return(lcd_menu);
		}
		if(Check_for_Up_Button())
		{
 		    upd_dpy = 1;
			var_ptr = 0;
		    menu_ptr--;
		    if(menu_ptr < 0)
			  	menu_ptr = CAN_Test_Last_Menu;
		}
		if(Check_for_Down_Button())
		{
 		    upd_dpy = 1;
			var_ptr = 0;
		    menu_ptr++;
						 
			if (menu_ptr > CAN_Test_Last_Menu)
				menu_ptr = 0;
		}

		if(Check_for_Enter_Button() )
		{
 		    upd_dpy = 1;
		  	menu_level = (int16)(CAN_Test_Menu_list[menu_ptr] + 1);
		  	var_menu_ptr = CAN_Test_Menu_list[menu_ptr];
		  	upd_dpy = 1;
		  	LCD_Sub_Init = 1;
			return(lcd_menu);
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			line_ptr = (menu_ptr % 3);
			start_ptr = menu_ptr - line_ptr;
		    for(i=0; i<=19; i++)
		    {
	  			LCD_Display[0][i] = xLCD_Main_Menu[2][i];
			  	LCD_Display[1][i] = CAN_Test_Items[CAN_Test_Menu_list[start_ptr]][i];
			  	if ((start_ptr + 1) > CAN_Test_Last_Menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else 
			  	{
				  	LCD_Display[2][i] = CAN_Test_Items[CAN_Test_Menu_list[start_ptr+1]][i];
			  		if ((start_ptr + 2) > CAN_Test_Last_Menu)
				  		LCD_Display[3][i] = ' ';
		    		else
					  	LCD_Display[3][i] = CAN_Test_Items[CAN_Test_Menu_list[start_ptr+2]][i];
			  	}
		    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}
	if ((menu_level >= 1) && (menu_level <= 13))
	{			// Change adjustable variables
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
  		    mode_sel++;
  		    upd_dpy = 1;
  		    if(mode_sel > 1)	// save field new value entered
  		    {
//				wr_one_fvar(var_menu_tbl[var_menu_ptr][var_ptr]);
				wrfvar();
  		    	send_galcom_fvar(MAIN_CPU,var_menu_tbl[var_menu_ptr][var_ptr]);
				mode_sel = 0;
				LCD_Dig_Point = 0;
  		    }
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
  		    if(mode_sel == 0)	// Leave the field var procedure
  		    {
				menu_level = 0;
				upd_dpy = 1;
				return(lcd_menu);
  		    }
  		    if(mode_sel > 0)
  		    {
				upd_dpy = 1;
				LCD_Dig_Point = set_adj_digit(fvars_max[var_menu_tbl[var_menu_ptr][var_ptr]],LCD_Dig_Point);			
  		    }
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
  		    UpButton = 1;  // incriment
  		    upd_dpy = 1;
  		    if(mode_sel == 0)
  		    {
				var_ptr--;
				if(var_ptr < 0)
				    var_ptr = last_var[var_menu_ptr];	 // Last fvars # 
  		    }
  		    else	// increment fieldvar
				adjust_variable(&fvars[var_menu_tbl[var_menu_ptr][var_ptr]],
						fvars_max[var_menu_tbl[var_menu_ptr][var_ptr]],
					fvars_min[var_menu_tbl[var_menu_ptr][var_ptr]],1,LCD_Dig_Point);
  		}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
  		    DownButton = 1;  // decriment
  		    upd_dpy = 1;
  		    if(mode_sel == 0)
  		    {
				var_ptr++;
				if(var_ptr > last_var[var_menu_ptr])	  // Last fvars # 
				    var_ptr = 0;
  		    }
  		    else	// decrement fieldvar
				adjust_variable(&fvars[var_menu_tbl[var_menu_ptr][var_ptr]],
						fvars_max[var_menu_tbl[var_menu_ptr][var_ptr]],
						fvars_min[var_menu_tbl[var_menu_ptr][var_ptr]],0,LCD_Dig_Point);
  		}

	   	if ((PasswordFlag == false) && (mode_sel > 0))
	   	{
	  		StoreLCDInfo();
			clrLCDdpy();
	  		LCD_Menu = 25;
	  		LCD_Init = 1;
			return(lcd_menu);
   	   	}

	    line_ptr = (var_ptr % 3);
		if(mode_sel > 0)
			fv_line = 2;
		else
			fv_line = line_ptr+1;
  	
  		if(upd_dpy == 1)
  		{
		    upd_dpy = 0;
		    
		    start_ptr = var_ptr - line_ptr;
		    
			
 		    for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = getcaps(CAN_Test_Items[var_menu_ptr][i]);
				if(mode_sel > 0)
				{
					LCD_Display[1][i] = LCD_Par[var_menu_tbl[var_menu_ptr][var_ptr]][i];
					LCD_Display[2][i] = LCD_Field_Vars[var_menu_tbl[var_menu_ptr][var_ptr]][i];
					LCD_Display[3][i] = ' ';
				}
				else
				{
					LCD_Display[1][i] = LCD_Field_Vars[var_menu_tbl[var_menu_ptr][start_ptr]][i];
					if ((start_ptr+1) > last_var[var_menu_ptr])
					{
						LCD_Display[2][i] = ' ';
						LCD_Display[3][i] = ' ';
					}
					else
					{
						LCD_Display[2][i] = LCD_Field_Vars[var_menu_tbl[var_menu_ptr][start_ptr+1]][i];
					
						if ((start_ptr+2) > last_var[var_menu_ptr])
							LCD_Display[3][i] = ' ';
						else
							LCD_Display[3][i] = LCD_Field_Vars[var_menu_tbl[var_menu_ptr][start_ptr+2]][i];
					}
				}
  		    }

			if(mode_sel > 0)
				Display_Var_Label(var_menu_ptr,var_menu_tbl[var_menu_ptr][var_ptr],fvars[var_menu_tbl[var_menu_ptr][var_ptr]]);
			for (i=0;i<=2;i++)
			{
				if (mode_sel > 0)
				{
					if ((var_menu_tbl[var_menu_ptr][var_ptr] == fvpassword) && (PasswordFlag == false))
					{
			  		    for(i=14; i<=19; i++)
							LCD_Display[fv_line][i] = '*';
					}
					else
					{
						display_variable(fv_line, 14, fvars_deci[var_menu_tbl[var_menu_ptr][var_ptr]], fvars[var_menu_tbl[var_menu_ptr][var_ptr]], 1);
					}
					break;		// only show one character when changing value
				}
				else if ((start_ptr+i) <= last_var[var_menu_ptr])
				{
					if ((var_menu_tbl[var_menu_ptr][start_ptr+i] == fvpassword) && (PasswordFlag == false))
					{
			  		    for(i=14; i<=19; i++)
							LCD_Display[i+1][i] = '*';
					}
					else
					{
						display_variable(i+1, 14, fvars_deci[var_menu_tbl[var_menu_ptr][start_ptr+i]], fvars[var_menu_tbl[var_menu_ptr][start_ptr+i]], 1);
					}
				}
			}
			LCD_Flash_TMP[1] = LCD_Display[fv_line][LCD_Dig_Loc];
			if (mode_sel == 0)		// show cursor
			{
				LCD_Display[fv_line][13] = '>';
			}
  		}
  		flash_digit(LCD_Flash_TMP[1], fv_line,LCD_Dig_Loc,(mode_sel > 0));
	}
	if (menu_level == 14)
	{
		menu_level = 0;
	}
	if (menu_level == 15)
	{
//		menu_level = Adjust_SPB_Parameters(menu_level);
		menu_level = 0;
		if (menu_level == 0)
			upd_dpy = 1;
	}
	if (menu_level == 16)
	{
		menu_level = Adjust_NTS_Parameters(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
	return(lcd_menu);
}


//************************************
// Job Statistics - Menu 3
//************************************

int16 job_statistics (int16 lcd_menu)
{
	int16 i;
	int32 tmp_long = 0;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 var_ptr;
	static int16 mode_sel;
	
    if(LCD_Init == 1) // First time through
    {
		LCD_Init = 0;
		upd_dpy = 1;
		menu_ptr = 0;
		menu_level = 0;
		var_ptr = 0;
  	}

	if (menu_level == 0)
	{			// Rotate through job statistics menu items
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
			  menu_ptr = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
		    if(menu_ptr > 1)
			  menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	menu_level = menu_ptr + 1;
			LCD_Sub_Init = 1;
	  		var_ptr = 0;
			return(lcd_menu);
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
		    {
		  		LCD_Display[0][i] = xLCD_Main_Menu[3][i];
				LCD_Display[1][i] = LCD_Job_Statistics[0][i];
				LCD_Display[2][i] = LCD_Job_Statistics[1][i];
				LCD_Display[3][i] = ' ';
		    }
			LCD_Display[menu_ptr+1][0] = '>';		// show cursor
		}
	}
	if (menu_level == 1)
	{			// View Job Statistics
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to menu level 1
			ModeButton = 1;  //
			menu_ptr = 0;
			menu_level = 0;
		  	var_ptr = 0;
		  	upd_dpy = 1;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    var_ptr--;
		    if(var_ptr < 0)
			  var_ptr = 17;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    var_ptr++;
		    if(var_ptr > 17)
			  var_ptr = 0;
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
		    {
			  LCD_Display[1][i] = LCD_Statistics[(var_ptr)][i];
			  if(i < 8)
			      LCD_Display[2][i] = ' ';
			  LCD_Display[3][i] = ' ';
		    }
		    if( (var_ptr) < 9)
			  	itoa(lvars[var_ptr], LCD_String);
		    else
		    {
			  	if((lvars[nmbuc] + lvars[nmbdc]) > 0)
			  	{
			      // Percent Answered In Less than 15 Second
			      	if(var_ptr == 13)
					   	tmp_long = ( ((lvars[3] + lvars[8]) * (int32)1000) /
							 (lvars[nmbuc]+lvars[nmbdc]) );
				      // Percent Answered In Less than 30 Second
				    if(var_ptr == 14)
					  	tmp_long = ( ((lvars[4] + lvars[9]) * (int32)1000) /
							(lvars[nmbuc]+lvars[nmbdc]) );
				      // Percent Answered In Less than 45 Second
				    if(var_ptr == 15)
					  	tmp_long = ( ((lvars[5] + lvars[10]) * (int32)1000) /
							(lvars[nmbuc]+lvars[nmbdc]) );
				    // Percent Answered In Less than 60 Second
				    if(var_ptr == 16)
						tmp_long = ( ((lvars[6] + lvars[11]) * (int32)1000) /
							(lvars[nmbuc]+lvars[nmbdc]) );
				      // Percent Answered In More than 60 Second
				    if(var_ptr == 17)
					  	tmp_long = ( ((lvars[7] + lvars[12]) * (int32)1000) /
							(lvars[nmbuc]+lvars[nmbdc]) );
				}
				itoa(tmp_long, LCD_String);
		    }
		    i=0;
		    while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		    {
				LCD_Display[2][(i+8)] = LCD_String[i];
				i++;
		    }
		    if( var_ptr > 8)
		    {
				if(tmp_long != 0)
				{
				    LCD_Display[2][(i+8)] = LCD_String[(i-1)];
				    LCD_Display[2][(i+7)] = '.';
				    i++;
				}
				LCD_Display[2][(i+8)] = ' ';
				i++;
				LCD_Display[2][(i+8)] = '%';
				i++;
		    }
		    while(i < 16)
		    {
				LCD_Display[2][(i+8)] = ' ';
				i++;
		    }
		}
	}
	if (menu_level == 2)
	{			// Clear Job Statistics
	  	if(LCD_Sub_Init == 1)	// first time through Clear Job Statistics
	  	{
	  	    LCD_Sub_Init = 0;
	  	    for(i=0; i<=19; i++)
	  	    {
				  LCD_Display[1][i] = LCD_Clear_Stats[0][i];
				  LCD_Display[2][i] = LCD_Clear_Stats[1][i];
				  LCD_Display[3][i] = ' ';
	  	    }
	  	}

		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to menu level 1
			ModeButton = 1;  //
			menu_level = 0;
			menu_ptr = 1;
		  	upd_dpy = 1;
			return(lcd_menu);
		}
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  	{
	    	EnterButton = 1;
	    	for(i=0; i<=19; i++)
	    	{
				  LCD_Display[1][i] = LCD_Clear_Stats[2][i];
				  LCD_Display[2][i] = LCD_Clear_Stats[3][i];
				  LCD_Display[3][i] = ' ';
	    	}
			clear_job_statistics();
	  	}
	}
	return(lcd_menu);
}


//***************************************
// Inputs and Outputs Subroutine - Menu 4
//***************************************

int16 inputs_and_outputs (int16 lcd_menu)
{

	int16 i;
	int16 start_ptr;
	int16 line_ptr;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	
    if(LCD_Init == 1) // First time through
    {
		upd_dpy = 1;
		LCD_Init = 0;
  	}

	if (menu_level == 0)
	{			// Rotate through input and output menu items
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
			  menu_ptr = Last_IO_Menu;
			if ((Dispatcher != cons[carnmb]) || ((gc_hall_io_dev == 0) && (nmb_grcan_dev == 0)))
			{
				if (menu_ptr == 1)
					menu_ptr = 0;
			}
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
			if ((Dispatcher != cons[carnmb]) || ((gc_hall_io_dev == 0) && (nmb_grcan_dev == 0)))
			{
				if (menu_ptr == 1)
					menu_ptr = 2;
			}
		    if(menu_ptr > Last_IO_Menu)
			  menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	menu_level = (int16)(IO_Menu_list[menu_ptr] + 1);
		  	LCD_Sub_Init = 1;		// for menus that do not need menu[2]
			return(lcd_menu);
		}
		if(upd_dpy == 1)
		{
		  	upd_dpy = 0;
	  		line_ptr = menu_ptr % 3;
	  		start_ptr = menu_ptr - line_ptr;
	  	    for(i=0; i<=19; i++)
	  	    {
	  			LCD_Display[0][i] = xLCD_Main_Menu[4][i];
				LCD_Display[1][i] = LCD_Input_Output[IO_Menu_list[start_ptr]][i];
				if ((start_ptr + 1) > Last_IO_Menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else
			  	{
					LCD_Display[2][i] = LCD_Input_Output[IO_Menu_list[start_ptr+1]][i];
					if ((start_ptr + 2) > Last_IO_Menu)
				  		LCD_Display[3][i] = ' ';
					else
						LCD_Display[3][i] = LCD_Input_Output[IO_Menu_list[start_ptr+2]][i];
			  	}
	  	    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}
	if (menu_level == 1)
	{
		menu_level = Display_Car_IO(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
  	}
	if (menu_level == 2)
	{
		menu_level = Display_Group_IO(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}
	if (menu_level == 3)
	{
		menu_level = Display_NTS_IO(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}
	if (menu_level == 4)
	{
		menu_level = Display_FPGA1_IO(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}
	if (menu_level == 5)
	{
		menu_level = Relocate_IO(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}
	return(lcd_menu);
}

//*********************************************
// Setup Car and Hall Calls Subroutine - Menu 5
//*********************************************

int16 setup_car_and_hall_calls (int16 lcd_menu)
{
	int16 i,j;
	int16 start_ptr;
	int16 line_ptr;
	int16 fl_ix;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 last_menu;
	static int16 mode_sel;
	static int16 floor_ix;
  	static int16 secure_yes;
	
	if (LCD_Init == 1)
	{
		LCD_Init = 0; 
		upd_dpy = 1;
		menu_level = 0;
		menu_ptr = 0;
		floor_ix = cons[bottomf];
 	    // Build the menu list
		j = 0;
 	    for (i=0;i<Num_Set_Calls_Menus;i++)
 	    {
			if (Dispatcher != cons[carnmb]) 
			{
				if ((i == 1) || (i == 2))
				{
					if (cons[rear] == 0)
						i = 6;
					else
						i = 3;
				}

				if ((i == 4) || (i == 5))
					i = 6;
			}
			else
			{
				if ((cons[rear] == 0) && (i == 3))
					i = 4; 
				if ((grtop_rfl == 0) && ((i == 4) || (i == 5)))
					i = 6; 
			}

			if ((cons[rear] == 0) && (i == 7))
				i = 8;
				
			if (i >= Num_Set_Calls_Menus)
				break;  // Have to break here if the last menu is not used
			else
			{
	 	    	Menu_list[j] = (int8)i;	
	 	    	j++;
			}
 	    }
 	    
 	    last_menu = j - 1;
 	    for (i=j;i<Num_Set_Calls_Menus;i++)
 	    	Menu_list[i] = Num_Set_Calls_Menus;
 	    	
   	}

	if (menu_level == 0)
	{			// Rotate through Call menu items
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
			  	menu_ptr = last_menu;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
						 
			if (menu_ptr > last_menu)
				menu_ptr = 0;

		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
			secure_yes = 0;
			mode_sel = 0;
		  	menu_level = (int16)(Menu_list[menu_ptr] + 1);
			return(lcd_menu);
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
	  		line_ptr = menu_ptr % 3;
	  		start_ptr = menu_ptr - line_ptr;
	  	    for(i=0; i<=19; i++)
	  	    {
	  			LCD_Display[0][i] = xLCD_Main_Menu[5][i];
				LCD_Display[1][i] = LCD_Setup_Calls[Menu_list[start_ptr]][i];
				if ((start_ptr + 1) > last_menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else
			  	{
					LCD_Display[2][i] = LCD_Setup_Calls[Menu_list[start_ptr+1]][i];
					if ((start_ptr + 2) > last_menu)
				  		LCD_Display[3][i] = ' ';
					else
						LCD_Display[3][i] = LCD_Setup_Calls[Menu_list[start_ptr+2]][i];
			  	}
	  	    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}
	if (menu_level == 1)
	{      	  // Setup a Car Call
  		if(mode_sel == 0)	// first time through Setup a Car Call
  		{
  			mode_sel = 1;
  		    upd_dpy = 1;
			floor_ix = cons[bottomf];
  		    for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = LCD_Calls[0][i];
				LCD_Display[1][i] = LCD_Calls[1][i];
				LCD_Display[2][i] = LCD_Calls[2][i];
				LCD_Display[3][i] = ' ';
  		    }
		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
			set_carcall(cons[carnmb],floor_ix);
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
			menu_level = 0;
			upd_dpy = 1;
			return(lcd_menu);
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
  		    UpButton = 1;  // incriment
  		    upd_dpy = 1;
  		    floor_ix++;
  		    if(floor_ix > cons[topf])
				  floor_ix = cons[bottomf];
  		}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
  		    DownButton = 1;  // decriment
  		    upd_dpy = 1;
  		    floor_ix--;
  		    if(floor_ix < cons[bottomf])
				  floor_ix = cons[topf];
  		}
 		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[2][18] = flmrk[floor_ix][0];
  		   LCD_Display[2][19] = flmrk[floor_ix][1];
  		}
	}
	if (menu_level == 2)
	{			// Setup a Down Hall Call
  		if(mode_sel == 0)	// first time through Setup a Down Call
  		{
  			mode_sel = 1;
  		    upd_dpy = 1;
			floor_ix = cons[bottomf];
      		for(i=0; i<=19; i++)
      		{
				LCD_Display[0][i] = LCD_Calls[0][i];
				LCD_Display[1][i] = LCD_Calls[1][i];
				LCD_Display[2][i] = LCD_Calls[3][i];
				LCD_Display[3][i] = ' ';
      		}
  		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
      		EnterButton = 1;
			set_dncall(floor_ix);
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
      		ModeButton = 1;  //
			menu_level = 0;
			upd_dpy = 1;
			return(lcd_menu);
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
      		UpButton = 1;  // incriment
      		upd_dpy = 1;
	      	floor_ix++;
	      	if(floor_ix > cons[topf])
			  	floor_ix = (cons[bottomf]+1);
	  	}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
      		DownButton = 1;  // decriment
      		upd_dpy = 1;
      		floor_ix--;
      		if(floor_ix < (cons[bottomf]+1))
				  floor_ix = cons[topf];
  		}
  		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[2][15] = flmrk[floor_ix][0];
  		   LCD_Display[2][16] = flmrk[floor_ix][1];
  		}
	}
	if (menu_level == 3)
	{			// Setup an Up Hall Call
  		if(mode_sel == 0)	// first time through Setup a Up Call
  		{
  			mode_sel = 1;
  		    upd_dpy = 1;
			floor_ix = cons[bottomf];
  		    for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = LCD_Calls[0][i];
				LCD_Display[1][i] = LCD_Calls[1][i];
				LCD_Display[2][i] = LCD_Calls[4][i];
				LCD_Display[3][i] = ' ';
  		    }
  		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
			set_upcall(floor_ix);
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
			menu_level = 0;
			upd_dpy = 1;
			return(lcd_menu);
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
  		    UpButton = 1;  // incriment
  		    upd_dpy = 1;
  		    floor_ix++;
  		    if(floor_ix > (cons[topf]-1))
				  floor_ix = cons[bottomf];
  		}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
  		    DownButton = 1;  // decriment
  		    upd_dpy = 1;
  		    floor_ix--;
  		    if(floor_ix < cons[bottomf])
				  floor_ix = (cons[topf]-1);
  		}
  		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[2][15] = flmrk[floor_ix][0];
  		   LCD_Display[2][16] = flmrk[floor_ix][1];
  		}
	}
	if (menu_level == 4)
	{      	  // Setup a rear Car Call
  		if(mode_sel == 0)	// first time through Setup a Car Call
  		{
  			mode_sel = 1;
  		    upd_dpy = 1;
			floor_ix = cons[bottomf];
  		    for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = LCD_Calls[0][i];
				LCD_Display[1][i] = LCD_Calls[1][i];
				LCD_Display[2][i] = LCD_Calls[5][i];
				LCD_Display[3][i] = ' ';
  		    }
  		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
			set_rcarcall(cons[carnmb],floor_ix);
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
			menu_level = 0;
			upd_dpy = 1;
			return(lcd_menu);
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
  		    UpButton = 1;  // incriment
  		    upd_dpy = 1;
  		    floor_ix++;
  		    if(floor_ix > cons[topf])
				  floor_ix = cons[bottomf];
  		}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
  		    DownButton = 1;  // decriment
  		    upd_dpy = 1;
  		    floor_ix--;
  		    if(floor_ix < cons[bottomf])
				  floor_ix = cons[topf];
  		}
  		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[2][17] = flmrk[floor_ix][0];
  		   LCD_Display[2][18] = flmrk[floor_ix][1];
  		}
	}
	if (menu_level == 5)
	{			// Setup a rear Down Hall Call
  		if(mode_sel == 0)	// first time through Setup a Down Call
  		{
  			mode_sel = 1;
  		    upd_dpy = 1;
			floor_ix = cons[bottomf];
      		for(i=0; i<=19; i++)
      		{
				LCD_Display[0][i] = LCD_Calls[0][i];
				LCD_Display[1][i] = LCD_Calls[1][i];
				LCD_Display[2][i] = LCD_Calls[6][i];
				LCD_Display[3][i] = ' ';
      		}
  		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
      		EnterButton = 1;
			set_rdncall(floor_ix);
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
      		ModeButton = 1;  //
			menu_level = 0;
			upd_dpy = 1;
			return(lcd_menu);
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
      		UpButton = 1;  // incriment
      		upd_dpy = 1;
	      	floor_ix++;
	      	if(floor_ix > cons[topf])
			  	floor_ix = (cons[bottomf]+1);
	  	}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
      		DownButton = 1;  // decriment
      		upd_dpy = 1;
      		floor_ix--;
      		if(floor_ix < (cons[bottomf]+1))
				  floor_ix = cons[topf];
  		}
  		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[2][16] = flmrk[floor_ix][0];
  		   LCD_Display[2][17] = flmrk[floor_ix][1];
  		}
	}
	if (menu_level == 6)
	{			// Setup a rear Up Hall Call
  		if(mode_sel == 0)	// first time through Setup a Up Call
  		{
  			mode_sel = 1;
  		    upd_dpy = 1;
			floor_ix = cons[bottomf];
  		    for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = LCD_Calls[0][i];
				LCD_Display[1][i] = LCD_Calls[1][i];
				LCD_Display[2][i] = LCD_Calls[7][i];
				LCD_Display[3][i] = ' ';
  		    }
			LCD_Display[0][22] = 'n';		// an Up call instead of a Up call
  		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
			set_rupcall(floor_ix);
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
			menu_level = 0;
			upd_dpy = 1;
			return(lcd_menu);
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
  		    UpButton = 1;  // incriment
  		    upd_dpy = 1;
  		    floor_ix++;
  		    if(floor_ix > (cons[topf]-1))
				  floor_ix = cons[bottomf];
  		}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
  		    DownButton = 1;  // decriment
  		    upd_dpy = 1;
  		    floor_ix--;
  		    if(floor_ix < cons[bottomf])
				  floor_ix = (cons[topf]-1);
  		}
  		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[2][16] = flmrk[floor_ix][0];
  		   LCD_Display[2][17] = flmrk[floor_ix][1];
  		}
	}
	if (menu_level == 7)
	{			// Secure front Car Call
  		if(mode_sel == 0)	// first time through Secure Car Call
  		{
  			
	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	      		EnterButton = 1;
				if (secure_yes == 0)
				{
					menu_level = 0;
					upd_dpy = 1;
				}
				else
				{
		      		mode_sel = 1;
		      		floor_ix = cons[bottomf];
		      		for(i=0; i<=19; i++)
		      		{
				  		LCD_Display[0][i] = LCD_Secure_Calls[0][i];
				  		LCD_Display[1][i] = LCD_Secure_Calls[1][i];
				  		LCD_Display[2][i] = LCD_Secure_Calls[2][i];
				  		LCD_Display[3][i] = LCD_Secure_Calls[3][i];
		      		}
		      		upd_dpy = 1;
				}
				return(lcd_menu);
		  	}
	  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  		{
	      		UpButton = 1;  // incriment
				secure_yes = 1;
		      	upd_dpy = 1;
		  	}
	  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  		{
	      		DownButton = 1;  // decriment
				secure_yes = 0;
		      	upd_dpy = 1;
		  	}
			if ((upd_dpy == 1) && (mode_sel == 0) && (menu_level == 7))
			{
				upd_dpy = 0;
	      		for(i=0; i<=19; i++)
	      		{
				  	LCD_Display[0][i] = getcaps(LCD_Setup_Calls[Menu_list[menu_ptr]][i]);
			  		LCD_Display[1][i] = LCD_Secure_Calls[5][i];
					if (secure_yes == 0)
			  			LCD_Display[2][i] = LCD_Secure_Calls[6][i];
					else
				  		LCD_Display[2][i] = LCD_Secure_Calls[7][i];
					LCD_Display[3][i] = ' ';
	      		}
			}
  		}
		else
		{
	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	      		EnterButton = 1;
	  		    upd_dpy = 1;
				fl_ix = (floor_ix-1)/32;
				if ((cc_sec_mask[fl_ix] & fl_mask[floor_ix]) == 0)
					cc_sec_mask[fl_ix] |= fl_mask[floor_ix];
				else
					cc_sec_mask[fl_ix] &= ~fl_mask[floor_ix];
				wrfvar();
	  		}
	  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	  		{
	      		ModeButton = 1;  //
				menu_level = 0;
				upd_dpy = 1;
				return(lcd_menu);
	  		}
	  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  		{
	      		UpButton = 1;  // incriment
	      		upd_dpy = 1;
		      	floor_ix++;
		      	if(floor_ix > cons[topf])
				  	floor_ix = cons[bottomf];
		  	}
	  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  		{
	      		DownButton = 1;  // decriment
	      		upd_dpy = 1;
	      		floor_ix--;
	      		if(floor_ix < cons[bottomf])
					  floor_ix = cons[topf];
	  		}
		}
  		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[3][16] = flmrk[floor_ix][0];
  		   LCD_Display[3][17] = flmrk[floor_ix][1];
		   	if ((cc_sec_mask[(floor_ix-1)/32] & fl_mask[floor_ix]) != 0)
  		   		LCD_Display[3][18] = '*';
			else
  		   		LCD_Display[3][18] = ' ';
  		}
	}
	if (menu_level == 8)
	{      	  // Secure a Rear Car Call
  		if(mode_sel == 0)	// first time through Secure a Rear Car Call
  		{
	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	      		EnterButton = 1;
				if (secure_yes == 0)
				{
					menu_level = 0;
					upd_dpy = 1;
				}
				else
				{
		  		    mode_sel = 1;
		  		    floor_ix = cons[bottomf];
		  		    for(i=0; i<=19; i++)
		  		    {
				  		LCD_Display[0][i] = LCD_Secure_Calls[0][i];
				  		LCD_Display[1][i] = LCD_Secure_Calls[1][i];
				  		LCD_Display[2][i] = LCD_Secure_Calls[2][i];
				  		LCD_Display[3][i] = LCD_Secure_Calls[4][i];
		  		    }
		      		upd_dpy = 1;
				}
				return(lcd_menu);
		  	}
	  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  		{
	      		UpButton = 1;  // incriment
				secure_yes = 1;
		      	upd_dpy = 1;
		  	}
	  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  		{
	      		DownButton = 1;  // decriment
				secure_yes = 0;
		      	upd_dpy = 1;
		  	}
			if ((upd_dpy == 1) && (mode_sel == 0) && (menu_level == 8))
			{
	      		for(i=0; i<=19; i++)
	      		{
				  	LCD_Display[0][i] = getcaps(LCD_Setup_Calls[Menu_list[menu_ptr]][i]);
			  		LCD_Display[1][i] = LCD_Secure_Calls[5][i];
					if (secure_yes == 0)
			  			LCD_Display[2][i] = LCD_Secure_Calls[6][i];
					else
				  		LCD_Display[2][i] = LCD_Secure_Calls[7][i];
					LCD_Display[3][i] = ' ';
	      		}
			}
  		}
		else
		{
	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	  		    upd_dpy = 1;
	  		    EnterButton = 1;
				fl_ix = (floor_ix-1)/32;
				if ((rcc_sec_mask[fl_ix] & fl_mask[floor_ix]) == 0)
					rcc_sec_mask[fl_ix] |= fl_mask[floor_ix];
				else
					rcc_sec_mask[fl_ix] &= ~fl_mask[floor_ix];					
				wrfvar();
	  		}
	  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	  		{
	  		    ModeButton = 1;  //
				menu_level = 0;
				upd_dpy = 1;
				return(lcd_menu);
	  		}
	  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  		{
	  		    UpButton = 1;  // incriment
	  		    upd_dpy = 1;
	  		    floor_ix++;
	  		    if(floor_ix > cons[topf])
					  floor_ix = cons[bottomf];
	  		}
	  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  		{
	  		    DownButton = 1;  // decriment
	  		    upd_dpy = 1;
	  		    floor_ix--;
	  		    if(floor_ix < cons[bottomf])
					  floor_ix = cons[topf];
	  		}
		}
  		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[3][16] = flmrk[floor_ix][0];
  		   LCD_Display[3][17] = flmrk[floor_ix][1];
		   	if ((rcc_sec_mask[(floor_ix-1)/32] & fl_mask[floor_ix]) != 0)
  		   		LCD_Display[3][18] = '*';
			else
  		   		LCD_Display[3][18] = ' ';
  		}
	}

	if (menu_level == 9)
	{      	  // Front Car Call	test
		if(mode_sel == 0)	// first time through Setup a Car Call
		{
			if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	    		EnterButton = 1;
				if (((enable_cc_test == 0) && (cancel_cc_test == 0)) 
					|| (continue_cc_test == 1))
				{
					menu_level = 0;
					upd_dpy = 1;
				}
				else
				{
			   		mode_sel = 1;
			   		floor_ix = cons[bottomf];
			   		for(i=0; i<=19; i++)
		   			{
			  			LCD_Display[0][i] = LCD_Car_Call_Test[0][i];
			  			LCD_Display[1][i] = LCD_Car_Call_Test[1][i];
			  			LCD_Display[2][i] = LCD_Car_Call_Test[2][i];
			  			LCD_Display[3][i] = LCD_Car_Call_Test[3][i];
			   		}
			   		upd_dpy = 1;
				}
				return (lcd_menu);
			}

			if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  		{
	    	 	UpButton = 1;  // incriment
		   		upd_dpy = 1;
				if ((cc_test_mask[0] != 0) || (cc_test_mask[1] != 0))
					continue_cc_test = 1; 
		 		else
		 			enable_cc_test = 1;
			}

	  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  		{
	    		DownButton = 1;  // decriment
		   		upd_dpy = 1;
				if ((cc_test_mask[0] != 0) || (cc_test_mask[1] != 0))
					continue_cc_test = 0;
				else
				{
					cancel_cc_test = 0;
					enable_cc_test = 0;
					cc_test_mask[0] = 0;
					cc_test_mask[1] = 0;
				}
			}
			if ((upd_dpy == 1) && (mode_sel == 0) && (menu_level == 9))
			{
				upd_dpy = 0;
    			for(i=0; i<=19; i++)
	    		{
					if ((cc_test_mask[0] != 0) || (cc_test_mask[1] != 0))
					{
					  	LCD_Display[0][i] = getcaps(LCD_Setup_Calls[Menu_list[menu_ptr]][i]);
		  				LCD_Display[1][i] = LCD_Enable_Car_Call_Test[3][i];
						if (continue_cc_test == 0)
		  					LCD_Display[2][i] = LCD_Enable_Car_Call_Test[5][i];
						else
			  				LCD_Display[2][i] = LCD_Enable_Car_Call_Test[4][i];
  		   				LCD_Display[3][i] = ' ';
					}
					else
					{
				  		LCD_Display[0][i] = getcaps(LCD_Setup_Calls[Menu_list[menu_ptr]][i]);
		  				LCD_Display[1][i] = LCD_Enable_Car_Call_Test[0][i];
						if (enable_cc_test == 0)
		  					LCD_Display[2][i] = LCD_Enable_Car_Call_Test[1][i];
						else
			  				LCD_Display[2][i] = LCD_Enable_Car_Call_Test[2][i];
  		   				LCD_Display[3][i] = ' ';
					}
	    		}
			}
		}
		else
		{
	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		  	{
		     	upd_dpy = 1;
		  		EnterButton = 1;
				fl_ix = (floor_ix-1)/32;
		
				if ((cc_test_mask[fl_ix] & fl_mask[floor_ix]) == 0)
					cc_test_mask[fl_ix] |= fl_mask[floor_ix];
				else
					cc_test_mask[fl_ix] &= ~fl_mask[floor_ix];					
			    
				set_carcall(cons[carnmb],floor_ix);

				if (cons[rear] !=0)
					set_rcarcall(cons[carnmb],floor_ix);
		  	}
	  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
			{
			    ModeButton = 1;  //
				menu_level = 0;
				upd_dpy = 1;
				return(lcd_menu);
	  		}
	  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  			{
  			    UpButton = 1;  // incriment
	  		    upd_dpy = 1;
	  		    floor_ix++;
  			    if(floor_ix > cons[topf])
					  floor_ix = cons[bottomf];
	  		}
	  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  		{
			    DownButton = 1;  // decriment
			    upd_dpy = 1;
  			    floor_ix--;
	  		    if(floor_ix < cons[bottomf])
					  floor_ix = cons[topf];
  			}
	
		}
	  		
  		if(upd_dpy == 1)
  		{
  		   upd_dpy = 0;
  		   LCD_Display[3][16] = flmrk[floor_ix][0];
  		   LCD_Display[3][17] = flmrk[floor_ix][1];

		   	if ((cc_test_mask[(floor_ix-1)/32] & fl_mask[floor_ix]) != 0)
	  	   		LCD_Display[3][18] = '*';
			else
		   		LCD_Display[3][18] = ' ';
  		}
	}
	return(lcd_menu);
}



//****************************************
// Run Elevator Status Subroutine - Menu 6
//****************************************

int16 run_elevator_status (int16 lcd_menu)
{
	int16 i,j;
	static int16 page_sel;
	static int16 mode_sel;
	static int16 cc_fl;
	static int16 upd_dpy;
	
  	if(LCD_Init == 1)	// first time through 
  	{
  	    LCD_Init = 0;
	  	page_sel = 0;
		mode_sel = 0;
  		Elevator_Status_Init(page_sel);
  	}
  	
  	if (mode_sel == 0)
  	{
	  	
	  	if (rdLCDio(Lcd_ENTER) == 1)
	  	{
		  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		  	{
				UpButton = 1;  // incriment
				next_trace += 10;		// Jump up 10 trace steps
		  	}
		  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		  	{
		      	DownButton = 1;  // decriment
				next_trace -= 10;		// Jump down 10 trace steps
		  	}
		  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		  	{
		      	ModeButton = 1;  //
				trig_dpy_ix = 1;		// Home Jump to start of trace
//				end_dpy_ix = 1;			// END Junp to end of trace
		  	}
	  	}
	  	else
	  	{
	  		
		  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		  	{
				UpButton = 1;  // incriment
		      	page_sel++;
				if (Trace_Stop == 1)
				{
					if (page_sel > 11)
						page_sel = 0;
				}
				else
				{
					if (page_sel > 13)
						page_sel = 0;
				}
		  	  	Elevator_Status_Init(page_sel);
		  	}
		  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		  	{
		      	DownButton = 1;  // decriment
		      	page_sel--;
				if (Trace_Stop == 1)
				{
					if (page_sel < 0)
						page_sel = 11;
				}
				else
				{
					if (page_sel < 0)
						page_sel = 13;
				}

			  	Elevator_Status_Init(page_sel);
		  	}
		  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		  	{
		      	ModeButton = 1;  //
				if (page_sel == 0)
				{
			 		mode_sel = 2;		// Go to trace command page
				  	upd_dpy = 1;
				}
				else
				{
				  	mode_sel = 0;
				  	page_sel = 0;
				  	Elevator_Status_Init(page_sel);
		 		}
		  	}
		  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		  	{
		  	  	EnterButton = 1;
	 			if (Trace_Stop == 1)
	 			{
	 				next_trace += 1;		// 1 trace step
				}
	 			else 
	 			{
				  	if (mode_sel == 0)
				  	{
					  	mode_sel = 1;
					  	cc_fl = 1;
					  	upd_dpy = 1;
				  	}
	 			}
		  	}
	  	}
	  	
	  	// Adjust trace index
	  	
		if (Trace_Stop == 1)
		{
			if (set_dpy_ix == 0)
			{
				set_dpy_ix = 1;
				trace_dpy_ix = trace_stop_ix;
			}
			if (trig_dpy_ix == 1)
			{
				trig_dpy_ix = 0;
				trace_dpy_ix = trace_stop_ix;
			}
			if (end_dpy_ix == 1)
			{
				end_dpy_ix = 0;
					
				if ((trace_stop_ix + 35) >= max_trace_ix)
					trace_dpy_ix = (int16)(35 - (max_trace_ix - trace_stop_ix));
				else
					trace_dpy_ix = (int16)(trace_stop_ix + 35);
				
			}
				
			if (next_trace != 0)
			{
				if (next_trace > 0)
				{
					
					if ((trace_dpy_ix + next_trace) >= max_trace_ix)
						trace_dpy_ix = next_trace - (max_trace_ix - trace_dpy_ix);
					else
						trace_dpy_ix += next_trace;
				}
				else
				{
					if ((trace_dpy_ix + next_trace) < 0)
						trace_dpy_ix = max_trace_ix + trace_dpy_ix + next_trace;	  // Restart the index
					else	
						trace_dpy_ix += next_trace;
				
				}
				next_trace = 0;
			}
		}	
		else
		{
			set_dpy_ix = 0;
			trace_dpy_ix = Trace_ix;
		}
		
	  	Elevator_Status(page_sel,&S_Trace[trace_dpy_ix]);
  	}
  	else if (mode_sel == 1)
  	{		// Set Car Call 
	  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  	{
			UpButton = 1;  // incriment
	      	cc_fl++;
			if (cc_fl > cons[topf])
				cc_fl = cons[bottomf];
			
			upd_dpy = 1;
	  	}
	  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  	{
	      	DownButton = 1;  // decriment
	      	cc_fl--;
			if (cc_fl < cons[bottomf])
				cc_fl = cons[topf];

			upd_dpy = 1;
	  	}
	  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	  	{
	      	ModeButton = 1;  //
		  	mode_sel = 0;
		   	Elevator_Status_Init(page_sel);
			return(lcd_menu);
	  	}
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  	{
	  	  	EnterButton = 1;
	  	  	if (set_carcall(cons[carnmb],cc_fl) == 0)
	  	  	{
	  	  		if (cons[rear] !=0)
					set_rcarcall(cons[carnmb],cc_fl);
	  	  	}
		  	mode_sel = 0;
		  	Elevator_Status_Init(page_sel);
			return(lcd_menu);
	  	}
	  	
	  	if (upd_dpy == 1)
	  	{
	  		upd_dpy = 0;
	  		j = 0;
  		    for(i=5; i<=15; i++)
  		    {
				LCD_Display[1][i] = Set_Calls_Window[0][j];
			  	LCD_Display[2][i] = Set_Calls_Window[1][j];
			  	LCD_Display[3][i] = Set_Calls_Window[2][j];
			  	j++;
  		    }
  			LCD_Display[2][13] = flmrk[cc_fl][0];
  			LCD_Display[2][14] = flmrk[cc_fl][1];
	  	}
  	}
  	else if (mode_sel == 2)
  	{		// Trace Command
	  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	  	{
	      	ModeButton = 1;  //
		  	mode_sel = 0;
			lcd_menu = 0;
			return(lcd_menu);
	  	}
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  	{
	  	  	EnterButton = 1;
		  	mode_sel = 0;
		  	set_dpy_ix = 0;
	 		if (Trace_Stop == 0)
				Trace_Trigger = 1;		// Stop trace
			else
				Trace_Stop = 0;			// Start trace (allow to run)
			
		  	Elevator_Status_Init(page_sel);
			return(lcd_menu);
	  	}
	  	
	  	if (upd_dpy == 1)
	  	{
	  		upd_dpy = 0;
	  		j = 0;
  		    for(i=5; i<=16; i++)
  		    {
				LCD_Display[0][i] = Trace_Window[0][j];
	 			if (Trace_Stop == 0)
			  		LCD_Display[1][i] = Trace_Window[1][j];
	 			else
				  	LCD_Display[1][i] = Trace_Window[2][j];
			  	LCD_Display[2][i] = Trace_Window[3][j];
				LCD_Display[3][i] = Trace_Window[4][j];
			  	j++;
  		    }
	  	}
  	}

	return(lcd_menu);
}

int16 view_fault_log (int16 lcd_menu)
{
	int16 i;
	int16 j;
	int16 k;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 menu_ptr_ret;
	static int16 dpy_flt_ix;
	static int16 dpy_nmb_flt;
	static int16 dpy_flt_line;
	static int16 dpy_ix;

	
	if (LCD_Init == 1)
	{
		LCD_Init = 0; 
		upd_dpy = 1;
		menu_level = 0;
		menu_ptr = 0;
  	}

	if (menu_level == 0)
	{			// Rotate through Fault menu items
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
			  menu_ptr = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
		    if(menu_ptr > 1)
			  menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	menu_level = (int16)(menu_ptr + 1);
  			menu_ptr_ret = menu_ptr;
  		    menu_ptr = 0;
  		    LCD_Sub_Init = 1;
		  	return(lcd_menu);
		}
	
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
		    {
	  			LCD_Display[0][i] = xLCD_Main_Menu[7][i];
			  	LCD_Display[1][i] = LCD_Fault_Log[0][i];
			  	LCD_Display[2][i] = LCD_Fault_Log[1][i];
			  	LCD_Display[3][i] = ' ';
		    }
			LCD_Display[menu_ptr+1][0] = '>';		// show cursor
		}
	}
	else if (menu_level == 1)
	{		// View Fault Log  or detailed fault log
	  	if (LCD_Sub_Init == 1)	// first time through View Fault Log
	  	{
	  		LCD_Sub_Init = 0;
	      	upd_dpy = 1;
		  	i = 0;
		  	j = 0;
			dpy_ix = 1;
			k = Flt_ix;
		  	k++;
		  	if (k >= max_flt_ix)
			  	k = 0;
		  	while ((i < max_flt_ix) && (j == 0))
		  	{
				if (f.Flts[k].code != 0)
					j = k;
				else
				{
					k++;
					if (k >= max_flt_ix)
						k = 0;
				}
				i++;
		
		  	}
			dpy_nmb_flt = (max_flt_ix - i) + 1;
	      	dpy_flt_ix = j;
			dpy_flt_line = 0;
	  	}
	  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	  	{
	      	ModeButton = 1;  
			if (dpy_flt_line == 0)
			{
				menu_level = 0;
				menu_ptr = menu_ptr_ret;
	 			upd_dpy = 1;
	 			return(lcd_menu);
			}
			else   // not at zero so step back one level
			{
				dpy_flt_line = 0;
			  	upd_dpy = 1;
			}
	  	}
	  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  	{
	      	UpButton = 1;  // incriment
	      	upd_dpy = 1;
			if(dpy_flt_line == 0)
			{
			  	i = 0;
			  	j = 0;
			  	k = dpy_flt_ix;
				if (k == Flt_ix)
					dpy_ix = 0;	 //  will be changed to 1 below
			  	k++;
			  	if (k >= max_flt_ix)
				  	k = 0;
			  	while ((i < max_flt_ix) && (j == 0))
			  	{
					if (f.Flts[k].code != 0)
						j = k;
					else
					{
						k++;
						if (k >= max_flt_ix)
							k = 0;
					}
					i++;
			
			  	}
				if ((i > 1) && (((max_flt_ix - i) + 1) > dpy_nmb_flt))
					dpy_nmb_flt = (max_flt_ix - i) + 1;
				if (dpy_flt_ix != j)
				{
					if (dpy_ix < max_flt_ix)
						dpy_ix++;
					else
						dpy_ix = 1;
				}
				else if (dpy_ix == 0)
					dpy_ix = 1;
	      		dpy_flt_ix = j;
			}
			else
			{
				
				if (dpy_flt_line < 24)
					dpy_flt_line ++;
				else
				{
					if (((f.Flts[dpy_flt_ix].code == f_MRCAN_device_fault) || 
						 (f.Flts[dpy_flt_ix].code == f_CTCAN_device_fault) ||
						 (f.Flts[dpy_flt_ix].code == f_GRCAN_device_fault)) &&
						(f.Flts[dpy_flt_ix].device != 0) &&
						(f.Flts[dpy_flt_ix].prog_flag1 != 0))
					{ // display device and device fault with count
						dpy_flt_line = 1;
					}
					else if (((f.Flts[dpy_flt_ix].code == f_MRCAN_device_reset) || 
						 (f.Flts[dpy_flt_ix].code == f_CTCAN_device_reset) ||
						 (f.Flts[dpy_flt_ix].code == f_GRCAN_device_reset)) &&
						(f.Flts[dpy_flt_ix].device != 0))
					{		// Display Device and device requested initialization
						dpy_flt_line = 1;
					}
					else
						dpy_flt_line = 2;

				}
			}
	  	}
	  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  	{
	      	DownButton = 1;  // decriment
	      	upd_dpy = 1;
			if(dpy_flt_line == 0)
			{
			  	i = 0;
			  	j = 0;
			  	k = dpy_flt_ix;
			  	k--;
			  	if (k < 0)
				  	k = max_flt_ix - 1;
			  	while ((i < max_flt_ix) && (j == 0))
			  	{
					if (f.Flts[k].code != 0)
						j = k;
					else
					{
						k--;
				    	if (k < 0)
					    	k = max_flt_ix - 1;
					}
					i++;
			
			  	}
				if ((i > 1) && (((max_flt_ix - i) + 1) > dpy_nmb_flt))
					dpy_nmb_flt = (max_flt_ix - i) + 1;
				if (dpy_flt_ix != j)
				{
					if (dpy_ix > 1)
						dpy_ix--;
					else
						dpy_ix = dpy_nmb_flt;
				}
	      		dpy_flt_ix = j;
			}
			else
			{
				if (dpy_flt_line > 1)
					dpy_flt_line--;
				else
					dpy_flt_line = 24;
				
				if (dpy_flt_line == 1)
				{
					if (((f.Flts[dpy_flt_ix].code == f_MRCAN_device_fault) || 
						 (f.Flts[dpy_flt_ix].code == f_CTCAN_device_fault) ||
						 (f.Flts[dpy_flt_ix].code == f_GRCAN_device_fault)) &&
						(f.Flts[dpy_flt_ix].device != 0) &&
						(f.Flts[dpy_flt_ix].prog_flag1 != 0))
					{ // display device and device fault with count
						dpy_flt_line = 1;
					}
					else if (((f.Flts[dpy_flt_ix].code == f_MRCAN_device_reset) || 
						 (f.Flts[dpy_flt_ix].code == f_CTCAN_device_reset) ||
						 (f.Flts[dpy_flt_ix].code == f_GRCAN_device_reset)) &&
						(f.Flts[dpy_flt_ix].device != 0))
					{		// Display Device and device requested initialization
						dpy_flt_line = 1;
					}
					else
						dpy_flt_line = 24;

				}

			}
	  	}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
			if (dpy_flt_line == 0)
			{
				if (((f.Flts[dpy_flt_ix].code == f_MRCAN_device_fault) || 
					 (f.Flts[dpy_flt_ix].code == f_CTCAN_device_fault) ||
					 (f.Flts[dpy_flt_ix].code == f_GRCAN_device_fault)) &&
					(f.Flts[dpy_flt_ix].device != 0) &&
					(f.Flts[dpy_flt_ix].prog_flag1 != 0))
				{ // display device and device fault with count
					dpy_flt_line = 1;
				}
				else if (((f.Flts[dpy_flt_ix].code == f_MRCAN_device_reset) || 
					 (f.Flts[dpy_flt_ix].code == f_CTCAN_device_reset) ||
					 (f.Flts[dpy_flt_ix].code == f_GRCAN_device_reset)) &&
					(f.Flts[dpy_flt_ix].device != 0))
				{		// Display Device and device requested initialization
					dpy_flt_line = 1;
				}
				else
					dpy_flt_line = 2;
			}
			upd_dpy = 1;
		}

	  	if(upd_dpy != 0)
	  	{
			fault_display(dpy_flt_line, dpy_flt_ix, dpy_ix);
	     	upd_dpy = 0;
	  	}
	}
	else if (menu_level == 2)
	{				// Clear Fault Log
  		if(LCD_Sub_Init == 1)	// first time through View Fault Log
  		{
	  		LCD_Sub_Init = 0;
  		    for(i=0; i<=19; i++)
  		    {
				  LCD_Display[0][i] = LCD_Clear_Fault[0][i];
				  LCD_Display[1][i] = LCD_Clear_Fault[1][i];
				  LCD_Display[2][i] = ' ';
				  LCD_Display[3][i] = ' ';
  		    }
  		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
  		    for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = LCD_Clear_Fault[2][i];
				LCD_Display[1][i] = LCD_Clear_Fault[3][i];
				  LCD_Display[2][i] = ' ';
				  LCD_Display[3][i] = ' ';
  		    }
	      	for(i=0; i<max_flt_ix; i++)
	      	{
				f.Flts[i].code = 0;
				f.Flts[i].count = 0;
	      	}
			Flt_ix = 0;
  		    Update_All_Flt();
#if (GALaxy_4 == 1)
			clr_mrcan_dev_comm_loss();
			clr_ctcan_dev_comm_loss();
#else
			clr_can_dev_comm_loss();
#endif
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
			menu_level = 0;
			menu_ptr = menu_ptr_ret;
			upd_dpy = 1;
			return(lcd_menu);
  		}
	}
	else
		menu_level = 0;
	return(lcd_menu);
}

int16 display_hoistway_floor_table (int16 lcd_menu)
{
	int16 i;
	int16 start_ptr;
	int16 line_ptr;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	
    if(LCD_Init == 1) // First time through
    {
		upd_dpy = 1;
		LCD_Init = 0;
  	}

	if (menu_level == 0)
	{			// Rotate through input and output menu items
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
			  menu_ptr = Last_Hoistway_Menu;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
		    if(menu_ptr > Last_Hoistway_Menu)
			  menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	menu_level = (int16)(Hoistway_Menu_list[menu_ptr] + 1);
		  	LCD_Sub_Init = 1;		// for menus that do not need menu[2]
			return(lcd_menu);
		}
		if(upd_dpy == 1)
		{
		  	upd_dpy = 0;
	  		line_ptr = menu_ptr % 3;
	  		start_ptr = menu_ptr - line_ptr;
	  	    for(i=0; i<=19; i++)
	  	    {
	  			LCD_Display[0][i] = xLCD_Main_Menu[9][i];
				LCD_Display[1][i] = LCD_Dpy_Hoistway[Hoistway_Menu_list[start_ptr]][i];
				if ((start_ptr + 1) > Last_Hoistway_Menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else
			  	{
					LCD_Display[2][i] = LCD_Dpy_Hoistway[Hoistway_Menu_list[start_ptr+1]][i];
					if ((start_ptr + 2) > Last_Hoistway_Menu)
				  		LCD_Display[3][i] = ' ';
					else
						LCD_Display[3][i] = LCD_Dpy_Hoistway[Hoistway_Menu_list[start_ptr+2]][i];
			  	}
	  	    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}
	else if (menu_level == 1)  
	{
		menu_level = display_hoistway(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}
	else if (menu_level == 2)
	{
		menu_level = display_short_floor_sd(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}
	else if (menu_level == 3)
	{
		menu_level = display_cpu_limits(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}
	else if (menu_level == 4)
	{
		menu_level = display_dz_off(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}
	else if (menu_level == 5)
	{
		menu_level = dpy_aps_valid_fl_table(menu_level);
		if (menu_level == 0)
		{
			upd_dpy = 1;
		}
	}

	return(lcd_menu);
}

int16 set_service_timers (int16 lcd_menu)
{
	lcd_menu = 0;
	return (lcd_menu);
/*
	int16 i;
		
	if (LCD_Pointer == 10)
	{
		upd_dpy = 1;
		LCD_Pointer++; 
		Menu_level = 0;
	    for(i=0; i<=19; i++)
	  		LCD_Display[0][i] = LCD_Main_Menu[10][i];
  	}

	if (Menu_level == 0)
	{			// Rotate through Call menu items
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
		{	   // Exit to main menu
			LCD_Mode_PB = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    upd_dpy = 1;
		    LCD_Pointer++;
		    if(LCD_Pointer > 20)
			  LCD_Pointer = 11;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    upd_dpy = 1;
		    LCD_Pointer--;
		    if(LCD_Pointer < 11)
			  LCD_Pointer = 20;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
		  	Timer_Number = (LCD_Pointer - 11);
			LCD_Pointer = 0;
			Menu_level = 1;
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
			  	LCD_Display[1][i] = LCD_Timer_Menu[0][i];
			LCD_Display[1][19] = (char)((LCD_Pointer - 11) + '0'); 
		}
	}

	if (Menu_level > 0)
		Service_Timers();
	
	*/
	return(lcd_menu);
}

int16 cc_pb_security (int16 lcd_menu)
{
	lcd_menu = 0;
	return (lcd_menu);
/*
	int16 i;
	
	if(LCD_Pointer == 11)
	{
		LCD_Pointer++;
		Menu_level = 0;
		LCD_Sub_Point = 0;       //  used to navigate thru  the  Security_dpy Table
		LCD_Dig_Point = 0;	
	}
	if(Menu_level == 0)
	{
		for(i=0;i<=19;i++)
		{
			LCD_Display[0][i] = LCD_Main_Menu[LCD_Pointer-1][i];
		}

		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{			   
			LCD_Enter_PB = 1;

			if(LCD_Sub_Point == 0)
			{		   // Setup codes per floor
				Menu_level = 2;
				LCD_FloorNum = 1;
				Security_Dpy_Index = 0;
			}
			else if(LCD_Sub_Point == 1)
			{					 // Remove all codes for all floors
				Menu_level = 6;
				Security_Dpy_Index = 10;
			}			
			LCD_Dig_Point = 0;
			LCD_Sub_Point = 0; 
			clrLCDdpy(); 					
			return;	
		}
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
		{
			LCD_Mode_PB = 1;
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
			LCD_UP_PB = 1;  // incriment
			if (Dispatcher == cons[carnmb])
			{
				LCD_Sub_Point++;
				if (LCD_Sub_Point > 1)
					LCD_Sub_Point = 0;
			}
			else
				LCD_Sub_Point = 0;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
			LCD_DN_PB = 1;  // decriment
			if (Dispatcher == cons[carnmb])
			{
				LCD_Sub_Point--;
				if (LCD_Sub_Point < 0)
					LCD_Sub_Point = 1;
			}
			else
				LCD_Sub_Point = 0;
		}
		for(i=0;i<=19;i++)
		{
			LCD_Display[1][i] = Security_dpy[LCD_Sub_Point][i];
		}			

	}
	if(Menu_level == 1)					   // Check to see if user still wants to enter command
	{									   // Verify Yes or no
		for(i=0;i<=19;i++)
		{
			LCD_Display[0][i] = Security_dpy[Security_Dpy_Index][i];
		}				
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{			   
			LCD_Enter_PB = 1;					
			if(Security_Dpy_Index == 3)
			{
				if(LCD_Sub_Point == 1)
				{
					if (code_floor_access == 1)
						Code_Num[0] |= 0x80;
					else
						Code_Num[0] &= 0x7F;
					gc_Add_Sec_Fl_Req = (uint8)LCD_FloorNum;
					gc_Add_Sec_Code[0] = Code_Num[0];
					gc_Add_Sec_Code[1] = Code_Num[1];
					gc_Add_Sec_Code[2] = Code_Num[2];
					gc_Add_Sec_Code[3]	= Code_Num[3];
					if (Add_Security_Code(LCD_FloorNum, &CodeTable, LookUpTable, Code_Num) == 1)
						put_pkt_req_all_cars(5);
				}
				Menu_level = 3;
				Security_Dpy_Index = Floor_Dpy_Offset;
				LCD_Sub_Point = 1;
				return;
			}
			else if(Security_Dpy_Index == 4)
			{
				if(LCD_Sub_Point == 1)
				{
					if (code_floor_access == 1)
						Code_Num[0] |= 0x80;
					else
						Code_Num[0] &= 0x7F;
					gc_Del_Sec_Fl_Req = (uint8)LCD_FloorNum;
					gc_Del_Sec_Code[0] = Code_Num[0];
					gc_Del_Sec_Code[1] = Code_Num[1];
					gc_Del_Sec_Code[2] = Code_Num[2];
					gc_Del_Sec_Code[3]	= Code_Num[3];
					if (Delete_Security_Code(LCD_FloorNum, &CodeTable, LookUpTable, Code_Num) == 1)
						put_pkt_req_all_cars(6);
				}
				Menu_level = 3;
				Security_Dpy_Index = Floor_Dpy_Offset;
				LCD_Sub_Point = 2;
				return;
			}
			else if(Security_Dpy_Index == 5)
			{
				if(LCD_Sub_Point == 1)
				{
					gc_Del_All_Codes_Fl_Req = (uint8)LCD_FloorNum;
					put_pkt_req_all_cars(33);
					Clear_All_Codes_One_Floor(LCD_FloorNum, &CodeTable, LookUpTable);
				}
				Menu_level = 3;
				Security_Dpy_Index = Floor_Dpy_Offset;
				LCD_Sub_Point = 3;
				return;
			}
			else if(Security_Dpy_Index == 10)
			{		  // Add one code all floors
				if(LCD_Sub_Point == 1)
				{
					if (code_floor_access == 1)
						Code_Num[0] |= 0x80;
					else
						Code_Num[0] &= 0x7F;
					gc_Add_Sec_Fl_Req = 0xFF;
					gc_Add_Sec_Code[0] = Code_Num[0];
					gc_Add_Sec_Code[1] = Code_Num[1];
					gc_Add_Sec_Code[2] = Code_Num[2];
					gc_Add_Sec_Code[3]	= Code_Num[3];
					Add_One_Code_All_Floors(1, 0, Code_Num);
					put_pkt_req_all_cars(5);
				}
				Menu_level = 6;
				LCD_Sub_Point = 1;
				return;
			}							
			else if(Security_Dpy_Index == 11)
			{			 // Delete one code all floors
				if(LCD_Sub_Point == 1)
				{
					if (code_floor_access == 1)
						Code_Num[0] |= 0x80;
					else
						Code_Num[0] &= 0x7F;
					gc_Del_Sec_Fl_Req = 0xFF;
					gc_Del_Sec_Code[0] = Code_Num[0];
					gc_Del_Sec_Code[1] = Code_Num[1];
					gc_Del_Sec_Code[2] = Code_Num[2];
					gc_Del_Sec_Code[3]	= Code_Num[3];
					Delete_One_Code_All_Floors(1, 0, Code_Num);
					put_pkt_req_all_cars(6);
				}
				Menu_level = 6;
				LCD_Sub_Point = 1;
				return;
			}							
			else if(Security_Dpy_Index == 12)
			{		   // Delete all codes alll floors
				if(LCD_Sub_Point == 1)
				{
					gc_Del_All_Codes_Fl_Req = 0xFF;
					put_pkt_req_all_cars(33);
					Clear_All_Codes_All_Floors(1,0);
				}
				Menu_level = 6;
				LCD_Sub_Point = 1;
				return;
			}							
			clrLCDdpy();
		}
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
		{
			LCD_Mode_PB = 1; 				   	
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
			LCD_UP_PB = 1;  // incriment
			LCD_Sub_Point++;
			if (LCD_Sub_Point > 1)
				LCD_Sub_Point = 0;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
			LCD_DN_PB = 1;  // decriment
			LCD_Sub_Point--;
			if (LCD_Sub_Point < 0)
				LCD_Sub_Point = 1;
		}

		if (LCD_Flash != 0)
		{
			for(i=0;i<=19;i++)
			{
				LCD_Display[1][i] = Security_dpy[NO_YES_Dpy_Offset + LCD_Sub_Point][i];
			}
		}
		else
		{
			for(i=10;i<=15;i++)
			{
				LCD_Display[1][i] = ' ';
			}
		}
	}
	if(Menu_level == 2)
	{				// Select floor to work with
		for(i=0;i<=19;i++)
		{
			LCD_Display[0][i] = Security_dpy[MainMenu_Dpy_Offset][i];
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{			   
			LCD_Enter_PB = 1;
			Security_Dpy_Index = Function_Dpy_Offset;
			LCD_Sub_Point = 0;
			Menu_level = 3;
			clrLCDdpy();
			return;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
		{
			LCD_Mode_PB = 1;
			Menu_level = 0;
			LCD_Sub_Point = MainMenu_Dpy_Offset;
			return;
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
			LCD_UP_PB = 1;  // incriment
			LCD_FloorNum++;
			if (LCD_FloorNum > cons[grtopf])
				LCD_FloorNum = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
			LCD_DN_PB = 1;  // decriment
			LCD_FloorNum--;
			if(LCD_FloorNum < 1)
				LCD_FloorNum = cons[grtopf];
		}

		for(i=0;i<=19;i++)
		{
			LCD_Display[1][i] = Security_dpy[Floor_Dpy_Offset][i];
		}
		LCD_Display[1][16] = flmrk[LCD_FloorNum][0];
		LCD_Display[1][17] = flmrk[LCD_FloorNum][1];
	}
	if(Menu_level == 3)
	{				  // Select code modification operation
		for(i=0;i<=19;i++)
		{
			LCD_Display[0][i] = Security_dpy[Floor_Dpy_Offset][i];
		}
		LCD_Display[0][16] = flmrk[LCD_FloorNum][0];
		LCD_Display[0][17] = flmrk[LCD_FloorNum][1];

		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{			   
			LCD_Enter_PB = 1;					
			Security_Dpy_Index = LCD_Sub_Point + Function_Dpy_Offset;

			if((Security_Dpy_Index == 3) || (Security_Dpy_Index == 4))	 // Add or Delete Code
			{
				//initialize the  code digits 
				Code_Num[0] = 1;
				Code_Num[1] = 1;
				Code_Num[2] = 1;
				Code_Num[3] = 1;
				Menu_level = 4;
			}
			else if(Security_Dpy_Index == 5)	 // Clear All Codes
			{
				Menu_level = 1;					  
			}
			else if(Security_Dpy_Index == 2)	 // View Codes
				Menu_level = 5;		 					
			
			Read_CodeTbl(LCD_FloorNum, &CodeTable, LookUpTable);
			LCD_Sub_Point = 0;
			LCD_Dig_Point = 1;
			clrLCDdpy();
			return;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
		{
			LCD_Mode_PB = 1;
			Menu_level = 2;
			LCD_Sub_Point = 0;
			Security_Dpy_Index = MainMenu_Dpy_Offset; 				 // Back to Modify Floor Security Display
			return;
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
			LCD_UP_PB = 1;  // incriment
			if (Dispatcher == cons[carnmb])
			{
				LCD_Sub_Point++;
				if(LCD_Sub_Point > 3)
					LCD_Sub_Point = 0;
			}
			else
				LCD_Sub_Point = 0;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
			LCD_DN_PB = 1;  // decriment
			if (Dispatcher == cons[carnmb])
			{
				LCD_Sub_Point--;
				if(LCD_Sub_Point < 0)
					LCD_Sub_Point = 3;
			}
			else
				LCD_Sub_Point = 0;
		} 

		for(i=0;i<=19;i++)
		{
			LCD_Display[1][i] = Security_dpy[LCD_Sub_Point+Function_Dpy_Offset][i];
		}
	}
	if(Menu_level == 4)						            // Add && delete Code
	{
		for(i=0;i<=19;i++)
		{
			LCD_Display[0][i] = Security_dpy[Security_Dpy_Index][i];
		}

		if ((Security_Dpy_Index == 3) && (CodeTable.NumCode >= NumMaxCd)) 
		{		// Display code table full
			if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
			{			   
				LCD_Enter_PB = 1;
				Menu_level = 3;
				Security_Dpy_Index = Floor_Dpy_Offset;
				LCD_Sub_Point = 1;
				return;
			}
			if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
			{
				LCD_Mode_PB = 1;	     
				Menu_level = 3;
				Security_Dpy_Index = Floor_Dpy_Offset;
				LCD_Sub_Point = 1;
				return;	
			}
			for(i=0;i<=19;i++)
			{
				LCD_Display[1][i] = Security_dpy[13][i];
			}
		}
		else
		{			
			if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
			{			   
				LCD_Enter_PB = 1;
				Menu_level = 1;	
			}
			if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
			{
				LCD_Mode_PB = 1;	     
				LCD_Dig_Point++;
				if (LCD_Dig_Point > 5)
					LCD_Dig_Point = 1;
			}
			if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
			{
				LCD_UP_PB = 1;  // incriment
				if(LCD_Dig_Point == 1)
				{
					Code_Num[0]++;
					if (Code_Num[0] > (unsigned char)cons[grtopf])
						Code_Num[0] = 1;
				}
				else if(LCD_Dig_Point == 2)
				{
					Code_Num[1]++;
					if (Code_Num[1] > (unsigned char)cons[grtopf])
						Code_Num[1] = 1;

				}
				else if(LCD_Dig_Point == 3)
				{
					Code_Num[2]++;
					if (Code_Num[2] > (unsigned char)cons[grtopf])
						Code_Num[2] = 1;
				}
				else if(LCD_Dig_Point == 4)
				{
					Code_Num[3]++;
					if (Code_Num[3] > (unsigned char)cons[grtopf])
						Code_Num[3] = 1;					
				}
				else if (LCD_Dig_Point == 5)
				{
					if (code_floor_access == 1)
						code_floor_access = 0;
					else
						code_floor_access = 1;
				}
			}
			if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
			{
				LCD_DN_PB = 1;  // decriment
				if(LCD_Dig_Point == 1)
				{
					Code_Num[0]--;
					if(Code_Num[0] < 1)
						Code_Num[0] = (uint8)cons[grtopf];
				}
				else if(LCD_Dig_Point == 2)
				{
					Code_Num[1]--;
					if(Code_Num[1] < 1)
						Code_Num[1] = (uint8)cons[grtopf];

				}
				else if(LCD_Dig_Point == 3)
				{
					Code_Num[2]--;
					if(Code_Num[2] < 1)
						Code_Num[2] = (uint8)cons[grtopf];
				}
				else if(LCD_Dig_Point == 4)
				{
					Code_Num[3]--;
					if(Code_Num[3] < 1)
						Code_Num[3] = (uint8)cons[grtopf];				
				}
				else if (LCD_Dig_Point == 5)
				{
					if (code_floor_access == 1)
						code_floor_access = 0;
					else
						code_floor_access = 1;
				}
			}
			LCD_Display[1][4] = flmrk[Code_Num[0]][0];
			LCD_Display[1][5] = flmrk[Code_Num[0]][1];
			LCD_Display[1][8] = flmrk[Code_Num[1]][0];
			LCD_Display[1][9] = flmrk[Code_Num[1]][1];
			LCD_Display[1][12] = flmrk[Code_Num[2]][0];
			LCD_Display[1][13] = flmrk[Code_Num[2]][1];
			LCD_Display[1][16] = flmrk[Code_Num[3]][0];
			LCD_Display[1][17] = flmrk[Code_Num[3]][1];

			if (code_floor_access == 1)
				LCD_Display[1][21] = 'R';
			else
				LCD_Display[1][21] = 'F';

			if (LCD_Flash != 0)
			{
				if(LCD_Dig_Point == 1)
				{
					LCD_Display[1][4] = ' ';
					LCD_Display[1][5] = ' ';
				}
				else if(LCD_Dig_Point == 2)
				{
					LCD_Display[1][8] = ' ';
					LCD_Display[1][9] = ' ';
				}
				else if(LCD_Dig_Point == 3)
				{
					LCD_Display[1][12] = ' ';
					LCD_Display[1][13] = ' ';
				}
				else if(LCD_Dig_Point == 4)
				{
					LCD_Display[1][16] = ' ';
					LCD_Display[1][17] = ' ';
				}
				else if (LCD_Dig_Point == 5)
				{
					LCD_Display[1][21] = ' ';
				}
			} 
		}
	}
	if(Menu_level == 5)									// View code 
	{
		for(i=0;i<=19;i++)
		{
			LCD_Display[0][i] = Security_dpy[Security_Dpy_Index][i];
		}

		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{			   
			LCD_Enter_PB = 1;
			Menu_level = 3;
			LCD_Sub_Point = 0;
			Security_Dpy_Index = 2;
			return;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
		{
			LCD_Mode_PB = 1;
			Menu_level = 3;
			LCD_Sub_Point = 0;
			Security_Dpy_Index = 2;
			return;
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
			LCD_UP_PB = 1;  // incriment
			LCD_Sub_Point++;
			if(LCD_Sub_Point > CodeTable.NumCode - 1)
				LCD_Sub_Point = 0;	 
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
			LCD_DN_PB = 1;  // decriment
			LCD_Sub_Point--;
			if(LCD_Sub_Point < 0)
			{
				if (CodeTable.NumCode > 0)
					LCD_Sub_Point = CodeTable.NumCode - 1;
				else
					LCD_Sub_Point = 0;
			}
				
		}

		if (CodeTable.Codes[LCD_Sub_Point][0] == 0)
		{
			for(i=0;i<=19;i++)
			{
				LCD_Display[1][i] = Security_dpy[Code_Tbl_Empty_Offset][i];
			}
		}
		else
		{
			LCD_Display[1][4] = flmrk[CodeTable.Codes[LCD_Sub_Point][0] & 0x7F][0];
			LCD_Display[1][5] = flmrk[CodeTable.Codes[LCD_Sub_Point][0] & 0x7F][1];
			LCD_Display[1][8] = flmrk[CodeTable.Codes[LCD_Sub_Point][1]][0];
			LCD_Display[1][9] = flmrk[CodeTable.Codes[LCD_Sub_Point][1]][1];
			LCD_Display[1][12] = flmrk[CodeTable.Codes[LCD_Sub_Point][2]][0];
			LCD_Display[1][13] = flmrk[CodeTable.Codes[LCD_Sub_Point][2]][1];
			LCD_Display[1][16] = flmrk[CodeTable.Codes[LCD_Sub_Point][3]][0];
			LCD_Display[1][17] = flmrk[CodeTable.Codes[LCD_Sub_Point][3]][1];
			if ((CodeTable.Codes[LCD_Sub_Point][0] & 0x80) != 0)
				LCD_Display[1][21] = 'R';
			else
				LCD_Display[1][21] = 'F';
		}
	}
	if (Menu_level == 6)
	{		// Add or delete single code or delete all codes for all floors
		for(i=0;i<=19;i++)
		{
			LCD_Display[0][i] = Security_dpy[1][i];
		}

		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{			   
			LCD_Enter_PB = 1;					

			if((Security_Dpy_Index == 10)||(Security_Dpy_Index == 11))	 // Add or Delete Code
			{
				//initialize the  code digits 
				Code_Num[0] = 1;
				Code_Num[1] = 1;
				Code_Num[2] = 1;
				Code_Num[3] = 1;
				Menu_level = 4;
			}
			else if(Security_Dpy_Index == 12)	 // Clear All Codes
			{
				Menu_level = 1;					  
			}
			LCD_Sub_Point = 0;
			LCD_Dig_Point = 1;
			clrLCDdpy();
			return;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
		{
			LCD_Mode_PB = 1;
			Menu_level = 0;
			LCD_Sub_Point = 1;
			Security_Dpy_Index = 1; 				 // Back to Sec Codes all floors Display
			return;
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
			LCD_UP_PB = 1;  // incriment
			if (Dispatcher == cons[carnmb])
			{
				Security_Dpy_Index++;
				if(Security_Dpy_Index > 12)
					Security_Dpy_Index = 10;
			}
			else
				Security_Dpy_Index = 0;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
			LCD_DN_PB = 1;  // decriment
			if (Dispatcher == cons[carnmb])
			{
				Security_Dpy_Index--;
				if(Security_Dpy_Index < 10)
					Security_Dpy_Index = 12;
			}
			else
				Security_Dpy_Index = 0;
		} 

		for(i=0;i<=19;i++)
		{
			LCD_Display[1][i] = Security_dpy[Security_Dpy_Index][i];
		}
	}
	*/
	return(lcd_menu);
}

int16 adjust_lcd_display (int16 lcd_menu)
{
	int16 i;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	int16 contrast_point;
	static int16 new_contrast;
	static int16 new_brightness;

	if (LCD_Init == 1)
	{
		LCD_Init  = 0;
		menu_ptr = 0;
		menu_level = 0;
		contrast_point = 0;
  	    upd_dpy = 1;
	}

	if (menu_level == 0)
	{			// Select time menu

		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
				menu_ptr = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
	    	if(menu_ptr > 1)
				menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	menu_level = menu_ptr + 1;
		  	LCD_Sub_Init = 1;
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = xLCD_Main_Menu[12][i];
			  	LCD_Display[1][i] = LCD_Dpy_Adjust_Menu[0][i];
			  	LCD_Display[2][i] = LCD_Dpy_Adjust_Menu[1][i];
			  	LCD_Display[3][i] = ' ';
		    }
			LCD_Display[menu_ptr+1][0] = '>';		// show cursor
		}
	}
	else if (menu_level == 1)
	{
		if (LCD_Sub_Init == 1)
		{
			LCD_Sub_Init = 0;
			new_contrast = get_contrast();
			if (new_contrast < 25)
				new_contrast = 25;
		  	upd_dpy = 1;
		}
		
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			menu_level = 0;
		  	upd_dpy = 1;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    new_contrast++;
	    	if(new_contrast > 50)
				new_contrast = 50;
	    	set_contrast(new_contrast);
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    new_contrast--;
		    if(new_contrast < 25)
				new_contrast = 25;
	    	set_contrast(new_contrast);
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
		    {
			  	LCD_Display[0][i] = getcaps(LCD_Dpy_Adjust_Menu[0][i]);
			  	LCD_Display[1][i] = ' ';
			  	LCD_Display[2][i] = LCD_Dpy_Adjust_Menu[2][i];
			  	LCD_Display[3][i] = ' ';
		    }
		    contrast_point = (new_contrast - 25)/2;
			LCD_Display[2][contrast_point+2] = '|';		// show cursor
		  	itoa(new_contrast, LCD_String);  // position
			if(new_contrast < 10)                // of the
			  	LCD_Display[2][18] = LCD_String[0];         // fault
			else
			{
			  	LCD_Display[2][17] = LCD_String[0];
			  	LCD_Display[2][18] = LCD_String[1];
			}
		}
	}
	else if (menu_level == 2)
	{
		if (LCD_Sub_Init == 1)
		{
			LCD_Sub_Init = 0;
			new_brightness = get_brightness();
		  	upd_dpy = 1;
		}
		
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			menu_level = 0;
		  	upd_dpy = 1;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    new_brightness++;
	    	if(new_brightness > 8)
				new_brightness = 8;
	    	set_brightness(new_brightness);
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    new_brightness--;
		    if(new_brightness < 1)
				new_brightness = 1;
	    	set_brightness(new_brightness);
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
		    {
			  	LCD_Display[0][i] = getcaps(LCD_Dpy_Adjust_Menu[1][i]);
			  	LCD_Display[1][i] = ' ';
			  	LCD_Display[2][i] = LCD_Dpy_Adjust_Menu[3][i];
			  	LCD_Display[3][i] = ' ';
		    }
			LCD_Display[2][new_brightness+1] = '|';		// show cursor
		  	itoa(new_brightness, LCD_String);  // position
			if(new_brightness < 10)                // of the
			  	LCD_Display[2][13] = LCD_String[0];         // fault
			else
			{
			  	LCD_Display[2][12] = LCD_String[0];
			  	LCD_Display[2][13] = LCD_String[1];
			}
		}
	}
	return(lcd_menu);
}

int16 display_immediate_fault(int16 lcd_menu)
{
	lcd_menu = 0;
	return (lcd_menu);
/*
	if (LCD_Pointer == 24)
	{
		upd_dpy = 1;
		LCD_Pointer++;
		FaultInterrupt = true;			
	}

   	if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
  	{
  	    LCD_Enter_PB = 1;
		FaultInterrupt = false;			
  	}
  	if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
  	{
  	    LCD_Mode_PB = 1;  
		FaultInterrupt = false;			
  	}
  	if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
  	{
  	    LCD_UP_PB = 1;  // incriment
		FaultInterrupt = false;			
  	}
  	if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
  	{
  	    LCD_DN_PB = 1;  // decriment
		FaultInterrupt = false;			
	}
  	
  	if(upd_dpy == 1)
  	{
     	upd_dpy = 0;
		fault_display(25,Flt_ix,0);		// 25 used for special to not show index #
  	}
	if (FaultInterrupt == false)
		ReturnLCDMenu();
	*/
	return(lcd_menu);
}

int16 software_utilities (int16 lcd_menu)
{
	int16 i,j;
  	int16 program_status = 0;
	int16 start_ptr;
	int16 line_ptr;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 menu_ptr_ret;
	static int16 mode_sel;
	static int16 init_state;
	static union {
			uint8 b[2];			// byte: char 0 - 1
			uint16 w;			// word: short int
		}uwu;					// unsigned word union

	
	if (LCD_Init == 1)
	{
		LCD_Init = 0; 
		upd_dpy = 1;
		menu_level = 0;
		menu_ptr = 0;
  	}

	if (menu_level == 0)
	{			// Rotate through Call menu items
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{	   // Exit to main menu
			ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    menu_ptr--;
		    if(menu_ptr < 0)
			  menu_ptr = Last_SW_Menu;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    menu_ptr++;
		    if(menu_ptr > Last_SW_Menu)
			  menu_ptr = 0;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		  	upd_dpy = 1;
		  	menu_level = (int16)(SW_Menu_list[menu_ptr] + 1);
  		    mode_sel = 0;
  			menu_ptr_ret = menu_ptr;
  		    menu_ptr = 0;
  		    LCD_Sub_Init = 1;
		  	return(lcd_menu);
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
	  		line_ptr = menu_ptr % 3;
	  		start_ptr = menu_ptr - line_ptr;
	  	    for(i=0; i<=19; i++)
	  	    {
	  			LCD_Display[0][i] = xLCD_Main_Menu[13][i];
				LCD_Display[1][i] = LCD_Software_Menu[SW_Menu_list[start_ptr]][i];
				if ((start_ptr + 1) > Last_SW_Menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else
			  	{
					LCD_Display[2][i] = LCD_Software_Menu[SW_Menu_list[start_ptr+1]][i];
					if ((start_ptr + 2) > Last_SW_Menu)
				  		LCD_Display[3][i] = ' ';
					else
						LCD_Display[3][i] = LCD_Software_Menu[SW_Menu_list[start_ptr+2]][i];
			  	}
	  	    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}
	if (menu_level == 1)
	{			// Display software Version
  		if(upd_dpy == 1)	
  		{
	    	upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
				LCD_Display[0][i] = LCD_Version[0][i];
				LCD_Display[1][i] = LCD_Version[1][i];
				LCD_Display[2][i] = LCD_Version[2][i];
				LCD_Display[3][i] = LCD_Version[3][i];
	    	}
			
			LCD_Display[0][5] = car_bld_no[cons[carnmb]][0];
			LCD_Display[0][6] = car_bld_no[cons[carnmb]][1];

#if (Tract_HR == 1)
			LCD_Display[0][18] = 'T';
			LCD_Display[0][19] = 'R';
#else
			LCD_Display[0][18] = 'H';
			LCD_Display[0][19] = 'Y';
#endif

			LCD_Display[1][16] = (char)(cons[carnmb] + '0');

			if ((version/10) == 0)
				LCD_Display[3][6] = ' ';
			else
				LCD_Display[3][6] = (char)((version/10) + '0');
			LCD_Display[3][7] = (char)((version%10) + '0');
			LCD_Display[3][8] = '.';
			LCD_Display[3][9] = (char)((revision/10) + '0');
			LCD_Display[3][10] = (char)((revision%10) + '0');

			LCD_Display[3][11] = '.';
			LCD_Display[3][12] = (char)((interim_revision/10) + '0');
			LCD_Display[3][13] = (char)((interim_revision%10) + '0');

  #if (Simulator == 1)
			LCD_Display[3][17] = 'S';
			LCD_Display[3][18] = 'i';
			LCD_Display[3][19] = 'm';
#endif
			
		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
      		ModeButton = 1;  //
			menu_level = 0;
			menu_ptr = menu_ptr_ret;
 			upd_dpy = 1;
 			return(lcd_menu);
 		}
	}
	if (menu_level == 2)
	{      	  // Jump to Power Up Routine
		if (PasswordFlag == false)
	   	{
	  		StoreLCDInfo();
			clrLCDdpy();
	  		LCD_Menu = 25;
	  		LCD_Init = 1;
 			return(lcd_menu);
   	   	}
  		if(upd_dpy == 1)	
  		{
  		    upd_dpy = 0;
  		    for(i=0; i<=19; i++)
  		    {
  		    	LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Jump_info[0][i];
				LCD_Display[2][i] = LCD_Jump_info[1][i];
  		    	LCD_Display[3][i] = ' ';
  		    }
  		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
#if (Simulator == 0)
	    	if(chk_ins() == 1)
			{ 			// Car must be on inspection
#endif
				clrall();
  // Get Ready to Jump
                JumpToPowerUp(lcd_menu);
/*                
		 	    addr_ptr = 0x00000000;
		 	    address_code = (*(uint32*)addr_ptr);
				addr_ptr = 0x00000004;
				start_address = (*(uint32*)addr_ptr);
				program_status = 0;
					
				
				if (address_code == 0x015A015A)
				{
					if (start_address != 0)
					{
						if (get_MRAM_code() == 0x4D7C)
						{
				  		    for(i=0; i<=19; i++)
				  		    {
				  		    	LCD_Display[0][i] = ' ';
								LCD_Display[1][i] = ' ';
								LCD_Display[2][i] = ' ';
				  		    	LCD_Display[3][i] = ' ';
				  		    }
							timers[tsec] = 0;	
							delay(20000);						
							write_MRAM_code(0x5E8D);		// Write code to stay in powerup routine
							SIU.SRCR.B.SSR = 1;				// Software Reset
 							return(lcd_menu);
						}
						else
							program_status = 1;
					}
					else 
						program_status = 2;
				}
				else 
					program_status = 3;
				
	  		    for(i=0; i<=19; i++)
	  		    {
			  		LCD_Display[1][i] = LCD_Jump_info[4][i];
			  		LCD_Display[2][i] = LCD_Jump_info[program_status+4][i];
	  		    }
*/
	  		    
#if (Simulator == 0)
	 		} // END OF 'Get Ready to Jump'
			else
			{
	  		    for(i=0; i<=19; i++)
	  		    {
			  		LCD_Display[1][i] = LCD_Jump_info[2][i];
			  		LCD_Display[2][i] = LCD_Jump_info[3][i];
	  		    }
			}
#endif
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
			menu_level = 0;
			menu_ptr = menu_ptr_ret;
 			upd_dpy = 1;
 			return(lcd_menu);
  		}
	} // END OF "Jump to Power Up Routine" (wbg)
	if (menu_level == 3)
	{      	  // Watchdog test
		if (PasswordFlag == false)
	   	{
	  		StoreLCDInfo();
			clrLCDdpy();
	  		LCD_Menu = 25;
	  		LCD_Init = 1;
 			return(lcd_menu);
   	   	}
  		if(upd_dpy == 1)	
  		{
  		    upd_dpy = 0;
  		    for(i=0; i<=19; i++)
  		    {
  		    	LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Watchdog_info[0][i];
				LCD_Display[2][i] = LCD_Watchdog_info[1][i];
  		    	LCD_Display[3][i] = ' ';
  		    }
  		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
			if (EnterButton == 0)
			{
				
	  		    for(i=0; i<=19; i++)
	  		    {
			  		LCD_Display[0][i] = ' ';
			  		LCD_Display[1][i] = ' ';
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
	  		    }
			}
  		    EnterButton = 1;
	    	if(chk_ins() == 1)
			{ 			// Car must be on inspection
				while(1)
					;
	 		}
			else
			{
	  		    for(i=0; i<=19; i++)
	  		    {
			  		LCD_Display[1][i] = LCD_Watchdog_info[2][i];
			  		LCD_Display[2][i] = LCD_Watchdog_info[3][i];
	  		    }
			}
		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  //
			menu_level = 0;
 			upd_dpy = 1;
			menu_ptr = menu_ptr_ret;
 			return(lcd_menu);
 		}
	}
	if (menu_level == 4)
	{			// Reset Debug Variables
  		if(upd_dpy == 1)	
  		{
	    	upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
  		    	LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Debug_Reset[0][i];
				LCD_Display[2][i] = LCD_Debug_Reset[1][i];
  		    	LCD_Display[3][i] = ' ';
	    	}
			debug1 = 0;
			debug2 = 0;
			debug3 = 0;
			debug4 = 0;
			debug5 = 0;
			debug6 = 0;
			debug7 = 0;
			debug8 = 0;
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
      		ModeButton = 1;  
			menu_level = 0;
 			upd_dpy = 1;
 			menu_ptr = menu_ptr_ret;
			return(lcd_menu);
  		}
	}
	
	if (menu_level == 5)
	{      	  // Store Field Variables
		if (PasswordFlag == false)
	   	{
	  		StoreLCDInfo();
			clrLCDdpy();
	  		LCD_Menu = 25;
	  		LCD_Pointer = 25;
 			return(lcd_menu);
   	   	}

  		if (((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0)) || (mode_sel == 4))
  		{
	  	  	ModeButton = 1;  
  			if (mode_sel != 0)
  			{
  				mode_sel = 0;
  				upd_dpy = 1;
  			}
  			else
  			{
				menu_level = 0;
	 			upd_dpy = 1;
				menu_ptr = menu_ptr_ret;
	 			return(lcd_menu);
  			}
  		}
/*    	if(chk_ins() == 0)  // Car not on inspection
    	{
  		    for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = LCD_SD_Read_Write_info[0][i];
				LCD_Display[1][i] = LCD_SD_Read_Write_info[1][i];
				LCD_Display[2][i] = LCD_SD_Read_Write_info[2][i];
				LCD_Display[3][i] = LCD_SD_Read_Write_info[3][i];
			}
 			return(lcd_menu);
    	}
    	else
*/    	{
			if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
			{
			    UpButton = 1;  // incriment
			    if (mode_sel < 2)
			    {
				    upd_dpy = 1;
				    menu_ptr--;
				    if(menu_ptr < 0)
					  menu_ptr = Last_SD_Menu;
				    
		  		    mode_sel = 0;
			    }
			    else if (mode_sel == 2)
			    	mode_sel = 3;
			    else
			    	mode_sel = 2;
			}

			if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
			{
			    DownButton = 1;  // decriment
			    if (mode_sel < 2)
			    {
				    upd_dpy = 1;
				    menu_ptr++;
				    if(menu_ptr > Last_SD_Menu)
					  menu_ptr = 0;
				    
		  		    mode_sel = 0;
			    }
			    else if (mode_sel == 2)
			    	mode_sel = 3;
			    else
			    	mode_sel = 2;
			}
	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	  		    EnterButton = 1;
				upd_dpy = 0;
				if (mode_sel <= 2)
				{
					
					j = fvarstoSD(menu_ptr,(mode_sel == 2));
					if (j == 10)
						mode_sel = 2;
					else	
						mode_sel = 1;	
					for(i=0; i<=19; i++)
				  	{
  		    			LCD_Display[0][i] = getcaps(LCD_Software_Menu[4][i]);
					  	LCD_Display[1][i] = LCD_SD_Read_Write[menu_ptr][i];
					    LCD_Display[2][i] = LCD_SD_Read_Write_info[j+4][i];
			  			LCD_Display[3][i] = ' ';
					}
				}
				else
				{
					upd_dpy = 1;
					mode_sel = 4;
				}
	 		}
  		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
	  		line_ptr = menu_ptr % 3;
	  		start_ptr = menu_ptr - line_ptr;
	  	    for(i=0; i<=19; i++)
	  	    {
  		    	LCD_Display[0][i] = getcaps(LCD_Software_Menu[4][i]);
				LCD_Display[1][i] = LCD_SD_Read_Write[start_ptr][i];
				if ((start_ptr + 1) > Last_SD_Menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else
			  	{
					LCD_Display[2][i] = LCD_SD_Read_Write[start_ptr+1][i];
					if ((start_ptr + 2) > Last_SD_Menu)
				  		LCD_Display[3][i] = ' ';
					else
						LCD_Display[3][i] = LCD_SD_Read_Write[start_ptr+2][i];
			  	}
	  	    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
		else if (mode_sel == 2)
			LCD_Display[2][17] = 'Y';
		else if (mode_sel == 3)
			LCD_Display[2][17] = 'N';

	}
  	
	if (menu_level == 6)
	{      	  // SD Card Status
		if (PasswordFlag == false)
	   	{
	  		StoreLCDInfo();
			clrLCDdpy();
	  		LCD_Menu = 25;
	  		LCD_Pointer = 25;
 			return(lcd_menu);
   	   	}

  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
	  		ModeButton = 1;  
  			sd_error = 0;
  			if (mode_sel == 1)
  			{
  				mode_sel = 0;
  				upd_dpy = 1;
  			}
  			else
  			{
				menu_level = 0;
	 			upd_dpy = 1;
				menu_ptr = menu_ptr_ret;
	 			return(lcd_menu);
  			}
  		}
  			
   	{

	  		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  		{
	  		    EnterButton = 1;
  				mode_sel = 1;
				sd_error = Load_Store_Program(0);
				if (sd_error == 0)
					sd_init = 1;
				else
					sd_init = 0;
				if (sd_error == 0)
				{
		  		    for(i=0; i<=19; i++)
		  		    {
				  		LCD_Display[1][i] = getcaps(LCD_Software_Menu[5][i]);
				  		LCD_Display[2][i] = LCD_SD_Card_Status[1][i];
		  		    }
			  	    if (sd_init == 1)
			  	    	LCD_Display[2][5] = '1';
			  	    if (sdhc == 1)
			  	    	LCD_Display[2][10] = '1';
			  	    if (sdvs == 1)
			  	    	LCD_Display[2][18] = '1';
				}
				else
	  			{
				    for(i=0; i<=19; i++)
				    {
						LCD_Display[1][i] = LCD_SD_Error[0][i];
					  	LCD_Display[2][i] = LCD_SD_Error[sd_error][i];
				    }
	  			}
	  		}
	  		if (upd_dpy == 1)
	  		{
	  			upd_dpy = 0;
	  		    for(i=0; i<=19; i++)
	  		    {
 			    	LCD_Display[0][i] = ' ';
					LCD_Display[1][i] = getcaps(LCD_Software_Menu[5][i]);
					LCD_Display[2][i] = LCD_SD_Card_Status[0][i];
   		 		   	LCD_Display[3][i] = ' ';
	  		    }
	  		}
	    }
	    
	}
	
	if (menu_level == 7)
	{			// Set View WiFi Settings
		menu_level = wifi_menu(menu_level);
		if (menu_level == 0)
		{
			menu_level = 0;
			menu_ptr = menu_ptr_ret;
 			upd_dpy = 1;
		}
	}
/*			
	if (menu_level == 8)
	{			// Preset Limit Velocitiy Variables
		if (PasswordFlag == false)
	   	{
  		    LCD_Pointer = 21;
			Menu_level = 0;
	  		StoreLCDInfo();
			clrLCDdpy();
	  		LCD_Menu = 25;
	  		LCD_Pointer = 25;
 			return;
   	   	}
  		if(LCD_Sub_Init == 7)	
  		{
	    	LCD_Sub_Init = 0;
	    	for(i=0; i<=19; i++)
	    	{
 			    LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Init_Vars[0][i];
				LCD_Display[2][i] = LCD_Init_Vars[1][i];
 			    LCD_Display[3][i] = ' ';
	    	}
  		}
  		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
  		{
  		    LCD_Enter_PB = 1;
			LCD_Sub_Point = 1;
			init_limit_values();		// set velocity values to top speed once
		  	wrfvar();
			SPB_eep_ix = 0;		   	
			SPB_eep_nmb = (int16)(numspbvars);
			SPB_can.eep_req = 2;							// send data to spb processor
			wr_spb_fvar();				// data already sent to spb but needs to be written in eeprom
			
			NTS_eep_ix = 0;	 			
			NTS_eep_nmb = (int16)(numntsvars);
			NTS_can.eep_req = 2;			 				// send data to nts processor
			wr_nts_fvar();				// data already sent to nts but needs to be written in eeprom
			wrfvar();
	    	for(i=0; i<=19; i++)
	    	{
 		    	LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Init_Vars[2][i];
				LCD_Display[2][i] = LCD_Init_Vars[3][i];
 		    	LCD_Display[3][i] = ' ';
	    	}
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
  		{
      		LCD_Mode_PB = 1;  
			menu_level = 0;
 			upd_dpy = 1;
 			return(lcd_menu);
  		}
	}
*/	      	
	if (menu_level == 9)
	{			// Set Trace Condition
		menu_level = trace_trigger_menu(menu_level);
		if (menu_level == 0)
		{
			menu_level = 0;
 			upd_dpy = 1;
			menu_ptr = menu_ptr_ret;
		}
	}
	return(lcd_menu);
}

int16 password (int16 lcd_menu)
{
	int16 i;
	
	static int16 upd_dpy;
	static int16 mode_sel;

	if(LCD_Init == 1)
	{
		LCD_Init = 0;
		upd_dpy = 1;
  	    LCD_Dig_Point = 0;
		mode_sel = 0;
		Password = 0;
    	for(i=0; i<=19; i++)
  			LCD_Display[0][i] = Password_dpy[2][i];
        sprintf(&LCD_Display[1][9],"%04i",Password);
	}

	if(mode_sel == 0)
	{
   		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  		{
  		    EnterButton = 1;
  		    upd_dpy = 1;
			 if ((Password == fvars[fvpassword]) || (Password == 1927))		  // Leave the password display screen
			 {
			     PasswordFlag = true;
				 timers[tpassword] = 0;
				 ReturnLCDMenu();
			 }
			 else
		        mode_sel = 1;
			
			return (lcd_menu);
  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  		{
  		    ModeButton = 1;  
			upd_dpy = 1;
			LCD_Dig_Point = set_adj_digit(9999,LCD_Dig_Point);			
  		}
  		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  		{
  		    UpButton = 1;  // incriment
  		    upd_dpy = 1;
  		   	// increment fieldvar
			adjust_variable(&Password,9999,0,1,LCD_Dig_Point);
  		}
  		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  		{
  		    DownButton = 1;  // decriment
  		    upd_dpy = 1;
  		   	// decrement fieldvar
		    adjust_variable(&Password,9999,0,0,LCD_Dig_Point);
  		}
  		 
	}

	if(mode_sel == 1)					  //Wrong Password Display
	{
    	for(i=0; i<=19; i++)
		{
	  		LCD_Display[0][i] = Password_dpy[0][i];
			LCD_Display[1][i] =	Password_dpy[1][i];
        }
   		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0)) //Go back to password screen
  		{
  		    EnterButton = 1;					
			mode_sel = 0;
			clrLCDdpy();
			return (lcd_menu);

  		}
  		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0)) // leave password screen
  		{
  		    ModeButton = 1;
  		    ReturnLCDMenu();	
			return (lcd_menu);
  		}
	}
	if(upd_dpy == 1)
	{
		upd_dpy = 0;
   		LCD_Dig_Loc = 12-LCD_Dig_Point;
		sprintf(&LCD_Display[1][9],"%04i",Password);
		LCD_Flash_TMP[0] = LCD_Display[1][LCD_Dig_Loc];
	}

	flash_digit(LCD_Flash_TMP[0], 1,LCD_Dig_Loc,(mode_sel == 0));

	return(lcd_menu);
}



// ************************************************
//  Display hoistway tables
// ************************************************

int16 display_hoistway(int16 lcd_menu)
{
	int16 i;
	float fl_dist;
	static int16 var_access;
	static int16 var_sel;
	int16 digit_start = 0;
	static int16 upd_dpy;
	static int16 fl;
	
	if(LCD_Sub_Init == 1)	// first time through Display hoistway tables
	{
		LCD_Sub_Init = 0;
		fl = cons[bottomf];
		upd_dpy = 1;
		var_access = 0;
		var_sel = 0;
		LCD_Dig_Point = 0;
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		if (var_access == 0)
		{
			fl++;
			if (fl > cons[topf])
				fl = cons[bottomf];
		}
		else if (var_access == 1)
		{
			var_sel--;
			if (var_sel < 0)
				var_sel = 2;
		}
		else if (var_access == 2)
		{
			
			if (var_sel == 0)
			  	adjust_variable(&DPP_Fl_Dn_SD[fl],0x7fff,0,1,LCD_Dig_Point);
			else if (var_sel == 1)
			  	adjust_variable_32(&DPP_Floor_Pos[fl],0x7fffffff,0,1,LCD_Dig_Point);
			else if (var_sel == 2)
			  	adjust_variable(&DPP_Fl_Up_SD[fl],0x7fff,0,1,LCD_Dig_Point);
		}
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		if (var_access == 0)
		{
			fl--;
			if (fl < cons[bottomf])
				fl = cons[topf];
	    }
	    else if (var_access == 1)
	    {
			var_sel++;
			if (var_sel > 2)
				var_sel = 0;
	    }
		else if (var_access == 2)
		{
			if (var_sel == 0)
			  	adjust_variable(&DPP_Fl_Dn_SD[fl],0x7fff,0,0,LCD_Dig_Point);
			else if (var_sel == 1)
			  	adjust_variable_32(&DPP_Floor_Pos[fl],0x7fffffff,0,0,LCD_Dig_Point);
			else if (var_sel == 2)
			  	adjust_variable(&DPP_Fl_Up_SD[fl],0x7fff,0,0,LCD_Dig_Point);
		}
	}

	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    upd_dpy = 1;
		if (var_access == 0) 
		{						// 0 = no access, 1 = modify var
			if(PasswordFlag == false)
		   	{
		  		StoreLCDInfo();
				clrLCDdpy();
		  		LCD_Menu = 25;
		  		LCD_Init = 1;
				return(lcd_menu);
	   	   	}
			var_access = 1;
		}
		else if (var_access == 1)
		{
			if (((fl != cons[topf]) || (var_sel != 0)) && ((fl != cons[bottomf]) || (var_sel != 2)))
				var_access = 2;
		}
		else if (var_access > 1)	// save field new value entered
	    {
		  	Wrt_Hoistway();
			var_access = 1;
		  	LCD_Dig_Point = 0;
	    }
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  //
		if (var_access == 0)
		{
			lcd_menu = 0;
			return(lcd_menu);
		}
		else if (var_access == 1)
		{
			upd_dpy = 1;
			var_access = 0;
		}
		else if (var_access == 2)
		{
			upd_dpy = 1;
			if (var_sel == 1)
	  			LCD_Dig_Point = set_adj_digit(0x7fffffff,LCD_Dig_Point);
			else 
	  			LCD_Dig_Point = set_adj_digit(0x7fff,LCD_Dig_Point);
		}
	}

	if(upd_dpy == 1)
	{
		upd_dpy = 0;
		fl_dist = 0;
		for(i=0; i<=19; i++)
		{
			LCD_Display[0][i] = LCD_Floor_SD[0][i];
			LCD_Display[1][i] = LCD_Floor_SD[1][i];
			LCD_Display[2][i] = LCD_Floor_SD[2][i];
			LCD_Display[3][i] = LCD_Floor_SD[3][i];
		}
	  
	  	itoa(fl,LCD_String);
		if(fl < 10)               
		  	LCD_Display[0][7] = LCD_String[0];    
		else
		{
		  	LCD_Display[0][6] = LCD_String[0];
		  	LCD_Display[0][7] = LCD_String[1];
		}
	    
	  	// Down Slowdown
	  	
	  	if (fl == cons[topf])
	  	{
			LCD_Display[1][5] = '-';
			LCD_Display[1][6] = '-';
			LCD_Display[1][7] = '-';
	  	}
	  	else
	  	{
			if ((var_access > 1) && (var_sel == 0))
				sprintf(&LCD_Display[1][4],"%05i",DPP_Fl_Dn_SD[fl]);
			else
				sprintf(&LCD_Display[1][4],"%-5i",DPP_Fl_Dn_SD[fl]);
			LCD_Display[1][9] = ' ';
			
		   	fl_dist = (float)( (((float)DPP_Fl_Dn_SD[fl])/(float)Pulses_per_Inch)/12.0);
			sprintf(&LCD_Display[1][15],"%5.1f",fl_dist);
	  	}
	  	
		// floor position
		if ((var_access > 1) && (var_sel == 1))
			sprintf(&LCD_Display[2][4],"%09i",DPP_Floor_Pos[fl]);
		else
			sprintf(&LCD_Display[2][4],"%-9i",DPP_Floor_Pos[fl]);
		
	    LCD_Display[2][1] = flmrk[fl][0];//Put Display
	    LCD_Display[2][2] = flmrk[fl][1];// floor marking
		LCD_Display[2][13] = ' ';
	    
	   	fl_dist = (float)( (((float)(DPP_Floor_Pos[fl]-DPP_Floor_Pos[cons[bottomf]]))/(float)Pulses_per_Inch)/12.0);
		sprintf(&LCD_Display[2][15],"%5.1f",fl_dist);

		// Up slodown
		if (fl == cons[bottomf])
		{
			LCD_Display[3][5] = '-';
			LCD_Display[3][6] = '-';
			LCD_Display[3][7] = '-';
		}
		else
		{
			if ((var_access > 1) && (var_sel == 2))
				sprintf(&LCD_Display[3][4],"%05i",DPP_Fl_Up_SD[fl]);
			else
				sprintf(&LCD_Display[3][4],"%-5i",DPP_Fl_Up_SD[fl]);
			LCD_Display[3][9] = ' ';
			
		   	fl_dist = (float)( (((float)DPP_Fl_Up_SD[fl])/(float)Pulses_per_Inch)/12.0);
			sprintf(&LCD_Display[3][15],"%5.1f",fl_dist);
		}

		if (var_access > 0)
		{
			if (var_sel == 0)
			{
				LCD_Display[1][3] = '>';
				digit_start = 8;
			}
			else if (var_sel == 1)
			{
				LCD_Display[2][3] = '>';
				digit_start = 12;
			}
			else if (var_sel == 2)
			{
				LCD_Display[3][3] = '>';
				digit_start = 8;
			}
			LCD_Dig_Loc = digit_start-LCD_Dig_Point;
	  		LCD_Flash_TMP[0] = LCD_Display[var_sel+1][LCD_Dig_Loc];
		}
	}
	if (var_access == 1)
		flash_digit('>', var_sel+1, 3, 1);
	else if (var_access == 2)
		flash_digit(LCD_Flash_TMP[0], var_sel+1, LCD_Dig_Loc, 1);

	return (lcd_menu);
}

// ************************************************
//  Display hoistway short floor slowdowns
// ************************************************

int16 display_short_floor_sd(int16 lcd_menu)
{
	int16 i;
	float fl_dist;
	static int16 var_access;
	static int16 var_sel;
	int16 digit_start = 0;
	static int16 upd_dpy;
	static int16 fl;
	
	if(LCD_Sub_Init == 1)	// first time through Display hoistway tables
	{
		LCD_Sub_Init = 0;
		fl = cons[bottomf];
		upd_dpy = 1;
		var_access = 0;
		var_sel = 0;
		LCD_Dig_Point = 0;
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		if (var_access == 0)
		{
			fl++;
			if (fl > cons[topf])
				fl = cons[bottomf];
		}
		else if (var_access == 1)
		{
			var_sel--;
			if (var_sel < 0)
				var_sel = 2;
		}
		else if (var_access == 2)
		{
			
			if (var_sel == 0)
			  	adjust_variable(&DPP_SFl_Dn_SD[fl],0x7fff,0,1,LCD_Dig_Point);
			else if (var_sel == 1)
			  	adjust_variable_32(&DPP_Floor_Pos[fl],0x7fffffff,0,1,LCD_Dig_Point);
			else if (var_sel == 2)
			  	adjust_variable(&DPP_SFl_Up_SD[fl],0x7fff,0,1,LCD_Dig_Point);
		}
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		if (var_access == 0)
		{
			fl--;
			if (fl < cons[bottomf])
				fl = cons[topf];
	    }
	    else if (var_access == 1)
	    {
			var_sel++;
			if (var_sel > 2)
				var_sel = 0;
	    }
		else if (var_access == 2)
		{
			if (var_sel == 0)
			  	adjust_variable(&DPP_SFl_Dn_SD[fl],0x7fff,0,0,LCD_Dig_Point);
			else if (var_sel == 1)
			  	adjust_variable_32(&DPP_Floor_Pos[fl],0x7fffffff,0,0,LCD_Dig_Point);
			else if (var_sel == 2)
			  	adjust_variable(&DPP_SFl_Up_SD[fl],0x7fff,0,0,LCD_Dig_Point);
		}
	}

	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    upd_dpy = 1;
		if (var_access == 0) 
		{						// 0 = no access, 1 = modify var
			if(PasswordFlag == false)
		   	{
		  		StoreLCDInfo();
				clrLCDdpy();
		  		LCD_Menu = 25;
		  		LCD_Init = 1;
				return(lcd_menu);
	   	   	}
			var_access = 1;
		}
		else if (var_access == 1)
		{
			if (((fl == cons[topf]) && (var_sel != 0)) || ((fl == cons[bottomf]) && (var_sel != 2)))
				var_access = 2;
		}
		else if (var_access > 1)	// save field new value entered
	    {
		  	Wrt_Hoistway();
			var_access = 1;
		  	LCD_Dig_Point = 0;
	    }
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  //
		if (var_access == 0)
		{
			lcd_menu = 0;
			return(lcd_menu);
		}
		else if (var_access == 1)
		{
			upd_dpy = 1;
			var_access = 0;
		}
		else if (var_access == 2)
		{
			upd_dpy = 1;
			if (var_sel == 1)
	  			LCD_Dig_Point = set_adj_digit(0x7fffffff,LCD_Dig_Point);
			else 
	  			LCD_Dig_Point = set_adj_digit(0x7fff,LCD_Dig_Point);
		}
	}

	if(upd_dpy == 1)
	{
		upd_dpy = 0;
		fl_dist = 0;
		for(i=0; i<=19; i++)
		{
			LCD_Display[0][i] = LCD_Short_Floor_SD[0][i];
			LCD_Display[1][i] = LCD_Short_Floor_SD[1][i];
			LCD_Display[2][i] = LCD_Short_Floor_SD[2][i];
			LCD_Display[3][i] = LCD_Short_Floor_SD[3][i];
		}
	  
	  	itoa(fl,LCD_String);
		if(fl < 10)               
		  	LCD_Display[0][7] = LCD_String[0];    
		else
		{
		  	LCD_Display[0][6] = LCD_String[0];
		  	LCD_Display[0][7] = LCD_String[1];
		}
	    
	  	// Down slowdown count
	  	
	  	if (fl == cons[topf])
	  	{
			LCD_Display[1][5] = '-';
			LCD_Display[1][6] = '-';
			LCD_Display[1][7] = '-';
	  	}
	  	else
	  	{
			if ((var_access > 1) && (var_sel == 0))
				sprintf(&LCD_Display[1][4],"%05i",DPP_SFl_Dn_SD[fl]);
			else
				sprintf(&LCD_Display[1][4],"%-5i",DPP_SFl_Dn_SD[fl]);
			LCD_Display[1][9] = ' ';
			
		   	fl_dist = (float)( (((float)DPP_SFl_Dn_SD[fl])/(float)Pulses_per_Inch)/12.0);
			sprintf(&LCD_Display[1][15],"%5.1f",fl_dist);
	  	}
	  	
	  	
	  	// Floor position
		if ((var_access > 1) && (var_sel == 1))
			sprintf(&LCD_Display[2][4],"%09i",DPP_Floor_Pos[fl]);
		else
			sprintf(&LCD_Display[2][4],"%-9i",DPP_Floor_Pos[fl]);
		
	    LCD_Display[2][1] = flmrk[fl][0];//Put Display
	    LCD_Display[2][2] = flmrk[fl][1];// floor marking
		LCD_Display[2][13] = ' ';
	    
	   	fl_dist = (float)( (((float)(DPP_Floor_Pos[fl]-DPP_Floor_Pos[cons[bottomf]]))/(float)Pulses_per_Inch)/12.0);
		sprintf(&LCD_Display[2][15],"%5.1f",fl_dist);


		// Up slodown count
		if (fl == cons[bottomf])
		{
			LCD_Display[3][5] = '-';
			LCD_Display[3][6] = '-';
			LCD_Display[3][7] = '-';
		}
		else
		{
			if ((var_access > 1) && (var_sel == 2))
				sprintf(&LCD_Display[3][4],"%05i",DPP_SFl_Up_SD[fl]);
			else
				sprintf(&LCD_Display[3][4],"%-5i",DPP_SFl_Up_SD[fl]);
			LCD_Display[3][9] = ' ';
			
		   	fl_dist = (float)( (((float)DPP_SFl_Up_SD[fl])/(float)Pulses_per_Inch)/12.0);
			sprintf(&LCD_Display[3][15],"%5.1f",fl_dist);
		}

		if (var_access > 0)
		{
			if (var_sel == 0)
			{
				LCD_Display[1][3] = '>';
				digit_start = 8;
			}
			else if (var_sel == 1)
			{
				LCD_Display[2][3] = '>';
				digit_start = 12;
			}
			else if (var_sel == 2)
			{
				LCD_Display[3][3] = '>';
				digit_start = 8;
			}
			LCD_Dig_Loc = digit_start-LCD_Dig_Point;
	  		LCD_Flash_TMP[0] = LCD_Display[var_sel+1][LCD_Dig_Loc];
		}
	}
	if (var_access == 1)
		flash_digit('>', var_sel+1, 3, 1);
	else if (var_access == 2)
		flash_digit(LCD_Flash_TMP[0], var_sel+1, LCD_Dig_Loc, 1);

	return (lcd_menu);
}

int16 dpy_aps_valid_fl_table(int16 lcd_menu)
{
	
	int16 i,j;
	static int16 upd_dpy;
	
	if (LCD_Sub_Init == 1)
	{		// first time through
		LCD_Sub_Init = 0;
  	    upd_dpy = 1;
		timers[taps_ui] = 0;
		NTS_spi.valid_fl_req = 1;
	}
	
 	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  	{
  	    EnterButton = 1;
  	    upd_dpy = 1;
  	}
  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  	{
  	    ModeButton = 1;  //
		lcd_menu = 0;
		return(lcd_menu);
  	}
  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  	{
  	    UpButton = 1;  // incriment
  	    upd_dpy = 1;
  	}
  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  	{
  	    DownButton = 1;  // decriment
  	    upd_dpy = 1;
  	}

	if (timers[taps_ui] >= 3)		// request io status every 300 msec
	{
		NTS_spi.valid_fl_req = 1;
		timers[taps_ui] = 0;
	}
	
	if ((upd_dpy == 1) || (NTS_spi.valid_fl_rcvd == 1))
	{
		upd_dpy = 0;
		if (NTS_spi.valid_fl_rcvd == 1)
			NTS_spi.valid_fl_rcvd = 0;
		
		for(i=0; i<=19; i++)
		{
		    LCD_Display[0][i] = LCD_aps_valid_fl_tbl[0][i];
			LCD_Display[1][i] = LCD_aps_valid_fl_tbl[1][i];
		    LCD_Display[2][i] = LCD_aps_valid_fl_tbl[2][i];
			LCD_Display[3][i] = LCD_aps_valid_fl_tbl[3][i];
		}
		j=16;
		for (i=4; i<=19; i++)
		{
			if ((NTS_spi.valid_fl_tbl  & fl_mask[j]) != 0)
				LCD_Display[1][i] = '1';  // tables
			if ((NTS_spi.valid_clip_tbl  & fl_mask[j]) != 0)
				LCD_Display[3][i] = '1';  // tables
			j--;
		}
	}
	return (lcd_menu);
}



//*****************************************************
// Display cpu nts limits
//*****************************************************

int16 display_cpu_limits(int16 lcd_menu)
{
	int16 i;
	int16 fl = 1;
	float fl_dist;
	static int16 upd_dpy;
	static int16 hw_sel;

	if(LCD_Sub_Init == 1)	// first time through Display cpu limits
	{
		LCD_Sub_Init = 0;
		upd_dpy = 1;
		hw_sel = 0;
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		hw_sel++;
		if (hw_sel > 1)
			hw_sel = 0;
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		hw_sel--;
		if (hw_sel < 0)
			hw_sel = 1;
	}

	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  //
		lcd_menu = 0;
		return(lcd_menu);
	}

	if(upd_dpy == 1)
	{
		upd_dpy = 0;
		fl_dist = 0;
		for(i=0; i<=19; i++)
		{
			LCD_Display[0][i] = LCD_Term_Limits[0][i];
			if (hw_sel == 1)
			{
				LCD_Display[1][i] = LCD_Term_Limits[1][i];
				LCD_Display[2][i] = LCD_Term_Limits[2][i];
				LCD_Display[3][i] = LCD_Term_Limits[3][i];
			}
			else
			{
				LCD_Display[1][i] = LCD_Term_Limits[4][i];
				LCD_Display[2][i] = LCD_Term_Limits[5][i];
				LCD_Display[3][i] = LCD_Term_Limits[6][i];
			}
		}

	    
		if (hw_sel == 1)
		{
			sprintf(&LCD_Display[1][4],"%-9i",DPP_UN);
			LCD_Display[1][13] = ' ';
			
		   	fl_dist = (float)( (((float)(DPP_UN-DPP_Floor_Pos[cons[bottomf]]))/(float)Pulses_per_Inch)/12.0);
			sprintf(&LCD_Display[1][15],"%5.1f",fl_dist);
		}
		else
		{
			sprintf(&LCD_Display[1][4],"%-9i",DPP_DT);
			LCD_Display[1][13] = ' ';
			
		   	fl_dist = (float)( (((float)(DPP_DT-DPP_Floor_Pos[cons[bottomf]]))/(float)Pulses_per_Inch)/12.0);
			sprintf(&LCD_Display[1][15],"%5.1f",fl_dist);
		}

		if (hw_sel == 0)
			fl = cons[bottomf];
		else
			fl = cons[topf];
		
		sprintf(&LCD_Display[2][4],"%-9i",DPP_Floor_Pos[fl]);
		
	    LCD_Display[2][1] = flmrk[fl][0];//Put Display
	    LCD_Display[2][2] = flmrk[fl][1];// floor marking
		LCD_Display[2][13] = ' ';
	    
	   	fl_dist = (float)( (((float)(DPP_Floor_Pos[fl]-DPP_Floor_Pos[cons[bottomf]]))/(float)Pulses_per_Inch)/12.0);
		sprintf(&LCD_Display[2][15],"%5.1f",fl_dist);



		if (hw_sel == 1)
		{
			sprintf(&LCD_Display[3][4],"%-9i",DPP_UT);
			LCD_Display[3][13] = ' ';
			
		   	fl_dist = (float)( (((float)(DPP_UT-DPP_Floor_Pos[cons[bottomf]]))/(float)Pulses_per_Inch)/12.0);
			sprintf(&LCD_Display[3][15],"%5.1f",fl_dist);
		}
		else
		{
			sprintf(&LCD_Display[3][4],"%-9i",DPP_DN);
			LCD_Display[3][13] = ' ';
			
		   	fl_dist = (float)( (((float)(DPP_DN-DPP_Floor_Pos[cons[bottomf]]))/(float)Pulses_per_Inch)/12.0);
			sprintf(&LCD_Display[3][15],"%5.1f",fl_dist);
		}

	}
	return (lcd_menu);
}


// **************************************************
// Display Doorzone, Level zone and Selector Count
// **************************************************

int16 display_dz_off(int16 lcd_menu)
{
	int16 i;
	int16 fl = 1;
	float fl_dist;
	static int16 var_access;
	static int16 var_sel;
	int16 digit_start = 0;
	static int16 upd_dpy;
	static int16 hw_sel;
	int16 max_offset = 0;

	if(LCD_Sub_Init == 1)	// first time through Display cpu limits
	{
		LCD_Sub_Init = 0;
		upd_dpy = 1;
		var_access = 0;
		var_sel = 0;
		LCD_Dig_Point = 0;
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		if (var_access == 1)
		{
			var_sel--;
			if (var_sel < 0)
				var_sel = 1;
		}
		else if (var_access == 2)
		{
			max_offset = (int16)(Pulses_per_Inch);
			if (var_sel == 0)
			  	adjust_variable(&Dn_fl_level_dist,max_offset,0,1,LCD_Dig_Point);
			else if (var_sel == 1)
			  	adjust_variable(&Up_fl_level_dist,max_offset,0,1,LCD_Dig_Point);
		}
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		if (var_access == 1)
	    {
			var_sel++;
			if (var_sel > 1)
				var_sel = 0;
	    }
		else if (var_access == 2)
		{
			max_offset = (int16)(Pulses_per_Inch);
			if (var_sel == 0)
			  	adjust_variable(&Dn_fl_level_dist,max_offset,0,0,LCD_Dig_Point);
			else if (var_sel == 1)
			  	adjust_variable(&Up_fl_level_dist,max_offset,0,0,LCD_Dig_Point);
		}
	}

	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    upd_dpy = 1;
		if (var_access == 0) 
		{						// 0 = no access, 1 = modify var
			if(PasswordFlag == false)
		   	{
		  		StoreLCDInfo();
				clrLCDdpy();
		  		LCD_Menu = 25;
		  		LCD_Init = 1;
				return(lcd_menu);
	   	   	}
			var_access = 1;
		}
		else if (var_access == 1)
		{
			var_access = 2;
		}
		else if (var_access > 1)	// save field new value entered
	    {
		  	Wrt_Hoistway();
			var_access = 1;
		  	LCD_Dig_Point = 0;
	    }
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  //
		if (var_access == 0)
		{
			lcd_menu = 0;
			return(lcd_menu);
		}
		else if (var_access == 1)
		{
			upd_dpy = 1;
			var_access = 0;
		}
		else if (var_access == 2)
		{
			upd_dpy = 1;
  			LCD_Dig_Point = set_adj_digit(0x7fff,LCD_Dig_Point);
		}
	}

	if(upd_dpy == 1)
	{
		upd_dpy = 0;
		fl_dist = 0;
		for(i=0; i<=19; i++)
		{
			LCD_Display[0][i] = LCD_lev_dist[0][i];	 //  Header
			LCD_Display[1][i] = LCD_lev_dist[1][i];  //  Dn level distance
			LCD_Display[2][i] = LCD_lev_dist[2][i];  //  Up level distance
			LCD_Display[3][i] = LCD_lev_dist[3][i];  //  Pulses per inch
		}

	    

		if ((var_access > 1) && (var_sel == 0))
			sprintf(&LCD_Display[1][11],"%05i",Dn_fl_level_dist);
		else
			sprintf(&LCD_Display[1][11],"%-5i",Dn_fl_level_dist);
		LCD_Display[1][16] = ' ';
			
		if ((var_access > 1) && (var_sel == 1))
			sprintf(&LCD_Display[2][11],"%05i",Up_fl_level_dist);
		else
			sprintf(&LCD_Display[2][11],"%-5i",Up_fl_level_dist);
		LCD_Display[2][16] = ' ';
			
		sprintf(&LCD_Display[3][6],"%5.1f",Pulses_per_Inch);
		LCD_Display[3][11] = ' ';



		if (var_access > 0)
		{
			if (var_sel == 0)
			{
				LCD_Display[1][9] = '>';
				digit_start = 15;
			}
			else if (var_sel == 1)
			{
				LCD_Display[2][9] = '>';
				digit_start = 15;
			}
			LCD_Dig_Loc = digit_start-LCD_Dig_Point;
	  		LCD_Flash_TMP[0] = LCD_Display[var_sel+1][LCD_Dig_Loc];
		}
	}
	if (var_access == 1)
		flash_digit('>', var_sel+1, 9, 1);
	else if (var_access == 2)
		flash_digit(LCD_Flash_TMP[0], var_sel+1, LCD_Dig_Loc, 1);

	return (lcd_menu);
}


//********************************
// Display variable 
//********************************

void display_variable(int16 row, int16 col, int16 digit, int16 lcd_par, int16 flash)
{							// display data
	char next_char;
	int16 digit_adjust = 0;

	if (digit > 3)
		digit = 0;

	next_char = LCD_Display[row][col+6];
	if (digit == 1)
	{
		if (LCD_Dig_Point > 0)
			digit_adjust = 1;
		sprintf(&LCD_Display[row][col],"%04i.%01i",lcd_par/10,lcd_par%10);
	}
	else if (digit == 2)
	{
		if (LCD_Dig_Point > 1)
			digit_adjust = 1;
		sprintf(&LCD_Display[row][col],"%03i.%02i",lcd_par/100,lcd_par%100);
	}
	else if (digit == 3)
	{
		if (LCD_Dig_Point > 2)
			digit_adjust = 1;
		sprintf(&LCD_Display[row][col],"%02i.%03i",lcd_par/1000,lcd_par%1000);
	}
	else
		sprintf(&LCD_Display[row][col],"%06i",lcd_par);

	LCD_Display[row][col+6] = next_char;

	if (flash == 1)
	{
		LCD_Dig_Loc = (col + 5) - (LCD_Dig_Point + digit_adjust);
  		LCD_Flash_TMP[row] = LCD_Display[row][LCD_Dig_Loc];
	}
}
//********************************
// Display variable  11 digits
//********************************

void display_variable_11_digits(int16 row, int16 col, int16 digit, int32 lcd_par, int16 flash)
{							// display data
	char next_char;
	int16 digit_adjust = 0;

	if (digit > 3)
		digit = 0;

	next_char = LCD_Display[row][col+11];
	if (digit == 1)
	{
		if (LCD_Dig_Point > 0)
			digit_adjust = 1;
		sprintf(&LCD_Display[row][col],"%09i.%01i",lcd_par/10,lcd_par%10);
	}
	else if (digit == 2)
	{
		if (LCD_Dig_Point > 1)
			digit_adjust = 1;
		sprintf(&LCD_Display[row][col],"%08i.%02i",lcd_par/100,lcd_par%100);
	}
	else if (digit == 3)
	{
		if (LCD_Dig_Point > 2)
			digit_adjust = 1;
		sprintf(&LCD_Display[row][col],"%07i.%03i",lcd_par/1000,lcd_par%1000);
	}
	else
		sprintf(&LCD_Display[row][col],"%011i",lcd_par);

	LCD_Display[row][col+11] = next_char;

	if (flash == 1)
	{
		LCD_Dig_Loc = (col + 10) - (LCD_Dig_Point + digit_adjust);
  		LCD_Flash_TMP[row] = LCD_Display[row][LCD_Dig_Loc];
	}
}



//***********************************
// Adjust field adjustable variable
//***********************************

const int16 adj_amt[10] = {1,10,100,1000,10000,-1,-1,-1,-1,-1};

void adjust_variable (int16 *pvar, int16 max, int16 min, int16 inc, int16 digit)
{

	if (digit > 5)
		digit = 0;

	if (inc == 1)		// Increment variable
		*pvar += adj_amt[digit];
	else
		*pvar -= adj_amt[digit];

	if (*pvar > max)
		*pvar = max;
	else if ((min >= 0) && (*pvar < 0)) 
		*pvar = min;
	else if (*pvar < min)
		*pvar = min;
}

//***********************************
// Adjust field adjustable variable
//***********************************

const int32 adj_amt_32[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};

void adjust_variable_32 (int32 *pvar, int32 max, int32 min, int32 inc, int16 digit)
{

	if (digit > 9)
		digit = 0;

	if (inc == 1)		// Increment variable
		*pvar += adj_amt_32[digit];
	else
		*pvar -= adj_amt_32[digit];

	if (*pvar > max)
		*pvar = max;
	else if ((min >= 0) && (*pvar < 0)) 
		*pvar = min;
	else if (*pvar < min)
		*pvar = min;
}

//********************************************
// Set the digit to alter adjustable variable
//********************************************

int16 set_adj_digit (int32 max,int16 digit)
{
	int32 tmax;

	digit++;
	if (digit > 9)
		digit = 0;

	if (max == -1)
		tmax = cons[nmbcars];
	else if (max == -2)
		tmax = cons[grtopf];
	else if (max == -3)
		tmax = cons[speed];
	else
		tmax = max;

	if ((uint16)adj_amt_32[digit] > (uint16)tmax)
		digit = 0;
	return digit;
}


//***********************************
// Flash digit being adjusted
//***********************************

void flash_digit (char dchar, int16 row, int16 col, int16 enable)
{
	if (enable == 1)
	{
		if(LCD_Flash != 0)
			LCD_Display[row][col] = dchar;
		else
			LCD_Display[row][col] = ' ';
	}
}


//*********************************************************
// Get io offset from bit location
//*********************************************************

int16 count_one_bit_16(uint16 bitnmb)
{
	int16 i;
	int16 j = 0;
	int16 k = 0;

	for (i=0;i<=15;i++)
	{
		if ((bitnmb & bittbl[i]) != 0)
		{
			j = i;
			k++;
			if (k > 1)
				return 16;
		}
	}
	if (k <= 1)
		return j;
	else
		return 16;
}



//******************************************
// Display Variable Label instead of number
//******************************************

// Car Push Button Options Menu 10
/*
  CC Button On Color				275 C 	0
  CC Button Off Color				276 C 	5
  CC Button Security Color			286 C 	10
  Car Call Button Att Up Color		320 C 	15
  Car Call Button Att Dn Color		325 C 	20
  Fire light Color					330 C 	26
  Medical light Color				335 C 	31
  Emergency light Color				340 C 	36
  OTS light Color					345 C 	41
  Backlight Output lights  			350 C 	46

// Hall Push Button Options	Menu 11

  HCB On Color 	   					264 G 	0 
  HCB Off Color 	   				265 G 	5 
  HCB Sec Color	   					266 G 	10
  HCB Sec Lt 		   				267 G  	15
},
*/

void Display_Var_Label(int16 menu, int16 fv_index, int16 var)
{
	int16 i;
	int16 j;
	
	const char * LCD_Var_Label_ptr;
	
	if ( ((menu == CC_COP_Lights_Menu) && ((fv_index == fvcconcolor) || (fv_index == fvccoffcolor) || (fv_index == fvccseccolor) || (fv_index == fvccattupcolor) || (fv_index == fvccattdncolor) ||
			(fv_index == fvcopfirecolor) || (fv_index == fvcopmedcolor) || (fv_index == fvcopemcolor)	|| (fv_index == fvcopotscolor))) ||
		 ((menu == HC_IR_Call_Lights_Menu) && ((fv_index == fvhcbuponcolor) || (fv_index == fvhcbupoffcolor) || (fv_index == fvhcbdnoncolor) || (fv_index == fvhcbdnoffcolor)|| (fv_index == fvhcbiruponcolor) || 
			(fv_index == fvhcbirupoffcolor)|| (fv_index == fvhcbirdnoncolor) || (fv_index == fvhcbirdnoffcolor))) ||
		 ((menu == CB_Vip_HSec_Call_Lights_Menu) && ((fv_index == fvhcbcboncolor) || (fv_index == fvhcbcboffcolor) || (fv_index == fvhcbviponcolor) || (fv_index == fvhcbvipoffcolor)|| (fv_index == fvhcbseccolor))) )
	{
//		LCD_Var_Label_ptr = &LCD_Var_Labels[var][1];
	    for(i=0; i<=19; i++)
        {
	    	LCD_Display[1][i] =  LCD_Var_Labels[var][i];
	    }
	}
	else if ((menu == CC_COP_Lights_Menu) && (fv_index == fvbklton))
	{		// Backlight Output lights
	    for(i=0; i<=19; i++)
  	    {
	    	LCD_Display[1][i] =  LCD_Backlight_Output[var][i];
	    }
	}
	else if ((menu == CB_Vip_HSec_Call_Lights_Menu) && (fv_index == fvhcbseclt))
	{ 	// HCB Sec Lt
	    for(i=0; i<=19; i++)
  	    {
	    	LCD_Display[1][i] =  LCD_Button_Sec_Light[var][i];
	    }
	}
#if (GALaxy_4 == 1)
	else if ((menu == Safety_Proc_Adj_Vars_Menu) && (fv_index == 3))		// encoder type
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = SPB_Encoder_Type[var][i];
	    }
	}
#endif	
	else if((menu == System_Options_Menu) && ((fv_index >= fvexcflt1) && (fv_index <= fvexcflt6)))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Fault[var][i];
	    }
	}
	else if ((menu == System_Options_Menu) && (fv_index == fvenctype))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = CAN_Encoder_Type[var][i];
	    }
	}
	else if ((menu == Car_Motion_Menu) && (fv_index == fvuldeadzone))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = UL_Dead_Zone_Sensor[var][i];
	    }
	}
	else if ((menu == Car_Motion_Menu) && (fv_index == fvdldeadzone))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = DL_Dead_Zone_Sensor[var][i];
	    }
	}
	else if ((menu == System_Options_Menu) && 
		((fv_index == fvbaud) || (fv_index == fvbaud2) || (fv_index == fv485baud) || (fv_index == fvupsbaud) ))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = Baud_Display[var][i];
	    }
	}
	else if ((menu == System_Options_Menu) && (fv_index == fvdrvbaud))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = Drv_Baud_Display[var][i];
	    }
	}
	else if((menu == System_Options_Menu) && ((fv_index == fvcomdiag1) || (fv_index == fvcomdiag2)) )
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = Com_Sel_Display[var][i];
	    }
	}
	else if ((menu == Service_Options_Menu) && (fv_index == fvservoctrl))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Servf[var][i];
	    }
	}
	else if ((menu == Group_Options_Menu) && (fv_index == fvxasgn))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = Xassign_Display[var][i];
	    }
	}
	else if ((menu == Emergency_Service_Menu) && (fv_index == fvfiresw))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = Fire_SW_Display[var][i];
	    }
	}
	else if ((menu == Emergency_Service_Menu) && (fv_index == fvfire_rst))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = Fire_RST_Display[var][i];
	    }
	}
	else if ((menu == Car_Options_Menu)  && (fv_index == fviser))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = ISER_Display[var][i];
	    }
	}
	else if ((menu == Group_Options_Menu) && (fv_index == fvirctl))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = IR_CTL_Display[var][i];
	    }
	}
	else if ((menu == Emergency_Service_Menu) && (fv_index == fvfsofl))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = FSO_Display[var][i];
	    }
	}
	else if ((menu == Car_Options_Menu) && (fv_index == fvrmcc))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = RMCC_Display[var][i];
	    }
	}
	else if ((menu == Service_Options_Menu) && (fv_index == fvsecrec))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = SEC_RCL_Display[var][i];
	    }
	}
	else if ((menu == Service_Options_Menu) && (fv_index == fvdobsec))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = DOB_SEC_Display[var][i];
	    }
	}
	else if ((menu == Group_Options_Menu) && (fv_index == fvepopled))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = OP_LED_Display[var][i];
	    }
	}
	else if ((menu == Group_Options_Menu) && (fv_index == fveppkled))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = PK_LED_Display[var][i];
	    }
	}
	else if ((menu == Service_Options_Menu) && (fv_index == fvheloff))
	{
		j = count_one_bit_16(var);
		
		if (j != 16)
			LCD_Var_Label_ptr = &HEOF_Display[j][0];
		else
			LCD_Var_Label_ptr = &Multi_Display[var][0];

	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Var_Label_ptr[i];
	    }
//	    for(i=0; i<=19; i++)
//		{
//			LCD_Display[1][i] = HEOF_Display[var][i];
//	    }
	}
	else if ((menu == Service_Options_Menu) && (fv_index == fvsecrec2))
	{
	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = SEC_RCL_2_Display[var][i];
	    }
	}
	else if ((menu == Car_Options_Menu) && (fv_index == fvopenlctl))
	{

		j = count_one_bit_16(var);
		
		if (j != 16)
			LCD_Var_Label_ptr = &OPENL_Display[j][0];
		else
			LCD_Var_Label_ptr = &Multi_Display[var][0];

	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Var_Label_ptr[i];
	    }
	}
	else if ((menu == Car_Options_Menu) && (fv_index == fvoser1))
	{

		j = count_one_bit_16(var);
		
		if (j != 16)
			LCD_Var_Label_ptr = &OSER_CTL_Display[j][0];
		else
			LCD_Var_Label_ptr = &Multi_Display[var][0];

	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Var_Label_ptr[i];
	    }
	    
	}
	else if ((menu == Service_Options_Menu) && (fv_index == fvsabben))
	{

		j = count_one_bit_16(var);
		
		if (j != 16)
			LCD_Var_Label_ptr = &SABB_EN_Display[j][0];
		else
			LCD_Var_Label_ptr = &Multi_Display[var][0];

	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Var_Label_ptr[i];
	    }
	    
	}
	else if ((menu == Service_Options_Menu) && (fv_index == fvceloff))
	{

		j = count_one_bit_16(var);
		
		if (j != 16)
			LCD_Var_Label_ptr = &ELEV_OFF_Display[j][0];
		else
			LCD_Var_Label_ptr = &Multi_Display[var][0];

	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Var_Label_ptr[i];
	    }
	    
	}
	else if ((menu == Service_Options_Menu) && (fv_index == fvceloff2))
	{

		j = count_one_bit_16(var);
		
		if (j != 16)
			LCD_Var_Label_ptr = &ELEV_OFF_2_Display[j][0];
		else
			LCD_Var_Label_ptr = &Multi_Display[var][0];

	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Var_Label_ptr[i];
	    }
	    
	}
	else if ((menu == Car_Options_Menu) && (fv_index == fvrmccd))
	{

		j = count_one_bit_16(var);
		
		if (j != 16)
			LCD_Var_Label_ptr = &RMCC_DIS_Display[j][0];
		else
			LCD_Var_Label_ptr = &Multi_Display[var][0];

	    for(i=0; i<=19; i++)
  	    {
			LCD_Display[1][i] = LCD_Var_Label_ptr[i];
	    }
	    
	}
	
}



//*************************************
//** Elevator Status Initialization ***
//*************************************

void Elevator_Status_Init (int16 Init_Flag)
{
	int16 i,j;
	static int16 upd_dpy;

	if(Init_Flag == 0)
	{		// Elevator Graphics display
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = ' ';  
		   	LCD_Display[1][i] = ' ';
		   	LCD_Display[2][i] = ' ';
		   	LCD_Display[3][i] = ' ';
	    }
	}
	else if (Init_Flag == 1)
	{		// Elevator Service Display		   
	    upd_dpy = 1;	 // Update PI
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = ' ';  
		   	LCD_Display[1][i] = ' ';
		   	LCD_Display[2][i] = ' ';
		   	LCD_Display[3][i] = ' ';
	    }
	}
	else if (Init_Flag == 2)
	{		// NTS Status
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_NTS_Status[0][i];
			LCD_Display[1][i] = LCD_NTS_Status[1][i];
			LCD_Display[2][i] = LCD_NTS_Status[2][i];
			LCD_Display[3][i] = LCD_NTS_Status[3][i];
		}
	}
	else if ((Init_Flag >= 3) && (Init_Flag <= 10))
	{	// IO Display
		j = (Init_Flag - 3) * 12;

		display_io_name(j);
	}
	else if (Init_Flag == 11)
	{					//  Chk Run State, Start State, Leveling State, door state
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Detail_Fault[32][i];  
		   	LCD_Display[1][i] = LCD_Detail_Fault[33][i];
			LCD_Display[2][i] = LCD_Detail_Fault[34][i];  
		   	LCD_Display[3][i] = LCD_Detail_Fault[35][i];
	    }
	}
	else if (Init_Flag == 12)
	{
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Dbg_Status[0][i];
			LCD_Display[1][i] = LCD_Dbg_Status[1][i];
			LCD_Display[2][i] = LCD_Dbg_Status[2][i];
			LCD_Display[3][i] = LCD_Dbg_Status[3][i];
		}
	}
	else if (Init_Flag == 13)
	{
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Dbg_Status2[0][i];
			LCD_Display[1][i] = LCD_Dbg_Status2[1][i];
			LCD_Display[2][i] = LCD_Dbg_Status2[2][i];
			LCD_Display[3][i] = LCD_Dbg_Status2[3][i];
		}
	}
/*	
	else if (Init_Flag == 9)
	{
	    upd_dpy = 1;	 // Run  Screen
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Run_Status[0][i];
			LCD_Display[1][i] = LCD_Run_Status[1][i];
		}
	}
	else if (Init_Flag == 10)
	{					  // Fire status
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Fire_dpy_Status[0][i];
			LCD_Display[1][i] = LCD_Fire_dpy_Status[1][i];
		}
	}
	else if (Init_Flag == 11)
	{
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Run_Time[0][i];
			LCD_Display[1][i] = LCD_Run_Time[1][i];
		}
	}
	else if (Init_Flag == 14)
	{
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Voltage[0][i];
			LCD_Display[1][i] = LCD_Voltage[1][i];
		}
	}
	else if (Init_Flag == 23)
	{					// Fault bits
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Detail_Fault[30][i];  
		   	LCD_Display[1][i] = LCD_Detail_Fault[31][i];
	    }
	}
	else if (Init_Flag == 24)
	{					// Nudging status, Door Request, Call Flags, Chk Run State, Start State, Leveling State
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Detail_Fault[38][i];  
		   	LCD_Display[1][i] = LCD_Detail_Fault[39][i];
	    }
	}
	else if (Init_Flag == 25)
	{					// Check door status, front slowdown, rear slowdown and motion timer.
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_Detail_Fault[40][i];  
		   	LCD_Display[1][i] = LCD_Detail_Fault[41][i];
	    }
	}
	*/
}

void display_io_name (int16 ioloc)
{
	int16 i,j,k,m;
	
	/*		Example:
		  "xxxAD.|xL120.|xxxTF.",	// 0
		  "xxIND.|xxS10.|xxxBF.",	// 1
		  "xxLBP.|     .|xxxPS.",	// 2
		  "xxGBP.|xxGOV.|xxHSS.",	// 3
	*/		
		
    for(i=0; i<=19; i++)
    {
    	LCD_Display[0][i] = LCD_Dpy_IO_Row[i];
		LCD_Display[1][i] = LCD_Dpy_IO_Row[i];
	  	LCD_Display[2][i] = LCD_Dpy_IO_Row[i];
	    LCD_Display[3][i] = LCD_Dpy_IO_Row[i];
    }

    for(i=0; i<=2; i++)
    {
	    k = (i * 7);
	    m = i * 4;
		for (j=0;j<=4;j++)
		{
			LCD_Display[0][k + j] = ioname[ioloc+m][j];
			LCD_Display[1][k + j] = ioname[ioloc+m+1][j];
			LCD_Display[2][k + j] = ioname[ioloc+m+2][j];
			LCD_Display[3][k + j] = ioname[ioloc+m+3][j];
    	}
    }
	if ((io_valid[(ioloc+m)/8] & bittbl[(ioloc+m) & 0x7]) != 0)
	{
		LCD_Display[0][k + 5] = io_off;
	}
	if ((io_valid[(ioloc+m+1)/8] & bittbl[(ioloc+m+1) & 0x7]) != 0)
	{
		LCD_Display[1][k + 5] = io_off;
	}
	if ((io_valid[(ioloc+m+2)/8] & bittbl[(ioloc+m+2) & 0x7]) != 0)
	{
		LCD_Display[2][k + 5] = io_off;
	}
	if ((io_valid[(ioloc+m+3)/8] & bittbl[(ioloc+m+3) & 0x7]) != 0)
	{
		LCD_Display[3][k + 5] = io_off;
	}
}
	
static int16 const io_row[12] = {0,1,2,3,0,1,2,3,0,1,2,3};
static int16 const io_col[12] = {5,5,5,5,12,12,12,12,19,19,19,19};
	
void display_io_value (int16 ioloc, struct Fault *ptr)
{
	int16 i,k;
  	for (i=0;i<=11;i++)
  	{
		k = (ioloc+i)/8;
		if ((io_valid[k] & bittbl[(ioloc+i) & 0x7]) != 0) 
		{		 // valid i/o
			if((ptr->iodata[k] & bittbl[(ioloc+i) & 0x7]) != 0)
			{
			    if((IO_In_Out[k] & bittbl[(ioloc+i) & 0x7]) != 0)	// it is an output off
					LCD_Display[io_row[i]][io_col[i]] = io_off;
			    else										// it is an input  on
					LCD_Display[io_row[i]][io_col[i]] = io_on;
			}
	 		else
			{
			    if((IO_In_Out[k] & bittbl[(ioloc+i) & 0x7]) != 0)	// it is an output on
					LCD_Display[io_row[i]][io_col[i]] = io_on;
	  	  	  	else										// it is an input off
					LCD_Display[io_row[i]][io_col[i]] = io_off;
			}
		}
  	}
}

//***********************************************
//******     Elevator Service    *****************
//***********************************************

void Elevator_Service (struct Fault *ptr)
{
	int16 i,j;
	static int16 upd_dpy;

	// Service on line 1

	if ((ptr->servf == s_INSP) && (ptr->procf == 2))
	{					  // on inspection 
		for(i=0; i<=19; i++)
		{
		    LCD_Display[0][i] = LCD_Ins_Servf[ptr->ins_servf][i];
		}
	}
	else if ((ptr->servf == s_FIRE2) || (ptr->servf == s_F1MAIN) || (ptr->servf == s_F1ALT))
	{		// on fire service
		for(i=0; i<=19; i++)
		{
		    LCD_Display[0][i] = LCD_Fire_status[ptr->firef][i];
		}
	}
	else
   	{
		for(i=0; i<=19; i++)
		{
		    LCD_Display[0][i] = LCD_Servf[ptr->servf][i];
		}
   	}

	// Elevator process line 2
    for(i=0; i<=19; i++)
    {
		LCD_Display[1][i] = LCD_Procf[ptr->procf][i];
    }
    upd_dpy = 1;

	// Elevator Door Line 3

	if ((ptr->doorf != 0) || ((cons[rear] == 1) && (ptr->rdoorf != 0)))
  	{
  		if (ptr->doorf != 0)
  		{
	  	    for(i=0; i<=19; i++)
	  	    {
				if ((ptr->servf == s_AUTO) && (ptr->doorf == 2))
					LCD_Display[2][i] = LCD_Door_dwell[0][i];
				else
				   	LCD_Display[2][i] = LCD_Doorf[ptr->doorf][i];
		  	}
	      	upd_dpy = 1;
	  	}
	  	else 
	  	{
			for(i=0; i<=19; i++)
			{
				if ((ptr->servf == s_AUTO) && (ptr->rdoorf == 2))
					LCD_Display[2][i] = LCD_Door_dwell[1][i];
				else
					LCD_Display[2][i] = LCD_Rdoorf[ptr->rdoorf][i];
		    }
	  	    upd_dpy = 1;
	  	}
	}
	else 
	{		// doorf == 0 the check limits

		if (cons[rear] == 0) 
		{
			for(i=0; i<=19; i++)
			{
				if (rdinp_fm(i_GS,ptr) == 1)
					LCD_Display[2][i] = LCD_Doorf[0][i];
				else if ((ptr->run_statusf & rs_DOL) == 0)
				   	LCD_Display[2][i] = LCD_Doorf[2][i];
				else 
				   	LCD_Display[2][i] = LCD_Doorf[5][i];
		    }
		}
		else 
		{
			for(i=0; i<=19; i++)
			{
				if ((rdinp_fm(i_RGS,ptr) == 1) && (rdinp_fm(i_GS,ptr) == 1))
					LCD_Display[2][i] = LCD_Doorf[0][i];
				else if ((rdinp_fm(i_RGS,ptr) == 0) && (rdinp_fm(i_GS,ptr) == 1))
				{
					if ((ptr->run_statusf & rs_DOLR) == 0)
					   	LCD_Display[2][i] = LCD_Rdoorf[2][i];
					else 
					   	LCD_Display[2][i] = LCD_Rdoorf[5][i];
				}
				else if ((rdinp_fm(i_RGS,ptr) == 1) && (rdinp_fm(i_GS,ptr) == 0))
				{
					if ((ptr->run_statusf & rs_DOL) == 0)
					   	LCD_Display[2][i] = LCD_Doorf[2][i];
					else 
					   	LCD_Display[2][i] = LCD_Doorf[5][i];
				}
				else 
				{
					if ((ptr->run_statusf & rs_DOL) == 0)
					   	LCD_Display[2][i] = LCD_Doorf[2][i];
					else if ((ptr->run_statusf & rs_DOLR) == 0)
					   	LCD_Display[2][i] = LCD_Rdoorf[2][i];
					else 
					   	LCD_Display[2][i] = LCD_Doorf[5][i];
				}
			}
		}
	}				
	

	// Status Flag Line 4
	
	if ((ptr->statusf != 0) || (ptr->statusf2 != 0) || (ptr->statusf3 != 0) || (ptr->statusf4 != 0))
	{

	  	if (((ptr->statusf & sf_SS) != 0) && (ptr->ss_status != 0))
	  	{
			j = 0;
			while (j<=15)
			{
			    if ((ptr->ss_status  & bittbl[j++]) != 0)
			    	break;
			}
	  	    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[3][i] = LCD_SS_Status[j][i];
	  	    }
	  	}
	  	else if (ptr->statusf != 0)
	  	{
			j = 0;
			while (j<=31)
			{
		      	if ((ptr->statusf & l_bit_mask[j++]) != 0)
			    	break;
			}
	  	    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[3][i] = LCD_Statusf[j][i];
	  	    }
	  	}
	  	else if (ptr->statusf2 != 0)
	  	{
			j = 0;
			while (j<=31)
			{
		      	if ((ptr->statusf2 & l_bit_mask[j++]) != 0)
			    	break;
			}
	  	    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[3][i] = LCD_Statusf2[j][i];
	  	    }
	  	}
	  	else if (ptr->statusf3 != 0)
	  	{
			j = 0;
			while (j<=31)
			{
		      	if ((ptr->statusf3 & l_bit_mask[j++]) != 0)
			    	break;
			}
	  	    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[1][i] = LCD_Statusf3[j][i];
	  	    }
	  	}
	  	else
	  	{
			j = 0;
			while (j<=31)
			{
		      	if ((ptr->statusf4 & l_bit_mask[j++]) != 0)
			    	break;
			}
	  	    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[1][i] = LCD_Statusf4[j][i];
	  	    }
	  	}
	}
	else 
	{
	  for(i=0; i<=19; i++)
	  {
	      LCD_Display[3][i] = LCD_Fault[ptr->code][i];
	  }
	}
	

	LCD_Display[2][18] = flmrk[ptr->pos][0];
	LCD_Display[2][19] = flmrk[ptr->pos][1];
}


//************************************
//************************************
//************************************
void Elevator_Status (int16 Status_Dpy, struct Fault *ptr)
{
	int16 i;

	switch (Status_Dpy)
	{
		case 0:
		{
				//  " [_]^15 DPC= 00000000",   // 36 - 10
				//  " MVs^   SDC=000000000",   // 37 - 10
				//  " AUT    Up Fast      ",   // 38 - 5, 18
				//  " 000^fpm  tix=       ",   // 39 - 6, 16
		  	LCD_Display[0][1] = '[';
		  	LCD_Display[0][3] = ']';

			if (cons[rear] == 0)
			{
				if (((ptr->run_statusf & rs_DCL) == 0) || (rdinp_fm(i_GS,ptr) == 1))
				  	LCD_Display[0][2] = doors_closed;		// both doors closed
				else
				  	LCD_Display[0][2] = fdoor_open;		// Front door open
			}
			else
			{
				if (((ptr->run_statusf & (rs_DCL | rs_DCLR)) == 0) || ((rdinp_fm(i_GS,ptr) == 1) && (rdinp_fm(i_RGS,ptr) == 1)))
				  	LCD_Display[0][2] = doors_closed;		// both doors closed
				else 
				{
					if (((ptr->run_statusf & rs_DCL) == 0) && ((ptr->run_statusf & rs_DCLR) != 0))
					  	LCD_Display[0][2] = rdoor_open;		// rear door open
					else if (((ptr->run_statusf & rs_DCL) != 0) && ((ptr->run_statusf & rs_DCLR) == 0))
					  	LCD_Display[0][2] = fdoor_open;		// Front door open
					else
					  	LCD_Display[0][2] = ' ';		// both doors open
				}
			}
			
			
			if (ptr->dpref == 1)
		  		LCD_Display[0][4] = up_arrow;		// Up
			else if (ptr->dpref == 2)
			  	LCD_Display[0][4] = dn_arrow;		// Down
			else
			  	LCD_Display[0][4] = ' ';			// None
					  	
			LCD_Display[0][5] = flmrk[ptr->pos][0];
			LCD_Display[0][6] = flmrk[ptr->pos][1];
			
			
				
			if (rdinp_fm(i_MCC,ptr) == 1)
			  	LCD_Display[1][1] = motor_on;		// motor on
			else
			  	LCD_Display[1][1] = ' ';		// motor off
			
			if ((rdoutp_fm(o_RUN,ptr) == 1) && ((rdoutp_fm(o_SU,ptr) == 1) || (rdoutp_fm(o_SD,ptr) == 1)))
			{
				if (rdinp_fm(i_SU,ptr) == 1)
		  			LCD_Display[1][2] = up_arrow;		// Up
				else
				  	LCD_Display[1][2] = dn_arrow;		// Down
				  	
				LCD_Display[1][3] = valve_on;		// valve on
				  	
				if ((rdoutp_fm(o_SUF,ptr) == 1) || (rdoutp_fm(o_SDF,ptr) == 1))
		  			LCD_Display[1][4] = 'f';		// fast valve
				else
				  	LCD_Display[1][4] = ' ';		// no fast valve
				  	
			}
			else
			{
				LCD_Display[1][2] = ' ';		// Direction off
				LCD_Display[1][3] = ' ';		// valve off
				LCD_Display[1][4] = ' ';		// high speed off
			}
			
		  	LCD_Display[2][1] = servf_tbl[ptr->servf][0];	
		  	LCD_Display[2][2] = servf_tbl[ptr->servf][1];	
		  	LCD_Display[2][3] = servf_tbl[ptr->servf][2];

			LCD_Display[0][8] = 'D';
			LCD_Display[0][9] = 'P';
			LCD_Display[0][10] = 'C';
			LCD_Display[0][11] = '=';

			itoa(ptr->dpp_count, LCD_String);  // Put out current Dpp count
			i=0;
			while(((LCD_String[i] >= '0') && (LCD_String[i] <= '9')) || (LCD_String[i] == '-'))
			{
			 	LCD_Display[0][i+12] = LCD_String[i];
			 	i++;
			}
			while((i+12) < 19)
			{
			 	LCD_Display[0][i+12] = ' ';
			 	i++;
			}
			

			if (((ptr->run_statusf & (rs_UL | rs_DL | rs_DZ)) != 0) && ((ptr->run_statusf & rs_HSF) == 0)) 
			{		// In the door zone and not running high speed
				LCD_Display[1][8] = 'F';
				LCD_Display[1][9] = 'P';
				LCD_Display[1][10] = 'C';
				LCD_Display[1][11] = '=';
				itoa(ptr->fl_dpp_count, LCD_String);  // Put out current Dpp count
				i=0;
				while(((LCD_String[i] >= '0') && (LCD_String[i] <= '9')) || (LCD_String[i] == '-'))
				{
				 	LCD_Display[1][i+12] = LCD_String[i];
				 	i++;
				}
				while((i+12) < 19)
				{
				 	LCD_Display[1][i+12] = ' ';
				 	i++;
				}
			}
			else
			{
				LCD_Display[1][8] = 'S';
				LCD_Display[1][9] = 'D';
				LCD_Display[1][10] = 'C';
				LCD_Display[1][11] = '=';
				itoa(ptr->sd_count, LCD_String);  // Put out current Dpp count
				i=0;
				while(((LCD_String[i] >= '0') && (LCD_String[i] <= '9')) || (LCD_String[i] == '-'))
				{
				 	LCD_Display[1][i+12] = LCD_String[i];
				 	i++;
				}
				while((i+12) < 19)
				{
				 	LCD_Display[1][i+12] = ' ';
				 	i++;
				}
			}
			
			// Elevator process line 3
			if ((ptr->procf>=3) && (ptr->procf<=11))
			{
			    for(i=8; i<=18; i++)
			    {
					LCD_Display[2][i] = LCD_Procf2[ptr->procf-3][i-8];
			    }
			}
			else
			{
			    for(i=8; i<=19; i++)
			    {
					LCD_Display[2][i] = ' ';
			    }
			}

			// Car Velocity
			itoa(ptr->enc_vel_fpm,LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
			  	LCD_Display[3][(i+1)] = LCD_String[i];
			  	i++;
			}
			while((i+1) < 9)
			{
			  	LCD_Display[3][(i+1)] = ' ';
			  	i++;
			}

			if (ptr->enc_dir == 1)
			  	LCD_Display[3][5] = up_arrow;
			else if (ptr->enc_dir == 2)
			  	LCD_Display[3][5] = dn_arrow;
			else
			  	LCD_Display[3][5] = ' ';
			
			LCD_Display[3][6] = 'f';
			LCD_Display[3][7] = 'p';
			LCD_Display[3][8] = 'm';

			if (Trace_Stop == 1)
			{
				if ((trace_stop_ix + 36) >= max_trace_ix)		// go to beginning of trace
					trace_dpy_offset = (int16)(36 - (max_trace_ix - trace_stop_ix));
				else
					trace_dpy_offset = (int16)(trace_stop_ix + 36);
					
				if (trace_dpy_ix < trace_dpy_offset)
					dpy_offset_ix = (trace_dpy_ix + (max_trace_ix - trace_dpy_offset));
				else
					dpy_offset_ix = trace_dpy_ix - trace_dpy_offset;
				
				// Trace Index
				LCD_Display[3][13] = 't';
				LCD_Display[3][14] = 'i';
				LCD_Display[3][15] = 'x';
				LCD_Display[3][16] = '=';
				itoa(dpy_offset_ix,LCD_String);
				i=0;
				while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
				{
				  	LCD_Display[3][i+17] = LCD_String[i];
				  	i++;
				}
				while((i+17) <= 19)
				{
				  	LCD_Display[3][(i+17)] = ' ';
				  	i++;
				}
			}
			
			break;
		}
		
		case 1:
		{
			Elevator_Service(ptr);
			break;
		}

		case 2:	// NTS Status
		{
			//		NTS Position Count
			itoa(ptr->NTS_poscnt, LCD_String);  // Put out current NTS position count
			i=0;
			while(((LCD_String[i] >= '0') && (LCD_String[i] <= '9')) || (LCD_String[i] == '-'))
			{
			 	LCD_Display[0][i+12] = LCD_String[i];
			 	i++;
			}
			while((i+12) <= 19)
			{
			 	LCD_Display[0][i+12] = ' ';
			 	i++;
			}

			// NTS velocity
			itoa(ptr->NTS_vel,LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
			  	LCD_Display[1][(i+6)] = LCD_String[i];
			  	i++;
			}
			while((i+6) < 10)
			{
			  	LCD_Display[1][(i+6)] = ' ';
			  	i++;
			}

			if (ptr->NTS_vel_dir == 1)
			  	LCD_Display[1][10] = up_arrow;
			else if (ptr->NTS_vel_dir == 2)
			  	LCD_Display[1][10] = dn_arrow;
			else
			  	LCD_Display[1][10] = ' ';
			
			LCD_Display[1][11] = 'f';
			LCD_Display[1][12] = 'p';
			LCD_Display[1][13] = 'm';
			
			if ((ptr->NTS_limits & 0x01) != 0)	// UN
				LCD_Display[2][2] = io_on;
			else
				LCD_Display[2][2] = io_off;
			
			if ((ptr->NTS_limits & 0x02) != 0)	// UT
				LCD_Display[2][6] = io_on;
			else
				LCD_Display[2][6] = io_off;
				
			if ((ptr->NTS_doorzone & 0x02) != 0)	// DZ
				LCD_Display[2][11] = io_on;
			else
				LCD_Display[2][11] = io_off;
			
			if ((ptr->NTS_status1 & 0x01) != 0)	// Hoistway learn
				LCD_Display[2][19] = io_on;
			else
				LCD_Display[2][19] = io_off;
			
			if ((ptr->NTS_limits & 0x10) != 0)	// DN
				LCD_Display[3][2] = io_on;
			else
				LCD_Display[3][2] = io_off;
				
			if ((ptr->NTS_limits & 0x20) != 0)	// DT
				LCD_Display[3][6] = io_on;
			else
				LCD_Display[3][6] = io_off;
				
			if ((ptr->NTS_status1 & 0x08) != 0)	// Online
				LCD_Display[3][11] = io_on;
			else
				LCD_Display[3][11] = io_off;
		
			if ((ptr->NTS_status1 & 0x04) != 0)	// CPF Clip Fault
				LCD_Display[3][19] = io_on;
			else
				LCD_Display[3][19] = io_off;
		
			break;
		}
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		{
			i = (Status_Dpy - 3) * 12;
			display_io_value(i,ptr);
			break;
		}
		case 11: // Check run, start, level and doors
		{
			//  "CkDrS =             ",	// 32 - 6, 17
			//  "CkStS =             ",	// 33 - 6, 17
			//  "CkRunS=             ",	// 34 - 6, 17
			//  "CkLevS=             ",	// 35 - 6, 17
  		    for(i=8; i<=19; i++)
		    {
				LCD_Display[0][i] = LCD_ckdoor_tbl[ptr->chkdoor_st][i-8];
		    }

  		    for(i=8; i<=19; i++)
		    {
				LCD_Display[1][i] = LCD_ckstart_tbl[ptr->chkstart_st][i-8];
		    }

  		    for(i=8; i<=19; i++)
		    {
				LCD_Display[2][i] = LCD_ckrun_tbl[ptr->chkrun_st][i-8];
		    }

  		    for(i=8; i<=19; i++)
		    {
				LCD_Display[3][i] = LCD_cklevel_tbl[ptr->chklevel_st][i-8];
		    }

			break;
		}
		case 12:
		{		   // debug
			sprintf(LCD_String,"%06xh",(int)debug1);
		    for(i=0; i<=6; i++)
		    {
			   	LCD_Display[0][10+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%06xh",(int)debug2);
		    for(i=0; i<=6; i++)
		    {
			   	LCD_Display[1][10+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%06xh",(int)debug3);
		    for(i=0; i<=6; i++)
		    {
			   	LCD_Display[2][10+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%06xh",(int)debug4);
		    for(i=0; i<=6; i++)
		    {
			   	LCD_Display[3][10+i] = LCD_String[i];
		    }
			break;
		}

		case 13:
		{		 // debug
			sprintf(LCD_String,"%06xh",(int)debug5);
		    for(i=0; i<=6; i++)
		    {
			   	LCD_Display[0][10+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%06xh",(int)debug6);
		    for(i=0; i<=6; i++)
		    {
			   	LCD_Display[1][10+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%06xh",(int)debug7);
		    for(i=0; i<=6; i++)
		    {
			   	LCD_Display[2][10+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%06xh",(int)debug8);
		    for(i=0; i<=6; i++)
		    {
			   	LCD_Display[3][10+i] = LCD_String[i];
		    }
			break;
		}
/*
		case 2: // LCD_Detail_Fault Menu Active Data 0
				//	"servf=   , procf=   "	// 0  - 6, 17
				//  "doorf=   .rdoorf=   ", // 1  - 6, 17
				//  "dpref=   ,  dirf=   ", // 2  - 6, 17
				//  " empf=   ,  medf=   ".	// 3  - 6, 17
  		{	   
			sprintf(LCD_String,"%2i",servf);
		  	LCD_Display[0][6] = LCD_String[0];
		  	LCD_Display[0][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",procf);
		  	LCD_Display[0][17] = LCD_String[0];
		  	LCD_Display[0][18] = LCD_String[1];

			sprintf(LCD_String,"%2i",doorf);
		  	LCD_Display[1][6] = LCD_String[0];
		  	LCD_Display[1][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",rdoorf);
		  	LCD_Display[1][17] = LCD_String[0];
		  	LCD_Display[1][18] = LCD_String[1];

			sprintf(LCD_String,"%2i",dpref);
		  	LCD_Display[2][6] = LCD_String[0];
		  	LCD_Display[2][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",dirf);
		  	LCD_Display[2][17] = LCD_String[0];
		  	LCD_Display[2][18] = LCD_String[1];
			
			sprintf(LCD_String,"%2i",empf);
		  	LCD_Display[3][6] = LCD_String[0];
		  	LCD_Display[3][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",medf);
		  	LCD_Display[3][17] = LCD_String[0];
		  	LCD_Display[3][18] = LCD_String[1];

			break;
		}

		case 3: // LCD_Detail_Fault Menu Active Data 1
				//  "codeb=   ,   eqf=   ",   // 4  - 6, 17
				//  "firef=   ,rfiref=   ",   // 5  - 6, 17
				//  "  hsf=   ,startf=   ",   // 6  - 6, 17
				//  "stepf=   ,estopf=   ",   // 7  - 6, 17
		{	
			sprintf(LCD_String,"%2i",codebf);
		  	LCD_Display[0][6] = LCD_String[0];
		  	LCD_Display[0][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",eqf);
		  	LCD_Display[0][17] = LCD_String[0];
		  	LCD_Display[0][18] = LCD_String[1];

			sprintf(LCD_String,"%2i",firef);
		  	LCD_Display[1][6] = LCD_String[0];
		  	LCD_Display[1][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",rfiref);
		  	LCD_Display[1][17] = LCD_String[0];
		  	LCD_Display[1][18] = LCD_String[1];

			sprintf(LCD_String,"%2i",hsf);
		  	LCD_Display[2][6] = LCD_String[0];
		  	LCD_Display[2][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",startf);
		  	LCD_Display[2][17] = LCD_String[0];
		  	LCD_Display[2][18] = LCD_String[1];

			sprintf(LCD_String,"%2i",stepf);
		  	LCD_Display[3][6] = LCD_String[0];
		  	LCD_Display[3][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",estop);
		  	LCD_Display[3][17] = LCD_String[0];
		  	LCD_Display[3][18] = LCD_String[1];

			break;
		}
		
		case 4: // LCD_Detail_Fault Menu Active Data 2
				//  "callf=   ,dcalls=   ",   // 8  - 6, 17
				//  "nstop=   ,nxstop=   ",   // 9  - 6, 17
				//  "Relev=   ,LevStr=   ",   // 10 - 6, 17
				//  "NCUds=   ,DOseqf=   ",   // 11 - 6, 17
		{	
			sprintf(LCD_String,"%02x",Call_Flags);
		  	LCD_Display[0][6] = LCD_String[0];
		  	LCD_Display[0][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",dcalls);
		  	LCD_Display[0][17] = LCD_String[0];
		  	LCD_Display[0][18] = LCD_String[1];

			sprintf(LCD_String,"%2i",nstopf);
		  	LCD_Display[1][6] = LCD_String[0];
		  	LCD_Display[1][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",stops[0]);
		  	LCD_Display[1][17] = LCD_String[0];
		  	LCD_Display[1][18] = LCD_String[1];

			sprintf(LCD_String,"%2i",relevel);
		  	LCD_Display[2][6] = LCD_String[0];
		  	LCD_Display[2][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",lev_startf);
		  	LCD_Display[2][17] = LCD_String[0];
		  	LCD_Display[2][18] = LCD_String[1];

			sprintf(LCD_String,"%2i",ncu_door_seq);
		  	LCD_Display[3][6] = LCD_String[0];
		  	LCD_Display[3][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",Door_Seq_Flags);
		  	LCD_Display[3][17] = LCD_String[0];
		  	LCD_Display[3][18] = LCD_String[1];
			break;
		}
		
		case 5: // LCD_Detail_Fault Menu Active Data 3
				//  "NudSt=   ,DoorRq=   ",   // 12 - 6, 17
				//  "FSd=     ,RSd=      ",   // 13 - 4, 14
				//  "InsSt=   ,InsSvc=   ",   // 14 - 6, 17
				//  "Motion Tmr =        ",   // 15 - 13
		{	
			sprintf(LCD_String,"%02x",Nudgst_Flags);
		  	LCD_Display[0][6] = LCD_String[0];
		  	LCD_Display[0][7] = LCD_String[1];

			sprintf(LCD_String,"%02x",Door_Req_Flags);
		  	LCD_Display[0][17] = LCD_String[0];
		  	LCD_Display[0][18] = LCD_String[1];

			sprintf(LCD_String,"%04x",front_slowdown);
		  	LCD_Display[1][4] = LCD_String[0];
		  	LCD_Display[1][5] = LCD_String[1];
		  	LCD_Display[1][6] = LCD_String[2];
		  	LCD_Display[1][7] = LCD_String[3];

			sprintf(LCD_String,"%04x",rear_slowdown);
		  	LCD_Display[1][14] = LCD_String[0];
		  	LCD_Display[1][15] = LCD_String[1];
		  	LCD_Display[1][16] = LCD_String[2];
		  	LCD_Display[1][17] = LCD_String[3];

			sprintf(LCD_String,"%2x",ins_status);
		  	LCD_Display[2][6] = LCD_String[0];
		  	LCD_Display[2][7] = LCD_String[1];

			sprintf(LCD_String,"%2i",ins_servf);
		  	LCD_Display[2][17] = LCD_String[0];
		  	LCD_Display[2][18] = LCD_String[1];

			sprintf(LCD_String,"%5i",timers[tmotion]);
		  	LCD_Display[3][13] = LCD_String[0];
		  	LCD_Display[3][14] = LCD_String[1];
   		  	LCD_Display[3][15] = LCD_String[2];
		  	LCD_Display[3][16] = LCD_String[3];
		  	LCD_Display[3][17] = LCD_String[4];
			break;
		}
		
		case 6: // statusf and statusf2, SS Status, Pwr Status and Run Status    
				//  " statusf =          ",   // 16 - 11
				//  "statusf2 =          ",   // 17 - 11
				//  "Run Stat =          ",   // 18 - 11
				//  "ssSt=    ,pwrSt=    ",   // 19 - 5, 16
		{	 
			sprintf(LCD_String,"%08xh",(int32)statusf);
		    for(i=0; i<=8; i++)
		    {
			   	LCD_Display[0][11+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%08xh",(int32)statusf2);
		    for(i=0; i<=8; i++)
		    {
			   	LCD_Display[1][11+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%08xh",(int32)run_statusf);
		    for(i=0; i<=8; i++)
		    {
			   	LCD_Display[2][11+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%04x",(int16)safety_string_status);
		    for(i=0; i<4; i++)
		    {
			   	LCD_Display[3][5+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%04x",(int16)power_status);
		    for(i=0; i<4; i++)
		    {
			   	LCD_Display[3][16+i] = LCD_String[i];
		    }
			break;
		}
		case 7:
		{	 // statusf3 and statusf4
			sprintf(LCD_String,"%08xh",(int32)statusf3);
		    for(i=0; i<=8; i++)
		    {
			   	LCD_Display[0][13+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%08xh",(int32)statusf4);
		    for(i=0; i<=8; i++)
		    {
			   	LCD_Display[1][13+i] = LCD_String[i];
		    }
			break;
		}

		
		case 10: // SS Status, Pwr Status and Run Status    run_statusf
		{	 


			break;
		}
		case 11: // LCD_Detail_Fault Menu Active Data 4
		{
				//  "FltB1=   . FltB2=   ", // 30 - 6, 17
				//  "FltB3=   . FltB4=   ", // 31 - 6, 17
				//  "CkDrS=   ,CkRunS=   ",	// 34 - 6, 17
				//  "CkStS=   ,CkLevS=   ",	// 35 - 6, 17
			sprintf(LCD_String,"%02x",Fault_Bits[0]);
		  	LCD_Display[0][6] = LCD_String[0];
		  	LCD_Display[0][7] = LCD_String[1];
		  	LCD_Display[0][8] = 'h';

			sprintf(LCD_String,"%02x",Fault_Bits[1]);
		  	LCD_Display[0][17] = LCD_String[0];
		  	LCD_Display[0][18] = LCD_String[1];
		  	LCD_Display[0][19] = 'h';
		  	
			sprintf(LCD_String,"%02x",Fault_Bits[2]);
		  	LCD_Display[1][6] = LCD_String[0];
		  	LCD_Display[1][7] = LCD_String[1];
		  	LCD_Display[1][8] = 'h';

			sprintf(LCD_String,"%02x",Fault_Bits[3]);
		  	LCD_Display[1][17] = LCD_String[0];
		  	LCD_Display[1][18] = LCD_String[1];
		  	LCD_Display[1][19] = 'h';
		  	
			sprintf(LCD_String,"%02x",chkdoor_state);
		  	LCD_Display[2][6] = LCD_String[0];
		  	LCD_Display[2][7] = LCD_String[1];

			sprintf(LCD_String,"%02x",chkrun_state);
		  	LCD_Display[2][17] = LCD_String[0];
		  	LCD_Display[2][18] = LCD_String[1];

			sprintf(LCD_String,"%02x",chkstart_state);
		  	LCD_Display[3][6] = LCD_String[0];
		  	LCD_Display[3][7] = LCD_String[1];

			sprintf(LCD_String,"%02x",chklevel_state);
		  	LCD_Display[3][17] = LCD_String[0];
		  	LCD_Display[3][18] = LCD_String[1];

			break;
		}

		case 12:	// Run Status
		{
				//  "RUN= ,HS= ,SU= ,SD= ", // 4,9,14,19
				//  "DZA= ,UL= ,DZ= ,DL= ", // 4,9,14,19
				//  "DOL= ,TL= ,ML= ,BL= ", // 4,9,14,19
				//  "DCL= ,DC= ,DO= ,GS= ",	// 4,9,14,19
				
				
				//  "RUN= ,HS= ,SU= ,SD= ", // 4,9,14,19
			if ((run_statusf & rs_RUN) != 0)
				LCD_Display[0][4] = '1';
			else
				LCD_Display[0][4] = '0';
			
			if ((run_statusf & rs_HSF) != 0)
				LCD_Display[0][9] = '1';
			else
				LCD_Display[0][9] = '0';
			
			if ((run_statusf & rs_UP) != 0)
				LCD_Display[0][14] = '1';
			else
				LCD_Display[0][14] = '0';
			
			if ((run_statusf & rs_DNR) != 0)
				LCD_Display[0][19] = '1';
			else
				LCD_Display[0][19] = '0';
			
				//  "DZA= ,UL= ,DZ= ,DL= ", // 4,9,14,19
			if ((run_statusf & rs_DZA) != 0)
				LCD_Display[1][4] = '1';
			else
				LCD_Display[1][4] = '0';
			
			if ((run_statusf & rs_UL) != 0)
				LCD_Display[1][9] = '1';
			else
				LCD_Display[1][9] = '0';
			
			if ((run_statusf & rs_DZ) != 0)
				LCD_Display[1][14] = '1';
			else
				LCD_Display[1][14] = '0';
			
			if ((run_statusf & rs_DL) != 0)
				LCD_Display[1][19] = '1';
			else
				LCD_Display[1][19] = '0';
			
				//  "DOL= ,TL= ,ML= ,BL= ", // 4,9,14,19
			if ((run_statusf & rs_DCL) != 0)
				LCD_Display[2][4] = '1';
			else
				LCD_Display[2][4] = '0';
			
			if ((run_statusf & rs_DLT) != 0)
				LCD_Display[2][9] = '1';
			else
				LCD_Display[2][9] = '0';
			
			if ((run_statusf & rs_DLM) != 0)
				LCD_Display[2][14] = '1';
			else
				LCD_Display[2][14] = '0';
			
			if ((run_statusf & rs_DLB) != 0)
				LCD_Display[2][19] = '1';
			else
				LCD_Display[2][19] = '0';
				
			
				//  "DCL= ,DC= ,DO= ,GS= ",	// 4,9,14,19
			if ((run_statusf & rs_DCL) != 0)
				LCD_Display[3][4] = '1';
			else
				LCD_Display[3][4] = '0';
			
			if ((run_statusf & rs_DC) != 0)
				LCD_Display[3][9] = '1';
			else
				LCD_Display[3][9] = '0';
			
			if ((run_statusf & rs_DO) != 0)
				LCD_Display[3][14] = '1';
			else
				LCD_Display[3][14] = '0';
			
			if ((run_statusf & rs_GS) != 0)
				LCD_Display[3][19] = '1';
			else
				LCD_Display[3][19] = '0';
			break;
		}
		
		case 13:	// Run Status with rear door
		{
				//  "RUN= ,HS= ,SU= ,SD= ", // 4,9,14,19
				//  "DZA= ,UL= ,DZ= ,DL= ", // 4,9,14,19
				//  "DOLR= ,RLM= ,RGS=   ", // 5,11,17
				//  "DCLR= ,DCR= ,DOR=   ", // 5,11,17
				
				
				//  "RUN= ,HS= ,SU= ,SD= ", // 4,9,14,19
			if ((run_statusf & rs_RUN) != 0)
				LCD_Display[0][4] = '1';
			else
				LCD_Display[0][4] = '0';
			
			if ((run_statusf & rs_HSF) != 0)
				LCD_Display[0][9] = '1';
			else
				LCD_Display[0][9] = '0';
			
			if ((run_statusf & rs_UP) != 0)
				LCD_Display[0][14] = '1';
			else
				LCD_Display[0][14] = '0';
			
			if ((run_statusf & rs_DNR) != 0)
				LCD_Display[0][19] = '1';
			else
				LCD_Display[0][19] = '0';
			
				//  "DZA= ,UL= ,DZ= ,DL= ", // 4,9,14,19
			if ((run_statusf & rs_DZA) != 0)
				LCD_Display[1][4] = '1';
			else
				LCD_Display[1][4] = '0';
			
			if ((run_statusf & rs_UL) != 0)
				LCD_Display[1][9] = '1';
			else
				LCD_Display[1][9] = '0';
			
			if ((run_statusf & rs_DZ) != 0)
				LCD_Display[1][14] = '1';
			else
				LCD_Display[1][14] = '0';
			
			if ((run_statusf & rs_DL) != 0)
				LCD_Display[1][19] = '1';
			else
				LCD_Display[1][19] = '0';
			
				//  "DOLR= ,RLM= ,RGS=   ", // 5,11,17
			if ((run_statusf & rs_DCLR) != 0)
				LCD_Display[2][5] = '1';
			else
				LCD_Display[2][5] = '0';
			
			if ((run_statusf & rs_RLM) != 0)
				LCD_Display[2][11] = '1';
			else
				LCD_Display[2][11] = '0';
			
			if ((run_statusf & rs_RGS) != 0)
				LCD_Display[2][17] = '1';
			else
				LCD_Display[2][17] = '0';
			
			
				//  "DCLR= ,DCR= ,DOR=   ", // 5,11,17
			if ((run_statusf & rs_DCLR) != 0)
				LCD_Display[3][4] = '1';
			else
				LCD_Display[3][4] = '0';
			
			if ((run_statusf & rs_DCR) != 0)
				LCD_Display[3][9] = '1';
			else
				LCD_Display[3][9] = '0';
			
			if ((run_statusf & rs_DOR) != 0)
				LCD_Display[3][14] = '1';
			else
				LCD_Display[3][14] = '0';
			
			break;
		}

		case 14:
		{			   // fire status
			sprintf(LCD_String,"%2i",fs1_effect);
		  	LCD_Display[0][5] = LCD_String[0];
		  	LCD_Display[0][6] = LCD_String[1];

			sprintf(LCD_String,"%2i",fire_reset);
		  	LCD_Display[0][13] = LCD_String[0];
		  	LCD_Display[0][14] = LCD_String[1];

			sprintf(LCD_String,"%2x",car_Grp_Stat.c_grp_fire_ctrl);
		  	LCD_Display[0][21] = LCD_String[0];
		  	LCD_Display[0][22] = LCD_String[1];
		  	LCD_Display[0][23] = 'h';

			sprintf(LCD_String,"%2i",fs2_to_fs1);
		  	LCD_Display[1][5] = LCD_String[0];
		  	LCD_Display[1][6] = LCD_String[1];

			sprintf(LCD_String,"%2i",fire_sensor);
		  	LCD_Display[1][13] = LCD_String[0];
		  	LCD_Display[1][14] = LCD_String[1];

			sprintf(LCD_String,"%2x",fire_status);
		  	LCD_Display[1][21] = LCD_String[0];
		  	LCD_Display[1][22] = LCD_String[1];
		  	LCD_Display[1][23] = 'h';

			break;
		}
		
		case 15:
		{		  // LCD_Run Timer Display
#if (Traction == 1)

			// Pre-torque time
			for (i=0;i<=7;i++)
				LCD_String[i] = ' ';

			if ((procf >= 14) && (procf <= 16))
				i = pre_torque_time;
			else
				i = 0;
			sprintf(LCD_String, "%2d.%-1d", i/10, i%10);
		  	LCD_Display[0][4] = LCD_String[0];
		  	LCD_Display[0][5] = LCD_String[1];
		  	LCD_Display[0][6] = LCD_String[2];
		  	LCD_Display[0][7] = LCD_String[3];
		  	
			// Run time from start of pattern
			for (i=0;i<=7;i++)
				LCD_String[i] = ' ';

			if ((run_time > pre_torque_time) && (procf >= 14) && (procf <= 16))
				i = run_time - pre_torque_time;
			else
				i = 0;
			sprintf(LCD_String, "%3d.%-1d", i/10, i%10);
		  	LCD_Display[0][13] = LCD_String[0];
		  	LCD_Display[0][14] = LCD_String[1];
		  	LCD_Display[0][15] = LCD_String[2];
		  	LCD_Display[0][16] = LCD_String[3];
		  	LCD_Display[0][17] = LCD_String[4];

			if (short_fl_run == 1)
				LCD_Display[0][23] = '1';
			else
				LCD_Display[0][23] = '0';
			
			// Pre-open door time
			for (i=0;i<=7;i++)
				LCD_String[i] = ' ';

			if ((run_time >= preopen_time) && (preopen_time != 0) && (procf >= 14) && (procf <= 16))
				i = run_time - preopen_time;
			else
				i = 0;
			sprintf(LCD_String, "%2d.%-1d", i/10, i%10);
		  	LCD_Display[1][4] = LCD_String[0];
		  	LCD_Display[1][5] = LCD_String[1];
		  	LCD_Display[1][6] = LCD_String[2];
		  	LCD_Display[1][7] = LCD_String[3];

			for (i=0;i<=7;i++)
				LCD_String[i] = ' ';

			// Leveling time
			if ((run_time >= targetfl_time) && (procf >= 14) && (procf <= 16))
				i = run_time - targetfl_time;
			else
				i = 0;
			sprintf(LCD_String, "%2d.%-1d", i/10, i%10);
		  	LCD_Display[1][13] = LCD_String[0];
		  	LCD_Display[1][14] = LCD_String[1];
		  	LCD_Display[1][15] = LCD_String[2];
		  	LCD_Display[1][16] = LCD_String[3];

			if (Roll_early_exit == 1)
				LCD_Display[1][23] = '1';
			else
				LCD_Display[1][23] = '0';

#endif			
			break;
		}

		case 18:			   // NTS Status
		{
			NTS_can.cmd_req = 1;
			itoa(NTS_can.velocity,LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
			  	LCD_Display[0][(i+8)] = LCD_String[i];
			  	i++;
			}
			while((i+8) < 12)
			{
			  	LCD_Display[0][(i+8)] = ' ';
			  	i++;
			}
			
			if (NTS_can.vel_dir == 1)
				LCD_Display[0][12] = 'U';
			else if (NTS_can.vel_dir == 2)
				LCD_Display[0][12] = 'D';
			else
				LCD_Display[0][12] = ' ';
						
			j = 0;
			for (i=18;i<=19;i++)
				LCD_Display[0][i] = nts_service[NTS_can.service][j++];

			if (((NTS_can.limit_flt[0] & 0xf5) != 0) || ((NTS_can.limit_flt[1] & 0x0f) != 0))
				j = 9;
			else if (((NTS_can.limit_flt[1] & 0x50) != 0) || ((NTS_can.limit_flt[2] & 0x7f) != 0))
				j = 8;
			else if ((NTS_can.status & 0x40) != 0)
				j = 7;						// emsd 
			else if ((NTS_can.status & 0x20) != 0)
				j = 6;						// vel fault
			else if ((NTS_can.status & 0x80) != 0)
				j = 5;						// vel dir fault
			else if ((NTS_can.status & 0x04) == 0)
				j = 4;						// sel comm fault
			else if ((NTS_can.status & 0x08) == 0)
				j = 3;						// spb comm fault
			else if ((NTS_can.status & 0x02) != 0)
				j = 2;						// running dn
			else if ((NTS_can.status & 0x01) != 0)
				j = 1;						// running up
			else 
				j = 0;						// run enable
				
			for (i=0;i<17;i++)
				LCD_Display[1][7+i] = nts_lim_stat[j][i];
			
			break;
		}
	*/
		default:
			break;
	}
}

char getcaps (char c)
{
	if ((c >= 'a') && (c <= 'z'))
		c &= ~0x20;
	return(c);
}


/*
void Service_Timers (void)
{

	int16 i;
	int16 max;
	static int16 prev_flash;
	static int16 md_ix;
	static int16 dt_ix;
	static int16 dow_ix;
	static int16 tim_ix;
	static int16 modify_var;
	static int16 date_time_ix;
	static int16 hr_min_ix;
	int16 date_time_max[6] = {12,31,23,59,23,59};
	int16 date_time_min[6] = { 0, 0, 0, 0, 0, 0};
	int16 dow_time_max[4] = {23,59,23,59};
	int16 dow_time_min[4] = {0, 0, 0, 0};
	int16 digit_loc[6] = {14,17,14,17,14,17};
						//     Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
	int16 days_of_month[13] = {0, 31,  29,  31,  30,  31,  30,  31,  30,  31,  31,  30,  31}; 


 	if (Menu_level == 1)
 	{			// Rotate through Call menu items
 		if (LCD_Pointer == 0)
 		{
 			upd_dpy = 1;
 			LCD_Pointer++; 
 		    for(i=0; i<=19; i++)
 			  	LCD_Display[0][i] = LCD_Timer_Menu[0][i];
 			LCD_Display[0][19] = (char)(Timer_Number + '0'); 
 	  	}
 		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
 		{	   // Exit to main menu
 			LCD_Mode_PB = 1;  //
			Menu_level = 0;
			LCD_Pointer = Timer_Number + 11;
			for(i=0; i<=19; i++)
			{
				LCD_Display[0][i] = LCD_Main_Menu[10][i];
			  	LCD_Display[1][i] = LCD_Timer_Menu[0][i];
			}
			LCD_Display[1][19] = (char)((LCD_Pointer - 11) + '0'); 
			return;
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    upd_dpy = 1;
		    LCD_Pointer++;
		    if(LCD_Pointer > 7)
			  LCD_Pointer = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    upd_dpy = 1;
		    LCD_Pointer--;
		    if(LCD_Pointer < 1)
			  LCD_Pointer = 7;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
		  	Menu_level = (LCD_Pointer + 1);
			LCD_Pointer = 0;
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
		    for(i=0; i<=19; i++)
			  	LCD_Display[1][i] = LCD_Timer_Menu[LCD_Pointer][i];
		}
	}

	if (Menu_level == 2)
	{				   // Set the Month/Day Timers
 		if (LCD_Pointer == 0)
 		{
 			upd_dpy = 1;
 			LCD_Pointer++; 
			line_select = 0;
			date_time_ix = 0;
			LCD_Dig_Point = 0;
			modify_var = 0;
			md_ix = 0;
			dt_ix = 0;
 		    for(i=0; i<=19; i++)
 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
			get_month_day_timer(Timer_Number,md_ix);
 	  	}
 		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
 		{	   // Exit to main menu
 			LCD_Mode_PB = 1;  //
			if (modify_var == 1)
			{
				upd_dpy = 1;
				if (LCD_Dig_Point == 1)
				{			   // back to first digit
					LCD_Dig_Point = 0;
					date_time_ix ^= 0x01;  // swap byte for date or time
				}
				else
				{			   // second digit
		  			LCD_Dig_Point = 1;	
		  		}		
			}
			else if (line_select > 0)
			{
			 	line_select --;
				if (line_select == 0)
				{
					upd_dpy = 1;
		 		    for(i=0; i<=19; i++)
		 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
					get_month_day_timer(Timer_Number,md_ix);
					dt_ix = 0;
					date_time_ix = 0;
				}
			}
			else
			{
				line_select = 0;
				LCD_Pointer = Menu_level-1;
				Menu_level = 1;
				for(i=0; i<=19; i++)
				{
					LCD_Display[0][i] = LCD_Timer_Menu[0][i];
				  	LCD_Display[1][i] = LCD_Timer_Menu[LCD_Pointer][i];
				}
				LCD_Display[0][19] = (char)(Timer_Number + '0'); 
			}
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    upd_dpy = 1;
			if (line_select == 0)
			{						// next month/day timer
				md_ix++;
				if (md_ix > 2)
					md_ix = 0;
				get_month_day_timer(Timer_Number,md_ix);
			}
			else if (line_select == 1)
			{						// next date/time setting
				dt_ix++;
				if (dt_ix > 2)
					dt_ix = 0;
				date_time_ix = dt_ix * 2;
			}
			else if (modify_var == 1)
			{						 // Adjust the time or date
				if (date_time_ix == 1)
					max = days_of_month[date_time_var[c_month]];
				else
					max = date_time_max[date_time_ix];

		  		adjust_variable(&date_time_var[date_time_ix],max,date_time_min[date_time_ix],1,LCD_Dig_Point);
			}
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    upd_dpy = 1;
			if (line_select == 0)
			{						// next month/day timer
				md_ix--;
				if (md_ix < 0)
					md_ix = 2;
				get_month_day_timer(Timer_Number,md_ix);
			}
			else if (line_select == 1)
			{						   // next date/time setting
				dt_ix--;
				if (dt_ix < 0)
					dt_ix = 2;
				date_time_ix = dt_ix * 2;
			}
			else if (modify_var == 1)
			{							// Adjust the time or date
				if (date_time_ix == 1)
					max = days_of_month[date_time_var[c_month]];
				else
					max = date_time_max[date_time_ix];

		  		adjust_variable(&date_time_var[date_time_ix],max,date_time_min[date_time_ix],0,LCD_Dig_Point);
			}
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
			if (line_select == 0)
			{						// Select which month/day timer
				line_select = 1;
	 		    for(i=0; i<=19; i++)
	 			  	LCD_Display[0][i] = LCD_Month_Day_Timer_Menu[0][i];
				LCD_Display[0][23] = (char)(md_ix + '0'); 
			}
			else if (line_select == 1)
			{						// enter modify mode
				modify_var = 1;
				line_select = 2;
				LCD_Dig_Point = 0;
				date_time_ix = dt_ix * 2;
			}
			else if (line_select == 2)
			{						   // Update the new timer value
				modify_var = 0;
				line_select = 1;
				set_month_day_timer(Timer_Number,md_ix);
				Wrt_Grp_Svc_Timer(Timer_Number);
			}
		}

		if ((line_select == 2) && (prev_flash != LCD_Flash))
			upd_dpy = 1;
		prev_flash = LCD_Flash;

		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			if (line_select == 0)
			{
	 		    for(i=0; i<=19; i++)
	 			  	LCD_Display[1][i] = LCD_Month_Day_Timer_Menu[0][i];
				LCD_Display[1][23] = (char)(md_ix + '0'); 
			}
			else 
			{
	 		    for(i=0; i<=19; i++)
	 			  	LCD_Display[1][i] = LCD_Month_Day_Timer_Menu[dt_ix + 1][i];

				sprintf(&LCD_Display[1][digit_loc[(date_time_ix & 0xFFFE)]],"%02i",date_time_var[(date_time_ix & 0xFFFE)]);
				sprintf(&LCD_Display[1][digit_loc[(date_time_ix & 0xFFFE)+1]],"%02i",date_time_var[(date_time_ix & 0xFFFE)+1]);
				if (dt_ix == 0)
					LCD_Display[1][16] = '/';
				else
					LCD_Display[1][16] = ':';

			  	LCD_Flash_TMP[0] = LCD_Display[1][(digit_loc[date_time_ix]+1)-LCD_Dig_Point];
	 		}
			flash_digit(LCD_Flash_TMP[0], 1, (digit_loc[date_time_ix]+1)-LCD_Dig_Point, (modify_var == 1));
		}
	}
	if (Menu_level == 3)
	{			// Set the Day of the Week timers
 		if (LCD_Pointer == 0)
 		{
 			upd_dpy = 1;
 			LCD_Pointer++; 
			line_select = 0;
			hr_min_ix = 0;
			LCD_Dig_Point = 0;
			modify_var = 0;
			dow_ix = 0;
			tim_ix = 0;
 		    for(i=0; i<=19; i++)
 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
			get_dow_timer(Timer_Number,dow_ix);
 	  	}
 		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
 		{	   // Exit to main menu
 			LCD_Mode_PB = 1;  //
			if (modify_var == 1)
			{
				upd_dpy = 1;
				if (LCD_Dig_Point == 1)
				{			   // back to first digit
					LCD_Dig_Point = 0;
					hr_min_ix ^= 0x01;  // swap byte for date or time
				}
				else
				{			   // second digit
		  			LCD_Dig_Point = 1;	
		  		}		
			}
			else if (line_select > 0)
			{
			 	line_select --;
				if (line_select == 0)
				{
					upd_dpy = 1;
		 		    for(i=0; i<=19; i++)
		 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
					get_dow_timer(Timer_Number,dow_ix);
					tim_ix = 0;
					hr_min_ix = 0;
				}
			}
			else
			{
				line_select = 0;
				LCD_Pointer = Menu_level-1;
				Menu_level = 1;
				for(i=0; i<=19; i++)
				{
					LCD_Display[0][i] = LCD_Timer_Menu[0][i];
				  	LCD_Display[1][i] = LCD_Timer_Menu[LCD_Pointer][i];
				}
				LCD_Display[0][19] = (char)Timer_Number + '0'; 
			}
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    upd_dpy = 1;
			if (line_select == 0)
			{						// daily timer
				dow_ix++;
				if (dow_ix > 6)
					dow_ix = 0;
				get_dow_timer(Timer_Number,dow_ix);
			}
			else if (line_select == 1)
			{						// next time setting
				tim_ix++;
				if (tim_ix > 1)
					tim_ix = 0;
				hr_min_ix = tim_ix * 2;
			}
			else if (modify_var == 1)
			{						 // Adjust the time or date
		  		adjust_variable(&dow_time_var[hr_min_ix],dow_time_max[hr_min_ix],dow_time_min[hr_min_ix],1,LCD_Dig_Point);
			}
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    upd_dpy = 1;
			if (line_select == 0)
			{						// next month/day timer
				dow_ix--;
				if (dow_ix < 0)
					dow_ix = 6;
				get_dow_timer(Timer_Number,dow_ix);
			}
			else if (line_select == 1)
			{						   // next date/time setting
				tim_ix--;
				if (tim_ix < 0)
					tim_ix = 1;
				hr_min_ix = tim_ix * 2;
			}
			else if (modify_var == 1)
			{							// Adjust the time or date
		  		adjust_variable(&dow_time_var[hr_min_ix],dow_time_max[hr_min_ix],dow_time_min[hr_min_ix],0,LCD_Dig_Point);
			}
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
			if (line_select == 0)
			{						// Select which month/day timer
				line_select = 1;
	 		    for(i=0; i<=19; i++)
	 			  	LCD_Display[0][i] = LCD_DOW_Timer_Menu[dow_ix][i];
			}
			else if (line_select == 1)
			{						// enter modify mode
				modify_var = 1;
				line_select = 2;
				LCD_Dig_Point = 0;
				hr_min_ix = tim_ix * 2;
			}
			else if (line_select == 2)
			{						   // Update the new timer value
				modify_var = 0;
				line_select = 1;
				set_dow_timer(Timer_Number,dow_ix);
				Wrt_Grp_Svc_Timer(Timer_Number);
			}
		}

		if ((line_select == 2) && (prev_flash != LCD_Flash))
			upd_dpy = 1;
		prev_flash = LCD_Flash;

		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			if (line_select == 0)
			{
	 		    for(i=0; i<=19; i++)
	 			  	LCD_Display[1][i] = LCD_DOW_Timer_Menu[dow_ix][i];
			}
			else 
			{
	 		    for(i=0; i<=19; i++)
	 			  	LCD_Display[1][i] = LCD_DOW_Timer_Menu[tim_ix + 7][i];

				sprintf(&LCD_Display[1][digit_loc[(hr_min_ix & 0xFFFE)]],"%02i",dow_time_var[(hr_min_ix & 0xFFFE)]);
				sprintf(&LCD_Display[1][digit_loc[(hr_min_ix & 0xFFFE)+1]],"%02i",dow_time_var[(hr_min_ix & 0xFFFE)+1]);
				LCD_Display[1][16] = ':';

			  	LCD_Flash_TMP[0] = LCD_Display[1][(digit_loc[hr_min_ix]+1)-LCD_Dig_Point];
	 		}
			flash_digit(LCD_Flash_TMP[0], 1, (digit_loc[hr_min_ix]+1)-LCD_Dig_Point, (modify_var == 1));
		}
	}
	if (Menu_level == 4)
	{					// Set the timer service
 		if (LCD_Pointer == 0)
 		{
 			upd_dpy = 1;
 			LCD_Pointer++; 
			status_select = 0;
			Timer_Service = svc_t[Timer_Number].service;
 		    for(i=0; i<=19; i++)
 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
 	  	}
 		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
 		{	   // Exit to main menu
 			LCD_Mode_PB = 1;  //
			status_select = 0;
			LCD_Pointer = Menu_level-1;
			Menu_level = 1;
			for(i=0; i<=19; i++)
			{
				LCD_Display[0][i] = LCD_Timer_Menu[0][i];
			  	LCD_Display[1][i] = LCD_Timer_Menu[LCD_Pointer][i];
			}
			LCD_Display[0][19] = (char)Timer_Number + '0'; 
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    upd_dpy = 1;
			if (status_select == 1)
			{
				Timer_Service++;
				if (Timer_Service > s_last)
					Timer_Service = s_none;
			}
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    upd_dpy = 1;
			if (status_select == 1)
			{
				Timer_Service--;
				if (Timer_Service < s_none)
					Timer_Service = s_last;
			}
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
			if (status_select == 1)
			{
				status_select = 0;
				svc_t[Timer_Number].service = Timer_Service;
				Wrt_Grp_Svc_Timer(Timer_Number);
			}
			else
				status_select = 1;
		}

		if ((status_select == 1) && (prev_flash != LCD_Flash))
			upd_dpy = 1;
		prev_flash = LCD_Flash;

		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			if ((status_select == 1) && (LCD_Flash == 0))
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = ' ';
			}
			else
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = LCD_Timer_Service_Menu[(uint8)Timer_Service][i];
			}
		}
	}
	if (Menu_level == 5)
	{			// Timer Activation Logic menu
		if (LCD_Pointer == 0)
 		{
 			upd_dpy = 1;
 			LCD_Pointer++; 
			status_select = 0;
			Timer_Logic = svc_t[Timer_Number].logic;
 		    for(i=0; i<=19; i++)	// printing Timer Type on the top line
 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
 	  	}
		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
 		{	   // Exit to main menu
 			LCD_Mode_PB = 1;  //
			LCD_Pointer = Menu_level-1;
			Menu_level = 1;
			for(i=0; i<=19; i++)
			{
				LCD_Display[0][i] = LCD_Timer_Menu[0][i];
			  	LCD_Display[1][i] = LCD_Timer_Menu[LCD_Pointer][i];
			}
			LCD_Display[0][19] = (char)Timer_Number + '0'; 
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    upd_dpy = 1;
			if (status_select == 1)
			{
				Timer_Logic++;
				if (Timer_Logic > 3)
					Timer_Logic = 0;
			}
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    upd_dpy = 1;
			if (status_select == 1)
			{
				Timer_Logic--;
				if (Timer_Logic < 0)
					Timer_Logic = 3;
			}
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
			if (status_select == 1)
			{
				status_select = 0;
				svc_t[Timer_Number].logic = Timer_Logic;
				Wrt_Grp_Svc_Timer(Timer_Number);
			}
			else
				status_select = 1;
		}

		if ((status_select == 1) && (prev_flash != LCD_Flash))
			upd_dpy = 1;
		prev_flash = LCD_Flash;

		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			if ((status_select == 1) && (LCD_Flash == 0))
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = ' ';
			}
			else
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = LCD_Timer_Logic[(uint8)Timer_Logic][i];
			}
		}
	}
	if (Menu_level == 6)
	{			// Set the timer enable
 		if (LCD_Pointer == 0)
 		{
 			upd_dpy = 1;
 			LCD_Pointer++;
 			status_select = 0; 
			Timer_Status = svc_t[Timer_Number].status;
 		    for(i=0; i<=19; i++)
 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
 	  	}
 		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
 		{	   // Exit to main menu
 			LCD_Mode_PB = 1;  //
			status_select = 0;
			LCD_Pointer = Menu_level-1;
			Menu_level = 1;
			for(i=0; i<=19; i++)
			{
				LCD_Display[0][i] = LCD_Timer_Menu[0][i];
			  	LCD_Display[1][i] = LCD_Timer_Menu[LCD_Pointer][i];
			}
			LCD_Display[0][19] = (char)Timer_Number + '0'; 
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    upd_dpy = 1;
			if (status_select == 1)
				Timer_Status ^= 0x02;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    upd_dpy = 1;
			if (status_select == 1)
				Timer_Status ^= 0x02;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
			if (status_select == 1)
			{
				status_select = 0;
				svc_t[Timer_Number].status = (svc_t[Timer_Number].status & 0xFD) | (Timer_Status & 0x02);
				Wrt_Grp_Svc_Timer(Timer_Number);
			}
			else
				status_select = 1;
		}

		if ((status_select == 1) && (prev_flash != LCD_Flash))
			upd_dpy = 1;
		prev_flash = LCD_Flash;

		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			Timer_Status = (Timer_Status & 0xFE) | (svc_t[Timer_Number].status & 0x01);
			if ((status_select == 1) && (LCD_Flash == 0))
			{
			    for(i=12; i<=19; i++)
				  	LCD_Display[1][i] = ' ';
			}
			else
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = LCD_Timer_Status_Menu[Timer_Status & 0x03][i];
			}
		}
	}
	if (Menu_level == 7)
	{					// Copy Day of Week Timers
 		if (LCD_Pointer == 0)
 		{
 			upd_dpy = 1;
 			LCD_Pointer++; 
			status_select = 0;
 		    for(i=0; i<=19; i++)
 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
 	  	}
 		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
 		{	   // Exit to main menu
 			LCD_Mode_PB = 1;  //
			LCD_Pointer = Menu_level-1;
			Menu_level = 1;
			for(i=0; i<=19; i++)
			{
				LCD_Display[0][i] = LCD_Timer_Menu[0][i];
			  	LCD_Display[1][i] = LCD_Timer_Menu[LCD_Pointer][i];
			}
			LCD_Display[0][19] = (char)Timer_Number + '0'; 
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
			if (status_select == 0)
			{
				LCD_Pointer++;
				if (LCD_Pointer > 2)
					LCD_Pointer = 1;
			}
			else
				status_select = 0;
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
			if (status_select == 0)
			{
				LCD_Pointer --;
				if (LCD_Pointer < 1)
					LCD_Pointer = 2;
			}
			else
				status_select = 0;
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
			if (status_select == 0)
			{
				status_select = 1;
				if (LCD_Pointer == 1)
				{
					for (i=3;i<=6;i++)
						copy_timer(Timer_Number,2,i);
				}
				else
					copy_timer(Timer_Number,0,1);

				Wrt_Grp_Svc_Timer(Timer_Number);
			}
			else
				status_select = 0;
		}

		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			if (status_select == 1)
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = LCD_Timer_Copy_Menu[LCD_Pointer + 1][i];
			}
			else
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = LCD_Timer_Copy_Menu[LCD_Pointer - 1][i];
			}
		}
	}
	if (Menu_level == 8)
	{					// Clear Timer
 		if (LCD_Pointer == 0)
 		{
 			upd_dpy = 1;
 			LCD_Pointer++; 
			status_select = 0;
 		    for(i=0; i<=19; i++)
 			  	LCD_Display[0][i] = LCD_Timer_Menu[Menu_level-1][i];
 	  	}
 		if((rdLCDio(Lcd_MODE) == 0) && (LCD_Mode_PB == 0))
 		{	   // Exit to main menu
 			LCD_Mode_PB = 1;  //
			LCD_Pointer = Menu_level-1;
			Menu_level = 1;
			for(i=0; i<=19; i++)
			{
				LCD_Display[0][i] = LCD_Timer_Menu[0][i];
			  	LCD_Display[1][i] = LCD_Timer_Menu[LCD_Pointer][i];
			}
			LCD_Display[0][19] = (char)Timer_Number + '0'; 
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
			status_select = 0;
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
			status_select = 0;
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	upd_dpy = 1;
			if (status_select == 0)
			{
				status_select = 1;
				clear_timer(Timer_Number);
				Wrt_Grp_Svc_Timer(Timer_Number);
			}
			else
				status_select = 0;
		}

		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			if (status_select == 1)
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = LCD_Timer_Clear_Menu[1][i];
			}
			else
			{
			    for(i=0; i<=19; i++)
				  	LCD_Display[1][i] = LCD_Timer_Clear_Menu[0][i];
			}
		}
	}
	if (Menu_level > 8)
		Menu_level = 1;
}

void clear_timer(int16 Timer)
{
	int16 i;

	svc_t[Timer].status = 0;		
	svc_t[Timer].service = 0;
	svc_t[Timer].logic = 0;
	svc_t[Timer].option = 0;
	for (i=0;i<=2;i++)
	{
		svc_t[Timer].month[i] = 0;
		svc_t[Timer].day[i] = 0;
		svc_t[Timer].md_on_hour[i] = 0;
		svc_t[Timer].md_off_hour[i] = 0;
		svc_t[Timer].md_on_minute[i] = 0;
		svc_t[Timer].md_off_minute[i] = 0;
	}
	for (i=0;i<=6;i++)
	{
		svc_t[Timer].on_hour[i] = 0;
		svc_t[Timer].off_hour[i] = 0;
		svc_t[Timer].on_minute[i] = 0;
		svc_t[Timer].off_minute[i] = 0;
	}	
}

void copy_timer(int16 timer_number, int16 Timer_Ix_1, int16 Timer_Ix_2)
{
	svc_t[timer_number].on_hour[Timer_Ix_2] = svc_t[timer_number].on_hour[Timer_Ix_1];
	svc_t[timer_number].on_minute[Timer_Ix_2] = svc_t[timer_number].on_minute[Timer_Ix_1];
	svc_t[timer_number].off_hour[Timer_Ix_2] = svc_t[timer_number].off_hour[Timer_Ix_1];
	svc_t[timer_number].off_minute[Timer_Ix_2] = svc_t[timer_number].off_minute[Timer_Ix_1];
}

void get_month_day_timer(int16 timer_number,int16 Timer_Ix)
{
	// Month/Day timer
	date_time_var[c_month] = svc_t[timer_number].month[Timer_Ix];
	date_time_var[c_day] =	svc_t[timer_number].day[Timer_Ix];
	date_time_var[c_md_on_hour] = svc_t[timer_number].md_on_hour[Timer_Ix];
	date_time_var[c_md_on_minute] = svc_t[timer_number].md_on_minute[Timer_Ix]; 
	date_time_var[c_md_off_hour] = svc_t[timer_number].md_off_hour[Timer_Ix];
	date_time_var[c_md_off_minute] = svc_t[timer_number].md_off_minute[Timer_Ix]; 
}

void set_month_day_timer(int16 timer_number,int16 Timer_Ix)
{
	// Month/Day timer
	svc_t[timer_number].month[Timer_Ix] = (uint8)date_time_var[c_month];
	svc_t[timer_number].day[Timer_Ix] = (uint8)date_time_var[c_day];
	svc_t[timer_number].md_on_hour[Timer_Ix] = (uint8)date_time_var[c_md_on_hour];
	svc_t[timer_number].md_off_hour[Timer_Ix] = (uint8)date_time_var[c_md_off_hour];
	svc_t[timer_number].md_on_minute[Timer_Ix] = (uint8)date_time_var[c_md_on_minute]; 
	svc_t[timer_number].md_off_minute[Timer_Ix] = (uint8)date_time_var[c_md_off_minute]; 
}

void get_dow_timer(int16 timer_number,int16 Timer_Ix)
{
	// day of week timer
	dow_time_var[c_on_hour] =  svc_t[timer_number].on_hour[Timer_Ix];
	dow_time_var[c_on_minute] = svc_t[timer_number].on_minute[Timer_Ix]; 
	dow_time_var[c_off_hour] = svc_t[timer_number].off_hour[Timer_Ix];
	dow_time_var[c_off_minute] = svc_t[timer_number].off_minute[Timer_Ix]; 
}

void set_dow_timer(int16 timer_number,int16 Timer_Ix)
{
	// day of week timer
	svc_t[timer_number].on_hour[Timer_Ix] = (uint8)dow_time_var[c_on_hour];
	svc_t[timer_number].off_hour[Timer_Ix] = (uint8)dow_time_var[c_off_hour];
	svc_t[timer_number].on_minute[Timer_Ix] = (uint8)dow_time_var[c_on_minute]; 
	svc_t[timer_number].off_minute[Timer_Ix] = (uint8)dow_time_var[c_off_minute]; 
}
*/

//****************************
//  Fault display routine
//****************************

void fault_display (int16 fault_line, int16 dpy_flt_ix, int16 dpy_ix)
{
	int16 i;
//	int16 j;
//	char *c;
	int16 vel_diff = 0;

  	if (f.Flts[dpy_flt_ix].code == 0)
		fault_line = 0;

	if ((fault_line == 0) || (fault_line == 25))
	{
 
		for(i=0; i<=19; i++)
		{
		  	LCD_Display[0][i] = LCD_Fault[f.Flts[dpy_flt_ix].code][i];
		  	if(f.Flts[dpy_flt_ix].code != 0)
		  	{
			  	LCD_Display[1][i] = LCD_Fault_Xtra[0][i];
			  	LCD_Display[2][i] = LCD_Fault_Xtra[1][i];
			  	if ((i>=1) && (i<=3))
				  	LCD_Display[2][i] = servf_tbl[f.Flts[dpy_flt_ix].servf][i-1];
			  	LCD_Display[3][i] = LCD_Procf[f.Flts[dpy_flt_ix].procf][i];
		  	}
		  	else
		  	{
		      	LCD_Display[1][i] = LCD_Clear_Fault[4][i];
		      	LCD_Display[2][i] = ' ';
		      	LCD_Display[3][i] = ' ';
		  	}
		}
		if(f.Flts[dpy_flt_ix].code != 0)
		{
			//   "ix=00,P= 1,F= G,#001",   // Line 1
			if (fault_line == 25)
			{
				  	LCD_Display[1][3] = '0';
				  	LCD_Display[1][4] = '0';
			}
			else
			{
			  	itoa(dpy_ix, LCD_String);  // display index
				if(dpy_ix < 10)  
				{              // of the
				  	LCD_Display[1][3] = '0';
				  	LCD_Display[1][4] = LCD_String[0];         // fault index
				}
				else
				{
				  	LCD_Display[1][3] = LCD_String[0];
				  	LCD_Display[1][4] = LCD_String[1];
				}
			}
			
		  	itoa(f.Flts[dpy_flt_ix].pos, LCD_String);  // position
			if(f.Flts[dpy_flt_ix].pos < 10)                // of the
			  	LCD_Display[1][9] = LCD_String[0];         // fault
			else
			{
			  	LCD_Display[1][8] = LCD_String[0];
			  	LCD_Display[1][9] = LCD_String[1];
			}

  		    LCD_Display[1][13] = flmrk[f.Flts[dpy_flt_ix].pos][0];	// Building position of fault
  		    LCD_Display[1][14] = flmrk[f.Flts[dpy_flt_ix].pos][1];
			
			
			itoa(f.Flts[dpy_flt_ix].count, LCD_String);  // # of Ocurences
			if(f.Flts[dpy_flt_ix].count < 10)                // of the
			{
			  	LCD_Display[1][17] = '0';
			  	LCD_Display[1][18] = '0';
			  	LCD_Display[1][19] = LCD_String[0];         // fault
			}
			else if(f.Flts[dpy_flt_ix].count < 100)
			{
			  	LCD_Display[1][17] = '0';
			  	LCD_Display[1][18] = LCD_String[0];
			  	LCD_Display[1][19] = LCD_String[1];
			}
			else
			{
			  	LCD_Display[1][17] = LCD_String[0];
			  	LCD_Display[1][18] = LCD_String[1];
			  	LCD_Display[1][19] = LCD_String[2];
			}


			itoa(f.Flts[dpy_flt_ix].hour, LCD_String);  // Hour
			if(f.Flts[dpy_flt_ix].hour < 10)                // of the
			  	LCD_Display[2][7] = LCD_String[0];         // fault
			else
			{
			  	LCD_Display[2][6] = LCD_String[0];
			  LCD_Display[2][7] = LCD_String[1];
			}
			itoa(f.Flts[dpy_flt_ix].min, LCD_String);  // Minute
			if(f.Flts[dpy_flt_ix].min < 10)                // of the
			{
			  	LCD_Display[2][9] = '0';
			  	LCD_Display[2][10] = LCD_String[0];         // fault
			}
			else
			{
			  	LCD_Display[2][9] = LCD_String[0];
			  	LCD_Display[2][10] = LCD_String[1];
			}

			itoa(f.Flts[dpy_flt_ix].sec, LCD_String);  // second
			if(f.Flts[dpy_flt_ix].sec < 10)                // of the
			{
			  	LCD_Display[2][12] = '0';
			  	LCD_Display[2][13] = LCD_String[0];         // fault
			}
			else
			{
			  	LCD_Display[2][12] = LCD_String[0];
			  	LCD_Display[2][13] = LCD_String[1];
			}

			itoa(f.Flts[dpy_flt_ix].mon, LCD_String);  // Month
			if(f.Flts[dpy_flt_ix].mon < 10)                // of the
			{
			  	LCD_Display[2][15] = '0';
			  	LCD_Display[2][16] = LCD_String[0];         // fault
			}
			else
			{
			  	LCD_Display[2][15] = LCD_String[0];
			  	LCD_Display[2][16] = LCD_String[1];
			}
			itoa(f.Flts[dpy_flt_ix].day, LCD_String);  // Day
			if(f.Flts[dpy_flt_ix].day < 10)                // of the
			{
			  	LCD_Display[2][18] = '0';
			  	LCD_Display[2][19] = LCD_String[0];         // fault
			}
			else
			{
			  	LCD_Display[2][18] = LCD_String[0];
			  	LCD_Display[2][19] = LCD_String[1];
			}

		}
	}
	
}

/*
	else 
	{

		if (fault_line == 1)
		{
			
			c = &Can_Device_Name[0][0];

			if (((f.Flts[dpy_flt_ix].code == f_MRCAN_device_fault) || 
				 (f.Flts[dpy_flt_ix].code == f_CTCAN_device_fault) ||
				 (f.Flts[dpy_flt_ix].code == f_GRCAN_device_fault)) &&
				(f.Flts[dpy_flt_ix].device != 0) &&
				(f.Flts[dpy_flt_ix].prog_flag1 != 0))
			{ // display device and device fault with count
				
				j = f.Flts[dpy_flt_ix].prog_flag1;
				for(i=0; i<=19; i++)
			    {
					LCD_Display[0][i] = LCD_device_Fault[0][i];  
				   	LCD_Display[1][i] = LCD_device_Fault[j][i];
			    }

		    
				j = f.Flts[dpy_flt_ix].device;
				if (f.Flts[dpy_flt_ix].code == f_MRCAN_device_fault)
				{
					if (j == c_mrcan_NTS)
						c = &Can_Device_Name[1][0];
					else if (j == c_mrcan_SPB)
						c = &Can_Device_Name[3][0];
					else if (j == c_mrcan_LW)
						c = &Can_Device_Name[7][0];
					else if (j == c_mrcan_VS)
						c = &Can_Device_Name[8][0];
					else if (j == c_mrcan_PI_LANT)
						c = &Can_Device_Name[9][0];
					else if (j >= c_mrcan_SEB_1)
						c = &Can_Device_Name[10][0];
	
					for (i=0;i<=7;i++)
						LCD_Display[0][i+4] = c[i];
					
					if (j >= c_mrcan_SEB_1)
					{
						itoa((j-c_mrcan_SEB_1)+1,LCD_String);
						LCD_Display[0][12] = LCD_String[0];
						if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
							LCD_Display[0][13] = LCD_String[1];
					}
				}
				else if (f.Flts[dpy_flt_ix].code == f_CTCAN_device_fault)
				{
					if (j == c_ctcan_ENC_SEL)
						c = &Can_Device_Name[0][0];
					else if (j == c_ctcan_SEL)
						c = &Can_Device_Name[4][0];
					else if (j == c_ctcan_DOOR)
						c = &Can_Device_Name[5][0];
					else if (j == c_ctcan_RDOOR)
						c = &Can_Device_Name[6][0];
					else if (j == c_ctcan_LW)
						c = &Can_Device_Name[7][0];
					else if (j == c_ctcan_VS)
						c = &Can_Device_Name[8][0];
					else if (j == c_ctcan_PI_LANT)
						c = &Can_Device_Name[9][0];
					else if (j == c_ctcan_SEB_1)		// CAR TOP
						c = &Can_Device_Name[12][0];
					else if (j == c_ctcan_SEB_2)		// SELECTOR DZ
						c = &Can_Device_Name[13][0];
					else if (j == c_ctcan_SEB_3)		// SELECTOR LIMITS
						c = &Can_Device_Name[14][0];
					else if (j >= c_ctcan_SEB_4)
						c = &Can_Device_Name[10][0];
					
					for (i=0;i<=7;i++)
						LCD_Display[0][i+4] = c[i];
						
					if (j >= c_ctcan_SEB_1)
					{
						itoa((j-c_ctcan_SEB_1)+1,LCD_String);
						LCD_Display[0][12] = LCD_String[0];
						if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
							LCD_Display[0][13] = LCD_String[1];
					}
				}
				else
				{
					if (j == c_grcan_VS)
						c = &Can_Device_Name[8][0];
					else if (j == c_grcan_FI_DPY)
						c = &Can_Device_Name[9][0];
					else if (j >= c_grcan_SEB_1)
						c = &Can_Device_Name[10][0];					
					
					for (i=0;i<=7;i++)
						LCD_Display[0][i+4] = c[i];

					if (j >= c_grcan_SEB_1)
					{
						itoa((j-c_grcan_SEB_1)+1,LCD_String);
						LCD_Display[0][12] = LCD_String[0];
						if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
							LCD_Display[0][13] = LCD_String[1];
					}

				}
			
				if (f.Flts[dpy_flt_ix].prog_flag2 < 1) 
					j = 1;
				if (f.Flts[dpy_flt_ix].prog_flag2 > 127)
					j = 127;
				// display number of faults
				sprintf(LCD_String,"%3i",j);
			  	LCD_Display[0][21] = LCD_String[0];
			  	LCD_Display[0][22] = LCD_String[1];
			  	LCD_Display[0][23] = LCD_String[2];
			}
			else if (((f.Flts[dpy_flt_ix].code == f_MRCAN_device_reset) || 
				 (f.Flts[dpy_flt_ix].code == f_CTCAN_device_reset) ||
				 (f.Flts[dpy_flt_ix].code == f_GRCAN_device_reset)) &&
				(f.Flts[dpy_flt_ix].device != 0))
			{		// Display Device and device requested initialization
			
				for(i=0; i<=19; i++)
			    {
					LCD_Display[0][i] = LCD_device_Reset[0][i];  
				   	LCD_Display[1][i] = LCD_device_Reset[1][i];
			    }
				j = f.Flts[dpy_flt_ix].device;
				if (f.Flts[dpy_flt_ix].code == f_MRCAN_device_reset)
				{
					if (j == c_mrcan_NTS)
						c = &Can_Device_Name[1][0];
					else if (j == c_mrcan_SPB)
						c = &Can_Device_Name[3][0];
					else if (j == c_mrcan_LW)
						c = &Can_Device_Name[7][0];
					else if (j == c_mrcan_VS)
						c = &Can_Device_Name[8][0];
					else if (j == c_mrcan_PI_LANT)
						c = &Can_Device_Name[9][0];
					else if (j >= c_mrcan_SEB_1)
						c = &Can_Device_Name[10][0];
	
					for (i=0;i<=7;i++)
						LCD_Display[0][i+7] = c[i];
					
					if (j >= c_mrcan_SEB_1)
					{
						itoa((j-c_mrcan_SEB_1)+1,LCD_String);
						LCD_Display[0][15] = LCD_String[0];
						if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
							LCD_Display[0][16] = LCD_String[1];
					}
				}
				else if (f.Flts[dpy_flt_ix].code == f_CTCAN_device_reset)
				{
					if (j == c_ctcan_ENC_SEL)
						c = &Can_Device_Name[0][0];
					else if (j == c_ctcan_SEL)
						c = &Can_Device_Name[4][0];
					else if (j == c_ctcan_DOOR)
						c = &Can_Device_Name[5][0];
					else if (j == c_ctcan_RDOOR)
						c = &Can_Device_Name[6][0];
					else if (j == c_ctcan_LW)
						c = &Can_Device_Name[7][0];
					else if (j == c_ctcan_VS)
						c = &Can_Device_Name[8][0];
					else if (j == c_ctcan_PI_LANT)
						c = &Can_Device_Name[9][0];
					else if (j == c_ctcan_SEB_1)		// CAR TOP
						c = &Can_Device_Name[12][0];
					else if (j == c_ctcan_SEB_2)		// SELECTOR DZ
						c = &Can_Device_Name[13][0];
					else if (j == c_ctcan_SEB_3)		// SELECTOR LIMITS
						c = &Can_Device_Name[14][0];
					else if (j >= c_ctcan_SEB_4)
						c = &Can_Device_Name[10][0];
					
					for (i=0;i<=7;i++)
						LCD_Display[0][i+7] = c[i];
						
					if (j >= c_ctcan_SEB_1)
					{
						itoa((j-c_ctcan_SEB_1)+1,LCD_String);
						LCD_Display[0][15] = LCD_String[0];
						if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
							LCD_Display[0][16] = LCD_String[1];
					}
				}
				else
				{
					if (j == c_grcan_VS)
						c = &Can_Device_Name[8][0];
					else if (j == c_grcan_FI_DPY)
						c = &Can_Device_Name[9][0];
					else if (j >= c_grcan_SEB_1)
						c = &Can_Device_Name[10][0];					
					
					for (i=0;i<=7;i++)
						LCD_Display[0][i+7] = c[i];

					if (j >= c_grcan_SEB_1)
					{
						itoa((j-c_grcan_SEB_1)+1,LCD_String);
						LCD_Display[0][15] = LCD_String[0];
						if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
							LCD_Display[0][16] = LCD_String[1];
					}

				}
			}

		}
		if (fault_line > 1)
		{
			j = (fault_line - 2) * 2;
			for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = LCD_Detail_Fault[j][i];  
			   	LCD_Display[1][i] = LCD_Detail_Fault[j+1][i];
		    }

		}
		if (fault_line == 2)
		{
			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].servf);
		  	LCD_Display[0][4] = LCD_String[0];
		  	LCD_Display[0][5] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].procf);
		  	LCD_Display[0][12] = LCD_String[0];
		  	LCD_Display[0][13] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].doorf);
		  	LCD_Display[0][20] = LCD_String[0];
		  	LCD_Display[0][21] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].rdoorf);
		  	LCD_Display[1][4] = LCD_String[0];
		  	LCD_Display[1][5] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].dpref);
		  	LCD_Display[1][12] = LCD_String[0];
		  	LCD_Display[1][13] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].dirf);
		  	LCD_Display[1][20] = LCD_String[0];
		  	LCD_Display[1][21] = LCD_String[1];
		}
		else if (fault_line == 3)
		{
			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].empf);
		  	LCD_Display[0][4] = LCD_String[0];
		  	LCD_Display[0][5] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].medf);
		  	LCD_Display[0][12] = LCD_String[0];
		  	LCD_Display[0][13] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].codebf);
		  	LCD_Display[0][20] = LCD_String[0];
		  	LCD_Display[0][21] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].eqf);
		  	LCD_Display[1][4] = LCD_String[0];
		  	LCD_Display[1][5] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].firef);
		  	LCD_Display[1][12] = LCD_String[0];
		  	LCD_Display[1][13] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].rfiref);
		  	LCD_Display[1][20] = LCD_String[0];
		  	LCD_Display[1][21] = LCD_String[1];

		}
		else if (fault_line == 4)
		{
			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].hsf);
		  	LCD_Display[0][4] = LCD_String[0];
		  	LCD_Display[0][5] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].startf);
		  	LCD_Display[0][12] = LCD_String[0];
		  	LCD_Display[0][13] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].dcalls);
		  	LCD_Display[0][20] = LCD_String[0];
		  	LCD_Display[0][21] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].estop);
		  	LCD_Display[1][4] = LCD_String[0];
		  	LCD_Display[1][5] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].nstopf);
		  	LCD_Display[1][12] = LCD_String[0];
		  	LCD_Display[1][13] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].relevel);
		  	LCD_Display[1][20] = LCD_String[0];
		  	LCD_Display[1][21] = LCD_String[1];

		}
		else if (fault_line == 5)
		{
			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].stepf);
		  	LCD_Display[0][4] = LCD_String[0];
		  	LCD_Display[0][5] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].door_seqf);
		  	LCD_Display[0][12] = LCD_String[0];
		  	LCD_Display[0][13] = LCD_String[1];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].next_stop);
		  	LCD_Display[0][20] = LCD_String[0];
		  	LCD_Display[0][21] = LCD_String[1];

			j = f.Flts[dpy_flt_ix].ins_status;
		  	LCD_Display[1][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][5] = hex_to_ascii[j & 0x0f];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].ncu_door_seq);
		  	LCD_Display[1][12] = LCD_String[0];
		  	LCD_Display[1][13] = LCD_String[1];

			j = f.Flts[dpy_flt_ix].device;
		  	LCD_Display[1][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][21] = hex_to_ascii[j & 0x0f];

		}
		else if (fault_line == 6)
		{
			j = f.Flts[dpy_flt_ix].prog_flag1;
		  	LCD_Display[0][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].prog_flag2;
		  	LCD_Display[0][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].device_2;
		  	LCD_Display[0][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][21] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[0];
		  	LCD_Display[1][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[1];
		  	LCD_Display[1][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[2];
		  	LCD_Display[1][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][21] = hex_to_ascii[j & 0x0f];

		}
		else if (fault_line == 7)
		{
			j = f.Flts[dpy_flt_ix].iodata[3];
		  	LCD_Display[0][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[4];
		  	LCD_Display[0][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[5];
		  	LCD_Display[0][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][21] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[6];
		  	LCD_Display[1][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[7];
		  	LCD_Display[1][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[8];
		  	LCD_Display[1][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][21] = hex_to_ascii[j & 0x0f];

		}
		else if (fault_line == 8)
		{
			j = f.Flts[dpy_flt_ix].iodata[9];
		  	LCD_Display[0][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[10];
		  	LCD_Display[0][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[11];
		  	LCD_Display[0][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][21] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[12];
		  	LCD_Display[1][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[13];
		  	LCD_Display[1][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[14];
		  	LCD_Display[1][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][21] = hex_to_ascii[j & 0x0f];

		}
		else if (fault_line == 9)
		{
			j = f.Flts[dpy_flt_ix].iodata[15];
		  	LCD_Display[0][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[16];
		  	LCD_Display[0][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[17];
		  	LCD_Display[0][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][21] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[18];
		  	LCD_Display[1][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[19];
		  	LCD_Display[1][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[20];
		  	LCD_Display[1][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][21] = hex_to_ascii[j & 0x0f];

		}
		else if (fault_line == 10)
		{

			j = f.Flts[dpy_flt_ix].iodata[21];
		  	LCD_Display[0][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[22];
		  	LCD_Display[0][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[23];
		  	LCD_Display[0][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][21] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[24];
		  	LCD_Display[1][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[25];
		  	LCD_Display[1][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[26];
		  	LCD_Display[1][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][21] = hex_to_ascii[j & 0x0f];

		}
		else if (fault_line == 11)
		{

			j = f.Flts[dpy_flt_ix].iodata[27];
		  	LCD_Display[0][4] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][5] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[28];
		  	LCD_Display[0][12] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][13] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].iodata[29];
		  	LCD_Display[0][20] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][21] = hex_to_ascii[j & 0x0f];

		}
		else if (fault_line == 12)
		{
			sprintf(LCD_String,"%08xh",(int)f.Flts[dpy_flt_ix].statusf);
		    for(i=0; i<=7; i++)
		    {
			   	LCD_Display[0][13+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%08xh",(int)f.Flts[dpy_flt_ix].statusf2);
		    for(i=0; i<=7; i++)
		    {
			   	LCD_Display[1][13+i] = LCD_String[i];
		    }
		}
		else if (fault_line == 13)
		{
			sprintf(LCD_String,"%08xh",(int)f.Flts[LCD_Sub_Point].statusf3);
		    for(i=0; i<=7; i++)
		    {
			   	LCD_Display[0][13+i] = LCD_String[i];
		    }
			sprintf(LCD_String,"%08xh",(int)f.Flts[LCD_Sub_Point].statusf4);
		    for(i=0; i<=7; i++)
		    {
			   	LCD_Display[1][13+i] = LCD_String[i];
		    }
		}
		else if (fault_line == 13)
		{
#if(Traction == 1)
			sprintf(LCD_String,"%8d",f.Flts[dpy_flt_ix].dpp_count); 
		    for(i=0; i<=7; i++)
		    {
			   	LCD_Display[0][3+i] = LCD_String[i];
		    }

			sprintf(LCD_String,"%8d",f.Flts[dpy_flt_ix].sd_count); 
		    for(i=0; i<=7; i++)
		    {
			   	LCD_Display[0][16+i] = LCD_String[i];
		    }

			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].dmd_vel_fpm); 
		    for(i=0; i<=4; i++)
		    {
			   	LCD_Display[1][4+i] = LCD_String[i];
		    }

			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].enc_vel_fpm); 
		    for(i=0; i<=4; i++)
		    {
			   	LCD_Display[1][19+i] = LCD_String[i];
		    }
#endif
		}
		else if (fault_line == 14)
		{
#if(Traction == 1)
			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].vel_fpm); 
		    for(i=0; i<=4; i++)
		    {
			   	LCD_Display[0][6+i] = LCD_String[i];
		    }

			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].dmd_vel_fpm); 
		    for(i=0; i<=4; i++)
		    {
			   	LCD_Display[0][19+i] = LCD_String[i];
		    }

			if(abs(f.Flts[dpy_flt_ix].drv_vel_fpm-f.Flts[dpy_flt_ix].enc_vel_fpm)>9999)
				vel_diff = 9999;
			else
				vel_diff = f.Flts[dpy_flt_ix].dmd_vel_fpm-f.Flts[dpy_flt_ix].enc_vel_fpm;
		

			sprintf(LCD_String,"%5i",vel_diff); 
		    for(i=0; i<=4; i++)
		    {
			   	LCD_Display[1][5+i] = LCD_String[i];
		    }

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].enc_dir); 
		    for(i=0; i<=1; i++)
		    {
			   	LCD_Display[1][22+i] = LCD_String[i];
		    }

#endif
		}
		else if (fault_line == 15)
		{

#if(Traction == 1)
			sprintf(LCD_String,"%8d",f.Flts[dpy_flt_ix].SPB_poscnt); 
		    for(i=0; i<=7; i++)
		    {
			   	LCD_Display[0][10+i] = LCD_String[i];
		    }

			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].SPB_vel); 
		    for(i=0; i<=4; i++)
		    {
			   	LCD_Display[1][8+i] = LCD_String[i];
		    }

			j = f.Flts[dpy_flt_ix].SPB_status; 
   			LCD_Display[1][20] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][21] = hex_to_ascii[j & 0x0f];

#endif
		}
		else if (fault_line == 16)
		{

#if(Traction == 1)
 			j = f.Flts[dpy_flt_ix].SPB_service; 
   			LCD_Display[0][8] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[0][9] = hex_to_ascii[j & 0x0f];

 			j = f.Flts[dpy_flt_ix].SPB_command; 
   			LCD_Display[0][15] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[0][16] = hex_to_ascii[j & 0x0f];

 			j = f.Flts[dpy_flt_ix].SPB_ios[0]; 
   			LCD_Display[0][21] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[0][22] = hex_to_ascii[j & 0x0f];

 			j = f.Flts[dpy_flt_ix].SPB_ios[1]; 
   			LCD_Display[1][3] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][4] = hex_to_ascii[j & 0x0f];

 			j = f.Flts[dpy_flt_ix].SPB_ios[2]; 
   			LCD_Display[1][9] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][10] = hex_to_ascii[j & 0x0f];

 			j = f.Flts[dpy_flt_ix].SPB_ios[3]; 
   			LCD_Display[1][15] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][16] = hex_to_ascii[j & 0x0f];

 			j = f.Flts[dpy_flt_ix].SPB_ios[4]; 
   			LCD_Display[1][21] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][22] = hex_to_ascii[j & 0x0f];



#endif
		}
		else if (fault_line == 17)
		{

			j = f.Flts[dpy_flt_ix].fault_bits[0];
		  	LCD_Display[0][6] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][7] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].fault_bits[1];
		  	LCD_Display[0][19] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[0][20] = hex_to_ascii[j & 0x0f];


			j = f.Flts[dpy_flt_ix].fault_bits[2];
		  	LCD_Display[1][6] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][7] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].fault_bits[3];
		  	LCD_Display[1][19] = hex_to_ascii[(j >> 4) & 0x0f];
		  	LCD_Display[1][20] = hex_to_ascii[j & 0x0f];


		}
		else if (fault_line == 18)
		{

			j = f.Flts[dpy_flt_ix].ss_status;
		  	LCD_Display[0][7] = hex_to_ascii[(j >> 12) & 0x000f];
		  	LCD_Display[0][8] = hex_to_ascii[(j >> 8) & 0x000f];
		  	LCD_Display[0][9] = hex_to_ascii[(j >> 4) & 0x000f];
		  	LCD_Display[0][10] = hex_to_ascii[j & 0x000f];

			j = f.Flts[dpy_flt_ix].power_status;
		  	LCD_Display[0][20] = hex_to_ascii[(j >> 12) & 0x000f];
		  	LCD_Display[0][21] = hex_to_ascii[(j >> 8) & 0x000f];
		  	LCD_Display[0][22] = hex_to_ascii[(j >> 4) & 0x000f];
		  	LCD_Display[0][23] = hex_to_ascii[j & 0x000f];


			sprintf(LCD_String,"%08xh",(int)f.Flts[dpy_flt_ix].run_statusf);
		    for(i=0; i<=7; i++)
		    {
			   	LCD_Display[1][13+i] = LCD_String[i];
		    }


		}
		else if (fault_line == 19)
		{

			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].NTS_vel); 
		    for(i=0; i<=4; i++)
		    {
			   	LCD_Display[0][8+i] = LCD_String[i];
		    }

			j = f.Flts[dpy_flt_ix].NTS_status; 
   			LCD_Display[0][20] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[0][21] = hex_to_ascii[j & 0x0f];

			j = f.Flts[dpy_flt_ix].NTS_service; 
   			LCD_Display[1][9] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][10] = hex_to_ascii[j & 0x0f];
			
			j = f.Flts[dpy_flt_ix].NTS_command; 
   			LCD_Display[1][17] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][18] = hex_to_ascii[j & 0x0f];

		}
		else if (fault_line == 20)
		{

			j = f.Flts[dpy_flt_ix].NTS_limit_flt[0]; 
   			LCD_Display[0][5] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[0][6] = hex_to_ascii[j & 0x0f];
			
			j = f.Flts[dpy_flt_ix].NTS_limit_flt[1]; 
   			LCD_Display[0][13] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[0][14] = hex_to_ascii[j & 0x0f];
			
			j = f.Flts[dpy_flt_ix].NTS_limit_flt[2]; 
   			LCD_Display[0][21] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[0][22] = hex_to_ascii[j & 0x0f];
			
			  	
			j = f.Flts[dpy_flt_ix].NTS_ios[0]; 
   			LCD_Display[1][5] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][6] = hex_to_ascii[j & 0x0f];
			
			j = f.Flts[dpy_flt_ix].NTS_ios[1]; 
   			LCD_Display[1][13] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][14] = hex_to_ascii[j & 0x0f];
			
			j = f.Flts[dpy_flt_ix].NTS_ios[2]; 
   			LCD_Display[1][21] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[1][22] = hex_to_ascii[j & 0x0f];
			
		}
		else if (fault_line == 21)
		{	// LCD_Detail_Fault Menu
			//  "Nud=  , DRq=  , CFg=    ",	// 34
			//  "RnS=  , StS=  , LvS=    ",	// 35
		
			sprintf(LCD_String,"%02x",f.Flts[dpy_flt_ix].nudgst_flags);
		  	LCD_Display[0][4] = LCD_String[0];
		  	LCD_Display[0][5] = LCD_String[1];

			sprintf(LCD_String,"%02x",f.Flts[dpy_flt_ix].door_req_flags);
		  	LCD_Display[0][12] = LCD_String[0];
		  	LCD_Display[0][13] = LCD_String[1];

			sprintf(LCD_String,"%02x",f.Flts[dpy_flt_ix].call_flags);
		  	LCD_Display[0][20] = LCD_String[0];
		  	LCD_Display[0][21] = LCD_String[1];

			sprintf(LCD_String,"%02x",f.Flts[dpy_flt_ix].chkrun_st);
		  	LCD_Display[1][4] = LCD_String[0];
		  	LCD_Display[1][5] = LCD_String[1];

			sprintf(LCD_String,"%02x",f.Flts[dpy_flt_ix].chkstart_st);
		  	LCD_Display[1][12] = LCD_String[0];
		  	LCD_Display[1][13] = LCD_String[1];

			sprintf(LCD_String,"%02x",f.Flts[dpy_flt_ix].chkdoor_st);
		  	LCD_Display[1][20] = LCD_String[0];
		  	LCD_Display[1][21] = LCD_String[1];
		}
		else if (fault_line == 22)
		{	// LCD_Detail_Fault Menu 
			//	"DrS=  ,FSD=    ,RSd=    ",   // 40
			//  "Motion Tmr =            ",   // 41
		
			sprintf(LCD_String,"%02x",f.Flts[dpy_flt_ix].chkdoor_st);
		  	LCD_Display[0][4] = LCD_String[0];
		  	LCD_Display[0][5] = LCD_String[1];

			sprintf(LCD_String,"%04x",f.Flts[dpy_flt_ix].front_sd);
		  	LCD_Display[0][11] = LCD_String[0];
		  	LCD_Display[0][12] = LCD_String[1];
		  	LCD_Display[0][13] = LCD_String[2];
		  	LCD_Display[0][14] = LCD_String[3];

			sprintf(LCD_String,"%04x",f.Flts[dpy_flt_ix].rear_sd);
		  	LCD_Display[0][20] = LCD_String[0];
		  	LCD_Display[0][21] = LCD_String[1];
		  	LCD_Display[0][22] = LCD_String[2];
		  	LCD_Display[0][23] = LCD_String[3];

			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].motion_tmr);
		  	LCD_Display[1][13] = LCD_String[0];
		  	LCD_Display[1][14] = LCD_String[1];
   		  	LCD_Display[1][15] = LCD_String[2];
		  	LCD_Display[1][16] = LCD_String[3];
		  	LCD_Display[1][17] = LCD_String[4];
		}
		else if (fault_line == 23)
		{	// LCD_Detail_Fault 
			//  "PAL Vel=00000, Stat=    ",   // 42
  			//  "Ins Svc=00, % Load=0000 ",   // 43
		
			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].PAL_vel); 
		    for(i=0; i<=4; i++)
		    {
			   	LCD_Display[0][8+i] = LCD_String[i];
		    }

			j = f.Flts[dpy_flt_ix].PAL_stat; 
   			LCD_Display[0][20] = hex_to_ascii[(j >> 4) & 0x0f];
			LCD_Display[0][21] = hex_to_ascii[j & 0x0f];

			sprintf(LCD_String,"%2i",f.Flts[dpy_flt_ix].ins_servf);
		  	LCD_Display[1][8] = LCD_String[0];
		  	LCD_Display[1][9] = LCD_String[1];

			sprintf(LCD_String,"%3i",f.Flts[dpy_flt_ix].percent_load/10); 
		    for(i=0; i<=3; i++)
		    {
			   	LCD_Display[1][21+i] = LCD_String[i];
		    }
		}
		else if (fault_line == 24)
		{	// LCD_Detail_Fault Menu 
			//  "DrvCmd=0000,TrqCmd=00000",   // 44
			//  "DvStat=0000,MtrTrq=00000",   // 45
		
			sprintf(LCD_String,"%04x",f.Flts[dpy_flt_ix].drv_cmd);
		  	LCD_Display[0][7] = LCD_String[0];
		  	LCD_Display[0][8] = LCD_String[1];
		  	LCD_Display[0][9] = LCD_String[2];
		  	LCD_Display[0][10] = LCD_String[3];

			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].drv_torque_cmd);
		  	LCD_Display[0][19] = LCD_String[0];
		  	LCD_Display[0][20] = LCD_String[1];
		  	LCD_Display[0][21] = LCD_String[2];
		  	LCD_Display[0][22] = LCD_String[3];
		  	LCD_Display[0][23] = LCD_String[4];

			sprintf(LCD_String,"%04xh",(int16)f.Flts[dpy_flt_ix].drv_status);
		  	LCD_Display[1][7] = LCD_String[0];
		  	LCD_Display[1][8] = LCD_String[1];
		  	LCD_Display[1][9] = LCD_String[2];
		  	LCD_Display[1][10] = LCD_String[3];
		    
			sprintf(LCD_String,"%5i",f.Flts[dpy_flt_ix].drv_mtr_torque);
		  	LCD_Display[1][19] = LCD_String[0];
		  	LCD_Display[1][20] = LCD_String[1];
		  	LCD_Display[1][21] = LCD_String[2];
		  	LCD_Display[1][22] = LCD_String[3];
		  	LCD_Display[1][23] = LCD_String[4];

		}
	}

}

*/


/* Revision History

 03/04/02 1.14 mhd		1.  Made LCD module.
						2.  Set all const data variables as far.
						3.  Added Emergency power parameters.
						4.  Write Dispatcher number where car number was located on input/output screen
4/1/02	2.00 mhd		1.  Added status for group comm.
05/29/02 2.02 mhd		1.  Changed Attendent to Attendant (service display)
						2.  Deleted Delay using inctime in init_LCD routine since inctime calls Refresh_LCD
							and causes the display to go dark before clearing.
06/25/02 2.03 mhd		1.  Added car cal lockout security i/o's to lcd screen.
                        2.  Correct tx_cnt and rx_cnt for group comm status (display positive counts using unsigned
						    variables - negative counts shows spaces).
07/30/02 3.00 mhd		1.  Display group comm loss count on comm status display.
10/24/02 3.01 mhd		1.  Changed video display to show new fault data and to rotate fault screen when enter button hit.
						2. Deleted Car_Flts array and use Flts array for all cars.
12/19/02 3.02 mhd		1.  Changed Gen Run Time to say Gen/Lt/Fan Tim.
						2. 	Added second level to the following menu items: 
												Job Statistics
													View Job Statistics
													Clear Job Statistics
												Input and Outputs
													Car Inputs and Outputs
													Group Inputs and Outputs
												Setup Calls
													Setup Down Call
													Setup Car call
													Setup Up Call
												Fault Log
													View Fault Log
													Clear Fault Log
												Elevator Setup
													Set Speed Clamp # 1
													Set Speed Clamp # 2
													Set Speed Clamp # 3
													Set Speed Clamp # 4
													Set Speed Clamp # 5
													Lear Hoistway
												Diagnostics
													System Status Log
													Clear System Status Log
													Group Comm
													Clear Group Comm
						3.	Show Serial Hall Call I/o's on Input and Output screen.
						4.	Setup Up and Down Serial Hall Calls
						5.  In Learn Hoistway state, changed dpp count display index to i+3 from i+4 and changed
							check from ((i+4) < 12) to ((i+3) < 11)) to correct display problem of writting over
							the V in Vel. (Changed to match display hoistway table state).
						6. Moved BP8 to ETH, renamed ETH to EQ and moved to I/O #190, moved COL to I/O #191, and if non-distance
						   feedback than use TPL for BKS input.
						7. Use drive_type == 1 for hydro, drive_type == 3,4,5 for distance feedback, and deleted
						   using drive_type == 6 for non-distance feedback because there may be other cases for ndf
						8. Fixed index to write dpp to lcd display during learn hoistway.
2/11/03  				9. Display car number with software version.
3/18/03  3.03 mhd.		1. Added subroutines for setting car and hall calls.
						2. Made clear_job_statistics subroutine.
4/3/03 3.04 mhd			1. Use hall call serial comm if car serial comm used (sercom & 3) != 0.
4/23/03  				2. In main menu, use Traction == 0 to skip setup menu
6/4/03					3. Fix bug in diagnostics menu (array for LCD display used an index of 5 instead of 12).
 						4. Added return to lobby service.
 						5. Fix bug in diagnostics menu (array for LCD display used an index of 5 instead of 12).
 						6. Change PI location for special case when serial hc board used and floors > = 14 and <= 21.
						7. Moved rdfvar and wrfvar from hydro.cpp to this file.
						8. Add new field variable (DOB over Nudg) and (Emerg Dispatch).
						9. Put gripper_fault into backuped stored memory to remember status during power outage.
						10. Set car call bit in serial.brd_io when car serial comm used. See set_carcall.
						11. Added Rear security input labels (RCS) to ionames.
 7/7/03 				12. Added Lobby request and next up parameters.
						13. Allow hall call i/o board to be greater than 10 and display ir, code blue and security io data.
7/30/03	3.05 mhd		1. Added code blue car select field variable.
						2. Added ir car select field variable.
8/11/03 3.06 mhd		1. Change statusf to 32 bits therefore had to add two bytes to each block in the sys_stat buffer.
						2. Use TPL location for BKS for both traction and traction ndf.  Add LWA to s-curve board where
						   BKS was located.
8/29/03 3.07 mhd		1. Relocate LWB and LWA when using an NDF controller.
10/2/03 3.09 mhd		1. Use (sercom & 1) != 0 so car serial comm can be used without hall serial comm.  Also check
						   gc_hall_io_dev for serial hall i/o devices.
						2. Add adjustment variable for independent to override security.
10/10/03 3.10 mhd		1. Modified Refresh LCD so that fire service is shown when firef > 0.
11/11/03 3.11 mhd		1. Modified Set Speed Clamp procedures to check to be out of level zone.
12/15/03 3.15 mhd		1. Decreased the debounce count for the lcd switches when not a satdx4 cpu.
12/17/03 3.16 mhd		1. Added parameter fvars[fvbkcrel] to enable dropping the brake cool relay immediately during a relevel
						   to proved a partial brake lift on relevel.
						2. Added parameter fvars[fvbksrel] for a seperate brake pick delay during relevel.						
12/24/03 3.17 mdh		1. Added parameters for up and down peak operation.
						2. Added Overspeed test and Buffer test.
						3. Added TOC can comm error.
4/21/04 4.00 mhd		1. Setup new io locations for 1038 board. Changed board initialization, io addresses and started io array at 0.
8/27/04 4.03 mhd		1. Added reset jack procedure.
1/10/04 4.04 pn			1. Added field variable to invert brake lift switch input.
						2. Added field variable to invert in-service light output. 
						3. Added field variable to change a car other than 1 or 2 to become the master.
						4. Added field variable to time out video display.
1/12/04	4.04 pn			1. Added parking floor and width parameters to park cars at specific floors.
2/9/05 v4.07 mhd		1. Added load weighing setup screens.
						2. Correct detailed fault display output for IOD=.
4/25/05 4.14 mhd		1. Modify load weighing setup to record empty and full value at each floor.
5/6/05 v4.17 mhd		1. Added can buss error count to can diagnostic display.
 						2. Ask if you are sure you want to secure car calls if secure car calls is selected.
6/2/05 V4.19 mhd		1. Changed fault label from DPR input fail to DPM input fail.
6/22/05 v4.23 mhd		1. Before returning to pwrup routine, enable DOS interrupts.
6/22/05 V4.23 mhd		2. Added local gettime, getdate, settime setdate so that the 18.2 msec DOS interrupt could be disabled.
7/12/05 v4.24 mhd		1. Added pulse count error codes.
7/27/05 V4.26 mhd		1. Change fault display for MC to MCA and MS to MCC.
						2. Changed STP input off to CS input off.
8/4/05 V4.27 mhd		1. Added watchdog test.
						2. Added IR control variable.
8/22/05 v4.29 mhd		1. Changed start index for can from c_can_SPB to c_can_START to add the brake board c_can_BRK.
10/20/05 v4.36 mhd		1. Reset rope gripper from the LCD interface.
10/31/05 v4.37 mhd		1. Added safety string status to elevator status display.
11/9/05 v4.39 mhd		1. Added setup rear car and hall calls from LCD interface.
11/16/05 v4.39 mhd		2. Added door open/close delay time variable
11/21/05 V4.40 mhd		1. Added security recall variable.
11/28/05 v4.41 mhd		1. Added secirity recall floor. 
						2. Modified remoted cc options.
12/8/05 v4.43 mhd		1. Deleted Dynamic Brake Resistor temperature variable.
12/14/05 V4.44 mhd		1. When resetting the gripper, make sure the enter button is not being pressed when entering the
                           routine for the first time (do a return until the enter is not pressed).  This will keep GR1R
						   and GR2R from pulsing when entering in the reset gripper function.
2/1/06 v4.51 mhd		1. Added short dwell door timer
2/24/06 v4.56 mhd		1. Added parameter for stop at lobby fvars[fvstplby] and to invert CLF.
3/14/06 v4.59 mhd		1. Added parameters to invert TPL, LPS and LOS.
						2. Added parameter to nudge door with no onward calls.
						3. Added fvfireop variable
4/4/06 v4.61 mhd		1. Added fire options display. 
						2. Added ATT set car call when hall call set variable.
4/24/06 v4.64 mhd		1. Added Code blue door time, cb light and cb ind req variables.
						2. Added Hospital Service.
5/21/06 v4.67 mhd		1. Start buffer and counterweight test from the top or bottom floor instead of top - 1 and bottom + 1.
						2. Changed display for "p input off" to "p input on".
5/30/06 V4.68 mhd		1. Corrected display of gpi inputs in get_io_status.
6/5/06 V4.69 mhd		1. Added variable for single automatic pushbutton.
6/27/06 V4.74 mhd		1. Changed fviser to use +2 for in-use light
7/7/06 v4.75 mhd		1. Added variables fvdonodol to turn on the door open output if dol is not active when dwelling and 
						   also added fvdisglt to disable the gate and lock test for New York City.
7/25/06 v4.77 mhd		1. Changed max value for dpy_flt_line in in increment section to show the fault display.
7/26/06 5.00 mhd		1. Added realtime displays for LCD_Detail_Fault data in the Elevator Status display.  Also made subroutines
						   to execute this function.
8/15/06 v5.01 mhd		1. Added fvars[fvrlvst] relevel start speed.
8/16/06 v5.02 mhd		1. Added fvars[fvemdec] (Em_Decel) for the emergency decel rate.
						2. Round up when sending velocity and enc_vel to display by adding 0.5 before converting value to an integer.
9/5/06 v5.04 mhd		1. Added fvars[fvrtl] return to lobby variable
10/13/06 v5.06 mhd		1. Added parking type fvars[fvparktype] variable.
						2. Mofify gripper_fault reset to turn on GR1R AND GR2R regardless of the state of GTS.
11/6/06 v5.08 mhd		1. Added UT6/DT6, UT5/DT5 and UT4/DT4 in display hoistway table and elevator setup displays.
						2. Changed Cam_Mode finished valued from 10 to 16.
						3. Corrected parking floor index for 6 and 7.
						4. Added variable for non-simultaneous doors.
11/29/06 v5.09 mhd		1. Added variable for pre-opening door and for percent overspeed.  
						2. Modified variable for gripper trip from spb speed and overspeed.
12/26/06 v5.10 mhd		1. Added pos count update enable, pulse delay and gripper buzzer enable flags
						2. Changed brake lift switch invert option to show 0 for N/C and 1 for N/O.
12/29/06 v5.11 mhd		1. Added display for pulse count update in hoistway table display
						2. Use -1 for max number of cars and -2 for max number of floors for max adjustable variables and
					   	   use -1 for car number and -2 for bottom floor for the min variables.
						3. Added lowoil switch fault.
						4. Changed all double data types to float for the djgpp compiler.
1/12/07 v5.13 mhd		1. Add bit in fire option to turn off fire buzzer when car reaches fire floor.
1/18/07 v5.15 mhd		1. Added fire option 2 variable.
						2. Added brake opto delay.
1/26/07 v5.16 mhd		1. Changed "GATE OR LOCK OFF" status flag to "Door Close Contact".
2/15/07 v5.18 mhd		1. Made arrays out of variables that used one bit for floor from 1 32 word to 2.
						2. Added more pages for the car and group io display.
						3. Made the floor mask large enough for 60 floors.
						4. Show device version and revision on group comm display.
						5. Modified get_io_status for additional i/o boards.
2/23/07 v5.19 mhd		1. Corrected size declaration for bittbl from int to int16.
						2. Added interim revision number int_revision.
3/22/07 v5.21 mhd		1. Corrected the return from mode screen in load weighing setup: use LCD_Elev_Setup[16] instead of
							LCD_Elev_Setup[12].
4/11/07 v5.22 mhd		1. Added fvars[fvlevfltcnt]  leveling fault count variable.
						2. Added torque down variable fvars[fvtorqdn].
5/11/07 v5.23 mhd		1. Left out "break;" statement in detailed Elevator_Status.
						2. Changed soft stop timer to .2 sec minimum. 
						3. Changed relevel fault count to 3 trys minimum.
						4. Added torque ramp time.
						5. Added several error codes for hoistway variable check.
5/29/07 v5.24 mhd		1. Modified label for ISER Output Ctl.
						2. Added Electric Eye time-out parameter.
6/6/07 v5.25 mhd		1. Added variable fvars[fvotshc] "OTS No HC Canc" to prevent from cancelling hall calls when all cars go out of service.
6/18/07 v5.27 mhd		1. Added ability to modify the hoistway table floor and limit counts.
7/10/07 v5.30 mhd		1. Added phase 1 door close time-out variable.
8/6/07 v5.31 mhd		1. Added variable fvnohcreop to prevent hall call from reopening when set and onward hall call.
8/13/07 v5.33 mhd		1. Add bit in fireop2 to disable blink for phase 2 fire light (chicago only). Default is to blink
						   fire light when motor room or hoistway smoke detector active.
8/20/07 v5.34 mhd		1. Added drive ready fault counter.
9/13/07 v5.36 mhd		1. Change accel and decel max to 300 and jerk rates to 480.
9/24/07 				2. Change fviser & 0x04 to out of service alarm.
						3. Added fvrtldt (return to lobby door dwell time)
						4. Moved Double stroke variable to car options.
						5. Placed Em power floor in car options and left it in group options.
						6. Placed Em Dispatch in group options and left it in car options.
10/1/07 V5.37 mhd		1. Increased number of variables from 180 to 360. 
						2. Added NCU Lantern Control Variable.
						3. Added NCU Preference Control Variable.
10/4/07 v5.38 mhd		1. Added Second Riser Control variable.
						2. Added Second Riser Lanter Control variable.
10/18/07 v5.38.1 mhd	1. Added BKS Fault slowdown error 
10/31/07 v5.38.4 mhd	1. Added HBZ/PI Dis at non-valid floor.
11/13/07 v5.39 mhd		1. Added vip timer, vip op and nmb vip cars variables.
			   pn/mhd	2. Added Car call test.
11/20/07 v5.39.1 mhd	1. Added parameters for Emergency Power park and op outputs.
11/20/07 v5.39.2 mhd	1. Added elevator off option variable.
11/28/07 v5.39.3 mhd	1. Added drive baud and drive update rate variables.
12/01/07 v5.39.4 mhd	1. Added alternate lobby floor, alternate parking floor, lobby request control and 
						   handicap hall button control variables.
12/11/07 v5.39.5 pn		1. Added paramter to time-out closing of freight doors.
12/12/07 v5.39.6 mhd	1. Added leveling time-out fault.
12/28/07 v5.39.9 mhd	1. Use -3 for cons[speed] in adjustable variables.
						2. Use nmb_hcio_brds instead of fixed value to correct for highrise cars.
1/7/08	v5.40.1 mhd		1. Modified elevator_service to check for servf on fire service and to show door status on earthquake service if door are closed.
						2. Modified front and rear door display to show proper door location when on fire service.  When both front and rear are closed,
						   show door closed and not rdor closed.
1/15/08	v5.40.2 mhd		1. Add variable "RCM Hold" to hold retiring cam at valid floor until a door open is requested.
1/25/08 v5.40.3 mhd		1. Changed name of RCM_HWDoorCL to RCM_HWDoor. Change variable RCM HOLD TO RCM Control added bit 2 for advanced RCM when RCM_HWDoor set.
2/5/08 mhd				2. Changed "DCL Input Off" to "Looking for DCL".
1/30/08 v5.40.4 mhd		1. Allow brake lift on inspection with door open.  Make user press two additional keys to verify. 
						2. Change hall addres # from 32-44 to B6-B18 to reflect the jumper configuration.
						3. Added variable for recall from fs1 alt floor fvrecallfs1a
2/4/08 v5.41 mhd		1. Added variable for automatic learn hoistway stall time.
						2. Added automatic learn hoistway.
						3. Check that pulses increment every two seconds.	  
						4. Corrected io status for hall calls from 26 to 37 for highrise cars.
3/1/08 v5.41.1 hdl		1. Restructured the LCD interface for changing the date. Now you can change year by units, tens hunderds and thousands. Also no invalid years can be entered.
3/11/08 v5.42 mhd		1. Added new brake parameters, brake pick start, brake relevel start, brake pick rate and brake relevel rate.
						2. Added short floor slowdown timer and Mid short floor slowdown timer for hydro and traction ndf.
3/20/08 V5.42.2 mhd		1. Added short floor up and down slowdown timers for short and mid short floors.  Changed current timers to down and added ones for up.
3/21/08 V5.42.3 mhd		1. Added HS Elev Off Floor, GOV Grip Trip, PI Serv Msg 1, and PI Serv Msg 2 variables.
3/24/08 v5.42.4 mhd		1. Added variables for Short Floor Soft Start, Short Floor Accel rate, Short Floor Roll Over Jerk, Short Floor Decel Jerk, Short Floor Decel Rate and Short Floor Targ Distance.
3/25/08 V5.42.5 mhd		1. Added short floor control variable.
4/1/08 v5.43 mhd		1. Modified brake lift on inspection to swich screens if safe() is picked or dropped.
						2. Moved inspection door close to service options.
4/17/08 v5.43.5 mhd		1. Added EP recall delay variable.
4/18/08 v5.43.6 mhd		1. Modified stop at lobby parameter display.
						2. Added Group override security timer.
4/25/08 v5.43.8 mhd		1. Corrected debounce for lcd buttons.
5/16/08 v5.44 mhd		1. Added hc on and off brightness variables.
						2. Changed min brake pick and hold voltage to 20 and min brake relevel voltate to 10.
6/10/08 v5.44.1 mhd		1. Changed ul or dl off fault to ul and dl off fault.
6/30/08 V5.44.5 mhd		1. Reversed serial io hall calls order to start with 1D at the top of display so get_io_status() had to be modified.
						2. Changed pf3 to dv2 for device 2 in error log and prog_flag3 to device_2.
7/3/08 v5.45 mhd		1. Reset gripper latch only if no SPB comm error and the SPB does not have an Unintended Motion fault.
						2. Added parameters fvacctopfl, and fvaccbotfl.
						3. Corrected last_var[] for service option on tract ndf. 
7/10/08 v5.45.1 mhd		1. Added variable fvars[fvppstop] to enable stop on count.
7/22/08 v5.45.2 mhd		1. Deleted DPP_DZU_Count[], DPP_DZD_Count[] and DPP_FL_Offset[] to use DZU_Dist, DZD_Dist, Up_fl_level_dist and Dn_fl_level_dist instead.
8/15/08 v5.45.5 mhd		1. Add parameter for sabbath restart time fvars[fvsabtim].
8/20/08 V5.45.6 mhd		1. Added Security Recall as service 24.
9/2/08 v5.45.7 mhd		1. If no valid front from during buffer test the set car call for rear floor.
9/24/08 v5.46 WC/mhd    1. Automatically display fault on LCD  when it occurs; Function Refresh_LCD in LCD.cc was modified
                           as well as the function record_faults in control.cc
						2. Adjustable varible fvfltdpy in globle.cc and control was to created to turn on or off the above feature
9/29/08 v5.46 mhd		3. Added reset low oil from LCD display for 2007 code.
						4. Added GOV reset.
10/2/08					5. Added behind car call cancel variable.
10/15/08				6. Delete clearing LCD_State_ix when password is not being checked i.e. (fvars[fvpassword] == 0).
10/15/08 v5.46.1 mhd	1. Reset LCD_Menu to known state in ReturnLCDMenu if LCD_State_ix == 0.
						2. Jump to powerup.exe routine same as a traction of hydro running on ts5300 cpu.
10/20/08 v5.46.2 mhd	1. Added advance door enable timer for non-simultaneous doors.
10/21/08 v5.46.3 mhd	1. Added variables, Group service parking, Binary Preset Always, Short floor hsf without hsv. (only use Binary Preset with this version)
11/4/08 v5.46.5 mhd     1. Added Service Timers.
11/18/08 v5.46.6 mhd	1. Changed baud rate for drive.
						1. Added modbus protocol parameter.
11/28/08 v5.46.7 mhd	1. Added Adjust Drive Parameters routine for delta drive.
12/15/08 v5.46.8 mhd	1. Added floor offset back into table. see display_fl_off().
1/12/09 v5.47.1 mhd		1. Change the loop when displaying HC serial i/o board to check all the group devices each time.
						2. Also display the correct hall call driver board number instead of always 1.
1/14/09 v5.47.2 mhd		1. Added get io status check for gpio board 9 on non HR car in get_io_status() routine.
						2. Corrected io status for case 20: in grp_io_status.
						3. Corrected diagnostic display for hc comm boards.
1/20/09 v5.47.3 mhd		1. Added version and revision for serial hall call boards.
2/02/09 v5.47.4 pn		1. Added Sabbath feature as service 25.
2/9/09 v5.47.5 mhd		1. Added variable "Alt Rcl FS Off" (fvaltrclfs) to allow car to return to fire alternate floor after car has returned to main recall floor from FS switch 
						   and FS switch is then turned off.
2/10/09 v5.47.6 mhd		1. Added car call push button security code entry and service activation.
						2. Added Day of the week display under date and time menu.
3/1/09 v5.47.7 mhd		1. Made corrections to car call pushbutton security user input.
3/10/09 v5.47.10 mhd	1. Added emergency power recovery mode.
						2. Added mechanism to keep the ul and dl offsets on a hoistway learn.
						3. Allow interim version to be two digits.
						4. During auto hoistway learn, check for (LCD_Sub_Point != 13) to disable mode button to prevent car from contiuning to run 
						   if LCD_Menu == 0 and mode button pressed while going down looking for DN.
						5. Added parameter for inspection decel stop rate.
3/19/09 v5.47.11 mhd	1. Added Ep_Recov_Vel for emergency power recovery velocity.
						2. Added Em_Lev_Decel Emergency Deceleration to Leveling Rate.
						3. Added Cycle Run time.
4/1/09 v5.48.1 mhd		1. Modified adjust_variable() routine to not treat -1,-2 and -3 specail for min and max values.  This is done in utils.cc only.
4/22/09 v5.48.4 mhd		1. Added hall call red, green and blue on and off parameters.
5/15/09 v5.48.5	hdl		1. Added field variable and ouput for Door open light. Options FireRecall complete, return to lobby complete and emp = 4 home doors open
5/18/09 v5.48.6 mhd		1. Added power loss brake control in Elevator Setup menu.
						2. Corrected adjustment of slowdown limits above UT2 and DT2.
5/20/09 v5.48.7 mhd		1. Added diagnostic display for ups comm and status.
						2. Corrected get_io_status to use gc_uc_dsr[], gc_dc_dsr[], etc. instead of gc_uc[] and gc_dc[], etc.
5/29/09 v5.48.8 mhd		1. Added parameters for hall call button colors.
						2. Added hoistway sensor 2 return floor variable.
						3. Display fire service software variables on status screen.
6/3/09 v5.48.9 mhd		1. Added fault code for ul,dl setup count fault.  If rpm is incorrect the calculation for 8 inches is incorrect.
						2. Corrected display for UPS status.
						3. Deleted brake time-out for power loss brake pick. (Car moves very slow).
						4. Corrected UPS status bit test.
6/12/09 v5.48.11 mhd	1. Added fault for EP Recall time-out, EP Recall ots and SPB SFC fault.
						2. Added Drive speed multiplier parameter. 
7/20/09 v5.49.1 mhd		1. Added errors for up and down normal limit setup.						
7/30/09 v5.49.5 mhd		1. Added parameter for advanced pre-torque.
8/1/09 v5.49.6 mhd		1. Added HCB Reset and Init command in diagnostic display.  Rearranged diagnostic display to show hall call board status after group status.
8/8/09  v5.49.7 mhd		1. Changed HcbControl to an array to send control data to all hall call driver boards.
8/11/09 V5.49.8 mhd		1. Changed bit in HcbControl for reset from bit 4 to bit 0x80.	(Will only work with version 2.01 and higher driver board.)
						2. Added selector preset fault code.
8/28/09 V5.49.11 mhd	1. Added car call light color and brightness control variables.
9/1/09 v5.49.11 wc		1. LCD_Pointer corrected in diagnostic DN button if drive comm and serial hall buttons.
9/10/09 v5.49.11 mhd	1. Added Governor trip function.
9/15/09 V5.50 mhd		1. Modified gripper reset display, parameters and error codes to better suit the Emergency brake. see EBK.	
9/30/09 v5.50.3 mhd		1. Added Sabbath control variable.
10/7/09 v5.50.4 mhd		1. Added detector edge time-out fault.
10/12/09 v5.50.4 mhd	1. Added parameters for low line voltage and low door voltage.
						2. Added faults for L1, L2, L3 and Door low voltage.
10/21/09 v5.50.5 mhd	1. Re-arranged menu system to add Modified Motion, Pushbutton Option and System Options.

11/25/09 v5.50.8 hdl	1. added two field variables, fvsabuzt and fvsadt. One is the timer for the buzzer to go off
							before doors close. The second is a dwelling time that engages once the sabbath operation is ON	
12/31/09 v5.50.10 hdl	1. added function reinit_LCD to reset the display in the event that it gets garbage on the screen
							you need to press Mode -> Up -> Down -> Up -> Down.
						2. Corrected LCD load weigher routine. Issue: if no comm to load weigher board,
							the display will say complete, get frozen and you could not exit out to the main menu
12/9/09/v5.50.11 mhd	1. Modified Power Loss Brake lift to use the MBP to turn off after time delay to shut off UPS.
						2. Added error codes for run inhibit from reset count, at floor shutdown and emergency motion exit from inspection.
						3. Added hot oil fault error display message.  
12/28/09 v5.51.2 mhd	1. Added ee cancel dwel parameter.
						2. Added lobby dwell and lobby off to Sabbath Enable Control variable.
01/9/10 V5.51.4 mhd		1. Added EP recover move select count and at floor select variables.
1/26/10 v5.51.9 mhd		1. Changed check for PWL_Brake == 0 to != 1 to allow for PWL_Brake == 2. 
						2. Changed PWL status and faults to UPS.
						3. Send command to SPB to display car speed when lifting brake on inspection.
						4. Show comm status for Eaton UPS_Type == 1.
2/8/10 v5.51.10 mhd		1. Added parameter to assign parking floor to car with door open.
3/12/10 v5.51.14 mhd	1. Added security activation timers for Floor security configurations 2, 3, and 4. 
3/19/10 V5.52 mhd		1. Changed SEB #18 label to CC1 #18.
3/23/10 v5.51.16 mhd 	1. Added variables for Sabbath start at lobby and to use cab lanterns for direction arrows.
3/24/10 v5.52 mhd		1. Added fault messages for RGB Car Call Board.
3/29/10 V5.51.1 mhd		1. Added variables fvclantctl (cab lantern control), fvmaxdoorhld (max door hold time), and fvaccdoorcl (access door close).
4/2/10 v5.52.2 mhd		1. Show door status when servf == 0 and procf >= 14 and procf <= 16.  Car level at the floor and out of service.
5/3/10 5.52.7 mhd		1. Added error code for hall call low supply voltage.
6/7/10 V5.52.9 mhd		1. Increased number of variables from 360 to 420. 
						2. Added parameters for hall call light control for up and down, ir, code blue and vip.
						3. Add CB,Vip & HSec Call Light menu item, renamed Car Call menu to CC & COP Lights, and renamed Hall Call menu to HC & IR Call Lights.
						4. Added parameters Vip lantern control and Group Car Call Override Control.

6/21/10 v5.52.10 hdl	1. Added 3 exclusion fault field variables and control variables for each (6 fvars). You can choose not to display faults based on ctrl
						2. Changed logic for retiring cam to attempt 4 times to make up locks before declaring a fault and shutiing down
						3. Added RCM / lock fault
6/24/10 v5.52.11 mhd	1. Corrected label for hall call address from Hall #C21 and C23. 
7/1/10 v5.52.11 mhd		1. Added new electronic emergency brake parameters
7/8/10 v5.52.14 mhd		1. Added LCD_HCB_Com_Fault[] variables for Up Input Overload, Dn Input Overload, Aux Up Input Overload, and Aux Dn Input Overload. 
7/29/10 v5.52.19 mhd	1. Modified get_io_status() to add code blue second riser inputs.  
						2. Added parameter for second riser code blue car, for ir car code blue enable and ir code blue penalty time.

8/10/10 v5.52.22 hdl	1. Exclusion faults were not displayed properly. Modified Display_Var_Label()
8/24/10	v5.52.26 hdl		1. Added INSEC output and fvars[fvinsec] for inverting the output. 
							It outputs whether the in car security is enabled or not
9/8/10 v5.52.29 mhd		1. Added Encoder Dbn Interval parameter.
10/6/10 V5.52.36 mhd	1. Added modbus comm to Yaskawa L1000 drive (drive_type[]=15).
						2. Added service ups parameter to allow the ups to be disconnectd from the comm while it is being serviced.

10/18/10 v5.52.58 hdl 	1. The label for emergency drop rate was incorrect. Changed from 5 to 1.5

10/25/10 v5.52.39 hdl	1. Added the menu under Software version to back up flashcards to the flashcard and recover from there
10/25/10 V5.52.41 mhd	1. Added two additional I/O board on can bus.
11/11/10 V5.52.46 mhd	1. Added security variables to activate modified express service security for hall calls on individual cars.	 See Hall Call secure direction.
11/19/10 V5.52.47 mhd	1. Added UPS baud rate parameter.
11/23/10 v5.52.48	hdl	1. Added fvhcattbuz to allow car to aknowledge hall calls bu buzzing for 1 second every new call comes in. To be used in attendant cars
11/28/10 v5.52.49 mhd	1. Deleted all parameters to activate modified express service except for Hall Call assignment secure type.
1/11/11 v5.52.56 mhd	1. Added encoder preset fault.
						2. Delete Delta Drive.
2/2/11 v5.52.58	hdl		1. Changed the max for fveppkled from 1 to 2. It allos the PRK led to come on only when teh car is at the emp floor 

3/16/11 v5.52.09 hdl	1. in find io status the labels for the group security board were off by one on the second board fpr group security . 
							changed index from 9 to 8 in case 35
5/20/11 v5.54.02 mhd	1. Added parameters for direction check and ee memory location.
7/8/11 v5.54.11 mhd		1. Fixed menu index for Elevator Setup menu for hydro when using GOV reset on roped hydro.
7/11/11 v5.54.12 mhd	1. Allow OSERL OutCtrl 2 to be changed to zero. Set to 1 gives control to OSERL Outctl 1.
7/12/11 v5.54.14 mhd	1. Added variables for MED door reopen and MED independent override.
7/26/11 v5.54.17 mhd	1. Added Base count and encoder init count to display.
				 hdl	2. Added user message 3 parameter.

9/22/11 v5.54.25 mhd	1. Added 1st recall and select car parameters for second power feeder

10/13/11 v5.54.32 mhd	1. Add menu item to reset debug variables in Software Version menu.

11/29/11 v5.54.40 hdl	1. Changed display hoistway tables routine to subtract DPP_DN instead of 5000. 
						All limit switches distances in feet were off by 13 feet since we are not starting at 5000 anymore.

01/03/2012 v5.54.46 pn	1. Changed Exclusion fault feature from 3 to 6 and eliminated the exclusion fault control option for each exlusion fault.								
1/30/12 v6.0 mhd		1. Added parameter and fault for velocity difference check between SPB velocity and Encoder velocity.
						2. Added seperate flag for encoder velocity unintended motion in fvspbgrip variable.
						3. Deleted parameter fvdisglt that allows the gate and lock check for NY to be disabled.

2/1/12 v5.54.53 mhd		1. Added parameters fvdobovrdcb	and fvdenfsv to allow DOB to override DCB on phase 2 (for Miami) and special recall operation for Denver to close the doors after
						   the car recalls on phase 1.  Doors are reopened from the hall call button at the recall floor.
						2. Added parameter fvf1dwell for denver phase 1 dwell time
3/8/12 v6.0.1 mhd		1. Changed SPB Vel Difference Fault to Slip Detect Fault. Also added Slip Vel Difference Disable parameter.
3/8/12 v5.54.62 mhd		1. Changed SPB Vel Difference Fault to Slip Detect Fault. Also added Slip Vel Difference Disable parameter.

3/14//12 v5.54.63 hdl	1. Added check for emergency breake when resetting emergency brake so we could turn on UTM in LCD routine 
5/1/12  v6.0.7 mhd		1. Check for gripper fault during hoistway learn.
5/1/12 v6.0.7 mhd		1. Added new brake control cons[EM_BRK] == 7.  This is to have a controled drop of the emergency brake even after MC drops.
8/14/12 v6.0.16 mhd		1. Added Auto_Learn_Hoistway(1) when LCD_Sub_Point == 14 to make sure that learn run command turns off. 
						2. Also in auto learn, changed LCD_Sub_Point = 16 from 15 in down check when LCD_Sub_Point == 13 to give the correct error code.
9/17/12 v6.0.21 mhd		1. Added motor rpm for keb drive.
						2. Added modification for velocity display for power loss brake lift.
1/4/13 v7.0.0 mhd		1. Added Riot Control service when cons[prison] == 2, servf = s_RIOT from v5.55.15 atl_fed_pen.
2/13/13 v7.0.1 mhd		1. Added emergency power manual select (fvep_selsw_rcl) and acces speed (fvaccspd) paremeters.
9/3/13 v7.0.18 mhd		1. Changed Display_Var_Label() routine to use the variable index number instead of the menu index number to identify which variable needs to display a label.
						   The reason for this change is that when a new variable is added to the middle of a menu, the changed in index number no longer matters.
						2. Added keb display type parameter to select new keb display instead of using a different drive number.
						3. Added parameter to select the encoder type instead of using the cons file parameter.
9/12/13 v7.0.19 mhd		1, Seperated Elevator_Setup from lcd.c.
						2. Added parameter for single or 3 phase brake input voltage.
						3. Moved elevator_setup() to a seperate module.
9/30/13 v7.0.21 mhd		1. Added check for inspection to read or write to eeprom. (Previously commented out)
						2. Added distance feed forward parameter.
10/29/13 v7.0.24 mhd	1. Added parameters 516 and 517 below to match GALaxy 3. .
							  "SR CCSec by Dir=        ",	// 516 C Second Riser car calls security by direction 
							  "Min Door Tim En=        ",x	// 517 C Minimum short door time enable

11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 where necessary to allow any car to be the group.
						2. Replaced iodata[1] with iodata[cons[carnmb]] where necessary.
11/27/13 V7.0.30 mhd	1. Replaced 4 instances of Sys_Stat_Size with MRAM_Sys_Stat_Size
						2. Added variable for fire reset 2.
						3. Added variable to Invert HC security inputs
1/20/14 v7.0.32 mhd		1. In diagnostic display added "to" to grcan, ctcan and mrcan such as "Group GRCAN to SEB #1"
3/21/14 v7.0.37 mhd		1. Display additional fault and status data from fault display and elevator status.screen.
6/26/14 v7.1.4 mhd		1. Displayed inspection service when on inspection.
7/23/14 v7.1.6 mhd		1. Added additional faults
11/10/14  V7.1.20 mhd	1. When field variable changed, send data to galcom device.
11/25/14 v7.1.21 mhd	1. Write field variables (limit values) when limit velocities are initialized.
12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
							12/02/13 v6.1.18 as		1. Added Trace options under software utilities to start and initialize trace, or clear trace
							9/12/14 v6.1.76 as		1. Added additional I/O board to Can_Device_Name[].
							10/7/14 v6.01.87 as		1. Added below changes related to G3 software:
													9/26/14 v6.1.87 cl		1. Added "COP/Remote Dis" to Car options 
																			2. Change Indepent security description to "+1=CCS +2=SECF +4=COP   " and change max to 7
							11/5/14 v6.1.95 as		1. Modified get_io_status() routine to show correct IO status when we have cons[CB_SR] for io board 52 and 53.
2/26/15 v7.1.27 mhd		1. Set the minimum time on the password time-out to 10 seconds.
4/6/15 v7.1.29 mhd		1. Moved IO dislay routines to a seperate file (iodpy.c).
6/19/15 v7.1.35 mhd		1. Changed EBK1 faults to EBK and EBK2 faults to EBK1 to match IO names.
7/13/15 v7.1.38 as		1. Added printing option " 3 = 115200 Baud        " on the Drv_Baud_Display array.
9/17/15 V7.1.41 mhd		1. Added earthquake reset to display.  
						2. Fixed hall call driver board on-line in Diagnostics HC Boards Comm Status.
11/18/15 v7.1.48 as		1. Added sending packet 109 when Date and Time is updated on the dispatcher through LCD.
11/23/15 v7.1.50 mhd	1. Change fault code Learn HW Safe Fault to Hoistway Safe Fault so that it can be used for both Hoistway learn and brake test.
12/21/15 v7.1.54 tb		1. Added delimiters to LCD_Fault[] to return fault string from array when searching for "[faultnumber]F -".
						2. Changed LCD_Par for parameters 186 (fvheloff) and 528 (fvceloff) to match with GALaxy 3:
							"+1=RC+2=DO,+4=CLFof+8=DC",   // 186 - Hall Elevator Off options
							"+1=RC+2=DO,+4=CLFof+8=DC",   // 528 - Car  Elevator Off options
1/4/16 V7.1.56 mhd		1. Added "Parameter Setup Flt" to device_fault table.
						2. Added "PAR ER" to spb_service.
01/12/16 v7.1.62 tb		1. Changed LCD_Par and LCD_Field_Vars strings for fvheofovrind to match G3.
*/
