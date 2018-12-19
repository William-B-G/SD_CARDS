//__declspec(section ".rodata")

// Bit Address of I/O on Board #1

extern int16 DLB_;			// Door Lock Bottom Input Board Address
extern uint8 DLB_B;			// Door Lock Bottom Input Bit Number
extern uint8 DLB_D;  		// Door Lock Bottom Input Data Array Number

extern int16 DLM;    		// Door Lock Middle Input Board Address
extern uint8 DLMB;     		// Door Lock Middle Input Bit Number
extern uint8 DLMD;       	// Door Lock Middle Input Data Array Number

extern int16 DLT;      		// Door Lock Top Input Board Address
extern uint8 DLTB;     		// Door Lock Top Input Bit Number
extern uint8 DLTD;       	// Door Lock Top Input Data Array Number

extern int16 RLM; 			// Rear Lock Middle Input Board Address
extern uint8 RLMB;			// Rear Lock Middle Input Bit Number
extern uint8 RLMD;  		// Rear Lock Middle Input Data Array Number

extern int16 DLB_1; 		// Door Lock Bottom - Secondary Input Board Address
extern uint8 DLB_1B;		// Door Lock Bottom - Secondary Input Bit Number
extern uint8 DLB_1D;    	// Door Lock Bottom - Secondary Input Data Array Number

extern int16 DLM_1;  		// Door Lock Middle - Secondary Input Board Address
extern uint8 DLM_1B;		// Door Lock Middle - Secondary Input Bit Number
extern uint8 DLM_1D; 		// Door Lock Middle - Secondary Input Data Array Number

extern int16 DLT_1;			// Door Lock Top - Secondary Input Board Address
extern uint8 DLT_1B;		// Door Lock Top - Secondary Input Bit Number
extern uint8 DLT_1D;	   	// Door Lock Top - Secondary Input Data Array Number

extern int16 RLM_1;	  		// Rear Lock Middle - Secondary Input Board Address
extern uint8 RLM_1B;		// Rear Lock Middle - Secondary Input Bit Number
extern uint8 RLM_1D;    	// Rear Lock Middle - Secondary Input Data Array Number

extern int16 ACC;			// Access Input Board Address
extern uint8 ACCB;			// Access Input Bit Number
extern uint8 ACCD;   		// Access Input Data Array Number

extern int16 BAD;			// Bottom Access Down Input Board Address
extern uint8 BADB;			// Bottom Access Down Input Bit Number
extern uint8 BADD;   		// Bottom Access Down Input Data Array Number

extern int16 BAU;			// Bottom Access Up Input Board Address
extern uint8 BAUB;			// Bottom Access Up Input Bit Number
extern uint8 BAUD;   		// Bottom Access Up Input Data Array Number

extern int16 TAD;			// Top Access Down Input Board Address
extern uint8 TADB;			// Top Access Down Input Bit Number
extern uint8 TADD;   		// Top Access Down Input Data Array Number



// Bit Address of I/O on Board #2

extern int16 TAU;			// Top Access Up Input Board Address
extern uint8 TAUB;			// Top Access Up Input Bit Number
extern uint8 TAUD;   		// Top Access Up Input Data Array Number

extern int16 INE;    	 	// Inspection Switch Enable Input Board Address
extern uint8 INEB;  	   	// Inspection Switch Enable Input Bit Number
extern uint8 INED;       	// Inspection Switch Enable Input Data Array Number

extern int16 SFC;   	  	// Secondary Fault Control Input Board Address
extern uint8 SFCB;  	   	// Secondary Fault Control Input Bit Number
extern uint8 SFCD;       	// Secondary Fault Control Input Data Array Number

extern int16 HC;	       	// Group Power Input Board Address
extern uint8 HCB;        	// Group Power Input Bit Number
extern uint8 HCD;        	// Group Power Input Data Array Number

// Note: Same input as SS - SS moved to CWS when FFS is used.

extern int16 FFS;	     	// Fire Fighters Stop Switch Input Board Address
extern uint8 FFSB; 	    	// Fire Fighters Stop Switch Input Bit Number
extern uint8 FFSD;      	// Fire Fighters Stop Switch Input Data Array Number

#if ((Traction == 0) && (Tract_OL == 0))
extern int16 TPH;      		// Temperature High Input Board Address
extern uint8 TPHB;       	// Temperature High Input Bit Number
extern uint8 TPHD;          // Temperature High Input Data Array Number
#endif

// Note: same input as PREVIOUS CWS

extern int16 CTS;       	// Car Top Inspecion Switch Input Board Address
extern uint8 CTSB; 	    	// Car Top Inspecion Switch Input Bit Number
extern uint8 CTSD;      	// Car Top Inspecion Switch Input Data Array Number

#if ((Traction == 1) || (Tract_OL == 1))
	// Must not conflict with DT5
	extern int16 SABO;   	  	// sabbath On Output Board Address
	extern uint8 SABOB;      	// sabbath On Output Bit Number
	extern uint8 SABOD;  	    // sabbath On Output Data Array Number

#endif

extern int16 EDHL;				// Extended Door Time time Hall Input Data Array Number
extern uint8 EDHLB;				// Extended Door Time Hall Input Data Array Number
extern uint8 EDHLD; 			// Extended Door Time Hall Input Data Array Number


// Bit Address of I/O on Board #3

extern int16 GBP;			// Gate Bypass Input Board Address
extern uint8 GBPB;			// Gate Bypass Input Bit Number
extern uint8 GBPD;  		// BGate Bypass Input Data Array Number

extern int16 LBP;			// Lock Bypass Input Board Address
extern uint8 LBPB;			// Lock Bypass Input Bit Number
extern uint8 LBPD;  		// Lock Bypass Input Data Array Number

extern int16 IND;		    // Independent Input Board Address
extern uint8 INDB;       	// Independent Input Bit Number
extern uint8 INDD;   	    // Independent Input Data Array Number

extern int16 AD;      		// Automatic Doors Input Board Address
extern uint8 ADB;       	// Automatic Doors Input Bit Number
extern uint8 ADD;     	    // Automatic Doors Input Data Array Number

extern int16 DEL;     	 	// Delta Contactor Input Board Address
extern uint8 DELB;       	// Delta Contactor Input Bit Number
extern uint8 DELD;   	     // Delta Contactor Input Data Array Number

#if ((Tract_OL == 1) || (Traction == 1))
 	extern int16 BKS;    	 	// Brake Switch Input Board Address
 	extern uint8 BKSB;     	// Brake Switch Input Bit Number
 	extern uint8 BKSD;   	    // Brake Switch Input Data Array Number
#else
	//(tpl AND BKS) mapped the same
	extern int16 TPL;   	   	// Temperature Low Input Board Address
	extern uint8 TPLB;      	// Temperature Low Input Bit Number
	extern uint8 TPLD;    	    // Temperature Low Input Data Array Number
#endif

extern int16 GS;    		// Gate Switch Input Board Address
extern uint8 GSB;     		// Gate Switch Input Bit Number
extern uint8 GSD;       	// Gate Switch Input Data Array Number

extern int16 GS_1;			// Gate Switch - Secondary Input Board Address
extern uint8 GS_1B;			// Gate Switch - Secondary Input Bit Number
extern uint8 GS_1D; 		// Gate Switch - Secondary Input Data Array Number

extern int16 LC;     	 	// Logic Power Input Board Address
extern uint8 LCB;        	// Logic Power Input Bit Number
extern uint8 LCD;      		// Logic Power Input Data Array Number

extern int16 INS;      		// Inspection Input Board Address
extern uint8 INSB;       	// Inspection Input Bit Number
extern uint8 INSD;      	// Inspection Input Data Array Number

extern int16 UPML;     		// UP Micro Leveling Output Board Address
extern uint8 UPMLB;      	// UP Micro Leveling Output Bit Number
extern uint8 UPMLD;       	// UP Micro Leveling Output Data Array Number

extern int16 DNML;     		// DN Micro Leveling Output Board Address
extern uint8 DNMLB;      	// DN Micro Leveling Output Bit Number
extern uint8 DNMLD;       	// DN Micro Leveling Output Data Array Number

extern int16 FSDO;     		// Front Swing Door Open Output Board Address
extern uint8 FSDOB;      	// Front Swing Door Open Output Bit Number
extern uint8 FSDOD;       	// Front Swing Door Open Output Data Array Number

extern int16 RSDO;     		// Rear Swing Door Open Output Board Address
extern uint8 RSDOB;      	// Rear Swing Door Open Output Bit Number
extern uint8 RSDOD;			// Rear Swing Door Open Output Data Array Number


// Bit Address of I/O on Board #4

extern int16 IU;      		// Inspection Up Input Board Address
extern uint8 IUB;       	// Inspection Up Input Bit Number
extern uint8 IUD;         	// Inspection Up Input Data Array Number

extern int16 ID;      		// Inspection Down Input Board Address
extern uint8 IDB;       	// Inspection Down Input Bit Number
extern uint8 IDD;         	// Inspection Down Input Data Array Number

extern int16 STP;     		// Stop Switch Input Board Address
extern uint8 STPB;       	// Stop Switch Input Bit Number
extern uint8 STPD;        	// Stop Switch Input Data Array Number

extern int16 ICI;			// In-Car Inspection Input Board Address
extern uint8 ICIB;			// In-Car Inspection Input Bit Number
extern uint8 ICID;  		// In-Car Inspection Input Data Array Number

extern int16 MRIN;			// Motor Room Inspection Input Board Address
extern uint8 MRINB;			// Motor Room Inspection Input Bit Number
extern uint8 MRIND;  		// Motor Room Inspection Input Data Array Number

extern int16 MRIU;			// Motor Room Inspection Up Input Board Address
extern uint8 MRIUB;			// Motor Room Inspection Up Input Bit Number
extern uint8 MRIUD;  		// Motor Room Inspection Up Input Data Array Number

extern int16 MRID;			// Motor Room Inspection Down Input Board Address
extern uint8 MRIDB;			// Motor Room Inspection Down Input Bit Number
extern uint8 MRIDD;  		// Motor Room Inspection Down Input Data Array Number

extern int16 AUTO;			// Auto Input Board Address
extern uint8 AUTOB;			// Auto Down Run Input Bit Number
extern uint8 AUTOD;  		// Auto Down Run Input Data Array Number

extern int16 P;   	  	 	// Potential Input Board Address
extern uint8 PB;      	 	// Potential Input Bit Number
extern uint8 PD;         	// Potential Input Data Array Number

extern int16 FSTI;			// Fire Stop Switch Bypass Input Board Address
extern uint8 FSTIB;			// Fire Stop Switch Bypass Input Bit Number
extern uint8 FSTID;  		// Fire Stop Switch Bypass Input Data Array Number

extern int16 GTS;			// Gripper Trip Switch Input Board Address
extern uint8 GTSB;			// Gripper Trip Switch Input Bit Number
extern uint8 GTSD;			// Gripper Trip Switch Input Data Array Number

extern int16 READY;    		// Ready Input Board Address
extern uint8 READYB;     	// Ready Input Bit Number
extern uint8 READYD;      	// Ready Input Data Array Number

extern int16 GRT1;			// Gripper Control 1 Input Board Address
extern uint8 GRT1B;			// Gripper Control 1 Input Bit Number
extern uint8 GRT1D;  		// Gripper Control 1 Input Data Array Number

extern int16 GRT2;			// Gripper Control 2 Input Board Address
extern uint8 GRT2B;			// Gripper Control 2 Input Bit Number
extern uint8 GRT2D;  		// Gripper Control 2 Input Data Array Number

extern int16 BRKI;			// Brake Control Input Board Address
extern uint8 BRKIB;			// Brake Control Input Bit Number
extern uint8 BRKID;  		// Brake Control Input Data Array Number

extern int16 DON;			// Drive On Input Board Address
extern uint8 DONB;			// Drive On Input Bit Number
extern uint8 DOND;  		// Drive On Input Data Array Number

extern int16 PFC;     		// Primary Fault Control Output Board Address
extern uint8 PFCB;      	// Primary Fault Control Output Bit Number
extern uint8 PFCD;       	// Primary Fault Control Output Data Array Number

extern int16 MCA;     	 	// Master Contactor Output Board Address
extern uint8 MCAB;       	// Master Contactor Output Bit Number
extern uint8 MCAD;       	// Master Contactor Output Data Array Number

extern int16 MCC;     		// Motor Contactor Control Output Board Address
extern uint8 MCCB;       	// Motor Contactor Control Output Bit Number
extern uint8 MCCD;        	// Motor Contactor Control Output Data Array Number

extern int16 RST;       	// Drive Reset Output Board Address
extern uint8 RSTB;       	// Drive Reset Output Bit Number
extern uint8 RSTD;        	// Drive Reset Output Data Array Number


// Bit Address of I/O on Board #5

