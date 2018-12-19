#define d_GCODEBLUE 1
#include "global.h"


int16 g_cb_fl[car_size];
int16 g_rcb_fl[car_size];
int16 grp_codebf[car_size];

	// grp_codebf = 0 no code blue commands from the group
	// grp_codebf = 1 All cars on ind or out of service, ind car turn on indicator

uint32 g_cbmsk[2]={0,0};			// Bit mask for valid code blue call floors
uint32 g_rcbmsk[2]={0,0};		// Bit mask for valid rear code blue call floors


uint32 cb_canc_hold[2] = {0,0};
uint32 rcb_canc_hold[2] = {0,0};
bool cb_demand = 0;


int16 best_std_cb_car(int16 fl, int16 cb_sel_car);
int16 best_std_rcb_car(int16 fl, int16 cb_sel_car);
int16 best_sr_cb_car(int16 fl, int16 cb_sel_car);
int16 best_sr_rcb_car(int16 fl, int16 cb_sel_car);
void cb_dual_riser(void);
void sr_front_code_blue(void);
void sr_rear_code_blue(void);
void std_front_code_blue(void);
void std_rear_code_blue(void);
void code_blue(void);
int16 cb_car_on_ind(int16 cb_sel_car);


//*********************************************
//***  Best front code blue car to recall *****
//*********************************************

int16 best_std_cb_car(int16 fl, int16 cb_sel_car)
{
	int16 i;
	int16 cb_car;
	int16 cb_car_eta;
	int16 eta;

	if ((cb_sel_car >= 1) && (cb_sel_car <= cons[nmbcars]))
	{
		if ((g_cb_fl[cb_sel_car] == 0) && (Car_ots_nbp[cb_sel_car] == 0) && (g_rcb_fl[cb_sel_car] != fl))	// in service and not assigned a code blue
			return cb_sel_car;	   // first selection
		else if ((g_cb_fl[cb_sel_car] == fl) && ((Car_ots_nbp[cb_sel_car] == 0) || 
			 (grp_Car_Stat[cb_sel_car].g_servf == s_CBLUE) || ((grp_Car_Stat[cb_sel_car].g_codebf == 4) && (grp_Car_Stat[cb_sel_car].g_pos == fl))))
			return cb_sel_car;		// already assigned code blue
		else if  ((grp_Car_Stat[cb_sel_car].g_servf == s_HSV)	&& (grp_Car_Stat[cb_sel_car].g_pos == fl))
			return cb_sel_car;		// pre-selected code blue car
		else
			return 0;	   // else pre-selected code blue car is out of service	
	}

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_cb_fl[i] == fl) && ((i != ir_car) || (fvars[fvcbovrir] == 1)) &&  
			((cons[CB_SR] == 0) || (gc_cb_sr == 0) || (valid_cbpr_car[i] == 1))  &&  // primary riser car 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_CBLUE) || 
				(((grp_Car_Stat[i].g_codebf == 4) || (grp_Car_Stat[i].g_servf == s_HSV)) && (grp_Car_Stat[i].g_pos == fl))))
			return i;		// already assigned this floor
	}

	cb_car_eta = 255;
	eta = 254;
	cb_car = 0;
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((Car_ots_nbp[i] == 0) && (ccmsk[i][fl] == 1) && 
				((cons[CBCar] == 0) || (valid_cb_car[i] == 1)) && ((cons[CB_SR] == 0) || (gc_cb_sr == 0) || (valid_cbpr_car[i] == 1)))
		{													// valid code blue car and a primary riser car
			if (fl == 1)
		  		eta = car_up_eta[i][fl];
			else if (fl == cons[grtopf])
				eta = car_dn_eta[i][fl];
			else 
			{
				if (car_dn_eta[i][fl] <	car_up_eta[i][fl])
					eta = car_dn_eta[i][fl];
				else		  		
			  		eta = car_up_eta[i][fl];
			}
			if (i == ir_car)
			{
				eta += fvars[fvircbpenalty];
				if (eta > 255)
					eta = 255;
			}
		}
		else if ((grp_Car_Stat[i].g_servf == s_HSV) && (grp_Car_Stat[i].g_pos == fl))
			eta = 0;
		else
			eta = 255;

		if ((g_cb_fl[i] == 0) && (g_rcb_fl[i] != fl) && (eta < cb_car_eta) && 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_HSV)) && (ccmsk[i][fl] == 1) && 
			((cons[CB_SR] == 0) || (gc_cb_sr == 0) || (valid_cbpr_car[i] == 1)) &&
			((i != ir_car) || (fvars[fvcbovrir] == 1)))
		{
			cb_car = i;
			cb_car_eta = eta;
		}
	}

	return cb_car;
}

