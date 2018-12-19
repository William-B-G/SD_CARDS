
extern int16 net_cmd_req[2];
extern int16 net_cmd_resp[2];
extern int16 net_cmd_mode_req[2];
extern int16 in_net_cmd_mode[2];

extern void Send_Net_Cmd(int16 comnmb, int16 cmd);
extern void En_Net_Cmd_Mode (int16 comnmb);
extern void Proc_Net_Cmd (int16 comnmb);
extern int16 Rcv_Net_Response (int16 comnmb, int16 net_cmd, bool init_flag);














	


