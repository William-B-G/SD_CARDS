// I/O Port selection variables

#define INV_IO 0
#define MAINIO 1
#define MRCAN 2
#define CTCAN 3
#define GRCAN 4


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

extern char spare_ioname[][5];
extern const char SPARE_IOS [][5]; 
extern char spare_io_stat[];

extern int16 mr_last_array;
extern int16 ct_last_array;
extern int16 gr_last_array;


//*********************************************************************************************
// MAIN I/O BOARD
//*********************************************************************************************

extern struct io si_AD;
extern struct io si_IND;
extern struct io si_LBP;
extern struct io si_GBP;
extern struct io si_L120; //"formerly LPH"
extern struct io si_S10;
extern struct io si_FEP;
extern struct io si_EBK;	// Emergency brake coil input
extern struct io si_GOV;


extern struct io si_TF;
extern struct io si_BF;
extern struct io si_PS;
extern struct io si_HSS;
extern struct io si_ACC;
extern struct io si_BAD;
extern struct io si_BAU;
extern struct io si_TAD;

extern struct io si_TAU;
extern struct io si_DLB;
extern struct io si_DLB_1;
extern struct io si_DLM;
extern struct io si_DLM_1;
extern struct io si_DLT;
extern struct io si_DLT_1;
extern struct io si_RLM;

extern struct io si_RLM_1;
extern struct io si_TDC;
extern struct io si_MDC;
extern struct io si_BDC;
extern struct io si_MDCR;
extern struct io si_BKS;
extern struct io si_EBKS;	// Emergency Brake Switch input
extern struct io si_SPI;

extern struct io si_MRIU;
extern struct io si_MRIN;
extern struct io si_AUTO;
extern struct io si_MRSW;
extern struct io si_SFC;
extern struct io si_PFC;
extern struct io si_MRID;
extern struct io si_MRIE;	// Machine room inspection input
extern struct io si_ICA;

extern struct io si_CEN;
extern struct io si_SPD;
extern struct io si_CFLT;
extern struct io si_L120B;
extern struct io si_NTSF;
extern struct io si_PALF;
extern struct io si_TSTM;
extern struct io si_RDOOR;
extern struct io si_TSD;

extern struct io si_READY;	// Drive Ready input
extern struct io si_GTS;	// Gripper Trip Switch input
extern struct io si_GRT1;
extern struct io si_GRT2;
extern struct io si_RGS_1;
extern struct io si_GS;
extern struct io si_GS_1;
extern struct io si_FST;

extern struct io si_DN;	// Down Directional Stop
extern struct io si_DZ;	// Door Zone input (DZU AND DZD)
extern struct io si_DZ_1;	// Door Zone input (DZU AND DZD)
extern struct io si_RGS;
extern struct io si_CTA;
extern struct io si_DTS;	// Down Emergeny Terminal Slowdown
extern struct io si_UN;	// Up Directional Stop
extern struct io si_UTS;	// Up Emergency Terminal Slowdown 

extern struct io si_ID;
extern struct io si_INE;
extern struct io si_IU;
extern struct io si_INS;
extern struct io si_P;
extern struct io si_ICI;
extern struct io si_FFS;

extern struct io si_MCC;
extern struct io si_MCA;
extern struct io si_DON;
extern struct io si_BRK;
extern struct io si_RUN;
extern struct io si_MCX;
extern struct io si_RUNA;
extern struct io si_RUNX;

extern struct io si_SD;
extern struct io si_SDF;
extern struct io si_SU;
extern struct io si_SUF;
extern struct io si_NTS;
extern struct io si_ETS;

extern struct io so_CEN;
extern struct io so_MTO;
extern struct io so_STE;
extern struct io so_UL;
extern struct io so_DL;
extern struct io so_DZ;
extern struct io so_SELOK;
extern struct io so_SP75;
extern struct io so_SP150;
extern struct io so_HWLRN;


extern struct io so_FLH;
extern struct io so_GR2R;
extern struct io so_LE;
extern struct io so_LE1;
extern struct io so_FST;
extern struct io so_FSTP;

extern struct io so_RST;
extern struct io so_MCC;
extern struct io so_MCA;
extern struct io so_PFC;
extern struct io so_DBG;
extern struct io so_FF;
extern struct io so_BUZ;
extern struct io so_EBK1;

extern struct io so_RUNA;
extern struct io so_RUN;
extern struct io so_SD;
extern struct io so_SDF;
extern struct io so_SU;
extern struct io so_SUF;

extern struct io so_BRK;
extern struct io so_EBK2;
extern struct io so_DBC;
	


