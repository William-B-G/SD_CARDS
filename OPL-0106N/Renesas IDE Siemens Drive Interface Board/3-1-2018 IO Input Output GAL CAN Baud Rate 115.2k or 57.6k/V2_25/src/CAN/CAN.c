/*
 * CAN.c
 *
 *  Created on: Apr 8, 2014
 *      Author: williec
 */



#include "../Movfr.h"
#include "../Timer/Timer.h"
#include "../StarterCodes/iodefine.h"

/* CAN STATE CODES */
#define		R_CAN_STATUS_ERROR_ACTIVE   (int)0x0000001
#define		R_CAN_STATUS_ERROR_PASSIVE  (int)0x0000002
#define		R_CAN_STATUS_BUSOFF 		(int)0x0000004

void SetCAN_ADDR(unsigned char NewAddr);
void Init_CAN(void);
void Init_CAN_Baud(void);
void Enable_CAN_Port(void);
void Disable_CAN_Port(void);
int Config_CTLR_CanRegister(void);
int ExitSleepMode(void);
int EnterSleepMode(void);
int EnterResetMode(void);
int EnterHaltMode(void);
int EnterOperationalMode(void);
void Config_RxMailBox(unsigned char Packet);
void Config_TxMailBox(unsigned char Packet);
void PackCan(unsigned char Packet,unsigned char TxReq);
void TransmitCan(unsigned char Packet);
int TxDone(unsigned char Packet);
int RecievePacketAvailable(unsigned char Packet);
void UnpackCan(unsigned char Packet);
void UpdateDoorStatus(void);
void UpdateInputStatus(void);
void CanControl(void);
int CheckBusErrorState(void);

int TransmitPacket29 (void);
int TransmitPacket12 (void);
int TransmitPacket1 (void);
int TransmitPacket14 (void);





unsigned char TxPacketList[4] ={29,12,1,14};
unsigned char RxPacketList[4] ={28,1,12,14};
unsigned char Packet29TxReqFlg = 0;
unsigned char Packet12TxReqFlg = 0;
unsigned char Packet14TxReqFlg = 0;
int RxCnt = 0;


enum{RegisterNotAccessable,RegisterConfig};
enum{OperationMode,ResetMode,HaltMode,ResetForceMode};
enum{ModeChangeOk,ModeChangeFailed};



unsigned char CanAddr = 7;
unsigned char PacketReceived = 0;
unsigned char MovfrOnline = 0;

unsigned char InputStatus1 =0;
unsigned char InputStatus2 =0;
unsigned char InputStatus3 =0;

unsigned char CAN_DOOR_HEAVY = 0;
unsigned char CAN_DOOR_CLOSE = 0;
unsigned char CAN_DOOR_NUDGE = 0;
unsigned char CAN_DOOR_OPEN = 0;
unsigned char TxCanInProcess = 0;
unsigned char ActiveTxPacket = 0;


union {
	struct {
		unsigned dol:1;					// door open limie
		unsigned dcl:1;					// door close limit
		unsigned dpm:1;					// door protection module limit
		unsigned unused3:1;				// unused
		unsigned overspeed:1;			// overspeed detected so reopen
		unsigned overtorque:1;			// overtorque detected so reopen
		unsigned det_edge:1;			// ee or detector edge so reopen
		unsigned unused7:1;
	}Bit;
	unsigned char Byte;
} Status_1;


union {
	struct {
		unsigned fault:1;				//fault
		unsigned opening:1;				// door opening
		unsigned closing:1;				// door closing
		unsigned unused:1;				// unused
		unsigned monitor:4;				// monitor value
	};
	unsigned char Byte;
} Status_2;

union {
	struct {
		unsigned Open:1;				//door open
		unsigned Close:1;				// door close
		unsigned Nudge:1;				// door nudge close
		unsigned DZ:1;					// door zone on
		unsigned unused4:1;
		unsigned Heavy:1;				// heavy door
		unsigned Narrow:1;				// Narrow door
		unsigned unused7:1;				// unused
	};
	unsigned char Byte;
} Command;

