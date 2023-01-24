#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>



#define WR _IOW('a','a',int32_t)
#define RD _IOR('a','b',int32_t)

int main()
{
    int fd;
    int32_t value,number;

    printf("*********************************************\n");
    printf("*********************IOCTL by Keerthana************************\n");
    printf("Opening driver\n");
    fd = open("/dev/driver2_device",O_RDWR);

    if(fd<0)
    {
        printf("Cannot open devicefile   \n");
        return 0;
    }
    printf("Enter the value to set/get  :  0/1-");
    scanf(" %d",&number);
    printf("Writing to driver\n");
    ioctl(fd,WR,(char*)&number);
    printf("Reading from driver\n");
    ioctl(fd,RD,(char*)&value);
    printf("Value is %d\n",value);
    printf("Closing Driver\n");
    close(fd);
    return 0;
}
