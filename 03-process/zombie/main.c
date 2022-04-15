#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])   /* Cấp phát stack frame cho hàm main() */
{
    /* code */
    pid_t child_pid;                /* Lưu trong stack frame của main() */
    int status;
    int counter = 0;
 
        child_pid = fork();         
	if (child_pid >= 0) {
		if (0 == child_pid) {       /* Process con */
			printf("Child process termination\n");
			//do nothing	
 			//while(1);
	        } else {                    /* Process cha */
	            while(1);  
	            //wait(&status);
	        }
	} else {                        /* fork() return -1 nếu lỗi. */
	        printf("fork() unsuccessfully\n"); 
	}

    return 0;
}
