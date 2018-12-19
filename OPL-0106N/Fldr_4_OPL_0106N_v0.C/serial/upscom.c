/*

	UPS Serial Communications on Serial Expansion port 1
	
*/
#define d_UPSCOM 1
#include "global.h"

// *************************************
//        UPS Variables
// *************************************

// Ups status
int16 ups_bat_status = 0;				// power loss ups battery status
#define c_ups_on 0x08
#define c_on_battery 0x10
#define c_low_battery 0x40
#define c_utility_lost 0x20
#define c_battery_charging 0x80

unsigned char ups_low_bat = 0;			// power loss ups low battery 
unsigned char ups_on_bat_pwr = 0;		// power loss ups on battery power
unsigned char ups_turned_off = 0;		// power loss ups turned on
unsigned char ups_com_online = 1;		// power loss ups online
unsigned char ups_online_count = 0;		// ups online count
int16 ups_com_tx_cnt = 0;					// transmit count
int16 ups_com_rx_cnt = 0;					// receive count
//char bat_volt[6] = {'0','0','.','0','0',0};	// Battery voltage
char bat_cap[6] = {'0','0','.','0','0',0};	// Battery capacity
//float batvolt = 0.0;					// floating point battery voltage
float batcap = 100.0;						// floating point battery capacity
unsigned char ups_com_seq = 0;			// power loss ups comm sequence
unsigned char ups_rcv_seq = 0;			// power loss ups receive sequence
unsigned char ups_packet_req = 0;		// ups comm packet request flag
int16 ups_pack_len = 0;			// ups comm pack langth
int16 ups_unpack_len = 0;		// ups comm unpack length 
int16 ups_rcv_start = 0;		// receive start location
unsigned char ups_rcvbuf[sx_rbufsize+1];			// ups packet receive buffer
unsigned char ups_trmbuf[sx_tbufsize+1];			// ups packet transmit buffer
int16 ups_rcv_ix = 0;						// block receive index
int16 ups_trm_ix = 0;						// transmit index
int16 rx_ix = 0;							// packet receive index
int16 ups_fault_latch = 0;				// bits to prevent multiple faults on one occurance

char block_id=0;
unsigned char rcv_chksum=0;
unsigned char pkt_len=0;
unsigned char pkt_seq_num=0;
unsigned char map_init;
bool pkt_flag;							// swap between batter capacity and alarm request
unsigned char pkt_seq_cnt = 0;			// packet sequence count

int16 OnBatteryAlarm_loc = 0;
int16 LowBattery_loc = 0;
int16 BatteryCharging_loc = 0;
int16 ShutdownImminentAlarm_loc = 0;
int16 AutoShutdownPending_loc = 0;
int16 UtilityNotPresent_loc = 0;
int16 alarm_size = 0;
int16 BatteryCapacity_loc = 0;
int16 BatteryCapacityType = 0;
int16 map_size = 0;


void pack_apc_ups_com (void);
void unpack_apc_ups_com (void);
void UPS_comm(void);

void pack_eaton_ups_com (int16 pkt);
void trm_eaton_ups_com (void);
void unpack_eaton_ups_com (void);
int16 valid_ups_char(char ch);
void get_meter_and_alarm_location(int16 start_ix);
void get_meter_status(int16 start_ix);
void get_status(int16 start_ix);
void get_alarm_status(int16 start_ix);

void pack_eaton_ups_shut_com (int16 pkt);
void trm_eaton_ups_shut_com (void);
void unpack_eaton_ups_shut_com (void);
extern uint8 ascii_to_hex(uint8 value);


unsigned char tx_sync = 0;
unsigned char rx_sync = 0;
unsigned char last_pkt = 0;
unsigned char tx_ack = 0;
unsigned char notify = 0;

extern int16 dbgscn;
extern int16 freeze;	

static union {
		uint8 b[4];			// byte: char 0 - 3
		uint16 w[2];		// word: short int 0 - 1
		uint32 dw;			// double word: int
	}uiu;					// unsigned integer union
		


// Comm to UPS
void UPS_comm(void)
{
	if (cons[UPS_Type] == 1)
	{		// Eaton
		trm_eaton_ups_com();
		unpack_eaton_ups_com();
	}
	else if (cons[UPS_Type] == 2)
	{		// Eaton
		trm_eaton_ups_shut_com();
		unpack_eaton_ups_shut_com();
	}
	else
	{	  // APC
		pack_apc_ups_com();
		unpack_apc_ups_com();
	}

/* 
#if (Simulator == 1)
	ups_com_online = 1;
	ups_bat_status = 8;
	batcap = 100.0;
#endif
*/
	if (ups_low_bat == 1)
	{
		if ((ups_fault_latch & 0x01) == 0)
		{
			record_fault(f_pwl_ups_low_bat);
			ups_fault_latch |= 0x01;
		} 
	}
	else
		ups_fault_latch &= ~0x01;

	if (ups_com_online == 0)
	{
		if ((ups_fault_latch & 0x02) == 0)
		{
			record_fault(f_pwl_ups_online);
			ups_fault_latch |= 0x02;
		} 
	}
	else
		ups_fault_latch &= ~0x02;
	if (ups_on_bat_pwr == 1)
	{
		if ((ups_fault_latch & 0x04) == 0)
		{
			record_fault(f_pwl_ups_on_bat_pwr);
			ups_fault_latch |= 0x04;
		} 
	}
	else
		ups_fault_latch &= ~0x04;
	if (ups_turned_off == 1)
	{
		if ((ups_fault_latch & 0x08) == 0)
		{
			record_fault(f_pwl_ups_turned_off);
			ups_fault_latch |= 0x08;
		} 
	}
	else
		ups_fault_latch &= ~0x08;

	if (batcap < ((float)fvars[fvbatcap]/10.0))
	{
		if ((ups_fault_latch & 0x10) == 0)
		{
			record_fault(f_pwl_ups_low_bat_cap);
			ups_fault_latch |= 0x10;
		} 
	}
	else
		ups_fault_latch &= ~0x10;

	if (fvars[fvsrvups] == 1)	// override shutdown to service ups
		pwl_ups_fault = 0;
	else if ((cons[PWL_Brake] == 1) || ((cons[PWL_Brake] == 2) && (cons[Emp_Recover] == 2)))
	{
		if ((ups_com_online == 0) || (ups_low_bat == 1) || (ups_on_bat_pwr == 1) || (ups_turned_off == 1) || (batcap < ((float)fvars[fvbatcap]/10.0)))
			pwl_ups_fault = 1;
		else
			pwl_ups_fault = 0;
	}
	else
		pwl_ups_fault = 0;

/*	if (pwl_ups_fault == 1)
	{
		if ((dpy_mode == 4) && (dbg_page == 6))
			freeze = 1;
	}
*/

}

