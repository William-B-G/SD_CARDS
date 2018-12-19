// MEMORY MAP 1/15/1998 REVISION 1.10



#if (Little_Endian == 1)
	#define lb 0
	#define mlb 1
	#define mhb 2
	#define hb 3
	#define lbw 0
	#define hbw 1
	#define lw 0
	#define hw 1
#else
	#define lb 3
	#define mlb 2
	#define mhb 1
	#define hb 0
	#define lbw 1
	#define hbw 0
	#define lw 1
	#define hw 0
#endif
// int16 B = 0;
// short int x;
// B = sizeof x;
typedef int int32;
typedef unsigned int uint32;
typedef short int int16;
typedef unsigned short int uint16;
typedef signed char int8;
typedef unsigned char uint8;
typedef unsigned char bool;

#define FATFS_NO_DEF_TYPES

#define true 1
#define false 0

#if (SATV40 == 1)
#define c_dbn_cnt 3
#else 
 #if (TS3300 == 1)
	#define c_dbn_cnt 4
 #else
	#define c_dbn_cnt 5
 #endif
#endif

#define c_io_dbn_time 100  	// 1 second
#define c_io_dbn_time_short 10  	// 1 second

#define fl_size 13
#define car_size 9 
#define c_max_hc_boards 27
#define NumMaxFl 12	  //  Number of Maxium Floor

// Security Variables
#define NumMaxCd 200  //  Number of maxium Code per Floor
#define Fl_Code_Length ((NumMaxCd*4) + 10)	  // +10: 6 for valid, numcode, chksum and 4 for extra zeros

#define BIT1 0x01
#define BIT2 0x02
#define BIT3 0x04
#define BIT4 0x08
#define BIT5 0x10
#define BIT6 0x20
#define BIT7 0x40
#define BIT8 0x80

#define c_misc_io_addr 140		// Misc. i/o address to display data from brake board and others

