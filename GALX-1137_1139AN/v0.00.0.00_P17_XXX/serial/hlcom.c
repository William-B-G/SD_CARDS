//  Serial Integroup Emergency Power Routine

#define d_HLCOM 1
#include "global.h"

extern unsigned char RS_485_RxBuf[];
extern int16 Add_msgx_First_TimeOnly(char *msg, int16 MsgTag_0_39);
extern uint8 Hall_Lantern_Select;
extern uint8 Hall_Lantern_floor_A;
extern uint8 Hall_Lantern_floor_B;
extern uint8 Hall_Lantern_Data_A;
extern uint8 Hall_Lantern_Data_B;
extern uint8 Hall_Test_Data;

extern void SnapShot(void);
extern int16 TimeForTheSnapShot(int16 dummy);
extern void Clear_Msg_Board(void);
extern int16 Log_an_event(char *msg);

/*		Hall Lantern Comm Description
		Rs-485 half duplex
		8 data bits
		1 start bit
		1 stop bit
		No parity sent on address

		Comm Protocol
		Master  						Hall Lantern      
		START '['                		'['				
		TO ADDRESS						FROM ADDRESS
		Packet Marker ':'				':'	
		Packet #						Packet #		
		Length							Length			
		DATA 1                  		DATA 1   	      
		DATA 2                  		DATA 2            
			...                    			...
		DATA LAST               		DATA LAST         
		LRC CHECKSUM               		LRC CHECKSUM      
		END	']'							']'				

		
		Transmit buffer is packed exactly as data is transmitted 
		so the byte count is the number of bytes placed in the hc_trm_buf[]

		There is no receive data.

		Packets are not tested for a fixed length but for the transmitted
		length being equal to the actual length.


	Packet/Function DESCRIPTION

	PACKET# 	TO 			FROM		BC			DESCRIPTION
		25		HL			CPU			8	   		Message #25: PI and Lantern Data
												  	Car position		Binary position
												  	Ascii PI 0			Ascii Position low byte
												  	Ascii PI 1			Ascii Position High byte

													Lantern Status		Bit 7: LUP (lantern up)
																		Bit 6: LDN (lantern down)
																		Bit 5: RLUP (rear lantern up)
																		Bit 4: RLDN (rear lantern down)
																		Bit 3: UDA (up direction arrow)
																		Bit 2: DDA (down direction arrow)
																		Bit 1: User Message Bit 1
																		Bit 0: User Message Bit 0

												  	Motion Status		Bit 7: CULR UPREF (up preference)
												  						Bit 6: CDLR DNPREF (down preference)
												  						Bit 5: CUL run (car running)
												  						Bit 4: CDL leveling (car leveling)
												  						Bit 3: Out of Service
																		Bit 2: Inspection
																		Bit 1: Independent
																		Bit 0: unused

													Fire Status			Bit 7: PH2 (fire phase 2)
																		Bit 6: PH1 Complete (fire phase 1 complete)
																		Bit 5: PH1 Recall Main (fire phase 1 recall active)
																		Bit 4: EMP Recall (emergency power recall active)
												  						Bit 3: EMP Recall Complete (emergency power recall complete)
												  						Bit 2: Power Loss (Normal power is out)
												  						Bit 1: Imminent Shutdown
																		Bit 0: PH1 Recall Alternate (fire phase 1 recall active)

													Door Status 		Bits 3,2,1,0
												  						doorf = 0 doors closed no action
												  						doorf = 1 doors are opening
												  						doorf = 2 doors are dwellingse)
																		doorf = 3 doors are closing
																		doorf = 4 doors are nudging closed

																		Bits 7,6,5,4
													  					rdoorf = 0 doors closed no action
													  					rdoorf = 1 doors are opening
													  					rdoorf = 2 doors are dwelling
													  					rdoorf = 3 doors are closing
													  					rdoorf = 4 doors are nudging closed

													Misc Status			Bit 7: Activate speech
																		Bit 6: At Lobby (car at lobby floor)
																		Bit 5: Overload (load overload)
																		Bit 4: Load Bypass
																		Bit 3: EMT (Emergency Medical Technician Service)
																		Bit 2: Code Blue
																		Bit 1: Seismic
																		Bit 0: User Message Bit 2 

																		User Message Bits 2,1,0
																						  0 0 0	No message
																						  0 0 1 Message 1
																						  0 1 0 Message 2
																						  0 1 1 Message 3
																						  1 0 0 Message 4
																						  1 0 1 Message 5
																						  1 1 0 Message 6
																						  1 1 1 Message 7
																						   
*/

