// Call to group controll from Z0 processor

#include "MPC5668G.h"
#include "pinassign.h"
#if (GALaxy_4 == 1)
	#include "G4_hmmap.h"
#else
	#include "hmmap.h"
#endif

extern void group_control (void);
extern void init_group_control (void);
int main_p1(void);


int main_p1(void) {
  int16 data_tmp = 0;
 
	init_group_control();
	
  	/* Loop forever */
 	for (;;) {

   		group_control();
  	}
}



