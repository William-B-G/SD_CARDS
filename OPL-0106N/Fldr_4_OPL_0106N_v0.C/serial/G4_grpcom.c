#define Baud 0		// Set baud rate for comm 2: 0 = 57600, 1=38400, 2=19200
#define d_GRPCOM 1
#include "global.h"

/*		Group Comm Description
		Rs-485 half duplex
		8 data bits
		1 start bit
		1 stop bit
		Even parity sent on address
		Odd parity sent for all data

		Receiver set for odd parity only and set to interrupt on error
		When even parity address sent, receiver gets interrupted and verifies address

		This protocol us a modified Modbus RTU protocol. Instead of using a wait time delay to 
		verify the start of a packet, a ':' is transmitted similar to the ascii protocol.  Also, for
		the end of packet, an '@' symbol is sent instead of wait times.	 The BYTE COUNT is always
		sent in the first data byte of the function.  An LRC (Longitudinal Redundancy Check)
		is used instead of a CRC (Cyclical Redundancy Check) and includes the address, function 
		and data.
		
		Comm Protocol

		Master  						Slave             Parity
		START ':'                		':'					SET
		TO ADDRESS						FROM ADDRESS		CLR
		FUNCTION #						FUNCTION #			CLR
		Compress zero flag				Compress zero Flag	CLR
		DATA 1                  		DATA 1   	        CLR
		DATA 2                  		DATA 2            	CLR
			...                    			...
		DATA LAST               		DATA LAST         	CLR
		LRC CHECKSUM               		LRC CHECKSUM        CLR
		END	'@'							'@'					CLR

		
		Transmit buffer is packed exactly as data is transmitted

		Receive buffer is packed from receive data plus the actual length
			and the actual checksum as follows:

			ADDRESS
			FUNCTION/PACKET
			BYTE COUNT
			DATA
			...
			CHECKSUM

		Packets are not tested for a fixed length but for the transmitted
		length being equal to the actual length.


		GROUP BUS
	CAR 1    		  1
	CAR 2             2
	CAR 3             3
	CAR 4             4
	CAR 5             5
	CAR 6             6
	CAR 7             7
	CAR 8             8
	MR_DPY            9
	LBY_DPY           10
	Packet/Function DESCRIPTION

	PACKET# 	TO 			FROM		BC			DESCRIPTION
	0           xxx         xxx          x          Packet #0: Invalid
	1			CAR			GRP					   	Packet #1: Car Data to Group
	2			GRP			CAR					   	Packet #2: Group Data to Cars
	3 			GRP			CAR						Packet #3: Set a front car call on a slave car
	4 			GRP			CAR						Packet #4: Set a rear car call on a slave car
	5 			GRP			CAR			 5			Packet #5: add a security code
													  Byte Count
													  Floor #	0=invalid, 255=All floors
													  Code 1st byte
													  Code 2nd byte
													  Code 3rd byte
													  Code 4th byte
	6 			GRP			CAR			 5			Packet #6: delete security code
													  Byte Count
													  Floor #	0=invalid, 255=All floors
													  Code 1st byte
													  Code 2nd byte
													  Code 3rd byte
													  Code 4th byte
	7 												Packet #7: Request Security Code Table for Floor
													  Byte Count
													  Floor Table Request
	8 												Packet #8: Request Fault informatin from Car
	9 												Packet #9: Transmit error codes to group
	10												Packet #10: Clear Fault information on Car
	11			ALL			ALL			2			Packet #11: Request Packet
													  Byte Count
	  												  Packet Number (to respond with version, init, etc.)
	12			ALL 		ALL			3 			Packet #12: Send Version and Revision Number
													  Byte Count
													  Device Version
													  Device Revision
	13			GRP			CAR			9-33		Packet #13: Send Security Car Call Lockout data to car
													  Byte Count
													  Security Flag
													  Floors 1 - 8
													  Floors 9 - 16
													  Floors 17 - 24
													  Floors 25 - 32
													  Rear Floors 1 - 8
													  Rear Floors 9 - 16
													  Rear Floors 17 - 24
													  Rear Floors 25 - 29
													  ids Floors 1 - 8
													  ids Floors 9 - 16
													  ids Floors 17 - 24
													  ids Floors 25 - 32
													  ids Rear Floors 1 - 8
													  ids Rear Floors 9 - 16
													  ids Rear Floors 17 - 24
													  ids Rear Floors 25 - 32
														if (high rise car)
														  Floors 33 - 40
														  Floors 41 - 48
														  Floors 49 - 56
														  Floors 57 - 60
														  Rear Floors 33 - 40
														  Rear Floors 41 - 48
														  Rear Floors 49 - 56
														  Rear Floors 57 - 60
														  ids Floors 33 - 40
														  ids Floors 41 - 48
														  ids Floors 49 - 56
														  ids Floors 57 - 60
														  ids Rear Floors 33 - 40
														  ids Rear Floors 41 - 48
														  ids Rear Floors 49 - 56
														  ids Rear Floors 57 - 60
	14			GRP			CAR			 2			Packet #14: Send Code Blue floor to Car
													  Byte Count
													  Front Code blue floor
													  Rear Code Blue floor
	15			GRP			CAR			9-17		Packet #15: Send Security Car Call Lockout Override data to car
													  Byte Count
													  Security Override Flag
													  Floors 1 - 8
													  Floors 9 - 16
													  Floors 17 - 24
													  Floors 25 - 32
													  Rear Floors 1 - 8
													  Rear Floors 9 - 16
													  Rear Floors 17 - 24
													  Rear Floors 25 - 32
														if (high rise car)
														  Floors 33 - 40
														  Floors 41 - 48
														  Floors 49 - 56
														  Floors 57 - 60
														  Rear Floors 33 - 40
														  Rear Floors 41 - 48
														  Rear Floors 49 - 56
														  Rear Floors 57 - 60
	16												Unused
	20	 		GRP 		SEC			1			Packet #20: Request security data
												   	  Byte Count
													  Response: Packet 21
	21			SEC			GRP			3			Packet #21:	Security data from hall serial board
												   	  Byte Count
													  Floors 1-8,	Floors 25-29
													  Floors 9-16,	Rear Floors 25-28 
													  Floors 17-24,	Rear Floor 29
	22			GRP			CAR			16			Packet #22: Send gpi input data to car
													  Byte Count
													  gpi[0] - gpi[11]
	23			CAR			GRP			16			Packet #23: Send gpo output data to group
													  Byte Count
													  gpo[0] - gpo[11]
	24			GRP			CAR						Packet #24: Request cgpo output data to group from car 
	25			GRP			ALL						Packet #25: PI lantern message for simple hydro
	26	 		ALL			CAR			 2	   		Packet #26: Send/Request version and revision
													  Byte Count
													  Version Number
													  revision	  


	31			GRP			CARS  	    8-120		   	Packet #31: Send Security Code Checksum and Number of Codes for floors 1-30
														Byte Count
														Floor Start
														Number of Codes low byte Floor 1
														Number of Codes high byte Floor 1
														Checksum low byte Floor 1
														Checksum high byte Floor 1
															   .
															   .
															   .
														Number of Codes low byte Floor n (max n = 30)
														Number of Codes high byte Floor n
														Checksum low byte Floor n
														Checksum high byte Floor n

	32			GRP			CARS		8-120			Packet #32: Send Floor Security Codes (50 codes at a time) (each floor)
														Byte Count
														Floor #
														Code Index	low byte
														Code Index 	high byte
														Code 1 1st byte
														Code 1 2nd byte
														Code 1 3rd byte
														Code 1 4th byte 
																.
																.
																.
														Code n 1st byte		(max n = 50)
														Code n 2nd byte
														Code n 3rd byte
														Code n 4th byte 

	33 			GRP			CAR			 3			Packet #33: delete security code
													  Byte Count
													  Floor #	0=invalid, 255=All floors


	35			CAR			GRP			13			Packet #35: Line and Door Voltage from Car 
														Byte Count
										 			  L1 Voltage low byte
													  L1 Voltage high byte
										 			  L2 Voltage low byte
													  L2 Voltage high byte
										 			  L3 Voltage low byte
													  L4 Voltage high byte
										 			  Door Voltage low byte
													  Door Voltage high byte
*/

//*********************************
// Group Comm Definitions
//*********************************

#define e_ok 0
#define e_inv_address 0x01
#define e_inv_pkt_st 0x02
#define e_inv_len 0x04
#define e_inv_ck_sum 0x08
#define e_inv_pkt_end 0x10
#define e_inv_parity 0x20
#define e_inv_case 0x40
#define e_FE_OE 0x80


// Device number declaratons

#define c_CAR1_addr 1
#define c_CAR2_addr 2
#define c_CAR3_addr 3
#define c_CAR4_addr 4
#define c_CAR5_addr 5
#define c_CAR6_addr 6
#define c_CAR7_addr 7
#define c_CAR8_addr 8
#define c_MR_DPY_addr 9
#define c_LBY_DPY_addr 10

#define c_com_no_dev 20			// Max of 20 device address


#if (Tract_HR == 1)
#define c_que_buf_len 60
#else
#define c_que_buf_len 20
#endif

#define c_trm_buf_len 250
#define c_trm_buf_size 400
#define c_rcv_dma_buf_len 300
#define c_rcv_dma_buf_size 400
#define c_rcv_buf_len 250
#define c_rcv_buf_size 400
#define c_hc_brds_per_dvr 24			// Number of hall call boards per driver board

// Car to group buffer size
#if (Tract_HR == 1)
#define c_buf_size 512
#else
#define c_buf_size 250
#endif


#if (Baud == 0)		  // Higher baud rate (57600)
 #define c_slave_min_time_out 2			// 30 Msec
 #define c_slave_max_time_out 7			// 70 Msec
 #define c_master_packet_time 10
 #define c_master_time_out 30
 #define c_full_update_count 30		// roughly 1.5 seconds
 int16 const min_response_wait[3] = {4,2,0};
 #if (Tract_HR == 1)
  int16 const master_count[car_size] = {200,200,150,130,120,110,100,90,80};		 // roughly 1-5 seconds
  int16 const com_master_time_out[car_size]= {3,3,3,4,4,5,5,6,6};
 #else
  int16 const master_count[car_size] = {200,200,150,130,120,110,100};		 // roughly 1-5 seconds
  int16 const com_master_time_out[car_size]= {3,3,3,4,4,5,5};
 #endif
//  int16 const com_master_time_out[car_size]= {2,2,2,3,3,4,4};
#else
 #if(Baud == 1)
  //  Baud rate at 38400
  #define c_slave_min_time_out 4			// 40 Msec
  #define c_slave_max_time_out 8
  #define c_master_packet_time 15
  #define c_master_time_out 35
  #define c_full_update_count 35	// roughly 1.5 seconds
  int16 const min_response_wait[3] = {5,3,1};
  #if (Tract_HR == 1)
   int16 const master_count[car_size] = {150,150,130,120,110,100,90,80,70};		 // roughly 1-5 seconds
   int16 const com_master_time_out[car_size]= {3,3,3,4,4,5,5,6,6};
  #else
   int16 const master_count[car_size] = {150,150,130,120,110,100,90};		 // roughly 1-5 seconds
   int16 const com_master_time_out[car_size]= {3,3,3,4,4,5,5};
  #endif
 #else
  #define c_slave_min_time_out 10			// 100 Msec
  #define c_slave_max_time_out 16
  #define c_master_packet_time 20
  #define c_master_time_out 40
  #define c_full_update_count 15	// roughly 1.5 seconds
  int16 const min_response_wait[3] = {8,4,2};
  #if (Tract_HR == 1)
   int16 const master_count[car_size] = {100,100,75,68,60,55,50,40,30};		 // roughly 1-5 seconds
   int16 const com_master_time_out[car_size]= {3,3,3,4,4,5,5,6,6};
  #else
   int16 const master_count[car_size] = {100,100,75,68,60,55,50};		 // roughly 1-5 seconds
   int16 const com_master_time_out[car_size]= {3,3,3,4,4,5,5};
  #endif
 #endif
#endif


								
int16 Dispatcher = 1;		// Dispatcher for this car
int16 prev_Dispatcher = 1;	// Previous Dispatcher
								
int32 gc_source=0;						// Group comm interrupt source
int16 gc_count=0;						// Group comm interrput count
int16 gc_hc_dev=0;						// Set to 1 when hall call serial board used
int16 gc_hall_io_dev=0;					// Set to 1 when using any serial hall i/o device
int16 gc_restart_req=0;					// restart request
int16 gc_unpack_error = 0;				// Unpack flag set but no data unpacked
int16 gc_ix = 0;							// index for elevator or device to communicate with
int16 gc_car_ix = 0;						// index to keep track of which car was polled
int16 gc_hc_ix = 0;						// index to keep track of polling the remaining hall call devices
int16 gc_car_cnt = 0;						// Number of cars to comm to before hall device
int16 gc_addr = 0;            			// Address sent or received on comm
int16 gc_port = 0;						// Group Comm port
int16 gc_master = 0;						// Set to 1 if this car is master
int16 gc_master_cnt = 0;					// Counter to check for car 1
int16 gc_car = 1;							// Group Comm car 
				// List of devices on group comm
int16 gc_dev_list[c_com_no_dev];			// device numbers are placed sequentially in list
				// Version number for each device
int16 gc_dev_ver[c_com_no_dev];			// Device number is used as index
				// Revision number for each device
int16 gc_dev_rev[c_com_no_dev];			// Device number is used as index
int16 gc_dev_online[c_com_no_dev];		// Device number is used as index
				// Transmit packet count
uint16 gc_dev_tx_cnt[c_com_no_dev];	// Device number is used as index
				// Receive packet count
uint16 gc_dev_rx_cnt[c_com_no_dev];	// Device number is used as index
uint16 gc_loss_cnt[c_com_no_dev];		// Device number is used as index
int16 gc_pkt_11_req_pkt[5] = {0,0,0,0,0};					// Used to request a packet from packet 11
int16 pkt_11_gix = 0;						// packet 11 get index
int16 pkt_11_pix = 0;						// packet 11 put index
int16 gc_num_dev = 0;						// Number of devices on comm
int16 gc_num_hc_dev = 0;					// Number of hall call devices on serial comm
int16 gc_num_car_dev = 0;					// Number of cars on serial comm
int16 gc_trm_int = 0;    					// Car comunication port transmit interupt running
int16 gc_trm_buf_empty = 0;				// Communications port transmit buffer is empty
int16 gc_response_wait = 0;				// Wait for response when set before transmitting again
int16 slave_max_time_out = 7;				// Slave maximum time out
int16 response_wait_time = 5;				// Response wait time (depends on the number of devices)
int16 gc_trm_case = 0;        			// Car switch case index for transmit state machine
int16 gc_trm_ix = 0;          			// trasmit buffer index
int16 gc_trm_len = 0;  			    	// trasmit buffer length
int16 gc_rcv_int = 0;     				// Car comunication port receive interupt running
int16 gc_rcv_case = 0;        			// Car switch case index for receive state machine
int16 gc_rcv_com_status = 0;				// Receive comm status bits
int16 gc_unpack = 0;          			// Set to 1 to unpack Car receive buffer
int16 gc_rcv_pix = 0;          			// Car receive buffer put index
int16 gc_rcv_dma_gix = 0;          		// Car receive buffer dma get index
int16 gc_rcv_gix = 0;          			// Car receive buffer get index
uint8 gc_rcv_len = 0;			// Car receive length sent in Packet
uint8 gc_rcv_act_len = 0;		// Car actual receive length of Packet
uint8 gc_rcv_ck_sum = 0;		// Car receive comm checksum
uint16 gc_trm_buf[c_trm_buf_size];  // transmit buffer
uint8 gc_rcv_buf[c_rcv_buf_size];  // receive buffer
uint16 gc_rcv_dma_buf[c_rcv_dma_buf_size];

#if (Tract_HR == 1)
	bool Car_Not_Talking[car_size]={0,0,0,0,0,0,0,0,0}; // car not communication one 484 port
	bool full_update_req[car_size]={0,0,0,0,0,0,0,0,0};	// Request by group for car to send a full update
#else
	bool Car_Not_Talking[car_size]={0,0,0,0,0,0,0}; // car not communication one 484 port
	bool full_update_req[car_size]={0,0,0,0,0,0,0};	// Request by group for car to send a full update
#endif
int16 full_update_count = 0;
int16 full_update_car = 2;
bool car_full_update_req = 1;			// Car flag to send a full update to the group
bool update_flag=0;
bool Group_Comm = 0;		// Group comm with car
uint16 parity_status = 0;

bool std_pkt_tx[9] = {0,0,0,0,0,0,0,0,0};		// standard packet 2 was transmitted when == 1

// Data arrays to compress transmission from car to group

#if (Tract_HR == 1)
#define c_buf_size 512
#else
#define c_buf_size 250
#endif
uint8 z_trm_buf[c_buf_size];		// temporary transmit hold buffer to remove zero's
uint8 z_rcv_buf[c_buf_size];		// temporary receive hold buffer to remove zero's
uint8 pack_buf[c_buf_size];			// Car data packed into this buffer first
uint8 hold_buf[c_buf_size];			// Buffer used to compare packed data with data transmitted previously to send data that has changed
uint8 unpack_buf[8][c_buf_size];	// Group unpacks the car data that has changed into a buffer for each car

// Hall call comm I/O's and variables

bool gc_luc = 0;					// Lobby Up request for seperate i/o board
bool gc_luc_canc = 0;				// Lobby Up request cancel for seperate i/o board

uint32 gc_uc[2]={0,0};				// Up calls from comm
uint32 gc_uc_set[2]={0,0};			// Set up calls on comm
uint32 gc_uc_canc[2]={0,0};			// Cancel up calls on comm

uint32 gc_dc[2]={0,0};				// Down calls from comm
uint32 gc_dc_set[2]={0,0};			// Set down calls on comm
uint32 gc_dc_canc[2]={0,0};			// Cancel down calls on comm

uint32 gc_ruc[2]={0,0};				// Up calls from comm
uint32 gc_ruc_set[2]={0,0};			// Set up calls on comm
uint32 gc_ruc_canc[2]={0,0};		// Cancel up calls on comm

uint32 gc_rdc[2]={0,0};				// Down calls from comm
uint32 gc_rdc_set[2]={0,0};			// Set down calls on comm
uint32 gc_rdc_canc[2]={0,0};		// Cancel down calls on comm

uint32 gc_uc_dsr[2]={0,0};  		// Up calls from dual serial riser comm
uint32 gc_dc_dsr[2]={0,0};  		// Down calls from  dual serial riser comm
uint32 gc_ruc_dsr[2]={0,0}; 		// Up calls from  dual serial riser comm
uint32 gc_rdc_dsr[2]={0,0}; 		// Down calls from  dual serial riser comm

uint8 gc_ir=0;						// Inconspicious riser switch activation (last floor down call + 1)
uint8 gc_ir_2=0;						// Inconspicious riser switch activation for second car (last floor down call + 2)
uint8 ir_hc=0;						// Use ir for std hall call if set
uint8 rir_hc=0;						// Use rir for std hall call if set

uint32 gc_iruc[2]={0,0};			// Up calls from comm
uint32 gc_iruc_set[2]={0,0};		// Set up calls on comm
uint32 gc_iruc_canc[2]={0,0};		// Cancel up calls on comm

uint32 gc_irdc[2]={0,0};			// Down calls from comm
uint32 gc_irdc_set[2]={0,0};		// Set down calls on comm
uint32 gc_irdc_canc[2]={0,0};		// Cancel down calls on comm

