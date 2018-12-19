#define sx_rbufsize 300
#define sx_tbufsize 100

extern int16 sx_baud_tbl[];
extern int16 sx2_baud_tbl[];
extern unsigned char sx_Tranbuf_Empty[]; // Com 1-2 Transmiter buffer is empty if 1 has chars if 0
extern unsigned char sx_Tranbuf_Full[];	// Com Transmit buffer is full if 1 and 0 if not
extern unsigned char sx_tranbuf[2][sx_tbufsize+1];  // com transmit buffer
extern unsigned char sx_recbuf[2][sx_rbufsize+1];   // com receive buffer
extern int16 sx_com_baud[2];	// default baud rate for sx comm

extern void sx_initcom(void);
extern void sx_sendc( int16 comnmb, int16 letter);
extern void sx_enable_tx(int16 comnmb);
extern void init_sx_com_port_1(void);
extern void init_sx_com_port_2(void);
extern void sx_com_1_int(void);
extern void sx_com_2_int(void);
extern void se1_tx_dma_start(int16 len);
extern void se1_dma_trm_finished_int(void);
extern void se2_tx_dma_start(int16 len);
extern void se2_dma_trm_finished_int(void);
extern int16 sx_isrxempty(int16 comnmb);
extern int16 sx_istxcomplete(int16 comnmb);
extern int16 sx_asigetc( int16 comnmb);
extern int16 sx_istxDMAcomplete(int16 comnmb);

