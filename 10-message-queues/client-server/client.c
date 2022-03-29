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

static int clientId;

/**************************************************************************************************************
Function declaration
***************************************************************************************************************/
static void removeQueue(void);

/**************************************************************************************************************
Function implementations
***************************************************************************************************************/

/* remove Message Queue */
static void removeQueue(void)
{
    if (msgctl(clientId, IPC_RMID, NULL) == -1)
        log("msgctl");
}

int main(int argc, char *argv[])
{
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t msgLen, totBytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        log("%s pathname\n", argv[0]);

    if (strlen(argv[1]) > sizeof(req.pathname) - 1)
        log("pathname too long (max: %ld bytes)\n", (long) sizeof(req.pathname) - 1);

    /* Get server's queue identifier; create queue for response */
    serverId = msgget(SERVER_KEY, S_IWUSR);
    if (serverId == -1)
        log("msgget - server message queue");
    
    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId == -1)
        log("msgget - client message queue");
        
    if (atexit(removeQueue) != 0)
        log("atexit");

    /* Send message asking for file named in argv[1] */
    req.mtype = 1; /* Any type will do */
    req.clientId = clientId;
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';

    /* Ensure string is terminated */
    if (msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1)
        log("msgsnd");

    /* Get first response, which may be failure notification */
    msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
    if (msgLen == -1)
        log("msgrcv");

    if (resp.mtype == RESP_MQ_FAILURE) {
        printf("%s\n", resp.data); /* Display msg from server */

    if (msgctl(clientId, IPC_RMID, NULL) == -1)
        log("msgctl");
        exit(EXIT_FAILURE);
    }

    /* File was opened successfully by server; process messages
    (including the one already received) containing file data */
    totBytes = msgLen;      /* Count first message */
    for (numMsgs = 1; resp.mtype == RESP_MQ_DATA; numMsgs++) {
        msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
        if (msgLen == -1)
            log("msgrcv");
        totBytes += msgLen;
    }

    printf("Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);
    exit(EXIT_SUCCESS);
}