int16 Time_4_snap_shot = 0; // wbg 2018-4-28

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

// ***************************************************
//        Serial Hall Lantern Variables
// ***************************************************

int16 hl_com_tx_cnt = 0;					// transmit count
int16 hl_com_rx_cnt = 0;					// receive count

#define c_HL_addr 20

#define c_hl_buf_size 30

// ***************************************************
//        Serial Hall Lantern Function Declaration
// ***************************************************
void hl_com(void);
void pack_hl_com (int16 pkt, int16 dev);
int16 receive_hl_com (void);

// ***************************************************
//        Serial Hall Lantern Pack Data Routine
// ***************************************************

void pack_hl_com (int16 pkt, int16 dev)  // pack_hl_com(25, 20);
{  // tagwbg 19
	int16 i,len;
	int16 valid_packet = 0;
	unsigned char chksum = 0;
	static int16 ZYM_position= 0;

    
	switch (pkt)
	{
	    case 22:  // tagwbg 22 
            for(i=0; i< 12; i++)
            {
                sx_tranbuf[SE1COM][i] = 0;    
            }
            sx_tranbuf[SE1COM][0] = 'A'; // 65 = 'A'; 91 = '['        	
        	
        	len = 12;
			valid_packet = 1;
			break;
				
	    case 23:  // tagwbg 22 
            for(i=0; i< 12; i++)
            {
                sx_tranbuf[SE1COM][i] = 0;    
            }
            sx_tranbuf[SE1COM][0] = 'B'; // 65 = 'A'; 91 = '['        	
        	
        	len = 12;
			valid_packet = 1;
			
	        break;	
	    case 24:  // tagwbg 22 
            for(i=0; i< 12; i++)
            {
                sx_tranbuf[SE1COM][i] = 0;    
            }
            sx_tranbuf[SE1COM][ZYM_position] = 'A'; // 65 = 'A'; 91 = '['
        	
        	ZYM_position++;	
        	if(ZYM_position >12) ZYM_position = 0;
        	
        	len = 12;
			valid_packet = 1;
			
	        break;
	        
		case 25: 								// Send status information
		    
			sx_tranbuf[SE1COM][0] = '['; // '[';	// Start Character
			sx_tranbuf[SE1COM][1] = (uint8)dev;	// To Address
			sx_tranbuf[SE1COM][2] = ':';		// Packet marker
			sx_tranbuf[SE1COM][3] = 25;		// packet number
			sx_tranbuf[SE1COM][4] = 13;		// length
//			if (cons[Xref_PI_Pos] == 1)
            if(Hall_Lantern_Select == 1)
				sx_tranbuf[SE1COM][5] = consB[23];  // Hall_Lantern_floor_A; // xpi_pos[pi_pos];
			else
				sx_tranbuf[SE1COM][5] = consB[23]; // Hall_Lantern_floor_B; // pi_pos;
			if ((servf == 25) && ((fvars[fvsabbdis] & 0x01) != 0))
			{ // Never gets here. wbg 2018-4-28 @ 9 AM ( 1137 / 1139 tester )
//			    SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25
				sx_tranbuf[SE1COM][6] = 'S';
				sx_tranbuf[SE1COM][7] = 'B';
			}
			else 
			{
				sx_tranbuf[SE1COM][6] = flmrk[pi_pos][0];
				sx_tranbuf[SE1COM][7] = flmrk[pi_pos][1];
			}
			if((alrm_pi_stat == 1) && (timers[tsec] != 0))
			{
			    // Never gets here. wbg 2018-4-28 @ 9 AM ( 1137 / 1139 tester )			    
				sx_tranbuf[SE1COM][6] = 'A';
				sx_tranbuf[SE1COM][7] = 'L';
			}
			
			if(Hall_Lantern_Select == 1)
			    sx_tranbuf[SE1COM][8]  = Hall_Test_Data; // consB[24]; // Hall_Lantern_Data_A; // pi_lant_stat;
			else sx_tranbuf[SE1COM][8] = Hall_Test_Data; // consB[26]; // Hall_Lantern_Data_B; // pi_lant_stat;
			sx_tranbuf[SE1COM][9] = pi_motion_stat;
			sx_tranbuf[SE1COM][10] = pi_fire_stat;
			sx_tranbuf[SE1COM][11] = pi_door_stat;
			sx_tranbuf[SE1COM][12] = pi_misc_stat;
			len = 13;
			valid_packet = 1;
			
			break;

		default:
			break;				
	}

	if (valid_packet == 1)
	{
		sx_tranbuf[SE1COM][4] = (uint8)len;
		chksum = 0;
		for (i=0;i<len;i++)
		{
			chksum += sx_tranbuf[SE1COM][i];
		}
		chksum = (uint8)(~chksum + 1);
		sx_tranbuf[SE1COM][i++] = chksum;
		sx_tranbuf[SE1COM][i++] = ']';		// end character
		SnapShot();
		sx_enable_tx(SE1COM);
		se1_tx_dma_start(len+2);
		hl_com_tx_cnt++;
 //       sprintf(src,"06: Just sent hal cm");
 //       Add_msgx_First_TimeOnly(src,  5);	
//        SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25 	
	}
}