uint32 gc_riruc[2]={0,0};			// Up calls from comm
uint32 gc_riruc_set[2]={0,0};		// Set up calls on comm
uint32 gc_riruc_canc[2]={0,0};		// Cancel up calls on comm

uint8 gc_rir=0;						// IRear nconspicious riser switch activation (last floor rear down call + 1)
uint32 gc_rirdc[2]={0,0};			// Down calls from comm
uint32 gc_rirdc_set[2]={0,0};		// Set down calls on comm
uint32 gc_rirdc_canc[2]={0,0};		// Cancel down calls on comm

uint8 gc_cb_sr=0;					// Code Blue second riser switch activation (last floor down call + 1)
uint8 gc_rcb_sr=0;			 	 	// Code Blue rear second riser switch activation (last floor rear down call + 1)
uint8 cb_dr=0;						// Use cb dual riser for std hall call if set
uint8 rcb_dr=0;						// Use rcb dual riser for std hall call if set

uint32 gc_cb[2]={0,0};				// Code Blue floors from comm
uint32 gc_cb_set[2]={0,0};			// Set code blue call
uint32 gc_cb_canc[2]={0,0};			// Cancel code blue call
uint32 gc_rcb[2]={0,0};				// Rear Code Blue floors from comm
uint32 gc_rcb_set[2]={0,0};			// Set rear code blue call
uint32 gc_rcb_canc[2]={0,0};		// Cancel rear code blue call

uint32 gc_cbsr[2]={0,0};			// Code Blue second riser floors from comm
uint32 gc_cbsr_set[2]={0,0};		// Set code blue second riser call   
uint32 gc_cbsr_canc[2]={0,0};		// Cancel code blue second riser call
uint32 gc_rcbsr[2]={0,0};			// Rear Code Blue second riser floors from comm
uint32 gc_rcbsr_set[2]={0,0};		// Set rear code blue second riser call   
uint32 gc_rcbsr_canc[2]={0,0};		// Cancel rear code blue second riser call

uint8 update_hc_sec=0;
uint8 gc_hsec_prev=0;				// Group front hall call security flag
uint32 gc_hc_sec_prev[2]={0,0};		// Hall Call Lockout Security
uint8 gc_rhsec_prev=0;			   	// Group rear hall call security flag
uint32 gc_rhc_sec_prev[2]={0,0};	// Hall Call Lockout Security


uint8 gc_sec=0;						// Group car call lockout security flag
uint32 gc_cc_sec[2]={0,0};			// Car Call Lockout Security
uint8 gc_rsec=0;					// Group rear car call lockout security flag
uint32 gc_rcc_sec[2]={0,0};			// Car Call Lockout Security
uint8 gc_hsec=0;					// Group front hall call security flag
uint32 gc_hc_sec[2]={0,0};			// Hall Call Lockout Security
uint8 gc_rhsec=0;			   		// Group rear hall call security flag
uint32 gc_rhc_sec[2]={0,0};			// Hall Call Lockout Security

uint8 gc_sec_ovr=0;							// Group car call security override flag
uint32 gc_cc_sec_ovr[2] = {0,0};			// Group car call security override 
uint32 gc_cc_sec_ovr_set[2] = {0,0};		// Group car call security override output set
uint32 gc_cc_sec_ovr_panic[2] = {0,0};		// Group car call security override panic stop
uint32 gc_cc_sec_ovr_toggle[2] = {0,0};		// Group car call security override toggle 
uint8 gc_rsec_ovr=0;							// Group rear car call security override flag
uint32 gc_rcc_sec_ovr[2] = {0,0};			// Group rear car call security override
uint32 gc_rcc_sec_ovr_set[2] = {0,0};		// Group rear car call security override output set
uint32 gc_rcc_sec_ovr_panic[2] = {0,0};		// Group rear car call security override panic stop
uint32 gc_rcc_sec_ovr_toggle[2] = {0,0};	// Group rear car call security override toggle

uint32 gc_xuc[2] = {0,0};	  		// Group cross assignment up call input
uint32 gc_xuct[2] = {0,0};			// Group cross assignment up call transfer flag
uint32 gc_xucl[2] = {0,0};			// Group cross assignment up call latch
uint32 gc_xdc[2] = {0,0};		  	// Group cross assignment down call	input
uint32 gc_xdct[2] = {0,0};			// Group cross assignment up call transfer flag
uint32 gc_xdcl[2] = {0,0};			// Group cross assignment down call latch


uint32 gc_hcb_sec[2] = {0,0};		// hcb security inputs
uint32 gc_hcb_aux[2] = {0,0};		// hcb aux inputs
uint32 gc_hcb_rsec[2] = {0,0};		// hcb rear security inputs
uint32 gc_hcb_raux[2] = {0,0};		// hcb rear aux inputs

int32 tg_dcmsk[2] = {0,0};			// Temporary mask variables
int32 tg_ucmsk[2] = {0,0};
int32 tg_rdcmsk[2] = {0,0};
int32 tg_rucmsk[2] = {0,0};

// pointer is loaded with gc_cb and gc_rcb in utils routine
uint32 *gc_vip;				// Vip floors from comm
uint32 *gc_vip_set;			// Set vip call
uint32 *gc_vip_canc;		// Cancel vip call
uint32 *gc_rvip;			// Rear Vip floors from comm
uint32 *gc_rvip_set;		// Set rear vip call
uint32 *gc_rvip_canc;		// Cancel rear vip call


// Dual riser mask
int32 pfdsr_msk[2]={0,0};		// Primary front dual serial riser mask
int32 sfdsr_msk[2]={0,0};		// Secondary front dual serial riser mask
int32 prdsr_msk[2]={0,0};		// Primary rear dual serial riser mask
int32 srdsr_msk[2]={0,0};		// Secondary rear dual serial riser mask


int16 flt_tx_ix=0;
bool comm_loss=0;

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		
// Group Comm security variables

uint8 gc_Add_Sec_Fl_Req = 0;			// Send security floor to add code
uint8 gc_Add_Sec_Code[4] = {0,0,0,0};	// Security Code to Add a code
uint8 gc_Del_Sec_Fl_Req = 0;			// Send Security floor to delete code
uint8 gc_Del_Sec_Code[4] = {0,0,0,0};	// Security code to delete a code
uint8 gc_Del_All_Codes_Fl_Req = 0;		// Delete all codes for one floor or all floors
uint8 gc_Sec_Fl_Tbl_Req[9] = {0,0,0,0,0,0,0,0,0};			// Floor request for security table
int16 gc_LookUpTable[NumMaxFl];
int16 gc_Sec_Req_Car = 0;							// Current Security Table Request Car
int16 gc_Sec_Req_Seq = 0;							// Security Request Sequence to send codes to car
int16 gc_Sec_Chksum_Fl = 0;						// Floor start for Security Checksum
int16 gc_Sec_Code_Index = 0;						// Code Index pointer
int16 gc_Code_Xfer_Comp = 0;						// Code table transfer complete

struct CdTable gc_CodeTable;
struct Cd_Tbl_Chk gc_Code_Tbl_Chk[fl_size];

// Local Function Declaration

int16 put_pkt_req(int16 pkt_nmb, int16 addr, int16 com_nmb);
void put_pkt_req_all_cars(int16 pkt_nmb);
int16 get_pkt_req(int16 addr, int16 com_nmb);
int16 get_pkt_11_req (void);
int16 get_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb, int16 time);
void put_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb);
void put_pkt_11_req (int16 pkt_nmb);
void send_hc_dvr_req (int16 cmd);
void uncompress(int16 dev, int16 ix);
void zero_req_que(void); 
void zero_status_que(void);
void Req_Car_Voltage (void);
void Send_CC_Codes(void);

int16 receive(void);
void transmit(void);
void grp_com_tx_start(void);
void grp_dma_trm_int(void);
void grp_com_timer_int(void);
void grp_dma_rcv_int (void);
void verify_packet_in_buffer (void);
void Group_Com(void );
void pack_slave_grp_com(void);
uint8 unpack_grp_com(void);
void pack_grp_com(uint16 Packet,uint16 to_addr);
void init_grp_com(void);
void init_grp_port (void);
void reinit_grp_com (void);
void init_grp_DMA(void);
void init_grp_com_variables(void);
int16 compress_zero(int16 buf_len, int16 max_len,int16 compress_flag);
int16 compress(int16 buf_len, int16 max_len);

void init_grp_com (void)
{

	_disable();

	init_grp_com_variables();

	init_grp_port();
	init_grp_DMA();

	PIT.TCTRL2.B.TEN = 0;		// Stop Timer
	INTC_InstallINTCInterruptHandler(grp_com_timer_int, 150, 2);  // Set interrupt vector for PIT2


	GRPport.CR1.B.RE = 1;					// Receiver Enable
  	EDMA.SERQR.R = GRPdmaChRx;				// Enable group coomm DMA channel
	GRPport.CR2.B.RXDMA = 1;				// Receive DMA Control: 0 = disabled, 1 = enabled
	
	_enable();		
	
}

void reinit_grp_com (void)
{
	_disable();

	gc_port = GRPCOM;

	init_grp_port();
	init_grp_DMA();
	
	PIT.TCTRL2.B.TEN = 0;		// Stop Timer
	INTC_InstallINTCInterruptHandler(grp_com_timer_int, 150, 2);  // Set interrupt vector for PIT2

	GRPport.CR1.B.RE = 1;					// Receiver Enable
  	EDMA.SERQR.R = GRPdmaChRx;				// Enable group coomm DMA channel
	GRPport.CR2.B.RXDMA = 1;				// Receive DMA Control: 0 = disabled, 1 = enabled

	_disable();			
}

void init_grp_port (void)
{		// Group comm
	uint16 data_in;
	
	GRPtxDis; 						// Turn off Transmit driver

	GRPport.CR1.B.TE = 0;			// Transmitter Disable
	GRPport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable

	// Control register 1	  
	GRPport.LCR.B.LIN = 0;			// Place in SCI mode	
	GRPport.CR1.B.SBR = 118;		// 57,600 BAUD
	
	GRPport.CR1.B.LOOPS = 0;		// Receiver source mode LOOPS:RSRC 00 = Dual wire mode
	GRPport.CR1.B.RSRC = 0;
	
	GRPport.CR1.B.M = 1;			// Frame Format Mode: set to 1 for nine bits
	GRPport.CR1.B.WAKE = 0;			// Wake bit: set to 1 for 9th bit wake up
	GRPport.CR1.B.PE = 0;			// Parity Enable: 0 = disabled, 1 = enabled 
	GRPport.CR3.B.M2 = 0;
	GRPport.CR3.B.EROE = 1;			// Set error bit in receive data register 
	GRPport.CR3.B.ERFE = 1;			// Set error bit in receive data register
	GRPport.CR3.B.ERPE = 1;			// Set error bit in receive data register
		
	GRPport.CR1.B.PT = 0;			// Parity Type: 0 = even, 1 = odd

	GRPport.CR1.B.ILT = 0;			// Idle Line Type: 0 = idle line detection after low bit
	GRPport.CR1.B.TIE = 0;			// Transmit Interrupt Enable (data tranferred to shift register)
	GRPport.CR1.B.TCIE = 0;			// Transmit Complete Interrupt Enable (data shifted out of shift register)
	GRPport.CR1.B.RIE = 0;			// Receive Interrupt Enable
	GRPport.CR1.B.ILIE = 0;			// Idle line interrupt enable
	GRPport.CR1.B.RWU = 0;			// Receiver Wake Up mode: 0 = normal, 1 = wakeup on 9th bit address
	GRPport.CR1.B.SBK = 0;			// Send Break Character
	
	// Control Register 2
	GRPport.CR2.B.MDIS = 0;			// Module Disable
	GRPport.CR2.B.FBR = 0;			// Fast Bit Error Detect: 0 = normal, 1 = use in LIN mode	
	GRPport.CR2.B.BSTP = 0;			// DMA Stop on Bit Error: 0 = tx dma regardless of bit errors
	GRPport.CR2.B.IEBERR = 0;		// Bit Error Interrupt Enable
	GRPport.CR2.B.RXDMA = 0;		// Receive DMA Control: 0 = disabled, 1 = enabled
	GRPport.CR2.B.TXDMA = 0;		// Transmit DMA Control: 0 = disabled, 1 = enabled
	GRPport.CR2.B.BRK13 = 0;		// Break Character Length
	GRPport.CR2.B.TXDIR = 0;		// TXD pin output enable: Used in signel wire mode
	GRPport.CR2.B.BESM13 = 0;		// Fast Bit Error Detection Sample Mode: 0=RS9,1=RS10 LIN mode only
	GRPport.CR2.B.SBSTP = 0;		// Transmit Stop on Bit Error
	GRPport.CR2.B.RXPOL = 0;		// RXD Pin polarity: 0 = normal, 1 = inverted
	GRPport.CR2.B.PMSK = 0;			// Parity Bit Masking: 0 = in bit position, 1 = 0 in parity position	
	GRPport.CR2.B.ORIE = 0;			// Overrun Interrupt Enable
	GRPport.CR2.B.NFIE = 0;			// Noise Interrupt Enable
	GRPport.CR2.B.FEIE = 0;			// Frame Error Interrupt Enable
	GRPport.CR2.B.PFIE = 0;			// Parity Error Interrupt Enable
	
	
	// Enable must be last
	GRPport.CR1.B.TE = 1;			// Transmitter Enable
	GRPport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable

	data_in = GRPport.SDR.R;
	data_in = GRPport.SDR.R;

}

//*****************************************
// DMA Initialization for group Comm
//*****************************************


