
//*********************************
// User Comm Definitions
//*********************************

// buffer sizes	
#define rbufsize 1000
#define tbufsize 3000

extern int16 com_rx_char_cnt[];
extern int16 good_packet[];
extern int16 bad_packet[];
extern int16 dma_start_cnt[];
extern int16 dma_end_cnt[];


extern int16 com_1_count;
extern int32 com_1_source;
extern int32 com_1_modem_status;
extern int32 com_1_line_status;
extern int16 com_2_count;
extern int32 com_2_source;
extern int32 com_2_modem_status;
extern int32 com_2_line_status;


// User Comm Variables
extern int16 Com_Mode[];
extern int16 com_nmb_fl[];
extern int16 com_nmb_car[];
extern int16 com_nmb_io[];
extern int16 grp_Car_Stat_req[];
extern char unsigned cc_req_fl[], rcc_req_fl[];		// hold's car call request floor number
extern int16 comm_baud[];	// default baud rate for user and drive comm interface
extern int16 baud_tbl[];


// character strings used for communications
extern unsigned char rmbuf[][rbufsize+1];             // com 1 recieve message buffer
extern unsigned char tranbuf[][tbufsize+1];  // com 1 transmit buffer
extern unsigned char recbuf[][rbufsize+1];   // com 1 receive buffer
extern int16 mpnt[];            // message pointer for com 1-2
extern int16 rbs[]; // receive buffer start pointer com 1-2
extern int16 rbf[]; // receive buffer end pointer com 1-2
extern int16 tbs[];             // trasmit buffer start pointer com 1-2
extern int16 tbf[];             // trasmit buffer end pointer com 1-2
extern int16 Com_Tran_Int[];	// comunication port transmit interupt running
extern int16 Com_Recv_Int[];	// comunication port transmit interupt running
extern unsigned char Tranbuf_Empty[]; // Com 1-2 Transmiter buffer is empty if 1 has chars if 0
extern unsigned char Tranbuf_Full[];	// Com Transmit buffer is full if 1 and 0 if not
extern int16 Try_Restart[]; // try to restablish commuiniction with other com devices

extern int16 logon[];          	// logon = 0 no activity on com port #1
extern int16 User_Log[];
extern int16 connect[];
extern int16 modem_status[];
extern int16 modem_dcd[];
extern int16 modem_state[];
extern int16 init_modem[];
extern int16 user_com_ver[];			// default= 0, Increment with each comm change
extern int16 user_com_type[];			// User type: 1 = galcom, 2 = GALileo

extern int16 com_version;

// Comm security variables for com1 and com2 (note com3 is the drive comm)

extern unsigned char Com_Add_Sec_Fl_Req[];					// Send security floor to add code
extern unsigned char Com_Add_Sec_Code[][4];				// Security Code to Add a code
extern unsigned char Com_Del_Sec_Fl_Req[];					// Send Security floor to delete code
extern unsigned char Com_Del_Sec_Code[][4];				// Security code to delete a code
extern unsigned char Com_Del_All_Codes_Fl_Req[];			// Delete all codes for one floor or all floors
extern unsigned char Com_Sec_Fl_Tbl_Req[]; 				// Floor request for security table
extern int16 Com_LookUpTable[][NumMaxFl];
extern int16 Com_Sec_Chksum_Fl[];							// Floor start for Security Checksum


// User Interface UI security flags
extern int16 ui_sec;
extern int32 ui_uc_sec[];
extern int32 ui_ruc_sec[];
extern int32 ui_dc_sec[];
extern int32 ui_rdc_sec[];
extern int32 ui_cc_sec[][car_size];
extern int32 ui_cc_sec_prev[][car_size];

extern int32 ui_rcc_sec[][car_size];
extern int32 ui_rcc_sec_prev[][car_size];

extern int32 ui_uhc_carsec[][9];
extern int32 ui_dhc_carsec[][9];
extern int32 ui_ruhc_carsec[][9];
extern int32 ui_rdhc_carsec[][9];


// User Interface UI control flags
extern int16 ui_ind[];
extern int16 ui_rtl[];
extern int16 ui_heof[];
extern int16 ui_ad_off[];
extern int16 ui_uppeak;
extern int16 ui_dnpeak;
extern int16 ui_alt_lby;
extern int16 ui_alt_park;
extern int16 ui_ncu;
extern int16 ui_alt_fl_sec;


extern int16 const com[][12];

extern int16 asigetc(int16);
extern int16 isrxempty(int16);

extern void initcom(void );

//lint -esym(534,put_pkt_req)
extern int16 put_pkt_req(int16 pkt_nmb, int16 addr, int16 com_nmb);
extern void put_pkt_req_all_cars(int16 pkt_nmb);
extern int16 get_pkt_req(int16 addr, int16 com_nmb);
extern int16 get_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb, int16 time);
extern void put_pkt_status(int16 pkt_nmb, int16 addr, int16 com_nmb);

extern void calc_can_devices_color_ix_chksum(void);
extern void zero_req_que(void); 
extern void zero_status_que(void);
extern int16 recvm(int16);
extern int16 istxempty( int16 comnmb);
extern void sendc( int16, int16);
extern void Restart_Com(int16 );

extern void com_1_dma_trm_int(void);
extern void com_1_tx_start(int16 len);
extern void com_2_dma_trm_int(void);
extern void com_2_tx_start(int16 len);

