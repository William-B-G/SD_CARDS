extern int16 ir_car;	// inconspicious riser car
extern int16 ir_active;
extern int16 ir_select;
extern int16 prev_ir_active;
extern int16 ir_call_enable;
extern int16 sr_car;

extern int32 g_irup_hall_call[2][car_size];
extern int32 g_irdn_hall_call[2][car_size];
extern int32 g_rirup_hall_call[2][car_size];
extern int32 g_rirdn_hall_call[2][car_size];

extern int32 g_trup_hall_call[2][car_size];	// third riser
extern int32 g_trdn_hall_call[2][car_size];
extern int32 g_rtrup_hall_call[2][car_size];
extern int32 g_rtrdn_hall_call[2][car_size];

extern int16 g_asign_irup[fl_size];
extern int16 g_asign_irdn[fl_size];
extern int16 g_asign_rirup[fl_size];
extern int16 g_asign_rirdn[fl_size];

extern int16 g_asign_trup[fl_size];
extern int16 g_asign_trdn[fl_size];
extern int16 g_asign_rtrup[fl_size];
extern int16 g_asign_rtrdn[fl_size];

extern int16 sr_cc_ctrl[car_size];	// Second Riser car call control
extern int16 Car_on_SR[car_size];		// Car on Second Riser only operation (do not assign from std riser) 
extern int16 Valid_SR_Car[car_size];		// Valid car for second riser operation
extern int16 Valid_SR_Car_Backup[car_size];

extern int16 tr_cc_ctrl[car_size];	// Third Riser car call control
extern int16 Valid_TR_Car[car_size];	// Valid car for third riser operation
extern int16 Car_on_TR[car_size];		// Car on Third Riser only operation (do not assign from std riser)

extern uint32 uc_canc_hold[2];
extern uint32 dc_canc_hold[2];
extern uint32 ruc_canc_hold[2];
extern uint32 rdc_canc_hold[2];

extern uint32 uc_dsr_canc_hold[2];
extern uint32 dc_dsr_canc_hold[2];
extern uint32 ruc_dsr_canc_hold[2];
extern uint32 rdc_dsr_canc_hold[2];


extern int16 get_any_ir_hall_call (void);
extern void grp_clrir(void);
extern void grp_clrir_canc();
extern void grp_clrirhc();
extern void latch_ir_hallcall(void);
extern void latch_sr_hallcall(void);
extern void latch_tr_hallcall(void);
extern void assign_sr_hallcall (void);
extern void assign_tr_hallcall (void);
extern void group_ir_hallcancel(void);
extern void group_sr_hallcancel();
extern void group_tr_hallcancel();
extern void ir_control (void);
extern void ir_dual_riser (void);

#if (Tract_HR == 1)
extern void dual_serial_riser (void);
#endif

