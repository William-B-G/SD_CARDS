//  Comm Routines
/* User interface communications protocol


Packet Format to the controller:

	Bytes		Data
	1			Start Character 	'P'
	2			Length		(number of bytes transmitted including the start character, the checksum and the end character
	2			Length check (length check = length - 1)		
	1			Packet Number
	n			Packet Data 
	1			Checksum		Checksum includes the length, length check byte, the packet number and packet data (if any)
	1			End Character		'@'

Packet Format from the controller:  (Note that the length and the length check sent from the controller are two (2) bytes each)

	Bytes		Data
	1			Start Character 	'P'
	2			Length		(number of bytes transmitted including the start character, the checksum and the end character
	2			Length check (length check = length - 1)		
	1			Packet Number
	n			Packet Data 
	1			Checksum		Checksum includes the length, length check byte, the packet number and packet data (if any)
	1			End Character		'@'

	
From	To	Packet	R-Pkt	Data

UI		CPU		1				Request user id and password log on
CPU		UI			1				Send user id and password log on status
UI		CPU		3				Request to log off
CPU		UI			3				Send Log In and AD status
UI		CPU		4				Write new user name
CPU		UI			4				User Name write acknowledged
UI		CPU		5				Write new password
CPU		UI			5				Password write ack
UI		CPU		6				Update Date and Time
UI		CPU		7				Send Galcom version 
CPU		UI			8				Send Car version, revision, and job number
UI		CPU		8				Send Restart Comm

UI		CPU			9			Request Encription Key
CPU		UI		9					Send Encription Key

UI		CPU		10				Request Group Initialization Data
CPU		UI			10				Send Group Initialization Data
UI		CPU		11				Request Extended Group Initialization
CPU		UI			11				Send Extended Group Initialization
UI		CPU		12				Request Additional Extended Group Initialization
CPU		UI			12				Send Additional Extended Group Initialization
UI		CPU		13				Request I/O boards used for car
CPU		UI			13				Send I/O boards used for car
UI		CPU		14				Request I/O board input and output locations
CPU		UI			14				Send I/O board input and output locations
UI		CPU		15				Request valid I/O locations
CPU		UI			15				Send valid I/O locations
UI		CPU		16 				Request Group I/O boards used and valid I/O locations
CPU		UI			16 				Send Group I/O boards used and valid I/O locations
UI		CPU		17				Request Car I/O Names per board
CPU		UI			17				Send Car I/O Names per board
UI		CPU		18 				Request Group I/O Names per board
CPU		UI			18 				Send Group I/O Names per board
UI		CPU		19				Request Group I/O board input and output locations
CPU		UI			19				Send Group I/O board input and output locations
					

UI		CPU		20				Request to send the field variables (600 variables are sent in five packets 0 - 5)
CPU		UI			20				Send field variables
UI		CPU		21				Update Field Variables
UI		CPU		22				Request Digital speed clamp settings and terminal limit speeds
CPU		UI			22				Send Digital speed clamp settings and terminal limit speeds
UI		CPU		23				Receive Speed Clamp Settings
UI		CPU		24				Request Live Hoistway Learn info
CPU		UI			24				Send Live Hoistway Learn info
UI		CPU		25				Request to send 1 to 60 adjustable variables
UI		CPU		26				Receive adjustable variables 1 to 60 (respond with packet 26)
CPU		UI			26				Send adjustable variables 1 to 60
UI		CPU		27				Request to send Adjustable variable menu locations
CPU		UI			27				Send adjustable variable menu locations
CPU		UI			28				Send one adjustable variable to User Interface

UI		CPU		29				Request Car Motion Status
CPU		UI			29 				Send Car Motion Status
UI		CPU		30				Request Car I/O and Status
CPU		UI			30				Send Car I/O and Status		
UI		CPU		31				Request Group Data Packet
CPU		UI			31				Send Group Data Packet
UI		CPU		32				Request hall Call I/O data
CPU		UI			32				Send hall Call I/O data
UI		CPU		33				Set Car Call
UI		CPU		34				Set Up Call
UI		CPU		35				Set Down call
UI		CPU		36				Set Rear Car call
UI		CPU		37				Set Rear Up Call
UI		CPU		38				Set	Rear Down Call
UI		CPU		39				Stop auto re-transmission

UI		CPU		40				Request Detail Faults for car
CPU		UI			40				Send Detail Faults for car
UI		CPU		41				Clear Car faults for particular car
UI		CPU		42				Request Job Statistics	
CPU		UI			42				Send Job Statistics	
UI		CPU		43				Clear Job Statistics
UI		CPU		44 				Request ETA data for each car
CPU		UI			44 				Send ETA data for each car
UI		CPU		45				Request Trace Data
CPU		UI			45				Send Trace Data
UI		CPU		46				Send Trace Setup Data
CPU		UI			46				Acknowledge
UI		CPU		47				Request Service and Process flag table
CPU		UI			47				Send Service and Process flag table
UI		CPU		48				Request One Detailed fault
CPU		UI			48				Send One Detailed fault
UI		CPU		49				Set Car call Test/Request Car call test status
CPU		UI			49				Send car call test status

UI		CPU		50 	 			Add Security Code
UI		CPU		51	 			Delete Security Code
UI		CPU		52				Delete All Security Codes Per Floor
UI		CPU		53				Request security code table to CPU
CPU		UI			53				Send Security Code Table from CPU
CPU		UI		 	54				Request Security Code Table to UI
UI		CPU		54	 			Receive Security Code Table from UI
UI		CPU		55 				Request Security Checksum Table
CPU		UI			55				Send Security Checksum Table
UI		CPU		56				Receive Security Checksum Table
UI		CPU		57				Request User Interface Front and Rear hall call security
CPU		UI		 	57				Send User Interface Front and Rear hall call security
UI		CPU		58				Receive User Interface Fron and Rear hall call security
UI		CPU		59				Request UI Front Car call Security for all Cars
CPU		UI		 	59				Send UI Front Car call Security for all Cars
UI		CPU		60				Receive UI Front Car Cal Security for all cars
UI		CPU		61				Request UI Rear car call security for all cars
CPU		UI		 	61				Send UI Rear car call security for all cars
UI		CPU		62				Receive UI Rear car call security for all cars
UI		CPU		63				Request UI Front Hall Call Lockouts for each car
CPU		UI			63				Send UI Front Hall Call Lockouts for each car
UI		CPU		64				Receive UI Front Hall Call Lockouts for each car
UI		CPU		65				Request UI Rear Hall Call Lockouts for each car
CPU		UI			65				Send UI Rear Hall Call Lockouts for each car
UI		CPU		66				Receive UI Rear Hall Call Lockouts for each car

UI		CPU		67				Request Car Trace Variables
UI		CPU		68				Receive Car Trace Variables
CPU		UI			68				Send Car Trace Variables
UI		CPU		69				Request Car Trace Motion Status
CPU		UI			69 				Send Car Trace Motion Status

UI		CPU		70				Request Service Timer settings
CPU		UI			70				Send Service Timer settings
UI		CPU		71				Receive Service Timer settings
UI		CPU		72				Request Line voltage for all cars
CPU		UI		 	72				Send Line voltage for all cars
UI		CPU		73				Request car data for fault and service change detection
CPU		UI		 	73				Send car data for fault and service change detection
UI		CPU		74				Request UI Control Variables
CPU		UI		 	74				Send UI Control Variables
UI		CPU		75				Receive UI Control Variables
UI		CPU		76				Request Group Alarms
CPU		UI		 	76				Send Group Alarms
UI		CPU		77				Request Current Fault for all Cars
CPU		UI		 	77				Send Current Fault for all Cars
UI		CPU		78				Set VIP Floor call
UI		CPU		79				Set Car call run multiple floors

UI		CPU		80				Comm debug keyboard command
CPU		UI			80				Terminal Output Data

CPU		UI			81				Group Traffic Call Data

CPU 	UI			90			CTC CAR TOP CAN COMMUNICATION STATUS
CPU 	UI			91			MRC CAR TOP CAN COMMUNICATION STATUS
CPU		UI			92			GCC CAR TOP CAN COMMUNICATION STATUS
CPU		UI			93			Auto Hoistway Learn
								
				
				
						

*/

#define d_GALCOM 1
#include "global.h"

#define MAIN_CPU 1
#define SPB_CPU 2
#define NTS_CPU 3

#define Adjuster_Access		218 
#define Mechanic_Access		172
#define Service_Access		87
#define Building_Access		61
#define Monitor_Access		20


// Temporary variables until we get the accelerometer on the car to work

int16 accel_x = 0;
int16 accel_y = 0;
int16 accel_z = 0;


int16 galcom_req[2] = {0,0};
int16 galcom_req_upd[2] = {0,0};	// set when galcom_req is updated
int16 com_flt_ix[2]= {0,0};
int16 com_flt_type[2] = {0,0};
int16 flt_offset_ix[2] = {0,0};
int16 com_req_car_flt[2] = {0,0};
int16 com_car_req[2] = {0,0};
int16 com_access_level[2] = {0,0};
int16 com_access_level_pk3[2] = {0,0};
int16 com_access_packet[2] = {0,0};
int16 com_access_status[2] = {0,0};	// access status bit 0: 1 if AD_status failed
									//               bit 1: 1 if Access level failed
int16 com_io_brd[2] = {0,0};
int16 com_fvar_block_ix[2] = {0,0};
int16 com_rst_flt_ix[2] = {0,0};
int16 com_loop_ix[2] = {0,0};
int16 com_svc_tmr[2] = {0,0};
int16 com_nmb_fvar[2] = {0,0};
int16 com_fvar_ix[2][60] = {0,0};
int16 com_fvar_cpu[2] = {0,0}; 
int16 tx_delay_cnt[2] = {0,0};
int16 com_menu_req[2] = {0,0};
int16 com_trace_ix[2] = {0,0};
int16 trace_offset_ix[2] = {0,0};
int16 com_trace_cmd[2] = {0,0};
int16 com_trace_condition_1[2] = {0,0};
int16 com_trace_condition_2[2] = {0,0};

int16 com_cc_test_cmd = 0;
int32 com_cc_test_mask[2] = {0,0};


int16 reboot_seq = 0;
int16 send_reboot_cmd = 0;


int16 galcom_len[2] = {0,0};
int16 galcom_len_ck[2] = {0,0};
		

int16 lcd_fvar_cpu[2] = {0,0}; 
int16 lcd_fvar_ix[2] = {0,0};

int16 com_fvar_req[2] = {0,0};

int16 cc_latched_ack = 0;
 
bool AD_status = 0;

uint32 key[4] = 
{
	0x83721236,0x45772639,0x87463528,0x23485039
};

// Comm security variables for com1 and com2 (note com3 is the drive comm)

extern struct CdTable Com_CodeTable[2];
extern struct Cd_Tbl_Chk Com_Code_Tbl_Chk[2][fl_size];

extern struct Fault S_Trace[];


static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		
union flt_union {
		unsigned char fltb[sizeof(Fault_type)];
		uint16 fltw[sizeof(Fault_type)/2];
		struct Fault Flt;
	}com_ltf;

int16 clear_to_send(int16 com_nmb);
void send_galcom_fvar (int16 fvar_cpu, int16 fvar_ix);
int16 rxgalcom(int16 Which_Com);
void send_reboot (void);
void galcom (int16 com_nmb);
void galcom_pack (int16 com_nmb, int16 msg);
void galcom_unpack(int16 com_nmb, int16 msg);
void xxtea(uint32 *v, int16 n, uint32 key[4]); 

int16 txptr[2] = {0,0};


int16 clear_to_send(int16 com_nmb)
{
	if (com_nmb == USR1COM)
	{
		if ((fvars[fvcomdiag1] == 5) || (fvars[fvcomdiag1] == 7))
		{
		  	if(SIU.GPDI[CTS5].B.PDI == 1)
		 		return(0);
		   	else
		  		return(1);		// Clear to send when input low
		}
		else
			return(1);		// no flow control
	}
	else if (com_nmb == USR2COM)
	{
		if ((fvars[fvcomdiag2] == 5) || (fvars[fvcomdiag2] == 7))
		{
		  	if(SIU.GPDI[CTS2].B.PDI == 1)
		 		return(0);
		   	else
		  		return(1);		// Clear to send when input low
		}
		else
			return(1);		// no flow control
	}
	return(0);
}

//*****************************************
// This is receive a galcom message procedure
// it returns a packet value when a complete message
// is received and a 0 when not complete
//
// Index 0 = P
//       1 = length low byte
//		 2 = length high byte
//		 3 = length check low byte
//		 4 = length check hight byte
//       5 = packet number
//		 n = data (checksum can be in the last byte of the data) (index 6 starts the packet data)
//       n+1 = @ - end of packet
//*****************************************


int16 rxgalcom(int16 Which_Com)
{
  	int16 i,j;
  	int16 rbempt=0;
  	int16 endmess=0;
 
  	rbempt = isrxempty(Which_Com);
  	while((rbempt == 0) && (endmess == 0))
  	{
	  	rmbuf[Which_Com][mpnt[Which_Com]] = (uint8)asigetc(Which_Com);
	  	mpnt[Which_Com]++;
	  	rbempt = isrxempty(Which_Com);
	  	if(rmbuf[Which_Com][0] != 'P')
	  	{
	  		galcom_len[Which_Com] = 0;
	  	  	mpnt[Which_Com] = 0;
	  	}
	  	else
	  	{
	  	  	if (mpnt[Which_Com] >= rbufsize)
	  	  	{
	  			rbempt = 1;
	  			galcom_len[Which_Com] = 0;
	  			mpnt[Which_Com] = 0;
	  	  	}
	  	  	else if (mpnt[Which_Com] > 4)
	  	  	{
	  	  		if (galcom_len[Which_Com] == 0)
	  	  		{
		  	  		uwu.b[lbw] = rmbuf[Which_Com][1];
		  	  		uwu.b[hbw] = rmbuf[Which_Com][2];
		  	  		galcom_len[Which_Com] = uwu.w;
		  	  		uwu.b[lbw] = rmbuf[Which_Com][3];
		  	  		uwu.b[hbw] = rmbuf[Which_Com][4];
		  	  		galcom_len_ck[Which_Com] = uwu.w;
		  	  		
		  	  		if ((galcom_len[Which_Com] > rbufsize) || (galcom_len[Which_Com] != (galcom_len_ck[Which_Com] + 1)))
		  	  		{	// Compare length check with length byte
			  			galcom_len[Which_Com] = 0;
			  			rbempt = 1;
			  			mpnt[Which_Com] = 0;
		  	  		}
	  	  		}
		  	  	else if (mpnt[Which_Com] == galcom_len[Which_Com])
		  	  	{
		  	  	    if(rmbuf[Which_Com][galcom_len_ck[Which_Com]] == '@') // Good Packet
		  	  	    {
	  					good_packet[Which_Com]++;
		  				rmbuf[Which_Com][galcom_len_ck[Which_Com]] = 0;
		  				endmess = rmbuf[Which_Com][5];	// packet number
		  				mpnt[Which_Com] = 0;
			  			galcom_len[Which_Com] = 0;
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
		  				galcom_len[Which_Com] = 0;
		  	  	    }
		  	  	    rbempt = 1;
		  	  	}
		  	  	else if (mpnt[Which_Com] > galcom_len[Which_Com])
		  	  	{
		  			rbempt = 1;
		  			galcom_len[Which_Com] = 0;
		  			mpnt[Which_Com] = 0;
		  	  	}
	  	  	}
	  	  	
	  	}
  	}
  	return(endmess);
}



//****************************************************
// Send field variable to galcom device
//****************************************************

void send_galcom_fvar (int16 fvar_cpu, int16 fvar_ix)
{
	// send packets to connected devices

	if (connect[USR1COM] == 1)
	{
		lcd_fvar_cpu[USR1COM] = fvar_cpu;
		lcd_fvar_ix[USR1COM] = fvar_ix;
		put_pkt_req (28, cons[carnmb], USR1COM);
	}
	
	if (connect[USR2COM] == 1)
	{
		lcd_fvar_cpu[USR2COM] = fvar_cpu;
		lcd_fvar_ix[USR2COM] = fvar_ix;
		put_pkt_req (28, cons[carnmb], USR2COM);
	}
}


//****************************************************
// Send reboot sequence to wifi device
//****************************************************

void send_reboot (void)
{
	switch(reboot_seq)
	{
		case 0:
			if (in_net_cmd_mode[0] == 0)
			{
				net_cmd_resp[0] = 0;
				net_cmd_mode_req[0] = 1;
				in_net_cmd_mode[0] = 0;
				reboot_seq = 1;
			}
			else
				reboot_seq = 2;
			
			break;
			
		case 1:
			if (in_net_cmd_mode[0] == -1)
			{		// timed-out but send request anyway
				reboot_seq = 2;
				net_cmd_resp[0] = 0;
				net_cmd_req[0] = 5;		// request reboot device
			}
			else if (in_net_cmd_mode[0] == 1)
			{	// in command mode so send reboot request
				reboot_seq = 2;
				net_cmd_resp[0] = 0;
				net_cmd_req[0] = 5;		// request reboot device
			}
			break;
			
		case 2:
			send_reboot_cmd = 0;
			reboot_seq = 0;
			break;
		default:
			send_reboot_cmd = 0;
			reboot_seq = 0;
			break;
	}
}

// ********************************************************
// This is galcom human interface routine
// ********************************************************

void galcom (int16 com_nmb)
{
	int16 i;


	if (com_nmb > USR2COM)			// some memory arrays are only 2 deep
		return;
	
	if (send_reboot_cmd == 1)
		send_reboot();
	else
		reboot_seq = 0;
	
	if ((net_cmd_mode_req[com_nmb] == 1) && (in_net_cmd_mode[com_nmb] == 0))
	{
		En_Net_Cmd_Mode(com_nmb);
		return;
	}
	else if (in_net_cmd_mode[com_nmb] == 1)
	{
		Proc_Net_Cmd(com_nmb);
		return;
	}

	if (Com_Recv_Int[com_nmb] == 1)
		Com_Recv_Int[com_nmb] = 0;

	if (com_nmb == USR1COM)
	{		// COM1 
		if (Tranbuf_Empty[com_nmb] == 0)
		{
			if (((USR1port.SR.B.TDRE == 1) && (USR1port.CR1.B.TIE == 1)) || (USR1port.CR2.B.TXDMA == 1)) // Interupt source is transmiter hold reg empty
			{
				if (timers[tcom_tran+com_nmb] > 20)
				{
					Restart_Com(com_nmb);
				}
			}
	 		else
	 			timers[tcom_tran+com_nmb] = 0;
		}
	 	else
	 		timers[tcom_tran+com_nmb] = 0;
	 	
	 	
//	 	if (timers[tuser_com+com_nmb] > 30)
//	 	{ 						// heart beat every 3 sec
//			timers[tuser_com+com_nmb] = 0;
//	  		put_pkt_req (7, cons[carnmb], com_nmb);
//	 	}
	 		
	 		
	}
	else if (com_nmb == USR2COM)
	{		// COM2
		if (Tranbuf_Empty[com_nmb] == 0)
		{
			if (((USR2port.SR.B.TDRE == 1) && (USR2port.CR1.B.TIE == 1))  || (USR2port.CR2.B.TXDMA == 1)) // Interupt source is transmiter hold reg empty
			{
				if (timers[tcom_tran+com_nmb] > 20)
				{
					Restart_Com(com_nmb);
				}
			}
	 		else
	 			timers[tcom_tran+com_nmb] = 0;
		}
	 	else
	 		timers[tcom_tran+com_nmb] = 0;
	 	
//	 	if (timers[tuser_com+com_nmb] > 30)
//	 	{ 						// heart beat every 3 ec
//			timers[tuser_com+com_nmb] = 0;
//	  		put_pkt_req (7, cons[carnmb], com_nmb);
//	 	}
	 		
	}


	if (com_req_car_flt[com_nmb] != 0) 
	{
    	i = get_pkt_status(36, com_car_req[com_nmb], gc_port, 100);
		if (i == 1)
		{
	  		put_pkt_req (40, cons[carnmb], com_nmb);
			com_req_car_flt[com_nmb] = 0;
		}
		else if ((i == 0) && (gc_dev_online[com_car_req[com_nmb]] == 0)) // car is offline
		{
	  		put_pkt_req (40, cons[carnmb], com_nmb);
			com_req_car_flt[com_nmb] = 0;
		}
	}
	

	if (rdinp(i_AD) == 0) 
		timer_sec[ts_pwd] = 0;
	
	if (timer_sec[ts_pwd] <= (fvars[fvpwtime]/10))		// the password value
		AD_status = 1;
	else
		AD_status = 0;
	
	if (connect[com_nmb] == 1)
	{
		if (timers[tcom_recv + com_nmb] > 100)
		{			// reset com once after 10 seconds
			connect[com_nmb] = 0;
			Restart_Com(com_nmb);
#if (Simulator == 0)
			send_reboot_cmd = 1;	//	add back after david's testing is finished
#endif
		}
	}
	
	if (galcom_req[com_nmb] != 0)
	{
		if (timers[tcom_pkreq + com_nmb] > 30)
			galcom_req[com_nmb] = 0;
	}
	else
		timers[tcom_pkreq + com_nmb] = 0;
	
	if (clear_to_send(com_nmb) == 1)
	{
	  	i = rxgalcom(com_nmb);
	  	if(i != 0)
	  	{
	      	connect[com_nmb] = 1;
			timers[tcom_recv + com_nmb] = 0;
	      	galcom_unpack(com_nmb,i);
	  	}
		if (Tranbuf_Empty[com_nmb] == 1)
		{
			if (tx_delay_cnt[com_nmb] >= 2)
			{
				tx_delay_cnt[com_nmb] = 0;			
		 		i = get_pkt_req(cons[carnmb], com_nmb);
	  			if (i != 0)
	  				galcom_pack(com_nmb, i);
	  			else if (galcom_req[com_nmb] != 0)
	  			{
	  				galcom_pack(com_nmb, galcom_req[com_nmb]);
	  			}
	  		}
	  		else
	  			tx_delay_cnt[com_nmb]++;
	  	}
	  	else
	  		tx_delay_cnt[com_nmb] = 0;
	}
}

