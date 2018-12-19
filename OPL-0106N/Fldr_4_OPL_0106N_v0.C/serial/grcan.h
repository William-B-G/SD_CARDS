#define c_max_grcan_dev 65	// 55 devices 
// #define c_max_grcan_io_brd 40	// 40 i/o boards *** use nmb_hcio_brds instead
#define c_max_grcan_ser_array (nmb_hcio_brds * 3)

#define  c_grcan_START	  1		  // Can start index
#define  c_grcan_VS		  1		  // Voltage Sensor
#define  c_grcan_FI_DPY   2		  // Serial PI and Lantern (global address for all pi and lantern devices)
#define	 c_grcan_SEB_1	  3		  // Serial I/O Expansion board 1	(io brd 7)
#define	 c_grcan_SEB_2	  4		  // Serial I/O Expansion board 2	(io brd 8)
#define	 c_grcan_SEB_3	  5		  // Serial I/O Expansion board 3	(io brd 9)
#define	 c_grcan_SEB_4	  6		  // Serial I/O Expansion board 4	(io brd 10)
#define	 c_grcan_SEB_5	  7		  // Serial I/O Expansion board 5	(io brd 11)
#define	 c_grcan_SEB_6	  8		  // Serial I/O Expansion board 6	(io brd 12)
#define	 c_grcan_SEB_7	  9		  // Serial I/O Expansion board 7	(io brd 13)
#define  c_grcan_SEB_8	 10		  // Serial I/O Expansion board 8   (io brd 14)
#define  c_grcan_SEB_9	 11		  // Serial I/O Expansion board 9   (io brd 15)
#define  c_grcan_SEB_10	 12		  // Serial I/O Expansion board 10  (io brd 16)
#define  c_grcan_SEB_11	 13		  // Serial I/O Expansion board 11  (io brd 17)
#define  c_grcan_SEB_12	 14		  // Serial I/O Expansion board 12  (io brd 18) 
#define  c_grcan_SEB_13	 15		  // Serial I/O Expansion board 13  (io brd 19) 
#define  c_grcan_SEB_14	 16		  // Serial I/O Expansion board 14  (io brd 20) 
#define  c_grcan_SEB_15	 17		  // Serial I/O Expansion board 15  (io brd 21) 
#define  c_grcan_SEB_16	 18		  // Serial I/O Expansion board 16  (io brd 22) 
#define  c_grcan_SEB_17	 19		  // Serial I/O Expansion board 17  (io brd 23) 
#define  c_grcan_SEB_18	 20		  // Serial I/O Expansion board 18  (io brd 24) 
#define  c_grcan_SEB_19	 21		  // Serial I/O Expansion board 19  (io brd 25) 
#define  c_grcan_SEB_20	 22		  // Serial I/O Expansion board 20  (io brd 26)
#define  c_grcan_SEB_21	 23		  // Serial I/O Expansion board 21  (io brd 27)
#define  c_grcan_SEB_22	 24		  // Serial I/O Expansion board 22  (io brd 28)
#define  c_grcan_SEB_23	 25		  // Serial I/O Expansion board 23  (io brd 29) 
#define  c_grcan_SEB_24	 26		  // Serial I/O Expansion board 24  (io brd 30) 
#define  c_grcan_SEB_25	 27		  // Serial I/O Expansion board 25  (io brd 31) 
#define  c_grcan_SEB_26	 28		  // Serial I/O Expansion board 26  (io brd 32) 
#define  c_grcan_SEB_27	 29		  // Serial I/O Expansion board 27  (io brd 33) 
#define  c_grcan_SEB_28	 30		  // Serial I/O Expansion board 28  (io brd 34) 
#define  c_grcan_SEB_29	 31		  // Serial I/O Expansion board 29  (io brd 35) 
#define  c_grcan_SEB_30	 32		  // Serial I/O Expansion board 30  (io brd 36)
#define  c_grcan_SEB_31	 33		  // Serial I/O Expansion board 31  (io brd 37)
#define  c_grcan_SEB_32	 34		  // Serial I/O Expansion board 32  (io brd 38)
#define  c_grcan_SEB_33	 35		  // Serial I/O Expansion board 33  (io brd 39) 
#define  c_grcan_SEB_34	 36		  // Serial I/O Expansion board 34  (io brd 40) 
#define  c_grcan_SEB_35	 37		  // Serial I/O Expansion board 35  (io brd 41) 
#define  c_grcan_SEB_36	 38		  // Serial I/O Expansion board 36  (io brd 42) 
#define  c_grcan_SEB_37	 39		  // Serial I/O Expansion board 37  (io brd 43) 
#define  c_grcan_SEB_38	 40		  // Serial I/O Expansion board 38  (io brd 44) 
#define  c_grcan_SEB_39	 41		  // Serial I/O Expansion board 39  (io brd 45) 
#define  c_grcan_SEB_40	 42		  // Serial I/O Expansion board 40  (io brd 46)
#define  c_grcan_HCDB_1	 43		  // Hall Call Driver board 1
#define  c_grcan_HCDB_2	 44		  // Hall Call Driver board 1
#define  c_grcan_HCDB_3	 45		  // Hall Call Driver board 1
#define  c_grcan_HCDB_4	 46		  // Hall Call Driver board 1
#define  c_grcan_HCDB_5	 47		  // Hall Call Driver board 1
#define  c_grcan_HCDB_6	 48		  // Hall Call Driver board 1
#define  c_grcan_HCDB_7	 49		  // Hall Call Driver board 1
#define  c_grcan_HCDB_8	 50		  // Hall Call Driver board 1
#define  c_grcan_HCDB_9	 51		  // Hall Call Driver board 1
#define  c_grcan_HCDB_10 52		  // Hall Call Driver board 1
#define  c_grcan_HCB_1	 15		  // Hall Call Board 1
#define  c_grcan_HCB_2	 16		  // Hall Call Board 2
#define  c_grcan_HCB_3	 17		  // Hall Call Board 3
#define  c_grcan_HCB_4	 18		  // Hall Call Board 4
#define  c_grcan_HCB_5	 19		  // Hall Call Board 5
#define  c_grcan_HCB_6	 20		  // Hall Call Board 6
#define  c_grcan_HCB_7	 21		  // Hall Call Board 7
#define  c_grcan_HCB_8	 22		  // Hall Call Board 8
#define  c_grcan_HCB_9	 23		  // Hall Call Board 9
#define  c_grcan_HCB_10	 24		  // Hall Call Board 10
#define  c_grcan_HCB_11	 25		  // Hall Call Board 11
#define  c_grcan_HCB_12	 26		  // Hall Call Board 12
#define  c_grcan_HCB_13	 27		  // Hall Call Board 13
#define  c_grcan_HCB_14	 28		  // Hall Call Board 14
#define  c_grcan_HCB_15	 29		  // Hall Call Board 15
#define  c_grcan_HCB_16	 30		  // Hall Call Board 16
#define  c_grcan_HCB_17	 31		  // Hall Call Board 17
#define  c_grcan_HCB_18	 32		  // Hall Call Board 18
#define  c_grcan_HCB_19	 33		  // Hall Call Board 19
#define  c_grcan_HCB_20	 34		  // Hall Call Board 20
#define  c_grcan_HCB_21	 35		  // Hall Call Board 21
#define  c_grcan_HCB_22	 36		  // Hall Call Board 22
#define  c_grcan_HCB_23	 37		  // Hall Call Board 23
#define  c_grcan_HCB_24	 38		  // Hall Call Board 24
#define  c_grcan_HCB_25	 39		  // Hall Call Board 25
#define  c_grcan_HCB_26	 40		  // Hall Call Board 26
#define  c_grcan_HCB_27	 41		  // Hall Call Board 27
#define  c_grcan_HCB_28	 42		  // Hall Call Board 28
#define  c_grcan_HCB_29	 43		  // Hall Call Board 29
#define  c_grcan_HCB_30	 44		  // Hall Call Board 30
#define  c_grcan_HCB_31	 45		  // Hall Call Board 31
#define  c_grcan_HCB_32	 46		  // Hall Call Board 32
#define  c_grcan_HCB_33	 47		  // Hall Call Board 33
#define  c_grcan_HCB_34	 48		  // Hall Call Board 34
#define  c_grcan_HCB_35	 49		  // Hall Call Board 35
#define  c_grcan_HCB_36	 50		  // Hall Call Board 36
#define  c_grcan_HCB_37	 51		  // Hall Call Board 37
#define  c_grcan_HCB_38	 52		  // Hall Call Board 38
#define  c_grcan_HCB_39	 53		  // Hall Call Board 39
#define  c_grcan_HCB_40	 54		  // Hall Call Board 40
#define  c_grcan_HCB_41	 55		  // Hall Call Board 41
#define  c_grcan_HCB_42	 56		  // Hall Call Board 42
#define  c_grcan_HCB_43	 57		  // Hall Call Board 43
#define  c_grcan_HCB_44	 58		  // Hall Call Board 44
#define  c_grcan_HCB_45	 59		  // Hall Call Board 45
#define  c_grcan_HCB_46	 60		  // Hall Call Board 46
#define  c_grcan_HCB_47	 61		  // Hall Call Board 47
#define  c_grcan_HCB_48	 62		  // Hall Call Board 48
#define  c_grcan_END	 62		  // Last can device
#define  c_grcan_GLOBAL	 63		  // Global can address