/*

// Timer enable and watch-dog timer address for com_mem board

#define c_timer_en_addr 0x24a

// defining the battery backed memory map for each file

#define C1036_Page_Low_Add 0x243
#define C1036_Page_Mid_Add 0x244
#define C1036_Page_High_Add 0x245

#define C1036_EERam_Add 0x246
#define C1036_NVRam_Add 0x247
#define C1036_EERam_Add_NI 0x248	// EERam address without automatic address increment
#define C1036_NVRam_Add_NI 0x249	// NVRam address without automatic address increment

// TS5300 Digital I/O Ports
	#define 	DIO1_ctl 0x7A
	#define 	DIO1_port 0x7B
	#define		DIO2_ctl 0x7D
	#define		DIO2_port 0x7E


		// EERam Memory Location   - Each Page is 8K or 32K
 
#define Pwrup_Dat_Page 0
#define Pwrup_Dat_Offset 0				// allow 30 bytes

#define RTC_Dat_Page 0					// Real time clock Year 100's and update flag
#define RTC_Dat_Offset 0x20				

#define Fire_Dat_Page 0					// allow more than 24 bytes
#define Fire_Dat_Offset 0x100			// 100h = 256 bytes

#define FVARS_Dat_Page 0
#define FVARS_Dat_Offset 0x200			// 180 * 2 = 360 bytes + 19 + 18 + 16 = 413	(200h = 512 bytes)

#define PassW_Dat_Page 0
#define PassW_Dat_Offset 0x400			// 6 * 19 * 2 = 228 bytes long  (100h = 256)

#define LW_Dat_Page 0					 // load weigh data moved from fvars data
#define LW_Dat_Offset 0x500				// load weigh data per floor 60 * 4 = 240 bytes	 (100h = 256)

#define Hoist_Dat_Page 0				
#define Hoist_Dat_Offset 0x600	 		// 9 limits * 4 * 2 = 72 + (60 * 4) = 312 + flag (2) = 314 for 60 floors

// modified 7/22/08 mhd --- no longer use dz offset table or floor offset --- use DZU_Dist, DZD_Dist, Up_fl_level_dist and Dn_fl_level_dist
#define Fl_Update_Adjust_Page 0				// (4 * 4) = 16 Door zone distance and level distance both up and down 
#define Fl_Update_Adjust_Offset 0x1000	

#define FVARS_Dat_Page2 0
#define FVARS_Dat_Offset2 0x1300		// 180 * 2 = 360 (200h = 512 bytes)	0x1300 - 0x1500	 

#define Group_Timer_Page 0				// Group timers for service activations (AFFh = 2815) bytes max of 57 Service timers
#define Group_Timer_Offset 0x1500	 	// 0x1500 - 0x1FFF   

#define FVARS_Dat_Page3 0
#define FVARS_Dat_Offset3a 0x1D00		// 180 * 2 = 360 (200h = 512 bytes)	0x2000 - 0x2200 (for use with smaller eeprom chips)	 
#define FVARS_Dat_Offset3 0x2000		// 180 * 2 = 360 (200h = 512 bytes)	0x2000 - 0x2200	 



#if (Tract_HR == 1)
			// NVRam Memory blocks -	 Each page is 64K  - Total 256K

	#define Stats_Dat_Page 0				// Job Statistics
	#define Stats_Dat_Offset 0				// allow minimum of 54 bytes

	#define Sys_Stat_Dat_Page 0
	#define Sys_Stat_Dat_Offset 0x200		// 200H = 512 bytes
	#define Sys_Stat_Size 255				// allow minimum of 2550 bytes	(1000 - 200h = e00h = 3584

	#define Flight_Dat_Page 0		 		// ETA flight times
	#define Flight_Dat_Offset 0x1000	  	// length of 2048 for 32 floors	 (6000h - 1000h = 5000h = 20480 bytes)

	#define Flts_Dat_Page 0
	#define Flts_Dat_Offset 0x6000			// 80 * 50  = 4000	 (2000H = 8192 bytes) Allow for increased fault data

	#define SUN_Dat_Page 0					// 4 bytes per call count per floor	per 15 min period (4 * 24) = 96 periods
	#define SUN_Dat_Offset 0x8000			// for 32 floors = 4 * 97 * 30 = 11640	  (8000h = 32767 bytes)

	#define MON_Dat_Page 1					// 32767 / (4 * 97) = 84 floors max
	#define MON_Dat_Offset 0x0000		

	#define TUE_Dat_Page 1
	#define TUE_Dat_Offset 0x8000

	#define WED_Dat_Page 2			
	#define WED_Dat_Offset 0x0000		

	#define THR_Dat_Page 2
	#define THR_Dat_Offset 0x8000

	#define FRI_Dat_Page 3			
	#define FRI_Dat_Offset 0x0000		

	#define SAT_Dat_Page 3
	#define SAT_Dat_Offset 0x8000

   //Security Floor Code  
	
	#define Sec_code_page1 4
	#define Sec_page_1_offset 0

	#define FL1_Code_Page Sec_code_page1
	#define FL1_CodeTbl_OffSet Sec_page_1_offset
	#define FL1_Code_Offset FL1_CodeTbl_OffSet+6		  

    #define FL2_Code_Page Sec_code_page1
	#define FL2_CodeTbl_OffSet FL1_CodeTbl_OffSet+Fl_Code_Length
	#define FL2_Code_Offset FL2_CodeTbl_OffSet+6		  

    #define FL3_Code_Page Sec_code_page1
	#define FL3_CodeTbl_OffSet FL2_CodeTbl_OffSet+Fl_Code_Length
	#define FL3_Code_Offset FL3_CodeTbl_OffSet+6         

	#define FL4_Code_Page Sec_code_page1
	#define FL4_CodeTbl_OffSet FL3_CodeTbl_OffSet+Fl_Code_Length
	#define FL4_Code_Offset FL4_CodeTbl_OffSet+6         

    #define FL5_Code_Page Sec_code_page1
	#define FL5_CodeTbl_OffSet FL4_CodeTbl_OffSet+Fl_Code_Length
	#define FL5_Code_Offset FL5_CodeTbl_OffSet+6         

    #define FL6_Code_Page Sec_code_page1
	#define FL6_CodeTbl_OffSet FL5_CodeTbl_OffSet+Fl_Code_Length
	#define FL6_Code_Offset FL6_CodeTbl_OffSet+6         

	#define FL7_Code_Page Sec_code_page1
	#define FL7_CodeTbl_OffSet FL6_CodeTbl_OffSet+Fl_Code_Length
	#define FL7_Code_Offset FL7_CodeTbl_OffSet+6         

    #define FL8_Code_Page Sec_code_page1
	#define FL8_CodeTbl_OffSet FL7_CodeTbl_OffSet+Fl_Code_Length
	#define FL8_Code_Offset FL8_CodeTbl_OffSet+6         

    #define FL9_Code_Page Sec_code_page1							 
	#define FL9_CodeTbl_OffSet FL8_CodeTbl_OffSet+Fl_Code_Length
	#define FL9_Code_Offset FL9_CodeTbl_OffSet+6         

	#define FL10_Code_Page Sec_code_page1
	#define FL10_CodeTbl_OffSet FL9_CodeTbl_OffSet+Fl_Code_Length
	#define FL10_Code_Offset FL10_CodeTbl_OffSet+6         

    #define FL11_Code_Page Sec_code_page1
	#define FL11_CodeTbl_OffSet FL10_CodeTbl_OffSet+Fl_Code_Length
	#define FL11_Code_Offset FL11_CodeTbl_OffSet+6         

    #define FL12_Code_Page Sec_code_page1
	#define FL12_CodeTbl_OffSet FL11_CodeTbl_OffSet+Fl_Code_Length
	#define FL12_Code_Offset FL12_CodeTbl_OffSet+6         

	#define FL13_Code_Page Sec_code_page1
	#define FL13_CodeTbl_OffSet FL12_CodeTbl_OffSet+Fl_Code_Length
	#define FL13_Code_Offset FL13_CodeTbl_OffSet+6         

    #define FL14_Code_Page Sec_code_page1
	#define FL14_CodeTbl_OffSet FL13_CodeTbl_OffSet+Fl_Code_Length
	#define FL14_Code_Offset FL14_CodeTbl_OffSet+6         

    #define FL15_Code_Page Sec_code_page1
	#define FL15_CodeTbl_OffSet FL14_CodeTbl_OffSet+Fl_Code_Length
	#define FL15_Code_Offset FL15_CodeTbl_OffSet+6         

	#define FL16_Code_Page Sec_code_page1
	#define FL16_CodeTbl_OffSet FL15_CodeTbl_OffSet+Fl_Code_Length
	#define FL16_Code_Offset FL16_CodeTbl_OffSet+6         

    #define FL17_Code_Page Sec_code_page1
	#define FL17_CodeTbl_OffSet FL16_CodeTbl_OffSet+Fl_Code_Length
	#define FL17_Code_Offset FL17_CodeTbl_OffSet+6         

    #define FL18_Code_Page Sec_code_page1
	#define FL18_CodeTbl_OffSet FL17_CodeTbl_OffSet+Fl_Code_Length
	#define FL18_Code_Offset FL18_CodeTbl_OffSet+6         

	#define FL19_Code_Page Sec_code_page1
	#define FL19_CodeTbl_OffSet FL18_CodeTbl_OffSet+Fl_Code_Length
	#define FL19_Code_Offset FL19_CodeTbl_OffSet+6         

    #define FL20_Code_Page Sec_code_page1
	#define FL20_CodeTbl_OffSet FL19_CodeTbl_OffSet+Fl_Code_Length
	#define FL20_Code_Offset FL20_CodeTbl_OffSet+6         

    #define FL21_Code_Page Sec_code_page1
	#define FL21_CodeTbl_OffSet FL20_CodeTbl_OffSet+Fl_Code_Length
	#define FL21_Code_Offset FL21_CodeTbl_OffSet+6         

	#define FL22_Code_Page Sec_code_page1
	#define FL22_CodeTbl_OffSet FL21_CodeTbl_OffSet+Fl_Code_Length
	#define FL22_Code_Offset FL22_CodeTbl_OffSet+6         

    #define FL23_Code_Page Sec_code_page1
	#define FL23_CodeTbl_OffSet FL22_CodeTbl_OffSet+Fl_Code_Length
	#define FL23_Code_Offset FL23_CodeTbl_OffSet+6         

    #define FL24_Code_Page Sec_code_page1
	#define FL24_CodeTbl_OffSet FL23_CodeTbl_OffSet+Fl_Code_Length
	#define FL24_Code_Offset FL24_CodeTbl_OffSet+6         

	#define FL25_Code_Page Sec_code_page1
	#define FL25_CodeTbl_OffSet FL24_CodeTbl_OffSet+Fl_Code_Length
	#define FL25_Code_Offset FL25_CodeTbl_OffSet+6         

    #define FL26_Code_Page Sec_code_page1
	#define FL26_CodeTbl_OffSet FL25_CodeTbl_OffSet+Fl_Code_Length
	#define FL26_Code_Offset FL26_CodeTbl_OffSet+6         

    #define FL27_Code_Page Sec_code_page1
	#define FL27_CodeTbl_OffSet FL26_CodeTbl_OffSet+Fl_Code_Length
	#define FL27_Code_Offset FL27_CodeTbl_OffSet+6         

	#define FL28_Code_Page Sec_code_page1
	#define FL28_CodeTbl_OffSet FL27_CodeTbl_OffSet+Fl_Code_Length
	#define FL28_Code_Offset FL28_CodeTbl_OffSet+6         

    #define FL29_Code_Page Sec_code_page1
	#define FL29_CodeTbl_OffSet FL28_CodeTbl_OffSet+Fl_Code_Length
	#define FL29_Code_Offset FL29_CodeTbl_OffSet+6         

    #define FL30_Code_Page Sec_code_page1
	#define FL30_CodeTbl_OffSet FL29_CodeTbl_OffSet+Fl_Code_Length
	#define FL30_Code_Offset FL30_CodeTbl_OffSet+6         
	
	//Floors  31 to 60
	#define Sec_code_page2 4
	#define Sec_page_2_offset FL30_CodeTbl_OffSet+Fl_Code_Length

	#define FL31_Code_Page Sec_code_page2
	#define FL31_CodeTbl_OffSet Sec_page_2_offset
	#define FL31_Code_Offset FL31_CodeTbl_OffSet+6		  

    #define FL32_Code_Page Sec_code_page2
	#define FL32_CodeTbl_OffSet FL31_CodeTbl_OffSet+Fl_Code_Length
	#define FL32_Code_Offset FL32_CodeTbl_OffSet+6		  

    #define FL33_Code_Page Sec_code_page2
	#define FL33_CodeTbl_OffSet FL32_CodeTbl_OffSet+Fl_Code_Length
	#define FL33_Code_Offset FL33_CodeTbl_OffSet+6		  

	#define FL34_Code_Page Sec_code_page2
	#define FL34_CodeTbl_OffSet FL33_CodeTbl_OffSet+Fl_Code_Length
	#define FL34_Code_Offset FL34_CodeTbl_OffSet+6		  

    #define FL35_Code_Page Sec_code_page2
	#define FL35_CodeTbl_OffSet FL34_CodeTbl_OffSet+Fl_Code_Length
	#define FL35_Code_Offset FL35_CodeTbl_OffSet+6		  

    #define FL36_Code_Page Sec_code_page2
	#define FL36_CodeTbl_OffSet FL35_CodeTbl_OffSet+Fl_Code_Length
	#define FL36_Code_Offset FL36_CodeTbl_OffSet+6		  

	#define FL37_Code_Page Sec_code_page2
	#define FL37_CodeTbl_OffSet FL36_CodeTbl_OffSet+Fl_Code_Length
	#define FL37_Code_Offset FL37_CodeTbl_OffSet+6		  

    #define FL38_Code_Page Sec_code_page2
	#define FL38_CodeTbl_OffSet FL37_CodeTbl_OffSet+Fl_Code_Length
	#define FL38_Code_Offset FL38_CodeTbl_OffSet+6		  

    #define FL39_Code_Page Sec_code_page2
	#define FL39_CodeTbl_OffSet FL38_CodeTbl_OffSet+Fl_Code_Length
	#define FL39_Code_Offset FL39_CodeTbl_OffSet+6		  

	#define FL40_Code_Page Sec_code_page2
	#define FL40_CodeTbl_OffSet FL39_CodeTbl_OffSet+Fl_Code_Length
	#define FL40_Code_Offset FL40_CodeTbl_OffSet+6		  

    #define FL41_Code_Page Sec_code_page2
	#define FL41_CodeTbl_OffSet FL40_CodeTbl_OffSet+Fl_Code_Length
	#define FL41_Code_Offset FL41_CodeTbl_OffSet+6		  

    #define FL42_Code_Page Sec_code_page2
	#define FL42_CodeTbl_OffSet FL41_CodeTbl_OffSet+Fl_Code_Length
	#define FL42_Code_Offset FL42_CodeTbl_OffSet+6		  

	#define FL43_Code_Page Sec_code_page2
	#define FL43_CodeTbl_OffSet FL42_CodeTbl_OffSet+Fl_Code_Length
	#define FL43_Code_Offset FL43_CodeTbl_OffSet+6		  

    #define FL44_Code_Page Sec_code_page2
	#define FL44_CodeTbl_OffSet FL43_CodeTbl_OffSet+Fl_Code_Length
	#define FL44_Code_Offset FL44_CodeTbl_OffSet+6		  

    #define FL45_Code_Page Sec_code_page2
	#define FL45_CodeTbl_OffSet FL44_CodeTbl_OffSet+Fl_Code_Length
	#define FL45_Code_Offset FL45_CodeTbl_OffSet+6		  

	#define FL46_Code_Page Sec_code_page2
	#define FL46_CodeTbl_OffSet FL45_CodeTbl_OffSet+Fl_Code_Length
	#define FL46_Code_Offset FL46_CodeTbl_OffSet+6		  

    #define FL47_Code_Page Sec_code_page2
	#define FL47_CodeTbl_OffSet FL46_CodeTbl_OffSet+Fl_Code_Length
	#define FL47_Code_Offset FL47_CodeTbl_OffSet+6		  

    #define FL48_Code_Page Sec_code_page2
	#define FL48_CodeTbl_OffSet FL47_CodeTbl_OffSet+Fl_Code_Length
	#define FL48_Code_Offset FL48_CodeTbl_OffSet+6		  

	#define FL49_Code_Page Sec_code_page2
	#define FL49_CodeTbl_OffSet FL48_CodeTbl_OffSet+Fl_Code_Length
	#define FL49_Code_Offset FL49_CodeTbl_OffSet+6		  

    #define FL50_Code_Page Sec_code_page2
	#define FL50_CodeTbl_OffSet FL49_CodeTbl_OffSet+Fl_Code_Length
	#define FL50_Code_Offset FL50_CodeTbl_OffSet+6		  

    #define FL51_Code_Page Sec_code_page2
	#define FL51_CodeTbl_OffSet FL50_CodeTbl_OffSet+Fl_Code_Length
	#define FL51_Code_Offset FL51_CodeTbl_OffSet+6		  

	#define FL52_Code_Page Sec_code_page2
	#define FL52_CodeTbl_OffSet FL51_CodeTbl_OffSet+Fl_Code_Length
	#define FL52_Code_Offset FL52_CodeTbl_OffSet+6		  

    #define FL53_Code_Page Sec_code_page2
	#define FL53_CodeTbl_OffSet FL52_CodeTbl_OffSet+Fl_Code_Length
	#define FL53_Code_Offset FL53_CodeTbl_OffSet+6		  

    #define FL54_Code_Page Sec_code_page2
	#define FL54_CodeTbl_OffSet FL53_CodeTbl_OffSet+Fl_Code_Length
	#define FL54_Code_Offset FL54_CodeTbl_OffSet+6		  

	#define FL55_Code_Page Sec_code_page2
	#define FL55_CodeTbl_OffSet FL54_CodeTbl_OffSet+Fl_Code_Length
	#define FL55_Code_Offset FL55_CodeTbl_OffSet+6		  

    #define FL56_Code_Page Sec_code_page2
	#define FL56_CodeTbl_OffSet FL55_CodeTbl_OffSet+Fl_Code_Length
	#define FL56_Code_Offset FL56_CodeTbl_OffSet+6		  

    #define FL57_Code_Page Sec_code_page2
	#define FL57_CodeTbl_OffSet FL56_CodeTbl_OffSet+Fl_Code_Length
	#define FL57_Code_Offset FL57_CodeTbl_OffSet+6		  

	#define FL58_Code_Page Sec_code_page2
	#define FL58_CodeTbl_OffSet FL57_CodeTbl_OffSet+Fl_Code_Length
	#define FL58_Code_Offset FL58_CodeTbl_OffSet+6		  

    #define FL59_Code_Page Sec_code_page2
	#define FL59_CodeTbl_OffSet FL58_CodeTbl_OffSet+Fl_Code_Length
	#define FL59_Code_Offset FL59_CodeTbl_OffSet+6		  

    #define FL60_Code_Page Sec_code_page2
	#define FL60_CodeTbl_OffSet FL59_CodeTbl_OffSet+Fl_Code_Length
	#define FL60_Code_Offset FL60_CodeTbl_OffSet+6		  

	//Code Lookup Table	
	#define Code_LookUpTbl_Page 5
	#define Code_LookUpTbl_Offset 0
	#define Fl_LookUpTbl_Length 120

	#define Code_LookUpTbl_Size 7200    // 60*2*60 
				
    #define FL1_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL1_LookUpTbl_OffSet Code_LookUpTbl_Offset
	
	#define FL2_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL2_LookUpTbl_OffSet FL1_LookUpTbl_OffSet+Fl_LookUpTbl_Length	 // 60 * 2 120 bytes

	#define FL3_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL3_LookUpTbl_OffSet FL2_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL4_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL4_LookUpTbl_OffSet FL3_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL5_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL5_LookUpTbl_OffSet FL4_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL6_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL6_LookUpTbl_OffSet FL5_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL7_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL7_LookUpTbl_OffSet FL6_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL8_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL8_LookUpTbl_OffSet FL7_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL9_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL9_LookUpTbl_OffSet FL8_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL10_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL10_LookUpTbl_OffSet FL9_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL11_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL11_LookUpTbl_OffSet FL10_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL12_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL12_LookUpTbl_OffSet FL11_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL13_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL13_LookUpTbl_OffSet FL12_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL14_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL14_LookUpTbl_OffSet FL13_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL15_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL15_LookUpTbl_OffSet FL14_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL16_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL16_LookUpTbl_OffSet FL15_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL17_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL17_LookUpTbl_OffSet FL16_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL18_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL18_LookUpTbl_OffSet FL17_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL19_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL19_LookUpTbl_OffSet FL18_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL20_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL20_LookUpTbl_OffSet FL19_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL21_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL21_LookUpTbl_OffSet FL20_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL22_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL22_LookUpTbl_OffSet FL21_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL23_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL23_LookUpTbl_OffSet FL22_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL24_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL24_LookUpTbl_OffSet FL23_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL25_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL25_LookUpTbl_OffSet FL24_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL26_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL26_LookUpTbl_OffSet FL25_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL27_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL27_LookUpTbl_OffSet FL26_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL28_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL28_LookUpTbl_OffSet FL27_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL29_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL29_LookUpTbl_OffSet FL28_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL30_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL30_LookUpTbl_OffSet FL29_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL31_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL31_LookUpTbl_OffSet FL30_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL32_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL32_LookUpTbl_OffSet FL31_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL33_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL33_LookUpTbl_OffSet FL32_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL34_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL34_LookUpTbl_OffSet FL33_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL35_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL35_LookUpTbl_OffSet FL34_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL36_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL36_LookUpTbl_OffSet FL35_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL37_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL37_LookUpTbl_OffSet FL36_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL38_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL38_LookUpTbl_OffSet FL37_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL39_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL39_LookUpTbl_OffSet FL38_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL40_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL40_LookUpTbl_OffSet FL39_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL41_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL41_LookUpTbl_OffSet FL40_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL42_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL42_LookUpTbl_OffSet FL41_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL43_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL43_LookUpTbl_OffSet FL42_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL44_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL44_LookUpTbl_OffSet FL43_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL45_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL45_LookUpTbl_OffSet FL44_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL46_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL46_LookUpTbl_OffSet FL45_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL47_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL47_LookUpTbl_OffSet FL46_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL48_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL48_LookUpTbl_OffSet FL47_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL49_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL49_LookUpTbl_OffSet FL48_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL50_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL50_LookUpTbl_OffSet FL49_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL51_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL51_LookUpTbl_OffSet FL50_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL52_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL52_LookUpTbl_OffSet FL51_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL53_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL53_LookUpTbl_OffSet FL52_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL54_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL54_LookUpTbl_OffSet FL53_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL55_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL55_LookUpTbl_OffSet FL54_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL56_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL56_LookUpTbl_OffSet FL55_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL57_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL57_LookUpTbl_OffSet FL56_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL58_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL58_LookUpTbl_OffSet FL57_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL59_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL59_LookUpTbl_OffSet FL58_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL60_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL60_LookUpTbl_OffSet FL59_LookUpTbl_OffSet+Fl_LookUpTbl_Length

#else
			// NVRam Memory blocks -	 Each page is 64K  - Total 128K

	#define Stats_Dat_Page 0				// Job Statistics
	#define Stats_Dat_Offset 0				// allow minimum of 54 bytes

	#define Sys_Stat_Dat_Page 0
	#define Sys_Stat_Dat_Offset 0x200		// 200H = 512 bytes
	#define Sys_Stat_Size 255				// allow minimum of 2550 bytes	(1000 - 200h = e00h = 3584

	#define Flight_Dat_Page 0		 		// ETA flight times
	#define Flight_Dat_Offset 0x1000	  	// length of 2048 for 32 floors	 (3000h - 1000h = 2000h = 8192 bytes)

	#define Flts_Dat_Page 0
	#define Flts_Dat_Offset 0x3000			// 80 * 50  = 4000	 (1000H = 4096 bytes) Allow for increased fault data

	#define SUN_Dat_Page 0					// 4 bytes per call count per floor	per 15 min period (4 * 24) = 96 periods
	#define SUN_Dat_Offset 0x4000			// for 32 floors = 4 * 97 * 30 = 11640	  (4000h = 16384 bytes)

	#define MON_Dat_Page 0					// 16384 / (4 * 97) = 42 floors max
	#define MON_Dat_Offset 0x8000		

	#define TUE_Dat_Page 0
	#define TUE_Dat_Offset 0xC000

	#define WED_Dat_Page 1			
	#define WED_Dat_Offset 0		

	#define THR_Dat_Page 1
	#define THR_Dat_Offset 0x4000

	#define FRI_Dat_Page 1			
	#define FRI_Dat_Offset 0x8000		

	#define SAT_Dat_Page 1
	#define SAT_Dat_Offset 0xC000

   //Security Floor Code  
	
	#define Sec_code_page1 2
	#define Sec_page_1_offset 0

	#define FL1_Code_Page Sec_code_page1
	#define FL1_CodeTbl_OffSet Sec_page_1_offset
	#define FL1_Code_Offset FL1_CodeTbl_OffSet+6		  

    #define FL2_Code_Page Sec_code_page1
	#define FL2_CodeTbl_OffSet FL1_CodeTbl_OffSet+Fl_Code_Length
	#define FL2_Code_Offset FL2_CodeTbl_OffSet+6		  

    #define FL3_Code_Page Sec_code_page1
	#define FL3_CodeTbl_OffSet FL2_CodeTbl_OffSet+Fl_Code_Length
	#define FL3_Code_Offset FL3_CodeTbl_OffSet+6         

	#define FL4_Code_Page Sec_code_page1
	#define FL4_CodeTbl_OffSet FL3_CodeTbl_OffSet+Fl_Code_Length
	#define FL4_Code_Offset FL4_CodeTbl_OffSet+6         

    #define FL5_Code_Page Sec_code_page1
	#define FL5_CodeTbl_OffSet FL4_CodeTbl_OffSet+Fl_Code_Length
	#define FL5_Code_Offset FL5_CodeTbl_OffSet+6         

    #define FL6_Code_Page Sec_code_page1
	#define FL6_CodeTbl_OffSet FL5_CodeTbl_OffSet+Fl_Code_Length
	#define FL6_Code_Offset FL6_CodeTbl_OffSet+6         

	#define FL7_Code_Page Sec_code_page1
	#define FL7_CodeTbl_OffSet FL6_CodeTbl_OffSet+Fl_Code_Length
	#define FL7_Code_Offset FL7_CodeTbl_OffSet+6         

    #define FL8_Code_Page Sec_code_page1
	#define FL8_CodeTbl_OffSet FL7_CodeTbl_OffSet+Fl_Code_Length
	#define FL8_Code_Offset FL8_CodeTbl_OffSet+6         

    #define FL9_Code_Page Sec_code_page1							 
	#define FL9_CodeTbl_OffSet FL8_CodeTbl_OffSet+Fl_Code_Length
	#define FL9_Code_Offset FL9_CodeTbl_OffSet+6         

	#define FL10_Code_Page Sec_code_page1
	#define FL10_CodeTbl_OffSet FL9_CodeTbl_OffSet+Fl_Code_Length
	#define FL10_Code_Offset FL10_CodeTbl_OffSet+6         

    #define FL11_Code_Page Sec_code_page1
	#define FL11_CodeTbl_OffSet FL10_CodeTbl_OffSet+Fl_Code_Length
	#define FL11_Code_Offset FL11_CodeTbl_OffSet+6         

    #define FL12_Code_Page Sec_code_page1
	#define FL12_CodeTbl_OffSet FL11_CodeTbl_OffSet+Fl_Code_Length
	#define FL12_Code_Offset FL12_CodeTbl_OffSet+6         

	#define FL13_Code_Page Sec_code_page1
	#define FL13_CodeTbl_OffSet FL12_CodeTbl_OffSet+Fl_Code_Length
	#define FL13_Code_Offset FL13_CodeTbl_OffSet+6         

    #define FL14_Code_Page Sec_code_page1
	#define FL14_CodeTbl_OffSet FL13_CodeTbl_OffSet+Fl_Code_Length
	#define FL14_Code_Offset FL14_CodeTbl_OffSet+6         

    #define FL15_Code_Page Sec_code_page1
	#define FL15_CodeTbl_OffSet FL14_CodeTbl_OffSet+Fl_Code_Length
	#define FL15_Code_Offset FL15_CodeTbl_OffSet+6         

	#define FL16_Code_Page Sec_code_page1
	#define FL16_CodeTbl_OffSet FL15_CodeTbl_OffSet+Fl_Code_Length
	#define FL16_Code_Offset FL16_CodeTbl_OffSet+6         

    #define FL17_Code_Page Sec_code_page1
	#define FL17_CodeTbl_OffSet FL16_CodeTbl_OffSet+Fl_Code_Length
	#define FL17_Code_Offset FL17_CodeTbl_OffSet+6         

    #define FL18_Code_Page Sec_code_page1
	#define FL18_CodeTbl_OffSet FL17_CodeTbl_OffSet+Fl_Code_Length
	#define FL18_Code_Offset FL18_CodeTbl_OffSet+6         

	#define FL19_Code_Page Sec_code_page1
	#define FL19_CodeTbl_OffSet FL18_CodeTbl_OffSet+Fl_Code_Length
	#define FL19_Code_Offset FL19_CodeTbl_OffSet+6         

    #define FL20_Code_Page Sec_code_page1
	#define FL20_CodeTbl_OffSet FL19_CodeTbl_OffSet+Fl_Code_Length
	#define FL20_Code_Offset FL20_CodeTbl_OffSet+6         

    #define FL21_Code_Page Sec_code_page1
	#define FL21_CodeTbl_OffSet FL20_CodeTbl_OffSet+Fl_Code_Length
	#define FL21_Code_Offset FL21_CodeTbl_OffSet+6         

	#define FL22_Code_Page Sec_code_page1
	#define FL22_CodeTbl_OffSet FL21_CodeTbl_OffSet+Fl_Code_Length
	#define FL22_Code_Offset FL22_CodeTbl_OffSet+6         

    #define FL23_Code_Page Sec_code_page1
	#define FL23_CodeTbl_OffSet FL22_CodeTbl_OffSet+Fl_Code_Length
	#define FL23_Code_Offset FL23_CodeTbl_OffSet+6         

    #define FL24_Code_Page Sec_code_page1
	#define FL24_CodeTbl_OffSet FL23_CodeTbl_OffSet+Fl_Code_Length
	#define FL24_Code_Offset FL24_CodeTbl_OffSet+6         

	#define FL25_Code_Page Sec_code_page1
	#define FL25_CodeTbl_OffSet FL24_CodeTbl_OffSet+Fl_Code_Length
	#define FL25_Code_Offset FL25_CodeTbl_OffSet+6         

    #define FL26_Code_Page Sec_code_page1
	#define FL26_CodeTbl_OffSet FL25_CodeTbl_OffSet+Fl_Code_Length
	#define FL26_Code_Offset FL26_CodeTbl_OffSet+6         

    #define FL27_Code_Page Sec_code_page1
	#define FL27_CodeTbl_OffSet FL26_CodeTbl_OffSet+Fl_Code_Length
	#define FL27_Code_Offset FL27_CodeTbl_OffSet+6         

	#define FL28_Code_Page Sec_code_page1
	#define FL28_CodeTbl_OffSet FL27_CodeTbl_OffSet+Fl_Code_Length
	#define FL28_Code_Offset FL28_CodeTbl_OffSet+6         

    #define FL29_Code_Page Sec_code_page1
	#define FL29_CodeTbl_OffSet FL28_CodeTbl_OffSet+Fl_Code_Length
	#define FL29_Code_Offset FL29_CodeTbl_OffSet+6         

    #define FL30_Code_Page Sec_code_page1
	#define FL30_CodeTbl_OffSet FL29_CodeTbl_OffSet+Fl_Code_Length
	#define FL30_Code_Offset FL30_CodeTbl_OffSet+6         


	//Code Lookup Table	
	#define Code_LookUpTbl_Page 3
	#define Code_LookUpTbl_Offset 0
	#define Fl_LookUpTbl_Length 60

	#define Code_LookUpTbl_Size 1800    // 30*2*30 


    #define FL1_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL1_LookUpTbl_OffSet Code_LookUpTbl_Offset
	
	#define FL2_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL2_LookUpTbl_OffSet FL1_LookUpTbl_OffSet+Fl_LookUpTbl_Length	 // 30 * 2 60 bytes

	#define FL3_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL3_LookUpTbl_OffSet FL2_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL4_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL4_LookUpTbl_OffSet FL3_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL5_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL5_LookUpTbl_OffSet FL4_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL6_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL6_LookUpTbl_OffSet FL5_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL7_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL7_LookUpTbl_OffSet FL6_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL8_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL8_LookUpTbl_OffSet FL7_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL9_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL9_LookUpTbl_OffSet FL8_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL10_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL10_LookUpTbl_OffSet FL9_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL11_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL11_LookUpTbl_OffSet FL10_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL12_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL12_LookUpTbl_OffSet FL11_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL13_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL13_LookUpTbl_OffSet FL12_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL14_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL14_LookUpTbl_OffSet FL13_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL15_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL15_LookUpTbl_OffSet FL14_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL16_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL16_LookUpTbl_OffSet FL15_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL17_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL17_LookUpTbl_OffSet FL16_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL18_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL18_LookUpTbl_OffSet FL17_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL19_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL19_LookUpTbl_OffSet FL18_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL20_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL20_LookUpTbl_OffSet FL19_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL21_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL21_LookUpTbl_OffSet FL20_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL22_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL22_LookUpTbl_OffSet FL21_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL23_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL23_LookUpTbl_OffSet FL22_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL24_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL24_LookUpTbl_OffSet FL23_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL25_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL25_LookUpTbl_OffSet FL24_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL26_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL26_LookUpTbl_OffSet FL25_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL27_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL27_LookUpTbl_OffSet FL26_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL28_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL28_LookUpTbl_OffSet FL27_LookUpTbl_OffSet+Fl_LookUpTbl_Length
	
	#define FL29_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL29_LookUpTbl_OffSet FL28_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL30_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL30_LookUpTbl_OffSet FL29_LookUpTbl_OffSet+Fl_LookUpTbl_Length

	#define FL31_LookUpTbl_Page Code_LookUpTbl_Page
	#define FL31_LookUpTbl_OffSet FL30_LookUpTbl_OffSet+Fl_LookUpTbl_Length

#endif


#define Encoder_Command 0x216   // Encoder Command Register Address
#define Encoder_Enable  0x214   // Encoder Enable Register Address
#define Encoder_Status  0x214	// Encoder Status register
#define Encoder_Base    0x210   // Encoder Base Address
#define Encoder_Busy    0x80    // Busy Bit in Command Register
#define Encoder_CLR     0x88    // Clear Counter #1
#define Encoder_CLR2    0x89    // Clear Counter #2
#define Encoder_CLR3    0x8a    // Clear Counter #3
#define Encoder_CLR4    0x8b    // Clear Counter #4
#define Encoder_Preset	0x8c	// Encoder count preset
#define Encoder_CLRa    0x90    // Clear Counter All Counters
#define Encoder_RD1     0x84    // Read Encoder Counter #1
#define Encoder_RD2     0x85    // Read Encoder Counter #2
#define Encoder_RD3     0x86    // Read Encoder Counter #3
#define Encoder_RD4     0x87    // Read Encoder Counter #4
#define Encoder_RDCAN   0x87    // Read Encoder Counter CAN
#define Encoder_EN1     0x01    // Encoder 1 Enable Enable Register
#define Encoder_EN2     0x02    // Encoder 2 Enable Enable Register
#define Encoder_EN3     0x03    // Encoder 3 Enable Enable Register
#define Encoder_EN4     0x04    // Encoder 4 Enable Enable Register
#define Encoder_CAN		0x04	// Encoder CAN enable register

#define B1004 0X090     // 100 A CH 0 B CL     IN=1 OUT=0
#define B1005 0X099     // 8255 CONTROL DATA
#define B1007 0X099     // A=APORT CH=CPORT HIGH NIB B=BPORT CL=CPORT LOW NIB
*/

