/* 
	DL20 Communications Interface through Serial Expansion port
	
*/


#define d_DL20COM 1
#include "global.h"


// ****************************************************
// DL-20 Display variable
// ****************************************************

unsigned char dl20_com_online = 1;			// DL20 online
unsigned char dl20_online_count = 0;		// DL20 online count
int16 dl20_com_tx_cnt = 0;					// transmit count
int16 dl20_com_rx_cnt = 0;					// receive count
int16 DL20_tx_timer = 0;					// DL20 10 msec transmit timer

unsigned char dl20_com_seq = 0;				// DL20 comm sequence
unsigned char dl20_rcv_seq = 0;				// DL20 receive sequence
unsigned char dl20_packet_rcv = 0;			// DL20 comm packet request flag
unsigned char dl20_packet_req = 0;			// DL20 comm packet request flag
unsigned char dl20_pack_len = 0;			// DL20 comm pack langth
unsigned char dl20_unpack_len = 0;			// DL20 comm unpack length 
unsigned char dl20_rcvbuf[50];				// DL20 packet receive buffer
unsigned char dl20_trmbuf[30];				// DL20 packet transmit buffer
int16 dl20_rcv_ix = 0;						// block receive index
int16 dl20_trm_ix = 0;						// transmit index
int16 dl20_rx_ix = 0;						// packet receive index
unsigned char dl20_pkt_len=0;
unsigned char dl20_rcv_chksum=0;


void Set_DL20_Fault(void);
void Set_DL20_Alarm(void);
void sendc_DL20(int16 comnmb,int16 letter);
void enable_tx_DL20(int16 comnmb);
int16 isrxempty_DL20(int16 comnmb);
int16 asigetc_DL20(int16 comnmb);
int16 istxcomplete_DL20(int16 comnmb);
int16 istxempty_DL20(int16 comnmb);
void DL20_comm(int16 comnmb);

void pack_DL20_com (int16 pkt, int16 comnmb);
void unpack_DL20_com (int pkt);
void receive_DL20_com (int16 comnmb);


int16 voice_on_count = 0;
int16 voice_off_count = 0;
uint8 voice_cmd = 0;
uint8 activate_voice = 0;

// *********************************
// Com for DL-20 Display


void sendc_DL20(int16 comnmb,int16 letter)
{
	sendc(comnmb,letter);
}

void enable_tx_DL20(int16 comnmb)
{
	if (comnmb == USR1COM)
		USR1txEn;
	else
		USR2txEn;
}
int16 isrxempty_DL20(int16 comnmb)
{
	return(isrxempty(comnmb));
}

int16 asigetc_DL20(int16 comnmb)
{
	return(asigetc(comnmb));
}	

int16 istxcomplete_DL20(int16 comnmb)
{
	return(istxempty(comnmb));
}

int16 istxempty_DL20(int16 comnmb)
{
	return (Tranbuf_Empty[comnmb]);
}



// DL20 Fault #						GALaxy Fault Description
//------------    					------------------------

