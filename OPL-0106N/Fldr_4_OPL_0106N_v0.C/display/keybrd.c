/* ============================================================ 
This code is adapted from the keyboard GCC files
============================================================ */
#define d_KEYBRD 1
#include "global.h"


int16 scancode = 0;
int16 kb_int_cnt = 0;
int16 key_code[10]={0,0,0,0,0,0,0,0,0,0};
int16 key_pix = 0;
int16 key_gix = 0;

int16 key_enter=0;
int16 key_status=0;

void keybrd_control(void);
int16 key_brd (int16 cmd);
void keybrd_init(void);

#define	PRIVATE	static

#ifndef TRUE
#define	TRUE		1
#endif
#ifndef FALSE
#define	FALSE		0
#endif

#define	ENTRIES(a)	(sizeof(a)/sizeof(a[0]))

typedef struct KEY_STATE
	{
	uint8	ins	;
	uint8	rshift	;
	uint8	lshift	;
	uint8	alt	;
	uint8	ctrl	;
	uint8	caps	;
	uint8	scrl	;
	uint8	num	;
	uint8 	tnum ;
	} key_state_type /* __attribute__((packed)) KEY_STATE*/ ;


uint8 SetsKybdState(uint16 code);
uint16 ScanCode2Ascii(uint16 code); 

#define	KYBD_STS_PORT	0x64
#define	KYBD_DAT_PORT	0x60

#define	KYBD_DAT_RDY	0x01
#define	BREAK_CODE	0x80


#define	SIZE	20

void keybrd_init(void)
{

	// Reset vectors to control keyboard and print screen
//	_disable();

    /* Install PIT3_isr into the interrupt vector table */
/*    INTC_InstallINTCInterruptHandler(keybrd_int, 57, 1);
	SIU.IFEER.B.IFEE13 = 1;		// Falling edge trigger
	SIU.IDFR.B.DFL = 7;			// 7 = 2^7 = 128 Active low for 1.1862 usec
	SIU.ISEL1.B.ESEL13 = 3;		// Get interrupt from ISEL2
	SIU.ISEL2.B.ESEL13 = 3;		// Get interrupt from ISEL2A
	SIU.ISEL2A.B.ESEL13 = 1;	// Set for PJ13
	SIU.EISR.B.EIF13 = 1;		// Reset any pending interrupt flags
	SIU.DIRER.B.EIRE13 = 1;		// Enable interrupt on PJ13
*/
	rd_keybrd();			// clear buffer to enable interrupts
	rd_keybrd();			// clear buffer to enable interrupts
	rd_keybrd();			// clear buffer to enable interrupts
//	_enable();
}



struct KEY_STATE key_state ; /* Initialized to all FALSE by loader */

//PRIVATE BYTE8		scan_codes[SIZE] ;
//PRIVATE unsigned int	nq = 0 ;
//PRIVATE unsigned int	dq = 0 ;
//PRIVATE unsigned int	count = 0 ;

