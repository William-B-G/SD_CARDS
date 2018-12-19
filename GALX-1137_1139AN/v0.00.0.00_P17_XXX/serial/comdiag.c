//  Comm Routines
#define d_COMDIAG 1
#include "global.h"


int16 Loop1_ix = 0;
int16 Loop2_ix = 0;
int16 max_pkt_len = 135;
int16 com_nmb_io_brd = 33;


int16 diag_flt_ix=0;
int16 reset_flt_ix=0;
int16 send_flt_data;

extern struct CdTable Com_CodeTable[2];
extern struct Cd_Tbl_Chk Com_Code_Tbl_Chk[2][fl_size];

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
		

void comm_diag (int16 com_nmb);
void Init_Modem(int16 com_nmb);
void Modem_Quiet_Mode(int16 com_nmb);


//********************************
// Initialize Modem
//********************************
void Init_Modem(int16 com_nmb)
{
//      sendc(USR1COM,'~');
//      sendc(USR1COM,'~');
//      sendc(USR1COM,'~');
//      sendc(USR1COM,'+');
//      sendc(USR1COM,'+');
//      sendc(USR1COM,'+');
//      sendc(USR1COM,'~');
//      sendc(USR1COM,'~');
//      sendc(USR1COM,'~');
      sendc(com_nmb,'A');
      sendc(com_nmb,'T');
      sendc(com_nmb,'S');
      sendc(com_nmb,'0');
      sendc(com_nmb,'=');
      sendc(com_nmb,'1');
      sendc(com_nmb,CR);
}

//********************************
// Set Modem to Quiet Mode
//********************************
void Modem_Quiet_Mode (int16 com_nmb)
{
      sendc(com_nmb,'A');
      sendc(com_nmb,'T');
      sendc(com_nmb,'Q');
      sendc(com_nmb,'1');
      sendc(com_nmb,CR);
}

// ********************************************************
// This is comm diagnostic human interface routine
// ********************************************************

