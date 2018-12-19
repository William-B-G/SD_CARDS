
#define d_SECURITY 1
#include "global.h"

//************* Security  Structures ******************************//							   
							   

struct CdTable {
	int16 ValidTable;
	int16 NumCode;
	int16 CheckSum;
	unsigned char Codes[NumMaxCd][4];
};

struct CdTable CodeTable; 


// Check sum and count code table to keep tract of changes in the table
struct Cd_Tbl_Chk {
	int16 NumCode;
	int16 CheckSum;
};

struct Cd_Tbl_Chk Code_Tbl_Chk[fl_size];


unsigned char floor_secured[fl_size];
unsigned char rfloor_secured[fl_size];
int16 floor_access_req = 0;
 
extern struct CdTable gc_CodeTable;
extern struct Cd_Tbl_Chk gc_Code_Tbl_Chk[fl_size];
extern struct CdTable Com_CodeTable[2];
extern struct Cd_Tbl_Chk Com_Code_Tbl_Chk[2][fl_size];
	
//************* Security Variables ******************************//

//Variables used only in the LCD interface functions
int16 CodeIndex = 0;	   				    			 
int16 NumCode = 0;
int16 LookUpTable[NumMaxFl];
int16 CodeLoc = 0;
unsigned char CC_Code[4] = {0,0,0,0};
int16 Code_Count=0;		// How many codes have been entered
int16 PB_Pushed=0;		// If a car call is currently pushed
int16 Security_code_flag=0;	// Security flag 0 waiting 1 entering code
				// 2 checking code 3 waiting to have nocalls

uint8 ccpbs_fl_access_req = 0;			// Set for 1 second when a secured call is latched by entering a valid code
uint8 ccpbs_codes[4] = {0,0,0,0};		// Buffer for storing codes entered when a secured call is successfully latched

//Variables used only for the user interface functions

			

void Read_CodeTbl(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[]);	// Read Code table
void Write_CodeTbl(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[]);	  // Write Code Table
//lint -esym(534,Add_One_Code_All_Floors)
int16 Add_One_Code_All_Floors(int16 start_flag, int16 wherefrom, unsigned char Code_Num[]);
//lint -esym(534,Delete_One_Code_All_Floors)
int16 Delete_One_Code_All_Floors(int16 start_flag, int16 wherefrom, unsigned char Code_Num[]);
int16 Clear_All_Codes_All_Floors(int16 start_flag, int16 wherefrom);		// Clear all Codes  from memory
//lint -esym(534,Add_Security_Code)
int16 Add_Security_Code(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[], unsigned char Code_Num[]);
//lint -esym(534,Delete_Security_Code)
int16 Delete_Security_Code(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[], unsigned char Code_Num[]);
void Clear_All_Codes_One_Floor(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[]);
bool Check_Security_Code(unsigned char dig[], struct CdTable *ctp, int16 LkUpTbl[]);
void Sort_CodeTbl(struct CdTable *ctp);
int16 get_FirstCode_Index(unsigned char dig, int16 numcodes, unsigned char codes[][4]);
void cclatch(void);
int16 chk_push_button(void );
int16 Verify_Code_Check_Table (struct Cd_Tbl_Chk *ckp);
int16 Valid_Security_Code (unsigned char Sec_Code[], int16 Floor_Req);
void Init_Code_Check_Table (void);
void Read_LookUpTable_Array(int16 FloorNum, int16 LkUpTbl[]);
void Write_LookUpTable_Array(int16 FloorNum, int16 LkUpTbl[]);
void Modify_LookUpTable_Array(struct CdTable *ctp, int16 LkUpTbl[]);
int16 get_LookUpTbl_index(unsigned char First_Code, int16 FloorNum);
void set_secured_fl(void);


// Floor NV Ram offset and page numbers
#if (Tract_HR == 1)
uint16 MRAM_fl_code_page[NumMaxFl+1] = {MRAM_FL1_Code_Page,MRAM_FL1_Code_Page,MRAM_FL2_Code_Page,MRAM_FL3_Code_Page,MRAM_FL4_Code_Page,MRAM_FL5_Code_Page,MRAM_FL6_Code_Page,MRAM_FL7_Code_Page,MRAM_FL8_Code_Page,MRAM_FL9_Code_Page,MRAM_FL10_Code_Page,
					    MRAM_FL11_Code_Page,MRAM_FL12_Code_Page,MRAM_FL13_Code_Page,MRAM_FL14_Code_Page,MRAM_FL15_Code_Page,MRAM_FL16_Code_Page,MRAM_FL17_Code_Page,MRAM_FL18_Code_Page,MRAM_FL19_Code_Page,MRAM_FL20_Code_Page,
					    MRAM_FL21_Code_Page,MRAM_FL22_Code_Page,MRAM_FL23_Code_Page,MRAM_FL24_Code_Page,MRAM_FL25_Code_Page,MRAM_FL26_Code_Page,MRAM_FL27_Code_Page,MRAM_FL28_Code_Page,MRAM_FL29_Code_Page,MRAM_FL30_Code_Page,
					    MRAM_FL31_Code_Page,MRAM_FL32_Code_Page,MRAM_FL33_Code_Page,MRAM_FL34_Code_Page,MRAM_FL35_Code_Page,MRAM_FL36_Code_Page,MRAM_FL37_Code_Page,MRAM_FL38_Code_Page,MRAM_FL39_Code_Page,MRAM_FL40_Code_Page,
					    MRAM_FL41_Code_Page,MRAM_FL42_Code_Page,MRAM_FL43_Code_Page,MRAM_FL44_Code_Page,MRAM_FL45_Code_Page,MRAM_FL46_Code_Page,MRAM_FL47_Code_Page,MRAM_FL48_Code_Page,MRAM_FL49_Code_Page,MRAM_FL50_Code_Page,
					    MRAM_FL51_Code_Page,MRAM_FL52_Code_Page,MRAM_FL53_Code_Page,MRAM_FL54_Code_Page,MRAM_FL55_Code_Page,MRAM_FL56_Code_Page,MRAM_FL57_Code_Page,MRAM_FL58_Code_Page,MRAM_FL59_Code_Page,MRAM_FL60_Code_Page};
uint16 MRAM_fl_CodeTbl_offset[NumMaxFl+1] = {MRAM_FL1_CodeTbl_OffSet,MRAM_FL1_CodeTbl_OffSet,MRAM_FL2_CodeTbl_OffSet,MRAM_FL3_CodeTbl_OffSet,MRAM_FL4_CodeTbl_OffSet,MRAM_FL5_CodeTbl_OffSet,MRAM_FL6_CodeTbl_OffSet,MRAM_FL7_CodeTbl_OffSet,MRAM_FL8_CodeTbl_OffSet,MRAM_FL9_CodeTbl_OffSet,MRAM_FL10_CodeTbl_OffSet,
					    MRAM_FL11_CodeTbl_OffSet,MRAM_FL12_CodeTbl_OffSet,MRAM_FL13_CodeTbl_OffSet,MRAM_FL14_CodeTbl_OffSet,MRAM_FL15_CodeTbl_OffSet,MRAM_FL16_CodeTbl_OffSet,MRAM_FL17_CodeTbl_OffSet,MRAM_FL18_CodeTbl_OffSet,MRAM_FL19_CodeTbl_OffSet,MRAM_FL20_CodeTbl_OffSet,
					    MRAM_FL21_CodeTbl_OffSet,MRAM_FL22_CodeTbl_OffSet,MRAM_FL23_CodeTbl_OffSet,MRAM_FL24_CodeTbl_OffSet,MRAM_FL25_CodeTbl_OffSet,MRAM_FL26_CodeTbl_OffSet,MRAM_FL27_CodeTbl_OffSet,MRAM_FL28_CodeTbl_OffSet,MRAM_FL29_CodeTbl_OffSet,MRAM_FL30_CodeTbl_OffSet,
					    MRAM_FL31_CodeTbl_OffSet,MRAM_FL32_CodeTbl_OffSet,MRAM_FL33_CodeTbl_OffSet,MRAM_FL34_CodeTbl_OffSet,MRAM_FL35_CodeTbl_OffSet,MRAM_FL36_CodeTbl_OffSet,MRAM_FL37_CodeTbl_OffSet,MRAM_FL38_CodeTbl_OffSet,MRAM_FL39_CodeTbl_OffSet,MRAM_FL40_CodeTbl_OffSet,
					    MRAM_FL41_CodeTbl_OffSet,MRAM_FL42_CodeTbl_OffSet,MRAM_FL43_CodeTbl_OffSet,MRAM_FL44_CodeTbl_OffSet,MRAM_FL45_CodeTbl_OffSet,MRAM_FL46_CodeTbl_OffSet,MRAM_FL47_CodeTbl_OffSet,MRAM_FL48_CodeTbl_OffSet,MRAM_FL49_CodeTbl_OffSet,MRAM_FL50_CodeTbl_OffSet,
					    MRAM_FL51_CodeTbl_OffSet,MRAM_FL52_CodeTbl_OffSet,MRAM_FL53_CodeTbl_OffSet,MRAM_FL54_CodeTbl_OffSet,MRAM_FL55_CodeTbl_OffSet,MRAM_FL56_CodeTbl_OffSet,MRAM_FL57_CodeTbl_OffSet,MRAM_FL58_CodeTbl_OffSet,MRAM_FL59_CodeTbl_OffSet,MRAM_FL60_CodeTbl_OffSet};