extern int16 RUNI;			// Run Control Input Board Address
extern uint8 RUNIB;			// Run Control Input Bit Number
extern uint8 RUNID;  		// Run Control Input Data Array Number

extern int16 RUNAI; 		// Run Aux Control Input Board Address
extern uint8 RUNAIB;		// Run Aux Control Input Bit Number
extern uint8 RUNAID;   		// Run Aux Control Input Data Array Number

extern int16 MCCI;			// Motor Contactor Control Input Board Address
extern uint8 MCCIB;			// Motor Contactor Control Input Bit Number
extern uint8 MCCID;   		// Motor Contactor Control Input Data Array Number

extern int16 MCAI;			// Motor Contactor Input Board Address
extern uint8 MCAIB;			// Motor Contactor Input Bit Number
extern uint8 MCAID;			// Motor Contactor Input Data Array Number

extern int16 RGS;			// Rear Gate Input Board Address
extern uint8 RGSB;			// Rear Gate Input Bit Number
extern uint8 RGSD; 			// Rear Gate Input Data Array Number

extern int16 RGS_1;  		// Rear Gate Switch - Secondary Input Board Address
extern uint8 RGS_1B;   		// Rear Gate Switch - Secondary Input Bit Number
extern uint8 RGS_1D;     	// Rear Gate Switch - Secondary Input Data Array Number

extern int16 SECFM;			// Security Floor Mask Select Input Board Address
extern uint8 SECFMB;    	// Security Floor Mask Select Input Bit Number
extern uint8 SECFMD;     	// Security Floor Mask Select Input Data Array Number

extern int16 CCPBS;			// Car Call Push Button Security Input Board Address(spi14 Terminal)
extern uint8 CCPBSB;    	// Car Call Push Button Security Input Bit Number
extern uint8 CCPBSD;     	// Car Call Push Button Security Input Data Array Number


// Note: SAB uses the same io location as SECFM
extern int16 SAB;			// Sabbath Service Input Board Address
extern uint8 SABB;    		// Sabbath Service Input Bit Number
extern uint8 SABD;     		// Sabbath Service Input Data Array Number

// Note: INDS uses the same io location as SECFM
extern int16 INDS;			// Independent Security Input Board Address
extern uint8 INDSB;  	  	// Independent Security Input Bit Number
extern uint8 INDSD;   	 	// Independent Security Input Data Array Number

extern int16 LVC;			// Leveling Clamp Input Board Address
extern uint8 LVCB;			// Leveling Clamp Input Bit Number
extern uint8 LVCD;  		// Leveling Clamp Input Data Array Number

extern int16 DNI;			// Down Run Input Board Address
extern uint8 DNIB;			// Down Run Input Bit Number
extern uint8 DNID;  		// Down Run Input Data Array Number

extern int16 DFI;			// Down Fast Run Input Board Address
extern uint8 DFIB;			// Down Fast Input Bit Number
extern uint8 DFID;  		// Down Fast Input Data Array Number

extern int16 UPI;			// Up Run Input Board Address
extern uint8 UPIB;			// Up Run Input Bit Number
extern uint8 UPID; 			// Up Run Input Data Array Number

extern int16 UFI;			// Up Fast Run Input Board Address
extern uint8 UFIB;			// Up Fast Run Input Bit Number
extern uint8 UFID;  		// Up Fast Run Input Data Array Number

extern int16 GOV;     	 	// Governor Input Board Address
extern uint8 GOVB;       	// Governor Input Bit Number
extern uint8 GOVD;        	// Governor Input Data Array Number

extern int16 PS;   	   		// Pit Switch Input Board Address
extern uint8 PSB;       	// Pit Switch Input Bit Number
extern uint8 PSD;        	// Pit Switch Input Data Array Number

// Note: Same output as EMLH
extern int16 GBL;			// Gate Bypass light output Board Address
extern uint8 GBLB;			// Gate Bypass light output Bit Number
extern uint8 GBLD; 			// Gate Bypass light output Data Array Number

extern int16 EQL;			// Earthquake light output Board Address
extern uint8 EQLB;			// Earthquake light output Bit Number
extern uint8 EQLD;			// Earthquake light output Data Array Number

// Note: Same output as EQL
extern int16 LBL;			// Lock Bypass light output Board Address
extern uint8 LBLB;			// Lock Bypass light output Bit Number
extern uint8 LBLD;			// Lock Bypass light output Data Array Number

extern int16 FSO;    	 	// Fire Service Output Board Address
extern uint8 FSOB;      	// Fire Service Output Bit Number
extern uint8 FSOD;       	// Fire Service Output Data Array Number

// Note: LD and ISER and ELOO share the same output
extern int16 ELOO;      	// Elevator Off Output Board Address
extern uint8 ELOOB;      	// Elevator Off Output Bit Number
extern uint8 ELOOD;      	// Elevator Off Output Data Array Number

extern int16 LD;      		// Down Hall Lantern Output Board Address
extern uint8 LDB;    	  	// Down Hall Lantern Output Bit Number
extern uint8 LDD;        	// Down Hall Lantern Output Data Array Number

extern int16 TCU;			// This car up output Board Address
extern uint8 TCUB;			// This car up output Bit Number
extern uint8 TCUD;			// This car up output Data Array Number 

// Note: LU and TCU share the same output
extern int16 LU;      		// Up Hall Lantern Output Board Address
extern uint8 LUB;       	// Up Hall Lantern Output Bit Number
extern uint8 LUD;        	// Up Hall Lantern Output Data Array Number

extern int16 MST;   	   	// Motor Starter Y Delta Timer Output Board Address
extern uint8 MSTB;       	// Motor Starter Y Delta Timer Output Bit Number
extern uint8 MSTD;        	// Motor Starter Y Delta Timer Output Data Array Number

#if ((Traction == 1) || (Tract_OL == 1))

	// GOVR Same outpu as MST
	extern int16 GOVR;     	// Governor Reset Output Board Address
	extern uint8 GOVRB;      	// Governor Reset Output Bit Number
	extern uint8 GOVRD;       // Governor Reset Output Data Array Number

#else

	// GOVR Same outpu as EMLH
	extern int16 GOVR;     		// Governor Reset Output Board Address
	extern uint8 GOVRB;      	// Governor Reset Output Bit Number
	extern uint8 GOVRD;       	// Governor Reset Output Data Array Number

#endif

extern int16 RCF;      		// Retiring Cam for freight Output Board Address
extern uint8 RCFB;       	// Retiring Cam for freight Output Bit Number
extern uint8 RCFD;        	// Retiring Cam for freight Output Data Array Number

extern int16 GEN;      		// Generator Output Board Address
extern uint8 GENB;       	// Generator Output Bit Number
extern uint8 GEND;        	// Generator Output Data Array Number

// Note: MTB uses the same io location as GEN
extern int16 MTB;	      	// Motor Blower Output Board Address
extern uint8 MTBB;       	// Motor Blower Output Bit Number
extern uint8 MTBD;       	// Motor Blower Output Data Array Number

// GOVT Same outpu as MTB
extern int16 GOVT;     		// Governor Trip Output Board Address
extern uint8 GOVTB;      	// Governor Trip Output Bit Number
extern uint8 GOVTD;       	// Governor Trip Output Data Array Number

extern int16 FF;       		// Field Force BRD ADD
extern uint8 FFB;        	// Field Force Bit Number
extern uint8 FFD;        	// Field Force Data Array Number



// Bit Address of I/O on Board #6

extern int16 RUNA;			// Run Aux Output Board Address
extern uint8 RUNAB;      	// Run Aux Output Bit Number
extern uint8 RUNAD;    	   	// Run Aux Output Data Array Number

extern int16 RUN;     		// Run Output Board Address
extern uint8 RUNB;      	// Run Output Bit Number
extern uint8 RUND;       	// Run Output Data Array Number

extern int16 GR1R;    	 	// Rope Gripper 1 Output Board Address
extern uint8 GR1RB;      	// Rope Gripper 1 Output Bit Number
extern uint8 GR1RD;       	// Rope Gripper 1 Output Data Array Number

extern int16 GR2R;	    	// Rope Gripper 2 Output Board Address
extern uint8 GR2RB;      	// Rope Gripper 2 Output Bit Number
extern uint8 GR2RD;       	// Rope Gripper 2 Output Data Array Number

extern int16 LE;       		// Level Enable Output Board Address
extern uint8 LEB;        	// Level Enable Output Bit Number
extern uint8 LED;         	// Level Enable Output Data Array Number

extern int16 LE1;      		// Level Enable - Secondary Output Board Address
extern uint8 LE1B;       	// Level Enable - Secondary Output Bit Number
extern uint8 LE1D;        	// Level Enable - Secondary Output Data Array Number

extern int16 FST;   		// Second Fire Stop Bypass Output Board Address
extern uint8 FSTB;     		// Second Fire Stop Bypass Output Bit Number
extern uint8 FSTD;      	// Second Fire Stop Bypass Output Data Array Number

extern int16 FSTP;     		// Fire Stop Bypass Output Board Address
extern uint8 FSTPB;      	// Fire Stop Bypass Output Bit Number
extern uint8 FSTPD;       	// Fire Stop Bypass Output Data Array Number

extern int16 DNR;       	// Down Relay Output Board Address
extern uint8 DNRB;       	// Down Relay Output Bit Number
extern uint8 DNRD;        	// Down Relay Output Data Array Number

extern int16 DF;       		// Down Fast Output Board Address
extern uint8 DFB;        	// Down Fast Output Bit Number
extern uint8 DFD;         	// Down Fast Output Data Array Number

extern int16 UP;       		// Up Relay Output Board Address
extern uint8 UPB;        	// Up Relay Output Bit Number
extern uint8 UPD;         	// Up Relay Output Data Array Number

extern int16 UPF;      		// Up Fast Output Board Address
extern uint8 UPFB;       	// Up Fast Output Bit Number
extern uint8 UPFD;        	// Up Fast Output Data Array Number

extern int16 BRK;      		// Brake Output Board Address
extern uint8 BRKB;       	// Brake Output Bit Number
extern uint8 BRKD;       	// Brake Output Data Array Number

extern int16 BRKC;     		// Brake Cooling Output Board Address
extern uint8 BRKCB;      	// Brake Cooling Output Bit Number
extern uint8 BRKCD;      	// Brake Cooling Output Data Array Number

extern int16 DBC;      		// Dynamic Braking Resistor Control Output Board Address
extern uint8 DBCB;       	// Dynamic Braking Resistor Control Output Bit Number
extern uint8 DBCD;        	// Dynamic Braking Resistor Control Output Data Array Number

extern int16 BKS1;			// Brake Lift switch 1 Input Board Address
extern uint8 BKS1B;			// Brake Lift switch 1 Input Bit Number
extern uint8 BKS1D;			// Brake Lift switch 1 Input Data Array Number

extern int16 BKS2;			// Brake Lift switch 2 Input Board Address
extern uint8 BKS2B;			// Brake Lift switch 2 Input Bit Number
extern uint8 BKS2D;			// Brake Lift switch 2 Input Data Array Number

extern int16 BDBS;			// Brake Drive Bypass Switch Input Board Address
extern uint8 BDBSB;			// Brake Drive Bypass Switch Input Bit Number
extern uint8 BDBSD;			// Brake Drive Bypass Switch Input Data Array Number

extern int16 XBKS1;			// Brake Lift switch 1 Input Board Address
extern uint8 XBKS1B;		// Brake Lift switch 1 Input Bit Number
extern uint8 XBKS1D;		// Brake Lift switch 1 Input Data Array Number

extern int16 XBKS2;			// Brake Lift switch 2 Input Board Address
extern uint8 XBKS2B;		// Brake Lift switch 2 Input Bit Number
extern uint8 XBKS2D;		// Brake Lift switch 2 Input Data Array Number

extern int16 XBDBS;			// Brake Drive Bypass Switch Input Board Address
extern uint8 XBDBSB;		// Brake Drive Bypass Switch Input Bit Number
extern uint8 XBDBSD;		// Brake Drive Bypass Switch Input Data Array Number


//*********************************************************************
//********** Serial Car Comm I/O 's  **********************************
//*********************************************************************


 // Bit Address for board # 8

extern int16 INSEC;  	  	// Door Open Light Output Board Address
extern uint8 INSECB;     	// Door Open Light Output Bit Number
extern uint8 INSECD;      	// Door Open Light Output Data Array Number

extern int16 OPENL;    		// Door Open Light Output Board Address
extern uint8 OPENLB;     	// Door Open Light Output Bit Number
extern uint8 OPENLD;      	// Door Open Light Output Data Array Number

extern int16 FSOCS;    		// Fire Service Output Board Address
extern uint8 FSOCSB;     	// Fire Service Output Bit Number
extern uint8 FSOCSD;      	// Fire Service Output Data Array Number


 // Bit Address for board # 9

