//  Initialize I/O name array
//  With this routine, the ionames are read from the code area and placed in a ram array.  This data array is setup for
//  the specific job and can be transmitted serially to the user interface pc.
// add IFL, IFB, DET, RDET 
#define d_IOINIT 1
#include "global.h"
#include "fat_filelib.h"

// input indexes
int16 i_bit_ix = 0;
int16 i_array_ix = 0;
int16 i_count = 0;

// output indexes
int16 o_bit_ix = 0;
int16 o_array_ix = 0;
int16 o_count = 0;

uint8 io_name_fault = 0;

char fiodatbuf[200];
char char_in[6];

int16 board;
int16 init_car = 1;


unsigned char iodata[car_size][nmb_io_arrays];
unsigned char ioset[nmb_io_arrays];
unsigned char ioclr[nmb_io_arrays];
unsigned char IO_In_Out[nmb_io_arrays];		// outputs are 1's inputs are 0's
unsigned char IO_board[car_size][nmb_io_brds];
char io_name[8];
char  ioname [nmb_ios][5];
unsigned char ionamechksum[nmb_io_brds];
unsigned char  io_valid[nmb_io_arrays];
unsigned char  io_send_name[nmb_io_arrays];
unsigned char io_on_color_ix[nmb_ios]; // 0=non,1=Sec,2=AttUP,3=AttDN,4=OffButton,5=OnButton,6=Fire,7=Medical,8=Emergency,9=OTS,4=Non-cc buttons
char  hcioname [nmb_hc_ios][5];
unsigned char  gio_valid[nmb_hc_io_arrays];
unsigned char GIO_In_Out[nmb_hc_io_arrays];		// outputs are 1's inputs are 0's
int16 HC_IO_board[nmb_hcio_brds];				// 0x01 = hc board, 0x02 = Serial io, 0x04 = ir, 0x08 = cb, 0x20 = vip
unsigned char  gio_send_name[nmb_hc_io_arrays];

int16 mr_last_array = 0;
int16 ct_last_array = 0;
int16 gr_last_array = 0;

char spare_ioname[18][5];
char spare_io_stat[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const char SPARE_IOS[18][5] = 
{
  "MSPI1",
  "MSPI2",
  "MSPI3",
  "MSPO1",
  "MSPO2",
  "MSPO3",
  "CSPI1",
  "CSPI2",
  "CSPI3",
  "CSPO1",
  "CSPO2",
  "CSPO3",
  "GSPI1",
  "GSPI2",
  "GSPI3",
  "GSPO1",
  "GSPO2",
  "GSPO3",
};


#define giodata(i) iodata[0][i]

extern const uint16 bittbl[16];





int16 io_bd_order[86] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
                        31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,
                        61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85};



#define Door_Test 0

#define INV_IO 0
#define MAINIO 1
#define MRCAN 2
#define CTCAN 3
#define GRCAN 4




/*
		Rules:		1. All i/o's are enabled on lthe main i/o board
					2. Voltage specific I/O's are placed first (i.e. Slowdown Limits, Door I/O, Cab lanterns)
					3. Car and Hall I/O can be 110 or 24 Volts
					4. Options i/o's for the car or hall must match the car or hall I/O voltages.
*/


#define NoLight 0
#define SecLight 1
#define AttUpLight 2
#define AttDnLight 3
#define OffLight 4
#define OnLight 5
#define FireLight 6
#define MedLight 7
#define EmLight 8
#define OTSLight 9



struct io {
	uint8 bus;
	uint8 bit;
	uint16 addr;
	uint16 array;
};



void set_input_start(int16 i_array_s, int16 i_bit_s);
void set_output_start(int16 o_array_s, int16 o_bit_s);
void set_grp_input_location(char *s_ioname);
void set_grp_output_location(char *s_ioname);
int16 next_grp_inp();
int16 next_grp_outp();
void set_xassignt_io (int16 last_array);

int16 Load_IO_Names (void);

int8 init_io (struct io *iop, uint8 bus, char *name, uint8 type, uint8 on_color);
int8 init_gio (struct io *iop, uint8 bus, char *name, uint8 type);
void gio_init (void);
void io_init(void);
//void set_io_board_status (void);
void init_ionames (void);
void init_iodata(void);
void init_ioname_chksum();
void load_spare_ios (void);
void remove_relocated_ios (void);

int16 count_bit(unsigned char bitnmb);


/*

iodata 						bus 0		
index 0					index[0] = address 
	bit1					bit1
	bit2					bit2	
	bit3					bit3
	bit4					bit4
	bit5					bit5
	bit6					bit6
	bit7					bit7
	bit8					bit8
1
2
3
.
.
.											bus 1
index n									index n + x  ct_serial.iobrd[x]
	bit1									bit1
	bit2									bit2	
	bit3									bit3
	bit4									bit4
	bit5									bit5
	bit6									bit6
	bit7									bit7
	bit8									bit8
n+1
n+2
n+3
.
.
.															bus 2
index m													index m + x  mr_serial.iobrd[x]
	bit1													bit1
	bit2													bit2	
	bit3													bit3
	bit4													bit4
	bit5													bit5
	bit6													bit6
	bit7													bit7
	bit8													bit8
m+1
m+2
m+3
m+4
.
.
.

*/

//*********************************************************************************************
// MAIN I/O BOARD
//*********************************************************************************************

struct io si_AD = {0};
struct io si_IND = {0};
struct io si_LBP = {0};
struct io si_GBP = {0};
struct io si_L120 = {0};		// local/lantern 120 vac on main i/o board "formerly LPH"
struct io si_S10 = {0};
struct io si_FEP = {0};
struct io si_EBK = {0};		// Emergency brake coil input

struct io si_GOV = {0};

struct io si_TF = {0};
struct io si_BF = {0};
struct io si_PS = {0};
struct io si_HSS = {0};
struct io si_ACC = {0};
struct io si_BAD = {0};
struct io si_BAU = {0};
struct io si_TAD = {0};

struct io si_TAU = {0};
struct io si_DLB = {0};
struct io si_DLB_1 = {0};
struct io si_DLM = {0};
struct io si_DLM_1 = {0};
struct io si_DLT = {0};
struct io si_DLT_1 = {0};
struct io si_RLM = {0};

struct io si_RLM_1 = {0};
struct io si_TDC = {0};
struct io si_MDC = {0};
struct io si_BDC = {0};
struct io si_MDCR = {0};
struct io si_BKS = {0};
struct io si_EBKS = {0};	// Emergency Brake Switch input
struct io si_SPI = {0};

struct io si_MRIU = {0};
struct io si_MRIN = {0};
struct io si_AUTO = {0};
struct io si_MRSW = {0};
struct io si_ICA = {0};
struct io si_MRID = {0};
struct io si_MRIE = {0};	// Machine room inspection input

struct io si_CEN = {0};
struct io si_SPD = {0};
struct io si_CFLT = {0};
struct io si_L120B = {0};
struct io si_NTSF = {0};
struct io si_PALF = {0};
struct io si_TSTM = {0};
struct io si_RDOOR = {0};
struct io si_TSD = {0};

struct io si_READY = {0};	// Drive Ready input
struct io si_GTS = {0};	// Gripper Trip Switch input
struct io si_GRT1 = {0};
struct io si_GRT2 = {0};
struct io si_RGS_1 = {0};
struct io si_GS = {0};
struct io si_GS_1 = {0};
struct io si_FST = {0};

struct io si_DN = {0};	// Down Directional Stop
struct io si_DZ = {0};	// Door Zone input (DZU AND DZD)
struct io si_DZ_1 = {0};	// Door Zone input (DZU AND DZD)
struct io si_RGS = {0};
struct io si_CTA = {0};
struct io si_DTS = {0};	// Down Emergeny Terminal Slowdown
struct io si_UN = {0};	// Up Directional Stop
struct io si_UTS = {0};	// Up Emergency Terminal Slowdown 

struct io si_ID = {0};
struct io si_INE = {0};
struct io si_IU = {0};
struct io si_INS = {0};
struct io si_P = {0};
struct io si_ICI = {0};
struct io si_FFS = {0};

struct io si_MCC = {0};
struct io si_MCA = {0};
struct io si_DON = {0};
struct io si_BRK = {0};
struct io si_RUN = {0};
struct io si_MCX = {0};
struct io si_RUNA = {0};
struct io si_RUNX = {0};

struct io si_SD = {0};
struct io si_SDF = {0};
struct io si_SU = {0};
struct io si_SUF = {0};
struct io si_NTS = {0};
struct io si_ETS = {0};


struct io so_CEN = {0};
struct io so_MTO = {0};
struct io so_STE = {0};
struct io so_UL = {0};
struct io so_DL = {0};
struct io so_DZ = {0};
struct io so_SELOK = {0};
struct io so_SP75 = {0};
struct io so_SP150 = {0};
struct io so_HWLRN = {0};


struct io so_FLH = {0};
struct io so_GR2R = {0};
struct io so_LE = {0};
struct io so_LE1 = {0};
struct io so_FST = {0};
struct io so_FSTP = {0};

struct io so_RST = {0};
struct io so_MCC = {0};
struct io so_MCA = {0};
struct io so_PFC = {0};
struct io so_DBG = {0};
struct io so_FF = {0};
struct io so_BUZ = {0};
struct io so_EBK1 = {0};

struct io so_RUNA = {0};
struct io so_RUN = {0};
struct io so_SD = {0};
struct io so_SDF = {0};
struct io so_SU = {0};
struct io so_SUF = {0};

struct io so_BRK = {0};
struct io so_EBK2 = {0};
struct io so_DBC = {0};
	


//*********************************************************************************************
// Machine Room Can I/O
//*********************************************************************************************

struct io si_EBK1 = {0};	// Emergency brake 1 coil input
struct io si_EBK2 = {0};	// Emergency brake 2 coil input
struct io si_EBA = {0};	// Emergency brake A coil input
struct io si_EBB = {0};	// Emergency brake B coil input

struct io si_LC = {0};	// LC 120VAC input 

struct io si_TPL = {0};
struct io si_TPH = {0};

struct io si_SEE = {0};
struct io si_TEE = {0};
struct io si_CMP = {0};
struct io si_BFS = {0};
struct io si_PDS = {0};
struct io si_HMO = {0};

struct io si_TGDO = {0};
struct io si_TGDOR = {0};
struct io si_TGDC = {0};
struct io si_TGDCR = {0};
struct io si_CCPBS = {0};

struct io si_XBKS1 = {0};
struct io si_BKS1 = {0}; 
struct io si_BKS2 = {0};
struct io si_BDBS = {0};


struct io si_RM_ED = {0};
struct io si_RM_EDR = {0};
struct io si_RM_INDC = {0}; 
struct io si_OSERA = {0};
struct io si_SAB = {0};
struct io si_SECFM = {0};

struct io si_SCSMR = {0};
struct io si_SCSRMR = {0};

struct io si_BP1 = {0};
struct io si_BP2 = {0};
struct io si_BP4 = {0};
struct io si_BP8 = {0};
struct io si_BP16 = {0};
struct io si_BP32 = {0};


struct io si_LPS = {0};
struct io si_LOS = {0};
struct io si_RM_DOB = {0};
struct io si_RM_DCB = {0};
struct io si_RM_DCBR = {0};
struct io si_RM_DOBR = {0};

struct io si_EMP = {0};
struct io si_EPS = {0};
struct io si_EPT = {0};
struct io si_HEOF = {0};
struct io si_HEOFR = {0};
struct io si_JAILB = {0};
struct io si_HUGI = {0};
struct io si_HUGB = {0};
struct io si_RTL = {0};
struct io si_RTLH = {0}; 
struct io si_RTLDC = {0};
struct io si_EMSH = {0};

struct io si_THDS = {0};

struct io si_FS = {0};
struct io si_BP = {0};
struct io si_FSX = {0};
struct io si_ALT = {0}; 
struct io si_MES = {0};
struct io si_MRS = {0};
struct io si_HWS = {0};
struct io si_HWS2 = {0};


struct io si_EQR = {0};
struct io si_ETH = {0};
struct io si_COL = {0};


struct io si_CWS = {0};
struct io si_ELOF = {0};
struct io si_CCP = {0};
struct io si_CCLP = {0};
struct io si_HCLP = {0};
struct io si_LHCP = {0};
struct io si_DMO = {0};
struct io so_ISPD = {0};


struct io so_BRKC = {0};
struct io so_EBK = {0};
struct io so_EBKC = {0};
struct io so_EBA1 = {0};
struct io so_EBA2 = {0};
struct io so_EBB1 = {0};
struct io so_EBB2 = {0};
struct io so_EBC = {0};
struct io so_UPML = {0};
struct io so_DNML = {0};
struct io so_NPL = {0};
struct io so_EMLH = {0};
struct io so_EPLH = {0};
struct io so_EPRL = {0};
struct io so_EPSL = {0};
struct io so_HUGO = {0};
struct io so_HUGA = {0};
struct io so_NPD = {0};
struct io so_EPD = {0};
struct io so_EPO = {0};
struct io so_TCU = {0};
struct io so_LU = {0};
struct io so_LD = {0};
struct io so_LUR = {0};
struct io so_LDR = {0};
struct io so_GEN = {0};
struct io so_DNO = {0};
struct io so_MTB = {0};
struct io so_GBL = {0};
struct io so_LBL = {0};
struct io so_GOVR = {0};
struct io so_GOVT = {0};


struct io so_RCF = {0};
struct io so_MST = {0};
struct io so_GR1R = {0};
struct io so_UTM = {0};
struct io so_TDOL = {0};
struct io so_TDCL = {0};
struct io so_TDOLR = {0};
struct io so_TDCLR = {0};
struct io so_TAVL = {0};
struct io so_TAVL2 = {0};
struct io so_TIND = {0};
struct io so_TMED = {0};
struct io so_TFSO = {0};
struct io so_TP1 = {0};
struct io so_TP2 = {0};
struct io so_TP3 = {0};
struct io so_TP4 = {0};
struct io so_TP5 = {0};
struct io so_TP6 = {0};
struct io so_EQL = {0};
struct io so_MBC = {0};
struct io so_MBP = {0};
struct io so_MB1 = {0};
struct io so_MB2 = {0};
struct io so_FSDO = {0};
struct io so_RSDO = {0};
struct io so_RVDR = {0};
struct io so_REVR = {0};
struct io so_RVD = {0};
struct io so_REV = {0};
struct io so_HVD = {0};
struct io so_HVDR = {0};



struct io so_FSO = {0};
struct io so_FiFL = {0};
struct io so_FLCT = {0};
struct io so_FBNBCT = {0};
struct io so_FBNB = {0};
struct io so_DCA = {0};
struct io so_HB = {0};
struct io so_SABUZ = {0};
struct io so_SABEE = {0};
struct io so_SABO = {0};
struct io so_ISER = {0};
struct io so_INSO = {0};
struct io so_OSERB = {0};
struct io so_OSERL = {0};
struct io so_OPENL = {0};
struct io so_INSEC = {0};
struct io so_FSOCS = {0};
struct io so_FSORS = {0};
struct io so_FW = {0};
struct io so_DDA = {0};
struct io so_UDA = {0};
struct io so_DPI1 = {0};
struct io so_DPI2 = {0};
struct io so_DPI3 = {0};
struct io so_DPI4 = {0};
struct io so_DPI5 = {0};
struct io so_DPI6 = {0};
struct io so_OLL = {0};
struct io so_HDB = {0};
struct io so_DESL = {0};
struct io so_INSP = {0};
struct io so_ADO = {0};
struct io so_RADO = {0};
struct io so_PH1R = {0};
struct io so_PH2HD = {0};
struct io so_PH2ON = {0};
struct io so_PH2OF = {0};
struct io so_DAO = {0};
struct io so_RC = {0};
struct io so_SQUEZ = {0};
struct io so_RDT = {0};
struct io so_MPTR = {0};
struct io so_CODG = {0}; 	// Door/Gate Cut-out output for courion door interface
struct io so_COHS = {0};
struct io so_ATO = {0};
struct io so_IAO = {0};
struct io so_FPH1 = {0};
struct io so_RAHS = {0};
struct io so_FPH2 = {0};
struct io so_DSC = {0};
struct io so_FDOB = {0};
struct io so_RDOB = {0};
struct io so_TC = {0};
struct io so_FRCL = {0};
struct io so_HCO = {0};
struct io so_FH = {0};
struct io so_FPS2 = {0};

struct io so_EFST = {0};
struct io so_EMS1Z = {0};
struct io so_EMS2Z = {0};
struct io so_EMS3Z = {0};
struct io so_EMS4Z = {0};
struct io so_EMS5Z = {0};
struct io so_EMS6Z = {0};
struct io so_EMS7Z = {0};
struct io so_EMS8Z = {0};
struct io so_EMS9Z = {0};
struct io so_EMS10Z = {0};
struct io so_EMS11Z = {0};
struct io so_FRT1Z = {0};
struct io so_FRT2Z = {0};
struct io so_FRT3Z = {0};
struct io so_PDO = {0};
struct io so_PDOR = {0};
struct io so_G_INSP = {0};
struct io so_G_ADO = {0};
struct io so_G_FDCBO = {0};
struct io so_G_RDCBO = {0};
struct io so_G_ADOH = {0};
struct io so_G_RDO = {0};
struct io so_G_RDOH = {0};
struct io so_G_RC = {0};
struct io so_G_SOPEN = {0};
struct io so_G_ININD = {0};
struct io so_G_FH1R = {0};
struct io so_G_PH2HD = {0};
struct io so_G_PH2ON = {0};
struct io so_G_PH2OF = {0};

struct io so_RUDA = {0};
struct io so_RDDA = {0};
struct io so_RFSO = {0};
struct io so_RPI1 = {0};
struct io so_RPI2 = {0};
struct io so_RPI3 = {0};
struct io so_RPI4 = {0};
struct io so_RPI5 = {0};
struct io so_RPI6 = {0};
struct io so_RPI7 = {0};
struct io so_RPI8 = {0};
struct io so_RPI9 = {0};
struct io so_RPI10 = {0};
struct io so_RPI11 = {0};
struct io so_RPI12 = {0};
struct io so_RPI13 = {0};
struct io so_RPI14 = {0};
struct io so_RPI15 = {0};
struct io so_RPI16 = {0};
struct io so_RPI17 = {0};
struct io so_RPI18 = {0};
struct io so_RPI19 = {0};
struct io so_RPI20 = {0};
struct io so_RPI21 = {0};

struct io so_SERV = {0};
struct io so_SERVC = {0};

struct io si_SECUR = {0};
struct io si_RSECU = {0};
struct io si_OSEC = {0};
struct io si_ORSEC = {0};
struct io si_ALTPI = {0};

struct io so_MESO = {0};	// Main Eggress Smoke Output Board Address	
struct io so_ALTO = {0};	// Fire Service Alternate Floor Output Board Address	

struct io si_SMK[fl_size] = {0};		// multiple smoke inputs

struct io si_SCD = {0};			// Security Disabling Input Board Address
struct io si_RSPD = {0};		// Reduced Speed Input Data Array Number

struct io si_RDZE = {0};		// Rear Door Zone Enable Input Board Address
struct io so_PDCA = {0};		// Peelle Door Close Alarm Output Board Address
struct io so_PFDCA = {0};		// Peelle Force Door Close Alarm Output Board Address
struct io so_FETST = {0};		// Freight Front Electric Eye Test Output Board Address
struct io so_RETST = {0};		// Freight Rear Electric Eye Test Output Board Address

struct io si_FETST = {0};		// Freight Front Electric Eye Test Input Board Address
struct io si_RETST = {0};		// Freight Rear Electric Eye Test Input Board Address
struct io si_EE2 = {0};			// Electric Eye Input Board Address
struct io si_EER2 = {0};		// Electric Eye Input Board Address
struct io si_HDOB = {0};		// Hall Door Open Button Input Board Address
struct io si_HDCB = {0};		// Hall Door Close Button Input Board Address
struct io si_HDOBR = {0};		// Hall Door Open Button Rear Input Board Address
struct io si_HDCBR = {0};		// Hall Door Close Button Input Rear Board Address

struct io so_OSERD = {0};		// Out of service DOB stuck door open Output Board Address
struct io so_ADOR[fl_size] = {0};	// Auto Swing Door Pulse Ouput Board Address 	ado #1 
struct io so_ADOF[fl_size] = {0};	// Rear Auto Swing Door Pulse Ouput Board Address 	ado #1 

struct io si_MSPI1 = {0};
struct io si_MSPI2 = {0};
struct io si_MSPI3 = {0};
struct io so_MSPO1 = {0};
struct io so_MSPO2 = {0};
struct io so_MSPO3 = {0};



//ZYM 10/18/2017
//input
struct io si_MLi0 = {0};
struct io si_MLi1 = {0};
struct io si_MLi2 = {0};
struct io si_MLi3 = {0};
struct io si_MLi4 = {0};
struct io si_MLi5 = {0};
struct io si_MLi6 = {0};
struct io si_MLi7 = {0};
struct io si_MLi8 = {0};
struct io si_MLi9 = {0};
struct io si_MLi10 = {0};
struct io si_MLi11 = {0};

//output
struct io so_MLo0 = {0};
struct io so_MLo1 = {0};
struct io so_MLo2 = {0};
struct io so_MLo3 = {0};
struct io so_MLo4 = {0};
struct io so_MLo5 = {0};
struct io so_MLo6 = {0};
struct io so_MLo7 = {0};
struct io so_MLo8 = {0};
struct io so_MLo9 = {0};
struct io so_MLo10 = {0};
struct io so_MLo11 = {0};

//input
struct io si_MRi0 = {0};
struct io si_MRi1 = {0};
struct io si_MRi2 = {0};
struct io si_MRi3 = {0};
struct io si_MRi4 = {0};
struct io si_MRi5 = {0};
struct io si_MRi6 = {0};
struct io si_MRi7 = {0};
struct io si_MRi8 = {0};
struct io si_MRi9 = {0};
struct io si_MRi10 = {0};
struct io si_MRi11 = {0};

//output
struct io so_MRo0 = {0};
struct io so_MRo1 = {0};
struct io so_MRo2 = {0};
struct io so_MRo3 = {0};
struct io so_MRo4 = {0};
struct io so_MRo5 = {0};
struct io so_MRo6 = {0};
struct io so_MRo7 = {0};
struct io so_MRo8 = {0};
struct io so_MRo9 = {0};
struct io so_MRo10 = {0};
struct io so_MRo11 = {0};



//ZYM 10/20/2017
//input
struct io si_suf = {0};
struct io si_sufc = {0};
struct io si_su = {0};
struct io si_suc = {0};
struct io si_sdf = {0};
struct io si_sdfc = {0};
struct io si_sd = {0};
struct io si_sdc = {0};
struct io si_s6s10 = {0};

//output
struct io so_S5LPS = {0};
struct io so_S4LOS = {0};
struct io so_S3TPL = {0};



//input
struct io si_s2mrw = {0};
struct io si_24s10 = {0};
struct io si_22gov = {0};
struct io si_20tlh = {0};
struct io si_h18ps = {0};
struct io si_16mrw = {0};

//output
struct io so_S1TPH = {0};
struct io so_23GOV = {0};
struct io so_21TLH = {0};
struct io so_H19PS = {0};
struct io so_17HSS = {0};



//input
struct io si_14mrw = {0};
struct io si_11mrw = {0};
struct io si_h8s10 = {0};
struct io si_h6s10 = {0};
struct io si_h4s10 = {0};

//output
struct io so_15TSD = {0};
struct io so_13BAU = {0};
struct io so_12BAD = {0};
struct io so_10TAU = {0};
struct io so_H9TAD = {0};
struct io so_H7DLB = {0};
struct io so_H5DLM = {0};



//input
struct io si_h2s10 = {0};
struct io si_mrsw = {0};
struct io si_mc = {0};
struct io si_mc1 = {0};
struct io si_ste = {0};
struct io si_s10 = {0};

//output
struct io so_H3DLT = {0};
struct io so_RLM = {0};
//-struct io so_MC1 = {0};
struct io so_0MTO = {0};
struct io so_0SPD = {0};
struct io so_0CFLT = {0};
struct io so_TMRLY = {0};



//input
struct io si_18hss = {0};

//output
struct io so_17CST = {0};
struct io so_16INS = {0};
struct io so_T15IU = {0};
struct io so_T14ID = {0};
struct io so_13IEN = {0};
struct io so_12ICA = {0};
struct io so_11ICI = {0};
struct io so_10ACC = {0};
struct io so_T9GS = {0};
struct io so_RGS = {0};
struct io so_RDRLY = {0};


//*********************************************************************************************
// Car Top Can I/O
//*********************************************************************************************

struct io si_RLS = {0};
struct io si_CSS = {0};
struct io si_CTS = {0};
struct io si_CST = {0};
struct io si_DET = {0}; 
struct io si_RDET = {0}; 
struct io si_HWSET = {0}; 	// RUN/SETUP JUMPER


struct io so_IFL = {0};	// Inspection Fire Light
struct io so_IFB = {0};	// Inspection Fire Buzzer

struct io so_CUL = {0};	// Car Up lantern
struct io so_CDL = {0};	// Car Down lantern
struct io so_CLF = {0};	// Cab Light and Fan
struct io so_FAN = {0};	// cab Fan
struct io so_LIGHT = {0};	// Cab Light
struct io so_SELCM = {0};	// Selector Comm LED
struct io so_NTSCM = {0};	// NTS Comm LED



struct io si_DT = {0}; 	// Down Terminal Slowdosn
struct io si_DT1 = {0}; 	// 1st Down Terminal limit
struct io si_DT2 = {0}; 	// 2nd Down Terminal limit
struct io si_DT3 = {0}; 	// 3rd Down Terminal limit
struct io si_DT4 = {0}; 	// 4th Down Terminal limit
struct io si_DT5 = {0}; 	// 5th Down Terminal limit
struct io si_DT6 = {0}; 	// 6th Down Terminal limit

struct io si_UT = {0}; 	// Up Terminal Slowdown
struct io si_UT1 = {0};	// 1st Up Terminal limit
struct io si_UT2 = {0};	// 2nd Up Terminal limit
struct io si_UT3 = {0}; 	// 3rd Up Terminal limit
struct io si_UT4 = {0}; 	// 4th Up Terminal Limit
struct io si_UT5 = {0}; 	// 5th Up Terminal limit
struct io si_UT6 = {0}; 	// 6th Up Terminal Limit

