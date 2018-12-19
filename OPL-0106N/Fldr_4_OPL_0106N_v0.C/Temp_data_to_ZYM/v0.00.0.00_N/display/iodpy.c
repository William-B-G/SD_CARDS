// Controller I/O Display and Relocation
#define d_IODPY 1
#include "global.h"

uint16 get_io_status(int16 brdnmb, int16 ix);
uint16 get_hcio_status(int16 brdnmb, int16 ix);
void find_io_start(int16 inc);				 
int16 Display_Car_IO (int16 lcd_mode);
int16 Display_Group_IO(int16 lcd_mode);
int16 Display_FPGA1_IO (int16 lcd_menu);
int16 Relocate_IO(int16 lcd_mode);
void get_init_io_ix (int8 search_dir);
void get_next_io_ix (int8 search_dir);
void display_reloc_status(int16 mode);
int16 Not_Valid_IO_Name(int16 port);

int16 reloc_menu_lev;
int16 reloc_sub_menu_lev;
int16 spare_io_ix;
int16 io_ix;
int16 io_ix_start;
int16 io_ix_max;
int16 iotype;		// 0 = input, 1 = outpu
int16 ioport;		// 0 = MRCAN, 1 = CTCAN, 2= GRCAN
int16 prev_Flash = 0;

const char LCD_Relocate_IO_Menu[3][21] = {
  " Add IO Relocation  ",
  " Remove Relocated IO",
  " Clr Relocation Tbl ",
};

const char LCD_Add_Remove_IO_Menu[2][21] = {
  " Sel Location: MSPI1",
  " Select IO: None    ",
};

const char spare_io_none[5] = {"None "};

const char LCD_Clear_IO_Reloc_TBL[3][21]={
  "Hit Ent to Clear the",
  "IO Relocation Table ",
  "      Cleared       ",
};

const char hc_brd_comm[5][21] = {
  "   Group I/O SEB #1 ",	// 0
  " Hall Call Board #1 ",	// 1
  " Comm Status = 0    ",	// 2
  " HC Driver Board #1 ",	// 3
  "   continued ...    ",	// 4
};


const char LCD_Board[86][21] = {
  "   Main I/O Board   ",	// 1
  "   Main I/O Board   ",	// 2
  "   Main I/O Board   ",	// 3 
  "   Main I/O Board   ",	// 4
  "   Main I/O Board   ",	// 5
  "   Main I/O Board   ",	// 6
  "  MRCAN I/O Board   ",	// 7
  "  MRCAN I/O Board   ",	// 8
  "  MRCAN I/O Board   ",	// 9
  "  MRCAN I/O Board   ",	// 10
  "  MRCAN I/O Board   ",	// 11
  "  MRCAN I/O Board   ",	// 12
  "  MRCAN I/O Board   ",	// 13
  "  MRCAN I/O Board   ",	// 14
  "  MRCAN I/O Board   ",	// 15
  "  MRCAN I/O Board   ",	// 16
  "  MRCAN I/O Board   ",	// 17
  "  MRCAN I/O Board   ",	// 18
  "  MRCAN I/O Board   ",	// 19
  "  MRCAN I/O Board   ",	// 20
  "  MRCAN I/O Board   ",	// 21
  "  MRCAN I/O Board   ",	// 22
  "  MRCAN I/O Board   ",	// 23
  "  MRCAN I/O Board   ",	// 24
  "  MRCAN I/O Board   ",	// 25
  "  MRCAN I/O Board   ",	// 26
  "  MRCAN I/O Board   ",	// 27
  "  MRCAN I/O Board   ",	// 28
  "  MRCAN I/O Board   ",	// 29
  "  MRCAN I/O Board   ",	// 30
  "  MRCAN I/O Board   ",	// 31
  "  MRCAN I/O Board   ",	// 32
  "  MRCAN I/O Board   ",	// 33
  "  MRCAN I/O Board   ",	// 34
  "  MRCAN I/O Board   ",	// 35
  "  MRCAN I/O Board   ",	// 36
  "  MRCAN I/O Board   ",	// 37
  "  MRCAN I/O Board   ",	// 38
  "  MRCAN I/O Board   ",	// 39
  "  MRCAN I/O Board   ",	// 40
  "  MRCAN I/O Board   ",	// 41
  "  MRCAN I/O Board   ",	// 42
  "  MRCAN I/O Board   ",	// 43
  "  MRCAN I/O Board   ",	// 44
  "  MRCAN I/O Board   ",	// 45
  "  MRCAN I/O Board   ",	// 46
  "     COP Board      ",	// 47
  "   SELECTOR Board   ",	// 48
  "   SELECTOR Board   ",	// 49
  "    DOOR Board      ",	// 50
  "  REAR DOOR Board   ",	// 51
  "  CTCAN I/O Board   ",	// 52
  "  CTCAN I/O Board   ",	// 53
  "  CTCAN I/O Board   ",	// 54
  "  CTCAN I/O Board   ",	// 55
  "  CTCAN I/O Board   ",	// 56
  "  CTCAN I/O Board   ",	// 57
  "  CTCAN I/O Board   ",	// 58
  "  CTCAN I/O Board   ",	// 59
  "  CTCAN I/O Board   ",	// 60
  "  CTCAN I/O Board   ",	// 61
  "  CTCAN I/O Board   ",	// 62
  "  CTCAN I/O Board   ",	// 63
  "  CTCAN I/O Board   ",	// 64
  "  CTCAN I/O Board   ",	// 65
  "  CTCAN I/O Board   ",	// 66
  "  CTCAN I/O Board   ",	// 67
  "  CTCAN I/O Board   ",	// 68
  "  CTCAN I/O Board   ",	// 69
  "  CTCAN I/O Board   ",	// 70
  "  CTCAN I/O Board   ",	// 71
  "  CTCAN I/O Board   ",	// 72
  "  CTCAN I/O Board   ",	// 73
  "  CTCAN I/O Board   ",	// 74
  "  CTCAN I/O Board   ",	// 75
  "  CTCAN I/O Board   ",	// 76
  "  CTCAN I/O Board   ",	// 77
  "  CTCAN I/O Board   ",	// 78
  "  CTCAN I/O Board   ",	// 79
  "  CTCAN I/O Board   ",	// 80
  "  CTCAN I/O Board   ",	// 81
  "  CTCAN I/O Board   ",	// 82
  "  CTCAN I/O Board   ",	// 83
  "  CTCAN I/O Board   ",	// 84
  "  CTCAN I/O Board   ",	// 85
  "  CTCAN I/O Board   ",	// 86
};


const char LCD_Board_Group[21] = {
  "     Block # 1      "	// 1
};
const char LCD_Board_Address[21] = {
  "    Address # 1     "	// 1
};

/*
const char LCD_IO_Dpy_Row[12][21] = {
  " 1         5        ",	// 0
  " 2         6        ",	// 1
  " 3         7        ",	// 2
  " 4         8        ",	// 3
  " 9        13        ",	// 4
  "10        14        ",	// 5
  "11        15        ",	// 6
  "12        16        ",	// 7
  "17        21        ",	// 8
  "18        22        ",	// 9
  "19        23        ",	// 10
  "20        24        ",	// 11
};
*/

const char LCD_IO_Dpy_Row[21] = {
  "      |      |      ",	// Seperate I/O
};
const char LCD_HC_IO_Dpy_Row[21] = {
  "   |      |      |  ",	// Individual Hall Call Board I/O
};

int16 io_ix_first,io_ix_last;
int16 io_loc[12], io_bit_mask[12];
static int16 const io_row[12] = {0,1,2,3,0,1,2,3,0,1,2,3};
static int16 const io_col[12] = {5,5,5,5,12,12,12,12,19,19,19,19};
static int16 const hcio_row[4] = {2,3,2,3};
static int16 const hcio_col[4] = {9,9,16,16};


static int16 brdnmb,hc_io_ix,io_status,comm_dpy;
static int16 dpy_header, valid_io;


//*******************************************
// Routine to Display the Car IO
//*******************************************

