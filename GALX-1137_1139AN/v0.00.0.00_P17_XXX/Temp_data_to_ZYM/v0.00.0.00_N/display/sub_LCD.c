
#include "global.h"
#include "GALX_1132_TEST.h"


void Sprint_Lcd_SignalChar(int Row,int Col,unsigned char Ch);
void Sprint_Sub_String(int Row,int Col,char *S,int size);





   






//--------------------------------------------------------
//Load Signal Char to LCD  Buffer                  
void Sprint_Lcd_SignalChar(int Row,int Col,unsigned char Ch)
{
	if((Row < 4) && (Col <20))
		LCD_Display[Row][Col] = Ch;		
}


//--------------------------------------------------------
//Load sub string to LCD                   
void Sprint_Sub_String(int Row,int Col,char *S,int size)
{
	int i;
	
	for(i = 0;i<size && S[i]!= NULL;i++)
		LCD_Display[Row][Col+i] = S[i];			
}