extern int16 TF;			// Top Final Input Board Address
extern uint8 TFB;			// Top Final Input Bit Number
extern uint8 TFD;			// Top Final Input Data Array Number

extern int16 BF;			// Bottom Final Input Board Address
extern uint8 BFB;			// Bottom Final Input Bit Number
extern uint8 BFD;			// Bottom Final Input Data Array Number

extern int16 PDS;			// Pit Door Switch Input Board Address
extern uint8 PDSB;			// Pit Door Switch Bit Number
extern uint8 PDSD;			// Pit Door Switch Data Array Number

extern int16 BFS;			// Buffer Switch Input Board Address
extern uint8 BFSB;			// Buffer Switch Input Bit Number
extern uint8 BFSD;			// Buffer Switch Input Data Array Number

extern int16 CMP;			// Compensating Ropw Switch Input Board Address
extern uint8 CMPB;			// Compensating Ropw Switch Input Bit Number
extern uint8 CMPD;			// Compensating Ropw Switch Input Data Array Number

extern int16 TEE;			// Top Emergency Exit Switch Input Board Address
extern uint8 TEEB;			// Top Emergency Exit Switch Input Bit Number
extern uint8 TEED;			// Top Emergency Exit Switch Input Data Array Number

extern int16 SEE;			// Side Emergency Exit Switch Input Board Address
extern uint8 SEEB;			// Side Emergency Exit Switch Input Bit Number
extern uint8 SEED;			// Side Emergency Exit Switch Input Data Array Number

extern int16 CSS;			// Car Safety Switch Input Board Address
extern uint8 CSSB;			// Car Safety Switch Input Bit Number
extern uint8 CSSD;			// Car Safety Switch Input Data Array Number
						   
extern int16 CCP;			// Car Call Power Input Board Address
extern uint8 CCPB;			// Car Call Power Input Bit Number
extern uint8 CCPD;			// Car Call Power Input Data Array Number

extern int16 CCLP;			// Car Call Light Power Input Board Address
extern uint8 CCLPB;			// Car Call Light Power Input Bit Number
extern uint8 CCLPD;			// Car Call Light Power Input Data Array Number

extern int16 HCLP;			// Hall Call Light Power Input Board Address
extern uint8 HCLPB;			// Hall Call Light Power Input Bit Number
extern uint8 HCLPD;			// Hall Call Light Power Input Data Array Number

extern int16 LHCP;			// Lobby Hall Call Common Power Input Board Address
extern uint8 LHCPB;			// Lobby Hall Call Common Power Input Bit Number
extern uint8 LHCPD;			// Lobby Hall Call Common Power Input Data Array Number

extern int16 HMO;			// Hoistway Motor Overload Input Board Address
extern uint8 HMOB;			// Hoistway Motor Overload Input Bit Number
extern uint8 HMOD;			// Hoistway Motor Overload Input Data Array Number
	
extern int16 DMO_;			// Door Motor Overload Input Board Address
extern uint8 DMO_B;			// Door Motor Overload Input Bit Number
extern uint8 DMO_D;			// Door Motor Overload Input Data Array Number




// Board #12

extern int16 FSORS;     	// Fire Service Output Board Address
extern uint8 FSORSB;     	// Fire Service Output Bit Number
extern uint8 FSORSD;     	// Fire Service Output Data Array Number

// Top of Car I/O's

// Block 25

extern int16 BP1;			// Binary Position #1 Input Board Address
extern uint8 BP1B;			// Binary Position #1 Input Bit Number
extern uint8 BP1D; 			// Binary Position #1 Input Data Array Number

extern int16 BP2;			// Binary Position #2 Input Board Address
extern uint8 BP2B;			// Binary Position #2 Input Bit Number
extern uint8 BP2D; 			// Binary Position #2 Input Data Array Number

extern int16 BP4;			// Binary Position #4 Input Board Address
extern uint8 BP4B;			// Binary Position #4 Input Bit Number
extern uint8 BP4D;   		// Binary Position #4 Input Data Array Number

extern int16 BP8;    		// Binary Position #8 Input Board Address
extern uint8 BP8B;    	 	// Binary Position #8 Input Bit Number
extern uint8 BP8D;      	// Binary Position #8 Input Data Array Number
	
extern int16 BP16;   		// Binary Position #16 Input Board Address
extern uint8 BP16B;     	// Binary Position #16 Input Bit Number
extern uint8 BP16D;      	// Binary Position #16 Input Data Array Number

//extern uint8 EP;     		// Even Parity Input Board Address
//extern uint8 EPB;       	// Even Parity Input Bit Numbe	
//extern uint8 EPD;2          // Up Slowdown Input Data Array Number

extern int16 DZSA;      	// DZSA (both dz sensors ored) Input Board Address
extern uint8 DZSAB;     	// DZSA (both dz sensors ored) Input Bit Number	
extern uint8 DZSAD;       	// DZSA (both dz sensors ored) Input Data Array Number

extern int16 DNS;     		// DNS Dn normal limit Input Input Board Address
extern uint8 DNSB;       	// DNS Dn normal limit Input Input Bit Number	
extern uint8 DNSD;        	// DNS Dn normal limit Input Input Data Array Number

extern int16 UNS;     	 	// UNS Up normal limit Input Board Address
extern uint8 UNSB;       	// UNS Up normal limit Input Input Bit Number	
extern uint8 UNSD;        	// UNS Up normal limit Input Input Data Array Number

extern int16 BP32;   		// Binary Position #32 Input Board Address
extern uint8 BP32B;     	// Binary Position #32 Input Bit Number
extern uint8 BP32D;      	// Binary Position #32 Input Data Array Number

extern int16 US;      		// Up Slowdown Input Board Address
extern uint8 USB;       	// Up Slowdown Input Bit Numbe	
extern uint8 USD;        	// Up Slowdown Input Data Array Number

extern int16 DS;     	 	// Down Slowdown Input Board Address
extern uint8 DSB;       	// Down Slowdown Input Bit Number
extern uint8 DSD;        	// Down Slowdown Input Data Array Number

extern int16 SE;    		// Saftey Edge Input Board Address
extern uint8 SEB;        	// Saftey Edge Input Bit Number
extern uint8 SED;         	// Saftey Edge Input Data Array Number

extern int16 LWA;   		// Load Weighing Anti-nuisance Input Board Address
extern uint8 LWAB;	   	 	// Load Weighing Anti-nuisance Input Bit Number
extern uint8 LWAD;			// Load Weighing Anti-nuisance Input Data Array Number

extern int16 OVL;    	 	// Over Load Input Board Address
extern uint8 OVLB;   	  	// Over Load Input Bit Number
extern uint8 OVLD;			// Over Load Input Data Array Number

// Note: Same input as OVL
extern int16 ELOF;   		// Elevator Off Input Board Address
extern uint8 ELOFB;     	// Elevator Off Input Bit Number
extern uint8 ELOFD;			// Elevator Off Input Data Array Number

extern int16 LWB;			// Load Weighing Bypass Input Board Address
extern uint8 LWBB;			// Load Weighing Bypass Input Bit Number
extern uint8 LWBD;			// Load Weighing Bypass Input Data Array Number

extern int16 LWD;    	 	// Load Weighing Dispatch Input Board Address
extern uint8 LWDB;   	  	// Load Weighing Dispatch Input Bit Number
extern uint8 LWDD;      	// Load Weighing Dispatch Input Data Array Number

extern int16 FBNBCT;  	 	// Fire buzzer Output Board Address
extern uint8 FBNBCTB;    	// Fire buzzer Output Bit Number
extern uint8 FBNBCTD;   	// Fire buzzer Output Data Array Number

extern int16 FLCT;   		// Fire light Output Board Address
extern uint8 FLCTB;    		// Fire light Output Bit Number
extern uint8 FLCTD;      	// Fire light Output Data Array Number


// Block 22	  Peelle Door Operator

extern int16 INSP;     		// Inspection output for peelle door interface Board Address
extern uint8 INSPB;       	// Inspection output for peelle door interface Bit Number
extern uint8 INSPD;       	// Inspection output for peelle door interface Data Array Number

extern int16 ADO;      		// Automatic door open output for peelle door interface Board Address
extern uint8 ADOB;       	// Automatic door open output for peelle door interface Bit Number
extern uint8 ADOD;        	// Automatic door open output for peelle door interface Data Array Number

extern int16 PH1R;  	   	// Phase 1 recall output for peelle door interface Board Address
extern uint8 PH1RB;      	// Phase 1 recall output for peelle door interface Bit Number
extern uint8 PH1RD;       	// Phase 1 recall output for peelle door interface Data Array Number

extern int16 DESL;   	 	// Designated landing output for peelle door interface Board Address
extern uint8 DESLB;      	// Designated landing output for peelle door interface Bit Number
extern uint8 DESLD;       	// Designated landing output for peelle door interface Data Array Number

extern int16 PH2ON;     	// Phase 2 on output for peelle door interface Board Address
extern uint8 PH2ONB;      	// Phase 2 on output for peelle door interface Bit Number
extern uint8 PH2OND;      	// Phase 2 on output for peelle door interface Data Array Number

extern int16 PH2HD;     	// Phase 2 hold output for peelle door interface Board Address
extern uint8 PH2HDB;     	// Phase 2 hold output for peelle door interface Bit Number
extern uint8 PH2HDD;      	// Phase 2 hold output for peelle door interface Data Array Number

extern int16 PH2OF;    		// Phase 2 off output for peelle door interface Board Address
extern uint8 PH2OFB;      	// Phase 2 off output for peelle door interface Bit Number
extern uint8 PH2OFD;      	// Phase 2 off output for peelle door interface Data Array Number

extern int16 HDB;       	// Hall button cut-off output for peelle door interface Board Address
extern uint8 HDBB;        	// Hall button cut-off output for peelle door interface Bit Number
extern uint8 HDBD;        	// Hall button cut-off output for peelle door interface Data Array Number

extern int16 RC;  			// retiring cam output for peelle door interface board address.
extern uint8 RCB;			// retiring cam output for peelle door interface bit number.
extern uint8 RCD;			// retiring cam output for peelle door interface data array number.

extern int16 RDT;  			// Rear door transfer relay peelle door interface board address.
extern uint8 RDTB;			// Rear door transfer relay peelle door interface bit number.
extern uint8 RDTD;			// Rear door transfer relay peelle door interface data array number.

extern int16 RADO;      	// Automatic door open output for peelle door interface Board Address
extern uint8 RADOB;   	    // Automatic door open output for peelle door interface Bit Number
extern uint8 RADOD;   	    // Automatic door open output for peelle door interface Data Array Number

extern int16 DAO;			// Designated Attendant Operation output for peelle door interface Board Address
extern uint8 DAOB;       	// Designated Attendant Operation output for peelle door interface Bit Number
extern uint8 DAOD;   	    // Designated Attendant Operation output for peelle door interface Data Array Number


//NEW GUILBERT DOOR INTERFACE

extern int16 G_FDCBO;
extern uint8 G_FDCBOB; 
extern uint8 G_FDCBOD;  

extern int16 G_RDCBO;
extern uint8 G_RDCBOB; 
extern uint8 G_RDCBOD; 

extern int16 G_FH1R;
extern uint8 G_FH1RB; 
extern uint8 G_FH1RD;  

extern int16 G_PH2ON;
extern uint8 G_PH2ONB; 
extern uint8 G_PH2OND;   

extern int16 G_PH2HD;
extern uint8 G_PH2HDB; 
extern uint8 G_PH2HDD;  

extern int16 G_PH2OF;
extern uint8 G_PH2OFB;  
extern uint8 G_PH2OFD;    

extern int16 G_INSP;
extern uint8 G_INSPB; 
extern uint8 G_INSPD;   

extern int16 G_SOPEN;
extern uint8 G_SOPENB; 
extern uint8 G_SOPEND;   

extern int16 G_ADO;
extern uint8 G_ADOB; 
extern uint8 G_ADOD;   

extern int16 G_ADOH;
extern uint8 G_ADOHB; 
extern uint8 G_ADOHD;   

extern int16 G_RDO; 
extern uint8 G_RDOB;	
extern uint8 G_RDOD;		

extern int16 G_RDOH;
extern uint8 G_RDOHB;	
extern uint8 G_RDOHD;		

extern int16 G_RC;
extern uint8 G_RCB;
extern uint8 G_RCD;  

extern int16 G_ININD;
extern uint8 G_ININDB; 
extern uint8 G_ININDD; 


// Block 22	Courion Door operator

extern int16 MPTR;      	// Power-up reset output for courion door interface Board Address
extern uint8 MPTRB;       	// Power-up reset output for courion door interface Bit Number
extern uint8 MPTRD;       	// Power-up reset output for courion door interface Data Array Number

