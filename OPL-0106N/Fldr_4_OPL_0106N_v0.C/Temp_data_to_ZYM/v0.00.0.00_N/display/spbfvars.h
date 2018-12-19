#define numspbvars 31
#define spb_rear_door 6
#define spb_buffer_type 17
#define spb_setup_mode 26
#define lastspbfvar 28			// set to the last variable number

extern uint16 spb_last_var; 
extern const char SPB_Field_Vars[][25];

extern int16 spb_fvars_deci[numspbvars];
extern int16 spb_fvars_max[numspbvars];
extern int16 spb_fvars_min[numspbvars];
extern int16 spb_fvars_default[numspbvars];
extern int16 spb_fvars[numspbvars];
extern uint16 const spb_var_ix[numspbvars];

extern void detect_spb_fvars_change (void);
extern int16 Adjust_SPB_Parameters (int16 lcd_menu);
extern int16 Display_SPB_IO (int16 lcd_menu);
