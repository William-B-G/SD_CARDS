#include "global.h"
#include "MPC5668G.h"
#include "SLCAN.h"



void CAN_Control(void);
void CAN_Init(void);
void ClrCanCnt(void);
void CanRequest(int ch);

int SendCanPack = 0;
int CanInDiagMode = 0;
int16 CanPackCnt = 0;


#define canA 0
#define canB 1
#define canC 2
#define canD 3

int16 CAN_SELECT = 0;

int16 A_TxCnt = 0;
int16 B_TxCnt = 0;
int16 C_TxCnt = 0;
int16 D_TxCnt = 0;

int16 A_RxCnt = 0;
int16 B_RxCnt = 0;
int16 C_RxCnt = 0;
int16 D_RxCnt = 0;


int16 A_TxDebugCnt = 0;
int16 B_TxDebugCnt = 0;
int16 C_TxDebugCnt = 0;
int16 D_TxDebugCnt = 0;

int16 A_RxDebugCnt = 0;
int16 B_RxDebugCnt = 0;
int16 C_RxDebugCnt = 0;
int16 D_RxDebugCnt = 0;


int16 A_req = 0;
int16 B_req = 0;
int16 C_req = 0;
int16 D_req = 0;



uint8 CanA_TxErrCnt = 0;
uint8 CanA_RxErrCnt = 0;
uint8 CanB_TxErrCnt = 0;
uint8 CanB_RxErrCnt = 0;
uint8 CanC_TxErrCnt = 0;
uint8 CanC_RxErrCnt = 0;
uint8 CanD_TxErrCnt = 0;
uint8 CanD_RxErrCnt = 0;












/*********************************************************/
/**             	   CAN Init                         **/
/*********************************************************/

void CAN_Init(void)
{
    CAN_SELECT = 0;
//-	Init_CanA_bus();
    Init_CanB_bus();
//-	Init_CanC_bus();
//-	Init_CanD_bus();
    return;
}

/*********************************************************/
/**             	 CAN Control                        **/
/*********************************************************/
void CAN_Control(void)
{
    CAN_B_Control();

    /* //-
    	switch(CAN_SELECT)
    	{
    		case canA:
    			CAN_A_Control();
    			CAN_SELECT = canB;
    			break;
    		case canB:
    			CAN_B_Control();
    			CAN_SELECT = canC;
    			break;
    		case canC:
    			CAN_C_Control();
    			CAN_SELECT = canD;
    			break;
    		case canD:
    			CAN_D_Control();
    			CAN_SELECT = canA;
    			break;
    	}
    */
    return;
}

/*********************************************************/
/**             	Clear  CAN Count                    **/
/*********************************************************/

void ClrCanCnt(void)
{
    CAN_SELECT = 0;
    SendCanPack = 0;
    A_TxCnt = 0;
    B_TxCnt = 0;
    C_TxCnt = 0;
    D_TxCnt = 0;

    A_RxCnt = 0;
    B_RxCnt = 0;
    C_RxCnt = 0;
    D_RxCnt = 0;

    CanA_TxErrCnt = 0;
    CanA_RxErrCnt = 0;
    CanB_TxErrCnt = 0;
    CanB_RxErrCnt = 0;
    CanC_TxErrCnt = 0;
    CanC_RxErrCnt = 0;
    CanD_TxErrCnt = 0;
    CanD_RxErrCnt = 0;

    CanPackCnt = 0;

    A_TxDebugCnt = 0;
    B_TxDebugCnt = 0;
    C_TxDebugCnt = 0;
    D_TxDebugCnt = 0;

    A_RxDebugCnt = 0;
    B_RxDebugCnt = 0;
    C_RxDebugCnt = 0;
    D_RxDebugCnt = 0;

    A_req = 0;
    B_req = 0;
    C_req = 0;
    D_req = 0;
}

/*********************************************************/
/**             	Request Pack 10                     **/
/*********************************************************/
void CanRequest(int ch)
{
    if(ch == 1)
        A_req = 1;
    else if(ch == 2)
        B_req = 1;
    else if(ch == 3)
        C_req = 1;
    else if(ch == 4)
        D_req = 1;
}
































