//*********************************************************************************************
// Machine Room Can I/O
//*********************************************************************************************

extern struct io si_EBK1;	// Emergency brake 1 coil input
extern struct io si_EBK2;	// Emergency brake 2 coil input
extern struct io si_EBA;	// Emergency brake A coil input
extern struct io si_EBB;	// Emergency brake B coil input
extern struct io si_LC;	// LC 120VAC input 

extern struct io si_TPL;
extern struct io si_TPH;

extern struct io si_SEE;
extern struct io si_TEE;
extern struct io si_CMP;
extern struct io si_BFS;
extern struct io si_PDS;
extern struct io si_HMO;

extern struct io si_TGDO;
extern struct io si_TGDOR;
extern struct io si_TGDC;
extern struct io si_TGDCR;
extern struct io si_CCPBS;

extern struct io si_XBKS1;
extern struct io si_BKS1; 
extern struct io si_BKS2;
extern struct io si_BDBS;


extern struct io si_RM_ED;
extern struct io si_RM_EDR;
extern struct io si_RM_INDC; 
extern struct io si_OSERA;
extern struct io si_SAB;
extern struct io si_SECFM;

extern struct io si_SCSMR;
extern struct io si_SCSRMR;

extern struct io si_BP1;
extern struct io si_BP2;
extern struct io si_BP4;
extern struct io si_BP8;
extern struct io si_BP16;
extern struct io si_BP32;


extern struct io si_LPS;
extern struct io si_LOS;
extern struct io si_RM_DOB;
extern struct io si_RM_DCB;
extern struct io si_RM_DCBR;
extern struct io si_RM_DOBR;

extern struct io si_EMP;
extern struct io si_EPS;
extern struct io si_EPT;
extern struct io si_HEOF;
extern struct io si_HEOFR;
extern struct io si_JAILB;
extern struct io si_HUGI;
extern struct io si_HUGB;
extern struct io si_RTL;
extern struct io si_RTLH; 
extern struct io si_RTLDC;
extern struct io si_EMSH;

extern struct io si_THDS;

extern struct io si_FS;
extern struct io si_BP;
extern struct io si_FSX;
extern struct io si_ALT; 
extern struct io si_MES;
extern struct io si_MRS;
extern struct io si_HWS;
extern struct io si_HWS2;


extern struct io si_EQR;
extern struct io si_ETH;
extern struct io si_COL;


extern struct io si_CWS;
extern struct io si_ELOF;
extern struct io si_CCP;
extern struct io si_CCLP;
extern struct io si_HCLP;
extern struct io si_LHCP;
extern struct io si_DMO;
extern struct io so_ISPD;


extern struct io so_BRKC;
extern struct io so_EBK;
extern struct io so_EBKC;
extern struct io so_EBA1;
extern struct io so_EBA2;
extern struct io so_EBB1;
extern struct io so_EBB2;
extern struct io so_EBC;
extern struct io so_UPML;
extern struct io so_DNML;
extern struct io so_NPL;
extern struct io so_EMLH;
extern struct io so_EPLH;
extern struct io so_EPRL;
extern struct io so_EPSL;
extern struct io so_HUGO;
extern struct io so_HUGA;
extern struct io so_NPD;
extern struct io so_EPD;
extern struct io so_EPO;
extern struct io so_TCU;
extern struct io so_LU;
extern struct io so_LD;
extern struct io so_LUR;
extern struct io so_LDR;
extern struct io so_GEN;
extern struct io so_DNO;
extern struct io so_MTB;
extern struct io so_GBL;
extern struct io so_LBL;
extern struct io so_GOVR;
extern struct io so_GOVT;


extern struct io so_RCF;
extern struct io so_MST;
extern struct io so_GR1R;
extern struct io so_UTM;
extern struct io so_TDOL;
extern struct io so_TDCL;
extern struct io so_TDOLR;
extern struct io so_TDCLR;
extern struct io so_TAVL;
extern struct io so_TAVL2;
extern struct io so_TIND;
extern struct io so_TMED;
extern struct io so_TFSO;
extern struct io so_TP1;
extern struct io so_TP2;
extern struct io so_TP3;
extern struct io so_TP4;
extern struct io so_TP5;
extern struct io so_TP6;
extern struct io so_EQL;
extern struct io so_MBC;
extern struct io so_MBP;
extern struct io so_MB1;
extern struct io so_MB2;
extern struct io so_FSDO;
extern struct io so_RSDO;
extern struct io so_RVDR;
extern struct io so_REVR;
extern struct io so_RVD;
extern struct io so_REV;
extern struct io so_HVD;
extern struct io so_HVDR;


