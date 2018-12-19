#define d_VIDEO 1
#include "global.h"
#include <stdarg.h>
#define Quick_Update 1


// Variables for Video Screen
int16 dpy_mode=0;			// Selects the display screen
						// 0 = menu
						// 1 = standard job screen
						// 2 = Job statistics
						// 3 = Security display
						// 4 = debug display

int16 video_timeout = 0;		//	video timout
int16 save_dpy_mode = 0;
int16 visible_cmd = 0;
int16 video_off = 0;
int16 video_exist = 0;
int16 Car_Select = 1;
int16 Car_Select_MRdpy;
int16 Car_IO_Select = 1;
int16 io_page = 0;
int16 grp_page = 0;
int16 nmbhcb = 0;
int16 next_page_hcb_2 = 0;		// hall call board number to start page	2

int16 next_page_hcb_3 = 0;		// hall call board number to start page 3
int16 next_page_hcb_4 = 0;		// hall call board number to start page	4
int16 next_page_hcb_5 = 0;		// hall call board number to start page	5
#if (GALaxy_4 == 1)
const int16 fixed_io_brd_dpy = 5;
#else
const int16 fixed_io_brd_dpy = 6;
#endif
int16 nmbgio_page = 0;		// Number of group io pages
int16 dbg_page = 0;
int16 grtop_rfl=0;
int16 grbot_rfl=0;

#if (Tract_HR == 1)
int16 page_2_brd_nmb[car_size]={0,0,0,0,0,0,0,0,0};
int16 page_3_brd_nmb[car_size]={0,0,0,0,0,0,0,0,0};
int16 page_4_brd_nmb[car_size]={0,0,0,0,0,0,0,0,0};
int16 page_5_brd_nmb[car_size]={0,0,0,0,0,0,0,0,0};
int16 max_nmb_io_brd[car_size]={0,0,0,0,0,0,0,0,0};
int16 iobd_rotate_start[car_size] = {0,0,0,0,0,0,0,0,0};
int16 last_io_brd[car_size]={0,0,0,0,0,0,0,0,0};
#else
int16 page_2_brd_nmb[car_size]={0,0,0,0,0,0,0};
int16 page_3_brd_nmb[car_size]={0,0,0,0,0,0,0};
int16 max_nmb_io_brd[car_size]={0,0,0,0,0,0,0};
int16 iobd_rotate_start[car_size] = {0,0,0,0,0,0,0};
int16 last_io_brd[car_size]={0,0,0,0,0,0,0};
#endif

int vid_test = 0;
int flt_case = 0;
int peter = 0;
int set_memory_variable = 0;
int enter_key = 0;
int16 menu_cmd;
int16 trace_num;
int16 trace_dpy_ix = 0;
int16 trace_screen_state = 0;
int16 trace_init_screen_state = 0;
int16 Display_Trace = 0;
int16 Prev_Display_Trace = 0;
int16 next_trace = 0;
int8 set_dpy_ix = 0;
int8 trig_dpy_ix = 0;
int8 end_dpy_ix = 0;
int16 sub_state = 0;
int16 v_sub_state = 0;
int16 Trace_Track_Rotate_Ix = 0;
int16 Prev_Trace_Track_Rotate_Ix = 0;
int16 Track_Rotate_State = 0;

extern struct Fault S_Trace[];


int16 fault_dpy_ix;
int16 fault_ix_start;

/* 
// Variables for IGEP
extern int16 igep_car_recall_order[2][9];	// Car return order
extern int16 igep_car_select_order[2][9];	// Car selection order
extern int16 igep_car_man_sel_req[2][9];	  	// Manual selection request
extern bool car_fire_sw_on[2];

extern const int icr_loc[30]; 
extern const int ricr_loc[30]; 
extern const int epio_loc[30]; 
*/

#if ((Traction == 1) || (Tract_OL == 1))

int16 drv_par_group = 0;
int16 tst_drv_par_ret = 0;

			//         0, 1, 2, 3, 4, 5, 6, 7, 8,9,10,11,12,13
int16 grp_par_max[14] = {15,30,34,19,15,20,28,28,10,9,21,18,31,31};

#endif


static int16 debug_screen_init = 0;
static int16 bar=22;
static int16 Vertical_line=179;
static int16 doub_vert_line=186;
static int16 Car_Left=221;
static int16 Car_Right=222;
static int16 Car_Fdoor_Open=223;
static int16 Car_Door_Closed=219;
static int16 Car_Rdoor_Open=220;
static int16 Car_FRdoor_Open = 32;
static int16 Up_Arrow=30;
static int16 Dn_Arrow=31;
static int16 Car_Call=4;
static int16 update_dpy;
static int16 upd_dpy_req;
static int16 board_nmb;
static int16 hcdb_add_offset = 0;
static int16 tbl,col,row;
static int16 kb_col,kb_row;
static int16 flt_ix, dpy_flt, dpy_loop;
int16 max_car_row,max_window_row,max_row;
int16 Vid_Car_Xpos[9];                    /* this is the x pos in carbox for cars */
static int16 v_old_Car_Select,v_old_dirf,v_old_dpref;
static int16 v_old_procf,v_old_firef,v_old_pos,v_old_doorf;
static int16 v_old_rdoorf, v_old_servf;
static int16 v_old_cur_flt, v_old_enc_dir;
static int32 v_old_statusf,v_old_statusf2;
static int16 v_old_ss_statusf;
static uint8 v_ins_servfv;

int16 upd_procf;
static int16 in_fire_door;

int16 vc_state = 0;

static char Dev_Name[10];

static char kb_buff[60];
static int16 kbp, echo, prev_echo;
static int16 key_entry;
int32 debug_variable[10];			 		
int32 dump_num[3], dump_len[3], dump_status[3];
int32 var_off = 0x40000000;
static int16 wnum;
static int32 num[5];

#define Run_State 0
#define Reset_State 1
#define Preset_State 2
#define turn_video_off 1
#define turn_video_on 0
#define Display_hc_io_add 1 


static int16 Prev_Car_Select;
static int16 Prev_Display_Fault;
static int16 prev_grp_page;
int16 group_dpy_init = 0;
int16 io_screen_init = 0;
int16 io_grp_screen_init = 0;
int16 freeze = 0;
int16 keybrd_found = 0;
int16 menu_init = 0;
int16 max_menu_items = 0;
int16 menu_ix = 1;
int16 O_menu_ix = 1;
int16 Call_Pointer_Y = 0;
int16 Call_Pointer_X = 0;
int16 arrow_key = 0;
int16 detailed_fault = 0;

int16 iobd_rotate_ix = 0;

int16 mem_type = 0;
int16 mem_page = 0;
int16 mem_offset = 0;
int16 mem_len = 0;
int16 next_mem_page = 0;

struct Fault *flp;

struct galtime tx;
struct galdate dx;

#if ((Traction == 1) || (Tract_OL == 1))
static union {
		unsigned char ib[2];		// int byte 0 - 1
		uint16 ii;		// int int
	}t2;
#endif

// no longer used in drive comm so this can be changed to a different variable in the future
int32 drv_com_source = 0;

void menu_control (int16 init_flag);
void hidden_kbrd_control (void);
void debug_control (void);
void debug_cmd (int16 len);
int16 LocNonSpace(int16 len);
int16 sel_car_num (int16 ms_seq);
int16 are_you_sure (int16 ms_seq, int16 *cmd);
int16 time_date (int16 ms_seq);
int16 valid_kbd_num(int16 ix, int16 j);
int16 input_date(void);
int16 input_time(void);
int16 popup_msg (int16 ms_seq, char *msg);
void init_menu_screen (int16 init_flag);
void job_stats_screen(int16 init_flag);
void video_time_out(int16 video_mode);
void Init_Video(void);
void Track_Screen(int16 Car_Select);
void rotate_io_bd (int16 dir);
int16 find_brdnmb(int16 Car_Select, int16 brd_ix);
int16 get_comm_status (int16 board_nmb);	//       0 = no comm, 1 = comm with board,  2 = comm on previous board
void Init_Io_Screen(int16 Car_Select, int16 init_flag);
void Io_Screen(int16 Car_Select,int16 init_flag);
int16 Trace_Track_Screen(int16 Car_Select, struct Fault *ptr, int16 state);
int16 Trace_Track_Screen_1(int16 Car_Select, struct Fault *ptr, int16 state);
int16 Trace_Track_Screen_2(struct Fault *ptr, int16 state);
int16 Trace_Track_Screen_3(struct Fault *ptr, int16 state);
int16 Trace_Track_Screen_4(struct Fault *ptr, int16 state);
int16 Trace_Track_Screen_5(struct Fault *ptr, int16 state);
int16 Trace_Track_Screen_6(struct Fault *ptr, int16 state);
int16 Trace_Track_Screen_7(struct Fault *ptr, int16 state);
int16 Trace_Io_Screen(int16 Car_Select, int16 state, struct Fault *ptr);
int16 Init_Trace_Io_Screen(int16 Car_Select, int16 state);
int16 Init_Vertical_Lines(int16 state);
int16 Init_Trace_Block(int16 state);
int16 Init_Trace_Block_1(int16 state);
int16 Init_Trace_Block_2(int16 state);
int16 Init_Trace_Block_3(int16 state);
int16 Init_Trace_Block_4(int16 state);
int16 Init_Trace_Block_5(int16 state);
int16 Init_Trace_Block_6(int16 state);
int16 Init_Trace_Block_7(int16 state);
void Video_PI(int16 car_no, int16 init_flag);
void Mem_dump_screen(int16 init_flag);
void Init_Grp_Io_Screen(int16 init_flag);
void Grp_Io_Screen(int16 init_flag);
void Init_Group_Hall_Display(int16 init_flag);
void Init_Group_Hall_Display_44(int16 init_flag);
void Init_Group_Hall_Display_60(int16 init_flag);
void Group_Hall_Display(int16 init_flag);
void Group_Hall_Display_44(int16 init_flag);
void Group_Hall_Display_60(int16 init_flag);
void Fault_Screen(int16 Car_Select, int16 init_flag);
void Init_Debug_Screen(int16 init_flag);
void Debug_Screen(int16 init_flag);
int16 drv_screen(int16 state);
#if (GALaxy_4 == 1)
int16 mrcan_screen(int16 state);
int16 ctcan_screen(int16 state);
#else
int16 can_screen(int16 state);
#endif
int16 keyboard_debug_screen(int16 state);
int16 group_debug_screen(int16 state);
int16 igep_debug_screen(int16 state);
#if(Traction == 1)
int16 pulse_count_update_screen(int16 state);
#endif
int16 scan_cmd(void);
void put_trace(int16 row, int16 col,int16 num);
int16 get_numbers(int16 i);
int16 get_hex_numbers(int16 i);
int32 get_num(int16 buf_st);
void clr_windows(int16 wnum);
void set_font(unsigned char font);

int16 vclear(int16 v_w, int16 vc_state);
int16 vcreat(int16 row_len, int16 col_wid, int16 attr);
void vdelete( int16 v_w);
void vratputc(int16 v_w, int16 row, int16 col, int16 attr, char c);
int16 valid_window (int16 v_w);
void vframe (int16 v_w, unsigned char attr, int16 frame_type);
void vlocate ( int16 v_w, int16 row, int16 col);
void vdelete( int16 v_w);
void vratputf( int16 v_w, int16 row, int16 col, int16 attr, char *fmt,...);
void lcurset (int16 v_w, int16 row, int16 col);

void place_car(int16 car_no, int16 pos, int16 attr);
void clear_car(int16 car_no, int16 pos);
void place_hall_call(int16 fl, int16 type);
void clear_hall_call(int16 fl, int16 type);
void place_assign_call(int16 fl, int16 car, int16 type);
void clear_assign_call(int16 fl, int16 car, int16 type);
void Video_PI_60(int16 car_no, int16 init_flag);

void Set_Color(uint16 color);
void ScreenPutChar(uint8 c, uint16 color, int16 col, int16 row);
void ScreenPutString(char *p,int16 attr, int16 col, int16 row);
void setcursortype(int16 cur_t);
void ScreenSetCursor (int16 row, int16 col);
void ScreenClear (void);

void video_display_control(void);

int16 get_keybrd (int16 row_st, int16 col_st, int16 max_len, int16 echo,int16 usr_win);
int16 get_arrow_keybrd (void);

void GrpHall_PosInd_MOVE(void);
void GrpHall_PosInd_SET(int16 fl,int16 car);
void GrpHall_PosInd_CLEAR(int16 fl,int16 car);
int16  UpHC_PosInd = 0;			// Up hall call position indicator
int16  DnHC_PosInd  = 0;			// Down hall call position indicator

//////////////////////////////////////////////////////////////
//**********************************************************//
//       IO SCREEN CAR DISPLAY MESSAGES data structures     //
//**********************************************************//
//////////////////////////////////////////////////////////////

struct lcD_line_1_struct{
	char dpy[50][25]; 
	unsigned char chksum[5];
	int16 nmb_items;
};

struct lcD_line_2_struct{
	char dpy[300][25]; 
	unsigned char chksum[30];
	int16 nmb_items;
};

struct lcD_line_3_struct{
	char dpy[600][25]; 
	unsigned char chksum[60];
	int16 nmb_items;
};

struct IO_screen_lcD_struct{
	struct lcD_line_1_struct line1;
	struct lcD_line_2_struct line2;
	struct lcD_line_3_struct line3;
};

struct IO_screen_lcD_struct IO_scn_lcD;

void init_IO_scn_lcD (void);
void calculate_IO_scn_checksums(void);

 
const char car_status[10][81] = {
"Automatic Service   | Dmd Vel =      fpm | Pos =     PI =    | Run Cmd  =       ",
"Status: OK          | Enc Vel =      fpm | Flr Cnt =         | Enc Dir  =       ",
"Waiting Assignment  | Vel Diff=      fpm | Pls Cnt =         | Dir Pref =       ",
"Fire Off:Normal Svc | Drv Vel =      fpm | predof=0, estop=0 | Drv Cmd= OFF     ",
"No Current Fault    | Vel Calc=      fpm | startf=0, hsf=0   | DvS=0000,%Ld=000 ",
"Elev Door Closed    | SPB Vel =      fpm | SD = NO           | TrqCmd = 00000   ",
"No Rear Door        | NTS Vel =      fpm | RSD= NO           | MtrTrq = 00000   ",
"Normal Power        | stepf=0,  nstopf=0 | lev_startf=0|                        ",
"Motion Timer = 0000 | dcalls=0, nxstp=00 | relevel=0   |                        ",
"Status: OK          | Enc Vel =      fpm | Targ Cnt=         | Enc Dir  =       ",
};


const char car_status_2[6][81] = {
"sfS10=0|sfGRP=0|sfPOS=0|sfGDL=0|sfBKS=0|sfDBR=0|sfTLM=0|sfFST=0|Device   =      ",
" sfHC=0| sfIO=0| sfAD=0|  sfP=0|sfTOC=0|sfSHD=0|sfGTS=0|sfSEL=0|Device 2 =      ",
" sfSS=0|sfINS=0|sfSTP=0|sfDCL=0|sfDRV=0|sfAST=0|sfDZF=0|sfUDL=0|Prog Flag 1=    ",
"sfRDY=0|sfBPI=0| sfDZ=0|sfDCC=0|sfSPB=0|sfSAF=0|sfBKC=0|sfLEV=0|Prog Flag 2=    ",
"No Med Emergency   | Check Run   =   | ChLev Start =   |                        ",
"No Code Blue       | Check Start =   | Check Door  =   |                        ",
};

const char car_status_3[6][81] = {
"  DLM=0|  LBP=0|  ICI=0|  RGS=0|  GR1=0|   GS=0| velF=0|rUMOT=0|gr1PK=1| bkLV=0|",
"   DL=0|  MRI=0|  ACC=0|  RLM=0|  ETS=0|  DLT=0|posIN=0|sUMOT=0|sMENU=0|rCAN2=0|",
" AUTO=0|   UL=0|  UTS=0|  DLB=0|   UP=0| SFCO=0| actF=0| gr1T=0|brkTG=0| ntsT=0|",
"  GBP=0|  INS=0|  DTS=0|   DZ=0|  DNR=0|PICEN=0|posUD=0| sfcT=0|ebkTG=0| etsT=0|",
"adDIS=0|sUMTV=0| SPB Cnt =             |  LSC=0|ntsTV=0|                        ",
"SPB Vel =      | SPB:AUTO  , OK        | comF=0|etsTV=0|                        ",
}; 

const char car_status_4[6][81] = {
"   UN=0|  UT3=0|   DN=0|  DT3=0|  UPI=0|  unF=0| ut4F=0| dt1F=0| dt5F=0|fVELD=0|",
"   UT=0|  UT4=0|   DT=0|  DT4=0| DNRI=0|  utF=0| ut5F=0| dt2F=0| dt6F=0|fSPVD=0|",
"  UT1=0|  UT5=0|  DT1=0|  DT5=0| NTSD=0| utsF=0| ut6F=0| dt3F=0|fLIMD=0|fUDLD=0|",
"  UT2=0|  UT6=0|  DT2=0|  DT6=0| NTS1=0| ut1F=0|  dnF=0| dt4F=0|fSPBD=0| fLEV=0|",
"NTS Vel =      |isAOF=0|fHWLN=0| fHWI=0| ut2F=0|  dtF=0|                        ",
"NTS Cmd = NTS,ADD | NTS:NORMAL,Run En  | ut3F=0| dtsF=0|                        ",
};

const char car_status_5[6][81] = {
"rsRUN=0| rsUL=0|rsDLB=0|rsDOL=0| rsEE=0|rsHSF=0| rsDO=0|rsB28=0|fCOPC=0|fSLCT=0|",
"rsDNR=0| rsDZ=0| rsGS=0|rsDCL=0| rsSE=0|rsSTF=0| rsDC=0|rsB29=0|fSPBC=0|fSELC=0|",
" rsUP=0|rsDLT=0|rsRLM=0|rDOLR=0|rsEER=0|rLSTF=0|rsDOR=0|rsB30=0|fB2B2=0|fENCC=0|",
" rsDL=0|rsDLM=0|rsRGS=0|rDCLR=0|rsSER=0|rsDZA=0|rsDCR=0|rsB31=0|fB2B3=0|fENCI=0|",
"ncuseq:        | PAL Stat= 0   |isLBP=0|isINS=0|isACC=0|                        ",
"No Seismic     | PAL Vel= 0    |isGBP=0|isMRI=0|isICI=0|                        ",
};

const char car_status_6[6][81] = {
"sfHWI=0|sfDVF=0|sfPWL=0|s1036=0|sfECN=0|sfNIT=0|sfVSC=0|sfEES=0|fDVOF=0|fB3B4=0|",
"sfFDC=0|sfBKR=0|sfEBK=0|sfRSR=0|sfIOT=0|sfDRQ=0|sfLVR=0|sfERS=0|fDBRT=0|fB3B5=0|",
"sfRDC=0|sfDMO=0|sfKEB=0|sfSCT=0|sfGRT=0|sfDPM=0|sfTSP=0|sfB30=0|fMTOL=0|fB3B6=0|",
"sfLVF=0|sfHWL=0|sfAFS=0|sfCOP=0|sfDVP=0|sfRPM=0|sfSPR=0|sfB31=0|fB3B3=0|fB3B7=0|",
" doRQ=0| cdRQ=0|rdoRQ=0|rcdRQ=0| ngUP=0| ngNP=0|rngDP=0|                        ",
" dbRQ=0| doEN=0|rdbRQ=0|rdOEN=0| ngDP=0|rngUP=0|rngNP=0|                        ",
};

const char car_status_7[6][81] = {
"ssGOV=0|ssHSS=0|ssFFS=0|ssB12=0| psHC=0|psLHC=0|psB08=0|psB12=0|fGRIP=0|fB4B4=0|",
" ssTF=0|ssCTS=0|ssCST=0|ssB13=0|psHCL=0|psFEP=0|psB09=0|psB13=0|fENCD=0|fB4B5=0|",
" ssBF=0|ssCSS=0|ssMRS=0|ssB14=0| psCC=0|psB06=0|psB10=0|psB14=0|fB4B2=0|fB4B6=0|",
" ssPS=0|ssRLS=0|ssGTS=0|ssB15=0|psCCL=0|psB07=0|psB11=0|psB15=0|fB4B3=0|fB4B7=0|",
" dcAB=0|owcAB=0|occAB=0|ohcAB=0| dsUP=0| dsNP=0|rdsDP=0|                        ",
" dcBL=0|owcBL=0|occBL=0|ohcBL=0| dsDP=0|rdsUP=0|rdsNP=0|                        ",
};


const char medf_status[4][19] = {
"No Med Emergency   ",
"Med Emerg Recall   ",
"At Med Recall Floor",
"Med Emerg Service  ",
};

const char codebf_status[5][19] = {
"No Code Blue       ",
"Code Blue Recall   ",
"At Code Blue Floor ",
"At CB Fl Door Open ",
"Code Blue Complete ",
};

const char eqf_status[5][15] = {
"No Seismic     ",
"Ethquake Active",
"Cweight Derail ",
"Recover From CW",
"Equake Shutdown",
};

const char ncu_status[5][8] = {
"No NCU  ",
"OpenDoor",
"DwelDoor",
"ClDoorOC",
"DrTimOut",
};


const char nts_command_dpy[8][7] = {
"NO CMD ",
"EMSD   ",
"NTST   ",
"ETST   ",
"ADD    ",
"NTS,ADD",
"ETS,ADD",
"BUF,ADD",
};


const char car_status_h[3][81] = {
"Automatic Service   | Pos =     PI =     |                                      ",
"Waiting Assignment  | Run Cmd  =         |                                      ",
"No Current Fault    | Dir Pref =         |                                      ",
};

const char brake_status[9][17] = {
" Brake Control  ",
" Req Volt =     ",
" Volt Out =     ",
"  Volt In =     ", 
"  Amp Out =     ",
"Fault Code:     ",
"Jumper Set:     ",
"BKS1 =   BKS2 = ",
"BRBP =          ",
};

const char drive_status[6][17] = {
" Drive Control  ",
" Mtr Amps =     ",
" Mtr Volt =     ",
"  Mtr Trq =     ",
"  Mtr Spd =     ",
"   Mtr Hz =     ",
};

const char empf_status[8][21] = {
"Normal Power        ",
"Emerg Power Hold    ",
"EmPwr Hold Door Open",
"Emerg Power Recall  ",
"EmPwr Home Door Open",
"EP Home Door Closed ",
"EmPwr Run Selected  ",
"EP Hold Door Closed ",
};

static const char trace_nts_lim_stat[10][9] = {
		"Run En  ",
		"Run UP  ",
		"Run Down",
		"SPB ComF",
		"SEL ComF",
		"Dir Flt ",
		"Vel Flt ",
		"EmSD Flt",
		"DnLimitF",
		"UpLimitF",
};
static const char slowdown_dpy[11][3] = {
		"NO",
		"UC",
		"DC",
		"CC",
		"  ",
		"UD",
		"DD",
		"CD",
		"  ",
		"IU",
		"ID",
};


static const char drv_cmd_dpy[5][8] = {
		"OFF    ",
		"RUN    ",
		"RST    ",
		"RUN, UP",
		"RUN, DN",
};

static const char drv_stat_dpy[7][8] = {
		"RDY    ",
		"CRL    ",
		"CRL,RUN",
		"FLT    ",
		"FLT,CRL",
		"FLT,RUN",
		"RUN    ",
};


const char Service[35][20] = {            // service flag
  "Out of Service      ",	// servf = 0 out of service
  "Automatic Service   ",	// servf = 1 full automatic service
  "Independent Service ",	// servf = 2 independent service
  "Load Weighing ByPass",	// servf = 3 load weighing by pass service
  "Attendant Service   ",	// servf = 4 attendant service
  "Code Blue Service   ",	// servf = 5 code blue service
  "Fire Service Phase 2",	// servf = 6 fire phase 2 service
  "Emergency Power Serv",	// servf = 7 emergency power service
  "Earthquake Service  ",	// servf = 8 earth quake emergency service
  "Fire Phase 1 Main   ",	// servf = 9 fire service phase 1 main eggress
  "Fire Phase 1 Alt    ",	// servf = 10 fire service phase 1 alt eggress
  "Homing              ",	// servf = 11 homing operation
  "Reset Going Up      ",	// servf = 12 reset dive up
  "Reset Going Down    ",	// servf = 13 reset dive down
  "Stalled Out of Serv ",	// servf = 14 low oil operation
  "Return to Lobby     ",	// servf = 15 return to lobby
  "Load Overload       ",	// servf = 16 load overload
  "Medical Emergency   ",	// servf = 17 Medical Emergency service
  "Cal. Load Weigher   ",	// servf = 18 Calibrage load weigher 
  "Car Sw Elevator Off ",	// servf = 19 Elevator Off  
  "Hall Sw Elevator Off",	// servf = 20 Hall Sw. Elevator Off 
  "Low Pressure        ",	// servf = 21 Low Pressure 
  "Hospital Service    ",	// servf = 22 Hospital Service
  "Priority Service    ",	// servf = 23 Priority Service (VIP)
  "Security Recall     ",   // servf = 24 Security Recall
  "Sabbath Service     ",   // servf = 25 Sabbath service
  "TUG Service         ",	// servf = 26 TUG Service operation
  "Hot Oil Operation   ",	// servf = 27 Hot Oil Operation
  "Riot Control        ", 	// servf = 28 Riot Control
  "Car Sw Elevator Off2",   // servf = 29 Car Elevator Off
  "                    ", 	// servf = 30 Unused
  "                    ", 	// servf = 31 Unused
  "                    ", 	// servf = 32 Unused
  "                    ", 	// servf = 33 Unused
  "                    ", 	// servf = 34 Unused
};

const char Inspect_Servf[8][20]={            // service flag
  "Invalid Insp Input  ",	// ins_servf = 0 invalid inspection input
  "Car Top Inspection  ",	// ins_servf = 1 Car Top inspection
  "Machine Room Inspect",	// ins_servf = 2 Machine Room Inspection
  "Access Inspection   ",	// ins_servf = 3 Access Inspection
  "In-Car Inspection   ",	// ins_servf = 4 In-Car inspection
  "Car Top Lock Bypass ",	// ins_servf = 5 Car Top Lock Bypass inspection
  "Car Top Gate Bypass ",	// ins_servf = 6 Car Top Gate Bypass inspection
  "CarTop Gate&Lock Byp",	// ins_servf = 7 Car Top Gate and Lock Bypass inspection
};


const char fire_status_dpy[9][20] = {
  "Fire Off:Normal Svc",   // firef == 0
  "Fire 1 Main Return ",   // firef == 1
  "Fire 1 Alt Return  ",   // firef == 2		 
  "Fire 1 Return Comp ",   // firef == 3
  "Fire 2 Door Hold   ",   // firef == 4
  "F2 Const Press DO  ",   // firef == 5
  "F2 Const Press DC  ",   // firef == 6
  "Fire 2 Hold        ",   // firef == 7
  "F2 Momentary PB DC ",   // firef == 8
};

const char fire_door[5][13] = { 
  "Door Closed ",	 // doorf == 0
  "Door Opening",	 // doorf == 1
  "Door Open   ",	 // doorf == 2
  "Door Closing",	 // doorf == 3
  "Door Nudging",	 // doorf == 4
};

const char fire_rdoor[5][13] = {
  "RDor Closed ",	 // rdoorf == 0
  "RDor Opening",	 // rdoorf == 1
  "RDor Open   ",	 // rdoorf == 2
  "RDor Closing",	 // rdoorf == 3
  "RDor Nudging",	 // rdoorf == 4
};


const char call_status_dpy[20][51]={
	"Number of Car Calls",                            /* lvars[0] */
	"Number of Up Calls",                             /* lvars[1] */
	"Number of Down Calls",                           /* lvars[2] */
	"Number of Up Calls Less than 15 seconds =",      /* lvars[3] */
	"Number of Up Calls Less than 30 seconds =",      /* lvars[4] */
	"Number of Up Calls Less than 45 seconds =",      /* lvars[5] */
	"Number of Up Calls Less than 60 seconds =",      /* lvars[6] */
	"Number of Up Calls More than 60 seconds =",      /* lvars[7] */
	"Number of Down Calls Less than 15 seconds =",    /* lvars[8] */
	"Number of Down Calls Less than 30 seconds =",    /* lvars[9] */
	"Number of Down Calls Less than 45 seconds =",    /* lvars[10] */
	"Number of Down Calls Less than 60 seconds =",    /* lvars[11] */
	"Number of Down Calls More than 60 seconds =",    /* lvars[12] */
};

// Video windows

struct tag_vid_win {
		int16 in_use;
		int16 row;
		int16 col;
		int16 row_len;
		int16 col_wid;
		unsigned char attr;
		unsigned char frame;
} vid_win[20];

#define c_max_nmb_row 101
#define c_max_nmb_col 161
#define FRSINGLE 0

#define BACKGROUND	0
#define FOREGROUND	1
#define BLACK		0x00
#define BLUE		0x01
#define GREEN		0x02
#define CYAN		0x03
#define RED			0x04
#define MAGENTA 	0x05
#define BROWN		0x06
#define LTGREY		0x07
#define DKGREY		0x08
#define LTBLUE		0x09
#define LTGREEN 	0x0A
#define LTCYAN		0x0B
#define LTRED		0x0C
#define LTMAGENTA	0x0D
#define YELLOW		0x0E
#define WHITE		0x0F

int16 gwindow = 0;
int16 upbox = 0;
int16 dnbox = 0;		
int16 carbox = 0; 		
int16 arrow1 = 0;		
int16 arrow2 = 0;	
int16 arrow3 = 0;	
int16 arrow4 = 0;	
int16 arrow5 = 0;	
int16 arrow6 = 0;	
int16 arrow7 = 0;	
int16 arrow8 = 0;	
int16 nmlscn = 0;	
int16 dbgscn = 0;	
int16 trascn = 0;	
int16 menuscn	= 0;	
int16 popupbox = 0;	
int16 current_color = 0;


int16 char_width = 8;
int16 char_height = 9;
char cursor_type = 0;		// no cursor

#define _NOCURSOR 0
#define _NORMALCURSOR 1
#define _SOLIDCURSOR 2

void setcursortype(int16 cur_t)
{
	if (cur_t != 0)
		cursor_type = 0x01;
	else 
		cursor_type = 0;
}
void ScreenPutChar(uint8 c, uint16 color, int16 Col, int16 Row)
{
	int32 addr;
	addr = (Row * 80) + Col;
	if (current_color != color)
		wr_vga_reg(4800,color);
	current_color = color;
	wr_vga(addr,c);
}

void Set_Color(uint16 color)
{
	wr_vga_reg(4800,color);
	current_color = color;
}

void ScreenSetCursor (int16 Row, int16 Col)
{
	int32 addr;
	int16 cursor;
	addr = (Row * 80) + Col;
	cursor = rd_vga_reg(addr);
	wr_vga_reg(addr,(int16)((cursor & 0xFE) | cursor_type));
}

void ScreenClear (void)
{
	wr_vga_reg(4804,1);

}

void ScreenPutString(char *p,int16 attr, int16 Col, int16 Row)
{
	int16 i = 0;
	int16 col_ix = 0;
	
	while ((p[i] != 0) && ((Col + col_ix) < 80))
	{
		ScreenPutChar(p[i],attr,Col+col_ix,Row);
		i++;
		col_ix++;
	}
		
}


#define _KEYBRD_READ  		  0    // Read character. 
#define	_KEYBRD_READY 		  1    // Check character. 
#define _KEYBRD_SHIFTSTATUS   2    // Get shift status. 
#define _KEYBRD_ALTSTATUS 	  3	   // Get alt status.


/******************************************************/
/***       Moves the position  indicator            ***/
/******************************************************/
void GrpHall_PosInd_MOVE(void)
{
	static int16 PrevCAR_NO =0;
	static int16 PrevFL = 0;
	static int16 CurCAR_NO = 0;
	static int16 CurFL = 0;
	
	CurCAR_NO = Call_Pointer_X;
	CurFL =  Call_Pointer_Y;						

	if((PrevCAR_NO!=CurCAR_NO)||(PrevFL != CurFL))
	{
		GrpHall_PosInd_CLEAR(PrevFL,PrevCAR_NO);
		PrevCAR_NO = CurCAR_NO;
		PrevFL = CurFL;									
	}
	GrpHall_PosInd_SET(CurFL,CurCAR_NO);
}

/******************************************************/
/***       Set the position  indicator              ***/
/******************************************************/
void GrpHall_PosInd_SET(int16 fl,int16 car)
{
	int16 Col;
	int16 Row;
	
	if((car>0)&&(car<(cons[nmbcars] + 1)))				   // Car Call
	{
		if(fl <= (cons[grtopf] - 30))
		{
			Col = 49 + ((car-1)*4);
			Row = 48 - fl;
		}
		else
		{
			Col = 9 + ((car-1)*4);
			Row = (cons[grtopf] + 18) - fl;		  
		}
		if(((fl!=grp_Car_Stat[car].g_pos)&&(grp_Car_Stat[car].g_pos!=0))||(grp_Car_Stat[car].g_pos==0)&&(fl!=1))
			ScreenPutChar((uint8)cc_PosInd,DKGREY,Col,Row);
	}
	else if(car == 0)										// UP Hall Call
	{
		if(fl <= (cons[grtopf] - 30))
		{
			Col = 47;
			Row = 48 - fl;
		}
		else
		{
			Col = 7;
			Row = (cons[grtopf] + 18) - fl;
		}
		ScreenPutChar((uint8)c_hc_up_arrow,DKGREY,Col,Row);			 
	}
	else if(car == (cons[nmbcars] + 1))					    // Down Hall Call
	{
		if(fl <= (cons[grtopf] - 30))
		{
			Col = 47;
			Row = 48 - fl;
		}
		else
		{
			Col = 7;
			Row = (cons[grtopf] + 18) - fl;
		}
		ScreenPutChar((uint8)c_hc_dn_arrow,DKGREY,Col,Row);
	}  
}

/******************************************************/
/***       Clears the position  indicator           ***/
/******************************************************/
void GrpHall_PosInd_CLEAR(int16 fl,int16 car)
{
	int16 Col;
	int16 Row;
	
	if((car>0)&&(car<(cons[nmbcars] + 1)))				   // Car Call
	{
		if(fl <= (cons[grtopf] - 30))
		{
			Col = 49 + ((car-1)*4);
			Row = 48 - fl;
		}
		else
		{
			Col = 9 + ((car-1)*4);
			Row = (cons[grtopf] + 18) - fl;
		}
		if(((fl!=grp_Car_Stat[car].g_pos)&&(grp_Car_Stat[car].g_pos!=0))||(grp_Car_Stat[car].g_pos==0)&&(fl!=1))
			ScreenPutChar(' ',DKGREY,Col,Row);	
	}
	else if(car == 0)										// UP Hall Call
	{
		if(fl <= (cons[grtopf] - 30))
		{
			Col = 47;
			Row = 48 - fl;
		}
		else
		{
			Col = 7;
			Row = (cons[grtopf] + 18) - fl;
		}
		ScreenPutChar(' ',DKGREY,Col,Row);			 
	}
	else if(car == (cons[nmbcars] + 1))					    // Down Hall Call
	{
		if(fl <= (cons[grtopf] - 30))
		{
			Col = 47;
			Row = 48 - fl;
		}
		else
		{
			Col = 7;
			Row = (cons[grtopf] + 18) - fl;
		}
		ScreenPutChar(' ',DKGREY,Col,Row);
	}
}


/******************************************************/
/*** This is the First time video initialization    ***/
/******************************************************/
void Init_Video(void)
{

//	set_vert_res(scan_lines_400);
//	set_vmode(CB80x25);	// with 8x16 font	
  	if (cons[grtopf] > 23)
		set_font(font_8x8);	 // using 8x8 font floats the rows to 50
	else
		set_font(font_8x14);	// 28 rows font size
	wr_vga_reg(4801,0);	// clear foreground color register
	wr_vga_reg(4802,0);	// clear background color register
	Set_Color(WHITE);			// Use 4800 foreground in ls nibble and background in ms nibble
//    ScreenClear();
	setcursortype(_NOCURSOR);
  	group_dpy_init = 0;
	grp_page = 0;
	io_page = 0;
  	io_screen_init = 0;
  	io_grp_screen_init = 0;
	Display_Fault = 0;
	Display_Trace = 0;
	Car_Select = 0;
	Car_IO_Select = 0;	
	trace_init_screen_state = Init_Trace_Io_Screen(Car_Select,trace_init_screen_state);
	Fault_Screen(cons[carnmb],Reset_State);
	Init_Io_Screen(cons[carnmb],Reset_State);
	Init_Group_Hall_Display(Reset_State);
	Init_Grp_Io_Screen(Reset_State);
	job_stats_screen(Reset_State);
	init_menu_screen(Reset_State);
	init_IO_scn_lcD();
	calculate_IO_scn_checksums();
	dpy_mode = 1;

	//init variables for group hall screen
	UpHC_PosInd = 0;			// Up hall call position indicator
	DnHC_PosInd  = cons[nmbcars] + 1;	  // Down hall call position indicator

  	freeze = 0;
	menu_ix = 1;
	O_menu_ix = 1;
}

/************************************************************/
/*** This is the Main Video and keyboard interface entry ****/
/************************************************************/

void video_display_control(void)
{

	if (Dispatcher != cons[carnmb])
	{
//		if ((dpy_mode != 1) && (dpy_mode != 4) && (dpy_mode != 5))
//			dpy_mode = 1;
		Car_Select = cons[carnmb];		
	}

	if ( (video_exist == 1) && ((timers[tvideo] > fvars[fvtovideo]) && (fvars[fvtovideo] != 0))
		&& (dpy_mode != 6) )
	{
		save_dpy_mode = dpy_mode;
		dpy_mode = 6;
		video_timeout = 1;
		video_off = 0;
	}

	if (dpy_mode == 0)
	{		// Menu Screen
		menu_control(Run_State);
		Prev_Car_Select = 99;
	}
	else if (dpy_mode == 1)
  	{		   // Standard job displays
		hidden_kbrd_control();
	  	if ((Car_Select != Prev_Car_Select) || 
			((Car_Select == 0) && (grp_page != prev_grp_page)) )
	  	{
	  		Car_IO_Select = 0;
			Init_Grp_Io_Screen(Reset_State);
			Init_Group_Hall_Display(Reset_State);
			Fault_Screen(Car_Select,Reset_State);
			Init_Io_Screen(Car_Select,Reset_State);
			trace_init_screen_state = Init_Trace_Io_Screen(Car_Select,0);
	  	}

		if (Car_Select == 0)
		{
	  		Car_IO_Select = 0;
			if (((grp_page == 1) || (grp_page == 2) 
#if (Tract_HR == 1)
				|| (grp_page == 3) || (grp_page == 4)
#endif
#if (GALaxy_4 == 1)
				) && ((gc_hall_io_dev != 0) || (nmb_grcan_dev != 0)))
#else
				) && (gc_hall_io_dev != 0))
#endif
			{
				if (io_grp_screen_init == 0)
					Init_Grp_Io_Screen(Run_State);
				else 
					Grp_Io_Screen(Run_State);
			}
			else
			{
				if (group_dpy_init == 0)
					Init_Group_Hall_Display(Run_State);
				else
					Group_Hall_Display(Run_State);
			}
		}
		else if (Car_Select >= 1)
		{	
			if (Display_Fault != Prev_Display_Fault)
			{
				Init_Io_Screen(Car_Select,Reset_State);
				if (Display_Fault == 1)
					Fault_Screen(Car_Select,Reset_State);
				else
					trace_init_screen_state = Init_Trace_Io_Screen(Car_Select,0);
			}	
			Prev_Display_Fault = Display_Fault;
			
			if (Display_Trace != Prev_Display_Trace)
			{
				Init_Io_Screen(Car_Select,Reset_State);
				if (Display_Trace == 1)
					trace_init_screen_state = Init_Trace_Io_Screen(Car_Select,0);
			}	
			Prev_Display_Trace = Display_Trace;

			if (Display_Fault == 1)
			{
	 	 		Car_IO_Select = 0;
				Fault_Screen(Car_Select,Run_State);
			}
			else if (Display_Trace == 1)
			{
				Car_IO_Select = 0;
				if (trace_init_screen_state != 0)
				{
					trace_init_screen_state = Init_Trace_Io_Screen(Car_Select,trace_init_screen_state);
					trace_dpy_ix = Trace_ix;
				}
				else 
				{
					if (Trace_Track_Rotate_Ix != Prev_Trace_Track_Rotate_Ix)
					{
						Track_Rotate_State = Init_Trace_Block(Track_Rotate_State);
						if (Track_Rotate_State == 0)
						{
							trace_screen_state = Trace_Io_Screen(Car_Select,0,&S_Trace[trace_dpy_ix]);
							Prev_Trace_Track_Rotate_Ix = Trace_Track_Rotate_Ix;
						}
					}
					else
					{
						
						Track_Rotate_State = 0;
				
						if (Trace_Stop == 1)
						{
							if (set_dpy_ix == 0)
							{
								set_dpy_ix = 1;
								trace_dpy_ix = trace_stop_ix;
								trace_screen_state = Trace_Io_Screen(Car_Select,0,&S_Trace[trace_dpy_ix]);
							}
							if (trig_dpy_ix == 1)
							{
								trig_dpy_ix = 0;
								trace_dpy_ix = trace_stop_ix;
								trace_screen_state = Trace_Io_Screen(Car_Select,0,&S_Trace[trace_dpy_ix]);
							}
							if (end_dpy_ix == 1)
							{
								end_dpy_ix = 0;
									
								if ((trace_stop_ix + 35) >= max_trace_ix)
									trace_dpy_ix = (int16)(35 - (max_trace_ix - trace_stop_ix));
								else
									trace_dpy_ix = (int16)(trace_stop_ix + 35);
								
								trace_screen_state = Trace_Io_Screen(Car_Select,0,&S_Trace[trace_dpy_ix]);
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
								trace_screen_state = Trace_Io_Screen(Car_Select,0,&S_Trace[trace_dpy_ix]);
							}
						}	
						else
						{
							set_dpy_ix = 0;
							trace_dpy_ix = Trace_ix;
							if (trace_screen_state == 0)
							{
								trace_screen_state = 1;
							}
							
						}
							
						if (trace_screen_state != 0)
						{
				    		trace_screen_state = Trace_Io_Screen(Car_Select,trace_screen_state,&S_Trace[trace_dpy_ix]);
						}
					}
				}
			}
			else
			{
				Car_IO_Select = Car_Select;
				if (io_screen_init == 0)
					Init_Io_Screen(Car_Select,Run_State);
				else  if (freeze == 0)	   // Continue to update the display when not in freeze mode
			    	Io_Screen(Car_Select,Run_State);
			}
		}
		else
			Car_Select = 1;
		
		Prev_Car_Select = Car_Select;
		prev_grp_page = grp_page;
  	}
	else if (dpy_mode == 2)
	{
 		Car_IO_Select = 0;
		hidden_kbrd_control();
		job_stats_screen(Run_State);
	}	
	else if (dpy_mode == 3)
	{
 		Car_IO_Select = 0;
		hidden_kbrd_control();
		Prev_Car_Select = 99;
	}
  	else if (dpy_mode == 4)
	{				// Debug screen display
 		Car_IO_Select = 0;
		debug_control();
		Prev_Car_Select = 99;
	}
	else if (dpy_mode == 5)
	{
 		Car_IO_Select = 0;
		hidden_kbrd_control();
		Mem_dump_screen(Run_State);
	}	
	else if (dpy_mode == 6)
	{
		Car_IO_Select = 0;
		hidden_kbrd_control();
   		if (video_timeout == 1)
			video_time_out(turn_video_off);
		else if ( (LCD_Mode_PB == 1) || (LCD_Enter_PB == 1) || (LCD_UP_PB == 1) || (LCD_DN_PB == 1) )
			video_time_out(turn_video_on);
	}
}
  
// ---------------------------------------------------------------------
// Get Communications Status
//       0 = no comm, 1 = comm with board,  2 = comm on previous board
// ---------------------------------------------------------------------
int16 get_comm_status (int16 board_nmb)
{
	int16 comm_status = 0;
	int16 devnmb = 0;
		
	if ((HC_IO_board[board_nmb] & 0x02) != 0)
	{			// Serial Hall call driver boards
		if ((board_nmb%6) == 0) 	// using a driver board
		{
			devnmb = (board_nmb/6) + c_grcan_HC_1;		
			comm_status = (grcan_dev[devnmb].online == 1);
		}
		else
			comm_status = 2;
	}
	else
	{
		devnmb = board_nmb + c_grcan_SEB_1;
		if ((devnmb >= c_grcan_SEB_1) && (devnmb <= c_grcan_END))
			comm_status = (grcan_dev[devnmb].online == 1);
	}
	return(comm_status);
}

/********************************************/
/***  Get arrow and keyboard data entry  ****/
/********************************************/
int16 get_arrow_keybrd (void)
{
	int16 len=0;

	if(key_brd(_KEYBRD_READY) != 0)
	{
	    key_entry = key_brd(_KEYBRD_READ);
		timers[tvideo] = 0;
		if((key_entry & 0x0ff) == 0)         // ARROW OR FUNCTION KEY 
		{
			if((key_entry >> 8) == 0x48)     // Up arrow pushed 
				arrow_key = 1;
			if((key_entry >> 8) == 0x50)     // down arrow pushed 
				arrow_key = 2;
			if((key_entry >> 8) == 0x4B)     // left arrow pushed 
				arrow_key = 4;
			if((key_entry >> 8) == 0x4D)     // right arrow pushed 
				arrow_key = 3;
		}
		else
		{
			arrow_key = 0;
	  		kb_buff[kbp] = (uint8)(key_entry & 0xff); 
			if ((kb_buff[kbp] >= 'a') && (kb_buff[kbp] <= 'z'))
				kb_buff[kbp] = kb_buff[kbp] & 0xdf;
			if (kb_buff[kbp] == CR)
			{
				len = kbp+1;
				kbp = 0;
				if (len == 1)	// Hit enter only
				{
					return 1;
				}
				debug_cmd(len);
			}
			else if (kb_buff[kbp] == BS)
			{
				if (kbp > 0)
					kbp--;
			}
			else if ((kb_buff[kbp] == ESC) || (keybrd_found == 0))
			{
				keybrd_found = 1;
				dpy_mode = 0;
				init_menu_screen(Reset_State);
				kbp = 0;
			}
	  		else
			{
				if (kbp < 10)
					kbp++;
			}
		}
	}
	else
		arrow_key = 0;

	return 0;
}

/*******************************************/
/***  Standard get keyboard data entry  ****/
/*******************************************/

int16 get_keybrd (int16 row_st, int16 col_st, int16 max_len, int16 echo, int16 usr_win)
{
	int16 len = 0;
	
	if(key_brd(_KEYBRD_READY) != 0)
	{		   // key stroke was made
	    key_entry = key_brd(_KEYBRD_READ);
		timers[tvideo] = 0;
		if((key_entry & 0x0ff) != 0)         // ARROW OR FUNCTION KEY 
		{
			arrow_key = 0;
	  		kb_buff[kbp] = (uint8)(key_entry & 0xff); 
			if ((kb_buff[kbp] >= 'a') && (kb_buff[kbp] <= 'z'))
				kb_buff[kbp] = kb_buff[kbp] & 0xdf;
			if (kb_buff[kbp] == CR)
			{
				len = kbp+1;
				kbp = 0;
				row = row_st;
				col = col_st;
				if (echo == 1)
					vratputf(usr_win,row,col,WHITE,"_         ");
				return len;
			}
			else if (kb_buff[kbp] == BS)
			{
				if (echo == 1)
				{		// Debug screen
					if (col > col_st)
						col--;
					if (col < col_st)
						col = col_st;
					row = row_st;
					vratputc(usr_win,row,col+1,WHITE,' ');
					vratputc(usr_win,row,col,WHITE,'_');
					lcurset(usr_win,row,col);
				}
				if (kbp > 0)
					kbp--;
			}
			else if ((kb_buff[kbp] == ESC) && (dpy_mode == 4))
			{
				dpy_mode = 0;
				init_menu_screen(Reset_State);
				kbp = 0;
			}
			else 
			{
				if (echo == 1)
				{			
					if (col < (max_len + col_st))
					{
						vratputc(usr_win,row,col,WHITE,kb_buff[kbp]);
						vratputc(usr_win,row,col+1,WHITE,'_');
						col++;
					}
				}
				if (kbp < max_len)
					kbp++;
			}
		}
	}
	return len;
}

/***********************************************/
/***  Hidden keyboard during video display  ****/
/***********************************************/
void hidden_kbrd_control (void)
{
	int16 len = 0;

	if(key_brd(_KEYBRD_READY) != 0)
	{
	    key_entry = key_brd(_KEYBRD_READ);
		timers[tvideo] = 0;

		if (keybrd_found == 0)
		{
			keybrd_found = 1;
			dpy_mode = 0;
			init_menu_screen(Reset_State);
			kbp = 0;
			return;
		}

		if((key_entry & 0x0ff) == 0)         // ARROW OR FUNCTION KEY 
		{
			keybrd_found = 1;
			if((key_entry >> 8) == 0x48)     // Up arrow pushed 
			{
				Call_Pointer_Y++;
				if( (Call_Pointer_Y >= cons[grtopf]) && (Call_Pointer_X == 0) )
	    			Call_Pointer_Y = 1;
				if(Call_Pointer_Y > cons[grtopf])
				{
	    			if(Call_Pointer_X == (cons[nmbcars] + 1) )
						Call_Pointer_Y = 2;
	    			else
						Call_Pointer_Y = 1;
				}
				arrow_key = 1;
			}
			if((key_entry >> 8) == 0x50)     // down arrow pushed 
			{
				Call_Pointer_Y--;
				if( (Call_Pointer_Y <= 1) && (Call_Pointer_X == (cons[nmbcars] + 1)) )
	    			Call_Pointer_Y = cons[grtopf];
				if( (Call_Pointer_Y < 1) && (Call_Pointer_X == 0) )
	    			Call_Pointer_Y = (cons[grtopf] - 1);
				if(Call_Pointer_Y < 1)
					Call_Pointer_Y = cons[grtopf];
				arrow_key = 2;
			}
			if((key_entry >> 8) == 0x4B)     // left arrow pushed 
			{
				Call_Pointer_X--;
				if(Call_Pointer_X < 0)
	    			Call_Pointer_X = (cons[nmbcars] + 1);
				if( (Call_Pointer_X == (cons[nmbcars] + 1)) && (Call_Pointer_Y == 1) )
	    			Call_Pointer_Y = 2;
				if( (Call_Pointer_X == 0) && (Call_Pointer_Y == cons[grtopf]) )
	    			Call_Pointer_Y = (cons[grtopf] - 1);
				arrow_key = 4;
			}
			if((key_entry >> 8) == 0x4D)     // right arrow pushed 
			{
				Call_Pointer_X++;
				if(Call_Pointer_X > (cons[nmbcars] + 1) )
	    			Call_Pointer_X = 0;
				if( (Call_Pointer_X == (cons[nmbcars] + 1)) &&(Call_Pointer_Y == 1) )
	    			Call_Pointer_Y = 2;
				if( (Call_Pointer_X == 0) && (Call_Pointer_Y == cons[grtopf]) )
	    			Call_Pointer_Y = (cons[grtopf] - 1);
				arrow_key = 3;
			}
 			if((key_entry >> 8) == 0x49)     // page up pushed 
			{
				arrow_key = 5;
			}
 			if((key_entry >> 8) == 0x51)     // page down pushed 
			{
				arrow_key = 6;
			}
 			if((key_entry >> 8) == 0x47)     // home pushed 
			{
				arrow_key = 7;
			}
 			if((key_entry >> 8) == 0x4F)     // end pushed 
			{
				arrow_key = 8;
			}
 			if((key_entry >> 8) == 0x3B)     // F1 pushed 
			{
				arrow_key = 9;
			}
 			if((key_entry >> 8) == 0x3C)     // F2 pushed 
			{
				arrow_key = 10;
			}
			if ((dpy_mode == 1) && (Car_Select != 0))
			{		// Car Display
				if (arrow_key == 1)
				{	// Up arrow
					if (Display_Trace == 1)
						next_trace+= 1;			 		// Up 1 trace step
					else if ((Display_Fault == 1) && (detailed_fault == 1))
						next_trace+= 1;			 		// Up 1 trace step
					else if (cons[carnmb] == Dispatcher)
					{
						if (Car_Select < cons[nmbcars])
							Car_Select++;
						else
							Car_Select = 1;
					}
					else
						Car_Select = cons[carnmb];
				}
				else if (arrow_key == 2)
				{	// Down Arrow
					if (Display_Trace == 1)
						next_trace -= 1;			  	// Down 1 trace step
					else if ((Display_Fault == 1) && (detailed_fault == 1))
						next_trace -= 1;			  	// Down 1 trace step
					else if (cons[carnmb] == Dispatcher)
					{
						if (Car_Select <= 1)
							Car_Select = cons[nmbcars];
						else
							Car_Select--;
					}
					else
						Car_Select = cons[carnmb];
				}
				else if (arrow_key == 3)
				{
					if (Display_Trace == 1)
					{
						if (Trace_Track_Rotate_Ix < 6)
							Trace_Track_Rotate_Ix++;			  	// rotate to next screen
						else
							Trace_Track_Rotate_Ix = 0;
					}
					else if (Display_Fault == 1)
					{
						if (detailed_fault == 1)
						{
							if (Trace_Track_Rotate_Ix < 6)
								Trace_Track_Rotate_Ix++;			  	// rotate to next screen
							else
								Trace_Track_Rotate_Ix = 0;
						}
						Rotate_Fault = 1;
					}
					else 
						rotate_io_bd(1);
				}	 
				else if (arrow_key == 4)
				{
					if (Display_Trace == 1)
					{
						if (Trace_Track_Rotate_Ix > 0)
							Trace_Track_Rotate_Ix--;			  	// rotate to previous screen
						else
							Trace_Track_Rotate_Ix = 6;
					}
					else if (Display_Fault == 1)
					{
						if (detailed_fault == 1)
						{
							if (Trace_Track_Rotate_Ix > 0)
								Trace_Track_Rotate_Ix--;			  	// rotate to previous screen
							else
								Trace_Track_Rotate_Ix = 6;
						}
						Rotate_Fault = 1;
					}
					else 
						rotate_io_bd(0);
				}
				else if (arrow_key == 5)
				{						   		// PgUp Jump up 10 trace steps
					if (Display_Trace == 1)
						next_trace += 10;
				}
				else if (arrow_key == 6)
				{							  	// PgDn Jump down 10 trace steps
					if (Display_Trace == 1)
						next_trace -= 10;
				}
				else if (arrow_key == 7)
				{								// Home Jump to start of trace
					if (Display_Trace == 1)
						trig_dpy_ix = 1;
				}
				else if (arrow_key == 8)
				{								// END Junp to end of trace
					if (Display_Trace == 1)
						end_dpy_ix = 1;
				}
				else if (arrow_key == 9)
				{								 // F1 Start trace (allow to run)
					if (Display_Trace == 1)
						Trace_Stop = 0;
				}
				else if (arrow_key == 10)
				{								// F2 Stop trace
					if (Display_Trace == 1)
						Trace_Trigger = 1;
				}
				
			}
#if (GALaxy_4 == 1)
			if ((dpy_mode == 1) && (Car_Select == 0) && (grp_page != 0) && ((gc_hall_io_dev != 0) || (nmb_grcan_dev != 0)) && (cons[carnmb] == Dispatcher))

#else
			if ((dpy_mode == 1) && (Car_Select == 0) && (grp_page != 0) && (gc_hall_io_dev != 0) && (cons[carnmb] == Dispatcher))
#endif
			{		  // Rotate to next page for hall display
				if (arrow_key == 3)
				{			// right arrow key
					if ((grp_page > 0) && (grp_page < nmbgio_page))				
						grp_page ++;
				}
				if (arrow_key == 4)
				{			// left arrow key
					if (grp_page > 1)				
						grp_page --;
				}
				if (grp_page > nmbgio_page)
					grp_page = nmbgio_page;
			}
		}
		else
		{
			arrow_key = 0;
	  		kb_buff[kbp] = (uint8)(key_entry & 0xff); 

			if ((kb_buff[0] != 0) && (video_off == 1))
				video_time_out(turn_video_on);

			if ((kb_buff[kbp] >= 'a') && (kb_buff[kbp] <= 'z'))
				kb_buff[kbp] = kb_buff[kbp] & 0xdf;
			if (kb_buff[kbp] == CR)
			{
				len = kbp+1;
				kbp = 0;
				if (len == 1)	// Hit enter only
				{
					if (dpy_mode == 1)
					{
						if (Car_Select == 0)
						{	// Group Display
							if((key_brd(_KEYBRD_SHIFTSTATUS) & 0x01) != 0)	// changed from 0x03 to 0x01
							{		// Shift key is pressed with enter so do rear calls
								timers[tvideo] = 0;
								if (Call_Pointer_X == 0)
									set_rupcall(Call_Pointer_Y);
								else if (Call_Pointer_X == (cons[nmbcars]+1))
									set_rdncall(Call_Pointer_Y);
								else if ((Call_Pointer_X >= 1) && (Call_Pointer_X <= cons[nmbcars]))
									set_rcarcall(vidcar[Call_Pointer_X],Call_Pointer_Y);
							}
							else
							{		// Front calls
								if (Call_Pointer_X == 0)
									set_upcall(Call_Pointer_Y);
								else if (Call_Pointer_X == (cons[nmbcars]+1))
									set_dncall(Call_Pointer_Y);
								else if ((Call_Pointer_X >= 1) && (Call_Pointer_X <= cons[nmbcars]))
									set_carcall(vidcar[Call_Pointer_X],Call_Pointer_Y);
							}
						}
						else if (Display_Fault == 1)
							Rotate_Fault = 1; // Car Display and in fault mode
					}
					else if (dpy_mode == 5)
						next_mem_page = 1;
				}
				else if (len == 2)
				{		// one key command
					if (kb_buff[0] == 'G')
					{
						dpy_mode = 1;
						Init_Group_Hall_Display(Reset_State);
						Car_Select = 0;
						grp_page = 0;
					}
					else if (kb_buff[0] == 'H')
					{
#if (GALaxy_4 == 1)
						if ((gc_hall_io_dev != 0) || (nmb_grcan_dev != 0))   // hall call comm
#else
						if (gc_hall_io_dev != 0)   // hall call comm
#endif
						{
							dpy_mode = 1;
							Init_Grp_Io_Screen(Reset_State);
							Car_Select = 0;
							grp_page = 1;
						}
					}
					else if (kb_buff[0] == 'C')
					{
						if ((Car_Select < 1) || (Car_Select > cons[nmbcars]))
							Car_Select = cons[carnmb];
						Init_Io_Screen(Car_Select,Reset_State);
						dpy_mode = 1;
						Display_Fault = 0;
						Display_Trace = 0;
					}
					else if (kb_buff[0] == 'F')
					{
						if ((Car_Select < 1) || (Car_Select > cons[nmbcars]))
							Car_Select = cons[carnmb];
						Fault_Screen(Car_Select,Reset_State);	 // Reset here. Runs in video_display_control
						Display_Fault = 1;
						Display_Trace = 0;
						dpy_mode = 1;
					}
					else if (kb_buff[0] == 'T')
					{
//						if ((Car_Select < 1) || (Car_Select > cons[nmbcars]))
						Car_Select = cons[carnmb];
						trace_init_screen_state = Init_Trace_Io_Screen(Car_Select,0);
						Display_Trace = 1;
						Display_Fault = 0;
						dpy_mode = 1;
					}
				}
				debug_cmd(len);
			}
			else if (kb_buff[kbp] == BS)
			{
				if (kbp > 0)
					kbp--;
			}
			else if (kb_buff[kbp] == ESC) 
			{
				keybrd_found = 1;
				dpy_mode = 0;
				init_menu_screen(Reset_State);
				kbp = 0;
			}
	  		else
			{
				if (kbp < 10)
					kbp++;
			}
		}
	}
	else
		arrow_key = 0;
}



//***********************************************************************
//***********************************************************************
//*************  Group Menu Routines  ***********************************
//***********************************************************************
//***********************************************************************

//****************************************************
//**********  Menu Screen Control ********************
//****************************************************

void menu_control (int16 init_flag)
{
	int16 status=0;
	static char *msg;
	static int16 state;
	static int16 sub_state;
	int16 i;
	int16 cursor_r = 26;
	if (init_flag == 1)
	{
		state = 0;
		sub_state = 0;
		return;
	}

	if (menu_init == 0)
		init_menu_screen(Run_State);
	else
	{
		switch (state)
		{
			case 0:
				status = get_arrow_keybrd();
				if (arrow_key == 1)
				{		// Up arrow
					if (menu_ix > 1)
						menu_ix--;
					else
						menu_ix = max_menu_items;
				}
				else if (arrow_key == 2)
				{	   // Down Arrow
					if (menu_ix < max_menu_items)
						menu_ix++;
					else
						menu_ix = 1;
				}
				if (menu_ix != O_menu_ix)
				{		  // show new menu item
					vratputf(menuscn,O_menu_ix+1,10,WHITE," ");
					vratputf(menuscn,menu_ix+1,10,WHITE,"%c",cursor_r);
				}
				O_menu_ix = menu_ix;					
				if (status == 1)
				{
					if((Dispatcher != cons[carnmb]) && (menu_ix > 1))   // Not the Dispatcher
						state = menu_ix + 2;		// Skip Group and Hall Call I/O display
					else						
						state = menu_ix;
					sub_state = 0;
				}
				break;
			case 1:		// Time and date
				sub_state = time_date(sub_state);
				if(sub_state == 0)
					state = 'r';
				break;
			case 2:		// Group Display Screen
				dpy_mode = 1;
				Init_Group_Hall_Display(Reset_State);
				Car_Select = 0;
				grp_page = 0;
				state = 0;
				break;
			case 3:		// Group hall call display
				dpy_mode = 1;
				Init_Grp_Io_Screen(Reset_State);
				Car_Select = 0;
				grp_page = 1;
				state = 0;
				break;			
			case 4:		// Car Display Screen
				sub_state = sel_car_num(sub_state);
				if (sub_state == 0)
				{	// Car number has been selected
					Init_Io_Screen(Car_Select,Reset_State);
					dpy_mode = 1;
					Display_Trace = 0;
					Display_Fault = 0;
					state = 0;		// clear state
				}
				break;
			case 5: 		// View Job Statistics
				dpy_mode = 2;
				job_stats_screen(Reset_State);		 // Reset here. Runs in video_display_control
				state = 0;
				break;
			case 6:		// Clear Job Statistics
				menu_cmd = 0;
				state = 'j';
				break;
			case 7:		// View Car Faults
				sub_state = sel_car_num(sub_state);
				if (sub_state == 0)
				{	// Car number has been selected
					detailed_fault = 0;
					Fault_Screen(Car_Select,Reset_State);	 // Reset here. Runs in video_display_control
					Display_Trace = 0;
					Display_Fault = 1;
					dpy_mode = 1;
				}
				break;					 
			case 8:		// View Extended Car Faults
				sub_state = sel_car_num(sub_state);
				if (sub_state == 0)
				{	// Car number has been selected
					detailed_fault = 1;
					Fault_Screen(Car_Select,Reset_State);	 // Reset here. Runs in video_display_control
					Display_Trace = 0;
					Display_Fault = 1;
					next_trace = 0;
					dpy_mode = 1;
				}
				break;					 
			case 9:		// Clear Car Faults
				sub_state = sel_car_num(sub_state);
				if (sub_state == 0)
				{	// Car number has been selected
					state = 'f';
					menu_cmd = 0;
				}
				break;
			case 10:		// Car Trace Display Screen
				Car_Select = cons[carnmb];
				{	// Car number has been selected
					trace_init_screen_state = Init_Trace_Io_Screen(Car_Select,0);
					dpy_mode = 1;
					Display_Fault = 0;
					Display_Trace = 1;
					next_trace = 0;
					state = 0;		// clear state
				}
				break;
			case 'f':		// Clear Car Faults
				sub_state = are_you_sure(sub_state,&menu_cmd);
				if (sub_state == 0)
				{
					if (menu_cmd == 0)
						state = 'r';
					else
					{
						if(Car_Select == cons[carnmb])
						{
							msg = "Car Faults Cleared. Hit Any Key.  ";
					      	for(i=0; i<max_flt_ix; i++)
					      	{
								f.Flts[i].code = 0;
								f.Flts[i].count = 0;
					      	}
							Flt_ix = 0;
						  	Up_Date_Flt(1);
	#if (GALaxy_4 == 1)
							clr_mrcan_dev_comm_loss();
							clr_ctcan_dev_comm_loss();
	#else
							clr_can_dev_comm_loss();
	#endif
						}				
						else
						{
							msg = "Clear Faults Sent. Hit Any Key.  ";
							put_pkt_req(10,Car_Select,gc_port);
						}		 
						state = 'a';		// any key popup message
					}	
				}
 				break;
			case 'j':		// Clear job statics
				sub_state = are_you_sure(sub_state,&menu_cmd);
				if (sub_state == 0)
				{
					if (menu_cmd == 0)
						state = 'r';
					else
					{
						clear_job_statistics();
						msg = "Job Statistics Cleared. Hit Any Key.  ";
						state = 'a';	// anykey popup
					}
				}
				break;
			
			case 'a':	   // any key popup message
				sub_state = popup_msg(sub_state,msg);
				if (sub_state == 0)
				{
					state = 'r';
				}
				break;
			case 'r':	// reset menu row and column
				row = 1;
				col = 20;
				state = 0;
				break;
			default:
				row = 1;
				col = 20;
				state = 0;
				break;
		}
	}
}

//****************************************************
//***************  Init Menu Screen  *****************
//****************************************************

void init_menu_screen (int16 init_flag)
{
	static int16 state;
	int16 up_arrow = 24;
	int16 down_arrow = 25;
	int16 cursor_r = 26;
	
	if (init_flag == 1)
	{
		state = 0;
		menu_init = 0;
		return;
	}

	switch (state)
	{
		case 0:
			wnum = 0;
			state = 1;
		    ScreenClear();
			break;
		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:	 
			set_font(font_8x14);	// set 8x14 to get 28 rows
			max_row = 24;
			max_car_row = 21;
			max_window_row = 28;
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			if (Dispatcher == cons[carnmb])
				max_menu_items = 10;				// Must be set to the number of menu items
			else
				max_menu_items = 8;				// Must be set to the number of menu items
			menuscn = vcreat( max_menu_items + 4, 42, WHITE);
			vlocate( menuscn, 5, 20);
			state = 5;
			break;
		case 5:
			vframe( menuscn, WHITE, FRSINGLE );
			state = 6;
			break;
		case 6:
			row = 0;
			col = 10;
			vratputf(menuscn,row,col+6,WHITE,"Group Menu");
			row+=2;
			vratputf(menuscn,row,col,WHITE,"  Time and Date");
			row++;
			state = 7;
			break;
		case 7:
			if (Dispatcher == cons[carnmb])
			{
				
				vratputf(menuscn,row,col,WHITE,"  Group Display");
				row++;
				vratputf(menuscn,row,col,WHITE,"  Hall I/O Display");
				row++;
			}
			vratputf(menuscn,row,col,WHITE,"  Car I/O Display");
			row++;
 			state = 8;
			break;
		case 8:
			vratputf(menuscn,row,col,WHITE,"  View Job Statistics");
			row++;
			vratputf(menuscn,row,col,WHITE,"  Clear Job Statistics");
			row++;
			vratputf(menuscn,row,col,WHITE,"  View Car Faults");
			row++;
			state = 9;
			break;
		case 9:
			vratputf(menuscn,row,col,WHITE,"  View Detailed Car Faults");
			row++;
			vratputf(menuscn,row,col,WHITE,"  Clear Car Faults");
			row++;
			vratputf(menuscn,row,col,WHITE,"  Car Trace Screen");
			row+=2;
			vratputf(menuscn,row,1,WHITE,"<%c%c> Move  <Enter> Select  <ESC> Return",up_arrow,down_arrow);
			vratputf(menuscn,menu_ix+1,10,WHITE,"%c",cursor_r);
			row = 1;
			col = 20;
			echo = 0;
			state = 10;
			break;
		case 10:
			popupbox = vcreat( 3, 40, WHITE);
			vlocate( popupbox, max_menu_items + 11, 20);
			state = 11;
			break;
		case 11:
			menu_init = 1;
			menu_control(Reset_State);	// Reset State 
			state = 0;
			break;

		default:
			state = 0;
			break;
  	}
}

//****************************************************
//**********  Select Car Number **********************
//****************************************************

int16 sel_car_num (int16 state)
{
	int16 status;
	int16 up_arrow = 24;
	int16 down_arrow = 25;

	switch(state)
	{
		case 0: // initial state
			state = 1;
		//lint -fallthrough
		case 1:	  // Car Display - Get Car number from popup box
			vframe( popupbox, WHITE, FRSINGLE );
			Car_Select = cons[carnmb];
			state++;
			break;
		case 2:
	 		vratputf(popupbox,1,1,WHITE,"Select Car Number: %.2s            <%c%c>",car_bld_no[Car_Select],up_arrow,down_arrow);
			row = 1;
			col = 20;
			state ++;
			break;
 		case 3:
			status = get_arrow_keybrd();
			if (arrow_key == 1)
			{	// Up arrow
				if (cons[carnmb] == Dispatcher)
				{
					if (Car_Select < cons[nmbcars])
						Car_Select++;
					else
						Car_Select = 1;
				}
				else
					Car_Select = cons[carnmb];
				state = 2;
			}
			else if (arrow_key == 2)
			{	// Down Arrow
				if (cons[carnmb] == Dispatcher)
				{
					if (Car_Select <= 1)
						Car_Select = cons[nmbcars];
					else
						Car_Select--;
				}
				else
					Car_Select = cons[carnmb];
				state = 2;
			}
			else if (status == 1)
			{
				vc_state = 0;
				state = 4;
			}
			break;
			
		case 4:
			vc_state = vclear(popupbox,vc_state);
			if (vc_state == 0)
				state = 0;
			break;
			
		default:
			state = 0;
			break;
	}
	return state;
}


//****************************************************
//************  Are You Sure Popup Menu  *************
//****************************************************

int16 are_you_sure (int16 state, int16 *cmd)
{
	switch(state)
	{
		case 0:		// initial state
			*cmd = 0;
			state = 1;
		//lint -fallthrough
		case 1:		// Clear popup
			vframe( popupbox, WHITE, FRSINGLE );
			state++;
			break;
		case 2:
			vratputf(popupbox,1,1,WHITE,"Are You Sure Y/N ? _");
			row = 1;
			col = 20;
			state ++;
			break;
 		case 3:
			if (get_keybrd(1,20,1,1,popupbox) == 2)
			{
				if ((char)(kb_buff[0] & 0xDF) == 'Y')
				{
					*cmd = 1;
					vc_state = 0;
					state = 4;
				}
				else if ((char)(kb_buff[0] & 0xDF) == 'N')
				{
					*cmd = 0;
					vc_state = 0;
					state = 4;
				}
			}
			break;
			
		case 4:
			vc_state = vclear(popupbox,vc_state);
			if (vc_state == 0)
				state = 0;
			break;
			
		default:
			*cmd = 0;
			state = 4;
			break;
	}
	return (state);
}

//****************************************************
//*********     Display popup message     ************
//****************************************************

int16 popup_msg (int16 state, char *msg)
{

	switch(state)
	{
		case 0:		// initial state
			state = 1;
		//lint -fallthrough
		case 1:		// Clear popup
			vframe( popupbox, WHITE, FRSINGLE );
			state = 2;
			break;
		case 2:
			vratputf(popupbox,1,1,WHITE,"%s",msg);
			row = 1;
			col = 20;
			state = 3;
			break;
 		case 3:		
			if (key_brd(_KEYBRD_READY) != 0)
			{		   // key stroke was made
			    key_entry = key_brd(_KEYBRD_READ);
				if((key_entry & 0x0ff) != 0)
				{			// any key
					timers[tvideo] = 0;
				    key_brd(_KEYBRD_READ);
					vc_state = 0;;
					state = 4;
				}
			}
			break;
			
		case 4:
			vc_state = vclear(popupbox,vc_state);
			if (vc_state == 0)
				state = 0;
			break;
			
		default:
			state = 0;
			break;
	}
	return (state);
}

//****************************************************
//****************  Time and Date Menu  **************
//****************************************************

int16 time_date (int16 state)
{
	switch(state)
	{
		case 0:		// initial state
			state = 1;
		//lint -fallthrough
		case 1:		// View popup box
			vframe( popupbox, WHITE, FRSINGLE );
			state++;
			break;
		case 2:
			local_gettime();
			vratputf(popupbox,1,1,WHITE,"Time: %2i:%02i:%02i = _",t.hour,t.minute,t.second);
			tx.hour = t.hour;
			tx.minute = t.minute;
			tx.second = t.second;
			state ++;
			row = 1;
			col = 18;
			break;
		case 3:
			if (get_keybrd(1,18,8,1,popupbox) != 0)
			{	
				if (input_time() == 1)
				{
					t.hour = tx.hour;
					t.minute = tx.minute;
					t.second = tx.second;
					local_settime();
					if (cons[carnmb] == Dispatcher)
						put_pkt_req_all_cars(109);		// Update time on all other cars
					state = 4;
					vc_state = 0;
				}
				else
					state = 2;
			}
			break;

		case 4:
			vc_state = vclear(popupbox,vc_state);
			if (vc_state == 0)
				state = 5;
			break;
			
		case 5:
			vframe( popupbox, WHITE, FRSINGLE );
			state ++;
			break;

		case 6:
			local_getdate();
			vratputf(popupbox,1,1,WHITE,"Date: %2i/%02i/%4i = _",d.month,d.day,d.year);
			dx.day = d.day;
			dx.month = d.month;
			dx.year = d.year;
			row = 1;
			col = 20;
			state ++;
			break;

		case 7:
			if (get_keybrd(1,20,10,1,popupbox) != 0)
			{	
				if (input_date() == 1)
				{
					d.day = dx.day;
					d.month = dx.month;
					d.year = dx.year;
					local_setdate();
					state = 8;
					vc_state = 0;
				}
				else
					state = 6;
			}
			break;
 		case 8:
			vc_state = vclear(popupbox,vc_state);
			if (vc_state == 0)
				state = 0;
			break;
			
		default:
			vc_state = 0;
			state = 8;
			break;
	}
	return (state);
}

					//     Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
int days_of_month[13] = {0, 31,  28,  31,  30,  31,  30,  31,  30,  31,  31,  30,  31}; 

//**************************************************
//*********** Input date from keyboard buffer  *****
//**************************************************
int16 input_date(void)
{
	int32 i;
	int16 j,ix;
	int16 valid = 0;
	int16 leap_year = 0;

	j = 0;
	i = 0;
	ix = 0;
	while (j < (ix+4))
	{
		if (kb_buff[j] == '/')
		{
			kb_buff[j] = 0;
			i = atoi(&kb_buff[ix]);
			if ((i > 0) && (i <= 12))
			{
				dx.month = (uint8)i;
				j++;
				valid = 1;
				break;
			}
			return valid;
		}
		if (kb_buff[j] == CR)
		{
			valid = 1;
			if (j > ix)	
			{	
				kb_buff[j] = 0;
				i = atoi(&kb_buff[ix]);
				if ((i > 0) && (i <= 12))
					dx.month = (uint8)i;
				else valid = 0;
			}
			return valid;
		}
		j++;
	}

	if (valid != 1)
		return valid;
	valid = 0;
	ix = j;
	while (j < (ix+4))
	{
		if (kb_buff[j] == '/')
		{
			kb_buff[j] = 0;
			i = atoi(&kb_buff[ix]);
			leap_year = (((dx.year%4) == 0) && ((dx.year%100) != 0)); 
			if (((i > 0) && (i <= days_of_month[dx.month])) || ((dx.month == 2) && (leap_year != 0) && (dx.day == 29)))
			{
				dx.day = (uint8)i;
				j++;
				valid = 1;
				break;
			}
			return valid;
		}
		if (kb_buff[j] == CR)
		{
			valid = 1;
			if (j > ix)	
			{	
				kb_buff[j] = 0;
				i = atoi(&kb_buff[ix]);
				leap_year = (((dx.year%4) == 0) && ((dx.year%100) != 0)); 
				if (((i > 0) && (i <= days_of_month[dx.month])) || ((dx.month == 2) && (leap_year != 0) && (dx.day == 29)))
					dx.day = (uint8)i;
				else valid = 0;
			}
			return valid;
		}
		j++;
	}

	if (valid != 1)
		return valid;
	valid = 0;
	ix = j;
	while (j < (ix+8))
	{
		if (kb_buff[j] == CR)
		{
			valid = 1;
			if (j > ix)	
			{	
				kb_buff[j] = 0;
				i = atoi(&kb_buff[ix]);
				if ((i > 0) && (i <= 6000))
				{
					dx.year = (uint16)i;
					leap_year = (((dx.year%4) == 0) && ((dx.year%100) != 0)); 
					if ((leap_year == 0) && (dx.month == 2) && (dx.day > 28))
						valid = 0;		// leap year error
				}
				else valid = 0;
			}
			return valid;
		}
		j++;
	}
	return valid;
}

//**********************************************
//**** Keyboard data has is a valid number ****
//**********************************************

int16 valid_kbd_num(int16 ix, int16 j)
{
	int16 i;
	int16 good_num = 0;

	for (i = ix; i<=j; i++)
	{
		if ((kb_buff[i] >= '0') && (kb_buff[i] <= '9'))
			good_num = 1;		   // must have a valid number
		else if ((kb_buff[i] != ' ') && (kb_buff[i] != 0))
			return 0;		// cannon have any invalid numbers
	}
	if (good_num == 1)
		return 1;
	else
		return 0;
}

//**************************************************
//*********** Input time from keyboard buffer  *****
//**************************************************
int16 input_time(void)
{
	int32 i;
	int16 j,ix;
	int16 valid = 0;	   // returns 1 for a valid time

	j = 0;
	i = 0;
	ix = 0;
	while (j < (ix+4))
	{
		if (kb_buff[j] == ':')
		{
			kb_buff[j] = 0;
			i = atoi(&kb_buff[ix]);
			if ((i >= 0) && (i <= 23) && (valid_kbd_num(ix,j) != 0))
			{
				tx.hour = (uint8)i;
				valid = 1;
				j++;
				break;
			}
			return valid;
		}
		if (kb_buff[j] == CR)
		{
			valid =1;
			if (j > ix)	
			{	
				kb_buff[j] = 0;
				i = atoi(&kb_buff[ix]);
				if ((i >= 0) && (i <= 23) && (valid_kbd_num(ix,j) != 0))
					tx.hour = (uint8)i;
				else valid = 0;
			}
			return valid;
		}
		j++;
	}
	if (valid != 1)
		return valid;
	valid = 0;
	ix = j;
	while (j < (ix+4))
	{
		if (kb_buff[j] == ':')
		{
			kb_buff[j] = 0;
			i = atoi(&kb_buff[ix]);
			if ((i >= 0) && (i <= 59) && (valid_kbd_num(ix,j) != 0))
			{
				tx.minute = (uint8)i;
				j++;
				valid = 1;
				break;
			}
			return valid;
		}
		if (kb_buff[j] == CR)
		{
			valid = 1;
			if (j > ix)	
			{	
				kb_buff[j] = 0;
				i = atoi(&kb_buff[ix]);
				if ((i >= 0) && (i <= 59) && (valid_kbd_num(ix,j) != 0))
					tx.minute = (uint8)i;
				else valid = 0;
			}
			return valid;
		}
		j++;
	}

	if (valid != 1)
		return valid;
	valid = 0;

	ix = j;
	while (j < (ix+8))
	{
		if (kb_buff[j] == CR)
		{
			if (j > ix)	
			{	
				valid = 1;
				kb_buff[j] = 0;
				i = atoi(&kb_buff[ix]);
				if ((i >= 0) && (i <= 59) && (valid_kbd_num(ix,j) != 0))
					tx.second = (uint8)i;
				else valid = 0;
			}
			return valid;
		}
		j++;
	}
	return valid;
}
 
//**********************************************************************
//**********************************************************************
//***************** Job Statistics Routines ****************************
//**********************************************************************
//**********************************************************************

//*********************************************
// This is the job statistics video screen ***
//*********************************************
void job_stats_screen(int16 init_flag)
{
  static int16 j,ss_ix;
  static int16 state;

	if (init_flag == 1)
	{
		state = 0;
		return;
	}

	switch (state)
	{
		case 0:
			wnum = 0;
			state = 1;
		    ScreenClear();
			break;
		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:	 
			set_font(font_8x14);	// set 8x14 to get 28 rows
			max_row = 24;
			max_car_row = 21;
			max_window_row = 28;
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			nmlscn = vcreat( max_window_row, 80, WHITE);
			vlocate( nmlscn, 1, 1);
			state = 5;
			row = 2;
			col = 4;
			break;
		case 5:
		    vratputf(nmlscn, row, col,WHITE, "Elevator Statistics Screen");
			ss_ix = 0;
			state = 6;
			row += 2;
			col = 4;
			break;

		case 6:
			while ((j<=2) && (ss_ix <= 12))
			{			// three at a time
	      		vratputf(nmlscn,row++, col,WHITE,"%s %ld",call_status_dpy[ss_ix],lvars[ss_ix]);
				j++;
				ss_ix++;
			}
			if (ss_ix > 12)
				state = 7;
			j = 0;
			break;

		case 7:
			state = 8;
			row = 18;
			col = 4;
		    if((lvars[1] + lvars[2]) > 0)
		    	state = 8;
			else
				state = 10;
			break;
		case 8:
	     	vratputf(nmlscn,row++, col,WHITE,"%i Percent Answered In Less than 15 Second",
		 		(((lvars[3] + lvars[8]) * 100) / (lvars[1] + lvars[2])) );
	     	vratputf(nmlscn,row++, col,WHITE,"%i Percent Answered In Less than 30 Second",
		 		(((lvars[4] + lvars[9]) * 100) / (lvars[1] + lvars[2])) );
	     	vratputf(nmlscn,row++, col,WHITE,"%i Percent Answered In Less than 45 Second",
		 		(((lvars[5] + lvars[10]) * 100) / (lvars[1] + lvars[2])) );

			state = 9;
			break;
		case 9:
	     	vratputf(nmlscn,row++, col,WHITE,"%i Percent Answered In Less than 60 Second",
		 		(((lvars[6] + lvars[11]) * 100) / (lvars[1] + lvars[2])) );
	     	vratputf(nmlscn,row++, col,WHITE,"%i Percent Answered In More than 60 Second",
		 		(((lvars[7] + lvars[12]) * 100) / (lvars[1] + lvars[2])) );
			
			state = 10;
			break;
		case 10:
			break;

		default:
			state = 0;
			break;
  	}		
  	return;
}

//**************************************************************************
//**************************************************************************
//************  Car Diagnostic Screen Routines  ****************************
//**************************************************************************
//**************************************************************************

void rotate_io_bd (int16 dir)
{
	int16 prev_rotate_ix;

	prev_rotate_ix = iobd_rotate_ix;

	if (dir == 1)
	{
		iobd_rotate_ix ++;
		if (iobd_rotate_ix > (max_nmb_io_brd[Car_Select]-fixed_io_brd_dpy))
		{
				iobd_rotate_ix = 1;
		}
	}
	else
	{
		iobd_rotate_ix--;
		if (iobd_rotate_ix < 1)
		{
				iobd_rotate_ix = (max_nmb_io_brd[Car_Select]-fixed_io_brd_dpy);
		}
	}

	if ((max_nmb_io_brd[Car_Select] <= 10) && (iobd_rotate_ix > 1))
		iobd_rotate_ix = 1;			   // no extra boards to rotate


	if (iobd_rotate_ix != prev_rotate_ix)
	{
		Init_Io_Screen(Car_Select,Preset_State);
		Io_Screen(Car_Select,Reset_State);
	}
}

int16 find_brdnmb(int16 Car_Select, int16 brd_ix)
{			   // finds valid io board from rotate index
	int16 i,j;				 // if brd_ix == 5 then it will find the fifth board in the table
	j = 0;
	
	i = fixed_io_brd_dpy;
	while (1)
	{
	 	if (IO_board[Car_Select][io_bd_order[i]] != 0)
		{
			j++;
			if (j == brd_ix)
			{
				return(io_bd_order[i]+1);
			}
			if (j >= (max_nmb_io_brd[Car_Select] - fixed_io_brd_dpy))	  // rotate to beginning
			{
				if (max_nmb_io_brd[Car_Select] <= 10)
					return 0;
			}
			if (j >= max_nmb_io_brd[Car_Select])
				return 0;
		}
		i++;
		if (i >= nmb_io_brds)
//			return(0);
			i = fixed_io_brd_dpy + 1;
	}

	return(0);		// returns zero if no board found
}		
			
//******************************************************
//*** This is the Monitor I/O status initialization  ***
//******************************************************
void Init_Io_Screen(int16 Car_Select, int16 init_flag)
{
    int16 j,k,color;
	static int16 ionmb,bitmsk;
	static int16 state;

	if (init_flag == 1)
	{
		state = 0;
		io_screen_init = 0;
		return;
	}
	else if (init_flag == 2)
	{
		io_screen_init = 0;
		Display_Trace = 0;
		Display_Fault = 0;
		col = fixed_io_brd_dpy;
		row = 1;
		board_nmb = find_brdnmb(Car_Select, iobd_rotate_ix + (col-fixed_io_brd_dpy));
		if (board_nmb != 0)
		{
			ionmb = ((board_nmb-1) * 24);
			state = 7;
		}
		else
			state = 0;
		return;
	}

	switch (state)
	{

		case 0:
			wnum = 0;
			state = 1;
		    ScreenClear();
			break;
		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:	 
			set_font(font_8x14);	// set 8x14 to get 28 rows
			max_row = 24;
			max_car_row = 21;
			max_window_row = 30;
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			nmlscn = vcreat( max_window_row, 80, WHITE);
			vlocate( nmlscn, 0, 0);
			state = 5;
			break;
		case 5:
			vratputf(nmlscn,0,0,YELLOW,"CAR #%.2s                            I/O Screen",car_bld_no[Car_Select]);
			board_nmb = io_bd_order[1];
			row = 1;
			col = 0;
			state = 6;
			ionmb = (board_nmb-1) * 24;
			iobd_rotate_ix = 1;
			break;

		case 6:			  // do the first six blocks on the 1038
#if (TS3300 == 1)
			for(j=1; j<=12; j++)
#else
			for(j=1; j<=6; j++)
#endif
			{		  // Do one board at a time on each display column
				k = ionmb/8;
				bitmsk = bittbl[(row-1) & 0x07];
				if ((IO_In_Out[k] & bitmsk) == 0)
					color = WHITE;		// input
				else
					color = LTCYAN;		// output

				vratputf( nmlscn, row,(col*8),color,"%.5s  ",ioname[ionmb]); 
				vratputf( nmlscn, row,((col*8)+7),WHITE,"%c",Vertical_line);

				if ((io_valid[k] & bitmsk) != 0)
					vratputf( nmlscn, row,((col*8)+5),color,"=");

				row++;
				ionmb++;
			}

			if (row > max_row)
			{		   // do the first six blocks on the 1038
				if ((col < fixed_io_brd_dpy) && ((board_nmb-1) != last_io_brd[Car_Select]))
				{
					row = 1;
					col++;
					board_nmb = io_bd_order[col+1];
					ionmb = ((board_nmb-1) * 24);
				}
				else
				{
					col = fixed_io_brd_dpy;
					board_nmb = find_brdnmb(Car_Select, iobd_rotate_ix + (col-fixed_io_brd_dpy));

					if (board_nmb != 0)
					{
						ionmb = ((board_nmb-1) * 24);
						state = 7;
					}
					else
						state = 8;
					row = 1;
				}
			}
			break;

		case 7:			// Do the next four blocks
#if (TS3300 == 1)
			for(j=1; j<=12; j++)
#else
			for(j=1; j<=6; j++)
#endif
			{		  // Do one board at a time on each display column
				k = ionmb/8;
				bitmsk = bittbl[(row-1) & 0x07];
				if ((IO_In_Out[k] & bitmsk) == 0)
					color = WHITE;		// input
				else
					color = LTCYAN;		// output

				vratputf( nmlscn, row,(col*8),color,"%.5s  ",ioname[ionmb]); 
				vratputf( nmlscn, row,((col*8)+7),WHITE,"%c",Vertical_line);

				if ((io_valid[k] & bitmsk) != 0)
					vratputf( nmlscn, row,((col*8)+5),color,"=");

				row++;
				ionmb++;
			}

			if (row > max_row)
			{
				if (col < 10)
				{
					row = 1;
					col++;
					board_nmb = find_brdnmb(Car_Select, iobd_rotate_ix + (col-fixed_io_brd_dpy));

					if (board_nmb != 0)
						ionmb = ((board_nmb-1) * 24);
					else
						state = 9;
				}
				else
					state = 9;
			}
			break;

		case 8:			// Clear the remaining blocks
#if (TS3300 == 1)
			for(j=1; j<=12; j++)
#else
			for(j=1; j<=6; j++)
#endif
			{		  // Do one board at a time on each display column
				color = WHITE;		// input
				vratputf( nmlscn, row,(col*8),color,"       "); 
				vratputf( nmlscn, row,((col*8)+7),WHITE,"%c",Vertical_line);
				row++;
			}

			if (row > max_row)
			{
				if (col < 10)
				{
					row = 1;
					col++;
				}
				else
					state = 9;
			}
			break;

		case 9:
			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
			    vratputf(nmlscn,25,0,YELLOW,"%.80s",car_status[0]);
			    vratputf(nmlscn,26,0,YELLOW,"%.80s",car_status[1]);
			    vratputf(nmlscn,27,0,YELLOW,"%.80s",car_status[2]);
			}
			else
			{
			    vratputf(nmlscn,25,0,YELLOW,"%.80s",car_status_h[0]);
			    vratputf(nmlscn,26,0,YELLOW,"%.80s",car_status_h[1]);
			    vratputf(nmlscn,27,0,YELLOW,"%.80s",car_status_h[2]);
			}
		    vratputf( nmlscn, 25,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 26,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 27,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 28,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 29,20,WHITE,"%c",doub_vert_line);
			state = 10;
			break;
		case 10:
		    vratputf( nmlscn, 25,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 26,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 27,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 28,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 29,41,WHITE,"%c",doub_vert_line);
			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
			    vratputf( nmlscn, 25,61,WHITE,"%c",doub_vert_line);
			    vratputf( nmlscn, 26,61,WHITE,"%c",doub_vert_line);
			    vratputf( nmlscn, 27,61,WHITE,"%c",doub_vert_line);
			}
			state = 11;
			break;
		case 11:			   // Force a display update
			col = 13;
			row = 25;
			upd_dpy_req = 1;
			v_old_pos = -1;
			v_old_procf = -1;
			v_old_servf = -1;
			v_old_firef = -1;
			v_old_doorf = -1;
			v_old_rdoorf = -1;
			v_old_dirf = -1;
			v_old_dpref = -1;
			v_old_cur_flt = -1;
			v_old_enc_dir = -1;
			v_old_Car_Select = -1;
			v_old_statusf = -1;
			v_old_statusf2 = -1;
			v_old_ss_statusf = -1;
			in_fire_door = 0;
			state = 0;
			Io_Screen(Car_Select,Reset_State);		// Reset State flag
			io_screen_init = 1;
			break;
		default:
			break;
	}

}

//***************************************
//*** This is the Monitor i/o status  ***
//***************************************
void Io_Screen(int16 Car_Select, int16 init_flag)
{
	int16 j,k,bitmsk,car_io;
	static int16 ionmb;
	static int16 state;


	if (init_flag == 1)
	{
		Car_IO_Select = Car_Select;
		state = 0;
		return;
	}
	
	Car_IO_Select = Car_Select;
	switch (state)
	{
		case 0:

		  	Track_Screen(Car_Select);
			state = 1;
			row = 1;
			col = 0;
			if (upd_dpy_req == 1)
				update_dpy = 1;		// Start the update at the beginning only
			else
				update_dpy = 0;
			upd_dpy_req = 0;

			board_nmb = io_bd_order[1];
			ionmb = (board_nmb-1) * 24;
			break;

		case 1:

			car_io = Car_Select;

#if (Quick_Update == 1)
			for(j=1; j<=12; j++)  // DISPLAY INPUTS
#else
			for(j=1; j<=6; j++)  // DISPLAY INPUTS
#endif
			{
				k = ionmb/8;

				bitmsk = bittbl[(row-1) & 0x07];
				if ((io_valid[k] & bitmsk) != 0)
				{
					if ((IO_In_Out[k] & bitmsk) == 0)
					{		// Inputs
						if((iodata[car_io][k] & bitmsk) != 0)
						    vratputf( nmlscn, row,((col*8)+6),GREEN,"1");
						else
						    vratputf( nmlscn, row,((col*8)+6),WHITE,"0");
			    	}
					else
					{	   // Outputs
						if((iodata[car_io][k] & bitmsk) != 0)
						    vratputf( nmlscn, row,((col*8)+6),LTCYAN,"0");
						else
						    vratputf( nmlscn, row,((col*8)+6),RED,"1");
				    }
				}
				row++;
				ionmb++;
			}

			if (row > max_row)
			{		   // do the first six blocks on the 1038
				if ((col < (fixed_io_brd_dpy-1)) && (board_nmb != last_io_brd[Car_Select]))
				{
					row = 1;
					col++;
					board_nmb++;
					ionmb = ((board_nmb-1) * 24);
				}
				else
				{
					col = fixed_io_brd_dpy;
					board_nmb = find_brdnmb(Car_Select, iobd_rotate_ix + (col-fixed_io_brd_dpy));
					if (board_nmb != 0)
					{
						ionmb = ((board_nmb-1) * 24);
						state = 2;
					}
					else
						state = 0;
					row = 1;
				}
			}
			break;
		case 2:

			car_io = Car_Select;

#if (Quick_Update == 1)
			for(j=1; j<=12; j++)  // DISPLAY INPUTS
#else
			for(j=1; j<=6; j++)  // DISPLAY INPUTS
#endif
			{
				k = ionmb/8;

				bitmsk = bittbl[(row-1) & 0x07];
				if ((io_valid[k] & bitmsk) != 0)
				{
					if ((IO_In_Out[k] & bitmsk) == 0)
					{		// Inputs
						if((iodata[car_io][k] & bitmsk) != 0)
						    vratputf( nmlscn, row,((col*8)+6),GREEN,"1");
						else
						    vratputf( nmlscn, row,((col*8)+6),WHITE,"0");
			    	}
					else
					{	   // Outputs
						if((iodata[car_io][k] & bitmsk) != 0)
						    vratputf( nmlscn, row,((col*8)+6),LTCYAN,"0");
						else
						    vratputf( nmlscn, row,((col*8)+6),RED,"1");
				    }
				}
				row++;
				ionmb++;
			}

			if (row > max_row)
			{
				if (col < 10)
				{
					row = 1;
					col++;
					board_nmb = find_brdnmb(Car_Select, iobd_rotate_ix + (col-fixed_io_brd_dpy));
					if (board_nmb != 0)
						ionmb = ((board_nmb-1) * 24);
					else
						state = 0;
				}
				else
					state = 0;
			}
			break;
		default:
			break;
	}
	
	
	return;
}


//*************************************************
//***     THIS IS THE VIDEO TRACK MODE PROC     ***
//*************************************************
void Track_Screen(int16 Car_Select)
{
	int16 j,procfv,firefv,hsfv,pos,doorfv,rdoorfv,servfv,dirfv,dprefv,ss_statv; 
	uint8 ins_servfv;
	int16 cur_fltv;
	int32 statusfv;
	int32 statusf2v;


#if (Traction == 1)
	int16 enc_dirv,enc_velv,dmd_velv;
	int32 pls_cntv,flr_cntv;
#endif
	int16 ulv,dlv,upd_pi,attr;
	int16 row,col;


	// Update local io's
	pos = grp_Car_Stat[Car_Select].g_pos;
	procfv = grp_Car_Stat[Car_Select].g_procf;
	servfv = grp_Car_Stat[Car_Select].g_servf;
	firefv = grp_Car_Stat[Car_Select].g_firef;
	doorfv = grp_Car_Stat[Car_Select].g_doorf;
	rdoorfv = grp_Car_Stat[Car_Select].g_rdoorf;
	dirfv = grp_Car_Stat[Car_Select].g_dirf;
	dprefv = grp_Car_Stat[Car_Select].g_dpref;
    cur_fltv = grp_Car_Stat[Car_Select].g_cur_flt;
	statusfv = grp_Car_Stat[Car_Select].g_statusf1;
	statusf2v = grp_Car_Stat[Car_Select].g_statusf2;
	ss_statv = grp_Car_Stat[Car_Select].g_ss_stat;
	ins_servfv = grp_Car_Stat[Car_Select].g_ins_servf;
#if (Traction == 1)
	enc_dirv = grp_Car_Stat[Car_Select].g_enc_dir;
	enc_velv = grp_Car_Stat[Car_Select].g_enc_vel;
	dmd_velv = grp_Car_Stat[Car_Select].g_dmd_vel;
	pls_cntv = grp_Car_Stat[Car_Select].g_pls_cnt;
	flr_cntv = grp_Car_Stat[Car_Select].g_flr_cnt;
#endif
	upd_pi = 0;

	if (Car_Select != v_old_Car_Select)
	{		// Force update of everything
		v_old_pos = -1;
		v_old_procf = -1;
		v_old_servf = -1;
		v_old_firef = -1;
		v_old_doorf = -1;
		v_old_rdoorf = -1;
		v_old_dirf = -1;
		v_old_dpref = -1;
		v_old_cur_flt = -1;
		v_old_enc_dir = -1;
		v_old_Car_Select = Car_Select;
		v_old_statusf = -1;
		v_old_statusf2 = -1;
		v_old_ss_statusf = -1;
		in_fire_door = 0;
		v_ins_servfv = 0xff;
	}


#if (Traction == 1) 
	if (procfv == 3)
		hsfv = 1;			// Equal 3 momentarily (procf >=4) && (procf <= 11) will determine the display
	else
		hsfv = 0;
#else
  #if (Tract_OL == 1)
  	if( (g_rdcoutp(Car_Select, o_UPF) == 1) && (g_rdcoutp(Car_Select, o_DF) == 1) )		// UPF:B1,DF:B0 - 1,1 = high speed
  #else
	if((g_rdcoutp(Car_Select, o_UPF) == 1) || (g_rdcoutp(Car_Select, o_DF) == 1))
  #endif
		hsfv = 1;
	else
    	hsfv = 0;
#endif

	vid_test = hsfv;

	if ((grp_Car_Stat[Car_Select].g_run_statusf & rs_DL) != 0)
		dlv = 1;
	else
		dlv = 0;

	if ((grp_Car_Stat[Car_Select].g_run_statusf & rs_UL) != 0)
		ulv = 1;
	else
		ulv = 0;
	

	if ((servfv != v_old_servf) || (v_ins_servfv != ins_servfv)) 
	{
		if (servfv > last_servf)
			servfv = 0;
		if (servfv == 1)
			attr = YELLOW;
		else 
			attr = WHITE;
		
		if ((servfv == 0) && (procfv == 2))
		    vratputf(nmlscn,25,0,attr,"%.20s",Inspect_Servf[ins_servfv]);
	  	else
		    vratputf(nmlscn,25,0,attr,"%.20s",Service[servfv]);
		v_old_servf = servfv;
		v_ins_servfv = ins_servfv;
		v_old_doorf = -1;
		v_old_rdoorf = -1;
	}

	// status 
	
	if ((v_old_statusf != statusfv) ||  (v_old_statusf2 != statusf2v) || (v_old_ss_statusf != ss_statv))
	{
		
		upd_pi = 1;
		attr = WHITE;
		j = 0;
		if (((statusfv & sf_SS) != 0) && (ss_statv != 0))
		{
			while (j<=15)
			{
			    if ((ss_statv & bittbl[j++]) != 0)
			    	break;
			}
			vratputf(nmlscn,26,0,attr,"%.19s",LCD_SS_Status[j]);
		}
		else if (statusfv != 0)
		{
			while (j<=31)
			{
			    if ((statusfv & l_bit_mask[j++]) != 0)
			    	break;
			}
			vratputf(nmlscn,26,0,attr,"%.19s",LCD_Statusf[j]);
		}
		else if (statusf2v != 0)
		{
			while (j<=31)
			{
			    if ((statusf2v & l_bit_mask[j++]) != 0)
			    	break;
			}
				vratputf(nmlscn,26,0,attr,"%.19s",LCD_Statusf2[j]);
		}
		else 
		{
			attr = YELLOW;
			vratputf(nmlscn,26,0,attr,"%.19s",LCD_Statusf[0]);
		}
	}
    v_old_statusf = statusfv;
    v_old_statusf2 = statusf2v;
    v_old_ss_statusf = ss_statv;
			
	// 	 Fire operation
	if (firefv != v_old_firef)
	{
		if (firefv == 0)
			attr = YELLOW;
    	else
			attr = WHITE;
    	
    	vratputf(nmlscn,27,0,attr,"%.19s",fire_status_dpy[firefv]);
		upd_pi = 1;
	}
	v_old_firef = firef;

	// door opearation
	
	if (doorfv != v_old_doorf)
	{
		if (doorfv == 0)
			attr = YELLOW;
		else
			attr = WHITE;
		if ((servfv == 1) && (doorfv == 2))
	    	vratputf(nmlscn,28,22,attr,"Elev Door Dwelling ");
		else
	    	vratputf(nmlscn,28,22,attr,"%.19s",LCD_Doorf[doorfv]);
	    v_old_doorf = doorfv;
		upd_pi = 1;
	}
	if(rdoorfv != v_old_rdoorf)
	{
		if (rdoorfv == 0)
			attr = YELLOW;
		else
			attr = WHITE;
		
		if (rear_op[Car_Select] == 0)
	    	vratputf(nmlscn,29,22,YELLOW,"No Rear Door       ");
		
		else if ((servfv == 1) && (rdoorfv == 2))
	    	vratputf(nmlscn,29,22,attr,"Elev Door Dwelling ");
		else
	    	vratputf(nmlscn,29,22,attr,"%.19s",LCD_Rdoorf[rdoorfv]);
	    v_old_rdoorf = rdoorfv;
		upd_pi = 1;
	}

	if (procfv == 3)
	{
		if (dirfv == 1)
		{				// moving up
			if (hsfv == 1)
				j = 3;	  // procf==3 && hsf==1 && dirf==1
			else
			{
				if (ulv == 0)
					j = 4;	// procf== 3 && hsf==0 && UL==0
				else
					j = 5;	// procf== 3 && hsf==0 && UL==1
			}
		}
		else
		{
			if (hsfv == 1)
				j = 6;	  // procf== 3 && hsf==1 && dirf==2
			else
			{
				if (dlv == 0)
					j = 7;	// procf== 3 && hsf==0 && DL==0
				else
					j = 8;	// procf== 3 && hsf==0 && DL==1
			}
		}
		vratputf(nmlscn,28,0,WHITE,"%.19s",LCD_Procf[j]);
		v_old_procf = -1;		// force an update
	}
	else if (procfv != v_old_procf) 
	{
		vid_test = 0;
 		if (procfv < 3)
			j = procfv;
		else
			j = procfv + 5;
    
    	if (procfv == 15) 
			attr = YELLOW;
		else 
			attr = WHITE;
		vratputf(nmlscn,28,0,attr,"%.19s",LCD_Procf[j]);
		v_old_procf = procfv;
		if (procfv !=0) 
			upd_pi = 1;
	}
	
	if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
		(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
		(Drive_Type[Car_Select] == 16))
	{
		row = 25;
		col = 74;
	}
	else
	{
		row = 26;
		col = 33;
	}
	if (dirfv != v_old_dirf)
	{
		if (dirfv == 1)
			vratputf(nmlscn,row,col,WHITE,"Up  ");
		else if (dirfv == 2)
			vratputf(nmlscn,row,col,WHITE,"Down");
		else 
			vratputf(nmlscn,row,col,YELLOW,"Stop");
		v_old_dirf = dirfv;
		upd_pi = 1;
	}	

	if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
		(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
		(Drive_Type[Car_Select] == 16))
	{
		row = 27;
		col = 74;
	}
	else
	{
		row = 27;
		col = 33;
	}
	if (dprefv != v_old_dpref)
	{
		v_old_dpref = dprefv;
		if (dprefv == 1)
			vratputf(nmlscn,row,col,WHITE,"Up  ");
		else if (dprefv == 2)
			vratputf(nmlscn,row,col,WHITE,"Down");
		else 
			vratputf(nmlscn,row,col,YELLOW,"None");
	}


	if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
		(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
		(Drive_Type[Car_Select] == 16))
	{
		row = 25;
		col = 49;
	}
	else
	{
		row = 25;
		col = 28;
	}
	if (((pos != v_old_pos) || (upd_pi == 1)) && (procfv != 0))
	{
		if (dirfv == 0)
			attr = YELLOW;
		else
			attr = WHITE;
		vratputf(nmlscn,row,col,attr,"%2i",pos);
		vratputf(nmlscn,row,col+9,attr,"%.2s",flmrk[pos]);
		v_old_pos = pos;
	}

//	upd_pi = 0;

#if (Traction == 1)

	if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
		(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
		(Drive_Type[Car_Select] == 16))
	{
		if (v_old_enc_dir != enc_dirv)
		{
			if (enc_dirv == 1)
				vratputf(nmlscn,26,74,WHITE,"Up  ");
			else if (enc_dirv == 2)
				vratputf(nmlscn,26,74,WHITE,"Down");
			else 
				vratputf(nmlscn,26,74,YELLOW,"None");
			v_old_enc_dir = enc_dirv;
		}
		if (dmd_velv == 0)
			attr = YELLOW;
		else
			attr = WHITE;
		vratputf(nmlscn,25,31,attr,"%5i",(dmd_velv & 0xffff));
		vratputf(nmlscn,26,31,attr,"%5i",(enc_velv & 0xffff));
		vratputf(nmlscn,26,36,attr," ");
		if(abs(dmd_velv-enc_velv)>9999)
			vratputf(nmlscn,27,31,attr,"%5i",9999);
		else
			vratputf(nmlscn,27,31,attr,"%5i",dmd_velv-enc_velv);
		vratputf(nmlscn,27,36,attr," ");
		vratputf(nmlscn,26,51,attr,"%9ld",flr_cntv);
		vratputf(nmlscn,26,60,attr," ");
		vratputf(nmlscn,27,51,attr,"%9ld",pls_cntv);
		vratputf(nmlscn,27,60,attr," ");
	}

#endif


	if (v_old_cur_flt != cur_fltv)
	{
		if (cur_fltv == 0)
			vratputf(nmlscn,29,0,YELLOW,"No Current Fault    ");
		else 
			vratputf(nmlscn,29,0,WHITE,"%.20s",&LCD_Fault[cur_fltv][3]);
		v_old_cur_flt = cur_fltv;
	}

	return;
}

int16 Init_Vertical_Lines(int16 state)
{
	switch (state)
	{

		case 0:
			state = 1;
			break;
			
		case 1:

			vratputf( nmlscn, 28,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,39,WHITE,"%c",Vertical_line);
			state = 2;
			break;
			
		case 2:

			vratputf( nmlscn, 28,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,55,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,63,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,71,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,79,WHITE,"%c",Vertical_line);
			state = 3;
			break;
			
		case 3:
			vratputf( nmlscn, 29,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,39,WHITE,"%c",Vertical_line);
			state = 4;
			break;
			
		case 4:
			vratputf( nmlscn, 29,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,55,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,63,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,71,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,79,WHITE,"%c",Vertical_line);
			state = 5;
			break;
			
		case 5:
			vratputf( nmlscn, 30,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,39,WHITE,"%c",Vertical_line);
			state = 6;
			break;
			
		case 6:
			vratputf( nmlscn, 30,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,55,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,63,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,71,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,79,WHITE,"%c",Vertical_line);
			state = 7;
			break;
			
		case 7:
			vratputf( nmlscn, 31,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,39,WHITE,"%c",Vertical_line);
			state = 8;
			break;
			
		case 8:
			vratputf( nmlscn, 31,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,55,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,63,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,71,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,79,WHITE,"%c",Vertical_line);
			state = 9;
			break;
			
			
		case 9:			   
			state = 0;
			break;
			
		default:
			break;
	}
			
	return (state);

}



int16 trace_io_array_tbl[30] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,18,19,138,140,141,142,143,144,145,146,147,149,153,156,150,152};

//******************************************************
//*** This is the Monitor Trace I/O status initialization  ***
//******************************************************
int16 Init_Trace_Io_Screen(int16 Car_Select, int16 state)
{
    int16 j,color;
	static int16 io_array, array_ix;
	static int16 ionmb, bitmsk;

	switch (state)
	{

		case 0:
			state = 1;
			Trace_Track_Rotate_Ix = 0;
			break;
		case 1:
			wnum = 0;
			state = 2;
		    ScreenClear();
			break;
		case 2:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 3;
			else
				wnum++;
			break;
		case 3:	 
			set_font(font_8x14);	// set 8x14 to get 28 rows
			max_row = 24;
			max_car_row = 21;
			max_window_row = 30;
			state = 4;
			break;
		case 4:
			setcursortype(_NOCURSOR);
			state = 5;
			break;
		case 5:
			nmlscn = vcreat( max_window_row, 80, WHITE);
			vlocate( nmlscn, 0, 0);
			state = 6;
			break;
		case 6:
			
			if (Display_Trace == 1)	
				vratputf(nmlscn,0,0,YELLOW,"CAR #%.2s                      Trace I/O Screen",car_bld_no[Car_Select]);
			else
				vratputf(nmlscn,0,0,YELLOW,"Car#%.2s",car_bld_no[Car_Select]);
			
			board_nmb = io_bd_order[1];
			row = 1;
			col = 0;
			state = 7;
			io_array = 0;
			array_ix = 0;
			break;

		case 7:			  // do the first six blocks on the 1102
		    io_array = trace_io_array_tbl[array_ix];
		    ionmb = io_array * 8;
			for(j=1; j<=8; j++)
			{		  // Do one board at a time on each display column
				bitmsk = bittbl[(row-1) & 0x07];
				if ((IO_In_Out[io_array] & bitmsk) == 0)
					color = WHITE;		// input
				else
					color = LTCYAN;		// output

				vratputf( nmlscn, row,(col*8),color,"%.5s  ",ioname[ionmb]); 
				vratputf( nmlscn, row,((col*8)+7),WHITE,"%c",Vertical_line);

				if ((io_valid[io_array] & bitmsk) != 0)
					vratputf( nmlscn, row,((col*8)+5),color,"=");

				row++;
				ionmb++;
			}
			if (row > max_row)
			{		   // do the first six blocks on the 1038
				if (col < 9) 
				{
					row = 1;
					col++;
				}
			}
			array_ix++;
			if (array_ix > 14)
				state = 8;
			break;

		case 8:			// Do the next four blocks
		    io_array = trace_io_array_tbl[array_ix];
		    ionmb = io_array * 8;
			for(j=1; j<=8; j++)
			{		  // Do one board at a time on each display column
				bitmsk = bittbl[(row-1) & 0x07];
				if ((IO_In_Out[io_array] & bitmsk) == 0)
					color = WHITE;		// input
				else
					color = LTCYAN;		// output

				vratputf( nmlscn, row,(col*8),color,"%.5s  ",ioname[ionmb]); 
				vratputf( nmlscn, row,((col*8)+7),WHITE,"%c",Vertical_line);

				if ((io_valid[io_array] & bitmsk) != 0)
					vratputf( nmlscn, row,((col*8)+5),color,"=");

				row++;
				ionmb++;
			}

			if (row > max_row)
			{		   // do the first six blocks on the 1038
				if (col < 9) 
				{
					row = 1;
					col++;
				}
			}
			array_ix++;
			if (cons[rear] != 0)
			{
				if (array_ix > 29)
					state = 10;
			}
			else
			{
				if (array_ix > 27)
					state = 9;
			}
			break;

		case 9:			// Clear the remaining blocks
		    io_array = trace_io_array_tbl[array_ix];
		    ionmb = io_array * 8;
			if (array_ix <= 29)
			{
				for(j=1; j<=8; j++)
				{		  // Do one board at a time on each display column
					color = WHITE;		// input
					vratputf( nmlscn, row,(col*8),color,"       "); 
					vratputf( nmlscn, row,((col*8)+7),WHITE,"%c",Vertical_line);
					row++;
				}

				if (row > max_row)
				{		   // do the first six blocks on the 1038
					if (col < 9) 
					{
						row = 1;
						col++;
					}
				}
			}
			array_ix++;
			if (array_ix > 29)
				state = 10;
			
			break;

		case 10:
			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
			    vratputf(nmlscn,25,0,YELLOW,"%.80s",car_status[0]);
			    vratputf(nmlscn,26,0,YELLOW,"%.80s",car_status[9]);
			    vratputf(nmlscn,27,0,YELLOW,"%.80s",car_status[2]);
			}
			else
			{
			    vratputf(nmlscn,25,0,YELLOW,"%.80s",car_status_h[0]);
			    vratputf(nmlscn,26,0,YELLOW,"%.80s",car_status_h[1]);
			    vratputf(nmlscn,27,0,YELLOW,"%.80s",car_status_h[2]);
			}
		    vratputf( nmlscn, 25,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 26,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 27,20,WHITE,"%c",doub_vert_line);
			state = 11;
		    break;
		   
		case 11:
		    vratputf( nmlscn, 25,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 26,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 27,41,WHITE,"%c",doub_vert_line);
			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
			    vratputf( nmlscn, 25,61,WHITE,"%c",doub_vert_line);
			    vratputf( nmlscn, 26,61,WHITE,"%c",doub_vert_line);
			    vratputf( nmlscn, 27,61,WHITE,"%c",doub_vert_line);
			}
			sub_state = 0;
			state = 12;
			break;
			
		case 12:
			sub_state = Init_Trace_Block_1(sub_state);
			if (sub_state == 0)
				state = 13;
			break;
			
		case 13:			   // Force a display update
			state = 0;
			Trace_Track_Rotate_Ix = 0;
			Prev_Trace_Track_Rotate_Ix = Trace_Track_Rotate_Ix;
			trace_dpy_ix = 0;
			trace_screen_state = Trace_Io_Screen(Car_Select,0,&S_Trace[trace_dpy_ix]);
			break;
			
		default:
			break;
	}
	
	return (state);

}

int16 Init_Trace_Block(int16 state)
{
	static int16 init_trace_state;

	switch (state)
	{

		case 0:
			state = 1;
			init_trace_state = 0;
			break;
			
		case 1:
			if (Trace_Track_Rotate_Ix == 0)
		  		init_trace_state = Init_Trace_Block_1(init_trace_state);
			else if (Trace_Track_Rotate_Ix == 1)
		  		init_trace_state = Init_Trace_Block_2(init_trace_state);
			else if (Trace_Track_Rotate_Ix == 2)
		  		init_trace_state = Init_Trace_Block_3(init_trace_state);
			else if (Trace_Track_Rotate_Ix == 3)
		  		init_trace_state = Init_Trace_Block_4(init_trace_state);
			else if (Trace_Track_Rotate_Ix == 4)
		  		init_trace_state = Init_Trace_Block_5(init_trace_state);
			else if (Trace_Track_Rotate_Ix == 5)
		  		init_trace_state = Init_Trace_Block_6(init_trace_state);
			else if (Trace_Track_Rotate_Ix == 6)
		  		init_trace_state = Init_Trace_Block_7(init_trace_state);
																		  
			if (init_trace_state == 0)																		  
				state = 2;
			break;
			
		case 2:
			state = 0;
			break;
			
		default:
			break;
	}
			
	return (state);

}


int16 Init_Trace_Block_1(int16 state)
{
 
	switch (state)
	{

		case 0:
			state = 1;
			break;
			
		case 1:

		    vratputf(nmlscn,28,0,YELLOW,"%.80s",car_status[3]);
		    vratputf(nmlscn,29,0,YELLOW,"%.80s",car_status[4]);
		    vratputf(nmlscn,30,0,YELLOW,"%.80s",car_status[5]);
		    vratputf(nmlscn,31,0,YELLOW,"%.80s",car_status[6]);
		    vratputf(nmlscn,32,0,YELLOW,"%.80s",car_status[7]);
		    vratputf(nmlscn,33,0,YELLOW,"%.80s",car_status[8]);

			state = 2;
			break;
			
		case 2:
		    vratputf( nmlscn, 28,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 29,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 30,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 31,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 32,20,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 33,20,WHITE,"%c",doub_vert_line);
			state = 3;
			break;
			
		case 3:
		    vratputf( nmlscn, 28,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 29,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 30,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 31,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 32,41,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 33,41,WHITE,"%c",doub_vert_line);
		    state = 4;
		    break;
		    
		case 4:
		    vratputf( nmlscn, 28,61,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 29,61,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 30,61,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 31,61,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 32,55,WHITE,"%c",doub_vert_line);
		    vratputf( nmlscn, 33,55,WHITE,"%c",doub_vert_line);
			state = 5;
			break;
			
		case 5:			   
			state = 0;
			break;
			
		default:
			break;
	}
			
	return (state);

}


int16 Init_Trace_Block_2 (int16 state)
{

	switch (state)
	{

		case 0:
			state = 1;
			break;
			
		case 1:

		    vratputf(nmlscn,28,0,YELLOW,"%.80s",car_status_2[0]);
		    vratputf(nmlscn,29,0,YELLOW,"%.80s",car_status_2[1]);
		    vratputf(nmlscn,30,0,YELLOW,"%.80s",car_status_2[2]);
		    vratputf(nmlscn,31,0,YELLOW,"%.80s",car_status_2[3]);
		    vratputf(nmlscn,32,0,YELLOW,"%.80s",car_status_2[4]);
		    vratputf(nmlscn,33,0,YELLOW,"%.80s",car_status_2[5]);

			state = 2;
			break;
			
		case 2:
			vratputf( nmlscn, 28,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,39,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,55,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 28,63,WHITE,"%c",Vertical_line);
			state = 3;
			break;
			
		case 3:
			vratputf( nmlscn, 29,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,39,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,55,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 29,63,WHITE,"%c",Vertical_line);
			state = 4;
			break;
			
		case 4:
			vratputf( nmlscn, 30,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,39,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,55,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 30,63,WHITE,"%c",Vertical_line);
			state = 5;
			break;
			
		case 5:
			vratputf( nmlscn, 31,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,39,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,55,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 31,63,WHITE,"%c",Vertical_line);
			state = 6;
			break;
			
			
		case 6:
		    vratputf( nmlscn, 32,19,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,37,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,55,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,19,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,37,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,55,WHITE,"%c",Vertical_line);
		    state = 7;
		    break;
		    
			
		case 7:			   
			state = 0;
			break;
			
		default:
			break;
	}
			
	return (state);

}


int16 Init_Trace_Block_3(int16 state)
{

	switch (state)
	{

		case 0:
			state = 1;
			break;
			
		case 1:

		    vratputf(nmlscn,28,0,YELLOW,"%.80s",car_status_3[0]);
		    vratputf(nmlscn,29,0,YELLOW,"%.80s",car_status_3[1]);
		    vratputf(nmlscn,30,0,YELLOW,"%.80s",car_status_3[2]);
		    vratputf(nmlscn,31,0,YELLOW,"%.80s",car_status_3[3]);
		    vratputf(nmlscn,32,0,YELLOW,"%.80s",car_status_3[4]);
		    vratputf(nmlscn,33,0,YELLOW,"%.80s",car_status_3[5]);
			v_sub_state = 0;
			state = 2;
			break;
			
		case 2:
			v_sub_state = Init_Vertical_Lines(v_sub_state);
			if (v_sub_state == 0)
				state = 3;
			break;
			
			
		case 3:
		    vratputf( nmlscn, 32,7,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,15,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,39,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,47,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,55,WHITE,"%c",Vertical_line);
		    state = 4;
		    break;
		    
		case 4:
		    vratputf( nmlscn, 33,15,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,39,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,47,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,55,WHITE,"%c",Vertical_line);
		    state = 5;
		    break;
		    
			
		case 5:			   
			state = 0;
			break;
			
		default:
			break;
	}
			
	return (state);

}

int16 Init_Trace_Block_4(int16 state)
{

	switch (state)
	{

		case 0:
			state = 1;
			break;
			
		case 1:
		    vratputf(nmlscn,28,0,YELLOW,"%.80s",car_status_4[0]);
		    vratputf(nmlscn,29,0,YELLOW,"%.80s",car_status_4[1]);
		    vratputf(nmlscn,30,0,YELLOW,"%.80s",car_status_4[2]);
		    vratputf(nmlscn,31,0,YELLOW,"%.80s",car_status_4[3]);
		    vratputf(nmlscn,32,0,YELLOW,"%.80s",car_status_4[4]);
		    vratputf(nmlscn,33,0,YELLOW,"%.80s",car_status_4[5]);

			state = 2;
			break;
			
		case 2:
			v_sub_state = Init_Vertical_Lines(v_sub_state);
			if (v_sub_state == 0)
				state = 3;
			break;
			
			
		case 3:
		    vratputf( nmlscn, 32,15,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,23,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,31,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,39,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,47,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,55,WHITE,"%c",Vertical_line);
		    state = 4;
		    break;
		    
		case 4:
		    vratputf( nmlscn, 33,18,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,39,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,47,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,55,WHITE,"%c",Vertical_line);
		    state = 5;
		    break;
		    
			
		case 5:			   
			state = 0;
			break;
			
		default:
			break;
	}
			
	return (state);

}


int16 Init_Trace_Block_5(int16 state)
{

	switch (state)
	{

		case 0:
			state = 1;
			break;
			
		case 1:
		    vratputf(nmlscn,28,0,YELLOW,"%.80s",car_status_5[0]);
		    vratputf(nmlscn,29,0,YELLOW,"%.80s",car_status_5[1]);
		    vratputf(nmlscn,30,0,YELLOW,"%.80s",car_status_5[2]);
		    vratputf(nmlscn,31,0,YELLOW,"%.80s",car_status_5[3]);
		    vratputf(nmlscn,32,0,YELLOW,"%.80s",car_status_5[4]);
		    vratputf(nmlscn,33,0,YELLOW,"%.80s",car_status_5[5]);

			state = 2;
			break;
			
		case 2:
			v_sub_state = Init_Vertical_Lines(v_sub_state);
			if (v_sub_state == 0)
				state = 3;
			break;
			
			
		case 3:
		    vratputf( nmlscn, 32,15,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,31,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,39,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,47,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 32,55,WHITE,"%c",Vertical_line);
		    state = 4;
		    break;
		    
		case 4:
		    vratputf( nmlscn, 33,15,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,31,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,39,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,47,WHITE,"%c",Vertical_line);
		    vratputf( nmlscn, 33,55,WHITE,"%c",Vertical_line);
		    state = 5;
		    break;
		    
			
		case 5:			   
			state = 0;
			break;
			
		default:
			break;
	}
			
	return (state);

}

int16 Init_Trace_Block_6(int16 state)
{

	switch (state)
	{

		case 0:
			state = 1;
			break;
			
		case 1:
		    vratputf(nmlscn,28,0,YELLOW,"%.80s",car_status_6[0]);
		    vratputf(nmlscn,29,0,YELLOW,"%.80s",car_status_6[1]);
		    vratputf(nmlscn,30,0,YELLOW,"%.80s",car_status_6[2]);
		    vratputf(nmlscn,31,0,YELLOW,"%.80s",car_status_6[3]);
		    vratputf(nmlscn,32,0,YELLOW,"%.80s",car_status_6[4]);
		    vratputf(nmlscn,33,0,YELLOW,"%.80s",car_status_6[5]);

			state = 2;
			break;
			
		case 2:
			v_sub_state = Init_Vertical_Lines(v_sub_state);
			if (v_sub_state == 0)
				state = 3;
			break;
			
			
		case 3:
			vratputf( nmlscn, 32,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,39,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,55,WHITE,"%c",Vertical_line);
			state = 4;
			break;
		    
		case 4:
			vratputf( nmlscn, 33,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,39,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,55,WHITE,"%c",Vertical_line);
			state = 5;
			break;
		    
			
		case 5:			   
			state = 0;
			break;
			
		default:
			break;
	}
			
	return (state);

}

int16 Init_Trace_Block_7(int16 state)
{

	switch (state)
	{

		case 0:
			state = 1;
			break;
			
		case 1:
		    vratputf(nmlscn,28,0,YELLOW,"%.80s",car_status_7[0]);
		    vratputf(nmlscn,29,0,YELLOW,"%.80s",car_status_7[1]);
		    vratputf(nmlscn,30,0,YELLOW,"%.80s",car_status_7[2]);
		    vratputf(nmlscn,31,0,YELLOW,"%.80s",car_status_7[3]);
		    vratputf(nmlscn,32,0,YELLOW,"%.80s",car_status_7[4]);
		    vratputf(nmlscn,33,0,YELLOW,"%.80s",car_status_7[5]);

			state = 2;
			break;
			
		case 2:
			v_sub_state = Init_Vertical_Lines(v_sub_state);
			if (v_sub_state == 0)
				state = 3;
			break;
			
			
		case 3:
			vratputf( nmlscn, 32,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,39,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 32,55,WHITE,"%c",Vertical_line);
			state = 4;
			break;
		    
		case 4:
			vratputf( nmlscn, 33,7,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,15,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,23,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,31,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,39,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,47,WHITE,"%c",Vertical_line);
			vratputf( nmlscn, 33,55,WHITE,"%c",Vertical_line);
			state = 5;
			break;
		    
			
		case 5:			   
			state = 0;
			break;

		default:
			break;
	}
			
	return (state);

}



//***************************************
//*** This is the Monitor i/o status  ***
//***************************************
int16 Trace_Io_Screen(int16 Car_Select, int16 state, struct Fault *ptr)
{
	int16 j;
	static int16 io_array, array_ix;
	static int16 ionmb, bitmsk;
	static int16 trace_state;


	switch (state)
	{
		case 0:
			state = 1;
			trace_state = 0;
			break;
			
		case 1:

		  	trace_state = Trace_Track_Screen(Car_Select,ptr,trace_state);
		  	if (trace_state == 0)
				state = 2;
			break;
		case 2:

			if (Trace_Track_Rotate_Ix == 0)
		  		trace_state = Trace_Track_Screen_1(Car_Select,ptr,trace_state);
			else if (Trace_Track_Rotate_Ix == 1)
		  		trace_state = Trace_Track_Screen_2(ptr,trace_state);
			else if (Trace_Track_Rotate_Ix == 2)
		  		trace_state = Trace_Track_Screen_3(ptr,trace_state);
			else if (Trace_Track_Rotate_Ix == 3)
		  		trace_state = Trace_Track_Screen_4(ptr,trace_state);
			else if (Trace_Track_Rotate_Ix == 4)
		  		trace_state = Trace_Track_Screen_5(ptr,trace_state);
			else if (Trace_Track_Rotate_Ix == 5)
		  		trace_state = Trace_Track_Screen_6(ptr,trace_state);
			else if (Trace_Track_Rotate_Ix == 6)
		  		trace_state = Trace_Track_Screen_7(ptr,trace_state);
			
			if (trace_state == 0)
				state = 3;
			row = 1;
			col = 0;
			io_array = 0;
			array_ix = 0;
			break;

		case 3:

		    io_array = trace_io_array_tbl[array_ix];
		    ionmb = io_array * 8;
			for(j=1; j<=8; j++)
			{
				bitmsk = bittbl[(row-1) & 0x07];
				if ((io_valid[io_array] & bitmsk) != 0)
				{
					if ((IO_In_Out[io_array] & bitmsk) == 0)
					{		// Inputs
						if((ptr->iodata[array_ix] & bitmsk) != 0)
						    vratputf( nmlscn, row,((col*8)+6),GREEN,"1");
						else
						    vratputf( nmlscn, row,((col*8)+6),WHITE,"0");
			    	}
					else
					{	   // Outputs
						if((ptr->iodata[array_ix] & bitmsk) != 0)
						    vratputf( nmlscn, row,((col*8)+6),LTCYAN,"0");
						else
						    vratputf( nmlscn, row,((col*8)+6),RED,"1");
				    }
				}
				row++;
				ionmb++;
			}

			if (row > max_row)
			{		   // do the first six blocks on the 1038
				if (col < 9) 
				{
					row = 1;
					col++;
				}
			}
			array_ix++;
			if (array_ix > 14)
				state = 4;
			break;
			
		case 4:

		    io_array = trace_io_array_tbl[array_ix];
		    ionmb = io_array * 8;
			for(j=1; j<=8; j++)
			{
				bitmsk = bittbl[(row-1) & 0x07];
				if ((io_valid[io_array] & bitmsk) != 0)
				{
					if ((IO_In_Out[io_array] & bitmsk) == 0)
					{		// Inputs
						if((ptr->iodata[array_ix] & bitmsk) != 0)
						    vratputf( nmlscn, row,((col*8)+6),GREEN,"1");
						else
						    vratputf( nmlscn, row,((col*8)+6),WHITE,"0");
			    	}
					else
					{	   // Outputs
						if((ptr->iodata[array_ix] & bitmsk) != 0)
						    vratputf( nmlscn, row,((col*8)+6),LTCYAN,"0");
						else
						    vratputf( nmlscn, row,((col*8)+6),RED,"1");
				    }
				}
				row++;
				ionmb++;
			}

			if (row > max_row)
			{		   // do the first six blocks on the 1038
				if (col < 9) 
				{
					row = 1;
					col++;
				}
			}
			array_ix++;
			if (cons[rear] != 0)
			{
				if (array_ix > 29)
					state = 0;
			}
			else
			{
				if (array_ix > 27)
					state = 0;
			}
			break;

		default:
			break;
	}
	
	
	return(state);
}

//*************************************************
//***     THIS IS THE VIDEO Trace TRACK MODE PROC     ***
// Call Trace_Track_Screem(cannmb,&S_Trace[Trace_dpy_ix])
//*************************************************
int16 Trace_Track_Screen(int16 Car_Select, struct Fault *ptr, int16 state)
{
	int16 row,col;
    int16 attr,j;
    int16 trace_dpy_offset;
    int16 dpy_offset_ix;
					
					
	switch (state)
	{
		case 0:
			state = 1;
			
		case 1:
		
			if (Display_Trace == 1)	
			{
					// set offset to beginning of trace
				if ((trace_stop_ix + 36) >= max_trace_ix)		// go to beginning of trace
					trace_dpy_offset = (int16)(36 - (max_trace_ix - trace_stop_ix));
				else
					trace_dpy_offset = (int16)(trace_stop_ix + 36);
					
				if (trace_dpy_ix < trace_dpy_offset)
					dpy_offset_ix = (trace_dpy_ix + (max_trace_ix - trace_dpy_offset));
				else
					dpy_offset_ix = trace_dpy_ix - trace_dpy_offset;
				
				vratputf(nmlscn,0,10,WHITE,"Trace ix:%3i,%3i",trace_dpy_ix,dpy_offset_ix);

				if (Trace_Stop == 1)
			    	vratputf(nmlscn,0,64,WHITE,"Trace Buffer ");
				else	
			    	vratputf(nmlscn,0,64,WHITE,"Live Buffer ");
			}

			if (ptr->servf == 1)
				attr = YELLOW;
			else 
				attr = WHITE;
				
			if ((ptr->servf == 0) && (ptr->procf == 2))
			    vratputf(nmlscn,25,0,attr,"%.20s",Inspect_Servf[ptr->ins_servf]);
		  	else
			    vratputf(nmlscn,25,0,attr,"%.20s",Service[ptr->servf]);

			// status 
			
			attr = WHITE;
			j = 0;
			if (((ptr->statusf & sf_SS) != 0) && (ptr->ss_status != 0))
			{
				while (j<=15)
				{
				    if ((ptr->ss_status  & bittbl[j++]) != 0)
				    	break;
				}
				vratputf(nmlscn,26,0,attr,"%.19s",LCD_SS_Status[j]);
			}
			else if (ptr->statusf != 0)
			{
				while (j<=31)
				{
				    if ((ptr->statusf & l_bit_mask[j++]) != 0)
				    	break;
				}
				vratputf(nmlscn,26,0,attr,"%.19s",LCD_Statusf[j]);
			}
			else if (ptr->statusf2 != 0)
			{
				while (j<=31)
				{
				    if ((ptr->statusf2 & l_bit_mask[j++]) != 0)
				    	break;
				}
				vratputf(nmlscn,26,0,attr,"%.19s",LCD_Statusf2[j]);
			}
			else 
			{
				attr = YELLOW;
				vratputf(nmlscn,26,0,attr,"%.19s",LCD_Statusf[0]);
			}
	
			// Process flag

			if (ptr->procf == 3)
			{
				if (ptr->dirf == 1)
				{				// moving up
					if (ptr->hsf == 1)
						j = 3;	  // procf==3 && hsf==1 && dirf==1
					else
					{
						if ((ptr->run_statusf & rs_UL) == 0)
							j = 4;	// procf== 3 && hsf==0 && UL==0
						else
							j = 5;	// procf== 3 && hsf==0 && UL==1
					}
				}
				else
				{
					if (ptr->hsf == 1)
						j = 6;	  // procf== 3 && hsf==1 && dirf==2
					else
					{
						if ((ptr->run_statusf & rs_DL) == 0)
							j = 7;	// procf== 3 && hsf==0 && DL==0
						else
							j = 8;	// procf== 3 && hsf==0 && DL==1
					}
				}
				vratputf(nmlscn,27,0,WHITE,"%.19s",LCD_Procf[j]);
			}
			else  
			{
				vid_test = 0;
		 		if (ptr->procf < 3)
					j = ptr->procf;
				else
					j = (int16)(ptr->procf + 5);
		    
		    	if (ptr->procf == 15) 
					attr = YELLOW;
				else 
					attr = WHITE;
				vratputf(nmlscn,27,0,attr,"%.19s",LCD_Procf[j]);
			}
			
			state = 2;
			break;
			
		case 2:
		
		#if (Traction == 1)

			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
				if (ptr->enc_dir == 1)
					vratputf(nmlscn,26,74,WHITE,"Up  ");
				else if (ptr->enc_dir == 2)
					vratputf(nmlscn,26,74,WHITE,"Down");
				else 
					vratputf(nmlscn,26,74,YELLOW,"None");
				
				if (ptr->dmd_vel_fpm == 0)
					attr = YELLOW;
				else
					attr = WHITE;
				vratputf(nmlscn,25,31,attr,"%5i",(ptr->dmd_vel_fpm & 0xffff));
				vratputf(nmlscn,26,31,attr,"%5i",(ptr->enc_vel_fpm & 0xffff));
				vratputf(nmlscn,26,36,attr," ");
				if(abs(ptr->drv_vel_fpm-ptr->enc_vel_fpm)>9999)
					vratputf(nmlscn,27,31,attr,"%5i",9999);
				else
					vratputf(nmlscn,27,31,attr,"%5i",ptr->dmd_vel_fpm-ptr->enc_vel_fpm);
				
				vratputf(nmlscn,27,36,attr," ");
				vratputf(nmlscn,26,51,attr,"%9ld",ptr->dpp_target);
				vratputf(nmlscn,26,60,attr," ");
				vratputf(nmlscn,27,51,attr,"%9ld",ptr->dpp_count);
				vratputf(nmlscn,27,60,attr," ");
				
				
			}

		#endif
			state = 3;
			break;


		case 3:
		
			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
				row = 25;
				col = 74;
			}
			else
			{
				row = 26;
				col = 33;
			}

			if (ptr->dirf == 1)
				vratputf(nmlscn,row,col,WHITE,"Up  ");
			else if (ptr->dirf == 2)
				vratputf(nmlscn,row,col,WHITE,"Down");
			else 
				vratputf(nmlscn,row,col,YELLOW,"Stop");

			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
				row = 27;
				col = 74;
			}
			else
			{
				row = 27;
				col = 33;
			}

			if (ptr->dpref == 1)
				vratputf(nmlscn,row,col,WHITE,"Up  ");
			else if (ptr->dpref == 2)
				vratputf(nmlscn,row,col,WHITE,"Down");
			else 
				vratputf(nmlscn,row,col,YELLOW,"None");


			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
				row = 25;
				col = 49;
			}
			else
			{
				row = 25;
				col = 28;
			}

			if (ptr->dirf == 0)
				attr = YELLOW;
			else
				attr = WHITE;
			vratputf(nmlscn,row,col,attr,"%2i",ptr->pos);
			vratputf(nmlscn,row,col+9,attr,"%.2s",flmrk[ptr->pos]);
			
			if (Display_Fault == 1)
				state = 4;
			else
				state = 0;
			break;
		
		case 4:
			vratputf(nmlscn,0,7,WHITE,"%2i %.20s ",fault_dpy_ix,&LCD_Fault[ptr->code][3]);
			vratputf(nmlscn,0,31,WHITE,
					"%2i:%02i:%02i %2i/%02i Cnt=%3i ",
					flp->hour,flp->min,flp->sec,
					flp->mon,flp->day,
					flp->count);

			if (((ptr->code == f_MRCAN_device_fault) || (ptr->code == f_CTCAN_device_fault) || (ptr->code == f_GRCAN_device_fault)) && 
				(ptr->device != 0) && (ptr->prog_flag1 != 0))
			{ // display device and device fault with count
								
				Set_Device_Name(Dev_Name,ptr->code,ptr->device);
				// display device name and number of faults
				vratputf(nmlscn,0,54,WHITE,"%.8s:",Dev_Name);
				// Display device fault
				vratputf(nmlscn,0,63,WHITE,"%.20s ",LCD_device_Fault[ptr->prog_flag1]);
									
			}
			else if (((ptr->code == f_MRCAN_device_reset) || (ptr->code == f_CTCAN_device_reset) || (ptr->code == f_GRCAN_device_reset)) &&
				(ptr->device != 0))
			{		// Display Device and device requested initialization
					
				Set_Device_Name(Dev_Name,ptr->code,ptr->device);

				// display device name and number of faults
				vratputf(nmlscn,0,54,WHITE,"%.8s:",Dev_Name);
				// Display device fault
				vratputf(nmlscn,0,63,WHITE," Req Init");
							
			}
			else
				vratputf(nmlscn,0,54,WHITE,"                           ");
			
			state = 0;
			break;
			
		default:
			state = 0;
			break;
	}

	return(state);
}


int16 Trace_Track_Screen_1(int16 Car_Select, struct Fault *ptr, int16 state)
{
    int16 attr,k,j;
    int16 run_attr;
    int16 mtimer;
    
			
	switch (state)
	{
		case 0:
			state = 1;
			
		case 1:
			// 	 Fire operation
			if (ptr->firef == 0)
				attr = YELLOW;
		   	else
				attr = WHITE;
		    	
		   	vratputf(nmlscn,28,0,attr,"%.19s",fire_status_dpy[ptr->firef]);

			// door opearation
			
			if (ptr->doorf == 0)
				attr = YELLOW;
			else
				attr = WHITE;
			
			if ((ptr->servf == 1) && (ptr->doorf == 2))
		    	vratputf(nmlscn,30,0,attr,"Elev Door Dwelling ");
			else
		    	vratputf(nmlscn,30,0,attr,"%.19s",LCD_Doorf[ptr->doorf]);
			
			if (ptr->rdoorf == 0)
				attr = YELLOW;
			else
				attr = WHITE;
				
			if (rear_op[Car_Select] == 0)
		    	vratputf(nmlscn,31,0,YELLOW,"No Rear Door       ");
			else if ((ptr->servf == 1) && (ptr->rdoorf == 2))
			   	vratputf(nmlscn,31,0,attr,"Elev Door Dwelling ");
			else
			   	vratputf(nmlscn,31,0,attr,"%.19s",LCD_Rdoorf[ptr->rdoorf]);
			

			if (ptr->empf == 0)
				attr = YELLOW;
			else
				attr = WHITE;
			
		    vratputf(nmlscn,32,0,attr,"%.20s",empf_status[ptr->empf]);
		    
			if ((ptr->run_statusf & rs_RUN) == 0)
			{
				mtimer = 0;
				run_attr = YELLOW;
			}
			else
			{
				mtimer = (int16)(ptr->motion_tmr & 0xffff);
				run_attr = WHITE;
			}
			vratputf(nmlscn,33,14,run_attr,"%5i",mtimer);
			state = 2;
			break;	

		case 2:
		
		#if (Traction == 1)

			if ((Drive_Type[Car_Select] == 3) || (Drive_Type[Car_Select] == 4) || (Drive_Type[Car_Select] == 5) || (Drive_Type[Car_Select] == 15) || 
				(Drive_Type[Car_Select] == 10) || (Drive_Type[Car_Select] == 11) || (Drive_Type[Car_Select] == 12) || (Drive_Type[Car_Select] == 14) ||
				(Drive_Type[Car_Select] == 16))
			{
				
				if (ptr->drv_vel_fpm == 0)
					attr = YELLOW;
				else
					attr = WHITE;
				vratputf(nmlscn,28,31,attr,"%5i",(ptr->drv_vel_fpm & 0xffff));
				vratputf(nmlscn,29,31,attr,"%5i",(ptr->vel_fpm & 0xffff));

				vratputf(nmlscn,30,31,attr,"%5i",(ptr->SPB_vel & 0xffff));
		 		vratputf(nmlscn,31,31,attr,"%5i",(ptr->NTS_vel & 0xffff));
			
				if ((ptr->run_statusf & rs_RUN) == 0)
					run_attr = YELLOW;
				else
					run_attr = WHITE;
				
				if (ptr->stepf == 1)
					vratputf( nmlscn,32,28,run_attr,"1");
				else
					vratputf( nmlscn,32,28,run_attr,"0");
				
				if (ptr->nstopf == 1)
					vratputf( nmlscn,32,39,run_attr,"1");
				else
					vratputf( nmlscn,32,39,run_attr,"0");
				
				vratputf(nmlscn,33,29,run_attr,"%1i",ptr->dcalls);
				vratputf(nmlscn,33,38,run_attr,"%2i",ptr->next_stop);
				
			}

		#endif

  			state = 3;
			break;	

		case 3:
		
			if (ptr->code == 0)
				vratputf(nmlscn,29,0,YELLOW,"No Current Fault    ");
			else 
				vratputf(nmlscn,29,0,WHITE,"%.20s",&LCD_Fault[ptr->code][3]);
			
			if ((ptr->door_seqf & 0x01) != 0)
				vratputf( nmlscn,28,50,WHITE,"1");
			else
				vratputf( nmlscn,28,50,YELLOW,"0");
			
			if (ptr->estop == 1)
				vratputf( nmlscn,28,59,WHITE,"1");
			else
				vratputf( nmlscn,28,59,YELLOW,"0");

			if (ptr->startf == 1)
				vratputf( nmlscn,29,50,WHITE,"1");
			else
				vratputf( nmlscn,29,50,YELLOW,"0");
			
			if (ptr->hsf == 1)
				vratputf( nmlscn,29,57,WHITE,"1");
			else
				vratputf( nmlscn,29,57,YELLOW,"0");

			j = 0;
			k = 0;
			while ((j<=3) && (k<10))
			{
				if ((ptr->front_sd & bittbl[k]) != 0)
				{
		  			vratputf(nmlscn,30,48+(j*3),WHITE,"%.2s,",slowdown_dpy[k+1]);
					j++;
				}
				k++;
			}
			while (j<=3)
			{
				if (j==0)
					vratputf(nmlscn,30,48+(j*3),YELLOW,"%.2s,",slowdown_dpy[j]);
				else
					vratputf(nmlscn,30,48+(j*3),YELLOW,"   ");
				
				j++;
			}
		 
	 		state = 4;
	 		break;	

		case 4:
			j = 0;
			k = 0;
			while ((j<=3) && (k<10))
			{
				if ((ptr->rear_sd & bittbl[k]) != 0)
				{
		  			vratputf(nmlscn,31,48+(j*3),WHITE,"%.2s,",slowdown_dpy[k+1]);
					j++;
				}
				k++;
			}
			while (j<=3)
			{
				if (j==0)
					vratputf(nmlscn,31,48+(j*3),YELLOW,"%.2s,",slowdown_dpy[j]);
				else
					vratputf(nmlscn,31,48+(j*3),YELLOW,"   ");
				
				j++;
			}

		  	if (ptr->lev_startf == 1)
				vratputf( nmlscn,32,54,WHITE,"1");
			else
				vratputf( nmlscn,32,54,YELLOW,"0");
			
			if (ptr->relevel == 1)
				vratputf( nmlscn,33,51,WHITE,"1");
			else
				vratputf( nmlscn,33,51,YELLOW,"0");
			
		 
		    if ((ptr->drv_cmd & 0x01) != 0)
		    {
		    	attr = WHITE;
		    	if ((ptr->drv_cmd & 0x04) != 0)
		    		j = 3;
		    	else if ((ptr->drv_cmd & 0x08) != 0) 
		    		j = 4;
		    	else
		    		j = 1;
		    }
		    else if ((ptr->drv_cmd & 0x02) != 0)
		    {
		    	attr = WHITE;
		    	j = 2;
		    }
		    else
		    {
		    	attr = YELLOW;
		    	j = 0;
		    }
		    
		 	vratputf(nmlscn,28,71,attr,"%.7s",drv_cmd_dpy[j]);
		 	
	    	if (ptr->drv_status != 0)
			 	vratputf(nmlscn,29,67,WHITE,"%4x",ptr->drv_status);
			else
			 	vratputf(nmlscn,29,67,YELLOW,"%4x",ptr->drv_status);

		 	vratputf(nmlscn,29,76,YELLOW,"%3d",ptr->percent_load);

	    	if (ptr->drv_torque_cmd != 0)
			 	vratputf(nmlscn,30,72,WHITE,"%6i",ptr->drv_torque_cmd);
			 else
			 	vratputf(nmlscn,30,72,YELLOW,"%6i",ptr->drv_torque_cmd);

	    	if (ptr->drv_mtr_torque != 0)
			 	vratputf(nmlscn,31,72,WHITE,"%6i",ptr->drv_mtr_torque);
			else
			 	vratputf(nmlscn,31,72,YELLOW,"%6i",ptr->drv_mtr_torque);
    	
	    	state = 0;
	    	break;
		
		default:
			state = 0;
			break;	 	
	}

	return(state);
}



int16 Trace_Track_Screen_2(struct Fault *ptr, int16 state)
{
	int16 i,j;
	int16 row,col;
	int16 attr;
	
	switch (state)
	{
		case 0:
			state = 1;
			
		case 1:
			row = 28;
			col = 0;
						  
			i = 0;
			while (i<8)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->statusf & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			
			state = 2;
			break;	

		case 2:
			row = 28;
			col = 2;
						  
			i = 8;
			while (i<16)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->statusf & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			
			state = 3;
			break;	

		case 3:
			row = 28;
			col = 4;
						  
			i = 16;
			while (i<24)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->statusf & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			
			state = 4;
			break;	

		case 4:
			row = 28;
			col = 6;
						  
			i = 24;
			while (i<32)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->statusf & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			
			state = 5;
			break;	

		case 5:
		
			vratputf(nmlscn,28,74,YELLOW,"%3i",ptr->device);
			vratputf(nmlscn,29,74,YELLOW,"%3i",ptr->device_2);
			vratputf(nmlscn,30,76,YELLOW,"%3i",ptr->prog_flag1);
			vratputf(nmlscn,31,76,YELLOW,"%3i",ptr->prog_flag2);

			
			if (ptr->medf == 0)
				attr = YELLOW;
			else
				attr = WHITE;
			
		    vratputf(nmlscn,32,0,attr,"%.18s",medf_status[ptr->medf]);
		    
			state = 6;
			break;	

		case 6:
		
			if (ptr->codebf == 0)
				attr = YELLOW;
			else
				attr = WHITE;
			
		    vratputf(nmlscn,33,0,attr,"%.18s",codebf_status[ptr->codebf]);
		    
			vratputf(nmlscn,32,35,YELLOW,"%2i",ptr->chkrun_st);
			vratputf(nmlscn,32,52,YELLOW,"%2i",ptr->chklevel_st);
			vratputf(nmlscn,33,35,YELLOW,"%2i",ptr->chkstart_st);
			vratputf(nmlscn,33,53,YELLOW,"%2i",ptr->chkdoor_st);

			state = 0;
			break;	

		default:
			state = 0;
			break;
	}
	return(state);
}



int16 Trace_Track_Screen_3(struct Fault *ptr, int16 state)
{
	int16 attr;
	int16 row,col;
	int16 i,j;

				/* SPB ios[5] 
				
								//byte0
									BIT0: dlm
									BIT1: dl			
									BIT2: automatic	
									BIT3: gbp		
									BIT4: lbp		
									BIT5: mri		
									BIT6: ul		
									BIT7: ctin		

								//byte1
									BIT0: ici		:1;
									BIT1: acc		:1;
									BIT2: uts		:1;
									BIT3: dts		:1;
									BIT4: rgs		:1;
									BIT5: rlm1		:1;
									BIT6: dlb1		:1;
									BIT7: dzo		:1;

								//byte2
									BIT0: unused20	:1;
									BIT1: unused21	:1;		
									BIT2: ets		:1;
									BIT3: pup		:1;
									BIT4: pdown		:1;
									BIT5: gs		:1;
									BIT6: dlt		:1;
									BIT7: unused27	:1;

								//byte3
									BIT0: sfco:1;					// Secondary Fault Control
									BIT1: picen:1;					// Pic Enable
									BIT2: lscs:1;					// Leveling Speed Control
									BIT3: comflt:1;					// Comm Fault
									BIT4: velflt:1;					// Velocity Fault
									BIT5: posinit:1;				// Position Count Initialized
									BIT6: actflt:1;					// Active Fault
									BIT7: posupd:1;					// Position Count Updated

								//byte4
									BIT0: gr1:1;						// Gripper Relay output
									BIT1: unused
									BIT2: unused
									BIT3: unused
									BIT4: unused
									BIT5: unused
									BIT6: unused
									BIT7: unused
*/

	switch (state)
	{
		case 0:
			state = 1;
			
		case 1:
			row = 28;
			col = 0;
						  
			i = 0;
			for (i=0;i<=1;i++)
			{						// See byte 0 above
				for(j=0;j<=3;j++)
				{
					if ((ptr->SPB_ios[0] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}
			
			state = 2;
			break;	

		case 2:
			row = 28;
			col = 2;
						  
			i = 0;
			for (i=0;i<=1;i++)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->SPB_ios[1] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}
			
			state = 3;
			break;	

		case 3:
			if ((ptr->SPB_ios[4] & 0x01) != 0)		// gr1
				vratputf( nmlscn,28,38,WHITE,"1");
			else
				vratputf( nmlscn,28,38,YELLOW,"0");
			
			if ((ptr->SPB_ios[2] & 0x04) != 0)
				vratputf( nmlscn,29,38,WHITE,"1");
			else
				vratputf( nmlscn,29,38,YELLOW,"0");
			
			row = 30;
			col = 4;
						  
			for(j=0;j<=1;j++)
			{					// pup and pdown
				if ((ptr->SPB_ios[2] & bittbl[j+3]) != 0)
				    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
				else			 
				    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
			}
			
			row = 28;
			col = 5;
			for(j=0;j<=1;j++)
			{
				if ((ptr->SPB_ios[2] & bittbl[j+5]) != 0)
				    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
				else			 
				    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
			}
			
			state = 4;
			break;	

		case 4:
			row = 30;
			col = 5;
						  
			i = 0;
			for (i=0;i<=1;i++)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->SPB_ios[3] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
				row = 28;
			}
			
			state = 5;
			break;	

		case 5:
			
		//			uint16 command;					// Bit 0: 1 = NTS Test (verification)
													// Bit 1: 1 = ETS Test (verification)
													// Bit 2: 1 = Reset Unintended motion fault
													// Bit 3: 1 = Set Unintended motion fault
													// Bit 4: 1 = GR1 test
													// Bit 5: 1 = SFC test
													// Bit 6: 1 = GR1 pick command
													// Bit 7: 1 = Force Car Speed Menu
													// Bit 8: 1 = Trigger Brake (reserved)
													// Bit 9: 1 = Trigger Emergency Brake (reserved)
													// Bit10: 0 = Brk Line Voltage, 1 = Em Brk Line Voltage (reserved)
													// Bit11: 1 = Reset can2 comm status
													// Bit12: 1 = NTS Test
													// Bit13: 1 = ETS Test
													// Bit14: 1 = Automatic Door disabled
													// Bit15: 1 = Set Unintended motion fault (verification)
												
			row = 28;
			col = 7;
						  
			i = 0;
			for (i=0;i<=2;i++)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->SPB_command & bittbl[(i*4)+j+2]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}

			
			state = 6;
			break;	

		case 6:
			if ((ptr->SPB_command & 0x01) != 0)
				vratputf( nmlscn,32,54,WHITE,"1");
			else
				vratputf( nmlscn,32,54,YELLOW,"0");
			
			if ((ptr->SPB_command & 0x02) != 0)
				vratputf( nmlscn,33,54,WHITE,"1");
			else
				vratputf( nmlscn,33,54,YELLOW,"0");
			
			if ((ptr->SPB_command & 0x4000) != 0)
				vratputf( nmlscn,32,6,WHITE,"1");
			else
				vratputf( nmlscn,32,6,YELLOW,"0");
			
			if ((ptr->SPB_command & 0x8000) != 0)
				vratputf( nmlscn,32,14,WHITE,"1");
			else
				vratputf( nmlscn,32,14,YELLOW,"0");
			
			if (ptr->SPB_vel == 0) 
				attr = YELLOW;
			else 
				attr = WHITE;
			
			vratputf(nmlscn,32,27,attr,"%9ld",ptr->SPB_poscnt);
			vratputf(nmlscn,32,38,attr," ");

			vratputf(nmlscn,33,9,attr,"%4i",(ptr->SPB_vel & 0xffff));
			
			if ((ptr->SPB_ios[4] & 0x40) != 0)
				vratputf(nmlscn,33,14,WHITE,"U");
			else if ((ptr->SPB_ios[4] & 0x80) != 0)
				vratputf(nmlscn,33,14,WHITE,"D");
			else
				vratputf(nmlscn,33,14,YELLOW," ");
			
			
			if (ptr->SPB_service == 0) 
				attr = YELLOW;
			else 
				attr = WHITE;

			vratputf(nmlscn,33,21,attr,"%.6s",spb_service[ptr->SPB_service]);
/*				
			"SFC",		// sfc relay  0x01 
			"PIH",		// Pic Enable	0x02
			"GRF",		// Gripper Picked 0x04
			"COM",		// Comm fault 0x08
			"VEL",		// Velocity fault 0x10
			"PCI",		// Position initialized	0x20
			"ACT",		// Active Fault 0x40
			"PCU",		// Position Updated 0x80
			"OK ",
*/
			if ((ptr->SPB_status & 0x4) != 0)
				j = 2; // gripper fault
			else if ((ptr->SPB_status & 0x10) != 0)
				j = 4; // velocity fault
			else if ((ptr->SPB_status & 0x1) != 0)
				j = 0; // sfc off
			else if ((ptr->SPB_status & 0x40) != 0)
				j = 6; // active fault
			else if ((ptr->SPB_status & 0x2) != 0)
				j = 1; // PIC Not Enabled			// picen is inverted in comm unpack
			else if ((ptr->SPB_status & 0x8) != 0)
				j = 3; // com fault
//			else if ((ptr->SPB_status & 0x20) != 0)
//				j = 5; // Position Initialized
//			else if ((ptr->SPB_status & 0x80) != 0)
//				j = 7; // Position Updated
			else 
				j = 8;	// ok
				
		    if (j >= 8) 
				attr = YELLOW;
			else 
				attr = WHITE;
			vratputf(nmlscn,33,29,attr,"%.3s",spb_status[j]);
			state = 0;
			break;	

		default:
			state = 0;
			break;
	}
	
	return(state);

}



int16 Trace_Track_Screen_4(struct Fault *ptr,int16 state)
{
	int16 attr;
	int16 row,col;
	int16 i,j,k;

//			unsigned char ios[3];

									// byte 0
										//  Bit0:	un;
										//  Bit1:	ut;
										//  Bit2:	ut1;
										//  Bit3:	ut2;
										//  Bit4:	ut3;
										//  Bit5:	ut4;
										//  Bit6:	ut5;
										//  Bit7:	ut6;
									// byte 1
										//  Bit0:	dn;
										//  Bit1:	dt;
										//  Bit2:	dt1;
										//  Bit3:	dt2;
										//  Bit4:	dt3;
										//  Bit5:	dt4;
										//  Bit6:	dt5;
										//  Bit7:	dt6;
									// byte 2
										//  Bit0:	UPI;
										//  Bit1:	DNRI;
										//  Bit2:	NTSD;
										//  Bit3:	NTSD1;
 										//  Bit4:	unused;
										//  Bit5:	unused;
										//  Bit6:	unused;
										//  Bit7:	unused;
										
										
	switch (state)
	{
		case 0:
			state = 1;
			
		case 1:
			row = 28;
			col = 0;
						  
			i = 0;
			for (i=0;i<=1;i++)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->NTS_ios[0] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}
			
			state = 2;
			break;	

		case 2:
			row = 28;
			col = 2;
						  
			i = 0;
			for (i=0;i<=1;i++)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->NTS_ios[1] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}
			
			state = 3;
			break;	

		case 3:
			row = 28;
			col = 4;
						  
			for(j=0;j<=3;j++)
			{
				if ((ptr->NTS_ios[2] & bittbl[j]) != 0)
				    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
				else			 
				    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
			}
			
		//			unsigned char limit_flt[3];		// bit for each limit fault
												// Up Slowdown Inputs
												// byte 0
													//  Bit0: UN
													//  Bit1: 
													//  Bit2: UT
													//  Bit3: 
													//  Bit4: UTS
													//  Bit5: UT1
													//  Bit6: UT2
													//  Bit7: UT3
													
												// byte 1
													//  Bit0: UT4
													//  Bit1: UT5
													//  Bit2: UT6
													//  Bit3
													//  Bit4: DN
													//  Bit5: 
													//  Bit6: DT
													//  Bit7: 
													
												// byte 2
													//  Bit0: DTS
													//  Bit1: DT1
													//  Bit2: DT2
													//  Bit3: DT3
													//  Bit4: DT4
													//  Bit5: DT5
													//  Bit6: DT6
													//  Bit7
			
			
			if ((ptr->NTS_limit_flt[0] & 0x01) != 0)
				vratputf( nmlscn,28,46,WHITE,"1");
			else
				vratputf( nmlscn,28,46,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[0] & 0x04) != 0)
				vratputf( nmlscn,29,46,WHITE,"1");
			else
				vratputf( nmlscn,29,46,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[0] & 0x10) != 0)
				vratputf( nmlscn,30,46,WHITE,"1");
			else
				vratputf( nmlscn,30,46,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[0] & 0x20) != 0)
				vratputf( nmlscn,31,46,WHITE,"1");
			else
				vratputf( nmlscn,31,46,YELLOW,"0");
			
			state = 4;
			break;	

		case 4:

			if ((ptr->NTS_limit_flt[0] & 0x40) != 0)
				vratputf( nmlscn,32,46,WHITE,"1");
			else
				vratputf( nmlscn,32,46,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[0] & 0x80) != 0)
				vratputf( nmlscn,33,46,WHITE,"1");
			else
				vratputf( nmlscn,33,46,YELLOW,"0");
			
			
			if ((ptr->NTS_limit_flt[1] & 0x01) != 0)
				vratputf( nmlscn,28,54,WHITE,"1");
			else
				vratputf( nmlscn,28,54,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[1] & 0x02) != 0)
				vratputf( nmlscn,29,54,WHITE,"1");
			else
				vratputf( nmlscn,29,54,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[1] & 0x04) != 0)
				vratputf( nmlscn,30,54,WHITE,"1");
			else
				vratputf( nmlscn,30,54,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[1] & 0x10) != 0)
				vratputf( nmlscn,31,54,WHITE,"1");
			else
				vratputf( nmlscn,31,54,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[1] & 0x40) != 0)
				vratputf( nmlscn,32,54,WHITE,"1");
			else
				vratputf( nmlscn,32,54,YELLOW,"0");
			
			state = 5;
			break;	

		case 5:

			if ((ptr->NTS_limit_flt[2] & 0x01) != 0)
				vratputf( nmlscn,33,54,WHITE,"1");
			else
				vratputf( nmlscn,33,54,YELLOW,"0");
			
			
			if ((ptr->NTS_limit_flt[2] & 0x02) != 0)
				vratputf( nmlscn,28,62,WHITE,"1");
			else
				vratputf( nmlscn,28,62,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[2] & 0x04) != 0)
				vratputf( nmlscn,29,62,WHITE,"1");
			else
				vratputf( nmlscn,29,62,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[2] & 0x08) != 0)
				vratputf( nmlscn,30,62,WHITE,"1");
			else
				vratputf( nmlscn,30,62,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[2] & 0x10) != 0)
				vratputf( nmlscn,31,62,WHITE,"1");
			else
				vratputf( nmlscn,31,62,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[2] & 0x20) != 0)
				vratputf( nmlscn,28,70,WHITE,"1");
			else
				vratputf( nmlscn,28,70,YELLOW,"0");
			
			if ((ptr->NTS_limit_flt[2] & 0x40) != 0)
				vratputf( nmlscn,29,70,WHITE,"1");
			else
				vratputf( nmlscn,20,70,YELLOW,"0");
			
			
			state = 6;
			break;	

		case 6:
		
		//	Fault_Bits[0] = (uint8)( (Hoistway_Learned & 0x01) ||
		//									((hardware_init_fault & 0x01) << 1) ||
		//									((limit_dir_fault & 0x01) << 2) ||
		//									((spb_dir_fault & 0x01) << 3) ||
		//									((vel_decel_diff_fault & 0x01) << 4) ||
		//									((spb_vel_diff_fault & 0x01) << 5) ||
		//									((uldl_dir_fault & 0x01) << 6) ||
		//									((leveling_fault & 0x01) << 7) );

			row = 28;
			col = 8;
			k=2;			  
			i = 0;
			for (i=0;i<=1;i++)
			{
				for(j=k;j<=3;j++)
				{
					if ((ptr->fault_bits[0] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
				k = 0;
				row = 28;
			}
			
			state = 7;
			break;	

		case 7:
			if (ptr->ins_status != 0)
			{
				if ((ptr->ins_status & 0x40) != 0)		  // AUTO off
					vratputf( nmlscn,32,22,WHITE,"1");
				else
					vratputf( nmlscn,32,22,YELLOW,"0");
			}
			else
				vratputf( nmlscn,32,22,YELLOW,"0");
			
			if ((ptr->fault_bits[0] & 0x01) != 0)	   // HWLRN
				vratputf( nmlscn,32,30,YELLOW,"1");
			else
				vratputf( nmlscn,32,30,WHITE,"0");
			
			if ((ptr->fault_bits[0] & 0x02) != 0)	   // HWIF
				vratputf( nmlscn,32,38,WHITE,"1");
			else
				vratputf( nmlscn,32,38,YELLOW,"0");
			
			
		 	if (ptr->NTS_vel == 0) 
				attr = YELLOW;
			else 
				attr = WHITE;
			
			vratputf(nmlscn,32,9,attr,"%4i",(ptr->NTS_vel & 0xffff));
			
			if ((ptr->NTS_status & 0x01) != 0)
				vratputf(nmlscn,32,14,WHITE,"U");
			else if ((ptr->NTS_status & 0x02) != 0)
				vratputf(nmlscn,32,14,WHITE,"D");
			else
				vratputf(nmlscn,32,14,YELLOW," ");
			
			
		 	j = 0;

			if ((ptr->NTS_command & 0x0F) != 0)
			{
				if ((ptr->NTS_command & 0x01) != 0)
					j = 1;
				else if ((ptr->NTS_command & 0x02) != 0)
					j = 2;
				else if ((ptr->NTS_command & 0x04) != 0)
					j = 3;
				else if ((ptr->NTS_command & 0x08) != 0)
					j = 4;
				else if ((ptr->NTS_command & 0x0A) != 0x0A)
					j = 5;
				else if ((ptr->NTS_command & 0x0C) != 0x0C)
					j = 6;
				else if ((ptr->NTS_command & 0x0E) != 0x0E)
					j = 7;
			}

			if (j == 0) 
				attr = YELLOW;
			else 
				attr = WHITE;
			
			vratputf(nmlscn,33,10,attr,"%.7s",nts_command_dpy[j]);

		    if (ptr->NTS_service == 1) 
				attr = YELLOW;
			else 
				attr = WHITE;
			
			vratputf(nmlscn,33,24,attr,"%.6s",nts_service[ptr->NTS_service]);
			
										// byte 0
											//  Bit0: UN
											//  Bit1: 
											//  Bit2: UT
											//  Bit3: 
											//  Bit4: UTS
											//  Bit5: UT1
											//  Bit6: UT2
											//  Bit7: UT3
											
										// byte 1
											//  Bit0: UT4
											//  Bit1: UT5
											//  Bit2: UT6
											//  Bit3
											//  Bit4: DN
											//  Bit5: 
											//  Bit6: DT
											//  Bit7: 
											
										// byte 2
											//  Bit0: DTS
											//  Bit1: DT1
											//  Bit2: DT2
											//  Bit3: DT3
											//  Bit4: DT4
											//  Bit5: DT5
											//  Bit6: DT6
											//  Bit7
				
		 	if (((ptr->NTS_limit_flt[0] & 0xf5) != 0) || ((ptr->NTS_limit_flt[1] & 0x0f) != 0))
				j = 9;
			else if (((ptr->NTS_limit_flt[1] & 0x50) != 0) || ((ptr->NTS_limit_flt[2] & 0x7f) != 0))
				j = 8;
			else if ((ptr->NTS_status & 0x40) != 0)
				j = 7;						// emsd 
			else if ((ptr->NTS_status & 0x20) != 0)
				j = 6;						// vel fault
			else if ((ptr->NTS_status & 0x80) != 0)
				j = 5;						// vel dir fault
			else if ((ptr->NTS_status & 0x04) == 0)
				j = 4;						// sel comm fault
			else if ((ptr->NTS_status & 0x08) == 0)
				j = 3;						// spb comm fault
			else if ((ptr->NTS_status & 0x02) != 0)
				j = 2;						// running dn
			else if ((ptr->NTS_status & 0x01) != 0)
				j = 1;						// running up
			else 
				j = 0;						// run enable
						
			if (j == 0) 
				attr = YELLOW;
			else 
				attr = WHITE;
			vratputf(nmlscn,33,31,attr,"%.8s",trace_nts_lim_stat[j]);
			state = 0;
			break;

		default:
			state = 0;
			break;
	}
	
	return(state);

}


int16 Trace_Track_Screen_5(struct Fault *ptr, int16 state)
{
	int16 attr;
	int16 row,col;
	int16 i,j;
	
	switch (state)
	{
		case 0:
			state = 1;
			
		case 1:
			row = 28;
			col = 0;
						  
			i = 0;
			while (i<8)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->run_statusf & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			state = 2;
			break;

		case 2:
			row = 28;
			col = 2;
						  
			i = 8;
			while (i<16)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->run_statusf & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			state = 3;
			break;

		case 3:
			row = 28;
			col = 4;
						  
			i = 16;
			while (i<24)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->run_statusf & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			state = 4;
			break;

		case 4:
			row = 28;
			col = 6;
						  
			i = 24;
			while (i<32)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->run_statusf & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			state = 5;
			break;
			
		case 5:
		//	Fault_Bits[1] = (uint8)( (cop_can_error & 0x01) ||
		//									((spb_can_error & 0x01) << 1) ||
		//									((SEL_Count_Valid & 0x01) << 4) ||
		//									((sel_can_error & 0x01) << 5) ||
		//									((can_enc_com_err & 0x01) << 6) ||
		//									((can_enc_init & 0x01) << 7) );

			row = 28;
			col = 8;
						  
			i = 0;
			for (i=0;i<=1;i++)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->fault_bits[1] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}
			
			state = 6;
			break;
						
		case 6:
			
			if (ptr->ncu_door_seq == 0)
				attr = YELLOW;
			else
				attr = WHITE;
			
		    vratputf(nmlscn,32,7,attr,"%.8s",ncu_status[ptr->ncu_door_seq]);
			
			if (ptr->eqf == 0)
				attr = YELLOW;
			else
				attr = WHITE;
			
		    vratputf(nmlscn,33,0,attr,"%.15s",eqf_status[ptr->eqf]);
		    
			/*
							BIT0:  pulse_flt		
							BIT1:  etsu_flt		
							BIT2:  etsd_flt		
							BIT3:  pal_inh		
							BIT4:  rsb_en		// reduced stroke buffer enabled
							BIT5:  esld_test	 // ignore esld and/or write enable
							BIT6:  rear_door_en	
							BIT7:  enc_dir		
			*/
			if ((ptr->PAL_stat & 0x01) != 0)
				j = 0;			// PFT
			else if ((ptr->PAL_stat & 0x02) != 0)
				j = 1;			// UTF
			else if ((ptr->PAL_stat & 0x04) != 0)
				j = 2;			// DTF		
			else if ((ptr->PAL_stat & 0x08) != 0)
				j = 3;			// Fault
			else if ((ptr->PAL_stat & 0x20) != 0)
				j = 5;			// etsld test mode
			else if ((ptr->PAL_stat & 0x10) != 0)
				j = 4;			// reduced stroke buffer mode
			else if ((ptr->PAL_stat & 0x40) != 0)
				j = 6;			// rear door mode
//			else if ((ptr->PAL_stat & 0x80) != 0)
//				j = 7;			// encoder direction
			else 
				j = 8;
			
		    if (j >= 7) 
				attr = YELLOW;
			else 
				attr = WHITE;
			vratputf(nmlscn,32,26,attr,"%.3s",pal_status[j]);

		 	vratputf(nmlscn,33,25,YELLOW,"%4i",ptr->PAL_vel);


			if (ptr->PAL_vel != 0)
			{
				if ((ptr->PAL_stat & 0x80) != 0)
					vratputf(nmlscn,33,30,WHITE,"U");
				else
					vratputf(nmlscn,33,30,WHITE,"D");
			}
			else
				vratputf(nmlscn,33,30,YELLOW," ");


		 	// ins_status
		 	// bit 0: INS (car top inspection)
			// bit 1: MRIN (machine room inspection)
			// bit 2: ACC (access)
			// bit 3: ICI (in car inspection)
			// bit 4; LBP (lock bypass)
			// bit 5: GBP (gate bypass)
			// bit 6: AUTO (automatic)


			if ((ptr->ins_status & 0x10) != 0)		  // LBP
				vratputf( nmlscn,32,38,WHITE,"1");	  
			else
				vratputf( nmlscn,32,38,YELLOW,"0");
			
			if ((ptr->ins_status & 0x20) != 0)		  // GBP
				vratputf( nmlscn,33,38,WHITE,"1");
			else
				vratputf( nmlscn,33,38,YELLOW,"0");
			
			state = 7;
			break;
						
		case 7:
			
			if ((ptr->ins_status & 0x01) != 0)		  // ACC
				vratputf( nmlscn,32,46,WHITE,"1");
			else
				vratputf( nmlscn,32,46,YELLOW,"0");
			
			if ((ptr->ins_status & 0x02) != 0)		  // MRIN
				vratputf( nmlscn,33,46,WHITE,"1");
			else
				vratputf( nmlscn,33,46,YELLOW,"0");
			
			if ((ptr->ins_status & 0x04) != 0)		 // ACC
				vratputf( nmlscn,32,54,WHITE,"1");
			else
				vratputf( nmlscn,32,54,YELLOW,"0");
			
			if ((ptr->ins_status & 0x08) != 0)		 // ICI
				vratputf( nmlscn,33,54,WHITE,"1");
			else
				vratputf( nmlscn,33,54,YELLOW,"0");
			
			state = 0;
			break;
	
		default:
			state = 0;
			break;
	}
	
	return(state);

	
}

int16 Trace_Track_Screen_6(struct Fault *ptr, int16 state)
{
	int16 row,col;
	int16 i,j;

	switch (state)
	{
		case 0:
			state = 1;
			
		case 1:
			row = 28;
			col = 0;
						  
			i = 0;
			while (i<8)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->statusf2 & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			
			state = 2;
			break;
						
		case 2:
			row = 28;
			col = 2;
						  
			i = 8;
			while (i<16)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->statusf2 & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			
			state = 3;
			break;
						
		case 3:
			row = 28;
			col = 4;
						  
			i = 16;
			while (i<24)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->statusf2 & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			
			state = 4;
			break;
						
		case 4:
			row = 28;
			col = 6;
						  
			i = 24;
			while (i<32)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->statusf2 & l_bit_mask[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			
			state = 5;
			break;
						
		case 5:
			
		//	Fault_Bits[2] = (uint8)( (drive_online & 0x01) ||
		//									((DBR_Temp_Fault  & 0x01) << 1) ||
		//									((door_motor_ovl & 0x01) << 2) );

			row = 28;
			col = 8;
						  
			i = 0;
			for (i=0;i<=1;i++)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->fault_bits[2] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}
			
			state = 6;
			break;
						
		case 6:
			
		//		Door_Req_Flags = (uint8)(((rdoor_en & 0x01) << 7) | ((rcc_req & 0x01) << 6) | 
		//										((rdob_req & 0x01) << 5) | ((rdoor_req & 0x01) << 4) |
		//										((fdoor_en & 0x01) << 3) | ((fcc_req & 0x01) << 2) | 
		//										((fdob_req & 0x01) << 1) | (fdoor_req & 0x01));
												
			row = 32;
			col = 0;

			i = 0;
			for (i=0;i<=3;i++)
			{
				for(j=0;j<=1;j++)
				{
					if ((ptr->door_req_flags & bittbl[(i*2)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}
			
 			state = 7;
			break;
						
		case 7:
			

		//		Nudgst_Flags = (uint8)(((rnudgst & 0x0f) << 4) | (nudgst & 0x0f));
						  
			if ((ptr->nudgst_flags & 0x01) != 0)		  // Nudge with up pilot
				vratputf( nmlscn,32,38,WHITE,"1");	  
			else
				vratputf( nmlscn,32,38,YELLOW,"0");
			
			if ((ptr->nudgst_flags & 0x02) != 0)		  // Nudge with down pilot
				vratputf( nmlscn,33,38,WHITE,"1");
			else
				vratputf( nmlscn,33,38,YELLOW,"0");
			
			if ((ptr->nudgst_flags & 0x04) != 0)		  // Nudge with no pilot
				vratputf( nmlscn,32,46,WHITE,"1");
			else
				vratputf( nmlscn,32,46,YELLOW,"0");
			
			if ((ptr->nudgst_flags & 0x10) != 0)		  // Rear Nudge with up pilot
				vratputf( nmlscn,33,46,WHITE,"1");
			else
				vratputf( nmlscn,33,46,YELLOW,"0");
			
			if ((ptr->nudgst_flags & 0x20) != 0)		 // Rear Nudge with down pilot
				vratputf( nmlscn,32,54,WHITE,"1");
			else
				vratputf( nmlscn,32,54,YELLOW,"0");
			
			if ((ptr->nudgst_flags & 0x40) != 0)		 // Rear Nudge with no pilot
				vratputf( nmlscn,33,54,WHITE,"1");
			else
				vratputf( nmlscn,33,54,YELLOW,"0");
			
			state = 0;
			break;
	
		default:
			state = 0;
			break;
	}
	
	return(state);

}

int16 Trace_Track_Screen_7(struct Fault *ptr, int16 state)
{
	int16 row,col;
	uint8 t_door_seq_flags;
	int16 i,j;
	
	switch (state)
	{
		case 0:
			state = 1;
			
		case 1:
			row = 28;
			col = 0;
						  
			i = 0;
			while (i<8)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->ss_status & bittbl[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			state = 2;
			break;
						
			
		case 2:
			row = 28;
			col = 2;
						  
			i = 8;
			while (i<16)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->ss_status & bittbl[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			state = 3;
			break;
						
		case 3:
			row = 28;
			col = 4;
						  
			i = 0;
			while (i<8)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->power_status & bittbl[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			state = 4;
			break;
			
		case 4:
			row = 28;
			col = 6;
						  
			i = 8;
			while (i<16)
			{
				for(j=0;j<4;j++)
				{
					if ((ptr->power_status & bittbl[i]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
					i++;			
				}
				col++;
			}
			state = 5;
			break;
						
		case 5:
			
		//	Fault_Bits[3] = (uint8)( (gripper_fault & 0x01) ||
		//							((enc_dir_fault & 0x01) << 1) );						;

			row = 28;
			col = 8;
						  
			i = 0;
			for (i=0;i<=1;i++)
			{
				for(j=0;j<=3;j++)
				{
					if ((ptr->fault_bits[3] & bittbl[(i*4)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}

			state = 6;
			break;
						
		case 6:
			
		//		Call_Flags = (uint8)(((onward_hc & 0x03) << 6) | ((onward_cc & 0x03) << 4) | 
		//										 ((onward_calls & 0x03) << 2) | (dcalls & 0x03));
			row = 32;
			col = 0;
			i = 0;
			for (i=0;i<=3;i++)
			{
				for(j=0;j<=1;j++)
				{
					if ((ptr->call_flags & bittbl[(i*2)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}
			
			state = 7;
			break;
						
		case 7:
			
		//		Door_Seq_Flags = (uint8)(((rdo_seq & 0x07) << 5) | ((do_seq & 0x07) << 1) | (predof & 0x01));

			row = 32;
			col = 4;
			t_door_seq_flags = ptr->door_seqf >> 1;
			i = 0;
			for (i=0;i<=2;i++)
			{
				for(j=0;j<=1;j++)
				{
					if ((t_door_seq_flags & bittbl[(i*2)+j]) != 0)
					    vratputf( nmlscn, row+j,((col*8)+6),WHITE,"1");
					else			 
					    vratputf( nmlscn, row+j,((col*8)+6),YELLOW,"0");
				}
				col++;
			}

			state = 0;
			break;
	
		default:
			state = 0;
			break;
	}
	
	return(state);

}

//*******************************************************************
//*** This is the Group Serial Hall Call status initialization    ***
//*******************************************************************
void Init_Grp_Io_Screen(int16 init_flag)
{
    int16 j,found,k,color;
	static int16 ionmb,bitmsk;
	static int16 state;

	if (init_flag == 1)
	{		  // Reset state machine
		io_grp_screen_init = 0;
		state = 0;
		return;
	}

	switch (state)
	{

		case 0:
			wnum = 0;
			state = 1;
		    ScreenClear();
			break;
		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:	 
			set_font(font_8x14);	// set 8x14 to get 28 rows
			max_row = 24;
			max_car_row = 21;
			max_window_row = 28;
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			nmlscn = vcreat( max_window_row, 80, WHITE);
			vlocate( nmlscn, 1, 1);
			state = 5;
			break;
		case 5:
			vratputf(nmlscn,0,0,YELLOW,"                        Group Hall Call Screen",Car_Select);
			row = 1;
			col = 0;
			if (nmbhcb < 10)
				board_nmb = 0;
			else if (grp_page == 2)
				board_nmb = next_page_hcb_2;
#if (Tract_HR == 1)
			else if (grp_page == 3)
				board_nmb = next_page_hcb_3;
			else if (grp_page == 4)
				board_nmb = next_page_hcb_4;
			else if (grp_page == 5)
				board_nmb = next_page_hcb_5;
#endif
			else board_nmb = 0;
			found = 0;
			while ((board_nmb < nmb_hcio_brds) && (found == 0))
			{				  // Get the next io board being used
				if (HC_IO_board[board_nmb] != 0)
					found = 1;
				else
					board_nmb++;
			}
			ionmb = board_nmb * 24;
			state = 6;
			break;
		case 6:			// Display front hall calls
			if ((row == 1) && (get_comm_status(board_nmb) != 2))
			{
				if ((HC_IO_board[board_nmb] & 0x02) != 0)
				{			// Serial Hall call driver boards
					if (((board_nmb%6) == 0) && ((cons[sercom] & 0x20) != 0))	// using a driver board
					{
						vratputf(nmlscn, 1,col*8,YELLOW,"HCDB#   ");
						vratputf(nmlscn, 2,col*8,YELLOW," SEL:AD ");
						vratputf(nmlscn, 3,col*8,YELLOW," Comm= ");
					}
				}
				else
				{
					vratputf(nmlscn, 1,col*8,YELLOW," HC#   ");
					vratputf(nmlscn, 3,col*8,YELLOW," Comm= ");
				}
			}

#if (Quick_Update == 1)
			for(j=1; j<=4; j++)
#else
			for(j=1; j<=2; j++)
#endif
			{		  // Do one board at a time on each display column

				//display for inp and out
				k = ionmb/8;
				bitmsk = bittbl[(row-1) & 0x07];
				if ((GIO_In_Out[k] & bitmsk) == 0)
					color = WHITE;
				else
					color = LTCYAN;
				
				vratputf( nmlscn, row+3,(col*8),color,"%.5s ",hcioname[ionmb]);
				vratputf( nmlscn, row+3,((col*8)+7),WHITE,"%c",Vertical_line);
				if ((gio_valid[ionmb/8] & bittbl[ionmb & 0x7]) != 0)
					vratputf( nmlscn, row+3,((col*8)+5),color,"=");
					
				row++;
				ionmb++;
			}

			if (row > max_row)
			{
				if (col < 10) 
				{
					found = 0;
					board_nmb++;
					while ((board_nmb < nmb_hcio_brds) && (found == 0))
					{				  // Get the next io board being used
						if (HC_IO_board[board_nmb] != 0)
							found = 1;
						else
							board_nmb++;
					}
					if (found == 0)
						state = 7;
					else
					{
						ionmb = (board_nmb * 24);
						row = 1;
						col++;
					}
				}
				else
					state = 7;		// exit
			}
			break;
		case 7:			   // Turn on display and set init flag
			col = 13;
			row = 26;
			io_grp_screen_init = 1;
			Grp_Io_Screen(Reset_State);		// Reset state flag
			state = 0;
			break;
		default:
			break;
	}

}

//**************************************************
//*** This is the Group Serial Hall Call status  ***
//**************************************************
void Grp_Io_Screen(int16 init_flag)
{
	int16 j,k,on_color,off_color,hcdb_nmb;
	int16 comm_status=0; 
	static int16 ionmb,bitmsk;
	int16 found, io_status;
	static int16 state;
#if (Display_hc_io_add == 1) 
	static char tempstring[3];
#endif

	if (init_flag == 1)
	{	
		state = 0;
		return;
	}
	
	switch (state)
	{
		case 0:
			state = 1;
			row = 1;
			col = 0;
			if (nmbhcb < 10)
				board_nmb = 0;
			else if (grp_page == 2)
				board_nmb = next_page_hcb_2;
#if (Tract_HR == 1)
			else if (grp_page == 3)
				board_nmb = next_page_hcb_3;
			else if (grp_page == 4)
				board_nmb = next_page_hcb_4;
#endif
			else board_nmb = 0;
			found = 0;
			while ((board_nmb < nmb_hcio_brds) && (found == 0))
			{				  // Get the next io board being used
				if (HC_IO_board[board_nmb] != 0)
					found = 1;
				else
					board_nmb++;
			}
			ionmb = board_nmb * 24;
			break;

		case 1:
#if (Quick_Update == 1)
			for(j=1; j<=8; j++)  // DISPLAY INPUTS
#else
			for(j=1; j<=4; j++)  // DISPLAY INPUTS
#endif
			{
			
				io_status = get_io_status(board_nmb,row);

				comm_status = get_comm_status(board_nmb);

				if (j == 1)
				{
#if (Display_hc_io_add == 1) 

					
					if ((cons[sercom] & 0x20) != 0)
					{
						if (((board_nmb%6) == 0) && (board_nmb != 0))
							hcdb_add_offset = board_nmb/6;
						else
							hcdb_add_offset = 0;
						
						hcdb_nmb = (board_nmb+1)-(5*hcdb_add_offset);
						if (hcdb_nmb == 13)
						{
							debug2 = board_nmb;
							debug3 = hcdb_add_offset;
						}
						if ((HC_IO_board[board_nmb] & 0x02) != 0)
							sprintf(tempstring,"#%d",hcdb_nmb);
						else
							sprintf(tempstring," %d",hcdb_nmb+10);
					}
					else if ((cons[sercom] & 0x20) == 0)
					{
						if (board_nmb+1 < 31)
						{
							sprintf(tempstring," %d",board_nmb+1);
						}
						else if (board_nmb+1 < 45)
						{
							sprintf(tempstring,"  %d",board_nmb+1-26);
							vratputf(nmlscn, 2,col*8,YELLOW," SEL: B");
						}
						else
						{
							sprintf(tempstring,"  %d",board_nmb+1-26-18);
							vratputf(nmlscn, 2,col*8,YELLOW," SEL: C");
						}
					}
					
					if((comm_status == 0)||(comm_status == 1))
						vratputf( nmlscn, 1,((col*8)+4),YELLOW,tempstring);					
					else
					    vratputf( nmlscn, 3,((col*8)+6),WHITE," ");
#endif

					if(comm_status == 0)
					    vratputf( nmlscn, 3,((col*8)+6),YELLOW,"0");
					else if (comm_status == 1)
					    vratputf( nmlscn, 3,((col*8)+6),GREEN,"1");
					else
					    vratputf( nmlscn, 3,((col*8)+6),WHITE," ");
				}				

				if ((gio_valid[ionmb/8] & bittbl[ionmb & 0x7]) != 0) 
				{
					k = ionmb/8;
					bitmsk = bittbl[(row-1) & 0x07];
					if ((GIO_In_Out[k] & bitmsk) == 0)
					{
						on_color = GREEN;		// input
						off_color = WHITE;
					}
					else
					{
						on_color = RED;			// output
						off_color = LTCYAN;	
					}
				
					if(io_status != 0)
					    vratputf( nmlscn, row+3,((col*8)+6),on_color,"1");
					else
					    vratputf( nmlscn, row+3,((col*8)+6),off_color,"0");
				}
				row++;
				ionmb++;
			}

			if (row > max_row)
			{
				if (col < 10)
				{
					found = 0;
					board_nmb++;
					while ((board_nmb < nmb_hcio_brds) && (found == 0))
					{				  // Get the next io board being used
						if (HC_IO_board[board_nmb] != 0)
							found = 1;
						else
							board_nmb++;
					}
					if (found == 0)
						state = 0;
					else
					{
						row = 1;
						col++;
						ionmb = board_nmb * 24;
					}
				}
				else
					state = 0;
			}
			break;
		default:
			break;
	}
	
	return;
}


//****************************************************
//**     This function initialize the monitor       **
//****************************************************
void Init_Group_Hall_Display (int16 init_flag)
{
	if (cons[grtopf] > 44)
		Init_Group_Hall_Display_60(init_flag);
	else
		Init_Group_Hall_Display_44(init_flag);
}
//*************************************************************************
//**     This function initialize the monitor for 44 floors or less      **
//*************************************************************************
void Init_Group_Hall_Display_44 (int16 init_flag)
{						// Initialization done in steps
	int16 i,j;
	int16 clred=RED;
	int16 space,left_over;
	static int16 fl, fl_ix,fl_row;
	static int16 state;

	if (init_flag == 1)
	{
		state = 0;
		group_dpy_init = 0;
		return;
	}
	
	switch (state)
	{
		case 0:
			for(i = 1; i <= cons[nmbcars]; i++)
			{
				if (i <= 6)
					vidpos[i] = fvars[fvvidc+i];
				else
					vidpos[i] = fvars[fvvidc+82+i];
				if ((vidpos[i] < 1) || (vidpos[i] > cons[nmbcars]))
					vidpos[i] = i;

				space = (51 - (3 * cons[nmbcars]))/(cons[nmbcars] + 1);
				left_over = (51 - ((space * (cons[nmbcars] + 1) + (3 * cons[nmbcars]))));

				Vid_Car_Xpos[i] = (space * vidpos[i]) + (3 * (vidpos[i] - 1)) + (left_over/2);
				for (j = 1; j<= cons[nmbcars]; j++)
				{	 // index through each video column for ecah car
					if (vidpos[i] == j)	// find the video position of car i
						vidcar[j] = i;		// store the car number in vidcar indexed by the video location
				}
			}

			state = 1;
			wnum = 0;
		    ScreenClear();
			break;
		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:
		 	if (cons[grtopf] > 23)
			{
				max_car_row = 45;
				max_window_row = 50;
				max_row = 48;
				set_font(font_8x8);		// set font size to 8x8 to get 50 rows
			}
			else
			{
				max_car_row = 24;		// from 21
				max_row = 27;			// from 24
				max_window_row = 29;	// from 26
				set_font(font_8x14);	// set to get 28 rows
			}
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			gwindow = vcreat( max_window_row, 80, WHITE);
			vlocate( gwindow, 0, 0);
			state = 5;
			break;
		case 5:		
			upbox = vcreat( max_car_row, 5, WHITE);
			vlocate( upbox, 3, 4);
			state = 6;
			break;
		case 6:
			dnbox = vcreat( max_car_row, 5, WHITE);
			vlocate( dnbox, 3, 72);
			state = 7;
			break;
		case 7:
			carbox = vcreat( max_car_row, 51, WHITE);
			vlocate( carbox, 3, c_car_offset);
			state = 8;
			break;
		case 8:
			arrow1 = vcreat( 1, 3, WHITE);
			vlocate( arrow1, max_row, (Vid_Car_Xpos[1] + c_car_offset));
			state = 9;
			break;
		case 9:
			arrow2 = vcreat( 1, 3, WHITE);
			vlocate( arrow2, max_row, (Vid_Car_Xpos[2] + c_car_offset));
			state = 10;
			break;
		case 10:
			arrow3 = vcreat( 1, 3, WHITE);
			vlocate( arrow3, max_row, (Vid_Car_Xpos[3] + c_car_offset));
			state = 11;
			break;
		case 11:
			arrow4 = vcreat( 1, 3, WHITE);
			vlocate( arrow4, max_row, (Vid_Car_Xpos[4] + c_car_offset));
			state = 12;
			break;
		case 12:
			arrow5 = vcreat( 1, 3, WHITE);
			vlocate( arrow5, max_row, (Vid_Car_Xpos[5] + c_car_offset));
			state = 13;
			break;
		case 13:
			arrow6 = vcreat( 1, 3, WHITE);
			vlocate( arrow6, max_row, (Vid_Car_Xpos[6] + c_car_offset));
			state = 14;
			break;
		case 14:
			arrow7 = vcreat( 1, 3, WHITE);
			vlocate( arrow7, max_row, (Vid_Car_Xpos[7] + c_car_offset));
			state = 15;
			break;
		case 15:
			arrow8 = vcreat( 1, 3, WHITE);
			vlocate( arrow8, max_row, (Vid_Car_Xpos[8] + c_car_offset));
			state = 16;
			break;
		case 16:
			vframe( upbox, WHITE, FRSINGLE );
			state = 17;
			break;
		case 17:
			vframe( dnbox, WHITE, FRSINGLE );
			state = 18;
			break;
		case 18:
			vframe( carbox, WHITE, FRSINGLE );
			state = 19;
			break;
		case 19:
			vratputf( gwindow, 0, 5, GREEN, "UP");
			for (i=1;i<=cons[nmbcars];i++)
			{
				vratputf( gwindow, 0, (Vid_Car_Xpos[i] + c_car_offset),WHITE, "CAR");
			}
			vratputf( gwindow, 0, 73, clred, "DN");
			vratputf( gwindow, 1, 4, GREEN, "CALLS");
			state = 20;
			break;
		case 20:
			for(i=1;i<=cons[nmbcars];i++)
			{
				vratputf( gwindow, 1, (Vid_Car_Xpos[i] + c_car_offset),WHITE,"#%.2s",car_bld_no[i]);
			}
			vratputf( gwindow, 1, 72, clred, "CALLS");
			state = 21;
			if(cons[grtopf] >= 11)
			{
				fl_row = max_row-cons[grtopf];
				fl_ix = 1;
			}
			else
			{
				fl_row = max_row-(cons[grtopf]*2);
				fl_ix = 2;
			}
			fl = cons[grtopf];
			break;
		case 21:
			if (fl >= 1)
			{
				vratputf( gwindow, fl_row, 11,WHITE, "%.2s",flmrk[fl]);
				vratputf( gwindow, fl_row, 68,WHITE, "%.2s",flmrk[fl]);
			}
			if (fl_row < max_row)
			{
				fl_row = fl_row + fl_ix;
				fl--;
			}
			else
				state = 22;
			break;
		case 22: // Force a screen update
			vid_test = 0;
			Video_PI(Car_Select,Reset_State);		// Set init flags
			group_dpy_init = 1;
			state = 0;
			break;
		default:
			break;

	}
}

//*************************************************************************
//**     This function initialize the monitor for 60 floors or less      **
//*************************************************************************
void Init_Group_Hall_Display_60 (int16 init_flag)
{						// Initialization done in steps
	int16 i,j;
	int16 space,left_over;
	static int16 fl,fl_row;
	static int16 fl_next_col;
	static int16 fl_row_next_col;
	static int16 state;

	if (init_flag == 1)
	{
		state = 0;
		group_dpy_init = 0;
		return;
	}
	

	switch (state)
	{
		case 0:
			for(i = 1; i <= cons[nmbcars]; i++)
			{
				if (i <= 6)
					vidpos[i] = fvars[fvvidc+i];
				else
					vidpos[i] = fvars[fvvidc+82+i];
				if ((vidpos[i] < 1) || (vidpos[i] > cons[nmbcars]))
					vidpos[i] = i;

				space = (80 - (5 * cons[nmbcars]))/(cons[nmbcars] + 1);
				left_over = (80 - ((space * (cons[nmbcars] + 1) + (5 * cons[nmbcars]))));

				Vid_Car_Xpos[i] = (space * vidpos[i]) + (5 * (vidpos[i] - 1)) + (left_over/2);
				for (j = 1; j<= cons[nmbcars]; j++)
				{	 // index through each video column for ecah car
					if (vidpos[i] == j)	// find the video position of car i
						vidcar[j] = i;		// store the car number in vidcar indexed by the video location
				}
			}

			state = 1;
			wnum = 0;
		    ScreenClear();
			break;
		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:
			max_car_row = 45;
			max_window_row = 50;
			max_row = 30;
			set_font(font_8x8);		// set font size to 8x8 to get 50 rows
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			gwindow = vcreat( max_window_row, 80, WHITE);
			vlocate( gwindow, 0, 0);
			state = 5;
			break;
		case 5:
			for (i=1;i<=cons[nmbcars];i++)
			{
				vratputf( gwindow, 0, (Vid_Car_Xpos[i] + c_car_offset_60),WHITE,"CAR #%.2s",car_bld_no[i]);
			}
			state = 6;
			break;
		case 6:
			for (i=1;i<=cons[nmbcars];i++)
			{
				vratputf( gwindow, 16, (4 * (i-1)) + 9,WHITE,"%.2s",car_bld_no[i]);
				vratputf( gwindow, 16 + 30 - (cons[grtopf] - 30), (4 * (i-1)) + 49,WHITE,"%.2s",car_bld_no[i]);
			}
			state = 7;
			fl_row = 1;
			fl = cons[grtopf];
			fl_next_col = cons[grtopf] - 30;
			fl_row_next_col = 2 * fl_next_col;
			break;
		case 7:
			ScreenPutChar((uint8)c_single_col, WHITE, 4, fl_row + 17);
			vratputf( gwindow, fl_row + 17, 5,WHITE, "%.2s",flmrk[fl]);
			if (fl <= fl_row_next_col)
			{
				ScreenPutChar((uint8)c_single_col, DKGREY, 41, fl_row + 17);
				ScreenPutChar((uint8)c_single_col, WHITE, 44, fl_row + 17);
				vratputf( gwindow, fl_row + 17, 45,WHITE, "%.2s",flmrk[fl - fl_next_col]);
			}
			if ((fl_row < max_row) && (fl > 1))
			{
				fl_row ++;
				fl--;
			}
			else
				state = 8;
			break;
		case 8:
			ScreenPutChar((uint8)c_single_col, DKGREY, 4, 48);
			ScreenPutChar((uint8)c_bottom_left, DKGREY, 4, 49);
			for (i=5;i<=40;i++)
				ScreenPutChar((uint8)c_single_line, DKGREY, i, 49);
			ScreenPutChar((uint8)c_bottom_right, DKGREY, 41, 49);
			ScreenPutChar((uint8)c_single_col, DKGREY, 41, 48);
			ScreenPutChar((uint8)c_top_left, DKGREY, 41, 47 - fl_next_col);
			ScreenPutChar((uint8)c_single_line, DKGREY, 42, 47 - fl_next_col);
			ScreenPutChar((uint8)c_single_line, DKGREY, 43, 47 - fl_next_col);
			ScreenPutChar((uint8)c_top_right, DKGREY, 44, 47 - fl_next_col);
			state = 9;
			break;
		case 9: // Force a screen update
			vid_test = 0;
			Video_PI_60(1,1);
			group_dpy_init = 1;
			state = 0;
			break;
		default:
			break;

	}
}

//****************************************************
//*** This handles group hall calls on the monitor ***
//****************************************************
void Group_Hall_Display(int16 init_flag)
{
	if (cons[grtopf] > 44)
		Group_Hall_Display_60(init_flag);
	else
		Group_Hall_Display_44(init_flag);
}
//******************************************************************
//*** This handles group hall calls for 44 floors on the monitor ***
//******************************************************************
void Group_Hall_Display_44(int16 init_flag)
{
	int16 pos,assign_car;
	int16 atr = 0;
	static int16 ghd_car_no,fl;
    int16 j,yinc;
	static int16 state;
	int16 fl_ix;

	if (init_flag == 1)
	{
		state = 0;
		return;
	}

    if(cons[grtopf] >= 11)
		yinc = 1;
    else
		yinc = 2;

	switch (state)
	{

		case 0: 
			fl = 1;
			state = 1;
//			break;		no break here so there's no delay to start next state

		case 1:				   // Display up Hall Calls
			j = 0;
#if (Quick_Update == 1)
			while ((fl<cons[grtopf]) && (j<10))
#else
			while ((fl<cons[grtopf]) && (j<3))
#endif
		    {		// do three floors at a time
				if( (g_asign_up[fl] != 0) || (g_asign_upr[fl] != 0) || 
						((cons[LbyUpReq] == 1) && (fl == lobby_fl) && (lby_up_assign >= 1) && (lby_up_assign <= cons[nmbcars])) )
				{
					if (g_asign_up[fl] != 0)
						assign_car = g_asign_up[fl];
					else if (g_asign_upr[fl] != 0)
						assign_car = g_asign_upr[fl];
					else if ((cons[LbyUpReq] == 1) && (fl == lobby_fl) && (lby_up_assign >= 1) && (lby_up_assign <= cons[nmbcars]))
						assign_car = lby_up_assign;
					else
						assign_car = g_asign_up[fl];
				
				    vratputf( upbox, (max_car_row-(fl*yinc)), 0, GREEN, "%3i",car_up_eta[assign_car][fl]);
				}
				else
				    vratputf( upbox, (max_car_row-(fl*yinc)), 0, GREEN, "   ");
				fl++;
				j++;
		    }
			if (fl >= cons[grtopf])
			{
				fl = 2;
				state = 2;
			}
			break;

		case 2:				   // Display Down Hall Calls
			j = 0;
#if (Quick_Update == 1)
			while ((fl<=cons[grtopf]) && (j<10))
#else
			while ((fl<=cons[grtopf]) && (j<3))
#endif
		    {		// do three floors at a time
				if( (g_asign_dn[fl] != 0) || (g_asign_dnr[fl] != 0) )
				{
					if (g_asign_dn[fl] != 0)
						assign_car = g_asign_dn[fl];
					else
						assign_car = g_asign_dnr[fl];
				    vratputf( dnbox, (max_car_row-(fl*yinc)), 1, RED, "%3i",car_dn_eta[assign_car][fl]);
				}
				else
				    vratputf( dnbox, (max_car_row-(fl*yinc)), 1, RED, "   ");
				fl++;
				j++;
		    }
			if (fl > cons[grtopf])
			{
				state = 3;
				ghd_car_no = 1;
			}
		break;

		case 3:					 // Display car 
		    Video_PI(ghd_car_no,Run_State);
			if (ghd_car_no < cons[nmbcars])
				ghd_car_no++;
			else
			{
				fl = 1;
				ghd_car_no = 1;
				state = 4;
			}
		break;

		case 4:						  // Display Up Assignments

			pos = grp_Car_Stat[ghd_car_no].g_pos;
			if ((pos < 1) || (pos > cons[grtopf]))
				pos = 1;
			j = 0;
#if (Quick_Update == 1)
			while ((fl<cons[grtopf]) && (j<10))
#else
			while ((fl<cons[grtopf]) && (j<3))
#endif
		    {		// do three floors at a time
			    if(pos != fl)
			    {
					if ((g_asign_up[fl] == ghd_car_no) || (g_asign_irup[fl] == ghd_car_no) || (g_asign_trup[fl] == ghd_car_no) ||
						((cons[LbyUpReq] == 1) && (fl == lobby_fl) && (lby_up_assign == ghd_car_no)))
					{
						atr = GREEN;
						if ((((g_asign_up[fl] == g_asign_irup[fl]) || (lby_up_assign == g_asign_irup[fl])) && (g_asign_irup[fl] == ghd_car_no)) || 
							 (((g_asign_up[fl] == g_asign_trup[fl]) || (lby_up_assign == g_asign_trup[fl])) && (g_asign_trup[fl] == ghd_car_no)))		// AS check lby_up_assign
							atr = LTGREEN;
						else if (g_asign_irup[fl] == ghd_car_no)
					    	atr = LTCYAN;
						else if (g_asign_trup[fl] == ghd_car_no)
							atr = YELLOW;
					    vratputf(carbox, (max_car_row-(fl*yinc)),Vid_Car_Xpos[ghd_car_no], atr, "%c",Up_Arrow);
					}
					else
					{
					    vratputf(carbox, (max_car_row-(fl*yinc)),Vid_Car_Xpos[ghd_car_no], WHITE, " ");
					}
			    }
				if ((g_asign_upr[fl] == ghd_car_no) || (g_asign_rirup[fl] == ghd_car_no))
			    {
					atr = GREEN;
					if (g_asign_upr[fl] == g_asign_rirup[fl])
						atr = LTGREEN;
					else if (g_asign_rirup[fl] == ghd_car_no)
						atr = LTCYAN;
					vratputf(carbox, (max_car_row-(fl*yinc)),(Vid_Car_Xpos[ghd_car_no]-1), GREEN, "%c",Up_Arrow);
			    }
			    else
			    {
					vratputf(carbox, (max_car_row-(fl*yinc)),(Vid_Car_Xpos[ghd_car_no]-1), WHITE, " ");
			    }
				fl++;
				j++;
			}
		
			if (fl >= cons[grtopf])
			{		// finished floors for one car
				if (ghd_car_no < cons[nmbcars])
				{		  // do the next car
					fl = 1;
					ghd_car_no++;
				}
				else
				{
					fl = 2;
					ghd_car_no = 1;
					state = 5;
				}
			}

		break;

		case 5:						   // Display Down Assignments
			pos = grp_Car_Stat[ghd_car_no].g_pos;
			if ((pos < 1) || (pos > cons[grtopf]))
				pos = 1;
			j = 0;
#if (Quick_Update == 1)
			while ((fl<=cons[grtopf]) && (j<10))
#else
			while ((fl<=cons[grtopf]) && (j<3))
#endif
		    {		// do three floors at a time
			    if(pos != fl)
			    {
					if ((g_asign_dn[fl] == ghd_car_no) || (g_asign_irdn[fl] == ghd_car_no) || (g_asign_trdn[fl] == ghd_car_no))
					{
						atr = RED;
						if ((g_asign_dn[fl] != 0) && ((g_asign_dn[fl] == g_asign_irdn[fl]) || (g_asign_dn[fl] == g_asign_trdn[fl])))
							atr = BROWN;
						else if (g_asign_irdn[fl] == ghd_car_no)
							atr = LTRED;
					    else if (g_asign_trdn[fl] == ghd_car_no)
							atr = LTMAGENTA;
					    vratputf(carbox, (max_car_row-(fl*yinc)),(Vid_Car_Xpos[ghd_car_no]+2), atr, "%c",Dn_Arrow);
					}
					else
					{
					    vratputf(carbox, (max_car_row-(fl*yinc)),(Vid_Car_Xpos[ghd_car_no]+2), WHITE, " ");
					}
			    }
				if ((g_asign_dnr[fl] == ghd_car_no) || (g_asign_rirdn[fl] == ghd_car_no))
			    {
					atr = RED;
					if (g_asign_dnr[fl] == g_asign_rirdn[fl])
						atr = BROWN;
					else if (g_asign_rirdn[fl] == ghd_car_no)
						atr = LTRED;
					vratputf(carbox, (max_car_row-(fl*yinc)),(Vid_Car_Xpos[ghd_car_no]+3), atr, "%c",Dn_Arrow);
			    }
			    else
			    {
					vratputf(carbox, (max_car_row-(fl*yinc)),(Vid_Car_Xpos[ghd_car_no]+3), WHITE, " ");
			    }
				fl++;
				j++;
			}
			
			if (fl > cons[grtopf])
			{		// finished floors for one car
				if (ghd_car_no < cons[nmbcars])
				{
					fl = 1;
					ghd_car_no++;
				}
				else
				{
					fl = 1;
					ghd_car_no = 1;
					state = 6;
				}
			}
			break;

		case 6:					 // Display car 
		    Video_PI(ghd_car_no,Run_State);
			if (ghd_car_no < cons[nmbcars])
				ghd_car_no++;
			else
			{
				fl = 1;
				ghd_car_no = 1;
				state = 7;
			}
		break;


		case 7:						 // Display Car Calls

			pos = grp_Car_Stat[ghd_car_no].g_pos;
			if ((pos < 1) || (pos > cons[grtopf]))
				pos = 1;

			j = 0;
#if (Quick_Update == 1)
			while ((fl<=cons[grtopf]) && (j<10))
#else
			while ((fl<=cons[grtopf]) && (j<3))
#endif
		    {		// do three floors at a time
				fl_ix = (fl-1)/32;				 
				if(pos != fl)
				{
				    if ( ( ((grp_Car_Stat[ghd_car_no].g_cc[fl_ix] & fl_mask[fl]) != 0) ||
						(((grp_Car_Stat[ghd_car_no].g_rcc[fl_ix] & fl_mask[fl]) != 0) && (rear_op[ghd_car_no] !=0)) )	&&
						 (gc_dev_online[ghd_car_no] == 1) && (Car_Not_Talking[ghd_car_no] == 0) )
						vratputf(carbox, (max_car_row-(fl*yinc)),(Vid_Car_Xpos[ghd_car_no]+1), WHITE, "%c",Car_Call);
				    else
						vratputf(carbox, (max_car_row-(fl*yinc)),(Vid_Car_Xpos[ghd_car_no]+1), WHITE, " ");
				}
				fl++;
				j++;
			}

			if (fl > cons[grtopf])
			{		// finished floors for one car
				if (ghd_car_no < cons[nmbcars])
				{
					fl = 1;
					ghd_car_no++;
				}
				else
				{
					fl = 1;
					ghd_car_no = 1;
					state = 8;
				}
			}
		break;

		case 8:
			if (gservf == 0)
			{
				if ((dn_peak == 1) && (up_peak == 0))
					vratputf( gwindow, 0, 11, LTCYAN, "DnPk ");
				else if ((up_peak == 1) && (dn_peak == 0))
					vratputf( gwindow, 0, 11, LTCYAN, "UpPk ");
				else if ((up_peak == 1) && (dn_peak == 1))
					vratputf( gwindow, 0, 11, LTCYAN, "HEAVY");					
				else
					vratputf( gwindow, 0, 11, LTCYAN, "NORM ");
			}
			else if (gservf == 1)
				vratputf( gwindow, 0, 11, MAGENTA, "FIRE ");
			else if (gservf == 2)
				vratputf( gwindow, 0, 11, WHITE, "EMPWR");
			else if (gservf == 3)
				vratputf( gwindow, 0, 11, MAGENTA, "FI/EP");
					
			state = 0;
			break;
		default:
			break;

	}

}
//******************************************************************
//*** This handles group hall calls for 60 floors on the monitor ***
//******************************************************************
void Group_Hall_Display_60(int16 init_flag)
{
	int16 pos;
	static int16 ghd_car_no,fl;
    int16 j;
	static int16 state;
	int16 fl_ix;

	if (init_flag == 1)
	{
		state = 0;
		return;
	}

	switch (state)
	{

		case 0: 
			fl = 1;
			state = 1;
//			break;		no break here so there's no delay to start next state


		case 1:				   // Display Down Hall Calls
			j = 0;
#if (Quick_Update == 1)
			while ((fl<=cons[grtopf]) && (j<10))
#else
			while ((fl<=cons[grtopf]) && (j<3))
#endif
		    {		// do three floors at a time
				if (g_asign_up[fl] != 0)
				{
					if (g_asign_dn[fl] != 0)
						place_hall_call(fl, 3);
					else
						place_hall_call(fl, 1);
				}
				else if (g_asign_dn[fl] != 0)
					place_hall_call(fl, 2);
				else
					clear_hall_call(fl,0);
				if (g_asign_upr[fl] != 0)
				{
					if (g_asign_dnr[fl] != 0)
						place_hall_call(fl, 12);
					else
						place_hall_call(fl, 4);
				}
				else if (g_asign_dnr[fl] != 0)
					place_hall_call(fl, 8);
				else
					clear_hall_call(fl,1);
				fl++;
				j++;
		    }
			if (fl > cons[grtopf])
			{
				state = 2;
				ghd_car_no = 1;
			}
		break;

		case 2:					 // Display car 
			Video_PI_60(ghd_car_no,0);
			if (ghd_car_no < cons[nmbcars])
				ghd_car_no++;
			else
			{
				fl = 1;
				ghd_car_no = 1;
				state = 3;
			}
		break;

		case 3:						  // Display Up and Down Assignments

			j = 0;
#if (Quick_Update == 1)
			while ((fl<=cons[grtopf]) && (j<10))
#else
			while ((fl<=cons[grtopf]) && (j<3))
#endif
		    {		// do three floors at a time
				if ((g_asign_up[fl] == ghd_car_no) || ((cons[LbyUpReq] == 1) && (fl == lobby_fl) && (lby_up_assign == ghd_car_no)))
				{
					if(g_asign_dn[fl] == ghd_car_no)
						place_assign_call(fl,ghd_car_no, 3);
					else
						place_assign_call(fl,ghd_car_no, 1);
				}
				else if (g_asign_dn[fl] == ghd_car_no)
					place_assign_call(fl,ghd_car_no, 2);
				else
					clear_assign_call(fl,ghd_car_no,0);

				if(g_asign_upr[fl] == ghd_car_no)
			    {
					if(g_asign_dnr[fl] == ghd_car_no)
						place_assign_call(fl,ghd_car_no, 12);
					else
						place_assign_call(fl,ghd_car_no, 4);
			    }
				else if(g_asign_dnr[fl] == ghd_car_no)
					place_assign_call(fl,ghd_car_no, 8);
				else
					clear_assign_call(fl,ghd_car_no, 1);
				fl++;
				j++;
			}
		
			if (fl >= cons[grtopf])
			{		// finished floors for one car
				if (ghd_car_no < cons[nmbcars])
				{		  // do the next car
					fl = 1;
					ghd_car_no++;
				}
				else
				{
					fl = 2;
					ghd_car_no = 1;
					state = 4;
				}
			}

		break;

		case 4:					 // Display car 
			Video_PI_60(ghd_car_no,0);
			if (ghd_car_no < cons[nmbcars])
				ghd_car_no++;
			else
			{
				fl = 1;
				ghd_car_no = 1;
				state = 5;
			}
		break;


		case 5:						 // Display Car Calls

			pos = grp_Car_Stat[ghd_car_no].g_pos;
			if ((pos < 1) || (pos > cons[grtopf]))
				pos = 1;


			j = 0;
#if (Quick_Update == 1)
			while ((fl<=cons[grtopf]) && (j<10))
#else
			while ((fl<=cons[grtopf]) && (j<3))
#endif
		    {		// do three floors at a time
				fl_ix = (fl-1)/32;				 
				if(pos != fl)
				{
				    if ( ( ((grp_Car_Stat[ghd_car_no].g_cc[fl_ix] & fl_mask[fl]) != 0) ||
						(((grp_Car_Stat[ghd_car_no].g_rcc[fl_ix] & fl_mask[fl]) != 0) && (rear_op[ghd_car_no] !=0)) )	&&
						 (gc_dev_online[ghd_car_no] == 1) && (Car_Not_Talking[ghd_car_no] == 0) )
						place_assign_call(fl,ghd_car_no,16);
				    else
						clear_assign_call(fl,ghd_car_no,2);
				}
				fl++;
				j++;
			}

			if (fl > cons[grtopf])
			{		// finished floors for one car
				if (ghd_car_no < cons[nmbcars])
				{
					fl = 1;
					ghd_car_no++;
				}
				else
				{
					fl = 1;
					ghd_car_no = 1;
					state = 6;
				}
			}
		break;

		case 6:
			if (gservf == 0)
			{
				if ((dn_peak == 1) && (up_peak == 0))
					vratputf( gwindow, 14, 3, LTCYAN, "DnPk ");
				else if ((up_peak == 1) && (dn_peak == 0))
					vratputf( gwindow, 14, 3, LTCYAN, "UpPk ");
				else if ((up_peak == 1) && (dn_peak == 1))
					vratputf( gwindow, 14, 3, LTCYAN, "HEAVY");					
				else
					vratputf( gwindow, 14, 3, LTCYAN, "NORM ");
			}
			else if (gservf == 1)
				vratputf( gwindow, 14, 3, MAGENTA, "FIRE ");
			else if (gservf == 2)
				vratputf( gwindow, 14, 3, WHITE, "EMPWR");
			else if (gservf == 3)
				vratputf( gwindow, 14, 3, MAGENTA, "FI/EP");
					
			state = 0;
			break;
		default:
			break;

	}

}

const char servf_tbl[35][4] = {
		"OTS",            //0
		"AUT",            //1
		"IND",            //2
		"LWB",            //3
		"ATT",            //4
		"CBL",            //5
		"FS2",            //6
		"EMP",            //7
		"EQS",            //8
		"FS1",            //9
		"FSA",            //10
		"HOM",            //11
		"RST",            //12
		"RST",            //13
		"STL",            //14
		"RTL",            //15
		"OVL",            //16
		"MED",            //17
		"LWC",            //18
		"EOF",            //19
		"HEO",            //20
		"LPR",            //21
		"HSV",            //22
		"VIP",            //23
		"SEC",            //24
        "SBT",            //25
		"TUG",            //26
		"HOT",            //27
		"ROT",            //28
		"EO2",            //29
		"   ",            //30
		"   ",            //31
		"   ",            //32
		"   ",            //33
		"   ",            //34
		};


void place_hall_call(int16 fl, int16 type)
{
	int16 col;
	int16 row;

	if (fl <= (cons[grtopf] - 30))
	{
		row = 48 - fl;
		col = 42;
	}
	else
	{
		row = (cons[grtopf] + 18) - fl;
		col = 2;
	}
	switch  (type)
	{
		case 1: 	// front up hall call
			ScreenPutChar((uint8)c_hc_up_arrow, GREEN, col+1, row);
			break;	
		case 2:    // front down hall call
			ScreenPutChar((uint8)c_hc_dn_arrow, RED, col+1, row);
			break;
		case 3:	   // front up and dow hall call
			ScreenPutChar((uint8)c_hc_up_dn_arrow, CYAN, col+1, row);
			break;
		case 4: 	// rear up hall call
			ScreenPutChar((uint8)c_hc_up_arrow, LTGREEN, col, row);
			break;	
		case 8:    // rear down hall call
			ScreenPutChar((uint8)c_hc_dn_arrow, LTRED, col, row);
			break;
		case 12:	   // rear up and dow hall call
			ScreenPutChar((uint8)c_hc_up_dn_arrow, CYAN, col, row);
			break;
		default:
			break;
	}
		
}

void clear_hall_call(int16 fl, int16 type)
{
	int16 col;
	int16 row;

	if (fl <= (cons[grtopf] - 30))
	{
		row = 48 - fl;
		col = 42;
	}
	else
	{
		row = (cons[grtopf] + 18) - fl;
		col = 2;
	}
	switch  (type)
	{
		case 0:	   // front up and dow hall call
			ScreenPutChar(' ', CYAN, col+1, row);
			break;
		case 1:	   // rear up and dow hall call
			ScreenPutChar(' ', CYAN, col, row);
			break;
		default:
			break;
	}
		
}

void place_assign_call(int16 fl, int16 car, int16 type)
{
	int16 col;
	int16 row;

	if (fl <= (cons[grtopf] - 30))
	{
		col = 48 + ((car -1) * 4);
		row = 48 - fl;
	}
	else
	{
		col = 8 + ((car -1) * 4);
		row = (cons[grtopf] + 18) - fl;
	}
	switch  (type)
	{
		case 1: 	// front up hall call
			ScreenPutChar((char)c_hc_up_arrow, GREEN, col+2, row);
			break;	
		case 2:    // front down hall call
			ScreenPutChar((char)c_hc_dn_arrow, RED, col+2, row);
			break;
		case 3:	   // front up and dow hall call
			ScreenPutChar((char)c_hc_up_dn_arrow, CYAN, col+2, row);
			break;
		case 4: 	// rear up hall call
			ScreenPutChar((char)c_hc_up_arrow, LTGREEN, col, row);
			break;	
		case 8:    // rear down hall call
			ScreenPutChar((char)c_hc_dn_arrow, LTRED, col, row);
			break;
		case 12:	   // rear up and dow hall call
			ScreenPutChar((char)c_hc_up_dn_arrow, LTCYAN, col, row);
			break;
		case 16:		// car call 
			ScreenPutChar((char)c_cc_car, WHITE, col+1, row);
			break;
		default:
			break;
	}
		
}

void clear_assign_call(int16 fl, int16 car, int16 type)
{
	int16 col;
	int16 row;

	if (fl <= (cons[grtopf] - 30))
	{
		col = 48 + ((car -1) * 4);
		row = 48 - fl;
	}
	else
	{
		col = 8 + ((car -1) * 4);
		row = (cons[grtopf] + 18) - fl;
	}
	switch  (type)
	{
		case 0:	   // front up and dow hall call
			ScreenPutChar(' ', CYAN, col+2, row);
			break;
		case 1:	   // rear up and dow hall call
			ScreenPutChar(' ', LTCYAN, col, row);
			break;
		case 2:		// car call 
			ScreenPutChar(' ', WHITE, col+1, row);
			break;
		default:
			break;
	}
		
}

void place_car(int16 car_no, int16 pos, int16 attr)
{

	if (pos <= (cons[grtopf] - 30))
	{
		col = 49 + ((car_no - 1) * 4);
		row = 48 - pos;
	}
	else
	{
		col = 9 + ((car_no -1) * 4);
		row = (cons[grtopf] + 18) - pos;
	}
	ScreenPutChar((uint8)c_car, attr, col, row);
}

void clear_car(int16 car_no, int16 pos )
{
	if (pos <= (cons[grtopf] - 30))
	{
		col = 49 + ((car_no -1) * 4);
		row = 48 - pos;
	}
	else
	{
		col = 9 + ((car_no -1) * 4);
		row = (cons[grtopf] + 18) - pos;
	}
	ScreenPutChar(' ', WHITE, col, row);
}

//*****************************************************
//**     this is the video position function         **
//*****************************************************
void Video_PI_60(int16 car_no, int16 init_flag)
{
	int16 firefv,doorfv,rdoorfv,dprefv,servfv,empfv,ncu_doorv;
	int16 i,yinc,pos;
	int16 arrow_window = arrow1;
	int16 svc_attr;
	int16 newx,newy,newdoor_top,newdoor_mid,newdoor_bot;
	int16 newdir = LTCYAN;
	static int16 prev_rdoorf[car_size],prev_doorf[car_size],prev_servfv[car_size];
	static int16 prev_dpref[car_size],prev_pos[car_size],prev_firef[car_size];
	static int16 prev_empf[car_size],prev_ncu_door[car_size];
	static int16 prev_fault[car_size];
	static int16 prdo[car_size],pfdo[car_size],pcarcom[car_size],p_asvtof[9];
	int16 fdo,rdo;

	if (init_flag == 1)
	{
		for (i=1;i<=cons[nmbcars];i++)
		{
			prev_pos[i] = -1;
			prev_dpref[i] = -1;
			prev_doorf[i] = -1;
			prev_rdoorf[i] = -1;
			prev_servfv[i] = -1;
			prev_firef[i] = -1;
			prev_empf[i] = -1;
			prev_fault[i] = -1;
			prdo[i] = -1;
			pfdo[i] = -1;
			pcarcom[i] = -1;
		}
		return;
	}


	if(cons[grtopf] >= 11)
		yinc = 1;
	else
		yinc = 2;


	pos = grp_Car_Stat[car_no].g_pos;
	if ((pos < 1) || (pos > cons[grtopf]))
		pos = 1;

	servfv = grp_Car_Stat[car_no].g_servf;
	firefv = grp_Car_Stat[car_no].g_firef;
	doorfv = grp_Car_Stat[car_no].g_doorf;
	rdoorfv = grp_Car_Stat[car_no].g_rdoorf;
	dprefv = grp_Car_Stat[car_no].g_dpref;
	empfv = grp_empf[power_feeder[car_no]][car_no];
	ncu_doorv = ncu_door[car_no];

	if ( ((grp_Car_Stat[car_no].g_run_statusf & rs_GS) == 0) || ((doorfv != 0) && ((grp_Car_Stat[car_no].g_run_statusf & rs_DCL) != 0)) )
		fdo = 1;
	else fdo = 0;

	if ((rear_op[car_no] != 0) && 
		(  ((grp_Car_Stat[car_no].g_run_statusf & rs_RGS) == 0) || ((rdoorfv != 0) && ((grp_Car_Stat[car_no].g_run_statusf & rs_DCLR) != 0)) ))
		rdo = 1;
	else rdo = 0;

    if( (pos != prev_pos[car_no]) || (dprefv != prev_dpref[car_no]) ||
		(doorfv != prev_doorf[car_no]) || (servfv != prev_servfv[car_no]) ||
		(rdoorfv != prev_rdoorf[car_no]) || (firefv != prev_firef[car_no]) ||
		(empfv != prev_empf[car_no]) || (ncu_doorv != prev_ncu_door[car_no]) ||
		(grp_Car_Stat[car_no].g_car_has_fault != prev_fault[car_no]) || (fdo != pfdo[car_no]) || 
		(rdo != prdo[car_no]) || (Car_Not_Talking[car_no] != pcarcom[car_no]) ||
		(p_asvtof[car_no] != asvtof[car_no]) )
    {
		if(car_no == 1)
		    arrow_window = arrow1;
		else if(car_no == 2)
		    arrow_window = arrow2;
		else if(car_no == 3)
		    arrow_window = arrow3;
		else if(car_no == 4)
		    arrow_window = arrow4;
		else if(car_no == 5)
		    arrow_window = arrow5;
		else if(car_no == 6)
		    arrow_window = arrow6;
		else if(car_no == 7)
		    arrow_window = arrow7;
		else if(car_no == 8)
		    arrow_window = arrow8;

		newx = Vid_Car_Xpos[car_no] + c_car_offset_60;
		newy = 5;

		if ( (prev_pos[car_no] < 1) || (prev_pos[car_no] > cons[grtopf]) )
			prev_pos[car_no] = cons[grtopf];

		if (ncu_doorv == 1)
			newdir = YELLOW;
		else if(dprefv == 0)      // no direction 
			newdir = LTCYAN;          // blue 
		else if(dprefv == 1)      // up direction 
			newdir = GREEN;     // green 
		else if(dprefv == 2)      // down direction 
			newdir = RED;          // red 

		if (Car_Not_Talking[car_no] == 1)
		{			// front and rear closed
		    newdoor_top = Car_Door_Closed;
			newdoor_mid = Car_Door_Closed;
			newdoor_bot = Car_Door_Closed;
		}
		else if( (fdo == 1) && (rdo == 1))
		{				  // front and rear open
		    newdoor_top = ' ';
			newdoor_mid = ' ';
			newdoor_bot = ' ';
		}
		else if ( (fdo == 1) && (rdoorfv == 0) )
		{				   // front door open
		    newdoor_top = Car_Door_Closed;
			newdoor_mid = ' ';
			newdoor_bot = ' ';
		}
		else if( (doorfv == 0) &&  (rdo == 1) )
		{				// rear door open
		    newdoor_top = ' ';
			newdoor_mid = ' ';
			newdoor_bot = Car_Door_Closed;
		}
		else
		{			// front and rear closed
		    newdoor_top = Car_Door_Closed;
			newdoor_mid = Car_Door_Closed;
			newdoor_bot = Car_Door_Closed;
		}

		if (((servfv != 1) && (servfv !=11) && (servfv != 4)) || (Car_Not_Talking[car_no] == 1) || (asvtof[car_no] == 1)) 
			newdir = WHITE;
		if (firefv != 0)
			newdir = MAGENTA;

		vratputf( gwindow,   newy, newx, newdir, "%c%c%c%c%c",Car_Right,newdoor_top,newdoor_top,newdoor_top,Car_Left );
		vratputf( gwindow, newy+1, newx, newdir, "%c%c%c%c%c",Car_Right,newdoor_mid,newdoor_mid,newdoor_mid,Car_Left );
		vratputf( gwindow, newy+2, newx, newdir, "%c%c%c%c%c",Car_Right,newdoor_bot,newdoor_bot,newdoor_bot,Car_Left );

		clear_car(car_no,prev_pos[car_no]);
		place_car(car_no,pos,newdir);


		if ((asvtof[car_no] == 1) && (grp_Car_Stat[car_no].g_servf == 1))
			svc_attr = LTBLUE;
		else if (grp_Car_Stat[car_no].g_car_has_fault != 0)
			svc_attr = LTRED;
		else
			svc_attr = WHITE;

		if (Car_Not_Talking[car_no] == 1)
			vratputf( gwindow, 3, newx+1, WHITE, "COM");
		else
			vratputf( gwindow, 3, newx+1, svc_attr, "%.3s", servf_tbl[servfv]);

		vratputf( gwindow, newy+4 , newx,LTCYAN,"%.2s",flmrk[pos]);	// CAR position

		if(newdir == GREEN)
		    vratputf( gwindow, newy+4, newx+2, GREEN, " %c ",Up_Arrow );
		else if(newdir == LTCYAN)
		    vratputf( gwindow, newy+4, newx+2, LTCYAN, "   ");
		else if(newdir == RED)
		    vratputf( gwindow, newy+4, newx+2, RED, " %c ",Dn_Arrow );
		else if (newdir == YELLOW)
		    vratputf( gwindow, newy+4, newx+2, YELLOW, " %c ",Up_Arrow );
		else
		    vratputf( gwindow, newy+4, newx+2, WHITE, " %c ",bar );

		if (car_no == Dispatcher)
		{
			vratputf( gwindow, 0, (Vid_Car_Xpos[car_no] + c_car_offset_60),YELLOW,"CAR #%.2s",car_bld_no[car_no]);
		}
		else
		{
			vratputf( gwindow, 0, (Vid_Car_Xpos[car_no] + c_car_offset_60),WHITE,"CAR #%.2s",car_bld_no[car_no]);
		}

		prev_pos[car_no] = pos;
		prev_dpref[car_no] = dprefv;
		prev_doorf[car_no] = doorfv;
		prev_rdoorf[car_no] = rdoorfv;
		prev_servfv[car_no] = servfv;
		prev_firef[car_no] = firefv;
		prev_empf[car_no] = empfv;
		prev_ncu_door[car_no] = ncu_doorv;
		prev_fault[car_no] = grp_Car_Stat[car_no].g_car_has_fault;
		prdo[car_no] = rdo;
		pfdo[car_no] = fdo;
		pcarcom[car_no] = Car_Not_Talking[car_no];
		p_asvtof[car_no] = asvtof[car_no];
    }
	GrpHall_PosInd_MOVE();
	return;
}

//*****************************************************
//**     this is the video position function         **
//*****************************************************
void Video_PI(int16 car_no, int16 init_flag)
{
	int16 firefv,pos,doorfv,rdoorfv,dprefv,servfv,empfv,ncu_doorv;
	int16 i,yinc;
	int16 arrow_window = arrow1;
	int16 svc_attr;
	int16 oldy,newx,newy,newdoor;
	int16 newdir = LTCYAN;
	static int16 prev_rdoorf[car_size],prev_doorf[car_size],prev_servfv[car_size];
	static int16 prev_dpref[car_size],prev_pos[car_size],prev_firef[car_size];
	static int16 prev_empf[car_size],prev_ncu_door[car_size];
	static int16 O_Pointer_X, O_Pointer_Y;
	static int16 prev_fault[car_size];
	static int16 prdo[car_size],pfdo[car_size],pcarcom[car_size],p_asvtof[9];
	int16 cursor_r = 26;
	int16 cursor_l = 27;
	int16 fdo,rdo;

	if (init_flag == 1)
	{
		for (i=1;i<=cons[nmbcars];i++)
		{
			prev_pos[i] = -1;
			prev_dpref[i] = -1;
			prev_doorf[i] = -1;
			prev_rdoorf[i] = -1;
			prev_servfv[i] = -1;
			prev_firef[i] = -1;
			prev_empf[i] = -1;
			prev_fault[i] = -1;
			prdo[i] = -1;
			pfdo[i] = -1;
			pcarcom[i] = -1;
		}
		O_Pointer_X = 0;
		O_Pointer_Y = 1;
		Call_Pointer_X = 1;
		Call_Pointer_Y = grp_Car_Stat[cons[carnmb]].g_pos;
		return;
	}


	if(cons[grtopf] >= 11)
		yinc = 1;
	else
		yinc = 2;

    if (((Call_Pointer_Y != O_Pointer_Y) || (Call_Pointer_X != O_Pointer_X)) && (keybrd_found == 1))
    {
		if (O_Pointer_X == 0)
	    	vratputf( upbox, (max_car_row - (O_Pointer_Y*yinc)), 4, WHITE, " ");
		else if (O_Pointer_X == (cons[nmbcars] + 1))
		    vratputf( dnbox, (max_car_row - (O_Pointer_Y*yinc)), 0, WHITE, " ");
		else if (O_Pointer_X == 1)
		{
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[1]]-2), WHITE, " ");
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[1]]+4), WHITE, " ");
		}
		else if (O_Pointer_X == 2)
		{
			vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[2]]-2), WHITE, " ");
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[2]]+4), WHITE, " ");
		}
		else if (O_Pointer_X == 3)
	  	{
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[3]]-2), WHITE, " ");
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[3]]+4), WHITE, " ");
	  	}
		else if (O_Pointer_X == 4)
	  	{
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[4]]-2), WHITE, " ");
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[4]]+4), WHITE, " ");
	  	}
		else if (O_Pointer_X == 5)
	  	{
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[5]]-2), WHITE, " ");
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[5]]+4), WHITE, " ");
	  	}
		else if (O_Pointer_X == 6)
	  	{
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[6]]-2), WHITE, " ");
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[6]]+4), WHITE, " ");
	  	}
		else if (O_Pointer_X == 7)
	  	{
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[7]]-2), WHITE, " ");
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[7]]+4), WHITE, " ");
	  	}
		else if (O_Pointer_X == 8)
	  	{
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[8]]-2), WHITE, " ");
		    vratputf( carbox, (max_car_row - (O_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[8]]+4), WHITE, " ");
	  	}

		if (Call_Pointer_X == 0)
	    	vratputf( upbox, (max_car_row - (Call_Pointer_Y*yinc)), 4, WHITE, "%c",cursor_r);
		else if (Call_Pointer_X == (cons[nmbcars] + 1))
	    	vratputf( dnbox, (max_car_row - (Call_Pointer_Y*yinc)), 0, WHITE, "%c",cursor_l);
		else if (Call_Pointer_X == 1)
		{
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[1]]-2), WHITE, "%c",cursor_r);
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[1]]+4), WHITE, "%c",cursor_l);
		}
  		else if (Call_Pointer_X == 2)
  		{
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[2]]-2), WHITE, "%c",cursor_r);
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[2]]+4), WHITE, "%c",cursor_l);
		}
  		else if (Call_Pointer_X == 3)
	  	{
	    	vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[3]]-2), WHITE, "%c",cursor_r);
	    	vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[3]]+4), WHITE, "%c",cursor_l);
	  	}
  		else if (Call_Pointer_X == 4)
	  	{
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[4]]-2), WHITE, "%c",cursor_r);
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[4]]+4), WHITE, "%c",cursor_l);
	  	}
  		else if (Call_Pointer_X == 5)
	  	{
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[5]]-2), WHITE, "%c",cursor_r);
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[5]]+4), WHITE, "%c",cursor_l);
	  	}
  		else if (Call_Pointer_X == 6)
	  	{
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[6]]-2), WHITE, "%c",cursor_r);
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[6]]+4), WHITE, "%c",cursor_l);
	  	}
  		else if (Call_Pointer_X == 7)
	  	{
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[7]]-2), WHITE, "%c",cursor_r);
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[7]]+4), WHITE, "%c",cursor_l);
	  	}
  		else if (Call_Pointer_X == 8)
	  	{
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[8]]-2), WHITE, "%c",cursor_r);
		    vratputf( carbox, (max_car_row - (Call_Pointer_Y*yinc)), (Vid_Car_Xpos[vidcar[8]]+4), WHITE, "%c",cursor_l);
	  	}

      	O_Pointer_Y = Call_Pointer_Y;
      	O_Pointer_X = Call_Pointer_X;
    }

	pos = grp_Car_Stat[car_no].g_pos;
	if ((pos < 1) || (pos > cons[grtopf]))
		pos = 1;
	servfv = grp_Car_Stat[car_no].g_servf;
	firefv = grp_Car_Stat[car_no].g_firef;
	doorfv = grp_Car_Stat[car_no].g_doorf;
	rdoorfv = grp_Car_Stat[car_no].g_rdoorf;
	dprefv = grp_Car_Stat[car_no].g_dpref;
	empfv = grp_empf[power_feeder[car_no]][car_no];
	ncu_doorv = ncu_door[car_no];

	if ( ((grp_Car_Stat[car_no].g_run_statusf & rs_GS) == 0) || ((doorfv != 0) && ((grp_Car_Stat[car_no].g_run_statusf & rs_DCL) != 0)) )
		fdo = 1;
	else fdo = 0;

	if ((rear_op[car_no] != 0) && 
		(  ((grp_Car_Stat[car_no].g_run_statusf & rs_RGS) == 0) || ((rdoorfv != 0) && ((grp_Car_Stat[car_no].g_run_statusf & rs_DCLR) != 0)) ))
		rdo = 1;
	else rdo = 0;

    if( (pos != prev_pos[car_no]) || (dprefv != prev_dpref[car_no]) ||
		(doorfv != prev_doorf[car_no]) || (servfv != prev_servfv[car_no]) ||
		(rdoorfv != prev_rdoorf[car_no]) || (firefv != prev_firef[car_no]) ||
		(empfv != prev_empf[car_no]) || (ncu_doorv != prev_ncu_door[car_no]) ||
		(grp_Car_Stat[car_no].g_car_has_fault != prev_fault[car_no]) || (fdo != pfdo[car_no]) || 
		(rdo != prdo[car_no]) || (Car_Not_Talking[car_no] != pcarcom[car_no]) ||
		(p_asvtof[car_no] != asvtof[car_no]) )
    {
		if(car_no == 1)
		    arrow_window = arrow1;
		else if(car_no == 2)
		    arrow_window = arrow2;
		else if(car_no == 3)
		    arrow_window = arrow3;
		else if(car_no == 4)
		    arrow_window = arrow4;
		else if(car_no == 5)
		    arrow_window = arrow5;
		else if(car_no == 6)
		    arrow_window = arrow6;
		else if(car_no == 7)
		    arrow_window = arrow7;
		else if(car_no == 8)
		    arrow_window = arrow8;

		newx = Vid_Car_Xpos[car_no];
		if ( (prev_pos[car_no] < 1) || (prev_pos[car_no] > cons[grtopf]) )
			prev_pos[car_no] = cons[grtopf];
		oldy = (max_car_row - (prev_pos[car_no]*yinc));
		newy = (max_car_row - (pos*yinc));
		if (ncu_doorv == 1)
			newdir = YELLOW;
		else if(dprefv == 0)      // no direction 
			newdir = LTCYAN;          // blue 
		else if(dprefv == 1)      // up direction 
			newdir = GREEN;     // green 
		else if(dprefv == 2)      // down direction 
			newdir = RED;          // red 

		if (Car_Not_Talking[car_no] == 1)
		    newdoor = Car_Door_Closed;
		else if( (fdo == 1) && (rdo == 1))
		    newdoor = Car_FRdoor_Open;
		else if ( (fdo == 1) && (rdoorfv == 0) )
		    newdoor = Car_Fdoor_Open;
		else if( (doorfv == 0) &&  (rdo == 1) )
		    newdoor = Car_Rdoor_Open;
		else
		    newdoor = Car_Door_Closed;

		if (((servfv != 1) && (servfv !=11) && (servfv != 4)) || (Car_Not_Talking[car_no] == 1) || (asvtof[car_no] == 1)) 
			newdir = WHITE;
		if (firefv != 0)
			newdir = MAGENTA;

		vratputf( carbox, oldy, newx, WHITE, "   ");
		vratputf( carbox, newy, newx, newdir, "%c%c%c",Car_Right,newdoor,Car_Left );

		if ((asvtof[car_no] == 1) && (grp_Car_Stat[car_no].g_servf == 1))
			svc_attr = LTBLUE;
		else if (grp_Car_Stat[car_no].g_car_has_fault != 0)
			svc_attr = LTRED;
		else
			svc_attr = WHITE;

		if (Car_Not_Talking[car_no] == 1)
			vratputf( carbox, 0, newx, WHITE, "COM");
		else
			vratputf( carbox, 0, newx, svc_attr, "%.3s", servf_tbl[servfv]);

		if(newdir == GREEN)
		    vratputf( arrow_window, 0, 0, GREEN, " %c ",Up_Arrow );
		else if(newdir == LTCYAN)
		    vratputf( arrow_window, 0, 0, LTCYAN, "   ");
		else if(newdir == RED)
		    vratputf( arrow_window, 0, 0, RED, " %c ",Dn_Arrow );
		else if (newdir == YELLOW)
		    vratputf( arrow_window, 0, 0, YELLOW, " %c ",Up_Arrow );
		else
		    vratputf( arrow_window, 0, 0, WHITE, " %c ",bar );

		if (car_no == Dispatcher)
		{
			vratputf( gwindow, 0, (Vid_Car_Xpos[car_no] + c_car_offset), YELLOW, "CAR");
			vratputf( gwindow, 1, (Vid_Car_Xpos[car_no] + c_car_offset),YELLOW,"#%.2s",car_bld_no[car_no]);
		}
		else
		{
			vratputf( gwindow, 0, (Vid_Car_Xpos[car_no] + c_car_offset), WHITE, "CAR");
			vratputf( gwindow, 1, (Vid_Car_Xpos[car_no] + c_car_offset),WHITE,"#%.2s",car_bld_no[car_no]);
		}

		prev_pos[car_no] = pos;
		prev_dpref[car_no] = dprefv;
		prev_doorf[car_no] = doorfv;
		prev_rdoorf[car_no] = rdoorfv;
		prev_servfv[car_no] = servfv;
		prev_firef[car_no] = firefv;
		prev_empf[car_no] = empfv;
		prev_ncu_door[car_no] = ncu_doorv;
		prev_fault[car_no] = grp_Car_Stat[car_no].g_car_has_fault;
		prdo[car_no] = rdo;
		pfdo[car_no] = fdo;
		pcarcom[car_no] = Car_Not_Talking[car_no];
		p_asvtof[car_no] = asvtof[car_no];
    }
	return;
}

void Mem_dump_screen(int16 init_flag)
{
  	int16 k;
	static int16 state;
	static int16 mem_ix;
	uint32 *ptr;
static union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
	if (init_flag == 1)
	{
		state = 0;
		mem_ix = 0;
		return;
	}

	switch (state)
	{
		case 0:
			wnum = 0;
			state = 1;
		    ScreenClear();
			break;
		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:	 
			set_font(font_8x14);	// set 8x14 to get 28 rows
			max_row = 24;
			max_car_row = 21;
			max_window_row = 28;
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			nmlscn = vcreat( max_window_row, 80, WHITE);
			vlocate( nmlscn, 1, 1);
			state = 5;
			break;
		case 5:
			if (mem_type == 0)
				vratputf(nmlscn,1,4,WHITE,"EERam Memory Dump");
			else if (mem_type == 1)
				vratputf(nmlscn,1,4,WHITE,"NVRam Memory Dump");
			else if (mem_type == 2)
				vratputf(nmlscn,1,4,WHITE,"MRam Memory Dump");
			else if (mem_type == 3)
				vratputf(nmlscn,1,4,WHITE,"Ram Memory Dump");

			row = 2;
			col = 1;

			state = 6;

			break;
		case 6:
			k = 0;
			while ((k<=20) && (col <= 80))
			{
				if (mem_type == 2)
				{
					
					vratputf(nmlscn,row,col,WHITE,"%2xh,",rd_mram(mem_page ,(uint16)(mem_offset + mem_ix)));
					k++; 
					mem_ix++;
					col+=4;
				}
				else if (mem_type == 3)
				{
					uiu.b[mhb] = (uint8)mem_page;
					uiu.b[hb] = 0x40;
					uiu.w[lw] = (uint16)(mem_offset + mem_ix);
					ptr = (uint32 (*))uiu.dw;
					vratputf(nmlscn,row,col,WHITE,"%08xh,",*ptr);
					mem_ix+=4;
					k+=4;
					col+=10;
				}
				if (mem_ix >= mem_len)
					break;
			}
			if (mem_ix >= mem_len)
				state = 7;
			else if (col >= 80)
			{
				if (row > 24)
					state = 7;
				else
				{
					col = 1;
					row++;
				}
			}
			break;

		case 7:
			if (next_mem_page == 1)
			{
				if (mem_ix >= mem_len)
				{
					dpy_mode = 4;	// debug screen
					Init_Debug_Screen(Reset_State);
					state = 0;
				}
				next_mem_page = 0;
			}
			break;

		default:
			state = 0;
			break;
  	}
  	return;
}


//**********************************************
//***    This is the video faults screen     ***
//**********************************************
void Fault_Screen(int16 Car_Select,int16 init_flag)
{

  	int16 j,k;
	static int16 state;
	static int16 loop_ix;
	static int16 dpy_ix;
	static int16 init_trace_state;
	static int16 trace_state;
		
	if (init_flag == 1)
	{
		state = 0;
		return;
	}

	switch (state)
	{
		case 0:
			wnum = 0;
			state = 1;
		    ScreenClear();
			break;
		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:	 
			set_font(font_8x14);	// set 8x14 to get 28 rows
			max_row = 24;
			max_car_row = 21;
			max_window_row = 28;
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			nmlscn = vcreat( max_window_row, 80, WHITE);
			vlocate( nmlscn, 1, 1);
			state = 5;
			break;
		case 5:
			if (Car_Select != cons[carnmb])
			{
				flt_ix = Car_Flt_ix + 1;
				vratputf(nmlscn,1,3,WHITE,"Waiting for Data Car #%.2s",car_bld_no[Car_Select]);
				put_pkt_req(8,Car_Select,gc_port);
				state = 6;
				timers[tfltcarcom] = 0;
			}
			else
			{
				flt_ix = Flt_ix + 1;
				timers[tfltcarcom] = 21;
				state = 7;
			}
			if ((flt_ix >= max_flt_ix) || (flt_ix < 0))
				flt_ix = 0;	
			loop_ix = 0;
			dpy_flt = 0;
			dpy_ix = 1;

			break;

		case 6:
			j = get_pkt_status(36,Car_Select,gc_port,200);
			if (j == 1)
				state = 7;
			else if (j == 2)
				state = 9;
			break;

		case 7:
			if (timers[tfltcarcom] > 20)
			{
				row = 3;
				col = 1;
				dpy_loop = 0;
				state = 8;
				vc_state = 0;
			}
			break;
			
		case 8:
			vc_state = vclear(popupbox,vc_state);
			if (vc_state == 0)
			{
				if (detailed_fault == 0)
				{
					vratputf(nmlscn,1,1,WHITE,"Fault Log Car #%.2s",car_bld_no[Car_Select]);
					state = 9;
				}
				else
				{
					if (Car_Select != cons[carnmb])
						flt_ix = Car_Flt_ix + 1;
					else
						flt_ix = Flt_ix + 1;
					
					if ((flt_ix >= max_flt_ix) || (flt_ix < 0))
						flt_ix = 0;	
					fault_ix_start = flt_ix;
					fault_dpy_ix = 1;
					state = 11;
				}
			}
			break;
		

		case 9:
			
			k = 1;
#if(Quick_Update == 1)
			while ((row < 28) && (loop_ix < max_flt_ix) && (k <= 2))
#else
			while ((row < 28) && (loop_ix < max_flt_ix) && (k <= 4))
#endif
			{
				if (Car_Select != cons[carnmb])
					flp = &cf.Flts[flt_ix];
				else
					flp = &f.Flts[flt_ix];

				if (flp->code != 0)
				{
					dpy_flt = 1;
					dpy_loop = 1;
					vratputf(nmlscn,row,col,WHITE,"%3i %.20s ",dpy_ix++,&LCD_Fault[flp->code][3]);
					vratputf(nmlscn,row++,(col+25),WHITE,
					"%2i:%02i:%02i %2i/%02i/%4i Position =%2i Occurrences =%4i ",
					flp->hour,flp->min,flp->sec,
					flp->mon,flp->day,
					flp->year,flp->pos,
					flp->count);
					k++;
			   	}
				flt_ix++;
				if (flt_ix >= max_flt_ix)
					flt_ix = 0;
				loop_ix++;
				if ((detailed_fault == 1)  && (row >= 20))
					break;				 // can only do one at a time with detailed faults
			}
			if (loop_ix >= max_flt_ix)
			{
				if (dpy_flt == 0)
					vratputf(nmlscn,row++,(col+28),WHITE,"No Occurences                                 ");
				else if (dpy_loop == 0)				// Nothing put on display this time through
					Rotate_Fault = 1;			// Rotate to next set of faults
			}
			if ((row >= 28) || (loop_ix >= max_flt_ix) || ((detailed_fault == 1) && (row >= 20)))
			 	state = 10;
			
			break;

		case 10:
			if (loop_ix >= max_flt_ix)
			{
				if (Car_Select != cons[carnmb])
					flt_ix = Car_Flt_ix + 1;
				else
					flt_ix = Flt_ix + 1;

				if ((flt_ix >= max_flt_ix) || (flt_ix < 0))
					flt_ix = 0;
				dpy_ix = 1;	
				loop_ix = 0;
				dpy_flt = 0;
			}
			if (Rotate_Fault == 1)		// View more faults
			{
				Rotate_Fault = 0;
			 	state = 7;
			}
			break;

		case 11:
			Track_Rotate_State = 0;
			while (loop_ix < max_flt_ix)
			{
				
				if (Car_Select != cons[carnmb])
					flp = &cf.Flts[flt_ix];
				else
					flp = &f.Flts[flt_ix];

				if (flp->code != 0)
				{
					dpy_flt = 1;
					init_trace_state = 0;
					state = 14;
					break;
				}
				else
				{
					flt_ix++;
					if (flt_ix >= max_flt_ix)
						flt_ix = 0;
					
					fault_ix_start = flt_ix;
					loop_ix++;
				}
			}
			if (loop_ix >= max_flt_ix)
			{
				if (dpy_flt == 0)
					vratputf(nmlscn,0,7,WHITE,"No Occurences                                 ");
			 	state = 12;
			}
			
			break;

		case 12:
			if ((next_trace != 0) || (Trace_Track_Rotate_Ix != Prev_Trace_Track_Rotate_Ix))
			{
				state = 7;
				next_trace = 0;
				Prev_Trace_Track_Rotate_Ix = Trace_Track_Rotate_Ix;
				Track_Rotate_State = 0;
			}
			break;
			
		case 13:
			if (next_trace != 0)
			{
				loop_ix = 0;
				while (loop_ix < max_flt_ix)
				{
						
					if (next_trace > 0)
					{
							
						if ((flt_ix + next_trace) >= max_flt_ix)
							flt_ix = next_trace - (max_flt_ix - flt_ix);
						else
							flt_ix += next_trace;
					}
					else
					{
						if ((flt_ix + next_trace) < 0)
							flt_ix = max_flt_ix + flt_ix + next_trace;	  // Restart the index
						else	
							flt_ix += next_trace;
					
					}
					
				
					if (Car_Select != cons[carnmb])
						flp = &cf.Flts[flt_ix];
					else
						flp = &f.Flts[flt_ix];

					if (flp->code != 0)
					{
						break;
					}
					loop_ix++;
				}
				next_trace = 0;
				if (flt_ix >= fault_ix_start)
					fault_dpy_ix = (flt_ix - fault_ix_start) + 1;
				else
					fault_dpy_ix = (max_flt_ix - fault_ix_start) + flt_ix + 1;
				
				state = 15;
			}
			if (Trace_Track_Rotate_Ix != Prev_Trace_Track_Rotate_Ix)
			{
				Track_Rotate_State = Init_Trace_Block(Track_Rotate_State);
				if (Track_Rotate_State == 0)
				{
					state = 15;
					Prev_Trace_Track_Rotate_Ix = Trace_Track_Rotate_Ix;
				}
			}
			else
				Track_Rotate_State = 0;
			break;
				
		case 14:
				
			init_trace_state = Init_Trace_Io_Screen(Car_Select,init_trace_state);
			if (init_trace_state == 0)
			{
				trace_state = 0;
				state = 15;
			}
			break;
			
		case 15:
			if (Car_Select != cons[carnmb])
				flp = &cf.Flts[flt_ix];
			else
				flp = &f.Flts[flt_ix];
			trace_state = Trace_Io_Screen(Car_Select, trace_state, flp);
			if (trace_state == 0)
				state = 13;
			break;
		default:
			state = 0;
			break;
  	}
  	return;
}

void video_time_out	(int16 video_mode)
{
	if (video_mode == turn_video_off)
	{		 // Turn off video
		video_timeout = 0;
		video_off = 1;
	    ScreenClear();
	}
	else
	{	   // Turn on video
		timers[tvideo] = 0;
		video_off = 0;
		dpy_mode = save_dpy_mode;

		if (save_dpy_mode == 0)
		{
			dpy_mode = 0;
			init_menu_screen(Reset_State);
		}
		else if (save_dpy_mode == 1)
		{
			if (Car_Select == 0)
			{
				if (((grp_page == 1) || (grp_page == 2)
#if (Tract_HR == 1)
					|| (grp_page == 3) || (grp_page == 4)
#endif
#if (GALaxy_4 == 1)
					) && ((gc_hall_io_dev != 0) || (nmb_grcan_dev != 0)))
#else
					) && (gc_hall_io_dev != 0))
#endif
				{
					Init_Grp_Io_Screen(Reset_State);
					Init_Grp_Io_Screen(Run_State);
				}
				else
				{
					Init_Group_Hall_Display(Reset_State);
					Init_Group_Hall_Display(Run_State);
				}
			}
			else if (Car_Select >= 1)
			{	
				if (Display_Fault == 1)
				{
					dpy_mode = 0;
					init_menu_screen(Reset_State);
//					Fault_Screen(Car_Select,Reset_State);
				}
				else
				{
					Init_Io_Screen(Car_Select,Reset_State);
					Init_Io_Screen(Car_Select,Run_State);
				}
			}
		}
		else if (save_dpy_mode == 2)
		{
			dpy_mode = 0;
			init_menu_screen(Reset_State);
//			job_stats_screen(Reset_State);
		}	
		else if (save_dpy_mode == 3)
		{		// dpy_mode ??
			dpy_mode = 0;
			init_menu_screen(Reset_State);
		}
	  	else if (save_dpy_mode == 4)
		{				// Debug screen display
			Init_Debug_Screen(Reset_State);
			Init_Debug_Screen(Run_State);
		}
		else if (save_dpy_mode == 5)
		{
			dpy_mode = 5;
			Init_Debug_Screen(Reset_State);
			Init_Debug_Screen(Run_State);
//			Mem_dump_screen(Reset_State);
		}	
	}
}

//********************************************************************************************
//********************************************************************************************
//****************   Debug Screen Routines  **************************************************
//********************************************************************************************
//********************************************************************************************


void debug_control (void)
{
	int16 len=0;

	if (dbg_page == 7)
	{
		hidden_kbrd_control();
		len = 0;
	}
	else
		len = get_keybrd(9,2,50,1,trascn);	// debug

	if (len != 0)
	{
		debug_cmd(len);
	}
	if (debug_screen_init == 0)
	{
		Init_Debug_Screen(Run_State);
	}
	else
		Debug_Screen(Run_State);
}

void debug_cmd (int16 len)
{
	int32 i;
	int16 seq, j;
	int16 k;	
	int16 fl_ix;
	bool HexInput;
	int16 FirstNonSpace;
#if (Simulator == 1)
	unsigned char Code_Num[4]={1,1,1,1};
#endif
	char t_off[6];
	char d_off[6];
	unsigned char mem_pattern[8] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};

	enter_key = 0;

	switch (scan_cmd())
	{
		case 0:
			enter_key = 1;
			break;
		case 1:		 // DBG - Initialize debug mode
			i = get_num(3);
			if (i == 1)
				dbg_page = 1;
			else if (i == 2)
				dbg_page = 2;
#if(Traction == 1)
			else if (i == 3)
				dbg_page = 3;
#endif
			else if (i == 4)
				dbg_page = 4;
			else if (i == 5)
				dbg_page = 5;
			else if (i == 6)
				dbg_page = 6;
			else if (i == 7)
				dbg_page = 7;
			else if (i == 8)
				dbg_page = 8;
			else if (i == 9)
				dbg_page = 9;
			else
				dbg_page = 0;

			dpy_mode = 4;
			Init_Debug_Screen(Reset_State);
			freeze = 0;
			echo = 1;
			row = 9;
			col = 2;
			vratputf(trascn,row,col,WHITE,">                                                  ");
			break;
		case 2:	  	 // EXI - Exit debug mode							
			if (Dispatcher == cons[carnmb])
			{
				dpy_mode = 0;
				Init_Debug_Screen(Reset_State);
				init_menu_screen(Reset_State);
				freeze = 0;
			}
			else
			{
				dpy_mode = 1;
				echo = 0;
				Init_Io_Screen(Car_Select,Reset_State);		// Reset State variable
				Init_Grp_Io_Screen(Reset_State);		// Reset State Variable
				Init_Debug_Screen(Reset_State);
				freeze = 0;
			}
			break;					
		case 3:			// MTR - Memory TRace: MTR 0,VEL,7453,f
			if (dpy_mode == 4)
			{				   // debug mode only
			    HexInput = false;		// set default input type as decimal
			    FirstNonSpace = LocNonSpace(len); // Locate first NonSpace

				if ( (kb_buff[FirstNonSpace+1] == ',') &&
					(kb_buff[FirstNonSpace] >= '0') && (kb_buff[FirstNonSpace] <= '9'))
				{		// valid so far
					// Get data name
					j = 0; 
					trace_num = (int16)(kb_buff[FirstNonSpace] - '0');
					seq = 0;
					for (i=FirstNonSpace+2;i<=len;i++)  
					{		// Decode the input string
						if (seq == 0)
						{	   // Get data variable name
							if (kb_buff[i] == ',')
							{
								while (j<10)
								{
									trace_name[trace_num][j] =' ';
									j++;
								}
								trace_name[trace_num][j] = 0;	// Set last byte to zero
								j = 0;
								seq = 1;		// Have the name so get the address seg
							} 
							else if (i<FirstNonSpace+12)	   // max of ten characters long
							{
								trace_name[trace_num][j] = kb_buff[i];
								j++;
							}
						}
						else if (seq == 1)
						{
							if (kb_buff[i] == CR)
							{
								t_off[j] = 0;	// Set last byte to zero
								j = 0;
								seq = 4;
								trace_format[trace_num] = 'D';
							}
							else if (kb_buff[i] == ',')
							{
								t_off[j] = 0;
								j = 0;
								seq = 2;
							}
							else
							{
								t_off[j] = kb_buff[i];
								j++;
							}
						}
						else if (seq == 2)
						{
							kb_buff[i] &= 0xdf;
							if ( (kb_buff[i] == 'D') || (kb_buff[i] == 'S') || 
							     (kb_buff[i] == 'X') || (kb_buff[i] == 'U') || 
							     (kb_buff[i] == 'C') || (kb_buff[i] == 'E') || 
							     (kb_buff[i] == 'F') || (kb_buff[i] == 'L') ||	
							     (kb_buff[i] == 'H') || (kb_buff[i] == 'B') || 
							     (kb_buff[i] == 'W') || (kb_buff[i] == 'I') )
								 trace_format[trace_num] = kb_buff[i];
							else
								trace_format[trace_num] = 'D';
							seq = 3;
						}
                       	else if (seq==3)
						{
							if (kb_buff[i] == CR)
							{
								seq = 4;
							}
							else if (kb_buff[i] == ',')
							{
								seq = 5;
								j = 0;
							}
  						}
						else if (seq == 5)
						{
							if (kb_buff[i] == CR)
							{
								d_off[j] = 0;	// Set last byte to zero
								j = 0;
								seq = 4;
							}
							else if(kb_buff[i] =='H')
							   HexInput = true;
							else
							{
								set_memory_variable = 1;
								d_off[j] = kb_buff[i];
								j++;
							}
						}
					}
						  
					if (seq == 4)
					{
						trace_offset[trace_num] = atoh(t_off);
						trace_ptr[trace_num] = (uint32 *)(atoh(t_off) + var_off);
					   
					   // Accept Hex  or Integer  input 
						if(HexInput == true)
					    	debug_variable[trace_num] = atoh(d_off);
					   	else 
					    	debug_variable[trace_num] = atoi(d_off);
					   
						trace_status[trace_num] = 1;
						if (trace_num < 5)
						{
							col = 6;
							row = trace_num + 2;
						}
						else
						{
							col = 43;
							row = (trace_num - 5) + 2;
						}
						vratputf(trascn,row,col,WHITE,"%.10s",trace_name[trace_num]); 
						vratputf(trascn,row,col+11,WHITE,"%0x",(int32)trace_ptr[trace_num]);
						put_trace(row,col+22,trace_num);
					}
				}
				freeze = 0;
			}
			break;
		case 4:		// CLR - Clear trace variables
			if (dpy_mode == 4)
			{		// debug mode only
				if ((kb_buff[3] == ' ') && (kb_buff[5] == CR) &&
					(kb_buff[4] >= '0') && (kb_buff[4] <= '9'))
				{		// valid so far
					trace_num = (int16)(kb_buff[4] - '0');
					trace_status[trace_num] = 0;
					if (trace_num < 5)
					{
						col = 6;
						row = trace_num + 2;
					}
					else
					{
						col = 43;
						row = (trace_num - 5) + 2;
					}
					vratputf(trascn,row,col,WHITE,"                                 "); 
					for (i=0;i<=2;i++)
					{
						if (dump_num[i] == trace_num)
						{
							dump_status[i] = 0;
							vratputf(trascn,(int16)(i+7),4,WHITE,"                                                                           "); 
						}
					}
				}
				freeze = 0;
			}
			break;
		case 5:		// INI - Simulator hoistway initialization
			i = get_num(3);
#if (Simulator == 1) 
	#if (Traction == 1)
			Reset_Hoistway_Init();
			Init_Hoistway();
			hardware_init_fault = 0;
			Hoistway_Learned = 1;
			Wrt_Hoistway();
	#endif
			sim_init(0);
	#if ((Traction == 1) || (Tract_OL == 1))
		// This has to be done here because sim_init will cause a gripper fault when GTS goes off
			if (cons[ansi] >= 2000)
			{
				if (gripper_fault != 0)
				{
					clroutp(o_GR1R);
					clroutp(o_GR2R);
					clroutp(o_LE);
					clroutp(o_LE1);
				}
				else
				{
					// Pick the rope gripper
					setoutp(o_GR1R);
					setoutp(o_GR2R);
					gripper_test = 0;
				}
			}
			else
			{
				setoutp(o_GR1R);
				setoutp(o_GR2R);
				gripper_test = 0;
			}
			if ((rdoutp(o_GR1R) == 1) || 
				((rdoutp(o_LE) == 1) && (rdoutp(o_LE1) == 1) && (rdoutp(o_GR2R) == 1)))
				setinp(i_GRT1);
			else
				clrinp(i_GRT1);

			if ((rdoutp(o_LE) == 1) ||
			    ((rdoutp(o_GR1R) == 1) && (rdoutp(o_LE1) == 1) && (rdoutp(o_GR2R) == 1)))
				setinp(i_GRT2);
			else
				clrinp(i_GRT2);

			if ((rdoutp(o_GR1R) == 1) && (rdoutp(o_GR2R) == 1))
				setinp(i_GTS);
			else
				clrinp(i_GTS);
  	#endif		
#endif
			if ((i < 3) || (i > 10))
				i = 0;
			init_flight_times((int16)i);
			clear_call_data();
			freeze = 0;
			break;
		case 6:		// CCI - Set Car Call
			i = get_num(3);
			set_carcall(cons[carnmb],(int16)i);
//			Call_Pointer_X = cons[carnmb];
//			Call_Pointer_Y = i;
			freeze = 0;
			break;
		case 7:		// CCC - Clear Car Call
			i = get_num(3);
			if ((i >= 1) && (i <= cons[grtopf]))   // allow to cancel any floor
			{
				clr_both_inps(i_CC(i),i_XCC(i));
				clr_both_outps(o_CCL(i),o_XCCL(i)); 
			    carcb[i] = 0;
			}
			break;
		case 8:		// UCI - Set Up Call
			i = get_num(3);
			set_upcall((int16)i);
//			Call_Pointer_X = UpHC_PosInd;
//			Call_Pointer_Y = i;
			freeze = 0;
			break;
		case 9:		// UCC - Clear Up Call
			i = get_num(3);
			if ((i >= 1) && (i <= cons[grtopf]))
			{
				fl_ix = ((int16)i-1)/32;
#if (GALaxy_4 == 0)
				if (gc_hc_dev == 0)
				{
					clrinp(i_UC(i));
					clroutp(o_UCL(i)); 
				}
				else
#endif
					gc_uc_canc[fl_ix] |= fl_mask[i];

				gc_uc[fl_ix] &= ~fl_mask[i];
				g_up_hall_call[fl_ix][0] &= ~fl_mask[i];
				g_asign_up[i] = 0;
			}
			break;
		case 10:		// DCI - Set Down Call
			i = get_num(3);
			set_dncall((int16)i);
//			Call_Pointer_X = DnHC_PosInd;
//			Call_Pointer_Y = i;
			freeze = 0;
			break;
		case 11:		// DCC - Clear Down Call
			i = get_num(3);
			if ((i >= 1) && (i <= cons[grtopf]))
			{
				fl_ix = ((int16)i-1)/32;
#if (GALaxy_4 == 0)
				if (gc_hc_dev == 0)
				{
					clrinp(i_DC(i));
					clroutp(o_DCL(i)); 
				}
				else
#endif
					gc_dc_canc[fl_ix] |= fl_mask[i];

				gc_dc[fl_ix] &= ~fl_mask[i];
				g_dn_hall_call[fl_ix][0] &= ~fl_mask[i];
				g_asign_dn[i] = 0;
			}
			break;
		case 12:		// CAR - Set diagnostic screen to car# or group (0)
			i = get_num(3);
			if ((i == 0) || ((i >= 1) && (i <= cons[nmbcars])))
		    	Car_Select = (int16)i;
			Display_Trace = 0;
			Display_Fault = 0;
			// Set variables to exit to car screen
			dpy_mode = 1;
			echo = 0;
			Init_Io_Screen(Car_Select,Reset_State);		// Reset State variable
			Init_Grp_Io_Screen(Reset_State);		// Reset State Variable
			Init_Debug_Screen(Reset_State);
			grp_page = 0;
			freeze = 0;
			break;
		case 13:		// FRZ - Freeze the screen
			prev_echo = echo;
			echo = 0;
			freeze = 1;
			break;
		case 14:		// UFZ - UN-Freeze the screen
			echo = prev_echo;
			freeze = 0;
			break;
		case 15:		// PAG - Change diagnostic screen page
			if (Car_Select != 0)
			{
				freeze = 0;
			}
			break;
		case 16:			// IOS - IO Sim Data Set: IOS array0-78,bit=0-7 ex. IOS 22,0 sets bit
#if (Simulator == 1)
			k = get_numbers(3);
			if (k == 2)
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if (num[0] < 18)
				{
					ioset[num[0]] |= bittbl[num[1] & 0x07];
					ioclr[num[0]] |= bittbl[num[1] & 0x07];
				}
				else if (num[0] < 138)
				{
					mrc_ioset[num[0]-18] |= bittbl[num[1] & 0x07];
					mrc_ioclr[num[0]-18] |= bittbl[num[1] & 0x07];
				}
				else if (num[0] < 258)
				{
					ctc_ioset[num[0]-138] |= bittbl[num[1] & 0x07];
					ctc_ioclr[num[0]-138] |= bittbl[num[1] & 0x07];
				}
					
	#else
				if (num[0] <= 80)
				{
					ioset[num[0]] |= bittbl[num[1] & 0x07];
					ioclr[num[0]] |= bittbl[num[1] & 0x07];
				}
	#endif
			}
			freeze = 0;
#endif			
			break;
		case 17:	   // FLT - Display car faults
			i = get_num(3);
#if (Simulator == 0)
			if ((i >= 1) && (i <= cons[nmbcars]))
			{
		    	Car_Select = (int16)i;
				// Set variables to exit to car screen
				dpy_mode = 1;
				echo = 0;
				if (Display_Fault == 0)
				{
					Fault_Screen(Car_Select,Reset_State);
					Rotate_Fault = 0;
				}
				else
					Rotate_Fault = 1;
				Init_Debug_Screen(Reset_State);
				Display_Trace = 0;
				Display_Fault = 1;
				freeze = 0;
			}
#else
			if (i == 0)
			{		// Run Fault Test
				i = 1;
			  	while(i <= max_flt_ix)
			  	{
					record_fault((int16)i++);
			  	}
			}
			else
			{
				if(i <= max_nmbflts)
				{
					if (i == f_MRCAN_device_fault) 
					{
						fault_device = c_mrcan_SPB;
						fault_prog_flag1 = 3; 	  // UTS speed fault
						fault_prog_flag2 = 1; 	  // 1 occurance
						
					}
					else if (i == f_CTCAN_device_fault) 
					{
						fault_device = c_ctcan_SEL;
						fault_prog_flag1 = 49; 	  // UN pulse fault
						fault_prog_flag2 = 8; 	  // 8 occurance
					}
					else if (i == f_GRCAN_device_fault)
					{
						fault_device = c_grcan_SEB_1;
						fault_prog_flag1 = 13;		// comm fault 
						fault_prog_flag2 = 3; 	  // 3 occurance
					}
					else if (i == f_MRCAN_device_reset)
					{
						fault_device = c_mrcan_NTS;
						fault_prog_flag1 = 0; 	  
						fault_prog_flag2 = 0; 	  
					}
					else if (i == f_CTCAN_device_reset)
					{
						fault_device = c_ctcan_SEL;
						fault_prog_flag1 = 0; 	  
						fault_prog_flag2 = 0; 	  
					}
					else if (i == f_GRCAN_device_reset)
					{
						fault_device = c_grcan_SEB_1;
						fault_prog_flag1 = 0;		 
						fault_prog_flag2 = 0; 	  
					}
					else
					{
						fault_device = 0;
						fault_prog_flag1 = 0;	
						fault_prog_flag2 = 0; 
					}
					record_fault((int16)i);
				}
			}
#endif
			break;
		case 18:		// DUM - Dump data from MTR variable  DUM (0-2),(mtr 0-9),len
			k = get_numbers(3);
			if ((k == 2) || (k == 3))
			{			  // must have two values
				if ((num[0] < 2) && (num[1] < 10))
				{
					dump_num[num[0]] = num[1];		// num[0] == dump index, num[1] == trace number
					if(k==2)
						dump_len[num[0]] = 70;			// default to max
					else
						dump_len[num[0]] = num[2];
					dump_status[num[0]] = 1;
				}
			}		
			break;
		case 19:		// TST - Door limit Test AND fault test
			i = get_num(3);
			if (i == 0)
			{
#if (Simulator == 1)
				dcl_test = 0;
				dol_test = 0;
				dc_test = 0;
				sim_stall = 0;
				sim_enc_dir = 0;
				sim_SEL_test = 0;
				sim_tract_slip = 0;
				sim_ul_dl_swap = 0;
#endif
			}
#if (Simulator == 1)
			else if (i == 1)
				dcl_test = 1;
			else if (i == 2)
				dol_test = 1;
			else if (i == 3)
				dc_test = 1;
			else if (i==4)
			{
				sim_tract_slip = 1;
			}
			else if (i == 5)
			{
				setinp(i_DLM);		// Turn on door lock middle
				setinp(i_DLM_1);
			}
			else if (i == 6)
				sim_DPP_Count = S_DPP_Floor_Pos[sim_pos] - (int32)(2.0 * Pulses_per_Inch);
			else if (i == 7)
				sim_DPP_Count = S_DPP_Floor_Pos[sim_pos] - (int32)(6.0 * Pulses_per_Inch);
			else if (i == 8)
			{
				sim_DPP_Count = S_DPP_Floor_Pos[sim_pos] + (int32)(2.0 * Pulses_per_Inch);
//				sim_stall = 1;
			}
			else if (i == 9)
			{
				sim_DPP_Count = S_DPP_Floor_Pos[sim_pos] + (int32)(6.0 * Pulses_per_Inch);
//				sim_stall = 1;
			}
			else if (i == 10)
				sim_stall = 1;
			else if (i==11)
			{
				SPB_can.eep_req = 1;
			}
			else if (i==12)
			{
				if (SPB_can.eep_rcvd == 1)
				{
					vratputf(trascn,7,3,WHITE,"0=%3i,1=%3i,2=%3i,3=%3i,4=%3i",SPB_can.eeprom[0],SPB_can.eeprom[1],SPB_can.eeprom[2],SPB_can.eeprom[3],SPB_can.eeprom[4]);
					vratputf(trascn,8,3,WHITE,"5=%3i,6=%3i,7=%3i,8=%3i,9=%3i",SPB_can.eeprom[5],SPB_can.eeprom[6],SPB_can.eeprom[7],SPB_can.eeprom[8],SPB_can.eeprom[9]);
				}
			}
			else if (i==13)
			{
				if (SPB_can.eep_rcvd == 1)
				{
					vratputf(trascn,7,3,WHITE,"10=%3i,1=%3i,2=%3i,3=%3i,4=%3i",SPB_can.eeprom[10],SPB_can.eeprom[11],SPB_can.eeprom[12],SPB_can.eeprom[13],SPB_can.eeprom[14]);
					vratputf(trascn,8,3,WHITE,"5=%3i,6=%3i,7=%3i,8=%3i,9=%3i",SPB_can.eeprom[15],SPB_can.eeprom[16],SPB_can.eeprom[17],SPB_can.eeprom[18],SPB_can.eeprom[19]);
				}
			}
			else if (i==14)
			{
				if (SPB_can.eep_rcvd == 1)
				{
					vratputf(trascn,7,3,WHITE,"20=%3i,1=%3i,2=%3i,3=%3i,4=%3i",SPB_can.eeprom[20],SPB_can.eeprom[21],SPB_can.eeprom[22],SPB_can.eeprom[23],SPB_can.eeprom[24]);
					vratputf(trascn,8,3,WHITE,"5=%3i,6=%3i,7=%3i,8=%3i,9=%3i",SPB_can.eeprom[25],SPB_can.eeprom[26],SPB_can.eeprom[27],SPB_can.eeprom[28],SPB_can.eeprom[29]);
				}
			}
			else if (i == 19)
				rdfvar();
			else if (i == 20)
				record_fault(f_hcb_fet_short_up);
			else if (i == 21)
				put_pkt_status(3,2,DRVCOM);
			else if (i == 22)
				sim_enc_dir = 1;
			else if (i == 23)
				sim_enc_dir = 2;
			else if (i == 24)
				sim_SEL_test = 1;
			else if (i == 26)
			{
				L1_Voltage = 2401;
				L2_Voltage = 2382;
				L3_Voltage = 2393;
				Door_Voltage = 2084;
			}
			else if (i == 27)
			{
				L1_Voltage = 0;
				L2_Voltage = 0;
				L3_Voltage = 0;
				Door_Voltage = 0;
			}
			else if (i == 28)
				sim_ul_dl_swap = 1;
#endif
			else if (i == 25)
			{
				debug1 = 0;
				debug2 = 0;
				debug3 = 0;
				debug4 = 0;
				debug5 = 0;
				debug6 = 0;
				debug7 = 0;
				debug8 = 0;
				//writeTrafficLogToSD();
			}
			else if (i == 29)
			{
			/*     //THIS SECTION WAS ADDED TO PRESET THE COUNT OF THE OPEN CAN ENCODER TO 1000 WHILE TESTING DEVICE
					DPP_Count = 1000;
					preset_encoder_count(DPP_Count);
					Prev_DPP_Count = DPP_Count;
					prev_Enc_Count = DPP_Count;	
					old_dpp = DPP_Count;
			*/
			}
#if (Simulator == 1)
			else if(i == 30)
			{
				g_ids_fsx = 1;
				
			}
			else if(i == 31)
			{
			 	g_ids_fsx = 0;
			}
#endif
			else 
			{
#if (Simulator == 1)
				dcl_test = 0;
				dol_test = 0;
				dc_test = 0;
	  			sim_enc_dir = 0;
				sim_stall = 0;
				sim_SEL_test = 0;
				sim_tract_slip = 0;
				sim_ul_dl_swap = 0;
#endif
			}
			break;
		case 20:			// IOC - IO Sim Data Clear: IOC array0-78,bit=0-7 ex. IOC 22,0 Clears bit
#if (Simulator == 1)
			k = get_numbers(3);
			if (k == 2)
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if (num[0] < 18)
				{
					ioset[num[0]] &= ~bittbl[num[1] & 0x07];
					ioclr[num[0]] &= ~bittbl[num[1] & 0x07];
				}
				else if (num[0] < 138)
				{
					mrc_ioset[num[0]-18] &= ~bittbl[num[1] & 0x07];
					mrc_ioclr[num[0]-18] &= ~bittbl[num[1] & 0x07];
				}
				else if (num[0] < 258)
				{
					ctc_ioset[num[0]-138] &= ~bittbl[num[1] & 0x07];
					ctc_ioclr[num[0]-138] &= ~bittbl[num[1] & 0x07];
				}
					
	#else
				if (num[0] <= 80)
				{ 
					ioclr[num[0]] &= ~bittbl[num[1] & 0x07];
					ioset[num[0]] &= ~bittbl[num[1] & 0x07];
				}
	#endif
			}
			
			freeze = 0;
#endif
			break;
		case 21:			// RIO - Reset IO: RIO array0-78,bit=0-7 ex. RIO 22,0 Resets bit to normal state
#if (Simulator == 1)			
			k = get_numbers(3);
			if (k == 2)
			{			  // must have two values
				if (num[0] <= 80)
				{ 
					ioclr[num[0]] |= bittbl[num[1] & 0x07];
					ioset[num[0]] &= ~bittbl[num[1] & 0x07];
				}
			}
			
			freeze = 0;
#endif
			break;
		case 22:			// RAI - Reset all IO
#if (Simulator == 1)
			for (i=0;i<nmb_io_arrays;i++)
			{
				ioclr[i] = 0xFF;
				ioset[i] = 0;
			}
		#if (GALaxy_4 == 1)
			for (i=0;i<c_max_ctcan_ser_array;i++)
			{
				ctc_ioclr[i] = 0xff;
				ctc_ioset[i] = 0;
			}
			for (i=0;i<c_max_mrcan_ser_array;i++)
			{
				mrc_ioclr[i] = 0xff;
				mrc_ioset[i] = 0;
			}
			for (i=0;i<c_max_grcan_ser_array;i++)
			{
				grc_ioclr[i] = 0xff;
				grc_ioset[i] = 0;
			}
		#else

			for (i=0;i<c_max_ser_addr;i++)
			{
				cioclr[i] = 0xff;
				cioset[i] = 0;
			}
		#endif

#endif
			break;
		case 23:			// MEN - Return to menu
			if (Dispatcher == cons[carnmb])
			{
				init_menu_screen(Reset_State);
				dpy_mode = 0;
			}
			break;
		case 24:		// GRP - Set diagnostic screen to group 
	    	Car_Select = 0;
			Display_Trace = 0;
			Display_Fault = 0;
			// Set variables to exit to car screen
			dpy_mode = 1;
			echo = 0;
			Init_Io_Screen(Car_Select,Reset_State);		// Reset State variable
			Init_Grp_Io_Screen(Reset_State);		// Reset State Variable
			Init_Debug_Screen(Reset_State);
			Init_Group_Hall_Display(Reset_State);
			grp_page = 0;
			freeze = 0;
			break;
		case 25:		// HAL - Set diagnostic screen HALL CALL I/O
#if (GALaxy_4 == 1)
			if((gc_hall_io_dev != 0) || (nmb_grcan_dev != 0))	   // hall call comm
#else
			if(gc_hall_io_dev != 0)	   // hall call comm
#endif
			{
				Display_Trace = 0;
				Display_Fault = 0;
				echo = 0;
				dpy_mode = 1;
				Init_Io_Screen(Car_Select,Reset_State);		// Reset State variable
				Init_Group_Hall_Display(Reset_State);
				Init_Debug_Screen(Reset_State);
				Init_Grp_Io_Screen(Reset_State);
				Car_Select = 0;
				grp_page = 1;
				freeze = 0;
			}
			break;
		case 26:		 // CAN - Initialize debug mode and display CAN data.
			dpy_mode = 4;
			dbg_page = 1;
			Init_Debug_Screen(Reset_State);
			freeze = 0;
			echo = 1;
			row = 9;
			col = 2;
			vratputf(trascn,row,col,WHITE,">                                                  ");
			break;
		case 27:			// CIS - CAN IO Sim Data Set: CIS brd no, bit   CIS 19,2<CR> sets bit
#if (Simulator == 1)						   // Look up brd no from HMMAP.H i.e. BRD14cB = 19
			k = get_numbers(3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 0)
				if ((num[0] < c_max_ser_addr) && (num[1] <= 7))
				{	 // valid device numbers
					cioset[num[0]] |= bittbl[num[1] & 0x07];
					cioclr[num[0]] |= bittbl[num[1] & 0x07];
				}
	#else
				if ((num[0] < c_max_ctcan_ser_array) && (num[1] <= 7))
				{	 // valid device numbers
					ctc_ioset[num[0]] |= bittbl[num[1] & 0x07];
					ctc_ioclr[num[0]] |= bittbl[num[1] & 0x07];
				}
	#endif
			}
			freeze = 0;
#endif			
			break;
		case 28:			// CIC - CAN IO Sim Data Clear: CIC brd no, bit   CIC 19,2<CR> sets bit
#if (Simulator == 1)					  // Look up brd no from HMMAP.H i.e. BRD14cB = 19
			k = get_numbers(3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 0)
				if ((num[0] < c_max_ser_addr) && (num[1] <= 7))
				{	 // valid device numbers
					cioclr[num[0]] &= ~bittbl[num[1] & 0x07];
					cioset[num[0]] &= ~bittbl[num[1] & 0x07];
				}
	#else
				if ((num[0] < c_max_ctcan_ser_array) && (num[1] <= 7))
				{	 // valid device numbers
					ctc_ioclr[num[0]] &= ~bittbl[num[1] & 0x07];
					ctc_ioset[num[0]] &= ~bittbl[num[1] & 0x07];
				}
	#endif
			}
			
			freeze = 0;
#endif
			break;
		case 29:			// MEM memory dump command = MEM type, page, offset, len,  MEM 0,0,1234,100 <CR> Page 0 for ram/ee, 1 for nvram
			if ((procf < 3) || (procf > 11))
			{
				k = get_numbers(3);
				if (k >= 3)
				{		// Must have at least 3 numbers
					if ((num[0] >= 0) && (num[0] <= 3))
					{
						mem_type = (int16)num[0];
						if ((num[1] >= 0) && (num[1] < 4))
						{
							mem_page = (int16)num[1];
							mem_offset = (int16)num[2];
							if (k == 4)
								mem_len = (int16)num[3];
							else
								mem_len = 40;
							Mem_dump_screen(Reset_State);
							dpy_mode = 5;
						}
					}
				}
			}
			break;
		case 30:			// FIL fil memory command - FIL type, page, offset, len, data FIL 1,1,2000,100,255 <cr>
			if ((procf < 3) || (procf > 11))
			{
				k = get_numbers(3);
				if (k >= 3)
				{		// Must have at least 3 numbers
					if ((num[0] >= 0) && (num[0] <= 3))
					{
						if (num[0] > 1)
							mem_type = (int16)num[0] - 2;
						else
							mem_type = (int16)num[0];
						if ((num[1] >= 0) && (num[1] < 4))
						{
							mem_page = (int16)num[1];
							mem_offset = (int16)num[2];
							if (k >= 4)
								mem_len = (int16)num[3];
							else
								mem_len = 40;
							if (k == 5) 
							{
								if (num[4] > 255)
									num[4] = 255;
							}
							else
								num[4] = 0;
							j = 0;
							for(i=0;i<mem_len;i++)
							{
								if (num[0] > 1)
								{
									wr_mram(mem_page,(int16)(mem_offset + i),(uint16)(mem_pattern[j & 0x07]));
									j++;
								}
								else
									wr_mram(mem_page,(int16)(mem_offset + i),(uint16)num[4]);
							}
						}
					}
				}
			}
			break;
		case 31:			// INP - Input from i/o address INP 255<CR>
			freeze = 0;
			break;
		case 32:			// OUT - Output to i/o address OUT 124,255<cr>
			freeze = 0;
			break;
		case 33:		// RCI - Set Rear Car Call
			i = get_num(3);
			set_rcarcall(cons[carnmb],(int16)i);
//			Call_Pointer_X = cons[carnmb];
//			Call_Pointer_Y = i;
			freeze = 0;
			break;
		case 34:		// RUI - Set Rear Up Call
			i = get_num(3);
			set_rupcall((int16)i);
//			Call_Pointer_X = UpHC_PosInd;
//			Call_Pointer_Y = i;
			freeze = 0;
			break;
		case 35:		// RDI - Set Rear Down Call
			i = get_num(3);
			set_rdncall((int16)i);
//			Call_Pointer_X = DnHC_PosInd;
//			Call_Pointer_Y = i;
			freeze = 0;
			break;
		case 36:		// FDL - 1 = 0PEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = get_num(3);
			sim_dl_open = 0;
			sim_dl_close = 0;
			if (i == 1)
				sim_dl_open = 1;
			if (i == 2)
				sim_dl_close = 1;
#endif
			break;
		case 37:		// FGS - 1 = OPEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = get_num(3);
			sim_gs_open = 0;
			sim_gs_close = 0;
			if (i == 1)
				sim_gs_open = 1;
			if (i == 2)
				sim_gs_close = 1;
#endif
			break;
		case 38:		// RDL - 1 = 0PEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = get_num(3);
			sim_dlr_open = 0;
			sim_dlr_close = 0;
			if (i == 1)
				sim_dlr_open = 1;
			if (i == 2)
				sim_dlr_close = 1;
#endif
			break;
		case 39:		// RGS - 1 = OPEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = get_num(3);
			sim_gsr_open = 0;
			sim_gsr_close = 0;
			if (i == 1)
				sim_gsr_open = 1;
			if (i == 2)
				sim_gsr_close = 1;
#endif
			break;
		case 40:		// FDC - 1 = 0PEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = get_num(3);
			sim_dc_open = 0;
			sim_dc_close = 0;
			if (i == 1)
				sim_dc_open = 1;
			if (i == 2)
				sim_dc_close = 1;
#endif
			break;
		case 41:		// CMR: Can Message Request CMD dev,msg
#if (GALaxy_4 == 0)
			k = get_numbers(3);
			if (k >= 2)
			{
				if ((num[0] >= c_can_START) && (num[0] <= c_can_TOC) && (num[1] != 0))
				{
					can_dev_req = (int16)num[0];
					can_msg_req = (int16)num[1];
					can_msg_cmd = 2;
					can_msg_cnt = 1;
				}
			}
			else if (k == 1)
			{
				if (num[0] <= 49)
				{
					can_dev[c_can_SEB_18 - c_can_START].msg_req = (int8)num[0]; 
					if (num[0] == 47)
					{
						can_dev[c_can_SEB_18 - c_can_START].label_ix = 0;
						can_dev[c_can_SEB_18 - c_can_START].label_req = 1;
					}
				}
			}
#endif
			break;
		case 42:			// IOT - IO Sim Data Toggle: IOT array0-78,bit=0-7 IOT 13,1: IOT tottles, 
#if (Simulator == 1)
			k = get_numbers(3);
	#if (GALaxy_4 == 0)
			if (k == 2)
			{			  // must have two values
				if ((num[0] <= 80) && (num[1] <= 7))
				{ 
					toggle_array = (int16)num[0];
					toggle_bit = (int16)num[1];
					toggle_state = 0;
				}
			}
			else
			{			
				if (toggle_state == 0)
				{
					toggle_state = 1;
					ioset[toggle_array] |= bittbl[toggle_bit & 0x07];
					ioclr[toggle_array] |= bittbl[toggle_bit & 0x07];
				}
				else
				{
					toggle_state = 0;
					ioclr[toggle_array] &= ~bittbl[toggle_bit & 0x07];
					ioset[toggle_array] &= ~bittbl[toggle_bit & 0x07];
				}
			}
	#else
			if (k == 2)
			{			  // must have two values
				if ((num[0] <= 258) && (num[1] <= 7))
				{ 
					toggle_array = (int16)num[0];
					toggle_bit = (int16)num[1];
					toggle_state = 0;
				}
			}
			else
			{			
				if (toggle_state == 0)
				{
					toggle_state = 1;
					if (toggle_array < 18)
					{
						ioset[toggle_array] |= bittbl[toggle_bit & 0x07];
						ioclr[toggle_array] |= bittbl[toggle_bit & 0x07];
					}
					else if (toggle_array < 138)
					{
						ctc_ioset[toggle_array-18] |= bittbl[toggle_bit & 0x07];
						ctc_ioclr[toggle_array-18] |= bittbl[toggle_bit & 0x07];
					}
					else if (toggle_array < 258)
					{
						mrc_ioset[toggle_array-138] |= bittbl[toggle_bit & 0x07];
						mrc_ioclr[toggle_array-138] |= bittbl[toggle_bit & 0x07];
					}
				}
				else
				{
					toggle_state = 0;
					if (toggle_array < 18)
					{
						ioclr[toggle_array] &= ~bittbl[toggle_bit & 0x07];
						ioset[toggle_array] &= ~bittbl[toggle_bit & 0x07];
					}
					else if (toggle_array < 138)
					{
						ctc_ioset[toggle_array-18] &= ~bittbl[toggle_bit & 0x07];
						ctc_ioclr[toggle_array-18] &= ~bittbl[toggle_bit & 0x07];
					}
					else if (toggle_array < 258)
					{
						mrc_ioset[toggle_array-138] &= ~bittbl[toggle_bit & 0x07];
						mrc_ioclr[toggle_array-138] &= ~bittbl[toggle_bit & 0x07];
					}
				}
			}
	#endif
			freeze = 0;
#endif
			break;
		case 43:			// STI - STart Integer Memory Offset
#if (Simulator == 1)
			k = get_numbers(3);
			if (k == 1)
			{			  // must have one number
				var_off = num[0];
			}
			else
				vratputf(trascn,8,6,WHITE,"STI=%0x",var_off);

			freeze = 0;
#endif
			break;
		case 44:			// CIT - IO Sim Data Toggle: CIT array0-78,bit=0-7 IOT 13,1: CIT tottles, 
#if (Simulator == 1)
			k = get_numbers(3);
	#if (GALaxy_4 == 0)
			if (k == 2)
			{			  // must have two values
				if ((num[0] < 80) && (num[1] <= 7))
				{	 // valid device numbers
					toggle_array = (int16)(num[0]);
					toggle_bit = (int16)(num[1]);
					toggle_state = 0;
				}
			}
			else
			{			
				if (toggle_state == 0)
				{
					toggle_state = 1;
					cioset[toggle_array] |= bittbl[toggle_bit & 0x07];
					cioclr[toggle_array] |= bittbl[toggle_bit & 0x07];
				}
				else
				{
					toggle_state = 0;
					cioclr[toggle_array] &= ~bittbl[toggle_bit & 0x07];
					cioset[toggle_array] &= ~bittbl[toggle_bit & 0x07];
				}
			}
	#else
			if (k == 2)
			{			  // must have two values
				if ((num[0] < c_max_ctcan_ser_array) && (num[1] <= 7))
				{	 // valid device numbers
					toggle_array = (int16)(num[0]+18);
					toggle_bit = (int16)(num[1]+18);
					toggle_state = 0;
				}
			}
			else
			{			
				if ((toggle_array >= 18) && (toggle_array < 138))
				{
					if (toggle_state == 0)
					{
						toggle_state = 1;

						ctc_ioset[toggle_array-18] |= bittbl[toggle_bit & 0x07];
						ctc_ioclr[toggle_array-18] |= bittbl[toggle_bit & 0x07];
					}
					else
					{
						toggle_state = 0;
						ctc_ioclr[toggle_array-18] &= ~bittbl[toggle_bit & 0x07];
						ctc_ioset[toggle_array-18] &= ~bittbl[toggle_bit & 0x07];
					}
				}
			}
	#endif
			freeze = 0;
#endif
			break;

#if (Simulator == 1)

			case 45:		// RPU  Reset Pulse Update Trace
#if(Traction == 1)
				for(i=0;i<fl_size;i++)
					DPP_updf[i] = 0;
#endif
			break;
#endif
#if (GALaxy_4 == 0)
		case 46:			// FDP - Front Door Parameter Request: FDP 1343<ENT> OR FDP 1343,1333<ENT>
			k = get_hex_numbers(3);
			if (k != 0)
			{
				DOOR_can.par_add_req = (int8)num[0];
				can_dev[c_can_DOOR - c_can_START].msg_req = 30;
				if (k == 2)
				{
					DOOR_can.par_cmd |= b_door_par_wr;
					DOOR_can.par_dat_wrt = (int8)num[1];
				}
				else
				{
					DOOR_can.par_cmd &= ~b_door_par_wr;
					DOOR_can.par_dat_wrt = 0;
				}
			}
			break;

		case 47:			// RDP - Rear Door Parameter Request: RDP 1343<ENT> OR rDP 1343,1333<ENT>
			k = get_hex_numbers(3);
			if (k != 0)
			{
				RDOOR_can.par_add_req = (int8)num[0];
				can_dev[c_can_RDOOR - c_can_START].msg_req = 30;

				if (k == 2)
				{
					RDOOR_can.par_cmd |= b_door_par_wr;
					RDOOR_can.par_dat_wrt = (int8)num[1];
				}
				else
				{
					RDOOR_can.par_cmd &= ~b_door_par_wr;
					RDOOR_can.par_dat_wrt = 0;
				}
			}
			break;
		case 48:			// FDV - Front door version: FDV 1,1 <ENTER>
			k = get_hex_numbers(3);
			if (k != 0)
			{
				if (k == 2)
				{
				 	can_dev[c_can_DOOR-c_can_START].version = (int8)num[0];
					can_dev[c_can_DOOR-c_can_START].revision = (int8)num[1];
				}
				else
				{
				 	can_dev[c_can_DOOR-c_can_START].version = (int8)num[0];
					can_dev[c_can_DOOR-c_can_START].revision = 0;
				}
			}
			break;
		case 49:			// RDV - Front door version: RDV 1,1 <ENTER>
			k = get_hex_numbers(3);
			if (k != 0)
			{
				if (k == 2)
				{
				 	can_dev[c_can_RDOOR-c_can_START].version = (int8)num[0];
					can_dev[c_can_RDOOR-c_can_START].revision = (int8)num[1];
				}
				else
				{
				 	can_dev[c_can_RDOOR-c_can_START].version = (int8)num[0];
					can_dev[c_can_RDOOR-c_can_START].revision = 0;
				}
			}
			break;
#endif
		case 50:			// FDS - Front Door STATUSt: FDS 1343<ENT> 
			k = get_hex_numbers(3);
			if (k != 0)
			{
				DOOR_can.status_ret = (int8)num[0];
				DOOR_can.reflect_data = 1;
			}
			break;
		
		case 51:			// DVP - Drive Parameter Request: DVP 1343<ENT> OR DVP 1343,1333<ENT>
#if ((Traction == 1) || (Tract_OL == 1))
/*			k = get_numbers(3);			delete KEB parameters
			if (((Drive_Type[cons[carnmb]] == 12) && (fvars[fvkebdpytype] == 0)) || (Drive_Type[cons[carnmb]] == 14))
			{
				if ((num[0] <= grp_par_max[drv_par_group]) && (drv_par_group >= 0) && (drv_par_group <= 13))
				{
					if (Drive_Type[cons[carnmb]] == 12)
					{
						if (k == 2)
						{		 // Write drive parameter
							drv_wpar_data = (int16)num[1];
							drv_wpar_addr = keb_drv_par_tbl[drv_par_group].par[num[0] & 0x00ff].address;
							drv_wpar_set = 	keb_drv_par_tbl[drv_par_group].par[num[0] & 0x00ff].set;
							put_pkt_req('g',2,DRVCOM);
						}
						else
						{	  // Read drive parameter
							tst_drv_par_ret = (int16)num[0];
							drv_rpar_addr = keb_drv_par_tbl[drv_par_group].par[num[0] & 0x00ff].address;;
							drv_rpar_set = 	keb_drv_par_tbl[drv_par_group].par[num[0] & 0x00ff].set;
							put_pkt_req('G',2,DRVCOM);
						}
					}
					else if (Drive_Type[cons[carnmb]] == 14)
					{
						if (k == 2)
						{		 // Write drive parameter
							t2.ib[1] = (int8)drv_par_group;
							t2.ib[0] = (int8)(num[0] & 0x00ff);
							drv_wpar_addr = t2.ii;
							drv_wpar_data = (int16)num[1];
							put_pkt_req(6,2,DRVCOM);
						}
						else
						{	  // Read drive parameter
							t2.ib[1] = (int8)drv_par_group;
							t2.ib[0] = (int8)(num[0] & 0x00ff);
							drv_rpar_addr = t2.ii;
							put_pkt_req(3,2,DRVCOM);
						}
					}
				}
			}
			else if (Drive_Type[cons[carnmb]] == 15)
			{
				if (num[0] <= 0x1000)
				{
					if (k == 2)
					{		 // Write drive parameter
						drv_wpar_addr = (int16)num[0];
						drv_wpar_data = (int16)num[1];
						put_pkt_req(6,2,DRVCOM);
					}
					else
					{	  // Read drive parameter
						drv_rpar_addr = (int16)num[0];
						put_pkt_req(3,2,DRVCOM);
					}
				}
			}
			else 
			{
				if (num[0] <= 204)
				{
					if (k == 2)
					{		 // Write drive parameter
						t2.ib[1] = 0;
						t2.ib[0] = (int8)(num[0] & 0x00ff);
						drv_wpar_addr = t2.ii;
						drv_wpar_data = (int16)num[1];
						put_pkt_req(0x43,2,DRVCOM);
					}
					else
					{	  // Read drive parameter
						t2.ib[1] = 0;
						t2.ib[0] = (int8)(num[0] & 0x00ff);
						drv_rpar_addr = t2.ii;
						put_pkt_req(0x61,2,DRVCOM);
					}
				}
			}
			*/

#endif
			break;
		case 52:			// DPG - Drive Parameter Group: DPG 01<ENT>
#if ((Traction == 1) || (Tract_OL == 1))
			i = get_num(3);
			if ((i >= 0) && (i<=13))
				drv_par_group = (int16)i;
#endif
			break;

		case 53:			// COD = floor number to fill code for entire floor table
#if (Simulator == 1)
			k = get_numbers(3);
			if ((k > 0) && (k <= 2))
			{
				if ((num[0] > 0) && (num[0] <= cons[topf]))
				{			  // must have two values
					
					if (k == 2)
					{
						if ((num[1] <= 0) || (num[1] > NumMaxCd))
							num[1] = 1;
					}
					else
						num[1] = NumMaxCd;

					Read_CodeTbl((int16)num[0],&CodeTable, LookUpTable);
					CodeTable.NumCode = 0;
					//initialize the  code digits 
					Code_Num[0] = 1;
					Code_Num[1] = 1;
					Code_Num[2] = 1;
					Code_Num[3] = 1;
					for (i=0;i<num[1];i++)
					{
						CodeTable.NumCode++;
						
						CodeTable.Codes[i][0] = Code_Num[0];
						CodeTable.Codes[i][1] = Code_Num[1];
						CodeTable.Codes[i][2] = Code_Num[2];
						CodeTable.Codes[i][3] = Code_Num[3];
						Code_Num[3]++;
						if (Code_Num[3] > 4)
						{
							Code_Num[3] = 1;
							Code_Num[2]++;
							if (Code_Num[2] > 4)
							{
								Code_Num[2] = 1;
								Code_Num[1]++;
								if (Code_Num[1] > 4)
								{
									Code_Num[1] = 1;
									Code_Num[0]++;
									if (Code_Num[0] > cons[grtopf])
										Code_Num[0] = 1;
								}
							}
						}
					}
					Write_CodeTbl((int16)num[0],&CodeTable, LookUpTable);
				}
				else if (num[0] == 0)
				{
					for (k=1;k<=cons[grtopf];k++)
					{
						Read_CodeTbl(k,&CodeTable, LookUpTable);
						CodeTable.NumCode = 0;
						//initialize the  code digits 
						Code_Num[0] = 1;
						Code_Num[1] = 1;
						Code_Num[2] = 1;
						Code_Num[3] = 1;
						if (k == 2)
						{
							if ((num[1] <= 0) || (num[1] > NumMaxCd))
								num[1] = 1;
						}
						else
							num[1] = NumMaxCd;
						for (i=0;i<num[1];i++)
						{
							CodeTable.NumCode++;
							
							CodeTable.Codes[i][0] = Code_Num[0];
							CodeTable.Codes[i][1] = Code_Num[1];
							CodeTable.Codes[i][2] = Code_Num[2];
							CodeTable.Codes[i][3] = Code_Num[3];
							Code_Num[3]++;
							if (Code_Num[3] > 4)
							{
								Code_Num[3] = 1;
								Code_Num[2]++;
								if (Code_Num[2] > 4)
								{
									Code_Num[2] = 1;
									Code_Num[1]++;
									if (Code_Num[1] > 4)
									{
										Code_Num[1] = 1;
										Code_Num[0]++;
										if (Code_Num[0] > cons[grtopf])
											Code_Num[0] = 1;
									}
								}
							}
						}
						Write_CodeTbl(k,&CodeTable, LookUpTable);
					}
				}
			}
#endif
			break;


		case 54:			// RAV = reset all field variables to defaults
#if (Simulator == 1)
			reset_fvars();
#endif
			break;

		case 55:			// CAF = clear all faults in the group 
#if (Simulator == 1)
	      	for(i=0; i<max_flt_ix; i++)
	      	{
				f.Flts[i].code = 0;
				f.Flts[i].count = 0;
	      	}
			Flt_ix = 0;
		  	Up_Date_Flt(1);
	#if (GALaxy_4 == 1)
			clr_mrcan_dev_comm_loss();
			clr_ctcan_dev_comm_loss();
	#else
			clr_can_dev_comm_loss();
	#endif

			for(i=1;i<=cons[nmbcars];i++)
			{
				put_pkt_req(10,(int16)i,gc_port);
			}

#endif
			break;
		
		case 56:		// RLF = Reset Long Term Faults
			clr_LT_flts();
			break;

		case 57:			// MIS - CAN IO Sim Data Set: MIS brd no, bit   MIS 19,2<CR> sets bit
#if (Simulator == 1)						   // Look up brd no  
			k = get_numbers(3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if ((num[0] < c_max_mrcan_ser_array) && (num[1] <= 7))
				{	 // valid device numbers
					mrc_ioset[num[0]] |= bittbl[num[1] & 0x07];
					mrc_ioclr[num[0]] |= bittbl[num[1] & 0x07];
				}
	#endif
			}
			freeze = 0;
#endif			
			break;
		case 58:			// MIC - CAN IO Sim Data Clear: MIC brd no, bit   MIC 19,2<CR> sets bit
#if (Simulator == 1)					  // Look up brd no 
			k = get_numbers(3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if ((num[0] < c_max_mrcan_ser_array) && (num[1] <= 7))
				{	 // valid device numbers
					mrc_ioclr[num[0]] &= ~bittbl[num[1] & 0x07];
					mrc_ioset[num[0]] &= ~bittbl[num[1] & 0x07];
				}
	#endif
			}
			
			freeze = 0;
#endif
			break;
		case 59:			// MIT - IO Sim Data Toggle: MIT array0-78,bit=0-7 IOT 13,1: MIT toggles, 
#if ((Simulator == 1) && (GALaxy_4 == 1))
			k = get_numbers(3);
			if (k == 2)
			{			  // must have two values
				if ((num[0] < c_max_mrcan_ser_array) && (num[1] <= 7))
				{	 // valid device numbers
					toggle_array = (int16)(num[0]+138);
					toggle_bit = (int16)(num[1]+138);
					toggle_state = 0;
				}
			}
			else
			{			
				if ((toggle_array >= 138) && (toggle_array < 258))
				{
					if (toggle_state == 0)
					{
						toggle_state = 1;

						mrc_ioset[toggle_array-138] |= bittbl[toggle_bit & 0x07];
						mrc_ioclr[toggle_array-138] |= bittbl[toggle_bit & 0x07];
					}
					else
					{
						toggle_state = 0;
						mrc_ioclr[toggle_array-138] &= ~bittbl[toggle_bit & 0x07];
						mrc_ioset[toggle_array-138] &= ~bittbl[toggle_bit & 0x07];
					}
				}
			}
			freeze = 0;
#endif
			break;
			
		case 60: 			// ENC - Encoder test commands
			break;
			
		case 61:		// IRU = Inconspicious Riser Up Call
			i = get_num(3);
			set_ir_upcall((int16)i);
			break;

		case 62:		// IRD = Inconspicious Riser Down Call
			i = get_num(3);
			set_ir_dncall((int16)i);
			break;
			
 		case 63:			// SGP - Set Galcom Packet: SGP 0,31<ENT>
			k = get_numbers(3);
			if (k == 2)
			{
				if (num[0] < 2)
				{
					if (num[1] == 40)
					{
						com_rst_flt_ix[num[0]] = 1;
						com_car_req[num[0]] = cons[carnmb];
					}
					galcom_req[num[0]] = (int16)num[1];
				}
			}
			else if (k == 1)
			{
				if (num[0] < 2)
					galcom_req[num[0]] = 0;
			}
			break;

		case 64:		// CLC = Clear comm counters

			com_rx_char_cnt[0] = 0;
			good_packet[0] = 0;
			bad_packet[0] = 0;
			dma_start_cnt[0] = 0;
			dma_end_cnt[0] = 0;

			break;
			
		case 65:		//CFS
#if (Simulator == 1)
			firef = 0;
			old_firef = 0;
			rfiref = 0;
			old_rfiref = 0;			
#endif
			break;
						
		case 66:			// GIS - Group IO Sim Data Set: MIS brd no, bit   MIS 19,2<CR> sets bit
#if (Simulator == 1)						   // Look up brd no  
			k = get_numbers(3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if ((num[0] < c_max_grcan_ser_array) && (num[1] <= 7))
				{	 // valid device numbers
					grc_ioset[num[0]] |= bittbl[num[1] & 0x07];
					grc_ioclr[num[0]] |= bittbl[num[1] & 0x07];
				}
	#endif
			}
			freeze = 0;
#endif			
			break;
			
		case 67:			// GIC - Group IO Sim Data Clear: MIC brd no, bit   MIC 19,2<CR> sets bit
#if (Simulator == 1)					  // Look up brd no 
			k = get_numbers(3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if ((num[0] < c_max_grcan_ser_array) && (num[1] <= 7))
				{	 // valid device numbers
					grc_ioclr[num[0]] &= ~bittbl[num[1] & 0x07];
					grc_ioset[num[0]] &= ~bittbl[num[1] & 0x07];
				}
	#endif
			}
			
			freeze = 0;
#endif
			break;

		default:
			freeze = 0;
			break;
	}

	if ((dpy_mode == 4) && (echo == 1))
	{
		row = 9;
		col = 1;
		vratputf(trascn,row,col,WHITE,">_                                                 ");
		col = 2;
	}


}


// *****************************************************
//   This  routine  finds the location for first Nonspace
// ******************************************************
int16 LocNonSpace(int16 len)
{
	int16 i;
	int16 Loc=4;
	for(i=3;i<len;i++)
	{
	    if((kb_buff[i] | 0x20)!=' ')
		{
			Loc=i;
			break;
		}
    }
   return Loc;
}

// ****************************************
// This routine places numbers in num array
// ****************************************

int16 get_numbers(int16 i)
{				// i is the buffer start
	int16 j,k;
	j = i;
	k = 0;
	do {
		i++;
		if ((kb_buff[i] == ',') || (kb_buff[i] == CR))
		{
			kb_buff[i] = 0;
			num[k++] = atoi(&kb_buff[j]);
			j = i + 1;
		}
	} while ((kb_buff[i] != CR) && (i<=20));
	return (k);
}

int16 get_hex_numbers(int16 i)
{				// i is the buffer start
	int16 j,k;
	j = i;
	k = 0;
	do {
		i++;
		if ((kb_buff[i] == ',') || (kb_buff[i] == CR))
		{
			kb_buff[i] = 0;
			num[k++] = atoh(&kb_buff[j]);
			j = i + 1;
		}
	} while ((kb_buff[i] != CR) && (i<=20));
	return (k);
}

int32 get_num(int16 buf_st)
{
	int i,j;
	i = 0xFFFF;

	for (j=buf_st;j<=20;j++)
	{
		if (kb_buff[j] == CR)
		{
			kb_buff[j] = 0;
			i = atoi(&kb_buff[buf_st]); 
			return i;
		}
	}
	return i;
}

#define cmdsize 205
const char cmdtbl[cmdsize] =  {"   DBGEXIMTRCLRINICCICCCUCIUCCDCIDCC"	// 36
							  "CARFRZUFZPAGIOSFLTDUMTSTIOCRIORAIMEN"	// 72
							  "GRPHALCANCISCICMEMFILINPOUTRCIRUIRDI"	//108
							  "FDLFGSRDLRGSFDCCMRIOTSTICITRPUFDPRDP"	//144
							  "FDVRDVFDSDVPDPGCODRAVCAFRLFMISMICMIT"	//180
							  "ENCIRUIRDSGPCLCCFSGISGIC"};

int16 scan_cmd(void)
{
	int16 i,cmd;
	cmd = 0;
	for (i=0;i<(cmdsize-2);i+=3)
	{
		if ((kb_buff[0] == cmdtbl[i]) && 
		   (kb_buff[1] == cmdtbl[i+1]) && 
		   (kb_buff[2] == cmdtbl[i+2]))
		{		// Match upper or lower case (& 0xdf)
			cmd = i/3;
			break;
		}
	}
	return cmd;
}

//****************************************************
//********* Init Deubg Screen ************************
//****************************************************

	const char dbg_tbl_1 [10][11] = {
	"  servf = ",
	"  procf = ",
	"   empf = ",
	"   dirf = ",
	"  dpref = ",
	" dcalls = ",
	"    hsf = ",
	"  stepf = ",
	" nstopf = ",		
	"relevel = ",
	};

	const char dbg_tbl_2 [10][14] = {
	"     doorf = ",
	"    rdoorf = ",
	"     doort = ",
	"  closetry = ",
	" rclosetry = ",
	"    rdoort = ",
	"     firef = ",
	"fsl_effect = ",
	"FS_DOB_DIS = ",
	"fs2_to_fs1 = ",
	};

	const char dbg_tbl_3 [14][15] = {
	"    debug 1 = ",
	"    debug 2 = ",
	"    debug 3 = ",
	"    debug 4 = ",
	"    debug 5 = ",
	"    debug 6 = ",
	"    debug 7 = ",
	"    debug 8 = ",
	"Rx Char Cnt = ",
	"Good Pkt Cnt= ",
	"Bad Pkt Cnt = ",
	"DMA Strt Cnt= ",
	"DMA End Cnt = ",
	"io cmd data = ",
	};

	const char dbg_tbl_4 [10][13] = {
	" position = ",
	"DPP_Count = ",
	" Velocity = ",
	" Vel_Step = ",
	"  Vel_Max = ",
	"  Vel fpm = ",
	"DPP_Floor = ",
	"Pulses/in = ",
	"  sim_pos = ",
	"sim_DPP_C = ",
	};

	const char can_tbl[5][6][11] = {
	{
		" status = ",
		"   mode = ",
		"arb los = ",
		"err cod = ",
		"tx errc = ",
		"rx errc = ",
		},
	{
		" RI_cnt = ",
		" TI_cnt = ",
		"EWI_cnt = ",
		"DOI_cnt = ",
		"WUI_cnt = ",
		"EPI_cnt = ",
		},
	{
		"ALI_cnt = ",
		"BEI_cnt = ",
		"  DBG 1 = ",
		"  DBG 2 = ",
		"  DBG 3 = ",
		"  DBG 4 = ",
		},
	{
		"IR_Msk1 = ",
		"IR_Msk2 = ",
		"INTstat = ",
		" INTcnt = ",
		" ONLINE = ",
		"          ",
		},
	{
		"  error = ",
		" SELcnt = ",
		" Fl Pos = ",
		" Status = ",
		"          ",
		"          ",
		},
	};

	const char can_tbl_2 [4][11] = {
	" CAN TX = ",
	" CAN RX = ",
	"DEVICES = ",
	"VERSION = ",
	};

	const char drv_tbl_2 [4][11] = {
	" DRV TX = ",
	" DRV RX = ",
	"        = ",
	"        = ",
	};

	const char door_tbl[5][6][11] = {
	{
		" status = ",
		"   mode = ",
		"arb los = ",
		"err cod = ",
		"tx errc = ",
		"rx errc = ",
		},
	{
		"  FDcmd = ",
		"FDstat1 = ",
		"FDstat2 = ",
		"FDdpreq = ",
		"FDdpret = ",
		"FDfault = ",
		},
	{
		"FDaddwr = ",
		"FDparwr = ",
		"FDaddrd = ",
		"FDparrd = ",
		" FDvolt = ",
		" FDamps = ",
		},
	{
		"  RDcmd = ",
		"RDstat1 = ",
		"RDstat2 = ",
		"RDdpreq = ",
		"RDdpret = ",
		"RDfault = ",
		},
	{
		"RDaddwr = ",
		"RDparwr = ",
		"RDaddrd = ",
		"RDparrd = ",
		" RDvolt = ",
		" RDamps = ",
		},
	};
	const char drive_tbl[5][6][11] = {
	{
		"isource = ",
		"mstatus = ",
		"lstatus = ",
		" icount = ",
		" tx cnt = ",
		" rx cnt = ",
		},
	{
		"TxB ful = ",
		"TxB emp = ",
		"    tbf = ",
		"    tbs = ",
		"    rbf = ",
		"    rbs = ",
		},
	{
		"CmTxInt = ",
		"CmRxInt = ",
		" debug1 = ",
		" debug2 = ",
		" debug3 = ",
		" debug4 = ",
		},
	{
		"drv cmd = ",
		"drv vel = ",
		"dstatus = ",
		"enc spd = ",
		"voltage = ",
		"current = ",
		},
	{
		"Freq.Hz = ",
		"fltcode = ",
		"wparadd = ",
		"wpardat = ",
		"rparadd = ",
		"rpardat = ",
		},
	};



	const char grp_tbl [10][81] = {                                                     
	"Var              | Var [ix]        0,   1,   2,   3,   4,   5,   6,   7,   8,   ", 
	"NCU_Car    =       NCU_door[] =     ,    ,    ,    ,    ,    ,    ,    ,    ,   ", 
	"NCU_seq    =       NCU_que[]  =     ,    ,    ,    ,    ,    ,    ,    ,    ,   ", 
	"Lby_Req    =       Lby_req[]  =     ,    ,    ,    ,    ,    ,    ,    ,    ,   ", 
	"#L_Rcl_req =       g_park[]   =     ,    ,    ,    ,    ,    ,    ,    ,    ,   ", 
	"#C_at_lby  =       park_fl[]  =     ,    ,    ,    ,    ,    ,    ,    ,    ,   ", 
	"next_up    =       park_full[]=     ,    ,    ,    ,    ,    ,    ,    ,    ,   ", 
	"ncu_do_seq =       how_many[] =     ,    ,    ,    ,    ,    ,    ,    ,    ,   ", 

	"fdoor_rq=  X  rdoor_rq=  X  nudtst =  X  g_secf[]=      ,    ,    ,    ,    ,   ", 
	"fdoor_en=  X  rdoor_en=  X  rnudgst=  X  nmb_cars_in_svc=     all_cars_ots=     "	
	};

	const char igep_tbl[20][80] = {
	"    Variable Name               [0]  [1]  [2]  [3]  [4]  [5]  [6]  [7]  [8]   ", //0
	"      grp_empf         [0][] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //1
	"                       [1][] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //2
	"   car_recall_order    [0][] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //3
	"                       [1][] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //4
	"   car_select_order    [0][] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //5
	"                       [1][] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //6
	"      man_sel_req      [0][] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //7
	"                       [1][] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //8 
	"     power_feeder         [] =   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   Z,   ", //9
	"                                                                              ", //10
	"       Name         [0][1]|      Name         [0][1]|       Name        [0][1]", //11
	"ep_auto_recall_en[]= Z, Z | nmb_recall_cars[]= Z, Z |  cars_in_motion[]= Z, Z ", //12
	"ep_auto_select_en[]= Z, Z |   car_recall_en[]= Z, Z | em_pwr_2,ep_xfr_2= Z, Z ", //13
	"       ep_run_en []= Z, Z |      recall_seq[]= Z, Z |  car_fire_sw_on[]= Z, Z ", //14
	"      cars_moving[]= Z, Z |  cars_recalling[]= Z, Z | nmb_select_cars[]= Z, Z ", //15
	"       select_req[]= Z, Z |   recalled_cars[]= Z, Z |   car_select_en[]= Z, Z ", //16
	"  prev_select_req[]= Z, Z |  recall_attempt[]= Z, Z |      select_seq[]= Z, Z ", //17
	"      man_sel_seq[]= Z, Z | recall_finished[]= Z, Z |   cars_selected[]= Z, Z ", //18
	"                                                                              "  //19
	};																	  
	const char run_tbl [10][81] = {
	"Pos,pf,Tr,   DZ Cnt,  Pls Cnt, Cnt Diff,VelErCt,VelUpdCnt, Upd Diff, Car Vel,", 
	"  1, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,",
	"  2, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,",
	"  3, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,",
	"  4, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,",
	"  5, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,",
	"  6, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,",
	"  7, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,",
	"  8, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,",
	"  9, 0, 0, 00000000, 00000000, 00000000, 000000, 00000000, 00000000, 0000000,"
	};


void Init_Debug_Screen (int16 init_flag)
{
	int16 i;
	static int state;

	if (init_flag == 1)
	{		// reset state machine
		state = 0;
		echo = 0;
		prev_echo = 0;
		kbp = 0;
		freeze = 0;
/*		if (debug_screen_init == 0)
		{
			for (i=0;i<10;i++)
				trace_status[i] = 0;
			var_off = (int)0x40000000;
		}
*/
		debug_screen_init = 0;
		return;
	}

	switch (state)
	{
		case 0:
			state = 1;
			wnum = 0;
		    ScreenClear();
			break;

		case 1:
			clr_windows(wnum);
			if (wnum >= 16)
				state = 2;
			else
				wnum++;
			break;
		case 2:
			max_car_row = 21;
			max_row = 24;
			max_window_row = 26;
			set_font(font_8x14);	// set to get 25 rows
			state = 3;
			break;
		case 3:
			setcursortype(_NOCURSOR);
			state = 4;
			break;
		case 4:
			if (dbg_page == 7)
			{
				dbgscn = vcreat( 21, 78, WHITE );
				vlocate( dbgscn, 1, 1);
				state = 5;
			}
			else
			{
				if(dbg_page == 0)
					dbgscn = vcreat( 15, 78, WHITE ); // adding for more rows for debugs
				else
					dbgscn = vcreat( 11, 78, WHITE );
				vlocate( dbgscn, 1, 1);
				state = 5;
			}
			break;
		case 5:
			vframe( dbgscn, WHITE, FRSINGLE );
			if (dbg_page == 7)
				state = 9;
			else
				state = 6;
			break;
		case 6:
			trascn = vcreat( 10, 78, WHITE);
			if(dbg_page == 0)
				vlocate( trascn, 18, 1);
			else
				vlocate( trascn, 14, 1);
			state = 7;
			break;
		case 7:
			state = 8;
			break;
		case 8:
			vframe( trascn, WHITE, FRSINGLE );
			state = 9;
			break;
		case 9:
			if (dbg_page == 0)
			{
				vratputf(dbgscn,0,33,WHITE,"Debug Screen");
				state = 10;
			}
			else if (dbg_page == 1)
			{
#if (GALaxy_4 == 1)
				vratputf(dbgscn,0,31,WHITE,"CT Can Bus Screen");
#else			
				vratputf(dbgscn,0,31,WHITE,"Can Bus Screen");
#endif
				row = 1;
				tbl = 0;
				col = 3;
				state = 14;
			}
			else if (dbg_page == 2)
			{
				vratputf(dbgscn,0,31,WHITE,"Group Debug Screen");
				state = 22;	
			}
#if(Traction == 1)
			else if (dbg_page == 3)
			{
				vratputf(dbgscn,0,20,WHITE,"Pulse Count Update Screen       Max Err =%7i",max_error_count);
				state = 24;	
			}
#endif
			else if (dbg_page == 4)
			{
				vratputf(dbgscn,0,31,WHITE,"Can Bus Door Screen");
				row = 1;
				tbl = 0;
				col = 3;
				state = 14;
			}
			else if (dbg_page == 5)
			{
				vratputf(dbgscn,0,31,WHITE,"Drive Comm Screen");
				row = 1;
				tbl = 0;
				col = 3;
				state = 14;
			}
			else if (dbg_page == 6)
			{
				vratputf(dbgscn,0,31,WHITE,"UPS Comm Screen");
				vratputf(dbgscn,1,1,WHITE,"RX=");
				vratputf(dbgscn,9,1,WHITE,"TX=");
				row = 1;
				tbl = 0;
				col = 3;
				state = 16;
			}
			else if (dbg_page == 7)
			{
				vratputf(dbgscn,0,31,WHITE,"Split Feeder EMP Screen");
				row = 1;
				tbl = 0;
				col = 3;
				state = 26;
			}
			else if (dbg_page == 8)
			{
				vratputf(dbgscn,0,31,WHITE,"Keyboard Screen");
//				vratputf(dbgscn,1,1,WHITE,"Raw Key Code");
				row = 1;
				tbl = 0;
				col = 3;
				kb_row = 1;
				kb_col = 1;
				g_rix = p_rix;
				state = 16;		// goto trace screen
			}
			else if (dbg_page == 9)
			{
			{
				vratputf(dbgscn,0,31,WHITE,"MR Can Bus Screen");
				row = 1;
				tbl = 0;
				col = 3;
				state = 14;
			}
			}
			
			if(dbg_page != 7)
			{
				vratputf(trascn,0,30,WHITE,"Memory Trace Screen");
			}
			row = 1;
			break;
		case 10:
			for (i=0;i<=4;i++)
			{
				vratputf(dbgscn,row,3,WHITE,"%.10s",dbg_tbl_1[row-1]);
				row++;
			}
			if (row >= 10)
			{
				row = 1;
				state++;
			}
			break;
		case 11:
			for (i=0;i<=4;i++)
			{
				vratputf(dbgscn,row,17,WHITE,"%.13s",dbg_tbl_2[row - 1]);
				row++;
			}
			if (row >= 10)
			{
				row = 1;
				state++;
			}
			break;

		case 12:
			for (i=0;i<=4;i++)
			{
				vratputf(dbgscn,row,34,WHITE,"%.18s",dbg_tbl_3[row-1]);
				row++;
			}
			if (row >= 14)
			{
				row = 1;
				state++;
			}
			break;
		case 13:
			for (i=0;i<=4;i++)
			{
				vratputf(dbgscn,row,56,WHITE,"%.12s",dbg_tbl_4[row-1]);
				row++;
			}
			if (row >= 10)
			{
				row = 1;
				state = 16;	// trace screen setup
			}
			break;

		// can bus setup
		case 14:
			if (dbg_page == 4)
			{
				for (i=0;i<=2;i++)
				{	// print out door can table
					vratputf(dbgscn,row,col,WHITE,"%.10s",door_tbl[tbl][row-1]);
					row++;
				}
			}
			else if (dbg_page == 5)
			{
				for (i=0;i<=2;i++)
				{	// print out door can table
					vratputf(dbgscn,row,col,WHITE,"%.10s",drive_tbl[tbl][row-1]);
					row++;
				}
			}
			else
			{
				for (i=0;i<=2;i++)
				{	// print out can table
					vratputf(dbgscn,row,col,WHITE,"%.10s",can_tbl[tbl][row-1]);
					row++;
				}
			}
			if (row >= 5)
			{
				row = 1;
				col += 14;
				if (tbl >= 4)
				{
					tbl = 0;
					row = 7;
				 	state++;
				}
				else
					tbl++;
			}
			break;

		case 15:
			if (dbg_page == 5)
			{
				vratputf(dbgscn,row++,3,WHITE,"%.10s",drv_tbl_2[0]);
				vratputf(dbgscn,row++,3,WHITE,"%.10s",drv_tbl_2[1]);
				vratputf(dbgscn,row++,3,WHITE,"%.10s",drv_tbl_2[2]);
				vratputf(dbgscn,row++,3,WHITE,"%.10s",drv_tbl_2[3]);
			}
			else
			{
				vratputf(dbgscn,row++,3,WHITE,"%.10s",can_tbl_2[0]);
				vratputf(dbgscn,row++,3,WHITE,"%.10s",can_tbl_2[1]);
				vratputf(dbgscn,row++,3,WHITE,"%.10s",can_tbl_2[2]);
				vratputf(dbgscn,row++,3,WHITE,"%.10s",can_tbl_2[3]);
			}
			state = 16;
			break;

		// Trace Screen setup
		case 16:		
			vratputf(trascn,row,6, WHITE,"Name       Address       Data");
			vratputf(trascn,row,43,WHITE,"Name       Address       Data");
			row = 2;
			state++;
			break;
		case 17:
			for (i=0;i<=4;i++)
			{
				vratputf(trascn,row,3,WHITE,"%i) ",i);
				row++;
			}
			row = 2;
			state++;
			break;
		case 18:
			for (i=5;i<=9;i++)
			{
				vratputf(trascn,row,40,WHITE,"%i) ",i);
				row++;
			}
			row = 1;
			state++;
			break;
		case 19:
			for (i=0;i<5;i++)
			{
				if (trace_status[i] == 1)
				{
					vratputf(trascn,i+2,6,WHITE,"%.10s",trace_name[i]); 
					vratputf(trascn,i+2,17,WHITE,"%0x",(int32)trace_ptr[i]);
					put_trace(i+2,28,i);
				}
			}
			state++;
			break;
		case 20:
			for (i=5;i<10;i++)
			{
				if (trace_status[i] == 1)
				{
					vratputf(trascn,(i-5)+2,43,WHITE,"%.10s",trace_name[i]); 
					vratputf(trascn,(i-5)+2,54,WHITE,"%0x",(int32)trace_ptr[i]);
					put_trace((i-5)+2,65,i);
				}
			}
			state++;
			break;
		case 21:
			state = 0;
			debug_screen_init = 1;
			Debug_Screen(Reset_State);
			row = 9;
			col = 1;
			vratputf(trascn,row,col,WHITE,">                                                  ");
			col = 2;
			break;

		// group debug screen

		case 22:
			vratputf(dbgscn,1,1,WHITE,"%.77s",grp_tbl[0]);
			vratputf(dbgscn,2,1,WHITE,"%.77s",grp_tbl[1]);
			vratputf(dbgscn,3,1,WHITE,"%.77s",grp_tbl[2]);
			vratputf(dbgscn,4,1,WHITE,"%.77s",grp_tbl[3]);
			vratputf(dbgscn,5,1,WHITE,"%.77s",grp_tbl[4]);
			state = 23;
			break;

		case 23:
			vratputf(dbgscn,6,1,WHITE,"%.77s",grp_tbl[5]);
			vratputf(dbgscn,7,1,WHITE,"%.77s",grp_tbl[6]);
			vratputf(dbgscn,8,1,WHITE,"%.77s",grp_tbl[7]);
			vratputf(dbgscn,9,1,WHITE,"%.77s",grp_tbl[8]);
			vratputf(dbgscn,10,1,WHITE,"%.77s",grp_tbl[9]);
			state = 16;
			break;
		// group debug screen

		case 24:
			vratputf(dbgscn,1,1,WHITE,"%.77s",run_tbl[0]);
			vratputf(dbgscn,2,1,WHITE,"%.77s",run_tbl[1]);
			vratputf(dbgscn,3,1,WHITE,"%.77s",run_tbl[2]);
			vratputf(dbgscn,4,1,WHITE,"%.77s",run_tbl[3]);
			vratputf(dbgscn,5,1,WHITE,"%.77s",run_tbl[4]);
			state = 25;
			break;

		case 25:
			vratputf(dbgscn,6,1,WHITE,"%.77s",run_tbl[5]);
			vratputf(dbgscn,7,1,WHITE,"%.77s",run_tbl[6]);
			vratputf(dbgscn,8,1,WHITE,"%.77s",run_tbl[7]);
			vratputf(dbgscn,9,1,WHITE,"%.77s",run_tbl[8]);
			vratputf(dbgscn,10,1,WHITE,"%.77s",run_tbl[9]);
			state = 16;
			break;

		case 26:
			vratputf(dbgscn,1,1,WHITE,"%.77s",igep_tbl[0]);
			vratputf(dbgscn,2,1,WHITE,"%.77s",igep_tbl[1]);
			vratputf(dbgscn,3,1,WHITE,"%.77s",igep_tbl[2]);
			vratputf(dbgscn,4,1,WHITE,"%.77s",igep_tbl[3]);
			vratputf(dbgscn,5,1,WHITE,"%.77s",igep_tbl[4]);
			state = 27;
			break;

		case 27:
			vratputf(dbgscn,6,1,WHITE,"%.77s",igep_tbl[5]);
			vratputf(dbgscn,7,1,WHITE,"%.77s",igep_tbl[6]);
			vratputf(dbgscn,8,1,WHITE,"%.77s",igep_tbl[7]);
			vratputf(dbgscn,9,1,WHITE,"%.77s",igep_tbl[8]);
			vratputf(dbgscn,10,1,WHITE,"%.77s",igep_tbl[9]);
			state = 28;
			break;

		case 28:
			vratputf(dbgscn,11,1,WHITE,"%.77s",igep_tbl[10]);
			vratputf(dbgscn,12,1,WHITE,"%.77s",igep_tbl[11]);
			vratputf(dbgscn,13,1,WHITE,"%.77s",igep_tbl[12]);
			vratputf(dbgscn,14,1,WHITE,"%.77s",igep_tbl[13]);
			vratputf(dbgscn,15,1,WHITE,"%.77s",igep_tbl[14]);
			state = 29;
			break;

		case 29:
			vratputf(dbgscn,16,1,WHITE,"%.77s",igep_tbl[15]);
			vratputf(dbgscn,17,1,WHITE,"%.77s",igep_tbl[16]);
			vratputf(dbgscn,18,1,WHITE,"%.77s",igep_tbl[17]);
			vratputf(dbgscn,19,1,WHITE,"%.77s",igep_tbl[18]);
			vratputf(dbgscn,20,1,WHITE,"%.77s",igep_tbl[19]);
			debug_screen_init = 1;
			state = 0;
			break;

		default:
			break;			
	}
	
}

//****************************************************
//********* Display Deubg Screen *********************
//****************************************************

void Debug_Screen(int16 init_flag)
{
	int16 i;
#if (Traction == 1)
	float tempd;
#endif
	void *vptr;
	static int16 state;
	static int16 sub_state;

	if (init_flag == 1)
	{
		state = 0;
		return;
	}


	switch (state)
	{
		case 0:
			if ((dbg_page == 1) || (dbg_page == 4) || (dbg_page == 5) || (dbg_page == 9))
			{			// can bus screen
				state = 1;
				sub_state = 0;
			}
			else if (dbg_page == 2)
			{			// group debug screen
				state = 15;
				sub_state = 0;
			}
#if(Traction == 1)
			else if (dbg_page == 3)
			{			// pulse count update screen
				state = 16;
				sub_state = 0;
			}
#endif
			else if (dbg_page == 6)
			{
				state = 10;
				sub_state = 0;
			}
			else if (dbg_page == 7)
			{
				state = 17;
				sub_state = 0;
			}
			else if (dbg_page == 8)
			{
				state = 18;
				sub_state = 0;
			}
			else
				state = 2;
			break;
		case 1:
#if (GALaxy_4 == 1)
			if (dbg_page == 1)
				sub_state = ctcan_screen(sub_state);
			else if (dbg_page == 9)
				sub_state = mrcan_screen(sub_state);
#else
			if ((dbg_page == 1) || (dbg_page == 4))
				sub_state = can_screen(sub_state);
#endif
			else if (dbg_page == 5)
				sub_state = drv_screen(sub_state);
			
			if (sub_state == 0)
				state = 10;
			break;
		case 2:
			vratputf(dbgscn,1,13,WHITE,"%2i",servf);
			vratputf(dbgscn,2,13,WHITE,"%2i",procf);
			vratputf(dbgscn,3,13,WHITE,"%2i",empf);
			vratputf(dbgscn,4,13,WHITE,"%2i",dirf);
			vratputf(dbgscn,5,13,WHITE,"%2i",dpref);
			state++;
			break;
		case 3:
			vratputf(dbgscn,6,13,WHITE,"%2i",dcalls);
			vratputf(dbgscn,7,13,WHITE,"%2i",hsf);
			vratputf(dbgscn,8,13,WHITE,"%2i",stepf);
			vratputf(dbgscn,9,13,WHITE,"%2i",nstopf);
			vratputf(dbgscn,10,13,WHITE,"%2i",relevel);
			state++;
			break;
		case 4:
			vratputf(dbgscn,1,30,WHITE,"%2i",doorf);
			vratputf(dbgscn,2,30,WHITE,"%2i",rdoorf);
			vratputf(dbgscn,3,30,WHITE,"%2d",doort);
			vratputf(dbgscn,4,30,WHITE,"%2i",closetry);
			vratputf(dbgscn,5,30,WHITE,"%2i",rclosetry);
			state++;
			break;
		case 5:
			vratputf(dbgscn,6,30,WHITE,"%2i",rdoort);
			vratputf(dbgscn,7,30,WHITE,"%2i",firef);
			vratputf(dbgscn,8,30,WHITE,"%2i",fs1_effect);
			vratputf(dbgscn,9,30,WHITE,"%2i",FS_DOB_DIS);
			vratputf(dbgscn,10,30,WHITE,"%2i",fs2_to_fs1);
			state++;
			break;
		case 6:
			vratputf(dbgscn,1,48,WHITE,"%2x",debug1);
			vratputf(dbgscn,2,48,WHITE,"%2x",debug2);
			vratputf(dbgscn,3,48,WHITE,"%2x",debug3);
			vratputf(dbgscn,4,48,WHITE,"%2x",debug4);
			vratputf(dbgscn,5,48,WHITE,"%2x",debug5);
			vratputf(dbgscn,6,48,WHITE,"%2x",debug6);
			vratputf(dbgscn,7,48,WHITE,"%2x",debug7);
			vratputf(dbgscn,8,48,WHITE,"%2x",debug8);

			state++;
			break;

		case 7:
/*
			vratputf(dbgscn,9,48,WHITE,"%2d",Roll_early_exit);
			vratputf(dbgscn,10,48,WHITE,"%2d",short_fl_run);
			//run time
			if ((procf >= 14) && (procf <= 16))
				i = run_time;
			else
				i = 0;
			vratputf(dbgscn,11,48,WHITE,"%3.2f",((float)(i)/10.0));
			//target time
			if ((run_time >= targetfl_time) && (procf >= 14) && (procf <= 16))
				i = run_time - targetfl_time;
			else
				i = 0;
			vratputf(dbgscn,12,48,WHITE,"%3.2f",((float)(i)/10.0));
			//preopen time
			if ((run_time >= preopen_time) && (preopen_time != 0) && (procf >= 14) && (procf <= 16))
				i = run_time - preopen_time;
			else
				i = 0;
			vratputf(dbgscn,13,48,WHITE,"%3.2f",((float)(i)/10.0));
			vratputf(dbgscn,14,48,WHITE,"%2x",dpy_io_data);
			state++;
*/			
			vratputf(dbgscn,9,48,WHITE,"%5i",com_rx_char_cnt[0]);
			vratputf(dbgscn,10,48,WHITE,"%5i",good_packet[0]);
			vratputf(dbgscn,11,48,WHITE,"%5i",bad_packet[0]);
			vratputf(dbgscn,12,48,WHITE,"%5i",dma_start_cnt[0]);
			vratputf(dbgscn,13,48,WHITE,"%5i",dma_end_cnt[0]);
			vratputf(dbgscn,14,48,WHITE,"%2x",dpy_io_data);
			state++;
			break;
			
		case 8:
			vratputf(dbgscn,1,68,WHITE,"%7d",position);
#if (Traction == 1)
			vratputf(dbgscn,2,68,WHITE,"%7ld",DPP_Count);
			vratputf(dbgscn,3,68,WHITE,"%7.1f",Velocity);
			vratputf(dbgscn,4,68,WHITE,"%7.1f",Vel_Step);
			vratputf(dbgscn,5,68,WHITE,"%7.1f",Vel_Max);
#endif
			state++;
			break;
		case 9:
#if (Traction == 1)
			tempd = (float)((Velocity * 5.0)/Pulses_per_Inch);
			vratputf(dbgscn,6,68,WHITE,"%3i",(int)(tempd + 0.5));
#endif
#if (Traction == 1)
			vratputf(dbgscn,7,68,WHITE,"%7ld",DPP_Floor_Pos[position]);
			vratputf(dbgscn,8,68,WHITE,"%7.1f",Pulses_per_Inch);
#else
    #if (Simulator == 1)
			vratputf(dbgscn,7,68,WHITE,"%7ld",S_DPP_Floor_Pos[position]);
			vratputf(dbgscn,8,68,WHITE,"%7.1f",Pulses_per_Inch);
	#endif
#endif

#if (Simulator == 1)
			vratputf(dbgscn,9,68,WHITE,"%7d",sim_pos);
			vratputf(dbgscn,10,68,WHITE,"%7ld",sim_DPP_Count);
#endif
			state++;
			break;
		case 10:
 			for (i=0;i<5;i++)
			{
				if (trace_status[i] == 1)
					put_trace(i+2,28,i);
			}

			state++;
			break;
		case 11:
			for (i=5;i<10;i++)
			{
				if (trace_status[i] == 1)
					put_trace((i-5)+2,65,i);
			}

			state++;
			break;
		case 12:
			if (dump_status[0] == 1)
			{
				vptr = trace_ptr[dump_num[0]];
				vratputf(trascn,7,3,WHITE,"%1d)",dump_num[0]);
				for(i=0;i<dump_len[0];i+=3)
				{
					vratputf(trascn,7,i+6,WHITE,"%2x,",(*(char*)vptr & 0xff));
//					(char*)vptr = (char*)vptr + 1;
				}					
			}
			state++;
			break;
		case 13:
			if (dump_status[1] == 1)
			{
				vptr = trace_ptr[dump_num[1]];

				vratputf(trascn,8,3,WHITE,"%1d)",dump_num[1]);
				for(i=0;i<dump_len[1];i+=3)
				{
					vratputf(trascn,8,i+6,WHITE,"%2x,",(*(char*)vptr & 0xff));
//					(char*)vptr = (char*)vptr + 1;
				}					
			}
			state++;
			break;
		case 14:
			if (dump_status[2] == 1)
			{
				vptr = trace_ptr[dump_num[2]];

				vratputf(trascn,9,3,WHITE,"%1d)",dump_num[2]);
				for(i=0;i<dump_len[2];i+=3)
				{
					vratputf(trascn,9,i+6,WHITE,"%2x,",(*(int*)vptr & 0xff));
//					(char*)vptr = (char*)vptr + 1;
				}					
			}
			state = 0;
			break;
		case 15:
			sub_state = group_debug_screen(sub_state);
			if (sub_state == 0)
				state = 10;
			break;
		case 16:
#if(Traction == 1)
			sub_state = pulse_count_update_screen(sub_state);
#endif
			if (sub_state == 0)
				state = 10;
			break;
		case 17:
			echo = 0;
			sub_state = igep_debug_screen(sub_state);
			break;
		case 18:
			sub_state = keyboard_debug_screen(sub_state);
			if (sub_state == 0)
				state = 10;
			break;
		default:
			break;
	}
}

void put_trace(int16 row, int16 col,int16 num)
{
	int32 *vptr32;
	int16 *vptr16;
	int8 *vptr8;
	float *vptrfloat;	
  
   					 
	if (set_memory_variable == 1)
    {
    	if ((trace_format[num] == 'B') || (trace_format[num] == 'C') || (trace_format[num] == 'H'))
    	{
			vptr8 = (int8 *)trace_ptr[num];
        	*vptr8 = (uint8)debug_variable[num];       // write the byte variable to an integer 
    	}
    	else if ((trace_format[num] == 'W') || (trace_format[num] == 'S'))
    	{
			vptr16 = (int16 *)trace_ptr[num];
    		*vptr16 = (uint16)debug_variable[num];      // write word variable (int16 is defined as a short int)
    	}
    	else
        {
			vptr32 = (int32 *)trace_ptr[num];
        	*vptr32 = (uint32)debug_variable[num];        // write the integer variable to an integer 
        }
	}
   
     	     
	switch (trace_format[num])
	{
		case 'D':
			vptr32 = (int32 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7d",*vptr32);
			break;
		case 'B':
			vptr8 = (int8 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7d",*vptr8);
			break;
		case 'H':
			vptr8 = (int8 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7x",*vptr8);
			break;
		case 'W':
			vptr16 = (int16 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7x",*vptr16);
			break;
		case 'S':		// Short Integer 16 bits
			vptr16 = (int16 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7d",*vptr16);
			break;
		case 'X':
			vptr32 = (int32 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7x",*vptr32);
			break;
		case 'I':	// Integer 32 bits
			vptr32 = (int32 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7d",*vptr32);
			break;
		case 'U':
			vptr32 = (int32 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7u",*vptr32);
			break;
		case 'C':
			vptr8 = (int8 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%c",*vptr8);
			break;
		case 'L':			  
			vptr32 = (int32 *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7ld",*vptr32);
			break;
		case 'E':
			vptrfloat = (float *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7.4e",*vptrfloat);
			break;
		case 'F':
			vptrfloat = (float *)trace_ptr[num];
			vratputf(trascn,row,col,WHITE,"%7.1f",*vptrfloat);
			break;
		default:
			vptr32 = (int32 *)trace_ptr[num];
		  	vratputf(trascn,row,col,WHITE,"%7d",*vptr32);
			break;
	}  
	
  	set_memory_variable = 0;  
}

#if (GALaxy_4 == 1)
int16 mrcan_screen(int16 state)
{
	int16 i,j;
	static int16 dpy_ix;

	switch (state)
	{
		case 0:
			vratputf(dbgscn,1,14,WHITE,"%2x",mrcan_status);
			vratputf(dbgscn,2,14,WHITE,"%2x",mrcan_mode);
			vratputf(dbgscn,3,14,WHITE,"%2x",mrcan_arb_lost_cap);
			state++;
			break;
		case 1:
			vratputf(dbgscn,4,14,WHITE,"%2x",mrcan_err_code_cap);
			vratputf(dbgscn,5,13,WHITE,"%3i",mrcan_tx_err_cnt);
			vratputf(dbgscn,6,13,WHITE,"%3i",mrcan_rx_err_cnt);
			state++;
			break;
		case 2:
//				vratputf(dbgscn,1,27,WHITE,"%3i",RI_cnt);
//				vratputf(dbgscn,2,27,WHITE,"%3i",TI_cnt);
//				vratputf(dbgscn,3,27,WHITE,"%3i",EWI_cnt);
			state++;
			break;
		case 3:
//				vratputf(dbgscn,4,27,WHITE,"%3i",DOI_cnt);
//				vratputf(dbgscn,5,27,WHITE,"%3i",WUI_cnt);
//				vratputf(dbgscn,6,27,WHITE,"%3i",EPI_cnt);
			state++;
			break;
		case 4:
//			vratputf(dbgscn,1,41,WHITE,"%3i",ALI_cnt);
//			vratputf(dbgscn,2,41,WHITE,"%3i",BEI_cnt);
			vratputf(dbgscn,3,42,WHITE,"%2x",mrcan_debug_1 & 0x00ff);
			state++;
			break;
		case 5:
			vratputf(dbgscn,4,42,WHITE,"%2x",mrcan_debug_2 & 0x00ff);
			vratputf(dbgscn,5,42,WHITE,"%2x",mrcan_debug_3 & 0x00ff);
			vratputf(dbgscn,6,42,WHITE,"%2x",mrcan_debug_4 & 0x00ff);
			state++;
			break;
		case 6:
			vratputf(dbgscn,1,56,WHITE,"%2x",ir_mask_1);
			vratputf(dbgscn,2,56,WHITE,"%2x",ir_mask_2);
			vratputf(dbgscn,3,56,WHITE,"%2x",mrcan_int_status);
			state++;
			break;
		case 7:
			vratputf(dbgscn,4,55,WHITE,"%3i",mrcan_int_count);
			j = 0;
			for(i=0;i<nmb_mrcan_dev;i++)
			{
				vratputf(dbgscn,5,55+j,WHITE,"%1i,",mrcan_dev[mrcan_dev_list[i]].online);
				j+=2;
			}
			state++;
			break;
		case 8:
			vratputf(dbgscn,1,69,WHITE,"%2x",mrcan_error & 0x00ff);
//			vratputf(dbgscn,2,68,WHITE,"%7ld",SEL_can.poscnt);
			state++;
			break;
		case 9:
//	 		vratputf(dbgscn,3,69,WHITE,"%2x",SEL_can.position);
//		  	vratputf(dbgscn,4,69,WHITE,"%2x",SEL_can.status);
			state++;
			break;
		case 10:
			i = 13;
			dpy_ix = mrc_tpix;
			vratputf(dbgscn,7,i,WHITE,"%02x,",mrcan_tx[dpy_ix].len);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",mrcan_tx[dpy_ix].msg);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",mrcan_tx[dpy_ix].addr);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",mrcan_tx[dpy_ix].id2);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",mrcan_tx[dpy_ix].id3);
			state++;
			break;
		case 11:
			j = 0;
			for(i=0;i<8;i++)
			{
				vratputf(dbgscn,7,28+j,WHITE,"%02x,",mrcan_tx[dpy_ix].data[i]);
				j+=3;
			}
			state++;
			break;
		case 12:
			dpy_ix = mrc_rpix;
			i=13;
			vratputf(dbgscn,8,i,WHITE,"%02x,",mrcan_rx[dpy_ix].len);	
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",mrcan_rx[dpy_ix].msg);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",mrcan_rx[dpy_ix].addr);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",mrcan_rx[dpy_ix].id2);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",mrcan_rx[dpy_ix].id3);
			state++;
			break;
		case 13:
			j = 0;
			for(i=0;i<8;i++)
			{
				vratputf(dbgscn,8,28+j,WHITE,"%02x,",mrcan_rx[dpy_ix].data[i]);
				j+=3;
			}
			state++;
			break;
		case 14:
			j = 0;
			for(i=0;i<nmb_mrcan_dev;i++)
			{
				vratputf(dbgscn,9,13+j,WHITE,"%2i,",mrcan_addr[mrcan_dev_list[i]]);
				j+=3;
			}
			state++;  
			break;
		case 15:
			j = 0;
			for(i=0;i<nmb_mrcan_dev;i++)
			{
				vratputf(dbgscn,10,13+j,WHITE,"%2i.%02i,",mrcan_dev[mrcan_dev_list[i]].version,
										mrcan_dev[mrcan_dev_list[i]].revision);
				j+=6;
			}
			state = 0;
			break;
		default:
			break;
	}
	return state;
}  


int16 ctcan_screen(int16 state)
{
	int16 i,j;
	static int16 dpy_ix;

	switch (state)
	{
		case 0:
			vratputf(dbgscn,1,14,WHITE,"%2x",ctcan_status);
			vratputf(dbgscn,2,14,WHITE,"%2x",ctcan_mode);
			vratputf(dbgscn,3,14,WHITE,"%2x",ctcan_arb_lost_cap);
			state++;
			break;
		case 1:
			vratputf(dbgscn,4,14,WHITE,"%2x",ctcan_err_code_cap);
			vratputf(dbgscn,5,13,WHITE,"%3i",ctcan_tx_err_cnt);
			vratputf(dbgscn,6,13,WHITE,"%3i",ctcan_rx_err_cnt);
			state++;
			break;
		case 2:
//				vratputf(dbgscn,1,27,WHITE,"%3i",RI_cnt);
//				vratputf(dbgscn,2,27,WHITE,"%3i",TI_cnt);
//				vratputf(dbgscn,3,27,WHITE,"%3i",EWI_cnt);
			state++;
			break;
		case 3:
//				vratputf(dbgscn,4,27,WHITE,"%3i",DOI_cnt);
//				vratputf(dbgscn,5,27,WHITE,"%3i",WUI_cnt);
//				vratputf(dbgscn,6,27,WHITE,"%3i",EPI_cnt);
			state++;
			break;
		case 4:
//			vratputf(dbgscn,1,41,WHITE,"%3i",ALI_cnt);
//			vratputf(dbgscn,2,41,WHITE,"%3i",BEI_cnt);
			vratputf(dbgscn,3,42,WHITE,"%2x",ctcan_debug_1 & 0x00ff);
			state++;
			break;
		case 5:
			vratputf(dbgscn,4,42,WHITE,"%2x",ctcan_debug_2 & 0x00ff);
			vratputf(dbgscn,5,42,WHITE,"%2x",ctcan_debug_3 & 0x00ff);
			vratputf(dbgscn,6,42,WHITE,"%2x",ctcan_debug_4 & 0x00ff);
			state++;
			break;
		case 6:
			vratputf(dbgscn,1,56,WHITE,"%2x",ir_mask_1);
			vratputf(dbgscn,2,56,WHITE,"%2x",ir_mask_2);
			vratputf(dbgscn,3,56,WHITE,"%2x",ctcan_int_status);
			state++;
			break;
		case 7:
			vratputf(dbgscn,4,55,WHITE,"%3i",ctcan_int_count);
			j = 0;
			for(i=0;i<nmb_ctcan_dev;i++)
			{
				vratputf(dbgscn,5,55+j,WHITE,"%1i,",ctcan_dev[ctcan_dev_list[i]].online);
				j+=2;
			}
			state++;
			break;
		case 8:
			vratputf(dbgscn,1,69,WHITE,"%2x",ctcan_error & 0x00ff);
//			vratputf(dbgscn,2,68,WHITE,"%7ld",SEL_can.poscnt);
			state++;
			break;
		case 9:
//	 		vratputf(dbgscn,3,69,WHITE,"%2x",SEL_can.position);
//		  	vratputf(dbgscn,4,69,WHITE,"%2x",SEL_can.status);
			state++;
			break;
		case 10:
			i = 13;
			dpy_ix = ctc_tpix;
			vratputf(dbgscn,7,i,WHITE,"%02x,",ctcan_tx[dpy_ix].len);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",ctcan_tx[dpy_ix].msg);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",ctcan_tx[dpy_ix].addr);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",ctcan_tx[dpy_ix].id2);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",ctcan_tx[dpy_ix].id3);
			state++;
			break;
		case 11:
			j = 0;
			for(i=0;i<8;i++)
			{
				vratputf(dbgscn,7,28+j,WHITE,"%02x,",ctcan_tx[dpy_ix].data[i]);
				j+=3;
			}
			state++;
			break;
		case 12:
			dpy_ix = ctc_rpix;
			i=13;
			vratputf(dbgscn,8,i,WHITE,"%02x,",ctcan_rx[dpy_ix].len);	
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",ctcan_rx[dpy_ix].msg);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",ctcan_rx[dpy_ix].addr);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",ctcan_rx[dpy_ix].id2);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",ctcan_rx[dpy_ix].id3);
			state++;
			break;
		case 13:
			j = 0;
			for(i=0;i<8;i++)
			{
				vratputf(dbgscn,8,28+j,WHITE,"%02x,",ctcan_rx[dpy_ix].data[i]);
				j+=3;
			}
			state++;
			break;
		case 14:
			j = 0;
			for(i=0;i<nmb_ctcan_dev;i++)
			{
				vratputf(dbgscn,9,13+j,WHITE,"%2i,",ctcan_addr[ctcan_dev_list[i]]);
				j+=3;
			}
			state++;  
			break;
		case 15:
			j = 0;
			for(i=0;i<nmb_ctcan_dev;i++)
			{
				vratputf(dbgscn,10,13+j,WHITE,"%2i.%02i,",ctcan_dev[ctcan_dev_list[i]].version,
										ctcan_dev[ctcan_dev_list[i]].revision);
				j+=6;
			}
			state = 0;
			break;
		default:
			break;
	}
	return state;
}  

#else
int16 can_screen(int16 state)
{
	int16 i,j;
	static int16 dpy_ix;

	switch (state)
	{
		case 0:
			vratputf(dbgscn,1,14,WHITE,"%2x",can_status);
			vratputf(dbgscn,2,14,WHITE,"%2x",can_mode);
			vratputf(dbgscn,3,14,WHITE,"%2x",can_arb_lost_cap);
			state++;
			break;
		case 1:
			vratputf(dbgscn,4,14,WHITE,"%2x",can_err_code_cap);
			vratputf(dbgscn,5,13,WHITE,"%3i",can_tx_err_cnt);
			vratputf(dbgscn,6,13,WHITE,"%3i",can_rx_err_cnt);
			state++;
			break;
		case 2:
			if (dbg_page == 4)
			{
				vratputf(dbgscn,1,28,WHITE,"%2x",DOOR_can.command);
				vratputf(dbgscn,2,28,WHITE,"%2x",DOOR_can.status_1);
				vratputf(dbgscn,3,28,WHITE,"%2x",DOOR_can.status_2);
			}
			else
			{
				vratputf(dbgscn,1,27,WHITE,"%3i",RI_cnt);
//				vratputf(dbgscn,2,27,WHITE,"%3i",TI_cnt);
//				vratputf(dbgscn,3,27,WHITE,"%3i",EWI_cnt);
			}
			state++;
			break;
		case 3:
			if (dbg_page == 4)
			{
				vratputf(dbgscn,4,26,WHITE,"%4x",DOOR_can.dpy_req);
				vratputf(dbgscn,5,26,WHITE,"%4x",DOOR_can.dpy_ret);
				vratputf(dbgscn,6,26,WHITE,"%4x",DOOR_can.fault);
			}
			else
			{
//				vratputf(dbgscn,4,27,WHITE,"%3i",DOI_cnt);
//				vratputf(dbgscn,5,27,WHITE,"%3i",WUI_cnt);
//				vratputf(dbgscn,6,27,WHITE,"%3i",EPI_cnt);
			}
			state++;
			break;
		case 4:
			if (dbg_page == 4)
			{
				vratputf(dbgscn,1,40,WHITE,"%4x",DOOR_can.par_add_req);
				vratputf(dbgscn,2,40,WHITE,"%4x",DOOR_can.par_dat_wrt);
				vratputf(dbgscn,3,40,WHITE,"%4x",DOOR_can.par_add_ret);
			}
			else
			{
//				vratputf(dbgscn,1,41,WHITE,"%3i",ALI_cnt);
//				vratputf(dbgscn,2,41,WHITE,"%3i",BEI_cnt);
				vratputf(dbgscn,3,42,WHITE,"%2x",can_debug_1 & 0x00ff);
			}
			state++;
			break;
		case 5:
			if (dbg_page == 4)
			{
				vratputf(dbgscn,4,40,WHITE,"%4x",DOOR_can.par_dat_rd);
				vratputf(dbgscn,5,40,WHITE,"%4x",DOOR_can.voltage);
				vratputf(dbgscn,6,40,WHITE,"%4x",DOOR_can.current);
			}
			else
			{
				vratputf(dbgscn,4,42,WHITE,"%2x",can_debug_2 & 0x00ff);
				vratputf(dbgscn,5,42,WHITE,"%2x",can_debug_3 & 0x00ff);
				vratputf(dbgscn,6,42,WHITE,"%2x",can_debug_4 & 0x00ff);
			}
			state++;
			break;
		case 6:
			if (dbg_page == 4)
			{
				vratputf(dbgscn,1,56,WHITE,"%2x",RDOOR_can.command);
				vratputf(dbgscn,2,56,WHITE,"%2x",RDOOR_can.status_1);
				vratputf(dbgscn,3,56,WHITE,"%2x",RDOOR_can.status_2);
			}
			else
			{
				vratputf(dbgscn,1,56,WHITE,"%2x",ir_mask_1);
				vratputf(dbgscn,2,56,WHITE,"%2x",ir_mask_2);
				vratputf(dbgscn,3,56,WHITE,"%2x",can_int_status);
			}
			state++;
			break;
		case 7:
			if (dbg_page == 4)
			{
				vratputf(dbgscn,4,54,WHITE,"%4x",RDOOR_can.dpy_req);
				vratputf(dbgscn,5,54,WHITE,"%4x",RDOOR_can.dpy_ret);
				vratputf(dbgscn,6,54,WHITE,"%4x",RDOOR_can.fault);
			}
			else
			{
				vratputf(dbgscn,4,55,WHITE,"%3i",can_int_count);
				j = 0;
				for(i=0;i<nmb_can_dev;i++)
				{
					vratputf(dbgscn,5,55+j,WHITE,"%1i,",can_dev[can_dev_list[i]-c_can_START].online);
					j+=2;
				}
			}
			state++;
			break;
		case 8:
			if (dbg_page == 4)
			{
		 		vratputf(dbgscn,1,68,WHITE,"%4x",RDOOR_can.par_add_req);
		 		vratputf(dbgscn,2,68,WHITE,"%4x",RDOOR_can.par_dat_wrt);
		 		vratputf(dbgscn,3,68,WHITE,"%4x",RDOOR_can.par_add_ret);
			}
			else
			{
				vratputf(dbgscn,1,69,WHITE,"%2x",can_error & 0x00ff);
				vratputf(dbgscn,2,68,WHITE,"%7ld",SEL_can.poscnt);
			}
			state++;
			break;
		case 9:
			if (dbg_page == 4)
			{
		 		vratputf(dbgscn,4,68,WHITE,"%4x",RDOOR_can.par_dat_rd);
		 		vratputf(dbgscn,5,68,WHITE,"%4x",RDOOR_can.voltage);
		 		vratputf(dbgscn,6,68,WHITE,"%4x",RDOOR_can.current);
			}
			else
			{
		 		vratputf(dbgscn,3,69,WHITE,"%2x",SEL_can.position);
			  	vratputf(dbgscn,4,69,WHITE,"%2x",SEL_can.status);
			}
			state++;
			break;
		case 10:
			i = 13;
			dpy_ix = ctpix;
			vratputf(dbgscn,7,i,WHITE,"%02x,",can_tx[dpy_ix].len);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",can_tx[dpy_ix].msg);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",can_tx[dpy_ix].addr);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",can_tx[dpy_ix].id2);
			i+=3;
			vratputf(dbgscn,7,i,WHITE,"%02x,",can_tx[dpy_ix].id3);
			state++;
			break;
		case 11:
			j = 0;
			for(i=0;i<8;i++)
			{
				vratputf(dbgscn,7,28+j,WHITE,"%02x,",can_tx[dpy_ix].data[i]);
				j+=3;
			}
			state++;
			break;
		case 12:
			dpy_ix = crpix;
			i=13;
			vratputf(dbgscn,8,i,WHITE,"%02x,",can_rx[dpy_ix].len);	
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",can_rx[dpy_ix].msg);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",can_rx[dpy_ix].addr);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",can_rx[dpy_ix].id2);
			i+=3;
			vratputf(dbgscn,8,i,WHITE,"%02x,",can_rx[dpy_ix].id3);
			state++;
			break;
		case 13:
			j = 0;
			for(i=0;i<8;i++)
			{
				vratputf(dbgscn,8,28+j,WHITE,"%02x,",can_rx[dpy_ix].data[i]);
				j+=3;
			}
			state++;
			break;
		case 14:
			j = 0;
			for(i=0;i<nmb_can_dev;i++)
			{
				vratputf(dbgscn,9,13+j,WHITE,"%2i,",can_dev_list[i]);
				j+=3;
			}
			state++;  
			break;
		case 15:
			j = 0;
			for(i=0;i<nmb_can_dev;i++)
			{
				vratputf(dbgscn,10,13+j,WHITE,"%2i.%02i,",can_dev[can_dev_list[i]-c_can_START].version,
										can_dev[can_dev_list[i]-c_can_START].revision);
				j+=6;
			}
			state = 0;
			break;
		default:
			break;
	}
	return state;
}  
#endif

int16 drv_screen(int16 state)
{
	int16 i,j;
	static int16 dpy_ix;

	switch (state)
	{
		case 0:
			vratputf(dbgscn,1,14,WHITE,"%2x",drv_com_source);
			vratputf(dbgscn,2,14,WHITE,"%2x",drv_com_source);
			vratputf(dbgscn,3,14,WHITE,"%2x",drv_com_source);
			state++;
			break;
		case 1:
#if ((Traction == 1) || (Tract_OL == 1))
			vratputf(dbgscn,4,12,WHITE,"%4i",drv_com_count);
			vratputf(dbgscn,5,12,WHITE,"%4i",drive_trm_cnt);
			vratputf(dbgscn,6,12,WHITE,"%4i",drive_rcv_cnt);
#endif
			state++;
			break;
		case 2:
			vratputf(dbgscn,1,29,WHITE,"%1x",Tranbuf_Full[DRVCOM]);
			vratputf(dbgscn,2,29,WHITE,"%1x",Tranbuf_Empty[DRVCOM]);
			vratputf(dbgscn,3,27,WHITE,"%3i",tbf[DRVCOM]);
			state++;
			break;
		case 3:
			vratputf(dbgscn,4,27,WHITE,"%3i",tbs[DRVCOM]);
			vratputf(dbgscn,5,27,WHITE,"%3i",rbf[DRVCOM]);
			vratputf(dbgscn,6,27,WHITE,"%3i",rbs[DRVCOM]);
			state++;
			break;
		case 4:
			vratputf(dbgscn,1,43,WHITE,"%1x",Com_Tran_Int[DRVCOM]);
			vratputf(dbgscn,2,43,WHITE,"%1x",Com_Recv_Int[DRVCOM]);
			vratputf(dbgscn,3,40,WHITE,"%4x",debug1);
			state++;
			break;
		case 5:
			vratputf(dbgscn,4,40,WHITE,"%4x",debug2);
			vratputf(dbgscn,5,40,WHITE,"%4x",debug3);
			vratputf(dbgscn,6,40,WHITE,"%4x",debug4);
			state++;
			break;
		case 6:
#if ((Traction == 1) || (Tract_OL == 1))
			vratputf(dbgscn,1,54,WHITE,"%4x",drv_command);
			vratputf(dbgscn,2,54,WHITE,"%4i",drive_vel);
			vratputf(dbgscn,3,54,WHITE,"%4x",drv_status);
#endif
			state++;
			break;
		case 7:
#if ((Traction == 1) || (Tract_OL == 1))
			vratputf(dbgscn,4,54,WHITE,"%4x",drv_encoder_spd);
			vratputf(dbgscn,5,54,WHITE,"%4x",drv_motor_voltage);
			vratputf(dbgscn,6,54,WHITE,"%4x",drv_motor_current);
#endif
			state++;
			break;
		case 8:
#if ((Traction == 1) || (Tract_OL == 1))
	 		vratputf(dbgscn,1,68,WHITE,"%4x",drv_motor_frequency);
	 		vratputf(dbgscn,2,68,WHITE,"%4x",drv_fault_code);
			if (Drive_Type[cons[carnmb]] == 14)
	 			vratputf(dbgscn,3,68,WHITE,"%2i%2i",(drv_wpar_addr>>8),(drv_wpar_addr & 0x00ff));
			else
	 			vratputf(dbgscn,3,68,WHITE,"%5i",drv_wpar_addr);
#endif
			state++;
			break;
		case 9:
#if ((Traction == 1) || (Tract_OL == 1))
	 		vratputf(dbgscn,4,68,WHITE,"%5i",drv_wpar_data);
			if (Drive_Type[cons[carnmb]] == 14)
	 			vratputf(dbgscn,5,68,WHITE,"%2i%2i",(drv_rpar_addr>>8),(drv_rpar_addr & 0x00ff));
			else
	 			vratputf(dbgscn,5,68,WHITE,"%5i",drv_rpar_addr);
	 		vratputf(dbgscn,6,68,WHITE,"%5i",drv_rpar_data);
#endif
			state++;
			break;
		case 10:
#if ((Traction == 1) || (Tract_OL == 1))
			j = 0;
			for(i=0;i<13;i++)
			{
				vratputf(dbgscn,7,13+j,WHITE,"%02x,",drv_trm_buf[i]);
				j+=3;
			}
#endif
			state++;
			break;
		case 11:
#if ((Traction == 1) || (Tract_OL == 1))
			j = 0;
			for(i=0;i<18;i++)
			{
				vratputf(dbgscn,8,13+j,WHITE,"%02x,",drv_rcv_buf[i]);
				j+=3;
			}
#endif
			state = 0;
			break;

		default:
			break;
	}
	return state;
}  


int16 keyboard_debug_screen(int16 state)
{

	switch (state)
	{
		case 0:
			if (g_rix != p_rix)
				state = 1;
			else
				state = 0;
			break;
		case 1:
			vratputf(dbgscn,kb_row,(kb_col-1) * 3,WHITE,"%02x,",rawcode[g_rix]);
			if (g_rix < 99)
				g_rix++;
			else g_rix = 0;
			
			kb_col++;
			if (kb_col > 24)
			{
				kb_col = 1;
				kb_row++;
				if (kb_row > 10)
					kb_row = 1;
			}

			vratputf(dbgscn,kb_row,(kb_col-1) * 3,WHITE," < ");
			if (g_rix == p_rix)
				state = 0;
			break;
		default:
			break;
	}
	return state;
}  

int16 group_debug_screen(int16 state)
{
	int16 i,j;

	switch (state)
	{
		case 0:
			vratputf(dbgscn,2,16,WHITE,"%2i",ncu_car);
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,2,35+j,WHITE,"%2i,",ncu_door[i]);
				j+=5;
			}
			state++;  
			break;
		case 1:
			vratputf(dbgscn,3,16,WHITE,"%2i",ncu_seq);
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,3,35+j,WHITE,"%2i,",ncu_que[i]);
				j+=5;
			}
			state++;  
			break;
		case 2:
			vratputf(dbgscn,4,16,WHITE,"%2i",lobby_request);
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,4,35+j,WHITE,"%2i,",lby_req[i]);
				j+=5;
			}
			state++;  
			break;
		case 3:
			vratputf(dbgscn,5,16,WHITE,"%2i",nmb_lby_recall_req);
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,5,35+j,WHITE,"%2i,",g_park[i]);
				j+=5;
			}
			state++;  
			break;
		case 4:
			vratputf(dbgscn,6,16,WHITE,"%2i",nmb_cars_at_lobby);
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,6,35+j,WHITE,"%2i,",grp_parking_floor[i]);
				j+=5;
			}
			state++;  
			break;
		case 5:
			vratputf(dbgscn,7,16,WHITE,"%2i",next_up);
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,7,35+j,WHITE,"%2i,",prk_floor_full[i]);
				j+=5;
			}
			state++;  
			break;
		case 6:
			vratputf(dbgscn,8,16,WHITE,"%2i",ncu_door_seq);
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,8,35+j,WHITE,"%2i,",how_many[i]);
				j+=5;
			}
			state++;  
			break;
		case 7:
			vratputf(dbgscn,9,11,WHITE,"%2i",fdoor_req);
			vratputf(dbgscn,9,25,WHITE,"%2i",rdoor_req);
			vratputf(dbgscn,9,38,WHITE,"%2i",nudgst);
			for(i=0;i<=4;i++)
			{
				vratputf(dbgscn,9,55+5*i,WHITE,"%2i",g_secf_table[i]);
			}
			state++;  
			break;
		case 8:
			vratputf(dbgscn,10,11,WHITE,"%2i",fdoor_en);
			vratputf(dbgscn,10,25,WHITE,"%2i",rdoor_en);
			vratputf(dbgscn,10,38,WHITE,"%2i",rnudgst);
			vratputf(dbgscn,10,58,WHITE,"%2i",nmb_cars_in_svc);
			vratputf(dbgscn,10,76,WHITE,"%2i",all_cars_ots);
			
			state = 0;
			break;
		default:
			break;
	}
	return state;
}  

int16 igep_debug_screen(int16 state)
{

	int16 i,j;
	switch(state)
	{
		case 0:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,2,33+j,WHITE,"%2i,",grp_empf[0][i]);
				j+=5;
			}
			state++;  
			break;
		case 1:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,3,33+j,WHITE,"%2i,",grp_empf[1][i]);
				j+=5;
			}
			state++;  
			break;
		case 2:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,4,33+j,WHITE,"%2i,",car_recall_order[0][i]);
				j+=5;
			}
			state++;  
			break;
		case 3:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,5,33+j,WHITE,"%2i,",car_recall_order[1][i]);
				j+=5;
			}
			state++;  
			break;
		case 4:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,6,33+j,WHITE,"%2i,",car_select_order[0][i]);
				j+=5;
			}
			state++;  
			break;
		case 5:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,7,33+j,WHITE,"%2i,",car_select_order[1][i]);
				j+=5;
			}
			state++;  
			break;
		case 6:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,8,33+j,WHITE,"%2i,",man_sel_req[0][i]);
				j+=5;
			}
			state++;  
			break;
		case 7:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,9,33+j,WHITE,"%2i,",man_sel_req[1][i]);
				j+=5;
			}
			state++;  
			break;
		case 8:
			j = 0;
			for(i=0;i<=8;i++)
			{
				vratputf(dbgscn,10,33+j,WHITE,"%2i,",power_feeder[i]);
				j+=5;
			}
			state++;  
			break;

		case 9:
			vratputf(dbgscn,13,21,WHITE,"%2i",ep_auto_recall_en[0]);
			vratputf(dbgscn,13,24,WHITE,"%2i",ep_auto_recall_en[1]);
			vratputf(dbgscn,14,21,WHITE,"%2i",ep_auto_select_en[0]);
			vratputf(dbgscn,14,24,WHITE,"%2i",ep_auto_select_en[1]);
			vratputf(dbgscn,15,21,WHITE,"%2i",ep_run_en[0]);
			vratputf(dbgscn,15,24,WHITE,"%2i",ep_run_en[1]);
			vratputf(dbgscn,16,21,WHITE,"%2i",cars_moving[0]);
			state++;  
			break;

		case 10:
			vratputf(dbgscn,16,24,WHITE,"%2i",cars_moving[1]);
			vratputf(dbgscn,17,21,WHITE,"%2i",select_req[0]);
			vratputf(dbgscn,17,24,WHITE,"%2i",select_req[1]);
			vratputf(dbgscn,18,21,WHITE,"%2i",prev_select_req[0]);
			vratputf(dbgscn,18,24,WHITE,"%2i",prev_select_req[1]);
			vratputf(dbgscn,19,21,WHITE,"%2i",man_sel_seq[0]);
			vratputf(dbgscn,19,24,WHITE,"%2i",man_sel_seq[1]);
			state++;  
			break;

		case 11:
			vratputf(dbgscn,13,47,WHITE,"%2i",nmb_recall_cars[0]);
			vratputf(dbgscn,13,50,WHITE,"%2i",nmb_recall_cars[1]);
			vratputf(dbgscn,14,47,WHITE,"%2i",car_recall_en[0]);
			vratputf(dbgscn,14,50,WHITE,"%2i",car_recall_en[1]);
			vratputf(dbgscn,15,47,WHITE,"%2i",recall_seq[0]);
			vratputf(dbgscn,15,50,WHITE,"%2i",recall_seq[1]);
			vratputf(dbgscn,16,47,WHITE,"%2i",cars_recalling[0]);
			state++;  
			break;

		case 12:
			vratputf(dbgscn,16,50,WHITE,"%2i",cars_recalling[1]);
			vratputf(dbgscn,17,47,WHITE,"%2i",recalled_cars[0]);
			vratputf(dbgscn,17,50,WHITE,"%2i",recalled_cars[1]);
			vratputf(dbgscn,18,47,WHITE,"%2i",recall_attempt[0]);
			vratputf(dbgscn,18,50,WHITE,"%2i",recall_attempt[1]);
			vratputf(dbgscn,19,47,WHITE,"%2i",recall_finished[0]);
			vratputf(dbgscn,19,50,WHITE,"%2i",recall_finished[1]);
			state++;  
			break;

		case 13:
			vratputf(dbgscn,13,73,WHITE,"%2i",cars_in_motion[0]);
			vratputf(dbgscn,13,76,WHITE,"%2i",cars_in_motion[1]);
			vratputf(dbgscn,14,73,WHITE,"%2i",em_pwr_2);
			vratputf(dbgscn,14,76,WHITE,"%2i",ep_xfr_2);
			vratputf(dbgscn,15,73,WHITE,"%2i",car_fire_sw_on[0]);
			vratputf(dbgscn,15,76,WHITE,"%2i",car_fire_sw_on[1]);
			vratputf(dbgscn,16,73,WHITE,"%2i",nmb_select_cars[0]);
			state++;  
			break;

		case 14:
			vratputf(dbgscn,16,76,WHITE,"%2i",nmb_select_cars[1]);
			vratputf(dbgscn,17,73,WHITE,"%2i",car_select_en[0]);
			vratputf(dbgscn,17,76,WHITE,"%2i",car_select_en[1]);
			vratputf(dbgscn,18,73,WHITE,"%2i",select_seq[0]);
			vratputf(dbgscn,18,76,WHITE,"%2i",select_seq[1]);
			vratputf(dbgscn,19,73,WHITE,"%2i",cars_selected[0]);
			vratputf(dbgscn,19,76,WHITE,"%2i",cars_selected[1]);
			state = 0;  
			break;

		default:
			break;
	}
	return state;
}

#if(Traction == 1)
int16 pulse_count_update_screen(int16 state)
{
	int16 i;
	static int16 pos;
	int32 DPP_Count_diff;
	int32 DPP_Vel_Count_upd;
	int32 Vel_Count_upd_diff;
	int32 DPP_DZ_Count;

	switch (state)
	{
		case 0:
			pos = cons[bottomf];
			DPP_DZ_Count = 0;
			DPP_Count_diff = 0;
			DPP_Vel_Count_upd = 0;
			Vel_Count_upd_diff = 0;
			state = 1;
 					  
		//lint -fallthrough
 		case 1:		// "Pos,pf,Tr,   DZ Cnt,  Pls Cnt, Cnt Diff,Err Cnt,  Upd Val, Upd Diff, Car Vel,",
			for(i=0;i<=8;i++)
			{

				if ((pos <= cons[topf]) && (DPP_updf[pos] != 0))
				{
					if ((DPP_updf[pos] & 0x01) != 0)
					{
						DPP_DZ_Count = (DPP_Floor_Pos[pos] - DZU_Dist);
						DPP_Count_diff = DPP_Count_at_DZ[pos] - DPP_DZ_Count;
						DPP_Vel_Count_upd = DPP_DZ_Count + DPP_Vel_ERR_Count[pos];
						Vel_Count_upd_diff = DPP_Vel_Count_upd - DPP_Count_at_DZ[pos]; 
					}
					else if ((DPP_updf[pos] & 0x02) != 0)
					{
						DPP_DZ_Count = (DPP_Floor_Pos[pos] + DZD_Dist);
						DPP_Count_diff = DPP_Count_at_DZ[pos] - DPP_DZ_Count;
						DPP_Vel_Count_upd = DPP_DZ_Count - DPP_Vel_ERR_Count[pos];
						Vel_Count_upd_diff = DPP_Vel_Count_upd - DPP_Count_at_DZ[pos]; 
					}
					vratputf(dbgscn,2+i,2,WHITE,"%.2s,",flmrk[pos]);
					vratputf(dbgscn,2+i,5,WHITE,"%2i,",procf_at_upd[pos]);
					vratputf(dbgscn,2+i,8,WHITE,"%2i,",DPP_updf[pos]);
					vratputf(dbgscn,2+i,12,WHITE,"%8i,",DPP_DZ_Count);
					vratputf(dbgscn,2+i,22,WHITE,"%8i,",DPP_Count_at_DZ[pos]);
					vratputf(dbgscn,2+i,32,WHITE,"%8i,",DPP_Count_diff);
					vratputf(dbgscn,2+i,42,WHITE,"%6i,",DPP_Vel_ERR_Count[pos]);
					vratputf(dbgscn,2+i,50,WHITE,"%8i,",DPP_Vel_Count_upd);
					vratputf(dbgscn,2+i,60,WHITE,"%8i,",Vel_Count_upd_diff);
					vratputf(dbgscn,2+i,70,WHITE,"%7i,",vel_at_upd[pos]);
				}
				else
					vratputf(dbgscn,2+i,1,WHITE,"   ,  ,  ,         ,         ,         ,       ,         ,         ,        ,");

				pos++;
			}
			state = 2;  
			break;
		case 2:
			if (enter_key == 1)
			{
				enter_key = 0;
				if (pos >= cons[topf])
					state = 0;
				else
					state = 1;
			}
			break;
		default:
			break;
	}
	return state;
}  
#endif

void clr_windows(int16 wnum)
{
	switch (wnum)
	{
		case 0:
			if (gwindow != 0)
				vdelete(gwindow);
			gwindow = 0;
			break;
		case 1:
			if (upbox != 0)
				vdelete(upbox);
			upbox = 0;
			break;
		case 2:
			if (dnbox != 0)
				vdelete(dnbox);
			dnbox = 0;
			break;
		case 3:
			if (carbox != 0)
				vdelete(carbox);
			carbox = 0;
			break;
		case 4:
			if (arrow1 != 0)
				vdelete(arrow1);
		 	arrow1 = 0;
			break;
		case 5:
			if (arrow2 != 0)
				vdelete(arrow2);
		 	arrow2 = 0;
			break;
		case 6:
			if (arrow3 != 0)
				vdelete(arrow3);
		 	arrow3 = 0;
			break;
		case 7:
			if (arrow4 != 0)
				vdelete(arrow4);
		 	arrow4 = 0;
			break;
		case 8:
			if (arrow5 != 0)
				vdelete(arrow5);
		 	arrow5 = 0;
			break;
		case 9:
			if (arrow6 != 0)
				vdelete(arrow6);
		 	arrow6 = 0;
			break;
		case 10:
			if (arrow7 != 0)
				vdelete(arrow7);
		 	arrow7 = 0;
			break;
		case 11:
			if (arrow8 != 0)
				vdelete(arrow8);
		 	arrow8 = 0;
			break;
		case 12:
			if (nmlscn != 0)
				vdelete(nmlscn);
			nmlscn = 0;
			break;
		case 13:
			if (trascn != 0)
				vdelete(trascn);
			trascn = 0;
			break;
		case 14:
			if (dbgscn != 0)
				vdelete(dbgscn);
			dbgscn = 0;	
			break;
		case 15:
			if (menuscn != 0)
				vdelete(menuscn);
			menuscn = 0;
			break;
		case 16:
			if (popupbox != 0)
				vdelete(popupbox);
			popupbox = 0;
			break;
		default:
			break;
	}
}

void set_font(unsigned char font)
{
	wr_vga_reg(4803,font);
	/*
		Valid fonts
			11h = 8x14
			12h = 8x8
			14h = 8x16

		if font value not set as above; set to 14h
	
	*/
/*	__dpmi_regs r;

	if ((font < 0x11) || (font > 0x14))
		font = 0x14;
	r.h.ah = 0x11;
	r.h.al = font;
	r.h.bl = 0;
*/
//	__dpmi_int(0x10,&r);

}

/*void set_vmode(uint16 mode)
{
	 	   	0 = 40x25 color burst enabled
			1 = 40x25 color burst disabled
			2 = 80x25 color burst enabled
			3 = 80x25 color burst disabled
	
	__dpmi_regs r;
	
	if (mode > 3)
		mode = 3;
	r.x.ax = mode;
	__dpmi_int(0x10,&r);

}
*/
/*
void set_vert_res (unsigned char res)
{
	
		scan lines
			0 = 200
			1 = 350
			2 = 400
	
	
  __dpmi_regs r;

	if (res > 2)
		res = 2;

	r.h.ah = 0x12;
	r.h.al = res;
	r.h.bl = 0x30;
	__dpmi_int(0x10,&r);

}
*/
int16 vcreat(int16 row_len, int16 col_wid, int16 attr)
{
	int16 i;

	for (i=0;i<20;i++)
	{
		if (vid_win[i].in_use == 0)
		{
			if ((col_wid <= c_max_nmb_col) && (row_len <= c_max_nmb_row))
			{
				vid_win[i].col_wid = col_wid;
				vid_win[i].row_len = row_len;
				vid_win[i].attr = (uint8)attr;
				vid_win[i].in_use = 1;
				return i;
			}
			else
				return -1;
		}
	}
	return -1;
}

int16 valid_window (int16 v_w)
{
	if (vid_win[v_w].in_use != 1)
		return 0;
	if (vid_win[v_w].row > c_max_nmb_row)
		return 0;
	if (vid_win[v_w].row_len > c_max_nmb_row)
		return 0;
	if ((vid_win[v_w].row + vid_win[v_w].row_len) > c_max_nmb_row)
		return 0;
	if (vid_win[v_w].col > c_max_nmb_col)
		return 0;
	if (vid_win[v_w].col_wid > c_max_nmb_col)
		return 0;
	if ((vid_win[v_w].col + vid_win[v_w].col_wid) > c_max_nmb_col)
		return 0;
	return 1;
}

void vframe (int16 v_w, unsigned char attr, int16 frame_type)
{
	int16 i,start_col,start_row,end_col,end_row;
	uint8 vert_char, horz_char;
	uint8 top_left, top_right, bot_left, bot_right;

	if (valid_window(v_w) == 1) 
	{
		if (frame_type == 0)
		{
			vert_char = (uint8)c_single_col;
			horz_char = (uint8)c_single_line;
			top_left = (uint8)c_top_left;
			top_right = (uint8)c_top_right;
			bot_left = (uint8)c_bottom_left;
			bot_right = (uint8)c_bottom_right;
		}
		else
		{
			vert_char = (uint8)c_dbl_col;
			horz_char = (uint8)c_dbl_line;
			top_left = (uint8)c_dbl_top_left;
			top_right = (uint8)c_dbl_top_right;
			bot_left = (uint8)c_dbl_bottom_left;
			bot_right = (uint8)c_dbl_bottom_right;
		}
		
		start_col = vid_win[v_w].col;
		end_col = vid_win[v_w].col + vid_win[v_w].col_wid;
		start_row = vid_win[v_w].row;
		end_row = vid_win[v_w].row + vid_win[v_w].row_len;
		if (start_col > 0)
			start_col--;
		if (start_row > 0)
			start_row--;
		// draw top and bottom lines
		for(i=start_col+1;i<end_col;i++)
		{
			ScreenPutChar(horz_char, attr, i, start_row);
			ScreenPutChar(horz_char, attr, i, end_row);
		}		

		// draw left and right lines
		for(i=start_row+1;i<end_row;i++)
		{
			ScreenPutChar(vert_char, attr, start_col, i);
			ScreenPutChar(vert_char, attr, end_col, i);
		}

		ScreenPutChar(top_left, attr, start_col, start_row);
		ScreenPutChar(bot_left, attr, start_col, end_row);
		ScreenPutChar(top_right,attr, end_col, start_row);
		ScreenPutChar(bot_right, attr, end_col, end_row);	
		
	}

}


int16 vclear(int16 v_w, int16 state)
{
	static int16 start_col, start_row;
	static int16 i,j;

	switch (state)
	{
		case 0:
			if (vid_win[v_w].in_use == 1)
			{
				start_col = vid_win[v_w].col;
				start_row = vid_win[v_w].row;
				if (start_col > 0)
					start_col--;
				if (start_row > 0)
					start_row--;
				state = 1;
				i = 0;
			}
			break;
			
		case 1:
			if (i <= (vid_win[v_w].row_len + 1))
			{
				j = 0;
				while (j <= (vid_win[v_w].col_wid + 1))
				{
					ScreenPutChar(' ', vid_win[v_w].attr, start_col + j, start_row + i);
					j++;
				}
				i++;
			}
			else
			{
				state = 0;
			}
			break;
		
		default:
			state = 0;
			break;
				
	}
	return (state);
}

// Locate video window

void vlocate ( int16 v_w, int16 row, int16 col)
{
	if (vid_win[v_w].in_use == 1)
	{
		if ((row >= 0) && (col >= 0) && (row <=c_max_nmb_row) && (col<=c_max_nmb_col))
		{
			vid_win[v_w].row = row;
			vid_win[v_w].col = col;
		}
	}
}

void lcurset (int16 v_w, int16 row, int16 col)
{
	int16 new_col, new_row;
	if (vid_win[v_w].in_use == 1)
	{
		if ((row >= 0) && (col>= 0) && (row <=c_max_nmb_row) && (col<=c_max_nmb_col))
		{
			new_row = vid_win[v_w].row + row;
			new_col = vid_win[v_w].col + col;
			ScreenSetCursor(new_row, new_col);
		}
	}
}

void vdelete( int16 v_w)
{
	vid_win[v_w].in_use = 0;
	vid_win[v_w].row = 0;
	vid_win[v_w].row_len = 0;
	vid_win[v_w].col = 0;
	vid_win[v_w].col_wid = 0;

}

void vratputc(int16 v_w, int16 row, int16 col, int16 attr, char c)
{
	ScreenPutChar(c,attr,vid_win[v_w].col + col,vid_win[v_w].row + row);
}

void vratputf(int16 v_w, int16 row, int16 col, int16 attr, char *fmt,...)
{
   	va_list  args;
  	char _spfbuf[100];
 
   	va_start( args, fmt );
   	vsprintf( _spfbuf, fmt, args );
   	va_end( args );
   	ScreenPutString(_spfbuf,attr,vid_win[v_w].col + col,vid_win[v_w].row + row);
}

void init_IO_scn_lcD (void)
{
	int16 i,j;

	IO_scn_lcD.line1.nmb_items = 50;
	IO_scn_lcD.line2.nmb_items = 300;
	IO_scn_lcD.line3.nmb_items = 600;
	
	//LINE 1	
	for(i = 0; i<29; i++)
		for(j = 0; j<25; j++)
			IO_scn_lcD.line1.dpy[i][j] = Service[i][j];
	
	for(i = 29; i<IO_scn_lcD.line1.nmb_items; i++)
		for(j = 0; j<25; j++)
			IO_scn_lcD.line1.dpy[i][j] = ' ';
	
	//LINE 2
	for(i = 0; i<29; i++)
		for(j = 0; j<25; j++)
			IO_scn_lcD.line2.dpy[i][j] = Service[i][j];
	
	for(i = 29; i<IO_scn_lcD.line2.nmb_items; i++)
		for(j = 0; j<25; j++)
			IO_scn_lcD.line2.dpy[i][j] = ' ';
	
	//LINE 3
	for(i = 0; i<max_nmbflts+1; i++)
		for(j = 0; j<25; j++)
			IO_scn_lcD.line3.dpy[i][j] = LCD_Fault[i][j];
	
	for(i = max_nmbflts+1; i<IO_scn_lcD.line3.nmb_items; i++)
		for(j = 0; j<25; j++)
			IO_scn_lcD.line2.dpy[i][j] = ' ';
	
	
	
}


void calculate_IO_scn_checksums(void)
{
	unsigned char chksum_temp;
	int16 i,j,k;
	int16 nmb_chksums;


	nmb_chksums = IO_scn_lcD.line1.nmb_items / 10;// 10 string lines get one checksum
	for(i = 0; i<nmb_chksums;i++)
	{
		chksum_temp = 0;
		for(j = 0; j<10; j++)		// 10 string lines get one checksum
		{
			for(k = 0; k<25; k++)
			{
				chksum_temp += IO_scn_lcD.line1.dpy[i*10+j][k];
			}
		}
		IO_scn_lcD.line1.chksum[i] = chksum_temp;
	}
	
	nmb_chksums = IO_scn_lcD.line2.nmb_items / 10;// 10 string lines get one checksum
	for(i = 0; i<nmb_chksums;i++)
	{
		chksum_temp = 0;
		for(j = 0; j<10; j++)		// 10 string lines get one checksum
		{
			for(k = 0; k<25; k++)
			{
				chksum_temp += IO_scn_lcD.line2.dpy[i*10+j][k];
			}
		}
		IO_scn_lcD.line2.chksum[i] = chksum_temp;
	}
	
	nmb_chksums = IO_scn_lcD.line3.nmb_items / 10;// 10 string lines get one checksum
	for(i = 0; i<nmb_chksums;i++)
	{
		chksum_temp = 0;
		for(j = 0; j<10; j++)		// 10 string lines get one checksum
		{
			for(k = 0; k<25; k++)
			{
				chksum_temp += IO_scn_lcD.line3.dpy[i*10+j][k];
			}
		}
		IO_scn_lcD.line3.chksum[i] = chksum_temp;
	}
	
}

/* Revision History

  6/21/01  1.05  mhd 	1. Changed io screen names to 5 characters and seperated io screen into 
  						   multiple pages. 
						2. Copy ioname to group.ccp so hydro without video can use this table
						   without linking the video routine.
  7/27/01  1.08 mhd.	1. Place vertical line between io names on io screen.
  8/1/01  1.08 mhd		1. Changed initialization of video display: Use set_vmode, set_vert_res
						   and set_font instead of textmode().  This was done to get 28 rows for
 						   the i/o screen and to call only set_font when changing from i/o to
						   group screen.
						2. Add elevator status information on bottom of i/o screen	 
						3. Use 28 rows for minimum size group screen
  8/27/01 1.10 mhd		1. Remove rear hall call when doors open (fixed bug).
  9/12/01 1.10 mhd		2. Added status flag to display.
  9/14/01 1.10 mhd		3. Changed the call sequence for the door,fire and procf display.
  10/18/01 1.10 mhd		4. Replace cons[hydro] with Drive_Type[carnmb].
					    5. Changed Array sizes in group to six cars.
  10/25/01 1.10 mhd		6. set old_firef = -1 if doorf changes to 0.
						7. Display the firef data whenever it is not zero and the procf is
						   >=14 and <= 16.
  2/20/02 1.12 mhd		1. Added fault screen.
  3/11/02 1.14 mhd		1. Added Dispatch status on i/o screen.
  3/19/02 1.14 mhd		2. Added debounce variable prev_firef[] for video_pi routine.
  4/17/02 2.01 mhd		1. Corrected bug in Tract_Screen. Now use (doorfv == 0) instead of (doorf == 0).
  05/24/02 2.02 mhd		1. Added door open and close limit test.
  05/29/02 2.02 mhd		2. Changed fault time and day in printf call in Fault_Screen from %2i to %02i to display leading zeros.
  06/13/02 2.02 mhd		3. Use temp (float) to calculate velocity then convert temp to integer. This avoids
						   a compiler calculation error.
  06/24/02 2.03 mhd		1. Added code to display i/o's for car call lockouts.  see init_io_screen.
  06/25/02 2.03 mhd		2. Changed IOS command to set i/o's, added IOC command to clear them, RIO command to reset to normal
  						   state and RAI command to reset all i/o's to normal state.
  07/9/02 3.00 mhd		1. When using iodata array, use labels instead of numbers such as DNRD instead of 7.
  10/21/02 3.01 mhd		1. For Tract non-distance feedback controllers, use UPF and DF as speed command signals B1:B0.
						   This changes check for hsfv flag.
						2. Changed fault display to show new fault data.
						3. Deleted Car_Flts array and use Flts array for all cars.
  12/19/02 3.02 mhd		1. Added group hall call i/o screen.
						2. Setup serial hall calls.
						3. Use drive_type == 1 for hydro, drive_type == 3,4,5 for distance feedback, and deleted
						   using drive_type == 6 for non-distance feedback because there may be other cases for ndf
 						4. Show car as normal color on display when on attendant operation.
 						5. Show car as out-of-service if on emergency power and not selected.
						6. Show group service on group display.
  3/03/03 3.03 mhd		1. Added menu system for group.	 Video and menu control executed from video_display_control routine.
  4/3/03  3.04 mhd		1. Added can diagnostics software.
						2. Use hall call serial comm if car serial comm used (sercom & 3) != 0.
  06/04/03				3. Added return to lobby service.
 						4. Change PI location for special case when serial hc board used and floors > = 14 and <= 21.
 7/1/03 				5. Rotate hall call io screen if number of hc io boards > 10.
						6. Added Yellow next up car.
  8/14/03 3.06 mhd		1. Added serial i/o set and clear commands from the keyboard.
 8/26/03 				2. Added load overload display.
						3. Anded velocity variables with 0xffff to prevent overflow.
8/29/03 3.07 mhd		1. Relocate LWB and LWA when using an NDF controller.
9/5/03 3.08 mhd			1. Use video position to place car in correct column.
10/2/03 3.09 mhd		1. Use (sercom & 1) != 0 so car serial comm can be used without hall serial comm.  Also check
						   gc_hall_io_dev for serial hall i/o devices.
10/10/03 3.10 mhd		1. Modified Track_Screen so that fire service is shown only when firef > 4.
10/17/03				2. Use (procfv == 3) to set hsfv for Track_Screen on traction cars.  DF input cannot be used for 
						   DC-Scr drive cars.
 						3. In Tract_Screen, incorrectly used index of 1 instead of car_io when checking UF and DF to
                           set hsfv flag. Change index to car_io.
11/11/03 3.11 mhd		1. Used car_dat for car label number.
12/24/03 2.17 mhd		1. Change loop count on Io_Screen writing data from 4 to 6.  This is to update the screen a little faster.
2/18/04 3.19 mhd		1. Modified Tract_Screen to add Medical Emergency Service.
						2. Also in Tract_Screen, use ((servfv == 6) || (servf == 9)) when setting door status to prevent fire service door from being displayed
						   when Medical Emergency Service is active.
2/26/04 4.00 mhd		1. Moved Reset_Hoistway_Init to utils.cpp.
						2. Added Mem_dump_screen to provide a memory test means for the comm memory board.
						3. Used car_dat[] for car label on i/o screen and fault screen.	Check for car number to be equal to car_dat[i]
						   in sel_car_num routine.
4/21/04 5.00 mhd		1. Setup new io locations for 1038 board. Changed board initialization, io addresses and started io array at 0.
12/6/04 v3.36 mhd		1. Fix days_of_month index error.
1/10/05 v4.04 pn		1.  Added software to time-out video display.
						2.  Added software to change color attribute on group screen to indicate which car is the dispatcher.
3/25/05 v4.08 mhd		1. Changed timing for Group_hall_display - call Video_PI twice in the loop.	 Renamed TS3300 to Quick_Update.
						   Use Quick_Update all the time.
4/22/05 V4.12 mhd		1. Modified video pi to show doors open when gate not made and COM when comm lost to car.
4/26/05 v4.15 mhd		1. Added hall call display for lobby up hall call request.
6/22/05 V4.23 mhd		1. Added local gettime, getdate, settime setdate so that the 18.2 msec DOS interrupt could be disabled.
7/14/04 v4.25 mhd		1. Added default statement at the end of all switch case commands is there were none.
						2. Added command CMR to manually request messages (like msg 10) on the can bus (to request device error codes).
8/22/05 v4.29 mhd		1. Changed start index for can from c_can_SPB to c_can_START to add the brake board c_can_BRK.
10/31/05 v4.37 mhd		1. Added safety string status to track_screen display.
5/30/06 V4.68 mhd		1. Corrected display of gpi inputs in get_io_status.
7/7/06 v4.75 mhd		1. Change int car_dat[7] to unsigned char car_bld_no[7][2] to allow two character building car number.
						2. Change sel_car_num to increment through car building number instead of entering car number.
8/17/06 v5.02 mhd		1. Round up when sending velocity and enc_vel to display by adding 0.5 before converting value to an integer.
						2. In Init_io_Screen, use ts3300 compile flag to decrease init time.
9/27/06 v5.05 mhd		1. Modify dispatch screen to allow 8 cars.
11/08/06 v5.08 mhd		1. Added group_debug_screen.
						2. User vidpos[i] = fvars[fvvidc+82+i] instead of +88 to correct index in Init_Group_Hall_Display routine.
12/20/06 v5.10 mhd		1. Added video display DBG3 for position pulse update display.
1/8/07 v5.11 mhd		1. Changed all double data types to float for the djgpp compiler.
2/15/07 v5.18 mhd		1. Made arrays out of variables that used one bit for floor from 1 32 word to 2.
						2. Added more pages for the car and group io display.
						3. Made the floor mask large enough for 60 floors.
						4. Modified io status in group io screen.
2/23/07 v5.19 mhd		1. Corrected size declaration for bittbl from int to int16.
2/26/07					2. In group hall display, changed cons[rear] to rear_op[ghd_car_no] to display rear car call for car with rear door.
						3. Modified group display to show upto 60 floors.
6/6/07 v5.25 mhd		1. Changed car stall service on group display from AST to STL.  
						2. Added auto service timeout from group and show display of AST. 
10/8/07 v5.38 mhd		1. Added SR hall call assignments to display.
10/17/07 v5.38.1 mhd	1. Modified row index for place car, hall call and assign_call for 60 floors.
11/28/07 v5.39.4 mhd	1. Use lobby_fl variable instead of fvars[fvlob] so that lobby_fl can be changed to alternate lobby fvars[fvaltlob]. 	
12/12/07 v5.39.6 mhd	1. Added rdoort variable.
12/14/07 v5.39.7 mhd	1. Added run time variables on debug display.
12/28/07 v5.39.9 mhd	1. Display run time variable (non-zero) only when car stopped.
						2. Use nmb_hcio_brds instead of fixed value to correct for highrise cars.
						3. Added Roll_early_exit variable to detect error with short floor run.
1/7/08	v5.40.1 mhd		1. Modified elevator_service to check for servf on fire service and to show door status on earthquake service if door are closed.
						2. Modified front and rear door display to show proper door location when on fire service.  When both front and rear are closed,
						   show door closed and not rdor closed.
2/1/08 v5.40.4 mhd		1. Corrected group display for 60 floors.
2/22/08 v5.41 mhd		1. Corrected io status for hall calls from 26 to 37 for highrise cars.
6/25/08 v5.44.4 WC/mhd  1. Modify the debug_cmd function to accept byte and word length output in the mtr command
6/27/08 v5.44.4 WC/mhd  1. Modify the debug_cmd function to  allow any extra space in the mtr command message
6/30/08 v5.44.4 WC/mhd  1. Modify the debug_cmd function to accept  hex and decimal input in the mtr command .
6/19/08 v5.44.5 mhd		1. Added debug screen for delta drive serial communications.
						2. Corrected group 60 car display to show car number on right sided of display.
						3. Changed prog_flag3 to device_2.
7/22/08 v5.45.2 mhd		1. Deleted DPP_DZU_Count[], DPP_DZD_Count[] and DPP_FL_Offset[] to use DZU_Dist, DZD_Dist, Up_fl_level_dist and Dn_fl_level_dist instead.
8/20/08 V5.45.6 mhd		1. Added Security Recall as service 24.
1/14/09 v5.47.2 mhd		1. Added comm status check for gpio board 9 on non HR car in get_comm_status() routine.
2/02/09 V5.47.4 pn		1. Added Sabbath feature as service 25.
2/24/09 v5.47.6 mhd		1. Use car call output latch ccl and ccrl instead of inputs cc and ccr to show car calls on display.
3/1/09 v5.47.7 mhd		1. Added code entry for car call pushbutton security.
6/22/09 v5.48.12 hdl	1. RAV command from the keyboard will reset all field variables to defaults
2/2/10 v5.51.10 mhd		1. Delete setting the x and y index when setting car or hall calls from the CCI/UCI/DCI commands.
4/2/10 v5.52.2 mhd		1. Show door status when servf == 0 and procf >= 14 and procf <= 16.  Car level at the floor and out of service.
4/28/10 v5.52.4 jd/mhd	1. Modified Fault_Screen() case 6 to use get_pkt_status packet 36 instead of 9 and changed time-out from 40 to 100.
10/26/10 v5.52.41 mhd	1. Modified find_brdnmb() to add two additional I/O board on can bus.
11/29/10 v5.52.50 mhd	1. Add check for (Drive_Type[Car_Select] == 15) to car diagnostic display.
03/07/11 v5.53.06 hdl	1. Corrected display to allow more than 9 millions in DPP count in car io display
 
4/25/11 v5.53.15 hdl	1. Added the code to have the address of the HC boards display on the hc io display (Display_hc_io_add == 1)
7/26/11 v5.54.17 mhd	1. Corrected display of hall call board number B6 - B31. Sub 26 instead of 31.
3/21/12 v5.54.66 hdl	1. Added igep_car_man_slct_req to the data required for IGEP service
6/1/12 v5.54.85 hdl		1. Coorected group io display to properly show some of the labels for addresses above 44 (with serial hall calls)
1/4/13 v7.0.0 mhd		1. Added Riot Control service when cons[prison] == 2, servf = 28 from v5.55.15 atl_fed_pen.
8/12/13 v7.0.16 mhd		1. In Init_Group_Hall_Display_44(), check fl to be >= 1 so that invalid floor number is not displayed.
8/21/12 v7.0.16 mhd		1. Added the following changes from GALaxy 3:
						{
							as		1. When on Auto Service Time-Out service, changed "AUT" status color to blue
						 	as		1. When migrating changes from Galaxy III in debug_cmd() routine changed ENC - Encoder test command from case 56 to case 59
						 	as 		1. Modified debug_cmd() to add IRU/IRD Inconspicious Riser Up/Down Call command 
						}
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 where necessary to allow any car to be the group.
						2. Replaced iodata[1] with iodata[cons[carnmb]] where necessary.
1/7/14 v7.0.31 mhd		1. Added Car_IO_Select to select diagnostic car seperate from the car selected to send faults (also Car_Select).
3/21/14 v7.0.37 mhd		1. Display additional fault data from fault display screen.
4/4/14 v7.0.40 mhd		1. Corrected bug in fault log display.
6/26/14 v7.1.4 mhd		1. Displayed inspection service when on inspection.
8/11/14 v7.1.6 mhd	 	1. Adde trace io display.
9/8/14 v7.1.9 mhd		1. Call Trace_IO_Screen() to initialize the trace data if rotate screen is entered.
10/2/14 v7.1.11 mhd		1. Changed index k for slowdown display to k < 10 from k <= 10.
						2. Added safety processor position count.
10/6/14 v7.1.12 mhd		1. Changed fault_type variable stops[0] to next_stop because that is what it is.
10/30/14 v7,1,19 mhd	1, Inverted the output of pup, pdown, and gr1, SPB I/O's in Trace_Tract_Screen_3().
11/18/14 v7.1.20 mhd	1. Corrected NTS fault to show DnLimitF for down limit fault instead of UpLimitF and vice versa.
2/14/15 v7.1.26	mhd		1. Swapped CkLev Start and Check Start labels.
4/16/15 v7.1.30 mhd		1. Corrected pal direction display.
8/18/15 V7.1.40 MHD		1. Added GIS and GIC commands to simulate group IOs.
9/17/15 V7.1.41 mhd		1. Corrected hall call board i/o display for hall call driver boards.
11/11/15 v7.1.47 tb		1. Fixed error where hall call screen board addresses were not being displayed.
11/18/15 v7.1.48 as		1. Send packet 109 on group comm when date changed using video monitor.
11/23/15 v7.1.50 mhd	1. Changed drv_torque_cmd and drv_mtr_torque write to screen format from %5d to %6i in Trace_Track_Screen_1().
12/8/15 v7.1.53 mhd		1. Corrected trace display location for prog_flag_1, prog_flag_2 and device_2
12/21/15 v7.1.54 tb		1. Modified Grp_Io_Screen() to correctly display hall call screen board addresses and dip switch selection letter (if any).
						   Group outputs will now also display in LTCYAN text with RED for displaying 1 when an output is high.
12/30/15 v7.1.56 mhd	1. Added PAR ER service and Parameter Setup Flt to SPB service and faults.
01/05/16 v7.1.59 tb		1. Modified Init_Grp_Io_Screen() and Grp_Io_Screen() to correctly display addresses for serial hall call driver boards.

*/