void comm_diag (int16 com_nmb)
{
  	int16 i,j,k,qq,bbb,car = 1;
	int16 ck_sum;
	int16 Var_Nmb;
	int16 brdnmb,ionmb;
	int16 el;

#if (Traction == 1)
	int16 m;
#endif
#if(Security == 1)
  	int16 k,K,L,M;
  	int16 Tuser_name[20];
  	int16 Tuser_access[23];
  	int16 Tuser_code[5];
#endif
  	int32 tcc_set[2];
	int16 fl_ix;

	if (com_nmb > USR2COM)			// some memory arrays are only 2 deep
		return;

	if (Com_Recv_Int[com_nmb] == 1)
		Com_Recv_Int[com_nmb] = 0;

/*
	if ((com_nmb == USR1COM) || (com_nmb == USR2COM))
	{
		modem_status[com_nmb] = inp(com[com_nmb][c_modem_stat]);
	  	if ((modem_status[com_nmb] & 0x20) == 0x20) // Checking if DSR line set (terminal connected)
		{
			if (connect[com_nmb] == 0)			// Reset comm first time connected
			{
				Restart_Com(com_nmb);
				init_modem[com_nmb] = 1;
				Com_Mode[com_nmb] = 0;
			}
			connect[com_nmb] = 1;

			if ((modem_status[com_nmb] & 0x80) == 0x80)	// Carrier detected on the modem
				modem_dcd[com_nmb] = 1;
			else
			{
				if(modem_dcd[com_nmb] == 1)
				{
					init_modem[com_nmb] = 1;			// re-initialize the modem after carrier is lost
					modem_state[com_nmb] = 0;
					Com_Mode[com_nmb] = 0;
				}
				modem_dcd[com_nmb] = 0;
			}
			if ((cons[modem] == 1) && (init_modem[com_nmb] == 1))
			{			   // Initialize modem if modem used
			 	switch (modem_state[com_nmb])
			 	{
					case 0:
						timers[tuser_com + com_nmb] = 0;
						modem_state[com_nmb] = 1;
						break;
			 		case 1:
			 			if (timers[tuser_com + com_nmb] > 20)
			 			{
				 			Init_Modem(com_nmb);		// Init modem	
				 			timers[tuser_com + com_nmb] = 0;
				 			modem_state[com_nmb] = 2;
						}
			 			break;
			 		case 2:
			 			if (timers[tuser_com + com_nmb] > 20)
			 			{
							Modem_Quiet_Mode(com_nmb);		// Set quiet mode
							init_modem[com_nmb] = 0;
							modem_state[com_nmb] = 0;
						}
						break;
					default:
						break;
				}
			}
			else
			{			 // ok to reset the timer in normal operation
				if (Tranbuf_Empty[com_nmb] == 0)
				{		// Transmitter has data
					if (timers[tuser_com + com_nmb] > 50)
					{	// Transmitter is not running (should be empty by now)
						Restart_Com(com_nmb);
						timers[tuser_com + com_nmb] = 0;
						init_modem[com_nmb] = 1;
						modem_state[com_nmb] = 0;
					}
				}
				else
					timers[tuser_com + com_nmb] = 0;
			}
		}
		else 
		{
			if (connect[com_nmb] == 1)
			{
				connect[com_nmb] = 0;
				Com_Mode[com_nmb] = 0;
			}
			timers[tuser_com + com_nmb] = 0;
			modem_state[com_nmb] = 0;
			modem_dcd[com_nmb] = 0;
			init_modem[com_nmb] = 0;
		}
	}
*/
	if (com_nmb == USR1COM)
	{		// COM1 
		if (Tranbuf_Empty[com_nmb] == 0)
		{
			if ((USR1port.SR.B.TDRE == 1) && (USR1port.CR1.B.TIE == 1))  
			// Interupt source is transmiter hold reg empty
			{
				if (timers[tcom1_tran+com_nmb] > 2)
				{
					Restart_Com(com_nmb);
				}
			}
	 		else
	 			timers[tcom1_tran+com_nmb] = 0;
		}
	 	else
	 		timers[tcom1_tran+com_nmb] = 0;
	}
	else if (com_nmb == USR2COM)
	{		// COM2
		if (Tranbuf_Empty[com_nmb] == 0)
		{
			if ((USR2port.SR.B.TDRE == 1) && (USR2port.CR1.B.TIE == 1))  // Interupt source is transmiter hold reg empty
			{
				if (timers[tcom1_tran+com_nmb] > 2)
				{
					Restart_Com(com_nmb);
				}
			}
	 		else
	 			timers[tcom1_tran+com_nmb] = 0;
		}
	 	else
	 		timers[tcom1_tran+com_nmb] = 0;
	}

	if (connect[com_nmb] == 1)
	{
		if (timers[tuser_com + com_nmb] > 30)
		{			// reset com once after 3 seconds
			connect[com_nmb] = 0;
			Restart_Com(com_nmb);
		}
	}

  	i = recvm(com_nmb);
  	if(i != 0)
  	{
      	Com_Mode[com_nmb] = i;
      	connect[com_nmb] = 1;
		timers[tuser_com + com_nmb] = 0;
  	}

	switch (Com_Mode[com_nmb])
	{
		case 1:					 // Send the first 120 field variables
		  	if(Tranbuf_Empty[com_nmb])
		  	{		
				local_getdate();
				local_gettime();
		      	fvars[fvyear] = d.year;
		      	fvars[fvmon] = d.month;
		      	fvars[fvday] = d.day;
		      	fvars[fvhour] = t.hour;
		      	fvars[fvmin] = t.minute;

		      	sendc(com_nmb,0xff);
		      	sendc(com_nmb,0xff);
				if (galcom_ver >= 2)
					j = 119;
				else
					j = 59;

				// always send checksum
				qq = 0;
		      	for(i=0; i<=j; i++)
		      	{									   // calculate checksum
			  		qq += fvars[i];
		      	}
				sendc(com_nmb,qq & 0x0ff);					// send checksum
				sendc(com_nmb,((qq >> 8) & 0x00ff));

		      	for(i=0; i<=j; i++)
		      	{
			  		sendc(com_nmb,fvars[i] & 0x00ff);
			  		sendc(com_nmb,((fvars[i] >> 8) & 0x00ff));
		      	}
		      	sendc(com_nmb,'V');
				Com_Mode[com_nmb] = 0;
		  	}
			break;
		case 2:				  // Receive the first 120 field variables
			// Always get the checksum
			bbb = (uint16)(rmbuf[com_nmb][3]&0x00ff);		// read in checksum
			bbb |= (uint16)((rmbuf[com_nmb][4] << 8)&0xff00);
			ck_sum = 0;
			if (galcom_ver >= 2)
				j = 240;
			else
				j = 120;
		    for(i=0; i<j; i=(i+2))
		    {				// calculate checksum
			  	qq = (uint16)(rmbuf[com_nmb][i+5]&0xff);
			  	qq |= (uint16)((rmbuf[com_nmb][i+6] << 8)&0xff00);
				ck_sum += qq;
		    }
			if (ck_sum == ((int16)bbb))
			{		// if checksum matches then update variables
				Var_Nmb = 0;
			  	for(i=0; i<j; i=(i+2))
			  	{	   
					if(Var_Nmb != fvbaud)
					{	// do not allow the laptop update the serial comm baud rate
					  	qq = (uint16)(rmbuf[com_nmb][i+5]&0xff);
					  	qq |= (uint16)((rmbuf[com_nmb][i+6] << 8)&0xff00);
						if ((qq >= fvars_min[Var_Nmb]) && (qq <= fvars_max[Var_Nmb]))
					  		fvars[Var_Nmb] = qq;
			  		}
					Var_Nmb++;
			  	}
			  	wrfvar();
			}
			Com_Mode[com_nmb] = 1;
			break;

		case 3:					
		  	if(Tranbuf_Empty[com_nmb])
		  	{		
		      	sendc(com_nmb,'I');
//				if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 3)))
					sendc(com_nmb,cons[carnmb]);
		      	for(i=0; i<com_nmb_io[com_nmb]; i++)
				  	sendc(com_nmb,iodata[cons[carnmb]][i]);
		      	sendc(com_nmb,procf);
		      	sendc(com_nmb,doorf);
		      	sendc(com_nmb,empf);
		      	sendc(com_nmb,firef);
		      	sendc(com_nmb,servf);
		      	sendc(com_nmb,rdoorf);

				sendc(com_nmb,position);
				sendc(com_nmb,dirf);
				sendc(com_nmb,dpref);
						
				uwu.w = Current_Fault;		
				sendc(com_nmb,uwu.b[lbw]);		
//				sendc(com_nmb,Current_Fault);
				uiu.dw = statusf;		
				sendc(com_nmb,uiu.b[lb]);		
				sendc(com_nmb,uiu.b[mlb]);		
		#if (Traction == 1)
				sendc(com_nmb,Enc_dir);
				uwu.w = (uint16)(((Dmd_Velocity * 5.0)/(Pulses_per_Inch)) + 0.5);	// demand velocity
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uwu.w = (uint16)(((Enc_vel * 5.0)/(Pulses_per_Inch)) + 0.5);		// encoder velocity
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uiu.dw = DPP_Floor_Pos[position];
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				uiu.dw = DPP_Count;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
		#else			// no encoder, velocity or pulse count
				sendc(com_nmb,0);		// ENC_DIR
				sendc(com_nmb,0);		// VELOCITY
				sendc(com_nmb,0);		// Velocity
				sendc(com_nmb,0);		// Enc Velocity
				sendc(com_nmb,0);		// Enc Velocity
				sendc(com_nmb,0);		// Dpp_Floor_Pos
				sendc(com_nmb,0);		// Dpp_Floor_Pos
				sendc(com_nmb,0);		// Dpp_Floor_Pos
				sendc(com_nmb,0);		// Dpp_Floor_Pos
				sendc(com_nmb,0);		// Dpp_Coount
				sendc(com_nmb,0);		// Dpp_Coount
				sendc(com_nmb,0);		// Dpp_Coount
				sendc(com_nmb,0);		// Dpp_Coount
		#endif

//				if (galcom_ver >= 2)
				{
					uiu.dw = statusf;
					sendc(com_nmb,uiu.b[mhb]);		
					sendc(com_nmb,uiu.b[hb]);		
				}

//				if (galcom_ver >= 6)
				{
					uwu.w = Current_Fault;		
					sendc(com_nmb,uwu.b[hbw]);		
				}

//				if (user_com_ver[com_nmb] >= 1)
				{
					sendc(com_nmb,ins_status);	
					sendc(com_nmb,fire_status);	
					uiu.dw = statusf2;
					sendc(com_nmb,uiu.b[lb]);	
					sendc(com_nmb,uiu.b[mlb]);	
					sendc(com_nmb,uiu.b[mhb]);	
					sendc(com_nmb,uiu.b[hb]);
					sendc(com_nmb,power_status);
					uwu.w = safety_string_status;
					sendc(com_nmb,uwu.b[lbw]);	
					sendc(com_nmb,uwu.b[hbw]);	
				}
		      	sendc(com_nmb,'O');
	    		Com_Mode[com_nmb] = 0;
		  	}
			break;

		case 4:
		  	if(Tranbuf_Empty[com_nmb])
		  	{
		      	sendc(com_nmb,'S');
		      	for(i=0; i<=12; i++)
		      	{
					uiu.dw = lvars[i];
			  		sendc(com_nmb,uiu.b[lb]);
			  		sendc(com_nmb,uiu.b[mlb]);
			  		sendc(com_nmb,uiu.b[mhb]);
			  		sendc(com_nmb,uiu.b[hb]);
		      	}
		      	sendc(com_nmb,'T');
	    		Com_Mode[com_nmb] = 0;
		  	}
			break;

		case 5:		 // Set car call for this car
			set_carcall(cons[carnmb],(uint16)(rmbuf[com_nmb][3] & 0xff));
	    	Com_Mode[com_nmb] = 0;
			break;

		case 6:		  // Set up call
			set_upcall((uint16)(rmbuf[com_nmb][3] & 0xff));
   		 	Com_Mode[com_nmb] = 0;
			break;
		
		case 7:		// Set date and time
	    	d.year = rmbuf[com_nmb][3];
	    	d.year = (uint16)((rmbuf[com_nmb][4] << 4) | d.year);
	    	d.month = rmbuf[com_nmb][5];
	    	d.day = rmbuf[com_nmb][6];
	    	t.hour = rmbuf[com_nmb][7];
	    	t.minute = rmbuf[com_nmb][8];
			local_setdate();
			local_settime();
	    	Com_Mode[com_nmb] = 0;
			break;

		case 8:		   // Send eta data for each car
		  	if (Tranbuf_Empty[com_nmb])
		  	{		  
		      	sendc(com_nmb,'E');
		      	for(j=1; j<=com_nmb_car[com_nmb]; j++)
		      	{
				  	for(i=2; i<=com_nmb_fl[com_nmb]; i++)
				  	{
						sendc(com_nmb,car_dn_eta[j][i]);
				  	}
				  	for(i=1; i< com_nmb_fl[com_nmb]; i++)
				  	{
						sendc(com_nmb,car_up_eta[j][i]);
				  	}
		      	}
		      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      	{
				  	sendc(com_nmb,grp_Car_Stat[i].g_nxt_stp);
		      	}
		      	sendc(com_nmb,'A');
		  	}
			break;

		case 9:	  // Send faults for this car
  			if (Tranbuf_Empty[com_nmb]) 
  			{			
      			sendc(com_nmb,'F');
				if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 3)))
					sendc(com_nmb,cons[carnmb]);
				if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 6)))
				{
					if (reset_flt_ix == 1)
					{
						diag_flt_ix = Flt_ix + 1;
						if (diag_flt_ix >= max_flt_ix)
				    		diag_flt_ix = 0;
					}
					i = 0;
					if (galcom_ver >= 6)
						max_pkt_len = 150;
					else
						max_pkt_len = 135;

					if(reset_flt_ix != 1 && Loop2_ix == 0)		// added if-else statement
					{
						while (i < max_pkt_len)
						{
							sendc(com_nmb, 0);
							i++;
						}
					}
					else
					{
						while ((Loop2_ix < max_flt_ix) && (i < max_pkt_len))
						{
							if (f.Flts[diag_flt_ix].code != 0)
							{
								sendc(com_nmb,f.Flts[diag_flt_ix].code);
								if (galcom_ver >= 6)
									sendc(com_nmb,(uint16)((f.Flts[diag_flt_ix].code >> 8) & 0x00ff));	// high byte
								sendc(com_nmb,f.Flts[diag_flt_ix].day);                         // day
								sendc(com_nmb,f.Flts[diag_flt_ix].mon);                         // month
								sendc(com_nmb,(f.Flts[diag_flt_ix].year >> 8) & 0x00FF);     // HIGH BYTE year 
								sendc(com_nmb,(f.Flts[diag_flt_ix].year & 0x00FF));          // LOW BYTE year 
								sendc(com_nmb,f.Flts[diag_flt_ix].min);                         // minute
								sendc(com_nmb,f.Flts[diag_flt_ix].hour);                         // hour
								sendc(com_nmb,f.Flts[diag_flt_ix].pos);                         // position
								sendc(com_nmb,(uint16)(f.Flts[diag_flt_ix].count & 0x00ff));              // low byte nmb occur
								if (galcom_ver >= 6)				 
									i+=10;
								else
									i+=9;
							}
							diag_flt_ix++;	  
							if (diag_flt_ix >= max_flt_ix)
								diag_flt_ix = 0;
							Loop2_ix++;
						}

						if (Loop2_ix >= max_flt_ix)
						{
							//diag_flt_ix = 0;
							Loop2_ix = 0;
							while (i < max_pkt_len)
							{				// Packet must be a fixed length so fill with zero's
								sendc(com_nmb,0);
								i++;
							}
						}
					}	
				}
				else
				{
		      		for(i=1; i<=15; i++)
		      		{
						sendc(com_nmb,f.Flts[diag_flt_ix].day);                         // day
						sendc(com_nmb,f.Flts[diag_flt_ix].mon);                         // month
						if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 3)))
						{
							sendc(com_nmb,(f.Flts[diag_flt_ix].year >> 8) & 0x00FF);     // HIGH BYTE year 
							sendc(com_nmb,(f.Flts[diag_flt_ix].year & 0x00FF));          // LOW BYTE year 
						}
						sendc(com_nmb,f.Flts[diag_flt_ix].min);                         // minute
						sendc(com_nmb,f.Flts[diag_flt_ix].hour);                         // hour
						sendc(com_nmb,f.Flts[diag_flt_ix].pos);                         // position
						sendc(com_nmb,0);              // low byte nmb occur
		      		}
				}	//end else statement
	      		sendc(com_nmb,'T');
				Com_Mode[com_nmb] = 0;
			}
			reset_flt_ix = 0;	//moved to the bottom
			break;
		
		case 10:	// Clear faults for any car
		    if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 3)))
	 		{ 
	 			if((rmbuf[com_nmb][3] >= 1) && (rmbuf[com_nmb][3] <= cons[nmbcars]))
	     		{
	 				grp_Car_Stat_req[com_nmb] = rmbuf[com_nmb][3];
	 			}
	 			else
	 				grp_Car_Stat_req[com_nmb] = 0;
	 		}
	 		else
	 			grp_Car_Stat_req[com_nmb] = cons[carnmb];
	 	   
			
	 		if (grp_Car_Stat_req[com_nmb] !=0)
	 		{	
				if(grp_Car_Stat_req[com_nmb] == cons[carnmb])
	 			{
	 		      	for(i=0; i<max_flt_ix; i++)
	 		      	{
	 					f.Flts[i].code = 0;
	 					f.Flts[i].count = 0;
	 		      	}
	 				Flt_ix = 0;
	 			  	Update_All_Flt();
	 			}				
	 			else
	 			{
	 				put_pkt_req(10,grp_Car_Stat_req[com_nmb],gc_port);
	 			}
	 		}
	       	Com_Mode[com_nmb] = 0;
			break;

		case 11:	   // Log on  user
	      	for(j=0; j<=6; j++)
	      	{
		      	i = 3;
		      	while((rmbuf[com_nmb][i] == user[j][(i-3)]) && (user[j][(i-3)] != 13))
		      	{
			    	i++;
		      	}
		      	if((user[j][(i-3)] == 13) && (i > 5))             // good user
		      	{
			   		sendc(com_nmb,'U');
	#if (auto_elevator == 1)
			   		sendc(com_nmb,1);
	#else
				   	sendc(com_nmb,2);
	#endif
			   		sendc(com_nmb,'S');
			   		logon[com_nmb] = 2;
			   		User_Log[com_nmb] = j;
					com_nmb_fl[com_nmb] = cons[grtopf];	
	#if (Tract_HR == 1)
//					com_nmb_io[com_nmb] = 145;	// 145 instead of 85 
					com_nmb_io[com_nmb] = 157;	// 155 instead of 85 
	#else
//					com_nmb_io[com_nmb] = 85;	// 85 instead of 84: index 39 gets sent twice 
					com_nmb_io[com_nmb] = 97;	 
	#endif
					com_nmb_car[com_nmb] = cons[nmbcars];
					j = 7;			// exit for loop since valid user found
		      	}
			}
			
	      	if(logon[com_nmb] != 2)          // not a valid user
	      	{
				sendc(com_nmb,'U');
				sendc(com_nmb,0);
				sendc(com_nmb,'S');
				logon[com_nmb] = 0;
	      	}
	      	Com_Mode[com_nmb] = 0;
			break;
		
		case 12:
		  	if((Com_Mode[com_nmb] == 12) && (logon[com_nmb] == 2))	   // log on password
		  	{
		      	j = User_Log[com_nmb];
		      	i = 3;
		      	while((rmbuf[com_nmb][i] == pass[j][(i-3)]) && (pass[j][(i-3)] != 13))
		      	{
			    	i++;
		      	}
		      	if((pass[j][(i-3)] == 13) && (i > 5))             // good password
		      	{
			   		sendc(com_nmb,'U');
		#if (auto_elevator == 1)
			   		sendc(com_nmb,1);
		#else
			   		sendc(com_nmb,2);
		#endif
			   		sendc(com_nmb,'S');
			   		logon[com_nmb] = 3;
		      	}
		      	if(logon[com_nmb] != 3)          // not a valid password
		      	{
					sendc(com_nmb,'U');
					sendc(com_nmb,0);
					sendc(com_nmb,'S');
					logon[com_nmb] = 0;
		      	}
		      	Com_Mode[com_nmb] = 0;
		  	}
			break;

		case 13:
			sendc(com_nmb,'U');
	#if (auto_elevator == 1)
			sendc(com_nmb,1);
	#else
			sendc(com_nmb,2);
	#endif
			sendc(com_nmb,'S');
			logon[com_nmb] = 0;
			Com_Mode[com_nmb] = 0;
			break;

		case 14:
			if((rmbuf[com_nmb][3] > 1) && (rmbuf[com_nmb][3] < 7))
			{
				i = 4;
				while( (rmbuf[com_nmb][i] != 13) && (i < 25) )
				{
					user[rmbuf[com_nmb][3]][(i-4)] = rmbuf[com_nmb][i];
					i++;
				}
				user[rmbuf[com_nmb][3]][(i-4)] = CR;
				sendc(com_nmb,'U');
	#if (auto_elevator == 1)
				sendc(com_nmb,1);
	#else
				sendc(com_nmb,2);
	#endif
				sendc(com_nmb,'S');
				wruvar();
			}
			Com_Mode[com_nmb] = 0;
			break;

		case 15:			 // New Password
			if((rmbuf[com_nmb][3] > 1) && (rmbuf[com_nmb][3] < 7))
			{
				i = 4;
				while( (rmbuf[com_nmb][i] != 13) && (i < 25) )
				{
					pass[rmbuf[com_nmb][3]][(i-4)] = rmbuf[com_nmb][i];
					i++;
				}
				pass[rmbuf[com_nmb][3]][(i-4)] = CR;
				sendc(com_nmb,'U');
	#if (auto_elevator == 1)
				sendc(com_nmb,1);
	#else
				sendc(com_nmb,2);
	#endif
				sendc(com_nmb,'S');
				wruvar();
			}
			Com_Mode[com_nmb] = 0;
	  		break;
	  
	  	case 16:	 // Group initializaton data
			sendc(com_nmb,'S');
			for(i=1; i<=com_nmb_fl[com_nmb]; i++)
				sendc(com_nmb,flmrk[i][1]);
			for(i=1; i<=com_nmb_fl[com_nmb]; i++)
				sendc(com_nmb,flmrk[i][0]);
			sendc(com_nmb,(cons[carnmb] & 0xff));
			sendc(com_nmb,(cons[grtopf] & 0xff));
			sendc(com_nmb,(cons[nmbcars] & 0xff));
			sendc(com_nmb,(cons[rear] & 0xff));
			sendc(com_nmb,'T');
			Com_Mode[com_nmb] = 0;
			break;

		case 17:	   // Group data packet
		  	if (Tranbuf_Empty[com_nmb])	
		  	{
		      	sendc(com_nmb,'V');
		      	for(i=1; i<=(com_nmb_fl[com_nmb] - 1); i++)
		      	{
			      	sendc(com_nmb,g_asign_up[i]);
		      	}
		      	for(i=2; i<=com_nmb_fl[com_nmb]; i++)
		      	{
			      	sendc(com_nmb,g_asign_dn[i]);
		      	}
		      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      	{
			      	sendc(com_nmb,grp_Car_Stat[i].g_pos);
		      	}
		      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      	{
					sendc(com_nmb,grp_Car_Stat[i].g_dpref);
		      	}
		      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      	{
			      	sendc(com_nmb,grp_Car_Stat[i].g_servf);
		      	}
		      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      	{
					tcc_set[0] = 0;
					tcc_set[1] = 0;
					if ((rear_op[i] != 0) && (i >= 1) && (Car_Not_Talking[i] == 0))
					{ 
						for(qq=1; qq<=com_nmb_fl[com_nmb]; qq++)
						{
							fl_ix = (qq-1)/32;				 
							if ((grp_Car_Stat[i].g_rcc[fl_ix] & fl_mask[qq]) != 0)
					       		tcc_set[fl_ix] |= fl_mask[qq];
							else
								tcc_set[fl_ix] &= ~fl_mask[qq];
						}
					}
					else
					{
						tcc_set[0] = 0;
						tcc_set[1] = 0;
					}
					uiu.dw = tcc_set[0];
					sendc(com_nmb,uiu.b[lb]);
					if (com_nmb_fl[com_nmb] > 8)
						sendc(com_nmb,uiu.b[mlb]);
					if (com_nmb_fl[com_nmb] > 16)
						sendc(com_nmb,uiu.b[mhb]);
					if (com_nmb_fl[com_nmb] > 24)
						sendc(com_nmb,uiu.b[hb]);
					uiu.dw = tcc_set[1];
					if (com_nmb_fl[com_nmb] > 32)
						sendc(com_nmb,uiu.b[lb]);
					if (com_nmb_fl[com_nmb] > 40)
						sendc(com_nmb,uiu.b[mlb]);
					if (com_nmb_fl[com_nmb] > 48)
						sendc(com_nmb,uiu.b[mhb]);
					if (com_nmb_fl[com_nmb] > 56)
						sendc(com_nmb,uiu.b[hb]);
		      	}
		      	sendc(com_nmb,firef);
		      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      	{
			      	sendc(com_nmb,grp_Car_Stat[i].g_doorf);
		      	}
		      	for(i=2; i<=com_nmb_fl[com_nmb]; i++)
		      	{
					if (g_asign_dn[i] == 0)
						sendc(com_nmb,car_dn_eta[g_asign_dnr[i]][i]);		
					else 
						sendc(com_nmb,car_dn_eta[g_asign_dn[i]][i]);		
				}
		      	for(i=1; i<com_nmb_fl[com_nmb]; i++)			
				{
					if (g_asign_up[i] == 0)
						sendc(com_nmb,car_up_eta[g_asign_upr[i]][i]);		
					else
						sendc(com_nmb,car_up_eta[g_asign_up[i]][i]);		
		      	}
		      	for(i=1; i<=(com_nmb_fl[com_nmb] - 1); i++)
		      	{
			      	sendc(com_nmb,g_asign_upr[i]);
		      	}
		      	for(i=2; i<=com_nmb_fl[com_nmb]; i++)
		      	{
			      	sendc(com_nmb,g_asign_dnr[i]);
		      	}


		      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      	{
					tcc_set[0] = 0;
					tcc_set[1] = 0;
					if (Car_Not_Talking[i] == 0)
					{ 
						for(qq=1; qq<=com_nmb_fl[com_nmb]; qq++)
						{
							fl_ix = (qq-1)/32;
							if ((grp_Car_Stat[i].g_cc[fl_ix] & fl_mask[qq]) != 0)
					       		tcc_set[fl_ix] |= fl_mask[qq];
							else
								tcc_set[fl_ix] &= ~fl_mask[qq];
						}
					}
					else	// else we don't have valid data
					{
						tcc_set[0] = 0;
						tcc_set[1] = 0;
					}
					uiu.dw = tcc_set[0];
					sendc(com_nmb,uiu.b[lb]);
					if (com_nmb_fl[com_nmb] > 8)
						sendc(com_nmb,uiu.b[mlb]);
					if (com_nmb_fl[com_nmb] > 16)
						sendc(com_nmb,uiu.b[mhb]);
					if (com_nmb_fl[com_nmb] > 24)
						sendc(com_nmb,uiu.b[hb]);
					uiu.dw = tcc_set[1];
					if (com_nmb_fl[com_nmb] > 32)
						sendc(com_nmb,uiu.b[lb]);
					if (com_nmb_fl[com_nmb] > 40)
						sendc(com_nmb,uiu.b[mlb]);
					if (com_nmb_fl[com_nmb] > 48)
						sendc(com_nmb,uiu.b[mhb]);
					if (com_nmb_fl[com_nmb] > 56)
						sendc(com_nmb,uiu.b[hb]);
		      	}
		      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      	{
			      	sendc(com_nmb,grp_Car_Stat[i].g_rdoorf);
		      	}
				if ((com_nmb == USR2COM) || (galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 10)))
				{
		      		for(i=1; i<=com_nmb_car[com_nmb]; i++)
		      		{
			      		sendc(com_nmb,ncu_door[i]);
		      		}
		      	}	

				if (user_com_ver[com_nmb] >= 1)
				{
			      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
			      	{
				      	sendc(com_nmb,grp_Car_Stat[i].g_ins_status);
			      	}
			      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
			      	{
				      	sendc(com_nmb,grp_Car_Stat[i].g_fire_status);
			      	}
			      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
			      	{
						uwu.w = grp_Car_Stat[i].g_cur_flt;
						sendc(com_nmb,uwu.b[lbw]);
						sendc(com_nmb,uwu.b[hbw]);
			      	}
			      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
			      	{
						uiu.dw = grp_Car_Stat[i].g_statusf1;
						sendc(com_nmb,uiu.b[lb]);
						sendc(com_nmb,uiu.b[mlb]);
						sendc(com_nmb,uiu.b[mhb]);
						sendc(com_nmb,uiu.b[hb]);
					}
			      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
			      	{
						uiu.dw = grp_Car_Stat[i].g_statusf2;
						sendc(com_nmb,uiu.b[lb]);
						sendc(com_nmb,uiu.b[mlb]);
						sendc(com_nmb,uiu.b[mhb]);
						sendc(com_nmb,uiu.b[hb]);
			      	}
			      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
			      	{
				      	sendc(com_nmb,grp_Car_Stat[i].g_pwr_status);
			      	}
			      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
			      	{
						uwu.w = grp_Car_Stat[i].g_ss_stat;
						sendc(com_nmb,uwu.b[lbw]);
						sendc(com_nmb,uwu.b[hbw]);
			      	}
				}

				if (user_com_ver[com_nmb] >= 2)
				{
					for(i=1; i<=com_nmb_car[com_nmb]; i++)
			      	{
						sendc(com_nmb,grp_Car_Stat[i].g_car_has_fault);
					}
					sendc(com_nmb,gservf);
					sendc(com_nmb,up_peak);
					sendc(com_nmb,dn_peak);
				}

		      	sendc(com_nmb,'I');
				Com_Mode[com_nmb] = 0;
		  	}
			break;