//*********************************************
//***  Best rear code blue car to recall *****
//*********************************************

int16 best_std_rcb_car(int16 fl, int16 cb_sel_car)
{
	int16 i;
	int16 cb_car;
	int16 cb_car_eta;
	int16 eta;

	if ((cb_sel_car >= 1) && (cb_sel_car <= cons[nmbcars]))
	{
		if ((g_rcb_fl[cb_sel_car] == 0) && (Car_ots_nbp[cb_sel_car] == 0) && (g_cb_fl[cb_sel_car] != fl))	// in service and not assigned a code blue
			return cb_sel_car;	   // first selection
		else if ((g_rcb_fl[cb_sel_car] == fl) && ((Car_ots_nbp[cb_sel_car] == 0) || 
			 (grp_Car_Stat[cb_sel_car].g_servf == s_CBLUE) || ((grp_Car_Stat[cb_sel_car].g_codebf == 4) && (grp_Car_Stat[cb_sel_car].g_pos == fl))))
			return cb_sel_car;		// already assigned code blue
		else if  ((grp_Car_Stat[cb_sel_car].g_servf == s_HSV)	&& (grp_Car_Stat[cb_sel_car].g_pos == fl))
			return cb_sel_car;		// pre-selected code blue car
		else
			return 0;	   // else pre-selected code blue car is out of service	
	}

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_rcb_fl[i] == fl) && ((i != ir_car) || (fvars[fvcbovrir] == 1))  && 
			((cons[CB_SR] == 0) || (gc_rcb_sr == 0) || (valid_cbpr_car[i] == 1)) && 	  // a primary riser car
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_CBLUE) || 
				(((grp_Car_Stat[i].g_codebf == 4) || (grp_Car_Stat[i].g_servf == s_HSV)) && (grp_Car_Stat[i].g_pos == fl))))
			return i;		// already assigned this floor
	}

	cb_car_eta = 255;
	eta = 254;
	cb_car = 0;
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((Car_ots_nbp[i] == 0) && (rccmsk[i][fl] == 1) && 
			((cons[CBCar] == 0) || (valid_cb_car[i] == 1)) && ((cons[CB_SR] == 0) || (gc_rcb_sr == 0) || (valid_cbpr_car[i] == 1)))
		{															// valid code blue car and a primary riser car
			if (fl == 1)
		  		eta = car_up_eta[i][fl];
			else if (fl == cons[grtopf])
				eta = car_dn_eta[i][fl];
			else 
			{
				if (car_dn_eta[i][fl] <	car_up_eta[i][fl])
					eta = car_dn_eta[i][fl];
				else		  		
			  		eta = car_up_eta[i][fl];
			}
			if (i == ir_car)
			{
				eta += fvars[fvircbpenalty];
				if (eta > 255)
					eta = 255;
			}
		}
		else if ((grp_Car_Stat[i].g_servf == s_HSV) && (grp_Car_Stat[i].g_pos == fl))
			eta = 0;
		else
			eta = 255;

		if ((g_rcb_fl[i] == 0) && (g_cb_fl[i] != fl) && (eta < cb_car_eta) && 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_HSV)) && (rccmsk[i][fl] == 1) && 
			((cons[CB_SR] == 0) || (gc_rcb_sr == 0) || (valid_cbpr_car[i] == 1)) &&
			((i != ir_car) || (fvars[fvcbovrir] == 1)))
		{
			cb_car = i;
			cb_car_eta = eta;
		}
	}

	return cb_car;
}

