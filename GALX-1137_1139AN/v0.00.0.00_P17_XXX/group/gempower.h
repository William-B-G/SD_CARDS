// Emergency power variables
extern int16 ep_auto_recall_en;  				// Emergency Power Auto Recall enable
extern int16 ep_auto_select_en;  				// Emergency Power Auto Select Enable
extern int16 ep_auto_select_en_prev;			// Emergency Power Auto Select Enable  previous
extern int16 car_fire_sw_on;

extern int16 ep_run_en;							// Emergency Power Run enable
extern int16 ep_run_ctl;						// Emergency power run ctl output
extern int16 man_sel_req[car_size];				// manual selection request from switch input
extern int16 car_recall_order[car_size];	 	// Car return order
extern int16 car_select_order[car_size]; 		// Car selection order
extern int16 car_recall_order_fixed[car_size]; 	// Car return order from cons file
extern int16 car_select_order_fixed[car_size]; 	// Car selection order from cons file
extern int16 nmb_recall_cars;					// number of cars for each power feeder
extern int16 nmb_select_cars;					// number of cars for each power feeder
extern int16 car_recall_en;						// enable recall of cars for each power feeder
extern int16 car_select_en;						// enable selection of cars for each power feeder
extern int16 select_req;						// detect change in manual selection switch				
extern int16 prev_select_req;					// detect change in manual selection switch
extern int16 man_sel_seq;					  	// manual selection state machine sequence
extern int16 recall_seq;					  	// Recall state machine sequence
extern int16 cars_recalling;				  	// Number of cars recalling
extern int16 recalled_cars;				  		// number of recalled cars
extern int16 recall_finished;				  	// Recall sequence finished
extern int16 recall_attempt;				  	// Number of recall attempts
extern int16 select_seq;					  	// Car Select state machine sequence
extern int16 cars_selected;					  	// Number of cars selected
extern int16 xcars_sel_from_eps;				// Number of cars selected from EPS that are NOT part of select group
extern int16 cars_moving;					  	// Set true when any car is moving
extern int16 cars_in_motion;				  	// Number of cars in motion that are not selected
extern int16 car_recall_order_fixed[car_size]; 	// Car return order from cons file
extern int16 car_select_order_fixed[car_size]; 	// Car selection order from cons file

extern  int16 ep_prk[car_size];					// EP Car returned and parked output 
extern int16 ep_op[car_size];					// EP Car operation output 
extern int16 ep_sel[car_size];					// EP Car selected output 

extern int16 gempf;			// data from grp_empf[pwr_feed][car] of the group is transferred into gempf for this car

extern int16 grp_empf[car_size];		// emergency power group flag
#define no_ep 0							//  0 not selected to run
#define ep_wait	1						//  1 wait on group emp
#define ep_wait_door 2 					//  2 wait with doors open
#define ep_recall 3						//  3 car selected to recall 
#define ep_home	4						//  4 car recalled successfully
#define ep_home_close 5					//  5 car recalled and cycle doors closed
#define ep_select 6						//  6 car selected to run on automatic
#define ep_switch 7						//  7 car selected to run from selection switch
#define ep_timed_out 8					//  8 car timed-out during recall
#define ep_out_of_svc 9					//  9 car is out of service during recall

#define epc_wait_close 7				//  7 car is waiting with the doors closed				 

extern void group_em_power(void);

