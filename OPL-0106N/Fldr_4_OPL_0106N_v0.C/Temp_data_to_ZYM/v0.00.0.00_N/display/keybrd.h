extern int16 scancode;
extern int16 kb_int_cnt;
extern int16 key_code;
extern int16 key_enter;
extern int16 key_status;
extern uint16 rawcode[100];
extern uint16 p_rix;
extern uint16 g_rix;

extern void keybrd_control(void);
//lint -esym(534,key_brd)
extern int16 key_brd (int16 cmd);
extern void keybrd_init(void);