int16 Display_Car_IO (int16 lcd_menu)
{
	int16 i,j,k,m;
	int16 block_num, board_type, address;
	int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 mode_sel;
	
  	if(LCD_Sub_Init == 1)	// first time through Inputs and Outputs
  	{
  	    upd_dpy = 1;
  	    LCD_Sub_Init = 0;
		io_ix_last = nmb_ios;			// Start at the last two boards
		io_ix_first = 0;
		dpy_header = 1;
		valid_io = 0;
		find_io_start(1);
  	}
  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  	{
  	    ModeButton = 1;  //
		lcd_menu = 0;
		return(lcd_menu);
  	}
  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  	{
  	    UpButton = 1;  // incriment
  	    upd_dpy = 1;
		valid_io = 0;
		if ((io_loc[0]%24) == 0)
		{	// start of page
			if (dpy_header == 1)
				dpy_header = 0;
			else
				find_io_start(1);
		}
		else
		{
			if (dpy_header == 0)
			{
				find_io_start(1);
				if ((io_loc[0]%24) == 0)
					dpy_header = 1;
			}
			else
				dpy_header = 0;
		}
  	}
  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  	{
  	    DownButton = 1;  // decriment
  	    upd_dpy = 1;
		valid_io = 0;
		if ((io_loc[0]%24) == 0) 
		{				// start of page
			if (dpy_header == 1)
			{		 // have already displayed the header for the next page
				find_io_start(0);	// goto the previous block of i/o's		
				dpy_header = 0;
			}
			else
				dpy_header = 1;		// goto the current header
		}
		else 
		{
			if (dpy_header == 0)
				find_io_start(0);		// backup to the previous block of i/o's
  			else
				dpy_header = 0;
		}
  	}
  	if(upd_dpy == 1)
  	{
		upd_dpy = 0;
		block_num = (io_loc[0]/24)+1;
		if (block_num < 7)
			board_type = 0;
		else
			board_type = 1;
		if (dpy_header == 1)
		{
  		    for(i=0; i<=19; i++)
  		    {
  		    	LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Board[io_loc[0]/24][i];
				if (board_type == 0)
			  		LCD_Display[2][i] = LCD_Board_Group[i];
				else
			  		LCD_Display[2][i] = LCD_Board_Address[i];
				
    		    LCD_Display[3][i] = ' ';
		    }
		    if (board_type == 0)
		    {
			    	
				itoa((io_loc[0]/24)+1, LCD_String);
				LCD_Display[2][13] = LCD_String[0];
				if (LCD_String[1] != 0)
					LCD_Display[2][14] = LCD_String[1];
		    }
		    else
		    {
		    	if (block_num < 47)
		    		address = block_num - 6;
		    	else
		    		address = block_num - 46;
		    
				itoa(address, LCD_String);
				LCD_Display[2][14] = LCD_String[0];
				if (LCD_String[1] != 0)
					LCD_Display[2][15] = LCD_String[1];
		    }
		    
		}
		else
		{
		//		Example:
		//	  "     Block # 1      ",	
		//	  "xxxAD.|xL120.|xxxTF.",	// 0
		//	  "xxIND.|xxS10.|xxxBF.",	// 1
		//	  "xxLBP.|     .|xxxPS.",	// 2
		//	  "xxGBP.|xxGOV.|xxHSS.",	// 3
				
		
  		    for(i=0; i<=19; i++)
  		    {
  		    	LCD_Display[0][i] = LCD_IO_Dpy_Row[i];
				LCD_Display[1][i] = LCD_IO_Dpy_Row[i];
			  	LCD_Display[2][i] = LCD_IO_Dpy_Row[i];
    		    LCD_Display[3][i] = LCD_IO_Dpy_Row[i];
		    }

			valid_io = 0;
	  	    for(i=0; i<=2; i++)
	  	    {
			    k = (i * 7);
			    m = i * 4;
				for (j=0;j<=4;j++)
				{
					LCD_Display[0][k + j] = ioname[io_loc[m]][j];
					LCD_Display[1][k + j] = ioname[io_loc[m+1]][j];
					LCD_Display[2][k + j] = ioname[io_loc[m+2]][j];
					LCD_Display[3][k + j] = ioname[io_loc[m+3]][j];
	  	    	}
				if ((io_valid[io_loc[m]/8] & bittbl[io_loc[m] & 0x7]) != 0)
				{
					LCD_Display[0][k + 5] = io_off;
					valid_io = 1;
				}
				if ((io_valid[io_loc[m+1]/8] & bittbl[io_loc[m+1] & 0x7]) != 0)
				{
					LCD_Display[1][k + 5] = io_off;
					valid_io = 1;
				}
				if ((io_valid[io_loc[m+2]/8] & bittbl[io_loc[m+2] & 0x7]) != 0)
				{
					LCD_Display[2][k + 5] = io_off;
					valid_io = 1;
				}
				if ((io_valid[io_loc[m+3]/8] & bittbl[io_loc[m+3] & 0x7]) != 0)
				{
					LCD_Display[3][k + 5] = io_off;
					valid_io = 1;
				}
	  	    }

			if (valid_io == 0)
			{
				LCD_Display[0][2] = '.';
				LCD_Display[0][3] = '.';
				LCD_Display[0][4] = '.';
			}
		}
  	}
	if ((dpy_header == 0) && (valid_io == 1))
	{
	  	for (i=0;i<=11;i++)
	  	{
			k = io_loc[i]/8;
			if ((io_valid[k] & bittbl[io_loc[i] & 0x7]) != 0) 
			{		 // valid i/o
				if((iodata[cons[carnmb]][k] & io_bit_mask[i]) != 0)
				{
				    if((IO_In_Out[k] & io_bit_mask[i]) != 0)	// it is an output off
						LCD_Display[io_row[i]][io_col[i]] = io_off;
				    else										// it is an input  on
						LCD_Display[io_row[i]][io_col[i]] = io_on;
				}
		 		else
				{
				    if((IO_In_Out[k] & io_bit_mask[i]) != 0)	// it is an output on
						LCD_Display[io_row[i]][io_col[i]] = io_on;
		  	  	  	else										// it is an input off
						LCD_Display[io_row[i]][io_col[i]] = io_off;
				}
			}
	  	}
	}
	return(lcd_menu);
}

//*******************************************
// Routine to Display the Group IO
//*******************************************

