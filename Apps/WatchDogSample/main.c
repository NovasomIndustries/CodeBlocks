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
FILE *wfd;
int fd,ret;
unsigned int    reason,counter;

	if((fd = open("/dev/watchdog", O_RDWR, 0)) < 0)
	{
		printf("Can not open /dev/watchdog\n");
		return -1;
	}
    ret = ioctl(fd, WDIOC_GETBOOTSTATUS, &reason);
    printf("WDIOC_GETBOOTSTATUS reason = 0x%08x , ret = %d\n",reason,ret);

    if ( reason != 0 )
    {
        wfd=fopen("/tmp/wdog_counter","r");
        if ( wfd !=NULL)
        {
            fscanf(wfd,"%d",&counter);
            printf("read : counter = %d\n",counter);
            fclose(wfd);
            counter++;
            wfd=fopen("/tmp/wdog_counter","w");
            if ( wfd !=NULL)
            {
                fprintf(wfd,"%d",counter);
                printf("write : counter = %d\n",counter);
                fclose(wfd);
                system("/tmp/www/store_wdog_counter");
            }
        }
        system("echo 1 > /tmp/boot_reason");
    }
    else
    {
        wfd=fopen("/tmp/reboot_counter","r");
        if ( wfd !=NULL)
        {
            fscanf(wfd,"REBOOT_COUNTER=%d",&counter);
            printf("read : counter = %d\n",counter);
            fclose(wfd);
            counter++;
            wfd=fopen("/tmp/reboot_counter","w");
            if ( wfd !=NULL)
            {
                fprintf(wfd,"REBOOT_COUNTER=%d",counter);
                printf("write : counter = %d\n",counter);
                fclose(wfd);
                system("/tmp/www/store_reboot_counter");
            }
        }
    }

    while(1)
    {
        ret = ioctl(fd, WDIOC_KEEPALIVE, NULL);
        sleep(1);
    }
    return 0;
}
