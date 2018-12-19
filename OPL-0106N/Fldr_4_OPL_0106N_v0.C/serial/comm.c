//  Comm Routines
#define d_COMM 1
#include "global.h"

void enableIrq(void); 
void disableIrq(void);
void zero_req_que(void); 
void zero_status_que(void);
void Small_initcom(void);
extern void init_grp_DMA(void);
extern void Small_init_grp_DMA(void);

int print_com1(const char* ctrl1, ... );

int32 com_debug1 = 0;
int32 com_debug2 = 0;
int32 com_debug3 = 0;
int32 com_debug4 = 0;

int16 Com_Mode[3] = {0,0,0};
char unsigned cc_req_fl[3], rcc_req_fl[3];		// hold's car call request floor number

int16 com_nmb_fl[3] = {29,29,29};
int16 com_nmb_car[3] = {6,6,6};
int16 com_nmb_io[3] = {157,157,97};
int16 grp_Car_Stat_req[3] = {0,0,0};

// character strings used for communications
#define rbufsize 1000
#define tbufsize 3000
uint8 rmbuf[3][rbufsize+1];             	// com 1 recieve message buffer
uint8 tranbuf[3][tbufsize+1];  			// com 1 transmit buffer
uint8 recbuf[3][rbufsize+1];   			// com 1 receive buffer
int16 mpnt[3]={0,0,0};       				// message pointer for com 1-2
int16 rbs[3]={0,0,0}; 						// receive buffer start pointer com 1-2
int16 rbf[3]={0,0,0}; 						// receive buffer end pointer com 1-2
int16 tbs[3]={0,0,0};             			// trasmit buffer start pointer com 1-2
int16 tbf[3]={0,0,0};             			// trasmit buffer end pointer com 1-2
int16 Com_Tran_Int[3]={0,0};				// comunication port transmit interupt running
int16 Com_Recv_Int[3]={0,0};				// comunication port transmit interupt running
unsigned char Tranbuf_Empty[3] = {1,1,1}; // Com 1-2 Transmiter buffer is empty if 1 has chars if 0
unsigned char Tranbuf_Full[3] = {0,0,0};	// Com Transmit buffer is full if 1 and 0 if not
int16 Try_Restart[3]={0,0,0}; 				// try to restablish commuiniction with other comm devices

int16 logon[2] = {0,0};          // logon = 0 no activity on com port #1
			// logon = 1 sent out user name waiting for answer
			// logon = 2 sent our password waiting for answer
			// logon = 3 user has loged on the system
int16 User_Log[2] = {0,0};
int16 connect[2] = {0,0};
int16 modem_status[2] = {0,0};
int16 modem_dcd[2] = {0,0};
int16 modem_state[2] = {0,0};
int16 init_modem[2] = {0,0};
int16 user_com_ver[2] = {0,0};			// default= 0, Increment with each comm change
int16 user_com_type[2] = {0,0};			// User type: 1 = galcom, 2 = GALileo

int16 com_version = 4;

// User Interface UI security flags
int16 ui_sec = 0;
int32 ui_uc_sec[2] = {0,0};
int32 ui_ruc_sec[2] = {0,0};
int32 ui_dc_sec[2] = {0,0};
int32 ui_rdc_sec[2] = {0,0};

int32 ui_cc_sec[2][car_size];
int32 ui_cc_sec_prev[2][car_size];

int32 ui_rcc_sec[2][car_size];
int32 ui_rcc_sec_prev[2][car_size];

int32 ui_uhc_carsec[2][car_size];
int32 ui_dhc_carsec[2][car_size];
int32 ui_ruhc_carsec[2][car_size];
int32 ui_rdhc_carsec[2][car_size];


// User Interface UI control flags
int16 ui_ind[car_size];
int16 ui_rtl[car_size];
int16 ui_heof[car_size];
int16 ui_ad_off[car_size];
int16 ui_uppeak = 0;
int16 ui_dnpeak = 0;
int16 ui_alt_lby = 0;
int16 ui_alt_park = 0;
int16 ui_ncu = 0;
int16 ui_alt_fl_sec = 0;

// Comm security variables for com1 and com2 (note com3 is the drive comm)

unsigned char Com_Add_Sec_Fl_Req[2] = {0,0};					// Send security floor to add code
unsigned char Com_Add_Sec_Code[2][4] = {{0,0,0,0},{0,0,0,0}};	// Security Code to Add a code
unsigned char Com_Del_Sec_Fl_Req[2] = {0,0};					// Send Security floor to delete code
unsigned char Com_Del_Sec_Code[2][4] = {{0,0,0,0},{0,0,0,0}};	// Security code to delete a code
unsigned char Com_Del_All_Codes_Fl_Req[2] = {0,0};				// Delete all codes for one floor or all floors
unsigned char Com_Sec_Fl_Tbl_Req[2] = {0,0}; 					// Floor request for security table
int16 Com_LookUpTable[2][NumMaxFl];
int16 Com_Sec_Chksum_Fl[2] = {0,0};								// Floor start for Security Checksum

struct CdTable Com_CodeTable[2];
struct Cd_Tbl_Chk Com_Code_Tbl_Chk[2][fl_size];

// Comm Que variables

#define c_que_len 40
#define c_nmb_com_ports 7

// comm ports that use the queue
/*
USR1COM 0		// Must be 0 and 1 for USR1 and USR2 comm
USR2COM 1
TERMCOM 2
DRVCOM 3		// used for put and get que
GRPCOM 4
GCANCOM 5		// used for put and get que
HLCOM 6			// used for put and get que
*/

struct pkt_req_que {
	int16 pkt[c_que_len];
	int16 addr[c_que_len];
} req_que[c_nmb_com_ports];		 // que for each comm port

struct pkt_status_que {
	int16 pkt[c_que_len];
	int16 addr[c_que_len];
	int16 status[c_que_len];
	int16 timer[c_que_len];
} st_que[c_nmb_com_ports];		// que for each comm port


// User Comm Variables
int16 com_rx_char_cnt[3] = {0,0,0};
int16 good_packet[3] = {0,0,0};
int16 bad_packet[3] = {0,0,0};
int16 dma_start_cnt[3] = {0,0,0};
int16 dma_end_cnt[3] = {0,0,0};


int16 com_1_count = 0;
int32 com_1_source = 0;
int32 com_1_modem_status = 0;
int32 com_1_line_status = 0;
int16 com_2_count = 0;
int32 com_2_source = 0;
int32 com_2_modem_status = 0;
int32 com_2_line_status = 0;
int16 com_3_count = 0;
int32 com_3_source = 0;
int32 com_3_modem_status = 0;
int32 com_3_line_status = 0;


//				 {19.2k,19.2k,19.2k}
int16 comm_baud[3]={354,  354, 354};	// default baud rate for user interface	drive comm

//				  {		
//				     {2400, 4800, 9600, 19.2, 38.4, 57.6, 115.2, 230400 baud}		
int16 baud_tbl[8] = {2832, 1416,  708,  354,  177,  118,    59,     29 };	// baud setting for 108.75 MHz
	
// baud setting for 108.75 MHz

	
// Prototypes of all functions in comm.cpp
int16 get_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb, int16 time);
int16 put_pkt_req (int16 pkt_nmb, int16 addr, int16 com_nmb);
int16 get_pkt_req (int16 addr, int16 com_nmb);
int16 asigetc(int16);
void com1int(void);
void com2int(void);
int16 compress_buf(int16 buf_len, int16 max_len);
void dev_off_line(int16 dev);
void Restart_Com(int16 );
void initcom(void );
#if ((Traction == 1) || (Tract_OL == 1))
extern void init_drv_com(void);
#endif
int16 isrxempty(int16);
int16 istxempty( int16 comnmb);
int16 recvm(int16);
void sendc( int16, int16);
void com_1_dma_trm_int(void);
void com_1_tx_start(int16 len);
void com_2_dma_trm_int(void);
void com_2_tx_start(int16 len);
void com_device_init(uint16 comnmb);
void put_pkt_req_all_cars (int16 pkt_nmb);
void put_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb);
// com_1_dma_trm_int()


// ********************************************************
// This is interupt driven get char from the receive buffer
// ********************************************************
int16 asigetc( int16 comnmb)
{
  	int16 i;
  	i = recbuf[comnmb][rbs[comnmb]];
//  	recbuf[comnmb][rbs[comnmb]] = 0;
  	rbs[comnmb]++;
  	if(rbs[comnmb] >= rbufsize)
		rbs[comnmb] = 0;
  	com_rx_char_cnt[comnmb]++;
  	return(i);
}



//**************************************************
// serial interrupt for com port #1 (user interface)
//**************************************************
void com1int(void)
{
	char char_temp = 0;
	com_1_count++;
	com_1_source = USR1port.SR.R;
	if ((com_1_source & 0x1F000000) != 0)		// Overrun, Noise, Framing or Parity error
	{
		USR1port.SR.R = 0x1F000000;	// Clear error flags by writing ones. (clear the idle flag too)
	}
	if ((USR1port.SR.B.RDRF == 1) && (USR1port.CR1.B.RIE == 1))	//Interupt source is a recieve data ready
	{	// should never occur
		Com_Recv_Int[USR1COM] = 1;		// set a flag receive interupts running
		char_temp = (uint8)USR1port.SDR.B.D;	// Read data byte from buffer
		USR1port.SR.R = 0x20000000;           /* Clear RDRF flag */
	}
	if ((USR1port.SR.B.TDRE == 1) && (USR1port.CR1.B.TIE == 1))  // Interupt source is transmiter hold reg empty
	{
		Com_Tran_Int[USR1COM] = 1;		// set a flag Transmit interupts running
	
		USR1port.SR.R = 0x80000000;           /* Clear TDRE flag */
		USR1port.SDR.B.D = tranbuf[USR1COM][tbf[USR1COM]];
	
		tbf[USR1COM]++;
		if(tbf[USR1COM] >= tbufsize)
			tbf[USR1COM] = 0;
		
			
		if (tbs[USR1COM] >= tbf[USR1COM])
		{
			if ((tbf[USR1COM] + (tbufsize - tbs[USR1COM])) >= 50)
				Tranbuf_Full[USR1COM] = 0;
		}
		else
		{
			if ((tbf[USR1COM] - tbs[USR1COM]) >= 50)
				Tranbuf_Full[USR1COM] = 0;
		}
		if(tbs[USR1COM] == tbf[USR1COM])   // disabel transmiter empty interupt
		{
			USR1port.CR1.B.TIE = 0;			// Transmit Interrupt Disabled
			Tranbuf_Empty[USR1COM] = 1;
			Tranbuf_Full[USR1COM] = 0;
		}
	}
}

