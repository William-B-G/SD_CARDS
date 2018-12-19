
// Cedes CAN Bus Selector - Direct access to video position data
extern struct CED_can_type_tap{
	int32	position_count;
	int16	velocity;
	uint8 	vel_dir;			// velocity direction
    union {
    	uint16 R;
        struct {
            uint16:1;
            uint16 ALIGN:3;
            uint16 REV:4;
            uint16:3;
            uint16 IN1:1;
            uint16 IN2:1;
            uint16 CONTRAST:2;
            uint16 CLIP:1;
       } B;
    } Status;

    union {
        uint8 R;
        struct {
            uint8 CHANNEL1:2;
            uint8 CHANNEL2:2;
            uint8:1;
            uint8 GENERAL:3;
        } B;
    } Error;
	bool 	online;
	uint8	online_cnt;
	int16	tx_cnt;
	int16	rx_cnt;
}CED_can;		// Cedes Selector data CAN channel 1 - direct to CEDes device


 

extern int16 ced_can_tx_err_cnt;
extern int16 ced_can_rx_err_cnt;
extern int16 ced_can_online;		// ced can online
extern int16 ced_aps_can_error;		// ced aps can error
extern int16 ced_aps_fault;			// ced aps fault

extern int16 ced_data_in_cnt;
extern int16 ced_data_in_capture_cnt;
extern int16 ced_data_in_2_cnt;
extern int16 ced_data_in_2_capture_cnt;




extern void Ced_Can_Control (void);
extern void ced_init_can_bus (int16 baud_change);
extern void Ced_Pack_Can_Interrupt(void);



extern int16 Copy_galx_1121tester_NTS_Modify_Output_byte;





















