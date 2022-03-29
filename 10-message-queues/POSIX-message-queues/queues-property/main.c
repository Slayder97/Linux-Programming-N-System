#include <stdio.h>
#include <string.h>
#include <mqueue.h>  
#include <errno.h>  

#define MQ_MODE (S_IRUSR | S_IWUSR ) 
  
int main(int argc, char *argv[])  
{  
    struct mq_attr attr, attrp;

    // attr.mq_maxmsg = 5;
    // attr.mq_msgsize = 2048;
    
    printf("Create mqueu\n");  
    mqd_t mqid = mq_open("/mqueue", O_RDWR | O_CREAT | O_NONBLOCK, MQ_MODE, NULL);  
    if (mqid == -1) {  
        printf("mq_open() error %d: %s\n", errno, strerror(errno));  
        return -2;  
    }

    if (mq_getattr(mqid, &attrp) != 0) {  
        printf("mq_open() error %d: %s\n", errno, strerror(errno));  
        return -3;  
    }

    if (attrp.mq_flags == 0)  
        printf("mq_flags = 0\n");  
    else  
        printf("mq_flags = O_NONBLOCK\n");

    printf("mq_maxmsg = %ld,\n", attrp.mq_maxmsg);  
    printf("mq_msgsize = %ld\n", attrp.mq_msgsize);  
    printf("mq_curmsgs = %ld\n", attrp.mq_curmsgs);  

    mq_unlink("/mqueue");
    return 0;  
}