// ***************************************************
//        Serial Hall Lantern UnPack Data Routine
// ***************************************************
int16 receive_hl_com (void)
{  // tagwbg 18		
  	int16 rbempt=0,rx_cnt=0;
  	int16 j = 0;
	unsigned char rcv_char = 0;

// sx_rbs[SE1COM] = 0;

    for(j=0;j<10;j++)
    {
        RS_485_RxBuf[j++] = sx_recbuf[SE1COM][j];    
    }
/*
  	rbempt = sx_isrxempty(SE1COM);	
  	while((rbempt == 0) && (rx_cnt < 20))
  	{ 
	  	rcv_char = (uint8)sx_asigetc(SE1COM);
	  	if (j < 30)
	  	{
	  	  RS_485_RxBuf[j++] = rcv_char;
	  	  rx_cnt++;   
	  	}
	  	rbempt = sx_isrxempty(SE1COM);	  	
	}
*/	
	return 8; // rx_cnt;
}

// ***************************************************
//        Serial Hall Lantern Comm Routine
// ***************************************************
void hl_com(void)  // wbg_2018_4_27_a
{
	unsigned char pkt_req = 0;
	unsigned char dev_req = 0;
	
//    SIU.GPDO[Z6_LED].R = !SIU.GPDO[Z6_LED].R;  // toggle LED wbg  // wbg_01_2018_4_25
	debug4++;
	if (sx_istxDMAcomplete(SE1COM) == 1) 
	{
		debug3++;
		if (sx_Tranbuf_Empty[SE1COM] == 1)
		{
			receive_hl_com();		// Keep the receive channel clear since we are not using it.
			debug2++;
			SE1txDis; 						// Turn off Transmit driver
			
			if (timers[tsx_txB] > 0)
			{
			    
				timers[tsx_txB] = 0;
				pkt_req = 25;
				dev_req = 20;
				pack_hl_com(pkt_req, dev_req);
				debug1++;
			}
		}
	}
}


void SnapShot(void)
{
  int16 i = 0;
  
    if(Time_4_snap_shot)
    {   
        Clear_Msg_Board();
        Time_4_snap_shot = 0;
    	for(i = 0; i < 15; i++)
    	{
    	    sprintf(src,"%d = %d", i, sx_tranbuf[SE1COM][i] );
    		Log_an_event(src);
    	}
    	
    }
	
}


/* 
Revision History

  9/22/11 v5.54.25 mhd		1. Added comm for InterGroup Emergency Power IGEP.
  11/17/11 v5.54.39 hdl		1. Changes for the IGP comm packets
 3/21/12 v5.54.66 hdl		1. Added igep_car_man_slct_req to the data required for IGEP service
							2. Changed the length of IGEP package to 60 from 50
5/10/12 v5.54.78 hdl		1. Added packet 4 for igep so igep box can pickl communication with car 2.
  8/14/17 V8.0.7 mhd		1. Changed com to use DMA for the transmit.
*/