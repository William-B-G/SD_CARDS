// This file contains car call and hall call routines

#define d_CALLS 1
#include "global.h"


int32 c_up_hall_call[2] = {0,0};
int32 c_dn_hall_call[2] = {0,0};
int32 c_upr_hall_call[2] = {0,0};
int32 c_dnr_hall_call[2] = {0,0};
int32 c_cc_req[2] = {0,0};
int32 c_rcc_req[2] = {0,0};


int32 c_irup_hall_call[2] = {0,0};
int32 c_irdn_hall_call[2] = {0,0};
int32 c_rirup_hall_call[2] = {0,0};
int32 c_rirdn_hall_call[2] = {0,0};

int16 cancel_up = 0;
int16 cancel_dn = 0;
int16 cancel_upr = 0;
int16 cancel_dnr = 0;

int16 cancel_irup = 0;
int16 cancel_irdn = 0;
int16 cancel_rirup = 0;
int16 cancel_rirdn = 0;

int16 cancel_trup = 0;
int16 cancel_trdn = 0;
int16 cancel_rtrup = 0;
int16 cancel_rtrdn = 0;


// Car call test variables
int32 cc_test_mask[2] = {0,0};		// Car call test mask
int16 enable_cc_test;
int16 continue_cc_test;
int16 cancel_cc_test;
int16 sab_cc_topff = 0;
int16 sab_cc_topfr = 0; 
int16 sab_cc_botff = 0;
int16 sab_cc_botfr = 0; 
int16 sabbath_rst_msk = 0; 

int16 sabbath_auto_calls = 0;
int16 ccltim[fl_size];         // car call output timers
int16 rccltim[fl_size];         // rear car call output timers


void sabbath_std_svc(void);
void sabbath_down_col_svc(void);
void sabbath_up_col_svc(void);
void sabbath_odd_even_svc(void);
void sabbath_start_at_lobby(void);
void sab_up_dn_col_cc_msk_swap(void);
void sabbath_tables_cc(void);
int16 set_carcall(int16 car_no, int16 fl_no);
void set_upcall(int16 fl_no);
void set_dncall(int16 fl_no);
void set_rcarcall(int16 car_no, int16 fl_no);
void set_rupcall(int16 fl_no);
void set_rdncall(int16 fl_no);
void set_ir_dncall(int16 fl_no);
void set_ir_upcall(int16 fl_no);
void get_onward_calls (void);
void carcall_test(void);
void next_stop_lobby(void);
void carcall(void );
void cclatch(void);
void carcancel(void );
void sec_cc_latch (void);
void clr_sec_cc_latch (int16 select);
void clrcc(int16 select );
void clrhc(void );
void hallcall(void );
void hallcancel(void );
void stop_at_lobby(void);


void sabbath_std_svc(void)
{
	int16 i;
	int16 fl_ix;
	static int16 prev_dpref;
	uint32 sab_fl_msk[2];
	uint32 sab_rfl_msk[2];

	if (cons[Sabbath] == 2)
	{
		sab_fl_msk[0] = gc_sab_fl_msk[0];
		sab_fl_msk[1] = gc_sab_fl_msk[1];
		sab_rfl_msk[0] = gc_sab_rfl_msk[0];
		sab_rfl_msk[1] = gc_sab_rfl_msk[1];
	}
	else 
	{
		sab_fl_msk[0]  = 0xffffffff;
		sab_fl_msk[1]  = 0xffffffff;
		sab_rfl_msk[0]  = 0xffffffff;
		sab_rfl_msk[1]  = 0xffffffff;
	}
	
	if (((dpref == 0) || (dpref != prev_dpref)) && (firef == 0) && (rfiref == 0))
	{
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if (cons[SAB_Fl] == 1)
			{
				if (sab_ccmsk[i] == 1)
					set_carcall(cons[carnmb],i);
				if ((sab_rccmsk[i] == 1) && (cons[rear] == 1))
					set_rcarcall(cons[carnmb],i);
			}
			else
			{
				fl_ix = (i-1)/32;
				if ((((sab_fl_msk[fl_ix]  & fl_mask[i]) != 0) || (i == lobby_fl)) && (i != position))
				{
					if (fvars[fvccsonsab] == 1)		// car call lockouts enabled on sabbath service
					{	
						if ((master_cc_sec[fl_ix] & fl_mask[i]) == 0)
							set_carcall(cons[carnmb],i);
					}
					else
						set_carcall(cons[carnmb],i);
				}
				if (((sab_rfl_msk[fl_ix]  & fl_mask[i]) != 0) && (i != position) && (cons[rear] == 1))
				{
					if (fvars[fvccsonsab] == 1)		// car call lockouts enabled on sabbath service
					{	
						if ((master_rcc_sec[fl_ix] & fl_mask[i]) == 0)
							set_rcarcall(cons[carnmb],i);
					}
					else
						set_rcarcall(cons[carnmb],i);
				}
			}
		}
	}
	prev_dpref = dpref;
}

void sabbath_down_col_svc(void)
{
	int16 i;
	int sab_topf_location;	//0 =  front 1 = rear
	static int16 prev_dpref;


	if(sab_cc_topff > sab_cc_topfr)
		sab_topf_location = 0;
	else
		sab_topf_location = 1;


	if (((dpref == 0) || (dpref != prev_dpref)) && (firef == 0) && (rfiref == 0))
	{
		if((sab_topf_location == 0) && (position != sab_cc_topff)) 
		{
			if (sab_ccmsk[sab_cc_topff] == 1)
				set_carcall(cons[carnmb],sab_cc_topff);
		}
		else if((sab_topf_location == 1) && (position != sab_cc_topfr))
		{		
			if (sab_rccmsk[sab_cc_topfr] == 1)
				set_rcarcall(cons[carnmb],sab_cc_topfr);
			set_carcall(cons[carnmb],sab_cc_topfr);
		}
		else
		{
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if ((cons[SAB_Fl] == 1) || (cons[Sabbath] == 6))
				{
					if (sab_ccmsk[i] == 1)
						set_carcall(cons[carnmb],i);
					if ((sab_rccmsk[i] == 1) && (cons[rear] == 1))
						set_rcarcall(cons[carnmb],i);
				}
			}
		}
		
	}
	prev_dpref = dpref;
}


void sabbath_up_col_svc(void)
{
	int16 i;
	int16 sab_botf_location=0;	//0 =  front 1 = rear
	static int16 prev_dpref;
	
	if(cons[rear] == 0)
		sab_botf_location = 0;
	else
	{
		if(sab_cc_botff != 0)
			sab_botf_location = 0;
		if((sab_cc_botfr != 0) && (sab_cc_botfr < sab_cc_botff))
			sab_botf_location = 1;
	}
		
		
	if (((dpref == 0) || (dpref != prev_dpref)) && (firef == 0) && (rfiref == 0))
	{
		if((sab_botf_location == 0) && (position != sab_cc_botff)) 
		{
			if (sab_ccmsk[sab_cc_botff] == 1)
				set_carcall(cons[carnmb],sab_cc_botff);
		}
		else if((sab_botf_location == 1) && (position != sab_cc_botfr))
		{		
			if (sab_rccmsk[sab_cc_botfr] == 1)
				set_rcarcall(cons[carnmb],sab_cc_botfr);
		}
		else
		{
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if ((cons[SAB_Fl] == 1) || (cons[Sabbath] == 6))
				{
					if (sab_ccmsk[i] == 1)
						set_carcall(cons[carnmb],i);
					if ((sab_rccmsk[i] == 1) && (cons[rear] == 1))
						set_rcarcall(cons[carnmb],i);
				}
			}
		}
		
	}
	prev_dpref = dpref;
}

// This Sabbath service start from the lobby, then uses the up sabbath mask for the calls in the up direction
// and the down sabbath mask for the calls in the down direction

void sabbath_odd_even_svc(void)
{
	int16 i;
	static int16 prev_dpref;

	if (((dpref == 0) || (dpref != prev_dpref)) && (firef == 0) && (rfiref == 0))
	{
		if ((position == cons[topf]) || (position > (cons[topf]/2)))
		{

			for (i=position-1;i>=cons[bottomf];i--)
			{
				if (cons[SAB_Fl] == 1)
				{
					if (sab_dcmsk[i] == 1)
						set_carcall(cons[carnmb],i);
					if ((sab_rdcmsk[i] == 1) && (cons[rear] == 1))
						set_rcarcall(cons[carnmb],i);
				}
			}
		}
		else		// go in the up direction
		{
			for (i=position+1;i<=cons[topf];i++)
			{
				if (cons[SAB_Fl] == 1)
				{
					if (sab_ucmsk[i] == 1)
						set_carcall(cons[carnmb],i);
					if ((sab_rucmsk[i] == 1) && (cons[rear] == 1))
						set_rcarcall(cons[carnmb],i);
				}
			}
		}
	}
	prev_dpref = dpref;
}

//*********************************************
//***** Sabbath up down collective calls  *****
//***** with mask reading gpi             *****
//*********************************************

