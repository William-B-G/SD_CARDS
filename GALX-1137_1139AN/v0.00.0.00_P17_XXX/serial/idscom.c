//  Comm Routines
#define Baud 0		// Set baud rate for comm 2: 0 = 57600, 1=38400, 2=19200

#define d_IDSCOM 1
#include "global.h"

#if (Little_Endian == 1)
	uint8 endian_tbl[8] = 
	{
		0,1,2,3,0,1,2,3
	};
#else
	uint8 endian_tbl[8] = 
	{
		3,2,1,0,3,2,1,0
	};
#endif

// ids security flags
int32 ids_uc_sec[2] = {0,0};
int32 ids_ruc_sec[2] = {0,0};
int32 ids_dc_sec[2] = {0,0};
int32 ids_rdc_sec[2] = {0,0};

int32 ids_cc_sec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
int32 ids_rcc_sec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};

int32 ids_uhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
int32 ids_dhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
int32 ids_ruhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
int32 ids_rdhc_carsec[2][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};


// ids control flags
int16 ids_ind[9] = {0,0,0,0,0,0,0,0,0};
int16 ids_rtl[9] = {0,0,0,0,0,0,0,0,0};
int16 ids_heof[9] = {0,0,0,0,0,0,0,0,0};
int16 ids_ad_off[9] = {0,0,0,0,0,0,0,0,0};
int16 ids_allw_door[9] = {0,0,0,0,0,0,0,0,0};
int16 ids_return_flr = 0;
int16 ids_return_typ = 0;
int16 g_ids_return = 0;	// group ids special return flag for each car

						// recall floor [6 bits]
						// return type  [1 bit]
						// allow door	[1 bit]
int16 ids_uppeak = 0;
int16 ids_dnpeak = 0;
int16 ids_alt_lby = 0;
int16 ids_alt_park = 0;
int16 ids_ncu = 0;
int16 ids_alt_fl_sec = 0;
int16 ids_refresh = 1;		// refresh the ids system status
int16 ids_FL_Mask = 0;
int16 g_ids_fsx = 0;
int16 ids_online = 0;


// Data structure for Lift-net (Intergrated Display Systems)

	unsigned char buff[1500];
	unsigned char dbuff[1500];


// Group data structure
#define grp 0
#define heartbeat 1
#define gstat 2
#define uhc 8
#define dhc	16
#define uha	24
#define dha	32
#define haw 16		// hall assignment width (up and down)
#define ruhc 152
#define rdhc 160
#define ruha 168
#define rdha 176

// Individual car data starting points
#define c1 300
#define c2 362
#define c3 424
#define c4 486
#define c5 548
#define c6 610
#define c7 672
#define c8 734

// Structure for car data
#define bpi 0			// binary pi
#define stat 2			// status
#define cc 14			// car calls
#define rcc 22			// rear car calls
#define flt 30			// fault buffer
#define svc 55			// service byte

#define	igrp 800
#define igrpreturndo 801
#define igrpuhc	802
#define igrpdhc	810
#define igrpruhc 818
#define igrprdhc 826
#define igrpuhs	 834
#define igrpdhs  842
#define igrpruhs 850
#define igrprdhs 858
#define igrpccs	 866
#define igrprccs 874
#define igrpreturndc 889

//definitions for the offsets of car control bits

#define ic1 890				// must be 890 for new: changed to 878 for test tower
#define ic2 ic1+42			// must be 932
#define ic3 ic2+42			// must be 974
#define ic4 ic3+42			// must be 1016
#define ic5 ic4+42			// must be 1058
#define ic6 ic5+42			// must be 1100
#define ic7 ic6+42			// must be 1142
#define ic8 ic7+42			// must be 1184

//definitions of all the offsets for the hall call security control bits

#define ihc1 1226			   
#define ihc2 ihc1+32		   
#define ihc3 ihc2+32		   
#define ihc4 ihc3+32		   
#define ihc5 ihc4+32		   
#define ihc6 ihc5+32		   
#define ihc7 ihc6+32		   
#define ihc8 ihc7+32

#define icc 4
#define ircc 12
#define iccs 20
#define irccs 28

//car data offsets
int car_off[9]={c1,c1,c2,c3,c4,c5,c6,c7,c8};
//car control offsets
int icar_off[9]={ic1,ic1,ic2,ic3,ic4,ic5,ic6,ic7,ic8};
//car hall call control offsets
int ihcar_off[9]={ihc1,ihc1,ihc2,ihc3,ihc4,ihc5,ihc6,ihc7,ihc8};

#define iuhcs	0
#define idhcs	8
#define iurhcs	16
#define idrhcs	24



unsigned char upd[1200/8];

unsigned char baddr=1;		// Labeled as board address in the ids protocol but used to allow tables greater than 255 
unsigned char addh;			// Temp to transmit address high byte
unsigned char addl;			// Temp to transmit address low byte
unsigned char offsh;		// Temp to transmit offset high byte
unsigned char offsl;		// Temp to transmit offset low byte
unsigned char buffh;		// Temp to transmit buffer data high byte
unsigned char buffl;		// Temp to transmit buffer data low byte
unsigned char calc_CRC;	// CRC calculated value

extern const uint16 bittbl[16];

#define end_m 0x00			// End of Message
#define sbyte_m 0x10		// Send byte
#define refresh_m 0x11		// Refresh all data
#define rbit_m 0x12			// Receive bit set
#define init_m 0x13			// Initialize device
#define rbyte_m 0x14		// Receive byte	   

int ids_good_packet[3];
int rcv_state=0;
int16 ids_seq = 0;
int ids_online_cnt = 0;
unsigned char rcv_calc_CRC;
unsigned char rcv_CRC;

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 0 - 1
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
void init_ids (void);
void ids_comm(void);
void clr_ids_control (void);
void set_grp_ids_data (int16 seq);
void set_car_ids_data (int16 el);
void send_ids_data (void);
void get_ids_command (void);
int16 rcv_ids(void); 
void exec_ids_cmd(int16 offset, char bitno, char value);
void exec_ids_byte_cmd(int16 offset, char value);
void refresh_data(void);
int16 drv_isrxempty(void);
unsigned char ismsgvalid (unsigned char msg);
unsigned char get_msg_len(unsigned char msg, unsigned char nextbyte);
unsigned char crotl (unsigned char value, unsigned char count);

//**********************
// Character rotate left
//**********************
unsigned char crotl (unsigned char value, unsigned char count)
{
	unsigned char i;

	for (i=0;i<count;i++)
	{
		if ((value & 0x80) != 0)
			value = ((value << 1) | 0x01);
		else
			value = value << 1;
	}
	return (value);
}

//*******************************************
// Initialize the IDS remote monitor Protocol
//*******************************************
void init_ids (void)
{
	int16 i;

	rcv_state = 0;
	ids_seq = 0;
	mpnt[USR2COM] = 0;
	for (i=0;i<(int)sizeof(buff);i++)
	{
		buff[i] = 0;
		dbuff[i] = 0;
	}
	refresh_data();
	ids_refresh = 1;
}


//*********************************************
// ** Clear IDS control variables
//*********************************************

void clr_ids_control (void)
{
int16 i,j;

	for (i=0;i<=1;i++)
	{
		// ids security flags
		ids_uc_sec[i] = 0;
		ids_ruc_sec[i] = 0;
		ids_dc_sec[i] = 0;
		ids_rdc_sec[i] = 0;
		for (j=0;j<=8;j++)
		{
			ids_cc_sec[i][j] = 0;
			ids_rcc_sec[i][j] = 0;
			ids_uhc_carsec[i][j] = 0; 
			ids_ruhc_carsec[i][j] = 0;
			ids_dhc_carsec[i][j] = 0; 
			ids_rdhc_carsec[i][j] = 0;
						
		}
		ids_FL_Mask = 1;
	}

	for(j=0;j<=8;j++)
	{
		// ids control flags
		ids_ind[j] = 0;
		ids_rtl[j] = 0;
		ids_heof[j] = 0;
		ids_ad_off[j] = 0;
		ids_allw_door[j] = 0;
	}
	ids_return_flr = 0;
	ids_return_typ = 0;
	ids_uppeak = 0;
	ids_dnpeak = 0;
	ids_alt_lby = 0;
	ids_alt_park = 0;
	ids_ncu = 0;
	ids_alt_fl_sec = 0;
	g_ids_fsx = 0;
}

//******************************************
// Main Call for the IDS Serial Protocol
//******************************************