#if (Traction == 1)
		case 18:			// send speed clamp data
		  	if( (Tranbuf_Empty[com_nmb]) && (com_nmb == USR1COM))
		  	{												   // ONLY FROM COM1
				sendc(com_nmb,'C');
				uwu.w =0;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				if(rmbuf[com_nmb][3] == 1)	// set speed clamp #1 door zone clamp
				{
			    	sendc(com_nmb,140);
			     	sendc(com_nmb,0);
				}
				else	// set speed clamps #2 - #5
				{
			     	if(DT_Speed[(rmbuf[com_nmb][3] - 2)] > UT_Speed[(rmbuf[com_nmb][3] - 2)])
			     	{
						uwu.w = DT_Speed[(rmbuf[com_nmb][3] - 2)];
				 		sendc(com_nmb,uwu.b[lbw]);
				 		sendc(com_nmb,uwu.b[hbw]);
			     	}
			     	else
			     	{
						uwu.w = UT_Speed[(rmbuf[com_nmb][3] - 2)];
				 		sendc(com_nmb,uwu.b[lbw]);
				 		sendc(com_nmb,uwu.b[hbw]);
			     	}
				}
				sendc(com_nmb,'c');
		  	}
			break;

		case 20:			// Learn Hoistway
			if (com_nmb == USR1COM)
			{		// Only allow setup from com 1
				Learn_Hoistway();
				if(Tranbuf_Empty[com_nmb])
				{
					sendc(com_nmb,'L');
					uiu.dw = DPP_Count;
					sendc(com_nmb,uiu.b[lb]);
					sendc(com_nmb,uiu.b[mlb]);
					sendc(com_nmb,uiu.b[mhb]);
					sendc(com_nmb,uiu.b[hb]);
					for(i=1; i<=com_nmb_fl[com_nmb]; i++)
					{
						uiu.dw = DPP_Floor_Pos[i];
						sendc(com_nmb,uiu.b[lb]);
						sendc(com_nmb,uiu.b[mlb]);
						sendc(com_nmb,uiu.b[mhb]);
						sendc(com_nmb,uiu.b[hb]);
					}
					sendc(com_nmb,Cam_Mode);
					uwu.w = position;
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
					uwu.w = (int16)(((Enc_vel * 5.0)/(Pulses_per_Inch)) + 0.5);		// encoder velocity
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
					uwu.w = (int16)(Pulses_per_Inch);
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
					uiu.dw = DPP_DN;
					sendc(com_nmb,uiu.b[lb]);
					sendc(com_nmb,uiu.b[mlb]);
					sendc(com_nmb,uiu.b[mhb]);
					sendc(com_nmb,uiu.b[hb]);
	#if (Tract_HR == 1)
				  	for(i = 0; i <= 6; i++)
	#else
					for(i=0; i<=3; i++)
	#endif
					{
						uiu.dw = DPP_DT[i];
						sendc(com_nmb,uiu.b[lb]);
						sendc(com_nmb,uiu.b[mlb]);
						sendc(com_nmb,uiu.b[mhb]);
						sendc(com_nmb,uiu.b[hb]);
					}
					uiu.dw = DPP_UN;
					sendc(com_nmb,uiu.b[lb]);
					sendc(com_nmb,uiu.b[mlb]);
					sendc(com_nmb,uiu.b[mhb]);
					sendc(com_nmb,uiu.b[hb]);
	#if (Tract_HR == 1)
				  	for(i = 0; i <= 6; i++)
	#else
					for(i=0; i<=3; i++)
	#endif
					{
						uiu.dw = DPP_UT[i];
						sendc(com_nmb,uiu.b[lb]);
						sendc(com_nmb,uiu.b[mlb]);
						sendc(com_nmb,uiu.b[mhb]);
						sendc(com_nmb,uiu.b[hb]);
					}
					sendc(com_nmb,'W');
				}
				if(Cam_Mode == 16)
				{
					Com_Mode[com_nmb] = 0;
		  			Learn_Mode = 0;
		  		}
		  	}
		  	else
		  	{
		      	if((LCD_Mode != 8) || (Menu_level != 10))
		  			Learn_Mode = 0;
		  	}
			break;

		case 21:
		  	if (Tranbuf_Empty[com_nmb])
		  	{
				sendc(com_nmb,'L');
				uiu.dw = DPP_Count;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				for(i=1; i<=com_nmb_fl[com_nmb]; i++)
				{
					uiu.dw = DPP_Floor_Pos[i];
					sendc(com_nmb,uiu.b[lb]);
					sendc(com_nmb,uiu.b[mlb]);
					sendc(com_nmb,uiu.b[mhb]);
					sendc(com_nmb,uiu.b[hb]);
				}
				sendc(com_nmb,Cam_Mode);
				uwu.w = position;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uwu.w = (int16)(((Enc_vel * 5.0)/(Pulses_per_Inch)) + 0.5);		// encoder velocity
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uwu.w = (int16)(Pulses_per_Inch);
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uiu.dw = DPP_DN;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				uiu.dw = DPP_DT;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				uiu.dw = DPP_UN;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				uiu.dw = DPP_UT;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				sendc(com_nmb,'W');
		  	}
			break;
