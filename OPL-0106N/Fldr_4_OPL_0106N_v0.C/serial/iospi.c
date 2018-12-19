#define d_IOSPI 1
#include "global.h"
#include "GALX_1121_TEST.h"

extern int16 Log_an_event(char *msg);
//-extern void *GRtest_osignal_array[];
extern void *CTtest_osignal_array[];

// ZYM 12-7-2017
extern int16 NTS_SPI_TEST_3bit;	 		//ONLY using less than 3 bit


/* Main IO SPI port

Main Cpu to IO board SPI communications

	Data from Main CPU					Data from IO Board FPGA2 
	Data 1	Address high Byte (0x61)	Data 1  Dummy Byte
	Data 2  Address low Byte (0x2A)		Data 2  Dummy Byte
	Data 3  Output[0]					Data 3  Input[0]
	Data 4  Output[1]					Data 4  Input[1]
	Data 5  0x80 heartbeat				Data 5  Input[2]
	Data 6  XOR Checksum				Data 6  Input[3]
	Data 7  0xFF						Data 7  Input[4]
	Data 8  0xFF						Data 8  Input[5]
	Data 9  0xFF						Data 9  Input[6]
	Data 10 0xFF						Data 10 Input[7]
	Data 11 0xFF						Data 11  version/revision
	Data 12	0xFF						Data 12 XOR Checksum


	Data from Main CPU					Data from IO Board FPGA1 
	Data 1	Address high Byte (0x93)	Data 1  Dummy Byte
	Data 2  Address low Byte (0x55)		Data 2  Dummy Byte
	Data 3  MC Drop Delay High Byte		Data 3  Input[0]
	Data 4  MC Drop Delay Low Byte		Data 4  Input[1]
	Data 5  0x80 heartbeat				Data 5  Input[2]
	Data 6  XOR Checksum				Data 6  Input[3]
	Data 7  0xFF						Data 7  Input[4]
	Data 8  0xFF						Data 8  Input[5]
	Data 9  0xFF						Data 9  Input[6]
	Data 10 0xFF						Data 10 Input[7]
	Data 11 0xFF						Data 11  version/revision
	Data 12	0xFF						Data 12 XOR Checksum

	Transmission is always started by Main CPU acting as master
	All packets are the same length
	The data packets are defined below: 

			   data_in_raw(0) <= DN_L & DT_L & UN_L & UT_L & SDI_L & SDFI_L & LPS_L & LOS_L;                  -- 0
	 		   data_in_raw(1) <= TPL_L & TPH_L & S10_L & GOV_L & HSS_L & TSD_L & CENi_L & L120B_L;            -- 1
			   data_in_raw(2) <= L120_L & BAU_L & BAD_L & TAU_L & TAD_L & DLB_L & DLB1_L & DLM_L;             -- 2
	 		   data_in_raw(3) <= DLM1_L & DLT_L & DLT1_L & RLM_L & RLM1_L & TEST_MODE & REAR_DOOR & ACC_L;    -- 3
			   data_in_raw(4) <= RESET_LV & PAL_FLT & NTS_FLT & GS_L & GS1_L & RGS_L & RGS1_L & ICI_L;        -- 4
			   data_in_raw(5) <= IEN_L & RUNi_L & MRID_L & MRIE_L & MRIU_L & MRI_L & ID_L & IU_L;             -- 5
			   data_in_raw(6) <= CTIN_L & CST_L & CFLT_L & SPD_L & MTO_L & MCi_L & AUTO_L & AD_L;             -- 6
			   data_in_raw(7) <= IND_L & LBP_L & GLB_L & GBP_L & MRSW_L & SUFI_L & SUI_L & DZ_L;              -- 7

			   Byte 0 (RUN, SDF, SUF, SU, SD, LUL, DZ, LDL)
	 		   Byte 1 (   , SPD150,SPD75,SPDLEV,SELOK,STE,MC,CEN)

*/

void init_iospi (void);
void init_IOSPI_DMA(void);
void iospi_start(int16 len);
void iospi_dma_rx_int(void);
void iospi_control (void);
void iospi_pack (void);
void iospi_unpack (void);
void i1spi_pack (void);
void i1spi_unpack (void);
void put_iospi_data(void);
uint16 get_iospi_data (void);
void nts_hwl_control(void);
void nts_spi_pack (uint8 msg);
void nts_spi_unpack (void);
void zero_nts_spi_msg_req (void);
int16 put_nts_spi_msg_req (int8 msg);
int8 get_nts_spi_msg_req (void);

#define DSPI_D_Port 0xC3F94000L
#define IOSPIdr DSPI_D_Port + 0x0034
#define IOSPIsr DSPI_D_Port + 0x003A
#define IOSPIdmaRxIntLoc eDMA_18_Int

#define IOSPIdmaTX 0x98
#define IOSPIdmaRX 0x99

#define c_iospi_dma_buf_len 25
#define c_dma_xfers 5
uint32 iospi_tx_dma_buf[c_iospi_dma_buf_len];
uint16 iospi_rx_dma_buf[c_iospi_dma_buf_len];

uint8 spi_transferred = 0;
uint8 spi_xfer_seq = 0;
uint8 iospi_req = 0;
uint8 io_ctrl_cntr = 0;

uint8 SUF_output_cmnd_flag = 0;
uint8 SU_output_cmnd_flag  = 0;
uint8 SDF_output_cmnd_flag = 0;
uint8 SD_output_cmnd_flag  = 0;
uint8 STE_output_cmnd_flag = 0;
uint8 CEN_output_cmnd_flag = 0;
uint8 MC_output_cmnd_flag  = 0;
uint8 RUN_output_cmnd_flag  = 0;


#define NTS_proc_addr 1

int16 NTS_eep_ix = 0;
int16 NTS_eep_nmb = 0;
int16 NTS_eep_count = 0;

uint8 nts_spi_error = 0;
uint8 nts_spi_online = 0;
uint8 nts_spi_rcvd = 0;
uint8 nts_status_req = 0;
int16 nts_tx_count = 0;
int16 nts_rx_count = 0;



//*********************************************
// FPGA 2 Data Structure
//*********************************************

// I/O structure for FPGA 2

union iospi_in_tag
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
/*


    -- Set output statuses          -- BIT #
    CEN_CPU <= data_out(0);         --  0
    MC_CPU <= data_out(1);          --  1
    STE_CPU <= data_out(2);         --  2
    SEL_OK_CPU <= data_out(3);      --  3
    SPD_LEV_CPU <= data_out(4);     --  4
    SPD_75_CPU <= data_out(5);      --  5
    SPD_150_CPU <= data_out(6);     --  6
    DZ_CLIP_CPU <= data_out(7);     --  7
    DL_CPU <= data_out(8);      	--  8
	LDL <= NOT data_out(8);         --      LDL same as DL_CPU BIT8
	DZ_CPU <= data_out(9);          --  9
    LDZ <= NOT data_out(9);         --      LDZ same as DZ_CPU BIT9
    UL_CPU <= data_out(10);         --  10
    LUL <= NOT data_out(10);        --      LUL same as UL_CPU BIT10
	DNR_CPU <= data_out(11);        --  11
    UPR_CPU <= data_out(12);        --  12
    UPF_CPU <= data_out(13);        --  13
    DNF_CPU <= data_out(14);        --  14
    RUN_CPU <= data_out(15);        --  15
*/

union iospi_out_tag
{
	// byte 0 (RUN, SDF, SUF, SU, SD, LUL, DZ, LDL)
	// Byte 1 (   , SPD150,SPD75,SPDLEV,SELOK,STE,MC,CEN)
	uint8 buff[4];
	struct iospi_out_st_tag
	{
	   	union {
	    	uint16 R;
	        struct {
// ZYM 12-8-2017
				// To FPGA2 its LOW byte and MATCHing GALX1132 CPU HIGH byte	       
//-				uint8 :1;				// Bit 15: 
				uint8 CPLD2_SPI_TEST_bit:1; // Bit 15:  using for SPI commucation check between CPU and CPLD2	AS Bit 7 from CPLD2
				
				uint8 SPD150:1;			// Bit 14:  Speed <= 150 fpm 
				uint8 SPD75:1;			// Bit 13:  Speed <= 75 fpm
	            uint8 SPDLEV:1;			// Bit 12:  Speed <= Leveling speed			SAME AS "SPD_LEV_CPU" from CPLD2
	            uint8 SELOK:1;			// Bit 11:  Selector OK
	            uint8 STE:1;			// Bit 10:  Starter Enable
	            uint8 MC:1;				// Bit 09:  Motor Contactor
	            uint8 CEN:1;			// Bit 08:  Controller Enable											AS Bit 0 from CPLD2
	            
	            // To FPGA1 its HIGH byte and MATCHing GALX1132 CPU LOW byte
				uint8 RUN:1;			// Bit 07: Run Output													AS Bit 15 from CPLD2
				uint8 SDF:1;			// Bit 06: Solenoid Down Fast				SAME AS "DNF_CPU"from CPLD2 
				uint8 SUF:1;			// Bit 05: Solenoid Up Fast					SAME AS "UPF_CPU"from CPLD2 
	            uint8 SU:1;				// Bit 04: Solenoid Up						SAME AS "UPR_CPU"from CPLD2
	            uint8 SD:1;				// Bit 03: Solenoid Down					SAME AS "DNR_CPU"from CPLD2
	            uint8 LUL:1;			// Bit 02: Up Level LED
	            uint8 DZ_1:1;			// Bit 01:  Door Zone Cpu
	            uint8 LDL:1;			// Bit 00:  Down Level LED					SAME AS "DL_CPU"from CPLD2	AS Bit 8 from CPLD2
	      } B;
	    } buff0;
	   	uint8 heart_beat_addr;
	    uint8 xor_cksum;
	}ST;
} iospi_out;



//*********************************************
// FPGA 1 Data Structure
//*********************************************

// Input structure for FPGA 1

union i1spi_in_tag
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


// Output structure for FPGA 1

union i1spi_out_tag
{
	uint8 buff[4];
	struct i1spi_out_st_tag
	{
		uint16 mc_drop_delay;
	   	uint8 heart_beat_addr;
	    uint8 xor_cksum;
	}ST;
} i1spi_out;


//ZYM 12-12-2017
union FPGA1_HW_Modify_Output_tag
{
	uint8 buff[2];
	uint16 mc_drop_delay;
	
// ZYM 12-12-2017
//Wrong Mapping to FPGA1	
/* //-	   
	struct
	{
		// To FPGA1 it is high byte and MATCHing GALX1132 CPU high byte		 		
		uint16 PDT_LP   : 1; 		//15
		uint16 PUT_LP   : 1;        //14
		uint16 PDN_LP   : 1;        //13
		uint16 PUN_LP   : 1;		//12
		uint16 PSDF_LP  : 1;		//11 
		uint16 PSUF_LP  : 1;		//10
		uint16 PSD_LP   : 1;		//9 
		uint16 PSU_LP   : 1;		//8 
		
		// To FPGA1 it is low byte and MATCHing GALX1132 CPU low byte
		uint16 UNUSED   : 3;	
		
		//CPU sending 1 to FPGA1
        //FPGA2 return 6 back to CPU
		uint16 CPLD1_SPI_TEST_bit : 1; //4  using for SPI commucation check between CPU and CPLD1	
		uint16 PMC_LP   : 1;		//3
		uint16 PSTE_LP  : 1;		//2
		uint16 PCEN_LP  : 1;		//1		SAME AS MC1			
		uint16 PRUN_LP  : 1;		//0		
	}B0;
*/

//Correct Mapping to FPGA1
	struct
	{
		// To FPGA1 it is LOW byte and MATCHing GALX1132 CPU HIGH byte	
		uint16 UNUSED   : 2;	
		
		uint16 DZ_PAL   : 1; 			//13  		*DZ_PAL is OUT from FPGA1 TO FPGA2, 
 												   //But For Testing Code, 
												   //CPU NO NEED to READ back From FPGA1(DEFAULT Value '0')
												   //BUT CPU READ back from FPGA2    
		//CPU sending 1 to FPGA1
        //FPGA2 return 6 back to CPU 	
		uint16 CPLD1_SPI_TEST_bit : 1; 	//12  using for SPI commucation check between CPU and CPLD1			
 		uint16 PMC_LP   : 1;			//11 
 		uint16 PSTE_LP  : 1;			//10
 		uint16 PCEN_LP  : 1;			//9 	SAME AS MC1
 		uint16 PRUN_LP  : 1;			//8 
		
