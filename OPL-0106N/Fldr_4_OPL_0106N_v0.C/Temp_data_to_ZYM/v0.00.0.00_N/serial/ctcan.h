#define c_max_ctcan_dev 55	// 55 devices 
#define c_max_ctcan_io_brd 40	// 40 i/o boards 
#define c_max_ctcan_ser_array (c_max_ctcan_io_brd * 3)


extern int16 cop_can_error;		// Prevent movement of the car on automatic when a cop device error
extern int16 sel_can_error;		// Prevent running the car when the selector is not communicating
extern int16 selector_ul_deadzone;
extern int16 selector_dl_deadzone;
extern int16 sel_can_online;
extern int16 lw_can_online;
extern int16 cop_can_online;
extern int16 vs_can_online;


#define  c_ctcan_START	  1		  // Can start index
#define  c_ctcan_ENC_SEL  1		  // Encoded Tape Selector
#define  c_ctcan_SEL	  2		  // Selector board
#define  c_ctcan_DOOR	  3		  // Front Door board
#define  c_ctcan_RDOOR	  4		  // Rear Door Board
#define  c_ctcan_LW		  5		  // Load weigher
#define  c_ctcan_VS		  6		  // Voltage Sensor
#define  c_ctcan_PI_LANT  7		  // Serial PI and Lantern (global address for all pi and lantern devices)
#define	 c_ctcan_SEB_1	  8		  // Serial I/O Expansion board 1	(io brd 7)
#define	 c_ctcan_SEB_2	  9		  // Serial I/O Expansion board 2	(io brd 8)
#define	 c_ctcan_SEB_3	 10		  // Serial I/O Expansion board 3	(io brd 9)
#define	 c_ctcan_SEB_4	 11		  // Serial I/O Expansion board 4	(io brd 10)
#define	 c_ctcan_SEB_5	 12		  // Serial I/O Expansion board 5	(io brd 11)
#define	 c_ctcan_SEB_6	 13		  // Serial I/O Expansion board 6	(io brd 12)
#define	 c_ctcan_SEB_7	 14		  // Serial I/O Expansion board 7	(io brd 13)
#define  c_ctcan_SEB_8	 15		  // Serial I/O Expansion board 8   (io brd 14)
#define  c_ctcan_SEB_9	 16		  // Serial I/O Expansion board 9   (io brd 15)
#define  c_ctcan_SEB_10	 17		  // Serial I/O Expansion board 10  (io brd 16)
#define  c_ctcan_SEB_11	 18		  // Serial I/O Expansion board 11  (io brd 17)
#define  c_ctcan_SEB_12	 19		  // Serial I/O Expansion board 12  (io brd 18) 
#define  c_ctcan_SEB_13	 20		  // Serial I/O Expansion board 13  (io brd 19) 
#define  c_ctcan_SEB_14	 21		  // Serial I/O Expansion board 14  (io brd 20) 
#define  c_ctcan_SEB_15	 22		  // Serial I/O Expansion board 15  (io brd 21) 
#define  c_ctcan_SEB_16	 23		  // Serial I/O Expansion board 16  (io brd 22) 
#define  c_ctcan_SEB_17	 24		  // Serial I/O Expansion board 17  (io brd 23) 
#define  c_ctcan_SEB_18	 25		  // Serial I/O Expansion board 18  (io brd 24) 
#define  c_ctcan_SEB_19	 26		  // Serial I/O Expansion board 19  (io brd 25) 
#define  c_ctcan_SEB_20	 27		  // Serial I/O Expansion board 20  (io brd 26)
#define  c_ctcan_SEB_21	 28		  // Serial I/O Expansion board 21  (io brd 27)
#define  c_ctcan_SEB_22	 29		  // Serial I/O Expansion board 22  (io brd 28)
#define  c_ctcan_SEB_23	 30		  // Serial I/O Expansion board 23  (io brd 29) 
#define  c_ctcan_SEB_24	 31		  // Serial I/O Expansion board 24  (io brd 30) 
#define  c_ctcan_SEB_25	 32		  // Serial I/O Expansion board 25  (io brd 31) 
#define  c_ctcan_SEB_26	 33		  // Serial I/O Expansion board 26  (io brd 32) 
#define  c_ctcan_SEB_27	 34		  // Serial I/O Expansion board 27  (io brd 33) 
#define  c_ctcan_SEB_28	 35		  // Serial I/O Expansion board 28  (io brd 34) 
#define  c_ctcan_SEB_29	 36		  // Serial I/O Expansion board 29  (io brd 35) 
#define  c_ctcan_SEB_30	 37		  // Serial I/O Expansion board 30  (io brd 36)
#define  c_ctcan_SEB_31	 38		  // Serial I/O Expansion board 31  (io brd 37)
#define  c_ctcan_SEB_32	 39		  // Serial I/O Expansion board 32  (io brd 38)
#define  c_ctcan_SEB_33	 40		  // Serial I/O Expansion board 33  (io brd 39) 
#define  c_ctcan_SEB_34	 41		  // Serial I/O Expansion board 34  (io brd 40) 
#define  c_ctcan_SEB_35	 42		  // Serial I/O Expansion board 35  (io brd 41) 
#define  c_ctcan_SEB_36	 43		  // Serial I/O Expansion board 36  (io brd 42) 
#define  c_ctcan_SEB_37	 44		  // Serial I/O Expansion board 37  (io brd 43) 
#define  c_ctcan_SEB_38	 45		  // Serial I/O Expansion board 38  (io brd 44) 
#define  c_ctcan_SEB_39	 46		  // Serial I/O Expansion board 39  (io brd 45) 
#define  c_ctcan_SEB_40	 47		  // Serial I/O Expansion board 40  (io brd 46)
#define  c_ctcan_END	 48		  // Last can device
#define  c_ctcan_GLOBAL	 49		  // Global can address


