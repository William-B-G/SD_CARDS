/*******************************************************************************
Function Name : Process I/O
Engineer      : Mark Duckworth
Date          : 5/16/12
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : 
Issues        : NONE
*******************************************************************************/

#define d_PIO 1
#include "global.h"

#define Sim_Control 1		// Set to 0 for special simulator with actual controller
#define Sim_Motion 1
#define Sim_Door 1


void clroutp(int16 address, uint8 bitno, uint8 arrno);
void clrinp(int16 address, uint8 bitno, uint8 arrno);
uint8 rdinp(int16 address, uint8 bitno, uint8 arrno);
uint8 rdoutp(int16 address, uint8 bitno, uint8 arrno);
void setoutp(int16 address, uint8 bitno, uint8 arrno);
void setinp(int16 address, uint8 bitno, uint8 arrno);
uint8 g_rdoutp(uint16 car,int16 address, uint8 bitno, uint8 arrno);
uint8 g_rdinp(uint16 car,int16 address,uint8 bitno,uint8 arrno);
void read_ports(void);


//*****************************************
// Read input ports
//*****************************************

void read_ports(void)
{
	
#if (Simulator == 0)

	iodata[1][0] = inp(BRD1A);
	iodata[1][1] = inp(BRD1B);
	iodata[1][2] = inp(BRD1C);
	iodata[1][3] = inp(BRD2A);
	iodata[1][4] = inp(BRD2B);
	iodata[1][5] = inp(BRD2C);
	iodata[1][6] = inp(BRD3A);
	iodata[1][7] = inp(BRD3B);
	iodata[1][9] = inp(BRD4A);
	iodata[1][10] = inp(BRD4B);
	iodata[1][12] = inp(BRD5A);
	iodata[1][13] = inp(BRD5B);
	
	// 1st serial expansion board
	iodata[1][18] = serial.brd_io[BRD7A];
	iodata[1][19] = serial.brd_io[BRD7B];
	iodata[1][20] = serial.brd_io[BRD7C];

	// 2nd serial expansion board
	iodata[1][24] = serial.brd_io[BRD9A];
	iodata[1][25] = serial.brd_io[BRD9B];
	iodata[1][26] = serial.brd_io[BRD9C];

	// 3rd serial expansion Board
	iodata[1][30] = serial.brd_io[BRD11A];
	iodata[1][31] = serial.brd_io[BRD11B];
	iodata[1][32] = serial.brd_io[BRD11C];

	// 4th serial expansion board
	iodata[1][36] = serial.brd_io[BRD13A];					// updating brd14a
	iodata[1][37] = serial.brd_io[BRD13B];					// updating brd14b
	iodata[1][38] = serial.brd_io[BRD13C];					// updating brd14c
	

	// 5th serial expansion board
	iodata[1][42] = serial.brd_io[BRD15A];					// updating brd16a
	iodata[1][43] = serial.brd_io[BRD15B];					// updating brd16b
	iodata[1][44] = serial.brd_io[BRD15C];					// updating brd16c
	

	// 6th serial expansion board
	iodata[1][48] = serial.brd_io[BRD17A];					// updating brd18a
	iodata[1][49] = serial.brd_io[BRD17B];					// updating brd18b
	iodata[1][50] = serial.brd_io[BRD17C];					// updating brd18c
	

	// 7th serial expansion board
	iodata[1][54] = serial.brd_io[BRD19A];				  	// updating brd20a
	iodata[1][55] = serial.brd_io[BRD19A];  				// updating brd20b
	iodata[1][56] = serial.brd_io[BRD19A];  				// updating brd20c
	
	// 7th serial expansion board
	iodata[1][60] = serial.brd_io[BRD21A];				  	// updating brd20a
	iodata[1][61] = serial.brd_io[BRD21A];  				// updating brd20b
	iodata[1][62] = serial.brd_io[BRD21A];  				// updating brd20c

	// TOC board
	iodata[1][66] = serial.brd_io[BRD23A];					// updating brd22a
	iodata[1][67] = serial.brd_io[BRD23B];					// updating brd22b
	iodata[1][68] = serial.brd_io[BRD23C];					// updating brd22c
	iodata[1][72] = serial.brd_io[BRD25A];					// updating brd24a
	iodata[1][73] = serial.brd_io[BRD25B];					// updating brd24b
		
#else
	#if (debug_io_enable == 1)

		iodata[1][18] = serial.brd_io[BRD7A];
		iodata[1][19] = serial.brd_io[BRD7B];
		iodata[1][20] = serial.brd_io[BRD7C];

	#endif
#endif


}



// *****************************************
// This is the turn off an output procedure
// *****************************************
void clroutp(int16 address,uint8 bitno,uint8 arrno)
{
//	if (address < c_max_ser_addr)
//		serial.brd_io[address] |= bitno;
//	else if (address == c_misc_io_addr)
//		misc_io_data |= bitno;
//	else
  		iodata[1][arrno] = (iodata[1][arrno] | bitno);
  	
  	if (address > c_max_ser_addr)
	  	outp(address,iodata[1][arrno]);
  	return;
}

void clrinp(int16 address,uint8 bitno,uint8 arrno)
{
	if (address < c_max_ser_addr)
		serial.brd_io[address] &= ~bitno;
	else if (address == c_misc_io_addr)
		misc_io_data &= ~bitno;
	else
  		iodata[1][arrno] &= ~bitno;
  	return;
}


//*********************************************
// Read Output from the iodata port
//*********************************************