//**********************************************************
//***  Best second riser front code blue car to recall *****
//**********************************************************

int16 best_sr_cb_car(int16 fl, int16 cb_sel_car)
{
	int16 i;
	int16 cb_car;
	int16 cb_car_eta;
	int16 eta;

	if ((cb_sel_car >= 1) && (cb_sel_car <= cons[nmbcars]))
	{
		if ((g_cb_fl[cb_sel_car] == 0) && (Car_ots_nbp[cb_sel_car] == 0) && (g_rcb_fl[cb_sel_car] != fl))	// in service and not assigned a code blue
			return cb_sel_car;	   // first selection
		else if ((g_cb_fl[cb_sel_car] == fl) && ((Car_ots_nbp[cb_sel_car] == 0) || 
			 (grp_Car_Stat[cb_sel_car].g_servf == s_CBLUE) || ((grp_Car_Stat[cb_sel_car].g_codebf == 4) && (grp_Car_Stat[cb_sel_car].g_pos == fl))))
			return cb_sel_car;		// already assigned code blue
		else if  ((grp_Car_Stat[cb_sel_car].g_servf == s_HSV)	&& (grp_Car_Stat[cb_sel_car].g_pos == fl))
			return cb_sel_car;		// pre-selected code blue car
		else
			return 0;	   // else pre-selected code blue car is out of service	
	}

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_cb_fl[i] == fl) && ((i != ir_car) || (fvars[fvcbovrir] == 1)) && 
			(cons[CB_SR] == 1) && (gc_cb_sr == 1) && (valid_cbsr_car[i] == 1) && 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_CBLUE) || 
				(((grp_Car_Stat[i].g_codebf == 4) || (grp_Car_Stat[i].g_servf == s_HSV)) && (grp_Car_Stat[i].g_pos == fl))))
			return i;		// already assigned this floor
	}

	cb_car_eta = 255;
	eta = 254;
	cb_car = 0;
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((Car_ots_nbp[i] == 0) && (ccmsk[i][fl] == 1) && 
			((cons[CBCar] == 0) || (valid_cb_car[i] == 1))  && (cons[CB_SR] == 1) && (gc_cb_sr == 1) && (valid_cbsr_car[i] == 1))
		{
			if (fl == 1)
		  		eta = car_up_eta[i][fl];
			else if (fl == cons[grtopf])
				eta = car_dn_eta[i][fl];
			else 
			{
				if (car_dn_eta[i][fl] <	car_up_eta[i][fl])
					eta = car_dn_eta[i][fl];
				else		  		
			  		eta = car_up_eta[i][fl];
			}
			if (i == ir_car)
			{
				eta += fvars[fvircbpenalty];
				if (eta > 255)
					eta = 255;
			}
		}
		else if ((grp_Car_Stat[i].g_servf == s_HSV) && (grp_Car_Stat[i].g_pos == fl))
			eta = 0;
		else
			eta = 255;

		if ((g_cb_fl[i] == 0) && (g_rcb_fl[i] != fl) && (eta < cb_car_eta) && 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_HSV)) && (ccmsk[i][fl] == 1) && 
			(cons[CB_SR] == 1) && (gc_cb_sr == 1) && (valid_cbsr_car[i] == 1) &&
			((i != ir_car) || (fvars[fvcbovrir] == 1)))
		{
			cb_car = i;
			cb_car_eta = eta;
		}
	}

	return cb_car;
}

//*********************************************************
//***  Best second riser rear code blue car to recall *****
//*********************************************************

