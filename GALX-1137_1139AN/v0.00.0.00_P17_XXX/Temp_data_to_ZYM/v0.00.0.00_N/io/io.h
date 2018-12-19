
#include "pinassign.h"
#if (GALaxy_4 == 1)
	#include "G4_iomap.h"					 
#else
	#include "iomap.h"					 
#endif

#if (GALaxy_4 == 1)
  #define nmb_io_brds 86
  #define nmb_hcio_brds 60
  #define nmb_hc_dvr_brds 10
#else
 #if (Tract_HR == 1)
   #define nmb_io_brds 53
   #define nmb_hcio_brds 60
 #else
   #define nmb_io_brds 34
   #define nmb_hcio_brds 27
 #endif
#endif

#define nmb_io_arrays nmb_io_brds * 3
#define nmb_ios nmb_io_brds * 24
#define nmb_hc_io_arrays nmb_hcio_brds * 3
#define nmb_hc_ios nmb_hcio_brds * 24


#if (d_IOINIT == 0)
	#include "G4_io_init.h"
#endif
#if (d_PIO == 0)
	#include "G4_pio.h"
#endif

#if (d_INIT_PINS == 0)
	#include "init_io_pins.h"
#endif

#if (d_GRP_IO == 0)
	#include "grp_io.h"
#endif

