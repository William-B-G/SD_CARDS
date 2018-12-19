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


void rdport(uint16 addr, uint16 array);
void read_ports (void);
void clr_both_outps(void *ptr1, void *ptr2);
void set_both_outps(void *ptr1, void *ptr2);
uint8 rd_both_inps(void * ptr1, void *ptr2);
void clr_both_inps(void *ptr1, void *ptr2);
void set_both_inps(void *ptr1, void *ptr2);
uint8 rdinp(void *ptr);
uint8 rdoutp(void *ptr);
void setoutp(void *ptr);
void clroutp(void *ptr);
void setinp(void *ptr);
void clrinp(void *ptr);
uint8 rdinp_fm(void *ptr,void *ptrf);
uint8 rdoutp_fm(void *ptr, void *ptrf);
uint8 g_rdcoutp(uint16 car, void *ptr);
uint8 g_rdcinp(uint16 car, void *ptr);
uint8 rdginp(void *ptr);
void setgoutp(void *ptr);
void clrgoutp(void *ptr);
void setoutp(void *ptr);
void setginp(void *ptr);
void clrginp(void *ptr);


//*****************************************
// Read input port
//*****************************************

void read_ports (void)
{

#if (Simulator == 0)
	uint16 i;
	uint16 j;
	uint16 k;
/*
	rdport(BRD1A,0);
	rdport(BRD1B,1);
	rdport(BRD1C,2);
	rdport(BRD2A,3);
	rdport(BRD2B,4);
	rdport(BRD2C,5);
	rdport(BRD3A,6);
	rdport(BRD3B,7);
	rdport(BRD3C,8);
	rdport(BRD4A,9);
*/
	k = 0;
	for(i=6;i<46;i++)
	{
		for(j=0;j<=2;j++)
		{
			if (IO_board[cons[carnmb]][i] != 0)
				rdport(k,(uint16)(18+k));
			k++;
		}
	}
	
	k = 0;	
	for(i=46;i<86;i++)
	{
		for(j=0;j<=2;j++)
		{
			if (IO_board[cons[carnmb]][i] != 0)
				rdport(k,(uint16)(138+k));
			k++;
		}
	}
	

#endif

}

//*****************************************
// Read input port
//*****************************************

void rdport(uint16 addr, uint16 array)
{
	if (IO_In_Out[array] != 0xFF)		// Have inputs on the port
	{
		if (array < 18)
;
//			iodata[cons[carnmb]][array] = (uint8)((iodata[cons[carnmb]][array] & IO_In_Out[array]) | (inp(addr) & ~IO_In_Out[array]));
		else if (array < 138)
			iodata[cons[carnmb]][array] = (uint8)((iodata[cons[carnmb]][array] & IO_In_Out[array]) | (serial_mrc.brd_io[addr] & ~IO_In_Out[array]));
		else 
			iodata[cons[carnmb]][array] = (uint8)((iodata[cons[carnmb]][array] & IO_In_Out[array]) | (serial_ctc.brd_io[addr] & ~IO_In_Out[array]));
	}
}
	
//*****************************
// Read the Or of two inputs
//*****************************

uint8 rd_both_inps(void * ptr1, void *ptr2)
{
	if (rdinp(ptr1) == 1)
		return 1;
	else if ((cons[aux_COP] != 0) && (rdinp(ptr2) == 1))
		return 1;
	else
		return 0;
}

//*****************************
// Set both outputs
//*****************************


void set_both_outps(void *ptr1, void *ptr2)
{
	setoutp(ptr1);
	if (cons[aux_COP] != 0) 
		setoutp(ptr2);
}

//*****************************
// Clear both outputs
//*****************************


void clr_both_outps(void *ptr1, void *ptr2)
{
	clroutp(ptr1);
	if (cons[aux_COP] != 0) 
		clroutp(ptr2);
}

//*****************************
// Set both inputs
//*****************************


void set_both_inps(void *ptr1, void *ptr2)
{
	setinp(ptr1);
	if (cons[aux_COP] != 0) 
		setinp(ptr2);
}

//*****************************
// Clear both inputs
//*****************************