#endif
		case 19:
			Com_Mode[com_nmb] = 0;
			break;

		case 22:	// set down call
			set_dncall((uint16)(rmbuf[com_nmb][3] & 0xff));
	    	Com_Mode[com_nmb] = 0;
			break;

		case 23:	 // Set rear up hall call
			set_rupcall((uint16)(rmbuf[com_nmb][3] & 0xff));
	    	Com_Mode[com_nmb] = 0;
			break;

		case 24:	// Set rear down call
			set_rdncall((uint16)(rmbuf[com_nmb][3] & 0xff));
	    	Com_Mode[com_nmb] = 0;
			break;

		case 25:	 // set rear car call for this car
			set_rcarcall(cons[carnmb],(uint16)(rmbuf[com_nmb][3] & 0xff));
	    	Com_Mode[com_nmb] = 0;
			break;

		case 26:		// Send car status for cars 2 through 8
		case 27:
		case 28:
		case 52:
		case 53:
		case 67:
		case 68:
		  	if(Tranbuf_Empty[com_nmb])
		  	{	   
				if ((Com_Mode[com_nmb] == 26) || (Com_Mode[com_nmb] == 27) || (Com_Mode[com_nmb] == 28))
					car = Com_Mode[com_nmb] - 24; // cars 2 through 4
				else if ((Com_Mode[com_nmb] == 52) || (Com_Mode[com_nmb] == 53))
					car = Com_Mode[com_nmb] - 47; // cars 5 and 6	(Com_Mode[com_nmb] (- 52) + 5
				else if ((Com_Mode[com_nmb] == 67) || (Com_Mode[com_nmb] == 68))
					car = Com_Mode[com_nmb] - 60; // cars 7 and 8	(Com_Mode[com_nmb] (- 67) + 7
		      	sendc(com_nmb,'I');
				if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 3)))
					sendc(com_nmb,car);
		      	for(i=0; i<com_nmb_io[com_nmb]; i++)
				{
				  	sendc(com_nmb,iodata[car][i]);
				}
		      	sendc(com_nmb,grp_Car_Stat[car].g_procf);
		      	sendc(com_nmb,grp_Car_Stat[car].g_doorf);
		      	sendc(com_nmb,grp_Car_Stat[car].g_empf);
		      	sendc(com_nmb,grp_Car_Stat[car].g_firef);
		      	sendc(com_nmb,grp_Car_Stat[car].g_servf);
		      	sendc(com_nmb,grp_Car_Stat[car].g_rdoorf);

				sendc(com_nmb,grp_Car_Stat[car].g_pos);
				sendc(com_nmb,grp_Car_Stat[car].g_dirf);
				sendc(com_nmb,grp_Car_Stat[car].g_dpref);		
				sendc(com_nmb,grp_Car_Stat[car].g_cur_flt);
				uwu.w = (uint16)(grp_Car_Stat[car].g_statusf1 & 0x0000FFFF);		
				sendc(com_nmb,uwu.b[lbw]);		
				sendc(com_nmb,uwu.b[hbw]);		
				sendc(com_nmb,grp_Car_Stat[car].g_enc_dir);
				uwu.w = grp_Car_Stat[car].g_dmd_vel;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uwu.w = grp_Car_Stat[car].g_enc_vel;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uiu.dw = grp_Car_Stat[car].g_flr_cnt;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				uiu.dw = grp_Car_Stat[car].g_pls_cnt;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);

				if (galcom_ver >= 2)
				{
					uwu.w = (uint16)(grp_Car_Stat[car].g_statusf1 >> 16);
					sendc(com_nmb,uwu.b[lbw]);		
					sendc(com_nmb,uwu.b[hbw]);		
				}

				if (galcom_ver >= 6)
				{
					uwu.w = grp_Car_Stat[car].g_cur_flt;
					sendc(com_nmb,uwu.b[hbw]);		
				}

				if (user_com_ver[com_nmb] >= 1)
				{
					sendc(com_nmb,grp_Car_Stat[car].g_ins_status);	
					sendc(com_nmb,grp_Car_Stat[car].g_fire_status);	
					uwu.w = (uint16)(grp_Car_Stat[car].g_statusf2 & 0x0000FFFF);
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
					uwu.w = (uint16)(grp_Car_Stat[car].g_statusf2 >> 16);
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
			      	sendc(com_nmb,grp_Car_Stat[car].g_pwr_status);
					uwu.w = grp_Car_Stat[car].g_ss_stat;
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
				}
		      	sendc(com_nmb,'O');
		  	}
			break;

		case 29:		   // Set car cal for any car
			if ((rmbuf[com_nmb][4] >= 1) && (rmbuf[com_nmb][4] <= cons[nmbcars]))
				set_carcall(rmbuf[com_nmb][4],(uint16)(rmbuf[com_nmb][3] & 0xff));
	    	Com_Mode[com_nmb] = 0;
			break;

		case 30:			// Set rear car call for any call
			if ((rmbuf[com_nmb][4] >= 1) && (rmbuf[com_nmb][4] <= cons[nmbcars]))
				set_rcarcall(rmbuf[com_nmb][4],(uint16)(rmbuf[com_nmb][3] & 0xff));
	    	Com_Mode[com_nmb] = 0;
			break;
		
		case 31:			// Clear job statistics
			clear_job_statistics();
			Com_Mode[com_nmb] = 0;
			break;

		case 32:		// add security code
			if (LCD_Menu == 11)
				LCD_Init = 1;	// In security display: reset so index is not corrupted

			Com_Add_Sec_Fl_Req[com_nmb] = rmbuf[com_nmb][3];
			Com_Add_Sec_Code[com_nmb][0] = rmbuf[com_nmb][4];
			Com_Add_Sec_Code[com_nmb][1] = rmbuf[com_nmb][5];
			Com_Add_Sec_Code[com_nmb][2] = rmbuf[com_nmb][6];
			Com_Add_Sec_Code[com_nmb][3] = rmbuf[com_nmb][7];
			if ((Com_Add_Sec_Fl_Req[com_nmb] > 0) && (Com_Add_Sec_Fl_Req[com_nmb] <= cons[grtopf]))
			{
				Read_CodeTbl(Com_Add_Sec_Fl_Req[com_nmb],&Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
				Add_Security_Code(Com_Add_Sec_Fl_Req[com_nmb], &Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb], Com_Add_Sec_Code[com_nmb]);
			}
			else if (Com_Add_Sec_Fl_Req[com_nmb] == 0xFF)
			{
				Add_One_Code_All_Floors(1,com_nmb+2,Com_Add_Sec_Code[com_nmb]);
			}
	      	Com_Mode[com_nmb] = 0;
			break;
	
		case 33:	   // Delete security code
			if (LCD_Menu == 11)
				LCD_Init = 1;	// In security display: reset so index is not corrupted

			Com_Del_Sec_Fl_Req[com_nmb] = rmbuf[com_nmb][3];
			Com_Del_Sec_Code[com_nmb][0] = rmbuf[com_nmb][4];
			Com_Del_Sec_Code[com_nmb][1] = rmbuf[com_nmb][5];
			Com_Del_Sec_Code[com_nmb][2] = rmbuf[com_nmb][6];
			Com_Del_Sec_Code[com_nmb][3] = rmbuf[com_nmb][7];
			if ((Com_Del_Sec_Fl_Req[com_nmb] > 0) && (Com_Del_Sec_Fl_Req[com_nmb] <= cons[grtopf]))
			{
				Read_CodeTbl(Com_Add_Sec_Fl_Req[com_nmb],&Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
				Delete_Security_Code(Com_Del_Sec_Fl_Req[com_nmb], &Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb], Com_Del_Sec_Code[com_nmb]);
			}
			else if (Com_Del_Sec_Fl_Req[com_nmb] == 0xFF)
			{
				Delete_One_Code_All_Floors(1,com_nmb+2,Com_Del_Sec_Code[com_nmb]);
			}
	      	Com_Mode[com_nmb] = 0;
			break;

		case 34:		// Send a security code table

		  	if (Tranbuf_Empty[com_nmb])
		  	{
				Com_Sec_Fl_Tbl_Req[com_nmb] = rmbuf[com_nmb][3];		// receive floor that is requested
				if ((Com_Sec_Fl_Tbl_Req[com_nmb] > 0) && (Com_Sec_Fl_Tbl_Req[com_nmb] <= cons[grtopf]))
				{
					Read_CodeTbl(Com_Sec_Fl_Tbl_Req[com_nmb],&Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
			      	sendc(com_nmb,34);			// Packet number
					sendc(com_nmb,Com_Sec_Fl_Tbl_Req[com_nmb]);
					sendc(com_nmb,Com_CodeTable[com_nmb].NumCode);			// number of codes in table

					i = 0;
					while (i < Com_CodeTable[com_nmb].NumCode)
					{
						sendc(com_nmb,Com_CodeTable[com_nmb].Codes[i][0]);
						sendc(com_nmb,Com_CodeTable[com_nmb].Codes[i][1]);
						sendc(com_nmb,Com_CodeTable[com_nmb].Codes[i][2]);
						sendc(com_nmb,Com_CodeTable[com_nmb].Codes[i][3]);
						i++;
					}
			      	sendc(com_nmb,134);
				}
		      	Com_Mode[com_nmb] = 0;
		  	}
			break;

		case 35:			  // Receive security code table
			Com_Sec_Fl_Tbl_Req[com_nmb] = rmbuf[com_nmb][3];		// receive floor that is requested
			if ((Com_Sec_Fl_Tbl_Req[com_nmb] > 0) && (Com_Sec_Fl_Tbl_Req[com_nmb] <= cons[grtopf]))
			{
				if (rmbuf[com_nmb][4] < NumMaxCd)
				{
					Com_CodeTable[com_nmb].NumCode = rmbuf[com_nmb][4];

					j = 0;
					i = 0;
					while (i < Com_CodeTable[com_nmb].NumCode)
					{
						Com_CodeTable[com_nmb].Codes[i][0] = rmbuf[com_nmb][6+j];
						Com_CodeTable[com_nmb].Codes[i][1] = rmbuf[com_nmb][7+j];
						Com_CodeTable[com_nmb].Codes[i][2] = rmbuf[com_nmb][8+j];
						Com_CodeTable[com_nmb].Codes[i][3] = rmbuf[com_nmb][9+j];
						i++;
						j+=4;
					}
					if (LCD_Menu == 11)
						LCD_Init = 1;	// In security display: reset so index is not corrupted

					Write_CodeTbl(Com_Sec_Fl_Tbl_Req[com_nmb], &Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
				}
			}
	      	Com_Mode[com_nmb] = 0;
			break;

		case 36:			 // Send security checksum table
		  	if (Tranbuf_Empty[com_nmb])
		  	{
		      	sendc(com_nmb,36);					// packet number
				sendc(com_nmb,cons[grtopf]);

				for (i = 1;i <= cons[grtopf];i++)
				{
					uwu.w = Code_Tbl_Chk[i].NumCode;
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
					uwu.w = Code_Tbl_Chk[i].CheckSum;
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
				}
		      	sendc(com_nmb,136);
		      	Com_Mode[com_nmb] = 0;
		  	}
			break;

		case 37:			   // Receive security checksum table
			Com_Sec_Chksum_Fl[com_nmb] = rmbuf[com_nmb][3];		  // get the number of floors
			if (Com_Sec_Chksum_Fl[com_nmb] == cons[grtopf])
			{
				j = 0;
				for (i = 1; i <= cons[grtopf]; i++)
				{	
					uwu.b[lbw] = rmbuf[com_nmb][4+j];
					uwu.b[hbw] = rmbuf[com_nmb][5+j];
					Com_Code_Tbl_Chk[com_nmb][i].NumCode = uwu.w;
					uwu.b[lbw] = rmbuf[com_nmb][6+j];
					uwu.b[hbw] = rmbuf[com_nmb][7+j];
					Com_Code_Tbl_Chk[com_nmb][i].CheckSum = uwu.w;
					j += 4;
				}
			
				Com_Sec_Fl_Tbl_Req[com_nmb] = (uint8)Verify_Code_Check_Table(&Com_Code_Tbl_Chk[com_nmb][Com_Sec_Chksum_Fl[com_nmb]]);
				
				if (Com_Sec_Fl_Tbl_Req[com_nmb] != 0)
			      	Com_Mode[com_nmb] = 39;			// request packet to send codes for floor 
				else
		      		Com_Mode[com_nmb] = 0;
			}
			else
				Com_Mode[com_nmb] = 0;
			break;

		case 38:			 // Delete all security codes per floor or all floors
			Com_Del_All_Codes_Fl_Req[com_nmb] = rmbuf[com_nmb][3];
			if (Com_Del_All_Codes_Fl_Req[com_nmb] != 0)
			{
				if ((Com_Del_All_Codes_Fl_Req[com_nmb] >= cons[bottomf]) && (Com_Del_All_Codes_Fl_Req[com_nmb] <= cons[topf]))
				{
					if (LCD_Menu == 11)
						LCD_Init = 1;	// In security display: reset so index is not corrupted

					Clear_All_Codes_One_Floor(Com_Del_All_Codes_Fl_Req[com_nmb], &Com_CodeTable[com_nmb], Com_LookUpTable[com_nmb]);
				}
				else if (Com_Del_All_Codes_Fl_Req[com_nmb] == 0xFF)
				{
					if (LCD_Menu == 11)
						LCD_Init = 1;	// In security display: reset so index is not corrupted

					Clear_All_Codes_All_Floors(1,com_nmb + 2);
				}
			}
	      	Com_Mode[com_nmb] = 0;
			break;

		case 39:				 // send request for security code table
		  	if (Tranbuf_Empty[com_nmb])
		  	{
		     	sendc(com_nmb,39);
		      	sendc(com_nmb,35);
		      	sendc(com_nmb,Com_Sec_Fl_Tbl_Req[com_nmb]);
				sendc(com_nmb,139);
		  	}
			break;

		case 40:		  // Extended group initialization
			// set variable for new type comm since this packet was requested
			com_nmb_fl[com_nmb] = cons[grtopf];	
	#if (Tract_HR == 1)
//			com_nmb_io[com_nmb] = 145;	// 145 instead of 85 
			com_nmb_io[com_nmb] = 157;	// 145 instead of 85 
	#else
//			com_nmb_io[com_nmb] = 85;	// 85 instead of 84: index 39 gets sent twice 
			com_nmb_io[com_nmb] = 97;	// 85 instead of 84: index 39 gets sent twice 
	#endif
			com_nmb_car[com_nmb] = cons[nmbcars];
			sendc(com_nmb,'v');
			sendc(com_nmb,version);
			sendc(com_nmb,revision);
			sendc(com_nmb,com_nmb_fl[com_nmb]);   	// number of floors
			sendc(com_nmb,com_nmb_car[com_nmb]);	// number of cars
			sendc(com_nmb,com_nmb_io[com_nmb]);		// number of i/o array index
			sendc(com_nmb,com_version);				// previously comm update number
	#if (Tract_HR == 1)
			for (i=1;i<=8;i++)
	#else
			for (i=1;i<=6;i++)
	#endif
			{		   // set for six cars (future)
				sendc(com_nmb,top_fl[i]);
				sendc(com_nmb,top_rfl[i]);
				sendc(com_nmb,rear_op[i]);
				sendc(com_nmb,cons[BinPiIn]);	// will be an array for number of cars in the future
				sendc(com_nmb,Drive_Type[i]);	// will be an array in the future
			}

			if (user_com_ver[com_nmb] >= 3)
			{
			    sendc(com_nmb,interim_revision);
			}

			sendc(com_nmb,'r');
			Com_Mode[com_nmb] = 0;
			break;

		case 41:			   // Jump to power up routine - Can only upload from com1
/*			if ((Tranbuf_Empty[com_nmb]) && (com_nmb == USR1COM))
			{				
		    	if(chk_ins() == 1)
				{ 			// Car must be on inspection
					if (logon[com_nmb] == 3)
					{
						dog_ints_off();
						keybrd_restore();
						_disable();
						outp(0x21,(inp(0x21) & 0xFE));
						outp(0xA1,(inp(0xA1) & 0x8C));
						_enable();
		//				fcloseall();
						if((Drive_Type[cons[carnmb]] == 1)|| (Drive_Type[cons[carnmb]] == 9))
						{
		#if (TS5300 == 1)
				   			execl("PWRUP.EXE",".","0","T",(char*)NULL);
		#else
				   			execl("PWRUP.EXE",".","1","T",(char*)NULL);
		#endif
						}
						else
				   			execl("PWRUP.EXE",".","0","T",(char*)NULL);

						keybrd_init();
						dog_ints_on();
						initcom();
					}
					else
					{			
						sendc(com_nmb,'t');
						sendc(com_nmb,'L');
						sendc(com_nmb,'m');
					}
				}
				else
				{
					sendc(com_nmb,'t');
					sendc(com_nmb,'I');
					sendc(com_nmb,'m');
				}
				Com_Mode[com_nmb] = 0;
			}
*/
			break;


	// Com_Mode[com_nmb] 41 - 46 used for file transfer and power up 

		case 47:	   // Request for fault data
	    	if((rmbuf[com_nmb][3] >= 1) && (rmbuf[com_nmb][3] <= cons[nmbcars]))
	    	{
				grp_Car_Stat_req[com_nmb] = rmbuf[com_nmb][3];
				if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >=6)))
					reset_flt_ix = rmbuf[com_nmb][4];
				if (grp_Car_Stat_req[com_nmb] == cons[carnmb]){
					Com_Mode[com_nmb] = 9;
					if(reset_flt_ix == 1) 
				    { 				  				
				    	Loop2_ix = 0;
					}
				}
				else
				{						
				    timers[tfltdelay] = 0;
				    if(reset_flt_ix == 1) 
				    { 
					    put_pkt_req(8,grp_Car_Stat_req[com_nmb],gc_port);
	                    Loop1_ix = 0;
					}
		    		Com_Mode[com_nmb] = 48;
				}
	    	}
			else
				Com_Mode[com_nmb] = 0;
			break;

		case 48:		// Send fault data
			if (Tranbuf_Empty[com_nmb])
			{	 
				if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 6)))
				{
					if (reset_flt_ix == 1){
						diag_flt_ix = Car_Flt_ix + 1;
						if (diag_flt_ix >= max_flt_ix)
						    diag_flt_ix = 0;
					}
					send_flt_data = 0;
				  	if (reset_flt_ix == 1) 
					{
		                i = get_pkt_status(36, grp_Car_Stat_req[com_nmb], gc_port, 100);
						if (i == 1)
							send_flt_data = 1;
						if ((i == 0) && (gc_dev_online[grp_Car_Stat_req[com_nmb]] == 0)) // car is offline
							send_flt_data = 1;
					}
					else
						send_flt_data = 1;
					if (send_flt_data == 1)
					{			
				      	sendc(com_nmb,'F');
						sendc(com_nmb,grp_Car_Stat_req[com_nmb]);
						i = 0;
						if (galcom_ver >= 6)
							max_pkt_len = 150;
						else
							max_pkt_len = 135;

						if(reset_flt_ix != 1 && Loop1_ix == 0)		// added if-else statement
						{
							while (i < max_pkt_len)
							{
								sendc(com_nmb, 0);
								i++;
							}
						}
						else
						{
							while ((Loop1_ix < max_flt_ix) && (i < max_pkt_len) && (gc_dev_online[grp_Car_Stat_req[com_nmb]] != 0))
							{
								if (cf.Flts[diag_flt_ix].code != 0)
								{
									sendc(com_nmb,cf.Flts[diag_flt_ix].code);
									if (galcom_ver >= 6)
										sendc(com_nmb,(uint16)((cf.Flts[diag_flt_ix].code >> 8) & 0x00ff));	// high byte
									sendc(com_nmb,cf.Flts[diag_flt_ix].day);                         // day
									sendc(com_nmb,cf.Flts[diag_flt_ix].mon);                         // month
									sendc(com_nmb,(cf.Flts[diag_flt_ix].year >> 8) & 0x00FF);     // HIGH BYTE year 
									sendc(com_nmb,(cf.Flts[diag_flt_ix].year & 0x00FF));          // LOW BYTE year 
									sendc(com_nmb,cf.Flts[diag_flt_ix].min);                         // minute
									sendc(com_nmb,cf.Flts[diag_flt_ix].hour);                         // hour
									sendc(com_nmb,cf.Flts[diag_flt_ix].pos);                         // position
									sendc(com_nmb,(uint16)(cf.Flts[diag_flt_ix].count & 0x00ff));              // low byte nmb occur
									if (galcom_ver >= 6)
										i+=10;
									else
										i+=9;
								}
								diag_flt_ix++;
								if (diag_flt_ix >= max_flt_ix)
							    	diag_flt_ix = 0;
								Loop1_ix++;
							}
							if ((Loop1_ix >= max_flt_ix)||(gc_dev_online[grp_Car_Stat_req[com_nmb]] == 0))
							{
								Loop1_ix = 0;
								while (i < max_pkt_len)
								{				// Packet must be a fixed length so fill with zero's
									sendc(com_nmb,0);
									i++;
								}
							}
						}
				      	sendc(com_nmb,'T');
						Com_Mode[com_nmb] = 0;
						reset_flt_ix = 0;	
					}
				}
				else
				{	
					i = get_pkt_status(36, grp_Car_Stat_req[com_nmb], gc_port, 100);
					if(i == 1)
					{  // if packet has been returned with data then send data to hi
				      	sendc(com_nmb,'F');
						sendc(com_nmb,grp_Car_Stat_req[com_nmb]);
				      	for(i=1; i<=15; i++)
				      	{
							sendc(com_nmb,cf.Flts[diag_flt_ix].day);                         // day
							sendc(com_nmb,cf.Flts[diag_flt_ix].mon);                         // month
							sendc(com_nmb,(cf.Flts[diag_flt_ix].year >> 8) & 0x00FF);     // HIGH BYTE year 
							sendc(com_nmb,(cf.Flts[diag_flt_ix].year & 0x00FF));          // LOW BYTE year 
							sendc(com_nmb,cf.Flts[diag_flt_ix].min);                         // minute
							sendc(com_nmb,cf.Flts[diag_flt_ix].hour);                         // hour
							sendc(com_nmb,cf.Flts[diag_flt_ix].pos);                         // position
							sendc(com_nmb,0);              // low byte nmb occur
				      	}
				      	sendc(com_nmb,'T');
						Com_Mode[com_nmb] = 0;
					}
					else if (i == 2)		// packet timed out
						Com_Mode[com_nmb] = 0;
				}
			}
			break;

		case 49:		 // Get galcom version
			galcom_ver = rmbuf[com_nmb][3];
			galcom_rev = rmbuf[com_nmb][4];
			if (rmbuf[com_nmb][1] > 6)
			{
				user_com_ver[com_nmb] = rmbuf[com_nmb][5];
				user_com_type[com_nmb] = rmbuf[com_nmb][6];	 // 1=galcom, 2=galileo
			}
			else
			{
				user_com_ver[com_nmb] = 0;
				user_com_type[com_nmb] = 0;
			}
			Com_Mode[com_nmb] = 0;
			break;

		case 50:				   // Extended group initialization data
					 // Additional variables for future expansion
			sendc(com_nmb,'a');
			sendc(com_nmb,cons[sercom]);				// Serial Comm Devices
			if (cons[Second_Riser] != 0)
				sendc(com_nmb,cons[Second_Riser]);				// Inconspicious Riser
			else
				sendc(com_nmb,cons[incrs]);				// Inconspicious Riser

			sendc(com_nmb,cons[SecFlCfg]);				// Security Floor Mask Configuration
			sendc(com_nmb,cons[cblue]);				// Code Blue operation
			sendc(com_nmb,cons[epgrp]);				// Inter-group emergency power
			sendc(com_nmb,cons[equake]);				// Earthquake
			if (galcom_ver >= 2)
			{
				sendc(com_nmb,cons[att]);	 		// Attendant
				sendc(com_nmb,cons[loadw]);	 		// Load weighing
				uwu.w = cons[speed];
				sendc(com_nmb,uwu.b[lbw]);	 			// Car Speed low byte
				sendc(com_nmb,uwu.b[hbw]);	 			// Car Speed high byte
				sendc(com_nmb,nmb_hcio_brds);	 	// Maximum number of hall call boards  if zero use 20 for tract, 46 for tract_hr.
				sendc(com_nmb,cons[Dual_Ser_Riser]); // Dual Serial Riser

				for (i=1;i<=6;i++)
				{		   // set for six or eight cars (future)
					sendc(com_nmb,Security_Type[i]);
					sendc(com_nmb,cons[dispis]);	 // Only have data on this car right now! Discrete PI's 
					sendc(com_nmb,fvars[fvvidc+i]);	 // Video car position
					sendc(com_nmb,0);	 // Unused
				}
	#if (Tract_HR == 1)
				for (i=7;i<=8;i++)
				{		   // set for six or eight cars (future)
					sendc(com_nmb,Security_Type[i]);
					sendc(com_nmb,cons[dispis]);	 // Only have data on this car right now! Discrete PI's 
					sendc(com_nmb,fvars[fvvidc7+(i-7)]);	 // Video car position
					sendc(com_nmb,0);	 // Unused
				}
	#endif
			}
			else
			{
				for (i=1;i<=6;i++)
				{		   // set for six cars (future)
					sendc(com_nmb,Security_Type[i]);
					sendc(com_nmb,cons[dispis]);	 // Only have data on this car right now! Discrete PI's 
					sendc(com_nmb,0);	 // Unused
					sendc(com_nmb,0);	 // Unused
					sendc(com_nmb,0);	 // Unused
				}
			}
			sendc(com_nmb,'x');
			Com_Mode[com_nmb] = 0;
			break;

		case 51:			  // hall call i/o's
			sendc(com_nmb,'h');				// Start of packet

			j = c_com_no_dev-g_invalid_hc1_addr;

			// Device comm status
			for (i=0;i<j;i++)
				sendc(com_nmb,gc_dev_online[g_invalid_hc1_addr+i]);

			// Floors 1-13
			sendc(com_nmb,(uint16)((gc_dc[0] & 0x000001feL) >> 1));
			sendc(com_nmb,(uint16)(((gc_uc[0] & 0x0000000fL) << 4) | ((gc_dc[0] & 0x00001e00L) >> 9)));
			sendc(com_nmb,(uint16)((gc_uc[0] & 0x00000ff0L) >> 4));
			// Floors 14 - 25
			sendc(com_nmb,(uint16)((gc_dc[0] & 0x001fe000L) >> 13));
			sendc(com_nmb,(uint16)(((gc_uc[0] & 0x0000f000L) >> 8) | ((gc_dc[0] & 0x01e00000L) >> 21)));
			sendc(com_nmb,(uint16)((gc_uc[0] & 0x00ff0000L) >> 16));
	#if (Tract_HR == 1)
			// Floors 25 - 37
			sendc(com_nmb,(uint16)(((gc_dc[0] & 0xfe000000L) >> 25) | ((gc_dc[1] & 0x00000001L) << 7)));
			sendc(com_nmb,(uint16)(((gc_dc[1] & 0x0000001eL) >> 1) | ((gc_uc[0] & 0x0f000000L) >> 20)));
			sendc(com_nmb,(uint16)(((gc_uc[0] & 0xf0000000L) >> 28) |((gc_uc[1] & 0x0000000fL) << 4)));

			// Floors 37 - 49
			sendc(com_nmb,(uint16)((gc_dc[1] & 0x00001fe0L) >> 5));
			sendc(com_nmb,(uint16)(((gc_dc[1] & 0x0001e000L) >> 13) | (gc_uc[1] & 0x000000f0L)));
			sendc(com_nmb,(uint16)((gc_uc[1] & 0x0000ff00L) >> 8));

			// Floors 49 - 60
			sendc(com_nmb,(uint16)((gc_dc[1] & 0x01fe0000L) >> 17));
			sendc(com_nmb,(uint16)(((gc_dc[1] & 0x1e000000L) >> 25) | ((gc_uc[1] & 0x000f0000L) >> 12)));
			sendc(com_nmb,(uint16)((gc_uc[1] & 0x0ff00000L) >> 20));
	#else
			// Floors 26 - 31 front and rear
			sendc(com_nmb,(uint16)(((gc_dc[0] & 0x7e000000L) >> 25) | ((gc_uc[0] & 0x03000000L) >> 18)));
			sendc(com_nmb,(uint16)(((gc_uc[0] & 0x3c000000L) >> 26) | ((gc_rdc[0] & 0x1e000000L) >> 21)));
			sendc(com_nmb,(uint16)(((gc_rdc[0] & 0x60000000L) >> 29) | ((gc_ruc[0] & 0x3f000000L) >> 22)));
	#endif

			// First 13 rear floors
			sendc(com_nmb,(uint16)((gc_rdc[0] & 0x000001feL) >> 1));
			sendc(com_nmb,(uint16)(((gc_ruc[0] & 0x0000000fL) << 4) | ((gc_rdc[0] & 0x00001e00L) >> 9)));
			sendc(com_nmb,(uint16)((gc_ruc[0] & 0x00000ff0L) >> 4));
			// Rear Floors 14 - 25
			sendc(com_nmb,(uint16)((gc_rdc[0] & 0x001fe000L) >> 13));
			sendc(com_nmb,(uint16)(((gc_ruc[0] & 0x0000f000L) >> 8) | ((gc_rdc[0] & 0x01e00000L) >> 21)));
			sendc(com_nmb,(uint16)(((gc_ruc[0] & 0x00ff0000L) >> 16)));

	#if (Tract_HR == 1)
			// Floors 25 - 37
			sendc(com_nmb,(uint16)(((gc_rdc[0] & 0xfe000000L) >> 25) | ((gc_rdc[1] & 0x00000001L) << 7)));
			sendc(com_nmb,(uint16)(((gc_rdc[1] & 0x0000001eL) >> 1) | ((gc_ruc[0] & 0x0f000000L) >> 20)));
			sendc(com_nmb,(uint16)(((gc_ruc[0] & 0xf0000000L) >> 28) | ((gc_ruc[1] & 0x0000000fL) << 4)));
			// Floors 37 - 49
			sendc(com_nmb,(uint16)((gc_rdc[1] & 0x00001fe0L) >> 5));
			sendc(com_nmb,(uint16)(((gc_rdc[1] & 0x0001e000L) >> 13) | (gc_ruc[1] & 0x000000f0L)));
			sendc(com_nmb,(uint16)((gc_ruc[1] & 0x0000ff00L) >> 8));
			// Floors 49 - 60
			sendc(com_nmb,(uint16)((gc_rdc[1] & 0x01fe0000L) >> 17));
			sendc(com_nmb,(uint16)(((gc_rdc[1] & 0x1e000000L) >> 25) | ((gc_ruc[1] & 0x000f0000L) >> 12)));
			sendc(com_nmb,(uint16)((gc_ruc[1] & 0x0ff00000L) >> 20));
	#endif
			// ICR Floors 1-13
			sendc(com_nmb,(uint16)((gc_irdc[0] & 0x000001feL) >> 1));
			sendc(com_nmb,(uint16)(((gc_iruc[0] & 0x0000000fL) << 4) | ((gc_irdc[0] & 0x00001e00L) >> 9)));
			sendc(com_nmb,(uint16)((gc_iruc[0] & 0x00000ff0L) >> 4));
			// ICR Floors 14 - 25
			sendc(com_nmb,(uint16)((gc_irdc[0] & 0x001fe000L) >> 13));
			sendc(com_nmb,(uint16)(((gc_iruc[0] & 0x0000f000L) >> 8) | ((gc_irdc[0] & 0x01e00000L) >> 21)));
			sendc(com_nmb,(uint16)((gc_iruc[0] & 0x00ff0000L) >> 16));

	#if (Tract_HR == 1)
			// Floors 25 - 37
			sendc(com_nmb,(uint16)(((gc_irdc[0] & 0xfe000000L) >> 25) | ((gc_irdc[1] & 0x00000001L) << 7)));
			sendc(com_nmb,(uint16)(((gc_irdc[1] & 0x0000001eL) >> 1) | ((gc_iruc[0] & 0x0f000000L) >> 20)));
			sendc(com_nmb,(uint16)(((gc_iruc[0] & 0xf0000000L) >> 28) | ((gc_iruc[1] & 0x0000000fL) << 4)));
			// Floors 37 - 49
			sendc(com_nmb,(uint16)((gc_irdc[1] & 0x00001fe0L) >> 5));
			sendc(com_nmb,(uint16)(((gc_irdc[1] & 0x0001e000L) >> 13) | (gc_iruc[1] & 0x000000f0L)));
			sendc(com_nmb,(uint16)((gc_iruc[1] & 0x0000ff00L) >> 8));
			// Floors 49 - 60
			sendc(com_nmb,(uint16)((gc_irdc[1] & 0x01fe0000L) >> 17));
			sendc(com_nmb,(uint16)(((gc_irdc[1] & 0x1e000000L) >> 25) | ((gc_iruc[1] & 0x000f0000L) >> 12)));
			sendc(com_nmb,(uint16)((gc_iruc[1] & 0x0ff00000L) >> 20));
	#else
			// ICR Floors 26 - 31 front and rear
			sendc(com_nmb,(uint16)(((gc_irdc[0] & 0x7e000000L) >> 25) | ((gc_iruc[0] & 0x03000000L) >> 18)));
			sendc(com_nmb,(uint16)(((gc_iruc[0] & 0x3c000000L) >> 26) | ((gc_rirdc[0] & 0x1e000000L) >> 21)));
			sendc(com_nmb,(uint16)(((gc_rirdc[0] & 0x60000000L) >> 29) | ((gc_riruc[0] & 0x3f000000L) >> 22)));
	#endif
			// ICR First 13 rear floors
			sendc(com_nmb,(uint16)((gc_rirdc[0] & 0x000001feL) >> 1));
			sendc(com_nmb,(uint16)(((gc_riruc[0] & 0x0000000fL) << 4) | ((gc_rirdc[0] & 0x00001e00L) >> 9)));
			sendc(com_nmb,(uint16)((gc_riruc[0] & 0x00000ff0L) >> 4));
			// ICR Rear Floors 14 - 25
			sendc(com_nmb,(uint16)((gc_rirdc[0] & 0x001fe000L) >> 13));
			sendc(com_nmb,(uint16)(((gc_riruc[0] & 0x0000f000L) >> 8) | ((gc_rirdc[0] & 0x01e00000L) >> 21)));
			sendc(com_nmb,(uint16)((gc_riruc[0] & 0x00ff0000L) >> 16));

	#if(Tract_HR == 1)
			// Floors 25 - 37
			sendc(com_nmb,(uint16)(((gc_rirdc[0] & 0xfe000000L) >> 25) | ((gc_rirdc[1] & 0x00000001L) << 7)));
			sendc(com_nmb,(uint16)(((gc_rirdc[1] & 0x0000001eL) >> 1) | ((gc_riruc[0] & 0x0f000000L) >> 20)));
			sendc(com_nmb,(uint16)(((gc_riruc[0] & 0xf0000000L) >> 28) | ((gc_riruc[1] & 0x0000000fL) << 4)));
			// Floors 37 - 49
			sendc(com_nmb,(uint16)((gc_rirdc[1] & 0x00001fe0L) >> 5));
			sendc(com_nmb,(uint16)(((gc_rirdc[1] & 0x0001e000L) >> 13) | (gc_riruc[1] & 0x000000f0L)));
			sendc(com_nmb,(uint16)((gc_riruc[1] & 0x0000ff00L) >> 8));
			// Floors 49 - 60
			sendc(com_nmb,(uint16)((gc_rirdc[1] & 0x01fe0000L) >> 17));
			sendc(com_nmb,(uint16)(((gc_rirdc[1] & 0x1e000000L) >> 25) | ((gc_riruc[1] & 0x000f0000L) >> 12)));
			sendc(com_nmb,(uint16)((gc_riruc[1] & 0x0ff00000L) >> 20));
	#endif
			if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >= 10)))
			{
				// CB Floors 1-24
				sendc(com_nmb,(uint16)(gc_cb[0] & 0x000000ffL));
				sendc(com_nmb,(uint16)((gc_cb[0] & 0x0000ff00L) >> 8));
				sendc(com_nmb,(uint16)((gc_cb[0] & 0x00ff0000L) >> 16));

	#if (Tract_HR == 1)
				// floors 25-47 
				sendc(com_nmb,(uint16)((gc_cb[0] & 0xff000000L) >> 24));
				sendc(com_nmb,(uint16)(gc_cb[1] & 0x000000ffL));
				sendc(com_nmb,(uint16)((gc_cb[1] & 0x0000ff00L) >> 8));
				// floors 48 - 60 
				sendc(com_nmb,(uint16)((gc_cb[1] & 0x00ff0000L) >> 16));
				sendc(com_nmb,(uint16)(((gc_cb[1] & 0x0f000000L) >> 24) | ((gc_rcb[1] & 0x000f0000L) >> 12)));
				sendc(com_nmb,(uint16)((gc_rcb[1] & 0x0ff00000L) >> 20));
	#else
				// CB Floors 25-31, Rear CB Floors 25-31
				sendc(com_nmb,(uint16)((gc_cb[0] & 0x7f000000L) >> 24));
				sendc(com_nmb,(uint16)((gc_rcb[0] & 0x0f000000L) >> 20));
				sendc(com_nmb,(uint16)((gc_rcb[0] & 0x70000000L) >> 28));
	#endif
				// RCB Floors 1-24
				sendc(com_nmb,(uint16)(gc_rcb[0] & 0x000000ffL));
				sendc(com_nmb,(uint16)((gc_rcb[0] & 0x0000ff00L) >> 8));
				sendc(com_nmb,(uint16)((gc_rcb[0] & 0x00ff0000L) >> 16));

	#if(Tract_HR == 1)
				// floors 25-47 
				sendc(com_nmb,(uint16)((gc_rcb[0] & 0xff000000L) >> 24));
				sendc(com_nmb,(uint16)(gc_rcb[1] & 0x000000ffL));
				sendc(com_nmb,(uint16)((gc_rcb[1] & 0x0000ff00L) >> 8));
	#endif

				// SEC Floors 1-24
				sendc(com_nmb,(uint16)(gc_cc_sec[0] & 0x000000ffL));
				sendc(com_nmb,(uint16)((gc_cc_sec[0] & 0x0000ff00L) >> 8));
				sendc(com_nmb,(uint16)((gc_cc_sec[0] & 0x00ff0000L) >> 16));

	#if (Tract_HR == 1)
				// floors 25-47 
				sendc(com_nmb,(uint16)((gc_cc_sec[0] & 0xff000000L) >> 24));
				sendc(com_nmb,(uint16)(gc_cc_sec[1] & 0x000000ffL));
				sendc(com_nmb,(uint16)((gc_cc_sec[1] & 0x0000ff00L) >> 8));
				// floors 48 - 60 
				sendc(com_nmb,(uint16)((gc_cc_sec[1] & 0x00ff0000L) >> 16));
				sendc(com_nmb,(uint16)((gc_cc_sec[1] & 0x0f000000L) >> 24));
				sendc(com_nmb,0);
	#else
				// SEC Floors 25-31, RSEC Floors 25-31
				sendc(com_nmb,(uint16)((gc_cc_sec[0] & 0x7f000000L) >> 24));
				sendc(com_nmb,(uint16)((gc_rcc_sec[0] & 0x0f000000L) >> 20));
				sendc(com_nmb,(uint16)((gc_rcc_sec[0] & 0x70000000L) >> 28));
	#endif
				// RSEC Floors 1-24
				sendc(com_nmb,(uint16)(gc_rcc_sec[0] & 0x000000ffL));
				sendc(com_nmb,(uint16)((gc_rcc_sec[0] & 0x0000ff00L) >> 8));
				sendc(com_nmb,(uint16)((gc_rcc_sec[0] & 0x00ff0000L) >> 16));

	#if(Tract_HR == 1)
				// floors 25-47 
				sendc(com_nmb,(uint16)((gc_rcc_sec[0] & 0xff000000L) >> 24));
				sendc(com_nmb,(uint16)(gc_rcc_sec[1] & 0x000000ffL));
				sendc(com_nmb,(uint16)((gc_rcc_sec[1] & 0x0000ff00L) >> 8));

				// floors 48 - 60 
				sendc(com_nmb,(uint16)((gc_rcc_sec[1] & 0x00ff0000L) >> 16));
				sendc(com_nmb,(uint16)((gc_rcc_sec[1] & 0x0f000000L) >> 24));
				sendc(com_nmb,0);
	#endif
				// Card 17
				sendc(com_nmb,gpi[0]);
				sendc(com_nmb,gpi[1]);
				sendc(com_nmb,gpi[2]);
				// Card 18
				sendc(com_nmb,gpi[3]);
				sendc(com_nmb,gpi[4]);
				sendc(com_nmb,gpi[5]);
				// Card 19
				sendc(com_nmb,gpi[6]);
				sendc(com_nmb,gpi[7]);
				sendc(com_nmb,gpi[8]);
				// Card 20
				sendc(com_nmb,gpi[9]);
				sendc(com_nmb,gpi[10]);
				sendc(com_nmb,gpi[11]);
	#if(Tract_HR == 1)
				// Card 39
				sendc(com_nmb,gpi[12]);
				sendc(com_nmb,gpi[13]);
				sendc(com_nmb,gpi[14]);
				// Card 40
				sendc(com_nmb,gpi[15]);
				sendc(com_nmb,gpi[16]);
				sendc(com_nmb,gpi[17]);
				// Card 41
				sendc(com_nmb,gpi[18]);
				sendc(com_nmb,gpi[19]);
				sendc(com_nmb,gpi[20]);
				// Card 42
				sendc(com_nmb,gpi[21]);
				sendc(com_nmb,gpi[22]);
				sendc(com_nmb,gpi[23]);
				// Card 43
				sendc(com_nmb,gpi[24]);
				sendc(com_nmb,gpi[25]);
				sendc(com_nmb,gpi[26]);
	#endif
				if (cons[Dual_Ser_Riser] != 0)
				{
					// Floors 1-13
					sendc(com_nmb,(uint16)((gc_dc_dsr[0] & 0x000001feL) >> 1));
					sendc(com_nmb,(uint16)(((gc_uc_dsr[0] & 0x0000000fL) << 4) | ((gc_dc_dsr[0] & 0x00001e00L) >> 9)));
					sendc(com_nmb,(uint16)((gc_uc_dsr[0] & 0x00000ff0L) >> 4));
					// Floors 14 - 25
					sendc(com_nmb,(uint16)((gc_dc_dsr[0] & 0x001fe000L) >> 13));
					sendc(com_nmb,(uint16)(((gc_uc_dsr[0] & 0x0000f000L) >> 8) | ((gc_dc_dsr[0] & 0x01e00000L) >> 21)));
					sendc(com_nmb,(uint16)((gc_uc_dsr[0] & 0x00ff0000L) >> 16));
			#if (Tract_HR == 1)
					// Floors 25 - 37
					sendc(com_nmb,(uint16)(((gc_dc_dsr[0] & 0xfe000000L) >> 25) | ((gc_dc_dsr[1] & 0x00000001L) << 7)));
					sendc(com_nmb,(uint16)(((gc_dc_dsr[1] & 0x0000001eL) >> 1) | ((gc_uc_dsr[0] & 0x0f000000L) >> 20)));
					sendc(com_nmb,(uint16)(((gc_uc_dsr[0] & 0xf0000000L) >> 28) |((gc_uc_dsr[1] & 0x0000000fL) << 4)));

					// Floors 37 - 49
					sendc(com_nmb,(uint16)((gc_dc_dsr[1] & 0x00001fe0L) >> 5));
					sendc(com_nmb,(uint16)(((gc_dc_dsr[1] & 0x0001e000L) >> 13) | (gc_uc_dsr[1] & 0x000000f0L)));
					sendc(com_nmb,(uint16)((gc_uc_dsr[1] & 0x0000ff00L) >> 8));

					// Floors 49 - 60
					sendc(com_nmb,(uint16)((gc_dc_dsr[1] & 0x01fe0000L) >> 17));
					sendc(com_nmb,(uint16)(((gc_dc_dsr[1] & 0x1e000000L) >> 25) | ((gc_uc_dsr[1] & 0x000f0000L) >> 12)));
					sendc(com_nmb,(uint16)((gc_uc_dsr[1] & 0x0ff00000L) >> 20));
			#else
					// Floors 26 - 31 front and rear
					sendc(com_nmb,(uint16)(((gc_dc_dsr[0] & 0x7e000000L) >> 25) | ((gc_uc_dsr[0] & 0x03000000L) >> 18)));
					sendc(com_nmb,(uint16)(((gc_uc_dsr[0] & 0x3c000000L) >> 26) | ((gc_rdc_dsr[0] & 0x1e000000L) >> 21)));
					sendc(com_nmb,(uint16)(((gc_rdc_dsr[0] & 0x60000000L) >> 29) | ((gc_ruc_dsr[0] & 0x3f000000L) >> 22)));
			#endif

					// First 13 rear floors
					sendc(com_nmb,(uint16)((gc_rdc_dsr[0] & 0x000001feL) >> 1));
					sendc(com_nmb,(uint16)(((gc_ruc_dsr[0] & 0x0000000fL) << 4) | ((gc_rdc_dsr[0] & 0x00001e00L) >> 9)));
					sendc(com_nmb,(uint16)((gc_ruc_dsr[0] & 0x00000ff0L) >> 4));
					// Rear Floors 14 - 25
					sendc(com_nmb,(uint16)((gc_rdc_dsr[0] & 0x001fe000L) >> 13));
					sendc(com_nmb,(uint16)(((gc_ruc_dsr[0] & 0x0000f000L) >> 8) | ((gc_rdc_dsr[0] & 0x01e00000L) >> 21)));
					sendc(com_nmb,(uint16)((gc_ruc_dsr[0] & 0x00ff0000L) >> 16));

			#if (Tract_HR == 1)
					// Floors 25 - 37
					sendc(com_nmb,(uint16)(((gc_rdc_dsr[0] & 0xfe000000L) >> 25) | ((gc_rdc_dsr[1] & 0x00000001L) << 7)));
					sendc(com_nmb,(uint16)(((gc_rdc_dsr[1] & 0x0000001eL) >> 1) | ((gc_ruc_dsr[0] & 0x0f000000L) >> 20)));
					sendc(com_nmb,(uint16)(((gc_ruc_dsr[0] & 0xf0000000L) >> 28) | ((gc_ruc_dsr[1] & 0x0000000fL) << 4)));
					// Floors 37 - 49
					sendc(com_nmb,(uint16)((gc_rdc_dsr[1] & 0x00001fe0L) >> 5));
					sendc(com_nmb,(uint16)(((gc_rdc_dsr[1] & 0x0001e000L) >> 13) | (gc_ruc_dsr[1] & 0x000000f0L)));
					sendc(com_nmb,(uint16)((gc_ruc_dsr[1] & 0x0000ff00L) >> 8));
					// Floors 49 - 60
					sendc(com_nmb,(uint16)((gc_rdc_dsr[1] & 0x01fe0000L) >> 17));
					sendc(com_nmb,(uint16)(((gc_rdc_dsr[1] & 0x1e000000L) >> 25) | ((gc_ruc_dsr[1] & 0x000f0000L) >> 12)));
					sendc(com_nmb,(uint16)((gc_ruc_dsr[1] & 0x0ff00000L) >> 20));
			#endif
				}
			}

			sendc(com_nmb,'H');
			Com_Mode[com_nmb] = 0;
			break;


	// See Com_Mode[] == 26 for 52 and 53

		case 54:			 // Send i/o board setup for cars 1-3
		  	if (Tranbuf_Empty[com_nmb])
			{
		#if (Tract_HR == 1)
//				com_nmb_io_brd = 49;
				com_nmb_io_brd = 53;
		#else
				if (galcom_ver >= 6)
//					com_nmb_io_brd = 29;
					com_nmb_io_brd = 33;
				else
					com_nmb_io_brd = 28;
		#endif
		      	sendc(com_nmb,54);
				for(j=1; j<=3; j++)
				{
		      		for(i=0; i<com_nmb_io_brd; i++)
				  		sendc(com_nmb,(uint8)(IO_board[j][i] & 0x01));
				}
		      	sendc(com_nmb,154);
				Com_Mode[com_nmb] = 0;
			}
			break;

		case 55:			 // Send i/o board setup for cars 4-6
		  	if (Tranbuf_Empty[com_nmb])
			{
		#if (Tract_HR == 1)
//				com_nmb_io_brd = 49;
				com_nmb_io_brd = 53;
		#else
				if (galcom_ver >= 6)
//					com_nmb_io_brd = 29;
					com_nmb_io_brd = 33;
				else
					com_nmb_io_brd = 28;
		#endif
		      	sendc(com_nmb,55);
				for(j=4; j<=6; j++)
				{
		      		for(i=0; i<com_nmb_io_brd; i++)
				  		sendc(com_nmb,(uint8)(IO_board[j][i] & 0x01));
				}
		      	sendc(com_nmb,155);
				Com_Mode[com_nmb] = 0;
			}
			break;

		case 56:		// Send which i/o are inputs 0's and outputs 1's
		  	if (Tranbuf_Empty[com_nmb])
			{
		      	sendc(com_nmb,56);
				for(i=0;i<com_nmb_io[com_nmb];i++)
					sendc(com_nmb,IO_In_Out[i]);
		      	sendc(com_nmb,156);
				Com_Mode[com_nmb] = 0;
			}
			break;

		case 57:			// Send valid i/o's 1= valid
		  	if (Tranbuf_Empty[com_nmb])
			{
		      	sendc(com_nmb,57);
		#if (Tract_HR == 1)
				for(i=0;i<com_nmb_io[com_nmb];i++)
		#else
				if (galcom_ver >= 6)
					j = com_nmb_io[com_nmb];
				else
					j=82;
				for(i=0;i<j;i++)
		#endif
					sendc(com_nmb,io_valid[i]);
		      	sendc(com_nmb,157);
				Com_Mode[com_nmb] = 0;
			}
			break;

		case 58:			// Send group valid i/o's 1 = valid
		  	if (Tranbuf_Empty[com_nmb])
			{
		      	sendc(com_nmb,58);
				for(i=0;i<(nmb_hcio_brds * 3);i++)				// hr car index changed from 136 to 135
					sendc(com_nmb,gio_valid[i]);
				for(i=0;i<nmb_hcio_brds;i++)
					sendc(com_nmb,HC_IO_board[i] & 0x01);
		      	sendc(com_nmb,158);

				Com_Mode[com_nmb] = 0;
			}
			break;

		case 59:		// Send car i/o names
		  	if (Tranbuf_Empty[com_nmb])
			{
		#if (Tract_HR == 1)
//				com_nmb_io_brd = 49;
				com_nmb_io_brd = 53;
		#else
				if (galcom_ver >= 6)
//					com_nmb_io_brd = 29;
					com_nmb_io_brd = 33;
				
					com_nmb_io_brd = 28;
		#endif
		      	sendc(com_nmb,59);
				if (rmbuf[com_nmb][3] < com_nmb_io_brd)
				{
					brdnmb = rmbuf[com_nmb][3];	 // brdnmb location: 0 to num boards - 1
			      	sendc(com_nmb,brdnmb);
					ionmb = brdnmb * 24;
					for(j=ionmb; j<(ionmb+24); j++)
					{
				    	for(i=0; i<5; i++)
						sendc(com_nmb,ioname[j][i]);
					}
				}
		      	sendc(com_nmb,159);

				Com_Mode[com_nmb] = 0;
			}
			break;

		case 60:			// Send group i/o names
		  	if(Tranbuf_Empty[com_nmb])
			{
		      	sendc(com_nmb,60);
				if (rmbuf[com_nmb][3] < nmb_hcio_brds)
				{
					brdnmb = rmbuf[com_nmb][3];		  // brdnmb location: 0 to num boards - 1
			      	sendc(com_nmb,brdnmb);
					ionmb = brdnmb * 24;
					for(j=ionmb; j<(ionmb+24); j++)
					{
						for(i=0; i<5; i++)
		  					sendc(com_nmb,hcioname[j][i]);
		 			}
		 		}
		       	sendc(com_nmb,160);

				Com_Mode[com_nmb] = 0;
			}
			break;

		case 61:		  // send variables from 120 to 180
		  	if (Tranbuf_Empty[com_nmb])
		  	{			  
		      	sendc(com_nmb,0x61);
				if (galcom_ver >= 6)
					j = 239;
				else
					j = 179;

				qq = 0;
			    for(i=120; i<=j; i++)
			    {									   // calculate checksum
					qq += fvars[i];
			    }
				sendc(com_nmb,qq & 0x0ff);					// send checksum
				sendc(com_nmb,((qq >> 8) & 0x00ff));

			    for(i=120; i<=j; i++)
			    {
					sendc(com_nmb,fvars[i] & 0x00ff);
					sendc(com_nmb,((fvars[i] >> 8) & 0x00ff));
			    }
			    sendc(com_nmb,'V');
				Com_Mode[com_nmb] = 0;
		  	}
			break;

		case 62:			 // receive variable from 120 to 180
		  	bbb = (uint16)(rmbuf[com_nmb][3] & 0x00ff);		// read in checksum
		  	bbb |= ((rmbuf[com_nmb][4] << 8) & 0xff00);
			ck_sum = 0;
			if (galcom_ver >= 6)
				j = 240;
			else
				j = 120;
		  	for(i=0; i<j; i=(i+2))
		  	{				// calculate checksum
			  	qq = (uint16)(rmbuf[com_nmb][i+5]&0x00ff);
			  	qq |= ((rmbuf[com_nmb][i+6] << 8)&0xff00);
				ck_sum += qq;
		  	}
			if (ck_sum == ((int16)bbb))
			{		// if checksum matches then update variables
				Var_Nmb = 120;
		      	for(i=0; i<j; i=(i+2))
		      	{	   
					if(Var_Nmb != fvbaud)
					{	// do not allow the laptop update the serial comm baud rate
					  	qq = (uint16)(rmbuf[com_nmb][i+5]&0x00ff);
					  	qq |= ((rmbuf[com_nmb][i+6] << 8)&0xff00);
						if ((qq >= fvars_min[Var_Nmb]) && (qq <= fvars_max[Var_Nmb]))
					  		fvars[Var_Nmb] = qq;
		      		}
					Var_Nmb++;
		      	}
		      	wrfvar();
			}
		   	Com_Mode[com_nmb] = 61;
			break;

