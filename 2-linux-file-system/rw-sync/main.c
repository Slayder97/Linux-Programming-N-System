#include <aio.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 16

static int flag=0;
static void *value;

void sig_handler(int sig, siginfo_t *siginfo, void *ucontex)
{
    flag=1;
    value= siginfo->si_value.sival_ptr;
}

int main(void)
{
    int fd;
    struct sigaction action;
    struct sigevent sev;
    struct aiocb my_aiocb;
    char buf[BUFSIZE]={0};

    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = &sig_handler;
    if (sigaction(SIGRTMIN+1, &action,NULL) == SIG_ERR)
        printf("\ncan't catch SIGRTMIN+1\n");
    
    // read file
    if ((fd = open( "./test.txt", O_RDONLY )) == -1)
    	return 0;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN+1;
    sev.sigev_value.sival_ptr = buf;

    my_aiocb.aio_buf = buf;
    my_aiocb.aio_fildes = fd;
    my_aiocb.aio_nbytes = BUFSIZE-1;
    my_aiocb.aio_offset = 0;
    my_aiocb.aio_sigevent = sev;
    aio_read( &my_aiocb );

    while(1)
    {
        if(flag == 1)
        {
            printf("read file: %s \n", value);
            break;
        }
        sleep(1);
    }

    close(fd);
    return 0;
}
