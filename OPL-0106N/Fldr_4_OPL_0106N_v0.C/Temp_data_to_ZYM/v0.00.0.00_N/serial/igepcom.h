// Data from serial intergroup emergency power
extern unsigned char igep_com_online;
extern int16 igep_car_recall_order[9];		// Car return order
extern int16 igep_car_select_order[9];		// Car selection order
extern int16 igep_car_man_slct_req[9];	  	// Manual selection request
extern int16 igep_car_man_sel_req[9];	  	// Manual selection request

extern void IGEP_comm(void);
