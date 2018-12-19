// Secure Digital Include file

extern uint8 sdhc;		// SD High Capacity = 1
extern uint8 sdvs;		// SD Voltage Status 1=okay
extern uint8 sd_init;		// secure digital init flag

extern void init_sdspi (void);
extern uint8 sd_initialization(void);
extern int16 detect_sd(void);
int16 det_sd_wr_prot(void);
extern void copy_read_data_from_dma_buff(uint8 *addr);
extern uint8 sd_read_block(uint32 block_addr);
extern void copy_write_data_to_dma_buff(uint8 *addr);
extern uint8 sd_write_block(uint32 block_addr);
extern uint8 get_read_block_status(void);
extern uint8 get_write_block_status(void);
extern int32 media_read(uint32 sector, uint8 *buffer, uint32 sector_count);
extern int32 media_write(uint32 sector, uint8 *buffer, uint32 sector_count);
