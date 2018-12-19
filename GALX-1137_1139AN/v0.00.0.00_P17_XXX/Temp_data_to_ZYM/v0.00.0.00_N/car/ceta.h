
// Average ETA time variables
extern int16 nmb_asg_uc;			// Number of assigned up calls
extern int32 avg_uc_eta;			// Average Up call ETA
extern int32 sum_uc_eta;			// Sum of uc eta
extern int16 nmb_asg_dc;			// Number of assigned down calls
extern int32 avg_dc_eta;			// Average Down call ETA
extern int32 sum_dc_eta;			// Sum of dc eta
extern int16 nmb_asg_iruc;			// Number of assigned ir up calls
extern int16 nmb_asg_irdc;			// Number of assigned ir down calls
	
extern int16 xassign;				// Enable cross assignment operation			
extern int16 xasgn_toggle;		// When ratio of calls is the same dispatch one to each system			

extern int16 car_up_eta[car_size][fl_size];
extern int16 car_dn_eta[car_size][fl_size];


extern void Car_ETA(void );