#define dl20_S10 		1	 	// Car Power Loss
#define dl20_HC 		2		// Hall Call Power Loss
#define dl20_SS 		3		// Safety String Fault
#define dl20_READY		4		// Drive Ready Fault
#define dl20_GRIP   	5		// Gripper/Emergency Brake Fault
#define dl20_IO			6		// I/O error during redundancy check
#define dl20_INS		7		// Inspection or Lock Bypass Fault
#define dl20_STP		8		// Stop Switch Open
#define dl20_GDL		9		// Gate Switch or Door Lock Fault
#define dl20_BKS		10		// Brake lift switch error
#define dl20_DBR		11		// DB Resistor Temp. Error
#define dl20_SHUTDN		12		// Shutdown (too many fault runs)
#define dl20_ASTST		13		// Annual Safety Test
#define dl20_TLimit		14		// UT, UTS, DT or DTS limit error
#define dl20_GTS		15		// Gripper Trip Input Off Fault
#define dl20_LEV		16		// Leveling fault
#define dl20_HWINIT		17		// Hardware Init fault
#define dl20_FDC		18		// Front Door Closing Fault
#define dl20_RDC		19		// Rear Door Closing Fault
#define dl20_BKSR		20		// Brake lift switch run error
#define dl20_LearnHW	21		// Learn Hoistway Fault
#define dl20_PWLUPS		22		// Power Loss UPS Fault
#define dl20_AF_SHUTDN	23		// At Floor Shutdown
#define dl20_ResetRun	24		// Reset run fault
#define dl20_GOV		25		// Governor input open
#define dl20_TF			26		// Top Final Limit Open
#define dl20_BF			27		// Bottom Final Limit open
#define dl20_PS			28		// Pit Switch open
#define dl20_HSS		29		// Hoistway Safety
#define dl20_CTS		30		// Car Top Stop switch open
#define dl20_CSS		31		// Car Safety Switch open
#define dl20_RLS		32		// Rail Lock Switch
#define dl20_FFS		33		// Fire Fighter Stop Switch
#define dl20_MRSW		34		// Machine Room Stop Switch
#define dl20_LOWOILSW	35		// Low Oil Switch
#define dl20_STALLED	36		// Stalled Fault
#define dl20_LOWOILPRES 37		// Low oil pressure
#define dl20_HOTOIL		38		// Hot oil
#define dl20_LVolt		39		// Line Voltage Fault
#define dl20_DVolt		40		// Door Voltage Fault
#define dl20_DMO		41		// Door motor overload


uint8 dl20_flt[4] = {0,0,0,0};

// ***************************************************
// Set DL20 fault codes
// ***************************************************

