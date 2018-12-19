#define c_max_mrcan_dev 55	// 55 devices 
#define c_max_mrcan_io_brd 40	// 40 i/o boards 
#define c_max_mrcan_ser_array (c_max_mrcan_io_brd * 3)

#define  c_mrcan_START	  1		  // Can start index
#define  c_mrcan_NTS	  1		  // NTS Normal Terminal Slowdown CPU
#define  c_mrcan_SPB	  2		  // Safety processor board
#define  c_mrcan_LW		  3		  // Load weigher
#define  c_mrcan_VS		  4		  // Voltage Sensor
#define  c_mrcan_PI_LANT  5		  // Serial PI and Lantern (global address for all pi and lantern devices)
#define	 c_mrcan_SEB_1	  6		  // Serial I/O Expansion board 1	(io brd 7)
#define	 c_mrcan_SEB_2	  7		  // Serial I/O Expansion board 2	(io brd 8)
#define	 c_mrcan_SEB_3	  8		  // Serial I/O Expansion board 3	(io brd 9)
#define	 c_mrcan_SEB_4	  9		  // Serial I/O Expansion board 4	(io brd 10)
#define	 c_mrcan_SEB_5	 10		  // Serial I/O Expansion board 5	(io brd 11)
#define	 c_mrcan_SEB_6	 11		  // Serial I/O Expansion board 6	(io brd 12)
#define	 c_mrcan_SEB_7	 12		  // Serial I/O Expansion board 7	(io brd 13)
#define  c_mrcan_SEB_8	 13		  // Serial I/O Expansion board 8   (io brd 14)
#define  c_mrcan_SEB_9	 14		  // Serial I/O Expansion board 9   (io brd 15)
#define  c_mrcan_SEB_10	 15		  // Serial I/O Expansion board 10  (io brd 16)
#define  c_mrcan_SEB_11	 16		  // Serial I/O Expansion board 11  (io brd 17)
#define  c_mrcan_SEB_12	 17		  // Serial I/O Expansion board 12  (io brd 18) 
#define  c_mrcan_SEB_13	 18		  // Serial I/O Expansion board 13  (io brd 19) 
#define  c_mrcan_SEB_14	 19		  // Serial I/O Expansion board 14  (io brd 20) 
#define  c_mrcan_SEB_15	 20		  // Serial I/O Expansion board 15  (io brd 21) 
#define  c_mrcan_SEB_16	 21		  // Serial I/O Expansion board 16  (io brd 22) 
#define  c_mrcan_SEB_17	 22		  // Serial I/O Expansion board 17  (io brd 23) 
#define  c_mrcan_SEB_18	 23		  // Serial I/O Expansion board 18  (io brd 24) 
#define  c_mrcan_SEB_19	 24		  // Serial I/O Expansion board 19  (io brd 25) 
#define  c_mrcan_SEB_20	 25		  // Serial I/O Expansion board 20  (io brd 26)
#define  c_mrcan_SEB_21	 26		  // Serial I/O Expansion board 21  (io brd 27)
#define  c_mrcan_SEB_22	 27		  // Serial I/O Expansion board 22  (io brd 28)
#define  c_mrcan_SEB_23	 28		  // Serial I/O Expansion board 23  (io brd 29) 
#define  c_mrcan_SEB_24	 29		  // Serial I/O Expansion board 24  (io brd 30) 
#define  c_mrcan_SEB_25	 30		  // Serial I/O Expansion board 25  (io brd 31) 
#define  c_mrcan_SEB_26	 31		  // Serial I/O Expansion board 26  (io brd 32) 
#define  c_mrcan_SEB_27	 32		  // Serial I/O Expansion board 27  (io brd 33) 
#define  c_mrcan_SEB_28	 33		  // Serial I/O Expansion board 28  (io brd 34) 
#define  c_mrcan_SEB_29	 34		  // Serial I/O Expansion board 29  (io brd 35) 
#define  c_mrcan_SEB_30	 35		  // Serial I/O Expansion board 30  (io brd 36)
#define  c_mrcan_SEB_31	 36		  // Serial I/O Expansion board 31  (io brd 37)
#define  c_mrcan_SEB_32	 37		  // Serial I/O Expansion board 32  (io brd 38)
#define  c_mrcan_SEB_33	 38		  // Serial I/O Expansion board 33  (io brd 39) 
#define  c_mrcan_SEB_34	 39		  // Serial I/O Expansion board 34  (io brd 40) 
#define  c_mrcan_SEB_35	 40		  // Serial I/O Expansion board 35  (io brd 41) 
#define  c_mrcan_SEB_36	 41		  // Serial I/O Expansion board 36  (io brd 42) 
#define  c_mrcan_SEB_37	 42		  // Serial I/O Expansion board 37  (io brd 43) 
#define  c_mrcan_SEB_38	 43		  // Serial I/O Expansion board 38  (io brd 44) 
#define  c_mrcan_SEB_39	 44		  // Serial I/O Expansion board 39  (io brd 45) 
#define  c_mrcan_SEB_40	 44		  // Serial I/O Expansion board 40  (io brd 46)
#define  c_mrcan_END	 45		  // Last can device
#define  c_mrcan_GLOBAL	 46		  // Global can address

