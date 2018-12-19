
// Drive comm variables

extern int16 drive_fault;
extern int16 drive_com_error;
extern int16 drive_online;
extern int16 drive_port;
extern int16 rcv_drive_msg;
extern int16 drive_vel;
extern int16 drive_rcv_cnt;
extern int16 drive_trm_cnt;
extern int16 drive_version;
extern int16 drive_revision;
extern float Drv_Velocity;
extern int16 drv_com_seq;		// sequence count for DSD 15 msec timer
extern int16 run_drive_com;		// set to 1 to run drive com
extern int16 delay_com_cnt;		// delay count for DSD 15 msec timer
extern int16 drv_soft_reset;		// set to send software reset to drive

extern int32 drv_status;
extern int32 drv_command;
extern int16 drv_trq;
extern int16 drv_trq_cmd;
extern int16 drv_line_voltage;
extern int16 drv_motor_current;
extern int16 drv_regen_torque;
extern int16 drv_motor_torque;
extern int16 drv_encoder_spd;
extern int16 drv_motor_voltage;
extern int16 drv_motor_frequency;
extern int16 drv_fault_code;

extern int32 drv_wpar_data;
extern int32 drv_rpar_data;
extern int16 drv_wpar_addr;
extern int16 drv_rpar_addr;
extern int16 drv_rpar_ret_addr;
extern int16 drv_wpar_ret_data;
extern int16 drv_wpar_ret_addr;
extern int16 drv_wpar_set;
extern int16 drv_rpar_set;

extern int16 drv_mode;
extern int16 drv_read_status;
extern int16 drv_rstatus_addr;
extern uint8 drv_nak;
extern uint8 drv_ack;
extern uint8 drv_eot;
extern int16 drv_error;
extern int16 drv_pkt_status;
extern uint8 drv_response_code[2];

extern unsigned char drv_trm_buf[40];
extern unsigned char drv_rcv_buf[40];

extern uint16 drv_baud;
extern int16 drv_baud_tbl[5];
extern int16 keb_drv_baud_tbl[5];
extern int16 drv_modbus_tbl[6];
extern int16 drv_modbus;

extern int16 drv_ep_mode;
extern int16 drv_rset_ep_mode;
extern int16 drv_has_power;				// used for emergency power lowering, 1 = normal power, 2 = emergency power

extern int16 drv_com_count;
extern int32 drv_com_source;

extern int16 set_drv_ep_mode (int16 drv_ep_mode_seq);
extern int16 reset_drv_ep_mode (int16 drv_rset_ep_mode_seq);

extern void init_drv_com (void);
extern void drv_com (void);
extern void verify_run_mode (void);


