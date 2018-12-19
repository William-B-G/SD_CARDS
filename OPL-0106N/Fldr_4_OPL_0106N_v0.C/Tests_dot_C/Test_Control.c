// Test_Control.c

#include "global.h"
#include "Tests.h"

#define SEB_io_index 12

const char *IO_Labels[] = 
{
    "i_RLScp", // 0
    "i_CSScp",
    "i_CTScp",
    "i_DETcp",
    "i_CTA  ",
    "i_INS  ", // 5
    "i_IEN  ",
    "i_IU   ",
    "i_ID   ",
    "i_FANST",
    "i_LIGST", // 10
    "i_CDL_T",
    "i_CUL_T",
    "i_CTS_T",
    "i_FFSWT",
    "i_CST_T", // 15
    "i_ICC_T",
    "i_ICA_T",
    "i_ICI_T",
    "i_ACC_T",
    "i_L15  ", // 20
    "i_L25  ",
    "i_C24_T", // 22
    "i_SWi0 ",
    "i_SWi1 ",
    "i_SWi2 ",
    "i_SWi3 ",
    "i_SWi4 ",
    "i_SWi5 ",

    "O_IFLcp",
    "O_IFBcp",
    "O_CULcp",
    "O_CDLcp",
    "O_FANcp",
    "O_LIGcp",
    "O_CSS  ",
    "O_C120T", 
    "O_L15RL",
    "O_L15_1",
    "O_FSS  ",
    "O_CST  ",
    "O_ICC  ",
    "O_ICA  ", 
    "O_ICI  ",
    "O_ACC  ",
    "O_BELL ",
    "O_CLC  ",
    "O_C24  ",
    "O_LDO6 ",
    "O_LDO7 ",
    "O_LDO8 ",
    "O_LDO9 ",
    "O_LDO10",
    "O_LDO11",       	
};


// (6) wbg 2018-9-02 OPL-0106N
const void *SEB_inputs_array[] = 
{
	i_rono ,
	i_dclnc,
	i_dclno,
	i_dolnc,
	i_dolno,
	i_heavy,
	i_nudge,
	i_close,
	i_open ,	
};


const void *SEB_0047N_inputs_array[] = 
{
    i_SWi0,
    i_SWi1,
    i_SWi2,
    i_SWi3,
    i_SWi4,
    i_SWi5,	
};

// O_Last_enum
// (6) wbg 2018-9-02 OPL-0106N
const void *SEB_outputs_array[] =
{
	O_CN19V,
	O_CN18V,
	O_CLOSE,
	O_OPEN ,
	O_NUDGE,
	O_HEAVY,
	O_DCL  ,
	O_RO   ,
	O_DOL  ,	
};

const void *SEB_1134_COP_in_array[SEB_io_index] = 
{
    i_RLS_COPi0,
    i_CSS_COPi1,
    i_CTS_COPi2,
    i_DET_COPi3,
};

const void *Extr_1134_COP_in_array[] = 
{
    i_EE_ctcan,
    i_DOL_ctcan,
    i_DCL_ctcan,
    i_SE_ctcan,
    i_DC_ctcan,
};

const void *Extr_1134_COP_out_array[] = 
{
    O_DO_ctcan,
    O_L15_ctcan,
    O_L25_ctcan,
};


const void *SEB_1134_COP_out_array[SEB_io_index] = 
{
    O_IFL_COPo0,
    O_IFB_COPo1,
    O_CUL_COPo2,
    O_CDL_COPo3,
    O_FAN_COPo4,
    O_LIG_COPo5,
};

const void *SEB_1_in_array[SEB_io_index] = 
{
	i_CTA_ctcan,
	i_INS_ctcan,
	i_IEN_ctcan,
	i_IU_ctcan,
	i_ID_ctcan,
	i_FANST_ctcan,
	i_LIGST_ctcan,
	i_CDL_T_ctcan,
	i_CUL_T_ctcan,
	i_CTS_T_ctcan,
	i_FFSWT_ctcan,
};

const void *SEB_1_out_array[SEB_io_index] = 
{
    O_CSS_ctcan,      	
};

const void *SEB_2_in_array[SEB_io_index] = 
{
	i_CST_T_ctcan,
	i_ICC_T_ctcan,
	i_ICA_T_ctcan,
	i_ICI_T_ctcan,
	i_ACC_T_ctcan,
};

const void *SEB_2_out_array[SEB_io_index] = 
{
    O_C120T_ctcan, 
    O_L15RL_ctcan,
    O_L15_T_ctcan,
    O_FSS_ctcan,
    O_CST_ctcan,
    O_ICC_ctcan,         	
};

const void *SEB_3_in_array[SEB_io_index] = 
{
	i_L15_ctcan,
	i_L25_ctcan,
};

const void *SEB_3_out_array[SEB_io_index] = 
{
    O_ICA_ctcan, 
    O_ICI_ctcan,
    O_ACC_ctcan,
    O_BELL_ctcan,
    O_CLC_ctcan,         	
};

const void *SEB_4_in_array[SEB_io_index] = 
{
	i_C24_T_ctcan,
};

const void *SEB_4_out_array[SEB_io_index] = 
{
    O_C24_ctcan,          	
};


