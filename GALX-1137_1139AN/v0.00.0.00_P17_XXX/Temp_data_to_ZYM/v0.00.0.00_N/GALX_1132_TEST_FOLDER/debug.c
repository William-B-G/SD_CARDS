// debug.c
// Utilities for debugging (wbg)

#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "comm.h"
#include <string.h>

#define MAX_MSG_BOARD_INDEX 39

extern void sendstring(int16 comnmb, char *c);
extern void sendc( int16 comnmb, int16 letter);
extern void grp_com_tx_start(void);

void debug_msg(char *msg, int line);
void *Msg_Board = NULL;
char Msg_Board_A[40][20];  // to display messages to the display (wbg)
char Msg_Board_B[40][20];  // to display messages to the display (wbg)
char Msg_Board_C[40][20];  // to display messages to the display (wbg)

int16 Msg_Board_index = 0; // BOARD A
int16 Displayed_msg_page =  0;
int16 Max_Displayed_msg_page = 0; // 0 - 9

int16 Displayed_msg_page_B =  0;
int16 Msg_Board_B_index = 0;
int16 Max_Displayed_msg_page_B = 0; // 0 - 9

int16 Displayed_msg_page_C =  0;
int16 Msg_Board_index_C = 0;
int16 Max_Displayed_msg_page_C = 0; // 0 - 9

int16 LogIndex = 0;   // BOARD A
int16 LogIndex_C = 0; // BOARD C


// ********************************************************
// Displays a msg on a line of display. Use sprintf() and than
//   call debug_msg()
//  (wbg - September 2017)
// ********************************************************

//  sprintf(src,"11: %d %d", USR1port.CR1.B.SBR, USR2port.CR1.B.SBR);
//  debug_msg(src, 3);
// If baud rate is 19.2 kb, the  last line of the display will
// show "11: 354 354         "

/**********************************************
* Function:void debug_msg(char *msg, int line)*
* Description:Sends one line to the 4-line    *
*   display, when 'line' is 0 - 3. If 'line'  *
*   is 4 or 5, it sends (not yet implimented) *
*   the msg to the RS232 port, com 1. If      *
*   'line' is 6 the msg is sent to the Group  *
*   com (RS-485) and if 'line' is 7 the msg   *
*   goes to the Hall com.                     *
*                                             *
*   when the msg goes to a com port, the msg  *
*   is terminated by a line feed, carriage    *
*   return, and a '>' is placed in column 1   *
*   of the new line.                          *
*                                             *
* Created: September, 2017                    *
*                                             *
* Example:                                    *
*                                             *
*                                             *
* Name: William Grauer                        *
*                                             *
*                                             *
*                                             *
*                                             *
**********************************************/

void debug_msg(char *msg, int line)
{
    //  if line > 3 send debug message to a comm port
    if(line < 4) {
        strncpy(&LCD_Display[line][0], msg, 19);
        return;
    } else if (line < 6) {
        // go to RS232 port
        //sendstring(comnmb,&dbg_com_buf[comnmb][0]);
//    	sendstring(line-4, msg); // 19.2 kb
        sendc(line-4,'>');
    }
    // gc_trm_buf
    else if (line == 6) {
        // go to RS485 Group comm port
        gc_trm_buf[0] = ':';				// Start of packet
        gc_trm_buf[1] = '1';
        gc_trm_buf[2] = '3';					// Packet number
        gc_trm_buf[3] = '5';
        gc_trm_buf[4] = '\n';
        gc_trm_buf[5] = '\r';
        gc_trm_buf[6] = '>';
        gc_trm_buf[7] = NULL;

        grp_com_tx_start();  // for group com // GRPport.CR1.B.SBR
    } else if (line == 7) {
        // go to RS485 Hall comm port  // sx_tranbuf[SE1COM][0]
//     strcpy(&sx_tranbuf[SE1COM][0], Text);
        // TODO add the CR,LF,'>' to end of sx_tranbuf[SE1COM][0]
        // sx_enable_tx(SE1COM);
        // se1_tx_dma_start(len+2);
        sx_tranbuf[SE1COM][0] = 'a';
        sx_tranbuf[SE1COM][1] = '1';
        sx_tranbuf[SE1COM][2] = '2';
        sx_tranbuf[SE1COM][3] = '3';
        sx_tranbuf[SE1COM][4] = '\n';
        sx_tranbuf[SE1COM][5] = '\r';
        sx_tranbuf[SE1COM][6] = '>';
        sx_tranbuf[SE1COM][7] = NULL;
        sx_enable_tx(SE1COM);
        se1_tx_dma_start(7);
    }
}


