#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFFER_SIZE 100
  
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[BUFFER_SIZE];
} message;
  
int main()
{
    key_t key;
    int msgid;
  
    // ftok to generate unique key
    key = ftok("progfile", 65);
  
    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Enter type message: "); 
    scanf("%ld", &message.mesg_type); 
    stdin = freopen(NULL,"r",stdin); // clear input buffer

    printf("Enter message: "); 
    fgets(message.mesg_text, BUFFER_SIZE, stdin);
  
    // msgsnd to send message
    msgsnd(msgid, &message, sizeof(message), 0);
  
    return 0;
}