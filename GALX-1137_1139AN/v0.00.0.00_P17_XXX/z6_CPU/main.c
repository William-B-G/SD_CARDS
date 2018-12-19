#include "global.h"

//#include "GALX_1132_TEST.h"
//#include "GALX_1121_TEST.h"
//#include "GALX_1137_1139.h"

#include "Tests.h"

//ZYM 12-1-2017
#include "cedes_can.h"

//ZYM 3-17-2017
void init_cedes_pit (void);

int wbg_02;  // for debug only
int wbg_04;  // for debug only
int16 wbg_01;  // for debug only
int wbg_03;  // for debug only   
int8 wbg_start = 0;  // 1 = fall through to main menu (when MODE_Button)
int16 I_WD_index = 10;
int16 J_WD_index = 0;
char Version_Display[21];

char *x5_label = "Hello_World"; // string constant

int16 rsCOM1_Reply = 0;
int16 rsCOM2_Reply = 0;
char src[50];
char src_B[25];
char src_C[25];
int8 msgSent[40];  // BOARD A
int8 msgSent_C[40];  // BOARD C
unsigned char RS_485_RxBuf[20];
uint8 Group_CAN_Test_OK = 0;
uint8 Mr_CAN_Test_OK = 0;
uint8 Car_CAN_Test_OK = 0;
uint8 Enable_CAN_Ctrl = 1; // disable while doing CAN test (wbg)
uint8 Enable_CT_CAN_Ctrl = 1; // disable while doing Car Top CAN test (wbg)
uint8 Enable_MR_CAN_Ctrl = 1; // disable while doing Machine Room CAN test (wbg)
uint8 Enable_gr_CAN_Ctrl = 1; // disable while doing group CAN test (wbg)
uint8 Enable_RS232_Ctrl = 1; // disable while doing RS232 test (wbg)
uint8 Enable_RS485_Ctrl = 1; // disable while doing RS485 test (wbg)
uint8 Enable_Hallcom_Ctrl = 1; // disable while doing RS485 test (wbg)
uint8 Enable_iospi_Ctrl = 1;   // for FPGA com. Disable on start.
uint8 Initial_Pwr_Up_Complete = 0;
uint8 CurrentTestPassFail = 1;  // 0 = Fail; 1 = Pass
uint8 Current_113x_TestPassFail = 1;  // 0 = Fail; 1 = Pass
uint8 Current_1139_TestPassFail = 1;  // 0 = Fail; 1 = Pass
uint8 First_Half_PassFail = 1;
uint8 Second_Half_PassFail = 1;
uint8 Chan_1_1137 = 0;
uint8 Chan_2_1137 = 0;
uint8 ForceTestToIndicateFail = 0;
unsigned char tempBuff[30] = {0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0};
int16 WhichBuffer = 0;
int16 RS485_Baud = 354;  // Default to 19.2kb for RS485. Must be 230400 for WiFi
int16 ver_rev = 0;
int16 ioUnpkOK = 0;
int16 i1UnpkOK = 0;
int16 ntsUnpkOK = 0;  // gets set to 1 for any valid msg unpacked (wbg).
int16 Display_ISR_timing = 0;  // if 1 then send the ISR timing to msgboard A.
int16 Enable_Display_ISR_timing = 0; // set/reset from a menu item (wbg).
int32 min_ISR_timer = 0;
int16 Functional_Test_is_Running = FALSE;
int16 WhichMenuToStart = 0;
int16 MenuPtrArrayIndex = 0;
uint8 cpld1_B_mlb = 0x93; // address for CPLD1 (safety PAL)
uint8 cpld1_B_lb  = 0x55;
uint8 cpld2_B_mlb = 0x61; // address for CPLD2 (read IO PAL)
uint8 cpld2_B_lb  = 0x2a;
uint8 Hall_Lantern_Select = 1; // default to floor 1
uint8 Hall_Lantern_floor_A = 1;
uint8 Hall_Lantern_floor_B = 2;
uint8 Hall_Lantern_Data_A = 0;
uint8 Hall_Lantern_Data_B = 0xff;

void wbg_debug_clear_LCD(int8 rows);
void WBG_Debug_code(void);
void wbg_debug_msg(char *the_msg_1, int num_to_display, 
    char *the_msg_2, int index_to_display);