// ON 1102 BOARD													

#define BRD1A 0x100     // Board #1 A port address
#define BRD1B 0x101     // Board #1 B port address
#define BRD1C 0x102     // Board #1 C port address
#define BRD1  1	    	// Board #1 control port address

#define BRD2A 0x103     // Board #2 A port address
#define BRD2B 0x104     // Board #2 B port address
#define BRD2C 0x105     // Board #2 C port address
#define BRD2  2		    // Board #2 control port address

#define BRD3A 0x106     // Board #3 A port address
#define BRD3B 0x107     // Board #3 B port address
#define BRD3C 0x108     // Board #3 C port address
#define BRD3  3		   	// Board #3 control port address

#define BRD4A 0x109     // Board #4 A port address
#define BRD4B 0x10A     // Board #4 B port address
#define BRD4C 0x10B     // Board #4 C port address
#define BRD4  4			 // Board #4 control port address

#define BRD5A 0x10C     // Board #5 A port address
#define BRD5B 0x10D     // Board #5 B port address
#define BRD5C 0x10E     // Board #5 C port address
#define BRD5  5     	// Board #5 control port address

#define BRD6A 0x10F     // Board #6 A port address
#define BRD6B 0x110     // Board #6 B port address
#define BRD6C c_misc_io_addr     // Board #6 C port address		-- address used for misc. i/o (brake board).
#define BRD6  6     	// Board #6 control port address



