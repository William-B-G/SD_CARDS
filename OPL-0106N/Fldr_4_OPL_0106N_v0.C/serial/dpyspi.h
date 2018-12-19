#define io_off 			0x00
#define io_on 			0x01
#define motor_on		0x02
#define valve_on		0x03
#define up_arrow 		0x04
#define dn_arrow		0x05
#define doors_closed 	0x06
#define fdoor_open		0x07
#define rdoor_open		'_'

extern uint8 put_dpyspi;
extern int16 contrast;
extern int16 brightness;

extern void init_dpyspi (void);
extern void LCD_Buffer_to_spi (void);
extern void write_dpyspi(int16 row);
extern void set_contrast(int16 new_contrast);
extern void set_brightness(int16 new_bright);
extern int16 get_contrast(void);
extern int16 get_brightness(void);
