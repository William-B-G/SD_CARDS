//  MsgBoard.c

#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1137_1139.h"

extern int16 Log_an_event_C(char *msg);


enum {
    Init_Msg_Board,
    Disply_Msg_Board,
    Monitor_Buttons
};


int16 DisplayGrpUnpkData(int16 dummy)
{    
    Clear_All_Boards(1);
    Get_Srl_Hall_Station_Pkg(0);
    sprintf(src,"14/16; %d,%d",Case_14_data, Case_16_data);
	Log_an_event(src);
	Add_msg_to_array_B(src,16);	
    return Display_Message_Board(0);
}

int16 DisplayMsgBrd_A(int16 dummy)
{
    return Display_Message_Board(0);
}

int16 DisplayMsgBrd_B(int16 dummy)
{
    return Display_Message_Board_B(0);
}

int16 DisplayMsgBrd_C(int16 dummy)
{
    return Display_Message_Board_C(0);
}

int16 Tst_DisplayMsgBrd_C(int16 dummy)
{
	Log_an_event_C("into chk Initial x ");
    return Display_Message_Board_C(0);
}

/* - WBG 2018-1-24
int16 Display_Message_Board(int16 dummy)
{
    static int16 MsgBrdCase = 0;
    int16 FakeParam = 0;

    FakeParam = dummy;

    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(MsgBrdCase) {
    case Init_Msg_Board:
        clr_lcd_dply();
        Displayed_msg_page = 0;
        Max_Displayed_msg_page = 0;
        MsgBrdCase = Disply_Msg_Board;
        break;

    case Disply_Msg_Board:
        MsgBrd_StartupPage();
        MsgBrdCase = Monitor_Buttons;
        break;

    case Monitor_Buttons:
        if(Check_for_Up_Button()) {
            // display the previous message board
            Display_previous_msg_block();
        }

        if (Check_for_Down_Button()) {
            // display the next message board
            Display_next_msg_block();
        }

        if(Check_for_Mode_Button()) {
            MsgBrdCase = Init_Msg_Board;
            return FINISHED_WITH_SERVICE;
        }
        break;

    default:
        MsgBrdCase = Init_Msg_Board;
        New_write_display("MESSAGE BOARD       ",1,0);
        New_write_display("Executing...DEFAULT ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;	// 0
}
*/


int16 Display_Message_Board(int16 Param)
{
    static int16 MsgBrdCase = 0;
    int16 RetestParam = 0;

    RetestParam = Param & 0x8000;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(MsgBrdCase) {
    case Init_Msg_Board:
        clr_lcd_dply();
        Displayed_msg_page = 0;
        Max_Displayed_msg_page = 0;
        MsgBrdCase = Disply_Msg_Board;
        break;

    case Disply_Msg_Board:
        MsgBrd_StartupPage();
        MsgBrdCase = Monitor_Buttons;
        break;

    case Monitor_Buttons:
        if(Check_for_Up_Button()) {
            // display the previous message board
            Display_previous_msg_block();
        }

        if (Check_for_Down_Button()) {
            // display the next message board
            Display_next_msg_block();
        }

        if(Check_for_Mode_Button()) {
            MsgBrdCase = Init_Msg_Board;
            return FINISHED_WITH_SERVICE;
        }
        if(RetestParam)
        {
            if(Check_for_Enter_Button()) {
                MsgBrdCase = Init_Msg_Board;
                return CLOSE_MSG_BOARD; 
            }
        }
        break;

    default:
        MsgBrdCase = Init_Msg_Board;
        New_write_display("MESSAGE BOARD       ",1,0);
        New_write_display("Executing...DEFAULT ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;	// 0
}

int16 Display_Message_Board_C(int16 dummy)
{
    static int16 MsgBrdCase = 0;
    int16 FakeParam = 0;

    FakeParam = dummy;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(MsgBrdCase) {
    case Init_Msg_Board:
        clr_lcd_dply();
        Displayed_msg_page_C = 0; // Displayed_msg_page_C
        Max_Displayed_msg_page_C = 0;
        MsgBrdCase = Disply_Msg_Board;
        break;

    case Disply_Msg_Board:
        MsgBrd_StartupPage_C();
        MsgBrdCase = Monitor_Buttons;
        break;

    case Monitor_Buttons:
        if(Check_for_Up_Button()) {
            // display the previous message board
            Display_previous_msg_block_C();
        }

        if (Check_for_Down_Button()) {
            // display the next message board
            Display_next_msg_block_C();
        }

        if(Check_for_Mode_Button()) {
            MsgBrdCase = Init_Msg_Board;
            return FINISHED_WITH_SERVICE;
        }
        break;

    default:
        MsgBrdCase = Init_Msg_Board;
        New_write_display("MESSAGE BOARD       ",1,0);
        New_write_display("Executing...DEFAULT ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;	// 0
}