uint16 MRAM_fl_Code_offset[NumMaxFl+1] = {MRAM_FL1_Code_Offset,MRAM_FL1_Code_Offset,MRAM_FL2_Code_Offset,MRAM_FL3_Code_Offset,MRAM_FL4_Code_Offset,MRAM_FL5_Code_Offset,MRAM_FL6_Code_Offset,MRAM_FL7_Code_Offset,MRAM_FL8_Code_Offset,MRAM_FL9_Code_Offset,MRAM_FL10_Code_Offset,
					    MRAM_FL11_Code_Offset,MRAM_FL12_Code_Offset,MRAM_FL13_Code_Offset,MRAM_FL14_Code_Offset,MRAM_FL15_Code_Offset,MRAM_FL16_Code_Offset,MRAM_FL17_Code_Offset,MRAM_FL18_Code_Offset,MRAM_FL19_Code_Offset,MRAM_FL20_Code_Offset,
					    MRAM_FL21_Code_Offset,MRAM_FL22_Code_Offset,MRAM_FL23_Code_Offset,MRAM_FL24_Code_Offset,MRAM_FL25_Code_Offset,MRAM_FL26_Code_Offset,MRAM_FL27_Code_Offset,MRAM_FL28_Code_Offset,MRAM_FL29_Code_Offset,MRAM_FL30_Code_Offset,
					    MRAM_FL31_Code_Offset,MRAM_FL32_Code_Offset,MRAM_FL33_Code_Offset,MRAM_FL34_Code_Offset,MRAM_FL35_Code_Offset,MRAM_FL36_Code_Offset,MRAM_FL37_Code_Offset,MRAM_FL38_Code_Offset,MRAM_FL39_Code_Offset,MRAM_FL40_Code_Offset,
					    MRAM_FL41_Code_Offset,MRAM_FL42_Code_Offset,MRAM_FL43_Code_Offset,MRAM_FL44_Code_Offset,MRAM_FL45_Code_Offset,MRAM_FL46_Code_Offset,MRAM_FL47_Code_Offset,MRAM_FL48_Code_Offset,MRAM_FL49_Code_Offset,MRAM_FL50_Code_Offset,
					    MRAM_FL51_Code_Offset,MRAM_FL52_Code_Offset,MRAM_FL53_Code_Offset,MRAM_FL54_Code_Offset,MRAM_FL55_Code_Offset,MRAM_FL56_Code_Offset,MRAM_FL57_Code_Offset,MRAM_FL58_Code_Offset,MRAM_FL59_Code_Offset,MRAM_FL60_Code_Offset};
uint16 MRAM_fl_Code_LookUpTbl_Page[NumMaxFl+1] = {MRAM_FL1_LookUpTbl_Page,MRAM_FL1_LookUpTbl_Page,MRAM_FL2_LookUpTbl_Page,MRAM_FL3_LookUpTbl_Page,MRAM_FL4_LookUpTbl_Page,MRAM_FL5_LookUpTbl_Page,MRAM_FL6_LookUpTbl_Page,MRAM_FL7_LookUpTbl_Page,MRAM_FL8_LookUpTbl_Page,MRAM_FL9_LookUpTbl_Page,MRAM_FL10_LookUpTbl_Page,
					    MRAM_FL11_LookUpTbl_Page,MRAM_FL12_LookUpTbl_Page,MRAM_FL13_LookUpTbl_Page,MRAM_FL14_LookUpTbl_Page,MRAM_FL15_LookUpTbl_Page,MRAM_FL16_LookUpTbl_Page,MRAM_FL17_LookUpTbl_Page,MRAM_FL18_LookUpTbl_Page,MRAM_FL19_LookUpTbl_Page,MRAM_FL20_LookUpTbl_Page,
					    MRAM_FL21_LookUpTbl_Page,MRAM_FL22_LookUpTbl_Page,MRAM_FL23_LookUpTbl_Page,MRAM_FL24_LookUpTbl_Page,MRAM_FL25_LookUpTbl_Page,MRAM_FL26_LookUpTbl_Page,MRAM_FL27_LookUpTbl_Page,MRAM_FL28_LookUpTbl_Page,MRAM_FL29_LookUpTbl_Page,MRAM_FL30_LookUpTbl_Page,
					    MRAM_FL31_LookUpTbl_Page,MRAM_FL32_LookUpTbl_Page,MRAM_FL33_LookUpTbl_Page,MRAM_FL34_LookUpTbl_Page,MRAM_FL35_LookUpTbl_Page,MRAM_FL36_LookUpTbl_Page,MRAM_FL37_LookUpTbl_Page,MRAM_FL38_LookUpTbl_Page,MRAM_FL39_LookUpTbl_Page,MRAM_FL40_LookUpTbl_Page,
					    MRAM_FL41_LookUpTbl_Page,MRAM_FL42_LookUpTbl_Page,MRAM_FL43_LookUpTbl_Page,MRAM_FL44_LookUpTbl_Page,MRAM_FL45_LookUpTbl_Page,MRAM_FL46_LookUpTbl_Page,MRAM_FL47_LookUpTbl_Page,MRAM_FL48_LookUpTbl_Page,MRAM_FL49_LookUpTbl_Page,MRAM_FL50_LookUpTbl_Page,
					    MRAM_FL51_LookUpTbl_Page,MRAM_FL52_LookUpTbl_Page,MRAM_FL53_LookUpTbl_Page,MRAM_FL54_LookUpTbl_Page,MRAM_FL55_LookUpTbl_Page,MRAM_FL56_LookUpTbl_Page,MRAM_FL57_LookUpTbl_Page,MRAM_FL58_LookUpTbl_Page,MRAM_FL59_LookUpTbl_Page,MRAM_FL60_LookUpTbl_Page};
