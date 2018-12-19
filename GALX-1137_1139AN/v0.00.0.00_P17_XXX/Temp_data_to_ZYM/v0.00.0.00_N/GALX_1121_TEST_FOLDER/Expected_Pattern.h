//  Expected_Pattern.h

// search for tagwbg 102 to see description of the following labels 
const char *FPGA_1_char[] = 
  {    // tagwbg 104 DN
       "AUTO ", "LBP  ", "GLB  ", "GBP  ",       "BAU  ", "BAD  ", "TAU  ", "TAD  ",
       "DLB  ", "DLM  ", "DLT  ", "RLM  ",       "LPS  ", "RGS  ", "GS   ", "ACC  ",  // buff0 

       "ICI  ", "CTIN ", "IU   ", "ID   ",       "MRI  ", "MRIU ", "MRID ", "ZERO1",
       "CENCP", "MCCPU", "STECP", "DNRCP",       "UPRCP", "UPFCP", "DNFCP", "ULCPU",  // buff1

       "DZCPU", "DLCPU", "DZCPU", "SP150",       "SP75 ", "SELOK", "RUNCP", "PSDF ",
       "PSUF ", "PSU  ", "PSD  ", "PDT  ",       "PUT  ", "PUN  ", "PDN  ", "PSTE ",  // buff2 

       "PMC  ", "PCEN ", "PRUN ", "PAFLT",       "DTNTS", "UTNTS", "UNNTS", "DNNTS",
       "DZNTS", "SLOKN", "S150N", "S75NT",       "NTSFT", "RERDR", "TSTMD", "ZERO2",  // buff3                   
  } ;
  
const char *FPGA_1_Look_At[] = 
  {    // tagwbg 104 DN
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",   

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",  

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",   

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",                    
  };
  
const char *FPGA_1_Look_At_Additional[] = 
  {    // tagwbg 104 DN
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",   

       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",   

       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",   

       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",                      
  };  
// search for tagwbg 103 to see description of the following labels  
const char *FPGA_2_char[] = 
  {    
       "DN   ", "DT   ", "UN   ", "UT   ",       "SD   ", "SDF  ", "SPS  ", "LOS  ",
       "TPL  ", "TPH  ", "S10  ", "GOV  ",       "HSS  ", "TSD  ", "CEN  ", "L120B",  // buff0 

       "L120 ", "BAU  ", "BAD  ", "TAU  ",       "TAD  ", "DLB  ", "DLB1 ", "DLM  ",
       "DLM1 ", "DLT  ", "DLT1 ", "RLM  ",       "RLM1 ", "TSTMD", "TSTDR", "ADD  ",  // buff1

       "RESET", "PLFLT", "NTFLT", "GS   ",       "GS1  ", "RGS  ", "RGS1 ", "ICI  ",
       "IEN  ", "RUN  ", "MRID ", "MRIE ",       "MRIU ", "MRID ", "ID   ", "IU   ",  // buff2 

       "CTIN ", "CST  ", "CFLT ", "SPD  ",       "MTO  ", "MC   ", "AUTO ", "AD   ",
       "IND  ", "LBP  ", "GLB  ", "GBP  ",       "MRSW ", "SUF  ", "SU   ", "DZ   ",  // buff3                    
  };  
  
  const char *FPGA_2_Look_At[] = 
  {    // tagwbg 104 DN
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",   

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",  

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",   

       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",
       "ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx","ICxxx",                  
  };
  
const char *FPGA_2_Look_At_Additional[] = 
  {    // tagwbg 104 DN
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",   

       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",   

       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",   

       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",
       "IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ","IC-XyZ",                   
  };

//////////////// DN
const int16 Exp_iDN_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3140
    0x0 , 0x0 , 0x200 , 0x31B0
};

const int16 Exp_iDN_FPGA_2[4] =
{
// (short)0x8021 , (short)0x8006 ,(short)0xE040 , (short)0x4108
   (short)0x8021 , (short)0x8006 ,(short)0xE040 , (short)0x4188
};


//////////////// DT
const int16 Exp_iDT_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3840
    0x0 , 0x0 , 0x200 , 0x38B0
};

const int16 Exp_iDT_FPGA_2[4] =
{
//  (short)0x4021 , (short)0x8006 , (short)0xE040 , (short)0x4108
    (short)0x4021 , (short)0x8006 , (short)0xE040 , (short)0x4188
};


