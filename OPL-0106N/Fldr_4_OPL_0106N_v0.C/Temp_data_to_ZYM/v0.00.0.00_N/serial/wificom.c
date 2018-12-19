#define d_WIFICOM 1
#include "global.h"

/*
		Network Commands
		
set ip dhcp 2						// set dhcp operation
set ip address 192.168.1.201 2000 	// set local ip address
set ip localport 2000				// set local port
set ip netmask 255.255.255.0    	// set network mask
set ip gateway 192.168.1.1			// set gateway ip address
set ip host 192.168.1.200:2000  	// set host ip address
set ip remote 2000					// set host port
set ip proto 2               		// 0-31

set dns address 192.168.1.1 		// set dns address
set dns name GALaxyIV				// set dns name

set wlan ssid GALaxyIV				// set ssid name
set wlan join 1						// join automatically on power up
set wlan pass galaxy4				// ssid password
set wlan tx 12 						// power 0-12 0 and 12 set to max 12


set u b 115200						// set uart baud 115200


open 192.168.1.200 2000	    		// open connection to ip port 
close								// close connection
join GALaxyIV						// join network GALaxyIV
leave								// leave network
exit								// exit command mode
reboot								// reboot device
save								// save parameters command


// possible future commands
ping 192.168.1.205  10				// ping 10 times
scan								// scan for network devices

*/


void Send_Net_Cmd(int16 comnmb, int16 cmd);
void En_Net_Cmd_Mode (int16 comnmb);
void Proc_Net_Cmd (int16 comnmb);
int16 convert_ip (char * c, uint16 * ip);
void copy_name (char * s, char * d, char end_char);
int16 Rcv_Net_Response (int16 comnmb, int16 net_cmd, bool init_flag);
int16 Get_Net_Vars (int16 comnmb);
int16 Set_Net_Vars(int16 comnmb);

int16 get_net_vars_seq[2] = {0,0};
int16 set_net_vars_seq[2] = {0,0};
int16 net_rcv_ix[2] = {0,0};
int16 net_rcv_st[2] = {0,0};
int16 net_rcv_seq[2] = {0,0};
int16 en_cmd_state[2] = {0,0};
int16 net_cmd_req[2] = {0,0};
int16 net_cmd_resp[2] = {0,0};
int16 net_cmd_mode_req[2] = {0,0};
int16 in_net_cmd_mode[2] = {0,0};
int16 net_cmd[2] = {0,0};
int16 net_response[2] = {0,0};

uint8 txbuf[2][30];

const char Network_Uart_Baud[8][6] = {
  "000000",
  "002400",
  "004800",
  "009600",
  "019200",
  "038400",
  "057600",
  "115200",
};


