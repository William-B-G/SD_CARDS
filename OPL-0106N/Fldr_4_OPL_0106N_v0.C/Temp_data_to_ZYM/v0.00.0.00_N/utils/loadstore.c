// This file contains the user access to load and store program files
#define d_LOADSTORE 1
#include "global.h"

#include "fat_filelib.h"

uint16 Load_Store_Program(uint16 cmd);
void put_car_faults (int16 car,FL_FILE *fp1);
void put_car_lt_faults (FL_FILE *fp1);
void put_car_stats (FL_FILE *fp1);
void put_car_data (FL_FILE *fp1);
int16 get_car_data (FL_FILE *fp1);
int16 get_valid_code(FL_FILE *fp1);
void Set_Device_Name(char * dev_name, int16 code, uint8 dev);

int16 file_car_nmb = 0;
int16 file_car_speed = 0;
int16 file_nmb_cars = 0;
int16 file_bottom_fl = 0;
int16 file_top_fl = 0;
int16 file_grp_top_fl = 0;
int16 file_last_fvar = 0;

char file_string[181];
char fdatbuf[181];

const char call_status_dpy[20][51]={
	"Number of Car Calls",                            /* lvars[0] */
	"Number of Up Calls",                             /* lvars[1] */
	"Number of Down Calls",                           /* lvars[2] */
	"Number of Up Calls Less than 15 seconds =",      /* lvars[3] */
	"Number of Up Calls Less than 30 seconds =",      /* lvars[4] */
	"Number of Up Calls Less than 45 seconds =",      /* lvars[5] */
	"Number of Up Calls Less than 60 seconds =",      /* lvars[6] */
	"Number of Up Calls More than 60 seconds =",      /* lvars[7] */
	"Number of Down Calls Less than 15 seconds =",    /* lvars[8] */
	"Number of Down Calls Less than 30 seconds =",    /* lvars[9] */
	"Number of Down Calls Less than 45 seconds =",    /* lvars[10] */
	"Number of Down Calls Less than 60 seconds =",    /* lvars[11] */
	"Number of Down Calls More than 60 seconds =",    /* lvars[12] */
};


int16 fvarstoSD(int16 command, int16 force_write);

char Field_Vars_Unused[25] =   {
	"Unused Variable=        "	// Unused variable
};

void put_car_data (FL_FILE *fp1)
{

	sprintf(file_string,"Car Data");
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);
	
	sprintf(file_string,"%06d // Car Nunber ",cons[carnmb]);
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);
	sprintf(file_string,"%06d // Car Speed ",cons[speed]);
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);
	sprintf(file_string,"%06d // Number of Cars ",cons[nmbcars]);
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);
	sprintf(file_string,"%06d // Bottom Floor ",cons[bottomf]);
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);
	sprintf(file_string,"%06d // Top Floor ",cons[topf]);
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);
	sprintf(file_string,"%06d // Group Top Floor ",cons[grtopf]);
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);
}

int16 get_car_data (FL_FILE *fp1)
{
	fl_fgets(fdatbuf,180,fp1);				// Car Data
	if (strstr(fdatbuf,"Car Data") != 0)	
	{		// Get file Car Data
	   	fl_fgets(fdatbuf,180,fp1);				// Car Number
		file_car_nmb = (int16)atoi(fdatbuf);
	   	fl_fgets(fdatbuf,180,fp1);				// Car Speed
		file_car_speed = (int16)atoi(fdatbuf);
	   	fl_fgets(fdatbuf,180,fp1);				// Number of Cars
		file_nmb_cars = (int16)atoi(fdatbuf);
	   	fl_fgets(fdatbuf,180,fp1);				// Bottom Floor
		file_bottom_fl = (int16)atoi(fdatbuf);
	   	fl_fgets(fdatbuf,180,fp1);				// Top Floor
		file_top_fl = (int16)atoi(fdatbuf);
	   	fl_fgets(fdatbuf,180,fp1);				// Group Top Floor
		file_grp_top_fl = (int16)atoi(fdatbuf);
		return 1;
	}
	else
		return 0;
}

int16 get_valid_code(FL_FILE *fp1)
{
	char char_in;
	
	char_in = (char)fl_fgetc(fp1);
	if (char_in == ' ')
		fdatbuf[0] = '0';
	else if ((char_in >= '0') && (char_in <= '9'))
		fdatbuf[0] = char_in;
	else
		return 0;
	char_in = (char)fl_fgetc(fp1);
	if ((char_in >= '0') && (char_in <= '9'))
		fdatbuf[1] = char_in;
	else
		return 0;
	fdatbuf[2] = 0;	// null charater
	return 1;
	
}

static char Can_Device_Name[16][9] = {
"ENC SEL ",	// c_ctcan_ENC_SEL
"NTS PROC", // c_mrcan_NTS
"BRAKE   ", // Not used at this time
"SAF PROC", // c_mrcan_SPB
"SELECTOR", // c_ctcan_SEL
"DOOR    ", // c_ctcan_DOOR
"RDOOR   ", // c_ctcan_RDOOR
"LW BRD  ", // c_mrcan_LW, c_ctcan_LW
"VS BRD  ", // c_mrcan_VS, c_ctcan_VS
"PI BRD  ", // c_mrcan_PI_LANT, c_ctcan_PI_LANT
"SEB #1  ", // c_mrcan_SEB_1, c_ctcan_SEB_4, c_grcan_SEB_1
"FI PI BD", // c_grcan_FI_DPY
"COP BD  ", // c_ctcan_SEB_1, c_ctcan_COP
"SEL DZ  ", // c_ctcan_SEB_2, c_ctcan_SEL_DZ
"SEL LIM ", // c_ctcan_SEB_3, c_ctcan_SEL_LIM
"HCDB #1 ", // c_grcan_HCDB_1
};

static char Dev_Name[10];
static char T_String[10];


void Set_Device_Name(char * dev_name, int16 code, uint8 dev)
{
	int16 k = 0;
	
	if (code == f_MRCAN_device_fault)
	{
		if (dev == c_mrcan_NTS)
			k = 1;
		else if (dev == c_mrcan_SPB)
			k = 3;
		else if (dev == c_mrcan_LW)
			k = 7;
		else if (dev == c_mrcan_VS)
			k = 8;
		else if (dev == c_mrcan_PI_LANT)
			k = 9;
		else if (dev >= c_mrcan_SEB_1)
			k = 10;
		else
			k = 1;
					
		strncpy(dev_name,Can_Device_Name[k],8);

		if (dev >= c_mrcan_SEB_1)
		{
			itoa((dev-c_mrcan_SEB_1)+1,T_String);
			dev_name[5] = T_String[0];
			if ((T_String[1] >= '0') && (T_String[1] <= '9'))
				dev_name[6] = T_String[1];
		}
	}
	else if (code == f_CTCAN_device_fault)
	{
		if (dev == c_ctcan_ENC_SEL)
			k = 0;
		else if (dev == c_ctcan_SEL)
			k = 4;
		else if (dev == c_ctcan_DOOR)
			k = 5;
		else if (dev == c_ctcan_RDOOR)
			k = 6;
		else if (dev == c_ctcan_LW)
			k = 7;
		else if (dev == c_ctcan_VS)
			k = 8;
		else if (dev == c_ctcan_PI_LANT)
			k = 9;
		else if (dev == c_ctcan_SEB_1)
			k = 12;
		else if (dev == c_ctcan_SEB_2)
			k = 13;
		else if (dev == c_ctcan_SEB_3)
			k = 14;
		else if (dev >= c_ctcan_SEB_4)
			k = 10;
		else 
			k = 0;
					
		strncpy(dev_name,Can_Device_Name[k],8);

		if (dev >= c_ctcan_SEB_4)
		{
			itoa((dev-c_ctcan_SEB_1)+1,T_String);
			dev_name[5] = T_String[0];
			if ((T_String[1] >= '0') && (T_String[1] <= '9'))
				dev_name[6] = T_String[1];
		}
	}
	else
	{
		if (dev == c_grcan_VS)
			k = 8;
		else if (dev == c_grcan_FI_DPY)
			k = 9;
		else if ((dev >= c_grcan_SEB_1) && (dev <= c_grcan_SEB_40))
			k = 10;
		else if (dev >= c_grcan_HCDB_1)
			k = 15;
		else 
			k = 10;
					
		strncpy(dev_name,Can_Device_Name[k],8);

		if ((dev >= c_grcan_SEB_1) && (dev <= c_grcan_SEB_40))
		{
			itoa((dev-c_grcan_SEB_1)+1,T_String);
			dev_name[5] = T_String[0];
			if ((T_String[1] >= '0') && (T_String[1] <= '9'))
				dev_name[6] = T_String[1];
		}
		else if (dev >= c_grcan_HCDB_1)
		{
			itoa((dev-c_grcan_HCDB_1)+1,T_String);
			dev_name[5] = T_String[0];
			if ((T_String[1] >= '0') && (T_String[1] <= '9'))
				dev_name[6] = T_String[1];
		}
    }
}