void sab_up_dn_col_cc_msk_swap()
{
	int16 i;
	int16 fl_ix;
	int16 update_gpi = 0;
	static uint32 sab_fl_msk[2];
	static uint32 sab_rfl_msk[2];
	
	if (sabbath_rst_msk == 0)
	{	// reset the mask on initial startup
		for (i=cons[bottomf];i<=cons[topf];i++)
			sab_ccmsk[i] = 0;
		sabbath_rst_msk = 1;
		update_gpi = 1;
	}


	for (i=0;i<2;i++)
	{
		if (sab_fl_msk[i] != gc_sab_fl_msk[i])
			update_gpi = 1;
		sab_fl_msk[i] = gc_sab_fl_msk[i];
		if (cons[rear] != 0)
		{
			if (sab_rfl_msk[i] != gc_sab_rfl_msk[i])
				update_gpi = 1;
		}
		sab_rfl_msk[i] = gc_sab_rfl_msk[i];
	}
	
	if (update_gpi == 1)
	{
		// reset top and bottom floor
		sab_cc_topff = 0;
		sab_cc_botff = 0;

		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			fl_ix = (i-1)/32;
			
			if (ccnmsk[cons[carnmb]][i] != 0)
			{
				sab_ccmsk[i] = ((sab_fl_msk[fl_ix] & fl_mask[i]) != 0);
				if (sab_ccmsk[i] == 1)
				{
					sab_cc_topff = i;

					if (sab_cc_botff == 0)
						sab_cc_botff = i;
				}
			}
			
			// rear sabbath car calls mask from inputs
			if (cons[rear] != 0)
			{
				if (rccnmsk[cons[carnmb]][i] != 0)
				{
					sab_rccmsk[i] = ((sab_rfl_msk[fl_ix] & fl_mask[i]) != 0);
					if (sab_rccmsk[i] == 1)
					{
						sab_cc_topfr = i;
						if (sab_cc_botfr == 0)
							sab_cc_botfr = i;
					}
				}
			} 
		}
	}

	if ((sabbath_start == 1) || ((fvars[fvsabben2] & 0x01) == 0))
	{
		servf = s_SABBATH;
		if(fvars[fvsabcolmode] == 0)
			sabbath_down_col_svc();
		else
			sabbath_up_col_svc();
	}
	else
		sabbath_start_at_lobby();
	
}



void sabbath_start_at_lobby(void)
{

	if ((firef == 0) && (rfiref == 0))
	{
		servf = s_SABBATH;				// take car out of group control
		if (dpref == 0)
		{
	 		if (position != lobby_fl )
				set_carcall(cons[carnmb],lobby_fl);
			else
			{
				servf = s_SABBATH;		// set sabbath service
				sabbath_start = 1;
			}
		}
	}
}


//********************************************************
// Special sabbath use push button SAB 
// input to go in the following sequence of latching cc's,
// once this seq finished go back to normal service
//********************************************************

void sabbath_tables_cc(void)
{
	int16 i;
	
	if (sabbath_active == 1)
	{
		if (sabbath_auto_calls == 0)
		{	// clear all of the existing car calls 
			clrcc(0);
			sabbath_auto_calls = 1;
		}
		else
		{
			if (sabbath_auto_calls == 1)
			{	
				// set the car calls from the sab_cc mask first
				sabbath_auto_calls = 2;
				ccmsk[cons[carnmb]] = &sab_ccmsk[0];
				if (cons[rear] == 1)
					rccmsk[cons[carnmb]] = &sab_rccmsk[0];
				clrcc(1);		// cancel the call with the invalid mask
				for (i=cons[bottomf];i<=cons[topf];i++)
				{
					if (sab_ccmsk[i] == 1)
						set_carcall(cons[carnmb],i);
					if ((sab_rccmsk[i] == 1) && (cons[rear] == 1))
						set_rcarcall(cons[carnmb],i);
				}
			}
			else if ((sabbath_auto_calls == 2) && (onward_cc == 0) && (dpref == 0))
			{	// when on the last car call doors about to open latch the second set of cc's
				if (doorf == 1)
				{
					// set the car calls from the sab_uc mask first
					sabbath_auto_calls = 3;
					ccmsk[cons[carnmb]] = &sab_ucmsk[0];
					if (cons[rear] == 1)
						rccmsk[cons[carnmb]] = &sab_rucmsk[0];
					clrcc(1);		// cancel the call with the invalid mask
					for (i=cons[bottomf];i<=cons[topf];i++)
					{
						if (sab_ucmsk[i] == 1)
							set_carcall(cons[carnmb],i);
						if ((sab_rucmsk[i] == 1) && (cons[rear] == 1))
							set_rcarcall(cons[carnmb],i);
					}
				}
			}
		}
	}
	else
	{
		if (sabbath_auto_calls != 0)
		{	// reset car call mask
			ccmsk[cons[carnmb]] = &ccnmsk[cons[carnmb]][0];
			if (cons[rear] == 1)
				rccmsk[cons[carnmb]] = &rccnmsk[cons[carnmb]][0];
			sabbath_auto_calls = 0;
		}
	}
}


void stop_at_lobby(void)
{
	int16 set_call = 0;

	if (((position > lobby_fl) && (cc_below_lby == 1) && ((fvars[fvstplby] & 0x02) != 0)) ||
  	     ((position < lobby_fl) && (cc_above_lby == 1) && ((fvars[fvstplby] & 0x01) != 0))) 
		set_call = 1;	  	     
	
	if ((set_call == 1) && (firef == 0) && (hsvf == 0) && (chk_ins() == 0))
	{
		if (carcb[lobby_fl] == 0)
	 		lvars[nmbcc]++;
	 	carcb[lobby_fl] = 1;
 		set_both_outps(o_CCL(lobby_fl),o_XCCL(lobby_fl));
	}
}

// *****************************************
// Get onward calls
// *****************************************
void get_onward_calls (void)
{
	int16 i,cbelow=0,cabove=0;
	int16 ccbelow=0,ccabove=0;
	int16 hcbelow=0,hcabove=0;

	onward_cc = 0;
	onward_hc = 0;
  	for(i=cons[bottomf]; i <= cons[topf]; i++)
  	{
    	if(carcb[i] == 1)  // There is a car call
    	{
      		if(i > position) // call above car
				ccabove = 1;
      		if(i < position) // call below car
				ccbelow = 1;
    	}
    	if(cons[rear] != 0)
    	{
	    	if(rcarcb[i] == 1)  // There is a rear car call
	    	{
	      		if(i > position) // call above car
					ccabove = 1;
	      		if(i < position) // call below car
					ccbelow = 1;
	    	}
    	}
    	if((upcb[i] == 1)&&(i != cons[topf]))   // There is a up hall call
    	{
      		if(i < position) // call below car
				hcbelow = 1;
      		if(i > position) // call above car
				hcabove = 1;
    	}
    	if(cons[rear] != 0)
    	{
	    	if((rupcb[i] == 1)&&(i != cons[topf]))   // There is a rear up hall call
	    	{
	      		if(i < position) // call below car
					hcbelow = 1;
	      		if(i > position) // call above car
					hcabove = 1;
	    	}
    	}
    	if((downcb[i] == 1)&&(i != cons[bottomf]))   // There is a down hall call
    	{
      		if(i > position) // call above car
				hcabove = 1;
      		if(i < position) // call below car
				hcbelow = 1;
    	}
    	if(cons[rear] != 0)
    	{
	    	if((rdowncb[i] == 1)&&(i != cons[bottomf]))   // There is a down hall call
	    	{
	      		if(i > position) // call above car
					hcabove = 1;
	      		if(i < position) // call below car
					hcbelow = 1;
	    	}
    	}
  	}

	// Special condition for stuck buttons
    if ((upcb[position] == 1) && (position != cons[topf]))   // There is a up hall call
    {
		if (doorf != 0)
		{
			if ((cons[rear] != 0) && (rupcb[position] == 1) && ((rnudgst & 0x01) == 0))
				cabove = 1;
			if ((cons[rear] != 0) && (rdowncb[position] == 1) && ((rnudgst & 0x02) == 0))
				cbelow = 1;
			if ((downcb[position] == 1) && ((nudgst & 0x02) == 0))
				cbelow = 1;
		}
    }
    if ((cons[rear] != 0) && (rupcb[position] == 1) && (position != cons[topf]))   // There is a rear up hall call
    {
		if (rdoorf != 0)
		{
			if ((upcb[position] == 1) && ((nudgst & 0x01) == 0))
				cabove = 1;
			if ((rdowncb[position] == 1) && ((rnudgst & 0x02) == 0))
				cbelow = 1;
			if ((downcb[position] == 1) && ((nudgst & 0x02) == 0))
				cbelow = 1;
		}
    }
    if ((downcb[position] == 1) && (position != cons[bottomf]))   // There is a down hall call
    {
		if (doorf != 0)
		{
			if ((cons[rear] != 0) && (rdowncb[position] == 1) && ((rnudgst & 0x02) == 0))
				cbelow = 1;
			if ((cons[rear] != 0) && (rupcb[position] == 1) && ((rnudgst & 0x01) == 0))
				cabove = 1;
			if ((upcb[position] == 1) && ((nudgst & 0x01) == 0))
				cabove = 1;
		}
    }
    if ((cons[rear] != 0) && (rdowncb[position] == 1) && (position != cons[bottomf]))   // There is a down hall call
    {
		if (rdoorf != 0)
		{
			if ((downcb[position] == 1) && ((nudgst & 0x02) == 0))
				cbelow = 1;
			if ((upcb[position] == 1) && ((nudgst & 0x01) == 0))
				cabove = 1;
			if ((rupcb[position] == 1) && ((rnudgst & 0x01) == 0))
				cabove = 1;
		}
	}

	onward_cc = (ccabove & 1) | ((ccbelow & 1) << 1);
	onward_hc = (hcabove & 1) | ((hcbelow & 1) << 1);
	onward_calls = (cabove & 1) | ((cbelow & 1) << 1) | onward_hc | onward_cc;
}

//******************************************************
//*********  Set Car Call Routine  *********************
//******************************************************

int16 set_carcall(int16 car_no, int16 fl_no)
{
	if (car_no == cons[carnmb])
	{
	    if ((fl_no >= cons[bottomf]) && (fl_no <= cons[topf]) && (ccmsk[car_no][fl_no] == 1) && (firef == 0) && (chk_ins() == 0))
		{
			if(carcb[fl_no] == 0)
			{
			    carcb[fl_no] = 1;
			    lvars[nmbcc]++;
			}
			set_both_outps(o_CCL(fl_no),o_XCCL(fl_no));
			set_both_inps(i_CC(fl_no),i_XCC(fl_no));
			return 1;
		}
	}
	else
	{
		if ((car_no >= 1) && (car_no <= cons[nmbcars]))
		{
		    if ((fl_no >= 1) && (fl_no <= cons[grtopf]) && (ccmsk[car_no][fl_no] == 1))
		   	{
	   			cc_req_fl[0] = (uint8)(fl_no & 0xff);		// set up a front car call on a slave car
	   			put_pkt_req(3,car_no,gc_port);
	   			return 1;
	   		}
		}
	}
	return 0;
}

