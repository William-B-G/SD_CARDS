//  Diagnostic Menus

#define d_DIAGDPY 1
#include "global.h"

// Display variables for System Status
int32 ss_statusf;
int16 ss_pos;
int16 ss_cur_flt;
int16 ss_procf;
int16 ss_servf;
int16 ss_ul;
int16 ss_dl;
int16 ss_hsf;
int16 ss_dirf;

int16 hcb_fl_off = 0;
int16 hcb_pkt_ret = 0;
int16 hcb_pkt_req = 0;

int16 dpy_system_status_buffer (int16 lcd_menu);
int16 clear_system_status_buffer (int16 lcd_menu);
int16 dpy_group_com_status (int16 lcd_menu);
int16 clear_group_com_status (int16 lcd_menu);
int16 dpy_hcb_com_status (int16 lcd_menu);
int16 reset_init_hcb_devices (int16 lcd_menu);
int16 dpy_mrcan_status(int16 lcd_menu);
int16 dpy_ctcan_status(int16 lcd_menu);
int16 dpy_grcan_status(int16 lcd_menu);
int16 clear_ctcan_status(int16 lcd_menu);
int16 clear_mrcan_status(int16 lcd_menu);
int16 clear_grcan_status(int16 lcd_menu);
void dpy_elgo_sel_data (int16 dpy_line, int16 sel_dev, void *ptr);
void dpy_selcan_data (int16 dpy_line, int16 sel_dev, void *ptr);
int16 selcan_status(int16 lcd_menu);
int16 clear_selcan_status(int16 lcd_menu);
int16 dpy_drive_com_status (int16 lcd_menu);
int16 clear_drive_com_status (int16 lcd_menu);
int16 dpy_ups_com_status (int16 lcd_menu);
int16 clear_ups_com_status (int16 lcd_menu);
int16 dpy_can_enc_com_status(int16 lcd_menu);
int16 clear_can_enc_com_status (int16 lcd_menu);

int16 diagnostics (int16 lcd_menu);


#pragma section const_type

#define Num_Diag_Menus 18
static  const char LCD_Diagnostics[Num_Diag_Menus][21] = {
  " Group Comm Status  ",	  // - 0 - 
  " Clr Grp Comm Status",	  // - 1 - 
  " Grp CAN Comm Status",	  // - 2 - 
  " Clr Grp CAN Status ",	  // - 3 - 
  " HC Brds Comm Status",	  // - 4 - 
  " Reset & Init HCBs  ",	  // - 5 - 
  " Car CAN Com Status ",	  // - 6 - 
  " Clr Car CAN Status ",	  // - 7 - 
  " MachRm CAN Com Stat",	  // - 8 - 
  " Clr MR CAN Com Stat",	  // - 9 - 
  " Sel CAN Com Stat   ",	  // - 10 - 
  " Clr Sel CAN Status ",	  // - 11 - 
  " Drive Comm Status  ",	  // - 12 - 
  " Clr Drive Comm Stat",	  // - 13 - 
  " UPS Comm Status    ",	  // - 14 - 
  " Clear UPS Comm Stat",	  // - 15 - 
  " CAN Enc Comm Status",	  // - 16 - 
  " Clr CAN Enc Status ",	  // - 17 - 
};


static  const char LCD_Clear_Sys_Stat[4][21]={
  "   Press Enter to   ",
  "Clear System Status ",
  "    System Status   ",
  "       Cleared      ",
};

static  const char LCD_No_Data[21] = {
  "   No Data Logged   "
};

static  const char LCD_Stat_Dpy[21] = {
  "     Status:OTS     "
};



static  const char LCD_GC_Com_Stat[5][21]={
  "Group to Car # 1    ",
  "Car # 1 to Group    ",
  "Internal Comm       ",
  "TxC 00000  RxC 00000",
  "Ver: 0.00  On-line=0",
};
		
static  const char LCD_Clear_GC_Com_Stat[3][21]={
  "Press Enter to Clear",
  "  Group Comm Status ",
  "       Cleared      ",
};

static  const char Alt_Device_Name[68][9] = {
"        ",
"Car # 1 ",
"Car # 2 ",
"Car # 3 ",
"Car # 4 ",
"Car # 5 ",
"Car # 6 ",
"Car # 7 ",
"Car # 8 ",
"MR Dpy  ",
"Lby Dpy ",
"Hall # 1",
"Hall # 2",
"Hall # 3",
"Hall # 4",
"Hall # 5",
"Hall # 6",
"Hall # 7",
"Hall # 8",
"Hall # 9",
"Hall #10",
"Hall #11",
"Hall #12",
"Hall #13",
"Hall #14",
"Hall #15",
"Hall #16",
"Hall #17",
"Hall #18",
"Hall #19",
"Hall #20",
"Hall #21",
"Hall #22",
"Hall #23",
"Hall #24",
"Hall #25",
"Hall #26",
"Hall #27",
"Hall #28",
"Hall #29",
"Hall #30",
"Hall #31",
"Hall #32",
"Hall # 1",
"Hall # 2",
"Hall # 3",
"Hall # 4",
"Hall # 5",
"Hall # 6",
"Hall # 7",
"Hall # 8",
"Hall # 9",
"Hall #10",
"Hall #43",
"Hall #44",
"Hall #45",
"Hall #46",
"Hall #47",
"Hall #48",
"Hall #49",
"Hall #50",
"Hall #51",
"Hall #52",
"Hall #53",
"Hall #54",
"Hall #55",
"Hall #56",
"Hall #57",
};

static  const char Device_Name[68][9] = {
"        ",
"Car # 1 ",
"Car # 2 ",
"Car # 3 ",
"Car # 4 ",
"Car # 5 ",
"Car # 6 ",
"Car # 7 ",
"Car # 8 ",
"MR Dpy  ",
"Lby Dpy ",
"Hall # 1",
"Hall # 2",
"Hall # 3",
"Hall # 4",
"Hall # 5",
"Hall # 6",
"Hall # 7",
"Hall # 8",
"Hall # 9",
"Hall #10",
"Hall #11",
"Hall #12",
"Hall #13",
"Hall #14",
"Hall #15",
"Hall #16",
"Hall #17",
"Hall #18",
"Hall #19",
"Hall #20",
"Hall #21",
"Hall #22",
"Hall #23",
"Hall #24",
"Hall #25",
"Hall #26",
"Hall #27",
"Hall #28",
"Hall #29",
"Hall #30",
"Hall #31",
"Hall #32",
"Hall #33",
"Hall #34",
"Hall #35",
"Hall #36",
"Hall #37",
"Hall #38",
"Hall #39",
"Hall #40",
"Hall #41",
"Hall #42",
"Hall #43",
"Hall #44",
"Hall #45",
"Hall #46",
"Hall #47",
"Hall #48",
"Hall #49",
"Hall #50",
"Hall #51",
"Hall #52",
"Hall #53",
"Hall #54",
"Hall #55",
"Hall #56",
"Hall #57",
};



static  const char LCD_CAN_Com_Stat[21][21]={
  "CTCAN to            ",	// 0	
  "MRCAN to            ",	// 1
  "  CPU to Selector   ",	// 2
  "GRCAN to            ",	// 3
  "TxC 00000  RxC 00000",	// 4
  "Ver: 0.00  On-line=0",	// 5
  "Can Bus Error Count ",	// 6
  "TxE 00000  RxE 00000",	// 7
  "Rx Error Cnt 00000  ",	// 8
  "Rx Data Cnt  00000  ",	// 9
  "PAL Ver: 0.00 St= Ok",	// 10
  "CPU Selector Status:",	// 11
  "CNT A:              ",	// 12
  "CNT B:              ",	// 13
  "VEL: 0000 fpm  Dir:U",	// 14
  "Error: ---          ",	// 15
  "Warning: ---        ",	// 16
  "Align: ---          ",	// 17  Use two alignment lines
  "Contrast: ---       ",	// 18
  "ClipA=  B=  Off= -- ",	// 19
  "Count:              ",	// 20
};
 

 
static const char LCD_CED_Warning[3][10] = {
	"No Warning",
	"Pos A Calc",
	"Pos B Calc",
};

static const char LCD_Contrast[4][10] = {
	"> 10:1 OK ",
	"> 1:7 Serv",
	"> 1:3 Warn",
	"< 1:3 Stop",
};

static const char LCD_CED_Error[5][13] = {
	"No Error     ",
	"Internal     ",
	"Communication",
	"Pos Invalid  ",
	"Vel Invalid  ",
};

static const char LCD_CED_Alignment[14][13] = {
	"L/R:Centered ",
	"Left:Slight  ",
	"Left:Moderate",
	"Left:At Limit",
	"Right:Slight ",
	"Right:Moderat",
	"Right:AtLimit",
	"F/N:Centered ",
	"Near:Slight  ",
	"Near:Moderate",
	"Near:At Limit",
	"Far:Slight   ",
	"Far:Moderate ",
	"Far:At Limit ",
};

static  const char LCD_Clear_CAN_Com_Stat[6][21]={
  "Press Enter to Clear",
  " Car Top Can Status ",
  "Mach Room Can Status",
  " Selector Can Status",
  "  Group Can Status  ",
  "      Cleared       ",
};