int16 best_sr_rcb_car(int16 fl, int16 cb_sel_car)
{
	int16 i;
	int16 cb_car;
	int16 cb_car_eta;
	int16 eta;

	if ((cb_sel_car >= 1) && (cb_sel_car <= cons[nmbcars]))
	{
		if ((g_rcb_fl[cb_sel_car] == 0) && (Car_ots_nbp[cb_sel_car] == 0) && (g_cb_fl[cb_sel_car] != fl))	// in service and not assigned a code blue
			return cb_sel_car;	   // first selection
		else if ((g_rcb_fl[cb_sel_car] == fl) && ((Car_ots_nbp[cb_sel_car] == 0) || 
			 (grp_Car_Stat[cb_sel_car].g_servf == s_CBLUE) || ((grp_Car_Stat[cb_sel_car].g_codebf == 4) && (grp_Car_Stat[cb_sel_car].g_pos == fl))))
			return cb_sel_car;		// already assigned code blue
		else if  ((grp_Car_Stat[cb_sel_car].g_servf == s_HSV)	&& (grp_Car_Stat[cb_sel_car].g_pos == fl))
			return cb_sel_car;		// pre-selected code blue car
		else
			return 0;	   // else pre-selected code blue car is out of service	
	}

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_rcb_fl[i] == fl) && ((i != ir_car) || (fvars[fvcbovrir] == 1)) && 
			(cons[CB_SR] == 1) && (gc_rcb_sr == 1) && (valid_cbsr_car[i] == 1) && 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_CBLUE) || 
				(((grp_Car_Stat[i].g_codebf == 4) || (grp_Car_Stat[i].g_servf == s_HSV)) && (grp_Car_Stat[i].g_pos == fl))))
			return i;		// already assigned this floor
	}

	cb_car_eta = 255;
	eta = 254;
	cb_car = 0;
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((Car_ots_nbp[i] == 0) && (rccmsk[i][fl] == 1) && 
			((cons[CBCar] == 0) || (valid_cb_car[i] == 1)) && (cons[CB_SR] == 1) && (gc_rcb_sr == 1) && (valid_cbsr_car[i] == 1))
		{
			if (fl == 1)
		  		eta = car_up_eta[i][fl];
			else if (fl == cons[grtopf])
				eta = car_dn_eta[i][fl];
			else 
			{
				if (car_dn_eta[i][fl] <	car_up_eta[i][fl])
					eta = car_dn_eta[i][fl];
				else		  		
			  		eta = car_up_eta[i][fl];
			}
			if (i == ir_car)
			{
				eta += fvars[fvircbpenalty];
				if (eta > 255)
					eta = 255;
			}
		}
		else if ((grp_Car_Stat[i].g_servf == s_HSV) && (grp_Car_Stat[i].g_pos == fl))
			eta = 0;
		else
			eta = 255;

		if ((g_rcb_fl[i] == 0) && (g_cb_fl[i] != fl) && (eta < cb_car_eta) && 
			((Car_ots_nbp[i] == 0) || (grp_Car_Stat[i].g_servf == s_HSV)) && (rccmsk[i][fl] == 1) && 
			(cons[CB_SR] == 1) && (gc_rcb_sr == 1) && (valid_cbsr_car[i] == 1) &&
			((i != ir_car) || (fvars[fvcbovrir] == 1)))
		{
			cb_car = i;
			cb_car_eta = eta;
		}
	}

	return cb_car;
}

//*********************************************
//** Code blue CAR ON IND ********************
//*********************************************

int16 cb_car_on_ind(int16 cb_sel_car)
{
	int16 i;
	int16 indcar = 0;

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((grp_Car_Stat[i].g_servf == s_IND) && (fvars[fvcbind] == 1) &&
			((cb_sel_car == i) || (cb_sel_car == 0)))	// car on independent service
		{
			grp_codebf[i] |= 0x0001;
			indcar = 1;
		}

	}
	return(indcar);
}

//**********************************************************************
//** Code blue Dual riser or'ed with standard riser ********************
//**********************************************************************

