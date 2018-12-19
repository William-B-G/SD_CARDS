extern int32 sim_DPP_Count;		// dpp counts for simulator
extern int16 sim_vel_timer;		// multiplied times velocity to get distance traveled
extern int16 sim_door_timer;		// door motion timer

#if (Traction == 0)
extern float Pulses_per_Inch;         // How many digital pulses in an inch
#endif

extern int16 dcl_test;
extern int16 dol_test;
extern int16 dc_test;

extern int16 toggle_array;
extern int16 toggle_bit;
extern int16 toggle_state;

extern int16 dn_hc_range_1;
extern int16 dn_hc_range_2;
extern int16 dn_hc_range_3;
extern int16 dn_hc_range_4;

struct hoistway_struct
{
	long dnl;
	long unl;
	long dtl;
	long utl;
	long pps[20]; // zero location unused
};

typedef struct hoistway_struct hoistway_struct_type;
extern hoistway_struct_type hstw;
extern int hw_tbl_file;

extern int16 sim_stall;
extern int16 sim_SEL_test;
extern int32 S_DPP_Floor_Pos[20];         // UP Normal's Digital Position
extern int16 sim_pos;

extern int16 sim_gs_open;
extern int16 sim_gs_close;
extern int16 sim_dl_open;
extern int16 sim_dl_close;
extern int16 sim_gsr_open;
extern int16 sim_gsr_close;
extern int16 sim_dlr_open;
extern int16 sim_dlr_close; 
extern int16 sim_dc_close;
extern int16 sim_dc_open;
extern int16 sim_enc_dir;
extern int16 sim_ul_dl_swap;
extern int16 pfc_on;
extern int16 sfc_on;

extern void sim_init(int16 init);
extern void sim_control (void);