struct io si_DL = {0};  	// Down Level Input
struct io si_DZA = {0}; 	// Door Zone A input (DZU OR DZD)
struct io si_UL = {0}; 	// Up level input
struct io si_DZsel = {0};	// Door Zone Selector Count 

struct io si_UL1 = {0}; 	// Up level input
struct io si_UL2 = {0}; 	// Up level input
struct io si_UL3 = {0}; 	// Up level input
struct io si_UL4 = {0}; 	// Up level input

struct io si_DL1 = {0};  	// Down Level Input
struct io si_DL2 = {0};  	// Down Level Input
struct io si_DL3 = {0};  	// Down Level Input
struct io si_DL4 = {0};  	// Down Level Input


struct io si_DZU = {0};		// Door Zone Sensor input
struct io si_DZD = {0};		// Door Zone Sensor input
struct io si_DZ1A = {0};	// Door Zone Sensor input
struct io si_DZ2A = {0};	// Door Zone Sensor input
struct io si_DZ1B = {0};	// Door Zone Sensor input
struct io si_DZ2B = {0};	// Door Zone Sensor input
struct io si_UNL = {0};		// Up Normal Latch
struct io si_UNsel = {0};	// Up Normal Selector Count
struct io si_UTsel = {0};	// Up Terminal Slowdown Selector Count 
struct io si_UTSsn = {0};	// Up Emergency Terminal Slowdown Sensor 

struct io si_DNL = {0};		// Down Normal Latch
struct io si_DNsel = {0};	// Down Normal Selector Count
struct io si_DTsel = {0};	// Down Terminal Slowdown Selector Count
struct io si_DTSsn = {0};	// Down Emergency Terminal Slowdown Sensor
struct io si_ETSsn = {0};	// Down Emergency Terminal Slowdown Sensor

struct io si_SE = {0};
struct io si_SER = {0};
struct io si_EE = {0};
struct io si_EER = {0};
struct io si_HVDR = {0};
struct io si_DCL = {0};
struct io si_DOL = {0};
struct io si_DCLR = {0};
struct io si_DOLR = {0};
struct io si_ALRM = {0};
struct io si_ALRMR = {0};
struct io si_EMS = {0};
#define i_HVDR &si_HVDR


struct io si_movfr_DCL = {0};
struct io si_movfr_DOL = {0};
struct io si_movfr_EE = {0};
struct io si_movfr_DPM = {0};
struct io si_movfr_DCLR = {0};
struct io si_movfr_DOLR = {0};
struct io si_movfr_EER = {0};
struct io si_movfr_RPM = {0};


struct io si_std_DCL = {0};
struct io si_std_DOL = {0};
struct io si_std_EE = {0};
struct io si_std_DPM = {0};
struct io si_std_DCLR = {0};
struct io si_std_DOLR = {0};
struct io si_std_EER = {0};
struct io si_std_RPM = {0};


struct io si_DPM = {0};
struct io si_RPM = {0};

struct io si_DOB = {0};
struct io si_DCB = {0};
struct io si_DOBR = {0};
struct io si_DCBR = {0};

struct io si_FS2 = {0};
struct io si_FS2H = {0};
struct io si_FS2C = {0};

struct io si_FiDOB = {0};
struct io si_FiDOBR = {0};
struct io si_FiFS2C = {0};
struct io si_FiDCB = {0};
struct io si_FiDCBR = {0};

struct io si_ICID = {0};
struct io si_ICIU = {0};
struct io si_ED = {0};
struct io si_EDHL = {0};
struct io si_EDR = {0};
struct io si_OVL = {0};
struct io si_LWA = {0};
struct io si_LWB = {0};
struct io si_LWD = {0};

struct io si_ATT = {0};
struct io si_ATTUP = {0};
struct io si_ATTDN = {0};
struct io si_ATTBY = {0};
struct io si_ATTHC = {0};


struct io si_DZSA = {0};
struct io si_UNS = {0};
struct io si_DNS = {0};
struct io si_DLW = {0};
struct io si_HBE = {0};
struct io si_HBER = {0};

struct io si_INDC = {0};
struct io si_INDS = {0};
struct io si_IDSO = {0};
struct io si_ICVIP = {0};
struct io si_SCS = {0};
struct io si_SCSR = {0};
struct io si_ZON = {0};


struct io si_DS = {0};
struct io si_US = {0};

							      																		
struct io so_DO = {0};
struct io so_DC = {0};
struct io so_NUD = {0}; // so_HVD
//struct io so_HVD = {0};
struct io so_DOR = {0};
struct io so_DCR = {0};
struct io so_NUDR = {0};
struct io so_DZDE = {0};
struct io so_DTR = {0};
struct io so_SFST = {0};
struct io so_HEOFL = {0};
struct io so_CULR = {0};
struct io so_CDLR = {0};
struct io so_RCM = {0};
struct io so_FL = {0};
struct io so_FS1L = {0};
struct io so_FS2L = {0};
struct io so_F1ML = {0};
struct io so_F1AL = {0};
struct io so_EML = {0};
struct io so_ATDL = {0};
struct io so_ATUL = {0};
struct io so_EQLC = {0};
struct io so_EDL = {0};

struct io so_std_DO = {0};
struct io so_std_DC = {0};
struct io so_std_NUD = {0};
struct io so_movfr_DO = {0};
struct io so_movfr_DC = {0};
struct io so_movfr_NUD = {0};

struct io so_std_DOR = {0};
struct io so_std_DCR = {0};
struct io so_std_NUDR = {0};
struct io so_movfr_DOR = {0};
struct io so_movfr_DCR = {0};
struct io so_movfr_NUDR = {0};

		
struct io so_DDAC = {0};
struct io so_UDAC = {0};
struct io so_DPI1C = {0};
struct io so_DPI2C = {0};
struct io so_DPI3C = {0};
struct io so_DPI4C = {0};
struct io so_DPI5C = {0};
struct io so_DPI6C = {0};

struct io so_PIO[fl_size] = {0};
struct io so_PICO[fl_size] = {0};

struct io so_ULO[fl_size] = {0};
struct io so_DLO[fl_size] = {0};
struct io so_ULRO[fl_size] = {0};
struct io so_DLRO[fl_size] = {0};

struct io so_UALO[fl_size] = {0};
struct io so_DALO[fl_size] = {0};
struct io so_UALRO[fl_size] = {0};
struct io so_DALRO[fl_size] = {0};


struct io si_CCI[fl_size] = {0};
struct io so_CCO[fl_size] = {0};
struct io si_CCRI[fl_size] = {0};
struct io so_CCRO[fl_size] = {0};
struct io si_CCSI[fl_size] = {0};
struct io si_OCSI[fl_size] = {0};
struct io si_RCCSI[fl_size] = {0};
struct io si_ORSI[fl_size] = {0};
struct io si_RM_CCI[fl_size] = {0};
struct io so_RM_CCO[fl_size] = {0};
struct io si_RM_CCRI[fl_size] = {0};
struct io so_RM_CCRO[fl_size] = {0};
struct io so_SCCO[fl_size] = {0};
struct io so_SCCRO[fl_size] = {0};

struct io si_DSI = {0};			// Door Stop Input Board Address
struct io si_RDSI = {0};		// Rear Door Stop Input Board Address
struct io si_CEOF = {0};		// Car Elevator Off Input Board Address
struct io si_CEOFR = {0};		// Car Elevator Off Reset Board Address
struct io so_CEOFL = {0};		// Car Elevator Off Light Board Address
struct io si_ATHCD = {0};		// Attendant Hall Call Respond Disable Input Board Address


struct io si_CSPI1 = {0};
struct io si_CSPI2 = {0};
struct io si_CSPI3 = {0};
struct io so_CSPO1 = {0};
struct io so_CSPO2 = {0};
struct io so_CSPO3 = {0};


//ZYM 10/16/2017
//input
struct io si_CTi0 = {0};
struct io si_CTi1 = {0};
struct io si_CTi2 = {0};
struct io si_CTi3 = {0};
struct io si_CTi4 = {0};
struct io si_CTi5 = {0};
struct io si_CTi6 = {0};
struct io si_CTi7 = {0};
struct io si_CTi8 = {0};
struct io si_CTi9 = {0};
struct io si_CTi10 = {0};
struct io si_CTi11 = {0};

//output
struct io so_CTo0 = {0};
struct io so_CTo1 = {0};
struct io so_CTo2 = {0};
struct io so_CTo3 = {0};
struct io so_CTo4 = {0};
struct io so_CTo5 = {0};
struct io so_CTo6 = {0};
struct io so_CTo7 = {0};
struct io so_CTo8 = {0};
struct io so_CTo9 = {0};
struct io so_CTo10 = {0};
struct io so_CTo11 = {0};


// input
struct io si_RLS_COPi0 = {0};
struct io si_CSS_COPi1 = {0};
struct io si_CTS_COPi2 = {0};
struct io si_DET_COPi3 = {0};

// OUTPUT
struct io sO_IFL_COPo0 = {0};
struct io sO_IFB_COPo1 = {0};
struct io sO_CUL_COPo2 = {0};
struct io sO_CDL_COPo3 = {0};
struct io sO_FAN_COPo4 = {0};
struct io sO_LIG_COPo5 = {0};

//wbg 2018-5-05
//input
struct io si_SWi0 = {0};
struct io si_SWi1 = {0};
struct io si_SWi2 = {0};
struct io si_SWi3 = {0};
struct io si_SWi4 = {0};
struct io si_SWi5 = {0};
struct io si_SWi6 = {0};
struct io si_SWi7 = {0};
struct io si_SWi8 = {0};
struct io si_SWi9 = {0};
struct io si_SWi10 = {0};
struct io si_SWi11 = {0};

//output
struct io SO_LDO0 = {0};
struct io SO_LDO1 = {0};
struct io SO_LDO2 = {0};
struct io SO_LDO3 = {0};
struct io SO_LDO4 = {0};
struct io SO_LDO5 = {0};
struct io SO_LDO6 = {0};
struct io SO_LDO7 = {0};
struct io SO_LDO8 = {0};
struct io SO_LDO9 = {0};
struct io SO_LDO10 = {0};
struct io SO_LDO11 = {0};


// (4) wbg 2018-9-02 OPL-0106N
//input
struct io si_ronc  = {0};
struct io si_rono  = {0};
struct io si_dclnc = {0};
struct io si_dclno = {0};
struct io si_dolnc = {0};
struct io si_dolno = {0};
struct io si_heavy = {0};
struct io si_nudge = {0};
struct io si_close = {0};
struct io si_open  = {0};

//output
struct io SO_CN19V = {0};
struct io SO_CN18V = {0};
struct io SO_CLOSE = {0};
struct io SO_OPEN  = {0};
struct io SO_NUDGE = {0};
struct io SO_HEAVY = {0};
struct io SO_DCL   = {0};
struct io SO_RO    = {0};
struct io SO_DOL   = {0};

// wbg 2018-5-17

// input
struct io si_CTA_ctcan = {0};
struct io si_INS_ctcan = {0};
struct io si_IEN_ctcan = {0};
struct io si_IU_ctcan = {0};
struct io si_ID_ctcan = {0};
struct io si_FANST_ctcan = {0};
struct io si_LIGST_ctcan = {0};
struct io si_CDL_T_ctcan = {0};
struct io si_CUL_T_ctcan = {0};
struct io si_CTS_T_ctcan = {0};
struct io si_FFSWT_ctcan = {0};

// output
struct io SO_CSS_ctcan   = {0};

// wbg 2018-5-18

// input
struct io si_CST_T_ctcan = {0};
struct io si_ICC_T_ctcan = {0};
struct io si_ICA_T_ctcan = {0};
struct io si_ICI_T_ctcan = {0};
struct io si_ACC_T_ctcan = {0};

// output
struct io SO_C120T_ctcan = {0};
struct io SO_L15RL_ctcan = {0};
struct io SO_L15_T_ctcan = {0};
struct io SO_FSS_ctcan   = {0};
struct io SO_CST_ctcan   = {0};
struct io SO_ICC_ctcan   = {0};

// wbg 2018-8-06
// input
struct io si_EE_ctcan    = {0};
struct io si_DOL_ctcan   = {0};
struct io si_DCL_ctcan   = {0};
struct io si_SE_ctcan    = {0};
struct io si_DC_ctcan    = {0};

// output
struct io SO_DO_ctcan    = {0};
struct io SO_L15_ctcan   = {0};
struct io SO_L25_ctcan   = {0};



// wbg 2018-5-18

// input
struct io si_L15_ctcan = {0};
struct io si_L25_ctcan = {0};

// output
struct io SO_ICA_ctcan   = {0};
struct io SO_ICI_ctcan   = {0};
struct io SO_ACC_ctcan   = {0};
struct io SO_BELL_ctcan  = {0};
struct io SO_CLC_ctcan   = {0};

// wbg 2018-5-18

// input
struct io si_C24_T_ctcan = {0};

// output
struct io SO_C24_ctcan   = {0};

// wbg 2018-6-27
// output
struct io SO_dbg01   = {0};
struct io SO_dbg02   = {0};
struct io SO_dbg03   = {0};
struct io SO_dbg04   = {0};
struct io SO_dbg05   = {0};
struct io SO_dbg06   = {0};
struct io SO_dbg07   = {0};
struct io SO_dbg08   = {0};
struct io SO_dbg09   = {0};
struct io SO_dbg10   = {0};


//*******************************
//  Aux Car Station
//*******************************

struct io si_XCCI[fl_size] = {0};
struct io so_XCCO[fl_size] = {0};
struct io si_XCCRI[fl_size] = {0};
struct io so_XCCRO[fl_size] = {0};

struct io si_XDOB = {0};
struct io si_XDCB = {0};
struct io si_XDOBR = {0};
struct io si_XDCBR = {0};
struct io si_XALRM = {0};
struct io si_XHBE = {0};
struct io si_XED = {0};
struct io si_XINDC = {0};
struct io si_XALRMR = {0};
struct io si_XHBER = {0};
struct io si_XEDR = {0};



//*********************************************************
// Group IOs
//*********************************************************

struct io si_UC[fl_size] = {0};
struct io so_UCL[fl_size] = {0};
struct io si_DC[fl_size] = {0};
struct io so_DCL[fl_size] = {0};

struct io si_RUC[fl_size] = {0};
struct io so_RUCL[fl_size] = {0};
struct io si_RDC[fl_size] = {0};
struct io so_RDCL[fl_size] = {0};

struct io si_IRUC[fl_size] = {0};
struct io so_IRUCL[fl_size] = {0};
struct io si_IRDC[fl_size] = {0};
struct io so_IRDCL[fl_size] = {0};

struct io si_RIRUC[fl_size] = {0};
struct io so_RIRUCL[fl_size] = {0};
struct io si_RIRDC[fl_size] = {0};
struct io so_RIRDCL[fl_size] = {0};

struct io si_TRUC[fl_size] = {0};
struct io so_TRUCL[fl_size] = {0};
struct io si_TRDC[fl_size] = {0};
struct io so_TRDCL[fl_size] = {0};

struct io si_XUC[fl_size] = {0};
struct io so_XUCL[fl_size] = {0};
struct io si_XDC[fl_size] = {0};
struct io so_XDCL[fl_size] = {0};

struct io si_XRUC[fl_size] = {0};
struct io so_XRUCL[fl_size] = {0};
struct io si_XRDC[fl_size] = {0};
struct io so_XRDCL[fl_size] = {0};


struct io si_CB[fl_size] = {0};
struct io so_CBL[fl_size] = {0};
struct io si_RCB[fl_size] = {0};
struct io so_RCBL[fl_size] = {0};

struct io si_UB[fl_size] = {0};
struct io so_UBL[fl_size] = {0};
struct io si_DB[fl_size] = {0};
struct io so_DBL[fl_size] = {0};

struct io si_URB[fl_size] = {0};
struct io so_URBL[fl_size] = {0};
struct io si_DRB[fl_size] = {0};
struct io so_DRBL[fl_size] = {0};

struct io si_CB_SR[fl_size] = {0};
struct io so_CB_SRL[fl_size] = {0};
struct io si_RCB_SR[fl_size] = {0};
struct io so_RCB_SRL[fl_size] = {0};

struct io si_VIP[fl_size] = {0};
struct io so_VIPL[fl_size] = {0};

struct io si_RVIP[fl_size] = {0};
struct io so_RVIPL[fl_size] = {0};

// Car Call lockout from the group
struct io si_GCSEC = {0};
struct io si_RGCSC = {0}; 
struct io si_GCS[fl_size] = {0};
struct io si_RGCS[fl_size] = {0};

// Car Call Security Override from the group
struct io si_GCSO = {0}; 
struct io si_RGCSO = {0}; 
struct io si_GOS[fl_size] = {0};
struct io so_GOSO[fl_size] = {0};
struct io si_RGOS[fl_size] = {0};
struct io so_RGOSO[fl_size] = {0};

// Floor (hall and car call) lockout from the group
struct io si_FLSEC = {0}; 
struct io si_RFLSC = {0}; 
struct io si_FLS[fl_size] = {0};
struct io si_RFLS[fl_size] = {0};

// Hall Call lockout from the group
struct io si_HSEC = {0}; 
struct io si_RHSEC = {0}; 
struct io si_HCS[fl_size] = {0};
struct io si_RHCS[fl_size] = {0};

// Serial Hall Call lockout from the group
struct io si_SHSEC = {0}; 
struct io si_RSHSEC = {0}; 
struct io si_SHS[fl_size] = {0};
struct io si_RSHS[fl_size] = {0};

// Serial Hall Call lockout Light from the group
struct io so_SHSL[fl_size] = {0};
struct io so_RSHSL[fl_size] = {0};

//  Hugs security inputs
struct io si_HUGSEC = {0};
struct io si_RHUGSEC = {0}; 
struct io si_HGS[fl_size] = {0};
struct io si_RHGS[fl_size] = {0};

// Sabbath Floor Enable from Group
struct io si_SABF = {0}; 
struct io si_RSABF = {0}; 
struct io si_SBF[fl_size] = {0};
struct io si_SBR[fl_size] = {0};



// Group I/O
struct io si_HC = {0};
struct io si_ICR = {0}; 
struct io si_ICR2 = {0}; 
struct io si_RICR = {0}; 

struct io si_VIPEN = {0}; 
struct io si_RVIPEN = {0}; 

struct io si_LUC = {0};
struct io so_LUCL = {0};

struct io si_CASEC = {0}; 
struct io si_APRK = {0}; 
struct io si_NCU = {0}; 
struct io si_ALBY = {0}; 

struct io si_AUTEP = {0};
struct io si_RECEN = {0};
struct io si_RUNEN = {0};
struct io so_RUNCT = {0};
struct io so_EPRCLF = {0};

struct io si_CBSR = {0}; 
struct io si_RCBSR = {0}; 
struct io si_TR = {0}; 

struct io si_SECF1 = {0};
struct io si_SECF2 = {0};
struct io si_SECF3 = {0};
struct io si_TMREN = {0};
struct io so_GSERV = {0}; 

struct io so_PRK[car_size] = {0};
struct io so_OP[car_size] = {0};
struct io so_SEL[car_size] = {0};

struct io si_NCA = {0};		// NO cars available from other group (cross assignment)

struct io si_CYCLE = {0};		// Second Riser Cycling Enable Input Board Address
struct io so_CYCLO = {0};		// Second Riser Cycling Output Board Address
struct io si_VPECC = {0};    // Group VIP Car Calls Enable Input Board Address

struct io si_GSPI1 = {0};
struct io si_GSPI2 = {0};
struct io si_GSPI3 = {0};
struct io so_GSPO1 = {0};
struct io so_GSPO2 = {0};
struct io so_GSPO3 = {0};

//ZYM 10/9/2017
/*
struct io si_TESi0 = {0};
struct io si_TESi1 = {0};
struct io si_TESi2 = {0};
struct io si_TESi3 = {0};
struct io si_TESi4 = {0};
struct io si_TESi5 = {0};
struct io si_TESi6 = {0};
struct io si_TESi7 = {0};
struct io si_TESi8 = {0};
struct io si_TESi9 = {0};
struct io si_TEi10 = {0};
struct io si_TEi11 = {0};

struct io so_TESo0 = {0};
struct io so_TESo1 = {0};
struct io so_TESo2 = {0};
struct io so_TESo3 = {0};
struct io so_TESo4 = {0};
struct io so_TESo5 = {0};
struct io so_TESo6 = {0};
struct io so_TESo7 = {0};
struct io so_TESo8 = {0};
struct io so_TESo9 = {0};
struct io so_TEo10 = {0};
struct io so_TEo11 = {0};

struct io GSPO1 = {0};
*/

//ZYM 10/9/2017
//input
struct io si_GRi0 = {0};
struct io si_GRi1 = {0};
struct io si_GRi2 = {0};
struct io si_GRi3 = {0};
struct io si_GRi4 = {0};
struct io si_GRi5 = {0};
struct io si_GRi6 = {0};
struct io si_GRi7 = {0};
struct io si_GRi8 = {0};
struct io si_GRi9 = {0};
struct io si_GRi10 = {0};
struct io si_GRi111 = {0};

//output
struct io so_GRo0 = {0};
struct io so_GRo1 = {0};
struct io so_GRo2 = {0};
struct io so_GRo3 = {0};
struct io so_GRo4 = {0};
struct io so_GRo5 = {0};
struct io so_GRo6 = {0};
struct io so_GRo7 = {0};
struct io so_GRo8 = {0};
struct io so_GRo9 = {0};
struct io so_GRo10 = {0};
struct io so_GRo11 = {0};



//*********************************************************
// Get io offset from bit location
//*********************************************************

int16 count_bit(unsigned char bitnmb)
{
	int16 i;
	int16 j;

	j = 0;
	for (i=0;i<=7;i++)
	{
		if ((bitnmb & bittbl[i]) != 0)
			return j;
		j++;
	}
	return j;
}


//********************************
// Initialize the i/o location
//********************************
// type = 0x01 for output, 0x10 for send name

int8  init_io (struct io *iop, uint8 bus, char *name, uint8 type, uint8 on_color)
{
	int16 io_start = 0;
	int16 io_end = 0;
	uint8 found = 0;
	int16 i;
	
	if (bus == MAINIO)
	{
		io_start = 0;
		io_end = 18 * 8;
	}
	else if (bus == MRCAN)
	{
		io_start = 18 * 8;
		io_end = 138 * 8;
	}
	else if (bus == CTCAN)
	{
		io_start = 138 * 8; 
		io_end = 258 * 8;
	}
	
	for(i=io_start;i<io_end;i++)
	{
		if ((ioname[i][0] == name[0]) &&
			(ioname[i][1] == name[1]) &&
			(ioname[i][2] == name[2]) &&
			(ioname[i][3] == name[3]) &&
			(ioname[i][4] == name[4]))
		{
			found = 1;
			break;
		}
	}
	
	if (found == 1)
	{
		iop->bus = bus;
		iop->array = (uint16)(i/8);		
		iop->bit = (uint8)bittbl[i%8];
		if ((bus == MAINIO) && (iop->array < 18))
			iop->addr = (uint16)(iop->array + 0x100);		
		else if ((bus == MRCAN) && (iop->array >= 18))
			iop->addr = (uint16)(iop->array - 18);		
		else if ((bus == CTCAN) && (iop->array >= 138))
			iop->addr = (uint16)(iop->array - 138);	
		else 
			iop->addr = 0x110;
		
		io_valid[iop->array] |= iop->bit; 
		io_on_color_ix[i] = on_color; 
		if ((type & 0x10) != 0)
			io_send_name[iop->array] |= iop->bit;
		else
			io_send_name[iop->array] &= ~iop->bit; 
		if ((type & 0x01) != 0)
			IO_In_Out[iop->array] |= iop->bit;
		else
			IO_In_Out[iop->array] &= ~iop->bit;
	
		if(init_car == cons[carnmb])
		{
			if (on_color != 0)
				IO_board[init_car][(iop->array/3)] = 3;		
			else
				IO_board[init_car][(iop->array/3)] = 1;		
		}
	}
	return found;
}

//***********************************
// Initialize the group i/o location
//***********************************
// type = 0x01 for output, 0x10 for send name, 0x80 for I/O not on serial hall board
// type = 0x04 = ir, 0x08 = cb, 0x20 = vip
// hc_io_board[] = 0x01 = hc board, 0x02 = Serial hc driver board, 0x04 = ir, 0x08 = cb, 0x20 = vip, 0x40 = Serial HC Board
int8 init_gio (struct io *iop, uint8 bus, char *name, uint8 type)
{
	int16 i;
	uint8 found = 0;
	uint16 array = 0;
	uint8 bit = 0;
	
	if (bus == GRCAN)
	{
		for(i=0;i<nmb_hc_ios;i++)
		{
			if ((hcioname[i][0] == name[0]) &&
				(hcioname[i][1] == name[1]) &&
				(hcioname[i][2] == name[2]) &&
				(hcioname[i][3] == name[3]) &&
				(hcioname[i][4] == name[4]))
			{
				found = 1;
				break;
			}
		}
		
		if (found == 1)
		{
			iop->bus = bus;
			iop->array = (uint16)(i/8);		
			iop->bit = (uint8)bittbl[i%8];
			iop->addr = iop->array;
			gio_valid[iop->array] |=  iop->bit; 
			
			if ((type & 0x10) != 0)
				gio_send_name[iop->array] |= iop->bit;
			else
				gio_send_name[iop->array] &= ~iop->bit; 
			if ((type & 0x01) != 0)
				GIO_In_Out[iop->array] |= iop->bit;
			else
				GIO_In_Out[iop->array] &= ~iop->bit;
			
			if (((type & 0x80) == 0) && ((cons[sercom] & 0x01) != 0))
			{
				HC_IO_board[iop->array] = 0x41;
			}
			else if (((type & 0x80) == 0) && ((cons[sercom] & 0x20) != 0))
			{
				HC_IO_board[(iop->array/3)] = 3;
				if ((type & 0x04) != 0)	
					HC_IO_board[(iop->array/3)] |= 0x04;		// ir call type
				if ((type & 0x08) != 0)	
					HC_IO_board[(iop->array/3)] |= 0x08;		// CB call type
				if ((type & 0x20) != 0)	
					HC_IO_board[(iop->array/3)] |= 0x20;		// VIP call type
			}
			else
				// else standard IO
				HC_IO_board[(iop->array/3)] = 1;
		}
	}
	return found;
}


void set_input_start(int16 i_array_s, int16 i_bit_s)
{
    i_array_ix = i_array_s;
    i_bit_ix = i_bit_s;
    i_count = 0;
}

