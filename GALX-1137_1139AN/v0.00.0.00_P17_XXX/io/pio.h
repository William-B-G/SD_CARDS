extern void clroutp(int16 address, uint8 bitno, uint8 arrno);
extern void clrinp(int16 address, uint8 bitno, uint8 arrno);
extern uint8 rdinp(int16 address, uint8 bitno, uint8 arrno);
extern uint8 rdoutp(int16 address, uint8 bitno, uint8 arrno);
extern void setoutp(int16 address, uint8 bitno, uint8 arrno);
extern void setinp(int16 address, uint8 bitno, uint8 arrno);
extern uint8 g_rdoutp(uint16 car, int16 address, uint8 bitno, uint8 arrno);
extern uint8 g_rdinp(uint16 car,int16 address,uint8 bitno,uint8 arrno);
extern void read_ports(void);