extern struct io so_FSO;
extern struct io so_FiFL;
extern struct io so_FLCT;
extern struct io so_FBNBCT;
extern struct io so_FBNB;
extern struct io so_DCA;
extern struct io so_HB;
extern struct io so_SABUZ;
extern struct io so_SABEE;
extern struct io so_SABO;
extern struct io so_ISER;
extern struct io so_INSO;
extern struct io so_OSERB;
extern struct io so_OSERL;
extern struct io so_OPENL;
extern struct io so_INSEC;
extern struct io so_FSOCS;
extern struct io so_FSORS;
extern struct io so_FW;
extern struct io so_DDA;
extern struct io so_UDA;
extern struct io so_DPI1;
extern struct io so_DPI2;
extern struct io so_DPI3;
extern struct io so_DPI4;
extern struct io so_DPI5;
extern struct io so_DPI6;
extern struct io so_DDAC;
extern struct io so_UDAC;
extern struct io so_DPI1C;
extern struct io so_DPI2C;
extern struct io so_DPI3C;
extern struct io so_DPI4C;
extern struct io so_DPI5C;
extern struct io so_DPI6C;

extern struct io so_OLL;
extern struct io so_HDB;
extern struct io so_DESL;
extern struct io so_INSP;
extern struct io so_ADO;
extern struct io so_RADO;
extern struct io so_PH1R;
extern struct io so_PH2HD;
extern struct io so_PH2ON;
extern struct io so_PH2OF;
extern struct io so_DAO;
extern struct io so_RC;
extern struct io so_SQUEZ;
extern struct io so_RDT;
extern struct io so_MPTR;
extern struct io so_CODG; 	// Door/Gate Cut-out output for courion door interface
extern struct io so_COHS;
extern struct io so_ATO;
extern struct io so_IAO;
extern struct io so_FPH1;
extern struct io so_RAHS;
extern struct io so_FPH2;
extern struct io so_DSC;
extern struct io so_FDOB;
extern struct io so_RDOB;
extern struct io so_TC;
extern struct io so_FRCL;
extern struct io so_HCO;
extern struct io so_FH;
extern struct io so_FPS2;

extern struct io so_EFST;
extern struct io so_EMS1Z;
extern struct io so_EMS2Z;
extern struct io so_EMS3Z;
extern struct io so_EMS4Z;
extern struct io so_EMS5Z;
extern struct io so_EMS6Z;
extern struct io so_EMS7Z;
extern struct io so_EMS8Z;
extern struct io so_EMS9Z;
extern struct io so_EMS10Z;
extern struct io so_EMS11Z;
extern struct io so_FRT1Z;
extern struct io so_FRT2Z;
extern struct io so_FRT3Z;
extern struct io so_PDO;
extern struct io so_PDOR;
extern struct io so_G_INSP;
extern struct io so_G_ADO;
extern struct io so_G_FDCBO;
extern struct io so_G_RDCBO;
extern struct io so_G_ADOH;
extern struct io so_G_RDO;
extern struct io so_G_RDOH;
extern struct io so_G_RC;
extern struct io so_G_SOPEN;
extern struct io so_G_ININD;
extern struct io so_G_FH1R;
extern struct io so_G_PH2HD;
extern struct io so_G_PH2ON;
extern struct io so_G_PH2OF;

extern struct io so_RUDA;
extern struct io so_RDDA;
extern struct io so_RFSO;
extern struct io so_RPI1;
extern struct io so_RPI2;
extern struct io so_RPI3;
extern struct io so_RPI4;
extern struct io so_RPI5;
extern struct io so_RPI6;
extern struct io so_RPI7;
extern struct io so_RPI8;
extern struct io so_RPI9;
extern struct io so_RPI10;
extern struct io so_RPI11;
extern struct io so_RPI12;
extern struct io so_RPI13;
extern struct io so_RPI14;
extern struct io so_RPI15;
extern struct io so_RPI16;
extern struct io so_RPI17;
extern struct io so_RPI18;
extern struct io so_RPI19;
extern struct io so_RPI20;
extern struct io so_RPI21;

extern struct io so_SERV;
extern struct io so_SERVC;

extern struct io si_SECUR;
extern struct io si_RSECU;
extern struct io si_OSEC;
extern struct io si_ORSEC;
extern struct io si_ALTPI;

extern struct io so_MESO;
extern struct io so_ALTO;
extern struct io si_SMK[fl_size];		

