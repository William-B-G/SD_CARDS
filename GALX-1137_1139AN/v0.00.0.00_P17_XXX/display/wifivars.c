// WiFi Adjustable Variables
#define d_WIFIVARS 1
#include "global.h"


int16 wifi_menu(int16 lcd_menu);
void adjust_wifi_variable(void);
void display_wifi_variable(int16 var_ix);
void display_wifi_ip(int16 wifivar_ix);
void display_ascii_sel_line (void);
int16 set_ascii_ix (int16 wifivar_ix);
void insert_char (int16 wifivar_ix);
void insert_spaces (int16 wifivar_ix);
void delete_spaces (int16 wifivar_ix);
void clear_line	 (int16 wifivar_ix);
void display_wifi_label (int16 var, int16 text_ix);
void init_wifi_defaults(void);
void update_wifi(void);


static int16 upd_dpy;

// wifi variables

uint16 car_ip_address[4] = {192,168,10,50};
uint16 local_port = 5560;			
uint16 gateway_ip_address[4] = {192,168,10,10}; 	
uint16 host_ip_address[4] = {192,168,10,10};		
uint16 host_port = 5560;			
char network_ssid[25] = {"Galweb"};	
char network_password[25] = {"knockknock"};
char dns_name[25] =  {"GALaxy IV"};
uint16 dhcp = 1;	
int16 join_network = 1;
int16 network_uart_baud = 7;
int16 network_uart_flow = 1;		// enable flow control
uint16 dns_ip_address[4] = {0,0,0,0};
uint16 network_mask[4] = {255,255,255,0};
int16 network_protocol = 0x02;
int16 tx_power = 12;
int16 auto_conn_time = 30;
uint16 com_flush_timer = 1;		// default to 1 msec

#define dev_name_len 25
char device_id_name[dev_name_len] =  {"galcar~"};		// ~ will be replaced with car number




int16 wifi_setup_started = 0;
int16 baud_attempt = 0;
int16 wifi_vars_sent = 0;
int16 wifi_setup_complete;




char dummy_addr[4];	

int16 wifi_menu_level = 0;
int16 wifi_menu_sel = 0;
int16 wifi_par_ix = 0;
int16 ip_addr_ix = 0;
int16 wifi_var_menu_level = 0;
int16 wifi_menu_cmd = 0;
int16 wifi_message = 0;
	
int16 wifi_flash = 0;
int16 ascii_line_ix = 0;
int16 ascii_char_ix = 0;
int16 edit_char_ix = 0;
int16 edit_level = 0;
int16 Char_Line_Loc[2] = {0,0};
int16 Char_Flash_Loc[2] = {0,0};
char Char_Flash[2] = {'>',' '};

#define Num_WiFi_Menus 7

const char LCD_WiFi_Vars_Menu[Num_WiFi_Menus][21] = {
  " Enter WiFi Cmd Mode",
  " Read Vars from WiFi",
  " Set/View WiFi Vars ",
  " Send Var to WiFi Bd",
  " Save WiFi Board Var",
  " Reboot WiFi Device ",
  " Exit WiFi Cmd Mode ",
};


struct tag_wifi_par_tbl {
	int16 numpar;
	struct tag_wifi_par {
		int16 digit;
		int16 min;
		int16 max;
		char name [21];
		uint8 type;				// 0 = ip address, 1 = integer, 2 = ascii string, 3 = label
		int16 text_dpy_ix;
		void *ptr;
	} par[30];
} wifi_par_tbl = 

{
		// Wifi Parameters
		18,		// parameters - 1 used in table	
		{
			//{digit,min,max,"name",type,text_dpy_ix,data pointer}
		 	{0,0,255, 		"   Car IP Address   ",0,0,&car_ip_address},		// parameter  0, "name", type, address ptr
		 	{0,0,32000,		"    IP Local Port   ",1,1,&local_port},			// parameter  1, "name", type, address ptr
		 	{0,0,255,		"    Sub Net Mask    ",0,0,&network_mask},			// parameter  2, "name", type, address ptr
		 	{0,0,255,		" Gateway IP Address ",0,0,&gateway_ip_address},	// parameter  3, "name", type, address ptr
		 	{0,0,255,  		"   Host IP Address  ",0,0,&host_ip_address},		// parameter  4, "name", type, address ptr
		 	{0,0,32000,	    "    IP Host Port    ",1,0,&host_port},				// parameter  5, "name", type, address ptr
		 	{0,0,24,		"      DNS Name      ",2,0,&dns_name},				// parameter  6, "name", type, address ptr
			{0,0,255,		"   DNS IP Address   ",0,0,&dns_ip_address},		// parameter  7, "name", type, address ptr
		 	{0,0,4, 		"     DHCP Mode      ",3,1,&dhcp},					// parameter  8, "name", type, address ptr
			{0,0,31,		"    IP Protocol     ",3,2,&network_protocol},		// parameter  9, "name", type, address ptr
		 	{0,0,24,		" Network SSID (name)",2,0,&network_ssid},			// parameter 10, "name", type, address ptr
		 	{0,0,24,		"  Network Password  ",2,0,&network_password},		// parameter 11, "name", type, address ptr
		 	{0,0,1,	 	 	" Join Network Mode  ",3,3,&join_network},			// parameter 12, "name", type, address ptr
			{0,0,12,		"Antenna Tx Power dB ",1,0,&tx_power},				// parameter 13, "name", type, address ptr
		 	{0,1,7,			" Network Uart Baud  ",3,4,&network_uart_baud},		// parameter 14, "name", type, address ptr
			{0,0,1,			"Network Uart FlowCtl",3,5,&network_uart_flow},		// parameter 15, "name", type, address ptr
			{0,0,60,		" Auto Connect Timer ",1,0,&auto_conn_time},		// parameter 16, "name", type, address ptr
			{0,0,24,		"   Device ID Name   ",2,0,&device_id_name},		// parameter 17, "name", type, address ptr
			{0,0,50,		"  Comm Flush Timer  ",1,0,&com_flush_timer},		// parameter 18, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 19, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 20, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 21, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 22, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 23, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 24, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 25, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 26, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 27, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 28, "name", type, address ptr
			{0,0,0,			"                    ",0,0,&dummy_addr},			// parameter 29, "name", type, address ptr
		},
};


