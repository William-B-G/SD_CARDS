
// Definitions for Video routine

#define BS 8
#define ESC 27          /* defines an ascii escape */
#define LPT1 0

#define c_car_offset 15
#define c_top_left 0xDA
#define c_top_right 0xBF
#define c_bottom_left 0xC0
#define c_bottom_right 0xD9
#define c_single_line 0xC4
#define c_single_col 0xB3

#define c_dbl_top_left 0xC9
#define c_dbl_top_right 0xBB
#define c_dbl_bottom_left 0xC8
#define c_dbl_bottom_right 0xBC
#define c_dbl_line 0xCD
#define c_dbl_col 0xBA

#define c_car_offset_60 0
#define c_hc_up_arrow  0x18
#define c_hc_dn_arrow  0x19
#define c_hc_up_dn_arrow 0x12
#define c_cc_car 0xfe
#define c_car 0xdb

#define cc_PosInd 0xb1

#define scan_lines_200 0
#define scan_lines_350 1
#define scan_lines_400 2
#define CB40x25 0
#define C40x25 1
#define CB80x25 2
#define C80x25 3
#define font_8x14 0
#define font_8x8 1
#define font_8x16 0


#if (d_LCDDPY == 0)
	#include "lcd.h"
#endif
#if (d_KEYBRD == 0)
	#include "keybrd.h"
#endif
#if (d_IODPY == 0)
	#include "iodpy.h"
#endif
#if (d_NTSFVARS == 0)
	#include "ntsfvars.h"
#endif
#if (d_WIFIVARS == 0)
	#include "wifivars.h"
#endif
#if (d_TRACEDPY == 0)
	#include "tracedpy.h"
#endif

#if (d_ELEVSETUP == 0)
	#include "elevsetup.h"
#endif

#if (d_DIAGDPY == 0)
	#include "diagnostic.h"
#endif