void InitComms(void); 
extern void Fill_up_msg_array(void);



/*******************************************************************************
* External objects
*******************************************************************************/    

/* Prototype for PRC1 startup */
extern void __start_p1();
extern void control(void); 
extern void init_control(void);
extern void inttim(void);
extern void int_tsec(void);
extern void InitGPIO(void);
extern void InitGPIO_p1(void);
extern void MenuManager(void);
extern void RSCOM1_sendc(int16 letter);
extern void sendstring(int16 comnmb, char *c);

extern short int fvars[];
#define fvcputiming 434			// Cpu timing output control
 
 /*******************************************************************************
* Constants and macros
*******************************************************************************/

#define  FLASH_REG FLASH.PFCRP0.R
//#define FLASH_DATA  0x0800EF00
//#define FLASH_DATA  0x04006B00
#define FLASH_DATA  0x00016B00
#define FLASH_REG2 FLASH.PFCRP1.R
//#define  FLASH_DATA2 0x30002900
#define FLASH_DATA2 0x30006B00

/*******************************************************************************
* VERSION
*******************************************************************************/
const int8 version =  0;  // tagwbg 11 - version Now
const int8 revision = 0;
const int8 interim_revision = 0;  // interim revision valid 0 - 255
const int8 Sandbox_revision = 'X' ;  // Normally, set this = '0'. 
  // Then increment as 'A', 'B', etc    

//char *VerDate = "2018-05-06 @15:45";  //  P12
//char *VerDate = "2018-05-07 @06:30";  //  P13
//char *VerDate = "2018-05-07 @11:30";  //  P14
//char *VerDate = "2018-05-07 @16:00";  //  P15
//char *VerDate = "2018-05-09 @11:00";  //  P16
//char *VerDate = "2018-05-10 @07:40";  //  P17
char *VerDate = "2018-05-14 @07:49";  //  X00 - Experimental, generic version for any test.

/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/
static void InitHW(void);
static void InitSysclk(void);
static void FlashConfig(void);
static void Delay(void);
static void Delay_1s(void);

/*******************************************************************************
* Local variables
*******************************************************************************/ 

/*******************************************************************************
* Local functions
*******************************************************************************/
          
/*******************************************************************************
Function Name : InitHW
Engineer      : b08110
Date          : Feb-18-2010
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : initialization of the hw for the purposes of this example
Issues        : NONE
*******************************************************************************/
static void InitHW(void)
{
    disable_dog();
    InitSysclk();
    FlashConfig();
    InitGPIO();    
	InitGPIO_p1();	
}

/*******************************************************************************
Function Name : InitSysclk
Engineer      : b08110
Date          : Feb-18-2010
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : initialize Fsys 108.75 MHz PLL with 40 MHz crystal reference
                monitor PK9 (connector P30 pin 10 on MPC5668EVB) for Fsys/2
Issues        : NONE
*******************************************************************************/
static void InitSysclk(void)
{
    /* External clock enable. ECEN = 1 */
    /* External clock divide by 2. ECDF = 0b01 */
    SIU.ECCR.R = 0x00000009; 
    
    /* configure PK9 for CLKOUT */ 
    /* PA = 0b01 */
    /* OBE = 1 output */
    /* IBE = 0 */
    /* ODE = 0 */
    /* HYS = 0 */
    /* SRC = 0b11 max.slew rate */
    /* WPE = 0 */
    /* WPS = 0 */    
//    SIU.PCR[153].R = 0x060C;
 
 #if 1 
    /* EN40MOSC is enabled out of reset by default */  
    /* enable external oscillator */ 
//    CRP.CLKSRC.B.EN40MOSC = 0b1;      
    CRP.CLKSRC.B.EN40MOSC = 0x0001;      
 #endif
    
    /*
    1. determine the approptiate value EPREDIV, EMFD, ERFD. 
    for Fsys = 108.75 MHz with a 40 MHz crystal reference, we will get the final
    values: 

    EPREDIV = 7 (divide by 8) 
    EMFD = 71 (divide by 87) 
    ERFD = 3 (divide by 4) in this case, 
    Fvco will be 435 MHz. 
    */
         
    /* 2. write ERFD = 5 (divide by 6). disable FM (EDEPTH = 0) */ 
    FMPLL.ESYNCR2.R = 5; 

    /* 3. EPREDIV = 7 (div by 8), EMFD = 71 (div by 87) */ 
//    FMPLL.ESYNCR1.R = 0xF0090054; // EPREDIV = 9 (div by 10), EMFD = 84 (div by 100) -- for 100Mhz
    FMPLL.ESYNCR1.R = 0xF0070047; // EPREDIV = 7 (div by 8), EMFD = 71 (div by 87)
 
    /* 4. wait for LOCK */ 
    while (FMPLL.SYNSR.B.LOCK != 1) {} 
 
    /* 5. write final ERFD = 3 (divide by 4). disable FM (EDEPTH = 0) */ 
    FMPLL.ESYNCR2.R = 3; 
 
    /* 6. select PLL as the Fsys */ 
    SIU.SYSCLK.B.SYSCLKSEL = 2;         
}