void Set_DL20_Fault(void)
{
	
	int flt_ix = 0;
	
	if ((statusf & sf_S10) != 0)
	{
		dl20_flt[flt_ix++] = dl20_S10;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_HC) != 0)
	{
		dl20_flt[flt_ix++] = dl20_HC;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_SS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_SS;
		if (flt_ix > 3)
			return;
	}

	if ((safety_string_status & sss_GOV) != 0)
	{
		dl20_flt[flt_ix++] = dl20_GOV;
		if (flt_ix > 3)
			return;
	}
	if ((safety_string_status & sss_TF) != 0)
	{
		dl20_flt[flt_ix++] = dl20_TF;
		if (flt_ix > 3)
			return;
	}
	if ((safety_string_status & sss_BF) != 0)
	{
		dl20_flt[flt_ix++] = dl20_BF;
		if (flt_ix > 3)
			return;
	}
	if ((safety_string_status & sss_PS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_PS;
		if (flt_ix > 3)
			return;
	}
	if ((safety_string_status & sss_HSS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_HSS;
		if (flt_ix > 3)
			return;
	}
	if ((safety_string_status & sss_CTS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_CTS;
		if (flt_ix > 3)
			return;
	}
	if ((safety_string_status & sss_CSS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_CSS;				// 10
		if (flt_ix > 3)
			return;
	}
	if ((safety_string_status & sss_RLS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_RLS;
		if (flt_ix > 3)
			return;
	}
	if (((safety_string_status & sss_FFS) != 0)	 || ((statusf & sf_FSTP) != 0))
	{
		dl20_flt[flt_ix++] = dl20_FFS;
		if (flt_ix > 3)
			return;
	}
	if ((safety_string_status & sss_MRSW) != 0)
	{
		dl20_flt[flt_ix++] = dl20_MRSW;
		if (flt_ix > 3)
			return;
	}

	if ((statusf & sf_READY) != 0)
	{
		dl20_flt[flt_ix++] = dl20_READY;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_GRIP) != 0)
	{
		dl20_flt[flt_ix++] = dl20_GRIP;
		if (flt_ix > 3)
			return;
	}
		
	if ((statusf & sf_IO) != 0)
	{
		dl20_flt[flt_ix++] = dl20_IO;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_INS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_INS;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_STP) != 0)
	{
		dl20_flt[flt_ix++] = dl20_STP;
		if (flt_ix > 3)
			return;
	}
	
	if ((statusf2 & sf_HWINIT) != 0)
	{
		dl20_flt[flt_ix++] = dl20_HWINIT;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_LearnHW) != 0)
	{
		dl20_flt[flt_ix++] = dl20_LearnHW;			// 20
		if (flt_ix > 3)
			return;
	}
	
	if (procf == 17)
	{			// Low oil / Stalled
		if ((cons[Low_Oil] == 1) &&
				(((rdinp(i_LOS) == 1) && (fvars[fvlos] == 0)) || 
			 	 ((rdinp(i_LOS) == 0) && (fvars[fvlos] == 1))) ) 
			dl20_flt[flt_ix++] = dl20_LOWOILSW;
		else
			dl20_flt[flt_ix++] = dl20_STALLED;
		if (flt_ix > 3)
			return;
	}
	if (procf == 19)
	{			// Low oil pressure mode
		dl20_flt[flt_ix++] = dl20_LOWOILPRES;
		if (flt_ix > 3)
			return;
	}

	if (procf == 22)
	{			// Hot oil mode
		dl20_flt[flt_ix] = dl20_HOTOIL;
		if (flt_ix > 3)
			return;
	}
				

	if ((statusf2 & sf_LVolt) != 0)
	{
		dl20_flt[flt_ix++] = dl20_LVolt;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_DVolt) != 0)
	{
		dl20_flt[flt_ix++] = dl20_DVolt;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_DMO) != 0)
	{
		dl20_flt[flt_ix++] = dl20_DMO;
		if (flt_ix > 3)
			return;
	}

	if ((statusf & sf_BKS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_BKS;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_BKSR) != 0)
	{
		dl20_flt[flt_ix++] = dl20_BKSR;
		if (flt_ix > 3)
			return;
	}
	
	if ((statusf & sf_GTS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_GTS;			   	// 30
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_LEV) != 0)
	{
		dl20_flt[flt_ix++] = dl20_LEV;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_GDL) != 0)
	{
		dl20_flt[flt_ix++] = dl20_GDL;
		if (flt_ix > 3)
			return;
	}

	if ((statusf & sf_DBR) != 0)
	{
		dl20_flt[flt_ix++] = dl20_DBR;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_SHUTDN) != 0)
	{
		dl20_flt[flt_ix++] = dl20_SHUTDN;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_ASTST) != 0)
	{
		dl20_flt[flt_ix++] = dl20_ASTST;
		if (flt_ix > 3)
			return;
	}
	if ((statusf & sf_TLimit) != 0)
	{
		dl20_flt[flt_ix++] = dl20_TLimit;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_FDC) != 0)
	{
		dl20_flt[flt_ix++] = dl20_FDC;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_RDC) != 0)
	{
		dl20_flt[flt_ix++] = dl20_RDC;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_PWLUPS) != 0)
	{
		dl20_flt[flt_ix++] = dl20_PWLUPS;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_AF_SHUTDN) != 0)
	{
		dl20_flt[flt_ix++] = dl20_AF_SHUTDN;
		if (flt_ix > 3)
			return;
	}
	if ((statusf2 & sf_ResetRun) != 0)
	{
		dl20_flt[flt_ix++] = dl20_ResetRun;
		if (flt_ix > 3)
			return;
	}
	
}




