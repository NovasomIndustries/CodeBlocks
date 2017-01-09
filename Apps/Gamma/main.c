#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <linux/fb.h>

/****************************************************/
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
#define MXCFB_CSC_UPDATE        _IOW('F', 0x2D, struct mxcfb_csc_matrix)


/*******************************************************/


struct mxcfb_gamma {
	int enable;
	int constk[16];
	int slopek[16];
};


int fd_fb0 = 0;


void get_gamma_coeff(float gamma, int constk[], int slopek[])
{
unsigned int tk[17] = {0, 2, 4, 8, 16, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 383};

unsigned int k;
unsigned int rk[16], yk[17];
float center, width, start, height, t, scurve[17], gammacurve[17];

        center = 32; /* nominal 32 - must be in {1,380} where is s-curve centered*/
        width = 32; /* nominal 32 - must be in {1,380} how narrow is s-curve */
        for(k=0;k<17;k++) {
                t = (float)tk[k];
                scurve[k] = (256.F/3.14159F)*((float) atan((t-center)/width));
                gammacurve[k] = 255.F* ((float) pow(t/384.F,1.0/gamma));
        }

        start = scurve[0];
        height = scurve[16] - start;
        for(k=0;k<17;k++) {
                scurve[k] = (256.F/height)*(scurve[k]-start);
                yk[k] = (int)(scurve[k] * gammacurve[k]/256.F + 0.5) << 1;
        }

        for(k=0;k<16;k++)
                rk[k] = yk[k+1] - yk[k];


        for(k=0;k<5;k++) {
                constk[k] = yk[k] - rk[k];
                slopek[k] = rk[k] << (4-k);

        }
        for(k=5;k<16;k++) {
                constk[k] = yk[k];
                slopek[k] = rk[k] >> 1;
        }

	for(k=0;k<16;k++) {
		constk[k] /= 2;
		constk[k] &= 0x1ff;

	}


}

int gammaFunc(int fd_fb,float gammainput)
{
struct mxcfb_gamma gamma;
int k,retval=0;
char output[50];

    printf("\nGamma %f\n", gammainput);

    get_gamma_coeff(gammainput, gamma.constk, gamma.slopek);

    printf("constk:");
    for(k=0;k<16;k++)
        printf("%04x ", gamma.constk[k]);
    printf("\nslopek:");
    for(k=0;k<16;k++)
        printf("%04x ",  gamma.slopek[k]);
    printf("\n");

    gamma.enable = 1;
    retval = ioctl(fd_fb, MXCFB_SET_GAMMA, &gamma);
    if (retval < 0)
    {
        printf("Ioctl MXCFB_SET_GAMMA fail!\n");
        close(fd_fb);
        retval= -1;
    }
    else
    {
        memset(output,0,sizeof(output));
        snprintf(output,50,"echo %.03f > /tmp/GammaDefaultValue",gammainput);
        system(output);
    }
    return retval;
}

void fb_test_gamma(int fd_fb,char *gammaArg)
{
float gammainput=0.0;

    if(sscanf( gammaArg, "%f", &gammainput)==1)
        gammaFunc(fd_fb,gammainput);
    else
    {
        printf("Usage Gamma value [0.0 - 10.0]\n");
        close(fd_fb);
    }
}

int main(int argc, char **argv)
{
    printf("Opened fb0\n");
    if(argv[1] != 0)
    {
        if ((fd_fb0 = open("/dev/fb0", O_RDWR, 0)) < 0)
        {
            printf("Unable to open /dev/fb0\n");
            return -1;
        }
        fb_test_gamma(fd_fb0,argv[1]);
        close(fd_fb0);
    }
    else
    {
        printf("Usage Gamma value [0.0 - 10.0]\n");
        return -1;
    }
    return 0;
}