/*******************************************************************************
Function Name : FlashConfig
Engineer      : b08110
Date          : Mar-04-2010
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : PFCRP0 and PFCRP1 adjusted for actual Fsys
Issues        : expected Fsys = 100 MHz, IOP operates 50 MHz
*******************************************************************************/
static void FlashConfig(void) 
{
    unsigned int mem_write_code [] = 
    {

    #if __option(vle)
        /*for processors which support VLE only or for 'VLE on' option*/
        0xD0344400, /* stw r3,(0)r4 machine code: writes r3 contents to addr in r4 then se_nop*/
        0x7C0006AC, /* mbar machine code: ensure prior store completed */
        0x44000004  /* blr machine code: branches to return address in link register */
    #else
        0x90640000, /* stw r3,(0)r4 machine code: writes r3 contents to addr in r4 */
        0x7C0006AC, /* mbar machine code: ensure prior store completed */
        0x4E800020  /* blr machine code: branches to return address in link register */
    #endif // __option(vle)

    };	

typedef void (*mem_write_code_ptr_t)(unsigned int, unsigned int);
 
    (*((mem_write_code_ptr_t)mem_write_code)) 	/* cast mem_write_code as func ptr*/
                                 /* * de-references func ptr, i.e. converts to func*/
        (FLASH_DATA,            /* which passes integer (in r3) */
        (unsigned int)&FLASH_REG);
      
//#if defined(FLASH_REG2)
    (*((mem_write_code_ptr_t)mem_write_code)) 	/* cast mem_write_code as func ptr*/
                                 /* * de-references func ptr, i.e. converts to func*/
        (FLASH_DATA2,            /* which passes integer (in r3) */
        (unsigned int)&FLASH_REG2);
//#endif 
 
}

static void Unexpected_Interrupt (void)
{
   while (1) {}; 	 /* Wait forever or for watchdog timeout */
}

/*******************************************************************************
Function Name : InitPIT
Engineer      : b08110
Date          : Feb-18-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : - 1 second periodic interrup using the PIT3 module
Issues        : - expecting 108.75 MHz fsys for the PIT timer
*******************************************************************************/
static void InitPIT(void)
{
	/* 30: MDIS = 0 to enable clock for PITs. */
    /* 31: FRZ = 1 for Timers stopped in debug mode */
    PIT.MCR.R = 0x00000001;    

	// 10 msec interrupt timer - PIT 1
//	PIT.LDVAL1.R = 1000000 - 1;		// setup timer 1 for 10 msec with 100 MHz fsys
	PIT.LDVAL1.R = 1087500 - 1;		// setup timer 1 for 10 msec with 108.75 MHz fsys
	PIT.TFLG1.B.TIF = 1;			// Clear interrupt flag so an interrupt does not occur right away
	PIT.TCTRL1.B.TIE = 1;		// Enable interrupts
	PIT.TCTRL1.B.TEN = 1;		// Start Timer
                
    // 1 second timer - PIT 3
//    PIT.LDVAL3.R = 100000000 - 1;
    PIT.LDVAL3.R = 108750000 - 1;
    	
    /* clear the TIF flag */
    PIT.TFLG3.R = 0x00000001;
    	
    /* 30: TIE = 1 for interrupt request enabled */
    /* 31: TEN = 1 for timer active */
    PIT.TCTRL3.R = 0x00000003;
    
    
//ZYM 3-17-2017 work as cedes head
//-	init_cedes_pit();
}