#if (Traction == 1)
		case 63:		 // send digital speed clamp setting and terminal limit speeds
		  	if (Tranbuf_Empty[com_nmb])
		  	{			
		      	sendc(com_nmb,'T');
				if (galcom_ver >= 6)
				{
					j = 8;
					m = 7;
				}
				else
				{
					j = 5;
					m = 4;
				}
				qq = 0;
			    for(i=0; i<=j; i++)
			    {									   // calculate checksum
					qq += speed_clamp[i];
			    }
			    for(i=0; i < m; i++)
			    {									   // calculate checksum
					qq += UT_Speed[i];
					qq += DT_Speed[i];
			    }
				qq += DTS_Speed;
				qq += UTS_Speed;

				sendc(com_nmb,qq & 0x0ff);					// send checksum
				sendc(com_nmb,((qq >> 8) & 0x00ff));

			    for(i=0; i<=j; i++)
			    {
					uwu.w = speed_clamp[i];
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
				}
			    for(i=0; i < m; i++)
			    {
					uwu.w = DT_Speed[i];
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
				}
				uwu.w = DTS_Speed;
			 	sendc(com_nmb,uwu.b[lbw]);
			 	sendc(com_nmb,uwu.b[hbw]);

			    for(i=0; i < m; i++)
			    {
					uwu.w = UT_Speed[i];
					sendc(com_nmb,uwu.b[lbw]);
					sendc(com_nmb,uwu.b[hbw]);
			    }
				uwu.w = UTS_Speed;
			 	sendc(com_nmb,uwu.b[lbw]);
			 	sendc(com_nmb,uwu.b[hbw]);

			    sendc(com_nmb,'V');

				Com_Mode[com_nmb] = 0;
		  	}
			break;

		// Do not use com_mode[] = 64 because 64 is the @ (0x40) symbol

  		case 65:		  // receive digital speed clamp settings
	  	  	bbb = (uint16)(rmbuf[com_nmb][3]&0xff);		// read in checksum
		  	bbb |= ((rmbuf[com_nmb][4] << 8)&0xff00);
			ck_sum = 0;
			if (galcom_ver >= 6)
				j = 18;
			else
				j = 12;
		   	for(i=0; i<j; i=(i+2))
		   	{				// calculate checksum
			  	qq = (uint16)(rmbuf[com_nmb][i+5]&0xff);
			  	qq |= ((rmbuf[com_nmb][i+6] << 8)&0xff00);
				ck_sum += qq;
		   	}
			if (ck_sum == ((int16)bbb))
			{		// if checksum matches then update variables
				Var_Nmb = 0;
		      	for(i=0; i<j; i=(i+2))
		      	{	   
				  	qq = (uint16)(rmbuf[com_nmb][i+5]&0xff);
				  	qq |= ((rmbuf[com_nmb][i+6] << 8)&0xff00);
				  	speed_clamp[Var_Nmb] = qq;
					Var_Nmb++;
		      	}
		      	wrfvar();
			}
		   	Com_Mode[com_nmb] = 63;
			break;