//////////////// UN
const int16 Exp_iUN_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3240
    0x0 , 0x0 , 0x200 , 0x32B0
};

const int16 Exp_iUN_FPGA_2[4] =
{
//  (short)0x2021 , (short)0x8006 , (short)0xE040 , (short)0x4108
    (short)0x2021 , (short)0x8006 , (short)0xE040 , (short)0x4188
};


//////////////// UT
const int16 Exp_iUT_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3440
    0x20, 0x0 , 0x200 , 0x34B0
};

const int16 Exp_iUT_FPGA_2[4] =
{
//  (short)0x1025 , (short)0x8006 , (short)0xE040 , (short)0x4108
    (short)0x0021 , (short)0x8066 , (short)0xE040 , (short)0x4188
};


//////////////// CEN
const int16 Exp_iCEN_FPGA_1[4] =
{
//  0x0 , 0x0080 , 0x0200 , 0x7040
    0x0 , 0x0080 , 0x0200 , 0x70B0
};

const int16 Exp_iCEN_FPGA_2[4] =
{
//  (short)0x0023 , (short)0x8006 , (short)0xE040 , (short)0x4108
    (short)0x0023 , (short)0x8006 , (short)0xE040 , (short)0x4188
};
 

//////////////// MC
const int16 Exp_iMC_FPGA_1[4] =
{
//  0x0 , 0x40 , 0x200 , 0x3040
//  0x0 , 0x40 , 0x200 , 0x30B0
    0x0 , 0x40 , 0x200 , 0x2000
};

const int16 Exp_iMC_FPGA_2[4] =
{
//   (short)0x021 , (short)0x8006 , (short)0xE040 , (short)0x4508
//   (short)0x021 , (short)0x8006 , (short)0xE040 , (short)0x4588
     (short)0x021 , (short)0x8000 , (short)0x8040 , (short)0x4508
};

//////////////// MRSW
const int16 Exp_iMRSW_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3040
//  0x0 , 0x0 , 0x200 , 0x30B0
    0x0 , 0x0, 0x200 , 0x2000
};

const int16 Exp_iMRSW_FPGA_2[4] =
{
//  (short)0x021 , (short)0x8006 , (short)0xE040 , (short)0x4108
//  (short)0x021 , (short)0x8006 , (short)0xE040 , (short)0x4188
    (short)0x021 , (short)0x8000 , (short)0x8040 , (short)0x4108
};

//////////////// SUF
const int16 Exp_iSUF_FPGA_1[4] =
{
//  0x0 , 0x4 , 0x200 , 0x3440
    0x0 , 0x4 , 0x200 , 0x2400
};

const int16 Exp_iSUF_FPGA_2[4] =
{
//  (short)0x1025 , (short)0x8006 , (short)0xE040 , (short)0x410C
    (short)0x1025 , (short)0x8000 , (short)0x8040 , (short)0x410C
};


//////////////// SU
const int16 Exp_iSU_FPGA_1[4] =
{
//  0x0 , 0x8 , 0x200 , 0x3240
    0x0 , 0x8 , 0x200 , 0x2200
};

const int16 Exp_iSU_FPGA_2[4] =
{
//  (short)0x2021 , (short)0x8006 , (short)0xE040 , (short)0x410A
    (short)0x2021 , (short)0x8000 , (short)0x8040 , (short)0x410A
};

//////////////// SD
const int16 Exp_iSD_FPGA_1[4] =
{
//  0x0 , 0x10 , 0x200 , 0x3140
//  0x0 , 0x10 , 0x200 , 0x31B0
    0x0 , 0x10 , 0x200 , 0x2100
};

const int16 Exp_iSD_FPGA_2[4] =
{
//  (short)0x8821 , (short)0x8006 , (short)0xE040 , (short)0x4108
//  (short)0x8821 , (short)0x8006 , (short)0xE040 , (short)0x4188
    (short)0x8821 , (short)0x8000 , (short)0x8040 , (short)0x4108
};

//////////////// SDF
const int16 Exp_iSDF_FPGA_1[4] =
{
//  0x0 , 0x2 , 0x200 , 0x3840
//  0x0 , 0x2 , 0x200 , 0x38B0
    0x0 , 0x2 , 0x200 , 0x2800
};