//******************************************************
//*********  Set Up Call Routine  *********************
//******************************************************

void set_upcall(int16 fl_no)
{
	int16 fl_ix;

	if (cons[carnmb] == Dispatcher)
	{
		fl_ix = (fl_no - 1)/32;
		if((fl_no > 0) && (fl_no <= cons[grtopf]) && ((g_ucmsk[fl_ix] & fl_mask[fl_no]) != 0) && (firef == 0) && (all_cars_ots == 0) )
		{
//			if(upcb[fl_no] == 0)
//				upcb[fl_no] = 1;
			if (gc_hc_dev == 0)
			{

			    setgoutp(o_UCL(fl_no));
#if (Simulator == 1)
				setginp(i_UC(fl_no));
//				iodata[1][ucd[fl_no]] |= ucb[fl_no];
#endif
			}
			else
			{
				gc_uc_set[fl_ix] |= fl_mask[fl_no];
//				gc_uc[fl_ix] |= fl_mask[fl_no];
			}
		}
    }
}


//******************************************************
//*********  Set Down Call Routine  ********************
//******************************************************

void set_dncall(int16 fl_no)
{
	int16 fl_ix;

	if (cons[carnmb] == Dispatcher)
	{
		fl_ix = (fl_no - 1)/32;
		if((fl_no > 0) && (fl_no <= cons[grtopf]) && ((g_dcmsk[fl_ix] & fl_mask[fl_no]) != 0) && (firef == 0) && (all_cars_ots == 0) )
		{
//			if(downcb[fl_no] == 0)
//				downcb[fl_no] = 1;

			if (gc_hc_dev == 0)
			{
			    setgoutp(o_DCL(fl_no));
#if (Simulator == 1)
				setginp(i_DC(fl_no));
//				iodata[1][dcd[fl_no]] |= dcb[fl_no];
#endif
			}
			else
			{
				gc_dc_set[fl_ix] |= fl_mask[fl_no];
//				gc_dc[fl_ix] |= fl_mask[fl_no];
			}
		}
    }
}


//******************************************************
//*********  Set Rear Car Call Routine  ****************
//******************************************************

void set_rcarcall(int16 car_no, int16 fl_no)
{
	if (car_no == cons[carnmb])
	{
	    if ((fl_no >= cons[bottomf]) && (fl_no <= cons[topf]) && (rccmsk[car_no][fl_no] == 1) && (rfiref == 0) && (chk_ins() == 0))
		{
			if(rcarcb[fl_no] == 0)
			{
			    rcarcb[fl_no] = 1;
			    lvars[nmbcc]++;
			}
			set_both_outps(o_CCRL(fl_no),o_XCCRL(fl_no));
			set_both_inps(i_CCR(fl_no),i_XCCR(fl_no));
		}
	}
	else
	{
		if ((car_no >= 1) && (car_no <= cons[nmbcars]))
		{
		    if ((fl_no >= 1) && (fl_no <= cons[grtopf]) && (rccmsk[car_no][fl_no] == 1))
			{
				rcc_req_fl[0] = (uint8)(fl_no & 0xff);		// set up a front car call on a slave car
				put_pkt_req(4,car_no,gc_port);
			}
		}
	}
}

//******************************************************
//*********  Set Rear Up Call Routine  *****************
//******************************************************

void set_rupcall(int16 fl_no)
{
	int16 fl_ix;

	if (cons[carnmb] == Dispatcher)
	{
		fl_ix = (fl_no - 1)/32;
		if((fl_no > 0) && (fl_no <= cons[grtopf]) && ((g_rucmsk[fl_ix] & fl_mask[fl_no]) != 0) && (firef == 0) && (all_cars_ots == 0) )
		{
			if (gc_hc_dev == 0)
			{
			    setgoutp(o_RUCL(fl_no));
#if (Simulator == 1)
				setginp(i_RUC(fl_no));
//				iodata[1][dcd[fl_no]] |= dcb[fl_no];
#endif
			}
			else
			{
	//    		if(rupcb[fl_no] == 0)
	//				rupcb[fl_no] = 1;

				gc_ruc_set[fl_ix] |= fl_mask[fl_no];
	//			gc_ruc[fl_ix] |= fl_mask[fl_no];
			}
		}
   	}
}

//******************************************************
//*********  Set Rear Down Call Routine  ***************
//******************************************************

void set_rdncall(int16 fl_no)
{
	int16 fl_ix;

	if (cons[carnmb] == Dispatcher)
	{
		fl_ix = (fl_no - 1)/32;
		if((fl_no > 0) && (fl_no <= cons[grtopf]) && ((g_rdcmsk[fl_ix] & fl_mask[fl_no]) != 0) && (firef == 0) && (all_cars_ots == 0) )
		{
			if (gc_hc_dev == 0)
			{
			    setgoutp(o_RDCL(fl_no));
#if (Simulator == 1)
				setginp(i_RDC(fl_no));
#endif
			}
			else
			{
//    			if(rdowncb[fl_no] == 0)
//					rdowncb[fl_no] = 1;

				gc_rdc_set[fl_ix] |= fl_mask[fl_no];
//				gc_rdc[fl_ix] |= fl_mask[fl_no];
			}

		}
	}
}

//******************************************************
//*********  Set Inconspicious Up Call Routine  ********
//******************************************************

void set_ir_upcall(int16 fl_no)
{
	int16 fl_ix;
	
	if ((cons[incrs] != 0) || (cons[Second_Riser] != 0))
	{
		if (cons[carnmb] == Dispatcher)
		{
			if (((gc_ir != 0) && (fvars[fvircar] != 0)) || 
			    ((gc_ir_2 != 0) && (fvars[fvircar2] != 0) && (fvars[fvirctl] & 0x01) != 0) || 
			    ((fvars[fvircar] != 0) && ((fvars[fvirctl] & 0x01) != 0)))
			{
				fl_ix = (fl_no - 1)/32;
				if ((fl_no > 0) && (fl_no <= cons[grtopf]) && ((g_irucmsk[fl_ix] & fl_mask[fl_no]) != 0) && (firef == 0) && (all_cars_ots == 0))
				{
					gc_iruc_set[fl_ix] |= fl_mask[fl_no];
					if ((fvars[fvirctl] & 0x01) != 0)
						ir_active = 1;
//					gc_iruc[fl_ix] |= fl_mask[fl_no];
				}
			}
	    }
	}
}

//******************************************************
//*********  Set Inconspicious Down Call Routine  ******
//******************************************************


void set_ir_dncall(int16 fl_no)
{
	int16 fl_ix;

	if ((cons[incrs] != 0) || (cons[Second_Riser] != 0))
	{
		if (cons[carnmb] == Dispatcher)
		{
			if (((gc_ir != 0) && (fvars[fvircar] != 0)) || 
			    ((gc_ir_2 != 0) && (fvars[fvircar2] != 0) && (fvars[fvirctl] & 0x01) != 0) || 
			    ((fvars[fvircar] != 0) && ((fvars[fvirctl] & 0x01) != 0)))
			{
				fl_ix = (fl_no - 1)/32;
				if ((fl_no > 0) && (fl_no <= cons[grtopf]) && ((g_irdcmsk[fl_ix] & fl_mask[fl_no]) != 0) && (firef == 0) && (all_cars_ots == 0))
				{
					gc_irdc_set[fl_ix] |= fl_mask[fl_no];
					if ((fvars[fvirctl] & 0x01) != 0)
						ir_active = 1;
//					gc_irdc[fl_ix] |= fl_mask[fl_no];
				}
			}
		}
	}
}



// *****************************************
// This is the clear car call procedure
// *****************************************
void clrcc(int16 select)
{						// select: 0 = all floors, 1 = invalid floors (mask has changed)
  	int16 i;
  	for(i = cons[bottomf]; i <= cons[topf]; i++)
  	{
		if ((select == 0) || ((select == 1) && (ccmsk[cons[carnmb]][i] == 0)))
		{
	  	  	clr_both_outps(o_CCL(i),o_XCCL(i));
			if (cons[Remote_CC] != 0)
		  		clroutp(o_RM_CCL(i));
	  	  	carcb[i] = 0;
	  	  	cctim[i] = 0;
#if (Simulator == 1)
			clr_both_inps(i_CC(i),i_XCC(i));
			if (cons[Remote_CC] != 0)
			{
				clrinp(i_RM_CC(i));
			}
#endif
		}

    	if(cons[rear] != 0)
    	{
			if ((select == 0) || ((select == 1) && (rccmsk[cons[carnmb]][i] == 0)))
			{
				clr_both_outps(o_CCRL(i),o_XCCRL(i));
				if (cons[Remote_CC] != 0)
					clroutp(o_RM_CCRL(i));
				rcarcb[i] = 0;
				ccrtim[i] = 0;
#if (Simulator == 1)
				clr_both_inps(i_CCR(i),i_XCCR(i));
				if (cons[Remote_CC] != 0)
				{
					clrinp(i_RM_CCR(i));
				}
#endif
			}
    	}
  	}
  	timers[ccct] = 0;
	nmb_carcalls = 0;
	
	if (select == 0)
	{
		pi_cc_mrcan_floor = 0;
		pi_cc_mrcan_command = 0x04;   // clear all calls after last call
		pi_cc_mrcan_update = 1;

		pi_cc_floor = 0;
		pi_cc_command = 0x04;   // clear all calls after last call
		pi_cc_update = 1;
	}
	
	if ((Security_Type[cons[carnmb]] & 0x40) != 0)
		clr_sec_cc_latch(select);
	
  	return;
}


