//  FPGA.c


#include "global.h"
#include "GALX_1132_TEST.h"
#include "GALX_1121_TEST.h"
#include "GALX_1121_global.h"



extern void iospi_pack (void);
extern void i1spi_pack (void);
extern void nts_spi_pack (uint8 msg);
extern uint16 iospi_rx_dma_buf[];
void log_CPLD1(void);
void log_CPLD2(void);
void log_NTS(void);
void Show_FPGA_Addresses(void);
int16 Set_specific_output(int16 Output_Num_and_on_off);
int16 Set_All_outputs(int16 on_off);
int16 UseFPGA1_B0 = 1;
uint8 CPLD1_unpk_flg = 0;
uint8 CPLD2_unpk_flg = 0;
uint8 NTS_unpk_flg = 0;
uint8 CPLD1_unpk_ctrl = 1;
uint8 CPLD2_unpk_ctrl = 1;
uint8 NTS_unpk_ctrl = 1;

void *FPGA_ptr;



// ZYM 12-7-2017
int16 NTS_SPI_TEST_3bit=0;		//ONLY using less than 3 bit


enum
{
    InitTask,
    FirstUnpk,
    PackAgain,
    Wait,
    RunTask,
    Failed,
    Passed,
    TaskDone,
    Retest_option, // Required for Retest Option
    Display_The_Buff
};

enum
{
    CPLD1,
    CPLD2,
    NTS
};


typedef union {
    unsigned short int word;
    unsigned char byte[2];
} WORD_BYTE;

WORD_BYTE	word_byte;



int16 return_CPLD1_SPI_TEST_byte=0;
int16 return_CPLD2_SPI_TEST_byte=0;

extern int32 test_msg;
extern int32 test_msg1;
extern int32 test_msg2;
extern int32 test_msg3;
extern int32 test_msg4;





#define SPI_Fault_Display_Index	2
char SPI_Fault_Display[SPI_Fault_Display_Index][20]=
{
	"Failed.             ",
	"IC4,6   CN9         ",	
};





