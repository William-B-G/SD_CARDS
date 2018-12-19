
/*
// I/O Port selection variables

extern int16 io_sel;
extern int16 a2004_io_sel;
extern int16 ss_io_sel;
extern int16 lps_io_sel;
extern int16 fdoor_io_sel;
extern int16 rdoor_io_sel;
extern int16 ee_io_sel;
extern int16 eer_io_sel;
extern int16 hvd_io_sel;
extern int16 f_rm_io_sel;
extern int16 r_rm_io_sel;
extern int16 tdc_io_sel;
extern int16 bdc_io_sel;
extern int16 mdc_io_sel;
extern int16 rcm_io_sel;
extern int16 ct_io_sel;
extern int16 dlw_io_sel;
extern int16 eq_io_sel;
extern int16 los_io_sel;
extern int16 rtldc_io_sel;
extern int16 clant_io_sel;
extern int16 rclant_io_sel;
extern int16 iserv_io_sel;
extern int16 oserl_io_sel;
extern int16 eor_io_sel;
extern int16 mbc_io_sel;
extern int16 rcc_io_sel;
extern int16 alrm_io_sel;
extern int16 mlv_io_sel;
extern int16 hemp_io_sel;
extern int16 ept_io_sel;
extern int16 eplh_io_sel;
extern int16 hfire_io_sel;
extern int16 flh_io_sel;
extern int16 mrie_io_sel;
extern int16 ebks_io_sel;
extern int16 servo_io_sel;

extern int16 dza_io_sel;
extern int16 dz_io_sel;
extern int16 lev_io_sel;
extern int16 up_limit_io_sel;
extern int16 dn_limit_io_sel;

extern int16 fbz_io_sel;

extern int16 rtl_io_sel;
extern int16 heof_io_sel;
extern int16 rtlh_io_sel;
extern int16 emsh_io_sel;
extern int16 emlh_io_sel;

*/
extern unsigned char IO_In_Out[nmb_ios];
extern unsigned char ioset[nmb_ios];
extern unsigned char ioclr[nmb_ios];
extern unsigned char iodata[car_size][nmb_ios];
extern unsigned char IO_board[car_size][nmb_io_brds];


#if (Tract_HR == 1)

extern int16 io_bd_order[53];
extern int16 brdadd[6][3];
extern int16 brdtype[53];
extern int16 HC_IO_board[c_max_hc_boards];

#else

extern int16 io_bd_order[33];
extern int16 brdadd[6][3];
extern int16 brdtype[33];
extern int16 HC_IO_board[c_max_hc_boards];
#endif

extern char  ioname [][5];
extern unsigned char  io_valid[];
extern unsigned char  io_send_name[];
extern unsigned char io_on_color_ix[];
extern char  hcioname [][5];
extern unsigned char  gio_valid[];


extern void io_init(void);
extern void init_iodata(void);
extern int16 count_bit(unsigned char bitnmb);