extern int16 FPH2;     		// Phase 2 output for courion door interface Board Address
extern uint8 FPH2B;       	// Phase 2 output for courion door interface Bit Number
extern uint8 FPH2D;       	// Phase 2 output for courion door interface Data Array Number

extern int16 CODG;     		// Door/Gate Cut-out output for courion door interface Board Address
extern uint8 CODGB;      	// Door/Gate Cut-out output for courion door interface Bit Number
extern uint8 CODGD;       	// Door/Gate Cut-out output for courion door interface Data Array Number

extern int16 IAO;      		// Automatic door open output for courion door interface Board Address
extern uint8 IAOB;       	// Automatic door open output for courion door interface Bit Number
extern uint8 IAOD;        	// Automatic door open output for courion door interface Data Array Number

extern int16 COHS;     		// Hall Station Cut-out output for courion door interface Board Address
extern uint8 COHSB;      	// Hall Station Cut-out output for courion door interface Bit Number
extern uint8 COHSD;       	// Hall Station Cut-out output for courion door interface Data Array Number

extern int16 PH2HD;      	// Phase 2 hold output for courion door interface Board Address
extern uint8 PH2HDB;      	// Phase 2 hold output for courion door interface Bit Number
extern uint8 PH2HDD;      	// Phase 2 hold output for courion door interface Data Array Number

extern int16 FPH1;       	// Phase 1 recall output for courion door interface Board Address
extern uint8 FPH1B;       	// Phase 1 recall output for courion door interface Bit Number
extern uint8 FPH1D;       	// Phase 1 recall output for courion door interface Data Array Number

extern int16 ATO;       	// Automatic timed close cut-out output for courion door interface Board Address
extern uint8 ATOB;        	// Automatic timed close cut-out output for courion door interface Bit Number
extern uint8 ATOD;        	// Automatic timed close cut-out output for courion door interface Data Array Number

extern int16 RC;  			// retiring cam output for courion door interface board address.
extern uint8 RCB;			// retiring cam output for courion door interface bit number.
extern uint8 RCD;			// retiring cam output for courion door interface data array number.

extern int16 RAHS;			// Re-activate hall stations for phase 1 recall from phase 2 for courion door interface
extern uint8 RAHSB;			// Re-activate hall stations for phase 1 recall from phase 2 for courion door interface
extern uint8 RAHSD;			// Re-activate hall stations for phase 1 recall from phase 2 for courion door interface

extern int16 RADO;    	 	// Automatic door open output for courion door interface Board Address
extern uint8 RADOB;       	// Automatic door open output for courion door interface Bit Number
extern uint8 RADOD;       	// Automatic door open output for courion door interface Data Array Number

// Block 22	 EMS Door operator
extern int16 EMS3Z; 		// Floor zoning for floor 3 on EMS operator Board Address
extern uint8 EMS3ZB;		// Floor zoning for floor 3 on EMS operator Bit Number
extern uint8 EMS3ZD;		// Floor zoning for floor 3 on EMS operator Data Array Number

extern int16 EMS4Z; 		// Floor zoning for floor 4 on EMS operator Board Address
extern uint8 EMS4ZB;		// Floor zoning for floor 4 on EMS operator Bit Number
extern uint8 EMS4ZD;		// Floor zoning for floor 4 on EMS operator Data Array Number

extern int16 EMS5Z; 		// Floor zoning for floor 5 on EMS operator Board Address
extern uint8 EMS5ZB;		// Floor zoning for floor 5 on EMS operator Bit Number
extern uint8 EMS5ZD;		// Floor zoning for floor 5 on EMS operator Data Array Number

extern int16 EMS6Z;  		// Floor zoning for floor 6 on EMS operator Board Address
extern uint8 EMS6ZB;		// Floor zoning for floor 6 on EMS operator Bit Number
extern uint8 EMS6ZD;		// Floor zoning for floor 6 on EMS operator Data Array Number

extern int16 EMS7Z;  		// Floor zoning for floor 7 on EMS operator Board Address
extern uint8 EMS7ZB;		// Floor zoning for floor 7 on EMS operator Bit Number
extern uint8 EMS7ZD;		// Floor zoning for floor 7 on EMS operator Data Array Number

extern int16 EMS8Z;  		// Floor zoning for floor 8 on EMS operator Board Address
extern uint8 EMS8ZB;		// Floor zoning for floor 8 on EMS operator Bit Number
extern uint8 EMS8ZD;		// Floor zoning for floor 8 on EMS operator Data Array Number

extern int16 EMS9Z;  		// Floor zoning for floor 9 on EMS operator Board Address
extern uint8 EMS9ZB;		// Floor zoning for floor 9 on EMS operator Bit Number
extern uint8 EMS9ZD;		// Floor zoning for floor 9 on EMS operator Data Array Number

extern int16 EMS10Z;  		// Floor zoning for floor 10 on EMS operator Board Address
extern uint8 EMS10ZB;		// Floor zoning for floor 10 on EMS operator Bit Number
extern uint8 EMS10ZD;		// Floor zoning for floor 10 on EMS operator Data Array Number

extern int16 EMS11Z; 		// Floor zoning for floor 11 on EMS operator Board Address
extern uint8 EMS11ZB;		// Floor zoning for floor 11 on EMS operator Bit Number
extern uint8 EMS11ZD;		// Floor zoning for floor 11 on EMS operator Data Array Number

extern int16 DSC;     	 	// Disable power operation of doors & gates for EMS operator Board Address
extern uint8 DSCB;      	// Disable power operation of doors & gates for EMS operator Bit Number
extern uint8 DSCD;        	// Disable power operation of doors & gates for EMS opeartor Data Array Number
	
extern int16 FDOB;     		// Automatically open front door & gates for EMS operator Board Address
extern uint8 FDOBB;      	// Automatically open front door & gates for EMS operator interface Bit Number
extern uint8 FDOBD;       	// Automatically open front door & gates for EMS operator Data Array Number

extern int16 RDOBR;     	// Automatically open rear door & gates for EMS operator Board Address
extern uint8 RDOBB;     	// Automatically open rear door & gates for EMS operator Bit Number
extern uint8 RDOBD;       	// Automatically open rear door & gates for EMS operator Data Array Number

extern int16 HCO;   	  	// Disable door operator buttons in hall for EMS operator Board Address
extern uint8 HCOB;      	// Disable door operator buttons in hall for EMS operator Bit Number
extern uint8 HCOD;       	// Disable door operator buttons in hall for EMS operator Data Array Number

extern int16 FRCL;      	// Closing sequence on fire phase 1 for EMS operator Board Address
extern uint8 FRCLB;       	// Closing sequence on fire phase 1 for EMS operator Bit Number
extern uint8 FRCLD;       	// Closing sequence on fire phase 1 for EMS operator Array Number

extern int16 FPS2;      	// Phase 2 on position for EMS operator Board Address
extern uint8 FPS2B;       	// Phase 2 on position for EMS operator Bit Number
extern uint8 FPS2D;       	// Phase 2 on position for EMS operator Data Array Number

extern int16 FH;      	 	// Phase 2 hold position for EMS operator Board Address
extern uint8 FHB;        	// Phase 2 hold position for EMS operator Bit Number
extern uint8 FHD;         	// Phase 2 hold position for EMS operator Data Array Number

extern int16 TC_;       	// Automatic timed close cut-out output for EMS operator Board Address
extern uint8 TC_B;        	// Automatic timed close cut-out output for EMS operator Bit Number
extern uint8 TC_D;        	// Automatic timed close cut-out output for EMS operator Data Array Number

extern int16 RC;  			// Floor zoning for floor 1 on EMS operator Board Address
extern uint8 RCB;			// Floor zoning for floor 1 on EMS operator Board Bit number
extern uint8 RCD;			// Floor zoning for floor 1 on EMS operator Data Array Number

extern int16 EMS1Z;  		// Floor zoning for floor 1 on EMS operator Board Address
extern uint8 EMS1ZB;		// Floor zoning for floor 1 on EMS operator Board Bit number
extern uint8 EMS1ZD;		// Floor zoning for floor 1 on EMS operator Data Array Number

extern int16 EMS2Z; 		// Floor zoning for floor 2 on EMS operator Board Address
extern uint8 EMS2ZB;		// Floor zoning for floor 2 on EMS operator Bit Number
extern uint8 EMS2ZD;		// Floor zoning for floor 2 on EMS operator Data Array Number

extern int16 EFST;			// FST output for EMS door operator Board Address
extern uint8 EFSTB;			// FST output for EMS door operator Bit Number
extern uint8 EFSTD;			// FST output for EMS door operator Data Array Number


// Block 22	 Front and/or rear automatic freight doors

extern int16 FRT1Z;  		// Floor zoning for floor 3 on EMS operator Board Address
extern uint8 FRT1ZB;		// Floor zoning for floor 3 on EMS operator Bit Number
extern uint8 FRT1ZD;		// Floor zoning for floor 3 on EMS operator Data Array Number

extern int16 FRT2Z;  		// Floor zoning for floor 3 on EMS operator Board Address
extern uint8 FRT2ZB;		// Floor zoning for floor 3 on EMS operator Bit Number
extern uint8 FRT2ZD;		// Floor zoning for floor 3 on EMS operator Data Array Number

extern int16 FRT3Z;  		// Floor zoning for floor 3 on EMS operator Board Address
extern uint8 FRT3ZB;		// Floor zoning for floor 3 on EMS operator Bit Number
extern uint8 FRT3ZD;		// Floor zoning for floor 3 on EMS operator Data Array Number

extern int16 RC;  			// retiring cam output for courion door interface board address.
extern uint8 RCB;			// retiring cam output for courion door interface bit number.
extern uint8 RCD;			// retiring cam output for courion door interface data array number.

extern int16 PDO; 			// Power Door Open output for courion door interface board address.
extern uint8 PDOB;			// Power Door Open output for courion door interface bit number.
extern uint8 PDOD;			// Power Door Open output for courion door interface data array number.

extern int16 PDOR;			// Power Door Open output for courion door interface board address.
extern uint8 PDORB;			// Power Door Open output for courion door interface bit number.
extern uint8 PDORD;  	 	// Power Door Open output for courion door interface data array number.


// Block 27 Local I/O board


	// Sheave brake i/o for 2007 compliance

	// Original sheave brake I/O s

extern int16 EBKi;  		// Emergency Brake Contactor Input Board Address
extern uint8 EBKiB;			// Emergency Brake Contactor Input Bit Number
extern uint8 EBKiD;			// Emergency Brake Contactor Input Data Array Number
	// Same IO location as EBKi
extern int16 EBAi;  		// Emergency Brake Contactor A1,A2 Input Board Address
extern uint8 EBAiB;			// Emergency Brake Contactor A1,A2 Input Bit Number
extern uint8 EBAiD;			// Emergency Brake Contactor A1,A2 Input Data Array Number

extern int16 EBBi;  		// Emergency Brake Contactor B1,B2 Input Board Address
extern uint8 EBBiB;			// Emergency Brake Contactor B1,B2 Input Bit Number
extern uint8 EBBiD;			// Emergency Brake Contactor B1,B2 Input Data Array Number

// i/o Used for Emergency brake delayed drop

extern int16 EBK2i; 	 	// Emergency Brake EBK2 Input Board Address
extern uint8 EBK2iB;		// Emergency Brake EBK2 Input Bit Number
extern uint8 EBK2iD;		// Emergency Brake EBK2 Input Data Array Number

extern int16 EBK1i;  		// Emergency Brake EBK1 Input Board Address
extern uint8 EBK1iB;		// Emergency Brake EBK1 Input Bit Number
extern uint8 EBK1iD;		// Emergency Brake EBK1 Input Data Array Number


// Block 28 Local I/O board

	// Sheave brake i/o for 2007 compliance

extern int16 EBB2;  		// Emergency Brake Contactor B2 Output Board Address
extern uint8 EBB2B;			// Emergency Brake Contactor B2 Output Bit Number
extern uint8 EBB2D;  		// Emergency Brake Contactor B2 Output Data Array Number

extern int16 EBB1;  		// Emergency Brake Contactor B1 Output Board Address
extern uint8 EBB1B;			// Emergency Brake Contactor B1 Output Bit Number
extern uint8 EBB1D;  		// Emergency Brake Contactor B1 Output Data Array Number

extern int16 EBA2;  		// Emergency Brake Contactor A2 Output Board Address
extern uint8 EBA2B;			// Emergency Brake Contactor A2 Output Bit Number
extern uint8 EBA2D; 		// Emergency Brake Contactor A2 Output Data Array Number

	// Original sheave brake I/O s
extern int16 EBK;  			// Emergency Brake Contactor Output Board Address
extern uint8 EBKB;			// Emergency Brake Contactor Output Bit Number
extern uint8 EBKD;			// Emergency Brake Contactor Output Data Array Number
	// Same IO location as EBK