//************************************************************************
//*********** Serial Car conn board address  *****************************
//************************************************************************
				   // Note: board # such as BRD9A 6, is the index for serial.brd_io[6]
				   // THE i/o array index is set in the individual i/o declaration such as TFD 24.
				   // SO in read i/o iodata[1][TFD] = serial.brd_io[BRD9A] and the bit number
				   // is declared as TFB BIT1. 

//***************************
// Machine Room Can Port
//***************************

// Serial EXPANSION IO BOARD #7

#define BRD7A 0    // Board #7C A port address
#define BRD7B 1    // Board #7C B port address
#define BRD7C 2    // Board #7C C port address
#define BRD7  7    // Board #7C control port address

// Serial EXPANSION IO BOARD #8

#define BRD8A 3    // Board #8C A port address
#define BRD8B 4    // Board #8C B port address
#define BRD8C 5    // Board #8C C port address
#define BRD8  8    // Board #8C control port address

// Serial EXPANSION IO BOARD #9

#define BRD9A 6    // Board #9C A port address
#define BRD9B 7    // Board #9C B port address
#define BRD9C 8    // Board #9C C port address
#define BRD9  9    // Board #9C control port address

// Serial EXPANSION IO BOARD #10

#define BRD10A 9    // Board #10C A port address
#define BRD10B 10   // Board #10C B port address
#define BRD10C 11   // Board #10C C port address
#define BRD10  9    // Board #10C control port address