// Pack data for Power Loss UPS battery backup device
/*
		Commands to UPS				UPS Response
			Y = Smart Mode 				SM<cr><lf>
			B = Battery Voltage			vv.vv<cr><lf>
			L = Line Voltage			vvv.v<cr><lf>
			O = Output Voltage			vvv.v<cr><lf>
			P = Load Power				www.w<cr><lf>
										!=on battery (no cr,lf)
										$=power retuned
										?=Unit powered on
			Q = UPS status			Status 
										bit 0: Calibration
										bit 1: Sleeping
										bit 2: Smart Boost
										bit 3: Powered On
										bit 4: On battery
										bit 5: Unknown
										bit 6: Battery low
										bit 7: Battery Charging
			f = Battery capacity		ccc.c %
			j = Run time remainning		tttt: minutes
*/

void pack_apc_ups_com (void)
{

	if (((timers[tsx_txA] > 20) && (ups_packet_req == 0)) || (timers[tsx_txA] > 100))
	{
		if (sx_Tranbuf_Empty[SE1COM] == 1)
		{
			switch (ups_com_seq)
			{
				case 0: 
					sx_enable_tx(SE1COM);
					sx_sendc(SE1COM,'Y');
					ups_packet_req = 1;
					ups_rcv_seq = 0;
					ups_rcv_ix = 0;
					ups_com_tx_cnt++;
					break;
				case 1:
					sx_enable_tx(SE1COM);
					sx_sendc(SE1COM,'Q');
					ups_packet_req = 1;
					ups_rcv_ix = 0;
					ups_rcv_seq = 0;
					ups_com_tx_cnt++;
					break;
				case 2:
					sx_enable_tx(SE1COM);
					sx_sendc(SE1COM,'f');
					ups_packet_req = 1;
					ups_rcv_ix = 0;
					ups_rcv_seq = 0;
					ups_com_tx_cnt++;
					break;
				default:
					ups_com_seq = 0;
					break;				
			}
		}
										   
		timers[tsx_txA] = 0;
		ups_rcv_ix = 0;
	}
}


int16 valid_ups_char(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return 1;
	else if ((ch == '.') || (ch == 'S') || (ch == 'M'))
		return 1;
	else
		return 0;
}

// Unpack data for Power Loss UPS battery backup device