//ZYM 3-17-2017
//*****************************************
// Init CEDES Communications Timer
//*****************************************
void init_cedes_pit (void)
{
    PIT.TCTRL6.B.TEN = 0;       // Stop Timer
    // Initialize timer
    PIT.LDVAL6.R = 271875 - 1;      // setup timer 6 for 2.5 msec with 108.75 MHz fsys
    //PIT.LDVAL6.R = 10875000 - 1;      // setup timer 6 for 100ms msec with 108.75 MHz fsys
    PIT.TFLG6.B.TIF = 1;            // Clear interrupt flag so an interrupt does not occur right away
    PIT.TCTRL6.B.TIE = 1;           // Enable interrupts
    PIT.TCTRL6.B.TEN = 1;           // Start Timer

}



void wbg_debug_clear_LCD(int8 rows)
{
  int8 ijk = 0;
  int8 j = 1;
    for (ijk = 0; ijk < 4; ijk++)
    {
		if (rows & j)
		{ // clear row
			delay(10000);  // delay 1.8 ms to allow LCD to reset    	    
			write_display("                    ", ijk,0);		
		} 
		j *= 2;   	
    }		
}

void wbg_debug_msg(char *the_msg_1, int num_to_display, 
    char *the_msg_2, int index_to_display)
{
    char My_Display[21];
    char My_Buff[21];
    int16 i;
	int16 j = 0;    

		for (i=0;i<=13;i++)
		{
			if (the_msg_1[j] == 0)
				break;
			else
				My_Buff[i] = the_msg_1[j++];
		}
		j = 0;
		My_Buff[i++] = '=';
		My_Buff[i++] = ' ';
		My_Buff[i++] = '%';
		My_Buff[i++] = '3';
		My_Buff[i++] = 'd';
		
    sprintf(My_Display, My_Buff , num_to_display );
	delay(10000);  // delay 1.8 ms to allow LCD to reset    	    
  	write_display(My_Display, 2,0);

		for (i=0;i<=13;i++)
		{
			if (the_msg_1[j] == 0)
				break;
			else
				My_Buff[i] = the_msg_2[j++];
		}
		My_Buff[i++] = '=';
		My_Buff[i++] = ' ';
		My_Buff[i++] = '%';
		My_Buff[i++] = '3';
		My_Buff[i++] = 'd';

    sprintf(My_Display, My_Buff , index_to_display );
	delay(10000);  // delay 1.8 ms to allow LCD to reset    	    
  	write_display(My_Display, 3,0);				
}

void WBG_Debug_code(void)
{
 if ( timers[tinc] == 9) // timers[tinc] incremented every 10ms,              
          { //  so this will be 100ms. 
            //  THIS 'else if' IS DEBUG CODE. REMOVE AFTER DEBUGGING 
            wbg_01++;  
            wbg_03 =  UpButton + (DownButton <<1) 
                + (ModeButton <<2) + (EnterButton <<3);  
                 	            
            wbg_debug_msg("4 Ln", wbg_03, "DSPL", wbg_01);
            if( (wbg_01 % 10) == 0)
            {   // check buttons every 100ms         	            
	            if(UpButton == ButtonPress) 
	            { // turn on LEDs 0 & 6
	              UpButton = ButtonPressAck;
	              SIU.GPDO[Z0_LED].R = 0;	// turn led on solid
	             // SIU.GPDO[Z6_LED].R = 0;	// turn led on solid	
	            }	            
	            if(DownButton == ButtonPress)
	            {  // turn off LEDs 0 & 6
	              DownButton = ButtonPressAck;
	              SIU.GPDO[Z0_LED].R = 1;	// turn led off solid
//-	              SIU.GPDO[Z6_LED].R = 1;	// turn led off solid	              	
	            }
	            if(EnterButton == ButtonPress) EnterButton = ButtonPressAck;
	            if(ModeButton == ButtonPress) 
	            {
	              ModeButton = ButtonPressAck;	            
	              wbg_start = 1; // set flag to go to main menu next interation
	            }	            	            
            } // END OF "if( (wbg_01 % 10) == 0)"
           
            if (wbg_01 >= 9999) 
            {
	              wbg_start = 1; // set flag to go to main menu next interation
	                // after counter reaches 9999            
/*            
	          wbg_01 = 0; // counts to 500 in 50 sec. [count is 10*500 because the count
	            // is only incremented when 'timers[tinc] == 9' ]  
	              // Therefore, ISR is 50s/(10*500) = 10ms per Timer_tic (in ISR).
	              //  So the ISR is 10ms.          
*/	                  
            }
          } // END OF "else if ( timers[tinc] == 9)"	
}


