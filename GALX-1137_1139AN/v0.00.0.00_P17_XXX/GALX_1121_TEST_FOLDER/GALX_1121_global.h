//ZYM 12-18-2017
//FPGA1 SPI Output
extern union FPGA1_HW_Modify_Output_tag
{
	uint8 buff[2];
	uint16 mc_drop_delay;
	
// ZYM 12-12-2017
//Wrong Mapping to FPGA1	
/* //-	   
	struct
	{
		// To FPGA1 its high byte and MATCHing GALX1132 CPU high byte		 		
		uint16 PDT_LP   : 1; 		//15
		uint16 PUT_LP   : 1;        //14
		uint16 PDN_LP   : 1;        //13
		uint16 PUN_LP   : 1;		//12
		uint16 PSDF_LP  : 1;		//11 
		uint16 PSUF_LP  : 1;		//10
		uint16 PSD_LP   : 1;		//9 
		uint16 PSU_LP   : 1;		//8 
		
		// To FPGA1 its low byte and MATCHing GALX1132 CPU low byte
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
		// To FPGA1 its LOW byte and MATCHing GALX1132 CPU HIGH byte	
		uint16 UNUSED   : 2;	
		
		uint16 DZ_PAL   : 1; 			//13  		*It is from FPGA1 TO FPGA2, But For Testing Code 
												   //CPU NO NEED to READ back From FPGA1(DEFAULT Value '0')
												   //BUT CPU READ back from FPGA2  
		//CPU sending 1 to FPGA1
        //FPGA2 return 6 back to CPU 	
		uint16 CPLD1_SPI_TEST_bit : 1; 	//12  using for SPI commucation check between CPU and CPLD1			
 		uint16 PMC_LP   : 1;			//11 
 		uint16 PSTE_LP  : 1;			//10
 		uint16 PCEN_LP  : 1;			//9 	SAME AS MC1
 		uint16 PRUN_LP  : 1;			//8 
		
		// To FPGA1 its HIGH byte and MATCHing GALX1132 CPU LOW byte	
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



//FPAG2 SPI Output
extern union iospi_out_tag
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




// ZYM 12-18-2017
//NTS SPI Output
extern union NTS_HW_Modify_Output_tag
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


//NTS CAN Output
extern union galx_1121tester_NTS_Modify_Output_tag
{
	uint8 byte;
	struct 
	{
		uint8 UNUSED : 2;
		uint8 PAL_FAULT_out   : 1;  //5
		uint8 SEL_OK_NTS_out  : 1;	//4
		uint8 NTS_FLT_out 	  : 1;	//3
		uint8 SPD_75_NTS_out  : 1;	//2
		uint8 SPD_150_NTS_out : 1;	//1	
		uint8 DZ_NTS_out      : 1;	//0
	};
} galx_1121tester_NTS_Modify_Output;



//NTS CAN Input
extern union galx_1121tester_NTS_Modify_Input_tag
{
	uint8 buff[1];
	
	struct galx_1121tester_NTS_Modify_tag
	{
		union
		{
			uint8 byte;
			struct 
			{		
				uint8 UNUSED 		  : 7;	
				uint8 test_mode_in    : 1;	//0										
			}B;
		}buff0;		
	}ST;	
} galx_1121tester_NTS_Modify_Input;




//------------------------------------------------------------------------------
//base on 100ms tick "timers[t_Generic_Delay]"
#define test_halfsec_delay	5
#define test_1s_delay		10
#define test_2s_delay		20
#define test_3s_delay		30
#define test_4s_delay		40
#define test_5s_delay		50
#define test_10s_delay		100
#define test_20s_delay		200
#define test_30s_delay		300































