//  Serial Integroup Emergency Power Routine

#define d_IGEPCOM 1
#include "global.h"


// ***************************************************
//        IGEP (InterGroup Emergency Power) Variables
// ***************************************************

int16 igep_car_recall_order[9]= {0,0,0,0,0,0,0,0,0}; 		// Car return order
int16 igep_car_select_order[9]= {0,0,0,0,0,0,0,0,0};	  	// Car selection order
int16 igep_car_man_sel_req[9]= {0,0,0,0,0,0,0,0,0};	  		// Manual selection request
unsigned char igep_com_online = 1;			// IGEP online
unsigned char igep_online_count = 0;		// IGEP online count
int16 igep_com_tx_cnt = 0;					// transmit count
int16 igep_com_rx_cnt = 0;					// receive count

unsigned char igep_com_seq = 0;				// IGEP comm sequence
unsigned char igep_rcv_seq = 0;				// IGEP receive sequence
unsigned char igep_packet_rcv = 0;			// IGEP comm packet request flag
unsigned char igep_packet_req = 0;			// IGEP comm packet request flag
unsigned char igep_pack_len = 0;			// IGEP comm pack langth
unsigned char igep_unpack_len = 0;			// IGEP comm unpack length 
unsigned char igep_rcvbuf[sx_rbufsize+1];	// IGEP packet receive buffer
unsigned char igep_trmbuf[sx_tbufsize+1];	// IGEP packet transmit buffer
int16 igep_rcv_ix = 0;						// block receive index
int16 igep_trm_ix = 0;						// transmit index
int16 igep_rx_ix = 0;						// packet receive index
unsigned char igep_pkt_len=0;
unsigned char igep_rcv_chksum=0;

void IGEP_comm(void);
void pack_IGEP_com (int16 pkt);
void unpack_IGEP_com (int pkt);
void receive_IGEP_com (void);

/* Emergency Power Control Protocol


Packet Protocol
	Start Byte
	Length
	To/From Device Number
	Packet Number
	Packet Data
	Checksum (includes start byte)

	Data to Car 1 or 2

	Auto Recall Enable
	Run Enabled
	recall order feeder 1
	recall order feeder 2


	Data from Car 1 or 2 to Emergency Power Control
	Car status
		g_empf
		g_pos
		g_servf
		g_firef
		g_fire_status
	recall finished
*/

void pack_IGEP_com (int16 pkt)
{
	int16 i,j;
	int16 valid_packet = 0;
	unsigned char chksum = 0;

	switch (pkt)
	{
		case 1: 								// Send status information
			igep_trmbuf[0] = 0x5B;				// start flag '['
			igep_trmbuf[1] = 0;					// packet length gets added later
			igep_trmbuf[2] = (uint8)pkt;					// packet number
			igep_trmbuf[3] = (uint8)cons[carnmb];		// device number
			j = 4;
 			for(i=1;i<=8;i++)
			{
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_pos;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_empf;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_servf;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_procf;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_firef;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_fire_status;
				igep_trmbuf[j++] = 0;		// no power feeder
				igep_trmbuf[j++] = (uint8)grp_empf[i];
				igep_trmbuf[j++] = 0;		// only one feeder
			}
			igep_trmbuf[j++] = (uint8)recall_finished;
			igep_trmbuf[j++] = 0;		// only one feeder

			igep_pack_len = (uint8)j;			
			valid_packet = 1;
			break;
		case 3:
			igep_trmbuf[0] = 0x5B;				// start flag '['
			igep_trmbuf[1] = 0;					// packet length gets added later
			igep_trmbuf[2] = (uint8)pkt;					// packet number
			igep_trmbuf[3] = (uint8)cons[carnmb];		// device number
			j = 4;
 			for(i=1;i<=8;i++)
			{
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_pos;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_empf;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_servf;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_procf;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_firef;
				igep_trmbuf[j++] = (uint8)grp_Car_Stat[i].g_fire_status;
				igep_trmbuf[j++] = 0;			// only one power feeder
				igep_trmbuf[j++] = (uint8)grp_empf[i];
				igep_trmbuf[j++] = 0;			// only one power feeder
			}
			igep_trmbuf[j++] = (uint8)recall_finished;
			igep_trmbuf[j++] = 0;				 // only one power feeder

			igep_pack_len = (uint8)j;			
			valid_packet = 1;
			break;

		default:
			break;				
	}

	if (valid_packet == 1)
	{
		igep_trmbuf[1] = (uint8)igep_pack_len;
		chksum = 0;
		for (i=0;i<igep_pack_len;i++)
		{
			chksum += igep_trmbuf[i];
		}
		chksum = (uint8)(~chksum + 1);
		igep_trmbuf[i] = chksum;
		igep_trm_ix = 0;
		sx_enable_tx(SE2COM);
		while (igep_trm_ix <= igep_pack_len)
		{
			sx_sendc(SE2COM,igep_trmbuf[igep_trm_ix]);
			igep_trm_ix++;
		}
		igep_com_tx_cnt++;
	}
}