void set_output_start(int16 o_array_s, int16 o_bit_s)
{
    o_array_ix = o_array_s;
    o_bit_ix = o_bit_s;
    o_count = 0;
}

void set_grp_input_location(char *s_ioname)
{
	int16 ix = (int16)((i_array_ix * 8) + i_bit_ix);
	strncpy(hcioname[ix],s_ioname,5);
}

void set_grp_output_location(char *s_ioname)
{
	int16 ix = (int16)((o_array_ix * 8) + o_bit_ix);
	strncpy(hcioname[ix],s_ioname,5);
}


//**************************************
// Increment to the next group input i/o
//**************************************

int16 next_grp_inp()
{
    int array_seq = (i_array_ix % 3);
    if (array_seq == 0)
    {
        // Increment the index
        if (i_bit_ix < 7)
            i_bit_ix++;	// go to the next bit location
        else
        {
            i_bit_ix = 0;
            i_array_ix++;
        }
    }
    else if (array_seq == 1)
    {
        // Increment the index
        if (i_bit_ix < 3)
            i_bit_ix++;	// go to the next bit location
        else
        {
            i_bit_ix = 0;
            i_array_ix += 2;
        }
    }
    else if (array_seq == 2)
    {       // this case should never happen
        i_array_ix++;
        i_bit_ix = 0;
    }
    i_count++;
    return i_count;
}


//***********************************
// Increment to the next group output i/o
//***********************************

int16 next_grp_outp()
{

    int array_seq = (o_array_ix % 3);
    if (array_seq == 0)
    {   // this case should never happen
        o_array_ix++;
        o_bit_ix = 4;
    }
    else if (array_seq == 1)
    {
        if (o_bit_ix < 7)
            o_bit_ix++;	// go to the next bit location
        else
        {	// go to the next port location
            o_bit_ix = 0;
            o_array_ix++;
        }
    }
    else if (array_seq == 2)
    {
        if (o_bit_ix < 7)
            o_bit_ix++;	// go to the next bit location
        else
        {	// go to the next port location
            o_bit_ix = 4;
            o_array_ix += 2;
        }
    }
    o_count++;
    return o_count;
}


//*******************************************
// Load I/O names from file
//*******************************************

int16 Load_IO_Names (void)
{
	FL_FILE *fp1;
	int16 ix = 0;
	int16 array = 0;
	int16 last_array = 0;
	int16 array_cnt = 0;
	int16 i;
	uint8 car_io_init = 0;
	uint8 group_io_init = 0;
	
  	disable_dog();

  	fp1 = fl_fopen("/io.dat","r");
	if (fp1 == NULL)
	   	fp1 = fl_fopen("/IO.DAT","r");
	if (fp1 != NULL)
	{
		fl_fgets(fiodatbuf,180,fp1);				
			
		if (strstr(fiodatbuf,"CAR I/O TABLE") != 0)	
		{		// I/O table
			ix = 0;
			array = 0;
			array_cnt = 0;
			while ((array < nmb_io_arrays) && (array_cnt < nmb_io_arrays))
			{
				fl_fgets(fiodatbuf,180,fp1);
				// read board number
				for (i=0;i<5;i++)
				{		// find first comma
					if ((fiodatbuf[i] >= '0') && (fiodatbuf[i] <= '9'))
						char_in[i] = fiodatbuf[i];
					else if ((fiodatbuf[i] == ',') || (fiodatbuf[i] == '.'))
					{
						char_in[i] = fiodatbuf[i];
						break;
					}
				}
				if ((char_in[i] == ',') || (char_in[i] == '.'))
				{
					
					char_in[i] = 0; // replace comma with zero to get array number only
					array = (int16)atoi(char_in);
					ix = (int16)(array * 8);
					i++;			// first character of name
					if ((array < nmb_io_arrays) && (ix < (nmb_ios-7)))
					{
						car_io_init = 1;
						strncpy(ioname[ix+0],&fiodatbuf[i+0],5);
						strncpy(ioname[ix+1],&fiodatbuf[i+6],5);
						strncpy(ioname[ix+2],&fiodatbuf[i+12],5);
						strncpy(ioname[ix+3],&fiodatbuf[i+18],5);
						strncpy(ioname[ix+4],&fiodatbuf[i+24],5);
						strncpy(ioname[ix+5],&fiodatbuf[i+30],5);
						strncpy(ioname[ix+6],&fiodatbuf[i+36],5);
						strncpy(ioname[ix+7],&fiodatbuf[i+42],5);
						if (array < 138)
							mr_last_array = array;
						else
							ct_last_array = array; 
					}
					else if (array >= 300)
						break;
					else
						car_io_init = 0;
				}
				else
					car_io_init = 0;
				array_cnt++;
			}
		}
		fl_fgets(fiodatbuf,180,fp1);
		if (strstr(fiodatbuf,"GROUP I/O TABLE") != 0)	
		{		// I/O table
		
			ix = 0;
			array = 0;
			array_cnt = 0;
			while ((array < nmb_hc_io_arrays) && (array_cnt < nmb_hc_io_arrays))
			{
				fl_fgets(fiodatbuf,180,fp1);
				// read board number
				for (i=0;i<5;i++)
				{		// find first comma
					if ((fiodatbuf[i] >= '0') && (fiodatbuf[i] <= '9'))
						char_in[i] = fiodatbuf[i];
					else if ((fiodatbuf[i] == ',') || (fiodatbuf[i] == '.'))
					{
						char_in[i] = fiodatbuf[i];
						break;
					}
				}
				if ((char_in[i] == ',') || (char_in[i] == '.'))
				{
					
					char_in[i] = 0; // replace comma with zero to get array number only
					array = (int16)atoi(char_in);
					ix = (int16)(array * 8);
					i++;
					if ((array < nmb_hc_io_arrays) && (ix < (nmb_hc_ios-7)))
					{
						group_io_init = 1;
						strncpy(hcioname[ix+0],&fiodatbuf[i+0],5);
						strncpy(hcioname[ix+1],&fiodatbuf[i+6],5);
						strncpy(hcioname[ix+2],&fiodatbuf[i+12],5);
						strncpy(hcioname[ix+3],&fiodatbuf[i+18],5);
						strncpy(hcioname[ix+4],&fiodatbuf[i+24],5);
						strncpy(hcioname[ix+5],&fiodatbuf[i+30],5);
						strncpy(hcioname[ix+6],&fiodatbuf[i+36],5);
						strncpy(hcioname[ix+7],&fiodatbuf[i+42],5);
						last_array = array;
					}
					else if (array >= 300)
						break;
					else
						group_io_init = 0;
				}
				else
					group_io_init = 0;
				array_cnt++;
			}

			gr_last_array = last_array;

			// Cross Assignment I/O
			if ((fvars[fvxasgn] & 0x05) != 0) 
			{		// front or rear cross assignment
				set_xassignt_io(last_array);
			}
		}
	}
    fl_fclose(fp1);
    
    load_spare_ios();
    remove_relocated_ios();
    
	enable_dog();
	if ((car_io_init == 1) && (group_io_init == 1))
		return 1;
	else
		return 0;		

}

// *************************************************
// Load spare IOS
// *************************************************

void load_spare_ios (void)
{
	int16 io_start = 0;
	int16 io_end = 0;
	int16 i;
	int16 j;

//	MRCAN bus
	io_start = 18 * 8;
	io_end = 138 * 8;
		
	
	for (j=0;j<6;j++)
	{
		if (spare_io_stat[j] == 0x27) 
		{
			
			for(i=io_start;i<io_end;i++)
			{
				if ((ioname[i][0] == SPARE_IOS[j][0]) &&
					(ioname[i][1] == SPARE_IOS[j][1]) &&
					(ioname[i][2] == SPARE_IOS[j][2]) &&
					(ioname[i][3] == SPARE_IOS[j][3]) &&
					(ioname[i][4] == SPARE_IOS[j][4]))
				{
					ioname[i][0] = spare_ioname[j][0];
				    ioname[i][1] = spare_ioname[j][1];
				    ioname[i][2] = spare_ioname[j][2];
				    ioname[i][3] = spare_ioname[j][3];
				    ioname[i][4] = spare_ioname[j][4];
					break;
				}
			}
		}
	}
	
	
// CTCAN bus
	io_start = 153 * 8; 
	io_end = nmb_ios;   // 258 * 8;

	for (j=6;j<12;j++)
	{
		if (spare_io_stat[j] == 0x27)
		{
				
			for(i=io_start;i<io_end;i++)
			{
				if ((ioname[i][0] == SPARE_IOS[j][0]) &&
					(ioname[i][1] == SPARE_IOS[j][1]) &&
					(ioname[i][2] == SPARE_IOS[j][2]) &&
					(ioname[i][3] == SPARE_IOS[j][3]) &&
					(ioname[i][4] == SPARE_IOS[j][4]))
				{
					ioname[i][0] = spare_ioname[j][0];
				    ioname[i][1] = spare_ioname[j][1];
				    ioname[i][2] = spare_ioname[j][2];
				    ioname[i][3] = spare_ioname[j][3];
				    ioname[i][4] = spare_ioname[j][4];
					break;
				}
			}
		}
	}
	
// GRCAN bus

	for (j=12;j<18;j++)
	{
		if (spare_io_stat[j] == 0x27)
		{
			
			for(i=0;i<nmb_hc_ios;i++)
			{
				if ((hcioname[i][0] == SPARE_IOS[j][0]) &&
					(hcioname[i][1] == SPARE_IOS[j][1]) &&
					(hcioname[i][2] == SPARE_IOS[j][2]) &&
					(hcioname[i][3] == SPARE_IOS[j][3]) &&
					(hcioname[i][4] == SPARE_IOS[j][4]))
				{
					hcioname[i][0] = spare_ioname[j][0];
				    hcioname[i][1] = spare_ioname[j][1];
				    hcioname[i][2] = spare_ioname[j][2];
				    hcioname[i][3] = spare_ioname[j][3];
				    hcioname[i][4] = spare_ioname[j][4];
					break;
				}
			}
		}
	}
	
	
}


// *************************************************
// Remove Relocated IOS
// *************************************************

void remove_relocated_ios (void)
{
	int16 io_start = 0;
	int16 io_end = 0;
	int16 i;
	int16 j;

//	MRCAN bus
	io_start = 18 * 8;
	io_end = 138 * 8;
		
	
	for (j=0;j<6;j++)
	{
		if (spare_io_stat[j] != 0)
		{
			
			for(i=io_start;i<io_end;i++)
			{
				if ((ioname[i][0] == spare_ioname[j][0]) &&
					(ioname[i][1] == spare_ioname[j][1]) &&
					(ioname[i][2] == spare_ioname[j][2]) &&
					(ioname[i][3] == spare_ioname[j][3]) &&
					(ioname[i][4] == spare_ioname[j][4]))
				{
					ioname[i][0] = ' ';
				    ioname[i][1] = ' ';
				    ioname[i][2] = ' ';
				    ioname[i][3] = ' ';
				    ioname[i][4] = ' ';
					break;
				}
			}
		}
	}
	
	
// CTCAN bus
	io_start = 153 * 8; 
	io_end = nmb_ios;   // 258 * 8;

	for (j=6;j<12;j++)
	{
		if (spare_io_stat[j] != 0)
		{
			
			for(i=io_start;i<io_end;i++)
			{
				if ((ioname[i][0] == spare_ioname[j][0]) &&
					(ioname[i][1] == spare_ioname[j][1]) &&
					(ioname[i][2] == spare_ioname[j][2]) &&
					(ioname[i][3] == spare_ioname[j][3]) &&
					(ioname[i][4] == spare_ioname[j][4]))
				{
					ioname[i][0] = ' ';
				    ioname[i][1] = ' ';
				    ioname[i][2] = ' ';
				    ioname[i][3] = ' ';
				    ioname[i][4] = ' ';
					break;
				}
			}
		}
	}
	
// GRCAN bus

	for (j=12;j<18;j++)
	{
		if (spare_io_stat[j] != 0)
		{
			
			for(i=0;i<nmb_hc_ios;i++)
			{
				if ((hcioname[i][0] == spare_ioname[j][0]) &&
					(hcioname[i][1] == spare_ioname[j][1]) &&
					(hcioname[i][2] == spare_ioname[j][2]) &&
					(hcioname[i][3] == spare_ioname[j][3]) &&
					(hcioname[i][4] == spare_ioname[j][4]))
				{
					hcioname[i][0] = ' ';
				    hcioname[i][1] = ' ';
				    hcioname[i][2] = ' ';
				    hcioname[i][3] = ' ';
				    hcioname[i][4] = ' ';
					break;
				}
			}
		}
	}
}

// *************************************************
//  Set Cross assignment I/O locations
// *************************************************

void set_xassignt_io (int16 last_array)
{
	int16 i,j,k;
	int16 fl_ix;
	int16 start_array;
	
  
	// start on the next i/o board
  	i_array_ix = last_array;

	start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
	set_input_start(start_array, 0);
	set_output_start(start_array + 1, 4);
 
	if ((fvars[fvxasgn] & 0x01) != 0) 
	{
		
	    // Front Hall Calls
		
		// Inputs must be seperate from outputs
		for (k = 1;k<=cons[grtopf];k+=12)
		{
			j = 1;
			i = j + k;
			while ((j <= 12) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
				if ((g_dcmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'X';
					io_name[3] = 'D';
					io_name[4] = 'I';
					set_grp_input_location(io_name);
		            next_grp_inp();
		            next_grp_outp();
				}
				j++;
				i = j + k;
			}
			
			// start on the next i/o board
			if (j < 12)
				start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
			else
				start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);

			j = 1;
			i = j + k;
			while ((j <= 12) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
				if ((g_dcmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'X';
					io_name[3] = 'D';
					io_name[4] = 'O';
					set_grp_output_location(io_name);
		            next_grp_inp();
		            next_grp_outp();
				}
				j++;
				i = j + k;
			}
			
			// start on the next i/o board
			if (j < 12)
				start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
			else
				start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);
			
			j = 0;
			i = j + k;
			while ((j < 12) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
				if ((g_ucmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'X';
					io_name[3] = 'U';
					io_name[4] = 'I';
					set_grp_input_location(io_name);
		            next_grp_inp();
		            next_grp_outp();
				}
				j++;
				i = j + k;
			}
		
			
			// start on the next i/o board
			if (j < 11)
				start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
			else
				start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);
			j = 0;
			i = j + k;
			while ((j < 12) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
				if ((g_ucmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'X';
					io_name[3] = 'U';
					io_name[4] = 'O';
					set_grp_output_location(io_name);
		            next_grp_inp();
		            next_grp_outp();
				}
				j++;
				i = j + k;
			}
			
			// start on the next i/o board
			if (j < 12)
				start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
			else
				start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);
		}															  
	}
	
	if ((fvars[fvxasgn] & 0x04) != 0) 
	{
		
	    // Rear Hall Calls
		// start on the next i/o board
		if (j < 12)
		{	// if not on a new board then start on one otherwise already set from last front
			start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);
		}		
		
		// Inputs must be seperate from outputs
		for (k = 1;k<=cons[grtopf];k+=12)
		{
			j = 1;
			i = j + k;
			while ((j <= 12) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
				if ((g_rdcmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'Z';
					io_name[3] = 'D';
					io_name[4] = 'I';
					set_grp_input_location(io_name);
		            next_grp_inp();
		            next_grp_outp();
				}
				j++;
				i = j + k;
			}
			
			// start on the next i/o board
			if (j < 12)
				start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
			else
				start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);

			j = 1;
			i = j + k;
			while ((j <= 12) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
				if ((g_rdcmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'Z';
					io_name[3] = 'D';
					io_name[4] = 'O';
					set_grp_output_location(io_name);
		            next_grp_inp();
		            next_grp_outp();
				}
				j++;
				i = j + k;
			}
			
			// start on the next i/o board
			if (j < 12)
				start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
			else
				start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);
			
			j = 0;
			i = j + k;
			while ((j < 12) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
				if ((g_rucmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'Z';
					io_name[3] = 'U';
					io_name[4] = 'I';
					set_grp_input_location(io_name);
		            next_grp_inp();
		            next_grp_outp();
				}
				j++;
				i = j + k;
			}
		
			
			// start on the next i/o board
			if (j < 11)
				start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
			else
				start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);
			j = 0;
			i = j + k;
			while ((j < 12) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
				if ((g_rucmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'Z';
					io_name[3] = 'U';
					io_name[4] = 'O';
					set_grp_output_location(io_name);
		            next_grp_inp();
		            next_grp_outp();
				}
				j++;
				i = j + k;
			}
			
			// start on the next i/o board
			if (j < 12)
				start_array = (int16)(i_array_ix + (3 - (i_array_ix % 3)));
			else
				start_array = i_array_ix;
			set_input_start(start_array, 0);
			set_output_start(start_array + 1, 4);
		}															  
	}
}

//**************************************************
//  Initialize all I/O
//**************************************************

void io_init (void)
{
	int16 i,j;
 	int16 start_ix=0;
 	
 	// IO_In_Out is set to a default but individual io bit are set during the call to init_io()
 	for(j=1;j<=cons[nmbcars];j++)
 	{
 	 	for(i=0;i<nmb_io_brds;i++)
	 		IO_board[j][i] = 0;	
 	}
 	
 	for(i=0;i<nmb_hcio_brds;i++)
 		HC_IO_board[i] = 0;	  
 		
	for (i=0;i<nmb_hc_io_arrays;i++)
	{
		gio_valid[i] = 0;
 	 	GIO_In_Out[i] = 0x00;	// default all others to 
	}
 	
 	
 	// Main i/o board default 
	for (i=0;i<nmb_io_arrays;i++)
	{
		io_valid[i] = 0;
 	 	IO_In_Out[i] = 0x00;	// default all others to 
	}
  	
 	IO_In_Out[10] = 0xFF;	// outputs
 	IO_In_Out[11] = 0xFF;	// outputs
 	IO_In_Out[12] = 0xFF;	// outputs
 	IO_In_Out[13] = 0xFF;	// outputs

 	init_car = cons[carnmb];
 
 #if ((Traction == 0) && (Tract_OL == 0))
//Hydro does not have a conflit with GOVR
	MST_enable = 1;
#endif
	
	init_ionames();

	if (Load_IO_Names() == 0)
		io_name_fault = 1;

	//*********************************************************************************************
	// MAIN I/O BOARD
	//*********************************************************************************************

	//         iostruct,   bus, addr,arr,bit,  name,type,On color,  
	init_io(  o_SUF,MAINIO,"  SUF",0x01,NoLight);
	init_io(   i_SUF,MAINIO," SUFi",0x00,NoLight);
	init_io(   o_SU,MAINIO,"   SU",0x01,NoLight);
	init_io(   i_SU,MAINIO,"  SUi",0x00,NoLight);
	
	init_io(  o_SDF,MAINIO,"  SDF",0x01,NoLight);
	init_io(  i_SDF,MAINIO," SDFi",0x00,NoLight);
	init_io(   o_SD,MAINIO,"   SD",0x01,NoLight);
	init_io(   i_SD,MAINIO,"  SDi",0x00,NoLight);

	init_io(  o_RUN,MAINIO,"  RUN",0x01,NoLight);
	init_io(  i_RUN,MAINIO," RUNi",0x00,NoLight);
	
	init_io(  i_AD,MAINIO,"   AD",0x00,NoLight);
	init_io(  i_IND,MAINIO,"  IND",0x00,NoLight);
	init_io(  i_LBP,MAINIO,"  LBP",0x00,NoLight);
	init_io(  i_GBP,MAINIO,"  GBP",0x00,NoLight);
	init_io(  i_L120,MAINIO," L120",0x00,NoLight);
	init_io(  i_S10,MAINIO,"  S10",0x00,NoLight);

	init_io(  i_GOV,MAINIO,"  GOV",0x00,NoLight);

	init_io(  i_HSS,MAINIO,"  HSS",0x00,NoLight);
	init_io(  i_ACC,MAINIO,"  ACC",0x00,NoLight);
	init_io(  i_BAD,MAINIO,"  BAD",0x00,NoLight);
	init_io(  i_BAU,MAINIO,"  BAU",0x00,NoLight);
	init_io(  i_TAD,MAINIO,"  TAD",0x00,NoLight);
	init_io(  i_TAU,MAINIO,"  TAU",0x00,NoLight);
	init_io(  i_DLB,MAINIO,"  DLB",0x00,NoLight);
	init_io(i_DLB_1,MAINIO,"DLB-1",0x00,NoLight);
	init_io(  i_DLM,MAINIO,"  DLM",0x00,NoLight);
	init_io(i_DLM_1,MAINIO,"DLM-1",0x00,NoLight);
	init_io(  i_DLT,MAINIO,"  DLT",0x00,NoLight);
	init_io(i_DLT_1,MAINIO,"DLT-1",0x00,NoLight);
	
	if (cons[rear] != 0)
	{
		init_io(  i_RLM,MAINIO,"  RLM",0x00,NoLight);
		init_io(i_RLM_1,MAINIO,"RLM-1",0x00,NoLight);
	}
	
	init_io(  i_CEN,MAINIO," CENi",0x00,NoLight);
	init_io(  i_SPD,MAINIO,"  SPD",0x00,NoLight);
	init_io( i_CFLT,MAINIO," CFLT",0x00,NoLight);
	init_io(i_L120B,MAINIO,"L120B",0x00,NoLight);
	init_io( i_NTSF,MAINIO," NTSF",0x00,NoLight);
	init_io( i_PALF,MAINIO," PALF",0x00,NoLight);
	init_io( i_TSTM,MAINIO," TSTM",0x00,NoLight);
	init_io(i_RDOOR,MAINIO,"RDOOR",0x00,NoLight);
	init_io(  i_TSD,MAINIO,"  TSD",0x00,NoLight);

	
	init_io( i_MRIU,MAINIO," MRIU",0x00,NoLight);
	init_io( i_MRIN,MAINIO," MRIN",0x00,NoLight);
	init_io( i_AUTO,MAINIO," AUTO",0x00,NoLight);
	init_io( i_MRSW,MAINIO," MRSW",0x00,NoLight);
	init_io( i_MRID,MAINIO," MRID",0x00,NoLight);
	init_io( i_MRIE,MAINIO," MRIE",0x00,NoLight);	// Machine room inspection input
	init_io(  i_ICA,MAINIO,"  ICA",0x00,NoLight);

	init_io(i_RGS_1,MAINIO,"RGS-1",0x00,NoLight);
	init_io(   i_GS,MAINIO,"   GS",0x00,NoLight);
	init_io( i_GS_1,MAINIO," GS-1",0x00,NoLight);

	init_io(   i_DZ,MAINIO,"   DZ",0x00,NoLight);	// Door Zone input (DZU AND DZD)
	init_io(  i_RGS,MAINIO,"  RGS",0x00,NoLight);
	init_io(  i_CTA,MAINIO,"  CTA",0x00,NoLight);
	init_io(   i_UN,MAINIO,"   UN",0x00,NoLight);	// Up Directional Stop
	init_io(   i_UT,MAINIO,"   UT",0x10,NoLight);	// Up Terminal Slowdown
	init_io(   i_DT,MAINIO,"   DT",0x10,NoLight);	// Down Terminal Slowdosn
	init_io(   i_DN,MAINIO,"   DN",0x00,NoLight);	// Down Directional Stop

	init_io(   i_ID,MAINIO,"   ID",0x00,NoLight);
	init_io(  i_INE,MAINIO,"  IEN",0x00,NoLight);
	init_io(   i_IU,MAINIO,"   IU",0x00,NoLight);
	init_io( i_CTIN,MAINIO," CTIN",0x00,NoLight);
	init_io(  i_ICI,MAINIO,"  ICI",0x00,NoLight);
	init_io(  i_CST,MAINIO,"  CST",0x00,NoLight);	// Car Stop Switch input

	init_io(  o_MCC,MAINIO,"   MC",0x01,NoLight);
	init_io(  i_MCC,MAINIO,"  MCi",0x00,NoLight);


	init_io(  o_DBG,MAINIO,"  DBG",0x01,NoLight);


	init_io(  o_CEN,MAINIO,"  CEN",0x01,NoLight);
	init_io(  o_MTO,MAINIO,"  MTO",0x01,NoLight);
	init_io(  o_STE,MAINIO,"  STE",0x01,NoLight);
	init_io(   o_UL,MAINIO,"  ULO",0x01,NoLight);
	init_io(   o_DL,MAINIO,"  DLO",0x01,NoLight);
	init_io(   o_DZ,MAINIO,"  DZO",0x01,NoLight);
	init_io(o_SELOK,MAINIO,"SELOK",0x01,NoLight);
	init_io( o_SP75,MAINIO," SP75",0x01,NoLight);
	init_io(o_SP150,MAINIO,"SP150",0x01,NoLight);
	init_io(o_HWLRN,MAINIO,"HWLRN",0x01,NoLight);

	if (cons[Low_Oil] == 1)
		init_io(i_LOS,MAINIO,"  LOS",0x00,NoLight);
	if (cons[Low_Press] == 1)
		init_io(i_LPS,MAINIO,"  LPS",0x00,NoLight);
	if (cons[hotoilSW] != 0)
		init_io(i_TPH,MAINIO,"  TPH",0x00,NoLight);
	init_io(i_TPL,MAINIO,"  TPL",0x00,NoLight);


	// APS Software inputs

	init_io(    i_UL,MAINIO,"   UL",0x10,NoLight);	// Up level input
	init_io(   i_DZA,MAINIO,"  DZA",0x10,NoLight);	// Door Zone A input (DZU OR DZD)
	init_io(    i_DL,MAINIO,"   DL",0x10,NoLight);	// Down Level Input
	init_io(  i_DZsel,MAINIO," DZ-1",0x10,NoLight);	// Door Zone A input (DZU OR DZD)
	

	init_io( i_UNsel,MAINIO," UN-1",0x10,NoLight);	// Up Terminal Slowdown
	init_io( i_UTsel,MAINIO," UT-1",0x10,NoLight);	// Up Terminal Slowdown
	init_io( i_DNsel,MAINIO," DN-1",0x10,NoLight);	// Down Terminal Slowdosn
	init_io( i_DTsel,MAINIO," DT-1",0x10,NoLight);	// Down Terminal Slowdosn
	



//************************************
// Set the Option I/O Locations
//************************************

	

//****************************************
// Machine room I/O
//****************************************



	// fire Service
	
	init_io(i_FS,MRCAN,"   FS",0x10,NoLight);
	init_io(i_BP,MRCAN,"   BP",0x10,NoLight);
	init_io(i_FSX,MRCAN,"  FSX",0x10,NoLight);
	init_io(i_ALT,MRCAN,"  ALT",0x10,NoLight);
	init_io(i_MES,MRCAN,"  MES",0x10,NoLight);
	init_io(i_MRS,MRCAN,"  MRS",0x10,NoLight);
	init_io(i_HWS,MRCAN,"  HWS",0x10,NoLight);
	init_io(i_HWS2,MRCAN," HWS2",0x10,NoLight);
	
	init_io(  o_FLH,MRCAN,"  FLH",0x01,NoLight);
	init_io(  o_BUZ,MRCAN,"  BUZ",0x01,NoLight);


	// Emergency Power

	init_io(i_EMP,MRCAN,"  EMP",0x10,NoLight);
	init_io(i_EPS,MRCAN,"  EPS",0x10,NoLight);



	init_io(i_EPT,MRCAN,"  EPT",0x10,NoLight);

	if ((cons[FI_EP_Ext] != 0) || (cons[FiCntrlCentr] == 1) || (cons[Australia] == 1))
	{
		init_io( o_NPL,MRCAN,"  NPL",0x11,NoLight);		// Normal Power Light
		init_io(o_EPLH,MRCAN," EPLH",0x11,NoLight);		// Emergency Power Light
		init_io(o_EPRL,MRCAN," EPRL",0x11,NoLight);		// Emergency power Recall
		init_io(o_EPSL,MRCAN," EPSL",0x11,NoLight);		// Emergency Power Select
		init_io(o_FS1L,MRCAN," FS1L",0x11,NoLight);		// Fire phase 1 light
		init_io(o_FS2L,MRCAN," FS2L",0x11,NoLight);		// Fire phase 2 light
	}


	if ((cons[mand] != 0) || (cons[frghtd] != 0) || (cons[Australia] == 1))
	{
		init_io(  i_TDC,MRCAN,"  TDC",0x10,NoLight);
		init_io(  i_MDC,MRCAN,"  MDC",0x10,NoLight);
		init_io(  i_BDC,MRCAN,"  BDC",0x10,NoLight);

		if (cons[rear] != 0)
		{
			init_io( i_MDCR,MRCAN," MDCR",0x10,NoLight);
		}
	}
	
	if (Drive_Type[cons[carnmb]] == 1)
		init_io(   o_MST,MRCAN,"  MST",0x01,NoLight);


	if ((cons[FiCntrlCentr] == 1) || (cons[openlout] != 0))
	{
		init_io(o_OPENL,MRCAN,"OPENL",0x11,NoLight);
	}
	if ((cons[distress] != 0) || (cons[FiCntrlCentr] == 1) || ((cons[servOUT] & 0x08) != 0))
	{
		oserl_enable = 1;
		init_io(o_OSERL,MRCAN,"OSERL",0x11,NoLight);
		if(cons[distress] == 1)
		{
			init_io(o_OSERB,MRCAN,"OSERB",0x11,NoLight);
			init_io(i_OSERA,MRCAN,"OSERA",0x10,NoLight);
		
		}
		if (cons[distress] == 2)
		{
			init_io(o_OSERD,MRCAN,"OSERD",0x11,NoLight);
		}
	}
	
	if ((Drive_Type[cons[carnmb]] == 1) || (Drive_Type[cons[carnmb]] == 9))  
	{		   // Hydro car
		init_io(o_ISPD,MRCAN," ISPD",0x11,NoLight);
	}
	
	if ((cons[servOUT] & 0x04) != 0)
		init_io(o_ISER,MRCAN," ISER",0x11,NoLight);
	
	if ((cons[Hall_Lant] & 0x04) != 0)
		init_io(o_TCU,MRCAN,"  TCU",0x11,NoLight);

	if(cons[exDHwLatFL] == 1)
		init_io(i_EDHL,MRCAN," EDHL",0x10,NoLight);
	
	if (cons[Remote_CC] != 0)
	{
		init_io(i_SCS,MRCAN,"  SCS",0x10,NoLight);
		if ((cons[Remote_CC] == 2) || (cons[Remote_CC] == 3))
			init_io(i_RM_ED,MRCAN,"   ED",0x10,NoLight);
		if (cons[Remote_CC] == 1)
			init_io(i_RM_INDC,MRCAN,"  IND",0x10,NoLight);
		
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if (ccmsk[cons[carnmb]][i] == 1)
			{
				io_name[0] = ' ';
				io_name[1] = ' ';
				io_name[2] = flmrk[i][0];
				io_name[3] = flmrk[i][1];
				io_name[4] = 'C';
				io_name[5] = 'A';
				init_io(i_RM_CC(i),MRCAN,&io_name[0],0x10,NoLight);
				init_io(o_RM_CCL(i),MRCAN,&io_name[1],0x01,NoLight);
			}		
		}
			
		if (cons[rear] != 0)
		{
			init_io(i_SCSR,MRCAN," SCSR",0x10,NoLight);
			if ((cons[Remote_CC] == 2) || (cons[Remote_CC] == 3))
				init_io(i_RM_EDR,MRCAN,"  EDR",0x10,NoLight);
			
			for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
			{
				if (rccmsk[cons[carnmb]][i] == 1)
				{
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'C';
					io_name[4] = 'R';
					io_name[5] = 'A';
					
					init_io(i_RM_CCR(i),MRCAN,&io_name[0],0x10,NoLight);
					init_io(o_RM_CCRL(i),MRCAN,&io_name[1],0x01,NoLight);
				}
			}
		}

	}
	
	init_io(i_FEP,MRCAN,"  FEP",0x10,NoLight);