static char Can_Device_Name[17][9] = {
"ENC SEL ",	// c_ctcan_ENC_SEL
"NTS PROC", // c_mrcan_NTS
"BRAKE   ", // Not used at this time
"SAF PROC", // c_mrcan_SPB
"SELECTOR", // c_ctcan_SEL
"DOOR    ", // c_ctcan_DOOR
"RDOOR   ", // c_ctcan_RDOOR
"LW BRD  ", // c_mrcan_LW, c_ctcan_LW
"VS BRD  ", // c_mrcan_VS, c_ctcan_VS
"PI BRD  ", // c_mrcan_PI_LANT, c_ctcan_PI_LANT
"SEB #1  ", // c_mrcan_SEB_1, c_ctcan_SEB_4, c_grcan_SEB_1
"FI PI BD", // c_grcan_FI_DPY
"COP BD  ", // c_ctcan_SEB_1, c_ctcan_COP
"SEL DZ  ", // c_ctcan_SEB_2, c_ctcan_SEL_DZ
"SEL LIM ", // c_ctcan_SEB_3, c_ctcan_SEL_LIM
"HCDB #1 ", // c_grcan_HCDB_1
"HCB #1  ", // c_grcan_HCB_1
};


static  const char LCD_DRIVE_Com_Stat[3][21]={
  "   Car to Drive     ",
  "TxC 00000  RxC 00000",
  "Ver: 0.00  On-line=0",
};

static  const char LCD_Clear_DRIVE_Com_Stat[3][21]={
  "Press Enter to Clear",
  " Drive Comm Status  ",
  "      Cleared       ",
};

static  const char Drive_Name[7][12] = {
"HPV-6/900  ",
"DSD-412    ",
"Quatro-DC  ",
"Quatro-AC  ",
"CombivrtF5 ",
"VDF-VL     ",
"L1000      ",
};

static  const char LCD_UPS_Com_Stat[9][21]={
  "    Car to UPS      ",
  "TxC 00000  RxC 00000",
  "Stat:      On-line=0",
  "Battery Level:      ",
  "Stat: UPS Powered ON",
  "Stat: UPS On Battery",
  "Stat: UPS Bat Low   ",
  "Stat: UPS Bat Charge",
  "Stat: UPS Line Lost ",
};

static  const char LCD_Clear_UPS_Com_Stat[3][21]={
  "Press Enter to Clear",
  "   UPS Comm Status  ",
  "      Cleared       ",
};

static  const char LCD_CAN_ENC_Com_Stat[5][21]={
  " Car to CAN Encoder ",
  "TxC 00000  RxC 00000",
  "PPR: 00000 On-line=0",
  "Can Enc Bus Er Count",
  "TxE 00000  RxE 00000",
};

static  const char LCD_Clear_CAN_ENC_Com_Stat[3][21]={
  "Press Enter to Clear",
  "CAN Enc Comm Status ",
  "      Cleared       ",
};


static  const char LCD_HCB_Com_Stat[5][21]={
  "HC Driver  # 1      ",
  "Off-Line ... No Comm",
  "HCB Fl# 1  Ver: 0.00",
  "Waiting for Version ",
  "Ver Req Timed-out   ",
};

static  const char LCD_HCDriver_Com_Fault[9][21]={
  "       No Fault     ",
  "Rx Flt @ Top FL Loop",
  "Tx Flt @ Top FL Loop",
  "Too Few HC Bd on Bus",
  "Too Many HC Bd onBus",
  "Rx Flt @ Bot FL Loop",
  "Tx Flt @ Bot FL Loop",
  " Invalid Status Bit ",
  " Invalid Status Bit ",
};

static  const char LCD_HCB_Com_Fault[49][21]={
  "       No Fault     ",
  "Tx to Below Flr Flt ", 
  "Tx to Above Flr Flt ",
  "Rx from BelowFlr Flt",
  "Rx from AboveFlr Flt",
  " Stuck Down Button  ",
  "  Stuck Up Button   ",
  "   Down LED Open    ",
  "    Up LED Open     ",
  "  Down LED Shorted  ",
  "   Up LED Shorted   ",
  "   Down FET Open    ",
  "    Up FET Open     ",
  "  Down FET Shorted  ",
  "   Up FET Shorted   ",
  " Not a Valid Floor  ",
  "      No Comm       ",
  "    Device Reset    ",
  "  No Dn Led Board   ",
  "  No Up Led Board   ",
  "  LED Short Red Up  ",
  " LED Short Green Up ",
  " LED Short Blue Up  ",
  " LED Short Blue Dn  ",
  " LED Short Green Dn ",
  "  LED Short Red Dn  ",
  "  LED Open Red Up   ",
  " LED Open Green Up  ",
  "  LED Open Blue Up  ",
  "  LED Open Blue Dn  ",
  " LED Open Green Dn  ",
  "  LED Open Red Dn   ",
  "  FET Short Red Up  ",
  " FET Short Green Up ",
  " FET Short Blue Up  ",
  " FET Short Blue Dn  ",
  " FET Short Green Dn ",
  "  FET Short Red Dn  ",
  "  FET Open Red Up   ",
  " FET Open Green Up  ",
  "  FET Open Blue Up  ",
  "  FET Open Blue Dn  ",
  " FET Open Green Dn  ",
  "  FET Open Red Dn   ",
  " Low Supply Voltage ",
  " Up Input Overload  ",   
  " Dn Input Overload  ",   
  " Aux Up Inp Overload",   
  " Aux Dn Inp Overload",   
};


static  const char LCD_Reset_HC_Com_Stat[3][21]={
  "Hit Enter to Reset &",
  "  Init HC Stations  ",
  " Reset Command Sent ",
};

#pragma section all_types