void galcom_pack (int16 com_nmb, int16 msg)
{
  	int16 i,j,k,qq;
	int16 brdnmb,ionmb;
  	int32 tcc_set[2];
	int16 fl_ix;
	int16 chksum;
	struct Fault *flp;
	struct Fault *ptr;
	int16 max_pkt_len = 135;
	int16 len = 0;
	uint8 valid_packet = 0;
	int16 el;
	int16 nmb_error_codes_ix = 0;
	int16 nmb_error_codes = 0;
	int16 Fault_ix = 0;
	int16 max_fault_ix = 0;


	txptr[com_nmb] = 5;
	
	switch (msg)
	{
	
		case 3:		// Access level status
			tranbuf[com_nmb][txptr[com_nmb]++] =  3;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[carnmb] & 0xff);
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(com_access_level_pk3[com_nmb] & 0xff);		// see access levels 
			tranbuf[com_nmb][txptr[com_nmb]++] = AD_status;				// must be 1 to change parameters
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_access_packet[com_nmb];
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_access_status[com_nmb];  // access status bit 0: 1 if AD_status failed
																					 //               bit 1: 1 if Access level failed
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 4:		// response for write user name
			tranbuf[com_nmb][txptr[com_nmb]++] =  4;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[carnmb] & 0xff);
	#if (auto_elevator == 1)
			tranbuf[com_nmb][txptr[com_nmb]++] = 1;
	#else
			tranbuf[com_nmb][txptr[com_nmb]++] = 2;
	#endif
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 5:			 // New Password
			tranbuf[com_nmb][txptr[com_nmb]++] =  5;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[carnmb] & 0xff);
	#if (auto_elevator == 1)
			tranbuf[com_nmb][txptr[com_nmb]++] = 1;
	#else
			tranbuf[com_nmb][txptr[com_nmb]++] = 2;
	#endif
			len = txptr[com_nmb];
			valid_packet = 1;
	  		break;
	
		case 7:		  // Version, Revision and Job Number
	     	tranbuf[com_nmb][txptr[com_nmb]++] = 7;
			tranbuf[com_nmb][txptr[com_nmb]++] = version;
			tranbuf[com_nmb][txptr[com_nmb]++] = revision;
		    tranbuf[com_nmb][txptr[com_nmb]++] = interim_revision;
			for (i=0;i<25;i++) 
			{
				
				tranbuf[com_nmb][txptr[com_nmb]++] = vidmes[0][i];	// job number
				if (vidmes[0][i] == CR)
					break;
				else if (i == 24)
					tranbuf[com_nmb][txptr[com_nmb]++] = CR;
			}
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[carnmb];
#if (Simulator != 0)
	#if (Traction == 1)
		    tranbuf[com_nmb][txptr[com_nmb]++] = 5;		// Traction Sim
	#else
		    tranbuf[com_nmb][txptr[com_nmb]++] = 5;		// Hydro Sim  6
	#endif
#else
	#if (Traction == 1)
		    tranbuf[com_nmb][txptr[com_nmb]++] = 1;		 // traction	
	#else
		    tranbuf[com_nmb][txptr[com_nmb]++] = 1;		 // hydro 2 
	#endif
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Dispatcher;	  // 99 = no dispatcher otherwise 0-8
 			uwu.w = Current_Fault;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			local_gettime();
			tranbuf[com_nmb][txptr[com_nmb]++] = t.minute;
		    tranbuf[com_nmb][txptr[com_nmb]++] = t.second;
			tranbuf[com_nmb][txptr[com_nmb]++] = t.hour;
			local_getdate();
			tranbuf[com_nmb][txptr[com_nmb]++] = d.day;
			tranbuf[com_nmb][txptr[com_nmb]++] = d.month;
			uwu.w = d.year;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			len = txptr[com_nmb];
			valid_packet = 1;
		    break;
	  		
		case 9:		// Send Encription Key
			tranbuf[com_nmb][txptr[com_nmb]++] =  9;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[carnmb] & 0xff);
			uiu.dw = key[0];											// encription key 0
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = key[1];											// encription key 1
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = key[2];											// encription key 2
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = key[3];											// encription key 3
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

	  		
	  	case 10:	 // Group initializaton data
			tranbuf[com_nmb][txptr[com_nmb]++] =  10;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[carnmb] & 0xff);
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[grtopf] & 0xff);
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[nmbcars] & 0xff);
			for(i=1; i<=cons[grtopf]; i++)
			{
				tranbuf[com_nmb][txptr[com_nmb]++] = ' ';	 // Reserved for third byte of floor marking
				tranbuf[com_nmb][txptr[com_nmb]++] = flmrk[i][0];
				tranbuf[com_nmb][txptr[com_nmb]++] = flmrk[i][1];
			}

			// Car call mask

			for (i=1;i<=cons[nmbcars];i++)
			{			// cc_mask must be changed to a per car array
					
				uiu.dw = cc_mask[i][0];				// car call mask 
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

				uiu.dw = cc_mask[i][1];				// car call mask 
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

				uiu.dw = rcc_mask[i][0];				// rear car call mask 
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

				uiu.dw = rcc_mask[i][1];				// rear car call mask 
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			}
	
			// Hall call mask
			
			uiu.dw = g_ucmsk[0];			// Bit mask for valid up hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_ucmsk[1];			// Bit mask for valid up hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_dcmsk[0];			// Bit mask for valid down hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_dcmsk[1];			// Bit mask for valid down hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_rucmsk[0];		// Bit mask for valid up hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_rucmsk[1];		// Bit mask for valid up hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_rdcmsk[0];		// Bit mask for valid down hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_rdcmsk[1];		// Bit mask for valid down hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

		
			// IR/Second Riser Mask
			
			uiu.dw = g_irucmsk[0];		// Bit mask for valid ir up hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_irucmsk[1];		// Bit mask for valid ir up hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_irdcmsk[0];		// Bit mask for valid ir down hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_irdcmsk[1];		// Bit mask for valid ir down hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_rirucmsk[0];		// Bit mask for valid rear ir up hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_rirucmsk[1];		// Bit mask for valid rear ir up hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = g_rirdcmsk[0];		// Bit mask for valid rear ir down hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			
			uiu.dw = g_rirdcmsk[1];		// Bit mask for valid rear ir down hall call floors
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grbot_rfl;				// actual bottom rear floor number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grtop_rfl;				// actual top rear floor number
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 11:		  // Extended car initialization
	     	tranbuf[com_nmb][txptr[com_nmb]++] = 11;
			tranbuf[com_nmb][txptr[com_nmb]++] = version;
			tranbuf[com_nmb][txptr[com_nmb]++] = revision;
		    tranbuf[com_nmb][txptr[com_nmb]++] = interim_revision;
			uwu.w = nmb_io_arrays;									// number of i/o array index max for all cars
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];	 		// number of i/o array low byte
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];	 		// number of i/o array high byte
			tranbuf[com_nmb][txptr[com_nmb]++] = nmb_io_brds;			// number of i/o boards  max for all cars
#if (Simulator != 0)
	#if (Traction == 1)
		    tranbuf[com_nmb][txptr[com_nmb]++] = 5;		// Traction Sim
	#else
		    tranbuf[com_nmb][txptr[com_nmb]++] = 5;		// Hydro Sim
	#endif
#else
	#if (Traction == 1)
		    tranbuf[com_nmb][txptr[com_nmb]++] = 1;		 // traction	
	#else
		    tranbuf[com_nmb][txptr[com_nmb]++] = 1;		 // hydro
	#endif
#endif
			// The following items were moved from packet 12
			tranbuf[com_nmb][txptr[com_nmb]++] = NTS_Proc_Adj_Vars_Menu + 1;	 // Number of menus (last menu index + 1)
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[sercom];				// Serial Comm Devices
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[equake];				// Earthquake
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[att];	 		// Attendant
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[loadw];	 		// Load weighing
			uwu.w = cons[speed];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];	 			// Car Speed low byte
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];	 			// Car Speed high byte
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[dispis];	 // Only have data on this car right now! Discrete PI's 
			
			// The following items were sent previously for each car
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)top_fl[cons[carnmb]];
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)top_rfl[cons[carnmb]];
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)rear_op[cons[carnmb]];
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[BinPiIn];	// will be an array for number of cars in the future
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Drive_Type[cons[carnmb]];	// will be an array in the future
			if ((Drive_Type[cons[carnmb]] == 1) || (Drive_Type[cons[carnmb]] == 9))
				tranbuf[com_nmb][txptr[com_nmb]++] = 1;	 // Controller Type = 0; Hydro
			else if (((Drive_Type[cons[carnmb]] >= 3) && (Drive_Type[cons[carnmb]] <= 5)) || (Drive_Type[cons[carnmb]] >= 10))
				tranbuf[com_nmb][txptr[com_nmb]++] = 1;	 // Controller Type = 1; Traction Distance Feedback
			else
				tranbuf[com_nmb][txptr[com_nmb]++] = 2;	 // Controller Type = 2; Traction non-Distance Feedback
				
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 12:				   // Extended group initialization data
					 // Additional variables for future expansion
	     	tranbuf[com_nmb][txptr[com_nmb]++] = 12;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[Second_Riser];				// Inconspicious Riser
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[incrs];				// Inconspicious Riser

			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[SecFlCfg];				// Security Floor Mask Configuration
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[cblue];				// Code Blue operation
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[epgrp];				// Inter-group emergency power
			tranbuf[com_nmb][txptr[com_nmb]++] = nmb_hcio_brds;	 	// Maximum number of hall call boards  if zero use 20 for tract, 46 for tract_hr.
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[Dual_Ser_Riser]; // Dual Serial Riser
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Dispatcher;	  // 99 = no dispatcher otherwise 0-8
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[Dest_Disp];	 // Destination Dispatch flag
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused

			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[nmbcars];	// number of cars
			for (i=1;i<=cons[nmbcars];i++)
			{		   // set for six or eight cars (future)
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Security_Type[i];
				if (i < 7)
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)fvars[fvvidc+i];	 // Video car position
				else
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)fvars[fvvidc7+(i-7)];	 // Video car position
				tranbuf[com_nmb][txptr[com_nmb]++] = car_bld_no[i][0];	 // car building elevator
				tranbuf[com_nmb][txptr[com_nmb]++] = car_bld_no[i][1];	 // car building elevator
				tranbuf[com_nmb][txptr[com_nmb]++] = ' ';	 // Reserved for third byte of car building elevator
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)rear_op[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
				tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
				tranbuf[com_nmb][txptr[com_nmb]++] = 0;	 // Unused
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 13:			 // Send i/o board used for cars 
			if(com_car_req[com_nmb] == cons[carnmb])
			{
		      	tranbuf[com_nmb][txptr[com_nmb]++] = 13;					// packet number
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
			    tranbuf[com_nmb][txptr[com_nmb]++] = nmb_io_brds;
		      	for(i=0; i<nmb_io_brds; i++)
			  		tranbuf[com_nmb][txptr[com_nmb]++] = IO_board[com_car_req[com_nmb]][i];
				len = txptr[com_nmb];
				valid_packet = 1;
			}
			break;

		case 14:		// Send which i/o are inputs 0's and outputs 1's
			if(com_car_req[com_nmb] == cons[carnmb])
			{
		      	tranbuf[com_nmb][txptr[com_nmb]++] = 14;					// packet number
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nmb_io_brds);
				for(i=0;i<(nmb_io_brds * 3);i++)
					tranbuf[com_nmb][txptr[com_nmb]++] = IO_In_Out[i];		// this will be changed to an array per car
				len = txptr[com_nmb];
				valid_packet = 1;
			}
			break;

		case 15:			// Send valid i/o's 1= valid
			if(com_car_req[com_nmb] == cons[carnmb])
			{
		      	tranbuf[com_nmb][txptr[com_nmb]++] = 15;					// packet number
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nmb_io_brds);
				for(i=0;i<(nmb_io_brds * 3);i++)
					tranbuf[com_nmb][txptr[com_nmb]++] = io_valid[i];		// this will be changed to an array per car
				len = txptr[com_nmb];
				valid_packet = 1;
			}
			break;

		case 16:			// Send group valid i/o's 1 = valid
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 16;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = nmb_hcio_brds;
			for(i=0;i<(nmb_hcio_brds * 3);i++)				// hr car index changed from 136 to 135
				tranbuf[com_nmb][txptr[com_nmb]++] = gio_valid[i];
	      	tranbuf[com_nmb][txptr[com_nmb]++] = nmb_hcio_brds;
			for(i=0;i<nmb_hcio_brds;i++)
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(HC_IO_board[i] & 0x03);
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 17:		// Send car i/o names
			if(com_car_req[com_nmb] == cons[carnmb])
			{
		      	tranbuf[com_nmb][txptr[com_nmb]++] = 17;					// packet number
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
				if (com_io_brd[com_nmb] < nmb_io_brds)
				{
					brdnmb = com_io_brd[com_nmb];	 // brdnmb location: 0 to num boards - 1
			      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)brdnmb;
					ionmb = brdnmb * 24;
					for(j=ionmb; j<(ionmb+24); j++)
					{
				    	for(i=0; i<5; i++)
							tranbuf[com_nmb][txptr[com_nmb]++] = ioname[j][i];		// this will not change for driver device software
					}
				}
				len = txptr[com_nmb];
				valid_packet = 1;
			}
			break;

		case 18:			// Send group i/o names
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 18;					// packet number
			if (com_io_brd[com_nmb] < nmb_hcio_brds)
			{
				brdnmb = com_io_brd[com_nmb];		  // brdnmb location: 0 to num boards - 1
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)brdnmb;
				ionmb = brdnmb * 24;
				for(j=ionmb; j<(ionmb+24); j++)
				{
					for(i=0; i<5; i++)
	  					tranbuf[com_nmb][txptr[com_nmb]++] = hcioname[j][i];
	 			}
	 		}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 19:		// Send which group i/o are inputs 0's and outputs 1's
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 19;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nmb_hcio_brds);
			for(i=0;i<(nmb_hcio_brds * 3);i++)
				tranbuf[com_nmb][txptr[com_nmb]++] = GIO_In_Out[i];		// this will be changed to an array per car
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 20:					 // Request to send the field variables (600 variables are sent in five packets 0 - 5)
			if (com_fvar_block_ix[com_nmb] < 5)
			{		// check for valid packet
				tranbuf[com_nmb][txptr[com_nmb]++] =  20;			// packet number
					
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
				tranbuf[com_nmb][txptr[com_nmb]++] =  (uint8)com_fvar_block_ix[com_nmb];		// send in groups of 120 variables
					
				// always send checksum
				qq = 0;
				j = com_fvar_block_ix[com_nmb] * 120;
		      	for(i=j; i<(j+120); i++)
		      	{									   // calculate checksum
			  		qq += fvars[i];
		      	}
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(qq & 0x0ff);					// send checksum
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((qq >> 8) & 0x00ff);

		      	for(i=j; i<(j+120); i++)
		      	{
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(fvars[i] & 0x00ff);
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((fvars[i] >> 8) & 0x00ff);
		      	}
				len = txptr[com_nmb];
				valid_packet = 1;
		  	}
			break;

