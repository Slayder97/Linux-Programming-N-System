/**************************************************************************************************************
 <Copyright PHONGLT9>
Product:
Module:
Version: 1.0
Author:  phonglt9
Created: 16/03/2022
***************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stddef.h>     /* For definition of offsetof() */
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

/**************************************************************************************************************
Macro definitions
***************************************************************************************************************/
/* For debbugging purposees only */
#ifdef DEBUG
#undef DEBUG
#define DEBUG(fmt, args...) { log_printf (LOG_ERROR, "%d: ", __LINE__); log_printf (LOG_ERROR, fmt, ## args); }
#else
#undef DEBUG
#define DEBUG(fmt, args...)
#endif

/* Errors level */
#define LOG_NORMAL      0x01
#define LOG_ERROR       0x02

#define SERVER_KEY 0x1aaaaaa1 /* Key for server's message queue */

struct requestMsg {             /* Requests (client to server) */
    long mtype;                 /* Unused */
    int clientId;               /* ID of client's message queue */
    char pathname[PATH_MAX];    /* File to be returned */
};

#define REQ_MSG_SIZE (offsetof(struct requestMsg, pathname) - offsetof(struct requestMsg, clientId) + PATH_MAX)
#define RESP_MSG_SIZE 8192

struct responseMsg {            /* Responses (server to client) */
    long mtype;                 /* One of RESP_MT_* values below */
    char data[RESP_MSG_SIZE];   /* File content / response message */
};

/* Types for response messages sent from server to client */
enum typeMsg {
    RESP_MQ_FAILURE = 1,    /* File couldn't be opened */
    RESP_MQ_DATA,           /* Message contains file data */
    RESP_MQ_END             /* File data complete */
};

/**************************************************************************************************************
Constants variables
***************************************************************************************************************/

/**************************************************************************************************************
Global variables
***************************************************************************************************************/
 
#define LOG     0x01
#define log(...) {  \
    do {    \
        if ((LOG)) { \
            printf("\n[%s:%s:%d] ==>> ", __FILE__, __func__, __LINE__); \
            printf(__VA_ARGS__); \
            printf("\n"); \
        } \
    } while (0); \
}

/**************************************************************************************************************
Function declaration
***************************************************************************************************************/
static void grimReaper(int sig);
static void serveRequest(const struct requestMsg *req);

/**************************************************************************************************************
Function implementations
***************************************************************************************************************/

/* 2. SIGCHLD handler */
void grimReaper(int sig)
{
    int savedErrno;
    savedErrno = errno; /* waitpid() might change 'errno' */

    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;

    errno = savedErrno;
}

/* 4. Executed in child process: serve a single client */ 
void serveRequest(const struct requestMsg *req)
{
    int fd;
    ssize_t numRead;
    struct responseMsg resp;

    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) { /* Open failed: send error text */
        resp.mtype = RESP_MQ_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        msgsnd(req->clientId, &resp, strlen(resp.data) + 1, 0);
        exit(EXIT_FAILURE); /* and terminate */
    }

    /* Transmit file contents in messages with type RESP_MT_DATA. We don't
    diagnose read() and msgsnd() errors since we can't notify client. */
    resp.mtype = RESP_MQ_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)
        if (msgsnd(req->clientId, &resp, numRead, 0) == -1)
            break;

    /* Send a message of type RESP_MT_END to signify end-of-file */
    resp.mtype = RESP_MQ_END;
    msgsnd(req->clientId, &resp, 0, 0); /* Zero-length mtext */
}

int main(int argc, char *argv[])
{
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    int serverId;
    struct sigaction sa;

    /* 1. Create server message queue */
    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IWGRP);
    if (serverId == -1)
        log("msgget");

    /* 2. Establish SIGCHLD handler to reap terminated children */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        log("sigaction");

    /* 3. Read requests, handle each in a separate child process */
    for (;;) {
        msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1) {
            if (errno == EINTR) /* Interrupted by SIGCHLD handler? */
                continue;       /* ... then restart msgrcv() */
            log("msgrcv");   /* Some other error */
            break;              /* ... so terminate loop */
        }

        pid = fork();           /* Create child process */
        if (pid == -1) {
            log("fork");
            break;
        }

        if (pid == 0) {         /* 4. Child handles request */
            serveRequest(&req);
            exit(EXIT_SUCCESS);
        }   
        /* Parent loops to receive next client request */
    }

    /* 5. If msgrcv() or fork() fails, remove server MQ and exit */
    if (msgctl(serverId, IPC_RMID, NULL) == -1)
        log("msgctl");
    exit(EXIT_SUCCESS);
}