void Send_Net_Cmd(int16 comnmb, int16 cmd)
{
	int16 len;
	bool valid_cmd = 0;
	int16 i,k;

	switch (cmd)
	{
		case 0: 		// command 0: put device in command mode
			len = 3;
			strncpy((char*)&tranbuf[comnmb],"$$$",len);
			valid_cmd = 1;	 		
			break;
			
		case 1:			// command 1: get ip
			len = 8;
			strncpy((char*)&tranbuf[comnmb],"get ip\r\n",len);
			valid_cmd = 1;	 		
			break;
				 		
		case 2:			// command 2: get dns
			len = 9;
			strncpy((char*)&tranbuf[comnmb],"get dns\r\n",len);
			valid_cmd = 1;	 		
			break;
				 		
		case 3:			// command 3: get wlan
			len = 10;
			strncpy((char*)&tranbuf[comnmb],"get wlan\r\n",len);
			valid_cmd = 1;	 		
			break;
				 
		case 4:			// command 4: get uart
			len = 10;
			strncpy((char*)&tranbuf[comnmb],"get uart\r\n",len);
			valid_cmd = 1;	 		
			break;
				 
		case 5:		 	// set dhcp operation
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set ip d %i\r\n",dhcp);
			valid_cmd = 1;	 		
			break;

		case 6:			// set local ip address	   	
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set ip a %i.%i.%i.%i\r\n",car_ip_address[0],car_ip_address[1],car_ip_address[2],car_ip_address[3]);
			valid_cmd = 1;	 		
			break;
		case 7:			   // set local port
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set ip l %i\r\n",local_port);
			valid_cmd = 1;	 		
			break;
		case 8:			// set network mask	   	
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set ip n %i.%i.%i.%i\r\n",network_mask[0],network_mask[1],network_mask[2],network_mask[3]);
			valid_cmd = 1;	 		
			break;
		case 9:			// set gateway ip address	   	
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set ip g %i.%i.%i.%i\r\n",gateway_ip_address[0],gateway_ip_address[1],gateway_ip_address[2],gateway_ip_address[3]);
			valid_cmd = 1;	 		
			break;
		case 10:			// set host address	   	
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set ip h %i.%i.%i.%i\r\n",host_ip_address[0],host_ip_address[1],host_ip_address[2],host_ip_address[3]);
			valid_cmd = 1;	 		
			break;
 		case 11:			   // set host port
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set ip r %i\r\n",host_port);
			valid_cmd = 1;	 		
			break;

		case 12:		 	// set network protocol   0-31
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set ip p %i\r\n",network_protocol);
			valid_cmd = 1;	 		
			break;

		case 13:		 	// unused
			len = 15;
			break;

		case 14:			// set dns ip address	   	
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set dns a %i.%i.%i.%i\r\n",dns_ip_address[0],dns_ip_address[1],dns_ip_address[2],dns_ip_address[3]);
			valid_cmd = 1;	 		
			break;

		case 15:		 	// set dns name
			strncpy((char*)&tranbuf[comnmb],"set dns name ",13);
			k = 13;
			for (i=0;i<24;i++)
			{
				if (dns_name[i] != 0)
				{
					if (dns_name[i] == ' ')
						tranbuf[comnmb][k++] = '$';		// replace space with $
					else
						tranbuf[comnmb][k++] = dns_name[i];
				}
				else
					break;
			}
			tranbuf[comnmb][k++] = CR;
			tranbuf[comnmb][k++] = LF;
			len = k;			
			valid_cmd = 1;	 		
			break;

		case 16:		 	// set wlan ssid 
			strncpy((char*)&tranbuf[comnmb],"set wlan ssid ",14);
			k = 14;
			for (i=0;i<24;i++)
			{
				if (network_ssid[i] != 0)
				{
					if (network_ssid[i] == ' ')
						tranbuf[comnmb][k++] = '$';		// replace space with $
					else
						tranbuf[comnmb][k++] = network_ssid[i];
				}
				else
					break;
			}
			tranbuf[comnmb][k++] = CR;
			tranbuf[comnmb][k++] = LF;
			len = k;			
			valid_cmd = 1;	 		
			break;

		case 17:		 	// set wlan join  
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set wlan join %i\r\n",join_network);
			valid_cmd = 1;	 		
			break;

		case 18:		 	// set wlan password
			strncpy((char*)&tranbuf[comnmb],"set wlan pass ",14);
			k = 14;
			for (i=0;i<24;i++)
			{
				if (network_password[i] != 0)
				{
					if (network_password[i] == ' ')
						tranbuf[comnmb][k++] = '$';		// replace space with $
					else
						tranbuf[comnmb][k++] = network_password[i];
				}
				else
					break;
			}
			tranbuf[comnmb][k++] = CR;
			tranbuf[comnmb][k++] = LF;
			len = k;			
			valid_cmd = 1;	 		
			break;

		case 19:		 	// set wlan TxPower  
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set wlan tx %i\r\n",tx_power);
			valid_cmd = 1;	 		
			break;

		case 20:		 	// set uart baudrate  
			strncpy((char*)&tranbuf[comnmb],"set u b ",8);
			k = 8;
			for (i=0;i<6;i++)
			{
				tranbuf[comnmb][k++] = Network_Uart_Baud[network_uart_baud][i];
			}
			tranbuf[comnmb][k++] = CR;
			tranbuf[comnmb][k++] = LF;
			len = k;			
			valid_cmd = 1;	 		
			break;
			
		case 21:			// open network connection	   	
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"open %i.%i.%i.%i %i\r\n",host_ip_address[0],host_ip_address[1],host_ip_address[2],host_ip_address[3],host_port);
			valid_cmd = 1;	 		
			break;
		
 		case 22:			// close network connection
			len = 7;
			strncpy((char*)&tranbuf[comnmb],"close\r\n",len);
			valid_cmd = 1;	 		
			break;
				 		
		case 23:		 	// Join network 
			strncpy((char*)&tranbuf[comnmb],"join ",5);
			k = 5;
			for (i=0;i<24;i++)
			{
				if (network_ssid[i] != 0)
				{
					if (network_ssid[i] == ' ')
						tranbuf[comnmb][k++] = '$';		// replace space with $
					else
						tranbuf[comnmb][k++] = network_ssid[i];
				}
				else
					break;
			}
			tranbuf[comnmb][k++] = CR;
			tranbuf[comnmb][k++] = LF;
			len = k;			
			valid_cmd = 1;	 		
			break;

 		case 24:			// leave network connection
			len = 7;
			strncpy((char*)&tranbuf[comnmb],"leave\r\n",len);
			valid_cmd = 1;	 		
			break;
				 		
 		case 25:			// exit command mode
			len = 8;
			strncpy((char*)&tranbuf[comnmb],"\r\nexit\r\n",len);
			valid_cmd = 1;	 		
			break;
				 		
 		case 26:			// reboot wifi device
			len = 8;
			strncpy((char*)&tranbuf[comnmb],"reboot\r\n",len);
			valid_cmd = 1;	 		
			break;

 		case 27:			// save wifi device parameters
			len = 6;
			strncpy((char*)&tranbuf[comnmb],"save\r\n",len);
			valid_cmd = 1;	 		
			break;
			
		case 28:		 	// set uart flow control  
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set u flow %i\r\n",network_uart_flow);
			valid_cmd = 1;	 		
			break;

 		case 29:			// set sys autoconn every 20 seconds
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set sys autoconn %i\r\n",auto_conn_time);
			valid_cmd = 1;	 		
			break;
			
		case 30:			// command 30: get sys
			len = 9;
			strncpy((char*)&tranbuf[comnmb],"get sys\r\n",len);
			valid_cmd = 1;	 		
			break;
				 		
		case 31:		 	// command 31: set device ID name
			strncpy((char*)&tranbuf[comnmb],"set opt deviceid ",17);
			k = 17;
			for (i=0;i<24;i++)
			{
				if (device_id_name[i] != 0)
				{
					if (device_id_name[i] == ' ')
						tranbuf[comnmb][k++] = '$';		// replace space with $
					else
						tranbuf[comnmb][k++] = device_id_name[i];
				}
				else
					break;
			}
			tranbuf[comnmb][k++] = CR;
			tranbuf[comnmb][k++] = LF;
			len = k;			
			valid_cmd = 1;	 		
			break;
			
		case 32:			// command 32: get option
			len = 9;
			strncpy((char*)&tranbuf[comnmb],"get opt\r\n",len);
			valid_cmd = 1;	 		
			break;

		case 33:			// command 33: get com
			len = 9;
			strncpy((char*)&tranbuf[comnmb],"get com\r\n",len);
			valid_cmd = 1;	 		
			break;

		case 34:		 	// command 34: set comm time  
			len = 0;
			sprintf((char*)&tranbuf[comnmb],"set comm time %i\r\n",com_flush_timer);
			valid_cmd = 1;	 		
			break;

    	default:
    		break;
	}

    if (valid_cmd == 1)
    {
    	
    	if (len == 0)
    	{
	    	for (i=0;i<=30;i++)
	    	{
	    		if (tranbuf[comnmb][i] == LF)
	  				break;
	    	}
   			len = i+1;
    	}
		if (comnmb == USR1COM)
		{
			com_1_tx_start(len);
		}
		else if (comnmb == USR2COM)
		{
			com_2_tx_start(len);
		}
    }
}


