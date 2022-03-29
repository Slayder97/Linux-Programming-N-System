#include <stdlib.h>     // standard stuff
#include <sys/mman.h>   // mmap()
#include <stdio.h>      // io stuff
#include <unistd.h>     // sleep()
#include <semaphore.h>  // semaphore()
#include <time.h>       // time()


#define BUFFER_SIZE 10

typedef struct Buffer{
    char **Tuples;
    int inSlotIndex;
    int outSlotIndex;
} Buffer;

int main()
{
    /* Create shared memory */
    int *buffer = (char*)mmap(NULL, sizeof(int)*BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    /* Create shared semaphores */
    sem_t *mutex = (sem_t*)mmap(NULL, sizeof(sem_t*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *full = (sem_t*)mmap(NULL, sizeof(sem_t*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *empty = (sem_t*)mmap(NULL, sizeof(sem_t*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_init(mutex, 1, 1);
    sem_init(empty, 1, BUFFER_SIZE);
    sem_init(full, 1, 0);

    /* Forking */
    pid_t producer;
    pid_t consumer;

    /* Child producer process */
    if((producer = fork()) == 0)
    {
        while(1)
        {
            sem_wait(empty);
            sem_wait(mutex);
           
			/* YOUR TASK: IMPLEMENT IN/OUT POINTERS TO ADD/TAKE FROM THE BUFFERS IN FIFO FASHION */
            printf("Producer creates something\n");
           
            sem_post(full);
            sem_post(mutex);

            /* Sleep between 0 and 3 seconds */
            srand(time(NULL));
            sleep(rand()% 5);
        }
    }

    /* Child consumer process */
    if((consumer = fork()) == 0)
    {
        while(1)
        {
            sem_wait(full);
            sem_wait(mutex);
            
			/* YOUR TASK: IMPLEMENT IN/OUT POINTERS TO ADD/TAKE FROM THE BUFFERS IN FIFO FASHION */
            printf("Consumer takes something\n");

            sem_post(mutex);
            sem_post(empty);

            /* Sleep between 3 and 8 seconds */
            srand(time(NULL));
            sleep(3 + rand()% 5);
        }
    }
    /* Parent */
    else
    {
        while(1)
        {
            sleep(10);
            int takenSlots;
            sem_getvalue(full, &takenSlots);
            printf("Items in the buffer: %d/%d\n", takenSlots, BUFFER_SIZE);
        }
    }

}