unsigned char ret_status_1;			// return status 1 from car
unsigned char prev_Status_1 =0x04;			// previoius status 1 //  Dpm has reverse logic
unsigned char data_request;			// Monitor data request


#define ON 1
#define OFF 0




/* Bit set defines */
#define		MBX_0		0x00000001
#define		MBX_1		0x00000002
#define		MBX_2		0x00000004
#define		MBX_3		0x00000008
#define		MBX_4		0x00000010
#define		MBX_5		0x00000020
#define		MBX_6		0x00000040
#define		MBX_7		0x00000080
#define		MBX_8		0x00000100
#define		MBX_9		0x00000200
#define		MBX_10		0x00000400
#define		MBX_11		0x00000800
#define		MBX_12		0x00001000
#define		MBX_13		0x00002000
#define		MBX_14		0x00004000
#define		MBX_15		0x00008000
#define		MBX_16		0x00010000
#define		MBX_17		0x00020000
#define		MBX_18		0x00040000
#define		MBX_19		0x00080000
#define		MBX_20		0x00100000
#define		MBX_21		0x00200000
#define		MBX_22		0x00400000
#define		MBX_23		0x00800000
#define		MBX_24		0x01000000
#define		MBX_25		0x02000000
#define		MBX_26		0x04000000
#define		MBX_27		0x08000000
#define		MBX_28		0x10000000
#define		MBX_29		0x20000000
#define		MBX_30		0x40000000
#define		MBX_31		0x80000000

/******************************************************************************
Constant definitions
*****************************************************************************/
/* Mem. area for bit set defines */
static const int 	MailBoxBitMap[32] = { MBX_0,  MBX_1,  MBX_2,  MBX_3,
										MBX_4,  MBX_5,  MBX_6,  MBX_7,
										MBX_8,  MBX_9,  MBX_10, MBX_11,
										MBX_12, MBX_13, MBX_14, MBX_15,
										MBX_16, MBX_17, MBX_18, MBX_19,
										MBX_20, MBX_21, MBX_22, MBX_23,
										MBX_24, MBX_25, MBX_26, MBX_27,
										MBX_28, MBX_29, MBX_30, MBX_31,
										};



/************************************/
/**     	Set Can Addr           **/
/************************************/
void SetCAN_ADDR(unsigned char NewAddr)
{
	CanAddr = NewAddr;
	return;
}



/************************************/
/**     	Init CAN               **/
/************************************/
void Init_CAN(void)
{
	int i,j;

	Enable_CAN_Port();
	SYSTEM.MSTPCRB.BIT.MSTPB0  =  0;
	if(ExitSleepMode() == ModeChangeOk)
	{
		if(EnterResetMode() == ModeChangeOk)
		{
			Config_CTLR_CanRegister();
			Init_CAN_Baud();

			//set the receiver mask
			CAN0.MKIVLR = 0xFFFFFFFF; //  only  Mailbox 0 --> 4
			if(EnterHaltMode() == ModeChangeOk)
			{
				for (i = 0; i < 32; i++)
				{
					CAN0.MB[i].ID.LONG = 0x00;
					CAN0.MB[i].DLC.WORD = 0x00;
					for (j = 0; j < 8; j++)
						CAN0.MB[i].DATA[j] = 0x00;
					for (j = 0; j < 2; j++)
						CAN0.MB[i].TS.WORD = 0x00;
				}

				 //  Configure Rx Mailbox 1,28
				 Config_RxMailBox(1);
				 Config_RxMailBox(28);


				 Config_RxMailBox(12);
				 CAN0.MB[12].ID.BIT.RTR =1;

				 Config_RxMailBox(14);
				 CAN0.MB[14].ID.BIT.RTR =1;

				 //  Configure Tx Mailbox 12,29
				 Config_TxMailBox(29);


				 CAN0.MIER = 0;  // Make Sure Interrupt is Disable
			}
			else
			{
				// Leave  Room  in case  I want to Log Errors
			}

			if(EnterOperationalMode() == ModeChangeOk)
			{
				CAN0.EIFR.BYTE = 0;
				CAN0.ECSR.BYTE = 0;
				CAN0.RECR = 0;
				CAN0.TECR = 0;

//Modify
				//Set the TSRC bit to 1 in CAN operation mode,
				//Time Stamp Counter
				CAN0.CTLR.BIT.TSRC=1;
			}
			else
			{
				// Leave  Room  in case  I want to Log Errors
			}
		}
		else
		{
			// Leave  Room  in case  I want to Log Errors
		}
	}
	else
	{
		// Leave  Room  in case  I want to Log Errors
	}


}