uint16 MRAM_fl_Code_LookUpTbl_OffSet[NumMaxFl+1] = {MRAM_FL1_LookUpTbl_OffSet,MRAM_FL1_LookUpTbl_OffSet,MRAM_FL2_LookUpTbl_OffSet,MRAM_FL3_LookUpTbl_OffSet,MRAM_FL4_LookUpTbl_OffSet,MRAM_FL5_LookUpTbl_OffSet,MRAM_FL6_LookUpTbl_OffSet,MRAM_FL7_LookUpTbl_OffSet,MRAM_FL8_LookUpTbl_OffSet,MRAM_FL9_LookUpTbl_OffSet,MRAM_FL10_LookUpTbl_OffSet,
					    MRAM_FL11_LookUpTbl_OffSet,MRAM_FL12_LookUpTbl_OffSet,MRAM_FL13_LookUpTbl_OffSet,MRAM_FL14_LookUpTbl_OffSet,MRAM_FL15_LookUpTbl_OffSet,MRAM_FL16_LookUpTbl_OffSet,MRAM_FL17_LookUpTbl_OffSet,MRAM_FL18_LookUpTbl_OffSet,MRAM_FL19_LookUpTbl_OffSet,MRAM_FL20_LookUpTbl_OffSet,
					    MRAM_FL21_LookUpTbl_OffSet,MRAM_FL22_LookUpTbl_OffSet,MRAM_FL23_LookUpTbl_OffSet,MRAM_FL24_LookUpTbl_OffSet,MRAM_FL25_LookUpTbl_OffSet,MRAM_FL26_LookUpTbl_OffSet,MRAM_FL27_LookUpTbl_OffSet,MRAM_FL28_LookUpTbl_OffSet,MRAM_FL29_LookUpTbl_OffSet,MRAM_FL30_LookUpTbl_OffSet,
					    MRAM_FL31_LookUpTbl_OffSet,MRAM_FL32_LookUpTbl_OffSet,MRAM_FL33_LookUpTbl_OffSet,MRAM_FL34_LookUpTbl_OffSet,MRAM_FL35_LookUpTbl_OffSet,MRAM_FL36_LookUpTbl_OffSet,MRAM_FL37_LookUpTbl_OffSet,MRAM_FL38_LookUpTbl_OffSet,MRAM_FL39_LookUpTbl_OffSet,MRAM_FL40_LookUpTbl_OffSet,
					    MRAM_FL41_LookUpTbl_OffSet,MRAM_FL42_LookUpTbl_OffSet,MRAM_FL43_LookUpTbl_OffSet,MRAM_FL44_LookUpTbl_OffSet,MRAM_FL45_LookUpTbl_OffSet,MRAM_FL46_LookUpTbl_OffSet,MRAM_FL47_LookUpTbl_OffSet,MRAM_FL48_LookUpTbl_OffSet,MRAM_FL49_LookUpTbl_OffSet,MRAM_FL50_LookUpTbl_OffSet,
					    MRAM_FL51_LookUpTbl_OffSet,MRAM_FL52_LookUpTbl_OffSet,MRAM_FL53_LookUpTbl_OffSet,MRAM_FL54_LookUpTbl_OffSet,MRAM_FL55_LookUpTbl_OffSet,MRAM_FL56_LookUpTbl_OffSet,MRAM_FL57_LookUpTbl_OffSet,MRAM_FL58_LookUpTbl_OffSet,MRAM_FL59_LookUpTbl_OffSet,MRAM_FL60_LookUpTbl_OffSet};
#else
uint16 MRAM_fl_code_page[NumMaxFl+1] = {MRAM_FL1_Code_Page,MRAM_FL1_Code_Page,MRAM_FL2_Code_Page,MRAM_FL3_Code_Page,MRAM_FL4_Code_Page,MRAM_FL5_Code_Page,MRAM_FL6_Code_Page,MRAM_FL7_Code_Page,MRAM_FL8_Code_Page,MRAM_FL9_Code_Page,MRAM_FL10_Code_Page,
					    MRAM_FL11_Code_Page,MRAM_FL12_Code_Page};
uint16 MRAM_fl_CodeTbl_offset[NumMaxFl+1] = {MRAM_FL1_CodeTbl_OffSet,MRAM_FL1_CodeTbl_OffSet,MRAM_FL2_CodeTbl_OffSet,MRAM_FL3_CodeTbl_OffSet,MRAM_FL4_CodeTbl_OffSet,MRAM_FL5_CodeTbl_OffSet,MRAM_FL6_CodeTbl_OffSet,MRAM_FL7_CodeTbl_OffSet,MRAM_FL8_CodeTbl_OffSet,MRAM_FL9_CodeTbl_OffSet,MRAM_FL10_CodeTbl_OffSet,
					    MRAM_FL11_CodeTbl_OffSet,MRAM_FL12_CodeTbl_OffSet};
uint16 MRAM_fl_Code_offset[NumMaxFl+1] = {MRAM_FL1_Code_Offset,MRAM_FL1_Code_Offset,MRAM_FL2_Code_Offset,MRAM_FL3_Code_Offset,MRAM_FL4_Code_Offset,MRAM_FL5_Code_Offset,MRAM_FL6_Code_Offset,MRAM_FL7_Code_Offset,MRAM_FL8_Code_Offset,MRAM_FL9_Code_Offset,MRAM_FL10_Code_Offset,
					    MRAM_FL11_Code_Offset,MRAM_FL12_Code_Offset};
uint16 MRAM_fl_Code_LookUpTbl_Page[NumMaxFl+1] = {MRAM_FL1_LookUpTbl_Page,MRAM_FL1_LookUpTbl_Page,MRAM_FL2_LookUpTbl_Page,MRAM_FL3_LookUpTbl_Page,MRAM_FL4_LookUpTbl_Page,MRAM_FL5_LookUpTbl_Page,MRAM_FL6_LookUpTbl_Page,MRAM_FL7_LookUpTbl_Page,MRAM_FL8_LookUpTbl_Page,MRAM_FL9_LookUpTbl_Page,MRAM_FL10_LookUpTbl_Page,
					    MRAM_FL11_LookUpTbl_Page,MRAM_FL12_LookUpTbl_Page,};
uint16 MRAM_fl_Code_LookUpTbl_OffSet[NumMaxFl+1] = {MRAM_FL1_LookUpTbl_OffSet,MRAM_FL1_LookUpTbl_OffSet,MRAM_FL2_LookUpTbl_OffSet,MRAM_FL3_LookUpTbl_OffSet,MRAM_FL4_LookUpTbl_OffSet,MRAM_FL5_LookUpTbl_OffSet,MRAM_FL6_LookUpTbl_OffSet,MRAM_FL7_LookUpTbl_OffSet,MRAM_FL8_LookUpTbl_OffSet,MRAM_FL9_LookUpTbl_OffSet,MRAM_FL10_LookUpTbl_OffSet,
					    MRAM_FL11_LookUpTbl_OffSet,MRAM_FL12_LookUpTbl_OffSet};
#endif


//***************************************************************************************
//***************************************************************************************
// Following Subroutines used by LCD routine to input codes from user
// Note: Front or Rear access bit set in Code_Num[0] as 0x80 (for rear)
//***************************************************************************************
//***************************************************************************************

/***************************************************************
*******    Read LookUp Table Array from NV Ram     *************
****************************************************************/
void Read_LookUpTable_Array(int16 FloorNum, int16 LkUpTbl[])
{
	int16 i, m;
	m = MRAM_fl_Code_LookUpTbl_OffSet[FloorNum];
	for(i=0;i<NumMaxFl;i++)
	{
		LkUpTbl[i] = rd_mram(MRAM_fl_Code_LookUpTbl_Page[FloorNum],m);
		m+=2;
	} 	
}
/***********************************************************
*******    Write LookUp Table Array to NV Ram  *************
************************************************************/
void Write_LookUpTable_Array(int16 FloorNum, int16 LkUpTbl[])
{
	int16 i,k;
	k = MRAM_fl_Code_LookUpTbl_OffSet[FloorNum];
	for(i=0;i<NumMaxFl;i++)
	{
		wr_mram(MRAM_fl_Code_LookUpTbl_Page[FloorNum],k,LkUpTbl[i]);
		k+=2;
	} 	
}

/***************************************************
*******       Modify LookUp Table      *************
****************************************************/
void Modify_LookUpTable_Array(struct CdTable *ctp, int16 LkUpTbl[])
{
	int16 i;
	int16 j;
	unsigned char x;
	bool IndexFound;
	for(i = 0;i<NumMaxFl;i++)
	{
		x = (unsigned char)i+1;
		IndexFound = false;
		for(j = 0;j<ctp->NumCode;j++)
		{
			// Note: Front or Rear access bit set in Code_Num[0] as 0x80 (for rear)
			if(((ctp->Codes[j][0] & 0x7F) == x) && !IndexFound)
			{	  // Only looking at first conde and not front or rear
				IndexFound = true;
				LkUpTbl[i] = j;	
			}			
		}	
	}
	
}