int16 Read_FPGA_ver_rev(int16 which_fpga)
{
//-  int16 Delay_Count = consB[20];
  int16 Delay_Count=test_1s_delay;
  static int16 NextCaseAfterWait = 0;
  static int16 The_FPGA  = 0;
  static int16 the_case  = InitTask;
  static int16 Retest_Option_Parm = 0; // Required for Retest Option
  static int16 RetestResponse = 1; // Required for Retest Option
  static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
  int16 i = 0;
  
  static int16 max_counter=10;
  static int16 counter=0;
  static int16 sub_cpld1=0;
  static int16 sub_cpld2=0;  
  static int16 sub_nts=0;
  
    switch(the_case)
    {    	
// ZYM 12-7-2017
	    case InitTask:
            The_FPGA = which_fpga;
            clr_lcd_dply(); // clear 4-line display
            Clear_Log_Buffer();
//            for(i=0;i<6;i++) clrgoutp(GRtest_osignal_array[i]);                      
	        CurrentTestPassFail = 0 ;  // fail
            ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
            Retest_Option_Parm = 0; // Required for Retest Option
            RetestResponse = 1; // Required for Retest Option            
            if(ForceTestToIndicateFail)
            {
              the_case = Failed;
              ReturnVal = CONTINUE_SERVICE; 
              break;  
            }                         
            
          	NextCaseAfterWait=RunTask;
          	the_case=Wait; 
          	
          	counter=0;
          	sub_cpld1=0;  
          	sub_cpld2=0;
          	sub_nts=0; 	
          	NTS_SPI_TEST_3bit=0;
          	
//-       	if(Delay_Count < 1)  Delay_Count = 1;
//-        	if(Delay_Count > 20) Delay_Count = 20;	 
        	timers[t_Generic_Delay]=0;                
        break;
        	
/* //-    
        case InitTask:
            The_FPGA = which_fpga;
            clr_lcd_dply(); // clear 4-line display
            Clear_Log_Buffer();
//            for(i=0;i<6;i++) clrgoutp(GRtest_osignal_array[i]);                      
            CurrentTestPassFail = 0 ;  // fail
            ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
            Retest_Option_Parm = 0; // Required for Retest Option
            RetestResponse = 1; // Required for Retest Option            
            if(ForceTestToIndicateFail)
            {
              the_case = Failed;
              ReturnVal = CONTINUE_SERVICE; 
              break;  
            }            
            Delay_Count = 3;
            switch(The_FPGA)
            {
                case CPLD1:
                  i1spi_pack();
                  break;  

                case CPLD2:
                  iospi_pack();
                  break;    
                
                case NTS:
                  nts_spi_pack(1);
                  break;  
                        
                default:                 
                  break;                
            }
            NextCaseAfterWait = FirstUnpk;
            the_case = Wait;                      
        break;

        case FirstUnpk:
            switch(The_FPGA)
            {
                case CPLD1: // i1spi
                  i1spi_unpack(); 
                  break;  

                case CPLD2: // iospi
                  iospi_unpack();  
                  break;    
                
                case NTS:
                  nts_spi_unpack();
                  break;  
                        
                default:                 
                  break;                
            }   
            NextCaseAfterWait = PackAgain;
            the_case = Wait;     
            break;
                    
        case PackAgain:
            Clear_Log_Buffer();
            switch(The_FPGA)
            {
                case CPLD1:
                  i1spi_pack();
                  break;  

                case CPLD2:
                  iospi_pack();
                  break;    
                
                case NTS:
                  nts_spi_pack(1);
                  break;  
                        
                default:                 
                  break;                
            }
            NextCaseAfterWait = RunTask;
            the_case = Wait;        
            break;            
*/            
        case RunTask:
            switch(The_FPGA)
            {
                case CPLD1: // i1spi   
                	switch(sub_cpld1)
                	{
                		case 0:                			
        					New_write_display("FPGA1 SPI TEST      ",0,0);
        					New_write_display("TX:      RX:        ",1,0);
        					New_write_display("Counter:            ",2,0);
        					New_write_display("                    ",3,0); 
                			FPGA1_HW_Modify_Output.B0.CPLD1_SPI_TEST_bit=1;
                			sub_cpld1=1;
                			timers[t_Generic_Delay]=0;
                			sprintf(&LCD_Display[1][3],"%02i",FPGA1_HW_Modify_Output.B0.CPLD1_SPI_TEST_bit);   
                		break;
                		
                		case 1:
                			if(timers[t_Generic_Delay]<=Delay_Count)
                	 			break;
                	 	
                			if(i1UnpkOK)
                  	 		{
                  	 			//CPU sending 1 to FPGA1,FPGA1 return 6 back to CPU  
                     			//CPU sending 0 to FPGA1,FPGA1 return 0 back to CPU                      	
                     			if(return_CPLD1_SPI_TEST_byte==FPGA1_HW_Modify_Output.B0.CPLD1_SPI_TEST_bit*6)                  	 		
                     			{
                     				counter++;	
                     				sub_cpld1=2;
                     			}
                     			else
                     			{
                     				the_case=Failed;
                     			}
                     		}
                     		else
                     		{
                     			the_case=Failed;
                     		}  
                     		timers[t_Generic_Delay]=0;                        		                
                		break;
                		
                		case 2:
                			FPGA1_HW_Modify_Output.B0.CPLD1_SPI_TEST_bit=0;
                			sub_cpld1=3;
                			timers[t_Generic_Delay]=0;
                			sprintf(&LCD_Display[1][3],"%02i",FPGA1_HW_Modify_Output.B0.CPLD1_SPI_TEST_bit);  
                		break;
                		
                		case 3:
                			if(timers[t_Generic_Delay]<=Delay_Count)
                	 			break;
                	 	                			
                			
                			if(i1UnpkOK)
                  	 		{
                  	 			//CPU sending 1 to FPGA1,FPGA1 return 6 back to CPU  
                     			//CPU sending 0 to FPGA1,FPGA1 return 0 back to CPU                      	
                     			if(return_CPLD1_SPI_TEST_byte==FPGA1_HW_Modify_Output.B0.CPLD1_SPI_TEST_bit)                  	 		
                     			{
                     				counter++;
                     				if(counter<max_counter)
                     				{
                     					sub_cpld1=0;	
                     				}
                     				else
                     				{
                     					counter=0;
                     					sub_cpld1=0;
                     					the_case=Passed;		
                     				}                     				
                     			}
                     			else
                     			{
                     				the_case=Failed;
                     			}
                     		}
                     		else
                     		{
                     			the_case=Failed;
                     		} 
                     		timers[t_Generic_Delay]=0;                            		              
                		break;
                		
                		default:
                			counter=0;  
                			sub_cpld1=0;                   			            
                		break; 
                	} 
                	                	
                	sprintf(&LCD_Display[1][12],"%02i",return_CPLD1_SPI_TEST_byte);   
                	sprintf(&LCD_Display[2][8],"%02i",counter);               	              	                 	                               	 
                break;  

                case CPLD2: // iospi
                	switch(sub_cpld2)
                	{
                		case 0:
                			New_write_display("FPGA2 SPI TEST      ",0,0);
        					New_write_display("TX:      RX:        ",1,0);
        					New_write_display("Counter:            ",2,0);
        					New_write_display("                    ",3,0); 
                			iospi_out.ST.buff0.B.CPLD2_SPI_TEST_bit=1;
                			sub_cpld2=1;
                			timers[t_Generic_Delay]=0;
                			sprintf(&LCD_Display[1][3],"%02i",iospi_out.ST.buff0.B.CPLD2_SPI_TEST_bit); 
                		break;
                		
                		case 1:
                			if(timers[t_Generic_Delay]<=Delay_Count)
                	 			break;
                	 	
                			                			
                			if(ioUnpkOK)
                     		{
                     			//CPU sending 1 to FPGA2,FPGA2 return 7 back to CPU  
                     			//CPU sending 0 to FPGA2,FPGA2 return 0 back to CPU                    	
                     			if(return_CPLD2_SPI_TEST_byte==iospi_out.ST.buff0.B.CPLD2_SPI_TEST_bit*7)
                     			{
                     				counter++;	
                     				sub_cpld2=2;
                     			}
                     			else
                     			{
                     				the_case=Failed;
                     			}
                     		}
                     		else
                     		{
                     			the_case=Failed;
                     		}  
                     		timers[t_Generic_Delay]=0;                   
                		break;
                		
                		case 2:
                			iospi_out.ST.buff0.B.CPLD2_SPI_TEST_bit=0;
                			sub_cpld2=3;
                			timers[t_Generic_Delay]=0;
                			sprintf(&LCD_Display[1][3],"%02i",iospi_out.ST.buff0.B.CPLD2_SPI_TEST_bit); 
                		break;
                		
                		case 3:
                			if(timers[t_Generic_Delay]<=Delay_Count)
                	 			break;
                	 	                			
                			
                			if(ioUnpkOK)
                     		{
                     			//CPU sending 1 to FPGA2,FPGA2 return 7 back to CPU  
                     			//CPU sending 0 to FPGA2,FPGA2 return 0 back to CPU                    	
                     			if(return_CPLD2_SPI_TEST_byte==iospi_out.ST.buff0.B.CPLD2_SPI_TEST_bit)
                     			{
                     				counter++;
                     				if(counter<max_counter)
                     				{
                     					sub_cpld2=0;	
                     				}
                     				else
                     				{
                     					counter=0;
                     					sub_cpld2=0;
                     					the_case=Passed;		
                     				}                     				
                     			}
                     			else
                     			{
                     				the_case=Failed;
                     			}
                     		}
                     		else
                     		{
                     			the_case=Failed;
                     		} 
                     		timers[t_Generic_Delay]=0;                    
                		break;
                		
                		default:
                			counter=0;  
                			sub_cpld2=0;               
                		break; 
                	}  
                	                	  
                	sprintf(&LCD_Display[1][12],"%02i",return_CPLD2_SPI_TEST_byte); 
                	sprintf(&LCD_Display[2][8],"%02i",counter);               	
                break;    
                
                case NTS:
					switch(sub_nts)
                	{
                		case 0:
                			New_write_display("NTS SPI TEST        ",0,0);
        					New_write_display("TX:      RX:        ",1,0);
        					New_write_display("Counter:            ",2,0);
        					New_write_display("                    ",3,0); 
                			NTS_SPI_TEST_3bit++;
                			sub_nts=1;
                			timers[t_Generic_Delay]=0;
                			sprintf(&LCD_Display[1][3],"%02i",NTS_SPI_TEST_3bit);   
                		break;
                		
                		case 1:
                			if(timers[t_Generic_Delay]<=Delay_Count)
                	 			break;
                	 	
                			SIU.GPDO[Z6_LED].R=!SIU.GPDO[Z6_LED].R;                			
                			if(ntsUnpkOK)
				  			{
				  				//CPU sending and receiving same data from NTS 
				  				if(NTS_spi.ver==NTS_SPI_TEST_3bit)
				  				{
				  					counter++;	                     				
                     				if(counter<max_counter)
                     				{
                      					sub_nts=0;	
                     				}
                     				else
                     				{
                     					counter=0;
                      					sub_nts=0;
                      					the_case=Passed;		
                     				}    
				  				}
				  				else    
                     			{
                      				the_case=Failed;
                     			}				
				  			}                     			
                     		else
                     		{
                      			the_case=Failed;
                     		}  
                     		timers[t_Generic_Delay]=0;                   
                		break;
                		
                		
                		default:
                			counter=0;  
                			sub_nts=0;               
                		break;        
                	}
                	
                	sprintf(&LCD_Display[1][12],"%02i",NTS_spi.ver);   
                	sprintf(&LCD_Display[2][8],"%02i",counter);             	                	                
                break;  
                       
                default:                 
                break;                
            } 
            
            
            
/* //-            
			if(CurrentTestPassFail)the_case = Passed;
              else the_case = Failed;
              
              

            if(The_FPGA == 1)word_byte.word=iospi_in.buff[0]; 
              	else word_byte.word=i1spi_in.buff[0];
         // word_byte.word=iospi_in.buff[0];            
          	sprintf(src,"0=%d", word_byte.byte[0]);                   
//          	Log_an_event(src);          

          	sprintf(src,"1=%d", word_byte.byte[1]);
//          	Log_an_event(src);

          	if(The_FPGA == 1)word_byte.word=iospi_in.buff[1]; 
              	else word_byte.word=i1spi_in.buff[1];  
         // word_byte.word=iospi_in.buff[1];        
          	sprintf(src,"2=%d", word_byte.byte[0]);
//         	 	Log_an_event(src);
          	sprintf(src,"3=%d", word_byte.byte[1]);
//         	 	Log_an_event(src);
          
         	if(The_FPGA == 1)word_byte.word=iospi_in.buff[2]; 
              	else word_byte.word=i1spi_in.buff[2];
        //  word_byte.word=iospi_in.buff[2];            
          	sprintf(src,"4=%d", word_byte.byte[0]);
//          	Log_an_event(src);
          	sprintf(src,"5=%d", word_byte.byte[1]);
//         		Log_an_event(src);
          
         	if(The_FPGA == 1)word_byte.word=iospi_in.buff[3]; 
             	else word_byte.word=i1spi_in.buff[3];
         // word_byte.word=iospi_in.buff[3];            
          	sprintf(src,"6=%d", word_byte.byte[0]);
//          	Log_an_event(src);
          	sprintf(src,"7=%d", word_byte.byte[1]);
//          	Log_an_event(src);
                          
            if(CurrentTestPassFail)the_case = Passed;
              else the_case = Failed;
            sprintf(src,"ver_rev=%d", ver_rev);
//            Log_an_event(src); 
            
            if(The_FPGA == 2)
            { //  NTS
              Clear_Log_Buffer();
              sprintf(src,"ver_rev=%d %d",NTS_spi.ver, NTS_spi.rev );
//              Log_an_event(src);  
            }   
*/                         
        break;
         
        case Wait:
            if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
                timers[t_Generic_Delay] = 0; //
                the_case = NextCaseAfterWait;
            }        
            break;
 
        case Passed:
        	clr_lcd_dply();
        	switch(The_FPGA)
            {
            	case CPLD1: // i1spi   
					New_write_display("FPGA1 SPI TEST      ",0,0);
				break;

				case CPLD2: // iospi
					New_write_display("FPGA2 SPI TEST      ",0,0);
				break;
				
				case NTS:
					New_write_display("NTS SPI TEST        ",0,0);
				break;            
            }
          	New_write_display("TEST PASSED         ",1,0);
         	New_write_display("GOOD JOB            ",2,0);                   
          	CurrentTestPassFail = 1 ;
         	the_case = TaskDone;
          break; 
  
        case Failed:
//-        	clr_lcd_dply();
          	switch(The_FPGA)
            {
            	case CPLD1: // i1spi   
					New_write_display(SPI_Fault_Display[0],3,0);
        
        			GALX1121_Fault_Display_ADDIN_Function();
        			Log_an_event(SPI_Fault_Display[1]);
				break;

				case CPLD2: // iospi
					New_write_display(SPI_Fault_Display[0],3,0);
        
        			GALX1121_Fault_Display_ADDIN_Function();
        			Log_an_event(SPI_Fault_Display[1]);
				break;
				
				case NTS:
					New_write_display(SPI_Fault_Display[0],3,0);
        
        			GALX1121_Fault_Display_ADDIN_Function();
        			Log_an_event(SPI_Fault_Display[1]);
				break;            
            }
//-       sprintf(&LCD_Display[0][1],"%5i", test_msg4);
//-       sprintf(&LCD_Display[0][7],"%5i", test_msg);
//-       sprintf(&LCD_Display[1][1],"%5i", test_msg1);
//-       sprintf(&LCD_Display[1][7],"%5i", test_msg2);
//-       sprintf(&LCD_Display[2][1],"%5i", test_msg3);
          CurrentTestPassFail = 0;
          the_case = Retest_option;
          break;                
                
        case Retest_option:
            RetestResponse = Handle_Retest_Request(Retest_Option_Parm);
            Retest_Option_Parm = 1;
            if(RetestResponse == 0) the_case = TaskDone;
            else if(RetestResponse == 1) the_case = InitTask; 
            else 
            ReturnVal = CONTINUE_SERVICE;  
          break;                
                
        case TaskDone:
//-         New_write_display("Done      ",0,0);
//-         New_write_display(src,1,0);
//          for(i=0;i<12;i++) 
//              clrgoutp(GRtest_osignal_array[i]);                      
                     
          the_case = InitTask;
          // Required for Retest Option
          if(RetestResponse == 0)ReturnVal = FINISHED_WITH_SERVICE; 
          else ReturnVal = DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
          break;                
        
        default:
          ReturnVal = DEFAULT;
          break;        
    }
    return ReturnVal; // Required for Retest Option	    
}



