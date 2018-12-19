// min_docment.c
// Provides minimal documentation on how to set up io.dat; consB.dat;
// where files are located and address for GALx-1107AN I/O board.
// Uses msg board A, or C (depending on consB[28] to deliver info.
// Log_an_event("outputs not off     ");  if( () () )
#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1137_1139.h"
#include "GALX_1121_global.h"

int16 The_msg_board_is = 0;

enum 
{
  initialize,
  fill_msg_board,
  display_msg_board,
	
};

enum 
{
  io_dat,
  consB_Kase,
  GALx_1107AN,
  file_location,
  Addr_1137,
  consB_Array,
  
};

////////////////////////////////

int16 Log_the_event(char *msg)
{
    if(The_msg_board_is == 1) Log_an_event(msg);
      else Log_an_event_C(msg);
}
    
int16 Generate_mini_doc(int16 which_doc)
{ // which_doc = 0 => io.dat
  //   1 => consB
  //   2 => 1107AN
  //   3 => file location
      
      int16 i = 0;
  

      switch (which_doc)
      {   // short description
          case io_dat:                              
              
              Log_the_event("use io #138 CT CAN  ");
              Log_the_event("starting with CTi0  ");
              
              Log_the_event("and ending with     ");
              Log_the_event("CTo11.              ");
              Log_the_event("Set address to 1    ");
              Log_the_event("for GALx_1107AN     ");
              Log_the_event("Verify that comm    ");
              Log_the_event("is blinking.        "); 
              
              Log_the_event("  io.dat format     ");
              Log_the_event("0 - 17 for Main I/O ");
              Log_the_event("18 - 137 MRCAN      ");
              Log_the_event("138 - 300 CTCAN     ");
              Log_the_event("GROUP CAN, 0 - 300  ");
             
              Log_the_event("Lst Update 5/07/18  ");
              break;
              
          case consB_Kase:
              Log_the_event("consB[0]:set for 16 ");
              Log_the_event("consB[21]: debug    ");
              Log_the_event("[23] HL address.    ");
              Log_the_event("[24] HL data.       ");
              Log_the_event("[27] direction for  ");
              Log_the_event("1137 & 1139.        ");
              Log_the_event("[27] also for select");
              Log_the_event("MR or CT CAN        ");
              Log_the_event("[28] 1 = msg_board A"); 
              Log_the_event("[28] 0 = msg_board C");
              Log_the_event("Lst Update 3/23/18  ");                        
              break;
                          
          case GALx_1107AN:
              Log_the_event("GALx_1107AN -       ");
              Log_the_event("Set address to 1    ");
              Log_the_event("for CT CAN          ");
              Log_the_event("Set address to 3    ");
              Log_the_event("for MR CAN          ");
              Log_the_event("Lst Update 3/23/18  ");
              break;
              
          case Addr_1137:
              Log_the_event("SET GALX-1137 addrs ");
              Log_the_event("Position 1, addr 1  ");
              Log_the_event("Position 2, addr 2  ");
              Log_the_event("Position 3, addr 3  ");
              Log_the_event("SET GALX-1139 addrs ");
              Log_the_event("For 1139, set addrs ");
              Log_the_event("to 3 for all 3 brds ");
              
              Log_the_event("                    ");
              Log_the_event("Lst Update 5/10/18  ");
              break;
                            
          case file_location:
              Log_the_event("v0.00.0.17_P in     ");
              Log_the_event("BoardTestSoftware/  ");
              Log_the_event("GALx1137 CAN...  /  ");
              Log_the_event("GALx1137 CAN...  /  ");
              Log_the_event("v0.00.0.00_P17   /  ");
              Log_the_event("Controller       /  ");
              Log_the_event("Controller.cww      ");
              Log_the_event("Lst Update 5/10/18  ");           
              break; 
              
          case consB_Array:              
	           for(i=0; i < 30; i++)
	           {
		           	sprintf(src,"consB[%d] = %d", i, consB[i]);
				    Log_the_event(src);
	           }
		      break;
		                                          
                 
          default:
              break;                
      }  // END OF switch(which_doc)    
}

int16 Display_a_min_doc(int16 which_doc)
{
    int16 i,j,k = 0;
    static int16 mini_Kase = initialize;
    static int16 Delay_Count = 3;     
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
    
    switch (mini_Kase)
    {
        case initialize:
            //  ADD CODE
            clr_lcd_dply();
            The_msg_board_is = consB[28] & 1;
            if (consB[28] & 1) Clear_Msg_Board();
              else Clear_Msg_Board_C();
            ReturnVal = CONTINUE_SERVICE;

            mini_Kase = fill_msg_board;
            break;
            
        case fill_msg_board:
            Generate_mini_doc(which_doc);
            //  ADD CODE
            mini_Kase = display_msg_board;
            break;
                        
        case display_msg_board:
            if (consB[28] & 1)
              ReturnVal =  Display_Message_Board(0);
                else ReturnVal =  Display_Message_Board_C(0);
            if(ReturnVal == FINISHED_WITH_SERVICE)
              mini_Kase = initialize;
            break;
            
        default:
            break;
            
    }
    return ReturnVal; 
}