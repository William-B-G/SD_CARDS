// KEB drive data access file
#define d_KEBDRV 1
#include "global.h"


// Drive parameter variables
static int16 Drv_Par_Grp_Ptr;
static int16 Drv_Par_Ptr;
static int32 Drv_Par;
static int16 drv_par_req;
static int16 drv_par_ret;
static int16 In_Drv_Grp;
static int16 Mod_Drv_Par;
static int16 flash_disable;

#pragma section const_type

//__declspec(section ".rodata")

const char keb_drv_par_grp_tbl[14][25] = {
  "LF 02-05 Init and Learn ",
  "LF 08-19 Motor Data     ",
  "LF 20-25 Machine Data   ",
  "LF 26-30 Encoder Data   ",
  "LF 31-38 Speed Control  ",
  "LF 41-57 Pattern Control",
  "LF 57-79 Misc. Functions",
  "LF 80-99 Diag. Functions",
  "US 01-10 Configuration  ",
  "US 16-37 Advanced Func. ",
  "Ld 18-33 Adv. Elev. Drv.",
  "LP 01-23 Positioning    ",
  "di 00-03 Digital Inputs ",
  "do 42-83 Digital Outputs",
};

const char par_range[25] = {
  "  Invalid #:            "};

const char lf02_txt[7][25] = {	 	// 1	Operating Mode
  "0=Invalid Mode          ",
  "1=AbSPd:Abs. Analog Spd ",
  "2=d SPd:Digital Speed   ",
  "3=A tor:Analog Torque   ",
  "4=A SPd:Analog Speed    ",
  "5=SErSP:Serial Speed    ",
  "6=BiSPd:Binary Speed    ",
};
 
const char lf03_txt[7][25] = {	 	// 2	Drive configuration
  "0=run:Run Operation     ",
  "1=conF:Configuration    ",
  "2=Econ:E-Config(Stopped)",
  "3=S Lrn:Start Learn     ",
  "4=S Lrn:Learn Inertia   ",
  "5=S Lrn:Learn Enc Pos   ",
  "6=SPI:Lrn Enc No Motion ",
};

const char lf04_txt[5][25] = {	 // 3		 Drive Mode
  "0=Invld:Invalid Config  ",
  "1=ICLSd:IM Closed Loop  ",
  "2=IGLSS:IM Grless Cl Lp ",
  "3=PCLSd:PM Closed Loop  ",
  "4=PGLSS:PM Grless Cl Lp ",
};

const char on_off_txt[2][25] = {	 	// 4  General On Off
  "0 = Off                 ",
  "1 = On                  ",
};

const char lf26_0_txt[32][25] = {	 // 5	Encoder Type
  "00=noInt:No Card Install",	// 0
  "01=Incremental Input    ",   // 1
  "02=Incremental Output   ",	// 2
  "03=Incremental I/O      ",	// 3
  "04=Incremental I/O Dir  ",	// 4
  "05=Init:Initiator       ",   // 5   
  "06=SSI:Sync. Ser Intface",	// 6
  "07=rESoL:Resolver       ",	// 7
  "08=Tach:Tachometer      ",   // 8
  "09=Incremental Output   ",   // 9
  "10=Incremental Output   ",   // 10
  "11=HIPEr:Hiperface      ",	// 11
  "12=Inc24:15-24V HTL Incr",	// 12
  "13=IncIE:5V TTL Incr.   ",	// 13
  "14=SinCo:Sine Cosine/SSI",	// 14
  "15=I24PE:15-24V HTL Incr",	// 15
  "16=EndAt:EnDat          ",	// 16
  "17=I24IE:Inc 24 + Er Det",   // 17
  "18=Analog +/- 10V       ",   // 18
  "18=rESoL:Resolver       ",	// 19
  "20=SSI:SSI-Sin/Cos      ",   // 20
  "21=OVSL:Overspd Limiter ",   // 21
  "22=PhASE:UVW            ",   // 22
  "23=Inc30:Incr Out 10-30V",   // 23
  "24=Inc30:Incr Out 10-30V",   // 24
  "25=OVSL:Overspd Lim HTL ",   // 25
  "26=Inc5:Inc 5V TTL ExSup",	// 26
  "27=I24PE:Inc 24 + Er Det",	// 27
  "28=EBIS:EnDat 2.2 Biss  ",   // 28
  "29=Inc:Incr. Out TTL    ",   // 29
  "30=Unrc:Unrecognized Enc",	// 30
  "31=noInt:No Interface   ",   // 31
};

									//   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
const unsigned char lf26_1_xref [66] = { 0, 0, 1,21,21,21,21, 2,21,21,21,21,21,21,21,21, 3, 4, 5,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
									//     33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,
									       21, 6,21,21,21,21, 7,21,21,21,21,21,21,21,21, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,21,21,21,22 };

const char lf26_1_txt[23][25] = {	 // 6	   Encoder Connection Type
  "noEnc:No Encoder Detect ",		// 0	  0
  "SCS67:SCS 60/70         ",		// 2	  1
  "SCn67:SCM 60/70         ",		// 7	  2
  "SinCo:Sin/Cos no absolut",		// 16	  3
  "SinCo:Sin/Cos absolute  ",	    // 17	  4
  "SSI:SSI Absolute        ",		// 18	  5
  "SrS56:SRS 50/60         ",		// 34	  6
  "Srn56:SRM 50/60         ",		// 39	  7
  "EnDat:EnDat             ",		// 48	  8
  "EnDat:EnDat Single Turn ",		// 49	  9
  "EnDat:SKS 36            ", 		// 50	  10
  "EnDat:EnDat Linear      ",		// 51	  11
  "EnDat:EnDat Multi Turn  ",		// 52	  12
  "EnDat:EnDat22 Sngl Turn ",		// 53	  13
  "EnDat:EnDat22 Mult Turn ",		// 54	  14
  "EnDat:SKM 36            ", 		// 55	  15
  "EnDat:EnDat22 Linear    ",		// 56	  16
  "BEnD:BiSS Hengstler St  ",		// 57	  17
  "BEnD:BiSS Hengstler Mt  ",		// 58	  18
  "BEnD:BiSS Mode C St     ",		// 59	  19
  "BEnD:BiSS Mode C Mt     ",		// 60	  20
  "EncUn:Encoder Undefined ",		// 64	  21
  "noEnc:No Encoder Detect ",       // 65     22 (code 255 from drive)
};

									//   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
const unsigned char lf26_2_xref [101] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
									//     33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
									        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
									//     65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96
									        0, 0, 3, 4, 5, 6, 7, 0, 0, 8, 9,10,11,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,13, 0, 0, 0,14,
									// 	   97,98, 99,100
									       15,16, 17, 18  };

const char lf26_2_txt[19][25] = {	 // 7	  Encoder Connection Status
  "UKN:Unknown Error       ",		// 0	   0
  "conn: Serial Com. Estab ",		// 16	   1
  "EncId:Unknown Encoder ID",		// 64	   2
  "bdCbi:Cbl Break Incrm Ch",		// 67	   3
  "bdCba:Cbl Break Absol Ch",		// 68	   4
  "PoSde:Pos Deviation High",		// 69	   5
  "BdPPr:Enc PPR No Match  ",		// 70	   6
  "BdInt:Bad Interf. Card  ",		// 71	   7
  "bdSuP:Bad Power Supply  ",		// 74	   8
  "OHEnc:Enc. Over Heated  ",		// 75	   9
  "OSEnc:Encoder Over Speed",		// 76	   10
  "ErEnc:Enc Intern Failure",		// 77	   11
  "ErEnc:Internal Failure  ",		// 78	   12
  "FrtEn:Formatting Enc.   ",		// 92	   13
  "nEEnc:New Encoder Found ",		// 96	   14
  "noFrt:Mem. Not Formated ",		// 97	   15
  "EncBs:Encoder Busy      ",		// 98	   16
  "EncEr: 000000 EncoderErr",		// 128	   17
  "OFF:No Com. to Encoder  ",		// 255	   18
};