//**********************************
// serial interrupt for com port #2 
//**********************************
void com2int(void)
{
	char char_temp = 0;
	com_2_count++;
	com_2_source = USR2port.SR.R;
	if ((com_2_source & 0x1F000000) != 0)		// Overrun, Noise, Framing or Parity error
	{
		USR2port.SR.R = 0x1F000000;	// Clear error flags by writing ones. (clear idle flag too)
	}
	if ((USR2port.SR.B.RDRF == 1) && (USR2port.CR1.B.RIE == 1))	//Interupt source is a recieve data ready
	{		// should never occur
		Com_Recv_Int[USR2COM] = 1;		// set a flag receive interupts running
		char_temp = (uint8)USR2port.SDR.B.D;	// Read data byte from buffer
		USR2port.SR.R = 0x20000000;           /* Clear RDRF flag */
	}
	if ((USR2port.SR.B.TDRE == 1) && (USR2port.CR1.B.TIE == 1))  // Interupt source is transmiter hold reg empty
	{
		Com_Tran_Int[USR2COM] = 1;		// set a flag Transmit interupts running
	
		USR2port.SR.R = 0x80000000;           /* Clear TDRE flag */
		USR2port.SDR.B.D = tranbuf[USR2COM][tbf[USR2COM]];
	
		tbf[USR2COM]++;
		if(tbf[USR2COM] >= tbufsize)
			tbf[USR2COM] = 0;
		
			
		if (tbs[USR2COM] >= tbf[USR2COM])
		{
			if ((tbf[USR2COM] + (tbufsize - tbs[USR2COM])) >= 50)
				Tranbuf_Full[USR2COM] = 0;
		}
		else
		{
			if ((tbf[USR2COM] - tbs[USR2COM]) >= 50)
				Tranbuf_Full[USR2COM] = 0;
		}
		if(tbs[USR2COM] == tbf[USR2COM])   // disabel transmiter empty interupt
		{
			USR2port.CR1.B.TIE = 0;			// Transmit Interrupt Disabled
			Tranbuf_Empty[USR2COM] = 1;
			Tranbuf_Full[USR2COM] = 0;
		}
	}
}

#define USR1BUF_LEN 100
char USR1buf[USR1BUF_LEN];

int print_com1(const char* ctrl1, ... )
{
	uint16 i;
	uint16 tx_stat = 0;
	timers[tprntcomto] = 0;
	do 
	{
		tx_stat = istxempty(USR1COM);
	} while((tx_stat == 0) && (timers[tprntcomto] < 300));
	
	sprintf(USR1buf,ctrl1);
	for (i=0;i<USR1BUF_LEN;i++)
	{
		if(USR1buf[i] == 0)
			break;
		else
			sendc(USR1COM,USR1buf[i]);
	}
	return 0;
}


// ***************************************************************
// ******** Get packet number off of request que *****************
// Get packet out of que:  Data is taken off of the que from
// the device address in the order of when it was placed for
// that address.  The bottom of the que should all be zeros.
//****************************************************************
// ***************************************************************

int16 get_pkt_req (int16 addr, int16 com_nmb)
{
	int16 ix, pkt;

	ix = 0;
	pkt = 0;			// return 0 if no Packet on queue

	while (ix < c_que_len)
	{
		if (req_que[com_nmb].addr[ix] == addr) 
		{      	// Packet for this device found
			pkt = req_que[com_nmb].pkt[ix];	  // hold packet number for return data
			break;
		}
		else
			ix++;
	}

	// shift que data to the top (start where ix left off)

	while (ix < (c_que_len-1))
	{
		if ((req_que[com_nmb].pkt[ix+1] != 0) && (req_que[com_nmb].addr[ix+1] != 0))
		{   // Packet was taken from the middle of the que so shift data up
			req_que[com_nmb].pkt[ix] = req_que[com_nmb].pkt[ix+1];
			req_que[com_nmb].addr[ix] = req_que[com_nmb].addr[ix+1];
			ix++;
		}
		else
		{  // zero last Packet taken
			req_que[com_nmb].pkt[ix] = 0;	  // clear the que
			req_que[com_nmb].addr[ix] = 0;	  // clear the que
			break;
		}
	}
	// zero last Packet taken  Fix 10/22/14 if buffer overrun mhd
	req_que[com_nmb].pkt[ix] = 0;	  // clear the que
	req_que[com_nmb].addr[ix] = 0;	  // clear the que

	return pkt;
}

// ***************************************************************
// ******** Get packet status off of status que ******************
// ***************************************************************

int16 get_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb, int16 time)
{
	int16 ix, timer_exp;
	
	ix = 0;
	while (ix < c_que_len)
	{
		if ((st_que[com_nmb].pkt[ix] == pkt_nmb) && (st_que[com_nmb].addr[ix] == addr))
		{
			timer_exp = 0;
			if((st_que[com_nmb].timer[ix] & 0x8000) == 0x8000)
			{		// timer has to overflow for correct time
				if (timers[tcom_que] < 0x3FFF)
				{	// Overflowed to zero
					if (timers[tcom_que] > (st_que[com_nmb].timer[ix] & 0x7FFF))
						timer_exp = 1;
				}
			}
			else 
			{
				if(timers[tcom_que] >= st_que[com_nmb].timer[ix])
				{	// timer has expired
					timer_exp = 1;	   
				}
			}
			if(st_que[com_nmb].status[ix] == 1)
			{
				st_que[com_nmb].pkt[ix] = 0;
				st_que[com_nmb].addr[ix] = 0;
			}
			else if (timer_exp == 1)
			{		// signify timer has expired
				st_que[com_nmb].status[ix] = 2;
				st_que[com_nmb].pkt[ix] = 0;
			}
			return st_que[com_nmb].status[ix];
		}
		ix++;
	}

	ix = 0;
	while (ix < c_que_len)
	{
		if (st_que[com_nmb].pkt[ix] == 0)
		{
			st_que[com_nmb].pkt[ix] = pkt_nmb;
			st_que[com_nmb].addr[ix] = addr;
			// tcom_que can only go to 7fff (value of 8000 with sum means timer has to overflow)
			st_que[com_nmb].timer[ix] = timers[tcom_que] + time;   
			st_que[com_nmb].status[ix] = 0;
			return 0;
		}
		ix++;
	}
	return -1;	// Packet is not in the status queue
}


//*************************************************
//  Comm Device Init
//*************************************************

