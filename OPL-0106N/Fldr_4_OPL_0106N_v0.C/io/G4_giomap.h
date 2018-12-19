

typedef struct gio {
	uint8 array;
	uint8 bit;
} gio_type;

struct gio i_DC[61] = {
	
  {HCBRD1A,		BIT1,"   2D", 0x01},               // I/O #1
  {HCBRD1A,		BIT2,"   3D", 0x01},               // I/O #2
};

struct gio i_UC[61] = {

  {HCBRD1B,		BIT5,"   1U", 0x01},               // I/O #13
  {HCBRD1B,		BIT6,"   2U", 0x01},               // I/O #14
};

struct gio i_DCR[61] = {
  {HCBRD4A,		BIT1,"  2DR", 0x01},               // I/O #73
  {HCBRD4A,		BIT2,"  3DR", 0x01},               // I/O #74
};


struct gio i_UR[61] = {
  {HCBRD4B,		BIT5,"  1UR", 0x01},               // I/O #85
  {HCBRD4B,		BIT6,"  2UR", 0x01},               // I/O #86
};

struct gio i_DI[61] = {
  {HCBRD6A,		BIT1,"  2DI", 0x01},               // I/O #121
  {HCBRD6A,		BIT2,"  3DI", 0x01},               // I/O #122
};

struct gio i_UI[61] = {
  {HCBRD6B,		BIT5,"  1UI", 0x01},               // I/O #133
  {HCBRD6B,		BIT6,"  2UI", 0x01},               // I/O #134
};


struct gio i_DIR[61] = {
  {HCBRD9A,		BIT1," 2DIR", 0x01},               // I/O #193
  {HCBRD9A,		BIT2," 3DIR", 0x01},               // I/O #194
};

struct gio i_UIR[61] = {		 
  {HCBRD9B,		BIT5," 1UIR", 0x01},               // I/O #205
  {HCBRD9B,		BIT6," 2UIR", 0x01},               // I/O #206
};



// HC CARD # 11
struct gio i_CB[61] = {		 
  {HCBRD11A,	BIT1,"  1CB", 0x01},               // I/O #241
  {HCBRD11A,	BIT2,"  2CB", 0x01},               // I/O #242
};


struct gio i_CBR[61] = {		 
  {HCBRD13A,	BIT1," 1CBR", 0x01},               // I/O #289 
  {HCBRD13A,	BIT2," 2CBR", 0x01},               // I/O #290
};
   
struct gio i_GCS[61] = {		 
   // HC CARD # 14		 Default is security_type +2
  {HCBRD14A,	BIT1,"GCSEC", 0x04},               // I/O #313
  {HCBRD14A,	BIT2," 1GCS", 0x01},               // I/O #314
};

struct gio i_GRS[61] = {		 
// HC CARD # 16
  {HCBRD16A,	BIT1,"RGSEC", 0x00},               // I/O #361
  {HCBRD16A,	BIT2," 1GRS", 0x01},               // I/O #362
};

struct gio i_GIO[61] = {		 
// HC CARD # 17
  {HCBRD17A,	BIT1,"  IO1", 0x04},               // I/O #385
  {HCBRD17A,	BIT2,"  IO2", 0x04},               // I/O #386
  {HCBRD43C,	BIT8,"IO192", 0x04},               // I/O #1032
};




struct gio i_CBS[61] = {		 
// HC CARD # 53
  {HCBRD53A,	BIT1," 1CBS", 0x01},               // I/O #1249
  {HCBRD53A,	BIT2," 2CBS", 0x01},               // I/O #1250
};

struct gio i_CRS[61] = {		 
// HC CARD # 56
  {HCBRD56A,	BIT1," 1CRS", 0x01},               // I/O #1321  
  {HCBRD56A,	BIT2," 2CRS", 0x01},               // I/O #1322
};