const char lf98_99_txt[129][25] = {	 // 8  Drive Fault and Status
  "noP:No Operation        ",		// 0
  "E.OP:DC Bus Over Voltage",		// 1
  "E.UP:DC Bus Under Volt  ",		// 2
  "E.Uph:Phase Failure     ",		// 3
  "E.OC:Overcurrent        ",		// 4
  "E.5:Output Phase Failure",		// 5 
  "E.OHI:Overheat Internal ",		// 6
  "E.nOHI:No Overheat Intrn",		// 7
  "E.OH:Overheat Pwr Module",		// 8
  "E.dOH:Drive Overheat    ",		// 9
  "10 = Reset Fault History",		// 10
  "E.ndOH:No Drive Overheat",		// 11
  "E.PU:Error Power Unit   ",		// 12
  "no PU:Pwr Unit Not Ready",		// 13
  "E.PUIN:Power Unit Inv   ",		// 14
  "E.LSF:Load Shunt Fault  ",		// 15
  "E.OL:Error Overload     ",		// 16
  "E.nOL:No Error Overload ",		// 17
  "E.buS:Error Bus         ",		// 18
  "E.OL2:Error Overload 2  ",		// 19
  "E.nOL2:No Err Overload 2",		// 20
  "E.EEP:EEPPOM Defective  ",		// 21
  "E.PUCC:Power Unit Comm  ",		// 22
  "E.SbuS:Error Bus Sync   ",		// 23
  "E.24:Error Max. Accel   ",		// 24
  "E.25:Error Spd Ctrl Lmt ",		// 25
  "E.26:                   ",		// 26
  "E.27:                   ",		// 27
  "E.28:                   ",		// 28
  "E.29:                   ",		// 29
  "E.OH2:Motor Protection  ",		// 30
  "E.EF:External Fault     ",		// 31
  "E.EnC1:Error Encoder 1  ",		// 32
  "E.PFC:Power Factor Ctrl ",		// 33
  "E.EnC2:Error Encoder 2  ",		// 34
  "E.EnCC:Encoder Change   ",		// 35 
  "E.nOH:No Overheat PwrMod",		// 36
  "E.37:                   ",		// 37
  "E.38:                   ",		// 38
  "E.Set:Error Set         ",		// 39
  "E.40:                   ",		// 40
  "E.41:                   ",		// 41
  "E.42:                   ",		// 42
  "E.43:                   ",		// 43
  "E.SLF:Software Limit Fwd",		// 44
  "E.SLr:Software Limit Rev",		// 45
  "E.PrF:Protect Rotate Fwd",		// 46
  "E.Prr:Protect Rotate Rev",		// 47
  "E.48:                   ",		// 48
  "E.Puci:Pwr Unit Code Inv",		// 49
  "E.Puch:Power Unit Change",		// 50
  "E.dri:Error Driver Relay",		// 51
  "E.Hyb:Error Hybrid      ",		// 52
  "E.iEd:Input Error Detect",		// 53
  "E.co1:Counter Overrun 1 ",		// 54
  "E.co2:Counter Overrun 2 ",		// 55
  "E.br:Error Brake        ",		// 56
  "E.ini:Error Init MFC    ",		// 57
  "E.OS:Error Overspeed    ",		// 58
  "E.HybC:Err Hybrid Change",		// 59
  "E.Cdd:Err Calc Drv Data ",		// 60
  "E.61:                   ",		// 61
  "E.62:                   ",		// 62
  "E.63:                   ",		// 63
  "Facc:Forward Accel      ",		// 64
  "Fdec:Forward Decel      ",		// 65
  "Fcon:Forward Constant   ",		// 66
  "rAcc:Reverse Accel      ",		// 67
  "rdEc:Reverse Decel      ",		// 68
  "rCon:Reverse Constant   ",		// 69
  "LS:Low Speed            ",		// 70
  "SLL:Stall               ",		// 71
  "LAS:LA Stop             ",		// 72
  "LdS:Ld Stop             ",		// 73
  "SSF:Speed Search        ",		// 74
  "dcb:DC Brake            ",		// 75
  "bbL:Base Block          ",		// 76
  "dLS:Low Speed DC Brake  ",		// 77
  "POFF:Power Off          ",		// 78
  "StOP:Quick Stop         ",		// 79
  "HCL:Hardware Current Lmt",		// 80
  "SrA:Spd Srch Ref Active ",		// 81
  "Cdd:Calculating Drv Data",		// 82
  "POSI	Positioning        ",		// 83
  "PLS:Low Speed Power Off ",		// 84
  "bon:Close Brake         ",		// 85
  "boFF:Open Brake         ",		// 86
  "A.OHI:AStop Overheat Int",		// 87
  "A.nOH:No Overheat PwrMod",		// 88
  "A.OH:Overheat Power Mod ",		// 89
  "A.EF:AStop Extern Fault ",		// 90
  "A.ndOH:No Drive Overheat",		// 91
  "A.nOHI:No Overheat Inter",		// 92
  "A.buS:AStop Bus         ",		// 93
  "A.PrF:AStop Prot Rot Fwd",		// 94
  "A.Prr:AStop Prot Rot Rev",		// 95
  "A.dOH:AStop Drv Overheat",		// 96
  "A.OH2:AStop Motor Prot  ",		// 97
  "A.nOL:No AStop Overload ",		// 98
  "A.OL:AStop Overload     ",		// 99
  "A.OL2:AStop Overload 2  ",		// 100
  "A.nOL2:No AStop Overld 2",		// 101
  "A.Set:Abnormal Stop Set ",		// 102
  "A.SbuS:AStop Bus Sync   ",		// 103
  "A.SLF:AStop SW Limit Fwd",		// 104
  "A.SLr:AStop SW Limit Rev",		// 105
  "A.106:AStop Max. Accel  ",		// 106
  "A.107:AStop Spd Ctrl Lmt",		// 107
  "A.108:                  ",		// 108
  "A.109:                  ",		// 109
  "A.110:                  ",		// 110
  "A.111:                  ",		// 111
  "A.112:                  ",		// 112
  "A.113:                  ",		// 113
  "A.114:                  ",		// 114
  "A.115:                  ",		// 115
  "A.116:                  ",		// 116
  "A.117:                  ",		// 117
  "A.118:                  ",		// 118
  "A.119:                  ",		// 119
  "A.120:                  ",		// 120
  "rFP:Ready Positioning   ",		// 121
  "PA:Positioning Active   ",		// 122
  "PnA:Pos Not Accessible  ",		// 123
  "E.124:Prot Rotation Fwd ",		// 124
  "E.125:Prot Rotation Rev ",		// 125
  "E.126:Pos Not Accessible",		// 126
  "Cddr:Calc Drive Data Rdy",		// 127
  "E.128:Reference Found   ",		// 128
};

const char us36_txt[7][25] = {	 // 9	Baud rate
  "0 = 1200 Baud           ",
  "1 = 2400 Baud           ",
  "2 = 4800 Baud           ",
  "3 = 9600 Baud           ",
  "4 = 19200 Baud          ",
  "5 = 38400 Baud          ",
  "6 = 57600 Baud          ",
};

const char ld26_txt[2][25] = {	 // 10	Rotor Position Mode
  "0 = Ld not equal lq     ",
  "1 = ld equal lq         ",
};

const char lp01_txt[3][25] = {	 // 11	Rotor Position Mode
  "0 = Off                 ",
  "1 = Teach               ",
  "2 = 1 Flr Positioning On",
};

const char di00_txt[2][25] = {	 // 12	Rotor Position Mode
  "0 = PnP                 ",
  "1 = nPn                 ",
};

const char do42_txt[16][25] = {	 // 13 Output Inversion
  " 0=No Inversion         ",
  " 1=X2A.18               ",
  " 2=X2A.19               ",
  " 3=X2A.18,19            ",
  " 4=X2A.24-26            ",
  " 5=X2A.18,24-26         ",
  " 6=X2A.19,24-26         ",
  " 7=X2A.18,19,24-26      ",
  " 8=X2A.27-29            ",
  " 9=X2A.18,27-29         ",
  "10=X2A.19,27-29         ",
  "11=X2A.18,19,27-29      ",
  "12=X2A.24-27,27-29      ",
  "13=X2A.18,24-26,27-29   ",
  "14=X2A.19,24-26,27-29   ",
  "15=X2A.18,19,24-26,27-29",
};

const char out_sel_txt[7][25] = {	 // 14 Output Selection
  "0 = Fault               ",
  "1 = Ready               ",
  "2 = Drive On            ",
  "3 = At Speed            ",
  "4 = High Speed          ",
  "5 = Brake Control       ",
  "6 = Motor Contactor Ctrl",
};


const char us03_04_txt[10][25] = {	 // 15 Load Default Parameters
  "0=PASS: Load Successful",
  "1=Load: Load Program   ",
  "2=NA: Reserved         ",
  "3=Idata: Invalid Data  ",
  "4=NA: Not Applicable   ",
  "5=NA: Not Applicable   ",
  "6=buSY: Inverter Busy  ",
  "7=NA: Not Applicable   ",
  "8=bdPAS: Invld Password",
  "9=NA: Not Applicable   ",
};

const char lf29_txt[10][25] = {		// 16 Encoder sample rate
  "0 = 0.5 msec            ",
  "1 = 1 msec              ",
  "2 = 2 msec              ",
  "3 = 4 msec              ",
  "4 = 8 msec              ",
  "5 = 16 msec             ",
  "6 = 32 msec             ",
  "7 = 64 msec             ",
  "8 = 128 msec            ",
  "9 = 256 msec            ",
};

const char lf38_txt[2][25] = {		// 17 Switching frequency
  "0 = 8 kHz               ",
  "1 = 16 kHz              ",
};

const char lf61_txt[7][25] = {	 	// 18 Emergency Power Operation
  "0=Off: No Function      ",
  "1=SPd1: Speed 1         ",
  "2=SPd2: Speed 2         ",
  "3=SPd3: Speed 3         ",
  "4=di 1: Input X2A.10    ",
};

const char us01_txt[2][25] = {		// 19 Password
  "AP_on: Elev Application ",
  "AP: Application         ",
};


const char ld33_txt[11][25] = {		// 20 Torque Command Filter sample rate
  "0 = off                 ",
  "1 = 0.5 msec            ",
  "2 = 1 msec              ",
  "3 = 2 msec              ",
  "4 = 4 msec              ",
  "5 = 8 msec              ",
  "6 = 16 msec             ",
  "7 = 32 msec             ",
  "8 = 64 msec             ",
  "9 = 128 msec            ",
  "10 = 256 msec           ",
};

const char ld31_txt[10][25] = {		// 20 FFTC Filter sample rate
  "0 = off                 ",
  "1 = 2 msec              ",
  "2 = 4 msec              ",
  "3 = 8 msec              ",
  "4 = 16 msec             ",
  "5 = 32 msec             ",
  "6 = 64 msec             ",
  "7 = 128 msec            ",
  "8 = 256 msec            ",
  "9 = 512 msec            ",
};

const char drv_par_min_max[25] = {
  "Mn=       Mx=           "
};

const char drv_par_wait[25] = {
  "        waiting for data"
};
const char drv_par_view[25] = {
  "                        "
};
const char drv_par_timed_out[25] = {
  "        request time-out"
};

const char 	not_while_running[25] = {
  "   Not While Running    "
};
			
const char 	par_wrt_process[8][25] = {
  "   Waiting for Drive    ", // 0
  "   Reading LF.03 Mode   ", // 1
  "   Change Drive LF.03   ", // 2
  "    to Stop Mode? Y     ", // 3
  "     to Run Mode? Y     ", // 4
  "Writing LF.03 Conf Mode ", // 5
  " Writing LF.03 Run Mode ", // 6
  "Writing Drive Parameter ", // 7
};
				  
const char 	drv_response_error[16][25] = {
  "        No Error        ", // 0
  "        Not Ready       ", // 1
  "        Addr/PW Invalid ", // 2
  "        Data Invalid    ", // 3
  "        Write Protected ", // 4
  "        BCC Error       ", // 5
  "        Inverter Busy   ", // 6
  "        Serv Not Avail  ", // 7
  "        Password Invalid", // 8
  "        Telegram Error  ", // 9
  "        Transmission Err", // 10
  "        Set ID Invalid  ", // 11
  "        Set ID Invalid  ", // 12
  "        Not This SW Ver ", // 13
  "        Not Possible    ", // 14
  "        NAK/EOT w/Inv Er", // 15 not used

};