extern struct io si_SCD;		// Security Disabling Input Board Address
extern struct io si_RSPD;		// Reduced Speed Input Data Array Number

extern struct io si_RDZE;		// Rear Door Zone Door Open Enable Input Board Address
extern struct io so_PDCA;		// Peelle Door Close Alarm Output Board Address
extern struct io so_PFDCA;		// Peelle Force Door Close Alarm Output Board Address
extern struct io so_FETST;		// Freight Front Electric Eye Test Output Board Address
extern struct io so_RETST;		// Freight Rear Electric Eye Test Output Board Address

extern struct io si_FETST;		// Freight Front Electric Eye Test Input Board Address
extern struct io si_RETST;		// Freight Rear Electric Eye Test Input Board Address
extern struct io si_EE2;		// Electric Eye Input Board Address
extern struct io si_EER2;		// Electric Eye Input Board Address
extern struct io si_HDOB;		// Hall Door Open Button Input Board Address
extern struct io si_HDCB;		// Hall Door Close Button Input Board Address
extern struct io si_HDOBR;		// Hall Door Open Button Rear Input Board Address
extern struct io si_HDCBR;		// Hall Door Close Button Input Rear Board Address

extern struct io so_OSERD;		// Out of service DOB stuck door open Output Board Address
extern struct io so_ADOF[fl_size];	// Auto Swing Door Pulse Ouput Board Address 	ado #1 
extern struct io so_ADOR[fl_size];	// Rear Auto Swing Door Pulse Ouput Board Address 	ado #1 



//ZYM 10/18/2017
//input
extern struct io si_MLi0;
extern struct io si_MLi1;
extern struct io si_MLi2;
extern struct io si_MLi3;
extern struct io si_MLi4;
extern struct io si_MLi5;
extern struct io si_MLi6;
extern struct io si_MLi7;
extern struct io si_MLi8;
extern struct io si_MLi9;
extern struct io si_MLi10;
extern struct io si_MLi11;

//output
extern struct io so_MLo0;
extern struct io so_MLo1;
extern struct io so_MLo2;
extern struct io so_MLo3;
extern struct io so_MLo4;
extern struct io so_MLo5;
extern struct io so_MLo6;
extern struct io so_MLo7;
extern struct io so_MLo8;
extern struct io so_MLo9;
extern struct io so_MLo10;
extern struct io so_MLo11;

//input
extern struct io si_MRi0;
extern struct io si_MRi1;
extern struct io si_MRi2;
extern struct io si_MRi3;
extern struct io si_MRi4;
extern struct io si_MRi5;
extern struct io si_MRi6;
extern struct io si_MRi7;
extern struct io si_MRi8;
extern struct io si_MRi9;
extern struct io si_MRi10;
extern struct io si_MRi11;

//output
extern struct io so_MRo0;
extern struct io so_MRo1;
extern struct io so_MRo2;
extern struct io so_MRo3;
extern struct io so_MRo4;
extern struct io so_MRo5;
extern struct io so_MRo6;
extern struct io so_MRo7;
extern struct io so_MRo8;
extern struct io so_MRo9;
extern struct io so_MRo10;
extern struct io so_MRo11;


//ZYM 10/20/2017
//input
extern struct io si_suf;
extern struct io si_sufc;
extern struct io si_su;
extern struct io si_suc;
extern struct io si_sdf;
extern struct io si_sdfc;
extern struct io si_sd;
extern struct io si_sdc;
extern struct io si_s6s10;

//output
extern struct io so_S5LPS;
extern struct io so_S4LOS;
extern struct io so_S3TPL;



//input
extern struct io si_s2mrw;
extern struct io si_24s10;
extern struct io si_22gov;
extern struct io si_20tlh;
extern struct io si_h18ps;
extern struct io si_16mrw;

//output
extern struct io so_S1TPH;
extern struct io so_23GOV;
extern struct io so_21TLH;
extern struct io so_H19PS;
extern struct io so_17HSS;



//input
extern struct io si_14mrw;
extern struct io si_11mrw;
extern struct io si_h8s10;
extern struct io si_h6s10;
extern struct io si_h4s10;

//output
extern struct io so_15TSD;
extern struct io so_13BAU;
extern struct io so_12BAD;
extern struct io so_10TAU;
extern struct io so_H9TAD;
extern struct io so_H7DLB;
extern struct io so_H5DLM;




//input
extern struct io si_h2s10;
extern struct io si_mrsw;
extern struct io si_mc;
extern struct io si_mc1;
extern struct io si_ste;
extern struct io si_s10;

