extern int16 NTS_eep_ix;
extern int16 NTS_eep_nmb;
extern int16 NTS_eep_count;
extern uint8 nts_spi_error;
extern uint8 nts_spi_online;
extern int16 nts_tx_count;
extern int16 nts_rx_count;


// NTS - Cedes Selector setup and status interface through SPI port

extern struct tag_NTS_spi {
   	union {						// Command
    	uint8 R;
        struct {
			uint8 RESET_FLT: 1;				// Bit 7: Reset Fault
			uint8 :1;						// Bit 6:
			uint8 CLR_HWTBL: 1;				// Bit 5: Clear hoistway table
            uint8 :1;						// Bit 4: 
            uint8 HW_LEARN:1;				// Bit 3: Enter HW Learn Mode
            uint8 TOP_FL:1;					// Bit 2: Top Floor
            uint8 BOT_FL:1;					// Bit 1: Bottom Floor
            uint8 MARK_FL:1;				// Bit 0: Mark Floor
      } B;
    } Command1;
   	union {						// Command
    	uint8 R;
        struct {
			uint8 SET_UPSD: 1;				// Bit 7: 1 = Set NTS Up Slowdown
			uint8 SET_DNSD: 1;				// Bit 6: 1 = Set NTS Down Slowdown	
			uint8 RST_CNT:1;				// Bit 5: 1 = Reset tx/rx cnt
            uint8 SIM:1;					// Bit 4: 1 = Controller simulator software
            uint8 RESET_JACK:1;				// Bit 3: 1 = Reset Jack operation 
            uint8 TL_TEST:1;				// Bit 2: 1 = Terminal Limit Test
            uint8 AD_OFF:1;					// Bit 1: 1 = Automatic Door disabled
            uint8 NTS_TEST:1;				// Bit 0: 1 = NTS Test
      } B;
    } Command2;
    union {
    	uint8 R;
        struct {
			uint8 Floor_Marked: 1;		// Bit 7: Floor Marked
			uint8 Fvar_setup_flt: 1;	// Bit 6: Field Variable Setup Fault
			uint8 :1;					// Bit 5: 
            uint8 sel_ok:1;				// Bit 4: 1=Selector OK,0=not setup or fault
            uint8 sel_online:1;			// Bit 3: 1 = selector online
            uint8 dz_clip_flt:1;		// Bit 2: DZ Clip fault
            uint8 ced_can_flt:1;		// Bit 1: Cedes comm fault
            uint8 Valid_Fl_Tbl:1;		// Bit 0: Valid Floor Table
      } B;
    } Status1;
    union {
    	uint8 R;
        struct {
			uint8 rear_door:1;			// Bit 7: Rear Door Jumper
			uint8 setup:1;				// Bit 6: Setup Jumper
			uint8 UNo:1;				// Bit 5: 1=Up Normal output on
            uint8 DNo:1;				// Bit 4: 1=Down Normal output on
            uint8 UTo:1;				// Bit 3: 1=Up Terminal output on
            uint8 DTo:1;				// Bit 2: 1=Down Terminal output on
            uint8 up_dir:1;				// Bit 1: 1=direction down
            uint8 dn_dir:1;				// Bit 0: 1=direction up
      } B;
    } Status2;
	uint8 doorzone;				// Bits for each door zone input
								// byte 0
									//  Bit0: UL
									//	Bit1: DZ
									//  Bit2: DZA
									//  Bit3: DL
									//  Bit4: dz clip
									//  Bit5: 
									//  Bit6: 
									//  Bit7: 
											
	uint8 limits;				// bit for each limit 
								// byte 0
									//  Bit0: UN
									//  Bit1: UT
									//  Bit2: 
									//  Bit3: 
									//  Bit4: DN
									//  Bit5: DT
									//  Bit6: 
									//  Bit7: 
	int16 velocity;					// selector velocity 
	uint8 vel_dir;					// velocity direction
	int16 ut_vel;					// speed when ut hit
	int16 dt_vel;					// speed when dt hit
	int32 pos_cnt;					// position count
	uint8 	hwl_floor;				// Hoistway Learn Floor
	uint8	hwl_num_floors;			// Hoistway Learn Number of Floors
	uint8	hwl_num_limits;			// Hoistway Learn Number of Limits
	uint16	hwl_speed;				// Hoistway Learn Speed	
	uint8 	command1_ret;				
	uint8 	command2_ret;				
	uint8 	hwl_floor_ret;			// Hoistway Learn Floor
	uint8	hwl_num_floors_ret;		// Hoistway Learn Number of Floors
	uint8	hwl_num_limits_ret;		// Hoistway Learn Number of Limits
	uint16	hwl_speed_ret;			// Hoistway Learn Speed	
	unsigned char ver;
	unsigned char rev;
	int16 eeprom[31];
	uint32 	valid_fl_tbl;			// Valid floor bits set to 1
	uint32 	valid_clip_tbl;			// Valid floor bits set to 1
	uint8 pkt_req;					// packet number of requested packedt
	uint8 eep_req;					// 1=request eeprom variables, 2=send variables
	uint8 valid_fl_req;				// 1=request valid floor table
	uint8 can_req;					// 1=request can bus data
	uint8 eep_rcvd;					// 1=eeprom variables received
	uint8 valid_fl_rcvd;			// 1=valid floor table returned
	uint8 can_rcvd;					// 1=received can bus data
} NTS_spi;							// Cedes selector data from NTS processor

