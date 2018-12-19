// Diagnostics.h

#define MAX_0047_INDEX 5

extern int16 Diagnostic_Control(int16 DiagIO_Map_index );
extern int16 Set_Diag_output(int16 DiagIO_Map_index, int16 state);
extern void FirePreReqSignal(int IoIndex);
extern int16 Toggle_4_0047(int16 channel);
extern int16 Toggle_4_0047_input(int16 channel);
extern int16 Read_the_Output(int16 DiagIO_Map_index);
