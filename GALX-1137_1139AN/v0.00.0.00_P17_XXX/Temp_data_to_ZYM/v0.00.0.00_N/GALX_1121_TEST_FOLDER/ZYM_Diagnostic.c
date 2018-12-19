// ZYM_Diagnostic.c

#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"

#define  OFF 0
#define  ON  1

int16 zym_diagnostic_mode = 0;  // set to 1 while doing the diagnostics
    // so that Display_Message_Board_C() will monitor the 'Enter' button.
int16 zym_mode_Enter_Button = 0;  // gets set to 1 in Display_Message_Board_C()
    // if the Enter button is pressed.
int16 zym_mode_On_Off = OFF; // used to store the current state of the diagnostic
    // item_to_control.       

enum
{
    Init_Kase,
    Read_disabled_unpack,
    Read_Enabled_unpack,
    Monitor_Buttons,
    Update,
};

int16 Disabled_unpack_1[4]; // FPGA 1 Disabled item unpack data
int16 Disabled_unpack_2[4]; // FPGA 2 Disabled item unpack data

int16 ZYM_Diagnostic(int16 item_to_control)
{    /*store the unpack data prior to enabling the item.  Then enable
     and compare the new unpack data with old and use diff to create
     an array of diffs.  Use the array of diffs to sprintf() to 
     msg board C.  Use the Enter key to toggle the item on/off. and see the
     result on display.*/

    static int16 Kase = Init_Kase;
    static int16 ReturnVal = CONTINUE_SERVICE;
    static int16 Delay_Count = 3;
    int16 i = 0;

        switch (Kase)
        {   // short description
            case Init_Kase:
                ReturnVal = CONTINUE_SERVICE; // FINISHED_WITH_SERVICE; DONE_BUT_CONTINUE_DISPLAY; 
                    // CONTINUE_SERVICE;
                Clear_Msg_Board();
                Clear_Msg_Board_B();
                Clear_Msg_Board_C();
                zym_mode_On_Off = OFF;
                zym_mode_Enter_Button = 0;
                zym_diagnostic_mode = 1;  // set to 1 while doing the diagnostics
                    // so that Display_Message_Board_C() 
                    // will monitor the 'Enter' button.
                Preliminary_120VAC(6); // Sets 120VAC to CST
                Delay_Count = 3; 
                timers[t_Generic_Delay] = 0; 
                Kase = Read_disabled_unpack;                
                break;
                
            case Read_disabled_unpack: // First, look at the FPGA's state before turnning
                // the item_to_control ON.
                if(timers[t_Generic_Delay] <= Delay_Count ) break;
                else{
                    // read unpack data and save in array.
                    // then enable the item
                    _disable();
                    for (i=0;i<4;i++)
                    {
                        Disabled_unpack_1[i] = i1spi_in.buff[i];
                        Disabled_unpack_2[i] = iospi_in.buff[i];                                          
                    }   
                    _enable();                     
                    Set_FPGA_bits_B(item_to_control + 0x0f00); // turns bits ON
                    zym_mode_On_Off = ON;
                    timers[t_Generic_Delay] = 0;
                    Kase = Read_Enabled_unpack;
                }
                break;
                            
            case Read_Enabled_unpack:
                    if(timers[t_Generic_Delay] <= Delay_Count ) break;
                    else{
                        // read unpack data and compare to Disabled_unpack[].
                        // Build array of diffs.   
                        _disable();
                        for (i=0;i<4;i++)
                        {
                            CPLD_1_Buff[i] = i1spi_in.buff[i];
                            CPLD_2_Buff[i] = iospi_in.buff[i];                                          
                        }   
                        _enable();   
                        Build_up_Diff_Array(item_to_control);
                        Compare_and_build(item_to_control);
                        Add_Msg_to_C_for_zym_diag(0);  // fills msg board C                
                        timers[t_Generic_Delay] = 0;
                        Kase = Monitor_Buttons;
                    }
                break;

            case Update: // wait, then read unpack data
                // and display new data to msgBoard C. Then go back to
                // monitoring buttons.
                if(timers[t_Generic_Delay] <= Delay_Count ) break;
                else{
                    _disable();
                    for (i=0;i<4;i++)
                    {
                        CPLD_1_Buff[i] = i1spi_in.buff[i];
                        CPLD_2_Buff[i] = iospi_in.buff[i];                                          
                    }   
                    _enable(); 
                    Clear_Msg_Board_C();  
                    Add_Msg_to_C_for_zym_diag(0);  // fills msg board C                      
                    timers[t_Generic_Delay] = 0;
                }
                Kase = Monitor_Buttons;
                break;

            case Monitor_Buttons:
                ReturnVal = Display_Message_Board_C(0); // This routine will monitor
                    // the Enter button and, if pressed, will set "zym_mode_Enter_Button", 
                if (ReturnVal == FINISHED_WITH_SERVICE)
                {
                    Kase = Init_Kase;
                    for(i=0; i<12; i++)
                    {
                      clroutp(Virtual_Board_B_OUT[i]); // Turn off outputs on 1106AN, bd 4 
                    } 
                    Prelim_Done = FALSE;                     
                    break;
                }

                if(zym_mode_Enter_Button == 1) 
                 {
                    zym_mode_Enter_Button = 0;
                    // toggle on/off and then go to Update;
                    if(zym_mode_On_Off == ON){
                        Set_FPGA_bits_B(item_to_control); // turn OFF
                        zym_mode_On_Off = OFF;
                    }
                    else{
                        Set_FPGA_bits_B(item_to_control + 0x0f00); // turns bits ON
                        zym_mode_On_Off = ON;
                    }
                    timers[t_Generic_Delay] = 0;
                    Kase = Update;
                 }
     
                break;            
                   
            default:
                break;                
        }  // END OF switch(Kase)
            
    return ReturnVal;             
} // END OF ZYM_Diagnostic()


