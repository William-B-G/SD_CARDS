#define d_GSECURITY 1
#include "global.h"

int16 Grp_CC_Lockouts = 0;	// Group car call lockouts security option flag
int16 Grp_CC_Override = 0;	// Group car call lockout override security option flag
int16 Grp_HC_Lockouts = 0;	// Group hall call lockouts security option flag
int16 Grp_FL_Lockouts = 0;	// Group car and hall call lockouts security option flag


//   SECF Control from the group
unsigned char sec_fl_ctrl[9] = {0,0,0,0,0,0,0,0,0};


// Security flags and masks

uint8  update_hc_sec=0;
uint8  gc_hsec_prev=0;				// Group front hall call security flag
uint32 gc_hc_sec_prev[2]={0,0};		// Hall Call Lockout Security
uint8  gc_rhsec_prev=0;			   	// Group rear hall call security flag
uint32 gc_rhc_sec_prev[2]={0,0};	// Hall Call Lockout Security


uint8  gc_sec=0;					// Group car call lockout security flag
uint32 gc_cc_sec[2]={0,0};			// Car Call Lockout Security
uint8  gc_rsec=0;					// Group rear car call lockout security flag
uint32 gc_rcc_sec[2]={0,0};			// Car Call Lockout Security
uint8  gc_hsec=0;					// Group front hall call security flag
uint8  gc_shsec=0;					// Group serial front hall call security flag
uint32 gc_hc_sec[2]={0,0};			// Hall Call Lockout Security
uint8  gc_rhsec=0;			   		// Group rear hall call security flag
uint8  gc_rshsec=0;			   		// Group rear serial hall call security flag
uint32 gc_rhc_sec[2]={0,0};			// Hall Call Lockout Security

uint8  gc_sab_fl=0;					// Sabath Floor Enable from Group
uint32 gc_sab_fl_msk[2]={0,0};		// Sabath Floor Mask from Group
uint8  gc_sab_rfl=0;				// Sabbath Rear Floor Enable from Group
uint32 gc_sab_rfl_msk[2]={0,0};		// Sabath Rear Floor Mask from Group

uint8 gc_hugsec=0;						// Front hugs security enable
uint8 gc_rhugsec=0;					// Rear hugs security enable
uint32 gc_hugs_sec[2] = {0,0};			// Hugs security mask
uint32 gc_rhugs_sec[2] = {0,0};		// Hugs rear security mask


uint8  gc_sec_ovr=0;						// Group car call security override flag
uint32 gc_cc_sec_ovr[2] = {0,0};			// Group car call security override 
uint32 gc_cc_sec_ovr_set[2] = {0,0};		// Group car call security override output set
uint32 gc_cc_sec_ovr_panic[2] = {0,0};		// Group car call security override panic stop
uint32 gc_cc_sec_ovr_toggle[2] = {0,0};		// Group car call security override toggle 
uint8  gc_rsec_ovr=0;						// Group rear car call security override flag
uint32 gc_rcc_sec_ovr[2] = {0,0};			// Group rear car call security override
uint32 gc_rcc_sec_ovr_set[2] = {0,0};		// Group rear car call security override output set
uint32 gc_rcc_sec_ovr_panic[2] = {0,0};		// Group rear car call security override panic stop
uint32 gc_rcc_sec_ovr_toggle[2] = {0,0};	// Group rear car call security override toggle


unsigned char g_secf_fl[9] = {0,0,0,0,0,0,0,0,0};		// SECF security input flag per car from the group
unsigned char secf_secured_fl[9] = {0,0,0,0,0,0,0,0,0}; // Flag tells wheather security masks already applied

int16 strtup_secf_rst_msks = 0;	// flag to make sure that the secf masks update on group startup
int16 secf_msks_applied = 0;	// Security masks applied flag
int16 on_sec_mode = 0;			// flag sets when any one of the car is secured
int16 strtup_sr_rset_msks = 0;
int16 sr_on_serv = 0;
int32 prev_g_cc_sec[2] = {0,0}; // Previous group car call security flag
int32 gc_ccs_per_fl[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
int32 gc_rccs_per_fl[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
int16 prev_sel_sr_car[9] = {0,0,0,0,0,0,0,0,0};


uint32 ssec_uhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 ssec_dhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 ssec_ruhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 ssec_rdhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};

uint32 flcfg_uhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 flcfg_dhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 flcfg_ruhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 flcfg_rdhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};

uint32 master_uhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 master_dhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 master_ruhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
uint32 master_rdhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};

uint16 hc_car_sec[9] = {0,0,0,0,0,0,0,0,0};
uint16 g_secf_table[5] = {0, 0, 0, 0, 0};


// Group Car Call Override Variables
uint8 cc_sec_ovr_tmr[fl_size];
uint8 rcc_sec_ovr_tmr[fl_size];
uint8 cc_sec_ovr_tmr_en[fl_size];
uint8 rcc_sec_ovr_tmr_en[fl_size];
bool update_ovr_tmr = 0;
int16 second_tmr = 0; 


// Group secf security variable
uint8 g_secf_seq = 0;
uint8 has_assignment[9];

// Group front/rear disable opposite hc variables
int16 update_hc_sec_time = 0;
int16 sec_ftimer[fl_size];		// sec timers per floor for front riser
int16 sec_rtimer[fl_size];		// sec timers per floor for rear riser
int16 sec_ir_ftimer[fl_size];	// sec timers per floor for ir front riser
int16 sec_ir_rtimer[fl_size];	// sec timers per floor for ir rear riser
void hc_sec_timers(void);


void set_master_hc_sec(void);
void group_lockouts(void);
void group_swap_fl_tbl (void);
void group_swap_fl_tbl_2 (void);
void group_swap_fl_tbl_3 (void);
void group_sec_car_fl_keysw (void);
void group_sabbath (void);
void grp_cc_override(void);
void evacuation_svc(void);


//*******************************************************************
// Set master hall call security bit
//*******************************************************************