#if (Traction == 1)
		case 22:		 // send digital speed clamp setting and terminal limit speeds
			j = 8;
			k = 7;
			tranbuf[com_nmb][txptr[com_nmb]++] =  22;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)j;					// number of speed clamps
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)k;					// number of ut/dt speed values
			qq = 0;
		    for(i=0; i<=j; i++)
		    {									   // calculate checksum
				qq += speed_clamp[i];
		    }
		    for(i=0; i < k; i++)
		    {									   // calculate checksum
				qq += UT_Speed[i];
				qq += DT_Speed[i];
		    }
			qq += DTS_Speed;
			qq += UTS_Speed;

			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(qq & 0x0ff);					// send checksum
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((qq >> 8) & 0x00ff);

		    for(i=0; i<=j; i++)
		    {
				uwu.w = speed_clamp[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			}
		    for(i=0; i < k; i++)
		    {
				uwu.w = DT_Speed[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			}
			uwu.w = DTS_Speed;
		 	tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
		 	tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

		    for(i=0; i < k; i++)
		    {
				uwu.w = UT_Speed[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
		    }
			uwu.w = UTS_Speed;
		 	tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
		 	tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 24:			// send info during hoistway learn
			tranbuf[com_nmb][txptr[com_nmb]++] =  24;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
			uiu.dw = DPP_Count;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			for(i=1; i<=cons[grtopf]; i++)
			{
				uiu.dw = DPP_Floor_Pos[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			}
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Cam_Mode;
			uwu.w = position;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = (int16)(((Enc_vel * 5.0)/(Pulses_per_Inch)) + 0.5);		// encoder velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = (int16)(Pulses_per_Inch);
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uiu.dw = DPP_DN;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = DPP_DT;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = DPP_UN;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = DPP_UT;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
#endif

		case 26:			// Send the field variables 1 to 60
			tranbuf[com_nmb][txptr[com_nmb]++] =  26;			// packet number
					
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];   // car number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_fvar_cpu[com_nmb]; 	// cpu number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_nmb_fvar[com_nmb];  // number of fvars
					
			// always send checksum
			i = 0;
			qq = 0;
			while (i < com_nmb_fvar[com_nmb])
		    {				// calculate checksum
			  	qq += com_fvar_ix[com_nmb][i];
	      		if (com_fvar_cpu[com_nmb] == MAIN_CPU)
	      		{
				  	qq += fvars[com_fvar_ix[com_nmb][i]];
	      		}
/*
      			else if (com_fvar_cpu[com_nmb] == SPB_CPU)
      			{
				  	qq += spb_fvars[com_fvar_ix[com_nmb][i]];
      			}
*/
      			else if (com_fvar_cpu[com_nmb] == NTS_CPU)
      			{
				  	qq += nts_fvars[com_fvar_ix[com_nmb][i]];
      			}
				i++;
		    }
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(qq & 0x0ff);					// send checksum
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((qq >> 8) & 0x00ff);

			i = 0;
			while (i < com_nmb_fvar[com_nmb])
	      	{
	      		if ((com_fvar_cpu[com_nmb] == MAIN_CPU) && (com_fvar_ix[com_nmb][i] < 600))
	      		{					  // standard variables
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(com_fvar_ix[com_nmb][i] & 0x00ff);					   // index
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((com_fvar_ix[com_nmb][i] >> 8) & 0x00ff);
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(fvars[com_fvar_ix[com_nmb][i]] & 0x00ff);			   // fvar
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((fvars[com_fvar_ix[com_nmb][i]] >> 8) & 0x00ff);
	      		}
 /*     			else if ((com_fvar_cpu[com_nmb] == SPB_CPU) && (com_fvar_ix[com_nmb][i] < numspbvars))
      			{					  // Spb Variables
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(com_fvar_ix[com_nmb][i] & 0x00ff);					   // index
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((com_fvar_ix[com_nmb][i] >> 8) & 0x00ff);
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(spb_fvars[com_fvar_ix[com_nmb][i]] & 0x00ff);			   // fvar
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((spb_fvars[com_fvar_ix[com_nmb][i]] >> 8) & 0x00ff);
      			}
 */
      			else if ((com_fvar_cpu[com_nmb] == NTS_CPU) && (com_fvar_ix[com_nmb][i] < numntsvars))
      			{					  // nts variables
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(com_fvar_ix[com_nmb][i] & 0x00ff);					   // index
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((com_fvar_ix[com_nmb][i] >> 8) & 0x00ff);
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nts_fvars[com_fvar_ix[com_nmb][i]] & 0x00ff);			   // fvar
			  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((nts_fvars[com_fvar_ix[com_nmb][i]] >> 8) & 0x00ff);
      			}
	      		i++;
	      	}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;



		case 27:	  	// Send Menu Setup
		
			tranbuf[com_nmb][txptr[com_nmb]++] =  27;			// packet number
						
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[carnmb];  				// car number
      		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_fvar_cpu[com_nmb];	   // cpu number
	      	// Send menu index number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_menu_req[com_nmb];		// Menu data being requested
		      	
	      
   			if ((com_fvar_cpu[com_nmb] == MAIN_CPU) && (com_menu_req[com_nmb] < Num_Fvars_Menus))
   			{
		      	// Send menu name
		      	for (i=0;i<=23;i++)
		      	{
		      		tranbuf[com_nmb][txptr[com_nmb]++] = CAN_Test_Items[com_menu_req[com_nmb]][i];
		      	}

	      		// Send number of items in the menu
	      		tranbuf[com_nmb][txptr[com_nmb]++] = last_var [com_menu_req[com_nmb]] + 1;	 // index of last variable
		      		
	      		// Send menu item order
		      		
	      		i = 0;
	      		while (i <= (last_var [com_menu_req[com_nmb]]))
	      		{
	      			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(var_menu_tbl[com_menu_req[com_nmb]][i] & 0x00ff);
	      			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((var_menu_tbl[com_menu_req[com_nmb]][i] >> 8) & 0x00ff);
					i++;
	      		}
	      	}

   			else if ((com_fvar_cpu[com_nmb] == NTS_CPU) && (com_menu_req[com_nmb] == NTS_Proc_Adj_Vars_Menu))
   			{
		      	// Send menu name
		      	for (i=0;i<=23;i++)
		      	{
		      		tranbuf[com_nmb][txptr[com_nmb]++] = CAN_Test_Items[NTS_Proc_Adj_Vars_Menu+1][i];
		      	}

	      		// Send number of items in the menu
	      		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)nts_last_var + 1;						// index of last variable
		      		
	      		// Send menu item order
		      		
	      		i = 0;
	      		while (i <= nts_last_var)
	      		{
	      			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nts_var_ix[i] & 0x00ff);
	      			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((nts_var_ix[i] >> 8) & 0x00ff);
					i++;
	      		}
	      	}
	      	else 
	      	{		 // this menu array is not displayed such as the drive parameter menu
		      	// Send menu name
		      	for (i=0;i<=23;i++)
		      	{
		      		tranbuf[com_nmb][txptr[com_nmb]++] = LCD_Main_Menu[0][i];		   	// send GALaxy as the menu
		      	}

	      		// Send number of items in the menu
	      		tranbuf[com_nmb][txptr[com_nmb]++] = 0;		   		// index of last variable
	      	}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
					
		case 28:			// Send one field variable when parameter modified from LCD interface
			tranbuf[com_nmb][txptr[com_nmb]++] =  28;			// packet number
					
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[carnmb];   // car number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)lcd_fvar_cpu[com_nmb]; 	// cpu number
					
			// no checksum so send the variable in two locations
      		if ((lcd_fvar_cpu[com_nmb] == MAIN_CPU) && (lcd_fvar_ix[com_nmb] < 600))
      		{					  // standard variables
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(lcd_fvar_ix[com_nmb] & 0x00ff);					   // index
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((lcd_fvar_ix[com_nmb] >> 8) & 0x00ff);
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(fvars[lcd_fvar_ix[com_nmb]] & 0x00ff);			   // fvar
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((fvars[lcd_fvar_ix[com_nmb]] >> 8) & 0x00ff);
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(fvars[lcd_fvar_ix[com_nmb]] & 0x00ff);			   // fvar
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((fvars[lcd_fvar_ix[com_nmb]] >> 8) & 0x00ff);
      		}
/*
  			else if ((lcd_fvar_cpu[com_nmb] == SPB_CPU) && (lcd_fvar_ix[com_nmb] < numspbvars))
  			{					  // Spb Variables
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(lcd_fvar_ix[com_nmb] & 0x00ff);					   // index
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((lcd_fvar_ix[com_nmb] >> 8) & 0x00ff);
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(spb_fvars[lcd_fvar_ix[com_nmb]] & 0x00ff);			   // fvar
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((spb_fvars[lcd_fvar_ix[com_nmb]] >> 8) & 0x00ff);
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(spb_fvars[lcd_fvar_ix[com_nmb]] & 0x00ff);			   // fvar
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((spb_fvars[lcd_fvar_ix[com_nmb]] >> 8) & 0x00ff);
     		}
*/
     		else if ((lcd_fvar_cpu[com_nmb] == NTS_CPU) && (lcd_fvar_ix[com_nmb] < numntsvars))
     		{					  // nts variables
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(lcd_fvar_ix[com_nmb] & 0x00ff);					   // index
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((lcd_fvar_ix[com_nmb] >> 8) & 0x00ff);
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nts_fvars[lcd_fvar_ix[com_nmb]] & 0x00ff);			   // fvar
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((nts_fvars[lcd_fvar_ix[com_nmb]] >> 8) & 0x00ff);
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nts_fvars[lcd_fvar_ix[com_nmb]] & 0x00ff);			   // fvar
		  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((nts_fvars[lcd_fvar_ix[com_nmb]] >> 8) & 0x00ff);
     		}
 			len = txptr[com_nmb];
			valid_packet = 1;
			break;


		case 29:			// Send car I/O and status		
			tranbuf[com_nmb][txptr[com_nmb]++] =  29;					// packet number

			if (cons[carnmb] != 0)
			{			// not a destination dispatcher device
				if(com_car_req[com_nmb] == 0) // if number not valid, send this car's data
				{
					com_car_req[com_nmb] = cons[carnmb];
				}
			}
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];

	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)procf;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)doorf;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)servf;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)rdoorf;

			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)position;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)dirf;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)dpref;		

 			uwu.w = Current_Fault;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];		
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];		

			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Enc_dir;
			uwu.w = dmd_vel_fpm;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = enc_vel_fpm;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			
			uiu.dw = DPP_Floor_Pos[position];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			
			uiu.dw = DPP_Count;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

		 	uiu.dw = run_statusf;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
#if (Traction == 1)
			uwu.w = drv_vel_fpm;											// drive velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#else
			uwu.w = 0;											// drive velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#endif			
			uwu.w = accel_x;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = accel_y;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = accel_z;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 30:			// Send car I/O and status		
			tranbuf[com_nmb][txptr[com_nmb]++] =  30;					// packet number


			if (cons[carnmb] != 0)
			{			// not a destination dispatcher device
				if(com_car_req[com_nmb] == 0) // if number not valid, send this car's io data
				{
					com_car_req[com_nmb] = cons[carnmb];
				}
			}
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nmb_io_brds);
		
	     	for(i=0; i<(nmb_io_brds * 3); i++)			// data for all 86 i/o boards
			{
			  	tranbuf[com_nmb][txptr[com_nmb]++] = iodata[com_car_req[com_nmb]][i];
			}


	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)procf;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)doorf;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[cons[carnmb]].g_empf;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)firef;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)servf;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)rdoorf;

			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)position;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)dirf;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)dpref;		

 			uwu.w = Current_Fault;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];		
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];		

			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Enc_dir;
			uwu.w = dmd_vel_fpm;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = enc_vel_fpm;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uiu.dw = DPP_Floor_Pos[position];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = DPP_Count;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = statusf;		
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
					
			uiu.dw = statusf2;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ins_status;	
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)power_status;	
	      	
		 	uwu.w = (uint8)fire_status;		 	
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = safety_string_status;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];


		 	uwu.w = (int16)grp_Car_Stat[cons[carnmb]].g_control;	
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
 	
		 	uiu.dw = run_statusf;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

 			uiu.dw = grp_Car_Stat[com_car_req[com_nmb]].g_cc[0];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
 	
			uiu.dw = grp_Car_Stat[com_car_req[com_nmb]].g_cc[1];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
 	
		 	uiu.dw = grp_Car_Stat[com_car_req[com_nmb]].g_rcc[0];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
 
 			uiu.dw = grp_Car_Stat[com_car_req[com_nmb]].g_rcc[1];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

 			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)car_alrm;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)car_has_fault;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ins_servf;
			
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)medf;		// medical
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)codebf;	// code blue
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)eqf;		// seismic
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)rfiref;

			uwu.w = vel_fpm;												// Vel Calc
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = front_slowdown;											// Slowdown
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = rear_slowdown;											// Rear Slowdown
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];


	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ncu_door_seq;				// NCU
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)stepf;						// Step flag
 	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)dcalls;					// direction calls
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)predof;					// pre door open flag
 	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)startf;					// start flag
 	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)nstopf;					// need to stop flag
 	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)next_stop;					// next stop
 	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)estop;						// emergency stop
 	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)hsf;						// high speed flag
 	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)lev_startf;				// leveling start flag
 	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)relevel;					// relevel flag

#if (Traction == 1)
			uwu.w = drv_vel_fpm;											// drive velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = (uint16)(drv_command & 0xffff);							// drive Command
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];


			uwu.w = (uint16)drv_status;										// drive status
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = drv_motor_torque;										// drive motor torque
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = drv_trq_cmd;												// drive torque
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#else
			uwu.w = 0;											// drive velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = 0;							// drive Command
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];


			uwu.w = 0;										// drive status
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = 0;										// drive motor torque
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = 0;												// drive torque
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#endif

	 	    tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(percent_load/10);	// percent load

			uwu.w = timers[tmotion];										// motion timer
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = NTS_spi.velocity;										// NTS Velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

 			uiu.dw = NTS_spi.pos_cnt;										// NTS Position Count
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

	 	    tranbuf[com_nmb][txptr[com_nmb]++] = NTS_spi.Status1.R;			// NTS status 1
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = NTS_spi.Status1.R;			// NTS status 2

			tranbuf[com_nmb][txptr[com_nmb]++] = NTS_spi.Command1.R;		// NTS command 1
			tranbuf[com_nmb][txptr[com_nmb]++] = NTS_spi.Command2.R;		// NTS command 2


	 	    tranbuf[com_nmb][txptr[com_nmb]++] = NTS_spi.doorzone;			// NTS doorzone
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = NTS_spi.limits;			// NTS limits
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = NTS_spi.vel_dir;			// NTS velocity direction
	 	    
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 1
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 2
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 3
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 4
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 5
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 6
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 7
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 8
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 9
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 10
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 11
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 12
	 	    tranbuf[com_nmb][txptr[com_nmb]++] = 0;							// unused 13
	
			uwu.w = 0;												// PAL Velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			tranbuf[com_nmb][txptr[com_nmb]++] = 0;				// PAL Status
	
			tranbuf[com_nmb][txptr[com_nmb]++] = Nudgst_Flags;				// Nudging status
			tranbuf[com_nmb][txptr[com_nmb]++] = Door_Req_Flags;			// door request flags
			tranbuf[com_nmb][txptr[com_nmb]++] = Call_Flags;				// call flags
	  		tranbuf[com_nmb][txptr[com_nmb]++] = chkrun_state;				// last check run state
	  		tranbuf[com_nmb][txptr[com_nmb]++] = chkstart_state;			// last check start state
	  		tranbuf[com_nmb][txptr[com_nmb]++] = chklevel_state;			// last check level state
	  		tranbuf[com_nmb][txptr[com_nmb]++] = chkdoor_state;				// last check door state
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Door_Seq_Flags;		// door sequence flags
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)fault_device;		
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)fault_prog_flag1;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)fault_prog_flag2;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)fault_device_2;
			tranbuf[com_nmb][txptr[com_nmb]++] = Fault_Bits[0];				// fault bits
			tranbuf[com_nmb][txptr[com_nmb]++] = Fault_Bits[1];					
			tranbuf[com_nmb][txptr[com_nmb]++] = Fault_Bits[2];
			tranbuf[com_nmb][txptr[com_nmb]++] = Fault_Bits[3];
			
			if (galcom_ver > 2)
			{
				uiu.dw = statusf3;		
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
						
				uiu.dw = statusf4;
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			}
			
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 31:	   // Group data packet
			tranbuf[com_nmb][txptr[com_nmb]++] =  31;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[grtopf] & 0xff);
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(cons[nmbcars] & 0xff);
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(grtop_rfl & 0xff);
			
	      	for(i=1; i<=(cons[grtopf] - 1); i++)
	      	{
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)g_asign_up[i];
	      	}
	      	for(i=2; i<=cons[grtopf]; i++)
	      	{
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)g_asign_dn[i];
	      	}
	      	for(i=1; i<=cons[nmbcars]; i++)
	      	{
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_pos;
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_dpref;
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_servf;
				tcc_set[0] = 0;
				tcc_set[1] = 0;
				if ((i >= 1) && (Car_Not_Talking[i] == 0))
				{ 
					for(qq=1; qq<=cons[grtopf]; qq++)
					{
						fl_ix = (qq-1)/32;				 
						if ((grp_Car_Stat[i].g_cc[fl_ix] & fl_mask[qq]) != 0)
				       		tcc_set[fl_ix] |= fl_mask[qq];
						else
							tcc_set[fl_ix] &= ~fl_mask[qq];
					}
				}
				else
				{
					tcc_set[0] = 0;
					tcc_set[1] = 0;
				}
				uiu.dw = tcc_set[0];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
				uiu.dw = tcc_set[1];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_firef;
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_doorf;
	      		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ncu_door[i];
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_ins_status;
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_fire_status;
				uwu.w = grp_Car_Stat[i].g_cur_flt;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				uiu.dw = grp_Car_Stat[i].g_statusf1;
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
				uiu.dw = grp_Car_Stat[i].g_statusf2;
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_pwr_status;
				uwu.w = grp_Car_Stat[i].g_ss_stat;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_car_has_fault;
	      	}
		   	for(i=2; i<=cons[grtopf]; i++)
	      	{
				if (g_asign_dn[i] == 0)
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)car_dn_eta[g_asign_dnr[i]][i];		
				else 
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)car_dn_eta[g_asign_dn[i]][i];		
			}
			
		  	for(i=1; i<cons[grtopf]; i++)
			{
				if (g_asign_up[i] == 0)
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)car_up_eta[g_asign_upr[i]][i];		
				else
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)car_up_eta[g_asign_up[i]][i];		
	      	}
	      	if (grtop_rfl != 0)
	      	{
		      		
		      	for(i=1; i<=(cons[grtopf] - 1); i++)
		      	{
			      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)g_asign_upr[i];
		      	}
		      	for(i=2; i<=cons[grtopf]; i++)
		      	{
			      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)g_asign_dnr[i];
		      	}


		      	for(i=1; i<=cons[nmbcars]; i++)
		      	{
		      		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)i;
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)rear_op[i];		//top_rfl
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)top_rfl[i];		//highest rear top floor
					tcc_set[0] = 0;
					tcc_set[1] = 0;
					if (rear_op[i] != 0)
					{
							
						if ((i >= 1) && (Car_Not_Talking[i] == 0))
						{ 
							for(qq=1; qq<=cons[grtopf]; qq++)
							{
								fl_ix = (qq-1)/32;
								if ((grp_Car_Stat[i].g_rcc[fl_ix] & fl_mask[qq]) != 0)
						       		tcc_set[fl_ix] |= fl_mask[qq];
								else
									tcc_set[fl_ix] &= ~fl_mask[qq];
							}
						}
						else	// else we don't have valid data
						{
							tcc_set[0] = 0;
							tcc_set[1] = 0;
						}
						uiu.dw = tcc_set[0];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
						uiu.dw = tcc_set[1];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
				      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_rdoorf;
					}
		      	}
			}
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)gservf;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)up_peak;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)dn_peak;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Dispatcher;	  // 99 = no dispatcher otherwise 0-8
			if (galcom_ver > 2)
			{
		      	for(i=1; i<=cons[nmbcars]; i++)
		      	{
					uiu.dw = grp_Car_Stat[i].g_statusf3;
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
					uiu.dw = grp_Car_Stat[i].g_statusf4;
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
		      	}
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 32:		// Send group i/o data
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 32;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(nmb_hcio_brds);
			for(i=0;i<(nmb_hcio_brds * 3);i++)
				tranbuf[com_nmb][txptr[com_nmb]++] = giodata(i);		
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
			
		case 33:			// send car call latched acknowledgement packet
			tranbuf[com_nmb][txptr[com_nmb]++] = 33;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cc_latched_ack;
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 40: // Detailed faults
			if (com_rst_flt_ix[com_nmb] == 1)
			{
				com_flt_ix[com_nmb] = Flt_ix + 1;
				if (com_flt_ix[com_nmb] >= max_flt_ix)
			    	com_flt_ix[com_nmb] = 0;
				com_rst_flt_ix[com_nmb] = 0;
			}

			if (galcom_ver > 2)
				max_pkt_len = 844;			// 169 * 5 = 845
			else
				max_pkt_len = 804;			// 161 * 5 = 805
			
			tranbuf[com_nmb][txptr[com_nmb]++] =  40;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
			// number of error codes
			nmb_error_codes_ix = txptr[com_nmb];
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;		// zero number of error codes to start but update at end
			nmb_error_codes = 0;
			i = 0;

			while ((com_loop_ix[com_nmb] < max_flt_ix) && (i < max_pkt_len) && (gc_dev_online[com_car_req[com_nmb]] != 0))
			{
				if (com_car_req[com_nmb] != cons[carnmb])
					flp = &cf.Flts[com_flt_ix[com_nmb]];
				else
					flp = &f.Flts[com_flt_ix[com_nmb]];
							
				if (flp->code != 0)
				{
					nmb_error_codes++;
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_flt_ix[com_nmb];			// send index #
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)flp->code;
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((flp->code >> 8) & 0x00ff);	// high byte
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->day;                        // day
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->mon;                        // month
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((flp->year >> 8) & 0x00FF);     	// HIGH BYTE year 
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(flp->year & 0x00FF);          	// LOW BYTE year 
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->min;                        // minute
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->hour;                       // hour
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->sec;						// seconds
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->pos;                        // position
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(flp->count & 0x00ff);           // low byte nmb occur
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->servf;						// service flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->procf;						// procedure flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->doorf;						// door flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->rdoorf;						// rear door flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->dpref;						// direction preference flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->dirf;						// direction flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->empf;						// emergency power car status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->medf;						// Mass. Medical Emergency Service
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->codebf;						// code blue flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->eqf;						// earthquake flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->firef;						// fire flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->rfiref;						// rear door fire flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->hsf;						// high speed flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->startf;						// start flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->dcalls;						// direction of calls
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->estop;						// emergency stop
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->nstopf;						// need to stop flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->relevel;					// relevel
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->stepf;						// position stepping flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->door_seqf;					// door sequence flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->next_stop;						// next stop
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->ins_status;					// inspection status
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->ncu_door_seq;				// ncu door sequence
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->device;						// device that had the fault
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->prog_flag1;					// prog flag 1
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->prog_flag2;					// prog flag 2
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->device_2;					// device 2 that had the fault
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[0];					// io data 0
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[1];					// io data 1
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[2];					// io data 2
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[3];					// io data 3
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[4];					// io data 4
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[5];					// io data 5
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[6];					// io data 6
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[7];					// io data 7
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[8];					// io data 8
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[9];					// io data 9
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[10];					// io data 10
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[11];					// io data 11
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[12];					// io data 12
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[13];					// io data 13
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[14];					// io data 18
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[15];					// io data 19
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[16];					// io data 138
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[17];					// io data 140
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[18];					// io data 141
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[19];					// io data 142
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[20];					// io data 143
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[21];					// io data 144
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[22];					// io data 145
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[23];					// io data 146
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[24];					// io data 147
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[25];					// io data 149
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[26];					// io data 153	   65
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[27];					// io data 147
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[28];					// io data 149
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[29];					// io data 153	   68
					uiu.dw = flp->statusf;								// status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
					uiu.dw = flp->dpp_count;							// digital position current count in pulses
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
					uiu.dw = flp->sd_count;							// target's digital position pulse count
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];		   // 80
#if (Traction == 1)
					uwu.w = flp->dmd_vel_fpm;		// demand velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
					uwu.w = flp->enc_vel_fpm;		// encoder velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_status1;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_status2;

					uwu.w = flp->NTS_vel;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

					uiu.dw = flp->NTS_poscnt;							// target's digital position pulse count
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];	 // 90
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

					uwu.w = flp->NTS_command1;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_command1;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_command2;

					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_doorzone;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_limits;		 
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_vel_dir;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;

					tranbuf[com_nmb][txptr[com_nmb]++] = flp->fault_bits[0];	  // 100
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->fault_bits[1];
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->fault_bits[2];		
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->fault_bits[3];		

					uiu.dw = flp->statusf2;								// status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

					uwu.w = flp->ss_status;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

					uwu.w = flp->power_status;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;

					tranbuf[com_nmb][txptr[com_nmb]++] = 0;		// 114
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;

					uiu.dw = flp->run_statusf;								// status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
	
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->nudgst_flags;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->door_req_flags;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->call_flags;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->chkrun_st;		 // 130
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->chkstart_st;		
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->chklevel_st;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->chkdoor_st;
					uwu.w = flp->front_sd;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					uwu.w = flp->rear_sd;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					uwu.w = flp->motion_tmr;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
		
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->enc_dir;		   // 140
			
