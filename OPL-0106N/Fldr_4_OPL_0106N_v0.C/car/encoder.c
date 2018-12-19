/* 
	Encoder Driver software

*/

#define d_ENCODER 1
#include "global.h"


int16 prev_enc_cnt = 0;
int16 enc_count = 0;
int16 enc_init_cnt;
int8 enc_cnt_upd = 0;

int16 reset_enc_base_count = 0;

union {
		int8 b[4];			// byte: char 0 - 3
		int16 w[2];			// word: short int 0 - 1
		int32 dw;			// double word: int
	}enc_cnt;				// integer union
		
void init_encoder_count(int32 Count);
void encoder_overflow (void);
void preset_encoder_count(int32 Count);
void Init_Encoder (void);
void Read_Encoder(void);

void Init_Encoder (void)
{

	if (cons[sel_type] == 4)
	{
		sel_init_can_bus(0);			// Initialize can bus encoder	
	}
	else if (cons[sel_type] == 3)
	{
#if (Traction == 1)
		enc_init_can_bus(0);			// Initialize can bus encoder	
#endif
	}
	else
	{
		
		EMIOS.MCR.B.GPRE = 12;
		EMIOS.MCR.B.GPREN = 1;
		EMIOS.MCR.B.FRZ = 1;
		EMIOS.MCR.B.GTBE = 1;
		
		// Initialize encoder registers
		EMIOS.CH[23].CCR.R = 0;
		EMIOS.CH[23].CADR.R = 0x0000;

		EMIOS.CH[23].CCNTR.R = 0;
		EMIOS.CH[23].CCR.B.MODE = 0x0D;
		EMIOS.CH[23].CCR.B.BSL = 0;		
		EMIOS.CH[23].CCR.B.UCPRE = 0;
		EMIOS.CH[23].CCR.B.UCPREN = 1;
		EMIOS.CH[23].CCR.B.FCK = 0;
		EMIOS.CH[23].CCR.B.IF = 4;
		EMIOS.CH[23].CCR.B.EDSEL = 0;
		EMIOS.CH[23].CCR.B.FEN = 0;		// disable overflow interrupt
	//	if (fvars[fvencdir] != 0)
	//		EMIOS.CH[23].CCR.B.EDPOL = 1;
	//	else
			EMIOS.CH[23].CCR.B.EDPOL = 0;		// Chip only counts when EDPOL bit is zero.
		EMIOS.CH[23].CSR.B.OVR = 1;
		EMIOS.CH[23].CSR.B.OVFL = 1;
		EMIOS.CH[23].CSR.B.FLAG = 1;
	

		enc_cnt.dw = 0;
	}
}	   



// ***************************************************
// This Reads the Encoder and up dates the DPP_Count
// ***************************************************
void Read_Encoder(void)
{
#if ((Simulator == 0) || (Sim_Motion == 0))

	int32 count_diff;


	if (cons[sel_type] == 4)
	{
		if (sel_can.online == 1)
		{
			DPP_Count = Enc_Base_Count + sel_can.pos_cnt_A;

		  	if (prev_Enc_Count > DPP_Count)
				count_diff = prev_Enc_Count - DPP_Count;
		  	else
				count_diff = DPP_Count - prev_Enc_Count;

			if (count_diff > (Pulses_per_Inch * 4.0))		//  Can't move 4 in 2-3 msec
			{
				fault_prog_flag1 = enc_cnt_upd;
				record_fault(f_encoder);
			}
			prev_Enc_Count = DPP_Count;	
		}
	}
	else if (cons[sel_type] == 3)
	{
		if (enc_save_timer < 3)
			return;
#if (Traction == 1)
		if ((can_enc_init == 1) && (can_enc_com_err == 0) && (EncCan_Upd == 0) && (timers[tenc_busy] > 3))
		{
			DPP_Count = Enc_Base_Count + can_enc_count;

		  	if (prev_Enc_Count > DPP_Count)
				count_diff = prev_Enc_Count - DPP_Count;
		  	else
				count_diff = DPP_Count - prev_Enc_Count;

			if (count_diff > (Pulses_per_Inch * 4.0))		//  Can't move 4 in 2-3 msec
			{
				fault_prog_flag1 = enc_cnt_upd;
				record_fault(f_encoder);
			}
			prev_Enc_Count = DPP_Count;	
		}
#endif
	}
	else
	{
		_disable();
		enc_count = (int16)EMIOS.CH[23].CCNTR.R;
		// Detect zero crossing
		if ((enc_count >= 0) && (enc_count < (int16)0x1FFF) && (prev_enc_cnt < 0))
		{
			enc_cnt.w[hw]++;
		}
		else if ((enc_count < 0) && (enc_count > (int16)0xE000) && (prev_enc_cnt >= 0))
		{
			enc_cnt.w[hw]--;
		}
		
		enc_cnt.w[lw] = enc_count;
		prev_enc_cnt = enc_count;
		
		if (fvars[fvencdir] == 0)
			DPP_Count = Enc_Base_Count + enc_cnt.dw;
		else
			DPP_Count = Enc_Base_Count - enc_cnt.dw;
		
		_enable();

	  	if (prev_Enc_Count > DPP_Count)
			count_diff = prev_Enc_Count - DPP_Count;
	  	else
			count_diff = DPP_Count - prev_Enc_Count;

		if (count_diff > (Pulses_per_Inch * 4.0))		//  Can't move 4 in 2-3 msec
		{
			fault_prog_flag1 = enc_cnt_upd;	
			record_fault(f_encoder);
		}
		prev_Enc_Count = DPP_Count;	
	}
	

#endif

//	Update_DPP_EE();


	if (cons[sel_type] == 3)
	{
		if ((reset_enc_base_count == 0) && (abs(Enc_Base_Count) > (Hoistway_Init_Count/2)))
			reset_enc_base_count = 1;
	}
}