//CAN0 ALL Register Descriptions (Hardware Manual Page 1075)

/************************************/
/**        Init CAN baud           **/
/************************************/
void Init_CAN_Baud(void)
{
	/*  MainCLock = 9,216,000 MHz */
	CAN0.BCR.BIT.SJW = 0;

//ZYM 3-1-2018

	//GAL CAN Baud Rate flag 1 -->  57.6k    0 --> 115.2k
	if(CAN_BaudRate_flag)
	{
		CAN0.BCR.BIT.BRP = 9; // 57600=115200/2 =  9,216,000/((BRP + 1) * 16)
	}
	else
	{
		CAN0.BCR.BIT.BRP = 4; // 115200 =  9,216,000/((BRP + 1) * 16)
	}

	CAN0.BCR.BIT.TSEG1 = 10; // 11 Tq
	CAN0.BCR.BIT.TSEG2 = 3; //  4 Tq
}





/***************************/
/**     Enable CAN Port   **/
/***************************/
void Enable_CAN_Port(void)
{

	IOPORT.PFJCAN.BIT.CANE = 1;

	/* PB5 as CTX0 and PB6 as CRX0. */
	PORTB.DDR.BIT.B5 = 1;
	PORTB.DDR.BIT.B6 = 0;


	/* Set ICR for PB6 (CRX0) in order to connect input to CAN periph. */
	PORTB.ICR.BIT.B6 = 1;

}


/***************************/
/**     Disable CAN Port  **/
/***************************/
void Disable_CAN_Port(void)
{

	IOPORT.PFJCAN.BIT.CANE = 0;
	PORTB.ICR.BIT.B6 = 0;
	return;
}



/************************************************/
/**     Configure CTLR Register in Resetmode   **/
/************************************************/
int Config_CTLR_CanRegister(void)
{
	//CAN0.STR.

	if(CAN0.STR.BIT.RSTST != 1) // make sure its  in Reset mode
		return RegisterNotAccessable;

	CAN0.CTLR.BIT.BOM = 0;  // Bus Off Recovery Mode Select, 0--> Set to Normal Mode  (Hardware Manual page 1076)
	CAN0.CTLR.BIT.MBM = 0;  // MailBox Mode Select,  0 --> Set to Normal MailBox Mode (Hardware Manual page 1076)
	CAN0.CTLR.BIT.IDFM =1;  // ID Format Mode Select, 1 --> Set to Extended ID mode (Hardware Manual page 1076)
	CAN0.CTLR.BIT.MLM = 1;  // Message Lost Mode Select, 1 -->  Overrun mode (Hardware Manual page 1076)
	CAN0.CTLR.BIT.TPM = 0;  // Transmission Priority Mode Select,  0 --> ID priority transmit mode
	CAN0.CTLR.BIT.TSRC = 0;
	CAN0.CTLR.BIT.TSPS = 3;

	return RegisterConfig;
}





/************************************************/
/**              Exit Sleep Mode               **/
/************************************************/
int ExitSleepMode(void)
{
	int CheckCnt = 0;
	CAN0.CTLR.BIT.SLPM = 0;
	while((CAN0.STR.BIT.SLPST == 1) && (CheckCnt < 8192))
		CheckCnt++;

	if(CheckCnt >= 8192)
		return ModeChangeFailed;
	else
		return ModeChangeOk;
}