const char ascii_adj[7][13] = 
{
	"ABCDEFGHIJKLM",
	"NOPQRSTUVWXYZ",
	"0123456789!~'",
	"=+-* .:;?@/#&",
	"abcdefghijklm",
	"nopqrstuvwxyz",
	"  Clear All  ",
};

const char LCD_DHCP_Mode[5][21] = {
  "0=DHCP Off:Static IP",
  "1=DHCP On:Acc Pnt IP",
  "2=Auto IP: No DHCP  ",
  "3=DHCP Cache:Prev IP",
  "4=DHCP Server:AP Mod",
};

// PROTO=UDP,TCP,SECURE,TCP_CLIENT,HTTP,

const char LCD_Network_Protocol[6][7] = {
  "Not Set",
  "UDP,   ",
  "TCP,   ",
  "SEC,   ",
  "TCP_CL,",
  "HTTP,  ",
};

const char nplen[6] = {7,4,4,4,7,5};

const char LCD_Network_join[2][21] = {
  "0=Join Net Manually ",
  "1=Join Net Automatic",
};


const char LCD_Network_Uart_Baud[8][21] = {
  "  0 = Ivalid Baud   ",
  "  1 = 2400   Baud   ",
  "  2 = 4800   Baud   ",
  "  3 = 9600   Baud   ",
  "  4 = 19200  Baud   ",
  "  5 = 38400  Baud   ",
  "  6 = 57600  Baud   ",
  "  7 = 115200 Baud   ",
};

const char LCD_Network_Uart_Flow[2][21] = {
  "0 = No Flow Control ",
  "1 = RTS/CTS Flow Ctl",
};


const char LCD_Wifi_Message[10][21] = {
  "    Please Wait ... ",	  	// 0
  "Not In Command Mode ",		// 1
  "Time-Out No Cmd Mode",		// 2
  "   In Command Mode  ",		// 3
  "  Command Timed-Out ",		// 4
  " Wifi Vars Received ",		// 5
  " Wifi Variables Sent",		// 6
  "Wifi Variables Saved",		// 7
  " Command Mode Exited",		// 8
  "Wifi Device Rebooted",		// 9
};

void init_wifi_defaults(void)
{
	int16 i;
	
	
	for (i=0;i<dev_name_len;i++)
	{
		if (device_id_name[i] == '~')
		{
			device_id_name[i] = (char)(cons[carnmb] + '0');	// change ~ to car number
			break;
		}
	}

}
	
//********************************
// Display Wifi Variable Label 
//********************************

void display_wifi_label (int16 var, int16 text_ix)
{
	int16 i,j;
	
	switch (text_ix)
	{
		
		case 0:	// no display
			break;
		case 1:	
		    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[2][i] = LCD_DHCP_Mode[var][i];
		    }
		    break;
		case 2:	// display network protocol
			if (var == 0)
				strncpy(&LCD_Display[2][0],LCD_Network_Protocol[0],nplen[0]);
			else
			{
				j = 0;
				for(i=0; i<5; i++)
				{
					if ((var & bittbl[i]) != 0)
					{
						strncpy(&LCD_Display[2][j],LCD_Network_Protocol[i+1],nplen[i+1]);
						j += nplen[i+1];
					}
				}
				for (i=j;i<=19;i++)
					LCD_Display[2][i] = ' ';
			}
			break;
		case 3:	   // Join Network Mode
			if (var > 1)
				var = 0;
		    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[2][i] = LCD_Network_join[var][i];
		    }
		    break;
		case 4:	  	//  Network uart Baudrate
			if (var > 7)
				var = 0;
		    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[2][i] = LCD_Network_Uart_Baud[var][i];
		    }
		    break;
			
		case 5:	   // Network Uart Flow Control
			if (var > 1)
				var = 0;
		    for(i=0; i<=19; i++)
	  	    {
				LCD_Display[2][i] = LCD_Network_Uart_Flow[var][i];
		    }
		    break;
		    
		default:
			break;
	}
		
}


//********************************
// Display ascii char select line 
//********************************
void display_ascii_sel_line (void)
{
	int16 i;
	
	for(i=0; i<=3; i++)
	{
		LCD_Display[3][i] = ' ';
	}
	for(i=0; i<=12; i++)
	{
		LCD_Display[3][i+4] = ascii_adj[ascii_line_ix][i];
	}
	for(i=17; i<=19; i++)
	{
		LCD_Display[3][i] = ' ';
	}
}


//********************************
// Set indexes to ascii table
//********************************

int16 set_ascii_ix (int16 wifivar_ix)
{
	int16 i;
	int16 k = 0;
	int16 found = 0;
	char ascii_char;
	char *vptr8; 

	vptr8 = (char *)wifi_par_tbl.par[wifivar_ix].ptr;
	ascii_char = *(vptr8 + edit_char_ix);
	ascii_line_ix = 0;
	ascii_char_ix = 0;
	while ((k<6) && (found == 0))
	{
		for (i=0;i<13;i++)
		{
			if (ascii_char == ascii_adj[k][i])
			{
				ascii_line_ix = k;
				ascii_char_ix = i;
				found = 1;
				return (found);
			}
		}
		k++;
	}
	
	return (found);

}

