//  OutPut_1106AN.c

/* //-
#include "global.h"
#include "GALX_1132_TEST.h"

// also see: G4_io_init.c; G4_iomap.h; MRCAN_TEST.C
// Usfull functions for 1106AN I/O board: setoutp(index_1_12); clroutp(index_1_12);
//     rdinp(index_1_12).
enum
{
    Init_Output_1106,
    CheckButtons,
    Output_1106_Done,
    UpDateTheDisplay,
};

void *o_3[12]= 
{
  o_OUT1,
  o_OUT2 ,
  o_OUT3 ,
  o_OUT4 ,
  o_OUT5 ,
  o_OUT6 ,
  o_OUT7 ,
  o_OUT8 ,
  o_OUT9 ,
  o_OUT10 ,
  o_OUT11 ,
  o_OUT12 ,   
};

void *i_3[12]= 
{
  i_IN1,
  i_IN2 ,
  i_IN3 ,
  i_IN4 ,
  i_IN5 ,
  i_IN6 ,
  i_IN7 ,
  i_IN8 ,
  i_IN9 ,
  i_IN10 ,
  i_IN11 ,
  i_IN12 ,   
};

void *o_4[12]= 
{
  o_out1,
  o_out2 ,
  o_out3 ,
  o_out4 ,
  o_out5 ,
  o_out6 ,
  o_out7 ,
  o_out8 ,
  o_out9 ,
  o_out10 ,
  o_out11 ,
  o_out12 ,   
};

void *i_4[12]= 
{
  i_in1,
  i_in2 ,
  i_in3 ,
  i_in4 ,
  i_in5 ,
  i_in6 ,
  i_in7 ,
  i_in8 ,
  i_in9 ,
  i_in10 ,
  i_in11 ,
  i_in12 ,   
};

struct IO_1106AN
{
  void *o_3;
  void *o_4;
  void *i_3;
  void *i_4;    
}io_3_4;

int16 All_On_Off(int16 Addr)
{
  void **WhichBd = NULL;
  int16 i = 0;
  int16 on_off = 0;
  int16 TheAddr = 0;
  int16 temp = 0;
  static int16 ReturnVal = CONTINUE_SERVICE;
  static debugCase = 0;
  // setoutp
  // return FINISHED_WITH_SERVICE
  SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
  switch(debugCase)
  {
    case 0: // init
      clr_lcd_dply(); // clear 4-line display
      Set_iospi_Ctrl_flag(1);
      TheAddr = Addr & 0x0ff; // use lowest byte.
      on_off = Addr >> 12;
     // on_off = Addr & 0x0f000; //  use upper byte,
      sprintf(src,"Addr=0x%x",Addr );
      New_write_display(src,1,0);
      
      sprintf(src,"on_off=0x%x", on_off );
      New_write_display(src,2,0);      
      
      sprintf(src,"TheAddr=%d",TheAddr );
      New_write_display(src,3,0);      
      
      
      if(TheAddr == 3)WhichBd =o_3; else if(TheAddr == 4)WhichBd =o_4;
      if(on_off == 0)
      {
          for(i=0; i<12; i++)
          {
              clroutp(WhichBd[i]);  
          }    
      }
      else if(on_off == 1)
      {
          for(i=0; i<12; i++)
          {
              setoutp(WhichBd[i]); // setoutp(o_4[output_num]); 
          }              
      }     
        ReturnVal = CONTINUE_SERVICE;
        debugCase = 1; // monitor buttons
    break;
    
    case 1: // monitor buttons
        if(Check_for_Mode_Button()) 
        {
            debugCase = 0;
            ReturnVal = FINISHED_WITH_SERVICE;
        }    
    break;
    
    default:
        debugCase = 0;
        ReturnVal = FINISHED_WITH_SERVICE;    
    break;    
  }  
  
  return ReturnVal;    
};

int16 Output_on_1106(int16 Addr)  // externed in // GALX_1132_TEST.h
{
    static int8 count_num = 0;
    static int8 Debug_LED = 0;  // Change the state of Z0_LED
    static int8 DisplayDebug_1 = 0;	// REMOVE OR MODIFY FOR ACTUAL TEST
    static int16 Force_Fail_Pass = 0; // REMOVE OR MODIFY FOR ACTUAL TEST
    static int16 Special_Char_to_Display = -1;
    static int16 output_num = 0; // values are 0 - 11
    static int16 Col = 0;
    static int16 Output_1106_Case = 0; // 
    static int16 RetestResponse = 1; // Required for Retest Option
    int16 CurrentOutputState = 0;
    int16 UpdateDisplay = 0;

    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option

    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
 
    switch(Output_1106_Case) {
    case Init_Output_1106:
        clr_lcd_dply(); // clear 4-line display
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
        Set_iospi_Ctrl_flag(1);
        if(Addr == 3)CurrentOutputState = rdinp(i_3[output_num]);
        else if(Addr == 4)CurrentOutputState = rdinp(i_4[output_num]); 
        sprintf(src,"chan[%d] = %d",output_num, CurrentOutputState);
//        New_write_display(src,1,0);
        Output_1106_Case = CheckButtons;

        break;

    case CheckButtons:  // 
        if(Check_for_Up_Button()) 
        {          
            if(++output_num > 11)output_num = 0;
            UpdateDisplay = 1;
        }
        if (Check_for_Down_Button()) 
        {
            if(--output_num < 0)output_num = 11;
            UpdateDisplay = 1;
        }
        
        if(Check_for_Enter_Button() )
        {
            if(Addr == 3)CurrentOutputState = rdinp(i_3[output_num]);
            else if(Addr == 4)CurrentOutputState = rdinp(i_4[output_num]);
            // setoutp ; clroutp
            if(CurrentOutputState == 1)
            { // set to zero
                if(Addr == 3)clroutp(o_3[output_num]); 
                else if(Addr == 4) clroutp(o_4[output_num]); 
            }
            else
            { // set to one
                if(Addr == 3)setoutp(o_3[output_num]);
                else if(Addr == 4)setoutp(o_4[output_num]);
            }
            UpdateDisplay = 1;
        }
        if(Check_for_Mode_Button()) 
        {
            Output_1106_Case = Output_1106_Done;
            break;
        }
        //if(UpdateDisplay) Output_1106_Case = UpDateTheDisplay;
        Output_1106_Case = UpDateTheDisplay;
        break;       
        
    case UpDateTheDisplay:
        if(Addr == 3)CurrentOutputState = rdinp(i_3[output_num]);
        else if(Addr == 4)CurrentOutputState = rdinp(i_4[output_num]);
        sprintf(src,"chan[%d] = %d ",output_num, !CurrentOutputState);
//        New_write_display(src,1,0);
        UpdateDisplay = 0;
        Output_1106_Case = CheckButtons;
        break;             
        
    case Output_1106_Done:
        Output_1106_Case = Init_Output_1106;
        ReturnVal = FINISHED_WITH_SERVICE;    
        break;        

    default:
        Output_1106_Case=Init_Output_1106;
        New_write_display("1106AN OUTPUT FUNCT ",1,0);
        New_write_display("Executing...DEFAULT ",2,0);
        // SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
        ReturnVal = DEFAULT;  // Required for Retest Option
        break;
    }
    return ReturnVal; // Required for Retest Option	// 0
}

int16 Read_1106(int16 Address)
{
    // setoutp(MRtest_isignal_array[index]);
    //MRtest_isignal_array
    // rdinp(MRtest_isignal_array[index])
    int16 i = 0;
    if(Address == 3)
    {
        for(i=0; i < 11; i++);
        {        
            sprintf(src, "[%d]=%d", i, rdinp(i_3[i])); 
            Log_an_event(src);         
        }        
    }
        if(Address == 4)
    {
        for(i=0; i < 11; i++);
        {        
            sprintf(src, "[%d]=%d", i, rdinp(i_4[i])); 
            Log_an_event(src);         
        }        
    }
    
    return FINISHED_WITH_SERVICE;
}
*/
