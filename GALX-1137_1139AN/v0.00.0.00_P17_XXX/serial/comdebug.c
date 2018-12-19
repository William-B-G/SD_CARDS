#define d_COMDEBUG 1
#include "global.h"

/*
	Com Debug communications routine to use throught RS-232 ports USR1COM or USR2COM
	
*/

// Local Function declarations

void comm_debug(int16 comnmb);
int16 recvdbg(int16 Which_Com);
void sendstring(int16 comnmb, char *c);
void debug_exec_command (int16 comnmb, int16 cmd);
int32 com_dbg_get_numbers(int16 comnmb,int16 i);
int32 com_dbg_get_hex_numbers(int16 comnmb,int16 i);
int32 com_dbg_get_num(int16 comnmb,int16 buf_st);
int16 com_dbg_scan_cmd(int16 comnmb);
void com_dbg_mem_dump_screen(int16 comnmb);
void com_dbg_trace_dump_screen(int16 comnmb);
int16 com_dbg_LocNonSpace(int16 comnmb, int16 len);

static union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;	
					// unsigned word union
static union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
// External Function declarations

extern int16 asigetc(int16);

// Local Ram Declaration
char dbg_com_buf[3][300];
int32 com_dbg_num[3][4];
int16 com_dbg_mem_type = 0;
int16 com_dbg_mem_page = 0;
int16 com_dbg_mem_offset = 0;
int16 com_dbg_mem_len = 0;
int32 com_dbg_var_off = 0x40000000;

int16 com_dbg_rmbuf_len[3] = {0,0,0};

int32 com_dbg_variable[10];			 		
int16 com_dbg_trace_num;
int16 alt_tim_data = 0;
int16 mute_echo_on_port = 0;

/* added by hernan but needs to be included in header*/
extern void galcom_pack (int16 com_nmb, int16 msg);
extern int16 com_fvar_block_ix[2];

//*****************************************
//*****************************************
// Comm Debug Serial port
//*****************************************
//*****************************************
void comm_debug(int16 comnmb)
{
	int16 cmd;
	cmd = recvdbg(comnmb);
	if (cmd != 0)
	{
		debug_exec_command(comnmb,cmd);
	}
	if ((run_trace == 1) && (trace_complete == 1))
	{
		run_trace = 0;
		sprintf(&dbg_com_buf[comnmb][0],"Trace Complete\n\r");
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		sendc(comnmb,'>');
	}
		
}

//*****************************************
// Receive debug comm
//*****************************************

int16 recvdbg(int16 Which_Com)
{
  	int16 rbempt=0,endmess=0;
  	rbempt = isrxempty(Which_Com);
  	while((rbempt == 0) && (endmess == 0))
  	{
	  	rmbuf[Which_Com][mpnt[Which_Com]] = (char) asigetc(Which_Com);
	  	if(mute_echo_on_port == 0) // only send data back when not in mute (MUT) mode  mode
	  	{
	  		sendc(Which_Com, rmbuf[Which_Com][mpnt[Which_Com]]);		// echo character
	  	}
	  	mpnt[Which_Com]++;
	  	rbempt = isrxempty(Which_Com);
	  	if (rmbuf[Which_Com][mpnt[Which_Com]-1] == CR)
	  	{		// Command entry
	  		if (mpnt[Which_Com] > 30) 
	  		{
	  			rbempt = 1;	
	  			mpnt[Which_Com] = 0;
	  			if(mute_echo_on_port == 0) // only send data back when not in mute (MUT) mode  mode
	  			{
		  			sendc(Which_Com,LF);
		  			sendc(Which_Com,'>');
	  			}
	  			endmess = 0;
	  		}
	  		else if (mpnt[Which_Com] > 1)
	  		{
	  			endmess = com_dbg_scan_cmd(Which_Com);
	  			com_dbg_rmbuf_len[Which_Com] = mpnt[Which_Com];
	  			rbempt = 1;
	  			mpnt[Which_Com] = 0;
	  			if(mute_echo_on_port == 0) // only send data back when not in mute (MUT) mode  mode
	  			{
		  			sendc(Which_Com,LF);
		  			sendc(Which_Com,'>');
	  			}
	  		}
	  		else if (mpnt[Which_Com] == 1)
	  		{
	  			rbempt = 1;	
	  			mpnt[Which_Com] = 0;
	  			if(mute_echo_on_port == 0) // only send data back when not in mute (MUT) mode  mode
	  			{
		  			sendc(Which_Com,LF);
		  			sendc(Which_Com,'>');
	  			}
	  			endmess = 0;
	  		}
	  	}
	  	else if (rmbuf[Which_Com][mpnt[Which_Com]-1] == BS)
	  	{
	  		if (mpnt[Which_Com] > 0)
	  			mpnt[Which_Com]--;
	  		if (mpnt[Which_Com] > 0)
	  			mpnt[Which_Com]--;
	  		sendc(Which_Com,' ');
	  		sendc(Which_Com,BS);
	  	}
	  	else if (mpnt[Which_Com] > 35)
	  	{
	  		rbempt = 1;	
	  		mpnt[Which_Com] = 0;
	  		sendc(Which_Com,LF);
	  		sendc(Which_Com,'>');
	  		endmess = 0;
	  	}
  	}
  	return(endmess);
}

// ****************************************
// Send character string to comm port
// ****************************************


void sendstring(int16 comnmb, char *c)
{
	int16 i = 0;
	while ((c[i] != 0) && (i < 100))
	{
		sendc(comnmb,c[i++]);
	}
}
	
// **********************************************************
// Debug command **** Note: do no use the characters @ and ;
// **********************************************************

void debug_exec_command (int16 comnmb, int16 cmd)
{
	int32 i,j,k;
	int32 fl_ix;
	uint32 temp = 0;
	int32 *vptr32;
	int16 *vptr16;
	int8 *vptr8;
	int16 seq;
	bool HexInput;
	int16 FirstNonSpace;
	int16 len;
	int16 set_var = 0;
	char t_off[6];
	char d_off[6];
	uint16 mem_pattern[8] = {0x0101,0x0202,0x0404,0x0808,0x1010,0x2020,0x4040,0x8080};
	
	
	switch (cmd)
	{
		case 0:
			break;
		case 1:	// STS - Controller status
			sprintf(&dbg_com_buf[comnmb][0],"System Status \n\r");
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],"servf = %2d, procf = %2d \n\r",servf,procf);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],"pos = %2d, dpp = %10d \n\r",position, DPP_Count);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,CR);
			sendc(comnmb,LF);
			sendc(comnmb,'>');
			break;
		case 2:		// CCI - Set Car Call
			i = com_dbg_get_num(comnmb, 3);
			set_carcall(cons[carnmb],(uint16)i);
			break;
		case 3:		// CCC - Clear Car Call
			i = com_dbg_get_num(comnmb, 3);
			if ((i >= 1) && (i <= cons[grtopf]))   // allow to cancel any floor
			{
				clr_both_inps(i_CC(i),i_XCC(i));
				clr_both_outps(o_CCL(i),o_XCCL(i)); 
			    carcb[i] = 0;
			}
			break;
		case 4:		// UCI - Set Up Call
			i = com_dbg_get_num(comnmb, 3);
			set_upcall((uint16)i);
			break;
		case 5:		// UCC - Clear Up Call
			i = com_dbg_get_num(comnmb, 3);
			if ((i >= 1) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
#if (GALaxy_4 == 0)
				if (gc_hc_dev == 0)
				{
					clrinp(i_UC(i));
					clroutp(o_UCL(i)); 
				}
				else
#endif
					gc_uc_canc[fl_ix] |= fl_mask[i];
				g_up_hall_call[fl_ix][0] &= ~fl_mask[i];
				g_asign_up[i] = 0;
			}
			break;
		case 6:		// DCI - Set Down Call
			i = com_dbg_get_num(comnmb, 3);
			set_dncall((uint16)i);
			break;
		case 7:		// DCC - Clear Down Call
			i = com_dbg_get_num(comnmb, 3);
			if ((i >= 1) && (i <= cons[grtopf]))
			{
				fl_ix = (i-1)/32;
#if (GALaxy_4 == 0)
				if (gc_hc_dev == 0)
				{
					clrinp(i_DC(i));
					clroutp(o_DCL(i)); 
				}
				else
#endif
					gc_dc_canc[fl_ix] |= fl_mask[i];
				g_dn_hall_call[fl_ix][0] &= ~fl_mask[i];
				g_asign_dn[i] = 0;
			}
			break;
		case 8:		// RCI - Set Rear Car Call
			i = com_dbg_get_num(comnmb, 3);
			set_rcarcall(cons[carnmb],(uint16)i);
			break;
		case 9:		// RUI - Set Rear Up Call
			i = com_dbg_get_num(comnmb, 3);
			set_rupcall((uint16)i);
			break;
		case 10:		// RDI - Set Rear Down Call
			i = com_dbg_get_num(comnmb, 3);
			set_rdncall((uint16)i);
			break;
		case 11:	// RCC - Rear Car Call Clear
			break;
		case 12: 	// RUC - Rear Up Call Clear
			break;
		case 13: 	// RDC - Rear Down Call Clear
			break;
		case 14:	// CBI - Set Code Blue Call
			break;
		case 15: 	// CBC - Code Blue Call Clear
			break;
		case 16: 	// IUI - Inconspicious Riser Up Call
			break;
		case 17: 	// IDI - Inconspicious Riser Up Call
			break;
		case 18: 	// IUC - Inconspicious Riser Up Call Clear
			break;
		case 19: 	// IDC - Inconspicious Riser Up Call Clear
			break;
		case 20: 	// SUI - Inconspicious Riser Up Call
			break;
		case 21: 	// SDI - Inconspicious Riser Up Call
			break;
		case 22: 	// SUC - Inconspicious Riser Up Call Clear
			break;
		case 23: 	// SDC - Inconspicious Riser Up Call Clear
			break;
		case 24:		// TST - Door limit Test AND fault test
			i = com_dbg_get_num(comnmb, 3);
			if (i == 0)
			{
#if (Simulator == 1)
				dcl_test = 0;
				dol_test = 0;
				dc_test = 0;
				sim_stall = 0;
				sim_enc_dir = 0;
				sim_SEL_test = 0;
				sim_ul_dl_swap = 0;
#endif
			}
