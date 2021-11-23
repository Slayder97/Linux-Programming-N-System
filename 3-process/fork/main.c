#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])   /* Cấp phát stack frame cho hàm main() */
{
    /* code */
    pid_t child_pid;                /* Lưu trong stack frame của main() */
    int counter = 2;                /* Lưu trong frame của main() */

    printf("Gia tri khoi tao cua counter: %d\n", counter);

    child_pid = fork();         
    if (child_pid >= 0) {
        if (0 == child_pid) {       /* Process con */
            printf("\nIm the child process, counter: %d\n", ++counter);
            printf("My PID is: %d, my parent PID is: %d\n", getpid(), getppid());
            
        } else {                    /* Process cha */
            printf("\nIm the parent process, counter: %d\n", ++counter);
            printf("My PID is: %d\n", getpid());
	    while (1);
        }
    } else {
        printf("fork() unsuccessfully\n");      // fork() return -1 nếu lỗi.
    }

    return 0;
}
