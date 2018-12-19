// Operator_Messages.c
#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"

#define MATCHES 1

char *trimwhitespace(char *str);
int16 Flush_OpM(void);
int16 Init_OpM(void);
int16 Test_OpM_bufferAdd(int16 number_of_msgs_to_add);

int16 Tryout_OpM(int16 Fail_type, int16 Index);
int16 normal_and_fpga_nts(int16 Fail_type, int16 Index);
int16 normal_and_component(int16 Fail_type, int16 Index);
int16 Check_failure_type(int16 Fail_type, int16 Index);
int16 TrimTrailingWhiteSpaceAllowed = TRUE;

const char *Component_table[] = 
  {    
       "ICx01","ICx02","ICx03","ICx04","ICx05","ICx06","ICx07","ICx08",
       "AUTO ", "LBP  ", "GLB  ", "GBP  ",       "BAU  ", "BAD  ", "TAU  ", "TAD  ",
       "DLB  ", "DLM  ", "DLT  ", "RLM  ",       "LPS  ", "RGS  ", "GS   ", "ACC  ",   

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",  

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",   

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",                    
  };

struct OpM_tag{		
	union {
		uint16 R;
		struct {
			uint16 SWITCH_YARD     :1 ; // 0= normal;  1= extend
			uint16 FPGA_COMPONENT  :1 ; // 0= FPGA  ;  1= COMPONENT 
			uint16 FAIL_ON         :1 ;
			uint16 FAIL_OFF        :1 ;			
			uint16 FAIL_SHORT      :1 ;
			uint16 FPGA_NTS        :2 ; // 0=FPGA1; 1=FPGA2; 3=NTS
			uint16 RESERVED        :2 ;
			uint16 PIN_NUMBER      :7 ;
		} Normal_and_FPGA_NTS;
		struct {
			uint16 SWITCH_YARD     :1 ; // 0= normal;  1= extend
			uint16 FPGA_COMPONENT  :1 ; // 0= FPGA  ;  1= COMPONENT
			uint16 FAIL_ON         :1 ;
			uint16 FAIL_OFF        :1 ;
			uint16 FAIL_SHORT      :1 ;
			uint16 COMPONENT_INDEX :11;
		} Normal_and_Component;	
		struct {
			uint16                 :2 ; 
			uint16 FAIL_TYPE       :3 ;
			uint16 COMPONENT_INDEX :11;
		} Fail_Type;			
		struct {
			uint16 SWITCH_YARD     :1 ; // 0= normal;  1= extend
			uint16 EXTEND_INDEX    :15; // add 200 to get index to next OPM[]
		} Extend;		
	} Status;
} const Operator_Messages[] = 
{
    0x0A04, // normal, fail type 1, FPGA 2, Pin 4
    0x101B, // normal,fail type 2 , FPGA 2, Pin 4
    0x680B, // normal,fail type 9 , Component index 11 
    0x8007, // extended, offset 7 (gets added to base of 200)
    0x100B,
    0x1234,
    0xABCD,
    0x1A45,
    0x100B,    
};

enum
{
	initialize,
	run_test,
	test_pass,
	test_done,
	test_failure,
	Retest_option,
};

int16 Tryout_OpM(int16 Fail_type, int16 Index)
{
    int16 switch_yard = 0;
    int16 New_Index = 0;
    int16 fpga_component = 0;
    int16 Afailure_type = 0;
    int16 AComponent_table_Idx = 0;

    Log_an_event("Start of OpM msgs");    
    sprintf(src,"Fail_t=%d, Idx=%d", Fail_type, Index );
    Log_an_event(src);
    switch_yard = Operator_Messages[Index].Status
       .Normal_and_FPGA_NTS.SWITCH_YARD;
    sprintf(src, "swtchYd %d",switch_yard);
    New_write_display(src,0,0);
    Log_an_event(src);
       
    fpga_component = Operator_Messages[Index].Status
       .Normal_and_FPGA_NTS.FPGA_COMPONENT;  
    sprintf(src, "fpgaComp %d",fpga_component);
    New_write_display(src,1,0);
    Log_an_event(src);        
       
    New_Index = Operator_Messages[Index].Status
       .Extend.EXTEND_INDEX + 200;
    sprintf(src, "ExIndx %d",New_Index);
    New_write_display(src,2,0);
    Log_an_event(src);         
       
    Afailure_type = Operator_Messages[Index].Status
       .Fail_Type.FAIL_TYPE;
    sprintf(src, "failType %d",Afailure_type);
    New_write_display(src,3,0);
    Log_an_event(src);         
       
    AComponent_table_Idx = 
        Operator_Messages[Index]
            .Status.Normal_and_Component.COMPONENT_INDEX ;
    sprintf(src, "CompIdx %d",AComponent_table_Idx);
    Log_an_event(src); 
            

            
    switch (switch_yard)
    {
        case 0: // normal  "C_"                      
            fpga_component = Operator_Messages[Index].Status
               .Normal_and_FPGA_NTS.FPGA_COMPONENT;
            sprintf(src, "C_%d iDX",fpga_component,Index );
            Log_an_event(src);                
            switch (fpga_component)
            {
                case 0: //FPGA
                    normal_and_fpga_nts(Fail_type, Index);
                    break;
                    
                case 1: // Component
                    normal_and_component(Fail_type, Index);
                    break;                                 
                       
                default:
                    break;                
            }        
            break;
            
        case 1: // extend "D_"
            New_Index = Operator_Messages[Index].Status
               .Extend.EXTEND_INDEX + 200;
            sprintf(src, "D_%d NewDX",New_Index );
            Log_an_event(src);               
//            Tryout_OpM(Fail_type, New_Index);
            break;                                    
               
        default:
            break;                
    }       
    Log_an_event("END of OpM msgs");     
    return 0;
}
   