// ********************************************************

// void Fill_up_msg_array(void)
// Fill the Msg_Board array with test messages
// (wbg - September 2017)
// ********************************************************
void Fill_up_msg_array(void) // only for debugging the message board code.
{
    int i = 0;
    char tempArray[22];

    Clear_Msg_Board();
    for ( i = 0; i <= MAX_MSG_BOARD_INDEX; i++) {
        sprintf(tempArray,"Test message %d", i );
        Add_msg_to_array(tempArray);
    }
    Msg_Board_index = 0;
}

// ********************************************************
// int16 Add_msgx_First_TimeOnly(char *msg, int16 MsgTag_0_39)
// Add this message but never add it a second time.
//   (wbg - September 2017)
// ********************************************************
int16 Add_msgx_First_TimeOnly(char *msg, int16 MsgTag_0_39)
{
    if(msgSent[MsgTag_0_39] == 1) {
        Add_msg_to_array(msg);
        msgSent[MsgTag_0_39] = 0;
        return 1;
    }
    return 0;
}


// ********************************************************
// int16 Add_msgx_First_TimeOnly_C(char *msg, int16 MsgTag_0_39)
// Add this message but never add it a second time.
//   (wbg - December 2017)
// ********************************************************
int16 Add_msgx_First_TimeOnly_C(char *msg, int16 MsgTag_0_39)
{
//    if(Functional_Test_is_Running == FALSE) return 0;
    if(msgSent_C[MsgTag_0_39] == 1) {
        Add_msg_to_array_C(msg);
        msgSent_C[MsgTag_0_39] = 0;
        return 1;
    }
    return 0;
}

// ********************************************************
// int16 Log_a_fault(char *msg, int16 One2reset)
// Add this message to message board A, in the next
//   open position.
//   (wbg - September 2017)
// ********************************************************
int16 Log_a_fault(char *msg, int16 One2reset)
{
    if(One2reset == 1) {
        Clear_Msg_Board();
//        LogIndex = 0;
        return 1;
    }
    if(LogIndex < 39) {
        Add_msgx_First_TimeOnly( msg, LogIndex);
        LogIndex++;
        return 1;
    } else return 0;
}


// ********************************************************
// int16 Log_a_fault(char *msg, int16 One2reset)
// Add this message to message board C, in the next
//   open position.
//   (wbg - December 2017)
// ********************************************************
int16 Log_a_fault_C(char *msg, int16 One2reset)
{
//    if(Functional_Test_is_Running == FALSE) return 0;
    if(One2reset == 1) {
        Clear_Msg_Board_C();
        return 1;
    }
    if(LogIndex_C < 39) {
       if(Add_msgx_First_TimeOnly_C( msg, LogIndex_C))
       {
           LogIndex_C++;
           return 1; 
       } 
       else return 0;                  
    } 
    else return 0;
}


// ********************************************************
// int16 Log_an_event(char *msg, int16 One2reset)
// Add this message to message board A, in the next
//   open position.
//   (wbg - October 2017)
// ********************************************************
int16 Log_an_event(char *msg)
{
    return Log_a_fault(msg, 0);
}

// ********************************************************
// int16 Log_an_event(char *msg, int16 One2reset)
// Add this message to message board C, in the next
//   open position.
//   (wbg - December 2017)
// ********************************************************
int16 Log_an_event_C(char *msg)
{
    return Log_a_fault_C(msg, 0);
}

// ********************************************************
// int16 Clear_Log_Buffer(void)
// Clears message board A,
//
//   (wbg - October 2017)
// ********************************************************
int16 Clear_Log_Buffer(void)
{
    if(Functional_Test_is_Running == FALSE)return Log_a_fault("", 1);
      else return 0; //  do not clear faults while doing the functional tests
}

