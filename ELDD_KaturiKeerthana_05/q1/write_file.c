#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char value[50];

    printf("*********************************************\n");
    printf("*********************Semaphore by Keerthana************************\n");
    printf("Opening driver\n");
    fd = open("/dev/driver1_device2",O_RDWR);

    if(fd<0)
    {
        printf("Cannot open devicefile   \n");
        return 0;
    }
    printf("Enter the value of string\n");
    scanf(" %[^\t\n]s",&value);
    printf("Writing to driver\n");
    write(fd,(char*)&value,sizeof(value));
    printf("Closing Driver\n");
    close(fd);
    return 0;
}
