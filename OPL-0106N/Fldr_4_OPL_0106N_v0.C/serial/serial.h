//********************************
// Global Comm Definitions
//********************************


#define USR1COM 0		// Must be 0 and 1 for USR1 and USR2 comm
#define USR2COM 1
#define TERMCOM 2
#define DRVCOM 3		// used for put and get que
#define GRPCOM 4
#define GCANCOM 5		// used for put and get que
#define HLCOM 6			// used for put and get que

#define SE1COM 0			// Serial Ex ports A and B
#define SE2COM 1

#define SCI_A_TX 0x82
#define SCI_A_RX 0x83
#define SCI_B_TX 0x84
#define SCI_B_RX 0x85
#define SCI_C_TX 0x86
#define SCI_C_RX 0x87
#define SCI_D_TX 0x88
#define SCI_D_RX 0x89
#define SCI_E_TX 0x8A
#define SCI_E_RX 0x8B
#define SCI_F_TX 0x8C
#define SCI_F_RX 0x8D

#define SCI_A_Port 0xFFFA0000L
#define SCI_A_DR SCI_A_Port + 0x0007
#define SCI_A_Int 113
#define SCI_B_Port 0xFFFA4000L
#define SCI_B_DR SCI_B_Port + 0x0007
#define SCI_B_Int 114
#define SCI_C_Port 0xFFFA8000L
#define SCI_C_DR SCI_C_Port + 0x0007
#define SCI_C_Int 115
#define SCI_D_Port 0xFFFAC000L
#define SCI_D_DR SCI_D_Port + 0x0007
#define SCI_D_Int 116
#define SCI_E_Port 0xFFFB0000L
#define SCI_E_DR SCI_E_Port + 0x0007
#define SCI_E_Int 270
#define SCI_F_Port 0xFFFB4000L
#define SCI_F_DR SCI_F_Port + 0x0007
#define SCI_F_Int 271

#define eDMA_Error 10
#define eDMA_0_Int 11
#define eDMA_1_Int 12
#define eDMA_2_Int 13
#define eDMA_3_Int 14
#define eDMA_4_Int 15
#define eDMA_5_Int 16
#define eDMA_6_Int 17
#define eDMA_7_Int 18
#define eDMA_8_Int 19
#define eDMA_9_Int 20
#define eDMA_10_Int 21
#define eDMA_11_Int 22
#define eDMA_12_Int 23
#define eDMA_13_Int 24
#define eDMA_14_Int 25
#define eDMA_15_Int 26
#define eDMA_16_Int 27
#define eDMA_17_Int 28
#define eDMA_18_Int 29
#define eDMA_19_Int 30
#define eDMA_20_Int 31



// DMA Channels Used

#define DRVdmaChTx 1
#define DRVdmaChRx 2
#define GRPdmaChTx 3
#define GRPdmaChRx 4
#define DPYSPIdmaChTx 5
#define DPYSPIdmaChRx 6
#define SDSPIdmaChTx 7
#define SDSPIdmaChRx 8
#define USR1dmaChRx 9
#define USR2dmaChRx 10
#define SE1dmaChRx 11
#define SE2dmaChRx 12
#define USR1dmaChTx 13
#define USR2dmaChTx 14
#define MRSPIdmaChTx 15
#define MRSPIdmaChRx 16
#define IOSPIdmaChTx 17
#define IOSPIdmaChRx 18
#define SE1dmaChTx 19
#define SE2dmaChTx 20


#define USR1port ESCI_A			// Must be port A (TX/RX0)
#define USR1intLoc SCI_A_Int
#define USR1dmaTX  SCI_A_TX
#define USR1dmaRX  SCI_A_RX
#define USR1dr SCI_A_DR
#define USR1dmaTxIntLoc eDMA_13_Int
#define USR1txEn SIU.GPDO[RTS0].R = 0
#define USR1txDis SIU.GPDO[RTS0].R = 1