int16 Display_Group_IO(int16 lcd_menu)
{
	int16 i,j,k,m,n;
	int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static uint8 local_init;
	static uint8 brd_sel;
	static int16 io_brd_offset;
	
	if(LCD_Sub_Init == 1)	// first time through Inputs and Outputs
	{
	    LCD_Sub_Init = 0;
		menu_level = 0;
		local_init = 1;
		brd_sel = 0;
  	    upd_dpy = 1;
		hc_io_ix = 0;
		comm_dpy = 1;
		valid_io = 0;

		for(i=0;i<nmb_hcio_brds;i++)
		{				  // Get the next io board being used
			if (HC_IO_board[i] != 0)
				break;
		}
		if (i < nmb_hcio_brds)	 // exited before the end so found a board
			brdnmb = i;
		else
			brdnmb = 0;
	}
	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	{
	    ModeButton = 1;  //
		lcd_menu = 0;
		return(lcd_menu);
	}							 
	if (menu_level == 0)
	{
		
		if(local_init != 0)	// first time through Inputs and Outputs
		{
 		    local_init = 0;
		    upd_dpy = 1;
			valid_io = 0;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
			valid_io = 0;
			if (comm_dpy == 1)
				comm_dpy = 0;
			else if ((hc_io_ix + 12) < 24)
				hc_io_ix += 12;
			else 
			{
				comm_dpy = 1;
				hc_io_ix = 0;
				j = brdnmb + 1;
				for (i=0;i<=nmb_hcio_brds;i++)		// loop around + 1
				{				  // Get the next io board being used
					if (HC_IO_board[j] != 0)
						break;
					else if (j < (nmb_hcio_brds-1))
						j++;
					else
						j = 0;
				}
				brdnmb = j;
				if ((HC_IO_board[brdnmb] & 0x40) != 0)
				{
					menu_level = 1;
					local_init = 1;
					return(lcd_menu);
				}
			}
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
			valid_io = 0;
			if (hc_io_ix >= 12)
				hc_io_ix -= 12;
			else if (comm_dpy == 0)
				comm_dpy = 1;
			else
			{
				comm_dpy = 0;
				hc_io_ix = 12;
				if (brdnmb > 0)
					j = brdnmb - 1;
				else 
					j = nmb_hcio_brds - 1;
				for (i=0;i<=nmb_hcio_brds;i++)		// loop around + 1
				{				  // Get the next io board being used
					if (HC_IO_board[j] != 0)
						break;
					else if (j > 0)
						j--;
					else
						j = nmb_hcio_brds - 1;
				}
				brdnmb = j;
				if ((HC_IO_board[brdnmb] & 0x40) != 0)
				{
					menu_level = 1;
					local_init = 2;
					return(lcd_menu);
				}
			}
		}
		if (comm_dpy == 1)
		{
			io_status = get_comm_status(brdnmb);
			if (upd_dpy == 1)
			{
				upd_dpy = 0;
		    	for(i=0; i<=19; i++)
	  		    {
					LCD_Display[0][i] = ' ';
					if (io_status == 2)
					{
						LCD_Display[1][i] = hc_brd_comm[3][i];
				  		LCD_Display[2][i] = hc_brd_comm[4][i];
					}
					else
					{
						if ((HC_IO_board[brdnmb] & 0x02) != 0)
						{
							LCD_Display[1][i] = hc_brd_comm[3][i];
					  		LCD_Display[2][i] = hc_brd_comm[2][i];
						}
						else
						{
							LCD_Display[1][i] = hc_brd_comm[0][i];
					  		LCD_Display[2][i] = hc_brd_comm[2][i];
						}
	  		    	}
			  		LCD_Display[3][i] = ' ';
	  		    }
			}
			if (io_status == 2)
				k = 0;
			else
				k = 1;
			itoa((brdnmb+k), LCD_String);  // Index
			LCD_Display[1][18] = LCD_String[0];
			if (LCD_String[1] != 0)
				LCD_Display[1][19] = LCD_String[1];
				
			if (io_status == 2)
			  	LCD_Display[2][15] = ' ';
			else if(io_status == 1)	  // it is an input  on
			  	LCD_Display[2][15] = '1';
		  	else				// it is an input off
			  	LCD_Display[2][15] = '0';

		}
		else
		{
			m = (brdnmb * 24) + hc_io_ix;
	  		if(upd_dpy == 1)
	  		{			 // Output 8 i/o at a time
				upd_dpy = 0;

			//		Example:
			//	  "xxHCP.|xxx3D.|xxx5D.",	// 0
			//	  "xxx1U.|xxx3U.|xxx5U.",	// 1
			//	  "xxx2D.|xxx4D.|xxx6D.",	// 2
			//	  "xxx2U.|xxx4U.|xxx6U.",	// 3
					
	  		    for(i=0; i<=19; i++)
	  		    {
	  		    	LCD_Display[0][i] = LCD_IO_Dpy_Row[i];
					LCD_Display[1][i] = LCD_IO_Dpy_Row[i];
				  	LCD_Display[2][i] = LCD_IO_Dpy_Row[i];
	    		    LCD_Display[3][i] = LCD_IO_Dpy_Row[i];
			    }
				valid_io = 0;
	    		for(i=0; i<=2; i++)
	  		    {
				    k = (i * 7);
				    n = i * 4;
					for (j=0;j<=4;j++)
					{
					  	LCD_Display[0][k + j] = hcioname[m+n][j];
					  	LCD_Display[1][k + j] = hcioname[m+n+1][j];
					  	LCD_Display[2][k + j] = hcioname[m+n+2][j];
					  	LCD_Display[3][k + j] = hcioname[m+n+3][j];
	  		    	}
					if ((gio_valid[(m+n)/8] & bittbl[(m+n) & 0x7]) != 0)
					{
						LCD_Display[0][k + 5] = io_off;
						valid_io = 1;
					}
					if ((gio_valid[(m+n+1)/8] & bittbl[(m+n+1) & 0x7]) != 0)
					{
						LCD_Display[1][k + 5] = io_off;
						valid_io = 1;
					}
					if ((gio_valid[(m+n+2)/8] & bittbl[(m+n+2) & 0x7]) != 0)
					{
						LCD_Display[2][k + 5] = io_off;
						valid_io = 1;
					}
					if ((gio_valid[(m+n+3)/8] & bittbl[(m+n+3) & 0x7]) != 0)
					{
						LCD_Display[3][k + 5] = io_off;
						valid_io = 1;
					}
	 		    }
				if (valid_io == 0)
				{
					LCD_Display[0][2] = '.';
					LCD_Display[0][3] = '.';
					LCD_Display[0][4] = '.';
				}
	  		}
			if (valid_io == 1)
			{
		  		for (i=0;i<=11;i++)
		  		{
					j = hc_io_ix + i + 1;
					if ((gio_valid[(m+i)/8] & bittbl[(m+i) & 0x7]) != 0)
					{
						io_status = get_io_status(brdnmb,j);
						
					  	if(io_status != 0)	  // it is an input  on
						  	LCD_Display[io_row[i]][io_col[i]] = io_on;
		  		  		else				// it is an input off
						  	LCD_Display[io_row[i]][io_col[i]] = io_off;
					}
		  		}
			}
		}
	}
	if (menu_level == 1)
	{
		if(local_init != 0)	// first time through Inputs and Outputs
		{
			io_brd_offset = 0;
			for(i=0;i<nmb_hcio_brds;i++)
			{				  // Get the next io board being used
				if ((HC_IO_board[i] & 0x40) != 0)
				{
					io_brd_offset = i - 1;
					break;
				}
			}

			if (local_init == 1)
				brd_sel = 0;
			else 
				brd_sel = 1;
		    upd_dpy = 1;
		    local_init = 0;
			valid_io = 0;
		}
		if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		{
		    UpButton = 1;  // incriment
		    upd_dpy = 1;
			valid_io = 0;
			j = brdnmb + 1;
			for (i=0;i<=nmb_hcio_brds;i++)		// loop around + 1
			{				  // Get the next io board being used
				if (HC_IO_board[j] != 0)
					break;
				else if (j < (nmb_hcio_brds-1))
					j++;
				else
					j = 0;
			}
			brdnmb = j;
			if ((HC_IO_board[brdnmb] & 0x40) == 0)
			{
				comm_dpy = 1;
				hc_io_ix = 0;
				menu_level = 0;
				local_init = 1;
				return(lcd_menu);
			}
		}
		if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		{
		    DownButton = 1;  // decriment
		    upd_dpy = 1;
			valid_io = 0;
			if (brdnmb > 0)
				j = brdnmb - 1;
			else 
				j = nmb_hcio_brds - 1;
			for (i=0;i<=nmb_hcio_brds;i++)		// loop around + 1
			{				  // Get the next io board being used
				if (HC_IO_board[j] != 0)
					break;
				else if (j > 0)
					j--;
				else
					j = nmb_hcio_brds - 1;
			}
			brdnmb = j;
			if ((HC_IO_board[brdnmb] & 0x40) == 0)
			{
				menu_level = 0;
				local_init = 2;
				comm_dpy = 0;
				hc_io_ix = 12;
				return(lcd_menu);
			}
		}

		if (upd_dpy == 1)
		{
			upd_dpy = 0;
			//		Example
			//	  " Hall Call Board #1 ",	// 0
			//	  " Comm Status = 0   .",	// 1
			//	  "   |xxxxx |xx1UA |  ",	// 2
			//	  "   |xxHCP |xxx1U | .",	// 3
						
	    	for(i=0; i<=19; i++)
  		    {
				LCD_Display[0][i] = hc_brd_comm[1][i];
		  		LCD_Display[1][i] = hc_brd_comm[2][i];
  		    }
  		    
			io_status = get_comm_status(brdnmb);
			itoa(brdnmb-io_brd_offset, LCD_String);  // Index
			LCD_Display[0][18] = LCD_String[0];
			if (LCD_String[1] != 0)
				LCD_Display[0][19] = LCD_String[1];
				
			if(io_status == 1)	  // it is an input on
			  	LCD_Display[1][15] = '1';
		  	else				// it is an input off
			  	LCD_Display[1][15] = '0';

			m = (brdnmb * 8);
  		    for(i=0; i<=19; i++)
  		    {
			  	LCD_Display[2][i] = LCD_HC_IO_Dpy_Row[i];
    		    LCD_Display[3][i] = LCD_HC_IO_Dpy_Row[i];
		    }
			for (j=0;j<=4;j++)
			{
			  	LCD_Display[2][4 + j] = hcioname[m][j];
			  	LCD_Display[3][4 + j] = hcioname[m+1][j];
			  	LCD_Display[2][11 + j] = hcioname[m+2][j];
			  	LCD_Display[3][11 + j] = hcioname[m+3][j];
 		    }
		
			valid_io = 0;
			if ((gio_valid[brdnmb] & bittbl[0]) != 0)
			{
				LCD_Display[2][9] = io_off;
				valid_io = 1;
			}
			if ((gio_valid[brdnmb] & bittbl[1]) != 0)
			{
				LCD_Display[3][9] = io_off;
				valid_io = 1;
			}
			if ((gio_valid[brdnmb] & bittbl[2]) != 0)
			{
				LCD_Display[2][16] = io_off;
				valid_io = 1;
			}
			if ((gio_valid[brdnmb] & bittbl[3]) != 0)
			{
				LCD_Display[3][16] = io_off;
				valid_io = 1;
			}
			if (valid_io == 0)
			{
				LCD_Display[2][2] = '.';
				LCD_Display[2][3] = '.';
				LCD_Display[2][4] = '.';
			}

		}
		
		if (valid_io == 1)
		{
			for (i=0;i<=3;i++)
			{
				if ((gio_valid[brdnmb] & bittbl[i & 0x7]) != 0)
				{
					io_status = get_hcio_status(brdnmb,i);
						
				  	if(io_status != 0)	  // it is an input  on
					  	LCD_Display[hcio_row[i]][hcio_col[i]] = io_on;
		  	  		else				// it is an input off
					  	LCD_Display[hcio_row[i]][hcio_col[i]] = io_off;
				}
		  	}

		}
	}
	return(lcd_menu);
}

//*******************************************
// Routine to Relocate the Car and Group IO
//*******************************************


