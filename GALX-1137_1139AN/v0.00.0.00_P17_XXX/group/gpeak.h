
// Up Peak Variables
extern int16 up_peak;				// Up peak flag
extern int16 up_peak_trig_cnt;	// Up peak trigger count
extern int16 local_up_peak;			// Local Up Peak Flag
extern unsigned char up_trig_mem [car_size];	// memory of a car's up peak trigger being set
extern int16 up_peak_trig[20];	// Up peak trigger count used for moving sum
extern int16 up_peak_trig_ptr;	// Pointer for up peak trigger syn
extern int16 sum_up_peak_trig;	// Sum of up peak trigger count
extern int16 nmb_cc_above_lby;   	// number of latchedcar calls above the lobby
extern int16 lwd_set_at_lby;	   	// Load weigh dispatch set at lobby
extern int16 up_peak_trigger;		// Up peak trigger detection to group

// Down Peak Variables
extern int16 dn_peak;				// Down Peak Flag
extern int16 nmb_dc_above_lby;	// Number of down calls answered above the lobby
extern int16 dn_peak_trig[20];	// Down peak trigger count used for moving sum
extern int16 dn_peak_trig_ptr;	// Pointer for down peak trigger sum
extern int16 sum_dn_peak_trig;	// Sum down peak trigger count


extern void group_dn_peak(void);
extern void group_up_peak(void);