extern struct SEL_can_type NTS_sel;


// Input structure for FPGA 1

extern union i1spi_in_tag
{
	uint16 buff[5];
	struct i1spi_st_tag
	{
	   	union {
	    	uint16 R;
			//			Bit# 7		Bit# 6		Bit# 5		Bit# 4		Bit# 3		Bit# 2		Bit# 1		Bit# 0
			//	Byte# 0	AUTO_L		LBP_L		GLB_L		GBP_L		BAU_L		BAD_L		TAU_L		TAD_L
			//	Byte# 1	DLB_L		DLM_L		DLT_L		RLM_L		LPS_L		RGS_L		GS_L		ACC_L
	        struct {
				uint16 AUTO:1;			// Bit 15: Auto
				uint16 LBP:1;			// Bit 14: Lock Bypass
				uint16 GLB:1;			// Bit 13: Gate/Lock Bypass
	            uint16 GBP:1;			// Bit 12: Gate Bypass
	            uint16 BAU:1;			// Bit 11: Bottom Access Up
	            uint16 BAD:1;			// Bit 10: Bottom Access Down
	            uint16 TAU:1;			// Bit 09: Top Access Up
	            uint16 TAD:1;			// Bit 08: Top Access Down
				uint16 DLB:1;			// Bit 07: Door Lock Bottom
				uint16 DLM:1;			// Bit 06: Door Lock Middle
				uint16 DLT:1;			// Bit 05: Door Lock Top
	            uint16 RLM:1;			// Bit 04: Rear Lock Middle
	            uint16 LPS:1;			// Bit 03: Low Pressure Switch
	            uint16 RGS:1;			// Bit 02: Rear Door Gate Switch
	            uint16 GS:1;			// Bit 01: Front Door Gate Switch
	            uint16 ACC:1;			// Bit 00: Access
	      } B;
	    } buff0;
	    union {
	    	uint16 R;
			//			Bit# 7		Bit# 6		Bit# 5		Bit# 4		Bit# 3		Bit# 2		Bit# 1		Bit# 0
			//	Byte# 2	ICI_L		CTIN_L		IU_L		ID_L		MRI_L		MRIU_L		MRID_L		"0" Always
			//	Byte# 3	CEN_CPU		MC_CPU		STE_CPU		DNR_CPU		UPR_CPU		UPF_CPU		DNF_CPU		UL_CPU
	       struct {
				uint16 ICI:1;			// Bit 15: In Car Inspection
				uint16 CTIN:1;			// Bit 14: Car Top Inspection
				uint16 IU:1;			// Bit 13: Car Top Inspection Up
	            uint16 ID:1;			// Bit 12: Car Top Inspection Down
	            uint16 MRI:1;			// Bit 11: Machine Room Inspection
	            uint16 MRIU:1;			// Bit 10: Machine Room Inspection Up
	            uint16 MRID:1;			// Bit 09: Machine Room Inspection Down
	            uint16 :1;				// Bit 08: Always 0
				uint16 CEN_CPU:1;		// Bit 07: CEN CPU
				uint16 MC_CPU:1;		// Bit 06: MC CPU
				uint16 STE_CPU:1;		// Bit 05: STE CPU
				uint16 DNR_CPU:1;		// Bit 04: SD CPU
	            uint16 UPR_CPU:1;		// Bit 03: SU CPU
	            uint16 UPF_CPU:1;		// Bit 02: SUF CPU
	            uint16 DNF_CPU:1;		// Bit 01: SDF CPU
	            uint16 UL_CPU:1;		// Bit 00: UL CPU
	      } B;
	    } buff1;
	    union {
	    	uint16 R;
			//			Bit# 7		Bit# 6		Bit# 5		Bit# 4		Bit# 3		Bit# 2		Bit# 1		Bit# 0
			//	Byte# 4	DZ_CPU		DL_CPU		DZ_PAL		SPD_150_CPU	SPD_75_CPU	SEL_OK_CPU	RUN_CPU		PSDF_LP
			//	Byte# 5	PSUF_LP		PSU_LP		PSD_LP		PDT_LP		PUT_LP		PUN_LP		PDN_LP		PSTE_LP
	        struct {
				uint16 DZ_CPU:1;		// Bit 15: DZ CPU
				uint16 DL_CPU:1;		// Bit 14: DL CPU
				uint16 DZ_PAL:1;		// Bit 13: DZ PAL
	            uint16 SPD_150_CPU:1;	// Bit 12: S150 CPU
	            uint16 SPD_75_CPU:1;	// Bit 11: S75 CPU
	            uint16 SEL_OK_CPU:1;	// Bit 10: SEL OK CPU
	            uint16 RUN_CPU:1;		// Bit 09: RUN CPU
	            uint16 PSDF:1;			// Bit 08: PAL Solenoid Down Fast
				uint16 PSUF:1;			// Bit 07: PAL Solenoid Up Fast
				uint16 PSU:1;			// Bit 06: PAL Solenoid Up
				uint16 PSD:1;			// Bit 05: PAL Solenoid Down 
	            uint16 PDT:1;			// Bit 04: PAL DT
	            uint16 PUT:1;			// Bit 03: PAL UT
	            uint16 PUN:1;			// Bit 02: PAL UN 
	            uint16 PDN:1;			// Bit 01: PAL DN
	            uint16 PSTE:1;			// Bit 00: PAL STE
	      } B;
	    } buff2;
	    union {
	    	uint16 R;
			//			Bit# 7		Bit# 6		Bit# 5		Bit# 4		Bit# 3		Bit# 2		Bit# 1		Bit# 0
			//	Byte# 6	PMC_LP		PCEN_LP		PRUN_LP		PAL_FLT		DT_NTS		UT_NTS		UN_NTS		DN_NTS
			//	Byte# 7	DZ_NTS		SEL_OK_NTS	SPD_150_NTS	SPD_75_NTS	NTS_FLT		REAR_DOOR	TEST_MODE	"0" Always
	        struct {
				uint16 PMC:1;				// Bit 15: PAL MC
				uint16 PCEN:1;				// Bit 14: PAL CEN
				uint16 PRUN:1;				// Bit 13: PAL RUN
	            uint16 PAL_FLT:1;			// Bit 12: PAL FAULT
	            uint16 DT_NTS:1;			// Bit 11: NTS DT
	            uint16 UT_NTS:1;			// Bit 10: NTS UT
	            uint16 UN_NTS:1;			// Bit 09: NTS UN
	            uint16 DN_NTS:1;			// Bit 08: NTS DN
				uint16 DZ_NTS:1;			// Bit 07: NTS DZ
				uint16 SEL_OK_NTS:1;		// Bit 06: NTS SEL OK
				uint16 SPD_150_NTS:1;		// Bit 05: NTS S150
	            uint16 SPD_75_NTS:1;		// Bit 04: NTS S75
	            uint16 NTS_FLT:1;			// Bit 03: NTS FLT
	            uint16 REAR_DOOR:1;			// Bit 02: Rear Door
	            uint16 TEST_MODE:1;			// Bit 01: Test Mode
	            uint16 :1;					// Bit 00: Always 0
	      } B;
	    } buff3;
	   	uint8 ver_rev;
	    uint8 xor_cksum;
	}ST;
} i1spi_in;