extern int16 EBA1;  		// Emergency Brake Contactor A1 Output Board Address
extern uint8 EBA1B;			// Emergency Brake Contactor A1 Output Bit Number
extern uint8 EBA1D;  		// Emergency Brake Contactor A1 Output Data Array Number

	// Original sheave brake I/O s
extern int16 EBKC;  		// Emergency Brake Cool Contactor Output Board Address
extern uint8 EBKCB;			// Emergency Brake Cool Contactor Output Bit Number
extern uint8 EBKCD;			// Emergency Brake Cool Contactor Output Data Array Number
	// Same IO locations as EBKC
extern int16 EBC;  			// Emergency Brake Cool Contactor Output Board Address
extern uint8 EBCB;			// Emergency Brake Cool Contactor Output Bit Number
extern uint8 EBCD;			// Emergency Brake Cool Contactor Output Data Array Number
	
// i/o Used for Emergency brake delayed drop

extern int16 EBK2;  		// Emergency Brake Contactor 2 Output Board Address
extern uint8 EBK2B;			// Emergency Brake Contactor 2 Output Bit Number
extern uint8 EBK2D;  		// Emergency Brake Contactor 2 Output Data Array Number

extern int16 EBK1;  		// Emergency Brake Contactor 1 Output Board Address
extern uint8 EBK1B;			// Emergency Brake Contactor 1 Output Bit Number
extern uint8 EBK1D;  		// Emergency Brake Contactor 1 Output Data Array Number

extern int16 UTM;  			// Unintended Motion Relay Output Board Address
extern uint8 UTMB;			// Unintended Motion Relay Output Bit Number
extern uint8 UTMD;			// Unintended Motion Relay Output Data Array Number


// Manual power loss brake

extern int16 MBP;  			// Manual Power Loss Brake Power Output Board Address
extern uint8 MBPB;			// Manual Power Loss Brake Power Output Bit Number
extern uint8 MBPD;			// Manual Power Loss Brake Power Output Data Array Number

extern int16 MB2; 	 		// Manual Power Loss Brake Brake Pick 2 Output Board Address
extern uint8 MB2B;			// Manual Power Loss Brake Brake Pick 2 Bit Number
extern uint8 MB2D;			// Manual Power Loss Brake Brake Pick 2 Data Array Number

extern int16 MB1;  			// Manual Power Loss Brake Brake Pick 1 Output Board Address
extern uint8 MB1B;			// Manual Power Loss Brake Brake Pick 1 Bit Number
extern uint8 MB1D;			// Manual Power Loss Brake Brake Pick 1 Data Array Number

// Emergency Power Recovery Single Phase UPS

extern int16 NPD;  			// Normal Power Drive Output Board Address
extern uint8 NPDB;			// Normal Power Drive Output Bit Number
extern uint8 NPDD;			// Normal Power Drive Output Data Array Number

extern int16 EPD; 		// Emergency Power Drive Output Board Address
extern uint8 EPDB;		// Emergency Power Drive Bit Number
extern uint8 EPDD;		// Emergency Power Drive Data Array Number

extern int16 EPO; 			// Emergency Power Off Output Board Address
extern uint8 EPOB;			// Emergency Power Off Bit Number
extern uint8 EPOD;			// Emergency Power Off Data Array Number

// Block 31 Local I/O board


extern int16 TGDO;			//Emergency TUG 5CC Input Board Address
extern uint8 TGDOB;      	//Emergency TUG 5CC Input Bit Number
extern uint8 TGDOD;       	//Emergency TUG 5CC Input Data Array Number

extern int16 TGDC;			//Emergency TUG 6CC Input Board Address
extern uint8 TGDCB;      	//Emergency TUG 6CC Input Bit Number
extern uint8 TGDCD;      	//Emergency TUG 6CC Input Data Array Number

extern int16 TGDOR;			//Emergency TUG 5CC Input Board Address
extern uint8 TGDORB;     	//Emergency TUG 5CC Input Bit Number
extern uint8 TGDORD;      	//Emergency TUG 5CC Input Data Array Number

extern int16 TGDCR;			//Emergency TUG 6CC Input Board Address
extern uint8 TGDCRB;     	//Emergency TUG 6CC Input Bit Number
extern uint8 TGDCRD;      	//Emergency TUG 6CC Input Data Array Number

extern int16 THDS;			//Emergency TUG 6CC Input Board Address
extern uint8 THDSB;      	//Emergency TUG 6CC Input Bit Number
extern uint8 THDSD;       	//Emergency TUG 6CC Input Data Array Number


// Block 32 Local I/O board

extern int16 TAVL;			// Car Available Output Board Address	
extern uint8 TAVLB;			// Car Available Output Bit Number
extern uint8 TAVLD;			// Car Available Output Data Array Number

extern int16 TAVL2;			// Car Available Output Board Address	
extern uint8 TAVL2B;		// Car Available Output Bit Number
extern uint8 TAVL2D;		// Car Available Output Data Array Number

extern int16 TDOL;			// Door Open Limit Output Board Address	
extern uint8 TDOLB;			// Door Open Limit Output Bit Number
extern uint8 TDOLD;			// Door Open Limit Output Data Array Number

extern int16 TDCL;			// Door Close Limit Output Board Address	
extern uint8 TDCLB;			// Door Close Limit Output Bit Number
extern uint8 TDCLD;			// Door Close Limit Output Data Array Number

extern int16 TDOLR;			// Rear Door Open Limit Output Board Address	
extern uint8 TDOLRB;		// Rear Door Open Limit Output Bit Number
extern uint8 TDOLRD;		// Rear Door Open Limit Output Data Array Number

extern int16 TDCLR;			// Rear Door Close Limit Output Board Address	
extern uint8 TDCLRB;		// Rear Door Close Limit Output Bit Number
extern uint8 TDCLRD;		// Rear Door Close Limit Output Data Array Number

extern int16 TIND;			// Car In Independent Output Board Address	
extern uint8 TINDB;			// Car In Independent Output Bit Number
extern uint8 TINDD;			// Car In Independent Output Data Array Number
                        
extern int16 TMED;	 		// Car In Medical Service Output Board Address	
extern uint8 TMEDB;	 	  	// Car In Medical Service Output Bit Number
extern uint8 TMEDD;	 	  	// Car In Medical Service Output Data Array Number

extern int16 TFSO;   		// Car In Fire Service Output Board Address	
extern uint8 TFSOB;		  	// Car In Fire Service Output Bit Number
extern uint8 TFSOD;  		// Car In Fire Service Output Data Array Number

extern int16 TP1;			// Binary Position P1 Output Board Address	
extern uint8 TP1B;			// Binary Position P1 Output Bit Number
extern uint8 TP1D;			// Binary Position P1 Output Data Array Number

extern int16 TP2;			// Binary Position P2 Output Board Address	
extern uint8 TP2B;			// Binary Position P2 Output Bit Number
extern uint8 TP2D;			// Binary Position P2 Output Data Array Number
                           
extern int16 TP3;			// Binary Position P3 Output Board Address	
extern uint8 TP3B;			// Binary Position P3 Output Bit Number
extern uint8 TP3D;			// Binary Position P3 Output Data Array Number
                           
extern int16 TP4;			// Binary Position P4 Output Board Address	
extern uint8 TP4B;			// Binary Position P4 Output Bit Number
extern uint8 TP4D;			// Binary Position P4 Output Data Array Number
                           
extern int16 TP5;			// Binary Position P5 Output Board Address	
extern uint8 TP5B;			// Binary Position P5 Output Bit Number
extern uint8 TP5D;			// Binary Position P5 Output Data Array Number
                           
extern int16 TP6;			// Binary Position P6 Output Board Address	
extern uint8 TP6B;			// Binary Position P6 Output Bit Number
extern uint8 TP6D;			// Binary Position P6 Output Data Array Number


//**********************************************************
//The following five I/O placed here to use a dummy location

extern int16 FW;       	// Field Weakening Output Board Address
extern uint8 FWB;        	// Field Weakening Bit Number
extern uint8 FWD;        	// Field Weakening Data Array Number

//**********************************************************


// Address board 45
extern int16 FiDCB;			// Fire Door Close Button Board Address
extern uint8 FiDCBB;     	// Fire Door Close Button Bit Number
extern uint8 FiDCBD;      	// Fire Door Close Button Data Array Number
	
extern int16 FiDOB;			// Fire Door Open Button Board Address
extern uint8 FiDOBB;      	// Fire Door Open Button Bit Number
extern uint8 FiDOBD;     	// Fire Door Open Button Data Array Number

extern int16 FiFS2C;		// Fire Cancel Button Board Address
extern uint8 FiFS2CB;     	// Fire Cancel Button Bit Number
extern uint8 FiFS2CD;     	// Fire Cancel Button Data Array Number

extern int16 FiDCBR;		// Fire Rrear Door Close Button Board Address
extern uint8 FiDCBRB;     	// Fire Rrear Door Close Button Bit Number
extern uint8 FiDCBRD;      // Fire Rrear Door Close Button Data Array Number

extern int16 FiDOBR;		// Fire Rrear Door Open Button Board Address
extern uint8 FiDOBRB;     	// Fire Rrear Door Open Button Bit Number
extern uint8 FiDOBRD;     	// Fire Rrear Door Open Button Data Array Number

extern int16 ICIU;			// In Car inspection up Input Board Address
extern uint8 ICIUB;			// In Car inspection up Input Bit Number
extern uint8 ICIUD;			// In Car inspection up Input Data Array Number
	  
extern int16 ICID_;			// In Car inspection down Input Board Address
extern uint8 ICID_B;		// In Car inspection down Input Bit Number
extern uint8 ICID_D;		// In Car inspection down Input Data Array Number

extern int16 FiFL;			// Fire Control Fire Light Output Board Address
extern uint8 FiFLB;      	// Fire Control Fire Light Output Bit Number
extern uint8 FiFLD;     	// Fire Control Fire Light Output Data Array Number


// Address board 47
extern int16 PHONE;			// PHONE Input Board Address
extern uint8 PHONEB; 	  	// Phone input Bit Number
extern uint8 PHONED;     	// Phone Data Array Number



// Address board 48

extern int16 ATUL;    	 	// Attendant Up Light Output Board Address
extern uint8 ATULB;       	// Attendant Up Light Bit Number
extern uint8 ATULD;       	// Attendant Up Light Data Array Number

extern int16 ATDL;      	// Attendant Down Light Output Board Address
extern uint8 ATDLB;       	// Attendant Down Light Bit Number
extern uint8 ATDLD;       	// Attendant Down Light Data Array Number


//**********************************************************
// Radial PI Outputs

extern int16 RDDA;		// Car Available Output Board Address	
extern uint8 RDDAB;		// Car Available Output Bit Number
extern uint8 RDDAD;		// Car Available Output Data Array Number

extern int16 RUDA;		// Car Available Output Board Address	
extern uint8 RUDAB;		// Car Available Output Bit Number
extern uint8 RUDAD;		// Car Available Output Data Array Number

extern int16 RFSO;		// Car Available Output Board Address	
extern uint8 RFSOB;		// Car Available Output Bit Number
extern uint8 RFSOD;		// Car Available Output Data Array Number

extern int16 RPI1;		// Car Available Output Board Address	
extern uint8 RPI1B;		// Car Available Output Bit Number
extern uint8 RPI1D;	   	// Car Available Output Data Array Number

extern int16 RPI2;		// Car Available Output Board Address	
extern uint8 RPI2B;		// Car Available Output Bit Number
extern uint8 RPI2D;	   	// Car Available Output Data Array Number

extern int16 RPI3;		// Car Available Output Board Address	
extern uint8 RPI3B;		// Car Available Output Bit Number
extern uint8 RPI3D;	   	// Car Available Output Data Array Number

extern int16 RPI4;		// Car Available Output Board Address	
extern uint8 RPI4B;		// Car Available Output Bit Number
extern uint8 RPI4D;	   	// Car Available Output Data Array Number

extern int16 RPI5;		// Car Available Output Board Address	
extern uint8 RPI5B;		// Car Available Output Bit Number
extern uint8 RPI5D;	   	// Car Available Output Data Array Number

extern int16 RPI6;		// Car Available Output Board Address	
extern uint8 RPI6B;		// Car Available Output Bit Number
extern uint8 RPI6D;	   	// Car Available Output Data Array Number

extern int16 RPI7;		// Car Available Output Board Address	
extern uint8 RPI7B;		// Car Available Output Bit Number
extern uint8 RPI7D;	   	// Car Available Output Data Array Number

extern int16 RPI8;		// Car Available Output Board Address	
extern uint8 RPI8B;		// Car Available Output Bit Number
extern uint8 RPI8D;	   	// Car Available Output Data Array Number