//	if (Car_Call_Lockouts == 1)	
//		init_io(o_INSEC,MRCAN,"INSEC",0x11,NoLight);

   	if ((cons[Hall_Lant] & 0x01) != 0)
   	{
  		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if (dcnmsk[cons[carnmb]][i] == 1)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'L';
				io_name[4] = 'D';
				init_io(o_DNL(i),MRCAN,&io_name[0],0x11,NoLight);
			}
			if (ucnmsk[cons[carnmb]][i] == 1)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'L';
				io_name[4] = 'U';
				init_io(o_UPL(i),MRCAN,&io_name[0],0x11,NoLight);
			}
		}
		
		if (cons[rear] != 0)
		{
			for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
			{
				if (rdcnmsk[cons[carnmb]][i] == 1)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'L';
					io_name[3] = 'D';
					io_name[4] = 'R';
					init_io(o_DNLR(i),MRCAN,&io_name[0],0x11,NoLight);
				}
				if (rucnmsk[cons[carnmb]][i] == 1)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'L';
					io_name[3] = 'U';
					io_name[4] = 'R';
					init_io(o_UPLR(i),MRCAN,&io_name[0],0x11,NoLight);
				}
			}
		}
 		
   	}

	if ((cons[Hall_Lant] & 0x02) != 0)
	{	// Discrete PI in the hall
		init_io(o_UDA,MRCAN,"  UDA",0x11,NoLight);
		init_io(o_DDA,MRCAN,"  DDA",0x11,NoLight);
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if (valid_fl[i] != 0)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'P';
				io_name[4] = 'I';
				init_io(o_PI(i),MRCAN,&io_name[0],0x11,NoLight);
			}
		}

	}
	else if ((cons[Hall_Lant] & 0x80) != 0)
	{		// binary PI's in the hall
		init_io(o_UDA,MRCAN,"  UDA",0x11,NoLight);
		init_io(o_DDA,MRCAN,"  DDA",0x11,NoLight);
		init_io(o_DPI1,MRCAN," DPI1",0x11,NoLight);
		init_io(o_DPI2,MRCAN," DPI2",0x11,NoLight);
		if (cons[topf] > 3)
			init_io(o_DPI3,MRCAN," DPI3",0x11,NoLight);
		if (cons[topf] > 7)
			init_io(o_DPI4,MRCAN," DPI4",0x11,NoLight);
		if (cons[topf] > 15)
			init_io(o_DPI5,MRCAN," DPI5",0x11,NoLight);
		if (cons[topf] > 31)
			init_io(o_DPI6,MRCAN," DPI6",0x11,NoLight);
	}
	else if ((cons[dispis] & 0x40) != 0)
	{		// radial PI
		init_io(o_UDA,MRCAN,"  UDA",0x11,NoLight);
		init_io(o_DDA,MRCAN,"  DDA",0x11,NoLight);
		init_io(o_DPI1,MRCAN," DPI1",0x11,NoLight);
		init_io(o_DPI2,MRCAN," DPI2",0x11,NoLight);
		init_io(o_DPI3,MRCAN," DPI3",0x11,NoLight);
		init_io(o_DPI4,MRCAN," DPI4",0x11,NoLight);
		init_io(o_DPI5,MRCAN," DPI5",0x11,NoLight);
		init_io(o_DPI6,MRCAN," DPI6",0x11,NoLight);
	}


	if ((cons[dispis] & 2) != 0)
	{
		init_io(o_LU,MRCAN,"  LUL",0x11,NoLight);
		init_io(o_LD,MRCAN,"  LDL",0x11,NoLight);
		if (cons[rear] != 0)
		{
			init_io(o_LUR,MRCAN," LULR",0x11,NoLight);
			init_io(o_LDR,MRCAN," LDLR",0x11,NoLight);
		}
	}


	if (((cons[Elev_off] & 8) != 0) || (((fvars[fvheloff2] & 0x08) != 0)))
	{
		if (init_io(o_HEOFL,MRCAN," ELOO",0x11,NoLight) == 0)
			init_io(o_HEOFL,MRCAN,"HEOFL",0x11,NoLight);
	}


	if (cons[otslbp] != 0)
	{
#if ((Traction == 1) || (Tract_OL == 1))
		init_io(i_RTL,MRCAN,"  RTL",0x10,NoLight);
#else
		init_io(i_RTLH,MRCAN," RTLH",0x10,NoLight);
#endif
		if ((cons[otslbp] & 0x04) != 0)
		{
			init_io(i_RTLDC,MRCAN,"RTLDC",0x10,NoLight);
		}
	}


#if (Traction == 1)	
	if((cons[EP_Profile] & 0x02) != 0)
		init_io(i_RSPD,MRCAN," RSPD",0x10,NoLight);
#endif	

	if (cons[equake] == 1) 
	{		// add earthquake 
		init_io(i_ETH,MRCAN,"  ETH",0x10,NoLight);
		init_io(o_EQL,MRCAN,"  EQL",0x11,NoLight);

#if ((Traction == 1) || (Tract_OL == 1))
		init_io(i_COL,MRCAN,"  COL",0x10,NoLight);
#endif
		init_io(i_EQR,MRCAN,"  EQR",0x10,NoLight);

#if (Tract_OL == 1)
		init_io(i_CWS,MRCAN,"  CWS",0x10,NoLight);
#endif
	}


	if ((cons[hugs] == 1) || (cons[hugs] == 3))
	{
		init_io(o_HUGO,MRCAN," HUGO",0x11,NoLight);
	}

	if (cons[hugs]== 3)
	{
		init_io(i_HUGI,MRCAN," HUGI",0x10,NoLight);
		init_io(i_HUGB,MRCAN," HUGB",0x10,NoLight);
		init_io(o_HUGA,MRCAN," HUGA",0x11,NoLight);
	}


	if ((cons[Sabbath] == 4) || (cons[Sabbath] == 6) || (cons[Sabbath] == 5) || (cons[Sabbath] == 7))
	{
		init_io(o_SABO,MRCAN," SABO",0x11,NoLight);
	}
	
	if ((cons[Sabbath] != 0)&&(cons[Super_Sabbath] == 1))
	{
		init_io(o_SABUZ,MRCAN,"SABUZ",0x11,NoLight);
		init_io(o_SABEE,MRCAN,"SABEE",0x11,NoLight);
	}
	
	if (cons[cblue] == 1)
	{
		init_io(o_EMLH,MRCAN," EMLH",0x11,NoLight);// CBL
	}
	else if (cons[medEm] == 1) 
	{		// Medical Emergency Service
		init_io(i_EMSH,MRCAN," EMSH",0x10,NoLight);
		init_io(o_EMLH,MRCAN," EMLH",0x11,NoLight);
	}

	if ((cons[sec_dis] & 0x01) != 0)
		init_io(i_SCD,MRCAN,"  SCD",0x10,NoLight);


	if((Car_Call_Lockouts == 1) && (cons[CCS_BUS] != 1))
	{
		if((cons[CCS_BUS] & 0x02) == 0)
		{//do enable		
			init_io(i_SECUR,MRCAN,"SECUR",0x10,NoLight);
		}
		if((cons[CCS_BUS] & 0x04) == 0)
		{//do inputs
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if (ccmsk[cons[carnmb]][i] == 1)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'C';
					io_name[3] = 'C';
					io_name[4] = 'S';
					init_io(i_CCS(i),MRCAN,io_name,0x10,NoLight);
				}
			}
		}		
		if (cons[rear] != 0)
		{
			if((cons[CCS_BUS] & 0x02) == 0)
			{//do enable
				init_io(i_RSECU,MRCAN,"RSECU",0x10,NoLight);
			}
			if((cons[CCS_BUS] & 0x04) == 0)
			{//do inputs
				for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
				{
					if (rccmsk[cons[carnmb]][i] == 1)
					{
						io_name[0] = flmrk[i][0];
						io_name[1] = flmrk[i][1];
						io_name[2] = 'R';
						io_name[3] = 'C';
						io_name[4] = 'S';
						
						init_io(i_RCCS(i),MRCAN,io_name,0x10,NoLight);
					}
				}
			}
		}
	}



	if((Lockout_Override == 1) && (cons[CCS_BUS] != 1))
	{
		if((cons[CCS_BUS] & 0x02) == 0)
		{//do enable
			init_io(i_OSEC,MRCAN," OSEC",0x10,NoLight);
		}
		if((cons[CCS_BUS] & 0x04) == 0)
		{//do inputs		
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if (ccmsk[cons[carnmb]][i] == 1)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'O';
					io_name[3] = 'C';
					io_name[4] = 'S';
					init_io(i_OCS(i),MRCAN,io_name,0x10,NoLight);
				}
			}
		}
		if (cons[rear] != 0)
		{
			if((cons[CCS_BUS] & 0x02) == 0)
			{//do enable			
				init_io(i_ORSEC,MRCAN,"ORSEC",0x10,NoLight);
			}
			if((cons[CCS_BUS] & 0x04) == 0)
			{//do inputs			
				for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
				{
					if (rccmsk[cons[carnmb]][i] == 1)
					{
						io_name[0] = flmrk[i][0];
						io_name[1] = flmrk[i][1];
						io_name[2] = 'O';
						io_name[3] = 'R';
						io_name[4] = 'S';
						
						init_io(i_ORS(i),MRCAN,io_name,0x10,NoLight);
					}
				}
			}
		}
	}

	
	if ((cons[frghtd] == 6) || (cons[frghtd] == 7) || (cons[frghtd] == 8))
	{
		init_io(o_DCA,MRCAN,"  DCA",0x11,NoLight);
	}
	 

	if (cons[GLBP_LED] == 1) 
	{	// Gate and Lock bypass LED output (external) for Rudy Rule
		init_io(o_GBL,MRCAN,"  GBL",0x11,NoLight);
		init_io(o_LBL,MRCAN,"  LBL",0x11,NoLight);
	}


	if (cons[Micro_Relev] == 1)
	{
		init_io(o_UPML,MRCAN," UPML",0x11,NoLight);
		init_io(o_DNML,MRCAN," DNML",0x11,NoLight);
	}
	

	if ((cons[mand] != 0) && (cons[mand] != 2))
	{
		init_io(i_DOL,MRCAN,"  DOL",0x10,NoLight);
		init_io(i_DCL,MRCAN,"  DCL",0x10,NoLight);
		if (cons[rear] != 0)
		{
			init_io(i_DOLR,MRCAN," DOLR",0x10,NoLight);
			init_io(i_DCLR,MRCAN," DCLR",0x10,NoLight);
		}
	}
	
	if ((cons[mand] != 0) || (cons[frghtd] != 0) || (cons[RCM_HWDoor] == 1))
	{
		init_io(o_RCM,MRCAN,"  RCM",0x11,NoLight);
//		init_io(o_RCMR,MRCAN," RCMR",0x01,NoLight);
	}

	if (cons[door_type] == 1) // otis ovl door operator
	{
		
		init_io(o_REV,MRCAN,"  REV",0x11,NoLight);
		init_io(o_RVD,MRCAN,"  RVD",0x11,NoLight);
		init_io(o_INSO,MRCAN," INSO",0x11,NoLight);

		if (cons[rear] != 0)
		{
			init_io(o_REVR,MRCAN," REVR",0x11,NoLight);
			init_io(o_RVDR,MRCAN," RVDR",0x11,NoLight);
		}
	}
	


#if ((Traction == 1) || (Tract_OL == 1))

	if (fvars[fvrcmfrght] == 1)
	{
		rcm_frght_out_en = 1;
		init_io(o_RCF,MRCAN,"  RCF",0x11,NoLight);
	}