/*******************************************************************************
* Main call board tester for Z6 CPU on GALx1132BT
*******************************************************************************/
int32_t main(void) 
{
    uint32_t i =  0;
    int16 comnmb = 2;

    InitHW();
    //InitComms(); // Remove. See email from Mark (2017-9-22)
    // Also, this (InitComms()) gets done below in 'init_control();'
    // Receive chars are done via DMA.
        
    // Set all interrupts to the Unexpected interrupt vector    
    for(i=0;i<316;i++)
    {
	    INTC_InstallINTCInterruptHandler(Unexpected_Interrupt, (uint16)i, 10);
    }
        
    /* Install PIT3_isr into the interrupt vector table */
    INTC_InstallINTCInterruptHandler(int_tsec, 151, 1);
      // 1 s interrupt tagwbg 08

  	/* Install our handlers */  
  	INTC_InstallINTCInterruptHandler(inttim, 149, 1); 
  	  // 1/100 s interrupt tagwbg 07
    
// ZYM 12-1-2017 work as cedes head      
//-  	INTC_InstallINTCInterruptHandler(Ced_Pack_Can_Interrupt, 154, 1); 
  	
  	
    /* lower current INTC priority to start INTC interrupts */
    INTC.CPR_PRC0.R = 0;
    
    /* Initilize pit timers */
    InitPIT();
    
    // sytem timer
	STM._CR.B.CPS = 0;		// divide by 1
	// Set DMA priority operation for all DMA devices
	STM._CR.B.TEN = 0;		// disable for now
	STM.CNT.R = 0;
	
  	EDMA.CRR.R = 0x0000000C; 	// Use round robin priority arbitration for DMA groups and channels  
	init_control();
	SIU.GPDO[Z0_LED].R = 1;	// turn led OFF (wbg)// tagwbg 14
 	SIU.GPDO[Z6_LED].R = 1;	// turn led OFF (wbg)// tagwbg 14	
  
  	/* Start the other core by writing CRP.Z0VEC.R */ 
  	CRP.Z0VEC.R = (unsigned long)__start_p1; 
  	 	    
     wbg_01 = 0; 
     wbg_02 = 0;
     Clear_Msg_Board();
     Clear_Msg_Board_B();
     Clear_Msg_Board_C();
     Log_a_fault(src, 1); // erases fault log

     SW_Vers_disp();
           
    petdog();
    SetupRTC(); 
    MenuPtrArrayIndex = 0;  

    //  consB[] is set up in rdconsB_std() which is in utils.c
    if(consB[0] < Get_Num_of_Menus() ) Start_a_Menu(consB[0]);
       else Start_a_Menu(0);
   	// sets the start menu from the contents of consB.dat file
   	if(consB[27] & 0x100) Use_CT_CAN(0);
    
   	
// 	Enable_RS485_Ctrl = DISABLED;  // disable RS-485 until ready to test RS485
// 	Enable_Hallcom_Ctrl = DISABLED; // disable RS-485 until ready to test RS485
    Set_RS485_Baud(NINETEEN_DOT_2);
 	min_ISR_timer = 1087500 - 1;
 	Enable_gr_CAN_Ctrl = 0; // disable group CAN for 1137 / 1139 test
 	
  	for (;;) {   /* Loop forever */ 
  	    	
          petdog(); 
          
          if((Timer_tic > 1) || (PIT.CVAL1.R < 500000) || Display_ISR_timing) 
          {
              sprintf(src,"timerTic %d  %d", PIT.CVAL1.R, Timer_tic );
              if(Enable_Display_ISR_timing) Log_an_event(src);
//            setgoutp(GRtest_osignal_array[7]); 
          }
          while (Timer_tic == 0)
          {
             petdog(); 	
      		 if(Enable_CAN_Ctrl) comm_control(); //normally called, but suspended while
                 // doing CAN test so that CAN test can control CAN traffic.    	                     
             if(Enable_iospi_Ctrl) iospi_control();	
          }
          // fall through after timer ISR (10ms) (wbg)          
          Timer_tic = 0;  // is set in the timer ISR (wbg)     
///////            
                       
          new_inctime(); // increment all SW timers.
                      		  
          if(timers[tinc] & 0x01) // 20ms
          {	// enter here every 10 ms for "if(1)", or 20ms with "if(timers[tinc] & 0x01)"    
          	                       
         	  DebouncePushButtons(); // tagwbg 02          
              MenuManager(); // the init_control() calls init_LCD() which 
          	    // sets LCD_Upd_Dpy = 1 so that the first time
          	    // into MenuManager() the display will be updated. 
          	    // THE MenuManager() RUNS THE TESTS THAT ARE SPECIFIED IN THE MENU SELECTION.        	  
              LCD_Buffer_to_spi(); // write the buffers to the display via SPI 
                                                    		                    	;
          }      	  	        	       			    	    		    	
  	} // END OF "for (;;)" 
} // END OF "main()"