void clr_both_inps(void *ptr1, void *ptr2)
{
	clrinp(ptr1);
	if (cons[aux_COP] != 0) 
		clrinp(ptr2);
}


   
   
// *******************************************
// This is the read an input procedure
// If voltage return 1 if no voltage return 0
// *******************************************
uint8 rdinp(void *ptr)
{
	struct io *iop = (struct io *)ptr;

	if (iop->bus != INV_IO)
	{

#if (Simulator == 0)
		if (iop->bus == CTCAN)
			iodata[cons[carnmb]][iop->array] = (uint8)((iodata[cons[carnmb]][iop->array] & IO_In_Out[iop->array]) | (serial_ctc.brd_io[iop->addr] & ~IO_In_Out[iop->array]));
		else if (iop->bus == MRCAN)
			iodata[cons[carnmb]][iop->array] = (uint8)((iodata[cons[carnmb]][iop->array] & IO_In_Out[iop->array]) | (serial_mrc.brd_io[iop->addr] & ~IO_In_Out[iop->array]));
		else if (iop->bus == MAINIO)
		{
			if (iop->addr == c_misc_io_addr)
				iodata[cons[carnmb]][iop->array] = misc_io_data;
//			else
//				iodata[cons[carnmb]][iop->array] = (uint8)((iodata[cons[carnmb]][iop->array] & IO_In_Out[iop->array]) | (inp(iop->addr) & ~IO_In_Out[iop->array]));
		}
#else
/*
#if (Sim_Door == 0)
		if (iop->array == 2)
		{
	      	iodata[cons[carnmb]][iop->array] &= (BIT1 | BIT8);
	      	iodata[cons[carnmb]][iop->array] |= (inp(iop->addr) & ~(BIT1 | BIT8));
		}
		if (iop->array == 5)
		{
	      	iodata[cons[carnmb]][iop->array] &= ~(BIT6 | BIT7);
	      	iodata[cons[carnmb]][iop->array] |= (inp(iop->addr) & (BIT6 | BIT7));
		}
#endif

  #if (Sim_Control == 0)		

		if (iop->array == 0)
	      	iodata[cons[carnmb]][iop->array] = inp(iop->addr);
		
		if (iop->array == 1)
	      	iodata[cons[carnmb]][iop->array] = inp(iop->addr);
		
		if (iop->array == 2)
		{
	      	iodata[cons[carnmb]][iop->array] &= ~(BIT1);
	      	iodata[cons[carnmb]][iop->array] |= (inp(iop->addr) & BIT1);
		}
		
		if (iop->array == 3)
		{
	      	iodata[cons[carnmb]][iop->array] &= ~(BIT6 | BIT7 | BIT8);
	      	iodata[cons[carnmb]][iop->array] |= (inp(iop->addr) & (BIT6 | BIT7 | BIT8));
		}

		if (iop->array == 4)
	      	iodata[cons[carnmb]][iop->array] = inp(iop->addr);
		
		if (iop->array == 5)
		{
	      	iodata[cons[carnmb]][iop->array] &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT8);
	      	iodata[cons[carnmb]][iop->array] |= (inp(iop->addr) & (BIT1 | BIT2 | BIT3 | BIT4 | BIT8));
		}

		if (iop->array == 6)
		{
	      	iodata[cons[carnmb]][iop->array] &= ~(BIT1);
	      	iodata[cons[carnmb]][iop->array] |= (inp(iop->addr) & BIT1);
		}

		if (iop->array == 7)
	      	iodata[cons[carnmb]][iop->array] = inp(iop->addr);
		
		if (iop->array == 8)
	      	iodata[cons[carnmb]][iop->array] = inp(iop->addr);
		
		if (iop->array == 9)
	      	iodata[cons[carnmb]][iop->array] = (inp(iop->addr) & (BIT1 | BIT2 | BIT3 | BIT4));



  #endif
*/
		if (iop->bus == MAINIO)
		{
			if (iop->addr == c_misc_io_addr)
				iodata[cons[carnmb]][iop->array] = misc_io_data;
		}
		else if (iop->bus == CTCAN)
			iodata[cons[carnmb]][iop->array] = (uint8)((iodata[cons[carnmb]][iop->array] & IO_In_Out[iop->array]) | (serial_ctc.brd_io[iop->addr] & ~IO_In_Out[iop->array]));
		else if (iop->bus == MRCAN)
			iodata[cons[carnmb]][iop->array] = (uint8)((iodata[cons[carnmb]][iop->array] & IO_In_Out[iop->array]) | (serial_mrc.brd_io[iop->addr] & ~IO_In_Out[iop->array]));
#endif

	   	if ((iodata[cons[carnmb]][iop->array] & iop->bit) != 0)
	  		return 1;
	   	else
	  		return 0;
	}
	else 
		return 0;
}