#endif

	if ((cons[Elev_off] & 2) != 0)
	{
		init_io(i_HEOF,MRCAN," HEOF",0x10,NoLight);
   		if ((cons[Elev_off] & 4) != 0)
	   	{
			if (init_io(i_HEOFR,MRCAN,"  EOR",0x10,NoLight) == 0)
				init_io(i_HEOFR,MRCAN,"HEOFR",0x10,NoLight);	
		}
	}
	
	if(cons[Prison] == 1)
	{
		init_io(i_JAILB,MRCAN,"JAILB",0x10,NoLight);
		if (init_io(i_HEOFR,MRCAN,"  EOR",0x10,NoLight) == 0)
			init_io(i_HEOFR,MRCAN,"HEOFR",0x10,NoLight);
	}


	if (cons[MtrBlower] == 1) 
	{
		init_io(o_MTB,MRCAN,"  MTB",0x11,NoLight);
	}

  	if ((cons[SecFlCfg] == 1) || (cons[SecFlCfg] == 3) || (cons[SecFlCfg] == 5) || (cons[SecFlCfg] == 7) || (cons[SecFlCfg] == 8))
	{
		init_io(i_SECFM,MRCAN,"SECFM",0x10,NoLight);
	}
	
	if (cons[Sabbath] != 0)
	{
		init_io(i_SAB,MRCAN,"  SAB",0x10,NoLight);
	}
	
	if (cons[Alt_Ascii_pi] == 2) 
		init_io(i_ALTPI,MRCAN,"ALTPI",0x10,NoLight);
	
	if (cons[cc_pb_sec] == 1)
	{
		init_io(i_CCPBS,MRCAN,"CCPBS",0x10,NoLight);
	}


	if (cons[Auto_Swing_DO] == 1) 
	{
		init_io(o_FSDO,MRCAN," FSDO",0x11,NoLight);
		if (cons[rear] != 0)
		{
			init_io(o_RSDO,MRCAN," RSDO",0x11,NoLight);
		}
	}


	if (cons[GOV_Reset] == 1)
	{
		init_io(o_GOVR,MRCAN," GOVR",0x11,NoLight);
		init_io(o_GOVT,MRCAN," GOVT",0x11,NoLight);
	}

	// 2=peele,3=courion modelE,4=ems, 5=courion model D
	if (cons[frghtd] == 2)	   // peele
	{
		init_io(o_INSP,MRCAN," INSP",0x11,NoLight);
		init_io(o_ADO,MRCAN,"  ADO",0x11,NoLight);
		init_io(o_PH1R,MRCAN," PH1R",0x11,NoLight);
		init_io(o_DESL,MRCAN," DESL",0x11,NoLight);
		init_io(o_PH2ON,MRCAN,"PH20N",0x11,NoLight);
		init_io(o_PH2HD,MRCAN,"PH2HD",0x11,NoLight);
		init_io(o_PH2OF,MRCAN,"PH20F",0x11,NoLight);
		init_io(o_HDB,MRCAN,"  HDB",0x11,NoLight);
		init_io(o_RC,MRCAN,"   RC",0x11,NoLight);
		init_io(o_SQUEZ,MRCAN,"SQUEZ",0x11,NoLight);

		if (cons[StagRear] == 1)
		{
			init_io(o_RDT,MRCAN,"  RDT",0x11,NoLight);
		}
		if (cons[rear] != 0)
		{  
			init_io(o_RADO,MRCAN," RADO",0x11,NoLight);
		}
	
		init_io(o_DAO,MRCAN,"  DAO",0x11,NoLight);
	}

	if ((cons[frghtd] == 3) || (cons[frghtd] == 5))	   // courion model D or E
	{
		init_io(o_MPTR,MRCAN," MPTR",0x11,NoLight);
		init_io(o_FPH2,MRCAN," FPH2",0x11,NoLight);
		init_io(o_CODG,MRCAN," CODG",0x11,NoLight);
		init_io(o_IAO,MRCAN,"  IAO",0x11,NoLight);
		init_io(o_COHS,MRCAN," COHS",0x11,NoLight);
		init_io(o_PH2HD,MRCAN,"PH2HD",0x11,NoLight);
		init_io(o_FPH1,MRCAN," FPH1",0x11,NoLight);
		init_io(o_ATO,MRCAN,"  ATO",0x11,NoLight);
		init_io(o_RAHS,MRCAN," RAHS",0x11,NoLight);
		init_io(o_RC,MRCAN,"   RC",0x11,NoLight);

		if (cons[rear] != 0)
		{  
			init_io(o_RADO,MRCAN," RADO",0x11,NoLight);
		}
	
	}

	if (cons[frghtd] == 4)		 // ems
	{
		init_io(o_EMS1Z,MRCAN,"EMS1Z",0x11,NoLight);
		init_io(o_EMS2Z,MRCAN,"EMS2Z",0x11,NoLight);
		init_io(o_EMS3Z,MRCAN,"EMS3Z",0x11,NoLight);
		init_io(o_EMS4Z,MRCAN,"EMS4Z",0x11,NoLight);
		init_io(o_EMS5Z,MRCAN,"EMS5Z",0x11,NoLight);
		init_io(o_EMS6Z,MRCAN,"EMS6Z",0x11,NoLight);
		init_io(o_EMS7Z,MRCAN,"EMS7Z",0x11,NoLight);
		init_io(o_EMS8Z,MRCAN,"EMS8Z",0x11,NoLight);
		init_io(o_EMS9Z,MRCAN,"EMS9Z",0x11,NoLight);
		init_io(o_EMS10Z,MRCAN,"EM10Z",0x11,NoLight);
		init_io(o_EMS11Z,MRCAN,"EM11Z",0x11,NoLight);
		init_io(o_DSC,MRCAN,"  DSC",0x11,NoLight);
		init_io(o_FDOB,MRCAN," FDOB",0x11,NoLight);
		init_io(o_RDOB,MRCAN," RDOB",0x11,NoLight);
		init_io(o_HCO,MRCAN,"  HCO",0x11,NoLight);
		init_io(o_FRCL,MRCAN," FRCL",0x11,NoLight);
		init_io(o_FPS2,MRCAN," FPS2",0x11,NoLight);
		init_io(o_FH,MRCAN,"   FH",0x11,NoLight);
		init_io(o_TC,MRCAN,"   TC",0x11,NoLight);
		init_io(o_RC,MRCAN,"   RC",0x11,NoLight);
		init_io(o_EFST,MRCAN," EFST",0x11,NoLight);
	}

	if ((cons[frghtd] == 6) || (cons[frghtd] == 7) || (cons[frghtd] == 8)) 
	{	   // Automatic front or rear freight door
		init_io(o_RC,MRCAN,"   RC",0x11,NoLight);
		init_io(o_FRT1Z,MRCAN,"FRT1Z",0x11,NoLight);
		init_io(o_FRT2Z,MRCAN,"FRT2Z",0x11,NoLight);
		init_io(o_FRT3Z,MRCAN,"FRT3Z",0x11,NoLight);
		init_io(o_PDO,MRCAN,"  PDO",0x11,NoLight);
		init_io(o_PDOR,MRCAN," PDOR",0x11,NoLight);
	}
	
	if (cons[frghtd] == 9)	   // Guilbert
	{
		init_io(o_G_FH1R,MRCAN," FH1R",0x11,NoLight);
		init_io(o_G_FDCBO,MRCAN,"FDCBO",0x11,NoLight);
		init_io(o_G_RDCBO,MRCAN,"RDCBO",0x11,NoLight);
		init_io(o_G_PH2ON,MRCAN,"PH2ON",0x11,NoLight);
		init_io(o_G_PH2HD,MRCAN,"PH2HD",0x11,NoLight);
		init_io(o_G_PH2OF,MRCAN,"PH2OF",0x11,NoLight);
		init_io(o_G_INSP,MRCAN," INSP",0x11,NoLight);
		init_io(o_G_SOPEN,MRCAN,"SOPEN",0x11,NoLight);
		init_io(o_G_ADO,MRCAN,"  ADO",0x11,NoLight);
		init_io(o_G_ADOH,MRCAN," ADOH",0x11,NoLight);
		init_io(o_G_RDO,MRCAN,"  RDO",0x11,NoLight);
		init_io(o_G_RDOH,MRCAN," RDOH",0x11,NoLight);
		init_io(o_G_RC,MRCAN,"   RC",0x11,NoLight);
		init_io(o_G_ININD,MRCAN,"ININD",0x11,NoLight);
	}

	if ((cons[frghtd] == 10) || (cons[frghtd] == 11) || (cons[frghtd] == 12))	   // peelle front and rear auto door
	{
		init_io(o_RC,MRCAN,"   RC",0x11,NoLight);
		init_io(o_PDCA,MRCAN," PDCA",0x11,NoLight);
	   	
		if ((cons[frghtd] == 10) || (cons[frghtd] == 12))
		{	// front auto peelle doors enabled

			init_io(i_HDOB,MRCAN," HDOB",0x10,NoLight);
			init_io(i_HDCB,MRCAN," HDCB",0x10,NoLight);

			if (cons[eetst_dis] == 0)
			{
				init_io(i_FETST,MRCAN,"FETSi",0x10,NoLight);
				init_io(o_FETST,MRCAN,"FETST",0x11,NoLight);
			}

			init_io(i_EE2,MRCAN,"  EE2",0x10,NoLight);
			init_io(i_DSI,MRCAN,"  DSI",0x10,NoLight);
		}

		if ((cons[rear] != 0) && ((cons[frghtd] == 11) || (cons[frghtd] == 12))) 
		{	// rear peelle doors enabled
			init_io(i_HDOBR,MRCAN,"HDOBR",0x10,NoLight);
			init_io(i_HDCBR,MRCAN,"HDCBR",0x10,NoLight);

			if (cons[eetst_dis] == 0)
			{
				init_io(i_RETST,MRCAN,"RETSi",0x10,NoLight);
				init_io(o_RETST,MRCAN,"RETST",0x11,NoLight);
			}

			init_io(i_EER2,MRCAN," EER2",0x10,NoLight);
			init_io(i_RDSI,MRCAN," RDSI",0x10,NoLight);
		}
	}

	if (cons[DZDoorEn] == 2)
	{
		init_io(i_RDZE,MRCAN," RDZE",0x10,NoLight);
		init_io(o_PFDCA,MRCAN,"PFDCA",0x11,NoLight);
	}



	if ((cons[housing] == 1) || (cons[housing] == 2))
	{
		init_io(i_PDS,MRCAN,"  PDS",0x10,NoLight);
		init_io(i_BFS,MRCAN,"  BFS",0x10,NoLight);
		init_io(i_CMP,MRCAN,"  CMP",0x10,NoLight);
		init_io(i_TEE,MRCAN,"  TEE",0x10,NoLight);
		init_io(i_SEE,MRCAN,"  SEE",0x10,NoLight);
		init_io(i_CCP,MRCAN,"  CCP",0x10,NoLight);
		init_io(i_CCLP,MRCAN," CCLP",0x10,NoLight);
		init_io(i_HCLP,MRCAN," HCLP",0x10,NoLight);
	}

	if (cons[housing] == 2)
	{

		init_io(i_HMO,MRCAN,"  HMO",0x10,NoLight);
		init_io(i_DMO,MRCAN,"  DMO",0x10,NoLight);
		init_io(i_LHCP,MRCAN," LHCP",0x10,NoLight);
	}

	if (cons[Reset_Jack] == 1)
	{			// Reset jack operation
		init_io(o_DNO,MRCAN,"  DNO",0x11,NoLight);
	}

	

	if (cons[PWL_Brake] == 1)
	{
		init_io(o_MBP,MRCAN,"  MBP",0x11,NoLight);
		init_io(o_MBC,MRCAN,"  MBC",0x11,NoLight);
	}

	if (cons[Emp_Recover] == 2)
	{
		init_io(o_NPD,MRCAN,"  NPD",0x11,NoLight);
		init_io(o_EPD,MRCAN,"  EPD",0x11,NoLight);
		init_io(o_EPO,MRCAN,"  EPO",0x11,NoLight);
	}
 	if (cons[PWL_Brake] == 2)
	{
		init_io(o_MBC,MRCAN,"  MBC",0x11,NoLight);
	}

	if(cons[TUG] == 1)
	{
		init_io(i_TGDO,MRCAN," TGDO",0x10,NoLight);
		init_io(i_TGDC,MRCAN," TGDC",0x10,NoLight);
		init_io(i_TGDOR,MRCAN,"TGDOR",0x10,NoLight);
		init_io(i_TGDCR,MRCAN,"TGDCR",0x10,NoLight);
		init_io(i_THDS,MRCAN," THDS",0x10,NoLight);
		init_io(o_TDOL,MRCAN," TDOL",0x11,NoLight);
		init_io(o_TDCL,MRCAN," TDCL",0x11,NoLight);
		init_io(o_TDOLR,MRCAN,"TDOLR",0x11,NoLight);
		init_io(o_TDCLR,MRCAN,"TDCLR",0x11,NoLight);
		init_io(o_TAVL,MRCAN," TAVL",0x11,NoLight);
		init_io(o_TAVL2,MRCAN,"TAVL2",0x11,NoLight);
		init_io(o_TIND,MRCAN," TIND",0x11,NoLight);
		init_io(o_TMED,MRCAN," TMED",0x11,NoLight);
		init_io(o_TFSO,MRCAN," TFSO",0x11,NoLight);

		init_io(o_TP1,MRCAN,"  TP1",0x11,NoLight);
		init_io(o_TP2,MRCAN,"  TP2",0x11,NoLight);
		init_io(o_TP3,MRCAN,"  TP3",0x11,NoLight);
		init_io(o_TP4,MRCAN,"  TP4",0x11,NoLight);
		init_io(o_TP5,MRCAN,"  TP5",0x11,NoLight);
		init_io(o_TP6,MRCAN,"  TP6",0x11,NoLight);
	}

	if ((cons[servOUT] & 0x02) != 0)
		init_io(o_SERV,MRCAN,"SERVO",0x11,NoLight);
	


	if ((cons[smoke_interface] & 0x01) != 0) 
	{
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			io_name[0] = flmrk[i][0];
			io_name[1] = flmrk[i][1];
			io_name[2] = 'S';
			io_name[3] = 'M';
			io_name[4] = 'K';
			init_io(i_SMK(i),MRCAN,&io_name[0],0x10,NoLight);
		}
		init_io(o_MESO,MRCAN," MESO",0x11,NoLight);
		init_io(o_ALTO,MRCAN," ALTO",0x11,NoLight);
	}

	if (cons[Auto_Swing_DO] == 2)
	{
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			io_name[0] = flmrk[i][0];
			io_name[1] = flmrk[i][1];
			io_name[2] = 'A';
			io_name[3] = 'D';
			io_name[4] = 'O';
			init_io(o_ADOF(i),MRCAN,&io_name[0],0x10,NoLight);
		}
	}
	
	if ((cons[Auto_Swing_DO] == 2) && (cons[rear] != 0)) 
	{
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			io_name[0] = flmrk[i][0];
			io_name[1] = flmrk[i][1];
			io_name[2] = 'R';
			io_name[3] = 'D';
			io_name[4] = 'O';
			init_io(o_ADOR(i),MRCAN,&io_name[0],0x11,NoLight);
		}
	}
	
	
	init_io(i_MSP1,MRCAN,"MSPI1",0x10,NoLight);
	init_io(i_MSP2,MRCAN,"MSPI2",0x10,NoLight);
	init_io(i_MSP3,MRCAN,"MSPI3",0x10,NoLight);
	init_io(o_MSP1,MRCAN,"MSPO1",0x11,NoLight);
	init_io(o_MSP2,MRCAN,"MSPO2",0x11,NoLight);
	init_io(o_MSP3,MRCAN,"MSPO3",0x11,NoLight);
	

	//****************************************
	// Car and car top I/O
	//****************************************

	init_io(i_DOLR,CTCAN," DOLR",0x10,NoLight);
	init_io(i_DCLR,CTCAN," DCLR",0x10,NoLight);
	init_io( i_RPM,CTCAN,"  RPM",0x10,NoLight);
	init_io( i_EER,CTCAN,"  EER",0x10,NoLight);
	init_io( i_SER,CTCAN,"  SER",0x10,NoLight);
	init_io( o_DOR,CTCAN,"  DOR",0x11,NoLight);
	init_io( o_DCR,CTCAN,"  DCR",0x11,NoLight);
	init_io( o_HVDR,CTCAN," HVDR",0x11,NoLight);
	init_io(o_NUDR,CTCAN," NUDR",0x11,NoLight);

	init_io(  i_FFS,CTCAN,"  FFS",0x10,NoLight);
	init_io(  i_CSS,CTCAN,"  CSS",0x10,NoLight);
	init_io(  i_CTS,CTCAN,"  CTS",0x10,NoLight);
	init_io(  i_DET,CTCAN,"  DET",0x10,NoLight);
	init_io(  i_FST,CTCAN," FSTi",0x10,NoLight);
	init_io(  i_HWSET,CTCAN,"HWSET",0x10,NoLight);

	init_io(  o_FST,CTCAN,"  FST",0x11,NoLight);
	init_io( o_FSTP,CTCAN," FSTP",0x11,NoLight);
	init_io(  o_IFL,CTCAN,"  IFL",0x11,NoLight);
	init_io(  o_IFB,CTCAN,"  IFB",0x11,NoLight);
	
	init_io(  o_CUL,CTCAN,"  CUL",0x11,NoLight);
	init_io(  o_CDL,CTCAN,"  CDL",0x11,NoLight);
	init_io(  o_FAN,CTCAN,"  FAN",0x11,NoLight);
	init_io(o_LIGHT,CTCAN,"  LIG",0x11,NoLight);
	init_io(o_SELCM,CTCAN,"SELCM",0x11,NoLight);
	init_io(o_NTSCM,CTCAN,"NTSCM",0x11,NoLight);
	
	if ((cons[mand] == 0) || (cons[mand] == 2))
	{
		init_io(i_DOL,CTCAN,"  DOL",0x10,NoLight);
		init_io(i_DCL,CTCAN,"  DCL",0x10,NoLight);
		init_io(i_DPM,CTCAN,"  DPM",0x10,NoLight);
		init_io( i_EE,CTCAN,"   EE",0x10,NoLight);
		init_io( i_SE,CTCAN,"   SE",0x10,NoLight);
		init_io( o_DO,CTCAN,"   DO",0x11,NoLight);
		init_io( o_HVD,CTCAN,"  HVD",0x11,NoLight);
		init_io( o_DC,CTCAN,"   DC",0x11,NoLight);
		init_io(o_NUD,CTCAN,"  NUD",0x11,NoLight);
	
		if (cons[rear] != 0)
		{
			init_io(i_DOLR,CTCAN," DOLR",0x10,NoLight);
			init_io(i_DCLR,CTCAN," DCLR",0x10,NoLight);
			init_io( i_RPM,CTCAN,"  RPM",0x10,NoLight);
			init_io( i_EER,CTCAN,"  EER",0x10,NoLight);
			init_io( i_SER,CTCAN,"  SER",0x10,NoLight);
			init_io( o_DOR,CTCAN,"  DOR",0x11,NoLight);
			init_io( o_DCR,CTCAN,"  DCR",0x11,NoLight);
			init_io(o_NUDR,CTCAN," NUDR",0x11,NoLight);
		}
		if (cons[heavy_door] == 1)
		{
			
			init_io(o_HVD,CTCAN,"  HVD",0x11,NoLight);
			if (cons[rear] != 0) 
			{
				init_io(o_HVDR,CTCAN," HVDR",0x11,NoLight);
			}
		}
	}
	
	for (i=cons[bottomf];i<=cons[topf];i++)
	{
		if (ccnmsk[cons[carnmb]][i] == 1)
		{
			io_name[0] = ' ';
			io_name[1] = ' ';
			io_name[2] = flmrk[i][0];
			io_name[3] = flmrk[i][1];
			io_name[4] = 'C';
			io_name[5] = 'A';
			init_io(  i_CC(i),CTCAN,&io_name[0],0x10,OnLight);
			init_io( o_CCL(i),CTCAN,&io_name[1],0x01,OnLight);
		}		
	}
	
	if ((cons[aux_COP] & 0x03) != 0)
	{
			
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if (ccnmsk[cons[carnmb]][i] == 1)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'C';
				io_name[4] = 'X';
				io_name[5] = 'A';
				init_io(  i_XCC(i),CTCAN,&io_name[0],0x10,OnLight);
				init_io( o_XCCL(i),CTCAN,&io_name[1],0x01,OnLight);
			}		
		}
	}
	
 	if (((cons[Car_Elev_off] & 1) != 0))
   	{
		init_io(i_CEOF,CTCAN," CEOF",0x10,OnLight);
	   	if (((cons[Car_Elev_off] & 2) != 0))
			init_io(i_CEOFR,CTCAN,"CEOFR",0x10,OnLight);
	   	if (((cons[Car_Elev_off] & 4) != 0))
			init_io(o_CEOFL,CTCAN,"CEOFL",0x11,OnLight);
   	}
	
	// Rear Car Calls
	if (cons[rear] != 0)
	{
		
		for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
		{
			if (rccnmsk[cons[carnmb]][i] == 1)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'C';
				io_name[4] = 'R';
				io_name[5] = 'A';
				
				init_io(  i_CCR(i),CTCAN,&io_name[0],0x10,OnLight);
				init_io( o_CCRL(i),CTCAN,&io_name[1],0x01,OnLight);
			}
				
		}
		
		init_io(  i_DCBR,CTCAN," DCBR",0x10,OnLight);
		init_io(  i_DOBR,CTCAN," DOBR",0x10,OnLight);

		init_io(i_ALRMR,CTCAN," ALMR",0x10,OnLight);
		init_io(i_EDR,CTCAN,"  EDR",0x10,OnLight);
		init_io(  i_RDET,CTCAN," DETR",0x10,NoLight);
		if ((cons[hben] & 0x01) != 0)
			init_io(i_HBER,CTCAN," HBER",0x10,OnLight);

		
		if (cons[RGB_CC] == 1)
		{
			init_io(i_FiDCBR,CTCAN,"FDCBR",0x10,OnLight);
			init_io(i_FiDOBR,CTCAN,"FDOBR",0x10,OnLight);
		}

		if ((cons[aux_COP] & 0x05) != 0)
		{
			for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
			{
				if (rccnmsk[cons[carnmb]][i] == 1)
				{
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'C';
					io_name[4] = 'Z';
					io_name[5] = 'A';
				
					
					init_io(  i_XCCR(i),CTCAN,&io_name[0],0x10,OnLight);
					init_io( o_XCCRL(i),CTCAN,&io_name[1],0x01,OnLight);
				}
				
			}
			init_io(  i_XDCBR,CTCAN,"DCBRX",0x10,OnLight);
			init_io(  i_XDOBR,CTCAN,"DOBRX",0x10,OnLight);
			init_io(i_XALRMR,CTCAN,"ALMRX",0x10,OnLight);
			init_io(i_XEDR,CTCAN," EDRX",0x10,OnLight);
			if ((cons[hben] & 0x01) != 0)
				init_io(i_XHBER,CTCAN,"HBERX",0x10,OnLight);
		}
	}
		
	

	//***************************************************
	// place all i/o's that can be placed on an RGB board
	//***************************************************
	
	
	init_io(   i_DCB,CTCAN,"  DCB",0x10,OnLight);
	init_io(   i_DOB,CTCAN,"  DOB",0x10,OnLight);

	if ((cons[aux_COP] & 0x03) != 0)
	{
		init_io(   i_XDCB,CTCAN," DCBX",0x10,OnLight);
		init_io(   i_XDOB,CTCAN," DOBX",0x10,OnLight);
	}

	// fire Service
	
	init_io(  i_FS2C,CTCAN," FS2C",0x10,OnLight);

	init_io(o_FL,CTCAN,"   FL",0x11,FireLight);

	if (cons[F1_ML_AL] == 1)
	{
		init_io(o_F1AL,CTCAN," F1AL",0x11,FireLight);
		init_io(o_F1ML,CTCAN," F1ML",0x11,FireLight);
	}
	
	
	if ((cons[hben] & 0x01) != 0)
	{
		init_io(i_HBE,CTCAN,"  HBE",0x10,OnLight);
		if ((cons[aux_COP] & 0x03) != 0)
			init_io(i_XHBE,CTCAN," HBEX",0x10,OnLight);
	}


	init_io(i_ALRM,CTCAN,"  ALM",0x10,OnLight);
	if ((cons[aux_COP] & 0x03) != 0)
		init_io(i_XALRM,CTCAN," ALMX",0x10,OnLight);
	
	init_io(i_ED,CTCAN,"   ED",0x10,OnLight);
	if ((cons[aux_COP] & 0x03) != 0)
	{
		init_io(i_XINDC,CTCAN,"INDCX",0x10,OnLight);
		init_io(i_XED,CTCAN,"  EDX",0x10,OnLight);
	}

	if (cons[RGB_CC] == 1)
	{
		init_io(i_ICIU,CTCAN," ICIU",0x10,OnLight);
		init_io(i_ICID,CTCAN," ICID",0x10,OnLight);
		init_io(i_FiDCB,CTCAN,"FiDCB",0x10,OnLight); 
		init_io(i_FiDOB,CTCAN,"FiDOB",0x10,OnLight);
		init_io(i_FiFS2C,CTCAN,"FiS2C",0x10,OnLight);
		init_io(o_FiFL,CTCAN," FiFL",0x11,FireLight);
	}
	
	if ((cons[att] != 0) || ((cons[Car_Sw] & 1) != 0))
	{
		init_io(i_ATT,CTCAN,"  ATT",0x10,NoLight);
		if (cons[att] != 0)
		{
			init_io(i_ATTUP,CTCAN,"  ATU",0x10,OnLight);
			init_io(i_ATTDN,CTCAN,"  ATD",0x10,OnLight);
			init_io(i_ATTBY,CTCAN,"  ATB",0x10,OnLight);
			init_io(i_ATTHC,CTCAN,"ATTHC",0x10,OnLight);
			init_io(i_ATHCD,CTCAN,"ATHCD",0x10,OnLight);
			init_io(o_ATDL,CTCAN," ATDL",0x11,AttDnLight);
			init_io(o_ATUL,CTCAN," ATUL",0x11,AttUpLight);
		}
		else if ((cons[Car_Sw] & 1) != 0)
		{
			init_io(i_ATTUP,CTCAN,"  ATU",0x10,NoLight);
			init_io(i_ATTDN,CTCAN,"  ATD",0x10,NoLight);
			init_io(i_ATTBY,CTCAN,"  ATB",0x10,NoLight);
			init_io(i_ATTHC,CTCAN,"ATTHC",0x10,NoLight);
		}
		if ((cons[att] == 1) || (cons[att] == 2) || ((cons[Car_Sw] & 1) != 0))
		{
	  		for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if ((dcnmsk[cons[carnmb]][i] == 1) || ((irdcmsk[cons[carnmb]][i] == 1) && (cons[Second_Riser] != 0)))
				{
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'A';
					io_name[4] = 'D';
					init_io(o_DAL(i),CTCAN,&io_name[0],0x11,NoLight);
				}
				if ((ucnmsk[cons[carnmb]][i] == 1) || ((irucmsk[cons[carnmb]][i] == 1) && (cons[Second_Riser] != 0)))
				{
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'A';
					io_name[4] = 'U';
					init_io(o_UAL(i),CTCAN,&io_name[0],0x11,NoLight);
				}
			}
			
			if (cons[rear] != 0)
			{
				for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
				{
					if ((rdcnmsk[cons[carnmb]][i] == 1) || ((rirdcmsk[cons[carnmb]][i] == 1) && (cons[Second_Riser] != 0)))
					{
						io_name[0] = flmrk[i][0];
						io_name[1] = flmrk[i][1];
						io_name[2] = 'A';
						io_name[3] = 'D';
						io_name[4] = 'R';
						init_io(o_DALR(i),CTCAN,&io_name[0],0x11,NoLight);
					}
					if ((rucnmsk[cons[carnmb]][i] == 1) || ((rirucmsk[cons[carnmb]][i] == 1) && (cons[Second_Riser] != 0)))
					{
						io_name[0] = flmrk[i][0];
						io_name[1] = flmrk[i][1];
						io_name[2] = 'A';
						io_name[3] = 'U';
						io_name[4] = 'R';
						init_io(o_UALR(i),CTCAN,&io_name[0],0x11,NoLight);
					}
				}
			}
		}
	}

	if ((cons[loadw] & 1) != 0)
	{
		init_io(o_OLL,CTCAN,"  OLL",0x11,OTSLight);
		init_io(i_LWA,CTCAN,"  LWA",0x10,NoLight);
		init_io(i_LWB,CTCAN,"  LWB",0x10,NoLight);
		init_io(i_LWD,CTCAN,"  LWD",0x10,NoLight);
		init_io(i_OVL,CTCAN,"  OVL",0x10,NoLight);
	}
	else
	{
		if ((cons[loadw] & 2) != 0)
	 		init_io(o_OLL,CTCAN,"  OLL",0x11,OTSLight);
		
		if ((cons[loadw] & 4) != 0)
			init_io(i_LWB,CTCAN,"  LWB",0x10,NoLight);
	}

	if (cons[medEm] == 1) 
	{		// Medical Emergency Service
		init_io(o_EML,CTCAN,"  EML",0x11,MedLight);
	}
	else if (cons[cblue] == 1)
	{
		init_io(o_EML,CTCAN,"  CBL",0x11,MedLight);
	}


	if (cons[exDHwL] == 1) 
	{		// Extended door hold light   
		init_io(o_EDL,CTCAN,"  EDL",0x11,OTSLight);
	}

	if (cons[equake] == 1) 
	{		// add earthquake light on car
		init_io(o_EQLC,CTCAN," EQLC",0x11,EmLight);
	}


	//*********************************************************
	// place all i/o's that can be placed on an Standard board
	//*********************************************************


	if ((cons[FS2_SW] == 1) || (cons[Australia] == 1))
		init_io(i_FS2,CTCAN,"FS2OF",0x10,NoLight);
	else
		init_io(i_FS2,CTCAN,"  FS2",0x10,NoLight);

	if (cons[Australia] == 1)
		init_io(i_FS2H,CTCAN," FS2S",0x10,NoLight);
	else
		init_io(i_FS2H,CTCAN," FS2H",0x10,NoLight);
	
	init_io(o_FBNB,CTCAN," FBNB",0x11,NoLight);
	init_io(o_HB,CTCAN,"   HB",0x11,NoLight);
	init_io(i_INDC,CTCAN," INDC",0x10,NoLight);
	

	if (cons[rear] != 0)
	{
		init_io(o_CULR,CTCAN," CULR",0x11,NoLight);
		init_io(o_CDLR,CTCAN," CDLR",0x11,NoLight);
	}


	if ((cons[F1_CT] == 1) && ((cons[sercom] & 0x04) != 0))
	{
		init_io(o_FLCT,CTCAN," FLCT",0x11,NoLight);
		init_io(o_FBNBCT,CTCAN," FBCT",0x11,NoLight);
	}
	
	if ((cons[servOUT] & 0x01) != 0)
		init_io(o_SERVC,CTCAN,"SERVC",0x11,NoLight);
	
	if (cons[incarvip] == 1)
		init_io(i_ICVIP,CTCAN,"ICVIP",0x10,NoLight);


	if (cons[medEm] == 1) 
	{		// Medical Emergency Service
		init_io(i_EMS,CTCAN,"  EMS",0x10,NoLight);
	}

	if (cons[cblue] == 1)
	{
		init_io(i_EMS,CTCAN,"  HSV",0x10,NoLight);
	}

	if ((cons[Elev_off] & 1) != 0)
	{
		init_io(i_ELOF,CTCAN," ELOF",0x10,NoLight);
	}


	if (cons[LW_dis_sw] == 1) 
	{	 
		 // Disable Loadweigher switch
		init_io(i_DLW,CTCAN,"  DLW",0x10,NoLight);
	}

	if (cons[IndSec] == 1) 
	{
		init_io(i_INDS,CTCAN," INDS",0x10,NoLight);
		init_io(i_IDSO,CTCAN," INDO",0x10,NoLight);
	}
	
	if(((cons[dflt_MOVFR] & 0x02) != 0) && ((cons[sercom] & 0x10) != 0))
	{
		init_io(o_std_DO,CTCAN,"  xDO",0x11,NoLight);
		init_io(o_std_DC,CTCAN,"  xDC",0x11,NoLight);
		init_io(o_std_NUD,CTCAN," xNUD",0x11,NoLight);
		if (cons[rear] != 0)
		{
			init_io(o_std_DOR,CTCAN," xDOR",0x11,NoLight);
			init_io(o_std_DCR,CTCAN," xDCR",0x11,NoLight);
			init_io(o_std_NUDR,CTCAN,"xNUDR",0x11,NoLight);
		}
	}

	
	if (cons[vshort_tfl] != 0)
	{
		init_io(o_SFST,CTCAN," SFST",0x11,NoLight);	// Short Floor Selector Transfer
	}
	
	if ((cons[dispis] & 0x01) != 0)
	{	// Discrete PI on the car
		init_io(o_UDAC,CTCAN," UDAC",0x11,NoLight);
		init_io(o_DDAC,CTCAN," DDAC",0x11,NoLight);
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if (valid_fl[i] != 0)
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'P';
				io_name[3] = 'I';
				io_name[4] = 'C';
				init_io(o_PIC(i),CTCAN,&io_name[0],0x11,NoLight);
			}
		}
	}
	else if ((cons[dispis] & 0x80) != 0)
	{			// binary PI's on the car
		init_io(o_UDAC,CTCAN," UDAC",0x11,NoLight);
		init_io(o_DDAC,CTCAN," DDAC",0x11,NoLight);
		init_io(o_DPI1C,CTCAN,"DPIC1",0x11,NoLight);
		init_io(o_DPI2C,CTCAN,"DPIC2",0x11,NoLight);
		if (cons[topf] > 3)
			init_io(o_DPI3C,CTCAN,"DPIC3",0x11,NoLight);
		if (cons[topf] > 7)
			init_io(o_DPI4C,CTCAN,"DPIC4",0x11,NoLight);
		if (cons[topf] > 15)
			init_io(o_DPI5C,CTCAN,"DPIC5",0x11,NoLight);
		if (cons[topf] > 31)
			init_io(o_DPI6C,CTCAN,"DPIC6",0x11,NoLight);
	}

	if((Car_Call_Lockouts == 1) && ((cons[CCS_BUS] & 0x07) != 0))
	{
		if(((cons[CCS_BUS] & 0x02) != 0) || ((cons[CCS_BUS] & 0x01) != 0))
		{//do enable
			init_io(i_SECUR,CTCAN,"SECUR",0x10,NoLight);
		}		
		if(((cons[CCS_BUS] & 0x04) != 0) || ((cons[CCS_BUS] & 0x01) != 0))
		{//do inputs
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if (ccmsk[cons[carnmb]][i] == 1)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'C';
					io_name[3] = 'C';
					io_name[4] = 'S';
					init_io(i_CCS(i),CTCAN,io_name,0x10,NoLight);
				}
			}
		}	
		
		if (cons[rear] != 0)
		{
			if(((cons[CCS_BUS] & 0x02) != 0) || ((cons[CCS_BUS] & 0x01) != 0))
			{//do enable
				init_io(i_RSECU,CTCAN,"RSECU",0x10,NoLight);
			}			
			if(((cons[CCS_BUS] & 0x04) != 0) || ((cons[CCS_BUS] & 0x01) != 0))
			{//do inputs
				for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
				{
					if (rccmsk[cons[carnmb]][i] == 1)
					{
						io_name[0] = flmrk[i][0];
						io_name[1] = flmrk[i][1];
						io_name[2] = 'R';
						io_name[3] = 'C';
						io_name[4] = 'S';
						
						init_io(i_RCCS(i),CTCAN,io_name,0x10,NoLight);
					}
				}
			}					
		}
	}	
	
	if((Lockout_Override == 1) && ((cons[CCS_BUS] & 0x07) != 0))
	{
		if(((cons[CCS_BUS] & 0x02) != 0) || ((cons[CCS_BUS] & 0x01) != 0))
		{//do enable
			init_io(i_OSEC,CTCAN," OSEC",0x10,NoLight);
		}
		if(((cons[CCS_BUS] & 0x04) != 0) || ((cons[CCS_BUS] & 0x01) != 0))
		{//do inputs	
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if (ccmsk[cons[carnmb]][i] == 1)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'O';
					io_name[3] = 'C';
					io_name[4] = 'S';
					init_io(i_OCS(i),CTCAN,io_name,0x10,NoLight);
				}
			}
		}		
		if (cons[rear] != 0)
		{
			if(((cons[CCS_BUS] & 0x02) != 0) || ((cons[CCS_BUS] & 0x01) != 0))
			{//do enable
				init_io(i_ORSEC,CTCAN,"ORSEC",0x10,NoLight);
			}
			if(((cons[CCS_BUS] & 0x04) != 0) || ((cons[CCS_BUS] & 0x01) != 0))
			{//do inputs	
				for (i=bot_rfl;i<=top_rfl[cons[carnmb]];i++)
				{
					if (rccmsk[cons[carnmb]][i] == 1)
					{
						io_name[0] = flmrk[i][0];
						io_name[1] = flmrk[i][1];
						io_name[2] = 'O';
						io_name[3] = 'R';
						io_name[4] = 'S';
						init_io(i_ORS(i),CTCAN,io_name,0x10,NoLight);
					}
				}
			}
		}
	}

	if (cons[DZDoorEn] == 1)
	{
		init_io(o_DZDE,CTCAN," DZDE",0x11,NoLight);
	}

	if (cons[zoneSW] == 1)
	{
		init_io(i_ZON,CTCAN," ZON1",0x11,NoLight);	// Need zones for number of floors
	}


	init_io(i_CSP1,CTCAN,"CSPI1",0x10,NoLight);
	init_io(i_CSP2,CTCAN,"CSPI2",0x10,NoLight);
	init_io(i_CSP3,CTCAN,"CSPI3",0x10,NoLight);
	init_io(o_CSP1,CTCAN,"CSPO1",0x11,NoLight);
	init_io(o_CSP2,CTCAN,"CSPO2",0x11,NoLight);
	init_io(o_CSP3,CTCAN,"CSPO3",0x11,NoLight);
	
	
	
