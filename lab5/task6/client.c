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


struct message {
    long type;
    char text[150];
};


int main(int argc, char * argv[]) {
    key_t clientKey = 1488;
    key_t serverKey = 228;
    int messageType = getpid();
    if (argc > 2) {
        fprintf(stderr, "Можно передать лишь 1 опцию: -dfq или -united\n");
    }
    if (argc == 2) {
        if (!strcmp(argv[1], "-dfq")) { // у каждого клиента своя очередь
            clientKey = ftok(argv[0], getpid());
        }
        else if (!strcmp(argv[1], "-united")) { // у всех вообще 1 очередь (очередь сервера)
            clientKey = serverKey;
        }
        else if (!strcmp(argv[1], "-sht")) { // один тип сообщений для всех клиентов
            messageType = 666;
        }
        else {
            fprintf(stderr, "Некорректная опция\n");
        }
    }
    int clientsQID = msgget(clientKey, IPC_CREAT | 0600);
    int serverQID = msgget(serverKey, IPC_CREAT | 0600);
    struct message * forServer = (struct message * ) malloc(sizeof(struct message));
    forServer->type = 1;
    sprintf(forServer->text, "%d_%d", clientsQID, messageType);
    if (msgsnd(serverQID, forServer, 150, 0) == -1) {
        perror("Ошибка при послании от клиента к серверу");
        exit(1);
    }
    struct message * forClient = (struct message * ) malloc(sizeof(struct message));
    printf("PID клиента на клиенте = %d\n", getpid());
    if (msgrcv(clientsQID, forClient, 150, messageType, 0) == -1) {
        perror("Ошибка при получении сообщения клиентом");
        exit(1);
    }
    printf("Получен ответ от сервера: %s\n", forClient->text);
    if (argc == 2 && !strcmp(argv[1], "-dfq")) { // у каждого клиента своя очередь
        if (msgctl(clientsQID, IPC_RMID, 0) == -1) {
            perror("У каждого клиента своя очередь, но ошибка");
        }
    }
    exit(0);
}

/*Сделаем несколько опций:
 * 1) -dfq : у каждого клиента своя очередь сообщений
 * 2) -united : у всех процессов одна очередь сообщений (очередь сервера)
 * 3) -sht : всем клиентам доступны сообщения определенного типа
*/