#if (Traction == 1)
					uwu.w = flp->vel_fpm;			// calculated velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
					uwu.w = flp->drv_vel_fpm;		// drive demand velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					
					
#if (Traction == 1)
					uwu.w = flp->PAL_vel;			// calculated velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->PAL_stat;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->ins_servf;
#if (Traction == 1)
					uwu.w = flp->drv_cmd;			// drive command
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					
					uwu.w = (uint16)flp->drv_status;			// drive status
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					
#if (Traction == 1)
					uwu.w = flp->drv_mtr_torque;			// drive motor torque
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					
	
#if (Traction == 1)
					uwu.w = flp->drv_torque_cmd;			// drive torque
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
					uwu.w = flp->percent_load;			// percent load
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->lev_startf;		
 					tranbuf[com_nmb][txptr[com_nmb]++] = flp->spare;				  // 160 

					if (galcom_ver > 2)
					{
						uiu.dw = flp->statusf3;								// status flag
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

						uiu.dw = flp->statusf4;								// status flag
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

						i+=169;	  // 168  + 1 for fault index 169
					}
					else
						i+=161;	  // 160  + 1 for fault index 161
				}
				com_flt_ix[com_nmb]++;
				if (com_flt_ix[com_nmb] >= max_flt_ix)
			    	com_flt_ix[com_nmb] = 0;
				com_loop_ix[com_nmb]++;
			}
			if ((com_loop_ix[com_nmb] >= max_flt_ix)||(gc_dev_online[com_car_req[com_nmb]] == 0))
			{
				com_loop_ix[com_nmb] = 0;
				tranbuf[com_nmb][txptr[com_nmb]++] = 1;		   // last packet flag
			}
			else
			{
				put_pkt_req (40, cons[carnmb], com_nmb);	// not finished so put packet back on queue
				tranbuf[com_nmb][txptr[com_nmb]++] = 0;		   // last packet flag
			}
			len = txptr[com_nmb];
			tranbuf[com_nmb][nmb_error_codes_ix] = (uint8)nmb_error_codes;		// number of error codes sent
			
			valid_packet = 1;
			break;

		case 42:			// Send job statictics
			tranbuf[com_nmb][txptr[com_nmb]++] =  42;					// packet number
	      	for(i=0; i<=12; i++)
	      	{
				uiu.dw = lvars[i];
		  		tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
		  		tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
		  		tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
		  		tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
	      	}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 44:		   // Send eta data for each car
			if (com_car_req[com_nmb] <= cons[nmbcars])
			{
				tranbuf[com_nmb][txptr[com_nmb]++] =  44;					// packet number
	      		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[grtopf];

			  	for(i=2; i<= cons[grtopf]; i++)
			  	{
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)car_dn_eta[com_car_req[com_nmb]][i];
			  	}
			  	for(i=1; i<cons[grtopf]; i++)
			  	{
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)car_up_eta[com_car_req[com_nmb]][i];
			  	}
			  	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[com_car_req[com_nmb]].g_nxt_stp;
				len = txptr[com_nmb];
				valid_packet = 1;
	      	}
			break;


		case 45: // Trace data
		
			tranbuf[com_nmb][txptr[com_nmb]++] =  45;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];

			if (Trace_Stop == 1)
			{
				
				// set offset to beginning of trace
				if ((trace_stop_ix + 36) >= max_trace_ix)		// go to beginning of trace
					trace_offset_ix[com_nmb] = (int16)(36 - (max_trace_ix - trace_stop_ix));
				else
					trace_offset_ix[com_nmb] = (int16)(trace_stop_ix + 36);
					
				// add ui index to offset					
				if ((trace_offset_ix[com_nmb] + com_trace_ix[com_nmb]) >= max_trace_ix)
					trace_offset_ix[com_nmb] = com_trace_ix[com_nmb] - (max_trace_ix - trace_offset_ix[com_nmb]);
				else
					trace_offset_ix[com_nmb] += com_trace_ix[com_nmb];
				
				ptr = &S_Trace[trace_offset_ix[com_nmb]];
			}
			else
			{
				ptr = &S_Trace[Trace_ix];
			}
			
	  		tranbuf[com_nmb][txptr[com_nmb]++] = Trace_Stop;
			uwu.w = (uint16)com_trace_ix[com_nmb];			// send index 
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ptr->code;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((ptr->code >> 8) & 0x00ff);	// high byte
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->day;                        // day
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->mon;                        // month
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((ptr->year >> 8) & 0x00FF);     	// HIGH BYTE year 
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(ptr->year & 0x00FF);          	// LOW BYTE year 
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->min;                        // minute
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->hour;                       // hour
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->sec;						// seconds
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->pos;                        // position
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(ptr->count & 0x00ff);           // low byte nmb occur
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->servf;						// service flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->procf;						// procedure flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->doorf;						// door flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->rdoorf;						// rear door flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->dpref;						// direction preference flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->dirf;						// direction flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->empf;						// emergency power car status flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->medf;						// Mass. Medical Emergency Service
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->codebf;						// code blue flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->eqf;						// earthquake flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->firef;						// fire flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->rfiref;						// rear door fire flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->hsf;						// high speed flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->startf;						// start flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->dcalls;						// direction of calls
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->estop;						// emergency stop
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->nstopf;						// need to stop flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->relevel;					// relevel
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->stepf;						// position stepping flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->door_seqf;					// door sequence flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->next_stop;						// next stop
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->ins_status;					// inspection status
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->ncu_door_seq;				// ncu door sequence
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->device;						// device that had the fault
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->prog_flag1;					// prog flag 1
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->prog_flag2;					// prog flag 2
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->device_2;					// device 2 that had the fault
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[0];					// io data 0
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[1];					// io data 1
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[2];					// io data 2
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[3];					// io data 3
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[4];					// io data 4
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[5];					// io data 5
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[6];					// io data 6
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[7];					// io data 7
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[8];					// io data 8
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[9];					// io data 9
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[10];					// io data 10
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[11];					// io data 11
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[12];					// io data 12
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[13];					// io data 13
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[14];					// io data 18
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[15];					// io data 19
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[16];					// io data 138
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[17];					// io data 140
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[18];					// io data 141
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[19];					// io data 142
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[20];					// io data 143
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[21];					// io data 144
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[22];					// io data 145
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[23];					// io data 146
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[24];					// io data 147
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[25];					// io data 149
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[26];					// io data 153	   65
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[27];					// io data 147
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[28];					// io data 149
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->iodata[29];					// io data 153	   68
			uiu.dw = ptr->statusf;								// status flag
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = ptr->dpp_count;							// digital position current count in pulses
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = ptr->sd_count;							// target's digital position pulse count
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];		   // 80
#if (Traction == 1)
			uwu.w = ptr->dmd_vel_fpm;		// demand velocity
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
			uwu.w = ptr->enc_vel_fpm;		// encoder velocity
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->NTS_status1;
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->NTS_status2;

			uwu.w = ptr->NTS_vel;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uiu.dw = ptr->NTS_poscnt;							// target's digital position pulse count
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];	 // 90
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->NTS_command1;
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->NTS_command2;

			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->NTS_doorzone;
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->NTS_limits;		 
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->NTS_vel_dir;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;

			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->fault_bits[0];	  // 100
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->fault_bits[1];
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->fault_bits[2];		
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->fault_bits[3];		

			uiu.dw = ptr->statusf2;								// status flag
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uwu.w = ptr->ss_status;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = ptr->power_status;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;

			tranbuf[com_nmb][txptr[com_nmb]++] = 0;		// 114
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;

			uiu.dw = ptr->run_statusf;								// status flag
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
	
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->nudgst_flags;
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->door_req_flags;
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->call_flags;
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->chkrun_st;		 // 130
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->chkstart_st;		
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->chklevel_st;
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->chkdoor_st;
			uwu.w = ptr->front_sd;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = ptr->rear_sd;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = ptr->motion_tmr;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
		
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->enc_dir;		   // 140
			
#if (Traction == 1)
			uwu.w = ptr->vel_fpm;			// calculated velocity
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
			uwu.w = ptr->drv_vel_fpm;		// drive demand velocity
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					
					
#if (Traction == 1)
			uwu.w = ptr->PAL_vel;			// calculated velocity
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->PAL_stat;
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->ins_servf;
#if (Traction == 1)
			uwu.w = ptr->drv_cmd;			// drive command
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			
			uwu.w = (uint16)ptr->drv_status;								// drive status flag
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

#if (Traction == 1)
			uwu.w = ptr->drv_mtr_torque;			// drive motor torque
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
	
#if (Traction == 1)
			uwu.w = ptr->drv_torque_cmd;			// drive torque command
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
			uwu.w = ptr->percent_load;			// percent load
#else
			uwu.w = 0;
#endif
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->lev_startf;		
 			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->spare;				  // 160 
 			
			if (galcom_ver > 2)
			{
				uiu.dw = ptr->statusf3;								// status flag
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

				uiu.dw = ptr->statusf4;								// status flag
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			}
			

			len = txptr[com_nmb];
			
			valid_packet = 1;
			break;

		case 46:			// Send Trace Status
			tranbuf[com_nmb][txptr[com_nmb]++] =  46;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[carnmb];
	  		tranbuf[com_nmb][txptr[com_nmb]++] = Trace_Trigger;
	  		tranbuf[com_nmb][txptr[com_nmb]++] = Trace_Stop;
	  		tranbuf[com_nmb][txptr[com_nmb]++] = 0;
	  		tranbuf[com_nmb][txptr[com_nmb]++] = 0;
			len = txptr[com_nmb];
			valid_packet = 1;
			break;


		case 47:			// Send Service and Process Table 
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 47;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = last_servf+1;			// number of service flags
			for(i=0;i<=last_servf;i++)
			{
				for(k=0;k<20;k++)
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Service[i][k];		// taken from video.c (left justified)
			}
			for(i=0;i<=last_servf;i++)				
			{
				for(k=0;k<3;k++)
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)servf_tbl[i][k];		// taken from video.c
			}
	      	tranbuf[com_nmb][txptr[com_nmb]++] = last_procf+1;			// number of process flags
			for(i=0;i<=last_procf;i++)				
			{
				for(k=0;k<18;k++)
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)procf_table[i][k];		// taken from tracedpy.c
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 48: // Detailed faults (one fault at a time)
			if (com_flt_type[com_nmb] == 0)
			{
				Fault_ix = Flt_ix;
				max_fault_ix = max_flt_ix;
			}
			else
			{
				Fault_ix = LT_Flt_ix;
				max_fault_ix = max_lt_flt_ix;
			}
	
			if ((com_flt_ix[com_nmb] < max_fault_ix) && (com_car_req[com_nmb] == cons[carnmb]))
			{
				if ((Fault_ix + 1) >= max_fault_ix)
					flt_offset_ix[com_nmb] = 0;
				else
					flt_offset_ix[com_nmb] = (int16)(Fault_ix + 1);
				
				if ((flt_offset_ix[com_nmb] + com_flt_ix[com_nmb]) >= max_fault_ix)
					flt_offset_ix[com_nmb] = com_flt_ix[com_nmb] - (max_fault_ix - flt_offset_ix[com_nmb]);
				else
					flt_offset_ix[com_nmb] += com_flt_ix[com_nmb];
				
//				max_pkt_len = 161;			// 161 
				tranbuf[com_nmb][txptr[com_nmb]++] =  48;					// packet number
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
				// number of error codes
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_flt_type[com_nmb];		// com fault type
				

				if (com_flt_type[com_nmb] == 0)
					flp = &f.Flts[flt_offset_ix[com_nmb]];
				else
				{
					rd_LT_flts(flt_offset_ix[com_nmb], com_ltf.fltw);
					flp = &com_ltf.Flt;
				}
				
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_flt_ix[com_nmb];			// send index #
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)flp->code;
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((flp->code >> 8) & 0x00ff);	// high byte

				if (flp->code != 0)
				{
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->day;                        // day
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->mon;                        // month
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((flp->year >> 8) & 0x00FF);     	// HIGH BYTE year 
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(flp->year & 0x00FF);          	// LOW BYTE year 
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->min;                        // minute
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->hour;                       // hour
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->sec;						// seconds
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->pos;                        // position
					tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(flp->count & 0x00ff);           // low byte nmb occur
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->servf;						// service flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->procf;						// procedure flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->doorf;						// door flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->rdoorf;						// rear door flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->dpref;						// direction preference flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->dirf;						// direction flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->empf;						// emergency power car status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->medf;						// Mass. Medical Emergency Service
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->codebf;						// code blue flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->eqf;						// earthquake flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->firef;						// fire flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->rfiref;						// rear door fire flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->hsf;						// high speed flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->startf;						// start flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->dcalls;						// direction of calls
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->estop;						// emergency stop
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->nstopf;						// need to stop flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->relevel;					// relevel
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->stepf;						// position stepping flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->door_seqf;					// door sequence flag
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->next_stop;						// next stop
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->ins_status;					// inspection status
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->ncu_door_seq;				// ncu door sequence
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->device;						// device that had the fault
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->prog_flag1;					// prog flag 1
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->prog_flag2;					// prog flag 2
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->device_2;					// device 2 that had the fault
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[0];					// io data 0
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[1];					// io data 1
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[2];					// io data 2
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[3];					// io data 3
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[4];					// io data 4
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[5];					// io data 5
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[6];					// io data 6
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[7];					// io data 7
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[8];					// io data 8
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[9];					// io data 9
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[10];					// io data 10
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[11];					// io data 11
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[12];					// io data 12
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[13];					// io data 13
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[14];					// io data 18
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[15];					// io data 19
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[16];					// io data 138
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[17];					// io data 140
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[18];					// io data 141
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[19];					// io data 142
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[20];					// io data 143
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[21];					// io data 144
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[22];					// io data 145
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[23];					// io data 146
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[24];					// io data 147
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[25];					// io data 149
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[26];					// io data 153	   65
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[27];					// io data 147
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[28];					// io data 149
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->iodata[29];					// io data 153	   68
					uiu.dw = flp->statusf;								// status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
					uiu.dw = flp->dpp_count;							// digital position current count in pulses
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
					uiu.dw = flp->sd_count;							// target's digital position pulse count
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];		   // 80
#if (Traction == 1)
					uwu.w = flp->dmd_vel_fpm;		// demand velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
					uwu.w = flp->enc_vel_fpm;		// encoder velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_status1;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_status2;

					uwu.w = flp->NTS_vel;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

					uiu.dw = flp->NTS_poscnt;							// target's digital position pulse count
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];	 // 90
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_command1;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_command2;

					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_doorzone;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_limits;		 
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->NTS_vel_dir;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;

					tranbuf[com_nmb][txptr[com_nmb]++] = flp->fault_bits[0];	  // 100
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->fault_bits[1];
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->fault_bits[2];		
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->fault_bits[3];		

					uiu.dw = flp->statusf2;								// status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

					uwu.w = flp->ss_status;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

					uwu.w = flp->power_status;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;

					tranbuf[com_nmb][txptr[com_nmb]++] = 0;		// 114
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;
					tranbuf[com_nmb][txptr[com_nmb]++] = 0;

					uiu.dw = flp->run_statusf;								// status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
	
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->nudgst_flags;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->door_req_flags;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->call_flags;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->chkrun_st;		 // 130
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->chkstart_st;		
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->chklevel_st;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->chkdoor_st;
					uwu.w = flp->front_sd;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					uwu.w = flp->rear_sd;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					uwu.w = flp->motion_tmr;
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
		
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->enc_dir;		   // 140
			
#if (Traction == 1)
					uwu.w = flp->vel_fpm;			// calculated velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
					uwu.w = flp->drv_vel_fpm;		// drive demand velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					
					
#if (Traction == 1)
					uwu.w = flp->PAL_vel;			// calculated velocity
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->PAL_stat;
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->ins_servf;
#if (Traction == 1)
					uwu.w = flp->drv_cmd;			// drive command
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					
					uwu.w = (uint16)flp->drv_status;								// drive status flag
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
	
#if (Traction == 1)
					uwu.w = flp->drv_mtr_torque;			// drive motor torque
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

#if (Traction == 1)
					uwu.w = flp->drv_torque_cmd;			// drive torque command
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
#if (Traction == 1)
					uwu.w = flp->percent_load;			// percent load
#else
					uwu.w = 0;
