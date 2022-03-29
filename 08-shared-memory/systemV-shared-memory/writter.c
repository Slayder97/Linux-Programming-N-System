#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char const *argv[])
{
    /**
     *  @brief Shared memory key
     * 
     *  1. Can be an arbitrary value  (SHM_KEY 0x1234)
     *  2. IPC_PRIVATE: Running processes as server and client (parent and child relationship)
     *  3. Key_t ftok(const char *path, int id);
     *  @param[in] path file path
     *  @param[in] id   projectID, only 8 lower bits
     *  @return identifier key if ok. -1 on error
     */
    key_t key = ftok("./shmfile", 65);

    /**
     *  int shmget(key_t key, size_t size, int shmflg)
     *  @brief Create shared memory segment
     * 
     *  @param[in] key Identifier key
     *  @param[in] size The size of the shared memory segment rounded to multiple of PAGE_SIZE
     *  @param[in] shmflg mode flags
     *                    IPC_CREAT --> Creating new segment. if the segment associated with this key already
     *                              exists, use the existing segment.
     *                    IPC_EXCL  --> Create new segment. if the segment already exists, the call fails.
     *  @return Shared memory ID if OK, −1 on error
     */
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    /**
     * void * shmat(int shmid, const void *shmaddr, int shmflg)
     * @brief Attaching a shared memory segment to the address space of the calling process
     * 
     * @param[in] shmid shared memory ID
     * @param[in] shmaddr Calling address will be attached to. 
     *                    If the value is 0 (NULL), the kernel will attach to the appropriate memory area
     * @param[in] shmflg SHM_RDONLY --> Attaches the segment for read-only purpose, by default it is read-write
     *                   SHM_REMAP  --> Replaces the existing mapping in the range specified by shmaddr and continuing till the end of segment
     *                              If shmaddr is non-zero and the SHM_RND flag is not used, the segment will be attached to the given address.
     *                              If the value is non-zero and the SHM_RND flag is used, the address attached to is: 
     *                              shmaddr – (addr module SHMLBA - Lower Boundary Address), which means the address is translated to the next SHMLBA
     * @return Pointer to shared memory segment if OK, −1 on error
     */   
    char *str = (char*)shmat(shmid, (void *)0, 0);

    printf("Message to shared memory: ");
    fgets(str, 1024, stdin);

    /**
     * int shmdt(const void *shmaddr)
     * @brief Detaching the shared memory segment from the address space of the calling process
     * 
     * @param[in] shmaddr The address of shared memory segment to be detached, returned by the shmat()
     *                    system call this command does not remove shared memory ID and structure from 
     *                    the system shared memory ID will persist until a process in the system calls
     *                    shmctl with the IPC_RMID command.
     * @return 0 if OK, −1 on error
     */  
    while(1);
    shmdt(str);
  
    return 0;

}