// ********************************************************
// int16 Clear_Log_Buffer(void)
// Clears message board B,
//
//   (wbg - December 2017)
// ********************************************************
int16 Clear_Log_Buffer_C(void)
{
    if(Functional_Test_is_Running == FALSE)return Log_a_fault_C("", 1);
      else return 0; //  do not clear faults while doing the functional tests
}


// ********************************************************
// int16 Add_msg_to_array_B(char *msg, int16 index)
// Add this message to message board B, in the specific
//   position given by 'index'.
// Example usage:
//    sprintf(src,"11: %d %d", USR1port.CR1.B.SBR, USR2port.CR1.B.SBR);
//    Add_msg_to_array_B(src, 2);
//   (wbg - September 2017)
// ********************************************************
int16 Add_msg_to_array_B(char *msg, int16 index)
{
    int i = 0;
    int j = 0;

    if(index >= 0 && index < 39) {
        for (i = 2; i <= 19; i++) Msg_Board_B[index][i] = 0;
        // clear out the text past the ##
        for (i = 2; i <= 19; i++) {
            if (msg[j] == 0)
                break;
            else
                Msg_Board_B[index][i] = msg[j++];
        }
    }
    return 1;
}

// ********************************************************
// int16 Add_msg_to_array(char *msg)
// Adds a message to the next empty spot in the Msg_Board array
// Usage:
//      sprintf(tempArray,"Test message %d", i );
//      Add_msg_to_array(tempArray);
// (wbg - September 2017)
// ********************************************************
int16 Add_msg_to_array(char *msg) // returns 1 if success
{
    int i = 0;
    int j = 0;
    int index = 0;

    if (Msg_Board_index <= MAX_MSG_BOARD_INDEX) {
        for (i = 0; i <= 19; i++) {
            if (msg[j] == 0)
                break;
            else
                Msg_Board_A[Msg_Board_index][i] = msg[j++];
        }
        Msg_Board_index++;
        if (Msg_Board_index > MAX_MSG_BOARD_INDEX)
            Msg_Board_index = 0;
        return 1; // success
    }
    return 0; // failure
}

// ********************************************************
// int16 Add_msg_to_array_C(char *msg)
// Adds a message to the next empty spot in the Msg_Board array
// Usage:
//      sprintf(tempArray,"Test message %d", i );
//      Add_msg_to_array(tempArray);
// (wbg - September 2017)
// ********************************************************
int16 Add_msg_to_array_C(char *msg) // returns 1 if success
{
    int i = 0;
    int j = 0;
    int index = 0;

    if (Msg_Board_index_C <= MAX_MSG_BOARD_INDEX) {
        for (i = 0; i <= 19; i++) {
            if (msg[j] == 0)
                break;
            else
            {
                Msg_Board_C[Msg_Board_index_C][i] = msg[j++];
//                setoutp(o_4[5]);   
            }
                
        }
        Msg_Board_index_C++;
        if (Msg_Board_index_C > MAX_MSG_BOARD_INDEX)
            Msg_Board_index_C = 0;
        return 1; // success
    }
    return 0; // failure
}

// ********************************************************
// void Clear_Msg_Board(void)
// Clears out the the Msg_Board A array
// (wbg - September 2017)
// ********************************************************
void Clear_Msg_Board(void)
{
    int i = 0;
    int j = 0;
    int index = 0;

    for (index = 0; index <= MAX_MSG_BOARD_INDEX; index++) {
        msgSent[index] = 1;
        for (i=0; i<=19; i++) Msg_Board_A[index][i] = 0;
    }
    Msg_Board_index = 0;
    LogIndex = 0;
}

// ********************************************************
// void Clear_Msg_Board_B(void)
// Clears out the the Msg_Board B array and places the
// line number (00 - 39) in column 0 and 1.
// (wbg - September 2017)
// ********************************************************
void Clear_Msg_Board_B(void)  // for B
{
    int i = 0;
    int index = 0;

    for (index = 0; index <= MAX_MSG_BOARD_INDEX; index++) {

        for (i=2; i<=19; i++) Msg_Board_B[index][i] = 0;
        sprintf(&Msg_Board_B[index][0], "%2d", index );
    }
}