/************************************************/
/**              Enter Sleep Mode              **/
/************************************************/
int EnterSleepMode(void)
{
	int CheckCnt = 0;
	CAN0.CTLR.BIT.SLPM = 1;

	while((CAN0.STR.BIT.SLPST == 0) && (CheckCnt < 8192))
		CheckCnt++;

	if(CheckCnt >= 8192)
		return ModeChangeFailed;
	else
		return ModeChangeOk;
}

/************************************************/
/**              Enter Reset Mode              **/
/************************************************/
int EnterResetMode(void)
{
	int CheckCnt = 0;
	CAN0.CTLR.BIT.CANM = ResetMode;

	while((CAN0.STR.BIT.RSTST == 0) && (CheckCnt < 8192))
		CheckCnt++;

	if(CheckCnt >= 8192)
		return ModeChangeFailed;
	else
		return ModeChangeOk;
}

/************************************************/
/**              Enter Halt Mode               **/
/************************************************/
int EnterHaltMode(void)
{
	int CheckCnt = 0;
	CAN0.CTLR.BIT.CANM = HaltMode;

	while((CAN0.STR.BIT.HLTST == 0) && (CheckCnt < 8192))
		CheckCnt++;

	if(CheckCnt >= 8192)
		return ModeChangeFailed;
	else
		return ModeChangeOk;
}

/************************************************/
/**            Enter Operational Mode          **/
/************************************************/
int EnterOperationalMode(void)
{
	int CheckCnt = 0;
	CAN0.CTLR.BIT.CANM = OperationMode;

	while(((CAN0.STR.BIT.HLTST == 1)||(CAN0.STR.BIT.RSTST == 1)) && (CheckCnt < 8192))
		CheckCnt++;

	if(CheckCnt >= 8192)
		return ModeChangeFailed;
	else
		return ModeChangeOk;
}



//MB setting Hardware Manual Page 1086
//MKR setting Hardware Manual Page 1082
//MKIVLR setting Hardware Manual Page 1085
//MCTL setting Hardware Manual Page 1092
/************************************************/
/**            Config_RxMailBox                **/
/************************************************/
void Config_RxMailBox(unsigned char Packet)
{

	CAN0.MCTL[Packet].BYTE = 0;
	CAN0.MB[Packet].ID.BIT.SID = ((short int)Packet << 3) | ((short int)CanAddr >> 5);
	CAN0.MB[Packet].ID.BIT.EID = ((short int)CanAddr << 13);
	CAN0.MB[Packet].ID.BIT.IDE = 1;

	// Set Mask
	CAN0.MKR[Packet/4].BIT.SID = 0x7FF;
	CAN0.MKR[Packet/4].BIT.EID = 0x3FFFF;

	CAN0.MKIVLR &= ~(MailBoxBitMap[Packet]);

	CAN0.MCTL[Packet].BYTE = 0x40;

}


/************************************************/
/**            Config_TxMailBox                **/
/************************************************/
void Config_TxMailBox(unsigned char Packet)
{

	CAN0.MCTL[Packet].BYTE = 0;
	CAN0.MB[Packet].ID.BIT.SID = ((short int)Packet << 3) | ((short int)CanAddr >> 5);
	CAN0.MB[Packet].ID.BIT.EID = ((short int)CanAddr << 13);
	CAN0.MB[Packet].ID.BIT.IDE = 1;
}

