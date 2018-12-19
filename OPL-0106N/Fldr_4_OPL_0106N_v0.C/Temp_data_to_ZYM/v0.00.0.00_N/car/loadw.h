// Load weighing and pre-torque variables
extern int32 torque;					// amount to torque drive
extern uint32 drive_torque;	// torque value to send to the drive
extern int8 pre_torque_set;			// Set to 1 when pretorque is set
extern int32 percent_load;			// Percent load from load weigher
extern uint16 lw_data[];		// raw load weigher data
extern int16 lw_ix;					// load data index
extern uint32 avg_load;		// average load weigher reading
extern uint8 lw_command;		// command to load weigher
#define c_lw_read 1					// command to read load value
#define c_lw_calibrate 2			// Calibrate load weigher
#define c_lw_run_cal 4				// Run Calibrations routine
extern uint8 ret_lw_command;	// command returned from load weigher
// load data variable stored in flash memory
extern int16 lw_empty[];
extern int16 lw_full[]; 
extern unsigned char lw_status;		// load weigher status
#define c_lw_setup 0x1				// load weigher device setup
#define c_lw_on 0x2					// load weigher is on
#define c_lw_error 0x4				// load weigher has an error
#define c_lw_empty_setup 0x8	// load weigher empty setup
#define c_lw_full_setup 0x10	// load weigher full setup
#define c_lw_avg_load 0x20		// load weigher uses up and down average for load value
extern unsigned char lw_dir;		// load weigher direction
#define c_lw_up 1					// load in the up direction
#define c_lw_dn 2					// load in the down direction
#define c_lw_dir_error 0x80			// load direction error
extern unsigned char lw_error;		// load weigher error code
extern int16 lw_setup_seq;			// load weigher setup sequence
extern int16 load_bp;					// load bypass variable 1 = bypass
extern int16 load_an;					// load antinuisance 1 = not enough load
extern int16 load_dis;				// load dispatch 1 = dispatch car
extern int16 load_ovl;				// load overload 1 = car is overloaded
extern int16 no_wheelchair_cap;		// load high so no room for a wheelchair when 1.
extern int16 lw_cal_seq;				// load weigher calibration sequence flag
extern int16 lw_disable;				// logical flag to disable loadweigher from switch

extern void get_load (int16 force_upd);
extern void setup_load_weigher (void);
extern void set_pre_torque (void);
extern void calibrate_lw (void);
