#include <netinet/in.h>
#include <sys/socket.h>
#include <stddef.h>

#define BUF_SIZE 10 
#define PORT_NUM 50002

int main(int argc, char *argv[])
{    
	struct sockaddr_in svaddr;
    int fd;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];
	
    fd = socket(AF_INET, SOCK_DGRAM, 0);      
    if (fd == -1)
		return 1;
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(PORT_NUM);
    if(inet_pton(AF_INET, argv[1], &svaddr.sin_addr) <= 0)
		return 1;

	msgLen = strlen(argv[2]);
	if(sendto(fd, argv[2],msgLen,0,(struct sockaddr *)&svaddr, sizeof(struct sockaddr_in)) != msgLen)
		return 1;
	numBytes = recvfrom(fd, resp, BUF_SIZE, 0, NULL, NULL);
	if (numBytes == -1)
			return 1;
	else
		printf("Response : %s\n", resp);
    
    return 0;
}
