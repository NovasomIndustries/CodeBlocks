#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#define I2CDEV                  "/dev/i2c-2"
#define DEV_FAIL_ORETVAL        -1
#define DEV_FAIL_IRETVAL        -2

#define AT24C16_I2CADDR        0xa0
unsigned char   buf[32];

int open_i2cdev(void)
{
int file;

    if ((file = open(I2CDEV,O_RDWR)) < 0)
    {
        printf("Failed to open the i2c bus.");
        return 0;
    }
    if (ioctl(file,I2C_SLAVE,AT24C16_I2CADDR>>1) < 0)
    {
        printf("Failed to acquire bus access and/or talk to 24c16@%x.\n",AT24C16_I2CADDR);
        return 0;
    }
    return file;
}

void close_i2cdev(int file)
{
    close(file);
}

unsigned char write_at24c16(int file,unsigned char address,unsigned char *buffer_in , unsigned char count)
{
int res,i;
unsigned char   lbuf[32];

    lbuf[0] = address;   /* Set address */
    for(i=0;i<count;i++)
        lbuf[i+1] = buffer_in[i];     /* Store value */
    if ((res=write(file,lbuf,count+1)) != count+1)
    {
        printf("%s : i2c bus write address on write failed %d\n",__FUNCTION__,res);
        return -1;
    }
    usleep(10000);
    return 0;
}

unsigned char read_at24c16(int file,unsigned char address,unsigned char *buffer_out,unsigned char count)
{
int res;
    buf[0] = address;   /* Set address */
    if ((res=write(file,buf,1)) != 1)
    {
        printf("%s : i2c bus write address 0x%x on read failed %d\n",__FUNCTION__,address,res);
        return -1;
    }
    if (read(file,buffer_out,count) != count) /* Read @ address */
    {
        printf("%s : i2c bus read failed\n",__FUNCTION__);
        return -1;
    }
    return 0;
}

int main()
{
int file,i;

    file = open_i2cdev();
    if ( file == 0)
            return -1;
    for(i=0;i<16;i++)
        buf[i] = i;

    write_at24c16(file,0,buf,16);
    for(i=0;i<16;i++)
        buf[i] = 0;
    read_at24c16(file,0,buf,16);

    for(i=0;i<16;i++)
        printf("%d ",buf[i]);
    printf("\n");

    close_i2cdev(file);
    return 0;
}