//**************************************************************
//**************************************************************
// Initialize the Code check table 
// Build a code check table from the data in MRAM
//**************************************************************
//**************************************************************

void Init_Code_Check_Table (void)
{
	int16 i;
	int16 j;
	int16 k;
	
	for (i=1;i<=cons[grtopf];i++)
	{
	   	// Get the number of Codes for that specific floor
		k = MRAM_fl_CodeTbl_offset[i];
		if (rd_mram(MRAM_fl_code_page[i],k) != (int16)0x29b5)
		{		  // Validation code is invalid	so zero table
			for(j=0;j<MRAM_Fl_Code_Length;j++)
			{
				wr_mram(MRAM_fl_code_page[i],k,0x0000);
				k+=2;
			}
			k = MRAM_fl_CodeTbl_offset[i];
			wr_mram(MRAM_fl_code_page[i],k,0x29b5);

			// clear look up table
			k = MRAM_fl_Code_LookUpTbl_OffSet[i];
			for(j=0;j<NumMaxFl;j++)
			{
				wr_mram(MRAM_fl_Code_LookUpTbl_Page[i],k,-1);
				k+=2;
			} 	

			Code_Tbl_Chk[i].NumCode = 0;
			Code_Tbl_Chk[i].CheckSum = 0;
		}
		else
		{
			Code_Tbl_Chk[i].NumCode = rd_mram(MRAM_fl_code_page[i],k);
			k+=2;
			Code_Tbl_Chk[i].CheckSum = rd_mram(MRAM_fl_code_page[i],k);
			k+=2;
		}
	}
}

/**************************************************************
*******       Read Code Table from NV Ram         *************
***************************************************************/


void Read_CodeTbl(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[])
{ 
	int16 i,k;
	union {
			uint8 b[2];	// int byte 0 and 1
			uint16 w;		// short int
	}uwu;

	Read_LookUpTable_Array(FloorNum, LkUpTbl);

   	// Get the number of Codes for that specific floor
	k = MRAM_fl_CodeTbl_offset[FloorNum];
	ctp->ValidTable = rd_mram(MRAM_fl_code_page[FloorNum],k);
	k+=2;

	ctp->NumCode = rd_mram(MRAM_fl_code_page[FloorNum],k);
	k+=2;

	ctp->CheckSum = rd_mram(MRAM_fl_code_page[FloorNum],k);
	k+=2;

	ctp->Codes[0][0] = 0;
	ctp->Codes[0][1] = 0;
	ctp->Codes[0][2] = 0;
	ctp->Codes[0][3] = 0;

   // Get Codes
	k = MRAM_fl_Code_offset[FloorNum];
	i = 0;
	while ((i < ctp->NumCode) && (i < NumMaxCd))
	{
		uwu.w = rd_mram(MRAM_fl_code_page[FloorNum],k);
		k+=2;
		ctp->Codes[i][0] = uwu.b[lbw];
		ctp->Codes[i][1] = uwu.b[hbw];
		uwu.w = rd_mram(MRAM_fl_code_page[FloorNum],k);
		k+=2;
	    ctp->Codes[i][2] = uwu.b[lbw];
		ctp->Codes[i][3] = uwu.b[hbw];
		i++; 		
	}
   
} 
/************************************************************
*******      Write Code Table to NV Ram       ***************
*******  Used for LCD and Comm Routines that  ***************
*******  is why pointers are used for the structurs ********* 
*************************************************************/

void Write_CodeTbl(int16 FloorNum, struct CdTable *ctp,int16 LkUpTbl[])
{											 //                ^^^ note FloorNum must be used in parameter
	int16 i,k; 
	union {
			uint8 b[2];	// int byte 0 and 1
			uint16 w;		// short int
	}uwu;

	Modify_LookUpTable_Array(ctp, LkUpTbl);
	Write_LookUpTable_Array(FloorNum, LkUpTbl);  

	//Dumps the code into memory
	ctp->CheckSum = 0;
	k = MRAM_fl_Code_offset[FloorNum];
	i = 0;
	while ((i < ctp->NumCode) && (i < NumMaxCd))
	{
		uwu.b[lbw] = ctp->Codes[i][0];
		uwu.b[hbw] = ctp->Codes[i][1];
		wr_mram(MRAM_fl_code_page[FloorNum],k,uwu.w);
		k+=2;
		uwu.b[lbw] = ctp->Codes[i][2];
		uwu.b[hbw] = ctp->Codes[i][3]; 
		wr_mram(MRAM_fl_code_page[FloorNum],k,uwu.w);
		k+=2;
		ctp->CheckSum += ctp->Codes[i][0];		
		ctp->CheckSum += ctp->Codes[i][1];		
		ctp->CheckSum += ctp->Codes[i][2];		
		ctp->CheckSum += ctp->Codes[i][3];		
		i++;
	}	
	// Write zeros to the last code	+ 1 (four bytes)
	wr_mram(MRAM_fl_code_page[FloorNum],k,0x0000);
	k+=2;
	wr_mram(MRAM_fl_code_page[FloorNum],k,0x0000);
	k+=2;

	k = MRAM_fl_CodeTbl_offset[FloorNum];
	ctp->ValidTable = 0x29b5;
	wr_mram(MRAM_fl_code_page[FloorNum],k,0x29b5);
	k+=2;

	wr_mram(MRAM_fl_code_page[FloorNum],k,ctp->NumCode);
	k+=2;
	
	wr_mram(MRAM_fl_code_page[FloorNum],k,ctp->CheckSum);
	k+=2;
	
	Code_Tbl_Chk[FloorNum].CheckSum = ctp->CheckSum;
	Code_Tbl_Chk[FloorNum].NumCode = ctp->NumCode;
}


/***************************************************
*******   Code Floor    ***************
****************************************************/

int16 Add_One_Code_All_Floors(int16 start_flag, int16 wherefrom, unsigned char Code_Num[])
{								// return zero when not busy
								// wherefrom: 0=LCD,1=gc, 2=Com1, 3=Com2
	//lint -esym(438,start_flag)
	static int16 fl_ix[4];
	static int16 add_one_seq[4];
	static int16 add_one_cnt[4];
	static unsigned char code[4][4];

	if ((start_flag == 1) && (add_one_seq[wherefrom] == 0))
	{
		start_flag = 0;
		add_one_cnt[wherefrom] = 5;
		add_one_seq[wherefrom] = 1;
		code[wherefrom][0] = Code_Num[0];
		code[wherefrom][1] = Code_Num[1];
		code[wherefrom][2] = Code_Num[2];
		code[wherefrom][3] = Code_Num[3];
		fl_ix[wherefrom] = 1;
	}
	else if ((start_flag == 0) && (add_one_seq[wherefrom] == 1))
	{
		if (add_one_cnt[wherefrom] >= 5)
		{
			add_one_cnt[wherefrom] = 0;
			if (fl_ix[wherefrom] <= cons[grtopf])
			{
				//Clear  CodeTable
				if (wherefrom == 0) 
				{				// lcd
					Read_CodeTbl(fl_ix[wherefrom],	&CodeTable, LookUpTable);
					Add_Security_Code(fl_ix[wherefrom], &CodeTable, LookUpTable, code[wherefrom]);
				}
				else if (wherefrom == 1)
				{				// group comm
					Read_CodeTbl(fl_ix[wherefrom],	&gc_CodeTable, gc_LookUpTable);
					Add_Security_Code(fl_ix[wherefrom], &gc_CodeTable, gc_LookUpTable, code[wherefrom]);
				}
				else if (wherefrom == 2)
				{			    // com1
					Read_CodeTbl(fl_ix[wherefrom],	&Com_CodeTable[0], Com_LookUpTable[0]);
					Add_Security_Code(fl_ix[wherefrom], &Com_CodeTable[0], Com_LookUpTable[0], code[wherefrom]);
				}
				else if (wherefrom == 3)
				{				// com2
					Read_CodeTbl(fl_ix[wherefrom],	&Com_CodeTable[1], Com_LookUpTable[1]);
					Add_Security_Code(fl_ix[wherefrom], &Com_CodeTable[1], Com_LookUpTable[1], code[wherefrom]);
				}

				fl_ix[wherefrom]++;
			}
			else
			{
				add_one_seq[wherefrom] = 0;
			}
		}
		else 
			add_one_cnt[wherefrom]++;  
	}

	return add_one_seq[wherefrom];
}

