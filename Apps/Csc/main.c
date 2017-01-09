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

void usage()
{
	printf("    Usage :\n");
	printf("      imx_csc <0,1,2 or 3, or f <csc file> for file ops> \n");
	printf("      0 is standard\n");
	printf("      1 is no red component\n");
	printf("      2 is no green component\n");
	printf("      3 is no blue component\n");
}

int main(int argc, char *argv[])
{
int fd;
int index = 1;
int i;
FILE *fp;

	if(argc < 2)
	{
		printf("%d invald arguments!\n", argc);
		usage();
		return -1;
	}

	fd = open("/dev/fb0",O_RDWR);
	if((fd = open("/dev/fb0", O_RDWR, 0)) < 0)
	{
		printf("Can not open device\n");
		return -1;
	}
	switch ( *argv[1] )
	{
        case '0' :
        case '1' :
        case '2' :
        case '3' : index = atoi(argv[1]); break;
        case 'f' :
                    index = 4;
                    fp = fopen(argv[2],"r");
                    if(fp == NULL)
                    {
                        printf("CSC file open Error!");
                        close(fd);
                        return -1;
                    }
                    for(i = 0;i < 5;i++)
                    {
                        fscanf(fp,"%d,%d,%d",&file_matrix[i][0],&file_matrix[i][1],&file_matrix[i][2]);
                    }
                    fb_set_std_table(fd, index);
                    fclose(fp);
                    close(fd);
                    return 0;
        default :   printf("invald arguments!\n");
                    usage();
                    return -1;

	}
    fb_set_std_table(fd, index);
	close(fd);
	printf("Done.\n");
	return 0;
}
