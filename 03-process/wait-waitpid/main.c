#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])   /* Cấp phát stack frame cho hàm main() */
{
    /* code */
    pid_t child_pid;                /* Lưu trong stack frame của main() */
    int status, rv;

    child_pid = fork();         
    if (child_pid >= 0) {
        if (0 == child_pid) {       /* Process con */
            printf("Im the child process, my PID: %d\n", getpid());
            while(1);
            //printf("Child process terminate after 5 seconds\n");
	    //sleep(5);
	    //exit(0);

        } else {                     /* Process cha */
	    //while(1);
            rv = wait(&status);
            if (rv == -1) {
                printf("wait() unsuccessful\n");
            }

            printf("\nIm the parent process, PID child process: %d\n", rv);
            
           // if (WIFEXITED(status)) {
             //   printf("Normally termination, status = %d\n", WEXITSTATUS(status));
           // } else if (WIFSIGNALED(status)) {
            //    printf("killed by signal, value = %d\n", WTERMSIG(status));
            //} 
        }
    } else {
        printf("fork() unsuccessfully\n");      // fork() return -1 nếu lỗi.
    }

    return 0;
}