#define USR2port ESCI_B			// Must be port B (TX/RX1)	
#define USR2intLoc SCI_B_Int
#define USR2dmaTX  SCI_B_TX
#define USR2dmaRX  SCI_B_RX
#define USR2dr SCI_B_DR
#define USR2dmaTxIntLoc eDMA_14_Int
#define USR2txEn SIU.GPDO[RTS1].R = 0
#define USR2txDis SIU.GPDO[RTS1].R = 1

#define DRVport ESCI_C			// Must be port C (TX/RX2)
#define DRVintLoc SCI_C_Int
#define DRVtxEn SIU.GPDO[RTS2].R = 0
#define DRVtxDis SIU.GPDO[RTS2].R = 1
#define DRVdmaTX  SCI_C_TX
#define DRVdmaRX  SCI_C_RX
#define DRVdr SCI_C_DR
#define DRVdmaTxIntLoc eDMA_1_Int
#define DRVdmaRxIntLoc eDMA_2_Int

#define GRPport ESCI_D			// Must be port D (TX/RX3) 
#define GRPintLoc SCI_D_Int
#define GRPtxEn SIU.GPDO[RTS3].R = 0
#define GRPtxDis SIU.GPDO[RTS3].R = 1
#define GRPdmaTX  SCI_D_TX
#define GRPdmaRX  SCI_D_RX
#define GRPdr SCI_D_Port + 0x0006	// Reading and Writing two bytes at a time
#define GRPdmaTxIntLoc eDMA_3_Int
#define GRPdmaRxIntLoc eDMA_4_Int

#define SE1port ESCI_E			// Must be port E (TX/RX4)
#define SE1intLoc SCI_E_Int
#define SE1txEn	SIU.GPDO[RTS4].R = 0
#define SE1txDis SIU.GPDO[RTS4].R = 1
#define SE1dmaTX  SCI_E_TX
#define SE1dmaRX  SCI_E_RX
#define SE1dr SCI_E_DR
#define SE1dmaTxIntLoc eDMA_19_Int

#define SE2port ESCI_F			// Must be port F (TX/RX5)
#define SE2intLoc SCI_F_Int
#define SE2txEn	SIU.GPDO[RTS5].R = 0
#define SE2txDis SIU.GPDO[RTS5].R = 1
#define SE2dmaTX  SCI_F_TX
#define SE2dmaRX  SCI_F_RX
#define SE2dr SCI_F_DR
#define SE2dmaTxIntLoc eDMA_20_Int

#if (d_GRPCOM == 0)
	#include "grpcom.h"
#endif

#if (d_CTCAN == 0)
	#include "ctcan.h"
#endif
#if (d_MRCAN == 0)
	#include "mrcan.h"
#endif
#if (d_GRCAN == 0)
	#include "grcan.h"
#endif

#if (d_COMM == 0)
	#include "comm.h"
#endif
#if (d_COMDIAG == 0)
	#include "comdiag.h"
#endif
#if (d_GALCOM == 0)
	#include "galcom.h"
#endif
#if (d_WIFICOM == 0)
	#include "wificom.h"
#endif
#if (d_IDSCOM == 0)
	#include "idscom.h"
#endif
#if (d_SERIALEX == 0)
	#include "serialex.h"
#endif
#if (d_COMDEBUG == 0)
	#include "comdebug.h"
#endif
#if (d_RTCMRSPI == 0)
	#include "rtcMRspi.h"
#endif
#if (d_DPYSPI == 0)
	#include "dpyspi.h"
#endif

#if (d_SDSPI == 0)
	#include "sdspi.h"
#endif

#if (d_IOSPI == 0)
	#include "iospi.h"
#endif

#if (d_SEL_CAN == 0)
 	#include "sel_can.h"
#endif
#if (d_UPSCOM == 0)
	#include "upscom.h"
#endif
#if (d_IGEPCOM == 0)
	#include "igepcom.h"
#endif
#if (d_DL20COM == 0)
	#include "dl20com.h"
#endif
#if (d_HLCOM == 0)
	#include "hlcom.h"
#endif