// DL20 Alarm #				GALaxy Alarm Description
// ------------    			------------------------
#define dl20_BPI	1			// Binary Position Input Error
#define dl20_POS	2			// Position Error
#define dl20_AD		3			// No automatic Doors
#define dl20_DZ		4			// Door Zone fault
#define dl20_P		5			// No Potential Input
#define dl20_DCL	6			// No DCL
#define dl20_DCC	7			// No Door Close Contact
#define dl20_TOC	8			// Top of Car Com Error
#define dl20_DRV	9			// Drive Com Error
#define dl20_SPB	10			// Safety Processor Board Can Com Error
#define dl20_ULDLDZ	11			// UL, DL and DZ off at floor
#define dl20_BKCAN	12			// Brake Board Can Com Error
#define dl20_SELCAN	13			// Selector Can Com Error
#define dl20_ULDL	14			// UL or DL fault
#define dl20_EBKCAN	15			// Emergency Brake Can Com Error
#define dl20_KEBRUN	16			// KEB Drive Not in Run Mode
#define dl20_CtlFlt	17			// Controller Fault/Alarm Occurred 
	

uint8 dl20_alarm[4] = {0,0,0,0};

// ***************************************************
// Set DL20 alarm codes
// ***************************************************


void Set_DL20_Alarm(void)
{
	
	int alarm_ix = 0;

	if ((statusf & sf_P) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_P;
		if (alarm_ix > 3)
			return;
	}
		
	
	if ((statusf & sf_DZ) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_DZ;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_DCL) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_DCL;
		if (alarm_ix > 3)
			return;
	}
	
	if ((statusf & sf_DCC) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_DCC;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_TOC) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_TOC;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_DRV) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_DRV;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_SPB) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_SPB;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_BKCAN) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_BKCAN;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_SELCAN) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_SELCAN;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf2 & sf_EBKCAN) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_EBKCAN;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf2 & sf_KEBRUN) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_KEBRUN;
		if (alarm_ix > 3)
			return;
	}

	if ((statusf & sf_ULDLDZ) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_ULDLDZ;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_ULDL) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_ULDL;
		if (alarm_ix > 3)
			return;
	}

	if ((statusf & sf_BPI) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_BPI;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_POS) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_POS;
		if (alarm_ix > 3)
			return;
	}
	if ((statusf & sf_AD) != 0)
	{
		dl20_alarm[alarm_ix++] = dl20_AD;
		if (alarm_ix > 3)
			return;
	}
			
	if (Current_Fault != 0)
	{	// new current fault
		dl20_alarm[alarm_ix] = dl20_CtlFlt;
		if (alarm_ix > 3)
			return;
	}
}



uint8 elevator_disable = 0;
uint8 phone_test_failed = 0;