void ids_comm(void)
{
	if (timers[theartbeat] > 200)
	{
		buff[heartbeat] = (buff[heartbeat] ^ 0x80) & 0x80;
		dbuff[heartbeat] = (uint8)(~buff[heartbeat]);
		timers[theartbeat] = 0;
		if (ids_refresh == 1)
		{
			ids_refresh = 2;
			refresh_data();
		}
		else if (ids_refresh == 2)
		{
			ids_refresh = 0;
			refresh_data();
		}
		if (ids_online_cnt > 1)
		{
			ids_online = 1;
			ids_online_cnt--;
		}
		else if (ids_online_cnt == 1) 
		{
			ids_online = 0;
			ids_online_cnt = 0;
			clr_ids_control();
		}
	}
	if (ids_seq <= 1)
	{
		set_grp_ids_data(ids_seq);		// Set the group data in the buffer
		ids_seq++;
	}
	else if (ids_seq > 1)
	{
		set_car_ids_data(ids_seq-1);		// Set the car data in the buffer
		if ((ids_seq-1) > cons[nmbcars])
			ids_seq = 0;
		else
			ids_seq++;
	}
	get_ids_command();		// Get the ids commands
	send_ids_data();		// Send the data to the ids monitor
}

// *****************************************************
// Set the group and car data in the ids monitor buffer
// *****************************************************

void set_grp_ids_data (int16 seq)
{
	int16 fl,el;
	uint16 ix;
	int16 fbyte;
	int16 fbit;
	int16 fl_ix;

	// Update group status

	ix = gstat;
	if (gservf == 0)
	{
		if ((up_peak == 1) && (dn_peak == 0))
			buff[ix] = 0x01;	// Up peak
		else if ((dn_peak == 1) && (up_peak == 0))
			buff[ix] = 0x02;	// Down Peak
		else if ((up_peak == 1) && (dn_peak == 1))
			buff[ix] = 0x04;	// Heavy Traffic
		else
			buff[ix] = 0x00;  // Normal
	}
	else if (gservf == 1)
		buff[ix] = 0x08;	// Fire Service
	else if (gservf == 2)
		buff[ix] = 0x10;	// Emergency Power
	else if (gservf == 3)
		buff[ix] = 0x18;	// Fire Service	and Emergency Power

	buff[ix+1] = 0;
	buff[ix+2] = 0;
	buff[ix+3] = 0;

	if (seq == 0)
	{
		// Update buffer for group data

	  	for(fl=1; fl<=cons[grtopf]; fl++)        
	  	{
			fbyte = (fl-1)/8;
			fbit = (fl-1) & 0x07;
			fl_ix = (fl-1)/32;
		    if(fl < cons[grtopf])
		    {
				ix = (uint16)(grp+uhc+fbyte);
		      	if((g_up_hall_call[fl_ix][0] & fl_mask[fl]) != 0)  // there is an up call
					buff[ix] |= bittbl[fbit];
				else if ((cons[LbyUpReq] == 1) && (lby_up_req == 1) && (fl == lobby_fl))
					buff[ix] |= bittbl[fbit];
				else
					buff[ix] &= ~bittbl[fbit];		
			}
	    	if(fl > 1)
	    	{
				ix = (uint16)(grp+dhc+fbyte);
		      	if((g_dn_hall_call[fl_ix][0] & fl_mask[fl]) != 0)  // there is a down call
					buff[ix] |= bittbl[fbit];
				else
					buff[ix] &= ~bittbl[fbit];
			}
			for(el=1; el<=cons[nmbcars]; el++)
			{
			    if(fl < cons[grtopf])
			    {
					ix = (uint16)(grp+uha+fbyte+((el-1)*haw));
				  	if((g_up_hall_call[fl_ix][el] & fl_mask[fl]) != 0)  // there is an up call
						buff[ix] |= bittbl[fbit];
					else
						buff[ix] &= ~bittbl[fbit];		
				}
		    	if(fl > 1)
		    	{
					ix = (uint16)(grp+dha+fbyte+((el-1)*haw));
				  	if((g_dn_hall_call[fl_ix][el] & fl_mask[fl]) != 0)  // there is a down call
						buff[ix] |= bittbl[fbit];
					else
						buff[ix] &= ~bittbl[fbit];
				}
			}	
		}
	}

	if (seq == 1)
	{
		if (grtop_rfl > 0)	
		{
		  	for(fl=1; fl<=grtop_rfl; fl++)        
		  	{
				fbyte = (fl-1)/8;
				fbit = (fl-1) & 0x07;
				fl_ix = (fl-1)/32;
			    if(fl <= grtop_rfl)
			    {
					ix = (uint16)(grp+ruhc+fbyte);
			      	if((g_upr_hall_call[fl_ix][0] & fl_mask[fl]) != 0)  // there is an up call
						buff[ix] |= bittbl[fbit];
					else
						buff[ix] &= ~bittbl[fbit];		
				}
		    	if(fl >= grbot_rfl)
		    	{
					ix = (uint16)(grp+rdhc+fbyte);
			      	if((g_dnr_hall_call[fl_ix][0] & fl_mask[fl]) != 0)  // there is a down call
						buff[ix] |= bittbl[fbit];
					else
						buff[ix] &= ~bittbl[fbit];
				}
				for(el=1; el<=cons[nmbcars]; el++)
				{
				    if(fl <= grtop_rfl)
				    {
						ix = (uint16)(grp+ruha+fbyte+((el-1)*haw));
					  	if((g_upr_hall_call[fl_ix][el] & fl_mask[fl]) != 0)  // there is an up call
							buff[ix] |= bittbl[fbit];
						else
							buff[ix] &= ~bittbl[fbit];		
					}
			    	if(fl >= grbot_rfl)
			    	{
						ix = (uint16)(grp+rdha+fbyte+((el-1)*haw));
					  	if((g_dnr_hall_call[fl_ix][el] & fl_mask[fl]) != 0)  // there is a down call
							buff[ix] |= bittbl[fbit];
						else
							buff[ix] &= ~bittbl[fbit];
					}
				}	
			}
		}
	}
}

// *****************************************************
// Set th car data in the ids monitor buffer
// *****************************************************

