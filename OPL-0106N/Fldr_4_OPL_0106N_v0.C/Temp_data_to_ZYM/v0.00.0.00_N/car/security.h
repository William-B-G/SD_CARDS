// Secuity variable


extern int16 Security_code_flag;	// Security flag 0 waiting 1 entering code
								// 2 checking code 3 waiting to have nocalls

struct CdTable{
	int16 ValidTable;
	int16 NumCode;
	int16 CheckSum;
	unsigned char Codes[NumMaxCd][4];
};

extern struct CdTable CodeTable; 

extern int16 LookUpTable[NumMaxFl];


// Check sum and count code table to keep tract of changes in the table
struct Cd_Tbl_Chk{
	int16 NumCode;
	int16 CheckSum;
};

extern struct Cd_Tbl_Chk Code_Tbl_Chk[fl_size];

//Security Code  Function
extern void Read_CodeTbl(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[]);	// Read Code table
extern void Write_CodeTbl(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[]);	  // Write Code Table
//lint -esym(534,Add_One_Code_All_Floors)
extern int16 Add_One_Code_All_Floors(int16 start_flag, int16 wherefrom, unsigned char Code_Num[]);
//lint -esym(534,Delete_One_Code_All_Floors)
extern int16 Delete_One_Code_All_Floors(int16 start_flag, int16 wherefrom, unsigned char Code_Num[]);
//lint -esym(534,Clear_All_Codes_All_Floors)
extern int16 Clear_All_Codes_All_Floors(int16 start_flag, int16 wherefrom);		// Clear all Codes  from memory
//lint -esym(534,Add_Security_Code)
extern int16 Add_Security_Code(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[], unsigned char Code_Num[]);
//lint -esym(534,Delete_Security_Code)
extern int16 Delete_Security_Code(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[], unsigned char Code_Num[]);
extern void Clear_All_Codes_One_Floor(int16 FloorNum, struct CdTable *ctp, int16 LkUpTbl[]);
extern int16 Verify_Code_Check_Table (struct Cd_Tbl_Chk *ckp);
extern void Init_Code_Check_Table (void);
extern int16 chk_push_button(void );

extern uint8 ccpbs_fl_access_req;
extern uint8 ccpbs_codes[4];