int16 Relocate_IO(int16 lcd_menu)
{
	int16 i,j;
	static int16 upd_dpy;
	static int16 menu_level;
	static int16 menu_ptr;
	static int16 mode_sel;
	static int16 menu_init;
	
	if (LCD_Sub_Init == 1)
	{		// first time through
		LCD_Sub_Init = 0;
		menu_ptr = 0;
  	    upd_dpy = 1;
  	    menu_level = 0;
  	    mode_sel = 0;
  	    spare_io_ix = 0;
	}
	
	if (menu_level == 0)
	{
		
	  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	  	{
	  	    ModeButton = 1;  //
			lcd_menu = 0;
			return(lcd_menu);
	  	}
	  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
	  	{
	  	    UpButton = 1;  // incriment
	  	    upd_dpy = 1;
	  	    menu_ptr--;
			if(menu_ptr < 0)
			    menu_ptr = 2;
	  	}
	  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
	  	{
	  	    DownButton = 1;  // decriment
	  	    upd_dpy = 1;
	  	    menu_ptr++;
			if(menu_ptr > 2)	  
				menu_ptr = 0;
	  	}

	 	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
	  	{
	  	    EnterButton = 1;
	  		spare_io_ix = 0;
	  		menu_level = menu_ptr + 1;
	  	    upd_dpy = 1;
	  	    mode_sel = 0;
	  	    if (menu_ptr == 2)
	  	    	menu_init = 1;	
			return(lcd_menu);
	  	}

		if (upd_dpy == 1)
		{
		    upd_dpy = 0;
			for(i=0; i<=19; i++)
			{
			    LCD_Display[0][i] = getcaps(LCD_Input_Output[4][i]);
				LCD_Display[1][i] = LCD_Relocate_IO_Menu[0][i];
				LCD_Display[2][i] = LCD_Relocate_IO_Menu[1][i];
				LCD_Display[3][i] = LCD_Relocate_IO_Menu[2][i];
			}
			LCD_Display[menu_ptr+1][0] = '>';		// show cursor
		}
	}
	else if (menu_level == 1)
	{
		if (mode_sel == 0)
		{
			
		  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		  	{
		  	    ModeButton = 1;  //
		  	    upd_dpy = 1;
				menu_level = 0;
				return(lcd_menu);
		  	}
		  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		  	{
		  	    UpButton = 1;  // incriment
		  	    upd_dpy = 1;
		  	    spare_io_ix++;
		  	    if ((cons[sercom] & 0x20) != 0)
		  	    {		// group serial driver boards cannot relocate io
					if(spare_io_ix > 11)	  
						spare_io_ix = 0;
		  	    }
		  	    else
		  	    {
					if(spare_io_ix > 17)	  
						spare_io_ix = 0;
		  	    }
		  	}
		  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		  	{
		  	    DownButton = 1;  // decriment
		  	    upd_dpy = 1;
		  	    spare_io_ix--;
		  	    if ((cons[sercom] & 0x20) != 0)
		  	    {		// group serial driver boards cannot relocate io
					if(spare_io_ix < 0)
					    spare_io_ix = 11;
		  	    }
		  	    else
		  	    {
					if(spare_io_ix < 0)
					    spare_io_ix = 17;
		  	    }
		  	}

		 	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		  	{
		  	    EnterButton = 1;
		  		mode_sel = 1;
		  		get_init_io_ix(1);
		  	    upd_dpy = 1;
		  	}
			if (upd_dpy == 1)
			{
		  	    upd_dpy = 0;
				for(i=0; i<=19; i++)
					LCD_Display[1][i] = getcaps(LCD_Relocate_IO_Menu[0][i]);
				display_reloc_status(0);
			}
		}
		else
		{
			if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		  	{
		  	    ModeButton = 1;  //
		  		mode_sel = 0;
		  	    upd_dpy = 1;
				return(lcd_menu);
		  	}
		  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		  	{
		  	    UpButton = 1;  // incriment
		  	    upd_dpy = 1;
		  	    get_next_io_ix(1);
		  	}
		  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		  	{
		  	    DownButton = 1;  // decriment
		  	    upd_dpy = 1;
		  	    get_next_io_ix(0);
		  	}

		 	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		  	{
		  		mode_sel = 0;
		  		for (i=0;i<=4;i++)
		  		{
		  			// copy ioname to spare_io_name
		  			if (spare_io_ix < 12)
		  				spare_ioname[spare_io_ix][i] = ioname[io_ix][i];
		  			else
		  				spare_ioname[spare_io_ix][i] = hcioname[io_ix][i];
		  		}
		  		spare_io_stat[spare_io_ix] = 0x27;
		  		// write spare_io_name to mram
		  		write_spare_ios();
		  		
		  	    EnterButton = 1;
		  	    upd_dpy = 1;
		   		LCD_Flash = 0;		// force the label to be displayed instead of the blank line
		  	}
			if (LCD_Flash != prev_Flash)
			{
				prev_Flash = LCD_Flash;
		  	    upd_dpy = 1;
			}
			
			if (upd_dpy == 1)
			{
		  	    upd_dpy = 0;
				for(i=0; i<=19; i++)
					LCD_Display[1][i] = getcaps(LCD_Relocate_IO_Menu[0][i]);
				display_reloc_status(1);
				if (LCD_Flash != 0)
				{
					for(i=0; i<=4; i++)
						LCD_Display[3][i+12] = ' ';
				}
			}
		}
	}
	else if (menu_level == 2)
	{		// clear individual io relocation
		if (mode_sel == 0)
		{
			
		  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		  	{
		  	    ModeButton = 1;  //
			    menu_ptr = 1;
				menu_level = 0;
		  	    upd_dpy = 1;
				return(lcd_menu);
		  	}
		  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		  	{
		  	    UpButton = 1;  // incriment
		  	    upd_dpy = 1;
		  	    spare_io_ix++;
		  	    if ((cons[sercom] & 0x20) != 0)
		  	    {		// group serial driver boards cannot relocate io
					if(spare_io_ix > 11)	  
						spare_io_ix = 0;
		  	    }
		  	    else
		  	    {
					if(spare_io_ix > 17)	  
						spare_io_ix = 0;
		  	    }
		  	}
		  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		  	{
		  	    DownButton = 1;  // decriment
		  	    upd_dpy = 1;
		  	    spare_io_ix--;
		  	    if ((cons[sercom] & 0x20) != 0)
		  	    {		// group serial driver boards cannot relocate io
					if(spare_io_ix < 0)
					    spare_io_ix = 11;
		  	    }
		  	    else
		  	    {
					if(spare_io_ix < 0)
					    spare_io_ix = 17;
		  	    }
		  	}

		 	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		  	{
		  	    EnterButton = 1;
		  		if (spare_io_stat[spare_io_ix] == 0x27)
		  			mode_sel = 1;
		  	    upd_dpy = 1;
		  	}
			if (upd_dpy == 1)
			{
		  	    upd_dpy = 0;
				for(i=0; i<=19; i++)
					LCD_Display[1][i] = getcaps(LCD_Relocate_IO_Menu[1][i]);
				display_reloc_status(0);
			}
		}
		else
		{
			if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
		  	{
		  	    ModeButton = 1;  //
		  		mode_sel = 0;
		  	    upd_dpy = 1;
	  			spare_io_stat[spare_io_ix] = 0x27;
				return(lcd_menu);
		  	}
		  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
		  	{
		  	    UpButton = 1;  // incriment
		  	    upd_dpy = 1;
		  		if (spare_io_stat[spare_io_ix] == 0x27)
		  			spare_io_stat[spare_io_ix] = 0;
		  		else
		  			spare_io_stat[spare_io_ix] = 0x27;
		  	}
		  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
		  	{
		  	    DownButton = 1;  // decriment
		  	    upd_dpy = 1;
		  		if (spare_io_stat[spare_io_ix] == 0x27)
		  			spare_io_stat[spare_io_ix] = 0;
		  		else
		  			spare_io_stat[spare_io_ix] = 0x27;
		  	}

		 	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		  	{
		  	    EnterButton = 1;
		  		mode_sel = 0;
		  		if (spare_io_stat[spare_io_ix] == 0)
		  		{
			  		for (i=0;i<=4;i++)
			  		{
			  			// clear the spare_io_name
		  				spare_ioname[spare_io_ix][i] = ' ';
			  		}
			  		spare_io_stat[spare_io_ix] = 0;
			  		// write spare_io_name to mram
			  		write_spare_ios();
		  		}
		  		
		  	    upd_dpy = 1;
		   		LCD_Flash = 0;		// force the label to be displayed instead of the blank line
		  	}
			if (LCD_Flash != prev_Flash)
			{
				prev_Flash = LCD_Flash;
		  	    upd_dpy = 1;
			}
			
			if (upd_dpy == 1)
			{
		  	    upd_dpy = 0;
				for(i=0; i<=19; i++)
				LCD_Display[1][i] = getcaps(LCD_Relocate_IO_Menu[1][i]);
				display_reloc_status(0);
				if (LCD_Flash != 0)
				{
					for(i=0; i<=4; i++)
						LCD_Display[3][i+12] = ' ';
				}
			}
		}
	}
	else if (menu_level == 3)
	{
		
		if(menu_init == 1)	// first time through Clear IO Relocation Table
		{
		    menu_init = 0;
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = ' ';
		  		LCD_Display[1][i] = LCD_Clear_IO_Reloc_TBL[0][i];
				LCD_Display[2][i] = LCD_Clear_IO_Reloc_TBL[1][i];
				LCD_Display[3][i] = ' ';
		    }
		}
		if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
		{
		    EnterButton = 1;
		    for(i=0; i<=19; i++)
		    {
				LCD_Display[0][i] = ' ';
				LCD_Display[1][i] = LCD_Clear_IO_Reloc_TBL[1][i];
				LCD_Display[2][i] = LCD_Clear_IO_Reloc_TBL[2][i];
				LCD_Display[3][i] = ' ';
		    }
		    for (j=0;j<=17;j++)
		    {
		    	
			  	for (i=0;i<=4;i++)
			  	{
			  		// copy ioname to spare_io_name
			  		spare_ioname[j][i] = SPARE_IOS[j][i];
			  	}
			  	spare_io_stat[j] = 0;
		    }
		  	// write spare_io_name to mram
		  	write_spare_ios();
		  		
		}
	  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
	  	{
	  	    ModeButton = 1;  //
		    menu_ptr = 2;
			menu_level = 0;
	  	    upd_dpy = 1;
			return(lcd_menu);
	  	}
	}
	return(lcd_menu);
}