/************************************************/
/**                Pack CAN                    **/
/************************************************/
void PackCan(unsigned char Packet,unsigned char TxReq)
{

	if(TxReq)
	{
		CAN0.MCTL[Packet].BYTE  = 0;
		CAN0.MB[Packet].DLC.BIT.DLC = 0;
		CAN0.MB[Packet].ID.BIT.RTR = 1;
		TransmitCan(Packet);
	}
	else
	{
		switch(Packet)
		{

			case 12:
				CAN0.MCTL[Packet].BYTE  = 0;
				CAN0.MB[Packet].DLC.BIT.DLC = 2;
				CAN0.MB[Packet].DATA[0] = Version;
				CAN0.MB[Packet].DATA[1] = Revision;
				TransmitCan(Packet);
				break;
			case 14:
				UpdateInputStatus();
				CAN0.MCTL[Packet].BYTE  = 0;
				CAN0.MB[Packet].DLC.BIT.DLC = 3;
				CAN0.MB[Packet].DATA[0] = InputStatus1;
				CAN0.MB[Packet].DATA[1] = InputStatus2;
				CAN0.MB[Packet].DATA[2] = InputStatus3;
				TransmitCan(Packet);
				break;
			case 29:
				CAN0.MCTL[Packet].BYTE  = 0;
				CAN0.MB[Packet].DLC.BIT.DLC = 5;
				CAN0.MB[Packet].DATA[0] = Status_1.Byte;
				CAN0.MB[Packet].DATA[1] = Status_2.Byte;
				CAN0.MB[Packet].DATA[2] = 0;
				CAN0.MB[Packet].DATA[3] = 0;
				CAN0.MB[Packet].DATA[4] = Command.Byte;
				CAN0.MB[Packet].DATA[5] = 0;
				CAN0.MB[Packet].DATA[6] = 0;
				CAN0.MB[Packet].DATA[7] = 0;
				TransmitCan(Packet);
				break;

		}
	}
}



//Hardware Manual page 1092
/************************************************/
/**                Transmit CAN                **/
/************************************************/
void TransmitCan(unsigned char Packet)
{
	int delayCnt = 0;

	CAN0.MCTL[Packet].BIT.TX.TRMREQ = 0;
	while(delayCnt < 20)
		delayCnt++;
	CAN0.MCTL[Packet].BIT.TX.SENTDATA = 0;

	CAN0.MCTL[Packet].BIT.TX.TRMREQ = 1;  // start Transmit

	TxCanInProcess = 1;
	ActiveTxPacket = Packet;

}


//Hardware Manual page 1092
/************************************************/
/**              Check Transmit CAN            **/
/************************************************/
int TxDone(unsigned char Packet)
{
	if(CAN0.MCTL[Packet].BIT.TX.SENTDATA == 1)
		return 1;
	else
		return 0;
}




//Hardware Manual page 1092
/************************************************/
/**                Check Rx Can                **/
/************************************************/
int RecievePacketAvailable(unsigned char Packet)
{

	int delayCnt = 0;

	if((CAN0.MCTL[Packet].BIT.RX.INVALDATA == 0) && (CAN0.MCTL[Packet].BIT.RX.NEWDATA == 1) && (CAN0.MCTL[Packet].BIT.RX.MSGLOST == 0))
		return 1;
	else
	{
		if(CAN0.MCTL[Packet].BIT.RX.MSGLOST == 1)
		{
			delayCnt = 0;
			/*
			    In both overwrite and overrun modes, the MSGLOST flag cannot be set to 0 by writing 0 by a program during five
				peripheral module clock (PCLK) cycles following the sixth bit of EOF
				ICLK = PCLK*8
			 */
			while(delayCnt<40)
				delayCnt++;
			CAN0.MCTL[Packet].BIT.RX.MSGLOST = 0;
		}
		return 0;
	}
}