void pack_DL20_com (int16 pkt, int16 comnmb)
{
	int16 i;
	int16 valid_packet = 0;
	unsigned char chksum = 0;
	uint8 status = 0;
	static int16 prev_pos;
	static int16 prev_firef;
	static int16 prev_eqf;
	static int16 prev_medf;
	static int16 prev_empf1;
	static int16 prev_empf2;
	static int16 prev_do_seq;
	static int16 prev_rdo_seq;
	static int16 prev_servf;
	static int16 prev_procf;
	static int16 prev_doorf;
	static int16 prev_rdoorf;
	static int16 prev_dpref;
	static int16 over_load;
	static int16 prev_over_load;
	

	switch (pkt)
	{
		case 1: 								// Send status information
			dl20_trmbuf[0] = (uint8)170;				// start flag '['
			dl20_trmbuf[1] = (uint8)position;					// packet length gets added later
			if (dpref == 1)
				dl20_trmbuf[2] = (uint8)1;					// packet number
			else if (dpref == 2)
				dl20_trmbuf[2] = (uint8)2;					// packet number
			else
				dl20_trmbuf[2] = (uint8)3;					// packet number
			status = 0;
			if (rdinp(i_ICI) == 1)
				status |= 0x01;
			if (rdinp(i_CTIN) == 1)
				status |= 0x02;
			if (rdinp(i_MRIN) == 1)
				status |= 0x04;
			if (rdinp(i_ACC) == 1)
				status |= 0x08;
			if (medf != 0)
				status |= 0x10;
			if (firef != 0)
				status |= 0x20;
			if (eqf != 0)
				status |= 0x40;
			dl20_trmbuf[3] = status;		

			status = 0;
			if (rdoutp(o_FBNB) == 1)
				status |= 0x01;
//			if (rdoutp(o_HB) == 1)
//				status |= 0x02;		// second buzzer not being used
			if ((rdinp(i_DCL) == 1) || ((cons[rear] != 0) && (rdinp(i_DCLR) == 1)))
				status |= 0x04;
			if ((rdinp(i_OVL) == 1) || (load_ovl == 1))
			{
				over_load = 1;
				status |= 0x08;
			}
			else 
				over_load = 0;
			
			dl20_trmbuf[4] = status;
			
			// voice
			if ((activate_voice == 0) && (voice_off_count > 3))
			{
				
				if ((procf == 2) && (procf != prev_procf))
				{
					voice_cmd = 11;		// Inspection Service
					activate_voice = 1;
					prev_procf = procf;				
				}			
				else if ((firef == 1) && (firef != prev_firef))
				{
					voice_cmd = 5;		// fire service
					activate_voice = 1;		
					prev_firef = firef;				
				}			
				else if ((eqf == 1) && (eqf != prev_eqf))
				{
					voice_cmd = 6;		// Seismic Service
					activate_voice = 1;
					prev_eqf = eqf;				
				}
				else if ((medf == 1) && (medf != prev_medf))
				{
					voice_cmd = 7;		//  Medical emergency
					activate_voice = 1;
					prev_medf = medf;				
				}
				else if ((over_load != prev_over_load) && ((doorf != 0) || (rdoorf != 0)))
				{
					voice_cmd = 8;				// car overload
					activate_voice = 1; 
					prev_over_load = over_load;				
				}			
				else if (((empf == 1) || (empf == 2)) && ((prev_empf1 != 1) || (prev_empf1 != 2)))
				{
					voice_cmd = 9;			// emergency power
					activate_voice = 1;
					prev_empf1 = empf;				
				}
				else if ((empf == 3) && (empf != prev_empf2))
				{
					voice_cmd = 10;			// emergency power please exit
					activate_voice = 1;
					prev_empf2 = empf;				
				}
				else if ((servf == s_IND) && (servf != prev_servf))
				{
					voice_cmd = 12;		// Indenpendent
					activate_voice = 1;
					prev_servf = servf;				
				}			
				else if (rdoutp(o_HB) == 1)
				{
					voice_cmd = 13; // floor chime
					activate_voice = 1;
				}
				else if ((position != prev_pos) && (procf == 15) && ((doorf != 0) || (rdoorf != 0)))
				{
					voice_cmd = 1;				// announce floor
					activate_voice = 1; 
					prev_pos = position;				
				}			
				else if (((do_seq != prev_do_seq) || ((cons[rear] != 0) && (rdo_seq != prev_rdo_seq))) ||
						(dpref != prev_dpref))
				{
					if ( ((do_seq != 0) && (doorf != 1)) || ((cons[rear] != 0) && (rdo_seq != 0) && (rdoorf != 1)) )
					{
						if (dpref == 1)
							voice_cmd = 2;		// going up
						else if (dpref == 2)
							voice_cmd = 3;		// going down
						else 
							voice_cmd = 0;
						if (voice_cmd != 0)
							activate_voice = 1;
					}
					prev_do_seq = do_seq;
					prev_rdo_seq = rdo_seq;
					prev_dpref = dpref;
				}
				else if ( ((doorf == 4) && (doorf != prev_doorf)) || 
						  ((cons[rear] != 0) && (rdoorf == 4) && (rdoorf != prev_rdoorf)) )
				{
					voice_cmd = 4;			// Nudging
					activate_voice = 1;
					prev_doorf = doorf;
					prev_rdoorf = rdoorf;
				}
			}
			
			if (activate_voice == 1)
			{
				voice_off_count = 0;
				if (voice_on_count > 4)
				{
					voice_on_count = 0;
					activate_voice = 0;
				}
				else
					voice_on_count ++;
			}
			else
			{
				if (voice_off_count < 5)
					voice_off_count++;
				voice_cmd = 0;
				voice_on_count = 0;
			}
					
			dl20_trmbuf[5] = voice_cmd;
			
			dl20_alarm[0] = 0;
			dl20_alarm[1] = 0;
			dl20_alarm[2] = 0;
			dl20_alarm[3] = 0;
			
			// Place only 4 alarms in buffer
			Set_DL20_Alarm();
			// alarms		
			dl20_trmbuf[6] = dl20_alarm[0];		
			dl20_trmbuf[7] = dl20_alarm[1];		
			dl20_trmbuf[8] = dl20_alarm[2];		
			dl20_trmbuf[9] = dl20_alarm[3];		
			
				
			dl20_flt[0] = 0;
			dl20_flt[1] = 0;
			dl20_flt[2] = 0;
			dl20_flt[3] = 0;
			
			// Place only 4 shutdown fault in buffer
			Set_DL20_Fault();
			// faults
			dl20_trmbuf[10] = dl20_flt[0];		
			dl20_trmbuf[11] = dl20_flt[1];		
			dl20_trmbuf[12] = dl20_flt[2];		
			dl20_trmbuf[13] = dl20_flt[3];		

			// date and time
			dl20_trmbuf[14] = d.month;		
			dl20_trmbuf[15] = d.day;		
			dl20_trmbuf[16] = (uint8)((d.year % 100) & 0x00FF);		
			dl20_trmbuf[17] = t.hour;
			dl20_trmbuf[18] = t.minute;		

			// reserved
			dl20_trmbuf[19] = 0;		
			dl20_trmbuf[20] = 0;		
			dl20_trmbuf[21] = 0;		
			dl20_trmbuf[22] = 0;		
			dl20_trmbuf[23] = 0;		
			dl20_trmbuf[24] = 0;		
			
			dl20_pack_len = (uint8)25;			
			valid_packet = 1;
			break;

		default:
			break;				
	}

	if (valid_packet == 1)
	{
		chksum = 0;
		for (i=1;i<dl20_pack_len;i++)
		{
			chksum += dl20_trmbuf[i];
		}
		dl20_trmbuf[i] = (uint8)chksum;
		dl20_trm_ix = 0;
		enable_tx_DL20(comnmb);
		while (dl20_trm_ix <= dl20_pack_len)
		{
			sendc_DL20(comnmb,dl20_trmbuf[dl20_trm_ix]);
			dl20_trm_ix++;
		}
		dl20_com_tx_cnt++;
	}
}