/* -------------------------------------------------------------------------------------- */
/* Scan code translation table.	                                                          */
/* The incoming scan code from the keyboard selects a row.                                */
/* The modifier status selects the column.                                                */
/* The word at the intersection of the two is the scan/ASCII code to                      */
/* put into the PC's type ahead buffer.                                                   */
/* If the value fetched from the table is zero, then we do not put the                    */
/* character into the type ahead buffer.                                                  */
/* -------------------------------------------------------------------------------------- */
uint16 scan_ascii[][8] =
	{
		    /* norm,  shft,   ctrl,    alt,    num,   caps,   shcap,  shnum */
/*--*/		{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/*ESC*/		{0x011B, 0x011B, 0x011B, 0x011B, 0x011B, 0x011B, 0x011B, 0x011B},
/*1 !*/		{0x0231, 0x0221, 0x0000, 0x7800, 0x0231, 0x0231, 0x0231, 0x0321},
/*2 @*/		{0x0332, 0x0340, 0x0300, 0x7900, 0x0332, 0x0332, 0x0332, 0x0332},
/*3 #*/		{0x0433, 0x0423, 0x0000, 0x7A00, 0x0433, 0x0433, 0x0423, 0x0423},
/*4 $*/		{0x0534, 0x0524, 0x0000, 0x7B00, 0x0534, 0x0534, 0x0524, 0x0524},
/*5 %*/		{0x0635, 0x0625, 0x0000, 0x7C00, 0x0635, 0x0635, 0x0625, 0x0625},
/*6 ^*/		{0x0736, 0x075E, 0x071E, 0x7D00, 0x0736, 0x0736, 0x075E, 0x075E},
/*7 &*/		{0x0837, 0x0826, 0x0000, 0x7E00, 0x0837, 0x0837, 0x0826, 0x0826},
/*8 **/		{0x0938, 0x092A, 0x0000, 0x7F00, 0x0938, 0x0938, 0x092A, 0x092A},
/*9 (*/		{0x0A39, 0x0A28, 0x0000, 0x8000, 0x0A39, 0x0A39, 0x0A28, 0x0A28},
/*0 )*/		{0x0B30, 0x0B29, 0x0000, 0x8100, 0x0B30, 0x0B30, 0x0B29, 0x0B29},
/*- _*/		{0x0C2D, 0x0C5F, 0x0000, 0x8200, 0x0C2D, 0x0C2D, 0x0C5F, 0x0C5F},
/*= +*/		{0x0D3D, 0x0D2B, 0x0000, 0x8300, 0x0D3D, 0x0D3D, 0x0D2B, 0x0D2B},
/*bksp*/	{0x0E08, 0x0E08, 0x0E7F, 0x0000, 0x0E08, 0x0E08, 0x0E08, 0x0E08},
/*Tab*/		{0x0F09, 0x0F00, 0x0000, 0x0000, 0x0F09, 0x0F09, 0x0F00, 0x0F00},
/*Q*/		{0x1071, 0x1051, 0x1011, 0x1000, 0x1071, 0x1051, 0x1051, 0x1071},
/*W*/		{0x1177, 0x1057, 0x1017, 0x1100, 0x1077, 0x1057, 0x1057, 0x1077},
/*E*/		{0x1265, 0x1245, 0x1205, 0x1200, 0x1265, 0x1245, 0x1245, 0x1265},
/*R*/		{0x1372, 0x1352, 0x1312, 0x1300, 0x1272, 0x1252, 0x1252, 0x1272},
/*T*/		{0x1474, 0x1454, 0x1414, 0x1400, 0x1474, 0x1454, 0x1454, 0x1474},
/*Y*/		{0x1579, 0x1559, 0x1519, 0x1500, 0x1579, 0x1559, 0x1579, 0x1559},
/*U*/		{0x1675, 0x1655, 0x1615, 0x1600, 0x1675, 0x1655, 0x1675, 0x1655},
/*I*/		{0x1769, 0x1749, 0x1709, 0x1700, 0x1769, 0x1749, 0x1769, 0x1749},
/*O*/		{0x186F, 0x184F, 0x180F, 0x1800, 0x186F, 0x184F, 0x186F, 0x184F},
/*P*/		{0x1970, 0x1950, 0x1910, 0x1900, 0x1970, 0x1950, 0x1970, 0x1950},
/*[ {*/		{0x1A5B, 0x1A7B, 0x1A1B, 0x0000, 0x1A5B, 0x1A5B, 0x1A7B, 0x1A7B},
/*] }*/		{0x1B5D, 0x1B7D, 0x1B1D, 0x0000, 0x1B5D, 0x1B5D, 0x1B7D, 0x1B7D},
/*entr*/	{0x1C0D, 0x1C0D, 0x1C0A, 0x0000, 0x1C0D, 0x1C0D, 0x1C0D, 0x1C0D},
/*ctrl*/	{0x1D00, 0x1D00, 0x1D00, 0x1D00, 0x1D00, 0x1D00, 0x1D00, 0x1D00},
/*A*/   	{0x1E61, 0x1E41, 0x1E01, 0x1E00, 0x1E61, 0x1E41, 0x1E61, 0x1E41},
/*S*/		{0x1F73, 0x1F53, 0x1F13, 0x1F00, 0x1F73, 0x1F53, 0x1F73, 0x1F53},
/*D*/		{0x2064, 0x2044, 0x2004, 0x2000, 0x2064, 0x2044, 0x2064, 0x2044},
/*F*/		{0x2166, 0x2146, 0x2106, 0x2100, 0x2166, 0x2146, 0x2166, 0x2146},
/*G*/		{0x2267, 0x2247, 0x2207, 0x2200, 0x2267, 0x2247, 0x2267, 0x2247},
/*H*/		{0x2368, 0x2348, 0x2308, 0x2300, 0x2368, 0x2348, 0x2368, 0x2348},
/*J*/		{0x246A, 0x244A, 0x240A, 0x2400, 0x246A, 0x244A, 0x246A, 0x244A},
/*K*/		{0x256B, 0x254B, 0x250B, 0x2500, 0x256B, 0x254B, 0x256B, 0x254B},
/*L*/		{0x266C, 0x264C, 0x260C, 0x2600, 0x266C, 0x264C, 0x266C, 0x264C},
/*; :*/		{0x273B, 0x273A, 0x0000, 0x0000, 0x273B, 0x273B, 0x273A, 0x273A},
/*' "*/		{0x2827, 0x2822, 0x0000, 0x0000, 0x2827, 0x2827, 0x2822, 0x2822},
/*` ~*/		{0x2960, 0x297E, 0x0000, 0x0000, 0x2960, 0x2960, 0x297E, 0x297E},
/*LShf*/	{0x2A00, 0x2A00, 0x2A00, 0x2A00, 0x2A00, 0x2A00, 0x2A00, 0x2A00},
/*\ |*/		{0x2B5C, 0x2B7C, 0x2B1C, 0x0000, 0x2B5C, 0x2B5C, 0x2B7C, 0x2B7C},
/*Z*/		{0x2C7A, 0x2C5A, 0x2C1A, 0x2C00, 0x2C7A, 0x2C5A, 0x2C7A, 0x2C5A},
/*X*/		{0x2D78, 0x2D58, 0x2D18, 0x2D00, 0x2D78, 0x2D58, 0x2D78, 0x2D58},
/*C*/		{0x2E63, 0x2E43, 0x2E03, 0x2E00, 0x2E63, 0x2E43, 0x2E63, 0x2E43},
/*V*/		{0x2F76, 0x2F56, 0x2F16, 0x2F00, 0x2F76, 0x2F56, 0x2F76, 0x2F56},
/*B*/		{0x3062, 0x3042, 0x3002, 0x3000, 0x3062, 0x3042, 0x3062, 0x3042},
/*N*/		{0x316E, 0x314E, 0x310E, 0x3100, 0x316E, 0x314E, 0x316E, 0x314E},
/*M*/		{0x326D, 0x324D, 0x320D, 0x3200, 0x326D, 0x324D, 0x326D, 0x324D},
/*, <*/		{0x332C, 0x333C, 0x0000, 0x0000, 0x332C, 0x332C, 0x333C, 0x333C},
/*. >*/		{0x342E, 0x343E, 0x0000, 0x0000, 0x342E, 0x342E, 0x343E, 0x343E},
/*/ ?*/		{0x352F, 0x353F, 0x0000, 0x0000, 0x352F, 0x352F, 0x353F, 0x353F},
/*rshf*/	{0x3600, 0x3600, 0x3600, 0x3600, 0x3600, 0x3600, 0x3600, 0x3600},
/** PS*/	{0x372A, 0x0000, 0x3710, 0x0000, 0x372A, 0x372A, 0x0000, 0x0000},
/*alt*/		{0x3800, 0x3800, 0x3800, 0x3800, 0x3800, 0x3800, 0x3800, 0x3800},
/*spc*/		{0x3920, 0x3920, 0x3920, 0x0000, 0x3920, 0x3920, 0x3920, 0x3920},
/*caps*/	{0x3A00, 0x3A00, 0x3A00, 0x3A00, 0x3A00, 0x3A00, 0x3A00, 0x3A00},
/*F1*/		{0x3B00, 0x5400, 0x5E00, 0x6800, 0x3B00, 0x3B00, 0x5400, 0x5400},
/*F2*/		{0x3C00, 0x5500, 0x5F00, 0x6900, 0x3C00, 0x3C00, 0x5500, 0x5500},
/*F3*/		{0x3D00, 0x5600, 0x6000, 0x6A00, 0x3D00, 0x3D00, 0x5600, 0x5600},
/*F4*/		{0x3E00, 0x5700, 0x6100, 0x6B00, 0x3E00, 0x3E00, 0x5700, 0x5700},
/*F5*/		{0x3F00, 0x5800, 0x6200, 0x6C00, 0x3F00, 0x3F00, 0x5800, 0x5800},
/*F6*/		{0x4000, 0x5900, 0x6300, 0x6D00, 0x4000, 0x4000, 0x5900, 0x5900},
/*F7*/		{0x4100, 0x5A00, 0x6400, 0x6E00, 0x4100, 0x4100, 0x5A00, 0x5A00},
/*F8*/		{0x4200, 0x5B00, 0x6500, 0x6F00, 0x4200, 0x4200, 0x5B00, 0x5B00},
/*F9*/		{0x4300, 0x5C00, 0x6600, 0x7000, 0x4300, 0x4300, 0x5C00, 0x5C00},
/*F10*/		{0x4400, 0x5D00, 0x6700, 0x7100, 0x4400, 0x4400, 0x5D00, 0x5D00},
/*num*/		{0x4500, 0x4500, 0x4500, 0x4500, 0x4500, 0x4500, 0x4500, 0x4500},
/*scrl*/	{0x4600, 0x4600, 0x4600, 0x4600, 0x4600, 0x4600, 0x4600, 0x4600},
/*home*/	{0x4700, 0x4737, 0x7700, 0x0000, 0x4737, 0x4700, 0x4737, 0x4700},
/*up*/		{0x4800, 0x4838, 0x0000, 0x0000, 0x4838, 0x4800, 0x4838, 0x4800},
/*pgup*/	{0x4900, 0x4939, 0x8400, 0x0000, 0x4939, 0x4900, 0x4939, 0x4900},
/*-*/		{0x4A2D, 0x4A2D, 0x0000, 0x0000, 0x4A2D, 0x4A2D, 0x4A2D, 0x4A2D},
/*left*/	{0x4B00, 0x4B00, 0x7300, 0x4B00, 0x4B00, 0x4B00, 0x4B00, 0x4B00},
/*Cntr*/	{0x4C00, 0x4C35, 0x0000, 0x0000, 0x4C35, 0x4C00, 0x4C35, 0x4C00},
/*rght*/	{0x4D00, 0x4D00, 0x7400, 0x4D00, 0x4D00, 0x4D00, 0x4D00, 0x4D00},
/*+*/		{0x4E2B, 0x4E2B, 0x0000, 0x0000, 0x4E2B, 0x4E2B, 0x4E2B, 0x4E2B},
/*end*/		{0x4F00, 0x4F31, 0x7500, 0x0000, 0x4F31, 0x4F00, 0x4F31, 0x4F00},
/*down*/	{0x5000, 0x5032, 0x0000, 0x0000, 0x5032, 0x5000, 0x5032, 0x5000},
/*pgdn*/	{0x5100, 0x5133, 0x7600, 0x0000, 0x5133, 0x5100, 0x5133, 0x5100},
/*ins*/		{0x5200, 0x5230, 0x0000, 0x0000, 0x5230, 0x5200, 0x5230, 0x5200},
/*del*/		{0x5300, 0x532E, 0x0000, 0x0000, 0x532E, 0x5300, 0x532E, 0x5300},
/*--*/		{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/*--*/		{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/*--*/		{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/*F11*/		{0x5700, 0x0000, 0x0000, 0x0000, 0x5700, 0x5700, 0x0000, 0x0000},
/*F12*/		{0x5800, 0x0000, 0x0000, 0x0000, 0x5800, 0x5800, 0x0000, 0x0000}
	} ;


unsigned char raw_key [] = 
{//    0,   1,   2,   3,   4,   5,   6,   7,   8    9    A,   B,   C,   D,   E,   F
	0x00,0x43,0x00,0x3F,0x3D,0x3B,0x3C,0x58,0x00,0x44,0x42,0x40,0x3E,0x0F,0x29,0x00,
  //  10,  11,  12   13   14   15   16   17,  18,  19   1A,  1B,  1C,  1D,  1E,  1F
    0x00,0x38,0x2A,0x00,0x1D,0x10,0x02,0x00,0x00,0x00,0x2C,0x1F,0x1E,0x11,0x03,0x00,
  //  20,  21,  22   23   24   25   26   27,  28,  29   2A,  2B,  2C,  2D,  2E,  2F
    0x00,0x2E,0x2D,0x20,0x12,0x05,0x04,0x00,0x00,0x39,0x2F,0x21,0x14,0x13,0x06,0x00,
  //  30,  31,  32   33   34   35   36   37,  38,  39   3A,  3B,  3C,  3D,  3E,  3F
    0x00,0x31,0x30,0x23,0x22,0x15,0x07,0x00,0x00,0x00,0x32,0x24,0x16,0x08,0x09,0x00,
  //  40,  41,  42   43   44   45   46   47,  48,  49   4A,  4B,  4C,  4D,  4E,  4F
    0x00,0x33,0x25,0x17,0x18,0x0B,0x0A,0x00,0x00,0x34,0x35,0x26,0x27,0x19,0x0C,0x00,
  //  50,  51,  52   53   54   55   56   57,  58,  59   5A,  5B,  5C,  5D,  5E,  5F
    0x00,0x00,0x28,0x00,0x1A,0x0D,0x00,0x00,0x3A,0x36,0x1C,0x1B,0x00,0x2B,0x00,0x00,
  //  60,  61,  62   63   64   65   66   67,  68,  69   6A,  6B,  6C,  6D,  6E,  6F
    0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x02,0x00,0x05,0x08,0x00,0x00,0x00,
  //  70,  71,  72   73   74   75   76   77,  78,  79   7A,  7B,  7C,  7D,  7E,  7F
    0x0B,0x34,0x03,0x06,0x07,0x09,0x01,0x45,0x57,0x4E,0x04,0x4A,0x37,0x0A,0x00,0x00,
  //  80,  81,  82   83   84   85   86   87,  88,  89   8A,  8B,  8C,  8D,  8E,  8F
    0x00,0x00,0x00,0x41
};
    	


uint16 ScanCode2Ascii(uint16 code) 
{
	if (code == 0xE0 || code == 0xE1)	return 0x0000 ; /* ignore */
	if (code & 0x80)			return code << 8 ; /* KeyUp */
	if (code >= ENTRIES(scan_ascii))	return 0x0000 ; /* ignore */

	if (key_state.alt)	return scan_ascii[code][3] ;
	if (key_state.ctrl)	return scan_ascii[code][2] ;
	if (code >= 0x47) {
		if (key_state.num) {
			if (key_state.lshift || key_state.rshift) {
				return scan_ascii[code][7] ;
			}
			return scan_ascii[code][4] ;
		}
	} else if (key_state.caps) {
		if (key_state.lshift || key_state.rshift) {
			return scan_ascii[code][6] ;
		}
		return scan_ascii[code][5] ;
	}

	if (key_state.lshift || key_state.rshift) {
		return scan_ascii[code][1] ;
	}

	return scan_ascii[code][0] ;
}

uint8 SetsKybdState(uint16 code)
	{
	switch (code)
		{
		case 0x36: key_state.rshift = TRUE  ; break ;
		case 0xB6: key_state.rshift = FALSE ; break ;

		case 0x2A: key_state.lshift = TRUE  ; break ;
		case 0xAA: key_state.lshift = FALSE ; break ;

		case 0x38: key_state.alt    = TRUE  ; break ;
		case 0xB8: key_state.alt	= FALSE ; break ;

		case 0x1D: key_state.ctrl	= TRUE  ; break ;
		case 0x9D: key_state.ctrl	= FALSE ; break ;

		case 0x3A: key_state.caps = !key_state.caps ;
		//lint -fallthrough
		case 0xBA: break ;

		case 0x46: key_state.scrl = !key_state.scrl ;
		//lint -fallthrough
		case 0xC6: break ;

		case 0x45: key_state.num  = !key_state.num  ;
		//lint -fallthrough
		case 0xC5: break ;

		case 0x52: key_state.ins  = !key_state.ins  ;
		//lint -fallthrough
		case 0xD2: break ;

		default:   return FALSE ;
		}

	return TRUE ;
	}


unsigned char special_key = 0;
unsigned char key_release = 0;
uint16 rawcode[100];
uint16 p_rix = 0;
uint16 g_rix = 0;

//*******************************************
// Keyboard intercept key routine
//*******************************************

void keybrd_control(void)
{
	
	
//	SIU.EISR.B.EIF13 = 1;		// Reset any pending interrupt flags
	kb_int_cnt = 0;
	while ((SIU.GPDI[KYB_IRQn].B.PDI == 0) && (kb_int_cnt < 5))
	{
		rawcode[p_rix] = rd_keybrd();
		if (rawcode[p_rix] == 0xAA) 
		{
			wr_keybrd(0xF6);
			scancode = 0;
			return;
		}

		if (rawcode[p_rix] == 0xF0) 
		{
			key_release = 1;
			special_key = 0;
			scancode = 0;
		}
		else if (rawcode[p_rix] == 0xE0)
		{
			special_key = 1;
			scancode = 0;
		}
		else if ((rawcode[p_rix] == 0x59) && (key_release == 1))		// right shift key release
		{
			key_release = 0;
			scancode = 0xB6;
		}
		else if ((rawcode[p_rix] == 0x12)  && (key_release == 1))		// left shift key release
		{
			key_release = 0;
			scancode = 0xAA;		
		}
		else if ((rawcode[p_rix] == 0x14) && (key_release == 1))		// Left or right control key release
		{
			key_release = 0;
			scancode = 0x9D;
		}
		else if ((rawcode[p_rix] == 0x11) && (key_release == 1))		// left or right alt key release
		{
			key_release = 0;
			scancode = 0xB8;
		}
		else if (key_release == 1) 
		{
			scancode = 0;
			key_release = 0;
		}
		else if (special_key == 1)
		{
			special_key = 0;
			if (rawcode[p_rix] == 0x11)
				scancode = 0x38;
			else if (rawcode[p_rix] == 0x12)
				scancode = 0x37;
			else if (rawcode[p_rix] == 0x14)
				scancode = 0x4C;
			else if (rawcode[p_rix] == 0x4A)
				scancode = 0x35;
			else if (rawcode[p_rix] == 0x5A)
				scancode = 0x1C;
			else if (rawcode[p_rix] == 0x69)
				scancode = 0x4F;
			else if (rawcode[p_rix] == 0x6B)
				scancode = 0x4B;
			else if (rawcode[p_rix] == 0x6C)
				scancode = 0x47;
			else if (rawcode[p_rix] == 0x70)
				scancode = 0x52;
			else if (rawcode[p_rix] == 0x71)
				scancode = 0x53;
			else if (rawcode[p_rix] == 0x72)
				scancode = 0x50;
			else if (rawcode[p_rix] == 0x74)
				scancode = 0x4D;
			else if (rawcode[p_rix] == 0x75)
				scancode = 0x48;
			else if (rawcode[p_rix] == 0x7A)
				scancode = 0x51;
			else if (rawcode[p_rix] == 0x7D)
				scancode = 0x49;
			else
				scancode = 0;
		}
		else if (rawcode[p_rix] < ENTRIES(raw_key))
		{
			scancode = raw_key[rawcode[p_rix]];
		}
		if (scancode != 0)
		{
			if (SetsKybdState(scancode) == FALSE)
			{
				key_code[key_pix] = ScanCode2Ascii(scancode);
				if (key_pix < 9)
					key_pix++;
				else
					key_pix = 0;
			
	 			key_enter = 1;
			}
		}
	
		if (p_rix < 99)
			p_rix++;
		else
			p_rix = 0;
		
		kb_int_cnt++;
	}
//	SIU.EISR.B.EIF13 = 1;		// Reset any pending interrupt flags
/*
	scancode = inp(0x60);
	key_status = inp(0x64);
	SetsKybdState(scancode);
	key_code = ScanCode2Ascii(scancode);
 	key_enter = 1;
 	outp(0x20,0x20);      	// Send EOI since old int 9 was not called
 */
}								

int16 key_brd (int16 cmd)
{
							// 	_KEYBRD_READ          0    Read character. 
							//	_KEYBRD_READY         1    Check character. 
							// 	_KEYBRD_SHIFTSTATUS   2    Get shift status. 
							//  _KEYBRD_ALTSTATUS 	  3	   Get alt status.
							//  _KEYBRD_SCANCODE	  4    Read the raw scan code
	int16 key_ret = 0;
	switch(cmd)
	{
		case 0:	
			key_ret = key_code[key_gix];
			key_code[key_gix] = 0;			// zero array byte to detect end of data
			if (key_gix < 9)
				key_gix++;
			else
				key_gix = 0;
			if (key_code[key_gix] == 0)
			{
				key_gix = 0;
				key_pix = 0;
				key_enter = 0;	// don't clear the key input command because there is more data
			}
			break;
		case 1:
			key_ret = key_enter;
			break;
		case 2:
			key_ret = ((key_state.rshift == TRUE) || (key_state.lshift == TRUE));
			break;
		case 3:
			key_ret = (key_state.alt == TRUE);
			break;
		case 4:
			key_ret = scancode;
			break;
		default:
			break;
	}
	return (key_ret);
}