/************************************************/
/**                Unpack CAN                  **/
/************************************************/
void UnpackCan(unsigned char Packet)
{

	switch(Packet)
	{
		case 1:
			CAN0.MCTL[Packet].BIT.RX.NEWDATA = 0;
			RxCnt = 3;
			break;
		case 12:
			CAN0.MCTL[Packet].BIT.RX.NEWDATA = 0;
			RxCnt = 3;
			break;
		case 14:
			CAN0.MCTL[Packet].BIT.RX.NEWDATA = 0;
			RxCnt = 3;
			break;
		case 28:
			Command.Byte = CAN0.MB[Packet].DATA[0];
			ret_status_1 = CAN0.MB[Packet].DATA[1];
			data_request = CAN0.MB[Packet].DATA[2];
			if(Command.Heavy)
			   	CAN_DOOR_HEAVY = ON;
			else
				CAN_DOOR_HEAVY = OFF;
			if(Command.Close)
			{									//door close
				CAN_DOOR_CLOSE = ON;
				CAN_DOOR_OPEN = OFF;
				if(Command.Nudge)	//nudge
					CAN_DOOR_NUDGE = ON;
				else
					CAN_DOOR_NUDGE = OFF;
			}
			else
			{
				CAN_DOOR_CLOSE = OFF;
				CAN_DOOR_NUDGE = OFF;
			   	if((Command.Open) && (Command.DZ))	//door open and door zone
				   	CAN_DOOR_OPEN = ON;
				else
				   	CAN_DOOR_OPEN = OFF;
			}
			CAN0.MCTL[Packet].BIT.RX.NEWDATA = 0;
			RxCnt = 3;
			break;
	}

}


/************************************************/
/**                Can Control                 **/
/************************************************/

void CanControl(void)
{
	int i = 0;

//-	static int CurrentBusState = 0;
//-	static int PrevBusState = 0;

	int delayCnt = 0;

	UpdateDoorStatus();


//Modify
//while Siemens Interface Board On,
//and GAL Controller got recycle power,
//with this CAN Initialization here, the CAN Communication got Lost, NO GOOD here
//without this loop, CAN Communication works fine
/*///
	CurrentBusState = CheckBusErrorState();

	if(CurrentBusState != PrevBusState)
	{
		if((CurrentBusState == R_CAN_STATUS_ERROR_ACTIVE) && (PrevBusState == R_CAN_STATUS_BUSOFF))
			Init_CAN();
		PrevBusState = CurrentBusState;
	}
*/




	// Check Receive Pack
	for(i=0;i<4;i++)
	{
		if(RecievePacketAvailable(RxPacketList[i]) == 1)
		{

			if(CAN0.MB[RxPacketList[i]].ID.BIT.RTR == 0)
			{
				UnpackCan(RxPacketList[i]);

				if(RxPacketList[i] == 28)
					Packet29TxReqFlg = 1;
				else if(RxPacketList[i] == 1)
					Packet12TxReqFlg = 1;
			}
			else
			{
				if(RxPacketList[i] == 12)
					Packet12TxReqFlg = 1;
				else if(RxPacketList[i] == 14)
					Packet14TxReqFlg = 1;
			}

			break;
		}
	}


	// Check Transmit
	if(TxCanInProcess == 1)
	{
		if(TxDone(ActiveTxPacket) == 1)
		{
			//ActiveTxPacket = 0;
			CAN0.MCTL[ActiveTxPacket].BIT.TX.TRMREQ = 0;
			while(delayCnt < 20)
				delayCnt++;

			CAN0.MCTL[ActiveTxPacket].BIT.TX.SENTDATA = 0;
			if(ActiveTxPacket == 12)
			{
				CAN0.MB[12].ID.BIT.RTR = 1;
				CAN0.MCTL[12].BYTE  = 0;
				CAN0.MCTL[12].BIT.RX.RECREQ = 1;
			}
			else if(ActiveTxPacket == 14)
			{
				CAN0.MB[14].ID.BIT.RTR = 1;
				CAN0.MCTL[14].BYTE  = 0;
				CAN0.MCTL[14].BIT.RX.RECREQ = 1;
			}
			else if(ActiveTxPacket == 1)
			{
				CAN0.MB[1].ID.BIT.RTR = 0;
				CAN0.MCTL[1].BYTE  = 0;
				CAN0.MCTL[1].BIT.RX.RECREQ = 1;
			}

			ActiveTxPacket = 0;
			TxCanInProcess = 0;
		}
	}
	else
	{
		for(i= 0;i<4 ;i++)
		{
			// A buffer is available
			if(TxPacketList[i] == 29)
			{
				if(TransmitPacket29())
					break;
			}
			else if(TxPacketList[i] == 12)
			{
				if(TransmitPacket12())
					break;
			}
			else if(TxPacketList[i] == 14)
			{
				if(TransmitPacket14())
					break;
			}
			else if(TxPacketList[i] == 1)
			{
				if(TransmitPacket1())
					break;
			}
		}
	}

	if(CanRxTimer > 5)
	{
		CanRxTimer  = 0;
		if(RxCnt> 0)
			RxCnt--;
	}

	if(RxCnt > 0)
		MovfrOnline = 1;
	else
		MovfrOnline = 0;

	return;
}

