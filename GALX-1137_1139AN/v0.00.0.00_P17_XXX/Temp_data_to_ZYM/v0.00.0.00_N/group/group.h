
// Que to collect call in the order received (front and rear collected together)  rear Or'ed with 0x80
#define c_hc_que_len fl_size * 2


#if (d_GCONTROL == 0)
	#include "gcontrol.h"
#endif
#if (d_GCOMMLOSS == 0)
	#include "gcommloss.h"
#endif
#if (d_GCODEBLUE == 0)
	#include "gcodeblue.h"
#endif
#if (d_GVIP == 0)
	#include "gvip.h"
#endif
#if (d_GSECURITY == 0)
	#include "gsecurity.h"
#endif
#if (d_GRISER == 0)
	#include "griser.h"
#endif
#if (d_GPEAK == 0)
	#include "gpeak.h"
#endif
#if (d_GPARKING == 0)
	#include "gparking.h"
#endif
#if (d_GLOBBY == 0)
	#include "globby.h"
#endif
#if (d_GHALLCALL == 0)
	#include "ghallcall.h"
#endif
#if (d_GEMPOWER == 0)
	#include "gempower.h"
#endif


