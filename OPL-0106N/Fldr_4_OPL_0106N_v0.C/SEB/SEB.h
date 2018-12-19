// SEB.h


enum IoType {NOTYPE,SEBTYPE,TOCTYPE,SWTYPE};

enum SebSignalOutEnum
{
    CSS_SEB_OUT,   // 0  CSS    SEB1 IO1; InspectionTest.c
    C120_SEB_OUT,  // 1  C120  SEB2 IO6 ; #405, 434, 438,443, 448,452  Diagnostic.c
    L15_SEB_OUT,   // 2  L15    SEB2 IO8 ; #78 CLBT.c; #435, 439, 453 Diagnostic.c
    CLC_SEB_OUT,   // 3  CLC    SEB3 IO7; #444, 449 Diagnostic.c
    FSS_SEB_OUT,   // 4  FSS    SEB2 IO10; IOTest.c ( IoTestList[][], DiagIO_Map[22][6] )
    CST_SEB_OUT,   // 5  CST    SEB2 IO11; IOTest.c IoTestList[][], case 8
    ICC_SEB_OUT,   // 6  ICC    SEB2 IO12; IOTest.c IoTestList[][], case 8
    ICA_SEB_OUT,   // 7  ICA    SEB3 IO1; IOTest.c IoTestList[][], case 8
    ICI_SEB_OUT,   // 8  ICI    SEB3 IO2; IOTest.c IoTestList[][], case 8
    ACC_SEB_OUT,   // 9  ACC    SEB3 IO3; IOTest.c IoTestList[][], case 8
    BELL_SEB_OUT,  // 10 BELL  SEB3 IO4 ; #50 CLBT.c
    C24_SEB_OUT,   // 11 C24    SEB4 IO1; PwrBuzzTest.c
    L15RL_SEB_OUT, // 12 L15RL SEB2 IO7; #152 CLBT.c; #410 Diagnostic.c // 120VAC to relay on right side (wbg)
    NumSebOut
};

enum SebSignalInEnum
{

    CTA_SEB_IN,   // 0  CTA     SEB1 IO2
    INS_SEB_IN,   // 1  INS SEB1 IO3
    IEN_SEB_IN,   // 2  IEN SEB1 IO4
    IU_SEB_IN,  // 3  IU    SEB1 IO5
    ID_SEB_IN,  // 4  ID    SEB1 IO6
    FANS_SEB_IN,  // 5  FANS    SEB1 IO7
    LIGS_SEB_IN,  // 6  LIGS    SEB1 IO8
    CDL_SEB_IN,   // 7  CDL SEB1 IO9
    CUL_SEB_IN,   // 8  CUL SEB1 IO10
    CTS_SEB_IN,   // 9  CTS SEB1 IO11
    FFSWT_SEB_IN, // 10 FFSWT   SEB1 IO12
    CST_SEB_IN,   // 11 CST SEB2 IO1
    ICC_SEB_IN,   // 12 ICC SEB2 IO2
    ICA_SEB_IN,   // 13 ICA SEB2 IO3
    ICI_SEB_IN,   // 14 ICI SEB2 IO4
    ACC_SEB_IN,   // 15 ACC SEB2 IO5
    C24_SEB_IN,   // 16 C24 SEB4 IO2
    L15_SEB_IN,   // 17 L15 SEB3 IO5
    L25_SEB_IN,   // 18 L25 SEB3 IO6
    NumSebIn
};

enum TocSignalOutEnum
{
	IFL_TOC_OUT,		// 0  IFL  IO7      // Fire Phase I light? (wbg)
	IFB_TOC_OUT,		// 1  IFB  IO8      // Fire Buzzer? (wbg)
	CUL_TOC_OUT,		// 2  CUL  IO9      // Cab up light? (wbg)
	CDL_TOC_OUT,		// 3  CDL  IO10     // Cab down light? (wbg)
	FANR_TOC_OUT,		// 4  FANR  IO11
	LIGR_TOC_OUT,		// 5  LIGR  IO12
	NumTocOut			
};

enum TocSignalInEnum
{
	RLS_TOC_IN,		// 0  RLS  IO1
	CSS_TOC_IN,		// 1  CSS  IO2
	CTS_TOC_IN,		// 2  CTS  IO3
	EE_TOC_IN, 		// 3  EE   IO4
	NumTocIn		
};