void init_grp_DMA(void) {

	// Initialize Transmit Comm DMA
  
  	EDMA.CERQR.R = GRPdmaChTx;      	// Disable Group transmit EDMA channel   

	INTC_InstallINTCInterruptHandler(grp_dma_trm_int, GRPdmaTxIntLoc, 1);  

  	EDMA.CPR[GRPdmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[GRPdmaChTx].R = GRPdmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[GRPdmaChTx].SADDR = (uint32) &gc_trm_buf[0];  	// Load address of source data 
  	EDMA.TCD[GRPdmaChTx].SSIZE = 1;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChTx].SOFF = 2;                         		// After transfer, add 2 to src addr
  	
  	EDMA.TCD[GRPdmaChTx].SLAST = -20;                      		// After major loop, reset src addr 
  	EDMA.TCD[GRPdmaChTx].SMOD = 0;                         		// Source modulo feature not used 

  	EDMA.TCD[GRPdmaChTx].DADDR =  GRPdr; 						// Load address of destination 
  	EDMA.TCD[GRPdmaChTx].DSIZE = 1;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChTx].DOFF = 0;                         		// Do not increment destination addr 
  	EDMA.TCD[GRPdmaChTx].DLAST_SGA = 0;                    		// After major loop, no dest addr change 
  	EDMA.TCD[GRPdmaChTx].DMOD = 0;                         		// Destination modulo feature not used 
  
  	EDMA.TCD[GRPdmaChTx].NBYTES= 2;                    			// Transfer 1 byte per minor loop 
  	EDMA.TCD[GRPdmaChTx].BITER = 10;                       		// 1 major loop iterations 
  	EDMA.TCD[GRPdmaChTx].CITER = 10;                       		// Initialize current iteraction count 
  	EDMA.TCD[GRPdmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[GRPdmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[GRPdmaChTx].INT_MAJ = 1;
  	EDMA.TCD[GRPdmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[GRPdmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[GRPdmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[GRPdmaChTx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[GRPdmaChTx].E_SG = 0; 
  	EDMA.TCD[GRPdmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[GRPdmaChTx].DONE = 0;
  	EDMA.TCD[GRPdmaChTx].ACTIVE = 0;
  	EDMA.TCD[GRPdmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = GRPdmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = GRPdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = GRPdmaChTx;           	// DMA Clear error Register 
  
  	
  	// Initialize Receive Comm DMA

 	EDMA.CERQR.R = GRPdmaChRx;      		// Disable group EDMA channel   
  
  	EDMA.CPR[GRPdmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[GRPdmaChRx].R = GRPdmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[GRPdmaChRx].SADDR = GRPdr;  						// Load address of source data 
  	EDMA.TCD[GRPdmaChRx].SSIZE = 1;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChRx].SOFF = 0;                         		// Do not increment source address
  	
  	EDMA.TCD[GRPdmaChRx].SLAST = 0;                      		// After major loop, reset src addr 
  	EDMA.TCD[GRPdmaChRx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[GRPdmaChRx].DADDR =  (uint32) &gc_rcv_dma_buf[0]; 						// Load address of destination 
  	EDMA.TCD[GRPdmaChRx].DSIZE = 1;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[GRPdmaChRx].DOFF = 2;                         		// Increment destination addr by 2 
  	EDMA.TCD[GRPdmaChRx].DLAST_SGA = - (c_rcv_dma_buf_len * 2);                    		// After major loop reset destination address
  	EDMA.TCD[GRPdmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
  
  	EDMA.TCD[GRPdmaChRx].NBYTES= 2;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[GRPdmaChRx].BITER = c_rcv_dma_buf_len;            // Major loop iterations 
  	EDMA.TCD[GRPdmaChRx].CITER = c_rcv_dma_buf_len;                       	// Initialize current iteraction count 
  	EDMA.TCD[GRPdmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
  	EDMA.TCD[GRPdmaChRx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[GRPdmaChRx].INT_MAJ = 0;
  	EDMA.TCD[GRPdmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[GRPdmaChRx].BITERE_LINK = 0;
  	EDMA.TCD[GRPdmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[GRPdmaChRx].MAJORLINKCH = 0;						// Major link channel 
  	EDMA.TCD[GRPdmaChRx].E_SG = 0; 
  	EDMA.TCD[GRPdmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[GRPdmaChRx].DONE = 0;
  	EDMA.TCD[GRPdmaChRx].ACTIVE = 0;
  	EDMA.TCD[GRPdmaChRx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = GRPdmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = GRPdmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = GRPdmaChRx;           	// DMA Clear error Register 
  
}

//****************************************
// Initialize group comm variables
//****************************************

void init_grp_com_variables(void)
{
	int16 i,el;				

	gc_port = GRPCOM;
	

	// note that if this car is group, addr will be changed to the car the
	// group is talking to.
	gc_addr = cons[carnmb];
	gc_master = 0;
	gc_trm_case = 0;
	gc_rcv_case = 0;
	gc_rcv_com_status = 0;
	gc_rcv_pix = 0;
	gc_rcv_gix = 0;
	gc_trm_ix = 0;
	gc_trm_buf_empty = 1;
	gc_unpack_error = 0;
	for(i=0;i<c_com_no_dev;i++)
	{
		gc_dev_list[i] = 0;
		gc_dev_ver[i] = 0;
		gc_dev_rev[i] = 0;
		gc_dev_online[i] = 0;
		gc_dev_tx_cnt[i] = 0;
		gc_dev_rx_cnt[i] = 0;
		gc_loss_cnt[i] = 0;
	}
	for(el=1;el<=cons[nmbcars];el++)
	{
		full_update_req[el] = 1;
		timers[tgcom_car + el] = 0;
		for(i=0;i<c_buf_size;i++)
			unpack_buf[el-1][i] = 0;
	}
	timers[tgc_tran] = 0;
	timers[tgc_recv] = 0;
	timers[tgcom_master] = 0;
	timers[tcom_response] = 0;	

	for(i=0;i<c_buf_size;i++)
	{
		pack_buf[i] = 0;
		hold_buf[i] = 0;
	}
	pack_grp_com(1,cons[carnmb]);
	gc_num_dev = 0;
	gc_num_hc_dev = 0;
	gc_num_car_dev = 0;
	for (i=cons[carnmb];i <= cons[nmbcars];i++)
		gc_dev_list[gc_num_dev++] = i;

	gc_num_car_dev = gc_num_dev;
	gc_ix = 0;
	gc_car_ix = 0;
	gc_hc_ix = 0;
	gc_hc_dev = 0;
	gc_hall_io_dev = 0;
	gc_car_cnt = 0;

		
	if (gc_num_dev != 0)
	{
		gc_num_hc_dev = gc_num_dev - gc_num_car_dev;
		if (gc_num_hc_dev < 0)
			gc_num_hc_dev = 0;
	}

	slave_max_time_out = c_slave_max_time_out;
	if (gc_num_dev <= 2)
	{
		response_wait_time = min_response_wait[0];
	}
	else if (gc_num_dev <= 4)
	{
		response_wait_time = min_response_wait[1];
	}
	else 
		response_wait_time = min_response_wait[2];
}



//**************************************************
// This is start the comm transmission on a COM Port
//**************************************************

void grp_com_tx_start(void)
{
	int16 len;
	int32 slen;
	
	gc_trm_case = 0;
	gc_trm_buf_empty = 0;

	len = (int16)(gc_trm_buf[3] + 2);
	slen = len * 2;

	_disable();	   
	// Turn off the receiver
	GRPport.CR1.B.RE = 0;			// Receiver Enable: 1 = Enable, 0 = Disable
	
	// Enable transmitter
	EDMA.CER.R = GRPdmaChTx;           	// DMA Clear error Register 
	GRPtxEn; 			// Turn on Transmit driver
 	GRPport.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled
    EDMA.TCD[GRPdmaChTx].SADDR = (uint32) &gc_trm_buf[0];  	// Load address of source data 
  	EDMA.TCD[GRPdmaChTx].SLAST = (0-slen);        // After major loop, reset src addr 
	EDMA.TCD[GRPdmaChTx].NBYTES = 2;	// Transfer len number byte per minor loop 
 	EDMA.TCD[GRPdmaChTx].BITER = len;   // 10 major loop iterations 
  	EDMA.TCD[GRPdmaChTx].CITER = len;   // Initialize current iteraction count 
  	EDMA.SERQR.R = GRPdmaChTx;      	// Enable Group EDMA channel   
	GRPport.CR2.B.TXDMA = 1;			// Transmit DMA Control: 0 = disabled, 1 = enabled
	_enable();
}

//*********************************************
// Group DMA transmit finished interrupt
//*********************************************

void grp_dma_trm_int(void)
{
	gc_trm_int = 1;
	
  	EDMA.CIRQR.R = GRPdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = GRPdmaChTx;      	// Disable Drive EDMA channel   
	GRPport.CR2.B.TXDMA = 0;			// Transmit DMA Control: 0 = disabled, 1 = enabled

	// Transmit end timer - PIT 2
	PIT.TCTRL2.B.TEN = 0;		// Reset Timer
	PIT.LDVAL2.R = (10875 * 4) - 1;		// setup timer 1 for (100 usec) * incremental amount with 108.75 MHz fsys
	PIT.TCTRL2.B.TEN = 1;		// Start Timer
	PIT.TFLG2.B.TIF = 1;			// Clear interrupt flag so an interrupt does not occur right away
	PIT.TCTRL2.B.TIE = 1;		// Enable interrupts
}

//*********************************************
// Group Transmit Finished Timer Interrupt
//*********************************************
void grp_com_timer_int(void)
{
	static int16 tmr_count;
	
	PIT.TFLG2.B.TIF = 1;		// Clear interrupt flag 
	PIT.TCTRL2.B.TIE = 0;		// Disable interrupts
	PIT.TCTRL2.B.TEN = 0;		// Stop Timer
		
	tmr_count = 100;
	while ((tmr_count > 0) && (GRPport.SR.B.TC == 0))
	{
		tmr_count--;
	}
	GRPtxDis; 				// Turn off Transmit driver
	gc_trm_buf_empty = 1;
	gc_trm_case = 0;
	gc_dev_tx_cnt[gc_addr]++;
	// Turn on the receiver
	GRPport.CR1.B.RE = 1;			// Receiver Enable: 1 = Enable, 0 = Disable

}

// ******************************************************
//		Verify Packet Data in buffer
//*******************************************************

void verify_packet_in_buffer (void)
{
	static int16 ite_count, beg_count, dma_count, status;

	beg_count = (int16)EDMA.TCD[GRPdmaChRx].BITER;
	ite_count = (int16)EDMA.TCD[GRPdmaChRx].CITER;
	dma_count = (int16)(beg_count - ite_count);
	
	status = e_ok;
	
	while ((gc_rcv_dma_gix != dma_count) && (status == e_ok) && (gc_rcv_case != 6))
	{
		status = receive();

		gc_rcv_dma_gix++;
		if (gc_rcv_dma_gix >= c_rcv_dma_buf_len)
			gc_rcv_dma_gix = 0;
	}
	if ((gc_unpack == 1) && (gc_rcv_case == 6))
		gc_rcv_case = 0;

}

// ******************************************************
//		Empty data from receive fifo
//*******************************************************

int16 receive (void)
{

	int16 exception;

	exception = e_ok;
	
	if (((gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x2000) != 0) && (gc_rcv_case > 0))
	{
		exception = e_FE_OE;
	}
	else
	{
		 
		switch (gc_rcv_case)
		{
			case 0:						// Get Start flag
				if ( (int16)(gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x20FF) == (int16)(':'))   		// Enable receive interrupt
				{
					if ((gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x8000) != 0)
					{		  // Must have the nineth bit set for the start flag (shows up in the RN flag)
						gc_rcv_int = 1;
						gc_rcv_case = 1;
						gc_rcv_len = 0;
						gc_rcv_act_len = 1;
						gc_rcv_ck_sum = 0;
						gc_rcv_gix = 0;
						gc_rcv_pix = 0;
						gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
						gc_rcv_pix++;
					}
					else
					{
						gc_rcv_case = 0;
					}
				}
				else
				{
					gc_rcv_case = 0;
				}
				
				break;

			case 1:					// Get address
				if ((int16)(gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x20FF) == gc_addr)
				{					// Valid address
					if (((gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x8000) != 0) || (gc_addr > 8))
					{		  // Must have the nineth bit set for the ADDRESS (shows up in the RN flag)
						gc_rcv_int = 1;
						gc_rcv_case = 2;		
						gc_rcv_len = 0;
						gc_rcv_act_len++;
						gc_rcv_ck_sum += (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
						gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
						gc_rcv_pix++;
					}
					else
					{
						exception = e_inv_address;
					}
				}
				else
				{
					exception = e_inv_address;		 // No exception since this is a normal exit (not my address)
				}
				break;				

			case 2:						// get Packet number
				gc_rcv_int = 1;
				gc_rcv_ck_sum += (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
				gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
				gc_rcv_pix++;
				gc_rcv_act_len++;
				gc_rcv_case = 3;
				break;

			case 3:     				// get comm length
				gc_rcv_int = 1;
				gc_rcv_len = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
				gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
				gc_rcv_ck_sum += gc_rcv_len;
				gc_rcv_pix++;
				if ((gc_rcv_pix + gc_rcv_len) >= c_rcv_buf_size)
				{ 		// Unpack is not working so ingnore packet
					exception = e_inv_len;
				}
				else
				{
					gc_rcv_act_len++;
					gc_rcv_case = 4;
				}
				break;

			case 4:					// receive data
				gc_rcv_int = 1;
				if (gc_rcv_act_len >= gc_rcv_len)
				{  			// finished receiving packet
					gc_rcv_ck_sum = (0xFF - gc_rcv_ck_sum) + 1;
					if (gc_rcv_ck_sum == (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix])
					{
						gc_rcv_case = 5;
						gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
						gc_rcv_pix++;
					}
					else
					{	
						exception = e_inv_ck_sum;
					}
				}
				else
				{           	// Get packet data
					gc_rcv_ck_sum += (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
					gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
					gc_rcv_act_len ++;
					gc_rcv_pix++;
					if (gc_rcv_pix >= c_rcv_buf_size)
						gc_rcv_case = 0;					// error
				}
				break;

			case 5:					// receive end
				gc_rcv_int = 1;
				if ((uint8)(gc_rcv_dma_buf[gc_rcv_dma_gix] & 0x00FF) == (uint8)('@'))
				{		// valid end character
					gc_rcv_buf[gc_rcv_pix] = (uint8)gc_rcv_dma_buf[gc_rcv_dma_gix];
					gc_rcv_pix++;
					gc_rcv_buf[gc_rcv_pix] = gc_rcv_act_len;	// store actual receive length
					gc_rcv_pix++;
					gc_rcv_buf[gc_rcv_pix] = gc_rcv_ck_sum;
					gc_unpack = 1;			// Packet has has been received (master unpacks right away)
					gc_rcv_case = 6;
				}
				else
				{
					exception = e_inv_pkt_end;
				}
				break;				
			
			case 6:			// finished
				gc_rcv_case = 6;
				break;

			default:    // Trun on receiver and line status interrupts
				exception = e_inv_case;
				break;
		}
	}

	
	if (exception != e_ok) 
	{
		gc_rcv_case = 0;
		gc_rcv_len = 0;
		gc_rcv_gix = 0;
		gc_rcv_pix = 0;
		gc_rcv_act_len = 0;
		gc_rcv_com_status |= exception;
	}
	return(exception);
}



//********************************
// group comunications procedure
//********************************
void Group_Com()
{
	int16 i,j,Packet,comm_time_out;
	int emp_bits;

	int16 static car_comm_flt_tbl[9] = {f_Car1_Comm,f_Car1_Comm,f_Car2_Comm,f_Car3_Comm,
									f_Car4_Comm,f_Car5_Comm,f_Car6_Comm,f_Car7_Comm,f_Car8_Comm};

#if (Traction == 1)
	float tempd;
#endif

	if (cons[nmbcars] == 1)
		Dispatcher = 1;
	else if (cons[nmbcars] >= 2)
	{
		if (cons[carnmb] == 1)
			Dispatcher = 1;
		else if (cons[carnmb] == 2)
		{
			if (timers[tgcom_car1] > 30)
			{
#if ((Traction == 1) || (Tract_OL == 1))
				if ((cons[Ser_IGEP] == 1) && (igep_com_online == 1))
					Dispatcher = 2;		// Car 2 can be dispatcher since IGEP master is on line
				else if ((Drive_Type[cons[carnmb]] != 1) && (Drive_Type[cons[carnmb]] != 9) && (rdinp(i_EMP) == 1))
#else
				if ((Drive_Type[cons[carnmb]] != 1) && (Drive_Type[cons[carnmb]] != 9) && (rdinp(i_EMP) == 1))
#endif
					Dispatcher = 1;		// Car 2 cannot be dispatcher on emergency power and not a hydro
				else
				{
					if (Dispatcher != 2)
						comm_loss = 1;
					Dispatcher = 2;
				}
			}
			else
				Dispatcher = 1;
		}
		else
		{ 					   // all other cars
			if (timers[tgcom_car1] < 30)
				Dispatcher = 1;		// comm to car 1
			else if (timers[tgcom_car2] < 30)
				Dispatcher = 2;			// Comm to car 2
			else if ((fvars[fvcmaster] >= cons[carnmb]) && (fvars[fvcmaster] <= cons[carnmb]))
 				Dispatcher = cons[carnmb];			// Select another car to be the dispatcher other than 1 or 2
			else
				Dispatcher = 1;			// Default to car 1
		}
	}

	if ((hsf == 0) && (procf >= 3) && (procf <= 11))
		Group_IO_Com[cons[carnmb]][g_nxt_stp] = position;
	Group_IO_Com[cons[carnmb]][g_pos] = position;
	Group_IO_Com[cons[carnmb]][g_procf] = procf;
	Group_IO_Com[cons[carnmb]][g_servf] = servf;

	emp_bits = 0;
	if (rdinp(i_EPS) == 1)
		emp_bits |= 0x08;
	else
		emp_bits &= ~0x08;

	if (rdinp(i_EMP) == 1)
		emp_bits |= 0x10;
	else
		emp_bits &= ~0x10;

	if ((rdinp(i_EPT) == 1) && ((cons[EMPop] & 0x01) != 0))
		emp_bits |= 0x20;
	else
		emp_bits &= ~0x20;


	Group_IO_Com[cons[carnmb]][g_empf] = (int16)((empf & 0x07) | emp_bits);		// clear ept input to send to group

	Group_IO_Com[cons[carnmb]][g_firef] = firef;
	Group_IO_Com[cons[carnmb]][g_doorf] = doorf;
	Group_IO_Com[cons[carnmb]][g_rdoorf] = rdoorf;
	Group_IO_Com[cons[carnmb]][g_dirf] = dirf;
	Group_IO_Com[cons[carnmb]][g_dpref] = dpref;
	Group_IO_Com[cons[carnmb]][g_codebf] = codebf;
	Group_IO_Com[cons[carnmb]][g_dcalls] = dcalls;
  	Group_IO_Com[cons[carnmb]][g_cur_flt] = Current_Fault;
  	Group_IO_Com[cons[carnmb]][g_statusf1_l] = (int16)(statusf & 0x0000ffffL);
  	Group_IO_Com[cons[carnmb]][g_statusf1_h] = (int16)((statusf & 0xffff0000L) >> 16);
	Group_IO_Com[cons[carnmb]][g_control] = (int16)((up_peak_trigger & 0x1) | (((onward_calls != 0) & 0x1) << 1) |
											(((no_wheelchair_cap  != 0) & 0x01) << 2));	  // Set control bits (15 bits unused)
	Group_IO_Com[cons[carnmb]][g_ss_stat] = safety_string_status;
	Group_IO_Com[cons[carnmb]][g_vipf] = vipf;
	Group_IO_Com[cons[carnmb]][g_nmbcc] = nmb_cc;
	Group_IO_Com[cons[carnmb]][g_topcc] = top_cc;
	Group_IO_Com[cons[carnmb]][g_botcc] = bot_cc;
	Group_IO_Com[cons[carnmb]][g_fire_status] = fire_status;
	Group_IO_Com[cons[carnmb]][g_ins_status] = ins_status;
  	Group_IO_Com[cons[carnmb]][g_statusf2_l] = (int16)(statusf2 & 0x0000ffffL);
  	Group_IO_Com[cons[carnmb]][g_statusf2_h] = (int16)((statusf2 & 0xffff0000L) >> 16);
	Group_IO_Com[cons[carnmb]][g_pwr_status] = power_status;
	
	if (f.Flts[Flt_ix].code != 0)
		car_has_fault[cons[carnmb]] = true;
	else
		car_has_fault[cons[carnmb]] = false;		// no fault

#if (Traction == 1)
  	g_enc_dir[cons[carnmb]] = Enc_dir;
	tempd = (float)((Dmd_Velocity * 5.0)/Pulses_per_Inch);	// has to be two steps due to conversion error
  	g_dmd_vel[cons[carnmb]] = (int16)(tempd + 0.5);
	tempd = (float)((Enc_vel * 5.0)/Pulses_per_Inch);
  	g_enc_vel[cons[carnmb]] = (int16)(tempd + 0.5);
  	g_flr_cnt[cons[carnmb]] = DPP_Floor_Pos[position];
  	g_pls_cnt[cons[carnmb]] = DPP_Count;
#else
  	g_enc_dir[cons[carnmb]] = 0;
  	g_dmd_vel[cons[carnmb]] = 0;
  	g_enc_vel[cons[carnmb]] = 0;
  	g_flr_cnt[cons[carnmb]] = 0;
  	g_pls_cnt[cons[carnmb]] = 0;
#endif

	g_L1_Voltage[cons[carnmb]] = L1_Voltage;
	g_L2_Voltage[cons[carnmb]] = L2_Voltage;
	g_L3_Voltage[cons[carnmb]] = L3_Voltage;
	g_Door_Voltage[cons[carnmb]] = Door_Voltage;

	if (cons[carnmb] == Dispatcher)
	{
		gc_dev_ver[cons[carnmb]] = version;
		gc_dev_rev[cons[carnmb]] = revision;
		gc_dev_online[cons[carnmb]] = 1;
		car_hc_fire_door_open = grp_hc_fire_door_open;
		group_alrm[cons[carnmb]] = car_alrm;			
	}
	else
	{
		gc_dev_ver[cons[carnmb]] = 0;
		gc_dev_rev[cons[carnmb]] = 0;
		gc_dev_online[cons[carnmb]] = 0;
	}

	if (comm_loss == 1)
	{		  // clear variables from the group on comm loss
		comm_loss = 0;
		g_park[cons[carnmb]] = 0;
		gempf = 0;
		ncu_door[cons[carnmb]] = 0; 
		lby_req[cons[carnmb]] = 0;
		ncu_que[cons[carnmb]] = 0;
		g_cb_fl[cons[carnmb]] = 0;
		g_rcb_fl[cons[carnmb]] = 0;
		grp_codebf[cons[carnmb]] = 0;
		asvto_pos[cons[carnmb]] = 0;
		car_asgn_sec = 0;
		alt_lobby = 0;
		local_up_peak = 0;
	}
	
	if (gc_trm_buf_empty == 1)
		verify_packet_in_buffer();		// not transmitting so check receive buffer


	if ((cons[carnmb] == Dispatcher) && (gc_master == 1))
	{  			// This car is the group

 		if (gc_trm_buf_empty == 0)
 			timers[tcom_response] = 0;			// Start timing after the packet is sent to the slave

		if (timers[tcom_response] >= slave_max_time_out)
			comm_time_out = 1;
		else if ((gc_rcv_case < 2) && (timers[tcom_response] >= c_slave_min_time_out))
			comm_time_out = 1;
		else
			comm_time_out = 0;

		if (timers[tcom_response] >= response_wait_time)
			gc_response_wait = 1;
		
		if ((gc_trm_buf_empty == 1) && (gc_num_dev > 1) && 
			(gc_response_wait == 1) && ((gc_unpack == 1) || (comm_time_out == 1)))
		{			// unpack data when available and minimum delay has passed

			if (gc_unpack == 1)
			{		   // data is ready to be unpacked
				gc_unpack = 0;
				if (unpack_grp_com() == 1)
				{
					if ((gc_dev_ver[gc_dev_list[gc_ix]] == 0) && (gc_dev_rev[gc_dev_list[gc_ix]] == 0))
					{
						if ((gc_dev_list[gc_ix] >= c_CAR1_addr) && (gc_dev_list[gc_ix] <= c_CAR8_addr))
						{		// use packet 26 only for cars
							put_pkt_req(26,gc_dev_list[gc_ix],gc_port);
						}
					}
							
					gc_dev_online[gc_dev_list[gc_ix]] = 1;
					gc_loss_cnt[gc_dev_list[gc_ix]] = 0;		// restart count
					if (cons[cc_pb_sec] == 1)
						Send_CC_Codes();

					if (cons[Voltage_Sensor] == 1)
						Req_Car_Voltage();
				}
				else
					gc_unpack_error ++;
			}
			else	// response time-out and no packet
			{
				gc_loss_cnt[gc_dev_list[gc_ix]]++;
				if (gc_loss_cnt[gc_dev_list[gc_ix]] > 9)
				{
					gc_loss_cnt[gc_dev_list[gc_ix]] = 1;
					gc_dev_online[gc_dev_list[gc_ix]] = 0;
				}
			}

			if (gc_restart_req == 1)
				reinit_grp_com();
			
			gc_restart_req = 0;
			if (gc_car_ix >= (cons[nmbcars]-cons[carnmb]))
			{
				gc_car_ix = 1;	// finished the cars so start over
			}
			else
				gc_car_ix++;
			gc_ix = gc_car_ix;

			if(cons[carnmb] >= 2)
			{
				if(gc_master_cnt > master_count[cons[nmbcars]])
				{	// every 50 to 100 times (3 to 5sec) through the loop release the comm to look for car 1 as master
					gc_master_cnt = 0;
					gc_master = 0;	   // cleared to allow car 1 to pick up as master

					// Setup car to receive from master 
					pack_slave_grp_com();
					gc_unpack = 0;
				}
				else
					gc_master_cnt++;
			}

			if ((gc_dev_list[gc_ix] != cons[carnmb]) && (gc_master == 1))
			{     // If this is not me
				full_update_count++;
				if (full_update_count > c_full_update_count) // get update every 1.5 second
				{					
					full_update_count = 0;
					if ((full_update_car <= 1) || (full_update_car > cons[nmbcars]))
						full_update_car = 2;
					else 
						full_update_car ++;
					full_update_req[full_update_car] = 1;					// set update flag for all cars
				}
				if ((gc_dev_list[gc_ix] >= c_CAR1_addr) && (gc_dev_list[gc_ix] <= c_CAR8_addr))
				{
					if (std_pkt_tx[gc_dev_list[gc_ix]] == 1)				// do not send consecutive request packets to cars
						Packet = get_pkt_req(gc_dev_list[gc_ix],gc_port);	// get a Packet from the queue
					else
						Packet = 0;
				}
				else
					Packet = get_pkt_req(gc_dev_list[gc_ix],gc_port);	// get a Packet from the queue
				if (Packet == 0)
				{
					if ((gc_dev_list[gc_ix] >= c_CAR1_addr) && (gc_dev_list[gc_ix] <= c_CAR8_addr))
						std_pkt_tx[gc_dev_list[gc_ix]] = 1;
					Packet = 2;       // else send the standard Packet
				}
				pack_grp_com(Packet,gc_dev_list[gc_ix]);  // transmit group data to car
				gc_addr = gc_dev_list[gc_ix];
				grp_com_tx_start();
				timers[tcom_response] = 0;				// restart timer
				gc_response_wait = 0;
			}
		}
	}
	else
	{    // This car is not the group
		if ((gc_unpack != 0) || (timers[tcom_response] > c_master_packet_time))
		{							// tcom_response is a 10 msec timer
			if (gc_unpack == 1)
			{
				unpack_grp_com();
				gc_unpack = 2;
			}
			// pack data to transmit back to the group next time
			if ((gc_trm_buf_empty == 1) && ((gc_unpack == 2)|| (timers[tcom_response] > c_master_time_out)))
			{
				pack_slave_grp_com();
				if (gc_unpack == 2)		// data received so respond
					grp_com_tx_start();
				gc_unpack = 0;
			}
		}
	}

	if(gc_num_dev > 1)
	{
		if(gc_trm_int == 1)
		{
			timers[tgc_tran] = 0;
			gc_trm_int = 0;
		}
		if(gc_rcv_int == 1)
		{
			timers[tgc_recv] = 0;
			gc_rcv_int = 0;
			if ((gc_master == 0) && (cons[carnmb] == 2))
				timers[tgcom_master] = 0;
		}

		if (cons[carnmb] == Dispatcher) 
		{				// I'm the dispatcher
			if ((gc_master == 1) || (cons[carnmb] == 1))	// Car 1 is always the master when it is on line
			{		   
				timers[tgcom_master] = 0;
				gc_master = 1;
			}
			else if (timers[tgcom_master] > com_master_time_out[cons[carnmb]])
			{			  // check to become master
				gc_master = 1;
				timers[tgc_tran] = 0;
				timers[tgc_recv] = 0;
				timers[tgcom_master] = 0;
			}
		}
		else
		{
			gc_master = 0;				// If I'm not the dispatcher, I can't be the comm master.
			timers[tgcom_master] = 0;
		}

		if (((timers[tgc_recv] > 50) || (timers[tgc_tran] > 50)) && (gc_trm_buf_empty == 1)) 
		{		 // No interrupts for too long and not transmitting
			reinit_grp_com();
			timers[tgc_tran] = 0;
			timers[tgc_recv] = 0;
			gc_trm_buf_empty = 1;
		}
	}

	if (cons[carnmb] == Dispatcher)
	{
 		Group_Comm = 1;			// if I'm the dispatcher, I'll always have comm with myself.
		Car_Not_Talking[cons[carnmb]] = 0;
		j = 1;
		for (i=cons[carnmb];i<=cons[nmbcars];i++)
		{	
			if (i != cons[carnmb])
			{
				if (timers[tgcom_car+i] > 20) 
				{		// comm loss with external car
					if (Car_Not_Talking[i] == 0)		// first time car is not talking
					{
					  	record_fault(car_comm_flt_tbl[i]);	  // set car comm fault
					}
					Car_Not_Talking[i] = 1;
					j++;

					if (timers[tgcom_car+i] > 50)
					{
						timers[tgcom_car+i] = 31;
						if (j >= cons[nmbcars])		// all the cars must have timed out
							gc_restart_req = 1;
					}
				}
			}
		}

		if (cons[carnmb] >= 2)
		{				// Special case for car 2 when it is the dispatcher
			if (prev_Dispatcher == 1)
			{						// Car 2 had a comm fault with car 1
			  	record_fault(f_Group_Comm);
			}
			for (i=1;i<cons[carnmb];i++)
				Car_Not_Talking[i] = 1;
			if (timers[tgcom_car1] > 30)
			{
				if (timers[tgcom_car1] > 50)
				{								 // no comm with CAR 1
					if (gc_loss_cnt[cons[carnmb]] > 9)
						gc_loss_cnt[cons[carnmb]] = 1;
					else
						gc_loss_cnt[cons[carnmb]]++;
					gc_dev_online[cons[carnmb]] = 0;
					timers[tgcom_car1] = 31;
				}
			}
			else
			{
				gc_dev_online[cons[carnmb]] = 1;
				gc_loss_cnt[cons[carnmb]] = 0;
			}
		}
	}	 
	else
	{
		if ( (timers[tgcom_car1] > 20) && (timers[tgcom_car2] > 20) && (timers[tgcom_car3] > 20) 
			&& (timers[tgcom_car4] > 20) && (timers[tgcom_car5] > 20) && (timers[tgcom_car6] > 20)
			&&  (timers[tgcom_car7] > 20))
		{
			if (timers[tgcom_car1] > 50) 
			{								 // no comm with group
				if (gc_loss_cnt[cons[carnmb]] > 9)
					gc_loss_cnt[cons[carnmb]] = 1;
				else
					gc_loss_cnt[cons[carnmb]]++;
				gc_dev_online[cons[carnmb]] = 0;
				reinit_grp_com();
				timers[tgcom_car1] = 31;
			}
			if (Group_Comm == 1)
			{	
			  	record_fault(f_Group_Comm);
				Restart_Com(gc_port);
			}
			Group_Comm = 0;		// No comm with car 1 or 2
			if ( ((Security_Type[cons[carnmb]] & 0x02) != 0) || ((Security_Type[cons[carnmb]] & 0x08) != 0) ||
				 ((Security_Type[cons[carnmb]] & 0x04) != 0))
			{
				gc_sec = 0;				// Comm lost so no security from group
				gc_rsec = 0;
			}
			if (cons[loc_svc_tmr] == 0)
			{		// No service activation timers if group loss and no local activation
				tmr_svc[s_cc_pb_sec] = 0;
				tmr_svc[s_sabbath] = 0;
				tmr_svc[s_ccl_override] = 0;
				tmr_svc[s_cc_lockouts] = 0;
		   }
		}
		else
		{
			gc_dev_online[cons[carnmb]] = 1;
			gc_loss_cnt[cons[carnmb]] = 0;
		}
	}

	prev_Dispatcher = Dispatcher;

	return;
}

//***************************************************
// Pack the slave com buffer and set to receive	data
//***************************************************

void pack_slave_grp_com(void)
{
	int16 Packet;
	Packet = get_pkt_req(cons[carnmb],gc_port);	// get a Packet from the queue
	if (Packet == 0)
			Packet = 1;       // else send the standard Packet
	gc_addr = cons[carnmb];
	pack_grp_com(Packet,cons[carnmb]);
	// set receiver on and line status interrupt 
	_disable();
	GRPtxDis; 			// Turn off Transmit driver
	// Turn off the receiver
	GRPport.CR1.B.RE = 1;			// Receiver Enable: 1 = Enable, 0 = Disable
	_enable();
	timers[tcom_response] = 0;				// restart timer
}

//*******************************
// Pack car to group comm packet
//*******************************
int16 pack_car_data_to_group(int16 dev)
{
	int16 ix,i,bc,j;
 	pack_buf[0] = ':';      	// Start of Packet
 	pack_buf[1] = (uint8)dev; 			// From address
 	pack_buf[2] = 1; 			// Packet number
 	if (car_full_update_req == 1)
 	{
		if (pack_buf[4] != 2)
		{
 			pack_buf[4] = 1;			// Set for full update
			update_flag = 1;
		}
 		full_update_count = 0;
 		car_full_update_req = 0;
 	}
 	else
 	{
 		full_update_count++;
		if ((update_flag == 1) && (full_update_count > 3))
		{
 			full_update_count = 0;
 			pack_buf[4] = 2;		// Continue update
			update_flag = 0;
		}
 		else if (full_update_count > 15)
 		{
 			full_update_count = 0;
 			pack_buf[4] = 1;		// Group has not requested an update
 			update_flag = 1;
 		}
 		else
 			pack_buf[4] = 0;
 	}


	// Pack Group_IO_Com data

 	ix = 5;
 	for(i=0; i<=g_cur_flt; i++)
 	{
 		pack_buf[ix++] = (uint8)Group_IO_Com[dev][i];
 	}
 	uwu.w = Group_IO_Com[dev][g_statusf1_l];
 	pack_buf[ix++] = uwu.b[lbw];
 	pack_buf[ix++] = uwu.b[hbw];
 	pack_buf[ix++] = (uint8)g_enc_dir[dev];
 	uwu.w = g_dmd_vel[dev];
 	pack_buf[ix++] = uwu.b[lbw];
 	pack_buf[ix++] = uwu.b[hbw];
 	uwu.w = g_enc_vel[dev];
 	pack_buf[ix++] = uwu.b[lbw];
 	pack_buf[ix++] = uwu.b[hbw];
 	uiu.dw = g_flr_cnt[dev];
 	pack_buf[ix++] = uiu.b[lb];
 	pack_buf[ix++] = uiu.b[mlb];
 	pack_buf[ix++] = uiu.b[mhb];
 	pack_buf[ix++] = uiu.b[hb];
 	uiu.dw = g_pls_cnt[dev];
 	pack_buf[ix++] = uiu.b[lb];
 	pack_buf[ix++] = uiu.b[mlb];
 	pack_buf[ix++] = uiu.b[mhb];
 	pack_buf[ix++] = uiu.b[hb];
 	uwu.w = Group_IO_Com[dev][g_statusf1_h];
 	pack_buf[ix++] = uiu.b[lb];
	pack_buf[ix++] = uiu.b[mlb];
	uwu.w = Group_IO_Com[dev][g_control];
	pack_buf[ix++] = uwu.b[lbw];
	pack_buf[ix++] = uwu.b[hbw];
	uwu.w = Group_IO_Com[dev][g_ss_stat];
	pack_buf[ix++] = uwu.b[lbw];
	pack_buf[ix++] = uwu.b[hbw];

	if (cons[Second_Riser] != 0)
	{
		pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_cancel_irup];
		pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_cancel_irdn];
		pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_cancel_rirup];
		pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_cancel_rirdn];
	}

	if ((cons[VipSvc] != 0) || (cons[SecFlCfg] == 4))
	{
		pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_vipf];
		pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_nmbcc];
		pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_topcc];
		pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_botcc];
	}
	// Pack iodata
 	for(i=0; i<=144; i++)
 	{                      
 		pack_buf[ix++] = iodata[1][i];
 	}
 	if (f.Flts[Flt_ix].code != 0)
 		pack_buf[ix++] = true;
 	else
 		pack_buf[ix++] = false;		// no fault

 	uwu.w = Group_IO_Com[dev][g_cur_flt];
	pack_buf[ix++] = uwu.b[hbw];		// send second byte

	pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_fire_status];

	pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_ins_status];	// Inspection Status

	uwu.w = Group_IO_Com[dev][g_statusf2_l];
	pack_buf[ix++] = uwu.b[lbw];
	pack_buf[ix++] = uwu.b[hbw];

											// add 6 extra bytes to com 10/12/09	v5.50.4
	uwu.w = Group_IO_Com[dev][g_statusf2_h];
	pack_buf[ix++] = uwu.b[lbw];
	pack_buf[ix++] = uwu.b[hbw];

	pack_buf[ix++] = (uint8)Group_IO_Com[dev][g_pwr_status];

	pack_buf[ix++] = (uint8)car_alrm;

	// Send 2 spare bytes 
	pack_buf[ix++] = 0;
	pack_buf[ix++] = 0;

	// Pack new iodata
 	for(i=145; i<=156; i++)
 	{                      
 		pack_buf[ix++] = iodata[1][i];
 	}

	bc = compress(ix,160);

	if (bc == 0)
	{			// compression did not work or full update flag set so send full buffer
		if (pack_buf[4] == 0)
		{
			if (update_flag == 0)
			{
				update_flag = 1;
				pack_buf[4] = 1;					// full update
			}
			else
			{
				pack_buf[4] = 2;					// full update
				update_flag = 0;
			}

		}
		j = 0;
		for(i=0;i<=(g_cancel_dn+5);i++)
		{										// 9 bytes
			z_trm_buf[j++] = pack_buf[i];	  	 
			hold_buf[i] = pack_buf[i];
		}
		if (pack_buf[4] == 1)				 
		{									 // 59 bytes
			for(i=(g_eta_2d+5);i<=(g_eta_31d+5);i++)
			{						  // 30 bytes
				z_trm_buf[j++] = pack_buf[i];	  
				hold_buf[i] = pack_buf[i];
			}
			for(i=(g_eta_1u+5);i<=(g_eta_29u+5);i++)
			{					   // 29 bytes
				z_trm_buf[j++] = pack_buf[i];	  
				hold_buf[i] = pack_buf[i];
			}
		}
		else
		{
			for(i=(g_eta_32d+5);i<=(g_eta_61d+5);i++)
			{						// 29 bytes
				z_trm_buf[j++] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
				hold_buf[i] = pack_buf[i];
			}
			for(i=(g_eta_30u+5);i<=(g_eta_60u+5);i++)
			{						// 30 bytes
				z_trm_buf[j++] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
				hold_buf[i] = pack_buf[i];
			}
		}
		for(i=(g_procf+5);i<=(g_cur_flt+26);i++)
		{								   // 162 - 129	 (34 bytes)
			z_trm_buf[j++] = pack_buf[i];	  
			hold_buf[i] = pack_buf[i];
		}

		if (cons[Second_Riser] != 0)
		{
			hold_buf[i] = pack_buf[i];
			z_trm_buf[j++] = pack_buf[i++];
			hold_buf[i] = pack_buf[i];
			z_trm_buf[j++] = pack_buf[i++];
			hold_buf[i] = pack_buf[i];
			z_trm_buf[j++] = pack_buf[i++];
			hold_buf[i] = pack_buf[i];
			z_trm_buf[j++] = pack_buf[i++];
		}
		if ((cons[VipSvc] != 0) || (cons[SecFlCfg] == 4))
		{
			hold_buf[i] = pack_buf[i];
			z_trm_buf[j++] = pack_buf[i++];
			hold_buf[i] = pack_buf[i];
			z_trm_buf[j++] = pack_buf[i++];
			hold_buf[i] = pack_buf[i];
			z_trm_buf[j++] = pack_buf[i++];
			hold_buf[i] = pack_buf[i];
			z_trm_buf[j++] = pack_buf[i++];
		}

		ix = i;
		// alway send the first 20 bytes of iodata
		for(i=ix;i<(ix+20);i++)
		{									// 20 bytes
			z_trm_buf[j++] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
			hold_buf[i] = pack_buf[i];
		}
		ix = i;
		if (pack_buf[4] == 1)
		{		 // send the iodata

			for(i=ix;i<(ix+72);i++)				 // 72 bytes
			{
				z_trm_buf[j++] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
				hold_buf[i] = pack_buf[i];
			}
		}
		else
		{										 // 72 bytes
			for(i=(ix+72);i<(ix+144);i++)
			{
				z_trm_buf[j++] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
				hold_buf[i] = pack_buf[i];
			}
		}
		z_trm_buf[j++] = pack_buf[i++];			// 1 byte send fault code
		z_trm_buf[j++] = pack_buf[i++];			// Send high byte of current fault
		z_trm_buf[j++] = pack_buf[i++];			// fire status
		z_trm_buf[j++] = pack_buf[i++];			// inspection status
		z_trm_buf[j++] = pack_buf[i++];			// statusf2 low byte
		z_trm_buf[j++] = pack_buf[i++];			// statusf2 middle low byte

		z_trm_buf[j++] = pack_buf[i++];			// statusf2 middle high byte
		z_trm_buf[j++] = pack_buf[i++];			// statusf2 high bytte
		z_trm_buf[j++] = pack_buf[i++];			// g_pwr_status byte
		z_trm_buf[j++] = pack_buf[i++];			// spare byte
		z_trm_buf[j++] = pack_buf[i++];			// spare byte
		z_trm_buf[j++] = pack_buf[i++];			// spare byte

		ix = 1;
		if (pack_buf[4] == 1)
		{
			for(i=ix;i<(ix+6);i++)					// new iodata
				z_trm_buf[j++] = pack_buf[i++];			
		}
		else
		{
			for(i=(ix+6);i<(ix+12);i++)				// new iodata
				z_trm_buf[j++] = pack_buf[i++];			
		}

		// j is the current byte count

	 	pack_buf[3] = (uint8)j;      	// byte count
		z_trm_buf[3] = (uint8)j;
		bc = compress_zero(j,200,1);
		gc_trm_buf[3] = (uint8)bc;      	// byte count
	}
	return bc;
}

//*******************************************
// Pack the group data to the car
//*******************************************

int16 pack_group_data_to_car(int16 dev)
{
	int16 bc;
	int16 j;
	int16 cbfl=0;
	int16 vipfl=0;

	z_trm_buf[0] = ':';      	// Start of Packet
	z_trm_buf[1] = (uint8)dev; 		// To address
	z_trm_buf[2] = 2;			// Packet number
	j=4;
	uiu.dw = g_up_hall_call[0][dev];
	z_trm_buf[j++] = uiu.b[lb];
	z_trm_buf[j++] = uiu.b[mlb];
	z_trm_buf[j++] = uiu.b[mhb];
	z_trm_buf[j++] = uiu.b[hb];
	uiu.dw = g_dn_hall_call[0][dev]; 
	z_trm_buf[j++] = uiu.b[lb];
	z_trm_buf[j++] = uiu.b[mlb];
	z_trm_buf[j++] = uiu.b[mhb];
	z_trm_buf[j++] = uiu.b[hb];
	if ((cons[Second_Riser] == 0) || (grtop_rfl != 0))
	{
		uiu.dw = g_upr_hall_call[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		uiu.dw = g_dnr_hall_call[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
	}
	if (cons[grtopf] > 32)
	{
		uiu.dw = g_up_hall_call[1][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		uiu.dw = g_dn_hall_call[1][dev]; 
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		if ((cons[Second_Riser] == 0) || (grtop_rfl != 0))
		{
			uiu.dw = g_upr_hall_call[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			uiu.dw = g_dnr_hall_call[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
		}
	}
	if (cons[Second_Riser] != 0)
	{
		uiu.dw = g_irup_hall_call[0][dev];
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		uiu.dw = g_irdn_hall_call[0][dev]; 
		z_trm_buf[j++] = uiu.b[lb];
		z_trm_buf[j++] = uiu.b[mlb];
		z_trm_buf[j++] = uiu.b[mhb];
		z_trm_buf[j++] = uiu.b[hb];
		if (grtop_rfl != 0)
		{
			uiu.dw = g_rirup_hall_call[0][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			uiu.dw = g_rirdn_hall_call[0][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
		}
		if (cons[grtopf] > 32)
		{
			uiu.dw = g_irup_hall_call[1][dev];
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			uiu.dw = g_irdn_hall_call[1][dev]; 
			z_trm_buf[j++] = uiu.b[lb];
			z_trm_buf[j++] = uiu.b[mlb];
			z_trm_buf[j++] = uiu.b[mhb];
			z_trm_buf[j++] = uiu.b[hb];
			if (grtop_rfl != 0)
			{
				uiu.dw = g_rirup_hall_call[1][dev];
				z_trm_buf[j++] = uiu.b[lb];
				z_trm_buf[j++] = uiu.b[mlb];
				z_trm_buf[j++] = uiu.b[mhb];
				z_trm_buf[j++] = uiu.b[hb];
				uiu.dw = g_rirdn_hall_call[1][dev];
				z_trm_buf[j++] = uiu.b[lb];
				z_trm_buf[j++] = uiu.b[mlb];
				z_trm_buf[j++] = uiu.b[mhb];
				z_trm_buf[j++] = uiu.b[hb];
			}
		}
	}

	z_trm_buf[j++] = (uint8)g_park[dev];
	z_trm_buf[j++] = (uint8)grp_empf[power_feeder[dev]][dev];
	z_trm_buf[j++] = (uint8)cons[carnmb];		// send group number to car

	if(full_update_req[dev] == 1)
	{
		full_update_req[dev] = 0;
		z_trm_buf[j++] = 1;					// request full update from car	 (set to request full update)
	}
	else
		z_trm_buf[j++] = 0;
	z_trm_buf[j++] = (uint8)ncu_door[dev];
	z_trm_buf[j++] = (uint8)lby_req[dev];
	z_trm_buf[j++] = (uint8)ncu_que[dev];
	if (g_cb_fl[dev] != 0)
		cbfl = g_cb_fl[dev];
	else if (g_rcb_fl[dev] != 0)
		cbfl = g_rcb_fl[dev] | 0x80;
	z_trm_buf[j++] = (uint8)cbfl;
	z_trm_buf[j++] = (uint8)((next_up & 0x01) | ((ir_active & 0x01) << 6) | ((in_grp_sabbath & 0x01) << 7));	 // Note: leave a bit for rear next up
	z_trm_buf[j++] = (uint8)grp_codebf[dev];
	z_trm_buf[j++] = (uint8)(((ids_ad_off[dev] & 0x01) << 3) | ((ids_heof[dev] & 0x01) << 2) | ((ids_rtl[dev] & 0x01) << 1) | (ids_ind[dev] & 0x01) |
					 ((ui_ad_off[dev] & 0x01) << 7) | ((ui_heof[dev] & 0x01) << 6) | ((ui_rtl[dev] & 0x01) << 5) | ((ui_ind[dev] & 0x01) << 4));
	z_trm_buf[j++] = (uint8)asvto_pos[dev];		// Auto service time-out position: group set pos of car when time-out occurs
	z_trm_buf[j++] = (uint8)car_asgn_sec;		// security flag to car
	if (cons[Second_Riser] != 0)
		z_trm_buf[j++] = (uint8)sr_cc_ctrl[dev];
	z_trm_buf[j++] = (uint8)(((any_hall_call != 0) & 0x01) | (((alt_lobby != 0) & 0x01) << 1) | 
				(((any_ir_hall_call != 0) & 0x01) << 2) | (((ir_car == dev) & 0x01) << 3) |
				(((tmr_svc[s_cc_lockouts] != 0) & 0x01) << 4) | (((tmr_svc[s_ccl_override] != 0) & 0x01) << 5) |
				(((tmr_svc[s_sabbath] != 0) & 0x01) << 6) | (((tmr_svc[s_cc_pb_sec] != 0) & 0x01) << 7));		// send spare byte
	if (cons[VipSvc] != 0)
	{
		if (g_vip_fl[dev] != 0)
		{
			vipfl = g_vip_fl[dev];
		}
		else if (g_rvip_fl[dev] != 0)
		{
			vipfl = g_rvip_fl[dev] | 0x80;
		}
		z_trm_buf[j++] = (uint8)vipfl;								// 34 send vip floor byte
	}
	if (cons[Smoke_2004] == 1)
	{
		z_trm_buf[j++] = (uint8)g_fire_ctrl;
	}
	g_ids_return = ids_return_flr;
	z_trm_buf[j++] = (uint8)(g_ids_return | (ids_return_typ & 0x01) << 6 | ((ids_allw_door[dev] & 0x01) << 7));
	
	z_trm_buf[j++] = (uint8)((sec_fl_ctrl[dev] & 0x07) | (((up_peak != 0) & 0x01) << 3) | ((grp_hc_fire_door_open & 0x01) << 4) | ((g_ids_fsx << 5)));

	// spare bytes	
	z_trm_buf[j++] = 0;
	z_trm_buf[j++] = 0;
		
	bc = compress_zero(j,(j*2)/3,1);
	gc_trm_buf[3] = (uint8)bc;      	// byte count
	return bc;
}
//*******************************************
// Pack data Packet for comm port
//*******************************************

void pack_grp_com(uint16 Packet,uint16 dev)
{
	int16 ix,i,j,k,bc;
	char unsigned ck_sum;

	ck_sum = 0;
	ix = 0;
	bc=0;
	switch (Packet)
	{
		case 0:			// Invalid packet
			break;
		case 1:      // Transmit car data to group
			bc = pack_car_data_to_group(dev);
			break;
		case 2:  	// Transmit group data to car
			bc = pack_group_data_to_car(dev);
			break;
		case 3:   // set a front car call on a slave car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 3;					// Packet number
			gc_trm_buf[3] = 5;					// byte count
			if (cc_req_fl[0] != 0)
			{
				gc_trm_buf[4] = cc_req_fl[0];			// car call floor
				cc_req_fl[0] = 0;
			}
			else if (cc_req_fl[1] != 0)
			{
				gc_trm_buf[4] = cc_req_fl[1];			// car call floor
				cc_req_fl[1] = 0;
			}
			else if (cc_req_fl[2] != 0)
			{
				gc_trm_buf[4] = cc_req_fl[2];			// car call floor
				cc_req_fl[2] = 0;
			}

			bc = 5;
			break;
		case 4:  // Set a rear car call on a slave car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 4;					// Packet number
			gc_trm_buf[3] = 5;					// packet byte count
			if (rcc_req_fl[0] != 0)
			{
				gc_trm_buf[4] = rcc_req_fl[0];		// car call floor
				rcc_req_fl[0] = 0;
			}
			else if (rcc_req_fl[1] != 0)
			{
				gc_trm_buf[4] = rcc_req_fl[1];		// car call floor
				rcc_req_fl[1] = 0;
			}
			else if (rcc_req_fl[2] != 0)
			{
				gc_trm_buf[4] = rcc_req_fl[2];		// car call floor
				rcc_req_fl[2] = 0;
			}
			bc = 5;
			break;
		case 5:	 // Add a Security Code
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 5;		// Packet number
			gc_trm_buf[3] = 9;		// packet byte count
			gc_trm_buf[4] = gc_Add_Sec_Fl_Req;
			gc_trm_buf[5] = gc_Add_Sec_Code[0];
			gc_trm_buf[6] = gc_Add_Sec_Code[1];
			gc_trm_buf[7] = gc_Add_Sec_Code[2];
			gc_trm_buf[8] = gc_Add_Sec_Code[3];
			bc = 9;
			break;
		case 6:	 // Delete a Security Code
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 6;		// Packet number
			gc_trm_buf[3] = 9;		// packet byte count
			gc_trm_buf[4] = gc_Del_Sec_Fl_Req;
			gc_trm_buf[5] = gc_Del_Sec_Code[0];
			gc_trm_buf[6] = gc_Del_Sec_Code[1];
			gc_trm_buf[7] = gc_Del_Sec_Code[2];
			gc_trm_buf[8] = gc_Del_Sec_Code[3];
			bc = 9;
			break;
		case 7:	 // Request a Security Code Table
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 7;		// Packet number
			gc_trm_buf[3] = 5;		// packet byte count
			gc_trm_buf[4] = gc_Sec_Fl_Tbl_Req[cons[carnmb]];
			bc = 5;
			break;
		case 8:			// Request Fault informatin from Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 8;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
		    for(i=0; i<max_flt_ix; i++)
		    {
				cf.Flts[i].code = 0;
				cf.Flts[i].count = 0;
		    }
			Car_Flt_ix = 0;
			break;
		case 9:			// Transmit error codes to group
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	 // From Address
			gc_trm_buf[2] = 9;		// Packet number
			gc_trm_buf[4] = Flt_ix;
			ix = 5;
			i = 0;

			while ((i < max_flt_ix) && (ix < 152))
			{
				if (f.Flts[flt_tx_ix].code != 0)
				{
					gc_trm_buf[ix++] = flt_tx_ix;
					for (j=0;j<(int)sizeof(Fault_type); j++)
						gc_trm_buf[ix++] = f.fltb[flt_tx_ix][j];
				}
				if (flt_tx_ix == Flt_ix)
					break;		// last one transmitted
				else
				{
					flt_tx_ix++;
					if (flt_tx_ix >= max_flt_ix)
						flt_tx_ix = 0;
					i++;
				}
			}

			if (flt_tx_ix != Flt_ix)
			{			 // Flt_ix will be the last one transmitted
				put_pkt_req(1,cons[carnmb],gc_port);	// Request a standard packet first
				put_pkt_req(9,cons[carnmb],gc_port);	// Send more fault data
			}	
			else
				put_pkt_req(36,cons[carnmb],gc_port);
			bc = ix;
			gc_trm_buf[3] = ix;	  // packet byte count
			break;

		case 10:		// Clear Fault information on Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 10;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
			break;
		case 11:			// Request Packet from device
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 11;		// Packet number
			gc_trm_buf[3] = 5;		// packet byte count
			gc_trm_buf[4] = get_pkt_11_req();		// request packet
			bc = 5;
			break;
		case 12:
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 12;		// Packet number
			gc_trm_buf[3] = 6;		// packet byte count
			gc_trm_buf[4] = version;		// version number
			gc_trm_buf[5] = revision;		//revision number
			bc = 6;
			break;
		case 13:			// Send Security Car Call Lockout data to car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 13;					// Packet number
			gc_trm_buf[3] = 21;					// packet byte count
			gc_trm_buf[4] = (uint16)((gc_rsec << 1) | (gc_sec & 0x01));				// car call lockout security input
			uiu.dw = gc_cc_sec[0];
			gc_trm_buf[5] = uiu.b[lb];
			gc_trm_buf[6] = uiu.b[mlb];
			gc_trm_buf[7] = uiu.b[mhb];
			gc_trm_buf[8] = uiu.b[hb];
			uiu.dw = gc_rcc_sec[0];
			gc_trm_buf[9] = uiu.b[lb];
			gc_trm_buf[10] = uiu.b[mlb];
			gc_trm_buf[11] = uiu.b[mhb];
			gc_trm_buf[12] = uiu.b[hb];
			uiu.dw = ids_cc_sec[0][dev];
			gc_trm_buf[13] = uiu.b[lb];
			gc_trm_buf[14] = uiu.b[mlb];
			gc_trm_buf[15] = uiu.b[mhb];
			gc_trm_buf[16] = uiu.b[hb];
			uiu.dw = ids_rcc_sec[0][dev];
			gc_trm_buf[17] = uiu.b[lb];
			gc_trm_buf[18] = uiu.b[mlb];
			gc_trm_buf[19] = uiu.b[mhb];
			gc_trm_buf[20] = uiu.b[hb];
			bc = 21;
#if (Tract_HR == 1)
			gc_trm_buf[3] = 37;					// packet byte count
			uiu.dw = gc_cc_sec[1];
			gc_trm_buf[21] = uiu.b[lb];
			gc_trm_buf[22] = uiu.b[mlb];
			gc_trm_buf[23] = uiu.b[mhb];
			gc_trm_buf[24] = uiu.b[hb];
			uiu.dw = gc_rcc_sec[1];
			gc_trm_buf[25] = uiu.b[lb];
			gc_trm_buf[26] = uiu.b[mlb];
			gc_trm_buf[27] = uiu.b[mhb];
			gc_trm_buf[28] = uiu.b[hb];
			uiu.dw = ids_cc_sec[1][dev];
			gc_trm_buf[29] = uiu.b[lb];
			gc_trm_buf[30] = uiu.b[mlb];
			gc_trm_buf[31] = uiu.b[mhb];
			gc_trm_buf[32] = uiu.b[hb];
			uiu.dw = ids_rcc_sec[1][dev];
			gc_trm_buf[33] = uiu.b[lb];
			gc_trm_buf[34] = uiu.b[mlb];
			gc_trm_buf[35] = uiu.b[mhb];
			gc_trm_buf[36] = uiu.b[hb];
			bc = 37;
#endif		
			break;
		case 14:
			break;
		case 15:			// Send Security Lockout Override data to car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 15;					// Packet number
			gc_trm_buf[3] = 21;					// packet byte count
			gc_trm_buf[4] = (uint16)((gc_rsec_ovr << 1) | (gc_sec_ovr & 0x01));				// car call lockout security input
			uiu.dw = gc_cc_sec_ovr_set[0];
			gc_trm_buf[5] = uiu.b[lb];
			gc_trm_buf[6] = uiu.b[mlb];
			gc_trm_buf[7] = uiu.b[mhb];
			gc_trm_buf[8] = uiu.b[hb];
			uiu.dw = gc_rcc_sec_ovr_set[0];
			gc_trm_buf[9] = uiu.b[lb];
			gc_trm_buf[10] = uiu.b[mlb];
			gc_trm_buf[11] = uiu.b[mhb];
			gc_trm_buf[12] = uiu.b[hb];
			uiu.dw = gc_cc_sec_ovr_panic[0];
			gc_trm_buf[13] = uiu.b[lb];
			gc_trm_buf[14] = uiu.b[mlb];
			gc_trm_buf[15] = uiu.b[mhb];
			gc_trm_buf[16] = uiu.b[hb];
			uiu.dw = gc_rcc_sec_ovr_panic[0];
			gc_trm_buf[17] = uiu.b[lb];
			gc_trm_buf[18] = uiu.b[mlb];
			gc_trm_buf[19] = uiu.b[mhb];
			gc_trm_buf[20] = uiu.b[hb];
			bc = 21;
#if (Tract_HR == 1)
			gc_trm_buf[3] = 37;					// packet byte count
			uiu.dw = gc_cc_sec_ovr_set[1];
			gc_trm_buf[21] = uiu.b[lb];
			gc_trm_buf[22] = uiu.b[mlb];
			gc_trm_buf[23] = uiu.b[mhb];
			gc_trm_buf[24] = uiu.b[hb];
			uiu.dw = gc_rcc_sec_ovr_set[1];
			gc_trm_buf[25] = uiu.b[lb];
			gc_trm_buf[26] = uiu.b[mlb];
			gc_trm_buf[27] = uiu.b[mhb];
			gc_trm_buf[28] = uiu.b[hb];
			uiu.dw = gc_cc_sec_ovr_panic[1];
			gc_trm_buf[29] = uiu.b[lb];
			gc_trm_buf[30] = uiu.b[mlb];
			gc_trm_buf[31] = uiu.b[mhb];
			gc_trm_buf[32] = uiu.b[hb];
			uiu.dw = gc_rcc_sec_ovr_panic[1];
			gc_trm_buf[33] = uiu.b[lb];
			gc_trm_buf[34] = uiu.b[mlb];
			gc_trm_buf[35] = uiu.b[mhb];
			gc_trm_buf[36] = uiu.b[hb];
			bc = 37;
#endif		
			break;
		case 16:
			break;
		case 20:		// Request car call lockout security information
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 20;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
			break;
		case 22:			// Send gpi data to car
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 22;					// Packet number
			gc_trm_buf[4] = gpi[0];
			gc_trm_buf[5] = gpi[1];
			gc_trm_buf[6] = gpi[2];
			gc_trm_buf[7] = gpi[3];
			gc_trm_buf[8] = gpi[4];
			gc_trm_buf[9] = gpi[5];
			gc_trm_buf[10] = gpi[6];
			gc_trm_buf[11] = gpi[7];
			gc_trm_buf[12] = gpi[8];
			gc_trm_buf[13] = gpi[9];
			gc_trm_buf[14] = gpi[10];
			gc_trm_buf[15] = gpi[11];
#if (Tract_HR == 1)
			gc_trm_buf[15] = gpi[12];
			gc_trm_buf[16] = gpi[13];
			gc_trm_buf[17] = gpi[14];
			gc_trm_buf[18] = gpi[15];
			gc_trm_buf[19] = gpi[16];
			gc_trm_buf[20] = gpi[17];
			gc_trm_buf[21] = gpi[18];
			gc_trm_buf[22] = gpi[19];
			gc_trm_buf[23] = gpi[20];
			gc_trm_buf[24] = gpi[21];
			gc_trm_buf[25] = gpi[22];
			gc_trm_buf[26] = gpi[23];
			gc_trm_buf[27] = gpi[24];
			gc_trm_buf[28] = gpi[25];
			gc_trm_buf[29] = gpi[26];
			gc_trm_buf[3] = 31;					// packet byte count
			bc = 31;
#else
			gc_trm_buf[3] = 16;					// packet byte count
			bc = 16;
#endif
			break;
		case 23:			// Send gpo data to group
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 22;					// Packet number
			gc_trm_buf[4] = cgpo[0];
			gc_trm_buf[5] = cgpo[1];
			gc_trm_buf[6] = cgpo[2];
			gc_trm_buf[7] = cgpo[3];
			gc_trm_buf[8] = cgpo[4];
			gc_trm_buf[9] = cgpo[5];
			gc_trm_buf[10] = cgpo[6];
			gc_trm_buf[11] = cgpo[7];
			gc_trm_buf[12] = cgpo[8];
			gc_trm_buf[13] = cgpo[9];
			gc_trm_buf[14] = cgpo[10];
			gc_trm_buf[15] = cgpo[11];
#if (Tract_HR == 1)
			gc_trm_buf[15] = cgpo[12];
			gc_trm_buf[16] = cgpo[13];
			gc_trm_buf[17] = cgpo[14];
			gc_trm_buf[18] = cgpo[15];
			gc_trm_buf[19] = cgpo[16];
			gc_trm_buf[20] = cgpo[17];
			gc_trm_buf[21] = cgpo[18];
			gc_trm_buf[22] = cgpo[19];
			gc_trm_buf[23] = cgpo[20];
			gc_trm_buf[24] = cgpo[21];
			gc_trm_buf[25] = cgpo[22];
			gc_trm_buf[26] = cgpo[23];
			gc_trm_buf[27] = cgpo[24];
			gc_trm_buf[28] = cgpo[25];
			gc_trm_buf[29] = cgpo[26];
			gc_trm_buf[3] = 31;					// packet byte count
			bc = 31;
#else
			gc_trm_buf[3] = 16;					// packet byte count
			bc = 16;
#endif
			break;
		case 24:			// Request CGPO (gpo) data from Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;	// To address
			gc_trm_buf[2] = 24;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
			break;
		case 25:			// PI lantern packet for simple hydro
			break;	
		case 26:			// Send and request version and revision
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 26;		// Packet number
			gc_trm_buf[3] = 6;		// packet byte count
			gc_trm_buf[4] = version;	// version number
			gc_trm_buf[5] = revision;
			bc = 6;
			break;


		case 31:		// Send Security Code Checksum and Number of Codes for floors 1-60 (only send 30 at a time)
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 31;					// Packet number
			gc_trm_buf[4] = gc_Sec_Chksum_Fl;
			if (gc_Sec_Chksum_Fl <= 0)
				gc_Sec_Chksum_Fl = 1;
			if (cons[grtopf] > 30)
			{
				if ((gc_Sec_Chksum_Fl > 30) && (gc_Sec_Chksum_Fl <= cons[grtopf]))
					k = cons[grtopf];
				else
				{
					gc_Sec_Chksum_Fl = 1;
					k = 30;
				}
			}
			else
			{
				gc_Sec_Chksum_Fl = 1;
				k = cons[grtopf];
			}

			j = 0;
			for (i = gc_Sec_Chksum_Fl;i <= k;i++)
			{
				uwu.w = Code_Tbl_Chk[i].NumCode;
				gc_trm_buf[5 + j] = uwu.b[lbw];
				gc_trm_buf[6 + j] = uwu.b[hbw];
				uwu.w = Code_Tbl_Chk[i].CheckSum;
				gc_trm_buf[7 + j] = uwu.b[lbw];
				gc_trm_buf[8 + j] = uwu.b[hbw];
				j+=4;
			}
			bc = j + 5;		// packet byte count
			gc_trm_buf[3] = bc;		// packet byte count
			break;

		case 32:		// Send Codes to Car for one floor
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 32;					// Packet number
			gc_trm_buf[4] = gc_Sec_Fl_Tbl_Req[dev];
			gc_trm_buf[5] = gc_Sec_Code_Index;
			gc_trm_buf[6] = gc_CodeTable.NumCode;

			if (gc_Sec_Code_Index < 0)
				gc_Sec_Code_Index = 1;

			if (gc_CodeTable.NumCode < 50)
				k = gc_CodeTable.NumCode;
			else
				k = 50;

			j = 0;
			i = 0;
			while (i < k)
			{
				gc_trm_buf[7 + j] = gc_CodeTable.Codes[gc_Sec_Code_Index+i][0];
				gc_trm_buf[8 + j] = gc_CodeTable.Codes[gc_Sec_Code_Index+i][1];
				gc_trm_buf[9 + j] = gc_CodeTable.Codes[gc_Sec_Code_Index+i][2];
				gc_trm_buf[10 + j] = gc_CodeTable.Codes[gc_Sec_Code_Index+i][3];
				i++;
				j+=4;
			}
			if ((gc_Sec_Code_Index + i) >= gc_CodeTable.NumCode)
				gc_Code_Xfer_Comp = 1;
			gc_Sec_Code_Index += k;
			bc = j + 7;		// packet byte count
			gc_trm_buf[3] = bc;		// packet byte count
			break;

		case 33:		// Send Command to delete all security codes for one or all floors
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 33;					// Packet number
			gc_trm_buf[3] = 5;		// packet byte count
			gc_trm_buf[4] = gc_Del_All_Codes_Fl_Req;		// fl_req == 255 for all floors
			bc = 5;		// packet byte count
			break;
			

		case 35:		// Send group voltage inputs
			gc_trm_buf[0] = ':';				// Start of packet
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 35;					// Packet number
			gc_trm_buf[3] = 13;		// packet byte count
			uwu.w = L1_Voltage;
			gc_trm_buf[4] = uwu.b[lbw];
			gc_trm_buf[5] = uwu.b[hbw];
			uwu.w = L2_Voltage;
			gc_trm_buf[6] = uwu.b[lbw];
			gc_trm_buf[7] = uwu.b[hbw];
			uwu.w = L3_Voltage;
			gc_trm_buf[8] = uwu.b[lbw];
			gc_trm_buf[9] = uwu.b[hbw];
			uwu.w = Door_Voltage;
			gc_trm_buf[10] = uwu.b[lbw];
			gc_trm_buf[11] = uwu.b[hbw];
			gc_trm_buf[12] = vs_can_online;
			bc = 13;		// packet byte count
			break;

		case 36:		// Send confirmation that all faults have been sent
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 36;		// Packet number
			gc_trm_buf[3] = 4;		// packet byte count
			bc = 4;
			break;
			
		case 37:		//  Send Car  floor lockout  info to other Car
			gc_trm_buf[0] = ':';
			gc_trm_buf[1] = dev;
			gc_trm_buf[2] = 37;		// Packet number
			gc_trm_buf[3] = 12;		// packet byte count
			uiu.dw = ui_cc_sec[0][dev];
			gc_trm_buf[4] = uiu.b[lb];
			gc_trm_buf[5] = uiu.b[mlb];
			gc_trm_buf[6] = uiu.b[mhb];
			gc_trm_buf[7] = uiu.b[hb];
		   	uiu.dw = ui_cc_sec[1][dev]; 
			gc_trm_buf[8] =  uiu.b[lb];	   
			gc_trm_buf[9] =  uiu.b[mlb];	   
			gc_trm_buf[10] = uiu.b[mhb];	   
			gc_trm_buf[11] = uiu.b[hb];	   
			bc = 12;			
			break;			

		default:
			break;
	}
	for (i=1; i < bc; i++)
		ck_sum += (uint8)gc_trm_buf[i];

	gc_trm_buf[i++] = (uint8)((0xFF - ck_sum) + 1); 			      // Set checksum in buffer
	gc_trm_buf[i++] = '@';						// packet end
	gc_trm_buf[i++] = 0xF0;						// Dummy byte
	gc_trm_buf[i] = 0xFF;						// Dummy byte
	gc_trm_buf[0] |= 0x4000;					// set ninth bit
	gc_trm_buf[1] |= 0x4000;					// set ninth bit
}

// ***************************************************************
// ******** Compress Zero's in comm  *****************************
// ***************************************************************

int16 compress_zero(int16 buf_len, int16 max_len,int16 compress_flag)
{
	// Data is in z_trm_buf[] and is checked for consecutive zeros.
	// For one zero you get two bytes of 0 followed by 1 and for n zeros
	// you get 0 followed by the number of zeros (including the first one).
	// You can never have a zero followed by zero. Any time there are more than
	// two consecutive zero's, you save at least 1 byte.  If the buffer is greater
	// than the max_len, then the compression is discarded and the full buffer 
	// is packed including the compress flag == 0. If the copression worked, the compress
	// flag is set to 1.  The packet length is returned. 

	int16 j=0;
	int16 i=0;
	int16 zero_count=0;


	j = 0;
	for(i=0;i<=3;i++)
	{
		gc_trm_buf[j++] = z_trm_buf[i];	  // copy over the first four bytes (must be sent all the time
	}
	j=5;								// skip the compress flag
	zero_count = 0;
	for (i=4;i<buf_len;i++)
	{
		if (z_trm_buf[i] == 0)
		{
			zero_count++;
			if (i == (buf_len-1))
			{	 // last byte is a zero
				gc_trm_buf[j++] = 0;
				gc_trm_buf[j++] = zero_count;
				zero_count = 0;
			}
		}
		else 
		{
			if (zero_count != 0)
			{
				gc_trm_buf[j++] = 0;
				gc_trm_buf[j++] = zero_count;
				zero_count = 0;
			}
			gc_trm_buf[j++] = z_trm_buf[i];
		}
	}
	if ((j < max_len) && (compress_flag == 1))
	{
		gc_trm_buf[4] = 4;
		return j;
	}
	else 
	{
		gc_trm_buf[4] = 8;	// compress flag
		j = 5;					// skip the compress flag
		for (i=4;i<buf_len;i++)
		{				  // repack the buffer with the compress flag in it
			gc_trm_buf[j++] = z_trm_buf[i];	  // copy over the first four bytes (must be sent all the time
		}
		return j;		 
	}
}

// ***************************************************************
// ********** Compress the car to group comm buffer **************
// ***************************************************************

int16 compress(int16 buf_len, int16 max_len)
{
	// Data is in pack_buf[] and is compared with hold_buf[], if the 
	// data is different send the buffer index, number of consecutive
	// bytes that changed, and then the bytes that changed.  Do this
	// multiple times until at the end of the buffer or until the size of
	// buffer is greater then max_len. If greater than max_len return 0 else
	// return byte count to show that buffer is compressed and ready to transmit
	
	int16 ix_flag, count, ix_t, i;

	if(pack_buf[4] == 0)
	{		  // compress the buffer with update information only

		for(i=0;i<=4;i++)
		{
			gc_trm_buf[i] = pack_buf[i];	  // copy over the first four bytes (must be sent all the time
			hold_buf[i] = pack_buf[i];
		}
		ix_flag = 0;
		count = 0;
		ix_t = i;									// i is where we need to start
		while ((i<=buf_len) && (ix_t <= max_len))
		{
			if (pack_buf[i] != hold_buf[i])
			{
				if (ix_flag == 0)
				{		// need to set the index
					ix_flag = 1;
#if (Tract_HR == 1)
					if (i >= 256)
					{	// since index cannot be a 1, us == 1 for 2 byte value of 256 + index
						gc_trm_buf[ix_t++] = 1;
						gc_trm_buf[ix_t++] = (uint16)(i-256);
					}
					else
#endif
						gc_trm_buf[ix_t++] = i;		// store index into buffer
					count = 0;
					gc_trm_buf[ix_t] = count;		// start with no consecutive bytes that have changed
					gc_trm_buf[ix_t+count+1] = pack_buf[i];	// store data byte
					hold_buf[ix_t+count+1] = pack_buf[i];		// update the hold buffer
				}
				else
				{
					count++;
					gc_trm_buf[ix_t] = count;				// update the count
					gc_trm_buf[ix_t+count+1] = pack_buf[i];	// store next byte
					hold_buf[ix_t+count+1] = pack_buf[i];		// update the hold buffer
				}
			}
			else
			{
				if (ix_flag == 1)
				{	// have to increment index by count amount + 1
					ix_flag = 0;
					ix_t = ix_t + count + 2;
				}
			}
			i++;					// next buffer location				
		}

		if (ix_t >= max_len)
			return 0;		// compression not that good 
		else
		{
			gc_trm_buf[3] = ix_t;			// update byte count
			hold_buf[3] = (uint8)ix_t;	
			return ix_t;					// compressed
		}
	}
	else
		return 0;		// full update - no compression

}

// ***********************************************************
// Get the packet number to be sent out with packet 11
// ***********************************************************

int16 get_pkt_11_req (void)
{
	int16 i = 0;
	int16 pkt_11_req = 0;
	while (i<5)
	{
		if (gc_pkt_11_req_pkt[pkt_11_gix] != 0)
		{
			pkt_11_req = gc_pkt_11_req_pkt[pkt_11_gix];
			gc_pkt_11_req_pkt[pkt_11_gix] = 0;
			return (pkt_11_req);
		}
		else
		{
			pkt_11_gix++;
			if (pkt_11_gix >= 5)
				pkt_11_gix = 0;
		}
		i++;
	}
	return (pkt_11_req);
}

 


// ***********************************************************
// Put the packet number in the queue for a packet 11 request
// ***********************************************************

void put_pkt_11_req (int16 pkt_nmb)
{
	int16 i = 0;
	while (i<5)
	{
		if (gc_pkt_11_req_pkt[pkt_11_pix] == 0)
		{
			gc_pkt_11_req_pkt[pkt_11_pix] = pkt_nmb;
			break;
		}
		else
		{
			pkt_11_pix ++;
			if (pkt_11_pix >= 5)
				pkt_11_pix = 0;
		}
		i++;
	}
}


//*************************************************
// Request the Line and Door voltage from each car
//*************************************************

void Req_Car_Voltage (void)
{
	static int16 second_cnt;
	static int16 volt_car;

	if (cons[nmbcars] > 1)
	{		 // only send Code Check if there are cars to send it to
		if (timers[tsec] != 0) 
		{
			if ((second_cnt & 0x01) == 0)
				second_cnt ++;
		}
		else
		{
			if ((second_cnt & 0x01) != 0)
				second_cnt ++;
		}

		if (second_cnt >= 5)
		{
			second_cnt = 0;
			volt_car++;
			if ((volt_car > cons[nmbcars]) || (volt_car <= 1))
				volt_car = 2;

			if (volt_car != cons[carnmb])
			{
				put_pkt_11_req(35);
				put_pkt_req(11,volt_car,gc_port);
			}
		}

	}
}

//********************************
// Send Car Call Codes to each car
//********************************

void Send_CC_Codes(void)
{
	static int16 second_cnt;
	static int16 prev_sec_cnt;
	static int16 cc_car;
	int16 i;

	if (cons[nmbcars] > 1)
	{		 // only send Code Check if there are cars to send it to
		if (timers[tsec] != 0) 
		{
			if ((second_cnt & 0x01) == 0)
				second_cnt ++;
		}
		else
		{
			if ((second_cnt & 0x01) != 0)
				second_cnt ++;
		}

		if (second_cnt >= 10)
		{
			second_cnt = 0;
			if (cons[grtopf] > 30)
			{
				if (gc_Sec_Chksum_Fl == 1)
					gc_Sec_Chksum_Fl = 31;
				else
					gc_Sec_Chksum_Fl = 1;
			}
			else
				gc_Sec_Chksum_Fl = 1;

			if (gc_Sec_Chksum_Fl == 1)
				cc_car++;

			if (cc_car > 8)					// each car receives the packet every 80 seconds
				cc_car = cons[carnmb]+1;

			if (cc_car <= cons[nmbcars])
			{
				if (cc_car != cons[carnmb])
				{	   // Send Code Checksum for each car to verify
					put_pkt_req(31,cc_car,gc_port);
				}
			}
		}
		if (((second_cnt & 0x01) != 0) && (prev_sec_cnt != second_cnt))
		{
			if (gc_Sec_Req_Car == 0)
			{
				for (i=cons[carnmb]+1;i<=cons[nmbcars];i++)
				{
					if ((gc_Sec_Fl_Tbl_Req[i] > 0) && (gc_Sec_Fl_Tbl_Req[i] <= cons[grtopf]))
					{
						gc_Sec_Req_Car = i;
						gc_Sec_Req_Seq = 1;
						break;
					}
				} 
			}
			else if ((gc_Sec_Req_Car > cons[carnmb]) && (gc_Sec_Req_Car <= cons[nmbcars]))
			{
				if (gc_dev_online[gc_Sec_Req_Car] == 1)
				{
					if (gc_Sec_Req_Seq == 1)
					{
						gc_Code_Xfer_Comp = 0;
						gc_Sec_Code_Index = 0;
						Read_CodeTbl(gc_Sec_Fl_Tbl_Req[gc_Sec_Req_Car], &gc_CodeTable, gc_LookUpTable);			
						put_pkt_req(32,gc_Sec_Req_Car,gc_port);
						gc_Sec_Req_Seq = 2;
					}
					else if (gc_Sec_Req_Seq == 2)
					{
						if (gc_Code_Xfer_Comp == 1)
						{
							gc_Sec_Fl_Tbl_Req[gc_Sec_Req_Car] = 0;
							gc_Sec_Req_Car = 0;
							gc_Sec_Req_Seq = 0;
						}
						else
						{		// send next packet
							second_cnt = 0;
							put_pkt_req(32,gc_Sec_Req_Car,gc_port);
						}
					}
				}
				else
				{
					gc_Sec_Fl_Tbl_Req[gc_Sec_Req_Car] = 0;
					gc_Sec_Req_Car = 0;
					gc_Sec_Req_Seq = 0;
				}
			}
			else
			{
				gc_Sec_Req_Car = 0;
				gc_Sec_Req_Seq = 0;
			}  
		}
		prev_sec_cnt = second_cnt;
	}
}

// ------------------------------------------------------------------------
// Send Hall call driver request
// ------------------------------------------------------------------------

void send_hc_dvr_req (int16 cmd)
{
}


//*********************************************************
//**** Uncompress zero's from the receive buffer  *********
//*********************************************************

int16 uncompress_zero(int16 ix)			// return uncompressed length
{
	int16 len, i,j,k;
	int16 zero_flag = 0;
	int16 zero_count = 0;

	len = (uint8)gc_rcv_buf[ix+3];

	j = 0;
	zero_flag = 0;
	zero_count = 0;
	for(i=0;i<=3;i++)
	{
		z_rcv_buf[j++] = (uint8)gc_rcv_buf[ix+i];
	}
	if ((uint8)gc_rcv_buf[ix+i] == 4)
	{								// zero's are compressed
		i++;						// skip the compress flag
		while(i < len)		// i starts at 5
		{
			if ((uint8)gc_rcv_buf[ix+i] == 0)
				zero_flag = 1;
			else
			{
				if (zero_flag == 1)
				{
					zero_count = (uint8)gc_rcv_buf[ix+i];
					for(k=0;k<zero_count;k++)
						z_rcv_buf[j++] = 0;
					zero_flag = 0;
				}
				else
					z_rcv_buf[j++] =  (uint8)gc_rcv_buf[ix+i];
			}
			i++;
		}
	}
	else
	{
		i++;				// skip the compress flag			
		while(i < len)		// i starts at 5
		{
			z_rcv_buf[j++] =  (uint8)gc_rcv_buf[ix+i];
			i++;
		}
	}
	return (j);
}
						


//*********************************************
//**** Uncompress the receive buffer  *********
//*********************************************

void uncompress(int16 dev, int16 ix)				
{
	int16 len, count, ix_r,i,j;
#if (Tract_HR == 1)
	int16 k;
#endif

	len = (uint8)gc_rcv_buf[ix+3];
	if ((uint8)gc_rcv_buf[ix+4] == 0)
	{	// Data is compressed

		for(i=0;i<=4;i++)
		{
			unpack_buf[dev-1][i] = (uint8)gc_rcv_buf[ix+i];
		}
		while(i < len)		// i starts at 5
		{
			ix_r = (uint8)gc_rcv_buf[ix+i];	   // get the index
#if (Tract_HR == 1)
			if (ix_r == 1)
			{		  // since 1 is not a valid index number use it to signal values greater than 255
				i++;
				ix_r = (uint16)((uint8)gc_rcv_buf[ix+i] + 256);
			}
#endif
			i++;
			count = (uint8)gc_rcv_buf[ix+i];		// get the count
			i++;
			for(j=0;j<=count;j++)
			{
				unpack_buf[dev-1][ix_r++] = (uint8)gc_rcv_buf[ix+i];		// store the data in the unpack buffer
				i++; 
			}
		}
	}
#if (Tract_HR == 1)
	else
	{
		uncompress_zero(ix);

		if (z_rcv_buf[4] == 1)
		{		// full packet update type 1
			i = 0;
			for(j=0;j<=(g_cancel_dn+5);j++)
			{										// 9 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			for(j=(g_eta_2d+5);j<=(g_eta_31d+5);j++)
			{										// 30 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			for(j=(g_eta_1u+5);j<=(g_eta_29u+5);j++)
			{										//  29 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			for(j=(g_procf+5);j<=(g_cur_flt+26);j++)
			{										// 34 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			if (cons[Second_Riser] != 0)
			{
				unpack_buf[dev-1][j++] = z_rcv_buf[i++];
				unpack_buf[dev-1][j++] = z_rcv_buf[i++];
				unpack_buf[dev-1][j++] = z_rcv_buf[i++];
				unpack_buf[dev-1][j++] = z_rcv_buf[i++];
			}
			k = j;
			for(j=k;j<(k+20);j++)
			{									 // 20 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			k = j;
			for(j=k;j<(k+72);j++)				 // 72 bytes
			{
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			k = j;
			for(j=(k+144);j<(k+150);j++)	  // 6 bytes
			{
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			if (len > i)						// 6 new bytes
			{
				k = j;
				for(j=k;j<(k+6);j++)	  // 6 bytes
				{
					unpack_buf[dev-1][j] = z_rcv_buf[i];
					i++;
				}
			}
			if (len > i)						// 6 new bytes (iodata)
			{
				k = j;
				for(j=k;j<(k+6);j++)	  // 6 bytes
				{
					unpack_buf[dev-1][j] = z_rcv_buf[i];
					i++;
				}
			}
		}
		else if (z_rcv_buf[4] == 2)
		{		// full packet update type 2
			i = 0;
			for(j=0;j<=(g_cancel_dn+5);j++)
			{										// 9 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			for(j=(g_eta_32d+5);j<=(g_eta_61d+5);j++)
			{										// 29 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			for(j=(g_eta_30u+5);j<=(g_eta_60u+5);j++)
			{										//  30 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			for(j=(g_procf+5);j<=(g_cur_flt+26);j++)
			{										// 34 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			if (cons[Second_Riser] != 0)
			{
				unpack_buf[dev-1][j++] = z_rcv_buf[i++];
				unpack_buf[dev-1][j++] = z_rcv_buf[i++];
				unpack_buf[dev-1][j++] = z_rcv_buf[i++];
				unpack_buf[dev-1][j++] = z_rcv_buf[i++];
			}
			k = j;
			for(j=k;j<(k+20);j++)
			{									 // 20 bytes
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			k = j;
			for(j=(k+72);j<(k+144);j++)	  // 72 bytes
			{
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}
			
			k = j;
			for(j=k;j<(k+6);j++)	  // 6 bytes
			{	//       ^ changed from 150 to 6		
				unpack_buf[dev-1][j] = z_rcv_buf[i];
				i++;
			}

			if (len > i)						// 6 new bytes
			{
				k = j;
				for(j=k;j<(k+6);j++)	  // 6 bytes
				{
					unpack_buf[dev-1][j] = z_rcv_buf[i];
					i++;
				}
			}
			if (len > i)						// another 6 new bytes (iodata)
			{
				k = j;
				for(j=k;j<(k+6);j++)	  // 6 bytes
				{
					unpack_buf[dev-1][j] = z_rcv_buf[i];
					i++;
				}
			}
		}		
	}
#else
	else
	{		// full packet update
		for(i=0;i<=len;i++)
		{
			unpack_buf[dev-1][i] = (uint8)gc_rcv_buf[ix+i];
		}
	}
#endif
}
//*********************************
// Unpack the car data to the group
//*********************************

void unpack_car_data_to_group(int16 dev, int16 ix)
{
	int16 i,j;

	if ((dev > 0) && (dev <= cons[nmbcars]))
	{
		uncompress(dev,ix);
		if (unpack_buf[dev-1][ix+4] == 1)
			full_update_req[dev] = 0;		// update request has been met for this car
		for(i=0; i<=g_cur_flt; i++)
		{
			Group_IO_Com[dev][i] = unpack_buf[dev-1][i+ix+5];
		}
	  	uwu.b[lbw] = unpack_buf[dev-1][ix+142];
	  	uwu.b[hbw] = unpack_buf[dev-1][ix+143];
		Group_IO_Com[dev][g_statusf1_l] = uwu.w;
	  	g_enc_dir[dev] = unpack_buf[dev-1][ix+144];
		uwu.b[lbw] = unpack_buf[dev-1][ix+145];
		uwu.b[hbw] = unpack_buf[dev-1][ix+146];
	  	g_dmd_vel[dev] = uwu.w;
		uwu.b[lbw] = unpack_buf[dev-1][ix+147];
		uwu.b[hbw] = unpack_buf[dev-1][ix+148];
	  	g_enc_vel[dev] = uwu.w;
		uiu.b[lb] = unpack_buf[dev-1][ix+149];
		uiu.b[mlb] = unpack_buf[dev-1][ix+150];
		uiu.b[mhb] = unpack_buf[dev-1][ix+151];
		uiu.b[hb] = unpack_buf[dev-1][ix+152];
	  	g_flr_cnt[dev] = uiu.dw;
		uiu.b[lb] = unpack_buf[dev-1][ix+153];
		uiu.b[mlb] = unpack_buf[dev-1][ix+154];
		uiu.b[mhb] = unpack_buf[dev-1][ix+155];
		uiu.b[hb] = unpack_buf[dev-1][ix+156];
	  	g_pls_cnt[dev] = uiu.dw;
		uwu.b[lbw] = unpack_buf[dev-1][ix+157];
		uwu.b[hbw] = unpack_buf[dev-1][ix+158];
	  	Group_IO_Com[dev][g_statusf1_h] = uwu.w;
		uwu.b[lbw] = unpack_buf[dev-1][ix+159];
		uwu.b[hbw] = unpack_buf[dev-1][ix+160];
	  	Group_IO_Com[dev][g_control] = uwu.w;
		uwu.b[lbw] = unpack_buf[dev-1][ix+161];
		uwu.b[hbw] = unpack_buf[dev-1][ix+162];
	  	Group_IO_Com[dev][g_ss_stat] = uwu.w;
		j = 163;
		if (cons[Second_Riser] != 0)
		{
			Group_IO_Com[dev][g_cancel_irup] = unpack_buf[dev-1][ix+j];
			Group_IO_Com[dev][g_cancel_irdn] = unpack_buf[dev-1][ix+j+1];
			Group_IO_Com[dev][g_cancel_rirup] = unpack_buf[dev-1][ix+j+2];
			Group_IO_Com[dev][g_cancel_rirdn] = unpack_buf[dev-1][ix+j+3];
			j+=4;
		}
		if ((cons[VipSvc] != 0) || (cons[SecFlCfg] == 4))
		{
			Group_IO_Com[dev][g_vipf] = unpack_buf[dev-1][ix+j];
			Group_IO_Com[dev][g_nmbcc] = unpack_buf[dev-1][ix+j+1];
			Group_IO_Com[dev][g_topcc] = unpack_buf[dev-1][ix+j+2];
			Group_IO_Com[dev][g_botcc] = unpack_buf[dev-1][ix+j+3];
			j+=4;
		}

		for(i=0; i<=144; i++)    // Note iodata index now uses the elevator number
			iodata[dev][i] = unpack_buf[dev-1][i+ix+j];

		j+=145;
		car_has_fault[dev] = unpack_buf[dev-1][ix+j];		// fault byte
		uwu.w = Group_IO_Com[dev][g_cur_flt];
		uwu.b[hbw] = unpack_buf[dev-1][ix+j+1];					// current fault high byte
		Group_IO_Com[dev][g_cur_flt] = uwu.w;

		Group_IO_Com[dev][g_fire_status] = unpack_buf[dev-1][ix+j+2];		// fire status from car
		Group_IO_Com[dev][g_ins_status] = unpack_buf[dev-1][ix+j+3];		// Inspection Status

		uwu.b[lbw] = unpack_buf[dev-1][ix+j+4];		// statusf2
		uwu.b[hbw] = unpack_buf[dev-1][ix+j+5];		// statusf2
		Group_IO_Com[dev][g_statusf2_l] = uwu.w;

		uwu.b[lbw] = unpack_buf[dev-1][ix+j+6];		// statusf2
		uwu.b[hbw] = unpack_buf[dev-1][ix+j+7];		// statusf2
		Group_IO_Com[dev][g_statusf2_h] = uwu.w;

		Group_IO_Com[dev][g_pwr_status] = unpack_buf[dev-1][ix+j+8];	   		// power status

		group_alrm[dev] = unpack_buf[dev-1][ix+j+9];							//car calarm status

		// unpack_buf[dev-1][ix+j+10];			// spare byte
		// unpack_buf[dev-1][ix+j+11];			// spare byte

		j+=12;
		for(i=0; i<=11; i++)    // Note iodata index now uses the elevator number
			iodata[dev][i+145] = unpack_buf[dev-1][i+ix+j];

		Car_Not_Talking[dev] = 0;
		timers[tgcom_car+dev] = 0;
	}
}


//**************************************************
// Unpack the group data to the car
//**************************************************

void unpack_group_data_to_car(int16 dev, int16 ix, int16 bc)
{
	int16 j;
	int16 cbfl;
	int16 vipfl;
	int16 spare_byte;
	int16 uc_len;

	j = bc;		// to get rid of warning

	uc_len = uncompress_zero(ix);

	j = ix + 4;

	uiu.b[lb] = z_rcv_buf[j++];
	uiu.b[mlb] = z_rcv_buf[j++];
	uiu.b[mhb] = z_rcv_buf[j++];
	uiu.b[hb] = z_rcv_buf[j++];
	g_up_hall_call[0][dev] = uiu.dw;

	uiu.b[lb] = z_rcv_buf[j++];
	uiu.b[mlb] = z_rcv_buf[j++];
	uiu.b[mhb] = z_rcv_buf[j++];
	uiu.b[hb] = z_rcv_buf[j++];
	g_dn_hall_call[0][dev] = uiu.dw;

	if ((cons[Second_Riser] == 0) || (grtop_rfl != 0))
	{

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		g_upr_hall_call[0][dev] = uiu.dw;

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		g_dnr_hall_call[0][dev] = uiu.dw;
	}

	if (cons[grtopf] > 32)
	{
		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		g_up_hall_call[1][dev] = uiu.dw;

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		g_dn_hall_call[1][dev] = uiu.dw;

		if ((cons[Second_Riser] == 0) || (grtop_rfl != 0))
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			g_upr_hall_call[1][dev] = uiu.dw;

			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			g_dnr_hall_call[1][dev] = uiu.dw;
		}
	}

	if (cons[Second_Riser] != 0)
	{
		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		g_irup_hall_call[0][dev] = uiu.dw;

		uiu.b[lb] = z_rcv_buf[j++];
		uiu.b[mlb] = z_rcv_buf[j++];
		uiu.b[mhb] = z_rcv_buf[j++];
		uiu.b[hb] = z_rcv_buf[j++];
		g_irdn_hall_call[0][dev] = uiu.dw;

		if (grtop_rfl != 0)
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			g_rirup_hall_call[0][dev] = uiu.dw;

			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			g_rirdn_hall_call[0][dev] = uiu.dw;
		}

		if (cons[grtopf] > 32)
		{
			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			g_irup_hall_call[1][dev] = uiu.dw;

			uiu.b[lb] = z_rcv_buf[j++];
			uiu.b[mlb] = z_rcv_buf[j++];
			uiu.b[mhb] = z_rcv_buf[j++];
			uiu.b[hb] = z_rcv_buf[j++];
			g_irdn_hall_call[1][dev] = uiu.dw;

			if (grtop_rfl != 0)
			{
				uiu.b[lb] = z_rcv_buf[j++];
				uiu.b[mlb] = z_rcv_buf[j++];
				uiu.b[mhb] = z_rcv_buf[j++];
				uiu.b[hb] = z_rcv_buf[j++];
				g_rirup_hall_call[1][dev] = uiu.dw;

				uiu.b[lb] = z_rcv_buf[j++];
				uiu.b[mlb] = z_rcv_buf[j++];
				uiu.b[mhb] = z_rcv_buf[j++];
				uiu.b[hb] = z_rcv_buf[j++];
				g_rirdn_hall_call[1][dev] = uiu.dw;
			}
		}
	}

	g_park[dev] = z_rcv_buf[j++];
	gempf = z_rcv_buf[j++];
	gc_car = z_rcv_buf[j++];
	car_full_update_req = z_rcv_buf[j++]; 
	ncu_door[dev] = z_rcv_buf[j++]; 
	lby_req[dev] = z_rcv_buf[j++];
	ncu_que[dev] = z_rcv_buf[j++];
	cbfl = z_rcv_buf[j++];
	if ((cbfl & 0x80) == 0)
	{
		g_cb_fl[dev] = cbfl;
		g_rcb_fl[dev] = 0;
	}
	else
	{
		g_rcb_fl[dev] = cbfl & 0x7f;
		g_cb_fl[dev] = 0;
	}
	 // Note: leave a bit for rear next up
	next_up = (uint16)(z_rcv_buf[j] & 0x01);
	ir_active = ((z_rcv_buf[j] & 0x40) != 0);
	in_grp_sabbath = ((z_rcv_buf[j] & 0x80) != 0);
	j++;
	grp_codebf[dev] = z_rcv_buf[j++];
	if (uc_len > 20)
	{
		ids_ind[dev] = (uint16)(z_rcv_buf[j] & 0x01);
		ids_rtl[dev] = (uint16)((z_rcv_buf[j] >> 1) & 0x01);
		ids_heof[dev] = (uint16)((z_rcv_buf[j] >> 2) & 0x01);
		ids_ad_off[dev] = (uint16)((z_rcv_buf[j] >> 3) & 0x01);
		ui_ind[dev] = (uint16)((z_rcv_buf[j] >> 4) & 0x01);
		ui_rtl[dev] = (uint16)((z_rcv_buf[j] >> 5) & 0x01);
		ui_heof[dev] =(uint16)( (z_rcv_buf[j] >> 6) & 0x01);
		ui_ad_off[dev] = (uint16)((z_rcv_buf[j] >> 7) & 0x01);
		j++;
	  	asvto_pos[dev] = z_rcv_buf[j++];	 			// Auto service time-out position
		car_asgn_sec = z_rcv_buf[j++];		// security flag from group
		if (cons[Second_Riser] != 0)
			sr_cc_ctrl[dev] = z_rcv_buf[j++];
		spare_byte = z_rcv_buf[j++];
  		if ((spare_byte & 0x01) != 0)		//		spare byte
			any_hall_call = 1;
		else
			any_hall_call = 0;
		if ((spare_byte & 0x02) != 0)
			alt_lobby = 1;
		else
			alt_lobby = 0;
  		if ((spare_byte & 0x04) != 0)		
			any_ir_hall_call = 1;
		else
			any_ir_hall_call = 0;
  		if ((spare_byte & 0x08) != 0)		
			ir_car = dev;
		else
			ir_car = 0;

		if (cons[loc_svc_tmr] == 0)
		{
	  		if ((spare_byte & 0x10) != 0)		
				tmr_svc[s_cc_lockouts] = 1;
			else
				tmr_svc[s_cc_lockouts] = 0;
	  		if ((spare_byte & 0x20) != 0)		
				tmr_svc[s_ccl_override] = 1;
			else
				tmr_svc[s_ccl_override] = 0;
	  		if ((spare_byte & 0x40) != 0)		
				tmr_svc[s_sabbath] = 1;
			else
				tmr_svc[s_sabbath] = 0;
	  		if ((spare_byte & 0x80) != 0)		
				tmr_svc[s_cc_pb_sec] = 1;
			else
				tmr_svc[s_cc_pb_sec] = 0;
		}

		if (cons[VipSvc] != 0)
		{
			vipfl = z_rcv_buf[j++];
			if ((vipfl & 0x80) == 0)
			{
				g_vip_fl[dev] = vipfl;
				g_rvip_fl[dev] = 0;
			}
			else
			{
				g_vip_fl[dev] = 0;
				g_rvip_fl[dev] = vipfl & 0x7f;
			}
		}
		if (cons[Smoke_2004] == 1)
		{
			g_fire_ctrl = z_rcv_buf[j++];
		}

		g_ids_return = z_rcv_buf[j++];
		ids_allw_door[dev] = (0x80 & g_ids_return) >> 7;
		ids_return_typ = (0x40 & g_ids_return) >> 6;
		ids_return_flr  = 0x3F & g_ids_return;

		sec_fl_ctrl[dev] = (uint8)(z_rcv_buf[j] & 0x07);
		local_up_peak = (uint8)((z_rcv_buf[j] & 0x08) >> 3);
		car_hc_fire_door_open = (uint8)((z_rcv_buf[j] >> 4) & 0x01);
		c_ids_fsx = (uint8)((z_rcv_buf[j] >> 5) & 0x01) ;
		j++;

			// spare bytes
		spare_byte = z_rcv_buf[j++];
		spare_byte = z_rcv_buf[j++];
	}
	Group_Comm = 1;
	timers[tgcom_car+gc_car] = 0;  // timer gets cleared for the car that is the group (car 1 or 2)
}

//*********************************************
// Unpack data Packet for comm port
//*********************************************

uint8 unpack_grp_com(void)
{
	static int16 dev, Packet,exception;
	uint8 found;
	int16 i,j,k,ix,hix;
	static unsigned char pkt_end, pkt_start, bc, act_len, ck_sum, act_ck_sum;
	int16 flt_ix;

	exception = e_ok;
	found = 0;
	ix = gc_rcv_gix;
	hix = gc_rcv_pix;
	
	// This is not a circular buffer: if the buffer gets full there is a problem
	// It should never get full!!!

	while ((ix < hix) && (found==0))
	{
		exception = e_ok;
		pkt_start = (uint8)gc_rcv_buf[ix];
		dev = (uint8)gc_rcv_buf[ix+1];
		Packet = (uint8)gc_rcv_buf[ix+2];
		bc = (uint8)gc_rcv_buf[ix+3];		   // byte count

		if ((dev > c_LBY_DPY_addr) || (dev == 0))
			exception = e_inv_address;
		if (pkt_start != ':')
			exception = e_inv_pkt_st;
		if (bc > c_rcv_buf_len)
			exception = e_inv_len;

		if (exception == e_ok)
		{			// Could be a valid packet
			ck_sum = (uint8)gc_rcv_buf[ix+bc];
			pkt_end = (uint8)gc_rcv_buf[(ix+bc)+1];
			act_len = (uint8)gc_rcv_buf[ix+bc+2];
			act_ck_sum = (uint8)gc_rcv_buf[(ix+bc)+3];
			if (act_len != bc)
				exception = e_inv_len;
			if (act_ck_sum != ck_sum)
				exception = e_inv_ck_sum;
			if (pkt_end != '@')
				exception = e_inv_pkt_end;
		}

		if (exception == e_ok)
		{		// valid packet
			found = 1;
			if (dev == gc_addr)
				gc_dev_rx_cnt[dev]++;
			switch (Packet)
			{
				case 0:		// Invalid
					break;
				case 1:     // Unpack car data to group
					unpack_car_data_to_group(dev,ix);
					break;
				case 2:		// Unpack group data to car
					if (dev == cons[carnmb]) // data is for this car
						unpack_group_data_to_car(dev,ix,bc);
					break;
				case 3:		  // set car call from group
					if (dev <= cons[nmbcars])
					{
						set_carcall(cons[carnmb],(uint8)(gc_rcv_buf[ix+4] & 0xff));
					}
					break;
				case 4:		  // Set rear car call from group
					if (dev <= cons[nmbcars])
					{
						set_rcarcall(cons[carnmb],(uint8)(gc_rcv_buf[ix+4] & 0xff));
					}
					break;
				case 5:         // Add a Security Code
					if (LCD_Mode == 11)
					{		   // In security display: reset so index is not corrupted
						Menu_level = 0;
						LCD_Pointer = 11;
					}
					gc_Add_Sec_Fl_Req = (uint8)gc_rcv_buf[ix+4];
					gc_Add_Sec_Code[0] = (uint8)gc_rcv_buf[ix+5];
					gc_Add_Sec_Code[1] = (uint8)gc_rcv_buf[ix+6];
					gc_Add_Sec_Code[2] = (uint8)gc_rcv_buf[ix+7];
					gc_Add_Sec_Code[3] = (uint8)gc_rcv_buf[ix+8];
					if ((gc_Add_Sec_Fl_Req > 0) && (gc_Add_Sec_Fl_Req <= cons[grtopf]))
					{
						Read_CodeTbl(gc_Add_Sec_Fl_Req,&gc_CodeTable, gc_LookUpTable);
						Add_Security_Code(gc_Add_Sec_Fl_Req, &gc_CodeTable, gc_LookUpTable, gc_Add_Sec_Code);
					}
					else if (gc_Add_Sec_Fl_Req == 0xFF)
					{
						Add_One_Code_All_Floors(1,1,gc_Add_Sec_Code);
					}
					break;
				case 6:     	// Delete a Security Code
					if (LCD_Mode == 11)
					{		   // In security display: reset so index is not corrupted
						Menu_level = 0;
						LCD_Pointer = 11;
					}
					gc_Del_Sec_Fl_Req = (uint8)gc_rcv_buf[ix+4];
					gc_Del_Sec_Code[0] = (uint8)gc_rcv_buf[ix+5];
					gc_Del_Sec_Code[1] = (uint8)gc_rcv_buf[ix+6];
					gc_Del_Sec_Code[2] = (uint8)gc_rcv_buf[ix+7];
					gc_Del_Sec_Code[3] = (uint8)gc_rcv_buf[ix+8];
					if ((gc_Del_Sec_Fl_Req > 0) && (gc_Del_Sec_Fl_Req <= cons[grtopf]))
					{
						Read_CodeTbl(gc_Add_Sec_Fl_Req,&gc_CodeTable, gc_LookUpTable);
						Delete_Security_Code(gc_Del_Sec_Fl_Req, &gc_CodeTable, gc_LookUpTable, gc_Del_Sec_Code);
					}
					else if (gc_Del_Sec_Fl_Req == 0xFF)
					{
						Delete_One_Code_All_Floors(1,1,gc_Del_Sec_Code);
					}
					break;
				case 7:     	// Request a Security Code Table
					gc_Sec_Fl_Tbl_Req[dev] = (uint8)gc_rcv_buf[ix+4];
					break;
				case 8:		// Request for fault information from group
					put_pkt_req(9,cons[carnmb],gc_port);
					flt_tx_ix = Flt_ix + 1;
					break;
				case 9:		// Fault data from car
					if (dev <= cons[nmbcars])
					{
						// put_pkt_status(9,dev,gc_port);
						if (bc > 5)
						{			// have data to unpack
							Car_Flt_ix = (uint8)gc_rcv_buf[ix+4];
							i = 5;
							while (i < bc)
							{
								flt_ix = (uint8)gc_rcv_buf[ix+i];
								if (flt_ix < max_flt_ix)
								{
									i++;
									for(j=0;j<(int)sizeof(Fault_type);j++) 
									{
										cf.fltb[flt_ix][j] = (uint8)gc_rcv_buf[ix+i];
										i++;
									}
								}
								else
									i+=sizeof(Fault_type);
							}
						}
					}
					break;	
				case 10:		// Clear fault information
					if (dev == cons[carnmb])
					{
						Flt_ix = 0;
				      	for(i=0; i<max_flt_ix; i++)
				      	{
							f.Flts[i].code = 0;
							f.Flts[i].count = 0;
				      	}
						Flt_ix = 0;
					  	Up_Date_Flt(1);
					}
					break;
				case 11:		// Packet request
					put_pkt_req((uint8)gc_rcv_buf[ix+4],dev,gc_port);
					break;
				case 12:		// Receive device version number
					if ((gc_dev_ver[dev] != 0) || (gc_dev_rev[dev] != 0))
					{
						fault_device = dev;
					  	record_fault(f_hc_dev_reset);
					}
					gc_dev_ver[dev] = (uint8)gc_rcv_buf[ix+4];			// Store device version number
					gc_dev_rev[dev] = (uint8)gc_rcv_buf[ix+5];			// Store device revision number
					break;
				case 13:	// Receive security car call lockout data from the group
					gc_sec = (uint8)(gc_rcv_buf[ix+4] & 0x01);		// Security switch input
					gc_rsec = (uint8)((gc_rcv_buf[ix+4] & 0x02) != 0);
					uiu.b[lb] =(uint8) gc_rcv_buf[ix+5];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+6];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+7];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+8];
					gc_cc_sec[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+9];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+10];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+11];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+12];
					gc_rcc_sec[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+13];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+14];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+15];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+16];
					ids_cc_sec[0][cons[carnmb]] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+17];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+18];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+19];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+20];
					ids_rcc_sec[0][cons[carnmb]] = uiu.dw;
#if (Tract_HR == 1) 
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+21];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+22];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+23];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+24];
					gc_cc_sec[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+25];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+26];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+27];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+28];
					gc_rcc_sec[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+29];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+30];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+31];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+32];
					ids_cc_sec[1][cons[carnmb]] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+33];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+34];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+35];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+36];
					ids_rcc_sec[1][cons[carnmb]] = uiu.dw;
#endif
					break;
				case 14:		
					break;
				case 15:	// Receive Security lockout override data from group	
					gc_sec_ovr = (uint8)(gc_rcv_buf[ix+4] & 0x01);		// Security switch input
					gc_rsec_ovr = (uint8)((gc_rcv_buf[ix+4] & 0x02) != 0);
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+5];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+6];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+7];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+8];
					gc_cc_sec_ovr_set[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+9];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+10];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+11];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+12];
					gc_rcc_sec_ovr_set[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+13];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+14];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+15];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+16];
					gc_cc_sec_ovr_panic[0] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+17];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+18];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+19];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+20];
					gc_rcc_sec_ovr_panic[0] = uiu.dw;