/////////////////////////////////////////////////////////////////


int16 Unpack_FPGA_Only(int16 Flags_and_which_fpga)
{
  static int16 The_FPGA  = 0;
  static int16 The_FPGA_flag  = 0;
  static int16 the_case  = InitTask;
  static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
  int16 i = 0;

    switch(the_case)
    {
        case InitTask:
            The_FPGA = Flags_and_which_fpga & 0x0fff;  //msb is reserved for a flag to log event.
            The_FPGA_flag = (Flags_and_which_fpga & 0xf000) >> 12;
            clr_lcd_dply(); // clear 4-line display
            Clear_Log_Buffer();
//            for(i=0;i<6;i++) clrgoutp(GRtest_osignal_array[i]);                      
 //           CurrentTestPassFail = 1 ;  // this is not a test
            ReturnVal = CONTINUE_SERVICE; 
            CPLD1_unpk_flg =  CPLD2_unpk_flg = NTS_unpk_flg = 0;
            CPLD1_unpk_ctrl =  CPLD2_unpk_ctrl = NTS_unpk_ctrl = 0;       
            the_case = RunTask;                      
        break;                               
            
        case RunTask:
            Clear_Log_Buffer();
            switch(The_FPGA)
            {
                case CPLD1: // i1spi
                    CPLD1_unpk_ctrl = 1;//  i1spi_unpack();
//                    Clear_Log_Buffer();
//                  if(The_FPGA_flag == 3)  log_CPLD1(); // Display buffer                  
                  break;  

                case CPLD2: // iospi
                    CPLD2_unpk_ctrl = 1;
                 // iospi_unpack(); 
//                  Clear_Log_Buffer(); 
//                  if(The_FPGA_flag == 3)  log_CPLD2(); // Display buffer                
                  break;    
                
                case NTS:
                    NTS_unpk_ctrl = 1;
                  // nts_spi_unpack();
//                  Clear_Log_Buffer();
//                  if(The_FPGA_flag == 3)   log_NTS(); // Display ver-rev                  
                  if(ntsUnpkOK || 
                      ( (iospi_rx_dma_buf[2]) == 1) &&
                          ( (iospi_rx_dma_buf[3]) == 2) )
                               CurrentTestPassFail = 1;
                  break;  
                        
                default:                 
                  break;                
            }  

            the_case = Display_The_Buff;               
            break; 
            
        case Display_The_Buff:
            if(CPLD1_unpk_flg || CPLD2_unpk_flg || NTS_unpk_flg )
            {
                CPLD1_unpk_ctrl =  CPLD2_unpk_ctrl = NTS_unpk_ctrl = 0;
                ReturnVal = DisplayMsgBrd_A(0);
                if(ReturnVal == CONTINUE_SERVICE) break;
                else
                {
                    the_case = TaskDone;
                    ReturnVal = CONTINUE_SERVICE;                
                }                 
            }
            else if(Check_for_Mode_Button()) {
                the_case = TaskDone;
                ReturnVal = CONTINUE_SERVICE;
            }            
            break;                       
                
        case TaskDone:                                         
          the_case = InitTask;
          Check_for_Mode_Button();
          Check_for_Enter_Button();  // Release Mode & Enter keys          
          // Required for Retest Option
          ReturnVal = FINISHED_WITH_SERVICE; 
          break;                
        
        default:
          ReturnVal = DEFAULT;
          break;        
    }
    return ReturnVal; 	    
}