void En_Net_Cmd_Mode (int16 comnmb)
{
	int16 response;
	int16 rbempt = 0;
	int16 count = 0;
	char temp;
	
	switch (en_cmd_state[comnmb])
	{
		case 0:		// set delay timer 
			timers[tcom_tran+comnmb] = 0;
			en_cmd_state[comnmb] = 1;
			break;
		case 1:		// delay at lease 250 msec
			if (timers[tcom_tran+comnmb] > 4)
			{
			    // clear receiave buffer
			  	rbempt = (uint8)isrxempty(comnmb);
			  	while((rbempt == 0) && (count < 1000))
			  	{
				  	temp = (uint8)asigetc(comnmb);
				  	rbempt = (uint8)isrxempty(comnmb);
				  	count++;
			  	}
			
				Send_Net_Cmd(comnmb,0);			// $$$
				Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
				timers[tcom_tran+comnmb] = 0;
				en_cmd_state[comnmb] = 2;
			}
			break;
		case 2:
			response = Rcv_Net_Response(comnmb,0,0); 
			if (response == 1)	 		// CMD Response
			{   // set delay timer
				timers[tcom_tran+comnmb] = 0;
				en_cmd_state[comnmb] = 3;
			}
			else if (response == -1)
			{   // time-out fault
				en_cmd_state[comnmb] = 4;
				timers[tcom_tran+comnmb] = 0;
			}
			break;
		case 3:
			if (timers[tcom_tran+comnmb] > 4)
			{
				in_net_cmd_mode[comnmb] = 1;
				net_cmd_mode_req[comnmb] = 0;	
				timers[tcom_tran+comnmb] = 0;
				en_cmd_state[comnmb] = 0;
			}
			break;
		case 4:
			if (timers[tcom_tran+comnmb] > 4)
			{
				in_net_cmd_mode[comnmb] = -1;
				net_cmd_mode_req[comnmb] = 0;	
				timers[tcom_tran+comnmb] = 0;
				en_cmd_state[comnmb] = 0;
			}
			break;
		default:
			break;
	}
			
	
}