// ********************************************************
// void Clear_Msg_Board_C(void)
// Clears out the the Msg_Board B array and places the
// line number (00 - 39) in column 0 and 1.
// (wbg - December 2017)
// ********************************************************
void Clear_Msg_Board_C(void)  // for C
{
    int i = 0;
    int index = 0;

    for (index = 0; index <= MAX_MSG_BOARD_INDEX; index++) {
        msgSent_C[index] = 1;
        for (i=0; i<=19; i++) Msg_Board_C[index][i] = 0;
    }
    Msg_Board_index_C = 0;
    LogIndex_C = 0;
}

// ********************************************************
// Returns the index (not yet implemented)
// (wbg - September 2017)
// ********************************************************
int16 Get_Msg_Board_index(void)  //
{
    return 0;
}

// ********************************************************
// Returns the message at index (not yet implemented)
// (wbg - September 2017)
// ********************************************************
int16 Get_Msg_from_Board(int16 the_Msg_Board_index) //
{
    int index = 0;

    index = the_Msg_Board_index;

    return 0;
}

// ********************************************************
// Returns the last message (not yet implemented)
// (wbg - September 2017)
// ********************************************************
int16 Get_Last_Msg_from_Board(void)  //
{
    return 0;
}

// ********************************************************
// void Display_4_lines(int16 Start_msg_index)
// Put up four lines of the message onto the display
// (wbg - September 2017)
// ********************************************************
void Display_4_lines(int16 Start_msg_index)
{
    int m = 0;
    int p = 0;
    clr_lcd_dply();
    for(m = 0; m <= 3; m++) {
        for (p = 0; p <=19; p++) {
            if ( Msg_Board_A[Start_msg_index + m][p] == 0)
                break;
            else
                LCD_Display[m][p] =
                    Msg_Board_A[Start_msg_index + m][p];
        }
    }
}

// ********************************************************
// void Display_4_lines_B(int16 Start_msg_index)
// Put up four lines of the message onto the display
// (wbg - September 2017)
// ********************************************************
void Display_4_lines_B(int16 Start_msg_index) // for msg board B
{
    int m = 0;
    int p = 0;
    clr_lcd_dply();
    for(m = 0; m <= 3; m++) {
        for (p = 0; p <=19; p++) {
            if ( Msg_Board_B[Start_msg_index + m][p] == 0)
                break;
            else
                LCD_Display[m][p] =
                    Msg_Board_B[Start_msg_index + m][p];
        }
    }
}

// ********************************************************
// void Display_4_lines_C(int16 Start_msg_index)
// Put up four lines of the message onto the display
// (wbg - December 2017)
// ********************************************************
void Display_4_lines_C(int16 Start_msg_index) // for msg board C
{
    int m = 0;
    int p = 0;
    clr_lcd_dply();
    for(m = 0; m <= 3; m++) {
        for (p = 0; p <=19; p++) {
            if ( Msg_Board_C[Start_msg_index + m][p] == 0)
                break;
            else
                LCD_Display[m][p] =
                    Msg_Board_C[Start_msg_index + m][p];
        }
    }
}


// ********************************************************

// int16 Try_to_display(int16 Start_msg_index)
// Perform a test to see if the msg array A at this index
// contains data. If the first position contains 0 the rest
// of the line will also contain 0.  If it contains 0, return
// 0 else return 1.
// (wbg - September 2017)
// ********************************************************
int16 Try_to_display(int16 Start_msg_index)
{
    if( (Msg_Board_A[Start_msg_index][0]) == 0)
        return 0; // if first char of a line is 0 then the rest of line is zero.
    else return 1;
}



// ********************************************************

// int16 Try_to_display(int16 Start_msg_index)
// Perform a test to see if the msg array C at this index
// contains data. If the first position contains 0 the rest
// of the line will also contain 0.  If it contains 0, return
// 0 else return 1.
// (wbg - September 2017)
// ********************************************************
int16 Try_to_display_C(int16 Start_msg_index)
{
    if( (Msg_Board_C[Start_msg_index][0]) == 0)
        return 0; // if first char of a line is 0 then the rest of line is zero.
    else return 1;
}