void set_car_ids_data(int16 el)
{
	int16 i;
	int16 fl;
	uint16 ix;
	int16 fbyte;
	int16 fbit;
	int16 fl_ix;

	// Update buffer for car data

	if ((el >= 1) && (el <= cons[nmbcars]))
	{

		ix = (uint16)(car_off[el]+bpi);

		// Binary PI
		buff[ix] = (uint8)grp_Car_Stat[el].g_pos;
		// Status bits
		ix = (uint16)(car_off[el]+stat);
		// Car power
		if ((grp_Car_Stat[el].g_statusf1 & sf_S10) != 0)
			buff[ix] |= 0x01;
		else
			buff[ix] &= ~0x01;
		// In service
		if ((grp_Car_Stat[el].g_servf == s_AUTO) || (grp_Car_Stat[el].g_servf == s_ATT) || (grp_Car_Stat[el].g_servf == s_HOMING))
			buff[ix] |= 0x02;
		else
			buff[ix] &= ~0x02;
		// Safeties
		if ((grp_Car_Stat[el].g_statusf1 & sf_SS) != 0)
			buff[ix] |= 0x04;
		else
			buff[ix] &= ~0x04;
		// Inspection
		if ((grp_Car_Stat[el].g_servf == s_INSP) && (grp_Car_Stat[el].g_procf == 2))	
			buff[ix] |= 0x08;
		else
			buff[ix] &= ~0x08;

		// Reset mode
		if ((grp_Car_Stat[el].g_servf == s_RESET) && (grp_Car_Stat[el].g_procf == 1))	
			buff[ix] |= 0x10;
		else
			buff[ix] &= ~0x10;

		// Independent
		if (grp_Car_Stat[el].g_servf == s_IND) 
			buff[ix] |= 0x20;
		else
			buff[ix] &= ~0x20;

		// FS1
		if (((grp_Car_Stat[el].g_fire_status & fs_FS) != 0) || ((grp_Car_Stat[el].g_fire_status & fs_MES) == 0) || ((grp_Car_Stat[el].g_fire_status & fs_ALT) == 0) ||
			((grp_Car_Stat[el].g_fire_status & fs_FSX) != 0)	|| ((grp_Car_Stat[el].g_fire_status & fs_HWS) != 0) || ((grp_Car_Stat[el].g_fire_status & fs_MRS) != 0) ||
			(((grp_Car_Stat[el].g_fire_status & fs_HWS2) != 0) && (cons[Smoke_2004] == 1)))
			buff[ix] |= 0x40;
		else
			buff[ix] &= ~0x40;

		// FS2
		if (((grp_Car_Stat[el].g_fire_status & fs_FS2) != 0) || ((grp_Car_Stat[el].g_fire_status & fs_FS2H) != 0))
			buff[ix] |= 0x80;
		else
			buff[ix] &= ~0x80;

		// Status byte 2
		ix++;

		// Stop Switch
		if ((grp_Car_Stat[el].g_statusf1 & sf_STP) != 0)
			buff[ix] |= 0x01;
		else
			buff[ix] &= ~0x01;

		// Auto Door Switch
		if ((grp_Car_Stat[el].g_statusf1 & sf_AD) != 0)
			buff[ix] |= 0x02;
		else
			buff[ix] &= ~0x02;

		// Emergency power car recall
		if (grp_empf[el] == 3)
			buff[ix] |= 0x04;
		else
			buff[ix] &= ~0x04;

		// Emergency power selected to run
		if ((grp_empf[el] == 6) || (grp_empf[el] == 7))
			buff[ix] |= 0x08;
		else
			buff[ix] &= ~0x08;

		// Earthquake Recovery
		if (eqf == 3)
			buff[ix] |= 0x10;
		else
			buff[ix] &= ~0x10;

		// Code Blue
		if (codebf == 1)
			buff[ix] |= 0x20;
		else
			buff[ix] &= ~0x20;
		
		// Load Bypass
		if (grp_Car_Stat[el].g_servf == s_LWBP)			
			buff[ix] |= 0x40;
		else
			buff[ix] &= ~0x40;

		// Alarm Button
		if ((cons[loadw] & 1) == 0)
		{
			if ((grp_Car_Stat[el].g_car_alrm & 0x01) != 0)		
				buff[ix] |= 0x80;
			else
				buff[ix] &= ~0x80;
		}
		else
			buff[ix] &= ~0x80;


		// Status byte 3
		ix++;

		// UL input
		if ((grp_Car_Stat[el].g_run_statusf & rs_UL) != 0)
			buff[ix] |= 0x01;
		else
			buff[ix] &= ~0x01;

		// DZ input
		if ((grp_Car_Stat[el].g_run_statusf & rs_DZ) != 0)
			buff[ix] |= 0x02;
		else
			buff[ix] &= ~0x02;

		// DL input
		if ((grp_Car_Stat[el].g_run_statusf & rs_DL) != 0)
			buff[ix] |= 0x04;
		else
			buff[ix] &= ~0x04;

		// Up direction 
		if (grp_Car_Stat[el].g_dirf == 1)
			buff[ix] |= 0x08;
		else
			buff[ix] &= ~0x08;

		// Down Direction 
		if (grp_Car_Stat[el].g_dirf == 2)
			buff[ix] |= 0x10;
		else
			buff[ix] &= ~0x10;

		// Up direction preference 
		if (grp_Car_Stat[el].g_dpref == 1)
			buff[ix] |= 0x20;
		else
			buff[ix] &= ~0x20;

		// Down Direction preference
		if (grp_Car_Stat[el].g_dpref == 2)
			buff[ix] |= 0x40;
		else
			buff[ix] &= ~0x40;

		if (((grp_Car_Stat[el].g_run_statusf & rs_UP) != 0) || ((grp_Car_Stat[el].g_run_statusf & rs_DNR) != 0))
			buff[ix] |= 0x80;
		else
			buff[ix] &= ~0x80;


		// Status byte 4
		ix++;

		// Gate Switch input
		if ((grp_Car_Stat[el].g_run_statusf & rs_GS) != 0)
			buff[ix] |= 0x01;
		else
			buff[ix] &= ~0x01;
		// Door Lock Bottom input
		if ((grp_Car_Stat[el].g_run_statusf & rs_DLB) != 0)
			buff[ix] |= 0x02;
		else
			buff[ix] &= ~0x02;
		// Door Lock Middle input
		if ((grp_Car_Stat[el].g_run_statusf & rs_DLM) != 0)
			buff[ix] |= 0x04;
		else
			buff[ix] &= ~0x04;
		// Door Lock Top input
		if ((grp_Car_Stat[el].g_run_statusf & rs_DLT) != 0)
			buff[ix] |= 0x08;
		else
			buff[ix] &= ~0x08;
		// Door Open Limit input
		if ((grp_Car_Stat[el].g_run_statusf & rs_DOL) != 0)
			buff[ix] |= 0x10;
		else
			buff[ix] &= ~0x10;
		// Door Closed Limit input
		if ((grp_Car_Stat[el].g_run_statusf & rs_DCL) != 0)
			buff[ix] |= 0x20;
		else
			buff[ix] &= ~0x20;
		//  Electric Eye/Detector Edge input
		if ((grp_Car_Stat[el].g_run_statusf & rs_EE) != 0)
			buff[ix] |= 0x40;
		else
			buff[ix] &= ~0x40;
		// Safety Edge input
		if ((grp_Car_Stat[el].g_run_statusf & rs_SE) != 0)
			buff[ix] |= 0x80;
		else
			buff[ix] &= ~0x80;


		// Status byte 5
		ix++;

		if (rear_op[el] != 0)
		{
			// Rear Gate Switch input
			if ((grp_Car_Stat[el].g_run_statusf & rs_RGS) != 0)
				buff[ix] |= 0x01;
			else
				buff[ix] &= ~0x01;
			// Rear Door Lock Middle input
			if ((grp_Car_Stat[el].g_run_statusf & rs_RLM) != 0)
				buff[ix] |= 0x02;
			else
				buff[ix] &= ~0x02;
			// Rear Door Open Limit input
			if ((grp_Car_Stat[el].g_run_statusf & rs_DOLR) != 0)
				buff[ix] |= 0x04;
			else
				buff[ix] &= ~0x04;
			// Rear Door Closed Limit input
			if ((grp_Car_Stat[el].g_run_statusf & rs_DCLR) != 0)
				buff[ix] |= 0x08;
			else
				buff[ix] &= ~0x08;
			//  Rear Electric Eye/Detector Edge input
			if ((grp_Car_Stat[el].g_run_statusf & rs_EER) != 0)
				buff[ix] |= 0x10;
			else
				buff[ix] &= ~0x10;
			// Rear Safety Edge input
			if ((grp_Car_Stat[el].g_run_statusf & rs_SER) != 0)
				buff[ix] |= 0x20;
			else
				buff[ix] &= ~0x20;
		}
		else buff[ix] = 0;

		// Status byte 6
		ix++;
		uiu.dw = grp_Car_Stat[el].g_statusf1;
		buff[ix] = uiu.b[lb];

		// Status byte 7
		ix++;
		buff[ix] = uiu.b[mlb];

		// Status byte 8
		ix++;
		buff[ix] = uiu.b[mhb];

		// Status byte 9
		ix++;
		buff[ix] = uiu.b[hb];

		// Status byte 10
		ix++;
		buff[ix] = 0;

		// Status byte 11
		ix++;
		buff[ix] = 0;

		// Status byte 12
		ix++;
		buff[ix] = 0;

	
		// Front and rear car calls
		for (fl=1;fl<=cons[grtopf];fl++)
		{
			fbyte = (fl-1)/8;
			fbit = (fl-1) & 0x07;
			fl_ix = (fl-1)/32;
			ix = (uint16)(car_off[el]+cc+fbyte);
			if ((grp_Car_Stat[el].g_cc[fl_ix] & fl_mask[fl]) != 0)
				buff[ix] |= bittbl[fbit];
			else
				buff[ix] &= ~bittbl[fbit];
			if (rear_op[el] != 0)
			{
				ix = (uint16)(car_off[el]+rcc+fbyte);
				if ((grp_Car_Stat[el].g_rcc[fl_ix] & fl_mask[fl]) != 0)
					buff[ix] |= bittbl[fbit];
				else
					buff[ix] &= ~bittbl[fbit];
			}
		}

		// Fault Bytes
		ix = (uint16)(car_off[el]+flt);
		if (grp_Car_Stat[el].g_cur_flt != 0)
		{	   // Have a fault
			fbyte = (grp_Car_Stat[el].g_cur_flt - 1)/8;  // fault byte
			fbit = (grp_Car_Stat[el].g_cur_flt - 1) & 0x07;	  // fault bit
			if (fbyte < 24)
				buff[ix+fbyte] |= bittbl[fbit];  
		}
		else
		{	 // Erase faults
			for (i=0;i<24;i++)
				buff[ix+i] = 0;
		}

		// Car Service
		ix = (uint16)(car_off[el] + svc);
		if (grp_empf[el] ==0)
		{
			buff[ix] = (uint8)grp_Car_Stat[el].g_servf;		// send service flag unless in emp
		}
		else
		{
			if ((grp_empf[el] == 4) || (grp_empf[el] == 5))
			{
				buff[ix] = 64;		// recalled successfully
			}
			else if ((grp_empf[el] == 8) || (grp_empf[el] == 9))
			{
				buff[ix] = 65;		//failed to recall
			}
			else if ((grp_empf[el] == 6) || (grp_empf[el] == 7))			// back in auto so it is ok to send service flag
			{
				buff[ix] = (uint8)grp_Car_Stat[el].g_servf ;
			}
			else
			{
				buff[ix] = (uint8)grp_Car_Stat[el].g_servf ;					//if nothing got sent, sent the regular value
			}

  		}



		// Send group security lockout data for each car
		uiu.dw = ids_cc_sec[0][el];
		ix = (uint16)(icar_off[el] + iccs);
		buff[ix] = uiu.b[lb];
		buff[ix+1] = uiu.b[mlb];
		buff[ix+2] = uiu.b[mhb];
		buff[ix+3] = uiu.b[hb];
#if (Tract_HR == 1)
		uiu.dw = ids_cc_sec[1][el];
		buff[ix+4] = uiu.b[lb];
		buff[ix+5] = uiu.b[mlb];
		buff[ix+6] = uiu.b[mhb];
		buff[ix+7] = uiu.b[hb];
#endif
		uiu.dw = ids_rcc_sec[0][el];
		ix = (uint16)(icar_off[el] + irccs);
		buff[ix] = uiu.b[lb];
		buff[ix+1] = uiu.b[mlb];
		buff[ix+2] = uiu.b[mhb];
		buff[ix+3] = uiu.b[hb];
#if (Tract_HR == 1)
		uiu.dw = ids_rcc_sec[1][el];
		buff[ix+4] = uiu.b[lb];
		buff[ix+5] = uiu.b[mlb];
		buff[ix+6] = uiu.b[mhb];
		buff[ix+7] = uiu.b[hb];
#endif

		// Send group security hall lockout data for each car
		
		//UP FRONT
		uiu.dw = ids_uhc_carsec[0][el];
		ix = (uint16)(ihcar_off[el] + iuhcs);
		buff[ix] = uiu.b[lb];
		buff[ix+1] = uiu.b[mlb];
		buff[ix+2] = uiu.b[mhb];
		buff[ix+3] = uiu.b[hb];
#if (Tract_HR == 1)
		uiu.dw = ids_uhc_carsec[1][el];
		buff[ix+4] = uiu.b[lb];
		buff[ix+5] = uiu.b[mlb];
		buff[ix+6] = uiu.b[mhb];
		buff[ix+7] = uiu.b[hb];
#endif

		//DOWN FRONT

		uiu.dw = ids_dhc_carsec[0][el];
		ix = (uint16)(ihcar_off[el] + idhcs);
		buff[ix] = uiu.b[lb];
		buff[ix+1] = uiu.b[mlb];
		buff[ix+2] = uiu.b[mhb];
		buff[ix+3] = uiu.b[hb];
#if (Tract_HR == 1)
		uiu.dw = ids_dhc_carsec[1][el];
		buff[ix+4] = uiu.b[lb];
		buff[ix+5] = uiu.b[mlb];
		buff[ix+6] = uiu.b[mhb];
		buff[ix+7] = uiu.b[hb];
#endif

		//UP REAR

		uiu.dw = ids_ruhc_carsec[0][el];
		ix = (uint16)(ihcar_off[el] + iurhcs);
		buff[ix] = uiu.b[lb];
		buff[ix+1] = uiu.b[mlb];
		buff[ix+2] = uiu.b[mhb];
		buff[ix+3] = uiu.b[hb];
#if (Tract_HR == 1)
		uiu.dw = ids_ruhc_carsec[1][el];
		buff[ix+4] = uiu.b[lb];
		buff[ix+5] = uiu.b[mlb];
		buff[ix+6] = uiu.b[mhb];
		buff[ix+7] = uiu.b[hb];
#endif

		//DOWN REAR

		uiu.dw = ids_rdhc_carsec[0][el];
		ix = (uint16)(ihcar_off[el] + idrhcs);
		buff[ix] = uiu.b[lb];
		buff[ix+1] = uiu.b[mlb];
		buff[ix+2] = uiu.b[mhb];
		buff[ix+3] = uiu.b[hb];
#if (Tract_HR == 1)
		uiu.dw = ids_rdhc_carsec[1][el];
		buff[ix+4] = uiu.b[lb];
		buff[ix+5] = uiu.b[mlb];
		buff[ix+6] = uiu.b[mhb];
		buff[ix+7] = uiu.b[hb];
#endif



	}

}