struct tag_keb_drv_par_tbl {
	int16 len;
	struct tag_par {
		int16 digit;
		int16 min;
		int32 max;
		char name [25];
		char unit [5];
		char text_seq;
		int16 address;
		unsigned char set;
		unsigned char rd_wr;
		unsigned char dpy;
		unsigned char par_name_len;
	} par[30];
} keb_drv_par_tbl[14] = 
{
	{
		// General Parameters  Group 0
		11,		// parameter length	
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,1,6, 		"LF.02 Operating Mode    ","  --",1,0x0382,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{0,0,2,			"LF.03 Drv Configuration ","  --",2,0x0383,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{0,1,4,			"LF.04 Drive Mode        ","  --",3,0x0384,1,0,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,0,10,  		"LF.05 Auto Reset        ","  --",22,0x0385,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,0,2,		    "US.04 Load Configuration","  --",15,0x0184,1,0,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,1,4,			"US.10 Select Config.    ","  --",3,0x018A,1,1,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,256,16384,	"LF.27 Encoder Pulse Num."," ppr",0,0x039B,1,1,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,3,			"LF.28 Swap Encoder Chan.","  --",0,0x039C,1,1,1,5},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,9, 		"LF.29 Enc Sample Rate   ","msec",0,0x039D,1,1,1,5},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,0,65535,	 	"LF.77 Absolute Enc. Pos ","  --",0,0x03CD,1,1,1,5},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"LF.82 X2A Terminal Input","  --",0,0x03D2,1,0,1,5},	// parameter 10, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"LF.99 Drive Status      ","  --",8,0x03E3,1,0,1,5},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// Motor Data	Group 1
		11,		// parameter length	LF 8-19
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,1,			"LF.08 Elect. Motor Prot.","  --",4,0x0388,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{1,1,4000,		"LF.09 Mtr Prot. Current ","Amps",0,0x0389,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{1,5,1250,		"LF.10 Rated Motor Power ","  Hp",0,0x038A,1,0,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{1,100,60000,	"LF.11 Rated Motor Speed "," rpm",0,0x038B,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{1,10,3000,		"LF.12 Rated Mtr Current ","Amps",0,0x038C,1,1,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{1,40,2000,		"LF.13 Rated Motor Freq  ","  Hz",0,0x038D,1,1,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,120,500,		"LF.14 Rated Motor Volt  ","Volt",0,0x038E,1,1,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{2,50,100,		"LF.15 Power Factor      ","  --",0,0x038F,1,1,1,5},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{1,5,60000,		"LF.16 Field Weakening Sp"," rpm",0,0x0390,1,1,1,5},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,1,10000,		"LF.17 Rated Motor Torque","lbft",0,0x0391,1,1,1,5},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{3,0,49999,		"LF.18 Motor Resistance  ","ohms",0,0x0392,1,1,1,5},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{2,1,5000,		"LF.19 Motor Inductance  ","  mH",0,0x0393,1,1,1,5},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// Machine Data	Group 2
		5,		// parameter length	LF 20-25
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,1600,		"LF.20 Contract Speed    "," fpm",0,0x0394,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{2,700,8000,	"LF.21 Sheave Diameter   ","inch",0,0x0395,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{2,100,25000,	"LF.22 Gear Reduc. Ratio ","  --",0,0x0396,1,1,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,1,8,			"LF.23 Roping Ratio      ","  --",0,0x0397,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,0,30000,		"LF.24 Load              ","  lb",0,0x0398,1,1,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{2,0,65535,		"LF.25 Est. Gear Ratio   ","  --",0,0x0399,1,0,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// Encoder Data	Group 3
		6,		// parameter length	LF 26-30
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,255,		"0.LF.26 Encoder Type    ","  --",5,0x039A,1,1,1,7},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{0,0,255,		"1.LF.26 Connection Type ","  --",6,0x039A,2,0,1,7},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{0,0,255,	 	"2.LF.26 Conn. Status    ","  --",7,0x039A,4,0,1,7},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,256,16384,	"LF.27 Encoder Pulse Num."," ppr",0,0x039B,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,0,3,			"LF.28 Swap Encoder Chan.","  --",0,0x039C,1,1,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,0,9, 		"LF.29 Enc Sample Rate   ","msec",16,0x039D,1,1,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,5,			"LF.30 Control Mode      ","  --",0,0x039E,1,1,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 12, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// Speed Control Group 4
		13,		// parameter length	LF 31-38
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,1,50396,		"A.LF.31 KP Speed Acc/HS ","  --",0,0x039F,1,1,1,7},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{0,1,50396,		"d.LF.31 KP Speed Dec/LS ","  --",0,0x039F,2,1,1,7},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{0,1,50396, 	"P.LF.31 KP Speed Pre-Trq","  --",0,0x039F,4,1,1,7},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,1,26214,		"A.LF.32 KI Speed Acc/HS ","  --",0,0x03A0,1,1,1,7},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,1,26214,   	"d.LF.32 KI Speed Dec/LS ","  --",0,0x03A0,2,1,1,7},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,1,26214,		"P.LF.32 KI Speed Pre-Trq","  --",0,0x03A0,4,1,1,7},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,8000, 		"A.LF.33 KI SpdOff Acc/HS","  --",0,0x03A1,1,1,1,7},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,8000,		"d.LF.33 KI SpdOff Dec/LS","  --",0,0x03A1,2,1,1,7},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,1,32767,		"LF.34 KP Current        ","  --",0,0x03A2,1,1,1,5},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,1,32767,		"LF.35 KI Current        ","  --",0,0x03A3,1,1,1,5},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,23590,		"0.LF.36 Maximum Torque  ","lbft",0,0x03A4,1,1,1,7},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,23590,		"1.LF.36 EP Maximum Torq ","lbft",0,0x03A4,2,1,1,7},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{1,0,255,		"LF.37 Low Spd Torq Boost","   %",0,0x03A5,1,1,1,5},	// parameter 12, digit, min, max, "name", "unit", txt, addr
		 	{0,0,1,			"LF.38 Switching Freq    ","    ",17,0x03A6,1,1,1,5},	// parameter 13, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// 	 Pattern Control Group 5
		25,		// parameter length	 LF 41-57
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{1,0,250,		"LF.41 Leveling Speed    "," fpm",0,0x03A9,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{1,0,16000,		"LF.42 High Speed        "," fpm",0,0x03AA,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{1,0,1500,		"LF.43 Inspection Speed  "," fpm",0,0x03AB,1,1,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{1,0,16000,		"LF.44 High Leveling Spd "," fpm",0,0x03AC,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{1,0,16000,		"LF.45 Intermediate Spd 1"," fpm",0,0x03AD,1,1,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{1,0,16000,		"LF.46 Intermediate Spd 2"," fpm",0,0x03AE,1,1,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{1,0,16000,		"LF.47 Intermediate Spd 3"," fpm",0,0x03AF,1,1,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{2,30,3200,		"0.LF.50 Str Jrk HS/HL/IS","fps3",0,0x03B2,1,1,1,7},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{2,30,3200,		"1.LF.50 Start Jerk LS/IS","fps3",0,0x03B2,2,1,1,7},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{2,30,3200,		"2.LF.50 Start Jerk ES   ","fps3",0,0x03B2,4,1,1,7},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{2,30,1200,		"0.LF.51 Accel HS/HL/IS  ","fps2",0,0x03B3,1,1,1,7},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{2,30,1200,		"1.LF.51 Accel LS/IS     ","fps2",0,0x03B3,2,1,1,7},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{2,30,1200,		"2.LF.51 Accel ES        ","fps2",0,0x03B3,4,1,1,7},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"0.LF.52 Acc Jrk HS/HL/IS","fps3",0,0x03B4,1,1,1,7},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"1.LF.52 Accel Jerk LS/IS","fps3",0,0x03B4,2,1,1,7},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"2.LF.52 Accel Jerk ES   ","fps3",0,0x03B4,4,1,1,7},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"0.LF.52 Dec Jrk HS/HL/IS","fps3",0,0x03B5,1,1,1,7},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"1.LF.52 Decel Jerk LS/IS","fps3",0,0x03B5,2,1,1,7},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"2.LF.52 Decel Jerk ES   ","fps3",0,0x03B5,4,1,1,7},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{2,30,1200,		"0.LF.53 Decel HS/HL/IS  ","fps2",0,0x03B6,1,1,1,7},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{2,30,1200,		"1.LF.53 Decel LS/IS     ","fps2",0,0x03B6,2,1,1,7},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{2,30,1200,		"2.LF.53 Decel ES        ","fps2",0,0x03B6,4,1,1,7},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"0.LF.55 Flare J HS/HL/IS","fps3",0,0x03B7,1,1,1,7},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"1.LF.55 Flare Jerk LS/IS","fps3",0,0x03B7,2,1,1,7},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"2.LF.55 Flare Jerk ES   ","fps3",0,0x03B7,4,1,1,7},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{2,30,3200,		"LF.56 Stop Jerk         ","fps3",0,0x03B8,1,1,1,5},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// 	 Misc. Functions Group 6
		12,		// parameter length	 LF 57-79
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,2,			"LF.57 Speed Following Er","  --",4,0x03B9,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{0,0,30,		"LF.58 Speed Difference  ","   %",0,0x03BA,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{1,1,10,		"LF.59 Following Er Timer"," sec",0,0x03BB,1,1,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,0,4,			"LF.61 Emergency Pwr Mode","    ",18,0x03BD,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{2,25,200, 		"LF.67 Pre-Torque Gain   ","  --",0,0x03C3,1,1,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{1,-1000,1000,	"LF.68 Pre-Torque Offset ","   %",0,0x03C4,1,1,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,-1,1,		"LF.69 Pre-Torq Direction","  --",0,0x03C5,1,1,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{1,0,30, 		"LF.70 Brake Rel/Strt Dly"," sec",0,0x03C6,1,1,1,5},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{1,0,10000,		"LF.71 Brake Release Dly "," sec",0,0x03C7,1,1,1,5},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,0,13,		"LF.76 Encoder Multiplier","  --",0,0x03CC,1,1,1,5},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,65535,	 	"LF.77 Absolute Enc. Pos ","  --",0,0x03CD,1,1,1,5},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{2,0,300,  		"LF.78 Brake Engage Time "," sec",0,0x03CE,1,1,1,5},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{1,1,30,   		"LF.79 Current Hold Time "," sec",0,0x03CF,1,1,1,5},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,0, 		"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{0,0,0, 		"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// 	 Diag. Functions Group 7
		22,		// parameter length	LF 80-99
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{2,0,0,			"LF.80 Software Version  ","  --",0,0x03D0,1,0,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{1,0,0,			"LF.81 Software Date     ","  --",0,0x03D1,1,0,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"LF.82 X2A Terminal Input","  --",0,0x03D2,1,0,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"LF.83 X2A Term. Output  ","  --",0,0x03D3,1,0,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"LF.86 Selected Speed    ","  --",0,0x03D6,1,0,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"LF.87 Actual Drive Load ","   %",0,0x03D7,1,0,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{1,0,0,			"LF.88 Motor Set Speed   "," rpm",0,0x03D8,1,0,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{1,0,0,			"LF.89 Actual Motor Speed"," rpm",0,0x03D9,1,0,1,5},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"LF.90 Elevator Speed    "," fpm",0,0x03DA,1,0,1,5},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{1,0,0,			"LF.93 Phase Current     ","Amps",0,0x03DD,1,0,1,5},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{1,0,0,			"LF.94 Peak Phase Current","Amps",0,0x03DE,1,1,1,5},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"LF.95 DC Bus Volatge    ","Volt",0,0x03DF,1,0,1,5},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"LF.96 Peak DC Bus Volts ","Volt",0,0x03E0,1,1,1,5},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{1,0,0,			"LF.97 Actual Output Freq","  Hz",0,0x03E1,1,0,1,5},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"0.LF.98 Last Fault 1    ","  --",8,0x03E2,1,1,1,7},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"1.LF.98 Last Fault 2    ","  --",8,0x03E2,2,0,1,7},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"2.LF.98 Last Fault 3    ","  --",8,0x03E2,4,0,1,7},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"3.LF.98 Last Fault 4    ","  --",8,0x03E2,8,0,1,7},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"4.LF.98 Last Fault 5    ","  --",8,0x03E2,16,0,1,7},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"5.LF.98 Last Fault 6    ","  --",8,0x03E2,32,0,1,7},	// parameter 19, digit, min, max, "name", "unit", txt, addr
	 		{0,0,128,		"6.LF.98 Last Fault 7    ","  --",8,0x03E2,64,0,1,7},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"7.LF.98 Last Fault 8    ","  --",8,0x03E2,128,0,1,7},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,128,		"LF.99 Drive Status      ","  --",8,0x03E3,1,0,1,5},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// 	 Configuration Group 8
		3,		// parameter length	 US 01-10
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,9999,		"US.01 Password          ","  --",19,0x0181,1,0,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{0,0,1,			"US.03 Default all LF Par","  --",15,0x0183,1,0,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{0,0,1,		    "US.04 Load Configuration","  --",15,0x0184,1,0,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,1,4,			"US.10 Select Config.    ","  --",3,0x018A,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// 	 Advanced Functions	Group 9
		16,		// parameter length	US 8-37
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,255,		"US.16 E.OL2 Function    ","  --",0,0x0190,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{2,0,3200, 		"US.17 Pre-Torq Ramp Up  "," sec",0,0x0191,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{2,0,3200, 		"US.18 Pre-Torq Ramp Down"," sec",0,0x0192,1,1,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,0,1600,	  	"US.20 Speed Maximum KI  "," fpm",0,0x0194,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,0,1600,	  	"US.21 Speed Minimum KI  "," fpm",0,0x0195,1,1,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,0,1,			"US.22 Speed Dep KP Gain ","  --",4,0x0196,1,1,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,50396,		"US.23 Min KP Gain Hi Spd","  --",0,0x0197,1,1,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,5000,		"US.24 KD Speed Gain     ","  --",0,0x0198,1,1,1,5},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,4,			"US.25 Phase Current Chck","  --",0,0x0199,1,1,1,5},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{1,-100,100,	"US.28 AN1 Zero Clamp    ","  --",0,0x019C,1,1,1,5},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{2,0,100, 		"US.29 HSP5 Watchdog Time"," sec",0,0x019D,1,1,1,5},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,1,32767,		"US.31 KP Synth Pre-Torq ","  --",0,0x019F,1,1,1,5},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,1,32767,		"US.32 KI Synth Pre-Torq ","  --",0,0x01A0,1,1,1,5},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,1,			"US.33 EdOH Function     ","  --",4,0x01A1,1,1,1,5},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{2,1,2000,		"US.34 Analog Patter Gain","  --",0,0x01A2,1,1,1,5},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,127,  		"US.35 Reference Split   ","msec",0,0x01A3,1,1,1,5},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,6,			"US.36 Baud Rate         ","baud",9,0x01A4,1,1,1,5},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// 	 Advancd Elevator Drive	Group 10
		15,		// parameter length	Ld 18-33
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{1,1,60000,		"Ld.18 Field Weak Corner "," rpm",0,0x0792,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{2,1,200,  		"Ld.19 Field Weak Curve  ","  --",0,0x0793,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{3,0,50000,	 	"Ld.20 Stator Resistance ","ohms",0,0x0794,1,1,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{2,1,50000,		"Ld.21 Sigma Inductance  ","  mH",0,0x0795,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{3,0,49999,		"Ld.22 Rotor Resistance  ","ohms",0,0x0796,1,1,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{2,1,50000,		"Ld.23 Mag/Max Inductance","  mH",0,0x0797,1,1,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,1023,	   	"Ld.24 Motor Control     ","  --",0,0x0798,1,1,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,110,		"Ld.25 Vmax Regulation   ","   %",0,0x0799,1,1,1,5},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		    {0,0,1,		    "Ld.26 Rotor Pos Mode    ","  --",10,0x079A,1,1,1,5},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,1,32767,	 	"Ld.27 KP Current        ","  --",0,0x079B,1,1,1,5},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,1,32767,	 	"Ld.28 KI Current        ","  --",0,0x079C,1,1,1,5},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,25000,		"Ld.29 Accel Torque      ","  Nm",0,0x079D,1,1,1,5},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		  	{2,0,107374,	"Ld.30 System Inertia    ","kgm2",0,0x079E,1,1,1,5},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,9,			"Ld.31 FFTC Filter       ","msec",21,0x079F,1,1,1,5},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{1,0,2000,		"Ld.32 FFTC Gain         ","   %",0,0x07A0,1,1,1,5},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,10,		"Ld.33 Torque Cmd Filter ","msec",20,0x07A1,1,1,1,5},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// 	   Positioning Group 11
		7,		// parameter length	LP 01-23
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,2,		    "LP.01 One Floor Position","  --",11,0x0881,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{1,0,2000, 		"LP.02 Min Slowdown Dist ","inch",0,0x0882,1,0,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{1,0,2000, 		"LP.03 HS Slowdown Dist  ","inch",0,0x0883,1,1,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
			{1,0,500,  		"LP.04 Correction Dist   ","inch",0,0x0884,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{1,0,99999,		"LP.12 Current Position  ","inch",0,0x088C,1,0,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,0,9999,	  	"LP.21 Scaling Incr. High","  --",0,0x0895,1,1,1,5},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,9999,	  	"LP.22 Scaling Incr. Low ","  --",0,0x0896,1,1,1,5},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{2,0,99999,	  	"LP.23 Scaling Distance  ","inch",0,0x0897,1,1,1,5},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 12, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// Digital Inputs Group 12
		1,		// parameter length	 di 00-03
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,1,		    "di.00 Input Type        ","  --",12,0x0580,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{0,0,127,		"di.03 Noise Filter      ","msec",0,0x0583,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
	{
		// Digital Outputs Group 13
		4,		// parameter length	 do 42-83
		{
			//{digit,min,max,"name","unit",txt_seq,address,set,rd_wr,dpy,par_len}
		 	{0,0,15,       "do.42 Output Inversion  ","    ",13,0x06AA,1,1,1,5},	// parameter  0, digit, min, max, "name", "unit", txt, addr
		 	{0,0,6,		   "do.80 Output X2A.18     ","    ",14,0x06D0,1,1,1,5},	// parameter  1, digit, min, max, "name", "unit", txt, addr
		 	{0,0,6,		   "do.81 Output X2A.19     ","    ",14,0x06D1,1,1,1,5},	// parameter  2, digit, min, max, "name", "unit", txt, addr
		 	{0,0,6,		   "do.82 Output X2A.24-26  ","    ",14,0x06D2,1,1,1,5},	// parameter  3, digit, min, max, "name", "unit", txt, addr
		 	{0,0,6,		   "do.83 Output X2A.27-29  ","    ",14,0x06D3,1,1,1,5},	// parameter  4, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  5, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  6, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  7, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  8, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter  9, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 10, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 11, digit, min, max, "name", "unit", txt, addr
		 	{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 12, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 13, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 14, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 15, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 16, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 17, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 18, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 19, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 20, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 21, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 22, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 23, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 24, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 25, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 26, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 27, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 28, digit, min, max, "name", "unit", txt, addr
			{0,0,0,			"                        ","    ",0,0x0000,0,0,0,0},	// parameter 29, digit, min, max, "name", "unit", txt, addr
		},
	},
};

#pragma section all_types

void Adjust_KEB_Drive_Parameters (void);
void set_KEB_par_min_max (void);
static int16 Get_Drv_Par(int32 *Drv_Par);
static int16 Req_Drv_Par (int16 Drv_Par_Grp_Ptr,int16 Drv_Par_Ptr);
//static int16 Upd_Drv_Par( int16 Drv_Par, int16 Drv_Par_Grp_Ptr,int16 Drv_Par_Ptr, int16 confg);
static void display_KEB_variable (int16 seq);
void verify_run_mode (void);

#if ((Traction == 1) || (Tract_OL == 1))

//********************************
// Adjust drive parameters
//********************************
void Adjust_KEB_Drive_Parameters (void)
{
	int16 i;
	static int16 yes_no;
	static int16 time_out_cnt;

	if(LCD_Pointer == 3)		// first time through procedure
	{
	    LCD_Pointer = 4;
	    Up_Date_LCD = 1;
		Drv_Par_Grp_Ptr = 0;
		Drv_Par_Ptr = 0;
	    LCD_Sub_Point = 0;
	    LCD_Dig_Point = 0;
		drv_par_req = 0;
		drv_par_ret = 0;
		time_out_cnt = 0;
		flash_disable = 0;
		set_KEB_par_min_max();
	    for(i=0; i<=23; i++)
			LCD_Display[0][i] = LCD_Field_Variables[Menu_level - 1][i];

		if (drv_mode == 100)
		{						 // drive mode has not been received so request drive mode
			Drv_Par_Ptr = 2;
			Drv_Par_Grp_Ptr = 0;
			drv_par_req = Req_Drv_Par(Drv_Par_Grp_Ptr,Drv_Par_Ptr);
		}
		else
		{
			drv_par_req = Req_Drv_Par(Drv_Par_Grp_Ptr,Drv_Par_Ptr);
		}
	}
	if (LCD_Pointer == 4)
	{			// Display and adjust drive parameters

		if ((drv_par_req == 1) && (drv_par_ret == 0))
		{
			drv_par_ret = Get_Drv_Par(&Drv_Par);
			if (drv_par_ret == 1)
			{
				drv_par_req = 0;
		    	Up_Date_LCD = 1;
				time_out_cnt = 0;
				if ((Drv_Par_Grp_Ptr == 0) && (Drv_Par_Ptr == 2))
				{
					drv_mode = (int16)Drv_Par;
					set_KEB_par_min_max();
				}
			}
			else if ((drv_par_ret == 2)	|| (drv_par_ret == -1))
			{
				if (time_out_cnt < 3)
				{
					time_out_cnt ++;
					drv_par_req = Req_Drv_Par(Drv_Par_Grp_Ptr,Drv_Par_Ptr);
					if (drv_par_req == 1)
						drv_par_ret = 0;
				}
				else
					Up_Date_LCD = 1;
			}
			else if ((drv_eot == 1) || (drv_nak == 1))
			{
				drv_par_req = 0;
				Up_Date_LCD = 1;
			}
		}
		else
			time_out_cnt = 0;

		if((rdLCDio(Lcd_MODE)== 0) && (LCD_Mode_PB == 0))
		{	   // Exit to main menu
			LCD_Mode_PB = 1;  //
			if (In_Drv_Grp == 0)
			{			// At parameter group level so return to adjust variable menu
				LCD_Pointer = 9;
			}
			else
			{
			 	Up_Date_LCD = 1;
				if (Mod_Drv_Par == 0)
					In_Drv_Grp = 0;		// return to display variable
				else if (drv_par_ret != 1)
					Mod_Drv_Par = 0;
				else		// move digit for variable
				 	LCD_Dig_Point = set_adj_digit(keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max,LCD_Dig_Point);			
			}
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    Up_Date_LCD = 1;
			if (In_Drv_Grp == 0)
			{		// Next Parameter group
			    Drv_Par_Grp_Ptr++;
	 		   	if(Drv_Par_Grp_Ptr > 13)
				  	Drv_Par_Grp_Ptr = 0;
			}
			else 
			{
				if (Mod_Drv_Par == 1)		// Adjust parameter
					adjust_variable_32(&Drv_Par,keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max,
									keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].min,1,LCD_Dig_Point);
				else
				{		// Next parameter
					i = 0;
					while (i < keb_drv_par_tbl[Drv_Par_Grp_Ptr].len)
					{
						Drv_Par_Ptr++;
						if (Drv_Par_Ptr > keb_drv_par_tbl[Drv_Par_Grp_Ptr].len)
							Drv_Par_Ptr = 0;
						
						if (keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].dpy == 1)
							break;												 // stop on the valid display messages
						i++;
					}

					drv_par_req = Req_Drv_Par(Drv_Par_Grp_Ptr,Drv_Par_Ptr);
					if (drv_par_req == 1)
						drv_par_ret = 0;
				}
			}
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    Up_Date_LCD = 1;
			if (In_Drv_Grp == 0)
			{		   // Next Parameter Group
			    Drv_Par_Grp_Ptr--;
			    if (Drv_Par_Grp_Ptr < 0)
					Drv_Par_Grp_Ptr = 13;
			}
			else 
			{
				if (Mod_Drv_Par == 1)	 // Adjust parameter
					adjust_variable_32(&Drv_Par,keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max,
									keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].min,0,LCD_Dig_Point);
				else
				{		// Next Parameter

					i = 0;
					while (i < keb_drv_par_tbl[Drv_Par_Grp_Ptr].len)
					{
						Drv_Par_Ptr--;
						if (Drv_Par_Ptr < 0)
							Drv_Par_Ptr = keb_drv_par_tbl[Drv_Par_Grp_Ptr].len;

						if (keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].dpy == 1)
							break;												 // stop on the valid display messages
						i++;
					}
					drv_par_req = Req_Drv_Par(Drv_Par_Grp_Ptr,Drv_Par_Ptr);
					if (drv_par_req == 1)
						drv_par_ret = 0;
				}
			}
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	Up_Date_LCD = 1;
			if (In_Drv_Grp == 0)
			{
				In_Drv_Grp = 1;
				Drv_Par_Ptr = 0;
				i = 0;
				while (i < keb_drv_par_tbl[Drv_Par_Grp_Ptr].len)
				{
					if (Drv_Par_Ptr > keb_drv_par_tbl[Drv_Par_Grp_Ptr].len)
						Drv_Par_Ptr = 0;
					
					if (keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].dpy == 1)
						break;												 // stop on the valid display messages
					Drv_Par_Ptr++;
					i++;
				}
				drv_par_req = Req_Drv_Par(Drv_Par_Grp_Ptr,Drv_Par_Ptr);
				if (drv_par_req == 1)
					drv_par_ret = 0;
			}
			else
			{
				if (keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].rd_wr == 1)
				{
					if (rdoutp(o_RUN) == 0)
					{
						if (Mod_Drv_Par == 0)
						{
						    LCD_Dig_Point = 0;
							Mod_Drv_Par = 1;
						}
						else
						{
							Mod_Drv_Par = 0;
							LCD_Pointer = 7;
							return;
						}											 
					}
					else
					{
						if (rdoutp(o_RUN) == 1)
							LCD_Pointer = 5;
						Mod_Drv_Par = 0;
					}
				}
				else
				{		 // if parameter is read only then update each time enter button hit
					Mod_Drv_Par = 0;
					drv_par_req = Req_Drv_Par(Drv_Par_Grp_Ptr,Drv_Par_Ptr);
					if (drv_par_req == 1)
						drv_par_ret = 0;
				}
			}
		}
		if(Up_Date_LCD == 1)
		{
			Up_Date_LCD = 0;
			if (In_Drv_Grp == 0)
			{			// Display the drive parameter group
			    for(i=0; i<=23; i++)
			    {
			
					LCD_Display[0][i] = LCD_Field_Variables[Menu_level - 1][i];
					LCD_Display[1][i] = keb_drv_par_grp_tbl[Drv_Par_Grp_Ptr][i];
				}
			}
			else			
			{			// Display the drive parameter and value
				if (Mod_Drv_Par == 0)
				{
				    for(i=0; i<=23; i++)
				    {
						LCD_Display[0][i] = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].name[i];
						if ((drv_nak == 1) || (drv_eot == 1)) 
						{
							if ((drv_error > 0) && (drv_error < 15))
								LCD_Display[1][i] = drv_response_error[drv_error][i];
							else
								LCD_Display[1][i] = drv_response_error[15][i];
						}
						else if (drv_par_ret == 0)
							LCD_Display[1][i] = drv_par_wait[i];
						else if (drv_par_ret == 2)
							LCD_Display[1][i] = drv_par_timed_out[i];
						else
							LCD_Display[1][i] = drv_par_view[i];
					}
				}
				else
				{
				    for(i=0; i<=23; i++)
				    {
						LCD_Display[0][i] = drv_par_min_max[i];
						if ((drv_nak == 1) || (drv_eot == 1)) 
						{
							if ((drv_error > 0) && (drv_error < 15))
								LCD_Display[1][i] = drv_response_error[drv_error][i];
							else
								LCD_Display[1][i] = drv_response_error[15][i];
						}
						else if (drv_par_ret == 0)
							LCD_Display[1][i] = drv_par_wait[i];
						else if (drv_par_ret == 2)
							LCD_Display[1][i] = drv_par_timed_out[i];
						else
							LCD_Display[1][i] = drv_par_view[i];

						if (i < keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].par_name_len)
							LCD_Display[1][i] = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].name[i];
						if (i>19)
							LCD_Display[0][i] = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].unit[i-20];
							
					}
					display_variable(0, 3, keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].digit, keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].min, 0);
					if (keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max >= 0x10000)
						display_variable_11_digits(0, 13, keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].digit, keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max, 0);
					else
						display_variable(0, 13, keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].digit, (int16)keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max, 0);
				}

				if (keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].text_seq == 0)
				{
					flash_disable = 0;
					if (drv_par_ret == 1)
					{
						if (keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max >= 0x10000)
							display_variable_11_digits(1,(int16)(keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].par_name_len + 2), keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].digit, Drv_Par, 1);
						else
							display_variable(1, (int16)(keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].par_name_len + 2), keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].digit, (int16)Drv_Par, 1);
					}
				}
				else
				{
					if (drv_par_ret == 1)
						display_KEB_variable(keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].text_seq);
				}
			}
		}
		flash_digit(LCD_Flash_TMP[1], 1,LCD_Dig_Loc,((Mod_Drv_Par == 1) && (drv_par_ret == 1) && (flash_disable == 0)));
	}

	if ((LCD_Pointer == 5) || (LCD_Pointer == 6))
	{			// Not while car running

		if (LCD_Pointer == 5)
		{
			LCD_Pointer = 6;
			for(i=0; i<=23; i++)
			{
				LCD_Display[1][i] = not_while_running[i];
			}
		}

		if((rdLCDio(Lcd_MODE)== 0) && (LCD_Mode_PB == 0))
		{	   // Exit to main menu
			LCD_Mode_PB = 1;  //
		    Up_Date_LCD = 1;
			LCD_Pointer = 4;
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    Up_Date_LCD = 1;
			LCD_Pointer = 4;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	Up_Date_LCD = 1;
			LCD_Pointer = 4;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    Up_Date_LCD = 1;
			LCD_Pointer = 4;
		}
	}


	if ((LCD_Pointer == 7) || (LCD_Pointer == 8))
	{			// Update drive parameter

		if (LCD_Pointer == 7)
		{			 // first time in to this section
			if ((Drv_Par_Grp_Ptr != 0) || ((Drv_Par_Grp_Ptr == 0) && (Drv_Par_Ptr >= 5)))
			{		// request lf.03 drive operating mode
				drv_rpar_addr = keb_drv_par_tbl[0].par[1].address;
				drv_rpar_set = keb_drv_par_tbl[0].par[1].set;
				put_pkt_req('G',2,DRVCOM);
				LCD_Sub_Point = 0;
				drv_nak = 0;
				drv_eot = 0;
				drv_error = 0;
			}
			else
				LCD_Sub_Point = 3;	  // skip reading lf.03 (this is the lf.03 group)
			drv_ack = 0;
			drv_par_req = 0;
			drv_par_ret = 0;
			time_out_cnt = 0;
			LCD_Pointer = 8;
			Up_Date_LCD = 1;
			yes_no = 1;			// 1 = yes
		    timers[tlcd_keb_dly] = 6;	
		}


		if (LCD_Sub_Point == 0)
		{
		  	i = get_pkt_status('G', 2, DRVCOM, 20);
			if (i == 1)
			{			   // lf.03 data read in
				drv_par_req = 0;
				Up_Date_LCD = 1;
				time_out_cnt = 0;
				drv_ack = 0;
				if (drv_rpar_data != 2)
					LCD_Sub_Point = 1;
				else 
					LCD_Sub_Point = 3;			
			}
			else if ((i == 2) || (i == -1))
			{		  // timed out or not in the status queue
				if (time_out_cnt < 3)
				{		   // packete timed out so try again
					time_out_cnt ++;
					drv_rpar_addr = keb_drv_par_tbl[0].par[1].address;
					drv_rpar_set = keb_drv_par_tbl[0].par[1].set;
					put_pkt_req('G',2,DRVCOM);
					drv_nak = 0;
					drv_eot = 0;
					drv_error = 0;
				}
				else
					Up_Date_LCD = 1;
			}
			else
				time_out_cnt = 0;
		}
		else
		{
			if (LCD_Sub_Point == 2)
			{	// lf.03 being changed to stop mode
			  	i = get_pkt_status('g', 2, DRVCOM, 20);
				if ((timers[tlcd_keb_dly] > 20) || ((i == 1) && (drv_ack == 1)))
				{								   // received an ack
 					LCD_Sub_Point = 3;				// change paremeter
				 	Up_Date_LCD = 1;
				}
			}
			else if (LCD_Sub_Point == 3)
			{		// Write parameter
				drv_wpar_addr = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].address;
				drv_wpar_set = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].set;
				drv_wpar_data = Drv_Par;
				put_pkt_req('g',2,DRVCOM);
				drv_nak = 0;
				drv_eot = 0;
				drv_error = 0;
				timers[tlcd_keb_dly] = 0;
					LCD_Sub_Point = 4;
			 	Up_Date_LCD = 1;
			}
			else if (LCD_Sub_Point == 4)
			{	// Parameter being written
			  	i = get_pkt_status('g', 2, DRVCOM, 20);
				if (timers[tlcd_keb_dly] > 5)
				{		// Request parameter parameter
					drv_rpar_addr = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].address;
					drv_rpar_set = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].set;
					put_pkt_req('G',2,DRVCOM);
					drv_nak = 0;
					drv_eot = 0;
					drv_error = 0;
					drv_par_req = 1;
					drv_par_ret = 0;
				 	Up_Date_LCD = 1;
					LCD_Pointer = 4;			// back to parameter menu
				}
			}
		}

		if((rdLCDio(Lcd_MODE)== 0) && (LCD_Mode_PB == 0))
		{	   // Exit to main menu
			LCD_Mode_PB = 1;  //
		    Up_Date_LCD = 1;
			LCD_Pointer = 4;
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    Up_Date_LCD = 1;
			if (yes_no == 1)
				yes_no = 0;
			else
				yes_no = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    Up_Date_LCD = 1;
			if (yes_no == 1)
				yes_no = 0;
			else
				yes_no = 1;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	Up_Date_LCD = 1;
			if (yes_no == 1)
			{		// yes
				if (LCD_Sub_Point == 1)
				{		// Display "Place drive in stop mode?"
					drv_wpar_addr = keb_drv_par_tbl[0].par[1].address;
					drv_wpar_set = keb_drv_par_tbl[0].par[1].set;
					drv_wpar_data = 2;
					put_pkt_req('g',2,DRVCOM);	 // put drive in stop mode
					drv_nak = 0;
					drv_eot = 0;
					drv_ack = 0;
					drv_error = 0;
					timers[tlcd_keb_dly] = 0;
					LCD_Sub_Point = 2;
				}
			}
			else
			{
				drv_par_req = 0;
				drv_par_ret = 1;
			 	Up_Date_LCD = 1;
				LCD_Pointer = 4;
				return;
			}
		}

		if (Up_Date_LCD == 1)
		{
			Up_Date_LCD = 0;
		    for(i=0; i<=23; i++)
		    {
				if (LCD_Sub_Point == 0)
				{		// Reading Conf mode
					LCD_Display[0][i] = par_wrt_process[1][i];
					LCD_Display[1][i] = par_wrt_process[0][i];
				}
				else if (LCD_Sub_Point == 1)
				{		// Place Drive in Conf Mode?
					LCD_Display[0][i] = par_wrt_process[2][i];
					LCD_Display[1][i] = par_wrt_process[3][i];
				}
				else if (LCD_Sub_Point == 2)
				{		// Writing Conf Mode
					LCD_Display[0][i] = par_wrt_process[5][i];
					LCD_Display[1][i] = par_wrt_process[0][i];
				}
				else if (LCD_Sub_Point == 4)
				{	   // Writing drive parameter
					LCD_Display[0][i] = par_wrt_process[7][i];
					LCD_Display[1][i] = par_wrt_process[0][i];
				}
			}
			if (LCD_Sub_Point == 1)
			{
				if (yes_no == 1)
					LCD_Display[1][18] = 'Y';
				else
					LCD_Display[1][18] = 'N';
			}
		}
	}

	if ((LCD_Pointer == 9) || (LCD_Pointer == 10))
	{			// Mode button pressed to exit

		if (LCD_Pointer == 9)
		{			 // first time in to this section
			drv_rpar_addr = keb_drv_par_tbl[0].par[1].address;
			drv_rpar_set = keb_drv_par_tbl[0].par[1].set;
			put_pkt_req('G',2,DRVCOM);
			drv_nak = 0;
			drv_eot = 0;
			drv_error = 0;
			LCD_Sub_Point = 0;
			drv_par_req = 0;
			drv_par_ret = 0;
			time_out_cnt = 0;
			LCD_Pointer = 10;
			Up_Date_LCD = 1;
			yes_no = 1;			// 1 = yes
		    timers[tlcd_keb_dly] = 6;	
		}


		if (LCD_Sub_Point == 0)
		{
		  	i = get_pkt_status('G', 2, DRVCOM, 20);
			if (i == 1)
			{			   // lf.03 data read in
				drv_par_req = 0;
				Up_Date_LCD = 1;
				time_out_cnt = 0;
				if (drv_rpar_data != 0)
					LCD_Sub_Point = 1;
				else 
					LCD_Sub_Point = 3;
			}
			else if ((i == 2) || (i == -1))
			{
				if (time_out_cnt < 3)
				{		   // packete timed out so try again
					time_out_cnt ++;
					drv_rpar_addr = keb_drv_par_tbl[0].par[1].address;
					drv_rpar_set = keb_drv_par_tbl[0].par[1].set;
					put_pkt_req('G',2,DRVCOM);
					drv_nak = 0;
					drv_eot = 0;
					drv_error = 0;
				}
				else
					Up_Date_LCD = 1;
			}
			else
				time_out_cnt = 0;
		}
		else if (LCD_Sub_Point == 2)
		{	// lf.03 being changed to run mode
		  	i = get_pkt_status('g', 2, DRVCOM, 20);
			if (timers[tlcd_keb_dly] > 10)
			{
				LCD_Sub_Point = 3;				// go back to main menu
			 	Up_Date_LCD = 1;
				return;
			}
		}
		else if (LCD_Sub_Point == 3)
		{		// Exit menu
			LCD_Pointer = Menu_level + 2;
			for(i=0; i<=23; i++)
		 	{
		 	    LCD_Display[0][i] = LCD_Main_Menu[2][i];
		 		LCD_Display[1][i] = LCD_Field_Variables[Menu_level - 1][i];
		 	}
		 	Menu_level = 0;
		 	return;
		}

		if((rdLCDio(Lcd_MODE)== 0) && (LCD_Mode_PB == 0))
		{	   // Exit to main menu
			LCD_Mode_PB = 1;  //
		    Up_Date_LCD = 1;
			LCD_Sub_Point = 3;
		}
		if((rdLCDio(Lcd_UP) == 0) && (LCD_UP_PB == 0))
		{
		    LCD_UP_PB = 1;  // incriment
		    Up_Date_LCD = 1;
			if (yes_no == 1)
				yes_no = 0;
			else
				yes_no = 1;
		}
		if((rdLCDio(Lcd_DN) == 0) && (LCD_DN_PB == 0))
		{
		    LCD_DN_PB = 1;  // decriment
		    Up_Date_LCD = 1;
			if (yes_no == 1)
				yes_no = 0;
			else
				yes_no = 1;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (LCD_Enter_PB == 0))
		{
		    LCD_Enter_PB = 1;
		  	Up_Date_LCD = 1;
			if (yes_no == 1)
			{		// yes
				if (LCD_Sub_Point == 1)
				{		// Display "Place drive in run mode?"
					drv_wpar_addr = keb_drv_par_tbl[0].par[1].address;
					drv_wpar_set = keb_drv_par_tbl[0].par[1].set;
					drv_wpar_data = 0;
					put_pkt_req('g',2,DRVCOM);	 // put drive in configure mode
					drv_nak = 0;
					drv_eot = 0;
					drv_error = 0;
					timers[tlcd_keb_dly] = 0;
					LCD_Sub_Point = 2;
				}
			}
			else
			{
			 	Up_Date_LCD = 1;
				LCD_Sub_Point = 3;
				return;
			}
		}
		if (Up_Date_LCD == 1)
		{
			Up_Date_LCD = 0;
		    for(i=0; i<=23; i++)
		    {
				if (LCD_Sub_Point == 0)
				{		// Reading run mode
					LCD_Display[0][i] = par_wrt_process[1][i];
					LCD_Display[1][i] = par_wrt_process[0][i];
				}
				else if (LCD_Sub_Point == 1)
				{		// Place Drive in run Mode?
					LCD_Display[0][i] = par_wrt_process[2][i];
					LCD_Display[1][i] = par_wrt_process[4][i];
				}
				else if (LCD_Sub_Point == 2)
				{		// Writing Run Mode
					LCD_Display[0][i] = par_wrt_process[6][i];
					LCD_Display[1][i] = par_wrt_process[0][i];
				}
			}
			if (LCD_Sub_Point == 1)
			{
				if (yes_no == 1)
					LCD_Display[1][18] = 'Y';
				else
					LCD_Display[1][18] = 'N';
			}
		}
	}
}