void cb_dual_riser (void)
{
	if (cons[CB_SR] == 1)
	{				  // Special ir riser input used as standard hall call when ir not in use
		if (gc_cb_sr == 0)
		{
			cb_dr = 1;
			gc_cb_set[0] |= ((gc_cb[0] ^ gc_cbsr[0]) & ~(gc_cb_canc[0] | cb_canc_hold[0])) & g_cbmsk[0];		// if the hall or ir call set send it to both boards
			cb_canc_hold[0] = (cb_canc_hold[0] | gc_cb_canc[0]) & (gc_cb[0] | gc_cbsr[0]) & g_cbmsk[0];
			gc_cb_canc[0] |= cb_canc_hold[0] & gc_cbsr[0] & g_cbmsk[0];
#if (Tract_HR == 1)
			gc_cb_set[1] |= ((gc_cb[1] ^ gc_cbsr[1]) & ~(gc_cb_canc[1] | cb_canc_hold[1])) & g_cbmsk[1];		// if the hall or ir call set send it to both boards
			cb_canc_hold[1] = (cb_canc_hold[1] | gc_cb_canc[1]) & (gc_cb[1] | gc_cbsr[1]) & g_cbmsk[1];
			gc_cb_canc[1] |= cb_canc_hold[1] & gc_cbsr[1] & g_cbmsk[1];
#endif
		}													
		// see where the data is packed in comm.cc.
		else
		{
			if (cb_dr == 1)
			{		// Previously on Standard Service so clear the set flags
				gc_cb_set[0] = 0;
#if (Tract_HR == 1)
				gc_cb_set[1] = 0;
#endif
			}
			cb_dr = 0;
			cb_canc_hold[0] = 0;
#if (Tract_HR == 1)
			cb_canc_hold[1] = 0;
#endif
		}	
		if (gc_rcb_sr == 0)
		{
			rcb_dr = 1;
			gc_rcb_set[0] |= ((gc_rcb[0] ^ gc_rcbsr[0]) & ~(gc_rcb_canc[0] | rcb_canc_hold[0])) & g_rcbmsk[0];
			rcb_canc_hold[0] = (rcb_canc_hold[0] | gc_rcb_canc[0]) & (gc_rcb[0] | gc_rcbsr[0]) & g_rcbmsk[0];
			gc_rcb_canc[0] |= rcb_canc_hold[0] & gc_rcbsr[0] & g_rcbmsk[0];
#if (Tract_HR == 1)
			gc_rcb_set[1] |= ((gc_rcb[1] ^ gc_rcbsr[1]) & ~(gc_rcb_canc[1] | rcb_canc_hold[1])) & g_rcbmsk[1];
			rcb_canc_hold[1] = (rcb_canc_hold[1] | gc_rcb_canc[1]) & (gc_rcb[1] | gc_rcbsr[1]) & g_rcbmsk[1];
			gc_rcb_canc[1] |= rcb_canc_hold[1] & gc_rcbsr[1] & g_rcbmsk[1];
#endif
		}
		else
		{
			if (rcb_dr == 1)
			{		// Previously on Standard Service so clear the set flags
				gc_rcb_set[0] = 0;
#if (Tract_HR == 1)
				gc_rcb_set[1] = 0;
#endif
			}
			rcb_dr = 0;
			rcb_canc_hold[0] = 0;
#if (Tract_HR == 1)
			rcb_canc_hold[1] = 0;
#endif
		}	
	}
	else
	{
		cb_dr = 0;
		rcb_dr = 0;
	} 
}


//*********************************************
//** Code blue selection **********************
//*********************************************

void code_blue(void)
{  
	int16 i;

	cb_demand = 0;
	grp_codebf[0] &= 0xFFFE;
	if (cons[CB_SR] == 1)
	{

		if (gc_cb_sr == 1) 
		{
			std_front_code_blue();		// first riser code blue operation
			sr_front_code_blue();			// second riser code blue operation
		}
		else
		{
			// Combine riser input
			std_front_code_blue();		// inputs or'ed together so use standard code blue
		}

		if (gc_rcb_sr == 1)
		{
			std_rear_code_blue();		// first riser code blue operation
			sr_rear_code_blue();			// second riser code blue operation
		}
		else
		{
			// Combine riser input
			std_rear_code_blue();		// inputs or'ed together so use standard code blue
		}
 
 		cb_dual_riser();

	}
	else
	{
		std_front_code_blue();			// standard code blue operation
		std_rear_code_blue();			// standard code blue operation
	}
	if (cb_demand == 0)
	{		// clear ind car request indicator light
		for (i=1;i<=cons[nmbcars];i++)
			grp_codebf[i] &= 0xFFFE;
	}
}