extern int16 ctcan_addr[c_max_ctcan_dev];


// Declarations for Serial I/O Boards

extern union tag_ser_ct {

	union 
	{
		uint8 iob[3];
		struct
		{
			uint8 io1:1;
			uint8 io2:1;
			uint8 io3:1;
			uint8 io4:1;
			uint8 io5:1;
			uint8 io6:1;
			uint8 io7:1;
			uint8 io8:1;
		}B[3];
		struct
		{
			uint8 nib1:4;
			uint8 nib2:4;
		}N[3];
	} dev [c_max_ctcan_io_brd];	
	
	unsigned char brd_io [c_max_ctcan_ser_array];
}serial_ctc;

extern  struct tag_sync {
		unsigned char iob[3];
		unsigned char reflect_data;
		unsigned char rtx_cnt;
} sync_dev_ctc [c_max_ctcan_io_brd];

#define ctcan_que_len 3

extern struct tag_ctcan_dev {
		unsigned char online;
		unsigned char online_cnt;
		unsigned char version;
		unsigned char revision;
		unsigned char msg_req[ctcan_que_len];
		unsigned char can_error;
		unsigned char can_error_data;
		unsigned char label_ix;
		unsigned char label_req;
		unsigned char dev_error;
		unsigned char dev_error_data[2];
		unsigned char local_dev_comm_loss;
		unsigned char color_ix;
		unsigned char color_ix_req;
		unsigned char io_color_ix_chksum; 
		uint16 tx_cnt;
		uint16 rx_cnt;
		uint16 ioname_chksum;
} ctcan_dev[c_max_ctcan_dev];

extern int16 ctcan_dev_req;
extern int16 ctcan_msg_req;
extern int16 ctcan_msg_cmd;
extern int16 ctcan_msg_cnt;

extern unsigned char ctc_ioset[c_max_ctcan_ser_array];
extern unsigned char ctc_ioclr[c_max_ctcan_ser_array];

extern unsigned char ctcan_dev_list [c_max_ctcan_dev];
extern int16 nmb_ctcan_ports;
extern int16 nmb_ctcan_dev;
extern int16 ram_seq;
extern int16 ctcan_init_count;
extern int16 init_mode_cnt;
extern int16 ir_mask_1;
extern int16 ir_mask_2;
extern int32 ctcan_status;
extern int16 ctcan_mode;
extern int16 ctcan_int_status;
extern int16 ctcan_error;
extern int16 ctcan_int_count;
extern int16 re_init_can;
extern int16 ctcan_arb_lost_cap;
extern int16 ctcan_err_code_cap;
extern int16 ctcan_tx_err_cnt;
extern int16 ctcan_rx_err_cnt;
extern int16 BEI_cnt;
extern int16 ALI_cnt;
extern int16 EPI_cnt;
extern int16 WUI_cnt;
extern int16 DOI_cnt;
extern int16 EWI_cnt;
extern int16 TI_cnt;
extern int16 RI_cnt;

extern int32 ctcan_debug_1;
extern int32 ctcan_debug_2;
extern int32 ctcan_debug_3;
extern int32 ctcan_debug_4;


#define c_tx_buf_siz 30

extern struct tag_tx {
		unsigned char len;
		unsigned char ex_id;	// flag for extended id mode
		unsigned char one_tx;	// Transmit one packet (no retransmit)
		unsigned char rtr;		// Remote Transmission Request
		unsigned char srr;		// Substitute Remote Request
		unsigned char msg;		// Message
		unsigned char addr;		// Address
		unsigned char id2;
		unsigned char id3;
		unsigned char data [8];
		} ctcan_tx[c_tx_buf_siz+1];
		