#if (Simulator == 1)
			else if (i == 1)
				dcl_test = 1;
			else if (i == 2)
				dol_test = 1;
			else if (i == 3)
				dc_test = 1;
			else if (i==4)
			{
			}
			else if (i == 5)
			{
				setinp(i_DLM);	// Turn on door lock middle
				setinp(i_DLM_1);	// Turn on door lock middle
			}
			else if (i == 6)
				sim_DPP_Count = S_DPP_Floor_Pos[sim_pos] - (int32)(2.0 * Pulses_per_Inch);
			else if (i == 7)
				sim_DPP_Count = S_DPP_Floor_Pos[sim_pos] - (int32)(6.0 * Pulses_per_Inch);
			else if (i == 8)
			{
				sim_DPP_Count = S_DPP_Floor_Pos[sim_pos] + (int32)(2.0 * Pulses_per_Inch);
//				sim_stall = 1;
			}
			else if (i == 9)
			{
				sim_DPP_Count = S_DPP_Floor_Pos[sim_pos] + (int32)(6.0 * Pulses_per_Inch);
//				sim_stall = 1;
			}
			else if (i == 10)
				sim_stall = 1;
			else if (i == 19)
				rdfvar();
			else if (i == 20)
				record_fault(f_hcb_fet_short_up);
			else if (i == 21)
				put_pkt_status(3,2,DRVCOM);
			else if (i == 22)
				sim_enc_dir = 1;
			else if (i == 23)
				sim_enc_dir = 2;
			else if (i == 24)
				sim_SEL_test = 1;
			else if (i == 26)
			{
				L1_Voltage = 2401;
				L2_Voltage = 2382;
				L3_Voltage = 2393;
				Door_Voltage = 2084;
			}
			else if (i == 27)
			{
				L1_Voltage = 0;
				L2_Voltage = 0;
				L3_Voltage = 0;
				Door_Voltage = 0;
			}
			else if (i == 28)
				sim_ul_dl_swap = 1;