static uint8 hex_to_ascii[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		

//***********************************************
// Display Group Com Status
//***********************************************

int16 dpy_group_com_status (int16 lcd_menu)
{
/*
	int16 i,j,k;
  	static int16 brd_ix;
  	static int16 upd_dpy;
  	static int16 line_select;
  	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		if (Dispatcher == cons[carnmb])
			brd_ix = 0;
		else
			brd_ix = cons[carnmb];
		upd_dpy = 1;
		line_select = 0;
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
		lcd_menu = 0;
		return(lcd_menu);
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		if (Dispatcher == cons[carnmb])
		{			// This is the group car
			if (gc_num_dev < 1)
				brd_ix = 0;
			else if (brd_ix < (gc_num_dev - 1)) 
				brd_ix++;
			else
				brd_ix = 0;
		}
		else
			brd_ix = cons[carnmb];
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		if (Dispatcher == cons[carnmb])
		{		 // This is the group car
			if (gc_num_dev < 1)
				brd_ix = 0;
			else if (brd_ix >= 1) 
				brd_ix--;
			else
				brd_ix = (gc_num_dev-1);
		}
		else
			brd_ix = cons[carnmb];
	}
	if (upd_dpy == 1)
	{
		upd_dpy = 0;
		if (Dispatcher == cons[carnmb])		 // This is the group car
		{
			j = gc_dev_list[brd_ix];
			k =  9;
		}
		else
		{
			j = cons[carnmb];
			k = 0;
		}

		if (j < 1)
			j = 1;


		// First Display line
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
			if (k == 9)
				LCD_Display[1][i] = LCD_GC_Com_Stat[0][i];	// Group to car
			else
				LCD_Display[1][i] = LCD_GC_Com_Stat[1][i];	// Car to group
	    }


		if (k != 0)
		{	 // Add car number to first line
			if ((cons[sercom] & 0x20) != 0)
			{
				for (i=0;i<8;i++)
					LCD_Display[1][i+k] = Alt_Device_Name[j][i];
			}
			else
			{
				for (i=0;i<8;i++)
					LCD_Display[1][i+k] = Device_Name[j][i];
			}
		}	
		   
		if (Dispatcher == j)
		{	 // Comm to the car that is the dispatcher
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[2][i] = LCD_GC_Com_Stat[2][i];		// internal comm
				LCD_Display[3][i] = LCD_GC_Com_Stat[4][i];		// version
	    	}
		}
		else
		{
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[2][i] = LCD_GC_Com_Stat[3][i];		// tx/rx count
				LCD_Display[3][i] = LCD_GC_Com_Stat[4][i];		// version
		    }
		    
			// Write comm data to display
		    itoa(gc_dev_tx_cnt[j], LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}

		    itoa(gc_dev_rx_cnt[j], LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}
		}

			// Write device version
		if ((gc_dev_ver[j]/10) == 0)
			LCD_Display[3][4] = ' ';
		else
			LCD_Display[1][4] = (char)((gc_dev_ver[j]/10) + '0');
		LCD_Display[3][5] = (char)((gc_dev_ver[j]%10) + '0');
		LCD_Display[3][7] = (char)((gc_dev_rev[j]/10) + '0');
		LCD_Display[3][8] = (char)((gc_dev_rev[j]%10) + '0');

		if (gc_dev_online[j] == 1)
			LCD_Display[3][19] = '1';		
		else
			LCD_Display[3][19] = '0';
		
	}
*/	
	return(lcd_menu);
}


//***********************************************
// Clear Group Com Status
//***********************************************

int16 clear_group_com_status (int16 lcd_menu)
{
/*
	int16 i;
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Clear_GC_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Clear_GC_Com_Stat[1][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Clear_GC_Com_Stat[1][i];
			LCD_Display[2][i] = LCD_Clear_GC_Com_Stat[2][i];
	    }
		for(i=0;i<c_com_no_dev;i++)
		{
			gc_dev_tx_cnt[i] = 0;
			gc_dev_rx_cnt[i] = 0;
			gc_loss_cnt[i] = 0;
		}
		gc_unpack_error = 0;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
	  	lcd_menu = 0;
		return(lcd_menu);
	}
*/	
	return(lcd_menu);
}


//***********************************************
// Display Hall Call Board Com Status
//***********************************************

int16 dpy_hcb_com_status (int16 lcd_menu)
{
/*
	int16 i,j;
  	static int16 hcb_sel;
  	static int16 flt_ix;
  	static int16 next_flt;
  	static int16 brd_ix;
  	static int16 upd_dpy;
  	static int16 lcd_dvr_ix;

	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		hcb_sel = 0;
		flt_ix = 0;
		brd_ix = 0;
		next_flt = 1;
		upd_dpy = 1;
		put_pkt_req(75,c_grcan_HCDB_1,GCANCOM);
		put_pkt_req(76,c_grcan_HCDB_1,GCANCOM);
		hcb_pkt_req = 1;
		hcb_pkt_ret = 0;
	}
	
	lcd_dvr_ix = 0;
	if ((grcan_dev_list[brd_ix] >= c_grcan_HCDB_1) && (grcan_dev_list[brd_ix] <= c_grcan_HCDB_10))
		lcd_dvr_ix = (int16)(grcan_dev_list[brd_ix] - c_grcan_HCDB_1);
	
	
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
		next_flt = 1;
		if (hcb_sel == 0)
		{
			put_pkt_req(75,lcd_dvr_ix + c_grcan_HCDB_1,GCANCOM);
			put_pkt_req(76,lcd_dvr_ix + c_grcan_HCDB_1,GCANCOM);
			hcb_pkt_req = 1;
			hcb_pkt_ret = 0;
		}

	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
		lcd_menu = 0;
		return(lcd_menu);
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
		upd_dpy = 0;

		if (hc_dvr_brd[lcd_dvr_ix].online == 1)
		{
			j = 0;
			while (j < 24)
			{
				hcb_sel++;
				if (hcb_sel > 24)
				{
					flt_ix = 0;
					hcb_sel = 0;
					next_flt = 1;
					break;
				}
				else if (hc_dvr_brd[lcd_dvr_ix].fault[hcb_sel-1] != 15)
				{
			    	upd_dpy = 1;
					break;
				}
				j++;
			}
		}
		else
			hcb_sel = 0;

		if (hcb_sel == 0)
		{
			brd_ix++;
			if (brd_ix >= nmb_dvr_brds)
				brd_ix = 0;
			i = brd_ix;
			while ((i < nmb_dvr_brds) && (upd_dpy == 0))
			{
				if ((grcan_dev_list[i] >= c_grcan_HCDB_1) && (grcan_dev_list[i] <= c_grcan_HCDB_10))
				{
					brd_ix = i;
					put_pkt_req(75,grcan_dev_list[i],GCANCOM);
					put_pkt_req(76,grcan_dev_list[i],GCANCOM);
					hcb_pkt_req = 1;
					hcb_pkt_ret = 0;
		    		upd_dpy = 1;
				}
				i++;
			}
		}
		if ((grcan_dev_list[brd_ix] >= c_grcan_HCDB_1) && (grcan_dev_list[brd_ix] <= c_grcan_HCDB_10))
			lcd_dvr_ix = (int16)(grcan_dev_list[brd_ix] - c_grcan_HCDB_1);
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
		upd_dpy = 0;

		if (hcb_sel == 0)
		{
			brd_ix--;
			if (brd_ix < 0)
				brd_ix = nmb_dvr_brds-1;
			i = brd_ix;
			while ((i >= 0) && (upd_dpy == 0))
			{
				if ((grcan_dev_list[i] >= c_grcan_HCDB_1) && (grcan_dev_list[i] <= c_grcan_HCDB_10))
				{
					brd_ix = i;
					put_pkt_req(75,grcan_dev_list[i],GCANCOM);
					put_pkt_req(76,grcan_dev_list[i],GCANCOM);
					hcb_pkt_req = 1;
					hcb_pkt_ret = 0;
		    		upd_dpy = 1;
				}
				i--;
			}
		}

		if ((grcan_dev_list[brd_ix] >= c_grcan_HCDB_1) && (grcan_dev_list[brd_ix] <= c_grcan_HCDB_10))
			lcd_dvr_ix = (int16)(grcan_dev_list[brd_ix] - c_grcan_HCDB_1);
		
		if (hc_dvr_brd[lcd_dvr_ix].online == 1)
		{
			j = 0;
			while (j < 24)
			{
				hcb_sel--;
				if (hcb_sel < 0)
				{
					hcb_sel = 24;
					if (hc_dvr_brd[lcd_dvr_ix].fault[hcb_sel-1] != 15)
					{
			    		upd_dpy = 1;
						break;
					}
				}
				else if (hcb_sel == 0)
				{
					flt_ix = 0;
					next_flt = 1;
					put_pkt_req(75,lcd_dvr_ix + c_grcan_HCDB_1,GCANCOM);
					put_pkt_req(76,lcd_dvr_ix + c_grcan_HCDB_1,GCANCOM);
					hcb_pkt_req = 1;
					hcb_pkt_ret = 0;
			    	upd_dpy = 1;
					break;
				}
				else if (hc_dvr_brd[lcd_dvr_ix].fault[hcb_sel-1] != 15)
				{
		    		upd_dpy = 1;
					break;
				}
				j++;
			}
		}
		else
			hcb_sel = 0;
	}

	if (hc_dvr_brd[lcd_dvr_ix].online == 1)
	{

		if ((hcb_pkt_req == 1) && (hcb_pkt_ret == 0))
		{ 
			hcb_pkt_ret = get_pkt_status(75,lcd_dvr_ix + c_grcan_HCDB_1,GCANCOM,40);
			if ((hcb_pkt_ret == 1) || (hcb_pkt_ret == 2))
			{
				hcb_pkt_req = 0;
				upd_dpy = 1;
			}
		}
	}

	if (upd_dpy == 1)
	{
		upd_dpy = 0;
		if (hc_dvr_brd[lcd_dvr_ix].online == 1)
		{
			if (hcb_sel == 0)
			{
				if (hcb_pkt_ret == 1)
				{
					if ((hc_dvr_brd[lcd_dvr_ix].status_1) != 0)
					{
						if (next_flt != 0)
						{
							next_flt = 0;
							for(i=0;i<=5;i++)
							{
								if (flt_ix >= 6)
									flt_ix = 1;
								else
									flt_ix++; 
								if ((hc_dvr_brd[lcd_dvr_ix].status_1 & bittbl[flt_ix-1]) != 0)
									break;
							}
						}
					}
					else
						flt_ix = 0;
				    for(i=0; i<=19; i++)
					{
				    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
						LCD_Display[1][i] = LCD_HCB_Com_Stat[0][i];
						LCD_Display[2][i] = LCD_HCDriver_Com_Fault[flt_ix][i];
						LCD_Display[3][i] = ' ';
					}
				}
				else if (hcb_pkt_ret == 0)
				{
				    for(i=0; i<=19; i++)
					{
				    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
						LCD_Display[1][i] = LCD_HCB_Com_Stat[0][i];
						LCD_Display[2][i] = LCD_HCB_Com_Stat[3][i];
						LCD_Display[3][i] = ' ';
					}
				}
				else if (hcb_pkt_ret == 2)
				{
				    for(i=0; i<=19; i++)
					{
				    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
						LCD_Display[1][i] = LCD_HCB_Com_Stat[0][i];
						LCD_Display[2][i] = LCD_HCB_Com_Stat[4][i];
						LCD_Display[3][i] = ' ';
					}
				}

				for (i=4;i<8;i++)
					LCD_Display[0][i+7] = Alt_Device_Name[lcd_dvr_ix + c_grcan_HCDB_1][i];
			}
			else
			{
			    for(i=0; i<=19; i++)
				{
		    		LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
					LCD_Display[1][i] = LCD_HCB_Com_Stat[2][i];
					LCD_Display[2][i] = LCD_HCB_Com_Fault[hc_dvr_brd[lcd_dvr_ix].fault[hcb_sel-1]][i];
					LCD_Display[3][i] = ' ';
				}

				if (cons[topf] <= 24)
					hcb_fl_off = 0;
				else if (cons[topf] <= 48)
				{
					if ((hcb_sel % 2) == 1)
						hcb_fl_off = 24;
					else
						hcb_fl_off = 0;
				}
				else
				{		// greater than 48 floors
					if ((hcb_sel % 3) == 1)
						hcb_fl_off = 24;
					else if ((hcb_sel % 3) == 2)
						hcb_fl_off = 48;
					else
						hcb_fl_off = 0;
				}
	  		    LCD_Display[1][7] = flmrk[hcb_sel + hcb_fl_off][0];
	  		    LCD_Display[1][8] = flmrk[hcb_sel + hcb_fl_off][1];

				LCD_ver = hc_dvr_brd[lcd_dvr_ix].version[hcb_sel-1];
				LCD_rev = hc_dvr_brd[lcd_dvr_ix].revision[hcb_sel-1];

				if ((LCD_ver/10) == 0)
					LCD_Display[1][15] = ' ';
				else
					LCD_Display[1][15] = (char)((LCD_ver/10) + '0');
				LCD_Display[1][16] = (char)((LCD_ver%10) + '0');
				LCD_Display[1][18] = (char)((LCD_rev/10) + '0');
				LCD_Display[1][19] = (char)((LCD_rev%10) + '0');

			}
		}
		else
		{
		    for(i=0; i<=19; i++)
			{
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_HCB_Com_Stat[0][i];
				LCD_Display[2][i] = LCD_HCB_Com_Stat[1][i];
				LCD_Display[3][i] = ' ';
			}


			for (i=4;i<8;i++)
				LCD_Display[1][i+7] = Alt_Device_Name[lcd_dvr_ix + c_grcan_HCDB_1][i];

		}
	}
*/	
	return(lcd_menu);
}


//***********************************************
// Reset and Init Hall Call Board Devices
//***********************************************

int16 reset_init_hcb_devices (int16 lcd_menu)
{
/*
	int16 i;
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Reset_HC_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Reset_HC_Com_Stat[1][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Reset_HC_Com_Stat[1][i];
			LCD_Display[2][i] = LCD_Reset_HC_Com_Stat[2][i];
	    }
	    send_hc_dvr_req(0);
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
	  	lcd_menu = 0;
		return(lcd_menu);
	}
*/	
	return(lcd_menu);
}




//************************************
// Car Top Communications Status
//************************************
int16 dpy_ctcan_status(int16 lcd_menu)
{
/*		
	int16 i,j,k;
	char *c;
	static int16 brd_ix;
	static int16 upd_dpy;
	static int16 status_select;

	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		brd_ix = 0;
		upd_dpy = 1;
		status_select = 1;
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
		ModeButton = 1;
		lcd_menu = 0;
		return(lcd_menu);
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		if (status_select == 1)
		{
			status_select = 0;
			brd_ix = 0;
		}
		else
		{
			if (nmb_ctcan_dev < 1)
				brd_ix = 0;
			else if (brd_ix < (nmb_ctcan_dev - 1)) 
				brd_ix++;
			else 
			{
				status_select = 1;
				brd_ix = 0;
			}
		}
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		if (status_select == 1)
		{
			status_select = 0;
			brd_ix = (nmb_ctcan_dev - 1);
		}
		else
		{
			if (nmb_ctcan_dev < 1)
				brd_ix = 0;
			else if (brd_ix >= 1) 
				brd_ix--;
			else
			{
				status_select = 1;
				brd_ix = (nmb_ctcan_dev - 1);
			}
		}
	}
	if (upd_dpy == 1)
	{
		upd_dpy = 0;
		if (status_select == 1)
		{
			// First Display line
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_CAN_Com_Stat[6][i];	// Can Error count 
				LCD_Display[2][i] = LCD_CAN_Com_Stat[7][i];	// Can error count
				LCD_Display[3][i] = ' ';
		    }
			// Write comm data to display
			itoa(ctcan_tx_err_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}

			itoa(ctcan_rx_err_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}
		}
		else
		{
			j = ctcan_dev_list[brd_ix];
			if ((j < c_ctcan_START) || (j > c_ctcan_END))
				j = c_ctcan_START;
			
			k =  j;
			// First Display line
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_CAN_Com_Stat[0][i];	// Car TO SEB 
				LCD_Display[2][i] = LCD_CAN_Com_Stat[4][i]; // Can tx/rx Count
				LCD_Display[3][i] = LCD_CAN_Com_Stat[5][i]; // Can version/online
		    }
				// Add Can device name to first line
			
			c = &Can_Device_Name[0][0];
			
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
				LCD_Display[1][i+9] = c[i];
				
			if (j >= c_ctcan_SEB_4)
			{
				itoa((j-c_ctcan_SEB_1)+1,LCD_String);
				LCD_Display[1][14] = LCD_String[0];
				if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
					LCD_Display[1][15] = LCD_String[1];
			}
			
			// Comm Status to external CAN devices
				// Write comm data to display
		    itoa(ctcan_dev[k].tx_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}
		    itoa(ctcan_dev[k].rx_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}

				// Write device version
			if ((ctcan_dev[k].version/10) == 0)
				LCD_Display[3][4] = ' ';
			else
				LCD_Display[3][4] = (char)((ctcan_dev[k].version/10) + '0');
			LCD_Display[3][5] = (char)((ctcan_dev[k].version%10) + '0');
			LCD_Display[3][7] = (char)((ctcan_dev[k].revision/10) + '0');
			LCD_Display[3][8] = (char)((ctcan_dev[k].revision%10) + '0');
				// Write on-line status
			if (ctcan_dev[k].online == 1)
				LCD_Display[3][19] = '1';		
			else
				LCD_Display[3][19] = '0';
		}
	}
*/	
	return(lcd_menu);
}

//************************************
// Clear Car Top Communications Status
//************************************
int16 clear_ctcan_status(int16 lcd_menu)
{	
/*	
	int16 i;
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Clear_CAN_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Clear_CAN_Com_Stat[1][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Clear_CAN_Com_Stat[1][i];
			LCD_Display[2][i] = LCD_Clear_CAN_Com_Stat[5][i];
	    }
		for(i=0;i<c_max_ctcan_dev;i++)
		{
			ctcan_dev[i].rx_cnt = 0;
			ctcan_dev[i].tx_cnt = 0;
		}
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
		ModeButton = 1;
		lcd_menu = 0;
		return(lcd_menu);
	}
*/	
	return(lcd_menu);
}

//*****************************************
//  Machine Room Communications Status
//*****************************************

int16 dpy_mrcan_status(int16 lcd_menu)
{		
/*
 	int16 i,j,k;
 	char *c;
 	static int16 brd_ix;
	static int16 upd_dpy;
	static int16 status_select;
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		brd_ix = 0;
		upd_dpy = 1;
		status_select = 1;
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
		ModeButton = 1;
		lcd_menu = 0;
		return(lcd_menu);
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		if (status_select == 1)
		{
			status_select = 0;
			brd_ix = 0;
		}
		else
		{
			if (nmb_mrcan_dev < 1)
				brd_ix = 0;
			else if (brd_ix < (nmb_mrcan_dev - 1)) 
				brd_ix++;
			else 
			{
				status_select = 1;
				brd_ix = 0;
			}
		}
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		if (status_select == 1)
		{
			status_select = 0;
			brd_ix = (nmb_mrcan_dev-1);
		}
		else
		{
			if (nmb_mrcan_dev < 1)
				brd_ix = 0;
			else if (brd_ix >= 1) 
				brd_ix--;
			else
			{
				status_select = 1;
				brd_ix = (nmb_mrcan_dev - 1);
			}
		}
	}
	if (upd_dpy == 1)
	{
		upd_dpy = 0;
		if (status_select == 1)
		{
			// First Display line
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_CAN_Com_Stat[6][i];	// Can Error count 
				LCD_Display[2][i] = LCD_CAN_Com_Stat[7][i];	// Can error count
				LCD_Display[3][i] = ' ';
		    }
			// Write comm data to display
			itoa(mrcan_tx_err_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}
	
			itoa(mrcan_rx_err_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}
		}
		else
		{
			j = mrcan_dev_list[brd_ix];
			if ((j < c_mrcan_START) || (j > c_mrcan_END))
				j = c_mrcan_START;
			
			k =  j;
			// First Display line
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_CAN_Com_Stat[1][i];	// MRCAN TO SEB 
				LCD_Display[2][i] = LCD_CAN_Com_Stat[4][i]; // Can tx/rx Count
				LCD_Display[3][i] = LCD_CAN_Com_Stat[5][i]; // Can version/online
		    }
				// Add Can device name to first line
 			
 			c = &Can_Device_Name[1][0];

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
				LCD_Display[1][i+9] = c[i];
				
			if (j >= c_mrcan_SEB_1)
			{
				itoa((j-c_mrcan_SEB_1)+1,LCD_String);
				LCD_Display[1][14] = LCD_String[0];
				if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
					LCD_Display[1][15] = LCD_String[1];
			}
			   
			// Second Display line
		    itoa(mrcan_dev[k].tx_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}
			
		    itoa(mrcan_dev[k].rx_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}

			// Write device version
			if ((mrcan_dev[k].version/10) == 0)
				LCD_Display[3][4] = ' ';
			else
				LCD_Display[1][4] = (char)((mrcan_dev[k].version/10) + '0');
			LCD_Display[3][5] = (char)((mrcan_dev[k].version%10) + '0');
			LCD_Display[3][7] = (char)((mrcan_dev[k].revision/10) + '0');
			LCD_Display[3][8] = (char)((mrcan_dev[k].revision%10) + '0');
				// Write on-line status
			if (mrcan_dev[k].online == 1)
				LCD_Display[3][19] = '1';		
			else
				LCD_Display[3][19] = '0';
		}
	}
*/	
	return(lcd_menu);
}


//***********************************************
// Clear Local Machine Room Communications Status
//***********************************************

int16 clear_mrcan_status(int16 lcd_menu)
{		

 	int16 i;
 	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Clear_CAN_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Clear_CAN_Com_Stat[2][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Clear_CAN_Com_Stat[2][i];
			LCD_Display[2][i] = LCD_Clear_CAN_Com_Stat[5][i];
	    }
		for(i=0;i<c_max_mrcan_dev;i++)
		{
			mrcan_dev[i].rx_cnt = 0;
			mrcan_dev[i].tx_cnt = 0;
		}
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
		ModeButton = 1;
		lcd_menu = 0;
		return(lcd_menu);
	}
	
	return(lcd_menu);
}