// Serial EXPANSION IO BOARD #11

#define BRD11A 12   // Board #11C A port address
#define BRD11B 13   // Board #11C B port address
#define BRD11C 14   // Board #11C C port address
#define BRD11  11   // Board #11C control port address
							   
// Serial EXPANSION IO BOARD #12

#define BRD12A 15   // Board #12C A port address
#define BRD12B 16   // Board #12C B port address
#define BRD12C 17   // Board #12C C port address
#define BRD12  12   // Board #12C control port address

// Serial EXPANSION IO BOARD #13

#define BRD13A 18   // Board #13C A port address
#define BRD13B 19   // Board #13C B port address
#define BRD13C 20   // Board #13C C port address
#define BRD13  13   // Board #13C control port address

// Serial EXPANSION IO BOARD #14

#define BRD14A 21   // Board #14C A port address
#define BRD14B 22   // Board #14C B port address
#define BRD14C 23   // Board #14C C port address
#define BRD14  14   // Board #14C control port address

// Serial EXPANSION IO BOARD #15

#define BRD15A 24    // Board #15C A port address
#define BRD15B 25    // Board #15C B port address
#define BRD15C 26    // Board #15C C port address
#define BRD15  15    // Board #15C control port address

// Serial EXPANSION IO BOARD #16

