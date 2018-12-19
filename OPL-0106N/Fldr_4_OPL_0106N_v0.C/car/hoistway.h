extern uint8 limit_ix;
extern const uint16 limit_dist[];
 
extern int16 hwl_mode;
extern int16 hwl_mode_car;
extern int16 hwl_mark;
extern int16 hwl_fl_update;
extern int16 hwl_floor;
extern int16 hwl_floor_marked;
extern int16 hwl_num_floors;
extern int16 hwl_num_limits;
extern int16 hwl_top_speed;
extern int16 hwl_seq;
extern int16 hwl_zero_hoistway;

extern int32 Hoistway_Init_Count;	// Hoistway init count


extern void set_HW_IO_from_APS_Sel (void);
extern void Set_Invalid_Floors (void);
extern void Set_limit_ix (void);
extern void Set_Limit_Table (void);
extern void APS_Car_Hoistway_Setup(void);
extern void APS_Hoistway_Unmark_Floor(void);
extern void APS_Hoistway_Mark_Floor(void);
extern int16 verify_hoistway(void);
extern void read_hoistway_count(void);
extern int16 verify_floor_table(void);
extern void zero_hoistway_table(void);