/*
//ZYM 10/16/2017
	// type = 0x01 for output, 0x10 for send name
	//CAN Add 12, dip switch setting 2
	//input
	init_io(i_CTi0,CTCAN," CTi0",0x10,NoLight);
	init_io(i_CTi1,CTCAN," CTi1",0x10,NoLight);
	init_io(i_CTi2,CTCAN," CTi2",0x10,NoLight);
	init_io(i_CTi3,CTCAN," CTi3",0x10,NoLight);
	init_io(i_CTi4,CTCAN," CTi4",0x10,NoLight);
	init_io(i_CTi5,CTCAN," CTi5",0x10,NoLight);
	init_io(i_CTi6,CTCAN," CTi6",0x10,NoLight);
	init_io(i_CTi7,CTCAN," CTi7",0x10,NoLight);
	init_io(i_CTi8,CTCAN," CTi8",0x10,NoLight);
	init_io(i_CTi9,CTCAN," CTi9",0x10,NoLight);
	init_io(i_CTi10,CTCAN,"CTi10",0x10,NoLight);
	init_io(i_CTi11,CTCAN,"CTi11",0x10,NoLight);	

	//output
	init_io(o_CTo0,CTCAN," CTo0",0x01,NoLight);
	init_io(o_CTo1,CTCAN," CTo1",0x01,NoLight);
	init_io(o_CTo2,CTCAN," CTo2",0x01,NoLight);
	init_io(o_CTo3,CTCAN," CTo3",0x01,NoLight);
	init_io(o_CTo4,CTCAN," CTo4",0x01,NoLight);
	init_io(o_CTo5,CTCAN," CTo5",0x01,NoLight);
	init_io(o_CTo6,CTCAN," CTo6",0x01,NoLight);
	init_io(o_CTo7,CTCAN," CTo7",0x01,NoLight);
	init_io(o_CTo8,CTCAN," CTo8",0x01,NoLight);
	init_io(o_CTo9,CTCAN," CTo9",0x01,NoLight);
	init_io(o_CTo10,CTCAN,"CTo10",0x01,NoLight);
	init_io(o_CTo11,CTCAN,"CTo11",0x01,NoLight);
*/	
	
	// type = 0x01 for output, 0x10 for send name
	// GALX-1134 COP Interface
	// 0x0xz ==> x = (1:display on; 0:no display)
//               z = (1:output;     0:input;)	
	//input
	init_io(i_RLS_COPi0,CTCAN,"  RLS",0x00,NoLight);
//	init_io(i_CSS_COPi1,CTCAN," iCSS",0x00,NoLight);
	init_io(i_CTS_COPi2,CTCAN,"  CTS",0x00,NoLight);
	init_io(i_DET_COPi3,CTCAN,"  DET",0x00,NoLight);
	
	//OUTPUT
	init_io(O_IFL_COPo0,CTCAN,"  IFL",0x01,NoLight);
	init_io(O_IFB_COPo1,CTCAN,"  IFB",0x01,NoLight);
	init_io(O_CUL_COPo2,CTCAN,"  CUL",0x01,NoLight);
	init_io(O_CDL_COPo3,CTCAN,"  CDL",0x01,NoLight);
	init_io(O_FAN_COPo4,CTCAN,"  FAN",0x01,NoLight);
	init_io(O_LIG_COPo5,CTCAN,"  LIG",0x01,NoLight);	
	
// input	
	init_io(i_CTi0,CTCAN," CTi0",0x10,NoLight);
	init_io(i_CTi1,CTCAN," CTi1",0x10,NoLight);
	init_io(i_CTi2,CTCAN," CTi2",0x10,NoLight);
	init_io(i_CTi3,CTCAN," CTi3",0x10,NoLight);
	init_io(i_CTi4,CTCAN," CTi4",0x10,NoLight);
	init_io(i_CTi5,CTCAN," CTi5",0x10,NoLight);
	
	init_io(i_CTi6,CTCAN," CTi6",0x10,NoLight);
	init_io(i_CTi7,CTCAN," CTi7",0x10,NoLight);
	init_io(i_CTi8,CTCAN," CTi8",0x10,NoLight);
	init_io(i_CTi9,CTCAN," CTi9",0x10,NoLight);
	init_io(i_CTi10,CTCAN,"CTi10",0x10,NoLight);
	init_io(i_CTi11,CTCAN,"CTi11",0x10,NoLight);	
/*
	//output
	init_io(o_CTo0,CTCAN," CTo0",0x01,NoLight); // since this is "0x01", the label is not displayed
	init_io(o_CTo1,CTCAN," CTo1",0x01,NoLight); // and these 6 items are not displayed
	init_io(o_CTo2,CTCAN," CTo2",0x01,NoLight);
	init_io(o_CTo3,CTCAN," CTo3",0x01,NoLight);
	init_io(o_CTo4,CTCAN," CTo4",0x01,NoLight);
	init_io(o_CTo5,CTCAN," CTo5",0x01,NoLight);
*/	
	//output
	init_io(o_CTo0,CTCAN," Dbg0", 0x11,NoLight); 
	init_io(o_CTo1,CTCAN," Dbg1", 0x11,NoLight); 
	init_io(o_CTo2,CTCAN," Dbg2", 0x11,NoLight);
	init_io(o_CTo3,CTCAN," Dbg3", 0x11,NoLight);
	init_io(o_CTo4,CTCAN," Dbg4", 0x11,NoLight);
	init_io(o_CTo5,CTCAN," Dbg5", 0x11,NoLight);
	init_io(o_CTo6,CTCAN," Dbg6", 0x11,NoLight); 
	init_io(o_CTo7,CTCAN," Dbg7", 0x11,NoLight);
	init_io(o_CTo8,CTCAN," Dbg8", 0x11,NoLight);
	init_io(o_CTo9,CTCAN," Dbg9", 0x11,NoLight);
	init_io(o_CTo10,CTCAN,"Dbg10",0x11,NoLight);
	init_io(o_CTo11,CTCAN,"Dbg11",0x11,NoLight);
		
//ZYM 10/18/2017
	//CAN Add 12, dip switch setting 2
/* //-	
	//input
	init_io(i_MLi0,MRCAN," MLi0",0x10,NoLight);
	init_io(i_MLi1,MRCAN," MLi1",0x10,NoLight);
	init_io(i_MLi2,MRCAN," MLi2",0x10,NoLight);
	init_io(i_MLi3,MRCAN," MLi3",0x10,NoLight);
	init_io(i_MLi4,MRCAN," MLi4",0x10,NoLight);
	init_io(i_MLi5,MRCAN," MLi5",0x10,NoLight);
	init_io(i_MLi6,MRCAN," MLi6",0x10,NoLight);
	init_io(i_MLi7,MRCAN," MLi7",0x10,NoLight);
	init_io(i_MLi8,MRCAN," MLi8",0x10,NoLight);
	init_io(i_MLi9,MRCAN," MLi9",0x10,NoLight);
	init_io(i_MLi10,MRCAN,"MLi10",0x10,NoLight);
	init_io(i_MLi11,MRCAN,"MLi11",0x10,NoLight);	

	//output
	init_io(o_MLo0,MRCAN," MLo0",0x01,NoLight);
	init_io(o_MLo1,MRCAN," MLo1",0x01,NoLight);
	init_io(o_MLo2,MRCAN," MLo2",0x01,NoLight);
	init_io(o_MLo3,MRCAN," MLo3",0x01,NoLight);
	init_io(o_MLo4,MRCAN," MLo4",0x01,NoLight);
	init_io(o_MLo5,MRCAN," MLo5",0x01,NoLight);
	init_io(o_MLo6,MRCAN," MLo6",0x01,NoLight);
	init_io(o_MLo7,MRCAN," MLo7",0x01,NoLight);
	init_io(o_MLo8,MRCAN," MLo8",0x01,NoLight);
	init_io(o_MLo9,MRCAN," MLo9",0x01,NoLight);
	init_io(o_MLo10,MRCAN,"MLo10",0x01,NoLight);
	init_io(o_MLo11,MRCAN,"MLo11",0x01,NoLight);
*/	

    //CAN Add 12, dip switch setting 3
	//input
	// 0x0xz ==> x = (1:display on; 0:no display)
//               z = (1:output;     0:input;)	             
	init_io(i_MRi0,MRCAN," MRi0",0x10,NoLight);
	init_io(i_MRi1,MRCAN," MRi1",0x10,NoLight);
	init_io(i_MRi2,MRCAN," MRi2",0x10,NoLight);
	init_io(i_MRi3,MRCAN," MRi3",0x10,NoLight);
	init_io(i_MRi4,MRCAN," MRi4",0x10,NoLight);
	init_io(i_MRi5,MRCAN," MRi5",0x10,NoLight);
/*	
	init_io(i_MRi6,MRCAN," MRi6",0x10,NoLight);
	init_io(i_MRi7,MRCAN," MRi7",0x10,NoLight);
	init_io(i_MRi8,MRCAN," MRi8",0x10,NoLight);
	init_io(i_MRi9,MRCAN," MRi9",0x10,NoLight);
	init_io(i_MRi10,MRCAN,"MRi10",0x10,NoLight);
	init_io(i_MRi11,MRCAN,"MRi11",0x10,NoLight);	
*/
	//output
/*	
	init_io(o_MRo0,MRCAN," MRo0",0x01,NoLight);
	init_io(o_MRo1,MRCAN," MRo1",0x01,NoLight);
	init_io(o_MRo2,MRCAN," MRo2",0x01,NoLight);
	init_io(o_MRo3,MRCAN," MRo3",0x01,NoLight);
	init_io(o_MRo4,MRCAN," MRo4",0x01,NoLight);
	init_io(o_MRo5,MRCAN," MRo5",0x01,NoLight);
*/	
	init_io(o_MRo6,MRCAN," MRo6",0x11,NoLight);
	init_io(o_MRo7,MRCAN," MRo7",0x11,NoLight);
	init_io(o_MRo8,MRCAN," MRo8",0x11,NoLight);
	init_io(o_MRo9,MRCAN," MRo9",0x11,NoLight);
	init_io(o_MRo10,MRCAN,"MRo10",0x11,NoLight);
	init_io(o_MRo11,MRCAN,"MRo11",0x11,NoLight);


/* //-
//ZYM 11/20/2017
    //CAN Add 14, dip switch setting 4
	//input
	init_io(i_suf,MRCAN,"  suf",0x10,NoLight);
	init_io(i_sufc,MRCAN," sufc",0x10,NoLight);
	init_io(i_su,MRCAN,"   su",0x10,NoLight);
	init_io(i_suc,MRCAN,"  suc",0x10,NoLight);
	init_io(i_sdf,MRCAN,"  sdf",0x10,NoLight);
	init_io(i_sdfc,MRCAN," sdfc",0x10,NoLight);
	init_io(i_sd,MRCAN,"   sd",0x10,NoLight);
	init_io(i_sdc,MRCAN,"  sdc",0x10,NoLight);
	init_io(i_s6s10,MRCAN,"s6s10",0x10,NoLight);

	//output
	init_io(o_S5LPS,MRCAN,"S5LPS",0x11,NoLight);
	init_io(o_S4LOS,MRCAN,"S4LOS",0x11,NoLight);
	init_io(o_S3TPL,MRCAN,"S3TPL",0x11,NoLight);
     
	
	//CAN Add 15, dip switch setting 5
	//input
	init_io(i_s2mrw,MRCAN,"s2mrw",0x10,NoLight);
	init_io(i_24s10,MRCAN,"24s10",0x10,NoLight);
	init_io(i_22gov,MRCAN,"22gov",0x10,NoLight);
	init_io(i_20tlh,MRCAN,"20tlh",0x10,NoLight);
	init_io(i_h18ps,MRCAN,"h18ps",0x10,NoLight);
	init_io(i_16mrw,MRCAN,"16mrw",0x10,NoLight);

	//output
	init_io(o_S1TPH,MRCAN,"S1TPH",0x11,NoLight);
	init_io(o_23GOV,MRCAN,"23GOV",0x11,NoLight);
	init_io(o_21TLH,MRCAN,"21TLH",0x11,NoLight);
	init_io(o_H19PS,MRCAN,"H19PS",0x11,NoLight);
	init_io(o_17HSS,MRCAN,"17HSS",0x11,NoLight);
	
	
	//CAN Add 16, dip switch setting 6
	//input
	init_io(i_14mrw,MRCAN,"14mrw",0x10,NoLight);
	init_io(i_11mrw,MRCAN,"11mrw",0x10,NoLight);
	init_io(i_h8s10,MRCAN,"h8s10",0x10,NoLight);
	init_io(i_h6s10,MRCAN,"h6s10",0x10,NoLight);
	init_io(i_h4s10,MRCAN,"h4s10",0x10,NoLight);

	//output
	init_io(o_15TSD,MRCAN,"15TSD",0x11,NoLight);
	init_io(o_13BAU,MRCAN,"13BAU",0x11,NoLight);
	init_io(o_12BAD,MRCAN,"12BAD",0x11,NoLight);
	init_io(o_10TAU,MRCAN,"10TAU",0x11,NoLight);
	init_io(o_H9TAD,MRCAN,"H9TAD",0x11,NoLight);
	init_io(o_H7DLB,MRCAN,"H7DLB",0x11,NoLight);
	init_io(o_H5DLM,MRCAN,"H5DLM",0x11,NoLight);
	
	
	//CAN Add 17, dip switch setting 7
	//input
	init_io(i_h2s10,MRCAN,"h2s10",0x10,NoLight);
	init_io(i_mrsw,MRCAN," mrsw",0x10,NoLight);
	init_io(i_mc,MRCAN,"   mc",0x10,NoLight);
	init_io(i_mc1,MRCAN,"  mc1",0x10,NoLight);
	init_io(i_ste,MRCAN,"  ste",0x10,NoLight);
	init_io(i_s10,MRCAN,"  s10",0x10,NoLight);

	//output
	init_io(o_H3DLT,MRCAN,"H3DLT",0x11,NoLight);
	init_io(o_RLM,MRCAN,"  RLM",0x11,NoLight);
//-	init_io(o_MC1,MRCAN,"  MC1",0x11,NoLight);
	init_io(o_0MTO,MRCAN," 0MTO",0x11,NoLight);
	init_io(o_0SPD,MRCAN," 0SPD",0x11,NoLight);
	init_io(o_0CFLT,MRCAN,"0CFLT",0x11,NoLight);
	init_io(o_TMRLY,MRCAN,"TMRLY",0x11,NoLight);
	
	
	//CAN Add 18, dip switch setting 8
	//input
	init_io(i_18hss,MRCAN,"18hss",0x10,NoLight);	

	//output
	init_io(o_17CST,MRCAN,"17CST",0x11,NoLight);
	init_io(o_16INS,MRCAN,"16INS",0x11,NoLight);
	init_io(o_T15IU,MRCAN,"T15IU",0x11,NoLight);
	init_io(o_T14ID,MRCAN,"T14ID",0x11,NoLight);
	init_io(o_13IEN,MRCAN,"13IEN",0x11,NoLight);
	init_io(o_12ICA,MRCAN,"12ICA",0x11,NoLight);
	init_io(o_11ICI,MRCAN,"11ICI",0x11,NoLight);
	init_io(o_10ACC,MRCAN,"10ACC",0x11,NoLight);
	init_io(o_T9GS,MRCAN," T9GS",0x11,NoLight);
	init_io(o_RGS,MRCAN,"  RGS",0x11,NoLight);
	init_io(o_RDRLY,MRCAN,"RDRLY",0x11,NoLight);
*/	
// ==============================================
// GALX-1106; SEB1: (CTCAN: 141, 142, 143)
// Dip switch = 2; CAN Addr 12
	// 0x0xz ==> x = (1:display on; 0:no display)
    //           z = (1:output;     0:input;)
    
    // output
    //init_io(O_CSS_ctcan,CTCAN,  "  CSS",0x11,NoLight);
    init_io(O_CSS_ctcan, CTCAN, " oCSS",0x11,NoLight);
    // input
    init_io(i_CTA_ctcan,  CTCAN, "  CTA",0x10,NoLight);
    init_io(i_INS_ctcan,  CTCAN, "  INS",0x10,NoLight);    
    init_io(i_IEN_ctcan,  CTCAN, "  IEN",0x10,NoLight);    
    init_io(i_IU_ctcan,   CTCAN, "   IU",0x10,NoLight);    
    init_io(i_ID_ctcan,   CTCAN, "   ID",0x10,NoLight);    
    init_io(i_FANST_ctcan,CTCAN, "FANST",0x10,NoLight);    
    init_io(i_LIGST_ctcan,CTCAN, "LIGST",0x10,NoLight);    
    init_io(i_CDL_T_ctcan,CTCAN, "CDL_T",0x10,NoLight);    
    init_io(i_CUL_T_ctcan,CTCAN, "CUL_T",0x10,NoLight);    
    init_io(i_CTS_T_ctcan,CTCAN, "CTS_T",0x10,NoLight);    
    init_io(i_FFSWT_ctcan,CTCAN, "FFSWT",0x10,NoLight);
    
// ==============================================
// GALX-1106; SEB2: (CTCAN: 144, 145, 146)
// Dip switch = 3; CAN Addr 13
	// 0x0xz ==> x = (1:display on; 0:no display)
    //           z = (1:output;     0:input;)
    
    // output
    init_io(O_C120T_ctcan,CTCAN,  "C120T",0x11,NoLight);
    init_io(O_L15RL_ctcan,CTCAN,  "L15RL",0x11,NoLight);
    init_io(O_L15_T_ctcan,CTCAN,  "L15_T",0x11,NoLight);
    init_io(O_FSS_ctcan,CTCAN,  "  FSS",0x11,NoLight);
    init_io(O_CST_ctcan,CTCAN,  "  CST",0x11,NoLight);
    init_io(O_ICC_ctcan,CTCAN,  "  ICC",0x11,NoLight);
    
    // input
    init_io(i_CST_T_ctcan,  CTCAN, "CST_T",0x10,NoLight);
    init_io(i_ICC_T_ctcan,  CTCAN, "ICC_T",0x10,NoLight);    
    init_io(i_ICA_T_ctcan,  CTCAN, "ICA_T",0x10,NoLight);    
    init_io(i_ICI_T_ctcan,   CTCAN, "ICI_T",0x10,NoLight);    
    init_io(i_ACC_T_ctcan,   CTCAN, "ACC_T",0x10,NoLight);        
    
// ==============================================
// GALX-1106; SEB4: (CTCAN: 147, 148, 149)
// Dip switch = 3; CAN Addr 14
// Locate in the CTB, not the test fixture.
	// 0x0xz ==> x = (1:display on; 0:no display)
    //           z = (1:output;     0:input;)
    
    // output
    init_io(O_DO_ctcan,CTCAN,  "DO",0x11,NoLight);
    init_io(O_L15_ctcan,CTCAN,  "L15",0x11,NoLight);
    init_io(O_L25_ctcan,CTCAN,  "L25",0x11,NoLight);
    
    // input
    init_io(i_EE_ctcan,  CTCAN, "EE",0x10,NoLight);
    init_io(i_DOL_ctcan,  CTCAN, "DOL",0x10,NoLight);    
    init_io(i_DCL_ctcan,  CTCAN, "DCL",0x10,NoLight);    
    init_io(i_SE_ctcan,   CTCAN, "SE",0x10,NoLight);    
    init_io(i_DC_ctcan,   CTCAN, "DC",0x10,NoLight);        
      
// ==============================================	

// ==============================================

// GALX-1106; SEB3: (CTCAN: 153, 154, 155)
// Dip switch = 6; CAN Addr 16
	// 0x0xz ==> x = (1:display on; 0:no display)
    //           z = (1:output;     0:input;)
    
    // output
    init_io(O_ICA_ctcan,CTCAN,  "  ICA",0x11,NoLight);
    init_io(O_ICI_ctcan,CTCAN,  "  ICI",0x11,NoLight);
    init_io(O_ACC_ctcan,CTCAN,  "  ACC",0x11,NoLight);
    init_io(O_BELL_ctcan,CTCAN, " BELL",0x11,NoLight);
    init_io(O_CLC_ctcan,CTCAN,  "  CLC",0x11,NoLight);
    
    // input
    init_io(i_L15_ctcan,  CTCAN, "  L15",0x10,NoLight);
    init_io(i_L25_ctcan,  CTCAN, "  L25",0x10,NoLight);    

    
// ==============================================
// 156, 157, 158 = CTCAN
// GALX-1106; SEB4: (CTCAN: 156, 157, 158)
// Dip switch = 7; CAN Addr 17
	// 0x0xz ==> x = (1:display on; 0:no display)
    //           z = (1:output;     0:input;)
    
    // output
    init_io(O_C24_ctcan,CTCAN,  "  C24",0x11,NoLight);
    
    // input
    init_io(i_C24_T_ctcan,  CTCAN, "C24_T",0x10,NoLight);       
    
// ==============================================
// (2)
    // 162, 163, 164 = CTCAN 
	// type = 0x01 for output, 0x10 for send name
	//CAN Add ??. dip switch setting 9
	// 0x0xz ==> x = (1:display on; 0:no display)
    //           z = (1:output;     0:input;)
   	
	// input
	init_io(i_SWi0,CTCAN," SWi0",0x10,NoLight);
	init_io(i_SWi1,CTCAN," SWi1",0x10,NoLight);
	init_io(i_SWi2,CTCAN," SWi2",0x10,NoLight);
	init_io(i_SWi3,CTCAN," SWi3",0x10,NoLight);
	init_io(i_SWi4,CTCAN," SWi4",0x10,NoLight);
	init_io(i_SWi5,CTCAN," SWi5",0x10,NoLight);	
	
	// output
	init_io(O_LDO6,CTCAN," LDO6",0x11,NoLight);
	init_io(O_LDO7,CTCAN," LDO7",0x11,NoLight);
	init_io(O_LDO8,CTCAN," LDO8",0x11,NoLight);
	init_io(O_LDO9,CTCAN," LDO9",0x11,NoLight);
	init_io(O_LDO10,CTCAN,"LDO10",0x11,NoLight);
	init_io(O_LDO11,CTCAN,"LDO11",0x11,NoLight);
	
// (2) wbg 2018-9-02 OPL-0106N
	// input
	init_io(i_ronc,CTCAN, " ronc",0x10,NoLight);
	init_io(i_rono,CTCAN, " rono",0x10,NoLight);
	init_io(i_dclnc,CTCAN,"dclnc",0x10,NoLight);
	init_io(i_dclno,CTCAN,"dclno",0x10,NoLight);
	init_io(i_dolnc,CTCAN,"dolnc",0x10,NoLight);
	init_io(i_dolno,CTCAN,"dolno",0x10,NoLight);	
	init_io(i_heavy,CTCAN,"heavy",0x10,NoLight);
	init_io(i_nudge,CTCAN,"nudge",0x10,NoLight);
	init_io(i_close,CTCAN,"close",0x10,NoLight);
	init_io(i_open,CTCAN, " open",0x10,NoLight);		
	
	// output
	init_io(O_CN19V,CTCAN,"CN19V",0x11,NoLight);
	init_io(O_CN18V,CTCAN,"CN18V",0x11,NoLight);
	init_io(O_CLOSE,CTCAN,"CLOSE",0x11,NoLight);
	init_io(O_OPEN, CTCAN," OPEN",0x11,NoLight);
	init_io(O_NUDGE,CTCAN,"NUDGE",0x11,NoLight);
	init_io(O_HEAVY,CTCAN,"HEAVY",0x11,NoLight);	
	init_io(O_DCL, CTCAN, "  DCL",0x11,NoLight);
	init_io(O_RO,CTCAN,   "   RO",0x11,NoLight);
	init_io(O_DOL,CTCAN,  "  DOL",0x11,NoLight);	

