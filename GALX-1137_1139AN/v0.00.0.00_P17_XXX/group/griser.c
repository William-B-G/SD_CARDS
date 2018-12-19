#define d_GRISER 1
#include "global.h"


int16 ir_car=0;	// inconspicious riser car
int16 ir_active = 0;
int16 ir_select = 0;
int16 prev_ir_active = 0;
int16 ir_call_enable= 0;
int16 tr_call_enable = 0;
int16 sr_car = 0;
int16 tr_car = 0;


int32 g_irup_hall_call[2][car_size];
int32 g_irdn_hall_call[2][car_size];
int32 g_rirup_hall_call[2][car_size];
int32 g_rirdn_hall_call[2][car_size];

int32 g_trup_hall_call[2][car_size];
int32 g_trdn_hall_call[2][car_size];
int32 g_rtrup_hall_call[2][car_size];
int32 g_rtrdn_hall_call[2][car_size];

int16 g_asign_irup[fl_size];
int16 g_asign_irdn[fl_size];
int16 g_asign_rirup[fl_size];
int16 g_asign_rirdn[fl_size];

int16 g_asign_trup[fl_size];
int16 g_asign_trdn[fl_size];
int16 g_asign_rtrup[fl_size];
int16 g_asign_rtrdn[fl_size];

int16 sr_cc_ctrl[car_size];	// Second Riser car call control
int16 Car_on_SR[car_size];		// Car on Second Riser only operation (do not assign from std riser) 
int16 Valid_SR_Car[car_size];		// Valid car for second riser operation
int16 Valid_SR_Car_Backup[car_size];

// third riser
int16 tr_cc_ctrl[car_size];		// Third Riser car call control
int16 Car_on_TR[car_size];		// Car on Third Riser only operation (do not assign calls from std riser) 
int16 Valid_TR_Car[car_size];	// Valid car for third riser operation


uint32 uc_canc_hold[2] = {0,0};
uint32 dc_canc_hold[2] = {0,0};
uint32 ruc_canc_hold[2] = {0,0};
uint32 rdc_canc_hold[2] = {0,0};

uint32 uc_dsr_canc_hold[2] = {0,0};
uint32 dc_dsr_canc_hold[2] = {0,0};
uint32 ruc_dsr_canc_hold[2] = {0,0};
uint32 rdc_dsr_canc_hold[2] = {0,0};


int16 get_any_ir_hall_call (void);
void grp_clrir(void);
void grp_clrir_canc();
void grp_clrirhc();
void latch_ir_hallcall(void);
void latch_sr_hallcall(void);
void latch_tr_hallcall(void);
void assign_sr_hallcall (void);
void assign_tr_hallcall (void);
void group_ir_hallcancel(void);
void group_sr_hallcancel();
void group_tr_hallcancel();
void ir_control (void);
void ir_dual_riser (void);
void sr_cycling_operation(void);

#if (Tract_HR == 1)
void dual_serial_riser (void);
#endif


//**************************************
// Routine to check for any ir hall call
//**************************************
int16 get_any_ir_hall_call (void)
{
	int16 i;
	any_ir_hall_call = 0;
	for (i=0;i<=1;i++)
	{
		if (((gc_iruc[i] & g_irucmsk[i]) != 0) || ((gc_irdc[i] & g_irdcmsk[i]) != 0) || 
			((gc_riruc[i] & g_rirucmsk[i]) != 0) || ((gc_rirdc[i] & g_rirdcmsk[i]) != 0))
			any_ir_hall_call = 1;
	}
	return any_ir_hall_call;
}

// **********************************************
// This is the group clear ir hall call procedure
// **********************************************
void grp_clrir()
{
	gc_iruc_canc[0] = 0xFFFFFFFFL;
	gc_irdc_canc[0] = 0xFFFFFFFFL;
	gc_iruc_canc[1] = 0xFFFFFFFFL;
	gc_irdc_canc[1] = 0xFFFFFFFFL;
  	if(grtop_rfl != 0)
  	{
		gc_riruc_canc[0] = 0xFFFFFFFFL;
		gc_rirdc_canc[0] = 0xFFFFFFFFL;
		gc_riruc_canc[1] = 0xFFFFFFFFL;
		gc_rirdc_canc[1] = 0xFFFFFFFFL;
	}
}

// ******************************************************
// This is the group clear ir hall call cancel procedure
// ******************************************************
void grp_clrir_canc()
{
	gc_iruc_canc[0] = 0;
	gc_irdc_canc[0] = 0;
	gc_iruc_canc[1] = 0;
	gc_irdc_canc[1] = 0;
  	if(grtop_rfl != 0)
  	{
		gc_riruc_canc[0] = 0;
		gc_rirdc_canc[0] = 0;
		gc_riruc_canc[1] = 0;
		gc_rirdc_canc[1] = 0;
	}
}


// **********************************************
// This is the group clear ir hall call procedure
// **********************************************
void grp_clrirhc()
{
  	int16 i;
  	// clr ir up
  	for(i = 1; i < cons[grtopf]; i++)
		g_asign_irup[i] = 0;
	
	gc_iruc_canc[0] = 0xFFFFFFFFL;
	gc_iruc_canc[1] = 0xFFFFFFFFL;
#if (Simulator == 1)
	gc_iruc[0] = 0;
	gc_iruc[1] = 0;
#endif
	
	g_irup_hall_call[0][0] = 0;
	g_irup_hall_call[1][0] = 0;
	for (i=1;i<car_size;i++)
	{
  		g_irup_hall_call[0][i] = 0;
  		g_irup_hall_call[1][i] = 0;
  	}

	// clr ir down
  	for(i = 2; i <= cons[grtopf]; i++)
	   	g_asign_irdn[i] = 0;
		  
	gc_irdc_canc[0] = 0xFFFFFFFFL;
	gc_irdc_canc[1] = 0xFFFFFFFFL;
#if (Simulator == 1)
	gc_irdc[0] = 0; 
	gc_irdc[1] = 0;
#endif
	
  	g_irdn_hall_call[0][0] = 0;
  	g_irdn_hall_call[1][0] = 0;
	for (i=1;i<car_size;i++)
	{
		g_irdn_hall_call[0][i] = 0;
		g_irdn_hall_call[1][i] = 0;
	}
	
	// has rear
  	if (grtop_rfl != 0)	
	{	
		// clr ir rear up
	  	for(i = 1; i < cons[grtopf]; i++)
			g_asign_rirup[i] = 0;
		
		gc_riruc_canc[0] = 0xFFFFFFFFL;
		gc_riruc_canc[1] = 0xFFFFFFFFL;
#if (Simulator == 1)
		gc_riruc[0] = 0;
		gc_riruc[1] = 0;
#endif
		
		g_rirup_hall_call[0][0] = 0;
		g_rirup_hall_call[1][0] = 0;
		for (i=1;i<car_size;i++)
		{
			g_rirup_hall_call[0][i] = 0;
			g_rirup_hall_call[1][i] = 0;
		}
		
		// clr ir rear down
	 	for(i = 2; i <= cons[grtopf]; i++)
			g_asign_rirdn[i] = 0;
			  
		gc_rirdc_canc[0] = 0xFFFFFFFFL;
		gc_rirdc_canc[1] = 0xFFFFFFFFL;
#if (Simulator == 1)
		gc_rirdc[0] = 0; 
		gc_rirdc[1] = 0;
#endif
		
		g_rirdn_hall_call[0][0] = 0;
		g_rirdn_hall_call[1][0] = 0;
		for (i=1;i<car_size;i++)
		{
			g_rirdn_hall_call[0][i] = 0;
			g_rirdn_hall_call[1][i] = 0;
		}
		
	}

  	return;
}


//****************************************************************
// This routine latches the IR Hall Calls from Serial Hall Call Board
//****************************************************************