/***************************************************
*******   Delete Code all Floor      ***************
****************************************************/

int16 Delete_One_Code_All_Floors(int16 start_flag, int16 wherefrom, unsigned char Code_Num[])
{								// return zero when not busy
								// wherefrom: 0=LCD,1=gc, 2=Com1, 3=Com2
	//lint -esym(438,start_flag)
	static int16 fl_ix[4];
	static int16 del_one_seq[4];
	static int16 del_one_cnt[4];
	static unsigned char code[4][4];

	if ((start_flag == 1) && (del_one_seq[wherefrom] == 0))
	{
		start_flag = 0;
		del_one_cnt[wherefrom] = 5;
		del_one_seq[wherefrom] = 1;
		code[wherefrom][0] = Code_Num[0];
		code[wherefrom][1] = Code_Num[1];
		code[wherefrom][2] = Code_Num[2];
		code[wherefrom][3] = Code_Num[3];
		fl_ix[wherefrom] = 1;
	}
	else if ((start_flag == 0) && (del_one_seq[wherefrom] == 1))
	{

		if (del_one_cnt[wherefrom] >= 5)
		{
			del_one_cnt[wherefrom] = 0;
			if (fl_ix[wherefrom] <= cons[grtopf])
			{
				//Clear  CodeTable
				if (wherefrom == 0) 
				{
					Read_CodeTbl(fl_ix[wherefrom],	&CodeTable, LookUpTable);
					Delete_Security_Code(fl_ix[wherefrom], &CodeTable, LookUpTable, code[wherefrom]);
				}
				else if (wherefrom == 1)
				{
					Read_CodeTbl(fl_ix[wherefrom],	&gc_CodeTable, gc_LookUpTable);
					Delete_Security_Code(fl_ix[wherefrom], &gc_CodeTable, gc_LookUpTable, code[wherefrom]);
				}
				else if (wherefrom == 2)
				{			    // com1
					Read_CodeTbl(fl_ix[wherefrom],	&Com_CodeTable[0], Com_LookUpTable[0]);
					Delete_Security_Code(fl_ix[wherefrom], &Com_CodeTable[0], Com_LookUpTable[0], code[wherefrom]);
				}
				else if (wherefrom == 3)
				{				// com2
					Read_CodeTbl(fl_ix[wherefrom],	&Com_CodeTable[1], Com_LookUpTable[1]);
					Delete_Security_Code(fl_ix[wherefrom], &Com_CodeTable[1], Com_LookUpTable[1], code[wherefrom]);
				}

				fl_ix[wherefrom]++;
			}
			else
			{
				del_one_seq[wherefrom] = 0;
			}
		}
		else 
			del_one_cnt[wherefrom]++;  
	}

	return del_one_seq[wherefrom];
}

/***************************************************
*******   Reset All Lockout Floor    ***************
****************************************************/
/* Each Page in Memory will Hold  data for 30 floor*/
int16 Clear_All_Codes_All_Floors(int16 start_flag, int16 wherefrom)
{								// return zero when not busy
								// wherefrom: 0=LCD,1=gc, 2=Com1, 3=Com2
	static int16 fl_ix[4];
	static int16 clr_all_seq[4];
	static int16 clr_all_cnt[4];

	if ((start_flag == 1) && (clr_all_seq[wherefrom] == 0))
	{
		start_flag = 0;
		clr_all_cnt[wherefrom] = 5;
		clr_all_seq[wherefrom] = 1;
		fl_ix[wherefrom] = 1;
	}
	else if ((start_flag == 0) && (clr_all_seq[wherefrom] == 1))
	{

		if (clr_all_cnt[wherefrom] >= 5)
		{
			clr_all_cnt[wherefrom] = 0;
			if (fl_ix[wherefrom] <= cons[grtopf])
			{
				//Clear  CodeTable
				if (wherefrom == 0) 
				{
					Clear_All_Codes_One_Floor(fl_ix[wherefrom],&CodeTable,LookUpTable);
				}
				else if (wherefrom == 1)
				{
					Clear_All_Codes_One_Floor(fl_ix[wherefrom], &gc_CodeTable,gc_LookUpTable);
				}
				else if (wherefrom == 2)
				{			    // com1
					Clear_All_Codes_One_Floor(fl_ix[wherefrom],&Com_CodeTable[0],Com_LookUpTable[0]);
				}
				else if (wherefrom == 3)
				{				// com2
					Clear_All_Codes_One_Floor(fl_ix[wherefrom],&Com_CodeTable[1],Com_LookUpTable[1]);
				}
				fl_ix[wherefrom]++;
			}
			else
			{
				clr_all_seq[wherefrom] = 0;
			}
		}
		else 
			clr_all_cnt[wherefrom]++;  
	}

	return clr_all_seq[wherefrom];
}


/***************************************************
*******      Clear All Floor Codes   ***************
****************************************************/  
void Clear_All_Codes_One_Floor(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[])
{
	int16 i,k;

	for( i=0;i<ctp->NumCode;i++)
	{
		ctp->Codes[i][0]=0;
		ctp->Codes[i][1]=0;
		ctp->Codes[i][2]=0;
		ctp->Codes[i][3]=0;	 		
	}
	ctp->ValidTable = 0x29b5;
  	ctp->CheckSum = 0;
   	ctp->NumCode = 0;

	// Clear LookUp Table
	for(i=0;i<NumMaxFl;i++)
	{
		LkUpTbl[i] = -1;		
	}
	Write_LookUpTable_Array(FloorNum, LkUpTbl);

	k = MRAM_fl_CodeTbl_offset[FloorNum];
	wr_mram(MRAM_fl_code_page[FloorNum],k,0x29b5);	// valid code
	k+=2;
	wr_mram(MRAM_fl_code_page[FloorNum],k,0x0000);				// Number of codes
	k+=2;
	wr_mram(MRAM_fl_code_page[FloorNum],k,0x0000);				// Checksum
	k+=2;
	wr_mram(MRAM_fl_code_page[FloorNum],k,0x0000);				// First Code in table
	k+=2;
	wr_mram(MRAM_fl_code_page[FloorNum],k,0x0000);				
	k+=2;

	Code_Tbl_Chk[FloorNum].CheckSum = 0;
	Code_Tbl_Chk[FloorNum].NumCode = 0;

}


/***************************************************
*******      Add Security Code       ***************
****************************************************/
int16 Add_Security_Code(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[], unsigned char Code_Num[])
{	
	if (ctp->NumCode >= NumMaxCd)
	{
		return 0;
	}
	else if(ctp->NumCode == 0)
	{
	   	ctp->Codes[0][0] = Code_Num[0];
		ctp->Codes[0][1] = Code_Num[1];
		ctp->Codes[0][2] = Code_Num[2];
		ctp->Codes[0][3] = Code_Num[3];
		ctp->NumCode++;
		Write_CodeTbl(FloorNum, ctp, LkUpTbl);
		return 1; 
	}
	else if(!Check_Security_Code(Code_Num, ctp, LkUpTbl))
	{
		ctp->Codes[ctp->NumCode][0] = Code_Num[0];
		ctp->Codes[ctp->NumCode][1] = Code_Num[1];
		ctp->Codes[ctp->NumCode][2] = Code_Num[2];
		ctp->Codes[ctp->NumCode][3] = Code_Num[3];
		ctp->NumCode++;
		Sort_CodeTbl(ctp);
		Write_CodeTbl(FloorNum, ctp, LkUpTbl); 
		return 1;
	}
	return 0;
}


