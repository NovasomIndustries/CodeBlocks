#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#define I2CDEV                  "/dev/i2c-2"
#define DEV_FAIL_ORETVAL        -1
#define DEV_FAIL_IRETVAL        -2

#define AT24C16_I2CADDR        0x50

int open_i2cdev(void)
{
int file;

    if ((file = open(I2CDEV,O_RDWR)) < 0)
    {
        printf("Failed to open the i2c bus.");
        return 0;
    }
    return file;
}

void close_i2cdev(int file)
{
    close(file);
}

int set_at24c16_device(int file)
{
    if (ioctl(file,I2C_SLAVE,AT24C16_I2CADDR>>1) < 0)
    {
        printf("Failed to acquire bus access and/or talk to 24c16.\n");
        return 1;
    }
    return 0;
}

int main()
{
int file;
    file = open_i2cdev();
    if ( file != 0)
    {
        if ( set_at24c16_device(file) != 0 )
            return -1;

    }
    close_i2cdev(file);
    return 0;
}