#endif

		case 66:		// Send i/o board setup for cars 7-8
		  	if (Tranbuf_Empty[com_nmb])
			{
		#if (Tract_HR == 1)
//				com_nmb_io_brd = 49;
				com_nmb_io_brd = 53;
		#else
				if (galcom_ver >= 6)
//					com_nmb_io_brd = 29;
					com_nmb_io_brd = 33;
				else
					com_nmb_io_brd = 28;
		#endif
		      	sendc(com_nmb,66);
				for(j=7; j<=8; j++)
				{
		      		for(i=0; i<com_nmb_io_brd; i++)
				  		sendc(com_nmb,(uint8)(IO_board[j][i] & 0x01));
				}
		      	sendc(com_nmb,166);
				Com_Mode[com_nmb] = 0;
			}
			break;


	// See Com_Mode[] == 26 for 67 and 68

		case 69:		 // send variables from 180 to 360	 (180 variables)
		  	if (Tranbuf_Empty[com_nmb])
		  	{			  
				if (galcom_ver >= 6)
				{
			      	sendc(com_nmb,0x69);

					qq = 0;
				    for(i=240; i<360; i++)
				    {									   // calculate checksum
						qq += fvars[i];
				    }
					sendc(com_nmb,qq & 0x0ff);					// send checksum
					sendc(com_nmb,((qq >> 8) & 0x00ff));

				    for(i=240; i<360; i++)
				    {
						sendc(com_nmb,fvars[i] & 0x00ff);
						sendc(com_nmb,((fvars[i] >> 8) & 0x00ff));
				    }
				    sendc(com_nmb,'V');
				}
				Com_Mode[com_nmb] = 0;
		  	}
			break;

		case 70:			 // receive variable from 180 to 360  (180 variables)
			bbb = (uint16)(rmbuf[com_nmb][3] & 0x00ff);		// read in checksum
			bbb |= ((rmbuf[com_nmb][4] << 8) & 0xff00);
			ck_sum = 0;
			for(i=0; i<240; i=(i+2))
			{				// calculate checksum
			  	qq = (uint16)(rmbuf[com_nmb][i+5]&0x00ff);
			  	qq |= ((rmbuf[com_nmb][i+6] << 8)&0xff00);
				ck_sum += qq;
			}
			if (ck_sum == ((int16)bbb))
			{		// if checksum matches then update variables
				Var_Nmb = 240;
			  	for(i=0; i<240; i=(i+2))
			  	{	   
					if(Var_Nmb != fvbaud)
					{	// do not allow the laptop update the serial comm baud rate
					  	qq = (uint16)(rmbuf[com_nmb][i+5]&0x00ff);
					  	qq |= ((rmbuf[com_nmb][i+6] << 8)&0xff00);
						if ((qq >= fvars_min[Var_Nmb]) && (qq <= fvars_max[Var_Nmb]))
					  		fvars[Var_Nmb] = qq;
			  		}
					Var_Nmb++;
			  	}
			  	wrfvar();
			}
		   	Com_Mode[com_nmb] = 69;
			break;

		case 71:					// Receive Group Service Timers
			i = rmbuf[com_nmb][3];
			if ((i >= 0) && (i <= 9))
			{
				svc_t[i].service = rmbuf[com_nmb][4];
				svc_t[i].status = rmbuf[com_nmb][5];
				j = 0;
				for (k=0;k<=2;k++)
				{
					svc_t[i].month[k] = rmbuf[com_nmb][j+6];
					svc_t[i].day[k] = rmbuf[com_nmb][j+7];
				  	svc_t[i].md_on_hour[k] = rmbuf[com_nmb][j+8];
				  	svc_t[i].md_off_hour[k] = rmbuf[com_nmb][j+9];
				  	svc_t[i].md_on_minute[k] = rmbuf[com_nmb][j+10];
				  	svc_t[i].md_off_minute[k] = rmbuf[com_nmb][j+11];
					j+=6;
				}
				for (k=0;k<=6;k++)
				{			   // j starts at 18 so j+6 = 24
				  	svc_t[i].on_hour[k] = rmbuf[com_nmb][j+6];
				  	svc_t[i].off_hour[k] = rmbuf[com_nmb][j+7];
				  	svc_t[i].on_minute[k] = rmbuf[com_nmb][j+8];
				  	svc_t[i].off_minute[k] = rmbuf[com_nmb][j+9];
					j+=4;
				}
				Wrt_Grp_Svc_Timer(i);
			}
			Com_Mode[com_nmb] = 0;
			break;

		case 72: 			// Send Group Service Timers
			j = rmbuf[com_nmb][3];
			if ((j >= 0) && (j <= 9))
			{
		      	sendc(com_nmb,0x72);
			  	sendc(com_nmb,svc_t[j].service);
			  	sendc(com_nmb,svc_t[j].status);
				for (i=0;i<=2;i++)
				{
				  	sendc(com_nmb,svc_t[j].month[i]);
				  	sendc(com_nmb,svc_t[j].day[i]);
				  	sendc(com_nmb,svc_t[j].md_on_hour[i]);
				  	sendc(com_nmb,svc_t[j].md_off_hour[i]);
				  	sendc(com_nmb,svc_t[j].md_on_minute[i]);
				  	sendc(com_nmb,svc_t[j].md_off_minute[i]);
				}
				for (i=0;i<=6;i++)
				{
				  	sendc(com_nmb,svc_t[j].on_hour[i]);
				  	sendc(com_nmb,svc_t[j].off_hour[i]);
				  	sendc(com_nmb,svc_t[j].on_minute[i]);
				  	sendc(com_nmb,svc_t[j].off_minute[i]);
				}
		      	sendc(com_nmb,172);
			}
			Com_Mode[com_nmb] = 0;
			break;

		case 73:						// Receive User Interface front hall call Security floor lockouts
			// Up and Down Hall call security bits
			uiu.b[lb] = rmbuf[com_nmb][3];
			uiu.b[mlb] = rmbuf[com_nmb][4];
			uiu.b[mhb] = rmbuf[com_nmb][5];
			uiu.b[hb] = rmbuf[com_nmb][6];
			ui_uc_sec[0] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][7];
			uiu.b[mlb] = rmbuf[com_nmb][8];
			uiu.b[mhb] = rmbuf[com_nmb][9];
			uiu.b[hb] = rmbuf[com_nmb][10];
			ui_uc_sec[1] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][11];
			uiu.b[mlb] = rmbuf[com_nmb][12];
			uiu.b[mhb] = rmbuf[com_nmb][13];
			uiu.b[hb] = rmbuf[com_nmb][14];
			ui_dc_sec[0] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][15];
			uiu.b[mlb] = rmbuf[com_nmb][16];
			uiu.b[mhb] = rmbuf[com_nmb][17];
			uiu.b[hb] = rmbuf[com_nmb][18];
			ui_dc_sec[1] = uiu.dw;
			Com_Mode[com_nmb] = 0;
			break;

		case 74:		// Send User Interface front hall call security floors
	      	sendc(com_nmb,0x74);
			uiu.dw = ui_uc_sec[0];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			uiu.dw = ui_uc_sec[1];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			uiu.dw = ui_dc_sec[0];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			uiu.dw = ui_dc_sec[1];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
	      	sendc(com_nmb,174);
			Com_Mode[com_nmb] = 0;
			break;

		case 75:						// Receive User Interface rear hall call Security floor lockouts
			// Rear Up and Down Security bits
			uiu.b[lb] = rmbuf[com_nmb][3];
			uiu.b[mlb] = rmbuf[com_nmb][4];
			uiu.b[mhb] = rmbuf[com_nmb][5];
			uiu.b[hb] = rmbuf[com_nmb][6];
			ui_ruc_sec[0] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][7];
			uiu.b[mlb] = rmbuf[com_nmb][8];
			uiu.b[mhb] = rmbuf[com_nmb][9];
			uiu.b[hb] = rmbuf[com_nmb][10];
			ui_ruc_sec[1] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][11];
			uiu.b[mlb] = rmbuf[com_nmb][12];
			uiu.b[mhb] = rmbuf[com_nmb][13];
			uiu.b[hb] = rmbuf[com_nmb][14];
			ui_rdc_sec[0] = uiu.dw;
			uiu.b[lb] = rmbuf[com_nmb][15];
			uiu.b[mlb] = rmbuf[com_nmb][16];
			uiu.b[mhb] = rmbuf[com_nmb][17];
			uiu.b[hb] = rmbuf[com_nmb][18];
			ui_rdc_sec[1] = uiu.dw;
			Com_Mode[com_nmb] = 0;
			break;

		case 76:			// Send User Interface rear hall call security floors
	      	sendc(com_nmb,0x76);
			uiu.dw = ui_ruc_sec[0];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			uiu.dw = ui_ruc_sec[1];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			uiu.dw = ui_rdc_sec[0];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			uiu.dw = ui_rdc_sec[1];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
	      	sendc(com_nmb,176);
			Com_Mode[com_nmb] = 0;
			break;

		case 77:							// Receive User Interface Front car call Security floor lockouts
			j = 3;
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				uiu.b[lb] = rmbuf[com_nmb][j+0];
				uiu.b[mlb] = rmbuf[com_nmb][j+1];
				uiu.b[mhb] = rmbuf[com_nmb][j+2];
				uiu.b[hb] = rmbuf[com_nmb][j+3];
				ui_cc_sec[0][i]	= uiu.dw;
				j+=4;
				uiu.b[lb] = rmbuf[com_nmb][j+0];
				uiu.b[mlb] = rmbuf[com_nmb][j+1];
				uiu.b[mhb] = rmbuf[com_nmb][j+2];
				uiu.b[hb] = rmbuf[com_nmb][j+3];
				ui_cc_sec[1][i]	= uiu.dw;
				j+=4;
				
				if (i != cons[carnmb])
				{
					if((ui_cc_sec[0][i] != ui_cc_sec_prev[0][i]) ||(ui_cc_sec[1][i] != ui_cc_sec_prev[1][i]))
					{
						if (ui_cc_sec[0][i] != ui_cc_sec_prev[0][i])
							ui_cc_sec_prev[0][i] = ui_cc_sec[0][i];
						else if (ui_cc_sec[1][i] != ui_cc_sec_prev[1][i])
							ui_cc_sec_prev[1][i] = ui_cc_sec[1][i];
						put_pkt_req(37,i,gc_port);
					}									
				}
				else
				{
					ui_cc_sec_prev[0][i] = ui_cc_sec[0][i];
					ui_cc_sec_prev[1][i] = ui_cc_sec[1][i];
				}
				
			}
			Com_Mode[com_nmb] = 0;
			break;

		case 78:			// Send User Interface front car call security floors
	      	sendc(com_nmb,0x78);
			j = 3;
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				uiu.dw = ui_cc_sec[0][i];
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				uiu.dw = ui_cc_sec[1][i];
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
			}
	      	sendc(com_nmb,178);
			Com_Mode[com_nmb] = 0;
			break;

		case 79: 							// Receive User Interface Rear car call Security floor lockouts
			j = 3;
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				uiu.b[lb] = rmbuf[com_nmb][j+0];
				uiu.b[mlb] = rmbuf[com_nmb][j+1];
				uiu.b[mhb] = rmbuf[com_nmb][j+2];
				uiu.b[hb] = rmbuf[com_nmb][j+3];
				ui_rcc_sec[0][i]	= uiu.dw;
				j+=4;
				uiu.b[lb] = rmbuf[com_nmb][j+0];
				uiu.b[mlb] = rmbuf[com_nmb][j+1];
				uiu.b[mhb] = rmbuf[com_nmb][j+2];
				uiu.b[hb] = rmbuf[com_nmb][j+3];
				ui_rcc_sec[1][i]	= uiu.dw;
				j+=4;
			}
			Com_Mode[com_nmb] = 0;
			break;
			
		case 80:			// Send User Interface rear car call security floors
	      	sendc(com_nmb,0x80);
			j = 3;
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				uiu.dw = ui_rcc_sec[0][i];
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
				uiu.dw = ui_rcc_sec[1][i];
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
			}
	      	sendc(com_nmb,180);
			Com_Mode[com_nmb] = 0;
			break;
			
		case 81:			// Send Line voltage for user interface
	      	sendc(com_nmb,0x81);
			j = 3;
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				uwu.w = grp_Car_Stat[i].g_L1_Voltage;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uwu.w = grp_Car_Stat[i].g_L2_Voltage;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uwu.w = grp_Car_Stat[i].g_L3_Voltage;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				uwu.w = grp_Car_Stat[i].g_Door_Voltage;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
				sendc(com_nmb,grp_Car_Stat[i].g_Voltage_Dev_Online);		// added additional byte
			}
	      	sendc(com_nmb,181);
			Com_Mode[com_nmb] = 0;
			break;

		case 82:			// Car data for fault and service change detection
	      	sendc(com_nmb,0x82);

	      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
	      	{
				sendc(com_nmb,grp_Car_Stat[i].g_pos);
	      	}
	      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
	      	{
				sendc(com_nmb,grp_Car_Stat[i].g_servf);
	      	}
	      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
	      	{
				sendc(com_nmb,grp_Car_Stat[i].g_procf);
	      	}
	      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
	      	{
				sendc(com_nmb,grp_Car_Stat[i].g_firef);
	      	}
	      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
	      	{
		      	sendc(com_nmb,grp_Car_Stat[i].g_ins_status);
	      	}
	      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
	      	{
		      	sendc(com_nmb,grp_Car_Stat[i].g_fire_status);
	      	}
	      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
	      	{
				uwu.w = grp_Car_Stat[i].g_cur_flt;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
	      	}
			for(i=1; i<=com_nmb_car[com_nmb]; i++)
			{
				uiu.dw = grp_Car_Stat[i].g_statusf1;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
			}
			for(i=1; i<=com_nmb_car[com_nmb]; i++)
			{
				uiu.dw = grp_Car_Stat[i].g_statusf2;
				sendc(com_nmb,uiu.b[lb]);
				sendc(com_nmb,uiu.b[mlb]);
				sendc(com_nmb,uiu.b[mhb]);
				sendc(com_nmb,uiu.b[hb]);
			}
			for(i=1; i<=com_nmb_car[com_nmb]; i++)
			{
			  	sendc(com_nmb,grp_Car_Stat[i].g_pwr_status);
			}
	      	for(i=1; i<=com_nmb_car[com_nmb]; i++)
	      	{
				uwu.w = grp_Car_Stat[i].g_ss_stat;
				sendc(com_nmb,uwu.b[lbw]);
				sendc(com_nmb,uwu.b[hbw]);
	      	}
	      	sendc(com_nmb,182);
			Com_Mode[com_nmb] = 0;
			break;

		case 83: 							// Receive User Interface Control Variables
			j = 3;
			ui_uppeak = rmbuf[com_nmb][j+0];
			ui_dnpeak = rmbuf[com_nmb][j+1];
			ui_alt_lby = rmbuf[com_nmb][j+2];
			ui_alt_park = rmbuf[com_nmb][j+3];
			ui_ncu = rmbuf[com_nmb][j+4];
			ui_alt_fl_sec = rmbuf[com_nmb][j+5];
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				ui_ind[i] = rmbuf[com_nmb][j+6];
				ui_rtl[i] = rmbuf[com_nmb][j+7];
				ui_heof[i] = rmbuf[com_nmb][j+8];
				ui_ad_off[i] = rmbuf[com_nmb][j+9];
				j+=4;
			}
			Com_Mode[com_nmb] = 0;
			break;
			
		case 84:			// Send User Interface Control Variables
	      	sendc(com_nmb,0x80);
			sendc(com_nmb,ui_uppeak);
			sendc(com_nmb,ui_dnpeak);
			sendc(com_nmb,ui_alt_lby);
			sendc(com_nmb,ui_alt_park);
			sendc(com_nmb,ui_ncu);
			sendc(com_nmb,ui_alt_fl_sec);
			for (i=1;i<=com_nmb_car[com_nmb];i++)
			{
				sendc(com_nmb,ui_ind[i]);
				sendc(com_nmb,ui_rtl[i]);
				sendc(com_nmb,ui_heof[i]);
				sendc(com_nmb,ui_ad_off[i]);
			}
	      	sendc(com_nmb,184);
			Com_Mode[com_nmb] = 0;
			break;
			
		case 85:	   // Request for detailed fault data
	    	if((rmbuf[com_nmb][3] >= 1) && (rmbuf[com_nmb][3] <= cons[nmbcars]))
	    	{
				grp_Car_Stat_req[com_nmb] = rmbuf[com_nmb][3];
				if ((galcom_ver > 1) || ((galcom_ver == 1) && (galcom_rev >=6)))
					reset_flt_ix = rmbuf[com_nmb][4];
				if (grp_Car_Stat_req[com_nmb] == cons[carnmb]){
					Com_Mode[com_nmb] = 86;
					if(reset_flt_ix == 1) 
				    { 				  				
				    	Loop2_ix = 0;
					}
				}
				else
				{						
				    timers[tfltdelay] = 0;
				    if(reset_flt_ix == 1) 
				    { 
					    put_pkt_req(8,grp_Car_Stat_req[com_nmb],gc_port);
	                    Loop1_ix = 0;
					}
		    		Com_Mode[com_nmb] = 87;
				}
	    	}
			else
				Com_Mode[com_nmb] = 0;
			break;

		case 86:	  // Send detailed faults for this car
  			if (Tranbuf_Empty[com_nmb]) 
  			{			
      			sendc(com_nmb,'F');
				sendc(com_nmb,cons[carnmb]);
				if (reset_flt_ix == 1)
				{
					diag_flt_ix = Flt_ix + 1;
					if (diag_flt_ix >= max_flt_ix)
						diag_flt_ix = 0;
				}
				i = 0;
				max_pkt_len = 68;

				if(reset_flt_ix != 1 && Loop2_ix == 0)
				{
					while (i < max_pkt_len)
					{
						sendc(com_nmb, 0);
						i++;
					}
				}
				else
				{
					while ((Loop2_ix < max_flt_ix) && (i < max_pkt_len))
					{
						if (f.Flts[diag_flt_ix].code != 0)
						{
							sendc(com_nmb,f.Flts[diag_flt_ix].code);
							sendc(com_nmb,(uint16)((f.Flts[diag_flt_ix].code >> 8) & 0x00ff));	// high byte
							sendc(com_nmb,f.Flts[diag_flt_ix].day);                         // day
							sendc(com_nmb,f.Flts[diag_flt_ix].mon);                         // month
							sendc(com_nmb,(f.Flts[diag_flt_ix].year >> 8) & 0x00FF);     	// HIGH BYTE year 
							sendc(com_nmb,(f.Flts[diag_flt_ix].year & 0x00FF));          	// LOW BYTE year 
							sendc(com_nmb,f.Flts[diag_flt_ix].min);                         // minute
							sendc(com_nmb,f.Flts[diag_flt_ix].hour);                        // hour
							sendc(com_nmb,f.Flts[diag_flt_ix].sec);							// seconds
							sendc(com_nmb,f.Flts[diag_flt_ix].pos);                         // position
							sendc(com_nmb,(uint16)((f.Flts[diag_flt_ix].count & 0x00ff)));            // low byte nmb occur
							sendc(com_nmb,f.Flts[diag_flt_ix].servf);						// service flag
							sendc(com_nmb,f.Flts[diag_flt_ix].procf);						// procedure flag
							sendc(com_nmb,f.Flts[diag_flt_ix].doorf);						// door flag
							sendc(com_nmb,f.Flts[diag_flt_ix].rdoorf);						// rear door flag
							sendc(com_nmb,f.Flts[diag_flt_ix].dpref);						// direction preference flag
							sendc(com_nmb,f.Flts[diag_flt_ix].dirf);						// direction flag
							sendc(com_nmb,f.Flts[diag_flt_ix].empf);						// emergency power car status flag
							sendc(com_nmb,f.Flts[diag_flt_ix].medf);						// Mass. Medical Emergency Service
							sendc(com_nmb,f.Flts[diag_flt_ix].codebf);						// code blue flag
							sendc(com_nmb,f.Flts[diag_flt_ix].eqf);							// earthquake flag
							sendc(com_nmb,f.Flts[diag_flt_ix].firef);						// fire flag
							sendc(com_nmb,f.Flts[diag_flt_ix].rfiref);						// rear door fire flag
							sendc(com_nmb,f.Flts[diag_flt_ix].hsf);							// high speed flag
							sendc(com_nmb,f.Flts[diag_flt_ix].startf);						// start flag
							sendc(com_nmb,f.Flts[diag_flt_ix].dcalls);						// direction of calls
							sendc(com_nmb,f.Flts[diag_flt_ix].estop);						// emergency stop
							sendc(com_nmb,f.Flts[diag_flt_ix].nstopf);						// need to stop flag
							sendc(com_nmb,f.Flts[diag_flt_ix].relevel);						// relevel
							sendc(com_nmb,f.Flts[diag_flt_ix].stepf);						// position stepping flag
							sendc(com_nmb,f.Flts[diag_flt_ix].door_seqf);					// door sequence flag
							sendc(com_nmb,f.Flts[diag_flt_ix].next_stop);					// next stop
							sendc(com_nmb,f.Flts[diag_flt_ix].ins_status);					// inspection status
							sendc(com_nmb,f.Flts[diag_flt_ix].ncu_door_seq);				// ncu door sequence
							sendc(com_nmb,f.Flts[diag_flt_ix].device);						// device that had the fault
							sendc(com_nmb,f.Flts[diag_flt_ix].prog_flag1);					// prog flag 1
							sendc(com_nmb,f.Flts[diag_flt_ix].prog_flag2);					// prog flag 2
							sendc(com_nmb,f.Flts[diag_flt_ix].device_2);					// device 2 that had the fault
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[0]);					// io data 0
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[1]);					// io data 1
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[2]);					// io data 2
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[3]);					// io data 4
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[4]);					// io data 5
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[5]);					// io data 6
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[6]);					// io data 7
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[7]);					// io data 9
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[8]);					// io data 10
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[9]);					// io data 12
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[10]);					// io data 13
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[11]);					// io data 72
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[12]);					// io data 73
							sendc(com_nmb,f.Flts[diag_flt_ix].iodata[13]);					// io data 74
							uiu.dw = f.Flts[diag_flt_ix].statusf;								// status flag
							sendc(com_nmb,uiu.b[lb]);
							sendc(com_nmb,uiu.b[mlb]);
							sendc(com_nmb,uiu.b[mhb]);
							sendc(com_nmb,uiu.b[hb]);
							uiu.dw = f.Flts[diag_flt_ix].dpp_count;								// digital position current count in pulses
							sendc(com_nmb,uiu.b[lb]);
							sendc(com_nmb,uiu.b[mlb]);
							sendc(com_nmb,uiu.b[mhb]);
							sendc(com_nmb,uiu.b[hb]);
							uiu.dw = f.Flts[diag_flt_ix].sd_count;							// target's digital position pulse count
							sendc(com_nmb,uiu.b[lb]);
							sendc(com_nmb,uiu.b[mlb]);
							sendc(com_nmb,uiu.b[mhb]);
							sendc(com_nmb,uiu.b[hb]);
