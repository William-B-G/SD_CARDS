extern bool front_vip; 	// Set to 1 for front vip service
extern bool rear_vip;	// Set to 1 for rear vip service

extern unsigned char VipCarNmb;
extern int16 fvip_en;				//vip front enable input
extern int16 rvip_en;				//vip rear enable input

extern int16 g_vip_fl[car_size];
extern int16 g_rvip_fl[car_size];

extern uint32 g_vipmsk[2];			// Bit mask for valid code blue call floors
extern uint32 g_rvipmsk[2];			// Bit mask for valid rear code blue call floors
extern int16 hp_car_sel[car_size];


extern void vip_recall(void);
extern void high_priority_fl(void);