void set_master_hc_sec(void)
{
	int16 el;
	int16 fl;
	int16 fl_ix;

	
	if (cons[SecFlCfg] == 4)
	{
	 	for(el=1; el<=cons[nmbcars]; el++)
		{
			if ((grp_Car_Stat[el].g_dpref == 0) || ((grp_Car_Stat[el].g_dpref & has_assignment[el]) == 0))
				has_assignment[el] = 0;
		}

		for (fl=1; fl<=cons[grtopf]; fl++)
		{
			fl_ix = (fl-1)/32;
			if ((g_asign_dn[fl] != 0) && (g_asign_dn[fl] <= cons[nmbcars]) && ((fvars[fvhcasgnsectype] & 0x02) != 0))
			{
				if ((master_dhc_carsec[fl_ix][g_asign_dn[fl]] & fl_mask[fl]) != 0)
					has_assignment[g_asign_dn[fl]] |= 2;
			}

			if ((g_asign_up[fl] != 0) && (g_asign_up[fl] <= cons[nmbcars]) && ((fvars[fvhcasgnsectype] & 0x01) != 0))
			{
				if ((master_uhc_carsec[fl_ix][g_asign_up[fl]] & fl_mask[fl]) != 0)
					has_assignment[g_asign_up[fl]] |= 1;
			}
			  
			if ((g_asign_dnr[fl] != 0) && (g_asign_dnr[fl] <= cons[nmbcars]) && ((fvars[fvhcasgnsectype] & 0x08) != 0))
			{
				if ((master_rdhc_carsec[fl_ix][g_asign_dnr[fl]] & fl_mask[fl]) != 0)
					has_assignment[g_asign_dnr[fl]] |= 2;
			}
			if ((g_asign_upr[fl] != 0) && (g_asign_upr[fl] <= cons[nmbcars]) && ((fvars[fvhcasgnsectype] & 0x04) != 0))
			{		// car has an assignment and the assignment is in the floor range
				if ((master_ruhc_carsec[fl_ix][g_asign_upr[fl]] & fl_mask[fl]) != 0)
					has_assignment[g_asign_upr[fl]] |= 1;
			}
		}

	 	for(el=1; el<=cons[nmbcars]; el++)
	 	{
			if (Car_ots_nbp[el] == 1)
			{			 // car out of service but not from load weigher bypass
				if (hc_car_sec[el] == 1)
				{		  // take system off security
					hc_car_sec[el] = 0;
					has_assignment[el] = 0;
					ssec_uhc_carsec[0][el] = 0;
					ssec_uhc_carsec[1][el] = 0;
					ssec_dhc_carsec[0][el] = 0;
					ssec_dhc_carsec[1][el] = 0;
					ssec_ruhc_carsec[0][el] = 0;
					ssec_ruhc_carsec[1][el] = 0;
					ssec_rdhc_carsec[0][el] = 0;
					ssec_rdhc_carsec[1][el] = 0;
				}
			}
			else if (fvars[fvhcasgnsectype] != 0)
			{				  // place system on security
				if (hc_car_sec[el] == 0)
				{
					hc_car_sec[el] = 1;
					ssec_uhc_carsec[0][el] = flcfg_uhc_carsec[0][el];
					ssec_uhc_carsec[1][el] = flcfg_uhc_carsec[0][el];
					ssec_dhc_carsec[0][el] = flcfg_dhc_carsec[0][el];
					ssec_dhc_carsec[1][el] = flcfg_dhc_carsec[0][el];
					ssec_ruhc_carsec[0][el] = flcfg_ruhc_carsec[0][el];
					ssec_ruhc_carsec[1][el] = flcfg_ruhc_carsec[0][el];
					ssec_rdhc_carsec[0][el] = flcfg_rdhc_carsec[0][el];
					ssec_rdhc_carsec[1][el] = flcfg_rdhc_carsec[0][el];
				}
			}
			else
			{		   // take system off security
				if ((hc_car_sec[el] == 1) && (has_assignment[el] == 0))
				{
					hc_car_sec[el] = 0;
					ssec_uhc_carsec[0][el] = 0;
					ssec_uhc_carsec[1][el] = 0;
					ssec_dhc_carsec[0][el] = 0;
					ssec_dhc_carsec[1][el] = 0;
					ssec_ruhc_carsec[0][el] = 0;
					ssec_ruhc_carsec[1][el] = 0;
					ssec_rdhc_carsec[0][el] = 0;
					ssec_rdhc_carsec[1][el] = 0;
				}
			}
		}
	}

 	for(el=1; el<=cons[nmbcars]; el++)
 	{
 		master_uhc_carsec[0][el] = ids_uhc_carsec[0][el] | ui_uhc_carsec[0][el] | ssec_uhc_carsec[0][el];
 		master_uhc_carsec[1][el] = ids_uhc_carsec[1][el] | ui_uhc_carsec[1][el] | ssec_uhc_carsec[1][el];
 		master_dhc_carsec[0][el] = ids_dhc_carsec[0][el] | ui_dhc_carsec[0][el] | ssec_dhc_carsec[0][el];
 		master_dhc_carsec[1][el] = ids_dhc_carsec[1][el] | ui_dhc_carsec[1][el] | ssec_dhc_carsec[1][el];
 		master_ruhc_carsec[0][el] = ids_ruhc_carsec[0][el] | ui_ruhc_carsec[0][el] | ssec_ruhc_carsec[0][el];
 		master_ruhc_carsec[1][el] = ids_ruhc_carsec[1][el] | ui_ruhc_carsec[1][el] | ssec_ruhc_carsec[1][el];
 		master_rdhc_carsec[0][el] = ids_rdhc_carsec[0][el] | ui_rdhc_carsec[0][el] | ssec_rdhc_carsec[0][el];
 		master_rdhc_carsec[1][el] = ids_rdhc_carsec[1][el] | ui_rdhc_carsec[1][el] | ssec_rdhc_carsec[1][el];
 	}
}

// ************************************************
// Group car call lockouts (from hall serial board)
// ************************************************