#if (Traction == 1)
							uwu.w = f.Flts[diag_flt_ix].drv_vel_fpm;		// demand velocity
#else
							uwu.w = f.Flts[diag_flt_ix].drv_vel_fpm;
#endif
							sendc(com_nmb,uwu.b[lbw]);
							sendc(com_nmb,uwu.b[hbw]);
#if (Traction == 1)
							uwu.w = f.Flts[diag_flt_ix].enc_vel_fpm;		// encoder velocity
#else
							uwu.w = f.Flts[diag_flt_ix].enc_vel_fpm;
#endif
							sendc(com_nmb,uwu.b[lbw]);
							sendc(com_nmb,uwu.b[hbw]);

							i+=68;
						}
						diag_flt_ix++;	  
						if (diag_flt_ix >= max_flt_ix)
							diag_flt_ix = 0;
						Loop2_ix++;
					}

					if (Loop2_ix >= max_flt_ix)
					{
						//diag_flt_ix = 0;
						Loop2_ix = 0;
						while (i < max_pkt_len)
						{				// Packet must be a fixed length so fill with zero's
							sendc(com_nmb,0);
							i++;
						}
					}
				}	
	      		sendc(com_nmb,'U');
				Com_Mode[com_nmb] = 0;
			}
			reset_flt_ix = 0;
			break;
 
		case 87:		// Send fault data
			if (Tranbuf_Empty[com_nmb])
			{	 
				if (reset_flt_ix == 1)
				{
					diag_flt_ix = Car_Flt_ix + 1;
					if (diag_flt_ix >= max_flt_ix)
					    diag_flt_ix = 0;
				}
				send_flt_data = 0;
				if (reset_flt_ix == 1) 
				{
		            i = get_pkt_status(36, grp_Car_Stat_req[com_nmb], gc_port, 100);
					if (i == 1)
						send_flt_data = 1;
					if ((i == 0) && (gc_dev_online[grp_Car_Stat_req[com_nmb]] == 0)) // car is offline
						send_flt_data = 1;
				}
				else
					send_flt_data = 1; 

				if (send_flt_data == 1)
				{			
				  	sendc(com_nmb,'F');
					sendc(com_nmb,grp_Car_Stat_req[com_nmb]);
					i = 0;
					max_pkt_len = 68;

					if(reset_flt_ix != 1 && Loop1_ix == 0)		// added if-else statement
					{
						while (i < max_pkt_len)
						{
							sendc(com_nmb, 0);
							i++;
						}
					}
					else
					{
						while ((Loop1_ix < max_flt_ix) && (i < max_pkt_len) && (gc_dev_online[grp_Car_Stat_req[com_nmb]] != 0))
						{
							if (cf.Flts[diag_flt_ix].code != 0)
							{
								sendc(com_nmb,cf.Flts[diag_flt_ix].code);
								sendc(com_nmb,(uint16)((cf.Flts[diag_flt_ix].code >> 8) & 0x00ff));	// high byte
								sendc(com_nmb,cf.Flts[diag_flt_ix].day);                        // day
								sendc(com_nmb,cf.Flts[diag_flt_ix].mon);                        // month
								sendc(com_nmb,(cf.Flts[diag_flt_ix].year >> 8) & 0x00FF);     	// HIGH BYTE year 
								sendc(com_nmb,(cf.Flts[diag_flt_ix].year & 0x00FF));          	// LOW BYTE year 
								sendc(com_nmb,cf.Flts[diag_flt_ix].min);                        // minute
								sendc(com_nmb,cf.Flts[diag_flt_ix].hour);                       // hour
								sendc(com_nmb,cf.Flts[diag_flt_ix].sec);						// seconds
								sendc(com_nmb,cf.Flts[diag_flt_ix].pos);                        // position
								sendc(com_nmb,(uint16)(cf.Flts[diag_flt_ix].count & 0x00ff));           // low byte nmb occur
								sendc(com_nmb,cf.Flts[diag_flt_ix].servf);						// service flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].procf);						// procedure flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].doorf);						// door flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].rdoorf);						// rear door flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].dpref);						// direction preference flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].dirf);						// direction flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].empf);						// emergency power car status flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].medf);						// Mass. Medical Emergency Service
								sendc(com_nmb,cf.Flts[diag_flt_ix].codebf);						// code blue flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].eqf);						// earthquake flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].firef);						// fire flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].rfiref);						// rear door fire flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].hsf);						// high speed flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].startf);						// start flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].dcalls);						// direction of calls
								sendc(com_nmb,cf.Flts[diag_flt_ix].estop);						// emergency stop
								sendc(com_nmb,cf.Flts[diag_flt_ix].nstopf);						// need to stop flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].relevel);					// relevel
								sendc(com_nmb,cf.Flts[diag_flt_ix].stepf);						// position stepping flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].door_seqf);					// door sequence flag
								sendc(com_nmb,cf.Flts[diag_flt_ix].next_stop);					// next stop
								sendc(com_nmb,cf.Flts[diag_flt_ix].ins_status);					// inspection status
								sendc(com_nmb,cf.Flts[diag_flt_ix].ncu_door_seq);				// ncu door sequence
								sendc(com_nmb,cf.Flts[diag_flt_ix].device);						// device that had the fault
								sendc(com_nmb,cf.Flts[diag_flt_ix].prog_flag1);					// prog flag 1
								sendc(com_nmb,cf.Flts[diag_flt_ix].prog_flag2);					// prog flag 2
								sendc(com_nmb,cf.Flts[diag_flt_ix].device_2);					// device 2 that had the fault
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[0]);					// io data 0
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[1]);					// io data 1
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[2]);					// io data 2
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[3]);					// io data 4
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[4]);					// io data 5
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[5]);					// io data 6
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[6]);					// io data 7
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[7]);					// io data 9
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[8]);					// io data 10
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[9]);					// io data 12
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[10]);					// io data 13
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[11]);					// io data 72
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[12]);					// io data 73
								sendc(com_nmb,cf.Flts[diag_flt_ix].iodata[13]);					// io data 74
								uiu.dw = cf.Flts[diag_flt_ix].statusf;								// status flag
								sendc(com_nmb,uiu.b[lb]);
								sendc(com_nmb,uiu.b[mlb]);
								sendc(com_nmb,uiu.b[mhb]);
								sendc(com_nmb,uiu.b[hb]);
								uiu.dw = cf.Flts[diag_flt_ix].dpp_count;							// digital position current count in pulses
								sendc(com_nmb,uiu.b[lb]);
								sendc(com_nmb,uiu.b[mlb]);
								sendc(com_nmb,uiu.b[mhb]);
								sendc(com_nmb,uiu.b[hb]);
								uiu.dw = cf.Flts[diag_flt_ix].sd_count;							// target's digital position pulse count
								sendc(com_nmb,uiu.b[lb]);
								sendc(com_nmb,uiu.b[mlb]);
								sendc(com_nmb,uiu.b[mhb]);
								sendc(com_nmb,uiu.b[hb]);