extern int16 ctc_tpix;					// can transmit buffer put index
extern int16 ctc_tgix;					// can transmit buffer get index
		
#define c_rx_buf_siz 30

extern struct tag_rx {
		unsigned char valid_msg;	// 1 when valid message in buffer
		unsigned char len;
		unsigned char ex_id;	// flag for extended id mode
		unsigned char rtr;		// Remote Transmission Request
		unsigned char srr;		// Substitute Remote Request
		unsigned char msg;		// Message
		unsigned char addr;		// Address
		unsigned char id2;
		unsigned char id3;
		unsigned char data [8];
		} ctcan_rx[c_rx_buf_siz + 1];
		
extern int16 ctc_rpix;					// can transmit buffer put index
extern int16 ctc_rgix;					// can transmit buffer get index
		

// Declarations for Door Operator Boards

// Front Door Operator Data

extern struct tag_DOOR {
		// Data Transmitted
		unsigned char command;
									// Bit 0: Open command   1 = Open
								 	// Bit 1: Close Command 1 = Close
								 	// Bit 2: Nudge Close	      1 = Close at nudging speed
								 	// Bit 3: Door Zone Status   1 = on dz
								 	// Bit 4: Unused
								 	// Bit 5: Heavy Door 1=use heavy door parameters
								 	// Bit 6: Narrow Door 1=use narrow door limit
								 	// Bit 7: Unused
		unsigned char prev_command;
		unsigned char status_ret;	/*	Status return bits
											Bit 0: Door Open Limit
											Bit 1: Door Close Limit
											Bit 2: DPM Limit
											Bit 3: Unused 
											Bit 4: Door reopen overspeed
											Bit 5: Door reopen over torque
											Bit 6: Door reopen ee/de
											Bit 7: Unused
									*/
		unsigned char reflect_data;	// Set true to send back status return data
		unsigned char par_cmd;		// Parameter Command
									/*  Bit 0: Read/Write Command 1 = write
										Bits 1-7 unused
									*/
		unsigned char prev_par_cmd;	// previous parameter command
		uint16 par_add_req;			// Parameter Address request 
		uint16 par_dat_wrt;			// Parameter Data wrt
		unsigned char dpy_req;		/* Display Request
										0 = Fault Code									
										1 = Output Voltage
										2 = Output Current
										3 = Output Frequency
										4-15 not defined at this time
									*/
		// Data Received
		unsigned char status_1;		/*	Status 1 bits
											Bit 0: Door Open Limit
											Bit 1: Door Close Limit
											Bit 2: DPM Limit
											Bit 3: Unused 
											Bit 4: Door reopen overspeed
											Bit 5: Door reopen over torque
											Bit 6: Door reopen ee/de
											Bit 7: Unused
									*/
		unsigned char status_2;			/*	Status 2 bits
											Bit 0: Fault
											Bit 1: Door Opening
											Bit 2: Door Closing
											Bit 3: Unused
											Bit 7,6,5,4: Monitor Value Returned  0-15 
											0 = Fault Code
											1 = Output Voltage
											2 = Output Current
											3 = Output Frequency
											4-15 not defined at this time
									*/
		unsigned char command_ret;		// Command byte return value (see command bits)
		unsigned char par_err;			// Parameter error
										/* 	0 = Okay
											1 = Illegal command code
											2 = Illegal data address
											3 = Illegal data value
										*/
		uint16 dpy_ret;				// Monitor value display return 
		uint16 fault;				// door fault code
		uint16 voltage;				// door output voltage
		uint16 current;				// door output current
		uint16 frequency;			// door output frequency
		uint16 par_add_ret;			// Parameter address return
		uint16 par_dat_rd;			// Parameter data read
		} DOOR_can; 

// Raar Door Operator Data