/***************************************************
*******      Remove Security Code    ***************
****************************************************/
int16 Delete_Security_Code(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[], unsigned char Code_Num[])
{
	int16 i;
	if (Check_Security_Code(Code_Num, ctp, LkUpTbl))
	{
		for(i = CodeLoc; i<ctp->NumCode;i++)
		{
			ctp->Codes[i][0] = ctp->Codes[i+1][0];
			ctp->Codes[i][1] = ctp->Codes[i+1][1];
			ctp->Codes[i][2] = ctp->Codes[i+1][2];
			ctp->Codes[i][3] = ctp->Codes[i+1][3];
		}
		ctp->Codes[i][0] = 0;
		ctp->Codes[i][1] = 0;
		ctp->Codes[i][2] = 0;
		ctp->Codes[i][3] = 0;
	    ctp->NumCode--;
		Write_CodeTbl(FloorNum, ctp, LkUpTbl);
		return 1; 
	}
	return 0;
}

/***************************************************
*******      Check Security Code    ****************
****************************************************/
bool Check_Security_Code(unsigned char dig[], struct CdTable *ctp, int16 LkUpTbl[])
{

	int16 i;
	int16 index;
	bool Pass = false;

	index = LkUpTbl[(dig[0] & 0x7F)-1];
	if ((index != -1) && (index < NumMaxCd))
	{
		i = 0;
		while (((ctp->Codes[index][0] & 0x7F) == (dig[0] & 0x7F)) && (index < ctp->NumCode))	 //lint !e661
		{			 // Looking at first code and not front or rear
			i++;
			if(i == 1)
			{
				if(ctp->Codes[index][i] != dig[1])
				{
					i=i-1;
					index++;
					Pass = false;
					if(dig[1] < ctp->Codes[index-1][1])	// CodeTable is Sorted no need to check
						break;
				}
				else
					Pass = true;
			}
			else if	(i == 2)
			{
				if(ctp->Codes[index][i] != dig[2])
				{
					i=i-2;
					index++;
					Pass = false;
					if(dig[2] < ctp->Codes[index-1][2])   // CodeTable is Sorted no need to check
						break;
				}
				else
					Pass = true;
			}
			else if	(i == 3)
			{
				if(ctp->Codes[index][i] != dig[3])
				{
					i=i-3;
					index++;
					Pass = false;
					if(dig[3] < ctp->Codes[index-1][3])   // CodeTable is Sorted no need to check
						break;
				}
				else if ((ctp->Codes[index][0] & 0x80) != (dig[0] & 0x80))
				{
					i=i-3;
					index++;
					Pass = false;
				}
				else
				{
					Pass = true;
				   	CodeLoc = index;
					break; 
				}
			}
		}
	}
   	else
		Pass = false;

	return Pass;
}

/***************************************************
*******        Sort Code Table         *************
****************************************************/
void Sort_CodeTbl(struct CdTable *ctp)
{
	
	int16 j;
	int16 i;
	uint8 InsertionKey_Num1;
	uint8 InsertionKey_Num2;
	uint8 InsertionKey_Num3;
	uint8 InsertionKey_Num4;
	bool Sorted = false;

	if ( ctp->NumCode > NumMaxCd)
		return;

	j = ctp->NumCode - 1;		// last code resides
	InsertionKey_Num1 = ctp->Codes[j][0];
	InsertionKey_Num2 = ctp->Codes[j][1];
	InsertionKey_Num3 = ctp->Codes[j][2];
	InsertionKey_Num4 = ctp->Codes[j][3];

	i = j-1;
	while ((i >= 0) && (Sorted == false))
	{
		if((ctp->Codes[i][0] & 0x7F) > (InsertionKey_Num1 & 0x7F))
		{
			ctp->Codes[i+1][0] = ctp->Codes[i][0];
			ctp->Codes[i+1][1] = ctp->Codes[i][1];
			ctp->Codes[i+1][2] = ctp->Codes[i][2];
			ctp->Codes[i+1][3] = ctp->Codes[i][3];
			i--;					
		}
		else if((ctp->Codes[i][0] & 0x7F) == (InsertionKey_Num1 & 0x7F))
		{
			if(ctp->Codes[i][1] > InsertionKey_Num2)
			{
				ctp->Codes[i+1][0] = ctp->Codes[i][0];
				ctp->Codes[i+1][1] = ctp->Codes[i][1];
				ctp->Codes[i+1][2] = ctp->Codes[i][2];
				ctp->Codes[i+1][3] = ctp->Codes[i][3];
				i--;
			}
			else if	(ctp->Codes[i][1] == InsertionKey_Num2)
			{
				if(ctp->Codes[i][2] > InsertionKey_Num3)
				{
					ctp->Codes[i+1][0] = ctp->Codes[i][0];
					ctp->Codes[i+1][1] = ctp->Codes[i][1];
					ctp->Codes[i+1][2] = ctp->Codes[i][2];
					ctp->Codes[i+1][3] = ctp->Codes[i][3];
					i--;
				}
				else if	(ctp->Codes[i][2] == InsertionKey_Num3)
				{
					if(ctp->Codes[i][3] > InsertionKey_Num4)
					{
						ctp->Codes[i+1][0] = ctp->Codes[i][0];
						ctp->Codes[i+1][1] = ctp->Codes[i][1];
						ctp->Codes[i+1][2] = ctp->Codes[i][2];
						ctp->Codes[i+1][3] = ctp->Codes[i][3];
						i--;
					}
					else 
						Sorted = true;
				}
				else
					Sorted = true;
			}
			else
				Sorted = true;
		}
		else
			Sorted = true;	
	}
	
	ctp->Codes[i+1][0] = InsertionKey_Num1;
	ctp->Codes[i+1][1] = InsertionKey_Num2;
	ctp->Codes[i+1][2] = InsertionKey_Num3;
	ctp->Codes[i+1][3] = InsertionKey_Num4;
	
}

//***********************************************************
//***********************************************************
// Verify Code Check for checksumm and number of codes 
//***********************************************************
//***********************************************************

int16 Verify_Code_Check_Table (struct Cd_Tbl_Chk *ckp)
{
	int16 i;
	int16 floor_upd = 0;

	i = 1;
	while ((i<=cons[grtopf]) && (floor_upd == 0))
	{
		if ((Code_Tbl_Chk[i].CheckSum != ckp->CheckSum) ||
			(Code_Tbl_Chk[i].NumCode != ckp->NumCode))
			floor_upd = i;
		i++;
	}
	return (floor_upd);
}

//***************************************************************************************
//***************************************************************************************
// Following Subroutines used by controller to verify code pushed with car call button
//***************************************************************************************
//***************************************************************************************

/***************************************************
*******       Check LookUp Table       *************
****************************************************/
int16 get_LookUpTbl_index(unsigned char First_Code, int16 FloorNum)
{
	uint16 CodeIndex_ML;
	CodeIndex_ML = (uint16)(First_Code - 1);
	return rd_mram(MRAM_fl_Code_LookUpTbl_Page[FloorNum],(int16)(MRAM_fl_Code_LookUpTbl_OffSet[FloorNum] + CodeIndex_ML));
}


/***************************************************
*******   Check the  security code entered  ********
****************************************************/