void Proc_Net_Cmd (int16 comnmb)
{
	int16 response;
	
	if (net_cmd_req[comnmb] == 1)
	{		 // get net variables
		get_net_vars_seq[comnmb] = 0;		  	// get net variables
		net_cmd[comnmb] = 1;
		net_cmd_resp[comnmb] = 0;
		net_cmd_req[comnmb] = 0;
	}
	else if (net_cmd_req[comnmb] == 2)
	{		 // set net variables
		set_net_vars_seq[comnmb] = 0;		  	// set net variables
		net_cmd[comnmb] = 2;
		net_cmd_resp[comnmb] = 0;
		net_cmd_req[comnmb] = 0;
	}
	else if (net_cmd_req[comnmb] == 3)
	{		 // send save command
		Send_Net_Cmd(comnmb,27);			// send save command
		Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
		net_cmd[comnmb] = 3;
		net_cmd_resp[comnmb] = 0;
		net_cmd_req[comnmb] = 0;
	}
	else if (net_cmd_req[comnmb] == 4)
	{		 // exit command mode
		Send_Net_Cmd(comnmb,25);			// exit command mode
		Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
		net_cmd[comnmb] = 4;
		net_cmd_resp[comnmb] = 0;
		net_cmd_req[comnmb] = 0;
	}
	else if (net_cmd_req[comnmb] == 5)
	{		 // send reboot command
		Send_Net_Cmd(comnmb,26);			// send reboot command
		Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
		net_cmd[comnmb] = 5;
		net_cmd_resp[comnmb] = 0;
		net_cmd_req[comnmb] = 0;
	}
	
	switch (net_cmd[comnmb])
	{
		case 0:
			net_cmd_req[comnmb] = 0;
			break;
		case 1:			// get net variables
			get_net_vars_seq[comnmb] = Get_Net_Vars(comnmb);
			if (get_net_vars_seq[comnmb] == 0)
			{
				get_net_vars_seq[comnmb] = 0;
				net_cmd[comnmb] = 0;	
			}
			break;
		case 2:			// set net variables
			set_net_vars_seq[comnmb] = Set_Net_Vars(comnmb);
			if (set_net_vars_seq[comnmb] == 0)
			{
				set_net_vars_seq[comnmb] = 0;
				net_cmd[comnmb] = 0;	
			}
			break;
		case 3:			// save net variables
			response = Rcv_Net_Response(comnmb,27,0);   // Storing in config
			if (response != 0)
			{
				net_cmd_resp[comnmb] = response;
				net_cmd[comnmb] = 0;	
			}
			break;
		case 4:			// EXIT command mode
			response = Rcv_Net_Response(comnmb,25,0);   // EXIT
			if (response != 0)
			{
				net_cmd_resp[comnmb] = response;
				net_cmd[comnmb] = 0;	
				in_net_cmd_mode[comnmb] = 0;    		// exit command mode and run normal comm
			}
			break;
		case 5:			// send reboot command
			response = Rcv_Net_Response(comnmb,26,0);   
			if (response != 0)
			{
				net_cmd_resp[comnmb] = 1;
				net_cmd[comnmb] = 0;	
				in_net_cmd_mode[comnmb] = 0;    		// reboot command
			}
			break;
		default:
			break;
					
	}
	
}

int16 convert_ip (char * c, uint16 * ip)
{
	int16 i = 0;
	int16 j = 0;
	int16 k = 0;
	
	while ((k < 4) && (c[i] != CR))
	{
		if ((c[i] == '.') || (c[i] == ':'))
		{
			c[i] = 0;
			ip[k++] = (int16)atoi(&c[j]);
			j = i + 1;
		}
		i++;	
	}
	if (k == 3)
	{
		ip[k++] = (int16)atoi(&c[j]);	   // last ip if no port number
		return 0;
	}
	else
		return((int16)atoi(&c[j])); // return port number 
}

void copy_name (char * s, char * d, char end_char)
{
	int16 i = 0;
	
	while ((i <= 23) && (s[i] != CR) && (s[i] != 0))
	{
		d[i] = s[i];
		i++;
	}
	while (i <= 24)
		d[i++] = end_char;		// null characters at the end
}