void unpack_apc_ups_com (void)
{
  	int16 rbempt=0,endmess=0;

	if ((ups_packet_req == 1) && (sx_Tranbuf_Empty[SE1COM] == 1))
	{
	  	rbempt = (uint8)sx_isrxempty(SE1COM);
	  	while((rbempt == 0) && (endmess == 0))
	  	{
		  	ups_rcvbuf[ups_rcv_ix] = (uint8)sx_asigetc(SE1COM);
		  	rbempt = (uint8)sx_isrxempty(SE1COM);
			switch (ups_rcv_seq)
			{
				case 0:		// data stored so look for CR
					if (ups_rcvbuf[ups_rcv_ix] == CR)
						ups_rcv_seq = 1;
					break;
				case 1:		// Have CR look for LF
					if (ups_rcvbuf[ups_rcv_ix] == LF)
					{	// Decode packet
						if (ups_com_seq == 0)
						{
							if ((ups_rcv_ix >= 3) && (valid_ups_char(ups_rcvbuf[ups_rcv_ix-2]) == 1) && (valid_ups_char(ups_rcvbuf[ups_rcv_ix-3]) == 1))
							{
								if ((ups_rcvbuf[ups_rcv_ix-2] == 'M') && (ups_rcvbuf[ups_rcv_ix-3] == 'S'))
								{						
									ups_com_seq = 1;
									ups_com_online = 1;
									ups_online_count = 0;
									ups_packet_req = 0;
									timers[tsx_rxA] = 0;
									ups_com_rx_cnt++;
								}
								ups_rcvbuf[ups_rcv_ix] = 0;
								ups_rcvbuf[ups_rcv_ix-1] = 0;
								ups_rcvbuf[ups_rcv_ix-2] = 0;
								ups_rcvbuf[ups_rcv_ix-3] = 0;
								endmess = 1;			
							}
						}
						else if (ups_com_seq == 1)
						{
							if ((ups_rcv_ix >= 3) && (valid_ups_char(ups_rcvbuf[ups_rcv_ix-2]) == 1) && (valid_ups_char(ups_rcvbuf[ups_rcv_ix-3]) == 1))
							{
								if ((ups_rcvbuf[ups_rcv_ix-2] != 'M') && (ups_rcvbuf[ups_rcv_ix-3] != 'S'))
								{

									ups_bat_status = (ascii_to_hex((uint8)(ups_rcvbuf[ups_rcv_ix-2]) & 0x0f));
									ups_bat_status |= ((ascii_to_hex((uint8)(ups_rcvbuf[ups_rcv_ix-3]) & 0x0f) << 4));
									if ((ups_bat_status & c_low_battery) != 0)
										ups_low_bat = 1;
									else
										ups_low_bat = 0;
									if ((ups_bat_status & c_on_battery) != 0)
										ups_on_bat_pwr = 1;
									else
										ups_on_bat_pwr = 0;

									if ((ups_bat_status & c_ups_on) == 0)
										ups_turned_off = 1;
									else
										ups_turned_off = 0;

									ups_com_seq = 2;
									ups_com_online = 1;
									ups_online_count = 0;
									ups_com_rx_cnt++;
									timers[tsx_rxA] = 0;
								}
								else
									ups_com_seq = 0;			// get another SM message
							}
							else
								ups_com_seq = 0;			// get another SM message

							ups_rcvbuf[ups_rcv_ix] = 0;
							ups_rcvbuf[ups_rcv_ix-1] = 0;
							ups_rcvbuf[ups_rcv_ix-2] = 0;
							ups_rcvbuf[ups_rcv_ix-3] = 0;

							endmess = 1;			
							ups_packet_req = 0;
						}
						else if (ups_com_seq == 2)
						{
							if ((ups_rcv_ix >= 5) && (valid_ups_char(ups_rcvbuf[ups_rcv_ix-2]) == 1) && (valid_ups_char(ups_rcvbuf[ups_rcv_ix-3]) == 1) &&
								(valid_ups_char(ups_rcvbuf[ups_rcv_ix-4]) == 1) && (valid_ups_char(ups_rcvbuf[ups_rcv_ix-5]) == 1))
							{
								if ((ups_rcvbuf[ups_rcv_ix-2] != 'M') && (ups_rcvbuf[ups_rcv_ix-3] != 'S'))
								{
									if ((ups_rcv_ix == 6) && (valid_ups_char(ups_rcvbuf[ups_rcv_ix-6]) == 1))
										bat_cap[0] = ups_rcvbuf[ups_rcv_ix-6];
									else
										bat_cap[0] = ' ';
									bat_cap[1] = ups_rcvbuf[ups_rcv_ix-5];
									bat_cap[2] = ups_rcvbuf[ups_rcv_ix-4];
									bat_cap[3] = ups_rcvbuf[ups_rcv_ix-3];
									bat_cap[4] = ups_rcvbuf[ups_rcv_ix-2];
									bat_cap[5] = 0;

									batcap = (float)atof(bat_cap);

									ups_com_seq = 0;
									ups_com_online = 1;
									ups_online_count = 0;
									timers[tsx_rxA] = 0;
									ups_com_rx_cnt++;
								}
								else
									ups_com_seq = 0;			// get another SM message
							}
							else
								ups_com_seq = 0;			// get another SM message

							ups_rcvbuf[ups_rcv_ix] = 0;
							ups_rcvbuf[ups_rcv_ix-1] = 0;
							ups_rcvbuf[ups_rcv_ix-2] = 0;
							ups_rcvbuf[ups_rcv_ix-3] = 0;
							ups_rcvbuf[ups_rcv_ix-4] = 0;
							ups_rcvbuf[ups_rcv_ix-5] = 0;
							ups_rcvbuf[ups_rcv_ix-6] = 0;

							ups_packet_req = 0;
							endmess = 1;			
						}
					}
					ups_rcv_seq = 0;
					break;
				default:
					ups_rcv_seq = 0;
					break;
			}
		  	ups_rcv_ix++;
			if (ups_rcv_ix > 10)
			{
				rbempt = 1;
				ups_rcv_ix = 0;
			}
		}
	}
	if (ups_packet_req == 1)
	{			 // Data has been transmitted to device
		if (timers[tsx_rxA] > 60)
		{ 
			ups_packet_req = 0;
			ups_online_count++;
			if (ups_online_count >= 10)
			{
				ups_online_count = 0;
				ups_com_online = 0;
			}
			timers[tsx_rxA] = 0;
			init_sx_com_port_1();
		}
	}
	else
		timers[tsx_rxA] = 0;
}

