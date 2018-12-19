#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"
#include "comm.h"



#define RS232MaxPack 10
#define RS232_COM1 0
#define RS232_COM2 1
#define rbufsize 1000
#define tbufsize 3000
#define InitRS232Test       0
#define SendRS232Message    1
#define RS232MessageDelay   2
#define CheckRS232Message   3
#define RS232SwitchCOM      4
#define RS232TestDone       5
#define RS232TestFailed     6
#define USR1COM 0		// Must be 0 and 1 for USR1 and USR2 comm
#define USR2COM 1

extern int16 rsCOM1_Reply;
extern int16 rsCOM2_Reply;

int16 COMSelect = 0;
int16 RS232PackCnt = 0;
int16 RS232FailedCase = 0;
unsigned char rsCOM1_Tranbuf_Empty = 1; // Com 1-2 Transmiter buffer is empty if 1 has chars if 0
unsigned char rsCOM1_Tranbuf_Full= 0;	// Com Transmit buffer is full if 1 and 0 if not
int16 rsCOM1_Com_Mode = 0;
int16 RS232_SELECT = 0;
int16 SendRS232Pack = 0;

int16 RS_232_TestStatus = 0; // pass = 1; fail = 2 (wbg)

int16 rsCOM2_rbs = 0; 						// receive buffer start pointer com 1-2
int16 rsCOM2_rbf = 0; 						// receive buffer end pointer com 1-2
int16 rsCOM2_tbs = 0;             			// trasmit buffer start pointer com 1-2
int16 rsCOM2_tbf = 0;             			// trasmit buffer end pointer com 1-2
int16 rsCOM2_RxPointer = 0;
int16 rsCOM2_Tran_Int = 0;				// comunication port transmit interupt running
int16 rsCOM2_Recv_Int = 0;				// comunication port transmit interupt running
unsigned char rsCOM2_Tranbuf_Empty = 1; // Com 1-2 Transmiter buffer is empty if 1 has chars if 0
unsigned char rsCOM2_Tranbuf_Full= 0;	// Com Transmit buffer is full if 1 and 0 if not

uint8 rsCOM1_tranbuf[tbufsize+1];
int16 rsCOM1_tbs = 0;             			// trasmit buffer start pointer com 1-2
int16 rsCOM1_tbf = 0;             			// trasmit buffer end pointer com 1-2
int16 rsCOM1_Tran_Int = 0;

int32 COM1_TO_COM2_PACK_SENT = 0;
int32 COM1_TO_COM2_PACK_RECEIVE = 0;
int32 COM2_TO_COM1_PACK_SENT = 0;
int32 COM2_TO_COM1_PACK_RECEIVE = 0;
int32 COM1_TO_COM3_PACK_SENT = 0;
int32 COM1_TO_COM3_PACK_RECEIVE = 0;
int32 COM3_TO_COM1_PACK_SENT = 0;
int32 COM3_TO_COM1_PACK_RECEIVE = 0;
int16 RS232TestCase = 0;
char Fault[2][25];
char DetailFault[2][25];
int16 xUpButtonStatus = 0;
int16 xDownButtonStatus = 0;
int16 xEnterButtonStatus = 0;
int16 xModeButtonStatus = 0;
int32 TESTCASE_1100 = 0;

int16 The_RS_232_test(int16 Operation);
void RS_233_Tx_Rx_Packets(void);
void TEST_RS232(void);
void RSCOM1_Diag (void);
void RSCOM2_Diag (void);
void RSCOM1_Unpack_DataIn(void);
void RSCOM2_Unpack_DataIn(void);
int16 RSCOM1_isrxempty(void);
int16 RSCOM2_isrxempty(void);
void ClrRS232Cnt(void);
void ClrFaultStringBuf(void);
void RSCOM1_Pack_DataOut(void);
void RSCOM2_Pack_DataOut(void);
void RSCOM1_sendc(int16 letter);
void RSCOM2_sendc(int16 letter);

enum {
    Init_RS_232_Test,
    Wait_for_Operator_Response,
    Send_Recieve_Packets,
    RS_232_test_Done,
    RS_232_test_Fail,
    RS_232_test_Pass,
    RS_232_test_Wait_State,
    RS_232_Now_Testing,
    COM1_to_COM2,
    GALX1100_TESTDONE,
    GALX1100_FAILED,
    Display_TestFailed, // Required for Retest Option
    Display_TestPass, // Required for Retest Option
    Display_TestDone, // Required for Retest Option
    Retest_option   // Required for Retest Option    
};