const int16 Exp_iSDF_FPGA_2[4] =
{
//  (short)0x4421 , (short)0x8006 , (short)0xE040 , (short)0x4108
//  (short)0x4421 , (short)0x8006 , (short)0xE040 , (short)0x4188
    (short)0x4421 , (short)0x8000 , (short)0x8040 , (short)0x4108
};

//////////////// GBP
const int16 Exp_iGBP_FPGA_1[4] =
{
    0x1000 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_iGBP_FPGA_2[4] =
{
    (short)0x0021 , (short)0x8006 , (short)0xE040 , (short)0x4118
};

//////////////// GLB
const int16 Exp_iGLB_FPGA_1[4] =
{
    (short)0xA000 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_iGLB_FPGA_2[4] =
{
    (short)0x0021 , (short)0x8006 , (short)0xE040 , (short)0x4328
};

//////////////// LBP
const int16 Exp_iLBP_FPGA_1[4] =
{
    0x4000 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_iLBP_FPGA_2[4] =
{
    (short)0x0021 , (short)0x8006 , (short)0xE040 ,(short)0x4148
};

//////////////// IND
const int16 Exp_iIND_FPGA_1[4] =
{
    (short)0xA000 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_iIND_FPGA_2[4] =
{
    0x0021 , (short)0x8006 , (short)0xE040 , 0x43A8
};

//////////////// AD
const int16 Exp_iAD_FPGA_1[4] =
{
    (short)0xA000 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_iAD_FPGA_2[4] =
{
    0x0021 , (short)0x8006 , (short)0xE040 , 0x4328
};

//////////////// AUTO
const int16 Exp_iAUTO_FPGA_1[4] =
{
    (short)0xA000 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_iAUTO_FPGA_2[4] =
{
    0x0021 , (short)0x08006 , (short)0xE040 , 0x4328
};

//////////////// MRI
const int16 Exp_iMRIN_FPGA_1[4] =
{
    0x2000 , 0x800 , 0x200 , 0x3040
};

const int16 Exp_iMRIN_FPGA_2[4] =
{
    0x21 , (short)0x8006 , (short)0xE044 , 0x4128
};
//////////////// MRIE
const int16 Exp_iMRIE_FPGA_1[4] =
{
    0x2000 , 0x800 , 0x200 , 0x3040
};

const int16 Exp_iMRIE_FPGA_2[4] =
{
    0x21 , (short)0x8006 , (short)0xE054 , 0x4128
};

//////////////// MRID
const int16 Exp_iMRID_FPGA_1[4] =
{
    0x2000 , 0xA00 , 0x200 , 0x3040
};

const int16 Exp_iMRID_FPGA_2[4] =
{
    0x0021 , (short)0x8006 , (short)0xE074 , 0x4128
};

//////////////// MRIU
const int16 Exp_iMRIU_FPGA_1[4] =
{
    0x2000 , 0xC00 , 0x200 , 0x3040
};

const int16 Exp_iMRIU_FPGA_2[4] =
{
    0x0021 , (short)0x8006 ,(short)0xE05C , 0x4128
};

//////////////// TSD
const int16 Exp_iTSD_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3040
//  0x20, 0x0 , 0x200 , 0x30B0
    0x00, 0x0 , 0x200 , 0x2000
};

const int16 Exp_iTSD_FPGA_2[4] =
{
//  0x0025 , (short)0x8006 , (short)0xE040 , 0x4108
//  0x0021 , (short)0x8066 , (short)0xE040 , 0x4188
    0x0025 , (short)0x8000 , (short)0x8040 , 0x4108
};

//////////////// DLT
const int16 Exp_kDLT_FPGA_1[4] =
{
//  0x20 , 0x0 , 0x200 , 0x3040
    0x20 , 0x0 , 0x000 , 0x0
};

const int16 Exp_kDLT_FPGA_2[4] =
{
//  0x0021 , (short)0x8066 , (short)0xE040 , 0x4108
    0x0021 , (short)0x8060 , (short)0x8000 , 0x0100
};

//////////////// RLM
const int16 Exp_kRLM_FPGA_1[4] =
{ // seb board 7
//  0x10 , 0x0 , 0x200 , 0x3040
    0x10 , 0x0 , 0x000 , 0x0
};

const int16 Exp_kRLM_FPGA_2[4] =
{
//  0x0021 , (short)0x801E , (short)0xE040 , 0x4108
    0x0021 , (short)0x8018 , (short)0x8000 , 0x0100
};


//////////////// DLM
const int16 Exp_kDLM_FPGA_1[4] =
{
    0x40 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_kDLM_FPGA_2[4] =
{
    0x0021 , (short)0x8186, (short)0xE040 , 0x4108
};


//////////////// DLB
const int16 Exp_kDLB_FPGA_1[4] =
{
    0x80 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_kDLB_FPGA_2[4] =
{
    0x0021 , (short)0x8606, (short)0xE040 , 0x4108
};

//////////////// LPS
const int16 Exp_kLPS_FPGA_1[4] =
{
//  0x8, 0x0 , 0x200 , 0x3040
    0x8, 0x0 , 0x000 , 0x10F0
};

const int16 Exp_kLPS_FPGA_2[4] =
{
//  0x0221 , (short)0x8006, (short)0xE040 , 0x4108
    0x0221 , (short)0x8006, (short)0xE000 , 0x0180
};

//////////////// LOS
const int16 Exp_kLOS_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3040
    0x0 , 0x0 , 0x000 , 0x10F0
};

const int16 Exp_kLOS_FPGA_2[4] =
{
//  0x0121 , (short)0x8006, (short)0xE040 , 0x4108
    0x0121 , (short)0x8006, (short)0xE000 , 0x0180
};

//////////////// TPL - ??? Passes even if not connected !
const int16 Exp_kTPL_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3040
    0x0 , 0x0 , 0x000 , 0x10F0
};

const int16 Exp_kTPL_FPGA_2[4] =
{
//  0x0021 , (short)0x8006, (short)0xE040 , 0x4108
    0x0021 , (short)0x8006, (short)0xE000 , 0x0180
};

//////////////// TPH
const int16 Exp_kTPH_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3040
    0x0 , 0x0 , 0x000 , 0x0
};

const int16 Exp_kTPH_FPGA_2[4] =
{
//  0x0061 , (short)0x8006, (short)0xE040 , 0x4108
    0x0061 , (short)0x8000, (short)0x8000 , 0x0100
};

//////////////// TLH
const int16 Exp_kTLH_H21_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x000 , 0x10B0
    0x0 , 0x0 , 0x000 , 0x0
};

const int16 Exp_kTLH_H21_FPGA_2[4] =
{
//  0x0021 , (short)0x8006, (short)0xE000 , 0x0180
    0x0021 , (short)0x8000, (short)0x8000 , 0x0100
};

//////////////// PS
const int16 Exp_kPS_H19_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x000 , 0x10B0
    0x0 , 0x0 , 0x000 , 0x0
};

const int16 Exp_kPS_H19_FPGA_2[4] =
{
//  0x0021 , (short)0x8006, (short)0xE000 , 0x0180
    0x0021 , (short)0x8000, (short)0x8000 , 0x0100
};

//////////////// S10
const int16 Exp_kS10_H24_FPGA_1[4] =
{
    0x0 , 0x0 , 0x200 , 0x3040
};

const int16 Exp_kS10_H24_FPGA_2[4] =
{
    0x0021 , (short)0x8006, (short)0xE040 , 0x4108
};

//////////////// GOV -H23
const int16 Exp_kGOV_FPGA_1[4] =
{
//  0x0 , 0x0 , 0x200 , 0x3040
    0x00 , 0x0 , 0x000 , 0x10B0
};

const int16 Exp_kGOV_FPGA_2[4] =
{
//  0x0021 , (short)0x8006, (short)0xE040 , 0x4108
    0x0021 , (short)0x8006, (short)0xE000 , 0x0180
};

//////////////// HSS -H17
const int16 Exp_kHSS_FPGA_1[4] =
{
//  0x80 , 0x0 , 0x200 , 0x3040
    0x00 , 0x0 , 0x000 , 0x10B0
};

const int16 Exp_kHSS_FPGA_2[4] =
{
//  0x0021 , (short)0x8606, (short)0xE040 , 0x4108
    0x0039 , (short)0x8006, (short)0xE000 , 0x0180
};

//////////////// ACCESS CONNECTION - BAU
const int16 Exp_iBAU_FPGA_1[4] =
{
//  0x800 , 0x0, 0x200 , 0x3040
//  0x800 , 0x0, 0x000 , 0x10B0
    0x800 , 0x0, 0x000 , 0x0000
};

const int16 Exp_iBAU_FPGA_2[4] =
{
//  0x0021 , (short)0xC006, (short)0xE040 , 0x4108
//  0x0021 , (short)0xC006, (short)0xE000 , 0x0180
    0x0021 , (short)0xC000, (short)0x8000 , 0x0100
};

//////////////// ACCESS CONNECTION - BAD
const int16 Exp_iBAD_FPGA_1[4] =
{
//  0x400 , 0x0, 0x200 , 0x3040
//  0x400 , 0x0, 0x0   , 0x10B0
    0x400 , 0x0, 0x0   , 0x0000
};

const int16 Exp_iBAD_FPGA_2[4] =
{
//  0x0021 , (short)0xA006, (short)0xE040 , 0x4108
//  0x0021 , (short)0xA006, (short)0xE000 , 0x0180 
    0x0021 , (short)0xA000, (short)0x8000 , 0x0100
};

//////////////// ACCESS CONNECTION - TAU
const int16 Exp_iTAU_FPGA_1[4] =
{
//  0x200 , 0x0, 0x200 , 0x3040
//  0x200 , 0x0, 0x000 , 0x10B0
    0x200 , 0x0, 0x000 , 0x0000
};

const int16 Exp_iTAU_FPGA_2[4] =
{
//  0x0021 , (short)0x9006, (short)0xE040 , 0x4108
//  0x0021 , (short)0x9006, (short)0xE000 , 0x0180
    0x0021 , (short)0x9000, (short)0x8000 , 0x0100
};

//////////////// ACCESS CONNECTION - TAD
const int16 Exp_iTAD_FPGA_1[4] =
{
//  0x100 , 0x0, 0x200 , 0x3040
//  0x100 , 0x0, 0x000 , 0x10B0
    0x100 , 0x0, 0x000 , 0x0000
};

const int16 Exp_iTAD_FPGA_2[4] =
{
//  0x0021 , (short)0x8806, (short)0xE040 , 0x4108
//  0x0021 , (short)0x8806, (short)0xE000 , 0x0180
    0x0021 , (short)0x8800, (short)0x8000 , 0x0100
};
/*
//////////////// TRAVELING CABLE CONNECTION - CST
const int16 Exp_iCST_FPGA_1[4] =
{
    0x100 , 0x0, 0x200 , 0x3040
};

const int16 Exp_iCST_FPGA_2[4] =
{
    0x0021 , (short)0x8806, (short)0xE040 , 0x4108
};
*/
//////////////// SOFT STARTER CONNECTION - CFLT
const int16 Exp_iCFLT_FPGA_1[4] =
{ // seb board 7
//  0x0 , 0x0, 0x200 , 0x3040
//  0x0 , 0x0, 0x000 , 0x0000
    0x0 , 0x0, 0x000 , 0x0000
};

const int16 Exp_iCFLT_FPGA_2[4] =
{
//  0x0021 , (short)0x8006, (short)0xE040 , 0x6188
//  0x0021 , (short)0x8006, (short)0xE000 , 0x2180
    0x0021 , (short)0x8000, (short)0x8000 , 0x2100
};

//////////////// SOFT STARTER CONNECTION - SPD
const int16 Exp_iSPD_FPGA_1[4] =
{ // seb board 7
//  0x0 , 0x0, 0x200 , 0x3040
    0x0 , 0x0, 0x000 , 0x0
};
 
const int16 Exp_iSPD_FPGA_2[4] =
{
//  0x0021 , (short)0x8006, (short)0xE040 , 0x5188
    0x0021 , (short)0x8000, (short)0x8000 , 0x1100
};

//////////////// SOFT STARTER CONNECTION - MTO
const int16 Exp_iMTO_FPGA_1[4] =
{
//  0x0 , 0x0, 0x200 , 0x3040
    0x0 , 0x0, 0x000 , 0x0
};

const int16 Exp_iMTO_FPGA_2[4] =
{
//  0x0021 , (short)0x8006, (short)0xE040 , 0x4988
    0x0021 , (short)0x8000, (short)0x8000 , 0x0900
};

//////////////// TRAVELING CABLE CONNECTION - INS
const int16 Exp_iINS_FPGA_1[4] =
{
    0x100 , 0x0, 0x200 , 0x3040
};

const int16 Exp_iINS_FPGA_2[4] =
{
    0x0021 , (short)0x8806, (short)0xE040 , 0x4108
};

//  WBG step 02