//********************************
// Insert Character into Parameter
//********************************

void insert_char (int16 wifivar_ix)
{
	char *s; 
	if (wifi_par_tbl.par[wifivar_ix].type == 2)
	{
		s = (char *)wifi_par_tbl.par[wifivar_ix].ptr;
		s[edit_char_ix] = ascii_adj[ascii_line_ix][ascii_char_ix];
	}
}

//********************************
// Insert Spaces into Parameter
//********************************

void insert_spaces (int16 wifivar_ix)
{
	int16 i;
	char *s; 

	if (wifi_par_tbl.par[wifivar_ix].type == 2)
	{
		s = (char *)wifi_par_tbl.par[wifivar_ix].ptr;
		for(i=0;i<=19;i++)
		{
			if ((s[i] < ' ') || (s[i] > 'z') )
			{
				s[i] = ' ';
			}
		}
		s[i] = 0;
	}
}

//********************************
// Clear Parameter Line
//********************************

void clear_line	 (int16 wifivar_ix)
{
	int16 i;
	char *s; 

	if (wifi_par_tbl.par[wifivar_ix].type == 2)
	{
		s = (char *)wifi_par_tbl.par[wifivar_ix].ptr;
		for(i=0;i<=19;i++)
		{
			s[i] = ' ';
		}
		s[i] = 0;		// null character
	}
}

//********************************
// Delete spaces Parameter
//********************************
void delete_spaces (int16 wifivar_ix)
{
	int16 i;
	int16 k;
	int16 last_space;
	char *s; 

	if (wifi_par_tbl.par[wifivar_ix].type == 2)
	{
		s = (char *)wifi_par_tbl.par[wifivar_ix].ptr;
		i = 0;
		k = 0;
		last_space = 0;
		while (i<=19)
		{
			if (s[i] != ' ') 
				last_space = 0;
			else if (last_space == 0)
				last_space = i;
			i++;
		}
		if (last_space != 0)		
			i = last_space;
		while (i <= 24)
			s[i++] = 0;
	}
}

			
//********************************
// Display WiFi Variable
//********************************

void display_wifi_variable(int16 wifivar_ix)
{
	int16 i,k;
	uint16 *vptr16;
	uint8 *s;
 	
	if (wifi_par_tbl.par[wifivar_ix].type == 0)
	{			// ip address
		vptr16 = (uint16 *)wifi_par_tbl.par[wifivar_ix].ptr;
			
	    itoa((*vptr16 & 0x00ff), LCD_String);
		i=0;
		k = 4;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[2][k++] = LCD_String[i++];
		}
		LCD_Display[2][k++] = '.';

	    itoa((*(vptr16+1) & 0x00ff), LCD_String);
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[2][(k++)] = LCD_String[i++];
		}
		LCD_Display[2][k++] = '.';
	
	    itoa((*(vptr16+2) & 0x00ff), LCD_String);
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[2][(k++)] = LCD_String[i++];
		}
		LCD_Display[2][k++] = '.';
	
	    itoa((*(vptr16+3) & 0x00ff), LCD_String);
		i=0;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[2][(k++)] = LCD_String[i++];
		}

	}
	else if (wifi_par_tbl.par[wifivar_ix].type == 1)
	{		 // integer
		vptr16 = (uint16 *)wifi_par_tbl.par[wifivar_ix].ptr;
	    itoa(*vptr16, LCD_String);
		i=0;
		k = 8;
		while((LCD_String[i] >= '0') && (LCD_String[i] <= '9'))
		{
			LCD_Display[2][k++] = LCD_String[i++];
		}
	}
	else if (wifi_par_tbl.par[wifivar_ix].type == 2)
	{			// Ascii
		s = (uint8 *)wifi_par_tbl.par[wifivar_ix].ptr;
		i=0;
		while((i<=19) && (s[i] != CR) && (s[i] != 0))
		{
			LCD_Display[2][i++] = s[i];
		}
	}
	else if (wifi_par_tbl.par[wifivar_ix].type == 3)
	{
		display_wifi_label(*(int16 *)wifi_par_tbl.par[wifi_par_ix].ptr,wifi_par_tbl.par[wifi_par_ix].text_dpy_ix);
	}
}


//***********************************
// Display WiFi IP
//***********************************

void display_wifi_ip(int16 wifivar_ix)
{
	int16 k;
	int16 *vptr16;
 	
	vptr16 = (int16 *)wifi_par_tbl.par[wifivar_ix].ptr;
			
		
	k = 3;
	sprintf(&LCD_Display[2][k],"%03i.",*vptr16);
	k += 4;
	sprintf(&LCD_Display[2][k],"%03i.",*(vptr16+1));
	k += 4;
	sprintf(&LCD_Display[2][k],"%03i.",*(vptr16+2));
	k += 4;
	sprintf(&LCD_Display[2][k],"%03i",*(vptr16+3));
	k += 3;
	LCD_Display[2][k++] = ' ';
	LCD_Display[2][k++] = ' ';
}

//********************************
// Adjust WiFi Variable
//********************************

