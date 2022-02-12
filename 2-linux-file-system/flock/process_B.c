#include <stdio.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include<fcntl.h>

int main(void) 
{
	int fd;
	char buf[16] = {0};

	if((fd=open("./text.txt",O_RDWR)) == -1) {
		printf("can not open file \n");
		return 0;
	} else {
		printf("open file test.txt \n");
	}

	if(flock(fd, LOCK_EX | LOCK_NB) == -1) {
		printf("can not get write lock\n");
	}
	
	if(flock(fd, LOCK_SH | LOCK_NB) == -1) {
		printf("can not get read lock\n");
	} else {
		printf("get read lock file\n");
		if(read(fd, buf, sizeof(buf)-1) == -1) {
			printf("can not read file \n");
			return 0;
		} else
    		printf("%s\n",buf);	
	}

	close(fd);

	return 0;
}