/*
	Eaton XCP UPS Communications Protocol
	
		Request Data 
			SFD				0xAB Start Frame Delimeter
			Length			0xx (max 127 bytes)
			Cmd/Data		0xx
			Checksum		Two's complement checksum  (Starts with SFC through last data byte)

		Response Data
			SFD				0xAB Start Frame Delimeter
			Data Block #    Data Block # from request (CMD - 0x30)
			Length			0xx (max 121 bytes)
			Sequence #		0xx (1 of multiple packets if packet > 121 bytes)
			Data			0xx
			Checksum		Two's complement checksum  (Starts with SFC through last data byte)
	

	Standard ID Block 		(The standard Id Block is required to get the Meter and Alarm Maps)
		Request: SFD, LEN, BLOCK, CS
		        0xAB, 0x01, 0x31, 0x23
		Response: SFD, BLOCK, LEN, SEQ,	Data (see table and XCP Spec.)
				 0xAB, (0x31-0x30) = 0x01, Length, Seq

		Data:
			Bytes 	Name 					Description / Values
			1 		Num CPUs 				The count of CPU version numbers which follows this byte
			2@ 		CPU Versions 			See below
			1		KVA Rating 				Two cases:
												1) If non-zero, than this byte is the KVA rating of the UPS. In this case, there is
													no Extended VA Rating field.
												2) If zero, then the next two bytes give the Extended VA Rating. This is the
													preferred implementation.
			(2) 	Extended VA Rating 		If present (KVA rating field equals 0), then this field is the UPS VA rating divided
												by 50; e.g., a value of 0x000F would be for a 750 VA UPS.
			1 		Output Phases 			This byte is the number of output phases for the unit.
			1 		Output Phase Angle 		This byte provides the output phase angle, in degrees.
			1 		Message Length 			This byte is the number of bytes (dml) that follow in the Descriptive Message.
			(dml) 	Descriptive Message 	The descriptive message is a string of ASCII character bytes that describe the
												unit type. An example would be 'Powerware 150 Model 125'.
			1 		Meter Map Size 			This byte gives the length of the Meter Map (next mms bytes).
			(mms) 	Meters Map 				This table of bytes describes the type of metered data that will be received. See
												the Meters Map Table for a description of these bytes
			1 		Alarm Map Size 			This byte gives the length of the Alarm Map (next ams bytes).
			(ams) 	Alarm Map 				This table of bytes describes which alarm types are present in the system. See
												the Alarm Map Table for a description of these bytes.
			2 		Config Block Length 	This word is the number of bytes in the data field of the Configuration block. Its
												presence can be used to determine that this unit can respond to a Config block
												request. Also, if this value is zero, then no configuration block is available but a
												statistics map MAY be present.
			1 		Statistics Map Size 	This gives the length of the Statistics Map (next sms bytes). Its presence can be
												used to determine that this unit can respond to a Statistics block request.
												Additionally, if this value is zero, then no statistics block is available.
			(sms) 	Statistics Map 			This table of bytes describes the type of statistics data that will be received. See
												the Statistics Map Table for a description of these bytes.
			2 		Size of the Alarm		Value indicates the maximum possible number of bytes in this block for this UPS.
					History Log					On UPSs conforming to BCM revs before AE, there may be an Alarm History log
												of up to 3600 bytes, even though this field is not present.
			2 		Size of the Custom	   	Set to 0.
					Events block
			2 		Size of the Topology   	Size of the UPS Topology block; size 0 if there is no UPS Topology block.
					block
			1 		Maximum Supported	   	Indicates the longest binary command that this UPS can process (including the header
					Command Length		   		and checksum bytes). May be the length of the UPS’ receive buffer.
			2 		Size of the Command		Number of bytes in the Command List block; size 0 if there is no Command List
					List block				block.
			2 		Size of the Outlet		Size of the Outlet Monitoring block; size 0 if there is no Outlet block.
					Monitoring block
			2 		Size of the Alarm		Maximum Size of the Active Alarms block
					block


			Additional Field Descriptions:
			CPU Versions: Each Version number consists of two two-digit BCD values, with Minor version number given in the
			first byte, and Major version number given in the second.  A value of 0x0317 is Version 3.17.
			The fixed order for the first five CPUs is defined as:

			Order 	Version for CPU: 		Description
			1 		Control Panel 			These two bytes are for the Control Panel or Communications Board Software
											Version Number. If there is only one CPU, this is the only version number given.
			2 		Inverter 				These 2 bytes are for the Version Number of the Inverter or Central Control Unit
											(CCU) CPU.
			3 		Rectifier 				These 2 bytes are for the Version Number of the Rectifier Software CPU.
			4 		Net Interface Card 		These 2 bytes are for the Version Number of the Network Interface Card CPU
											(internal or external network).
			5 		Display Controller 		If a separate display controller is used.

			If the second or third CPUs are not implemented, but the third or fourth are, then placeholder values of 00.00 must
			be given for the missing CPUs.
			There may be more than 5 CPUs, as indicated in the Num CPUs field.

*/

void pack_eaton_ups_com (int16 pkt)
{
	int16 i;
	unsigned char chksum = 0;

	switch (pkt)
	{
		case 1: 			// Request Standard ID Block  (get meter and alarm map)
			ups_trmbuf[0] = 0xAB;
			ups_trmbuf[1] = 0x01;
			ups_trmbuf[2] = 0x31;
			ups_pack_len = 3;			
			ups_packet_req = 1;
			break;

		case 3:				// Ups Status
			ups_trmbuf[0] = 0xAB;
			ups_trmbuf[1] = 0x01;
			ups_trmbuf[2] = 0x33;
			ups_pack_len = 3;
			ups_packet_req = 1;
			break;

		case 4:				// Battery Capacity
			ups_trmbuf[0] = 0xAB;
			ups_trmbuf[1] = 0x01;
			ups_trmbuf[2] = 0x34;
			ups_pack_len = 3;
			ups_packet_req = 1;
			break;

		case 5:				// Alarms Request
			ups_trmbuf[0] = 0xAB;
			ups_trmbuf[1] = 0x01;
			ups_trmbuf[2] = 0x35;
			ups_pack_len = 3;
			ups_packet_req = 1;
			break;

		default:
			break;				
	}

	chksum = 0;
	for (i=0;i<ups_pack_len;i++)
	{
		chksum += ups_trmbuf[i];
	}
	chksum = (uint8)(~chksum + 1);
	ups_trmbuf[i] = chksum;
	ups_trm_ix = 0;
	ups_rcv_ix = 0;
	pkt_seq_cnt = 0;
}

void trm_eaton_ups_com (void)
{
	unsigned char pkt_req = 0;

	if (((timers[tsx_txA] > 20) && (ups_packet_req == 0)) || (timers[tsx_txA] > 100))
	{					// 2 second delay only if packet unpacked  or timed out otherwise wait up to 10 seconds 
		if (sx_Tranbuf_Empty[SE1COM] == 1)
		{
			if (map_init == 1)
			{
				if (pkt_flag == 0)
				{			// Request status
					pkt_flag = 1;
					pkt_req = 3;
				}
				else
				{			// Request battery capacity
					pkt_flag = 0;
					pkt_req = 4;
				}
			}
			else
			{				// Request map
				pkt_flag = 0;
				pkt_req = 1;
			}

			pack_eaton_ups_com(pkt_req);

			sx_enable_tx(SE1COM);
			while (ups_trm_ix <= ups_pack_len)
			{
				sx_sendc(SE1COM,ups_trmbuf[ups_trm_ix]);
				ups_trm_ix++;
			}
			ups_packet_req = 1;
			ups_com_tx_cnt++;
		}
		timers[tsx_txA] = 0;
	}
}

