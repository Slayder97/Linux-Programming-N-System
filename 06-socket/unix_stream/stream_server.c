#include <sys/un.h>
#include <sys/socket.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>

#define BUF_SIZE 10
#define SOCK_PATH "./sock_stream"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr;
    int fd, datafd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE]={0};

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) 
        return 1;

    memset(&svaddr, 0, sizeof(struct sockaddr_un)); 
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SOCK_PATH, sizeof(svaddr.sun_path)-1);

    if (bind(fd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1) 
        return 1;
 
    if ((listen(fd, 5)) == -1)
	return 1; 
    else
	printf("start listening on server\n");
	
    if((datafd = accept(fd, (struct sockaddr*)&svaddr, &len)) == -1)
	return 1;
    else
	printf("accept connect\n");
	
    while(1) {
        if(read(datafd, buf, BUF_SIZE) > 0)
		{
			for (j = 0; j < BUF_SIZE; j++) 
				buf[j] = toupper((unsigned char) buf[j]);
			printf("start response %s\n",buf);
			write(datafd,buf,BUF_SIZE-1);
		}
    }
	close(fd);
	remove(SOCK_PATH);
}