#if (Tract_HR == 1) 
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+21];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+22];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+23];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+24];
					gc_cc_sec_ovr_set[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+25];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+26];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+27];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+28];
					gc_rcc_sec_ovr_set[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+29];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+30];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+31];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+32];
					gc_cc_sec_ovr_panic[1] = uiu.dw;
					uiu.b[lb] = (uint8)gc_rcv_buf[ix+33];
					uiu.b[mlb] = (uint8)gc_rcv_buf[ix+34];
					uiu.b[mhb] = (uint8)gc_rcv_buf[ix+35];
					uiu.b[hb] = (uint8)gc_rcv_buf[ix+36];
					gc_rcc_sec_ovr_panic[1] = uiu.dw;
#endif
					break;
				case 16:		
					break;
				case 17:		// Hall Call device initialization
					break;
				case 18:		// Set and cancel hall call
					break;


				case 22:	// Receive gpi input data from group
					gpi[0] = (uint8)gc_rcv_buf[ix+4];
					gpi[1] = (uint8)gc_rcv_buf[ix+5];
					gpi[2] = (uint8)gc_rcv_buf[ix+6];
					gpi[3] = (uint8)gc_rcv_buf[ix+7];
					gpi[4] = (uint8)gc_rcv_buf[ix+8];
					gpi[5] = (uint8)gc_rcv_buf[ix+9];
					gpi[6] = (uint8)gc_rcv_buf[ix+10];
					gpi[7] = (uint8)gc_rcv_buf[ix+11];
					gpi[8] = (uint8)gc_rcv_buf[ix+12];
					gpi[9] = (uint8)gc_rcv_buf[ix+13];
					gpi[10] = (uint8)gc_rcv_buf[ix+14];
					gpi[11] = (uint8)gc_rcv_buf[ix+15];
