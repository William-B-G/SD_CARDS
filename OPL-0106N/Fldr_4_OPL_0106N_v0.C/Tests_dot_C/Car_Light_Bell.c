// Car_Light_Bell.c

#include "global.h"
#include "Tests.h"

enum
{
    initialize,
    run_test,
    wait_4_pwr_to_come_up,
    test_pass,
    test_done,
    test_failure,
    Retest_option,
    Check_Buttons,
    Read_After_Delay,
};

int16 Check_24V(int16 dummy)
{

    int16 i,j,k = 0;
    static int16 test_Kase = initialize;

    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (test_Kase)
    {
        case initialize:

            ReturnVal = Standard_Init(CLEAR_MSG_BOARD_A, DELAY_3);
            ClearAllSEB();
            setoutp( (void *)SEB_outputs_array[O_C24_enum]); // Apply 24V

            test_Kase = run_test;
            break;
            
        case run_test:
            if(timers[t_Generic_Delay] < Delay_Count) break; 
            timers[t_Generic_Delay] = 0;           
            //  Check for presence of 24V
            if(rdinp( (void *)SEB_inputs_array[i_C24_T_enum]) == 1 )
            {
            	// 0 = fail; 1 = pass
            	test_Kase = test_pass;            	
            }
            else test_Kase = test_failure;
            break;
                        
        case test_pass:
            CurrentTestPassFail = 1; // Pass
            New_write_display("24vAC test PASS     ", 0, 0);
            New_write_display("Press MODE to       ", 1, 0);
            New_write_display("continue.           ", 2, 0);
            RetestResponse = 1; // DONE_BUT_CONTINUE_DISPLAY
            test_Kase = test_done;
            
            break;
            
        case test_done:
            clroutp( (void *)SEB_outputs_array[O_C24_enum]); // remove 24V
            test_Kase = initialize;
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;              
            break;
                                    
        case test_failure:
            test_Kase = Retest_option;
            break;
            
        case Retest_option: // Required for Retest Option
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) test_Kase = test_done;  // do not retest
            else if(RetestResponse == 1) test_Kase = initialize; // test again
                else ReturnVal = CONTINUE_SERVICE;    
            break;           
               
        default:
            break;                
    }
    return ReturnVal;
}

int16 Check_Car_Light(int16 dummy)
{

    int16 i,j,k = 0;
    static int16 test_Kase = initialize;
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            ReturnVal = Standard_Init(CLEAR_MSG_BOARD_A, DELAY_3);

            ClearAllSEB();
            FirePreReqSignal(LIG_ReqSignal_enum); // Car Light
            setoutp( (void *)SEB_outputs_array[O_C120T_enum]); // Apply 120 VAC
			New_write_display("Turn on Car Light SW", 0, 0);
			New_write_display("Press ENTER Btn     ", 1, 0);
			New_write_display("When ready.         ", 2, 0);
			New_write_display("MODE Btn to cancel  ", 3, 0);
           
            test_Kase = run_test ;
            break;
            
        case run_test:

            if (Check_for_Enter_Button())
            { 
				New_write_display("If Light is On      ", 0, 0);
				New_write_display("Press ENTER Btn     ", 1, 0);
				New_write_display("else press          ", 2, 0);
				New_write_display("MODE Btn            ", 3, 0);            
                test_Kase = Check_Buttons ;	
            }
	        if (Check_for_Mode_Button())
	        {
	            RetestResponse = 0;
	            test_Kase = test_done ;
	        }            
            break;
              
        case Check_Buttons:
            if (Check_for_Enter_Button())
            {
                test_Kase = test_pass;	
            }
	        if (Check_for_Mode_Button())
	        {	            
	            test_Kase = test_failure ;
	        }             
            break;
                           
        case test_pass:
            CurrentTestPassFail = 1; // Pass
            New_write_display("                    ",0,0); // row, col
            New_write_display("     Good           ",1,0); // row, col
            New_write_display("   Test PASSED      ",2,0); // row, col
            New_write_display("                    ",3,0); // row, col         
            test_Kase = test_done;            
            RetestResponse = 1;
            break;            
            
        case test_done:
            Un_FirePreReqSignal();
            clroutp( (void *)SEB_outputs_array[O_C120T_enum]); // remove 120 VAC
            test_Kase = initialize;
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;              
            break;
                                    
        case test_failure:
            test_Kase = Retest_option;
            break;
            
        case Retest_option: // Required for Retest Option
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) test_Kase = test_done;  // do not retest
            else if(RetestResponse == 1) test_Kase = initialize; // test again
                else ReturnVal = CONTINUE_SERVICE;    
            break;           
               
        default:
            break;                
    }
    return ReturnVal;
}


