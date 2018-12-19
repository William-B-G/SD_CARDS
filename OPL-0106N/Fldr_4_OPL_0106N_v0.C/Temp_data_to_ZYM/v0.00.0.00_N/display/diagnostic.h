
// Display variables for System Status
extern int32 ss_statusf;
extern int16 ss_pos;
extern int16 ss_cur_flt;
extern int16 ss_procf;
extern int16 ss_servf;
extern int16 ss_ul;
extern int16 ss_dl;
extern int16 ss_hsf;
extern int16 ss_dirf;

extern int16 diagnostics (int16 lcd_menu);
extern void dpy_elgo_sel_data (int16 dpy_line, int16 sel_dev, void *ptr);
extern void dpy_selcan_data (int16 dpy_line, int16 sel_dev, void *ptr);