void dpy_elgo_sel_data (int16 dpy_line, int16 sel_dev, void *ptr)
{
/*
	int16 i;
 	int16 dpy_msg;
	
	struct SEL_can_type *sel_canx = (struct SEL_can_type *)ptr;
	
	if (dpy_line == 0)
	{
		// First Display line
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_CAN_Com_Stat[11][i];	// Header	 
			LCD_Display[1][i] = LCD_CAN_Com_Stat[20][i];	// Count A 
			LCD_Display[2][i] = LCD_CAN_Com_Stat[14][i];	// Velocity and Direction					
			LCD_Display[3][i] = ' '; 
	    }
		if (sel_dev == 1)
		{
			LCD_Display[0][0] = 'N'; 
			LCD_Display[0][1] = 'T'; 
			LCD_Display[0][2] = 'S'; 
		}

		if (sel_dev == 0)
			itoa(sel_canx->pos_cnt_A, LCD_String);  // Put out current selector A count
		else
			itoa(sel_canx->pos_cnt_B, LCD_String);  // Put out current selector A count
		i=0;
		while(((LCD_String[i] >= '0') && (LCD_String[i] <= '9')) || (LCD_String[i] == '-'))
		{
		  	LCD_Display[1][(i+7)] = LCD_String[i];
		  	i++;
		}
		while((i+7) < 15)
		{
		  	LCD_Display[1][(i+7)] = ' ';
		  	i++;
		}
			
		
		itoa((int32)((float)(sel_canx->vel) * 0.182879),LCD_String);  // original data in 1mm/s (6.0/32.8084)
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
		  	LCD_Display[2][(i+5)] = LCD_String[i];
		  	i++;
		}
		while((i+5) < 9)
		{
		  	LCD_Display[2][(i+5)] = ' ';
		  	i++;
		}
			if (sel_canx->vel_dir == 1)
			LCD_Display[2][19] = 'U';
		else if (sel_canx->vel_dir == 2)
			LCD_Display[2][19] = 'D';
		else
			LCD_Display[2][19] = 'N';
			
	}
	else if (dpy_line == 1)
	{
		// First Display line
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_CAN_Com_Stat[11][i];	// Header	 
			LCD_Display[1][i] = LCD_CAN_Com_Stat[15][i]; 	// Error
			LCD_Display[2][i] = LCD_CAN_Com_Stat[16][i];	// Warnings 
			LCD_Display[3][i] = ' ';  
	    }
			
		if (sel_dev == 1)
		{
			LCD_Display[0][0] = 'N'; 
			LCD_Display[0][1] = 'T'; 
			LCD_Display[0][2] = 'S'; 
		}

		if (sel_canx->online == 1)
		{
   			dpy_msg = 0;
    		if (sel_canx->Error_Warning.B.INTERNAL == 1)
    			dpy_msg = 1;
    		else if (sel_canx->Error_Warning.B.COMM == 1)
    			dpy_msg = 2;
    		else if (sel_canx->Error_Warning.B.POS_INVALID == 1)
    			dpy_msg = 3;
    		else if (sel_canx->Error_Warning.B.VEL_INVALID == 1)
    			dpy_msg = 4;
    		
			for (i=0; i<=12; i++)		// Write Error Message by priority
				LCD_Display[1][i+7] = LCD_CED_Error[dpy_msg][i];
    			dpy_msg = 0;
    		if (sel_canx->Error_Warning.B.POS_A_EX == 1)
    			dpy_msg = 1;
    		else if (sel_canx->Error_Warning.B.POS_B_EX == 1)
    			dpy_msg = 2;
    		
			// Display Warning
			for (i=0; i<=9; i++)		// Warning message
				LCD_Display[2][i+9] = LCD_CED_Warning[dpy_msg][i];
			
		}
	}
*/	
}