extern int16 RPI9;		// Car Available Output Board Address	
extern uint8 RPI9B;		// Car Available Output Bit Number
extern uint8 RPI9D;	   	// Car Available Output Data Array Number

extern int16 RPI10;		// Car Available Output Board Address	
extern uint8 RPI10B;	// Car Available Output Bit Number
extern uint8 RPI10D; 	// Car Available Output Data Array Number

extern int16 RPI11;		// Car Available Output Board Address	
extern uint8 RPI11B;	// Car Available Output Bit Number
extern uint8 RPI11D;	// Car Available Output Data Array Number

extern int16 RPI12;		// Car Available Output Board Address	
extern uint8 RPI12B;	// Car Available Output Bit Number
extern uint8 RPI12D;	// Car Available Output Data Array Number

extern int16 RPI13;		// Car Available Output Board Address	
extern uint8 RPI13B;	// Car Available Output Bit Number
extern uint8 RPI13D;	// Car Available Output Data Array Number

extern int16 RPI14;		// Car Available Output Board Address	
extern uint8 RPI14B;	// Car Available Output Bit Number
extern uint8 RPI14D;	// Car Available Output Data Array Number

extern int16 RPI15;		// Car Available Output Board Address	
extern uint8 RPI15B;	// Car Available Output Bit Number
extern uint8 RPI15D;	// Car Available Output Data Array Number

extern int16 RPI16;		// Car Available Output Board Address	
extern uint8 RPI16B;	// Car Available Output Bit Number
extern uint8 RPI16D;	// Car Available Output Data Array Number

extern int16 RPI17;		// Car Available Output Board Address	
extern uint8 RPI17B;	// Car Available Output Bit Number
extern uint8 RPI17D;	// Car Available Output Data Array Number

extern int16 RPI18;		// Car Available Output Board Address	
extern uint8 RPI18B;	// Car Available Output Bit Number
extern uint8 RPI18D;	// Car Available Output Data Array Number

extern int16 RPI19;		// Car Available Output Board Address	
extern uint8 RPI19B;	// Car Available Output Bit Number
extern uint8 RPI19D;   	// Car Available Output Data Array Number

extern int16 RPI20;		// Car Available Output Board Address	
extern uint8 RPI20B;	// Car Available Output Bit Number
extern uint8 RPI20D;	// Car Available Output Data Array Number

extern int16 RPI21;		// Car Available Output Board Address	
extern uint8 RPI21B;	// Car Available Output Bit Number
extern uint8 RPI21D;	// Car Available Output Data Array Number


#if (Tract_HR == 1)
extern const int16 cc[33][61];
extern const unsigned char ccb[3][61];
extern const unsigned char ccd[3][61];
extern const int16 ccl[3][61];
extern const unsigned char cclb[3][61];
extern const unsigned char ccld[3][61];
extern const int16 ccr[2][61];
extern const unsigned char ccrb[2][61];
extern const unsigned char ccrd[2][61];
extern const int16 ccrl[2][61];
extern const unsigned char ccrlb[2][61];
extern const unsigned char ccrld[2][61];

extern const int16 rm_cc[2][61];
extern const unsigned char rm_ccb[2][61];
extern const unsigned char rm_ccd[2][61];
extern const int16 rm_ccl[2][61];
extern const unsigned char rm_cclb[2][61];
extern const unsigned char rm_ccld[2][61];

extern const int16 rm_ccr[2][61];
extern const unsigned char rm_ccrb[2][61];
extern const unsigned char rm_ccrd[2][61];
extern const int16 rm_ccrl[2][61];
extern const unsigned char rm_ccrlb[2][61];
extern const unsigned char rm_ccrld[2][61];

extern const int16 sccl[61];
extern const unsigned char scclb[61];
extern const unsigned char sccld[61];
extern const int16 sccrl[61];
extern const unsigned char sccrlb[61];
extern const unsigned char sccrld[61];
extern const int16 ccs[61];
extern const unsigned char ccsb[61];
extern const unsigned char ccsd[61];
extern const int16 rccs[61];
extern const unsigned char rccsb[61];
extern const unsigned char rccsd[61];
#else
extern const int16 cc[2][32];
extern const unsigned char ccb[2][32];
extern const unsigned char ccd[2][32];
extern const int16 ccl[2][32];
extern const unsigned char cclb[2][32];
extern const unsigned char ccld[2][32];
extern const int16 ccr[2][32];
extern const unsigned char ccrb[2][32];
extern const unsigned char ccrd[2][32];
extern const int16 ccrl[2][32];
extern const unsigned char ccrlb[2][32];
extern const unsigned char ccrld[2][32];

extern const int16 rm_cc[2][32];
extern const unsigned char rm_ccb[2][32];
extern const unsigned char rm_ccd[2][32];
extern const int16 rm_ccl[2][32];
extern const unsigned char rm_cclb[2][32];
extern const unsigned char rm_ccld[2][32];

extern const int16 rm_ccr[2][32];
extern const unsigned char rm_ccrb[2][32];
extern const unsigned char rm_ccrd[2][32];
extern const int16 rm_ccrl[2][32];
extern const unsigned char rm_ccrlb[2][32];
extern const unsigned char rm_ccrld[2][32];

extern const int16 sccl[32];
extern const unsigned char scclb[32];
extern const unsigned char sccld[32];
extern const int16 sccrl[32];
extern const unsigned char sccrlb[32];
extern const unsigned char sccrld[32];
extern const int16 ccs[32];
extern const unsigned char ccsb[32];
extern const unsigned char ccsd[32];
extern const int16 rccs[32];
extern const unsigned char rccsb[32];
extern const unsigned char rccsd[32];
#endif

extern const int16 dc[6];
extern const unsigned char dcb[6];
extern const unsigned char dcd[6];
extern const int16 dcl[6];
extern const unsigned char dclb[6];
extern const unsigned char dcld[6];
extern const int16 uc[6];
extern const unsigned char ucb[6];
extern const unsigned char ucd[6];
extern const int16 ucl[6];
extern const unsigned char uclb[6];
extern const unsigned char ucld[6];

// i/o blocks 0 and 1
extern const int16 DN[2];			// Down Directional Stop Input Board Address
extern const uint8 DNB[2];     	// Down Directional Stop Input Bit Number
extern const uint8 DND[2];        // Down Directional Stop Input Data Array Number

extern const int16 DT[2];   		// Down Terminal Slowdown Input Board Address
extern const uint8 DTB[2];     	// Down Terminal Slowdown Input Bit Number
extern const uint8 DTD[2];        // Down Termianl Slowdown Input Data Array Number
		
extern const int16 DTS[2];      	// Emergency Down Terminal Input Board Address
extern const uint8 DTSB[2];       // Emergency Down Terminal Input Bit Number
extern const uint8 DTSD[2];       // Emergency Down Terminal Input Data Array Number

extern const int16 DT1[2];      	// 1st Down Terminal Input Board Address
extern const uint8 DT1B[2];       // 1st Down Terminal Input Bit Number
extern const uint8 DT1D[2];	    // 1st Down Terminal Input Data Array Number

extern const int16 DT2[2];      	// 2nd Down Terminal Input Board Address
extern const uint8 DT2B[2];       // 2nd Down Terminal Input Bit Number
extern const uint8 DT2D[2];       // 2nd Down Terminal Input Data Array Number

extern const int16 DT3[2];     	// 3RD Down Terminal Input Board Address
extern const uint8 DT3B[2];       // 3RD Down Terminal Input Bit Number
extern const uint8 DT3D[2];       // 3RD Down Terminal Input Data Array Number
		
#if ((Traction == 1) || (Tract_OL == 1))
	extern const int16 DT4[2];     	// 4TH Down Terminal Input Board Address
	extern const uint8 DT4B[2];       // 4TH Down Terminal Input Bit Number
	extern const uint8 DT4D[2];      	// 4TH Down Terminal Input Data Array Number

	extern const int16 DT5[2];     	// 5TH Down Terminal Input Board Address
	extern const uint8 DT5B[2];       // 5TH Down Terminal Input Bit Number
	extern const uint8 DT5D[2];      	// 5TH Down Terminal Input Data Array Number

	extern const int16 DT6[2];     	// 6TH Down Terminal Input Board Address
	extern const uint8 DT6B[2];       // 6TH Down Terminal Input Bit Number
	extern const uint8 DT6D[2];      	// 6TH Down Terminal Input Data Array Number
#endif

extern const int16 MRIE[3];     	// Machine Room Inspection Enable Input Board Address
extern const uint8 MRIEB[3];     	// Machine Room Inspection Enable Input Bit Number
extern const uint8 MRIED[3];     	// Machine Room Inspection Enable Input Data Array Number

extern const int16 EBKS[4];  	  	 	// Emergency Brake Switch Input Board Address
extern const uint8 EBKSB[4];   	  	// Emergency Brake Switch Input Bit Number
extern const uint8 EBKSD[4];      	// Emergency Brake Switch Input Data Array Number

extern const int16 UN[2];      	// Up Terminal Stop Input Board Address
extern const uint8 UNB[2];      	// Up Termianl Stop Input Bit Number
extern const uint8 UND[2];        // Up Terminal Stop Input Data Array Number

extern const int16 UT[2];       	// Up Terminal Slowdown Input Board Address
extern const uint8 UTB[2];        // Up Terminal Slowdown Input Bit Number
extern const uint8 UTD[2];        // Up Terminal Slowdown Input Data Array Number

extern const int16 UTS[2];      	// Emergency Up Terminal Input Board Address
extern const uint8 UTSB[2];       // Emergency Up Terminal Input Bit Number
extern const uint8 UTSD[2];       // Emergency Up Terminal Input Data Array Number

extern const int16 UT1[2];      	// 1ST Up Terminal Input Board Address
extern const uint8 UT1B[2];       // 1ST Up Terminal Input Bit Number
extern const uint8 UT1D[2];       // 1ST Up Terminal Input Data Array Number
		
extern const int16 UT2[2];      	// 2nd Up Terminal Input Board Address
extern const uint8 UT2B[2];       // 2nd Up Terminal Input Bit Number
extern const uint8 UT2D[2];       // 2nd Up Terminal Input Data Array Number

extern const int16 UT3[2];       	// 3RD Up Terminal Input Board Address
extern const uint8 UT3B[2];       // 3RD Up Terminal Input Bit Number
extern const uint8 UT3D[2];       // 3RD Up Terminal Input Data Array Number

#if ((Traction == 1) || (Tract_OL == 1))
	extern const int16 UT4[2];    	// 4TH Up Terminal Input Board Address
	extern const uint8 UT4B[2];       // 4TH Up Terminal Input Bit Number
	extern const uint8 UT4D[2];       // 4TH Up Terminal Input Data Array Number

	extern const int16 UT5[2];       	// 5TH Up Terminal Input Board Address
	extern const uint8 UT5B[2];       // 5TH Up Terminal Input Bit Number
	extern const uint8 UT5D[2];       // 5TH Up Terminal Input Data Array Number
			
	extern const int16 UT6[2];       	// 5TH Up Terminal Input Board Address
	extern const uint8 UT6B[2];       // 5TH Up Terminal Input Bit Number
	extern const uint8 UT6D[2];       // 5TH Up Terminal Input Data Array Number
#endif

extern const int16 FBZ[3];     		// Machine room fire buzzer board address
extern const int8 FBZB[3];      		// Machine room fire buzzer Output Bit Number
extern const int8 FBZD[3];        	// Machine room fire buzzer Output Data Array Number


// i/o blocks 2 and 5
extern const int16 SS[2];			 // Saftey String Input Board Address
extern const uint8 SSB[2]; 		 // Saftey String Input Bit Number
extern const uint8 SSD[2]; 		 // Saftey String Input Data Array Number

extern const int16 CWS[2]; 		 // Counterweight Switch Input Board Address (Traction non-distance feedback)
extern const uint8 CWSB[2];		 // Counterweight Switch Input Bit Number
extern const uint8 CWSD[2];		 // Counterweight Switch Input Data Array Number
		
extern const int16 TDC[4];		// Top Door Close Contact input board address
extern const uint8 TDCB[4];		// Top Door Close Contact input bit number
extern const uint8 TDCD[4];	 	// Top Door Close Contact input data array number

extern const int16 BDC[4];		// Bottom Door Close Contact Input	Board Address
extern const uint8 BDCB[4];		// Bottom Door Close Contact Hall Input Bit Number
extern const uint8 BDCD[4];		// Bottom Door Close Contact Hall Input Data Array

// I/O Blocks 2 and 27

extern const int16 COL [2];     	// Earth Quake Collision Switch Board Address
extern const uint8 COLB [2];     	// Earth Quake Collision Switch Bit Number
extern const uint8 COLD [2];   	// Earth Quake Collision Switch Data Array Number

// Note: LOS is the same i/o as COL