#define TEST_RS_232_Fault_Display_Index	2
char TEST_RS_232_Fault_Display[TEST_RS_232_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC16,20   CN7,8     ",	
};








/**********************************************
* Function:                                   *
*   int16 The_RS_232_test(int16 Operation)    *
*                                             *
* Description: Sends packets from COM1 to     *
*     COM2, and then from COM2 to COM1.       *
*     Checks to be sure that the number of    *
*     packets sent = number received.         *
*                                             *
*                                             *
*                                             *                                              
* Created: September, 2017                    *
*                                             *                                     
*                                             *
*                                             *
* Name: William Grauer                        *
*                                             *
**********************************************/

int16 The_RS_232_test(int16 Operation)  // externed in // GALX_1132_TEST.h
{
    static int16 NextCaseAfterWait = 0;
    static int16 RS_232_TestCase = Init_RS_232_Test;
    static int16 Delay_Count = 3;
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option    
    int16 The_Operation;
    int16 m = 0;
    int16 n = 0;

    The_Operation = Operation;
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(RS_232_TestCase) {
        case Init_RS_232_Test:
            Clear_Log_Buffer(); 
            clr_lcd_dply(); // clear 4-line display 
            CurrentTestPassFail =0;  // fail
            ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
            Retest_Option_Parm = 0; // Required for Retest Option
            RetestResponse = 1; // Required for Retest Option            
            RS232_SELECT = RS232_COM1;
            rsCOM1_Reply = 0;
            rsCOM2_Reply = 0;
            RS_232_TestStatus = 0;
            USR1port.CR1.B.SBR = USR2port.CR1.B.SBR = RS485_Baud;
            New_write_display("RS232 TEST          ",0,0);
            New_write_display("Set Jumper to 2&3   ",1,0);
            New_write_display("ENTER key when ready",2,0);        
            
            RS_232_TestCase = Wait_for_Operator_Response; // RS_232_Now_Testing;
            break;

        case Wait_for_Operator_Response:
            if ( Check_for_Enter_Button() )
            {
            	New_write_display("Test in progress... ",1,0);
            	New_write_display("                    ",2,0);
                New_write_display("                    ",3,0);                        
                RS_232_TestCase = RS_232_Now_Testing;   
            }
            break;

        case RS_232_Now_Testing:
            RS_233_Tx_Rx_Packets();  // Similar to 'RS232_Control' in GALx1100
            // Sends packets from a designated COM.
            TEST_RS232();
            if (RS_232_TestStatus == 1) RS_232_TestCase = RS_232_test_Pass;
            else if (RS_232_TestStatus == 2) RS_232_TestCase = RS_232_test_Fail;
            break;

        case RS_232_test_Done:
            RS_232_TestCase = Init_RS_232_Test;
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  
            break;

        case RS_232_test_Pass:
            CurrentTestPassFail = 1; // pass
            RS_232_TestCase = Init_RS_232_Test;
            clr_lcd_dply();
            New_write_display("RS232 TEST          ",0,0);
            New_write_display("TEST PASSED         ",1,0);
            New_write_display("GOOD JOB            ",2,0);             
            ReturnVal = DONE_BUT_CONTINUE_DISPLAY; // Idle_time
            break;

        case RS_232_test_Fail:
            CurrentTestPassFail =0;  // fail
            //RS_232_TestCase = Init_RS_232_Test;
//-    		clr_lcd_dply();
        	New_write_display(TEST_RS_232_Fault_Display[0],3,0);
        
        	GALX1121_Fault_Display_ADDIN_Function();
        	Log_an_event(TEST_RS_232_Fault_Display[1]);
            RS_232_TestCase = Retest_option; // Required for Retest Option
            ReturnVal = CONTINUE_SERVICE; // Idle_time  
            break;
            
     case Retest_option: // Required for Retest Option
        RetestResponse = Handle_Retest_Request(Retest_Option_Parm++);
        if(RetestResponse == 0) RS_232_TestCase = RS_232_test_Done;
        else if(RetestResponse == 1) RS_232_TestCase = Init_RS_232_Test; 
        else 
        ReturnVal = CONTINUE_SERVICE; // Idle_time    
        break;               

        default:
            RS_232_TestCase = Init_RS_232_Test;
            clr_lcd_dply();
            New_write_display("TEST ERROR          ",1,0);
            New_write_display("default case taken  ",2,0);
            // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
            //return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
            return DEFAULT;
            break;
    }
    return ReturnVal; // Required for Retest Option		// 1
}

