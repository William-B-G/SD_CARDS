

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


extern unsigned char SEB_IO[NumOfSEB];
extern unsigned char SET_SEB_IO[NumOfSEB];

extern void InitSEB(void);

extern int UpdateAllSEB(void);
extern void FireOutPut(unsigned char Brd,unsigned char IONum);
extern void ClearOutPut(unsigned char Brd,unsigned char IONum);

extern void ReadAllSEB(void);
extern unsigned char ReadSEB(unsigned char Brd,unsigned char IONum);
