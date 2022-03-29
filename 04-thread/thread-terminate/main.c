#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_t thread_id1, thread_id2, thread_id3;

typedef struct {
    char name[30];
    char msg[30];
} thr_data_t;

static void *thr_handle1(void *args)
{
    thr_data_t *thr = (thr_data_t *)args;
    sleep(1);
    printf("hello %s !\n", thr->name);
    printf("thread1 handler\n");

    pthread_exit(NULL); // exit

}

static void *thr_handle2(void *args)
{
    sleep(5);
    //pthread_exit(NULL); // exit
    //exit(1);
    while (1) {
     	printf("thread2 handler\n"); 
   	    sleep(1);
    };
}

static void *thr_handle3(void *args)
{
    pthread_detach(pthread_self());
    //sleep(2);
    //pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    /* code */
    int ret, counter = 0;
    int retval;
    thr_data_t data = {0};

    strncpy(data.name, "phonglt9", sizeof(data.name));                 
    strncpy(data.msg, "Posix thread programming\n", sizeof(data.msg));

    if (ret = pthread_create(&thread_id1, NULL, &thr_handle1, &data)) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    if (ret = pthread_create(&thread_id2, NULL, &thr_handle2, NULL)) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }
    
    //sleep(5);
    //pthread_cancel(thread_id2);
    //pthread_join(thread_id2, NULL);
    //printf("thread_id2 termination\n"); 
    while (1) {
	    if (ret = pthread_create(&thread_id3, NULL, &thr_handle3, NULL)) {
        	printf("pthread_create() error number=%d\n", ret);
        	break;
    	}
	    counter++;
  	    //pthread_join(thread_id3, NULL);

        if (counter%1000 == 0) {
            printf("Thread created: %d\n", counter);
        }
    }   

    return 0;
}