extern const int16 LOS [2];     	// Low Oil Switch Board Address
extern const uint8 LOSB [2];     	// Low Oil Switch Bit Number
extern const uint8 LOSD [2];      // Low Oil Switch Data Array Number

extern const int16 ETH [2];     	// Earth quake sizmic switch Board Address
extern const uint8 ETHB [2];     	// Earth Quake sizmic switch Bit Number
extern const uint8 ETHD [2];       // Earth Quake sizmic switch Data Array Number

extern const int16 EQR [2];		// Earthquake operation reset input board address
extern const uint8 EQRB [2];		// Earthquake operation input bit number
extern const uint8 EQRD [2];		// Earthquake operation input data array number

// Note: EOR same as EQR
extern const int16 EOR [4];		// Elevator Off Reset input board address
extern const int8 EORB [4];		// Elevator Off Reset input bit number
extern const int8 EORD [4];		// Elevator Off Reset input data array number
											
// Note: RTLDC same as EQR
extern const int16 RTLDC [2];		// Recall to lobby keep doors closed input board address
extern const uint8 RTLDCB [2];	// Recall to lobby keep doors closed input bit number
extern const uint8 RTLDCD [2];	// Recall to lobby keep doors closed input data array number

extern const int16 LPS[2];		// Low Pressure Switch Input Board Address
extern const uint8 LPSB[2];		// Low Pressure Switch Input Bit Number
extern const uint8 LPSD[2];		// Low Pressure Switch Input Data Array

// i/o blocks 3 and 5
// Note: MDC uses the same input location as DPR
extern const int16 MDC  [3];    	// Middle Door Close Contact input value 1 Board Address
extern const uint8 MDCB [3];     	// Middle Door Close Contact input value 1 Bit Number
extern const uint8 MDCD [3]; 		// Middle Door Close Contact input value 1 Data Array Number

// Note: MDCR uses the same input location as RDPR
extern const int16 MDCR [3];		// Rear Middle Door Close Contact Input Board Address (Traction non-distance feedback)
extern const uint8 MDCRB [3];		// Rear Middle Door Close Contact Input Bit Number
extern const uint8 MDCRD [3];		// Rear Middle Door Close Contact Input Data Array Number

extern const int16 RCM[2];      	// Retiring Cam Output Board Address
extern const uint8 RCMB[2];      	// Retiring Cam Output Bit Number
extern const uint8 RCMD[2];      	// Retiring Cam Output Data Array Number
		
// io block 3 and 41

extern const int16 UL_1[2];		// Up Level - Secondary Input Board Address
extern const uint8 UL_1B[2];		// Up Level - Secondary Input Bit Number
extern const uint8 UL_1D[2];   	// Up Level - Secondary Input Data Array Number

extern const int16 UL[2];      	// Up Level Input Board Address
extern const uint8 ULB[2];       	// Up Level Input Bit Number
extern const uint8 ULD[2];        // Up Level Input Data Array Number

extern const int16 DZ[2];     	// Door Zone Input Board Address
extern const uint8 DZB[2];      	// Door Zone Input Bit Number
extern const uint8 DZD[2];        // Door Zone Input Data Array Number

extern const int16 DZA[2];      	// DZA (both dz sensors ored) Input Board Address
extern const uint8 DZAB[2];      	// DZA (both dz sensors ored) Input Bit Number	
extern const uint8 DZAD[2];       // DZA (both dz sensors ored) Input Data Array Number

extern const int16 DL[2];      	// Down Level Input Board Address
extern const uint8 DLB[2];       	// Down Level Input Bit Number
extern const uint8 DLD[2];   		// Down Level Input Data Array Number

extern const int16 DL_1[2];		// Down Level - Secondary Input Board Address
extern const uint8 DL_1B[2];		// Down Level - Secondary Input Bit Number
extern const uint8 DL_1D[2];		// Down Level - Secondary Input Data Array Number

// i/o block 5
extern const int16 ISER [2]; 
extern const uint8 ISERB [2];
extern const uint8 ISERD [2];

//io block 5 and 28

extern const int16 OSERL [2];		// Out of Service Light Output Board Address
extern const uint8 OSERLB [2];	// Out of Service Light Output Bit Number
extern const uint8 OSERLD [2];	// Out of Service Light Output Data Array Number


extern const int16 OSERB [1];
extern const uint8 OSERBB [1];
extern const uint8 OSERBD [1];

extern const int16 OSERA [1];
extern const uint8 OSERAB [1];
extern const uint8 OSERAD [1];

// I/O blocks 7 and 9
extern const int16 RM_DCBI [2];   // Door Close Button Input Board Address
extern const uint8 RM_DCBIB [2];	// Door Close Button Input Bit Number
extern const uint8 RM_DCBID [2];  // Door Close Button Input Data Array Number

extern const int16 RM_DOBI [2];	// Door Open Button Input Board Address
extern const uint8 RM_DOBIB [2]; 	// Door Open Button Input Bit Number
extern const uint8 RM_DOBID [2]; 	// Door Open Button Input Data Array Number

extern const int16 RM_INDC [2];	// Independent Input Board Address
extern const uint8 RM_INDCB [2]; 	// Independent Input Bit Number
extern const uint8 RM_INDCD [2];	// Independent Input Data Array Number

extern const int16 RM_ED [2];		// Extended Door Time Input Board Address
extern const uint8 RM_EDB [2]; 	// Extended Door Time Input Bit Number
extern const uint8 RM_EDD [2];	// Extended Door Time Input Data Array Number

extern const int16 SCS [2];  		// Switch to Remote Car Station Input Board Address(spi14 Terminal)
extern const uint8 SCSB [2];		// Switch to Remote Car Station Input Bit Number
extern const uint8 SCSD [2]; 		// Switch to Remote Car Station Input Data Array Number

//REMOTE REAR CAR STATION

extern const int16 RM_DCBR [2];   // Rear Door Close Button Input Board Address
extern const uint8 RM_DCBRB [2];	// Rear Door Close Button Input Bit Number
extern const uint8 RM_DCBRD [2];  // Rear Door Close Button Input Data Array Number

extern const int16 RM_DOBR [2];	// Rear Door Open Button Input Board Address
extern const uint8 RM_DOBRB [2]; 	// Rear Door Open Button Input Bit Number
extern const uint8 RM_DOBRD [2]; 	// Rear Door Open Button Input Data Array Number

extern const int16 SCSR [2];  	// Switch to Remote Car Station Input Board Address(spi14 Terminal)al)
extern const uint8 SCSRB [2];		// Switch to Remote Car Station Input Bit Number
extern const uint8 SCSRD [2]; 	// Switch to Remote Car Station Input Data Array Number

// Rear I/O's
// Block 17	 

extern const int16 DCBR[2];     	// Rear Door Close Button Input Board Address
extern const uint8 DCBRB[2];      // Rear Door Close Button Input Bit Number
extern const uint8 DCBRD[2];      // Rear Door Close Button Input Data Array Number

extern const int16 DOBR[2];     	// Rear Door Open Button Input Board Address
extern const uint8 DOBRB[2];      // Rear Door Open Button Input Bit Number
extern const uint8 DOBRD[2];	    // Rear Door Open Button Input Data Array Number


// Rear Door I/O Block 21 and 17
extern const int16 DOLR [3];     	// Door Open Limit Rear Input Board Address
extern const uint8 DOLRB [3];   	// Door Open Limit Rear Input Bit Number
extern const uint8 DOLRD [3];		// Door Open Limit Rear Input Data Array Number

extern const int16 DCLR [3];     	// Door Close Limit Rear Input Board Address
extern const uint8 DCLRB [3];    	// Door Close Limit Rear Input Bit Number
extern const uint8 DCLRD [3];		// Door Close Limit Rear Input Data Array Number

extern const int16 RPM [3];		// Rear Door Proctection Input Board Address (Traction non-distance feedback)
extern const uint8 RPMB [3];		// Rear Door Proctection Input Bit Number
extern const uint8 RPMD [3];		// Rear Door Proctection Input Data Array Number

extern const int16 EER [3];    	// Electric Eye Rear Input Board Address
extern const uint8 EERB [3];   	// Electric Eye Rear Input Bit Number
extern const uint8 EERD [3]; 		// Electric Eye Rear Input Data Array Number

extern const int16 SER [3];    	// Saftey Edge Rear Input Board Address
extern const uint8 SERB [3];   	// Saftey Edge Rear Input Bit Number
extern const uint8 SERD [3]; 		// Saftey Edge Rear Input Data Array Number

// Blocks 22 and 18

extern const int16 CDLR [2];    	// Down Cab Lantern Rear Output Board Address
extern const uint8 CDLRB [2];     // Down Cab Lantern Rear Output Bit Number
extern const uint8 CDLRD [2];    	// Down Cab Lantern Rear Output Data Array Number
						  
extern const int16 CULR [2]; 		// Up Cab Lantern Rear Output Board Address
extern const uint8 CULRB [2];		// Up Cab Lantern Rear Output Bit Number
extern const uint8 CULRD [2];    	// Up Cab Lantern Rear Output Data Array Number

extern const int16 REVR [3];     	// Rear Door Reversal Relay Output Board Address
extern const uint8 REVRB [3];    	// Rear Door Reversal Relay Output Bit Number
extern const uint8 REVRD [3];  	// Rear Door Reversal Relay Output Data Array Number

extern const int16 RVDR [3];     	// Rear Door Reversal Device Output Board Address
extern const uint8 RVDRB [3];    	// Rear Door Reversal Device Output Bit Number
extern const uint8 RVDRD [3];  	// Rear Door Reversal Device Output Data Array Number

// Note: HVDR has the same i/o location as RVDR for old board location (rdoor_io_sel = 0)
extern const int16 HVDR [3];    	// Rear Heavy Door Output Board Address
extern const uint8 HVDRB [3];   	// Rear Heavy Door Output Bit Number
extern const uint8 HVDRD [3];		// Rear Heavy Door Output Data Array Number

extern const int16 DOR [3];  			// Door Open Rear Output Board Address
extern const uint8 DORB [3]; 		// Door Open Rear Output Bit Number
extern const uint8 DORD [3]; 		// Door Open Rear Output Data Array Number

extern const int16 DCR [3];		// Door Close Rear Output Board Address
extern const uint8 DCRB [3]; 		// Door Close Rear Output Bit Number
extern const uint8 DCRD [3]; 		// Door Close Rear Output Data Array Number

extern const int16 NUDR [3]; 		// Nudge Rear Output Board Address
extern const uint8 NUDRB [3];		// Nudge Rear Output Bit Number
extern const uint8 NUDRD [3];		// Nudge Rear Output Data Array Number


// TOC AND COP BOARD INPUTS
// Block 23 and 13

extern const int16 FS2[3];
extern const uint8 FS2B[3];
extern const uint8 FS2D[3];

extern const int16 FS2H [3]; 
extern const uint8 FS2HB [3];
extern const uint8 FS2HD [3];

extern const int16 FS2C [3]; 
extern const uint8 FS2CB [3];
extern const uint8 FS2CD [3];

extern const int16 HBE [3]; 
extern const uint8 HBEB [3];
extern const uint8 HBED [3];

extern const int16 DCBI [3];
extern const uint8 DCBIB [3];
extern const uint8 DCBID [3];

extern const int16 DOBI [3]; 
extern const uint8 DOBIB [3];
extern const uint8 DOBID [3];

extern const int16 INDC [3]; 
extern const uint8 INDCB [3];
extern const uint8 INDCD [3];

extern const int16 EMS [3]; 
extern const uint8 EMSB [3]; 
extern const uint8 EMSD [3]; 

extern const int16 ZON [3]; 
extern const uint8 ZONB [3]; 
extern const uint8 ZOND [3]; 

extern const int16 DLW [7]; 
extern const uint8 DLWB [7]; 
extern const uint8 DLWD [7]; 

extern const int16 ATTHC [3]; 
extern const uint8 ATTHCB [3];
extern const uint8 ATTHCD [3];

extern const int16 ATT [3]; 
extern const uint8 ATTB [3];
extern const uint8 ATTD [3];

extern const int16 ATTUP [3]; 
extern const uint8 ATTUPB [3]; 
extern const uint8 ATTUPD [3]; 

extern const int16 ATTDN [3];
extern const uint8 ATTDNB [3]; 
extern const uint8 ATTDND [3]; 

extern const int16 IDSO [3];		// Independent Security Override Input Board Address
extern const uint8 IDSOB [3];	  	// Independent Security Override Input Number
extern const uint8 IDSOD [3];    	// Independent Security Override Input Data Array Number
						   
extern const int16 ATTBY [3];			// Attendant Bypass Button Input Board Address
extern const uint8 ATTBYB [3]; 		// Attendant Bypass Button Input Bit Number
extern const uint8 ATTBYD [3]; 		// Attendant Bypass Button Input Data Array Number