//output
extern struct io so_H3DLT;
extern struct io so_RLM;
//-extern struct io so_MC1;
extern struct io so_0MTO;
extern struct io so_0SPD;
extern struct io so_0CFLT;
extern struct io so_TMRLY;


//input
extern struct io si_18hss;

//output
extern struct io so_17CST;
extern struct io so_16INS;
extern struct io so_T15IU;
extern struct io so_T14ID;
extern struct io so_13IEN;
extern struct io so_12ICA;
extern struct io so_11ICI;
extern struct io so_10ACC;
extern struct io so_T9GS;
extern struct io so_RGS;
extern struct io so_RDRLY;

//*********************************************************************************************
// Car Top Can I/O
//*********************************************************************************************

extern struct io si_RLS;
extern struct io si_CSS;
extern struct io si_CTS;
extern struct io si_CST;
extern struct io si_DET;
extern struct io si_RDET;
extern struct io si_HWSET; 	// RUN/SETUP JUMPER

extern struct io so_IFL;	// Inspection Fire Light
extern struct io so_IFB;	// Inspection Fire Buzzer


extern struct io so_CUL;	// Car Up lantern
extern struct io so_CDL;	// Car Down lantern
extern struct io so_CLF;	// Cab Light and Fan
extern struct io so_FAN;	// cab Fan
extern struct io so_LIGHT;	// Cab Light
extern struct io so_SELCM;	// Selector Comm LED
extern struct io so_NTSCM;	// NTS Comm LED


extern struct io si_DT; 	// Down Terminal Slowdosn
extern struct io si_DT1; 	// 1st Down Terminal limit
extern struct io si_DT2; 	// 2nd Down Terminal limit
extern struct io si_DT3; 	// 3rd Down Terminal limit
extern struct io si_DT4; 	// 4th Down Terminal limit
extern struct io si_DT5; 	// 5th Down Terminal limit
extern struct io si_DT6; 	// 6th Down Terminal limit

extern struct io si_UT; 	// Up Terminal Slowdown
extern struct io si_UT1;	// 1st Up Terminal limit
extern struct io si_UT2;	// 2nd Up Terminal limit
extern struct io si_UT3; 	// 3rd Up Terminal limit
extern struct io si_UT4; 	// 4th Up Terminal Limit
extern struct io si_UT5; 	// 5th Up Terminal limit
extern struct io si_UT6; 	// 6th Up Terminal Limit

extern struct io si_DL;  	// Down Level Input
extern struct io si_DZA; 	// Door Zone A input (DZU OR DZD)
extern struct io si_UL; 	// Up level input
extern struct io si_DZsel;	// Door Zone Selector Count 

extern struct io si_UL1; 	// Up level input
extern struct io si_UL2; 	// Up level input
extern struct io si_UL3; 	// Up level input
extern struct io si_UL4; 	// Up level input

extern struct io si_DL1;  	// Down Level Input
extern struct io si_DL2;  	// Down Level Input
extern struct io si_DL3;  	// Down Level Input
extern struct io si_DL4;  	// Down Level Input

extern struct io si_DZ1A;	// Door Zone Sensor input
extern struct io si_DZ2A;	// Door Zone Sensor input
extern struct io si_DZ1B;	// Door Zone Sensor input
extern struct io si_DZ2B;	// Door Zone Sensor input
extern struct io si_UNL;	// Up Normal Latch
extern struct io si_UNsel;	// Up Normal Selector Count
extern struct io si_UTsel;	// Up Terminal Slowdown Selector Count
extern struct io si_UTSsn;	// Up Emergency Terminal Slowdown Sensor
extern struct io si_DZU;	// Up Door Zone Sensor input

extern struct io si_DNL;	// Down Normal Latch
extern struct io si_DNsel;	// Down Normal Selector Count
extern struct io si_DTsel;	// Down Terminal Slowdown Selector Count
extern struct io si_DTSsn;	// Down Emergency Terminal Slowdown Sensor
extern struct io si_DZD;	// Down Door Zone Sensor input


extern struct io si_SE;
extern struct io si_SER;
extern struct io si_EE;
extern struct io si_EER;
extern struct io si_DCL;
extern struct io si_DOL;
extern struct io si_DCLR;
extern struct io si_DOLR;
extern struct io si_ALRM;
extern struct io si_ALRMR;
extern struct io si_EMS;


extern struct io si_movfr_DCL;
extern struct io si_movfr_DOL;
extern struct io si_movfr_EE;
extern struct io si_movfr_DPM;
extern struct io si_movfr_DCLR;
extern struct io si_movfr_DOLR;
extern struct io si_movfr_EER;
extern struct io si_movfr_RPM;


