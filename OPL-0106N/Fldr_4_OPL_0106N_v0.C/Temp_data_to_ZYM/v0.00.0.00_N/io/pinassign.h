// CPU Port pins 0-154

enum 
{ PA0, PA1, PA2, PA3, PA4, PA5,	PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15, 
  PB0, PB1, PB2, PB3, PB4, PB5,	PB6, PB7, PB8, PB9,	PB10, PB11,	PB12, PB13, PB14, PB15,
  PC0, PC1,	PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9,	PC10, PC11, PC12, PC13,	PC14, PC15,
  PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11,	PD12, PD13,	PD14, PD15,
  PE0, PE1,	PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11,	PE12, PE13,	PE14, PE15,
  PF0, PF1,	PF2, PF3, PF4, PF5,	PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13,	PF14, PF15,
  PG0, PG1,	PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9,	PG10, PG11,	PG12, PG13, PG14, PG15,
  PH0, PH1,	PH2, PH3, PH4, PH5,	PH6, PH7, PH8, PH9,	PH10, PH11,	PH12, PH13,	PH14, PH15,
  PJ0, PJ1,	PJ2, PJ3, PJ4, PJ5, PJ6, PJ7, PJ8, PJ9,	PJ10, PJ11,	PJ12, PJ13,	PJ14, PJ15,
  PK0, PK1,	PK2, PK3, PK4, PK5, PK6, PK7, PK8, PK9,	PK10
};



// Pin definitions to match schemtatic

// Port PA 0-15 Expansion port
// Port PB 0-15 Unused
// Port PC 0-15 Unused except for pins 11,12 and 13

// LCD port control

#define UNUSED_PJ3 PJ3
#define UNUSED_PK0 PK0
#define UNUSED_PK1 PK1
#define UNUSED_PK2 PK2

#define Lcd_UP LCD_PB1
#define Lcd_DN LCD_PB2
#define Lcd_MODE LCD_PB3
#define Lcd_ENTER LCD_PB4



// LCD Push Buttons
#define LCD_PB4 PK4
#define LCD_PB3 PK5
#define LCD_PB2 PK6
#define LCD_PB1 PK7

// IO Board SPI-D control
#define IOBD_CLK PF12
#define IOBD_MOSI PF13 
#define IOBD_MISO PF14
#define IOBD_CS1n PF15
#define IOBD_CS2n PG2

// Unused pins
#define UNUSED_PJ0 PJ0
#define UNUSED_PJ1 PJ1
#define UNUSED_PJ2 PJ2
#define UNUSED_PJ4 PJ4
#define UNUSED_PJ5 PJ5
#define UNUSED_PJ6 PJ6
#define UNUSED_PJ7 PJ7 
#define UNUSED_PJ8 PJ8
#define UNUSED_PJ9 PJ9
#define UNUSED_PJ10 PJ10

// MRAM SPI-C control
#define MRAM_HOLDn PJ11
#define MRAM_HOLDn_2 PJ12
#define MRAM_WPn PJ13
#define MRAM_WPn_2 PJ14
#define MRAM_CSn PG6
#define MRAM_CSn_2 PG7

// Real-time Calendar Clock/MRAM SPI-C PF8-PF11
#define RTC_MRAM_CLK PF8
#define RTC_MRAM_DOUT PF9
#define RTC_MRAM_DIN PF10
#define RTC_CS PF11


// Can bus ports
#define CAN0_TX PD0
#define CAN0_RX PD1

#define CAN1_TX PD2
#define CAN1_RX PD3

#define CAN2_TX PD4
#define CAN2_RX PD5

#define CAN3_TX PD6
#define CAN3_RX PD7

// RS-232 and RS-485 ports
#define TX0 PE6			
#define RX0 PE7
#define RTS0 PE8

#define TX1 PD14		
#define RX1 PD15
#define RTS1 PE9

#define TX2 PE0
#define RX2 PE1
#define RTS2 PE10
#define CTS2 PD8 

#define TX3 PE2
#define RX3 PE3
#define RTS3 PE11

#define TX4 PE4
#define RX4 PE5
#define RTS4 PE12
#define CTS4 PD10 

#define TX5 PD12
#define RX5 PD13
#define RTS5 PE13
#define CTS5 PD11

// Test pins

#define TST1 PE14
#define TST2 PE15

#define Z6_LED PE14
#define Z0_LED PE15
#define TST3 PD9 
#define TST4 PC11
#define TST5 PC12
#define TST6 PC13

// LCD SPI-A Display PORT  PF0-PF3
#define DPY_CLK PF0
#define DPY_OUT PF1
#define DPY_IN PF2
#define DPY_CS PF3

// Secured Digital Port SPI-B PF4-PF7
#define SD_CLK PF4
#define SD_SO PF5
#define SD_SI PF6
#define SD_CS PF7
#define DETECT_SW PK8
#define PROTECT_SW PK10

// Port G - Unused except for pins 2,4,5,6 and 7
// Port H - Unused except for pins 0,8 and 9

// Dynamic brake temperature
#define DBR_TEMP PH0

// Encoder Quadrature input
#define EN1_A PH8
#define EN1_B PH9