void adjust_wifi_variable(void)
{
	int16 i;
	int16 *vptr16;
	
	if (wifi_var_menu_level == 0)
	{
		wifi_var_menu_level = 1;
	    upd_dpy = 1;
	    LCD_Dig_Point = 0;
	    ip_addr_ix = 0;	   
	}
	if (wifi_var_menu_level == 1)
	{
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    wifi_var_menu_level = (int16)(wifi_par_tbl.par[wifi_par_ix].type + 2);
		    if (wifi_var_menu_level == 4)
		    {
//		    	set_ascii_ix (wifi_par_ix);
		    	insert_spaces (wifi_par_ix);
		    	ascii_line_ix = 0;
		    	ascii_char_ix = 0;
		    }
		    edit_level = 0;
		    edit_char_ix = 0;
		    LCD_Dig_Point = 0;
		    ip_addr_ix = 0;	   // index to show three zeros so number can be adjusted
		    return;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			wifi_menu_level = 0;
			wifi_menu_sel = 2;
		    ModeButton = 1;  
		    upd_dpy = 1;
			return;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
			wifi_par_ix++;
			if(wifi_par_ix > wifi_par_tbl.numpar)	  // Last wifi vars 
			    wifi_par_ix = 0;
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
			wifi_par_ix--;
			if(wifi_par_ix < 0)
			    wifi_par_ix =  wifi_par_tbl.numpar;	 // Last wifi vars
		}

		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
				LCD_Display[0][i] = getcaps(LCD_WiFi_Vars_Menu[2][i]);
				LCD_Display[1][i] = wifi_par_tbl.par[wifi_par_ix].name[i];
				LCD_Display[2][i] = ' ';
				LCD_Display[3][i] = ' ';
	    	}
	    	display_wifi_variable(wifi_par_ix);
		}
	}
	else if (wifi_var_menu_level == 2)
	{		// adjust ip address	  type 0
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
			vptr16 = (int16 *)wifi_par_tbl.par[wifi_par_ix].ptr;
		    adjust_variable (vptr16 + ip_addr_ix, 
		    					(int16)wifi_par_tbl.par[wifi_par_ix].max, 
		    					wifi_par_tbl.par[wifi_par_ix].min, 1, LCD_Dig_Point);
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
			vptr16 = (int16 *)wifi_par_tbl.par[wifi_par_ix].ptr;
		    adjust_variable (vptr16 + ip_addr_ix, 
		    					(int16)wifi_par_tbl.par[wifi_par_ix].max, 
		    					wifi_par_tbl.par[wifi_par_ix].min, 0, LCD_Dig_Point);
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
			upd_dpy = 1;
			LCD_Dig_Point = set_adj_digit(wifi_par_tbl.par[wifi_par_ix].max,LCD_Dig_Point);
		}
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    LCD_Dig_Point = 0;
			if (ip_addr_ix < 3)
				ip_addr_ix++;
			else
			{
				ip_addr_ix = 0;
			    wifi_var_menu_level = 0;
			    return;
			}
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			display_wifi_ip(wifi_par_ix);
			LCD_Dig_Loc =  (ip_addr_ix * 4) + 5 - LCD_Dig_Point;
			LCD_Flash_TMP[1] = LCD_Display[2][LCD_Dig_Loc];
		}
  		flash_digit(LCD_Flash_TMP[1], 2,LCD_Dig_Loc,1);
	}
	else if (wifi_var_menu_level == 3)
	{		// adjust port number	 type 1
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    adjust_variable ((int16 *)wifi_par_tbl.par[wifi_par_ix].ptr, 
		    					(int16)wifi_par_tbl.par[wifi_par_ix].max, 
		    					wifi_par_tbl.par[wifi_par_ix].min, 1, LCD_Dig_Point);
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    adjust_variable ((int16 *)wifi_par_tbl.par[wifi_par_ix].ptr, 
		    					(int16)wifi_par_tbl.par[wifi_par_ix].max, 
		    					wifi_par_tbl.par[wifi_par_ix].min, 0, LCD_Dig_Point);
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
		    upd_dpy = 1;
			LCD_Dig_Point = set_adj_digit(wifi_par_tbl.par[wifi_par_ix].max,LCD_Dig_Point);
		}
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
			ip_addr_ix = 0;
		    wifi_var_menu_level = 0;
		    LCD_Dig_Point = 0;
			return;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			display_variable(2, 6, wifi_par_tbl.par[wifi_par_ix].digit, *(int16 *)wifi_par_tbl.par[wifi_par_ix].ptr, 1);
			LCD_Dig_Loc = 11 - LCD_Dig_Point;
			LCD_Flash_TMP[1] = LCD_Display[2][LCD_Dig_Loc];
		}
  		flash_digit(LCD_Flash_TMP[1], 2,LCD_Dig_Loc,1);
	}
	else if (wifi_var_menu_level == 4)
	{		// edit ascii name			   type 2
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    if (edit_level == 0)
		    {
		    	if (edit_char_ix > 0)
		    		edit_char_ix--;
		    	else
		    		edit_char_ix = 19;
		    }
		 	else if (edit_level == 1)
		    {
		    	if (ascii_line_ix != 6)
		    	{
				    if (ascii_char_ix > 0)
				    	ascii_char_ix--;
				    else
				    	ascii_char_ix = 12;
					insert_char (wifi_par_ix);
		    	}
		    }
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    if (edit_level == 0)
		    {
		    	if (edit_char_ix < 19)
		    		edit_char_ix++;
		    	else
		    		edit_char_ix = 0;
		    }
		    else if (edit_level == 1)
		    {
			    	
		    	if (ascii_line_ix != 6)
		    	{
					if (ascii_char_ix < 12)
						ascii_char_ix++;
					else
						ascii_char_ix = 0;
					insert_char (wifi_par_ix);
		    	}
		    }
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
		    upd_dpy = 1;
		    if (edit_level == 0)
		    {
			    wifi_var_menu_level = 0;
		    	edit_level = 0;
		    	delete_spaces(wifi_par_ix);
		    }
			else
			{
							    	
		    	if (ascii_line_ix < 6)
		    		ascii_line_ix++;
		    	else
		    		ascii_line_ix = 0;
			    	

		    	if (ascii_line_ix != 6)
					ascii_char_ix = 0;
		    	else
					ascii_char_ix = 11;
		    	
				insert_char (wifi_par_ix);
			}
		}
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
		    if (edit_level == 0)
		    {
		    	edit_level = 1;
    			set_ascii_ix (wifi_par_ix);
		    }
		    else
		    {
		    	if (ascii_line_ix == 6)
		    	{
		    		clear_line(wifi_par_ix);
		    		ascii_line_ix = 0;
		    		ascii_char_ix = 0;
		    		edit_char_ix = 0;
		    		edit_level = 0;
		    	}
		    	else
		    	{
					edit_level = 0;
		    	}
		    }
		}

		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			display_ascii_sel_line();
	    	display_wifi_variable(wifi_par_ix);
	    	
	    	if (edit_level == 0)
	    	{
				Char_Flash_Loc[1] = edit_char_ix;
				Char_Flash[1] = LCD_Display[2][Char_Flash_Loc[1]];
				Char_Line_Loc[1] = 2;

	  			if (Char_Flash[1] == ' ')
					Char_Flash[1] = (char)'_';		// change from 0xDB which cause display problem

	    	}
	    	else if (edit_level == 1) 
	    	{
				Char_Flash_Loc[0] = ascii_char_ix + 4;
				Char_Flash[0] = LCD_Display[3][Char_Flash_Loc[0]];
				Char_Line_Loc[0] = 3;

				if (Char_Flash[0] == ' ')
					Char_Flash[0] = (char)'_';

	    	}
		}
		if (edit_level == 1)
	  		flash_digit(Char_Flash[0], Char_Line_Loc[0],Char_Flash_Loc[0],1);
		else
	  		flash_digit(Char_Flash[1], Char_Line_Loc[1],Char_Flash_Loc[1],1);
	}
	else if (wifi_var_menu_level == 5)
	{		// adjust label	   type 3
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
		    adjust_variable ((int16 *)wifi_par_tbl.par[wifi_par_ix].ptr, 
		    					(int16)wifi_par_tbl.par[wifi_par_ix].max, 
		    					wifi_par_tbl.par[wifi_par_ix].min, 1, LCD_Dig_Point);
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
		    adjust_variable ((int16 *)wifi_par_tbl.par[wifi_par_ix].ptr, 
		    					(int16)wifi_par_tbl.par[wifi_par_ix].max, 
		    					wifi_par_tbl.par[wifi_par_ix].min, 0, LCD_Dig_Point);
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
		    ModeButton = 1;  
		    upd_dpy = 1;
		    LCD_Flash = 0;		// force the label to be displayed instead of the blank line
		}
	  	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
			ip_addr_ix = 0;
		    wifi_var_menu_level = 0;
		    LCD_Dig_Point = 0;
			return;
		}
		if (wifi_flash != LCD_Flash)
		{
			upd_dpy = 1;
			wifi_flash = LCD_Flash;
		}
			
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			if (LCD_Flash == 0)
			{
		    	for(i=0; i<=19; i++)
					LCD_Display[2][i] = ' ';
			}
			else
				display_wifi_label(*(int16 *)wifi_par_tbl.par[wifi_par_ix].ptr,wifi_par_tbl.par[wifi_par_ix].text_dpy_ix);
		}
	}
}