void dpy_selcan_data (int16 dpy_line, int16 sel_dev, void *ptr)
{
	int16 i;
 	int16 dpy_msg;
	
	struct SEL_can_type *sel_canx = (struct SEL_can_type *)ptr;
	
	if (dpy_line == 0)
	{
		// First Display line
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_CAN_Com_Stat[11][i];	// Header	 
			LCD_Display[1][i] = LCD_CAN_Com_Stat[12][i];	// Count A 
			LCD_Display[2][i] = LCD_CAN_Com_Stat[13][i]; 	// Count B
			LCD_Display[3][i] = LCD_CAN_Com_Stat[14][i];	// Velocity and Direction					
	    }

		if (sel_dev == 1)
		{
			LCD_Display[0][0] = 'N'; 
			LCD_Display[0][1] = 'T'; 
			LCD_Display[0][2] = 'S'; 
		}

		if (sel_dev == 0)
		{
			
			itoa(sel_canx->pos_cnt_A, LCD_String);  // Put out current selector A count
			i=0;
			while(((LCD_String[i] >= '0') && (LCD_String[i] <= '9')) || (LCD_String[i] == '-'))
			{
			  	LCD_Display[1][(i+7)] = LCD_String[i];
			  	i++;
			}
			while((i+7) < 15)
			{
			  	LCD_Display[1][(i+7)] = ' ';
			  	i++;
			}
		}
		else
		{
		  	LCD_Display[1][7] = '-';
		  	LCD_Display[1][8] = '-';
		  	LCD_Display[1][9] = '-';
		}
		
		if ((sel_dev == 1) || (sel_can.ver > 1))
		{
			
			itoa(sel_canx->pos_cnt_B, LCD_String);  // Put out current selector B count
			i=0;
			while(((LCD_String[i] >= '0') && (LCD_String[i] <= '9')) || (LCD_String[i] == '-'))
			{
			  	LCD_Display[2][(i+7)] = LCD_String[i];
			  	i++;
			}
			while((i+7) < 15)
			{
			  	LCD_Display[2][(i+7)] = ' ';
			  	i++;
			}
		}
		else
		{
		  	LCD_Display[2][7] = '-';
		  	LCD_Display[2][8] = '-';
		  	LCD_Display[2][9] = '-';
		}
		
		itoa((int32)((float)(sel_canx->vel) * 0.182879),LCD_String);  // original data in 1mm/s (6.0/32.8084)
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
		  	LCD_Display[3][(i+5)] = LCD_String[i];
		  	i++;
		}
		while((i+5) < 9)
		{
		  	LCD_Display[3][(i+5)] = ' ';
		  	i++;
		}
			if (sel_canx->vel_dir == 1)
			LCD_Display[3][19] = 'U';
		else if (sel_canx->vel_dir == 2)
			LCD_Display[3][19] = 'D';
		else
			LCD_Display[3][19] = 'N';
			
	}
	else if (dpy_line == 1)
	{
		// First Display line
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_CAN_Com_Stat[11][i];	// Header	 
			LCD_Display[1][i] = LCD_CAN_Com_Stat[15][i]; 	// Error
			LCD_Display[2][i] = LCD_CAN_Com_Stat[16][i];	// Warnings 
			LCD_Display[3][i] = LCD_CAN_Com_Stat[19][i];  	// Clip and clip offset
	    }
			
		if (sel_dev == 1)
		{
			LCD_Display[0][0] = 'N'; 
			LCD_Display[0][1] = 'T'; 
			LCD_Display[0][2] = 'S'; 
		}

		if (sel_canx->online == 1)
		{
   			dpy_msg = 0;
    		if (sel_canx->Error_Warning.B.INTERNAL == 1)
    			dpy_msg = 1;
    		else if (sel_canx->Error_Warning.B.COMM == 1)
    			dpy_msg = 2;
    		else if (sel_canx->Error_Warning.B.POS_INVALID == 1)
    			dpy_msg = 3;
    		else if (sel_canx->Error_Warning.B.VEL_INVALID == 1)
    			dpy_msg = 4;
    		
			for (i=0; i<=12; i++)		// Write Error Message by priority
				LCD_Display[1][i+7] = LCD_CED_Error[dpy_msg][i];
    			dpy_msg = 0;
    		if (sel_canx->Error_Warning.B.POS_A_EX == 1)
    			dpy_msg = 1;
    		else if (sel_canx->Error_Warning.B.POS_B_EX == 1)
    			dpy_msg = 2;
    		
			// Display Warning
			for (i=0; i<=9; i++)		// Warning message
				LCD_Display[2][i+9] = LCD_CED_Warning[dpy_msg][i];
				
			if (sel_canx->Status.B.CLIP_A == 1)
				LCD_Display[3][6] = '1';
			else
				LCD_Display[3][6] = '0';
			
			if (sel_canx->ver > 1)
			{
				if (sel_canx->Status.B.CLIP_B == 1)
					LCD_Display[3][10] = '1';
				else
					LCD_Display[3][10] = '0';
				
				sprintf(LCD_String, "%4d", sel_canx->clip_offset);
				for (i=0; i<4; i++)		// clip offset
					LCD_Display[3][i+16] = LCD_String[i];
			}
			else
			{
				LCD_Display[3][10] = '0';
			}
			
		}
	}
	else if (dpy_line == 2)
	{
			// First Display line
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_CAN_Com_Stat[11][i];	// Header	 
			LCD_Display[1][i] = LCD_CAN_Com_Stat[17][i];	// L/R Alignment 
			LCD_Display[2][i] = ' ';						// F/N Alignment
			LCD_Display[3][i] = LCD_CAN_Com_Stat[18][i]; 	// Contrast
	    }
			
		if (sel_dev == 1)
		{
			LCD_Display[0][0] = 'N'; 
			LCD_Display[0][1] = 'T'; 
			LCD_Display[0][2] = 'S'; 
		}

		if ((sel_canx->online == 1) && ((sel_canx->Error_Warning.R & 0x0F) == 0))
		{
        			dpy_msg = 0;
    		if (sel_canx->Status.B.LR_ALIGN != 0)
    		{
    			dpy_msg = sel_canx->Status.B.LR_ALIGN;
    			if (sel_canx->Status.B.RIGHT == 1)
    				dpy_msg += 3;
    		}
				// Alignment 
			for (i=0; i<=12; i++)		// Left/Right Alignment
				LCD_Display[1][i+7] = LCD_CED_Alignment[dpy_msg][i];
				
    			dpy_msg = 7;
    		if (sel_canx->Status.B.NF_ALIGN != 0)
    		{
    			dpy_msg += sel_canx->Status.B.NF_ALIGN;
    			if (sel_canx->Status.B.FAR == 1)
    				dpy_msg += 3;
    		}
    		
			for (i=0; i<=12; i++)		// Far/Near Alignment
				LCD_Display[2][i+7] = LCD_CED_Alignment[dpy_msg][i];
				
			for (i=0; i<=9; i++)		// Contrast
				LCD_Display[3][i+10] = LCD_Contrast[(sel_canx->Error_Warning.B.CONTRAST) & 0x03][i];
		}
	}
}