//*********************************************
//** Code blue Standard Riser Operation *******
//*********************************************

void std_front_code_blue(void)
{
	int16 i;
	int16 cb_car;
	int16 cb_call;
	int16 fl_ix;
	int16 cb_indcar = 0;
			 

  	for(i=1; i <= cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
	    if((gc_cb[fl_ix] & fl_mask[i]) != 0)// check to see if code blue call pushed
		{
			cb_demand = 1;
			gc_cb_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
			cb_car = best_std_cb_car(i,fvars[fvcbcar]);

			if (cb_car == 0) // Check if Code blue car # 1 is in independent
			{
				if (cb_car_on_ind(fvars[fvcbcar]) == 0)
					cb_indcar = 0;
				else
					cb_indcar = fvars[fvcbcar];
			}
			if ((cb_car == 0) && (fvars[fvcbcar2] != 0) && (cb_indcar == 0)) 	 // cannot assign the call yet
			{
				cb_car = best_std_cb_car(i,fvars[fvcbcar2]);
				if (cb_car == 0) // Check if Code blue car # 2 is in independent
				{
					if (cb_car_on_ind(fvars[fvcbcar2]) == 0)
						cb_indcar = 0;
					else
						cb_indcar = fvars[fvcbcar2];
				}
			}
			if((cb_indcar == 0)&&(cb_car == 0)&&((fvars[fvcbanycar] == 1)))
			{
				cb_car = best_std_cb_car(i,0);  //Check any car available				
			}

			if (cb_car == 0)
			{
				if ((cb_car_on_ind(fvars[fvcbcar]) == 0)&&(cb_indcar == 0))
					gc_cb_canc[fl_ix] |= fl_mask[i];
			}
			else if ((g_cb_fl[cb_car] == i) && (grp_Car_Stat[cb_car].g_pos == i) 
					&& ((grp_Car_Stat[i].g_servf == s_HSV) ||(grp_Car_Stat[cb_car].g_codebf == 3) || (grp_Car_Stat[cb_car].g_codebf == 4)))
			{	// cannot assign the call or code blue car is at the floor on code blue or hsv
				gc_cb_canc[fl_ix] |= fl_mask[i];
			}
			else if ((cons[CB_SR] == 0) || (gc_cb_sr == 0) || (valid_cbpr_car[cb_car] == 1))
			{
				g_cb_fl[cb_car] = i;			// assign call to car
				grp_codebf[cb_car] &= 0xFFFE;
			}
	    }
	    else
	    {
			gc_cb_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
		}
  	}
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_cb_fl[i] != 0) && ((cons[CB_SR] == 0) || (gc_cb_sr == 0) || (valid_cbpr_car[i] == 1)))
		{
			fl_ix = (g_cb_fl[i]-1)/32;

			cb_call = ((gc_cb[fl_ix] & fl_mask[g_cb_fl[i]]) != 0);

			if (((grp_Car_Stat[i].g_codebf == 4) || (grp_Car_Stat[i].g_servf != s_CBLUE)) && (cb_call == 0) )
				g_cb_fl[i] = 0;

			if ((grp_Car_Stat[i].g_servf == s_HSV) && (grp_Car_Stat[i].g_pos == g_cb_fl[i]) && (grp_Car_Stat[i].g_doorf == 2))
			{
				gc_cb_canc[fl_ix] |= fl_mask[g_cb_fl[i]];
				g_cb_fl[i] = 0;		// On hospital service, at the code blue floor and the door is open
			}
		}
	}
}

//*********************************************
//** Rear Code blue Standard Riser Operation *******
//*********************************************

