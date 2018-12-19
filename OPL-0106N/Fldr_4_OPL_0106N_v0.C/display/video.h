// Variables for Video Screen
extern int16 dpy_mode;			// Selects the display screen
						// 0 = menu
						// 1 = standard job screen
						// 2 = Job statistics
						// 3 = Security display
						// 4 = debug display

extern int16 video_timeout;		//	video timout
extern int16 save_dpy_mode;
extern int16 visible_cmd;
extern int16 video_off;
extern int16 video_exist;
extern int16 Car_Select;
extern int16 Car_Select_MRdpy;
extern int16 Car_IO_Select;
extern int16 io_page;
extern int16 grp_page;
extern int16 nmbhcb;
extern int16 next_page_hcb_2;		// hall call board number to start page	2
extern int16 Display_Trace;
extern int16 Trace_Track_Rotate_Ix;

extern const int16 fixed_io_brd_dpy;

#if (Tract_HR == 1)
extern int16 next_page_hcb_3;		// hall call board number to start page 3
extern int16 next_page_hcb_4;		// hall call board number to start page	4
extern int16 next_page_hcb_5;		// hall call board number to start page	5
#endif
extern int16 nmbgio_page;		// Number of group io pages
extern int16 dbg_page;
extern int16 grtop_rfl;
extern int16 grbot_rfl;
extern int16 page_2_brd_nmb[car_size];
extern int16 page_3_brd_nmb[car_size];
#if (Tract_HR == 1)
extern int16 page_4_brd_nmb[car_size];
extern int16 page_5_brd_nmb[car_size];
#endif
extern int16 max_nmb_io_brd[car_size];
extern int16 iobd_rotate_start[car_size];
extern int16 last_io_brd[car_size];

extern int drv_par_group;
extern int tst_drv_par_ret;

extern grp_par_max[14];
extern const char call_status_dpy[20][51];
extern const char Service[35][20];            // service flag
extern const char servf_tbl[35][4];

extern void Init_Video(void);
extern void Init_Debug_Screen(int16 init_flag);
extern void video_display_control(void);
extern int16 get_comm_status (int16 board_nmb);	  //       0 = no comm, 1 = comm with board,  2 = comm on previous board

struct lcD_line_1_struct{
	char dpy[50][25]; 
	unsigned char chksum[5];
	int16 nmb_items;
};

struct lcD_line_2_struct{
	char dpy[300][25]; 
	unsigned char chksum[30];
	int16 nmb_items;
};

struct lcD_line_3_struct{
	char dpy[600][25]; 
	unsigned char chksum[60];
	int16 nmb_items;
};

struct IO_screen_lcD_struct{
	struct lcD_line_1_struct line1;
	struct lcD_line_2_struct line2;
	struct lcD_line_3_struct line3;
};

extern struct IO_screen_lcD_struct IO_scn_lcD;

extern void init_IO_scn_lcD (void);
extern void calculate_IO_scn_checksums(void);

