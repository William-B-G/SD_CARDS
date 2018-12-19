// Load/Store Program Include File

extern int16 fvarstoSD(int16 cmd, int16 force_write);
extern uint16 Load_Store_Program(uint16 cmd);
extern void Set_Device_Name(char * dev_name, int16 code, uint8 dev);
#if (Simulator == 1)
extern uint8 writeTrafficLogToSD(void);
#endif
