extern struct tag_keb_drv_par_tbl {
	int16 len;
	struct tag_par {
		int16 digit;
		int16 min;
		int32 max;
		char name [25];
		char unit [5];
		char text_seq;
		int16 address;
		unsigned char set;
		unsigned char rd_wr;
		unsigned char dpy;
		unsigned char par_name_len;
	} par[30];
} keb_drv_par_tbl[14]; 
extern void Adjust_KEB_Drive_Parameters (void);

extern void verify_run_mode (void);
