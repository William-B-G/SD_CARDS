extern int16 grp_parking_floor[9];
extern int16 prk_floor_full[9];
extern int16 car_start_latch[9];
extern int16 park_fl_car[9];
extern int16 park_car_fl_ix[9];
extern int16 how_many[9];
extern int16 park_dist[9];
extern int16 best_car;
extern int16 best_rp_car;
extern int16 prev_best_car;
extern int16 prev_best_rp_car;
extern int16 cars_being_parked;
extern int16 nmb_parked_cars;
extern uint16 parking_fl_taken[fl_size];

extern int16 g_park[car_size];
extern int16 cars_to_park;

extern uint16 alt_park_inp;

extern void group_park(void );
extern void park_car(int16 park_fl_ix);
extern int best_parking_car(int16 parking_fl);
extern int16 best_reparking_car(int16 parking_fl,int16 parked_fl,int16 how_many);