//************************************************************
// Display the IO Relocation Status
//************************************************************

void display_reloc_status(int16 mode)
{
	int16 i;
	
	for(i=0; i<=19; i++)
	{
		LCD_Display[2][i] = LCD_Add_Remove_IO_Menu[0][i];
	    LCD_Display[3][i] = LCD_Add_Remove_IO_Menu[1][i];
	}
	
	switch(mode)
	{
		
		case 0:
			for(i=0; i<=4; i++)
			{
				LCD_Display[2][i+15] = SPARE_IOS[spare_io_ix][i];
				if (spare_io_stat[spare_io_ix] == 0x27)
			    	LCD_Display[3][i+12] = spare_ioname[spare_io_ix][i];
				else
			    	LCD_Display[3][i+12] = spare_io_none[i];
					
			}
			LCD_Display[2][0] = '>';		// show cursor
			break;
	
		case 1:
			for(i=0; i<=4; i++)
			{
				LCD_Display[2][i+15] = SPARE_IOS[spare_io_ix][i];
				if (spare_io_ix >= 12)
			    	LCD_Display[3][i+12] = hcioname[io_ix][i];
				else
			    	LCD_Display[3][i+12] = ioname[io_ix][i];
					
			}
			LCD_Display[3][0] = '>';		// show cursor
			break;
			
		default:
			break;
	}
}

//************************************************************
// Get the Initial IO relocation Index
//************************************************************
	
void get_init_io_ix (int8 search_dir)
{
	ioport = spare_io_ix/6; 		// 0 = MRCAN, 1 = CTCAN, 2 = GRCAN
	iotype = ((spare_io_ix/3) & 0x01);	// 0 = input, 1 = output
	
	if (search_dir == 1)		// search up
	{
		if (ioport == 0)
		{
			io_ix_start = 18 * 8;
			io_ix = io_ix_start;
			io_ix_max = mr_last_array * 8;
		}
		else if (ioport == 1)
		{
			io_ix_start = 153 * 8;
			io_ix = io_ix_start;
			io_ix_max = ct_last_array * 8;
		}
		else if (ioport == 2)
		{
			io_ix_start = 0;
			io_ix = io_ix_start;
			io_ix_max = gr_last_array * 8;
		}
		else
		{
			io_ix = 0;
			return;
		}
		
		if (iotype != 0)
			io_ix += 12;
		while (io_ix < io_ix_max)
		{
			if (Not_Valid_IO_Name(ioport) == 1)
			{
			 	io_ix++;
			 	if ((io_ix/12 & 0x01) != iotype)	
			 		io_ix += 12;		// go to next iotype location
			 }
			 else
			 	break;
		}
	}
	else
	{
		if (ioport == 0)
		{
			io_ix = mr_last_array * 8;
			io_ix_start = 18 * 8;
		}
		else if (ioport == 1)
		{
			io_ix = ct_last_array * 8;
			io_ix_start = 153 * 8;
		}
		else if (ioport == 2)
		{
			io_ix = gr_last_array * 8;
			io_ix_start = 0;
		}
		else
		{
			io_ix = 0;
			return;
		}
		
	 	if ((io_ix/12 & 0x01) != iotype)	
	 		io_ix -= 12;		// go to next iotype location
		while (io_ix >= io_ix_start)
		{
			if (Not_Valid_IO_Name(ioport) == 1)
			{
			 	io_ix--;
			 	if ((io_ix/12 & 0x01) != iotype)	
			 		io_ix -= 12;		// go to next iotype location
			}
			else
			 	break;
		}
	}
	
}


//************************************************************
// Get the next IO relocation Index
//************************************************************
	
void get_next_io_ix (int8 search_dir)
{
	ioport = spare_io_ix/6; 		// 0 = MRCAN, 1 = CTCAN, 2 = GRCAN
	iotype = ((spare_io_ix/3) & 0x01);	// 0 = input, 1 = output
	
	if (search_dir == 1)		// search up
	{
		if (ioport == 0)
		{
			io_ix_max = mr_last_array * 8;
		}
		else if (ioport == 1)
		{
			io_ix_max = ct_last_array * 8;
		}
		else if (ioport == 2)
		{
			io_ix_max = gr_last_array * 8;
		}
		else
		{
			io_ix = 0;
			return;
		}
		
		io_ix++;
	 	if ((io_ix/12 & 0x01) != iotype)	
			io_ix += 12;
		while (io_ix < io_ix_max)
		{
			if (Not_Valid_IO_Name(ioport) == 1)
			{
			 	io_ix++;
			 	if ((io_ix/12 & 0x01) != iotype)	
			 		io_ix += 12;		// go to next iotype location
			 }
			 else
			 	break;
		}
		if (io_ix >= io_ix_max)
			get_init_io_ix(1);		
	}
	else	// down direction search
	{
		if (ioport == 0)
		{
			io_ix_start = 18 * 8;
		}
		else if (ioport == 1)
		{
			io_ix_start = 153 * 8;
		}
		else if (ioport == 2)
		{
			io_ix_start = 0;
		}
		else
		{
			io_ix = 0;
			return;
		}
		
		io_ix--;
	 	if ((io_ix/12 & 0x01) != iotype)	
	 		io_ix -= 12;		// go to next iotype location
		while (io_ix >= io_ix_start)
		{
			if (Not_Valid_IO_Name(ioport) == 1)
			{
			 	io_ix--;
			 	if ((io_ix/12 & 0x01) != iotype)	
			 		io_ix -= 12;		// go to next iotype location
			}
			else
				break;
		}
		if (io_ix < io_ix_start)
			get_init_io_ix(0);		
	}
}

// ***********************************************
// Checks for Not a valid IO Name on port
// ***********************************************

	
int16 Not_Valid_IO_Name(int16 port)
{
	if (port == 0)
	{
		if (((ioname[io_ix][0] == ' ') &&
		 	(ioname[io_ix][1] == ' ') &&
		 	(ioname[io_ix][2] == ' ') &&
		 	(ioname[io_ix][3] == ' ') &&
		 	(ioname[io_ix][4] == ' ')) ||
		 		((ioname[io_ix][0] == 'M') &&
		 	 	 (ioname[io_ix][1] == 'S') &&
		 	 	 (ioname[io_ix][2] == 'P') &&
		 	 	 ((ioname[io_ix][3] == 'I') || (ioname[io_ix][3] == 'O')) &&
		 	 	 ((ioname[io_ix][4] == '1')  || (ioname[io_ix][4] == '2')  || (ioname[io_ix][4] == '3')) ))
		 {
		 	return 1;
		 }
	}
	else if (port == 1)
	{
		if (((ioname[io_ix][0] == ' ') &&
		 	(ioname[io_ix][1] == ' ') &&
		 	(ioname[io_ix][2] == ' ') &&
		 	(ioname[io_ix][3] == ' ') &&
		 	(ioname[io_ix][4] == ' ')) ||
		 		((ioname[io_ix][0] == 'C') &&
		 	 	 (ioname[io_ix][1] == 'S') &&
		 	 	 (ioname[io_ix][2] == 'P') &&
		 	 	 ((ioname[io_ix][3] == 'I') || (ioname[io_ix][3] == 'O')) &&
		 	 	 ((ioname[io_ix][4] == '1')  || (ioname[io_ix][4] == '2')  || (ioname[io_ix][4] == '3')) ))
		 {
		 	return 1;
		 }
	}
	else if (port == 2)
	{
		if (((hcioname[io_ix][0] == ' ') &&
		 	(hcioname[io_ix][1] == ' ') &&
		 	(hcioname[io_ix][2] == ' ') &&
		 	(hcioname[io_ix][3] == ' ') &&
		 	(hcioname[io_ix][4] == ' ')) ||
		 		((hcioname[io_ix][0] == 'G') &&
		 	 	 (hcioname[io_ix][1] == 'S') &&
		 	 	 (hcioname[io_ix][2] == 'P') &&
		 	 	 ((hcioname[io_ix][3] == 'I') || (hcioname[io_ix][3] == 'O')) &&
		 	 	 ((hcioname[io_ix][4] == '1')  || (hcioname[io_ix][4] == '2')  || (hcioname[io_ix][4] == '3')) ))
		 {
		 	return 1;
		 }
	}
	return 0;		// else valid io name
}