// *****************************************************
// Get the ids monitor commands for the group and cars
// *****************************************************

void get_ids_command (void)
{
	int16 ids_msg;
	int16 msg_len;
	int16 ix=0;
	unsigned char value,bitno;
	uint16 brd_addr,offset;
	int16 comnmb = USR2COM;

	msg_len = rcv_ids();
	if (msg_len != 0)
	{		// Have an Ids Packet
		ids_msg = rmbuf[comnmb][0];
		switch(ids_msg)
		{
			case refresh_m:		// Refresh all data request
				refresh_data();
				ids_online_cnt = 3;
				break;
			case rbit_m:		// Receive bit data to set
				if (((rmbuf[comnmb][1] & 0xf0) == 0x20) && ((rmbuf[comnmb][2] & 0xf0) == 0x30))
				{	  // Valid brd address
					brd_addr = (uint16)((rmbuf[comnmb][1] & 0x0f) << 4);
					brd_addr |= (uint16)(rmbuf[comnmb][2] & 0x0f);
					ix = 3;
					while ((rmbuf[comnmb][ix] != 00) && (ix < msg_len))
					{
						if (((rmbuf[comnmb][ix] & 0xf0) == 0x40) && ((rmbuf[comnmb][ix+1] & 0xf0) == 0x50))
						{
							offset = (uint16)((rmbuf[comnmb][ix] & 0x0f) << 4);
							offset |= (uint16)(rmbuf[comnmb][ix+1] & 0x0f);
							if (((rmbuf[comnmb][ix+2] & 0xf0) == 0x40) && ((rmbuf[comnmb][ix+3] & 0xf0) == 0x50))
							{
					   			bitno = (uint8)((rmbuf[comnmb][ix+2] & 0x0f) << 4);
								bitno |= (rmbuf[comnmb][ix+3] & 0x0f);
								if (((rmbuf[comnmb][ix+4] & 0xf0) == 0x40) && ((rmbuf[comnmb][ix+5] & 0xf0) == 0x50))
								{
									value = (uint8)((rmbuf[comnmb][ix+4] & 0x0f) << 4);
									value |= (rmbuf[comnmb][ix+5] & 0x0f);
									offset += ((brd_addr-1) * 100);
									if ((offset >= igrp) || (offset == heartbeat))			  
									{
										exec_ids_cmd(offset,bitno,value);
								  		buff[offset] &= ~bitno;
								  		buff[offset] |= value;
										ix+=6;
										ids_online_cnt = 3;
								  	}
									else break;
								}
								else break;
							}
							else break;
						}
						else break;
					}
				}
			 	break;
			case init_m:		// Initialize device
				break;
			case rbyte_m:		// Receive Byte data
				if (((rmbuf[comnmb][1] & 0xf0) == 0x20) && ((rmbuf[comnmb][2] & 0xf0) == 0x30))
				{	  // Valid brd address
					brd_addr = (uint16)((rmbuf[comnmb][1] & 0x0f) << 4);
					brd_addr |= (uint16)(rmbuf[comnmb][2] & 0x0f);
					ix = 3;
					while ((rmbuf[comnmb][ix] != 00) && (ix < msg_len))
					{
						if (((rmbuf[comnmb][ix] & 0xf0) == 0x40) && ((rmbuf[comnmb][ix+1] & 0xf0) == 0x50))
						{
							offset = (uint16)((rmbuf[comnmb][ix] & 0x0f) << 4);
							offset |= (uint16)(rmbuf[comnmb][ix+1] & 0x0f);
							if (((rmbuf[comnmb][ix+2] & 0xf0) == 0x40) && ((rmbuf[comnmb][ix+3] & 0xf0) == 0x50))
							{
								value = (uint8)((rmbuf[comnmb][ix+2] & 0x0f) << 4);
								value |= (rmbuf[comnmb][ix+3] & 0x0f);
								offset += ((brd_addr-1) * 100);
								if (offset >= igrp)
								{
									exec_ids_byte_cmd(offset, value);							
									buff[offset] = value;
									ix+=4;
									ids_online_cnt = 3;
								}
								else break;
							}
							else break;
						}
						else break;
					}
				}
				break;
			default:
				break;
		}
		mpnt[comnmb] = 0;
	}
}

//***********************************************
// This is receive an IDS message procedure
// it returns the message length when a complete
//  message	is received a 0 when not complete
//***********************************************