// wbg 2018-6-27
    //output
	init_io(O_dbg01,CTCAN,"dbg01",0x11,NoLight);  
	init_io(O_dbg02,CTCAN,"dbg02",0x11,NoLight);
	init_io(O_dbg03,CTCAN,"dbg03",0x11,NoLight);
	init_io(O_dbg04,CTCAN,"dbg04",0x11,NoLight);
	init_io(O_dbg05,CTCAN,"dbg05",0x11,NoLight);
	init_io(O_dbg06,CTCAN,"dbg06",0x11,NoLight);
	init_io(O_dbg07,CTCAN,"dbg07",0x11,NoLight);
	init_io(O_dbg08,CTCAN,"dbg08",0x11,NoLight);
	init_io(O_dbg09,CTCAN,"dbg09",0x11,NoLight);
	init_io(O_dbg10,CTCAN,"dbg10",0x11,NoLight);
	  

	
	
////////// =========================================
	
	gio_init();

//	set_io_board_status();

	init_iodata();	
	init_ioname_chksum();	
}


//********************************
// Initiate array with checksums 
// using for transfering io labels
//********************************

void init_ioname_chksum(void)
{
	unsigned char chksum;
	int i,j,k;
	int n;
	for(n=0; n<nmb_io_brds; n++)
	{
		chksum = 0;
		for(i=0;i<3;i++)
		{
			for(j=0;j<8;j++)
			{
				if((io_valid[3*n + i] & (unsigned char)(fl_mask[j+1]))!=0)
				{
					for(k=0;k<5;k++)
					{
						chksum += ioname[24*n + 8*i + j][k];
					}
					chksum += (8*i + j);
				}
				
			}
		}
	
		ionamechksum[n] = chksum;
		
		
	}
}




