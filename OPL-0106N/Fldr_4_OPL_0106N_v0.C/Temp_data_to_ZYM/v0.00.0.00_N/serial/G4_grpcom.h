//*********************************
// Group Comm variables
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


								
extern int16 Dispatcher;		// Dispatcher for this car
extern int16 prev_Dispatcher;	// Previous Dispatcher
											 

extern int16 gc_source;						// Group comm interrupt source
extern int16 gc_hc_dev;						// Set to 1 when hall call serial board used
extern int16 gc_hall_io_dev;					// Set to 1 when using any serial hall i/o device
extern int16 gc_restart_req;					// restart request
extern int16 gc_unpack_error;					// Unpack flag set but no data unpacked
extern int16 gc_ix;							// index for elevator or device to communicate with
extern int16 gc_car_ix; 						// index to keep track of which car was polled
extern int16 gc_hc_ix;  						// index to keep track of polling the remaining hall call devices
extern int16 gc_car_cnt;						// Number of cars to comm to before hall device
extern int16 gc_addr;            				// Address sent or received on comm
extern int16 gc_port;							// Group Comm port
extern int16 gc_master;						// Set to 1 if this car is master
extern int16 gc_car;							// Group Comm car 
				// List of devices on group comm
extern int16 gc_dev_list[c_com_no_dev];
				// Version number for each device
extern int16 gc_dev_ver[c_com_no_dev];
				// Revision number for each device
extern int16 gc_dev_rev[c_com_no_dev];
extern int16 gc_dev_online[c_com_no_dev];
				// Transmit packet count
extern uint16 gc_dev_tx_cnt[c_com_no_dev];
				// Receive packet count
extern uint16 gc_dev_rx_cnt[c_com_no_dev];
extern uint16 gc_loss_cnt[c_com_no_dev];
extern int16 gc_pkt_11_req_pkt[5]; 				// Used to request a packet from packet 11
extern int16 gc_num_dev;						// Number of devices on comm
extern int16 gc_trm_int;    					// Car comunication port transmit interupt running
extern int16 gc_trm_buf_empty;				// Communications port transmit buffer is empty
extern int16 gc_response_wait;				// Wait for response when set before transmitting again
extern int16 response_wait_time;				// Response wait time (depends on the number of devices)
extern int16 gc_trm_case;        				// Car switch case index for transmit state machine
extern int16 gc_trm_ix;          				// trasmit buffer index
extern int16 gc_trm_len;  			    	// trasmit buffer length
extern int16 gc_rcv_int;     					// Car comunication port receive interupt running
extern int16 gc_rcv_case;        				// Car switch case index for receive state machine
extern int16 gc_unpack;          				// Set to 1 to unpack Car receive buffer
extern int16 gc_rcv_pix;          			// Car receive buffer put index
extern int16 gc_rcv_gix;          			// Car receive buffer get index
extern uint8 gc_rcv_len;			// Car receive length sent in Packet
extern uint8 gc_rcv_act_len;		// Car actual receive length of Packet
extern uint8 gc_rcv_ck_sum;			// Car receive comm checksum
extern uint16 gc_trm_buf[c_trm_buf_size];  // transmit buffer
extern uint16 gc_rcv_buf[c_rcv_buf_size];  // receive buffer
extern bool Car_Not_Talking[car_size]; 				// car not communication one 484 port
extern int16 full_update_count;
extern int16 full_update_req[car_size];				// Request by group for car to send a full update
extern int16 car_full_update_req;				// Car flag to send a full update to the group
extern bool Group_Comm;		// Group comm with car

// Data arrays to compress transmission from car to group
extern char unsigned pack_buf[c_buf_size];			// Car data packed into this buffer first
extern char unsigned hold_buf[c_buf_size];			// Buffer used to compare packed data with data transmitted previously to send data that has changed
extern char unsigned unpack_buf[8][c_buf_size];	// Group unpacks the car data that has changed into a buffer for each car

// Hall call comm I/O's and variables