int16 Valid_Security_Code(unsigned char Sec_Code[], int16 Floor_Req)
{
	int16 i,k;
	int16 numcodes;
	int16 FloorNum;
	unsigned char code[4];
	union {
			uint8 b[2];	// int byte 0 and 1
			uint16 w;		// short int
	}uwu;

	FloorNum = Floor_Req & 0x7F;

	
	numcodes = rd_mram(MRAM_fl_code_page[FloorNum],(int16)(MRAM_fl_CodeTbl_offset[FloorNum]+2));

	if (numcodes > NumMaxCd)
	{
		return 0;
	}

	i = get_LookUpTbl_index(Sec_Code[0], FloorNum);

	k = (int16)(MRAM_fl_Code_offset[FloorNum]+ (2*i));
	
	uwu.w = rd_mram(MRAM_fl_code_page[FloorNum],k);
	k+=2;
	code[0] = uwu.b[lbw]; 
	code[1] = uwu.b[hbw]; 
	uwu.w = rd_mram(MRAM_fl_code_page[FloorNum],k);
	k+=2;
	code[2] = uwu.b[lbw]; 
	code[3] = uwu.b[hbw]; 
	while (i < numcodes)
	{
		if (((Sec_Code[0] & 0x7F) == (code[0] & 0x7F)) && ((code[0] & 0x80) == (Floor_Req & 0x80)) && (Sec_Code[1] == code[1]) && (Sec_Code[2] == code[2]) && (Sec_Code[3] == code[3]))
		{
			return 1; 
		}
		else
		{
			uwu.w = rd_mram(MRAM_fl_code_page[FloorNum],k);
			k+=2;
			code[0] = uwu.b[lbw]; 
			code[1] = uwu.b[hbw]; 
			uwu.w = rd_mram(MRAM_fl_code_page[FloorNum],k);
			k+=2;
			code[2] = uwu.b[lbw]; 
			code[3] = uwu.b[hbw]; 
			if (Sec_Code[0] < (code[0] & 0x7f))
			{
				return 0; 
			}
		}
		i++;
	}

	return 0; 
}