#if (Tract_HR == 1)
					gpi[12] = (uint8)gc_rcv_buf[ix+16];
					gpi[13] = (uint8)gc_rcv_buf[ix+17];
					gpi[14] = (uint8)gc_rcv_buf[ix+18];
					gpi[15] = (uint8)gc_rcv_buf[ix+19];
					gpi[16] = (uint8)gc_rcv_buf[ix+20];
					gpi[17] = (uint8)gc_rcv_buf[ix+21];
					gpi[18] = (uint8)gc_rcv_buf[ix+22];
					gpi[19] = (uint8)gc_rcv_buf[ix+23];
					gpi[20] = (uint8)gc_rcv_buf[ix+24];
					gpi[21] = (uint8)gc_rcv_buf[ix+25];
					gpi[22] = (uint8)gc_rcv_buf[ix+26];
					gpi[23] = (uint8)gc_rcv_buf[ix+27];
					gpi[24] = (uint8)gc_rcv_buf[ix+28];
					gpi[25] = (uint8)gc_rcv_buf[ix+29];
					gpi[26] = (uint8)gc_rcv_buf[ix+30];
#endif
					break;
				case 23:	// Receive gpo output data from car
					cgpo[0] = (uint8)gc_rcv_buf[ix+4];
					cgpo[1] = (uint8)gc_rcv_buf[ix+5];
					cgpo[2] = (uint8)gc_rcv_buf[ix+6];
					cgpo[3] = (uint8)gc_rcv_buf[ix+7];
					cgpo[4] = (uint8)gc_rcv_buf[ix+8];
					cgpo[5] = (uint8)gc_rcv_buf[ix+9];
					cgpo[6] = (uint8)gc_rcv_buf[ix+10];
					cgpo[7] = (uint8)gc_rcv_buf[ix+11];
					cgpo[8] = (uint8)gc_rcv_buf[ix+12];
					cgpo[9] = (uint8)gc_rcv_buf[ix+13];
					cgpo[10] = (uint8)gc_rcv_buf[ix+14];
					cgpo[11] = (uint8)gc_rcv_buf[ix+15];