extern int16 wbg_grp_pack_num[];
extern int16 wbg_grp_pack_index;
extern int16 grcan_addr[c_max_grcan_dev];

extern  int16 wbg_grp_pack_num[];
extern int16 wbg_grp_pack_index ;
extern int16 Total_Packs;
extern int16 Hall_Station_Addr;

extern union tag_ser_grc {
	
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
	} dev [nmb_hcio_brds];	

	struct
	{
		uint8 ihc[6];
		uint8 ohc[6];
		uint8 axihc[3];
		uint8 axohc[3];
	} devhc [nmb_hcio_brds/6];
		
	unsigned char brd_io [c_max_grcan_ser_array];
}serial_grc;


extern union tag_sync_grc {
	struct
	{
		unsigned char iob[3];
		unsigned char reflect_data;
		unsigned char rtx_cnt;
	} dev[nmb_hcio_brds];
	
	struct
	{
		uint8 ihc[6];
		uint8 ohc[6];
		uint8 axihc[3];
		uint8 axohc[3];
		unsigned char reflect_hc_data;
		unsigned char rtx_hc_cnt;
	} devhc [nmb_hcio_brds/6];
	
} sync_grc;

#define grcan_que_len 3

extern struct tag_grcan_dev {
		unsigned char online;
		unsigned char online_cnt;
		unsigned char version;
		unsigned char revision;
		unsigned char msg_req[grcan_que_len];
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
} grcan_dev[c_max_grcan_dev];

