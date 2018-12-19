



typedef union {
    unsigned short int word;
    unsigned char byte[2];
} COMMWORD_TYPE;


struct SebTag {
    unsigned char IN[3];
    unsigned char SET_OUT[3];
    unsigned char OUT[3];
    unsigned char OUT_ACK[3];
    unsigned char IN_ACK;
    unsigned char ADDR;
    unsigned char MsgReq;
    unsigned char io_Label_ix;
    unsigned char Online;
    unsigned char RxCnt;
    unsigned char Version;
    unsigned char Revision;
    unsigned char TxReSendCnt;
};
extern struct SebTag SEB;


extern int SendCanPack;
extern int CanInDiagMode;
extern int16 CanPackCnt;

extern int16 A_TxCnt;
extern int16 B_TxCnt;
extern int16 C_TxCnt;
extern int16 D_TxCnt;

extern int16 A_RxCnt;
extern int16 B_RxCnt;
extern int16 C_RxCnt;
extern int16 D_RxCnt;


extern int16 A_TxDebugCnt;
extern int16 B_TxDebugCnt;
extern int16 C_TxDebugCnt;
extern int16 D_TxDebugCnt;

extern int16 A_RxDebugCnt;
extern int16 B_RxDebugCnt;
extern int16 C_RxDebugCnt;
extern int16 D_RxDebugCnt;


extern uint8 CanA_TxErrCnt;
extern uint8 CanA_RxErrCnt;
extern uint8 CanB_TxErrCnt;
extern uint8 CanB_RxErrCnt;
extern uint8 CanC_TxErrCnt;
extern uint8 CanC_RxErrCnt;
extern uint8 CanD_TxErrCnt;
extern uint8 CanD_RxErrCnt;

extern int16 A_req;
extern int16 B_req;
extern int16 C_req;
extern int16 D_req;



extern void CAN_Control(void);
extern void CAN_Init(void);
extern void ClrCanCnt(void);
void CanRequest(int ch);