int16 rcv_ids(void)
{
  	int16 rbempt=0,valid_msg_len=0;
	int16 comnmb = USR2COM;
	static int16 msg_len;

  	rbempt = isrxempty(comnmb);
  	while((rbempt == 0) && (valid_msg_len == 0))
  	{
	  	rmbuf[comnmb][mpnt[comnmb]] = (uint8)asigetc(comnmb);
	  	rbempt = isrxempty(comnmb);
	  	mpnt[comnmb]++;

		if (mpnt[comnmb] >= (int)sizeof(rmbuf[comnmb]))
		{
			mpnt[comnmb] = 0;
			rcv_state = 0;
			msg_len = 0;
			rbempt = 1;	// exit loop
		}

		switch (rcv_state)
		{
			case 0:			// Look for start of packet
			  	if ((rmbuf[comnmb][0] == rbit_m) || (rmbuf[comnmb][0] == rbyte_m) ||
			  		(rmbuf[comnmb][0] == init_m) || (rmbuf[comnmb][0] == refresh_m))
				{
					rcv_calc_CRC = 0;
					rcv_calc_CRC ^= rmbuf[comnmb][0];
					rcv_calc_CRC = crotl(rcv_calc_CRC,1);
					rcv_state = 1;
					msg_len = 1;
				}
				else
				{
					msg_len = 0;
	  	  			mpnt[comnmb] = 0;
				}

				break;

			case 1:	   // Look for data and CRC 
				if ((rmbuf[comnmb][mpnt[comnmb]-1] & 0xf0) == 0x60)
					rcv_CRC = (uint8)(rmbuf[comnmb][mpnt[comnmb]-1] << 4);
				else if ((rmbuf[comnmb][mpnt[comnmb]-1] & 0xf0) == 0x70)
				{
					rcv_state = 2;
					rcv_CRC |= (rmbuf[comnmb][mpnt[comnmb]-1] & 0x0f);
				}
				else if (((rmbuf[comnmb][mpnt[comnmb]-1] & 0xf0) >= 0x10) && ((rmbuf[comnmb][mpnt[comnmb]-1] & 0xf0) <= 0x70))
				{	// Found data so calculate CRC
					msg_len++;
					rcv_calc_CRC ^= rmbuf[comnmb][mpnt[comnmb]-1];
					rcv_calc_CRC = crotl(rcv_calc_CRC,1);
				}
				else
				{	// error
	  				if(ids_good_packet[comnmb] > 0)
	  			    	ids_good_packet[comnmb]--;
					mpnt[comnmb] = 0;
					rcv_state = 0;
					msg_len = 0;
				}
				break;

			case 2:		// look for end message
				if (rmbuf[comnmb][mpnt[comnmb]-1] == end_m)
				{	// Found end message so check CRC
					if (rcv_calc_CRC == rcv_CRC)
					{
	  				  	if(ids_good_packet[comnmb] < 32000)
	  				    	ids_good_packet[comnmb]++;
						valid_msg_len = msg_len;	
					}
					else
					{
	  			      	if(ids_good_packet[comnmb] > 0)
	  			          	ids_good_packet[comnmb]--;
						mpnt[comnmb] = 0;
						msg_len = 0;
						rbempt = 1;
					}
					rcv_state = 0;
				}
				break;
			default:
				break;
		}
	}
  	return(valid_msg_len);
}


//*************************************
// Execute commnds from the ids monitor
//*************************************