// ********************************************************
// int16 MsgBrd_StartupPage(void)
//  Startup page. Display page 0 if it contains data
//  (wbg - September 2017)
// ********************************************************
int16 MsgBrd_StartupPage(void)
{
    Displayed_msg_page = 0;
    if ( Try_to_display(0) ) {
        Display_4_lines(0);
        return 1; // success
    } else return 0; // fail
}

// ********************************************************
// int16 MsgBrd_StartupPage_B(void)
//  Startup page. Display page 0 no matter what.
//  (wbg - September 2017)
// ********************************************************
int16 MsgBrd_StartupPage_B(void)
{
    Displayed_msg_page_B = 0;
    Display_4_lines_B(0);
    return 1; // success
}

// ********************************************************
// int16 MsgBrd_StartupPage_C(void)
//  Startup page. Display page 0 no matter what.
//  (wbg - December 2017)
// ********************************************************
int16 MsgBrd_StartupPage_C(void)
{
    Displayed_msg_page_C = 0;
    Display_4_lines_C(0);
    return 1; // success
}


// ********************************************************
// int16 Display_next_msg_block(void)
// displays the next four messages from msg Board A onto the LCD display
// Return 1 if display buffer was updated, or 0 if no more
// messages to display. In that case,'next' would go to
// the first.  (wbg - September 2017)
// ********************************************************
int16 Display_next_msg_block(void)
{
    if (Displayed_msg_page > 8) {
        MsgBrd_StartupPage();
        return 1; // success
    }

    if ( Try_to_display( (Displayed_msg_page + 1) * 4)  ) {
        Max_Displayed_msg_page = ++Displayed_msg_page;
        Display_4_lines((Displayed_msg_page) * 4);
        return 1; // success
    } else {
        // go to first page
        MsgBrd_StartupPage();
        return 1; // success
    }
    return 0; // fail
}

// ********************************************************
// int16 Display_next_msg_block_B(void)
// displays the next four messages from msg Board B onto the LCD display
// Return 1
//   (wbg - September 2017)
// ********************************************************
int16 Display_next_msg_block_B(void)
{
    if (Displayed_msg_page_B > 8) {
        MsgBrd_StartupPage_B();
        return 1; // success
    }
    Max_Displayed_msg_page_B = ++Displayed_msg_page_B;
    Display_4_lines_B((Displayed_msg_page_B) * 4);
    return 1; // success
}


// ********************************************************
// int16 Display_next_msg_block_C(void)
// displays the next four messages from msg Board C onto the LCD display
// Return 1 if display buffer was updated, or 0 if no more
// messages to display. In that case,'next' would go to
// the first.  (wbg - December 2017)
// ********************************************************
int16 Display_next_msg_block_C(void)
{
    if (Displayed_msg_page_C > 8) {
        MsgBrd_StartupPage_C();
        return 1; // success
    }

    if ( Try_to_display_C( (Displayed_msg_page_C + 1) * 4)  ) {
        Max_Displayed_msg_page_C = ++Displayed_msg_page_C;
        Display_4_lines_C((Displayed_msg_page_C) * 4);
        return 1; // success
    } else {
        // go to first page
        MsgBrd_StartupPage_C();
        return 1; // success
    }
    return 0; // fail
}

// ********************************************************
// int16 Display_previous_msg_block(void)
// displays the previous four messages (msg board A) onto the LCD display.
// If there are no more messages, it goes back to the first block of
// messages.
// Returns 1
//   (wbg - September 2017)
// ********************************************************
int16 Display_previous_msg_block(void)
{
    if (Displayed_msg_page == 0) Displayed_msg_page = Max_Displayed_msg_page;
    else --Displayed_msg_page;  // move to previous page
    Display_4_lines((Displayed_msg_page) * 4);
    return 1; // success
}

