#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/fs.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <math.h>
#include "main.h"

#define PI 3.141592
static const int neutral_coeff_csc[5][3] = {
	{255,   0,      0   },  /* R Component */
	{0,     255,    0   },  /* G Component */
	{0,     0,      255 },  /* B Component */
	{0,     0,      0   },	/* Luminance ( added to the single components ) */
	{2,     2,      2   },	/* Should be 2 , can be from -1 to 2 */
};
static const int nored_coeff_csc[5][3] = {
	{0,     0,      0   },  /* R Component */
	{0,     255,    0   },  /* G Component */
	{0,     0,      255 },  /* B Component */
	{0,     0,      0   },	/* Luminance ( added to the single components ) */
	{2,     2,      2   },	/* Should be 2 , can be from -1 to 2 */
};
static const int nogreen_coeff_csc[5][3] = {
	{255,   0,      0   },  /* R Component */
	{0,     0,    0   },  /* G Component */
	{0,     0,      255 },  /* B Component */
	{0,     0,      0   },	/* Luminance ( added to the single components ) */
	{2,     2,      2   },	/* Should be 2 , can be from -1 to 2 */
};
static const int noblue_coeff_csc[5][3] = {
	{255,   0,      0   },  /* R Component */
	{0,     255,    0   },  /* G Component */
	{0,     0,      0   },  /* B Component */
	{0,     0,      0   },	/* Luminance ( added to the single components ) */
	{2,     2,      2   },	/* Should be 2 , can be from -1 to 2 */
};

int file_matrix[5][3];

void fb_set_std_table(int fd_fb,int index)
{
struct mxcfb_csc_matrix csc_matrix;
int i,j,ret;

	memset(&csc_matrix,0,sizeof(csc_matrix));
	switch ( index )
	{
	    case 0 : memcpy(csc_matrix.param,neutral_coeff_csc,sizeof(neutral_coeff_csc)); break;
	    case 1 : memcpy(csc_matrix.param,nored_coeff_csc,sizeof(nored_coeff_csc)); break;
	    case 2 : memcpy(csc_matrix.param,nogreen_coeff_csc,sizeof(nogreen_coeff_csc)); break;
	    case 3 : memcpy(csc_matrix.param,noblue_coeff_csc,sizeof(noblue_coeff_csc)); break;
	    case 4 :
                for(i = 0;i < 3;i++)
                {
                    for(j = 0;j < 3;j++)
                    {
                        csc_matrix.param[i][j] = file_matrix[i][j] & 0x3FF;
                    }
                }
                for(i = 0;i < 3;i++)
                {
                    csc_matrix.param[3][i] = file_matrix[3][i] & 0x3FFF;
                    csc_matrix.param[4][i] = file_matrix[4][i];
                }
                break;
	    default : memcpy(csc_matrix.param,neutral_coeff_csc,sizeof(neutral_coeff_csc)); break;
	}
	ret = ioctl(fd_fb, MXCFB_CSC_UPDATE, &csc_matrix);
	printf("ret = %d\n",ret);
}