#endif
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->lev_startf;
							
 					if (galcom_ver > 2)
					{
						uiu.dw = flp->statusf3;								// status flag
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

						uiu.dw = flp->statusf4;								// status flag
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
						tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
					}
					
					tranbuf[com_nmb][txptr[com_nmb]++] = flp->spare;				  // 160 

				}
			}
			len = txptr[com_nmb];
			
			valid_packet = 1;
			break;

		case 49:			// Send Car Call Test Status  
							// car call test status  bit 0: enable_cc_test
							//						 bit 1: continue_cc_test
							//						 bit 2: cancel_cc_test
			tranbuf[com_nmb][txptr[com_nmb]++] =  49;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[carnmb];
	  		tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((enable_cc_test & 0x01) | ((continue_cc_test & 0x01) << 1) | ((cancel_cc_test & 0x01) << 2));
			uiu.dw = cc_test_mask[0];							
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = cc_test_mask[1];							
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 53:		// Send a security code table

			if ((Com_Sec_Fl_Tbl_Req[com_nmb] > 0) && (Com_Sec_Fl_Tbl_Req[com_nmb] <= cons[grtopf]))
			{
				Read_CodeTbl(Com_Sec_Fl_Tbl_Req[com_nmb],&Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
				tranbuf[com_nmb][txptr[com_nmb]++] =  53;					// packet number
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Com_Sec_Fl_Tbl_Req[com_nmb];				// floor
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)Com_CodeTable[com_nmb].NumCode;			// number of codes in table

				i = 0;
				while (i < Com_CodeTable[com_nmb].NumCode)
				{
					tranbuf[com_nmb][txptr[com_nmb]++] = Com_CodeTable[com_nmb].Codes[i][0];
					tranbuf[com_nmb][txptr[com_nmb]++] = Com_CodeTable[com_nmb].Codes[i][1];
					tranbuf[com_nmb][txptr[com_nmb]++] = Com_CodeTable[com_nmb].Codes[i][2];
					tranbuf[com_nmb][txptr[com_nmb]++] = Com_CodeTable[com_nmb].Codes[i][3];
					i++;
				}
				len = txptr[com_nmb];
				valid_packet = 1;
			}
			break;

		case 54:				 // send request for security code table
	     	tranbuf[com_nmb][txptr[com_nmb]++] = 54;
	      	tranbuf[com_nmb][txptr[com_nmb]++] = Com_Sec_Fl_Tbl_Req[com_nmb];
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 55:			 // Send security checksum table
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 55;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[grtopf];

			for (i = 1;i <= cons[grtopf];i++)
			{
				uwu.w = Code_Tbl_Chk[i].NumCode;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				uwu.w = Code_Tbl_Chk[i].CheckSum;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 57:		// Send User Interface front and rear hall call security floors
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 57;					// packet number
	      		// front
			uiu.dw = ui_uc_sec[0];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = ui_uc_sec[1];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = ui_dc_sec[0];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = ui_dc_sec[1];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
				// rear
			uiu.dw = ui_ruc_sec[0];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = ui_ruc_sec[1];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = ui_rdc_sec[0];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			uiu.dw = ui_rdc_sec[1];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			len = txptr[com_nmb];
			valid_packet = 1;
			break;


		case 59:			// Send User Interface front car call security floors
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 59;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[nmbcars];
			for (i=1;i<=cons[nmbcars];i++)
			{
				uiu.dw = ui_cc_sec[0][i];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
				uiu.dw = ui_cc_sec[1][i];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 61:			// Send User Interface rear car call security floors
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 61;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[nmbcars];
			for (i=1;i<=cons[nmbcars];i++)
			{
				uiu.dw = ui_rcc_sec[0][i];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
				uiu.dw = ui_rcc_sec[1][i];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
			
		case 63:		// Send User Interface front hall call security floors
			el =com_car_req[com_nmb];
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 63;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];

			uiu.dw = ui_uhc_carsec[0][el];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = ui_uhc_carsec[1][el];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = ui_dhc_carsec[0][el];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = ui_dhc_carsec[1][el];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			len = txptr[com_nmb];
			valid_packet = 1;
			break;
			
		case 65:		// Send User Interface rear hall call security floors
			el =com_car_req[com_nmb];
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 65;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
			
			uiu.dw = ui_ruhc_carsec[0][el];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			
			uiu.dw = ui_ruhc_carsec[1][el];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			
			uiu.dw = ui_rdhc_carsec[0][el];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			
			uiu.dw = ui_rdhc_carsec[1][el];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			len = txptr[com_nmb];
			valid_packet = 1;
			break;


		case 68:		// Send Trace Variables
		
			tranbuf[com_nmb][txptr[com_nmb]++] =  68;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];
	
			uiu.dw = trigger_logic;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = and_trigger_logic;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
		
			uiu.dw = trigger_enable;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uwu.w = trigger_arm;								
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = trigger_window;								
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = trace_interval;								
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = trigger_fault;								
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = trigger_servf;								
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = trigger_procf;								
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
	
			uiu.dw = trigger_run_statusf;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uwu.w = trigger_front_sd;								
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = trigger_rear_sd;								
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uiu.dw = trigger_statusf;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

			uiu.dw = trigger_statusf2;
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
	
			tranbuf[com_nmb][txptr[com_nmb]++] = trigger_fault_bits[0];
			tranbuf[com_nmb][txptr[com_nmb]++] = trigger_fault_bits[1];
			tranbuf[com_nmb][txptr[com_nmb]++] = trigger_fault_bits[2];
			tranbuf[com_nmb][txptr[com_nmb]++] = trigger_fault_bits[3];

			if (galcom_ver > 2)
			{
				uiu.dw = trigger_statusf3;
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];

				uiu.dw = trigger_statusf4;
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
	
			}

			len = txptr[com_nmb];
			valid_packet = 1;
			break;

			
		case 69: // Send Car Trace Motion data
		

			tranbuf[com_nmb][txptr[com_nmb]++] =  69;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)com_car_req[com_nmb];

			if (Trace_Stop == 1)
			{
				galcom_req[com_nmb] = 0;		// no auto request when the trace has stopped
				
				// set offset to beginning of trace
				if ((trace_stop_ix + 36) >= max_trace_ix)		// go to beginning of trace
					trace_offset_ix[com_nmb] = (int16)(36 - (max_trace_ix - trace_stop_ix));
				else
					trace_offset_ix[com_nmb] = (int16)(trace_stop_ix + 36);
					
				// add ui index to offset					
				if ((trace_offset_ix[com_nmb] + com_trace_ix[com_nmb]) >= max_trace_ix)
					trace_offset_ix[com_nmb] = com_trace_ix[com_nmb] - (max_trace_ix - trace_offset_ix[com_nmb]);
				else
					trace_offset_ix[com_nmb] += com_trace_ix[com_nmb];
				
				ptr = &S_Trace[trace_offset_ix[com_nmb]];
			}
			else
			{
				ptr = &S_Trace[Trace_ix];
			}
			
	  		tranbuf[com_nmb][txptr[com_nmb]++] = Trace_Stop;
			uwu.w = (uint16)com_trace_ix[com_nmb];			// send index 
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->pos;                        // position
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->servf;						// service flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->procf;						// procedure flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->doorf;						// door flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->rdoorf;						// rear door flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->dpref;						// direction preference flag
			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->dirf;						// direction flag

			// Current Fault
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ptr->code;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((ptr->code >> 8) & 0x00ff);	// high byte

			tranbuf[com_nmb][txptr[com_nmb]++] = ptr->enc_dir;		   
			
			uwu.w = ptr->dmd_vel_fpm;		//  demand velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = ptr->enc_vel_fpm;		// encoder velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uiu.dw = ptr->dpp_count;							// digital position current count in pulses
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			
			uiu.dw = ptr->sd_count;							// target's digital position pulse count
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];		   // 80

			uiu.dw = ptr->run_statusf;								// status flag
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
	
			uwu.w = ptr->drv_vel_fpm;		//  drive demand velocity
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			uwu.w = ptr->motion_tmr;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
		
			uwu.w = accel_x;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = accel_y;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
			uwu.w = accel_z;
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];

			len = txptr[com_nmb];
			
			valid_packet = 1;
			break;
			
		case 70: 			// Send Group Service Timers
			j = com_svc_tmr[com_nmb];
			if ((j >= 0) && (j <= 9))
			{
		      	tranbuf[com_nmb][txptr[com_nmb]++] = 70;					// packet number
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)j;					// service timer number
			  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].service;
			  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].status;
				for (i=0;i<=2;i++)
				{
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].month[i];
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].day[i];
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].md_on_hour[i];
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].md_off_hour[i];
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].md_on_minute[i];
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].md_off_minute[i];
				}
				for (i=0;i<=6;i++)
				{
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].on_hour[i];
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].off_hour[i];
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].on_minute[i];
				  	tranbuf[com_nmb][txptr[com_nmb]++] = svc_t[j].off_minute[i];
				}
				len = txptr[com_nmb];
				valid_packet = 1;
			}
			break;

		case 72:			// Send Line voltage for user interface
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 72;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[nmbcars];
			for (i=1;i<=cons[nmbcars];i++)
			{
				uwu.w = grp_Car_Stat[i].g_L1_Voltage;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				uwu.w = grp_Car_Stat[i].g_L2_Voltage;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				uwu.w = grp_Car_Stat[i].g_L3_Voltage;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				uwu.w = grp_Car_Stat[i].g_Door_Voltage;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_Voltage_Dev_Online;		// added additional byte
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 73:			// Car data for fault and service change detection
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 73;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[nmbcars];

	      	for(i=1; i<=cons[nmbcars]; i++)
	      	{
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_pos;
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_servf;
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_procf;
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_firef;
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_ins_status;
		      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_fire_status;
				uwu.w = grp_Car_Stat[i].g_cur_flt;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				uiu.dw = grp_Car_Stat[i].g_statusf1;
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
				uiu.dw = grp_Car_Stat[i].g_statusf2;
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
				tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			  	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grp_Car_Stat[i].g_pwr_status;
				uwu.w = grp_Car_Stat[i].g_ss_stat;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				
				if (galcom_ver > 2)
				{
						
					uiu.dw = grp_Car_Stat[i].g_statusf3;
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
					uiu.dw = grp_Car_Stat[i].g_statusf4;
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
					tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
				}
	      	}
	      	
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
			
		case 74:			// Send User Interface Control Variables
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 74;					// packet number
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_uppeak;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_dnpeak;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_alt_lby;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_alt_park;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_ncu;
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_alt_fl_sec;
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
			tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[nmbcars];
			for (i=1;i<=cons[nmbcars];i++)
			{
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_ind[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_rtl[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_heof[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ui_ad_off[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
				tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
				tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
				tranbuf[com_nmb][txptr[com_nmb]++] = 0;			// spare
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
			

			
		case 76:		// Send Group Alarms
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 76;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[nmbcars];
			for (i=1;i<=cons[nmbcars];i++)
			{ 
				tranbuf[com_nmb][txptr[com_nmb]++] = (unsigned char)grp_Car_Stat[i].g_car_alrm;		  
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
			
		case 77:			// Send Current Faults
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 77;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[nmbcars];
			for (i=1;i<=cons[nmbcars];i++)
			{
			   	uwu.w = grp_Car_Stat[i].g_cur_flt;
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[hbw];
				tranbuf[com_nmb][txptr[com_nmb]++] = uwu.b[lbw];
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
			
			
		case 80:			// Send command data terminal mode
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 80;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)cons[carnmb];
	      	i = 0;
			while ((tbs[TERMCOM] != tbf[TERMCOM]) && (i<500))
			{
				tranbuf[com_nmb][txptr[com_nmb]++] = tranbuf[TERMCOM][i];
				tbf[TERMCOM]++;
				if(tbf[TERMCOM] >= tbufsize)
					tbf[TERMCOM] = 0;
				i++;
			}
			Tranbuf_Empty[TERMCOM] = 1;
			Tranbuf_Full[TERMCOM] = 0;
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
			
		case 90:	//CTC CAR TOP CAN COMMUNICATION STATUS
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 90;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)nmb_ctcan_dev;
			for (i=0;i<nmb_ctcan_dev;i++)
			{ 
				tranbuf[com_nmb][txptr[com_nmb]++] = ctcan_dev_list[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)ctcan_addr[ctcan_dev_list[i]];
				tranbuf[com_nmb][txptr[com_nmb]++] = ctcan_dev[ctcan_dev_list[i]].version;
				tranbuf[com_nmb][txptr[com_nmb]++] = ctcan_dev[ctcan_dev_list[i]].revision;
				tranbuf[com_nmb][txptr[com_nmb]++] = ctcan_dev[ctcan_dev_list[i]].online;	
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 91:		//MRC CAR TOP CAN COMMUNICATION STATUS
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 91;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)nmb_mrcan_dev;
			for (i=0;i<nmb_mrcan_dev;i++)
			{ 
				tranbuf[com_nmb][txptr[com_nmb]++] = mrcan_dev_list[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)mrcan_addr[mrcan_dev_list[i]];
				tranbuf[com_nmb][txptr[com_nmb]++] = mrcan_dev[mrcan_dev_list[i]].version;
				tranbuf[com_nmb][txptr[com_nmb]++] = mrcan_dev[mrcan_dev_list[i]].revision;
				tranbuf[com_nmb][txptr[com_nmb]++] = mrcan_dev[mrcan_dev_list[i]].online;		  
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 92:		//GCC CAR TOP CAN COMMUNICATION STATUS
	      	tranbuf[com_nmb][txptr[com_nmb]++] = 92;					// packet number
	      	tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)nmb_grcan_dev;
			for (i=0;i<nmb_grcan_dev;i++)
			{ 
				tranbuf[com_nmb][txptr[com_nmb]++] = grcan_dev_list[i];
				tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)grcan_addr[grcan_dev_list[i]];
				tranbuf[com_nmb][txptr[com_nmb]++] = grcan_dev[grcan_dev_list[i]].version;
				tranbuf[com_nmb][txptr[com_nmb]++] = grcan_dev[grcan_dev_list[i]].revision;
				tranbuf[com_nmb][txptr[com_nmb]++] = grcan_dev[grcan_dev_list[i]].online;		  
			}
			len = txptr[com_nmb];
			valid_packet = 1;
			break;

		case 93:		// Auto Hoistway Learn
			break;
		case 94:
			break;
		case 95:
			break;
		case 96:
			break;
		case 97:
			tranbuf[com_nmb][txptr[com_nmb]++] = 97;
			uiu.dw = com_fvar_req[com_nmb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[lb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mlb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[mhb];
			tranbuf[com_nmb][txptr[com_nmb]++] = uiu.b[hb];
			
			i = fvars[com_fvar_req[com_nmb]];
			
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)(fvars[i] & 0x00ff);
			tranbuf[com_nmb][txptr[com_nmb]++] = (uint8)((fvars[i] >> 8) & 0x00ff);
			
			len = txptr[com_nmb];
			valid_packet = 1;
			break;
		default:
	    	break;
			
	}
	
	if (valid_packet == 1)
	{

		tranbuf[com_nmb][0] =  'P';				// packet Start Character
		uwu.w = (uint16)len+2;					// length = total packet length.
		tranbuf[com_nmb][1] = uwu.b[lbw];	 			// length low byte
		tranbuf[com_nmb][2] = uwu.b[hbw];	 			// length high byte
		uwu.w = (uint16)len+1;					// length check = length - 1.
		tranbuf[com_nmb][3] = uwu.b[lbw];	 			// length check low byte
		tranbuf[com_nmb][4] = uwu.b[hbw];	 			// length check high byte
		chksum = 0;
		for(i=1;i<len;i++)
		{
			chksum += tranbuf[com_nmb][i];
		}
		tranbuf[com_nmb][i++] = (uint8)chksum;
		tranbuf[com_nmb][i] = '@';
		if (com_nmb == USR1COM)
		{
			timers[tuser_com+com_nmb] = 0;
			com_1_tx_start(len+2);
		}
		else if (com_nmb == USR2COM)
		{
			timers[tuser_com+com_nmb] = 0;
			com_2_tx_start(len+2);
		}
	}

}


void galcom_unpack(int16 com_nmb, int16 msg)
{
  	int16 i,j,k,qq,bbb,car = 1;
	int16 ck_sum;
	int16 cm_cksum;
	int16 Var_Nmb;
	int32 address_code = 0;
	int32 start_address = 0;
  	int16 jump_status = 0;
	int16 len = 0;
	int16 fl_ix;
	int16 el;
	int16 rcv_nts_fvars = 0;
	int16 rcv_spb_fvars = 0;
	int16 rcv_fvars = 0;
	
	
	if (rmbuf[com_nmb][5] != msg)
		return;
	uwu.b[lbw] = (uint8)rmbuf[com_nmb][1];
	uwu.b[hbw] = (uint8)rmbuf[com_nmb][2];
	len = uwu.w;
	if (len > 500)
		return;
	ck_sum = 0;
	for (i=1;i<(len-2);i++)
	{
		ck_sum += rmbuf[com_nmb][i];
	}
	cm_cksum = rmbuf[com_nmb][i];
	if ((uint8)ck_sum != (uint8)cm_cksum)
		return;
	switch (msg)
	{
		case 1:	   // Log on  user
			logon[com_nmb] = 1;		// login attempt
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			// Check user id
	      	for(j=0; j<=6; j++)
	      	{
		      	i = 7;
		      	while((rmbuf[com_nmb][i] == user[j][(i-7)]) && (user[j][(i-7)] != CR))
		      	{
			    	i++;
		      	}
		      	if((user[j][(i-7)] == CR) && (i > 11))             // good user (min 5 char user)
		      	{
			   		logon[com_nmb] = 2;		// valie user id
			   		User_Log[com_nmb] = j;
					j = 7;			// exit for loop since valid user found
		      	}
			}
		  	if (logon[com_nmb] == 2)	   // received correct user 
		  	{		// Check for valid  password for this user
		      	j = User_Log[com_nmb];
		      	i = 7;
		      	while((rmbuf[com_nmb][i] == pass[j][(i-7)]) && (pass[j][(i-7)] != CR))
		      	{
			    	i++;
		      	}
		      	if((pass[j][(i-7)] == CR) && (i > 9))             // good password
		      	{
			   		logon[com_nmb] = 3;		// valid user id and password
		      	}
		  	}
		  	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
		  	com_access_packet[com_nmb] = 1;
			com_access_status[com_nmb] = 0;
			put_pkt_req(3,cons[carnmb],com_nmb);
			break;
		
		case 3:		// log off
			logon[com_nmb] = 0;
			Com_Mode[com_nmb] = 0;
		  	com_access_packet[com_nmb] = 3;
			com_access_status[com_nmb] = 0;
			com_access_level_pk3[com_nmb] = 0;
			put_pkt_req(3,cons[carnmb],com_nmb);
			break;

		case 4:	// write new user name
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access)) && (AD_status == 1))	   
		  	{
				if((rmbuf[com_nmb][8] > 1) && (rmbuf[com_nmb][8] < 7))
				{
					i = 9;
					while( (rmbuf[com_nmb][i] != CR) && (i < 27) )
					{
						user[rmbuf[com_nmb][8]][(i-9)] = rmbuf[com_nmb][i];
						i++;
					}
					user[rmbuf[com_nmb][8]][(i-9)] = CR;
					wruvar();
					put_pkt_req(4,cons[carnmb],com_nmb);
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		  		com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 4;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;

		case 5:			 // New Password
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access)) && (AD_status == 1))	   
		  	{
				if((rmbuf[com_nmb][8] > 1) && (rmbuf[com_nmb][8] < 7))
				{
					i = 9;
					while( (rmbuf[com_nmb][i] != 13) && (i < 27) )
					{
						pass[rmbuf[com_nmb][8]][(i-9)] = rmbuf[com_nmb][i];
						i++;
					}
					pass[rmbuf[com_nmb][8]][(i-9)] = CR;
					wruvar();
					put_pkt_req(5,cons[carnmb],com_nmb);
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 5;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
	  		break;
	  
		case 6:		// Set date and time
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access) || 
		  	    (com_access_level[com_nmb] == Building_Access)) && (AD_status == 1))	   
		  	{
		    	d.year = (uint16)(rmbuf[com_nmb][8] & 0xff);
		    	d.year = (uint16)((rmbuf[com_nmb][9] << 4) | d.year);
		    	d.month = (uint8)(rmbuf[com_nmb][10] & 0xff);
		    	d.day = (uint8)(rmbuf[com_nmb][11] & 0xff);
		    	t.hour = (uint8)(rmbuf[com_nmb][12] & 0xff);
		    	t.minute = (uint8)(rmbuf[com_nmb][13] & 0xff);
				local_setdate();
				local_settime();
				put_pkt_req_all_cars(109);		// Update time on all other cars
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access) &&
			  	    (com_access_level[com_nmb] != Building_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 6;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;
			
		case 7:		 // Get galcom version
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			galcom_ver = (uint16)(rmbuf[com_nmb][7] & 0xff);
			galcom_rev = (uint16)(rmbuf[com_nmb][8] & 0xff);
			user_com_ver[com_nmb] = (uint16)(rmbuf[com_nmb][9] & 0xff);
			user_com_type[com_nmb] = (uint16)(rmbuf[com_nmb][10] & 0xff);	 // 1=galcom, 2=galileo
			Com_Mode[com_nmb] = 0;
	  		put_pkt_req(7,cons[carnmb],com_nmb);
			break;
			
		case 8:
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			Restart_Com(com_nmb);
			break;
			
		case 9:		// request Encription Code
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req(9,cons[carnmb],com_nmb);
			break;
			
	  	case 10:	 // Request Group initializaton data
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req(10,cons[carnmb],com_nmb);
			break;

		case 11:		  // Extended group initialization 
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req(11,cons[carnmb],com_nmb);
			break;

		case 12:				   // Extended group initialization data
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req(12,cons[carnmb],com_nmb);
			break;

		case 13:			 // Send i/o board used for cars 
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
		  		put_pkt_req (13, cons[carnmb], com_nmb);
			break;

		case 14:		// Send which i/o are inputs 0's and outputs 1's
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
	  			put_pkt_req (14, cons[carnmb], com_nmb);
			break;

		case 15:			// Send valid i/o's 1= valid
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
	  			put_pkt_req (15, cons[carnmb], com_nmb);
			break;

		case 16:			// Send group i/o boards and valid i/o's 1 = valid
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req (16, cons[carnmb], com_nmb);
			break;

		case 17:		// Send car i/o names
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			com_io_brd[com_nmb] = (uint16)(rmbuf[com_nmb][8] & 0xff);
			if ((com_io_brd[com_nmb] < nmb_io_brds) && (com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
		  		put_pkt_req (17, cons[carnmb], com_nmb);
			break;

		case 18:			// Send group i/o names
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_io_brd[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
	  		put_pkt_req (18, cons[carnmb], com_nmb);
			break;

		case 19:		// Send which group i/o are inputs 0's and outputs 1's
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
  			put_pkt_req (19, cons[carnmb], com_nmb);
			break;

		case 20:					 // Request to send the field variables (600 variables are sent in five packets)
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	com_fvar_block_ix[com_nmb] = (uint16)(rmbuf[com_nmb][8] & 0xff);
		  	if ((com_fvar_block_ix[com_nmb] < 5) && (com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
		  		put_pkt_req (20, cons[carnmb], com_nmb);
			break;

		case 21:				  // Receive 120 field variables (600 variables are sent in five packets)

			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access)) && (AD_status == 1))	   
		  	{

				// Always get the checksum
				com_fvar_block_ix[com_nmb] = (uint16)(rmbuf[com_nmb][8] & 0xff);
				if (com_fvar_block_ix[com_nmb] < 5)
				{
					
					bbb = (uint16)(rmbuf[com_nmb][9]&0x00ff);		// read in checksum
					bbb |= (uint16)((rmbuf[com_nmb][10] << 8)&0xff00);
					ck_sum = 0;
					j = com_fvar_block_ix[com_nmb] * 120; 
					for(i=0; i<240; i=(i+2))
				    {				// calculate checksum
					  	qq = (uint16)(rmbuf[com_nmb][i+11]&0xff);
					  	qq |= (uint16)((rmbuf[com_nmb][i+12] << 8)&0xff00);
						ck_sum += qq;
				    }
					if (ck_sum == ((int16)bbb))
					{		// if checksum matches then update variables
						Var_Nmb = com_fvar_block_ix[com_nmb] * 120;
					  	for(i=0; i<240; i=(i+2))
					  	{	   
							if(Var_Nmb != fvbaud)
							{	// do not allow the laptop update the serial comm baud rate
							  	qq = (uint16)(rmbuf[com_nmb][i+11]&0xff);
							  	qq |= (uint16)((rmbuf[com_nmb][i+12] << 8)&0xff00);
								if ((qq >= fvars_min[Var_Nmb]) && (qq <= fvars_max[Var_Nmb]))
							  		fvars[Var_Nmb] = qq;
					  		}
							Var_Nmb++;
					  	}
					  	wrfvar();
					}
			  		put_pkt_req (20, cons[carnmb], com_nmb);
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 21;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
		  		
			break;

#if (Traction == 1)
		case 22:		 // Request digital speed clamp setting and terminal limit speeds
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req (22, cons[carnmb], com_nmb);
			break;
			
		
  		case 23:		  // receive digital speed clamp settings
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access)) && (AD_status == 1))	   
		  	{
				if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
				{
					
			  	  	bbb = (uint16)(rmbuf[com_nmb][8]&0xff);		// read in checksum
				  	bbb |= ((rmbuf[com_nmb][9] << 8)&0xff00);
					ck_sum = 0;
					j = 18;
				   	for(i=0; i<j; i=(i+2))
				   	{				// calculate checksum
					  	qq = (uint16)(rmbuf[com_nmb][i+10]&0xff);
					  	qq |= ((rmbuf[com_nmb][i+11] << 9)&0xff00);
						ck_sum += qq;
				   	}
					if (ck_sum == ((int16)bbb))
					{		// if checksum matches then update variables
						Var_Nmb = 0;
				      	for(i=0; i<j; i=(i+2))
				      	{	   
						  	qq = (uint16)(rmbuf[com_nmb][i+10]&0xff);
						  	qq |= ((rmbuf[com_nmb][i+11] << 8)&0xff00);
						  	speed_clamp[Var_Nmb] = qq;
							Var_Nmb++;
				      	}
				      	if (com_car_req[com_nmb] == cons[carnmb])
				      		wrfvar();
				      	//	else send packet to valid car
					}
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 23;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;
			
		case 24:			// Request Learn Hoistway monitor
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access)) && (AD_status == 1))	   
		  	{
				if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
		  			put_pkt_req(24,cons[carnmb],com_nmb);
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 23;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
		  	break;

#endif

		case 25:					 // Request to send the field variables 1  to 60 variables (index1,index2...index60)
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			com_fvar_cpu[com_nmb] = (uint16)(rmbuf[com_nmb][8] & 0xff);
			com_nmb_fvar[com_nmb] = (uint16)(rmbuf[com_nmb][9] & 0xff);
			
			if ((com_car_req[com_nmb] == cons[carnmb]) && (com_nmb_fvar[com_nmb] <= 60) && (com_fvar_cpu[com_nmb] > 0) && (com_fvar_cpu[com_nmb] <=3))
			{
				i = 0;
				j = 0;
				while (i < com_nmb_fvar[com_nmb])
				{
					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][j+10] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][j+11] & 0xff); 
					com_fvar_ix[com_nmb][i++] = uwu.w;
					j+=2;
				}
					
		  		put_pkt_req (26, cons[carnmb], com_nmb);
			}
			break;

		case 26:				  // Receive up to 60 fvars
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access)) && (AD_status == 1))	   
		  	{
				// Always get the checksum
				com_fvar_cpu[com_nmb] = (uint16)(rmbuf[com_nmb][8] & 0xff);
				com_nmb_fvar[com_nmb] = (uint16)(rmbuf[com_nmb][9] & 0xff);
				if ((com_car_req[com_nmb] == cons[carnmb]) && (com_nmb_fvar[com_nmb] <= 60) && (com_fvar_cpu[com_nmb] > 0) && (com_fvar_cpu[com_nmb] <=3))
				{
					
					bbb = (uint16)(rmbuf[com_nmb][10]&0x00ff);		// read in checksum
					bbb |= (uint16)((rmbuf[com_nmb][11] << 8)&0xff00);
					ck_sum = 0;
					i = 0;
					j = 0;
					while (i < com_nmb_fvar[com_nmb])
				    {				// calculate checksum
					  	qq = (uint16)(rmbuf[com_nmb][j+12]&0xff);
					  	qq |= (uint16)((rmbuf[com_nmb][j+13] << 8)&0xff00);	  // index
						ck_sum += qq;
						j+=2;
					  	qq = (uint16)(rmbuf[com_nmb][j+12]&0xff);
					  	qq |= (uint16)((rmbuf[com_nmb][j+13] << 8)&0xff00);	 // variable
						ck_sum += qq;
						j+=2;
						i++;
				    }
					if (ck_sum == ((int16)bbb))
					{		// if checksum matches then update variables
						i = 0;
						j = 0;
						NTS_eep_nmb=0;
						while (i < com_nmb_fvar[com_nmb])
						{
							uwu.b[lbw] = (uint8)(rmbuf[com_nmb][j+12] & 0xff); 
							uwu.b[hbw] = (uint8)(rmbuf[com_nmb][j+13] & 0xff); 
							com_fvar_ix[com_nmb][i] = uwu.w;
							j+=2;
							uwu.b[lbw] = (uint8)(rmbuf[com_nmb][j+12] & 0xff); 
							uwu.b[hbw] = (uint8)(rmbuf[com_nmb][j+13] & 0xff);
							if ((com_fvar_cpu[com_nmb] == MAIN_CPU) && (com_fvar_ix[com_nmb][i] < 600))
			      			{
								if ((uwu.w >= fvars_min[com_fvar_ix[com_nmb][i]]) && (uwu.w <= fvars_max[com_fvar_ix[com_nmb][i]]))
								{
									fvars[com_fvar_ix[com_nmb][i]] = uwu.w;
								}
			      			}
/*
			      			else if ((com_fvar_cpu[com_nmb] == SPB_CPU) && (com_fvar_ix[com_nmb][i] < numspbvars))
			      			{
								if ((uwu.w >= spb_fvars_min[com_fvar_ix[com_nmb][i]]) && (uwu.w <= spb_fvars_max[com_fvar_ix[com_nmb][i]]))
								{
									SPB_eep_ix = com_fvar_ix[com_nmb][i];
									spb_fvars[com_fvar_ix[com_nmb][i]] = uwu.w;
									SPB_eep_nmb++;
								}
			      			}
*/
			      			else if ((com_fvar_cpu[com_nmb] == NTS_CPU) && (com_fvar_ix[com_nmb][i] < numntsvars))
			      			{
			      				if ((uwu.w >= nts_fvars_min[com_fvar_ix[com_nmb][i]]) && (uwu.w <= nts_fvars_max[com_fvar_ix[com_nmb][i]]))
								{
									NTS_eep_ix = com_fvar_ix[com_nmb][i];
									nts_fvars[com_fvar_ix[com_nmb][i]] = uwu.w;
									NTS_eep_nmb++;
			      				}
			      			}
			      			
							j+=2;
							i++;
						}
						if (com_fvar_cpu[com_nmb] == MAIN_CPU)	
						{
					  		wrfvar();
						}
/*						else if (com_fvar_cpu[com_nmb] == SPB_CPU)
						{
							if (SPB_eep_nmb > 0)
							{
								SPB_can.eep_req = 2;
								SPB_can.eep_rcvd = 0;
								if (SPB_eep_nmb > 1)
								{
									SPB_eep_ix = 0;	
									SPB_eep_nmb = (int16)numspbvars;
								}
								timers[tspb_ui] = 0;
								wr_spb_fvar();
							}
						}
*/
						else if (com_fvar_cpu[com_nmb] == NTS_CPU)
						{
							if (NTS_eep_nmb > 0)
							{
								NTS_spi.eep_req = 2;
								NTS_spi.eep_rcvd = 0;
								if (NTS_eep_nmb > 1)
								{
									NTS_eep_ix = 0;
									NTS_eep_nmb = (int16)numntsvars;
								}
								timers[tnts_ui] = 0;
								wr_nts_fvar();
							}
						}
		
						put_pkt_req (26, cons[carnmb], com_nmb);
					}
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 26;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;
			
		case 27:			// Request Adjustable Variables menu loctions
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			com_menu_req[com_nmb] = (uint16)(rmbuf[com_nmb][8] & 0xff);
			if (com_car_req[com_nmb] == cons[carnmb])
			{
      			if (com_menu_req[com_nmb] < Num_Fvars_Menus)
      			{
      				com_fvar_cpu[com_nmb] = MAIN_CPU;
 		  			put_pkt_req (27, cons[carnmb], com_nmb);
     			}
      			else if (com_menu_req[com_nmb] == Safety_Proc_Adj_Vars_Menu)
      			{
      				com_fvar_cpu[com_nmb] = SPB_CPU;
  		  			put_pkt_req (27, cons[carnmb], com_nmb);
    			}
      			else if (com_menu_req[com_nmb] == NTS_Proc_Adj_Vars_Menu)
      			{
      				com_fvar_cpu[com_nmb] = NTS_CPU;
		  			put_pkt_req (27, cons[carnmb], com_nmb);
      			}
			}
			break;

		case 29:			// Request car Motion Data
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0x00ff);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
			{
				if (rmbuf[com_nmb][8] == 29)
				{
					galcom_req[com_nmb] = 29;
					timers[tcom_pkreq + com_nmb] = 0;
				}
				else if (galcom_req[com_nmb] == 29)
					galcom_req[com_nmb] = 0;
				put_pkt_req (29, cons[carnmb], com_nmb);
			}
			break;

		case 30:			// Request car status
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0x00ff);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
			{
				if (rmbuf[com_nmb][8] == 30)
				{
					galcom_req[com_nmb] = 30;
					timers[tcom_pkreq + com_nmb] = 0;
				}
				else if (galcom_req[com_nmb] == 30)
					galcom_req[com_nmb] = 0;
		  		put_pkt_req (30, cons[carnmb], com_nmb);
			}
			break;

		case 31:	   // Group data packet
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			if (rmbuf[com_nmb][7] == 31)
			{
				galcom_req[com_nmb] = 31;
				timers[tcom_pkreq + com_nmb] = 0;
			}
			else if (galcom_req[com_nmb] == 31)
				galcom_req[com_nmb] = 0;
			
	  		put_pkt_req(31,cons[carnmb],com_nmb);
			break;

		case 32:			  // hall call i/o's
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			if (rmbuf[com_nmb][7] == 32)
			{
				galcom_req[com_nmb] = 32;
				timers[tcom_pkreq + com_nmb] = 0;
			}
			else if (galcom_req[com_nmb] == 32)
				galcom_req[com_nmb] = 0;
	  		put_pkt_req(32,cons[carnmb],com_nmb);
			break;

		case 33:		   // Set car call for any car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			if ((rmbuf[com_nmb][7] >= 1) && (rmbuf[com_nmb][7] <= cons[nmbcars]))
				cc_latched_ack = set_carcall(rmbuf[com_nmb][7],(uint16)(rmbuf[com_nmb][8] & 0xff));	// carnmb, flnmb
			put_pkt_req(33,cons[carnmb],com_nmb);
			break;

		case 34:		  // Set up call
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			set_upcall((uint16)(rmbuf[com_nmb][7] & 0xff));
			break;

		case 35:	// set down call
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			set_dncall((uint16)(rmbuf[com_nmb][7] & 0xff));
			break;

		case 36:			// Set rear car call for any call
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			if ((rmbuf[com_nmb][7] >= 1) && (rmbuf[com_nmb][7] <= cons[nmbcars]))
				set_rcarcall(rmbuf[com_nmb][7],(uint16)(rmbuf[com_nmb][8] & 0xff));		
			break;
		
		case 37:	 // Set rear up hall call
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			set_rupcall((uint16)(rmbuf[com_nmb][7] & 0xff));
			break;

		case 38:	// Set rear down call
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			set_rdncall((uint16)(rmbuf[com_nmb][7] & 0xff));
			break;
			
		case 39:			  // Turn off auto retransmit
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			galcom_req[com_nmb] = 0;
			break;

		case 40:	  // Request detailed faults for select car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	    	if((rmbuf[com_nmb][7] >= 1) && (rmbuf[com_nmb][7] <= cons[nmbcars]))
	    	{
				com_car_req[com_nmb] = rmbuf[com_nmb][7];
				com_rst_flt_ix[com_nmb] = rmbuf[com_nmb][8];
				if (com_car_req[com_nmb] == cons[carnmb])
				{
					put_pkt_req(40,cons[carnmb],com_nmb);
	  				com_req_car_flt[com_nmb] = 0;
					if(com_rst_flt_ix[com_nmb] == 1) 
				    	com_loop_ix[com_nmb] = 0;
				}
				else
				{						
				    timers[tfltdelay] = 0;
				    if (com_rst_flt_ix[com_nmb] == 1) 
				    { 	// Request fault packet from other car
					    put_pkt_req(8,com_car_req[com_nmb],gc_port);
					    com_req_car_flt[com_nmb] = 1;
	                    com_loop_ix[com_nmb] = 0;
					}
					else
		  				put_pkt_req(40,cons[carnmb],com_nmb);
				}
	    	}
			break;
		
		case 41:	// Clear faults for any car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	if ((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access)) 
		  	{
			  		
	 			if((rmbuf[com_nmb][7] >= 1) && (rmbuf[com_nmb][7] <= cons[nmbcars]))
	 				com_car_req[com_nmb] = rmbuf[com_nmb][7];
		 		else
		 			com_car_req[com_nmb] = 0;	 	   
				
		 		if (com_car_req[com_nmb] !=0)
		 		{	
					if(com_car_req[com_nmb] == cons[carnmb])
		 			{
		 		      	for(i=0; i<max_flt_ix; i++)
		 		      	{
		 					f.Flts[i].code = 0;
		 					f.Flts[i].count = 0;
		 		      	}
		 				Flt_ix = 0;
		 			  	Update_All_Flt();
		 			}				
		 			else
		 			{		// Send Request to other car from group car
		 				put_pkt_req(10,com_car_req[com_nmb],gc_port);
		 			}
		 		}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 41;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;

		case 42:			// Request job statictics
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	put_pkt_req (42, cons[carnmb], com_nmb);
			break;

		case 43:			// Clear job statistics
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			clear_job_statistics();
			break;

		case 44:		   // Request eta data for individual car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			put_pkt_req (44, cons[carnmb], com_nmb);
			break;

		case 45:		   // Request trace data for individual car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			uwu.b[lbw] = (uint8)(rmbuf[com_nmb][8] & 0xff); 
			uwu.b[hbw] = (uint8)(rmbuf[com_nmb][9] & 0xff); 
		  	com_trace_ix[com_nmb] = uwu.w;
			put_pkt_req (45, cons[carnmb], com_nmb);
			break;

		case 46:		   // Get trace command from UI
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	com_trace_cmd[com_nmb] = (uint8)(rmbuf[com_nmb][8] & 0xff);
		  	if (com_car_req[com_nmb] == cons[carnmb])
		  	{
			  	switch (com_trace_cmd[com_nmb])
			  	{
			  		case 0:			// command 0: send status
						break;
						
			  		case 1:			// command 1: set trigger (stoppes trace after 35 frames)
			  			Trace_Trigger = 1;
			  			break;
			  			
			  		case 2:			// command 2: clear trace stop
			  			Trace_Stop = 0;
			  			break;
			  			
			  		default:
			  			break;
			  	}
			  	
			  	com_trace_condition_1[com_nmb] = (uint8)(rmbuf[com_nmb][9] & 0xff);
			  	com_trace_condition_2[com_nmb] = (uint8)(rmbuf[com_nmb][10] & 0xff);
				put_pkt_req (45, cons[carnmb], com_nmb);
		  	}
		  	
			break;

		case 47:		   // Request Service and Process flag table
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			put_pkt_req (47, cons[carnmb], com_nmb);
			break;

		case 48:		   // Request one fault packet for individual car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			uwu.b[lbw] = (uint8)(rmbuf[com_nmb][8] & 0xff); 
			uwu.b[hbw] = (uint8)(rmbuf[com_nmb][9] & 0xff); 
		  	com_flt_ix[com_nmb] = uwu.w;
		  	com_flt_type[com_nmb] = (uint16)(rmbuf[com_nmb][10] & 0xff);
			put_pkt_req (48, cons[carnmb], com_nmb);
			break;

		case 49:		   // Set/Request car call test
							// car call test command bit 0: enable_cc_test
							//						 bit 1: continue_cc_test
							//						 bit 2: cancel_cc_test
							//						 bit 3: request cc test status
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access))  && (AD_status == 1))	   
		  	{
		  		if (com_car_req[com_nmb] == cons[carnmb])
		  		{
		  			
				  	com_cc_test_cmd = (uint16)(rmbuf[com_nmb][8] & 0xff);
				  	if ((com_cc_test_cmd & 0x01) != 0) 
				  	{
						uiu.b[lb] = (uint8)(rmbuf[com_nmb][9] & 0xff);
						uiu.b[mlb] = (uint8)(rmbuf[com_nmb][10] & 0xff);
						uiu.b[mhb] = (uint8)(rmbuf[com_nmb][11] & 0xff);
						uiu.b[hb] = (uint8)(rmbuf[com_nmb][12] & 0xff);
				  		com_cc_test_mask[0] = uiu.dw;
						uiu.b[lb] = (uint8)(rmbuf[com_nmb][13] & 0xff);
						uiu.b[mlb] = (uint8)(rmbuf[com_nmb][14] & 0xff);
						uiu.b[mhb] = (uint8)(rmbuf[com_nmb][15] & 0xff);
						uiu.b[hb] = (uint8)(rmbuf[com_nmb][16] & 0xff);
				  		com_cc_test_mask[1] = uiu.dw;
				  		if ((com_cc_test_mask[0] != 0) || (com_cc_test_mask[1] != 0))
				  		{	// floor bits are set
				  			cc_test_mask[0] = com_cc_test_mask[0];
				  			cc_test_mask[1] = com_cc_test_mask[1];
				  			enable_cc_test = 1;
				  		}
				  	}
				  	else if ((com_cc_test_cmd & 0x02) != 0)
				  	{
				  		if ((cc_test_mask[0] != 0) || (cc_test_mask[1] != 0))
				  			continue_cc_test = 1;
				  	}
				  	else if ((com_cc_test_cmd & 0x04) != 0)
				  	{		// cancel the test and clear the car calls
				  		cancel_cc_test = 0;
				  		enable_cc_test = 0;
				  		continue_cc_test = 0;
				  		clrcc(0);
				  	}
				  	
				  	if ((com_cc_test_cmd & 0x08) != 0)
				  	{	// request com_cc_test status
						put_pkt_req (49, cons[carnmb], com_nmb);
				  	}
		  		}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 49;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
		  	break;

		case 50:		// add security code
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access) || 
		  	    (com_access_level[com_nmb] == Building_Access)) && (AD_status == 1))	   
		  	{
				if (LCD_Menu == 11)
					LCD_Init = 1;	// In security display: reset so index is not corrupted

				Com_Add_Sec_Fl_Req[com_nmb] = rmbuf[com_nmb][8];
				Com_Add_Sec_Code[com_nmb][0] = rmbuf[com_nmb][9];
				Com_Add_Sec_Code[com_nmb][1] = rmbuf[com_nmb][10];
				Com_Add_Sec_Code[com_nmb][2] = rmbuf[com_nmb][11];
				Com_Add_Sec_Code[com_nmb][3] = rmbuf[com_nmb][12];
				if ((Com_Add_Sec_Fl_Req[com_nmb] > 0) && (Com_Add_Sec_Fl_Req[com_nmb] <= cons[grtopf]))
				{
					Read_CodeTbl(Com_Add_Sec_Fl_Req[com_nmb],&Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
					Add_Security_Code(Com_Add_Sec_Fl_Req[com_nmb], &Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb], Com_Add_Sec_Code[com_nmb]);
				}
				else if (Com_Add_Sec_Fl_Req[com_nmb] == 0xFF)
				{
					Add_One_Code_All_Floors(1,com_nmb+2,Com_Add_Sec_Code[com_nmb]);
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access) &&
			  	    (com_access_level[com_nmb] != Building_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 50;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;
	
		case 51:	   // Delete security code
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access) || 
		  	    (com_access_level[com_nmb] == Building_Access)) && (AD_status == 1))	   
		  	{
				if (LCD_Menu == 11)
					LCD_Init = 1;	// In security display: reset so index is not corrupted

				Com_Del_Sec_Fl_Req[com_nmb] = rmbuf[com_nmb][8];
				Com_Del_Sec_Code[com_nmb][0] = rmbuf[com_nmb][9];
				Com_Del_Sec_Code[com_nmb][1] = rmbuf[com_nmb][10];
				Com_Del_Sec_Code[com_nmb][2] = rmbuf[com_nmb][11];
				Com_Del_Sec_Code[com_nmb][3] = rmbuf[com_nmb][12];
				if ((Com_Del_Sec_Fl_Req[com_nmb] > 0) && (Com_Del_Sec_Fl_Req[com_nmb] <= cons[grtopf]))
				{
					Read_CodeTbl(Com_Add_Sec_Fl_Req[com_nmb],&Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
					Delete_Security_Code(Com_Del_Sec_Fl_Req[com_nmb], &Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb], Com_Del_Sec_Code[com_nmb]);
				}
				else if (Com_Del_Sec_Fl_Req[com_nmb] == 0xFF)
				{
					Delete_One_Code_All_Floors(1,com_nmb+2,Com_Del_Sec_Code[com_nmb]);
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access) &&
			  	    (com_access_level[com_nmb] != Building_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 51;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;

		case 52:			 // Delete all security codes per floor or all floors
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access) || 
		  	    (com_access_level[com_nmb] == Building_Access)) && (AD_status == 1))	   
		  	{
				Com_Del_All_Codes_Fl_Req[com_nmb] = rmbuf[com_nmb][8];
				if (Com_Del_All_Codes_Fl_Req[com_nmb] != 0)
				{
					if ((Com_Del_All_Codes_Fl_Req[com_nmb] >= cons[bottomf]) && (Com_Del_All_Codes_Fl_Req[com_nmb] <= cons[topf]))
					{
						if (LCD_Menu == 11)
							LCD_Init = 1;	// In security display: reset so index is not corrupted

						Clear_All_Codes_One_Floor(Com_Del_All_Codes_Fl_Req[com_nmb], &Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
					}
					else if (Com_Del_All_Codes_Fl_Req[com_nmb] == 0xFF)
					{
						if (LCD_Menu == 11)
							LCD_Init = 1;	// In security display: reset so index is not corrupted

						Clear_All_Codes_All_Floors(1,com_nmb + 2);
					}
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access) &&
			  	    (com_access_level[com_nmb] != Building_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 52;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;

		case 53:		// Request a security code table
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access) || 
		  	    (com_access_level[com_nmb] == Building_Access)) && (AD_status == 1))	   
		  	{
				Com_Sec_Fl_Tbl_Req[com_nmb] = rmbuf[com_nmb][8];		// receive floor that is requested
				if ((Com_Sec_Fl_Tbl_Req[com_nmb] > 0) && (Com_Sec_Fl_Tbl_Req[com_nmb] <= cons[grtopf]))
			  		put_pkt_req(53,cons[carnmb],com_nmb);
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access) &&
			  	    (com_access_level[com_nmb] != Building_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 53;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;

		case 54:			  // Receive security code table
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access) || 
		  	    (com_access_level[com_nmb] == Building_Access)) && (AD_status == 1))	   
		  	{
				Com_Sec_Fl_Tbl_Req[com_nmb] = rmbuf[com_nmb][8];		// receive floor that is requested
				if ((Com_Sec_Fl_Tbl_Req[com_nmb] > 0) && (Com_Sec_Fl_Tbl_Req[com_nmb] <= cons[grtopf]))
				{
					if (rmbuf[com_nmb][5] < NumMaxCd)
					{
						Com_CodeTable[com_nmb].NumCode = rmbuf[com_nmb][9];

						j = 0;
						i = 0;
						while (i < Com_CodeTable[com_nmb].NumCode)
						{
							Com_CodeTable[com_nmb].Codes[i][0] = rmbuf[com_nmb][8+j];
							Com_CodeTable[com_nmb].Codes[i][1] = rmbuf[com_nmb][9+j];
							Com_CodeTable[com_nmb].Codes[i][2] = rmbuf[com_nmb][10+j];
							Com_CodeTable[com_nmb].Codes[i][3] = rmbuf[com_nmb][11+j];
							i++;
							j+=4;
						}
						if (LCD_Menu == 11)
							LCD_Init = 1;	// In security display: reset so index is not corrupted

						Write_CodeTbl(Com_Sec_Fl_Tbl_Req[com_nmb], &Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
					}
				}
		      	Com_Mode[com_nmb] = 0;
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access) &&
			  	    (com_access_level[com_nmb] != Building_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 54;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
		  	break;

		case 55:			 // Request security checksum table
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req(55,cons[carnmb],com_nmb);
			break;

		case 56:			   // Receive security checksum table
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access) || 
		  	    (com_access_level[com_nmb] == Building_Access)) && (AD_status == 1))	   
		  	{
				Com_Sec_Chksum_Fl[com_nmb] = rmbuf[com_nmb][8];		  // get the number of floors
				if (Com_Sec_Chksum_Fl[com_nmb] == cons[grtopf])
				{
					j = 0;
					for (i = 1; i <= cons[grtopf]; i++)
					{	
						uwu.b[lbw] = rmbuf[com_nmb][9+j];
						uwu.b[hbw] = rmbuf[com_nmb][10+j];
						Com_Code_Tbl_Chk[com_nmb][i].NumCode = uwu.w;
						uwu.b[lbw] = rmbuf[com_nmb][11+j];
						uwu.b[hbw] = rmbuf[com_nmb][12+j];
						Com_Code_Tbl_Chk[com_nmb][i].CheckSum = uwu.w;
						j += 4;
					}
				
					Com_Sec_Fl_Tbl_Req[com_nmb] = (uint8)Verify_Code_Check_Table(&Com_Code_Tbl_Chk[com_nmb][Com_Sec_Chksum_Fl[com_nmb]]);
					
					if (Com_Sec_Fl_Tbl_Req[com_nmb] != 0)
		  				put_pkt_req(53,cons[carnmb],com_nmb);		// request packet to send codes for floor
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access) &&
			  	    (com_access_level[com_nmb] != Building_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 56;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;



		case 57:		// Send User Interface front and rear hall call security floors
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req (57, cons[carnmb], com_nmb);
			break;

		case 58:						// Receive User Interface front and rear hall call Security floor lockouts
			// Up and Down Hall call security bits
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			uiu.b[lb] = rmbuf[com_nmb][7];
			uiu.b[mlb] = rmbuf[com_nmb][8];
			uiu.b[mhb] = rmbuf[com_nmb][9];
			uiu.b[hb] = rmbuf[com_nmb][10];
			ui_uc_sec[0] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][11];
			uiu.b[mlb] = rmbuf[com_nmb][12];
			uiu.b[mhb] = rmbuf[com_nmb][13];
			uiu.b[hb] = rmbuf[com_nmb][14];
			ui_uc_sec[1] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][15];
			uiu.b[mlb] = rmbuf[com_nmb][16];
			uiu.b[mhb] = rmbuf[com_nmb][17];
			uiu.b[hb] = rmbuf[com_nmb][18];
			ui_dc_sec[0] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][19];
			uiu.b[mlb] = rmbuf[com_nmb][20];
			uiu.b[mhb] = rmbuf[com_nmb][21];
			uiu.b[hb] = rmbuf[com_nmb][22];
			ui_dc_sec[1] = uiu.dw;
			// Rear Up and Down Security bits
			uiu.b[lb] = rmbuf[com_nmb][23];
			uiu.b[mlb] = rmbuf[com_nmb][24];
			uiu.b[mhb] = rmbuf[com_nmb][25];
			uiu.b[hb] = rmbuf[com_nmb][26];
			ui_ruc_sec[0] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][27];
			uiu.b[mlb] = rmbuf[com_nmb][28];
			uiu.b[mhb] = rmbuf[com_nmb][29];
			uiu.b[hb] = rmbuf[com_nmb][30];
			ui_ruc_sec[1] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][31];
			uiu.b[mlb] = rmbuf[com_nmb][32];
			uiu.b[mhb] = rmbuf[com_nmb][33];
			uiu.b[hb] = rmbuf[com_nmb][34];
			ui_rdc_sec[0] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][35];
			uiu.b[mlb] = rmbuf[com_nmb][36];
			uiu.b[mhb] = rmbuf[com_nmb][37];
			uiu.b[hb] = rmbuf[com_nmb][38];
			ui_rdc_sec[1] = uiu.dw;
			break;

		case 59:			// Send User Interface front car call security floors
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req (59, cons[carnmb], com_nmb);
			break;

		case 60:							// Receive User Interface Front car call Security floor lockouts
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			j = 7;
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				uiu.b[lb] = rmbuf[com_nmb][j+0];
				uiu.b[mlb] = rmbuf[com_nmb][j+1];
				uiu.b[mhb] = rmbuf[com_nmb][j+2];
				uiu.b[hb] = rmbuf[com_nmb][j+3];
				ui_cc_sec[0][i]	= uiu.dw;
				j+=4;
				uiu.b[lb] = rmbuf[com_nmb][j+0];
				uiu.b[mlb] = rmbuf[com_nmb][j+1];
				uiu.b[mhb] = rmbuf[com_nmb][j+2];
				uiu.b[hb] = rmbuf[com_nmb][j+3];
				ui_cc_sec[1][i]	= uiu.dw;
				j+=4;
				
				if (i != cons[carnmb])
				{
					if((ui_cc_sec[0][i] != ui_cc_sec_prev[0][i]) ||(ui_cc_sec[1][i] != ui_cc_sec_prev[1][i]))
					{
						if (ui_cc_sec[0][i] != ui_cc_sec_prev[0][i])
							ui_cc_sec_prev[0][i] = ui_cc_sec[0][i];
						else if (ui_cc_sec[1][i] != ui_cc_sec_prev[1][i])
							ui_cc_sec_prev[1][i] = ui_cc_sec[1][i];
						put_pkt_req(37,i,gc_port);
					}									
				}
				else
				{
					ui_cc_sec_prev[0][i] = ui_cc_sec[0][i];
					ui_cc_sec_prev[1][i] = ui_cc_sec[1][i];
				}
				
			}
			break;

		case 61:			// Send User Interface rear car call security floors
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req (61, cons[carnmb], com_nmb);
			break;
			
		case 62: 							// Receive User Interface Rear car call Security floor lockouts
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			j = 7;
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				uiu.b[lb] = rmbuf[com_nmb][j+0];
				uiu.b[mlb] = rmbuf[com_nmb][j+1];
				uiu.b[mhb] = rmbuf[com_nmb][j+2];
				uiu.b[hb] = rmbuf[com_nmb][j+3];
				ui_rcc_sec[0][i]	= uiu.dw;
				j+=4;
				uiu.b[lb] = rmbuf[com_nmb][j+0];
				uiu.b[mlb] = rmbuf[com_nmb][j+1];
				uiu.b[mhb] = rmbuf[com_nmb][j+2];
				uiu.b[hb] = rmbuf[com_nmb][j+3];
				ui_rcc_sec[1][i]	= uiu.dw;
				j+=4;
			}
			break;
			
		case 63: 		// Request for UI Front hall call security for car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
		  		put_pkt_req (64, cons[carnmb], com_nmb);
			break;
		
		case 64:						// Receive User Interface front hall call Security floor lockouts
			// Up and Down Car Hall call security bits
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			el = rmbuf[com_nmb][7];
			
			uiu.b[lb] = rmbuf[com_nmb][8];
			uiu.b[mlb] = rmbuf[com_nmb][9];
			uiu.b[mhb] = rmbuf[com_nmb][10];
			uiu.b[hb] = rmbuf[com_nmb][11];
			ui_uhc_carsec[0][el] = uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][12];
			uiu.b[mlb] = rmbuf[com_nmb][13];
			uiu.b[mhb] = rmbuf[com_nmb][14];
			uiu.b[hb] = rmbuf[com_nmb][15];
			ui_uhc_carsec[1][el] = uiu.dw;

			uiu.b[lb] = rmbuf[com_nmb][16];
			uiu.b[mlb] = rmbuf[com_nmb][17];
			uiu.b[mhb] = rmbuf[com_nmb][18];
			uiu.b[hb] = rmbuf[com_nmb][19];
			ui_dhc_carsec[0][el] = uiu.dw;

			uiu.b[lb] = rmbuf[com_nmb][20];
			uiu.b[mlb] = rmbuf[com_nmb][21];
			uiu.b[mhb] = rmbuf[com_nmb][22];
			uiu.b[hb] = rmbuf[com_nmb][23];
			ui_dhc_carsec[1][el] = uiu.dw;
			break;

		case 65:
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][6]);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
		  		put_pkt_req (65, cons[carnmb], com_nmb);
			break;
			
		case 66:						// Receive User Interface rear car hall call Security floor lockouts
		
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			el = rmbuf[com_nmb][7];
		
			uiu.b[lb] = rmbuf[com_nmb][8];
			uiu.b[mlb] = rmbuf[com_nmb][9];
			uiu.b[mhb] = rmbuf[com_nmb][10];
			uiu.b[hb] = rmbuf[com_nmb][11];
			ui_ruhc_carsec[0][el] = uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][12];
			uiu.b[mlb] = rmbuf[com_nmb][13];
			uiu.b[mhb] = rmbuf[com_nmb][14];
			uiu.b[hb] = rmbuf[com_nmb][15];;
			ui_ruhc_carsec[1][el]	= uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][16];
			uiu.b[mlb] = rmbuf[com_nmb][17];
			uiu.b[mhb] = rmbuf[com_nmb][18];
			uiu.b[hb] = rmbuf[com_nmb][19];
			ui_rdhc_carsec[0][el]	= uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][20];
			uiu.b[mlb] = rmbuf[com_nmb][21];
			uiu.b[mhb] = rmbuf[com_nmb][22];
			uiu.b[hb] = rmbuf[com_nmb][23];
			ui_rdhc_carsec[1][el] = uiu.dw;
			break;
			
		case 67:					 // Request to send the Trace variables 
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			
			if (com_car_req[com_nmb] == cons[carnmb]) 
		  		put_pkt_req (68, cons[carnmb], com_nmb);
			
			break;

		case 68:				  // Receive Trace variables
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	if (((com_access_level[com_nmb] == Adjuster_Access) ||
		  	    (com_access_level[com_nmb] == Mechanic_Access) ||
		  	    (com_access_level[com_nmb] == Service_Access)) && (AD_status == 1))	   
		  	{
				if (com_car_req[com_nmb] == cons[carnmb]) 
				{
					uiu.b[lb] = (uint8)(rmbuf[com_nmb][8] & 0xff);
					uiu.b[mlb] = (uint8)(rmbuf[com_nmb][9] & 0xff);
					uiu.b[mhb] = (uint8)(rmbuf[com_nmb][10] & 0xff);
					uiu.b[hb] = (uint8)(rmbuf[com_nmb][11] & 0xff);
					trigger_logic = uiu.dw;

					uiu.b[lb] = (uint8)(rmbuf[com_nmb][12] & 0xff);
					uiu.b[mlb] = (uint8)(rmbuf[com_nmb][13] & 0xff);
					uiu.b[mhb] = (uint8)(rmbuf[com_nmb][14] & 0xff);
					uiu.b[hb] = (uint8)(rmbuf[com_nmb][15] & 0xff);
					and_trigger_logic = uiu.dw;

					uiu.b[lb] = (uint8)(rmbuf[com_nmb][16] & 0xff);
					uiu.b[mlb] = (uint8)(rmbuf[com_nmb][17] & 0xff);
					uiu.b[mhb] = (uint8)(rmbuf[com_nmb][18] & 0xff);
					uiu.b[hb] = (uint8)(rmbuf[com_nmb][19] & 0xff);
					trigger_enable = uiu.dw;

					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][20] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][21] & 0xff); 
				  	trigger_arm = uwu.w;

					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][22] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][23] & 0xff); 
				  	trigger_window = uwu.w;

					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][24] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][25] & 0xff); 
				  	trace_interval = uwu.w;

					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][26] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][27] & 0xff); 
				  	trigger_fault = uwu.w;

					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][28] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][29] & 0xff); 
				  	trigger_servf = uwu.w;

					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][30] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][31] & 0xff); 
				  	trigger_procf = uwu.w;
	
					uiu.b[lb] = (uint8)(rmbuf[com_nmb][32] & 0xff);
					uiu.b[mlb] = (uint8)(rmbuf[com_nmb][33] & 0xff);
					uiu.b[mhb] = (uint8)(rmbuf[com_nmb][34] & 0xff);
					uiu.b[hb] = (uint8)(rmbuf[com_nmb][35] & 0xff);
					trigger_run_statusf = uiu.dw;

					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][36] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][37] & 0xff); 
				  	trigger_front_sd = uwu.w;

					uwu.b[lbw] = (uint8)(rmbuf[com_nmb][38] & 0xff); 
					uwu.b[hbw] = (uint8)(rmbuf[com_nmb][39] & 0xff); 
				  	trigger_rear_sd = uwu.w;

					uiu.b[lb] = (uint8)(rmbuf[com_nmb][40] & 0xff);
					uiu.b[mlb] = (uint8)(rmbuf[com_nmb][41] & 0xff);
					uiu.b[mhb] = (uint8)(rmbuf[com_nmb][42] & 0xff);
					uiu.b[hb] = (uint8)(rmbuf[com_nmb][43] & 0xff);
					trigger_statusf = uiu.dw;

					uiu.b[lb] = (uint8)(rmbuf[com_nmb][44] & 0xff);
					uiu.b[mlb] = (uint8)(rmbuf[com_nmb][45] & 0xff);
					uiu.b[mhb] = (uint8)(rmbuf[com_nmb][46] & 0xff);
					uiu.b[hb] = (uint8)(rmbuf[com_nmb][47] & 0xff);
					trigger_statusf2 = uiu.dw;
	
					trigger_fault_bits[0] = (uint8)(rmbuf[com_nmb][48] & 0xff);
					trigger_fault_bits[1] = (uint8)(rmbuf[com_nmb][49] & 0xff);
					trigger_fault_bits[2] = (uint8)(rmbuf[com_nmb][50] & 0xff);
					trigger_fault_bits[3] = (uint8)(rmbuf[com_nmb][51] & 0xff);


					if (galcom_ver > 2)
					{
						uiu.b[lb] = (uint8)(rmbuf[com_nmb][40] & 0xff);
						uiu.b[mlb] = (uint8)(rmbuf[com_nmb][41] & 0xff);
						uiu.b[mhb] = (uint8)(rmbuf[com_nmb][42] & 0xff);
						uiu.b[hb] = (uint8)(rmbuf[com_nmb][43] & 0xff);
						trigger_statusf3 = uiu.dw;

						uiu.b[lb] = (uint8)(rmbuf[com_nmb][44] & 0xff);
						uiu.b[mlb] = (uint8)(rmbuf[com_nmb][45] & 0xff);
						uiu.b[mhb] = (uint8)(rmbuf[com_nmb][46] & 0xff);
						uiu.b[hb] = (uint8)(rmbuf[com_nmb][47] & 0xff);
						trigger_statusf4 = uiu.dw;
					}
					
			   		write_trace_vars();
					put_pkt_req (68, cons[carnmb], com_nmb);
				}
		  	}
		  	else
		  	{
		  		com_access_status[com_nmb] = 0;	   
		  	
			  	if (AD_status == 0)
			  		com_access_status[com_nmb] |= 0x01;			// failed from auto door
			  	
			  	if ((com_access_level[com_nmb] != Adjuster_Access) &&
			  	    (com_access_level[com_nmb] != Mechanic_Access) &&
			  	    (com_access_level[com_nmb] != Service_Access))
			  		com_access_status[com_nmb] |= 0x02;	   		// failed from access level
		  	    
		 	 	com_access_level_pk3[com_nmb] = com_access_level[com_nmb];
			  	com_access_packet[com_nmb] = 68;
			  	put_pkt_req (3, cons[carnmb], com_nmb);
		  	}
			break;

		case 69:		   // Request motion trace data for individual car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
			{
				if (rmbuf[com_nmb][8] == 69)
				{
					galcom_req[com_nmb] = 69;
					timers[tcom_pkreq + com_nmb] = 0;
				}
				else if (galcom_req[com_nmb] == 69)
					galcom_req[com_nmb] = 0;
				
				uwu.b[lbw] = (uint8)(rmbuf[com_nmb][9] & 0xff); 
				uwu.b[hbw] = (uint8)(rmbuf[com_nmb][10] & 0xff); 
			  	com_trace_ix[com_nmb] = uwu.w;
				put_pkt_req (69, cons[carnmb], com_nmb);
			}
			break;

		case 70: 			// Send Group Service Timers
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_svc_tmr[com_nmb] = rmbuf[com_nmb][7];
	  		put_pkt_req (70, cons[carnmb], com_nmb);
			break;

		case 71:					// Receive Group Service Timers
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			i = rmbuf[com_nmb][7];
			if ((i >= 0) && (i <= 9))
			{
				svc_t[i].service = rmbuf[com_nmb][8];
				svc_t[i].status = rmbuf[com_nmb][9];
				svc_t[i].logic = rmbuf[com_nmb][10];
				svc_t[i].option = rmbuf[com_nmb][11];
				j = 12;
				for (k=0;k<=2;k++)
				{
					svc_t[i].month[k] = rmbuf[com_nmb][j];
					svc_t[i].day[k] = rmbuf[com_nmb][j+1];
				  	svc_t[i].md_on_hour[k] = rmbuf[com_nmb][j+2];
				  	svc_t[i].md_off_hour[k] = rmbuf[com_nmb][j+3];
				  	svc_t[i].md_on_minute[k] = rmbuf[com_nmb][j+4];
				  	svc_t[i].md_off_minute[k] = rmbuf[com_nmb][j+5];
					j+=6;
				}
				j++;	// next location
				for (k=0;k<=6;k++)
				{			   // j starts at 18 so j+6 = 24
				  	svc_t[i].on_hour[k] = rmbuf[com_nmb][j];
				  	svc_t[i].off_hour[k] = rmbuf[com_nmb][j+1];
				  	svc_t[i].on_minute[k] = rmbuf[com_nmb][j+2];
				  	svc_t[i].off_minute[k] = rmbuf[com_nmb][j+3];
					j+=4;
				}
				Wrt_Grp_Svc_Timer(i);
			}
			Com_Mode[com_nmb] = 0;
			break;

		case 72:			// Send Line voltage for user interface
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req (72, cons[carnmb], com_nmb);
			break;

		case 73:			// Car data for fault and service change detection
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req (73, cons[carnmb], com_nmb);
			break;

		case 74:			// Send User Interface Control Variables
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	  		put_pkt_req (74, cons[carnmb], com_nmb);
			break;
			
		case 75: 							// Receive User Interface Control Variables
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			j = 7;
			ui_uppeak = rmbuf[com_nmb][j+0];
			ui_dnpeak = rmbuf[com_nmb][j+1];
			ui_alt_lby = rmbuf[com_nmb][j+2];
			ui_alt_park = rmbuf[com_nmb][j+3];
			ui_ncu = rmbuf[com_nmb][j+4];
			ui_alt_fl_sec = rmbuf[com_nmb][j+5];
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				ui_ind[i] = rmbuf[com_nmb][j+6];
				ui_rtl[i] = rmbuf[com_nmb][j+7];
				ui_heof[i] = rmbuf[com_nmb][j+8];
				ui_ad_off[i] = rmbuf[com_nmb][j+9];
				j+=4;
			}
			break;
			
			
		case 76:		// group alarm  variables
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			put_pkt_req (76, cons[carnmb], com_nmb);
			break;
			
		case 77:		// current fault for each car
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			put_pkt_req (77, cons[carnmb], com_nmb);
			break;
			
		case 78:		// Set Vip call
			// byte 7 floor
			// byte 8 car number
			// byte 9 rear operation
					   
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
	    	fl_ix = ((int16)rmbuf[com_nmb][7]-1)/32;	
						
			if( rmbuf[com_nmb][9] == 0xff)
			{
				if(((rmbuf[com_nmb][7]>= 1)&& (rmbuf[com_nmb][7] <= cons[grtopf])) &&(rmbuf[com_nmb][8] <= cons[nmbcars]))
				{
					front_vip = 0;
					rear_vip = 1;
					VipCarNmb = rmbuf[com_nmb][8];
					gc_rvip_set[fl_ix]|=fl_mask[rmbuf[com_nmb][7]];
				}
			}
			else
			{
				if(((rmbuf[com_nmb][7]>= 1)&& (rmbuf[com_nmb][7] <= cons[grtopf])) &&(rmbuf[com_nmb][8] <= cons[nmbcars]))
				{
					front_vip = 1;
					rear_vip = 0;
					VipCarNmb = rmbuf[com_nmb][8];
					gc_vip_set[fl_ix]|=fl_mask[rmbuf[com_nmb][7]];
				}
			} 			

			break;	
			
		case 79:		   // Request run number of floors (access, car#, dir, #flrs)
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
		  	com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7] & 0xff);
		  	
			if (com_car_req[com_nmb] == cons[carnmb])
			{		// For this car
				if ((rmbuf[com_nmb][8] & 0xff) == 1)
				{		// up direction
					fl_ix = (uint16)(position + (rmbuf[com_nmb][9] & 0x0ff));
					if (fl_ix > cons[topf])
						fl_ix = cons[topf];
					set_carcall(com_car_req[com_nmb],fl_ix);	// carnmb, flnmb
				}
				else if ((rmbuf[com_nmb][8] & 0xff) == 2)
				{		// down direction
					fl_ix = (int16)(position - (rmbuf[com_nmb][9] & 0x0ff));
					if (fl_ix < cons[bottomf])
						fl_ix = cons[bottomf];
					set_carcall(com_car_req[com_nmb],fl_ix);	// carnmb, flnmb
				}
			}
			break;
			
		case 80: 							// Receive keyboard commands
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			i = 0;
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			if ((com_car_req[com_nmb] >= 1) && (com_car_req[com_nmb] <= cons[nmbcars]))
			{
				j = len - 8;
				if (j > 50)
					j = 50;
				else if (j < 0)
					j = 0;
				while (i < j) 
				{
					recbuf[TERMCOM][i] = rmbuf[com_nmb][i+8];
					if (recbuf[TERMCOM][i] == CR)
						break;
					i++;
				}
				recbuf[TERMCOM][i] = CR;		// add an extra CR incase data corrupted
  				rbs[TERMCOM] = 0;
				tbs[TERMCOM] = 0;
				tbf[TERMCOM] = 0;
				k = mute_echo_on_port;		// store mute value
				mute_echo_on_port = 1;
				comm_debug(TERMCOM);
				mute_echo_on_port = k;		// restore mute value
				
				if (tbs[TERMCOM] != 0)
					put_pkt_req (80, cons[carnmb], com_nmb);
						
			}
			break;
			
		case 90:					//CTC CAR TOP CAN Communication status
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			put_pkt_req(90, cons[carnmb], com_nmb);
			break;
			
		case 91:					//MRC MR CAN Communication status
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			put_pkt_req(91, cons[carnmb], com_nmb);
			break;
			
		case 92:					//GRP CAN Communication status
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			put_pkt_req(92, cons[carnmb], com_nmb);
			break;
		
		case 93:					//Auto hoistway learn
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			put_pkt_req(93, cons[carnmb], com_nmb);
			break;
			
		case 94:					//Set I/O
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			#if (Simulator == 1)
				#if (GALaxy_4 == 1)
					if (rmbuf[com_nmb][8] < 18)
					{
						ioset[rmbuf[com_nmb][8]] |= bittbl[rmbuf[com_nmb][9] & 0x07];
						ioclr[rmbuf[com_nmb][8]] |= bittbl[rmbuf[com_nmb][9] & 0x07];
					}
					else if (rmbuf[com_nmb][8] < 138)
					{
						mrc_ioset[rmbuf[com_nmb][8]-18] |= bittbl[rmbuf[com_nmb][9] & 0x07];
						mrc_ioclr[rmbuf[com_nmb][8]-18] |= bittbl[rmbuf[com_nmb][9] & 0x07];
					}
					else if (rmbuf[com_nmb][8] < 258)
					{
						ctc_ioset[rmbuf[com_nmb][8]-138] |= bittbl[rmbuf[com_nmb][9] & 0x07];
						ctc_ioclr[rmbuf[com_nmb][8]-138] |= bittbl[rmbuf[com_nmb][9] & 0x07];
					}	
				#else
			if (rmbuf[comnmb][8] <= 80)
					{
						ioset[rmbuf[com_nmb][8]] |= bittbl[rmbuf[com_nmb][9] & 0x07];
						ioclr[rmbuf[com_nmb][8]] |= bittbl[rmbuf[com_nmb][9] & 0x07];
					}
				#endif
			#endif
			break;
			
		case 95:					//Clear I/O				
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			#if (Simulator == 1)
				#if (GALaxy_4 == 1)
					if (rmbuf[com_nmb][8] < 18)
					{
						ioset[rmbuf[com_nmb][8]] &= ~bittbl[rmbuf[com_nmb][9] & 0x07];
						ioclr[rmbuf[com_nmb][8]] &= ~bittbl[rmbuf[com_nmb][9] & 0x07];
					}
					else if (rmbuf[com_nmb][8] < 138)
					{
						mrc_ioset[rmbuf[com_nmb][8]-18] &= ~bittbl[rmbuf[com_nmb][9] & 0x07];
						mrc_ioclr[rmbuf[com_nmb][8]-18] &= ~bittbl[rmbuf[com_nmb][9] & 0x07];
					}
					else if (rmbuf[com_nmb][8] < 258)
					{
						ctc_ioset[rmbuf[com_nmb][8]-138] &= ~bittbl[rmbuf[com_nmb][9] & 0x07];
						ctc_ioclr[rmbuf[com_nmb][8]-138] &= ~bittbl[rmbuf[com_nmb][9] & 0x07];
					}
						
				#else
					if (com_dbg_num[comnmb][0] <= 80)
					{ 
						ioclr[rmbuf[com_nmb][8]] &= ~bittbl[rmbuf[com_nmb][9] & 0x07];
						ioset[rmbuf[com_nmb][8]] &= ~bittbl[rmbuf[com_nmb][9] & 0x07];
					}
				#endif
			#endif
			break;
		case 96:
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			
			uiu.b[lb] = rmbuf[com_nmb][8];
			uiu.b[mlb] = rmbuf[com_nmb][9];
			uiu.b[mhb] = rmbuf[com_nmb][10];
			uiu.b[hb] = rmbuf[com_nmb][11];
			i = (short int)uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][12];
			uiu.b[mlb] = rmbuf[com_nmb][13];
			uiu.b[mhb] = rmbuf[com_nmb][14];
			uiu.b[hb] = rmbuf[com_nmb][15];
			j = (short int)uiu.dw;
			
			fvars[i] = j;
			wrfvar();
			break;
			
		case 97:
			com_access_level[com_nmb] = (uint16)(rmbuf[com_nmb][6] & 0xff);
			com_car_req[com_nmb] = (uint16)(rmbuf[com_nmb][7]);
			
			//read requested fvar number
			uiu.b[lb] = rmbuf[com_nmb][8];
			uiu.b[mlb] = rmbuf[com_nmb][9];
			uiu.b[mhb] = rmbuf[com_nmb][10];
			uiu.b[hb] = rmbuf[com_nmb][11];
			i = (short int)uiu.dw;
			com_fvar_req[com_nmb] = i;
			
			put_pkt_req(97, cons[carnmb], com_nmb);
			
			break;
			
		default:
	    	break;
			
	}
}