/*	  
get ip

DHCP=SERVER
IP=192.168.1.201:2000
NM=255.255.255.0
GW=192.168.1.1
HOST=192.168.1.200:2000
PROTO=UDP,TCP,SECURE,TCP_CLIENT,HTTP,

get dns

DNS=0.0.0.0
Name=Veritas

get wlan

SSID=Veritas
Join=1
Passphrase=prodigalson
TxPower=12

get u

Baudrate=115200


*/


int16 Rcv_Net_Response (int16 comnmb, int16 net_cmd, bool init_flag)
{
  	int16 rbempt=0,endmess=0;
  	int16 temp=0;
  	int16 ret_response =  0;
  	
  	if (init_flag == 1)
	{
		net_rcv_ix[comnmb] = 0;
		net_rcv_st[comnmb] = 0;
		net_rcv_seq[comnmb] = 0;
		net_response[comnmb] = 0;
		timers[tcom_recv+comnmb] = 0;
		return(0);
	}
  		

	if (Tranbuf_Empty[comnmb] == 1)
	{
	  	rbempt = (uint8)isrxempty(comnmb);
	  	while((rbempt == 0) && (endmess == 0))
	  	{
		  	rmbuf[comnmb][net_rcv_ix[comnmb]] = (uint8)asigetc(comnmb);
		  	rbempt = (uint8)isrxempty(comnmb);
			switch (net_rcv_seq[comnmb])
			{
				case 0:		// data stored so look for CR
					if (rmbuf[comnmb][net_rcv_ix[comnmb]] == CR)
						net_rcv_seq[comnmb] = 1;
					break;
				case 1:		// Have CR look for LF
					if (rmbuf[comnmb][net_rcv_ix[comnmb]] == LF)
					{	// Decode packet
						endmess = 1;
						switch (net_cmd)
						{
							case 0: // CMD
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"CMD",3) == 0)
									net_response[comnmb] |= 0x01;
								if (net_response[comnmb] == 0x01) 
								{
									ret_response = 1;
									net_rcv_seq[comnmb] = 0;
								}
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
									net_rcv_seq[comnmb] = 0;
								}
								else ret_response = 0;
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;
								break;

							case 1: // get ip
							
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"DHCP=",5) == 0)
								{
									if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5],"OFF",3) == 0)
										dhcp = 0;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5],"ON",2) == 0)
										dhcp = 1;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5],"AUTOIP",6) == 0)
										dhcp = 2;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5],"CACHE",5) == 0)
										dhcp = 3;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5],"SERVER",6) == 0)
										dhcp = 4;
									else 
										dhcp = 0;
									net_response[comnmb] |= 0x01;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"IP=",3) == 0)
								{
									local_port = convert_ip((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+3],&car_ip_address[0]);
									net_response[comnmb] |= 0x02;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"NM=",3) == 0)
								{
									temp = convert_ip((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+3],&network_mask[0]);
									net_response[comnmb] |= 0x04;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"GW=",3) == 0)
								{
									temp = convert_ip((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+3],&gateway_ip_address[0]);
									net_response[comnmb] |= 0x08;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"HOST=",5) == 0)
								{
									host_port = convert_ip((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5],&host_ip_address[0]);
									net_response[comnmb] |= 0x10;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"PROTO=",6) == 0)
								{
									network_protocol = 0;
									net_rcv_st[comnmb] += 6;
									if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"UDP,",4) == 0)
									{
										network_protocol |= 0x01;
										net_rcv_st[comnmb] += 4;
									}
									if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"TCP,",4) == 0)
									{
										network_protocol |= 0x02;
										net_rcv_st[comnmb] += 4;
									}
									if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"SECURE,",7) == 0)
									{
										network_protocol |= 0x04;
										net_rcv_st[comnmb] += 7;
									}
									if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"TCP_CLIENT,",10) == 0)
									{
										network_protocol |= 0x08;
										net_rcv_st[comnmb] += 10;
									}
									if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"HTTP,",5) == 0)
									{
										network_protocol |= 0x10;
										net_rcv_st[comnmb] += 5;
									}
									net_response[comnmb] |= 0x20;
								}
								
								if (net_response[comnmb] == 0x3F) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;

								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;
									
								break;
								
							case 2: // get dns
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"DNS=",4) == 0)
								{
									temp = convert_ip((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+4],&dns_ip_address[0]);
									net_response[comnmb] |= 0x01;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"Name=",5) == 0)
								{
									copy_name((char *)&rmbuf[comnmb][net_rcv_st[comnmb]+5],dns_name,0);
									net_response[comnmb] |= 0x02;
								}
								
								if (net_response[comnmb] == 0x03) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;

								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
							
							case 3: // get wlan
							
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"SSID=",5) == 0)
								{
									copy_name((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5],network_ssid,0);
									net_response[comnmb] |= 0x01;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"Join=",5) == 0)
								{
									join_network = (int16)atoi((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5]);
									net_response[comnmb] |= 0x02;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"Passphrase=",11) == 0)
								{
									copy_name((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+11],network_password,0);
									net_response[comnmb] |= 0x04;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"TxPower=",8) == 0)
								{
									tx_power = (int16)atoi((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+8]);
									net_response[comnmb] |= 0x08;
								}
								
								if (net_response[comnmb] == 0x0F) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;

								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
								
							case 4:	// get uart
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"Baudrate=",9) == 0)
								{
									if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9],"2400",4) == 0)
										network_uart_baud = 1;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9],"4800",4) == 0)
										network_uart_baud = 2;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9],"9600",4) == 0)
										network_uart_baud = 3;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9],"19200",5) == 0)
										network_uart_baud = 4;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9],"38400",5) == 0)
										network_uart_baud = 5;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9],"57600",5) == 0)
										network_uart_baud = 6;
									else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9],"115200",6) == 0)
										network_uart_baud = 7;
									else 
										network_uart_baud = 0;
									net_response[comnmb] |= 0x01;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"Flow=",5) == 0)
								{
 									network_uart_flow = (int16)atoh((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+5]);
									net_response[comnmb] |= 0x02;
								}


								if (net_response[comnmb] == 0x03) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;

								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
								
							case 30: // get sys
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"Autoconn=",9) == 0)
								{
									auto_conn_time = (int16)atoi((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9]);
									net_response[comnmb] |= 0x01;
								}
								
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;

								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
								
							case 32: // get option
							
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"DeviceId=",9) == 0)
								{
									copy_name((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+9],device_id_name,0);
									net_response[comnmb] |= 0x01;
								}
								
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;

								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
								
							case 33: // get com
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"FlushTimer=",11) == 0)
								{
									com_flush_timer = (uint16)atoi((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+11]);
									net_response[comnmb] |= 0x01;
								}