void put_car_faults (int16 car,FL_FILE *fp1)
{
	struct Fault *flp;
	int16 loop_ix = 0;
	int16 dpy_flt = 0;
	int16 flt_ix = 0;
	int16 dpy_ix = 1;
	int16 vel_diff = 0;
    int16 j;

	put_car_data(fp1);

	if (car != cons[carnmb])
		flt_ix = Car_Flt_ix + 1;
	else
		flt_ix = Flt_ix + 1;
	if ((flt_ix >= max_flt_ix) || (flt_ix < 0))
		flt_ix = 0;	

	sprintf(file_string,"Fault Log Car #%.2s",car_bld_no[car]);
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);		
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);		

	while (loop_ix < max_flt_ix)
	{
		if (car != cons[carnmb])
			flp = &cf.Flts[flt_ix];
		else
			flp = &f.Flts[flt_ix];

		if (flp->code != 0)
		{
			dpy_flt = 1;
			sprintf(file_string,"%3i %.20s ",dpy_ix++,&LCD_Fault[flp->code][0]);
			fl_fputs(file_string,fp1);
			sprintf(file_string,"%2i:%02i:%02i %2i/%02i/%4i Position =%2i Occurrences =%4i ",
					flp->hour,flp->min,flp->sec,flp->mon,flp->day,flp->year,flp->pos,flp->count);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);
			
			if (((flp->code == f_MRCAN_device_fault) || (flp->code == f_CTCAN_device_fault) || (flp->code == f_GRCAN_device_fault)) && 
				(flp->device != 0) && (flp->prog_flag1 != 0))
			{ // display device and device fault with count
				
				Set_Device_Name(Dev_Name,flp->code,flp->device);

				if (flp->prog_flag2 < 1) 
					j = 1;
				if (flp->prog_flag2 > 127)
					j = 127;
				// display device name and number of faults
				sprintf(file_string,"Dev: %.8s Cnt: %3i",Dev_Name,j);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);		
				// Display device fault
				sprintf(file_string,"%.24s ",LCD_device_Fault[flp->prog_flag1]);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);		
				
			}
			else if (((flp->code == f_MRCAN_device_reset) || (flp->code == f_CTCAN_device_reset) || (flp->code == f_GRCAN_device_reset)) &&
				(flp->device != 0))
			{		// Display Device and device requested initialization
				
				Set_Device_Name(Dev_Name,flp->code,flp->device);

				// display device name and number of faults
				sprintf(file_string,"Device: %.8s",Dev_Name);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);		
				// Display device fault
				sprintf(file_string,"%.24s ",LCD_device_Reset[1]);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);		
				
			}
					
			sprintf(file_string,"srv=%03i, prc=%03i, drf=%03i, rdf=%03i, dpr=%03i, dir=%03i, emp=%03i, med=%03i",
				flp->servf, flp->procf, flp->doorf, flp->rdoorf, 
				flp->dpref, flp->dirf, flp->empf, flp->medf);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"cbl=%03i, equ=%03i, fir=%03i, rfi=%03i, hsf=%03i, stf=%03i, cal=%03i, esp=%03i ",
				flp->codebf, flp->eqf, flp->firef, flp->rfiref,
				flp->hsf, flp->startf, flp->dcalls, flp->estop);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"nst=%03i, rlv=%03i, ste=%03i, dsf=%03i, st0=%03i, ins=%02xh, nds=%03i, dev=%02xh",
				flp->nstopf, flp->relevel, flp->stepf, flp->door_seqf,
				flp->next_stop, flp->ins_status, flp->ncu_door_seq, flp->device);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"pf1=%02xh, pf2=%02xh, dv2=%02xh, io0=%02xh, io1=%02xh, io2=%02xh, io3=%02xh, io4=%02xh",
				flp->prog_flag1, flp->prog_flag2, flp->device_2,
				(unsigned char)(flp->iodata[0]), (unsigned char)(flp->iodata[1]),
				(unsigned char)(flp->iodata[2]), (unsigned char)(flp->iodata[3]),
				(unsigned char)(flp->iodata[4]));
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"io5=%02xh, io6=%02xh, io7=%02xh, io8=%02xh, io9=%02xh, ioA=%02xh, ioB=%02xh, ioC=%02xh",
				(unsigned char)(flp->iodata[5]),(unsigned char)(flp->iodata[6]), 
				(unsigned char)(flp->iodata[7]),(unsigned char)(flp->iodata[8]), 
				(unsigned char)(flp->iodata[9]),(unsigned char)(flp->iodata[10]), 
				(unsigned char)(flp->iodata[11]),(unsigned char)(flp->iodata[12])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"ioD=%02xh, ioE=%02xh, ioF=%02xh, ioG=%02xh, ioH=%02xh, ioI=%02xh, ioJ=%02xh, ioK=%02xh",
				(unsigned char)(flp->iodata[13]),(unsigned char)(flp->iodata[14]), 
				(unsigned char)(flp->iodata[15]),(unsigned char)(flp->iodata[16]), 
				(unsigned char)(flp->iodata[17]),(unsigned char)(flp->iodata[18]), 
				(unsigned char)(flp->iodata[19]),(unsigned char)(flp->iodata[20])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"ioL=%02xh, ioM=%02xh, ioN=%02xh, ioO=%02xh, ioP=%02xh, ioQ=%02xh",
				(unsigned char)(flp->iodata[21]),(unsigned char)(flp->iodata[22]), 
				(unsigned char)(flp->iodata[23]),(unsigned char)(flp->iodata[24]), 
				(unsigned char)(flp->iodata[25]),(unsigned char)(flp->iodata[26])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"ioR=%02xh, ioS=%02xh, ioT=%02xh",
				(unsigned char)(flp->iodata[27]),(unsigned char)(flp->iodata[28]),(unsigned char)(flp->iodata[29])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"statusf=%08xh, statusf2=%08xh",flp->statusf,flp->statusf2);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"statusf3=%08xh, statusf4=%08xh",flp->statusf3,flp->statusf4);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

   			sprintf(file_string,"DPP Count = %7ld, Floor Count = %7ld, SD Count = %7ld, Enc Vel = %5i",
	   			flp->dpp_count, flp->fl_dpp_count, flp->sd_count, flp->enc_vel_fpm);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);
			
#if(Traction == 1)
			if(abs(flp->drv_vel_fpm-flp->enc_vel_fpm)>9999)
				vel_diff = 9999;
			else
				vel_diff = flp->dmd_vel_fpm-flp->enc_vel_fpm;
		
   			sprintf(file_string,"Calc Vel = %5i, Dmd Vel = %5i, Vel Diff = %5i, Enc Dir = %2i",
	   			flp->vel_fpm, flp->dmd_vel_fpm, flp->drv_vel_fpm, flp->enc_dir);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"SPB Poscnt = %7ld, SPB Vel = %5i",
			   	flp->SPB_poscnt, flp->SPB_vel); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"SPB Serv = %2xh, SPB Cmd = %2xh, SPB Stat = %2xh",
			   	flp->SPB_service, flp->SPB_command, flp->SPB_status); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"SPB io 1=%02xh, io 2=%02xh, io 3=%02xh, io 4=%02xh, ios5=%02xh",
				(unsigned char)(flp->SPB_ios[0]),(unsigned char)(flp->SPB_ios[1]), 
				(unsigned char)(flp->SPB_ios[2]),(unsigned char)(flp->SPB_ios[3]),
				(unsigned char)(flp->SPB_ios[4])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"Flt Bits 1 = %2xh, Flt Bits 2 = %2xh, Flt Bits 3 = %2xh, Flt Bits 4 = %2xh",
			   	flp->fault_bits[0], flp->fault_bits[1], flp->fault_bits[2], flp->fault_bits[3]); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);
#endif
					
			sprintf(file_string,"SS Status=%04xh, PWR Status=%04xh, Run Status=%08xh",flp->ss_status,flp->power_status,flp->run_statusf);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		