#if (Tract_HR == 1)
					cgpo[12] = (uint8)gc_rcv_buf[ix+16];
					cgpo[13] = (uint8)gc_rcv_buf[ix+17];
					cgpo[14] = (uint8)gc_rcv_buf[ix+18];
					cgpo[15] = (uint8)gc_rcv_buf[ix+19];
					cgpo[16] = (uint8)gc_rcv_buf[ix+20];
					cgpo[17] = (uint8)gc_rcv_buf[ix+21];
					cgpo[18] = (uint8)gc_rcv_buf[ix+22];
					cgpo[19] = (uint8)gc_rcv_buf[ix+23];
					cgpo[20] = (uint8)gc_rcv_buf[ix+24];
					cgpo[21] = (uint8)gc_rcv_buf[ix+25];
					cgpo[22] = (uint8)gc_rcv_buf[ix+26];
					cgpo[23] = (uint8)gc_rcv_buf[ix+27];
					cgpo[24] = (uint8)gc_rcv_buf[ix+28];
					cgpo[25] = (uint8)gc_rcv_buf[ix+29];
					cgpo[26] = (uint8)gc_rcv_buf[ix+30];
#endif
					break;
				case 24:		// Request for fault information from group
					put_pkt_req(23,cons[carnmb],gc_port);
					break;
				case 25:		// PI lantern packet for simple hydro
					break;
				case 26:
					if (dev == cons[carnmb]) // data is for this car
					{
						put_pkt_req(12,dev,gc_port);					
						gc_dev_ver[gc_car] = (uint8)gc_rcv_buf[ix+4];			// Store device version number
						gc_dev_rev[gc_car] = (uint8)gc_rcv_buf[ix+5];			// Store device revision number
					}
					break;
				case 31:		// Receive Security Code Checksum and number of codes for floor
					gc_Sec_Chksum_Fl = gc_rcv_buf[ix + 4];
					if (gc_Sec_Chksum_Fl <= 0)
						gc_Sec_Chksum_Fl = 1;
					if (cons[grtopf] > 30)
					{
						if ((gc_Sec_Chksum_Fl > 30) && (gc_Sec_Chksum_Fl <= cons[grtopf]))
							k = cons[grtopf];
						else
						{
							gc_Sec_Chksum_Fl = 1;
							k = 30;
						}
					}
					else
					{
						gc_Sec_Chksum_Fl = 1;
						k = cons[grtopf];
					}

					j = 0;
					for (i = gc_Sec_Chksum_Fl; i <= k; i++)
					{	
						uwu.b[lbw] = (uint8)gc_rcv_buf[ix+5+j];
						uwu.b[hbw] = (uint8)gc_rcv_buf[ix+6+j];
						gc_Code_Tbl_Chk[i].NumCode = uwu.w;
						uwu.b[lbw] = (uint8)gc_rcv_buf[ix+7+j];
						uwu.b[hbw] = (uint8)gc_rcv_buf[ix+8+j];
						gc_Code_Tbl_Chk[i].CheckSum = uwu.w;
						j += 4;
					}
					if ((cons[grtopf] <= 30) || ((gc_Sec_Chksum_Fl > 30) && (cons[grtopf] > 30)))
						gc_Sec_Fl_Tbl_Req[dev] = (uint8)Verify_Code_Check_Table(&gc_Code_Tbl_Chk[gc_Sec_Chksum_Fl]);	  // wait for all the data
					else
						gc_Sec_Fl_Tbl_Req[dev] = 0;
						
					if (gc_Sec_Fl_Tbl_Req[dev] != 0)
					{
						put_pkt_req(7,dev,gc_port);
					}
					break;

				case 32:		// Receive Codes to Car for one floor
					if (gc_Sec_Fl_Tbl_Req[cons[carnmb]] == gc_rcv_buf[ix+4])
					{
						gc_Sec_Code_Index = gc_rcv_buf[ix+5];
						gc_CodeTable.NumCode = gc_rcv_buf[ix+6];

						if (gc_Sec_Code_Index < 0)
							gc_Sec_Code_Index = 1;

						if (gc_CodeTable.NumCode < 50)
							k = gc_CodeTable.NumCode;
						else
							k = 50;

						j = 0;
						i = 0;
						while (i < k)
						{
							gc_CodeTable.Codes[gc_Sec_Code_Index+i][0] = (uint8)gc_rcv_buf[ix+7+j];
							gc_CodeTable.Codes[gc_Sec_Code_Index+i][1] = (uint8)gc_rcv_buf[ix+8+j];
							gc_CodeTable.Codes[gc_Sec_Code_Index+i][2] = (uint8)gc_rcv_buf[ix+9+j];
							gc_CodeTable.Codes[gc_Sec_Code_Index+i][3] = (uint8)gc_rcv_buf[ix+10+j];
							i++;
							j+=4;
						}
						if ((gc_Sec_Code_Index + i) >= gc_CodeTable.NumCode)
						{
							if (LCD_Mode == 11)
							{		   // In security display: reset so index is not corrupted
								Menu_level = 0;
								LCD_Pointer = 11;
							}
							Write_CodeTbl(gc_Sec_Fl_Tbl_Req[cons[carnmb]], &gc_CodeTable, gc_LookUpTable);
						}
					}
					break;

				case 33:		// Receive Command to delete all security codes for one or all floors
					gc_Del_All_Codes_Fl_Req = (uint8)gc_rcv_buf[ix+4];
					if (gc_Del_All_Codes_Fl_Req != 0)
					{
						if ((gc_Del_All_Codes_Fl_Req >= cons[bottomf]) && (gc_Del_All_Codes_Fl_Req <= cons[topf]))
						{
							if (LCD_Mode == 11)
							{		   // In security display: reset so index is not corrupted
								Menu_level = 0;
								LCD_Pointer = 11;
							}
							Clear_All_Codes_One_Floor(gc_Del_All_Codes_Fl_Req, &gc_CodeTable, gc_LookUpTable);
						}
						else if (gc_Del_All_Codes_Fl_Req == 0xFF)
						{
							if (LCD_Mode == 11)
							{		   // In security display: reset so index is not corrupted
								Menu_level = 0;
								LCD_Pointer = 11;
							}
							Clear_All_Codes_All_Floors(1,1);
						}
					}
					break;

				case 35:         // Receive voltage sensor input
					uwu.b[lbw] = (uint8)gc_rcv_buf[ix+4];
					uwu.b[hbw] = (uint8)gc_rcv_buf[ix+5];
					g_L1_Voltage[dev] = uwu.w;
					uwu.b[lbw] = (uint8)gc_rcv_buf[ix+6];
					uwu.b[hbw] = (uint8)gc_rcv_buf[ix+7];
					g_L2_Voltage[dev] = uwu.w;
					uwu.b[lbw] = (uint8)gc_rcv_buf[ix+8];
					uwu.b[hbw] = (uint8)gc_rcv_buf[ix+9];
					g_L3_Voltage[dev] = uwu.w;
					uwu.b[lbw] = (uint8)gc_rcv_buf[ix+10];
					uwu.b[hbw] = (uint8)gc_rcv_buf[ix+11];
					g_Door_Voltage[dev] = uwu.w;
					g_Voltage_Dev_Online[dev] = (uint8)gc_rcv_buf[ix+12];
					break;

				case 36:
					put_pkt_status(36,dev,gc_port);
					break;
					
				case 37:
				 	if(dev == cons[carnmb])
					{
					   	uiu.b[lb] = gc_rcv_buf[ix+4];
						uiu.b[mlb] = gc_rcv_buf[ix+5];
						uiu.b[mhb] = gc_rcv_buf[ix+6];
						uiu.b[hb] = gc_rcv_buf[ix+7];
						ui_cc_sec[0][cons[carnmb]] = uiu.dw;						   						
						uiu.b[lb] = gc_rcv_buf[ix+8];
						uiu.b[mlb] = gc_rcv_buf[ix+9];
						uiu.b[mhb] = gc_rcv_buf[ix+10];
						uiu.b[hb] = gc_rcv_buf[ix+11];
					   	ui_cc_sec[1][cons[carnmb]] = uiu.dw;			 
					} 				 
					break;

				default:
					break;
			}
			ix = (int16)(ix + bc + 3);
		}
		else
			ix++;
		gc_rcv_gix = ix;
	}
	return found;
}

/* Revision History

9/13/11 v5.54.24 hdl 	1. Added local_up_peak to the data sent over to each car

9/22/11 v5.54.25 mhd	1. Set EMP and EPT status in Group_IO_Com[cons[carnmb]][g_empf] for split power feeder operation.
						2. Use gempf instead of grp_empf[0] from group.
v6.0.0	04/08/11  mhd 		1. When gc_dev_ver and gc_dev_rev == 0, must initialize hall call board with packet 17 or 27 but not packet 26.

2/1/12 v5.54.53 mhd		1. Send bit from group to car for grp_hc_fire_door_open which get set as car_hc_fire_door_open. (Special for denver fire service)

2/27/11	v5.54.59 hdl	1. Send the information for hall calls security to both the main and second riser depending on field variable  fvhcsecctrl 

3/2/12 v5.54.60 mhd		1. Anded i_EPT with cons variable ((cons[EMPop] & 0x01) != 0) so the i_EPT is not set from BDC input. See emp_bits.
6/14/12 v5.54.87 hdl 	1. Added g_ids_fsx to control the fsx input being sent from liftnet from a master switch for all groups and all cars.
   ^^^ v6.0,12 mhd 
*/