#endif


static void display_KEB_variable (int16 seq)
{
	int16 i;
	int16 valid_par_range;
	int16 numeric_dpy = 0;
	const char *pa;


	pa = &drv_par_view[0];	
	
	if ((Drv_Par >= keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].min) && (Drv_Par <= keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max))
		valid_par_range = 1;
	else
		valid_par_range = 0;

		
	switch (seq)
	{
		case 0:
			break;
		case 1:
			if (valid_par_range == 1)
				pa = &lf02_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 2:
			if ((Drv_Par >= 0) && (Drv_Par <=6))
			{
				valid_par_range = 1;
				pa = &lf03_txt[Drv_Par][0];	
			}
			else
				valid_par_range = 0;

			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 3:
			if (valid_par_range == 1)
				pa = &lf04_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 4:
			if (valid_par_range == 1)
				pa = &on_off_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 5:
			LCD_Dig_Loc = 0;
			Drv_Par = (Drv_Par & 0x0ff);
			if (Drv_Par == 255)
				Drv_Par = 31;
			else if (Drv_Par > 30)
				Drv_Par = 30;
			valid_par_range = 1;	  // parameter range correct in this case
			pa = &lf26_0_txt[Drv_Par][0];	
			LCD_Dig_Loc = 1;
			flash_disable = 0;
			break;
		case 6:
			LCD_Dig_Loc = 0;
			Drv_Par = (Drv_Par >> 8) & 0x0ff;
			if (Drv_Par == 255)
				Drv_Par = 65;
			else if (Drv_Par > 64)
				Drv_Par = 64;
			valid_par_range = 1;	  // parameter range correct in this case
			pa = &lf26_1_txt[lf26_1_xref[Drv_Par]][0];
			flash_disable = 0;
			break;
		case 7:
			LCD_Dig_Loc = 0;
			Drv_Par = (Drv_Par >> 16) & 0x0ff;
			if (Drv_Par == 255)
			{
				Drv_Par = 100;
				pa = &lf26_2_txt[lf26_2_xref[Drv_Par]][0];
			}
			else if (Drv_Par > 98)
			{
				if (Drv_Par >= 128)
				{
					Drv_Par = Drv_Par - 128; 
					pa = &lf26_2_txt[lf26_2_xref[99]][0];
				}
				else
				{
					Drv_Par = 1;
					pa = &lf26_2_txt[lf26_2_xref[Drv_Par]][0];
					numeric_dpy = 1;
				}
			}
			else
				pa = &lf26_2_txt[lf26_2_xref[Drv_Par]][0];
			valid_par_range = 1;	  // parameter range correct in this case
			flash_disable = 0;
			break;
		case 8:
			if (valid_par_range == 1)
				pa = &lf98_99_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			if ((Mod_Drv_Par == 1) && (Drv_Par != 10))
			{
				numeric_dpy = 1;
				flash_disable = 0;
			}
			else
				flash_disable = 1;

			break;
		case 9:
			if (valid_par_range == 1)
				pa = &us36_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 10:
			if (valid_par_range == 1)
				pa = &ld26_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 11:
			if (valid_par_range == 1)
				pa = &lp01_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 12:
			if (valid_par_range == 1)
				pa = &di00_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 13:
			if (valid_par_range == 1)
				pa = &do42_txt[Drv_Par][0];	
			LCD_Dig_Loc = 1;
			flash_disable = 0;
			break;
		case 14:
			if (valid_par_range == 1)
				pa = &out_sel_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 15:
			valid_par_range = 1;
			if (Drv_Par >= 9)
				Drv_Par = 9;
			else
				pa = &us03_04_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 1;
			break;
		case 16:
			if (valid_par_range == 1)
				pa = &lf29_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 17:
			if (valid_par_range == 1)
				pa = &lf38_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 18:
			if (valid_par_range == 1)
				pa = &lf61_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 19:
			LCD_Dig_Loc = 0;
			if (Drv_Par == -7)
			{
				pa = &us01_txt[0][0];	
				flash_disable = 1;
				valid_par_range = 1;
			}
			else if (Drv_Par == -4)
			{
				pa = &us01_txt[1][0];	
				flash_disable = 1;
				valid_par_range = 1;
			}
			else
			{
				numeric_dpy = 1;
				flash_disable = 0;
			}
			break;
		case 20:
			if (valid_par_range == 1)
				pa = &ld33_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 21:
			if (valid_par_range == 1)
				pa = &ld31_txt[Drv_Par][0];	
			LCD_Dig_Loc = 0;
			flash_disable = 0;
			break;
		case 22:
			LCD_Dig_Loc = 0;
			if (Drv_Par == 0)
			{
				pa = &on_off_txt[Drv_Par][0];	
				flash_disable = 1;
			}
			else
			{
				numeric_dpy = 1;
				flash_disable = 0;
			}
			break;
			 
		default:
			break;
	}


  	LCD_Flash_TMP[1] = pa[LCD_Dig_Loc];
	if (numeric_dpy == 1)
	{
		if (keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].max >= 0x10000)
			display_variable_11_digits(1,(int16)(keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].par_name_len + 2), keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].digit, Drv_Par, 1);
		else
			display_variable(1, (int16)(keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].par_name_len + 2), keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].digit, (int16)Drv_Par, 1);	// resets LCD_Flash_TMP[1]
	}
	else if (valid_par_range == 1)
	{
	    for(i=0; i<=23; i++)
	    {
			if (drv_par_ret == 1)
				LCD_Display[1][i] = pa[i];
		}
	}
	else
	{
	    for(i=0; i<=23; i++)
	    {
			LCD_Display[1][i] = par_range[i];
		}
		display_variable(1, 12, keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].digit, (int16)Drv_Par, 0);
	}
}