#if (Traction == 1)
								uwu.w = cf.Flts[diag_flt_ix].drv_vel_fpm;	// demand velocity
#else
								uwu.w = f.Flts[diag_flt_ix].drv_vel_fpm;
#endif
								sendc(com_nmb,uwu.b[lbw]);
								sendc(com_nmb,uwu.b[hbw]);
#if (Traction == 1)
								uwu.w = cf.Flts[diag_flt_ix].enc_vel_fpm;		// encoder velocity
#else
								uwu.w = f.Flts[diag_flt_ix].enc_vel_fpm;
#endif
								sendc(com_nmb,uwu.b[lbw]);
								sendc(com_nmb,uwu.b[hbw]);

								i+=68;
							}
							diag_flt_ix++;
							if (diag_flt_ix >= max_flt_ix)
						    	diag_flt_ix = 0;
							Loop1_ix++;
						}
						if ((Loop1_ix >= max_flt_ix)||(gc_dev_online[grp_Car_Stat_req[com_nmb]] == 0))
						{
							Loop1_ix = 0;
							while (i < max_pkt_len)
							{				// Packet must be a fixed length so fill with zero's
								sendc(com_nmb,0);
								i++;
							}
						}
					}
				  	sendc(com_nmb,'U');
					Com_Mode[com_nmb] = 0;
					reset_flt_ix = 0;	
				}
			}
			break;

		case 88:		 // send variables from 360 to 480	 (120 variables)
		  	if (Tranbuf_Empty[com_nmb])
		  	{			  
				if (galcom_ver >= 6)
				{
			      	sendc(com_nmb,0x88);

					qq = 0;
				    for(i=360; i<480; i++)
				    {									   // calculate checksum
						qq += fvars[i];
				    }
					sendc(com_nmb,qq & 0x0ff);					// send checksum
					sendc(com_nmb,((qq >> 8) & 0x00ff));

				    for(i=360; i<480; i++)
				    {
						sendc(com_nmb,fvars[i] & 0x00ff);
						sendc(com_nmb,((fvars[i] >> 8) & 0x00ff));
				    }
				    sendc(com_nmb,'V');
				}
				Com_Mode[com_nmb] = 0;
		  	}
			break;

		case 89:			 // receive variable from 360 to 480  (120 variables)
			bbb = (uint16)(rmbuf[com_nmb][3] & 0x00ff);		// read in checksum
			bbb |= ((rmbuf[com_nmb][4] << 8) & 0xff00);
			ck_sum = 0;
			for(i=0; i<240; i=(i+2))
			{				// calculate checksum
			  	qq = (uint16)(rmbuf[com_nmb][i+5]&0x00ff);
			  	qq |= ((rmbuf[com_nmb][i+6] << 8)&0xff00);
				ck_sum += qq;
			}
			if (ck_sum == ((int16)bbb))
			{		// if checksum matches then update variables
				Var_Nmb = 360;
			  	for(i=0; i<240; i=(i+2))
			  	{	   
					if(Var_Nmb != fvbaud)
					{	// do not allow the laptop update the serial comm baud rate
					  	qq = (uint16)(rmbuf[com_nmb][i+5]&0x00ff);
					  	qq |= ((rmbuf[com_nmb][i+6] << 8)&0xff00);
						if ((qq >= fvars_min[Var_Nmb]) && (qq <= fvars_max[Var_Nmb]))
					  		fvars[Var_Nmb] = qq;
			  		}
					Var_Nmb++;
			  	}
			  	wrfvar();
			}
		   	Com_Mode[com_nmb] = 88;
			break;
			
		case 90:
			Restart_Com(com_nmb);
			Com_Mode[com_nmb] = 0;
			break;
			
		case 91:
			sendc(com_nmb,0x91);
			for(i = 0;i<9; i++)
			{ 
				sendc(com_nmb,(unsigned char)grp_Car_Stat[i].g_car_alrm);		  // skip first index
			}
			sendc(com_nmb,'o');
			Com_Mode[com_nmb] = 0;
			break;
			
		case 92:
			sendc(com_nmb,0x92);
			for(i = 1;i<9;i++)
			{
			   	uwu.w = grp_Car_Stat[i].g_cur_flt;
				sendc(com_nmb,uwu.b[hbw]);
				sendc(com_nmb,uwu.b[lbw]);
			}
			sendc(com_nmb,'o');
			Com_Mode[com_nmb] = 0;
			break;
			
		case 93:
			// byte 3 floor
			// byte 4 car number
			// byte 5 rear operation
					   
	    	fl_ix = ((int16)rmbuf[com_nmb][3]-1)/32;	
						
			if( rmbuf[com_nmb][5] == 0xff)
			{
				if(((rmbuf[com_nmb][3]>= 1)&& (rmbuf[com_nmb][3] <= cons[grtopf])) &&(rmbuf[com_nmb][4] <= cons[nmbcars]))
				{
					front_vip = 0;
					rear_vip = 1;
					VipCarNmb = rmbuf[com_nmb][4];
					gc_rvip_set[fl_ix]|=fl_mask[rmbuf[com_nmb][3]];
				}
			}
			else
			{
				if(((rmbuf[com_nmb][3]>= 1)&& (rmbuf[com_nmb][3] <= cons[grtopf])) &&(rmbuf[com_nmb][4] <= cons[nmbcars]))
				{
					front_vip = 1;
					rear_vip = 0;
					VipCarNmb = rmbuf[com_nmb][4];
					gc_vip_set[fl_ix]|=fl_mask[rmbuf[com_nmb][3]];
				}
			} 			

	    	Com_Mode[com_nmb] = 0;
			break;	
			

		case 94:						// Receive User Interface front hall call Security floor lockouts
			// Up and Down Car Hall call security bits
			el = rmbuf[com_nmb][3];
			
			uiu.b[lb] = rmbuf[com_nmb][4];
			uiu.b[mlb] = rmbuf[com_nmb][5];
			uiu.b[mhb] = rmbuf[com_nmb][6];
			uiu.b[hb] = rmbuf[com_nmb][7];
			ui_uhc_carsec[0][el] = uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][8];
			uiu.b[mlb] = rmbuf[com_nmb][9];
			uiu.b[mhb] = rmbuf[com_nmb][10];
			uiu.b[hb] = rmbuf[com_nmb][11];
			ui_uhc_carsec[1][el] = uiu.dw;

			uiu.b[lb] = rmbuf[com_nmb][12];
			uiu.b[mlb] = rmbuf[com_nmb][13];
			uiu.b[mhb] = rmbuf[com_nmb][14];
			uiu.b[hb] = rmbuf[com_nmb][15];
			ui_dhc_carsec[0][el] = uiu.dw;

			uiu.b[lb] = rmbuf[com_nmb][16];
			uiu.b[mlb] = rmbuf[com_nmb][17];
			uiu.b[mhb] = rmbuf[com_nmb][18];
			uiu.b[hb] = rmbuf[com_nmb][19];
			ui_dhc_carsec[1][el] = uiu.dw;

			Com_Mode[com_nmb] = 0;
			break;

		case 95:		// Send User Interface car front hall call security floors

			el = rmbuf[com_nmb][3];

			sendc(com_nmb,0x95);
			uiu.dw = ui_uhc_carsec[0][el];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);

			uiu.dw = ui_uhc_carsec[1][el];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);

			uiu.dw = ui_dhc_carsec[0][el];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);

			uiu.dw = ui_dhc_carsec[1][el];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);

	      	sendc(com_nmb,195);
			Com_Mode[com_nmb] = 0;
			break;
			

		case 96:						// Receive User Interface rear car hall call Security floor lockouts
		
			el = rmbuf[com_nmb][3];
		
			uiu.b[lb] = rmbuf[com_nmb][4];
			uiu.b[mlb] = rmbuf[com_nmb][5];
			uiu.b[mhb] = rmbuf[com_nmb][6];
			uiu.b[hb] = rmbuf[com_nmb][7];
			ui_ruhc_carsec[0][el] = uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][8];
			uiu.b[mlb] = rmbuf[com_nmb][9];
			uiu.b[mhb] = rmbuf[com_nmb][10];
			uiu.b[hb] = rmbuf[com_nmb][11];;
			ui_ruhc_carsec[1][el]	= uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][12];
			uiu.b[mlb] = rmbuf[com_nmb][13];
			uiu.b[mhb] = rmbuf[com_nmb][14];
			uiu.b[hb] = rmbuf[com_nmb][15];
			ui_rdhc_carsec[0][el]	= uiu.dw;
			
			uiu.b[lb] = rmbuf[com_nmb][16];
			uiu.b[mlb] = rmbuf[com_nmb][17];
			uiu.b[mhb] = rmbuf[com_nmb][18];
			uiu.b[hb] = rmbuf[com_nmb][19];
			ui_rdhc_carsec[1][el] = uiu.dw;

			Com_Mode[com_nmb] = 0;

			break;

		case 97:			// Send User Interface rear hall call security floors
			sendc(com_nmb,0x97);

			el = rmbuf[com_nmb][3];
			
			uiu.dw = ui_ruhc_carsec[0][el];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			
			uiu.dw = ui_ruhc_carsec[1][el];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			
			uiu.dw = ui_rdhc_carsec[0][el];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);
			
			uiu.dw = ui_rdhc_carsec[1][el];
			sendc(com_nmb,uiu.b[lb]);
			sendc(com_nmb,uiu.b[mlb]);
			sendc(com_nmb,uiu.b[mhb]);
			sendc(com_nmb,uiu.b[hb]);

	      	sendc(com_nmb,197);
			Com_Mode[com_nmb] = 0;

			break;
			
		case 98:		  // Send car_bld_no car labels
			sendc(com_nmb,'m'); 
#if (Tract_HR == 1)			
			for (i=1;i<=8;i++)
#else
			for (i=1;i<=6;i++)
#endif
			{
				sendc(com_nmb,car_bld_no[i][0]);
				sendc(com_nmb,car_bld_no[i][1]);
			}
			sendc(com_nmb,'z');
			Com_Mode[com_nmb] = 0;
			break;

		default:
	    	Com_Mode[com_nmb] = 0;
	    	break;
			
	}

}


/*
Revision History

3/21/12 v5.54.67 hdl	1. Added the Hall call security per car for galileo packets
9/7/12	v6.00.21 AS/mhd	1. Added packet 98 in comm_diag() to send car labels to lobby monitor
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.


*/
