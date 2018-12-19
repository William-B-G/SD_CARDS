/*
#include "global.h"

#include "SD_TEST.h"
#include "RTC_TEST.h"

#define MAX_MENU_DEPTH 20

void GALX_1132_TEST(void);
void clr_lcd_dply(void);
void fail_testagain(void);
void MenuManager(void);
void Push_Menu_LIFO(int16 LCD_Menu);
int16 Pop_Menu_LIFO(void);

int16 TESTCASE_1132=0;
int16 Menu_LIFO_Index = 0;

struct Menu_LIFO
{
	int16 LCD_Menu;
	int16 Case_of;
}Menu_LIFO_Instance[MAX_MENU_DEPTH];

void Push_Menu_LIFO(int16 LCD_Menu)
{
  if(Menu_LIFO_Index < MAX_MENU_DEPTH)
    Menu_LIFO_Instance[Menu_LIFO_Index++].LCD_Menu = LCD_Menu;
  else  // tag01
  {  // fatal error, display msg and go to endless loop always petting the dog
    	delay(10000);  // delay 1.8 ms to allow LCD to reset
    	write_display("FATAL ERROR at tag01", 2,0);
 	for (;;) {
    	petdog();
  	}
  }  // END OF else
}

int16 Pop_Menu_LIFO(void)
{
  if(Menu_LIFO_Index > 0)
    return Menu_LIFO_Instance[--Menu_LIFO_Index].LCD_Menu;
  else  // tag02
  {  // fatal error, display msg and go to endless loop always petting the dog
    	delay(10000);  // delay 1.8 ms to allow LCD to reset
    	write_display("FATAL ERROR at tag02", 2,0);
 	for (;;) {
    	petdog();
  	}
  }  // END OF else

}

enum
{
	Init1132TEST,
	RS232_TEST,
	SD_TEST,
	CAN_TEST,
	RS485_TEST,
	RTC_TEST,
	PC104_TEST,
	VideoSPI_TEST,
	MRAM_TEST,
	GALX1132_TESTDONE,
	GALX1132_FAILED
};

enum
{
    Init_the_Menu,
	Scroll_Main_Menu,
	Perform_Main_Menu_Function,
	Function_Pass,
	Function_Fail,
	xxTest
};

void MenuManager(void)
{
static int16 LCD_Menu = 0;
static int16 Init_Main_Menu = 0;  // set to 0 after initialization

switch(LCD_Menu)
{
	case Init_the_Menu:
		TESTCASE_1132=SD_TEST;
		break;

	case Scroll_Main_Menu:
		TEST_SD(8);
		if(SDTestCase==SDTestDone)
			TESTCASE_1132=RTC_TEST;
		break;

	case Perform_Main_Menu_Function:
		TEST_RTC(8);
		break;

	case Function_Pass:
		TEST_SD(8);
		if(SDTestCase==SDTestDone)
			TESTCASE_1132=RTC_TEST;
		break;

	case Function_Fail:
		TEST_RTC(8);
		break;

	default:
		TESTCASE_1132=0;
		break;
}

}
*/
/*
void GALX_1132_TEST(void)
{
	switch(TESTCASE_1132)
	{
		case Init1132TEST:
			TESTCASE_1132=SD_TEST;
			break;

		case SD_TEST:
			TEST_SD(8);
			if(SDTestCase==SDTestDone)
				TESTCASE_1132=RTC_TEST;
			break;

		case RTC_TEST:
			TEST_RTC();
			break;

		default:
			TESTCASE_1132=0;
			break;
	}
}
*/
/*
void clr_lcd_dply(void)
{
	write_display("                    ",0,0);
	delay(10000);			// delay to allow LCD to reset
	delay(10000);			// delay to allow LCD to reset

	write_display("                    ",1,0);
	delay(10000);			// delay to allow LCD to reset
	delay(10000);			// delay to allow LCD to reset

	write_display("                    ",2,0);
	delay(10000);			// delay to allow LCD to reset
	delay(10000);			// delay to allow LCD to reset

	write_display("                    ",3,0);
	delay(10000);			// delay to allow LCD to reset
	delay(10000);			// delay to allow LCD to reset
}

void fail_testagain(void)
{
	write_display("TEST FAILED (AGAIN?)",2,0);
	delay(10000);			// delay to allow LCD to reset
	delay(10000);			// delay to allow LCD to reset

	write_display("NO(MODE)/YES(ENTER) ",3,0);
	delay(10000);			// delay to allow LCD to reset
	delay(10000);			// delay to allow LCD to reset
}

*/

