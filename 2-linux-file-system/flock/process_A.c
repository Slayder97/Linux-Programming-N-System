#include <sys/stat.h>
#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) 
{
	int fd;
	char text[16] = {0};

	sprintf(text,"hello word\n");
	if((fd=open("./text.txt", O_RDWR|O_CREAT, 0666)) == -1) {
		printf("can not create file \n");
		return 0;
	} else {
		printf("create file text.txt\n");
	}

	if(write(fd, text, sizeof(text)-1) == -1) {
		printf("can not write file \n");
		return 0;
	} else {
		printf("write file \n");
	}

	if(flock(fd, LOCK_SH) == -1) {
		printf("can not set read lock\n");
	} else {
		printf("set read lock\n");
	}
    
	while(1) {
		sleep(1);
	}
	close(fd);

	return 0;
}
