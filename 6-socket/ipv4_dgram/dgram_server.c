#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stddef.h>
#include <ctype.h>

#define BUF_SIZE 10
#define PORT_NUM 50002

int main(int argc, char *argv[])
{
    struct sockaddr_in svaddr, claddr;
    int fd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];
    char claddrStr[INET_ADDRSTRLEN];

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) 
        return 1;

    memset(&svaddr, 0, sizeof(struct sockaddr_in)); 
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY;
    svaddr.sin_port = htons(PORT_NUM);

    if (bind(fd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in)) == -1) 
        return 1; 
    printf("start listening on server\n");
	
    for (;;) {
        len = sizeof(struct sockaddr_in);
        numBytes = recvfrom(fd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);
		if (numBytes == -1)
			return 1;
			
		if (inet_ntop(AF_INET, &claddr.sin_addr, claddrStr, INET_ADDRSTRLEN) == NULL) 
			printf("Couldn't convert client address to string\n"); 
		else 
			printf("Server received %ld bytes from (%s, %u)\n", (long) numBytes, claddrStr, ntohs(claddr.sin_port)); 

		for (j = 0; j < numBytes; j++) 
			buf[j] = toupper((unsigned char) buf[j]);
			
		if (sendto(fd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes) 
			printf("sendto error\n");
	}
}