///////////////////////


void log_CPLD1(void) // NOT YET WORKING
{ int16 i = 0;
  for(i=0; i<4; i++)
  {
    word_byte.word=i1spi_in.buff[i];
    sprintf(src,"%d=%d", 2*i, word_byte.byte[0]);                   
    Log_an_event(src);
    sprintf(src,"%d=%d", (2*i)+1, word_byte.byte[1]);                   
    Log_an_event(src); 
  }  
}

void log_CPLD2(void)  // NOT YET WORKING
{ int16 i = 0;
  for(i=0; i<4; i++)
  {
    word_byte.word=iospi_in.buff[i];
    sprintf(src,"%d=%d", 2*i, word_byte.byte[0]);                   
    Log_an_event(src);
    sprintf(src,"%d=%d", (2*i)+1, word_byte.byte[1]);                   
    Log_an_event(src); 
  }  
}

void log_NTS(void)
{
    sprintf(src,"ver_rev=%d %d",NTS_spi.ver, NTS_spi.rev );
    Log_an_event(src);
}


int16 Set_CPLD1_Addr(int16 param)
{
    switch(param)
    {
        case 0: // Default
            cpld1_B_mlb = 0x93; // address for CPLD1 (safety PAL)
            cpld1_B_lb  = 0x55;        
            break;
              
        case 1:
            cpld1_B_mlb = 0x93; // address for CPLD1 (safety PAL)
            cpld1_B_lb  = 0x56;        
            break;    
        
        case 2:
            cpld1_B_mlb = 0x93; // address for CPLD1 (safety PAL)
            cpld1_B_lb  = 0x57;        
            break;  

        case 3:
            cpld1_B_mlb = 0x93; // address for CPLD1 (safety PAL)
            cpld1_B_lb  = 0x58;        
            break;    
        
        case 4:
            cpld1_B_mlb = 0x93; // address for CPLD1 (safety PAL)
            cpld1_B_lb  = 0x59;        
            break;  
         
        case 5:
            cpld1_B_mlb = 0x93; // address for CPLD1 (safety PAL)
            cpld1_B_lb  = 0x5A;        
            break;  
                         
        default:
            return DEFAULT;
            break;           
    } 
    Show_FPGA_Addresses();    
    return FINISHED_WITH_SERVICE;    
}

void Show_FPGA_Addresses(void)
{
    Clear_Log_Buffer();
    sprintf(src,"CPLD1 Addr=0x%X%X",cpld1_B_mlb, cpld1_B_lb );
    Log_an_event(src);  
    sprintf(src,"CPLD2 Addr=0x%X%X",cpld2_B_mlb, cpld2_B_lb );
    Log_an_event(src);    
}

int16 Set_CPLD2_Addr(int16 param)
{
    switch(param)
    {
        case 0: // Default
            cpld2_B_mlb = 0x61; // address for CPLD2
            cpld2_B_lb  = 0x2A;        
            break;
              
        case 1:
            cpld2_B_mlb = 0x61; // address for CPLD2
            cpld2_B_lb  = 0xAB;        
            break;    
        
        case 2:
            cpld2_B_mlb = 0x61; // address for CPLD2
            cpld2_B_lb  = 0x2C;        
            break;  

        case 3:
            cpld2_B_mlb = 0x61; // address for CPLD2
            cpld2_B_lb  = 0x2D;        
            break;    
        
        case 4:
            cpld2_B_mlb = 0x61; // address for CPLD2
            cpld2_B_lb  = 0x2E;        
            break;  
         
        case 5:
            cpld2_B_mlb = 0x61; // address for CPLD2
            cpld2_B_lb  = 0x2F;        
            break;  
                         
        default:
            return DEFAULT;
            break;           
    }    
    Show_FPGA_Addresses();   
    return FINISHED_WITH_SERVICE;    
}


// Enable_iospi_Ctrl
int16 Set_iospi_Ctrl_flag(int16 enable_disable)
{
    Enable_iospi_Ctrl = enable_disable;
    return FINISHED_WITH_SERVICE;
}

/*
	//                 B.UT ???
    iospi_out.ST.buff0.B.SUF = 1;  //  OUTPUT MODULE 1
    
 //                      UN  -- from NTS
    iospi_out.ST.buff0.B.SU  = 1; //   OUTPUT MODULE 2
    
//                       DT  -- from NTS
    iospi_out.ST.buff0.B.SDF = 1; //   OUTPUT MODULE 3
      
//                       DN  -- from NTS    
    iospi_out.ST.buff0.B.SD  = 0; //   OUTPUT MODULE 4 
    
    iospi_out.ST.buff0.B.STE = 1; //   OUTPUT MODULE 5
    iospi_out.ST.buff0.B.CEN = 1;
    
    iospi_out.ST.buff0.B.MC = 1;  //   OUTPUT MODULE 6
//                     B.RUN ???
        
// NTS

    UN, DN, UT, DT

*/