#if(Traction == 1)
			sprintf(file_string,"NTS Vel = %5i, NTS Serv = %2xh, NTS Cmd = %2xh, NTS Stat = %2xh",
			   	flp->NTS_vel, flp->NTS_service,flp->NTS_command,flp->NTS_status); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"NTS Lim Flt 1 = %2xh, NTS Lim Flt 2 = %2xh, NTS Lim Flt 3 = %2xh",
			   	flp->NTS_limit_flt[0], flp->NTS_limit_flt[1],flp->NTS_limit_flt[2]); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);	
			sprintf(file_string,"NTS io 1=%02xh, io 2=%02xh, io 3=%02xh",
				(unsigned char)(flp->NTS_ios[0]),(unsigned char)(flp->NTS_ios[1]),(unsigned char)(flp->NTS_ios[2])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
#endif
	
			sprintf(file_string,"Nudg Flags=%02xh, Door Req=%02xh, Call Flags=%02xh",
				(unsigned char)(flp->nudgst_flags),(unsigned char)(flp->door_req_flags),(unsigned char)(flp->call_flags)); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"Chk Run=%02xh, Chk Start=%02xh, Chk Level=%02xh, Chk Door=%2xh",
				(unsigned char)(flp->chkrun_st),(unsigned char)(flp->chkstart_st),
				(unsigned char)(flp->chklevel_st),(unsigned char)(flp->chkdoor_st)); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"Front SD=%04xh, Rear SD=%04xh, Motion Tmr=%05i",
				flp->front_sd,flp->rear_sd,flp->motion_tmr);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"PAL Vel=%05i, PAL Statusf=%02xh, Inspect Svc=%03i",flp->PAL_vel,flp->PAL_stat,flp->ins_servf);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"Drive Command=%04xh, Drive Statusf=%04xh",flp->drv_cmd,flp->drv_status);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);					  

			sprintf(file_string,"Torque Command=%05i, Motor Torque=%5i, Percent Load=%3i",flp->drv_torque_cmd,flp->drv_mtr_torque,flp->percent_load/10);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);					  

			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
	   	}
		flt_ix++;
		if (flt_ix >= max_flt_ix)
			flt_ix = 0;
		loop_ix++;
	}
	if (loop_ix >= max_flt_ix)
	{
		if (dpy_flt == 0)
		{
			sprintf(file_string,"No Occurences");
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
		}
	}
}

void put_car_lt_faults (FL_FILE *fp1)
{
	int16 loop_ix = 0;
	int16 dpy_flt = 0;
	int16 flt_ix = 0;
	int16 dpy_ix = 1;
	int16 vel_diff = 0;
	int16 j;

	union flt_union {
		unsigned char fltb[sizeof(Fault_type)];
		uint16 fltw[sizeof(Fault_type)/2];
		struct Fault Flt;
	}ltf;

	put_car_data(fp1);

	flt_ix = LT_Flt_ix + 1;
	if ((flt_ix >= max_lt_flt_ix) || (flt_ix < 0))
		flt_ix = 0;	

	sprintf(file_string,"Long Term Fault Log Car #%.2s",car_bld_no[cons[carnmb]]);
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);		
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);		

	while (loop_ix < max_lt_flt_ix)
	{
		rd_LT_flts(flt_ix, ltf.fltw);
		
		if (ltf.Flt.code != 0)
		{
			dpy_flt = 1;
			sprintf(file_string,"%3i %.20s ",dpy_ix++,&LCD_Fault[ltf.Flt.code][0]);
			fl_fputs(file_string,fp1);
			sprintf(file_string,"%2i:%02i:%02i %2i/%02i/%4i Position =%2i Occurrences =%4i ",
					ltf.Flt.hour,ltf.Flt.min,ltf.Flt.sec,ltf.Flt.mon,ltf.Flt.day,ltf.Flt.year,ltf.Flt.pos,ltf.Flt.count);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			if (((ltf.Flt.code == f_MRCAN_device_fault) || (ltf.Flt.code == f_CTCAN_device_fault) || (ltf.Flt.code == f_GRCAN_device_fault)) && 
				(ltf.Flt.device != 0) && (ltf.Flt.prog_flag1 != 0))
			{ // display device and device fault with count
				
				Set_Device_Name(Dev_Name,ltf.Flt.code,ltf.Flt.device);

				if (ltf.Flt.prog_flag2 < 1) 
					j = 1;
				if (ltf.Flt.prog_flag2 > 127)
					j = 127;
				// display device name and number of faults
				sprintf(file_string,"Dev: %.8s Cnt: %3i",Dev_Name,j);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);		
				// Display device fault
				sprintf(file_string,"%.24s ",LCD_device_Fault[ltf.Flt.prog_flag1]);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);		
				
			}
			else if (((ltf.Flt.code == f_MRCAN_device_reset) || (ltf.Flt.code == f_CTCAN_device_reset) || (ltf.Flt.code == f_GRCAN_device_reset)) &&
				(ltf.Flt.device != 0))
			{		// Display Device and device requested initialization
				
				Set_Device_Name(Dev_Name,ltf.Flt.code,ltf.Flt.device);

				// display device name and number of faults
				sprintf(file_string,"Device: %.8s",Dev_Name);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);		
				// Display device fault
				sprintf(file_string,"%.24s ",LCD_device_Reset[1]);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);		
				
			}
					
			sprintf(file_string,"srv=%03i, prc=%03i, drf=%03i, rdf=%03i, dpr=%03i, dir=%03i, emp=%03i, med=%03i",
				ltf.Flt.servf, ltf.Flt.procf, ltf.Flt.doorf, ltf.Flt.rdoorf, 
				ltf.Flt.dpref, ltf.Flt.dirf, ltf.Flt.empf, ltf.Flt.medf);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"cbl=%03i, equ=%03i, fir=%03i, rfi=%03i, hsf=%03i, stf=%03i, cal=%03i, esp=%03i ",
				ltf.Flt.codebf, ltf.Flt.eqf, ltf.Flt.firef, ltf.Flt.rfiref,
				ltf.Flt.hsf, ltf.Flt.startf, ltf.Flt.dcalls, ltf.Flt.estop);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"nst=%03i, rlv=%03i, ste=%03i, dfs=%03i, st0=%03i, ins=%02xh, nds=%03i, dev=%02xh",
				ltf.Flt.nstopf, ltf.Flt.relevel, ltf.Flt.stepf, ltf.Flt.door_seqf,
				ltf.Flt.next_stop, ltf.Flt.ins_status, ltf.Flt.ncu_door_seq, ltf.Flt.device);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"pf1=%02xh, pf2=%02xh, dv2=%02xh, io0=%02xh, io1=%02xh, io2=%02xh, io3=%02xh, io4=%02xh",
				ltf.Flt.prog_flag1, ltf.Flt.prog_flag2, ltf.Flt.device_2,
				(unsigned char)(ltf.Flt.iodata[0]), (unsigned char)(ltf.Flt.iodata[1]),
				(unsigned char)(ltf.Flt.iodata[2]), (unsigned char)(ltf.Flt.iodata[3]),
				(unsigned char)(ltf.Flt.iodata[4]));
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"io5=%02xh, io6=%02xh, io7=%02xh, io8=%02xh, io9=%02xh, ioA=%02xh, ioB=%02xh, ioC=%02xh",
				(unsigned char)(ltf.Flt.iodata[5]),(unsigned char)(ltf.Flt.iodata[6]), 
				(unsigned char)(ltf.Flt.iodata[7]),(unsigned char)(ltf.Flt.iodata[8]), 
				(unsigned char)(ltf.Flt.iodata[9]),(unsigned char)(ltf.Flt.iodata[10]), 
				(unsigned char)(ltf.Flt.iodata[11]),(unsigned char)(ltf.Flt.iodata[12])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"ioD=%02xh, ioE=%02xh, ioF=%02xh, ioG=%02xh, ioH=%02xh, ioI=%02xh, ioJ=%02xh, ioK=%02xh",
				(unsigned char)(ltf.Flt.iodata[13]),(unsigned char)(ltf.Flt.iodata[14]), 
				(unsigned char)(ltf.Flt.iodata[15]),(unsigned char)(ltf.Flt.iodata[16]), 
				(unsigned char)(ltf.Flt.iodata[17]),(unsigned char)(ltf.Flt.iodata[18]), 
				(unsigned char)(ltf.Flt.iodata[19]),(unsigned char)(ltf.Flt.iodata[20])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"ioL=%02xh, ioM=%02xh, ioN=%02xh, ioO=%02xh, ioP=%02xh, ioQ=%02xh",
				(unsigned char)(ltf.Flt.iodata[21]),(unsigned char)(ltf.Flt.iodata[22]), 
				(unsigned char)(ltf.Flt.iodata[23]),(unsigned char)(ltf.Flt.iodata[24]), 
				(unsigned char)(ltf.Flt.iodata[25]),(unsigned char)(ltf.Flt.iodata[26])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"ioR=%02xh, ioS=%02xh, ioT=%02xh",
				(unsigned char)(ltf.Flt.iodata[27]),(unsigned char)(ltf.Flt.iodata[28]),(unsigned char)(ltf.Flt.iodata[29])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);	
				
			sprintf(file_string,"statusf=%08xh, statusf2=%08xh", ltf.Flt.statusf,ltf.Flt.statusf2);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);	

			sprintf(file_string,"statusf3=%08xh, statusf4=%08xh", ltf.Flt.statusf3,ltf.Flt.statusf4);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);	

    		sprintf(file_string,"DPP Count = %7ld, Floor Count = %7ld, SD Count = %7ld, Enc Vel = %5i",
	   			ltf.Flt.dpp_count, ltf.Flt.fl_dpp_count, ltf.Flt.sd_count, ltf.Flt.enc_vel_fpm);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);
			