//*****************************************
//  Selector Can Communications Status
//*****************************************

int16 selcan_status(int16 lcd_menu)
{		

 	int16 i;
 	int16 menu_end;
 	int16 menu_mid;
	static int16 upd_dpy;
	static int16 status_sel;
	struct SEL_can_type *sel_canx; 
	
	if (sel_can.ver == 3)
	{
		menu_mid = 3;
		menu_end = 5;
	}
	else
	{
		menu_mid = 4;
		menu_end = 7;
	}
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		upd_dpy = 1;
		status_sel = 0;
		NTS_spi.Command2.B.RST_CNT = 0;
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
		ModeButton = 1;
		lcd_menu = 0;
		return(lcd_menu);
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
	    status_sel ++;
	    if (status_sel > menu_end)
	    	status_sel = 0;
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
	    status_sel --;
	    if (status_sel < 0)
	    	status_sel = menu_end;
	}


	if (upd_dpy == 1)
	{
		upd_dpy = 1;

		if ((status_sel == 0) || (status_sel == menu_mid))
		{
			
			// First Display line
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = LCD_CAN_Com_Stat[2][i];	 
				LCD_Display[1][i] = LCD_CAN_Com_Stat[8][i];	// Can Rx Error Count 
				LCD_Display[2][i] = LCD_CAN_Com_Stat[9][i]; // Can rx Data Count
				LCD_Display[3][i] = LCD_CAN_Com_Stat[5][i]; // Can version/online
		    }

			if (status_sel == 0)
				sel_canx = &sel_can;
			else
			{
				NTS_spi.can_req = 1;
				LCD_Display[0][2] = 'N'; 
				LCD_Display[0][3] = 'T'; 
				LCD_Display[0][4] = 'S'; 
				sel_canx = &NTS_sel;
			}
			
			if (status_sel == 0)
			{
				
				itoa(sel_can_rx_err_cnt, LCD_String);
				i=0;
				while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
				{
					LCD_Display[1][(i+13)] = LCD_String[i];
					i++;
				}
				while((i+13) <= 17)
				{
					LCD_Display[1][(i+13)] = ' ';
					i++;
				}
			}
			else
			{
				LCD_Display[1][13] = '-'; 
				LCD_Display[1][14] = '-'; 
				LCD_Display[1][15] = '-'; 
				LCD_Display[1][16] = ' '; 
				LCD_Display[1][17] = ' '; 
			}
			
			// Display Receive Count
		    itoa((uint16)(sel_canx->rx_cnt), LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+13)] = LCD_String[i];
				i++;
			}
			while((i+13) <= 17)
			{
				LCD_Display[2][(i+13)] = ' ';
				i++;
			}

			// Display version/revision and online status
				// Write device version
				
			if ((sel_canx->ver/10) == 0)
				LCD_Display[3][4] = ' ';
			else
				LCD_Display[3][4] = (char)((sel_canx->ver/10) + '0');
			LCD_Display[3][5] = (char)((sel_canx->ver%10) + '0');
			LCD_Display[3][6] = '.';
			LCD_Display[3][7] = '0';
			LCD_Display[3][8] = ' ';
			
			if (sel_canx->online == 1)
				LCD_Display[3][19] = '1';		
			else
				LCD_Display[3][19] = '0';
		}
		else
		{
			if (sel_can.ver == 3)
			{
				if ((status_sel >= 1) && (status_sel <= 2))
					dpy_elgo_sel_data(status_sel-1,0,&sel_can);
				else if ((status_sel >= 4) && (status_sel <= 5))
				{
					NTS_spi.can_req = 1;
					dpy_elgo_sel_data(status_sel-4,1,&NTS_sel);
				}
			}
			else
			{
				if ((status_sel >= 1) && (status_sel <= 3))
					dpy_selcan_data(status_sel-1,0,&sel_can);
				else if ((status_sel >= 5) && (status_sel <= 7))
				{
					NTS_spi.can_req = 1;
					dpy_selcan_data(status_sel-5,1,&NTS_sel);
				}
			}
		}
	}

	return(lcd_menu);
}


//***********************************************
// Clear Safety Can Communications Status
//***********************************************

int16 clear_selcan_status(int16 lcd_menu)
{		

 	int16 i;
 	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Clear_CAN_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Clear_CAN_Com_Stat[3][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Clear_CAN_Com_Stat[3][i];
			LCD_Display[2][i] = LCD_Clear_CAN_Com_Stat[5][i];
	    }
	    sel_can.rx_cnt = 0;
	    sel_can.tx_cnt = 0;
	    NTS_sel.rx_cnt = 0;
	    NTS_sel.tx_cnt = 0;
		NTS_spi.Command2.B.RST_CNT = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
		ModeButton = 1;
		lcd_menu = 0;
		NTS_spi.Command2.B.RST_CNT = 0;
		return(lcd_menu);
	}
	return(lcd_menu);
}

//*****************************************
// Group Can Communications Status
//*****************************************

int16 dpy_grcan_status(int16 lcd_menu)
{		

 	int16 i,j,k;
 	char *c;
  	static int16 brd_ix;
	static int16 upd_dpy;
	static int16 status_select;
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		brd_ix = 0;
		upd_dpy = 1;
		status_select = 1;
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
		ModeButton = 1;
		lcd_menu = 0;
		return(lcd_menu);
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		if (status_select == 1)
		{
			status_select = 0;
			brd_ix = 0;
		}
		else
		{
			if (nmb_grcan_dev < 1)
				brd_ix = 0;
			else if (brd_ix < (nmb_grcan_dev - 1)) 
				brd_ix++;
			else 
			{
				status_select = 1;
				brd_ix = 0;
			}
		}
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		if (status_select == 1)
		{
			status_select = 0;
			brd_ix = (nmb_grcan_dev-1);
		}
		else
		{
			if (nmb_grcan_dev < 1)
				brd_ix = 0;
			else if (brd_ix >= 1) 
				brd_ix--;
			else
			{
				status_select = 1;
				brd_ix = (nmb_grcan_dev - 1);
			}
		}
	}
	if (upd_dpy == 1)
	{
		upd_dpy = 0;
		if (status_select == 1)
		{
			// First Display line
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_CAN_Com_Stat[6][i];	// Can Error count 
				LCD_Display[2][i] = LCD_CAN_Com_Stat[7][i];	// Can error count
				LCD_Display[3][i] = ' ';
		    }
			// Write comm data to display
			itoa(grcan_tx_err_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}
	
			itoa(grcan_rx_err_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}
		}
		else
		{
			j = grcan_dev_list[brd_ix];
			if ((j < c_grcan_START) || (j > c_grcan_END))
				j = c_grcan_START;
			
			k =  j;
			// First Display line
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_CAN_Com_Stat[3][i];	// GRCAN TO SEB 
				LCD_Display[2][i] = LCD_CAN_Com_Stat[4][i]; // Can tx/rx Count
				LCD_Display[3][i] = LCD_CAN_Com_Stat[5][i]; // Can version/online
		    }
				// Add Can device name to first line

			c = &Can_Device_Name[8][0];

			if ((cons[sercom] & 0x01) != 0)
			{	// Individual Serial Hall Call boards
				if (j == c_grcan_VS)
					c = &Can_Device_Name[8][0];
				else if (j == c_grcan_FI_DPY)
					c = &Can_Device_Name[9][0];
				else if ((j >= c_grcan_SEB_1) && (j <= c_grcan_SEB_12))
					c = &Can_Device_Name[10][0];
				else if (j >= c_grcan_HCB_1)
					c = &Can_Device_Name[16][0];
					
				for (i=0;i<=7;i++)
					LCD_Display[1][i+9] = c[i];
					
				if ((j >= c_grcan_SEB_1) && (j <= c_grcan_SEB_12))
				{
					itoa((j-c_grcan_SEB_1)+1,LCD_String);
					LCD_Display[1][14] = LCD_String[0];
					if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
						LCD_Display[1][15] = LCD_String[1];
				}
				else if (j >= c_grcan_HCB_1)
				{
					itoa((j-c_grcan_HCB_1)+1,LCD_String);
					LCD_Display[1][14] = LCD_String[0];
					if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
						LCD_Display[1][15] = LCD_String[1];
				}
				
			}
			else
			{		// Standard I/O or Hall Call driver boards
				if (j == c_grcan_VS)
					c = &Can_Device_Name[8][0];
				else if (j == c_grcan_FI_DPY)
					c = &Can_Device_Name[9][0];
				else if ((j >= c_grcan_SEB_1) && (j <= c_grcan_SEB_40))
					c = &Can_Device_Name[10][0];
				else if (j >= c_grcan_HCDB_1)
					c = &Can_Device_Name[15][0];
					
				for (i=0;i<=7;i++)
					LCD_Display[1][i+9] = c[i];
					
				if ((j >= c_grcan_SEB_1) && (j <= c_grcan_SEB_40))
				{
					itoa((j-c_grcan_SEB_1)+1,LCD_String);
					LCD_Display[1][14] = LCD_String[0];
					if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
						LCD_Display[1][15] = LCD_String[1];
				}
				else if (j >= c_grcan_HCDB_1)
				{
					itoa((j-c_grcan_HCDB_1)+1,LCD_String);
					LCD_Display[1][17] = LCD_String[0];
					if ((LCD_String[1] >= '0') && (LCD_String[1] <= '9'))
						LCD_Display[1][18] = LCD_String[1];
				}
				
			}
			   
		    itoa(grcan_dev[k].tx_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}
		
		    itoa(grcan_dev[k].rx_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}

			// Write device version
			if ((grcan_dev[k].version/10) == 0)
				LCD_Display[3][4] = ' ';
			else
				LCD_Display[3][4] = (char)((grcan_dev[k].version/10) + '0');
			LCD_Display[3][5] = (char)((grcan_dev[k].version%10) + '0');
			LCD_Display[3][7] = (char)((grcan_dev[k].revision/10) + '0');
			LCD_Display[3][8] = (char)((grcan_dev[k].revision%10) + '0');
				// Write on-line status
			if (grcan_dev[k].online == 1)
				LCD_Display[3][19] = '1';		
			else
				LCD_Display[3][19] = '0';
		}
	}
	return(lcd_menu);
}