void exec_ids_cmd(int16 offset, char bitno, char value)
{
	int16 el,fl,ix;
	uint32 temp;

	if (offset == heartbeat)
	{
		buff[heartbeat] = (uint8)(value & bitno);
		dbuff[heartbeat] ^= buff[heartbeat];
		ids_online_cnt = 3;
	}
	else if ((offset >= igrp) && (offset <= igrp+1))
	{ 	// Group control bits
		if ((bitno & 0x01) != 0)
		{
			if ((value & 0x01) != 0)
				ids_uppeak = 1;
			else
				ids_uppeak = 0;
		}
		if ((bitno & 0x02) != 0)
		{
			if ((value & 0x02) != 0)
				ids_dnpeak = 1;
			else
				ids_dnpeak = 0;
		}
  		if (cons[SecFlCfg] == 1)
		{
			if ((bitno & 0x04) != 0)
			{
				if ((value & 0x04) != 0)
					ids_alt_fl_sec = 1;
				else
					ids_alt_fl_sec = 0;
			}
		}
		if ((bitno & 0x08) != 0)
		{
			if ((value & 0x08) != 0)
				ids_ncu = 1;
			else
				ids_ncu = 0;
		}
		if ((bitno & 0x10) != 0)
		{
			if ((value & 0x10) != 0)
				ids_alt_lby = 1;
			else
				ids_alt_lby = 0;
		}
		if ((bitno & 0x20) != 0)
		{
			if ((value & 0x20) != 0)
				ids_alt_park = 1;
			else
				ids_alt_park = 0;
		}
		if ((bitno & 0x40) != 0)
		{
			if ((value & 0x40) != 0)
			{
				g_ids_fsx = 1;
			}
			else
			{
				g_ids_fsx = 0;
			}
		}
	}
	else if ((offset >= igrpuhc) && (offset < igrpdhc))
	{	// Group Up hall calls
		if ((value & bitno) != 0)  // bit is set
		{
			fl = (uint16)(((offset-igrpuhc)*8) + (count_bit(bitno) + 1));
			set_upcall(fl);
		}
	}
	else if ((offset >= igrpdhc) && (offset < igrpruhc))
	{	// Group Down hall calls
		if ((value & bitno) != 0)  // bit is set
		{
			fl = (uint16)(((offset-igrpdhc)*8) + (count_bit(bitno) + 1));
			set_dncall(fl);
		}
	}
	else if ((offset >= igrpruhc) && (offset < igrprdhc))
	{	// Group Rear Up hall calls
		if ((value & bitno) != 0)  // bit is set
		{
			fl = (uint16)(((offset-igrpruhc)*8) + (count_bit(bitno) + 1));
			set_rupcall(fl);
		}
	}
	else if ((offset >= igrprdhc) && (offset < igrpuhs))
	{	// Group Rear Down hall calls
		if ((value & bitno) != 0)  // bit is set
		{
			fl = (uint16)(((offset-igrprdhc)*8) + (count_bit(bitno) + 1));
			set_rdncall(fl);
		}
	}
	else if ((offset >= igrpuhs) && (offset < igrpdhs))
	{	// Group Up hall calls Security
		if ((offset - igrpuhs) < 8)
		{
			uiu.dw = ids_uc_sec[(offset - igrpuhs)/4];
			uiu.b[endian_tbl[(offset - igrpuhs)]] = (uint8)((uiu.b[endian_tbl[(offset - igrpuhs)]] & ~bitno) | value);
			ids_uc_sec[(offset - igrpuhs)/4] = uiu.dw;
		}
	}
	else if ((offset >= igrpdhs) && (offset < igrpruhs))
	{	// Group Down hall calls Security
		if ((offset - igrpdhs) < 8)
		{
			uiu.dw = ids_dc_sec[(offset - igrpdhs)/4];
			uiu.b[endian_tbl[(offset - igrpdhs)]] = (uint8)((uiu.b[endian_tbl[(offset - igrpdhs)]] & ~bitno) | value);
			ids_dc_sec[(offset - igrpdhs)/4] = uiu.dw;
		}
	}
	else if ((offset >= igrpruhs) && (offset < igrprdhs))
	{	// Group Rear Up hall calls	Security
		if ((offset - igrpruhs) < 8)
		{
			uiu.dw = ids_ruc_sec[(offset - igrpruhs)/4];
			uiu.b[endian_tbl[(offset - igrpruhs)]] = (uint8)((uiu.b[endian_tbl[(offset - igrpruhs)]] & ~bitno) | value);
			ids_ruc_sec[(offset - igrpruhs)/4] = uiu.dw;
		}
	}
	else if ((offset >= igrprdhs) && (offset < igrpccs))
	{	// Group Rear Down hall calls Security
		if ((offset - igrprdhs) < 8)
		{
			uiu.dw = ids_rdc_sec[(offset - igrprdhs)/4];
			uiu.b[endian_tbl[(offset - igrprdhs)]] = (uint8)((uiu.b[endian_tbl[(offset - igrprdhs)]] & ~bitno) | value);
			ids_rdc_sec[(offset - igrprdhs)/4] = uiu.dw;
		}
	}
	else if ((offset >= igrpccs) && (offset < igrprccs))
	{	// All cars cc sec
		if ((offset - igrpccs) < 8)
		{
			for(el=1;el<=cons[nmbcars];el++)
			{
				uiu.dw = ids_cc_sec[(offset - igrpccs)/4][el];
				uiu.b[endian_tbl[(offset - igrpccs)]] = (uint8)((uiu.b[endian_tbl[(offset - igrpccs)]] & ~bitno) | value);
				ids_cc_sec[(offset - igrpccs)/4][el] = uiu.dw;
				// send confermation back to ids
//				ix = icar_off[el] + iccs + (offset - igrpccs);
//				buff[ix] = (buff[ix] & ~bitno) | value;
			}
		}
	}
	else if ((offset >= igrprccs) && (offset < ic1))
	{	// All cars rcc sec
		if ((offset - igrprccs) < 8)
		{
			for(el=1;el<=cons[nmbcars];el++)
			{
				uiu.dw = ids_rcc_sec[(offset - igrprccs)/4][el];
				uiu.b[endian_tbl[(offset - igrprccs)]] = (uint8)((uiu.b[endian_tbl[(offset - igrprccs)]] & ~bitno) | value);
				ids_rcc_sec[(offset - igrprccs)/4][el] = uiu.dw;
				// send confermation back to ids
//				ix = icar_off[el] + irccs + (offset - igrprccs);
//				buff[ix] = (buff[ix] & ~bitno) | value;
			}
		}
	}
	else if ((offset >= ic1) && (offset < ihc1))
	{
		for(el=1;el<=cons[nmbcars];el++)
		{
			ix = (uint16)icar_off[el];
			if ((offset >= ix) && (offset < (ix+icc)))
			{		// Car command
				if ((bitno & 0x01) != 0)  // bit is set for rtl
				{	
					if ((value & 0x01) != 0)
						ids_rtl[el] = 1;
					else
						ids_rtl[el] = 0;
				}
				if ((bitno & 0x02) != 0)
				{
					if ((value & 0x02) != 0)
						ids_ind[el] = 1;
					else
						ids_ind[el] = 0;
				}
				if ((bitno & 0x04) != 0)
				{
					if ((value & 0x04) != 0)
						ids_ad_off[el] = 1;
					else
						ids_ad_off[el] = 0;
				}
				if ((bitno & 0x08) != 0)
				{
					if ((value & 0x08) != 0)
						ids_heof[el] = 1;
					else
						ids_heof[el] = 0;
				}
				if ((bitno & 0x10) != 0)
				{
					if ((value & 0x10) != 0)
						ids_allw_door[el] = 1;
					else
						ids_allw_door[el] = 0;
				}
			}
			else if ((offset >= (ix+icc)) && (offset < (ix+ircc)))
			{		// Set car call
				if ((value & bitno) != 0)  // bit is set
				{
					fl = (uint16)(((offset-(ix+icc)) * 8) + (count_bit(bitno)+1));
					set_carcall(el,fl);
				}
			}
			else if ((offset >= (ix+ircc)) && (offset < (ix+iccs)))
			{		// Set rear car call
				if ((value & bitno) != 0)  // bit is set
				{
					fl = (uint16)(((offset-(ix+ircc)) * 8) + (count_bit(bitno)+1));
					set_rcarcall(el,(uint16)fl);
				}
			}
			else if ((offset >= (ix+iccs)) && (offset < (ix+irccs)))
			{		// Set front car call security
				if ((offset - (ix+iccs)) < 8)		// can only do 32 floors 
				{
					uiu.dw = ids_cc_sec[(offset - (ix+iccs))/4][el];
					uiu.b[endian_tbl[(offset -(ix+iccs))]] = (uint8)((uiu.b[endian_tbl[(offset -(ix+iccs))]] & ~bitno) | value);
					ids_cc_sec[(offset - (ix+iccs))/4][el] = uiu.dw;
				}
			}
			else if ((offset >= (ix+irccs)) && (offset < (ix+irccs+8)))
			{		// Set rear car call security
				if ((offset - (ix+irccs)) < 8)		// can only do 32 floors 
				{
					uiu.dw = ids_rcc_sec[(offset - (ix+irccs))/4][el];
					uiu.b[endian_tbl[(offset -(ix+irccs))]] = (uint8)((uiu.b[endian_tbl[(offset -(ix+irccs))]] & ~bitno) | value);
					ids_rcc_sec[(offset - (ix+irccs))/4][el] = uiu.dw;
				}
			}
		}
	}
	else if (offset < (int)sizeof(buff))
	{
		ids_FL_Mask = 1;
		for(el=1;el<=cons[nmbcars];el++)
		{
			ix = (uint16)ihcar_off[el];
			if ((offset >= (ix+iuhcs)) && (offset < (ix+idhcs)))
			{
				if((offset - (ix+iuhcs)) < 8)
				{
					uiu.dw = ids_uhc_carsec[(offset - (ix+iuhcs))/4][el];
					temp = uiu.dw;
					uiu.b[endian_tbl[(offset - (ix+iuhcs))]] = (uint8)((uiu.b[endian_tbl[(offset - (ix+iuhcs))]] & ~bitno) | value); 
					ids_uhc_carsec[(offset - (ix+iuhcs))/4][el] = uiu.dw;
					if(uiu.dw == temp)
						dbuff[offset] = (uint8)(buff[offset] & bitno);
				}
			}
			else if ((offset >= (ix+idhcs)) && (offset < (ix+iurhcs)))
			{
				if((offset - (ix+idhcs)) < 8)
				{
					uiu.dw = ids_dhc_carsec[(offset - (ix+idhcs))/4][el];
					temp = uiu.dw;
					uiu.b[endian_tbl[(offset - (ix+idhcs))]] = (uint8)((uiu.b[endian_tbl[(offset - (ix+idhcs))]] & ~bitno) | value); 
					ids_dhc_carsec[(offset - (ix+idhcs))/4][el] = uiu.dw;
					if(uiu.dw == temp)
						dbuff[offset] = (uint8)(buff[offset] & bitno);
				}
			}
			else if ((offset >= (ix+iurhcs)) && (offset < (ix+idrhcs)))
			{
				if((offset - (ix+iurhcs)) < 8)
				{
					uiu.dw = ids_ruhc_carsec[(offset - (ix+iurhcs))/4][el];
					temp = uiu.dw;
					uiu.b[endian_tbl[(offset - (ix+iurhcs))]] = (uint8)((uiu.b[endian_tbl[(offset - (ix+iurhcs))]] & ~bitno) | value); 
					ids_ruhc_carsec[(offset - (ix+iurhcs))/4][el] = uiu.dw;
					if(uiu.dw == temp)
						dbuff[offset] = (uint8)(buff[offset] & bitno);		
				}
			}	
			else if ((offset >= (ix+idrhcs)) && (offset < (ix+idrhcs+8)))
			{
				if((offset - (ix+idrhcs)) < 8)
				{
					uiu.dw = ids_rdhc_carsec[(offset - (ix+idrhcs))/4][el];
					temp = uiu.dw;
					uiu.b[endian_tbl[(offset - (ix+idrhcs))]] = (uint8)((uiu.b[endian_tbl[(offset - (ix+idrhcs))]] & ~bitno) | value); 
					ids_rdhc_carsec[(offset - (ix+idrhcs))/4][el] = uiu.dw;
					if(uiu.dw == temp)
						dbuff[offset] = (uint8)(buff[offset] & bitno);
				}
			}
		}
	}
}		
//*************************************
// Execute commnds from the ids monitor
//*************************************