		// To FPGA1 it is HIGH byte and MATCHing GALX1132 CPU LOW byte	
		uint16 PDT_LP   : 1; 			//7
		uint16 PUT_LP   : 1;        	//6
		uint16 PDN_LP   : 1;        	//5							
 		uint16 PUN_LP   : 1;        	//4  using for SPI commucation check between CPU and CPLD1	
 		uint16 PSDF_LP  : 1;			//3
 		uint16 PSUF_LP  : 1;			//2
 		uint16 PSD_LP   : 1;			//1					
 		uint16 PSU_LP   : 1;			//0		
	}B0;		
} FPGA1_HW_Modify_Output;


// ZYM 12-5-2017
union NTS_HW_Modify_Output_tag
{
	uint8 byte;
	struct 
	{
		uint8 UNUSED : 4;
		uint8 DN_NTS_out : 1;	//3
		uint8 DT_NTS_out : 1;	//2
		uint8 UN_NTS_out : 1;	//1	
		uint8 UT_NTS_out : 1;	//0
	};
} NTS_HW_Modify_Output;


extern int16 return_CPLD1_SPI_TEST_byte;
extern int16 return_CPLD2_SPI_TEST_byte;

extern int32 test_msg;
extern int32 test_msg1;
extern int32 test_msg2;
extern int32 test_msg3;
extern int32 test_msg4;


//*********************************************
// NTS Data Structure
//*********************************************

// NTS - Cedes Selector setup and status interface through SPI port

