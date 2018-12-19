// wifi variables

extern uint16 car_ip_address[4];
extern uint16 local_port;			
extern uint16 gateway_ip_address[4]; 	
extern uint16 host_ip_address[4];		
extern uint16 host_port;			
extern char network_ssid[25];	
extern char network_password[25];
extern char dns_name[25];
extern uint16 dhcp;	
extern int16 join_network;
extern int16 network_uart_baud;
extern int16 network_uart_flow;
extern uint16 dns_ip_address[4];
extern uint16 network_mask[4];
extern int16 network_protocol;
extern int16 tcp_mode;
extern int16 tx_power;
extern int16 auto_conn_time;
extern char device_id_name[25];
extern uint16 com_flush_timer;

extern void init_wifi_defaults(void);
extern int16 wifi_menu(int16 lcd_menu);


extern void update_wifi(void);

extern int16 wifi_setup_complete;


