#endif
			else if (i == 29)
			{
				debug1 = 0;
				debug2 = 0;
				debug3 = 0;
				debug4 = 0;
				debug5 = 0;
				debug6 = 0;
				debug7 = 0;
				debug8 = 0;
			}
			else if (i == 30)
			{				
				debug1 = 1;
				debug2 = 0;
			}
			else if (i == 31)
				put_dpyspi = 1;
			else if (i == 32)
				put_dpyspi = 2;
			else if (i == 33)
			{
#if (Simulator == 1)
				nudgst |= 0x07;
				rnudgst |= 0x07;
#endif
			}
			else if (i == 34)
			{		// call data test
				sprintf(&dbg_com_buf[comnmb][0],"Clear Call Data \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
				clear_call_data();
			}
			else if (i == 35)
			{
				sprintf(&dbg_com_buf[comnmb][0],"Call Data Test \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
		  	    for(i=0; i<=96; i++)
		  	    {
					for(j=0; j<fl_size; j++)
					{
						nmb_up_call[i][j] = (int16)j;
						nmb_dn_call[i][j] = (int16)j;
					}
		  	    }
		  	    
		  	    set_var = old_day_of_week;
		  	    old_day_of_week = day_of_week;
					
				Up_Date_Calls();	
		  	    for(i=0; i<=96; i++)
		  	    {
					for(j=0; j<fl_size; j++)
					{
						nmb_up_call[i][j] = 0;
						nmb_dn_call[i][j] = 0;
					}
		  	    }
		  	    old_day_of_week = set_var;
				init_call_data();
				k = 0;
		  	    for(i=0; i<=96; i++)
		  	    {
					for(j=0; j<fl_size; j++)
					{
						if (k < 100)
						{
							sprintf(&dbg_com_buf[comnmb][0],"U=%2i,%2i,%2i \n\r",nmb_up_call[i][j],i,j);
							sendstring(comnmb,&dbg_com_buf[comnmb][0]);
							sprintf(&dbg_com_buf[comnmb][0],"D=%2i,%2i,%2i \n\r",nmb_dn_call[i][j],i,j);
							sendstring(comnmb,&dbg_com_buf[comnmb][0]);
						}
						k++;
					}
		  	    }
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 36)
			{		// field variable checksum test
				sprintf(&dbg_com_buf[comnmb][0],"Fault Checksum Test \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				
				wrfvar();
				set_var = verify_fvars(0);
				sprintf(&dbg_com_buf[comnmb][0],"Standard Table Error = %4x \n\r",set_var);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				set_var = verify_fvars(1);
				sprintf(&dbg_com_buf[comnmb][0],"Backup Table Error = %4x \n\r",set_var);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				

				j = chksum_fvars(0);
				sprintf(&dbg_com_buf[comnmb][0],"Std Calc Cksum = %4x \n\r",j);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				j = get_fvars_chksum(0);
				sprintf(&dbg_com_buf[comnmb][0],"Std Mem Cksum  = %4x \n\r",j);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				j = chksum_fvars(1);
				sprintf(&dbg_com_buf[comnmb][0],"Backup Calc Cksum = %4x \n\r",j);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				j = get_fvars_chksum(1);
				sprintf(&dbg_com_buf[comnmb][0],"Backup Mem Cksum  = %4x \n\r",j);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);

				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 37)
			{		// Job Statistics test
				sprintf(&dbg_com_buf[comnmb][0],"Clear Job Statistics Test \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				
				clear_job_statistics();
				
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 38)
			{
				sprintf(&dbg_com_buf[comnmb][0],"Preset Job Statistics \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				for(i=0; i<nmblvar; i++)
				{
					lvars[i] = 30 * (i + 1);
				}
				Update_Job_Statistics();

				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 39)
			{
				sprintf(&dbg_com_buf[comnmb][0],"Display Job Statistics \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				for(i=0; i<nmblvar; i++)
				{
					sprintf(&dbg_com_buf[comnmb][0],"Lvar[%2i]=%5i \n\r",i, lvars[i]);
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				}

				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 40)
			{		// Clear User Data
				sprintf(&dbg_com_buf[comnmb][0],"Clear User Data Test \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				
				reset_uvars();
				
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 41)
			{
				sprintf(&dbg_com_buf[comnmb][0],"Preset User Data \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
			    i=0;
			    while(i < 18)
			    {
					telnmb[i] = (int8)((i & 0x07)+0x30);
					
					if ((telnmb[i] < '0') || (telnmb[i] > '9'))
					{
						if ((telnmb[i] != CR) && (telnmb[i] != '.') && (telnmb[i] != '-'))
							telnmb[i] = CR;
					}	
					i++;
			    }
			    telnmb[i] = CR;
				if (telnmb[i] != CR)
					telnmb[i] = CR;
				
			  	for(j=2; j<=7; j++)
			  	{
					i=0;
					while(i < 18)      // reading in user defineable passwords
					{
					    pass[j][i] = (int8)LCD_Fault[j][i];
					    i++;
					}
					pass[j][i] = CR;
					i=0;
					while(i <18)       // reading in user defineable user names
					{
					    user[j][i] = (int8)LCD_Fault[j][i];
					    i++;
					}
					user[j][i] = CR;
			  	}
				wruvar();

				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 42)
			{
				sprintf(&dbg_com_buf[comnmb][0],"Display User Data \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);

				sprintf(&dbg_com_buf[comnmb][0],"telnmb=%.18s \n\r", telnmb);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);

			  	for(j=2; j<=7; j++)
			  	{
					sprintf(&dbg_com_buf[comnmb][0],"pass[%2i]=%.18s \n\r",j, pass[j]);
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					sprintf(&dbg_com_buf[comnmb][0],"user[%2i]=%.18s \n\r",j, user[j]);
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			  	}

				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 43)
			{		// Clear Service Timers
				sprintf(&dbg_com_buf[comnmb][0],"Clear Service Timers \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				
				Reset_Service_Timers();
				
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 44)
			{
				sprintf(&dbg_com_buf[comnmb][0],"Preset Service Timers \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);

				for (i=0;i<=9;i++)
				{
					
					svc_t[i].service = (int8)(i+1);
					svc_t[i].status = 1;
					svc_t[i].logic = 2;
					svc_t[i].option = 3;
					for(j=0;j<=2;j++)
					{
						svc_t[i].month[j] = (int8)(j + 1);
						svc_t[i].day[j] = (int8)(j + 1);
						svc_t[i].md_on_hour[j] = (int8)(j + 1);
						svc_t[i].md_off_hour[j] = (int8)(j + 1);
						svc_t[i].md_on_minute[j] = (int8)(j + 1);
						svc_t[i].md_off_minute[j] = (int8)(j + 1);
					}
					for(j=0;j<=6;j++)
					{
						svc_t[i].on_hour[j] = (int8)(j + 1);
						svc_t[i].off_hour[j] = (int8)(j + 1);
						svc_t[i].on_minute[j] = (int8)(j + 1);
						svc_t[i].off_minute[j] = (int8)(j + 1);
					}
					Wrt_Grp_Svc_Timer((int16)i);
				}
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 45)
			{
				sprintf(&dbg_com_buf[comnmb][0],"Display Service Timers \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				delay(10000);

				for (i=0;i<=9;i++)
				{
					sprintf(&dbg_com_buf[comnmb][0],"Service Timer %2i \n\r", i);
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					delay(10000);
					
					sprintf(&dbg_com_buf[comnmb][0],"  service=%.2i, status=%.2i, logic=%.2i, option=%.2i \n\r", 
							svc_t[i].service, svc_t[i].status,svc_t[i].logic,svc_t[i].option);
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					delay(10000);
					for(j=0;j<=2;j++)
					{
						sprintf(&dbg_com_buf[comnmb][0],"    month=%.2i, day=%.2i \n\r", svc_t[i].month[j], svc_t[i].day[j]);
						sendstring(comnmb,&dbg_com_buf[comnmb][0]);
						delay(10000);
						
						sprintf(&dbg_com_buf[comnmb][0],"    md_on_hour=%.2i, md_off_hour=%.2i, md_on_minute=%.2i, md_off_minute=%.2i \n\r", 
								svc_t[i].md_on_hour[j], svc_t[i].md_off_hour[j],svc_t[i].md_on_minute[j],svc_t[i].md_off_minute[j]);
						sendstring(comnmb,&dbg_com_buf[comnmb][0]);
						delay(10000);
					}
					for(j=0;j<=6;j++)
					{
						sprintf(&dbg_com_buf[comnmb][0],"    on_hour=%.2i, off_hour=%.2i, on_minute=%.2i, off_minute=%.2i \n\r", 
								svc_t[i].on_hour[j], svc_t[i].off_hour[j],svc_t[i].on_minute[j],svc_t[i].off_minute[j]);
						sendstring(comnmb,&dbg_com_buf[comnmb][0]);
						delay(10000);
					}
					petdog();
					delay(50000);
					petdog();
					delay(50000);
					petdog();
					delay(50000);
					petdog();
				}

				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 46)
			{		// zero flight time
				sprintf(&dbg_com_buf[comnmb][0],"Clear Flight Times \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				
			   	for(i=1; i<=cons[grtopf]; i++)
			   	{
					for(j=1; j<=cons[grtopf]; j++)
			   		{
						flight_times[i][j] = 0;
			  		}
			   	}
				Update_Flight_Times();
				
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 47)
			{	// Preset Flight times
				sprintf(&dbg_com_buf[comnmb][0],"Preset Flight Times \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				
				init_flight_times(0);
				
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 48)
			{	// Display flight times
				sprintf(&dbg_com_buf[comnmb][0],"Display Flight Times \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				delay(10000);

			   	for(i=1; i<=cons[grtopf]; i++)
			   	{
					for(j=1; j<=cons[grtopf]; j++)
			   		{
						sprintf(&dbg_com_buf[comnmb][0],"  Flight Time Floor %.2i to %.2i = %.6i\n\r", i,j, flight_times[i][j]);
						sendstring(comnmb,&dbg_com_buf[comnmb][0]);
						delay(10000);
			   		}
					petdog();
					delay(50000);
					sendc(comnmb,CR);
					sendc(comnmb,LF);
					petdog();
					delay(50000);
					petdog();
				}
					
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 49)
			{		// zero nts variable
				sprintf(&dbg_com_buf[comnmb][0],"Clear NTS Variable \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				
				nts_fvars[0] = 0;		// top speed
				nts_fvars[14] = 0;		// dt6
				wr_nts_fvar();
				
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 50)
			{	// Preset Flight times
				sprintf(&dbg_com_buf[comnmb][0],"Preset NTS Variable  \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				
				nts_fvars[0] = 235;		// top speed
				nts_fvars[14] = 153;		// dt6
				wr_nts_fvar();
				
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else if (i == 51)
			{	// Display flight times
				sprintf(&dbg_com_buf[comnmb][0],"Display NTS Variable  \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				delay(10000);

				sprintf(&dbg_com_buf[comnmb][0],"  Top Speed %.6i \n\r", nts_fvars[0]);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				delay(10000);
				sprintf(&dbg_com_buf[comnmb][0],"  DT6 Speed %.6i \n\r", nts_fvars[14]);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			else 
			{
#if (Simulator == 1)
				dcl_test = 0;
				dol_test = 0;
				dc_test = 0;
	  			sim_enc_dir = 0;
				sim_stall = 0;
				sim_SEL_test = 0;
				sim_ul_dl_swap = 0;
#endif
			}
			break;
		case 25:			// IOS - IO Sim Data Set: IOS array0-78,bit=0-7 ex. IOS 22,0 sets bit
#if (Simulator == 1)
			k = com_dbg_get_numbers(comnmb, 3);
			if (k == 2)
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if (com_dbg_num[comnmb][0] < 18)
				{
					ioset[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					ioclr[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
				else if (com_dbg_num[comnmb][0] < 138)
				{
					mrc_ioset[com_dbg_num[comnmb][0]-18] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					mrc_ioclr[com_dbg_num[comnmb][0]-18] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
				else if (com_dbg_num[comnmb][0] < 258)
				{
					ctc_ioset[com_dbg_num[comnmb][0]-138] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					ctc_ioclr[com_dbg_num[comnmb][0]-138] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
					
	#else
				if (com_dbg_num[comnmb][0] <= 80)
				{
					ioset[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					ioclr[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#endif
			}
#endif			
			break;
		case 26:			// IOC - IO Sim Data Clear: IOC array0-78,bit=0-7 ex. IOC 22,0 Clears bit
#if (Simulator == 1)
			k = com_dbg_get_numbers(comnmb, 3);
			if (k == 2)
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if (com_dbg_num[comnmb][0] < 18)
				{
					ioset[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					ioclr[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
				else if (com_dbg_num[comnmb][0] < 138)
				{
					mrc_ioset[com_dbg_num[comnmb][0]-18] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					mrc_ioclr[com_dbg_num[comnmb][0]-18] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
				else if (com_dbg_num[comnmb][0] < 258)
				{
					ctc_ioset[com_dbg_num[comnmb][0]-138] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					ctc_ioclr[com_dbg_num[comnmb][0]-138] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
					
	#else
				if (com_dbg_num[comnmb][0] <= 80)
				{ 
					ioclr[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					ioset[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#endif
			}
			else if (k == 3)
			{
				if (com_dbg_num[comnmb][0] < 255)
				{
					grc_ioset[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					grc_ioclr[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
			}
#endif
			break;
		case 27:			// RIO - Reset IO: RIO array0-78,bit=0-7 ex. RIO 22,0 Resets bit to normal state
#if (Simulator == 1)			
			k = com_dbg_get_numbers(comnmb, 3);
			if (k == 2)
			{			  // must have two values
				if (com_dbg_num[comnmb][0] <= 80)
				{ 
					ioclr[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					ioset[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
			}
			
#endif
			break;
		case 28:			// RAI - Reset all IO
#if (Simulator == 1)
	#if (GALaxy_4 == 1)
			for (i=0;i<nmb_io_arrays;i++)
			{
				ioclr[i] = 0xFF;
				ioset[i] = 0;
			}
			for (i=0;i<c_max_ctcan_ser_array;i++)
			{
				ctc_ioclr[i] = 0xff;
				ctc_ioset[i] = 0;
			}
			for (i=0;i<c_max_mrcan_ser_array;i++)
			{
				mrc_ioclr[i] = 0xff;
				mrc_ioset[i] = 0;
			}
			for (i=0;i<c_max_grcan_ser_array;i++)
			{
				grc_ioclr[i] = 0xff;
				grc_ioset[i] = 0;
			}
	#else

			for (i=0;i<c_max_ser_addr;i++)
			{
				cioclr[i] = 0xff;
				cioset[i] = 0;
			}
	#endif

#endif
			break;
		case 29:			// CIS - CAN IO Sim Data Set: CIS brd no, bit   CIS 19,2<CR> sets bit
#if (Simulator == 1)						   // Look up brd no from HMMAP.H i.e. BRD14cB = 19
			k = com_dbg_get_numbers(comnmb, 3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 0)

				if ((com_dbg_num[comnmb][0] < c_max_ser_addr) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					cioset[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					cioclr[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#else
				if ((com_dbg_num[comnmb][0] < c_max_ctcan_ser_array) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					ctc_ioset[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					ctc_ioclr[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#endif
			}
#endif			
			break;
		case 30:			// CIC - CAN IO Sim Data Clear: CIC brd no, bit   CIC 19,2<CR> sets bit
#if (Simulator == 1)					  // Look up brd no from HMMAP.H i.e. BRD14cB = 19
			k = com_dbg_get_numbers(comnmb, 3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 0)
				if ((com_dbg_num[comnmb][0] < c_max_ser_addr) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					cioclr[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					cioset[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#else
				if ((com_dbg_num[comnmb][0] < c_max_ctcan_ser_array) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					ctc_ioclr[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					ctc_ioset[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#endif
			}
#endif
			break;
		case 31:			// MEM memory dump command = MEM type, page, offset, len,  MEM 0,0,1234,100 <CR> Page 0 for ram/ee, 1 for nvram, 2 = mram,3=RAM
			if ((procf < 3) || (procf > 11))
			{
				k = com_dbg_get_hex_numbers(comnmb, 3);
				if (k >= 3)
				{		// Must have at least 3 numbers
					if ((com_dbg_num[comnmb][0] >= 0) && (com_dbg_num[comnmb][0] <= 3))
					{
						com_dbg_mem_type = (uint16)com_dbg_num[comnmb][0];
						if ((com_dbg_num[comnmb][1] >= 0) && (com_dbg_num[comnmb][1] < 16))
						{
							com_dbg_mem_page = (uint16)(com_dbg_num[comnmb][1]);
							com_dbg_mem_offset = (uint16)com_dbg_num[comnmb][2];
							if (k == 4)
								com_dbg_mem_len = (uint16)com_dbg_num[comnmb][3];
							else
								com_dbg_mem_len = 40;
							com_dbg_mem_dump_screen(comnmb);
						}
					}
				}
			}
			else
			{
				sprintf(&dbg_com_buf[comnmb][0],"Not While Car Moving! \n\r>");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			}
			break;
		case 32:			// FIL fil memory command - FIL type, page, offset, len, data FIL 1,1,2000,100,255 <cr>
			if ((procf < 3) || (procf > 11))
			{
				k = com_dbg_get_hex_numbers(comnmb, 3);
				if (k >= 3)
				{		// Must have at least 3 numbers
					if ((com_dbg_num[comnmb][0] >= 0) && (com_dbg_num[comnmb][0] <= 2))
					{
						com_dbg_mem_type = (uint16)com_dbg_num[comnmb][0];
						if ((com_dbg_num[comnmb][1] >= 0) && (com_dbg_num[comnmb][1] < 16))
						{
							com_dbg_mem_page = (uint16)com_dbg_num[comnmb][1];
							com_dbg_mem_offset = (uint16)com_dbg_num[comnmb][2];
							if (k >= 4)
								com_dbg_mem_len = (uint16)com_dbg_num[comnmb][3];
							else
								com_dbg_mem_len = 40;
							if (k == 5) 
							{
								if (com_dbg_num[comnmb][4] > 255)
									com_dbg_num[comnmb][4] = 255;
							}
							else
								com_dbg_num[comnmb][4] = 0;
							j = 0;
							for(i=0;i<com_dbg_mem_len;i+=2)
							{
								if (com_dbg_num[comnmb][4] == 254)
								{
									wr_mram(com_dbg_mem_page,(uint16)(com_dbg_mem_offset + i),(uint16)(mem_pattern[j & 0x07]));
									j++;
								}
								else
									wr_mram(com_dbg_mem_page,(uint16)(com_dbg_mem_offset + i),(uint16)(com_dbg_num[comnmb][4]));
							}
						}
					}
				}
			}
			else
			{
				sprintf(&dbg_com_buf[comnmb][0],"Not While Car Moving! \n\r>");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			}
			break;
		case 33:			// INP - Input from i/o address INP 255<CR>
/*			k = com_dbg_get_hex_numbers(comnmb, 3);			 // num[0] = brd_io number, num[2] = bit
			if (k == 1 )
			{			  // must have 1 values
				dpy_io_data = inp((uint16)com_dbg_num[comnmb][0]);
				sprintf(&dbg_com_buf[comnmb][0],"%04xh,",dpy_io_data);
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				sendc(comnmb,'>');
			}
			break;
		case 34:			// OUT - Output to i/o address OUT 124,255<cr>
			k = com_dbg_get_hex_numbers(comnmb, 3);			 // num[0] = accress, num[1] = data
			if (k == 2 )
			{			  // must have two values
				outp((uint8)com_dbg_num[comnmb][0],(uint8)com_dbg_num[comnmb][1]);
			}
*/			
			break;

		case 35:		// FDL - 1 = 0PEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = com_dbg_get_num(comnmb, 3);
			sim_dl_open = 0;
			sim_dl_close = 0;
			if (i == 1)
				sim_dl_open = 1;
			if (i == 2)
				sim_dl_close = 1;
#endif
			break;
		case 36:		// FGS - 1 = OPEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = com_dbg_get_num(comnmb, 3);
			sim_gs_open = 0;
			sim_gs_close = 0;
			if (i == 1)
				sim_gs_open = 1;
			if (i == 2)
				sim_gs_close = 1;
#endif
			break;
		case 37:		// RDL - 1 = 0PEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = com_dbg_get_num(comnmb, 3);
			sim_dlr_open = 0;
			sim_dlr_close = 0;
			if (i == 1)
				sim_dlr_open = 1;
			if (i == 2)
				sim_dlr_close = 1;
#endif
			break;
		case 38:		// RGS - 1 = OPEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = com_dbg_get_num(comnmb, 3);
			sim_gsr_open = 0;
			sim_gsr_close = 0;
			if (i == 1)
				sim_gsr_open = 1;
			if (i == 2)
				sim_gsr_close = 1;
#endif
			break;
		case 39:		// FDC - 1 = 0PEN, 2 = CLOSE, 0 = CLEAR
#if (Simulator == 1)
			i = com_dbg_get_num(comnmb, 3);
			sim_dc_open = 0;
			sim_dc_close = 0;
			if (i == 1)
				sim_dc_open = 1;
			if (i == 2)
				sim_dc_close = 1;
#endif
			break;
		case 40:		// CMR: Can Message Request CMD dev,msg
#if (GALaxy_4 == 0)
			k = com_dbg_get_numbers(comnmb, 3);
			if (k >= 2)
			{
				if ((com_dbg_num[comnmb][0] >= c_can_START) && (com_dbg_num[comnmb][0] <= c_can_TOC) && (com_dbg_num[comnmb][1] != 0))
				{
					can_dev_req = (uint16)com_dbg_num[comnmb][0];
					can_msg_req = (uint16)com_dbg_num[comnmb][1];
					can_msg_cmd = 2;
					can_msg_cnt = 1;
				}
			}
			else if (k == 1)
			{
				if (com_dbg_num[comnmb][0] <= 49)
				{
					can_dev[c_can_SEB_18 - c_can_START].msg_req = (uint8)com_dbg_num[comnmb][0]; 
					if (com_dbg_num[comnmb][0] == 47)
					{
						can_dev[c_can_SEB_18 - c_can_START].label_ix = 0;
						can_dev[c_can_SEB_18 - c_can_START].label_req = 1;
					}
				}
			}
#endif
			break;
		case 41:			// IOT - IO Sim Data Toggle: IOT array0-78,bit=0-7 IOT 13,1: IOT tottles, 
#if (Simulator == 1)
			k = com_dbg_get_numbers(comnmb, 3);
	#if (GALaxy_4 == 0)
			if (k == 2)
			{			  // must have two values
				if ((com_dbg_num[comnmb][0] <= 80) && (com_dbg_num[comnmb][1] <= 7))
				{ 
					toggle_array = (int16)com_dbg_num[comnmb][0];
					toggle_bit = (int16)com_dbg_num[comnmb][1];
					toggle_state = 0;
				}
			}
			else
			{			
				if (toggle_state == 0)
				{
					toggle_state = 1;
					ioset[toggle_array] |= bittbl[toggle_bit & 0x07];
					ioclr[toggle_array] |= bittbl[toggle_bit & 0x07];
				}
				else
				{
					toggle_state = 0;
					ioclr[toggle_array] &= ~bittbl[toggle_bit & 0x07];
					ioset[toggle_array] &= ~bittbl[toggle_bit & 0x07];
				}
			}
	#else
			if (k == 2)
			{			  // must have two values
				if ((com_dbg_num[comnmb][0] <= 258) && (com_dbg_num[comnmb][1] <= 7))
				{ 
					toggle_array = (int16)com_dbg_num[comnmb][0];
					toggle_bit = (int16)com_dbg_num[comnmb][1];
					toggle_state = 0;
				}
			}
			else
			{			
				if (toggle_state == 0)
				{
					toggle_state = 1;
					if (toggle_array < 18)
					{
						ioset[toggle_array] |= bittbl[toggle_bit & 0x07];
						ioclr[toggle_array] |= bittbl[toggle_bit & 0x07];
					}
					else if (toggle_array < 138)
					{
						ctc_ioset[toggle_array-18] |= bittbl[toggle_bit & 0x07];
						ctc_ioclr[toggle_array-18] |= bittbl[toggle_bit & 0x07];
					}
					else if (toggle_array < 258)
					{
						mrc_ioset[toggle_array-138] |= bittbl[toggle_bit & 0x07];
						mrc_ioclr[toggle_array-138] |= bittbl[toggle_bit & 0x07];
					}
				}
				else
				{
					toggle_state = 0;
					if (toggle_array < 18)
					{
						ioclr[toggle_array] &= ~bittbl[toggle_bit & 0x07];
						ioset[toggle_array] &= ~bittbl[toggle_bit & 0x07];
					}
					else if (toggle_array < 138)
					{
						ctc_ioset[toggle_array-18] &= ~bittbl[toggle_bit & 0x07];
						ctc_ioclr[toggle_array-18] &= ~bittbl[toggle_bit & 0x07];
					}
					else if (toggle_array < 258)
					{
						mrc_ioset[toggle_array-138] &= ~bittbl[toggle_bit & 0x07];
						mrc_ioclr[toggle_array-138] &= ~bittbl[toggle_bit & 0x07];
					}
				}
			}
	#endif
#endif
			break;
		case 42:			// STI - STart Integer Memory Offset
			break;
		case 43:			// CIT - IO Sim Data Toggle: CIT array0-78,bit=0-7 IOT 13,1: CIT tottles, 
#if (Simulator == 1)
			k = com_dbg_get_numbers(comnmb, 3);
	#if (GALaxy_4 == 0)
			if (k == 2)
			{			  // must have two values
				if ((com_dbg_num[comnmb][0] < 80) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					toggle_array = (int16)(com_dbg_num[comnmb][0]);
					toggle_bit = (int16)(com_dbg_num[comnmb][1]);
					toggle_state = 0;
				}
			}
			else
			{			
				if (toggle_state == 0)
				{
					toggle_state = 1;
					cioset[toggle_array] |= bittbl[toggle_bit & 0x07];
					cioclr[toggle_array] |= bittbl[toggle_bit & 0x07];
				}
				else
				{
					toggle_state = 0;
					cioclr[toggle_array] &= ~bittbl[toggle_bit & 0x07];
					cioset[toggle_array] &= ~bittbl[toggle_bit & 0x07];
				}
			}
	#else
			if (k == 2)
			{			  // must have two values
				if ((com_dbg_num[comnmb][0] < c_max_ctcan_ser_array) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					toggle_array = (int16)(com_dbg_num[comnmb][0]+18);
					toggle_bit = (int16)(com_dbg_num[comnmb][1]+18);
					toggle_state = 0;
				}
			}
			else
			{			
				if ((toggle_array >= 18) && (toggle_array < 138))
				{
					if (toggle_state == 0)
					{
						toggle_state = 1;

						ctc_ioset[toggle_array-18] |= bittbl[toggle_bit & 0x07];
						ctc_ioclr[toggle_array-18] |= bittbl[toggle_bit & 0x07];
					}
					else
					{
						toggle_state = 0;
						ctc_ioclr[toggle_array-18] &= ~bittbl[toggle_bit & 0x07];
						ctc_ioset[toggle_array-18] &= ~bittbl[toggle_bit & 0x07];
					}
				}
			}
	#endif
#endif
			break;

#if (Simulator == 1)

			case 44:		// RPU  Reset Pulse Update Trace
#if(Traction == 1)
				for(i=0;i<fl_size;i++)
					DPP_updf[i] = 0;
#endif
			break;
#endif
	#if (GALaxy_4 == 0)

		case 45:			// FDP - Front Door Parameter Request: FDP 1343<ENT> OR FDP 1343,1333<ENT>
			k = com_dbg_get_hex_numbers(comnmb, 3);
			if (k != 0)
			{
				DOOR_can.par_add_req = (uint16) com_dbg_num[comnmb][0];
				can_dev[c_can_DOOR - c_can_START].msg_req = 30;
				if (k == 2)
				{
					DOOR_can.par_cmd |= b_door_par_wr;
					DOOR_can.par_dat_wrt = (uint16) com_dbg_num[comnmb][1];
				}
				else
				{
					DOOR_can.par_cmd &= ~b_door_par_wr;
					DOOR_can.par_dat_wrt = 0;
				}
			}
			break;

		case 46:			// RDP - Rear Door Parameter Request: RDP 1343<ENT> OR rDP 1343,1333<ENT>
			k = com_dbg_get_hex_numbers(comnmb, 3);
			if (k != 0)
			{
				RDOOR_can.par_add_req = (uint16)com_dbg_num[comnmb][0];
				can_dev[c_can_RDOOR - c_can_START].msg_req = 30;

				if (k == 2)
				{
					RDOOR_can.par_cmd |= b_door_par_wr;
					RDOOR_can.par_dat_wrt = (uint16)com_dbg_num[comnmb][1];
				}
				else
				{
					RDOOR_can.par_cmd &= ~b_door_par_wr;
					RDOOR_can.par_dat_wrt = 0;
				}
			}
			break;
		case 47:			// FDV - Front door version: FDV 1,1 <ENTER>
			k = com_dbg_get_hex_numbers(comnmb, 3);
			if (k != 0)
			{
				if (k == 2)
				{
				 	can_dev[c_can_DOOR-c_can_START].version = (uint8)com_dbg_num[comnmb][0];
					can_dev[c_can_DOOR-c_can_START].revision = (uint8)com_dbg_num[comnmb][1];
				}
				else
				{
				 	can_dev[c_can_DOOR-c_can_START].version = (uint8)com_dbg_num[comnmb][0];
					can_dev[c_can_DOOR-c_can_START].revision = 0;
				}
			}
			break;
		case 48:			// RDV - Front door version: RDV 1,1 <ENTER>
			k = com_dbg_get_hex_numbers(comnmb, 3);
			if (k != 0)
			{
				if (k == 2)
				{
				 	can_dev[c_can_RDOOR-c_can_START].version = (uint8)com_dbg_num[comnmb][0];
					can_dev[c_can_RDOOR-c_can_START].revision = (uint8)com_dbg_num[comnmb][1];
				}
				else
				{
				 	can_dev[c_can_RDOOR-c_can_START].version = (uint8)com_dbg_num[comnmb][0];
					can_dev[c_can_RDOOR-c_can_START].revision = 0;
				}
			}
			break;
		case 49:			// FDS - Front Door STATUSt: FDS 1343<ENT> 
			k = com_dbg_get_hex_numbers(comnmb, 3);
			if (k != 0)
			{
				DOOR_can.status_ret = (uint8)com_dbg_num[comnmb][0];
				DOOR_can.reflect_data = 1;
			}
			break;
#endif
		case 50:			// DVP - Drive Parameter Request: DVP 1343<ENT> OR DVP 1343,1333<ENT>
#if ((Traction == 1) || (Tract_OL == 1))
/*			k = com_dbg_get_numbers(comnmb, 3);			delete KEB parameters
			if (((Drive_Type[cons[carnmb]] == 12) && (fvars[fvkebdpytype] == 0)) || (Drive_Type[cons[carnmb]] == 14))
			{
				if ((com_dbg_num[comnmb][0] <= grp_par_max[drv_par_group]) && (drv_par_group >= 0) && (drv_par_group <= 13))
				{
					if (Drive_Type[cons[carnmb]] == 12)
					{
						if (k == 2)
						{		 // Write drive parameter
							drv_wpar_data = com_dbg_num[comnmb][1];
							drv_wpar_addr = keb_drv_par_tbl[drv_par_group].par[com_dbg_num[comnmb][0] & 0x00ff].address;
							drv_wpar_set = 	keb_drv_par_tbl[drv_par_group].par[com_dbg_num[comnmb][0] & 0x00ff].set;
							put_pkt_req('g',2,DRVCOM);
						}
						else
						{	  // Read drive parameter
							tst_drv_par_ret = com_dbg_num[comnmb][0];
							drv_rpar_addr = keb_drv_par_tbl[drv_par_group].par[com_dbg_num[comnmb][0] & 0x00ff].address;;
							drv_rpar_set = 	keb_drv_par_tbl[drv_par_group].par[com_dbg_num[comnmb][0] & 0x00ff].set;
							put_pkt_req('G',2,DRVCOM);
						}
					}
					else if (Drive_Type[cons[carnmb]] == 14)
					{
						if (k == 2)
						{		 // Write drive parameter
							uwu.b[1] = (uint8)drv_par_group;
							uwu.b[0] = ((uint8)com_dbg_num[comnmb][0] & 0x00ff);
							drv_wpar_addr = uwu.w;
							drv_wpar_data = com_dbg_num[comnmb][1];
							put_pkt_req(6,2,DRVCOM);
						}
						else
						{	  // Read drive parameter
							uwu.b[1] = (uint8)drv_par_group;
							uwu.b[0] = ((uint8)com_dbg_num[comnmb][0] & 0x00ff);
							drv_rpar_addr = uwu.w;
							put_pkt_req(3,2,DRVCOM);
						}
					}
				}
			}
			else if (Drive_Type[cons[carnmb]] == 15)
			{
				if (com_dbg_num[comnmb][0] <= 0x1000)
				{
					if (k == 2)
					{		 // Write drive parameter
						drv_wpar_addr = (uint16)com_dbg_num[comnmb][0];
						drv_wpar_data = (uint16)com_dbg_num[comnmb][1];
						put_pkt_req(6,2,DRVCOM);
					}
					else
					{	  // Read drive parameter
						drv_rpar_addr = (uint16)com_dbg_num[comnmb][0];
						put_pkt_req(3,2,DRVCOM);
					}
				}
			}
			else 
			{
				if (com_dbg_num[comnmb][0] <= 204)
				{
					if (k == 2)
					{		 // Write drive parameter
						uwu.b[1] = 0;
						uwu.b[0] = ((uint8)com_dbg_num[comnmb][0] & 0x00ff);
						drv_wpar_addr = uwu.w;
						drv_wpar_data = com_dbg_num[comnmb][1];
						put_pkt_req(0x43,2,DRVCOM);
					}
					else
					{	  // Read drive parameter
						uwu.b[1] = 0;
						uwu.b[0] = ((uint8)com_dbg_num[comnmb][0] & 0x00ff);
						drv_rpar_addr = uwu.w;
						put_pkt_req(0x61,2,DRVCOM);
					}
				}
			}
*/
#endif
			break;
		case 51:			// RAV = reset all field variables to defaults
#if (Simulator == 1)
			reset_fvars();
#endif
			break;
		case 52:		// INI - Simulator hoistway initialization
			i = com_dbg_get_num(comnmb, 3);
#if (Simulator == 1) 
			Reset_Hoistway_Init();
			Init_Hoistway();
			Hoistway_Learned = 1;
			Wrt_Hoistway();
			sim_init(0);
			
	#if ((Traction == 1) || (Tract_OL == 1))
		// This has to be done here because sim_init will cause a gripper fault when GTS goes off
			if (cons[ansi] >= 2000)
			{
				if (gripper_fault != 0)
				{
					clroutp(o_GR1R);
					clroutp(o_GR2R);
					clroutp(o_LE);
					clroutp(o_LE1);
				}
				else
				{
					// Pick the rope gripper
					setoutp(o_GR1R);
					setoutp(o_GR2R);
					gripper_test = 0;
				}
			}
			else
			{
				setoutp(o_GR1R);
				setoutp(o_GR2R);
				gripper_test = 0;
			}
			if ((rdoutp(o_GR1R) == 1) || 
				((rdoutp(o_LE) == 1) && (rdoutp(o_LE1) == 1) && (rdoutp(o_GR2R) == 1)))
				setinp(i_GRT1);
			else
				clrinp(i_GRT1);

			if ((rdoutp(o_LE) == 1) ||
			    ((rdoutp(o_GR1R) == 1) && (rdoutp(o_LE1) == 1) && (rdoutp(o_GR2R) == 1)))
				setinp(i_GRT2);
			else
				clrinp(i_GRT2);

			if ((rdoutp(o_GR1R) == 1) && (rdoutp(o_GR2R) == 1))
				setinp(i_GTS);
			else
				clrinp(i_GTS);
  	#endif		
#endif
			if ((i < 3) || (i > 10))
				i = 0;
			init_flight_times((int16)i);
			clear_call_data();
			break;

		case 53:			// PPI = Pulses per inch
			sprintf(&dbg_com_buf[comnmb][0],"Pulses per Inch = %5.1f \n\r",Pulses_per_Inch);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,CR);
			sendc(comnmb,LF);
			sendc(comnmb,'>');
			break;

		case 54:			// WVD = Write VGA Data (Address,Value)
			break;

		case 55:			// RVR = Read VGA Register (Address)
			break;

		case 56:			// WVR = Write VGA Register (Address,Value)
			break;

		case 57:			// RKB = Read Keyboard
			break;

		case 58:			// WKB = Write Keyboard (value)
			break;

		case 59:		// KBM - Special Keyboard Test Mode
			break;
			
		case 60:		// KBE - Keyboard Test Mode Exit
			break;
			
		case 61:		// STR - Service Timer Reset
			Reset_Service_Timers();
			break;
/*			
		case 62:		// DBG - Video Debug command
			i = com_dbg_get_num(comnmb,3);
			if (i == 1)
				dbg_page = 1;
			else if (i == 2)
				dbg_page = 2;
#if(Traction == 1)
			else if (i == 3)
				dbg_page = 3;
#endif
			else if (i == 4)
				dbg_page = 4;
			else if (i == 5)
				dbg_page = 5;
			else if (i == 6)
				dbg_page = 6;
			else if (i == 7)
				dbg_page = 7;
			else if (i == 8)
				dbg_page = 8;
			else
				dbg_page = 0;

			dpy_mode = 4;
			Init_Debug_Screen(1);
			break;
*/			
		case 63:		// RLF = Reset Long Term Faults
			clr_LT_flts();
			break;
			
		case 64:			// MIS - CAN IO Sim Data Set: MIS brd no, bit   MIS 19,2<CR> sets bit
#if (Simulator == 1)						   // Look up brd no  
			k = com_dbg_get_numbers(comnmb, 3);
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if ((com_dbg_num[comnmb][0] < c_max_mrcan_ser_array) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					mrc_ioset[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					mrc_ioclr[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#endif
			}
#endif			
			break;
			
		case 65:			// MIC - CAN IO Sim Data Clear: MIC brd no, bit   MIC 19,2<CR> sets bit
#if (Simulator == 1)					  // Look up brd no 
			k = com_dbg_get_numbers(comnmb, 3);
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if ((com_dbg_num[comnmb][0] < c_max_mrcan_ser_array) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					mrc_ioclr[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					mrc_ioset[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#endif
			}
			
#endif
			break;
		case 66:			// MIT - IO Sim Data Toggle: MIT array0-78,bit=0-7 IOT 13,1: MIT toggles, 
#if ((Simulator == 1) && (GALaxy_4 == 1))
			k = com_dbg_get_numbers(comnmb, 3);
			if (k == 2)
			{			  // must have two values
				if ((com_dbg_num[comnmb][0] < c_max_mrcan_ser_array) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					toggle_array = (int16)(com_dbg_num[comnmb][0]+138);
					toggle_bit = (int16)(com_dbg_num[comnmb][1]+138);
					toggle_state = 0;
				}
			}
			else
			{			
				if ((toggle_array >= 138) && (toggle_array < 258))
				{
					if (toggle_state == 0)
					{
						toggle_state = 1;

						mrc_ioset[toggle_array-138] |= bittbl[toggle_bit & 0x07];
						mrc_ioclr[toggle_array-138] |= bittbl[toggle_bit & 0x07];
					}
					else
					{
						toggle_state = 0;
						mrc_ioclr[toggle_array-138] &= ~bittbl[toggle_bit & 0x07];
						mrc_ioset[toggle_array-138] &= ~bittbl[toggle_bit & 0x07];
					}
				}
			}
#endif
			break;
		case 67:			// TRA - TRAce: TRA 0,VEL,7453,B
			len = com_dbg_rmbuf_len[comnmb];
		    HexInput = false;		// set default input type as decimal
		    FirstNonSpace = com_dbg_LocNonSpace(comnmb, len); // Locate first NonSpace
			if ( (rmbuf[comnmb][FirstNonSpace+1] == ',') &&
				(rmbuf[comnmb][FirstNonSpace] >= '0') && (rmbuf[comnmb][FirstNonSpace] <= '9'))
			{		// valid so far
				// Get data name
				j = 0; 
				com_dbg_trace_num = (int16)(rmbuf[comnmb][FirstNonSpace] - '0');
				seq = 0;
				for (i=FirstNonSpace+2;i<=len;i++)  
				{		// Decode the input string
					if (seq == 0)
					{	   // Get data variable name
						if (rmbuf[comnmb][i] == ',')
						{
							while (j<10)
							{
								trace_name[com_dbg_trace_num][j] =' ';
								j++;
							}
							trace_name[com_dbg_trace_num][j] = 0;	// Set last byte to zero
							j = 0;
							seq = 1;		// Have the name so get the address seg
						} 
						else if (i<FirstNonSpace+12)	   // max of ten characters long
						{
							trace_name[com_dbg_trace_num][j] = rmbuf[comnmb][i];
							j++;
						}
					}
					else if (seq == 1)
					{
						if (rmbuf[comnmb][i] == CR)
						{
							t_off[j] = 0;	// Set last byte to zero
							j = 0;
							seq = 4;
							trace_format[com_dbg_trace_num] = 'D';
						}
						else if (rmbuf[comnmb][i] == ',')
						{
							t_off[j] = 0;
							j = 0;
							seq = 2;
						}
						else
						{
							t_off[j] = rmbuf[comnmb][i];
							j++;
						}
					}
					else if (seq == 2)
					{
						rmbuf[comnmb][i] &= 0xdf;
						if ( (rmbuf[comnmb][i] == 'D') || (rmbuf[comnmb][i] == 'S') || 
						     (rmbuf[comnmb][i] == 'X') || (rmbuf[comnmb][i] == 'U') || 
						     (rmbuf[comnmb][i] == 'C') || (rmbuf[comnmb][i] == 'E') || 
						     (rmbuf[comnmb][i] == 'F') || (rmbuf[comnmb][i] == 'L') ||	
						     (rmbuf[comnmb][i] == 'H') || (rmbuf[comnmb][i] == 'B') || 
						     (rmbuf[comnmb][i] == 'W') || (rmbuf[comnmb][i] == 'I') )
							 trace_format[com_dbg_trace_num] = rmbuf[comnmb][i];
						else
							trace_format[com_dbg_trace_num] = 'D';
						set_mask(com_dbg_trace_num);
						seq = 3;
					}
                  	else if (seq==3)
					{
						if (rmbuf[comnmb][i] == CR)
						{
							seq = 5;
							break;
						}
						else if (rmbuf[comnmb][i] == ',')
						{
							seq = 4;
							j = 0;
						}
					}
					else if (seq == 4)
					{
						if (rmbuf[comnmb][i] == CR)
						{
							d_off[j] = 0;	// Set last byte to zero
							j = 0;
							seq = 5;
							break;
						}
						else if(rmbuf[comnmb][i] =='H')
						   HexInput = true;
						else
						{
							set_var = 1;
							d_off[j] = rmbuf[comnmb][i];
							j++;
						}
					}
				}
					  
				if (seq == 5)
				{
					trace_offset[com_dbg_trace_num] = atoh(t_off);
					trace_ptr[com_dbg_trace_num] = (uint32 *)(atoh(t_off) + com_dbg_var_off);
				   
					trace_status[com_dbg_trace_num] = 1;

				   // Accept Hex  or Integer  input 
					if (set_var == 1)
					{
						set_var = 0;	
						if(HexInput == true)
					    	com_dbg_variable[com_dbg_trace_num] = atoh(d_off);
					   	else 
					    	com_dbg_variable[com_dbg_trace_num] = atoi(d_off);
					   
						
				    	if ((trace_format[com_dbg_trace_num] == 'B') || (trace_format[com_dbg_trace_num] == 'C') || (trace_format[com_dbg_trace_num] == 'H'))
				    	{
							vptr8 = (int8 *)trace_ptr[com_dbg_trace_num];
				        	*vptr8 = (uint8)com_dbg_variable[com_dbg_trace_num];       // write the byte variable to an integer 
				    	}
				    	else if ((trace_format[com_dbg_trace_num] == 'W') || (trace_format[com_dbg_trace_num] == 'S'))
				    	{
							vptr16 = (int16 *)trace_ptr[com_dbg_trace_num];
				    		*vptr16 = (uint16)com_dbg_variable[com_dbg_trace_num];      // write word variable (int16 is defined as a short int)
				    	}
				    	else
				        {
							vptr32 = (int32 *)trace_ptr[com_dbg_trace_num];
				        	*vptr32 = (uint32)com_dbg_variable[com_dbg_trace_num];        // write the integer variable to an integer 
				        }
					}
					sprintf(&dbg_com_buf[comnmb][0],"%.8s",trace_name[com_dbg_trace_num]);
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					sprintf(&dbg_com_buf[comnmb][0],",%08xh",(uint32)trace_ptr[com_dbg_trace_num]);
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					tr_data[com_dbg_trace_num] = ((uint32) *trace_ptr[com_dbg_trace_num]) & trace_mask[com_dbg_trace_num];
					sprintf(&dbg_com_buf[comnmb][0]," = %08xh",tr_data[com_dbg_trace_num]);
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					sendc(comnmb,CR);
					sendc(comnmb,LF);
					sendc(comnmb,'>');
				}
			}
			break;
		case 68:		// TRC - Clear trace variables
			if ((procf < 3) || (procf > 11))
			{
				if ((((rmbuf[comnmb][3] & 0xDF) == 'A') && (rmbuf[comnmb][4] == CR)) ||
				   ((rmbuf[comnmb][3] == ' ') && (rmbuf[comnmb][5] == CR))) 
				{
					if((rmbuf[comnmb][4] >= '0') && (rmbuf[comnmb][4] <= '5'))
					{		// valid so far
						com_dbg_trace_num = (int16)(rmbuf[comnmb][4] - '0');
						trace_record_fault[com_dbg_trace_num] = 0;
						trace_status[com_dbg_trace_num] = 0;
						trace_mask[com_dbg_trace_num] = 0;
						tr_data[com_dbg_trace_num] = 0;
						for (i=0;i<max_trace_size;i++)
							Trace_Data[i][com_dbg_trace_num] = 0;
					}
					else if ((rmbuf[comnmb][3] & 0xDF) == 'A')
					{
						for (j=0;j<=5;j++)
						{
 							trace_record_fault[j] = 0;
							trace_status[j] = 0;
							trace_mask[j] = 0;
							tr_data[j] = 0;
							for (i=0;i<max_trace_size;i++)
								Trace_Data[i][j] = 0;
						}
					}
				
					Trace_gix = 0;
					Trace_pix = 0;
					trace_complete = 0;
					run_trace = 0;
					trace_auto_start = 0;
					Update_Control_Flags();
				}
			}
			else
			{
				sprintf(&dbg_com_buf[comnmb][0],"Not While Car Moving! \n\r>");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			}
			break;
			
			
		case 69:		// TRD = Trace Dump
			if ((procf < 3) || (procf > 11))
			{
				k = com_dbg_get_numbers(comnmb, 3);
				if (k == 1)
					Trace_gix = com_dbg_num[comnmb][0];
				
				com_dbg_trace_dump_screen(comnmb);
				if (Trace_gix >= (max_trace_size - 1))
					Trace_gix = 0;
			}
			else
			{
				sprintf(&dbg_com_buf[comnmb][0],"Not While Car Moving! \n\r>");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			}
			break;
		
		case 70:		// TRS - Trace Start
			if (run_trace == 1)
			{
				sprintf(&dbg_com_buf[comnmb][0],"Trace Already Running! TRE to stop. \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,'>');
			}
			else
			{
				sprintf(&dbg_com_buf[comnmb][0],"Trace Started \n\r");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				sendc(comnmb,'>');
				start_debug_trace();
			}
			break;
			
		case 71:		// TRE - Trace Ended
			sprintf(&dbg_com_buf[comnmb][0],"Trace Ended \n\r");
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,'>');
			run_trace = 0;
			break;
			
		case 72:		// TRP - Traced Preset
		
#if (Traction == 1)
			init_debug_trace();
			trace_auto_start = 1;
			Update_Control_Flags();

			sprintf(&dbg_com_buf[comnmb][0],"%.8s",trace_name[0]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",(uint32)trace_ptr[0]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			tr_data[com_dbg_trace_num] = ((uint32) *trace_ptr[0]) & trace_mask[0];
			sprintf(&dbg_com_buf[comnmb][0]," = %08xh",tr_data[0]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,CR);
			sendc(comnmb,LF);
			sendc(comnmb,'>');

			sprintf(&dbg_com_buf[comnmb][0],"%.8s",trace_name[1]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",(uint32)trace_ptr[1]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			tr_data[com_dbg_trace_num] = ((uint32) *trace_ptr[1]) & trace_mask[1];
			sprintf(&dbg_com_buf[comnmb][0]," = %08xh",tr_data[1]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,CR);
			sendc(comnmb,LF);
			sendc(comnmb,'>');

			sprintf(&dbg_com_buf[comnmb][0],"%.8s",trace_name[2]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",(uint32)trace_ptr[2]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			tr_data[com_dbg_trace_num] = ((uint32) *trace_ptr[2]) & trace_mask[2];
			sprintf(&dbg_com_buf[comnmb][0]," = %08xh",tr_data[2]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,CR);
			sendc(comnmb,LF);
			sendc(comnmb,'>');

			sprintf(&dbg_com_buf[comnmb][0],"%.8s",trace_name[3]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",(uint32)trace_ptr[3]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			tr_data[com_dbg_trace_num] = ((uint32) *trace_ptr[3]) & trace_mask[3];
			sprintf(&dbg_com_buf[comnmb][0]," = %08xh",tr_data[3]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,CR);
			sendc(comnmb,LF);
			sendc(comnmb,'>');
			
			sprintf(&dbg_com_buf[comnmb][0],"Trace Started \n\r");
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,'>');
			
			start_debug_trace();
#endif
			break;

			
		case 73:		// TRF - Record fault for trace variables
			if ((procf < 3) || (procf > 11))
			{
				if ((((rmbuf[comnmb][3] & 0xDF) == 'A') && (rmbuf[comnmb][4] == CR)) ||
				   (((rmbuf[comnmb][3] & 0xDF) == ' ') && (rmbuf[comnmb][5] == CR))) 
				{
					if((rmbuf[comnmb][4] >= '0') && (rmbuf[comnmb][4] <= '3'))
					{		// valid so far
						com_dbg_trace_num = (int16)(rmbuf[comnmb][4] - '0');
 						trace_record_fault[com_dbg_trace_num] = 1;
					}
					else if ((rmbuf[comnmb][3] & 0xDF) == 'A')
					{
						for (j=0;j<=3;j++)
						{
 							trace_record_fault[j] = 1;
						}
					}
				}
				else if (((rmbuf[comnmb][3] & 0xDF) == 'C') && (rmbuf[comnmb][4] == CR))
				{
					for (j=0;j<=3;j++)
					{
						trace_record_fault[j] = 0;
					}
				}
			}
			else
			{
				sprintf(&dbg_com_buf[comnmb][0],"Not While Car Moving! \n\r>");
				sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			}
			break;
			
		case 74:		// TIM - System timming
			if ((((rmbuf[comnmb][3] & 0xDF) == 'C') && (rmbuf[comnmb][4] == CR)) ||
			   ((rmbuf[comnmb][3] == ' ') && ((rmbuf[comnmb][4] & 0xDF) == 'C')  && (rmbuf[comnmb][5] == CR))) 
			{
				inctim_min = 0x7FFFFFFF;
				inctim_max = 0;
				proctim_min = 0x7FFFFFFF;
				proctim_max = 0;
				sendc(comnmb,CR);
				sendc(comnmb,LF);
				alt_tim_data = 0;
			}
			else
			{
				if (alt_tim_data == 0)
				{
					alt_tim_data = 1;	
					temp = inctim_min/10875;
					sprintf(&dbg_com_buf[comnmb][0],"IncTime Min = %.2d.%.1d msec \n\r",(temp/10),(temp%10));
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					temp = inctim_max/10875;
					sprintf(&dbg_com_buf[comnmb][0],"IncTime Max = %.2d.%.1d msec \n\r",(temp/10),(temp%10));
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					temp = inctim_avg/10875;
					sprintf(&dbg_com_buf[comnmb][0],"IncTime Avg = %.2d.%.1d msec \n\r",(temp/10),(temp%10));
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				}
				else
				{
					alt_tim_data = 0;	
					temp = proctim_min/10875;
					sprintf(&dbg_com_buf[comnmb][0],"ProcTime Min = %.2d.%.1d msec \n\r",(temp/10),(temp%10));
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					temp = proctim_max/10875;
					sprintf(&dbg_com_buf[comnmb][0],"ProcTime Max = %.2d.%.1d msec \n\r",(temp/10),(temp%10));
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
					temp = proctim_avg/10875;
					sprintf(&dbg_com_buf[comnmb][0],"ProcTime Avg = %.2d.%.1d msec \n\r",(temp/10),(temp%10));
					sendstring(comnmb,&dbg_com_buf[comnmb][0]);
				}
			}
			sendc(comnmb,'>');
			break;
			
		case 75:		// GCP - Galcom packet  GCP packet number
			i = com_dbg_get_num(comnmb, 3);
			if ((i != 0) && (i < 100))
			{
				if (comnmb == USR2COM)
					put_pkt_req((uint16)i,cons[carnmb],USR1COM);
				else
					put_pkt_req((uint16)i,cons[carnmb],USR2COM);
			}
			break;
			
		case 76:		// GCR - Galcom packet  GCR packet number
			k = com_dbg_get_numbers(comnmb, 3);
			if(k != 0)
				i = com_dbg_num[comnmb][0];
			//i = com_dbg_get_num(comnmb, 3);
			if ((i != 0) && (i < 100))
			{
				if((i == 20) && (k == 2))
				{
					com_fvar_block_ix[comnmb] = (short int) com_dbg_num[comnmb][1];
					if(com_fvar_block_ix[comnmb] >= 5)
						com_fvar_block_ix[comnmb] = 0;
				}
				galcom_pack(USR1COM,(uint16)i);
			}
			break;		
			
		case 77:		//MUT mute or unmute echo on port
			i = com_dbg_get_num(comnmb, 3);
			if(i == 0)
			{
				mute_echo_on_port = 0;
			}
			else
			{
				mute_echo_on_port = 1;
			}
			break;
			
		case 78:		// 	"AVW"	Adjustable Variable Write
			k = com_dbg_get_numbers(comnmb, 3);
			
			if (k == 2)
			{
				fvars[com_dbg_num[comnmb][0]] = (short int)com_dbg_num[comnmb][1];
				wrfvar();
			}

		case 79:		// Set Fault for testing - FLT n sets fault number n; if n = 0 then set 50 faults.
			i = com_dbg_get_num(comnmb, 3);
			if (i == 0)
			{		// Run Fault Test
				i = 1;
			  	while(i <= max_flt_ix)
			  	{
					record_fault((int16)i++);
			  	}
			}
			else
			{
				if(i <= max_nmbflts)
				{
					if (i == f_MRCAN_device_fault) 
					{
						fault_device = c_mrcan_SPB;
						fault_prog_flag1 = 3; 	  // UTS speed fault
						fault_prog_flag2 = 1; 	  // 1 occurance
						
					}
					else if (i == f_CTCAN_device_fault) 
					{
						fault_device = c_ctcan_SEL;
						fault_prog_flag1 = 49; 	  // UN pulse fault
						fault_prog_flag2 = 8; 	  // 8 occurance
					}
					else if (i == f_GRCAN_device_fault)
					{
						fault_device = c_grcan_SEB_1;
						fault_prog_flag1 = 13;		// comm fault 
						fault_prog_flag2 = 3; 	  // 3 occurance
					}
					else if (i == f_MRCAN_device_reset)
					{
						fault_device = c_mrcan_NTS;
						fault_prog_flag1 = 0; 	  
						fault_prog_flag2 = 0; 	  
					}
					else if (i == f_CTCAN_device_reset)
					{
						fault_device = c_ctcan_SEL;
						fault_prog_flag1 = 0; 	  
						fault_prog_flag2 = 0; 	  
					}
					else if (i == f_GRCAN_device_reset)
					{
						fault_device = c_grcan_SEB_1;
						fault_prog_flag1 = 0;		 
						fault_prog_flag2 = 0; 	  
					}
					else
					{
						fault_device = 0;
						fault_prog_flag1 = 0;	
						fault_prog_flag2 = 0; 
					}
					record_fault((int16)i);
				}
			}

			break;	
		case 80:			// GIS - Group IO Sim Data Set: MIS brd no, bit   MIS 19,2<CR> sets bit
#if (Simulator == 1)						   // Look up brd no  
			k = com_dbg_get_numbers(comnmb, 3);
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if ((com_dbg_num[comnmb][0] < c_max_grcan_ser_array) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					grc_ioset[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
					grc_ioclr[com_dbg_num[comnmb][0]] |= bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#endif
			}
#endif			
			break;
			
		case 81:			// GIC - Group IO Sim Data Clear: MIC brd no, bit   MIC 19,2<CR> sets bit
#if (Simulator == 1)					  // Look up brd no 
			k = com_dbg_get_numbers(comnmb, 3);
			if (k == 2 )
			{			  // must have two values
	#if (GALaxy_4 == 1)
				if ((com_dbg_num[comnmb][0] < c_max_grcan_ser_array) && (com_dbg_num[comnmb][1] <= 7))
				{	 // valid device numbers
					grc_ioclr[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
					grc_ioset[com_dbg_num[comnmb][0]] &= ~bittbl[com_dbg_num[comnmb][1] & 0x07];
				}
	#endif
			}
#endif
			break;

		case 82:		// 	"SIM"	Simulator Status
#if (Simulator == 1)					  // Look up brd no 
			sprintf(&dbg_com_buf[comnmb][0],"Simulator Status \n\r");
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],"Car Pos = %.2i, SimPos = %.2i \n\r",position,sim_pos);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],"Car Dpp = %8i Sim Dpp = %8i \n\r",DPP_Count,sim_DPP_Count);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],"Sim Valid Floor = %.2i  \n\r",valid_fl[sim_pos]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,CR);
			sendc(comnmb,LF);
			sendc(comnmb,'>');
#endif
			break;
			
		case 83:		// DBV - Dump debug variables 1-8
			sprintf(&dbg_com_buf[comnmb][0],"Debug1 = %4x, Debug2 = %4x \n\r",debug1,debug2);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],"Debug3 = %4x, Debug4 = %4x \n\r",debug3,debug4);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],"Debug5 = %4x, Debug6 = %4x \n\r",debug5,debug6);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sprintf(&dbg_com_buf[comnmb][0],"Debug7 = %4x, Debug8 = %4x \n\r",debug7,debug8);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
			sendc(comnmb,CR);
			sendc(comnmb,LF);
			sendc(comnmb,'>');
			break;

		default:
			break;
	}
}

// ********************************************************
// Sends data from dbg_com_buf [] out on the specified com 
// channel. Use 'DockLite' to monitor the data dump
// ********************************************************

void com_dbg_wbg_dump(int16 comnmb)
{
    
}

// ********************************************************
// Clears out the dbg_com_buf [] for the  
// specified com (0,1,2,3)
// ********************************************************

void Clear_dbg_com_buf(int16 comnmb)
{
    
}

void com_dbg_mem_dump_screen(int16 comnmb)
{
  	int16 k;
	static int8 mem_ix;
	uint32 *ptr;
	
		
	if (com_dbg_mem_type == 0)
		sprintf(&dbg_com_buf[comnmb][0],"EERam Memory Dump \n\r");
	else if (com_dbg_mem_type == 1)
		sprintf(&dbg_com_buf[comnmb][0],"NVRam Memory Dump \n\r");
	else if (com_dbg_mem_type == 2)
		sprintf(&dbg_com_buf[comnmb][0],"MRam Memory Dump \n\r");
	else
		sprintf(&dbg_com_buf[comnmb][0],"Ram Memory Dump \n\r");

	sendstring(comnmb,&dbg_com_buf[comnmb][0]);

	k = 0;
	mem_ix = 0;
	
	
	while ((k<=300) && (Tranbuf_Full[comnmb] == 0))
	{
		if (com_dbg_mem_type == 2)
			sprintf(&dbg_com_buf[comnmb][0],"%04xh,",rd_mram(com_dbg_mem_page,(uint16)(com_dbg_mem_offset + mem_ix)));
		else if (com_dbg_mem_type == 3)
		{
			uiu.b[mhb] = (uint8)com_dbg_mem_page;
			uiu.b[hb] = 0x40;
			uiu.w[lw] = (uint16)(com_dbg_mem_offset + mem_ix);
			ptr = (uint32 (*))uiu.dw;
			sprintf(&dbg_com_buf[comnmb][0],"%08xh,",*ptr);
			mem_ix+=3;
			k+=3;
		}
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		k++; 
		mem_ix++;
		if (mem_ix >= com_dbg_mem_len)
			break;
	}
	sendc(comnmb,CR);
	sendc(comnmb,LF);
	sendc(comnmb,'>');
	
}


void com_dbg_trace_dump_screen(int16 comnmb)
{
  	int16 k;
	
		
	sprintf(&dbg_com_buf[comnmb][0],"Trace Memory Dump \n\r");
	sendstring(comnmb,&dbg_com_buf[comnmb][0]);
	sprintf(&dbg_com_buf[comnmb][0],"Index,Clock, Routine");
	sendstring(comnmb,&dbg_com_buf[comnmb][0]);
	if (trace_status[0] == 1)
	{
		sprintf(&dbg_com_buf[comnmb][0],",%.9s",trace_name[0]);
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
	}
	if (trace_status[1] == 1)
	{
		sprintf(&dbg_com_buf[comnmb][0],",%.9s",trace_name[1]);
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
	}
	if (trace_status[2] == 1)
	{
		sprintf(&dbg_com_buf[comnmb][0],",%.9s",trace_name[2]);
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
	}
	if (trace_status[3] == 1)
	{
		sprintf(&dbg_com_buf[comnmb][0],",%.9s",trace_name[3]);
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
	}
	if (trace_status[4] == 1)
	{
		sprintf(&dbg_com_buf[comnmb][0],",%.9s",trace_name[4]);
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
	}
	if (trace_status[5] == 1)
	{
		sprintf(&dbg_com_buf[comnmb][0],",%.9s",trace_name[5]);
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
	}
	sendc(comnmb,CR);
	sendc(comnmb,LF);

	k = 0;
	
	
	while ((k<20) && (Tranbuf_Full[comnmb] == 0))
	{
		sprintf(&dbg_com_buf[comnmb][0],"%04xh,%04xh,%s,",(uint16)Trace_gix,trace_clock[Trace_gix],trace_display[trace_routine[Trace_gix]]);
		sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		if (trace_status[0] == 1)
		{
			sprintf(&dbg_com_buf[comnmb][0],"%08xh",Trace_Data[Trace_gix][0]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		}
		if (trace_status[1] == 1)
		{
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",Trace_Data[Trace_gix][1]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		}
		if (trace_status[2] == 1)
		{
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",Trace_Data[Trace_gix][2]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		}
		if (trace_status[3] == 1)
		{
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",Trace_Data[Trace_gix][3]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		}
		if (trace_status[4] == 1)
		{
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",Trace_Data[Trace_gix][4]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		}
		if (trace_status[5] == 1)
		{
			sprintf(&dbg_com_buf[comnmb][0],",%08xh",Trace_Data[Trace_gix][5]);
			sendstring(comnmb,&dbg_com_buf[comnmb][0]);
		}
		sendc(comnmb,CR);
		sendc(comnmb,LF);
		k++; 
		if (Trace_gix < (max_trace_size - 1))
			Trace_gix++;
		else break;
	}
	sendc(comnmb,CR);
	sendc(comnmb,LF);
	sendc(comnmb,'>');
	
}




// ****************************************
// This routine places numbers in num array
// ****************************************

int com_dbg_get_numbers(int16 comnmb,int16 i)
{				// i is the buffer start
	int16 j,k;
	j = i;
	k = 0;
	do {
		i++;
		if ((rmbuf[comnmb][i] == ',') || (rmbuf[comnmb][i] == CR))
		{
			rmbuf[comnmb][i] = 0;
			com_dbg_num[comnmb][k++] = atoi((char *)&rmbuf[comnmb][j]);
			j = i + 1;
		}
	} while ((rmbuf[comnmb][i] != CR) && (i<=20));
	return (k);
}

int32 com_dbg_get_hex_numbers(int16 comnmb,int16 i)
{				// i is the buffer start
	int32 j,k;
	j = i;
	k = 0;
	do {
		i++;
		if ((rmbuf[comnmb][i] == ',') || (rmbuf[comnmb][i] == CR))
		{
			rmbuf[comnmb][i] = 0;
			com_dbg_num[comnmb][k++] = atoh((char *)&rmbuf[comnmb][j]);
			j = i + 1;
		}
	} while ((rmbuf[comnmb][i] != CR) && (i<=20));
	return (k);
}

int32 com_dbg_get_num(int16 comnmb,int16 buf_st)
{
	int32 i,j;
	i = 0xFFFF;

	for (j=buf_st;j<=20;j++)
	{
		if (rmbuf[comnmb][j] == CR)
		{
			rmbuf[comnmb][j] = 0;
			i = atoi((char *)&rmbuf[comnmb][buf_st]); 
			return i;
		}
	}
	return i;
}

// 42 -> 42
// 42 -> 84
// 42 -> 126
// 42 -> 168
// 42 -> 210
// 15 -> 225

#define com_dbg_cmdsize 252						// update table above when adding commands
const char com_dbg_cmdtbl[com_dbg_cmdsize] =  {"   STSCCICCCUCIUCCDCIDCCRCIRUIRDIRCCRUCRDC"
											   "CBICBCIUIIDIIUCIDCSUISDISUCSDCTSTIOSIOCRIO"
											   "RAICISCICMEMFILINPOUTFDLFGSRDLRGSFDCCMRIOT"
											   "STICITRPUFDPRDPFDVRDVFDSDVPRAVINIPPIWVDRVR"
											   "WVRRKBWKBKBMKBESTRDBGRLFMISMICMITTRATRCTRD"
											   "TRSTRETRPTRFTIMGCPGCRMUTAVWFLTGISGICSIMDBV"};

// *****************************************************
//   This  routine  finds the location for first Nonspace
// ******************************************************
int16 com_dbg_LocNonSpace(int16 comnmb, int16 len)
{
	int16 i;
	int16 Loc=4;
	for(i=3;i<len;i++)
	{
	    if((rmbuf[comnmb][i] | 0x20) !=' ')
		{
			Loc=i;
			break;
		}
    }
   return Loc;
}

int16 com_dbg_scan_cmd(int16 comnmb)
{
	int16 i,cmd;
	cmd = 0;
	for (i=0;i<(com_dbg_cmdsize-2);i+=3)
	{
		if (((rmbuf[comnmb][0] & 0xDF) == com_dbg_cmdtbl[i]) && 
		   ((rmbuf[comnmb][1] & 0xDF) == com_dbg_cmdtbl[i+1]) && 
		   ((rmbuf[comnmb][2] & 0xDF) == com_dbg_cmdtbl[i+2]))
		{		// Match upper or lower case (& 0xdf)
			cmd = i/3;
			break;
		}
	}
	return cmd;
}

/* Revision History
12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
				12/27/13 v6.1.24 mhd	1. Added TRF command to change trace_record_fault[] variable to select to record a fault or not when the trace occurs. 
2/12/15 V7.1.25 mhd		1. Integrated changes for test simulator.
8/18/15 V7.1.40 MHD		1. Added GIS and GIC commands to simulate group IOs.

*/