/*								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"FlushSize=",10) == 0)
								{
									com_flush_timer = (uint16)atoi((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+10]);
									net_response[comnmb] |= 0x02;
								}
								else if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"MatchChar=",10) == 0)
								{
									com_match_char = (uint16)atoh((char*)&rmbuf[comnmb][net_rcv_st[comnmb]+10]);
									net_response[comnmb] |= 0x04;
								}
*/								
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;

								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
								
								
								
							case 5: // AOK
							case 6:
							case 7:
							case 8:
							case 9:
							case 10:
							case 11:
							case 12:
							case 13:
							case 14:
							case 15:
							case 16:
							case 17:
							case 18:
							case 19:
							case 20:
							case 28:
							case 29:
							case 31:
							case 34:
										//lint -fallthrough
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"AOK",3) == 0)
									net_response[comnmb] |= 0x01;
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 10)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;
								
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;

							case 21: // OPEN
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"*OPEN*",6) == 0)
									net_response[comnmb] |= 0x01;
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;
								
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;

							case 22: // CLOSE
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"*CLOSE*",7) == 0)
									net_response[comnmb] |= 0x01;
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;
								
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
								
							case 23:	// Join Network
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"Associated!",11) == 0)
									net_response[comnmb] |= 0x01;
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;
								
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
								
							case 24: 	//leave
								ret_response = 1;
								
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
								
							case 25: // EXIT
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"EXIT",4) == 0)
									net_response[comnmb] |= 0x01;
								if (net_response[comnmb] == 0x01)
								{
									ret_response = 1;
								}
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else 
								{
									ret_response = 0;
								}
								
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;

							case 26:	// reboot		  
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"*Reboot*",8) == 0)
									net_response[comnmb] |= 0x01;
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;
								
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
							
							case 27: // SAVE : Storing in config
								if (strncmp((char*)&rmbuf[comnmb][net_rcv_st[comnmb]],"Storing in config",17) == 0)
									net_response[comnmb] |= 0x01;
								if (net_response[comnmb] == 0x01) 
									ret_response = 1;
								else if (timers[tcom_recv+comnmb] > 3)
								{
									ret_response = -1;
									net_response[comnmb] = -1;
								}
								else ret_response = 0;
								
								net_rcv_st[comnmb] = net_rcv_ix[comnmb] + 1;
								net_rcv_seq[comnmb] = 0;

								break;
					
							default:
								ret_response = 0;
								break;
						}
					}
					if (rmbuf[comnmb][net_rcv_ix[comnmb]] == CR)
						net_rcv_seq[comnmb] = 1;
					else
						net_rcv_seq[comnmb] = 0;
					break;
					
				default:
					net_rcv_seq[comnmb] = 0;
					break;
			}
		  	net_rcv_ix[comnmb]++;
			if (net_rcv_ix[comnmb] > 1000)
			{
				rbempt = 1;
				net_rcv_ix[comnmb] = 0;
			}
		}
		if (timers[tcom_recv+comnmb] > 20)
		{
			ret_response = -1;
		}
	}
	else
	{
		net_rcv_ix[comnmb] = 0;
		net_rcv_st[comnmb] = 0;
	}
	return (ret_response);
}

