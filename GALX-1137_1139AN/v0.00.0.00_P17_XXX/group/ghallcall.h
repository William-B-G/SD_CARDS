
// hall calls
extern int32 g_up_hall_call[2][car_size];
extern int32 g_dn_hall_call[2][car_size];
extern int32 g_upr_hall_call[2][car_size];
extern int32 g_dnr_hall_call[2][car_size];

extern int32 g_cc_req[2][car_size];		  		// car call request for car
extern int32 g_rcc_req[2][car_size];
extern int32 g_cc_dpy[2][car_size];		  		// car call destination for car
extern int32 g_rcc_dpy[2][car_size];


extern int16 nmb_up_call[100][fl_size];
extern int16 nmb_dn_call[100][fl_size];
extern int16 g_asign_up[fl_size];
extern int16 g_asign_dn[fl_size];
extern int16 g_asign_upr[fl_size];
extern int16 g_asign_dnr[fl_size];

extern bool gc_nca_xasgn;					// input for no cars available from other group

// Lobby Up call Request varibles
extern uint8 lby_up_req;
extern int16 lby_up_req_car[car_size];
extern int16 lby_up_assign;


extern int16 car_asgn_uc[fl_size];
extern int16 car_asgn_dc[fl_size];
extern int16 car_asgn_cc[fl_size];
extern int16 car_asgn_ruc[fl_size];
extern int16 car_asgn_rdc[fl_size];
extern int16 car_asgn_rcc[fl_size];
extern int16 car_asgn_sec;

// Que to collect call in the order received (front and rear collected together) rear Or'ed with 0x80
extern unsigned char up_hc_que[c_hc_que_len];
extern unsigned char dn_hc_que[c_hc_que_len];

// Traffic timers
extern int16 dctim[fl_size];          // down hall call timers
extern int16 uctim[fl_size];          // up hall call timers
extern int16 dcrtim[fl_size];          // down hall call timers
extern int16 ucrtim[fl_size];          // up hall call timers

// group hall call functions

extern void grp_clrhc(void );
extern void grp_clr_canc();
extern void group_hallcall(void );
extern int16 car_at_fl_cancel(int16 cancel_dir,int16 fl);
extern int16 place_uphc_in_que(int16 fl);
extern int16 remove_uphc_from_que(int16 fl);
extern int16 is_uphc_in_que(int16 fl);
extern void clr_uphc_que(void);
extern int16 place_dnhc_in_que(int16 fl);
extern int16 remove_dnhc_from_que(int16 fl);
extern int16 is_dnhc_in_que(int16 fl);
extern void clr_dnhc_que(void);
extern void latch_hallcall(void);
extern void latch_ser_hallcall(void);
extern void assign_hallcall (void);
extern int16 keep_assignment (int16 assigned_car, int16 new_assign_car, int16 fl, int16 dir, int16 front_rear);
extern int16 make_assignment (int16 assigned_car, int16 new_assign_car, int16 fl, int16 dir, int16 front_rear);
extern void group_handicap_car_floor(void);
extern void hall_call_cross_assignment (void);
extern void group_hallcancel(void );