//***********************************************
// Clear Group Can Communications Status
//***********************************************

int16 clear_grcan_status(int16 lcd_menu)
{		

 	int16 i;
 	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Clear_CAN_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Clear_CAN_Com_Stat[4][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Clear_CAN_Com_Stat[4][i];
			LCD_Display[2][i] = LCD_Clear_CAN_Com_Stat[5][i];
	    }
		for(i=0;i<c_max_grcan_dev;i++)
		{
			grcan_dev[i].rx_cnt = 0;
			grcan_dev[i].tx_cnt = 0;
		}
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
		ModeButton = 1;
		lcd_menu = 0;
		return(lcd_menu);
	}
	return(lcd_menu);
}

#if (Traction == 1)

//***********************************************
// Display Drive Can Communications Status
//***********************************************

int16 dpy_drive_com_status (int16 lcd_menu)
{
	int16 i,j;
	int16 upd_dpy;

	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		upd_dpy = 1;
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
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
	if (upd_dpy == 1)
	{
		upd_dpy = 0;
		// First Display line
	    for(i=0; i<=19; i++)
	    {
	    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
			LCD_Display[1][i] = LCD_DRIVE_Com_Stat[0][i];	
			LCD_Display[2][i] = LCD_DRIVE_Com_Stat[1][i];
			LCD_Display[3][i] = LCD_DRIVE_Com_Stat[2][i];
	    }

		if (Drive_Type[cons[carnmb]] == 10)
			j = 2;			// Quatro DC
		else if (Drive_Type[cons[carnmb]] == 11)
			j = 3;			// Quatro AC
		else if (Drive_Type[cons[carnmb]] == 12) 
			j = 4;			// KEB Combivert F5
		else if (Drive_Type[cons[carnmb]] == 14)
			j = 5;			// Delta Drive
		else if (Drive_Type[cons[carnmb]] == 15)
			j = 6;			// Yaskawa L1000
		else if	((Drive_Type[cons[carnmb]] == 5) || (Drive_Type[cons[carnmb]] == 8))
			j = 1;			// DSD 412
		else
			j = 0;		   // HPV 900 or 600

		// Add Drive name to first line
		for (i=0;i<10;i++)
			LCD_Display[1][i+12] = Drive_Name[j][i];
		   

		// Write comm data to display
	    itoa(drive_trm_cnt, LCD_String);
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[2][(i+4)] = LCD_String[i];
			i++;
		}
		while((i+4) <= 9)
		{
			LCD_Display[2][(i+4)] = ' ';
			i++;
		}

	    itoa(drive_rcv_cnt, LCD_String);
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[2][(i+15)] = LCD_String[i];
			i++;
		}
		while((i+15) <= 19)
		{
			LCD_Display[2][(i+15)] = ' ';
			i++;
		}

		uiu.dw = drv_status;
		LCD_Display[3][5] = hex_to_ascii[(uiu.b[lb] & 0xf0) >> 4];
		LCD_Display[3][5] |= hex_to_ascii[uiu.b[lb] & 0x0f];
		LCD_Display[3][6] = hex_to_ascii[(uiu.b[mlb] & 0xf0) >> 4];
		LCD_Display[3][6] |= hex_to_ascii[uiu.b[mlb] & 0x0f];
		LCD_Display[3][7] = hex_to_ascii[(uiu.b[mhb] & 0xf0) >> 4];
		LCD_Display[3][7] |= hex_to_ascii[uiu.b[mhb] & 0x0f];
		LCD_Display[3][8] = hex_to_ascii[(uiu.b[hb] & 0xf0) >> 4];
		LCD_Display[3][8] |= hex_to_ascii[uiu.b[hb] & 0x0f];

/*
		// Write device version
		if ((drive_version/10) == 0)
			LCD_Display[3][6] = ' ';
		else
			LCD_Display[1][6] = (char)(drive_version/10) + '0';
		LCD_Display[3][7] = (char)(drive_version%10) + '0';
		LCD_Display[3][9] = (char)(drive_revision/10) + '0';
		LCD_Display[3][10] = (char)(drive_revision%10) + '0';
*/
		// Write on-line status
		if (drive_online == 1)
			LCD_Display[1][19] = '1';		
		else
			LCD_Display[1][19] = '0';
		}
	}
	return(lcd_menu);
}


//***********************************************
// Clear Drive Can Communications Status
//***********************************************

int16 clear_drive_com_status (int16 lcd_menu)
{
	int16 i;
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Clear_DRIVE_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Clear_DRIVE_Com_Stat[1][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Clear_DRIVE_Com_Stat[1][i];
			LCD_Display[2][i] = LCD_Clear_DRIVE_Com_Stat[2][i];
	    }
		drive_rcv_cnt = 0;
		drive_trm_cnt = 0;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
	  	lcd_menu = 0;
		return(lcd_menu);
	}
	return(lcd_menu);
}


//***********************************************
// Display UPS Communications Status
//***********************************************

int16 dpy_ups_com_status (int16 lcd_menu)
{
	int16 i;
	int16 upsbit[6] = {0x08,0x10,0x40,0x80,0x20};
	static int16 line_select;
	static int16 upd_dpy;
	

	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		upd_dpy = 1;
		line_select = 0;
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
		lcd_menu = 0;
		return(lcd_menu);
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		line_select++;
		while (line_select <= 5)
		{
			if ((ups_bat_status & upsbit[line_select]) != 0)
				break;
			else
				line_select++;
		}

		if (line_select > 5)
			line_select = 0; 
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		line_select--;

		while (line_select > 0)
		{
			if ((ups_bat_status & upsbit[line_select]) != 0)
				break;
			else
				line_select--;
		}

		if (line_select < 0)
			line_select = 5;
	}
	if (upd_dpy == 1)
	{
		upd_dpy = 0;
		// First Display line
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[0][i] = LCD_UPS_Com_Stat[0][i];	//  
			LCD_Display[1][i] = LCD_UPS_Com_Stat[1][i];
			LCD_Display[2][i] = LCD_UPS_Com_Stat[2][i];
			LCD_Display[3][i] = LCD_UPS_Com_Stat[line_select+3][i];
 	  	}

	    itoa(ups_com_tx_cnt, LCD_String);
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[1][(i+4)] = LCD_String[i];
			i++;
		}
		while((i+4) <= 9)
		{
			LCD_Display[1][(i+4)] = ' ';
			i++;
		}

	    itoa(ups_com_rx_cnt, LCD_String);
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[1][(i+15)] = LCD_String[i];
			i++;
		}
		while((i+15) <= 19)
		{
			LCD_Display[1][(i+15)] = ' ';
			i++;
		}

		// Write device status
		LCD_Display[2][6] = hex_to_ascii[(ups_bat_status & 0xf0) >> 4];
		LCD_Display[2][6] |= hex_to_ascii[ups_bat_status & 0x0f];

		// Write on-line status
		if (ups_com_online == 1)
			LCD_Display[2][19] = '1';		
		else
			LCD_Display[2][19] = '0';
		

		if (line_select == 0)
		{
		    for(i=0; i<=4; i++)
				LCD_Display[3][i+14] = bat_cap[i];
			LCD_Display[3][19] = '%';
		}
	}
	return(lcd_menu);
}