int16 Compare_and_build(int16 which_item)
{   // check for diff in the unpack data between On/Off states, and build up
    // an array of indexes to the char buffer.
    int16 i = 0;
    static int16 a = 0;
    static int16 ReturnVal = CONTINUE_SERVICE;
        // FINISHED_WITH_SERVICE; DONE_BUT_CONTINUE_DISPLAY; 
        // CONTINUE_SERVICE;

    for ( i = 0; i < 4; ++i)
    { // First do FPGA1 unpack data.
        Display_Problem(CPLD_1_Buff[i] , Disabled_unpack_1[i], i, 1, i*16 );
    }

    for ( i = 0; i < 4; ++i)
    { // Now do FPGA2 unpack data.
        Display_Problem(CPLD_2_Buff[i] , Disabled_unpack_2[i], i, 2, i*16 );
    }

    return ReturnVal; 
        // FINISHED_WITH_SERVICE; DONE_BUT_CONTINUE_DISPLAY; 
        // CONTINUE_SERVICE;            
} // END OF Compare_and_build()


int16 Get_Bit_Status(int16 FPGA, int16 offset)
{   // if the bit a position 'offset' = 1, return 1, else return 0.
    static int16 a, i, j, k = 0;
    static int16 ReturnVal = CONTINUE_SERVICE;
        // FINISHED_WITH_SERVICE; DONE_BUT_CONTINUE_DISPLAY; 
        // CONTINUE_SERVICE;

    /* Diff_Array_1*/
    a = offset%16; // a is the remainder
    k = offset / 16; // is is the word number
    i = 0x8000 >> a;  // will be used as a mask
    Debug_A_wbg = a;
    Debug_B_wbg = k;
    Debug_C_wbg = i;

        switch (FPGA)
        {   //  
            case 1:
                if (CPLD_1_Buff[k] & i ) return 1;
                        else return 0;
            
                break;
                
            case 2:
                 if (CPLD_2_Buff[k] & i ) return 1;
                 else return 0;
                break;                                    
                   
            default:
                break;                
        }  // END OF switch(FPGA)
    
    return ReturnVal; 
        // FINISHED_WITH_SERVICE; DONE_BUT_CONTINUE_DISPLAY; 
        // CONTINUE_SERVICE;            
} // END OF Get_Bit_Status()

int16 strlwr(char *msg)
{
    int16 len = strlen(msg);
    int16 i = 0;
/*    
    for (i=0; i < len; i++)
    {
        if( msg[i] >= 'A' && msg[i] <= 'Z')
        {
            msg[i] += 32;
        }
    }
*/    
     return 0;
}