void set_KEB_par_min_max (void)
{
	if ((drv_mode >= keb_drv_par_tbl[0].par[2].min) && (drv_mode <= keb_drv_par_tbl[0].par[2].max))
	{
		switch (drv_mode)
		{
			case 1:			// IM Geared
				keb_drv_par_tbl[1].par[2].rd_wr = 1;	 // LF.10
				keb_drv_par_tbl[1].par[3].max = 60000;	 // LF.11
				keb_drv_par_tbl[1].par[5].max = 2000;	 // LF.13
				keb_drv_par_tbl[1].par[6].max = 500;	 // LF.14
				keb_drv_par_tbl[1].par[7].dpy = 1;	 	 // LF.15
				keb_drv_par_tbl[1].par[8].dpy = 1;	 	 // LF.16
				keb_drv_par_tbl[1].par[9].rd_wr = 0;	 // LF.17
				keb_drv_par_tbl[1].par[10].dpy = 0;	 	 // LF.18
				keb_drv_par_tbl[1].par[11].dpy = 0;	 	 // LF.19
				keb_drv_par_tbl[2].par[5].dpy = 1;	 	 // LF.25
				keb_drv_par_tbl[3].par[6].min = 0;	 	 // LF.30
				keb_drv_par_tbl[4].par[12].dpy = 1;	 	 // LF.37
				keb_drv_par_tbl[6].par[10].dpy = 0;	 	 // LF.77
				keb_drv_par_tbl[0].par[9].dpy = 0;	 	 // LF.77
				keb_drv_par_tbl[10].par[0].dpy = 1;	 	 // Ld.18
				keb_drv_par_tbl[10].par[1].dpy = 1;	 	 // Ld.19
				keb_drv_par_tbl[10].par[2].dpy = 1;	 	 // Ld.20
				keb_drv_par_tbl[10].par[3].dpy = 1;	 	 // Ld.21
				keb_drv_par_tbl[10].par[4].dpy = 1;	 	 // Ld.22
				keb_drv_par_tbl[10].par[5].max = 32767;  // Ld.23
				keb_drv_par_tbl[10].par[5].digit = 1; 	 // Ld.23
				keb_drv_par_tbl[10].par[5].dpy = 1; 	 // Ld.23
				keb_drv_par_tbl[10].par[8].dpy = 0;	 	 // Ld.26
				keb_drv_par_tbl[10].par[12].max = 1073741823;	 	 // Ld.30
				break;
			case 2:			// IM Gearless
				keb_drv_par_tbl[1].par[2].rd_wr = 1;	 // LF.10
				keb_drv_par_tbl[1].par[3].max = 5000;	 // LF.11
				keb_drv_par_tbl[1].par[5].max = 500;	 // LF.13
				keb_drv_par_tbl[1].par[6].max = 500;	 // LF.14
				keb_drv_par_tbl[1].par[7].dpy = 1;	 	 // LF.15
				keb_drv_par_tbl[1].par[8].dpy = 1;	 	 // LF.16
				keb_drv_par_tbl[1].par[9].rd_wr = 0;	 // LF.17
				keb_drv_par_tbl[1].par[10].dpy = 0;	 	 // LF.18
				keb_drv_par_tbl[1].par[11].dpy = 0;	 	 // LF.19
				keb_drv_par_tbl[2].par[5].dpy = 0;	 	 // LF.25
				keb_drv_par_tbl[3].par[6].min = 0;	 	 // LF.30
				keb_drv_par_tbl[4].par[12].dpy = 1;	 	 // LF.37
				keb_drv_par_tbl[6].par[10].dpy = 0;	 	 // LF.77
				keb_drv_par_tbl[0].par[9].dpy = 0;	 	 // LF.77
				keb_drv_par_tbl[10].par[0].dpy = 1;	 	 // Ld.18
				keb_drv_par_tbl[10].par[1].dpy = 1;	 	 // Ld.19
				keb_drv_par_tbl[10].par[2].dpy = 1;	 	 // Ld.20
				keb_drv_par_tbl[10].par[3].dpy = 1;	 	 // Ld.21
				keb_drv_par_tbl[10].par[4].dpy = 1;	 	 // Ld.22
				keb_drv_par_tbl[10].par[5].max = 32767;  // Ld.23
				keb_drv_par_tbl[10].par[5].digit = 1; 	 // Ld.23
				keb_drv_par_tbl[10].par[5].dpy = 1; 	 // Ld.23
				keb_drv_par_tbl[10].par[8].dpy = 0;	 	 // Ld.26
				keb_drv_par_tbl[10].par[12].max = 1073741823;	 	 // Ld.30
				break;
			case 3:			// PM Geared
				keb_drv_par_tbl[1].par[2].rd_wr = 0;	 // LF.10
				keb_drv_par_tbl[1].par[3].max = 60000;	 // LF.11
				keb_drv_par_tbl[1].par[5].max = 2000;	 // LF.13
				keb_drv_par_tbl[1].par[6].max = 32000;	 // LF.14
				keb_drv_par_tbl[1].par[7].dpy = 0;	 	 // LF.15
				keb_drv_par_tbl[1].par[8].dpy = 0;	 	 // LF.16
				keb_drv_par_tbl[1].par[9].rd_wr = 1;	 // LF.17
				keb_drv_par_tbl[1].par[10].dpy = 1;	 	 // LF.18
				keb_drv_par_tbl[1].par[11].dpy = 1;	 	 // LF.19
				keb_drv_par_tbl[2].par[5].dpy = 1;	 	 // LF.25
				keb_drv_par_tbl[3].par[6].min = 2;	 	 // LF.30
				keb_drv_par_tbl[4].par[12].dpy = 0;	 	 // LF.37
				keb_drv_par_tbl[6].par[10].dpy = 1;	 	 // LF.77
				keb_drv_par_tbl[0].par[9].dpy = 1;	 	 // LF.77
				keb_drv_par_tbl[10].par[0].dpy = 0;	 	 // Ld.18
				keb_drv_par_tbl[10].par[1].dpy = 0;	 	 // Ld.19
				keb_drv_par_tbl[10].par[2].dpy = 0;	 	 // Ld.20
				keb_drv_par_tbl[10].par[3].dpy = 0;	 	 // Ld.21
				keb_drv_par_tbl[10].par[4].dpy = 0;	 	 // Ld.22
				keb_drv_par_tbl[10].par[5].max = 50000;  // Ld.23
				keb_drv_par_tbl[10].par[5].digit = 2; 	 // Ld.23
				keb_drv_par_tbl[10].par[5].dpy = 1; 	 // Ld.23
				keb_drv_par_tbl[10].par[8].dpy = 1;	 	 // Ld.26
				keb_drv_par_tbl[10].par[12].max = 107374; 	 // Ld.30
				break;
			case 4:			// PM Gearless
				keb_drv_par_tbl[1].par[2].rd_wr = 0;	 // LF.10
				keb_drv_par_tbl[1].par[3].max = 5000;	 // LF.11
				keb_drv_par_tbl[1].par[5].max = 2000;	 // LF.13
				keb_drv_par_tbl[1].par[6].max = 32000;	 // LF.14
				keb_drv_par_tbl[1].par[7].dpy = 0;	 	 // LF.15
				keb_drv_par_tbl[1].par[8].dpy = 0;	 	 // LF.16
				keb_drv_par_tbl[1].par[9].rd_wr = 1;	 // LF.17
				keb_drv_par_tbl[1].par[10].dpy = 1;	 	 // LF.18
				keb_drv_par_tbl[1].par[11].dpy = 1;	 	 // LF.19
				keb_drv_par_tbl[2].par[5].dpy = 0;	 	 // LF.25
				keb_drv_par_tbl[3].par[6].min = 2;	 	 // LF.30
				keb_drv_par_tbl[4].par[12].dpy = 0;	 	 // LF.37
				keb_drv_par_tbl[6].par[10].dpy = 1;	 	 // LF.77
				keb_drv_par_tbl[0].par[9].dpy = 1;	 	 // LF.77
				keb_drv_par_tbl[10].par[0].dpy = 0;	 	 // Ld.18
				keb_drv_par_tbl[10].par[1].dpy = 0;	 	 // Ld.19
				keb_drv_par_tbl[10].par[2].dpy = 0;	 	 // Ld.20
				keb_drv_par_tbl[10].par[3].dpy = 0;	 	 // Ld.21
				keb_drv_par_tbl[10].par[4].dpy = 0;	 	 // Ld.22
				keb_drv_par_tbl[10].par[5].max = 50000;  // Ld.23
				keb_drv_par_tbl[10].par[5].digit = 2; 	 // Ld.23
				keb_drv_par_tbl[10].par[5].dpy = 1; 	 // Ld.23
				keb_drv_par_tbl[10].par[8].dpy = 1;	 	 // Ld.26
				keb_drv_par_tbl[10].par[12].max = 107374; 	 // Ld.30
				break;
			default:
				break;
		}
	}
}