#define DELTA 0x9e3779b9  //The magic constant, 2654435769 or 9E3779B916 is chosen to be 2^^32/x, where x is the golden ratio (1.618).
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))
 
void xxtea(uint32 *v, int16 n, uint32 key[4]) 
{
	uint32_t y, z, sum;
    unsigned p, rounds, e;
    
    if (n > 1) 
    {          /* Coding Part */
    	rounds = 6 + 52/n;
      	sum = 0;
      	z = v[n-1];
      	do 
      	{
        	sum += DELTA;
        	e = (sum >> 2) & 3;
        	for (p=0; p<n-1; p++) 
        	{
          		y = v[p+1]; 
          		z = v[p] += MX;
        	}
        	y = v[0];
        	z = v[n-1] += MX;
      	} while (--rounds);
    } 
    else if (n < -1) 
    {  /* Decoding Part */
      	n = -n;
      	rounds = 6 + 52/n;
      	sum = rounds*DELTA;
      	y = v[0];
      	do 
      	{
        	e = (sum >> 2) & 3;
        	for (p=n-1; p>0; p--) 
        	{
          		z = v[p-1];
          		y = v[p] -= MX;
        	}
        	z = v[n-1];
        	y = v[0] -= MX;
        	sum -= DELTA;
      	} while (--rounds);
    }
}