void std_rear_code_blue(void)
{
	int16 i;
	int16 cb_car;
	int16 cb_call;
	int16 fl_ix;
			 
 	if(grtop_rfl != 0)
 	{
	  	for(i=1; i <= cons[grtopf]; i++)
	  	{
			fl_ix = (i-1)/32;
		    if((gc_rcb[fl_ix] & fl_mask[i]) != 0)// check to see if up hall call pushed
			{
				cb_demand = 1;
				gc_rcb_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
				cb_car = best_std_rcb_car(i,fvars[fvcbcar]);
				if ((cb_car == 0) && (fvars[fvcbcar] != fvars[fvcbcar2] )) 	 // cannot assign the call yet
				{
					cb_car = best_std_rcb_car(i,fvars[fvcbcar2]);
				}
				if (cb_car == 0) 	 // cannot assign the call
				{
					if (cb_car_on_ind(fvars[fvcbcar]) == 0)
						gc_rcb_canc[fl_ix] |= fl_mask[i];
				}
				else if ((g_rcb_fl[cb_car] == i) && (grp_Car_Stat[cb_car].g_pos == i) 
					&& ((grp_Car_Stat[i].g_servf == s_HSV) || (grp_Car_Stat[cb_car].g_codebf == 3) || (grp_Car_Stat[cb_car].g_codebf == 4)))
				{	// code blue car is at the floor on code blue or hsv
					gc_rcb_canc[fl_ix] |= fl_mask[i];
				}
				else if ((cons[CB_SR] == 0) || (gc_rcb_sr == 0) || (valid_cbpr_car[cb_car] == 1))
				{
					g_rcb_fl[cb_car] = i;		 // assign call to car
					grp_codebf[cb_car] &= 0xFFFE;
				}
		    }
			else
			{
				gc_rcb_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			}
	    }

		for (i=1;i<=cons[nmbcars];i++)
		{
			if ((g_rcb_fl[i] != 0) && ((cons[CB_SR] == 0) || (gc_rcb_sr == 0) || (valid_cbpr_car[i] == 1)))
			{
				fl_ix = (g_rcb_fl[i]-1)/32;

				cb_call = ((gc_rcb[fl_ix] & fl_mask[g_rcb_fl[i]]) != 0);

				if (((grp_Car_Stat[i].g_codebf == 4) || (grp_Car_Stat[i].g_servf != s_CBLUE)) && (cb_call == 0) )
					g_rcb_fl[i] = 0;

				if ((grp_Car_Stat[i].g_servf == s_HSV) && (grp_Car_Stat[i].g_pos == g_rcb_fl[i]) && (grp_Car_Stat[i].g_rdoorf == 2))
				{
					gc_rcb_canc[fl_ix] |= fl_mask[g_rcb_fl[i]];
					g_rcb_fl[i] = 0;		// On hospital service,at the code blue floor and the door is open
				}
			}
		}

  	}
}

//***************************************************
//**** Front Code blue Second Riser Operation *******
//***************************************************

void sr_front_code_blue(void)
{
	int16 i;
	int16 cb_car;
	int16 cb_call;
	int16 fl_ix;
			 

  	for(i=1; i <= cons[grtopf]; i++)
  	{
		fl_ix = (i-1)/32;
	    if((gc_cbsr[fl_ix] & fl_mask[i]) != 0)// check to see if code blue call pushed
		{
			cb_demand = 1;
			gc_cbsr_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
			cb_car = best_sr_cb_car(i,fvars[fvcbsrcar]);
			if (cb_car == 0)
			{
				if (cb_car_on_ind(fvars[fvcbsrcar]) == 0)
					gc_cbsr_canc[fl_ix] |= fl_mask[i];
			}
			else if ((g_cb_fl[cb_car] == i) && (grp_Car_Stat[cb_car].g_pos == i) 
					&& ((grp_Car_Stat[i].g_servf == s_HSV) || (grp_Car_Stat[cb_car].g_codebf == 3) || (grp_Car_Stat[cb_car].g_codebf == 4)))
			{	// cannot assign the call or code blue car is at the floor on code blue or hsv
				gc_cbsr_canc[fl_ix] |= fl_mask[i];
			}
			else if ((cons[CB_SR] == 1) && (gc_cb_sr == 1) && (valid_cbsr_car[cb_car] == 1))
			{
				g_cb_fl[cb_car] = i;			// assign call to car
				grp_codebf[cb_car] &= 0xFFFE;
			}
	    }
	    else
	    {
			gc_cbsr_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
		}
  	}

	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((g_cb_fl[i] != 0) && (cons[CB_SR] == 1) && (gc_cb_sr == 1) && (valid_cbsr_car[i] == 1))
		{
			fl_ix = (g_cb_fl[i]-1)/32;

			cb_call = ((gc_cbsr[fl_ix] & fl_mask[g_cb_fl[i]]) != 0);

			if (((grp_Car_Stat[i].g_codebf == 4) || (grp_Car_Stat[i].g_servf != s_CBLUE)) && (cb_call == 0) )
				g_cb_fl[i] = 0;

			if ((grp_Car_Stat[i].g_servf == s_HSV) && (grp_Car_Stat[i].g_pos == g_cb_fl[i]) && (grp_Car_Stat[i].g_doorf == 2))
			{
				gc_cbsr_canc[fl_ix] |= fl_mask[g_cb_fl[i]];
				g_cb_fl[i] = 0;		// On hospital service and left the code blue floor
			}
		}
	} 
}
//**************************************************
//**** Rear Code blue Second Riser Operation *******
//**************************************************

