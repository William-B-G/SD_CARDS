
extern float ins_vel;
extern uint8 ins_servf;

#define c_INV_INS 0
#define c_CT_INS 1
#define c_MR_INS 2
#define c_ACC_INS 3
#define c_IC_INS 4
#define c_CT_LBP_INS 5
#define c_CT_GBP_INS 6
#define c_CT_LBP_GBP_INS 7
#define c_SEL_SETUP_INS 8

extern int chk_ins(void);
extern int chk_ins_sw(void);
extern int access_run_req (void);
extern void inspection(void );
extern void ins_run(int cmd);