void group_lockouts(void)
{
	int16 i;
#if (Tract_HR == 1)
	int16 j;
#endif
	int16 updatef;
	static int32 prev_gc_cc_sec[2];
	static int32 prev_gc_rcc_sec[2];
	static int16 prev_gc_sec;
	static int16 prev_gc_rsec;
	static int32 prev_ids_cc_sec[2][car_size];
	static int32 prev_ids_rcc_sec[2][car_size];

	if (Grp_HC_Lockouts == 0)
	{
		gc_hsec = 0;
		gc_rhsec = 0;
	}

	updatef = 0;
#if (Tract_HR == 1)
	for(j=0;j<=1;j++)
	{
		for (i=1;i<=cons[nmbcars];i++)
		{
			if ((prev_ids_cc_sec[j][i] != ids_cc_sec[j][i]) || (prev_ids_rcc_sec[j][i] != ids_rcc_sec[j][i])) 
			{
				updatef = 1;
				break;
			} 
		}
	}
	if ((prev_gc_cc_sec[1] != gc_cc_sec[1]) || (prev_gc_rcc_sec[1] != gc_rcc_sec[1]))
		updatef = 1;
#else
	for (i=1;i<=cons[nmbcars];i++)
	{
		if ((prev_ids_cc_sec[0][i] != ids_cc_sec[0][i]) || (prev_ids_rcc_sec[0][i] != ids_rcc_sec[0][i]))
		{
			updatef = 1;
			break;
		} 
	}
#endif

	if ((prev_gc_cc_sec[0] != gc_cc_sec[0]) || (prev_gc_rcc_sec[0] != gc_rcc_sec[0]) ||
		(prev_gc_sec != gc_sec) || (prev_gc_rsec != gc_rsec) ||	(updatef == 1))
	{
		updatef = 0;
		prev_gc_sec = gc_sec;
		prev_gc_rsec = gc_rsec;
		prev_gc_cc_sec[0] = gc_cc_sec[0];
		prev_gc_rcc_sec[0] = gc_rcc_sec[0];
#if (Tract_HR == 1)
		prev_gc_cc_sec[1] = gc_cc_sec[1];
		prev_gc_rcc_sec[1] = gc_rcc_sec[1];
		for(j=0;j<=1;j++)
		{
			for (i=1;i<=cons[nmbcars];i++)
			{
				prev_ids_cc_sec[j][i] = ids_cc_sec[j][i];
				prev_ids_rcc_sec[j][i] = ids_rcc_sec[j][i];
			}
		}
#else
		for (i=1;i<=cons[nmbcars];i++)
		{
			prev_ids_cc_sec[0][i] = ids_cc_sec[0][i];
			prev_ids_rcc_sec[0][i] = ids_rcc_sec[0][i];
		}
#endif

		if (cons[nmbcars] > 1)
		{
			for (i=1;i<=cons[nmbcars];i++)
			{
				if ((i != cons[carnmb]) && (gc_dev_online[i] == 1)) 
					put_pkt_req(13,i,gc_port);
			}
		}
		timers[tglkout] = 0;
	} 
	
	if (timers[tglkout] > 30)
	{	// update once every three seconds
		if (cons[nmbcars] > 1)
		{
			for (i=1;i<=cons[nmbcars];i++)
			{
				if ((i != cons[carnmb]) && (gc_dev_online[i] == 1)) 
					put_pkt_req(13,i,gc_port);
			}
		}
		timers[tglkout] = 0;
	}
}

//*************************************
// Group operated Sabbath Service
//*************************************

void group_sabbath (void)
{
	int16 no_calls;
	int16 i;

	//  above the lobby	and up collective below the lobby
	no_calls = 1;
	if ((g_up_hall_call[0][0] != 0) || (g_dn_hall_call[0][0] != 0))
		no_calls = 0;
	if ((grtop_rfl != 0) && ((g_upr_hall_call[0][0] != 0) || (g_dnr_hall_call[0][0] != 0)))
		no_calls = 0;
#if (Tract_HR == 1)
	if ((g_up_hall_call[1][0] != 0) || (g_dn_hall_call[1][0] != 0))
		no_calls = 0;
	if ((grtop_rfl != 0) && ((g_upr_hall_call[1][0] != 0) || (g_dnr_hall_call[1][0] != 0)))
		no_calls = 0;
#endif
	if ((no_calls == 1) || (in_grp_sabbath == 0))
	{
		if ((timers[tsabbath] > fvars[fvsabtim]) || (in_grp_sabbath == 0))
		{
			in_grp_sabbath = 1;
			if (cons[SAB_Fl] == 1)
			{
				for(i=1; i <= cons[grtopf]; i++)
				{	// Set down calls for floors above the lobby
					if (sab_ucmsk[i] == 1)
						set_upcall(i);
					if (sab_dcmsk[i] == 1)
						set_dncall(i);
					if (grtop_rfl != 0)
					{
						if (sab_rucmsk[i] == 1)
							set_rupcall(i);
						if (sab_rdcmsk[i] == 1) 
							set_rdncall(i);
					}
				}
			}
			else
			{
				for(i=1; i<=lobby_fl; i++)
				{	// Set up calls for floors below and at the lobby
					set_upcall(i);
					if (grtop_rfl != 0) 
						set_rupcall(i);
				}
				for(i=lobby_fl+1; i <= cons[grtopf]; i++)
				{	// Set down calls for floors above the lobby
					set_dncall(i);
					if (grtop_rfl != 0) 
						set_rdncall(i);
				}
			}
		}
	}
	else
		timers[tsabbath] = 0;
}

//*************************************
// Set mask for group security override
//*************************************

