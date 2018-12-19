//  Tester_Diagnostics.c


#include "global.h"
#include "GALX_1132_TEST.h"

enum {
    Init_CAN_case,
    View_Log,
    Dismiss
};

int16 Group_CAN_Diagnostics(void)
{
    int16 temp_index = 0;
    int16 ReturnVal = 0;
    static int16 CAN_case = Init_CAN_case;

    switch (CAN_case) {
    case Init_CAN_case:
        Clear_Log_Buffer();
        sprintf(src,"Devs: %d",
                nmb_grcan_dev);
        Log_a_fault(src, 0);
        for(temp_index = 0; temp_index <39; temp_index++) {
            if(temp_index >= nmb_grcan_dev) break;
            sprintf(src,"dev %d= %d : %d",temp_index,
                    grcan_dev_list[temp_index],
                    grcan_dev[temp_index].online );
            Log_an_event(src);
        }
        CAN_case = View_Log;
        break;

    case View_Log:
        ReturnVal = DisplayMsgBrd_A(0);
        if(ReturnVal == CONTINUE_SERVICE) return ReturnVal;
        CAN_case = Init_CAN_case;
        return ReturnVal;
        break;

    default:
        CAN_case = Init_CAN_case;
        clr_lcd_dply();
        New_write_display("Grp CAN Diag Error  ",1,0);
        New_write_display("default case taken  ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        //return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;
}

int16 CT_CAN_Diagnostics(void)
{
    int16 temp_index = 0;
    int16 ReturnVal = 0;
    static int16 CAN_case = Init_CAN_case;

    switch (CAN_case) {
    case Init_CAN_case:
        Clear_Log_Buffer();
        sprintf(src,"Devs: %d",
                nmb_ctcan_dev);
        Log_a_fault(src, 0);
        for(temp_index = 0; temp_index <39; temp_index++) {
            if(temp_index >= nmb_ctcan_dev) break;
            sprintf(src,"dev %d= %d : %d",temp_index,
                    ctcan_dev_list[temp_index],
                    ctcan_dev[temp_index].online );
            Log_an_event(src);
        }
        CAN_case = View_Log;
        break;

    case View_Log:
        ReturnVal = DisplayMsgBrd_A(0);
        if(ReturnVal == CONTINUE_SERVICE) return ReturnVal;
        CAN_case = Init_CAN_case;
        return ReturnVal;
        break;

    default:
        CAN_case = Init_CAN_case;
        clr_lcd_dply();
        New_write_display("CAR T CAN Diag Error",1,0);
        New_write_display("default case taken  ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        break;
    }
    return CONTINUE_SERVICE;
}

int16 MR_CAN_Diagnostics(void)
{
    int16 temp_index = 0;
    int16 ReturnVal = 0;
    static int16 CAN_case = Init_CAN_case;

    switch (CAN_case) {
    case Init_CAN_case:
        Clear_Log_Buffer();
        sprintf(src,"Devs: %d",
                nmb_mrcan_dev);
        Log_a_fault(src, 0);
        for(temp_index = 0; temp_index <39; temp_index++) {
            if(temp_index >= nmb_mrcan_dev) break;
            sprintf(src,"dev %d= %d : %d",temp_index,
                    mrcan_dev_list[temp_index],
                    mrcan_dev[temp_index].online );
            Log_an_event(src);
        }
        CAN_case = View_Log;
        break;

    case View_Log:
        ReturnVal = DisplayMsgBrd_A(0);
        if(ReturnVal == CONTINUE_SERVICE) return ReturnVal;
        CAN_case = Init_CAN_case;
        return ReturnVal;
        break;

    default:
        CAN_case = Init_CAN_case;
        clr_lcd_dply();
        New_write_display("CAR T CAN Diag Error",1,0);
        New_write_display("default case taken  ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        return DONE_BUT_CONTINUE_DISPLAY; // Idle_time
        break;
    }
    return CONTINUE_SERVICE;
}

int16 Selector_CAN_Diagnostics(void)
{
    return 0;
}