void unpack_eaton_ups_com (void)
{
  	int rbempt=0,endmess=0;
	unsigned char rcv_char = 0;

	if ((ups_packet_req == 1) && (sx_Tranbuf_Empty[SE1COM] == 1))
	{
	  	rbempt = sx_isrxempty(SE1COM);
	  	while((rbempt == 0) && (endmess == 0))
	  	{
		  	rcv_char = (uint8)sx_asigetc(SE1COM);
		  	rbempt = sx_isrxempty(SE1COM);
			switch (ups_rcv_seq)
			{
				case 0:		// Start Frame Byte
					if (rcv_char == 0xAB)
					{
					    rcv_chksum = 0xAB;	
						ups_rcv_seq = 1;
					}
					else
						ups_rcv_ix = 0;
					rx_ix = 0;
					break;
				case 1:		// look for block #
					block_id = rcv_char;
					if (block_id < 20)
					{
						rcv_chksum += rcv_char;
						ups_rcv_seq = 2;
					}
					else
						ups_rcv_seq = 0;		// invalid block id
					rx_ix = 0;
					break;
				case 2:		// get length
					pkt_len = rcv_char;
					if (pkt_len < 125)
					{
						rcv_chksum += rcv_char;
						ups_rcv_seq = 3;
					}
					else
						ups_rcv_seq = 0;		// invalid length
					rx_ix = 0;
					break;
				case 3:		// sequence number
					pkt_seq_num = rcv_char;
					rcv_chksum += rcv_char;
					ups_rcv_seq = 4;
					if ((pkt_seq_num & 0x7F) == 0x01)
					{
						ups_rcv_ix = 0;
						ups_rcv_start = ups_rcv_ix;
						pkt_seq_cnt = 0;
					}
					pkt_seq_cnt++;
					rx_ix = 0;
					break;
				case 4:
					ups_rcvbuf[ups_rcv_ix] = rcv_char;
					ups_rcv_ix++;
					rx_ix++;
					if (rx_ix < pkt_len)
					{
						rcv_chksum += rcv_char;
					}
					else
					{
						rcv_chksum += rcv_char;
						rcv_chksum = (uint8)(~rcv_chksum + 1);
						ups_rcv_seq = 5;
					}
					break;
				case 5:
					if (rcv_char == rcv_chksum)
					{		// valid packet
						if (((pkt_seq_num & 0x80) != 0) && ((pkt_seq_num & 0x7F) == pkt_seq_cnt))
						{		// received all data
							if (block_id == 1)
							{		// standard id
								map_init = 1;
								get_meter_and_alarm_location(ups_rcv_start);
							}
							if (block_id == 3)
								get_status(ups_rcv_start);
							if (block_id == 4)
								get_meter_status(ups_rcv_start);
							if (block_id == 5)
								get_alarm_status(ups_rcv_start);
						}
						ups_com_online = 1;
						ups_online_count = 0;
						ups_packet_req = 0;		// clear ups packet request
						timers[tsx_rxA] = 0;
						ups_com_rx_cnt++;
					}
					ups_rcv_seq = 0;		// finished
					break;
				default:
					ups_rcv_seq = 0;
					break;
			}
			if (ups_rcv_ix > 300)
			{
				rbempt = 1;
				ups_rcv_ix = 0;
			}
		}
	}
	if (ups_packet_req == 1)
	{			 // Data has been transmitted to device
		if (timers[tsx_rxA] > 60)
		{ 
			ups_packet_req = 0;
			ups_online_count++;
			if (ups_online_count >= 10)
			{
				ups_com_online = 0;
				ups_bat_status = 0;
				ups_online_count = 0;
				batcap = 0;
				sprintf(bat_cap,"%4.2f",batcap);
			}
			timers[tsx_rxA] = 0;
			init_sx_com_port_1();
		}
	}
	else
		timers[tsx_rxA] = 0;
}

void get_meter_and_alarm_location(int16 start_ix)
{
	int16 ix;
	int16 i;
	int16 j;
	int16 bit_loc = 0;
	int16 alarm_loc = 0;

	ix = start_ix;
	ix += (ups_rcvbuf[ix] * 2) + 1;	  // first byte is number of CPUs
	if (ups_rcvbuf[ix] == 0)	// ups kva is next two bytes
		ix += 3;
	else
		ix++;
	ix += 2;		// Output phase and phase angle
	ix += ups_rcvbuf[ix] + 1;		// message length + skip message
	map_size = ups_rcvbuf[ix];
	ix++;
	BatteryCapacity_loc = 0;
	for (i=1; i<=map_size; i++)
	{
		if (i <= 35)
		{
			if (ups_rcvbuf[ix] != 0)
				BatteryCapacity_loc++;
			if (i == 35)
				BatteryCapacityType = ups_rcvbuf[ix];
		}
		ix++;
	}
	alarm_size = ups_rcvbuf[ix];
	ix++;

	// Required alarm locations
	/*

		BatteryCharging(#150,byte19,bit6)

		Normal Shutdown
		OnBatteryAlarm(#168,byte22,bit0) 
		UtilityNotPresent(#59,byte8,bit3)

		Panic Shutdown 
		OnBatteryAlarm(#168,byte22,bit0) && LowBattery(#56,byte8,bit0) || 
		Shutdown Imminent Alarm(#55,byte7,bit7) || 
		Automatic Shutdown Pending(#206,byte26,bit6)
	*/

	OnBatteryAlarm_loc = 0;
	LowBattery_loc = 0;
	BatteryCharging_loc = 0;
	ShutdownImminentAlarm_loc = 0;
	AutoShutdownPending_loc = 0;
	UtilityNotPresent_loc = 0;

	for (i=1; i<=alarm_size; i++)
	{
 		for (j=0;j<=7;j++)
 		{
			if ((ups_rcvbuf[ix] & bittbl[j]) != 0)
 				alarm_loc++;
			
			bit_loc++;

			if (bit_loc == 55)	   // Shutdown Imminent
				ShutdownImminentAlarm_loc = alarm_loc;

			if (bit_loc == 56)	  // Low Battery
				LowBattery_loc = alarm_loc;

			if (bit_loc == 59)	  // Utility not present
				UtilityNotPresent_loc = alarm_loc;

			if (bit_loc == 150)	  // Battery Charging
				BatteryCharging_loc = alarm_loc;

			if (bit_loc == 168)	 // On Battery
				OnBatteryAlarm_loc = alarm_loc;

			if (bit_loc == 206)	// Auto Shutdown Pending
				AutoShutdownPending_loc = alarm_loc;
		}
		ix++;
	}
		 	
}

