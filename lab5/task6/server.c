#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int serverOn = 1;

void handler(int signal) {
    if (signal == SIGINT) {
        printf("Signal = %d, on = %d\n", signal, serverOn);
        serverOn = 0;
        printf("serverOn = %d\n", serverOn);
    }
}


struct message {
    long type;
    char text[150];
};


int main(int argc, char *argv[]) {
    key_t key = 228;
    int myQid = msgget(key, IPC_CREAT | 0600);
    int clientsQueueId;
    signal(SIGINT, handler);
    while (serverOn) {
        struct message *forServer = (struct message *) malloc(sizeof(struct message));
        int bytesQuantity = msgrcv(myQid, forServer, 150, 1, IPC_NOWAIT);
        if (bytesQuantity != -1) {
            char *ptr1 = strtok(forServer->text, "_");
            char *ptr2 = strtok(NULL, "_");
            clientsQueueId = atoi(ptr1);
            int clientPID = atoi(ptr2);
            printf("ID очереди сервера=%d, ID очереди клиента=%d, PID клиента=%d\n", myQid, clientsQueueId, clientPID);
            struct message *forClient = (struct message *) malloc(sizeof(struct message));
            forClient->type = clientPID;
            sprintf(forClient->text, "Ответ в очередь %d процессу %d\n", clientsQueueId, clientPID);
            if (msgsnd(clientsQueueId, forClient, 150, 0) == -1) {
                perror("Ошибка при послании клиенту от сервера");
                //exit(1);
            }
            free(forClient);
        }
        free(forServer);
    }
    if (msgctl(myQid, IPC_RMID, 0) == -1) {
        perror("Ошибка при удалении очереди сервера");
    }
    if (msgctl(clientsQueueId, IPC_RMID, 0) == -1) {
        perror("Ошибка при удалении очереди клиента");
    }
    exit(1);
}