#define BRD16A 27    // Board #16C A port address
#define BRD16B 28    // Board #16C B port address
#define BRD16C 29    // Board #16C C port address
#define BRD16  16    // Board #16C control port address

// Serial EXPANSION IO BOARD #17

#define BRD17A 30    // Board #17C A port address
#define BRD17B 31    // Board #17C B port address
#define BRD17C 32    // Board #17C C port address
#define BRD17  17    // Board #17C control port address

// Serial EXPANSION IO BOARD #18

#define BRD18A 33    // Board #18C A port address
#define BRD18B 34    // Board #18C B port address
#define BRD18C 35    // Board #18C C port address
#define BRD18  18    // Board #18C control port address

// Serial EXPANSION IO BOARD #19

#define BRD19A 36    // Board #19C A port address
#define BRD19B 37    // Board #19C B port address
#define BRD19C 38    // Board #19C C port address
#define BRD19  19    // Board #19C control port address

// Serial EXPANSION IO BOARD #20

#define BRD20A 39    // Board #20C A port address
#define BRD20B 40    // Board #20C B port address
#define BRD20C 41    // Board #20C C port address
#define BRD20  20    // Board #20C control port address

// Serial EXPANSION IO BOARD #21

#define BRD21A 42    // Board #21C A port address
#define BRD21B 43    // Board #21C B port address
#define BRD21C 44    // Board #21C C port address
#define BRD21  21    // Board #21C control port address

// Serial EXPANSION IO BOARD #22

#define BRD22A 45    // Board #22C A port address
#define BRD22B 46    // Board #22C B port address
#define BRD22C 47    // Board #22C C port address
#define BRD22  22    // Board #22C control port address

// Serial EXPANSION IO BOARD #23

#define BRD23A 48    // Board #23C A port address  
#define BRD23B 49    // Board #23C B port address	
#define BRD23C 50    // Board #23C C port address	
#define BRD23  23    // Board #23C control port address

// Serial EXPANSION IO BOARD #24

#define BRD24A 51    // Board #24C A port address	
#define BRD24B 52    // Board #24C B port address	
#define BRD24C 53    // Board #24C C port address	
#define BRD24  24    // Board #24C control port address

// Serial EXPANSION IO BOARD #25

#define BRD25A 54    // Board #25C A port address	
#define BRD25B 55    // Board #25C B port address
#define BRD25C 56    // Board #25C C port address
#define BRD25  25    // Board #25C control port address

// Serial EXPANSION IO BOARD #26

#define BRD26A 57    // Board #25C A port address
#define BRD26B 58    // Board #25C B port address
#define BRD26C 59    // Board #25C C port address
#define BRD26  26    // Board #25C control port address

// Serial EXPANSION IO BOARD #27

#define BRD27A 60	// Board #27 A port address
#define BRD27B 61   // Board #27 B port address
#define BRD27C 62   // Board #27 C port address
#define BRD27  27   // Board #27 control port address

// Serial EXPANSION IO BOARD #28

#define BRD28A 63   // Board #28C A port address
#define BRD28B 64   // Board #28C B port address
#define BRD28C 65   // Board #28C C port address
#define BRD28  28   // Board #28C control port address

// Serial EXPANSION IO BOARD #29

#define BRD29A 66    // Board #29C A port address
#define BRD29B 67    // Board #29C B port address
#define BRD29C 68    // Board #29C C port address
#define BRD29  29    // Board #29C control port address

// Serial EXPANSION IO BOARD #30

#define BRD30A 69    // Board #30C A port address
#define BRD30B 70    // Board #30C B port address
#define BRD30C 71    // Board #30C C port address
#define BRD30  30    // Board #30C control port address

// Serial EXPANSION IO BOARD #31

#define BRD31A 72    // Board #31C A port address
#define BRD31B 73    // Board #31C B port address
#define BRD31C 74    // Board #31C C port address
#define BRD31  31    // Board #31C control port address

// Serial EXPANSION IO BOARD #32

#define BRD32A 75    // Board #32C A port address
#define BRD32B 76    // Board #32C B port address
#define BRD32C 77    // Board #32C C port address
#define BRD32  32    // Board #32C control port address

// Serial EXPANSION IO BOARD #33

#define BRD33A 78    // Board #33C A port address
#define BRD33B 79    // Board #33C B port address
#define BRD33C 80    // Board #33C C port address
#define BRD33  33    // Board #33C control port address

// Serial EXPANSION IO BOARD #34

#define BRD34A 81    // Board #34C A port address
#define BRD34B 82    // Board #34C B port address
#define BRD34C 83    // Board #34C C port address
#define BRD34  34    // Board #34C control port address

// Serial EXPANSION IO BOARD #35

#define BRD35A 84    // Board #35C A port address
#define BRD35B 85    // Board #35C B port address
#define BRD35C 86    // Board #35C C port address
#define BRD35  35    // Board #35C control port address

// Serial EXPANSION IO BOARD #36

