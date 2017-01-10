#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/fs.h>
#include <linux/watchdog.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
int             fd;
unsigned int    boot_reason;
int timeout = 10;

	if((fd = open("/dev/watchdog", O_RDWR, 0)) < 0)
	{
		printf("Can't open /dev/watchdog\n");
		return -1;
	}
	if ( argc == 2 )
        timeout = atoi(argv[1]);
    ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
    printf("The timeout is set to %d seconds\n", timeout);

    ioctl(fd, WDIOC_GETBOOTSTATUS, &boot_reason);
    if ( boot_reason == 0 )
        printf("Regular boot\n");
    if ( boot_reason == 0x00000020 )
        printf("WD reboot\n");

    while(1)
    {
        ioctl(fd, WDIOC_KEEPALIVE, NULL);
        sleep(5);
    }
    return 0;
}