void unpack_DL20_com (int pkt)
{
	switch(pkt)
	{
		case 1: 
			break;
		case 20:	//packet 20
			if (dl20_rcvbuf[2] == 1)
				elevator_disable = 1;		// not implemented
			else
				elevator_disable = 0;
			dl20_packet_rcv = 1;
			dl20_online_count = 10;
			dl20_com_rx_cnt++;
			dl20_packet_req = 0;
			break;
		case 21:	//packet 21
			if (dl20_rcvbuf[2] == 1)
			{
				if (phone_test_failed == 0)
					record_fault(f_phone_test);
				phone_test_failed = 1;		// phone test
			}
			else
				phone_test_failed = 0;
	
			dl20_packet_rcv = 1;
			dl20_online_count = 10;
			dl20_com_rx_cnt++;
			dl20_packet_req = 0;
			break;
		default:
			break;
	}
}


void receive_DL20_com (int16 comnmb)
{
		
  	int16 rbempt=0,endmess=0;
	unsigned char rcv_char = 0;
	static int16 packet;

	rbempt = isrxempty_DL20(comnmb);

  	while((rbempt == 0) && (endmess == 0))
  	{
	  	rcv_char = (uint8)asigetc_DL20(comnmb);
	  	rbempt = isrxempty_DL20(comnmb);
		switch (dl20_rcv_seq)
		{
			case 0:		// Start Byte
				packet = 0;
				if (rcv_char == 170)		
				{
					dl20_rcv_seq = 1;
					dl20_rx_ix = 1; 
					dl20_rcv_ix = 0;
					dl20_pkt_len = 3;
					dl20_rcv_chksum = 170;
					dl20_rcvbuf[dl20_rcv_ix++] = rcv_char;
				}
				else
				{
					dl20_rx_ix = 0;
				}

				break;
			case 1:
				packet = rcv_char;
				dl20_rcvbuf[dl20_rcv_ix++] = rcv_char;
				dl20_rcv_chksum += rcv_char;
				if (((packet == 20) && (dl20_rcv_chksum == 190)) || ((packet == 21) && (dl20_rcv_chksum == 191)))
				{	
					dl20_rcv_seq = 2;
					dl20_rx_ix++;	  
				}
				else
				{
					dl20_rcv_seq = 0;		// invalid packet
					dl20_rcv_ix = 0;
					dl20_rx_ix = 0;

				}
				break;
			case 2:
				dl20_rcvbuf[dl20_rcv_ix++] = rcv_char;
				dl20_rx_ix++;
				if (dl20_rx_ix < dl20_pkt_len)
				{
					dl20_rcv_chksum += rcv_char;
				}
				else
				{
					dl20_rcv_chksum += rcv_char;
					dl20_rcv_seq = 0;
					unpack_DL20_com(packet);
					timers[trx_DL20] = 0;
					dl20_rcv_ix = 0;
					dl20_rx_ix = 0;
					dl20_rcv_chksum = 0;
				}
				break;
			default:
				dl20_rcv_seq = 0;		// invalid packet
				dl20_rcv_ix = 0;
				dl20_rx_ix = 0;
				break;
		}
		if (dl20_rcv_ix > 10)
		{
			rbempt = 1;
			dl20_rcv_seq = 0;		// invalid packet
			dl20_rcv_ix = 0;
			dl20_rx_ix = 0;
		}
	}
	if (timers[trx_DL20] > 360)
	{ 
		dl20_packet_req = 0;
		if (dl20_online_count > 0)
		{
			dl20_com_online = 1;
			dl20_online_count--;
		}
		else 
		{
			dl20_com_online = 0;
			dl20_online_count = 0;
		}
		timers[trx_DL20] = 0;
	}
}

