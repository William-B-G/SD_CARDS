extern const char LCD_Elev_Setup[][25];
#if (Traction == 1)
extern  char Learn[][25];
const char carbuffer_dpy[][25];
const char limit_test_dpy[][25];
#endif
extern void adjust_digital_speed_clamp (int16 clamp);
extern void load_weigher_adjust (void);
extern int16 Elevator_Setup (int16 lcd_menu);
extern int16 hwl_number_valid_fl (void);