extern union iospi_in_tag
{	
	uint16 buff[5];
	struct iospi_st_tag
	{
	   	union {
	    	uint16 R;
			//   data_in_raw(0) <= DN_L & DT_L & UN_L & UT_L & SDI_L & SDFI_L & LPS_L & LOS_L;                  -- 0
	 		//   data_in_raw(1) <= TPL_L & TPH_L & S10_L & GOV_L & HSS_L & TSD_L & CENi_L & L120B_L;            -- 1
	        struct {
				uint16 DN:1;			// Bit 15: Down Normal Limit
				uint16 DT:1;			// Bit 14: Down Terminal Limit
				uint16 UN:1;			// Bit 13: Up Normal Limit
	            uint16 UT:1;			// Bit 12: Up Terminal limit
	            uint16 SD:1;			// Bit 11: Solenoid Down Input
	            uint16 SDF:1;			// Bit 10: Solenoid Down Fast Input
	            uint16 LPS:1;			// Bit 09: Low Pressure Switch
	            uint16 LOS:1;			// Bit 08: Low Oil Switch
				uint16 TPL:1;			// Bit 07: Temperature Low
				uint16 TPH:1;			// Bit 06: Temperature High
				uint16 S10:1;			// Bit 05: S10 Power Fuse
	            uint16 GOV:1;			// Bit 04: Governor switch
	            uint16 HSS:1;			// Bit 03: Hoistway Safety Switch
	            uint16 TSD:1;			// Bit 02: Terminal Speed Device
	            uint16 CEN:1;			// Bit 01: Controller Enable Input
	            uint16 L120B:1;			// Bit 00: L120B Power Fuse (redundant)
	      } B;
	    } buff0;
	    union {
	    	uint16 R;
			//    data_in_raw(2) <= L120_L & BAU_L & BAD_L & TAU_L & TAD_L & DLB_L & DLB1_L & DLM_L;              -- 2
	 		//    data_in_raw(3) <= DLM1_L & DLT_L & DLT1_L & RLM_L & RLM1_L & TEST_MODE & REAR_DOOR & ACC_L;     -- 3
	       struct {
				uint16 L120:1;			// Bit 15: L120 Power
				uint16 BAU:1;			// Bit 14: Bottom Access Up
				uint16 BAD:1;			// Bit 13: Bottom Access Down
	            uint16 TAU:1;			// Bit 12: Top Access Up
	            uint16 TAD:1;			// Bit 11: Top Access Down
	            uint16 DLB:1;			// Bit 10: Door Lock Bottom
	            uint16 DLB1:1;			// Bit 09: Door Lock Bottom - 1
	            uint16 DLM:1;			// Bit 08: Door Lock Middle
				uint16 DLM1:1;			// Bit 07: Door Lock Middle - 1
				uint16 DLT:1;			// Bit 06: Door Lock Top
				uint16 DLT1:1;			// Bit 05: Door Lock Top - 1
	            uint16 RLM:1;			// Bit 04: Rear Lock Middle
	            uint16 RLM1:1;			// Bit 03: Rear Lock Middle - 1
	            uint16 TEST_MODE:1;		// Bit 02: Test_Mode Jumper
	            uint16 REAR_DOOR:1;		// Bit 01: Rear Door Jumper
	            uint16 ACC:1;			// Bit 00: Access
	      } B;
	    } buff1;
	    union {
	    	uint16 R;
			//    data_in_raw(4) <= RESET_LV & PAL_FLT & NTS_FLT & GS_L & GS1_L & RGS_L & RGS1_L & ICI_L;         -- 4
			//    data_in_raw(5) <= IEN_L & RUNi_L & MRID_L & MRIE_L & MRIU_L & MRI_L & ID_L & IU_L;              -- 5
	        struct {
				uint16 RESET:1;			// Bit 15: Cpu Reset
				uint16 PAL_FLT:1;		// Bit 14: PAL Fault
				uint16 NTS_FLT:1;		// Bit 13: NTS Fault
	            uint16 GS:1;			// Bit 12: Gate Switch
	            uint16 GS1:1;			// Bit 11: Gate Switch 1
	            uint16 RGS:1;			// Bit 10: Rear Gate Switch
	            uint16 RGS1:1;			// Bit 09: Rear Gate Swsitch 1
	            uint16 ICI:1;			// Bit 08: In Car Inspection
				uint16 IEN:1;			// Bit 07: Car top Inspection Enable
				uint16 RUN:1;			// Bit 06: Run input
				uint16 MRID:1;			// Bit 05: Machine Room Inspection Down
	            uint16 MRIE:1;			// Bit 04: Machine Room Inspection Enable
	            uint16 MRIU:1;			// Bit 03: Machine Room Inspection Up
	            uint16 MRIN:1;			// Bit 02: Machine Room Inspection 
	            uint16 ID:1;			// Bit 01: Car Top Inspection Down
	            uint16 IU:1;			// Bit 00: Car Top Inspection Up
	      } B;
	    } buff2;
	    union {
	    	uint16 R;
			//    data_in_raw(6) <= CTIN_L & CST_L & CFLT_L & SPD_L & MTO_L & MCi_L & AUTO_L & AD_L;              -- 6
			//    data_in_raw(7) <= IND_L & LBP_L & GLB_L & GBP_L & MRSW_L & SUFI_L & SUI_L & DZ_L;                -- 7
	        struct {
				uint16 CTIN:1;			// Bit 15: Car Top Inspection
				uint16 CST:1;			// Bit 14: Car Stop Switch
				uint16 CFLT:1;			// Bit 13: C-contactor Fault
	            uint16 SPD:1;			// Bit 12: Up to Speed
	            uint16 MTO:1;			// Bit 11: Motor Thermal Overload
	            uint16 MC:1;			// Bit 10: Motor Contactor
	            uint16 AUTO:1;			// Bit 09: Automatic
	            uint16 AD:1;			// Bit 08: Auto Door
				uint16 IND:1;			// Bit 07: Independent
				uint16 LBP:1;			// Bit 06: Lock Bypass
				uint16 GLB:1;			// Bit 05: Gate Lock Bypass
	            uint16 GBP:1;			// Bit 04: Gate Bypass
	            uint16 MRSW:1;			// Bit 03: Machine Room Stop Switch
	            uint16 SUF:1;			// Bit 02: Solenoid Up Fast
	            uint16 SU:1;			// Bit 01: Solenoid Up
	            uint16 DZ:1;			// Bit 00: Door Zone
	      } B;
	    } buff3;
	   	uint8 ver_rev;
	    uint8 xor_cksum;
	}ST;
} iospi_in;

extern void init_iospi (void);
extern void iospi_control (void);