int16 Set_All_outputs(int16 on_off)
{
    Set_iospi_Ctrl_flag(1);
    SUF_output_cmnd_flag = on_off; // 0
    SU_output_cmnd_flag  = on_off; // 1
    SDF_output_cmnd_flag = on_off; // 2
    SD_output_cmnd_flag = on_off;  // 3
    STE_output_cmnd_flag = on_off; // 4
    CEN_output_cmnd_flag = on_off; // 5
    MC_output_cmnd_flag  = on_off; // 6 
    RUN_output_cmnd_flag = on_off; // 7  
    return FINISHED_WITH_SERVICE;   
}

//ob 0  0  0  0  ^ 0  0  0  0 ^^ 0  0  0  0 ^ 0  0  0  0 
//   15 14 13 12   11 10 9  8    7  6  5  4   3  2  1  0
int16 Set_specific_output(int16 Output_Num_and_on_off)
{
  int16 outputNumber = Output_Num_and_on_off & 0x0F;
  int16 OnOff = Output_Num_and_on_off & 0x0F000;
  
  static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option 
  
  sprintf(&LCD_Display[0][1],"FPGA1=%05i",FPGA1_HW_Modify_Output.mc_drop_delay);
  sprintf(&LCD_Display[0][13],"%05i", return_CPLD1_SPI_TEST_byte);
  sprintf(&LCD_Display[1][1],"NTS=%03i",NTS_HW_Modify_Output.byte);
  sprintf(&LCD_Display[2][1],"rNTS=%03i",NTS_spi.rev);
  sprintf(&LCD_Display[2][10],"CPUver=%03i",NTS_spi.ver);
  sprintf(&LCD_Display[3][1],"SUF=%05i",iospi_out.ST.buff0.B.SUF);
  
  switch(outputNumber)
  {
    case 0:			//SUF    	
        if(OnOff)    
        {  
// ZYM 12-18-2017
// using "setoutp()" and "clroutp()" for MAIN IO not working, need to find out why?              
//-        	setoutp(o_SUF); 
        	iospi_out.ST.buff0.B.SUF=1;
         	FPGA1_HW_Modify_Output.B0.PSUF_LP=1;
        } 
        else    
        {
//-        	clroutp(o_SUF);
        	iospi_out.ST.buff0.B.SUF=0;
         	FPGA1_HW_Modify_Output.B0.PSUF_LP=0; 			 
        }
 	    ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time  
    break;
        
    case 1:    		//SU
        if(OnOff)
        {
//-        	setoutp(o_SU);
        	iospi_out.ST.buff0.B.SU=1; 
        	FPGA1_HW_Modify_Output.B0.PSU_LP=1;     
//-			FPGA1_HW_Modify_Output.mc_drop_delay=0xFFFF;   	
        }
        else
        {
//-        	clroutp(o_SU);
        	iospi_out.ST.buff0.B.SU=0;
         	FPGA1_HW_Modify_Output.B0.PSU_LP=0;
        }          
 	    ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time     
    break;
        
    case 2:			//SDF
        if(OnOff)
        {
//-        	setoutp(o_SDF);
        	iospi_out.ST.buff0.B.SDF=1;  
        	FPGA1_HW_Modify_Output.B0.PSDF_LP=1;
        }
        else 
        {
//-        	clroutp(o_SDF);
        	iospi_out.ST.buff0.B.SDF=0;
        	FPGA1_HW_Modify_Output.B0.PSDF_LP=0;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time    
    break;
        
    case 3:			//SD
        if(OnOff)
        {
//-        	setoutp(o_SD);
        	iospi_out.ST.buff0.B.SD=1;
        	FPGA1_HW_Modify_Output.B0.PSD_LP=1;
        } 
        else 
        {
//-        	clroutp(o_SD);
        	iospi_out.ST.buff0.B.SD=0;
        	FPGA1_HW_Modify_Output.B0.PSD_LP=0;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time    
    break;        
        
    case 4:			//STE
        if(OnOff)
        {
//-        	setoutp(o_STE);
        	iospi_out.ST.buff0.B.STE=1; 
        	FPGA1_HW_Modify_Output.B0.PSTE_LP=1;        	
        }
        else
        {
//-        	clroutp(o_STE);
        	iospi_out.ST.buff0.B.STE=0;
        	FPGA1_HW_Modify_Output.B0.PSTE_LP=0;
        } 
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time            
    break;
        
    case 5:			//CEN(MC1)
        if(OnOff)
        {
//-        	setoutp(o_CEN);
        	iospi_out.ST.buff0.B.CEN=1; 
        	FPGA1_HW_Modify_Output.B0.PCEN_LP=1;
        }
        else 
        {
//-        	clroutp(o_CEN);
        	iospi_out.ST.buff0.B.CEN=0;
        	FPGA1_HW_Modify_Output.B0.PCEN_LP=0;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time    
    break;
        
    case 6:			//MC
        if(OnOff)
        {
//-        	setoutp(o_MCC);
        	iospi_out.ST.buff0.B.MC=1;
        	FPGA1_HW_Modify_Output.B0.PMC_LP=1;
        } 
        else 
        {
//-        	clroutp(o_MCC);
        	iospi_out.ST.buff0.B.MC=0;
        	FPGA1_HW_Modify_Output.B0.PMC_LP=0;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time    
    break;             
        
    case 7:			//RUN
        if(OnOff)
        {
//-        	setoutp(o_RUN);
        	iospi_out.ST.buff0.B.RUN=1; 
        	FPGA1_HW_Modify_Output.B0.PRUN_LP=1;
        }
        else 
        {
//-        	clroutp(o_RUN);
        	iospi_out.ST.buff0.B.RUN=0;
        	FPGA1_HW_Modify_Output.B0.PRUN_LP=0;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
    break;     
        
    case 8:			//UT
        if(OnOff)
        {
        	NTS_HW_Modify_Output.UT_NTS_out=1; 
        	FPGA1_HW_Modify_Output.B0.PUT_LP=0x01;
        }
        else 
        {
        	NTS_HW_Modify_Output.UT_NTS_out=0;
        	FPGA1_HW_Modify_Output.B0.PUT_LP=0x00;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
    break;  
           
    case 9:			//UN
        if(OnOff)
        {
        	NTS_HW_Modify_Output.UN_NTS_out=1; 
        	FPGA1_HW_Modify_Output.B0.PUN_LP=0x01;
        }
        else 
        {
        	NTS_HW_Modify_Output.UN_NTS_out=0;
        	FPGA1_HW_Modify_Output.B0.PUN_LP=0x00;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
    break;     
     
    case 10:		//DT
        if(OnOff)
        {
        	NTS_HW_Modify_Output.DT_NTS_out=1; 
        	FPGA1_HW_Modify_Output.B0.PDT_LP=0x01;
        }
        else 
        {
        	NTS_HW_Modify_Output.DT_NTS_out=0;
        	FPGA1_HW_Modify_Output.B0.PDT_LP=0x00;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
     break;  
           
     case 11:		//DN
        if(OnOff)
        {
        	NTS_HW_Modify_Output.DN_NTS_out=1; 
        	FPGA1_HW_Modify_Output.B0.PDN_LP=0x01;
        }
        else 
        {
        	NTS_HW_Modify_Output.DN_NTS_out=0;
        	FPGA1_HW_Modify_Output.B0.PDN_LP=0x00;
        }
        ReturnVal= DONE_BUT_CONTINUE_DISPLAY;  // Idle_time         
     break;           
              
     default:
     break;                         
  } 
  return ReturnVal;   
}
// 


int16 Set_FPGA_bits_B(int16 On_Off_which_input)
{
  int8  PLD2_bit = 0;
  int16 CPLD2_Set = 0;
  int16 On_or_Off = 0;
  int16 which_input = 0;
  
  if(On_Off_which_input == -1) return FINISHED_WITH_SERVICE ;
  On_or_Off = (On_Off_which_input & 0xff00) >> 8;
  if(On_or_Off)On_or_Off = 1; else On_or_Off = 0;
  which_input = On_Off_which_input & 0x00ff;
  Preliminary_120VAC(6); // Sets 120VAC to CST & maybe TSD
//  FPGA1_HW_Modify_Output.B0.PRUN_LP = 1;
//  RUN_output_cmnd_flag= 1;
    
  switch(which_input)
      { 
        case iDN: // +        
            FPGA1_HW_Modify_Output.B0.PDN_LP = On_or_Off;
            NTS_HW_Modify_Output.DN_NTS_out= On_or_Off;          
            break;
            
        case iDT: // +
            FPGA1_HW_Modify_Output.B0.PDT_LP = On_or_Off;
            NTS_HW_Modify_Output.DT_NTS_out=  On_or_Off;         
            break;
                        
        case iUN: // +
            FPGA1_HW_Modify_Output.B0.PUN_LP = On_or_Off;
            NTS_HW_Modify_Output.UN_NTS_out= On_or_Off;         
            break;
            
        case iUT: // +
            FPGA1_HW_Modify_Output.B0.PUT_LP = On_or_Off; 
            NTS_HW_Modify_Output.UT_NTS_out= On_or_Off;  
            if(On_or_Off == 1) setoutp(Virtual_Board_B_OUT[2]); else  clroutp (Virtual_Board_B_OUT[2]); // TSD now out3 (2), not (0)     
            break; 
            
        case iSUF: //
            FPGA1_HW_Modify_Output.B0.PSUF_LP = On_or_Off;  // SUF
            FPGA1_HW_Modify_Output.B0.PUT_LP = On_or_Off;  // UT
            NTS_HW_Modify_Output.UT_NTS_out= On_or_Off; 
            SUF_output_cmnd_flag = On_or_Off;  // SUF
            if(On_or_Off == 1) setoutp(Virtual_Board_B_OUT[2]); else  clroutp (Virtual_Board_B_OUT[2]); // TSD      
            break; 

        case iSU: //
            FPGA1_HW_Modify_Output.B0.PSU_LP = On_or_Off;
            FPGA1_HW_Modify_Output.B0.PUN_LP = On_or_Off;
            NTS_HW_Modify_Output.UN_NTS_out= On_or_Off;
            SU_output_cmnd_flag = On_or_Off;  // SU        
            break;
            
        case iSD: // + Plus DN 
        	SD_output_cmnd_flag = On_or_Off;
        	FPGA1_HW_Modify_Output.B0.PSD_LP=On_or_Off; 
            FPGA1_HW_Modify_Output.B0.PDN_LP = On_or_Off;
            NTS_HW_Modify_Output.DN_NTS_out= On_or_Off;        	       
            break;
            
        case iSDF: // + Plus DT
        	SDF_output_cmnd_flag = On_or_Off;  
        	FPGA1_HW_Modify_Output.B0.PSDF_LP=On_or_Off; 
            FPGA1_HW_Modify_Output.B0.PDT_LP = On_or_Off;
            NTS_HW_Modify_Output.DT_NTS_out=  On_or_Off;        	       
            break;            
            
        case iTSD:
            if(On_or_Off == 1) setoutp(Virtual_Board_B_OUT[2]); else  clroutp (Virtual_Board_B_OUT[2]); // TSD 
            break;            
                        
        case iCEN: // +
            FPGA1_HW_Modify_Output.B0.PCEN_LP = On_or_Off;
            CEN_output_cmnd_flag = On_or_Off;            
            break;
            
        case iL120:
            break;  
            
        case iGBP:
            if(On_or_Off == 1) 
            {
                setoutp(Virtual_Board_B_OUT[4]); // ICA
                Optional_Display = TRUE;
                New_write_display("GBP-BYPASS CAR ON   ",0,0);
                New_write_display("                    ",1,0);
                New_write_display("                    ",2,0);
                New_write_display("ENTER when ready    ",3,0);
            } 
            else  clroutp (Virtual_Board_B_OUT[4]);      
            break;    
            
        case iGLB:
            if(On_or_Off == 1) setoutp(Virtual_Board_B_OUT[4]); else  clroutp (Virtual_Board_B_OUT[4]); // ICA                   
            break;
            
        case iLBP:
            if(On_or_Off == 1)  
            {
                setoutp(Virtual_Board_B_OUT[4]); // ICA
                Optional_Display = TRUE;
                New_write_display("LBP-BYPASS          ",0,0);
                New_write_display("HOISTWAY ON         ",1,0);
                New_write_display("                    ",2,0);
                New_write_display("ENTER when ready    ",3,0);
            } 
            else  clroutp (Virtual_Board_B_OUT[4]);                 
            break;    
            
        case iIND:
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[4]); // ICA
                Optional_Display = TRUE;
                New_write_display("IND-ON              ",0,0);
                New_write_display("                    ",1,0);
                New_write_display("                    ",2,0);
                New_write_display("ENTER when ready    ",3,0);
            } 
            else  clroutp (Virtual_Board_B_OUT[4]);                   
            break;
            
        case iAD:
            if(On_or_Off == 1) setoutp(Virtual_Board_B_OUT[4]); else  clroutp (Virtual_Board_B_OUT[4]); // ICA 
                  
            break;    
            
        case iAUTO:
            if(On_or_Off == 1) setoutp(Virtual_Board_B_OUT[4]); else  clroutp (Virtual_Board_B_OUT[4]); // ICA       
            break;  

        case iMRIN:
            if(On_or_Off == 1)  
            {
                setoutp(Virtual_Board_B_OUT[4]); // ICA
                Optional_Display = TRUE;
                New_write_display("MRI-INSP ON         ",0,0);
                New_write_display("                    ",1,0);
                New_write_display("                    ",2,0);
                New_write_display("ENTER when ready    ",3,0);
            } 
            else  clroutp (Virtual_Board_B_OUT[4]);                 
            break;
                        
        case iMRID:
            if(On_or_Off == 1) 
            {
                setoutp(Virtual_Board_B_OUT[4]); // ICA
                Optional_Display = TRUE;
                New_write_display("MRID-INSP ON        ",0,0);
                New_write_display("PRESS & HLD ENBL PB ",1,0);
                New_write_display("AND DOWN PB         ",2,0);
                New_write_display("ENTER when ready    ",3,0);
            } 
            else  clroutp (Virtual_Board_B_OUT[4]);                    
            break;   
            
        case iMRIE:
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[4]); // ICA
                Optional_Display = TRUE;
                New_write_display("MRIE-INSP ON        ",0,0);
                New_write_display("PRESS & HOLD        ",1,0);
                New_write_display("ENABLE PB           ",2,0);
                New_write_display("ENTER when ready    ",3,0);
            } 
            else  clroutp (Virtual_Board_B_OUT[4]);                    
            break;
            
        case iMRIU:
            if(On_or_Off == 1) 
            {
                setoutp(Virtual_Board_B_OUT[4]); // ICA
                Optional_Display = TRUE;
                New_write_display("MRIU-INSP ON        ",0,0);
                New_write_display("PRESS & HLD ENBL PB ",1,0);
                New_write_display("AND UP PB           ",2,0);
                New_write_display("ENTER when ready    ",3,0);
            } 
            else  clroutp (Virtual_Board_B_OUT[4]);                     
            break;                                                                                    
                        
        case jSTE_CPU: 
        	STE_output_cmnd_flag = On_or_Off; 
        	FPGA1_HW_Modify_Output.B0.PSTE_LP=On_or_Off;        
            break;                               
                           
        case iMC: 
            FPGA1_HW_Modify_Output.B0.PMC_LP = On_or_Off;
            MC_output_cmnd_flag = On_or_Off;        
            break;
            
        case kDLT: // seb board 7
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_C_OUT[6]); // HOISTWAY DOOR LOCK; DLT
            } 
            else  clroutp (Virtual_Board_C_OUT[6]);        
            break; 
            
        case kRLM: // seb board 7
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_C_OUT[7]); // HOISTWAY DOOR LOCK; RLM
            } 
            else  clroutp (Virtual_Board_C_OUT[7]);        
            break;    
            
        case kDLM: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[5]); // HOISTWAY DOOR LOCK; DLM
            } 
            else  clroutp (Virtual_Board_B_OUT[5]);        
            break;               
            
        case kDLB: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[6]); // HOISTWAY DOOR LOCK; DLB
