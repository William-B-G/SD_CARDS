// Lobby variables
extern int16 lobby_fl;			// lobby floor
extern uint8 alt_lobby; 			// set to 1 for alternate lobby operation

// lobby request and next up variables
extern int16 next_up;		// master next up service flag, 1 = system using next up
extern int16 ncu_car;
extern int16 ncu_door[car_size];
extern int16 ncu_que[car_size];
extern int16 nmb_ncu_que;
extern int16 ncu_door_seq;
extern int16 ncu_seq;

extern int16 lobby_request;
extern int16 lobby_req_car;
extern int16 nmb_cars_at_lobby;
extern int16 nmb_lby_recall_req;
extern int16 lby_req[car_size];


extern void group_lobby_request(void);
extern void group_nextup(void);