#define BRD36A 87    // Board #36C A port address
#define BRD36B 88    // Board #36C B port address
#define BRD36C 89    // Board #36C C port address
#define BRD36  36    // Board #36C control port address

// Serial EXPANSION IO BOARD #37

#define BRD37A 90    // Board #37C A port address
#define BRD37B 91    // Board #37C B port address
#define BRD37C 92    // Board #37C C port address
#define BRD37  37    // Board #37C control port address

// Serial EXPANSION IO BOARD #38

#define BRD38A 93    // Board #38C A port address
#define BRD38B 94    // Board #38C B port address
#define BRD38C 95    // Board #38C C port address
#define BRD38  38    // Board #38C control port address

// Serial EXPANSION IO BOARD #39

#define BRD39A 96    // Board #39C A port address
#define BRD39B 97    // Board #39C B port address
#define BRD39C 98    // Board #39C C port address
#define BRD39  39    // Board #39C control port address

// Serial EXPANSION IO BOARD #40

#define BRD40A 99    // Board #40C A port address
#define BRD40B 100   // Board #40C B port address
#define BRD40C 101   // Board #40C C port address
#define BRD40  40    // Board #40C control port address

// Serial EXPANSION IO BOARD #41

#define BRD41A 102   // Board #41C A port address
#define BRD41B 103   // Board #41C B port address
#define BRD41C 104   // Board #41C C port address
#define BRD41  41    // Board #41C control port address

// Serial EXPANSION IO BOARD #42

#define BRD42A 105   // Board #42C A port address
#define BRD42B 106   // Board #42C B port address
#define BRD42C 107   // Board #42C port address
#define BRD42  42    // Board #42C control port address

// Serial EXPANSION IO BOARD #43

#define BRD43A 108   // Board #43C A port address
#define BRD43B 109   // Board #43C B port address
#define BRD43C 110   // Board #43C C port address
#define BRD43  43    // Board #43C control port address

// Serial EXPANSION IO BOARD #44

#define BRD44A 111   // Board #44C A port address
#define BRD44B 112   // Board #44C B port address
#define BRD44C 113   // Board #44C port address
#define BRD44  44    // Board #44C control port address

// Serial EXPANSION IO BOARD #45

#define BRD45A 114   // Board #45C A port address
#define BRD45B 115   // Board #45C B port address
#define BRD45C 116   // Board #45C C port address
#define BRD45  45    // Board #45C control port address

// Serial EXPANSION IO BOARD #46

#define BRD46A 117   // Board #46C A port address
#define BRD46B 118   // Board #46C B port address
#define BRD46C 119   // Board #46C port address
#define BRD46  46    // Board #46C control port address



//***************************
// Car Top Can Port
//***************************

// Serial EXPANSION IO BOARD #47

#define BRD47A 0   	// Board #47C A port address
#define BRD47B 1   	// Board #47C B port address
#define BRD47C 2   	// Board #47C C port address
#define BRD47  47  	// Board #47C control port address

// Serial EXPANSION IO BOARD #48

#define BRD48A 3   	// Board #48C A port address
#define BRD48B 4   	// Board #48C B port address
#define BRD48C 5   	// Board #48C port address
#define BRD48  48  	// Board #48C control port address

// Serial EXPANSION IO BOARD #49

#define BRD49A 6   	// Board #49C A port address
#define BRD49B 7   	// Board #49C B port address
#define BRD49C 8   	// Board #49C C port address
#define BRD49  49   // Board #49 control port address

// Serial EXPANSION IO BOARD #50

#define BRD50A 9   	// Board #50 A port address
#define BRD50B 10   // Board #50 B port address
#define BRD50C 11   // Board #50 C port address
#define BRD50  50   // Board #50 control port address

// Serial EXPANSION IO BOARD #51

#define BRD51A 12   // Board #51 A port address
#define BRD51B 13   // Board #51 B port address
#define BRD51C 14   // Board #51 C port address
#define BRD51  51   // Board #51 control port address

// Serial EXPANSION IO BOARD #52

#define BRD52A 15   // Board #52 A port address
#define BRD52B 16   // Board #52 B port address
#define BRD52C 17   // Board #52 C port address
#define BRD52  52   // Board #52 control port address

// Serial EXPANSION IO BOARD #53

#define BRD53A 18   // Board #53 A port address
#define BRD53B 19   // Board #53 B port address
#define BRD53C 20   // Board #53 C port address
#define BRD53  53   // Board #53 control port address

// Serial EXPANSION IO BOARD #54

#define BRD54A 21   // Board #54 A port address
#define BRD54B 22   // Board #54 B port address
#define BRD54C 23   // Board #54 C port address
#define BRD54  54   // Board #54 control port address

// Serial EXPANSION IO BOARD #55

#define BRD55A 24   // Board #55 A port address
#define BRD55B 25   // Board #55 B port address
#define BRD55C 26   // Board #55 C port address
#define BRD55  55   // Board #55 control port address

// Serial EXPANSION IO BOARD #56

#define BRD56A 27   // Board #56 A port address
#define BRD56B 28   // Board #56 B port address
#define BRD56C 29   // Board #56 C port address
#define BRD56  56   // Board #56 control port address

// Serial EXPANSION IO BOARD #57

#define BRD57A 30   // Board #57 A port address
#define BRD57B 31   // Board #57 B port address
#define BRD57C 32   // Board #57 C port address
#define BRD57  57   // Board #57 control port address

// Serial EXPANSION IO BOARD #58

#define BRD58A 33   // Board #58 A port address
#define BRD58B 34   // Board #58 B port address
#define BRD58C 35   // Board #58 C port address
#define BRD58  58   // Board #58 control port address

// Serial EXPANSION IO BOARD #59

#define BRD59A 36   // Board #59 A port address
#define BRD59B 37   // Board #59 B port address
#define BRD59C 38   // Board #59 C port address
#define BRD59  59   // Board #59 control port address

// Serial EXPANSION IO BOARD #60

#define BRD60A 39   // Board #60 A port address
#define BRD60B 40   // Board #60 B port address
#define BRD60C 41   // Board #60 C port address
#define BRD60  60   // Board #60 control port address

// Serial EXPANSION IO BOARD #61

#define BRD61A 42   // Board #61 A port address
#define BRD61B 43   // Board #61 B port address
#define BRD61C 44   // Board #61 C port address
#define BRD61  61   // Board #61 control port address

// Serial EXPANSION IO BOARD #62

#define BRD62A 45   // Board #62 A port address
#define BRD62B 46   // Board #62 B port address
#define BRD62C 47   // Board #62 C port address
#define BRD62  62   // Board #62 control port address

// Serial EXPANSION IO BOARD #63

#define BRD63A 48   // Board #63 A port address
#define BRD63B 49   // Board #63 B port address
#define BRD63C 50   // Board #63 C port address
#define BRD63  63   // Board #63 control port address

// Serial EXPANSION IO BOARD #64

#define BRD64A 51   // Board #64 A port address
#define BRD64B 52   // Board #64 B port address
#define BRD64C 53   // Board #64 C port address
#define BRD64  64   // Board #64 control port address

// Serial EXPANSION IO BOARD #65

#define BRD65A 54   // Board #65 A port address
#define BRD65B 55   // Board #65 B port address
#define BRD65C 56   // Board #65 C port address
#define BRD65  65   // Board #65 control port address

// Serial EXPANSION IO BOARD #66

#define BRD66A 57   // Board #66 A port address
#define BRD66B 58   // Board #66 B port address
#define BRD66C 59   // Board #66 C port address
#define BRD66  66   // Board #66 control port address

// Serial EXPANSION IO BOARD #67

#define BRD67A 60   // Board #67 A port address
#define BRD67B 61   // Board #67 B port address
#define BRD67C 62   // Board #67 C port address
#define BRD67  67   // Board #67 control port address

// Serial EXPANSION IO BOARD #68

#define BRD68A 63   // Board #68 A port address
#define BRD68B 64   // Board #68 B port address
#define BRD68C 65   // Board #68 C port address
#define BRD68  68   // Board #68 control port address

