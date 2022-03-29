/******************************************************************************
*  @brief      Userspace application to test the Device driver
*
*  @author     Phonglt9 - phongxxxxx@gmail.com
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio_ext.h>

#define CDEV_PATH "/dev/m_device"

int fd, option;
char write_buf[1024];
char read_buf[1024];

void printMenu()
{

    printf("****Please Enter the Option******\n");
    printf("        1. Write                 \n");
    printf("        2. Read                  \n");
    printf("        3. Exit                  \n");
    printf("*********************************\n");
    printf(">>> ");
}

int main()
{   
    printf("*********************************\n");
    printf("*******Linux From Scratch*******\n\n");

    fd = open(CDEV_PATH, O_RDWR);
    if (fd < 0) {
        printf("Cannot open device file: %s...\n", CDEV_PATH);
        return -1;
    }

    while(1) {
        printMenu();

        scanf("%d",&option); 
        switch (option) {
            case 1:
                printf("Enter the string to write into driver: ");
                scanf("  %[^\t\n]s", write_buf);
                printf("Data Writing ... ");
                write(fd, write_buf, strlen(write_buf)+1);
                printf("Done!\n\n\n");
                break;

            case 2:
                printf("Data Reading ... ");
                read(fd, read_buf, 1024);                
                printf("Done!\n");
                printf("Data: %s\n\n\n", read_buf);
                break;

            case 3:
                close(fd);
                exit(1);               
                break;

            default:
                printf("Enter Valid option = %c\n",option);
                break;
        }
    }
    
    close(fd);

    return 0;
}