int BatCapDiv[16] = {  1,   2,   4,   8,  16,  32,   64,  128,
			 		 256, 512,1024,2048,4096,8192,16384,32768};
void get_meter_status(int16 start_ix)
{
	int16 ix = 0;
	
	ix = ((BatteryCapacity_loc -1) * 4) + start_ix;
	batcap = 0;
	if ((BatteryCapacityType & 0xF0) == 0xF0) 
	{		  // Fixed point
		uiu.b[lb] = ups_rcvbuf[ix];
		uiu.b[mlb] = ups_rcvbuf[ix + 1];
		uiu.b[mhb] = ups_rcvbuf[ix + 2];
		uiu.b[hb] = ups_rcvbuf[ix + 3];
		batcap = (float)uiu.dw/(float)BatCapDiv[BatteryCapacityType & 0xF];
	}
	sprintf(bat_cap,"%4.2f",batcap);
}

void get_status(int16 start_ix)
{
	ups_bat_status = 0;

	if ((ups_rcvbuf[start_ix] & 0x50) == 0x50)
	{
		ups_turned_off = 0;				 // ups is on
		ups_bat_status |= c_ups_on;
	}
	else
		ups_turned_off = 1;

	if ((ups_rcvbuf[start_ix + 1] & 0x08) != 0)
	{							// On battery power
		ups_bat_status |= c_on_battery;
		ups_on_bat_pwr = 1;
	}
	else
		ups_on_bat_pwr = 0;


	if ((ups_rcvbuf[start_ix + 1] & 0x20) != 0)
	{								// low battery
		ups_low_bat = 1;
		ups_bat_status |= c_low_battery;
	}
	else
		ups_low_bat = 0;


	if ((ups_rcvbuf[start_ix + 1] & 0x80) == 0)
		ups_bat_status |= c_utility_lost;		// Utility lost
	else if ((ups_rcvbuf[start_ix + 1] & 0x40) != 0)
		ups_bat_status |= c_battery_charging;		// battery charging
	
}

void get_alarm_status(int16 start_ix)
{

	// Match the bit locations for the APC ups since the display will match
/*		
		LowBattery(#56,byte8,bit0)
		BatteryCharging(#150,byte19,bit6)
		OnBatteryAlarm(#168,byte22,bit0) 
		UtilityNotPresent(#58,byte8,bit3)
*/
	ups_bat_status = 0;

	if ((LowBattery_loc != 0) && (ups_rcvbuf[LowBattery_loc + start_ix] != 0))
	{
		ups_low_bat = 1;
		ups_bat_status |= c_low_battery;
	}
	else
		ups_low_bat = 0;

	if ((BatteryCharging_loc != 0) && (ups_rcvbuf[BatteryCharging_loc + start_ix] != 0))
		ups_bat_status |= c_battery_charging;

	if ((OnBatteryAlarm_loc != 0) && (ups_rcvbuf[OnBatteryAlarm_loc + start_ix] != 0))
	{
		ups_bat_status |= c_on_battery;
		ups_on_bat_pwr = 1;
	}
	else
		ups_on_bat_pwr = 0;

	if ((UtilityNotPresent_loc != 0) && (ups_rcvbuf[UtilityNotPresent_loc + start_ix] != 0))
		ups_bat_status |= c_battery_charging;


	ups_turned_off = 0;
}



