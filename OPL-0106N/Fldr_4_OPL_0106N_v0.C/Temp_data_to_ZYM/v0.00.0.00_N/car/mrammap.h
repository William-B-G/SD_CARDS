// MRAM MEORY MAP 07/18/11 REVISION 1.0.0

// The MRAM memory is 1,048,576 X 16 bits.  1048576/65535 = 16 pages
// We have a total of 16 pages available

// Security Variables
#define MRAM_NumMaxCd 200  //  Number of maxium Code per Floor
#define MRAM_Fl_Code_Length ((MRAM_NumMaxCd*4) + 10)	  // +10: 6 for valid, numcode, chksum and 4 for extra zeros


		// MRam Memory Location   - 16 Pages of 64K

#define MRAM_Pwrup_Dat_Page 0
#define MRAM_Pwrup_Dat_Offset 0				// allow 30 bytes

#define MRAM_RTC_Dat_Page 0					// Real time clock Year 100's and update flag
#define MRAM_RTC_Dat_Offset 0x20				

#define MRAM_Fire_Dat_Page 0					// allow more than 24 bytes
#define MRAM_Fire_Dat_Offset 0x100			// 100h = 256 bytes

#define MRAM_FVARS_Dat_Page 0
#define MRAM_FVARS_Dat_Offset 0x200			// 600 * 2 = 1200 bytes + 18 + 16 = 1234	(500h = 1280 bytes)
#define MRAM_Speed_Clamp_Dat_Offset 0x700
#define MRAM_Sec_CC_Dat_Offset 0x740

#define MRAM_User_Dat_Page 0				// User data arrea (Telephone, Password, IP address
#define MRAM_User_Dat_Offset 0x800			// 6 * 19 * 4 = 456 bytes long  (200h = 512)
#define MRAM_PassW_Dat_Offset 0x840

#define MRAM_LW_Dat_Page 0					// load weigh data moved from fvars data
#define MRAM_LW_Dat_Offset 0x1000			// load weigh data per floor 61 * 8 = 488 bytes	 (200h = 512)

#define MRAM_Hoist_Dat_Page 0				
#define MRAM_Hoist_Dat_Offset 0x1200		// 9 limits * 4 * 2 = 72 + (60 * 4) = 312 + flag (2) = 314 for 60 floors

#define MRAM_Fl_Update_Adjust_Page 0		// (4 * 4) = 16 Level distance both up and down and (4 * 8) = 32 Slowdown  
#define MRAM_Fl_Update_Adjust_Offset 0x1400	

#define MRAM_Group_Timer_Page 0				// Group timers for service activations (AFFh = 2815) bytes max of 10 Service timers
#define MRAM_Group_Timer_Offset 0x1600	 	// 10 * 52 bytes per timer = 520 = 208h bytes for all 10 timers (set to 300h)    

#define MRAM_Flts_Dat_Page 0
#define MRAM_Flts_Dat_Offset 0x1900			// 160 * 50  = 8000	 (4000H = 16384 bytes) Allow for increased fault data

#define MRAM_Stats_Dat_Page 0				// Job Statistics (0x5B00 - 0x5900 = 0x200) 512 bytes available
#define MRAM_Stats_Dat_Offset 0x5900				// allow minimum of 82 bytes

//#define MRAM_Sys_Stat_Dat_Page 0			NOT USED
//#define MRAM_Sys_Stat_Dat_Offset 0x5B00		// 200H = 512 bytes
//#define MRAM_Sys_Stat_Size 255				// allow minimum of 2550 bytes	(1000 - 200h = e00h = 3584 

#define MRAM_Flight_Dat_Page 0		 		// ETA flight times 13x13 = 338 (12 floors max) (set to 200h)
#define MRAM_Flight_Dat_Offset 0x5D00	  	// length of 512