/*

Revision History:

12/21/12 v7.0.0 mhd		1. Added lenght check byte to verify length.  Length check byte must be equal to length - 1.
2/7/13	 v7.0.1	mhd		2. Made transmit length 2 bytes.
						3. Added floor mask to packet 10.
						4. Also send or receive car number in packets 14,15,17,20,22,23,30,40,41,44,63 and 65.
2/16/13					5. Added packet 19 to send which group io's are inputs or outputs.
7/30/13 v7.0.15 mhd		1. Corrected (comnmb == 1) to (comnmb == USR1COM) and (comnmb == 2) to (comnmb == USR2COM) since USR1COM = 0 and USR2COM = 1.
						2. Since this routine uses DMA, do not call sendc(). Instead call com_1_tx_start().
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 where needed to allow any car to be the group.
2/26/14 v7.0.37 mhd		1. Send number of error codes being sent and error code index with packet 40. 
4/7/14 v7.0.41 mhd		1. Added car data to packet 30.
						2. Added fault data to pack 40 and cleared flag for com_rst_flt_ix[com_nmb] when flag is set.
						3. Added auto re-transmit for packets 30, 31 and 32.
						4. Added paket 39 to turn off auto re-transmit.  Also re-transmit can be turned off from requesting the packet without the auto re-transmit flag set. 
4/12/14 v7.0.42 mhd		1. Use cc_mask[][] in packet 10.						

5/28/14 v7.1.1 mhd		1. Added template for flow control (clear_to_send()).
						2. Deleted heartbeat from controller.
6/13/14 v7.1.3 mhd		1. Added i+= in while loop for packet 26 in galcom_pack(); 
						2. Added packet # 27 to send menu information
						3. Changed packet 11 to an extended car initialization.
						4. Deleted sending cons[speed] in packet 7.
6/24/14 v7.1.4 mhd		1. Added Dispatcher variable to packets 7, 12 and 31.
						2. Moved car init data from packet 12 to packet 11.
7/25/14 v7.1.6 mhd		1. Added rear_op[] per car back into packet 12.
8/11/14 v7.1.7 mhd		1. Added fault data to detailed fault list.
10/3/14 v7.1.11 mhd		1. Added packets 45 and 46 for the trace data.
10/6/14 v7.1.12 mhd		1. Changed fault_type variable stops[0] to next_stop because that is what it is.
						2. Added data in car data packet to include trace data variables.
						3. Changed packet data to be mostly local car data.  
11/5/14 v7.1.19 mhd		1. Added packet 9 to send encription key
						2. Modified packet 3 to send log on status
						3. Added check for logon and AD input status to allow chaning parameters from the UI.
						4. Enabled flow control.
						5. Added xxtea (eXtended Tiny Encription Algorithm). Encription data will have to be 20 bytes (n=5).
11/10/14 v7.1.20 mhd	1. Combined user id and password packets together and respond with packet 3.
						2. Added packet 28 to send one field variable to galcom device.  Also added routine to put the packet 28 request on the queue.
						3. Send car number in packets 3,4, and 5.
11/19/14 v7.1.21 mhd	1. Changed logon mechanism to use access levels from server.
						2. Added packet 80 to send data to server in terminal mode using TERMCOM port and comdebug().
12/1/14 v7.1.22 mhd		1. Receive access level byte in all packet from server.
						2. Added 2 byte length and check length in receive packet see rxgalcom().
2/9/15 v7.1.25 mhd		1. send com_access_packet[] in packet 3.
2/12/15  v7.1.25 mhd	2. Added packets 90-93 for test simulator interface
2/16/15 v7.1.27 mhd		1. Use second timer for password and check for value being 10 times the password timer.
4/6/15 v7.1.29	mhd		1. Change password timer to be 1 x the password time instead of 10 x.
6/1/15 v7.1.32 mhd		1. Added mechanism to send reboot sequence to wifi device.
8/3/15 v7.1.41 mhd		1. Added com_access_level_pk3[] to keep the return access level correct when returned in packet 3.
						2. Added packet 29 for Car Motion Display
						3. Temporarily deleted reboot command for wifi device.
						4. Added cons[Dest_Disp] flag to extended group initialization packet.
						5. Send current fault in packet 7.
10/2/15 v7.1.42 mhd		1. Added automatic packet request time-out.  See timers[tcom_pkreq + com_nmb].
						2. Added packets 67 and 68 to send and receive the trace setup variables
						3. Added packet 69 to send motion trace data.
10/21/15 v7.1.44 tb		1. Modified packets 90-92 to send correct device addresses.
						2. Added packets 95-96 for setting/clearing I/Os on simulator and packet 97 for requesting single field variable.
11/09/15 v7.1.46 tb		1. Added packet 33 to send in response to a car call request packet, as an acknowledgement if the call was latched or not.
11/11/15 v7.1.48 as		1. Send date and time in packet 7 to Galileo.
						2. Changed auto retransmit clearing mechanism, only clear galcom_req[com_nmb] variable if the previously requested packet is not requesting retransmit anymore.
						3. Added sending packet 109 for sync date and time on other cars when recieved date update from Galileo in case 6 of galcom_unpack() routine.
11/23/15 v7.1.49 mhd/as	1. Modified case 26 to issues with Galileo updating nts and spb parameters.
							a) When nts/spb parameter update is made by Galileo to a single parameter set NTS_eep_ix = com_fvar_ix[com_nmb][i],
								otherwise, if multiple parameters changed update all paramters values.
							b) Clear timers[tnts_ui] timer when NTS parameter is updated to give more time for getting a response from NTS device, similar for SPB.
12/22/15 v7.1.55 mhd	1. Added write_trace_vars() call in packet 68 also corrected com_access_packet[com_nmb] = 68 instead of 26.
1/24/17 v8.0.1 mhd		1. Added statusf3 and statusf4 if galcom_ver > 2.
*/