/**************************************************************************/
/*****		              Update CAN Status                           *****/
/**************************************************************************/
void UpdateDoorStatus(void)
{
	if(DOL == 1)
		Status_1.Bit.dol = 1;
	else
		Status_1.Bit.dol = 0;

	if(DCL == 1)
		Status_1.Bit.dcl =1;
	else
		Status_1.Bit.dcl =0;

	if(GSM ==1)
		Status_1.Bit.dpm =0;
	else
		Status_1.Bit.dpm =1;

	if((REOPEN == 1)||(REOPEN_LCSE==1))
		Status_1.Bit.det_edge = 1;
	else
		Status_1.Bit.det_edge = 0;

	Status_2.opening = Command.Open;
	Status_2.closing = Command.Close;

}


/**************************************************************************/
/*****		              Update Input Status                           *****/
/**************************************************************************/
void UpdateInputStatus(void)
{
    //-------------------------------------------------------------
    //Run Switch
	if(RUN_PB)
		InputStatus1 |= 0x01;
	else
		InputStatus1 &= ~(0x01);


	//Setup Switch
	if(CAM_SETUP_PB)
		InputStatus1 |= 0x02;
	else
		InputStatus1 &= ~(0x02);


	//Auto Switch
	if(AUTO_PB)
		InputStatus1 |= 0x04;
	else
		InputStatus1 &= ~(0x04);


	//Manual Switch
	if(MANUAL_PB)
		InputStatus1 |= 0x08;
	else
		InputStatus1 &= ~(0x08);




	//-------------------------------------------------------------
	//Close Push Button
	if(CLOSE_PB)
		InputStatus1 |= 0x10;
	else
		InputStatus1 &= ~(0x10);


	//Open Push Button
	if(OPEN_PB)
		InputStatus1 |= 0x20;
	else
		InputStatus1 &= ~(0x20);


	//Nudging Push Button
	if(NUDGE_PB)
		InputStatus1 |= 0x40;
	else
		InputStatus1 &= ~(0x40);


	//Heavy Push Button
	if(HEAVY_PB)
		InputStatus1 |= 0x80;
	else
		InputStatus1 &= ~(0x80);


	//-------------------------------------------------------------
	//Close Input Card
	if(CLOSE)
		InputStatus2 |= 0x01;
	else
		InputStatus2 &= ~(0x01);


	//Open Input Card
	if(OPEN)
		InputStatus2 |= 0x02;
	else
		InputStatus2 &= ~(0x02);


	//Nudging Input Card
	if(NUDGE)
		InputStatus2 |= 0x04;
	else
		InputStatus2 &= ~(0x04);


	//Heavy Input Card
	if(HEAVY)
		InputStatus2 |= 0x08;
	else
		InputStatus2 &= ~(0x08);


	//DPM CAM
	if(GSM)
		InputStatus2 |= 0x10;
	else
		InputStatus2 &= ~(0x10);


	//AUX CAM
	if(AUX)
		InputStatus2 |= 0x20;
	else
		InputStatus2 &= ~(0x20);


	//SSO/FSO CAM
	if(SSFO_IN)
		InputStatus2 |= 0x40;
	else
		InputStatus2 &= ~(0x40);


	//MSO CAM
	if(SDO_IN)
		InputStatus2 |= 0x80;
	else
		InputStatus2 &= ~(0x80);



	//FSC CAM
	if(SDC_IN)
		InputStatus3 |= 0x01;
	else
		InputStatus3 &= ~(0x01);


	//DOL CAM
	if(DOL)
		InputStatus3 |= 0x02;
	else
		InputStatus3 &= ~(0x02);


	//DCL CAM
	if(DCL)
		InputStatus3 |= 0x04;
	else
		InputStatus3 &= ~(0x04);


	//-------------------------------------------------------------
	//Reopen
	if((REOPEN == 1)||(REOPEN_LCSE==1))
		InputStatus3 |= 0x08;
	else
		InputStatus3 &= ~(0x08);


	// Front Rear  Address  Jumper
	if(FR_Jumper)
		InputStatus3 |= 0x10;
	else
		InputStatus3 &= ~(0x10);
}