extern int16 mrcan_addr[c_max_mrcan_dev];


extern union tag_ser_mr {

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
	} dev [c_max_mrcan_io_brd];	
	
	unsigned char brd_io [c_max_mrcan_ser_array];
}serial_mrc;

extern  struct tag_sync_mrc {
		unsigned char iob[3];
		unsigned char reflect_data;
		unsigned char rtx_cnt;
} sync_dev_mrc [c_max_mrcan_io_brd];

#define mtcan_que_len 3

extern struct tag_mrcan_dev {
		unsigned char online;
		unsigned char online_cnt;
		unsigned char version;
		unsigned char revision;
		unsigned char msg_req[mtcan_que_len];
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
} mrcan_dev[c_max_mrcan_dev];

extern unsigned char mrc_ioset[c_max_mrcan_ser_array];
extern unsigned char mrc_ioclr[c_max_mrcan_ser_array];

extern unsigned char mrcan_dev_list [c_max_mrcan_dev];
extern int16 nmb_mrcan_ports;
extern int16 nmb_mrcan_dev;
extern int16 mrcan_init_count;
extern int32 mrcan_status;
extern int16 mrcan_mode;
extern int16 mrcan_int_status;
extern int16 mrcan_error;
extern int16 mrcan_int_count;
extern int16 re_init_mrcan;
extern int16 mrcan_arb_lost_cap;
extern int16 mrcan_err_code_cap;
extern int16 mrcan_tx_err_cnt;
extern int16 mrcan_rx_err_cnt;
extern int16 mrcan_BEI_cnt;
extern int16 mrcan_ALI_cnt;
extern int16 mrcan_EPI_cnt;
extern int16 mrcan_WUI_cnt;
extern int16 mrcan_DOI_cnt;
extern int16 mrcan_EWI_cnt;
extern int16 mrcan_TI_cnt;
extern int16 mrcan_RI_cnt;

extern int32 mrcan_debug_1;
extern int32 mrcan_debug_2;
extern int32 mrcan_debug_3;
extern int32 mrcan_debug_4;
extern int32 mrcan_debug_5;
extern int32 mrcan_debug_6;


#define c_mrcan_tx_buf_siz 30

extern struct tag_mrcan_tx {
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
		} mrcan_tx[c_mrcan_tx_buf_siz+1];
		
extern int16 mrc_tpix;					// can transmit buffer put index
extern int16 mrc_tgix;					// can transmit buffer get index
		
#define c_mrcan_rx_buf_siz 30

extern struct tag_mrcan_rx {
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
		} mrcan_rx[c_mrcan_rx_buf_siz + 1];
		
extern int16 mrc_rpix;					// can transmit buffer put index
extern int16 mrc_rgix;					// can transmit buffer get index


extern uint8 upd_pi_lant_mrcan;
extern uint8 pi_cc_mrcan_cnt;
extern uint8 pi_cc_mrcan_command;
extern uint8 pi_cc_mrcan_floor;
extern uint8 pi_mrcan_location;
extern uint8 pi_cc_mrcan_cnt;
extern uint8 pi_cc_mrcan_update;


// Prototypes of can communications board funtions
extern int16 mrcan_put_dev_msg_req (int16 dev, int8 msg);
extern void init_mrcan_bus (void);
extern void mrcan_control (void);
extern void clr_mrcan_dev_comm_loss (void);