extern struct io si_std_DCL;
extern struct io si_std_DOL;
extern struct io si_std_EE;
extern struct io si_std_DPM;
extern struct io si_std_DCLR;
extern struct io si_std_DOLR;
extern struct io si_std_EER;
extern struct io si_std_RPM;


extern struct io si_DPM;
extern struct io si_RPM;

extern struct io si_DOB;
extern struct io si_DCB;
extern struct io si_DOBR;
extern struct io si_DCBR;

extern struct io si_FS2;
extern struct io si_FS2H;
extern struct io si_FS2C;

extern struct io si_FiDOB;
extern struct io si_FiDOBR;
extern struct io si_FiFS2C;
extern struct io si_FiDCB;
extern struct io si_FiDCBR;

extern struct io si_ICID;
extern struct io si_ICIU;
extern struct io si_ED;
extern struct io si_EDHL;
extern struct io si_EDR;
extern struct io si_OVL;
extern struct io si_LWA;
extern struct io si_LWB;
extern struct io si_LWD;

extern struct io si_ATT;
extern struct io si_ATTUP;
extern struct io si_ATTDN;
extern struct io si_ATTBY;
extern struct io si_ATTHC;


extern struct io si_DZSA;
extern struct io si_UNS;
extern struct io si_DNS;
extern struct io si_DLW;
extern struct io si_HBE;
extern struct io si_HBER;


extern struct io si_INDC;
extern struct io si_INDS;
extern struct io si_IDSO;
extern struct io si_ICVIP;
extern struct io si_SCS;
extern struct io si_SCSR;
extern struct io si_ZON;


extern struct io si_DS;
extern struct io si_US;

							      																		
extern struct io so_DO;
extern struct io so_DC;
extern struct io so_NUD;
extern struct io so_DOR;
extern struct io so_DCR;
extern struct io so_NUDR;
extern struct io so_DZDE;
extern struct io so_DTR;
extern struct io so_SFST;
extern struct io so_HEOFL;
extern struct io so_CULR;
extern struct io so_CDLR;
extern struct io so_RCM;
extern struct io so_FL;
extern struct io so_FS1L;
extern struct io so_FS2L;
extern struct io so_F1ML;
extern struct io so_F1AL;
extern struct io so_EML;
extern struct io so_ATDL;
extern struct io so_ATUL;
extern struct io so_EQLC;
extern struct io so_EDL;

extern struct io so_std_DO;
extern struct io so_std_DC;
extern struct io so_std_NUD;
extern struct io so_movfr_DO;
extern struct io so_movfr_DC;
extern struct io so_movfr_NUD;

extern struct io so_std_DOR;
extern struct io so_std_DCR;
extern struct io so_std_NUDR;
extern struct io so_movfr_DOR;
extern struct io so_movfr_DCR;
extern struct io so_movfr_NUDR;

extern struct io so_PIO[fl_size];
extern struct io so_PICO[fl_size];

extern struct io so_ULO[fl_size];
extern struct io so_DLO[fl_size];
extern struct io so_ULRO[fl_size];
extern struct io so_DLRO[fl_size];

extern struct io so_UALO[fl_size];
extern struct io so_DALO[fl_size];
extern struct io so_UALRO[fl_size];
extern struct io so_DALRO[fl_size];
		
	
extern struct io si_CCI[fl_size];
extern struct io so_CCO[fl_size];
extern struct io si_CCRI[fl_size];
extern struct io so_CCRO[fl_size];
extern struct io si_CCSI[fl_size];
extern struct io si_OCSI[fl_size];
extern struct io si_RCCSI[fl_size];
extern struct io si_ORSI[fl_size];
extern struct io si_RM_CCI[fl_size];
extern struct io so_RM_CCO[fl_size];
extern struct io si_RM_CCRI[fl_size];
extern struct io so_RM_CCRO[fl_size];
extern struct io so_SCCO[fl_size];
extern struct io so_SCCRO[fl_size];

extern struct io si_DSI;		// Door Stop Input Board Address
extern struct io si_RDSI;		// Rear Door Stop Input Board Address
extern struct io si_CEOF;		// Car Elevator Off Input Board Address
extern struct io si_CEOFR;		// Car Elevator Off Reset Board Address
extern struct io so_CEOFL;		// Car Elevator Off Light Board Address
extern struct io si_ATHCD;		// Attendant Hall Call Respond Disable Input Board Address


//*******************************
//  Aux Car Station
//*******************************