#if(Traction == 1)
			if(abs(ltf.Flt.drv_vel_fpm-ltf.Flt.enc_vel_fpm)>9999)
				vel_diff = 9999;
			else
				vel_diff = ltf.Flt.dmd_vel_fpm-ltf.Flt.enc_vel_fpm;
		

   			sprintf(file_string,"Calc Vel = %5i, Dmd Vel = %5i, Vel Diff = %5i, Enc Dir = %2i",
	   			ltf.Flt.vel_fpm, ltf.Flt.dmd_vel_fpm, vel_diff, ltf.Flt.enc_dir);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"SPB Poscnt = %7ld, SPB Vel = %5i",
			   	ltf.Flt.SPB_poscnt, ltf.Flt.SPB_vel); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"SPB Serv = %2xh, SPB Cmd = %2xh, SPB Stat = %2xh",
			   	ltf.Flt.SPB_service, ltf.Flt.SPB_command, ltf.Flt.SPB_status); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"SPB ios1=%02xh, ios2=%02xh, ios3=%02xh, ios4=%02xh, ios5=%02xh",
				(unsigned char)(ltf.Flt.SPB_ios[0]),(unsigned char)(ltf.Flt.SPB_ios[1]), 
				(unsigned char)(ltf.Flt.SPB_ios[2]),(unsigned char)(ltf.Flt.SPB_ios[3]),
				(unsigned char)(ltf.Flt.SPB_ios[4])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
#endif

			sprintf(file_string,"Flt Bits 1 = %2xh, Flt Bits 2 = %2xh, Flt Bits 3 = %2xh, Flt Bits 4 = %2xh",
			   	ltf.Flt.fault_bits[0], ltf.Flt.fault_bits[1], ltf.Flt.fault_bits[2], ltf.Flt.fault_bits[3]); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);
			
			sprintf(file_string,"SS Status=%04xh, PWR Status=%04xh, Run Status=%08xh",ltf.Flt.ss_status,ltf.Flt.power_status,ltf.Flt.run_statusf);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		


#if(Traction == 1)
			sprintf(file_string,"NTS Vel = %5i, NTS Serv = %2xh, NTS Cmd = %2xh, NTS Stat = %2xh",
			   	ltf.Flt.NTS_vel, ltf.Flt.NTS_service,ltf.Flt.NTS_command,ltf.Flt.NTS_status); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
			sprintf(file_string,"NTS Lim Flt[0] = %2xh, NTS Lim Flt[1] = %2xh, NTS Lim Flt[2] = %2xh",
			   	ltf.Flt.NTS_limit_flt[0], ltf.Flt.NTS_limit_flt[1],ltf.Flt.NTS_limit_flt[2]); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);	
			sprintf(file_string,"NTS ios1=%02xh, ios2=%02xh, ios3=%02xh",
				(unsigned char)(ltf.Flt.NTS_ios[0]),(unsigned char)(ltf.Flt.NTS_ios[1]),(unsigned char)(ltf.Flt.NTS_ios[2])); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
#endif
				
			sprintf(file_string,"Nudg Flags=%02xh, Door Req=%02xh, Call Flags=%02xh",
				(unsigned char)(ltf.Flt.nudgst_flags),(unsigned char)(ltf.Flt.door_req_flags),(unsigned char)(ltf.Flt.call_flags)); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"Chk Run=%02xh, Chk Start=%02xh, Chk Level=%02xh, Chk Door=%2xh",
				(unsigned char)(ltf.Flt.chkrun_st),(unsigned char)(ltf.Flt.chkstart_st),
				(unsigned char)(ltf.Flt.chklevel_st),(unsigned char)(ltf.Flt.chkdoor_st)); 
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"Front SD=%04xh, Rear SD=%04xh, Motion Tmr=%05i",
				ltf.Flt.front_sd,ltf.Flt.rear_sd,ltf.Flt.motion_tmr);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"PAL Vel=%05i, PAL Statusf=%02xh, Inspect Svc=%03i",ltf.Flt.PAL_vel,ltf.Flt.PAL_stat,ltf.Flt.ins_servf);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		

			sprintf(file_string,"Drive Command=%04xh, Drive Statusf=%04xh",ltf.Flt.drv_cmd,ltf.Flt.drv_status);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);					  

			sprintf(file_string,"Torque Command=%05i, Motor Torque=%5i, Percent Load=%3i",ltf.Flt.drv_torque_cmd,ltf.Flt.drv_mtr_torque,ltf.Flt.percent_load/10);
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);					  


			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);
					
	   	}
		flt_ix++;
		if (flt_ix >= max_lt_flt_ix)
			flt_ix = 0;
		loop_ix++;
	}
	if (loop_ix >= max_lt_flt_ix)
	{
		if (dpy_flt == 0)
		{
			sprintf(file_string,"No Occurences");
			fl_fputs(file_string,fp1);
			fl_fputc(CR,fp1);
			fl_fputc(LF,fp1);		
		}
	}
}


void put_car_stats (FL_FILE *fp1)
{
	int16 i;
	
	sprintf(file_string, "Elevator Statistics");
	fl_fputs(file_string,fp1);
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);		
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);		

	for(i=0; i<=12; i++)
	{
   		sprintf(file_string,"%s %ld",call_status_dpy[i],lvars[i]);
		fl_fputs(file_string,fp1);
		fl_fputc(CR,fp1);
		fl_fputc(LF,fp1);		
	}
	
	fl_fputc(CR,fp1);
	fl_fputc(LF,fp1);		

	if((lvars[1] + lvars[2]) > 0)
	{
     	sprintf(file_string,"%i Percent Answered In Less than 15 Second",
	 		(((lvars[3] + lvars[8]) * 100) / (lvars[1] + lvars[2])) );
		fl_fputs(file_string,fp1);
		fl_fputc(CR,fp1);
		fl_fputc(LF,fp1);		
     	sprintf(file_string,"%i Percent Answered In Less than 30 Second",
	 		(((lvars[4] + lvars[9]) * 100) / (lvars[1] + lvars[2])) );
		fl_fputs(file_string,fp1);
		fl_fputc(CR,fp1);
		fl_fputc(LF,fp1);		
     	sprintf(file_string,"%i Percent Answered In Less than 45 Second",
	 		(((lvars[5] + lvars[10]) * 100) / (lvars[1] + lvars[2])) );
		fl_fputs(file_string,fp1);
		fl_fputc(CR,fp1);
		fl_fputc(LF,fp1);		
     	sprintf(file_string,"%i Percent Answered In Less than 60 Second",
	 		(((lvars[6] + lvars[11]) * 100) / (lvars[1] + lvars[2])) );
		fl_fputs(file_string,fp1);
		fl_fputc(CR,fp1);
		fl_fputc(LF,fp1);		
     	sprintf(file_string,"%i Percent Answered In More than 60 Second",
		 		(((lvars[7] + lvars[12]) * 100) / (lvars[1] + lvars[2])) );
		fl_fputs(file_string,fp1);
		fl_fputc(CR,fp1);
		fl_fputc(LF,fp1);		
	}
}