struct tag_NTS_spi {
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

struct SEL_can_type NTS_sel;

#define nts_spi_que_len 10
int8 nts_spi_msg_req[nts_spi_que_len];


//*****************************************************
// Init IO SPI port for FPGA 1, 2 AND NTS Processor
//*****************************************************


void init_iospi (void)
{
	int16 i;
	
	_disable();
	
	DSPI_D.MCR.B.HALT = 1;		// Stop SPI from running
	DSPI_D.MCR.B.MSTR = 1;		// Master Mode
	DSPI_D.MCR.B.CONT_SCKE = 0;	// Continuous SCK disabled
	DSPI_D.MCR.B.DCONF = 0;		// SPI mode
	DSPI_D.MCR.B.FRZ = 0;		// Do not freeze SPI during debug
	DSPI_D.MCR.B.MTFE = 0;		// No modified timing format
	DSPI_D.MCR.B.PCSSE = 0;		// No Peripheral Chip Select Strobe 
	DSPI_D.MCR.B.ROOE = 0; 		// Ignore overflow incomming data
	DSPI_D.MCR.B.PCSIS0 = 1;	// Peripheral Chip select active low state
	DSPI_D.MCR.B.PCSIS1 = 1;	// Peripheral Chip select active low state
	DSPI_D.MCR.B.PCSIS2 = 1;	// Peripheral Chip select active low state
	DSPI_D.MCR.B.PCSIS3 = 1;	// Peripheral Chip select active low state
	DSPI_D.MCR.B.PCSIS4 = 1;	// Peripheral Chip select active low state
	DSPI_D.MCR.B.PCSIS5 = 1;	// Peripheral Chip select active low state
	DSPI_D.MCR.B.DIS_TXF = 0;	// Allow Transmit Fifo
	DSPI_D.MCR.B.DIS_RXF = 0;	// Allow Receive Fifo
	DSPI_D.MCR.B.CLR_TXF = 1;	// Clear transmit fifo
	DSPI_D.MCR.B.CLR_RXF = 1;	// Clear receive fifo
	DSPI_D.MCR.B.SMPL_PT = 0;	// Sample point 0 - Not using modified timing format
	
	DSPI_D.TCR.B.SPI_TCNT = 0;	// Reset SPI transfer Count
/*	
	for (i=0;i<=7;i++)
	{
		DSPI_D.CTAR[i].B.FMSZ = 15;		// Transfer 16 bits
		DSPI_D.CTAR[i].B.CPOL = 1;		// Clock polarity is low
		DSPI_D.CTAR[i].B.CPHA = 1;		// Clock phase is leading edge of SCK
		DSPI_D.CTAR[i].B.LSBFE = 0;		// MSB first 
		
		// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 1024 = 9.416 usec (use one clock width)
		DSPI_D.CTAR[i].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
		DSPI_D.CTAR[i].B.CSSCK = 9;		// PCS to SCK  set to  1024  
		
		// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 512 = 4.708 usec (use 1/2 clock width) 
		DSPI_D.CTAR[i].B.PASC = 0;		// Prescaler = 1				
		DSPI_D.CTAR[i].B.ASC = 8;		// After SCK delay set to 512 
		
		// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 2048 = 18.832 usec (use 2 clock widths)
		DSPI_D.CTAR[i].B.PDT = 0;		// Prescaler = 1
		DSPI_D.CTAR[i].B._DT = 10;			// CS Delay Time  
																				 
		// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 512 = 106.20117875 KHz
		DSPI_D.CTAR[i].B.DBR = 0;		// No double baud rate
		DSPI_D.CTAR[i].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
		DSPI_D.CTAR[i].B.BR = 9;		// Baud rate prescaler (divide by 512)
	}
*/	


	//*****************************************************	
	// SPI to I/O FPGA on Main I/O board
	
	DSPI_D.CTAR[0].B.FMSZ = 15;		// Transfer 16 bits
	DSPI_D.CTAR[0].B.CPOL = 1;		// Clock polarity is low
	DSPI_D.CTAR[0].B.CPHA = 1;		// Clock phase is leading edge of SCK
	DSPI_D.CTAR[0].B.LSBFE = 0;		// MSB first 
		
	// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 1024 = 9.416 usec (use one clock width)
	DSPI_D.CTAR[0].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
	DSPI_D.CTAR[0].B.CSSCK = 9;		// PCS to SCK  set to  1024  
		
	// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 512 = 4.708 usec (use 1/2 clock width) 
	DSPI_D.CTAR[0].B.PASC = 0;		// Prescaler = 1				
	DSPI_D.CTAR[0].B.ASC = 8;		// After SCK delay set to 512 
		
	// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 2048 = 18.832 usec (use 2 clock widths)
	DSPI_D.CTAR[0].B.PDT = 0;		// Prescaler = 1
	DSPI_D.CTAR[0].B._DT = 10;			// CS Delay Time  
																				 
	// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 512 = 106.20117875 KHz
	DSPI_D.CTAR[0].B.DBR = 0;		// No double baud rate
	DSPI_D.CTAR[0].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
	DSPI_D.CTAR[0].B.BR = 9;		// Baud rate prescaler (divide by 512)
	//*****************************************************	

	//*****************************************************	
	// SPI to NTS processor on Main I/O board
	
	DSPI_D.CTAR[1].B.FMSZ = 7;		// Transfer 16 bits
	DSPI_D.CTAR[1].B.CPOL = 1;		// Clock polarity is low
	DSPI_D.CTAR[1].B.CPHA = 1;		// Clock phase is leading edge of SCK
	DSPI_D.CTAR[1].B.LSBFE = 0;		// MSB first 
		
	// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 1024 = 9.416 usec (use one clock width)
	DSPI_D.CTAR[1].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
	DSPI_D.CTAR[1].B.CSSCK = 9;		// PCS to SCK  set to  1024  
		
	// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 512 = 4.708 usec (use 1/2 clock width) 
	DSPI_D.CTAR[1].B.PASC = 0;		// Prescaler = 1				
	DSPI_D.CTAR[1].B.ASC = 8;		// After SCK delay set to 512 
		
	// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 3 x 4096 = 112.99 usec (use 8 clock width)
	DSPI_D.CTAR[1].B.PDT = 1;		// Prescaler = 3
	DSPI_D.CTAR[1].B._DT = 11;		// CS Delay Time (4096) 
																				 
	// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 1024 = 53.1 KHz 
	DSPI_D.CTAR[1].B.DBR = 0;		// No double baud rate
	DSPI_D.CTAR[1].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
	DSPI_D.CTAR[1].B.BR = 9;		// Baud rate prescaler (divide by 1024)
	//*****************************************************	


	for (i=2;i<=7;i++)
	{
		DSPI_D.CTAR[i].B.FMSZ = 15;		// Transfer 16 bits
		DSPI_D.CTAR[i].B.CPOL = 1;		// Clock polarity is low
		DSPI_D.CTAR[i].B.CPHA = 1;		// Clock phase is leading edge of SCK
		DSPI_D.CTAR[i].B.LSBFE = 0;		// MSB first 
		
		// PCS to SCK Delay tCSC = 1/fSYS x PCSSCK x CSSCK = 1/108.75 MHz x 1 x 1024 = 9.416 usec (use one clock width)
		DSPI_D.CTAR[i].B.PCSSCK = 0;	// PCS to SCK Delay Prescaler = 1
		DSPI_D.CTAR[i].B.CSSCK = 9;		// PCS to SCK  set to  1024  
		
		// After SCK Delay tASC = 1/fSYS x PASC x ASC = 1/108.75 MHz x 1 x 512 = 4.708 usec (use 1/2 clock width) 
		DSPI_D.CTAR[i].B.PASC = 0;		// Prescaler = 1				
		DSPI_D.CTAR[i].B.ASC = 8;		// After SCK delay set to 512 
		
		// Delay after Transfer tDT = 1/fSYS x PDT x DT = 1/108.75 MHz x 1 x 2048 = 18.832 usec (use 2 clock widths)
		DSPI_D.CTAR[i].B.PDT = 0;		// Prescaler = 1
		DSPI_D.CTAR[i].B._DT = 10;			// CS Delay Time  
																				 
		// Baud rate setting SCK baud = fSYS/PBR x (1+DBR)/BR = 108.75 MHz / 2 / 512 = 106.20117875 KHz
		DSPI_D.CTAR[i].B.DBR = 0;		// No double baud rate
		DSPI_D.CTAR[i].B.PBR = 0;		// Baud rate clock prescale (divide by 2)
		DSPI_D.CTAR[i].B.BR = 9;		// Baud rate prescaler (divide by 512)
	}
	
	
	// Clear the status register bits
	DSPI_D.SR.B.TCF = 1;			// Clear transfer complete flag
	DSPI_D.SR.B.TFUF = 1;			// Clear transmit underflow flag
	DSPI_D.SR.B.TFFF = 1; 			// Clear transmit fifo fill flag
	DSPI_D.SR.B.RFOF = 1;			// Clear receive fifo overflow flag
	DSPI_D.SR.B.RFDF = 1;			// Clear receive fifo drain flag

	
	// Set up DMA request for RTC SPI
	DSPI_D.RSER.B.TCF_RE = 0;		// Disable transmission request interrupt
	DSPI_D.RSER.B.EOQF_RE = 0;		// Disable DSPI finished interrupt
	DSPI_D.RSER.B.TFUF_RE = 0;		// Disable Transmit fifo underflow interrupt
	DSPI_D.RSER.B.RFOF_RE = 0;		// Disable Receive Fifo Overflow interrupt

	DSPI_D.RSER.B.TFFF_RE = 0;		// Leave Disabled for now: Enable Transit Fifo Fill
	DSPI_D.RSER.B.RFDF_RE = 0;		// Leave Disabled for not: Enable Receive FIFO drain request

//	DSPI_D.RSER.B.TFFF_RE = 0;		// Enable Transit Fifo Fill
//	DSPI_D.RSER.B.TFFF_DIRS = 0; 	// Set to generate DMA request for transmit fifo
//	DSPI_D.RSER.B.RFOF_RE = 0;		// Disable Receive Fifo Overflow interrupt
//	DSPI_D.RSER.B.RFDF_RE = 0;		// Enable Receive FIFO drain request
//	DSPI_D.RSER.B.RFDF_DIRS = 0;	// Set to generate DMA request for receive fifo
	
	
	DSPI_D.MCR.B.HALT = 0;		// Allow SPI to run
	DSPI_D.MCR.B.MDIS = 0;		// No Module disable

	SIU.PCR[IOBD_CLK].R = 0x60C;		
	SIU.PCR[IOBD_MOSI].R = 0x60C;		
	SIU.PCR[IOBD_MISO].R = 0x500;
	SIU.PCR[IOBD_CS1n].R = 0x60C;		// Chip select
	SIU.PCR[IOBD_CS2n].R = 0x60C;		// Chip select

	init_IOSPI_DMA();
	
	// preload the command word
    for(i=0; i<c_iospi_dma_buf_len; i++)
    {
    	iospi_tx_dma_buf[i] = 0x00010000;
    }

	DSPI_D.RSER.B.TFFF_DIRS = 1; 	// Set to generate DMA request for transmit fifo
	DSPI_D.RSER.B.RFDF_DIRS = 1;	// Set to generate DMA request for receive fifo
	DSPI_D.RSER.B.TFFF_RE = 1;		// Enable Transit Fifo Fill
	DSPI_D.RSER.B.RFDF_RE = 1;		// Enable Receive FIFO drain request

	iospi_out.ST.heart_beat_addr = 0;
	iospi_in.ST.ver_rev = 0;
	i1spi_out.ST.heart_beat_addr = 0;
	i1spi_in.ST.ver_rev = 0;

	zero_nts_spi_msg_req();	
	
	_enable();
}


//*****************************************
// DMA Initialization for I/O SPI Port
//*****************************************


void init_IOSPI_DMA(void) 
{

	// Initialize Transmit Comm DMA
  
  	EDMA.CERQR.R = IOSPIdmaChTx;      	// Disable Display SPI transmit EDMA channel   


  	EDMA.CPR[IOSPIdmaChTx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[IOSPIdmaChTx].R = IOSPIdmaTX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[IOSPIdmaChTx].SADDR = (uint32) &iospi_tx_dma_buf[0];  	// Load address of source data 
  	EDMA.TCD[IOSPIdmaChTx].SSIZE = 2;                        		// Read 2**0 = 1 byte per transfer 
  	EDMA.TCD[IOSPIdmaChTx].SOFF = 4;                         		// After transfer, add 2 to src addr
  	
  	EDMA.TCD[IOSPIdmaChTx].SLAST = - (c_dma_xfers * 4);        		// After major loop, reset src addr 
  	EDMA.TCD[IOSPIdmaChTx].SMOD = 0;                         		// Source modulo feature not used 

  	EDMA.TCD[IOSPIdmaChTx].DADDR =  IOSPIdr; 						// Load address of destination register
  	EDMA.TCD[IOSPIdmaChTx].DSIZE = 2;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[IOSPIdmaChTx].DOFF = 0;                         		// Do not increment destination addr 
  	EDMA.TCD[IOSPIdmaChTx].DLAST_SGA = 0;                    		// After major loop, no dest addr change 
  	EDMA.TCD[IOSPIdmaChTx].DMOD = 0;                         		// Destination modulo feature not used 
  
  	EDMA.TCD[IOSPIdmaChTx].NBYTES= 4;                    			// Transfer 1 byte per minor loop 
  	EDMA.TCD[IOSPIdmaChTx].BITER = c_dma_xfers;                		// 1 major loop iterations 
  	EDMA.TCD[IOSPIdmaChTx].CITER = c_dma_xfers;               		// Initialize current iteraction count 
  	EDMA.TCD[IOSPIdmaChTx].D_REQ = 1;                        		// Disable channel when major loop is done
  	EDMA.TCD[IOSPIdmaChTx].INT_HALF = 0;                     		// Interrupts are not used 
  	EDMA.TCD[IOSPIdmaChTx].INT_MAJ = 0;
  	EDMA.TCD[IOSPIdmaChTx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[IOSPIdmaChTx].BITERE_LINK = 0;
  	EDMA.TCD[IOSPIdmaChTx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[IOSPIdmaChTx].MAJORLINKCH = 0;							// Major link channel 
  	EDMA.TCD[IOSPIdmaChTx].E_SG = 0; 
  	EDMA.TCD[IOSPIdmaChTx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[IOSPIdmaChTx].DONE = 0;
  	EDMA.TCD[IOSPIdmaChTx].ACTIVE = 0;
  	EDMA.TCD[IOSPIdmaChTx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = IOSPIdmaChTx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = IOSPIdmaChTx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = IOSPIdmaChTx;           	// DMA Clear error Register 
  
  	
  	// Initialize Receive Comm DMA

 	EDMA.CERQR.R = IOSPIdmaChRx;      		// Disable group EDMA channel   
  
	INTC_InstallINTCInterruptHandler(iospi_dma_rx_int, IOSPIdmaRxIntLoc, 1);  

  	EDMA.CPR[IOSPIdmaChRx].R = 0x00;  		// Channel 1 priorites: group priority = 0, channel priority = 0  

	DMAMUX.CHCONFIG[IOSPIdmaChRx].R = IOSPIdmaRX;					// Set Mux to drive port (0x82 for ESCI_A TX)

    EDMA.TCD[IOSPIdmaChRx].SADDR = IOSPIsr;  						// Load address of source register data 
  	EDMA.TCD[IOSPIdmaChRx].SSIZE = 1;                        		// Read 2**0 = 2 byte per transfer 
  	EDMA.TCD[IOSPIdmaChRx].SOFF = 0;                         		// Do not increment source address
  	
  	EDMA.TCD[IOSPIdmaChRx].SLAST = 0;                      			// After major loop, reset src addr 
  	EDMA.TCD[IOSPIdmaChRx].SMOD = 0;                         		// Source modulo feature not used

  	EDMA.TCD[IOSPIdmaChRx].DADDR =  (uint32) &iospi_rx_dma_buf[0]; 				// Load address of destination 
  	EDMA.TCD[IOSPIdmaChRx].DSIZE = 1;                        		// Write 2**0 = 1 byte per transfer 
  	EDMA.TCD[IOSPIdmaChRx].DOFF = 2;                         		// Increment destination addr by 2 
  	EDMA.TCD[IOSPIdmaChRx].DLAST_SGA = - (c_dma_xfers * 2);                    		// After major loop reset destination address
  	EDMA.TCD[IOSPIdmaChRx].DMOD = 0;                         		// Destination modulo feature : Circular buffer 512 bytes (2^9) 
  
  	EDMA.TCD[IOSPIdmaChRx].NBYTES= 2;                    			// Transfer 1 byte per major loop 
  	EDMA.TCD[IOSPIdmaChRx].BITER = c_dma_xfers;        			    // Major loop iterations 
  	EDMA.TCD[IOSPIdmaChRx].CITER = c_dma_xfers;            			// Initialize current iteraction count 
  	EDMA.TCD[IOSPIdmaChRx].D_REQ = 0;                        		// Do not Disable channel when major loop is done
  	EDMA.TCD[IOSPIdmaChRx].INT_HALF = 0;                     		
  	EDMA.TCD[IOSPIdmaChRx].INT_MAJ = 1;								// Interrupt after major loop
  	EDMA.TCD[IOSPIdmaChRx].CITERE_LINK = 0;                  		// Linking is not used 									  
  	EDMA.TCD[IOSPIdmaChRx].BITERE_LINK = 0;
  	EDMA.TCD[IOSPIdmaChRx].MAJORE_LINK = 0;                  		// Dynamic program is not used
  	EDMA.TCD[IOSPIdmaChRx].MAJORLINKCH = 0;							// Major link channel 
  	EDMA.TCD[IOSPIdmaChRx].E_SG = 0; 
  	EDMA.TCD[IOSPIdmaChRx].BWC = 0;                          		// Default bandwidth control- no stalls 
  	EDMA.TCD[IOSPIdmaChRx].DONE = 0;
  	EDMA.TCD[IOSPIdmaChRx].ACTIVE = 0;
  	EDMA.TCD[IOSPIdmaChRx].START = 0;                        		// Initialize status flags 
  
  	EDMA.CEEIR.R = IOSPIdmaChRx;     		// DMA Clear Enable Error Interrupt Register  
  	EDMA.CIRQR.R = IOSPIdmaChRx;         	// DMA Clear Interrupt Request Register 
  	EDMA.CER.R = IOSPIdmaChRx;           	// DMA Clear error Register 
  
}

//**************************************************
// This is start the comm transmission on a COM Port
//**************************************************

void iospi_start(int16 len)
{
	spi_transferred = 0;
	
	_disable();	   
	// Turn off the SPI transmit and receive fifo
	DSPI_D.RSER.B.TFFF_RE = 0;		// Disable Transit Fifo Fill
	DSPI_D.RSER.B.RFDF_RE = 0;		// Dosab;e Receive FIFO drain request
	DSPI_D.RSER.B.TFFF_DIRS = 0; 	// Clear generate DMA request for transmit fifo
	DSPI_D.RSER.B.RFDF_DIRS = 0;	// Clear generate DMA request for receive fifo
	
	// Enable transmitter
	EDMA.CER.R = IOSPIdmaChTx;           	// DMA Clear error Register 
	EDMA.CER.R = IOSPIdmaChRx;           	// DMA Clear error Register 

    EDMA.TCD[IOSPIdmaChTx].SADDR = (uint32) &iospi_tx_dma_buf[0];  		// Load address of source data 
  	EDMA.TCD[IOSPIdmaChTx].SLAST = - (len * 4);        					// After major loop, reset src addr 
  	EDMA.TCD[IOSPIdmaChTx].BITER = len;          	   		         	// Major loop iterations 
  	EDMA.TCD[IOSPIdmaChTx].CITER = len;             		         	// Initialize current iteraction count 

  	EDMA.TCD[IOSPIdmaChRx].DADDR =  (uint32) &iospi_rx_dma_buf[0]; 		// Load address of destination 
  	EDMA.TCD[IOSPIdmaChRx].DLAST_SGA = - (len * 2);                  	// After major loop reset destination address
  	EDMA.TCD[IOSPIdmaChRx].BITER = len;         		  				// Major loop iterations 
  	EDMA.TCD[IOSPIdmaChRx].CITER = len;           		            	// Initialize current iteraction count 

  	EDMA.SERQR.R = IOSPIdmaChTx;      	// Enable Display SPI EDMA channel 
  	EDMA.SERQR.R = IOSPIdmaChRx;      	// Enable Display SPI EDMA channel 
  	  
	DSPI_D.RSER.B.TFFF_DIRS = 1; 	// Set to generate DMA request for transmit fifo
	DSPI_D.RSER.B.RFDF_DIRS = 1;	// Set to generate DMA request for receive fifo
	DSPI_D.RSER.B.TFFF_RE = 1;		// Enable Transit Fifo Fill
	DSPI_D.RSER.B.RFDF_RE = 1;		// Enable Receive FIFO drain request
	_enable();
}

//*********************************************
// I/O SPI DMA transmit finished interrupt
//*********************************************

void iospi_dma_rx_int(void)
{
	
  	EDMA.CIRQR.R = IOSPIdmaChRx;    // DMA Clear Interrupt Request Register 
  	EDMA.CERQR.R = IOSPIdmaChRx;    // Disable Display SPI EDMA channel   
	DSPI_D.RSER.B.TFFF_RE = 0;		// Enable Transit Fifo Fill
	DSPI_D.RSER.B.RFDF_RE = 0;		// Enable Receive FIFO drain request
	DSPI_D.RSER.B.TFFF_DIRS = 0; 	// Clear generate DMA request for transmit fifo
	DSPI_D.RSER.B.RFDF_DIRS = 0;	// Clear generate DMA request for receive fifo
	
	spi_transferred = 1;

}

void put_iospi_data(void)
{
	uint32 dontcare;
	uint16 count = 0;

	DSPI_D.SR.B.TCF = 1; 		// clear bit
	DSPI_D.PUSHR.R = 0x0001AA55; // Send data to rtc
  	while ((DSPI_D.SR.B.TCF != 1) && (count < 100))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	dontcare = DSPI_D.POPR.R;    		// Read data received by slave SPI 
  	DSPI_D.SR.R = 0x80020000;        	// Clear TCF, RDRF flags by writing 1 to them 
}

uint16 get_iospi_data (void)
{
	uint32 RecDataSlave;
	uint16 count = 0;

	DSPI_D.SR.B.TCF = 1; 				// clear bit
	DSPI_D.PUSHR.R = 0x0001AA55; 		// Send data to rtc
  	while ((DSPI_D.SR.B.TCF != 1) && (count < 100))
  	{		// Wait for Transmit Complete Flag = 1
  		count++;
  	}   
  	RecDataSlave = DSPI_D.POPR.R;    	// Read data received by slave SPI 
  	DSPI_D.SR.R = 0x80020000;        	// Clear TCF, RDRF flags by writing 1 to them 
  	return ((uint16)RecDataSlave);
}


//*********************************************
// Control for SPI I/O and NTS data
//*********************************************


void iospi_control (void)
{
// ZYM 12-7-2017
//-	uint8 nts_packet;

// ZYM 
//with "if (iospi_timer > 0)" collapse the FPGA1 2 NTS SPI communication, 
//because comparing back to the original production code, the 10ms loop got 
//controlled in the mian by "while (Timer_tic == 0)", so not fast enough to read 
//"void iospi_control (void)" routine.
//12-8-2017 take out "while (Timer_tic == 0)" in main, then "if (iospi_timer > 0)" works fine. 	
//also "(iospi_timer > 1)" which not enough time to finish the whole routine, 
//so changed to be "(iospi_timer > 2)" that works well
 	if (iospi_timer > 0)		// ten msec timer
 	{		
		if (((spi_transferred == 1) && (spi_xfer_seq == 0)) || (iospi_timer > 2))
		{ //-setoutp(CTtest_osignal_array[0]);
			// Transmit to display again
			iospi_timer = 0;
			spi_xfer_seq = 1;
			
//			if (iospi_req != 0)
//				iospi_packet = iospi_req;
//			else
//				iospi_packet = 3;
//			iospi_req = 0;
			iospi_pack();
		}
 	}
	else if ((spi_transferred == 1) && (spi_xfer_seq == 1))
	{ //-setoutp(CTtest_osignal_array[1]);
#if (Simulator == 0)
		iospi_unpack();
#endif
		if (io_ctrl_cntr > 4)
		{
// ZYM 12-7-2017		
//-			nts_hwl_control();
			io_ctrl_cntr = 0;		
			spi_xfer_seq = 2;
// ZYM 12-7-2017			
//-			nts_packet = get_nts_spi_msg_req();
//-			if (nts_packet == 0)
//-				nts_packet = 1;
			
#if (Simulator == 1)
			NTS_spi.Command2.B.SIM = 1;
#else
			NTS_spi.Command2.B.SIM = 0;
#endif
// ZYM 12-7-2017			
//-			nts_spi_pack(nts_packet);
			nts_spi_pack(1);
		}
		else
		{
			io_ctrl_cntr++;
			spi_xfer_seq = 0;
		}
	}
	else if ((spi_transferred == 1) && (spi_xfer_seq == 2))
	{ //-setoutp(CTtest_osignal_array[2]);
		spi_xfer_seq = 3;
// ZYM 12-7-2017		
//-		if (NTS_spi.eep_req == 2)
//-		{
//-			NTS_spi.eep_req = 0;
//-			NTS_eep_count = 0;					// zero count
//-			put_nts_spi_msg_req(3);
//-		}
		nts_spi_unpack();
		if (nts_spi_rcvd == 1)
		{
			nts_spi_rcvd = 0;
			nts_spi_error = 0;
			nts_spi_online = 1;
			timers[tntsupd] = 0;
		}
		else if (timers[tntsupd] > 3)
		{
#if (Simulator == 0)
			nts_spi_error = 1;
			nts_spi_online = 0;
#endif
		}
		i1spi_pack();

	}
	else if ((spi_transferred == 1) && (spi_xfer_seq == 3))
	{ //-setoutp(CTtest_osignal_array[3]);
		i1spi_unpack();
		spi_xfer_seq = 0;
	}
	
		
/* //-
//	uint8 iospi_packet;
	uint8 nts_packet;
	
	if (iospi_timer > 0)		// ten msec timer
	{
		
		if (((spi_transferred == 1) && (spi_xfer_seq == 0)) || (iospi_timer > 2))
		{
			// Transmit to display again
			iospi_timer = 0;
			spi_xfer_seq = 1;
			
//			if (iospi_req != 0)
//				iospi_packet = iospi_req;
//			else
//				iospi_packet = 3;
//			iospi_req = 0;
			iospi_pack();
		}
	}
	else if ((spi_transferred == 1) && (spi_xfer_seq == 1))
	{
#if (Simulator == 0)
		iospi_unpack();
#endif
		if (io_ctrl_cntr > 4)
		{
			nts_hwl_control();
			io_ctrl_cntr = 0;		
			spi_xfer_seq = 2;
			nts_packet = get_nts_spi_msg_req();
			if (nts_packet == 0)
				nts_packet = 1;
			
#if (Simulator == 1)
			NTS_spi.Command2.B.SIM = 1;
#else
			NTS_spi.Command2.B.SIM = 0;
#endif
			
			nts_spi_pack(nts_packet);
		}
		else
		{
			io_ctrl_cntr++;
			spi_xfer_seq = 0;
		}
	}
	else if ((spi_transferred == 1) && (spi_xfer_seq == 2))
	{
		spi_xfer_seq = 3;
		if (NTS_spi.eep_req == 2)
		{
			NTS_spi.eep_req = 0;
			NTS_eep_count = 0;					// zero count
			put_nts_spi_msg_req(3);
		}
		nts_spi_unpack();
		if (nts_spi_rcvd == 1)
		{
			nts_spi_rcvd = 0;
			nts_spi_error = 0;
			nts_spi_online = 1;
			timers[tntsupd] = 0;
		}
		else if (timers[tntsupd] > 3)
		{
#if (Simulator == 0)
			nts_spi_error = 1;
			nts_spi_online = 0;
#endif
		}
		i1spi_pack();

	}
	else if ((spi_transferred == 1) && (spi_xfer_seq == 3))
	{
		i1spi_unpack();
		spi_xfer_seq = 0;
	}
*/	
}


//*********************************************
// Pack SPI I/O Data to FPGA 2
//*********************************************

void iospi_pack (void)
{
	uint8 valid_msg = 0;
	
	static union {
			uint8 b[4];			// byte: char 0 - 3
			uint16 w[2];		// word: short int
			uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
	
		
//- setgoutp(GRtest_osignal_array[2]);  // 2 for pack and 3 for unpack

/* WBG 2017-11-13
 	if (rdoutp(o_RUN) != 0)
		iospi_out.ST.buff0.B.RUN = 1;
	else
		iospi_out.ST.buff0.B.RUN = 0;	
	if (rdoutp(o_SDF) != 0)
		iospi_out.ST.buff0.B.SDF = 1;
	else
		iospi_out.ST.buff0.B.SDF = 0;
	
	if (rdoutp(o_SUF) != 0)
		iospi_out.ST.buff0.B.SUF = 1;
	else
		iospi_out.ST.buff0.B.SUF = 0;
	
	if (rdoutp(o_SU) != 0)
		iospi_out.ST.buff0.B.SU = 1;
	else
		iospi_out.ST.buff0.B.SU = 0;
	
	if (rdoutp(o_SD) != 0)
		iospi_out.ST.buff0.B.SD = 1;
	else
		iospi_out.ST.buff0.B.SD = 0;
*/



// ZYM 12-8-2017	
/* //-
	if (rdoutp(o_STE) != 0)
		iospi_out.ST.buff0.B.STE = 1;
	else
		iospi_out.ST.buff0.B.STE = 0;
	
	if (rdoutp(o_MCC) != 0)
		iospi_out.ST.buff0.B.MC = 1;
	else
		iospi_out.ST.buff0.B.MC = 0;
	
  	if (rdoutp(o_CEN) != 0)					//same as MC1
		iospi_out.ST.buff0.B.CEN = 1;
	else
		iospi_out.ST.buff0.B.CEN = 0;
 

// ZYM 12-8-2017
//-	iospi_out.ST.buff0.B.CPLD2_SPI_TEST_bit=1;



	//                 B.UT ???
    iospi_out.ST.buff0.B.SUF = SUF_output_cmnd_flag;  //  OUTPUT MODULE 1
    
 //                      UN  -- from NTS
    iospi_out.ST.buff0.B.SU  = SU_output_cmnd_flag; //   OUTPUT MODULE 2
    
//                       DT  -- from NTS
    iospi_out.ST.buff0.B.SDF = SDF_output_cmnd_flag; //   OUTPUT MODULE 3
      
//                       DN  -- from NTS    
    iospi_out.ST.buff0.B.SD  = SD_output_cmnd_flag; //   OUTPUT MODULE 4 
    
    iospi_out.ST.buff0.B.STE = STE_output_cmnd_flag; //   OUTPUT MODULE 5
    iospi_out.ST.buff0.B.CEN = CEN_output_cmnd_flag;	//same as MC1
    
    iospi_out.ST.buff0.B.MC = MC_output_cmnd_flag;  //   OUTPUT MODULE 6
    iospi_out.ST.buff0.B.RUN = RUN_output_cmnd_flag;
//                    
*/   


       


//ZYM 1-19-2018
/*
	if (rdoutp(o_UL) != 0)
		iospi_out.ST.buff0.B.LUL = 1;
	else
		iospi_out.ST.buff0.B.LUL = 0;
	
	if (rdoutp(o_DZ) != 0)
		iospi_out.ST.buff0.B.DZ_1 = 1;
	else
		iospi_out.ST.buff0.B.DZ_1 = 0;
	
 	if (rdoutp(o_DL) != 0)
		iospi_out.ST.buff0.B.LDL = 1;
	else
		iospi_out.ST.buff0.B.LDL = 0;
	
	
	if (rdoutp(o_SP150) != 0)
		iospi_out.ST.buff0.B.SPD150= 1;
	else
		iospi_out.ST.buff0.B.SPD150 = 0;
	
	if (rdoutp(o_SP75) != 0)
		iospi_out.ST.buff0.B.SPD75 = 1;
	else
		iospi_out.ST.buff0.B.SPD75 = 0;
	
//	if (rdoutp(o_HWLRN) != 0)
//		iospi_out.ST.buff0.B.SPDLEV = 1;
//	else
//		iospi_out.ST.buff0.B.SPDLEV = 0;


	
  	if (rdoutp(o_SELOK) != 0)
		iospi_out.ST.buff0.B.SELOK = 1;
	else
		iospi_out.ST.buff0.B.SELOK = 0;
*/	



	// Send Elevator Status Information
	
	iospi_out.ST.xor_cksum = 0;				// initialize checksum
	
	uiu.b[mlb] = cpld2_B_mlb;  // 0x61;		// high byte address
	iospi_out.ST.xor_cksum ^= uiu.b[mlb];
	uiu.b[lb] = cpld2_B_lb;  // 0x2A;		// low byte address
	iospi_out.ST.xor_cksum ^= uiu.b[lb];
	uiu.w[hw] = 0;
	iospi_tx_dma_buf[0] = uiu.dw;				// load word
	
	uiu.b[mlb] = iospi_out.buff[1];				// first output data byte
	iospi_out.ST.xor_cksum ^= uiu.b[mlb];
	uiu.b[lb] = iospi_out.buff[0];				// second output data byte
	iospi_out.ST.xor_cksum ^= uiu.b[lb];
	uiu.w[hw] = 0;
	iospi_tx_dma_buf[1] = uiu.dw;				// load word

	iospi_out.ST.heart_beat_addr ^= 0x80;		// change the heartbeat
	uiu.b[mlb] = iospi_out.ST.heart_beat_addr;	//  load heartbeat
	iospi_out.ST.xor_cksum ^= uiu.b[mlb];
	uiu.b[lb] = iospi_out.ST.xor_cksum;			// load checksum	
	uiu.w[hw] = 0;
	iospi_tx_dma_buf[2] = uiu.dw;				// load word

	iospi_tx_dma_buf[0] |= 0x80010000;
	iospi_tx_dma_buf[1] |= 0x80010000;
	iospi_tx_dma_buf[2] |= 0x80010000;
	iospi_tx_dma_buf[3] = 0x8001FFFF;
	iospi_tx_dma_buf[4] = 0x8001FFFF;
	iospi_tx_dma_buf[5] = 0x0001FFFF;
	iospi_start(6);
}

//*********************************************
// Unpack SPI I/O Data from FPGA 2
//*********************************************
//    data_in_raw(0) <= DN_L & DT_L & UN_L & UT_L & SDI_L & SDFI_L & LPS_L & LOS_L;                   -- 0
//    data_in_raw(1) <= TPL_L & TPH_L & S10_L & GOV_L & HSS_L & TSD_L & CENi_L & L120B_L;             -- 1
//    data_in_raw(2) <= L120_L & BAU_L & BAD_L & TAU_L & TAD_L & DLB_L & DLB1_L & DLM_L;              -- 2
//    data_in_raw(3) <= DLM1_L & DLT_L & DLT1_L & RLM_L & RLM1_L & TEST_MODE & REAR_DOOR & ACC_L;     -- 3
//    data_in_raw(4) <= RESET_LV & PAL_FLT & NTS_FLT & GS_L & GS1_L & RGS_L & RGS1_L & ICI_L;         -- 4
//    data_in_raw(5) <= IEN_L & RUNi_L & MRID_L & MRIE_L & MRIU_L & MRI_L & ID_L & IU_L;              -- 5
//    data_in_raw(6) <= CTIN_L & CST_L & CFLT_L & SPD_L & MTO_L & MCi_L & AUTO_L & AD_L;              -- 6
//    data_in_raw(7) <= IND_L & LBP_L & GLB_L & GBP_L & MRSW_L & SUFI_L & SUI_L & DZ_L;               -- 7


void iospi_unpack (void)
{
	int16 i,k;
	uint8 xor_chksum;
	uint8 correct_spi_port = 0;
		
	static union {
			uint8 b[2];			// byte: char 0 - 1
			uint16 w;			// word: short int
	}uwu;					// unsigned word union

	
	
	if ((iospi_tx_dma_buf[0] & 0x7FFF0000) == 0x00010000)
		correct_spi_port = 1;
	
	if ((spi_transferred == 1) && (correct_spi_port == 1))
	{
		k = 1;
		for (i=0;i<5;i++)
		{
			iospi_in.buff[i] = iospi_rx_dma_buf[k++];
		}

		xor_chksum = 0;
		for (i=0;i<4;i++)
		{
			uwu.w = iospi_in.buff[i];
			xor_chksum ^= uwu.b[lbw];
			xor_chksum ^= uwu.b[hbw];
		}
		xor_chksum ^= iospi_in.ST.ver_rev;
		ver_rev = iospi_in.ST.ver_rev;
		
		
		if (iospi_in.ST.xor_cksum == xor_chksum)
		{
		    ioUnpkOK = 1;
		    
// ZYM 12-19-2017		    
//-			sprintf(src,"iospi_unpack");
//-         Log_an_event(src);  
//-		    setgoutp(GRtest_osignal_array[3]);  // 2 for pack and 3 for unpack
//-			sprintf(src,"Valid:ver=%d", iospi_in.ST.ver_rev);
//-         Log_an_event(src);

//-			for (i=0;i<5;i++)
//- 		{
//- 			iospi_in.buff[i] = iospi_rx_dma_buf[k++];
//- 		    sprintf(src,"iospi_in[%d]=0x%4.4X", i, iospi_in.buff[i]);
//-             Log_an_event(src);
//- 		}    
			
			
			
// ZYM 12-11-2017		
			return_CPLD2_SPI_TEST_byte=iospi_in.ST.ver_rev;
 			test_msg4=iospi_in.buff[0];
			test_msg=iospi_in.buff[1];
			test_msg1=iospi_in.buff[2];
			test_msg2=iospi_in.buff[3];
			test_msg3=iospi_in.buff[4];
		
		
			        
			//    data_in_raw(0) <= DN_L & DT_L & UN_L & UT_L & SDI_L & SDFI_L & LPS_L & LOS_L;                   -- 0
	 		if (iospi_in.ST.buff0.B.DN == 1)
				setinp(i_DN);
			else
				clrinp(i_DN);
			
			if (iospi_in.ST.buff0.B.DT == 1)
				setinp(i_DT);
			else
				clrinp(i_DT);
			
			if (iospi_in.ST.buff0.B.UN == 1)
				setinp(i_UN);
			else
				clrinp(i_UN);
			
			if (iospi_in.ST.buff0.B.UT == 1)
				setinp(i_UT);
			else
				clrinp(i_UT);
			
			if (iospi_in.ST.buff0.B.SD == 1)
				setinp(i_SD);
			else
				clrinp(i_SD);
			
			if (iospi_in.ST.buff0.B.SDF == 1)
				setinp(i_SDF);
			else
				clrinp(i_SDF);
			
			if (iospi_in.ST.buff0.B.LPS == 1)
				setinp(i_LPS);
			else
				clrinp(i_LPS);
			
			if (iospi_in.ST.buff0.B.LOS == 1)
				setinp(i_LOS);
			else
				clrinp(i_LOS);
			
			//    data_in_raw(1) <= TPL_L & TPH_L & S10_L & GOV_L & HSS_L & TSD_L & CENi_L & L120B_L;             -- 1
	 		if (iospi_in.ST.buff0.B.TPL == 1)
				setinp(i_TPL);
			else
				clrinp(i_TPL);
			
			if (iospi_in.ST.buff0.B.TPH == 1)
				setinp(i_TPH);
			else
				clrinp(i_TPH);
			
			if (iospi_in.ST.buff0.B.S10 == 1)
				setinp(i_S10);
			else
				clrinp(i_S10);
			
			if (iospi_in.ST.buff0.B.GOV == 1)
				setinp(i_GOV);
			else
				clrinp(i_GOV);
			
			if (iospi_in.ST.buff0.B.HSS == 1)
				setinp(i_HSS);
			else
				clrinp(i_HSS);
			
			if (iospi_in.ST.buff0.B.TSD == 1)
				setinp(i_TSD);
			else
				clrinp(i_TSD);
			
			if (iospi_in.ST.buff0.B.CEN == 1)
				setinp(i_CEN);
			else
				clrinp(i_CEN);
			
			if (iospi_in.ST.buff0.B.L120B == 1)
				setinp(i_L120B);
			else
				clrinp(i_L120B);
			
			//    data_in_raw(2) <= L120_L & BAU_L & BAD_L & TAU_L & TAD_L & DLB_L & DLB1_L & DLM_L;              -- 2
	 		if (iospi_in.ST.buff1.B.L120 == 1)
				setinp(i_L120);
			else
				clrinp(i_L120);
			
			if (iospi_in.ST.buff1.B.BAU == 1)
				setinp(i_BAU);
			else
				clrinp(i_BAU);
			
			if (iospi_in.ST.buff1.B.BAD == 1)
				setinp(i_BAD);
			else
				clrinp(i_BAD);
			
			if (iospi_in.ST.buff1.B.TAU == 1)
				setinp(i_TAU);
			else
				clrinp(i_TAU);
			
			if (iospi_in.ST.buff1.B.TAD == 1)
				setinp(i_TAD);
			else
				clrinp(i_TAD);
			
			if (iospi_in.ST.buff1.B.DLB == 1)
				setinp(i_DLB);
			else
				clrinp(i_DLB);
			
			if (iospi_in.ST.buff1.B.DLB1 == 1)
				setinp(i_DLB_1);
			else
				clrinp(i_DLB_1);
			
			if (iospi_in.ST.buff1.B.DLM == 1)
				setinp(i_DLM);
			else
				clrinp(i_DLM);
			
	 		//    data_in_raw(3) <= DLM1_L & DLT_L & DLT1_L & RLM_L & RLM1_L & TEST_MODE & REAR_DOOR & ACC_L;     -- 3
	 		if (iospi_in.ST.buff1.B.DLM1 == 1)
				setinp(i_DLM_1);
			else
				clrinp(i_DLM_1);
			
			if (iospi_in.ST.buff1.B.DLT == 1)
				setinp(i_DLT);
			else
				clrinp(i_DLT);
			
			if (iospi_in.ST.buff1.B.DLT1 == 1)
				setinp(i_DLT_1);
			else
				clrinp(i_DLT_1);
			
			if (iospi_in.ST.buff1.B.RLM == 1)
				setinp(i_RLM);
			else
				clrinp(i_RLM);
			
			if (iospi_in.ST.buff1.B.RLM1 == 1)
				setinp(i_RLM_1);
			else
				clrinp(i_RLM_1);
			
			if (iospi_in.ST.buff1.B.TEST_MODE == 0)
				setinp(i_TSTM);
			else
				clrinp(i_TSTM);
			
			if (iospi_in.ST.buff1.B.REAR_DOOR == 1)
				setinp(i_RDOOR);
			else
				clrinp(i_RDOOR);
			
			if (iospi_in.ST.buff1.B.ACC == 1)
				setinp(i_ACC);
			else
				clrinp(i_ACC);
			
			//    data_in_raw(4) <= RESET_LV & PAL_FLT & NTS_FLT & GS_L & GS1_L & RGS_L & RGS1_L & ICI_L;         -- 4
	//		if (iospi_in.ST.buff4.B.RESET == 1)
	//			setinp(i_RESET);
	//		else
	//			clrinp(i_RESET);
			
			if (iospi_in.ST.buff2.B.PAL_FLT == 0)
				setinp(i_PALF);
			else
				clrinp(i_PALF);
			
			if (iospi_in.ST.buff2.B.NTS_FLT == 0)
				setinp(i_NTSF);
			else
				clrinp(i_NTSF);
			
			if (iospi_in.ST.buff2.B.GS == 1)
				setinp(i_GS);
			else
				clrinp(i_GS);
			
			if (iospi_in.ST.buff2.B.GS1 == 1)
				setinp(i_GS_1);
			else
				clrinp(i_GS_1);
			
			if (iospi_in.ST.buff2.B.RGS == 1)
				setinp(i_RGS);
			else
				clrinp(i_RGS);
			
			if (iospi_in.ST.buff2.B.RGS1 == 1)
				setinp(i_RGS_1);
			else
				clrinp(i_RGS_1);
			
			if (iospi_in.ST.buff2.B.ICI == 1)
				setinp(i_ICI);
			else
				clrinp(i_ICI);
			
			//    data_in_raw(5) <= IEN_L & RUNi_L & MRID_L & MRIE_L & MRIU_L & MRI_L & ID_L & IU_L;              -- 5
	 		if (iospi_in.ST.buff2.B.IEN == 1)
				setinp(i_INE);
			else
				clrinp(i_INE);
			
			if (iospi_in.ST.buff2.B.RUN == 1)
				setinp(i_RUN);
			else
				clrinp(i_RUN);
			
			if (iospi_in.ST.buff2.B.MRID == 1)
				setinp(i_MRID);
			else
				clrinp(i_MRID);
			
			if (iospi_in.ST.buff2.B.MRIE == 1)
				setinp(i_MRIE);
			else
				clrinp(i_MRIE);
			
			if (iospi_in.ST.buff2.B.MRIU == 1)
				setinp(i_MRIU);
			else
				clrinp(i_MRIU);
			
			if (iospi_in.ST.buff2.B.MRIN == 1)
				setinp(i_MRIN);
			else
				clrinp(i_MRIN);
			
			if (iospi_in.ST.buff2.B.ID == 1)
				setinp(i_ID);
			else
				clrinp(i_ID);
			
			if (iospi_in.ST.buff2.B.IU == 1)
				setinp(i_IU);
			else
				clrinp(i_IU);
			
			//    data_in_raw(6) <= CTIN_L & CST_L & CFLT_L & SPD_L & MTO_L & MCi_L & AUTO_L & AD_L;              -- 6
	 		if (iospi_in.ST.buff3.B.CTIN == 1)
				setinp(i_CTIN);
			else
				clrinp(i_CTIN);
			
			if (iospi_in.ST.buff3.B.CST == 1)
				setinp(i_CST);
			else
				clrinp(i_CST);
			
			if (iospi_in.ST.buff3.B.CFLT == 1)
				setinp(i_CFLT);
			else
				clrinp(i_CFLT);
			
			if (iospi_in.ST.buff3.B.SPD == 1)
				setinp(i_SPD);
			else
				clrinp(i_SPD);
			
//			if (iospi_in.ST.buff3.B.MTO == 1)
//				setinp(i_MTO);
//			else
//				clrinp(i_MTO);
			
			if (iospi_in.ST.buff3.B.MC == 1)
				setinp(i_MCC);
			else
				clrinp(i_MCC);
			
			if (iospi_in.ST.buff3.B.AUTO == 1)
				setinp(i_AUTO);
			else
				clrinp(i_AUTO);
			
			if (iospi_in.ST.buff3.B.AD == 1)
				setinp(i_AD);
			else
				clrinp(i_AD);
			
			//    data_in_raw(7) <= IND_L & LBP_L & GLB_L & GBP_L & MRSW_L & SUFI_L & SUI_L & DZ_L;               -- 7
	 		if (iospi_in.ST.buff3.B.IND == 1)
				setinp(i_IND);
			else
				clrinp(i_IND);
			
			if (iospi_in.ST.buff3.B.LBP == 1)
				setinp(i_LBP);
			else
				clrinp(i_LBP);
			
			if (iospi_in.ST.buff3.B.GLB == 1)
				setinp(i_ICA);
			else
				clrinp(i_ICA);
			
			if (iospi_in.ST.buff3.B.GBP == 1)
				setinp(i_GBP);
			else
				clrinp(i_GBP);
			
			if (iospi_in.ST.buff3.B.MRSW == 1)
				setinp(i_MRSW);
			else
				clrinp(i_MRSW);
			
			if (iospi_in.ST.buff3.B.SUF == 1)
				setinp(i_SUF);
			else
				clrinp(i_SUF);
			
			if (iospi_in.ST.buff3.B.SU == 1)
				setinp(i_SU);
			else
				clrinp(i_SU);
			
			if (iospi_in.ST.buff3.B.DZ == 1)
				setinp(i_DZ);
			else
				clrinp(i_DZ);
		}
		
	}
}


//*********************************************
// Pack SPI I/O Data to FPGA 1
//*********************************************

void i1spi_pack (void)
{
	
	static union {
			uint8 b[4];			// byte: char 0 - 3
			uint16 w[2];		// word: short int
			uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		
	
//-    setgoutp(GRtest_osignal_array[0]);  // 0 for pack and 1 for unpack
	// Send Elevator Status Information
//	SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
	i1spi_out.ST.xor_cksum = 0;					// initialize checksum
	
	uiu.b[mlb] = cpld1_B_mlb;  //  0x93;							// high byte address
	i1spi_out.ST.xor_cksum ^= uiu.b[mlb];
	uiu.b[lb] = cpld1_B_lb;  // 0x55;							// low byte address
	i1spi_out.ST.xor_cksum ^= uiu.b[lb];
	uiu.w[hw] = 0;
	iospi_tx_dma_buf[0] = uiu.dw;				// load word

//ZYM 11-27-2017	
//-	FPGA1_HW_Modify_Output.B0.CPLD1_SPI_TEST_bit=1;
	i1spi_out.ST.mc_drop_delay=FPGA1_HW_Modify_Output.mc_drop_delay;	
//-	i1spi_out.ST.mc_drop_delay = (uint16)(fvars[fvsst] * 100);

	uiu.b[mlb] = i1spi_out.buff[0];				// High byte MC Drop timer
	i1spi_out.ST.xor_cksum ^= uiu.b[mlb];
	uiu.b[lb] = i1spi_out.buff[1];				// Low byte MC Drop timer
	i1spi_out.ST.xor_cksum ^= uiu.b[lb];
	uiu.w[hw] = 0;
	iospi_tx_dma_buf[1] = uiu.dw;				// load word

	i1spi_out.ST.heart_beat_addr ^= 0x80;		// change the heartbeat
	uiu.b[mlb] = i1spi_out.ST.heart_beat_addr;	//  load heartbeat
	i1spi_out.ST.xor_cksum ^= uiu.b[mlb];
	uiu.b[lb] = i1spi_out.ST.xor_cksum;			// load checksum	
	uiu.w[hw] = 0;
	iospi_tx_dma_buf[2] = uiu.dw;			// load word


	iospi_tx_dma_buf[0] |= 0x80010000;
	iospi_tx_dma_buf[1] |= 0x80010000;     
	iospi_tx_dma_buf[2] |= 0x80010000;
	iospi_tx_dma_buf[3] = 0x8001FFFF;
	iospi_tx_dma_buf[4] = 0x8001FFFF;
	iospi_tx_dma_buf[5] = 0x0001FFFF;
	iospi_start(6);
}

//*********************************************
// Unpack SPI I/O Data from FPGA 1
//*********************************************
/*
		Bit# 7		Bit# 6		Bit# 5		Bit# 4		Bit# 3		Bit# 2		Bit# 1		Bit# 0
Byte# 0	AUTO_L		LBP_L		GLB_L		GBP_L		BAU_L		BAD_L		TAU_L		TAD_L
Byte# 1	DLB_L		DLM_L		DLT_L		RLM_L		LPS_L		RGS_L		GS_L		ACC_L
Byte# 2	ICI_L		CTIN_L		IU_L		ID_L		MRI_L		MRIU_L		MRID_L		"0" Always
Byte# 3	CEN_CPU		MC_CPU		STE_CPU		DNR_CPU		UPR_CPU		UPF_CPU		DNF_CPU		UL_CPU
Byte# 4	DZ_CPU		DL_CPU		DZ_PAL		SPD_150_CPU	SPD_75_CPU	SEL_OK_CPU	RUN_CPU		PSDF_LP
Byte# 5	PSUF_LP		PSU_LP		PSD_LP		PDT_LP		PUT_LP		PUN_LP		PDN_LP		PSTE_LP
Byte# 6	PMC_LP		PCEN_LP		PRUN_LP		PAL_FLT		DT_NTS		UT_NTS		UN_NTS		DN_NTS
Byte# 7	DZ_NTS		SEL_OK_NTS	SPD_150NTS	SPD_75_NTS	NTS_FLT		REAR_DOOR	TEST_MODE	"0" Always



*/
void i1spi_unpack (void)
{
	int16 i,k;
	uint8 xor_chksum;
	uint8 rcvd_xor_chksum;
	uint8 correct_spi_port = 0;
		
	static union {
			uint8 b[2];			// byte: char 0 - 1
			uint16 w;			// word: short int
	}uwu;					// unsigned word union
	
	if ((iospi_tx_dma_buf[0] & 0x7FFF0000) == 0x00010000)
		correct_spi_port = 1;
	
	if ((spi_transferred == 1) && (correct_spi_port == 1))
	{
		xor_chksum = 0;
		for (i=1;i<5;i++)
		{
			uwu.w = iospi_rx_dma_buf[i];
			xor_chksum ^= uwu.b[lbw];
			xor_chksum ^= uwu.b[hbw];
		}
		
		uwu.w = iospi_rx_dma_buf[5];
		xor_chksum ^= uwu.b[hbw];			// checksum version
		rcvd_xor_chksum = uwu.b[lbw];		// get received checksum
		
		if (rcvd_xor_chksum == xor_chksum)
		{			
			i1UnpkOK = 1;
			ver_rev = i1spi_in.ST.ver_rev;
// ZYM 12-11-2017		
			return_CPLD1_SPI_TEST_byte=i1spi_in.ST.ver_rev;	
				
//			SIU.GPDO[Z6_LED].R = 0;	// turn led ON (wbg)// tagwbg 15
//-         setgoutp(GRtest_osignal_array[1]);  // 0 for pack and 1 for unpack
//-			sprintf(src,"i1spi_unpacked");
//-         Log_an_event(src);            
//-			sprintf(src,"Valid:ver=%d", i1spi_in.ST.ver_rev);
//-         Log_an_event(src);

			// valid checksum so copy into buffer
			k = 1;
			for (i=0;i<5;i++)
			{
				i1spi_in.buff[i] = iospi_rx_dma_buf[k++];
//-			    sprintf(src,"i1spi_in[%d]=0x%4.4X", i, i1spi_in.buff[i]);
//-             Log_an_event(src);
			}						
		}		
	}
}

/*

  	Main Cpu to NTS Cpu SPI communications

  buff[]	Data from Main CPU				Data from NTS Processor 
	0		Packet Start '@'				Dummy byte '0'
	1		Packet # (ascii)				Packet Start '@'
	2		Data 1							Packet # (ascii)
	3		Data 2							Data 1
	4		Data 3							Data 2
	5		Data 4							Data 3
	6		Data 5							Data 4
	7		Data 6							Data 5
	8		Data 7							Data 6	
	9		Data 8							Data 7
	10		Data 9							Data 8
	11		Data 10							Data 9
	12		Data 11							Data 10
	13		Data 12							Data 11
	14		Data 13							Data 12
	15		Data 14							Data 13
	16		Data 15							Data 14
	17		Data 16							Data 15
	18		Checksum						Data 16
	19		Packet End ':'					Checksum
	20		Dummy byte '0'					Packet End ':'

	Transmission is always started by Main CPU acting as master
	All packets are the same length
	The data packets are defined as follows: 



*****************************************************
						Main CPU	
*****************************************************

Packet #1
					
 				Send Command from Main CPU	
	Byte 1:				Command 1 
						Bit 0: Mark Floor 
						Bit 1: Bottom Floor 
						Bit 2: Top Floor Ack
						Bit 3: Enter HW Learn Mode 
						Bit 4: 
						Bit 5: Clear Hoistway Table							
						Bit 6:							
						Bit 7: Reset Fault	
						
	Byte 2:				Command 2 						
			            Bit 0: 1 = NTS Test
			            Bit 1: 1 = Automatic Door disabled
			            Bit 2: 1 = Terminal Limit Test
			            Bit 3: 
			            Bit 4: 
						Bit 5: 
						Bit 6: 1 = Set NTS Down Slowdown	
						Bit 7: 1 = Set NTS Up Slowdown
						
	Byte 3:				Hoistway Learn Floor					
	Byte 4:				Hoistway Learn Number of Floors	
			

				Send Status from Main CPU			
	Byte 5:				Status 1:
						Bit 0: Hoistway Learn
						Bit 1: CED Can Fault
						Bit 2: Selector OK
						Bit 3: Rear Door Jumper
						Bit 4: Setup Jumper						
						Bit 5: 							
						Bit 6: 1=direction down							
						Bit 7: 1=direction up							

	Byte 6:				Status 2:
						Bit 7: UN
						Bit 6: DN
						Bit 5: DZ
			            Bit 4: DZA
			            Bit 3: 
			            Bit 2: 
			            Bit 1: 
			            Bit 0: 

	Byte 7:				version
	Byte 8:				revision

 	Byte 9:				Command 1 Verify Byte			
 	Byte 10:			Command 2 Verify Byte			
	Byte 11: 			Request Packet #
	Byte 12: 			unused
	Byte 13:			unused
	Byte 14:			unused
	Byte 15:			unused
	Byte 16:			unused


Packet #3		Write Eeprom Packet
	Byte 1:				Start Parameter Address (1-32)							
	Byte 2:				Number of Parameters (1-4)			
	Byte 3:				Parameter start Low Byte	
	Byte 4:				Parameter start High Byte
	Byte 5:				Parameter start + 1 Low Byte
	Byte 6:				Parameter start + 1 High Byte
	Byte 7:				Parameter start + 2 Low Byte
 	Byte 8:				Parameter start + 2 High Byte			
	Byte 9: 			Parameter start + 3 Low Byte
	Byte 10: 			Parameter start + 3 High Byte
	Byte 11: 			Parameter start + 4 Low Byte
	Byte 12: 			Parameter start + 4 High Byte
	Byte 13: 			Parameter start + 5 Low Byte
	Byte 14: 			Parameter start + 5 High Byte
	Byte 15: 			Parameter start + 6 Low Byte
	Byte 16: 			Parameter start + 6 High Byte


*****************************************************
						NTS	Processor 
*****************************************************

Packet # 2		Read Command Return Data to NTS Processor 
	Byte 1:				Command 1 Return
						Bit 0: Mark Floor Ack
						Bit 1: Bottom Floor Ack
						Bit 2: Top Floor Ack
						Bit 3: Enter HW Learn Mode Ack
						Bit 4: 
						Bit 5: Clear Hoistway Table							
						Bit 6:							
						Bit 7: Reset Fault	
						
	Byte 2:				Command 2 return						
			            Bit 0: 1 = NTS Test
			            Bit 1: 1 = Automatic Door disabled
			            Bit 2: 1 = Terminal Limit Test
			            Bit 3: 
			            Bit 4: 
						Bit 5: 1 = Reset tx/rx count
						Bit 6: 1 = Set NTS Down Slowdown	
						Bit 7: 1 = Set NTS Up Slowdown
						
	Byte 3:				Return Hoistway Learn Floor

	Byte 4:				Status 1:
						Bit 0: Hoistway Learn
						Bit 1: CED Can Fault
						Bit 2: DZ clip fault
						Bit 3: 
						Bit 4: Selector OK						
						Bit 5: 							
						Bit 6: Setup Fault							
						Bit 7: Floor updated							

	Byte 5:				Status 2:
						Bit 7: Rear Door Jumper
						Bit 6: Setup Jumper
						Bit 5: 
			            Bit 4: 1=velocity fault
			            Bit 3: 1=limit (emsd) fault
			            Bit 2: 1=direction fault
			            Bit 1: 1=direction down
			            Bit 0: 1=direction up

	Byte 6:				Door Zone
						Bit0: UL
						Bit1: DZ
						Bit2: DZA
						Bit3: DL
						Bit4: dz clip
						Bit5: 
						Bit6: 
						Bit7: 

	Byte 7:				Limits
						Bit0: UN
						Bit1: UT
						Bit2: 
						Bit3: 
						Bit4: DN
						Bit5: DT
						Bit6: 
						Bit7:
						
	Byte 8:				Version
	Byte 9:				Revision 

	Byte 10:			NTS Velocity low byte
	Byte 11:			NTS Velocity high byte
	Byte 12: 			NTS Position Count low byte
	Byte 13:			NTS Position Count middle byte
	Byte 14: 			NTS Position Count high byte
	Byte 15:			Fault Byte							
	Byte 16:			Number of Occurances			
		
Packet # 4		Send Read Eeprom Packet
	Byte 1:				Start Parameter Address (1-32)							
	Byte 2:				Number of Parameters (1-4)			
	Byte 3:				Parameter start Low Byte	
	Byte 4:				Parameter start High Byte
	Byte 5:				Parameter start + 1 Low Byte
	Byte 6:				Parameter start + 1 High Byte
	Byte 7:				Parameter start + 2 Low Byte
 	Byte 8:				Parameter start + 2 High Byte			
	Byte 9: 			Parameter start + 3 Low Byte
	Byte 10: 			Parameter start + 3 High Byte
	Byte 11: 			Parameter start + 4 Low Byte
	Byte 12: 			Parameter start + 4 High Byte
	Byte 13: 			Parameter start + 5 Low Byte
	Byte 14: 			Parameter start + 5 High Byte
	Byte 15: 			Parameter start + 6 Low Byte
	Byte 16: 			Parameter start + 6 High Byte
	
Packet # 6		Send Valid Table Setup Data
	Byte 1:				Valid Floor Table low byte
	Byte 2:				Valid Floor Table high byte
	Byte 3:				unused							
	Byte 4:				unused			
	Byte 5:				unused	
	Byte 6:				unused
	Byte 7:				unused
	Byte 8:				unused
	Byte 9: 			Valid Clip Table low byte
	Byte 10: 			Valid Clip Table high byte
	Byte 11: 			unused
	Byte 12: 			unused
	Byte 13: 			unused
	Byte 14: 			unused
	Byte 15: 			unused
	Byte 16: 			unused
	
Packet #8		Send Selector Status
	Byte 1: 			Position B Low byte  Actual position in [0.5mm] steps
	Byte 2: 			Position B Middle byte
	Byte 3: 			Position B High byte
	Byte 4: 			Velocity B Low byte  Actual velocity in [mm/s]
	Byte 5: 			Velocity B High byte
	Byte 6: 			Error/Warn 
					Error
						Bit 0 Internal Error** Fatal
						Bit 1 Communication Error Fatal
						Bit 2 Position Validation Error according to SIL3** Fatal 
							  (more than one pos. extrapolated or crosscheck not successful)
						Bit 3 Velocity Validation Error according to SIL3** Fatal 
							  (crosscheck not successful)
					Warning
						Bit 4 Position A Warning, Position extrapolated
						Bit 5 Position B Warning, Position extrapolated
						Bit 6-7 Contrast:
							00 = OK
							01 = Service recommended (tape cleaning)
							10 = Warning (go to target floor and stop operation)
							11 = Stopped						 
	Byte 7: 			Status
					Clip
						Bit 0 Position Indicator Clip Camera A
							  Bit is set if a clip is detected on camera A - A offset if both set -- 
							  check clip offset for position information
						Bit 1 Position Indicator Clip Camera B
							  Bit is set if a clip is detected on camera B - 
							  check clip offset for position information
						Alignment
					Alignment left/right
						Bit 2 0 = left and 1 = right
						Bit 3-4 0 = centered, 1 = slightly off, 2 = moderately off, 3 = at the limit
					Alignment far/near
						Bit 5 0 = near and 1 = far
						Bit 6-7 0 = centered, 1 = slightly off, 2 = moderately off, 3 = at the limit
	Byte 8: 			Position A Low byte  Actual position in [0.5mm] steps
	Byte 9: 			Position A Middle byte
	Byte 10: 			Position A High byte
	Byte 11: 			Velocity A Low byte  Actual velocity in [mm/s]
	Byte 12: 			Velocity A High byte
	Byte 13: 			rx count Low byte
	Byte 14: 			rx count high byte
	Byte 15: 			vel_dir 
	Byte 16: 			online

*/

//*********************************************
// NTS Hoistway Learn Control
//*********************************************

void nts_hwl_control(void)
{
	if (rdinp(i_HWSET) == 1)
	{
		if ((rdinp(i_INS) == 1) || (rdinp(i_MRIN) == 1))
		{
			hwl_mode = 1;
	    	hwl_mode_car = 1;
		}
	}
	else if (hwl_mode_car == 1)
	{
		hwl_mode = 0;
		hwl_mode_car = 0;
	}

	if ((rdinp(i_INS) == 1) || (rdinp(i_MRIN) == 1))
	{
		NTS_spi.Command1.B.HW_LEARN = (hwl_mode == 1);
	
		if ((hwl_mode == 1) && (hwl_zero_hoistway == 1))
		{
			if ((NTS_spi.command1_ret & 0x20) != 0)
			{
				NTS_spi.Command1.B.CLR_HWTBL = 0;
				hwl_zero_hoistway = 0;
			}
			else
				NTS_spi.Command1.B.CLR_HWTBL = 1;
		}
	}
	else
	{
		hwl_mode = 0;
		hwl_zero_hoistway = 0;
		hwl_mark = 0;
		NTS_spi.Command1.B.HW_LEARN = 0;
		NTS_spi.Command1.B.CLR_HWTBL = 0;
		NTS_spi.Command1.B.MARK_FL = 0;
	}
	
}

//*********************************************
// Pack SPI I/O and command Data to NTS CPU
//*********************************************


void nts_spi_pack (uint8 msg)
{
	int16 i;
	uint8 valid_msg = 0;
	int16 len;
	uint8 chksum;
	
//	union {
//		uint8 b[4];			// byte: char 0 - 3
//		uint16 w[2];		// word: short int
//		uint32 dw;			// double word: int
//	}uiu;					// unsigned integer union
	union {
		uint8 b[2];			// byte: char 0 - 1
		uint16 w;			// word: short int
	}uwu;					// unsigned word union
	
//-	setgoutp(GRtest_osignal_array[4]);  // 4 for pack and 5 for unpack	
	switch (msg)
	{
		case 0:			// Test packet
			len = 18;
			iospi_tx_dma_buf[0] = '@';
			iospi_tx_dma_buf[1] = '0';		// packet number
			iospi_tx_dma_buf[2] = 1;
			iospi_tx_dma_buf[3] = 2;
			iospi_tx_dma_buf[4] = 3;
			iospi_tx_dma_buf[5] = 4;
			iospi_tx_dma_buf[6] = 5;
			iospi_tx_dma_buf[7] = 6;
			iospi_tx_dma_buf[8] = 7;
			iospi_tx_dma_buf[9] = 8;
			iospi_tx_dma_buf[10] = 9;
			iospi_tx_dma_buf[11] = 0;
			iospi_tx_dma_buf[12] = 0;
			iospi_tx_dma_buf[13] = 0;
			iospi_tx_dma_buf[14] = 0;
			iospi_tx_dma_buf[15] = 0;
			iospi_tx_dma_buf[16] = 0;
			iospi_tx_dma_buf[17] = 0;
			valid_msg = 1;
			break;
			
		case 1:			// Main CPU Command packet to NTS processor
			len = 18;
			iospi_tx_dma_buf[0] = '@';
			iospi_tx_dma_buf[1] = '1';
			iospi_tx_dma_buf[2] = NTS_spi.Command1.R;
			iospi_tx_dma_buf[3] = NTS_spi.Command2.R;
			iospi_tx_dma_buf[4] = NTS_spi.hwl_floor;
			iospi_tx_dma_buf[5] = NTS_spi.hwl_num_floors;
			iospi_tx_dma_buf[6] = 0;		// cpu status
			iospi_tx_dma_buf[7] = 0;		// cpu status
			iospi_tx_dma_buf[8] = NTS_SPI_TEST_3bit;
// ZYM 12-5-2017			
//-			iospi_tx_dma_buf[9] = revision;
			iospi_tx_dma_buf[9] = NTS_HW_Modify_Output.byte;

			iospi_tx_dma_buf[10] = NTS_spi.Command1.R;	// command verificataion
			iospi_tx_dma_buf[11] = NTS_spi.Command2.R;	// command verificaation
			if (nts_status_req == 1)
			{		// make sure the status is returned after each other request
				nts_status_req = 0;
				NTS_spi.pkt_req = 2;
			}
			else if (NTS_spi.eep_req == 1)
			{	// request eeprom packet
				NTS_spi.eep_req = 0;
				NTS_spi.pkt_req = 4;
				nts_status_req = 1;
			}
			else if (NTS_spi.can_req == 1)
			{	// request can bus packet
				NTS_spi.can_req = 0;
				NTS_spi.pkt_req = 8;
				nts_status_req = 1;
			}
			else if (NTS_spi.valid_fl_req == 1)
			{	// request valid floor table packet
				NTS_spi.valid_fl_req = 0;
				NTS_spi.pkt_req = 6;
				nts_status_req = 1;
			}
			iospi_tx_dma_buf[12] = NTS_spi.pkt_req;		// packet request
			iospi_tx_dma_buf[13] = 0;
			iospi_tx_dma_buf[14] = 0;
			iospi_tx_dma_buf[15] = 0;
			iospi_tx_dma_buf[16] = 0;
			iospi_tx_dma_buf[17] = 0;
			NTS_spi.pkt_req = 0;
			if (NTS_spi.Command2.B.RST_CNT == 1)
				NTS_spi.Command2.B.RST_CNT = 0;				// when set then send only once
			valid_msg = 1;
			break;
		
		case 3:			// 	Write Eeprom Packet
			SIU.GPDO[TST3].R = 0x01;
			len = 18;
			iospi_tx_dma_buf[0] = '@';
			iospi_tx_dma_buf[1] = '3';
			iospi_tx_dma_buf[2] = (uint8)(NTS_eep_ix + NTS_eep_count);
			i = 0;
			while (((NTS_eep_ix + NTS_eep_count) <= numntsvars) && (NTS_eep_count < NTS_eep_nmb) && (i<14))
			{
				uwu.w = nts_fvars[NTS_eep_ix + NTS_eep_count];
				iospi_tx_dma_buf[i+4] = uwu.b[lbw];
				i++;
				iospi_tx_dma_buf[i+4] = uwu.b[hbw];
				NTS_eep_count++;
				i++;
			}

			iospi_tx_dma_buf[3] = (uint8)(i);		// Number of valid parameter bytes sent

			while (i<14)
			{
				iospi_tx_dma_buf[i+4] = 0;		// zero unused bytes
				i++;
			}
				
				
			if (NTS_eep_count < NTS_eep_nmb)
			{
				if (put_nts_spi_msg_req(3) == 1)
					iospi_tx_dma_buf[2] |= 0x80;		
			}
			else
			{
				NTS_spi.eep_req = 1;	// read back parameters
			}
			valid_msg = 1;
			break;
			
		default:
			break;
	}
	
	if (valid_msg == 1)
	{
		nts_tx_count++;
		chksum = 0;
		for(i=0;i<len;i++)
		{
			chksum += iospi_tx_dma_buf[i];
			iospi_tx_dma_buf[i] |= 0x10020000;
		}
		iospi_tx_dma_buf[18] = chksum;			// check sum location
		iospi_tx_dma_buf[18] |= 0x10020000;
		iospi_tx_dma_buf[19] = ':';
		iospi_tx_dma_buf[19] |= 0x10020000;
		iospi_tx_dma_buf[20] = 0;
		iospi_tx_dma_buf[20] |= 0x10020000;
		iospi_start(len+3);
	}
}


//*********************************************
// Unpack SPI I/O and command Data from NTS CPU
//*********************************************

void nts_spi_unpack (void)
{
	int16 i,j,k;
	uint8 correct_spi_port = 0;
	uint8 chksum;
	uint8 pkt;
	
	union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
	
	static union {
			uint8 b[2];			// byte: char 0 - 1
			uint16 w;			// word: short int
	}uwu;					// unsigned word union
//- setgoutp(GRtest_osignal_array[11]); 
    ver_rev = NTS_spi.ver <<8;
    ver_rev += NTS_spi.rev;
	if ((iospi_tx_dma_buf[0] & 0x7FFF0000) == 0x10020000)
		correct_spi_port = 1;
	
	if ((spi_transferred == 1) && (correct_spi_port == 1))
	{
//-	    setgoutp(GRtest_osignal_array[10]);
		// Verify Packet
		if ((iospi_rx_dma_buf[1] == '@') && (iospi_rx_dma_buf[20] == ':'))
		{
//-		    setgoutp(GRtest_osignal_array[5]); 
			// calculate checksum
			chksum = 0;
			for(i=1;i<19;i++)
			{
				chksum += iospi_rx_dma_buf[i];
			}
			if (chksum == iospi_rx_dma_buf[19])
			{
//-			    setgoutp(GRtest_osignal_array[9]);
				pkt = (uint8)iospi_rx_dma_buf[2];
				
				if (pkt >= 0x30)
				{
					pkt = pkt - 0x30;
					ntsUnpkOK = 1;
//-					setgoutp(GRtest_osignal_array[8]);  
					sprintf(src,"nts_spi_unpack");
//                    Log_an_event(src); 
			        sprintf(src,"ver/rev=%d/%d pkt=%d",(uint8)iospi_rx_dma_buf[10],(uint8)iospi_rx_dma_buf[11], pkt);
//                    Log_an_event(src);	
					// valid packet
					switch(pkt)
					{
						/*
						Packet # 2		Read Command Return Data to NTS Processor 
							Byte 1:				Command 1 Return
							Byte 2:				Command 2 return						
							Byte 3:				Return Hoistway Learn Floor
							Byte 4:				Status 1:
							Byte 5:				Status 2:
							Byte 6:				Door Zone
							Byte 7:				Limits
							Byte 8:				Version
							Byte 9:				Revision 
							Byte 10:			NTS Velocity low byte
							Byte 11:			NTS Velocity high byte
							Byte 12: 			NTS Position Count low byte
							Byte 13:			NTS Position Count middle byte
							Byte 14: 			NTS Position Count high byte
							Byte 15:			Fault Byte							
							Byte 16:			Number of Occurances
							*/			

						case 2:
							NTS_spi.command1_ret = (uint8)iospi_rx_dma_buf[3];
							NTS_spi.command2_ret = (uint8)iospi_rx_dma_buf[4];
							NTS_spi.hwl_floor_ret = (uint8)iospi_rx_dma_buf[5];
							NTS_spi.Status1.R = (uint8)iospi_rx_dma_buf[6];
							NTS_spi.Status2.R = (uint8)iospi_rx_dma_buf[7];
							NTS_spi.doorzone = (uint8)iospi_rx_dma_buf[8];
							NTS_spi.limits = (uint8)iospi_rx_dma_buf[9];
							NTS_spi.ver = (uint8)iospi_rx_dma_buf[10];
							NTS_spi.rev = (uint8)iospi_rx_dma_buf[11];
							uwu.b[lbw] = (uint8)iospi_rx_dma_buf[12];
							uwu.b[hbw] = (uint8)iospi_rx_dma_buf[13];
							NTS_spi.velocity = uwu.w;
							uiu.b[lb] = (uint8)iospi_rx_dma_buf[14];
							uiu.b[mlb] = (uint8)iospi_rx_dma_buf[15];
							uiu.b[mhb] = (uint8)iospi_rx_dma_buf[16];
							uiu.b[hb] = 0;
							NTS_spi.pos_cnt = uiu.dw;
							if (iospi_rx_dma_buf[17] != 0)
							{
								fault_device = NTS_proc_addr;
								fault_prog_flag1 = iospi_rx_dma_buf[17];		// fault code
								fault_prog_flag2 = iospi_rx_dma_buf[18];		// fault count
								fault_device_2 = 0;	// speial code
	//							record_fault(f_NTS_device_fault);
							}
							hwl_fl_update = NTS_spi.Status1.B.Floor_Marked;	
							nts_spi_rcvd = 1;
							nts_rx_count++;
							break;
							
						/*
						Packet # 4		Send Read Eeprom Packet
							Byte 1:				Start Parameter Address (1-32)							
							Byte 2:				Number of Parameters (1-4)			
							Byte 3:				Parameter start Low Byte	
							Byte 4:				Parameter start High Byte
							Byte 5:				Parameter start + 1 Low Byte
							Byte 6:				Parameter start + 1 High Byte
							Byte 7:				Parameter start + 2 Low Byte
						 	Byte 8:				Parameter start + 2 High Byte			
							Byte 9: 			Parameter start + 3 Low Byte
							Byte 10: 			Parameter start + 3 High Byte
							Byte 11: 			Parameter start + 4 Low Byte
							Byte 12: 			Parameter start + 4 High Byte
							Byte 13: 			Parameter start + 5 Low Byte
							Byte 14: 			Parameter start + 5 High Byte
							Byte 15: 			Parameter start + 6 Low Byte
							Byte 16: 			Parameter start + 6 High Byte
							*/
						case 4:
							i = iospi_rx_dma_buf[3];		// Start index
							j = iospi_rx_dma_buf[4];		// Number of bytes
							k = 0;
							if ((j > 0) && (j <= 14))
							{
								while ((i < (int)(sizeof(NTS_spi.eeprom)/2)) && (k < j))
								{
									uwu.b[lbw] = (uint8)iospi_rx_dma_buf[k+5];
									k++;
									uwu.b[hbw] = (uint8)iospi_rx_dma_buf[k+5];
									k++;
									NTS_spi.eeprom[i] = uwu.w;
									i++;
								}
							}
							if (i >= (int)(sizeof(NTS_spi.eeprom)/2))
							{
								for(i=0;i<numntsvars;i++)
								{
									nts_fvars[i] = NTS_spi.eeprom[i];
								}
								NTS_spi.eep_rcvd = 1;
								SIU.GPDO[TST3].R = 0x00;
								NTS_spi.eep_req = 0;
							}
							nts_spi_rcvd = 1;
							nts_rx_count++;
							
							break;
							/*
						Packet # 6		Send Valid Table Setup Data
							Byte 1:				Valid Floor Table low byte
							Byte 2:				Valid Floor Table high byte
							Byte 3:				unused							
							Byte 4:				unused			
							Byte 5:				unused	
							Byte 6:				unused
							Byte 7:				unused
							Byte 8:				unused
							Byte 9: 			Valid Clip Table low byte
							Byte 10: 			Valid Clip Table high byte
							Byte 11: 			unused
							Byte 12: 			unused
							Byte 13: 			unused
							Byte 14: 			unused
							Byte 15: 			unused
							Byte 16: 			unused
							*/
						case 6:
							uiu.b[lb] = (uint8)iospi_rx_dma_buf[3];
							uiu.b[mlb] = (uint8)iospi_rx_dma_buf[4];
							uiu.b[mhb] = 0;
							uiu.b[hb] = 0;
							NTS_spi.valid_fl_tbl = uiu.dw;
							
							uiu.b[lb] = (uint8)iospi_rx_dma_buf[11];
							uiu.b[mlb] = (uint8)iospi_rx_dma_buf[12];
							uiu.b[mhb] = 0;
							uiu.b[hb] = 0;
							NTS_spi.valid_clip_tbl = uiu.dw;
							NTS_spi.valid_fl_rcvd = 1;
							nts_spi_rcvd = 1;
							nts_rx_count++;
							break;
							
							/*
							Packet #8		Send Selector Status
								Byte 1: 			Position B Low byte  Actual position in [0.5mm] steps
								Byte 2: 			Position B Middle byte
								Byte 3: 			Position B High byte
								Byte 4: 			Velocity B Low byte  Actual velocity in [mm/s]
								Byte 5: 			Velocity B High byte
								Byte 6: 			Error/Warn 
								Byte 7: 			Status
								Byte 8: 			Position A Low byte  Actual position in [0.5mm] steps
								Byte 9: 			Position A Middle byte
								Byte 10: 			Position A High byte
								Byte 11: 			Velocity A Low byte  Actual velocity in [mm/s]
								Byte 12: 			Velocity A High byte
								Byte 13: 			rx count Low byte
								Byte 14: 			rx count high byte
								Byte 15: 			vel_dir 
								Byte 16: 			online
							*/
						case 8:
							uiu.b[lb] = (uint8)iospi_rx_dma_buf[3];
							uiu.b[mlb] = (uint8)iospi_rx_dma_buf[4];
							uiu.b[mhb] = (uint8)iospi_rx_dma_buf[5];
							uiu.b[hb] = 0;
							NTS_sel.pos_cnt_B = uiu.dw;
							uwu.b[lbw] = (uint8)iospi_rx_dma_buf[6];
							uwu.b[hbw] = (uint8)iospi_rx_dma_buf[7];
							NTS_sel.vel = uwu.w;
							NTS_sel.Error_Warning.R = (uint8)iospi_rx_dma_buf[8];
							NTS_sel.Status.R = (uint8)iospi_rx_dma_buf[9];
							NTS_sel.clip_offset = (uint8)iospi_rx_dma_buf[10];
							NTS_sel.ver = (uint8)iospi_rx_dma_buf[11];
							uiu.b[lb] = (uint8)iospi_rx_dma_buf[12];
							uiu.b[mlb] = (uint8)iospi_rx_dma_buf[13];
							uiu.b[mhb] = (uint8)iospi_rx_dma_buf[14];
							uiu.b[hb] = 0;
							NTS_sel.pos_cnt_A = uiu.dw;
							uwu.b[lbw] = (uint8)iospi_rx_dma_buf[15];
							uwu.b[hbw] = (uint8)iospi_rx_dma_buf[16];
							NTS_sel.rx_cnt = uwu.w;
							NTS_sel.vel_dir = (uint8)iospi_rx_dma_buf[17];
							NTS_sel.online = (uint8)iospi_rx_dma_buf[18];
							NTS_spi.can_rcvd = 1;
							nts_spi_rcvd = 1;
							nts_rx_count++;
							break;


						default:
							break;
					}

				}
			}
		}
		
	}
}

//--------------------------------------
// Put message request on que
//--------------------------------------
int16 put_nts_spi_msg_req (int8 msg)
{
	int16 ix = 0;

	while(ix < nts_spi_que_len)
	{      // look for and open spot on the que
		if (nts_spi_msg_req[ix] == 0)
		{         // found an open spot so place Packet on que
			nts_spi_msg_req[ix] = msg;
			return 1;	// data on the que
		}
		else     // index to next spot (if non found, throw the data away)
			ix++;
	}
	return 0;		// que is full
}


//--------------------------------------
// Zero message request in que
//--------------------------------------
void zero_nts_spi_msg_req (void)
{
	int16 i;
	
	for (i=0;i<nts_spi_que_len;i++)
		nts_spi_msg_req[i] = 0;
}

//--------------------------------------
// Get message request from que
//--------------------------------------
int8 get_nts_spi_msg_req (void)
{
	int16 ix = 0;
	int8 msg = 0;			// return 0 if no Packet on queue

	while (ix < nts_spi_que_len)
	{
		if (nts_spi_msg_req[ix] != 0) 
		{      	// Packet for this device found
			msg = nts_spi_msg_req[ix];
			nts_spi_msg_req[ix] = 0;
			break;
		}
		else
			ix++;
	}

	// shift que data to the top (start where ix left off)

	while (ix < (nts_spi_que_len-1))
	{
		if (nts_spi_msg_req[ix+1] != 0)
		{   // Packet was taken from the middle of the que so shift data up
			nts_spi_msg_req[ix] = nts_spi_msg_req[ix+1];
			nts_spi_msg_req[ix+1] = 0;
			ix++;
		}
		else
			break;
	}
	return (msg);
}
