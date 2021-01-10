// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/msg.h>
#include <wait.h>
#include <errno.h>
#include <time.h>


#define MAXLINE 1024

struct message {
    long type;
    char text[150];
};

int PORT = 8080;
int mqID = -1;
int serverOn = 1;
int currentSockFD = -1;


void finish(int signal) {
    if (signal == SIGTERM) {
        printf("mqID = %d\n", mqID);
        msgctl(mqID, IPC_RMID, 0);
        serverOn = 0;
        while (wait(0) > 0);
        close(currentSockFD);
        exit(0);
    }
}

int genRandom(int lower, int upper) {
    srand(time(0));
    return (rand() % (upper - lower + 1)) + lower;
}


int handlingMaster(int figure, int master, int w1, int w2, int w3) {
    if (getpid() == master) {
        struct message *forOtherProcess = (struct message *) malloc(sizeof(struct message));
        struct message *forMe = (struct message *) malloc(sizeof(struct message));
        forOtherProcess->type = w1;
        sprintf(forOtherProcess->text, "%d_%d_%d_%d", figure, w2, w3, master);
        //printf("Записанный текст для отправления воркеру №1: %s\n", forOtherProcess->text);
        //printf("Здесь должен быть только родитель %d\n", getpid());
        int code = msgsnd(mqID, forOtherProcess, 150, 0);
        //printf("code = %d, error = %s\n", code, sys_errlist[errno]);
        int codeReceived = msgrcv(mqID, forMe, 150, master, 0);
        //printf("Сообщение от w3 принял, код %d, статус: %s, текст: %s\n", codeReceived, sys_errlist[errno], forMe->text);
        char * receivedFigure = strtok(forMe->text, "_");
        int newFigure = atoi(receivedFigure);
        free(forMe);
        free(forOtherProcess);
        forMe = NULL;
        forOtherProcess = NULL;
        return newFigure;
    }
}

void handlingWorker() {
    struct message *forOtherProcess = (struct message *) malloc(sizeof(struct message));
    struct message *forMe = (struct message *) malloc(sizeof(struct message));
    int code = msgrcv(mqID, forMe, 150, getpid(), 0);
    //printf("Процесс %d сообщение принял, код %d, ошибка: %s\n", getpid(), code, sys_errlist[errno]);
    char * figure = strtok(forMe->text, "_");
    int receivedFigure = atoi(figure);
    char * nextProcess = strtok(NULL, "_");
    int nextProc = atoi(nextProcess);
    char * nextText = strtok(NULL, "");
    int newFigure = receivedFigure << 1;
    usleep(genRandom(1, 100) * 1000);
    forOtherProcess->type = nextProc;
    sprintf(forOtherProcess->text, "%d_%s", newFigure, nextText);
    //printf("Рассчитанное воркером число %d, тип сообщения %ld, сообщение следующему %s\n", newFigure, forOtherProcess->type, forOtherProcess->text);
    int codeSent = msgsnd(mqID, forOtherProcess, 150, 0);
    //printf("Сообщение отправилось с кодом %d, ошибка : %s\n", codeSent, sys_errlist[errno]);
    free(forOtherProcess);
    free(forMe);
    forOtherProcess = NULL;
    forMe = NULL;
}


char * handlingString(char * forServer) {
    printf("Передано сообщение от клиента в функцию: %s\n", forServer);
    int master = getpid();
    int w1 = -1, w2 = -1, w3 = -1;
    w1 = fork();
    if (w1) {
        w2 = fork();
        if (w2) {
            w3 = fork();
        }
    }
    if (getpid() == master) {
        char *toClient = (char *) malloc(MAXLINE * sizeof(char));
        char * rest = forServer;
        char *figure = strtok_r(forServer, "_", &rest);
        printf("Часть строки %s, rest=%s\n", figure, rest);
        while (figure != NULL) {
            char miniBuf[20];
            //printf("Часть строки: %s\n", figure);
            int number = atoi(figure);
            int returned = handlingMaster(number, master, w1, w2, w3);
            //printf("returned = %d\n", returned);
            sprintf(miniBuf, "%d", returned);
            strcat(toClient, miniBuf);
            strcat(toClient, " ");
            figure = strtok_r(NULL, "_", &rest);
            printf("После разделения: %s, rest=%s\n", figure, rest);
        }
        kill(w1, SIGINT);
        kill(w2, SIGINT);
        kill(w3, SIGINT);
        while (wait(0) > 0);
        return toClient;
    }
    else {
        close(0);
        while (1) {
            handlingWorker();
        }
    }
}

void networkInteraction() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    currentSockFD = sockfd;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    while (serverOn) {
        int len, n;
        len = sizeof(cliaddr);  //len is value/result
        n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';
        printf("Прислано сообщение %s\n", buffer);
        char *toClient = handlingString(buffer);
        printf("Сообщение для клиента: %s\n", toClient);
        sendto(sockfd, (const char *) toClient, strlen(toClient), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        free(toClient);
        toClient = NULL;
    }
}

int main() {
    mqID = msgget(228, 0600 | IPC_CREAT);
    signal(SIGTERM, finish);
    networkInteraction();
    return 0;
}