// Comm FOR DL-20 Display
void DL20_comm(int16 comnmb)
{
	unsigned char pkt_req = 0;


	if ((fvars[fvcomdiag1] == 3) && (fvars[fvcomdiag2] == 3))
		return;		// can only use dl20 on one port
	

//	if (istxcomplete_DL20(comnmb)  == 1)  // only if half duplex
	receive_DL20_com(comnmb);

	if (istxempty_DL20(comnmb) == 0)
		DL20_tx_timer = 0;
	else if (DL20_tx_timer > 10)
	{
		DL20_tx_timer = 0;
		dl20_packet_rcv = 0;
		if (dl20_packet_req == 0)
		{
			pkt_req = 1;
		}
		else
		{
			pkt_req = dl20_packet_req;
		}
		pack_DL20_com(pkt_req, comnmb);
		dl20_packet_req = 0;
	}
}


/*

Revision History

8/12/13 v6.0.50 mhd			1. Added dl20 interface and set it up to work on com1, com2 and com3.
12/16/14 v7.1.24 mhd		1. Modified G4 to match changes in the following G3 software modifications: 
								08/01/14 v6.1.62 hdl		1. In IGEP_Comm() added SE2txDis after the data has been sent to allow half duplex communication.Fix bus error with RPI board.
4/2/15 v7.1.29 mhd			1. In DL20 Protocol, byte 17, must mod the year with 100 else the DL20 rejects the packet.
8/14/17 v8.0.7 mhd			1. Change DL20 COMM to only use COM1 OR COM2 rs-232 ports.
*/