extern bool gc_luc;	   			// Lobby Up request for seperate i/o board
extern bool gc_luc_canc;			// Lobby Up request cancel for seperate i/o board

extern uint32 gc_uc[2];			// Up calls from comm
extern uint32 gc_uc_set[2];		// Set up calls on comm
extern uint32 gc_uc_canc[2];		// Cancel up calls on comm

extern uint32 gc_dc[2];			// Down calls from comm
extern uint32 gc_dc_set[2];		// Set down calls on comm
extern uint32 gc_dc_canc[2];		// Cancel down calls on comm

extern uint32 gc_ruc[2];			// Up calls from comm
extern uint32 gc_ruc_set[2];		// Set up calls on comm
extern uint32 gc_ruc_canc[2];		// Cancel up calls on comm

extern uint32 gc_rdc[2];			// Down calls from comm
extern uint32 gc_rdc_set[2];		// Set down calls on comm
extern uint32 gc_rdc_canc[2];		// Cancel down calls on comm

extern uint32 gc_uc_dsr[2];  		// Up calls from dual serial riser comm
extern uint32 gc_dc_dsr[2];  		// Down calls from  dual serial riser comm
extern uint32 gc_ruc_dsr[2]; 		// Up calls from  dual serial riser comm
extern uint32 gc_rdc_dsr[2]; 		// Down calls from  dual serial riser comm


extern uint32 gc_iruc[2];			// Up calls from comm
extern uint32 gc_iruc_set[2];		// Set up calls on comm
extern uint32 gc_iruc_canc[2];	// Cancel up calls on comm

extern uint8 gc_ir;				// Inconspicious riser switch activation (last floor down call + 1)
extern uint8 gc_ir_2;				// Inconspicious riser switch activation for car 2 (last floor down call + 2)
extern uint8 ir_hc;				// Use ir for std hall call if set
extern uint8 rir_hc;				// Use rir for std hall call if set

extern uint32 gc_irdc[2];			// Down calls from comm
extern uint32 gc_irdc_set[2];		// Set down calls on comm
extern uint32 gc_irdc_canc[2];	// Cancel down calls on comm

extern uint32 gc_riruc[2];		// Up calls from comm
extern uint32 gc_riruc_set[2];	// Set up calls on comm
extern uint32 gc_riruc_canc[2];	// Cancel up calls on comm

extern uint32 gc_rir;				// Rear Inconspicious riser switch activation (last floor rear down call + 1)
extern uint32 gc_rirdc[2];		// Down calls from comm
extern uint32 gc_rirdc_set[2];	// Set down calls on comm
extern uint32 gc_rirdc_canc[2];	// Cancel down calls on comm

extern uint8 gc_cb_sr;			// Code Blue second riser switch activation (last floor down call + 1)
extern uint8 gc_rcb_sr;			// Code Blue rear second riser switch activation (last floor rear down call + 1)
extern uint8 cb_dr;				// Use cb dual riser for std hall call if set
extern uint8 rcb_dr;			// Use rcb dual riser for std hall call if set

extern uint32 gc_cb[2];			// Code Blue floors from comm
extern uint32 gc_cb_set[2];		// Set code blue call
extern uint32 gc_cb_canc[2];		// Cancel code blue call
extern uint32 gc_rcb[2];			// Code Blue floors from comm
extern uint32 gc_rcb_set[2];		// Set rear code blue call
extern uint32 gc_rcb_canc[2];		// Cancel rear code blue call

extern uint32 gc_cbsr[2];			// Code Blue second riser floors from comm
extern uint32 gc_cbsr_set[2];		// Set code blue second riser call   
extern uint32 gc_cbsr_canc[2];	// Cancel code blue second riser call
extern uint32 gc_rcbsr[2];		// Rear Code Blue second riser floors from comm
extern uint32 gc_rcbsr_set[2];	// Set rear code blue second riser call   
extern uint32 gc_rcbsr_canc[2];	// Cancel rear code blue second riser call

