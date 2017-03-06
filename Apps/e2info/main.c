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
#include <ctype.h>

#define I2CDEV                  "/dev/i2c-2"
#define DEV_FAIL_ORETVAL        -1
#define DEV_FAIL_IRETVAL        -2
#define VERSION 	            "1.0.0"

#define AT24C16_I2CADDR        0xa0
#define AT24C16_PAGESIZE       16

#define MAC_PAGE        0x00
#define PN_PAGE         0x10
#define SERIAL_PAGE     0x20
#define LOT_PAGE        0x30

unsigned char   buf[32];
unsigned char   macbuf[32];
unsigned char   pnbuf[32];
unsigned char   serialbuf[32];
unsigned char   lotbuf[32];

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


void usage(void)
{
const static char *do_usage =
"e2info " VERSION ", the configuration 24C16 EEPROM reader/writer\n"
"Copyright (c) 2017 by Novasis / Novasom Industries - All rights reserved.\n"
"Usage:\n"
"\te2info [-M aa:bb:cc:dd:ee:ff] -- set mac address, fixed format\n"
"\te2info [-m]                   -- get mac address\n"
"\te2info [-P 0123456789abcdef]  -- set part number\n"
"\te2info [-p]                   -- get part number\n"
"\te2info [-S 0123456789abcdef]  -- set serial\n"
"\te2info [-s]                   -- get serial\n"
"\te2info [-L 0123456789abcdef]  -- set lot\n"
"\te2info [-l]                   -- get lot\n"
"\te2info [-?]                   -- shows this help\n"
"\tAll fields are 16 chars max\n"
;
    printf("%s",do_usage);
    exit(0);
}

int isMAC(char *s)
{
int i,k;
    for(i = 0; i < 16; i++)
    {
        if(i % 3 != 2 && !isxdigit(s[i]))
            return 0;
        if(i % 3 == 2 && s[i] != ':')
            return 0;
    }
    i=0 , k=0;
    macbuf[i++] = 'M';
    macbuf[i++] = 'a';
    macbuf[i++] = 'c';
    macbuf[i++] = '=';
    macbuf[i++] = s[k++];
    macbuf[i++] = s[k++];
    k++;
    macbuf[i++] = s[k++];
    macbuf[i++] = s[k++];
    k++;
    macbuf[i++] = s[k++];
    macbuf[i++] = s[k++];
    k++;
    macbuf[i++] = s[k++];
    macbuf[i++] = s[k++];
    k++;
    macbuf[i++] = s[k++];
    macbuf[i++] = s[k++];
    k++;
    macbuf[i++] = s[k++];
    macbuf[i++] = s[k++];
    return 1;
}

void dump_e2prom(int file, int address)
{
unsigned char    *stra;

    read_at24c16(file,address,buf,AT24C16_PAGESIZE);
    stra = &buf[4];
    switch(address)
    {
        case MAC_PAGE:
                if ( ( buf[0] == 'M')  && ( buf[1] == 'a' ) && ( buf[2] == 'c' )&& ( buf[3] == '=' ))
                {
                    printf("MAC : %s\n",stra);
                }
                else
                    printf("Invalid MAC address, maybe the e2prom is blank\n");
                break;
        case PN_PAGE:
                /* Note : the P/N is saved with no headers, to use the full 16 bytes size */
                printf("P/N : %s\n",buf);
                break;
        case SERIAL_PAGE:
                if ( ( buf[0] == 'S')  && ( buf[1] == '/' ) && ( buf[2] == 'N' )&& ( buf[3] == '=' ))
                    printf("S/N : %s\n",stra);
                else
                    printf("Invalid S/N, maybe the e2prom is blank\n");
                break;
        case LOT_PAGE:
                if ( ( buf[0] == 'L')  && ( buf[1] == 'O' ) && ( buf[2] == 'T' )&& ( buf[3] == '=' ))
                    printf("LOT : %s\n",stra);
                else
                    printf("Invalid LOT, maybe the e2prom is blank\n");
                break;
    }
}

int main(int argc, char** argv)
{
int file,ret;
int op_macw=0, op_serialw=0, op_pnw=0, op_lotw=0 , op_dump = 0;
int op_macr=0, op_serialr=0, op_pnr=0, op_lotr=0;

    bzero(buf,sizeof(buf));
    if ( argc == 1 )
        op_dump = 1;
    else
    {
        while((ret = getopt(argc, argv, "M:mP:pS:sL:l?")) != -1)
        {
            switch(ret)
            {
                case 'M':
                    if ( strlen(optarg) != 17)
                    {
                        printf("Malformed MAC\n");
                        usage();
                    }
                    if ( isMAC(optarg))
                        op_macw = 1;
                    else
                    {
                        printf("Malformed MAC\n");
                        usage();
                    }
                    break;
                case 'P':
                    if ( strlen(optarg) <=16 )
                    {
                        sprintf(( char *)pnbuf,"%s",optarg);
                        op_pnw = 1;
                    }
                    else
                        usage();
                    break;
                case 'S':
                    if ( strlen(optarg) <=12 )
                    {
                        sprintf(( char *)serialbuf,"S/N=%s",optarg);
                        op_serialw = 1;
                    }
                    else
                        usage();
                    break;
                case 'L':
                    if ( strlen(optarg) <=12 )
                    {
                        sprintf(( char *)lotbuf,"LOT=%s",optarg);
                        op_lotw = 1;
                    }
                    else
                        usage();
                    break;
                case 'm':
                    op_macr = 1;
                    break;
                case 'p':
                    op_pnr = 1;
                    break;
                case 's':
                    op_serialr = 1;
                    break;
                case 'l':
                    op_lotr = 1;
                    break;
                case '?':
                    usage();
                    break;
                default :
                    usage();
                    break;
            }
        }
    }
    file = open_i2cdev();
    if ( file == 0)
            return -1;

    if ( op_macw )
    {
        write_at24c16(file,MAC_PAGE,macbuf,AT24C16_PAGESIZE);
        dump_e2prom(file,MAC_PAGE);
    }
    if ( op_pnw )
    {
        write_at24c16(file,PN_PAGE,pnbuf,AT24C16_PAGESIZE);
        dump_e2prom(file,PN_PAGE);
    }
    if ( op_serialw )
    {
        write_at24c16(file,SERIAL_PAGE,serialbuf,AT24C16_PAGESIZE);
        dump_e2prom(file,SERIAL_PAGE);
    }
    if ( op_lotw )
    {
        write_at24c16(file,LOT_PAGE,lotbuf,AT24C16_PAGESIZE);
        dump_e2prom(file,LOT_PAGE);
    }

    if ( op_macr )
    {
        dump_e2prom(file,MAC_PAGE);
    }
    if ( op_pnr )
    {
        dump_e2prom(file,PN_PAGE);
    }
    if ( op_serialr )
    {
        dump_e2prom(file,SERIAL_PAGE);
    }
    if ( op_lotr )
    {
        dump_e2prom(file,LOT_PAGE);
    }


    if ( op_dump )
    {
        dump_e2prom(file,MAC_PAGE);
        dump_e2prom(file,PN_PAGE);
        dump_e2prom(file,SERIAL_PAGE);
        dump_e2prom(file,LOT_PAGE);
    }

    close_i2cdev(file);
    return 0;
}
