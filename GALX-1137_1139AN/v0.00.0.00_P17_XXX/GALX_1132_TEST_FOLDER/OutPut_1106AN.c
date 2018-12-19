//  OutPut_1106AN.c


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

// tagwbg 105
// tagwbg 106
//  lower case lables indicate an 1106AN input channel;
//  UPPER CASE LABLES INDICATE AN 1106AN OUTPUT CHANNEL.
//

 
// tagwbg 105    //CAN Add 14, dip switch setting 4
	//output
void *Virtual_Board_A_OUT[12]= // used to be o_3 
{ // These will read the data items
  i_sufc ,  // 1 CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  // 2 CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  // 3 CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)  
  i_suf,    //CAN Add 14, dip switch setting 4
  i_sd ,    //CAN Add 14, dip switch setting 4
  i_sdf ,   //CAN Add 14, dip switch setting 4
  i_su ,    //CAN Add 14, dip switch setting 4  
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)   
};
// tagwbg 110
    //CAN Add 14, dip switch setting 4
	//input
void *Virtual_Board_A_in[12]=  // used to be i_3
{
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)  
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_sufc ,  //CAN Add 14, dip switch setting 4 (dummy location, waiting to be assigned)
  i_suf,    //CAN Add 14, dip switch setting 4 
  i_sd ,    //CAN Add 14, dip switch setting 4 
  i_sdf ,   //CAN Add 14, dip switch setting 4 
  i_su ,    //CAN Add 14, dip switch setting 4
  i_sdc,    //CAN Add 14, dip switch setting 4       
};

//  lower case lables indicate an 1106AN input channel;
//  UPPER CASE LABLES INDICATE AN 1106AN OUTPUT CHANNEL.

	//CAN Add 15, dip switch setting 5
	//output
void *Virtual_Board_B_OUT[12]=    // used to be o_4
{ //  These are 1106AN OUTPUTS  
  o_S3TPL,  //            CAN Add 14, dip switch setting 4
  o_S1TPH,  //            CAN Add 15, dip switch setting 5
  o_15TSD , // iTSD     //CAN Add 14, dip switch setting 4
  o_17CST , // CST      //CAN Add 14, dip switch setting 4
  o_12ICA , // ICA      //CAN Add 14  dip switch setting 4
  o_S5LPS,  //            CAN Add 14, dip switch setting 4 , // LPS
  o_S4LOS,  //            CAN Add 14, dip switch setting 4    
  o_23GOV,  //            CAN Add 15, dip switch setting 5
  o_21TLH,  //            CAN Add 15, dip switch setting 5
  o_H19PS,  //            CAN Add 15, dip switch setting 5
  o_17HSS,  //            CAN Add 15, dip switch setting 5
  o_10TAU , //CAN Add 16, dip switch setting 6 ,// TAU (TPL, SPD)
  
//  o_RLM,  // RLM        //CAN Add 17, dip switch setting 7
//  o_H3DLT , // DLT      //CAN Add 17, dip switch setting 7
//  
//  o_17CST , // CST      //CAN Add 18, dip switch setting 8
  
//  o_H5DLM , // DLM      //CAN Add 16, dip switch setting 6
//  o_H7DLB , // DLB      //CAN Add 16, dip switch setting 6
  
//  o_H9TAD,  //CAN Add 16, dip switch setting 6 , // TAD (LOS, MTO)
//  o_10TAU , //CAN Add 16, dip switch setting 6 ,// TAU (TPL, SPD)
//  o_12BAD , //CAN Add 16, dip switch setting 6 ,// BAD (TPH, CFLT)
//  o_13BAU , //CAN Add 16, dip switch setting 6 ,// (BAU, GOV-H22)   
};


	//CAN Add 15, dip switch setting 5
	//input
void *Virtual_Board_B_in[12]= 
{
  o_RLM,  // RLM        //CAN Add 17, dip switch setting 7
  o_H3DLT , // DLT      //CAN Add 17, dip switch setting 7
  o_15TSD , // iTSD     //CAN Add 16, dip switch setting 6
  
  o_12ICA , // ICA      //CAN Add 18, dip switch setting 8
  o_H5DLM , // DLM      //CAN Add 16, dip switch setting 6
  o_H7DLB , // DLB      //CAN Add 16, dip switch setting 6
  o_S5LPS,  //CAN Add 14, dip switch setting 4 , // LPS
  o_H9TAD,  //CAN Add 16, dip switch setting 6 , // TAD (LOS, MTO)
  o_10TAU , //CAN Add 16, dip switch setting 6 ,// TAU (TPL, SPD)
  o_12BAD , //CAN Add 16, dip switch setting 6 ,// BAD (TPH, CFLT)
  o_13BAU , //CAN Add 16, dip switch setting 6 ,// (BAU, GOV-H22)  
};