//******************************
// Group I/O
//******************************
void gio_init (void)
{
	int16 i;
	int16 fl_ix;
		
  	// type = 0x01 for output, 0x10 for send name, 0x80 for I/O not on serial hall board
	// type = 0x04 = ir, 0x08 = cb, 0x20 = vip
	// hc_io_board[] = 0x01 = hc board, 0x02 = Serial hc driver board, 0x04 = ir, 0x08 = cb, 0x20 = vip, 0x40 = Serial HC Board
 //ZYM 10/9/2017
/* 
 	//input
	init_gio(i_TESi0,GRCAN,"TESi0",0x90);
	init_gio(i_TESi1,GRCAN,"TESi1",0x90);
	init_gio(i_TESi2,GRCAN,"TESi2",0x90);
	init_gio(i_TESi3,GRCAN,"TESi3",0x90);
	init_gio(i_TESi4,GRCAN,"TESi4",0x90);
	init_gio(i_TESi5,GRCAN,"TESi5",0x90);
	init_gio(i_TESi6,GRCAN,"TESi6",0x90);
	init_gio(i_TESi7,GRCAN,"TESi7",0x90);
	init_gio(i_TESi8,GRCAN,"TESi8",0x90);
	init_gio(i_TESi9,GRCAN,"TESi9",0x90);
	init_gio(i_TEi10,GRCAN,"TEi10",0x90);
	init_gio(i_TEi11,GRCAN,"TEi11",0x90);
	
	//output
	init_gio(o_TESo0,GRCAN,"TESo0",0x81);
	init_gio(o_TESo1,GRCAN,"TESo1",0x81);
	init_gio(o_TESo2,GRCAN,"TESo2",0x81);
	init_gio(o_TESo3,GRCAN,"TESo3",0x81);
	init_gio(o_TESo4,GRCAN,"TESo4",0x81);
	init_gio(o_TESo5,GRCAN,"TESo5",0x81);
	init_gio(o_TESo6,GRCAN,"TESo6",0x81);
	init_gio(o_TESo7,GRCAN,"TESo7",0x81);
	init_gio(o_TESo8,GRCAN,"TESo8",0x81);
	init_gio(o_TESo9,GRCAN,"TESo9",0x81);
	init_gio(o_TEo10,GRCAN,"TEo10",0x81);
	init_gio(o_TEo11,GRCAN,"TEo11",0x81);
	init_gio(i_HC,GRCAN,"  HCP",0x10);
*/	
	
 //ZYM 10/9/2017
 	//CAN Add 12, dip switch setting 2
 	//input
	init_gio(i_GRi0,GRCAN," GRi0",0x90);
	init_gio(i_GRi1,GRCAN," GRi1",0x90);
	init_gio(i_GRi2,GRCAN," GRi2",0x90);
	init_gio(i_GRi3,GRCAN," GRi3",0x90);
	init_gio(i_GRi4,GRCAN," GRi4",0x90);
	init_gio(i_GRi5,GRCAN," GRi5",0x90);
	init_gio(i_GRi6,GRCAN," GRi6",0x90);
	init_gio(i_GRi7,GRCAN," GRi7",0x90);
	init_gio(i_GRi8,GRCAN," GRi8",0x90);
	init_gio(i_GRi9,GRCAN," GRi9",0x90);
	init_gio(i_GRi10,GRCAN,"GRi10",0x90);
	init_gio(i_GRi11,GRCAN,"GRi11",0x90);
	
	//output
	init_gio(o_GRo0,GRCAN," GRo0",0x81);
	init_gio(o_GRo1,GRCAN," GRo1",0x81);
	init_gio(o_GRo2,GRCAN," GRo2",0x81);
	init_gio(o_GRo3,GRCAN," GRo3",0x81);
	init_gio(o_GRo4,GRCAN," GRo4",0x81);
	init_gio(o_GRo5,GRCAN," GRo5",0x81);
	init_gio(o_GRo6,GRCAN," GRo6",0x81);
	init_gio(o_GRo7,GRCAN," GRo7",0x81);
	init_gio(o_GRo8,GRCAN," GRo8",0x81);
	init_gio(o_GRo9,GRCAN," GRo9",0x81);
	init_gio(o_GRo10,GRCAN,"GRo10",0x81);
	init_gio(o_GRo11,GRCAN,"GRo11",0x81);
		

	
	for(i=2;i<=cons[grtopf]; i++)
	{
		fl_ix = (i-1)/32;
		if ((g_dcmsk[fl_ix] & fl_mask[i]) != 0)
		{
			io_name[0] = ' ';
			io_name[1] = ' ';
			io_name[2] = flmrk[i][0];
			io_name[3] = flmrk[i][1];
			io_name[4] = 'D';
			io_name[5] = 'A';
			init_gio(i_DC(i),GRCAN,&io_name[0],0x10);
			init_gio(o_DCL(i),GRCAN,&io_name[1],0x01);
		}
	}															  
	for(i=1;i<cons[grtopf]; i++)
	{
		fl_ix = (i-1)/32;
		if ((g_ucmsk[fl_ix] & fl_mask[i]) != 0)
		{
			io_name[0] = ' ';
			io_name[1] = ' ';
			io_name[2] = flmrk[i][0];
			io_name[3] = flmrk[i][1];
			io_name[4] = 'U';
			io_name[5] = 'A';
			init_gio(i_UC(i),GRCAN,&io_name[0],0x10);
			init_gio(o_UCL(i),GRCAN,&io_name[1],0x01);
		}
	}

	if (grtop_rfl != 0)
	{
		
		for(i=grbot_rfl;i<=grtop_rfl; i++)
		{
			fl_ix = (i-1)/32;
			if (((g_rdcmsk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'D';
				io_name[4] = 'R';
				io_name[5] = 'A';
				init_gio(i_RDC(i),GRCAN,&io_name[0],0x10);
				init_gio(o_RDCL(i),GRCAN,&io_name[1],0x01);
			}
		}
		for(i=grbot_rfl;i<=grtop_rfl; i++)
		{
			fl_ix = (i-1)/32;
			if (((g_rucmsk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'U';
				io_name[4] = 'R';
				io_name[5] = 'A';
				init_gio(i_RUC(i),GRCAN,&io_name[0],0x10);
				init_gio(o_RUCL(i),GRCAN,&io_name[1],0x01);
			}
		}
	}
	
	if ((cons[incrs] != 0) || (cons[Second_Riser] != 0))
	{
		init_gio(i_ICR,GRCAN,"  ICR",0x90);
		init_gio(i_ICR2,GRCAN," ICR2",0x90);
		if (grtop_rfl != 0)
			init_gio(i_RICR,GRCAN," RICR",0x90);
		if (cons[Second_Riser] == 3)
		{
			init_gio(i_CYCLE,GRCAN,"CYCEN",0x90);
			init_gio(o_CYCLO,GRCAN,"CYCEL",0x91);
		}
		
		for(i=2;i<=cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_irdcmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'D';
				io_name[4] = 'I';
				io_name[5] = 'A';
				init_gio(i_IRDC(i),GRCAN,&io_name[0],0x10 | 0x04);
				init_gio(o_IRDCL(i),GRCAN,&io_name[1],0x01 | 0x04);
			}
		}
		for(i=1;i<cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_irucmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'U';
				io_name[4] = 'I';
				io_name[5] = 'A';
				init_gio(i_IRUC(i),GRCAN,&io_name[0],0x10 | 0x04);
				init_gio(o_IRUCL(i),GRCAN,&io_name[1],0x01 | 0x04);
			}
		}

		if (grtop_rfl != 0)
		{
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if (((g_rirdcmsk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'D';
					io_name[3] = 'I';
					io_name[4] = 'R';
					init_gio(i_RIRDC(i),GRCAN,&io_name[0],0x10 | 0x04);
					io_name[3] = 'R';
					io_name[4] = 'O';
					init_gio(o_RIRDCL(i),GRCAN,&io_name[0],0x01 | 0x04);
				}

				fl_ix = (i-1)/32;
				if (((g_rirucmsk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'U';
					io_name[3] = 'I';
					io_name[4] = 'R';
					init_gio(i_RIRUC(i),GRCAN,&io_name[0],0x10 | 0x04);
					io_name[3] = 'R';
					io_name[4] = 'O';
					init_gio(o_RIRUCL(i),GRCAN,&io_name[0],0x01 | 0x04);
				}
			}
		}
	}
	
	if (cons[Third_Riser] != 0)
	{
		init_gio(i_TR,GRCAN,"   TR",0x90);
		
		for(i=2;i<=cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_irdcmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'D';
				io_name[4] = 'T';
				io_name[5] = 'A';
				init_gio(i_TRDC(i),GRCAN,&io_name[0],0x10 | 0x04);
				init_gio(o_TRDCL(i),GRCAN,&io_name[1],0x01 | 0x04);
			}
		}
		for(i=1;i<cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_irucmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'U';
				io_name[4] = 'T';
				io_name[5] = 'A';
				init_gio(i_TRUC(i),GRCAN,&io_name[0],0x10 | 0x04);
				init_gio(o_TRUCL(i),GRCAN,&io_name[1],0x01 | 0x04);
			}
		}
	}
	
	if ((cons[CB_msk] != 0) || (cons[cblue] != 0))
	{
		for(i=1;i<=cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_cbmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'C';
				io_name[4] = 'B';
				io_name[5] = 'A';
				init_gio(i_CB(i),GRCAN,&io_name[0],0x10 | 0x08);
				init_gio(o_CBL(i),GRCAN,&io_name[1],0x01 | 0x08);
			}
		}
		if (grtop_rfl != 0)
		{
			for(i=1;i<=cons[grtopf]; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rcbmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'B';
					io_name[4] = 'R';
					io_name[5] = 'A';
					init_gio(i_RCB(i),GRCAN,&io_name[0],0x10 | 0x08);
					init_gio(o_RCBL(i),GRCAN,&io_name[1],0x01 | 0x08);
				}
			}
		}
	}

	if (cons[CB_SR] != 0)
	{
		for(i=1;i<=cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_cbmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'E';
				io_name[4] = 'M';
				io_name[5] = 'A';
				init_gio(i_CB_SR(i),GRCAN,&io_name[0],0x10 | 0x08);
				init_gio(o_CB_SRL(i),GRCAN,&io_name[1],0x01 | 0x08);
			}
		}
		if (grtop_rfl != 0)
		{
			for(i=1;i<=cons[grtopf]; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rcbmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'E';
					io_name[4] = 'R';
					io_name[5] = 'A';
					init_gio(i_RCB_SR(i),GRCAN,&io_name[0],0x10 | 0x08);
					init_gio(o_RCB_SRL(i),GRCAN,&io_name[1],0x01 | 0x08);
				}
			}
		}
	}

	if ((cons[VIP_msk] != 0) || (cons[VipSvc] != 0))
	{
		if(cons[vip_en] != 0)
		{
			
			init_gio(i_VIPEN,GRCAN,"VIPEN",0x90);
			if (grtop_rfl != 0)
				init_gio(i_RVIPEN,GRCAN,"RVPEN",0x90);
		}
	
		if ((cons[vip_en] & 0x02) != 0)
			init_gio(i_VPECC,GRCAN,"VPECC",0x90);
		
		for(i=1;i<=cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_vipmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'V';
				io_name[4] = 'P';
				io_name[5] = 'A';
				init_gio(i_VIP(i),GRCAN,&io_name[0],0x10 | 0x20);
				init_gio(o_VIPL(i),GRCAN,&io_name[1],0x01 | 0x20);
			}
		}
		if (grtop_rfl != 0)
		{
			for(i=1;i<=cons[grtopf]; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rvipmsk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'V';
					io_name[4] = 'R';
					io_name[5] = 'A';
					init_gio(i_RVIP(i),GRCAN,&io_name[0],0x10 | 0x20);
					init_gio(o_RVIPL(i),GRCAN,&io_name[1],0x01 | 0x20);
				}
			}
		}
	}

	
	if ((fvars[fvxasgn] & 0x01) != 0)
	{
		for(i=2;i<=cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_dcmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'X';
				io_name[3] = 'D';
				io_name[4] = 'I';
				init_gio(i_XDC(i),GRCAN,io_name,0x10);
				io_name[3] = 'D';
				io_name[4] = 'O';
				init_gio(o_XDCL(i),GRCAN,io_name,0x11);
			}
		}															  
		for(i=1;i<cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			if ((g_ucmsk[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'X';
				io_name[3] = 'U';
				io_name[4] = 'I';
				init_gio(i_XUC(i),GRCAN,io_name,0x10);
				io_name[3] = 'U';
				io_name[4] = 'O';
				init_gio(o_XUCL(i),GRCAN,io_name,0x11);
			}
		}
	}
	
	if ((grtop_rfl != 0) && ((fvars[fvxasgn] & 0x04) != 0))
	{
			
		for(i=grbot_rfl;i<=grtop_rfl; i++)
		{
			fl_ix = (i-1)/32;
			if (((g_rdcmsk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'Z';
				io_name[3] = 'D';
				io_name[4] = 'I';
				init_gio(i_XRDC(i),GRCAN,io_name,0x10);
				io_name[3] = 'D';
				io_name[4] = 'O';
				init_gio(o_XRDCL(i),GRCAN,io_name,0x11);
			}
		}
		for(i=grbot_rfl;i<=grtop_rfl; i++)
		{
			fl_ix = (i-1)/32;
			if (((g_rucmsk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'Z';
				io_name[3] = 'U';
				io_name[4] = 'I';
				init_gio(i_XRUC(i),GRCAN,io_name,0x10);
				io_name[3] = 'U';
				io_name[4] = 'O';
				init_gio(o_XRUCL(i),GRCAN,io_name,0x11);
			}
		}
	}
	
	// Dual Serial Riser
	if (((cons[sercom] & 0x20) != 0) && (cons[Dual_Ser_Riser] == 1))
	{
		for(i=1; i<=cons[grtopf]; i++)
		{
			fl_ix = (i-1)/32;
			{
				if((pfdsr_msk[fl_ix] & fl_mask[i]) != 0)
				{
				
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'D';
					io_name[4] = 'B';
					io_name[5] = 'A';
					init_gio(i_DB(i),GRCAN,&io_name[0],0x10 | 0x04);
					init_gio(o_DBL(i),GRCAN,&io_name[1],0x01 | 0x04);
					
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'U';
					io_name[4] = 'B';
					io_name[5] = 'A';
					init_gio(i_UB(i),GRCAN,&io_name[0],0x10 | 0x04);
					init_gio(o_UBL(i),GRCAN,&io_name[1],0x01 | 0x04);
					
				}
			}
		}
		if (cons[rear] != 0)
		{
			for (i=1; i<=cons[grtopf]; i++)
			{
				fl_ix = (i-1)/32;
				if ((prdsr_msk[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'D';
					io_name[4] = 'S';
					io_name[5] = 'A';
					init_gio(i_DRB(i),GRCAN,&io_name[0],0x10 | 0x04);
					init_gio(o_DRBL(i),GRCAN,&io_name[1],0x01 | 0x04);
					
					
					io_name[0] = ' ';
					io_name[1] = flmrk[i][0];
					io_name[2] = flmrk[i][1];
					io_name[3] = 'U';
					io_name[4] = 'S'; 
					io_name[5] = 'A';
					init_gio(i_URB(i),GRCAN,&io_name[0],0x10 | 0x04);
					init_gio(o_URBL(i),GRCAN,&io_name[1],0x01 | 0x04);
				}
			}
		}
	}


/*
	for(i=1;i<=cons[grtopf]; i++)
	{

		if (((cons[sercom] & 0x20) == 0) && (cons[CB_SR] == 1))
		{			  // no serial riser
		}
	}
	
	

*/

	// Floor Lockouts

	if (Grp_FL_Lockouts == 1)
	{
		init_gio(i_FLSEC,GRCAN,"FLSEC",0x90);

		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;
			if (((g_dcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_ucmsk[fl_ix] & fl_mask[i]) != 0) || ((g_cc_mask[fl_ix] & fl_mask[i]) != 0))
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'F';
				io_name[3] = 'L';
				io_name[4] = 'S';
				init_gio(i_FLS(i),GRCAN,&io_name[0],0x90);
			}
		}
		if (grtop_rfl != 0)
		{
			init_gio(i_RFLSC,GRCAN,"RFLSC",0x90);

			for (i=1;i<=cons[grtopf];i++)
			{
				
				fl_ix = (i-1)/32;
				if (((g_rdcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_rucmsk[fl_ix] & fl_mask[i]) != 0) || ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0))
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'R';
					io_name[3] = 'F';
					io_name[4] = 'S';
					init_gio(i_RFLS(i),GRCAN,&io_name[0],0x90);
				}
			}
		}
	}


	// Hall Call Lockouts
	if (Grp_HC_Lockouts == 1)
	{
		init_gio(i_HSEC,GRCAN," HSEC",0x90);

		for (i=1;i<=cons[grtopf];i++)
		{
			
			fl_ix = (i-1)/32;
			if (((g_dcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_ucmsk[fl_ix] & fl_mask[i]) != 0))
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'H';
				io_name[3] = 'C';
				io_name[4] = 'S';
				init_gio(i_HCS(i),GRCAN,&io_name[0],0x90);
			}
		}
		if (grtop_rfl != 0)
		{
			init_gio(i_RHSEC,GRCAN,"RHSEC",0x90);

			for (i=1;i<=cons[grtopf];i++)
			{
				
				fl_ix = (i-1)/32;
				if (((g_rdcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_rucmsk[fl_ix] & fl_mask[i]) != 0))
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'R';
					io_name[3] = 'H';
					io_name[4] = 'S';
					init_gio(i_RHCS(i),GRCAN,&io_name[0],0x90);
				}
			}
		}
	}


	if ( ((cons[sercom] & 0x20) != 0) && ((Security_Type[cons[carnmb]] & 0x100) != 0) )
	{
		init_gio(i_SHSEC,GRCAN," SSEC",0x90);

		for (i=1;i<=cons[grtopf];i++)
		{
			
			fl_ix = (i-1)/32;
			if (((g_dcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_ucmsk[fl_ix] & fl_mask[i]) != 0))
			{
				io_name[0] = ' ';
				io_name[1] = flmrk[i][0];
				io_name[2] = flmrk[i][1];
				io_name[3] = 'S';
				io_name[4] = 'S';
				io_name[5] = 'A';
				init_gio(i_SHS(i),GRCAN,&io_name[0],0x10);
				init_gio(o_SHSL(i),GRCAN,&io_name[1],0x01);
			}
		}
		if (grtop_rfl != 0)
		{
			init_gio(i_RSHSEC,GRCAN,"RSSEC",0x90);

			for (i=1;i<=cons[grtopf];i++)
			{
				
				fl_ix = (i-1)/32;
				if (((g_rdcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_rucmsk[fl_ix] & fl_mask[i]) != 0))
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'R';
					io_name[3] = 'S';
					io_name[4] = 'S';
					init_gio(i_RSHS(i),GRCAN,&io_name[0],0x10);
					io_name[2] = 'R';
					io_name[3] = 'S';
					io_name[4] = 'A';
					init_gio(o_RSHSL(i),GRCAN,&io_name[0],0x01);
				}
			}
		}
	}

	// Group car call overrides
	if (Grp_CC_Override == 1)
	{
		init_gio(i_GCSO,GRCAN," GCSO",0x90);

		for (i=1;i<=cons[grtopf];i++)
		{
			
			fl_ix = (i-1)/32;
			if ((g_cc_mask[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'G';
				io_name[3] = 'C';
				io_name[4] = 'O';
				init_gio(i_GOS(i),GRCAN,&io_name[0],0x90);
				
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'G';
				io_name[3] = 'O';
				io_name[4] = 'O';
				init_gio(o_GOSO(i),GRCAN,&io_name[0],0x90);
			}
		}

		if (grtop_rfl != 0)
		{
			init_gio(i_RGCSO,GRCAN,"RGCSO",0x90);

			for (i=1;i<=cons[grtopf];i++)
			{
				
				fl_ix = (i-1)/32;
				if ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'G';
					io_name[3] = 'R';
					io_name[4] = 'O';
					init_gio(i_RGOS(i),GRCAN,&io_name[0],0x90);
					
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'R';
					io_name[3] = 'O';
					io_name[4] = 'O';
					init_gio(o_RGOSO(i),GRCAN,&io_name[0],0x90);
				}
			}
		}
	}
	
	// Group car call security
	if (Grp_CC_Lockouts == 1)
	{
		init_gio(i_GCSEC,GRCAN,"GCSEC",0x90);

		for (i=1;i<=cons[grtopf];i++)
		{
			
			fl_ix = (i-1)/32;
			if ((g_cc_mask[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'G';
				io_name[3] = 'C';
				io_name[4] = 'S';
				init_gio(i_GCS(i),GRCAN,&io_name[0],0x90);
			}
		}

		if (grtop_rfl != 0)
		{
			init_gio(i_RGCSC,GRCAN,"RGCSC",0x10);

			for (i=1;i<=cons[grtopf];i++)
			{
				
				fl_ix = (i-1)/32;
				if ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'G';
					io_name[3] = 'R';
					io_name[4] = 'S';
					init_gio(i_RGCS(i),GRCAN,&io_name[0],0x90);
				}
			}
		}
	}
	
	if (cons[Sabbath] == 2)
	{
		init_gio(i_SABF,GRCAN," SABF",0x90);

		for (i=1;i<=cons[grtopf];i++)
		{
			
			fl_ix = (i-1)/32;
			if ((g_cc_mask[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'S';
				io_name[3] = 'B';
				io_name[4] = 'F';
				init_gio(i_SBF(i),GRCAN,&io_name[0],0x90);
			}
		}

		if (grtop_rfl != 0)
		{
			init_gio(i_RSABF,GRCAN,"RSABF",0x90);

			for (i=1;i<=cons[grtopf];i++)
			{
				
				fl_ix = (i-1)/32;
				if ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'S';
					io_name[3] = 'B';
					io_name[4] = 'R';
					init_gio(i_SBR(i),GRCAN,&io_name[0],0x90);
				}
			}
		}
	}
	
	
	// Hugs Security
	if ((cons[hugs] == 1) || (cons[hugs] == 2))
	{
		init_gio(i_HGSEC,GRCAN,"HGSEC",0x90);

		for (i=1;i<=cons[grtopf];i++)
		{
			
			fl_ix = (i-1)/32;
			if ((g_cc_mask[fl_ix] & fl_mask[i]) != 0)
			{
				io_name[0] = flmrk[i][0];
				io_name[1] = flmrk[i][1];
				io_name[2] = 'H';
				io_name[3] = 'G';
				io_name[4] = 'S';
				init_gio(i_HGS(i),GRCAN,&io_name[0],0x90);
			}
		}

		if (grtop_rfl != 0)
		{
			init_gio(i_RHGSEC,GRCAN,"RHGSC",0x90);

			for (i=1;i<=cons[grtopf];i++)
			{
				
				fl_ix = (i-1)/32;
				if ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0)
				{
					io_name[0] = flmrk[i][0];
					io_name[1] = flmrk[i][1];
					io_name[2] = 'R';
					io_name[3] = 'H';
					io_name[4] = 'G';
					init_gio(i_RHGS(i),GRCAN,&io_name[0],0x90);
				}
			}
		}
	}
	
	if (cons[LbyUpReq] == 1)
	{
		init_gio(i_LUC,GRCAN,"  LUC",0x90);
		init_gio(o_LUCL,GRCAN," LUCL",0x90);
	}

	if (cons[Car_Asgn_Fl] == 1)
	{
		init_gio(i_CASEC,GRCAN,"CASEC",0x90);
	}

	if (cons[Park_NCU_ALB] == 1)
	{
		init_gio(i_APRK,GRCAN," APRK",0x90);
		init_gio(i_NCU,GRCAN,"  NCU",0x90);
	}

	if ((cons[EMPop] & 0x08) != 0)
	{
		for (i=1;i<=cons[nmbcars];i++)
		{
			io_name[0] = 'P';
			io_name[1] = 'R';
			io_name[2] = 'K';
			io_name[3] = car_bld_no[i][0];
			io_name[4] = car_bld_no[i][1];
			init_gio(o_PRK(i),GRCAN,&io_name[0],0x91);
			
			io_name[0] = ' ';
			io_name[1] = 'O';
			io_name[2] = 'P';
			io_name[3] = car_bld_no[i][0];
			io_name[4] = car_bld_no[i][1];
			init_gio(o_OP(i),GRCAN,&io_name[0],0x91);
			
			io_name[0] = 'S';
			io_name[1] = 'E';
			io_name[2] = 'L';
			io_name[3] = car_bld_no[i][0];
			io_name[4] = car_bld_no[i][1];
			init_gio(o_SEL(i),GRCAN,&io_name[0],0x91);
		}
	}
	if ((cons[EMPop] & 0x10) != 0)		// EP recall finished output
	{
		init_gio(o_EPRCLF,GRCAN,"EPRCF",0x91);
	}
			
  	if (cons[SecFlCfg] == 2)
	{
		init_gio(i_SECF1,GRCAN,"SECF1",0x90);
		init_gio(i_SECF2,GRCAN,"SECF2",0x90);
		init_gio(i_SECF3,GRCAN,"SECF3",0x90);
	}

		// CODE BLUE second riser

	if (cons[CB_SR] != 0)
	{			  
		init_gio(i_CBSR,GRCAN," CBSR",0x90);
		if (grtop_rfl != 0)
			init_gio(i_RCBSR,GRCAN,"RCBSR",0x90);
	}


	if (cons[ep_auto_en] != 0)
	{
		init_gio(i_AUTEP,GRCAN,"AUTEP",0x90);
	}

	if (cons[epgrp] != 0)
	{
		init_gio(i_RECEN,GRCAN,"RECEN",0x90);
		init_gio(i_RUNEN,GRCAN,"RUNEN",0x90);
		init_gio(o_RUNCT,GRCAN,"RUNCT",0x91);
	}

			
	if (cons[svc_tmr_en] == 1)
		init_gio(i_TMREN,GRCAN,"TMREN",0x90);

	if ((cons[servOUT] & 0x10) != 0)
	{
		init_gio(o_GSERV,GRCAN,"GSERV",0x91);
	}

	if ((fvars[fvxasgn] & 0x05) != 0)	// NCA input - To monitor No Car Available from the old group, in such case assign hall calls to galaxy cars only
	{
		init_gio(i_NCA,GRCAN,"  NCA",0x90);
	}


	init_gio(i_GSP1,GRCAN,"GSPI1",0x90);
	init_gio(i_GSP2,GRCAN,"GSPI2",0x90);
	init_gio(i_GSP3,GRCAN,"GSPI3",0x90);
	init_gio(o_GSP1,GRCAN,"GSPO1",0x91);
	init_gio(o_GSP2,GRCAN,"GSPO2",0x91);
	init_gio(o_GSP3,GRCAN,"GSPO3",0x91);	

}


//**********************************
// Initialize io names to spaces
//**********************************

void init_ionames (void)
{
	int i;
	for(i=0;i<nmb_ios;i++)
	{
		ioname[i][0] = ' ';
		ioname[i][1] = ' ';
		ioname[i][2] = ' ';
		ioname[i][3] = ' ';
		ioname[i][4] = ' ';
	}
	
	for(i=0;i<nmb_hc_ios;i++)
	{
		hcioname[i][0] = ' ';
		hcioname[i][1] = ' ';
		hcioname[i][2] = ' ';
		hcioname[i][3] = ' ';
		hcioname[i][4] = ' ';
	}
	
}

/******************************************************/
/*** This is the Init the I/O Data routine ************/
/*** Initialize input and outputs to the off state ****/
/******************************************************/
void init_iodata(void)
{
	int i;

	for(i=0; i<nmb_io_arrays; i++)
	{		  // Do one board at a time on each display column
		iodata[cons[carnmb]][i] = IO_In_Out[i];	// Turn off input = 0  and outputs = ff
	}

	for(i=0;i<nmb_hc_io_arrays;i++)
	{		  // Do one board at a time on each display column
		iodata[0][i] = GIO_In_Out[i];	// Turn off input = 0  and outputs = ff
	}

#if (Tract_HR == 1)
	for (i=0; i<=29; i++)
#else
	for (i=0; i<=11; i++)
#endif
	{
		gpi[i] = 0;
		gpo[i] = 0;
		cgpi[i] = 0;
		cgpo[i] = 0;
	}
	
}



//******************************************************************
// Set io board status flag to display only the io data that is used
//******************************************************************

/*
void set_io_board_status (void)
{
	int16 i,j;

	page_2_brd_nmb[0] = 0;
	page_3_brd_nmb[0] = 0;
	page_4_brd_nmb[0] = 0;
	page_5_brd_nmb[0] = 0;
	max_nmb_io_brd[0] = 0;
	iobd_rotate_start[0] = 0;
	last_io_brd[0] = 0;


	nmbgio_page = 0;
	nmbhcb = 0;
	next_page_hcb_2 = 0;
	next_page_hcb_3 = 0;
	next_page_hcb_4 = 0;
	next_page_hcb_5 = 0;
	for (i=0;i<nmb_hcio_brds;i++)
	{
		if (HC_IO_board[i] != 0)
			nmbhcb++;

		if ((nmbhcb > 0) && (nmbhcb <= 10))
			nmbgio_page = 1;
			
		if ((nmbhcb > 10) && (next_page_hcb_2 == 0))
		{
			next_page_hcb_2 = i;
			nmbgio_page = 2;
		}

		if ((nmbhcb > 20) && (nmbgio_page == 2))
		{
			next_page_hcb_3 = i;
			nmbgio_page = 3;
		}
		if ((nmbhcb > 30) && (nmbgio_page == 3))
		{
			next_page_hcb_4 = i;
			nmbgio_page = 4;
		}
		if ((nmbhcb > 40) && (nmbgio_page == 4))
		{
			next_page_hcb_5 = i;
			nmbgio_page = 5;
		}

	}



	// *******************************************
	// Set Valid I/O boards
	// *******************************************

	for (i=1;i<=cons[nmbcars];i++)
	{
		page_2_brd_nmb[i] = 0;
		page_3_brd_nmb[i] = 0;
		page_4_brd_nmb[i] = 0;
		page_5_brd_nmb[i] = 0;
		max_nmb_io_brd[i] = 0;
		iobd_rotate_start[i] = 0;
		last_io_brd[i] = 0;
		for (j=0;j<nmb_io_brds;j++)
		{		  // set the first board for each video page
			if (i!= cons[carnmb])
				IO_board[i][io_bd_order[j]] = IO_board[cons[carnmb]][io_bd_order[j]];
			if (IO_board[i][io_bd_order[j]] != 0)
			{
				max_nmb_io_brd[i] ++;
				if ((max_nmb_io_brd[i] > 10) && (page_2_brd_nmb[i] == 0))
					page_2_brd_nmb[i] = io_bd_order[j];
				if ((max_nmb_io_brd[i] > 20) && (page_3_brd_nmb[i] == 0))
					page_3_brd_nmb[i] = io_bd_order[j];
				if ((max_nmb_io_brd[i] > 30) && (page_4_brd_nmb[i] == 0))
					page_4_brd_nmb[i] = io_bd_order[j];
				if ((max_nmb_io_brd[i] > 40) && (page_5_brd_nmb[i] == 0))
					page_5_brd_nmb[i] = io_bd_order[j];
				if ((j == (fixed_io_brd_dpy + 1)) && (iobd_rotate_start[i] == 0))
					iobd_rotate_start[i] = io_bd_order[j];
				last_io_brd[i] = io_bd_order[j];
			}
		}
	}
}
*/

/* Revision History

10/06/03 3.09 mhd		1. Created this routine to place all i/o name in a ram array to be able to transfer this data serially
						   to a user device.  
						2. When using serial COP i/o's, hall calls can be placed on the 1010 board if < 7 floors.
10/30/03 3.10 MHD		1. Added EML output.
11/11/03 3.12 mhd		1. Deleted using "DISP" on io board.
12/04/03 3.14 MHD		1. Added discrete Pi's and direction lanterns on serial cop board for hydro cars.
12/22/03 3.17 mhd		1. Add new security_type[] = 4 for seperate car call and hall call lockouts.  Use the rear car call 
				   		   lockouts for the hall calls.
3/3/04 4.00 mhd			1. Modified i/o's to uses serial comm to the Top of Car board. Have only one i/o configuration now.
4/21/04 4.00 mhd		1. Setup new io locations for 1038 board. Changed board initialization, io addresses and started io array at 0.
11/3/04 4.03 mhd		1. Changed otslbp flag from == 1 to != 0 to allow for the flag to be 2.
2/24/05 4.06 mhd		1. Added init for i/o's CUL, CDL, FL, FB/NB, HB and DTR.
3/1/05 4.07 mhd			1. Relocated 16CR, 17CR, CULR, CDLR, DOLR AND DCLR.
3/31/05 4.08 MHD		1. Make earthquake i/o's not show on screen unless eaq flag is set.
						2. Make medical emergency i/o's not show on screen unless med flag set.
4/23/05 4.11 mhd		1. Modified security i/o labels.
4/24/05 mhd    			2. Changed Security_Type to allow both car call lockouts and group car call lockouts.
4/26/05 v4.14 mhd		1. Added extra lobby up call cons[LbyUpReq].
5/6/05 v4.17 mhd		1. Correct index for icr_loc above 169.
5/12/05 v4.18 mhd		1. Use TDC, MDC, BDC, MDCR and RCM i/o's for manual doors.
5/18/05 				2. Changed LU and LD to LLU and LLD respectively.
6/23/05 V4.23 mhd		1. Changed MC to MCA and MCi to MCAi.
6/27/05 v4.23 mhd		2. Changed FSTP to FST, FSTP1 to FST1, input FST to FSTi and STP to CS.
7/12/05 v4.24 mhd 		1. Added new Security_Type 0x10 for Lockout_Override to be used with group or ids floor lockouts.
7/21/05 v4.25 mhd		1. Fixed index for RUNEN when using intergroup emergency power.
9/23/05 v4.34 pn		1. Added i/o's for peelle door interface.
						2. Delete access to s-curve board at i/o boards 26 and 27.
12/14/05 V4.44 mhd		1. Changed label for EP to DZA.
1/16/05 v4.47 pn		1. Added overload light when load weigher is set to 2.
1/16/06 V4.48 mhd 		1. Delete using CWS for hydro cars.
						2. Added FFS input.
2/6/06 v4.52 pn			1. Include rear car call override when option 5 is set in the cons file.
2/8/06 V4.25 mhd		1. Changed label for FFS to SS and SS to CTS.
2/16/06 v4.55 mhd		1. Do not use RCM when lobby lanterns used
2/27/06 V4.56 mhd		1. Added DNO (GEN) for reset jack operation (cons[home] >= 100).
3/1/06 v4.57 pn			1. Added seperate retiring cam output on courion interface I/O board.
3/6/06 		 mhd		2. Changed IAO to ADO for courion door.
						3. Changed RTL to check ((cons[sercom] & 0x01) != 0) instead of ((cons[sercom] & 0x01) == 0)
3/21/06 v4.59 mhd		1. Added RDT (Rear door transfer relay) for peele doors.
3/23/06 v4.60 mhd		1. Added seperate hall call security input on gpi board.
3/30/06 v4.fl_size pn    	1. Added rear auto door open for Courion door operator.
4/4/06 V4.fl_size mhd		2. Made TDC input an array to use EQR input.
4/28/06 v4.64 mhd		1. Corrected setting of ept_io_sel: Use EPT in place of EMH when no manual door, no emh and no low pressure switch.
5/3/06 V4.65 mhd		1. Corrected hall call security when security type flag == 4 (seperate floor and hall lockouts without rear).
						   So that the Floor lockout board is not shown when security type == 32.
						2. Added new labels for car and group security.
5/24/06 v4.67 pn		1. Added option for courion model D door operator. 
5/26/06 v4.68 mhd		1. Added group emergency power car status indicators.
						2. Added security car call latch output.
						3. Added ED for remoted car call station instead of INDC.
6/7/06 V4.70 mhd		1. Turn on io_board[22] for courion model D door operator so that the i/o's get displayed.
8/31/06 v5.04 mhd		1. Check flag for freight door so that cons[mand] does not have to be set also.	
						2. Move RCM to 4U output if LLU is used.
11/1/06 v5.07 mhd		1. Added DZDE door zone door enable output.
						2. Remove rear i/o's from display if no rear door.
11/8/06 v5.08 mhd		1. Added UT4/DT4, UT5/DT5 and UT6/DT6.
1/3/07 v5.10 mhd		1. For car lockout security, use board 9 if >= 24 floors instead of > 24.
						2. Use EMLH (light in hall) when car on hospital service.
2/1/07 v5.16 mhd		1. Modified files to use additional i/o boards for more than 29 floors. (see Tract_HR == 1)
2/20/07 v5.17 mhd		1. Changed cons[remote_cc] to cons[Remote_CC] to display ED input because remote_cc is a variable and not cons[] index.
2/15/07 V5.18 mhd		1. Renamed HC to HCC.
						2. Made the floor mask large enough for 60 floors.
						3. Changed iodata and related arrays from 85 to 145	and also changed gpi and related arrays from 11 to 30.
						4. Increased the number of car io boards from 27 to 48 and increased the number of group io boards from 20 to 46.
						5. Added new i/o names for additional boards.
2/23/07 v5.19 mhd		1. Corrected size declaration for bittbl from int to int16.
5/30/07 v5.24 mhd		1. Added new brake board input BKS1, BKS2  (Brake Lift Switch 1 and 2) and BDBS (Brake Drive Bypass Switch). 
6/15/07 v5.27 mhd		1. Use DTR for very short floor.
						2. Move BDC from EMH to EQ if emergency medical and manual doors.
7/3/07 v5.29 mhd		1. Modified set_io_board_status for security_type 0x40 to show only output if 0x40 is only set.
8/6/07 v5.31 mhd 		1. Added cons variable for jack reset (cons[Reset_Jack] == 1),
8/24/07 v5.35 mhd		1. Added Micro Releveling UPML and DNML outputs
8/28/07 v5.35.1 mhd 	1. Add Automatic Swing Door Open with manual gate.	Note: cons[mand] == 1.
10/4/07 v5.38 mhd		1. Added IR i/o when (cons[Second_Riser] != 0).
11/12/07 v5.39 mhd  	1. Allow VIP to use the code blues calls (if not used) or the rear code blue calls (if not used). 
11/19/07 v5.39.1 mhd	1. Added cons[Park_NCU_En] (parking and next up enable) i/o's and re-structed logic for cons[Car_Asgn_Fl] and cons[LbyUpReq].
11/28/07 V5.29.4 mhd	1. Move EDL to P5 from P1.	
12/20/07 v5.39.8 mhd	1. Added VIP labels on CB Board.	
						2. Change location for RICR from (cons[grtopf]+1) to (grtop_rfl + 1).
						3. Change Convert_pi to use grtop_rfl for limit on rear calls.
1/4/08 v5.40.1 mhd		1. Added i/o change cons[F1_CT] (Fire 1 Car Top) to place FBNB and FL on 1040 TOC board in place of EML and EQL.  When this option 
                           is selected, EML, EQL, and OLL are moved to P2, P3 and P4 on the 1040 Car call board.  HVD is moved to DZDE location and REV is 
						   moved to the DTR location on the 1040 (if option for HVD and REV selected).
						2. Added outputs F1ML and F1AL to EQL and EML outputs.  These outputs will follow the location of EML and EQL even when cons[F1_CT]
						   selected.
1/25/08 v5.40.3 mhd		1. Changed name of RCM_HWDoorCL to RCM_HWDoor. Change variable RCM HOLD TO RCM Control added bit 2 for advanced RCM when RCM_HWDoor set.
						2. Add RCM output when RCM_HWDoor is set with auto door.
2/8/08 v5.40.4 mhd		1. Added ATTHC attendant hall call cancel input (works for simplex cars only).
4/4/08 v5.42.7 pn		1. In the copy hall call i/o table to ram array for the cross assignment I/O's (traction
						   high rise) changed index from k = 672 to k = 648 in order to display the cross assignment I/O's
						   correctly.
4/3/08 v5.43 mhd		1. Fixed convert_call() to check for rear or front top floors being higher than the opposite top floor to add an up call at the top 
                           front or rear floor.
4/9/08 V5.43.1 mhd		1. Added input EOR for cons[Elev_off] latch reset.
4/15/08 v5.43.3 pn		1. Added cons[ids] bit 2 for adding an additional GPIO board for 
						   montoring of external signals through lift-net.
4/23/08 v5.43.7 mhd		1. Added i/o for emergency sheave brake EBK, EBKC, EBKi, EBKS.
5/22/08 v5.44 mhd		1. Added fdoor_io_sel and rdoor_io_sel for front and rear door movfe can bus. Added hvd_io_sel for movfe can bus.
6/13/08 v5.44.3 pn		1. Removed reference for traction to include hall elevator off feature for hydro.
6/30/08 V5.44.5 mhd		1. Reversed serial io hall calls order to start with 1D at the top of display.  Changed index from 240 to 239.
7/29/08 v5.45.3 mhd		1. Add FF output to io table if quatro dc drive, dsd-412 and traction.
8/19/08 v5.45.5 pn		1. Added 4 additional outputs for EMS door operator.
8/19/08 v5.45.6 mhd		1. Added IDSO and INDS for Independent Service Security
						2. Added cons[EM_BRK] = 5 for seperate EBC and UTM control.
9/3/08 v5.45.8 mhd		1. Added cons[hugs] == 1 for hugs security service. Uses (Security_Type & 0x20) i/o.
10/21/08 v5.46.3 mhd	1. Added DTR2 if DTR is being used by staggered rear floor.
12/16/08 v5.46.8 hdl	1. Added check for cons[att] == 2 to set attendant hall annunciator lights.
1/12/09 v5.47.1 mhd		1. For serial hall call check above 24 floors the index changes from i to i-240 to select the correct i/o.
1/13/09 v5.47.2 mhd		1. Added hall call board 21 and board 44 for extended group io when using serial hall calls.
3/1/09 v5.47.7 mhd		1. Added car call pushbutton security input CCPBS.
4/22/09 v5.48.4 mhd		1. Add output of DPM and RPM for serial movfr.
						2. Set i/o's for dual serial riser
5/13/09 v5.48.5 mhd		1. Added i/o's for manual power loss brake MBP, MBC, MB1, MB2.
6/1/09 v5.48.8 mhd		1. Move Earthquake i/o to BRD27 if HWS2 or INE OR SFC input functions added.
						2. Move manual door i/o TDC,BDC and MDC to BRD27 if INE or SFC input functions added. Also move RTLDC.
7/15/08 V5.49 mhd		1. Renamed FS2 to FS2OF when cons[FS2_SW] == 1.
8/19/09 v5.49.9 mhd		1. Corrected check for HC_IO_board[20] for cons[grtopf] > 25 instead of cons[grtopf] > 29 in set_io_board_status().
8/28/09 v5.49.11 mhd	1. Added set ios for selector input on board 41 if sel_type == 2.
						2. Added Governor trip ouput in the location of MTB.
9/30/09 v5.50.3 hdl		1. Modified location of ISER output.
10/12/09 v5.50.4 mhd	1. Added housing inputs HMO, DMO, and LHC.
11/11/09 v5.50.7 hdl	1. Allow EOR input to coexist with earthquake
12/1/09 v5.50.11 mhd	1. Added NPD, EPD and EPO for single phase UPS recovery.
12/29/09 V5.51.2 mhd	1. Added SABO, sabbath On output for 3D output on 1064 board. Must not conflict with DT5. 
3/15/10 v5.51.14 mhd	1. Added security inputs on board B18 (44) address for SECF1, SECF2 and SECF3.
3/16/10 v5.52 mhd		1. Corrected using io_sel with REV and RVD instead of ct_io_sel.
						2. Set io_sel = 2 and ct_io_sel = 3, when using cons[RGB_CC} = 1, rgb car call boards.
						3. Moved the following I/Os for the RGB car call board:
							UDAC, DDAC, DOBI, DCBI, FS2C, DTR, DTR2, CLF, FL, FNBN, ATTUP, ATTDN, ATT, ATTBY, ATTHC,
							CCL[], CC[], ISDO, EMS, HBE, HB, SABUZ, INDC, FS2, FS2H, ED, EDL, DLW, ATUL, ATDL, REV, RVD,
							DPI1, DPI2, DPI3, DPI4, DPI5, DPI6, DZDE, F1ML, F1AL, EML, EQL, DCA, CULR, CDLR and OLL.
						4. Corrected remote car call for high rise cars see remote_cc_io().
						5. Corrected override security for high rise cars.
3/29/10 V5.52.1 mhd 	1. Moved HBE, FS2C, ED, DOB, DCB and 7CC.
4/1/10 v5.52.2 mhd		1. Changed (cons[dispis] == 1) to ((cons[dispis] & 0x01) != 0) to allow for additional bit usage.
5/4/10 V5.52.7 mhd		1. Added In Car Inspection up and down inputs for RGB_CC boards.
						2. Added inputs FiDOB, FiDCB, FiFS2C, FiDOBR, FiDCBR, and output FiFL for rgb car call boards.
5/7/10 v5.52.8 mhd		1. Added color index for every io so that the car call and emergency outputs can be color selected.
7/7/10 V5.52.14 mhd		1. Added Aux_Brake and EM_BRK i/o's XBKS1, XBKS2 and XBDBS for electronic brakes.
7/29/10 v5.52.19 mhd	1. Added i/o's and i/o boards for code blue second riser
8/18/10 v5.52.25 mhd	1. Add io names for hall security (cons[SecFlCfg] == 3).
8/23/10 v5.52.26 hdl	1. Added HUGO (hugs output). This outputs maps by default to P1 in the car station. 
						2. Added INSEC output 
9/20/10 5.52.31 hdl		1. Added ELOO for a flashing output when in elevator off (servf = s_ELEV_OFF_HS)
						2. Added cons[Elev_off] & 8 option to enable ELOO
						3. Corrected Statement for rm_io_sel = 1. It did not map properly the SCS and  INDC
10/26/10 v5.52.41  mhd	1. Added boards 49,50,51 and 52 for traction high rise and boards 29,30,31 and 32 for all others.
1/11/10 v5.52.55 mhd	1. Added HEOF, RTL, EMSH, RTL and EMLH i/o's to boards 49,50 for high rise and boards 29,30 for all others when 24v hall fixture flag set.
1/18/11 v5.52.56 mhd	1. Corrected setting rtlh_io_sel.  The statement "rtlh_io_sel = a2004_io_sel" had to be done before the conditionsl
							statement "if (cons[FI_EP_Ext] != 0)".  
1/26/11 v5.52.58 hdl	1. Added outputs for EMS door operator. EMS8,9,10,11

4/8/11 v5.52.10 hdl		1. EPCOM output located in the top hall call plus one. It dinamically relocates to the last output if intergroup or any other is enable. It requires fvariable and reboot CPU

5/27/11 v5.54.01 hdl    1. Added  ATTBP display signal
5/30/11 v5.54.02 mhd	1. Corrected ATTBP for RGB_CC only so that ED/AB still shows up for other jobs.

6/1/11 v5.54.03 mhd		1. Added cons[Australia] variable to use external i/o board for hall fire and emergency power i/o. TDC, BDC, MDC and MDCR inputs added for automatic doors.
						2. Rename FS2H to FS2S for fire service start.
8/12/11 V5.52.21 HDL	1. added an ouput for DAO for Designated Attendant Operation

10/4/11 v5.52.27 hdl	1. changed logic for hvd_io_sel so it wont conflict with FL if RGB and F1_CT is set on

12/7/11 v5.52.42 hdl	1. Enable RGB boards all the time for PHONE input to be present	IO_board[i][47] = 1 IO_board[i][48] = 1
1/9/12 v5.54.48 hdl		1. Changed io location of eor for it to follow heof in the fire service board.
3/3/12 V5.54.fl_size hdl 	1. Blocked the SABO from alternating for regular sabbath
						2. Added (cons[Elev_off] & 0x10) to enable the ELOO to show just for telling the car finished recall of elevator off mode		

3/14//12 v5.54.63 hdl	1. Added f_rm_io_sel and r_rm_io_sel to set the location of the boards for remote car calls

3/29/12 v5.54.71 mhd	1. Added MRIE input array for motor room inspection enable.
4/18/12 v6.0.06 mhd		1. Made EBKS an array and use cons[EM_BRK] to enable emergency brake.
5/1/12 v6.0.7 mhd		1. Added new brake control cons[EM_BRK] == 7.  This is to have a controled drop of the emergency brake even after MC drops.
6/21/13 v7.0.9 mhd		1. Added hall lanterns, discrete and binary PI's and Attendant hall call annunciation.						
7/16/2013		pn.		1. Fixed the problem where the labels for the attendenant annunnicator outputs were not 
						   being sent to the I/O boards.
7//25/13 v7.0.15 mhd	1. Added o_std_DO, o_std_DOR ... etc for additional door oputput with movfr and made labels xDO,xDC,xNUD, etc.
11/22/13 v7.1.29 mhd	1. Corrected i_EDHL; was o_EDHL.
05/19/14 v7.1.46 hdl	1. Changed the name for input LPH to L120 to match the board terminal.
5/20/14 v7.0.47 mhd		1. Return value for found in init_io() and init_gio() to detect when an io is not valid
10/9/14 v7.1.16 mhd		1. Modified changes from galaxy 3 version 6.0.53 to 6.0.60.  See below:
						v6.0.59 as		1. Added smk_io[] to label and read in smoke sensor inputs, added ALTO and MESO outputs on the same board
10/28/14 v7.1.19 mhd	1. Moved DOL, DCL, RDOL and RDCL to MRCAN bus for manual doors.
12/4/14 V7.1.22 mhd		1. Added GSERV output for cross assignment to automatically swith to old group when all cars out of service.
3/30/15 V7.1.28 mhd		1. Added XINDC input for auxilary car station.
5/5/15 V7.1.31 mhd		1. Change INE to IEN.
06/04/15 v7.1.32 hl     1. Changed "  EMLH" to " EMLH". Removed extra space from label.
6/26/15 v7.1.36 as		1. Modified reading in cons[dispis] +1 discrete position indication reading and sending for displaying on the board.
12/07/15 v7.1.52 hl		1. cons[CCS_BUS] was modified to allow the location of enable input and security inputs to be in separate can ports. 
						   Now, you can have SECUR in the COP and the CCS inputs in the car Or the other way around. See cons file description.
12/22/15 v7.1.55 mhd	1. Delete initialization of MB1 and MB2 for power loss brake.
1/4/16 v7.1.56 mhd		1. Made spare I/O actual inputs and outputs so the show up on io boards.
01/05/16 v7.1.57 tb		1. Fixed bug where SECUR/CCS io's were not being initialized in CTCAN. Added check for ((cons[CCS_BUS] & 0x01) != 0),
01/05/16 v7.1.59 tb		1. Added I/O's for Dual Serial Riser when (((cons[sercom] & 0x20) != 0) && (cons[Dual_Ser_Riser] == 1)).
01/12/16 v7.1.63 tb		1, Modified I/O labels for cons[Remote_CC] to match COP I/O labels. No I/O conflict because Remote_CC I/O's are on MRCAN.
8/16/17 v8.0.8 mhd		1. Moved RLM and RLM-1 to MAINIO from MRCAN port.
						2. Renamed RDET to DETR.
8/25/17 V8.0.9 mhd		1. Set bit to display io for MDC, TDC, BDC and MDCR.
*/					