extern struct io si_XCCI[fl_size];
extern struct io so_XCCO[fl_size];
extern struct io si_XCCRI[fl_size];
extern struct io so_XCCRO[fl_size];

extern struct io si_XDOB;
extern struct io si_XDCB;
extern struct io si_XDOBR;
extern struct io si_XDCBR;
extern struct io si_XALRM;
extern struct io si_XHBE;
extern struct io si_XED;
extern struct io si_XINDC;
extern struct io si_XALRMR;
extern struct io si_XHBER;
extern struct io si_XEDR;


extern struct io si_UC[fl_size];
extern struct io so_UCL[fl_size];
extern struct io si_DC[fl_size];
extern struct io so_DCL[fl_size];

extern struct io si_RUC[fl_size];
extern struct io so_RUCL[fl_size];
extern struct io si_RDC[fl_size];
extern struct io so_RDCL[fl_size];

extern struct io si_IRUC[fl_size];
extern struct io so_IRUCL[fl_size];
extern struct io si_IRDC[fl_size];
extern struct io so_IRDCL[fl_size];

extern struct io si_RIRUC[fl_size];
extern struct io so_RIRUCL[fl_size];
extern struct io si_RIRDC[fl_size];
extern struct io so_RIRDCL[fl_size];

extern struct io si_TRUC[fl_size];
extern struct io so_TRUCL[fl_size];
extern struct io si_TRDC[fl_size];
extern struct io so_TRDCL[fl_size];

extern struct io si_XUC[fl_size];
extern struct io so_XUCL[fl_size];
extern struct io si_XDC[fl_size];
extern struct io so_XDCL[fl_size];

extern struct io si_XRUC[fl_size];
extern struct io so_XRUCL[fl_size];
extern struct io si_XRDC[fl_size];
extern struct io so_XRDCL[fl_size];

extern struct io si_UB[fl_size];
extern struct io so_UBL[fl_size];
extern struct io si_DB[fl_size];
extern struct io so_DBL[fl_size];

extern struct io si_URB[fl_size];
extern struct io so_URBL[fl_size];
extern struct io si_DRB[fl_size];
extern struct io so_DRBL[fl_size];

extern struct io si_CB[fl_size];
extern struct io so_CBL[fl_size];
extern struct io si_RCB[fl_size];
extern struct io so_RCBL[fl_size];

extern struct io si_CB_SR[fl_size];
extern struct io so_CB_SRL[fl_size];
extern struct io si_RCB_SR[fl_size];
extern struct io so_RCB_SRL[fl_size];

extern struct io si_VIP[fl_size];
extern struct io so_VIPL[fl_size];

extern struct io si_RVIP[fl_size];
extern struct io so_RVIPL[fl_size];

// Car Call lockout from the group
extern struct io si_GCSEC;
extern struct io si_RGCSC; 
extern struct io si_GCS[fl_size];
extern struct io si_RGCS[fl_size];

// Car Call Security Override from the group
extern struct io si_GCSO; 
extern struct io si_RGCSO; 
extern struct io si_GOS[fl_size];
extern struct io so_GOSO[fl_size];
extern struct io si_RGOS[fl_size];
extern struct io so_RGOSO[fl_size];

// Floor (hall and car call) lockout from the group
extern struct io si_FLSEC; 
extern struct io si_RFLSC; 
extern struct io si_FLS[fl_size];
extern struct io si_RFLS[fl_size];

// Hall Call lockout from the group
extern struct io si_HSEC; 
extern struct io si_RHSEC; 
extern struct io si_HCS[fl_size];
extern struct io si_RHCS[fl_size];

// Serial Hall Call lockout from the group
extern struct io si_SHSEC; 
extern struct io si_RSHSEC; 
extern struct io si_SHS[fl_size];
extern struct io si_RSHS[fl_size];

// Serial Hall Call lockout Light from the group
extern struct io so_SHSL[fl_size];
extern struct io so_RSHSL[fl_size];

//  Hugs security inputs
extern struct io si_HUGSEC;
extern struct io si_RHUGSEC; 
extern struct io si_HGS[fl_size];
extern struct io si_RHGS[fl_size];

// Sabbath Floor Enable from Group
extern struct io si_SABF; 
extern struct io si_RSABF; 
extern struct io si_SBF[fl_size];
extern struct io si_SBR[fl_size];


// Group I/O
extern struct io si_HC;
extern struct io si_ICR; 
extern struct io si_ICR2; 
extern struct io si_RICR; 

extern struct io si_LUC;
extern struct io so_LUCL;