// tagZYM 111  
// tagZYM 111B              
                // Exclude_Short_Signal(DLB_33);
                // Exclude_Short_Signal(RUN_CPU_66);
                // Exclude_Short_Signal(DN_NTS_80);
                // Exclude_Short_Signal(FPGA1 #3 #12);
                // Exclude_Short_Signal(FPGA1 #3 #13);
            } 
            else  clroutp (Virtual_Board_B_OUT[6]);        
            break;    
            
        case kLPS: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[5]); // OIL SENSING; LPS
            } 
            else  clroutp (Virtual_Board_B_OUT[5]);        
            break;
            
        case kLOS: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[6]); // OIL SENSING; LOS
            } 
            else  clroutp (Virtual_Board_B_OUT[6]);        
            break;
            
        case kTPL: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[0]); // OIL SENSING; TPL
            } 
            else  clroutp (Virtual_Board_B_OUT[0]);        
            break; 
                                                                                                              
        case kTPH: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[1]); // OIL SENSING; TPH
            } 
            else  clroutp (Virtual_Board_B_OUT[1]);        
            break;
            
        case kTLH: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[8]); // HATCH SAFETIES; TLH
            } 
            else  clroutp (Virtual_Board_B_OUT[8]);        
            break;
                        
        case kGOV: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[7]); // HATCH SAFETIES; GOV -H22
            } 
            else  clroutp (Virtual_Board_B_OUT[7]);        
            break;   
                        
        case kPS: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[9]); // HATCH SAFETIES; PS -H19
            } 
            else  clroutp (Virtual_Board_B_OUT[9]);        
            break;   
                        
        case kHSS: 
            if(On_or_Off == 1)
            {
                setoutp(Virtual_Board_B_OUT[10]); // HATCH SAFETIES; PS -H19
            } 
            else  clroutp (Virtual_Board_B_OUT[10]);        
            break;                                     
            
        case kS10_H24:   // HATCH SAFETIES;     
            break;
                                                 
        case iMRSW:
            break;
                                                 
        case iBAU:
            if(On_or_Off == 1)
            {  //  Change the board and channel after iodat file is updated wbg 2018/1/5
                setoutp(Virtual_Board_C_OUT[0]); // HATCH SAFETIES; GOV -H22
            } 
            else  clroutp (Virtual_Board_C_OUT[0]);        
            break;  
                                                 
        case iBAD:
            if(On_or_Off == 1)
            {  //  Change the board and channel after iodat file is updated wbg 2018/1/5
                setoutp(Virtual_Board_C_OUT[1]); // HATCH SAFETIES; GOV -H22
            } 
            else  clroutp (Virtual_Board_C_OUT[1]);        
            break; 
                                                 
        case iTAU:
            if(On_or_Off == 1)
            {  //  Change the board and channel after iodat file is updated wbg 2018/1/5
                setoutp(Virtual_Board_B_OUT[11]); // HATCH SAFETIES; GOV -H22
            } 
            else  clroutp (Virtual_Board_B_OUT[11]);        
            break; 
                                                 
        case iTAD:
            if(On_or_Off == 1)
            {  //  Change the board and channel after iodat file is updated wbg 2018/1/5
                setoutp(Virtual_Board_C_OUT[3]); // HATCH SAFETIES; GOV -H22
            } 
            else  clroutp (Virtual_Board_C_OUT[3]);        
            break;
                                                 