int16 fvarstoSD(int16 command, int16 force_write)
{
	int16 i;
	int16 j;
	int16 k;
	char char_in;
	static int16 value;
	static int16 index;
	int16 sd_error = 0;
	int16 exit_cmd = 0;		// Preset error for Invalid command

	FL_FILE *fp1 = NULL;

	if (sd_init == 0)
	{
		sd_error = sd_initialization();
		// Initialise File IO Library
		if (sd_error == 0)
		{
			fl_init();
			// Attach media access functions to library
			if (fl_attach_media(media_read, media_write) != FAT_INIT_OK)
			{
				sd_error = 0x55; 
				exit_cmd = 8;	// Media Init failed
			}
		}
		else
			exit_cmd = 7;		// sd init error
	}

	disable_dog();
		
	switch(command)
	{	
		case 0:	// Write Field Variables
			
			fp1 = fl_fopen("/fvars.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/fvars.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				
				write_display("Writing: Please Wait",2,0);
				fp1 = fl_fopen("/fvars.txt","w");
				put_car_data(fp1);
				
				// Read Field Variables
				sprintf(file_string,"Field Variables");
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

				sprintf(file_string,"%06d // Last Field Variable",lastfvar);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

				//FIELD VARIABLES
				for(i=0; i < nmbfvar; i++)
				{
					sprintf(file_string,"%06d ",fvars[i]);
					file_string[7] = '/';
					file_string[8] = '/';
					sprintf(&file_string[9]," %03d ",i);
					
					for (j = 0; j <= 14; j++)
					{
						if (i > lastfvar)
							file_string[14+j] = Field_Vars_Unused[j];
						else				
							file_string[14+j] = LCD_Field_Vars[i][j];
					}
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);

				}
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				fl_fputc(0,fp1);
				exit_cmd = 1;		// data written to file

			}
			else
				exit_cmd = 10;				// existing file

			break;

		case 1:		// Read Variables from file
			exit_cmd = 3;			// preset file not found error
			fp1 = fl_fopen("/fvars.txt","r");
			if (fp1 != NULL)
			{
				exit_cmd = 4;				// preset error for cannot read car data
				if (get_car_data(fp1) == 1)	
				{		// Valid Car Data
					exit_cmd = 5;			// preset error for invalid car data
					if ((file_car_nmb <= 8) && (file_car_speed >= 25) && (file_car_speed <= 2000) && 
						(file_bottom_fl > 0) &&	(file_bottom_fl < file_top_fl) &&
						(file_top_fl <= file_grp_top_fl))
					{
						exit_cmd = 6;			// preset error for invalid data field
						fl_fgets(fdatbuf,180,fp1);
						if (strstr(fdatbuf,"Field Variables") != 0)	
						{		// Get file Field Variables
					      	fl_fgets(fdatbuf,180,fp1);				// Last field variable
							file_last_fvar = (int16)atoi(fdatbuf);
							
							// READ FIELD VARIABLES FROM FILE

							for(i =0; i <= file_last_fvar; i++)
							{			// only load valid variables
								fl_fgets(fdatbuf,180,fp1);
								fvars[i] = (int16)atoi(fdatbuf);
							}
							wrfvar();
							exit_cmd = 2;		// no error set command for data read
						}
					}
				}
			}
			break;
			
		case 2:	// Write NTS Field Variables
			
			fp1 = fl_fopen("/ntsfvars.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/ntsfvars.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				
				write_display("Writing: Please Wait",2,0);
				fp1 = fl_fopen("/ntsfvars.txt","w");
				put_car_data(fp1);
				
				// Read Field Variables
				sprintf(file_string,"NTS Field Variables");
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

				sprintf(file_string,"%06d // Last Field Variable",lastntsfvar);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

				//SPB FIELD VARIABLES
				for(i=0; i < numntsvars; i++)
				{
					sprintf(file_string,"%06d ",nts_fvars[i]);
					file_string[7] = '/';
					file_string[8] = '/';
					sprintf(&file_string[9]," %03d ",i);
					
					for (j = 0; j <= 14; j++)
					{
						if (i > lastntsfvar)
							file_string[14+j] = Field_Vars_Unused[j];
						else				
							file_string[14+j] = NTS_Field_Vars[i][j];
					}
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);

				}
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				fl_fputc(0,fp1);
				exit_cmd = 1;		// data written to file

			}
			else
				exit_cmd = 10;				// existing file

			break;

		case 3:		// Read NTS Variables from file
			exit_cmd = 3;			// preset file not found error
			fp1 = fl_fopen("/ntsfvars.txt","r");
			if (fp1 != NULL)
			{
				exit_cmd = 4;				// preset error for cannot read car data
				if (get_car_data(fp1) == 1)	
				{		// Valid Car Data
					exit_cmd = 5;			// preset error for invalid car data
					if ((file_car_nmb <= 8) && (file_car_speed >= 25) && (file_car_speed <= 2000) && 
						(file_bottom_fl > 0) &&	(file_bottom_fl < file_top_fl) &&
						(file_top_fl <= file_grp_top_fl))
					{
						exit_cmd = 6;			// preset error for invalid data field
						fl_fgets(fdatbuf,180,fp1);
						if (strstr(fdatbuf,"NTS Field Variables") != 0)	
						{		// Get file Field Variables
					      	fl_fgets(fdatbuf,180,fp1);				// Last field variable
							file_last_fvar = (int16)atoi(fdatbuf);
							
							// READ FIELD VARIABLES FROM FILE

							for(i =0; i <= file_last_fvar; i++)
							{			// only load valid variables
								fl_fgets(fdatbuf,180,fp1);
								nts_fvars[i] = (int16)atoi(fdatbuf);
							}
							
							NTS_eep_ix = 0;		   	
							NTS_eep_nmb = (int16)(numntsvars);
							NTS_spi.eep_req = 2;							// send data to spb processor
							wr_nts_fvar();				// data already sent to spb but needs to be written in eeprom
			
							exit_cmd = 2;		// no error set command for data read
						}
					}
				}
			}
			break;

		case 4:		// Setup Data
		
			fp1 = fl_fopen("/setup.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/setup.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				write_display("Writing: Please Wait",2,0);
				fp1 = fl_fopen("/setup.txt","w");
				put_car_data(fp1);

				// SPEED CLAMPS			
				fl_fputs("Speed Clamps",fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

			    for(i=0; i<=8; i++)
			    {
					sprintf(file_string,"%06d // Speed Clamp %02d ",speed_clamp[i],i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);

			    }
			    
			    // Car call security mask
				fl_fputs("Car Call Security",fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

			    
				sprintf(file_string,"%04xh // Car Call Security Mask 0 ",cc_sec_mask[0]);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

				sprintf(file_string,"%04xh // Car Call Security Mask 0 ",cc_sec_mask[1]);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

				sprintf(file_string,"%04xh // Rear Car Call Security Mask 0 ",rcc_sec_mask[0]);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

				sprintf(file_string,"%04xh // Rear Car Call Security Mask 0 ",rcc_sec_mask[1]);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);

				//LOAD WEIGHER DATA
				fl_fputs("Load Weigher",fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				for(i=cons[bottomf]; i<=cons[topf]; i++)
				{
					sprintf(file_string,"%06d // Empty Car Floor %02d ",lw_empty[i],i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
				}
				for(i=cons[bottomf]; i<=cons[topf]; i++)
				{
					sprintf(file_string,"%06d //  Full Car Floor %02d ",lw_full[i],i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
				}
				
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				fl_fputc(0,fp1);
				exit_cmd = 1;		// data written to file

			}
			else
				exit_cmd = 10;				// existing file

			break;
			
		case 5: 		// Get Setup Data
				
			fp1 = fl_fopen("/setup.txt","r");
			if (fp1 != NULL)
			{
				exit_cmd = 4;				// preset error for cannot read car data
				if (get_car_data(fp1) == 1)	
				{		// Valid Car Data
					exit_cmd = 5;			// preset error for invalid car data
					if ((file_car_nmb <= 8) && (file_car_speed >= 25) && (file_car_speed <= 2000) && 
						(file_bottom_fl > 0) &&	(file_bottom_fl < file_top_fl) &&
						(file_top_fl <= file_grp_top_fl))
					{
						exit_cmd = 6;			// preset error for invalid data field
		   				fl_fgets(fdatbuf,180,fp1);				
						// SPEED CLAMPS			
						if (strstr(fdatbuf,"Speed Clamps") != 0)	
						{		// Get speed clamps
							
						    for(i=0; i<=8; i++)
						    {
			   					fl_fgets(fdatbuf,180,fp1);				 
								speed_clamp[i] = (int16)atoi(fdatbuf);
						    }
							exit_cmd = 2;		// no error set command for data read
							wrfvar();
						}
						
						if (exit_cmd == 6)
							break;
						exit_cmd = 6;			// preset error for invalid data field
						
		   				fl_fgets(fdatbuf,180,fp1);				
						// Car call security			
						if (strstr(fdatbuf,"Car Call Security") != 0)
						{
			   				fl_fgets(fdatbuf,180,fp1);				 
							cc_sec_mask[0] = atoh(fdatbuf);

			   				fl_fgets(fdatbuf,180,fp1);				 
							cc_sec_mask[1] = atoh(fdatbuf);

			   				fl_fgets(fdatbuf,180,fp1);				 
							rcc_sec_mask[0] = atoh(fdatbuf);

			   				fl_fgets(fdatbuf,180,fp1);				 
							rcc_sec_mask[1] = atoh(fdatbuf);
							exit_cmd = 2;		// no error set command for data read
							wrfvar();
						}

						if (exit_cmd == 6)
							break;
						exit_cmd = 6;			// preset error for invalid data field

		   				fl_fgets(fdatbuf,180,fp1);				 
						//LOAD WEIGHER DATA
						if (strstr(fdatbuf,"Load Weigher Up") != 0)
						{
							for(i=file_bottom_fl; i<=file_top_fl; i++)
							{				// Empty Car 
		   						fl_fgets(fdatbuf,180,fp1);				 
								lw_empty[i] = (int16)atoi(fdatbuf);
							}
							for(i=file_bottom_fl; i<=file_top_fl; i++)
							{				// Full Car
		   						fl_fgets(fdatbuf,180,fp1);				 
								lw_full[i] = (int16)atoi(fdatbuf);
							}
							if (verify_lw_data() == 1)
							{
								exit_cmd = 2;		// no error set command for data read
								lw_status |= c_lw_setup;		// set setup flag
								lw_status &= ~c_lw_error;		// clear error flag
								wrlwvar();
							}
							else
							{
								lw_status &= ~c_lw_on;				// load weigher is turned off
								lw_status &= ~c_lw_setup;		// error so clear setup flag
								lw_status |= c_lw_error;		// device has error
								record_fault(f_lw_load_table);
							}
						}
						else if (strstr(fdatbuf,"Load Weigher") != 0)
						{
							for(i=file_bottom_fl; i<=file_top_fl; i++)
							{				// Empty Car 
		   						fl_fgets(fdatbuf,180,fp1);				 
								lw_empty[i] = (int16)atoi(fdatbuf);
							}
							for(i=file_bottom_fl; i<=file_top_fl; i++)
							{				// Full Car
		   						fl_fgets(fdatbuf,180,fp1);				 
								lw_full[i] = (int16)atoi(fdatbuf);
							}
							if (verify_lw_data() == 1)
							{
								exit_cmd = 2;		// no error set command for data read
								lw_status |= c_lw_setup;		// set setup flag
								lw_status &= ~c_lw_error;		// clear error flag
								wrlwvar();
							}
							else
							{
								lw_status &= ~c_lw_on;				// load weigher is turned off
								lw_status &= ~c_lw_setup;		// error so clear setup flag
								lw_status |= c_lw_error;		// device has error
								record_fault(f_lw_load_table);
							}
							break;
						}
					}
				}

			}
			break;
			
		case 6:		// Hoistway Data

			fp1 = fl_fopen("/hoistway.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/hoistway.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				write_display("Writing: Please Wait ",2,0);
				fp1 = fl_fopen("/hoistway.txt","w");
				put_car_data(fp1);
				//HOISTWAY TABLES AND DATA
				sprintf(file_string,"Hoistway Table");
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
			  	for(i= 1; i<= cons[topf]; i++)
			  	{
					sprintf(file_string,"%010d // Floor Position Count ",DPP_Floor_Pos[i],i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
			  	}
				sprintf(file_string,"%010d // DN Count ",DPP_DN);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				sprintf(file_string,"%010d // DT%1d Count ",DPP_DT,i);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);				
				sprintf(file_string,"%010d // UT%1d Count ",DPP_UT,i);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);				
				sprintf(file_string,"%010d // UN Count ",DPP_UN);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				sprintf(file_string,"%05d // Up Fl Lev Dist ",Up_fl_level_dist);
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				sprintf(file_string,"%05d // Dn Fl Lev Dist ",Dn_fl_level_dist );
				fl_fputs(file_string,fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				
			  	for(i= 1; i<= cons[topf]; i++)
			  	{
					sprintf(file_string,"%05d // Floor %2d Up Slowdown ",DPP_Fl_Up_SD[i],i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
					sprintf(file_string,"%05d // Floor %2d Dn Slowdown ",DPP_Fl_Dn_SD[i],i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
			  	}
			  	
			  	for(i= 1; i<= cons[topf]; i++)
			  	{
					sprintf(file_string,"%05d // Floor %2d SF Up Slowdown ",DPP_SFl_Up_SD[i],i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
					sprintf(file_string,"%05d // Floor %2d SF Dn Slowdown ",DPP_SFl_Dn_SD[i],i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
			  	}
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				fl_fputc(0,fp1);
				exit_cmd = 1;		// data written to file

			}
			else
				exit_cmd = 10;				// existing file
		  	break;
		  	
		case 7:		// Hoistway Table
			fp1 = fl_fopen("/hoistway.txt","r");
			if (fp1 != NULL)
			{
				exit_cmd = 4;				// preset error for cannot read car data
				if (get_car_data(fp1) == 1)	
				{		// Valid Car Data
					exit_cmd = 5;			// preset error for invalid car data
					if ((file_car_nmb <= 8) && (file_car_speed >= 25) && (file_car_speed <= 2000) && 
						(file_bottom_fl > 0) &&	(file_bottom_fl < file_top_fl) && 
						(cons[topf] == file_top_fl) && (cons[bottomf] == file_bottom_fl) && 
						(file_top_fl <= file_grp_top_fl))
					{
						exit_cmd = 6;			// preset error for invalid data field
		   				fl_fgets(fdatbuf,180,fp1);				
						// Hoistway Table			
						if (strstr(fdatbuf,"Hoistway Table") != 0)	
						{		// Get hoistway data
		
						  	for(i= 1; i<= file_top_fl; i++)
						  	{
		   						fl_fgets(fdatbuf,180,fp1);				 
								DPP_Floor_Pos[i] = atoi(fdatbuf);
						  	}
						  	
	   						fl_fgets(fdatbuf,180,fp1);				 
							DPP_DN = atoi(fdatbuf);
						  	
	   						fl_fgets(fdatbuf,180,fp1);				 
							DPP_DT = atoi(fdatbuf);

	   						fl_fgets(fdatbuf,180,fp1);				 
							DPP_UT = atoi(fdatbuf);
							
	   						fl_fgets(fdatbuf,180,fp1);				 
							DPP_UN = atoi(fdatbuf);

	   						fl_fgets(fdatbuf,180,fp1);				 
							Up_fl_level_dist = (int16)atoi(fdatbuf);

	   						fl_fgets(fdatbuf,180,fp1);				 
							Dn_fl_level_dist = (int16)atoi(fdatbuf);
						
						  	for(i= 1; i<= cons[topf]; i++)
						  	{
		   						fl_fgets(fdatbuf,180,fp1);				 
								DPP_Fl_Up_SD[i] = (int16)atoi(fdatbuf);
		   						fl_fgets(fdatbuf,180,fp1);				 
								DPP_Fl_Dn_SD[i] = (int16)atoi(fdatbuf);
						  	}
						
						  	for(i= 1; i<= cons[topf]; i++)
						  	{
		   						fl_fgets(fdatbuf,180,fp1);				 
								DPP_SFl_Up_SD[i] = (int16)atoi(fdatbuf);
		   						fl_fgets(fdatbuf,180,fp1);				 
								DPP_SFl_Dn_SD[i] = (int16)atoi(fdatbuf);
						  	}
							i = verify_hoistway();
							if (i == 0)
							{
				  	  	  		Hoistway_Learned = 1;
								exit_cmd = 2;		// no error set command for data read
							}
							else
							{
				  	  	  		Hoistway_Learned = 0;
								record_fault(i);
							}
							Wrt_Hoistway();
						}
					}
				}
			}
			break;

		case 8:		// Write Service Timers
		
			fp1 = fl_fopen("/svctmr.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/svctmr.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				write_display("Writing: Please Wait",2,0);
				fp1 = fl_fopen("/svctmr.txt","w");
				put_car_data(fp1);
				
				fl_fputs("Service Timers",fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
			  	
				for(j = 0; j < 10; j++)
				{
					sprintf(file_string,"%05d // Service Timer #%2d Service ",svc_t[j].service,j);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
					sprintf(file_string,"%05d // Service Timer #%2d Status ",svc_t[j].status,j);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
					sprintf(file_string,"%05d // Service Timer #%2d Logic ",svc_t[j].logic,j);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
					sprintf(file_string,"%05d // Service Timer #%2d Option ",svc_t[j].option,j);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
					for (i=0;i<=2;i++)
					{
						sprintf(file_string,"%05d // Service Timer #%2d-%2d Month ",svc_t[j].month[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
						sprintf(file_string,"%05d // Service Timer #%2d-%2d Day ",svc_t[j].day[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
						sprintf(file_string,"%05d // Service Timer #%2d-%2d MD On Hr ",svc_t[j].md_on_hour[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
						sprintf(file_string,"%05d // Service Timer #%2d-%2d MD On min ",svc_t[j].md_on_minute[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
						sprintf(file_string,"%05d // Service Timer #%2d-%2d MD Off Hr ",svc_t[j].md_off_hour[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
						sprintf(file_string,"%05d // Service Timer #%2d-%2d MD Off min ",svc_t[j].md_off_minute[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
					}
					for (i=0;i<=6;i++)
					{
						sprintf(file_string,"%05d // Service Timer #%2d-%2d DOW On Hr ",svc_t[j].on_hour[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
						sprintf(file_string,"%05d // Service Timer #%2d-%2d DOW On Min ",svc_t[j].on_minute[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
						sprintf(file_string,"%05d // Service Timer #%2d-%2d DOW Off Hr ",svc_t[j].off_hour[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
						sprintf(file_string,"%05d // Service Timer #%2d-%2d DOW Off Min ",svc_t[j].off_minute[i],j,i);
						fl_fputs(file_string,fp1);
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);

					}

				}
			 	fl_fputc(CR,fp1);
			 	fl_fputc(LF,fp1);
				fl_fputc(0,fp1);
				exit_cmd = 1;		// data written to file

			}
			else
				exit_cmd = 10;				// existing file

			break;
			 
		case 9:		// Read Service Timers
		
			fp1 = fl_fopen("/svctmr.txt","r");
			if (fp1 != NULL)
			{
				exit_cmd = 4;				// preset error for cannot read car data
				if (get_car_data(fp1) == 1)	
				{		// Valid Car Data
					exit_cmd = 5;			// preset error for invalid car data
					if ((file_car_nmb <= 8) && (file_car_speed >= 25) && (file_car_speed <= 2000) && 
						(file_bottom_fl > 0) &&	(file_bottom_fl < file_top_fl) && 
						(file_top_fl <= file_grp_top_fl))
					{
						exit_cmd = 6;			// preset error for invalid data field
		   				fl_fgets(fdatbuf,180,fp1);				
						// Service Timers			
						if (strstr(fdatbuf,"Service Timers") != 0)	
						{		// Get Service Timer data
							for(j = 0; j < 10; j++)
							{
		   						fl_fgets(fdatbuf,180,fp1);				 
								svc_t[j].service = (uint8)atoi(fdatbuf);

		   						fl_fgets(fdatbuf,180,fp1);				 
								svc_t[j].status = (uint8)atoi(fdatbuf);

		   						fl_fgets(fdatbuf,180,fp1);				 
								svc_t[j].logic = (uint8)atoi(fdatbuf);

		   						fl_fgets(fdatbuf,180,fp1);				 
								svc_t[j].option = (uint8)atoi(fdatbuf);

								for (i=0;i<=2;i++)
								{
			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].month[i] = (uint8)atoi(fdatbuf);

			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].day[i] = (uint8)atoi(fdatbuf);

			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].md_on_hour[i] = (uint8)atoi(fdatbuf);

			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].md_on_minute[i] = (uint8)atoi(fdatbuf);

			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].md_off_hour[i] = (uint8)atoi(fdatbuf);

			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].md_off_minute[i] = (uint8)atoi(fdatbuf);
								}
								for (i=0;i<=6;i++)
								{
			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].on_hour[i] = (uint8)atoi(fdatbuf);

			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].on_minute[i] = (uint8)atoi(fdatbuf);

			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].off_hour[i] = (uint8)atoi(fdatbuf);

			   						fl_fgets(fdatbuf,180,fp1);				 
									svc_t[j].off_minute[i] = (uint8)atoi(fdatbuf);
								}
								Wrt_Grp_Svc_Timer (j);
							}
							exit_cmd = 2;		// no error set command for data read
						}
					}
				}
			}
			break;
			 
			
			
		case 10:	// Write Security Data to file (include call mask)
			fp1 = fl_fopen("/security.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/security.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				write_display("Writing: Please Wait",2,0);
				fp1 = fl_fopen("/security.txt","w");
				put_car_data(fp1);
				
				fl_fputs("Code Table",fp1);
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
			  	
				for (i=1;i<=cons[grtopf];i++)
				{
					Read_CodeTbl(i, &CodeTable, LookUpTable);
					sprintf(file_string,"Floor # %2d",i);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
					sprintf(file_string,"%05d // Number of Codes",CodeTable.NumCode);
					fl_fputs(file_string,fp1);
					fl_fputc(CR,fp1);
					fl_fputc(LF,fp1);
					for (j=0;j<(NumMaxCd/10);j++)
					{
						for(k=0;k<=9 ;k++)
						{
							if ((CodeTable.Codes[(j*10) + k][0] & 0x80) == 0)
								sprintf(file_string,"[%2d,%2d,%2d,%2d,F],",(CodeTable.Codes[(j*10) + k][0] & 0x7F),CodeTable.Codes[(j*10) + k][1],CodeTable.Codes[(j*10) + k][2],CodeTable.Codes[(j*10) + k][3]);
							else
								sprintf(file_string,"[%2d,%2d,%2d,%2d,R],",(CodeTable.Codes[(j*10) + k][0] & 0x7F),CodeTable.Codes[(j*10) + k][1],CodeTable.Codes[(j*10) + k][2],CodeTable.Codes[(j*10) + k][3]);
							fl_fputs(file_string,fp1);
						}
						fl_fputc(CR,fp1);
						fl_fputc(LF,fp1);
					}
					
				}
				fl_fputc(CR,fp1);
				fl_fputc(LF,fp1);
				fl_fputc(0,fp1);
				exit_cmd = 1;		// data written to file

			}
			else
				exit_cmd = 10;				// existing file

			break;
		case 11:	// Read Security Data from file
			fp1 = fl_fopen("/security.txt","r");
			if (fp1 != NULL)
			{
				exit_cmd = 4;				// preset error for cannot read car data
				if (get_car_data(fp1) == 1)	
				{		// Valid Car Data
					exit_cmd = 5;			// preset error for invalid car data
					if ((file_car_nmb <= 8) && (file_car_speed >= 25) && (file_car_speed <= 2000) && 
						(file_bottom_fl > 0) &&	(file_bottom_fl < file_top_fl) && 
						(file_top_fl <= file_grp_top_fl))
					{
						exit_cmd = 6;			// preset error for invalid data field
		   				fl_fgets(fdatbuf,180,fp1);				
						// Code Table			
						if (strstr(fdatbuf,"Code Table") != 0)	
						{		// Get Code Table data
							
							for (i=1;i<=cons[grtopf];i++)
							{
								exit_cmd = 6;			// preset error for invalid data field
				   				fl_fgets(fdatbuf,180,fp1);				
								// Start of new floor			
								if (strstr(fdatbuf,"Floor") == 0)
									break;	

		   						fl_fgets(fdatbuf,180,fp1);					// Get the number of valid codes		 
								CodeTable.NumCode = (int16)atoi(fdatbuf);
								if (CodeTable.NumCode > NumMaxCd)
									break;
								for (j=0;j<(NumMaxCd/10);j++)
								{
									for(k=0;k<=9 ;k++)
									{
									
										char_in = (char)fl_fgetc(fp1);	// [
										if (char_in != '[')
											goto exit_switch;
										if (get_valid_code(fp1) == 0)
											goto exit_switch;
										
										CodeTable.Codes[(j*10) + k][0] = (uint8)atoi(fdatbuf);
										
										char_in = (char)fl_fgetc(fp1);	// ,
										if (char_in != ',')
											goto exit_switch;
										
										if (get_valid_code(fp1) == 0)
											goto exit_switch;
										
										CodeTable.Codes[(j*10) + k][1] = (uint8)atoi(fdatbuf);

										char_in = (char)fl_fgetc(fp1);	// ,
										if (char_in != ',')
											goto exit_switch;
										
										if (get_valid_code(fp1) == 0)
											goto exit_switch;
										
										CodeTable.Codes[(j*10) + k][2] = (uint8)atoi(fdatbuf);

										char_in = (char)fl_fgetc(fp1);	// ,
										if (char_in != ',')
											goto exit_switch;
										
										if (get_valid_code(fp1) == 0)
											goto exit_switch;
										
										CodeTable.Codes[(j*10) + k][3] = (uint8)atoi(fdatbuf);
										
										char_in = (char)fl_fgetc(fp1);	// ,
										if (char_in != ',')
											goto exit_switch;
										
										char_in = (char)fl_fgetc(fp1);	// F or R
										if ((char_in != 'F') && (char_in != 'R'))
											goto exit_switch;
										if (char_in == 'R')
											CodeTable.Codes[(j*10) + k][0] |= 0x80;	
										
										char_in = (char)fl_fgetc(fp1);	// ]
										if (char_in != ']')
											goto exit_switch;
										char_in = (char)fl_fgetc(fp1);	// ,
										if (char_in != ',')
											goto exit_switch;
									}
									char_in = (char)fl_fgetc(fp1);		// CR
									if (char_in != CR)
										goto exit_switch;
									char_in = (char)fl_fgetc(fp1);		// LF
									if (char_in != LF)
										goto exit_switch;
								}
								Write_CodeTbl(i, &CodeTable, LookUpTable);
								exit_cmd = 2;		// no error set command for data read
							}
						}
					}
				}
			}
			break;
			
		case 12:		// Write Fault Log
			fp1 = fl_fopen("/fault.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/fault.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				write_display("Writing: Please Wait",2,0);
				fp1 = fl_fopen("/fault.txt","w");
				put_car_faults(cons[carnmb],fp1);

				exit_cmd = 1;		// data written to file
			}
			else
				exit_cmd = 10;				// existing file
			break;

		case 13:		// Write Long Term Fault Log
			fp1 = fl_fopen("/ltfault.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/ltfault.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				write_display("Writing: Please Wait",2,0);
				fp1 = fl_fopen("/ltfault.txt","w");
				put_car_lt_faults(fp1);

				exit_cmd = 1;		// data written to file
			}
			else
				exit_cmd = 10;				// existing file
			break;

		case 14:		// Write Elevator Statistics
			fp1 = fl_fopen("/jobstats.txt","r");
			if ((fp1 == NULL) || (force_write == 1))
			{
				if (force_write == 1)
				{	
					if (fp1 != NULL)
						fl_fclose(fp1);
					
					if (fl_remove("/jobstats.txt") != 0)
					{
						exit_cmd = 9;		// file delete error
						break;
					}
				}
				write_display("Writing: Please Wait",2,0);
				fp1 = fl_fopen("/jobstats.txt","w");
				
				put_car_stats(fp1);

				exit_cmd = 1;		// data written to file
			}
			else
				exit_cmd = 10;				// existing file
			break;


		default:
			break;
	}
	
exit_switch:
	
	delay(10000);
	fl_fclose(fp1);
	enable_dog();
	petdog();
	
	return exit_cmd;

}


uint16 Load_Store_Program (uint16 cmd)
{
	uint16 sd_error = 0;
	
	switch (cmd)
	{
		case 0:		// initiallize sd card
			sd_error = sd_initialization();
			// Initialise File IO Library
			if (sd_error == 0)
				fl_init();
			break;
			
		case 1:		// read block (temporary)
			break;
			
		case 2:		// write block (temporary)
			break;
				
		case 3:		// read cons.dat file
			break;
			
		case 4:		// write cons.dat file
			break;
			
		default:
			break;
	}
	
	return sd_error;
}

#if (Simulator == 1)
uint16 dayLogStartIx = 0;
uint16 dayNmbCallsLogged = 0;
uint16 nmbCallsWritten = 0;

Log_Ix_Type logIndex;


FL_FILE *fp2 = NULL;

uint8 writeTrafficLogToSD(void);

//*****************************************
//	Routine to test logging of the calls
//	Write traffic log to SD Card.
//	This routine is used for testing
//	purposes only, for reading data saved on
//	MRAM and save it as a csv file to
//	open on the computer.
//*****************************************

uint8 writeTrafficLogToSD(void)
{
	int16 i;
	int16 sd_err = 0;
	static uint8 state = 0;
	
	switch(state)
	{
		case 0:
				// Check if you have any calls logged for the requested date, e.g. 12/10/15		MM/DD/YY
				if (findLogForDate(12,10,15,&dayLogStartIx) != 0)
				{
					logGetIx = dayLogStartIx;							// Set logGetIx browsing pointer
					readLogIxFromMRAM(logGetIx,&logIndex);				// Read Day's 1nd Header Data
					getNextLogIx(&logIndex);							// Read Day's 2nd Header Data
					local_getdate();
					if ((d.month == 12) && (d.day == 10) && ((d.year%100) == 15))	// When log requested for today's date
						dayNmbCallsLogged = nmbLoggedCallsToday;
					else
						dayNmbCallsLogged = logIndex.header2.W1.nmb_calls;
					state = 1;
				}
				else
				{
					state = 6;
				}
				break;

		case 1:
				// Check and see if the sd card is initialized
				if (sd_init == 0)
				{
					sd_err = sd_initialization();
					// Initialise File IO Library
					if (sd_err == 0)
					{
						fl_init();
						// Attach media access functions to library
						if (fl_attach_media(media_read, media_write) != FAT_INIT_OK)
							state = 6;	// Media Init failed
						else
							state = 2;
					}
					else
						state = 6;		// sd init error
				}
				else
					state = 2;
				break;
				
		case 2:
				// Check if dispatcher_log directory exists
				disable_dog();
				if (fl_is_dir("/traffic_log/") == 0)
					fl_createdirectory("/traffic_log/");
				else
					state = 3;
				enable_dog();
				break;
				
		case 3:
				// check for new valid file name
				local_getdate();
				sprintf(file_string,"/traffic_log/%.2d-%.2d-%.2d.csv",d.month,d.day,d.year);
				disable_dog();

				fp2 = fl_fopen(file_string, "r");
				if (fp2 != NULL)	// error file already exists
				{
					fl_fclose(fp2);
					state = 6;
				}
				else				// valid file name
				{
					fl_fclose(fp2);
					state = 4;
				}
				enable_dog();
				break;
				
		case 4:
				// open the file for writing new data
				disable_dog();
				fp2 = fl_fopen(file_string,"w");
				state = 5;
				enable_dog();
				break;
				
		case 5:	
				disable_dog();
				i = 0;
				while (i < dayNmbCallsLogged)
				{
					getNextLogIx(&logIndex);
					sprintf(file_string,"%d,%d,%d,%d,%d,%d:%d:%d,%d,%d,%d,%d",
												   logIndex.call_log.W1.valid_log,
												   logIndex.call_log.W1.call_type,
												   logIndex.call_log.W1.hc_fl,
												   logIndex.call_log.W1.cc_fl,
												   logIndex.call_log.W2.car,
												   logIndex.call_log.W3.entry_hour,
												   logIndex.call_log.W2.entry_min,
												   logIndex.call_log.W2.entry_sec,
												   logIndex.call_log.W3.t_completed,
												   logIndex.call_log.W3.call_dtype,
												   logIndex.call_log.W3.hcdir,
												   logIndex.call_log.W3.spare_bit);
					fl_fputs(file_string,fp2);
					fl_fputc(CR,fp2);
					fl_fputc(LF,fp2);
					i++;
				}
				fl_fputc(0,fp2);
				fl_fclose(fp2);
				enable_dog();
				state = 6;
				break;
				
		case 6:
				// log write on sd complete, either with or without error
				state = 0;
				break;
	}	
	return state;
}

#endif

		
/*

Revision History::

12/18/12 v6.0.26 mhd 	1. Added logic and option variables in service timer structure.
3/21/14 v7.0.37 mhd		1. Added reading and writing SPB and NTS field variables.
						2. Set Hoistway_Learned flag if read of hoistway table is verified okay.
						3. Display additional fault data
7/23/14 v7.1.6 mhd		1. Added additional faults
2/19/15 v7.1.26 mhd		1. In Set_Device_Name() check for CTCAN device to be greater or equal to c_ctcan_SEB_4 to add a device number. Devices lower than this 
                           have a specific name.
11/04/15 v7.1.45 as		1. Added writeTrafficLogToSD() to write log date on SD Card reading from MRAM.
						   It is also an example for showing how to get log data for a given date as parameter.
						   Only written for simulator.
3/9/17 v8.0.1 mhd		1. Added printout for statusf3 and statusf4.
2*/