// **************************************
//  Get all network variables
// **************************************

int16 Get_Net_Vars (int16 comnmb)
{
	int16 response = 0;
	
	switch (get_net_vars_seq[comnmb])
	{
		case 0:	// send get ip command
			Send_Net_Cmd(comnmb,1);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			get_net_vars_seq[comnmb] = 1;
			break;
		case 1:	// get ip command response
			response = Rcv_Net_Response(comnmb,1,0); 
			if (response == 1)
			 	get_net_vars_seq[comnmb] = 2;
			else if (response == -1)
				get_net_vars_seq[comnmb] = 0;
			break;
		case 2:	// send get dns command
			Send_Net_Cmd(comnmb,2);			
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			get_net_vars_seq[comnmb] = 3;
			break;
		case 3: // get dns command response
			response = Rcv_Net_Response(comnmb,2,0); 
			if (response == 1)
			 	get_net_vars_seq[comnmb] = 4;
			else if (response == -1)
				get_net_vars_seq[comnmb] = 0;
			break;
		case 4:	// send get wlan command
			Send_Net_Cmd(comnmb,3);			
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			get_net_vars_seq[comnmb] = 5;
			break;
		case 5: // get wlan command response
			response = Rcv_Net_Response(comnmb,3,0); 
			if (response == 1)
			 	get_net_vars_seq[comnmb] = 6;
			else if (response == -1)
				get_net_vars_seq[comnmb] = 0;
			break;
		case 6:	// send get u a command
			Send_Net_Cmd(comnmb,4);			
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			get_net_vars_seq[comnmb] = 7;
			break;
		case 7: // get u a command response
			response = Rcv_Net_Response(comnmb,4,0); 
			if (response == 1)  
			 	get_net_vars_seq[comnmb] = 8;
			else if (response == -1)		
			 	get_net_vars_seq[comnmb] = 0;
			break;
		case 8:	// send get sys command
			Send_Net_Cmd(comnmb,30);			
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			get_net_vars_seq[comnmb] = 9;
			break;
		case 9: // get sys command response
			response = Rcv_Net_Response(comnmb,30,0); 
			if (response == 1)  
			 	get_net_vars_seq[comnmb] = 10;
			else if (response == -1)		
			 	get_net_vars_seq[comnmb] = 0;
			break;
		case 10:	// send get option command
			Send_Net_Cmd(comnmb,32);			
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			get_net_vars_seq[comnmb] = 11;
			break;
		case 11: // get option command response
			response = Rcv_Net_Response(comnmb,32,0); 
			if (response == 1)  
			 	get_net_vars_seq[comnmb] = 12;
			else if (response == -1)		
			 	get_net_vars_seq[comnmb] = 0;
			break;
		case 12:	// send get com command
			Send_Net_Cmd(comnmb,33);			
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			get_net_vars_seq[comnmb] = 13;
			break;
		case 13: // get com command response
			response = Rcv_Net_Response(comnmb,33,0); 
			if (response == 1)  
			{
				net_cmd_resp[comnmb] = response;
			 	get_net_vars_seq[comnmb] = 0;
			}
			else if (response == -1)		
			 	get_net_vars_seq[comnmb] = 0;
			break;
			
		default:
			get_net_vars_seq[comnmb] = 0;
			break;
	}
	
	if (response == -1)
			net_cmd_resp[comnmb] = response;
	return get_net_vars_seq[comnmb];
}

// **************************************
//  Set all network variables
// **************************************