extern uint8 gc_sec;				// Group car call lockout security flag
extern uint32 gc_cc_sec[2];		// Car Call Lockout Security
extern uint8 gc_rsec;				// Group rear car call lockout security flag
extern uint32 gc_rcc_sec[2];		// Car Call Lockout Security
extern uint8 gc_hsec;				// Group front hall call security flag
extern uint32 gc_hc_sec[2];		// Hall Call Lockout Security
extern uint8 gc_rhsec;			// Group rear hall call security flag
extern uint32 gc_rhc_sec[2];		// Hall Call Lockout Security

extern uint8 update_hc_sec;
extern uint8 gc_hsec_prev;				// Group front hall call security flag
extern uint32 gc_hc_sec_prev[2];		// Hall Call Lockout Security
extern uint8 gc_rhsec_prev;			   	// Group rear hall call security flag
extern uint32 gc_rhc_sec_prev[2];	// Hall Call Lockout Security


extern uint8 gc_sec_ovr;					// Group car call security override flag
extern uint32 gc_cc_sec_ovr[2];			// Group car call security override 
extern uint32 gc_cc_sec_ovr_set[2];		// Group car call security override output set
extern uint32 gc_cc_sec_ovr_panic[2];		// Group car call security override panic stop 
extern uint32 gc_cc_sec_ovr_toggle[2];	// Group car call security override toggle 
extern uint8 gc_rsec_ovr;					// Group rear car call security override flag
extern uint32 gc_rcc_sec_ovr[2];			// Group rear car call security override
extern uint32 gc_rcc_sec_ovr_set[2];		// Group rear car call security override output set
extern uint32 gc_rcc_sec_ovr_panic[2];	// Group rear car call security override panic stop
extern uint32 gc_rcc_sec_ovr_toggle[2];	// Group rear car call security override toggle

extern uint32 gc_xuc[2];	  		// Group cross assignment up call input
extern uint32 gc_xuct[2];			// Group cross assignment up call transfer flag
extern uint32 gc_xucl[2];			// Group cross assignment up call latch
extern uint32 gc_xdc[2];	 	 	// Group cross assignment down call	input
extern uint32 gc_xdct[2];			// Group cross assignment up call transfer flag
extern uint32 gc_xdcl[2];			// Group cross assignment down call latch

extern uint32 *gc_vip;			// Vip floors from comm
extern uint32 *gc_vip_set;		// Set vip call
extern uint32 *gc_vip_canc;		// Cancel vip call
extern uint32 *gc_rvip;			// Rear Vip floors from comm
extern uint32 *gc_rvip_set;		// Set rear vip call
extern uint32 *gc_rvip_canc;		// Cancel rear vip call

// Dual riser mask
extern int32 pfdsr_msk[2];			// Primary front dual serial riser mask
extern int32 sfdsr_msk[2];			// Secondary front dual serial riser mask
extern int32 prdsr_msk[2];			// Primary rear dual serial riser mask
extern int32 srdsr_msk[2];			// Secondary rear dual serial riser mask


// Group Comm security variables

extern uint8 gc_Add_Sec_Fl_Req;			// Send security floor to add code
extern uint8 gc_Add_Sec_Code[4];			// Security Code to Add a code
extern uint8 gc_Del_Sec_Fl_Req;			// Send Security floor to delete code
extern uint8 gc_Del_Sec_Code[4];			// Security code to delete a code
extern uint8 gc_Del_All_Codes_Fl_Req;		// Delete all codes for one floor or all floors
extern uint8 gc_Sec_Fl_Tbl_Req[9];			// Floor request for security table
extern int16 gc_LookUpTable[NumMaxFl];
extern int16 gc_Sec_Chksum_Fl;						// Floor start for Security Checksum
extern int16 gc_Sec_Code_Index;						// Code Index pointer
extern int16 gc_Sec_Num_Codes; 						// Number of Codes to transfer
extern int16 gc_Code_Xfer_Comp;						// Code table transfer complete


extern void Group_Com(void);
extern void init_grp_com (void);

extern void put_pkt_11_req (int16 pkt_nmb);
extern int16 get_pkt_11_req (void);
extern void send_hc_dvr_req (int16 cmd);