void sr_rear_code_blue(void)
{
	int16 i;
	int16 cb_car;
	int16 cb_call;
	int16 fl_ix;
			 

	if(grtop_rfl != 0)
	{
	  	for(i=1; i <= cons[grtopf]; i++)
	  	{
			fl_ix = (i-1)/32;

		    if((gc_rcbsr[fl_ix] & fl_mask[i]) != 0)// check to see if up hall call pushed
			{
				cb_demand = 1;
				gc_rcbsr_set[fl_ix] &= ~fl_mask[i];		// reset the set hall call flag
				cb_car = best_sr_rcb_car(i,fvars[fvcbsrcar]);
				if (cb_car == 0) 	 // cannot assign the call
				{
					if (cb_car_on_ind(fvars[fvcbsrcar]) == 0)
						gc_rcbsr_canc[fl_ix] |= fl_mask[i];
				}
				else if ((g_rcb_fl[cb_car] == i) && (grp_Car_Stat[cb_car].g_pos == i) 
					&& ((grp_Car_Stat[i].g_servf == s_HSV) || (grp_Car_Stat[cb_car].g_codebf == 3) || (grp_Car_Stat[cb_car].g_codebf == 4)))
				{	// code blue car is at the floor on code blue or hsv
					gc_rcbsr_canc[fl_ix] |= fl_mask[i];
				}
				else if ((cons[CB_SR] == 1) && (gc_rcb_sr == 1) && (valid_cbsr_car[cb_car] == 1))
				{
					g_rcb_fl[cb_car] = i;		// assign call to car
					grp_codebf[cb_car] &= 0xFFFE;
				}
		    }
			else
			{
				gc_rcbsr_canc[fl_ix] &= ~fl_mask[i];	 // Clear the cancel flag
			}
	    }

		for (i=1;i<=cons[nmbcars];i++)
		{
			if ((g_rcb_fl[i] != 0) && (cons[CB_SR] == 1) && (gc_rcb_sr == 1) && (valid_cbsr_car[i] == 1))
			{
				fl_ix = (g_rcb_fl[i]-1)/32;

				cb_call = ((gc_rcbsr[fl_ix] & fl_mask[g_rcb_fl[i]]) != 0);

				if (((grp_Car_Stat[i].g_codebf == 4) || (grp_Car_Stat[i].g_servf != s_CBLUE)) && (cb_call == 0) )
					g_rcb_fl[i] = 0;

				if ((grp_Car_Stat[i].g_servf == s_HSV) && (grp_Car_Stat[i].g_pos == g_rcb_fl[i]) && (grp_Car_Stat[i].g_rdoorf == 2))
				{
					gc_rcbsr_canc[fl_ix] |= fl_mask[g_rcb_fl[i]];
					g_rcb_fl[i] = 0;		// On hospital service and left the code blue floor
				}
			}
		}

  	}
}

/* 
Revision History

11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.

*/