void InitComms(void)
{
 	petdog();
 	init_mrcan_bus();
 	petdog();
 	init_ctcan_bus();
 	petdog();
	init_grcan_bus();
 	petdog();	
	initcom();	// must be after init_can_bus() 
	  // since the can bus chip provides the clock
  	petdog();    
}

void SetupRTC(void)
{
    d.year   = consB[15];
    d.month  = consB[14];
    d.day    = consB[13];
    t.hour   = consB[12];
    t.minute = consB[11];
    local_setdate();
    local_settime();    
}

int16 ViewRTC(int16)
{
  int16 i = 0; 
  int16 ReturnVal= 0;
  static int16 BuffRead_operation = Init_BuffRead;
  static int16 Delay_Count = 0;
  static int16 Kount = 0;

    switch (BuffRead_operation) {
    case Init_BuffRead:
        clr_lcd_dply(); // clear 4-line display
        timers[t_Generic_Delay] = 0;
        Delay_Count = 30; // 3s
        local_gettime();
        local_getdate();  // New_write_display("                    ",1,0);
        sprintf(src,"     %d/%d/%d", d.year, d.month, d.day );              
          New_write_display(src,0,0); // date
        sprintf(src,"     %d:%2.2d", t.hour, t.minute );              
          New_write_display(src,1,0); // time  
        if(t.hour > 12)
        {
        sprintf(src,"     %d:%2.2d PM", t.hour - 12, t.minute );              
          New_write_display(src,2,0); // time              
        }
        BuffRead_operation = Display_Buff;
        break;

    case Display_Buff:
        if(Check_for_Mode_Button()) {
            BuffRead_operation = Init_BuffRead;
            return FINISHED_WITH_SERVICE;
        }
        if(timers[t_Generic_Delay] >= Delay_Count ) { // each delay is 100ms
            timers[t_Generic_Delay] = 0; //
            if(Kount++ > 19)
            {
                Kount = 0;
                New_write_display(".                   ",3,0);                
                BuffRead_operation = Init_BuffRead;
            }
            else LCD_Display[3][Kount] = '.'; // 60s Progress bar
        }        
        break;  

    default:
        BuffRead_operation = Init_BuffRead;
        return DEFAULT;
        break;
    }
    return CONTINUE_SERVICE;    
}

void SW_Vers_disp(void)
{
     // init the SW Version display line
//    New_write_display("                    ",0,0);
    Fill_Entire_Line_One_Char(" ", 0);
    if(Sandbox_revision != '0')
    {  
        sprintf(Version_Display, 
          " V %d.%2.2d.%3.3d-%c %2.2d ",
              version,revision,interim_revision,
                Sandbox_revision,
                  MenuPtrArrayIndex);     
        LCD_Display[2][14] = '-'; // Sandbox_revision
        LCD_Display[2][15] = (char)Sandbox_revision;         		                     	
    }
     else sprintf(Version_Display, 
       " V %d.%2.2d.%3.3d %2.2d   ",
         version,revision,interim_revision, 
           MenuPtrArrayIndex);    
}
     