void *Virtual_Board_C_OUT[11]=   
{ //  These are 1106AN OUTPUTS  

  o_13BAU , //CAN Add 16, dip switch setting 6 ,// (BAU, GOV-H22)
  o_12BAD , //CAN Add 16, dip switch setting 6 ,// BAD (TPH, CFLT)
  o_10TAU , //CAN Add 16, dip switch setting 6 ,// TAU (TPL, SPD)  
  o_H9TAD,  //CAN Add 16, dip switch setting 6 , // TAD (LOS, MTO)
  o_H7DLB , // DLB      //CAN Add 16, dip switch setting 6
  o_H5DLM , // DLM      //CAN Add 16, dip switch setting 6
  o_H3DLT , // DLT      //CAN Add 17, dip switch setting 7
  o_RLM,  // RLM        //CAN Add 17, dip switch setting 7
  o_0CFLT,  // CFLT        //CAN Add 17, dip switch setting 7
  o_0SPD,  // SPD        //CAN Add 17, dip switch setting 7
  o_0MTO, // MTO        //CAN Add 17, dip switch setting 7
//  o_H3DLT , // DLT      //CAN Add 17, dip switch setting 7
//  
//  o_17CST , // CST      //CAN Add 18, dip switch setting 8
  
//  o_H5DLM , // DLM      //CAN Add 16, dip switch setting 6
//  o_H7DLB , // DLB      //CAN Add 16, dip switch setting 6
  
//  o_H9TAD,  //CAN Add 16, dip switch setting 6 , // TAD (LOS, MTO)
//  o_10TAU , //CAN Add 16, dip switch setting 6 ,// TAU (TPL, SPD)
//  o_12BAD , //CAN Add 16, dip switch setting 6 ,// BAD (TPH, CFLT)
//  o_13BAU , //CAN Add 16, dip switch setting 6 ,// (BAU, GOV-H22)   
};

struct IO_1106AN
{
  void *Virtual_Board_A_OUT;
  void *Virtual_Board_B_OUT;
  void *Virtual_Board_A_in;
  void *Virtual_Board_B_in;    
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
//-SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
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
      
      
      if(TheAddr == 3)WhichBd =Virtual_Board_A_OUT; else if(TheAddr == 4)WhichBd =Virtual_Board_B_OUT;
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

//- SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED
 
    switch(Output_1106_Case) {
    case Init_Output_1106:
        clr_lcd_dply(); // clear 4-line display
        ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
        Set_iospi_Ctrl_flag(1);
        if(Addr == 3)CurrentOutputState = rdinp(Virtual_Board_A_in[output_num]);
        else if(Addr == 4)CurrentOutputState = rdinp(Virtual_Board_B_in[output_num]); 
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
            if(Addr == 3)CurrentOutputState = rdinp(Virtual_Board_A_in[output_num]);
            else if(Addr == 4)CurrentOutputState = rdinp(Virtual_Board_B_in[output_num]);
            // setoutp ; clroutp
            if(CurrentOutputState == 1)
            { // set to zero
                if(Addr == 3)clroutp(Virtual_Board_A_OUT[output_num]); 
                else if(Addr == 4) clroutp(Virtual_Board_B_OUT[output_num]); 
            }
            else
            { // set to one
                if(Addr == 3)setoutp(Virtual_Board_A_OUT[output_num]);
                else if(Addr == 4)setoutp(Virtual_Board_B_OUT[output_num]);
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
        if(Addr == 3)CurrentOutputState = rdinp(Virtual_Board_A_in[output_num]);
        else if(Addr == 4)CurrentOutputState = rdinp(Virtual_Board_B_in[output_num]);
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
            sprintf(src, "[%d]=%d", i, rdinp(Virtual_Board_A_in[i])); 
            Log_an_event(src);         
        }        
    }
        if(Address == 4)
    {
        for(i=0; i < 11; i++);
        {        
            sprintf(src, "[%d]=%d", i, rdinp(Virtual_Board_B_in[i])); 
            Log_an_event(src);         
        }        
    }
    
    return FINISHED_WITH_SERVICE;
}