void exec_ids_byte_cmd(int16 offset, char value)
{
	int16 el,ix;


	if (offset == igrpreturndo)
	{		//update data for emergency special return services with door open
		if ((value != 0) && (value <= cons[grtopf]))
		{
			ids_return_flr = value;
			ids_return_typ = 0x01;
		}
		else if (buff[igrpreturndc] == 0)
		{					 // only clear the data if door close command not being used
			ids_return_flr = 0;
			ids_return_typ = 0x00;
		}
	}
	else if (offset == igrpreturndc)
	{		//update data for emergency special return services with door closed
		if ((value != 0) && (value <= cons[grtopf]))
		{
			ids_return_flr = value;
			ids_return_typ = 0x00;	
		}
		else if (buff[igrpreturndo] == 0)
		{					 // only clear the data if door open command not being used
			ids_return_flr = 0;
			ids_return_typ = 0x00;
		}
	}
	else if ((offset >= igrpuhs) && (offset < igrpdhs))
	{	// Group Up hall calls Security
		if ((offset - igrpuhs) < 8)
		{
			uiu.dw = ids_uc_sec[(offset - igrpuhs)/4];
			uiu.b[endian_tbl[(offset - igrpuhs)]] = value;
			ids_uc_sec[(offset - igrpuhs)/4] = uiu.dw;
		}
	}
	else if ((offset >= igrpdhs) && (offset < igrpruhs))
	{	// Group Down hall calls Security
		if ((offset - igrpdhs) < 8)
		{
			uiu.dw = ids_dc_sec[(offset - igrpdhs)/4];
			uiu.b[endian_tbl[(offset - igrpdhs)]] = value;
			ids_dc_sec[(offset - igrpdhs)/4] = uiu.dw;
		}
	}
	else if ((offset >= igrpruhs) && (offset < igrprdhs))
	{	// Group Rear Up hall calls	Security
		if ((offset - igrpruhs) < 8)
		{
			uiu.dw = ids_ruc_sec[(offset - igrpruhs)/4];
			uiu.b[endian_tbl[(offset - igrpruhs)]] = value;
			ids_ruc_sec[(offset - igrpruhs)/4] = uiu.dw;
		}
	}
	else if ((offset >= igrprdhs) && (offset < igrpccs))
	{	// Group Rear Down hall calls Security
		if ((offset - igrprdhs) < 8)
		{
			uiu.dw = ids_rdc_sec[(offset - igrprdhs)/4];
			uiu.b[endian_tbl[(offset - igrprdhs)]] = value;
			ids_rdc_sec[(offset - igrprdhs)/4] = uiu.dw;
		}
	}
	else if ((offset >= igrpccs) && (offset < igrprccs))
	{	// All cars cc sec
		if ((offset - igrpccs) < 8)
		{
			for(el=1;el<=cons[nmbcars];el++)
			{
				uiu.dw = ids_cc_sec[(offset - igrpccs)/4][el];
				uiu.b[endian_tbl[(offset - igrpccs)]] = value;
				ids_cc_sec[(offset - igrpccs)/4][el] = uiu.dw;
			}
		}
	}
	else if ((offset >= igrprccs) && (offset < ic1))
	{	// All cars rcc sec
		if ((offset - igrprccs) < 8)
		{
			for(el=1;el<=cons[nmbcars];el++)
			{
				uiu.dw = ids_rcc_sec[(offset - igrprccs)/4][el];
				uiu.b[endian_tbl[(offset - igrprccs)]] = value;
				ids_rcc_sec[(offset - igrprccs)/4][el] = uiu.dw;
			}
		}
	}
	else if ((offset >= ic1) && (offset < ihc1))
	{
		for(el=1;el<=cons[nmbcars];el++)
		{
			ix = (uint16)(icar_off[el]);
			if ((offset >= (ix+iccs)) && (offset < (ix+irccs)))
			{		// Set front car call security
				if ((offset - (ix+iccs)) < 8)		// can only do 32 floors 
				{
					uiu.dw = ids_cc_sec[(offset - (ix+iccs))/4][el];
					uiu.b[endian_tbl[(offset -(ix+iccs))]] = value;
					ids_cc_sec[(offset - (ix+iccs))/4][el] = uiu.dw;
				}
			}
			else if ((offset >= (ix+irccs)) && (offset < (ix+irccs+8)))
			{		// Set rear car call security
				if ((offset - (ix+irccs)) < 8)		// can only do 32 floors 
				{
					uiu.dw = ids_rcc_sec[(offset - (ix+irccs))/4][el];
					uiu.b[endian_tbl[(offset -(ix+irccs))]] = value;
					ids_rcc_sec[(offset - (ix+irccs))/4][el] = uiu.dw;
				}
			}
		}
	}
	else if (offset < (int)sizeof(buff))
	{
		ids_FL_Mask = 1;
		for(el=1;el<=cons[nmbcars];el++)
		{
			ix = (uint16)(ihcar_off[el]);
			if ((offset >= (ix+iuhcs)) && (offset < (ix+idhcs)))
			{
				if((offset - (ix+iuhcs)) < 8)
				{
					uiu.dw = ids_uhc_carsec[(offset - (ix+iuhcs))/4][el];
					uiu.b[endian_tbl[(offset - (ix+iuhcs))]] = value; 
					ids_uhc_carsec[(offset - (ix+iuhcs))/4][el] = uiu.dw;
				}
			}
			else if ((offset >= (ix+idhcs)) && (offset < (ix+iurhcs)))
			{
				if((offset - (ix+idhcs)) < 8)
				{
					uiu.dw = ids_dhc_carsec[(offset - (ix+idhcs))/4][el];
					uiu.b[endian_tbl[(offset - (ix+idhcs))]] = value; 
					ids_dhc_carsec[(offset - (ix+idhcs))/4][el] = uiu.dw;
				}				
			}
			else if ((offset >= (ix+iurhcs)) && (offset < (ix+idrhcs)))
			{
				if((offset - (ix+iurhcs)) < 8)
				{
					uiu.dw = ids_ruhc_carsec[(offset - (ix+iurhcs))/4][el];
					uiu.b[endian_tbl[(offset - (ix+iurhcs))]] = value; 
					ids_ruhc_carsec[(offset - (ix+iurhcs))/4][el] = uiu.dw;
				}	
				
			}
			else if ((offset >= (ix+idrhcs)) && (offset < (ix+idrhcs+8)))
			{
				if((offset - (ix+idrhcs)) < 8)
				{
					uiu.dw = ids_rdhc_carsec[(offset - (ix+idrhcs))/4][el];
					uiu.b[endian_tbl[(offset - (ix+idrhcs))]] = value; 
					ids_rdhc_carsec[(offset - (ix+idrhcs))/4][el] = uiu.dw;
				}
			}
		}
	}
}		

//***********************************************		
// Set the xor of all data used to refresh to ids
//***********************************************		

void refresh_data(void)
{
	int16 i,ix,nb_ix,el;

	// Group Status
	for (i=0;i<4;i++)
		dbuff[gstat+i] = (uint8)(~buff[gstat+i]);
	
	nb_ix = (cons[grtopf]-1)/8;
	for (i=0;i<=nb_ix;i++)
		dbuff[grp+uhc+i] = (uint8)(~buff[grp+uhc+i]);
	for (i=0;i<=nb_ix;i++)
		dbuff[grp+dhc+i] = (uint8)(~buff[grp+dhc+i]);

	for(el=1;el<=cons[nmbcars];el++)
	{
		ix = grp+uha+((el-1)*haw);
		for (i=0;i<=nb_ix;i++)
			dbuff[ix+i] = (uint8)(~buff[ix+i]);		
		ix = grp+dha+((el-1)*haw);
		for (i=0;i<=nb_ix;i++)
			dbuff[ix+i] = (uint8)(~buff[ix+i]);	
	}

	if (grtop_rfl > 0)
	{
		nb_ix = (grtop_rfl-1)/8;
		for (i=0;i<=nb_ix;i++)
			dbuff[grp+ruhc+i] = (uint8)(~buff[grp+ruhc+i]);
		for (i=0;i<=nb_ix;i++)
			dbuff[grp+rdhc+i] = (uint8)(~buff[grp+rdhc+i]);

		for(el=1;el<=cons[nmbcars];el++)
		{
			if (rear_op[el] != 0)
			{
				ix = grp+ruha+((el-1)*haw);
				for (i=0;i<=nb_ix;i++)
					dbuff[ix+i] = (uint8)(~buff[ix+i]);		
				ix = grp+rdha+((el-1)*haw);
				for (i=0;i<=nb_ix;i++)
					dbuff[ix+i] = (uint8)(~buff[ix+i]);	
			}	
		}
	}

	// Set refresh flags for the cars
	for (el=1;el<=cons[nmbcars];el++)
	{
		// Binary PI
		ix = (uint16)(car_off[el]+bpi);
		dbuff[ix] = (uint8)(~buff[ix]);

		// Status bits
		ix = (uint16)(car_off[el]+stat);
		for(i=0;i<12;i++)
			dbuff[ix+i] = (uint8)(~buff[ix+i]);

		// Front car calls
		nb_ix = (cons[grtopf]-1)/8;
		ix = (uint16)(car_off[el]+cc);
		for (i=0;i<=nb_ix;i++)
			dbuff[ix+i] = (uint8)(~buff[ix+i]);
		
		// Rear Car Calls
		if (rear_op[el] != 0)
		{
			ix = (uint16)(car_off[el]+rcc);
			for (i=0;i<=nb_ix;i++)
				dbuff[ix+i] = (uint8)(~buff[ix+i]);
		}

		// Fault Bytes
		ix = (uint16)(car_off[el]+flt);
		for (i=0;i<24;i++)
			dbuff[ix+i] = (uint8)(~buff[ix+i]);

		// Car Service
		ix = (uint16)(car_off[el]+svc);
		dbuff[ix] = (uint8)(~buff[ix]);
	}

	// Are we missing data block 800 to 889 here?

	// Send security data back to ids
	for(el=1;el<=cons[nmbcars];el++)
	{
		// send back to ids
		ix = (uint16)(icar_off[el] + iccs);
		dbuff[ix] = (uint8)(~buff[ix]);
		dbuff[ix+1] = (uint8)(~buff[ix+1]);
		dbuff[ix+2] = (uint8)(~buff[ix+2]);
		dbuff[ix+3] = (uint8)(~buff[ix+3]);
		dbuff[ix+4] = (uint8)(~buff[ix+4]);
		dbuff[ix+5] = (uint8)(~buff[ix+5]);
		dbuff[ix+6] = (uint8)(~buff[ix+6]);
		dbuff[ix+7] = (uint8)(~buff[ix+7]);
		ix = (uint16)(icar_off[el] + irccs);
		dbuff[ix] = (uint8)(~buff[ix]);
		dbuff[ix+1] = (uint8)(~buff[ix+1]);
		dbuff[ix+2] = (uint8)(~buff[ix+2]);
		dbuff[ix+3] = (uint8)(~buff[ix+3]);
		dbuff[ix+4] = (uint8)(~buff[ix+4]);
		dbuff[ix+5] = (uint8)(~buff[ix+5]);
		dbuff[ix+6] = (uint8)(~buff[ix+6]);
		dbuff[ix+7] = (uint8)(~buff[ix+7]);
	}
	if((cons[ids] & 4) != 0)
	{
		for(el=1;el<=cons[nmbcars];el++)
		{
			// send back to ids
			//up hall call sec
			ix = (uint16)(ihcar_off[el] + iuhcs);
			dbuff[ix] = (uint8)(~buff[ix]);
			dbuff[ix+1] = (uint8)(~buff[ix+1]);
			dbuff[ix+2] = (uint8)(~buff[ix+2]);
			dbuff[ix+3] = (uint8)(~buff[ix+3]);
			dbuff[ix+4] = (uint8)(~buff[ix+4]);
			dbuff[ix+5] = (uint8)(~buff[ix+5]);
			dbuff[ix+6] = (uint8)(~buff[ix+6]);
			dbuff[ix+7] = (uint8)(~buff[ix+7]);
			//dn hall call sec
			ix = (uint16)(ihcar_off[el] + idhcs);
			dbuff[ix] = (uint8)(~buff[ix]);
			dbuff[ix+1] = (uint8)(~buff[ix+1]);
			dbuff[ix+2] = (uint8)(~buff[ix+2]);
			dbuff[ix+3] = (uint8)(~buff[ix+3]);
			dbuff[ix+4] = (uint8)(~buff[ix+4]);
			dbuff[ix+5] = (uint8)(~buff[ix+5]);
			dbuff[ix+6] = (uint8)(~buff[ix+6]);
			dbuff[ix+7] = (uint8)(~buff[ix+7]);
			//up rear hall call sec
			ix = (uint16)(ihcar_off[el] + iurhcs);
			dbuff[ix] = (uint8)(~buff[ix]);
			dbuff[ix+1] = (uint8)(~buff[ix+1]);
			dbuff[ix+2] = (uint8)(~buff[ix+2]);
			dbuff[ix+3] = (uint8)(~buff[ix+3]);
			dbuff[ix+4] = (uint8)(~buff[ix+4]);
			dbuff[ix+5] = (uint8)(~buff[ix+5]);
			dbuff[ix+6] = (uint8)(~buff[ix+6]);
			dbuff[ix+7] = (uint8)(~buff[ix+7]);
			//dn rear hall call sec
			ix = (uint16)(ihcar_off[el] + idrhcs);
			dbuff[ix] = (uint8)(~buff[ix]);
			dbuff[ix+1] = (uint8)(~buff[ix+1]);
			dbuff[ix+2] = (uint8)(~buff[ix+2]);
			dbuff[ix+3] = (uint8)(~buff[ix+3]);
			dbuff[ix+4] = (uint8)(~buff[ix+4]);
			dbuff[ix+5] = (uint8)(~buff[ix+5]);
			dbuff[ix+6] = (uint8)(~buff[ix+6]);
			dbuff[ix+7] = (uint8)(~buff[ix+7]);
			
		}

	}

}