// ****************************
// Init the encoder pulse count 
// ****************************
void init_encoder_count(int32 Count)
{
	if (cons[sel_type] == 3)
	{
		if (enc_save_timer < 3)
			return;
	}

#if ((Simulator == 0) || (Sim_Motion == 0))
	if (cons[sel_type] == 3)
	{
#if (Traction == 1)
		EncCan_Upd = 1;
		can_enc_preset_count = Count;
		WriteEncCount = 1;
		Enc_Base_Count = 0;	
		enc_save_timer = 0;
		enc_init_cnt++;
#endif
	}
	else
	{
		_disable();
		enc_cnt.w[hw] = 0;
		enc_count = 0;
		prev_enc_cnt = 0;
		// Reset counter
		EMIOS.CH[23].CCR.B.MODE = 0x00;
		EMIOS.CH[23].CCNTR.R = 0;
		EMIOS.CH[23].CCR.B.MODE = 0x0D;
		Enc_Base_Count = Count;
		_enable();
	}
#else
	sim_DPP_Count = Count;
#endif
	prev_Enc_Count = Count;	
	Prev_DPP_Count = Count;
	Clear_Vel_Array = 1;
	enc_cnt_upd = 1;
   	Update_Enc_Base_Count();
}


// ****************************
// preset the encoder position 
// ****************************
void preset_encoder_count(int32 Count)
{

	if (cons[sel_type] == 3)
	{
		if (enc_save_timer < 3)
			return;
	}

#if ((Simulator == 0) || (Sim_Motion == 0))
	if (cons[sel_type] == 3)
	{
#if (Traction == 1)
      	Enc_Base_Count = Count - can_enc_count;
      	Update_Enc_Base_Count();
#endif
	}
	else
	{
		_disable();
		enc_cnt.dw = 0;
		enc_count = 0;
		prev_enc_cnt = 0;
		// Reset counter
		EMIOS.CH[23].CCR.B.MODE = 0x00;
		EMIOS.CH[23].CCNTR.R = 0;
		EMIOS.CH[23].CCR.B.MODE = 0x0D;
		Enc_Base_Count = Count;
		_enable();
	}
#else
	if ((Buffer_Test == 0) && (NTS_Test == 0) && (ETS_Test == 0))
		sim_DPP_Count = Count;
#endif
	
	prev_Enc_Count = Count;	
	Prev_DPP_Count = Count;
	Clear_Vel_Array = 1;
	enc_cnt_upd = 2;
}

/*
Change History:

8/14/12 v6.016 mhd 		1. Reversed encoder direction in Read_Encoder() for incremental encoder so that it matches existing jobs.
11/12/12 v6.0.24 mhd 	1. Check (timers[tenc_busy] > 3) to read encoder if sel_type is for can bus encoder..
6/9/13 v7.0.8 mhd		1. Prevent from reading encoder when enc_save_timer < 3 to prevent a velocity gripper fault.
						2. Call Update_Enc_Base_Count() when init_encoder_count() is called..
8/21/13 v7.0.16 mhd 	1. Check (timers[tenc_busy] > 3) to read encoder if sel_type is for can bus encoder..
						2. Prevent from reading encoder when enc_save_timer < 3 to prevent a velocity gripper fault.
						3. Call Update_Enc_Base_Count() when init_encoder_count() is called..
11/26/13 v7.0.30 mhd	1. Prevent calling preset_encoder_count() and init_encoder_count() from executing (do an immidiate return) if enc_sav_timer < 3.

*/
