
//   SECF Control from the group
extern unsigned char sec_fl_ctrl[9];
extern int16 strtup_secf_rst_msks;
extern int16 secf_msks_applied;
extern int16 sr_on_serv;
extern int32 prev_g_cc_sec[2]; // Previous group car call security flag
extern int32 gc_ccs_per_fl[2][9];
extern int32 gc_rccs_per_fl[2][9];


extern int16 Grp_CC_Lockouts;		// Group car call lockouts security option flag
extern int16 Grp_CC_Override;		// Group car call lockout override security option flag
extern int16 Grp_HC_Lockouts;		// Group hall call lockouts security option flag
extern int16 Grp_FL_Lockouts;		// Group car and hall call lockouts security option flag

// Security flags and masks

extern uint8 update_hc_sec;
extern uint8 gc_hsec_prev;				// Group front hall call security flag
extern uint32 gc_hc_sec_prev[2];		// Hall Call Lockout Security
extern uint8 gc_rhsec_prev;			   	// Group rear hall call security flag
extern uint32 gc_rhc_sec_prev[2];		// Hall Call Lockout Security

extern uint8 gc_sec;				// Group car call lockout security flag
extern uint32 gc_cc_sec[2];			// Car Call Lockout Security
extern uint8 gc_rsec;				// Group rear car call lockout security flag
extern uint32 gc_rcc_sec[2];		// Car Call Lockout Security
extern uint8 gc_hsec;				// Group front hall call security flag
extern uint8 gc_shsec;				// Group serial front hall call security flag
extern uint32 gc_hc_sec[2];			// Hall Call Lockout Security
extern uint8 gc_rhsec;				// Group rear hall call security flag
extern uint8 gc_rshsec;				// Group rear serial hall call security flag
extern uint32 gc_rhc_sec[2];		// Hall Call Lockout Security

extern uint8  gc_sab_fl;				// Sabath Floor Enable from Group
extern uint32 gc_sab_fl_msk[2];			// Sabath Floor Mask from Group
extern uint8  gc_sab_rfl;				// Sabbath Rear Floor Enable from Group
extern uint32 gc_sab_rfl_msk[2];		// Sabath Rear Floor Mask from Group

extern uint8 gc_hugsec;	   			// Front hugs security enable
extern uint8 gc_rhugsec;	   			// Rear hugs security enable
extern uint32 gc_hugs_sec[2];			// Hugs security mask
extern uint32 gc_rhugs_sec[2];			// Hugs rear security mask

extern uint8 gc_sec_ovr;					// Group car call security override flag
extern uint32 gc_cc_sec_ovr[2];				// Group car call security override 
extern uint32 gc_cc_sec_ovr_set[2];			// Group car call security override output set
extern uint32 gc_cc_sec_ovr_panic[2];		// Group car call security override panic stop 
extern uint32 gc_cc_sec_ovr_toggle[2];		// Group car call security override toggle 
extern uint8 gc_rsec_ovr;					// Group rear car call security override flag
extern uint32 gc_rcc_sec_ovr[2];			// Group rear car call security override
extern uint32 gc_rcc_sec_ovr_set[2];		// Group rear car call security override output set
extern uint32 gc_rcc_sec_ovr_panic[2];		// Group rear car call security override panic stop
extern uint32 gc_rcc_sec_ovr_toggle[2];		// Group rear car call security override toggle


extern uint32 ssec_uhc_carsec[2][9];
extern uint32 ssec_dhc_carsec[2][9];
extern uint32 ssec_ruhc_carsec[2][9];
extern uint32 ssec_rdhc_carsec[2][9];

extern uint32 flcfg_uhc_carsec[2][9];
extern uint32 flcfg_dhc_carsec[2][9];
extern uint32 flcfg_ruhc_carsec[2][9];
extern uint32 flcfg_rdhc_carsec[2][9];

extern uint32 master_uhc_carsec[2][9];
extern uint32 master_dhc_carsec[2][9];
extern uint32 master_ruhc_carsec[2][9];
extern uint32 master_rdhc_carsec[2][9];


extern uint16 g_secf_table[5];

extern uint16 hc_car_sec[9];

extern uint8 has_assignment[9];

extern int16 sec_ftimer[fl_size];
extern int16 sec_rtimer[fl_size];
extern int16 sec_ir_ftimer[fl_size];
extern int16 sec_ir_rtimer[fl_size];

extern void hc_sec_timers(void);
extern void set_master_hc_sec(void);
extern void group_lockouts(void);
extern void group_swap_fl_tbl (void);
extern void group_swap_fl_tbl_2 (void);
extern void group_swap_fl_tbl_3 (void);
extern void group_sec_car_fl_keysw (void);
extern void group_sabbath (void);
extern void grp_cc_override(void);
extern void evacuation_svc(void);
