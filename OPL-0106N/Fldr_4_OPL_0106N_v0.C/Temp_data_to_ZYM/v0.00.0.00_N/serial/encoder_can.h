
extern int32 can_enc_count;
extern int32 can_enc_preset_count;
extern int32 can_enc_speed;
extern int32 can_enc_accel;
extern int16 can_enc_ppr;
extern int16 can_enc_dir;
extern int32 can_enc_range;
extern int16 can_enc_op_status;

extern bool WriteEncCount;	
extern bool WriteEncPPR;
extern bool ReadEncPPR;
extern bool WriteEncDir;
extern bool WriteEncMaxPulses;
extern bool SaveEncPar;
extern bool SaveEncAppPar;

extern int8 enc_app_par_sav;
extern int8 enc_cnt_wrt;
extern int8 enc_par_sav;

extern bool EncCan_Upd;

extern int16 Enc_Packet_Req;

extern bool can_enc_init;
extern bool can_enc_com_err;
extern bool can_enc_online;
extern int16 can_enc_rx_cnt;
extern int16 can_enc_tx_cnt;
extern int16 can_enc_online_cnt;
extern int16 can_enc_statusf;		// can encoder status flag bit 0: 1 = Initialized the encoeder to default values

extern bool can_enc_init_fail;

extern int16 enc_can_tx_err_cnt;
extern int16 enc_can_rx_err_cnt;


extern void Enc_Can_Control (void);
extern void enc_init_can_bus (int16 baud_change);