//************************************************************
// ** This routine gets the io status for the hall call boards
//************************************************************
uint16 get_io_status(int16 brdnmb, int16 ix)
{
	uint16 io_status = 0;
	int16 array = (brdnmb * 3) + ((ix-1)/8);
	int16 bit = bittbl[(ix-1)%8];
	int16 io_comp = ((GIO_In_Out[array] & bit) != 0);
	
	if (HC_IO_board[brdnmb] != 0)
	{
		if ((giodata(array) &  bit) != 0)
			io_status = 1;
	}
	if (io_comp == 1)
		io_status = (uint16)((~io_status) & 0x01);
	return (io_status);
}

uint16 get_hcio_status(int16 brdnmb, int16 ix)
{
	uint16 io_status = 0;
	int16 array = brdnmb;
	int16 bit = bittbl[ix];
	int16 io_comp = ((GIO_In_Out[array] & bit) != 0);
	
	if (HC_IO_board[brdnmb] != 0)
	{
		if ((giodata(array) &  bit) != 0)
			io_status = 1;
	}
	if (io_comp == 1)
		io_status = (uint16)((~io_status) & 0x01);
	return (io_status);
}

//***********************************************
//******** Find the i/o start *******************
//***********************************************
void find_io_start(int16 inc)
{
	int16 i,k,brd,found,io_ix;

	if (inc == 1)
	{
		if (io_ix_last < nmb_ios)
			io_ix = io_ix_last + 1;
		else
			io_ix = 0;
		for (i=0;i<=11;i++)
		{
			found = 0;
			k = 0;
			while ((found == 0) && (k < nmb_ios))
			{
				brd = (io_ix/24);
				if (IO_board[cons[carnmb]][brd] != 0)
					found = 1;
				else
				{
					if (io_ix < nmb_ios)
						io_ix++;
					else
						io_ix = 0;
				}
				k++;
			}

			if (i == 0) 
				io_ix_first = io_ix;
			if (i == 11)
				io_ix_last = io_ix;
			io_loc[i] = io_ix;
			io_bit_mask[i] = bittbl[io_ix & 7];
			if (io_ix < nmb_ios)
				io_ix++;
			else
				io_ix = 0;
		}
	}
	else
	{
		if (io_ix_first > 0)
			io_ix = io_ix_first - 1;
		else
			io_ix = nmb_ios;			
		for (i=11;i>=0;i--)
		{
			found = 0;
			k = 0;
			while ((found == 0) && (k < nmb_ios))
			{
				brd = (io_ix/24);
				if (IO_board[cons[carnmb]][brd] != 0)
					found = 1;
				else
				{
					if (io_ix > 0)
						io_ix--;
					else
						io_ix = nmb_ios;
				}
				k++;
			}

			if (i == 0) 
				io_ix_first = io_ix;
			if (i == 11)
				io_ix_last = io_ix;
			io_loc[i] = io_ix;
			io_bit_mask[i] = bittbl[io_ix & 7];
			if (io_ix > 0)
				io_ix--;
			else
				io_ix = nmb_ios;
		}
	}
}


// FPGA1 IO display  --- make an even number of lines all the time
const char FPGA1_IOS [16][21] = 
{
//			Bit# 7		Bit# 6		Bit# 5		Bit# 4		Bit# 3		Bit# 2		Bit# 1		Bit# 0
//	Byte# 0	AUTO_L		LBP_L		GLB_L		GBP_L		BAU_L		BAD_L		TAU_L		TAD_L
//	Byte# 1	DLB_L		DLM_L		DLT_L		RLM_L		LPS_L		RGS_L		GS_L		ACC_L
//	Byte# 2	ICI_L		CTIN_L		IU_L		ID_L		MRI_L		MRIU_L		MRID_L		"0" Always
//	Byte# 3	CEN_CPU		MC_CPU		STE_CPU		DNR_CPU		UPR_CPU		UPF_CPU		DNF_CPU		UL_CPU

//	Byte# 4	DZ_CPU		DL_CPU		DZ_PAL		SPD_150_CPU	SPD_75_CPU	SEL_OK_CPU	RUN_CPU		PSDF_LP
//	Byte# 5	PSUF_LP		PSU_LP		PSD_LP		PDT_LP		PUT_LP		PUN_LP		PDN_LP		PSTE_LP
//	Byte# 6	PMC_LP		PCEN_LP		PRUN_LP		PAL_FLT		DT_NTS		UT_NTS		UN_NTS		DN_NTS
//	Byte# 7	DZ_NTS		SEL_OK_NTS	SPD_150NTS	SPD_75_NTS	NTS_FLT		REAR_DOOR	TEST_MODE	"0" Always

  "AUTO*|LBP*|GLB*|GBP*",
  " BAU*|BAD*|TAU*|TAD*",
  "CTIN*| IU*| ID*|ICI*",
  " MRI*|MIU*|MID*|ACC*",
  
  "RUNc*|MCc*|SUc*|UFc*",
  "STEc*|OKc*|SDc*|DFc*",
  "RUNp*|MCp*|SUp*|UFp*",
  "STEp*|    |SDp*|DFp*",
  
  " DLB*|DLM*|DLT*|RLM*",
  " LPS*|RGS*| GS*|    ",
  "CENc*|ULc*|DZc*|DLc*",
  "150c*|75c*|DZp*|DZn*",
  
  "150n*|75n*|UNn*|UTn*",
  "FLTn*|OKn*|DNn*|DTn*",
  "CENp*|RDR*|UNp*|UTp*",
  "FLTp*|TST*|DNp*|DTp*",

};

