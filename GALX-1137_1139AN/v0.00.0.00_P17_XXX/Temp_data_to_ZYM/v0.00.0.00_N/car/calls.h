
extern int32 c_up_hall_call[2];
extern int32 c_dn_hall_call[2];
extern int32 c_upr_hall_call[2];
extern int32 c_dnr_hall_call[2];
extern int32 c_cc_req[2];
extern int32 c_rcc_req[2];


extern int32 c_irup_hall_call[2];
extern int32 c_irdn_hall_call[2];
extern int32 c_rirup_hall_call[2];
extern int32 c_rirdn_hall_call[2];

extern int16 cancel_up;
extern int16 cancel_dn;
extern int16 cancel_upr;
extern int16 cancel_dnr;

extern int16 cancel_irup;
extern int16 cancel_irdn;
extern int16 cancel_rirup;
extern int16 cancel_rirdn;

extern int16 cancel_trup;
extern int16 cancel_trdn;
extern int16 cancel_rtrup;
extern int16 cancel_rtrdn;

// Car call test variables
extern int32 cc_test_mask[2];		// Car call test mask
extern int16 enable_cc_test;
extern int16 continue_cc_test;
extern int16 cancel_cc_test;
extern int16 sab_cc_topff;
extern int16 sab_cc_topfr; 
extern int16 sab_cc_botff;
extern int16 sab_cc_botfr; 

extern int16 sabbath_rst_msk;
extern int16 sabbath_auto_calls;

extern void sabbath_std_svc(void);
extern void sabbath_down_col_svc(void);
extern void sabbath_up_col_svc(void);
extern void sab_up_dn_col_cc_msk_swap(void);
extern void sabbath_tables_cc(void);
extern void sabbath_odd_even_svc(void);
extern void sabbath_start_at_lobby(void);
extern int16 set_carcall(int16 car_no, int16 fl_no);
extern void set_upcall(int16 fl_no);
extern void set_dncall(int16 fl_no);
extern void set_rcarcall(int16 car_no, int16 fl_no);
extern void set_rupcall(int16 fl_no);
extern void set_rdncall(int16 fl_no);
extern void get_onward_calls (void);
extern void carcall_test(void);
extern void next_stop_lobby(void);
extern void carcall(void );
extern void cclatch(void);
extern void carcancel(void );
extern void sec_cc_latch (void);
extern void clr_sec_cc_latch (int16 select);
extern void clrcc(int16 select );
extern void clrhc(void );
extern void hallcall(void );
extern void hallcancel(void );
extern void set_ir_dncall(int16 fl_no);
extern void set_ir_upcall(int16 fl_no);

