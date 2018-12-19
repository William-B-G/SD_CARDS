//  MsgBoard_B.c

#include "global.h"
#include "GALX_1132_TEST.h"

enum {
    Init_Msg_Board_B,
    Disply_Msg_Board_B,
    Monitor_Buttons_B
};

int16 MsgBrd_B_StartupPage(void);

int16 Display_Message_Board_B(int16 dummy)
{
    static int16 MsgBrdCase = 0;
    int16 FakeParam = 0;

    FakeParam = dummy;

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;  // toggle LED

    switch(MsgBrdCase) {
    case Init_Msg_Board_B:
        clr_lcd_dply();
        Displayed_msg_page = 0;
        Max_Displayed_msg_page = 0;
        MsgBrdCase = Disply_Msg_Board_B;
        break;

    case Disply_Msg_Board_B:
        MsgBrd_StartupPage_B();
        MsgBrdCase = Monitor_Buttons_B;
        break;

    case Monitor_Buttons_B:

        if(Check_for_Up_Button()) {
            // display the previous message board
            Display_previous_msg_block_B();
        }

        if (Check_for_Down_Button()) {
            // display the next message board
            Display_next_msg_block_B();
        }

        if(Check_for_Mode_Button()) {
            MsgBrdCase = Init_Msg_Board_B;
            return FINISHED_WITH_SERVICE;
        }

        break;

    default:
        MsgBrdCase = Init_Msg_Board_B;
        New_write_display("MESSAGE BOARD       ",1,0);
        New_write_display("Executing...DEFAULT ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;	// 0
}

int16 MsgBrd_B_StartupPage(void)
{
    return 0;  // fail
}