//********************************
// WiFi Variables
//********************************

int16 wifi_menu(int16 lcd_menu)
{
	int16 i;
	int16 last_menu;
	int16 start_ptr;
	int16 line_ptr;
	
	last_menu = Num_WiFi_Menus - 1;
	
	if (PasswordFlag == false)
   	{
  		StoreLCDInfo();
		clrLCDdpy();
  		LCD_Menu = 25;
  		LCD_Init = 1;
		return(lcd_menu);
   	}
	if(LCD_Sub_Init == 1)	
	{
    	LCD_Sub_Init = 0;
	    upd_dpy = 1;
	    wifi_menu_level = 0;
	    wifi_menu_sel = 0;
	    wifi_par_ix = 0;
	    ip_addr_ix = 0;
 	}
	if (wifi_menu_level == 0)
	{
		
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
			wifi_menu_sel--;
		    if (wifi_menu_sel < 0)
		    	wifi_menu_sel = last_menu;
		}

		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    wifi_menu_sel++;
		    if (wifi_menu_sel > last_menu)
		    	wifi_menu_sel = 0;
		    upd_dpy = 1;
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    upd_dpy = 1;
	   		wifi_menu_level = wifi_menu_sel+1;
	   		wifi_var_menu_level = 0;
	   		wifi_menu_cmd = 0;
		    wifi_par_ix = 0;
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
	  		ModeButton = 1;  
		    wifi_menu_sel = 0;
		    wifi_menu_level = 0;
		    wifi_par_ix = 0;
	  		lcd_menu = 0;
	  		return(lcd_menu);
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			line_ptr = (wifi_menu_sel % 3);
			start_ptr = wifi_menu_sel - line_ptr;
		    for(i=0; i<=19; i++)
		    {
	  			LCD_Display[0][i] = getcaps(LCD_Software_Menu[6][i]);
			  	LCD_Display[1][i] = LCD_WiFi_Vars_Menu[start_ptr][i];
			  	if ((start_ptr + 1) > last_menu)
			  	{
			  		LCD_Display[2][i] = ' ';
			  		LCD_Display[3][i] = ' ';
			  	}
			  	else 
			  	{
				  	LCD_Display[2][i] = LCD_WiFi_Vars_Menu[start_ptr+1][i];
			  		if ((start_ptr + 2) > last_menu)
				  		LCD_Display[3][i] = ' ';
		    		else
					  	LCD_Display[3][i] = LCD_WiFi_Vars_Menu[start_ptr+2][i];
			  	}
		    }
			LCD_Display[line_ptr+1][0] = '>';		// show cursor
		}
	}
	else if (wifi_menu_level == 1)
	{   	// Enter Wifi Command Mode
	
		if (wifi_menu_cmd == 0)
		{
			wifi_menu_cmd = 1;
			net_cmd_resp[0] = 0;
			net_cmd_mode_req[0] = 1;
			in_net_cmd_mode[0] = 0;
			wifi_message = 0;	// please wait
			upd_dpy = 1;
		}
		
		if (in_net_cmd_mode[0] == -1)
		{
			wifi_message = 2;		// timed out - not in command mode
			upd_dpy = 1;
		}
		else if (in_net_cmd_mode[0] == 1)
		{
			wifi_message = 3;	// in command mode
			upd_dpy = 1;
		}
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_WiFi_Vars_Menu[wifi_menu_sel][i];
				LCD_Display[2][i] = LCD_Wifi_Message[wifi_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			wifi_menu_level = 0;
			wifi_menu_sel = 0;
			wifi_menu_cmd = 0;
		    ModeButton = 1;  
		    upd_dpy = 1;
	  		return(lcd_menu);
		}
	}
	else if (wifi_menu_level == 2)
	{   	// read wifi variable
		if (wifi_menu_cmd == 0)
		{
			wifi_menu_cmd = 1;
			net_cmd_resp[0] = 0;
			
			upd_dpy = 1;
			if (in_net_cmd_mode[0] == 1)
			{
				wifi_message = 0;	// please wait
				net_cmd_req[0] = 1;		// request a get variable command
			}
			else
			{
				wifi_message = 1;	// not in command mode
			}
		}
		
		if (net_cmd_resp[0] == -1)
		{
			wifi_message = 4;		// timed out
			upd_dpy = 1;
		}
		else if (net_cmd_resp[0] == 1)
		{
			wifi_message = 5;	// variables received
			upd_dpy = 1;
		}
		
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_WiFi_Vars_Menu[wifi_menu_sel][i];
				LCD_Display[2][i] = LCD_Wifi_Message[wifi_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
			
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			wifi_menu_level = 0;
			wifi_menu_sel = 1;
		    ModeButton = 1;  
			upd_dpy = 1;
	  		return(lcd_menu);
		}
	}
	else if (wifi_menu_level == 3)
	{
		adjust_wifi_variable();
	}
	else if (wifi_menu_level == 4)
	{		// send local variables to wifi
		if (wifi_menu_cmd == 0)
		{
			wifi_menu_cmd = 1;
			net_cmd_resp[0] = 0;
			
			upd_dpy = 1;
			if (in_net_cmd_mode[0] == 1)
			{
				wifi_message = 0;	// please wait
				net_cmd_req[0] = 2;		// request a send variable to wifi card command
			}
			else
			{
				wifi_message = 1;	// not in command mode
			}
		}
		
		if (net_cmd_resp[0] == -1)
		{
			wifi_message = 4;		// timed out
			upd_dpy = 1;
		}
		else if (net_cmd_resp[0] == 1)
		{
			wifi_message = 6;	// variables sent
			upd_dpy = 1;
		}
		
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_WiFi_Vars_Menu[wifi_menu_sel][i];
				LCD_Display[2][i] = LCD_Wifi_Message[wifi_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
			
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			wifi_menu_level = 0;
			wifi_menu_sel = 3;
		    ModeButton = 1;  
			upd_dpy = 1;
	  		return(lcd_menu);
		}
	}
	else if (wifi_menu_level == 5)
	{		// save wifi variables on card
		if (wifi_menu_cmd == 0)
		{
			wifi_menu_cmd = 1;
			net_cmd_resp[0] = 0;
			
			upd_dpy = 1;
			if (in_net_cmd_mode[0] == 1)
			{
				wifi_message = 0;	// please wait
				net_cmd_req[0] = 3;		// request a send save variable to wifi card command
			}
			else
			{
				wifi_message = 1;	// not in command mode
			}
		}
		
		if (net_cmd_resp[0] == -1)
		{
			wifi_message = 4;		// timed out
			upd_dpy = 1;
		}
		else if (net_cmd_resp[0] == 1)
		{
			wifi_message = 7;	// variables saved
			upd_dpy = 1;
		}
		
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_WiFi_Vars_Menu[wifi_menu_sel][i];
				LCD_Display[2][i] = LCD_Wifi_Message[wifi_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
			
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			wifi_menu_level = 0;
			wifi_menu_sel = 4;
		    ModeButton = 1;
		    upd_dpy = 1;  
	  		return(lcd_menu);
		}
	}
	else if (wifi_menu_level == 6)
	{		// Send reboot command
		if (wifi_menu_cmd == 0)
		{
			wifi_menu_cmd = 1;
			net_cmd_resp[0] = 0;
			
			upd_dpy = 1;
			if (in_net_cmd_mode[0] == 1)
			{
				wifi_message = 0;	// please wait
				net_cmd_req[0] = 5;		// request reboot device
			}
			else
			{
				wifi_message = 1;	// not in command mode
			}
		}
		
		if (net_cmd_resp[0] == -1)
		{
			wifi_message = 4;		// timed out
			upd_dpy = 1;
		}
		else if (net_cmd_resp[0] == 1)
		{
			wifi_message = 9;	//reboot command received
			upd_dpy = 1;
		}
		
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_WiFi_Vars_Menu[wifi_menu_sel][i];
				LCD_Display[2][i] = LCD_Wifi_Message[wifi_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
			
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			wifi_menu_level = 0;
			wifi_menu_sel = 5;
		    ModeButton = 1;  
		    upd_dpy = 1;
	  		return(lcd_menu);
		}
	}
	else if (wifi_menu_level == 7)
	{		// exit wifi command mode
		if (wifi_menu_cmd == 0)
		{
			wifi_menu_cmd = 1;
			upd_dpy = 1;
			wifi_message = 0;	// please wait
			net_cmd_resp[0] = 0;
			net_cmd_req[0] = 4;		// request to exit command mode
			in_net_cmd_mode[0] = 1;	  // incase flag is incorrectly set 
			net_cmd_mode_req[0] = 0;
		}
		
		if (net_cmd_resp[0] == -1)
		{
			wifi_message = 4;		// timed out
			upd_dpy = 1;
		}
		else if (net_cmd_resp[0] == 1)
		{
			wifi_message = 8;	// not in command mode
			upd_dpy = 1;
		}
		
		if (upd_dpy == 1)
		{
			upd_dpy = 0;
	    	for(i=0; i<=19; i++)
	    	{
	    		LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_WiFi_Vars_Menu[wifi_menu_sel][i];
				LCD_Display[2][i] = LCD_Wifi_Message[wifi_message][i];
	    		LCD_Display[3][i] = ' ';
	    	}
		}
			
		if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		{
			wifi_menu_level = 0;
			wifi_menu_sel = 6;
		    ModeButton = 1;  
		    upd_dpy = 1;
	  		return(lcd_menu);
		}
	}

	return(lcd_menu);
}