/*
*      // tagwbg 01 for code removed from controller program 
*      // tagwbg 02 code  added
*      // tagwbg 03 write_dpyspi
*      // tagwbg 04 write_display() 
*      // tagwbg 05 LCD_Buffer_to_spi()
*      // tagwbg 06 Remember to leave this code, or replace with similar
*      // tagwbg 07 1/100 s interrupt 
*      // tagwbg 08 1 s interrupt
*      // tagwbg 09 This code could probably be removed
*      // tagwbg 10 This var, 'int_flag', gets set to 1 every 20ms and to 0 every 20ms
*      //    At 0ms, 20ms, 40ms, etc. gets set to 0.
*      //    At 10ms, 30ms, 50ms, etc. gets set to 1.
*      // tagwbg 11 - version
*      // tagwbg 12 - main menu definition
*      // tagwbg 13 - watchdog.
*      // tagwbg 14 - Disable LEDs 1 and 2 
*      // tagwbg 15 - LED Z6 ON
*      // tagwbg 16 - Blinking GALaxy in MenuManager.c 
*      // tagwbg 17 - 'load_special_char()' 
*      // tagwbg 18 - 'receive_hl_com();' pack_hl_com
*      // tagwbg 19 - 'pack_hl_com'
*      // tagwbg 19 - Read consB.dat file
*      // tagwbg 21   pack_hl_com(25, 20);
*      // tagwbg 22 - pack_hl_com(24, 20)
*
*/