//***************************************************
// This routine detects the ids data change and sends
// the data to the comm port
//***************************************************

void send_ids_data (void)
{

	unsigned char i; 
	int32 ix;
	int16 comnmb;
	int16 start_com;
	int16 max_baddr;

	comnmb = USR2COM;

	if((cons[ids] & 4) == 0)
		max_baddr = 13;
	else
		max_baddr = 15;
	
	if ((baddr > max_baddr) || (baddr == 0))
		baddr = 1;
	
	while ((baddr <= max_baddr) && (Tranbuf_Empty[comnmb] == 1))
	{
		start_com = 0;
		ix = ((baddr-1) * 100);
		for (i=0;i<100;i++)
		{
			if (buff[ix+i] != dbuff[ix+i])
			{
				if (start_com == 0)
				{
					start_com = 1;
					calc_CRC = 0;
					sendc(comnmb,0x10);		// Start Character
					calc_CRC ^= 0x10;
					calc_CRC = crotl(calc_CRC,1);
					addh = (baddr >> 4) | 0x20;
					addl = (baddr & 0x0f) | 0x30;
					sendc(comnmb,addh);		// Board address
					calc_CRC ^= addh;
					calc_CRC = crotl(calc_CRC,1);
					sendc(comnmb,addl);		// Board Address
					calc_CRC ^= addl;
					calc_CRC = crotl(calc_CRC,1);
				}
				offsh = (i >> 4) | 0x40;
				offsl = (i & 0x0f) | 0x50;
				buffh = ((buff[ix+i] >> 4) & 0x0f) | 0x40;
				buffl = (buff[ix+i] & 0x0f) | 0x50;
				sendc(comnmb,offsh);
				calc_CRC ^= offsh;
				calc_CRC = crotl(calc_CRC,1);
				sendc(comnmb,offsl);
				calc_CRC ^= offsl;
				calc_CRC = crotl(calc_CRC,1);
				sendc(comnmb,buffh);
				calc_CRC ^= buffh;
				calc_CRC = crotl(calc_CRC,1);
				sendc(comnmb,buffl);
				calc_CRC ^= buffl;
				calc_CRC = crotl(calc_CRC,1);
			}
			dbuff[ix+i] = buff[ix+i];
		}
		if (start_com == 1)
		{	// send crc and end of message
			sendc(comnmb,(int16)((calc_CRC >> 4) | 0x60));
			sendc(comnmb,(int16)((calc_CRC & 0x0f) | 0x70));
			sendc(comnmb,0);	   //	End of Message
		}
		baddr++;
	}
}


/* Revision History

4/26/05 v4.15 mhd		1. Send hall call set for lby_up_req.
6/27/05 v4.24 mhd		1. Added hall and car call security over the ids link.
7/14/04 v4.25 mhd		1. Added default statement at the end of all switch case commands is there were none.
9/13/05 V4.32 mhd		1. Adde Alram Button input to car data.
2/15/07 v5.18 mhd		1. Made the floor mask large enough for 60 floors.
2/23/07 v5.19 mhd		1. Added functions for IDS: rtl, ad_off, heof, ind, alt_fl_sec, uppeak and downpeak.
						2. Clear car buffer if car number less than the current car, i.e. car 2 is the group so clear car 1's data.
						3. Refresh buffer multiple times after powerup at 20 second intervals.
3/8/07 v5.19 mhd		4. Made ids_ind, ids_rtl, ids_heof and ids_ad_off array values.
11/28/07 v5.39.4 mhd	1. Use lobby_fl variable instead of fvars[fvlob] so that lobby_fl can be changed to alternate lobby fvars[fvaltlob]. 	
						2. Added alternatel lobby, alternate parking and next up.
6/1/09 v5.48.8 mhd		1. Added HWS2 input.
10/12/09 v5.50.4 mhd	1. Renamed grp_Car_Stat[i][g_statusf] and grp_Car_Stat[i][g_statusf2] to grp_Car_Stat[i].g_statusf1 and grp_Car_Stat[i][g_status1_h].

1/15/10 v5.51.5 hdl		1. Added the changes to receive and process the data for emergency returns. It pertains to offset 801 and 889
							in the car bits we are now also taking the door status (allow door) bit

2.22.10 v5.51.11 mhd	1. Added exec_ids_byte_cmd() routine to secure floor a byte at a time.
						2. Corrected index number for value in get_ids_command() when byte value sent.
						3. Added ids_online_cnt to detect when the ids system is disconnected.
						4. Added clr_ids_control() to clear all ids variables when comm is lost.
8/18/10 v5.52.24 hdl	1. clr_ids_control will always clear services after a minute because the heartbeat was not being processed
						   fixed in get_ids_command by oring the (offset == heartbeat)
11/4/10 v5.52.43 pn     1. In exec_ids_byte_cmd() routine for emergency special return services, 
						   the ids_return_flr was not being set to the value.

11/9/10 v5.52.45 hdl	1. Added offset for hall call lockouts per car. Buffer increased to 1500 and cons[ids] & 4 has to be enabled for new offsets to work.
						2. Changed the software to allow the masks to be changed form lifnet in order to get hcall security per car. Mask is used in assign_hallcall().

2/26/11	v5.53.05 hdl	1. Changed max_baddr from 12 and 14 tio 13 and 15. The last 100 bytes were not being sent. Car call lockouts would not work on any 8 car group for car 8.
2/26/12 v5.54.69 hdl	1. Changed the data being sent for car calls over to the lifnet so it displays car call lights instead of inputs
6/8/12 v5.54.86	 hdl	1. Added some data to be sent instead of the service byt to ids. ^4 and 65 are sent instead of servf for emp recalled sucessfully and failed to recall

6/9/12 v5.54.86 hdl		1. Added logic for fsx to be triggered by liftnet. c_ids_fsx use as a master recall fire switch
7/26/12 v6.0.12 mhd		1. Added modifications from version 5.54.86.
11/12/13 v7.0.28 mhd	1. Modified set_car_ids_data() so that any car can be the group.


*/