void latch_ir_hallcall(void)
{

	int16 i;
	int16 fl_ix;

	any_ir_hall_call = 0;

	// Latch UP IR Hall Calls

  	for(i=1; i < cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
	    if((gc_iruc[fl_ix] & fl_mask[i]) != 0)// check to see if up hall call pushed
		{
			if (((fvars[fvdisopphc] & 0x04) != 0) && (sec_ir_ftimer[i] < fvars[fvdisopphctm]))
			{
				if ((g_up_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0)
					gc_iruc_canc[fl_ix] |= fl_mask[i];
			}
			else
			{
				if((Car_ots_nbp[ir_car] == 0) && (ucmsk[ir_car][i] == 1) && (ir_call_enable == 1))
				{
					if ((fvars[fvdisopphc] & 0x04) != 0)
					{	// when first ir up/down hall call is pressed clear sec_ir_ftimer
						if (((g_up_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0) && ((g_dn_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0))
							sec_ir_ftimer[i] = 0;
					}
					g_up_hall_call[fl_ix][ir_car] |= fl_mask[i];
				}
				else if (ucmsk[ir_car][i] == 0)
					gc_iruc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
				gc_iruc_set[fl_ix] &= ~fl_mask[i];	// reset the set ir up hall call flag 
				any_ir_hall_call = 1;
			}
		}
	    else
	    {
	    	if ((gc_iruc_canc[fl_ix] & fl_mask[i]) != 0)
				gc_iruc_set[fl_ix] &= ~fl_mask[i];
	    	gc_iruc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
	    }
		
		if (rear_op[ir_select] != 0)
		{
		    if((gc_riruc[fl_ix] & fl_mask[i]) != 0)// check to see if up hall call pushed
			{
				if (((fvars[fvdisopphc] & 0x08) != 0) && (sec_ir_rtimer[i] < fvars[fvdisopphctm]))
				{
					if ((g_upr_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0)
						gc_riruc_canc[fl_ix] |= fl_mask[i];
				}
				else
				{
					if((Car_ots_nbp[ir_car] == 0) && (rucmsk[ir_car][i] == 1) && (ir_call_enable == 1))
					{
						if ((fvars[fvdisopphc] & 0x08) != 0)
						{
							if (((g_upr_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0) && ((g_dnr_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0))
								sec_ir_rtimer[i] = 0;
						}
						g_upr_hall_call[fl_ix][ir_car] |= fl_mask[i];
					}
					else if (rucmsk[ir_car][i] == 0)
						gc_riruc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
					gc_riruc_set[fl_ix] &= ~fl_mask[i];	// reset the set ir up hall call flag 
					any_ir_hall_call = 1;
				}
			}
			else
			{
				if ((gc_riruc_canc[fl_ix] & fl_mask[i]) != 0)
					gc_riruc_set[fl_ix] &= ~fl_mask[i];
				gc_riruc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			}
	    }
  	}

	// Latch the Down IR Hall Calls

  	for(i=2; i <= cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
      	if((gc_irdc[fl_ix] & fl_mask[i]) != 0)  //check to see if dn hall call pushed
		{
			if (((fvars[fvdisopphc] & 0x04) != 0) && (sec_ir_ftimer[i] < fvars[fvdisopphctm]))
			{
				if ((g_dn_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0)
					gc_irdc_canc[fl_ix] |= fl_mask[i];
			}
			else
			{
				if((Car_ots_nbp[ir_car] == 0) && (dcmsk[ir_car][i] == 1) && (ir_call_enable == 1))
				{
					if ((fvars[fvdisopphc] & 0x04) != 0)
					{	// when first ir up/down call is pressed clear front ir security timer
						if (((g_up_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0) && ((g_dn_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0))
							sec_ir_ftimer[i] = 0;
					}
					g_dn_hall_call[fl_ix][ir_car] |= fl_mask[i];
				}
				else if (dcmsk[ir_car][i] == 0)
					gc_irdc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
				gc_irdc_set[fl_ix] &= ~fl_mask[i];	// reset the set ir down hall call flag
				any_ir_hall_call = 1;
			}
		}
      	else
		{
			if ((gc_irdc_canc[fl_ix] & fl_mask[i]) != 0)
				gc_irdc_set[fl_ix] &= ~fl_mask[i];
			gc_irdc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
		}
		
      	if (rear_op[ir_select] != 0)
      	{
	      	if((gc_rirdc[fl_ix] & fl_mask[i]) != 0)  //check to see if dn hall call pushed
			{
				if (((fvars[fvdisopphc] & 0x08) != 0) && (sec_ir_rtimer[i] < fvars[fvdisopphctm]))
				{
					if ((g_dnr_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0)
						gc_rirdc_canc[fl_ix] |= fl_mask[i];
				}
				else
				{
					if((Car_ots_nbp[ir_car] == 0) && (rdcmsk[ir_car][i] == 1) && (ir_call_enable == 1))
					{
						if ((fvars[fvdisopphc] & 0x08) != 0)
						{
							if (((g_upr_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0) && ((g_dnr_hall_call[fl_ix][ir_car] & fl_mask[i]) == 0))
								sec_ir_rtimer[i] = 0;
						}
						g_dnr_hall_call[fl_ix][ir_car] |= fl_mask[i];
					}
					else if (rdcmsk[ir_car][i] == 0)
						gc_rirdc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
					gc_rirdc_set[fl_ix] &= ~fl_mask[i];	// reset the set ir down hall call flag
					any_ir_hall_call = 1;
				}
			}
	      	else
	      	{
	      		if ((gc_rirdc_canc[fl_ix] & fl_mask[i]) != 0)
					gc_rirdc_set[fl_ix] &= ~fl_mask[i];
				gc_rirdc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
	      	}
      	}
  	}
}

//****************************************************************
// This routine latches the IR Hall Calls from Serial Hall Call Board
//****************************************************************

void latch_sr_hallcall(void)
{

	int16 i;
	int16 fl_ix;

	// Latch UP IR Hall Calls

  	for(i=1; i < cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
	    if((gc_iruc[fl_ix] & fl_mask[i]) != 0)// check to see if up hall call pushed
		{
			if (((Grp_HC_Lockouts == 1) && (fvars[fvhcsecctrl] != 0) && (gc_hsec == 1) && ((gc_hc_sec[fl_ix] & fl_mask[i]) != 0)) ||
				(((fvars[fvdisopphc] & 0x04) != 0) && (sec_ir_ftimer[i] < fvars[fvdisopphctm])))
			{
				if ((g_irup_hall_call[fl_ix][0] & fl_mask[i]) == 0) 								
					gc_iruc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
			}
			else if (ir_call_enable == 1)
			{
				if ((fvars[fvdisopphc] & 0x04) != 0)
				{
					if (((g_irup_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((g_irdn_hall_call[fl_ix][0] & fl_mask[i]) == 0))
						sec_ir_ftimer[i] = 0;
				}
				g_irup_hall_call[fl_ix][0] |= fl_mask[i];
			}
			else 
				gc_iruc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
			gc_iruc_set[fl_ix] &= ~fl_mask[i];
		}
	    else
		{
			if ((gc_iruc_canc[fl_ix] & fl_mask[i]) != 0)
				gc_iruc_set[fl_ix] &= ~fl_mask[i];
			gc_iruc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			if((g_irup_hall_call[fl_ix][0] & fl_mask[i]) == 0)
				g_asign_irup[i] = 0;
		}

		if (grtop_rfl != 0)
		{
		    if((gc_riruc[fl_ix] & fl_mask[i]) != 0)// check to see if up hall call pushed
			{
				if (((Grp_HC_Lockouts == 1) && (fvars[fvhcsecctrl] != 0) && (gc_rhsec == 1) && ((gc_rhc_sec[fl_ix] & fl_mask[i]) != 0)) ||
					(((fvars[fvdisopphc] & 0x08) != 0) && (sec_ir_rtimer[i] < fvars[fvdisopphctm])))
				{
					if ((g_rirup_hall_call[fl_ix][0] & fl_mask[i]) == 0)
						gc_riruc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
				}
				else if (ir_call_enable == 1)
				{
					if ((fvars[fvdisopphc] & 0x08) != 0)
					{	// when first rear ir up/down hc is pressed clear sec_ir_rtimer
						if (((g_rirup_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((g_rirdn_hall_call[fl_ix][0] & fl_mask[i]) == 0))
							sec_ir_rtimer[i] = 0;
					}
					g_rirup_hall_call[fl_ix][0] |= fl_mask[i];
				}
				else
					gc_riruc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
				gc_riruc_set[fl_ix] &= ~fl_mask[i];
			}
			else
			{
				if ((gc_riruc_canc[fl_ix] & fl_mask[i]) != 0)
					gc_riruc_set[fl_ix] &= ~fl_mask[i];
				gc_riruc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
				if((g_rirup_hall_call[fl_ix][0] & fl_mask[i]) == 0)
					g_asign_rirup[i] = 0;
			}
	    }
  	}

	// Latch the Down IR Hall Calls

  	for(i=2; i <= cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
      	if ((gc_irdc[fl_ix] & fl_mask[i]) != 0)  //check to see if dn hall call pushed
		{
			if (((Grp_HC_Lockouts == 1) && (fvars[fvhcsecctrl] != 0) && (gc_hsec == 1) && ((gc_hc_sec[fl_ix] & fl_mask[i]) != 0)) || 
				(((fvars[fvdisopphc] & 0x04) != 0) && (sec_ir_ftimer[i] < fvars[fvdisopphctm])))
			{
				if ((g_irdn_hall_call[fl_ix][0] & fl_mask[i]) == 0) 			
					gc_irdc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
			}
			else if (ir_call_enable == 1)
			{
				if ((fvars[fvdisopphc] & 0x04) != 0)
				{
					if (((g_irdn_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((g_irup_hall_call[fl_ix][0] & fl_mask[i]) == 0))
						sec_ir_ftimer[i] = 0;
				}
				g_irdn_hall_call[fl_ix][0] |= fl_mask[i];
			}
			else
				gc_irdc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
			gc_irdc_set[fl_ix] &= ~fl_mask[i];
		}
      	else
		{
			if ((gc_irdc_canc[fl_ix] & fl_mask[i]) != 0)
				gc_irdc_set[fl_ix] &= ~fl_mask[i];
			gc_irdc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			if((g_irdn_hall_call[fl_ix][0] & fl_mask[i]) == 0)
				g_asign_irdn[i] = 0;
		}

		if (grtop_rfl != 0)
      	{
	      	if ((gc_rirdc[fl_ix] & fl_mask[i]) != 0)  //check to see if dn hall call pushed
			{
				if (((Grp_HC_Lockouts == 1) && (fvars[fvhcsecctrl] != 0) && (gc_rhsec == 1) && ((gc_rhc_sec[fl_ix] & fl_mask[i]) != 0)) ||
					(((fvars[fvdisopphc] & 0x08) != 0) && (sec_ir_rtimer[i] < fvars[fvdisopphctm])))
				{
					if ((g_rirdn_hall_call[fl_ix][0] & fl_mask[i]) == 0) 
						gc_rirdc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
				}
				else if (ir_call_enable == 1)
				{
					if ((fvars[fvdisopphc] & 0x08) != 0)
					{
						if (((g_rirdn_hall_call[fl_ix][0] & fl_mask[i]) == 0) && ((g_rirup_hall_call[fl_ix][0] & fl_mask[i]) == 0))
							sec_ir_rtimer[i] = 0;
					}
					g_rirdn_hall_call[fl_ix][0] |= fl_mask[i];
				}
				else
					gc_rirdc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
				gc_rirdc_set[fl_ix] &= ~fl_mask[i];
			}
	      	else
			{
				if ((gc_rirdc_canc[fl_ix] & fl_mask[i]) != 0)
					gc_rirdc_set[fl_ix] &= ~fl_mask[i];
				gc_rirdc_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
				if((g_rirdn_hall_call[fl_ix][0] & fl_mask[i]) == 0)
					g_asign_rirdn[i] = 0;
			}
      	}
  	}
}

//******************************************************
// This routine latches the TR Third Riser Hall Calls
//******************************************************

void latch_tr_hallcall(void)
{

	int i;
	int fl_ix;

	// Latch the UP TR Hall Calls

  	for (i=1; i<cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;

	    if ((gc_truc[fl_ix] & fl_mask[i]) != 0)		// check to see if up hall call pushed
		{
			if ((Grp_HC_Lockouts == 1) && (fvars[fvhcsecctrl] != 0) && (gc_hsec == 1) && ((gc_hc_sec[fl_ix] & fl_mask[i]) != 0)) 
			{
				if ((g_trup_hall_call[fl_ix][0] & fl_mask[i]) == 0) 								
					gc_truc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
			}
			else if (tr_call_enable == 1)
				g_trup_hall_call[fl_ix][0] |= fl_mask[i];	// assign the call
			else
				gc_truc_canc[fl_ix] |= fl_mask[i];
		}
	    else
		{
			gc_truc_canc[fl_ix] &= ~fl_mask[i];		// Clear the cancel flag
			if ((g_trup_hall_call[fl_ix][0] & fl_mask[i]) == 0)
				g_asign_trup[i] = 0;
		}
  	}

	// Latch the Down TR Hall Calls

  	for (i=2; i<=cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
		
      	if ((gc_trdc[fl_ix] & fl_mask[i]) != 0)		//check to see if dn hall call pushed
		{
			if ((Grp_HC_Lockouts == 1) && (fvars[fvhcsecctrl] != 0) && (gc_hsec == 1) && ((gc_hc_sec[fl_ix] & fl_mask[i]) != 0)) 
			{
				if ((g_trdn_hall_call[fl_ix][0] & fl_mask[i]) == 0) 			
					gc_trdc_canc[fl_ix] |= fl_mask[i];			// call cannot be assigned
			}
			else if (tr_call_enable == 1)
				g_trdn_hall_call[fl_ix][0] |= fl_mask[i];	// assign the call
			else
				gc_trdc_canc[fl_ix] |= fl_mask[i];
		}
      	else
		{
			gc_trdc_canc[fl_ix] &= ~fl_mask[i];		// Clear the cancel flag
			if ((g_trdn_hall_call[fl_ix][0] & fl_mask[i]) == 0)
				g_asign_trdn[i] = 0;
		}
	}
}




// ************************************************************
// This routine assigns the ir hall call to the appropriate car
// ************************************************************

void assign_sr_hallcall (void)
{
  	int16 i, k, assignment=0,rear_assignment=0;
	int16 fl_ix=0;

	nmb_asg_iruc = 0;
	nmb_asg_irdc = 0;

  	for(i=1; i<=cons[grtopf]; i++)        // decide who will take the calls
  	{
		fl_ix = (i-1)/32;
	    if(i < cons[grtopf])
	    {
	      	if((g_irup_hall_call[fl_ix][0] & fl_mask[i]) != 0)  // there is an up call
	      	{
				assignment = 0;
				for(k=cons[nmbcars]; k>=1; k--)
				{       // find the first elegible car to answer the call
				  	if ((Car_ots[k] == 0) && (irucmsk[k][i] == 1))
						assignment = k;
				}
				for(k=(assignment + 1); k<=cons[nmbcars]; k++)
				{       // find the car that can answer the call the fastest
				  	if ((Car_ots[k] == 0) && (irucmsk[k][i] == 1))
				  	{     // if it is inservice and there is an opening compare eta	
						if (make_assignment(assignment,k,i,1,0) == 1)
							assignment = k;
				  	}
				}

				if((assignment != g_asign_irup[i]) && (g_asign_irup[i] != 0) && (irucmsk[g_asign_irup[i]][i] == 1) && (Car_ots[g_asign_irup[i]] == 0) )  // assigning to a diffrent car
				{             // checking to see if its the old cars next stop 
					if (keep_assignment(g_asign_irup[i],assignment,i,1,0) == 1)
						assignment = g_asign_irup[i];
				}// if it was the old cars next stop let old car take it

				for(k=1; k<=cons[nmbcars]; k++)
				{
			    	if(k == assignment)
						g_irup_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
			    	else 
						g_irup_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
				}
				if (cons[Second_Riser] == 2)
				{
				 	if ((gc_ir == 0) && (gc_ir_2 == 0))
						assignment = 0;
				}
				else if (gc_ir == 0)
					assignment = 0;
				if (assignment == 0)   
				{
		    		g_irup_hall_call[fl_ix][0] &= ~fl_mask[i];// cancel call from group
					

			    	for(k=1; k<=cons[nmbcars]; k++)
						g_up_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1

					gc_iruc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//					gc_iruc[fl_ix] &= ~fl_mask[i];			// cancel call input
				}
				g_asign_irup[i] = assignment;
 				if (g_asign_irup[i] != 0)
					nmb_asg_iruc++;
     		}
      		else
      		{
				for(k=1; k<=cons[nmbcars]; k++)
	    	  		g_irup_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
      		}
    	}
    	if(i > 1)
    	{
		  	if((g_irdn_hall_call[fl_ix][0] & fl_mask[i]) != 0)      // there is an down call
		  	{
				assignment = 0;
				for(k=cons[nmbcars]; k>=1; k--)
				{       // find the first elegible car to answer the call
			  		if ((Car_ots[k] == 0) && (irdcmsk[k][i] == 1))
						assignment = k;
				}
				for(k=(assignment + 1); k<=cons[nmbcars]; k++)
				{       // find the car that can answer the call the fastest
			  		if ((Car_ots[k] == 0) && (irdcmsk[k][i] == 1))
			  		{     // if it is inservice and there is an opening compare eta
						if (make_assignment(assignment,k,i,2,0) == 1)
							assignment = k;
			  		}
				}

				// security option to force assignment calls to specific elevators
				if((assignment != g_asign_irdn[i]) && (g_asign_irdn[i] != 0) && (irdcmsk[g_asign_irdn[i]][i] == 1) && (Car_ots[g_asign_irdn[i]] == 0))  // assigning to a diffrent car
				{             // checking to see if its the old cars next stop
					if (keep_assignment(g_asign_irdn[i],assignment,i,2,0) == 1)
						assignment = g_asign_irdn[i];
				}// if it was the old cars next stop let old car take it

				for(k=1; k<=cons[nmbcars]; k++)
				{
			    	if(k == assignment)
						g_irdn_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
			    	else
						g_irdn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel the other cars
				}
				if (cons[Second_Riser] == 2)
				{
				 	if ((gc_ir == 0) && (gc_ir_2 == 0))
						assignment = 0;
				}
				else if (gc_ir == 0)
					assignment = 0;
				if (assignment == 0)  
				{
		   		 	g_irdn_hall_call[fl_ix][0] &= ~fl_mask[i];// cancel call from group

			    	for(k=1; k<=cons[nmbcars]; k++)
						g_irdn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1

					gc_irdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
					gc_irdc[fl_ix] &= ~fl_mask[i];			// cancel call input
				}
				g_asign_irdn[i] = assignment;
				if (g_asign_irdn[i] != 0)
					nmb_asg_irdc++;
		  	}
		  	else
		  	{
				for(k=1; k<=cons[nmbcars]; k++)
			      	g_irdn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
		  	}
	    }
			   
	    if(grtop_rfl != 0)
	    {
		  	if(i < cons[grtopf])
		  	{
		      	if((g_rirup_hall_call[fl_ix][0] & fl_mask[i]) != 0)  // there is an up call
		      	{
					rear_assignment = 0;
					for(k=cons[nmbcars]; k>=1; k--)
					{       // find the first elegible car to answer the call
				  		if ((Car_ots[k] == 0) && (rirucmsk[k][i] == 1))
							rear_assignment = k;
					}
					for (k=(rear_assignment + 1); k<=cons[nmbcars]; k++)
					{       // find the car that can answer the call the fastest
				  		if ((Car_ots[k] == 0) && (rirucmsk[k][i] == 1))
				  		{     // if it is inservice and there is an opening compare eta
					  		if (make_assignment(rear_assignment,k,i,1,1)	== 1)
								rear_assignment = k;
				  		}
					}

					if ((rear_assignment != g_asign_rirup[i]) && (g_asign_rirup[i] != 0) && (rirucmsk[g_asign_rirup[i]][i] == 1) && (Car_ots[g_asign_rirup[i]] == 0))  // assigning to a diffrent car
					{             // checking to see if its the old cars next stop
						if (keep_assignment(g_asign_rirup[i],rear_assignment,i,1,1) == 1) 
							rear_assignment = g_asign_rirup[i];
					}// if it was the old cars next stop let old car take it

					for(k=1; k<=cons[nmbcars]; k++)
					{
				    	if (k == rear_assignment)
							g_rirup_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
				    	else 
							g_rirup_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
					}
					if (gc_rir == 0)
						rear_assignment = 0;
					if(rear_assignment == 0)// ||(car_eta[rear_assignment][i+(g_eta_1u-1)] == 0xff))
					{
				    	g_rirup_hall_call[fl_ix][0] &= ~fl_mask[i];// cancel call from group
				    	for(k=1; k<=cons[nmbcars]; k++)
							g_rirup_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1
						gc_riruc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//						gc_riruc[fl_ix] &= ~fl_mask[i];			// cancel call input
			  		}
					g_asign_rirup[i] = rear_assignment;
					if (g_asign_rirup[i] != 0)
						nmb_asg_iruc++;
	    	  	}
		      	else
		      	{
					for(k=1; k<=cons[nmbcars]; k++)
			      		g_rirup_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
		      	}
		    }
		    if(i > 1)
		    {
			  	if((g_rirdn_hall_call[fl_ix][0] & fl_mask[i]) != 0)      // there is an down call
			  	{
					rear_assignment = 0;
					for (k=cons[nmbcars]; k>=1; k--)
					{       // find the first elegible car to answer the call
				  		if ((Car_ots[k] == 0) && (rirdcmsk[k][i] == 1))
							rear_assignment = k;
					}
					for (k=(rear_assignment + 1); k<=cons[nmbcars]; k++)
					{       // find the car that can answer the call the fastest
				  		if ((Car_ots[k] == 0) && (rirdcmsk[k][i] == 1))
				  		{     // if it is inservice and there is an opening compare eta
							if (make_assignment(rear_assignment,k,i,2,1) == 1)
								rear_assignment = k;
				  		}
					}

					// security option to force assignment calls to specific elevators
					if((rear_assignment != g_asign_rirdn[i]) && (g_asign_rirdn[i] != 0)	&& (rirdcmsk[g_asign_rirdn[i]][i] == 1) && (Car_ots[g_asign_rirdn[i]] == 0)) // assigning to a diffrent car
					{             // checking to see if its the old cars next stop
						if (keep_assignment(g_asign_rirdn[i],rear_assignment,i,2,1) == 1)
							rear_assignment = g_asign_rirdn[i];
					}// if it was the old cars next stop let old car take it

					for(k=1; k<=cons[nmbcars]; k++)
					{
				    	if(k == rear_assignment)
							g_rirdn_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
				    	else  
							g_rirdn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel the other cars
					}

					if (gc_rir == 0)
						rear_assignment = 0;

					if (rear_assignment == 0) // || (grp_Car_Stat[rear_assignment][i+2] == 0xff))
					{
				    	g_rirdn_hall_call[fl_ix][0] &= ~fl_mask[i];// cancel call from group
				    	for(k=1; k<=cons[nmbcars]; k++)
							g_rirdn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1

						gc_rirdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//						gc_rirdc[fl_ix] &= ~fl_mask[i];			// cancel call input
			  		}
					g_asign_rirdn[i] = rear_assignment;
			  		if (g_asign_rirdn[i] != 0)
						nmb_asg_irdc++;
			  	}
			  	else
			  	{
					for(k=1; k<=cons[nmbcars]; k++)
					  	g_rirdn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
			  	}
		  	}
		}
  	}

  	return;
}

// ************************************************************
// This routine assigns the tr hall call to the appropriate car
// ************************************************************

void assign_tr_hallcall (void)
{
  	int16 i, k, assignment=0;
	int16 fl_ix=0;

	
 	for (i=1; i<=cons[grtopf]; i++)        // decide who will take the calls
  	{
		fl_ix = (i-1)/32;
	    if (i < cons[grtopf])
	    {
	      	if ((g_trup_hall_call[fl_ix][0] & fl_mask[i]) != 0)		// there is an up call
	      	{
				assignment = 0;
				for (k=cons[nmbcars]; k>=1; k--)
				{       // find the first elegible car to answer the call
				  	if ((Car_ots[k] == 0) && (trucmsk[k][i] == 1))
						assignment = k;
				}

				for (k=(assignment + 1); k<=cons[nmbcars]; k++)
				{       // find the car that can answer the call the fastest
				  	if ((Car_ots[k] == 0) && (trucmsk[k][i] == 1))
				  	{     // if it is inservice and there is an opening compare eta	
						if (make_assignment(assignment,k,i,1,0) == 1)
							assignment = k;
				  	}
				}
				
				if ((assignment != g_asign_trup[i]) && (g_asign_trup[i] != 0) && (trucmsk[g_asign_trup[i]][i] == 1) && (Car_ots[g_asign_trup[i]] == 0) )	// assigning to a diffrent car
				{             // checking to see if its the old cars next stop 
					if (keep_assignment(g_asign_trup[i],assignment,i,1,0) == 1)
						assignment = g_asign_trup[i];
				}// if it was the old cars next stop let old car take it

				for (k=1; k<=cons[nmbcars]; k++)
				{
			    	if (k == assignment)
						g_trup_hall_call[fl_ix][k] |= fl_mask[i];	// asign the fastest car
			    	else 
						g_trup_hall_call[fl_ix][k] &= ~fl_mask[i];	// cancel call from others
				}

				if (gc_tr == 0)
					assignment = 0;

				if (assignment == 0)   
				{
		    		g_trup_hall_call[fl_ix][0] &= ~fl_mask[i];		// cancel call from group
					
					for (k=1; k<=cons[nmbcars]; k++)
					{
						g_trup_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from car #1
					}
					gc_truc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board

//					gc_truc[fl_ix] &= ~fl_mask[i];			// cancel call input
				}
				g_asign_trup[i] = assignment;
      		}
      		else
      		{
				for (k=1; k<=cons[nmbcars]; k++)
	    	  		g_trup_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel call from others
      		}
    	}
    	if(i > 1)
    	{
		  	if ((g_trdn_hall_call[fl_ix][0] & fl_mask[i]) != 0)      // there is an down call
		  	{
				assignment = 0;
				for (k=cons[nmbcars]; k>=1; k--)
				{       // find the first elegible car to answer the call
			  		if ((Car_ots[k] == 0) && (trdcmsk[k][i] == 1))
						assignment = k;
				}
				for (k=(assignment + 1); k<=cons[nmbcars]; k++)
				{       // find the car that can answer the call the fastest
			  		if ((Car_ots[k] == 0) && (trdcmsk[k][i] == 1))
			  		{     // if it is inservice and there is an opening compare eta
						if (make_assignment(assignment,k,i,2,0) == 1)
							assignment = k;
			  		}
				}

				// security option to force assignment calls to specific elevators
				if ((assignment != g_asign_trdn[i]) && (g_asign_trdn[i] != 0) && (trdcmsk[g_asign_trdn[i]][i] == 1) && (Car_ots[g_asign_trdn[i]] == 0))  // assigning to a diffrent car
				{             // checking to see if its the old cars next stop
					if (keep_assignment(g_asign_trdn[i],assignment,i,2,0) == 1)
						assignment = g_asign_trdn[i];
				}// if it was the old cars next stop let old car take it

				for (k=1; k<=cons[nmbcars]; k++)
				{
			    	if (k == assignment)
						g_trdn_hall_call[fl_ix][k] |= fl_mask[i];// asign the fastest car
			    	else
						g_trdn_hall_call[fl_ix][k] &= ~fl_mask[i];// cancel the other cars
				}
				
				if (gc_tr == 0)
					assignment = 0;

				if (assignment == 0)  
				{
		   		 	g_trdn_hall_call[fl_ix][0] &= ~fl_mask[i];		// cancel call from group

			    	for(k=1; k<=cons[nmbcars]; k++)
						g_trdn_hall_call[fl_ix][k] &= ~fl_mask[i];		// cancel call from car #1

					gc_trdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//					gc_trdc[fl_ix] &= ~fl_mask[i];			// cancel call input
				}
				g_asign_trdn[i] = assignment;
		  	}
		  	else
		  	{
				for(k=1; k<=cons[nmbcars]; k++)
			      	g_trdn_hall_call[fl_ix][k] &= ~fl_mask[i];		// cancel call from others
		  	}
	    }
  	}

  	return;
}



// ***********************************************
// this is the group ir hall call cancel procedure
// ***********************************************
void group_ir_hallcancel()
{
  int16 i;
  int16 fl_ix;
	
  	for(i=1; i<cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
		if ( ((g_up_hall_call[fl_ix][ir_car] & fl_mask[i]) != 0) && 
			((grp_Car_Stat[ir_car].g_cancel_up == i) || (Car_ots_nbp[ir_car] != 0) || 
			(((gc_iruc[fl_ix] & fl_mask[i]) == 0) && ((cons[incrs] != 3) || (g_asign_up[i] != ir_car)))) || 
			(ir_car == 0) )
		{				// call is being canceled or cannot be answered
			gc_iruc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//			gc_iruc[fl_ix] &= ~fl_mask[i];			// cancel call input
			g_up_hall_call[fl_ix][ir_car] &= ~fl_mask[i];
		}
	}

	for(i=2; i<=cons[grtopf]; i++)
	{
		fl_ix = (i-1)/32;
		if ( ((g_dn_hall_call[fl_ix][ir_car] & fl_mask[i]) != 0) &&	
			 ((grp_Car_Stat[ir_car].g_cancel_dn == i) || (Car_ots_nbp[ir_car] != 0) || 
			 (((gc_irdc[fl_ix] & fl_mask[i]) == 0) && ((cons[incrs] != 3) || (g_asign_dn[i] != ir_car)))) ||
			 (ir_car == 0) )
		{				// call is being canceled or cannot be answered
			gc_irdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//			gc_irdc[fl_ix] &= ~fl_mask[i];			// cancel call input
			g_dn_hall_call[fl_ix][ir_car] &= ~fl_mask[i];
		}
   	}
  	if (rear_op[ir_select] != 0)
  	{
	  	for(i=1; i<cons[grtopf]; i++)
	  	{
			fl_ix = (i-1)/32;
			if ( ((g_upr_hall_call[fl_ix][ir_car] & fl_mask[i]) != 0) && 
				  ((grp_Car_Stat[ir_car].g_cancel_upr == i) || (Car_ots_nbp[ir_car] != 0) || 
				  (((gc_riruc[fl_ix] & fl_mask[i]) == 0) && ((cons[incrs] != 3) || (g_asign_upr[i] != ir_car)))) || 
				  (ir_car == 0) )
			{				// call is being canceled or cannot be answered
				gc_riruc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//				gc_riruc[fl_ix] &= ~fl_mask[i];			// cancel call input
				g_upr_hall_call[fl_ix][ir_car] &= ~fl_mask[i];
			}
	    }
		for(i=2; i<=cons[grtopf]; i++)
	  	{	
			fl_ix = (i-1)/32;
			if ( ((g_dnr_hall_call[fl_ix][ir_car] & fl_mask[i]) != 0) && 
				  ((grp_Car_Stat[ir_car].g_cancel_dnr == i) || (Car_ots_nbp[ir_car] != 0) || 
				  (((gc_rirdc[fl_ix] & fl_mask[i]) == 0) && ((cons[incrs] != 3) || (g_asign_dnr[i] != ir_car)))) || 
				  (ir_car == 0) )
			{				// call is being canceled or cannot be answered
				gc_rirdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
//				gc_rirdc[fl_ix] &= ~fl_mask[i];			// cancel call input
				g_dnr_hall_call[fl_ix][ir_car] &= ~fl_mask[i];
			}
	    }
  	}
  	return;
}

// *********************************************************
// this is the group second riser hall call cancel procedure
// *********************************************************
void group_sr_hallcancel()
{
  int16 i,j;
  int16 fl_ix;

  	for(i=1; i<cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
		for(j=1; j<=cons[nmbcars]; j++)
		{
			if( (grp_Car_Stat[j].g_cancel_irup == i) && (g_asign_irup[i] == j) )
			{
				gc_iruc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
			}

			if( ((grp_Car_Stat[j].g_cancel_irup == i) && (g_asign_irup[i] == j)) &&
				((g_irup_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
				((gc_iruc[fl_ix] & fl_mask[i]) == 0) )
				g_irup_hall_call[fl_ix][0] &= ~fl_mask[i];
		}
  	}


  	for(i=2; i<=cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
	    for(j=1; j<=cons[nmbcars]; j++)
	    {
			if( (grp_Car_Stat[j].g_cancel_irdn == i) && (g_asign_irdn[i] == j) )
			{
				gc_irdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
			}

			if( ((grp_Car_Stat[j].g_cancel_irdn == i) && (g_asign_irdn[i] == j)) &&
				((g_irdn_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
			    ((gc_irdc[fl_ix] & fl_mask[i]) == 0) )
			    g_irdn_hall_call[fl_ix][0] &= ~fl_mask[i];
    	}
  	}
  	if(grtop_rfl != 0)
  	{
	  	for(i=1; i<cons[grtopf]; i++)
	  	{
			fl_ix = (i-1)/32;
		    for(j=1; j<=cons[nmbcars]; j++)
		    {
				if( (grp_Car_Stat[j].g_cancel_rirup == i) && (g_asign_rirup[i] == j) )
				{
					gc_riruc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
				}

				if( ((grp_Car_Stat[j].g_cancel_rirup == i) && (g_asign_rirup[i] == j)) &&
				    ((g_rirup_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
					((gc_riruc[fl_ix] & fl_mask[i]) == 0) )
				      g_rirup_hall_call[fl_ix][0] &= ~fl_mask[i];
	    	}
	  	}
	  	for(i=2; i<=cons[grtopf]; i++)
	  	{
			fl_ix = (i-1)/32;
		    for(j=1; j<=cons[nmbcars]; j++)
		    {
				if( (grp_Car_Stat[j].g_cancel_rirdn == i) && (g_asign_rirdn[i] == j) )
				{
					gc_rirdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
				}
	
				if( ((grp_Car_Stat[j].g_cancel_rirdn == i) && (g_asign_rirdn[i] == j)) &&
			    ((g_rirdn_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
			    ((gc_rirdc[fl_ix] & fl_mask[i]) == 0) )
				      g_rirdn_hall_call[fl_ix][0] &= ~fl_mask[i];
	    	}
	  	}
  	}
	return;
}

// *********************************************************
// this is the group third riser hall call cancel procedure
// *********************************************************

void group_tr_hallcancel()
{
	int16 i,j;
	int16 fl_ix;

  	for(i=1; i<cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
		for(j=1; j<=cons[nmbcars]; j++)
		{
			if ((grp_Car_Stat[j].g_cancel_trup == i) && (g_asign_trup[i] == j))
			{
				gc_truc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
			}

			if( ((grp_Car_Stat[j].g_cancel_trup == i) && (g_asign_trup[i] == j)) &&
				((g_trup_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
				((gc_truc[fl_ix] & fl_mask[i]) == 0) )
				g_trup_hall_call[fl_ix][0] &= ~fl_mask[i];
		}
  	}


  	for(i=2; i<=cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
	    for(j=1; j<=cons[nmbcars]; j++)
	    {
			if( (grp_Car_Stat[j].g_cancel_trdn == i) && (g_asign_trdn[i] == j) )
			{
				gc_trdc_canc[fl_ix] |= fl_mask[i];		// send cancel flag to serial hall call board
			}

			if( ((grp_Car_Stat[j].g_cancel_trdn == i) && (g_asign_trdn[i] == j)) &&
				((g_trdn_hall_call[fl_ix][0] & fl_mask[i]) != 0) &&
			    ((gc_trdc[fl_ix] & fl_mask[i]) == 0) )
			    g_trdn_hall_call[fl_ix][0] &= ~fl_mask[i];
    	}
  	}
  	return;
}


// ***************************************
// *** Inconspicious Riser Control Routine
// ***************************************

void ir_control (void)
{
	int16 i;

	if (cons[incrs] != 0)
	{

		if ((gc_ir_2 != 0) && (fvars[fvircar2] != 0))
			ir_select = fvars[fvircar2];
		else if (fvars[fvircar] != 0)
			ir_select = fvars[fvircar];

		if ((grp_Car_Stat[ir_select].g_dcalls != 0) || 
			(grp_Car_Stat[ir_select].g_dpref != 0) || 
			(grp_Car_Stat[ir_select].g_dirf != 0)	||
			(grp_Car_Stat[ir_select].g_doorf != 0) ||
			((grp_Car_Stat[ir_select].g_rdoorf != 0) && (rear_op[ir_select] == 1)) )
			timers[tiract] = 0;

		if ((((fvars[fvirctl] & 0x01) != 0) && (cons[incrs] == 1)) &&
			(((gc_iruc[0] & g_irucmsk[0]) != 0) || ((gc_irdc[0] & g_irdcmsk[0]) != 0) || 
#if (Tract_HR == 1)
			((gc_iruc[1] & g_irucmsk[1]) != 0) || ((gc_irdc[1] & g_irdcmsk[1]) != 0) || 
			((gc_riruc[1] & g_rirucmsk[1]) != 0) || ((gc_rirdc[1] & g_rirdcmsk[1]) != 0) ||
#endif
			((gc_riruc[0] & g_rirucmsk[0]) != 0) || ((gc_rirdc[0] & g_rirdcmsk[0]) != 0)))
		{
			if (ir_active == 0)
			{
				timers[tirots] = 0;
				g_up_hall_call[0][ir_select] = 0;
				g_dn_hall_call[0][ir_select] = 0;
				gc_iruc_canc[0] = 0; // Clear the cancel flag the first time through
				gc_irdc_canc[0] = 0;
			  	if (rear_op[ir_select] != 0)
			  	{
					g_upr_hall_call[0][ir_select] = 0;
					g_dnr_hall_call[0][ir_select] = 0;
					gc_riruc_canc[0] = 0;
					gc_rirdc_canc[0] = 0;
				}
#if (Tract_HR == 1)
				g_up_hall_call[1][ir_select] = 0;
				g_dn_hall_call[1][ir_select] = 0;
				gc_iruc_canc[1] = 0; // Clear the cancel flag the first time through
				gc_irdc_canc[1] = 0;
			  	if (rear_op[ir_select] != 0)
			  	{
					g_upr_hall_call[1][ir_select] = 0;
					g_dnr_hall_call[1][ir_select] = 0;
					gc_riruc_canc[1] = 0;
					gc_rirdc_canc[1] = 0;
				}
#endif
			}
			ir_active = 1;		// Turn on ir service from call
		}
		else if (((timers[tiract] > 10) && (grp_Car_Stat[ir_select].g_dcalls == 0)) || ((fvars[fvirctl] & 0x04) == 0))
		{													// wait untils car calls are finished or until ir call cancelled
			ir_active = 0;	   // Turn of ir service from call
		}

		if ( (gc_ir == 1) || (gc_rir == 1) || ((ir_active == 1) && ((fvars[fvirctl] & 0x01) != 0) && (cons[incrs] == 1)) )
		{
			if (gc_ir_2 == 0)
				ir_car = fvars[fvircar];
			else
				ir_car = fvars[fvircar2];

			if (((gc_ir == 1) || (gc_rir == 1)) && (ir_active == 0))
				ir_call_enable = 1;
		}
		else 
		{
			ir_call_enable = 0;
			ir_car = 0;
			if ((timers[tiract] > 20) && (timers[tiract] < 30))
			{
				gc_iruc_canc[0] = 0; // Clear the cancel flag the first time through
				gc_irdc_canc[0] = 0;
			  	if (rear_op[ir_select] != 0)
			  	{
					gc_riruc_canc[0] = 0;
					gc_rirdc_canc[0] = 0;
				}
#if (Tract_HR == 1)
				gc_iruc_canc[1] = 0; // Clear the cancel flag the first time through
				gc_irdc_canc[1] = 0;
			  	if (rear_op[ir_select] != 0)
			  	{
					gc_riruc_canc[1] = 0;
					gc_rirdc_canc[1] = 0;
				}
#endif
			}
		}

		if ((ir_call_enable == 0) && (ir_active == 1))
		{
			if (((timers[tiract] > 10) && (grp_Car_Stat[ir_select].g_dcalls == 0)) || ((fvars[fvirctl] & 0x02) == 0))
	 			ir_call_enable = 1;						   // Enable ir call assignment after car calls finished or as soon as ir call active
		}
	}
	else
	{
		if (cons[Second_Riser] != 0)
		{

			if (cons[Second_Riser] == 2)
				group_swap_fl_tbl_3();
			
			if (cons[Second_Riser] == 3)
				sr_cycling_operation();

			
			if ((gc_ir != 0) || (gc_rir != 0) || ((gc_ir_2 != 0) && (cons[Second_Riser] == 2)))
				ir_call_enable = 1;
			else
				ir_call_enable = 0;

			if (((fvars[fvsrctl] & 0x01) != 0) && (ir_call_enable == 1) ||
				((cons[Second_Riser] == 2) && (sr_on_serv == 1)))
			{			 // Assign car only SR calls
				if (sr_car != 1)
				{
					sr_car = 1;
					for (i=1;i<=cons[nmbcars];i++)
					{
						Car_on_SR[i] = Valid_SR_Car[i];
						if (Valid_SR_Car[i] != 0)
							sr_cc_ctrl[i] |= 0x01;
						else
							sr_cc_ctrl[i] &= ~0x01;
					}
				}
			}
			else 
			{
				if (sr_car != 0)
				{
					sr_car = 0;
					for (i=1;i<=cons[nmbcars];i++)
					{
						Car_on_SR[i] = 0;
						sr_cc_ctrl[i] &= ~0x01;
					}
				}
			}
		}
		ir_car = 0;
	}
	
	// Third riser allowed with Inconspicious Riser OR Second Riser as long as rear calls are disabled 

	if ((cons[Third_Riser] != 0) && (cons[rear] == 0))
	{	
	
		if (gc_tr != 0)
			tr_call_enable = 1;
		else
			tr_call_enable = 0;

		if (((fvars[fvtrctrl] & 0x01) != 0) && (tr_call_enable == 1))
		{			 // Assign car only TR calls
			if (tr_car != 1)
			{
				tr_car = 1;
				for (i=1;i<=cons[nmbcars];i++)
				{
					Car_on_TR[i] = Valid_TR_Car[i];
					if (Valid_TR_Car[i] != 0)
						tr_cc_ctrl[i] |= 0x01;
					else
						tr_cc_ctrl[i] &= ~0x01;
				}
			}
		}
		else 
		{
			if (tr_car != 0)
			{
				tr_car = 0;
				for (i=1;i<=cons[nmbcars];i++)
				{
					Car_on_TR[i] = 0;
					tr_cc_ctrl[i] &= ~0x01;
				}
			}
		}
	}
}

void ir_dual_riser (void)
{
	if ((cons[incrs] == 2) || ((cons[Second_Riser] != 0) && ((fvars[fvsrctl] & 0x02) != 0)))
	{				  // Special ir riser input used as dual standard hall call when ir not in use
		if (gc_ir == 0)
		{
			ir_hc = 1;
			gc_uc_set[0] |= ((gc_uc[0] ^ gc_iruc[0]) & ~(gc_uc_canc[0] | uc_canc_hold[0])) & g_ucmsk[0];		// if the hall or ir call set send it to both boards
			gc_dc_set[0] |= ((gc_dc[0] ^ gc_irdc[0]) & ~(gc_dc_canc[0] | dc_canc_hold[0])) & g_dcmsk[0];		// note in this mode the uc and dc data is sent to the ir board
			uc_canc_hold[0] = (uc_canc_hold[0] | gc_uc_canc[0]) & (gc_uc[0] | gc_iruc[0]) & g_ucmsk[0];
			dc_canc_hold[0] = (dc_canc_hold[0] | gc_dc_canc[0]) & (gc_dc[0] | gc_irdc[0]) & g_dcmsk[0];
#if (Tract_HR == 1)
			gc_uc_set[1] |= ((gc_uc[1] ^ gc_iruc[1]) & ~(gc_uc_canc[1] | uc_canc_hold[1])) & g_ucmsk[1];		// if the hall or ir call set send it to both boards
			gc_dc_set[1] |= ((gc_dc[1] ^ gc_irdc[1]) & ~(gc_dc_canc[1] | dc_canc_hold[1])) & g_dcmsk[1];		// note in this mode the uc and dc data is sent to the ir board
			uc_canc_hold[1] = (uc_canc_hold[1] | gc_uc_canc[1]) & (gc_uc[1] | gc_iruc[1]) & g_ucmsk[1];
			dc_canc_hold[1] = (dc_canc_hold[1] | gc_dc_canc[1]) & (gc_dc[1] | gc_irdc[1]) & g_dcmsk[1];
#endif
		}													
		// see where the data is packed in comm.cc.
		else
		{
			if (ir_hc == 1)
			{		// Previously on Standard Service so clear the set flags
				gc_uc_set[0] = 0;
				gc_dc_set[0] = 0;				
#if (Tract_HR == 1)
				gc_uc_set[1] = 0;
				gc_dc_set[1] = 0;				
#endif
			}
			ir_hc = 0;
			uc_canc_hold[0] = 0;
			dc_canc_hold[0] = 0;
#if (Tract_HR == 1)
			uc_canc_hold[1] = 0;
			dc_canc_hold[1] = 0;
#endif
		}	
		if (gc_rir == 0)
		{
			rir_hc = 1;
			gc_ruc_set[0] |= ((gc_ruc[0] ^ gc_riruc[0]) & ~(gc_ruc_canc[0] | ruc_canc_hold[0])) & g_rucmsk[0];
			gc_rdc_set[0] |= ((gc_rdc[0] ^ gc_rirdc[0]) & ~(gc_rdc_canc[0] | rdc_canc_hold[0])) & g_rdcmsk[0];
			ruc_canc_hold[0] = (ruc_canc_hold[0] | gc_ruc_canc[0]) & (gc_ruc[0] | gc_riruc[0]) & g_rucmsk[0];
			rdc_canc_hold[0] = (rdc_canc_hold[0] | gc_rdc_canc[0]) & (gc_rdc[0] | gc_rirdc[0]) & g_rdcmsk[0];
#if (Tract_HR == 1)
			gc_ruc_set[1] |= ((gc_ruc[1] ^ gc_riruc[1]) & ~(gc_ruc_canc[1] | ruc_canc_hold[1])) & g_rucmsk[1];
			gc_rdc_set[1] |= ((gc_rdc[1] ^ gc_rirdc[1]) & ~(gc_rdc_canc[1] | rdc_canc_hold[1])) & g_rdcmsk[1];
			ruc_canc_hold[1] = (ruc_canc_hold[1] | gc_ruc_canc[1]) & (gc_ruc[1] | gc_riruc[1]) & g_rucmsk[1];
			rdc_canc_hold[1] = (rdc_canc_hold[1] | gc_rdc_canc[1]) & (gc_rdc[1] | gc_rirdc[1]) & g_rdcmsk[1];
#endif
		}
		else
		{
			if (ir_hc == 1)
			{		// Previously on Standard Service so clear the set flags
				gc_ruc_set[0] = 0;
				gc_rdc_set[0] = 0;				
#if (Tract_HR == 1)
				gc_ruc_set[1] = 0;
				gc_rdc_set[1] = 0;				
#endif
			}
			rir_hc = 0;
			ruc_canc_hold[0] = 0;
			rdc_canc_hold[0] = 0;
#if (Tract_HR == 1)
			ruc_canc_hold[1] = 0;
			rdc_canc_hold[1] = 0;
#endif
		}	
	}
	else
	{
		ir_hc = 0;
		rir_hc = 0;
	} 
}

#if (Tract_HR == 1)
void dual_serial_riser (void)
{
	if (cons[Dual_Ser_Riser] == 1)
	{				  // Special Dual Serial hall riser for front or rear standard risers
		gc_uc_set[0] |= ((gc_uc[0] ^ gc_uc_dsr[0]) & ~(gc_uc_canc[0] | uc_dsr_canc_hold[0])) & g_ucmsk[0];		// if the hall or ir call set send it to both boards
		gc_dc_set[0] |= ((gc_dc[0] ^ gc_dc_dsr[0]) & ~(gc_dc_canc[0] | dc_dsr_canc_hold[0])) & g_dcmsk[0];		// note in this mode the uc and dc data is sent to the ir board
		uc_dsr_canc_hold[0] = (uc_dsr_canc_hold[0] | gc_uc_canc[0]) & (gc_uc[0] | gc_uc_dsr[0]) & g_ucmsk[0];
		dc_dsr_canc_hold[0] = (dc_dsr_canc_hold[0] | gc_dc_canc[0]) & (gc_dc[0] | gc_dc_dsr[0]) & g_dcmsk[0];
		gc_uc_set[1] |= ((gc_uc[1] ^ gc_uc_dsr[1]) & ~(gc_uc_canc[1] | uc_dsr_canc_hold[1])) & g_ucmsk[1];		// if the hall or ir call set send it to both boards
		gc_dc_set[1] |= ((gc_dc[1] ^ gc_dc_dsr[1]) & ~(gc_dc_canc[1] | dc_dsr_canc_hold[1])) & g_dcmsk[1];		// note in this mode the uc and dc data is sent to the ir board
		uc_dsr_canc_hold[1] = (uc_dsr_canc_hold[1] | gc_uc_canc[1]) & (gc_uc[1] | gc_uc_dsr[1]) & g_ucmsk[1];
		dc_dsr_canc_hold[1] = (dc_dsr_canc_hold[1] | gc_dc_canc[1]) & (gc_dc[1] | gc_dc_dsr[1]) & g_dcmsk[1];

		gc_ruc_set[0] |= ((gc_ruc[0] ^ gc_ruc_dsr[0]) & ~(gc_ruc_canc[0] | ruc_dsr_canc_hold[0])) & g_rucmsk[0];
		gc_rdc_set[0] |= ((gc_rdc[0] ^ gc_rdc_dsr[0]) & ~(gc_rdc_canc[0] | rdc_dsr_canc_hold[0])) & g_rdcmsk[0];
		ruc_dsr_canc_hold[0] = (ruc_dsr_canc_hold[0] | gc_ruc_canc[0]) & (gc_ruc[0] | gc_ruc_dsr[0]) & g_rucmsk[0];
		rdc_dsr_canc_hold[0] = (rdc_dsr_canc_hold[0] | gc_rdc_canc[0]) & (gc_rdc[0] | gc_rdc_dsr[0]) & g_rdcmsk[0];
		gc_ruc_set[1] |= ((gc_ruc[1] ^ gc_ruc_dsr[1]) & ~(gc_ruc_canc[1] | ruc_dsr_canc_hold[1])) & g_rucmsk[1];
		gc_rdc_set[1] |= ((gc_rdc[1] ^ gc_rdc_dsr[1]) & ~(gc_rdc_canc[1] | rdc_dsr_canc_hold[1])) & g_rdcmsk[1];
		ruc_dsr_canc_hold[1] = (ruc_dsr_canc_hold[1] | gc_ruc_canc[1]) & (gc_ruc[1] | gc_ruc_dsr[1]) & g_rucmsk[1];
		rdc_dsr_canc_hold[1] = (rdc_dsr_canc_hold[1] | gc_rdc_canc[1]) & (gc_rdc[1] | gc_rdc_dsr[1]) & g_rdcmsk[1];
	}
}
#endif


//**************************************************
//****** Second Riser cycling operation 
//**************************************************

void sr_cycling_operation(void)
{
	uint16 i = 0;
	static int16 cycling_svc_flag = 0;
	static int16 prev_nmb_cc = 0;
	
	// none of the cars on fire/emerg service AND ICR input ON and car is on automatic service
	if ((ir_call_enable == 1) && (gservf == 0) && (fvars[fvircar] != 0) && (Car_ots[fvars[fvircar]] == 0) && (grp_Car_Stat[fvars[fvircar]].g_servf == s_AUTO))
	{
		if (cycling_svc_flag == 0)
		{
			clroutp(o_CYCLO);
			if (rdinp(i_CYCLE) == 1)
			{
				set_ir_dncall(top_fl[fvars[fvircar]]);		// latch the top most down call on the ir car
				cycling_svc_flag = 1;
				prev_nmb_cc = grp_Car_Stat[fvars[fvircar]].g_nmbcc;
				timers[tiract] = 0;		// ir active timer used to give some time for calls to latch
			}
		}
		else
		{
			setoutp(o_CYCLO);
			if (timers[tiract] > 10)
			{
				if (((grp_Car_Stat[fvars[fvircar]].g_nmbcc > prev_nmb_cc) && ((nmb_asg_irdc != 0) || (nmb_asg_iruc != 0))) || (Car_ots[fvars[fvircar]] != 0))
				{	// cancel all the down calls
					grp_clrirhc();	// clear for rear calls too
					if (get_any_ir_hall_call() == 0)
						cycling_svc_flag = 0;
				}
				else if (nmb_asg_irdc == 0)
				{
					if ((cycling_svc_flag == 1) && (grp_Car_Stat[fvars[fvircar]].g_pos == cons[topf]))
					{
						// when at top floor latch 1st up and all down calls
						set_ir_upcall(cons[bottomf]);
						for(i=(uint16)(cons[bottomf]+1); i<cons[topf]; i++)
							set_ir_dncall(i);
						cycling_svc_flag = 2;
						timers[tiract] = 0;
					}
					else if (((grp_Car_Stat[fvars[fvircar]].g_pos == cons[bottomf]) && (cycling_svc_flag == 2)) ||
							 ((nmb_asg_irdc == 0) && (nmb_asg_iruc == 0)))
					{
						cycling_svc_flag = 0;
					}
				}
			}
			if (grp_Car_Stat[fvars[fvircar]].g_nmbcc <  prev_nmb_cc)
				prev_nmb_cc = grp_Car_Stat[fvars[fvircar]].g_nmbcc;
		}
	}
	else
	{
		clroutp(o_CYCLO);
		cycling_svc_flag = 0;
	}
}



/* Revision History
2/2/2012 v5.54.55 HDL	1. Added a fvars[fvhcsecctrl] to enable the hall call security to lockout the Second riser
4/13/12 v6.0.5 hdl/mhd	1. In latch_sr_hallcall, there was no check for existing call before sending the cancel bit.
							once hall security was being set back on the controller would send a cancell command to the driver
							and the acknowledge light awould go out
4/13/12 v5.54.72 hdl	1. In latch_sr_hallcall, there was no check for existing call before sending the cancel bit.
							once hall security was being set back on the controller would send a cancell command to the driver
							and the acknowledge light awould go out

8/23/12  v7.0.16 mhd	1.Added below Galaxy III changes made in v5.55.41 - v5.55.43
						2. Modified latch_ir_hallcall() routine to clear ir up/down call set flag when ir up/down call cancel is set
						3. Modified assign_sr_hallcall() routine to allow assignment when either gc_ir or gc_ir_2 inputs are ON for cons[Second_Riser] = 2
						4. Added routines latch_tr_hallcall(), assign_tr_hallcall() and group_tr_hallcancel() to latch, assign and cancel third riser hall calls. 
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.

10/9/14 v7.1.16 mhd		1. Modified changes from galaxy 3 version 6.0.53 to 6.0.60.  See below:
							v6.0.54 as		1. Found bug in group_tr_hallcancel(), changed from gc_irdc[fl_ix] to gc_trdc[fl_ix]
12/18/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
							3/04/2014 v6.1.35 as	1. Added sr_cycling_operation() routine for Second Riser cycling operation
							3/13/2013 v6.1.37 as	1. Changed latch_ir_hallcall(), latch_sr_hallcall() and latch_tr_hallcall() so call set flag clears when a call is successfully latched
							4/8/2013 v6.1.45 as		1. Modified sr_cycling_operation() so when clearing ir hall calls also clear last up ir hall call
							10/07/2014 v6.1.87 as	1. Modified sr_cycling_operation() to have CYCLO output set when on cycling operation.

*/