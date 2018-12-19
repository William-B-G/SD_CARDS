
#define MAIN_CPU 1
#define SPB_CPU 2
#define NTS_CPU 3


extern int16 galcom_req[2];
extern int16 com_car_req[2];
extern int16 com_rst_flt_ix[2];

extern void galcom (int16 com_nmb);
extern void send_galcom_fvar (int16 fvar_cpu, int16 fvar_ix);
