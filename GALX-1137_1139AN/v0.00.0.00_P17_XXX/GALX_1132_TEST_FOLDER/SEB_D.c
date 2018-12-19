// SEB_D.c

void UpdateAllSEB(void);
void FireOutPut(unsigned char Brd,unsigned char IONum);
void ClearOutPut(unsigned char Brd,unsigned char IONum);
void ReadAllSEB(void);
unsigned char ReadSEB(unsigned char Brd,unsigned char IONum);

typedef union {
    unsigned short int word;
    unsigned char byte[2];
} COMMWORD_TYPE;

typedef union {
    unsigned int dw;
    unsigned char byte[4];
} COMMDWORD_TYPE;

struct SebTag {
    unsigned char IN[3];
    unsigned char SET_OUT[3];
    unsigned char OUT[3];
    unsigned char OUT_ACK[3];
    unsigned char IN_ACK;
    unsigned char ADDR;
    unsigned char MsgReq;
    unsigned char io_Label_ix;
    unsigned char Online;
    unsigned char RxCnt;
    unsigned char Version;
    unsigned char Revision;
    unsigned char TxReSendCnt;
};

struct SebTag SEB;
COMMWORD_TYPE LabelCheckSum;

#define SEB2 1

#define BRD 0
#define IO  1

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

unsigned char BIT[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

//----------------------------------------------------------
//SEB IO

enum SEB_LIST {
    SLi0,		//0
    SLi1,		//1
    SLi2,		//2
    SLi3,		//3
    SLi4,		//4
    SLi5,		//5
    SLi6,		//6
    SLi7,		//7
    SLi8,		//8
    SLi9,		//9
    SLi10,		//10
    SLi11,		//11
    NumOfSEB	//12
};

unsigned char SEB_IO_MAP[NumOfSEB][2]= {
    {SEB2,0},       //0		SEB2 IO-1
    {SEB2,1},       //1 	SEB2 IO-2
    {SEB2,2},       //2 	SEB2 IO-3
    {SEB2,3},       //3 	SEB2 IO-4
    {SEB2,4},       //4 	SEB2 IO-5
    {SEB2,5},       //5 	SEB2 IO-6
    {SEB2,6},       //6 	SEB2 IO-7
    {SEB2,7},       //7 	SEB2 IO-8
    {SEB2,8},       //8 	SEB2 IO-9
    {SEB2,9},       //9 	SEB2 IO-10
    {SEB2,10},      //10 	SEB2 IO-11
    {SEB2,11},      //11	SEB2 IO-12
};

unsigned char SEB_IO[NumOfSEB];
unsigned char SET_SEB_IO[NumOfSEB];
unsigned char SET_SEB_IO_LAST[NumOfSEB];

//----------------------------------------------------------
//Update All SEB

void UpdateAllSEB(void)
{
    enum SEB_LIST i;

    for(i=SLi0; i<NumOfSEB; i++) {
        if(SET_SEB_IO[i]!= SET_SEB_IO_LAST[i]) {
            SET_SEB_IO_LAST[i] = SET_SEB_IO[i];
            if(SET_SEB_IO[i] == 1)
                FireOutPut(SEB_IO_MAP[i][BRD],SEB_IO_MAP[i][IO]);
            else
                ClearOutPut(SEB_IO_MAP[i][BRD],SEB_IO_MAP[i][IO]);
        }
    }
}

//----------------------------------------------------------
//Fire Output

void FireOutPut(unsigned char Brd,unsigned char IONum)
{
    int Byte = (IONum + 12)/8;
    int Bit = (IONum + 12)%8;
//-	SEB[Brd].SET_OUT[Byte] |= BIT[Bit];
    SEB.SET_OUT[Byte] |= BIT[Bit];
}

//----------------------------------------------------------
//Clear Output

void ClearOutPut(unsigned char Brd,unsigned char IONum)
{
    int Byte = (IONum + 12)/8;
    int Bit = (IONum + 12)%8;
//-	SEB[Brd].SET_OUT[Byte] &= ~BIT[Bit];
    SEB.SET_OUT[Byte] &= ~BIT[Bit];
}

//----------------------------------------------------------
//Read ALL SEB

void ReadAllSEB(void)
{
    int i;
    for(i= SLi0; i<(int)NumOfSEB; i++)
        SEB_IO[i]= ReadSEB(SEB_IO_MAP[i][0],SEB_IO_MAP[i][1]);
}

//----------------------------------------------------------
//Read SEB

unsigned char ReadSEB(unsigned char Brd,unsigned char IONum)
{
    int Byte = (int)(IONum/8);
    int Bit = (int)(IONum%8);

//-	if((SEB[Brd].IN[Byte] & BIT[Bit]) == BIT[Bit])
    if((SEB.IN[Byte] & BIT[Bit]) == BIT[Bit])
        return 1;
    else
        return 0;
}




