//*********************************************
// Read Output from the iodata port
//*********************************************

uint8 rdoutp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	if (iop->bus != INV_IO)
	{
	   	if ((iodata[cons[carnmb]][iop->array] & iop->bit) == 0)
	  		return 1;
	   	else
	  		return 0;
	}

	return 0;
}


// ****************************************
// This is the turn on an output procedure
// ****************************************

void setoutp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	
	if (iop->bus != INV_IO)
	{
		iodata[cons[carnmb]][iop->array] &= ~iop->bit;
//	  	if (iop->bus == MAINIO)
//	  		outp(iop->addr,iodata[cons[carnmb]][iop->array]);
	}
}

// *****************************************
// This is the turn off an output procedure
// *****************************************
void clroutp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	
	if (iop->bus != INV_IO)
	{
		
		iodata[cons[carnmb]][iop->array] |= iop->bit;
	  	
//	  	if (iop->bus == MAINIO)
//	  	{
//	  		if (iop->addr != c_misc_io_addr)
//		  		outp(iop->addr,iodata[cons[carnmb]][iop->array]);
//	  	}
	}
  	return;
}


//***************************************
// Set car input procedure
//***************************************

void setinp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	uint16 devnmb;

	if (iop->bus != INV_IO)
	{
		if (iop->bus == CTCAN)
		{
			devnmb = (iop->addr/3) + c_ctcan_SEB_1;
			if (ctcan_dev[devnmb].online == 0)
				serial_ctc.brd_io[iop->addr] |= iop->bit;
		}
		else if (iop->bus == MRCAN)
		{
			devnmb = (iop->addr/3) + c_mrcan_SEB_1;
			if (mrcan_dev[devnmb].online == 0)
				serial_mrc.brd_io[iop->addr] |= iop->bit;
		}
		else if (iop->bus == MAINIO)
		{
			if (iop->addr == c_misc_io_addr)
				misc_io_data |= iop->bit;
			else
				iodata[cons[carnmb]][iop->array] |= iop->bit;
		}
	}
}

// *****************************************
// This is the turn off an input procedure
// *****************************************

void clrinp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	uint16 devnmb;
	
	if (iop->bus != INV_IO)
	{
		if (iop->bus == CTCAN)
		{
			devnmb = (iop->addr/3) + c_ctcan_SEB_1;
			if (ctcan_dev[devnmb].online == 0)
				serial_ctc.brd_io[iop->addr] &= ~iop->bit;
		}
		else if (iop->bus == MRCAN)
		{
			devnmb = (iop->addr/3) + c_mrcan_SEB_1;
			if (mrcan_dev[devnmb].online == 0)
				serial_mrc.brd_io[iop->addr] &= ~iop->bit;
		}
		else if (iop->bus == MAINIO)
		{
			if (iop->addr == c_misc_io_addr)
				misc_io_data &= ~iop->bit;
			else
		  		iodata[cons[carnmb]][iop->array] &= ~iop->bit;
		}
	}
  	return;
}

//************************************************
// Read Input from the fault structure iodata port
//************************************************
uint8 rdinp_fm(void *ptr, void *ptrf)
{
	struct io *iop = (struct io *)ptr;
	struct Fault *fltp = (struct Fault *)ptrf;


	if (iop->bus == MAINIO)
	{
	   	if ((fltp->iodata[iop->array] & iop->bit) != 0)
	  		return 1;
	   	else
	  		return 0;
	}
	return 0;
}


