// varaibales for the LCD Display
extern char Menu_list[50];

extern char LCD_Display[4][21];
extern int16 LCD_Cur_Pos;
extern int16 LCD_Line;
extern int16 LCD_Menu;
extern int16 LCD_Pointer;
extern int16 LCD_Init;
extern int16 LCD_Sub_Init;
extern int16 UpButton;
extern int16 DownButton;
extern int16 ModeButton;
extern int16 EnterButton;
extern int16 LCD_Flash;
extern int16 LCD_Old_Flash;
extern int8 LCD_Month;
extern int16 LCD_Year;
extern int8 LCD_Day;
extern int8 LCD_Minute;
extern int8 LCD_Hour;
extern char LCD_String[30];
extern char LCD_Time_Date[24];
extern char LCD_Flash_TMP[6];
extern int16 LCD_Dig_Point;
extern int16 LCD_Dig_Loc;
extern int16 LCD_ver;
extern int16 LCD_rev;

extern bool PasswordFlag;	   // a Flag that specified whether or not the password was enter correctly

extern const char LCD_Ins_Servf[][21];
extern const char LCD_Servf[][21];
extern const char LCD_Procf[][21];
extern const char LCD_SS_Status[][21];
extern const char LCD_Statusf[][21];
extern const char LCD_Statusf2[][21];
extern const char LCD_Statusf3[][21];
extern const char LCD_Statusf4[][21];
extern const char LCD_Doorf[][21];
extern const char LCD_Rdoorf[][21];
extern const char LCD_Fault[][21];
extern const char CAN_Test_Items[][21];
extern const char LCD_Field_Vars[][21];
extern const char LCD_Main_Menu[][21];
extern const char xLCD_Main_Menu[][21];
extern const char LCD_Input_Output[][21];
extern const char LCD_device_Fault[][21]; 
extern const char LCD_device_Reset[][21]; 
extern const char LCD_Software_Menu[][21];
extern const char spb_service[][7];
extern const char spb_status[][4];
extern const char pal_status[][4];
extern const char nts_service[][7];
extern const char Service[35][20];
extern const char servf_tbl[35][4];


extern unsigned char lcd_data[3];
extern const uint16 bittbl[16];
extern const uint32 l_bit_mask[32];

extern int16 set_adj_digit (int32 max,int16 digit);
extern void adjust_variable (int16 *varptr, int16 max, int16 min, int16 inc, int16 digit);
extern void adjust_variable_32 (int32 *varptr, int32 max, int32 min, int32 inc, int16 digit);

extern void init_LCD(void);
extern void Refresh_LCD(void);
extern int16 rdLCDio(int16 bitloc);
extern void display_variable(int16 row, int16 col, int16 digit, int16 lcd_par, int16 flash);
extern void display_variable_11_digits(int16 row, int16 col, int16 digit, int32 lcd_par, int16 flash);
extern void flash_digit (char dchar, int16 row, int16 col, int16 enable);
extern void write_display(char *s, int16 row, int16 col);
extern void Fill_Entire_Line_One_Char(char *s, int16 row);
extern void New_write_display(char *s, int16 row, int16 col);
extern void Sprint_Lcd_SignalChar(int16 Row,int16 Col,unsigned char Ch);
extern void Display_Var_Label(int16 menu, int16 fv_index, int16 var);
extern void StoreLCDInfo(void);
extern void clrLCDdpy(void);
extern char getcaps (char c);

#define Car_Motion_Menu 0
#define Car_Brake_Menu 1
#define Modified_Motion_Menu 2
#define Car_Timers_Menu 3
#define Car_Options_Menu 4
#define Service_Options_Menu 5
#define Emergency_Service_Menu 6
#define Group_Dispatch_Menu 7
#define Group_Options_Menu 8
#define CC_COP_Lights_Menu 9
#define HC_IR_Call_Lights_Menu 10
#define CB_Vip_HSec_Call_Lights_Menu 11
#define System_Options_Menu 12
#define Drive_Parameters_Menu 13
#define Safety_Proc_Adj_Vars_Menu 14
#define NTS_Proc_Adj_Vars_Menu 15

#define Num_Fvars_Menus 12
extern uint16 grp_fvars_chksum[Num_Fvars_Menus];
extern unsigned char const last_var [Num_Fvars_Menus];
#define Fvars_Per_Menu 60
extern uint16 const var_menu_tbl[Num_Fvars_Menus][Fvars_Per_Menu];