extern bool grcan_car_online[car_size];
extern bool car_on_grcan;

extern unsigned char grc_ioset[c_max_grcan_ser_array];
extern unsigned char grc_ioclr[c_max_grcan_ser_array];

extern unsigned char grcan_dev_list [c_max_grcan_dev];
extern int16 nmb_grcan_ports;
extern int16 nmb_grcan_dev;
extern int16 nmb_dvr_brds;
extern int16 grcan_init_count;
extern int32 grcan_status;
extern int16 grcan_mode;
extern int16 grcan_int_status;
extern int16 grcan_error;
extern int16 grcan_int_count;
extern int16 re_init_grcan;
extern int16 grcan_arb_lost_cap;
extern int16 grcan_err_code_cap;
extern int16 grcan_tx_err_cnt;
extern int16 grcan_rx_err_cnt;
extern int16 grcan_BEI_cnt;
extern int16 grcan_ALI_cnt;
extern int16 grcan_EPI_cnt;
extern int16 grcan_WUI_cnt;
extern int16 grcan_DOI_cnt;
extern int16 grcan_EWI_cnt;
extern int16 grcan_TI_cnt;
extern int16 grcan_RI_cnt;

extern int32 grcan_debug_1;
extern int32 grcan_debug_2;
extern int32 grcan_debug_3;
extern int32 grcan_debug_4;
extern int32 grcan_debug_5;
extern int32 grcan_debug_6;


#define c_grcan_tx_buf_siz 30

extern struct tag_grcan_tx {
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
		} grcan_tx[c_grcan_tx_buf_siz+1];
		
extern int16 grc_tpix;					// can transmit buffer put index
extern int16 grc_tgix;					// can transmit buffer get index
		
#define c_grcan_rx_buf_siz 30

extern struct tag_grcan_rx {
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
		} grcan_rx[c_grcan_rx_buf_siz + 1];
		
extern int16 grc_rpix;					// can transmit buffer put index
extern int16 grc_rgix;					// can transmit buffer get index

#define c_hc_brds_per_dvr 24			// Number of hall call boards per driver board

extern int16 hcbd_flt_ix;

extern struct tag_hc_dvr_brd {
		unsigned char status_1;
		unsigned char prev_status_1;			// memory of previous fault
		unsigned char status_2;					
		unsigned char online;						// set to 1 if one line				
		unsigned char fault[c_hc_brds_per_dvr];
		unsigned char prev_fault[c_hc_brds_per_dvr];		// memory of previous faults
		unsigned char version[c_hc_brds_per_dvr];
		unsigned char revision[c_hc_brds_per_dvr];			
		unsigned char init_seq;
		unsigned char init_count;		
		unsigned char upd_seq;								// update sequence
		unsigned char color_seq;							// color update sequence
		unsigned char riser;								// riser: 0 = std, 1 = ir, 2= cb, 3 = vip  
		} hc_dvr_brd[nmb_hc_dvr_brds];

extern int16 HcbControl[nmb_hc_dvr_brds];		// Hall Call Button Control 					 
								//							bit0: 1 = flash Security
								//                          bit1: 1 = Reset HCB Stations
								//							bit2: 1 = Master Sec Enable					 
								//							bits 3-6 = unused
								//							bit7 = Reset and init hall staions boards	



// Prototypes of can communications board funtions
extern int16 grcan_put_dev_msg_req (int16 dev, int8 msg);
extern void init_grcan_bus (void);
extern void grcan_control (void);
extern int16 get_comm_status (int16 board_nmb);
extern void clr_grcan_dev_comm_loss (void);
extern void send_hc_dvr_req (int16 cmd);