//************************************************
// Read Output from the fault structure iodata port
//************************************************

uint8 rdoutp_fm(void *ptr, void *ptrf)
{
	struct io *iop = (struct io *)ptr;
	struct Fault *fltp = (struct Fault *)ptrf;

	if (iop->bus == MAINIO)
	{
	   	if ((fltp->iodata[iop->array] & iop->bit) == 0)
	  		return 1;
	   	else
	  		return 0;
	}

	return 0;
}



//******************************************************
// Read Input from the iodata port for a particular car
//******************************************************

uint8 g_rdcinp(uint16 car, void *ptr)
{
	struct io *iop = (struct io *)ptr;

   	if ((iodata[car][iop->array] & iop->bit) != 0)
  		return 1;
   	else
  		return 0;
}


//******************************************************
// Read Output from the iodata port for a particular car
//******************************************************

uint8 g_rdcoutp(uint16 car, void *ptr)
{
	struct io *iop = (struct io *)ptr;
   	if ((iodata[car][iop->array] & iop->bit) == 0)
  		return 1;
   	else
  		return 0;
}


//**************************************************************************
//************* Group inputs and outputs ***********************************
//**************************************************************************



// *******************************************
// This is the read an input procedure
// If voltage return 1 if no voltage return 0
// *******************************************
uint8 rdginp(void *ptr)
{
	struct io *iop = (struct io *)ptr;


	if (iop->bus != INV_IO)
	{
		if (iop->bus == GRCAN)			// iop->addr is the index when using serial comm
			giodata(iop->array) = (uint8)((giodata(iop->array) & GIO_In_Out[iop->array]) | (serial_grc.brd_io[iop->addr] & ~GIO_In_Out[iop->array]));

	   	if ((giodata(iop->array) & iop->bit) != 0)
	  		return 1;
	   	else
	  		return 0;
	}
	else return 0;
}


// ****************************************
// This is the turn on a GROUP output procedure
// ****************************************
void setgoutp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	
	if (iop->bus != INV_IO)
	{
		if (iop->bus == GRCAN) 
			giodata(iop->array) &= ~iop->bit;
	  	
	}
}

// *****************************************
// This is the turn off an output procedure
// *****************************************
void clrgoutp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	
	if (iop->bus != INV_IO)
	{
		if (iop->bus == GRCAN) 
			giodata(iop->array) |= iop->bit;
	}
  	return;
}

// *******************************************
// This is the turn on a GROUP input procedure
// *******************************************
void setginp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	uint16 devnmb;
	
	if (iop->bus != INV_IO)
	{
		if ((HC_IO_board[(iop->array/3)] & 0x03) == 3)
			devnmb = (iop->addr/12) +  c_grcan_HCDB_1;
		else
			devnmb = (iop->addr/3) + c_grcan_SEB_1;
		if (grcan_dev[devnmb].online == 0)
		{
			if (iop->bus == GRCAN) 
				serial_grc.brd_io[iop->addr] |= iop->bit;
		}
	}
}

// ********************************************
// This is the turn off a GROUP input procedure
// ********************************************
void clrginp(void *ptr)
{
	struct io *iop = (struct io *)ptr;
	uint16 devnmb;
	
	if (iop->bus != INV_IO)
	{
		if ((HC_IO_board[(iop->array/3)] & 0x03) == 3)
			devnmb = (iop->addr/12) +  c_grcan_HCDB_1;
		else
			devnmb = (iop->addr/3) + c_grcan_SEB_1;
		if (grcan_dev[devnmb].online == 0)
		{
			if (iop->bus == GRCAN) 
				serial_grc.brd_io[iop->addr] &= ~iop->bit;
		}
	}
}

/* 
Revision History

11/12/13 v7.0.28 	1. Changed iodata[] index to the car number all the time.  Do not use 1 for the local car.  This is so any car can be the group.
					2. Replaced iodata[1] with iodata[cons[carnmb]] where necessary.
2/26/15 v7.1.26 mhd	1. Added routine to clear destination dispatch car call data since group is not a destination dispacth group.

*/