extern struct io si_CASEC; 
extern struct io si_APRK; 
extern struct io si_NCU; 
extern struct io si_ALBY; 

extern struct io si_AUTEP;
extern struct io si_RECEN;
extern struct io si_RUNEN;
extern struct io so_RUNCT;
extern struct io so_EPRCLF;

extern struct io si_CBSR; 
extern struct io si_RCBSR; 
extern struct io si_TR; 

extern struct io si_SECF1;
extern struct io si_SECF2;
extern struct io si_SECF3;
extern struct io si_TMREN;
extern struct io so_GSERV;
 
extern struct io so_PRK[car_size];
extern struct io so_OP[car_size];
extern struct io so_SEL[car_size];

extern struct io si_NCA;

extern struct io si_CYCLE;		// Second Riser Cycling Enable Input Board Address
extern struct io so_CYCLO;		// Second Riser Cycling Output Board Address
extern struct io si_VPECC;    	// Group VIP Car Calls Enable Input Board Address


#define giodata(i) iodata[0][i]

extern unsigned char IO_In_Out[nmb_io_arrays];
extern unsigned char ioset[nmb_io_arrays];
extern unsigned char ioclr[nmb_io_arrays];
extern unsigned char iodata[9][nmb_io_arrays];
extern unsigned char IO_board[car_size][nmb_io_brds];

extern int16 brdadd[6][3];
extern int16 brdtype[nmb_io_brds];
extern int16 HC_IO_board[nmb_hcio_brds];

extern int16 io_bd_order[86];

extern char  ioname [][5];
extern unsigned char ionamechksum[nmb_io_brds];
extern unsigned char  io_valid[];
extern unsigned char  io_send_name[];
extern unsigned char io_on_color_ix[];
extern char  hcioname [][5];
extern unsigned char  gio_valid[];
extern unsigned char GIO_In_Out[];
extern unsigned char  gio_send_name[];


extern void io_init(void);
extern void init_iodata(void);
extern int16 count_bit(unsigned char bitnmb);


/*
//ZYM 10/9/2017
extern struct io si_TESi0;
extern struct io si_TESi1;
extern struct io si_TESi2;
extern struct io si_TESi3;
extern struct io si_TESi4;
extern struct io si_TESi5;
extern struct io si_TESi6;
extern struct io si_TESi7;
extern struct io si_TESi8;
extern struct io si_TESi9;
extern struct io si_TEi10;
extern struct io si_TEi11;

extern struct io so_TESo0;
extern struct io so_TESo1;
extern struct io so_TESo2;
extern struct io so_TESo3;
extern struct io so_TESo4;
extern struct io so_TESo5;
extern struct io so_TESo6;
extern struct io so_TESo7;
extern struct io so_TESo8 ;
extern struct io so_TESo9;
extern struct io so_TEo10;
extern struct io so_TEo11;

extern struct io so_GSPO1;
extern struct io so_GSPO2;
extern struct io so_GSPO3;
*/
//ZYM 10/9/2017
//input
extern struct io si_GRi0;
extern struct io si_GRi1;
extern struct io si_GRi2;
extern struct io si_GRi3;
extern struct io si_GRi4;
extern struct io si_GRi5;
extern struct io si_GRi6;
extern struct io si_GRi7;
extern struct io si_GRi8;
extern struct io si_GRi9;
extern struct io si_GRi10;
extern struct io si_GRi111;

//output
extern struct io so_GRo0;
extern struct io so_GRo1;
extern struct io so_GRo2;
extern struct io so_GRo3;
extern struct io so_GRo4;
extern struct io so_GRo5;
extern struct io so_GRo6;
extern struct io so_GRo7;
extern struct io so_GRo8;
extern struct io so_GRo9;
extern struct io so_GRo10;
extern struct io so_GRo11;

//ZYM 10/16/2017
//input
extern struct io si_CTi0;
extern struct io si_CTi1;
extern struct io si_CTi2;
extern struct io si_CTi3;
extern struct io si_CTi4;
extern struct io si_CTi5;
extern struct io si_CTi6;
extern struct io si_CTi7;
extern struct io si_CTi8;
extern struct io si_CTi9;
extern struct io si_CTi10;
extern struct io si_CTi11;

//output
extern struct io so_CTo0;
extern struct io so_CTo1;
extern struct io so_CTo2;
extern struct io so_CTo3;
extern struct io so_CTo4;
extern struct io so_CTo5;
extern struct io so_CTo6;
extern struct io so_CTo7;
extern struct io so_CTo8;
extern struct io so_CTo9;
extern struct io so_CTo10;
extern struct io so_CTo11;