int16 Check_Car_Buzzer_or_Bell(int16 Buzzer_or_Bell)
{
// Buzzer : Buzzer_or_Bell == 0
// Bell : Buzzer_or_Bell == 1 
// Fire Light : Buzzer_or_Bell == 2
    int16 i,j,k = 0;
    static int16 test_Kase = initialize;
    static int16 extra_delay = 0;
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            ReturnVal = Standard_Init(CLEAR_MSG_BOARD_A, DELAY_3);

            ClearAllSEB();
            setoutp( (void *)SEB_outputs_array[O_C120T_enum]); // Apply 120 VAC
            if(Buzzer_or_Bell != 1) setoutp( (void *)SEB_outputs_array[O_C24_enum]); // Apply 24 VAC

            test_Kase = wait_4_pwr_to_come_up ;
            break;

                           // wait_4_pwr_to_come_up
        case wait_4_pwr_to_come_up:
            if(timers[t_Generic_Delay] < 4 * Delay_Count ) break;
                      
            if (Buzzer_or_Bell == 0)
            {  // Buzzer (1)
                extra_delay = 15;
	            setoutp( (void *)SEB_outputs_array[O_IFB_COPo1_enum]); // Turn on buzzer
				New_write_display("Did you hear buzzer?", 0, 0);            	
            }
            else if (Buzzer_or_Bell == 1)
            {  // Bell (16)
                extra_delay = 15;
	            setoutp( (void *)SEB_outputs_array[O_BELL_enum]); // Turn on bell
				New_write_display("Did you hear BELL?  ", 0, 0);            	            	
            }
            else if (Buzzer_or_Bell == 2)
            {  // Fire Light (0)
                extra_delay = 20; // about ?s total
	            setoutp( (void *)SEB_outputs_array[O_IFL_COPo0_enum]); // Turn on Fire Light
				New_write_display("Was fire light on?  ", 0, 0);            	            	
            }            

			New_write_display("Enter(YES) Mode(NO) ", 1, 0);
            Delay_Count = 0;
            test_Kase = run_test;
 
            break;
            
        case run_test:
            if(timers[t_Generic_Delay] > Delay_Count + extra_delay) // about 1/2 s
            {
//                clroutp((void *)SEB_outputs_array[O_C120T_enum]);     // 120 VAC off

                clroutp((void *)SEB_outputs_array[O_IFB_COPo1_enum]); // annoying buzzer off
                clroutp((void *)SEB_outputs_array[O_BELL_enum]);      // bell off   
                clroutp((void *)SEB_outputs_array[O_IFL_COPo0_enum]);      // fire light off 
                clroutp( (void *)SEB_outputs_array[O_C24_enum]); //  24 VDC OFF
               
                timers[t_Generic_Delay] = 0;             
            }             
            if (Check_for_Enter_Button())
            {            
                test_Kase = test_pass ;	
            }
	        if (Check_for_Mode_Button())
	        {
	            test_Kase = test_failure ;
	        }            
            break;              
                           // wait_4_pwr_to_come_up
        case test_pass:
            CurrentTestPassFail = 1; // Pass
            test_Kase = test_done;
            RetestResponse = 0;
            break;            
            
        case test_done:
//            clroutp((void *)SEB_outputs_array[O_C120T_enum]);     // 120 VAC off
            clroutp((void *)SEB_outputs_array[O_IFB_COPo1_enum]); // annoying buzzer off
            clroutp((void *)SEB_outputs_array[O_BELL_enum]);      // bell off
            clroutp( (void *)SEB_outputs_array[O_C24_enum]); //  24 VAC OFF        
            test_Kase = initialize;
            // Required for Retest Option
            if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; // Idle_time  
            else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;              
            break;
                                    
        case test_failure:
//            clroutp((void *)SEB_outputs_array[O_C120T_enum]);     // 120 VAC off
            clroutp((void *)SEB_outputs_array[O_IFB_COPo1_enum]); // annoying buzzer off
            clroutp((void *)SEB_outputs_array[O_BELL_enum]);      // bell off 
            clroutp( (void *)SEB_outputs_array[O_C24_enum]); //  24 VAC OFF       
            test_Kase = Retest_option;
            break;
            
        case Retest_option: // Required for Retest Option
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) test_Kase = test_done;  // do not retest
            else if(RetestResponse == 1) test_Kase = initialize; // test again
                else ReturnVal = CONTINUE_SERVICE;    
            break;           
               
        default:
            break;                
    }
    return ReturnVal;
}