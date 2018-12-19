extern int16 bkt_cmd;			// Brake command
extern int16 bkt_auto_test;		// Brake auto test variable (used in LCD)
extern int16 bkt_test_seq;		// Brake test sequence
extern int16 bkt_run_cmd;		// Brake run command: 1 to run up and 2 to run down
extern int16 bkt_run_err;		// Error occured during Brake run
extern float bkt_run_vel;		// Brake run velocity
extern float bkt_max_speed;		// Brake max speed



extern void Brake_Test(void);
extern void bktest_run_cmd(int16 cmd);
extern void Run_Brake_Test (void);
