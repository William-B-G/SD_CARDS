
struct SEL_can_type {
	int32	pos_cnt_A;
	int32	pos_cnt_B;
	int16	vel;
	uint8 	vel_dir;			// velocity direction
 	int8 	clip_offset;
    union {
    	uint8 R;
        struct {
            uint8 NF_ALIGN:2;		// 0=Centered, 1=slightly off, 1=modertly off, 3=at the limit
            uint8 FAR:1;			// 0 = near, 1 = far
            uint8 LR_ALIGN:2;		// 0=Centered, 1=slightly off, 1=modertly off, 3=at the limit
            uint8 RIGHT:1;			// 0 = left, 1 = right
            uint8 CLIP_B:1;			// ClipB is detected (offset is for B)
            uint8 CLIP_A:1;			// CiipA is detected (offset is for A) (if both A and B then offset is for A)
       } B;
    } Status;

    union {
        uint8 R;
        struct {
            uint8 CONTRAST:2;		// 00=OK, 01=Service Recommended, 10=Warning (go to target floor and shutdown), 11=stopped
            uint8 POS_B_EX:1;		// Position B extrapolated
            uint8 POS_A_EX:1;		// Position A extrapolated
            uint8 VEL_INVALID:1;	// Velocity Validation Error
            uint8 POS_INVALID:1;	// Position Validation Error
            uint8 COMM:1;			// Communications Error
            uint8 INTERNAL:1;		// Internal Error
        } B;
    } Error_Warning;
    uint8	ver;					// protocol version 1 = initial, 2= latested with two pos counts
	bool 	online;
	uint8	online_cnt;
	int16	tx_cnt;
	int16	rx_cnt;
}; 

extern struct SEL_can_type sel_can;

extern int16 sel_can_tx_err_cnt;
extern int16 sel_can_rx_err_cnt;

extern int16 aps_sel_can_error;		// ced aps can error
extern int16 aps_sel_can_online;		// ced aps can online
extern int16 aps_sel_fault;
extern int32 sel_clip_cnt;
extern int16 sel_clip;

extern void Sel_Can_Control (void);
extern void sel_init_can_bus (int16 baud_change);