// ********************************************************
// int16 Display_previous_msg_block_B(void)
// displays the previous four messages (msg board B) onto the LCD display.
// If there are no more messages, it goes back to the first block of
// messages.
// Returns 1
//   (wbg - September 2017)
// ********************************************************
int16 Display_previous_msg_block_B(void)
{
    if (Displayed_msg_page_B == 0) Displayed_msg_page_B = 9;
    else --Displayed_msg_page_B;  // move to previous page
    Display_4_lines_B((Displayed_msg_page_B) * 4);
    return 1; // success
}

// ********************************************************
// int16 Display_previous_msg_block_C(void)
// displays the previous four messages (msg board C) onto the LCD display.
// If there are no more messages, it goes back to the first block of
// messages.
// Returns 1
//   (wbg - December 2017)
// ********************************************************
int16 Display_previous_msg_block_C(void)
{
    if (Displayed_msg_page_C == 0) Displayed_msg_page_C = Max_Displayed_msg_page_C;
    else --Displayed_msg_page_C;  // move to previous page
    Display_4_lines_C((Displayed_msg_page_C) * 4);
    return 1; // success
}


int16 Test_Msg_Board_A(int16 Task)
{
    int16 ReturnVal = CONTINUE_SERVICE;
    int16 i = 0;
    
    switch (Task)
    {
        case 0: // Clear msg board A
            Clear_Msg_Board();
            ReturnVal = FINISHED_WITH_SERVICE;
            break;
            
        case 1: //  Fill part way with data
            for(i = 0; i<15; i++)
            {
                sprintf(src,"P msg A - %d ", i);
                Log_an_event(src);    
            }
            ReturnVal = FINISHED_WITH_SERVICE;
            break;
                        
        case 2: // Fill all the way (40)
            for(i = 0; i<40; i++)
            {
                sprintf(src,"A msg A - %d ", i);
                Log_an_event(src);    
            }
            ReturnVal = FINISHED_WITH_SERVICE;        
            break;
            
        case 3: // Over fill
            for(i = 0; i<45; i++)
            {
                sprintf(src,"O msg A - %d ", i);
                Log_an_event(src);    
            }
            ReturnVal = FINISHED_WITH_SERVICE;        
            break;                           
            
        case 4: // display with up/down/mode buttons working
            ReturnVal = DisplayMsgBrd_A(0);
            break;            
                              
        default:
            break;                
    }    
      
    return ReturnVal;
}
///////////////////////////

int16 Test_Msg_Board_C(int16 Task)
{
    int16 ReturnVal = CONTINUE_SERVICE;
    int16 i = 0;
    
    switch (Task)
    {
        case 0: // Clear msg board C
            Clear_Msg_Board_C();
/*            
            clroutp (o_4[5]);
            clroutp (o_4[6]);
            clroutp (o_4[7]);
            clroutp (o_4[8]);
*/            
            ReturnVal = FINISHED_WITH_SERVICE;
            break;
            
        case 1: //  Fill part way with data (P)
            for(i = 0; i<15; i++)
            {
                sprintf(src,"P msg C - %d ", i);
                Log_an_event_C(src);    
            }
            ReturnVal = FINISHED_WITH_SERVICE;            
            break;
                        
        case 2: // Fill all the way (40)
            for(i = 0; i<40; i++)
            {
                sprintf(src,"A msg C - %d ", i);
                Log_an_event_C(src);    
            }
            ReturnVal = FINISHED_WITH_SERVICE;        
            break;
            
        case 3: // Over fill
            for(i = 0; i<45; i++)
            {
                sprintf(src,"O msg C - %d ", i);
                Log_an_event_C(src);    
            }
            ReturnVal = FINISHED_WITH_SERVICE;        
            break;            
               
            
        case 4: // display with up/down/mode buttons working
            ReturnVal = DisplayMsgBrd_C(0);
            break;    
            
        case 5: //  Fill part way with data (K)
            for(i = 0; i<7; i++)
            {
                sprintf(src,"K msg C - %d ", i);
                Log_an_event_C(src);    
            }
            ReturnVal = FINISHED_WITH_SERVICE;            
            break;                    
                              
        default:
            break;                
    }    
      
    return ReturnVal;
}
