#define numntsvars 31
#define nts_fvtop_speed 0
#define nts_fvnum_floors 1
#define nts_fvnum_limits 2
#define nts_fvlimit_dist 3
#define nts_debug_mode 5
#define lastntsfvar 5			// set to the last variable number

extern uint16 nts_last_var; 
extern const char NTS_Field_Vars[][21];

extern int16 nts_fvars_deci[numntsvars];
extern int16 nts_fvars_max[numntsvars];
extern int16 nts_fvars_min[numntsvars];
extern int16 nts_fvars_default[numntsvars];
extern int16 nts_fvars[numntsvars];
extern uint16 const nts_var_ix[numntsvars];

extern void detect_nts_fvars_change (void);
extern int16 Adjust_NTS_Parameters (int16 lcd_menu);
extern int16 Display_NTS_IO (int16 lcd_menu);
