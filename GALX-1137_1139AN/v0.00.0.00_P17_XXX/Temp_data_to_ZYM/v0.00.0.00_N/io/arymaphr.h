// Bit Address of I/O on Board #1

int16 DLB_ = BRD1B;		// Door Lock Bottom Input Board Address
uint8 DLB_B = BIT5;		// Door Lock Bottom Input Bit Number
uint8 DLB_D =1;   		// Door Lock Bottom Input Data Array Number

int16 DLM = BRD1B;    	// Door Lock Middle Input Board Address
uint8 DLMB = BIT6;     	// Door Lock Middle Input Bit Number
uint8 DLMD = 1;        	// Door Lock Middle Input Data Array Number

int16 DLT = BRD1B;      // Door Lock Top Input Board Address
uint8 DLTB = BIT7;       // Door Lock Top Input Bit Number
uint8 DLTD = 1;         	// Door Lock Top Input Data Array Number

int16 RLM = BRD1B; 		// Rear Lock Middle Input Board Address
uint8 RLMB = BIT8;		// Rear Lock Middle Input Bit Number
uint8 RLMD = 1;   		// Rear Lock Middle Input Data Array Number

int16 DLB_1 = BRD1C; 	// Door Lock Bottom - Secondary Input Board Address
uint8 DLB_1B = BIT1;		// Door Lock Bottom - Secondary Input Bit Number
uint8 DLB_1D = 2;    	// Door Lock Bottom - Secondary Input Data Array Number

int16 DLM_1 = BRD1C;  	// Door Lock Middle - Secondary Input Board Address
uint8 DLM_1B = BIT2;		// Door Lock Middle - Secondary Input Bit Number
uint8 DLM_1D = 2;  		// Door Lock Middle - Secondary Input Data Array Number

int16 DLT_1 = BRD1C;		// Door Lock Top - Secondary Input Board Address
uint8 DLT_1B = BIT3;		// Door Lock Top - Secondary Input Bit Number
uint8 DLT_1D = 2;	   	// Door Lock Top - Secondary Input Data Array Number

int16 RLM_1 = BRD1C;  	// Rear Lock Middle - Secondary Input Board Address
uint8 RLM_1B = BIT4;		// Rear Lock Middle - Secondary Input Bit Number
uint8 RLM_1D = 2;    	// Rear Lock Middle - Secondary Input Data Array Number

int16 ACC = BRD1C;		// Access Input Board Address
uint8 ACCB = BIT5;		// Access Input Bit Number
uint8 ACCD = 2;   		// Access Input Data Array Number

int16 BAD = BRD1C;		// Bottom Access Down Input Board Address
uint8 BADB = BIT6;		// Bottom Access Down Input Bit Number
uint8 BADD = 2;   		// Bottom Access Down Input Data Array Number

int16 BAU = BRD1C;		// Bottom Access Up Input Board Address
uint8 BAUB = BIT7;		// Bottom Access Up Input Bit Number
uint8 BAUD = 2;   		// Bottom Access Up Input Data Array Number

int16 TAD = BRD1C;		// Top Access Down Input Board Address
uint8 TADB = BIT8;		// Top Access Down Input Bit Number
uint8 TADD = 2;   		// Top Access Down Input Data Array Number



// Bit Address of I/O on Board #2

int16 TAU = BRD2A;		// Top Access Up Input Board Address
uint8 TAUB = BIT1;		// Top Access Up Input Bit Number
uint8 TAUD = 3;   		// Top Access Up Input Data Array Number

int16 INE = BRD2B;     	// Inspection Switch Enable Input Board Address
uint8 INEB = BIT4;     	// Inspection Switch Enable Input Bit Number
uint8 INED = 4;        	// Inspection Switch Enable Input Data Array Number

int16 SFC = BRD2B;     	// Secondary Fault Control Input Board Address
uint8 SFCB = BIT5;    	// Secondary Fault Control Input Bit Number
uint8 SFCD = 4;        	// Secondary Fault Control Input Data Array Number

int16 HC = BRD2B;       // Group Power Input Board Address
uint8 HCB = BIT7;        // Group Power Input Bit Number
uint8 HCD = 4;           // Group Power Input Data Array Number

// Note: Same input as SS - SS moved to CWS when FFS is used.

int16 FFS = BRD2B;      // Fire Fighters Stop Switch Input Board Address
uint8 FFSB = BIT8;     	// Fire Fighters Stop Switch Input Bit Number
uint8 FFSD = 4;       	// Fire Fighters Stop Switch Input Data Array Number


#if ((Traction == 0) && (Tract_OL == 0))
int16 TPH = BRD2C;      // Temperature High Input Board Address
uint8 TPHB = BIT7;       // Temperature High Input Bit Number
uint8 TPHD = 5;          // Temperature High Input Data Array Number
#endif

// Note: same input as PREVIOUS CWS

int16 CTS = BRD5B;       // Car Top Inspecion Switch Input Board Address
uint8 CTSB = BIT5;     	// Car Top Inspecion Switch Input Bit Number
uint8 CTSD = 13;       	// Car Top Inspecion Switch Input Data Array Number

#if ((Traction == 1) || (Tract_OL == 1))
	// Must not conflict with DT5
	int16 SABO = BRD3C;     	// sabbath On Output Board Address
	uint8 SABOB = BIT6;      // sabbath On Output Bit Number
	uint8 SABOD = 8;         // sabbath On Output Data Array Number

#endif

int16 EDHL = BRD2C;			// Extended Door Time time Hall Input Data Array Number
uint8 EDHLB = BIT8;			// Extended Door Time Hall Input Data Array Number
uint8 EDHLD = 5;  			// Extended Door Time Hall Input Data Array Number


// Bit Address of I/O on Board #3

int16 GBP = BRD3A;		// Gate Bypass Input Board Address
uint8 GBPB = BIT1;		// Gate Bypass Input Bit Number
uint8 GBPD = 6;   		// BGate Bypass Input Data Array Number

int16 LBP = BRD3A;		// Lock Bypass Input Board Address
uint8 LBPB = BIT2;		// Lock Bypass Input Bit Number
uint8 LBPD = 6;   		// Lock Bypass Input Data Array Number

int16 IND = BRD3A;      // Independent Input Board Address
uint8 INDB = BIT3;       // Independent Input Bit Number
uint8 INDD = 6;          // Independent Input Data Array Number

int16 AD = BRD3A;      // Automatic Doors Input Board Address
uint8 ADB = BIT4;       // Automatic Doors Input Bit Number
uint8 ADD = 6;          // Automatic Doors Input Data Array Number

int16 DEL = BRD3A;      // Delta Contactor Input Board Address
uint8 DELB = BIT5;       // Delta Contactor Input Bit Number
uint8 DELD = 6;          // Delta Contactor Input Data Array Number

#if ((Tract_OL == 1) || (Traction == 1))
 	int16 BKS = BRD3A;     // Brake Switch Input Board Address
 	uint8 BKSB = BIT6;     // Brake Switch Input Bit Number
 	uint8 BKSD = 6;        // Brake Switch Input Data Array Number
#else
	//(tpl AND BKS) mapped the same
	int16 TPL = BRD3A;      // Temperature Low Input Board Address
	uint8 TPLB = BIT6;       // Temperature Low Input Bit Number
	uint8 TPLD = 6;          // Temperature Low Input Data Array Number
#endif

int16 GS = BRD3B;    	// Gate Switch Input Board Address
uint8 GSB = BIT5;     	// Gate Switch Input Bit Number
uint8 GSD = 7;        	// Gate Switch Input Data Array Number

int16 GS_1 = BRD3B;		// Gate Switch - Secondary Input Board Address
uint8 GS_1B = BIT6;		// Gate Switch - Secondary Input Bit Number
uint8 GS_1D = 7;  		// Gate Switch - Secondary Input Data Array Number

int16 LC = BRD3B;       // Logic Power Input Board Address
uint8 LCB = BIT7;        // Logic Power Input Bit Number
uint8 LCD = 7;          	// Logic Power Input Data Array Number

int16 INS = BRD3B;      // Inspection Input Board Address
uint8 INSB = BIT8;       // Inspection Input Bit Number
uint8 INSD = 7;         	// Inspection Input Data Array Number

int16 UPML = BRD3C;     	// UP Micro Leveling Output Board Address
uint8 UPMLB = BIT1;      // UP Micro Leveling Output Bit Number
uint8 UPMLD = 8;        	// UP Micro Leveling Output Data Array Number

int16 DNML = BRD3C;     	// DN Micro Leveling Output Board Address
uint8 DNMLB = BIT8;      // DN Micro Leveling Output Bit Number
uint8 DNMLD = 8;        	// DN Micro Leveling Output Data Array Number

int16 FSDO = BRD3C;     	// Front Swing Door Open Output Board Address
uint8 FSDOB = BIT1;      // Front Swing Door Open Output Bit Number
uint8 FSDOD = 8;        	// Front Swing Door Open Output Data Array Number

int16 RSDO = BRD3C;     	// Rear Swing Door Open Output Board Address
uint8 RSDOB = BIT8;      // Rear Swing Door Open Output Bit Number
uint8 RSDOD = 8;        	// Rear Swing Door Open Output Data Array Number


// Bit Address of I/O on Board #4

int16 IU = BRD4A;      	// Inspection Up Input Board Address
uint8 IUB = BIT1;       	// Inspection Up Input Bit Number
uint8 IUD = 9;         	// Inspection Up Input Data Array Number

int16 ID = BRD4A;      	// Inspection Down Input Board Address
uint8 IDB = BIT2;       	// Inspection Down Input Bit Number
uint8 IDD = 9;          	// Inspection Down Input Data Array Number

int16 STP = BRD4A;      // Stop Switch Input Board Address
uint8 STPB = BIT3;       // Stop Switch Input Bit Number
uint8 STPD = 9;          // Stop Switch Input Data Array Number

int16 ICI = BRD4A;		// In-Car Inspection Input Board Address
uint8 ICIB = BIT4;		// In-Car Inspection Input Bit Number
uint8 ICID = 9;   		// In-Car Inspection Input Data Array Number

int16 MRIN = BRD4A;		// Motor Room Inspection Input Board Address
uint8 MRINB = BIT5;		// Motor Room Inspection Input Bit Number
uint8 MRIND = 9;   		// Motor Room Inspection Input Data Array Number

int16 MRIU = BRD4A;		// Motor Room Inspection Up Input Board Address
uint8 MRIUB = BIT7;		// Motor Room Inspection Up Input Bit Number
uint8 MRIUD = 9;   		// Motor Room Inspection Up Input Data Array Number

int16 MRID = BRD4A;		// Motor Room Inspection Down Input Board Address
uint8 MRIDB = BIT6;		// Motor Room Inspection Down Input Bit Number
uint8 MRIDD = 9;   		// Motor Room Inspection Down Input Data Array Number

int16 AUTO = BRD4A;		// Auto Input Board Address
uint8 AUTOB = BIT8;		// Auto Down Run Input Bit Number
uint8 AUTOD = 9;   		// Auto Down Run Input Data Array Number

int16 P = BRD4B;      	// Potential Input Board Address
uint8 PB = BIT1;       	// Potential Input Bit Number
uint8 PD = 10;          	// Potential Input Data Array Number

int16 FSTI = BRD4B;		// Fire Stop Switch Bypass Input Board Address
uint8 FSTIB = BIT2;		// Fire Stop Switch Bypass Input Bit Number
uint8 FSTID = 10;   		// Fire Stop Switch Bypass Input Data Array Number

int16 GTS = BRD4B;		// Gripper Trip Switch Input Board Address
uint8 GTSB = BIT3;		// Gripper Trip Switch Input Bit Number
uint8 GTSD = 10;			// Gripper Trip Switch Input Data Array Number

int16 READY = BRD4B;    	// Ready Input Board Address
uint8 READYB = BIT4;     // Ready Input Bit Number
uint8 READYD = 10;       // Ready Input Data Array Number

int16 GRT1 = BRD4B;		// Gripper Control 1 Input Board Address
uint8 GRT1B = BIT5;		// Gripper Control 1 Input Bit Number
uint8 GRT1D = 10;   		// Gripper Control 1 Input Data Array Number

int16 GRT2 = BRD4B;		// Gripper Control 2 Input Board Address
uint8 GRT2B = BIT6;		// Gripper Control 2 Input Bit Number
uint8 GRT2D = 10;   		// Gripper Control 2 Input Data Array Number

int16 BRKI = BRD4B;		// Brake Control Input Board Address
uint8 BRKIB = BIT7;		// Brake Control Input Bit Number
uint8 BRKID = 10;   		// Brake Control Input Data Array Number

int16 DON = BRD4B;		// Drive On Input Board Address
uint8 DONB = BIT8;		// Drive On Input Bit Number
uint8 DOND = 10;   		// Drive On Input Data Array Number

int16 PFC = BRD4C;     	// Primary Fault Control Output Board Address
uint8 PFCB = BIT1;      	// Primary Fault Control Output Bit Number
uint8 PFCD = 11;        	// Primary Fault Control Output Data Array Number

int16 MCA = BRD4C;      	// Master Contactor Output Board Address
uint8 MCAB = BIT2;       	// Master Contactor Output Bit Number
uint8 MCAD = 11;         	// Master Contactor Output Data Array Number

int16 MCC = BRD4C;      // Motor Contactor Control Output Board Address
uint8 MCCB = BIT3;       // Motor Contactor Control Output Bit Number
uint8 MCCD = 11;         // Motor Contactor Control Output Data Array Number

int16 RST = BRD4C;       // Drive Reset Output Board Address
uint8 RSTB = BIT4;       // Drive Reset Output Bit Number
uint8 RSTD = 11;         // Drive Reset Output Data Array Number


// Bit Address of I/O on Board #5

int16 RUNI = BRD5A;		// Run Control Input Board Address
uint8 RUNIB = BIT1;		// Run Control Input Bit Number
uint8 RUNID = 12;   		// Run Control Input Data Array Number

int16 RUNAI = BRD5A; 	// Run Aux Control Input Board Address
uint8 RUNAIB = BIT2;		// Run Aux Control Input Bit Number
uint8 RUNAID = 12;    	// Run Aux Control Input Data Array Number

int16 MCCI = BRD5A;		// Motor Contactor Control Input Board Address
uint8 MCCIB = BIT3;		// Motor Contactor Control Input Bit Number
uint8 MCCID = 12;   		// Motor Contactor Control Input Data Array Number

int16 MCAI = BRD5A;		// Motor Contactor Input Board Address
uint8 MCAIB = BIT4;		// Motor Contactor Input Bit Number
uint8 MCAID = 12;			// Motor Contactor Input Data Array Number

int16 RGS = BRD5A;		// Rear Gate Input Board Address
uint8 RGSB = BIT5;		// Rear Gate Input Bit Number
uint8 RGSD = 12;  		// Rear Gate Input Data Array Number

int16 RGS_1 = BRD5A;  	// Rear Gate Switch - Secondary Input Board Address
uint8 RGS_1B = BIT6;   	// Rear Gate Switch - Secondary Input Bit Number
uint8 RGS_1D = 12;     	// Rear Gate Switch - Secondary Input Data Array Number

int16 SECFM = BRD5A;		// Security Floor Mask Select Input Board Address
uint8 SECFMB = BIT7;    	// Security Floor Mask Select Input Bit Number
uint8 SECFMD = 12;     	// Security Floor Mask Select Input Data Array Number

int16 CCPBS = BRD5A;	// Car Call Push Button Security Input Board Address(spi14 Terminal)
uint8 CCPBSB = BIT7;    	// Car Call Push Button Security Input Bit Number
uint8 CCPBSD = 12;      	// Car Call Push Button Security Input Data Array Number


// Note: SAB uses the same io location as SECFM
int16 SAB = BRD5A;		// Sabbath Service Input Board Address
uint8 SABB = BIT7;    	// Sabbath Service Input Bit Number
uint8 SABD = 12;     	// Sabbath Service Input Data Array Number

// Note: INDS uses the same io location as SECFM
int16 INDS = BRD5A;		// Independent Security Input Board Address
uint8 INDSB = BIT7;    	// Independent Security Input Bit Number
uint8 INDSD = 12;     	// Independent Security Input Data Array Number

int16 LVC = BRD5A;		// Leveling Clamp Input Board Address
uint8 LVCB = BIT8;		// Leveling Clamp Input Bit Number
uint8 LVCD = 12;   		// Leveling Clamp Input Data Array Number

int16 DNI = BRD5B;		// Down Run Input Board Address
uint8 DNIB = BIT1;		// Down Run Input Bit Number
uint8 DNID = 13;   		// Down Run Input Data Array Number

int16 DFI = BRD5B;		// Down Fast Run Input Board Address
uint8 DFIB = BIT2;		// Down Fast Input Bit Number
uint8 DFID = 13;   		// Down Fast Input Data Array Number

int16 UPI = BRD5B;		// Up Run Input Board Address
uint8 UPIB = BIT3;		// Up Run Input Bit Number
uint8 UPID = 13;   		// Up Run Input Data Array Number

int16 UFI = BRD5B;		// Up Fast Run Input Board Address
uint8 UFIB = BIT4;		// Up Fast Run Input Bit Number
uint8 UFID = 13;   		// Up Fast Run Input Data Array Number

int16 GOV = BRD5B;     	// Governor Input Board Address
uint8 GOVB = BIT7;       // Governor Input Bit Number
uint8 GOVD = 13;         // Governor Input Data Array Number

int16 PS = BRD5B;      	// Pit Switch Input Board Address
uint8 PSB = BIT8;       	// Pit Switch Input Bit Number
uint8 PSD = 13;         	// Pit Switch Input Data Array Number

// Note: Same output as EMLH
int16 GBL = BRD5C;		// Gate Bypass light output Board Address
uint8 GBLB = BIT1;		// Gate Bypass light output Bit Number
uint8 GBLD = 14; 		// Gate Bypass light output Data Array Number

int16 EQL = BRD5C;		// Earthquake light output Board Address
uint8 EQLB = BIT2;		// Earthquake light output Bit Number
uint8 EQLD = 14;			// Earthquake light output Data Array Number

// Note: Same output as EQL
int16 LBL = BRD5C;		// Lock Bypass light output Board Address
uint8 LBLB = BIT2;		// Lock Bypass light output Bit Number
uint8 LBLD = 14;			// Lock Bypass light output Data Array Number

int16 FSO = BRD5C;     	// Fire Service Output Board Address
uint8 FSOB = BIT3;      	// Fire Service Output Bit Number
uint8 FSOD = 14;        	// Fire Service Output Data Array Number

// Note: LD and ISER and ELOO share the same output
int16 ELOO = BRD5C;      	// Elevator Off Output Board Address
uint8 ELOOB = BIT4;       	// Elevator Off Output Bit Number
uint8 ELOOD = 14;         	// Elevator Off Output Data Array Number

int16 LD = BRD5C;      	// Down Hall Lantern Output Board Address
uint8 LDB = BIT4;       	// Down Hall Lantern Output Bit Number
uint8 LDD = 14;         	// Down Hall Lantern Output Data Array Number

int16 TCU = BRD5C;		// This car up output Board Address
uint8 TCUB = BIT5;		// This car up output Bit Number
uint8 TCUD = 14;			// This car up output Data Array Number 

// Note: LU and TCU share the same output
int16 LU = BRD5C;      	// Up Hall Lantern Output Board Address
uint8 LUB = BIT5;       	// Up Hall Lantern Output Bit Number
uint8 LUD = 14;         	// Up Hall Lantern Output Data Array Number

int16 MST = BRD5C;      // Motor Starter Y Delta Timer Output Board Address
uint8 MSTB = BIT6;       // Motor Starter Y Delta Timer Output Bit Number
uint8 MSTD = 14;         // Motor Starter Y Delta Timer Output Data Array Number

#if ((Traction == 1) || (Tract_OL == 1))

	// GOVR Same outpu as MST
	int16 GOVR = BRD5C;     // Governor Reset Output Board Address
	uint8 GOVRB = BIT6;      // Governor Reset Output Bit Number
	uint8 GOVRD = 14;        // Governor Reset Output Data Array Number

#else

	// GOVR Same outpu as EMLH
	int16 GOVR = BRD5C;     // Governor Reset Output Board Address
	uint8 GOVRB = BIT1;      // Governor Reset Output Bit Number
	uint8 GOVRD = 14;        // Governor Reset Output Data Array Number

#endif

int16 RCF = BRD3C;      // Retiring Cam for freight Output Board Address
uint8 RCFB = BIT4;       // Retiring Cam for freight Output Bit Number	 
uint8 RCFD = 8;         	// Retiring Cam for freight Output Data Array Number

int16 GEN = BRD5C;      // Generator Output Board Address
uint8 GENB = BIT7;       // Generator Output Bit Number
uint8 GEND = 14;         // Generator Output Data Array Number

// Note: MTB uses the same io location as GEN
int16 MTB = BRD5C;      // Motor Blower Output Board Address
uint8 MTBB = BIT7;       // Motor Blower Output Bit Number
uint8 MTBD = 14;         // Motor Blower Output Data Array Number

// GOVT Same outpu as MTB
int16 GOVT = BRD5C;     // Governor Trip Output Board Address
uint8 GOVTB = BIT7;      // Governor Trip Output Bit Number
uint8 GOVTD = 14;        // Governor Trip Output Data Array Number

int16 FF = BRD5C;       	// Field Force BRD ADD
uint8 FFB = BIT8;        // Field Force Bit Number
uint8 FFD = 14;          // Field Force Data Array Number



// Bit Address of I/O on Board #6

int16 RUNA = BRD6A;      // Run Aux Output Board Address
uint8 RUNAB = BIT1;      // Run Aux Output Bit Number
uint8 RUNAD = 15;        // Run Aux Output Data Array Number

int16 RUN = BRD6A;     	// Run Output Board Address
uint8 RUNB = BIT2;      	// Run Output Bit Number
uint8 RUND = 15;        	// Run Output Data Array Number

int16 GR1R = BRD6A;     // Rope Gripper 1 Output Board Address
uint8 GR1RB = BIT3;      // Rope Gripper 1 Output Bit Number
uint8 GR1RD = 15;         // Rope Gripper 1 Output Data Array Number

int16 GR2R = BRD6A;     // Rope Gripper 2 Output Board Address
uint8 GR2RB = BIT4;      // Rope Gripper 2 Output Bit Number
uint8 GR2RD = 15;        // Rope Gripper 2 Output Data Array Number

int16 LE = BRD6A;       	// Level Enable Output Board Address
uint8 LEB = BIT5;        // Level Enable Output Bit Number
uint8 LED = 15;          // Level Enable Output Data Array Number

int16 LE1 = BRD6A;      	// Level Enable - Secondary Output Board Address
uint8 LE1B = BIT6;       // Level Enable - Secondary Output Bit Number
uint8 LE1D = 15;         // Level Enable - Secondary Output Data Array Number

int16 FST = BRD6A;    	// Second Fire Stop Bypass Output Board Address
uint8 FSTB = BIT7;     	// Second Fire Stop Bypass Output Bit Number
uint8 FSTD = 15;        	// Second Fire Stop Bypass Output Data Array Number

int16 FSTP = BRD6A;     // Fire Stop Bypass Output Board Address
uint8 FSTPB = BIT8;      // Fire Stop Bypass Output Bit Number
uint8 FSTPD = 15;        // Fire Stop Bypass Output Data Array Number

int16 DNR = BRD6B;       // Down Relay Output Board Address
uint8 DNRB = BIT1;       // Down Relay Output Bit Number
uint8 DNRD = 16;         // Down Relay Output Data Array Number

int16 DF = BRD6B;       // Down Fast Output Board Address
uint8 DFB = BIT2;        // Down Fast Output Bit Number
uint8 DFD = 16;          // Down Fast Output Data Array Number

int16 UP = BRD6B;       // Up Relay Output Board Address
uint8 UPB = BIT3;        // Up Relay Output Bit Number
uint8 UPD = 16;          // Up Relay Output Data Array Number

int16 UPF = BRD6B;      // Up Fast Output Board Address
uint8 UPFB = BIT4;       // Up Fast Output Bit Number
uint8 UPFD = 16;         // Up Fast Output Data Array Number

int16 BRK = BRD6B;      // Brake Output Board Address
uint8 BRKB = BIT5;       // Brake Output Bit Number
uint8 BRKD = 16;         // Brake Output Data Array Number

int16 BRKC = BRD6B;     // Brake Cooling Output Board Address
uint8 BRKCB = BIT6;      // Brake Cooling Output Bit Number
uint8 BRKCD = 16;        // Brake Cooling Output Data Array Number

int16 DBC = BRD6B;      // Dynamic Braking Resistor Control Output Board Address
uint8 DBCB = BIT7;       // Dynamic Braking Resistor Control Output Bit Number
uint8 DBCD = 16;         // Dynamic Braking Resistor Control Output Data Array Number

int16 BKS1 = BRD6C;		// Brake Lift switch 1 Input Board Address
uint8 BKS1B = BIT1;		// Brake Lift switch 1 Input Bit Number
uint8 BKS1D = 17;		// Brake Lift switch 1 Input Data Array Number

int16 BKS2 = BRD6C;		// Brake Lift switch 2 Input Board Address
uint8 BKS2B = BIT2;		// Brake Lift switch 2 Input Bit Number
uint8 BKS2D = 17;		// Brake Lift switch 2 Input Data Array Number

int16 BDBS = BRD6C;		// Brake Drive Bypass Switch Input Board Address
uint8 BDBSB = BIT3;		// Brake Drive Bypass Switch Input Bit Number
uint8 BDBSD = 17;		// Brake Drive Bypass Switch Input Data Array Number

int16 XBKS1 = BRD6C;		// Brake Lift switch 1 Input Board Address
uint8 XBKS1B = BIT4;		// Brake Lift switch 1 Input Bit Number
uint8 XBKS1D = 17;		// Brake Lift switch 1 Input Data Array Number

int16 XBKS2 = BRD6C;		// Brake Lift switch 2 Input Board Address
uint8 XBKS2B = BIT5;		// Brake Lift switch 2 Input Bit Number
uint8 XBKS2D = 17;		// Brake Lift switch 2 Input Data Array Number

int16 XBDBS = BRD6C;		// Brake Drive Bypass Switch Input Board Address
uint8 XBDBSB = BIT6;		// Brake Drive Bypass Switch Input Bit Number
uint8 XBDBSD = 17;		// Brake Drive Bypass Switch Input Data Array Number


//*********************************************************************
//********** Serial Car Comm I/O 's  **********************************
//*********************************************************************


 // Bit Address for board # 8

int16 INSEC = BRD8C;    // Door Open Light Output Board Address
uint8 INSECB = BIT6;     // Door Open Light Output Bit Number
uint8 INSECD = 23;       // Door Open Light Output Data Array Number

int16 OPENL = BRD8C;    // Door Open Light Output Board Address
uint8 OPENLB = BIT7;     // Door Open Light Output Bit Number
uint8 OPENLD = 23;       // Door Open Light Output Data Array Number

int16 FSOCS = BRD8C;    // Fire Service Output Board Address
uint8 FSOCSB = BIT8;     // Fire Service Output Bit Number
uint8 FSOCSD = 23;       // Fire Service Output Data Array Number


 // Bit Address for board # 9

int16 TF = BRD9A;		// Top Final Input Board Address
uint8 TFB = BIT1;		// Top Final Input Bit Number
uint8 TFD = 24;			// Top Final Input Data Array Number

int16 BF = BRD9A;		// Bottom Final Input Board Address
uint8 BFB = BIT2;		// Bottom Final Input Bit Number
uint8 BFD = 24;			// Bottom Final Input Data Array Number

int16 PDS = BRD9A;		// Pit Door Switch Input Board Address
uint8 PDSB = BIT3;		// Pit Door Switch Bit Number
uint8 PDSD = 24;			// Pit Door Switch Data Array Number

int16 BFS = BRD9A;		// Buffer Switch Input Board Address
uint8 BFSB = BIT4;		// Buffer Switch Input Bit Number
uint8 BFSD = 24;			// Buffer Switch Input Data Array Number

int16 CMP = BRD9A;		// Compensating Ropw Switch Input Board Address
uint8 CMPB = BIT5;		// Compensating Ropw Switch Input Bit Number
uint8 CMPD = 24;			// Compensating Ropw Switch Input Data Array Number

int16 TEE = BRD9A;		// Top Emergency Exit Switch Input Board Address
uint8 TEEB = BIT6;		// Top Emergency Exit Switch Input Bit Number
uint8 TEED = 24;			// Top Emergency Exit Switch Input Data Array Number

int16 SEE = BRD9A;		// Side Emergency Exit Switch Input Board Address
uint8 SEEB = BIT7;		// Side Emergency Exit Switch Input Bit Number
uint8 SEED = 24;			// Side Emergency Exit Switch Input Data Array Number

int16 CSS = BRD9A;		// Car Safety Switch Input Board Address
uint8 CSSB = BIT8;		// Car Safety Switch Input Bit Number
uint8 CSSD = 24;			// Car Safety Switch Input Data Array Number
						   
int16 CCP = BRD9B;		// Car Call Power Input Board Address
uint8 CCPB = BIT1;		// Car Call Power Input Bit Number
uint8 CCPD = 25;			// Car Call Power Input Data Array Number

int16 CCLP =  BRD9B;		// Car Call Light Power Input Board Address
uint8 CCLPB = BIT2;		// Car Call Light Power Input Bit Number
uint8 CCLPD = 25;			// Car Call Light Power Input Data Array Number

int16 HCLP = BRD9B;		// Hall Call Light Power Input Board Address
uint8 HCLPB = BIT3;		// Hall Call Light Power Input Bit Number
uint8 HCLPD = 25;			// Hall Call Light Power Input Data Array Number

int16 LHCP = BRD9B;		// Lobby Hall Call Common Power Input Board Address
uint8 LHCPB = BIT4;		// Lobby Hall Call Common Power Input Bit Number
uint8 LHCPD = 25;			// Lobby Hall Call Common Power Input Data Array Number

int16 HMO = BRD9B;		// Hoistway Motor Overload Input Board Address
uint8 HMOB = BIT5;		// Hoistway Motor Overload Input Bit Number
uint8 HMOD = 25;			// Hoistway Motor Overload Input Data Array Number

int16 DMO_ = BRD9B;		// Door Motor Overload Input Board Address
uint8 DMO_B = BIT6;		// Door Motor Overload Input Bit Number
uint8 DMO_D = 25;			// Door Motor Overload Input Data Array Number




// Board #12

int16 FSORS = BRD12C;     	// Fire Service Output Board Address
uint8 FSORSB = BIT8;      	// Fire Service Output Bit Number
uint8 FSORSD = 35;        	// Fire Service Output Data Array Number

// Top of Car I/O's

// Block 25

int16 BP1 = BRD25A;		// Binary Position #1 Input Board Address
uint8 BP1B = BIT1;		// Binary Position #1 Input Bit Number
uint8 BP1D = 72;   		// Binary Position #1 Input Data Array Number

int16 BP2 = BRD25A;		// Binary Position #2 Input Board Address
uint8 BP2B = BIT2;		// Binary Position #2 Input Bit Number
uint8 BP2D = 72;   		// Binary Position #2 Input Data Array Number

int16 BP4 = BRD25A;		// Binary Position #4 Input Board Address
uint8 BP4B = BIT3;		// Binary Position #4 Input Bit Number
uint8 BP4D = 72;   		// Binary Position #4 Input Data Array Number

int16 BP8 = BRD25A;   	// Binary Position #8 Input Board Address
uint8 BP8B = BIT4;     	// Binary Position #8 Input Bit Number
uint8 BP8D = 72;      	// Binary Position #8 Input Data Array Number

int16 BP16 = BRD25A;   	// Binary Position #16 Input Board Address
uint8 BP16B = BIT5;     	// Binary Position #16 Input Bit Number
uint8 BP16D = 72;      	// Binary Position #16 Input Data Array Number

//int16 EP = BRD25A;      // Even Parity Input Board Address
//uint8 EPB = BIT6;       	// Even Parity Input Bit Numbe	
//uint8 EPD = 72;          // Up Slowdown Input Data Array Number

int16 DZSA = BRD41A;      // DZSA (both dz sensors ored) Input Board Address
uint8 DZSAB = BIT4;       // DZSA (both dz sensors ored) Input Bit Number	
uint8 DZSAD = 120;        // DZSA (both dz sensors ored) Input Data Array Number

int16 DNS = BRD41A;      // DNS Dn normal limit Input Input Board Address
uint8 DNSB = BIT5;       // DNS Dn normal limit Input Input Bit Number	
uint8 DNSD = 120;        // DNS Dn normal limit Input Input Data Array Number

int16 UNS = BRD41B;      // UNS Up normal limit Input Board Address
uint8 UNSB = BIT1;       // UNS Up normal limit Input Input Bit Number	
uint8 UNSD = 121;        // UNS Up normal limit Input Input Data Array Number

int16 BP32 = BRD25A;   	// Binary Position #32 Input Board Address
uint8 BP32B = BIT7;     	// Binary Position #32 Input Bit Number
uint8 BP32D = 72;      	// Binary Position #32 Input Data Array Number

int16 US = BRD25A;      // Up Slowdown Input Board Address
uint8 USB = BIT7;       	// Up Slowdown Input Bit Numbe	
uint8 USD = 72;          // Up Slowdown Input Data Array Number

int16 DS = BRD25A;      // Down Slowdown Input Board Address
uint8 DSB = BIT8;       	// Down Slowdown Input Bit Number
uint8 DSD = 72;         	// Down Slowdown Input Data Array Number

int16 SE = BRD25B;     // Saftey Edge Input Board Address
uint8 SEB = BIT4;        // Saftey Edge Input Bit Number
uint8 SED = 73;          // Saftey Edge Input Data Array Number

int16 LWA = BRD25B;   	// Load Weighing Anti-nuisance Input Board Address
uint8 LWAB = BIT5;	    // Load Weighing Anti-nuisance Input Bit Number
uint8 LWAD = 73;			// Load Weighing Anti-nuisance Input Data Array Number

int16 OVL = BRD25B;     	// Over Load Input Board Address
uint8 OVLB = BIT6;     	// Over Load Input Bit Number
uint8 OVLD = 73;			// Over Load Input Data Array Number

// Note: Same input as OVL
int16 ELOF = BRD25B;    	// Elevator Off Input Board Address
uint8 ELOFB = BIT6;     	// Elevator Off Input Bit Number
uint8 ELOFD = 73;		// Elevator Off Input Data Array Number

int16 LWB = BRD25B;		// Load Weighing Bypass Input Board Address
uint8 LWBB = BIT7;		// Load Weighing Bypass Input Bit Number
uint8 LWBD = 73;			// Load Weighing Bypass Input Data Array Number

int16 LWD = BRD25B;     	// Load Weighing Dispatch Input Board Address
uint8 LWDB = BIT8;     	// Load Weighing Dispatch Input Bit Number
uint8 LWDD = 73;       	// Load Weighing Dispatch Input Data Array Number

int16 FBNBCT = BRD25C;   // Fire buzzer Output Board Address
uint8 FBNBCTB = BIT5;    // Fire buzzer Output Bit Number
uint8 FBNBCTD = 74;      // Fire buzzer Output Data Array Number

int16 FLCT = BRD25C;   	// Fire light Output Board Address
uint8 FLCTB = BIT7;    	// Fire light Output Bit Number
uint8 FLCTD = 74;      	// Fire light Output Data Array Number


// Block 22	  Peelle Door Operator

int16 INSP = BRD22B;      // Inspection output for peelle door interface Board Address
uint8 INSPB = BIT5;       // Inspection output for peelle door interface Bit Number
uint8 INSPD = 64;         // Inspection output for peelle door interface Data Array Number

int16 ADO = BRD22B;      // Automatic door open output for peelle door interface Board Address
uint8 ADOB = BIT6;       // Automatic door open output for peelle door interface Bit Number
uint8 ADOD = 64;         // Automatic door open output for peelle door interface Data Array Number

int16 PH1R = BRD22B;     // Phase 1 recall output for peelle door interface Board Address
uint8 PH1RB = BIT7;      // Phase 1 recall output for peelle door interface Bit Number
uint8 PH1RD = 64;        // Phase 1 recall output for peelle door interface Data Array Number

int16 DESL = BRD22B;     // Designated landing output for peelle door interface Board Address
uint8 DESLB = BIT8;      // Designated landing output for peelle door interface Bit Number
uint8 DESLD = 64;        // Designated landing output for peelle door interface Data Array Number

int16 PH2ON = BRD22C;     // Phase 2 on output for peelle door interface Board Address
uint8 PH2ONB = BIT1;      // Phase 2 on output for peelle door interface Bit Number
uint8 PH2OND = 65;        // Phase 2 on output for peelle door interface Data Array Number

int16 PH2HD = BRD22C;     // Phase 2 hold output for peelle door interface Board Address
uint8 PH2HDB = BIT2;      // Phase 2 hold output for peelle door interface Bit Number
uint8 PH2HDD = 65;        // Phase 2 hold output for peelle door interface Data Array Number

int16 PH2OF = BRD22C;     // Phase 2 off output for peelle door interface Board Address
uint8 PH2OFB = BIT3;      // Phase 2 off output for peelle door interface Bit Number
uint8 PH2OFD = 65;        // Phase 2 off output for peelle door interface Data Array Number

int16 HDB = BRD22C;       // Hall button cut-off output for peelle door interface Board Address
uint8 HDBB = BIT4;        // Hall button cut-off output for peelle door interface Bit Number
uint8 HDBD = 65;          // Hall button cut-off output for peelle door interface Data Array Number

int16 RC = BRD22C;  		// retiring cam output for peelle door interface board address.
uint8 RCB = BIT5;		// retiring cam output for peelle door interface bit number.
uint8 RCD = 65;			// retiring cam output for peelle door interface data array number.

int16 RDT = BRD22C;  	// Rear door transfer relay peelle door interface board address.
uint8 RDTB = BIT6;		// Rear door transfer relay peelle door interface bit number.
uint8 RDTD = 65;			// Rear door transfer relay peelle door interface data array number.

int16 RADO = BRD22C;      // Automatic door open output for peelle door interface Board Address
uint8 RADOB = BIT7;       // Automatic door open output for peelle door interface Bit Number
uint8 RADOD = 65;         // Automatic door open output for peelle door interface Data Array Number

int16 DAO = BRD22C;		// Designated Attendant Operation output for peelle door interface Board Address
uint8 DAOB = BIT8;       // Designated Attendant Operation output for peelle door interface Bit Number
uint8 DAOD = 65;         // Designated Attendant Operation output for peelle door interface Data Array Number


//NEW GUILBERT DOOR INTERFACE

int16 G_FDCBO = BRD22B;
uint8 G_FDCBOB = BIT3; 
uint8 G_FDCBOD = 64;  

int16 G_RDCBO = BRD22B;
uint8 G_RDCBOB = BIT4; 
uint8 G_RDCBOD = 64;  

int16 G_FH1R = BRD22B;
uint8 G_FH1RB = BIT5; 
uint8 G_FH1RD = 64;   

int16 G_PH2ON = BRD22B;
uint8 G_PH2ONB = BIT6; 
uint8 G_PH2OND = 64;   

int16 G_PH2HD = BRD22B;
uint8 G_PH2HDB = BIT7; 
uint8 G_PH2HDD = 64;   

int16 G_PH2OF = BRD22B; 
uint8 G_PH2OFB = BIT8;  
uint8 G_PH2OFD = 64;    

int16 G_INSP = BRD22C;
uint8 G_INSPB = BIT1; 
uint8 G_INSPD = 65;   

int16 G_SOPEN = BRD22C;
uint8 G_SOPENB = BIT2; 
uint8 G_SOPEND = 65;   

int16 G_ADO = BRD22C;
uint8 G_ADOB = BIT3; 
uint8 G_ADOD = 65;   

int16 G_ADOH = BRD22C;
uint8 G_ADOHB = BIT4; 
uint8 G_ADOHD = 65;   

int16 G_RDO = BRD22C; 
uint8 G_RDOB = BIT5;	
uint8 G_RDOD = 65;		

int16 G_RDOH = BRD22C;
uint8 G_RDOHB = BIT6;	
uint8 G_RDOHD = 65;		

int16 G_RC = BRD22C;
uint8 G_RCB = BIT7; 
uint8 G_RCD = 65;   

int16 G_ININD = BRD22C;
uint8 G_ININDB = BIT8; 
uint8 G_ININDD = 65; 


// Block 22	Courion Door operator

int16 MPTR = BRD22B;      // Power-up reset output for courion door interface Board Address
uint8 MPTRB = BIT5;       // Power-up reset output for courion door interface Bit Number
uint8 MPTRD = 64;         // Power-up reset output for courion door interface Data Array Number

int16 FPH2 = BRD22B;      // Phase 2 output for courion door interface Board Address
uint8 FPH2B = BIT6;       // Phase 2 output for courion door interface Bit Number
uint8 FPH2D = 64;         // Phase 2 output for courion door interface Data Array Number

int16 CODG = BRD22B;     // Door/Gate Cut-out output for courion door interface Board Address
uint8 CODGB = BIT7;      // Door/Gate Cut-out output for courion door interface Bit Number
uint8 CODGD = 64;        // Door/Gate Cut-out output for courion door interface Data Array Number

int16 IAO = BRD22B;      // Automatic door open output for courion door interface Board Address
uint8 IAOB = BIT8;       // Automatic door open output for courion door interface Bit Number
uint8 IAOD = 64;         // Automatic door open output for courion door interface Data Array Number

int16 COHS = BRD22C;     // Hall Station Cut-out output for courion door interface Board Address
uint8 COHSB = BIT1;      // Hall Station Cut-out output for courion door interface Bit Number
uint8 COHSD = 65;        // Hall Station Cut-out output for courion door interface Data Array Number

//int16 PH2HD = BRD22C;      // Phase 2 hold output for courion door interface Board Address
//uint8 PH2HDB = BIT2;       // Phase 2 hold output for courion door interface Bit Number
//uint8 PH2HDD = 65;         // Phase 2 hold output for courion door interface Data Array Number

int16 FPH1 = BRD22C;       // Phase 1 recall output for courion door interface Board Address
uint8 FPH1B = BIT3;        // Phase 1 recall output for courion door interface Bit Number
uint8 FPH1D = 65;         // Phase 1 recall output for courion door interface Data Array Number

int16 ATO = BRD22C;       // Automatic timed close cut-out output for courion door interface Board Address
uint8 ATOB = BIT4;        // Automatic timed close cut-out output for courion door interface Bit Number
uint8 ATOD = 65;          // Automatic timed close cut-out output for courion door interface Data Array Number

//int16 RC = BRD22C;  		// retiring cam output for courion door interface board address.
//uint8 RCB = BIT5;		// retiring cam output for courion door interface bit number.
//uint8 RCD = 65;			// retiring cam output for courion door interface data array number.

int16 RAHS = BRD22C;		// Re-activate hall stations for phase 1 recall from phase 2 for courion door interface
uint8 RAHSB = BIT6;		// Re-activate hall stations for phase 1 recall from phase 2 for courion door interface
uint8 RAHSD = 65;		// Re-activate hall stations for phase 1 recall from phase 2 for courion door interface

//int16 RADO = BRD22C;      // Automatic door open output for courion door interface Board Address
//uint8 RADOB = BIT7;       // Automatic door open output for courion door interface Bit Number
//uint8 RADOD = 65;         // Automatic door open output for courion door interface Data Array Number

// Block 22	 EMS Door operator
int16 EMS3Z = BRD22A;  	// Floor zoning for floor 3 on EMS operator Board Address
uint8 EMS3ZB = BIT5;		// Floor zoning for floor 3 on EMS operator Bit Number
uint8 EMS3ZD = 63;		// Floor zoning for floor 3 on EMS operator Data Array Number

int16 EMS4Z = BRD22A;  	// Floor zoning for floor 4 on EMS operator Board Address
uint8 EMS4ZB = BIT6;		// Floor zoning for floor 4 on EMS operator Bit Number
uint8 EMS4ZD = 63;		// Floor zoning for floor 4 on EMS operator Data Array Number

int16 EMS5Z = BRD22A;  	// Floor zoning for floor 5 on EMS operator Board Address
uint8 EMS5ZB = BIT7;		// Floor zoning for floor 5 on EMS operator Bit Number
uint8 EMS5ZD = 63;		// Floor zoning for floor 5 on EMS operator Data Array Number

int16 EMS6Z = BRD22A;  	// Floor zoning for floor 6 on EMS operator Board Address
uint8 EMS6ZB = BIT8;		// Floor zoning for floor 6 on EMS operator Bit Number
uint8 EMS6ZD = 63;		// Floor zoning for floor 6 on EMS operator Data Array Number

int16 EMS7Z = BRD22B;  	// Floor zoning for floor 7 on EMS operator Board Address
uint8 EMS7ZB = BIT1;		// Floor zoning for floor 7 on EMS operator Bit Number
uint8 EMS7ZD = 64;		// Floor zoning for floor 7 on EMS operator Data Array Number

int16 EMS8Z = BRD22B;  	// Floor zoning for floor 8 on EMS operator Board Address
uint8 EMS8ZB = BIT2;		// Floor zoning for floor 8 on EMS operator Bit Number
uint8 EMS8ZD = 64;		// Floor zoning for floor 8 on EMS operator Data Array Number

int16 EMS9Z = BRD22B;  	// Floor zoning for floor 9 on EMS operator Board Address
uint8 EMS9ZB = BIT3;		// Floor zoning for floor 9 on EMS operator Bit Number
uint8 EMS9ZD = 64;		// Floor zoning for floor 9 on EMS operator Data Array Number

int16 EMS10Z = BRD22B;  	// Floor zoning for floor 10 on EMS operator Board Address
uint8 EMS10ZB = BIT4;	// Floor zoning for floor 10 on EMS operator Bit Number
uint8 EMS10ZD = 64;		// Floor zoning for floor 10 on EMS operator Data Array Number

int16 EMS11Z = BRD22A;  	// Floor zoning for floor 11 on EMS operator Board Address
uint8 EMS11ZB = BIT1;	// Floor zoning for floor 11 on EMS operator Bit Number
uint8 EMS11ZD = 63;		// Floor zoning for floor 11 on EMS operator Data Array Number

int16 DSC = BRD22B;      // Disable power operation of doors & gates for EMS operator Board Address
uint8 DSCB = BIT5;       // Disable power operation of doors & gates for EMS operator Bit Number
uint8 DSCD = 64;         // Disable power operation of doors & gates for EMS opeartor Data Array Number

int16 FDOB = BRD22B;      // Automatically open front door & gates for EMS operator Board Address
uint8 FDOBB = BIT6;       // Automatically open front door & gates for EMS operator interface Bit Number
uint8 FDOBD = 64;         // Automatically open front door & gates for EMS operator Data Array Number

int16 RDOBR = BRD22B;     // Automatically open rear door & gates for EMS operator Board Address
uint8 RDOBB = BIT7;      // Automatically open rear door & gates for EMS operator Bit Number
uint8 RDOBD = 64;        // Automatically open rear door & gates for EMS operator Data Array Number

int16 HCO = BRD22B;      // Disable door operator buttons in hall for EMS operator Board Address
uint8 HCOB = BIT8;       // Disable door operator buttons in hall for EMS operator Bit Number
uint8 HCOD = 64;         // Disable door operator buttons in hall for EMS operator Data Array Number

int16 FRCL = BRD22C;      // Closing sequence on fire phase 1 for EMS operator Board Address
uint8 FRCLB = BIT1;       // Closing sequence on fire phase 1 for EMS operator Bit Number
uint8 FRCLD = 65;         // Closing sequence on fire phase 1 for EMS operator Array Number

int16 FPS2 = BRD22C;      // Phase 2 on position for EMS operator Board Address
uint8 FPS2B = BIT2;       // Phase 2 on position for EMS operator Bit Number
uint8 FPS2D = 65;         // Phase 2 on position for EMS operator Data Array Number

int16 FH = BRD22C;       // Phase 2 hold position for EMS operator Board Address
uint8 FHB = BIT3;        // Phase 2 hold position for EMS operator Bit Number
uint8 FHD = 65;         // Phase 2 hold position for EMS operator Data Array Number

int16 TC_ = BRD22C;       // Automatic timed close cut-out output for EMS operator Board Address
uint8 TC_B = BIT4;        // Automatic timed close cut-out output for EMS operator Bit Number
uint8 TC_D = 65;          // Automatic timed close cut-out output for EMS operator Data Array Number

//int16 RC = BRD22C;  		// Floor zoning for floor 1 on EMS operator Board Address
//uint8 RCB = BIT5;		// Floor zoning for floor 1 on EMS operator Board Bit number
//uint8 RCD = 65;			// Floor zoning for floor 1 on EMS operator Data Array Number

int16 EMS1Z = BRD22C;  	// Floor zoning for floor 1 on EMS operator Board Address
uint8 EMS1ZB = BIT6;		// Floor zoning for floor 1 on EMS operator Board Bit number
uint8 EMS1ZD = 65;		// Floor zoning for floor 1 on EMS operator Data Array Number

int16 EMS2Z = BRD22C;  	// Floor zoning for floor 2 on EMS operator Board Address
uint8 EMS2ZB = BIT7;		// Floor zoning for floor 2 on EMS operator Bit Number
uint8 EMS2ZD = 65;		// Floor zoning for floor 2 on EMS operator Data Array Number

int16 EFST = BRD22C;		// FST output for EMS door operator Board Address
uint8 EFSTB = BIT8;		// FST output for EMS door operator Bit Number
uint8 EFSTD = 65;		// FST output for EMS door operator Data Array Number


// Block 22	 Front and/or rear automatic freight doors

int16 FRT1Z = BRD22C;  	// Floor zoning for floor 3 on EMS operator Board Address
uint8 FRT1ZB = BIT2;		// Floor zoning for floor 3 on EMS operator Bit Number
uint8 FRT1ZD = 65;		// Floor zoning for floor 3 on EMS operator Data Array Number

int16 FRT2Z = BRD22C;  	// Floor zoning for floor 3 on EMS operator Board Address
uint8 FRT2ZB = BIT3;		// Floor zoning for floor 3 on EMS operator Bit Number
uint8 FRT2ZD = 65;		// Floor zoning for floor 3 on EMS operator Data Array Number

int16 FRT3Z = BRD22C;  	// Floor zoning for floor 3 on EMS operator Board Address
uint8 FRT3ZB = BIT4;		// Floor zoning for floor 3 on EMS operator Bit Number
uint8 FRT3ZD = 65;		// Floor zoning for floor 3 on EMS operator Data Array Number

//int16 RC = BRD22C;  		// retiring cam output for courion door interface board address.
//uint8 RCB = BIT5;		// retiring cam output for courion door interface bit number.
//uint8 RCD = 65;			// retiring cam output for courion door interface data array number.

int16 PDO = BRD22C; 		// Power Door Open output for courion door interface board address.
uint8 PDOB = BIT6;		// Power Door Open output for courion door interface bit number.
uint8 PDOD = 65;			// Power Door Open output for courion door interface data array number.

int16 PDOR = BRD22C;		// Power Door Open output for courion door interface board address.
uint8 PDORB = BIT7;		// Power Door Open output for courion door interface bit number.
uint8 PDORD = 65;   		// Power Door Open output for courion door interface data array number.


// Block 27 Local I/O board


	// Sheave brake i/o for 2007 compliance

	// Original sheave brake I/O s

int16 EBKi = BRD27A;  	// Emergency Brake Contactor Input Board Address
uint8 EBKiB = BIT1;		// Emergency Brake Contactor Input Bit Number
uint8 EBKiD = 78;		// Emergency Brake Contactor Input Data Array Number
	// Same IO location as EBKi
int16 EBAi = BRD27A;  	// Emergency Brake Contactor A1,A2 Input Board Address
uint8 EBAiB = BIT1;		// Emergency Brake Contactor A1,A2 Input Bit Number
uint8 EBAiD = 78;		// Emergency Brake Contactor A1,A2 Input Data Array Number

int16 EBBi = BRD27A;  	// Emergency Brake Contactor B1,B2 Input Board Address
uint8 EBBiB = BIT3;		// Emergency Brake Contactor B1,B2 Input Bit Number
uint8 EBBiD = 78;		// Emergency Brake Contactor B1,B2 Input Data Array Number

// i/o Used for Emergency brake delayed drop

int16 EBK2i = BRD27A;  	// Emergency Brake EBK2 Input Board Address
uint8 EBK2iB = BIT6;	// Emergency Brake EBK2 Input Bit Number
uint8 EBK2iD = 78;		// Emergency Brake EBK2 Input Data Array Number

int16 EBK1i = BRD27A;  	// Emergency Brake EBK1 Input Board Address
uint8 EBK1iB = BIT7;	// Emergency Brake EBK1 Input Bit Number
uint8 EBK1iD = 78;		// Emergency Brake EBK1 Input Data Array Number

// Block 28 Local I/O board

	// Sheave brake i/o for 2007 compliance

int16 EBB2 = BRD28A;  	// Emergency Brake Contactor B2 Output Board Address
uint8 EBB2B = BIT6;		// Emergency Brake Contactor B2 Output Bit Number
uint8 EBB2D = 81;  		// Emergency Brake Contactor B2 Output Data Array Number

int16 EBB1 = BRD28A;  	// Emergency Brake Contactor B1 Output Board Address
uint8 EBB1B = BIT7;		// Emergency Brake Contactor B1 Output Bit Number
uint8 EBB1D = 81;  		// Emergency Brake Contactor B1 Output Data Array Number

int16 EBA2 = BRD28A;  	// Emergency Brake Contactor A2 Output Board Address
uint8 EBA2B = BIT8;		// Emergency Brake Contactor A2 Output Bit Number
uint8 EBA2D = 81;  		// Emergency Brake Contactor A2 Output Data Array Number

	// Original sheave brake I/O s
int16 EBK = BRD28B;  	// Emergency Brake Contactor Output Board Address
uint8 EBKB = BIT1;		// Emergency Brake Contactor Output Bit Number
uint8 EBKD = 82;			// Emergency Brake Contactor Output Data Array Number
	// Same IO location as EBK
int16 EBA1 = BRD28B;  	// Emergency Brake Contactor A1 Output Board Address
uint8 EBA1B = BIT1;		// Emergency Brake Contactor A1 Output Bit Number
uint8 EBA1D = 82;  		// Emergency Brake Contactor A1 Output Data Array Number

	// Original sheave brake I/O s
int16 EBKC = BRD28B;  	// Emergency Brake Cool Contactor Output Board Address
uint8 EBKCB = BIT2;		// Emergency Brake Cool Contactor Output Bit Number
uint8 EBKCD = 82;		// Emergency Brake Cool Contactor Output Data Array Number
	// Same IO locations as EBKC
int16 EBC = BRD28B;  	// Emergency Brake Cool Contactor Output Board Address
uint8 EBCB = BIT2;		// Emergency Brake Cool Contactor Output Bit Number
uint8 EBCD = 82;		// Emergency Brake Cool Contactor Output Data Array Number


// i/o Used for Emergency brake delayed drop

int16 EBK2 = BRD28A;  	// Emergency Brake Contactor 2 Output Board Address
uint8 EBK2B = BIT6;		// Emergency Brake Contactor 2 Output Bit Number
uint8 EBK2D = 81;  		// Emergency Brake Contactor 2 Output Data Array Number

int16 EBK1 = BRD28A;  	// Emergency Brake Contactor 1 Output Board Address
uint8 EBK1B = BIT7;		// Emergency Brake Contactor 1 Output Bit Number
uint8 EBK1D = 81;  		// Emergency Brake Contactor 1 Output Data Array Number

int16 UTM = BRD28B;  	// Unintended Motion Relay Output Board Address
uint8 UTMB = BIT3;		// Unintended Motion Relay Output Bit Number
uint8 UTMD = 82;		// Unintended Motion Relay Output Data Array Number


// Manual power loss brake

int16 MBP = BRD28B;  	// Manual Power Loss Brake Power Output Board Address
uint8 MBPB = BIT1;		// Manual Power Loss Brake Power Output Bit Number
uint8 MBPD = 82;			// Manual Power Loss Brake Power Output Data Array Number

int16 MB2 = BRD28B;  	// Manual Power Loss Brake Brake Pick 2 Output Board Address
uint8 MB2B = BIT2;		// Manual Power Loss Brake Brake Pick 2 Bit Number
uint8 MB2D = 82;			// Manual Power Loss Brake Brake Pick 2 Data Array Number

int16 MB1 = BRD28B;  	// Manual Power Loss Brake Brake Pick 1 Output Board Address
uint8 MB1B = BIT3;		// Manual Power Loss Brake Brake Pick 1 Bit Number
uint8 MB1D = 82;			// Manual Power Loss Brake Brake Pick 1 Data Array Number

// Emergency Power Recovery Single Phase UPS

int16 NPD = BRD28B;  	// Normal Power Drive Output Board Address
uint8 NPDB = BIT1;		// Normal Power Drive Output Bit Number
uint8 NPDD = 82;			// Normal Power Drive Output Data Array Number

int16 EPD = BRD28B;  	// Emergency Power Drive Output Board Address
uint8 EPDB = BIT2;		// Emergency Power Drive Bit Number
uint8 EPDD = 82;			// Emergency Power Drive Data Array Number

int16 EPO = BRD28B;  	// Emergency Power Off Output Board Address
uint8 EPOB = BIT3;		// Emergency Power Off Bit Number
uint8 EPOD = 82;			// Emergency Power Off Data Array Number

// Block 31 Local I/O board


int16 TGDO = BRD31B;		//Emergency TUG 5CC Input Board Address
uint8 TGDOB = BIT5;      //Emergency TUG 5CC Input Bit Number
uint8 TGDOD = 91;        //Emergency TUG 5CC Input Data Array Number

int16 TGDC = BRD31B;		//Emergency TUG 6CC Input Board Address
uint8 TGDCB = BIT6;      //Emergency TUG 6CC Input Bit Number
uint8 TGDCD = 91;        //Emergency TUG 6CC Input Data Array Number

int16 TGDOR = BRD31B;	//Emergency TUG 5CC Input Board Address
uint8 TGDORB = BIT7;     //Emergency TUG 5CC Input Bit Number
uint8 TGDORD = 91;       //Emergency TUG 5CC Input Data Array Number

int16 TGDCR = BRD31B;	//Emergency TUG 6CC Input Board Address
uint8 TGDCRB = BIT8;     //Emergency TUG 6CC Input Bit Number
uint8 TGDCRD = 91;       //Emergency TUG 6CC Input Data Array Number

int16 THDS = BRD31C;		//Emergency TUG 6CC Input Board Address
uint8 THDSB = BIT1;      //Emergency TUG 6CC Input Bit Number
uint8 THDSD = 92;        //Emergency TUG 6CC Input Data Array Number


// Block 32 Local I/O board

int16 TAVL = BRD32B;		// Car Available Output Board Address	
uint8 TAVLB = BIT2;		// Car Available Output Bit Number
uint8 TAVLD = 94;		// Car Available Output Data Array Number

int16 TAVL2 = BRD32B;		// Car Available Output Board Address	
uint8 TAVL2B = BIT3;		// Car Available Output Bit Number
uint8 TAVL2D = 94;		// Car Available Output Data Array Number

int16 TDOL = BRD32B;		// Door Open Limit Output Board Address	
uint8 TDOLB = BIT4;		// Door Open Limit Output Bit Number
uint8 TDOLD = 94;		// Door Open Limit Output Data Array Number

int16 TDCL = BRD32B;		// Door Close Limit Output Board Address	
uint8 TDCLB = BIT5;		// Door Close Limit Output Bit Number
uint8 TDCLD = 94;		// Door Close Limit Output Data Array Number

int16 TDOLR = BRD32B;	// Rear Door Open Limit Output Board Address	
uint8 TDOLRB = BIT6;		// Rear Door Open Limit Output Bit Number
uint8 TDOLRD = 94;		// Rear Door Open Limit Output Data Array Number

int16 TDCLR = BRD32B;	// Rear Door Close Limit Output Board Address	
uint8 TDCLRB = BIT7;		// Rear Door Close Limit Output Bit Number
uint8 TDCLRD = 94;		// Rear Door Close Limit Output Data Array Number

int16 TIND = BRD32B;		// Car In Independent Output Board Address	
uint8 TINDB = BIT8;		// Car In Independent Output Bit Number
uint8 TINDD = 94;		// Car In Independent Output Data Array Number
                        
int16 TMED = BRD32C;	 	// Car In Medical Service Output Board Address	
uint8 TMEDB = BIT1;	   	// Car In Medical Service Output Bit Number
uint8 TMEDD = 95;	   	// Car In Medical Service Output Data Array Number

int16 TFSO = BRD32C;   	// Car In Fire Service Output Board Address	
uint8 TFSOB = BIT2;	   	// Car In Fire Service Output Bit Number
uint8 TFSOD = 95;	   	// Car In Fire Service Output Data Array Number

int16 TP1 = BRD32C;		// Binary Position P1 Output Board Address	
uint8 TP1B = BIT3;		// Binary Position P1 Output Bit Number
uint8 TP1D = 95;			// Binary Position P1 Output Data Array Number

int16 TP2 = BRD32C;		// Binary Position P2 Output Board Address	
uint8 TP2B = BIT4;		// Binary Position P2 Output Bit Number
uint8 TP2D = 95;			// Binary Position P2 Output Data Array Number
                           
int16 TP3 = BRD32C;		// Binary Position P3 Output Board Address	
uint8 TP3B = BIT5;		// Binary Position P3 Output Bit Number
uint8 TP3D = 95;			// Binary Position P3 Output Data Array Number
                           
int16 TP4 = BRD32C;		// Binary Position P4 Output Board Address	
uint8 TP4B = BIT6;		// Binary Position P4 Output Bit Number
uint8 TP4D = 95;			// Binary Position P4 Output Data Array Number
                           
int16 TP5 = BRD32C;		// Binary Position P5 Output Board Address	
uint8 TP5B = BIT7;		// Binary Position P5 Output Bit Number
uint8 TP5D = 95;			// Binary Position P5 Output Data Array Number
                           
int16 TP6 = BRD32C;		// Binary Position P6 Output Board Address	
uint8 TP6B = BIT8;		// Binary Position P6 Output Bit Number
uint8 TP6D = 95;			// Binary Position P6 Output Data Array Number


//**********************************************************
//The following five I/O placed here to use a dummy location

int16 FW = BRD27C;       // Field Weakening Output Board Address
uint8 FWB = BIT1;        // Field Weakening Bit Number
uint8 FWD = 80;          // Field Weakening Data Array Number

//**********************************************************


// Address board 45
int16 FiDCB = BRD45B;	// Fire Door Close Button Board Address
uint8 FiDCBB = BIT5;      // Fire Door Close Button Bit Number
uint8 FiDCBD = 133;       // Fire Door Close Button Data Array Number

int16 FiDOB = BRD45B;	// Fire Door Open Button Board Address
uint8 FiDOBB = BIT6;      // Fire Door Open Button Bit Number
uint8 FiDOBD = 133;       // Fire Door Open Button Data Array Number

int16 FiFS2C = BRD45B;	// Fire Cancel Button Board Address
uint8 FiFS2CB = BIT7;     // Fire Cancel Button Bit Number
uint8 FiFS2CD = 133;      // Fire Cancel Button Data Array Number

int16 FiDCBR = BRD45B;	// Fire Rrear Door Close Button Board Address
uint8 FiDCBRB = BIT8;     // Fire Rrear Door Close Button Bit Number
uint8 FiDCBRD = 133;      // Fire Rrear Door Close Button Data Array Number

int16 FiDOBR = BRD45C;	// Fire Rrear Door Open Button Board Address
uint8 FiDOBRB = BIT1;     // Fire Rrear Door Open Button Bit Number
uint8 FiDOBRD = 134;      // Fire Rrear Door Open Button Data Array Number

int16 ICIU = BRD45C;		// In Car inspection up Input Board Address
uint8 ICIUB = BIT2;		// In Car inspection up Input Bit Number
uint8 ICIUD = 134;		// In Car inspection up Input Data Array Number
	  
int16 ICID_ = BRD45C;		// In Car inspection down Input Board Address
uint8 ICID_B = BIT3;		// In Car inspection down Input Bit Number
uint8 ICID_D = 134;		// In Car inspection down Input Data Array Number

int16 FiFL = BRD46C;	// Fire Control Fire Light Output Board Address
uint8 FiFLB = BIT6;     	// Fire Control Fire Light Output Bit Number
uint8 FiFLD = 137;      	// Fire Control Fire Light Output Data Array Number


// Address board 47
int16 PHONE = BRD47A;	// PHONE Input Board Address
uint8 PHONEB = BIT8;     // Phone input Bit Number
uint8 PHONED = 138;      // Phone Data Array Number



// Address board 48

int16 ATUL = BRD48B;      // Attendant Up Light Output Board Address
uint8 ATULB = BIT1;        // Attendant Up Light Bit Number
uint8 ATULD = 142;          // Attendant Up Light Data Array Number

int16 ATDL = BRD48B;      // Attendant Down Light Output Board Address
uint8 ATDLB = BIT2;        // Attendant Down Light Bit Number
uint8 ATDLD = 142;          // Attendant Down Light Data Array Number

//**********************************************************
// Radial PI Outputs

int16 RDDA = BRD51A;	// Car Available Output Board Address	
uint8 RDDAB = BIT1;		// Car Available Output Bit Number
uint8 RDDAD = 153;		// Car Available Output Data Array Number

int16 RUDA = BRD51A;		// Car Available Output Board Address	
uint8 RUDAB = BIT2;		// Car Available Output Bit Number
uint8 RUDAD = 153;		// Car Available Output Data Array Number

int16 RFSO = BRD51A;	// Car Available Output Board Address	
uint8 RFSOB = BIT3;		// Car Available Output Bit Number
uint8 RFSOD = 153;		// Car Available Output Data Array Number

int16 RPI1 = BRD51A;	// Car Available Output Board Address	
uint8 RPI1B = BIT4;		// Car Available Output Bit Number
uint8 RPI1D = 153;	   	// Car Available Output Data Array Number

int16 RPI2 = BRD51A;	// Car Available Output Board Address	
uint8 RPI2B = BIT5;		// Car Available Output Bit Number
uint8 RPI2D = 153;	   	// Car Available Output Data Array Number

int16 RPI3 = BRD51A;	// Car Available Output Board Address	
uint8 RPI3B = BIT6;		// Car Available Output Bit Number
uint8 RPI3D = 153;	   	// Car Available Output Data Array Number

int16 RPI4 = BRD51A;	// Car Available Output Board Address	
uint8 RPI4B = BIT7;		// Car Available Output Bit Number
uint8 RPI4D = 153;	   	// Car Available Output Data Array Number

int16 RPI5 = BRD51A;	// Car Available Output Board Address	
uint8 RPI5B = BIT8;		// Car Available Output Bit Number
uint8 RPI5D = 153;	   	// Car Available Output Data Array Number

int16 RPI6 = BRD51B;	// Car Available Output Board Address	
uint8 RPI6B = BIT1;		// Car Available Output Bit Number
uint8 RPI6D = 154;	   	// Car Available Output Data Array Number

int16 RPI7 = BRD51B;	// Car Available Output Board Address	
uint8 RPI7B = BIT2;		// Car Available Output Bit Number
uint8 RPI7D = 154;	   	// Car Available Output Data Array Number

int16 RPI8 = BRD51B;	// Car Available Output Board Address	
uint8 RPI8B = BIT3;		// Car Available Output Bit Number
uint8 RPI8D = 154;	   	// Car Available Output Data Array Number

int16 RPI9 = BRD51B;	// Car Available Output Board Address	
uint8 RPI9B = BIT4;		// Car Available Output Bit Number
uint8 RPI9D = 154;	   	// Car Available Output Data Array Number

int16 RPI10 = BRD51B;	// Car Available Output Board Address	
uint8 RPI10B = BIT5;	// Car Available Output Bit Number
uint8 RPI10D = 154;	   	// Car Available Output Data Array Number

int16 RPI11 = BRD51B;	// Car Available Output Board Address	
uint8 RPI11B = BIT6;	// Car Available Output Bit Number
uint8 RPI11D = 154;	   	// Car Available Output Data Array Number

int16 RPI12 = BRD51B;	// Car Available Output Board Address	
uint8 RPI12B = BIT7;	// Car Available Output Bit Number
uint8 RPI12D = 154;	   	// Car Available Output Data Array Number

int16 RPI13 = BRD51B;	// Car Available Output Board Address	
uint8 RPI13B = BIT8;	// Car Available Output Bit Number
uint8 RPI13D = 154;	   	// Car Available Output Data Array Number

int16 RPI14 = BRD51C;	// Car Available Output Board Address	
uint8 RPI14B = BIT1;	// Car Available Output Bit Number
uint8 RPI14D = 155;	   	// Car Available Output Data Array Number

int16 RPI15 = BRD51C;	// Car Available Output Board Address	
uint8 RPI15B = BIT2;	// Car Available Output Bit Number
uint8 RPI15D = 155;	   	// Car Available Output Data Array Number

int16 RPI16 = BRD51C;	// Car Available Output Board Address	
uint8 RPI16B = BIT3;	// Car Available Output Bit Number
uint8 RPI16D = 155;	   	// Car Available Output Data Array Number

int16 RPI17 = BRD51C;	// Car Available Output Board Address	
uint8 RPI17B = BIT4;	// Car Available Output Bit Number
uint8 RPI17D = 155;	   	// Car Available Output Data Array Number

int16 RPI18 = BRD51C;	// Car Available Output Board Address	
uint8 RPI18B = BIT5;	// Car Available Output Bit Number
uint8 RPI18D = 155;	   	// Car Available Output Data Array Number

int16 RPI19 = BRD51C;	// Car Available Output Board Address	
uint8 RPI19B = BIT6;	// Car Available Output Bit Number
uint8 RPI19D = 155;	   	// Car Available Output Data Array Number

int16 RPI20 = BRD51C;	// Car Available Output Board Address	
uint8 RPI20B = BIT7;	// Car Available Output Bit Number
uint8 RPI20D = 155;	   	// Car Available Output Data Array Number

int16 RPI21 = BRD51C;	// Car Available Output Board Address	
uint8 RPI21B = BIT8;	// Car Available Output Bit Number
uint8 RPI21D = 155;	   	// Car Available Output Data Array Number



extern const int16 cc[3][61] = { 
{			  // Used with serial car call
	0,                      // Not Used
	// TOC SERIAL CAR BOARD
	BRD23A,                  // Car Call #1 Input Board Address
	BRD23A,                  // Car Call #2 Input Board Address
	BRD23A,                  // Car Call #3 Input Board Address
	BRD23A,                  // Car Call #4 Input Board Address
	BRD23A,                  // Car Call #5 Input Board Address
	BRD23A,                  // Car Call #6 Input Board Address
	BRD23A,                  // Car Call #7 Input Board Address
	BRD23A,                  // Car Call #8 Input Board Address
	BRD23B,                  // Car Call #9 Input Board Address
	BRD23B,                  // Car Call #10 Input  Board Address
	BRD23B,                  // Car Call #11 Input  Board Address
	BRD23B,                  // Car Call #12 Input Board Address
	// ON 2nd SERIAL CAR BOARD
	BRD15A,                  // Car Call #13 Input Board Address
	BRD15A,                  // Car Call #14 Input Board Address
	BRD15A,  	             // Car Call #15 Input Board Address
	BRD15A,	              	 // Car Call #16 Input Board Address
	BRD15A,                  // Car Call #17 Input Board Address
	BRD15A,                  // Car Call #18 Input Board Address
	BRD15A,                  // Car Call #19 Input Board Address
	BRD15A,                  // Car Call #20 Input Board Address
	BRD15B,                  // Car Call #21 Input Board Address
	BRD15B,                  // Car Call #22 Input Board Address
	BRD15B,                  // Car Call #23 Input Board Address
	BRD15B,                  // Car Call #24 Input Board Address
	BRD15B,                  // Car Call #25 Input Board Address
	BRD15B,                  // Car Call #26 Input Board Address
	BRD15B,                  // Car Call #27 Input Board Address
	BRD15B,                  // Car Call #28 Input Board Address
	BRD15C,                  // Car Call #29 Input Board Address
	BRD15C,                  // Car Call #30 Input Board Address
	BRD15C,                  // Car Call #31 Input Board Address
	BRD15C,                  // Car Call #32 Input Board Address
	BRD15C,                  // Car Call #33 Input Board Address
	BRD15C,                  // Car Call #34 Input Board Address
	BRD15C,                  // Car Call #35 Input Board Address
	BRD15C,                  // Car Call #36 Input Board Address
	// ON 3rd SERIAL CAR BOARD
	BRD29A,                  // Car Call #37 Input Board Address
	BRD29A,                  // Car Call #38 Input Board Address
	BRD29A,  	             // Car Call #39 Input Board Address
	BRD29A,	              	 // Car Call #40 Input Board Address
	BRD29A,                  // Car Call #41 Input Board Address
	BRD29A,                  // Car Call #42 Input Board Address
	BRD29A,                  // Car Call #43 Input Board Address
	BRD29A,                  // Car Call #44 Input Board Address
	BRD29B,                  // Car Call #45 Input Board Address
	BRD29B,                  // Car Call #46 Input Board Address
	BRD29B,                  // Car Call #47 Input Board Address
	BRD29B,                  // Car Call #48 Input Board Address
	BRD29B,                  // Car Call #49 Input Board Address
	BRD29B,                  // Car Call #50 Input Board Address
	BRD29B,                  // Car Call #51 Input Board Address
	BRD29B,                  // Car Call #52 Input Board Address
	BRD29C,                  // Car Call #53 Input Board Address
	BRD29C,                  // Car Call #54 Input Board Address
	BRD29C,                  // Car Call #55 Input Board Address
	BRD29C,                  // Car Call #56 Input Board Address
	BRD29C,                  // Car Call #57 Input Board Address
	BRD29C,                  // Car Call #58 Input Board Address
	BRD29C,                  // Car Call #59 Input Board Address
	BRD29C                   // Car Call #60 Input Board Address
	},
{			  // Used with serial car call
	0,                      // Not Used
	// 0n 1st Serial Car board
	BRD13A,                  // Car Call #1 Input Board Address
	BRD13A,                  // Car Call #2 Input Board Address
	BRD13A,                  // Car Call #3 Input Board Address
	BRD13A,                  // Car Call #4 Input Board Address
	BRD13A,                  // Car Call #5 Input Board Address
	BRD13A,                  // Car Call #6 Input Board Address
	BRD13A,                  // Car Call #7 Input Board Address
	BRD13A,                  // Car Call #8 Input Board Address
	BRD13B,                  // Car Call #9 Input Board Address
	BRD13B,                  // Car Call #10 Input  Board Address
	BRD13B,                  // Car Call #11 Input  Board Address
	BRD13B,                  // Car Call #12 Input Board Address
	// ON 2nd SERIAL CAR BOARD
	BRD15A,                  // Car Call #13 Input Board Address
	BRD15A,                  // Car Call #14 Input Board Address
	BRD15A,  	             // Car Call #15 Input Board Address
	BRD15A,	              	 // Car Call #16 Input Board Address
	BRD15A,                  // Car Call #17 Input Board Address
	BRD15A,                  // Car Call #18 Input Board Address
	BRD15A,                  // Car Call #19 Input Board Address
	BRD15A,                  // Car Call #20 Input Board Address
	BRD15B,                  // Car Call #21 Input Board Address
	BRD15B,                  // Car Call #22 Input Board Address
	BRD15B,                  // Car Call #23 Input Board Address
	BRD15B,                  // Car Call #24 Input Board Address
	BRD15B,                  // Car Call #25 Input Board Address
	BRD15B,                  // Car Call #26 Input Board Address
	BRD15B,                  // Car Call #27 Input Board Address
	BRD15B,                  // Car Call #28 Input Board Address
	BRD15C,                  // Car Call #29 Input Board Address
	BRD15C,                  // Car Call #30 Input Board Address
	BRD15C,                  // Car Call #31 Input Board Address
	BRD15C,                  // Car Call #32 Input Board Address
	BRD15C,                  // Car Call #33 Input Board Address
	BRD15C,                  // Car Call #34 Input Board Address
	BRD15C,                  // Car Call #35 Input Board Address
	BRD15C,                  // Car Call #36 Input Board Address
	// ON 3rd SERIAL CAR BOARD
	BRD29A,                  // Car Call #37 Input Board Address
	BRD29A,                  // Car Call #38 Input Board Address
	BRD29A,  	             // Car Call #39 Input Board Address
	BRD29A,	              	 // Car Call #40 Input Board Address
	BRD29A,                  // Car Call #41 Input Board Address
	BRD29A,                  // Car Call #42 Input Board Address
	BRD29A,                  // Car Call #43 Input Board Address
	BRD29A,                  // Car Call #44 Input Board Address
	BRD29B,                  // Car Call #45 Input Board Address
	BRD29B,                  // Car Call #46 Input Board Address
	BRD29B,                  // Car Call #47 Input Board Address
	BRD29B,                  // Car Call #48 Input Board Address
	BRD29B,                  // Car Call #49 Input Board Address
	BRD29B,                  // Car Call #50 Input Board Address
	BRD29B,                  // Car Call #51 Input Board Address
	BRD29B,                  // Car Call #52 Input Board Address
	BRD29C,                  // Car Call #53 Input Board Address
	BRD29C,                  // Car Call #54 Input Board Address
	BRD29C,                  // Car Call #55 Input Board Address
	BRD29C,                  // Car Call #56 Input Board Address
	BRD29C,                  // Car Call #57 Input Board Address
	BRD29C,                  // Car Call #58 Input Board Address
	BRD29C,                  // Car Call #59 Input Board Address
	BRD29C                   // Car Call #60 Input Board Address
	}, 
{			  // Used with RGB Car call Boards
	0,                      // Not Used
	// 0n 1st Serial Car board
	BRD43A,                  // Car Call #1 Input Board Address
	BRD43A,                  // Car Call #2 Input Board Address
	BRD43A,                  // Car Call #3 Input Board Address
	BRD43A,                  // Car Call #4 Input Board Address
	BRD43A,                  // Car Call #5 Input Board Address
	BRD43A,                  // Car Call #6 Input Board Address
	BRD43B,                  // Car Call #7 Input Board Address
	BRD43B,                  // Car Call #8 Input Board Address
	BRD43B,                  // Car Call #9 Input Board Address
	BRD43B,                  // Car Call #10 Input  Board Address
	BRD43C,                  // Car Call #11 Input  Board Address
	BRD43C,                  // Car Call #12 Input Board Address
	// ON 2nd SERIAL CAR BOARD
	BRD15A,                  // Car Call #13 Input Board Address
	BRD15A,                  // Car Call #14 Input Board Address
	BRD15A,  	             // Car Call #15 Input Board Address
	BRD15A,	              	 // Car Call #16 Input Board Address
	BRD15A,                  // Car Call #17 Input Board Address
	BRD15A,                  // Car Call #18 Input Board Address
	BRD15A,                  // Car Call #19 Input Board Address
	BRD15A,                  // Car Call #20 Input Board Address
	BRD15B,                  // Car Call #21 Input Board Address
	BRD15B,                  // Car Call #22 Input Board Address
	BRD15B,                  // Car Call #23 Input Board Address
	BRD15B,                  // Car Call #24 Input Board Address
	BRD15B,                  // Car Call #25 Input Board Address
	BRD15B,                  // Car Call #26 Input Board Address
	BRD15B,                  // Car Call #27 Input Board Address
	BRD15B,                  // Car Call #28 Input Board Address
	BRD15C,                  // Car Call #29 Input Board Address
	BRD15C,                  // Car Call #30 Input Board Address
	BRD15C,                  // Car Call #31 Input Board Address
	BRD15C,                  // Car Call #32 Input Board Address
	BRD15C,                  // Car Call #33 Input Board Address
	BRD15C,                  // Car Call #34 Input Board Address
	BRD15C,                  // Car Call #35 Input Board Address
	BRD15C,                  // Car Call #36 Input Board Address
	// ON 3rd SERIAL CAR BOARD
	BRD29A,                  // Car Call #37 Input Board Address
	BRD29A,                  // Car Call #38 Input Board Address
	BRD29A,  	             // Car Call #39 Input Board Address
	BRD29A,	              	 // Car Call #40 Input Board Address
	BRD29A,                  // Car Call #41 Input Board Address
	BRD29A,                  // Car Call #42 Input Board Address
	BRD29A,                  // Car Call #43 Input Board Address
	BRD29A,                  // Car Call #44 Input Board Address
	BRD29B,                  // Car Call #45 Input Board Address
	BRD29B,                  // Car Call #46 Input Board Address
	BRD29B,                  // Car Call #47 Input Board Address
	BRD29B,                  // Car Call #48 Input Board Address
	BRD29B,                  // Car Call #49 Input Board Address
	BRD29B,                  // Car Call #50 Input Board Address
	BRD29B,                  // Car Call #51 Input Board Address
	BRD29B,                  // Car Call #52 Input Board Address
	BRD29C,                  // Car Call #53 Input Board Address
	BRD29C,                  // Car Call #54 Input Board Address
	BRD29C,                  // Car Call #55 Input Board Address
	BRD29C,                  // Car Call #56 Input Board Address
	BRD29C,                  // Car Call #57 Input Board Address
	BRD29C,                  // Car Call #58 Input Board Address
	BRD29C,                  // Car Call #59 Input Board Address
	BRD29C                   // Car Call #60 Input Board Address
	} 
};
extern const unsigned char ccb[3][61] = {
{			  // Used with serial car call
	0,                      // Not Used
	// TOC serial car board
	BIT1,                   // Car Call #1 Input Bit Number
	BIT2,                   // Car Call #2 Input Bit Number
	BIT3,                   // Car Call #3 Input Bit Number
	BIT4,                   // Car Call #4 Input Bit Number
	BIT5,                   // Car Call #5 Input Bit Number
	BIT6,                   // Car Call #6 Input Bit Number
	BIT7,                   // Car Call #7 Input Bit Number
	BIT8,                   // Car Call #8 Input Bit Number
	BIT1,                   // Car Call #9 Input Bit Number
	BIT2,                   // Car Call #10 Input Bit Number
	BIT3,                   // Car Call #11 Input Bit Number
	BIT4,                   // Car Call #12 Input Bit Number
	// ON 2nd serial car board
	BIT1,                   // Car Call #13 Input Bit Number
	BIT2,                   // Car Call #14 Input Bit Number
	BIT3,                   // Car Call #15 Input Bit Number
	BIT4,                   // Car Call #16 Input Bit Number
	BIT5,                   // Car Call #17 Input Bit Number
	BIT6,                   // Car Call #18 Input Bit Number
	BIT7,                   // Car Call #19 Input Bit Number
	BIT8,                   // Car Call #20 Input Bit Number
	BIT1,                   // Car Call #21 Input Bit Number
	BIT2,                   // Car Call #22 Input Bit Number
	BIT3,                   // Car Call #23 Input Bit Number
	BIT4,                   // Car Call #24 Input Bit Number
	BIT5,                   // Car Call #25 Input Bit Number
	BIT6,                   // Car Call #26 Input Bit Number
	BIT7,                   // Car Call #27 Input Bit Number
	BIT8,                   // Car Call #28 Input Bit Number
	BIT1,                   // Car Call #29 Input Bit Number
	BIT2,                   // Car Call #30 Input Bit Number
	BIT3,                   // Car Call #31 Input Bit Number
	BIT4,                   // Car Call #32 Input Bit Number
	BIT5,                   // Car Call #33 Input Bit Number
	BIT6,                   // Car Call #34 Input Bit Number
	BIT7,                   // Car Call #35 Input Bit Number
	BIT8,                   // Car Call #36 Input Bit Number
	// ON 3rd serial car board
	BIT1,                   // Car Call #37 Input Bit Number
	BIT2,                   // Car Call #38 Input Bit Number
	BIT3,                   // Car Call #39 Input Bit Number
	BIT4,                   // Car Call #40 Input Bit Number
	BIT5,                   // Car Call #41 Input Bit Number
	BIT6,                   // Car Call #42 Input Bit Number
	BIT7,                   // Car Call #43 Input Bit Number
	BIT8,                   // Car Call #44 Input Bit Number
	BIT1,                   // Car Call #45 Input Bit Number
	BIT2,                   // Car Call #46 Input Bit Number
	BIT3,                   // Car Call #47 Input Bit Number
	BIT4,                   // Car Call #48 Input Bit Number
	BIT5,                   // Car Call #49 Input Bit Number
	BIT6,                   // Car Call #50 Input Bit Number
	BIT7,                   // Car Call #51 Input Bit Number
	BIT8,                   // Car Call #52 Input Bit Number
	BIT1,                   // Car Call #53 Input Bit Number
	BIT2,                   // Car Call #54 Input Bit Number
	BIT3,                   // Car Call #55 Input Bit Number
	BIT4,                   // Car Call #56 Input Bit Number
	BIT5,                   // Car Call #57 Input Bit Number
	BIT6,                   // Car Call #58 Input Bit Number
	BIT7,                   // Car Call #59 Input Bit Number
	BIT8                    // Car Call #60 Input Bit Number
	},
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st serial car board
	BIT1,                   // Car Call #1 Input Bit Number
	BIT2,                   // Car Call #2 Input Bit Number
	BIT3,                   // Car Call #3 Input Bit Number
	BIT4,                   // Car Call #4 Input Bit Number
	BIT5,                   // Car Call #5 Input Bit Number
	BIT6,                   // Car Call #6 Input Bit Number
	BIT7,                   // Car Call #7 Input Bit Number
	BIT8,                   // Car Call #8 Input Bit Number
	BIT1,                   // Car Call #9 Input Bit Number
	BIT2,                   // Car Call #10 Input Bit Number
	BIT3,                   // Car Call #11 Input Bit Number
	BIT4,                   // Car Call #12 Input Bit Number
	// ON 2nd serial car board
	BIT1,                   // Car Call #13 Input Bit Number
	BIT2,                   // Car Call #14 Input Bit Number
	BIT3,                   // Car Call #15 Input Bit Number
	BIT4,                   // Car Call #16 Input Bit Number
	BIT5,                   // Car Call #17 Input Bit Number
	BIT6,                   // Car Call #18 Input Bit Number
	BIT7,                   // Car Call #19 Input Bit Number
	BIT8,                   // Car Call #20 Input Bit Number
	BIT1,                   // Car Call #21 Input Bit Number
	BIT2,                   // Car Call #22 Input Bit Number
	BIT3,                   // Car Call #23 Input Bit Number
	BIT4,                   // Car Call #24 Input Bit Number
	BIT5,                   // Car Call #25 Input Bit Number
	BIT6,                   // Car Call #26 Input Bit Number
	BIT7,                   // Car Call #27 Input Bit Number
	BIT8,                   // Car Call #28 Input Bit Number
	BIT1,                   // Car Call #29 Input Bit Number
	BIT2,                   // Car Call #30 Input Bit Number
	BIT3,                   // Car Call #31 Input Bit Number
	BIT4,                   // Car Call #32 Input Bit Number
	BIT5,                   // Car Call #33 Input Bit Number
	BIT6,                   // Car Call #34 Input Bit Number
	BIT7,                   // Car Call #35 Input Bit Number
	BIT8,                   // Car Call #36 Input Bit Number
	// ON 3rd serial car board
	BIT1,                   // Car Call #37 Input Bit Number
	BIT2,                   // Car Call #38 Input Bit Number
	BIT3,                   // Car Call #39 Input Bit Number
	BIT4,                   // Car Call #40 Input Bit Number
	BIT5,                   // Car Call #41 Input Bit Number
	BIT6,                   // Car Call #42 Input Bit Number
	BIT7,                   // Car Call #43 Input Bit Number
	BIT8,                   // Car Call #44 Input Bit Number
	BIT1,                   // Car Call #45 Input Bit Number
	BIT2,                   // Car Call #46 Input Bit Number
	BIT3,                   // Car Call #47 Input Bit Number
	BIT4,                   // Car Call #48 Input Bit Number
	BIT5,                   // Car Call #49 Input Bit Number
	BIT6,                   // Car Call #50 Input Bit Number
	BIT7,                   // Car Call #51 Input Bit Number
	BIT8,                   // Car Call #52 Input Bit Number
	BIT1,                   // Car Call #53 Input Bit Number
	BIT2,                   // Car Call #54 Input Bit Number
	BIT3,                   // Car Call #55 Input Bit Number
	BIT4,                   // Car Call #56 Input Bit Number
	BIT5,                   // Car Call #57 Input Bit Number
	BIT6,                   // Car Call #58 Input Bit Number
	BIT7,                   // Car Call #59 Input Bit Number
	BIT8                    // Car Call #60 Input Bit Number
	}, 
{			  // Used with RGB car call boards
	0,                      // Not Used
	// ON 1st serial car board
	BIT1,                   // Car Call #1 Input Bit Number
	BIT2,                   // Car Call #2 Input Bit Number
	BIT3,                   // Car Call #3 Input Bit Number
	BIT4,                   // Car Call #4 Input Bit Number
	BIT5,                   // Car Call #5 Input Bit Number
	BIT6,                   // Car Call #6 Input Bit Number
	BIT5,                   // Car Call #7 Input Bit Number
	BIT6,                   // Car Call #8 Input Bit Number
	BIT7,                   // Car Call #9 Input Bit Number
	BIT8,                   // Car Call #10 Input Bit Number
	BIT1,                   // Car Call #11 Input Bit Number
	BIT2,                   // Car Call #12 Input Bit Number
	// ON 2nd serial car board
	BIT1,                   // Car Call #13 Input Bit Number
	BIT2,                   // Car Call #14 Input Bit Number
	BIT3,                   // Car Call #15 Input Bit Number
	BIT4,                   // Car Call #16 Input Bit Number
	BIT5,                   // Car Call #17 Input Bit Number
	BIT6,                   // Car Call #18 Input Bit Number
	BIT7,                   // Car Call #19 Input Bit Number
	BIT8,                   // Car Call #20 Input Bit Number
	BIT1,                   // Car Call #21 Input Bit Number
	BIT2,                   // Car Call #22 Input Bit Number
	BIT3,                   // Car Call #23 Input Bit Number
	BIT4,                   // Car Call #24 Input Bit Number
	BIT5,                   // Car Call #25 Input Bit Number
	BIT6,                   // Car Call #26 Input Bit Number
	BIT7,                   // Car Call #27 Input Bit Number
	BIT8,                   // Car Call #28 Input Bit Number
	BIT1,                   // Car Call #29 Input Bit Number
	BIT2,                   // Car Call #30 Input Bit Number
	BIT3,                   // Car Call #31 Input Bit Number
	BIT4,                   // Car Call #32 Input Bit Number
	BIT5,                   // Car Call #33 Input Bit Number
	BIT6,                   // Car Call #34 Input Bit Number
	BIT7,                   // Car Call #35 Input Bit Number
	BIT8,                   // Car Call #36 Input Bit Number
	// ON 3rd serial car board
	BIT1,                   // Car Call #37 Input Bit Number
	BIT2,                   // Car Call #38 Input Bit Number
	BIT3,                   // Car Call #39 Input Bit Number
	BIT4,                   // Car Call #40 Input Bit Number
	BIT5,                   // Car Call #41 Input Bit Number
	BIT6,                   // Car Call #42 Input Bit Number
	BIT7,                   // Car Call #43 Input Bit Number
	BIT8,                   // Car Call #44 Input Bit Number
	BIT1,                   // Car Call #45 Input Bit Number
	BIT2,                   // Car Call #46 Input Bit Number
	BIT3,                   // Car Call #47 Input Bit Number
	BIT4,                   // Car Call #48 Input Bit Number
	BIT5,                   // Car Call #49 Input Bit Number
	BIT6,                   // Car Call #50 Input Bit Number
	BIT7,                   // Car Call #51 Input Bit Number
	BIT8,                   // Car Call #52 Input Bit Number
	BIT1,                   // Car Call #53 Input Bit Number
	BIT2,                   // Car Call #54 Input Bit Number
	BIT3,                   // Car Call #55 Input Bit Number
	BIT4,                   // Car Call #56 Input Bit Number
	BIT5,                   // Car Call #57 Input Bit Number
	BIT6,                   // Car Call #58 Input Bit Number
	BIT7,                   // Car Call #59 Input Bit Number
	BIT8                    // Car Call #60 Input Bit Number
	} 
};

extern const unsigned char ccd[3][61] = {
{			  // Used with serial car call
	0,                      // Not Used
	// ON TOC serial car board
	66,                      // Car Call #1 Input Array Number
	66,                      // Car Call #2 Input Array Number
	66,                      // Car Call #3 Input Array Number
	66,                      // Car Call #4 Input Array Number
	66,                      // Car Call #5 Input Array Number
	66,                      // Car Call #6 Input Array Number
	66,                      // Car Call #7 Input Array Number
	66,                      // Car Call #8 Input Array Number
	67,                      // Car Call #9 Input Array Number
	67,                      // Car Call #10 Input Array Number
	67,                      // Car Call #11 Input Array Number
	67,                      // Car Call #12 Input Array Number

	// ON 2nd serial car board
	42,                     // Car Call #13 Input Array Number
	42,                     // Car Call #14 Input Array Number
	42,                     // Car Call #15 Input Array Number
	42,                     // Car Call #16 Input Array Number
	42,                     // Car Call #17 Input Array Number
	42,                     // Car Call #18 Input Array Number
	42,                     // Car Call #19 Input Array Number
	42,                     // Car Call #20 Input Array Number
	43,                     // Car Call #21 Input Array Number
	43,                     // Car Call #22 Input Array Number
	43,                     // Car Call #23 Input Array Number
	43,                     // Car Call #24 Input Array Number
	43,                     // Car Call #25 Input Array Number
	43,                     // Car Call #26 Input Array Number
	43,                     // Car Call #27 Input Array Number
	43,                     // Car Call #28 Input Array Number
	44,                     // Car Call #29 Input Array Number
	44,                     // Car Call #30 Input Array Number
	44,                     // Car Call #31 Input Array Number
	44,                     // Car Call #32 Input Array Number
	44,                     // Car Call #33 Input Array Number
	44,                     // Car Call #34 Input Array Number
	44,                     // Car Call #35 Input Array Number
	44,                     // Car Call #36 Input Array Number
	// ON 3rd serial car board
	84,                     // Car Call #37 Input Array Number
	84,                     // Car Call #38 Input Array Number
	84,                     // Car Call #39 Input Array Number
	84,                     // Car Call #40 Input Array Number
	84,                     // Car Call #41 Input Array Number
	84,                     // Car Call #42 Input Array Number
	84,                     // Car Call #43 Input Array Number
	84,                     // Car Call #44 Input Array Number
	85,                     // Car Call #45 Input Array Number
	85,                     // Car Call #46 Input Array Number
	85,                     // Car Call #47 Input Array Number
	85,                     // Car Call #48 Input Array Number
	85,                     // Car Call #49 Input Array Number
	85,                     // Car Call #50 Input Array Number
	85,                     // Car Call #51 Input Array Number
	85,                     // Car Call #52 Input Array Number
	86,                     // Car Call #53 Input Array Number
	86,                     // Car Call #54 Input Array Number
	86,                     // Car Call #55 Input Array Number
	86,                     // Car Call #56 Input Array Number
	86,                     // Car Call #57 Input Array Number
	86,                     // Car Call #58 Input Array Number
    86,                     // Car Call #59 Input Array Number
	86                      // Car Call #60 Input Array Number
	},
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st serial car board
	36,                      // Car Call #1 Input Array Number
	36,                      // Car Call #2 Input Array Number
	36,                      // Car Call #3 Input Array Number
	36,                      // Car Call #4 Input Array Number
	36,                      // Car Call #5 Input Array Number
	36,                      // Car Call #6 Input Array Number
	36,                      // Car Call #7 Input Array Number
	36,                      // Car Call #8 Input Array Number
	37,                      // Car Call #9 Input Array Number
	37,                      // Car Call #10 Input Array Number
	37,                      // Car Call #11 Input Array Number
	37,                      // Car Call #12 Input Array Number

	// ON 2nd serial car board
	42,                     // Car Call #13 Input Array Number
	42,                     // Car Call #14 Input Array Number
	42,                     // Car Call #15 Input Array Number
	42,                     // Car Call #16 Input Array Number
	42,                     // Car Call #17 Input Array Number
	42,                     // Car Call #18 Input Array Number
	42,                     // Car Call #19 Input Array Number
	42,                     // Car Call #20 Input Array Number
	43,                     // Car Call #21 Input Array Number
	43,                     // Car Call #22 Input Array Number
	43,                     // Car Call #23 Input Array Number
	43,                     // Car Call #24 Input Array Number
	43,                     // Car Call #25 Input Array Number
	43,                     // Car Call #26 Input Array Number
	43,                     // Car Call #27 Input Array Number
	43,                     // Car Call #28 Input Array Number
	44,                     // Car Call #29 Input Array Number
	44,                     // Car Call #30 Input Array Number
	44,                     // Car Call #31 Input Array Number
	44,                     // Car Call #32 Input Array Number
	44,                     // Car Call #33 Input Array Number
	44,                     // Car Call #34 Input Array Number
	44,                     // Car Call #35 Input Array Number
	44,                     // Car Call #36 Input Array Number
	// ON 3rd serial car board
	84,                     // Car Call #37 Input Array Number
	84,                     // Car Call #38 Input Array Number
	84,                     // Car Call #39 Input Array Number
	84,                     // Car Call #40 Input Array Number
	84,                     // Car Call #41 Input Array Number
	84,                     // Car Call #42 Input Array Number
	84,                     // Car Call #43 Input Array Number
	84,                     // Car Call #44 Input Array Number
	85,                     // Car Call #45 Input Array Number
	85,                     // Car Call #46 Input Array Number
	85,                     // Car Call #47 Input Array Number
	85,                     // Car Call #48 Input Array Number
	85,                     // Car Call #49 Input Array Number
	85,                     // Car Call #50 Input Array Number
	85,                     // Car Call #51 Input Array Number
	85,                     // Car Call #52 Input Array Number
	86,                     // Car Call #53 Input Array Number
	86,                     // Car Call #54 Input Array Number
	86,                     // Car Call #55 Input Array Number
	86,                     // Car Call #56 Input Array Number
	86,                     // Car Call #57 Input Array Number
	86,                     // Car Call #58 Input Array Number
    86,                     // Car Call #59 Input Array Number
	86                      // Car Call #60 Input Array Number
	}, 
{			  // Used with RGB car call boards
	0,                      // Not Used
	// ON 1st serial car board
	126,                      // Car Call #1 Input Array Number
	126,                      // Car Call #2 Input Array Number
	126,                      // Car Call #3 Input Array Number
	126,                      // Car Call #4 Input Array Number
	126,                      // Car Call #5 Input Array Number
	126,                      // Car Call #6 Input Array Number
	127,                      // Car Call #7 Input Array Number
	127,                      // Car Call #8 Input Array Number
	127,                      // Car Call #9 Input Array Number
	127,                      // Car Call #10 Input Array Number
	128,                      // Car Call #11 Input Array Number
	128,                      // Car Call #12 Input Array Number

	// ON 2nd serial car board
	42,                     // Car Call #13 Input Array Number
	42,                     // Car Call #14 Input Array Number
	42,                     // Car Call #15 Input Array Number
	42,                     // Car Call #16 Input Array Number
	42,                     // Car Call #17 Input Array Number
	42,                     // Car Call #18 Input Array Number
	42,                     // Car Call #19 Input Array Number
	42,                     // Car Call #20 Input Array Number
	43,                     // Car Call #21 Input Array Number
	43,                     // Car Call #22 Input Array Number
	43,                     // Car Call #23 Input Array Number
	43,                     // Car Call #24 Input Array Number
	43,                     // Car Call #25 Input Array Number
	43,                     // Car Call #26 Input Array Number
	43,                     // Car Call #27 Input Array Number
	43,                     // Car Call #28 Input Array Number
	44,                     // Car Call #29 Input Array Number
	44,                     // Car Call #30 Input Array Number
	44,                     // Car Call #31 Input Array Number
	44,                     // Car Call #32 Input Array Number
	44,                     // Car Call #33 Input Array Number
	44,                     // Car Call #34 Input Array Number
	44,                     // Car Call #35 Input Array Number
	44,                     // Car Call #36 Input Array Number
	// ON 3rd serial car board
	84,                     // Car Call #37 Input Array Number
	84,                     // Car Call #38 Input Array Number
	84,                     // Car Call #39 Input Array Number
	84,                     // Car Call #40 Input Array Number
	84,                     // Car Call #41 Input Array Number
	84,                     // Car Call #42 Input Array Number
	84,                     // Car Call #43 Input Array Number
	84,                     // Car Call #44 Input Array Number
	85,                     // Car Call #45 Input Array Number
	85,                     // Car Call #46 Input Array Number
	85,                     // Car Call #47 Input Array Number
	85,                     // Car Call #48 Input Array Number
	85,                     // Car Call #49 Input Array Number
	85,                     // Car Call #50 Input Array Number
	85,                     // Car Call #51 Input Array Number
	85,                     // Car Call #52 Input Array Number
	86,                     // Car Call #53 Input Array Number
	86,                     // Car Call #54 Input Array Number
	86,                     // Car Call #55 Input Array Number
	86,                     // Car Call #56 Input Array Number
	86,                     // Car Call #57 Input Array Number
	86,                     // Car Call #58 Input Array Number
    86,                     // Car Call #59 Input Array Number
	86                      // Car Call #60 Input Array Number
	} 
};

extern const int16 ccl[3][61] = {
{			  // Used with serial car call
	0,                      // Not Used
	// ON TOC Serial Car Board
	BRD24A,                  // Car Call Light #1 Output Board Address
	BRD24A,                  // Car Call Light #2 Output Board Address
	BRD24A,                  // Car Call Light #3 Output Board Address
	BRD24A,                  // Car Call Light #4 Output Board Address
	BRD24A,                  // Car Call Light #5 Output Board Address
	BRD24A,                  // Car Call Light #6 Output Board Address
	BRD24A,                  // Car Call Light #7 Output Board Address
	BRD24A,                  // Car Call Light #8 Output Board Address
	BRD24B,                  // Car Call Light #9 Output Board Address
	BRD24B,                  // Car Call Light #10 Output Board Address
	BRD24B,                  // Car Call Light #11 Output Board Address
	BRD24B,                  // Car Call Light #12 Output Board Address

	// ON 2nd Serial Car Board
	BRD16A,                 // Car Call Light #13 Output Board Address
	BRD16A,                 // Car Call Light #14 Output Board Address
	BRD16A,                 // Car Call Light #15 Output Board Address
	BRD16A,                 // Car Call Light #16 Output Board Address
	BRD16A,                 // Car Call Light #17 Output Board Address
	BRD16A,                 // Car Call Light #18 Output Board Address
	BRD16A,                 // Car Call Light #19 Output Board Address
	BRD16A,                 // Car Call Light #20 Output Board Address
	BRD16B,                 // Car Call Light #21 Output Board Address
	BRD16B,                 // Car Call Light #22 Output Board Address
	BRD16B,                 // Car Call Light #23 Output Board Address
	BRD16B,                 // Car Call Light #24 Output Board Address
	BRD16B,                 // Car Call Light #25 Output Board Address
	BRD16B,                 // Car Call Light #26 Output Board Address
	BRD16B,                 // Car Call Light #27 Output Board Address
	BRD16B,                 // Car Call Light #28 Output Board Address
	BRD16C,                 // Car Call Light #29 Output Board Address
	BRD16C,                 // Car Call Light #30 Output Board Address
	BRD16C,                 // Car Call Light #31 Output Board Address
	BRD16C,                 // Car Call Light #32 Output Board Address
	BRD16C,                 // Car Call Light #33 Output Board Address
	BRD16C,                 // Car Call Light #34 Output Board Address
	BRD16C,                 // Car Call Light #35 Output Board Address
	BRD16C,                 // Car Call Light #36 Output Board Address
	// ON 3rd Serial Car Board
	BRD30A,                 // Car Call Light #37 Output Board Address
	BRD30A,                 // Car Call Light #38 Output Board Address
	BRD30A,                 // Car Call Light #39 Output Board Address
	BRD30A,                 // Car Call Light #40 Output Board Address
	BRD30A,                 // Car Call Light #41 Output Board Address
	BRD30A,                 // Car Call Light #42 Output Board Address
	BRD30A,                 // Car Call Light #43 Output Board Address
	BRD30A,                 // Car Call Light #44 Output Board Address
	BRD30B,                 // Car Call Light #45 Output Board Address
	BRD30B,                 // Car Call Light #46 Output Board Address
	BRD30B,                 // Car Call Light #47 Output Board Address
	BRD30B,                 // Car Call Light #48 Output Board Address
	BRD30B,                 // Car Call Light #49 Output Board Address
	BRD30B,                 // Car Call Light #50 Output Board Address
	BRD30B,                 // Car Call Light #51 Output Board Address
	BRD30B,                 // Car Call Light #52 Output Board Address
	BRD30C,                 // Car Call Light #53 Output Board Address
	BRD30C,                 // Car Call Light #54 Output Board Address
	BRD30C,                 // Car Call Light #55 Output Board Address
	BRD30C,                 // Car Call Light #56 Output Board Address
	BRD30C,                 // Car Call Light #57 Output Board Address
	BRD30C,                 // Car Call Light #58 Output Board Address
	BRD30C,                 // Car Call Light #59 Output Board Address
	BRD30C                  // Car Call Light #60 Output Board Address
	},
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD14A,                  // Car Call Light #1 Output Board Address
	BRD14A,                  // Car Call Light #2 Output Board Address
	BRD14A,                  // Car Call Light #3 Output Board Address
	BRD14A,                  // Car Call Light #4 Output Board Address
	BRD14A,                  // Car Call Light #5 Output Board Address
	BRD14A,                  // Car Call Light #6 Output Board Address
	BRD14A,                  // Car Call Light #7 Output Board Address
	BRD14A,                  // Car Call Light #8 Output Board Address
	BRD14B,                  // Car Call Light #9 Output Board Address
	BRD14B,                  // Car Call Light #10 Output Board Address
	BRD14B,                  // Car Call Light #11 Output Board Address
	BRD14B,                  // Car Call Light #12 Output Board Address

	// ON 2nd Serial Car Board
	BRD16A,                 // Car Call Light #13 Output Board Address
	BRD16A,                 // Car Call Light #14 Output Board Address
	BRD16A,                 // Car Call Light #15 Output Board Address
	BRD16A,                 // Car Call Light #16 Output Board Address
	BRD16A,                 // Car Call Light #17 Output Board Address
	BRD16A,                 // Car Call Light #18 Output Board Address
	BRD16A,                 // Car Call Light #19 Output Board Address
	BRD16A,                 // Car Call Light #20 Output Board Address
	BRD16B,                 // Car Call Light #21 Output Board Address
	BRD16B,                 // Car Call Light #22 Output Board Address
	BRD16B,                 // Car Call Light #23 Output Board Address
	BRD16B,                 // Car Call Light #24 Output Board Address
	BRD16B,                 // Car Call Light #25 Output Board Address
	BRD16B,                 // Car Call Light #26 Output Board Address
	BRD16B,                 // Car Call Light #27 Output Board Address
	BRD16B,                 // Car Call Light #28 Output Board Address
	BRD16C,                 // Car Call Light #29 Output Board Address
	BRD16C,                 // Car Call Light #30 Output Board Address
	BRD16C,                 // Car Call Light #31 Output Board Address
	BRD16C,                 // Car Call Light #32 Output Board Address
	BRD16C,                 // Car Call Light #33 Output Board Address
	BRD16C,                 // Car Call Light #34 Output Board Address
	BRD16C,                 // Car Call Light #35 Output Board Address
	BRD16C,                 // Car Call Light #36 Output Board Address
	// ON 3rd Serial Car Board
	BRD30A,                 // Car Call Light #37 Output Board Address
	BRD30A,                 // Car Call Light #38 Output Board Address
	BRD30A,                 // Car Call Light #39 Output Board Address
	BRD30A,                 // Car Call Light #40 Output Board Address
	BRD30A,                 // Car Call Light #41 Output Board Address
	BRD30A,                 // Car Call Light #42 Output Board Address
	BRD30A,                 // Car Call Light #43 Output Board Address
	BRD30A,                 // Car Call Light #44 Output Board Address
	BRD30B,                 // Car Call Light #45 Output Board Address
	BRD30B,                 // Car Call Light #46 Output Board Address
	BRD30B,                 // Car Call Light #47 Output Board Address
	BRD30B,                 // Car Call Light #48 Output Board Address
	BRD30B,                 // Car Call Light #49 Output Board Address
	BRD30B,                 // Car Call Light #50 Output Board Address
	BRD30B,                 // Car Call Light #51 Output Board Address
	BRD30B,                 // Car Call Light #52 Output Board Address
	BRD30C,                 // Car Call Light #53 Output Board Address
	BRD30C,                 // Car Call Light #54 Output Board Address
	BRD30C,                 // Car Call Light #55 Output Board Address
	BRD30C,                 // Car Call Light #56 Output Board Address
	BRD30C,                 // Car Call Light #57 Output Board Address
	BRD30C,                 // Car Call Light #58 Output Board Address
	BRD30C,                 // Car Call Light #59 Output Board Address
	BRD30C                  // Car Call Light #60 Output Board Address
	}, 
{			  // Used with RBG car call	boards
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD44A,                  // Car Call Light #1 Output Board Address
	BRD44A,                  // Car Call Light #2 Output Board Address
	BRD44A,                  // Car Call Light #3 Output Board Address
	BRD44A,                  // Car Call Light #4 Output Board Address
	BRD44A,                  // Car Call Light #5 Output Board Address
	BRD44A,                  // Car Call Light #6 Output Board Address
	BRD44B,                  // Car Call Light #7 Output Board Address
	BRD44B,                  // Car Call Light #8 Output Board Address
	BRD44B,                  // Car Call Light #9 Output Board Address
	BRD44B,                  // Car Call Light #10 Output Board Address
	BRD44C,                  // Car Call Light #11 Output Board Address
	BRD44C,                  // Car Call Light #12 Output Board Address

	// ON 2nd Serial Car Board
	BRD16A,                 // Car Call Light #13 Output Board Address
	BRD16A,                 // Car Call Light #14 Output Board Address
	BRD16A,                 // Car Call Light #15 Output Board Address
	BRD16A,                 // Car Call Light #16 Output Board Address
	BRD16A,                 // Car Call Light #17 Output Board Address
	BRD16A,                 // Car Call Light #18 Output Board Address
	BRD16A,                 // Car Call Light #19 Output Board Address
	BRD16A,                 // Car Call Light #20 Output Board Address
	BRD16B,                 // Car Call Light #21 Output Board Address
	BRD16B,                 // Car Call Light #22 Output Board Address
	BRD16B,                 // Car Call Light #23 Output Board Address
	BRD16B,                 // Car Call Light #24 Output Board Address
	BRD16B,                 // Car Call Light #25 Output Board Address
	BRD16B,                 // Car Call Light #26 Output Board Address
	BRD16B,                 // Car Call Light #27 Output Board Address
	BRD16B,                 // Car Call Light #28 Output Board Address
	BRD16C,                 // Car Call Light #29 Output Board Address
	BRD16C,                 // Car Call Light #30 Output Board Address
	BRD16C,                 // Car Call Light #31 Output Board Address
	BRD16C,                 // Car Call Light #32 Output Board Address
	BRD16C,                 // Car Call Light #33 Output Board Address
	BRD16C,                 // Car Call Light #34 Output Board Address
	BRD16C,                 // Car Call Light #35 Output Board Address
	BRD16C,                 // Car Call Light #36 Output Board Address
	// ON 3rd Serial Car Board
	BRD30A,                 // Car Call Light #37 Output Board Address
	BRD30A,                 // Car Call Light #38 Output Board Address
	BRD30A,                 // Car Call Light #39 Output Board Address
	BRD30A,                 // Car Call Light #40 Output Board Address
	BRD30A,                 // Car Call Light #41 Output Board Address
	BRD30A,                 // Car Call Light #42 Output Board Address
	BRD30A,                 // Car Call Light #43 Output Board Address
	BRD30A,                 // Car Call Light #44 Output Board Address
	BRD30B,                 // Car Call Light #45 Output Board Address
	BRD30B,                 // Car Call Light #46 Output Board Address
	BRD30B,                 // Car Call Light #47 Output Board Address
	BRD30B,                 // Car Call Light #48 Output Board Address
	BRD30B,                 // Car Call Light #49 Output Board Address
	BRD30B,                 // Car Call Light #50 Output Board Address
	BRD30B,                 // Car Call Light #51 Output Board Address
	BRD30B,                 // Car Call Light #52 Output Board Address
	BRD30C,                 // Car Call Light #53 Output Board Address
	BRD30C,                 // Car Call Light #54 Output Board Address
	BRD30C,                 // Car Call Light #55 Output Board Address
	BRD30C,                 // Car Call Light #56 Output Board Address
	BRD30C,                 // Car Call Light #57 Output Board Address
	BRD30C,                 // Car Call Light #58 Output Board Address
	BRD30C,                 // Car Call Light #59 Output Board Address
	BRD30C                  // Car Call Light #60 Output Board Address
	} 
};
extern const unsigned char cclb[3][61] = {
{			  // Used with serial car call
	0,                      // Not Used
	// ON TOC Serial Car Board
	BIT1,                   // Car Call Light #1 Output Bit Number
	BIT2,                   // Car Call Light #2 Output Bit Number
	BIT3,                   // Car Call Light #3 Output Bit Number
	BIT4,                   // Car Call Light #4 Output Bit Number
	BIT5,                   // Car Call Light #5 Output Bit Number
	BIT6,                   // Car Call Light #6 Output Bit Number
	BIT7,                   // Car Call Light #7 Output Bit Number
	BIT8,                   // Car Call Light #8 Output Bit Number
	BIT1,                   // Car Call Light #9 Output Bit Number
	BIT2,                   // Car Call Light #10 Output Bit Number
	BIT3,                   // Car Call Light #11 Output Bit Number
	BIT4,                   // Car Call Light #12 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Car Call Light #13 Output Bit Number
	BIT2,                   // Car Call Light #14 Output Bit Number
	BIT3,                   // Car Call Light #15 Output Bit Number
	BIT4,                   // Car Call Light #16 Output Bit Number
	BIT5,                   // Car Call Light #17 Output Bit Number
	BIT6,                   // Car Call Light #18 Output Bit Number
	BIT7,                   // Car Call Light #19 Output Bit Number
	BIT8,                   // Car Call Light #20 Output Bit Number
	BIT1,                   // Car Call Light #21 Output Bit Number
	BIT2,                   // Car Call Light #22 Output Bit Number
	BIT3,                   // Car Call Light #23 Output Bit Number
	BIT4,                   // Car Call Light #24 Output Bit Number
	BIT5,                   // Car Call Light #25 Output Bit Number
	BIT6,                   // Car Call Light #26 Output Bit Number
	BIT7,                   // Car Call Light #27 Output Bit Number
	BIT8,                   // Car Call Light #28 Output Bit Number
	BIT1,                   // Car Call Light #29 Output Bit Number
	BIT2,                   // Car Call Light #30 Output Bit Number
	BIT3,                   // Car Call Light #31 Output Bit Number
	BIT4,                   // Car Call Light #32 Output Bit Number
	BIT5,                   // Car Call Light #33 Output Bit Number
	BIT6,                   // Car Call Light #34 Output Bit Number
	BIT7,                   // Car Call Light #35 Output Bit Number
	BIT8,                   // Car Call Light #36 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Car Call Light #37 Output Bit Number
	BIT2,                   // Car Call Light #38 Output Bit Number
	BIT3,                   // Car Call Light #39 Output Bit Number
	BIT4,                   // Car Call Light #40 Output Bit Number
	BIT5,                   // Car Call Light #41 Output Bit Number
	BIT6,                   // Car Call Light #42 Output Bit Number
	BIT7,                   // Car Call Light #43 Output Bit Number
	BIT8,                   // Car Call Light #44 Output Bit Number
	BIT1,                   // Car Call Light #45 Output Bit Number
	BIT2,                   // Car Call Light #46 Output Bit Number
	BIT3,                   // Car Call Light #47 Output Bit Number
	BIT4,                   // Car Call Light #48 Output Bit Number
	BIT5,                   // Car Call Light #49 Output Bit Number
	BIT6,                   // Car Call Light #50 Output Bit Number
	BIT7,                   // Car Call Light #51 Output Bit Number
	BIT8,                   // Car Call Light #52 Output Bit Number
	BIT1,                   // Car Call Light #53 Output Bit Number
	BIT2,                   // Car Call Light #54 Output Bit Number
	BIT3,                   // Car Call Light #55 Output Bit Number
	BIT4,                   // Car Call Light #56 Output Bit Number
	BIT5,                   // Car Call Light #57 Output Bit Number
	BIT6,                   // Car Call Light #58 Output Bit Number
	BIT7,                   // Car Call Light #59 Output Bit Number
	BIT8                    // Car Call Light #60 Output Bit Number
	},
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Car Call Light #1 Output Bit Number
	BIT2,                   // Car Call Light #2 Output Bit Number
	BIT3,                   // Car Call Light #3 Output Bit Number
	BIT4,                   // Car Call Light #4 Output Bit Number
	BIT5,                   // Car Call Light #5 Output Bit Number
	BIT6,                   // Car Call Light #6 Output Bit Number
	BIT7,                   // Car Call Light #7 Output Bit Number
	BIT8,                   // Car Call Light #8 Output Bit Number
	BIT1,                   // Car Call Light #9 Output Bit Number
	BIT2,                   // Car Call Light #10 Output Bit Number
	BIT3,                   // Car Call Light #11 Output Bit Number
	BIT4,                   // Car Call Light #12 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Car Call Light #13 Output Bit Number
	BIT2,                   // Car Call Light #14 Output Bit Number
	BIT3,                   // Car Call Light #15 Output Bit Number
	BIT4,                   // Car Call Light #16 Output Bit Number
	BIT5,                   // Car Call Light #17 Output Bit Number
	BIT6,                   // Car Call Light #18 Output Bit Number
	BIT7,                   // Car Call Light #19 Output Bit Number
	BIT8,                   // Car Call Light #20 Output Bit Number
	BIT1,                   // Car Call Light #21 Output Bit Number
	BIT2,                   // Car Call Light #22 Output Bit Number
	BIT3,                   // Car Call Light #23 Output Bit Number
	BIT4,                   // Car Call Light #24 Output Bit Number
	BIT5,                   // Car Call Light #25 Output Bit Number
	BIT6,                   // Car Call Light #26 Output Bit Number
	BIT7,                   // Car Call Light #27 Output Bit Number
	BIT8,                   // Car Call Light #28 Output Bit Number
	BIT1,                   // Car Call Light #29 Output Bit Number
	BIT2,                   // Car Call Light #30 Output Bit Number
	BIT3,                   // Car Call Light #31 Output Bit Number
	BIT4,                   // Car Call Light #32 Output Bit Number
	BIT5,                   // Car Call Light #33 Output Bit Number
	BIT6,                   // Car Call Light #34 Output Bit Number
	BIT7,                   // Car Call Light #35 Output Bit Number
	BIT8,                   // Car Call Light #36 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Car Call Light #37 Output Bit Number
	BIT2,                   // Car Call Light #38 Output Bit Number
	BIT3,                   // Car Call Light #39 Output Bit Number
	BIT4,                   // Car Call Light #40 Output Bit Number
	BIT5,                   // Car Call Light #41 Output Bit Number
	BIT6,                   // Car Call Light #42 Output Bit Number
	BIT7,                   // Car Call Light #43 Output Bit Number
	BIT8,                   // Car Call Light #44 Output Bit Number
	BIT1,                   // Car Call Light #45 Output Bit Number
	BIT2,                   // Car Call Light #46 Output Bit Number
	BIT3,                   // Car Call Light #47 Output Bit Number
	BIT4,                   // Car Call Light #48 Output Bit Number
	BIT5,                   // Car Call Light #49 Output Bit Number
	BIT6,                   // Car Call Light #50 Output Bit Number
	BIT7,                   // Car Call Light #51 Output Bit Number
	BIT8,                   // Car Call Light #52 Output Bit Number
	BIT1,                   // Car Call Light #53 Output Bit Number
	BIT2,                   // Car Call Light #54 Output Bit Number
	BIT3,                   // Car Call Light #55 Output Bit Number
	BIT4,                   // Car Call Light #56 Output Bit Number
	BIT5,                   // Car Call Light #57 Output Bit Number
	BIT6,                   // Car Call Light #58 Output Bit Number
	BIT7,                   // Car Call Light #59 Output Bit Number
	BIT8                    // Car Call Light #60 Output Bit Number
	},
{			  // Used with RGB car call boards
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Car Call Light #1 Output Bit Number
	BIT2,                   // Car Call Light #2 Output Bit Number
	BIT3,                   // Car Call Light #3 Output Bit Number
	BIT4,                   // Car Call Light #4 Output Bit Number
	BIT5,                   // Car Call Light #5 Output Bit Number
	BIT6,                   // Car Call Light #6 Output Bit Number
	BIT5,                   // Car Call Light #7 Output Bit Number
	BIT6,                   // Car Call Light #8 Output Bit Number
	BIT7,                   // Car Call Light #9 Output Bit Number
	BIT8,                   // Car Call Light #10 Output Bit Number
	BIT1,                   // Car Call Light #11 Output Bit Number
	BIT2,                   // Car Call Light #12 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Car Call Light #13 Output Bit Number
	BIT2,                   // Car Call Light #14 Output Bit Number
	BIT3,                   // Car Call Light #15 Output Bit Number
	BIT4,                   // Car Call Light #16 Output Bit Number
	BIT5,                   // Car Call Light #17 Output Bit Number
	BIT6,                   // Car Call Light #18 Output Bit Number
	BIT7,                   // Car Call Light #19 Output Bit Number
	BIT8,                   // Car Call Light #20 Output Bit Number
	BIT1,                   // Car Call Light #21 Output Bit Number
	BIT2,                   // Car Call Light #22 Output Bit Number
	BIT3,                   // Car Call Light #23 Output Bit Number
	BIT4,                   // Car Call Light #24 Output Bit Number
	BIT5,                   // Car Call Light #25 Output Bit Number
	BIT6,                   // Car Call Light #26 Output Bit Number
	BIT7,                   // Car Call Light #27 Output Bit Number
	BIT8,                   // Car Call Light #28 Output Bit Number
	BIT1,                   // Car Call Light #29 Output Bit Number
	BIT2,                   // Car Call Light #30 Output Bit Number
	BIT3,                   // Car Call Light #31 Output Bit Number
	BIT4,                   // Car Call Light #32 Output Bit Number
	BIT5,                   // Car Call Light #33 Output Bit Number
	BIT6,                   // Car Call Light #34 Output Bit Number
	BIT7,                   // Car Call Light #35 Output Bit Number
	BIT8,                   // Car Call Light #36 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Car Call Light #37 Output Bit Number
	BIT2,                   // Car Call Light #38 Output Bit Number
	BIT3,                   // Car Call Light #39 Output Bit Number
	BIT4,                   // Car Call Light #40 Output Bit Number
	BIT5,                   // Car Call Light #41 Output Bit Number
	BIT6,                   // Car Call Light #42 Output Bit Number
	BIT7,                   // Car Call Light #43 Output Bit Number
	BIT8,                   // Car Call Light #44 Output Bit Number
	BIT1,                   // Car Call Light #45 Output Bit Number
	BIT2,                   // Car Call Light #46 Output Bit Number
	BIT3,                   // Car Call Light #47 Output Bit Number
	BIT4,                   // Car Call Light #48 Output Bit Number
	BIT5,                   // Car Call Light #49 Output Bit Number
	BIT6,                   // Car Call Light #50 Output Bit Number
	BIT7,                   // Car Call Light #51 Output Bit Number
	BIT8,                   // Car Call Light #52 Output Bit Number
	BIT1,                   // Car Call Light #53 Output Bit Number
	BIT2,                   // Car Call Light #54 Output Bit Number
	BIT3,                   // Car Call Light #55 Output Bit Number
	BIT4,                   // Car Call Light #56 Output Bit Number
	BIT5,                   // Car Call Light #57 Output Bit Number
	BIT6,                   // Car Call Light #58 Output Bit Number
	BIT7,                   // Car Call Light #59 Output Bit Number
	BIT8                    // Car Call Light #60 Output Bit Number
	}
};
extern const unsigned char ccld[3][61] =  {
{			  // Used with serial car call
	0,                      // Not Used
	// ON TOC Serial Car Board
	69,                     // Car Call Light #1 Output Array Number
	69,                     // Car Call Light #2 Output Array Number
	69,                     // Car Call Light #3 Output Array Number
	69,                     // Car Call Light #4 Output Array Number
	69,                     // Car Call Light #5 Output Array Number
	69,                     // Car Call Light #6 Output Array Number
	69,                     // Car Call Light #7 Output Array Number
	69,                     // Car Call Light #8 Output Array Number
	70,                     // Car Call Light #9 Output Array Number
	70,                     // Car Call Light #10 Output Array Number
	70,                     // Car Call Light #11 Output Array Number
	70,                     // Car Call Light #12 Output Array Number
	// ON 2nd Serial Car Board
	45,                     // Car Call Light #13 Output Array Number
	45,                     // Car Call Light #14 Output Array Number
	45,                     // Car Call Light #15 Output Array Number
	45,                     // Car Call Light #16 Output Array Number
	45,                     // Car Call Light #17 Output Array Number
	45,                     // Car Call Light #18 Output Array Number
	45,                     // Car Call Light #19 Output Array Number
	45,                     // Car Call Light #20 Output Array Number
	46,                     // Car Call Light #21 Output Array Number
	46,                     // Car Call Light #22 Output Array Number
	46,                     // Car Call Light #23 Output Array Number
	46,                     // Car Call Light #24 Output Array Number
	46,                     // Car Call Light #25 Output Array Number
	46,                     // Car Call Light #26 Output Array Number
	46,                     // Car Call Light #27 Output Array Number
	46,                     // Car Call Light #28 Output Array Number
	47,                     // Car Call Light #29 Output Array Number
	47,                     // Car Call Light #30 Output Array Number
	47,                     // Car Call Light #31 Output Array Number
	47,                     // Car Call Light #32 Output Array Number
	47,                     // Car Call Light #33 Output Array Number
	47,                     // Car Call Light #34 Output Array Number
	47,                     // Car Call Light #35 Output Array Number
	47,                     // Car Call Light #36 Output Array Number
	// ON 3rd Serial Car Board
	87,                     // Car Call Light #37 Output Array Number
	87,                     // Car Call Light #38 Output Array Number
	87,                     // Car Call Light #39 Output Array Number
	87,                     // Car Call Light #40 Output Array Number
	87,                     // Car Call Light #41 Output Array Number
	87,                     // Car Call Light #42 Output Array Number
	87,                     // Car Call Light #43 Output Array Number
	87,                     // Car Call Light #44 Output Array Number
	88,                     // Car Call Light #45 Output Array Number
	88,                     // Car Call Light #46 Output Array Number
	88,                     // Car Call Light #47 Output Array Number
	88,                     // Car Call Light #48 Output Array Number
	88,                     // Car Call Light #49 Output Array Number
	88,                     // Car Call Light #50 Output Array Number
	88,                     // Car Call Light #51 Output Array Number
	88,                     // Car Call Light #52 Output Array Number
	89,                     // Car Call Light #53 Output Array Number
	89,                     // Car Call Light #54 Output Array Number
	89,                     // Car Call Light #55 Output Array Number
	89,                     // Car Call Light #56 Output Array Number
	89,                     // Car Call Light #57 Output Array Number
	89,                     // Car Call Light #58 Output Array Number
	89,                     // Car Call Light #59 Output Array Number
	89                      // Car Call Light #60 Output Array Number
	},
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	39,                     // Car Call Light #1 Output Array Number
	39,                     // Car Call Light #2 Output Array Number
	39,                     // Car Call Light #3 Output Array Number
	39,                     // Car Call Light #4 Output Array Number
	39,                     // Car Call Light #5 Output Array Number
	39,                     // Car Call Light #6 Output Array Number
	39,                     // Car Call Light #7 Output Array Number
	39,                     // Car Call Light #8 Output Array Number
	40,                     // Car Call Light #9 Output Array Number
	40,                     // Car Call Light #10 Output Array Number
	40,                     // Car Call Light #11 Output Array Number
	40,                     // Car Call Light #12 Output Array Number
	// ON 2nd Serial Car Board
	45,                     // Car Call Light #13 Output Array Number
	45,                     // Car Call Light #14 Output Array Number
	45,                     // Car Call Light #15 Output Array Number
	45,                     // Car Call Light #16 Output Array Number
	45,                     // Car Call Light #17 Output Array Number
	45,                     // Car Call Light #18 Output Array Number
	45,                     // Car Call Light #19 Output Array Number
	45,                     // Car Call Light #20 Output Array Number
	46,                     // Car Call Light #21 Output Array Number
	46,                     // Car Call Light #22 Output Array Number
	46,                     // Car Call Light #23 Output Array Number
	46,                     // Car Call Light #24 Output Array Number
	46,                     // Car Call Light #25 Output Array Number
	46,                     // Car Call Light #26 Output Array Number
	46,                     // Car Call Light #27 Output Array Number
	46,                     // Car Call Light #28 Output Array Number
	47,                     // Car Call Light #29 Output Array Number
	47,                     // Car Call Light #30 Output Array Number
	47,                     // Car Call Light #31 Output Array Number
	47,                     // Car Call Light #32 Output Array Number
	47,                     // Car Call Light #33 Output Array Number
	47,                     // Car Call Light #34 Output Array Number
	47,                     // Car Call Light #35 Output Array Number
	47,                     // Car Call Light #36 Output Array Number
	// ON 3rd Serial Car Board
	87,                     // Car Call Light #37 Output Array Number
	87,                     // Car Call Light #38 Output Array Number
	87,                     // Car Call Light #39 Output Array Number
	87,                     // Car Call Light #40 Output Array Number
	87,                     // Car Call Light #41 Output Array Number
	87,                     // Car Call Light #42 Output Array Number
	87,                     // Car Call Light #43 Output Array Number
	87,                     // Car Call Light #44 Output Array Number
	88,                     // Car Call Light #45 Output Array Number
	88,                     // Car Call Light #46 Output Array Number
	88,                     // Car Call Light #47 Output Array Number
	88,                     // Car Call Light #48 Output Array Number
	88,                     // Car Call Light #49 Output Array Number
	88,                     // Car Call Light #50 Output Array Number
	88,                     // Car Call Light #51 Output Array Number
	88,                     // Car Call Light #52 Output Array Number
	89,                     // Car Call Light #53 Output Array Number
	89,                     // Car Call Light #54 Output Array Number
	89,                     // Car Call Light #55 Output Array Number
	89,                     // Car Call Light #56 Output Array Number
	89,                     // Car Call Light #57 Output Array Number
	89,                     // Car Call Light #58 Output Array Number
	89,                     // Car Call Light #59 Output Array Number
	89                      // Car Call Light #60 Output Array Number
	},
{			  // Used with RBG car call boards
	0,                      // Not Used
	// ON 1st Serial Car Board
	129,                     // Car Call Light #1 Output Array Number
	129,                     // Car Call Light #2 Output Array Number
	129,                     // Car Call Light #3 Output Array Number
	129,                     // Car Call Light #4 Output Array Number
	129,                     // Car Call Light #5 Output Array Number
	129,                     // Car Call Light #6 Output Array Number
	130,                     // Car Call Light #7 Output Array Number
	130,                     // Car Call Light #8 Output Array Number
	130,                     // Car Call Light #9 Output Array Number
	130,                     // Car Call Light #10 Output Array Number
	131,                     // Car Call Light #11 Output Array Number
	131,                     // Car Call Light #12 Output Array Number
	// ON 2nd Serial Car Board
	45,                     // Car Call Light #13 Output Array Number
	45,                     // Car Call Light #14 Output Array Number
	45,                     // Car Call Light #15 Output Array Number
	45,                     // Car Call Light #16 Output Array Number
	45,                     // Car Call Light #17 Output Array Number
	45,                     // Car Call Light #18 Output Array Number
	45,                     // Car Call Light #19 Output Array Number
	45,                     // Car Call Light #20 Output Array Number
	46,                     // Car Call Light #21 Output Array Number
	46,                     // Car Call Light #22 Output Array Number
	46,                     // Car Call Light #23 Output Array Number
	46,                     // Car Call Light #24 Output Array Number
	46,                     // Car Call Light #25 Output Array Number
	46,                     // Car Call Light #26 Output Array Number
	46,                     // Car Call Light #27 Output Array Number
	46,                     // Car Call Light #28 Output Array Number
	47,                     // Car Call Light #29 Output Array Number
	47,                     // Car Call Light #30 Output Array Number
	47,                     // Car Call Light #31 Output Array Number
	47,                     // Car Call Light #32 Output Array Number
	47,                     // Car Call Light #33 Output Array Number
	47,                     // Car Call Light #34 Output Array Number
	47,                     // Car Call Light #35 Output Array Number
	47,                     // Car Call Light #36 Output Array Number
	// ON 3rd Serial Car Board
	87,                     // Car Call Light #37 Output Array Number
	87,                     // Car Call Light #38 Output Array Number
	87,                     // Car Call Light #39 Output Array Number
	87,                     // Car Call Light #40 Output Array Number
	87,                     // Car Call Light #41 Output Array Number
	87,                     // Car Call Light #42 Output Array Number
	87,                     // Car Call Light #43 Output Array Number
	87,                     // Car Call Light #44 Output Array Number
	88,                     // Car Call Light #45 Output Array Number
	88,                     // Car Call Light #46 Output Array Number
	88,                     // Car Call Light #47 Output Array Number
	88,                     // Car Call Light #48 Output Array Number
	88,                     // Car Call Light #49 Output Array Number
	88,                     // Car Call Light #50 Output Array Number
	88,                     // Car Call Light #51 Output Array Number
	88,                     // Car Call Light #52 Output Array Number
	89,                     // Car Call Light #53 Output Array Number
	89,                     // Car Call Light #54 Output Array Number
	89,                     // Car Call Light #55 Output Array Number
	89,                     // Car Call Light #56 Output Array Number
	89,                     // Car Call Light #57 Output Array Number
	89,                     // Car Call Light #58 Output Array Number
	89,                     // Car Call Light #59 Output Array Number
	89                      // Car Call Light #60 Output Array Number
	}
};

extern const int16 ccr[2][61] =  {	
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BRD17A,                  // Rear Car Call #1 Input Board Address
	BRD17A,                  // Rear Car Call #2 Input Board Address
	BRD17A,                  // Rear Car Call #3 Input Board Address
	BRD17A,                  // Rear Car Call #4 Input Board Address
	BRD17A,                  // Rear Car Call #5 Input Board Address
	BRD17A,                  // Rear Car Call #6 Input Board Address
	BRD17A,                  // Rear Car Call #7 Input Board Address
	BRD17A,                  // Rear Car Call #8 Input Board Address
	BRD17B,                  // Rear Car Call #9 Input Board Address
	BRD17B,                  // Rear Car Call #10 Input Board Address
	BRD17B,                  // Rear Car Call #11 Input  Board Address
	BRD17B,                  // Rear Car Call #12 Input  Board Address
	BRD17B,                  // Rear Car Call #13 Input Board Address
	BRD17B,                  // Rear Car Call #14 Input Board Address
	BRD17B,                  // Rear Car Call #15 Input Board Address
	BRD17B,                  // Rear Car Call #16 Input Board Address
	BRD17C,                 // Rear Car Call #17 Input Board Address
	// ON 2ND Serial Car Board
	BRD19A,                 // Rear Car Call #18 Input Board Address
	BRD19A,                 // Rear Car Call #19 Input Board Address
	BRD19A,                 // Rear Car Call #20 Input Board Address
	BRD19A,                 // Rear Car Call #21 Input Board Address
	BRD19A,                 // Rear Car Call #22 Input Board Address
	BRD19A,                 // Rear Car Call #23 Input Board Address
	BRD19A,                 // Rear Car Call #24 Input Board Address
	BRD19A,                 // Rear Car Call #25 Input Board Address
	BRD19B,                 // Rear Car Call #26 Input Board Address
	BRD19B,                 // Rear Car Call #27 Input Board Address
	BRD19B,                 // Rear Car Call #28 Input Board Address
	BRD19B,                 // Rear Car Call #29 Input Board Address
	BRD19B,                 // Rear Car Call #30 Input Board Address
	BRD19B,                 // Rear Car Call #31 Input Board Address
	BRD19B,                 // Rear Car Call #32 Input Board Address
	BRD19B,                 // Rear Car Call #33 Input Board Address
	BRD19C,                 // Rear Car Call #34 Input Board Address
	BRD19C,                 // Rear Car Call #35 Input Board Address
	BRD19C,                 // Rear Car Call #36 Input Board Address
	BRD19C,                 // Rear Car Call #37 Input Board Address
	BRD19C,                 // Rear Car Call #38 Input Board Address
	BRD19C,                 // Rear Car Call #39 Input Board Address
	BRD19C,                 // Rear Car Call #40 Input Board Address
	BRD19C,                 // Rear Car Call #41 Input Board Address
	// ON 3RD Serial Car Board
	BRD31A,                 // Rear Car Call #42 Input Board Address
	BRD31A,                 // Rear Car Call #43 Input Board Address
	BRD31A,                 // Rear Car Call #44 Input Board Address
	BRD31A,                 // Rear Car Call #45 Input Board Address
	BRD31A,                 // Rear Car Call #46 Input Board Address
	BRD31A,                 // Rear Car Call #47 Input Board Address
	BRD31A,                 // Rear Car Call #48 Input Board Address
	BRD31A,                 // Rear Car Call #49 Input Board Address
	BRD31B,                 // Rear Car Call #50 Input Board Address
	BRD31B,                 // Rear Car Call #51 Input Board Address
	BRD31B,                 // Rear Car Call #52 Input Board Address
	BRD31B,                 // Rear Car Call #53 Input Board Address
	BRD31B,                 // Rear Car Call #54 Input Board Address
	BRD31B,                 // Rear Car Call #55 Input Board Address
	BRD31B,                 // Rear Car Call #56 Input Board Address
	BRD31B,                 // Rear Car Call #57 Input Board Address
	BRD31C,                 // Rear Car Call #58 Input Board Address
	BRD31C,                 // Rear Car Call #59 Input Board Address
	BRD31C                  // Rear Car Call #60 Input Board Address
	},
{			 // Used with RGB Car Calls
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BRD17A,                  // Rear Car Call #1 Input Board Address
	BRD17A,                  // Rear Car Call #2 Input Board Address
	BRD17A,                  // Rear Car Call #3 Input Board Address
	BRD17A,                  // Rear Car Call #4 Input Board Address
	BRD17A,                  // Rear Car Call #5 Input Board Address
	BRD17A,                  // Rear Car Call #6 Input Board Address
	BRD17A,                  // Rear Car Call #7 Input Board Address
	BRD17A,                  // Rear Car Call #8 Input Board Address
	BRD17B,                  // Rear Car Call #9 Input Board Address
	BRD17B,                  // Rear Car Call #10 Input Board Address
	BRD17B,                  // Rear Car Call #11 Input  Board Address
	BRD17B,                  // Rear Car Call #12 Input  Board Address
	BRD17B,                  // Rear Car Call #13 Input Board Address
	BRD17B,                  // Rear Car Call #14 Input Board Address
	BRD17C,                  // Rear Car Call #15 Input Board Address
	BRD17C,                  // Rear Car Call #16 Input Board Address
	BRD17C,                 // Rear Car Call #17 Input Board Address
	// ON 2ND Serial Car Board
	BRD19A,                 // Rear Car Call #18 Input Board Address
	BRD19A,                 // Rear Car Call #19 Input Board Address
	BRD19A,                 // Rear Car Call #20 Input Board Address
	BRD19A,                 // Rear Car Call #21 Input Board Address
	BRD19A,                 // Rear Car Call #22 Input Board Address
	BRD19A,                 // Rear Car Call #23 Input Board Address
	BRD19A,                 // Rear Car Call #24 Input Board Address
	BRD19A,                 // Rear Car Call #25 Input Board Address
	BRD19B,                 // Rear Car Call #26 Input Board Address
	BRD19B,                 // Rear Car Call #27 Input Board Address
	BRD19B,                 // Rear Car Call #28 Input Board Address
	BRD19B,                 // Rear Car Call #29 Input Board Address
	BRD19B,                 // Rear Car Call #30 Input Board Address
	BRD19B,                 // Rear Car Call #31 Input Board Address
	BRD19B,                 // Rear Car Call #32 Input Board Address
	BRD19B,                 // Rear Car Call #33 Input Board Address
	BRD19C,                 // Rear Car Call #34 Input Board Address
	BRD19C,                 // Rear Car Call #35 Input Board Address
	BRD19C,                 // Rear Car Call #36 Input Board Address
	BRD19C,                 // Rear Car Call #37 Input Board Address
	BRD19C,                 // Rear Car Call #38 Input Board Address
	BRD19C,                 // Rear Car Call #39 Input Board Address
	BRD19C,                 // Rear Car Call #40 Input Board Address
	BRD19C,                 // Rear Car Call #41 Input Board Address
	// ON 3RD Serial Car Board
	BRD31A,                 // Rear Car Call #42 Input Board Address
	BRD31A,                 // Rear Car Call #43 Input Board Address
	BRD31A,                 // Rear Car Call #44 Input Board Address
	BRD31A,                 // Rear Car Call #45 Input Board Address
	BRD31A,                 // Rear Car Call #46 Input Board Address
	BRD31A,                 // Rear Car Call #47 Input Board Address
	BRD31A,                 // Rear Car Call #48 Input Board Address
	BRD31A,                 // Rear Car Call #49 Input Board Address
	BRD31B,                 // Rear Car Call #50 Input Board Address
	BRD31B,                 // Rear Car Call #51 Input Board Address
	BRD31B,                 // Rear Car Call #52 Input Board Address
	BRD31B,                 // Rear Car Call #53 Input Board Address
	BRD31B,                 // Rear Car Call #54 Input Board Address
	BRD31B,                 // Rear Car Call #55 Input Board Address
	BRD31B,                 // Rear Car Call #56 Input Board Address
	BRD31B,                 // Rear Car Call #57 Input Board Address
	BRD31C,                 // Rear Car Call #58 Input Board Address
	BRD31C,                 // Rear Car Call #59 Input Board Address
	BRD31C                  // Rear Car Call #60 Input Board Address
	}
};
extern const unsigned char ccrb[2][61] = { 
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BIT1,                   // Rear Car Call #1 Input Bit Number
	BIT2,                   // Rear Car Call #2 Input Bit Number
	BIT3,                   // Rear Car Call #3 Input Bit Number
	BIT4,                   // Rear Car Call #4 Input Bit Number
	BIT5,                   // Rear Car Call #5 Input Bit Number
	BIT6,                   // Rear Car Call #6 Input Bit Number
	BIT7,                   // Rear Car Call #7 Input Bit Number
	BIT8,                   // Rear Car Call #8 Input Bit Number
	BIT1,                   // Rear Car Call #9 Input Bit Number
	BIT2,                   // Rear Car Call #10 Input Bit Number
	BIT3,                   // Rear Car Call #11 Input Bit Number
	BIT4,                   // Rear Car Call #12 Input Bit Number
	BIT5,                   // Rear Car Call #13 Input Bit Number
	BIT6,                   // Rear Car Call #14 Input Bit Number
	BIT7,                   // Rear Car Call #15 Input Bit Number
	BIT8,                   // Rear Car Call #16 Input Bit Number
	BIT1,                   // Rear Car Call #17 Input Bit Number
	// ON 2ND Serial Car Board
	BIT1,                   // Rear Car Call #18 Input Bit Number
	BIT2,                   // Rear Car Call #19 Input Bit Number
	BIT3,                   // Rear Car Call #20 Input Bit Number
	BIT4,                   // Rear Car Call #21 Input Bit Number
	BIT5,                   // Rear Car Call #22 Input Bit Number
	BIT6,                   // Rear Car Call #23 Input Bit Number
	BIT7,                   // Rear Car Call #24 Input Bit Number
	BIT8,                   // Rear Car Call #25 Input Bit Number
	BIT1,                   // Rear Car Call #26 Input Bit Number
	BIT2,                   // Rear Car Call #27 Input Bit Number
	BIT3,                   // Rear Car Call #28 Input Bit Number
	BIT4,                   // Rear Car Call #29 Input Bit Number
	BIT5,                   // Rear Car Call #30 Input Bit Number
	BIT6,                   // Rear Car Call #31 Input Bit Number
	BIT7,                   // Rear Car Call #32 Input Bit Number
	BIT8,                   // Rear Car Call #33 Input Bit Number
	BIT1,                   // Rear Car Call #34 Input Bit Number
	BIT2,                   // Rear Car Call #35 Input Bit Number
	BIT3,                   // Rear Car Call #36 Input Bit Number
	BIT4,                   // Rear Car Call #37 Input Bit Number
	BIT5,                   // Rear Car Call #38 Input Bit Number
	BIT6,                   // Rear Car Call #39 Input Bit Number
	BIT7,                   // Rear Car Call #40 Input Bit Number
	BIT8,                   // Rear Car Call #41 Input Bit Number
	// ON 3RD Serial Car Board
	BIT1,                   // Rear Car Call #42 Input Bit Number
	BIT2,                   // Rear Car Call #43 Input Bit Number
	BIT3,                   // Rear Car Call #44 Input Bit Number
	BIT4,                   // Rear Car Call #45 Input Bit Number
	BIT5,                   // Rear Car Call #46 Input Bit Number
	BIT6,                   // Rear Car Call #47 Input Bit Number
	BIT7,                   // Rear Car Call #48 Input Bit Number
	BIT8,                   // Rear Car Call #49 Input Bit Number
	BIT1,                   // Rear Car Call #50 Input Bit Number
	BIT2,                   // Rear Car Call #51 Input Bit Number
	BIT3,                   // Rear Car Call #52 Input Bit Number
	BIT4,                   // Rear Car Call #53 Input Bit Number
	BIT5,                   // Rear Car Call #54 Input Bit Number
	BIT6,                   // Rear Car Call #55 Input Bit Number
	BIT7,                   // Rear Car Call #56 Input Bit Number
	BIT8,                   // Rear Car Call #57 Input Bit Number
	BIT1,                   // Rear Car Call #58 Input Bit Number
	BIT2,                   // Rear Car Call #59 Input Bit Number
	BIT3,                   // Rear Car Call #60 Input Bit Number
	},
{			  // Used with RGB Car Calls
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BIT1,                   // Rear Car Call #1 Input Bit Number
	BIT2,                   // Rear Car Call #2 Input Bit Number
	BIT3,                   // Rear Car Call #3 Input Bit Number
	BIT4,                   // Rear Car Call #4 Input Bit Number
	BIT5,                   // Rear Car Call #5 Input Bit Number
	BIT6,                   // Rear Car Call #6 Input Bit Number
	BIT7,                   // Rear Car Call #7 Input Bit Number
	BIT8,                   // Rear Car Call #8 Input Bit Number
	BIT1,                   // Rear Car Call #9 Input Bit Number
	BIT2,                   // Rear Car Call #10 Input Bit Number
	BIT5,                   // Rear Car Call #11 Input Bit Number
	BIT6,                   // Rear Car Call #12 Input Bit Number
	BIT7,                   // Rear Car Call #13 Input Bit Number
	BIT8,                   // Rear Car Call #14 Input Bit Number
	BIT1,                   // Rear Car Call #15 Input Bit Number
	BIT2,                   // Rear Car Call #16 Input Bit Number
	BIT3,                   // Rear Car Call #17 Input Bit Number
	// ON 2ND Serial Car Board
	BIT1,                   // Rear Car Call #18 Input Bit Number
	BIT2,                   // Rear Car Call #19 Input Bit Number
	BIT3,                   // Rear Car Call #20 Input Bit Number
	BIT4,                   // Rear Car Call #21 Input Bit Number
	BIT5,                   // Rear Car Call #22 Input Bit Number
	BIT6,                   // Rear Car Call #23 Input Bit Number
	BIT7,                   // Rear Car Call #24 Input Bit Number
	BIT8,                   // Rear Car Call #25 Input Bit Number
	BIT1,                   // Rear Car Call #26 Input Bit Number
	BIT2,                   // Rear Car Call #27 Input Bit Number
	BIT3,                   // Rear Car Call #28 Input Bit Number
	BIT4,                   // Rear Car Call #29 Input Bit Number
	BIT5,                   // Rear Car Call #30 Input Bit Number
	BIT6,                   // Rear Car Call #31 Input Bit Number
	BIT7,                   // Rear Car Call #32 Input Bit Number
	BIT8,                   // Rear Car Call #33 Input Bit Number
	BIT1,                   // Rear Car Call #34 Input Bit Number
	BIT2,                   // Rear Car Call #35 Input Bit Number
	BIT3,                   // Rear Car Call #36 Input Bit Number
	BIT4,                   // Rear Car Call #37 Input Bit Number
	BIT5,                   // Rear Car Call #38 Input Bit Number
	BIT6,                   // Rear Car Call #39 Input Bit Number
	BIT7,                   // Rear Car Call #40 Input Bit Number
	BIT8,                   // Rear Car Call #41 Input Bit Number
	// ON 3RD Serial Car Board
	BIT1,                   // Rear Car Call #42 Input Bit Number
	BIT2,                   // Rear Car Call #43 Input Bit Number
	BIT3,                   // Rear Car Call #44 Input Bit Number
	BIT4,                   // Rear Car Call #45 Input Bit Number
	BIT5,                   // Rear Car Call #46 Input Bit Number
	BIT6,                   // Rear Car Call #47 Input Bit Number
	BIT7,                   // Rear Car Call #48 Input Bit Number
	BIT8,                   // Rear Car Call #49 Input Bit Number
	BIT1,                   // Rear Car Call #50 Input Bit Number
	BIT2,                   // Rear Car Call #51 Input Bit Number
	BIT3,                   // Rear Car Call #52 Input Bit Number
	BIT4,                   // Rear Car Call #53 Input Bit Number
	BIT5,                   // Rear Car Call #54 Input Bit Number
	BIT6,                   // Rear Car Call #55 Input Bit Number
	BIT7,                   // Rear Car Call #56 Input Bit Number
	BIT8,                   // Rear Car Call #57 Input Bit Number
	BIT1,                   // Rear Car Call #58 Input Bit Number
	BIT2,                   // Rear Car Call #59 Input Bit Number
	BIT3,                   // Rear Car Call #60 Input Bit Number
	}
};

extern const unsigned char ccrd[2][61] = { 
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	48,                     // Rear Car Call #1 Input Array Number
	48,                     // Rear Car Call #2 Input Array Number
	48,                     // Rear Car Call #3 Input Array Number
	48,                     // Rear Car Call #4 Input Array Number
	48,                     // Rear Car Call #5 Input Array Number
	48,                     // Rear Car Call #6 Input Array Number
	48,                     // Rear Car Call #7 Input Array Number
	48,                     // Rear Car Call #8 Input Array Number
	49,                     // Rear Car Call #9 Input Array Number
	49,                     // Rear Car Call #10 Input Array Number
	49,                     // Rear Car Call #11 Input Array Number
	49,                     // Rear Car Call #12 Input Array Number
	49,                     // Rear Car Call #13 Input Array Number
	49,                     // Rear Car Call #14 Input Array Number
	49,                     // Rear Car Call #15 Input Array Number
	49,                     // Rear Car Call #16 Input Array Number
	50,                     // Rear Car Call #17 Input Array Number
	// ON 2nd Serial Car Board
	54,                     // Rear Car Call #18 Input Array Number
	54,                     // Rear Car Call #19 Input Array Number
	54,                     // Rear Car Call #20 Input Array Number
	54,                     // Rear Car Call #21 Input Array Number
	54,                     // Rear Car Call #22 Input Array Number
	54,                     // Rear Car Call #23 Input Array Number
	54,                     // Rear Car Call #24 Input Array Number
	54,                     // Rear Car Call #25 Input Array Number
	55,                     // Rear Car Call #26 Input Array Number
	55,                     // Rear Car Call #27 Input Array Number
	55,                     // Rear Car Call #28 Input Array Number
	55,                     // Rear Car Call #29 Input Array Number
	55,                     // Rear Car Call #30 Input Array Number
	55,                     // Rear Car Call #31 Input Array Number
	55,                     // Rear Car Call #32 Input Array Number
	55,                     // Rear Car Call #33 Input Array Number
	56,                     // Rear Car Call #34 Input Array Number
	56,                     // Rear Car Call #35 Input Array Number
	56,                     // Rear Car Call #36 Input Array Number
	56,                     // Rear Car Call #37 Input Array Number
	56,                     // Rear Car Call #38 Input Array Number
	56,                     // Rear Car Call #39 Input Array Number
	56,                     // Rear Car Call #30 Input Array Number
	56,                     // Rear Car Call #41 Input Array Number
	// ON 2nd Serial Car Board
	90,                     // Rear Car Call #42 Input Array Number
	90,                     // Rear Car Call #43 Input Array Number
	90,                     // Rear Car Call #44 Input Array Number
	90,                     // Rear Car Call #45 Input Array Number
	90,                     // Rear Car Call #46 Input Array Number
	90,                     // Rear Car Call #47 Input Array Number
	90,                     // Rear Car Call #48 Input Array Number
	90,                     // Rear Car Call #49 Input Array Number
	91,                     // Rear Car Call #50 Input Array Number
	91,                     // Rear Car Call #51 Input Array Number
	91,                     // Rear Car Call #52 Input Array Number
	91,                     // Rear Car Call #53 Input Array Number
	91,                     // Rear Car Call #54 Input Array Number
	91,                     // Rear Car Call #55 Input Array Number
	91,                     // Rear Car Call #56 Input Array Number
	91,                     // Rear Car Call #57 Input Array Number
	92,                     // Rear Car Call #58 Input Array Number
	92,                     // Rear Car Call #59 Input Array Number
	92                      // Rear Car Call #60 Input Array Number
	},
{			  // Used with RGB car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	48,                     // Rear Car Call #1 Input Array Number
	48,                     // Rear Car Call #2 Input Array Number
	48,                     // Rear Car Call #3 Input Array Number
	48,                     // Rear Car Call #4 Input Array Number
	48,                     // Rear Car Call #5 Input Array Number
	48,                     // Rear Car Call #6 Input Array Number
	48,                     // Rear Car Call #7 Input Array Number
	48,                     // Rear Car Call #8 Input Array Number
	49,                     // Rear Car Call #9 Input Array Number
	49,                     // Rear Car Call #10 Input Array Number
	49,                     // Rear Car Call #11 Input Array Number
	49,                     // Rear Car Call #12 Input Array Number
	49,                     // Rear Car Call #13 Input Array Number
	49,                     // Rear Car Call #14 Input Array Number
	50,                     // Rear Car Call #15 Input Array Number
	50,                     // Rear Car Call #16 Input Array Number
	50,                     // Rear Car Call #17 Input Array Number
	// ON 2nd Serial Car Board
	54,                     // Rear Car Call #18 Input Array Number
	54,                     // Rear Car Call #19 Input Array Number
	54,                     // Rear Car Call #20 Input Array Number
	54,                     // Rear Car Call #21 Input Array Number
	54,                     // Rear Car Call #22 Input Array Number
	54,                     // Rear Car Call #23 Input Array Number
	54,                     // Rear Car Call #24 Input Array Number
	54,                     // Rear Car Call #25 Input Array Number
	55,                     // Rear Car Call #26 Input Array Number
	55,                     // Rear Car Call #27 Input Array Number
	55,                     // Rear Car Call #28 Input Array Number
	55,                     // Rear Car Call #29 Input Array Number
	55,                     // Rear Car Call #30 Input Array Number
	55,                     // Rear Car Call #31 Input Array Number
	55,                     // Rear Car Call #32 Input Array Number
	55,                     // Rear Car Call #33 Input Array Number
	56,                     // Rear Car Call #34 Input Array Number
	56,                     // Rear Car Call #35 Input Array Number
	56,                     // Rear Car Call #36 Input Array Number
	56,                     // Rear Car Call #37 Input Array Number
	56,                     // Rear Car Call #38 Input Array Number
	56,                     // Rear Car Call #39 Input Array Number
	56,                     // Rear Car Call #30 Input Array Number
	56,                     // Rear Car Call #41 Input Array Number
	// ON 2nd Serial Car Board
	90,                     // Rear Car Call #42 Input Array Number
	90,                     // Rear Car Call #43 Input Array Number
	90,                     // Rear Car Call #44 Input Array Number
	90,                     // Rear Car Call #45 Input Array Number
	90,                     // Rear Car Call #46 Input Array Number
	90,                     // Rear Car Call #47 Input Array Number
	90,                     // Rear Car Call #48 Input Array Number
	90,                     // Rear Car Call #49 Input Array Number
	91,                     // Rear Car Call #50 Input Array Number
	91,                     // Rear Car Call #51 Input Array Number
	91,                     // Rear Car Call #52 Input Array Number
	91,                     // Rear Car Call #53 Input Array Number
	91,                     // Rear Car Call #54 Input Array Number
	91,                     // Rear Car Call #55 Input Array Number
	91,                     // Rear Car Call #56 Input Array Number
	91,                     // Rear Car Call #57 Input Array Number
	92,                     // Rear Car Call #58 Input Array Number
	92,                     // Rear Car Call #59 Input Array Number
	92                      // Rear Car Call #60 Input Array Number
	}
};

extern const int16 ccrl[2][61] = {
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD18A,                  // Rear Car Call Light #1 Output Board Address
	BRD18A,                  // Rear Car Call Light #2 Output Board Address
	BRD18A,                  // Rear Car Call Light #3 Output Board Address
	BRD18A,                  // Rear Car Call Light #4 Output Board Address
	BRD18A,                  // Rear Car Call Light #5 Output Board Address
	BRD18A,                  // Rear Car Call Light #6 Output Board Address
	BRD18A,                  // Rear Car Call Light #7 Output Board Address
	BRD18A,                  // Rear Car Call Light #8 Output Board Address
	BRD18B,                  // Rear Car Call Light #9 Output Board Address
	BRD18B,                  // Rear Car Call Light #10 Output  Board Address
	BRD18B,                  // Rear Car Call Light #11 Output  Board Address
	BRD18B,                  // Rear Car Call Light #12 Output Board Address
	BRD18B,                  // Rear Car Call Light #13 Output Board Address
	BRD18B,                  // Rear Car Call Light #14 Output Board Address
	BRD18B,                  // Rear Car Call Light #15 Output Board Address
	BRD18B,                  // Rear Car Call Light #16 Output Board Address
	BRD18C,                 // Rear Car Call Light #17 Output Board Address
	// ON 2nd Serial Car Board
	BRD20A,                 // Rear Car Call Light #18 Output Board Address
	BRD20A,                 // Rear Car Call Light #19 Output Board Address
	BRD20A,                 // Rear Car Call Light #20 Output Board Address
	BRD20A,                 // Rear Car Call Light #21 Output Board Address
	BRD20A,                 // Rear Car Call Light #22 Output Board Address
	BRD20A,                 // Rear Car Call Light #23 Output Board Address
	BRD20A,                 // Rear Car Call Light #24 Output Board Address
	BRD20A,                 // Rear Car Call Light #25 Output Board Address
	BRD20B,                 // Rear Car Call Light #26 Output Board Address
	BRD20B,                 // Rear Car Call Light #27 Output Board Address
	BRD20B,                 // Rear Car Call Light #28 Output Board Address
	BRD20B,                 // Rear Car Call Light #29 Output Board Address
	BRD20B,                 // Rear Car Call Light #30 Output Board Address
	BRD20B,                 // Rear Car Call Light #31 Output Board Address
	BRD20B,                 // Rear Car Call Light #32 Output Board Address
	BRD20B,                 // Rear Car Call Light #33 Output Board Address
	BRD20C,                 // Rear Car Call Light #34 Output Board Address
	BRD20C,                 // Rear Car Call Light #35 Output Board Address
	BRD20C,                 // Rear Car Call Light #36 Output Board Address
	BRD20C,                 // Rear Car Call Light #37 Output Board Address
	BRD20C,                 // Rear Car Call Light #38 Output Board Address
	BRD20C,                 // Rear Car Call Light #39 Output Board Address
	BRD20C,                 // Rear Car Call Light #40 Output Board Address
	BRD20C,                 // Rear Car Call Light #41 Output Board Address
	// ON 3rd Serial Car Board
	BRD32A,                 // Rear Car Call Light #42 Output Board Address
	BRD32A,                 // Rear Car Call Light #43 Output Board Address
	BRD32A,                 // Rear Car Call Light #44 Output Board Address
	BRD32A,                 // Rear Car Call Light #45 Output Board Address
	BRD32A,                 // Rear Car Call Light #46 Output Board Address
	BRD32A,                 // Rear Car Call Light #47 Output Board Address
	BRD32A,                 // Rear Car Call Light #48 Output Board Address
	BRD32A,                 // Rear Car Call Light #49 Output Board Address
	BRD32B,                 // Rear Car Call Light #50 Output Board Address
	BRD32B,                 // Rear Car Call Light #51 Output Board Address
	BRD32B,                 // Rear Car Call Light #52 Output Board Address
	BRD32B,                 // Rear Car Call Light #53 Output Board Address
	BRD32B,                 // Rear Car Call Light #54 Output Board Address
	BRD32B,                 // Rear Car Call Light #55 Output Board Address
	BRD32B,                 // Rear Car Call Light #56 Output Board Address
	BRD32B,                 // Rear Car Call Light #57 Output Board Address
	BRD32C,                 // Rear Car Call Light #58 Output Board Address
	BRD32C,                 // Rear Car Call Light #59 Output Board Address
	BRD32C                  // Rear Car Call Light #60 Output Board Address
	},
{			  // Used with RBG car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD18A,                  // Rear Car Call Light #1 Output Board Address
	BRD18A,                  // Rear Car Call Light #2 Output Board Address
	BRD18A,                  // Rear Car Call Light #3 Output Board Address
	BRD18A,                  // Rear Car Call Light #4 Output Board Address
	BRD18A,                  // Rear Car Call Light #5 Output Board Address
	BRD18A,                  // Rear Car Call Light #6 Output Board Address
	BRD18A,                  // Rear Car Call Light #7 Output Board Address
	BRD18A,                  // Rear Car Call Light #8 Output Board Address
	BRD18B,                  // Rear Car Call Light #9 Output Board Address
	BRD18B,                  // Rear Car Call Light #10 Output  Board Address
	BRD18B,                  // Rear Car Call Light #11 Output  Board Address
	BRD18B,                  // Rear Car Call Light #12 Output Board Address
	BRD18B,                  // Rear Car Call Light #13 Output Board Address
	BRD18B,                  // Rear Car Call Light #14 Output Board Address
	BRD18C,                  // Rear Car Call Light #15 Output Board Address
	BRD18C,                  // Rear Car Call Light #16 Output Board Address
	BRD18C,                 // Rear Car Call Light #17 Output Board Address
	// ON 2nd Serial Car Board
	BRD20A,                 // Rear Car Call Light #18 Output Board Address
	BRD20A,                 // Rear Car Call Light #19 Output Board Address
	BRD20A,                 // Rear Car Call Light #20 Output Board Address
	BRD20A,                 // Rear Car Call Light #21 Output Board Address
	BRD20A,                 // Rear Car Call Light #22 Output Board Address
	BRD20A,                 // Rear Car Call Light #23 Output Board Address
	BRD20A,                 // Rear Car Call Light #24 Output Board Address
	BRD20A,                 // Rear Car Call Light #25 Output Board Address
	BRD20B,                 // Rear Car Call Light #26 Output Board Address
	BRD20B,                 // Rear Car Call Light #27 Output Board Address
	BRD20B,                 // Rear Car Call Light #28 Output Board Address
	BRD20B,                 // Rear Car Call Light #29 Output Board Address
	BRD20B,                 // Rear Car Call Light #30 Output Board Address
	BRD20B,                 // Rear Car Call Light #31 Output Board Address
	BRD20B,                 // Rear Car Call Light #32 Output Board Address
	BRD20B,                 // Rear Car Call Light #33 Output Board Address
	BRD20C,                 // Rear Car Call Light #34 Output Board Address
	BRD20C,                 // Rear Car Call Light #35 Output Board Address
	BRD20C,                 // Rear Car Call Light #36 Output Board Address
	BRD20C,                 // Rear Car Call Light #37 Output Board Address
	BRD20C,                 // Rear Car Call Light #38 Output Board Address
	BRD20C,                 // Rear Car Call Light #39 Output Board Address
	BRD20C,                 // Rear Car Call Light #40 Output Board Address
	BRD20C,                 // Rear Car Call Light #41 Output Board Address
	// ON 3rd Serial Car Board
	BRD32A,                 // Rear Car Call Light #42 Output Board Address
	BRD32A,                 // Rear Car Call Light #43 Output Board Address
	BRD32A,                 // Rear Car Call Light #44 Output Board Address
	BRD32A,                 // Rear Car Call Light #45 Output Board Address
	BRD32A,                 // Rear Car Call Light #46 Output Board Address
	BRD32A,                 // Rear Car Call Light #47 Output Board Address
	BRD32A,                 // Rear Car Call Light #48 Output Board Address
	BRD32A,                 // Rear Car Call Light #49 Output Board Address
	BRD32B,                 // Rear Car Call Light #50 Output Board Address
	BRD32B,                 // Rear Car Call Light #51 Output Board Address
	BRD32B,                 // Rear Car Call Light #52 Output Board Address
	BRD32B,                 // Rear Car Call Light #53 Output Board Address
	BRD32B,                 // Rear Car Call Light #54 Output Board Address
	BRD32B,                 // Rear Car Call Light #55 Output Board Address
	BRD32B,                 // Rear Car Call Light #56 Output Board Address
	BRD32B,                 // Rear Car Call Light #57 Output Board Address
	BRD32C,                 // Rear Car Call Light #58 Output Board Address
	BRD32C,                 // Rear Car Call Light #59 Output Board Address
	BRD32C                  // Rear Car Call Light #60 Output Board Address
	}
};
extern const unsigned char ccrlb[2][61] = {
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Rear Car Call Light #1 Output Bit Number
	BIT2,                   // Rear Car Call Light #2 Output Bit Number
	BIT3,                   // Rear Car Call Light #3 Output Bit Number
	BIT4,                   // Rear Car Call Light #4 Output Bit Number
	BIT5,                   // Rear Car Call Light #5 Output Bit Number
	BIT6,                   // Rear Car Call Light #6 Output Bit Number
	BIT7,                   // Rear Car Call Light #7 Output Bit Number
	BIT8,                   // Rear Car Call Light #8 Output Bit Number
	BIT1,                   // Rear Car Call Light #9 Output Bit Number
	BIT2,                   // Rear Car Call Light #10 Output Bit Number
	BIT3,                   // Rear Car Call Light #11 Output Bit Number
	BIT4,                   // Rear Car Call Light #12 Output Bit Number
	BIT5,                   // Rear Car Call Light #13 Output Bit Number
	BIT6,                   // Rear Car Call Light #14 Output Bit Number
	BIT7,                   // Rear Car Call Light #15 Output Bit Number
	BIT8,                   // Rear Car Call Light #16 Output Bit Number
	BIT1,                   // Rear Car Call Light #17 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Rear Car Call Light #18 Output Bit Number
	BIT2,                   // Rear Car Call Light #19 Output Bit Number
	BIT3,                   // Rear Car Call Light #20 Output Bit Number
	BIT4,                   // Rear Car Call Light #21 Output Bit Number
	BIT5,                   // Rear Car Call Light #22 Output Bit Number
	BIT6,                   // Rear Car Call Light #23 Output Bit Number
	BIT7,                   // Rear Car Call Light #24 Output Bit Number
	BIT8,                   // Rear Car Call Light #25 Output Bit Number
	BIT1,                   // Rear Car Call Light #26 Output Bit Number
	BIT2,                   // Rear Car Call Light #27 Output Bit Number
	BIT3,                   // Rear Car Call Light #28 Output Bit Number
	BIT4,                   // Rear Car Call Light #29 Output Bit Number
	BIT5,                   // Rear Car Call Light #30 Output Bit Number
	BIT6,                   // Rear Car Call Light #31 Output Bit Number
	BIT7,                   // Rear Car Call Light #32 Output Bit Number
	BIT8,                   // Rear Car Call Light #33 Output Bit Number
	BIT1,                   // Rear Car Call Light #34 Output Bit Number
	BIT2,                   // Rear Car Call Light #35 Output Bit Number
	BIT3,                   // Rear Car Call Light #36 Output Bit Number
	BIT4,                   // Rear Car Call Light #37 Output Bit Number
	BIT5,                   // Rear Car Call Light #38 Output Bit Number
	BIT6,                   // Rear Car Call Light #39 Output Bit Number
	BIT7,                   // Rear Car Call Light #40 Output Bit Number
	BIT8,                   // Rear Car Call Light #41 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Rear Car Call Light #42 Output Bit Number
	BIT2,                   // Rear Car Call Light #43 Output Bit Number
	BIT3,                   // Rear Car Call Light #44 Output Bit Number
	BIT4,                   // Rear Car Call Light #45 Output Bit Number
	BIT5,                   // Rear Car Call Light #46 Output Bit Number
	BIT6,                   // Rear Car Call Light #47 Output Bit Number
	BIT7,                   // Rear Car Call Light #48 Output Bit Number
	BIT8,                   // Rear Car Call Light #49 Output Bit Number
	BIT1,                   // Rear Car Call Light #50 Output Bit Number
	BIT2,                   // Rear Car Call Light #51 Output Bit Number
	BIT3,                   // Rear Car Call Light #52 Output Bit Number
	BIT4,                   // Rear Car Call Light #53 Output Bit Number
	BIT5,                   // Rear Car Call Light #54 Output Bit Number
	BIT6,                   // Rear Car Call Light #55 Output Bit Number
	BIT7,                   // Rear Car Call Light #56 Output Bit Number
	BIT8,                   // Rear Car Call Light #57 Output Bit Number
	BIT1,                   // Rear Car Call Light #58 Output Bit Number
	BIT2,                   // Rear Car Call Light #59 Output Bit Number
	BIT3                    // Rear Car Call Light #60 Output Bit Number
	},
{			  // Used with RGB car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Rear Car Call Light #1 Output Bit Number
	BIT2,                   // Rear Car Call Light #2 Output Bit Number
	BIT3,                   // Rear Car Call Light #3 Output Bit Number
	BIT4,                   // Rear Car Call Light #4 Output Bit Number
	BIT5,                   // Rear Car Call Light #5 Output Bit Number
	BIT6,                   // Rear Car Call Light #6 Output Bit Number
	BIT7,                   // Rear Car Call Light #7 Output Bit Number
	BIT8,                   // Rear Car Call Light #8 Output Bit Number
	BIT1,                   // Rear Car Call Light #9 Output Bit Number
	BIT2,                   // Rear Car Call Light #10 Output Bit Number
	BIT5,                   // Rear Car Call Light #11 Output Bit Number
	BIT6,                   // Rear Car Call Light #12 Output Bit Number
	BIT7,                   // Rear Car Call Light #13 Output Bit Number
	BIT8,                   // Rear Car Call Light #14 Output Bit Number
	BIT1,                   // Rear Car Call Light #15 Output Bit Number
	BIT2,                   // Rear Car Call Light #16 Output Bit Number
	BIT3,                   // Rear Car Call Light #17 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Rear Car Call Light #18 Output Bit Number
	BIT2,                   // Rear Car Call Light #19 Output Bit Number
	BIT3,                   // Rear Car Call Light #20 Output Bit Number
	BIT4,                   // Rear Car Call Light #21 Output Bit Number
	BIT5,                   // Rear Car Call Light #22 Output Bit Number
	BIT6,                   // Rear Car Call Light #23 Output Bit Number
	BIT7,                   // Rear Car Call Light #24 Output Bit Number
	BIT8,                   // Rear Car Call Light #25 Output Bit Number
	BIT1,                   // Rear Car Call Light #26 Output Bit Number
	BIT2,                   // Rear Car Call Light #27 Output Bit Number
	BIT3,                   // Rear Car Call Light #28 Output Bit Number
	BIT4,                   // Rear Car Call Light #29 Output Bit Number
	BIT5,                   // Rear Car Call Light #30 Output Bit Number
	BIT6,                   // Rear Car Call Light #31 Output Bit Number
	BIT7,                   // Rear Car Call Light #32 Output Bit Number
	BIT8,                   // Rear Car Call Light #33 Output Bit Number
	BIT1,                   // Rear Car Call Light #34 Output Bit Number
	BIT2,                   // Rear Car Call Light #35 Output Bit Number
	BIT3,                   // Rear Car Call Light #36 Output Bit Number
	BIT4,                   // Rear Car Call Light #37 Output Bit Number
	BIT5,                   // Rear Car Call Light #38 Output Bit Number
	BIT6,                   // Rear Car Call Light #39 Output Bit Number
	BIT7,                   // Rear Car Call Light #40 Output Bit Number
	BIT8,                   // Rear Car Call Light #41 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Rear Car Call Light #42 Output Bit Number
	BIT2,                   // Rear Car Call Light #43 Output Bit Number
	BIT3,                   // Rear Car Call Light #44 Output Bit Number
	BIT4,                   // Rear Car Call Light #45 Output Bit Number
	BIT5,                   // Rear Car Call Light #46 Output Bit Number
	BIT6,                   // Rear Car Call Light #47 Output Bit Number
	BIT7,                   // Rear Car Call Light #48 Output Bit Number
	BIT8,                   // Rear Car Call Light #49 Output Bit Number
	BIT1,                   // Rear Car Call Light #50 Output Bit Number
	BIT2,                   // Rear Car Call Light #51 Output Bit Number
	BIT3,                   // Rear Car Call Light #52 Output Bit Number
	BIT4,                   // Rear Car Call Light #53 Output Bit Number
	BIT5,                   // Rear Car Call Light #54 Output Bit Number
	BIT6,                   // Rear Car Call Light #55 Output Bit Number
	BIT7,                   // Rear Car Call Light #56 Output Bit Number
	BIT8,                   // Rear Car Call Light #57 Output Bit Number
	BIT1,                   // Rear Car Call Light #58 Output Bit Number
	BIT2,                   // Rear Car Call Light #59 Output Bit Number
	BIT3                    // Rear Car Call Light #60 Output Bit Number
	}
};

extern const unsigned char ccrld[2][61] = {
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	51,                     // Rear Car Call Light #1 Output Array Number
	51,                     // Rear Car Call Light #2 Output Array Number
	51,                     // Rear Car Call Light #3 Output Array Number
	51,                     // Rear Car Call Light #4 Output Array Number
	51,                     // Rear Car Call Light #5 Output Array Number
	51,                     // Rear Car Call Light #6 Output Array Number
	51,                     // Rear Car Call Light #7 Output Array Number
	51,                     // Rear Car Call Light #8 Output Array Number
	52,                     // Rear Car Call Light #9 Output Array Number
	52,                     // Rear Car Call Light #10 Output Array Number
	52,                     // Rear Car Call Light #11 Output Array Number
	52,                     // Rear Car Call Light #12 Output Array Number
	52,                     // Rear Car Call Light #13 Output Array Number
	52,                     // Rear Car Call Light #14 Output Array Number
	52,                     // Rear Car Call Light #15 Output Array Number
	52,                     // Rear Car Call Light #16 Output Array Number
	53,                     // Rear Car Call Light #17 Output Array Number
	// ON 2ND Serial Car Board
	57,                     // Rear Car Call Light #18 Output Array Number
	57,                     // Rear Car Call Light #19 Output Array Number
	57,                     // Rear Car Call Light #20 Output Array Number
	57,                     // Rear Car Call Light #21 Output Array Number
	57,                     // Rear Car Call Light #22 Output Array Number
	57,                     // Rear Car Call Light #23 Output Array Number
	57,                     // Rear Car Call Light #24 Output Array Number
	57,                     // Rear Car Call Light #25 Output Array Number
	58,                     // Rear Car Call Light #26 Output Array Number
	58,                     // Rear Car Call Light #27 Output Array Number
	58,                     // Rear Car Call Light #28 Output Array Number
	58,                     // Rear Car Call Light #29 Output Array Number
	58,                     // Rear Car Call Light #30 Output Array Number
	58,                     // Rear Car Call Light #31 Output Array Number
	58,                     // Rear Car Call Light #32 Output Array Number
	58,                     // Rear Car Call Light #33 Output Array Number
	59,                     // Rear Car Call Light #34 Output Array Number
	59,                     // Rear Car Call Light #35 Output Array Number
	59,                     // Rear Car Call Light #36 Output Array Number
	59,                     // Rear Car Call Light #37 Output Array Number
	59,                     // Rear Car Call Light #38 Output Array Number
	59,                     // Rear Car Call Light #39 Output Array Number
	59,                     // Rear Car Call Light #40 Output Array Number
	59,                     // Rear Car Call Light #41 Output Array Number
	// ON 3RD Serial Car Board
	93,                     // Rear Car Call Light #42 Output Array Number
	93,                     // Rear Car Call Light #43 Output Array Number
	93,                     // Rear Car Call Light #44 Output Array Number
	93,                     // Rear Car Call Light #45 Output Array Number
	93,                     // Rear Car Call Light #46 Output Array Number
	93,                     // Rear Car Call Light #47 Output Array Number
	93,                     // Rear Car Call Light #48 Output Array Number
	93,                     // Rear Car Call Light #49 Output Array Number
	94,                     // Rear Car Call Light #50 Output Array Number
	94,                     // Rear Car Call Light #51 Output Array Number
	94,                     // Rear Car Call Light #52 Output Array Number
	94,                     // Rear Car Call Light #53 Output Array Number
	94,                     // Rear Car Call Light #54 Output Array Number
	94,                     // Rear Car Call Light #55 Output Array Number
	94,                     // Rear Car Call Light #56 Output Array Number
	94,                     // Rear Car Call Light #57 Output Array Number
	95,                     // Rear Car Call Light #58 Output Array Number
	95,                     // Rear Car Call Light #59 Output Array Number
	95,                     // Rear Car Call Light #60 Output Array Number
	},
{			  // Used with RGB car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	51,                     // Rear Car Call Light #1 Output Array Number
	51,                     // Rear Car Call Light #2 Output Array Number
	51,                     // Rear Car Call Light #3 Output Array Number
	51,                     // Rear Car Call Light #4 Output Array Number
	51,                     // Rear Car Call Light #5 Output Array Number
	51,                     // Rear Car Call Light #6 Output Array Number
	51,                     // Rear Car Call Light #7 Output Array Number
	51,                     // Rear Car Call Light #8 Output Array Number
	52,                     // Rear Car Call Light #9 Output Array Number
	52,                     // Rear Car Call Light #10 Output Array Number
	52,                     // Rear Car Call Light #11 Output Array Number
	52,                     // Rear Car Call Light #12 Output Array Number
	52,                     // Rear Car Call Light #13 Output Array Number
	52,                     // Rear Car Call Light #14 Output Array Number
	53,                     // Rear Car Call Light #15 Output Array Number
	53,                     // Rear Car Call Light #16 Output Array Number
	53,                     // Rear Car Call Light #17 Output Array Number
	// ON 2ND Serial Car Board
	57,                     // Rear Car Call Light #18 Output Array Number
	57,                     // Rear Car Call Light #19 Output Array Number
	57,                     // Rear Car Call Light #20 Output Array Number
	57,                     // Rear Car Call Light #21 Output Array Number
	57,                     // Rear Car Call Light #22 Output Array Number
	57,                     // Rear Car Call Light #23 Output Array Number
	57,                     // Rear Car Call Light #24 Output Array Number
	57,                     // Rear Car Call Light #25 Output Array Number
	58,                     // Rear Car Call Light #26 Output Array Number
	58,                     // Rear Car Call Light #27 Output Array Number
	58,                     // Rear Car Call Light #28 Output Array Number
	58,                     // Rear Car Call Light #29 Output Array Number
	58,                     // Rear Car Call Light #30 Output Array Number
	58,                     // Rear Car Call Light #31 Output Array Number
	58,                     // Rear Car Call Light #32 Output Array Number
	58,                     // Rear Car Call Light #33 Output Array Number
	59,                     // Rear Car Call Light #34 Output Array Number
	59,                     // Rear Car Call Light #35 Output Array Number
	59,                     // Rear Car Call Light #36 Output Array Number
	59,                     // Rear Car Call Light #37 Output Array Number
	59,                     // Rear Car Call Light #38 Output Array Number
	59,                     // Rear Car Call Light #39 Output Array Number
	59,                     // Rear Car Call Light #40 Output Array Number
	59,                     // Rear Car Call Light #41 Output Array Number
	// ON 3RD Serial Car Board
	93,                     // Rear Car Call Light #42 Output Array Number
	93,                     // Rear Car Call Light #43 Output Array Number
	93,                     // Rear Car Call Light #44 Output Array Number
	93,                     // Rear Car Call Light #45 Output Array Number
	93,                     // Rear Car Call Light #46 Output Array Number
	93,                     // Rear Car Call Light #47 Output Array Number
	93,                     // Rear Car Call Light #48 Output Array Number
	93,                     // Rear Car Call Light #49 Output Array Number
	94,                     // Rear Car Call Light #50 Output Array Number
	94,                     // Rear Car Call Light #51 Output Array Number
	94,                     // Rear Car Call Light #52 Output Array Number
	94,                     // Rear Car Call Light #53 Output Array Number
	94,                     // Rear Car Call Light #54 Output Array Number
	94,                     // Rear Car Call Light #55 Output Array Number
	94,                     // Rear Car Call Light #56 Output Array Number
	94,                     // Rear Car Call Light #57 Output Array Number
	95,                     // Rear Car Call Light #58 Output Array Number
	95,                     // Rear Car Call Light #59 Output Array Number
	95,                     // Rear Car Call Light #60 Output Array Number
	}
};

extern const int16 rm_cc[2][61] = { 
{			  // Used with remote car stations
	0,                      // Not Used
	// 0n 1st Serial Car board
	BRD7A,                  // Car Call #1 Input Board Address
	BRD7A,                  // Car Call #2 Input Board Address
	BRD7A,                  // Car Call #3 Input Board Address
	BRD7A,                  // Car Call #4 Input Board Address
	BRD7A,                  // Car Call #5 Input Board Address
	BRD7A,                  // Car Call #6 Input Board Address
	BRD7A,                  // Car Call #7 Input Board Address
	BRD7A,                  // Car Call #8 Input Board Address
	BRD7B,                  // Car Call #9 Input Board Address
	BRD7B,                  // Car Call #10 Input  Board Address
	BRD7B,                  // Car Call #11 Input  Board Address
	BRD7B,                  // Car Call #12 Input Board Address
	BRD7B,                  // Car Call #13 Input Board Address
	BRD7B,                  // Car Call #14 Input Board Address
	BRD7B,  	            // Car Call #15 Input Board Address
	BRD7B,	              	// Car Call #16 Input Board Address
	BRD7C,                  // Car Call #17 Input Board Address
	BRD7C,                  // Car Call #18 Input Board Address
	BRD7C,                  // Car Call #19 Input Board Address
	BRD7C,                  // Car Call #20 Input Board Address
	// ON 2nd SERIAL CAR BOARD
	BRD33A,                 // Car Call #21 Input Board Address
	BRD33A,                 // Car Call #22 Input Board Address
	BRD33A,                 // Car Call #23 Input Board Address
	BRD33A,                 // Car Call #24 Input Board Address
	BRD33A,                 // Car Call #25 Input Board Address
	BRD33A,                 // Car Call #26 Input Board Address
	BRD33A,                 // Car Call #27 Input Board Address
	BRD33A,                 // Car Call #28 Input Board Address
	BRD33B,                 // Car Call #29 Input Board Address
	BRD33B,                 // Car Call #30 Input Board Address
	BRD33B,                 // Car Call #31 Input Board Address
	BRD33B,                 // Car Call #32 Input Board Address
	BRD33B,                 // Car Call #33 Input Board Address
	BRD33B,                 // Car Call #34 Input Board Address
	BRD33B,                 // Car Call #35 Input Board Address
	BRD33B,                 // Car Call #36 Input Board Address
	BRD33C,                 // Car Call #37 Input Board Address
	BRD33C,                 // Car Call #38 Input Board Address
	BRD33C,                 // Car Call #39 Input Board Address
	BRD33C,                 // Car Call #40 Input Board Address
	BRD33C,                 // Car Call #41 Input Board Address
	BRD33C,                 // Car Call #42 Input Board Address
	BRD33C,                 // Car Call #43 Input Board Address
	BRD33C,                 // Car Call #44 Input Board Address
	// ON 3rd SERIAL CAR BOARD
	BRD35A,                 // Car Call #45 Input Board Address
	BRD35A,                 // Car Call #46 Input Board Address
	BRD35A,                 // Car Call #47 Input Board Address
	BRD35A,                 // Car Call #48 Input Board Address
	BRD35A,                 // Car Call #49 Input Board Address
	BRD35A,                 // Car Call #50 Input Board Address
	BRD35A,                 // Car Call #51 Input Board Address
	BRD35A,                 // Car Call #52 Input Board Address
	BRD35B,                 // Car Call #53 Input Board Address
	BRD35B,                 // Car Call #54 Input Board Address
	BRD35B,                 // Car Call #55 Input Board Address
	BRD35B,                 // Car Call #56 Input Board Address
	BRD35B,                 // Car Call #57 Input Board Address
	BRD35B,                 // Car Call #58 Input Board Address
	BRD35B,                 // Car Call #59 Input Board Address
	BRD35B                  // Car Call #60 Input Board Address
	},
{			  // Used with remote car stations
	0,                      // Not Used
	// 0n 1st Serial Car board
	BRD9A,                  // Car Call #1 Input Board Address
	BRD9A,                  // Car Call #2 Input Board Address
	BRD9A,                  // Car Call #3 Input Board Address
	BRD9A,                  // Car Call #4 Input Board Address
	BRD9A,                  // Car Call #5 Input Board Address
	BRD9A,                  // Car Call #6 Input Board Address
	BRD9A,                  // Car Call #7 Input Board Address
	BRD9A,                  // Car Call #8 Input Board Address
	BRD9B,                  // Car Call #9 Input Board Address
	BRD9B,                  // Car Call #10 Input  Board Address
	BRD9B,                  // Car Call #11 Input  Board Address
	BRD9B,                  // Car Call #12 Input Board Address
	BRD9B,                  // Car Call #13 Input Board Address
	BRD9B,                  // Car Call #14 Input Board Address
	BRD9B,  	            // Car Call #15 Input Board Address
	BRD9B,	              	// Car Call #16 Input Board Address
	BRD9C,                  // Car Call #17 Input Board Address
	BRD9C,                  // Car Call #18 Input Board Address
	BRD9C,                  // Car Call #19 Input Board Address
	BRD9C,                  // Car Call #20 Input Board Address
	// ON 2nd SERIAL CAR BOARD
	BRD11A,                  // Car Call #21 Input Board Address
	BRD11A,                  // Car Call #22 Input Board Address
	BRD11A,                  // Car Call #23 Input Board Address
	BRD11A,                  // Car Call #24 Input Board Address
	BRD11A,                  // Car Call #25 Input Board Address
	BRD11A,                  // Car Call #26 Input Board Address
	BRD11A,                  // Car Call #27 Input Board Address
	BRD11A,                  // Car Call #28 Input Board Address
	BRD11B,                  // Car Call #29 Input Board Address
	BRD11B,                  // Car Call #30 Input Board Address
	BRD11B,                  // Car Call #31 Input Board Address
	BRD11B,                  // Car Call #32 Input Board Address
	BRD11B,                  // Car Call #33 Input Board Address
	BRD11B,                  // Car Call #34 Input Board Address
	BRD11B,                  // Car Call #35 Input Board Address
	BRD11B,                  // Car Call #36 Input Board Address
	BRD11C,                  // Car Call #37 Input Board Address
	BRD11C,                  // Car Call #38 Input Board Address
	BRD11C,                  // Car Call #39 Input Board Address
	BRD11C,                  // Car Call #40 Input Board Address
	BRD11C,                  // Car Call #41 Input Board Address
	BRD11C,                  // Car Call #42 Input Board Address
	BRD11C,                  // Car Call #43 Input Board Address
	BRD11C,                  // Car Call #44 Input Board Address
	// ON 3rd SERIAL CAR BOARD
	BRD37A,                  // Car Call #45 Input Board Address
	BRD37A,                  // Car Call #46 Input Board Address
	BRD37A,                  // Car Call #47 Input Board Address
	BRD37A,                  // Car Call #48 Input Board Address
	BRD37A,                  // Car Call #49 Input Board Address
	BRD37A,                  // Car Call #50 Input Board Address
	BRD37A,                  // Car Call #51 Input Board Address
	BRD37A,                  // Car Call #52 Input Board Address
	BRD37B,                  // Car Call #53 Input Board Address
	BRD37B,                  // Car Call #54 Input Board Address
	BRD37B,                  // Car Call #55 Input Board Address
	BRD37B,                  // Car Call #56 Input Board Address
	BRD37B,                  // Car Call #57 Input Board Address
	BRD37B,                  // Car Call #58 Input Board Address
	BRD37B,                  // Car Call #59 Input Board Address
	BRD37B,                  // Car Call #60 Input Board Address
	}
};
extern const unsigned char rm_ccb[2][61] = 	{
{			  // Used with REMOTE car call
	0,                      // Not Used
	// ON 1st serial car board
	BIT1,                   // Car Call #1 Input Bit Number
	BIT2,                   // Car Call #2 Input Bit Number
	BIT3,                   // Car Call #3 Input Bit Number
	BIT4,                   // Car Call #4 Input Bit Number
	BIT5,                   // Car Call #5 Input Bit Number
	BIT6,                   // Car Call #6 Input Bit Number
	BIT7,                   // Car Call #7 Input Bit Number
	BIT8,                   // Car Call #8 Input Bit Number
	BIT1,                   // Car Call #9 Input Bit Number
	BIT2,                   // Car Call #10 Input Bit Number
	BIT3,                   // Car Call #11 Input Bit Number
	BIT4,                   // Car Call #12 Input Bit Number
	BIT5,                   // Car Call #13 Input Bit Number
	BIT6,                   // Car Call #14 Input Bit Number
	BIT7,                   // Car Call #15 Input Bit Number
	BIT8,                   // Car Call #16 Input Bit Number
	BIT1,                   // Car Call #17 Input Bit Number
	BIT2,                   // Car Call #18 Input Bit Number
	BIT3,                   // Car Call #19 Input Bit Number
	BIT4,                   // Car Call #20 Input Bit Number
	// ON 2nd serial car board
	BIT1,                   // Car Call #21 Input Bit Number
	BIT2,                   // Car Call #22 Input Bit Number
	BIT3,                   // Car Call #23 Input Bit Number
	BIT4,                   // Car Call #24 Input Bit Number
	BIT5,                   // Car Call #25 Input Bit Number
	BIT6,                   // Car Call #26 Input Bit Number
	BIT7,                   // Car Call #27 Input Bit Number
	BIT8,                   // Car Call #28 Input Bit Number
	BIT1,                   // Car Call #29 Input Bit Number
	BIT2,                   // Car Call #30 Input Bit Number
	BIT3,                   // Car Call #31 Input Bit Number
	BIT4,                   // Car Call #32 Input Bit Number
	BIT5,                   // Car Call #33 Input Bit Number
	BIT6,                   // Car Call #34 Input Bit Number
	BIT7,                   // Car Call #35 Input Bit Number
	BIT8,                   // Car Call #36 Input Bit Number
	BIT1,                   // Car Call #37 Input Bit Number
	BIT2,                   // Car Call #38 Input Bit Number
	BIT3,                   // Car Call #39 Input Bit Number
	BIT4,                   // Car Call #40 Input Bit Number
	BIT5,                   // Car Call #41 Input Bit Number
	BIT6,                   // Car Call #42 Input Bit Number
	BIT7,                   // Car Call #43 Input Bit Number
	BIT8,                   // Car Call #44 Input Bit Number
	// ON 3rd serial car board
	BIT1,                   // Car Call #45 Input Bit Number
	BIT2,                   // Car Call #46 Input Bit Number
	BIT3,                   // Car Call #47 Input Bit Number
	BIT4,                   // Car Call #48 Input Bit Number
	BIT5,                   // Car Call #49 Input Bit Number
	BIT6,                   // Car Call #50 Input Bit Number
	BIT7,                   // Car Call #51 Input Bit Number
	BIT8,                   // Car Call #52 Input Bit Number
	BIT1,                   // Car Call #53 Input Bit Number
	BIT2,                   // Car Call #54 Input Bit Number
	BIT3,                   // Car Call #55 Input Bit Number
	BIT4,                   // Car Call #56 Input Bit Number
	BIT5,                   // Car Call #57 Input Bit Number
	BIT6,                   // Car Call #58 Input Bit Number
	BIT7,                   // Car Call #59 Input Bit Number
	BIT8                    // Car Call #60 Input Bit Number
	},
{			  // Used with REMOTE car call
	0,                      // Not Used
	// ON 1st serial car board
	BIT1,                   // Car Call #1 Input Bit Number
	BIT2,                   // Car Call #2 Input Bit Number
	BIT3,                   // Car Call #3 Input Bit Number
	BIT4,                   // Car Call #4 Input Bit Number
	BIT5,                   // Car Call #5 Input Bit Number
	BIT6,                   // Car Call #6 Input Bit Number
	BIT7,                   // Car Call #7 Input Bit Number
	BIT8,                   // Car Call #8 Input Bit Number
	BIT1,                   // Car Call #9 Input Bit Number
	BIT2,                   // Car Call #10 Input Bit Number
	BIT3,                   // Car Call #11 Input Bit Number
	BIT4,                   // Car Call #12 Input Bit Number
	BIT5,                   // Car Call #13 Input Bit Number
	BIT6,                   // Car Call #14 Input Bit Number
	BIT7,                   // Car Call #15 Input Bit Number
	BIT8,                   // Car Call #16 Input Bit Number
	BIT1,                   // Car Call #17 Input Bit Number
	BIT2,                   // Car Call #18 Input Bit Number
	BIT3,                   // Car Call #19 Input Bit Number
	BIT4,                   // Car Call #20 Input Bit Number
	// ON 2nd serial car board
	BIT1,                   // Car Call #21 Input Bit Number
	BIT2,                   // Car Call #22 Input Bit Number
	BIT3,                   // Car Call #23 Input Bit Number
	BIT4,                   // Car Call #24 Input Bit Number
	BIT5,                   // Car Call #25 Input Bit Number
	BIT6,                   // Car Call #26 Input Bit Number
	BIT7,                   // Car Call #27 Input Bit Number
	BIT8,                   // Car Call #28 Input Bit Number
	BIT1,                   // Car Call #29 Input Bit Number
	BIT2,                   // Car Call #30 Input Bit Number
	BIT3,                   // Car Call #31 Input Bit Number
	BIT4,                   // Car Call #32 Input Bit Number
	BIT5,                   // Car Call #33 Input Bit Number
	BIT6,                   // Car Call #34 Input Bit Number
	BIT7,                   // Car Call #35 Input Bit Number
	BIT8,                   // Car Call #36 Input Bit Number
	BIT1,                   // Car Call #37 Input Bit Number
	BIT2,                   // Car Call #38 Input Bit Number
	BIT3,                    // Car Call #39 Input Bit Number
	BIT4,                   // Car Call #40 Input Bit Number
	BIT5,                   // Car Call #41 Input Bit Number
	BIT6,                   // Car Call #42 Input Bit Number
	BIT7,                   // Car Call #43 Input Bit Number
	BIT8,                   // Car Call #44 Input Bit Number
	// ON 3rd serial car board
	BIT1,                   // Car Call #45 Input Bit Number
	BIT2,                   // Car Call #46 Input Bit Number
	BIT3,                   // Car Call #47 Input Bit Number
	BIT4,                   // Car Call #48 Input Bit Number
	BIT5,                   // Car Call #49 Input Bit Number
	BIT6,                   // Car Call #50 Input Bit Number
	BIT7,                   // Car Call #51 Input Bit Number
	BIT8,                   // Car Call #52 Input Bit Number
	BIT1,                   // Car Call #53 Input Bit Number
	BIT2,                   // Car Call #54 Input Bit Number
	BIT3,                   // Car Call #55 Input Bit Number
	BIT4,                   // Car Call #56 Input Bit Number
	BIT5,                   // Car Call #57 Input Bit Number
	BIT6,                   // Car Call #58 Input Bit Number
	BIT7,                   // Car Call #59 Input Bit Number
	BIT8                    // Car Call #60 Input Bit Number
	}
};

extern const unsigned char rm_ccd[2][61] = {
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st serial car board
	18,                     // Car Call #1 Input Array Number
	18,                     // Car Call #2 Input Array Number
	18,                     // Car Call #3 Input Array Number
	18,                     // Car Call #4 Input Array Number
	18,                     // Car Call #5 Input Array Number
	18,                     // Car Call #6 Input Array Number
	18,                     // Car Call #7 Input Array Number
	18,                     // Car Call #8 Input Array Number
	19,                     // Car Call #9 Input Array Number
	19,                     // Car Call #10 Input Array Number
	19,                     // Car Call #11 Input Array Number
	19,                     // Car Call #12 Input Array Number
	19,                     // Car Call #13 Input Array Number
	19,                     // Car Call #14 Input Array Number
	19,                     // Car Call #15 Input Array Number
	19,                     // Car Call #16 Input Array Number
	20,                     // Car Call #17 Input Array Number
	20,                     // Car Call #18 Input Array Number
	20,                     // Car Call #19 Input Array Number
	20,                     // Car Call #20 Input Array Number

	// ON 2nd serial car board
	96,                     // Car Call #21 Input Array Number
	96,                     // Car Call #22 Input Array Number
	96,                     // Car Call #23 Input Array Number
	96,                     // Car Call #24 Input Array Number
	96,                     // Car Call #25 Input Array Number
	96,                     // Car Call #26 Input Array Number
	96,                     // Car Call #27 Input Array Number
	96,                     // Car Call #28 Input Array Number
	97,                     // Car Call #29 Input Array Number
	97,                     // Car Call #30 Input Array Number
	97,                     // Car Call #31 Input Array Number
	97,                     // Car Call #32 Input Array Number
	97,                     // Car Call #33 Input Array Number
	97,                     // Car Call #34 Input Array Number
	97,                     // Car Call #35 Input Array Number
	97,                     // Car Call #36 Input Array Number
	98,                     // Car Call #37 Input Array Number
	98,                     // Car Call #38 Input Array Number
	98,                     // Car Call #39 Input Array Number
	98,                     // Car Call #40 Input Array Number
	98,                     // Car Call #41 Input Array Number
	98,                     // Car Call #42 Input Array Number
	98,                     // Car Call #43 Input Array Number
	98,                     // Car Call #44 Input Array Number
	// ON 3rd serial car board
	102,                    // Car Call #45 Input Array Number
	102,                    // Car Call #46 Input Array Number
	102,                    // Car Call #47 Input Array Number
	102,                    // Car Call #48 Input Array Number
	102,                    // Car Call #49 Input Array Number
	102,                    // Car Call #50 Input Array Number
	102,                    // Car Call #51 Input Array Number
	102,                    // Car Call #52 Input Array Number
	103,                    // Car Call #53 Input Array Number
	103,                    // Car Call #54 Input Array Number
	103,                    // Car Call #55 Input Array Number
	103,                    // Car Call #56 Input Array Number
	103,                    // Car Call #57 Input Array Number
	103,                    // Car Call #58 Input Array Number
	103,                    // Car Call #59 Input Array Number
	103,                    // Car Call #60 Input Array Number
	},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st serial car board
	24,                     // Car Call #1 Input Array Number
	24,                     // Car Call #2 Input Array Number
	24,                     // Car Call #3 Input Array Number
	24,                     // Car Call #4 Input Array Number
	24,                     // Car Call #5 Input Array Number
	24,                     // Car Call #6 Input Array Number
	24,                     // Car Call #7 Input Array Number
	24,                     // Car Call #8 Input Array Number
	25,                     // Car Call #9 Input Array Number
	25,                     // Car Call #10 Input Array Number
	25,                     // Car Call #11 Input Array Number
	25,                     // Car Call #12 Input Array Number
	25,                     // Car Call #13 Input Array Number
	25,                     // Car Call #14 Input Array Number
	25,                     // Car Call #15 Input Array Number
	25,                     // Car Call #16 Input Array Number
	26,                     // Car Call #17 Input Array Number
	26,                     // Car Call #18 Input Array Number
	26,                     // Car Call #19 Input Array Number
	26,                     // Car Call #20 Input Array Number

	// ON 2nd serial car board
	30,                     // Car Call #21 Input Array Number
	30,                     // Car Call #22 Input Array Number
	30,                     // Car Call #23 Input Array Number
	30,                     // Car Call #24 Input Array Number
	30,                     // Car Call #25 Input Array Number
	30,                     // Car Call #26 Input Array Number
	30,                     // Car Call #27 Input Array Number
	30,                     // Car Call #28 Input Array Number
	31,                     // Car Call #29 Input Array Number
	31,                     // Car Call #30 Input Array Number
	31,                     // Car Call #31 Input Array Number
	31,                     // Car Call #32 Input Array Number
	31,                     // Car Call #33 Input Array Number
	31,                     // Car Call #34 Input Array Number
	31,                     // Car Call #35 Input Array Number
	31,                     // Car Call #36 Input Array Number
	32,                     // Car Call #37 Input Array Number
	32,                     // Car Call #38 Input Array Number
	32,                     // Car Call #39 Input Array Number
	32,                     // Car Call #40 Input Array Number
	32,                     // Car Call #41 Input Array Number
	32,                     // Car Call #42 Input Array Number
	32,                     // Car Call #43 Input Array Number
	32,                     // Car Call #44 Input Array Number
	// ON 3rd serial car board
	108,                    // Car Call #45 Input Array Number
	108,                    // Car Call #46 Input Array Number
	108,                    // Car Call #47 Input Array Number
	108,                    // Car Call #48 Input Array Number
	108,                    // Car Call #49 Input Array Number
	108,                    // Car Call #50 Input Array Number
	108,                    // Car Call #51 Input Array Number
	108,                    // Car Call #52 Input Array Number
	109,                    // Car Call #53 Input Array Number
	109,                    // Car Call #54 Input Array Number
	109,                    // Car Call #55 Input Array Number
	109,                    // Car Call #56 Input Array Number
	109,                    // Car Call #57 Input Array Number
	109,                    // Car Call #58 Input Array Number
	109,                    // Car Call #59 Input Array Number
	109,                    // Car Call #60 Input Array Number
	}
};

extern const int16 rm_ccl[2][61] = {
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD8A,                 // Car Call Light #1 Output Board Address
	BRD8A,                 // Car Call Light #2 Output Board Address
	BRD8A,                 // Car Call Light #3 Output Board Address
	BRD8A,                 // Car Call Light #4 Output Board Address
	BRD8A,                 // Car Call Light #5 Output Board Address
	BRD8A,                 // Car Call Light #6 Output Board Address
	BRD8A,                 // Car Call Light #7 Output Board Address
	BRD8A,                 // Car Call Light #8 Output Board Address
	BRD8B,                 // Car Call Light #9 Output Board Address
	BRD8B,                 // Car Call Light #10 Output Board Address
	BRD8B,                 // Car Call Light #11 Output Board Address
	BRD8B,                 // Car Call Light #12 Output Board Address
	BRD8B,                 // Car Call Light #13 Output Board Address
	BRD8B,                 // Car Call Light #14 Output Board Address
	BRD8B,                 // Car Call Light #15 Output Board Address
	BRD8B,                 // Car Call Light #16 Output Board Address
	BRD8C,                 // Car Call Light #17 Output Board Address
	BRD8C,                 // Car Call Light #18 Output Board Address
	BRD8C,                 // Car Call Light #19 Output Board Address
	BRD8C,                 // Car Call Light #20 Output Board Address

	// ON 2nd Serial Car Board
	BRD34A,                // Car Call Light #21 Output Board Address
	BRD34A,                // Car Call Light #22 Output Board Address
	BRD34A,                // Car Call Light #23 Output Board Address
	BRD34A,                // Car Call Light #24 Output Board Address
	BRD34A,                // Car Call Light #25 Output Board Address
	BRD34A,                // Car Call Light #26 Output Board Address
	BRD34A,                // Car Call Light #27 Output Board Address
	BRD34A,                // Car Call Light #28 Output Board Address
	BRD34B,                // Car Call Light #29 Output Board Address
	BRD34B,                // Car Call Light #30 Output Board Address
	BRD34B,                // Car Call Light #31 Output Board Address
	BRD34B,                // Car Call Light #32 Output Board Address
	BRD34B,                // Car Call Light #33 Output Board Address
	BRD34B,                // Car Call Light #34 Output Board Address
	BRD34B,                // Car Call Light #35 Output Board Address
	BRD34B,                // Car Call Light #36 Output Board Address
	BRD34C,                // Car Call Light #37 Output Board Address
	BRD34C,                // Car Call Light #38 Output Board Address
	BRD34C,                // Car Call Light #39 Output Board Address
	BRD34C,                // Car Call Light #40 Output Board Address
	BRD34C,                // Car Call Light #41 Output Board Address
	BRD34C,                // Car Call Light #42 Output Board Address
	BRD34C,                // Car Call Light #43 Output Board Address
	BRD34C,                // Car Call Light #44 Output Board Address
	// ON 3rd Serial Car Board
	BRD36A,                // Car Call Light #45 Output Board Address
	BRD36A,                // Car Call Light #46 Output Board Address
	BRD36A,                // Car Call Light #47 Output Board Address
	BRD36A,                // Car Call Light #48 Output Board Address
	BRD36A,                // Car Call Light #49 Output Board Address
	BRD36A,                // Car Call Light #50 Output Board Address
	BRD36A,                // Car Call Light #51 Output Board Address
	BRD36A,                // Car Call Light #52 Output Board Address
	BRD36B,                // Car Call Light #53 Output Board Address
	BRD36B,                // Car Call Light #54 Output Board Address
	BRD36B,                // Car Call Light #55 Output Board Address
	BRD36B,                // Car Call Light #56 Output Board Address
	BRD36B,                // Car Call Light #57 Output Board Address
	BRD36B,                // Car Call Light #58 Output Board Address
	BRD36B,                // Car Call Light #59 Output Board Address
	BRD36B,                // Car Call Light #60 Output Board Address
	},
{
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD10A,                 // Car Call Light #1 Output Board Address
	BRD10A,                 // Car Call Light #2 Output Board Address
	BRD10A,                 // Car Call Light #3 Output Board Address
	BRD10A,                 // Car Call Light #4 Output Board Address
	BRD10A,                 // Car Call Light #5 Output Board Address
	BRD10A,                 // Car Call Light #6 Output Board Address
	BRD10A,                 // Car Call Light #7 Output Board Address
	BRD10A,                 // Car Call Light #8 Output Board Address
	BRD10B,                 // Car Call Light #9 Output Board Address
	BRD10B,                 // Car Call Light #10 Output Board Address
	BRD10B,                 // Car Call Light #11 Output Board Address
	BRD10B,                 // Car Call Light #12 Output Board Address
	BRD10B,                 // Car Call Light #13 Output Board Address
	BRD10B,                 // Car Call Light #14 Output Board Address
	BRD10B,                 // Car Call Light #15 Output Board Address
	BRD10B,                 // Car Call Light #16 Output Board Address
	BRD10C,                 // Car Call Light #17 Output Board Address
	BRD10C,                 // Car Call Light #18 Output Board Address
	BRD10C,                 // Car Call Light #19 Output Board Address
	BRD10C,                 // Car Call Light #20 Output Board Address

	// ON 2nd Serial Car Board
	BRD12A,                 // Car Call Light #21 Output Board Address
	BRD12A,                 // Car Call Light #22 Output Board Address
	BRD12A,                 // Car Call Light #23 Output Board Address
	BRD12A,                 // Car Call Light #24 Output Board Address
	BRD12A,                 // Car Call Light #25 Output Board Address
	BRD12A,                 // Car Call Light #26 Output Board Address
	BRD12A,                 // Car Call Light #27 Output Board Address
	BRD12A,                 // Car Call Light #28 Output Board Address
	BRD12B,                 // Car Call Light #29 Output Board Address
	BRD12B,                 // Car Call Light #30 Output Board Address
	BRD12B,                 // Car Call Light #31 Output Board Address
	BRD12B,                 // Car Call Light #22 Output Board Address
	BRD12B,                 // Car Call Light #33 Output Board Address
	BRD12B,                 // Car Call Light #34 Output Board Address
	BRD12B,                 // Car Call Light #35 Output Board Address
	BRD12B,                 // Car Call Light #36 Output Board Address
	BRD12C,                 // Car Call Light #37 Output Board Address
	BRD12C,                 // Car Call Light #38 Output Board Address
	BRD12C,                 // Car Call Light #39 Output Board Address
	BRD12C,                 // Car Call Light #40 Output Board Address
	BRD12C,                 // Car Call Light #41 Output Board Address
	BRD12C,                 // Car Call Light #42 Output Board Address
	BRD12C,                 // Car Call Light #43 Output Board Address
	BRD12C,                 // Car Call Light #44 Output Board Address
	// ON 2nd Serial Car Board
	BRD38A,                 // Car Call Light #45 Output Board Address
	BRD38A,                 // Car Call Light #46 Output Board Address
	BRD38A,                 // Car Call Light #47 Output Board Address
	BRD38A,                 // Car Call Light #48 Output Board Address
	BRD38A,                 // Car Call Light #49 Output Board Address
	BRD38A,                 // Car Call Light #50 Output Board Address
	BRD38A,                 // Car Call Light #51 Output Board Address
	BRD38A,                 // Car Call Light #52 Output Board Address
	BRD38B,                 // Car Call Light #53 Output Board Address
	BRD38B,                 // Car Call Light #54 Output Board Address
	BRD38B,                 // Car Call Light #55 Output Board Address
	BRD38B,                 // Car Call Light #56 Output Board Address
	BRD38B,                 // Car Call Light #57 Output Board Address
	BRD38B,                 // Car Call Light #58 Output Board Address
	BRD38B,                 // Car Call Light #59 Output Board Address
	BRD38B                  // Car Call Light #60 Output Board Address
	}
};
extern const unsigned char rm_cclb[2][61] =  {
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Car Call Light #1 Output Bit Number
	BIT2,                   // Car Call Light #2 Output Bit Number
	BIT3,                   // Car Call Light #3 Output Bit Number
	BIT4,                   // Car Call Light #4 Output Bit Number
	BIT5,                   // Car Call Light #5 Output Bit Number
	BIT6,                   // Car Call Light #6 Output Bit Number
	BIT7,                   // Car Call Light #7 Output Bit Number
	BIT8,                   // Car Call Light #8 Output Bit Number
	BIT1,                   // Car Call Light #9 Output Bit Number
	BIT2,                   // Car Call Light #10 Output Bit Number
	BIT3,                   // Car Call Light #11 Output Bit Number
	BIT4,                   // Car Call Light #12 Output Bit Number
	BIT5,                   // Car Call Light #13 Output Bit Number
	BIT6,                   // Car Call Light #14 Output Bit Number
	BIT7,                   // Car Call Light #15 Output Bit Number
	BIT8,                   // Car Call Light #16 Output Bit Number
	BIT1,                   // Car Call Light #17 Output Bit Number
	BIT2,                   // Car Call Light #18 Output Bit Number
	BIT3,                   // Car Call Light #19 Output Bit Number
	BIT4,                   // Car Call Light #20 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Car Call Light #21 Output Bit Number
	BIT2,                   // Car Call Light #22 Output Bit Number
	BIT3,                   // Car Call Light #23 Output Bit Number
	BIT4,                   // Car Call Light #24 Output Bit Number
	BIT5,                   // Car Call Light #25 Output Bit Number
	BIT6,                   // Car Call Light #26 Output Bit Number
	BIT7,                   // Car Call Light #27 Output Bit Number
	BIT8,                   // Car Call Light #28 Output Bit Number
	BIT1,                   // Car Call Light #29 Output Bit Number
	BIT2,                   // Car Call Light #30 Output Bit Number
	BIT3,                   // Car Call Light #31 Output Bit Number
	BIT4,                   // Car Call Light #32 Output Bit Number
	BIT5,                   // Car Call Light #33 Output Bit Number
	BIT6,                   // Car Call Light #34 Output Bit Number
	BIT7,                   // Car Call Light #35 Output Bit Number
	BIT8,                   // Car Call Light #36 Output Bit Number
	BIT1,                   // Car Call Light #37 Output Bit Number
	BIT2,                   // Car Call Light #38 Output Bit Number
	BIT3,                   // Car Call Light #39 Output Bit Number
	BIT4,                   // Car Call Light #40 Output Bit Number
	BIT5,                   // Car Call Light #41 Output Bit Number
	BIT6,                   // Car Call Light #42 Output Bit Number
	BIT7,                   // Car Call Light #43 Output Bit Number
	BIT8,                   // Car Call Light #44 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Car Call Light #45 Output Bit Number
	BIT2,                   // Car Call Light #46 Output Bit Number
	BIT3,                   // Car Call Light #47 Output Bit Number
	BIT4,                   // Car Call Light #48 Output Bit Number
	BIT5,                   // Car Call Light #49 Output Bit Number
	BIT6,                   // Car Call Light #50 Output Bit Number
	BIT7,                   // Car Call Light #51 Output Bit Number
	BIT8,                   // Car Call Light #52 Output Bit Number
	BIT1,                   // Car Call Light #53 Output Bit Number
	BIT2,                   // Car Call Light #54 Output Bit Number
	BIT3,                   // Car Call Light #55 Output Bit Number
	BIT4,                   // Car Call Light #56 Output Bit Number
	BIT5,                   // Car Call Light #57 Output Bit Number
	BIT6,                   // Car Call Light #58 Output Bit Number
	BIT7,                   // Car Call Light #59 Output Bit Number
	BIT8,                   // Car Call Light #60 Output Bit Number
	},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Car Call Light #1 Output Bit Number
	BIT2,                   // Car Call Light #2 Output Bit Number
	BIT3,                   // Car Call Light #3 Output Bit Number
	BIT4,                   // Car Call Light #4 Output Bit Number
	BIT5,                   // Car Call Light #5 Output Bit Number
	BIT6,                   // Car Call Light #6 Output Bit Number
	BIT7,                   // Car Call Light #7 Output Bit Number
	BIT8,                   // Car Call Light #8 Output Bit Number
	BIT1,                   // Car Call Light #9 Output Bit Number
	BIT2,                   // Car Call Light #10 Output Bit Number
	BIT3,                   // Car Call Light #11 Output Bit Number
	BIT4,                   // Car Call Light #12 Output Bit Number
	BIT5,                   // Car Call Light #13 Output Bit Number
	BIT6,                   // Car Call Light #14 Output Bit Number
	BIT7,                   // Car Call Light #15 Output Bit Number
	BIT8,                   // Car Call Light #16 Output Bit Number
	BIT1,                   // Car Call Light #17 Output Bit Number
	BIT2,                   // Car Call Light #18 Output Bit Number
	BIT3,                   // Car Call Light #19 Output Bit Number
	BIT4,                   // Car Call Light #20 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Car Call Light #21 Output Bit Number
	BIT2,                   // Car Call Light #22 Output Bit Number
	BIT3,                   // Car Call Light #23 Output Bit Number
	BIT4,                   // Car Call Light #24 Output Bit Number
	BIT5,                   // Car Call Light #25 Output Bit Number
	BIT6,                   // Car Call Light #26 Output Bit Number
	BIT7,                   // Car Call Light #27 Output Bit Number
	BIT8,                   // Car Call Light #28 Output Bit Number
	BIT1,                   // Car Call Light #29 Output Bit Number
	BIT2,                   // Car Call Light #30 Output Bit Number
	BIT3,                   // Car Call Light #31 Output Bit Number
	BIT4,                   // Car Call Light #32 Output Bit Number
	BIT5,                   // Car Call Light #33 Output Bit Number
	BIT6,                   // Car Call Light #34 Output Bit Number
	BIT7,                   // Car Call Light #35 Output Bit Number
	BIT8,                   // Car Call Light #36 Output Bit Number
	BIT1,                   // Car Call Light #37 Output Bit Number
	BIT2,                   // Car Call Light #38 Output Bit Number
	BIT3,                   // Car Call Light #39 Output Bit Number
	BIT4,                   // Car Call Light #40 Output Bit Number
	BIT5,                   // Car Call Light #41 Output Bit Number
	BIT6,                   // Car Call Light #42 Output Bit Number
	BIT7,                   // Car Call Light #43 Output Bit Number
	BIT8,                   // Car Call Light #44 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Car Call Light #45 Output Bit Number
	BIT2,                   // Car Call Light #46 Output Bit Number
	BIT3,                   // Car Call Light #47 Output Bit Number
	BIT4,                   // Car Call Light #48 Output Bit Number
	BIT5,                   // Car Call Light #49 Output Bit Number
	BIT6,                   // Car Call Light #50 Output Bit Number
	BIT7,                   // Car Call Light #51 Output Bit Number
	BIT8,                   // Car Call Light #52 Output Bit Number
	BIT1,                   // Car Call Light #53 Output Bit Number
	BIT2,                   // Car Call Light #54 Output Bit Number
	BIT3,                   // Car Call Light #55 Output Bit Number
	BIT4,                   // Car Call Light #56 Output Bit Number
	BIT5,                   // Car Call Light #57 Output Bit Number
	BIT6,                   // Car Call Light #58 Output Bit Number
	BIT7,                   // Car Call Light #59 Output Bit Number
	BIT8,                   // Car Call Light #60 Output Bit Number
	}
};
extern const unsigned char rm_ccld[2][61] =  {
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	21,                     // Car Call Light #1 Output Array Number
	21,                     // Car Call Light #2 Output Array Number
	21,                     // Car Call Light #3 Output Array Number
	21,                     // Car Call Light #4 Output Array Number
	21,                     // Car Call Light #5 Output Array Number
	21,                     // Car Call Light #6 Output Array Number
	21,                     // Car Call Light #7 Output Array Number
	21,                     // Car Call Light #8 Output Array Number
	22,                     // Car Call Light #9 Output Array Number
	22,                     // Car Call Light #10 Output Array Number
	22,                     // Car Call Light #11 Output Array Number
	22,                     // Car Call Light #12 Output Array Number
	22,                     // Car Call Light #13 Output Array Number
	22,                     // Car Call Light #14 Output Array Number
	22,                     // Car Call Light #15 Output Array Number
	22,                     // Car Call Light #16 Output Array Number
	23,                     // Car Call Light #17 Output Array Number
	23,                     // Car Call Light #18 Output Array Number
	23,                     // Car Call Light #19 Output Array Number
	23,                     // Car Call Light #20 Output Array Number
	// ON 2nd Serial Car Board
	99,                     // Car Call Light #21 Output Array Number
	99,                     // Car Call Light #22 Output Array Number
	99,                     // Car Call Light #23 Output Array Number
	99,                     // Car Call Light #24 Output Array Number
	99,                     // Car Call Light #25 Output Array Number
	99,                     // Car Call Light #26 Output Array Number
	99,                     // Car Call Light #27 Output Array Number
	99,                     // Car Call Light #28 Output Array Number
	100,                    // Car Call Light #29 Output Array Number
	100,                    // Car Call Light #30 Output Array Number
	100,                    // Car Call Light #31 Output Array Number
	100,                    // Car Call Light #32 Output Array Number
	100,                    // Car Call Light #33 Output Array Number
	100,                    // Car Call Light #34 Output Array Number
	100,                    // Car Call Light #35 Output Array Number
	100,                    // Car Call Light #36 Output Array Number
	101,                    // Car Call Light #37 Output Array Number
	101,                    // Car Call Light #38 Output Array Number
	101,                    // Car Call Light #39 Output Array Number
	101,                    // Car Call Light #40 Output Array Number
	101,                    // Car Call Light #41 Output Array Number
	101,                    // Car Call Light #42 Output Array Number
	101,                    // Car Call Light #43 Output Array Number
	101,                    // Car Call Light #44 Output Array Number
	// ON 3rd Serial Car Board
	105,                    // Car Call Light #45 Output Array Number
	105,                    // Car Call Light #46 Output Array Number
	105,                    // Car Call Light #47 Output Array Number
	105,                    // Car Call Light #48 Output Array Number
	105,                    // Car Call Light #49 Output Array Number
	105,                    // Car Call Light #50 Output Array Number
	105,                    // Car Call Light #51 Output Array Number
	105,                    // Car Call Light #52 Output Array Number
	106,                    // Car Call Light #53 Output Array Number
	106,                    // Car Call Light #54 Output Array Number
	106,                    // Car Call Light #55 Output Array Number
	106,                    // Car Call Light #56 Output Array Number
	106,                    // Car Call Light #57 Output Array Number
	106,                    // Car Call Light #58 Output Array Number
	106,                    // Car Call Light #59 Output Array Number
	106,                    // Car Call Light #60 Output Array Number
	},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	27,                     // Car Call Light #1 Output Array Number
	27,                     // Car Call Light #2 Output Array Number
	27,                     // Car Call Light #3 Output Array Number
	27,                     // Car Call Light #4 Output Array Number
	27,                     // Car Call Light #5 Output Array Number
	27,                     // Car Call Light #6 Output Array Number
	27,                     // Car Call Light #7 Output Array Number
	27,                     // Car Call Light #8 Output Array Number
	28,                     // Car Call Light #9 Output Array Number
	28,                     // Car Call Light #10 Output Array Number
	28,                     // Car Call Light #11 Output Array Number
	28,                     // Car Call Light #12 Output Array Number
	28,                     // Car Call Light #13 Output Array Number
	28,                     // Car Call Light #14 Output Array Number
	28,                     // Car Call Light #15 Output Array Number
	28,                     // Car Call Light #16 Output Array Number
	29,                     // Car Call Light #17 Output Array Number
	29,                     // Car Call Light #18 Output Array Number
	29,                     // Car Call Light #19 Output Array Number
	29,                     // Car Call Light #20 Output Array Number
	// ON 2nd Serial Car Board
	33,                     // Car Call Light #21 Output Array Number
	33,                     // Car Call Light #22 Output Array Number
	33,                     // Car Call Light #23 Output Array Number
	33,                     // Car Call Light #24 Output Array Number
	33,                     // Car Call Light #25 Output Array Number
	33,                     // Car Call Light #26 Output Array Number
	33,                     // Car Call Light #27 Output Array Number
	33,                     // Car Call Light #28 Output Array Number
	34,                     // Car Call Light #29 Output Array Number
	34,                     // Car Call Light #30 Output Array Number
	34,                     // Car Call Light #31 Output Array Number
	34,                     // Car Call Light #32 Output Array Number
	34,                     // Car Call Light #33 Output Array Number
	34,                     // Car Call Light #34 Output Array Number
	34,                     // Car Call Light #35 Output Array Number
	34,                     // Car Call Light #36 Output Array Number
	35,                     // Car Call Light #37 Output Array Number
	35,                     // Car Call Light #38 Output Array Number
	35,                     // Car Call Light #39 Output Array Number
	35,                     // Car Call Light #40 Output Array Number
	35,                     // Car Call Light #41 Output Array Number
	35,                     // Car Call Light #42 Output Array Number
	35,                     // Car Call Light #43 Output Array Number
	35,                     // Car Call Light #44 Output Array Number
	// ON 3rd Serial Car Board
	111,                    // Car Call Light #45 Output Array Number
	111,                    // Car Call Light #46 Output Array Number
	111,                    // Car Call Light #47 Output Array Number
	111,                    // Car Call Light #48 Output Array Number
	111,                    // Car Call Light #49 Output Array Number
	111,                    // Car Call Light #50 Output Array Number
	111,                    // Car Call Light #51 Output Array Number
	111,                    // Car Call Light #52 Output Array Number
	112,                    // Car Call Light #53 Output Array Number
	112,                    // Car Call Light #54 Output Array Number
	112,                    // Car Call Light #55 Output Array Number
	112,                    // Car Call Light #56 Output Array Number
	112,                    // Car Call Light #57 Output Array Number
	112,                    // Car Call Light #58 Output Array Number
	112,                    // Car Call Light #59 Output Array Number
	112,                    // Car Call Light #60 Output Array Number
	}
};

extern const int16 rm_ccr[2][61] = { 	
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BRD11A,                 // Rear Car Call #1 Input Board Address
	BRD11A,                 // Rear Car Call #2 Input Board Address
	BRD11A,                 // Rear Car Call #3 Input Board Address
	BRD11A,                 // Rear Car Call #4 Input Board Address
	BRD11A,                 // Rear Car Call #5 Input Board Address
	BRD11A,                 // Rear Car Call #6 Input Board Address
	BRD11A,                 // Rear Car Call #7 Input Board Address
	BRD11A,                 // Rear Car Call #8 Input Board Address
	BRD11B,                 // Rear Car Call #9 Input Board Address
	BRD11B,                 // Rear Car Call #10 Input Board Address
	BRD11B,                 // Rear Car Call #11 Input  Board Address
	BRD11B,                 // Rear Car Call #12 Input  Board Address
	BRD11B,                 // Rear Car Call #13 Input Board Address
	BRD11B,                 // Rear Car Call #14 Input Board Address
	BRD11B,                 // Rear Car Call #15 Input Board Address
	BRD11B,                 // Rear Car Call #16 Input Board Address
	BRD11C,                 // Rear Car Call #17 Input Board Address
	BRD11C,                 // Rear Car Call #18 Input Board Address
	BRD11C,                 // Rear Car Call #19 Input Board Address
	BRD11C,                 // Rear Car Call #20 Input Board Address
	// ON 2ND Serial Car Board
	BRD37A,                 // Rear Car Call #21 Input Board Address
	BRD37A,                 // Rear Car Call #22 Input Board Address
	BRD37A,                 // Rear Car Call #23 Input Board Address
	BRD37A,                 // Rear Car Call #24 Input Board Address
	BRD37A,                 // Rear Car Call #25 Input Board Address
	BRD37A,                 // Rear Car Call #26 Input Board Address
	BRD37A,                 // Rear Car Call #27 Input Board Address
	BRD37A,                 // Rear Car Call #28 Input Board Address
	BRD37B,                 // Rear Car Call #29 Input Board Address
	BRD37B,                 // Rear Car Call #30 Input Board Address
	BRD37B,                 // Rear Car Call #31 Input Board Address
	BRD37B,                 // Rear Car Call #32 Input Board Address
	BRD37B,                 // Rear Car Call #33 Input Board Address
	BRD37B,                 // Rear Car Call #34 Input Board Address
	BRD37B,                 // Rear Car Call #35 Input Board Address
	BRD37B,                 // Rear Car Call #36 Input Board Address
	BRD37C,                 // Rear Car Call #37 Input Board Address
	BRD37C,                 // Rear Car Call #38 Input Board Address
	BRD37C,                 // Rear Car Call #39 Input Board Address
	BRD37C,                 // Rear Car Call #40 Input Board Address
	BRD37C,                 // Rear Car Call #41 Input Board Address
	BRD37C,                 // Rear Car Call #42 Input Board Address
	BRD37C,                 // Rear Car Call #43 Input Board Address
	BRD37C,                 // Rear Car Call #44 Input Board Address
	// ON 3RD Serial Car Board
	BRD39A,                 // Rear Car Call #45 Input Board Address
	BRD39A,                 // Rear Car Call #46 Input Board Address
	BRD39A,                 // Rear Car Call #47 Input Board Address
	BRD39A,                 // Rear Car Call #48 Input Board Address
	BRD39A,                 // Rear Car Call #49 Input Board Address
	BRD39A,                 // Rear Car Call #50 Input Board Address
	BRD39A,                 // Rear Car Call #51 Input Board Address
	BRD39A,                 // Rear Car Call #52 Input Board Address
	BRD39B,                 // Rear Car Call #53 Input Board Address
	BRD39B,                 // Rear Car Call #54 Input Board Address
	BRD39B,                 // Rear Car Call #55 Input Board Address
	BRD39B,                 // Rear Car Call #56 Input Board Address
	BRD39B,                 // Rear Car Call #57 Input Board Address
	BRD39B,                 // Rear Car Call #58 Input Board Address
	BRD39B,                 // Rear Car Call #59 Input Board Address
	BRD39B,                 // Rear Car Call #60 Input Board Address
},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BRD39A,                 // Rear Car Call #1 Input Board Address
	BRD39A,                 // Rear Car Call #2 Input Board Address
	BRD39A,                 // Rear Car Call #3 Input Board Address
	BRD39A,                 // Rear Car Call #4 Input Board Address
	BRD39A,                 // Rear Car Call #5 Input Board Address
	BRD39A,                 // Rear Car Call #6 Input Board Address
	BRD39A,                 // Rear Car Call #7 Input Board Address
	BRD39A,                 // Rear Car Call #8 Input Board Address
	BRD39B,                 // Rear Car Call #9 Input Board Address
	BRD39B,                 // Rear Car Call #10 Input Board Address
	BRD39B,                 // Rear Car Call #11 Input  Board Address
	BRD39B,                 // Rear Car Call #12 Input  Board Address
	BRD39B,                 // Rear Car Call #13 Input Board Address
	BRD39B,                 // Rear Car Call #14 Input Board Address
	BRD39B,                 // Rear Car Call #15 Input Board Address
	BRD39B,                 // Rear Car Call #16 Input Board Address
	BRD39C,                 // Rear Car Call #17 Input Board Address
	BRD39C,                 // Rear Car Call #18 Input Board Address
	BRD39C,                 // Rear Car Call #19 Input Board Address
	BRD39C,                 // Rear Car Call #20 Input Board Address
	// ON 2ND Serial Car Board
	BRD37A,                 // Rear Car Call #21 Input Board Address
	BRD37A,                 // Rear Car Call #22 Input Board Address
	BRD37A,                 // Rear Car Call #23 Input Board Address
	BRD37A,                 // Rear Car Call #24 Input Board Address
	BRD37A,                 // Rear Car Call #25 Input Board Address
	BRD37A,                 // Rear Car Call #26 Input Board Address
	BRD37A,                 // Rear Car Call #27 Input Board Address
	BRD37A,                 // Rear Car Call #28 Input Board Address
	BRD37B,                 // Rear Car Call #29 Input Board Address
	BRD37B,                 // Rear Car Call #30 Input Board Address
	BRD37B,                 // Rear Car Call #31 Input Board Address
	BRD37B,                 // Rear Car Call #32 Input Board Address
	BRD37B,                 // Rear Car Call #33 Input Board Address
	BRD37B,                 // Rear Car Call #34 Input Board Address
	BRD37B,                 // Rear Car Call #35 Input Board Address
	BRD37B,                 // Rear Car Call #36 Input Board Address
	BRD37C,                 // Rear Car Call #37 Input Board Address
	BRD37C,                 // Rear Car Call #38 Input Board Address
	BRD37C,                 // Rear Car Call #39 Input Board Address
	BRD37C,                 // Rear Car Call #40 Input Board Address
	BRD37C,                 // Rear Car Call #41 Input Board Address
	BRD37C,                 // Rear Car Call #42 Input Board Address
	BRD37C,                 // Rear Car Call #43 Input Board Address
	BRD37C,                 // Rear Car Call #44 Input Board Address
	// ON 3RD Serial Car Board
	BRD39A,                 // Rear Car Call #45 Input Board Address
	BRD39A,                 // Rear Car Call #46 Input Board Address
	BRD39A,                 // Rear Car Call #47 Input Board Address
	BRD39A,                 // Rear Car Call #48 Input Board Address
	BRD39A,                 // Rear Car Call #49 Input Board Address
	BRD39A,                 // Rear Car Call #50 Input Board Address
	BRD39A,                 // Rear Car Call #51 Input Board Address
	BRD39A,                 // Rear Car Call #52 Input Board Address
	BRD39B,                 // Rear Car Call #53 Input Board Address
	BRD39B,                 // Rear Car Call #54 Input Board Address
	BRD39B,                 // Rear Car Call #55 Input Board Address
	BRD39B,                 // Rear Car Call #56 Input Board Address
	BRD39B,                 // Rear Car Call #57 Input Board Address
	BRD39B,                 // Rear Car Call #58 Input Board Address
	BRD39B,                 // Rear Car Call #59 Input Board Address
	BRD39B,                 // Rear Car Call #60 Input Board Address
	}
};

extern const unsigned char rm_ccrb[2][61] = { 
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BIT1,                   // Rear Car Call #1 Input Bit Number
	BIT2,                   // Rear Car Call #2 Input Bit Number
	BIT3,                   // Rear Car Call #3 Input Bit Number
	BIT4,                   // Rear Car Call #4 Input Bit Number
	BIT5,                   // Rear Car Call #5 Input Bit Number
	BIT6,                   // Rear Car Call #6 Input Bit Number
	BIT7,                   // Rear Car Call #7 Input Bit Number
	BIT8,                   // Rear Car Call #8 Input Bit Number
	BIT1,                   // Rear Car Call #9 Input Bit Number
	BIT2,                   // Rear Car Call #10 Input Bit Number
	BIT3,                   // Rear Car Call #11 Input Bit Number
	BIT4,                   // Rear Car Call #12 Input Bit Number
	BIT5,                   // Rear Car Call #13 Input Bit Number
	BIT6,                   // Rear Car Call #14 Input Bit Number
	BIT7,                   // Rear Car Call #15 Input Bit Number
	BIT8,                   // Rear Car Call #16 Input Bit Number
	BIT1,                   // Rear Car Call #17 Input Bit Number
	BIT2,                   // Rear Car Call #18 Input Bit Number
	BIT3,                   // Rear Car Call #19 Input Bit Number
	BIT4,                   // Rear Car Call #20 Input Bit Number
	// ON 2ND Serial Car Board
	BIT1,                   // Rear Car Call #21 Input Bit Number
	BIT2,                   // Rear Car Call #22 Input Bit Number
	BIT3,                   // Rear Car Call #23 Input Bit Number
	BIT4,                   // Rear Car Call #24 Input Bit Number
	BIT5,                   // Rear Car Call #25 Input Bit Number
	BIT6,                   // Rear Car Call #26 Input Bit Number
	BIT7,                   // Rear Car Call #27 Input Bit Number
	BIT8,                   // Rear Car Call #28 Input Bit Number
	BIT1,                   // Rear Car Call #29 Input Bit Number
	BIT2,                   // Rear Car Call #30 Input Bit Number
	BIT3,                   // Rear Car Call #31 Input Bit Number
	BIT4,                   // Rear Car Call #32 Input Bit Number
	BIT5,                   // Rear Car Call #33 Input Bit Number
	BIT6,                   // Rear Car Call #34 Input Bit Number
	BIT7,                   // Rear Car Call #35 Input Bit Number
	BIT8,                   // Rear Car Call #36 Input Bit Number
	BIT1,                   // Rear Car Call #37 Input Bit Number
	BIT2,                   // Rear Car Call #38 Input Bit Number
	BIT3,                   // Rear Car Call #39 Input Bit Number
	BIT4,                   // Rear Car Call #40 Input Bit Number
	BIT5,                   // Rear Car Call #41 Input Bit Number
	BIT6,                   // Rear Car Call #42 Input Bit Number
	BIT7,                   // Rear Car Call #43 Input Bit Number
	BIT8,                   // Rear Car Call #44 Input Bit Number
	// ON 3RD Serial Car Board
	BIT1,                   // Rear Car Call #45 Input Bit Number
	BIT2,                   // Rear Car Call #46 Input Bit Number
	BIT3,                   // Rear Car Call #47 Input Bit Number
	BIT4,                   // Rear Car Call #48 Input Bit Number
	BIT5,                   // Rear Car Call #49 Input Bit Number
	BIT6,                   // Rear Car Call #50 Input Bit Number
	BIT7,                   // Rear Car Call #51 Input Bit Number
	BIT8,                   // Rear Car Call #52 Input Bit Number
	BIT1,                   // Rear Car Call #53 Input Bit Number
	BIT2,                   // Rear Car Call #54 Input Bit Number
	BIT3,                   // Rear Car Call #55 Input Bit Number
	BIT4,                   // Rear Car Call #56 Input Bit Number
	BIT5,                   // Rear Car Call #57 Input Bit Number
	BIT6,                   // Rear Car Call #58 Input Bit Number
	BIT7,                   // Rear Car Call #59 Input Bit Number
	BIT8,                   // Rear Car Call #60 Input Bit Number
},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BIT1,                   // Rear Car Call #1 Input Bit Number
	BIT2,                   // Rear Car Call #2 Input Bit Number
	BIT3,                   // Rear Car Call #3 Input Bit Number
	BIT4,                   // Rear Car Call #4 Input Bit Number
	BIT5,                   // Rear Car Call #5 Input Bit Number
	BIT6,                   // Rear Car Call #6 Input Bit Number
	BIT7,                   // Rear Car Call #7 Input Bit Number
	BIT8,                   // Rear Car Call #8 Input Bit Number
	BIT1,                   // Rear Car Call #9 Input Bit Number
	BIT2,                   // Rear Car Call #10 Input Bit Number
	BIT3,                   // Rear Car Call #11 Input Bit Number
	BIT4,                   // Rear Car Call #12 Input Bit Number
	BIT5,                   // Rear Car Call #13 Input Bit Number
	BIT6,                   // Rear Car Call #14 Input Bit Number
	BIT7,                   // Rear Car Call #15 Input Bit Number
	BIT8,                   // Rear Car Call #16 Input Bit Number
	BIT1,                   // Rear Car Call #17 Input Bit Number
	BIT2,                   // Rear Car Call #18 Input Bit Number
	BIT3,                   // Rear Car Call #19 Input Bit Number
	BIT4,                   // Rear Car Call #20 Input Bit Number
	// ON 2ND Serial Car Board
	BIT1,                   // Rear Car Call #21 Input Bit Number
	BIT2,                   // Rear Car Call #22 Input Bit Number
	BIT3,                   // Rear Car Call #23 Input Bit Number
	BIT4,                   // Rear Car Call #24 Input Bit Number
	BIT5,                   // Rear Car Call #25 Input Bit Number
	BIT6,                   // Rear Car Call #26 Input Bit Number
	BIT7,                   // Rear Car Call #27 Input Bit Number
	BIT8,                   // Rear Car Call #28 Input Bit Number
	BIT1,                   // Rear Car Call #29 Input Bit Number
	BIT2,                   // Rear Car Call #30 Input Bit Number
	BIT3,                   // Rear Car Call #31 Input Bit Number
	BIT4,                   // Rear Car Call #32 Input Bit Number
	BIT5,                   // Rear Car Call #33 Input Bit Number
	BIT6,                   // Rear Car Call #34 Input Bit Number
	BIT7,                   // Rear Car Call #35 Input Bit Number
	BIT8,                   // Rear Car Call #36 Input Bit Number
	BIT1,                   // Rear Car Call #37 Input Bit Number
	BIT2,                   // Rear Car Call #38 Input Bit Number
	BIT3,                   // Rear Car Call #39 Input Bit Number
	BIT4,                   // Rear Car Call #40 Input Bit Number
	BIT5,                   // Rear Car Call #41 Input Bit Number
	BIT6,                   // Rear Car Call #42 Input Bit Number
	BIT7,                   // Rear Car Call #43 Input Bit Number
	BIT8,                   // Rear Car Call #44 Input Bit Number
	// ON 3RD Serial Car Board
	BIT1,                   // Rear Car Call #45 Input Bit Number
	BIT2,                   // Rear Car Call #46 Input Bit Number
	BIT3,                   // Rear Car Call #47 Input Bit Number
	BIT4,                   // Rear Car Call #48 Input Bit Number
	BIT5,                   // Rear Car Call #49 Input Bit Number
	BIT6,                   // Rear Car Call #50 Input Bit Number
	BIT7,                   // Rear Car Call #51 Input Bit Number
	BIT8,                   // Rear Car Call #52 Input Bit Number
	BIT1,                   // Rear Car Call #53 Input Bit Number
	BIT2,                   // Rear Car Call #54 Input Bit Number
	BIT3,                   // Rear Car Call #55 Input Bit Number
	BIT4,                   // Rear Car Call #56 Input Bit Number
	BIT5,                   // Rear Car Call #57 Input Bit Number
	BIT6,                   // Rear Car Call #58 Input Bit Number
	BIT7,                   // Rear Car Call #59 Input Bit Number
	BIT8,                   // Rear Car Call #60 Input Bit Number
	}
};

extern const unsigned char rm_ccrd[2][61] = {
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	30,                     // Rear Car Call #1 Input Array Number
	30,                     // Rear Car Call #2 Input Array Number
	30,                     // Rear Car Call #3 Input Array Number
	30,                     // Rear Car Call #4 Input Array Number
	30,                     // Rear Car Call #5 Input Array Number
	30,                     // Rear Car Call #6 Input Array Number
	30,                     // Rear Car Call #7 Input Array Number
	30,                     // Rear Car Call #8 Input Array Number
	31,                     // Rear Car Call #9 Input Array Number
	31,                     // Rear Car Call #10 Input Array Number
	31,                     // Rear Car Call #11 Input Array Number
	31,                     // Rear Car Call #12 Input Array Number
	31,                     // Rear Car Call #13 Input Array Number
	31,                     // Rear Car Call #14 Input Array Number
	31,                     // Rear Car Call #15 Input Array Number
	31,                     // Rear Car Call #16 Input Array Number
	32,                     // Rear Car Call #17 Input Array Number
	32,                     // Rear Car Call #18 Input Array Number
	32,                     // Rear Car Call #19 Input Array Number
	32,                     // Rear Car Call #20 Input Array Number
	// ON 2nd Serial Car Board
	108,                    // Rear Car Call #21 Input Array Number
	108,                    // Rear Car Call #22 Input Array Number
	108,                    // Rear Car Call #23 Input Array Number
	108,                    // Rear Car Call #24 Input Array Number
	108,                    // Rear Car Call #25 Input Array Number
	108,                    // Rear Car Call #26 Input Array Number
	108,                    // Rear Car Call #27 Input Array Number
	108,                    // Rear Car Call #28 Input Array Number
	109,                    // Rear Car Call #29 Input Array Number
	109,                    // Rear Car Call #30 Input Array Number
	109,                    // Rear Car Call #31 Input Array Number
	109,                    // Rear Car Call #32 Input Array Number
	109,                    // Rear Car Call #33 Input Array Number
	109,                    // Rear Car Call #34 Input Array Number
	109,                    // Rear Car Call #35 Input Array Number
	109,                    // Rear Car Call #36 Input Array Number
	110,                    // Rear Car Call #37 Input Array Number
	110,                    // Rear Car Call #38 Input Array Number
	110,                    // Rear Car Call #39 Input Array Number
	110,                    // Rear Car Call #40 Input Array Number
	110,                    // Rear Car Call #41 Input Array Number
	110,                    // Rear Car Call #42 Input Array Number
	110,                    // Rear Car Call #43 Input Array Number
	110,                    // Rear Car Call #44 Input Array Number
	// ON 3rd Serial Car Board
	114,                    // Rear Car Call #45 Input Array Number
	114,                    // Rear Car Call #46 Input Array Number
	114,                    // Rear Car Call #47 Input Array Number
	114,                    // Rear Car Call #48 Input Array Number
	114,                    // Rear Car Call #49 Input Array Number
	114,                    // Rear Car Call #50 Input Array Number
	114,                    // Rear Car Call #51 Input Array Number
	114,                    // Rear Car Call #52 Input Array Number
	115,                    // Rear Car Call #53 Input Array Number
	115,                    // Rear Car Call #54 Input Array Number
	115,                    // Rear Car Call #55 Input Array Number
	115,                    // Rear Car Call #56 Input Array Number
	115,                    // Rear Car Call #57 Input Array Number
	115,                    // Rear Car Call #58 Input Array Number
	115,                    // Rear Car Call #59 Input Array Number
	115,                    // Rear Car Call #60 Input Array Number
},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	114,                     // Rear Car Call #1 Input Array Number
	114,                     // Rear Car Call #2 Input Array Number
	114,                     // Rear Car Call #3 Input Array Number
	114,                     // Rear Car Call #4 Input Array Number
	114,                     // Rear Car Call #5 Input Array Number
	114,                     // Rear Car Call #6 Input Array Number
	114,                     // Rear Car Call #7 Input Array Number
	114,                     // Rear Car Call #8 Input Array Number
	115,                     // Rear Car Call #9 Input Array Number
	115,                     // Rear Car Call #10 Input Array Number
	115,                     // Rear Car Call #11 Input Array Number
	115,                     // Rear Car Call #12 Input Array Number
	115,                     // Rear Car Call #13 Input Array Number
	115,                     // Rear Car Call #14 Input Array Number
	115,                     // Rear Car Call #15 Input Array Number
	115,                     // Rear Car Call #16 Input Array Number
	116,                     // Rear Car Call #17 Input Array Number
	116,                     // Rear Car Call #18 Input Array Number
	116,                     // Rear Car Call #19 Input Array Number
	116,                     // Rear Car Call #20 Input Array Number
	// ON 2nd Serial Car Board
	108,                    // Rear Car Call #21 Input Array Number
	108,                    // Rear Car Call #22 Input Array Number
	108,                    // Rear Car Call #23 Input Array Number
	108,                    // Rear Car Call #24 Input Array Number
	108,                    // Rear Car Call #25 Input Array Number
	108,                    // Rear Car Call #26 Input Array Number
	108,                    // Rear Car Call #27 Input Array Number
	108,                    // Rear Car Call #28 Input Array Number
	109,                    // Rear Car Call #29 Input Array Number
	109,                    // Rear Car Call #30 Input Array Number
	109,                    // Rear Car Call #31 Input Array Number
	109,                    // Rear Car Call #32 Input Array Number
	109,                    // Rear Car Call #33 Input Array Number
	109,                    // Rear Car Call #34 Input Array Number
	109,                    // Rear Car Call #35 Input Array Number
	109,                    // Rear Car Call #36 Input Array Number
	110,                    // Rear Car Call #37 Input Array Number
	110,                    // Rear Car Call #38 Input Array Number
	110,                    // Rear Car Call #39 Input Array Number
	110,                    // Rear Car Call #40 Input Array Number
	110,                    // Rear Car Call #41 Input Array Number
	110,                    // Rear Car Call #42 Input Array Number
	110,                    // Rear Car Call #43 Input Array Number
	110,                    // Rear Car Call #44 Input Array Number
	// ON 3rd Serial Car Board
	114,                    // Rear Car Call #45 Input Array Number
	114,                    // Rear Car Call #46 Input Array Number
	114,                    // Rear Car Call #47 Input Array Number
	114,                    // Rear Car Call #48 Input Array Number
	114,                    // Rear Car Call #49 Input Array Number
	114,                    // Rear Car Call #50 Input Array Number
	114,                    // Rear Car Call #51 Input Array Number
	114,                    // Rear Car Call #52 Input Array Number
	115,                    // Rear Car Call #53 Input Array Number
	115,                    // Rear Car Call #54 Input Array Number
	115,                    // Rear Car Call #55 Input Array Number
	115,                    // Rear Car Call #56 Input Array Number
	115,                    // Rear Car Call #57 Input Array Number
	115,                    // Rear Car Call #58 Input Array Number
	115,                    // Rear Car Call #59 Input Array Number
	115,                    // Rear Car Call #60 Input Array Number
	}
};





extern const int16 rm_ccrl[2][61] = {
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD12A,                 // Rear Car Call Light #1 Output Board Address
	BRD12A,                 // Rear Car Call Light #2 Output Board Address
	BRD12A,                 // Rear Car Call Light #3 Output Board Address
	BRD12A,                 // Rear Car Call Light #4 Output Board Address
	BRD12A,                 // Rear Car Call Light #5 Output Board Address
	BRD12A,                 // Rear Car Call Light #6 Output Board Address
	BRD12A,                 // Rear Car Call Light #7 Output Board Address
	BRD12A,                 // Rear Car Call Light #8 Output Board Address
	BRD12B,                 // Rear Car Call Light #9 Output Board Address
	BRD12B,                 // Rear Car Call Light #10 Output  Board Address
	BRD12B,                 // Rear Car Call Light #11 Output  Board Address
	BRD12B,                 // Rear Car Call Light #12 Output Board Address
	BRD12B,                 // Rear Car Call Light #13 Output Board Address
	BRD12B,                 // Rear Car Call Light #14 Output Board Address
	BRD12B,                 // Rear Car Call Light #15 Output Board Address
	BRD12B,                 // Rear Car Call Light #16 Output Board Address
	BRD12C,                 // Rear Car Call Light #17 Output Board Address
	BRD12C,                 // Rear Car Call Light #18 Output Board Address
	BRD12C,                 // Rear Car Call Light #19 Output Board Address
	BRD12C,                 // Rear Car Call Light #20 Output Board Address
	// ON 2nd Serial Car Board
	BRD38A,                 // Rear Car Call Light #21 Output Board Address
	BRD38A,                 // Rear Car Call Light #22 Output Board Address
	BRD38A,                 // Rear Car Call Light #23 Output Board Address
	BRD38A,                 // Rear Car Call Light #24 Output Board Address
	BRD38A,                 // Rear Car Call Light #25 Output Board Address
	BRD38A,                 // Rear Car Call Light #26 Output Board Address
	BRD38A,                 // Rear Car Call Light #27 Output Board Address
	BRD38A,                 // Rear Car Call Light #28 Output Board Address
	BRD38B,                 // Rear Car Call Light #29 Output Board Address
	BRD38B,                 // Rear Car Call Light #30 Output Board Address
	BRD38B,                 // Rear Car Call Light #31 Output Board Address
	BRD38B,                 // Rear Car Call Light #32 Output Board Address
	BRD38B,                 // Rear Car Call Light #33 Output Board Address
	BRD38B,                 // Rear Car Call Light #34 Output Board Address
	BRD38B,                 // Rear Car Call Light #35 Output Board Address
	BRD38B,                 // Rear Car Call Light #36 Output Board Address
	BRD38C,                 // Rear Car Call Light #37 Output Board Address
	BRD38C,                 // Rear Car Call Light #38 Output Board Address
	BRD38C,                 // Rear Car Call Light #39 Output Board Address
	BRD38C,                 // Rear Car Call Light #40 Output Board Address
	BRD38C,                 // Rear Car Call Light #41 Output Board Address
	BRD38C,                 // Rear Car Call Light #42 Output Board Address
	BRD38C,                 // Rear Car Call Light #43 Output Board Address
	BRD38C,                 // Rear Car Call Light #44 Output Board Address
	// ON 3rd Serial Car Board
	BRD40A,                 // Rear Car Call Light #45 Output Board Address
	BRD40A,                 // Rear Car Call Light #46 Output Board Address
	BRD40A,                 // Rear Car Call Light #47 Output Board Address
	BRD40A,                 // Rear Car Call Light #48 Output Board Address
	BRD40A,                 // Rear Car Call Light #49 Output Board Address
	BRD40A,                 // Rear Car Call Light #50 Output Board Address
	BRD40A,                 // Rear Car Call Light #51 Output Board Address
	BRD40A,                 // Rear Car Call Light #52 Output Board Address
	BRD40B,                 // Rear Car Call Light #53 Output Board Address
	BRD40B,                 // Rear Car Call Light #54 Output Board Address
	BRD40B,                 // Rear Car Call Light #55 Output Board Address
	BRD40B,                 // Rear Car Call Light #56 Output Board Address
	BRD40B,                 // Rear Car Call Light #57 Output Board Address
	BRD40B,                 // Rear Car Call Light #58 Output Board Address
	BRD40B,                 // Rear Car Call Light #59 Output Board Address
	BRD40B,                 // Rear Car Call Light #60 Output Board Address
},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD40A,                 // Rear Car Call Light #1 Output Board Address
	BRD40A,                 // Rear Car Call Light #2 Output Board Address
	BRD40A,                 // Rear Car Call Light #3 Output Board Address
	BRD40A,                 // Rear Car Call Light #4 Output Board Address
	BRD40A,                 // Rear Car Call Light #5 Output Board Address
	BRD40A,                 // Rear Car Call Light #6 Output Board Address
	BRD40A,                 // Rear Car Call Light #7 Output Board Address
	BRD40A,                 // Rear Car Call Light #8 Output Board Address
	BRD40B,                 // Rear Car Call Light #9 Output Board Address
	BRD40B,                 // Rear Car Call Light #10 Output  Board Address
	BRD40B,                 // Rear Car Call Light #11 Output  Board Address
	BRD40B,                 // Rear Car Call Light #12 Output Board Address
	BRD40B,                 // Rear Car Call Light #13 Output Board Address
	BRD40B,                 // Rear Car Call Light #14 Output Board Address
	BRD40B,                 // Rear Car Call Light #15 Output Board Address
	BRD40B,                 // Rear Car Call Light #16 Output Board Address
	BRD40C,                 // Rear Car Call Light #17 Output Board Address
	BRD40C,                 // Rear Car Call Light #18 Output Board Address
	BRD40C,                 // Rear Car Call Light #19 Output Board Address
	BRD40C,                 // Rear Car Call Light #20 Output Board Address
	// ON 2nd Serial Car Board
	BRD38A,                 // Rear Car Call Light #21 Output Board Address
	BRD38A,                 // Rear Car Call Light #22 Output Board Address
	BRD38A,                 // Rear Car Call Light #23 Output Board Address
	BRD38A,                 // Rear Car Call Light #24 Output Board Address
	BRD38A,                 // Rear Car Call Light #25 Output Board Address
	BRD38A,                 // Rear Car Call Light #26 Output Board Address
	BRD38A,                 // Rear Car Call Light #27 Output Board Address
	BRD38A,                 // Rear Car Call Light #28 Output Board Address
	BRD38B,                 // Rear Car Call Light #29 Output Board Address
	BRD38B,                 // Rear Car Call Light #30 Output Board Address
	BRD38B,                 // Rear Car Call Light #31 Output Board Address
	BRD38B,                 // Rear Car Call Light #32 Output Board Address
	BRD38B,                 // Rear Car Call Light #33 Output Board Address
	BRD38B,                 // Rear Car Call Light #34 Output Board Address
	BRD38B,                 // Rear Car Call Light #35 Output Board Address
	BRD38B,                 // Rear Car Call Light #36 Output Board Address
	BRD38C,                 // Rear Car Call Light #37 Output Board Address
	BRD38C,                 // Rear Car Call Light #38 Output Board Address
	BRD38C,                 // Rear Car Call Light #39 Output Board Address
	BRD38C,                 // Rear Car Call Light #40 Output Board Address
	BRD38C,                 // Rear Car Call Light #41 Output Board Address
	BRD38C,                 // Rear Car Call Light #42 Output Board Address
	BRD38C,                 // Rear Car Call Light #43 Output Board Address
	BRD38C,                 // Rear Car Call Light #44 Output Board Address
	// ON 3rd Serial Car Board
	BRD40A,                 // Rear Car Call Light #45 Output Board Address
	BRD40A,                 // Rear Car Call Light #46 Output Board Address
	BRD40A,                 // Rear Car Call Light #47 Output Board Address
	BRD40A,                 // Rear Car Call Light #48 Output Board Address
	BRD40A,                 // Rear Car Call Light #49 Output Board Address
	BRD40A,                 // Rear Car Call Light #50 Output Board Address
	BRD40A,                 // Rear Car Call Light #51 Output Board Address
	BRD40A,                 // Rear Car Call Light #52 Output Board Address
	BRD40B,                 // Rear Car Call Light #53 Output Board Address
	BRD40B,                 // Rear Car Call Light #54 Output Board Address
	BRD40B,                 // Rear Car Call Light #55 Output Board Address
	BRD40B,                 // Rear Car Call Light #56 Output Board Address
	BRD40B,                 // Rear Car Call Light #57 Output Board Address
	BRD40B,                 // Rear Car Call Light #58 Output Board Address
	BRD40B,                 // Rear Car Call Light #59 Output Board Address
	BRD40B,                 // Rear Car Call Light #60 Output Board Address
	}
};

extern const unsigned char rm_ccrlb[2][61] = {
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Rear Car Call Light #1 Output Bit Number
	BIT2,                   // Rear Car Call Light #2 Output Bit Number
	BIT3,                   // Rear Car Call Light #3 Output Bit Number
	BIT4,                   // Rear Car Call Light #4 Output Bit Number
	BIT5,                   // Rear Car Call Light #5 Output Bit Number
	BIT6,                   // Rear Car Call Light #6 Output Bit Number
	BIT7,                   // Rear Car Call Light #7 Output Bit Number
	BIT8,                   // Rear Car Call Light #8 Output Bit Number
	BIT1,                   // Rear Car Call Light #9 Output Bit Number
	BIT2,                   // Rear Car Call Light #10 Output Bit Number
	BIT3,                   // Rear Car Call Light #11 Output Bit Number
	BIT4,                   // Rear Car Call Light #12 Output Bit Number
	BIT5,                   // Rear Car Call Light #13 Output Bit Number
	BIT6,                   // Rear Car Call Light #14 Output Bit Number
	BIT7,                   // Rear Car Call Light #15 Output Bit Number
	BIT8,                   // Rear Car Call Light #16 Output Bit Number
	BIT1,                   // Rear Car Call Light #17 Output Bit Number
	BIT2,                   // Rear Car Call Light #18 Output Bit Number
	BIT3,                   // Rear Car Call Light #19 Output Bit Number
	BIT4,                   // Rear Car Call Light #20 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Rear Car Call Light #21 Output Bit Number
	BIT2,                   // Rear Car Call Light #22 Output Bit Number
	BIT3,                   // Rear Car Call Light #23 Output Bit Number
	BIT4,                   // Rear Car Call Light #24 Output Bit Number
	BIT5,                   // Rear Car Call Light #25 Output Bit Number
	BIT6,                   // Rear Car Call Light #26 Output Bit Number
	BIT7,                   // Rear Car Call Light #27 Output Bit Number
	BIT8,                   // Rear Car Call Light #28 Output Bit Number
	BIT1,                   // Rear Car Call Light #29 Output Bit Number
	BIT2,                   // Rear Car Call Light #30 Output Bit Number
	BIT3,                   // Rear Car Call Light #31 Output Bit Number
	BIT4,                   // Rear Car Call Light #32 Output Bit Number
	BIT5,                   // Rear Car Call Light #33 Output Bit Number
	BIT6,                   // Rear Car Call Light #34 Output Bit Number
	BIT7,                   // Rear Car Call Light #35 Output Bit Number
	BIT8,                   // Rear Car Call Light #36 Output Bit Number
	BIT1,                   // Rear Car Call Light #37 Output Bit Number
	BIT2,                   // Rear Car Call Light #38 Output Bit Number
	BIT3,                   // Rear Car Call Light #39 Output Bit Number
	BIT4,                   // Rear Car Call Light #40 Output Bit Number
	BIT5,                   // Rear Car Call Light #41 Output Bit Number
	BIT6,                   // Rear Car Call Light #42 Output Bit Number
	BIT7,                   // Rear Car Call Light #43 Output Bit Number
	BIT8,                   // Rear Car Call Light #44 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Rear Car Call Light #45 Output Bit Number
	BIT2,                   // Rear Car Call Light #46 Output Bit Number
	BIT3,                   // Rear Car Call Light #47 Output Bit Number
	BIT4,                   // Rear Car Call Light #48 Output Bit Number
	BIT5,                   // Rear Car Call Light #49 Output Bit Number
	BIT6,                   // Rear Car Call Light #50 Output Bit Number
	BIT7,                   // Rear Car Call Light #51 Output Bit Number
	BIT8,                   // Rear Car Call Light #52 Output Bit Number
	BIT1,                   // Rear Car Call Light #53 Output Bit Number
	BIT2,                   // Rear Car Call Light #54 Output Bit Number
	BIT3,                   // Rear Car Call Light #55 Output Bit Number
	BIT4,                   // Rear Car Call Light #56 Output Bit Number
	BIT5,                   // Rear Car Call Light #57 Output Bit Number
	BIT6,                   // Rear Car Call Light #58 Output Bit Number
	BIT7,                   // Rear Car Call Light #59 Output Bit Number
	BIT8,                   // Rear Car Call Light #60 Output Bit Number
},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Rear Car Call Light #1 Output Bit Number
	BIT2,                   // Rear Car Call Light #2 Output Bit Number
	BIT3,                   // Rear Car Call Light #3 Output Bit Number
	BIT4,                   // Rear Car Call Light #4 Output Bit Number
	BIT5,                   // Rear Car Call Light #5 Output Bit Number
	BIT6,                   // Rear Car Call Light #6 Output Bit Number
	BIT7,                   // Rear Car Call Light #7 Output Bit Number
	BIT8,                   // Rear Car Call Light #8 Output Bit Number
	BIT1,                   // Rear Car Call Light #9 Output Bit Number
	BIT2,                   // Rear Car Call Light #10 Output Bit Number
	BIT3,                   // Rear Car Call Light #11 Output Bit Number
	BIT4,                   // Rear Car Call Light #12 Output Bit Number
	BIT5,                   // Rear Car Call Light #13 Output Bit Number
	BIT6,                   // Rear Car Call Light #14 Output Bit Number
	BIT7,                   // Rear Car Call Light #15 Output Bit Number
	BIT8,                   // Rear Car Call Light #16 Output Bit Number
	BIT1,                   // Rear Car Call Light #17 Output Bit Number
	BIT2,                   // Rear Car Call Light #18 Output Bit Number
	BIT3,                   // Rear Car Call Light #19 Output Bit Number
	BIT4,                   // Rear Car Call Light #20 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Rear Car Call Light #21 Output Bit Number
	BIT2,                   // Rear Car Call Light #22 Output Bit Number
	BIT3,                   // Rear Car Call Light #23 Output Bit Number
	BIT4,                   // Rear Car Call Light #24 Output Bit Number
	BIT5,                   // Rear Car Call Light #25 Output Bit Number
	BIT6,                   // Rear Car Call Light #26 Output Bit Number
	BIT7,                   // Rear Car Call Light #27 Output Bit Number
	BIT8,                   // Rear Car Call Light #28 Output Bit Number
	BIT1,                   // Rear Car Call Light #29 Output Bit Number
	BIT2,                   // Rear Car Call Light #30 Output Bit Number
	BIT3,                   // Rear Car Call Light #31 Output Bit Number
	BIT4,                   // Rear Car Call Light #32 Output Bit Number
	BIT5,                   // Rear Car Call Light #33 Output Bit Number
	BIT6,                   // Rear Car Call Light #34 Output Bit Number
	BIT7,                   // Rear Car Call Light #35 Output Bit Number
	BIT8,                   // Rear Car Call Light #36 Output Bit Number
	BIT1,                   // Rear Car Call Light #37 Output Bit Number
	BIT2,                   // Rear Car Call Light #38 Output Bit Number
	BIT3,                   // Rear Car Call Light #39 Output Bit Number
	BIT4,                   // Rear Car Call Light #40 Output Bit Number
	BIT5,                   // Rear Car Call Light #41 Output Bit Number
	BIT6,                   // Rear Car Call Light #42 Output Bit Number
	BIT7,                   // Rear Car Call Light #43 Output Bit Number
	BIT8,                   // Rear Car Call Light #44 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Rear Car Call Light #45 Output Bit Number
	BIT2,                   // Rear Car Call Light #46 Output Bit Number
	BIT3,                   // Rear Car Call Light #47 Output Bit Number
	BIT4,                   // Rear Car Call Light #48 Output Bit Number
	BIT5,                   // Rear Car Call Light #49 Output Bit Number
	BIT6,                   // Rear Car Call Light #50 Output Bit Number
	BIT7,                   // Rear Car Call Light #51 Output Bit Number
	BIT8,                   // Rear Car Call Light #52 Output Bit Number
	BIT1,                   // Rear Car Call Light #53 Output Bit Number
	BIT2,                   // Rear Car Call Light #54 Output Bit Number
	BIT3,                   // Rear Car Call Light #55 Output Bit Number
	BIT4,                   // Rear Car Call Light #56 Output Bit Number
	BIT5,                   // Rear Car Call Light #57 Output Bit Number
	BIT6,                   // Rear Car Call Light #58 Output Bit Number
	BIT7,                   // Rear Car Call Light #59 Output Bit Number
	BIT8,                   // Rear Car Call Light #60 Output Bit Number
	}
};

extern const unsigned char rm_ccrld[2][61] = {
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	33,                     // Rear Car Call Light #1 Output Array Number
	33,                     // Rear Car Call Light #2 Output Array Number
	33,                     // Rear Car Call Light #3 Output Array Number
	33,                     // Rear Car Call Light #4 Output Array Number
	33,                     // Rear Car Call Light #5 Output Array Number
	33,                     // Rear Car Call Light #6 Output Array Number
	33,                     // Rear Car Call Light #7 Output Array Number
	33,                     // Rear Car Call Light #8 Output Array Number
	34,                     // Rear Car Call Light #9 Output Array Number
	34,                     // Rear Car Call Light #10 Output Array Number
	34,                     // Rear Car Call Light #11 Output Array Number
	34,                     // Rear Car Call Light #12 Output Array Number
	34,                     // Rear Car Call Light #13 Output Array Number
	34,                     // Rear Car Call Light #14 Output Array Number
	34,                     // Rear Car Call Light #15 Output Array Number
	34,                     // Rear Car Call Light #16 Output Array Number
	35,                     // Rear Car Call Light #17 Output Array Number
	35,                     // Rear Car Call Light #18 Output Array Number
	35,                     // Rear Car Call Light #19 Output Array Number
	35,                     // Rear Car Call Light #20 Output Array Number
	// ON 2nd Serial Car Board
	111,                    // Rear Car Call Light #21 Output Array Number
	111,                    // Rear Car Call Light #22 Output Array Number
	111,                    // Rear Car Call Light #23 Output Array Number
	111,                    // Rear Car Call Light #24 Output Array Number
	111,                    // Rear Car Call Light #25 Output Array Number
	111,                    // Rear Car Call Light #26 Output Array Number
	111,                    // Rear Car Call Light #27 Output Array Number
	111,                    // Rear Car Call Light #28 Output Array Number
	112,                    // Rear Car Call Light #29 Output Array Number
	112,                    // Rear Car Call Light #30 Output Array Number
	112,                    // Rear Car Call Light #31 Output Array Number
	112,                    // Rear Car Call Light #32 Output Array Number
	112,                    // Rear Car Call Light #33 Output Array Number
	112,                    // Rear Car Call Light #34 Output Array Number
	112,                    // Rear Car Call Light #35 Output Array Number
	112,                    // Rear Car Call Light #36 Output Array Number
	113,                    // Rear Car Call Light #37 Output Array Number
	113,                    // Rear Car Call Light #38 Output Array Number
	113,                    // Rear Car Call Light #39 Output Array Number
	113,                    // Rear Car Call Light #40 Output Array Number
	113,                    // Rear Car Call Light #41 Output Array Number
	113,                    // Rear Car Call Light #42 Output Array Number
	113,                    // Rear Car Call Light #43 Output Array Number
	113,                    // Rear Car Call Light #44 Output Array Number
	// ON 3rd Serial Car Board
	117,                    // Rear Car Call Light #45 Output Array Number
	117,                    // Rear Car Call Light #46 Output Array Number
	117,                    // Rear Car Call Light #47 Output Array Number
	117,                    // Rear Car Call Light #48 Output Array Number
	117,                    // Rear Car Call Light #49 Output Array Number
	117,                    // Rear Car Call Light #50 Output Array Number
	117,                    // Rear Car Call Light #51 Output Array Number
	117,                    // Rear Car Call Light #52 Output Array Number
	118,                    // Rear Car Call Light #53 Output Array Number
	118,                    // Rear Car Call Light #54 Output Array Number
	118,                    // Rear Car Call Light #55 Output Array Number
	118,                    // Rear Car Call Light #56 Output Array Number
	118,                    // Rear Car Call Light #57 Output Array Number
	118,                    // Rear Car Call Light #58 Output Array Number
	118,                    // Rear Car Call Light #59 Output Array Number
	118,                    // Rear Car Call Light #60 Output Array Number
},
{			  // Used with Remote car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	117,                     // Rear Car Call Light #1 Output Array Number
	117,                     // Rear Car Call Light #2 Output Array Number
	117,                     // Rear Car Call Light #3 Output Array Number
	117,                     // Rear Car Call Light #4 Output Array Number
	117,                     // Rear Car Call Light #5 Output Array Number
	117,                     // Rear Car Call Light #6 Output Array Number
	117,                     // Rear Car Call Light #7 Output Array Number
	117,                     // Rear Car Call Light #8 Output Array Number
	118,                     // Rear Car Call Light #9 Output Array Number
	118,                     // Rear Car Call Light #10 Output Array Number
	118,                     // Rear Car Call Light #11 Output Array Number
	118,                     // Rear Car Call Light #12 Output Array Number
	118,                     // Rear Car Call Light #13 Output Array Number
	118,                     // Rear Car Call Light #14 Output Array Number
	118,                     // Rear Car Call Light #15 Output Array Number
	118,                     // Rear Car Call Light #16 Output Array Number
	119,                     // Rear Car Call Light #17 Output Array Number
	119,                     // Rear Car Call Light #18 Output Array Number
	119,                     // Rear Car Call Light #19 Output Array Number
	119,                     // Rear Car Call Light #20 Output Array Number
	// ON 2nd Serial Car Board
	111,                    // Rear Car Call Light #21 Output Array Number
	111,                    // Rear Car Call Light #22 Output Array Number
	111,                    // Rear Car Call Light #23 Output Array Number
	111,                    // Rear Car Call Light #24 Output Array Number
	111,                    // Rear Car Call Light #25 Output Array Number
	111,                    // Rear Car Call Light #26 Output Array Number
	111,                    // Rear Car Call Light #27 Output Array Number
	111,                    // Rear Car Call Light #28 Output Array Number
	112,                    // Rear Car Call Light #29 Output Array Number
	112,                    // Rear Car Call Light #30 Output Array Number
	112,                    // Rear Car Call Light #31 Output Array Number
	112,                    // Rear Car Call Light #32 Output Array Number
	112,                    // Rear Car Call Light #33 Output Array Number
	112,                    // Rear Car Call Light #34 Output Array Number
	112,                    // Rear Car Call Light #35 Output Array Number
	112,                    // Rear Car Call Light #36 Output Array Number
	113,                    // Rear Car Call Light #37 Output Array Number
	113,                    // Rear Car Call Light #38 Output Array Number
	113,                    // Rear Car Call Light #39 Output Array Number
	113,                    // Rear Car Call Light #40 Output Array Number
	113,                    // Rear Car Call Light #41 Output Array Number
	113,                    // Rear Car Call Light #42 Output Array Number
	113,                    // Rear Car Call Light #43 Output Array Number
	113,                    // Rear Car Call Light #44 Output Array Number
	// ON 3rd Serial Car Board
	117,                    // Rear Car Call Light #45 Output Array Number
	117,                    // Rear Car Call Light #46 Output Array Number
	117,                    // Rear Car Call Light #47 Output Array Number
	117,                    // Rear Car Call Light #48 Output Array Number
	117,                    // Rear Car Call Light #49 Output Array Number
	117,                    // Rear Car Call Light #50 Output Array Number
	117,                    // Rear Car Call Light #51 Output Array Number
	117,                    // Rear Car Call Light #52 Output Array Number
	118,                    // Rear Car Call Light #53 Output Array Number
	118,                    // Rear Car Call Light #54 Output Array Number
	118,                    // Rear Car Call Light #55 Output Array Number
	118,                    // Rear Car Call Light #56 Output Array Number
	118,                    // Rear Car Call Light #57 Output Array Number
	118,                    // Rear Car Call Light #58 Output Array Number
	118,                    // Rear Car Call Light #59 Output Array Number
	118,                    // Rear Car Call Light #60 Output Array Number
	}
};

extern const int16 sccl[61] = 	
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BRD8A,                  // Security Car Call Latch #1 Input Board Address
	BRD8A,                  // Security Car Call Latch #2 Input Board Address
	BRD8A,                  // Security Car Call Latch #3 Input Board Address
	BRD8A,                  // Security Car Call Latch #4 Input Board Address
	BRD8A,                  // Security Car Call Latch #5 Input Board Address
	BRD8A,                  // Security Car Call Latch #6 Input Board Address
	BRD8A,                  // Security Car Call Latch #7 Input Board Address
	BRD8A,                  // Security Car Call Latch #8 Input Board Address
	BRD8B,                  // Security Car Call Latch #9 Input Board Address
	BRD8B,                  // Security Car Call Latch #10 Input Board Address
	BRD8B,                  // Security Car Call Latch #11 Input Board Address
	BRD8B,                  // Security Car Call Latch #12 Input Board Address
	BRD8B,                  // Security Car Call Latch #13 Input Board Address
	BRD8B,                  // Security Car Call Latch #14 Input Board Address
	BRD8B,                  // Security Car Call Latch #15 Input Board Address
	BRD8B,                  // Security Car Call Latch #16 Input Board Address
	BRD8C,                  // Security Car Call Latch #17 Input Board Address
	BRD8C,                 // Security Car Call Latch #18 Input Board Address
	BRD8C,                 // Security Car Call Latch #19 Input Board Address
	BRD8C,                 // Security Car Call Latch #20 Input Board Address
	BRD8C,                 // Security Car Call Latch #21 Input Board Address
	BRD8C,                 // Security Car Call Latch #22 Input Board Address
	BRD8C,                 // Security Car Call Latch #23 Input Board Address
	// ON 2ND Serial Car Board
	BRD34A,                 // Security Car Call Latch #24 Input Board Address
	BRD34A,                 // Security Car Call Latch #25 Input Board Address
	BRD34A,                 // Security Car Call Latch #26 Input Board Address
	BRD34A,                 // Security Car Call Latch #27 Input Board Address
	BRD34A,                 // Security Car Call Latch #28 Input Board Address
	BRD34A,                 // Security Car Call Latch #29 Input Board Address
	BRD34A,                 // Security Car Call Latch #30 Input Board Address
	BRD34A,                 // Security Car Call Latch #31 Input Board Address
	BRD34B,                 // Security Car Call Latch #32 Input Board Address
	BRD34B,                 // Security Car Call Latch #33 Input Board Address
	BRD34B,                 // Security Car Call Latch #34 Input Board Address
	BRD34B,                 // Security Car Call Latch #35 Input Board Address
	BRD34B,                 // Security Car Call Latch #36 Input Board Address
	BRD34B,                 // Security Car Call Latch #37 Input Board Address
	BRD34B,                 // Security Car Call Latch #38 Input Board Address
	BRD34B,                 // Security Car Call Latch #39 Input Board Address
	BRD34C,                 // Security Car Call Latch #40 Input Board Address
	BRD34C,                 // Security Car Call Latch #41 Input Board Address
	BRD34C,                 // Security Car Call Latch #42 Input Board Address
	BRD34C,                 // Security Car Call Latch #43 Input Board Address
	BRD34C,                 // Security Car Call Latch #44 Input Board Address
	BRD34C,                 // Security Car Call Latch #45 Input Board Address
	BRD34C,                 // Security Car Call Latch #46 Input Board Address
	BRD34C,                 // Security Car Call Latch #47 Input Board Address
	// ON 3RD Serial Car Board
	BRD36A,                 // Security Car Call Latch #48 Input Board Address
	BRD36A,                 // Security Car Call Latch #49 Input Board Address
	BRD36A,                 // Security Car Call Latch #50 Input Board Address
	BRD36A,                 // Security Car Call Latch #51 Input Board Address
	BRD36A,                 // Security Car Call Latch #52 Input Board Address
	BRD36A,                 // Security Car Call Latch #53 Input Board Address
	BRD36A,                 // Security Car Call Latch #54 Input Board Address
	BRD36A,                 // Security Car Call Latch #55 Input Board Address
	BRD36B,                 // Security Car Call Latch #56 Input Board Address
	BRD36B,                 // Security Car Call Latch #57 Input Board Address
	BRD36B,                 // Security Car Call Latch #58 Input Board Address
	BRD36B,                 // Security Car Call Latch #59 Input Board Address
	BRD36B,                 // Security Car Call Latch #60 Input Board Address
};
extern const unsigned char scclb[61] =  
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1ST Serial Car Board
	BIT1,                   // Security Car Call Latch #1 Input Bit Number
	BIT2,                   // Security Car Call Latch #2 Input Bit Number
	BIT3,                   // Security Car Call Latch #3 Input Bit Number
	BIT4,                   // Security Car Call Latch #4 Input Bit Number
	BIT5,                   // Security Car Call Latch #5 Input Bit Number
	BIT6,                   // Security Car Call Latch #6 Input Bit Number
	BIT7,                   // Security Car Call Latch #7 Input Bit Number
	BIT8,                   // Security Car Call Latch #8 Input Bit Number
	BIT1,                   // Security Car Call Latch #9 Input Bit Number
	BIT2,                   // Security Car Call Latch #10 Input Bit Number
	BIT3,                   // Security Car Call Latch #11 Input Bit Number
	BIT4,                   // Security Car Call Latch #12 Input Bit Number
	BIT5,                   // Security Car Call Latch #13 Input Bit Number
	BIT6,                   // Security Car Call Latch #14 Input Bit Number
	BIT7,                   // Security Car Call Latch #15 Input Bit Number
	BIT8,                   // Security Car Call Latch #16 Input Bit Number
	BIT1,                   // Security Car Call Latch #17 Input Bit Number
	BIT2,                   // Security Car Call Latch #18 Input Bit Number
	BIT3,                   // Security Car Call Latch #19 Input Bit Number
	BIT4,                   // Security Car Call Latch #20 Input Bit Number
	BIT5,                   // Security Car Call Latch #21 Input Bit Number
	BIT6,                   // Security Car Call Latch #22 Input Bit Number
	BIT7,                   // Security Car Call Latch #23 Input Bit Number
	// ON 2ND Serial Car Board
	BIT1,                   // Security Car Call Latch #24 Input Bit Number
	BIT2,                   // Security Car Call Latch #25 Input Bit Number
	BIT3,                   // Security Car Call Latch #26 Input Bit Number
	BIT4,                   // Security Car Call Latch #27 Input Bit Number
	BIT5,                   // Security Car Call Latch #28 Input Bit Number
	BIT6,                   // Security Car Call Latch #29 Input Bit Number
	BIT7,                   // Security Car Call Latch #30 Input Bit Number
	BIT8,                   // Security Car Call Latch #31 Input Bit Number
	BIT1,                   // Security Car Call Latch #32 Input Bit Number
	BIT2,                   // Security Car Call Latch #33 Input Bit Number
	BIT3,                   // Security Car Call Latch #34 Input Bit Number
	BIT4,                   // Security Car Call Latch #35 Input Bit Number
	BIT5,                   // Security Car Call Latch #36 Input Bit Number
	BIT6,                   // Security Car Call Latch #37 Input Bit Number
	BIT7,                   // Security Car Call Latch #38 Input Bit Number
	BIT8,                   // Security Car Call Latch #39 Input Bit Number
	BIT1,                   // Security Car Call Latch #40 Input Bit Number
	BIT2,                   // Security Car Call Latch #41 Input Bit Number
	BIT3,                   // Security Car Call Latch #42 Input Bit Number
	BIT4,                   // Security Car Call Latch #43 Input Bit Number
	BIT5,                   // Security Car Call Latch #44 Input Bit Number
	BIT6,                   // Security Car Call Latch #45 Input Bit Number
	BIT7,                   // Security Car Call Latch #46 Input Bit Number
	BIT8,                   // Security Car Call Latch #47 Input Bit Number
	// ON 3RD Serial Car Board
	BIT1,                   // Security Car Call Latch #48 Input Bit Number
	BIT2,                   // Security Car Call Latch #59 Input Bit Number
	BIT3,                   // Security Car Call Latch #50 Input Bit Number
	BIT4,                   // Security Car Call Latch #51 Input Bit Number
	BIT5,                   // Security Car Call Latch #52 Input Bit Number
	BIT6,                   // Security Car Call Latch #53 Input Bit Number
	BIT7,                   // Security Car Call Latch #54 Input Bit Number
	BIT8,                   // Security Car Call Latch #55 Input Bit Number
	BIT1,                   // Security Car Call Latch #56 Input Bit Number
	BIT2,                   // Security Car Call Latch #57 Input Bit Number
	BIT3,                   // Security Car Call Latch #58 Input Bit Number
	BIT4,                   // Security Car Call Latch #59 Input Bit Number
	BIT5,                   // Security Car Call Latch #60 Input Bit Number
};

extern const unsigned char sccld[61] = 
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	21,                     // Security Car Call Latch #1 Input Array Number
	21,                     // Security Car Call Latch #2 Input Array Number
	21,                     // Security Car Call Latch #3 Input Array Number
	21,                     // Security Car Call Latch #4 Input Array Number
	21,                     // Security Car Call Latch #5 Input Array Number
	21,                     // Security Car Call Latch #6 Input Array Number
	21,                     // Security Car Call Latch #7 Input Array Number
	21,                     // Security Car Call Latch #8 Input Array Number
	22,                     // Security Car Call Latch #9 Input Array Number
	22,                     // Security Car Call Latch #10 Input Array Number
	22,                     // Security Car Call Latch #11 Input Array Number
	22,                     // Security Car Call Latch #12 Input Array Number
	22,                     // Security Car Call Latch #13 Input Array Number
	22,                     // Security Car Call Latch #14 Input Array Number
	22,                     // Security Car Call Latch #15 Input Array Number
	22,                     // Security Car Call Latch #16 Input Array Number
	23,                     // Security Car Call Latch #17 Input Array Number
	23,                     // Security Car Call Latch #18 Input Array Number
	23,                     // Security Car Call Latch #19 Input Array Number
	23,                     // Security Car Call Latch #20 Input Array Number
	23,                     // Security Car Call Latch #21 Input Array Number
	23,                     // Security Car Call Latch #22 Input Array Number
	23,                     // Security Car Call Latch #23 Input Array Number
	// ON 2nd Serial Car Board
	99,                     // Security Car Call Latch #24 Input Array Number
	99,                     // Security Car Call Latch #25 Input Array Number
	99,                     // Security Car Call Latch #26 Input Array Number
	99,                     // Security Car Call Latch #27 Input Array Number
	99,                     // Security Car Call Latch #28 Input Array Number
	99,                     // Security Car Call Latch #29 Input Array Number
	99,                     // Security Car Call Latch #30 Input Array Number
	99,                     // Security Car Call Latch #31 Input Array Number
	100,                    // Security Car Call Latch #32 Input Array Number
	100,                    // Security Car Call Latch #33 Input Array Number
	100,                    // Security Car Call Latch #34 Input Array Number
	100,                    // Security Car Call Latch #35 Input Array Number
	100,                    // Security Car Call Latch #36 Input Array Number
	100,                    // Security Car Call Latch #37 Input Array Number
	100,                    // Security Car Call Latch #38 Input Array Number
	100,                    // Security Car Call Latch #39 Input Array Number
	101,                    // Security Car Call Latch #40 Input Array Number
	101,                    // Security Car Call Latch #41 Input Array Number
	101,                    // Security Car Call Latch #42 Input Array Number
	101,                    // Security Car Call Latch #43 Input Array Number
	101,                    // Security Car Call Latch #44 Input Array Number
	101,                    // Security Car Call Latch #45 Input Array Number
	101,                    // Security Car Call Latch #46 Input Array Number
	101,                    // Security Car Call Latch #47 Input Array Number
	// ON 3rd Serial Car Board
	105,                    // Security Car Call Latch #48 Input Array Number
	105,                    // Security Car Call Latch #49 Input Array Number
	105,                    // Security Car Call Latch #50 Input Array Number
	105,                    // Security Car Call Latch #51 Input Array Number
	105,                    // Security Car Call Latch #52 Input Array Number
	105,                    // Security Car Call Latch #53 Input Array Number
	105,                    // Security Car Call Latch #54 Input Array Number
	105,                    // Security Car Call Latch #55 Input Array Number
	106,                    // Security Car Call Latch #56 Input Array Number
	106,                    // Security Car Call Latch #57 Input Array Number
	106,                    // Security Car Call Latch #58 Input Array Number
	106,                    // Security Car Call Latch #59 Input Array Number
	106,                    // Security Car Call Latch #60 Input Array Number
};

extern const int16 sccrl[61] =
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BRD12A,                  // Rear Security Car Call Latch  #1 Output Board Address
	BRD12A,                  // Rear Security Car Call Latch  #2 Output Board Address
	BRD12A,                  // Rear Security Car Call Latch  #3 Output Board Address
	BRD12A,                  // Rear Security Car Call Latch  #4 Output Board Address
	BRD12A,                  // Rear Security Car Call Latch  #5 Output Board Address
	BRD12A,                  // Rear Security Car Call Latch  #6 Output Board Address
	BRD12A,                  // Rear Security Car Call Latch  #7 Output Board Address
	BRD12A,                  // Rear Security Car Call Latch  #8 Output Board Address
	BRD12B,                  // Rear Security Car Call Latch  #9 Output Board Address
	BRD12B,                  // Rear Security Car Call Latch  #10 Output  Board Address
	BRD12B,                  // Rear Security Car Call Latch  #11 Output  Board Address
	BRD12B,                  // Rear Security Car Call Latch  #12 Output Board Address
	BRD12B,                  // Rear Security Car Call Latch  #13 Output Board Address
	BRD12B,                  // Rear Security Car Call Latch  #14 Output Board Address
	BRD12B,                  // Rear Security Car Call Latch  #15 Output Board Address
	BRD12B,                  // Rear Security Car Call Latch  #16 Output Board Address
	BRD12C,                  // Rear Security Car Call Latch  #17 Output Board Address
	BRD12C,                 // Rear Security Car Call Latch #18 Output Board Address
	BRD12C,                 // Rear Security Car Call Latch #19 Output Board Address
	BRD12C,                 // Rear Security Car Call Latch #20 Output Board Address
	BRD12C,                 // Rear Security Car Call Latch #21 Output Board Address
	BRD12C,                 // Rear Security Car Call Latch #22 Output Board Address
	BRD12C,                 // Rear Security Car Call Latch #23 Output Board Address
	// ON 2nd Serial Car Board
	BRD38A,                 // Rear Security Car Call Latch #24 Output Board Address
	BRD38A,                 // Rear Security Car Call Latch #25 Output Board Address
	BRD38A,                 // Rear Security Car Call Latch #26 Output Board Address
	BRD38A,                 // Rear Security Car Call Latch #27 Output Board Address
	BRD38A,                 // Rear Security Car Call Latch #28 Output Board Address
	BRD38A,                 // Rear Security Car Call Latch #29 Output Board Address
	BRD38A,                 // Rear Security Car Call Latch #30 Output Board Address
	BRD38A,                 // Rear Security Car Call Latch #31 Output Board Address
	BRD38B,                 // Rear Security Car Call Latch #32 Output Board Address
	BRD38B,                 // Rear Security Car Call Latch #33 Output Board Address
	BRD38B,                 // Rear Security Car Call Latch #34 Output Board Address
	BRD38B,                 // Rear Security Car Call Latch #35 Output Board Address
	BRD38B,                 // Rear Security Car Call Latch #36 Output Board Address
	BRD38B,                 // Rear Security Car Call Latch #37 Output Board Address
	BRD38B,                 // Rear Security Car Call Latch #38 Output Board Address
	BRD38B,                 // Rear Security Car Call Latch #39 Output Board Address
	BRD38C,                 // Rear Security Car Call Latch #40 Output Board Address
	BRD38C,                 // Rear Security Car Call Latch #41 Output Board Address
	BRD38C,                 // Rear Security Car Call Latch #42 Output Board Address
	BRD38C,                 // Rear Security Car Call Latch #43 Output Board Address
	BRD38C,                 // Rear Security Car Call Latch #44 Output Board Address
	BRD38C,                 // Rear Security Car Call Latch #45 Output Board Address
	BRD38C,                 // Rear Security Car Call Latch #46 Output Board Address
	BRD38C,                 // Rear Security Car Call Latch #47 Output Board Address
	// ON 3rd Serial Car Board
	BRD40A,                 // Rear Security Car Call Latch #48 Output Board Address
	BRD40A,                 // Rear Security Car Call Latch #49 Output Board Address
	BRD40A,                 // Rear Security Car Call Latch #50 Output Board Address
	BRD40A,                 // Rear Security Car Call Latch #51 Output Board Address
	BRD40A,                 // Rear Security Car Call Latch #52 Output Board Address
	BRD40A,                 // Rear Security Car Call Latch #53 Output Board Address
	BRD40A,                 // Rear Security Car Call Latch #54 Output Board Address
	BRD40A,                 // Rear Security Car Call Latch #55 Output Board Address
	BRD40B,                 // Rear Security Car Call Latch #56 Output Board Address
	BRD40B,                 // Rear Security Car Call Latch #57 Output Board Address
	BRD40B,                 // Rear Security Car Call Latch #58 Output Board Address
	BRD40B,                 // Rear Security Car Call Latch #59 Output Board Address
	BRD40B,                 // Rear Security Car Call Latch #60 Output Board Address
};
extern const unsigned char sccrlb[61] = 
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	BIT1,                   // Rear Security Car Call Latch #1 Output Bit Number
	BIT2,                   // Rear Security Car Call Latch #2 Output Bit Number
	BIT3,                   // Rear Security Car Call Latch #3 Output Bit Number
	BIT4,                   // Rear Security Car Call Latch #4 Output Bit Number
	BIT5,                   // Rear Security Car Call Latch #5 Output Bit Number
	BIT6,                   // Rear Security Car Call Latch #6 Output Bit Number
	BIT7,                   // Rear Security Car Call Latch #7 Output Bit Number
	BIT8,                   // Rear Security Car Call Latch #8 Output Bit Number
	BIT1,                   // Rear Security Car Call Latch #9 Output Bit Number
	BIT2,                   // Rear Security Car Call Latch #10 Output Bit Number
	BIT3,                   // Rear Security Car Call Latch #11 Output Bit Number
	BIT4,                   // Rear Security Car Call Latch #12 Output Bit Number
	BIT5,                   // Rear Security Car Call Latch #13 Output Bit Number
	BIT6,                   // Rear Security Car Call Latch #14 Output Bit Number
	BIT7,                   // Rear Security Car Call Latch #15 Output Bit Number
	BIT8,                   // Rear Security Car Call Latch #16 Output Bit Number
	BIT1,                   // Rear Security Car Call Latch #17 Output Bit Number
	BIT2,                   // Rear Security Car Call Latch #18 Output Bit Number
	BIT3,                   // Rear Security Car Call Latch #19 Output Bit Number
	BIT4,                   // Rear Security Car Call Latch #20 Output Bit Number
	BIT5,                   // Rear Security Car Call Latch #21 Output Bit Number
	BIT6,                   // Rear Security Car Call Latch #22 Output Bit Number
	BIT7,                   // Rear Security Car Call Latch #23 Output Bit Number
	// ON 2nd Serial Car Board
	BIT1,                   // Rear Security Car Call Latch #24 Output Bit Number
	BIT2,                   // Rear Security Car Call Latch #25 Output Bit Number
	BIT3,                   // Rear Security Car Call Latch #26 Output Bit Number
	BIT4,                   // Rear Security Car Call Latch #27 Output Bit Number
	BIT5,                   // Rear Security Car Call Latch #28 Output Bit Number
	BIT6,                   // Rear Security Car Call Latch #29 Output Bit Number
	BIT7,                   // Rear Security Car Call Latch #30 Output Bit Number
	BIT8,                   // Rear Security Car Call Latch #31 Output Bit Number
	BIT1,                   // Rear Security Car Call Latch #32 Output Bit Number
	BIT2,                   // Rear Security Car Call Latch #33 Output Bit Number
	BIT3,                   // Rear Security Car Call Latch #34 Output Bit Number
	BIT4,                   // Rear Security Car Call Latch #35 Output Bit Number
	BIT5,                   // Rear Security Car Call Latch #36 Output Bit Number
	BIT6,                   // Rear Security Car Call Latch #37 Output Bit Number
	BIT7,                   // Rear Security Car Call Latch #38 Output Bit Number
	BIT8,                   // Rear Security Car Call Latch #39 Output Bit Number
	BIT1,                   // Rear Security Car Call Latch #40 Output Bit Number
	BIT2,                   // Rear Security Car Call Latch #41 Output Bit Number
	BIT3,                   // Rear Security Car Call Latch #42 Output Bit Number
	BIT4,                   // Rear Security Car Call Latch #43 Output Bit Number
	BIT5,                   // Rear Security Car Call Latch #44 Output Bit Number
	BIT6,                   // Rear Security Car Call Latch #45 Output Bit Number
	BIT7,                   // Rear Security Car Call Latch #46 Output Bit Number
	BIT8,                   // Rear Security Car Call Latch #47 Output Bit Number
	// ON 3rd Serial Car Board
	BIT1,                   // Rear Security Car Call Latch #48 Output Bit Number
	BIT2,                   // Rear Security Car Call Latch #49 Output Bit Number
	BIT3,                   // Rear Security Car Call Latch #50 Output Bit Number
	BIT4,                   // Rear Security Car Call Latch #51 Output Bit Number
	BIT5,                   // Rear Security Car Call Latch #52 Output Bit Number
	BIT6,                   // Rear Security Car Call Latch #53 Output Bit Number
	BIT7,                   // Rear Security Car Call Latch #54 Output Bit Number
	BIT8,                   // Rear Security Car Call Latch #55 Output Bit Number
	BIT1,                   // Rear Security Car Call Latch #56 Output Bit Number
	BIT2,                   // Rear Security Car Call Latch #57 Output Bit Number
	BIT3,                   // Rear Security Car Call Latch #58 Output Bit Number
	BIT4,                   // Rear Security Car Call Latch #59 Output Bit Number
	BIT5,                   // Rear Security Car Call Latch #60 Output Bit Number
};

extern const unsigned char sccrld[61] = 
{			  // Used with serial car call
	0,                      // Not Used
	// ON 1st Serial Car Board
	33,                     // Rear Security Car Call Latch #1 Output Array Number
	33,                     // Rear Security Car Call Latch #2 Output Array Number
	33,                     // Rear Security Car Call Latch #3 Output Array Number
	33,                     // Rear Security Car Call Latch #4 Output Array Number
	33,                     // Rear Security Car Call Latch #5 Output Array Number
	33,                     // Rear Security Car Call Latch #6 Output Array Number
	33,                     // Rear Security Car Call Latch #7 Output Array Number
	33,                     // Rear Security Car Call Latch #8 Output Array Number
	34,                     // Rear Security Car Call Latch #9 Output Array Number
	34,                     // Rear Security Car Call Latch #10 Output Array Number
	34,                     // Rear Security Car Call Latch #11 Output Array Number
	34,                     // Rear Security Car Call Latch #12 Output Array Number
	34,                     // Rear Security Car Call Latch #13 Output Array Number
	34,                     // Rear Security Car Call Latch #14 Output Array Number
	34,                     // Rear Security Car Call Latch #15 Output Array Number
	34,                     // Rear Security Car Call Latch #16 Output Array Number
	35,                     // Rear Security Car Call Latch #17 Output Array Number
	35,                     // Rear Security Car Call Latch #18 Output Array Number
	35,                     // Rear Security Car Call Latch #19 Output Array Number
	35,                     // Rear Security Car Call Latch #20 Output Array Number
	35,                     // Rear Security Car Call Latch #21 Output Array Number
	35,                     // Rear Security Car Call Latch #22 Output Array Number
	35,                     // Rear Security Car Call Latch #23 Output Array Number
	// ON 2nd Serial Car Board
	111,                    // Rear Security Car Call Latch #24 Output Array Number
	111,                    // Rear Security Car Call Latch #25 Output Array Number
	111,                    // Rear Security Car Call Latch #26 Output Array Number
	111,                    // Rear Security Car Call Latch #27 Output Array Number
	111,                    // Rear Security Car Call Latch #28 Output Array Number
	111,                    // Rear Security Car Call Latch #29 Output Array Number
	111,                    // Rear Security Car Call Latch #30 Output Array Number
	111,                    // Rear Security Car Call Latch #31 Output Array Number
	112,                    // Rear Security Car Call Latch #32 Output Array Number
	112,                    // Rear Security Car Call Latch #33 Output Array Number
	112,                    // Rear Security Car Call Latch #34 Output Array Number
	112,                    // Rear Security Car Call Latch #35 Output Array Number
	112,                    // Rear Security Car Call Latch #36 Output Array Number
	112,                    // Rear Security Car Call Latch #37 Output Array Number
	112,                    // Rear Security Car Call Latch #38 Output Array Number
	112,                    // Rear Security Car Call Latch #39 Output Array Number
	113,                    // Rear Security Car Call Latch #40 Output Array Number
	113,                    // Rear Security Car Call Latch #41 Output Array Number
	113,                    // Rear Security Car Call Latch #42 Output Array Number
	113,                    // Rear Security Car Call Latch #43 Output Array Number
	113,                    // Rear Security Car Call Latch #44 Output Array Number
	113,                    // Rear Security Car Call Latch #45 Output Array Number
	113,                    // Rear Security Car Call Latch #46 Output Array Number
	113,                    // Rear Security Car Call Latch #47 Output Array Number
	// ON 3rd Serial Car Board
	117,                    // Rear Security Car Call Latch #48 Output Array Number
	117,                    // Rear Security Car Call Latch #49 Output Array Number
	117,                    // Rear Security Car Call Latch #50 Output Array Number
	117,                    // Rear Security Car Call Latch #51 Output Array Number
	117,                    // Rear Security Car Call Latch #52 Output Array Number
	117,                    // Rear Security Car Call Latch #53 Output Array Number
	117,                    // Rear Security Car Call Latch #54 Output Array Number
	117,                    // Rear Security Car Call Latch #55 Output Array Number
	118,                    // Rear Security Car Call Latch #56 Output Array Number
	118,                    // Rear Security Car Call Latch #57 Output Array Number
	118,                    // Rear Security Car Call Latch #58 Output Array Number
	118,                    // Rear Security Car Call Latch #59 Output Array Number
	118,                    // Rear Security Car Call Latch #60 Output Array Number
};

extern const int16 dc[6] = 
{			  // Used for 5 floors or less otherwise use serial hall calls
	0,                      // Not Used
	0,                      // Not Used
	// ON AE1010 BOARD
	BRD2C,                  // Down Hall Call #2 Input Board Address
	BRD2C,                  // Down Hall Call #3 Input Board Address
	BRD2C,                  // Down Hall Call #4 Input Board Address
	BRD2C,                  // Down Hall Call #5 Input Board Address
};

extern const unsigned char dcb[6] = 
{	   // Used for 5 floors or less otherwise use serial hall calls
	0,                      // Not Used
	0,                      // Not Used
	// ON AE1010 BOARD
	BIT5,                   // Down Hall Call #2 Input Bit Number
	BIT6,                   // Down Hall Call #3 Input Bit Number
	BIT7,                   // Down Hall Call #4 Input Bit Number
	BIT8,                   // Down Hall Call #5 Input Bit Number
};

extern const unsigned char dcd[6] = 
{	 // Used for 7 floors or less otherwise use serial hall calls
	0,                      // Not Used
	0,                      // Not Used
	// ON AE1010 BOARD
	5,                      // Down Hall Call #2 Input Array Number
	5,                      // Down Hall Call #3 Input Array Number
	5,                      // Down Hall Call #4 Input Array Number
	5,                      // Down Hall Call #5 Input Array Number
};

extern const int16 dcl[6] = 
{	   // Used for 5 floors or less otherwise use serial hall calls
	0,                      // Not Used
	0,                      // Not Used
	// ON AE1010 BOARD
	BRD3C,                  // Down Hall Call Light #2 Output Board Address
	BRD3C,                  // Down Hall Call Light #3 Output Board Address
	BRD3C,                  // Down Hall Call Light #4 Output Board Address
	BRD3C,                  // Down Hall Call Light #5 Output Board Address
};

extern const unsigned char dclb[6] = 
{		// Used for 5 floors or less otherwise use serial hall calls
	0,                      // Not Used
	0,                      // Not Used
	// ON AE1010 BOARD
	BIT5,                   // Down Hall Call Light #2 Output Bit Number
	BIT6,                   // Down Hall Call Light #3 Output Bit Number
	BIT7,                   // Down Hall Call Light #4 Output Bit Number
	BIT8,                   // Down Hall Call Light #5 Output Bit Number
};

extern const unsigned char dcld[6] = 
{	   // Used for 5 floors or less otherwise use serial hall calls
	0,                      // Not Used
	0,                      // Not Used
	// ON AE1010 BOARD
	8,                     // Down Hall Call Light #2 Output Array Number
	8,                     // Down Hall Call Light #3 Output Array Number
	8,                     // Down Hall Call Light #4 Output Array Number
	8,                     // Down Hall Call Light #5 Output Array Number
};


extern const int16 uc[6] = 
{		// Used for 5 floors or less otherwise use serial hall call board
	0,                      // Not Used
	// ON AE1010 BOARD
	BRD2C,                  // Up Hall Call #1 Input Board Address
	BRD2C,                  // Up Hall Call #2 Input Board Address
	BRD2C,                  // Up Hall Call #3 Input Board Address
	BRD2C,                  // Up Hall Call #4 Input Board Address
	0                       // Not Used
};

extern const unsigned char ucb[6] = 
{		// Used for 5 floors or less otherwise use serial hall call board
	0,                      // Not Used
	// ON AE1010 BOARD
	BIT1,                   // Up Hall Call #1 Input Bit Number
	BIT2,                   // Up Hall Call #2 Input Bit Number
	BIT3,                   // Up Hall Call #3 Input Bit Number
	BIT4,                   // Up Hall Call #4 Input Bit Number
	0                       // Not Used
};
extern const unsigned char ucd[6] = 
{		// Used for 5 floors or less otherwise use serial hall call board
	0,                      // Not Used
	// ON AE1010 BOARD
	5,                      // Up Hall Call #1 Input Array Number
	5,                      // Up Hall Call #2 Input Array Number
	5,                      // Up Hall Call #3 Input Array Number
	5,                      // Up Hall Call #4 Input Array Number
	0                       // Not Used
};

extern const int16 ucl[6] = 
{		// Used for 7 floors or less otherwise use serial hall call board
	0,                      // Not Used
	// ON AE1010 BOARD
	BRD3C,                  // Up Hall Call Light #1 Output Board Address
	BRD3C,                  // Up Hall Call Light #2 Output Board Address
	BRD3C,                  // Up Hall Call Light #3 Output Board Address
	BRD3C,                  // Up Hall Call Light #4 Output Board Address
	0                       // Not Used
};

extern const unsigned char uclb[6] = 
{		// Used for 5 floors or less otherwise use serial hall call board
	0,                      // Not Used
	// ON AE1010 BOARD
	BIT1,                   // Up Hall Call Light #1 Output Bit Number
	BIT2,                   // Up Hall Call Light #2 Output Bit Number
	BIT3,                   // Up Hall Call Light #3 Output Bit Number
	BIT4,                   // Up Hall Call Light #4 Output Bit Number
	0                       // Not Used
};

extern const unsigned char ucld[6] = 
{		// Used for 5 floors or less otherwise use serial hall call board
	0,                      // Not Used
	// ON AE1010 BOARD
	8,                     // Up Hall Call Light #1 Output Array Number
	8,                     // Up Hall Call Light #2 Output Array Number
	8,                     // Up Hall Call Light #3 Output Array Number
	8,                     // Up Hall Call Light #4 Output Array Number
	0                       // Not Used
};

extern const int16 ccs[61] = 
{					// Use when using car call comm
	// ON 1st Serial Car Board
	BRD7A,					// SECUR Security enable input
	BRD7A,                 // Car Call Security Input Board Address ccs #1     
	BRD7A,                 // Car Call Security Input Board Address	ccs #2  	   
	BRD7A,                 // Car Call Security Input Board Address	ccs #3     
	BRD7A,                 // Car Call Security Input Board Address	ccs #4      	
	BRD7A,                 // Car Call Security Input Board Address	ccs #5     
	BRD7A,                 // Car Call Security Input Board Address	ccs #6  	   
	BRD7A,                 // Car Call Security Input Board Address	ccs #7  	   
	BRD7B,                 // Car Call Security Input Board Address	ccs #8  	   	
	BRD7B,                 // Car Call Security Input Board Address	ccs #9     
	BRD7B,                 // Car Call Security Input Board Address	ccs #10     
	BRD7B,                 // Car Call Security Input Board Address	ccs #11	   
	BRD7B,                 // Car Call Security Input Board Address	ccs #12	    
	BRD7B,                 // Car Call Security Input Board Address	ccs #13	   
	BRD7B,                 // Car Call Security Input Board Address	ccs #14	    
	BRD7B,                 // Car Call Security Input Board Address	ccs #15	   
	BRD7C,                 // Car Call Security Input Board Address	ccs #16	    
	BRD7C,                 // Car Call Security Input Board Address	ccs #17	   
	BRD7C,                 // Car Call Security Input Board Address	ccs #18		   
	BRD7C,                 // Car Call Security Input Board Address	ccs #19	   
	BRD7C,                 // Car Call Security Input Board Address	ccs #20	    
	BRD7C,                 // Car Call Security Input Board Address	ccs #21	   
	BRD7C,                 // Car Call Security Input Board Address	ccs #22	    
	BRD7C,                 // Car Call Security Input Board Address	ccs #23	   
	// ON 2nd Serial Car Board
	BRD33A,                 // Car Call Security Input Board Address ccs #24	    
	BRD33A,                 // Car Call Security Input Board Address ccs #25	   
	BRD33A,                 // Car Call Security Input Board Address ccs #26	    
	BRD33A,                 // Car Call Security Input Board Address ccs #27	   
	BRD33A,                 // Car Call Security Input Board Address ccs #28	    
	BRD33A,                 // Car Call Security Input Board Address ccs #29	   
	BRD33A,                 // Car Call Security Input Board Address ccs #30	    
	BRD33A,                 // Car Call Security Input Board Address ccs #31	   
	BRD33B,                 // Car Call Security Input Board Address ccs #32	    
	BRD33B,                 // Car Call Security Input Board Address ccs #33	   
	BRD33B,                 // Car Call Security Input Board Address ccs #34	    
	BRD33B,                 // Car Call Security Input Board Address ccs #35	   
	BRD33B,                 // Car Call Security Input Board Address ccs #36	    
	BRD33B,                 // Car Call Security Input Board Address ccs #37	   
	BRD33B,                 // Car Call Security Input Board Address ccs #38	    
	BRD33B,                 // Car Call Security Input Board Address ccs #39	   
	BRD33C,                 // Car Call Security Input Board Address ccs #40	    
	BRD33C,                 // Car Call Security Input Board Address ccs #41	   
	BRD33C,                 // Car Call Security Input Board Address ccs #42	    
	BRD33C,                 // Car Call Security Input Board Address ccs #43	   
	BRD33C,                 // Car Call Security Input Board Address ccs #44	    
	BRD33C,                 // Car Call Security Input Board Address ccs #45	   
	BRD33C,                 // Car Call Security Input Board Address ccs #46	    
	BRD33C,                 // Car Call Security Input Board Address ccs #47	   
	// ON 3rd Serial Car Board
	BRD35A,                 // Car Call Security Input Board Address ccs #48	    
	BRD35A,                 // Car Call Security Input Board Address ccs #49	   
	BRD35A,                 // Car Call Security Input Board Address ccs #50	    
	BRD35A,                 // Car Call Security Input Board Address ccs #51	   
	BRD35A,                 // Car Call Security Input Board Address ccs #52	    
	BRD35A,                 // Car Call Security Input Board Address ccs #53	   
	BRD35A,                 // Car Call Security Input Board Address ccs #54	    
	BRD35A,                 // Car Call Security Input Board Address ccs #55	   
	BRD35B,                 // Car Call Security Input Board Address ccs #56	    
	BRD35B,                 // Car Call Security Input Board Address ccs #57	   
	BRD35B,                 // Car Call Security Input Board Address ccs #58	    
	BRD35B,                 // Car Call Security Input Board Address ccs #59	   
	BRD35B,                 // Car Call Security Input Board Address ccs #60	    
};

extern const unsigned char ccsb[61] = 
{
	// ON 1st Serial Car Board
	BIT1,					// SECUR Security enable input 
	BIT2,                   // Car Call Security Input Bit Number		ccs #1  
	BIT3,					// Car Call Security Input Bit Number		ccs #2  	
	BIT4,                   // Car Call Security Input Bit Number		ccs #3  
	BIT5,                   // Car Call Security Input Bit Number		ccs #4   
	BIT6,					// Car Call Security Input Bit Number		ccs #5  
	BIT7,                   // Car Call Security Input Bit Number		ccs #6  	
	BIT8,                   // Car Call Security Input Bit Number		ccs #7  	
	BIT1,                   // Car Call Security Input Bit Number		ccs #8  	
	BIT2,                   // Car Call Security Input Bit Number		ccs #9  
	BIT3,					// Car Call Security Input Bit Number		ccs #10  
	BIT4,                   // Car Call Security Input Bit Number		ccs #11	
	BIT5,                   // Car Call Security Input Bit Number		ccs #12	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #13	
	BIT7,					// Car Call Security Input Bit Number		ccs #14	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #15	
	BIT1,					// Car Call Security Input Bit Number		ccs #16	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #17	
	BIT3,                   // Car Call Security Input Bit Number		ccs #18		
	BIT4,					// Car Call Security Input Bit Number		ccs #19	
	BIT5,                   // Car Call Security Input Bit Number		ccs #20	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #21	
	BIT7,                   // Car Call Security Input Bit Number		ccs #22	 
	BIT8,					// Car Call Security Input Bit Number		ccs #23	
	// ON 2nd Serial Car Board
	BIT1,                   // Car Call Security Input Bit Number		ccs #24	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #25	
	BIT3,					// Car Call Security Input Bit Number		ccs #26	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #27	
	BIT5,                   // Car Call Security Input Bit Number		ccs #28	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #29	
	BIT7,                   // Car Call Security Input Bit Number		ccs #30	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #31	
	BIT1,                   // Car Call Security Input Bit Number		ccs #32	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #33	
	BIT3,					// Car Call Security Input Bit Number		ccs #34	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #35	
	BIT5,                   // Car Call Security Input Bit Number		ccs #36	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #37	
	BIT7,                   // Car Call Security Input Bit Number		ccs #38	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #39	
	BIT1,                   // Car Call Security Input Bit Number		ccs #40	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #41	
	BIT3,					// Car Call Security Input Bit Number		ccs #42	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #43	
	BIT5,                   // Car Call Security Input Bit Number		ccs #44	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #45	
	BIT7,                   // Car Call Security Input Bit Number		ccs #46	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #47	
	// ON 3rd Serial Car Board
	BIT1,                   // Car Call Security Input Bit Number		ccs #48	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #49	
	BIT3,					// Car Call Security Input Bit Number		ccs #50	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #51	
	BIT5,                   // Car Call Security Input Bit Number		ccs #52	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #53	
	BIT7,                   // Car Call Security Input Bit Number		ccs #54	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #55	
	BIT1,                   // Car Call Security Input Bit Number		ccs #56	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #57	
	BIT3,					// Car Call Security Input Bit Number		ccs #58	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #59	
	BIT5,                   // Car Call Security Input Bit Number		ccs #60	 
};


extern const unsigned char ccsd[61] = 
{
	// ON 1st Serial Car Board
	18,						// SECUR Security enable input
	18,                     // Car Call Security Input Array Number		ccs #1  
	18,                     // Car Call Security Input Array Number		ccs #2  
	18,                     // Car Call Security Input Array Number		ccs #3  
	18,                     // Car Call Security Input Array Number		ccs #4  
	18,                     // Car Call Security Input Array Number		ccs #5  
	18,                     // Car Call Security Input Array Number		ccs #6  
	18,                     // Car Call Security Input Array Number		ccs #7  
	19,                     // Car Call Security Input Array Number		ccs #8  
	19,                     // Car Call Security Input Array Number		ccs #9  
	19,                     // Car Call Security Input Array Number		ccs #10 
	19,                     // Car Call Security Input Array Number		ccs #11	
	19,                     // Car Call Security Input Array Number		ccs #12	
	19,                     // Car Call Security Input Array Number		ccs #13	
	19,                     // Car Call Security Input Array Number		ccs #14	
	19,                     // Car Call Security Input Array Number		ccs #15	
	20,                     // Car Call Security Input Array Number		ccs #16	
	20,                     // Car Call Security Input Array Number		ccs #17	
	20,                     // Car Call Security Input Array Number		ccs #18	
	20,                     // Car Call Security Input Array Number		ccs #19	
	20,                     // Car Call Security Input Array Number		ccs #20	
	20,                     // Car Call Security Input Array Number		ccs #21	
	20,                     // Car Call Security Input Array Number		ccs #22	
	20,                     // Car Call Security Input Array Number		ccs #23	
	// ON 2nd Serial Car Board
	96,                     // Car Call Security Input Array Number		ccs #24	
	96,                     // Car Call Security Input Array Number		ccs #25	
	96,                     // Car Call Security Input Array Number		ccs #26	
	96,                     // Car Call Security Input Array Number		ccs #27	
	96,                     // Car Call Security Input Array Number		ccs #28	
	96,                     // Car Call Security Input Array Number		ccs #29	
	96,                     // Car Call Security Input Array Number		ccs #30	
	96,                     // Car Call Security Input Array Number		ccs #31	
	97,                     // Car Call Security Input Array Number		ccs #32	
	97,                     // Car Call Security Input Array Number		ccs #33	
	97,                     // Car Call Security Input Array Number		ccs #34	
	97,                     // Car Call Security Input Array Number		ccs #35	
	97,                     // Car Call Security Input Array Number		ccs #36	
	97,                     // Car Call Security Input Array Number		ccs #37	
	97,                     // Car Call Security Input Array Number		ccs #38	
	97,                     // Car Call Security Input Array Number		ccs #39	
	98,                     // Car Call Security Input Array Number		ccs #40	
	98,                     // Car Call Security Input Array Number		ccs #41	
	98,                     // Car Call Security Input Array Number		ccs #42	
	98,                     // Car Call Security Input Array Number		ccs #43	
	98,                     // Car Call Security Input Array Number		ccs #44	
	98,                     // Car Call Security Input Array Number		ccs #45	
	98,                     // Car Call Security Input Array Number		ccs #46	
	98,                     // Car Call Security Input Array Number		ccs #47
	// ON 3rd Serial Car Board
	102,                    // Car Call Security Input Array Number		ccs #48	
	102,                    // Car Call Security Input Array Number		ccs #49	
	102,                    // Car Call Security Input Array Number		ccs #50	
	102,                    // Car Call Security Input Array Number		ccs #51	
	102,                    // Car Call Security Input Array Number		ccs #52	
	102,                    // Car Call Security Input Array Number		ccs #53	
	102,                    // Car Call Security Input Array Number		ccs #54	
    102,                    // Car Call Security Input Array Number		ccs #55	
	103,                    // Car Call Security Input Array Number		ccs #56	
	103,                    // Car Call Security Input Array Number		ccs #57	
	103,                    // Car Call Security Input Array Number		ccs #58	
	103,                    // Car Call Security Input Array Number		ccs #59	
	103,                    // Car Call Security Input Array Number		ccs #60	
};


extern const int16 rccs[61] = 
{					// Use when using car call comm
	// ON 3rd Serial Car Board
	BRD11A,					// RSECUR Rear Security Input
	BRD11A,                 // Car Call Security Input Board Address 	ccs #1     
	BRD11A,                 // Car Call Security Input Board Address	ccs #2  	   
	BRD11A,                 // Car Call Security Input Board Address	ccs #3     
	BRD11A,                 // Car Call Security Input Board Address	ccs #4      	
	BRD11A,                 // Car Call Security Input Board Address	ccs #5     
	BRD11A,                 // Car Call Security Input Board Address	ccs #6  	   
	BRD11A,                 // Car Call Security Input Board Address	ccs #7  	   
	BRD11B,                 // Car Call Security Input Board Address	ccs #8  	   	
	BRD11B,                 // Car Call Security Input Board Address	ccs #9     
	BRD11B,                 // Car Call Security Input Board Address	ccs #10     
	BRD11B,                 // Car Call Security Input Board Address	ccs #11	   
	BRD11B,                 // Car Call Security Input Board Address	ccs #12	    
	BRD11B,                 // Car Call Security Input Board Address	ccs #13	   
	BRD11B,                 // Car Call Security Input Board Address	ccs #14	    
	BRD11B,                 // Car Call Security Input Board Address	ccs #15	   
	BRD11C,                 // Car Call Security Input Board Address	ccs #16	    
	BRD11C,                 // Car Call Security Input Board Address	ccs #17	   
	BRD11C,                 // Car Call Security Input Board Address	ccs #18		   
	BRD11C,                 // Car Call Security Input Board Address	ccs #19	   
	BRD11C,                 // Car Call Security Input Board Address	ccs #20	    
	BRD11C,                 // Car Call Security Input Board Address	ccs #21	   
	BRD11C,                 // Car Call Security Input Board Address	ccs #22	    
	BRD11C,                 // Car Call Security Input Board Address	ccs #23	   
	// ON 2nd Serial Car Board
	BRD37A,                 // Car Call Security Input Board Address ccs #24	    
	BRD37A,                 // Car Call Security Input Board Address ccs #25	   
	BRD37A,                 // Car Call Security Input Board Address ccs #26	    
	BRD37A,                 // Car Call Security Input Board Address ccs #27	   
	BRD37A,                 // Car Call Security Input Board Address ccs #28	    
	BRD37A,                 // Car Call Security Input Board Address ccs #29	   
	BRD37A,                 // Car Call Security Input Board Address ccs #30	    
	BRD37A,                 // Car Call Security Input Board Address ccs #31	   
	BRD37B,                 // Car Call Security Input Board Address ccs #32	    
	BRD37B,                 // Car Call Security Input Board Address ccs #33	   
	BRD37B,                 // Car Call Security Input Board Address ccs #34	    
	BRD37B,                 // Car Call Security Input Board Address ccs #35	   
	BRD37B,                 // Car Call Security Input Board Address ccs #36	    
	BRD37B,                 // Car Call Security Input Board Address ccs #37	   
	BRD37B,                 // Car Call Security Input Board Address ccs #38	    
	BRD37B,                 // Car Call Security Input Board Address ccs #39	   
	BRD37C,                 // Car Call Security Input Board Address ccs #40	    
	BRD37C,                 // Car Call Security Input Board Address ccs #41	   
	BRD37C,                 // Car Call Security Input Board Address ccs #42	    
	BRD37C,                 // Car Call Security Input Board Address ccs #43	   
	BRD37C,                 // Car Call Security Input Board Address ccs #44	    
	BRD37C,                 // Car Call Security Input Board Address ccs #45	   
	BRD37C,                 // Car Call Security Input Board Address ccs #46	    
	BRD37C,                 // Car Call Security Input Board Address ccs #47	   
	// ON 3rd Serial Car Board
	BRD39A,                 // Car Call Security Input Board Address ccs #48	    
	BRD39A,                 // Car Call Security Input Board Address ccs #49	   
	BRD39A,                 // Car Call Security Input Board Address ccs #50	    
	BRD39A,                 // Car Call Security Input Board Address ccs #51	   
	BRD39A,                 // Car Call Security Input Board Address ccs #52	    
	BRD39A,                 // Car Call Security Input Board Address ccs #53	   
	BRD39A,                 // Car Call Security Input Board Address ccs #54	    
	BRD39A,                 // Car Call Security Input Board Address ccs #55	   
	BRD39B,                 // Car Call Security Input Board Address ccs #56	    
	BRD39B,                 // Car Call Security Input Board Address ccs #57	   
	BRD39B,                 // Car Call Security Input Board Address ccs #58	    
	BRD39B,                 // Car Call Security Input Board Address ccs #59	   
	BRD39B,                 // Car Call Security Input Board Address ccs #60	    
};

extern const unsigned char rccsb[61] = 
{
	// ON 3rd Serial Car Board
	BIT1,					// RSECUR Security enable input
	BIT2,                   // Car Call Security Input Bit Number		ccs #1  
	BIT3,					// Car Call Security Input Bit Number		ccs #2  	
	BIT4,                   // Car Call Security Input Bit Number		ccs #3  
	BIT5,                   // Car Call Security Input Bit Number		ccs #4   
	BIT6,					// Car Call Security Input Bit Number		ccs #5  
	BIT7,                   // Car Call Security Input Bit Number		ccs #6  	
	BIT8,                   // Car Call Security Input Bit Number		ccs #7  	
	BIT1,                   // Car Call Security Input Bit Number		ccs #8  	
	BIT2,                   // Car Call Security Input Bit Number		ccs #9  
	BIT3,					// Car Call Security Input Bit Number		ccs #10  
	BIT4,                   // Car Call Security Input Bit Number		ccs #11	
	BIT5,                   // Car Call Security Input Bit Number		ccs #12	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #13	
	BIT7,					// Car Call Security Input Bit Number		ccs #14	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #15	
	BIT1,					// Car Call Security Input Bit Number		ccs #16	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #17	
	BIT3,                   // Car Call Security Input Bit Number		ccs #18		
	BIT4,					// Car Call Security Input Bit Number		ccs #19	
	BIT5,                   // Car Call Security Input Bit Number		ccs #20	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #21	
	BIT7,                   // Car Call Security Input Bit Number		ccs #22	 
	BIT8,					// Car Call Security Input Bit Number		ccs #23	
	// ON 2nd Serial Car Board
	BIT1,                   // Car Call Security Input Bit Number		ccs #24	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #25	
	BIT3,					// Car Call Security Input Bit Number		ccs #26	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #27	
	BIT5,                   // Car Call Security Input Bit Number		ccs #28	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #29	
	BIT7,                   // Car Call Security Input Bit Number		ccs #30	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #31	
	BIT1,                   // Car Call Security Input Bit Number		ccs #32	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #33	
	BIT3,					// Car Call Security Input Bit Number		ccs #34	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #35	
	BIT5,                   // Car Call Security Input Bit Number		ccs #36	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #37	
	BIT7,                   // Car Call Security Input Bit Number		ccs #38	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #39	
	BIT1,                   // Car Call Security Input Bit Number		ccs #40	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #41	
	BIT3,					// Car Call Security Input Bit Number		ccs #42	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #43	
	BIT5,                   // Car Call Security Input Bit Number		ccs #44	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #45	
	BIT7,                   // Car Call Security Input Bit Number		ccs #46	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #47	
	// ON 3rd Serial Car Board
	BIT1,                   // Car Call Security Input Bit Number		ccs #48	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #49	
	BIT3,					// Car Call Security Input Bit Number		ccs #50	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #51	
	BIT5,                   // Car Call Security Input Bit Number		ccs #52	 
	BIT6,                   // Car Call Security Input Bit Number		ccs #53	
	BIT7,                   // Car Call Security Input Bit Number		ccs #54	 
	BIT8,                   // Car Call Security Input Bit Number		ccs #55	
	BIT1,                   // Car Call Security Input Bit Number		ccs #56	 
	BIT2,                   // Car Call Security Input Bit Number		ccs #57	
	BIT3,					// Car Call Security Input Bit Number		ccs #58	 
	BIT4,                   // Car Call Security Input Bit Number		ccs #59	
	BIT5,                   // Car Call Security Input Bit Number		ccs #60	 
};


extern const unsigned char rccsd[61] = 
{
	// ON 3RD Serial Car Board
	30,		 			// RSECUR Security enable input
	30,                    // Car Call Security Input Array Number		ccs #1  
	30,                    // Car Call Security Input Array Number		ccs #2  
	30,                    // Car Call Security Input Array Number		ccs #3  
	30,                    // Car Call Security Input Array Number		ccs #4  
	30,                    // Car Call Security Input Array Number		ccs #5  
	30,                    // Car Call Security Input Array Number		ccs #6  
	30,                    // Car Call Security Input Array Number		ccs #7  
	31,                    // Car Call Security Input Array Number		ccs #8  
	31,                    // Car Call Security Input Array Number		ccs #9  
	31,                    // Car Call Security Input Array Number		ccs #10 
	31,                    // Car Call Security Input Array Number		ccs #11	
	31,                    // Car Call Security Input Array Number		ccs #12	
	31,                    // Car Call Security Input Array Number		ccs #13	
	31,                    // Car Call Security Input Array Number		ccs #14	
	31,                    // Car Call Security Input Array Number		ccs #15	
	32,                    // Car Call Security Input Array Number		ccs #16	
	32,                    // Car Call Security Input Array Number		ccs #17	
	32,                    // Car Call Security Input Array Number		ccs #18	
	32,                    // Car Call Security Input Array Number		ccs #19	
	32,                    // Car Call Security Input Array Number		ccs #20	
	32,                    // Car Call Security Input Array Number		ccs #21	
	32,                    // Car Call Security Input Array Number		ccs #22	
	32,                    // Car Call Security Input Array Number		ccs #23	
	// ON 2nd Serial Car Board
	108,                    // Car Call Security Input Array Number		ccs #24	
	108,                    // Car Call Security Input Array Number		ccs #25	
	108,                    // Car Call Security Input Array Number		ccs #26	
	108,                    // Car Call Security Input Array Number		ccs #27	
	108,                    // Car Call Security Input Array Number		ccs #28	
	108,                    // Car Call Security Input Array Number		ccs #29	
	108,                    // Car Call Security Input Array Number		ccs #30	
	108,                    // Car Call Security Input Array Number		ccs #31	
	109,                    // Car Call Security Input Array Number		ccs #32	
	109,                    // Car Call Security Input Array Number		ccs #33	
	109,                    // Car Call Security Input Array Number		ccs #34	
	109,                    // Car Call Security Input Array Number		ccs #35	
	109,                    // Car Call Security Input Array Number		ccs #36	
	109,                    // Car Call Security Input Array Number		ccs #37	
	109,                    // Car Call Security Input Array Number		ccs #38	
	109,                    // Car Call Security Input Array Number		ccs #39	
	110,                    // Car Call Security Input Array Number		ccs #40	
	110,                    // Car Call Security Input Array Number		ccs #41	
	110,                    // Car Call Security Input Array Number		ccs #42	
	110,                    // Car Call Security Input Array Number		ccs #43	
	110,                    // Car Call Security Input Array Number		ccs #44	
	110,                    // Car Call Security Input Array Number		ccs #45	
	110,                    // Car Call Security Input Array Number		ccs #46	
	110,                    // Car Call Security Input Array Number		ccs #47
	// ON 3rd Serial Car Board
	114,                    // Car Call Security Input Array Number		ccs #48	
	114,                    // Car Call Security Input Array Number		ccs #49	
	114,                    // Car Call Security Input Array Number		ccs #50	
	114,                    // Car Call Security Input Array Number		ccs #51	
	114,                    // Car Call Security Input Array Number		ccs #52	
	114,                    // Car Call Security Input Array Number		ccs #53	
	114,                    // Car Call Security Input Array Number		ccs #54	
    114,                    // Car Call Security Input Array Number		ccs #55	
	115,                    // Car Call Security Input Array Number		ccs #56	
	115,                    // Car Call Security Input Array Number		ccs #57	
	115,                    // Car Call Security Input Array Number		ccs #58	
	115,                    // Car Call Security Input Array Number		ccs #59	
	115,                    // Car Call Security Input Array Number		ccs #60	
};
		

// i/o blocks 0 and 1
extern const int16 DN[2] = {BRD1A,BRD41A};		// Down Directional Stop Input Board Address
extern const uint8 DNB[2] = {BIT1,BIT1};     		// Down Directional Stop Input Bit Number
extern const uint8 DND[2] = {0,120};         		// Down Directional Stop Input Data Array Number

extern const int16 DT[2] = {BRD1A,BRD41A};   		// Down Terminal Slowdown Input Board Address
extern const uint8 DTB[2] = {BIT2,BIT2};     		// Down Terminal Slowdown Input Bit Number
extern const uint8 DTD[2] = {0,120};         		// Down Termianl Slowdown Input Data Array Number
		
extern const int16 DTS[2] = {BRD1A,BRD41C};      	// Emergency Down Terminal Input Board Address
extern const uint8 DTSB[2] = {BIT3,BIT1};       	// Emergency Down Terminal Input Bit Number
extern const uint8 DTSD[2] = {0,122};          	// Emergency Down Terminal Input Data Array Number

extern const int16 DT1[2] = {BRD1A,BRD41A};      	// 1st Down Terminal Input Board Address
extern const uint8 DT1B[2] = {BIT4,BIT3};       	// 1st Down Terminal Input Bit Number
extern const uint8 DT1D[2] = {0,120};	        	// 1st Down Terminal Input Data Array Number

extern const int16 DT2[2] = {BRD1A,BRD41A};      	// 2nd Down Terminal Input Board Address
extern const uint8 DT2B[2] = {BIT5,BIT4};       	// 2nd Down Terminal Input Bit Number
extern const uint8 DT2D[2] = {0,120};         	// 2nd Down Terminal Input Data Array Number

extern const int16 DT3[2] = {BRD1A,BRD41A};     	// 3RD Down Terminal Input Board Address
extern const uint8 DT3B[2] = {BIT6,BIT5};       	// 3RD Down Terminal Input Bit Number
extern const uint8 DT3D[2] = {0,120};         	// 3RD Down Terminal Input Data Array Number
		
#if ((Traction == 1) || (Tract_OL == 1))
	extern const int16 DT4[2] = {BRD2C,BRD41A};     	// 4TH Down Terminal Input Board Address
	extern const uint8 DT4B[2] = {BIT5,BIT6};       	// 4TH Down Terminal Input Bit Number
	extern const uint8 DT4D[2] = {5,120};         	// 4TH Down Terminal Input Data Array Number

	extern const int16 DT5[2] = {BRD2C,BRD41A};     	// 5TH Down Terminal Input Board Address
	extern const uint8 DT5B[2] = {BIT6,BIT7};       	// 5TH Down Terminal Input Bit Number
	extern const uint8 DT5D[2] = {5,120};         	// 5TH Down Terminal Input Data Array Number

	extern const int16 DT6[2] = {BRD2C,BRD41A};     	// 6TH Down Terminal Input Board Address
	extern const uint8 DT6B[2] = {BIT8,BIT8};       	// 6TH Down Terminal Input Bit Number
	extern const uint8 DT6D[2] = {5,120};         	// 6TH Down Terminal Input Data Array Number
#endif

   
extern const int16 MRIE[3] = {BRD2C,BRD2C,BRD2C};     // Machine Room Inspection Enable Input Board Address
extern const uint8 MRIEB[3] = {BIT8,BIT1,BIT6};       // Machine Room Inspection Enable Input Bit Number
extern const uint8 MRIED[3] = {5,5,5};         		// Machine Room Inspection Enable Input Data Array Number

extern const int16 EBKS[4] = {BRD27A,BRD2C,BRD2C,BRD2C};     // Emergency Brake Switch Input Board Address
extern const uint8 EBKSB[4] = {BIT2,BIT5,BIT7,BIT8};       // Emergency Brake Switch Input Bit Number
extern const uint8 EBKSD[4] = {78,5,5,5};         		// Emergency Brake Switch Input Data Array Number

extern const int16 UN[2] = {BRD1A,BRD41B};      	// Up Terminal Stop Input Board Address
extern const uint8 UNB[2] = {BIT7,BIT1};      	// Up Termianl Stop Input Bit Number
extern const uint8 UND[2] = {0,121};          	// Up Terminal Stop Input Data Array Number

extern const int16 UT[2] = {BRD1A,BRD41B};       	// Up Terminal Slowdown Input Board Address
extern const uint8 UTB[2] = {BIT8,BIT2};        	// Up Terminal Slowdown Input Bit Number
extern const uint8 UTD[2] = {0,121};           	// Up Terminal Slowdown Input Data Array Number

extern const int16 UTS[2] = {BRD1B,BRD41C};      	// Emergency Up Terminal Input Board Address
extern const uint8 UTSB[2] = {BIT1,BIT2};       	// Emergency Up Terminal Input Bit Number
extern const uint8 UTSD[2] = {1,122};          	// Emergency Up Terminal Input Data Array Number

extern const int16 UT1[2] = {BRD1B,BRD41B};      	// 1ST Up Terminal Input Board Address
extern const uint8 UT1B[2] = {BIT2,BIT3};       	// 1ST Up Terminal Input Bit Number
extern const uint8 UT1D[2] = {1,121};          	// 1ST Up Terminal Input Data Array Number
		
extern const int16 UT2[2] = {BRD1B,BRD41B};      	// 2nd Up Terminal Input Board Address
extern const uint8 UT2B[2] = {BIT3,BIT4};       	// 2nd Up Terminal Input Bit Number
extern const uint8 UT2D[2] = {1,121};          	// 2nd Up Terminal Input Data Array Number

extern const int16 UT3[2] = {BRD1B,BRD41B};       // 3RD Up Terminal Input Board Address
extern const uint8 UT3B[2] = {BIT4,BIT5};       	// 3RD Up Terminal Input Bit Number
extern const uint8 UT3D[2] = {1,121};          	// 3RD Up Terminal Input Data Array Number

#if ((Traction == 1) || (Tract_OL == 1))
	extern const int16 UT4[2] = {BRD2C,BRD41B};       // 4TH Up Terminal Input Board Address
	extern const uint8 UT4B[2] = {BIT1,BIT6};       	// 4TH Up Terminal Input Bit Number
	extern const uint8 UT4D[2] = {5,121};          	// 4TH Up Terminal Input Data Array Number

	extern const int16 UT5[2] = {BRD2C,BRD41B};       // 5TH Up Terminal Input Board Address
	extern const uint8 UT5B[2] = {BIT2,BIT7};       	// 5TH Up Terminal Input Bit Number
	extern const uint8 UT5D[2] = {5,121};          	// 5TH Up Terminal Input Data Array Number
			
	extern const int16 UT6[2] = {BRD2C,BRD41B};       // 5TH Up Terminal Input Board Address
	extern const uint8 UT6B[2] = {BIT4,BIT8};       	// 5TH Up Terminal Input Bit Number
	extern const uint8 UT6D[2] = {5,121};          	// 5TH Up Terminal Input Data Array Number
#endif

extern const int16 FBZ[3] = {BRD3C,BRD3C,BRD5C};     	// Machine room fire buzzer board address
extern const int8 FBZB[3] = {BIT1,BIT6,BIT5};      	// Machine room fire buzzer Output Bit Number
extern const int8 FBZD[3] = {8,8,14};        		// Machine room fire buzzer Output Data Array Number

// i/o blocks 2 and 5
extern const int16 SS[2] = {BRD2B,BRD5B};      	// Saftey String Input Board Address
extern const uint8 SSB[2] = {BIT8,BIT5};       	// Saftey String Input Bit Number
extern const uint8 SSD[2] = {4,13};          		// Saftey String Input Data Array Number

extern const int16 CWS[2] = {BRD5B,BRD2C};		// Counterweight Switch Input Board Address (Traction non-distance feedback)
extern const uint8 CWSB[2] = {BIT5,BIT8};			// Counterweight Switch Input Bit Number
extern const uint8 CWSD[2] = {13,5};				// Counterweight Switch Input Data Array Number

// Note: TDC uses the same input locations as EQR
extern const int16 TDC[4] = {BRD2A,BRD5A,BRD27A,BRD2B};	// Top Door Close Contact input board address
extern const uint8 TDCB[4] = {BIT5,BIT7,BIT5,BIT1};   	// Top Door Close Contact input bit number
extern const uint8 TDCD[4] = {3,12,78,4};		   	// Top Door Close Contact input data array number

// Note: BDC uses the same input location as EMS AND ETH (EQ)
extern const int16 BDC[4] = {BRD2A,BRD2A,BRD27A,BRD2B};	// Bottom Door Close Contact Input	Board Address
extern const uint8 BDCB[4] = {BIT6,BIT4,BIT4,BIT2};   	// Bottom Door Close Contact Hall Input Bit Number
extern const uint8 BDCD[4] = {3,3,78,4};		   	// Bottom Door Close Contact Hall Input Data Array

// I/O Blocks 2 and 27

extern const int16 COL [2] = {BRD2A,BRD27A};     	// Earth Quake Collision Switch Board Address
extern const uint8 COLB [2] = {BIT3,BIT1};     	// Earth Quake Collision Switch Bit Number
extern const uint8 COLD [2] = {3,78};       		// Earth Quake Collision Switch Data Array Number

// Note: LOS is the same i/o as COL

extern const int16 LOS [2] = {BRD2A,BRD2C};     	// Low Oil Switch Board Address
extern const uint8 LOSB [2] = {BIT3,BIT6};     	// Low Oil Switch Bit Number
extern const uint8 LOSD [2] = {3,5};       		// Low Oil Switch Data Array Number

extern const int16 ETH [2] = {BRD2A,BRD27A};     	// Earth quake sizmic switch Board Address
extern const uint8 ETHB [2] = {BIT4,BIT2};     	// Earth Quake sizmic switch Bit Number
extern const uint8 ETHD [2] = {3,78};       		// Earth Quake sizmic switch Data Array Number

extern const int16 EQR [2] = {BRD2A,BRD27A};		// Earthquake operation reset input board address
extern const uint8 EQRB [2] = {BIT5,BIT3};		// Earthquake operation input bit number
extern const uint8 EQRD [2] = {3,78};				// Earthquake operation input data array number

// Note: EOR same as EQR
extern const int16 EOR [4] = {BRD2A,BRD27B,BRD49B,BRD49B};		// Elevator Off Reset input board address
extern const uint8 EORB [4] = {BIT5,BIT1,BIT7,BIT7};		// Elevator Off Reset input bit number
extern const uint8 EORD [4] = {3,79,145,145};				// Elevator Off Reset input data array number
											
// Note: RTLDC same as EQR
extern const int16 RTLDC [2] = {BRD2A,BRD27A};	// Recall to lobby keep doors closed input board address
extern const uint8 RTLDCB [2] = {BIT5,BIT8};		// Recall to lobby keep doors closed input bit number
extern const uint8 RTLDCD [2] = {3,78};			// Recall to lobby keep doors closed input data array number

// Note: LPS is the same i/o location as EMSH

extern const int16 LPS[2] = {BRD2A,BRD2C};		// Low Pressure Switch Input Board Address
extern const uint8 LPSB[2] = {BIT6,BIT5};			// Low Pressure Switch Input Bit Number
extern const uint8 LPSD[2] = {3,5};				// Low Pressure Switch Input Data Array

// i/o blocks 3 and 5
// Note: MDC uses the same input location as DPR
extern const int16 MDC  [3] = {BRD3B,BRD27A,BRD2B};    	// Middle Door Close Contact input value 1 Board Address
extern const uint8 MDCB [3] = {BIT4,BIT6,BIT3};     	// Middle Door Close Contact input value 1 Bit Number
extern const uint8 MDCD [3] = {7,78,4};        		// Middle Door Close Contact input value 1 Data Array Number

// Note: MDCR uses the same input location as RDPR
extern const int16 MDCR [3] = {BRD5B,BRD27A,BRD2B};		// Rear Middle Door Close Contact Input Board Address (Traction non-distance feedback)
extern const uint8 MDCRB [3] = {BIT6,BIT7,BIT4};		// Rear Middle Door Close Contact Input Bit Number
extern const uint8 MDCRD [3] = {13,78,4};			// Rear Middle Door Close Contact Input Data Array Number

extern const int16 RCM[2] = {BRD5C,BRD3C};      	// Retiring Cam Output Board Address
extern const uint8 RCMB[2] = {BIT5,BIT4};       	// Retiring Cam Output Bit Number
extern const uint8 RCMD[2] = {14,8};	         	// Retiring Cam Output Data Array Number

// io block 3 and 41

extern const int16 UL_1[2] = {BRD3A,BRD41C};		// Up Level - Secondary Input Board Address
extern const uint8 UL_1B[2] = {BIT7,BIT3};		// Up Level - Secondary Input Bit Number
extern const uint8 UL_1D[2] = {6,122};   			// Up Level - Secondary Input Data Array Number

extern const int16 UL[2] = {BRD3B,BRD41C};      	// Up Level Input Board Address
extern const uint8 ULB[2] = {BIT1,BIT4};       	// Up Level Input Bit Number
extern const uint8 ULD[2] = {7,122};          	// Up Level Input Data Array Number

extern const int16 DZ[2] = {BRD3B,BRD41C};     	// Door Zone Input Board Address
extern const uint8 DZB[2] = {BIT2,BIT5};      	// Door Zone Input Bit Number
extern const uint8 DZD[2] = {7,122};         		// Door Zone Input Data Array Number

extern const int16 DZA[2] = {BRD25A,BRD41C};      // DZA (both dz sensors ored) Input Board Address
extern const uint8 DZAB[2] = {BIT6,BIT6};   	    // DZA (both dz sensors ored) Input Bit Number	
extern const uint8 DZAD[2] = {72,122};         	// DZA (both dz sensors ored) Input Data Array Number

extern const int16 DL[2] = {BRD3B,BRD41C};      	// Down Level Input Board Address
extern const uint8 DLB[2] = {BIT3,BIT7};       	// Down Level Input Bit Number
extern const uint8 DLD[2] = {7,122};         		// Down Level Input Data Array Number

extern const int16 DL_1[2] = {BRD3A,BRD41C};		// Down Level - Secondary Input Board Address
extern const uint8 DL_1B[2] = {BIT8,BIT8};		// Down Level - Secondary Input Bit Number
extern const uint8 DL_1D[2] = {6,122};   			// Down Level - Secondary Input Data Array Number


// i/o block 5
// ISER same i/o as LD and EQL

extern const int16 ISER [2] = {BRD5C,BRD5C};
extern const uint8 ISERB [2] = {BIT4,BIT2};
extern const uint8 ISERD [2] = {14,14};

//io block 5 and 28

extern const int16 OSERL [2] = {BRD5C,BRD50B} ;
extern const uint8 OSERLB [2] = {BIT2,BIT7};
extern const uint8 OSERLD [2] = {14,148};

extern const int16 OSERB [1] = {BRD50B} ;
extern const uint8 OSERBB [1] = {BIT8};
extern const uint8 OSERBD [1] = {148};

extern const int16 OSERA [1] = {BRD49B};
extern const uint8 OSERAB [1] = {BIT8};
extern const uint8 OSERAD [1] = {145};

// I/O blocks 7 and 9
extern const int16 RM_DCBI [2] = {BRD7C,BRD9C};   // Door Close Button Input Board Address
extern const uint8 RM_DCBIB [2] = {BIT6,BIT6};	// Door Close Button Input Bit Number
extern const uint8 RM_DCBID [2] = {20,26};	    // Door Close Button Input Data Array Number

extern const int16 RM_DOBI [2] = {BRD7C,BRD9C};	// Door Open Button Input Board Address
extern const uint8 RM_DOBIB [2] = {BIT7,BIT7}; 	// Door Open Button Input Bit Number
extern const uint8 RM_DOBID [2] = {20,26};     	// Door Open Button Input Data Array Number

extern const int16 RM_INDC [2] = {BRD7C,BRD9C};	// Independent Input Board Address
extern const uint8 RM_INDCB [2] = {BIT5,BIT5};   	// Independent Input Bit Number
extern const uint8 RM_INDCD [2] = {20,26};     	// Independent Input Data Array Number

// Note this input is the same location as rm_indc and is used when cons[remote_cc] == 2.
extern const int16 RM_ED [2] = {BRD7C,BRD9C};		// Extended door time Input Board Address
extern const uint8 RM_EDB [2] = {BIT5,BIT5};   	// Extended door time Input Bit Number
extern const uint8 RM_EDD [2] = {20,26};     		// Extended door time Input Data Array Number

extern const int16 SCS [2] = {BRD7C,BRD9C};  		// Switch to Remote Car Station Input Board Address(spi14 Terminal)
extern const uint8 SCSB [2] = {BIT8,BIT8};   		// Switch to Remote Car Station Input Bit Number
extern const uint8 SCSD [2] = {20,26};     		// Switch to Remote Car Station Input Data Array Number

extern const int16 RM_DCBR [2] = {BRD11C,BRD39C} ;   // Rear Door Close Button Input Board Address
extern const uint8 RM_DCBRB [2] = {BIT6,BIT6} ;	// Rear Door Close Button Input Bit Number
extern const uint8 RM_DCBRD [2] = {32,116} ;  // Rear Door Close Button Input Data Array Number

extern const int16 RM_DOBR [2] = {BRD11C,BRD39C};	// Rear Door Open Button Input Board Address
extern const uint8 RM_DOBRB [2] = {BIT7,BIT7}; 	// Rear Door Open Button Input Bit Number
extern const uint8 RM_DOBRD [2] = {32,116}; 	// Rear Door Open Button Input Data Array Number

extern const int16 SCSR [2] = {BRD11C,BRD39C};  	// Switch to Remote Car Station Input Board Address(spi14 Terminal)al)
extern const uint8 SCSRB [2] = {BIT7,BIT7};		// Switch to Remote Car Station Input Bit Number
extern const uint8 SCSRD [2] = {32,116}; 	// Switch to Remote Car Station Input Data Array Number


// Rear I/O's
// Block 17	 

extern const int16 DCBR[2] = {BRD17C,BRD17B};     // Rear Door Close Button Input Board Address
extern const uint8 DCBRB[2] = {BIT7,BIT3};       	// Rear Door Close Button Input Bit Number
extern const uint8 DCBRD[2] = {50,49};         	// Rear Door Close Button Input Data Array Number

extern const int16 DOBR[2] = {BRD17C,BRD17B};     // Rear Door Open Button Input Board Address
extern const uint8 DOBRB[2] = {BIT8,BIT4};       	// Rear Door Open Button Input Bit Number
extern const uint8 DOBRD[2] = {50,49};	        // Rear Door Open Button Input Data Array Number


// Rear Door I/O Block 21 and 17
extern const int16 DOLR [3] = {BRD17C,BRD21A,BRD26A};    	// Door Open Limit Rear Input Board Address
extern const uint8 DOLRB [3] = {BIT2,BIT1,BIT5};      	// Door Open Limit Rear Input Bit Number
extern const uint8 DOLRD [3] = {50,60,75};       			// Door Open Limit Rear Input Data Array Number

extern const int16 DCLR [3] = {BRD17C,BRD21A,BRD26A};    	// Door Close Limit Rear Input Board Address
extern const uint8 DCLRB [3] = {BIT3,BIT2,BIT6};	    	// Door Close Limit Rear Input Bit Number
extern const uint8 DCLRD [3] = {50,60,75};       			// Door Close Limit Rear Input Data Array Number

extern const int16 RPM [3] = {BRD5B,BRD5B,BRD26A};		// Rear Door Proctection Input Board Address (Traction non-distance feedback)
extern const uint8 RPMB [3] = {BIT6,BIT6,BIT7};			// Rear Door Proctection Input Bit Number
extern const uint8 RPMD [3] = {13,13,75};					// Rear Door Proctection Input Data Array Number

extern const int16 EER [3] = {BRD17C,BRD21A,BRD26A};     	// Electric Eye Rear Input Board Address
extern const uint8 EERB [3] = {BIT5,BIT3,BIT8};      		// Electric Eye Rear Input Bit Number
extern const uint8 EERD [3] = {50,60,75};        			// Electric Eye Rear Input Data Array Number

extern const int16 SER [3] = {BRD17C,BRD21A,BRD21A};     	// Saftey Edge Rear Input Board Address
extern const uint8 SERB [3] = {BIT6,BIT4,BIT4};      		// Saftey Edge Rear Input Bit Number
extern const uint8 SERD [3] = {50,60,60};        			// Saftey Edge Rear Input Data Array Number

// Blocks 22 and 18

extern const int16 CDLR [2] = {BRD18C,BRD46C};		    	// Down Cab Lantern Rear Output Board Address
extern const uint8 CDLRB [2] = {BIT2,BIT7};       	    // Down Cab Lantern Rear Output Bit Number
extern const uint8 CDLRD [2] = {53,137};         	      	// Down Cab Lantern Rear Output Data Array Number

extern const int16 CULR [2] = {BRD18C,BRD46C};    		// Up Cab Lantern Rear Output Board Address
extern const uint8 CULRB [2] = {BIT3,BIT8};          		// Up Cab Lantern Rear Output Bit Number
extern const uint8 CULRD [2] = {53,137};         	     	// Up Cab Lantern Rear Output Data Array Number

extern const int16 REVR [3] = {BRD18C,BRD22A,BRD22A};    	// Rear Door Reversal Relay Output Board Address
extern const uint8 REVRB [3] = {BIT4,BIT7,BIT7};       	// Rear Door Reversal Relay Output Bit Number
extern const uint8 REVRD [3] = {53,63,63};         		// Rear Door Reversal Relay Output Data Array Number

extern const int16 RVDR [3] = {BRD18C,BRD22A,BRD22A};    	// Rear Door Reversal Device Output Board Address
extern const uint8 RVDRB [3] = {BIT5,BIT8,BIT8};       	// Rear Door Reversal Device Output Bit Number
extern const uint8 RVDRD [3] = {53,63,63};         		// Rear Door Reversal Device Output Data Array Number

// Note: HVDR has the same i/o location as RVDR for old board location (rdoor_io_sel = 0)
extern const int16 HVDR [3] = {BRD18C,BRD22B,BRD26C};    	// Rear Heavy Door Output Board Address
extern const uint8 HVDRB [3] = {BIT5,BIT1,BIT1};       	// Rear Heavy Door Output Bit Number
extern const uint8 HVDRD [3] = {53,64,77};	  			// Rear Heavy Door Output Data Array Number

extern const int16 DOR [3] = {BRD18C,BRD22B,BRD26C};    	// Door Open Rear Output Board Address
extern const uint8 DORB [3] = {BIT6,BIT2,BIT2};			// Door Open Rear Output Bit Number
extern const uint8 DORD [3] = {53,64,77};					// Door Open Rear Output Data Array Number

extern const int16 DCR [3] = {BRD18C,BRD22B,BRD26C};		// Door Close Rear Output Board Address
extern const uint8 DCRB [3] = {BIT7,BIT3,BIT3};			// Door Close Rear Output Bit Number
extern const uint8 DCRD [3] = {53,64,77};					// Door Close Rear Output Data Array Number

extern const int16 NUDR [3] = {BRD18C,BRD22B,BRD26C};		// Nudge Rear Output Board Address
extern const uint8 NUDRB [3] = {BIT8,BIT4,BIT4};			// Nudge Rear Output Bit Number
extern const uint8 NUDRD [3] = {53,64,77};				// Nudge Rear Output Data Array Number


// TOC AND COP BOARD INPUTS
// Block 23 and 13

extern const int16 FS2 [3] = {BRD23B,BRD13B,BRD45A};     	// Fire ServiceII Switch Input Board Addres
extern const uint8 FS2B [3] = {BIT5,BIT5,BIT1};      		// Fire ServiceII Switch Input Bit Number
extern const uint8 FS2D [3] = {67,37,132};	           	// Fire ServiceII Switch Input Data Array Number

extern const int16 FS2H [3] = {BRD23B,BRD13B,BRD45A};     // Fire ServiceII Hold Input Board Address
extern const uint8 FS2HB [3] = {BIT6,BIT6,BIT2};       	// Fire ServiceII Hold Input Bit Number
extern const uint8 FS2HD [3] = {67,37,132};		       	// Fire ServiceII Hold Input Data Array Number

extern const int16 FS2C [3] = {BRD23B,BRD13B,BRD43A};     // Fire ServiceII Call Cancel Input Board Address
extern const uint8 FS2CB [3] = {BIT7,BIT7,BIT7};       	// Fire ServiceII Call Cancel Input Bit Number
extern const uint8 FS2CD [3] = {67,37,126}; 		    	// Fire ServiceII Call Cancel Input Data Array Number

extern const int16 HBE [3] = {BRD23B,BRD13B,BRD43A};		// Handicap Buzzer Enable Input Board Address
extern const uint8 HBEB [3] = {BIT8,BIT8,BIT8};			// Handicap Buzzer Enable Input Bit Number
extern const uint8 HBED [3] = {67,37,126};				// Handicap Buzzer Enable Input Data Array Number

extern const int16 DCBI [3] = {BRD23C,BRD13C,BRD43B};	    // Door Close Button Input Board Address
extern const uint8 DCBIB [3] = {BIT1,BIT1,BIT1};			// Door Close Button Input Bit Number
extern const uint8 DCBID [3] = {68,38,127};		        // Door Close Button Input Data Array Number

extern const int16 DOBI [3] = {BRD23C,BRD13C,BRD43B};	    // Door Open Button Input Board Address
extern const uint8 DOBIB [3] = {BIT2,BIT2,BIT2};		    // Door Open Button Input Bit Number
extern const uint8 DOBID [3] = {68,38,127};		        // Door Open Button Input Data Array Number

extern const int16 INDC [3] = {BRD23C,BRD13C,BRD45A};	    // Independent Input Board Address
extern const uint8 INDCB [3] = {BIT3,BIT3,BIT3};	        // Independent Input Bit Number
extern const uint8 INDCD [3] = {68,38,132};		        // Independent Input Data Array Number

extern const int16 EMS [3] = {BRD23C,BRD13C,BRD45A};		// Emergency Medical Servie Car switch Input Board Address
extern const uint8 EMSB [3] = {BIT4,BIT4,BIT4};			// Emergency Medical Servie Car switch Input Bit Number
extern const uint8 EMSD [3] = {68,38,132};				// Emergency Medical Servie Car switch Input Data Array Number

extern const int16 ZON [3] = {BRD23C,BRD13C,BRD45A};		// Zoning Enable Car switch Input Board Address
extern const uint8 ZONB [3] = {BIT4,BIT4,BIT4};			// Zoning Enable Car switch Input Bit Number
extern const uint8 ZOND [3] = {68,38,132};				// Zoning Enable Car switch Input Data Array Number

	// Note same i/o location as EMS
extern const int16 DLW [7] = {BRD23C,BRD13C,BRD23C,BRD13C,BRD23B,BRD13B,BRD45A};	// Disable Load Weigher switch Input Board Address
extern const uint8 DLWB [7] = {BIT4,BIT4,BIT5,BIT5,BIT8,BIT8,BIT5};			// Disable Load Weigher switch Input Bit Number
extern const uint8 DLWD [7] = {68,38,68,38,67,37,132};						// Disable Load Weigher switch Input Data Array Number

	// Note same i/o location as EMS
extern const int16 ATTHC [3] = {BRD23C,BRD13C,BRD47A};		// Attendant Hall Call Cancel Input Board Address
extern const uint8 ATTHCB [3] = {BIT4,BIT4,BIT4};			// Attendant Hall Call Cancel Input Bit Number
extern const uint8 ATTHCD [3] = {68,38,138};				// Attendant Hall Call Cancel Input Data Array Number

extern const int16 ATT [3] = {BRD23C,BRD13C,BRD45A};		// Attendent Operation Switch Board Address
extern const uint8 ATTB [3] = {BIT5,BIT5,BIT6};	     	// Attendent Operation Switch Bit Number
extern const uint8 ATTD [3] = {68,38,132};		   		// Attendent Operation Switch Data Array Number

extern const int16 ATTUP [3] = {BRD23C,BRD13C,BRD47A};	// Attendent up push button Board Address
extern const uint8 ATTUPB [3] = {BIT6,BIT6,BIT1};		   	// Attendent up push button Bit Number
extern const uint8 ATTUPD [3] = {68,38,138};		     	// Attendent up push button Data Array Number

// Independent Security Override and Attendant Down maped to the same location
extern const int16 ATTDN [3] = {BRD23C,BRD13C,BRD47A};	// Attendent down push button Board Address
extern const uint8 ATTDNB [3] = {BIT7,BIT7,BIT2};		  	// Attendent down push button Bit Number
extern const uint8 ATTDND [3] = {68,38,138};		    	// Attendent down push button Data Array Number

extern const int16 IDSO [3] = {BRD23C,BRD13C,BRD47A};			// Independent Security Override Input Board Address
extern const uint8 IDSOB [3] = {BIT7,BIT7,BIT5};		  		// Independent Security Override Input Number
extern const uint8 IDSOD [3] = {68,38,138};		    	// Independent Security Override Input Data Array Number

// Note: HUGB Extra Door Time and Attendant Bypass maped to the same location
extern const int16 ATTBY [3] = {BRD23C,BRD13C,BRD47A};	  	// Attendant Bypass Button Input Board Address
extern const uint8 ATTBYB [3] = {BIT8,BIT8,BIT3};	    	// Attendant Bypass Button Input Bit Number
extern const uint8 ATTBYD [3] = {68,38,138};			   	// Attendant Bypass Button Input Data Array Number

extern const int16 ED [3] = {BRD23C,BRD13C,BRD47A};  	  	// Extra Door Time Input Board Address
extern const uint8 EDB [3] = {BIT8,BIT8,BIT6};	    	  	// Extra Door Time Input Bit Number
extern const uint8 EDD [3] = {68,38,138};			     	// Extra Door Time Input Data Array Number

extern const int16 HUGB [3] = {BRD23C,BRD13C,BRD47A};  	  	// HUGS Bypass Input Board Address
extern const uint8 HUGBB [3] = {BIT8,BIT8,BIT6};	    	  	// HUGS Bypass Input Bit Number
extern const uint8 HUGBD [3] = {68,38,138};			     	// HUGS Bypass Input Data Array Number

// Note: Same input as LWA
extern const int16 ALRM [2] = {BRD25B,BRD43B};   			// Alarm Button Input Board Address
extern const uint8 ALRMB [2] = {BIT5,BIT3};	    		// Alarm Button Input Bit Number
extern const uint8 ALRMD [2] = {73,127};	 				// Alarm Button Input Data Array Number



// TOC AND COP BOARD OUTPUTS
// Block 24, 25	AND 14

extern const int16 DDAC [3] = {BRD24B,BRD14C,BRD48C};   	// Down Directional Arrow Output Board Address
extern const uint8 DDACB [3] = {BIT5,BIT2,BIT1};   	   	// Down Directional Arrow Output Bit Number
extern const uint8 DDACD [3] = {70,41,143};		        // Down Directional Arrow Output Data Array Number

extern const int16 DPI1 [3] = {BRD24B,BRD14C,BRD48C};		// Discrete/Digital Position Indicator Output 1 Board Address
extern const uint8 DPI1B [3] = {BIT6,BIT3,BIT2};	  		// Discrete/Digital Position Indicator Output 1 Bit Number
extern const uint8 DPI1D [3] = {70,41,143};		  		// Discrete/Digital Position Indicator Output 1 Array Number

extern const int16 DPI2 [3] = {BRD24B,BRD14C,BRD48C};		// Discrete/Digital Position Indicator Output 2 Board Address
extern const uint8 DPI2B [3] = {BIT7,BIT4,BIT3};		    // Discrete/Digital Position Indicator Output 2 Bit Number
extern const uint8 DPI2D [3] = {70,41,143};		        // Discrete/Digital Position Indicator Output 2 Array Number

extern const int16 DPI3 [3] = {BRD24B,BRD14C,BRD48C};	   	// Discrete/Digital Position Indicator Output 3 Board Address
extern const uint8 DPI3B [3] = {BIT8,BIT5,BIT4};			// Discrete/Digital Position Indicator Output 3 Bit Number
extern const uint8 DPI3D [3] = {70,41,143};				// Discrete/Digital Position Indicator Output 3 Array Number

extern const int16 DPI4 [3] = {BRD24C,BRD14C,BRD48C};		// Discrete/Digital Position Indicator Output 4 Board Address
extern const uint8 DPI4B [3] = {BIT1,BIT6,BIT5};			// Discrete/Digital Position Indicator Output 4 Bit Number
extern const uint8 DPI4D [3] = {71,41,143};				// Discrete/Digital Position Indicator Output 4 Array Number

extern const int16 DPI5 [3] = {BRD24C,BRD14C,BRD48C};	   	// Discrete/Digital Position Indicator Output 4 Board Address
extern const uint8 DPI5B [3] = {BIT2,BIT7,BIT6};      		// Discrete/Digital Position Indicator Output 4 Bit Number
extern const uint8 DPI5D [3] = {71,41,143};				// Discrete/Digital Position Indicator Output 4 Array Number

extern const int16 DPI6 [3] = {BRD24C,BRD25C,BRD48C};	   	// Discrete/Digital Position Indicator Output 4 Board Address
extern const uint8 DPI6B [3] = {BIT3,BIT4,BIT7};	      	// Discrete/Digital Position Indicator Output 4 Bit Number
extern const uint8 DPI6D [3] = {71,74,143};		        // Discrete/Digital Position Indicator Output 4 Array Number

//Note: HUGO and DPI1 are mapped to the same output	location

extern const int16 HUGO [3] = {BRD24B,BRD14C,BRD48C};		// HUGS Output 1 Board Address
extern const uint8 HUGOB [3] = {BIT6,BIT3,BIT2};	  		// HUGS Output 1 Bit Number
extern const uint8 HUGOD [3] = {70,41,143};		  			// HUGS Output 1 Array Number

//Note: HUGO and DPI1 are mapped to the same output	location

extern const int16 DCLT [3] = {BRD24B,BRD14C,BRD48C};		// HUGS Output 1 Board Address
extern const uint8 DCLTB [3] = {BIT6,BIT3,BIT2};	  		// HUGS Output 1 Bit Number
extern const uint8 DCLTD [3] = {70,41,143};		  		// HUGS Output 1 Array Number


// NOTE: EDL and DPI5 are mapped to the same output	location

extern const int16 EDL [3] = {BRD24C,BRD14C,BRD44C};			// Extended Door Hold Light output Board Address
extern const uint8 EDLB [3] = {BIT2,BIT7,BIT3};				// Extended Door Hold Light Bit Number
extern const uint8 EDLD [3] = {71,41,131};					// Extended Door Hold Light Array Number


//SERVO output mapped to 5U in main board or P1 in COP

extern const int16 SERVO [2] = {BRD3C,BRD14C};
extern const uint8 SERVOB [2] = {BIT8,BIT3};
extern const uint8 SERVOD [2] = {8,41};


// NOTE: SABUZ, EML and DPI5 are mapped to the same output	location

extern const int16 SABUZ [3] = {BRD24C,BRD14C,BRD46B};		// Sabbath warning buzzer/chime Output Board Address
extern const uint8 SABUZB [3] = {BIT2,BIT7,BIT3};			// Sabbath warning buzzer/chime Output Bit Number
extern const uint8 SABUZD [3] = {71,41,136};				// Sabbath warning buzzer/chime Output Array Number

// NOTE: HUGA, SABUZ, EML and DPI5 are mapped to the same output	location

extern const int16 HUGA [3] = {BRD24C,BRD14C,BRD46B};		// HUGS alarm Output Board Address
extern const uint8 HUGAB [3] = {BIT2,BIT7,BIT3};			// HUGS alarm Output Bit Number
extern const uint8 HUGAD [3] = {71,41,136};					// HUGS alarm Output Array Number

// NOTE: DTR2 and DPI5 are mapped to the same output location

extern const int16 DTR2 [3] = {BRD24C,BRD14C,BRD46B};		// Extended Door Hold Light output Board Address
extern const uint8 DTR2B [3] = {BIT2,BIT7,BIT4};			// Extended Door Hold Light Bit Number
extern const uint8 DTR2D [3] = {71,41,136};					// Extended Door Hold Light Array Number

// Note that staggard rear Door Transfer Relay uses the same i/o as DPI6 

extern const int16 DTR [3] = {BRD24C,BRD25C,BRD25C};	   		// Staggard Rear Door Transfer Relay Output 4 Board Address
extern const uint8 DTRB [3] = {BIT3,BIT4,BIT4};	      		// Staggard Rear Door Transfer Relay Output 4 Bit Number
extern const uint8 DTRD [3] = {71,74,74};		        	// Staggard Rear Door Transfer Relay Output 4 Array Number

// Note that Sabbath Electric Eye Enable uses the same i/o as DPI6 

extern const int16 SABEE [3] = {BRD24C,BRD25C,BRD25C};   	// Sabbath Electric Eye Enable Output 4 Board Address
extern const uint8 SABEEB [3] = {BIT3,BIT4,BIT4};	      	// Sabbath Electric Eye Enable Output 4 Bit Number
extern const uint8 SABEED [3] = {71,74,74};		        	// Sabbath Electric Eye Enable Output 4 Array Number

extern const int16 UDAC [3] = {BRD24C,BRD14C,BRD48C};   		// Up Directional Arrow Output Board Address
extern const uint8 UDACB [3] = {BIT4,BIT8,BIT8};		    // Up Directional Arrow Output Bit Number
extern const uint8 UDACD [3] = {71,41,143};		        // Up Directional Arrow Output Data Array Number

extern const int16 EQLC [4] = {BRD24C,BRD25C,BRD14C,BRD44C};	// Earthquake light output Board Address
extern const uint8 EQLCB [4] = {BIT5,BIT5,BIT4,BIT4};		// Earthquake light output Bit Number
extern const uint8 EQLCD [4] = {71,74,41,131};			// Earthquake light output Data Array Number

// Note: Same i/o as EQLC
extern const int16 F1ML [4] = {BRD24C,BRD25C,BRD14C,BRD44C};	// Fire 1 main return light output Board Address
extern const uint8 F1MLB [4] = {BIT5,BIT5,BIT4,BIT7};		// Fire 1 main return light output Bit Number
extern const uint8 F1MLD [4] = {71,74,41,131};			// Fire 1 main return light output Data Array Number

// Note: Same i/o as EQLC
extern const int16 RVD [4] = {BRD24C,BRD25C,BRD25C,BRD25C};	// Door Reversal Device Relay output Board Address
extern const uint8 RVDB [4] = {BIT5,BIT5,BIT4,BIT4};  		// Door Reversal Device Relay output Bit Number
extern const uint8 RVDD [4] = {71,74,74,74};				// Door Reversal Device Relay output Data Array Number

extern const int16 EML [4] = {BRD24C,BRD25C,BRD14C,BRD44C}; 	// Emergency Medical Light Output Board Address
extern const uint8 EMLB [4] = {BIT6,BIT6,BIT5,BIT5};		// Emergency Medical Light Output Bit Number
extern const uint8 EMLD [4] = {71,74,41,131};				// Emergency Medical Light Output Data Array Number

// Note: Same i/o as EML
extern const int16 DCA [4] = {BRD24C,BRD25C,BRD14C,BRD25C}; 	// Freight Door Close Alarm Output Board Address
extern const uint8 DCAB [4] = {BIT6,BIT6,BIT5,BIT6};		// Freight Door Close Alarm Output Bit Number
extern const uint8 DCAD [4] = {71,74,41,74};				// Freight Door Close Alarm Output Data Array Number

// Note: Same i/o as EML
extern const int16 F1AL [4] = {BRD24C,BRD25C,BRD14C,BRD44C};	// Fire 1 Alternate Return Light Output Board Address
extern const uint8 F1ALB [4] = {BIT6,BIT6,BIT5,BIT8};		// Fire 1 Alternate Return Light Output Bit Number
extern const uint8 F1ALD [4] = {71,74,41,131};			// Fire 1 Alternate Return Light Output Data Array Number

// Note: Same i/o as EML
extern const int16 DZDE [3] = {BRD24C,BRD25C,BRD25C};		// Door Zone Door Enable Output Board Address
extern const uint8 DZDEB [3] = {BIT6,BIT6,BIT6};			// Door Zone Door Enable Output Bit Number
extern const uint8 DZDED [3] = {71,74,74};				// Door Zone Door Enable Output Data Array Number

// Note: Same i/o as EML
extern const int16 REV [3] = {BRD24C,BRD25C,BRD25C};		   	// Door Reversal Relay Output Board Address
extern const uint8 REVB [3] = {BIT6,BIT6,BIT6};			// Door Reversal Relay Output Bit Number
extern const uint8 REVD [3] = {71,74,74};				// Door Reversal Relay Output Data Array Number

extern const int16 OLL [4] = {BRD24C,BRD25C,BRD14C,BRD44C};	// Overload light Output Board Address
extern const uint8 OLLB [4] = {BIT7,BIT7,BIT6,BIT6};		// Overload light Output Bit Number
extern const uint8 OLLD [4] = {71,74,41,131};				// Overload light Output Data Array Number

// Note same I/O locations as OLL
extern const int16 HVD [4] = {BRD24C,BRD25C,BRD25C,BRD26B}; 	// Heavy Door Output Board Address
extern const uint8 HVDB [4] = {BIT7,BIT7,BIT6,BIT1};		// Heavy Door Output Bit Number
extern const uint8 HVDD [4] = {71,74,74,76};				// Heavy Door Output Data Array Number

extern const int16 CLF [3] = {BRD24B,BRD25C,BRD25C};			// Cab Light and Fan Board Address
extern const uint8 CLFB [3] = {BIT8,BIT8,BIT8};			// Cab Light and Fan Bit Number
extern const uint8 CLFD [3] = {71,74,74};				// Cab Light and Fan Array Number


// Block 25	AND 14

extern const int16 DOL [2] = {BRD25B,BRD26A};   		// Door Open Limit Input Board Address
extern const uint8 DOLB [2] = {BIT1,BIT1};      		// Door Open Limit Input Bit Number
extern const uint8 DOLD [2] = {73,75};        		// Door Open Limit Input Data Array Number

extern const int16 DCL [2] = {BRD25B,BRD26A};    		// Door Close Limit Input Board Address
extern const uint8 DCLB [2] = {BIT2,BIT2};       		// Door Close Limit Input Bit Number
extern const uint8 DCLD [2] = {73,75};         		// Door Close Limit Input Data Array Number

extern const int16 DPM [2] = {BRD3B,BRD26A};	    	// Door Proctection input value 1 Board Address
extern const uint8 DPMB [2] = {BIT4,BIT3};	     	// Door Proctection input value 1 Bit Number
extern const uint8 DPMD [2] = {7,75};		        	// Door Proctection input value 1 Data Array Number

extern const int16 EE [2] = {BRD25B,BRD26A};   		// Electric Eye Input Board Address
extern const uint8 EEB [2] = {BIT3,BIT4};        		// Electric Eye Input Bit Number
extern const uint8 EED [2] = {73,75};          		// Electric Eye Input Data Array Number

extern const int16 DO [2] = {BRD25C,BRD26B};      	// Door Open Output Board Address
extern const uint8 DOB [2] = {BIT1,BIT2};        		// Door Open Output Bit Number
extern const uint8 DOD [2] = {74,76};         		// Door Open OUtput Data Array Number

extern const int16 DC [2] = {BRD25C,BRD26B};      	// Door Close Output Board Address
extern const uint8 DCB [2] = {BIT2,BIT3};       		// Door Close Output Bit Number
extern const uint8 DCD [2] = {74,76};         		// Door Close Output Data Array Number

extern const int16 NUD [2] = {BRD25C,BRD26B};     	// Nudge Output Board Address
extern const uint8 NUDB [2] = {BIT3,BIT4};       		// Nudge Output Bit Number
extern const uint8 NUDD [2] = {74,76};         		// Nudge Output Data Array Number

extern const int16 CUL [4] = {BRD25C,BRD14B,BRD46B,BRD25C};  	// Up Cab Lantern Output Board Address
extern const uint8 CULB [4] = {BIT4,BIT8,BIT1,BIT4};      	// Up Cab Lantern Output Bit Number
extern const uint8 CULD [4] = {74,40,136,74};		    		// Up Cab Lantern Output Data Array Number

extern const int16 CDL [4] = {BRD25C,BRD14B,BRD46A,BRD25C};  	// Down Cab Lantern Output Board Address
extern const uint8 CDLB [4] = {BIT5,BIT7,BIT8,BIT6};	   		// Down Cab Lantern Output Bit Number
extern const uint8 CDLD [4] = {74,40,135,74};		        	// Down Cab Lantern Output Data Array Number

extern const int16 FL [3] = {BRD25C,BRD14B,BRD44B};	      	// Fire Service Jewel Output Board Address
extern const uint8 FLB [3] = {BIT6,BIT5,BIT4};		        // Fire Service Jewel Output Bit Number
extern const uint8 FLD [3] = {74,40,130};		          		// Fire Service Jewel Output Data Array Number

extern const int16 FBNB [3] = {BRD25C,BRD14B,BRD46A};	   		// Fire Service/Nudging Buzzer Output Board Address
extern const uint8 FBNBB [3] = {BIT7,BIT6,BIT7};	     		// Fire Service/Nudging Buzzer Output Bit Number
extern const uint8 FBNBD [3] = {74,40,135};		        	// Fire Service/Nudging Buzzer Output Data Array Number

extern const int16 HB [3] = {BRD25C,BRD14C,BRD46B};	        // Handicap Floor Passing Output Board Address
extern const uint8 HBB [3] = {BIT8,BIT1,BIT2};		       	// Handicap Floor Passing Output Bit Number
extern const uint8 HBD [3] = {74,41,136};						// Handicap Floor Passing Data Array Number

// Block 28
extern const int16 MBC [2] = {BRD28B,BRD3C};  		// Manual Power Loss Brake Motor Contactor Output Board Address
extern const uint8 MBCB [2] = {BIT4,BIT8};			// Manual Power Loss Brake Motor Contactor Output Bit Number
extern const uint8 MBCD [2] = {82,8};					// Manual Power Loss Brake Motor Contactor Output Data Array Number

// Block 49

extern const int16 FS [2] = {BRD2B,BRD49A};       // Fire Service Switch Input Board Address
extern const uint8 FSB [2] = {BIT6,BIT1};        	// Fire Service Switch Input Bit Number
extern const uint8 FSD [2] = {4,144};           	// Fire Service Switch Input Data Array Number

extern const int16 FSX [2] = {BRD2A,BRD49A};		// Fire Switch Aux. Input Board Address
extern const uint8 FSXB [2] = {BIT2,BIT2};		// Fire Switch Aux. Input Bit Number
extern const uint8 FSXD [2] = {3,144};			// Fire Switch Aux. Input Data Array Number

extern const int16 BP [2] = {BRD2B,BRD49A};      	// Fire Service Bypass Input Board Address
extern const uint8 BPB [2] = {BIT5,BIT3};       	// Fire Service Bypass Input Bit Number
extern const uint8 BPD [2] = {4,144};          	// Fire Service Bypass Input Data Array Number

extern const int16 MES [2] = {BRD2B,BRD49A};      // Main Eggress Smoke Detector Board Address
extern const uint8 MESB [2] = {BIT3,BIT4};       	// Main Eggress Smoke Detector Bit Number
extern const uint8 MESD [2] = {4,144};         	// Main Eggress Smoke Detector Data Array Number

extern const int16 ALT [2] = {BRD2B,BRD49A};      // Fire Service Alternate Floor Input Board Address
extern const uint8 ALTB [2] = {BIT4,BIT5};		// Fire Service Alternate Floor Input Bit Number
extern const uint8 ALTD [2] = {4,144};			// Fire Service Alternate Floor Input Data Array Number

extern const int16 MRS [2] = {BRD2B,BRD49A};     	// Machine Room Smoke Detector Input Board Address
extern const uint8 MRSB [2] = {BIT2,BIT6};     	// Machine Room Smoke Detector Input Bit Number
extern const uint8 MRSD [2] = {4,144};      	  	// Machine Room Smoke Detector Input Data Array Number

extern const int16 HWS [2] = {BRD2B,BRD49A};     	// Hoistway Smoke Input Board Address
extern const uint8 HWSB [2] = {BIT1,BIT7};     	// Hoistway Smoke Input Bit Number
extern const uint8 HWSD [2] = {4,144};	        // Hoistway Smoke Input Data Array Number

extern const int16 HWS2 [2] = {BRD2A,BRD49A};    	// Hoistway Smoke 2 Input Board Address
extern const uint8 HWS2B [2] = {BIT2,BIT8};     	// Hoistway Smoke 2 Input Bit Number
extern const uint8 HWS2D [2] = {3,144};        	// Hoistway Smoke 2 Input Data Array Number

extern const int16 EMP [2] = {BRD2A,BRD49B};      // Emergency Power Input Board Address
extern const uint8 EMPB [2] = {BIT8,BIT1};       	// Emergency Power Input Bit Number
extern const uint8 EMPD [2] = {3,145};          	// Emergency Power Input Data Array Number

extern const int16 EPS [2] = {BRD2A,BRD49B};     	// Emergency Power Select Input Board Address
extern const uint8 EPSB [2] = {BIT7,BIT2};     	// Emergency Power Select Input Bit Number
extern const uint8 EPSD [2] = {3,145};	        // Emergency Power Select Input Data Array Number

// Note: EPT is the same i/o location as EMSH

extern const int16 EPT[3] = {BRD2A,BRD2C,BRD49B};		// Emergency Power Transfer Input Board Address
extern const uint8 EPTB[3] = {BIT6,BIT2,BIT3};	 	// Emergency Power Transfer Input Bit Number
extern const uint8 EPTD[3] = {3,5,145};				// Emergency Power Transfer Input Data Array

extern const int16 HEOF[2] = {BRD2C,BRD49B};     		// Hall Switch Elevator Off Input Board Address
extern const uint8 HEOFB[2] = {BIT7,BIT4};      		// Hall Switch Elevator Off Input Bit Number
extern const uint8 HEOFD[2] = {5,145};         		// Hall Switch Elevator Off Input Data Array Number

// JAILB Same as HEOF

extern const int16 JAILB[2] = {BRD2C,BRD29B};     	// Jail Break Input Board Address
extern const uint8 JAILBB[2] = {BIT7,BIT4};      		// Jail Break Input Bit Number
extern const uint8 JAILBD[2] = {5,85};         		// Jail Break Input Data Array Number

// HUGI same as HEOF

extern const int16 HUGI[2] = {BRD2C,BRD29B};			// HUGS Detection Input Board Address
extern const uint8 HUGIB[2] = {BIT7,BIT4};  			// HUGS Detection Input Bit Number
extern const uint8 HUGID[2] = {5,85};       			// HUGS Detection Input Data Array Number

	// RTL for traction
extern const int16 RTL[2] = {BRD2C,BRD49B};			// Return to Lobby Input Board Address
extern const uint8 RTLB[2] = {BIT3,BIT5};				// Return to Lobby Input Bit Number
extern const uint8 RTLD[2] = {5,145};	  				// Return to Lobby Input Data Array Number

extern const int16 EMSH[2] = {BRD2A,BRD49B};			// Emergency Medical Service Hall Input	Board Address
extern const uint8 EMSHB[2] = {BIT6,BIT6};			// Emergency Medical Service Hall Input Bit Number
extern const uint8 EMSHD[2] = {3,145};				// Emergency Medical Service Hall Input Data Array

// The P input is used for RTL on hydros prior to 2004 I/O Change.
	// RTL for hydro
extern const int16 RTLH[3] = {BRD4B,BRD2C,BRD49B};	// Return to Lobby Input Board Address
extern const uint8 RTLHB[3] = {BIT1,BIT3,BIT7};		// Return to Lobby Input Bit Number
extern const uint8 RTLHD[3] = {10,5,145};     		// Return to Lobby Input Data Array Number

extern const int16 EMLH[2] = {BRD5C,BRD50C};			// Emergency Medical Hall light output Board Address
extern const uint8 EMLHB[2] = {BIT1,BIT4};			// Emergency Medical Hall light output Bit Number
extern const uint8 EMLHD[2] = {14,149}; 				// Emergency Medical Hall light output Data Array Number

extern const int16 EPLH [2] = {BRD50B,BRD50C};       	// Emergency Power Hall Light Output Board Address
extern const uint8 EPLHB [2] = {BIT1,BIT5};        	// Emergency Power Hall Light Output Bit Number
extern const uint8 EPLHD [2] = {148,149};           	// Emergency Power Hall Light Output Data Array Number

extern const int16 EPRL [2] = {BRD50B,BRD50C};       	// Emergency Power Recall Light Output Board Address
extern const uint8 EPRLB [2] = {BIT2,BIT6};        	// Emergency Power Recall Light Output Bit Number
extern const uint8 EPRLD [2] = {148,149};           	// Emergency Power Recall Light Output Data Array Number

extern const int16 EPSL [2] = {BRD50B,BRD50C};       	// Emergency Power Select Light Output Board Address
extern const uint8 EPSLB [2] = {BIT3,BIT7};        	// Emergency Power Select Light Output Bit Number
extern const uint8 EPSLD [2] = {148,149};           	// Emergency Power Select Light Output Data Array Number

extern const int16 FLH [2] = {BRD50B,BRD50C};       	// Hall Fire Light Output Board Address
extern const uint8 FLHB [2] = {BIT4,BIT8};        	// Hall Fire Light Output Bit Number
extern const uint8 FLHD [2] = {148,149};           	// Hall Fire Light Output Data Array Number



/* Change history

2/11/03 3.02 mhd	1. Correct index for ccs_IO and ccs_start.	Also removed spi13, spi14 and spi15 from 
					   security table.
8/29/03 3.07 mhd	1. Relocate i/o's for LWA, LWB and add LWD.
10/30/03 3.10 mhd.	1. Added EML output.
01/21/05 4.05 mhd.		1. Fixed the following bug in this statement from 
						extern const int16 DCBID [2] = {68.38};  
						to:	extern const int DCBID [2] = {68,38};	
3/1/05 4.07 MHD		1. Relocated 16CR and 16CR.
4/21/05 4.08 mhd	1. Changed ccs security input SECUR to be at the 0th location in the array and to
					   be the first i/o.
5/18/09 v5.48.6 mhd	1. Corrected door output locations for can bus door, DO, DC, NUD and HVD.
12/4/09 v5.50.10 pn 1. Corrected EORB [2] = {BIT5,BIT0} to EORB [2] = {BIT5,BIT1}.
1/23/10 V5.51.9 mhd	1. Moved MBC from hmmap.h.
3/29/10 V5.52.1 mhd 1. Moved HBE, FS2C, ED, DOB, DCB and 7CC.
10/26/10 v5.52.40 mhd	1. Moved hall fire io to board 49.

*/
