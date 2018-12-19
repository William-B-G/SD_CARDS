//  PowerUp.c

#include "global.h"
#include "fat_filelib.h"
#include <string.h>

#include "GALX_1132_TEST.h"

extern int32 addr_ptr;
extern int32 address_code;
extern int32 start_address;

int16 JumpToPowerUp(int16 lcd_menu);

int16 program_status = 0;
int16 i = 0;

int16 JumpToPowerUp(int16 lcd_menu)
{
    // Get Ready to Jump
    int16 DummyVar = 0;
    DummyVar = lcd_menu;
    addr_ptr = 0x00000000;
    address_code = (*(uint32*)addr_ptr);
    addr_ptr = 0x00000004;
    start_address = (*(uint32*)addr_ptr);
    program_status = 0;

    if (1) { // (address_code == 0x015A015A)
        if (1) { // (start_address != 0)
            if (1) { // (get_MRAM_code() == 0x4D7C)
                for(i=0; i<=19; i++) {
                    LCD_Display[0][i] = ' ';
                    LCD_Display[1][i] = ' ';
                    LCD_Display[2][i] = ' ';
                    LCD_Display[3][i] = ' ';
                }
                timers[tsec] = 0;
                delay(20000);
                write_MRAM_code(0x5E8D);		// Write code to stay in powerup routine
                SIU.SRCR.B.SSR = 1;				// Software Reset
                return(-1);
            } else
                program_status = 1;
        } else
            program_status = 2;
    } else
        program_status = 3;
    clr_lcd_dply();
    New_write_display("Power-Up Jump Failed",1,0);
    New_write_display(" Invalid MRAM Code  ",2,0);

    return -1;
}