uint8 rdoutp(int16 address, uint8 bitno, uint8 arrno)
{
//	if (address < c_max_ser_addr)
//	{
//		if ((serial.brd_io[address] & bitno) == 0)
//			return 1;
//		else
//			return 0;
//	}
//	else if (address == c_misc_io_addr)
//	{
//		if ((misc_io_data & bitno) == 0)
//			return 1;
//		else
//			return 0;
//	}
// 	else
   	{
	   	if ((iodata[1][arrno] & bitno) == 0)
	 		return(1);		// Return a 1 if output is on
	   	else
	  		return(0);
   	}
}

//******************************************************
// Read Output from the iodata port for a particular car
//******************************************************

uint8 g_rdoutp(uint16 car,int16 address, uint8 bitno, uint8 arrno)
{
	if (car == cons[carnmb])
		car = 1;
   	if ((iodata[car][arrno] & bitno) == 0)
 		return(1);		// Return a 1 if output is on
   	else
		return(0);
}

uint8 g_rdinp(uint16 car,int16 address,uint8 bitno,uint8 arrno)
{
	if (car == cons[carnmb])
		car = 1;

   	if ((iodata[car][arrno] & bitno) != 0)
 		return(1);
   	else
  		return(0);
}
// *******************************************
// This is the read an input procedure
// If voltage return 1 if no voltage return 0
// *******************************************
uint8 rdinp(int16 address,uint8 bitno,uint8 arrno)
{
#if (Simulator == 0)
	if (address < c_max_ser_addr)
		iodata[1][arrno] = serial.brd_io[address];	  // address is the index when using serial comm
	else if (address == c_misc_io_addr)
		iodata[1][arrno] = misc_io_data;
	else
		iodata[1][arrno] = inp(address);
#else

#if (Sim_Door == 0)
	if (arrno == 1)
	{
      	iodata[1][arrno] &= (BIT1 | BIT2 | BIT3 | BIT4 | BIT8);
      	iodata[1][arrno] |= (inp(address) & ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT8));
	}
	if (arrno == 2)
	{
      	iodata[1][arrno] &= (BIT4 | BIT5 | BIT6 | BIT7 | BIT8);
      	iodata[1][arrno] |= (inp(address) & ~(BIT4 | BIT5 | BIT6 | BIT7 | BIT8));
	}
	if (arrno == 7)
	{
      	iodata[1][arrno] &= (BIT1 | BIT2 | BIT3 | BIT4 | BIT7 | BIT8);
      	iodata[1][arrno] |= (inp(address) & ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT7 | BIT8));
	}
#endif

  #if (Sim_Control == 0)		
	// Special simulator running in actual controller
//	if ((arrno == 4) && (bitno >= BIT7) && (bitno <= BIT8))
//      	iodata[1][arrno] = inp(address);


	if (arrno == 4)
      	iodata[1][arrno] = inp(address);
	if ((arrno == 6) && (bitno >= BIT1) && (bitno <= BIT6))
	{
      	iodata[1][arrno] &= 0xc0;
      	iodata[1][arrno] |= (inp(address) & 0x3f);
	}
	if ((arrno == 7) && (bitno >= BIT7) && (bitno <= BIT8))
    {
      	iodata[1][arrno] &= 0x3f;
      	iodata[1][arrno] |= (inp(address) & 0xc0);
	}
	if (arrno == 9)
      	iodata[1][arrno] = inp(address);
	
	if ((arrno == 10) && (bitno != BIT6))
	{

   		iodata[1][arrno] &= (BIT6);
      	iodata[1][arrno] |= (inp(address) & ~(BIT6));
		
	}
	if (arrno == 12) 
	{	
		if (((bitno >= BIT1) && (bitno <= BIT4)) || ((bitno >= BIT7) && (bitno <= BIT8)))
		{
      		iodata[1][arrno] &= (BIT5 | BIT6);
      		iodata[1][arrno] |= (inp(address) & ~(BIT5 | BIT6));
		}
	}
	if (arrno == 13) 
	{	
		if (((bitno >= BIT1) && (bitno <= BIT5)) || ((bitno >= BIT7) && (bitno <= BIT8)))
		{
      		iodata[1][arrno] &= BIT6;
      		iodata[1][arrno] |= (inp(address) & ~BIT6);
		}
	}
  #endif

	if (address < c_max_ser_addr)
		iodata[1][arrno] = serial.brd_io[address];	  // address is the index when using serial comm
	else if (address == c_misc_io_addr)
		iodata[1][arrno] = misc_io_data;
#endif

   	if ((iodata[1][arrno] & bitno) != 0)
 		return(1);
   	else
  		return(0);
}

// ****************************************
// This is the turn on an output procedure
// ****************************************
void setoutp(int16 address,uint8 bitno,uint8 arrno)
{
//	if (address < c_max_ser_addr)
//		serial.brd_io[address] &= ~bitno;
//	else if (address == c_misc_io_addr)
//		misc_io_data &= ~bitno;
//	else
  		iodata[1][arrno] &= ~bitno;
//	bitno = (bitno ^ 0xff);
//	iodata[1][arrno] = (iodata[1][arrno] & bitno);
  	if (address > c_max_ser_addr)
  		outp(address,iodata[1][arrno]);
  	return;
}

void setinp(int16 address,uint8 bitno,uint8 arrno)
{
	if (address < c_max_ser_addr)
		serial.brd_io[address] |= bitno;
	else if (address == c_misc_io_addr)
		misc_io_data |= bitno;
	else
  		iodata[1][arrno] |= bitno;
  	return;
}