void grp_cc_override(void)
{	
	int16 fl_ix;
	int16 i,j,fl;
	int16 updatef = 0;

	static int16 prev_gc_sec_ovr;
	static int16 prev_gc_rsec_ovr;
	static int32 prev_gc_cc_sec_ovr_set[2];
	static int32 prev_gc_rcc_sec_ovr_set[2];
	static int32 prev_gc_cc_sec_ovr_panic[2];
	static int32 prev_gc_rcc_sec_ovr_panic[2];
	

	if ((Security_Type[cons[carnmb]] & 0x80) == 0)
	{
		gc_sec_ovr = 0;
		gc_cc_sec_ovr[0] = 0;
		gc_cc_sec_ovr[1] = 0;
		gc_cc_sec_ovr_set[0] = 0;
		gc_cc_sec_ovr_set[1] = 0;
		gc_rsec_ovr = 0;
		gc_rcc_sec_ovr[0] = 0;
		gc_rcc_sec_ovr[1] = 0;
		gc_rcc_sec_ovr_set[0] = 0;
		gc_rcc_sec_ovr_set[1] = 0;
	}

	if (timers[tsec] != second_tmr)
		update_ovr_tmr = 1;

	second_tmr = timers[tsec];


	if (fvars[fvgccovride] == 0)
	{

		// Set the latch flag and set the output acknowledge

		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;

			// Activate the override for each floor

			if ((gc_cc_sec_ovr[fl_ix] & fl_mask[i]) != 0) 			// input is on
			{
				if ((gc_cc_sec_ovr_toggle[fl_ix] & fl_mask[i]) == 0)
					gc_cc_sec_ovr_set[fl_ix] |= fl_mask[i];				// turn on the set flag
				else
				{
					gc_cc_sec_ovr_set[fl_ix] &= ~fl_mask[i];			// clear the set flag
					gc_cc_sec_ovr_panic[fl_ix] |= fl_mask[i];			// turn on the panic flag
				}
			}
			else if ((gc_cc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0)
				gc_cc_sec_ovr_toggle[fl_ix] |= fl_mask[i];				// set the toggle flag
			else
			{
				if ((gc_cc_sec_ovr_toggle[fl_ix] & fl_mask[i]) == 0)
					gc_cc_sec_ovr_panic[fl_ix] &= ~fl_mask[i];			// do one iteration then clear the panic flag
				gc_cc_sec_ovr_toggle[fl_ix] &= ~fl_mask[i];			// clear the toggle flag	
			}
			
					 
		  	if (grtop_rfl != 0)
			{
				// Activate the override for each floor

				if ((gc_rcc_sec_ovr[fl_ix] & fl_mask[i]) != 0) 			// input is on
				{
					if ((gc_rcc_sec_ovr_toggle[fl_ix] & fl_mask[i]) == 0)
						gc_rcc_sec_ovr_set[fl_ix] |= fl_mask[i];				// turn on the set flag
					else
					{
						gc_rcc_sec_ovr_set[fl_ix] &= ~fl_mask[i];			// clear the set flag
						gc_rcc_sec_ovr_panic[fl_ix] |= fl_mask[i];			// turn on the panic flag
					}
				}
				else if ((gc_rcc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0)
					gc_rcc_sec_ovr_toggle[fl_ix] |= fl_mask[i];				// set the toggle flag
				else
				{
					if ((gc_rcc_sec_ovr_toggle[fl_ix] & fl_mask[i]) == 0)
						gc_rcc_sec_ovr_panic[fl_ix] &= ~fl_mask[i];			// do one iteration then clear the panic flag
					gc_rcc_sec_ovr_toggle[fl_ix] &= ~fl_mask[i];			// clear the toggle flag
				}			
			}

		}

		// Run once each second

		if (update_ovr_tmr == 1)
		{
			if ((fvars[fvsecfl] >= cons[bottomf]) && (fvars[fvsecfl] <= cons[topf]))
				fl = fvars[fvsecfl];
			else
				fl = fvars[fvlob];					
			
			for (i=1;i<=cons[grtopf];i++)
			{
				fl_ix = (i-1)/32;

				if ((cc_sec_ovr_tmr[i] < 250) && (cc_sec_ovr_tmr_en[i] == 1))
					cc_sec_ovr_tmr[i]++;
				else 
					cc_sec_ovr_tmr[i] = 0;

			  	if (grtop_rfl != 0)
				{
					if ((rcc_sec_ovr_tmr[i] < 250) && (rcc_sec_ovr_tmr_en[i] == 1))
						rcc_sec_ovr_tmr[i]++;
					else
						rcc_sec_ovr_tmr[i] = 0;
				}

				// The override is active so set the timer for each floor
				if ((gc_cc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0) 
				{			// note that a stuck button will work once then the timer will go to max and keep resetting	set
					if (cc_sec_ovr_tmr[i] > fvars[fvgccsect])
					{	 // timer expired so clear the set and the toggle
						gc_cc_sec_ovr_set[fl_ix] &= ~fl_mask[i];		
						gc_cc_sec_ovr_toggle[fl_ix] &= ~fl_mask[i];		
					}
					for (j=1;j<=cons[nmbcars];j++)
					{
				 		if ((grp_Car_Stat[j].g_pos == i) && (grp_Car_Stat[j].g_procf >= 14) && (grp_Car_Stat[j].g_procf <= 16))
				 		{	// Car at the floor and stopped
							if (grp_Car_Stat[j].g_doorf != 0)
							{	  // access was granted
								gc_cc_sec_ovr_set[fl_ix] &= ~fl_mask[i];		
								gc_cc_sec_ovr_toggle[fl_ix] &= ~fl_mask[i];
							}		
						}
						if ((cc_sec_ovr_tmr_en[i] == 0) && (grp_Car_Stat[j].g_pos == fl) && ((grp_Car_Stat[j].g_doorf != 0) || (grp_Car_Stat[j].g_rdoorf != 0)))
							cc_sec_ovr_tmr_en[i] = 1;
					}
				}
				else
				{
					cc_sec_ovr_tmr[i] = 0;
					cc_sec_ovr_tmr_en[i] = 0;
				}

			  	if (grtop_rfl != 0)
				{
					// The override is active so set the timer for each floor
					if ((gc_rcc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0) 
					{			// note that a stuck button will work once then the timer will go to max and keep resetting	set
						if (rcc_sec_ovr_tmr[i] > fvars[fvgccsect])
						{	 // timer expired so clear the set and the toggle
							gc_rcc_sec_ovr_set[fl_ix] &= ~fl_mask[i];		
							gc_rcc_sec_ovr_toggle[fl_ix] &= ~fl_mask[i];		
						}
						for (j=1;j<=cons[nmbcars];j++)
						{
					 		if ((grp_Car_Stat[j].g_pos == i) && (grp_Car_Stat[j].g_procf >= 14) && (grp_Car_Stat[j].g_procf <= 16))
					 		{	// Car at the floor and stopped
								if (grp_Car_Stat[j].g_rdoorf != 0)
								{	// access was granted
									gc_rcc_sec_ovr_set[fl_ix] &= ~fl_mask[i];		
									gc_rcc_sec_ovr_toggle[fl_ix] &= ~fl_mask[i];
								}
							}
							if ((rcc_sec_ovr_tmr_en[i] == 0) && (grp_Car_Stat[j].g_pos == fl) && ((grp_Car_Stat[j].g_doorf != 0) || (grp_Car_Stat[j].g_rdoorf != 0)))
								rcc_sec_ovr_tmr_en[i] = 1;
						}
					}
					else
					{
						rcc_sec_ovr_tmr[i] = 0;
						rcc_sec_ovr_tmr_en[i] = 0;
					}
				}
			}
		}
	}
	else	// bit set for group car call security override direct from switch
	{
		gc_cc_sec_ovr_panic[0] = 0;			// turn off the panic flag
		gc_cc_sec_ovr_panic[1] = 0;			// turn off the panic flag
		gc_rcc_sec_ovr_panic[0] = 0;		// turn off the panic flag
		gc_rcc_sec_ovr_panic[1] = 0;		// turn off the panic flag

		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;

			// Activate the override for each floor

			if ((gc_cc_sec_ovr[fl_ix] & fl_mask[i]) != 0) 			// input is on
				gc_cc_sec_ovr_set[fl_ix] |= fl_mask[i];				// turn on the set flag
			else
				gc_cc_sec_ovr_set[fl_ix] &= ~fl_mask[i];			// clear the set flag
					 
		  	if (grtop_rfl != 0)
			{
				// Activate the override for each floor

				if ((gc_rcc_sec_ovr[fl_ix] & fl_mask[i]) != 0) 			// input is on
					gc_rcc_sec_ovr_set[fl_ix] |= fl_mask[i];				// turn on the set flag
				else
					gc_rcc_sec_ovr_set[fl_ix] &= ~fl_mask[i];			// clear the set flag
			}
		}
	}

	update_ovr_tmr = 0;

	// check for change in status and request transfer of data to car

#if (Tract_HR == 1)
	if ((prev_gc_cc_sec_ovr_set[1] != gc_cc_sec_ovr_set[1]) || (prev_gc_rcc_sec_ovr_set[1] != gc_rcc_sec_ovr_set[1]) ||
		(prev_gc_cc_sec_ovr_panic[1] != gc_cc_sec_ovr_panic[1]) || (prev_gc_rcc_sec_ovr_panic[1] != gc_rcc_sec_ovr_panic[1]))
		updatef = 1;
#endif

	if ((prev_gc_cc_sec_ovr_set[0] != gc_cc_sec_ovr_set[0]) || (prev_gc_rcc_sec_ovr_set[0] != gc_rcc_sec_ovr_set[0]) ||
		(prev_gc_cc_sec_ovr_panic[0] != gc_cc_sec_ovr_panic[0]) || (prev_gc_rcc_sec_ovr_panic[0] != gc_rcc_sec_ovr_panic[0]) ||
		(prev_gc_sec_ovr != gc_sec_ovr) || (prev_gc_rsec_ovr != gc_rsec_ovr) ||	(updatef == 1))
	{
		updatef = 0;
		prev_gc_sec_ovr = gc_sec_ovr;
		prev_gc_rsec_ovr = gc_rsec_ovr;
		prev_gc_cc_sec_ovr_set[0] = gc_cc_sec_ovr_set[0];
		prev_gc_rcc_sec_ovr_set[0] = gc_rcc_sec_ovr_set[0];
#if (Tract_HR == 1)
		prev_gc_cc_sec_ovr_set[1] = gc_cc_sec_ovr_set[1];
		prev_gc_rcc_sec_ovr_set[1] = gc_rcc_sec_ovr_set[1];
#endif

		if (cons[nmbcars] > 1)
		{
			for (i=1;i<=cons[nmbcars];i++)
			{
				if ((i != cons[carnmb]) && (gc_dev_online[i] == 1)) 
					put_pkt_req(15,i,gc_port);
			}
		}
		timers[tgsecovr] = 0;
	} 
	
	if (timers[tgsecovr] > 30)
	{	// update once every three seconds
		if (cons[nmbcars] > 1)
		{
			for (i=1;i<=cons[nmbcars];i++)
			{
				if ((i != cons[carnmb]) && (gc_dev_online[i] == 1)) 
					put_pkt_req(15,i,gc_port);
			}
		}
		timers[tgsecovr] = 0;
	}
}

// ************************************************
// This routine swaps the floor assignment tables
//  for security
// ************************************************

void group_swap_fl_tbl (void)
{
	int16 g_secf_1;
	int16 g_secf_2;
	int16 g_secf_3;
	int16 i;


	g_secf_1 = ((gpi[g_invalid_io_loc+2] & 0x01) != 0) || (tmr_svc[s_alt_fl_sec] == 1);
	g_secf_2 = (gpi[g_invalid_io_loc+2] & 0x02) != 0;
	g_secf_3 = (gpi[g_invalid_io_loc+2] & 0x04) != 0;

	if ((g_secf_2 == 0) && (g_secf_3 == 0))
	{
		if (tmr_svc[s_fl_sec_tbl2] == 1)
		{
			g_secf_2 = 1;
			g_secf_3 = 0;
		}
		else if (tmr_svc[s_fl_sec_tbl3] == 1)
		{
			g_secf_2 = 0;
			g_secf_3 = 1;
		}
		else if (tmr_svc[s_fl_sec_tbl4] == 1)
		{
			g_secf_2 = 1;
			g_secf_3 = 1;
		}
	}

	if (g_secf_seq != 0)
		gpo[g_invalid_io_loc+2] |= 0x20;	// Set SECFO ouput for being on security
	else
		gpo[g_invalid_io_loc+2] &= ~0x20;


	if ((g_secf_3 == 0) && (g_secf_2 == 1) && (cons[SecFlCfg_2] != 0))
	{
		if (g_secf_seq != 2)
		{
			g_secf_seq = 2;
			for(i=1; i<car_size; i++)
			{
				sec_fl_ctrl[i] = 2;
				ucmsk[i] = &ucs2msk[i][0];
				dcmsk[i] = &dcs2msk[i][0];
				rucmsk[i] = &rucs2msk[i][0];
				rdcmsk[i] = &rdcs2msk[i][0];
			}
		}
	}
	else if ((g_secf_3 == 1) && (g_secf_2 == 0) && (cons[SecFlCfg_3] != 0))
	{
		if (g_secf_seq != 3)
		{
			g_secf_seq = 3;
			for(i=1; i<car_size; i++)
			{
				sec_fl_ctrl[i] = 3;
				ucmsk[i] = &ucs3msk[i][0];
				dcmsk[i] = &dcs3msk[i][0];
				rucmsk[i] = &rucs3msk[i][0];
				rdcmsk[i] = &rdcs3msk[i][0];
			}
		}
	}
	else if ((g_secf_3 == 1) && (g_secf_2 == 1) && (cons[SecFlCfg_4] != 0))
	{
		if (g_secf_seq != 4)
		{
			g_secf_seq = 4;
			for(i=1; i<car_size; i++)
			{
				sec_fl_ctrl[i] = 4;
				ucmsk[i] = &ucs4msk[i][0];
				dcmsk[i] = &dcs4msk[i][0];
				rucmsk[i] = &rucs4msk[i][0];
				rdcmsk[i] = &rdcs4msk[i][0];
			}
		}
	}
	else if (g_secf_1 == 1)
	{
		if (g_secf_seq != 1)
		{
			g_secf_seq = 1;
			for(i=1; i<car_size; i++)
			{
				sec_fl_ctrl[i] = 1;
				ucmsk[i] = &ucsmsk[i][0];
				dcmsk[i] = &dcsmsk[i][0];
				rucmsk[i] = &rucsmsk[i][0];
				rdcmsk[i] = &rdcsmsk[i][0];
			}
		}
	}
	else 
	{
		if (g_secf_seq != 0)
		{
			g_secf_seq = 0;
			for(i=1; i<car_size; i++)
			{
				sec_fl_ctrl[i] = 0;
				ucmsk[i] = &ucnmsk[i][0];
				dcmsk[i] = &dcnmsk[i][0];
				rucmsk[i] = &rucnmsk[i][0];
				rdcmsk[i] = &rdcnmsk[i][0];
			}
		}
	}
	g_secf_table[0] = g_secf_seq;
	g_secf_table[1] = sec_fl_ctrl[1];
	g_secf_table[2] = g_secf_1;
	g_secf_table[3] = g_secf_2;
	g_secf_table[4] = g_secf_3;	
}


// ************************************************
// This routine swaps the floor assignment tables
// for security with key switch per car
// ************************************************

void group_swap_fl_tbl_2 (void)
{
	int16 update_msks = 0;
	int16 i;
	
	g_secf_fl[1] = (gpi[g_invalid_io_loc+2] & 0x01) != 0;
	g_secf_fl[2] = (gpi[g_invalid_io_loc+2] & 0x02) != 0;
	g_secf_fl[3] = (gpi[g_invalid_io_loc+2] & 0x04) != 0;
	g_secf_fl[4] = (gpi[g_invalid_io_loc+2] & 0x80) != 0;
	
	if (strtup_secf_rst_msks == 0)
	{	// because dispatcher may change anytime, reset security flag to default stage on startup
		for (i=1; i<=cons[nmbcars]; i++)
		{
			sec_fl_ctrl[i] = 0;
		}
		strtup_secf_rst_msks = 1;
	}
	
	if ((gservf == 0) && (nmb_cars_in_svc > 1))
	{	// no car on fire/emerg service
		for (i=1; i<=cons[nmbcars]; i++)
		{	// check if any input is ON
			if (g_secf_fl[i] == 1)
				update_msks = 1;
		}
	}

	if (update_msks == 1)
	{
		for (i=1; i<=cons[nmbcars]; i++)
		{
			if (grp_Car_Stat[i].g_servf != s_AUTO)
			{	// not on automatic
				g_secf_fl[i] = 0;
			}

			if ((g_secf_fl[i] == 1) && (secf_secured_fl[i] == 0))	// on security
			{
				on_sec_mode = 1;
				secf_secured_fl[i] = 1;
				sec_fl_ctrl[i] = 1;
				ucmsk[i] = &ucsmsk[i][0];
				dcmsk[i] = &dcsmsk[i][0];
				rucmsk[i] = &rucsmsk[i][0];
				rdcmsk[i] = &rdcsmsk[i][0];
			}
			else if ((g_secf_fl[i] == 0) && (secf_secured_fl[i] == 1))
			{
				secf_secured_fl[i] = 0;
				sec_fl_ctrl[i] = 0;
				ucmsk[i] = &ucnmsk[i][0];
				dcmsk[i] = &dcnmsk[i][0];
				rucmsk[i] = &rucnmsk[i][0];
				rdcmsk[i] = &rdcnmsk[i][0];			
			}
		}
	}
	else if (on_sec_mode == 1)
	{
		on_sec_mode = 0;
		for (i=1; i<=cons[nmbcars]; i++)
		{
			secf_secured_fl[i] = 0;
			sec_fl_ctrl[i] = 0;
			ucmsk[i] = &ucnmsk[i][0];
			dcmsk[i] = &dcnmsk[i][0];
			rucmsk[i] = &rucnmsk[i][0];
			rdcmsk[i] = &rdcnmsk[i][0];
		}		
	}		
}

// ***************************************************
// This routine swaps the SR floor assignment tables
// by reading inputs from ICR/ICR2 and car selection
// from fvars[fvircar] and fvars[fvircar2]
// ***************************************************

void group_swap_fl_tbl_3 (void)
{
	int16 i,j;
	int16 sel_sr_car[9] = {0,0,0,0,0,0,0,0,0};	// second riser fvars selected cars

	sel_sr_car[1] = fvars[fvircar];
	sel_sr_car[2] = fvars[fvircar2];
	
	if (((fvars[fvsrctl] & 0x01) != 0) && (gservf == 0))
	{	// parameter is set and not on fire/emerg service
		
		if (strtup_sr_rset_msks == 0)
		{	// first time backup masks before changing them 	

			for (i=1; i<=cons[nmbcars]; i++)
			{	
				for(j=cons[bottomf]; j<=cons[topf]; j++)
				{
					// first time backup and reset masks
					irccmsk2[i][j] = irccmsk[i][j];
					irucmsk2[i][j] = irucmsk[i][j];
					irdcmsk2[i][j] = irdcmsk[i][j];
				}
				// Backup valid second riser car flag
				Valid_SR_Car_Backup[i] = Valid_SR_Car[i];
			}
			strtup_sr_rset_msks = 1;
		}
	
		if (sr_on_serv == 0)
		{	// fvsrctl parameter changed or recoverd on fire/emg service
			
			for (i=1; i<=cons[nmbcars]; i++)
			{	
				for(j=cons[bottomf]; j<=cons[topf]; j++)
				{
					irccmsk[i][j] = 0;
					irucmsk[i][j] = 0;
					irdcmsk[i][j] = 0;
				}
				Valid_SR_Car[i] = 0;
				prev_sel_sr_car[i] = 0;		
			}
			sr_on_serv = 1;
		}

		// 1st Second Riser car

		if ((gc_ir != 0) && (sel_sr_car[1] != 0))
		{	// input is ON and 1st car fvar is also non-zero
			if ((prev_sel_sr_car[1] != sel_sr_car[1]) && (Valid_SR_Car_Backup[sel_sr_car[1]] != 0))
			{	// previously selected car is different from current parameter value and has masks set valid
				
				Valid_SR_Car[sel_sr_car[1]] = 1;
				if (prev_sel_sr_car[1] != prev_sel_sr_car[2])
				{	// previous SR selected car1 is not set as 2nd SR car
					// so clear masks for previous car and set mask for new car 
					Valid_SR_Car[prev_sel_sr_car[1]] = 0;
					for (j=cons[bottomf]; j<=cons[topf]; j++)
					{
						irccmsk[prev_sel_sr_car[1]][j] = 0;
						irucmsk[prev_sel_sr_car[1]][j] = 0;
						irdcmsk[prev_sel_sr_car[1]][j] = 0;
						irccmsk[sel_sr_car[1]][j] = irccmsk2[sel_sr_car[1]][j];
						irucmsk[sel_sr_car[1]][j] = irucmsk2[sel_sr_car[1]][j];
						irdcmsk[sel_sr_car[1]][j] = irdcmsk2[sel_sr_car[1]][j];
					}
				}
				else
				{	// only set mask for new SR car, don't clear masks of previous car
					for (j=cons[bottomf]; j<=cons[topf]; j++)
					{
						irccmsk[sel_sr_car[1]][j] = irccmsk2[sel_sr_car[1]][j];
						irucmsk[sel_sr_car[1]][j] = irucmsk2[sel_sr_car[1]][j];
						irdcmsk[sel_sr_car[1]][j] = irdcmsk2[sel_sr_car[1]][j];
					}
				}
				sr_car = 0;
				prev_sel_sr_car[1] = sel_sr_car[1];
			}
			else if ((Valid_SR_Car_Backup[sel_sr_car[1]] == 0) && (prev_sel_sr_car[1] != 0))
			{	// new selected SR car is invalid
				
				if (prev_sel_sr_car[1] != prev_sel_sr_car[2])
				{	// previous SR selected car1 is not set as 2nd SR car 
					Valid_SR_Car[prev_sel_sr_car[1]] = 0;
					for (j=cons[bottomf]; j<=cons[topf]; j++)
					{
						irccmsk[prev_sel_sr_car[1]][j] = 0;
						irucmsk[prev_sel_sr_car[1]][j] = 0;
						irdcmsk[prev_sel_sr_car[1]][j] = 0;
					}
				}
				sr_car = 0;
				prev_sel_sr_car[1] = 0;
			}
		}
		else if (prev_sel_sr_car[1] != 0)
		{	// input turned off
			
			if (prev_sel_sr_car[1] != prev_sel_sr_car[2])
			{
				Valid_SR_Car[prev_sel_sr_car[1]] = 0;
				for (j=cons[bottomf]; j<=cons[topf]; j++)
				{
					irccmsk[prev_sel_sr_car[1]][j] = 0;
					irucmsk[prev_sel_sr_car[1]][j] = 0;
					irdcmsk[prev_sel_sr_car[1]][j] = 0;
				}
			}
			sr_car = 0;
			prev_sel_sr_car[1] = 0;
		}
		
		// 2nd Second Riser car

		if ((gc_ir_2 != 0) && (sel_sr_car[2] != 0))
		{	// input is ON and fvar is also non-zero
			if ((prev_sel_sr_car[2] != sel_sr_car[2]) && (Valid_SR_Car_Backup[sel_sr_car[2]] != 0))
			{	// previously selected car is different from current parameter value and has masks set valid
				
				Valid_SR_Car[sel_sr_car[2]] = 1;
				if (prev_sel_sr_car[2] != prev_sel_sr_car[1])
				{	// previous SR selected car2 is not set as 1st SR car
					// so clear masks for previous car and set mask for new car 
					Valid_SR_Car[prev_sel_sr_car[2]] = 0;
					for (j=cons[bottomf]; j<=cons[topf]; j++)
					{
						irccmsk[prev_sel_sr_car[2]][j] = 0;
						irucmsk[prev_sel_sr_car[2]][j] = 0;
						irdcmsk[prev_sel_sr_car[2]][j] = 0;
						irccmsk[sel_sr_car[2]][j] = irccmsk2[sel_sr_car[2]][j];
						irucmsk[sel_sr_car[2]][j] = irucmsk2[sel_sr_car[2]][j];
						irdcmsk[sel_sr_car[2]][j] = irdcmsk2[sel_sr_car[2]][j];
					}
				}
				else
				{	// only set mask for new SR car, don't clear masks of previous car
					for (j=cons[bottomf]; j<=cons[topf]; j++)
					{
						irccmsk[sel_sr_car[2]][j] = irccmsk2[sel_sr_car[2]][j];
						irucmsk[sel_sr_car[2]][j] = irucmsk2[sel_sr_car[2]][j];
						irdcmsk[sel_sr_car[2]][j] = irdcmsk2[sel_sr_car[2]][j];
					}
				}
				sr_car = 0;
				prev_sel_sr_car[2] = sel_sr_car[2];
			}
			else if ((Valid_SR_Car_Backup[sel_sr_car[2]] == 0) && (prev_sel_sr_car[2] != 0))
			{	// new selected SR car is invalid
				if (prev_sel_sr_car[2] != prev_sel_sr_car[1])
				{	// previous SR selected car2 is not set as 1st SR car 
					Valid_SR_Car[prev_sel_sr_car[2]] = 0;
					for (j=cons[bottomf]; j<=cons[topf]; j++)
					{
						irccmsk[prev_sel_sr_car[2]][j] = 0;
						irucmsk[prev_sel_sr_car[2]][j] = 0;
						irdcmsk[prev_sel_sr_car[2]][j] = 0;
					}
				}
				sr_car = 0;
				prev_sel_sr_car[2] = 0;
			}
		}
		else if (prev_sel_sr_car[2] != 0)
		{	// input turned off
			if (prev_sel_sr_car[2] != prev_sel_sr_car[1])
			{
				Valid_SR_Car[prev_sel_sr_car[2]] = 0;
				for (j=cons[bottomf]; j<=cons[topf]; j++)
				{
					irccmsk[prev_sel_sr_car[2]][j] = 0;
					irucmsk[prev_sel_sr_car[2]][j] = 0;
					irdcmsk[prev_sel_sr_car[2]][j] = 0;
				}
			}
			sr_car = 0;
			prev_sel_sr_car[2] = 0;
		}
	
	}
	else if (sr_on_serv == 1)
	{	// reset masks to normal ir masks
		for (i=1; i<=cons[nmbcars]; i++)
		{
			for(j=cons[bottomf]; j<=cons[topf]; j++)
			{
				irccmsk[i][j] = irccmsk2[i][j];
				irucmsk[i][j] = irucmsk2[i][j];
				irdcmsk[i][j] = irdcmsk2[i][j];
			}
			// Backup valid second riser car flag
			Valid_SR_Car[i] = Valid_SR_Car_Backup[i];
		}
		sr_on_serv = 0;
	}
}


// ************************************************
// This routine swaps the floor assignment tables
//  for security
// ************************************************

void group_sec_car_fl_keysw (void)
{
	int16 i,j;
	int16 bit_ix = 0;
	int16 byte_ix = 0;
	int16 update_gpi = 0;
	int16 fl_ix = 0;


	bit_ix = 0;
	if (gin_SECFM == 1)
	{

		for (i=0;i<40;i++)
		{
			if (gpi[i] != prev_gpi[i])
				update_gpi = 1;
			prev_gpi[i] = gpi[i];
		}

		if ((update_gpi == 1) || (timers[tgpio] > 30))
		{
			for(j=1; j<=cons[nmbcars]; j++)
			{
			  	for(i=1; i<=cons[grtopf]; i++)        // decide who will take the calls
			  	{
					byte_ix = bit_ix/8;

					fl_ix = (i-1)/32;	// 0 for low rise and 1 for high rise
					
					if ((gpi[byte_ix] & bittbl[bit_ix & 0x07]) != 0)
					{
						ucs4msk[j][i] = 0;
						dcs4msk[j][i] = 0;
						if (cons[SecFlCfg] == 7)
							gc_ccs_per_fl[fl_ix][j] |= fl_mask[i];
					}
					else
					{
						ucs4msk[j][i] = ucnmsk[j][i];
						dcs4msk[j][i] = dcnmsk[j][i];
						if (cons[SecFlCfg] == 7)
							gc_ccs_per_fl[fl_ix][j] &= ~fl_mask[i];
					}
					bit_ix++;
				}
			}
			timers[tgpio] = 0;
		}

		if ((Sec_Fl_Mask == 0) || (update_gpi == 1))
		{
			Sec_Fl_Mask = 1;
			
			for(i=1; i<car_size; i++)
			{
				ucmsk[i] = &ucs4msk[i][0];
				dcmsk[i] = &dcs4msk[i][0];
				rucmsk[i] = &rucs4msk[i][0];
				rdcmsk[i] = &rdcs4msk[i][0];
			}
		}
	}
	else
	{
		if (Sec_Fl_Mask != 0)
		{
			Sec_Fl_Mask = 0;
		    for(i=1; i<car_size; i++)
		    {
				ucmsk[i] = &ucnmsk[i][0];
				dcmsk[i] = &dcnmsk[i][0];
				rucmsk[i] = &rucnmsk[i][0];
				rdcmsk[i] = &rdcnmsk[i][0];
				if (cons[SecFlCfg] == 7)
				{	// clear and send group car call security mask
					for(j=1; j<=cons[grtopf]; j++)
					{
						fl_ix = (j-1)/32;
						gc_ccs_per_fl[fl_ix][i] &= ~fl_mask[j];
					}
					if (i != cons[carnmb])
					{
						if (gc_dev_online[i] == 1)
							put_pkt_req(13,i,gc_port);
					}
				}
			}
		}
	}
}

void evacuation_svc(void)
{
	static int prev_dpref[9];
	int16 el;
	for(el = 1;el<=cons[nmbcars];el++)
	{
		if(grp_Car_Stat[el].g_pos != fvars[fvlob])
		{
			if(grp_Car_Stat[el].g_dpref != prev_dpref[el])
			{
				set_carcall(el,fvars[fvlob]);
			}
			prev_dpref[el] = grp_Car_Stat[el].g_dpref;
		}
	}
	

}


//*************************************************
//*** Opposite Hall Call Disable Timers Update ****
//*************************************************

void hc_sec_timers(void)
{
	int16 i;
	
	if (((timers[tsec] != 0) && (update_hc_sec_time == 1)) || 
		((timers[tsec] == 0) && (update_hc_sec_time == 0)))
	{	// on rising and falling edge update 1 sec timers (every 1 second)
		if ((fvars[fvdisopphc] & 0x01) != 0)
		{	// front hc riser timers
			for (i=1;i<=cons[grtopf];i++)
			{
				if (sec_ftimer[i] < 32000)	// short int max range 32000 so reset timer 
					sec_ftimer[i]++;
				else
					sec_ftimer[i] = 30;
			}
		}
		
		if ((grtop_rfl != 0) && ((fvars[fvdisopphc] & 0x02) != 0))
		{	// rear hc riser timers
			for (i=1;i<=cons[grtopf];i++)
			{
				if (sec_rtimer[i] < 32000)
					sec_rtimer[i]++;
				else
					sec_rtimer[i] = 30;
			}
		}

		if ((fvars[fvdisopphc] & 0x04) != 0)
		{	// ir riser timers
			if ((cons[incrs] != 0) || (cons[Second_Riser] != 0))
			{
				for (i=1;i<=cons[grtopf];i++)
				{
					if (sec_ir_ftimer[i] < 32000)	// short int max range 32000 so reset timer 
						sec_ir_ftimer[i]++;
					else
						sec_ir_ftimer[i] = 30;
				}
			}
		}

		if ((grtop_rfl != 0) && ((fvars[fvdisopphc] & 0x08) != 0))
		{	// ir rear riser timers
			if ((cons[incrs] != 0) || (cons[Second_Riser] != 0))
			{
				for (i=1;i<=cons[grtopf];i++)
				{
					if (sec_ir_rtimer[i] < 32000)
						sec_ir_rtimer[i]++;
					else
						sec_ir_rtimer[i] = 30;
				}
			}
		}
	}
	
	if (timers[tsec] == 0)
		update_hc_sec_time = 1;
	else
		update_hc_sec_time = 0;
}



/* 
Revision History:

2/22/12 v5.54.57 hdl	1. Added a update_hc_sec variable to determine if the security data should be resent to the serial hall drivers.
						2. Blocked the security data from being sent if we are using the second riser security only
8/23/13 v7.0.16 mhd		1. Added changes from GALaxy 3 below:
						   {							
								 v5.55.26 as 		1. Modified group_swap_fl_tbl() for adding logic for SECFO security output.
								 v5.55.36 as		1. Added routine group_swap_fl_tbl_2() to swaps the floor assignment tables for cc/hc security with key switch per car
 								 v5.55.39 as		1. In group_sec_car_fl_keysw() added condition to update hall call masks whenever the masks are modified
 								 v6.00.41 as		1. Added below Galaxy III changes made in v5.55.41 - v5.55.43
													2. Added routine group_swap_fl_tbl_3() to swaps the SR floor assignment tables by reading inputs from ICR/ICR2 and car selection
													   from fvars[fvircar] and fvars[fvircar2]
							}
						   
11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.
12/18/14 v7.1.24 mhd	1. Modified G4 to match changes in the following G3 software modifications: 
								4/09/14 v6.1.47 as		1. Added hc_sec_timers() routine to increment opposite hall call disable timers

*/