//********************************
// WiFi Variables
//********************************
void update_wifi(void)
{
	int16 i;
	static int16 auto_wifi_command = 0;
	//static int16 baud_attempt = 0;
	int16 response = 0;
	
	static int16 command_attempt;
	static int16 start_command;
	
	// attempt with the correct comm port and baud setting on comm 1
	if (fvars[fvcomdiag1] != 6)
	{
		fvars[fvcomdiag1] = 6;
		wrfvar();
	}

	// first time in menu
	if (wifi_setup_started == 0)
	{
    	wifi_setup_started = 1;
	    auto_wifi_command = 0;
	    baud_attempt = 0;
	    wifi_menu_cmd = 0;
	    response = 0;
	    wifi_vars_sent = 0;
		start_command = 0;
	}

	if ((wifi_setup_started == 1) && (wifi_setup_complete == 0))
	{
		if (timers[tautowifi] > 50)	// give at least 3 seconds to send command and to allow LCD to update message
		{
			switch (auto_wifi_command)
			{
				case 0://try to put card in command mode
					if (start_command == 0)
					{
						start_command = 1;
						command_attempt = 0;
					}
					
					if (wifi_menu_cmd == 0)
					{
						wifi_menu_cmd = 1;
						net_cmd_resp[0] = 0;
						net_cmd_mode_req[0] = 1;
						in_net_cmd_mode[0] = 0;
					}
					
					if (in_net_cmd_mode[0] == 1)// we are in command mode
					{
						auto_wifi_command++;
						timers[tautowifi] = 0;
						wifi_menu_cmd = 0;
						wifi_message = 3;
						upd_dpy = 1;
						start_command = 0;
					}
					else if ((in_net_cmd_mode[0] == -1) && (timers[tautowifi] > 60))
					{
						if (baud_attempt < 8)
						{

							// try another baud rate
							fvars[fvbaud] = baud_attempt;
							wrfvar();
							send_galcom_fvar(MAIN_CPU, fvbaud);
							baud_attempt++;

							// reset variables for retry
							wifi_menu_cmd = 0;
							auto_wifi_command = 0;
							timers[tautowifi] = 0;	// reset timer
							
							wifi_message = 12;
							upd_dpy = 1;
						}
						else// timed out - not in commmand mode
						{
							if (command_attempt < 3)
							{
								command_attempt++;
								wifi_menu_cmd = 0;
								for(i=0; i<command_attempt; i++)
						    	{
									LCD_Display[3][i+3] = (char)(io_on);
						    	}
							}
							else
							{
								auto_wifi_command = -1;
								wifi_setup_complete = 2;// setup failed
									
								net_cmd_resp[0] = 0;
								net_cmd_req[0] = 4;		// request to exit command mode
								in_net_cmd_mode[0] = 1;	  // incase flag is incorrectly set 
								net_cmd_mode_req[0] = 0;
								
								wifi_message = 2;
								upd_dpy = 1;
							}
						}
							
					}
					
					if (upd_dpy == 1)
					{
						upd_dpy = 0;
				    	for(i=0; i<=19; i++)
				    	{
							LCD_Display[1][i] = LCD_Wifi_Message[wifi_message][i];
							LCD_Display[2][i] = LCD_Network_Uart_Baud[fvars[fvbaud]][i];
				    	}
					}
					break;
				case 1:// send local variables to wifi
					if (start_command == 0)
					{
						start_command = 1;
						command_attempt = 0;
					}
					
					if (wifi_menu_cmd == 0)
					{
						wifi_menu_cmd = 1;
						net_cmd_resp[0] = 0;
						
						upd_dpy = 1;
						if (in_net_cmd_mode[0] == 1)// we are in command mode
						{
							// Set network to minimum baud rate of 115200
							if (network_uart_baud < 6)
								network_uart_baud = 6;
						
							net_cmd_req[0] = 2;	// request a send variable to wifi card command
							wifi_message = 0;
							upd_dpy = 1;
							//Proc_Net_Cmd(USR1COM);
						}
						else// not in command mode
						{
							auto_wifi_command = -1;
							wifi_setup_complete = 2;// setup failed
							wifi_message = 2;
							upd_dpy = 1;
						}
					}
					
					if (net_cmd_resp[0] == -1)// timed out - not in command mode
					{
						if (command_attempt < 3)
						{
							command_attempt++;
							wifi_menu_cmd = 0;
							for(i=0; i<command_attempt; i++)
						    {
								LCD_Display[3][i+3] = (char)(io_on);
						    }
						}
						else
						{
							auto_wifi_command = -1;
							wifi_setup_complete = 2;// setup failed
							wifi_message = 2;
							upd_dpy = 1;
						}
					}
					else if ((net_cmd_resp[0] == 1) || (wifi_vars_sent == 1))// variables sent
					{
						auto_wifi_command++;
						timers[tautowifi] = 0;
						wifi_menu_cmd = 0;
						wifi_message = 6;
						upd_dpy = 1;
						start_command = 0;
					}
					
					if (upd_dpy == 1)
					{
						upd_dpy = 0;
				    	for(i=0; i<=19; i++)
				    	{
							LCD_Display[1][i] = LCD_Wifi_Message[wifi_message][i];
							LCD_Display[2][i] = ' ';
				    	}
					}
					break;
				case 2:// save local variables to the wifi card
					if (start_command == 0)
					{
						start_command = 1;
						command_attempt = 0;
					}
					
					if (wifi_menu_cmd == 0)
					{
						wifi_menu_cmd = 1;
						net_cmd_resp[0] = 0;
						
						upd_dpy = 1;
						if (in_net_cmd_mode[0] == 1)// we are in commmand mode
						{
							net_cmd_req[0] = 3;		// request a send save variable to wifi card command
							wifi_message = 0;
							upd_dpy = 1;
						}
						else// not in command mode
						{
							auto_wifi_command = -1;
							wifi_setup_complete = 2;// setup failed
							wifi_message = 2;
							upd_dpy = 1;
						}
					}
					
					if (net_cmd_resp[0] == -1)
					{
						if (command_attempt < 3)
						{
							command_attempt++;
							wifi_menu_cmd = 0;
							for(i=0; i<command_attempt; i++)
						    {
								LCD_Display[3][i+3] = (char)(io_on);
						    }
						}
						else
						{
							wifi_message = 4;		// timed out
							upd_dpy = 1;
							auto_wifi_command = -1;
							wifi_setup_complete = 2;// setup failed
						}
					}
					else if (net_cmd_resp[0] == 1)// variables saved
					{
						auto_wifi_command++;
						timers[tautowifi] = 0;
						wifi_menu_cmd = 0;
						wifi_message = 7;
						upd_dpy = 1;
						start_command = 0;
					}
					
					if (upd_dpy == 1)
					{
						upd_dpy = 0;
				    	for(i=0; i<=19; i++)
				    	{
							LCD_Display[1][i] = LCD_Wifi_Message[wifi_message][i];
							LCD_Display[2][i] = ' ';
							LCD_Display[3][i] = ' ';
				    	}
					}
					break;
				case 3:// reboot wifi card to exit command mode
					if (start_command == 0)
					{
						start_command = 1;
						command_attempt = 0;
					}
					
					if (wifi_menu_cmd == 0)
					{
						wifi_menu_cmd = 1;
						net_cmd_resp[0] = 0;
						
						upd_dpy = 1;
						if (in_net_cmd_mode[0] == 1)// we are in command mode
						{
							net_cmd_req[0] = 5;		// request reboot device
							wifi_message = 0;
							upd_dpy = 1;
						}
						else// not in command mode
						{
							auto_wifi_command = -1;
							wifi_setup_complete = 2;// setup failed
							wifi_message = 2;
							upd_dpy = 1;
						}
					}
					if (net_cmd_resp[0] == -1)// timed out - not in command mode					
					{
						if (command_attempt < 3)
						{
							command_attempt++;
							wifi_menu_cmd = 0;
							for(i=0; i<command_attempt; i++)
						    {
								LCD_Display[3][i+3] = (char)(io_on);
						    }
						}
						else
						{
							auto_wifi_command = -1;
							wifi_setup_complete = 2;// setup failed
							wifi_message = 2;
							upd_dpy = 1;	
						}
					}
					else if (net_cmd_resp[0] == 1)// reboot command received
					{
						//auto_wifi_command++;
						timers[tautowifi] = 0;
						wifi_menu_cmd = 0;
						wifi_message = 9;
						upd_dpy = 1;
						wifi_setup_complete = 1;//setup is complete
						// set the baud rate and save it
						fvars[fvbaud] = network_uart_baud;
						wrfvar();
						send_galcom_fvar(MAIN_CPU, fvbaud);
						wifi_vars_sent = 0;
						start_command = 0;
					}
					
					if (upd_dpy == 1)
					{
						upd_dpy = 0;
				    	for(i=0; i<=19; i++)
				    	{
							LCD_Display[1][i] = LCD_Wifi_Message[wifi_message][i];
							LCD_Display[2][i] = ' ';
				    	}
					}
					break;
				default:
					timers[tautowifi] = 0;
					wifi_menu_cmd = 0;
					baud_attempt = 0;
					break;
			}
		}
	}
}
/* 
Revision History

5/28/14 v7.1.1 mhd	1. Added flow control command/parameter.		  
8/1/14 v7.1.6 mhd	1. When editing an ascii string, changed code to not exit from the enter button (only swap modes).  Only exit from the mode button.
9/26/14 v7.1.10 mhd	1. Changed the ascii edit to shift right with the down button and left with the up.
					2. Added Reboot Wifi device command.
					3. Added auto connect timer parameter.
					4. Deleted Open host port command
11/5/14 v7.1.19 mhd	1. Added device id name parameter.
3/16/15 V7.1.27 mhd	1. Changed device_id_name GALcar 1 to galcar1.
1/5/16 v7.1.58 mhd 	1. Added Comm Flush Timer

*/