#define MRAM_SUN_Dat_Page 1					// 4 bytes per call count per floor	per 15 min period (4 * 24) = 96 periods
#define MRAM_SUN_Dat_Offset 0x0000			// for 13 floors = 4 * 97 * 13 = 5044 = 1384	  (1400h = 5120 bytes)

#define MRAM_MON_Dat_Page 1					// 32767 / (4 * 97) = 84 floors max
#define MRAM_MON_Dat_Offset 0x1400		

#define MRAM_TUE_Dat_Page 1
#define MRAM_TUE_Dat_Offset 0x2800

#define MRAM_WED_Dat_Page 1			
#define MRAM_WED_Dat_Offset 0x3C00		

#define MRAM_THR_Dat_Page 1
#define MRAM_THR_Dat_Offset 0x5000

#define MRAM_FRI_Dat_Page 1			
#define MRAM_FRI_Dat_Offset 0x6400		

#define MRAM_SAT_Dat_Page 1
#define MRAM_SAT_Dat_Offset 0x7800

#define MRAM_FVARS_Backup_Dat_Page 1
#define MRAM_FVARS_Backup_Dat_Offset 0x8C00			// 600 * 2 = 1200 bytes + 18 + 16 = 1234	(500h = 1280 bytes)
#define MRAM_Speed_Clamp_Backup_Dat_Offset 0x9100
#define MRAM_Sec_CC_Backup_Dat_Offset 0x9140

#define MRAM_SPB_FVARS_Dat_Page 1
#define MRAM_SPB_FVARS_Dat_Offset 0x9200			// Allow 128 words			

#define MRAM_NTS_FVARS_Dat_Page 1
#define MRAM_NTS_FVARS_Dat_Offset 0x9280			// Allow 128 words			

#define MRAM_FVARS_Checksum_Page 1
#define MRAM_FVARS_Checksum_Offset 0x9300		// only need 4 bytees

#define MRAM_Trace_Dat_Page 1
#define MRAM_Trace_Dat_Offset 0x9320				// Allow 22 words (40H = 64 words)

#define MRAM_IO_Dat_Page 1
#define MRAM_IO_Dat_Offset 0x9360				// Allow 54 words 18 replacable i/o with 6 bytes each + 1 id word net space is 0x1A0 (40H = 64 words)

#define MRAM_Traffic_Log_Vars_Page 1			// Allow 20 words to save Traffic Log Variables data
#define MRAM_Traffic_Log_Vars_Offset 0x9400



//Security Floor Code  
	
#define MRAM_Sec_code_page1 0
#define MRAM_Sec_page_1_offset 0x5F00					// table size = (200 * 4) + 10 = 810 * 12 = 9720 = 25f8h (set to 2700h)

#define MRAM_FL1_Code_Page MRAM_Sec_code_page1
#define MRAM_FL1_CodeTbl_OffSet MRAM_Sec_page_1_offset
#define MRAM_FL1_Code_Offset MRAM_FL1_CodeTbl_OffSet+6		  

#define MRAM_FL2_Code_Page MRAM_Sec_code_page1
#define MRAM_FL2_CodeTbl_OffSet MRAM_FL1_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL2_Code_Offset MRAM_FL2_CodeTbl_OffSet+6		  

#define MRAM_FL3_Code_Page MRAM_Sec_code_page1
#define MRAM_FL3_CodeTbl_OffSet MRAM_FL2_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL3_Code_Offset MRAM_FL3_CodeTbl_OffSet+6         

#define MRAM_FL4_Code_Page MRAM_Sec_code_page1
#define MRAM_FL4_CodeTbl_OffSet MRAM_FL3_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL4_Code_Offset MRAM_FL4_CodeTbl_OffSet+6         

#define MRAM_FL5_Code_Page MRAM_Sec_code_page1
#define MRAM_FL5_CodeTbl_OffSet MRAM_FL4_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL5_Code_Offset MRAM_FL5_CodeTbl_OffSet+6         