int16 Set_Net_Vars(int16 comnmb)
{
	int16 response = 0;
	
	switch (set_net_vars_seq[comnmb])
	{
		case 0:	// send dhcp data
			Send_Net_Cmd(comnmb,5);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 1;
			break;
		case 1:	// dhcp data response
			response = Rcv_Net_Response(comnmb,5,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 2;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 2:	// send ip address
			Send_Net_Cmd(comnmb,6);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 3;
			break;
		case 3:	// ip address response
			response = Rcv_Net_Response(comnmb,6,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 4;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 4:	// send local port
			Send_Net_Cmd(comnmb,7);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 5;
			break;
		case 5:	// local port response
			response = Rcv_Net_Response(comnmb,7,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 6;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 6:	// send network mask
			Send_Net_Cmd(comnmb,8);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 7;
			break;
		case 7:	// network mask response
			response = Rcv_Net_Response(comnmb,8,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 8;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 8:	// send gateway address
			Send_Net_Cmd(comnmb,9);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 9;
			break;
		case 9:	// gateway address response
			response = Rcv_Net_Response(comnmb,9,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 10;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 10:	// send host port
			Send_Net_Cmd(comnmb,10);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 11;
			break;
		case 11:	// host port response
			response = Rcv_Net_Response(comnmb,10,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 12;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 12:	// send host address
			Send_Net_Cmd(comnmb,11);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 13;
			break;
		case 13:	// host address response
			response = Rcv_Net_Response(comnmb,11,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 14;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 14:	// send network protocol
			Send_Net_Cmd(comnmb,12);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 15;
			break;
		case 15:	// network protocol response
			response = Rcv_Net_Response(comnmb,12,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 18;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 16:	// unused
			set_net_vars_seq[comnmb] = 18;
			break;
		case 17:	// unused
			set_net_vars_seq[comnmb] = 18;
			break;
		case 18:	// send dns address
			Send_Net_Cmd(comnmb,14);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 19;
			break;
		case 19:	// dns address response
			response = Rcv_Net_Response(comnmb,14,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 20;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 20:	// send dns name
			Send_Net_Cmd(comnmb,15);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 21;
			break;
		case 21:	// dns name response
			response = Rcv_Net_Response(comnmb,15,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 22;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 22:	// send SSID name
			Send_Net_Cmd(comnmb,16);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 23;
			break;
		case 23:	// SSID name response
			response = Rcv_Net_Response(comnmb,16,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 24;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 24:	// send join command
			Send_Net_Cmd(comnmb,17);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 25;
			break;
		case 25:	// join command response
			response = Rcv_Net_Response(comnmb,17,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 26;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 26:	// send password
			Send_Net_Cmd(comnmb,18);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 27;
			break;
		case 27:	// password response
			response = Rcv_Net_Response(comnmb,18,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 28;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 28:	// send tx power
			Send_Net_Cmd(comnmb,19);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 29;
			break;
		case 29:	// tx power response
			response = Rcv_Net_Response(comnmb,19,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 30;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 30:	// send baud rate
			Send_Net_Cmd(comnmb,20);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 31;
			break;
		case 31:	// baud rate response
			response = Rcv_Net_Response(comnmb,20,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 32;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 32:	// send flow command
			Send_Net_Cmd(comnmb,28);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 33;
			break;
		case 33:	// flow control response
			response = Rcv_Net_Response(comnmb,28,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 34;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 34:	// send auto connect time 
			Send_Net_Cmd(comnmb,29);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 35;
			break;
		case 35:	// auto connect response response
			response = Rcv_Net_Response(comnmb,29,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 36;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 36:	// send device id name 
			Send_Net_Cmd(comnmb,31);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 37;
			break;
		case 37:	// device id response response
			response = Rcv_Net_Response(comnmb,31,0); 
			if (response == 1)
				set_net_vars_seq[comnmb] = 38;
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
		case 38:	// send com timer 
			Send_Net_Cmd(comnmb,34);
			Rcv_Net_Response(comnmb,0,1);		// initialize state machine			
			set_net_vars_seq[comnmb] = 39;
			break;
		case 39:	// device id response response
			response = Rcv_Net_Response(comnmb,34,0); 
			if (response == 1)
			{
				net_cmd_resp[comnmb] = response;
				set_net_vars_seq[comnmb] = 0;
			}
			else if (response == -1)
				set_net_vars_seq[comnmb] = 0;
			break;
			
		default:
			set_net_vars_seq[comnmb] = 0;
			break;
	}
	
	if (response == -1)
		net_cmd_resp[comnmb] = response;
	
	return (set_net_vars_seq[comnmb]);	
}


/* 
Revision History

5/28/14 v7.1.1 mhd		1. Added flow control command/parameter.
9/29/14 v7.1.10 mhd		1. Added auto connect timer parameter.
						2. Empty receive buffer before sending command mode packet.
11/4/14 v7.1.19 mhd		1. Added get option command and set opt device id name.
1/4/16 v7.1.58 mhd		1. Added set com timer and get comm commands.

*/