//****************************************
// Set the floors that are secured 
//****************************************
void set_secured_fl(void)
{
	int16 i;
	int16 fl_ix;
	int16 sec_override;

	if ((servf == s_IND) && ((fvars[fvindsec] & 0x01) != 0))		// on independent and want to override security
		ind_override = 1;
	else if ((cons[IndSec] == 1) && (rdinp(i_INDS) == 1) && (servf == s_IND) && (rdinp(i_IDSO) == 1))
		ind_override = 1;
	else
		ind_override = 0;
	   
	
  	if (((servf == s_MED_EMS) || (servf == s_HSV)) && (fvars[fvmedccsovride] == 1))	// on code blue, medical service or hospital service
		med_override = 1;
	else
		med_override = 0;
	
	i = 1;
	floor_access_req = 0;
  	while ((i <= cons[topf]) && (floor_access_req == 0)) 
	{
		fl_ix = (i-1)/32;

		if ( ((Lockout_Override == 1) && ((rdinp(i_CCS(i)) == 1) || (rdinp(i_SECUR) == 1)) && ((c_tmr_svc[s_ccl_override] == 1) || (c_tmr_svc_en[s_ccl_override] == 0)) && (Car_Call_Lockouts == 0)) ||
		     ((Grp_CC_Override == 1) && ((gc_cc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0) && ((c_tmr_svc[s_gccl_override] == 1) || (c_tmr_svc_en[s_gccl_override] == 0)) ) )
			sec_override = 1;		// ccs input used for override or lockout but not both
		else
			sec_override = 0;

	    if(( ((cons[Car_Asgn_Fl] == 1) && (car_Grp_Stat.c_car_asgn_sec != 0) && (car_asgn_cc[i] != cons[carnmb]) && (car_asgn_cc[i] != 0)) ||
			 ((Car_Call_Lockouts != 0) && (rdinp(i_CCS(i)) != 0) && ((rdinp(i_SECUR) != 0) || (c_tmr_svc[s_cc_lockouts] != 0))) ||
			 ((cc_sec_mask[fl_ix] & fl_mask[i]) != 0) ||
			 ((cons[hugs] == 2) && (gc_hugsec == 1) && ((gc_hugs_sec[fl_ix] & fl_mask[i]) != 0)) ||
			 ((Grp_CC_Lockouts != 0) && ((gc_sec != 0) || (c_tmr_svc[s_gcc_lockouts] != 0)) && ((gc_cc_sec[fl_ix] & fl_mask[i]) != 0)) ||	// gc_cc_sec is shifted to use i+1
			 (((ids_cc_sec[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && ((cons[ids] & 1) != 0)) ||
		     (((gc_ccs_per_fl[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && (cons[SecFlCfg] == 7) && (fvars[fvccpbsongpccsec] == 1) && (cons[cc_pb_sec] == 1))
		    ) && (firef == 0) && (hsvf == 0) && (med_override == 0) && (med_override == 0) && (ind_override == 0) && (sec_override == 0) && (i != fvars[fvsecfl])		  
		  ) 
    	{					// the floor is secured
  			floor_secured[i] = 1;
			if ((rd_both_inps(i_CC(i),i_XCC(i)) == 1) && (carcb[i] == 0) && (floor_access_req == 0))
				floor_access_req = i;
		}
		else
		{
		  	floor_secured[i] = 0;
		}


	    if(cons[rear] != 0)
	    {
			if ( ((Lockout_Override == 1) && ((rdinp(i_RCCS(i)) == 1) || (rdinp(i_RSECU) == 1))  && ((c_tmr_svc[s_ccl_override] == 1) || (c_tmr_svc_en[s_ccl_override] == 0)) && (Car_Call_Lockouts == 0)) ||
		         (((Grp_CC_Override == 1) || (c_tmr_svc[s_gccl_override] == 1)) && ((gc_rcc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0)) )
				sec_override = 1;		// rccs input used for override or lockout but not both
			else
				sec_override = 0;

			if(( ((cons[Car_Asgn_Fl] != 0) && (car_Grp_Stat.c_car_asgn_sec != 0) && (car_asgn_rcc[i] != cons[carnmb]) && (car_asgn_rcc[i] != 0)) ||
				 ((Car_Call_Lockouts != 0) && (rdinp(i_RCCS(i)) != 0) && ((rdinp(i_RSECU) != 0) || (c_tmr_svc[s_cc_lockouts] != 0))) ||
				 ((rcc_sec_mask[fl_ix] & fl_mask[i]) != 0) ||
				 ((cons[hugs] == 2) && (gc_rhugsec == 1) && ((gc_rhugs_sec[fl_ix] & fl_mask[i]) != 0)) ||
		         ((Grp_CC_Lockouts != 0) && ((gc_rsec != 0) || (c_tmr_svc[s_gcc_lockouts] != 0)) && ((gc_rcc_sec[fl_ix] & fl_mask[i]) != 0)) ||  // gc_rcc_sec is shifted to use i+1
				 (((ids_rcc_sec[fl_ix][cons[carnmb]] & fl_mask[i]) != 0) && ((cons[ids] & 1) != 0))
			    ) && (firef == 0) && (hsvf == 0) && (med_override == 0) && (ind_override == 0)	&& (sec_override == 0) && (i != fvars[fvsecfl])		  
			  )
			{
  				rfloor_secured[i] = 1;
				if ((rd_both_inps(i_CCR(i),i_XCCR(i)) == 1) && (rcarcb[i] == 0) && (floor_access_req == 0))
					floor_access_req = (i | 0x80);		   // if 0x80 bit set then rear floor
			}
			else
			{
			  	rfloor_secured[i] = 0;
			}
		}
		else
		{
		  	rfloor_secured[i] = 0;
		}

		i++;
	}
}
//****************************************************************
//***  Security Mode Check to see a correct code was entered   ***
//****************************************************************
int16 chk_push_button(void)
{
  	int16 i;
  	int16 PB_Count;

  	if( ((rdinp(i_CCPBS) == 1) || (c_tmr_svc[s_cc_pb_sec] == 1)) && (cons[cc_pb_sec] == 1) && (firef == 0) && (hsvf == 0))
  	{
  	  	timers[ccct] = 0;
  	  	if(Security_code_flag == 0)    // waiting for a access attempt
  	  	{                              // Car call for secured floor will initiate sequence
			set_secured_fl();
  	  	  	if (floor_access_req != 0)
			{
  	  	  		Security_code_flag = 1;
				timers[tsecure] = 0;
			}
			else
			{
				cclatch();
			}
  	  	}
  	  	if(Security_code_flag == 1)    // Start code sequence
  	  	{
  	  	  	cc_sec_buz = 1;
  	  	  	timers[tsecbep] = 0;
  	  	  	PB_Count = 0;
			PB_Pushed = 0;
  	  	  	Code_Count = 0;
  	  	  	CC_Code[0] = 0;
  	  	  	CC_Code[1] = 0;
  	  	  	CC_Code[2] = 0;
  	  	  	CC_Code[3] = 0;
  	  	  	for(i=cons[bottomf]; i<=cons[topf]; i++)
  	  	  	{
				clr_both_outps(o_CCL(i),o_XCCL(i));
  	  	  	}
  	  	  	if ( ((rd_both_inps(i_CC(floor_access_req & 0x7F),i_XCC(floor_access_req & 0x7F)) == 0) && ((floor_access_req & 0x80) == 0)) ||
				 ((rd_both_inps(i_CCR(floor_access_req & 0x7F),i_XCCR(floor_access_req & 0x7F)) == 0) && ((floor_access_req & 0x80) != 0) && (cons[rear] != 0)) )
			{		   // if 0x80 bit set then rear floor
				Security_code_flag = 2;
	  	  	}
	  	  	if (timers[tsecure] > 80)
	  	  	{                       // end of code or timer up  or to many codes
				timers[tsecure] = 0;
				Security_code_flag = 3;
	  	  	}
  	  	}
  	  	if(Security_code_flag == 2)
  	  	{

  	  	  	if(timers[tsecbep] <= fvars[fvhbept])
	  	  	  	cc_sec_buz = 1;
  	  	  	else
	  	  	  	cc_sec_buz = 0;
  	  	  	for(i=cons[bottomf]; i<= cons[topf]; i++)
  	  	  	{
				clr_both_outps(o_CCL(i),o_XCCL(i));
				if (cons[rear] != 0)
					clr_both_outps(o_CCRL(i),o_XCCRL(i));
  	  	  	}
  	  	  	PB_Count = 0;
  	  	  	for(i=cons[bottomf]; i<=cons[topf]; i++)
  	  	  	{
				if ((rd_both_inps(i_CC(i),i_XCC(i)) == 1) ||
					((rd_both_inps(i_CCR(i),i_XCCR(i)) == 1) && (cons[rear] != 0)))
				{
				  	if(PB_Pushed == 0)    // first read of a pushed button
				  	{
				  	  	CC_Code[Code_Count] = (uint8)i;
			  	  	  	timers[tsecbep] = 0;
						timers[tsecure] = 0;
				  	  	Code_Count++;
				  	}
				  	PB_Pushed = 1;
				  	PB_Count++;
				}
  	  	  	}
			if(PB_Count == 0)
			{
				PB_Pushed = 0;
  	  		}

  	  	  	if((timers[tsecure] > 80) || (Code_Count >=4))
  	  	  	{                       // end of code or timer up  or to many codes
				timers[tsecure] = 0;
				Security_code_flag = 3;
  	  	  	}
			if ((rdinp(i_FS2C) == 1) || ((cons[RGB_CC] == 1) && (rdinp(i_FiFS2C) == 1)))
			{		// cancel sequence
	  	  	  	timers[tsecbep] = 0;
	  	  	  	Security_code_flag = 4;
	  	  	  	CC_Code[0] = 0;
	  	  	  	CC_Code[1] = 0;
	  	  	  	CC_Code[2] = 0;
	  	  	  	CC_Code[3] = 0;
			}
  	  	}

  	  	if(Security_code_flag == 3)        // user has entered code check validity
  	  	{
  	  	  	if (Valid_Security_Code(CC_Code, floor_access_req) == true)         /* user i is a valid user set car call */
  	  	  	{
				if (Code_Count == 4)
				{
				    ccpbs_fl_access_req = (uint8)(floor_access_req & 0x00FF);
				    ccpbs_codes[0] = CC_Code[0];
				    ccpbs_codes[1] = CC_Code[1];
				    ccpbs_codes[2] = CC_Code[2];
				    ccpbs_codes[3] = CC_Code[3];
				    timers[tccpbsgrptmr] = 0;

					if ((floor_access_req != 0) && ((floor_access_req & 0x80) == 0))
					{
						if(ccmsk[cons[carnmb]][floor_access_req & 0x7F] == 1)
						{
						    carcb[floor_access_req] = 1;
						    lvars[nmbcc]++;
						    set_both_outps(o_CCL(floor_access_req & 0x7F),o_XCCL(floor_access_req & 0x7F));
							if ((floor_access_req & 0x7F) == position)
								front_slowdown |= cc_do;
						}
					}
					else if ((floor_access_req != 0) && ((floor_access_req & 0x80) != 0))
					{		 // if 0x80 bit set then rear floor
						if(rccmsk[cons[carnmb]][floor_access_req & 0x7F] == 1)
						{
						    rcarcb[floor_access_req & 0x7F] = 1;
						    lvars[nmbcc]++;
							set_both_outps(o_CCRL(floor_access_req & 0x7F),o_XCCRL(floor_access_req & 0x7F));
							if ((floor_access_req & 0x7F) == position)
								rear_slowdown |= cc_do;
						}
					}
				}
  	  	  	}
  	  	  	timers[tsecbep] = 0;
  	  	  	Security_code_flag = 4;
  	  	}

  	  	if(Security_code_flag == 4)
  	  	{
  	  	  	if(timers[tsecbep] <= fvars[fvhbept])
	  	  	  	cc_sec_buz = 1;
  	  	  	else
  	  	  	{
	  	  	  	cc_sec_buz = 0;
				Security_code_flag = 0;
  	  	  	}
  	  	  	for(i=cons[bottomf]; i<=cons[topf]; i++)
  	  	  	{
				if(carcb[i] == 1)
				{
				    set_both_outps(o_CCL(i),o_XCCL(i));
				}
				if(rcarcb[i] == 1)
				{
				    set_both_outps(o_CCRL(i),o_XCCRL(i));
				}
  	  	  	}
  	  	}
  	  	
  	  	if (ccpbs_fl_access_req != 0)
		{
			if (timers[tccpbsgrptmr] > 10)		// Clear ccpbs vars sent on grp com after 1 second
			{
				ccpbs_fl_access_req = 0;
				ccpbs_codes[0] = 0;
				ccpbs_codes[1] = 0;
				ccpbs_codes[2] = 0;
				ccpbs_codes[3] = 0;
			}
		}
		return 1;
  	}
	else
  		return 0;
}
/* Revision History
3/1/09 v5.47.7 mhd		1. Modify security to be pointer based.
3/9/09 v5.47.8 mhd		1. Remove pointer base for Code_Tbl_Chk but leave it for CodeTable and Com_CodeTable because whenever anyone writes to the code table
					   only one Code_Tbl_Chk must be updated. The Com_Code_Tbl_Chk only holds the data being transmitted or received over the comm.
						2. Set car call door open flag to open door at floor from car call if code entered and door closed.
3/11/09 v5.47.10 mhd 	1. Added "Add_One_Code_All_Floors() and Delete_One_Code_All_Floors() routines.
6/11/09 v5.48.9 mhd		1. Use Fire 2 cancel input to cancel code entry sequence.
5/4/10 v5.52.7 mhd		1. Added input FiFS2C for rgb car call boards.
5/2/11 v5.53.18	mhd		1. Modified set_secured_fl() to OR gc_sec with s_gcc_lockouts and ccs with s_cc_lockouts so that service activation timers work properly with security car call lockouts.
						2. Added tmr_svc_en[] for group and car lockout overrides so the service timer would work properly and stay backward compatible.
8/21/13 v7.0.16 mhd 	1. Added the following changes from GALaxy 3:
						{
							 as		1. Added below Galaxy III changes made in v5.55.41 - v5.55.43
									2. Modified set_secured_fl() routine to add logic for gc_ccs_per_fl[] when the floor is secured
									3. In set_secured_fl() routine added call can be secured when (med_override == 0)
						}
7/28/15 v7.1.40 mhd		1. Added changes from GALaxy III below:
							7/06/15 v6.2.56 as		1. Modified Read_LookUpTable_Array() and Write_LookUpTable_Array() to save LookUpTable array on the correct memory location, similar to G3.
							7/27/15 v6.2.62 as		1. Modified Valid_Security_Code() routine to fix reading numcodes bytes from MRAM.
													2. Modified get_LookUpTbl_index() routine to fix offset for CodeIndex_ML.
*/