int16 normal_and_fpga_nts(int16 Fail_type, int16 Index)
{ // A_
    int16 failure_type = 0;
    
    failure_type = Check_failure_type(Fail_type, Index);
    sprintf(src,"A_ft=%d FT=%d Idx=%d",failure_type, Fail_type, Index );
    Log_an_event(src);    
    if(failure_type == MATCHES)
    { 
        // format msg and send to LCD display.
        return 0;
    }       
}
   
int16 normal_and_component(int16 Fail_type, int16 Index)
{ // B_
    int16 failure_type = 0;
    int16 Component_table_Idx = 0;
    
    failure_type = Check_failure_type(Fail_type, Index);
    sprintf(src,"B_ft=%d FT=%d Idx=%d",failure_type, Fail_type, Index );
    Log_an_event(src);    
    if(failure_type == MATCHES)
    {
        Component_table_Idx = 
            Operator_Messages[Index]
                .Status.Normal_and_Component.COMPONENT_INDEX ; 
        // format msg and send to LCD display.
        sprintf(src,"B_%d, %s",Component_table_Idx, Component_table[3 ] ); 
        Log_an_event(src);        
        return 0;
    }    
}

int16 Check_failure_type(int16 Fail_type, int16 Index)
{
    int16 failure_type;
    failure_type = Operator_Messages[Index].Status
       .Fail_Type.FAIL_TYPE;
    if(failure_type == Fail_type) return 1;
    else return -1; // not the type we are looking for.      
}

int16 Demo_OpM(int16 Fail_type_and_index)
{
    int16 fail_type = (Fail_type_and_index & 0xf000) >> 12;
    int16 index = Fail_type_and_index & 0x00ff;
    
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option    
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            clr_lcd_dply();
            Clear_Msg_Board();
            ReturnVal = CONTINUE_SERVICE; 
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            CurrentTestPassFail =0;  // fail
            timers[t_Generic_Delay] = 0;
            test_Kase = run_test;
            Tryout_OpM(fail_type, index);
            break;
            
        case run_test:
            if(timers[t_Generic_Delay] < Delay_Count) break;            
            //  ADD CODE
            test_Kase = test_failure;
            break;
                        
        case test_pass:
            test_Kase = test_done;
            break;
            
        case test_done:
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

int16 Add_OpM(int16 MessageIndex)
{ // Add a message to the build-up line, if there is room.  If there is
// not enough room, Send the build-up line to the msg board A; erase the
// build-up line and then add the message to the build-up line. 
    int16 msgIndexLen = 0;
    int16 build_up_line_Len = 0;    
    char src_B[50];
    
    src_B[0] = 0;
    strcpy(src_B,Component_table[MessageIndex]);
 //   if(TrimTrailingWhiteSpaceAllowed) src = trimwhitespace(src_B);
    msgIndexLen = strlen(trimwhitespace(src_B));
    if( (msgIndexLen == 0) || (msgIndexLen > 20) ) return -1;
    build_up_line_Len = strlen(src);
    if(msgIndexLen+build_up_line_Len > 18) // to allow for a comma
       // and space to be inserted.
    { // flush the current line and then recursivly call this function.
        if(build_up_line_Len > 2) strcat(src, ",");
        Flush_OpM();  // write out the current line and then erase 'src''
        Add_OpM(MessageIndex);
    }
    else
    { //  concatenate src and msg.
        if(build_up_line_Len > 2) strcat(src, ", ");
        strcat(src, trimwhitespace(src_B));  
    }
}

int16 Flush_OpM(void)
{ // Send the build-up line to the msg board A and then erase the
// build-up line. 
    Log_an_event(src); 
    Init_OpM();  
}

int16 Init_OpM(void)
{ //  Erase the build-up line and then add the message to the build-up line. 
    src[0] = 0;    
}

int16 Test_OpM_bufferAdd(int16 number_of_msgs_to_add)
{
    int16 i,j,k = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    int16 Number_of_Msgs_to_Add = 0;
     
    
//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    TrimTrailingWhiteSpaceAllowed = number_of_msgs_to_add & 0xF000;
    Number_of_Msgs_to_Add = number_of_msgs_to_add & 0x0FF;
    switch (test_Kase)
    {
        case initialize:
            //  ADD CODE
            clr_lcd_dply();
            Clear_Msg_Board();
            ReturnVal = CONTINUE_SERVICE;
            CurrentTestPassFail =0;  // fail
            Retest_Option_Parm = 0; 
            RetestResponse = 1;
            Delay_Count = 3; 
            timers[t_Generic_Delay] = 0;
            Init_OpM();
            test_Kase = run_test;
            break;
            
        case run_test:
            if(timers[t_Generic_Delay] < Delay_Count) break;            
            //  ADD CODE
            for(i = 0; i < Number_of_Msgs_to_Add; i++)
            {
                Add_OpM(i);   
            }
            Flush_OpM();
            test_Kase = test_failure; 
            break;
                        
        case test_pass:
            test_Kase = test_done;
            break;
            
        case test_done:
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
     
    
// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
// From: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
// wbg : 2018-1-25
char *trimwhitespace(char *str)
{
  char *end;
  
  if( TrimTrailingWhiteSpaceAllowed == 0) return str; // do not trim
  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}    