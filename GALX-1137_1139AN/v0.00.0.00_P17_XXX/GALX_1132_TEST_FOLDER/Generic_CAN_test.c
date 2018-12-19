// Generic_CAN_test.c

#include "global.h"
#include "GALX_1132_TEST.h"

int16 TEST_a_CAN(int16 Operation);

enum {
    Init_GRCAN_Test,
    Wait_for_Operator_Response,
    Send_GRCAN_Signal,
    Check_GRCAN_Signal,
    GRCAN_test_Fail,
    GRCAN_test_Pass
};

#define CAN_test_signal_index 12
#define CAN_index 4 // GR, MR, CT, SEL
/*
void *CANtest_osignal_array[CAN_test_signal_index][CAN_index]= 
{
    { o_GRo0, o_MRo0, o_CTo0, o_GRo0, },
}
*/
int16 TEST_a_CAN(int16 Operation)
{
    ;
}