/*
	SHUT protocol used but EATON and other UPS devices
	SYNC = 0x16
	DATA PACKET
		Packet Type	1 byte
			0x01 - Request from Host to UPS
			0x04 - Response from UPS to Host
			0x05 - Notify from UPS to Host
			Add 0x80 on last packet or only packet sent (end transmission)
		Data Length	1 byte
		Data D0-Dn	1-8 bytes
		Checksum	1 byte (XOR) Chksum = D0 xor D1 xor ... Dn

	UPS Commands
		GET REPORT
			0xA1	Request Type 1 (fixed)
			0x01	Request Type 2 (fixed)
			0xnn	Report ID -- Identifies the data being returned
			0x03	Report Type
			0x00	Interface LSB (fixed)
			0x00	Interface MSB (fixed)
			0xnn	Length LSB -- Number of data bytes being returned (can be set to 8 -- the msx)
			0x00	Length MSB (fixed)

		SET REPORT
			0x21	Request Type 1 (fixed)
			0x09	Request Type 2 (fixed)
			0xnn	Report ID -- Identifies the data being sent
			0x03	Report Type (fixed)
			0x00	Interface LSB (fixed)
			0x00	Interface MSB (fixed)
			0xnn	Length LSB -- Number of data bytes send in next packet
			0x00	Length MSB (fixed)

		SET REPORT Second packet with data
			0xnn	Report ID
			0xnn	Data Byte LSB
			0xnn	Data Byte Mid Byte
			0xnn	Data Byte MSB

	
	UPS Response
		GET REPORT RESPONSE	or NOTIFY PACKET
			0xnn	Report ID
			0xnn	Data 1
			0xnn	Data 2
			0xnn	Data n

		SET REPORT RESPONSE
			0x06	Ack

	CONTROLLER RESPONSE TO NOTITY
		0x06	Ack
			
			

	Report ID (Data being sent)
		Hex (Decimal)
		0x02 (02)	Present Status
				Byte 0:
					bit 0:  Main AC Status
					bit 1:  Charging Battery
					bit 2:  Discharging Battery
					bit 3:  Below remaining capacity limit
					bit 4:  Battery needs to be replaced
					bit 5:  Output Status 1 = good
					bit 6:  UPS shutdown in progress
					bit 7:  Power overload
				Byte 1:
					bit 0:	Hardware fault (bit 8 in documentation)
					bit 1-7 unknown at this time

		0x16 (22)	Battery Status
				Byte 0:	Remaining Battery Capacity = 0-100 percent
				Byte 1: Remaining Time Low byte
				Byte 2: Remaining Time High byte

		0x0F (15)	Delay Before Shutdown
				Byte 0:	LSB
				Byte 1: Mid Byte
				Byte 2: MSB

		0x0C (12)	Remaining Capacity Limit
				Byte 0:	Percent

		0x0E (14)	Percent Load
				Byte 0: Percent

		0x11 (17)	Delay Before Shutdown
				Byte 0:	LSB
				Byte 1: Mid Byte
				Byte 2: MSB

		0x10 (16)	Product Name index in String Descriptor
				Byte 0: Index on string
		
*/


void trm_eaton_ups_shut_com (void)
{
	unsigned char pkt_req = 0;

	if (tx_ack == 1)
	{
		sx_enable_tx(SE1COM);
		sx_sendc(SE1COM,0x06);	// send a sync character
		tx_ack = 0;
	}
	else if ((timers[tsx_txA] > 6) && (rx_sync == 0))
	{
		timers[tsx_txA] = 0;
		sx_enable_tx(SE1COM);
		sx_sendc(SE1COM,0x16);	// send a sync character
		tx_sync = 1;
	}
	else if ((timers[tsx_txA] > 20) && (rx_sync == 1) && (ups_packet_req == 0))
	{
		if (sx_Tranbuf_Empty[SE1COM] == 1)
		{
			if (pkt_flag == 0)
			{			// Request ups status
				pkt_flag = 1;
				pkt_req = 0x01;
			}
			else
			{			// Request battery capacity
				pkt_flag = 0;
				pkt_req = 0x06;
			}
			pack_eaton_ups_shut_com(pkt_req);

			sx_enable_tx(SE1COM);
			while (ups_trm_ix <= ups_pack_len)
			{
				sx_sendc(SE1COM,ups_trmbuf[ups_trm_ix]);
				ups_trm_ix++;
			}
			ups_packet_req = 1;
			ups_com_tx_cnt++;
		}
		timers[tsx_txA] = 0;
	}
}
		