void RS_233_Tx_Rx_Packets(void)
{
    switch(RS232_SELECT) {
    case RS232_COM1:
        RSCOM1_Diag();
        RS232_SELECT = RS232_COM2;
        break;
    case RS232_COM2:
        RSCOM2_Diag();
        RS232_SELECT = RS232_COM1;
        break;
    }
    return;
}

void TEST_RS232(void)
{
    static int16 One_Time_Rx = 0;

    switch(RS232TestCase) {
    case InitRS232Test:
        COMSelect = 0;
        RS232FailedCase = 0;
        ClrRS232Cnt();
        ClrFaultStringBuf();
        RS232PackCnt = 0;
        One_Time_Rx = 1;
//        sprintf(src,"RS232 Case init");
//        Add_msgx_First_TimeOnly(src, 2);
        RS232TestCase = SendRS232Message;
        timers[tRefreshInp] = 0;
        break;
    case SendRS232Message:
        rsCOM1_Reply = 1;
        rsCOM2_Reply = 1;
        /*
                sprintf(src,"set Send flags");
                Add_msgx_First_TimeOnly(src, 3);
                Add_msg_to_array_B(src, 3);
                Add_msg_to_array_B(src, 20);
                Add_msg_to_array_B(src, 35);
        */
        RS232TestCase = RS232MessageDelay;
        timers[trsCOM1Delay] = 0;
        break;
    case RS232MessageDelay:
        if(timers[trsCOM1Delay] < 5)
            break;
        RS232TestCase = CheckRS232Message;
        break;
    case CheckRS232Message:
//        sprintf(src,"check Rx after d");
//        Add_msgx_First_TimeOnly(src, 4);
   
   		New_write_display("COM1=     COM2=     ",1,0);
        LCD_Display[1][5] = (char)((COM1_TO_COM2_PACK_SENT%10) + '0');
        LCD_Display[1][15] = (char)((COM1_TO_COM2_PACK_RECEIVE%10) + '0');
            
            
        New_write_display("COM2=     COM1=     ",2,0);
        LCD_Display[2][5] = (char)((COM2_TO_COM1_PACK_SENT%10) + '0');
        LCD_Display[2][15] = (char)((COM2_TO_COM1_PACK_RECEIVE%10) + '0');
        
        
        if(COM1_TO_COM2_PACK_SENT != COM1_TO_COM2_PACK_RECEIVE) {            
//            sprintf(src,"COM1 %d, COM2 %d", COM1_TO_COM2_PACK_SENT,COM1_TO_COM2_PACK_RECEIVE );
//            Add_msgx_First_TimeOnly(src, 5);
            RS232TestCase = RS232TestFailed;
            break;
        }
        if(COM2_TO_COM1_PACK_SENT != COM2_TO_COM1_PACK_RECEIVE) {
//            sprintf(src,"COM2 %d, COM1 %d", COM2_TO_COM1_PACK_SENT,COM2_TO_COM1_PACK_RECEIVE );
//            Add_msgx_First_TimeOnly(src, 6);
            RS232TestCase = RS232TestFailed;
            break;
    	}
         		 		            
//-    	LCD_Display[3][RS232PackCnt] = '.'; // Progress bar
    	RS232PackCnt++;    
    	if(RS232PackCnt< RS232MaxPack)
        	RS232TestCase = SendRS232Message;
    	else RS232TestCase = RS232TestDone;
    break;

    case RS232SwitchCOM:
        break;

    case RS232TestDone:  // passed, send msg
        RS232TestCase = InitRS232Test;
        RS_232_TestStatus = 1; // RS_232_test_Pass;
        break;

    case RS232TestFailed:
        RS232TestCase = InitRS232Test;
        RS_232_TestStatus = 2; // RS_232_test_Fail;
        break;
    }
}
/*****************************************************/
/***        Get Char from receive buffer        ******/
/*****************************************************/

void RSCOM1_Diag (void)
{
    RSCOM1_Unpack_DataIn();
    if(rsCOM1_Reply == 1) {
//        sprintf(src,"send msg from com1");
//        Add_msgx_First_TimeOnly(src, 1);
        RSCOM1_Pack_DataOut();
        rsCOM1_Reply = 0;
    }
}

void RSCOM2_Diag (void)
{
    RSCOM2_Unpack_DataIn();

    if(rsCOM2_Reply == 1) {
        RSCOM2_Pack_DataOut();
        rsCOM2_Reply = 0;
    }
}