struct gio i_D2I[61] = {		 
// HC CARD # 9
  {HCBRD9A,		BIT1," 2D2I", 0x01},               // I/O #193
  {HCBRD9A,		BIT2," 3D2I", 0x01},               // I/O #194
;

struct gio i_U2I[61] = {		 
  {HCBRD9B,		BIT5," 1U2I", 0x01},               // I/O #205
  {HCBRD9B,		BIT6," 2U2I", 0x01},               // I/O #206
};

const struct hcio grp_io_ser[360] = {

  {HCBRD1A,		BIT2,"   1U", 0x01},               // I/O #2
  {HCBRD1A,		BIT3,"   2D", 0x01},               // I/O #3
  {HCBRD1A,		BIT4,"   2U", 0x01},               // I/O #4
  {HCBRD1A,		BIT5,"   3D", 0x01},               // I/O #5
  {HCBRD1A,		BIT6,"   3U", 0x01},               // I/O #6

  {HCBRD4A,		BIT2,"  1UR", 0x01},               // I/O #74
  {HCBRD4A,		BIT3,"  2DR", 0x01},               // I/O #75
  {HCBRD4A,		BIT4,"  2UR", 0x01},               // I/O #76
  {HCBRD4A,		BIT5,"  3DR", 0x01},               // I/O #77
  {HCBRD4A,		BIT6,"  3UR", 0x01},               // I/O #78
  
  {HCBRD6A,		BIT2,"  1UI", 0x01},               // I/O #122
  {HCBRD6A,		BIT3,"  2DI", 0x01},               // I/O #123
  {HCBRD6A,		BIT4,"  2UI", 0x01},               // I/O #124
  {HCBRD6A,		BIT5,"  3DI", 0x01},               // I/O #125
  {HCBRD6A,		BIT6,"  3UI", 0x01},               // I/O #126

  {HCBRD9A,		BIT2," 1UIR", 0x01},               // I/O #194
  {HCBRD9A,		BIT3," 2DIR", 0x01},               // I/O #195
  {HCBRD9A,		BIT4," 2UIR", 0x01},               // I/O #196
  {HCBRD9A,		BIT5," 3DIR", 0x01},               // I/O #197
  {HCBRD9A,		BIT6," 3UIR", 0x01},               // I/O #198

  {HCBRD45A,	BIT1,"     ", 0x00},               // I/O #529
  {HCBRD45A,	BIT2,"  1UB", 0x01},               // I/O #530
  {HCBRD45A,	BIT3,"  2DB", 0x01},               // I/O #531
  {HCBRD45A,	BIT4,"  2UB", 0x01},               // I/O #532
  {HCBRD45A,	BIT5,"  3DB", 0x01},               // I/O #533
  {HCBRD45A,	BIT6,"  3UB", 0x01},               // I/O #534


  {HCBRD48A,	BIT2," 1URB", 0x01},               // I/O #602
  {HCBRD48A,	BIT3," 2DRB", 0x01},               // I/O #603
  {HCBRD48A,	BIT4," 2URB", 0x01},               // I/O #604
  {HCBRD48A,	BIT5," 3DRB", 0x01},               // I/O #605
  {HCBRD48A,	BIT6," 3URB", 0x01},               // I/O #606
};


#if (Tract_HR == 1)
extern  const struct hcio xhcasgn_io[120] = {
// HC CARD # 9
  {HCBRD9A,		BIT1,"  2XD", 0x01},               // I/O #193
  {HCBRD9A,		BIT2,"  3XD", 0x01},               // I/O #194
  {HCBRD9B,		BIT5,"  1XU", 0x01},               // I/O #205
  {HCBRD9B,		BIT6,"  2XU", 0x01},               // I/O #206
  {HCBRD9B,		BIT7,"  3XU", 0x01},               // I/O #207
};

// security_type +4
extern  const struct hcio hcsec_io[72] = {
  {HCBRD16A,	BIT1,"     ", 0x00},               // I/O #361
  {HCBRD16A,	BIT2," 1HCS", 0x01},               // I/O #362
  {HCBRD16A,	BIT3," 2HCS", 0x01},               // I/O #363
};

// Security_type +8
extern  const struct hcio flsec_io[144] = {

   // HC CARD # 14
  {HCBRD14A,	BIT1," FSEC", 0x04},               // I/O #313
  {HCBRD14A,	BIT2," 1FLS", 0x01},               // I/O #314

// HC CARD # 16
  {HCBRD16A,	BIT1,"RFSEC", 0x04},               // I/O #361
  {HCBRD16A,	BIT2," 1RFS", 0x01},               // I/O #362
};


// Security_type +32
extern  const struct  hcio shcsec_io[144] = {

// HC CARD # 17
  {HCBRD17A,	BIT1," HSEC", 0x04},               // I/O #385
  {HCBRD17A,	BIT2," 1HCS", 0x01},               // I/O #386
  {HCBRD17A,	BIT3," 2HCS", 0x01},               // I/O #387

// HC CARD # 19
  {HCBRD19A,	BIT1,"RHSEC", 0x04},               // I/O #433
  {HCBRD19A,	BIT2," 1RHS", 0x01},               // I/O #434
  {HCBRD19A,	BIT3," 2RHS", 0x01},               // I/O #435
};	    
// Security_type +128
extern  const struct  hcio gsecovr_io[144] = {

// HC CARD # 17
  {HCBRD17A,	BIT1," GSOR", 0x04},               // I/O #385
  {HCBRD17A,	BIT2," 1GOC", 0x01},               // I/O #386
  {HCBRD17A,	BIT3," 2GOC", 0x01},               // I/O #387
  {HCBRD17A,	BIT4," 3GOC", 0x01},               // I/O #388
// HC CARD # 19
  {HCBRD19A,	BIT1,"RGSOR", 0x04},               // I/O #433
  {HCBRD19A,	BIT2," 1RGO", 0x01},               // I/O #434
  {HCBRD19A,	BIT3," 2RGO", 0x01},               // I/O #435
};	    

extern  const struct hcio vip_io[144] = {

// HC CARD # 11
  {HCBRD11A,	BIT1,"  1VP", 0x01},               // I/O #241
  {HCBRD11A,	BIT2,"  2VP", 0x01},               // I/O #242
// HC CARD # 13
  {HCBRD13A,	BIT1," 1VPR", 0x01},               // I/O #289 
  {HCBRD13A,	BIT2," 2VPR", 0x01},               // I/O #290
};


extern  const struct hcio vip_wcb_io[72] = {

// HC CARD # 13
  {HCBRD13A,	BIT1,"  1VP", 0x01},               // I/O #289 
  {HCBRD13A,	BIT2,"  2VP", 0x01},               // I/O #290
};


extern  const struct hcio emp_io[24] = {

// HC CARD # 20
  {HCBRD20A,	BIT1," 1PRK", 0x00},               // I/O #457
  {HCBRD20A,	BIT2," 2PRK", 0x00},               // I/O #458
  {HCBRD20A,	BIT3," 3PRK", 0x00},               // I/O #459
  {HCBRD20A,	BIT4," 4PRK", 0x00},               // I/O #460
  {HCBRD20A,	BIT5," 5PRK", 0x00},               // I/O #461
  {HCBRD20A,	BIT6," 6PRK", 0x00},               // I/O #462
  {HCBRD20A,	BIT7," 7PRK", 0x00},               // I/O #463
  {HCBRD20A,	BIT8," 8PRK", 0x00},               // I/O #464
  {HCBRD20B,	BIT1,"  1OP", 0x00},               // I/O #465
  {HCBRD20B,	BIT2,"  2OP", 0x00},               // I/O #466
  {HCBRD20B,	BIT3,"  3OP", 0x00},               // I/O #467
  {HCBRD20B,	BIT4,"  4OP", 0x00},               // I/O #468
  {HCBRD20B,	BIT5,"  5OP", 0x00},               // I/O #469
  {HCBRD20B,	BIT6,"  6OP", 0x00},               // I/O #470
  {HCBRD20B,	BIT7,"  7OP", 0x00},               // I/O #471
  {HCBRD20B,	BIT8,"  8OP", 0x00},               // I/O #472
  {HCBRD20C,	BIT1," 1SEL", 0x00},               // I/O #473
  {HCBRD20C,	BIT2," 2SEL", 0x00},               // I/O #474
  {HCBRD20C,	BIT3," 3SEL", 0x00},               // I/O #475
  {HCBRD20C,	BIT4," 4SEL", 0x00},               // I/O #476 
  {HCBRD20C,	BIT5," 5SEL", 0x00},               // I/O #477
  {HCBRD20C,	BIT6," 6SEL", 0x00},               // I/O #478
  {HCBRD20C,	BIT7," 7SEL", 0x00},               // I/O #479
  {HCBRD20C,	BIT8," 8SEL", 0x00},               // I/O #480
};

extern  const struct hcio ex_io[24] = {

// HC CARD # 40
  {HCBRD20A,	BIT1,"  ICR", 0x00},               // I/O #481/1033
  {HCBRD20A,	BIT2," ICR2", 0x00},               // I/O #482/1034
  {HCBRD20A,	BIT3," RICR", 0x00},               // I/O #483/1035
  {HCBRD20A,	BIT4,"     ", 0x00},               // I/O #484/1036
  {HCBRD20A,	BIT5,"  LUC", 0x00},               // I/O #485/1037
  {HCBRD20A,	BIT6,"CASEC", 0x00},               // I/O #486/1038
  {HCBRD20A,	BIT7," APRK", 0x00},               // I/O #487/1039
  {HCBRD20A,	BIT8,"  NCU", 0x00},               // I/O #488/1040
  {HCBRD20B,	BIT1," ALBY", 0x00},               // I/O #489/1041
  {HCBRD20B,	BIT2,"RECEN", 0x00},               // I/O #490/1042
  {HCBRD20B,	BIT3,"RUNEN", 0x00},               // I/O #491/1043
  {HCBRD20B,	BIT4,"RUNCT", 0x00},               // I/O #492/1044
  {HCBRD20B,	BIT5,"     ", 0x00},               // I/O #493/1045
  {HCBRD20B,	BIT6,"     ", 0x00},               // I/O #494/1046
  {HCBRD20B,	BIT7,"     ", 0x00},               // I/O #495/1047
  {HCBRD20B,	BIT8,"     ", 0x00},               // I/O #496/1048
  {HCBRD20C,	BIT1,"SECF1", 0x00},               // I/O #497/1049
  {HCBRD20C,	BIT2,"SECF2", 0x00},               // I/O #498/1050
  {HCBRD20C,	BIT3,"SECF3", 0x00},               // I/O #499/1051
  {HCBRD20C,	BIT4,"     ", 0x00},               // I/O #500/1052 
  {HCBRD20C,	BIT5,"     ", 0x00},               // I/O #501/1053
  {HCBRD20C,	BIT6,"     ", 0x00},               // I/O #502/1054
  {HCBRD20C,	BIT7,"     ", 0x00},               // I/O #503/1055
  {HCBRD20C,	BIT8,"     ", 0x00},               // I/O #504/1056
};