// Serial EXPANSION IO BOARD #69

#define BRD69A 66   // Board #69 A port address
#define BRD69B 67   // Board #69 B port address
#define BRD69C 68   // Board #69 C port address
#define BRD69  69   // Board #69 control port address

// Serial EXPANSION IO BOARD #70

#define BRD70A 69   // Board #70 A port address
#define BRD70B 70   // Board #70 B port address
#define BRD70C 71   // Board #70 C port address
#define BRD70  70   // Board #70 control port address

// Serial EXPANSION IO BOARD #71

#define BRD71A 72   // Board #71 A port address
#define BRD71B 73   // Board #71 B port address
#define BRD71C 74   // Board #71 C port address
#define BRD71  71   // Board #71 control port address

// Serial EXPANSION IO BOARD #72

#define BRD72A 75   // Board #72 A port address
#define BRD72B 76   // Board #72 B port address
#define BRD72C 77   // Board #72 C port address
#define BRD72  72   // Board #72 control port address

// Serial EXPANSION IO BOARD #73

#define BRD73A 78   // Board #73 A port address
#define BRD73B 79   // Board #73 B port address
#define BRD73C 80   // Board #73 C port address
#define BRD73  73   // Board #73 control port address

// Serial EXPANSION IO BOARD #74

#define BRD74A 81   // Board #74 A port address
#define BRD74B 82   // Board #74 B port address
#define BRD74C 83   // Board #74 C port address
#define BRD74  74   // Board #74 control port address

// Serial EXPANSION IO BOARD #75

#define BRD75A 84   // Board #75 A port address
#define BRD75B 85   // Board #75 B port address
#define BRD75C 86   // Board #75 C port address
#define BRD75  75   // Board #75 control port address

// Serial EXPANSION IO BOARD #76

#define BRD76A 87   // Board #76 A port address
#define BRD76B 88   // Board #76 B port address
#define BRD76C 89   // Board #76 C port address
#define BRD76  76   // Board #76 control port address

// Serial EXPANSION IO BOARD #77

#define BRD77A 90   // Board #77 A port address
#define BRD77B 91   // Board #77 B port address
#define BRD77C 92   // Board #77 C port address
#define BRD77  77   // Board #77 control port address

// Serial EXPANSION IO BOARD #78

#define BRD78A 93   // Board #78 A port address
#define BRD78B 94   // Board #78 B port address
#define BRD78C 95   // Board #78 C port address
#define BRD78  78   // Board #78 control port address

// Serial EXPANSION IO BOARD #79

#define BRD79A 96   // Board #79 A port address
#define BRD79B 97   // Board #79 B port address
#define BRD79C 98   // Board #79 C port address
#define BRD79  79   // Board #79 control port address

// Serial EXPANSION IO BOARD #80

#define BRD80A 99   // Board #80 A port address
#define BRD80B 100  // Board #80 B port address
#define BRD80C 101  // Board #80 C port address
#define BRD80  80   // Board #80 control port address

// Serial EXPANSION IO BOARD #81

#define BRD81A 102  // Board #81 A port address
#define BRD81B 103  // Board #81 B port address
#define BRD81C 104  // Board #81 C port address
#define BRD81  81   // Board #81 control port address

// Serial EXPANSION IO BOARD #82

#define BRD82A 105  // Board #82 A port address
#define BRD82B 106  // Board #82 B port address
#define BRD82C 107  // Board #82 C port address
#define BRD82  82   // Board #82 control port address

// Serial EXPANSION IO BOARD #83

#define BRD83A 108  // Board #83 A port address
#define BRD83B 109  // Board #83 B port address
#define BRD83C 110  // Board #83 C port address
#define BRD83  83   // Board #83 control port address

// Serial EXPANSION IO BOARD #84

#define BRD84A 111  // Board #84 A port address
#define BRD84B 112  // Board #84 B port address
#define BRD84C 113  // Board #84 C port address
#define BRD84  84   // Board #84 control port address

// Serial EXPANSION IO BOARD #85

#define BRD85A 114  // Board #85 A port address
#define BRD85B 115  // Board #85 B port address
#define BRD85C 116  // Board #85 C port address
#define BRD85  85   // Board #85 control port address

// Serial EXPANSION IO BOARD #86

#define BRD86A 117  // Board #86 A port address
#define BRD86B 118  // Board #86 B port address
#define BRD86C 119  // Board #86 C port address
#define BRD86  86   // Board #86 control port address


/*

#if (Tract_HR == 1)
	#define max_io 1248		// 52 * 3 * 8 = 1152
#else
	#define max_io 768		// 32 * 3 * 8 = 672
#endif
*/

/* Translation table from board number to array number

	Board #			Port A		Port B		Port C
 1038	1				0 	  		1 	 		2 	
		2				3 	  		4 	 		5 	
		3				6 			7 	 		8 	
		4				9 			10 			11 	
		5				12			13			14	
     	6	    		15			16			17	

 1029	7		1		18			19			20	
 		8	  	 		21			22			23	
 1029	9		2		24			25			26	
 		10	   			27			28			29	
 1029	11		3		30			31			32	
 		12	  			33			34			35	
 1029	13		4		36			37			38	
 		14	  			39			40			41	
 1029	15		5		42			43			44	
 		16	  			45			46			47	
 1029	17		6		48			49			50	
 		18	  			51			52			53	
 1029	19		7		54			55			56	
 		20	  			57			58			59	
 1029	21		8		60			61			62	
 		22	  			63			64			65	

 1037/	23		9		66			67			68	
 1040	24	  			69			70			71	
 		25				72			73			74	

S-Curve	26	  			75  		76  		77  
		27				78  		79  		80  

LCD 	28				81	I		82	O		83	O  (not used)
		
1029	29	   11		84			85			86
    	30				87			88			89
1029	31	   12		90			91			92
    	32				93			94			95
1029	33	   13		96			97			98
    	34			    99 		   100		   101
1029	35	   14	   102		   103		   104 
        36			   105		   106		   107
1029	37	   15	   108		   109		   110 
        38			   111		   112		   113
1029	39	   16	   114		   115		   116 
        40			   117		   118		   119
1029	41	   17	   120		   121		   122 
        42			   123		   124		   125
1029	43	   18	   126		   127		   128 
        44			   129		   130		   131
1029	45	   19	   132		   133		   134 
        46			   135		   136		   137
1029	47	   20	   138		   139		   140 
        48			   141		   142		   143
	
*/




/* Revision History

  6/7/01  1.04  mhd 	1. Moved hoistway data from page 0 offset 1024 to page 5 offset 0.  
  						   Also moved flight time data from page 0 offset 1536 to page 6 
  						   offset 0.
  6/29/01 1.05 mhd		1. Swapped location of FF with RST.
  06/21/02 2.03 mhd		1. Added CCS car call security i/o table.
 2/11/03 3.02 MHD		1. Moved ATT, ATTUP, ATTDN from 10C to 8C.
					  	2. Added ATTBY (Attendant Bypass) and mapped it to FS2C input.
 12/1/03 3.14 mhd		1. Moved call data memory location to allow for 29 floors see  MON_Dat_Page, WED_Dat_Page and FRI_Dat_Page.
 12/04/03 				2. Added discrete Pi's and direction lanterns on serial cop board for hyro cars.
 3/3/04 4.00 mhd		1. Modified i/o structure for serial comm to Top of Car board.
 3/1/05 4.07 MHD		1. Moved DOLR, DCLR, CULR and CDLR.
 4/23/05 4.08 mhd		1. Fixed block25. 25A was using 73 and 25b was using 72.  Swapped them.
 5/18/09 V5.48.6 mhd	1. Corrected board location for EBB2 AND EBB1 from BRD28B to BRD28A.
						2. Added output for power loss brake lift, MB1, MB2, MBP, and MBC.
 6/23/10 v5.52.11 mhd	1. Corrected board array locations for boards 23 and 24 (address 13 and 14).
*/