/*        case iCST:
            if(On_or_Off == 1)
            {  //  Change the board and channel after iodat file is updated wbg 2018/1/5
                setoutp(o_4[11]); //
            } 
            else  clroutp (o_4[11]);        
            break;  */        
                                                  
        case iCFLT: // seb board 7
            if(On_or_Off == 1)
            {  //  Change the board and channel after iodat file is updated wbg 2018/1/5
                setoutp(Virtual_Board_C_OUT[8]); //
            } 
            else  clroutp (Virtual_Board_C_OUT[8]);        
            break; 
                                                 
        case iSPD: // seb board 7
            if(On_or_Off == 1)
            {  //  Change the board and channel after iodat file is updated wbg 2018/1/5
                setoutp(Virtual_Board_C_OUT[9]); //
            } 
            else  clroutp (Virtual_Board_C_OUT[9]);        
            break;           
                                                 
        case iMTO: // seb board 7
            if(On_or_Off == 1)
            {  //  Change the board and channel after iodat file is updated wbg 2018/1/5
                setoutp(Virtual_Board_C_OUT[10]); //
            } 
            else  clroutp (Virtual_Board_C_OUT[10]);        
            break;                          

    //  WBG step 03' 

        case iDZ:
            break;
                                          
        default:
            break;                  
    }
    return FINISHED_WITH_SERVICE;
}