//******************************************************
//*********  Set Car Call Test Routine  *********************
//******************************************************

void carcall_test()
{
	int16 i = 0;
	int16 fl_ix = 0;
	static int prev_dpref;

	if (((dpref == 0) || (dpref != prev_dpref)) && (firef == 0) && (rfiref == 0))
	{
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			fl_ix = (i-1)/32;
		    if ( ((cc_test_mask[fl_ix] & fl_mask[i]) != 0) && (i != position) )
			{	set_carcall(cons[carnmb],i);
				if (cons[rear] == 1)
					set_rcarcall(cons[carnmb],i);
			}
		}
	}
	prev_dpref = dpref;
}

//******************************************************
//*********  Stop at Lobby Next Car Call Routine  ******
//******************************************************

void next_stop_lobby(void)
{
	if ((firef == 0) && (rfiref == 0))
	{ 
		if(ind() == 1)	 
		{
			if (dpref == 0)		// there is no preference
			{
				if(timers[tindrcl] > fvars[fvindrcltmr])
				{
					if (fvars[fvlob] != position)
					{
						set_carcall(cons[carnmb],fvars[fvlob]);
						if (cons[rear] == 1)
							set_rcarcall(cons[carnmb],fvars[fvlob]);
					}
				}
			}
			else
			{
				timers[tindrcl] = 0;
			}
		}
		else   // In automatic
		{
			if (((fvars[fvlob] < position) && (dpref == 2))	||		// lobby is below and preference is down
				((fvars[fvlob] > position) && (dpref == 1)) ||		// lobby is above and preference is up
				(dpref == 0))										// or there is no preference
			{
				if (fvars[fvlob] != position)
				{
					set_carcall(cons[carnmb],fvars[fvlob]);
					if (cons[rear] == 1)
						set_rcarcall(cons[carnmb],fvars[fvlob]);
				}
			}
		}
	}
}



// *****************************************
// This is the car call procedure
// *****************************************
void carcall()
{

	if((manual_door == 1) && (cons[att] == 1) && ((car_gate() == 0) || (man_dcc() == 0)) &&
		(((fvars[fvattmandir] & 0x04)!=0)  ||  ((athcd_in_enable == 1)&&(rdinp(i_ATHCD) == 1)))) 
	{
		timers[ccct] = 0;       // don't register a car call if gate or contacts are open
		clrcc(0);
  	}

  	if (((cons[mand] == 1) && (cons[frghtd] == 0) && (fvars[fvsapb] == 0)) || 
  			(((fvars[fvsapb] & 0x01) != 0) && (cons[nmbcars] == 1)))
  	{
		
	   if ((fvars[fvsapb] & 0x04) == 0)
  	   {
  	   		if(rdinp(i_DOL) == 0)// if manual doors and the door is open
  	   		{
		  		timers[ccct] = 0;       // don't register a car call
				clrcc(0);
  	   		}
	   		if (dpref != 0)
		  		timers[ccct] = 0;       // single automatic pushbutton
	   }
	   else 
	   {
  	   		if ((dpref != 0) && (nmb_cc !=0))
		  		timers[ccct] = 0;       // single automatic pushbutton
	   }

  	}
	if (cons[cc_pb_sec] == 1)
  	{
  	    if (chk_push_button() == 1)
  	    	return;
  	}

  	if(timers[ccct] <= 4)
  	    return;

	cclatch();

	if ((fvars[fvstplby] & 0x03) != 0)
		stop_at_lobby();

	if ((Security_Type[cons[carnmb]] & 0x40) != 0)
		sec_cc_latch();
}

//*******************************************
//*** Latch the car call if not secured
//*******************************************