/*  *****************************************
 * FREESCALE: SPC5668GVMG 
 *  system Clock 108.75 Mhz
 *    https://www.nxp.com/part/SPC5668GVMG
 *    https://www.nxp.com/docs/en/reference-manual/MPC5668xRM.pdf
 *
 * CPLD2 
 *
 *		VERSION INFORMATION  ( tagwbg 11 ) {line 18 of car\control.c }
 *        This SW started life as a clone of the GALx1132 controller SW 
 *          version v8.0.9.
 *        Search for "tagwbg"  to see changes made by William Grauer
 *
 *	v 0.0.0 - A  Start code for 1137 board (Serial Hall Station via Grp CAN) and
 *    1139 board (Serial Hall Lantern vial RS485 half-duplex).
 *	v 0.0.0 - E  Works for testing one 1139 and one 1137 at the same time.
 *    Copy of this code given to ZYM.
 *	v 0.0.0 - F  Change 1137 code to turn one output on at a time instead
 *    of both (in order to see if there is a short). Add code for setting hall lantern floor
 *    and data. See Hall_Lantern_floor_A and Hall_Lantern_floor_B, with Hall_Lantern_Data_A
 *    and Hall_Lantern_Data_B.
 *	v 0.0.0 - G (2018-3-15) Allow a Mode button exit from Hall_Station_CAN() in 
 *    "Wait_for_Operator_Response" case
 *	v 0.0.0 - J (2018-3-15) Set up to test 1, 2, or 3 boards at a time. Now works with one 1137
 *    and not tested with more then one.  Not set up to test 1139 in multi-board config.
 *	v 0.0.0 - K (2018-3-16) Set up to test 1137 & 1139 in multi-board config, and set so that
 *    we can test one 1137 and two 1139, or two 1137 and one 1139; but not at the same time
 *    (select either to test 1137 or 1139).
 *	v 0.0.0 - M (2018-3-17) Change from MR CAN to CT CAN for GALx1107AN I/O board.
 *	v 0.0.0 - N (2018-3-19) 
 *	v 0.0.4 - P (2018-5-01) Change comm_control() to not call grcan_control(). Instead
 *    call pack / unpack for group CAN, explicitly in the 1137 test. Also, add some men
 *    items to set individual lanterns in 1137 and to send reset 0, 2, 5, 8, 10 each time a
 *    lantern state is changed.
 *    This version success with a single 1137,
 *	v 0.0.6 - P (2018-5-03) 1139 tests work OK with the 3-up fixture, however, 1137 does 
 *    not work, as it always shows PASS even when a board is not plugged in.
 *	v 0.0.7 - P (2018-5-03) Use menu 22 as a template for second half test of 1137.
 *	v 0.0.12 - P (2018-5-06) Still working on test of 1137, #2. #1 and #3 seem OK.
 *	v 0.0.13 - P (2018-5-07) GALX-1137 now works with menu 17, 3-up.
 *	v 0.0.14 - P (2018-5-07) Set up GALX4-0047N 24V I/O board. Toggle enable via menu 24
 *	v 0.0.15 - P (2018-5-07) Diagnostics for 1137 (menu 26) now working.
 *    GALX4-0047N, 24V I/O SEB now set up as dip switch 9 and is now working.
 *    Test the GALX4-0047N with menu 24 and activate with switch 1. Works also for
 *    1139 - 3 up.  If it does not work with 1139 and this version, check the consB.dat file. 
 *	v 0.0.16 - P (2018-5-09) 
 *	v 0.0.17 - P (2018-5-10) - Production version for 1137 / 1139
 *	v 0.0.0  - X (2018-5-14) - A generic board test SW based on GALX-1132AN.
 *    MenuMgr broken up so that the menus anr in menus.c. Instead of having multiple include files
 *    for the test programs, put those include files into one, "Tests.h".
 
 * =======================================================================
 
 *    Restructure the original GALx1132 Controller SW from 'A' below, to 'B' below
 *  *******************************************
 *  'A' 
 *  	 Loop forever 
 * 	for (;;) {
 *   	i++;
 *   	control();
 * 	}
 *
 *  *******************************************
 *   'B' 
 *  	 Loop forever 
 *  	for (;;) {
 *   	i++;
 *      wait in a loop until the 10ms interrupt is triggerd. While in the loop,
 *          // keep petting the dog.
 *   	new_inctime();  // increment all SW timers after the 10ms timer interrupt is
 *   	    // triggered.
 *      comm_control(); // send/receive Com and CAN traffic
 *   	Service_Buttons(); // debounce the Up, Down, Mode, and Enter buttons
 *   	    	    
 *   	MenuManager();  // based on the buttons pressed or the state of the 
 *         //  testing, change the menu and then update the four LCD Buffers[]	
 *   	LCD_Buffer_to_spi(); // send the LCD Buffers[4] to the SPI
 *      comm_control();
 * 
 * 	} // END " for (;;) "
 *  *******************************************
 *

enum 
{
  DONE_BUT_CONTINUE_DISPLAY = -1, // failed
  FINISHED_WITH_SERVICE, // passed
  CONTINUE_SERVICE, // still performing test
  RETEST_OPTION_AFTER_DELAY,  //  to see msg
  RETEST_OPTION, //failed, but give option to redo
  CLOSE_MSG_BOARD, //  Used by retest option
  DEFAULT  // Fatal error - abort	
};
////////////////////////////////

    int16 i,j,k = 0;
    static int16 test_Kase = initialize;
    static int16 Delay_Count = 3; 
    static int16 Retest_Option_Parm = 0; // Required for Retest Option
    static int16 RetestResponse = 1; // Required for Retest Option
    static int16 ReturnVal = CONTINUE_SERVICE; // Required for Retest Option
    
    SIU.GPDO[Z0_LED].R = !SIU.GPDO[Z0_LED].R;  // toggle LED;
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
            test_Kase = run_test;
            break;
            
        case run_test:
            if(timers[t_Generic_Delay] < Delay_Count) break;            
            //  ADD CODE
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
    
=============================

  switch (Kase)
    {
        case 0:
            //  ADD CODE

            break;
            
        case 1:            
            //  ADD CODE
            
            break;
                        
        case 2:
 
            break;
            
        case 3:
              
            break;
                                    
        case 4:
 
            break;
            
        case 5: 
            
            break;           
               
        default:
        
            break;                
    }
        
 *
 *
 *
 *
 */
// C:\Galaxy\Hardware\GALX-1121_(GIV Hydro)\GALX-1121AN_REV_C_PCB-1143N (MAIN)\Project Outputs for GALX-1121AN_REV_C_PCB-1143N\OUTPUT