extern struct tag_RDOOR {
		// Data Transmitted
		unsigned char command;
									// Bit 0: Open command   1 = Open
								 	// Bit 1: Close Command 1 = Close
								 	// Bit 2: Nudge Close	      1 = Close at nudging speed
								 	// Bit 3: Door Zone Status   1 = on dz
								 	// Bit 4: Unused
								 	// Bit 5: Heavy Door 1=use heavy door parameters
								 	// Bit 6: Narrow Door 1=use narrow door limit
								 	// Bit 7: Unused
		unsigned char prev_command;
		unsigned char status_ret;	/*	Status return bits
											Bit 0: Door Open Limit
											Bit 1: Door Close Limit
											Bit 2: DPM Limit
											Bit 3: Unused 
											Bit 4: Door reopen overspeed
											Bit 5: Door reopen over torque
											Bit 6: Door reopen ee/de
											Bit 7: Unused
									*/
		unsigned char reflect_data;	// Set true to send back status return data
		unsigned char par_cmd;		// Parameter Command
									/*  Bit 0: Read/Write Command 1 = write
										Bits 1-7 unused
									*/
		unsigned char prev_par_cmd;	// previous parameter command
		uint16 par_add_req;			// Parameter Address request 
		uint16 par_dat_wrt;			// Parameter Data wrt
		unsigned char dpy_req;		/* Display Request
										0 = Fault Code									
										1 = Output Voltage
										2 = Output Current
										3 = Output Frequency
										4-15 not defined at this time
									*/
		// Data Received
		unsigned char status_1;		/*	Status 1 bits
											Bit 0: Door Open Limit
											Bit 1: Door Close Limit
											Bit 2: DPM Limit
											Bit 3: Unused 
											Bit 4: Door reopen overspeed
											Bit 5: Door reopen over torque
											Bit 6: Door reopen ee/de
											Bit 7: Unused
									*/
		unsigned char status_2;			/*	Status 2 bits
											Bit 0: Fault
											Bit 1: Door Opening
											Bit 2: Door Closing
											Bit 3: Unused
											Bit 7,6,5,4: Monitor Value Returned  0-15 
											0 = Fault Code
											1 = Output Voltage
											2 = Output Current
											3 = Output Frequency
											4-15 not defined at this time
									*/
		unsigned char command_ret;		// Command byte return value (see command bits)
		unsigned char par_err;			// Parameter error
										/* 	0 = Okay
											1 = Illegal command code
											2 = Illegal data address
											3 = Illegal data value
										*/
		uint16 dpy_ret;				// Monitor value display return 
		uint16 fault;				// door fault code
		uint16 voltage;				// door output voltage
		uint16 current;				// door output current
		uint16 frequency;			// door output frequency
		uint16 par_add_ret;			// Parameter address return
		uint16 par_dat_rd;			// Parameter data read
		} RDOOR_can; 

// Declarations for Selector Board
extern struct tag_SEL {
			unsigned char command;			// Selector Command
											//  Bit0: Preset Position count
											//  Bit1: Learn Floor Position
											//  Bit2: Reset Fault
											//  Bit3: Clear Fault table
			int32 poscnt;					// Car Position Count
			unsigned char position;			// Floor position	
			unsigned char doorzone;			// door zone inputs 
											//  Bit0: UL
											//	Bit1: DZ
											//  Bit2: DZA
											//  Bit3: DL
			unsigned char up_normal_limits;	// Up Slowdown Inputs
											//  Bit0: UN
											//  Bit1: UT
											//  Bit2: UT1
											//  Bit3: UT2
											//  Bit4: UT3
											//  Bit5: UT4
											//  Bit6: UT5
											//  Bit7: UT6
			unsigned char dn_normal_limits;	// Down Slowdown Inputs
											//  Bit0: DN
											//  Bit1: DT
											//  Bit2: DT1
											//  Bit3: DT2
											//  Bit4: DT3
											//  Bit5: DT4
											//  Bit6: DT5
											//  Bit7: DT6
			unsigned char em_terminal_limits; // Emergency Slowdown Inputs
											//  Bit0: DTS
											//  Bit1: UTS
			unsigned char status;			// Selector Status
											//  Bit0: Hoistway Learned
											//  Bit1: In Hw Learn
											//  Bit2: Fault
			unsigned char reflect_data;		// Return data verification
			unsigned char fault[10];		// Selector Faults
											//  0 = no fault
											//	1 = Sensor fault
		} SEL_can;

// Declarations for can bus pi and lantern devices

extern int16 alrm_pi_stat;
extern unsigned char pi_pos;
extern unsigned char lant_stat;
extern unsigned char motion_stat;
extern unsigned char fire_stat;
extern unsigned char door_stat;
extern unsigned char misc_stat;
extern unsigned char pi_lant_stat;
extern unsigned char pi_motion_stat;
extern unsigned char pi_fire_stat;
extern unsigned char pi_door_stat;
extern unsigned char pi_misc_stat;
extern unsigned char update_pi_lant;

extern uint32 pi_cc[2];
extern uint32 pi_rcc[2];

extern int32 c_cc_dpy[2];
extern int32 c_rcc_dpy[2];

extern uint8 pi_cc_command;
extern uint8 pi_cc_floor;
extern uint8 pi_location;
extern uint8 pi_cc_ctcan_cnt;
extern uint8 pi_cc_update;



// Prototypes of can communications board funtions
extern int16 ctcan_put_dev_msg_req (int16 dev, int8 msg);
extern void init_ctcan_bus (void);
extern void ctcan_control (void);
extern void clr_ctcan_dev_comm_loss (void);
extern void set_pi_lant_status (void);
#if (Traction == 1)
extern int16 blind (void);
#endif