void unpack_IGEP_com (int pkt)
{
	int16 i,j;
	switch(pkt)
	{
		case 1: 
			break;
		case 2:	//packet for car 1
			j = 0;
			ep_auto_recall_en = igep_rcvbuf[j++];			
			ep_auto_select_en = igep_rcvbuf[j++];			
			ep_run_en = igep_rcvbuf[j++];
			for (i=1;i<=8;i++)
				igep_car_recall_order[i] = igep_rcvbuf[j++];
			for (i=1;i<=8;i++)
				igep_car_select_order[i] = igep_rcvbuf[j++];
			for (i=1;i<=8;i++)
				igep_car_man_sel_req[i] = igep_rcvbuf[j++];

			igep_packet_rcv = 1;
			igep_online_count = 10;
			igep_com_rx_cnt++;
			igep_packet_req = 0;
			break;
		case 4:	 // packet for car 2   (if car 2 is dispatcher)
			j = 0;
			ep_auto_recall_en = igep_rcvbuf[j++];			
			ep_auto_select_en = igep_rcvbuf[j++];			
			ep_run_en = igep_rcvbuf[j++];
			for (i=1;i<=8;i++)
				igep_car_recall_order[i] = igep_rcvbuf[j++];
			for (i=1;i<=8;i++)
				igep_car_select_order[i] = igep_rcvbuf[j++];
			for (i=1;i<=8;i++)
				igep_car_man_sel_req[i] = igep_rcvbuf[j++];

			igep_packet_rcv = 1;
			igep_online_count = 10;
			igep_com_rx_cnt++;
			igep_packet_req = 0;
			break;
		default:
			break;
	}
}



void receive_IGEP_com (void)
{
		
  	int16 rbempt=0,endmess=0;
	unsigned char rcv_char = 0;
	static int16 packet;

  	rbempt = sx_isrxempty(SE2COM);

  	while((rbempt == 0) && (endmess == 0))
  	{
	  	rcv_char = (uint8)sx_asigetc(SE2COM);
	  	rbempt = sx_isrxempty(SE2COM);
		switch (igep_rcv_seq)
		{
			case 0:		// Start Byte
				packet = 0;
				if (rcv_char == 0x5B)		//      '['
				{
				    igep_rcv_chksum = 0x5B;		//      '['	
					igep_rcv_seq = 1;
					igep_rx_ix++; 
				}
				else
				{
					igep_rx_ix = 0;
				}

				break;
			case 1:		// get length
				igep_pkt_len = rcv_char;
				if (igep_pkt_len < 60)
				{
					igep_rcv_chksum += rcv_char;
					igep_rcv_seq = 2;
					igep_rx_ix++;	  
				}
				else
				{
					igep_rcv_seq = 0;		// invalid length
					igep_rx_ix = 0;
				}
				break;
			case 2:
				packet = rcv_char;
				igep_rcv_chksum += rcv_char;
				if(packet < 5)
				{	
					igep_rcv_seq = 3;
					igep_rx_ix++;	  
				}
				else
				{
					igep_rcv_seq = 0;		// invalid packet
					igep_rcv_ix = 0;
					igep_rx_ix = 0;

				}
				break;
			case 3:		// Device number
				igep_rcv_chksum += rcv_char;
				if (rcv_char == 9)
				{
					igep_rcv_seq = 4;
					igep_rcv_ix = 0;
					igep_rx_ix++;	
				}
				else
				{
					igep_rcv_seq = 0;		// invalid packet
					igep_rcv_ix = 0;
					igep_rx_ix = 0;
				}
				break;
			case 4:
				igep_rcvbuf[igep_rcv_ix] = rcv_char;
				igep_rcv_ix++;
				igep_rx_ix++;
				if (igep_rx_ix < igep_pkt_len)
				{
					igep_rcv_chksum += rcv_char;
				}
				else
				{
					igep_rcv_chksum += rcv_char;
					igep_rcv_chksum = (uint8)(~igep_rcv_chksum + 1);
					igep_rcv_seq = 5;
				}
				break;
			case 5:
				if (rcv_char == igep_rcv_chksum)
				{		// valid packet
					if(((Dispatcher == 1)&&(packet == 2))||((Dispatcher == 2)&&(packet == 4)))
						unpack_IGEP_com(packet);
					timers[tsx_rxB] = 0;
					igep_rcv_ix = 0;
					igep_rx_ix = 0;
					igep_rcv_chksum = 0;
				}
				igep_rcv_seq = 0;		// finished
				break;
			default:
				igep_rcv_seq = 0;
				break;
		}
		if (igep_rcv_ix > 300)
		{
			rbempt = 1;
			igep_rcv_ix = 0;
		}
	}
	if (timers[tsx_rxB] > 360)
	{ 
		igep_packet_req = 0;
		if (igep_online_count > 0)
		{
			igep_com_online = 1;
			igep_online_count--;
		}
		else 
		{
			igep_com_online = 0;
			igep_online_count = 0;
		}
		timers[tsx_rxB] = 0;
	}
}

// Comm FOR Inergroup Emergency Power
void IGEP_comm(void)
{
	unsigned char pkt_req = 0;

	if(sx_istxcomplete(SE2COM)) // added HERNAN
	{
		if (SE2port.SR.B.TC == 1)
		{	
			SE2txDis; 	
			receive_IGEP_com();
		}
	}

	if (igep_packet_rcv == 1)
	{
		igep_packet_rcv = 0;
		if (sx_Tranbuf_Empty[SE2COM] == 1)
		{
			if (igep_packet_req == 0)
			{
				if(Dispatcher == 1)
					pkt_req = 1;
				else if (Dispatcher == 2)
					pkt_req = 3;
			}
			else
			{
				pkt_req = igep_packet_req;
			}

			pack_IGEP_com(pkt_req);
		
			igep_packet_req = 0;
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
  8/14/17 v8.0.7 mhd		1. Changed comm to interrupt based on transmit.

*/