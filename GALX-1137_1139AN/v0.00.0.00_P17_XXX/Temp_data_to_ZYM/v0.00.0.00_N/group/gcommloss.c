#define d_GCOMMLOSS 1
#include "global.h"

bool hc_power_lost = 0;

uint16 emdispf = 0;
uint16 emdispf_bit = 0x0002;
uint16 emdispf_bitr = 0x0040;


void hallcall_comm_loss(void);
void set_front_hallcall_loss(void);
void set_rear_hallcall_loss(void);


// ***************************************************
// *** Hall Call Comm Loss routine *******************
//  This routine sets the hall calls for the board
//  that is not communicating
// ***************************************************

void hallcall_comm_loss(void)
{

	if ((gin_HCP == 1) && (fvars[fvemdisp] != 0))
	{			// have hall call power
		if ((fvars[fvemdisp] & 0x03) != 0)
			set_front_hallcall_loss();
		
		if ((grtop_rfl != 0)&&((fvars[fvemdisp] & 0x05) != 0))
			set_rear_hallcall_loss();
	}
	else
		emdispf &= ~0x07FE;			// Clear all the bits except for hall call power loss
}


//***********************************************
// Set front hall call for boards with commm loss
//***********************************************
			
void set_front_hallcall_loss(void)
{
	int16 i;
	int16 fl_ix=0;
	if (timers[temhc] > 80)
	{
		timers[temhc] = 0;
		emdispf |= emdispf_bit;
		if (lobby_fl > 1)
		{
			for(i=1; i<=lobby_fl; i++)
			{	// Set up calls for floors below and at the lobby
				fl_ix = (i-1)/32;
				if ((g_ucmsk[fl_ix] & fl_mask[i]) != 0)
					g_up_hall_call[fl_ix][0] |= fl_mask[i];
			}
		}

		for(i=lobby_fl; i <= cons[grtopf]; i++)
		{	// Set down calls for floors above the lobby
			fl_ix = (i-1)/32;
			if ((g_dcmsk[fl_ix] & fl_mask[i]) != 0)
				g_dn_hall_call[fl_ix][0] |= fl_mask[i];
		}
		clr_uphc_que();
		clr_dnhc_que();
	}
}


//***********************************************
// Set rear hall call for boards with commm loss
//***********************************************
			
void set_rear_hallcall_loss(void)
{
	int16 i;
	int16 fl_ix=0;
	if (timers[temrhc] > 80)
	{
		timers[temrhc] = 0;
		emdispf |= emdispf_bitr;
		if (grbot_rfl <= lobby_fl)
		{
			for(i=grbot_rfl; i<=lobby_fl; i++)
			{	// Set up calls for floors below and at the lobby
				fl_ix = (i-1)/32;
				if ((g_rucmsk[fl_ix] & fl_mask[i]) != 0)
					g_upr_hall_call[fl_ix][0] |= fl_mask[i];
			}
		}
		for(i=lobby_fl; i <= grtop_rfl; i++)
		{	// Set down calls for floors above the lobby
			fl_ix = (i-1)/32;
			if ((g_rdcmsk[fl_ix] & fl_mask[i]) != 0)
				g_dnr_hall_call[fl_ix][0] |= fl_mask[i];
		}
		clr_uphc_que();
		clr_dnhc_que();
	}
}

/*
Revision History:

3/19/12	v5.54.65 hdl	1. Corrected serial hall call loss for emergency dispatching on the second board (HC4 instead of HC3)
9/08/12 v5.55.12 hdl	1. Added bits to fvemdisp Emergency Dispatch Enable in order to allow emergency dispatching for Front or rear calls only independently. 
							Setting of 1 still does both frontand rear
12/18/12 v6.00.26 mhd	1. Added changes from galaxy3 from 5.55.06 to 5.55.20. (Left Vip only with my changes).

*/