int16 Display_FPGA1_IO (int16 lcd_menu)
{

	int16 i;
	int16 upd_dpy;
	static int16 page_dpy;
	
	if (LCD_Sub_Init == 1)
	{		// first time through
		LCD_Sub_Init = 0;
  	    upd_dpy = 1;
  	    page_dpy = 0;
	}
	
 	if((rdLCDio(Lcd_ENTER) == 0) && (EnterButton == 0))
  	{
  	    EnterButton = 1;
  	    upd_dpy = 1;
  	}
  	if((rdLCDio(Lcd_MODE) == 0) && (ModeButton == 0))
  	{
  	    ModeButton = 1;  //
		lcd_menu = 0;
		return(lcd_menu);
  	}
  	if((rdLCDio(Lcd_UP) == 0) && (UpButton == 0))
  	{
  	    UpButton = 1;  // incriment
  	    upd_dpy = 1;
    	page_dpy++;
  	    if (page_dpy > 3)
  	    	page_dpy = 0;
  	}
  	if((rdLCDio(Lcd_DN) == 0) && (DownButton == 0))
  	{
  	    DownButton = 1;  // decriment
  	    upd_dpy = 1;
  	    page_dpy--;
  	    if (page_dpy < 0)
  	    	page_dpy = 3;
  	}

	switch (page_dpy)
	{
		case 0:
			if (upd_dpy == 1)
			{
				upd_dpy = 0;
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = FPGA1_IOS[0][i];
					LCD_Display[1][i] = FPGA1_IOS[1][i];
				    LCD_Display[2][i] = FPGA1_IOS[2][i];
					LCD_Display[3][i] = FPGA1_IOS[3][i];
				}
			}

			//	Byte# 0	AUTO_L		LBP_L		GLB_L		GBP_L		BAU_L		BAD_L		TAU_L		TAD_L
			//  "AUTO*|LBP*|GLB*|GBP*",

			if (i1spi_in.ST.buff0.B.AUTO == 1)	// AUTO
				LCD_Display[0][4] = io_on;
			else
				LCD_Display[0][4] = io_off;
			
			if (i1spi_in.ST.buff0.B.LBP == 1)	// LBP
				LCD_Display[0][9] = io_on;
			else
				LCD_Display[0][9] = io_off;
				
			if (i1spi_in.ST.buff0.B.GLB == 1)	// GLB
				LCD_Display[0][14] = io_on;
			else
				LCD_Display[0][14] = io_off;
			
			if (i1spi_in.ST.buff0.B.GBP == 1)	// GBP
				LCD_Display[0][19] = io_on;
			else
				LCD_Display[0][19] = io_off;
				
			//	Byte# 0	AUTO_L		LBP_L		GLB_L		GBP_L		BAU_L		BAD_L		TAU_L		TAD_L
			//  " BAU*|BAD*|TAU*|TAD*",
				
			if (i1spi_in.ST.buff0.B.BAU == 1)	// BAU
				LCD_Display[1][4] = io_on;
			else
				LCD_Display[1][4] = io_off;
				
			if (i1spi_in.ST.buff0.B.BAD == 1)	// BAD
				LCD_Display[1][9] = io_on;
			else
				LCD_Display[1][9] = io_off;
				
			if (i1spi_in.ST.buff0.B.TAU == 1)	// TAU
				LCD_Display[1][14] = io_on;
			else
				LCD_Display[1][14] = io_off;
				
			if (i1spi_in.ST.buff0.B.TAD == 1)	// TAD
				LCD_Display[1][19] = io_on;
			else
				LCD_Display[1][19] = io_off;
				
				
			//	Byte# 2	ICI_L		CTIN_L		IU_L		ID_L		MRI_L		MRIU_L		MRID_L		"0" Always
			//  "CTIN*| IU*| ID*|ICI*",
				
			if (i1spi_in.ST.buff1.B.CTIN == 1)	// CTIN
				LCD_Display[2][4] = io_on;
			else
				LCD_Display[2][4] = io_off;
				
			if (i1spi_in.ST.buff1.B.IU == 1)	// IU
				LCD_Display[2][9] = io_on;
			else
				LCD_Display[2][9] = io_off;
				
			if (i1spi_in.ST.buff1.B.ID == 1)	// ID
				LCD_Display[2][14] = io_on;
			else
				LCD_Display[2][14] = io_off;
				
			if (i1spi_in.ST.buff1.B.ICI == 1)	// ICI
				LCD_Display[2][19] = io_on;
			else
				LCD_Display[2][19] = io_off;
				
				
			//	Byte# 2	ICI_L		CTIN_L		IU_L		ID_L		MRI_L		MRIU_L		MRID_L		"0" Always
			//	Byte# 1	DLB_L		DLM_L		DLT_L		RLM_L		LPS_L		RGS_L		GS_L		ACC_L
			//  " MRI*|MIU*|MID*|ACC*",
				
			if (i1spi_in.ST.buff1.B.MRI == 1)	// MRI
				LCD_Display[3][4] = io_on;
			else
				LCD_Display[3][4] = io_off;
				
			if (i1spi_in.ST.buff1.B.MRIU == 1)	// MRID
				LCD_Display[3][9] = io_on;
			else
				LCD_Display[3][9] = io_off;
				
			if (i1spi_in.ST.buff1.B.MRID == 1)	// MRID
				LCD_Display[3][14] = io_on;
			else
				LCD_Display[3][14] = io_off;
				
			if (i1spi_in.ST.buff0.B.ACC == 1)	// ACC
				LCD_Display[3][19] = io_on;
			else
				LCD_Display[3][19] = io_off;
			break;
		
		case 1:
			if (upd_dpy == 1)
			{
				upd_dpy = 0;
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = FPGA1_IOS[4][i];
					LCD_Display[1][i] = FPGA1_IOS[5][i];
				    LCD_Display[2][i] = FPGA1_IOS[6][i];
					LCD_Display[3][i] = FPGA1_IOS[7][i];
				}
			}

			//	Byte# 3	CEN_CPU		MC_CPU		STE_CPU		DNR_CPU		UPR_CPU		UPF_CPU		DNF_CPU		UL_CPU
			//	Byte# 4	DZ_CPU		DL_CPU		DZ_PAL		SPD_150_CPU	SPD_75_CPU	SEL_OK_CPU	RUN_CPU		PSDF_LP
			//  "RUNc*|MCc*|SUc*|UFc*",

			if (i1spi_in.ST.buff2.B.RUN_CPU == 1)	// RUN
				LCD_Display[0][4] = io_on;
			else
				LCD_Display[0][4] = io_off;
			
			if (i1spi_in.ST.buff1.B.MC_CPU == 1)	// MC
				LCD_Display[0][9] = io_on;
			else
				LCD_Display[0][9] = io_off;
				
			if (i1spi_in.ST.buff1.B.UPR_CPU == 1)	// SU
				LCD_Display[0][14] = io_on;
			else
				LCD_Display[0][14] = io_off;
			
			if (i1spi_in.ST.buff1.B.UPF_CPU == 1)	// SUF
				LCD_Display[0][19] = io_on;
			else
				LCD_Display[0][19] = io_off;
			

			//	Byte# 3	CEN_CPU		MC_CPU		STE_CPU		DNR_CPU		UPR_CPU		UPF_CPU		DNF_CPU		UL_CPU
			//	Byte# 4	DZ_CPU		DL_CPU		DZ_PAL		SPD_150_CPU	SPD_75_CPU	SEL_OK_CPU	RUN_CPU		PSDF_LP
				
			//  "STEc*|OKc*|SDc*|DFc*",
				
			if (i1spi_in.ST.buff1.B.STE_CPU == 1)	// STE
				LCD_Display[1][4] = io_on;
			else
				LCD_Display[1][4] = io_off;
				
			if (i1spi_in.ST.buff2.B.SEL_OK_CPU == 1)	// SEL OK
				LCD_Display[1][9] = io_on;
			else
				LCD_Display[1][9] = io_off;
				
			if (i1spi_in.ST.buff1.B.DNR_CPU == 1)	// SD
				LCD_Display[1][14] = io_on;
			else
				LCD_Display[1][14] = io_off;
				
			if (i1spi_in.ST.buff1.B.DNF_CPU == 1)	// SDF
				LCD_Display[1][19] = io_on;
			else
				LCD_Display[1][19] = io_off;
				
				

			//	Byte# 5	PSUF_LP		PSU_LP		PSD_LP		PDT_LP		PUT_LP		PUN_LP		PDN_LP		PSTE_LP
			//	Byte# 6	PMC_LP		PCEN_LP		PRUN_LP		PAL_FLT		DT_NTS		UT_NTS		UN_NTS		DN_NTS
			//  "RUNp*|MCp*|SUp*|UFp*",
				
			if (i1spi_in.ST.buff3.B.PRUN == 1)	// PRUN
				LCD_Display[2][4] = io_on;
			else
				LCD_Display[2][4] = io_off;
				
			if (i1spi_in.ST.buff3.B.PMC == 1)	// PMC
				LCD_Display[2][9] = io_on;
			else
				LCD_Display[2][9] = io_off;
				
			if (i1spi_in.ST.buff2.B.PSU == 1)	// PSU
				LCD_Display[2][14] = io_on;
			else
				LCD_Display[2][14] = io_off;
				
			if (i1spi_in.ST.buff2.B.PSUF == 1)	// PSUF
				LCD_Display[2][19] = io_on;
			else
				LCD_Display[2][19] = io_off;
				
				
			//	Byte# 4	DZ_CPU		DL_CPU		DZ_PAL		SPD_150_CPU	SPD_75_CPU	SEL_OK_CPU	RUN_CPU		PSDF_LP
			//	Byte# 5	PSUF_LP		PSU_LP		PSD_LP		PDT_LP		PUT_LP		PUN_LP		PDN_LP		PSTE_LP

			//  "STEp*|    |SDp*|DFp*",
				
			if (i1spi_in.ST.buff2.B.PSTE == 1)	// PSTE
				LCD_Display[3][4] = io_on;
			else
				LCD_Display[3][4] = io_off;
			
			
			// BLANK
				
			if (i1spi_in.ST.buff2.B.PSD == 1)	// PSD
				LCD_Display[3][14] = io_on;
			else
				LCD_Display[3][14] = io_off;
				
			if (i1spi_in.ST.buff2.B.PSDF == 1)	// PSDF
				LCD_Display[3][19] = io_on;
			else
				LCD_Display[3][19] = io_off;
		
			break;
			
		case 2:
			if (upd_dpy == 1)
			{
				upd_dpy = 0;
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = FPGA1_IOS[8][i];
					LCD_Display[1][i] = FPGA1_IOS[9][i];
				    LCD_Display[2][i] = FPGA1_IOS[10][i];
					LCD_Display[3][i] = FPGA1_IOS[11][i];
				}
			}

			//	Byte# 1	DLB_L		DLM_L		DLT_L		RLM_L		LPS_L		RGS_L		GS_L		ACC_L

			//  " DLB*|DLM*|DLT*|RLM*",
  

			if (i1spi_in.ST.buff0.B.DLB == 1)	// DLB
				LCD_Display[0][4] = io_on;
			else
				LCD_Display[0][4] = io_off;
			
			if (i1spi_in.ST.buff0.B.DLM == 1)	// DLM
				LCD_Display[0][9] = io_on;
			else
				LCD_Display[0][9] = io_off;
				
			if (i1spi_in.ST.buff0.B.DLT == 1)	// DLT
				LCD_Display[0][14] = io_on;
			else
				LCD_Display[0][14] = io_off;
			
			if (i1spi_in.ST.buff0.B.RLM == 1)	// RLM
				LCD_Display[0][19] = io_on;
			else
				LCD_Display[0][19] = io_off;
				

			//	Byte# 1	DLB_L		DLM_L		DLT_L		RLM_L		LPS_L		RGS_L		GS_L		ACC_L

			//  " LPS*|RGS*| GS*|    ",
				
			if (i1spi_in.ST.buff0.B.LPS == 1)	// LPS
				LCD_Display[1][4] = io_on;
			else
				LCD_Display[1][4] = io_off;
				
			if (i1spi_in.ST.buff0.B.RGS == 1)	// RGS
				LCD_Display[1][9] = io_on;
			else
				LCD_Display[1][9] = io_off;
				
			if (i1spi_in.ST.buff0.B.GS == 1)	// GS
				LCD_Display[1][14] = io_on;
			else
				LCD_Display[1][14] = io_off;
				

			// BLANK
				
			//	Byte# 3	CEN_CPU		MC_CPU		STE_CPU		DNR_CPU		UPR_CPU		UPF_CPU		DNF_CPU		UL_CPU
			//	Byte# 4	DZ_CPU		DL_CPU		DZ_PAL		SPD_150_CPU	SPD_75_CPU	SEL_OK_CPU	RUN_CPU		PSDF_LP

			//  "CENc*|ULc*|DZc*|DLc*",
				
			if (i1spi_in.ST.buff1.B.CEN_CPU == 1)	// CEN CPU
				LCD_Display[2][4] = io_on;
			else
				LCD_Display[2][4] = io_off;
				
			if (i1spi_in.ST.buff1.B.UL_CPU == 1)	// UL CPU
				LCD_Display[2][9] = io_on;
			else
				LCD_Display[2][9] = io_off;
				
			if (i1spi_in.ST.buff2.B.DZ_CPU == 1)	// DZ CPU
				LCD_Display[2][14] = io_on;
			else
				LCD_Display[2][14] = io_off;
				
			if (i1spi_in.ST.buff2.B.DL_CPU == 1)	// DL CPU
				LCD_Display[2][19] = io_on;
			else
				LCD_Display[2][19] = io_off;
				
				
			//	Byte# 4	DZ_CPU		DL_CPU		DZ_PAL		SPD_150_CPU	SPD_75_CPU	SEL_OK_CPU	RUN_CPU		PSDF_LP
			//	Byte# 7	DZ_NTS		SEL_OK_NTS	SPD_150NTS	SPD_75_NTS	NTS_FLT		REAR_DOOR	TEST_MODE	"0" Always
			//  "150c*|75c*|DZp*|DZn*",
				
			if (i1spi_in.ST.buff2.B.SPD_150_CPU == 1)	// SPEED 150 CPU
				LCD_Display[3][4] = io_on;
			else
				LCD_Display[3][4] = io_off;
				
			if (i1spi_in.ST.buff2.B.SPD_75_CPU == 1)	// SPEED 75 CPU
				LCD_Display[3][9] = io_on;
			else
				LCD_Display[3][9] = io_off;
				
			if (i1spi_in.ST.buff2.B.DZ_PAL == 1)	// DZ PAL
				LCD_Display[3][14] = io_on;
			else
				LCD_Display[3][14] = io_off;
				
			if (i1spi_in.ST.buff3.B.DZ_NTS == 1)	// DZ NTS
				LCD_Display[3][19] = io_on;
			else
				LCD_Display[3][19] = io_off;
		
			break;
			
		case 3:
			if (upd_dpy == 1)
			{
				upd_dpy = 0;
				for(i=0; i<=19; i++)
				{
				    LCD_Display[0][i] = FPGA1_IOS[12][i];
					LCD_Display[1][i] = FPGA1_IOS[13][i];
				    LCD_Display[2][i] = FPGA1_IOS[14][i];
					LCD_Display[3][i] = FPGA1_IOS[15][i];
				}
			}


			//	Byte# 6	PMC_LP		PCEN_LP		PRUN_LP		PAL_FLT		DT_NTS		UT_NTS		UN_NTS		DN_NTS
			//	Byte# 7	DZ_NTS		SEL_OK_NTS	SPD_150_NTS	SPD_75_NTS	NTS_FLT		REAR_DOOR	TEST_MODE	"0" Always
			//  "150n*|75n*|UNn*|UTn*",

			if (i1spi_in.ST.buff3.B.SPD_150_NTS == 1)	// SPEED 150 NTS
				LCD_Display[0][4] = io_on;
			else
				LCD_Display[0][4] = io_off;
			
			if (i1spi_in.ST.buff3.B.SPD_75_NTS == 1)	// SPEED 75 NTS
				LCD_Display[0][9] = io_on;
			else
				LCD_Display[0][9] = io_off;
				
			if (i1spi_in.ST.buff3.B.UN_NTS == 1)	// UN NTS
				LCD_Display[0][14] = io_on;
			else
				LCD_Display[0][14] = io_off;
			
			if (i1spi_in.ST.buff3.B.UT_NTS == 1)	// UT NTS
				LCD_Display[0][19] = io_on;
			else
				LCD_Display[0][19] = io_off;
				
			//	Byte# 6	PMC_LP		PCEN_LP		PRUN_LP		PAL_FLT		DT_NTS		UT_NTS		UN_NTS		DN_NTS
			//	Byte# 7	DZ_NTS		SEL_OK_NTS	SPD_150NTS	SPD_75_NTS	NTS_FLT		REAR_DOOR	TEST_MODE	"0" Always
			//  "FLTn*|OKn*|DNn*|DTn*",
				
			if (i1spi_in.ST.buff3.B.NTS_FLT == 1)	// NTS FAULT
				LCD_Display[1][4] = io_on;
			else
				LCD_Display[1][4] = io_off;
				
			if (i1spi_in.ST.buff3.B.SEL_OK_NTS == 1)	// SEL OK NTS
				LCD_Display[1][9] = io_on;
			else
				LCD_Display[1][9] = io_off;
				
			if (i1spi_in.ST.buff3.B.DN_NTS == 1)	// DN NTS
				LCD_Display[1][14] = io_on;
			else
				LCD_Display[1][14] = io_off;
				
			if (i1spi_in.ST.buff3.B.DT_NTS == 1)	// DT NTS
				LCD_Display[1][19] = io_on;
			else
				LCD_Display[1][19] = io_off;
				
				
			//	Byte# 5	PSUF_LP		PSU_LP		PSD_LP		PDT_LP		PUT_LP		PUN_LP		PDN_LP		PSTE_LP
			//	Byte# 6	PMC_LP		PCEN_LP		PRUN_LP		PAL_FLT		DT_NTS		UT_NTS		UN_NTS		DN_NTS
			//	Byte# 7	DZ_NTS		SEL_OK_NTS	SPD_150NTS	SPD_75_NTS	NTS_FLT		REAR_DOOR	TEST_MODE	"0" Always
			//  "CENp*|RDR*|UNp*|UTp*",
				
			if (i1spi_in.ST.buff3.B.PCEN == 1)	// PCEN
				LCD_Display[2][4] = io_on;
			else
				LCD_Display[2][4] = io_off;
				
			if (i1spi_in.ST.buff3.B.REAR_DOOR == 1)	// REAR DOOR
				LCD_Display[2][9] = io_on;
			else
				LCD_Display[2][9] = io_off;
				
			if (i1spi_in.ST.buff2.B.PUN == 1)	// PUN
				LCD_Display[2][14] = io_on;
			else
				LCD_Display[2][14] = io_off;
				
			if (i1spi_in.ST.buff2.B.PUT == 1)	// PUT
				LCD_Display[2][19] = io_on;
			else
				LCD_Display[2][19] = io_off;
				
				
			//	Byte# 5	PSUF_LP		PSU_LP		PSD_LP		PDT_LP		PUT_LP		PUN_LP		PDN_LP		PSTE_LP
			//	Byte# 6	PMC_LP		PCEN_LP		PRUN_LP		PAL_FLT		DT_NTS		UT_NTS		UN_NTS		DN_NTS
			//	Byte# 7	DZ_NTS		SEL_OK_NTS	SPD_150NTS	SPD_75_NTS	NTS_FLT		REAR_DOOR	TEST_MODE	"0" Always
			//  "FLTp*|TST*|DNp*|DTp*",
				
			if (i1spi_in.ST.buff3.B.PAL_FLT == 1)	// PAL_FLT
				LCD_Display[3][4] = io_on;
			else
				LCD_Display[3][4] = io_off;
				
			if (i1spi_in.ST.buff3.B.TEST_MODE == 1)	// TEST MODE
				LCD_Display[3][9] = io_on;
			else
				LCD_Display[3][9] = io_off;
				
			if (i1spi_in.ST.buff2.B.PDN == 1)	// PDN
				LCD_Display[3][14] = io_on;
			else
				LCD_Display[3][14] = io_off;
				
			if (i1spi_in.ST.buff2.B.PDT == 1)	// PDT
				LCD_Display[3][19] = io_on;
			else
				LCD_Display[3][19] = io_off;
			
			break;
		
			
		default:
			break;
	}
	return(lcd_menu);
}

/*

Revision History:

4/6/15 v7.1.29 mhd		1. Moved IO dislay routines to this file and added routines to relocate IOs.
8/16/17 V8.0.8 mhd		1. Corrected hall call board label and index for individual serial hall call boards.

*/