void pack_eaton_ups_shut_com (int16 report_id)
{
	int16 i;
	unsigned char len = 0;
	unsigned char chksum = 0;

	switch (report_id)
	{
		case 0x01:		// Present Status
			ups_trmbuf[2] = 0xA1;
			ups_trmbuf[3] = 0x01;
			ups_trmbuf[4] = 0x01;
			ups_trmbuf[5] = 0x03;
			ups_trmbuf[6] = 0x00;
			ups_trmbuf[7] = 0x00;
			ups_trmbuf[8] = 0x08;	// Max return length
			ups_trmbuf[9] = 0x00;
			len = 0x88;
			ups_pack_len = 10;			
			ups_packet_req = 1;
			break;

		case 0x06:		// Battert Status
			ups_trmbuf[2] = 0xA1;
			ups_trmbuf[3] = 0x01;
			ups_trmbuf[4] = 0x06;
			ups_trmbuf[5] = 0x03;
			ups_trmbuf[6] = 0x00;
			ups_trmbuf[7] = 0x00;
			ups_trmbuf[8] = 0x08;	// Max return length
			ups_trmbuf[9] = 0x00;
			len = 0x88;
			ups_pack_len = 10;			
			ups_packet_req = 1;
			break;

		default:
			break;				
	}

	if (ups_packet_req == 1)
	{
		ups_trmbuf[0] = 0x81;	 // not setup to do multiple packet transmits
		ups_trmbuf[1] = len;

		chksum = 0;
		for (i=2;i<ups_pack_len;i++)
		{
			chksum ^= ups_trmbuf[i];
		}
		ups_trmbuf[i] = chksum;
	}
	ups_trm_ix = 0;
	ups_rcv_ix = 0;
	pkt_seq_cnt = 0;
}
		
		
void unpack_eaton_ups_shut_com (void)
{
  	int16 rbempt=0,endmess=0;
	unsigned char rcv_char = 0;

	if (sx_Tranbuf_Empty[SE1COM] == 1)
	{
	  	rbempt = sx_isrxempty(SE1COM);
	  	while((rbempt == 0) && (endmess == 0))
	  	{
		  	rcv_char = (uint8)sx_asigetc(SE1COM);
		  	rbempt = sx_isrxempty(SE1COM);
			switch (ups_rcv_seq)
			{
				case 0:		// Receive Response or Notify Packet
					if (rcv_char == 0x15)		// negative acknowledge
						rx_sync = 0;			// re-sync with ups
					else if ((((rcv_char & 0x7F) == 0x04) && (ups_packet_req == 1)) || ((rcv_char & 0x7F) == 0x05))
					{
						if ((rcv_char & 0x7F) == 0x05)
							notify = 1;
						else
							notify = 0;
					    rcv_chksum = 0;	
						ups_rcv_seq = 1;
						if ((rcv_char & 0x80) != 0)
							last_pkt = 1;
						else
							last_pkt = 0;
					}
					else if ((rcv_char == 0x16) && (tx_sync == 1))
					{
						tx_sync = 0;
						rx_sync = 1;
						timers[tsx_rxA] = 0;
						ups_com_rx_cnt++;
					}
					else
						ups_rcv_ix = 0;
					rx_ix = 0;
					break;
				case 1:		// get packet length
					if ((rcv_char & 0x0f) == ((rcv_char & 0xf0) >> 4))
					{ 
						pkt_len = rcv_char & 0x0f;
						ups_rcv_seq = 2;
					}
					else
						ups_rcv_seq = 0;		// invalid block id
					rx_ix = 0;
					break;
				case 2:
					ups_rcvbuf[ups_rcv_ix] = rcv_char;
					ups_rcv_ix++;
					rx_ix++;
					if (rx_ix < pkt_len)
					{
						rcv_chksum ^= rcv_char;
					}
					else
					{
						rcv_chksum ^= rcv_char;
						ups_rcv_seq = 3;
					}
					break;
				case 3:
					if (rcv_char == rcv_chksum)
					{		// valid packet
						if (ups_rcvbuf[0] == 0x01)
						{
							ups_bat_status = 0;

							if ((ups_rcvbuf[1] & 0x01) == 0)
								ups_bat_status |= c_utility_lost;

							if ((ups_rcvbuf[1] & 0x02) != 0)
								ups_bat_status |= c_battery_charging;

							if ((ups_rcvbuf[1] & 0x04) != 0)
							{
								ups_bat_status |= c_on_battery;
								ups_on_bat_pwr = 1;
							}
							else
								ups_on_bat_pwr = 0;

							if ((ups_rcvbuf[1] & 0x18) != 0)
							{
								ups_bat_status |= c_low_battery;
								ups_low_bat = 1;
							}
							else
								ups_low_bat = 0;
							
							if ((ups_rcvbuf[1] & 0x20) == 0)
							{
								ups_bat_status |= c_ups_on;
								ups_turned_off = 0;
							}
							else
								ups_turned_off = 1;

						}
						else if (ups_rcvbuf[0] == 0x06)
						{
							batcap = (float)ups_rcvbuf[1];
							sprintf(bat_cap,"%4.2f",batcap);
						}
						ups_com_online = 1;
						ups_online_count = 0;
						ups_packet_req = 0;		// clear ups packet request
						timers[tsx_rxA] = 0;
						ups_com_rx_cnt++;
//						if (notify == 1)
						{
							notify = 0;
							tx_ack = 1;
						}
					}
					ups_rcv_seq = 0;		// finished
					break;
				default:
					ups_rcv_seq = 0;
					break;
			}
			if (ups_rcv_ix > 300)
			{
				rbempt = 1;
				ups_rcv_ix = 0;
			}
		}
	}
	if ((ups_packet_req == 1) || (tx_sync == 1))
	{			 // Data has been transmitted to device
		if (timers[tsx_rxA] > 60)
		{ 
			ups_packet_req = 0;
			ups_online_count++;
			rx_sync = 0;
			tx_sync = 0;
			if (ups_online_count >= 10)
			{
				ups_com_online = 0;
				ups_bat_status = 0;
				ups_online_count = 0;
				rx_sync = 0;
				tx_sync = 0;
				batcap = 0;
				sprintf(bat_cap,"%4.2f",batcap);
			}
			timers[tsx_rxA] = 0;
			init_sx_com_port_1();
		}
	}
	else
		timers[tsx_rxA] = 0;
}


/*
Revision History

  5/20/08 v5.48.7 mhd		1. Receive battery voltage and set individual fault flags.
  6/3/09 v5.48.9 mhd		1. Corrected data for battery voltage.  Also corrected error setting for fault condition in
							   pwl_comm() routine.
  9/3/09 v5.49.11 mhd		1. Modified check for UPS status and battery voltage.
  10/12/09 v5.50.4 mhd		1. Split up sx_initcom for each port using init_sx_com_port() subroutine.
							2. Reset com port if comm time-out.
  1/18/10 v5.51.6 mhd		1. Added communications for EATON UPS devices.
							2. Change pwl_com() to UPS_com().
  2/2/10 v5.51.10 mhd		1. Select com baud rate as 9600 if eaton UPS.
  8/5/10 v5.52.19 mhd		1. Corrected error setting for ups_low_bat.  Set error when ups_low_bat = 1 instead of 0.
  8/5/10 v5.52.20 mhd		1. Set battery capacity to 100.0 percent on power up so that you don't get a ups fault on powerup.
							2. Change time-out for 6 seconds and declare off line after 10 failures.
							3. Change the debug screen to see more of the receive data.
							4. Don't transmit again until data received or receiver timed out.
  10/8/10 v5.52.36 mhd		1. Added service ups parameter to allow the ups to be disconnectd from the comm while it is being serviced.
  11/19/10 V5.52.47 mhd		1. Added SHUT protocol for Eaton UPS.
  11/24/10 v5.52.49 mhd		1. Changed report id number in SHUT protocol from 0x02 to 0x01 and from 0x16 to 0x06.
							2. Clear ups_bat_status when unpacking report id 0x01 and zero battery capacity on time-out in both EATON protocols.
							3. Check tx_sync == 1, for packet time-out.
  8/14/17 v8.0.7 mhd		1. Changed comm to interrupt based on transmit.

*/