extern const int16 ED [3]; 			// Extended door time Input Board Address
extern const uint8 EDB [3];			// Extended door time Input Bit Number
extern const uint8 EDD [3];			// Extended door time Input Data Array Number

extern const int16 HUGB [3];  	  	// HUGS Bypass Input Board Address
extern const uint8 HUGBB [3];  	  	// HUGS Bypass Input Bit Number
extern const uint8 HUGBD [3];      	// HUGS Bypass Input Data Array Number

extern const int16 ALRM [2];   			// Alarm Button Input Board Address
extern const uint8 ALRMB [2];	    		// Alarm Button Input Bit Number
extern const uint8 ALRMD [2];	 			// Alarm Button Input Data Array Number


// TOC AND COP BOARD OUTPUTS
// Block 24, 25	AND 14

extern const int16 DOL [2];   			// Door Open Limit Input Board Address
extern const uint8 DOLB [2];      		// Door Open Limit Input Bit Number
extern const uint8 DOLD [2];        		// Door Open Limit Input Data Array Number

extern const int16 DCL [2];    			// Door Close Limit Input Board Address
extern const uint8 DCLB [2];       		// Door Close Limit Input Bit Number
extern const uint8 DCLD [2];         		// Door Close Limit Input Data Array Number

extern const int16 DPM [2];	    		// Door Proctection input value 1 Board Address
extern const uint8 DPMB [2];	     		// Door Proctection input value 1 Bit Number
extern const uint8 DPMD [2];		        // Door Proctection input value 1 Data Array Number

extern const int16 EE [2];   				// Electric Eye Input Board Address
extern const uint8 EEB [2];        		// Electric Eye Input Bit Number
extern const uint8 EED [2];          		// Electric Eye Input Data Array Number

extern const int16 DO [2];      			// Door Open Output Board Address
extern const uint8 DOB [2];        		// Door Open Output Bit Number
extern const uint8 DOD [2];         		// Door Open OUtput Data Array Number

extern const int16 DC [2];      			// Door Close Output Board Address
extern const uint8 DCB [2];       		// Door Close Output Bit Number
extern const uint8 DCD [2];         		// Door Close Output Data Array Number

extern const int16 NUD [2];     			// Nudge Output Board Address
extern const uint8 NUDB [2];       		// Nudge Output Bit Number
extern const uint8 NUDD [2];         		// Nudge Output Data Array Number

extern const int16 DDAC [3]; 
extern const uint8 DDACB [3];
extern const uint8 DDACD [3];

extern const int16 EDL [3]; 
extern const uint8 EDLB [3];
extern const uint8 EDLD [3];

extern const int16 SERVO [2]; 
extern const uint8 SERVOB [2];
extern const uint8 SERVOD [2];

extern const int16 SABUZ [3];				// Sabbath warning buzzer/chime Output Board Address
extern const uint8 SABUZB [3];  			// Sabbath warning buzzer/chime Output Bit Number
extern const uint8 SABUZD [3];  			// Sabbath warning buzzer/chime Output Array Number

extern const int16 HUGA [3]; 				// HUGS alarm Output Board Address
extern const uint8 HUGAB [3]; 			// HUGS alarm Output Bit Number
extern const uint8 HUGAD [3]; 			// HUGS alarm Output Array Number

extern const int16 DPI1 [3]; 
extern const uint8 DPI1B [3];
extern const uint8 DPI1D [3];

extern const int16 DPI2 [3]; 
extern const uint8 DPI2B [3];
extern const uint8 DPI2D [3];

extern const int16 DPI3 [3]; 
extern const uint8 DPI3B [3];
extern const uint8 DPI3D [3];

extern const int16 DPI4 [3]; 
extern const uint8 DPI4B [3];
extern const uint8 DPI4D [3];

extern const int16 DPI5 [3]; 
extern const uint8 DPI5B [3];
extern const uint8 DPI5D [3];

extern const int16 DPI6 [3]; 
extern const uint8 DPI6B [3];
extern const uint8 DPI6D [3];

extern const int16 HUGO [3];
extern const uint8 HUGOB [3]; 
extern const uint8 HUGOD [3]; 

extern const int16 DCLT [3];
extern const int8 DCLTB [3]; 
extern const int8 DCLTD [3]; 

extern const int16 DTR2 [3]; 
extern const uint8 DTR2B [3];
extern const uint8 DTR2D [3];

extern const int16 DTR [3]; 
extern const uint8 DTRB [3];
extern const uint8 DTRD [3];

extern const int16 SABEE [3]; 
extern const uint8 SABEEB [3];
extern const uint8 SABEED [3];

extern const int16 UDAC [3]; 
extern const uint8 UDACB [3];
extern const uint8 UDACD [3];

extern const int16 EQLC [4]; 
extern const uint8 EQLCB [4];
extern const uint8 EQLCD [4];

extern const int16 F1ML [4]; 
extern const uint8 F1MLB [4];
extern const uint8 F1MLD [4];

extern const int16 RVD [4]; 
extern const uint8 RVDB [4];
extern const uint8 RVDD [4];

extern const int16 EML [4]; 
extern const uint8 EMLB [4];
extern const uint8 EMLD [4];

extern const int16 DCA [4]; 
extern const uint8 DCAB [4];
extern const uint8 DCAD [4];

extern const int16 F1AL [4]; 
extern const uint8 F1ALB [4];
extern const uint8 F1ALD [4];

extern const int16 DZDE [3]; 
extern const uint8 DZDEB [3];
extern const uint8 DZDED [3];

extern const int16 REV [3]; 
extern const uint8 REVB [3];
extern const uint8 REVD [3];

extern const int16 OLL [4]; 
extern const uint8 OLLB [4];
extern const uint8 OLLD [4];

extern const int16 HVD [4]; 
extern const uint8 HVDB [4];
extern const uint8 HVDD [4];

extern const int16 CLF [3]; 
extern const uint8 CLFB [3];
extern const uint8 CLFD [3];


// Block 25	AND 14

extern const int16 CUL [4]; 
extern const uint8 CULB [4];
extern const uint8 CULD [4];

extern const int16 CDL [3]; 
extern const uint8 CDLB [3];
extern const uint8 CDLD [3];

extern const int16 FL [3]; 
extern const uint8 FLB [3];
extern const uint8 FLD [3];

extern const int16 FBNB [3]; 
extern const uint8 FBNBB [3];
extern const uint8 FBNBD [3];

extern const int16 HB [3]; 
extern const uint8 HBB [3];
extern const uint8 HBD [3];

// Block 28
extern const int16 MBC [2];  			// Manual Power Loss Brake Motor Contactor Output Board Address
extern const uint8 MBCB [2];			// Manual Power Loss Brake Motor Contactor Output Bit Number
extern const uint8 MBCD [2];			// Manual Power Loss Brake Motor Contactor Output Data Array Number

// Block 41
extern const int16 DZA [2];      		// DZA (both dz sensors ored) Input Board Address
extern const uint8 DZAB [2];   	    // DZA (both dz sensors ored) Input Bit Number	
extern const uint8 DZAD [2];         	// DZA (both dz sensors ored) Input Data Array Number

// Block 29	and 49

extern const int16 FS [2];       		// Fire Service Switch Input Board Address
extern const uint8 FSB [2];       	// Fire Service Switch Input Bit Number
extern const uint8 FSD [2];       	// Fire Service Switch Input Data Array Number

extern const int16 FSX [2];			// Fire Switch Aux. Input Board Address
extern const uint8 FSXB [2];			// Fire Switch Aux. Input Bit Number
extern const uint8 FSXD [2];			// Fire Switch Aux. Input Data Array Number

extern const int16 BP [2];      		// Fire Service Bypass Input Board Address
extern const uint8 BPB [2];       	// Fire Service Bypass Input Bit Number
extern const uint8 BPD [2];       	// Fire Service Bypass Input Data Array Number

extern const int16 MES [2];      		// Main Eggress Smoke Detector Board Address
extern const uint8 MESB [2];     		// Main Eggress Smoke Detector Bit Number
extern const uint8 MESD [2];      	// Main Eggress Smoke Detector Data Array Number

extern const int16 ALT [2];      		// Fire Service Alternate Floor Input Board Address
extern const uint8 ALTB [2];			// Fire Service Alternate Floor Input Bit Number
extern const uint8 ALTD [2];			// Fire Service Alternate Floor Input Data Array Number

extern const int16 MRS [2];     		// Machine Room Smoke Detector Input Board Address
extern const uint8 MRSB [2];     		// Machine Room Smoke Detector Input Bit Number
extern const uint8 MRSD [2];      	// Machine Room Smoke Detector Input Data Array Number

extern const int16 HWS [2];     		// Hoistway Smoke Input Board Address
extern const uint8 HWSB [2];     		// Hoistway Smoke Input Bit Number
extern const uint8 HWSD [2];	    	// Hoistway Smoke Input Data Array Number

extern const int16 HWS2 [2];    		// Hoistway Smoke 2 Input Board Address
extern const uint8 HWS2B [2];     	// Hoistway Smoke 2 Input Bit Number
extern const uint8 HWS2D [2];      	// Hoistway Smoke 2 Input Data Array Number

extern const int16 EMP [2];      		// Emergency Power Input Board Address
extern const uint8 EMPB [2];   		// Emergency Power Input Bit Number
extern const uint8 EMPD [2];     		// Emergency Power Input Data Array Number

extern const int16 EPS [2];     		// Emergency Power Select Input Board Address
extern const uint8 EPSB [2];     		// Emergency Power Select Input Bit Number
extern const uint8 EPSD [2];     		// Emergency Power Select Input Data Array Number

extern const int16 EPT[3];			// Emergency Power Transfer Input Board Address
extern const uint8 EPTB[3];			// Emergency Power Transfer Input Bit Number
extern const uint8 EPTD[3];			// Emergency Power Transfer Input Data Array

extern const int16 HEOF[2];     		// Hall Switch Elevator Off Input Board Address
extern const uint8 HEOFB[2];      		// Hall Switch Elevator Off Input Bit Number
extern const uint8 HEOFD[2];         		// Hall Switch Elevator Off Input Data Array Number

// HUGI same as HEOF

extern const int16 HUGI[2];     		// HUGS Detection Input Board Address
extern const uint8 HUGIB[2];      	// HUGS Detection Input Bit Number
extern const uint8 HUGID[2];      	// HUGS Detection Input Data Array Number

// JAILB Same as HEOF

extern const int16 JAILB[2];     		// Jail Break Input Board Address
extern const uint8 JAILBB[2];      	// Jail Break Input Bit Number
extern const uint8 JAILBD[2];      	// Jail Break Input Data Array Number


	// RTL for traction
extern const int16 RTL[2];			// Return to Lobby Input Board Address
extern const uint8 RTLB[2];				// Return to Lobby Input Bit Number
extern const uint8 RTLD[2];	  				// Return to Lobby Input Data Array Number

extern const int16 EMSH[2];			// Emergency Medical Service Hall Input	Board Address
extern const uint8 EMSHB[2];			// Emergency Medical Service Hall Input Bit Number
extern const uint8 EMSHD[2];					// Emergency Medical Service Hall Input Data Array

// The P input is used for RTL on hydros prior to 2004 I/O Change.
	// RTL for hydro
extern const int16 RTLH[3];	// Return to Lobby Input Board Address
extern const uint8 RTLHB[3];		// Return to Lobby Input Bit Number
extern const uint8 RTLHD[3];     			// Return to Lobby Input Data Array Number

extern const int16 EMLH[2];			// Emergency Medical Hall light output Board Address
extern const uint8 EMLHB[2];			// Emergency Medical Hall light output Bit Number
extern const uint8 EMLHD[2]; 			// Emergency Medical Hall light output Data Array Number

extern const int16 EPLH [2];       	// Emergency Power Hall Light Output Board Address
extern const uint8 EPLHB [2];        	// Emergency Power Hall Light Output Bit Number
extern const uint8 EPLHD [2];         // Emergency Power Hall Light Output Data Array Number

extern const int16 EPRL [2];       	// Emergency Power Recall Light Output Board Address
extern const uint8 EPRLB [2];        	// Emergency Power Recall Light Output Bit Number
extern const uint8 EPRLD [2];         // Emergency Power Recall Light Output Data Array Number

extern const int16 EPSL [2];       	// Emergency Power Select Light Output Board Address
extern const uint8 EPSLB [2];        	// Emergency Power Select Light Output Bit Number
extern const uint8 EPSLD [2];         // Emergency Power Select Light Output Data Array Number

extern const int16 FLH [2]; 			// Hall Fire Light Output Board Address
extern const uint8 FLHB [2];        	// Hall Fire Light Output Bit Number
extern const uint8 FLHD [2];          // Hall Fire Light Output Data Array Number