void com_device_init(uint16 comnmb)
{
	uint16 data_in;
	
	if (comnmb == USR1COM)
	{		
	  	/* Install our handlers */  
	  	INTC_InstallINTCInterruptHandler(com1int, USR1intLoc, 1);  

		// Control register 1	  	
		USR1port.LCR.B.LIN = 0;			// Place in SCI mode	
		USR1port.CR1.B.SBR = comm_baud[USR1COM];		
		
		USR1port.CR1.B.LOOPS = 0;			// Receiver source mode LOOPS:RSRC 00 = Dual wire mode
		USR1port.CR1.B.RSRC = 0;
		
		USR1port.CR1.B.M = 0;				// Frame Format Mode: set to 1 for nine bits
		USR1port.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
		USR1port.CR1.B.PE = 0;				// Parity Enable: 0 = disabled, 1 = enabled 
		USR1port.CR3.B.M2 = 0;

		USR1port.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd

		USR1port.CR1.B.ILT = 0;			// Idle Line Type: 0 = idle line detection after low bit
		USR1port.CR1.B.TIE = 0;			// Transmit Interrupt Enable (data tranferred to shift register)
		USR1port.CR1.B.TCIE = 0;		// Transmit Complete Interrupt Enable (data shifted out of shift register)
		USR1port.CR1.B.RIE = 0;			// Receive Interrupt Enable  Changed from 0 to 1 (wbg)
		USR1port.CR1.B.ILIE = 0;		// Idle line interrupt enable
		USR1port.CR1.B.RWU = 0;			// Receiver Wake Up mode: 0 = normal, 1 = wakeup on 9th bit address
		USR1port.CR1.B.SBK = 0;			// Send Break Character

		// Control Register 2
		USR1port.CR2.B.MDIS = 0;			// Module Disable
		USR1port.CR2.B.FBR = 0;				// Fast Bit Error Detect: 0 = normal, 1 = use in LIN mode	
		USR1port.CR2.B.BSTP = 0;			// DMA Stop on Bit Error: 0 = tx dma regardless of bit errors
		USR1port.CR2.B.IEBERR = 0;			// Bit Error Interrupt Enable
		USR1port.CR2.B.RXDMA = 1;			// Receive DMA Control: 0 = disabled, 1 = enabled. Changed from 1 to 0 (wbg)
		USR1port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
		USR1port.CR2.B.BRK13 = 0;			// Break Character Length
		USR1port.CR2.B.TXDIR = 0;			// TXD pin output enable: Used in signel wire mode
		USR1port.CR2.B.BESM13 = 0;			// Fast Bit Error Detection Sample Mode: 0=RS9,1=RS10 LIN mode only
		USR1port.CR2.B.SBSTP = 0;			// Transmit Stop on Bit Error
		USR1port.CR2.B.RXPOL = 0;			// RXD Pin polarity: 0 = normal, 1 = inverted
		USR1port.CR2.B.PMSK = 0;			// Parity Bit Masking: 0 = in bit position, 1 = 0 in parity position	
		USR1port.CR2.B.ORIE = 1;			// Overrun Interrupt Enable
		USR1port.CR2.B.NFIE = 1;			// Noise Interrupt Enable
		USR1port.CR2.B.FEIE = 1;			// Frame Error Interrupt Enable
		USR1port.CR2.B.PFIE = 1;			// Parity Error Interrupt Enable
		
		data_in = USR1port.SDR.R;
		data_in = USR1port.SDR.R;

		// Set RTS line high or low here when pin defined *******
		USR1txEn;
				
		Tranbuf_Empty[USR1COM] = 1;
		Tranbuf_Full[USR1COM] = 0;
		tbs[USR1COM] = 0;
		tbf[USR1COM] = 0;
		rbs[USR1COM] = 0;
		rbf[USR1COM] = 0;
		com_nmb_fl[USR1COM] = cons[grtopf];	
		com_nmb_io[USR1COM] = 157;	// 97 instead of 85 or 84: index 39 gets sent twice 
		com_nmb_car[USR1COM] = cons[nmbcars];

	  	// Initialize Transmit Comm DMA

	 	EDMA.CERQR.R = USR1dmaChTx;      		// Disable group EDMA channel   
	  
		INTC_InstallINTCInterruptHandler(com_1_dma_trm_int, USR1dmaTxIntLoc, 1);  

	  	EDMA.CPR[USR1dmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

		DMAMUX.CHCONFIG[USR1dmaChTx].R = USR1dmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

	    EDMA.TCD[USR1dmaChTx].SADDR = (uint32) &tranbuf[USR1COM][0];  						// Load address of source data 
	  	EDMA.TCD[USR1dmaChTx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
	  	EDMA.TCD[USR1dmaChTx].SOFF = 1;                         		// Increment source address by 1
	  	
	  	EDMA.TCD[USR1dmaChTx].SLAST = -10;                    	  		// After major loop, reset src addr 
	  	EDMA.TCD[USR1dmaChTx].SMOD = 0;                         		// Source modulo feature not used

	  	EDMA.TCD[USR1dmaChTx].DADDR =  USR1dr; 							// Load address of destination 
	  	EDMA.TCD[USR1dmaChTx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
	  	EDMA.TCD[USR1dmaChTx].DOFF = 0;                         		// Do not increment destination 
	  	EDMA.TCD[USR1dmaChTx].DLAST_SGA = 0; 				          	// After major loop reset destination address
	  	EDMA.TCD[USR1dmaChTx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
	  
	  	EDMA.TCD[USR1dmaChTx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
	  	EDMA.TCD[USR1dmaChTx].BITER = 10;		        	     		// Major loop iterations 
	  	EDMA.TCD[USR1dmaChTx].CITER = 10;       		    	  		// Initialize current iteraction count 
	  	EDMA.TCD[USR1dmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
	  	EDMA.TCD[USR1dmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
	  	EDMA.TCD[USR1dmaChTx].INT_MAJ = 1;
	  	EDMA.TCD[USR1dmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
	  	EDMA.TCD[USR1dmaChTx].BITERE_LINK = 0;
	  	EDMA.TCD[USR1dmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
	  	EDMA.TCD[USR1dmaChTx].MAJORLINKCH = 0;						// Major link channel 
	  	EDMA.TCD[USR1dmaChTx].E_SG = 0; 
	  	EDMA.TCD[USR1dmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
	  	EDMA.TCD[USR1dmaChTx].DONE = 0;
	  	EDMA.TCD[USR1dmaChTx].ACTIVE = 0;
	  	EDMA.TCD[USR1dmaChTx].START = 0;                        		// Initialize status flags 
	  
	  	EDMA.CEEIR.R = USR1dmaChTx;     		// DMA Clear Enable Error Interrupt Register  
	  	EDMA.CIRQR.R = USR1dmaChTx;         	// DMA Clear Interrupt Request Register 
	  	EDMA.CER.R = USR1dmaChTx;           	// DMA Clear error Register 

	  	// Initialize Receive Comm DMA

	 	EDMA.CERQR.R = USR1dmaChRx;      		// Disable group EDMA channel   
	  
	  	EDMA.CPR[USR1dmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

		DMAMUX.CHCONFIG[USR1dmaChRx].R = USR1dmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

	    EDMA.TCD[USR1dmaChRx].SADDR = USR1dr;  						// Load address of source data 
	  	EDMA.TCD[USR1dmaChRx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
	  	EDMA.TCD[USR1dmaChRx].SOFF = 0;                         		// Do not increment source address
	  	
	  	EDMA.TCD[USR1dmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
	  	EDMA.TCD[USR1dmaChRx].SMOD = 0;                         		// Source modulo feature not used

	  	EDMA.TCD[USR1dmaChRx].DADDR =  (uint32) &recbuf[USR1COM][0]; 			// Load address of destination 
	  	EDMA.TCD[USR1dmaChRx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
	  	EDMA.TCD[USR1dmaChRx].DOFF = 1;                         		// Increment destination addr by 1 
	  	EDMA.TCD[USR1dmaChRx].DLAST_SGA = (0 - rbufsize);           // After major loop reset destination address
	  	EDMA.TCD[USR1dmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
	  
	  	EDMA.TCD[USR1dmaChRx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
	  	EDMA.TCD[USR1dmaChRx].BITER = rbufsize;             		// Major loop iterations 
	  	EDMA.TCD[USR1dmaChRx].CITER = rbufsize;             		// Initialize current iteraction count 
	  	EDMA.TCD[USR1dmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
	  	EDMA.TCD[USR1dmaChRx].INT_HALF = 0;                     		// Interrupts are not used 
	  	EDMA.TCD[USR1dmaChRx].INT_MAJ = 0;
	  	EDMA.TCD[USR1dmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
	  	EDMA.TCD[USR1dmaChRx].BITERE_LINK = 0;
	  	EDMA.TCD[USR1dmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
	  	EDMA.TCD[USR1dmaChRx].MAJORLINKCH = 0;						// Major link channel 
	  	EDMA.TCD[USR1dmaChRx].E_SG = 0; 
	  	EDMA.TCD[USR1dmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
	  	EDMA.TCD[USR1dmaChRx].DONE = 0;
	  	EDMA.TCD[USR1dmaChRx].ACTIVE = 0;
	  	EDMA.TCD[USR1dmaChRx].START = 0;                        		// Initialize status flags 
	  
	  	EDMA.CEEIR.R = USR1dmaChRx;     		// DMA Clear Enable Error Interrupt Register  
	  	EDMA.CIRQR.R = USR1dmaChRx;         	// DMA Clear Interrupt Request Register 
	  	EDMA.CER.R = USR1dmaChRx;           	// DMA Clear error Register 
	  	EDMA.SERQR.R = USR1dmaChRx;				// Enable comm DMA channel
  
		// Enable must be last
		USR1port.CR1.B.TE = 1;			// Transmitter Enable
		USR1port.CR1.B.RE = 1;			// Receiver Enable
	}
	else if (comnmb == USR2COM)
	{
	  	/* Install our handlers */  
	  	INTC_InstallINTCInterruptHandler(com2int, USR2intLoc, 1);  

		// Control register 1	  	
		USR2port.LCR.B.LIN = 0;			// Place in SCI mode	
		USR2port.CR1.B.SBR = comm_baud[USR2COM];

		USR2port.CR1.B.LOOPS = 0;			// Receiver source mode LOOPS:RSRC 00 = Dual wire mode
		USR2port.CR1.B.RSRC = 0;

		USR2port.CR1.B.M = 0;				// Frame Format Mode: set to 1 for nine bits
		USR2port.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
		USR2port.CR1.B.PE = 0;			// Parity Enable: 0 = disabled, 1 = enabled 
		USR2port.CR3.B.M2 = 0;

		USR2port.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd

		USR2port.CR1.B.ILT = 0;			// Idle Line Type: 0 = idle line detection after low bit
		USR2port.CR1.B.TIE = 0;			// Transmit Interrupt Enable (data tranferred to shift register)
		USR2port.CR1.B.TCIE = 0;			// Transmit Complete Interrupt Enable (data shifted out of shift register)
		USR2port.CR1.B.RIE = 0;			// Receive Interrupt Enable
		USR2port.CR1.B.ILIE = 0;			// Idle line interrupt enable
		USR2port.CR1.B.RWU = 0;			// Receiver Wake Up mode: 0 = normal, 1 = wakeup on 9th bit address
		USR2port.CR1.B.SBK = 0;			// Send Break Character


		// Control Register 2
		USR2port.CR2.B.MDIS = 0;			// Module Disable
		USR2port.CR2.B.FBR = 0;			// Fast Bit Error Detect: 0 = normal, 1 = use in LIN mode	
		USR2port.CR2.B.BSTP = 0;			// DMA Stop on Bit Error: 0 = tx dma regardless of bit errors
		USR2port.CR2.B.IEBERR = 0;		// Bit Error Interrupt Enable
		USR2port.CR2.B.RXDMA = 1;			// Receive DMA Control: 0 = disabled, 1 = enabled
		USR2port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
		USR2port.CR2.B.BRK13 = 0;			// Break Character Length
		USR2port.CR2.B.TXDIR = 0;			// TXD pin output enable: Used in signel wire mode
		USR2port.CR2.B.BESM13 = 0;		// Fast Bit Error Detection Sample Mode: 0=RS9,1=RS10 LIN mode only
		USR2port.CR2.B.SBSTP = 0;			// Transmit Stop on Bit Error
		USR2port.CR2.B.RXPOL = 0;			// RXD Pin polarity: 0 = normal, 1 = inverted
		USR2port.CR2.B.PMSK = 0;			// Parity Bit Masking: 0 = in bit position, 1 = 0 in parity position	
		USR2port.CR2.B.ORIE = 1;			// Overrun Interrupt Enable
		USR2port.CR2.B.NFIE = 1;			// Noise Interrupt Enable
		USR2port.CR2.B.FEIE = 1;			// Frame Error Interrupt Enable
		USR2port.CR2.B.PFIE = 1;			// Parity Error Interrupt Enable
		
		data_in = USR2port.SDR.R;
		data_in = USR2port.SDR.R;

		// Set RTS line high or low here when pin defined *******
	  	if (cons[carnmb] == Dispatcher)
	  		USR2txEn;
		else 
			USR2txDis;
		
		Tranbuf_Empty[USR2COM] = 1;
		Tranbuf_Full[USR2COM] = 0;
		tbs[USR2COM] = 0;
		tbf[USR2COM] = 0;
		rbs[USR2COM] = 0;
		rbf[USR2COM] = 0;
		com_nmb_fl[USR2COM] = cons[grtopf];	
		com_nmb_io[USR2COM] = 157;	// 97 instead of 85 or 84: index 39 gets sent twice 
		com_nmb_car[USR2COM] = cons[nmbcars];

	  	// Initialize Transmit Comm DMA

	 	EDMA.CERQR.R = USR2dmaChTx;      		// Disable group EDMA channel   
	  
		INTC_InstallINTCInterruptHandler(com_2_dma_trm_int, USR2dmaTxIntLoc, 1);  

	  	EDMA.CPR[USR2dmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

		DMAMUX.CHCONFIG[USR2dmaChTx].R = USR2dmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

	    EDMA.TCD[USR2dmaChTx].SADDR = (uint32) &tranbuf[USR2COM][0];  						// Load address of source data 
	  	EDMA.TCD[USR2dmaChTx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
	  	EDMA.TCD[USR2dmaChTx].SOFF = 1;                         		// Increment source address by 1
	  	
	  	EDMA.TCD[USR2dmaChTx].SLAST = -10;                    	  		// After major loop, reset src addr 
	  	EDMA.TCD[USR2dmaChTx].SMOD = 0;                         		// Source modulo feature not used

	  	EDMA.TCD[USR2dmaChTx].DADDR =  USR2dr; 							// Load address of destination 
	  	EDMA.TCD[USR2dmaChTx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
	  	EDMA.TCD[USR2dmaChTx].DOFF = 0;                         		// Do not increment destination 
	  	EDMA.TCD[USR2dmaChTx].DLAST_SGA = 0; 				          	// After major loop reset destination address
	  	EDMA.TCD[USR2dmaChTx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
	  
	  	EDMA.TCD[USR2dmaChTx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
	  	EDMA.TCD[USR2dmaChTx].BITER = 10;		        	     		// Major loop iterations 
	  	EDMA.TCD[USR2dmaChTx].CITER = 10;       		    	  		// Initialize current iteraction count 
	  	EDMA.TCD[USR2dmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
	  	EDMA.TCD[USR2dmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
	  	EDMA.TCD[USR2dmaChTx].INT_MAJ = 1;
	  	EDMA.TCD[USR2dmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
	  	EDMA.TCD[USR2dmaChTx].BITERE_LINK = 0;
	  	EDMA.TCD[USR2dmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
	  	EDMA.TCD[USR2dmaChTx].MAJORLINKCH = 0;						// Major link channel 
	  	EDMA.TCD[USR2dmaChTx].E_SG = 0; 
	  	EDMA.TCD[USR2dmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
	  	EDMA.TCD[USR2dmaChTx].DONE = 0;
	  	EDMA.TCD[USR2dmaChTx].ACTIVE = 0;
	  	EDMA.TCD[USR2dmaChTx].START = 0;                        		// Initialize status flags 
	  
	  	EDMA.CEEIR.R = USR2dmaChTx;     		// DMA Clear Enable Error Interrupt Register  
	  	EDMA.CIRQR.R = USR2dmaChTx;         	// DMA Clear Interrupt Request Register 
	  	EDMA.CER.R = USR2dmaChTx;           	// DMA Clear error Register 

	  	// Initialize Receive Comm DMA

	 	EDMA.CERQR.R = USR2dmaChRx;      		// Disable group EDMA channel   
	  
	  	EDMA.CPR[USR2dmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

		DMAMUX.CHCONFIG[USR2dmaChRx].R = USR2dmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

	    EDMA.TCD[USR2dmaChRx].SADDR = USR2dr;  						// Load address of source data 
	  	EDMA.TCD[USR2dmaChRx].SSIZE = 0;                        		// Read 2**0 = 1 byte per transfer 
	  	EDMA.TCD[USR2dmaChRx].SOFF = 0;                         		// Do not increment source address
	  	
	  	EDMA.TCD[USR2dmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
	  	EDMA.TCD[USR2dmaChRx].SMOD = 0;                         		// Source modulo feature not used

	  	EDMA.TCD[USR2dmaChRx].DADDR =  (uint32) &recbuf[USR2COM][0]; 			// Load address of destination 
	  	EDMA.TCD[USR2dmaChRx].DSIZE = 0;                        		// Write 2**0 = 1 byte per transfer 
	  	EDMA.TCD[USR2dmaChRx].DOFF = 1;                         		// Increment destination addr by 1 
	  	EDMA.TCD[USR2dmaChRx].DLAST_SGA = (0 - rbufsize);           // After major loop reset destination address
	  	EDMA.TCD[USR2dmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
	  
	  	EDMA.TCD[USR2dmaChRx].NBYTES= 1;                    			// Transfer 1 byte per major loop 
	  	EDMA.TCD[USR2dmaChRx].BITER = rbufsize;             		// Major loop iterations 
	  	EDMA.TCD[USR2dmaChRx].CITER = rbufsize;             		// Initialize current iteraction count 
	  	EDMA.TCD[USR2dmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
	  	EDMA.TCD[USR2dmaChRx].INT_HALF = 0;                     		// Interrupts are not used 
	  	EDMA.TCD[USR2dmaChRx].INT_MAJ = 0;
	  	EDMA.TCD[USR2dmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
	  	EDMA.TCD[USR2dmaChRx].BITERE_LINK = 0;
	  	EDMA.TCD[USR2dmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
	  	EDMA.TCD[USR2dmaChRx].MAJORLINKCH = 0;						// Major link channel 
	  	EDMA.TCD[USR2dmaChRx].E_SG = 0; 
	  	EDMA.TCD[USR2dmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
	  	EDMA.TCD[USR2dmaChRx].DONE = 0;
	  	EDMA.TCD[USR2dmaChRx].ACTIVE = 0;
	  	EDMA.TCD[USR2dmaChRx].START = 0;                        		// Initialize status flags 
	  
	  	EDMA.CEEIR.R = USR2dmaChRx;     		// DMA Clear Enable Error Interrupt Register  
	  	EDMA.CIRQR.R = USR2dmaChRx;         	// DMA Clear Interrupt Request Register 
	  	EDMA.CER.R = USR2dmaChRx;           	// DMA Clear error Register 
	  	EDMA.SERQR.R = USR2dmaChRx;				// Enable group coomm DMA channel
 
 		// Enable must be last
		USR2port.CR1.B.TE = 1;			// Transmitter Enable
		USR2port.CR1.B.RE = 1;			// Receiver Enable 
	}
}

/************************************************
 * initialize the serial communications port
 * installs interrupt, sets speed, etc
 ************************************************/
void initcom(void)
{
	_disable();

	// Initialize group comm variables
	init_grp_com();

  	petdog();

#if ((Traction == 1) || (Tract_OL == 1))
	if ((cons[sercom] & 8) != 0)
		init_drv_com();
#endif

  	petdog();
	// init user comm 1
	com_device_init(USR1COM);
  	petdog();
	
	// init comm 2
	com_device_init(USR2COM);	
  	petdog();

	_enable();
}

void Small_initcom(void)
{
	_disable();

    Small_init_grp_DMA();
   
	GRPport.CR1.B.RE = 1;					// Receiver Enable
  	EDMA.SERQR.R = GRPdmaChRx;				// Enable group coomm DMA channel
	GRPport.CR2.B.RXDMA = 1;				// Receive DMA Control: 0 = disabled, 1 = enabled
  	petdog();
 	
	_enable();    
}
 

//****************************************
// Test to see if receive buffer is empty
//****************************************
int16 isrxempty(int16 comnmb)
{
	static int16 ite_count, beg_count, dma_count;

	if (comnmb == USR1COM)
	{
		beg_count = (int16)EDMA.TCD[USR1dmaChRx].BITER;
		ite_count = (int16)EDMA.TCD[USR1dmaChRx].CITER;
		dma_count = (int16)(beg_count - ite_count);
		
		if (rbs[comnmb] == dma_count) 
	      	return(1);
	    else
	    {
/*	    
    	  	if(comnmb == USR1COM)
    	  	{ // debug03
    	      New_write_display("rbs[0],dmaCount     ",3,0);
              LCD_Display[3][16] = (char)( rbs[comnmb]%10  + '0');
              LCD_Display[3][18] = (char)( dma_count%10  + '0');	  	    
    	  	}
*/    	  	
    	 	return(0);	        
	    }
	      
	}
	else if (comnmb == USR2COM)
	{
		beg_count = (int16)EDMA.TCD[USR2dmaChRx].BITER;
		ite_count = (int16)EDMA.TCD[USR2dmaChRx].CITER;
		dma_count = (int16)(beg_count - ite_count);
		
		if (rbs[comnmb] == dma_count) 
	      	return(1);
	    else
	      	return(0);
	}
	return(0);
}


//****************************************
// Test to see if transmit buffer is empty
//****************************************
int16 istxempty( int16 comnmb)
{
    if(comnmb > USR2COM)
		return(2);
    if(tbs[comnmb] == tbf[comnmb])
      	return(1);
    else
      	return(0);
}




// ***************************************************************
// ******** Put packet request on request que ********************
// Put Packet on the que; if que is full throw Packet away
//   since que should never be full. Data is placed in the que
//   in the order of when it was called for each device address.
//****************************************************************
//****************************************************************

//lint -esym(534,put_pkt_req)
int16 put_pkt_req (int16 pkt_nmb, int16 addr, int16 com_nmb)
{
	int16 ix;
	ix = 0;

	while(ix < c_que_len)
	{      // look for and open spot on the que
		if (req_que[com_nmb].pkt[ix] == 0)
		{         // found an open spot so place Packet on que
			req_que[com_nmb].pkt[ix] = pkt_nmb;
			req_que[com_nmb].addr[ix] = addr;
			return 1;	// data on the que
		}
		else     // index to next spot (if non found, throw the data away)
			ix++;
	}
	return 0;		// que is full
}


// ***************************************************************
// ******** Put packet request all cars **************************
// ***************************************************************

void put_pkt_req_all_cars (int16 pkt_nmb)
{
	int16 i;

	if (cons[nmbcars] > 1)
	{
		for (i=1;i<=cons[nmbcars];i++)
		{
			if (i != cons[carnmb])
				put_pkt_req(pkt_nmb,i,gc_port);
		}
	}
}

// ***************************************************************
// ******** Put packet status on status que **********************
// ***************************************************************

void put_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb)
{
	int16 ix;

	ix = 0;
	while (ix < c_que_len)
	{
		if ((st_que[com_nmb].pkt[ix] == pkt_nmb) && (st_que[com_nmb].addr[ix] == addr))
		{
			st_que[com_nmb].status[ix] = 1;
			return;
		}
		ix++;
	}
	return;
}



//*****************************************
// This is receive a message procedure
// it returns a mode value when a complete message
// is received a 0 wen not complete
//
// Index 0 = P
//       1 = length
//       2 = packet number
//		 n = data (checksum can be in the last byte of the data)
//       n+1 = @ - end of packet
//*****************************************
int16 recvm(int16 Which_Com)
{
  	int16 i,j;
  	int16 rbempt=0;
  	int16 endmess=0;
  	
  	rbempt = isrxempty(Which_Com);
//    New_write_display("rbempt =            ",3,0);
//    LCD_Display[3][9] = (char)((rbempt%10) + '0');
  	while((rbempt == 0) && (endmess == 0))
  	{  // if(Which_Com == USR2COM) SIU.GPDO[Z6_LED].R = 0;
  		// turn led ON (wbg) tagwbg 15 0.1.29_H debug01
	  	rmbuf[Which_Com][mpnt[Which_Com]] = (uint8)asigetc(Which_Com);
	  	mpnt[Which_Com]++;
/*	  	
	  	if(Which_Com == USR1COM)
	  	{ // debug02
	      New_write_display("mpnt[0] =           ",3,0);
          LCD_Display[3][10] = (char)( (mpnt[Which_Com] -1)%10  + '0');	  	    
	  	}
*/	  	
	  	rbempt = isrxempty(Which_Com);
	  	if(rmbuf[Which_Com][0] != 'R')
	  	{
	  	   // SIU.GPDO[Z6_LED].R = ~SIU.GPDO[Z6_LED].R;
//	  	    SIU.GPDO[Z6_LED].R = 0;  // turn on LED debug01
	  	   //New_write_display("rmbuf[0][0] =       ",3,0);
           //LCD_Display[3][14] = (char)(rmbuf[Which_Com][0]);
	  	   mpnt[Which_Com] = 0;	  	 
	  	}	  	  	
	  	else
	  	{	  	
	  	  	if( (mpnt[Which_Com] >= rbufsize) || 
	  	  		((mpnt[Which_Com] > 1) && (mpnt[Which_Com] > rmbuf[Which_Com][1])))
	  	  	{
	  			rbempt = 1;
	  			mpnt[Which_Com] = 0;
	  	  	}
	  	  	if( mpnt[Which_Com] == rmbuf[Which_Com][1] )
	  	  	{
	  	  	    if(rmbuf[Which_Com][(rmbuf[Which_Com][1]-1)] == '$') // Good Packet
	  	  	    { //if(Which_Com == USR2COM) SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15 debug15
  					good_packet[Which_Com]++;
	  				rmbuf[Which_Com][(rmbuf[Which_Com][1]-1)] = 0;
	  				endmess = rmbuf[Which_Com][2];
	  				mpnt[Which_Com] = 0;
	  	  	    }
	  	  	    else                                // Bad Packet
	  	  	    {
  			        bad_packet[Which_Com]++;
	  				j = 1;
	  				while((rmbuf[Which_Com][j] != 'P') &&
	  					(j < mpnt[Which_Com]))
	  				{
	  					j++;
	  				}
	  				i = 0;
	  				while(j < mpnt[Which_Com])        /* found another start character */
	  				{
	  					rmbuf[Which_Com][i] = rmbuf[Which_Com][j];
	  					i++;
	  					j++;
	  				}
	  				mpnt[Which_Com] = i;
	  	  	    }
	  	  	    rbempt = 1;
	  	  	}
	  	}
  	}
  	return(endmess);
}



//********************************************
// Restart Comm routine
//********************************************
void Restart_Com(int16 com_nmb)
{ 
	if (com_nmb <= USR2COM)			// only com1 for test	
	{
		_disable();
		com_device_init(com_nmb);
		_enable();	
	}
}

//**************************************************
// This is start the comm 1 transmission on a COM Port
//**************************************************

void com_1_tx_start(int16 len)
{
	
	_disable();	   
	
	// Enable transmitter
	EDMA.CER.R = USR1dmaChTx;           	// DMA Clear error Register 
 	USR1port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	Tranbuf_Empty[USR1COM] = 0;
    EDMA.TCD[USR1dmaChTx].SADDR = (uint32) &tranbuf[USR1COM][0];  	// Load address of source data 
  	EDMA.TCD[USR1dmaChTx].SLAST = (0-len);        // After major loop, reset src addr 
	EDMA.TCD[USR1dmaChTx].NBYTES = 1;	// Transfer len number byte per minor loop 
 	EDMA.TCD[USR1dmaChTx].BITER = len;   // 10 major loop iterations 
  	EDMA.TCD[USR1dmaChTx].CITER = len;   // Initialize current iteration count 
  	EDMA.SERQR.R = USR1dmaChTx;      	// Enable Group EDMA channel   
	USR1port.CR2.B.TXDMA = 1;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	dma_start_cnt[0]++;
	_enable();
}

//*********************************************
// Com 1 DMA transmit finished interrupt
//*********************************************

void com_1_dma_trm_int(void)
{
	
  	EDMA.CIRQR.R = USR1dmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = USR1dmaChTx;      	// Disable Drive EDMA channel   
	USR1port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	Tranbuf_Empty[USR1COM] = 1;
	dma_end_cnt[0]++;
}

//**************************************************
// This is start the comm 2 transmission on a COM Port
//**************************************************

void com_2_tx_start(int16 len)
{
	
	_disable();	   
	
	// Enable transmitter
	EDMA.CER.R = USR2dmaChTx;           	// DMA Clear error Register 
 	USR2port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	Tranbuf_Empty[USR2COM] = 0;
    EDMA.TCD[USR2dmaChTx].SADDR = (uint32) &tranbuf[USR2COM][0];  	// Load address of source data 
  	EDMA.TCD[USR2dmaChTx].SLAST = (0-len);        // After major loop, reset src addr 
	EDMA.TCD[USR2dmaChTx].NBYTES = 1;	// Transfer len number byte per minor loop 
 	EDMA.TCD[USR2dmaChTx].BITER = len;   // 10 major loop iterations 
  	EDMA.TCD[USR2dmaChTx].CITER = len;   // Initialize current iteraction count 
  	EDMA.SERQR.R = USR2dmaChTx;      	// Enable Group EDMA channel   
	USR2port.CR2.B.TXDMA = 1;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	dma_start_cnt[1]++;
	_enable();
}

//*********************************************
// Com 2 DMA transmit finished interrupt
//*********************************************

void com_2_dma_trm_int(void)
{
	
  	EDMA.CIRQR.R = USR2dmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = USR2dmaChTx;      	// Disable Drive EDMA channel   
	USR2port.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	Tranbuf_Empty[USR2COM] = 1;
	dma_end_cnt[1]++;
}



//*********************************************
// This is send a character out on a COM Port
//*********************************************
void sendc( int16 comnmb, int16 letter)
{
    if(comnmb <= USR2COM) 
    {
		_disable();
		tranbuf[comnmb][tbs[comnmb]] = (uint8)letter;
		tbs[comnmb]++;
		if(tbs[comnmb] >= tbufsize)
			tbs[comnmb] = 0;
		
		if (tbs[comnmb] >= tbf[comnmb])
		{
			if ((tbf[comnmb] + (tbufsize - tbs[comnmb])) < 50)
				Tranbuf_Full[comnmb] = 1;
		}
		else
		{
			if ((tbf[comnmb] - tbs[comnmb]) < 50)
				Tranbuf_Full[comnmb] = 1;
		}
		Com_Tran_Int[comnmb] = 0;
		
		Tranbuf_Empty[comnmb] = 0;
		if (comnmb == USR1COM) // debug11
		{ // debug06
//    	      New_write_display("letter =            ",2,0);
//            LCD_Display[2][10] = (char)(letter );		
//		  if(letter == 'R') SIU.GPDO[Z6_LED].R =  0;	 
           // turn led ON (wbg)// tagwbg 15		
		  USR1port.CR1.B.TIE = 1;  // Transmit Interrupt Enabled		    
		}

		else if (comnmb == USR2COM)
		{ // debug05
//		  SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
//		  if(letter == 'T') SIU.GPDO[Z6_LED].R = 0;	
            // turn led ON (wbg)// tagwbg 15
		  USR2port.CR1.B.TIE = 1;			// Transmit Interrupt Enabled		    
		}

		
		_enable();
    }
    else if(comnmb == TERMCOM) 
    {
		tranbuf[comnmb][tbs[comnmb]] = (uint8)letter;
		tbs[comnmb]++;
		if(tbs[comnmb] >= tbufsize)
			tbs[comnmb] = 0;
		
		if (tbs[comnmb] >= tbf[comnmb])
		{
			if ((tbf[comnmb] + (tbufsize - tbs[comnmb])) < 50)
				Tranbuf_Full[comnmb] = 1;
		}
		else
		{
			if ((tbf[comnmb] - tbs[comnmb]) < 50)
				Tranbuf_Full[comnmb] = 1;
		}
		Com_Tran_Int[comnmb] = 0;
		
		Tranbuf_Empty[comnmb] = 0;
    }
}

// ***************************************************************
// ******** Zero request que *************************************
// ***************************************************************
void zero_req_que(void) 
{
	int16 i,j;

	for(i=0;i<c_nmb_com_ports;i++)
	{
		for(j=0;j<c_que_len;j++)
		{
			req_que[i].pkt[j] = 0;
			req_que[i].addr[j] = 0;
		}
	}		
}

// ***************************************************************
// ******** Zero status que **************************************
// ***************************************************************

void zero_status_que(void)
{
	int16 i,j;
	for(i=0;i<c_nmb_com_ports;i++)
	{
		for(j=0;j<c_que_len;j++)
		{
			st_que[i].pkt[j] = 0;
			req_que[i].addr[j] = 0;
			st_que[i].status[j] = 0;
			st_que[i].timer[j] = 0;
		}
	}		
}


/* Revision History

4/1/02	2.00 mhd		1. Added multidrop group comm routines.
4/4/02	2.01 mhd		1. Add routines to compress and uncompress car to group comm packet.
4/17/01 2.01 mhd		2. Changed baud rate to 19200 for hydro controllers not using SATDX4 CPU boards.
						3. Also changed timing for comm polling when running at 19200.
6/7/02  2.02 mhd		1. When not dispatcher (Group_Com routine) set gc_master = 0.  Cannot be the comm
						   master when not the dispatcher.
6/13/02 2.02 mhd		2. Use temporary float to convert velocity to integer.  This avoids a conversion
						   error.
6/25/02 2.03 mhd		1. Added com_mode 50 to expand options sent to galcom.
7/1/02  2.03 mhd		2. Use parity bit to qualify start character in group comm. 
07/9/02 3.00 mhd		1. When using iodata array, use labels instead of numbers such as DNRD instead of 7.
7/30/02 3.00 mhd		2. Add gc_loss_cnt to count comm loss and to call restart_comm. 
10/21/02 3.01 mhd		1. For Tract non-distance feedback controllers, use UPF and DF as speed command signals B1:B0.
						   This caused logic for slowdown to be changed.
						2. Transfer new fault data from car to group and from group to HI in multiple packets.  
						3. Deleted Car_Flts array and use Flts array for all cars.
12/18/02 3.02 mhd		1. Changed group comm polling to pole each car then one hall call device.  Also allow different
						   slave_packet_time[] depending on the number of cars
						2. Modified gc_loss_cnt to count from 1 to 9 when comm is lost. Gc_dev_online is set to one 
						   when a packet is received and 0 when the gc_loss_cnt is > 9. 
						3. Start transmitting immediately after receiving a packet (no minimum delay before looking for unpack).
						4. Modified Group_Com heavily for better through put and quicker change from car 2 dispatcher to car 1.
						5. Send Discrete PI flag to user monitor via comm.
1/21/03 3.02 mhd		6. Change check for LCD_Mode to clear learn mode flag and speed clamps.
3/18/03 3.03 mhd		1. Packet 4 for rear car call request use rcc_req_fl instead of cc_req_fl.
						2. Use g_ucmsk, g_dcmsk, g_rucmsk and g_rdcmsk from hydro.cpp instead of gc_uc_mask, 
						   gc_dc_mask, gc_rdc_mask and gc_ruc_mask. 
						3. Use new subroutines for setting car and hall calls.
4/3/03 3.04 mhd			1. Use hall call serial comm if car serial comm used (
6/4/03 					2. Send Security floor mask configuration flag to galcom.
						3. Restore keyboard interrupt before jumping to powerup.exe program.
06/20/03 				4. Add group car call lockout packet
07/10/03 				5. Send next up and lobby request data to car
7/31/03 3.05 mhd		1. Added code blue serial hall calls
						2. Added ir serial hall calls.
8/11/03 3.06 mhd		1. Increased statusf flag to 32 bits. Added g_statusf2 int in group_io. Changed comm data to match.
						2. For ICR input, use top floor + 1 for input location.	(changes init_grp_com)
						3. Send option flag for inter-group emergency power.
						4. Delete ICR in old comm.
8/29/03 3.07 mhd		1. Send option flag for equake, att and loadweighing.
9/23/03 3.08 mhd		1. Send comm packet 52 and 53 for cars 5 and 6.
						2. Send video car position to galcom device.
10/2/03 3.09 mhd		1. Use (sercom & 1) != 0 so car serial comm can be used without hall serial comm.  Also check
						   gc_hall_io_dev for serial hall i/o devices.
						2. Added packets 54 and 55 to transmit the ionames and job setup information to the galcom user
						   interface.
10/17/03 3.10 mhd		1. Changed slowdown check when setting nextstop. Use procf flag with (hsf == 0) to determine slowdown since
						   DF is used for FF on DC-SCR drives.
12/22/03 3.17 mhd		1. Add new security_type[] = 4 for seperate car call and hall call lockouts.  Use the rear car call 
						   lockouts for the hall calls.
						2. Send next_up operation flag from the group to each car.
1/29/04	3.19 mhd		1. Added hall call cross assignment.
2/26/04 4.00 mhd		1. Used union declarations in pack and unpack routines to optimize code for speed and size.
						2. Added two user/lobby/motor room display comm ports for comm memory board.
4/21/04 5.00 mhd		1. Changed iodata index to use values 0 - 83.
6/4/04 v5.01 mhd		1. Changed gc_loss_cnt for car 2 or higher.
						2. Reset fifo transmitter and receiver during reset. Also reset transmitter fifo at start of transmission and
						   reset receive fifo when preparing to receive.
7/30/04 v4.02 mhd		1. Changed entire fault code mechanism.
9/2/04 v4.03 mhd		1. When calling grp_com_tx_start from receive, don't disable interrupts because interrupts are already disabled.  
						   The problem is that interrupts are then enabled while the receive routine is still at the interrupt level.
1/10/05 4.04 pn			1. Added field variable to change a car other than 1 or 2 to become the master
3/10/05 4.08 mhd		1. Added Tranbuf_Full flag if less than 50 bytes left in the transmit buffer.
3/21/05 4.08 mhd		2. Added gc_rsec and set gc_sec and gc_rsec from the first location of the security input.
4/9/05 4.09 mhd			1. Used rbufsize and tbufsize to change the size of the transmit and receive buffers.
4/23/05 4.11 mhd		1. Added clearing gc_rsec if comm loss to security board and clear gc_sec on first board comm loss only. (input moved)
4/24/05 v4.11 mhd 		2. Changed Security_Type to allow both car call lockouts and group car call lockouts.
5/1/05 V4.19 mhd		1. Changed timing sequence for pre-torque on DSD-412 drive.
6/17/05 v4.22 mhd		1. Changed in drv_com, multiply Vel_Step_Clamp by 3.0 instead of 2.0 to allow a faster decel during an emergency stop.
6/22/05 v4.23 mhd		1. Before returning to pwrup routine, enable DOS interrupts.
6/22/05 V4.23 mhd		2. Added local gettime, getdate, settime setdate so that the 18.2 msec DOS interrupt could be disabled.
7/11/05 v4.23 mhd		3. Start pre-torque sequence for DSD drive when pre_torque_set flag is = 1.
7/11/05 v4.24 mhd		4. Change device sequence index in group comm to talk to hc board after 2 car devices so the hc devices do not time-out.
7/14/05 v4.25 mhd		1. Added default statement at the end of all switch case commands is there were none.
7/27/05	V4.26 mhd		1. Changed Vel_Step_Limit = Vel_Step_Clamp * 6.0 instead of * 3.0 for DSD Drives because of the 15 msec increment. Use
                           Vel_Step_Clamp * 4.0 for the HPV drive on 10 msec increment.
8/3/05 v4.27 mhd		1. Changed comm update time for DSD from 15 msec to 10 msec at the recommendation of Don Vorath of Magnetek.
						2. Changed the pretorque sequence to match the changes to the GEN II DSD drive software.
10/31/05 v4.37 mhd		1. Added g_ss_stat flag to grp_com pack and unpack.
11/4/05 V4.38 mhd		1. Added packets 61 and 62 to transfer 60 additional field variables to user interface.
11/23/05 v4.41 mhd		1. Change DIO1 port to input and use DIO2 port for outputs.
12/12/05 v4.43 mhd		1. Correct byte count in pack_grp_com packet 1. Change from 178 to 180.
2/1/06 v4.50 mhd		1. Changed c_RCB2_addr to c_RCB1_addr in unpack routine (fix mistake).
2/9/06 v4.54 mhd/pn		1. Fixed bug where gc_rsec would not be sent correctly over the communication bus. 
4/24/06 v4.64 mhd		1. Added grp_codebf[] to group to car comm.
5/19/06 v4.67 mhd		1. Modified hpv pretorque sequence to delay the serial run command to the drive until after the torque is set.
6/22/06 v5.00 mhd		1. Corrected index in packet 62 to receive variables 120 to 180.
7/12/06 v5.00 mhd		1. Added packets 63 and 64 to set digital speed clamps.
7/21/06 v5.00 mhd		1. Modified packet 63 to send only 1 "T" at the beginning of the packet instead of 2. Also for UT and DT 
                           speed send 5 values instead of 6.
						2. Corrected checksum calculation in packet 63.
						3. Change packet 64 to 65 since 64 is the @ symbol.
						4. Add car[speed] in packet 50.
8/8/06 					5. Corrected packet 62 for index for checksum loop to be < 120 instead of 60.
						6. Corrected packet 65 for index for checksum loop to be 12 instead of 5. 
8/9/06 v5.01 mhd		1. Return encoder velocity in packets 20 and 21 instead of analog ADC value.
						2. Corrected packet 63 to send the UTS and DTS speed values.
						3. Disable RTS and DTS on com2 if car is not the dispatcher.  When car becomes the Dispatcher, DTR allow
						   the car to control the ids or motor room display on a 485 bus.
						4. Corrected car call data to group in packet 17 when car is not car 1.
						5. Deleted com_upd_nmb[] also allow com2 to send nextup data in packe 17 without the galcom version and revision.
						6. Default com_nmb_fl = cons[grtopf], com_nmb_io = 85 and com_nmb_car = cons[nmbcars]. Don't worry about backward
						   compatibility to old galcom devices
8/17/06 v5.02 mhd		1. Round up when sending velocity and enc_vel to group by adding 0.5 before converting value to an integer.
8/30/06 V5.03 mhd		1. Added tfltdelay timer to wait for fault packet for non-group car to user interface.
						2. Made corrections to packets 9 and 48 to send faults to user interface.
9/5/06 v5.04 mhd		1. Made corrections to packets 9, 10, 47, 48 and 58 for user interface.
						2. Deleted (short) conversion in packet 51.
9/8/06 v5.04 mhd		3. Check for valid car numbe before setting car call in packets 29 and 30.
10/13/06 v5.06 mhd		1. Change slave minimum time-out to 20 msec from 30 msec.
10/31/06 v5.06 mhd		1. When ir riser is being or'ed with the standard riser, send out the standard hall call set and cancel
                           to the ir board when not in ir mode. see pack_grp_com packet	#18.
11/6/06 V5.08 mhd		1. Changed Cam_Mode finished valued from 10 to 16 and changed Menu_Level check from 7 to 10.
						2. Use Dmd_Velocity for drive demand instead of Velocity. Also use Dmd_Velocity instead of Velocity 
						   for display data. Dmd_Velocity is clamped by limit_velocity routine.
11/30/06 v5.09 mhd		1. Changed the full_update[i] flag to only set for one car at a time.
1/8/07 v5.11 mhd		1. Changed all double data types to float for the djgpp compiler.
2/1/07 v5.16 mhd		1. Changed all hall call input long word to an array of [2] such as gc_uc to gc_uc[2] to allow
 						   for 60 floors.
						2. Added code to look for added board for floors > 29.
2/5/07   	 mhd		3. Modified car to group comm to allow for 60 floors: Changed pack and unpack, initialization and comm control.
2/15/07	v5.18 mhd		1. Request version and revision and send version and revision to and from car.
						2. Correct Unpack of version and revision in packet 12.
3/8/07 v5.19 mhd		1. Made ids_ind, ids_rtl, ids_heof and ids_ad_off array values and send data to each car.
5/1/07 v5.23 mhd		1. Changed com3 (drive comm) to use the 16 byte fifo buffer for transmit and receive.  This was necessary
                           to run the KEB drive at 57600 baud. Also use fifo on all drives.
6/6/07 v5.25 mhd		1. Send asvto_pos[] to car from group. 
						2. Corrected index (inserted ix) in asvto_pos[dev] = gc_rcv_buf[ix+31].
7/2/07 v5.29 mhd		1. Add car assignment to floor security flag from group to car.
7/20/07 v5.31 mhd		1. Enable MSR interrupts (RI) if cons[dpp_upd] == 2 for drive comm port.
9/27/07 v5.37 mhd		1. Packets 69 and 70 to transfer 180 new variables for a total of 360.
9/28/097 5.37 mhd		2. Run KEB drive at 38400 baud and poll every 15 msec.
						2. Changed c_RCB2_addr to c_RCB1_addr in unpack routine (fix mistake) again.
						3. Transfer codeblue variable by checking g_cb_fl and g_rcb_fl.  Set 0x80 bit if rcb.
10/3/07 5.38 mhd		1. Transfer varibles from group to car: g_irup_hall_call[0], g_irdn_hall_call[0], g_rirup_hall_call[0]
						   and g_rirup_hall_call[0] also attempt to keep comm varsions backwards compatible.
						2. Transfer variables grp_Car_Stat[dev].g_cancel_irup,grp_Car_Stat[dev].g_cancel_irdn,
						   grp_Car_Stat[dev].g_cancel_rirup and grp_Car_Stat[dev].g_cancel_rirdn from car to group.
						3. Added IR i/o when (cons[Second_Riser] != 0).
10/30/07 v5.38.3 mhd	1. Send flag to car that group has not hall calls (for attendant service).
11/12/07 v5.39 mhd		1. Correct init_grp_com to not poll address 8 (rear ir call) when second riser and calls less than 25 floors. 
						2. Allow VIP to use the code blues calls (if not used) or the rear code blue calls (if not used). 
12/19/07 v5.39.8 mhd	1. Added no_wheelchair_cap bit in g_control.
1/15/08 v5.40.2 mhd		1. Cancel data and set data for the 29 - 36 up was being sent from the wrong cancel array (index of 1 instead of 0).
2/1/08 v5.40.4 mhd		1. Delete using c_RCB4_addr since calls would be on c_CB4_addr for vip and code blue.
2/20/08 v5.41 mhd		1. Modify pulse update interrupt from dz (com3int) to clear interrupt anytime dz interrput occurs.
						2. Cancel data and set data for 29 - 32 rear, ir and rear ir up calls were being set from the wrong cancel array (index of 1 instead of 0).
3/21/08 v5.42.3 mhd		1. Use nmb_uc,nmb_dc and nmb_io variable to calculate the number of hall call boards used when special i/o's added.
4/15/08 v5.43.3 pn		1. Added option for cons[ids] bit 2 for adding an additional GPIO board for 
						   montoring of external signals through lift-net.
4/18/08 v5.43.6 mhd		1. Use group I/o board for group car call security override. (Security_Type & 0x80). Since Security_Type & 0x20 uses the same boards check
                           for (Security Type & 0xA0) instead.
5/16/08 v5.44 mhd		1. Added packets 27,28 and 29 for hall call driver boards.
6/11/08 v5.44.2 mhd		1. Modified packet 17 to allow for more than 32 floors.  Also clear comm_mode flag after each packet.
6/18/08 v5.44.5 mhd		1. Mofified drive comm for half duplex RS-485.
						2. Added record_fault() for serial hc driver and button boards.
						3. Change tbufsize from 500 to 600 for 8 car 60 floor job.
7/17/08 v5.45.1 mhd		1. Changed (cons[dpp_upd] == 2) to ((cons[dpp_upd] & 0x02) != 0).
7/20/08 v5.45.2 mhd		1. Use ((fvars[fvppupd] & 0x02) != 0) instead of ((cons[dpp_upd] & 0x02) != 0) to enable interrupts.
9/2/08 v5.45.8 mhd		1. Send second byte for Current_fault in Com_mode = 3 if galcom_ver > 6.  Also send in Com_mode = 26, 27 etc.
						2. Send second byte for g_cur_flt in car_data_to_group (both pack and unpack).  Also added 4 spare bytes.
						3. In type 2 of uncompress() routine the statement: for(j=(k+72);i<(k+144);j++) uses i as the index check
						   instead of j.  It was corrected to j in this version.
						4. Modified packets 61, 62, 69 and 70 to transmit and receive 120 parameters each with galcom version 6 and higher.
						5. When checking ck_sum against bbb, type cast bbb to (int16) to prevent the compiler from typing ck_sum to int in packets
						   2, 62, 65 and 70. 
9/22/08 v5.45.9 mhd		1. Send high byte of fault code in packets 9 and 48.
10/17/08 v5.46.1 mhd	1. Jump to powerup.exe routine same as a traction of hydro running on ts5300 cpu.
10/30/08 v5.46.4 mhd	1. Changed car_has_fault from fault code to true or false.
						2. Correct index i in packets 9 and 48 for i+=9 to i+=10 if galcom_ver > 6.
11/18/08 v5.46.6 mhd	1. Changed baud rate for drive to include 9600 and 115.2K.
						2. Added modbus variable for delta drive.
						3. Modified packets 18,20,21,40,50,51,54-60,65, and 66 in comm_diag routine for high rise car and user interface corrections.
11/25/08 v5.46.7 mhd	1. Use table to select error code for individual serial hall call board (see packet 29).
12/20/08 v5.46.8 hdl	1. Increased tbufsize from 600 to 1000.
1/9/09 v5.47 mhd		1. Transmit and receive any_ir_hall_call flag and ir_car flag from group to car. 
1/13/09 v5.47.2	mhd		1. Added nmb_hcio_brds to send in group init packet 50.
						2. Use c_GPIO9_addr for extended group io that are read into gpi[] and written out on gpo[].
						3. Modified packet 58 to use nmb_hcio_brds or (nmb_hcio_brds * 3) for the index.
1/20/09 v5.47.3 mhd		1. Added version and revision for serial hall call boards.
3/2/09 v5.47.7 mhd		1. Added control mechanism and packets (#5,6,7,31,32 and 33) to transmit security codes from group car to all the other cars.
						2. Send activations timer data for individual cars to each car from group car.
3/11/09 v5.47.10 mhd	1. Added car call pushbutton security comm packets.
						2. Made receive buffer 1000 bytes
						3. Change group comm security buffer name to start with gc and added comm security buffer for com0 and com1.
3/23/09 v5.47.11 mhd	1. Addes service timers to packet 71 and 72.
4/21/09 v5.48.3 mhd		1. Return -1 in get_pkt_status() if packet is not in the status que.
4/22/09 v5.48.4 mhd		1. Added red, green and blue, on and off intensity level in packet 28.
						2. Corrected dev_off_line() for c_RHC6_addr and c_RIRHC6_addr with serial boards by placing rear call cancel in c_HC6_addr and
						   c_IRHC6_addr.
						3. Modified packets 34, 36, 39 and 72 to use packet number and 100+packet number to bracket the packet.
						4. Send request for packet 35 instead of 34 in packet 39.
						5. Modified packet 9: changed COM1 to com_nmb.
5/15/09 v5.48.6 mhd/jd	1. Corrected index in packet 71.
5/20/09 v5.48.7 mhd/pn	1. Set fault_dev for device reset in packet 12.
						2. Send dual serial riser i/o's in packet 51 and send dual serial riser flag in packet 50.
5/28/09 v5.48.8 mhd		1. Modified packets 27 to serial hall call driver to mask primary and secondary risers.
						2. Send additional color control information in packet 28.
6/12/09 v5.48.11 mhd	1. Set the fault_device = dev before each record_fault(f_com_config) routine call.
8/8/09  v5.49.7 mhd		1. Changed HcbControl to an array to send control data to all hall call driver boards.
						2. Changed check for (bc > 30) to (uc_len > 20) in unpack_group_data_to_car().  uc_len is returned in uncompress_zero() so you know
						   there is enough data to unpack the remainder of the packet.
8/24/09 v5.49.10 mhd	1. In group comm pack and unpack, sent only 1 bit for next_up and use the msb of the byte being sent for in_grp_sabbath.
9/29/09 v5.50.3 mhd		1. Send ir_active flag from group to car.
10/8/09 v5.50.4 mhd		1. Added user_com_ver[],user_com_type[] in packet 49 (both from galcom or GALileo) and com_version in packet 40 (controller comm version).
						2. Set g_ins_status[car] from ins_status.
						3. Send g_ins_status[car] to galcom and galileo. see packets 3,17,26,27,28,52,53,67,68.
						4. Corrected bug in uncompress() by changing index from 150 to 6 for update type 2.
						5. Added L1, L2, L3 and Door Voltage.
						6. Added packets from 73 to 82.
						7. Renamed g_fire_status[] and g_ins_status[] to grp_Car_Stat[i].g_fire_status and grp_Car_Stat[i].g_ins_status.
						8. Renamed grp_Car_Stat[i][g_statusf] and grp_Car_Stat[i][g_statusf2] to grp_Car_Stat[i].g_statusf1 and grp_Car_Stat[i][g_status1_h].
						9. Added sending statusf2 and power_status to packets 3,17,26,27,28,52,53,67,68 and 82.
						10. Added sending grp_Car_Stat[i].g_ss_stat (safety_string_status) in packets 3,17,26,27,28,52,53,67,68 and 82.
						11. Added grp_Car_Stat[i].g_pos in the beginning of packet 82.
						12. Added sending Voltage_Dev_Online[] in packet 81. 
						13. Added Galileo control variables to packets 83 and 84
						14. Send Galileo car control variables from group to car - ui_ind, ui_rtl, ui_heof, and ui_ad_off
						15. Send ccl and ccrl instead of cc and ccr for car call display on the galcom and galileo packet 17.
						16. Also invert the logic on ccl and ccrl and quallify with Car_Not_Talking[].
						17. Corrected index in packet 84 for ui_ind[],ui_rtl[], etc.
10/30/09 v5.50.6 mhd	1. Send 4 spare bytes from group to car.
1/15/10 v5.51.5 hdl		1. g_ids_return to handle the operation for liftnet emergy recall services. Used one spare byte from the car to car comm
							to pass the information on liftnet emerngency recall services. Six bits for the floor, one bit for return type and one bit for door status
3/11/10 v5.51.14 mhd	1. send sec_fl_ctrl[] variable in spare_byte from group to car for security control.
4/28/10 v5.52.4 jd/mhd	1. Modified comm_diag() packets 9, 17, and 48 to correct sending fault to user interface.
						2. Added comm_diag() packets 85-87 to transmit extended fault.
						3. Modified pack_grp_com and unpack_grp_com packet 9 to correct sending fault data from car to group.
						4. Added pack_grp_com and unpack_grp_com packet 36 to correct sending fault data from car to group.
4/30/10 5.52.6 mhd		1. Zero security mask when not a standard hall call device in packet 27 (code blue and vip).
5/3/10 5.52.7 mhd		1. Added error code for hall call low supply voltage.
6/14/10 5.52.9 mhd		1. Modified packets 27,28 and 34 to send correct hall light timing, checksums and code blue/vip set and cancel flags.
8/1/10 v5.52.19 mhd		1. Added pack and unpack for code blue second riser boards c_CB1SR_addr to c_RCB2SR_addr. 
8/19/10 v5.52.25 mhd	1. Corrected bug in packet 17 to send setup data to c_GPIO2_addr through c_GPIO5_addr and c_GPIO11_addr.
9/16/10 v5.52.30 jd		1. Added packets 88 and 89 to transfer 120 new variables for a total of 480.
10/27/10 v5.52.41 mhd	1. Added new iodata in car to group comm.  Corrected count for k=191 to k=206 instead of 203 (191+12+3) in pack_car_data_to_group(), it seems to have left out the last
        				   three spare bytes.
						2. Changed comm_no_io from 85 to 97 for standard cars and from 145 to 157 for high rise cars.  Also changed com_nmb_io_brd from 29 to 33 and from 49 to 53.
11/11/10 v5.52.46 mhd	1. Send variables for g_nmbcc, g_topcc, and g_botcc in car data to group if (cons[SecFlCfg] == 4) as well as for vip.

2/9/2011 v5.53.00 jd	1. add user_com_ver[com_nmb] = 3 for sending interim version to galcom and galileo

2/25/11 v5.53.04 mhd    1. Delete using c_RCB4_addr and c_RCB2_addr when Checking the range for devices

3/11/11 v5.53.07 hdl	1. changed #define c_com_no_dev 67 for all projects low rise and high rise as well

5/20/11 v5.53.20 WC     1. Added Packet	 37 in pack_grp_com() and unpack_grp_com() to allow Car1  to send Galileo information to the rest of the group Cars
						2. Modify Packet 77 in Comm_diag()  to put packet 37 into put_pkt_req().
						3. Added ui_cc_sec_prev[][]  to retain previous value of ui_cc_sec[][].
7/12/11 v5.54.15 mhd	1. Added transmit comm timer to check for transmit comm failure and restart comm.
						2. Modify source check in com1int and com2int to match com3int.
						3. Put check for com1 or com2 trasmit buffer locking up and restart com.
7/18/11 v5.54.16 WC     1.  Added Com_Mode = 0 in  case 3  and 4  of Comm_Diag()
						2. Added a new Packet 90  to Comm_Diag to allow Redlion to reset Controller Comm
12/12/11 v5.54.44 WC    1. Added packet 91 in comm_diag() to send Galileo Alarm Array	

1/9/12	v5.54.48 WC		1. Added packet 92 to comm_diag() to send group fault information 

1/11/12 V5.54.49 WC     1. Added packet 93 to 	comm_diag() to get group VIP information 
7/30/13 v7.0.15 mhd		1. Corrected (comnmb == 1) to (comnmb == USR1COM) and (comnmb == 2) to (comnmb == USR2COM) since USR1COM = 0 and USR2COM = 1.
						2. Removed clearing the Receive enable (USR1port.CR1.B.RE) when DMA transmit is started.
						3. Had to set EDMA.TCD[USR1dmaChTx].INT_MAJ = 1 to use the transmit DMA Channel with interrupts
						2. Since galcom() routine uses DMA, do not call sendc(). Note that galcom stuffs data driectly into tranbuf[] array and is always indexed at zero..
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.

10/17/14 v7.1.17 mhd	1. Modified changes from galaxy 3 version 6.1.0 to 6.0.10.  See below:
						v6.1.05 hdl		1. Changed initialization of comm2. USR2txEn was added when DL20 software was introduced in version 6.0.52 and it affected the liftnet.
													Liftnet packets will be received but no sent if you were using a GAL-1045 board (232 to 485).

10/23/14 v7.1.19 mhd	1. Always zero the last que item when taking data out of que (see get_pkt_req()).
11/19/14 v7.1.21 mhd	1. Modified sendc() to send comm data back through galcom() usint TERMCOM port.
						2. Added arrays for TERMCOM port.
*/