/**************************************************************************/
/*****		              CheckBusErrorState                          *****/
/**************************************************************************/
int CheckBusErrorState(void)
{
	int BusState = R_CAN_STATUS_ERROR_ACTIVE;

	if (CAN0.STR.BIT.EST == 1)
	{
		/* Check error-passive state */
		if (CAN0.STR.BIT.EPST == 1)
		{
			BusState = R_CAN_STATUS_ERROR_PASSIVE;
		}

		/* Check bus-off state */
		else if (CAN0.STR.BIT.BOST == 1)
		{
			BusState = R_CAN_STATUS_BUSOFF;
		}
	}

	return BusState;
}

/**************************************************************************/
/*****		              TransmitPacket29                            *****/
/**************************************************************************/
int TransmitPacket29 (void)
{
	static int WaitForAckFlg = 0;

	if (Status_1.Byte != prev_Status_1)
	{
			prev_Status_1 = Status_1.Byte;
			PackCan(29,0);
			CanAckTimer = 0;
			WaitForAckFlg = 1;
			return 1;
	}
	else if(WaitForAckFlg == 1)
	{
		if(CanAckTimer > 1)
		{
			if (ret_status_1 != Status_1.Byte)
			{
				// Pack transmit buffer and send data again
					PackCan(29,0);
					CanAckTimer = 0;
					return 1;
			}
			else
				WaitForAckFlg = 0;
			CanAckTimer = 0;
		}
	}
	else if(Packet29TxReqFlg)
	{
		Packet29TxReqFlg = 0;
		PackCan(29,0);
		return 1;
	}

	return 0;
}

/**************************************************************************/
/*****		              TransmitPacket12                            *****/
/**************************************************************************/
int TransmitPacket12 (void)
{
	if(Packet12TxReqFlg)
	{
		Packet12TxReqFlg = 0;
		CAN0.MB[12].ID.BIT.RTR = 0;
		CAN0.MCTL[12].BIT.RX.RECREQ = 0;
		PackCan(12,0);
		return 1;
	}
	return 0;
}


/**************************************************************************/
/*****		              TransmitPacket1                            *****/
/**************************************************************************/
int TransmitPacket1 (void)
{
	if(MovfrOnline == 0)
	{
		if(CanOfflineTimer >=10)
		{
			CanOfflineTimer = 0;
			CAN0.MCTL[1].BIT.RX.RECREQ = 0;
			PackCan(1,1);
			return 1;
		}
	}
	return 0;
}


/**************************************************************************/
/*****		              TransmitPacket12                            *****/
/**************************************************************************/
int TransmitPacket14 (void)
{
	if(Packet14TxReqFlg)
	{
		Packet14TxReqFlg = 0;
		CAN0.MB[14].ID.BIT.RTR = 0;
		CAN0.MCTL[14].BIT.RX.RECREQ = 0;
		PackCan(14,0);
		return 1;
	}
	return 0;
}


