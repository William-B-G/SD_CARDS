// ids security flags
extern int32 ids_uc_sec[2];
extern int32 ids_ruc_sec[2];
extern int32 ids_dc_sec[2];
extern int32 ids_rdc_sec[2];
extern int32 ids_cc_sec[2][9];
extern int32 ids_rcc_sec[2][9];

extern int32 ids_uhc_carsec[2][9];
extern int32 ids_dhc_carsec[2][9];
extern int32 ids_ruhc_carsec[2][9];
extern int32 ids_rdhc_carsec[2][9];

// ids control flags
extern int16 ids_ind[9];
extern int16 ids_rtl[9];
extern int16 ids_heof[9];
extern int16 ids_ad_off[9];
extern int16 ids_allw_door[9];
extern int16 ids_return_flr;
extern int16 ids_return_typ;
extern int16 ids_uppeak;
extern int16 ids_dnpeak;
extern int16 ids_alt_lby;
extern int16 ids_alt_park;
extern int16 ids_ncu;
extern int16 ids_alt_fl_sec;
extern int16 ids_refresh;
extern int16 ids_FL_Mask;
extern int16 g_ids_fsx;
extern int16 ids_online;


extern void ids_comm(void);
extern void init_ids (void);