#define MRAM_FL6_Code_Page MRAM_Sec_code_page1
#define MRAM_FL6_CodeTbl_OffSet MRAM_FL5_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL6_Code_Offset MRAM_FL6_CodeTbl_OffSet+6         

#define MRAM_FL7_Code_Page MRAM_Sec_code_page1
#define MRAM_FL7_CodeTbl_OffSet MRAM_FL6_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL7_Code_Offset MRAM_FL7_CodeTbl_OffSet+6         

#define MRAM_FL8_Code_Page MRAM_Sec_code_page1
#define MRAM_FL8_CodeTbl_OffSet MRAM_FL7_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL8_Code_Offset MRAM_FL8_CodeTbl_OffSet+6         

#define MRAM_FL9_Code_Page MRAM_Sec_code_page1							 
#define MRAM_FL9_CodeTbl_OffSet MRAM_FL8_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL9_Code_Offset MRAM_FL9_CodeTbl_OffSet+6         

#define MRAM_FL10_Code_Page MRAM_Sec_code_page1
#define MRAM_FL10_CodeTbl_OffSet MRAM_FL9_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL10_Code_Offset MRAM_FL10_CodeTbl_OffSet+6         

#define MRAM_FL11_Code_Page MRAM_Sec_code_page1
#define MRAM_FL11_CodeTbl_OffSet MRAM_FL10_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL11_Code_Offset MRAM_FL11_CodeTbl_OffSet+6         

#define MRAM_FL12_Code_Page MRAM_Sec_code_page1
#define MRAM_FL12_CodeTbl_OffSet MRAM_FL11_CodeTbl_OffSet+MRAM_Fl_Code_Length
#define MRAM_FL12_Code_Offset MRAM_FL12_CodeTbl_OffSet+6         


	//Code Lookup Table	
#define MRAM_Code_LookUpTbl_Page 0
#define MRAM_Code_LookUpTbl_Offset 0x8600
#define MRAM_FL_LookUpTbl_Length 24

#define MRAM_Code_LookUpTbl_Size 288    // 12*2*12= 288 = 120H (set to 200h)
			
#define MRAM_FL1_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL1_LookUpTbl_OffSet MRAM_Code_LookUpTbl_Offset

#define MRAM_FL2_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL2_LookUpTbl_OffSet MRAM_FL1_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length	 // 12 * 2 = 24 bytes

#define MRAM_FL3_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL3_LookUpTbl_OffSet MRAM_FL2_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length

#define MRAM_FL4_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL4_LookUpTbl_OffSet MRAM_FL3_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length
	
#define MRAM_FL5_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL5_LookUpTbl_OffSet MRAM_FL4_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length

#define MRAM_FL6_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL6_LookUpTbl_OffSet MRAM_FL5_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length

#define MRAM_FL7_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL7_LookUpTbl_OffSet MRAM_FL6_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length
	
#define MRAM_FL8_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL8_LookUpTbl_OffSet MRAM_FL7_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length

#define MRAM_FL9_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL9_LookUpTbl_OffSet MRAM_FL8_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length

#define MRAM_FL10_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL10_LookUpTbl_OffSet MRAM_FL9_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length
	
#define MRAM_FL11_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL11_LookUpTbl_OffSet MRAM_FL10_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length

#define MRAM_FL12_LookUpTbl_Page MRAM_Code_LookUpTbl_Page
#define MRAM_FL12_LookUpTbl_OffSet MRAM_FL11_LookUpTbl_OffSet+MRAM_FL_LookUpTbl_Length


// Fault page to store Long Term faults in a circular buffer
#define max_lt_flt_ix 400
#define MRAM_LT_Flts_Dat_Page 3
#define MRAM_LT_Flts_Dat_Offset 0				// allow 32000 BYTES

#define MRAM_Traffic_Log_Page 4				// Allow entire page
#define MRAM_Traffic_Log_Offset 0



/* Revision History

*/