// *********************************************************
// When the drive packet is returned get the parameter data
// *********************************************************

static int16 Get_Drv_Par(int32 *Drv_Par)
{
	int16 i;
  	i = get_pkt_status('G', 2, DRVCOM, 20);
	*Drv_Par = drv_rpar_data;
    return i;	
}

// ***********************************************
// Request the drive parameter from the Delta drive
// ***********************************************

static int16 Req_Drv_Par (int16 Drv_Par_Grp_Ptr,int16 Drv_Par_Ptr)
{	  // Read drive parameter
	drv_rpar_addr = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].address;
	drv_rpar_set = keb_drv_par_tbl[Drv_Par_Grp_Ptr].par[Drv_Par_Ptr].set;
	put_pkt_req('G',2,DRVCOM);
	drv_nak = 0;
	drv_eot = 0;
	drv_error = 0;
	return 1;
}

void verify_run_mode (void)
{
static int time_out_cnt;
int i;

#if (Simulator == 1)
	keb_run_verify_seq = 3;
#endif

	if (keb_run_verify_seq == 0)
	{	// ready and waiting for command to verify run mode
	}
	else if (keb_run_verify_seq == 1)
	{
		drv_rpar_addr = 0x0383;
		drv_rpar_set = 1;
		put_pkt_req('G',2,DRVCOM);
		drv_nak = 0;
		drv_eot = 0;
		drv_error = 0;
		keb_run_verify_seq = 2;
		time_out_cnt = 0;
	}
	else if (keb_run_verify_seq == 2)
	{
  		i = get_pkt_status('G', 2, DRVCOM, 20);
		if (i == 1)
		{			   // lf.03 data read in
			drv_par_req = 0;
			Up_Date_LCD = 1;
			time_out_cnt = 0;
			if (drv_rpar_data == 0)
			{
				keb_run_verify_seq = 3;
				statusf2 &= ~sf_KEBRUN;
			}
			else 
			{
				keb_run_verify_seq = 1;
	
				drv_wpar_addr = 0x0383;
				drv_wpar_set = 1;
				drv_wpar_data = 0;
				put_pkt_req('g',2,DRVCOM);	 // put drive in run mode
				drv_nak = 0;
				drv_eot = 0;
				drv_error = 0;
			}
		}
		else if ((i == 2) || (i == -1))
		{
			if (time_out_cnt < 3)
			{		   // packete timed out so try again
				time_out_cnt ++;
				drv_rpar_addr = 0x0383;
				drv_rpar_set = 1;
				put_pkt_req('G',2,DRVCOM);
				drv_nak = 0;
				drv_eot = 0;
				drv_error = 0;
				keb_run_verify_seq = 2;
			}
			else if (time_out_cnt < 4)
			{
				if ((statusf2 & sf_KEBRUN) == 0)
					record_fault(f_keb_no_run_mode);
				time_out_cnt++;
				keb_run_verify_seq = 4;
				timers[tkebrun] = 0;
			}
		}
	}
	else if (keb_run_verify_seq == 3)
	{		// run mode verified
		statusf2 &= ~sf_KEBRUN;
	}
	else if (keb_run_verify_seq == 4)
	{		// run mode failed
		statusf2 |= sf_KEBRUN;
		if (timers[tkebrun] > 10)
			keb_run_verify_seq = 1;
	}

}

/* Revision History
  4/21/09 v5.48.3 mhd		1. Initial release.
  5/17/11 v5.54.02 mhd		1. Change config mode to stop mode.
							2. Added run mode verify check.

*/