//  Forced_outs_on_CPLD1
int16 Force_CPLD1(int16 TheOutputs)
{ // Access via menu 11
//    Clear_Msg_Board();
//    All_CPLD2_outputs_ON(0);
    FPGA1_HW_Modify_Output.mc_drop_delay = TheOutputs;
//    sprintf(src, "0x%04X", FPGA1_HW_Modify_Output.mc_drop_delay);
//    Log_an_event(src);
//    Forced_outs_on_CPLD1 = TheOutputs;
    return FINISHED_WITH_SERVICE;
}

int16 TSD_ctrl (int16 Channel)
{
   CST_ctrl(0);
   if(Channel < 5)setoutp(Virtual_Board_B_OUT[2]);// TSD IS NOW o_4[2]. No longer o_4[0] WBG 2017/12/21
     else clroutp(Virtual_Board_B_OUT[2]);
   return FINISHED_WITH_SERVICE;
}

int16 CST_ctrl (int16 dummy)
{
   int16 x = dummy;
   
   Set_a_BITx(0);  // PRUN_LP
   RUN_output_cmnd_flag = 1; // RUN_CPU
   setoutp(Virtual_Board_B_OUT[3]); // CST IS NOW o_4[3]. No longer o_4[1] WBG 2017/12/21

   return FINISHED_WITH_SERVICE;
}

int16 Set_a_BITx(int16 BitNum)
{
  if(UseFPGA1_B0 == 0)FPGA1_HW_Modify_Output.mc_drop_delay = 0;
  switch(BitNum)
  {
    case 0: // BIT 0 ; RUN
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PRUN_LP = 1; 
//        else FPGA1_HW_Modify_Output.B1.BIT_0 = 1;
          FPGA1_HW_Modify_Output.B0.PRUN_LP = 1;
        break;
    case 1: // BIT 1 ; CEN 
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PCEN_LP = 1; 
//        else         FPGA1_HW_Modify_Output.B1.BIT_1 = 1;
          FPGA1_HW_Modify_Output.B0.PCEN_LP = 1;
        break;
    case 2: // BIT 2 ; STE 
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PSTE_LP = 1; 
//        else         FPGA1_HW_Modify_Output.B1.BIT_2 = 1;
          FPGA1_HW_Modify_Output.B0.PSTE_LP = 1;
        break;   
    case 3: // BIT 3 ; MC
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PMC_LP = 1; 
//        else
//        FPGA1_HW_Modify_Output.B1.BIT_3 = 1;
        FPGA1_HW_Modify_Output.B0.PMC_LP = 1;
        break;
    case 4: // BIT 4 ; NOTHING 
//        if(UseFPGA1_B0)  FPGA1_HW_Modify_Output.B0.PRUN_LP = 1; 
//        else
//        FPGA1_HW_Modify_Output.B1.BIT_4 = 1;
        break;
    case 5: // BIT 5 ; NOTHING 
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PRUN_LP = 1; 
//        else
//        FPGA1_HW_Modify_Output.B1.BIT_5 = 1;
        break;
    case 6: //  BIT 66;NOTHING
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PRUN_LP = 1; 
//        else      
//        FPGA1_HW_Modify_Output.B1.BIT_6 = 1;
        break;
    case 7: //  BIT 7 ;NOTHING 
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PRUN_LP = 1; 
//        else     
//        FPGA1_HW_Modify_Output.B1.BIT_7 = 1;
        break;
    case 8: //  BIT 8 ;SU
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PSU_LP = 1; 
//        else      
//        FPGA1_HW_Modify_Output.B1.BIT_8 = 1;
        FPGA1_HW_Modify_Output.B0.PSU_LP = 1;
        break;
    case 9: //  BIT 9 ;SD 
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PSD_LP = 1; 
//        else     
//        FPGA1_HW_Modify_Output.B1.BIT_9 = 1;
        FPGA1_HW_Modify_Output.B0.PSD_LP = 1;
        break;
    case 10: //  BIT 10; SUF // 
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PSUF_LP = 1; 
//        else    
//        FPGA1_HW_Modify_Output.B1.BIT_10= 1;
        FPGA1_HW_Modify_Output.B0.PSUF_LP = 1;
        break;
    case 11: // BIT 11; SDF 
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PSDF_LP = 1; 
//        else     
//        FPGA1_HW_Modify_Output.B1.BIT_11= 1;
        FPGA1_HW_Modify_Output.B0.PSDF_LP = 1; 
        break;  
    case 12: //  BIT 12; UN 
        NTS_HW_Modify_Output.UN_NTS_out=1;
        FPGA1_HW_Modify_Output.B0.PUN_LP = 1;
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PUN_LP = 1; 
//        else     
//        FPGA1_HW_Modify_Output.B1.BIT_12= 1;
        break;
    case 13: // BIT 13; DN 
        NTS_HW_Modify_Output.DN_NTS_out=1;
        FPGA1_HW_Modify_Output.B0.PDN_LP = 1;
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PDN_LP = 1; 
//        else     
//        FPGA1_HW_Modify_Output.B1.BIT_13= 1;
        break;
    case 14: // BIT 14; UT 
        NTS_HW_Modify_Output.UT_NTS_out=1; 
        FPGA1_HW_Modify_Output.B0.PUT_LP = 1;
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PUT_LP = 1; 
//        else     
//        FPGA1_HW_Modify_Output.B1.BIT_14= 1;
        break;
    case 15: // BIT 15; DT 
        NTS_HW_Modify_Output.DT_NTS_out=1;
        FPGA1_HW_Modify_Output.B0.PDT_LP = 1; 
//        if(UseFPGA1_B0) FPGA1_HW_Modify_Output.B0.PDT_LP = 1; 
//        else     
//        FPGA1_HW_Modify_Output.B1.BIT_15= 1;
        break;
    default:
        break;
  }
//  sprintf(&LCD_Display[1][11],"0x%X",FPGA1_HW_Modify_Output.mc_drop_delay);
  return DONE_BUT_CONTINUE_DISPLAY;
}