//***********************************************
// Clear UPS Communications Status
//***********************************************

int16 clear_ups_com_status (int16 lcd_menu)
{
	int16 i;
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Clear_UPS_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Clear_UPS_Com_Stat[1][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Clear_UPS_Com_Stat[1][i];
			LCD_Display[2][i] = LCD_Clear_UPS_Com_Stat[2][i];
	    }
		ups_com_rx_cnt = 0;
		ups_com_tx_cnt = 0;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
		lcd_menu = 0;
		return(lcd_menu);
	}
	return(lcd_menu);
}


//***********************************************
// Display Can Encoder Communications Status
//***********************************************

int16 dpy_can_enc_com_status(int16 lcd_menu)
{
	int16 i;
	static int16 status_select;
	static int16 upd_dpy;
	

	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
		upd_dpy = 1;
		status_select = 1;
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{				// Update display
	    EnterButton = 1;
	    upd_dpy = 1;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
		lcd_menu = 0;
		return(lcd_menu);
	}
	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	{
	    UpButton = 1;  // incriment
	    upd_dpy = 1;
		if (status_select == 1)
			status_select = 0;
		else
			status_select = 1;
	}
	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	{
	    DownButton = 1;  // decriment
	    upd_dpy = 1;
		if (status_select == 1)
			status_select = 0;
		else
			status_select = 1;
	}
	if (upd_dpy == 1)
	{
		upd_dpy = 0;

		if (status_select == 1)
		{
			// First Display line
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_CAN_ENC_Com_Stat[3][i];	// Car TO CAN encoder 
				LCD_Display[2][i] = LCD_CAN_ENC_Com_Stat[4][i];	// Car TO CAN encoder
				LCD_Display[3][i] = ' ';
		    }
			// Write comm data to display
			itoa(enc_can_tx_err_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}

			itoa(enc_can_rx_err_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}
		}
		else
		{
			// First Display line
		    for(i=0; i<=19; i++)
		    {
		    	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
				LCD_Display[1][i] = LCD_CAN_ENC_Com_Stat[0][i];	// Car TO CAN Encoder 
				LCD_Display[2][i] = LCD_CAN_ENC_Com_Stat[1][i];
				LCD_Display[3][i] = LCD_CAN_ENC_Com_Stat[2][i];
		    }

			// Comm Status to external CAN devices
		    itoa(can_enc_tx_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[2][(i+4)] = ' ';
				i++;
			}

		    itoa(can_enc_rx_cnt, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[2][(i+15)] = LCD_String[i];
				i++;
			}
			while((i+15) <= 19)
			{
				LCD_Display[2][(i+15)] = ' ';
				i++;
			}
			
			// Write encoder ppr
		    itoa(can_enc_ppr, LCD_String);
			i=0;
			while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
			{
				LCD_Display[3][(i+4)] = LCD_String[i];
				i++;
			}
			while((i+4) <= 9)
			{
				LCD_Display[3][(i+4)] = ' ';
				i++;
			}

			// Write on-line status
			if (can_enc_online == 1)
				LCD_Display[3][19] = '1';		
			else
				LCD_Display[3][19] = '0';
		}
	}
}

//***********************************************
// Clear Can Encoder Communications Status
//***********************************************

int16 clear_can_enc_com_status (int16 lcd_menu)
{
	int16 i;
	
	if(LCD_Sub_Init == 1)	// first time through 
	{
	    LCD_Sub_Init = 0;
	    for(i=0; i<=19; i++)
	    {
		   	LCD_Display[0][i] = getcaps(LCD_Diagnostics[lcd_menu-1][i]);
	  		LCD_Display[1][i] = LCD_Clear_CAN_ENC_Com_Stat[0][i];
			LCD_Display[2][i] = LCD_Clear_CAN_ENC_Com_Stat[1][i];
			LCD_Display[3][i] = ' ';
	    }
	}
	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	{
	    EnterButton = 1;
	    for(i=0; i<=19; i++)
	    {
			LCD_Display[1][i] = LCD_Clear_CAN_ENC_Com_Stat[1][i];
			LCD_Display[2][i] = LCD_Clear_CAN_ENC_Com_Stat[2][i];
	    }
	    can_enc_rx_cnt = 0;
	    can_enc_tx_cnt = 0;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  
		lcd_menu = 0;
		return(lcd_menu);
	}
	return(lcd_menu);
}
#endif

//***********************************************
// Diagnostics Display
//***********************************************

int16 diagnostics (int16 lcd_menu)
{
	int16 fv_line = 0;
	int16 start_ptr;
	int16 line_ptr;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 last_menu;
	
  	static int16 line_select;
  	static int16 status_select;

  	int16 i,j;
  	

	if(LCD_Init == 1)		// first time through procedure
 	{
 	    LCD_Init = 0;
 	    menu_ptr = 0;
 	    menu_level = 0;
 	    upd_dpy = 1;

	    // Build the menu list
		j = 0;
 	    for (i=0;i<Num_Diag_Menus;i++)
 	    {
			if ((((cons[sercom] & 0x20) == 0) || (Dispatcher != cons[carnmb])) && ((i == 4) || (i == 5)))
				i = 6;	// no serial hall call driver boards
			
			if ((cons[sel_type] != 4) && ((i == 10) || (i == 11)))
				i = 12;		// no CAN selector
 	    	
			if (((cons[sercom] & 8) == 0) && ((i == 12) || (i == 13)))
		  		i = 14;	// No Drive comm display
			
			if (((cons[PWL_Brake] != 1) && (cons[UPS_Type] == 0)) && ((i == 14) || (i == 15)))
				i = 16;	 // no PWL UPS comm
			
			if ((cons[sel_type] != 3) && ((i == 16) || (i == 17)))
				i = 18;		// no CAN encoder
				
			if (i >= Num_Diag_Menus)
				break;  // Have to break here if the last menu is not used
			else
			{
	 	    	Menu_list[j] = (int8)i;	
	 	    	j++;
			}
 	    }
 	    
 	    last_menu = j - 1;
 	    for (i=j;i<Num_Diag_Menus;i++)
 	    	Menu_list[i] = Num_Diag_Menus;
 	}
  
	if (menu_level == 0)
	{			// Rotate through Diagnostic menu items
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
		  	menu_level = (int16)(Menu_list[menu_ptr] + 1);
		  	LCD_Sub_Init = 1;		// for menus that do not need menu[2]
			return(lcd_menu);
		}
		if(upd_dpy == 1)
		{
			upd_dpy = 0;
			line_ptr = (menu_ptr % 3);
			start_ptr = menu_ptr - line_ptr;
		    for(i=0; i<=19; i++)
		    {
	  			LCD_Display[0][i] = xLCD_Main_Menu[14][i];
			  	LCD_Display[1][i] = LCD_Diagnostics[Menu_list[start_ptr]][i];
			  	if ((start_ptr + 1) > last_menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else 
			  	{
				  	LCD_Display[2][i] = LCD_Diagnostics[Menu_list[start_ptr+1]][i];
			  		if ((start_ptr + 2) > last_menu)
				  		LCD_Display[3][i] = ' ';
		    		else
					  	LCD_Display[3][i] = LCD_Diagnostics[Menu_list[start_ptr+2]][i];
			  	}
		    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}

	if (menu_level == 1)
	{		// Group Communications Status
		menu_level = dpy_group_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
	if (menu_level == 2)
	{		// Clear Group Communications Status
		menu_level = clear_group_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 3)
	{
		menu_level = dpy_grcan_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
	if (menu_level == 4)
	{
		menu_level = clear_grcan_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
	if (menu_level == 5)
	{		// HC Boards Communication Status
		menu_level = dpy_hcb_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
	if (menu_level == 6)
	{		// Reset and Initialize Hall Call Devices
		menu_level = reset_init_hcb_devices(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 7)
	{
		menu_level = dpy_ctcan_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 8)
	{
		menu_level = clear_ctcan_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 9)
	{
		menu_level = dpy_mrcan_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 10)
	{
		menu_level = clear_mrcan_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 11)
	{
		menu_level = selcan_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}

	if (menu_level == 12)
	{
		menu_level = clear_selcan_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
#if ((Traction == 1) || (Tract_OL == 1))
	if (menu_level == 13)
	{		// DRIVE Communications Status
		menu_level = dpy_drive_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
	if (menu_level == 14)
	{		// Clear Drive Communications Status
		menu_level = clear_drive_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
#endif
#if ((Traction == 1) || (Tract_OL == 1))
	if (menu_level == 15)
	{		// UPS Communications Status
		menu_level = dpy_ups_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
	if (menu_level == 16)
	{		// Clear UPS Communications Status
		menu_level = clear_ups_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
#endif
#if (Traction == 1)
	if (menu_level == 17)
	{		// Can Encoder Communications Status
		menu_level = dpy_can_enc_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
	if (menu_level == 18)
	{		// Clear Can Encoder Communications Status
		menu_level = clear_can_enc_com_status(menu_level);
		if (menu_level == 0)
			upd_dpy = 1;
	}
#endif
	return(lcd_menu);
}

/*
Revision History
8/16/17 V8.0.8 mhd		1. Corrected hall call board label and index for individual serial hall call boards.

*/