void cclatch(void)
{
  	int16 i,start=cons[bottomf],finish=cons[topf];
	int16 sec_override;
	int16 enable_cc;
	int16 fl_ix;
	int16 cnt_cc = 0;
	int16 sr_cc_sec_by_dir = 0;


	top_cc = 0;
	bot_cc = cons[topf];
	nmb_cc = 0;
	nmb_carcalls = 0;
	nmb_cc_above_lby = 0;

  	if(((cons[behindc] == 1) || (fvars[fvbehindcc] == 1)) && (ind() == 0) && (firef == 0) && (rfiref == 0))
  	{
  	  	if((dpref == 1) && (position < cons[topf]))
  	  	  	start = (position+1);
  	  	if((dpref == 2) && (position > cons[bottomf]))
  	  	{
  	  	  	start = cons[bottomf];
  	  	  	finish = (position-1);
  	  	}
  	}

	if ((servf == s_IND) && ((fvars[fvindsec] & 0x01) != 0))		// on independent and want to override security
		ind_override = 1;
	else if ((cons[IndSec] == 1) && (rdinp(i_INDS) == 1) && (servf == s_IND) && (rdinp(i_IDSO) == 1))
		ind_override = 1;
	else
		ind_override = 0;

	
	if ((servf == s_IND) && ((fvars[fvindsec] & 0x02) != 0))		// on independent and want to override security
		ind_secf_override = 1;
	else if ((cons[IndSec] == 1) && (rdinp(i_INDS) == 1) && (servf == s_IND) && (rdinp(i_IDSO) == 1))
		ind_secf_override = 1;
	else
		ind_secf_override = 0;	
	
	if (((servf == s_MED_EMS) || (servf == s_HSV)) && (fvars[fvmedccsovride] == 1))
	// on code blue, medical service or hospital service
		med_override = 1;
	else
		med_override = 0; 
  	
	if ((cons[sec_dis] & 0x01) != 0)
	{
		if (position == lobby_fl)
		{
			if (secdis_override == 0)
			{
				timers[tsecdis] = 0;
				if (rdinp(i_SCD) == 1)
					secdis_override = 1;
			}
			else
			{
				if (timers[tsecdis] > fvars[fvsecdist])
					secdis_override = 0;
			}
		}
		else
		{
			secdis_override = 0;
			timers[tsecdis] = 0;
		}
	}

  	for(i=start; i <= finish; i++) 
  	{
		cnt_cc = 0;
		fl_ix = (i-1)/32;


		if ( ((Lockout_Override == 1) && ((rdinp(i_CCS(i)) == 1) || (rdinp(i_SECUR) == 1)) && ((c_tmr_svc[s_ccl_override] == 1) || (c_tmr_svc_en[s_ccl_override] == 0)) && (Car_Call_Lockouts == 0)) ||
		     ((Grp_CC_Override == 1) && ((gc_cc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0)  && ((c_tmr_svc[s_gccl_override] == 1) || (c_tmr_svc_en[s_gccl_override] == 0)) ) )
			sec_override = 1;		// ccs input used for override or lockout but not both
		else
			sec_override = 0;

		if ((cons[Second_Riser] == 1) && (cons[SecFlCfg] == 8) && (rdinp(i_SECFM) == 1))
		{
			if ((fvars[fvsrccsecbydir] != 0) && ((sr_cc_ctrl[cons[carnmb]] & 0x01) != 0))
			{	// second riser car call security by direction, works in conjunction with cons[SecFlCfg] = 8 car call masks tables
				if (((fvars[fvsrccsecbydir] == 1) && (((dpref == 0) || (dpref == 1)) && (i >= position))) || 	// allow up dir cc
					((fvars[fvsrccsecbydir] == 2) && (((dpref == 0) || (dpref == 2)) && (i <= position))) || 	// allow down dir cc
					(ccsmsk[cons[carnmb]][i] != 0))
					sr_cc_sec_by_dir = 0;
				else
					sr_cc_sec_by_dir = 1;
			}
		}
		
		if (((cons[smoke_interface] & 0x01) != 0) && ((cons[smoke_interface] & 0x02) != 0))
		{
			if ((hsvf != 0) && (firef != 0) && (i <= 24))
			{
				if ((rdinp(i_CC(i)) == 1) && (rdinp(i_SMK(i)) == 0))
					hsvf = 0;
			}
		}
		
		if ( ((cons[Car_Asgn_Fl] != 0) && (car_Grp_Stat.c_car_asgn_sec == 1) && (car_asgn_cc[i] != cons[carnmb]) && (car_asgn_cc[i] != 0)) ||
			 ((Car_Call_Lockouts == 1) && (rdinp(i_CCS(i)) == 1) && (((cons[sec_dis] & 0x02) == 0) || (((cons[sec_dis] & 0x02) != 0) && (position == lobby_fl))) && ((rdinp(i_SECUR) == 1) || (tmr_svc[s_cc_lockouts] == 1))) ||
			 ((cc_sec_mask[fl_ix] & fl_mask[i]) != 0) ||
			 ((cons[hugs] == 2) && (gc_hugsec == 1) && ((gc_hugs_sec[fl_ix] & fl_mask[i]) != 0)) ||
			 ((Grp_CC_Lockouts == 1) && ((gc_sec == 1) || (c_tmr_svc[s_gcc_lockouts] == 1)) && ((gc_cc_sec[fl_ix] & fl_mask[i]) != 0)) ||	 
			 (((ids_cc_sec[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && ((cons[ids] & 1) != 0)) ||
			 (((ui_cc_sec[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && ((cons[Galileo] & 1) != 0)) ||
			 (((gc_ccs_per_fl[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && (cons[SecFlCfg] == 7) && (fvars[fvccpbsongpccsec] == 1) && (cons[cc_pb_sec] == 1)) || (sr_cc_sec_by_dir == 1))

			master_cc_sec[fl_ix] |= fl_mask[i];
		else
			master_cc_sec[fl_ix] &= ~fl_mask[i];
			
    	if ( (ccmsk[cons[carnmb]][i] == 1) && ((i != position) || (procf < 13)) && 
    		 ( ((master_cc_sec[fl_ix] & fl_mask[i]) == 0) ||
			   (firef != 0) || (hsvf != 0) || (ind_override == 1) || (med_override == 1) ||
			   ((cons[Prison] != 0)&&(remote_cc == 1))||  
			   (sec_override == 1) || (i == fvars[fvsecfl]) || (secdis_override == 1)) )	  
    	{
			if ((remote_cc == 0) || (cons[Remote_CC] == 0) || 							//no remote COP
				((((fvars[fvrmccd] & 0x01) == 0) || ((dirf != 1 ) && (dpref != 1 ))) && //not disable for up or not going up
				 (((fvars[fvrmccd] & 0x02) == 0) || ((dirf != 2 ) && (dpref != 2 ))) && //not disable for dn or not going dn
				 ((fvars[fvrmcc] & 0x01) == 0)))										//do not seperate this logic
				enable_cc = 1;
			else
				enable_cc = 0;

    	  	if ( ((enable_cc == 1) && ((rd_both_inps(i_CC(i),i_XCC(i))==1) || ((c_cc_req[fl_ix] & fl_mask[i]) != 0))) || 
    	  		 ((remote_cc == 1) && (rdinp(i_RM_CC(i)) == 1)) )
    	  	{
    	  		secdis_override = 0;
#if ((Traction == 0) && (Tract_OL == 0))
				if ((((empf != 0) && ((cons[EMPop] & 0x02) == 0)) || (stallf != 0) || (hotoilf != 0)) && (firef >= 4) && (i > position))
				{
					carcb[i] = 0;	   // phase 2 empower lowering: cannot run up
					clr_both_outps(o_CCL(i),o_XCCL(i));
					if (cons[Remote_CC] != 0)
						clroutp(o_RM_CCL(i));
#if (Simulator == 1)
					clr_both_inps(i_CC(i),i_XCC(i));
					if (cons[Remote_CC] != 0)
						clrinp(i_RM_CC(i));
					
#endif
				}
				else 
#endif
				{
					if(carcb[i] == 0)
					{
					  	carcb[i] = 1;
					  	lvars[nmbcc]++;
						if ((hb_en == 1) && (((cons[hben] & 0x02) != 0) || ( ((cons[hben] & 0x04) != 0))))
							hb_cc_buz = 1;
						timers[thbcc] = 0;
					}
					if ((enable_cc == 1) || ((fvars[fvrmcc] & 0x04) != 0))
						set_both_outps(o_CCL(i),o_XCCL(i));
					if ((remote_cc == 1) || (((fvars[fvrmcc] & 0x02) != 0) && (cons[Remote_CC] != 0)))
						setoutp(o_RM_CCL(i));
					if (i > top_cc) 
						top_cc = i;
					if (i < bot_cc)
						bot_cc = i;
					nmb_cc++;
					cnt_cc = 1;
					nmb_carcalls++;
					if (i > lobby_fl)
						nmb_cc_above_lby++;		// count for up peak detection
				}
    	  	}
    	}
    	
	    if(cons[rear] != 0)
	    {
			if ( ((Lockout_Override == 1) && ((rdinp(i_RCCS(i)) == 1) || (rdinp(i_RSECU) == 1)) && ((c_tmr_svc[s_ccl_override] == 1) || (c_tmr_svc_en[s_ccl_override] == 0)) && (Car_Call_Lockouts == 0)) ||
		         ((Grp_CC_Override == 1) && ((gc_rcc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0) && ((c_tmr_svc[s_gccl_override] == 1) || (c_tmr_svc_en[s_gccl_override] == 0)) ) )
				sec_override = 1;		// rccs input used for override or lockout but not both
			else
				sec_override = 0;

			if ( ((cons[Car_Asgn_Fl] != 0) && (car_Grp_Stat.c_car_asgn_sec == 1) && (car_asgn_rcc[i] != cons[carnmb]) && (car_asgn_rcc[i] != 0)) ||
				 ((Car_Call_Lockouts == 1) && (rdinp(i_RCCS(i)) == 1) && ((rdinp(i_RSECU) == 1) || (c_tmr_svc[s_cc_lockouts] == 1))) ||
				 ((rcc_sec_mask[fl_ix] & fl_mask[i]) != 0) ||
				 ((cons[hugs] == 2) && (gc_rhugsec == 1) && ((gc_rhugs_sec[fl_ix] & fl_mask[i]) != 0)) ||
		         ((Grp_CC_Lockouts == 1) && ((gc_rsec == 1) || (c_tmr_svc[s_gcc_lockouts] == 1)) && ((gc_rcc_sec[fl_ix] & fl_mask[i]) != 0)) ||  // gc_rcc_sec is shifted to use i+1
				 (((ids_rcc_sec[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && ((cons[ids] & 1) != 0)) ||
				 (((ui_rcc_sec[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && ((cons[Galileo] & 1) != 0)) ||
				 (((gc_rccs_per_fl[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && (cons[SecFlCfg] == 7) && (fvars[fvccpbsongpccsec] == 1) && (cons[cc_pb_sec] == 1)) )
				master_rcc_sec[fl_ix] |= fl_mask[i];
			else
				master_rcc_sec[fl_ix] &= ~fl_mask[i];
			 
			if ( (rccmsk[cons[carnmb]][i] == 1) && ((i != position) || (procf < 13)) && 
				( ((master_rcc_sec[fl_ix] & fl_mask[i]) == 0) || 
				  (firef != 0) || (hsvf != 0) || (ind_override == 1) || (med_override == 1) ||
   				   ((cons[Prison] != 0)&&(remote_cc == 1))||   
				  (sec_override == 1) || (i == fvars[fvsecfl]) || (secdis_override == 1)) )
			{
				if ((remote_rcc == 0)  || (cons[Remote_CC] == 0) || 					//no remote COP
				((((fvars[fvrmccd] & 0x01) == 0) || ((dirf != 1 ) && (dpref != 1 ))) && //not disable for up or not going up
				 (((fvars[fvrmccd] & 0x02) == 0) || ((dirf != 2 ) && (dpref != 2 ))) && //not disable for dn or not going dn
				 ((fvars[fvrmcc] & 0x01) == 0)))										//do not seperate this logic
					enable_cc = 1;
				else
					enable_cc = 0;

			    if (((enable_cc == 1) && ((rd_both_inps(i_CCR(i),i_XCCR(i))==1) || ((c_rcc_req[fl_ix] & fl_mask[i]) != 0))) ||
					((remote_rcc == 1) && (rdinp(i_RM_CCR(i))==1)))
			    {
    	  			secdis_override = 0;
#if ((Traction == 0) && (Tract_OL == 0))
					if ((((empf != 0) && ((cons[EMPop] & 0x02) == 0)) || (stallf != 0) || (hotoilf != 0)) && (rfiref >= 4) && (i > position))
					{
						rcarcb[i] = 0;	   // phase 2 empower lowering: cannot run up
						clr_both_outps(o_CCRL(i),o_XCCRL(i));
						if (cons[Remote_CC] != 0)
							clroutp(o_RM_CCRL(i));

#if (Simulator == 1)
						clr_both_inps(i_CCR(i),i_XCCR(i));
						if (cons[Remote_CC] != 0)
							clrinp(i_RM_CCR(i));
#endif
					}
					else 
#endif
					{
						if(rcarcb[i] == 0)
						{
				    		rcarcb[i] = 1;
				    		lvars[nmbcc]++;
							if ((hb_en == 1) && ((cons[hben] & 0x02) != 0) || (( ((cons[hben] & 0x04) != 0))))
								hb_cc_buz = 1;
							timers[thbcc] = 0;
						}
						if ((enable_cc == 1) || ((fvars[fvrmcc] & 0x04) != 0))
							set_both_outps(o_CCRL(i),o_XCCRL(i));
						if ((remote_rcc == 1) || (((fvars[fvrmcc] & 0x02) != 0) && (cons[Remote_CC] != 0)))
							setoutp(o_RM_CCRL(i));
						if (i > top_cc) 
							top_cc = i;
						if (i < bot_cc)
							bot_cc = i;
						if (cnt_cc == 0)
							nmb_cc++;
						nmb_carcalls++;
						if (i > lobby_fl)
							nmb_cc_above_lby++;	   // count for up peak	detection
		    		}
				}
	    	}
	  	}
	}
}

// **************************************
// this is the car call cancel procedure
// **************************************
void carcancel()
{
  	int16 i;

	if ( ((manual_door != 0) && (manual_door != 2)) || (rdinp(i_AD) == 0) || (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) ||
		(cons[pbdoor] != 0)  || ((cons[pbdoormsk] != 0) && (pbdoor_fmsk[position] != 0)) || 
		(firef != 0) || (rfiref != 0) || (doorf != 0)|| ((cons[Prison] == 2) && (remote_cc == 1))) 
	{
	  	clr_both_outps(o_CCL(position),o_XCCL(position));
		if (cons[Remote_CC] != 0)
			  clroutp(o_RM_CCL(position));

	  	carcb[position] = 0;

#if (Simulator == 1)
		clr_both_inps(i_CC(position),i_XCC(position));
		if (cons[Remote_CC] != 0)
			clrinp(i_RM_CC(position));
#endif

	  	for(i=cons[bottomf]; i<=cons[topf]; i++)
	  	{
	    	if(carcb[i] == 0)
	    	  	cctim[i] = 0;
	    	if((rd_both_inps(i_CC(i),i_XCC(i)) == 0) && (cctim[i] > 10))
				cctim[i] = 0;
	  	}
	}
	if ( ((manual_rdoor != 0) && (manual_rdoor != 2)) || (rdinp(i_AD) == 0) || (((cons[ids] & 1) != 0) && (car_Grp_Stat.c_ids_ad_off == 1)) ||
		(cons[pbdoor] != 0)  || ((cons[pbdoormsk] != 0) && (pbdoor_rmsk[position] != 0)) || 
		(firef != 0) || (rfiref != 0) || (rdoorf != 0)|| ((cons[Prison] == 2) && (remote_cc == 1))) 
	{
	  	if(cons[rear] != 0)
	  	{
			clr_both_outps(o_CCRL(position),o_XCCRL(position));
			if (cons[Remote_CC] != 0)
				clroutp(o_RM_CCRL(position));
			rcarcb[position] = 0;

#if (Simulator == 1)
			clr_both_inps(i_CCR(position),i_XCCR(position));
			if (cons[Remote_CC] != 0)
				clrinp(i_RM_CCR(position));
#endif

			for(i=cons[bottomf]; i<=cons[topf]; i++)
			{
		    	if(rcarcb[i] == 0)
		    	  	ccrtim[i] = 0;
				if(rd_both_inps(i_CCR(i),i_XCCR(i))==0)
					ccrtim[i] = 0;
			}
	  	}
	}
  	return;
}


// **************************************
// Security car call latch output routine
// **************************************

void sec_cc_latch (void)
{
	int16 i;

	static int16 update_ccl_time;
	
	if (fvars[fvcltim] != 0)		// set the ccl output for the fvars[fvcltim] amount of time
	{
		if (((timers[tsec] != 0) && (update_ccl_time == 1)) || 
			((timers[tsec] == 0) && (update_ccl_time == 0)))
		{	// on rising and falling edge update 1 sec timers (every 1 second)
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if (ccltim[i] < 32000)
					ccltim[i]++;
				else
					ccltim[i]=0;
			}
			
			if (cons[rear] != 0)
			{
				for (i=cons[bottomf];i<=cons[topf];i++)
				{
					if (rccltim[i] < 32000)
						rccltim[i]++;
					else
						rccltim[i]=0;
				}
			}
		}
		
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if (rdoutp(o_CCL(i)) != 0)
			{
				// set the SCCL output for the fvars[fvcltim] amount of time
				if ((ccltim[i] != 0) && (ccltim[i] <= fvars[fvcltim]))
					setoutp(o_SCCL(i));
				else
					clroutp(o_SCCL(i));
			}
			else
			{
				ccltim[i]=0;
				clroutp(o_SCCL(i));
			}
		}
		
		if (cons[rear] != 0)
		{
			for (i=cons[bottomf];i<=cons[topf];i++)
			{
				if (rdoutp(o_CCRL(i)) == 1)
				{
					if ((rccltim[i] != 0) && (rccltim[i] <= fvars[fvcltim]))
						setoutp(o_SCCRL(i));
					else
						clroutp(o_SCCRL(i));
				}
				else
				{
					rccltim[i]=0;
					clroutp(o_SCCRL(i));
				}
			}
		}

		if (timers[tsec] == 0)
			update_ccl_time = 1;
		else
			update_ccl_time = 0;

	}
	else
	{
		for (i=cons[bottomf];i<=cons[topf];i++)
		{
			if (rdoutp(o_CCL(i)) == 1)
				setoutp(o_SCCL(i));
			else
				clroutp(o_SCCL(i));
			if (cons[rear] == 1)
			{
				if (rdoutp(o_CCRL(i)) == 1)
					setoutp(o_SCCRL(i));
				else
					clroutp(o_SCCRL(i));
			}
		}
	}
}


// **************************************
// Clear security car call latch output 
// **************************************

void clr_sec_cc_latch (int16 select)
{						// select: 0 = all floors, 1 = invalid floors (mask has changed)
	int16 i;

	for (i=cons[bottomf];i<=cons[topf];i++)
	{
		if ((select == 0) || ((select == 1) && (ccmsk[cons[carnmb]][i] == 0)))
			clroutp(o_SCCL(i));
		if (cons[rear] == 1)
		{
			if ((select == 0) || ((select == 1) && (rccmsk[cons[carnmb]][i] == 0)))
				clroutp(o_SCCRL(i));
		}
	}
}

// *****************************************
// This is the clear hall call procedure
// *****************************************
void clrhc()
{
  int16 i;
  for(i = cons[bottomf]; i < cons[topf]; i++)
  {
    upcb[i] = 0;
  }
  for(i = (cons[bottomf]+1); i <= cons[topf]; i++)
  {
    downcb[i] = 0;
  }
  cancel_dn = 0;
  cancel_up = 0;
  c_up_hall_call[0] = 0;
  c_up_hall_call[1] = 0;
  c_dn_hall_call[0] = 0;
  c_dn_hall_call[1] = 0;
  if(cons[rear] != 0)
  {
	for(i = cons[bottomf]; i < cons[topf]; i++)
	{
		rupcb[i] = 0;
	}
	for(i = (cons[bottomf]+1); i <= cons[topf]; i++)
	{
		rdowncb[i] = 0;
	}
	cancel_dnr = 0;
	cancel_upr = 0;
	c_upr_hall_call[0] = 0;
	c_upr_hall_call[1] = 0;
	c_dnr_hall_call[0] = 0;
	c_dnr_hall_call[1] = 0;
  }
  return;
}

// *******************************
// register a hall call procedure
// *******************************
void hallcall()
{
  	int16 i;
  	for(i=1; i < cons[grtopf]; i++)
  	{
		upcb[i] = 0;
  	  	if( (ucmsk[cons[carnmb]][i]==1) && ((c_up_hall_call[(i-1)/32] & fl_mask[i]) != 0) )
			upcb[i] = 1;

		if ((cons[Second_Riser] != 0) && ( (irucmsk[cons[carnmb]][i]==1) && ((c_irup_hall_call[(i-1)/32] & fl_mask[i]) != 0) ))
				upcb[i] = 1;

		if (((cons[Third_Riser] != 0) && (cons[rear] == 0)) && ((trucmsk[cons[carnmb]][i]==1) && ((g_trup_hall_call[(i-1)/32][cons[carnmb]] & fl_mask[i]) != 0)))
			upcb[i] = 1;
		
		if ((fvars[fvattcc] == 1) && (upcb[i] == 1) && (carcb[i] == 0) && (servf == s_ATT))	// on attandent
		{
			if((position != i) || ((procf >= 3) && (procf <= 11))) 
				set_carcall(cons[carnmb],i);
		}
  	}
  	for(i=2; i <= cons[grtopf]; i++)
  	{
		downcb[i] = 0;
  	  	if( (dcmsk[cons[carnmb]][i] == 1) && ((c_dn_hall_call[(i-1)/32] & fl_mask[i]) != 0) )
			downcb[i] = 1;

 	  	if ((cons[Second_Riser] != 0) && ( (irdcmsk[cons[carnmb]][i] == 1) && ((c_irdn_hall_call[(i-1)/32] & fl_mask[i]) != 0) ))
			downcb[i] = 1;

  	  	if (((cons[Third_Riser] != 0) && (cons[rear] == 0)) && ((trdcmsk[cons[carnmb]][i] == 1) && ((g_trdn_hall_call[(i-1)/32][cons[carnmb]] & fl_mask[i]) != 0)))
			downcb[i] = 1;

		if ((fvars[fvattcc] == 1) && (downcb[i] == 1) && (carcb[i] == 0) && (servf == s_ATT))	// on attandent
		{
			if((position != i) || ((procf >= 3) && (procf <= 11)))			
				set_carcall(cons[carnmb],i);
		}
  	}
  	if(cons[rear] != 0)
  	{
		for(i=1; i < cons[grtopf]; i++)
		{
			rupcb[i] = 0;
		    if( (rucmsk[cons[carnmb]][i]==1) && ((c_upr_hall_call[(i-1)/32] & fl_mask[i]) != 0) )
				rupcb[i] = 1;

		    if ((cons[Second_Riser] != 0) && ( (rirucmsk[cons[carnmb]][i]==1) && ((c_rirup_hall_call[(i-1)/32] & fl_mask[i]) != 0) ))
				rupcb[i] = 1;

			if ((fvars[fvattcc] == 1) && (rupcb[i] == 1) && (rcarcb[i] == 0) && (servf == s_ATT))	// on attandent
			{
				if((position != i) || ((procf >= 3) && (procf <= 11)))	
					set_rcarcall(cons[carnmb],i);
			}
		}
		for(i=2; i <= cons[grtopf]; i++)
		{
			rdowncb[i] = 0;
		    if( (rdcmsk[cons[carnmb]][i] == 1) && ((c_dnr_hall_call[(i-1)/32] & fl_mask[i]) != 0) )
				rdowncb[i] = 1;

		    if ((cons[Second_Riser] != 0) && ( (rirdcmsk[cons[carnmb]][i] == 1) && ((c_rirdn_hall_call[(i-1)/32] & fl_mask[i]) != 0) ))
				rdowncb[i] = 1;

			if ((fvars[fvattcc] == 1) && (rdowncb[i] == 1) && (rcarcb[i] == 0) && (servf == s_ATT))	// on attandent
			{
				if((position != i) || ((procf >= 3) && (procf <= 11)))	
					set_rcarcall(cons[carnmb],i);
			}
		}
  	}

  	return;
}

// ***************************************
// this is the hall call cancel procedure
// ***************************************
void hallcancel()
{
	int16 fl_ix;

	fl_ix = (position - 1)/32;
  	if((dpref != 2) && (position < cons[topf]))
  	{
//		if(( ((c_up_hall_call & fl_mask[position]) != 0) ||
//		    ((servf == s_AUTO) && (manual_door == 0) && (doorf != 0) && (closetry == 0)) )	&& (ncu_door_seq != 3))
		if ( ((c_up_hall_call[fl_ix] & fl_mask[position]) != 0) && (ncu_door_seq != 3))
		{
			if ((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))
				front_slowdown |= uc_do;	 
			if (((manual_door != 0) && (manual_door != 2)) || 
				((cons[pbdoormsk] != 0) && (pbdoor_fmsk[position] != 0)) || 
				((doorf != 0) && (closetry == 0) && 
	  				((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))))
			{
				cancel_up = position;
//				cancel_dn = 0;
			}
			else 
				cancel_up = 0;
		}
		else 
			cancel_up = 0;

  	}
	else
		cancel_up = 0;

  	if((dpref != 1) && (position != cons[bottomf]))
  	{
//		if( ((c_dn_hall_call & fl_mask[position]) != 0) ||
//		    ((servf == s_AUTO) && (manual_door == 0) && (doorf != 0) && (closetry == 0)) )
		if ((c_dn_hall_call[fl_ix] & fl_mask[position]) != 0)
		{
			if ((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))
				front_slowdown |= dc_do;
		    if (((manual_door != 0) && (manual_door != 2)) || 
				((cons[pbdoormsk] != 0) && (pbdoor_fmsk[position] != 0)) || 
		    	((doorf != 0) && (closetry == 0) && 
	  				((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))))
			{
				cancel_dn = position;
//				cancel_up = 0;
			}
			else
				cancel_dn = 0;
		}
		else 
			cancel_dn = 0;

  	}
	else
		cancel_dn = 0;

  	if(cons[rear] != 0)
  	{
	  	if((dpref != 2) && (position < cons[topf]))
	  	{
//			if( ((c_upr_hall_call & fl_mask[position]) != 0) ||
//			    ((servf == s_AUTO) && (manual_rdoor == 0) && (rdoorf != 0) && (rclosetry == 0)) )  
			if ((c_upr_hall_call[fl_ix] & fl_mask[position]) != 0) 
			{
				if ((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0))
					rear_slowdown |= uc_do;
			    if (((manual_rdoor != 0) && (manual_rdoor != 2)) || 
					((cons[pbdoormsk] != 0) && (pbdoor_rmsk[position] != 0)) || 
			    	((rdoorf != 0) && (rclosetry == 0) &&    
	  					((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0))))
				{
					cancel_upr = position;
//					cancel_dnr = 0;
				}
				else
					cancel_upr = 0;
			}
			else
				cancel_upr = 0;

	  	}
		else
			cancel_upr = 0;

	  	if((dpref != 1) && (position != cons[bottomf]))
	  	{
//			if( ((c_dnr_hall_call & fl_mask[position]) != 0) ||
//			    ((servf == s_AUTO) && (manual_rdoor == 0) && (rdoorf != 0) && (rclosetry == 0)) )
			if ((c_dnr_hall_call[fl_ix] & fl_mask[position]) != 0)
			{
				if ((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0))
					rear_slowdown |= dc_do;
			    if (((manual_rdoor != 0) && (manual_rdoor != 2)) || 
					((cons[pbdoormsk] != 0) && (pbdoor_rmsk[position] != 0)) || 
			    	((rdoorf != 0) && (rclosetry == 0) &&
	  					((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0))))
				{
					cancel_dnr = position;
//					cancel_upr = 0;
				}
				else
					cancel_dnr = 0;
		  	}
			else 
				cancel_dnr = 0;

		}
		else
			cancel_dnr = 0;
  	}


	if (cons[Second_Riser] != 0)
	{
	  	if((dpref != 2) && (position < cons[topf]))
	  	{
			if ((c_irup_hall_call[fl_ix] & fl_mask[position]) != 0)
			{
				if ((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))
					front_slowdown |= (uc_do | iruc_sd);
				if (((manual_door != 0) && (manual_door != 2)) || 
					((doorf != 0) && (closetry == 0) && 
		  				((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))))
				{
					cancel_irup = position;
				}
				else 
					cancel_irup = 0;
			}
			else 
				cancel_irup = 0;

	  	}
		else
			cancel_irup = 0;

	  	if((dpref != 1) && (position != cons[bottomf]))
	  	{
			if ((c_irdn_hall_call[fl_ix] & fl_mask[position]) != 0)
			{
				if ((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))
					front_slowdown |= (dc_do | irdc_sd);
			    if (((manual_door != 0) && (manual_door != 2)) || 
			    	((doorf != 0) && (closetry == 0) && 
		  				((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))))
				{
					cancel_irdn = position;
				}
				else
					cancel_irdn = 0;
			}
			else 
				cancel_irdn = 0;

	  	}
		else
			cancel_irdn = 0;

	  	if(cons[rear] != 0)
	  	{
		  	if((dpref != 2) && (position < cons[topf]))
		  	{
				if ((c_rirup_hall_call[fl_ix] & fl_mask[position]) != 0) 
				{
					if ((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0))
						rear_slowdown |= (uc_do | iruc_sd);
				    if (((manual_rdoor != 0) && (manual_rdoor != 2)) || 
				    	((rdoorf != 0) && (rclosetry == 0) &&    
		  					((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0))))
					{
						cancel_rirup = position;
					}
					else
						cancel_rirup = 0;
				}
				else
					cancel_rirup = 0;

		  	}
			else
				cancel_rirup = 0;

		  	if((dpref != 1) && (position != cons[bottomf]))
		  	{
				if ((c_rirdn_hall_call[fl_ix] & fl_mask[position]) != 0)
				{
					if ((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0))
						rear_slowdown |= (dc_do | irdc_sd);
				    if (((manual_rdoor != 0) && (manual_rdoor != 2)) || 
				    	((rdoorf != 0) && (rclosetry == 0) &&
		  					((nohcreopen == 0) || (onward_calls == 0) || (rdo_seq == 0))))
					{
						cancel_rirdn = position;
					}
					else
						cancel_rirdn = 0;
			  	}
				else 
					cancel_rirdn = 0;

			}
			else
				cancel_rirdn = 0;
	  	}
	}
	else
	{
		cancel_irup = 0;
		cancel_irdn = 0;
		cancel_rirup = 0;
		cancel_rirdn = 0;
	}
	
	if ((cons[Third_Riser] != 0) && (cons[rear] == 0))
	{
	  	if ((dpref != 2) && (position < cons[topf]))
	  	{
			if ((g_trup_hall_call[fl_ix][cons[carnmb]] & fl_mask[position]) != 0)
			{
				if ((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))
					front_slowdown |= (uc_do | iruc_sd);
				if (((manual_door != 0) && (manual_door != 2)) || 
					((doorf != 0) && (closetry == 0) && 
		  				((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))))
				{
					cancel_trup = position;
				}
				else 
					cancel_trup = 0;
			}
			else 
				cancel_trup = 0;
	  	}
		else
			cancel_trup = 0;

	  	if ((dpref != 1) && (position != cons[bottomf]))
	  	{
			if ((g_trdn_hall_call[fl_ix][cons[carnmb]] & fl_mask[position]) != 0)
			{
				if ((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))
					front_slowdown |= (dc_do | irdc_sd);
			    if (((manual_door != 0) && (manual_door != 2)) || 
			    	((doorf != 0) && (closetry == 0) && 
		  			 ((nohcreopen == 0) || (onward_calls == 0) || (do_seq == 0))))
				{
					cancel_trdn = position;
				}
				else
					cancel_trdn = 0;
			}
			else 
				cancel_trdn = 0;
	  	}
		else
			cancel_trdn = 0;
	}
	else
	{
		cancel_trup = 0;
		cancel_trdn = 0;
		cancel_rtrup = 0;
		cancel_rtrdn = 0;
	}
  	return;
}


/* Revision History
9/23/03 v3.08 mhd	1. Deleted check for (firef == 0) to allow setting a car call.
10/2/03 v3.09 mhd	1. User hall call sercom only when bit 0 is set.
12/1/03 v3.14 mhd	1. Increased index for j from 20 to 29 to allow for 29 floors.
					2. Changed location of memory to allow for needed data space (see hmmap.h)
					3. On power up the call to init_call_data does not init the nmb_up_call and nmb_dn_call	array
					   so I deleted the "else" line of check for ram word != 0x76.
					   Also check each day for a 0x76 to see if the day was initialized instead of just on Sunday.
2/25/04 4.00 mhd	1. Change read and writes to battery backed and eeprom memory
					2. Moved Up_Date_Calls and Init_call_data to utils.cpp
3/21/05 4.08 mhd	1. Changed ccs[cons[grtopf]+1] to ccs[0]. Use the first locatin as the security enable input.
					2. Added seperate security enable input for the rear car calls rccs[0].
					3. Added Remote car call input.
4/24/05 v4.11 mhd	1. Use gc_rsec and gc_rcc_sec for rear car calls.	
4/21/05 v4.12 mhd	1. Added Sabbath service.
6/22/05 V4.23 mhd	1. Added local gettime, getdate, settime setdate so that the 18.2 msec DOS interrupt could be disabled.
6/27/05 v4.24 mhd	1. Change the ids_cc_sec and ids_rcc_sec to arrays for each car.
7/12/05 v4.24 mhd	2. Use group input gpi[] to select which floor are service during sabbath service.
					3. Added security override using ccs inputs.
9/6/05 v4.30 mhd	1. Sound handicap buzzer when car call button hit and hb_en flag set.
9/9/05 v4.31 mhd	1. Corrected sabbath service.
11/9/05 V4.39 mhd	1. Changed remote car call operation to switch between cop or remote or both.
11/29/05 v4.41 mhd	1. Added security recall floor and allow variable setting to latach remote cc or cop cc.
12/6/05 v4.42 mhd	1. Do not allow car calls from being set in set_cc when car on inspection or fire service.
2/24/06 v4.56 mhd	1. Added parameter for stop at lobby fvars[fvstplby].
4/4/06 v4.61 mhd	1. Add parameter to set car call if on attendant and hall call set.
4/26/06 v4.64 mhd	1. Allow hospital service to override security.
6/2/06 v4.69 mhd	1. In hall cancel, clear cancel flag for opposite direction only if call registered or door != 0 and manual door == 0.
8/10/06 v5.01 mhd	1. Do not increment nmbuc or nmbdc when the hall call is set.
8/31/06 v5.04 mhd	1. Added manual_rdoor to check rear hall call cancellation.
9/8/06 v5.04 mhd	2. Check the car_no index in set_carcall before using ccmsk[][] because if car_no == 0 program will crash.
11/8/06 v5.08 mhd	1. Set an onward call from a call at the floor that has not been nudged closed. This is to allow the door at 
                       a floor with a stuck button to close and answer a call from an opposite door or in the opposite direction.
					2. Cancel the car call only when the door has started to open except on fire service or manual doors. This is to
					   prevent the car call acknowledgement light from going out while the passenger is waiting for the opposite door
					   to open for non-simultaneous door operation.
					3. Set door open slowdown mask when hall call set to allow the door select routine to set a door request if an opposite
					   door needs to open during non-simultaneous door operation.  Also cancel the hall call when the door starts to open
					   except for manual door opeation.
1/3/07 v5.11 mhd	1. In the set car call (front and rear) routines, set car call only if valid floors.
1/12/07 v5.12 mhd	1. Use field variable to disable single automatic push button operation. i.e. allow multiple car calls when variable set
                       to 2 and to enable SAPB op if simplex car and variable set to 1. 
2/15/07 v5.18 mhd 	1. Made arrays out of variables that used one bit for floor from 1 32 word to 2.
					2. Made the floor mask large enough for 60 floors.
3/1/07 v5.19 mhd	1. In set_upcall, set_dncall etc, do not set the upcb[] or downcb[] because this is a car array.
					2. Also use fl_mask[] in hallcall() routine and change the loop index to 1 to cons[grtopf] so that the upcb and downcb arrays
					   get cleared at invalid floors.
4/3/07 v5.22 mhd	1. Modify clrcc routine to clear all floors if select parameter == 0 or clear only invalid floor is select == 1.
6/25/07 v5.28 mhd	1. Cancel car call without checking door operations if cons[pbdoor] != 0.
7/3/07 v5.29 mhd	1. Added car assigned floor security from group.
8/6/07 v5.31 mhd	1. Added variable fvnohcreop to prevent hall call from reopening when set and onward hall call.
11/14/07 v5.39 pn	1. Added car call test.
11/28/07 v5.39.4 mhd	1. Use lobby_fl variable instead of fvars[fvlob] so that lobby_fl can be changed to alternate lobby fvars[fvaltlob]. 	
1/25/08 v5.40.3 mhd	1. Don't do behind car call cancel if on fire service. 
4/15/08 v5.43.3 pn	1. Changed "cons[ids] == 1" check to "((cons[ids] & 1) !=0)" since I added
					   an option for cons[ids] bit 2 for adding an additional GPIO board for 
					   montoring of external signals through lift-net.
4/18/08 v5.43.6 md	1. Moved stop at lobby to subroutine and modified it to look a car call above and below the lobby.
8/19/08 v5.45.6 mhd	1. Added Independent Service security operation in carcall() routine.
 					2. Set Sabbath floor mask from cons file for car sabbath service.
10/2/09 v5.46 mhd	1. Added behind car call cancel variable.
					2. Do not set the uc_do or dc_do flags in slowdown if the car cannot reopen the hall door.
10/16/08 v5.46.1 mhd	1. Changed set_carcall, and set_rcarcall to check the for valid floor in the beginning of the if 
						   statememt otherwise an invalid floor can cause the cpu to reset.
1/10/09 v5.47.2 mhd	1. Change hall call board numbers for simulator software to allow for the serial hall call boards to have
					   slightly different addressing.
2/02/09 v5.47.4 pn	1.  Added cons[sabbath] = 4.  Will only answer car calls in the 
						down direction and then express to the top floor to repeat the
						process.  It will also take the car out of group service.
2/10/09 v5.47.6 mhd	1. Corrected check in carcall() routine for ((cons[ids] & 1) == 0) instead of ((cons[ids] & 1) != 0).
					2. Placed setting of car call latch in cclatch() taken from carcall().
9/16/2009 v5.50.1 hdl	1. Added next_stop_lobby() routine to make the stop at lobby field variable (0x08) work.
9/28/09 v5.50.3 mhd		1. Added sabbath service = 5 to stop at odd or even floors up or down.
10/15/09 v5.50.4 mhd	1. Added Galileo car call lockouts.
10/21/09 v5.50.5 mhd	1. Corrected logic for next_stop_lobby() routine.
3/23/10 v5.51.16 mhd	1. Added Sabbath_start_at_lobby() routine.

5/14/10 v5.52.7 hdl		1. Added logic for cons[hben] == 4 for having the car call buzzer but no passing floor handicap buzzer

6/7/10 v5.52.9 mhd		1. Seperated car call security check to set master_cc_sec[] flag so that car call board can flash
						   light if car call secured from any security.

8/10/10 v5.22.22 pn/hdl	1.	nmb_carcalls = 0 in clrcc(). The Load weigher LWA input will clear the car calls and they will not be able to latch again
							unless the car went to a different landing 

9/24/10 5.52.33	hdl		1. Changed the logic for fvattcc parameter. "CC from HC" It only will work if the car is not at the same position where the car
							call is being pressed. If we had two calls in the same landing, the car will lock with the doors open.
10/27/10 5.52.40 mhd	1. Corrected rear car call security lockout rcc_sec_mask check from lcd display in cclatch().

11/6/10 v5.52.44 hdl	1. Added bit (fvars[fvattmandir] & 0x04) to allow the car only to start motion after the doors are closed and the car has a registered car call.
							if doors are not closed it will not allow calls to register. beginning of carcall();

1/10/11 v5.52.55 mhd/pn	1. In cclatch() allow hydro on emp to accept above car call if EMPop & 0x02 != 0.

2/2/11 v5.52.58	hdl		1. Allowed Remote car station to override security when used with cons[Prison]

3/17/11 v5.53.09 hdl	1. Changed the fl_ix_correction for the gccs under cclatch. The check was incorrect for the 32nd floor. We were looking at the wrong array
							fl_ix_correction is the same as fl_ix except it corrects the reading of security for the 32nd landing (high rise)

5/2/11 v5.53.18	mhd		1. Modified cclatch() to OR gc_sec with s_gcc_lockouts and ccs with s_cc_lockouts so that service activation timers work properly with security car call lockouts.
						2. Added tmr_svc_en[] for group and car lockout overrides so the service timer would work properly and stay backward compatible.

5/31/11 v5.54.02 mhd	1. Changed fl_ix_correction to fl_ix_gcc, set logic to i/32 and uss also for rear gcc security.

9/13/11 v5.54.24 hdl 	1. Changed the fvnohcreopen to nohcreopen to allow up peak to changed this logic
4/12/12 v6,05 hdl/mhd	1. Added code for remote car call to seperate front and rear using f_rm_io_sel and r_rm_io_sel.
5/25/12 v5.54.83 hdl	1. sab_cc_topff was added un utilities to be utilized in the down collective sabbath instead of the cons top floor. 
		v6.0.12 mhd 		This prevents the car from going to top floor if not set for it.
7/16/2013 v7.00.12 pn	1. Fixed front and rear car call security.  The master security input was being 
						   used incorrectly.  Instead of i_SECUR input it was looking for i_CCS(0) for the master
						   security input.  Same for the rear car call security.

12/26/12 V6.0.27 mhd	1. Created car variables for lby_req, g_park, g_up_hall_call, g_dn_hall_call ... g_rirdn_hall_call so that when the car cleared its variable
						   it would not affect the group variables.
****						   
1/4/13 v5.55.15 mhd		1. Added Riot Control service when cons[prison] == 2, servf = s_RIOT.
1/7/13 v5.55.21 hdl		1. Added the above change to the controller software galaxy 3						   
2/19/13 v6.0.31 hdl		1. Added the above change to the controller software galaxy 3S					   

3/22/13 v5.55.36 as		1. Modified routine sabbath_std_svc() to not allow sabbath car calls when fvars[fvccsonsab] car call lockouts on sabbath operation is enabled						   

4/17/13 v5.55.40 as		1. Added sab_up_dn_col_cc_msk_swap() routine to allow sabbath up/dn collective calls with the general perpuse io board.

5/30/13 v6.00.41 as		1. Added below Galaxy III changes made in v5.55.41 - v5.55.43
						2. Added set_ir_upcall() and set_ir_dncall() routines for setting front inconspicious up down calls
						3. Modified cclatch() to set master_cc_sec when gc_ccs_per_fl group car call security flag is set for (cons[SecFlCfg] == 7) and (fvars[fvccpbsongpccsec] == 1)
						4. Modified set_ir_upcall() and set_ir_dncall() routines to only accept IRU, IRD video command calls when allowed
***

8/13/13 v7.0.16 mhd		1. Added above changes from G3 V6.0.49.***
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 where necessary to allow any car to be the group.
						2. Replaced iodata[1] with iodata[cons[carnmb]] where necessary.

5/29/14 v7.1.2 mhd		1. Added c_cc_req[] and c_rcc_req[] car call request from group in to cclatch().

10/9/14 v7.1.16 mhd		1. Modified changes from galaxy 3 version 6.0.53 to 6.0.60.  See below:

						 v6.00.57 as		1. Modified cclatch() to add logic for Second Riser car call security by direction											
						 v6.00.59 as		1. Modified cclatch() to clear hsvf hospital service flag if a call is latched on the floor where smoke sensor was activated.
12/16/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
							10/7/14 v6.01.87 as		1. Added below changes related to G3 software:
													09/26/14 v6.01.87 cl 	1. Added fvars[fvrmccd] to disable Remote car call or COP when in the up or down direction.
							10/21/14 v6.01.92 as	1. Modified hallcancel() and carcancel() routines to allow car call and hall call to cancel when cons[pbdoormsk] is set coming at a floor.
							11/24/14 v6.2.01 as		1. Added below changes made related to pulsing of CL outputs:
													11/25/14 cl/as 	1. Added logic to pulse output CL when security_type is +64
11/09/15 v7.1.46 tb		1. Modified set_carcall() routine to return 1 if the car call was latched and 0 if it was not.
01/07/16 v7.1.60 as		1. Changed set_rupcall() and set_rdncall() routines to work similar as set_upcall() and set_dncall().
*/