/*****************************************************/
/***                UnPack RSCOM1 IN            ******/
/*****************************************************/
void RSCOM1_Unpack_DataIn(void)
{
    int16 MSG = recvm(USR1COM);
    switch(MSG) {
    case 1:
        if((rmbuf[USR1COM][3] == 1)
                &&(rmbuf[USR1COM][4] == 1)
                &&(rmbuf[USR1COM][5] == 1))
            COM2_TO_COM1_PACK_RECEIVE ++;
        break;
    default:
        break;
    }
}

/*****************************************************/
/***                UnPack RSCOM2 IN            ******/
/*****************************************************/
void RSCOM2_Unpack_DataIn(void)
{
    int16 MSG = recvm(USR2COM);
    switch(MSG) {
    case 1:
        if((rmbuf[USR2COM][3] == 1)
                &&(rmbuf[USR2COM][4] == 1)
                &&(rmbuf[USR2COM][5] == 1))
            COM1_TO_COM2_PACK_RECEIVE ++;
        break;
    default:
        break;
    }
}

/*********************************************************/
/**             	Clear  RS232 Count                  **/
/*********************************************************/

void ClrRS232Cnt(void)
{
    RS232_SELECT = 0;
    SendRS232Pack = 0;
    COM1_TO_COM2_PACK_SENT = 0;
    COM1_TO_COM2_PACK_RECEIVE = 0;
    COM2_TO_COM1_PACK_SENT = 0;
    COM2_TO_COM1_PACK_RECEIVE = 0;
    COM1_TO_COM3_PACK_SENT = 0;
    COM1_TO_COM3_PACK_RECEIVE = 0;
    COM3_TO_COM1_PACK_SENT = 0;
    COM3_TO_COM1_PACK_RECEIVE = 0;
    rsCOM2_Reply = 0;
}

/*********************************************************/
/**             Clear Fault String buffer               **/
/*********************************************************/

void ClrFaultStringBuf(void)
{
    int i;
    char  TopLine[25]=     "       No Fault         ";
    char  BottomLine[25] = "                        ";

    for(i = 0; i<25; i++) {
        Fault[0][i] = TopLine[i];
        Fault[1][i] = BottomLine[i];
        DetailFault[0][i] =' ';
        DetailFault[1][i] =' ';
    }
}

/*****************************************************/
/***                RSCOM1  Pack Out            ******/
/*****************************************************/
// /*
void RSCOM1_Pack_DataOut(void)
{
    if(Tranbuf_Empty[USR1COM]) {
        sendc(USR1COM,'R');
        sendc(USR1COM,7);
        sendc(USR1COM,1);
        sendc(USR1COM,1);
        sendc(USR1COM,1);
        sendc(USR1COM,1);
        sendc(USR1COM,'$');
        COM1_TO_COM2_PACK_SENT++;
    }
}

/*****************************************************/
/***                RSCOM2  Pack Out            ******/
/*****************************************************/

void RSCOM2_Pack_DataOut(void)
{
    if(Tranbuf_Empty[USR2COM]) {
        sendc(USR2COM,'R');
        sendc(USR2COM,7);
        sendc(USR2COM,1);
        sendc(USR2COM,1);
        sendc(USR2COM,1);
        sendc(USR2COM,1);
        sendc(USR2COM,'$');
        COM2_TO_COM1_PACK_SENT++;
    }
}

/*****************************************************/
/***             RSCOM1 Send Function           ******/
/*****************************************************/
void RSCOM1_sendc(int16 letter)
{
    _disable();
    rsCOM1_tranbuf[rsCOM1_tbs] = (uint8)letter;
    rsCOM1_tbs++; // tagwbg_05 # 221 rs_com1.c
    if(rsCOM1_tbs >= tbufsize)
        rsCOM1_tbs = 0;
    if (rsCOM1_tbs >= rsCOM1_tbf) {
        if ((rsCOM1_tbf + (tbufsize - rsCOM1_tbs)) < 50)
            rsCOM1_Tranbuf_Full = 1;
    } else {
        if ((rsCOM1_tbf - rsCOM1_tbs) < 50)
            rsCOM1_Tranbuf_Full = 1;
    }
    rsCOM1_Tran_Int = 0;
    rsCOM1_Tranbuf_Empty = 0;

    USR1port.CR1.B.TIE = 1;  // Transmit Interrupt Enabled
    _enable();
}



