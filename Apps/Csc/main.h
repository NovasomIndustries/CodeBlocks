
#define FB_SYNC_OE_LOW_ACT      0x80000000
#define FB_SYNC_CLK_LAT_FALL    0x40000000
#define FB_SYNC_DATA_INVERT     0x20000000
#define FB_SYNC_CLK_IDLE_EN     0x10000000
#define FB_SYNC_SHARP_MODE      0x08000000
#define FB_SYNC_SWAP_RGB        0x04000000
#define FB_ACCEL_TRIPLE_FLAG    0x00000000
#define FB_ACCEL_DOUBLE_FLAG    0x00000001

#define MXCFB_WAIT_FOR_VSYNC    _IOW('F', 0x20, u_int32_t)
#define MXCFB_SET_GBL_ALPHA     _IOW('F', 0x21, struct mxcfb_gbl_alpha)
#define MXCFB_SET_CLR_KEY       _IOW('F', 0x22, struct mxcfb_color_key)
#define MXCFB_SET_OVERLAY_POS   _IOWR('F', 0x24, struct mxcfb_pos)
#define MXCFB_GET_FB_IPU_CHAN   _IOR('F', 0x25, u_int32_t)
#define MXCFB_SET_LOC_ALPHA     _IOWR('F', 0x26, struct mxcfb_loc_alpha)
#define MXCFB_SET_LOC_ALP_BUF    _IOW('F', 0x27, unsigned long)
#define MXCFB_SET_GAMMA        _IOW('F', 0x28, struct mxcfb_gamma)
#define MXCFB_GET_FB_IPU_DI     _IOR('F', 0x29, u_int32_t)
#define MXCFB_GET_DIFMT        _IOR('F', 0x2A, u_int32_t)
#define MXCFB_GET_FB_BLANK     _IOR('F', 0x2B, u_int32_t)
#define MXCFB_SET_DIFMT         _IOW('F', 0x2C, u_int32_t)
//#define MXCFB_CSC_UPDATE        _IOW('F', 0x2D, struct mxcfb_csc_matrix)
#define MXCFB_CSC_UPDATE        _IOW('F', 0x2D, struct mxcfb_csc_matrix)
struct mxcfb_csc_matrix {
	int param[5][3];
};

struct mxcfb_gamma {
	int enable;
	int constk[16];
	int slopek[16];
};
struct mxcfb_gamma gammag;
extern int file_matrix[5][3];

extern  int coeff_csc[5][3];
extern  void fb_set_std_table(int fd_fb,int index);
extern  void set_color_index(int index);
extern  void color_temp_func(void);
extern  void multi_color_temp_coeff(void);
