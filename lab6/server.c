#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/msg.h>
#include <wait.h>
#include <errno.h>
#include <time.h>


#define MAXLINE 400
#define CONFIG "/home/artemcoolac/CLionProjects/AOS_Labs/lab6/config"

struct message {
    long type;
    char text[150];
};

int PORT = 8081;
int mqID = -1;
int serverOn = 1;
int currentSockFD = -1;
int configFD = -1;
int journalFD = -1;
int MQBlocked = 0;


void daemonize() {
    int pid = fork();
    if (pid < 0) {
        printf("Порождение дочернего процесса не было выполнено\n");
        exit(1);
    }
    if (pid > 0) {
        printf("Дочерний процесс %d\n", pid);
        exit(0);
    }
    umask(0);
    int sid = setsid();
    if (sid < 0) {
        exit(1);
    }
    chdir("/");
    for (int i = 0; i < 3; i++) {
        close(i);
    }
}


void reread(int signal) {
    if (signal == SIGHUP) {
        close(configFD); // закрываем конфигурационный файл на всякий случай
        int statusOK = 1;
        char loggingOldMessage[200];
        FILE * config = fopen(CONFIG, "r"); // открываем конфиг
        if (!config) {
            statusOK = 0; // если не открылся, просто продолжим работу как будто ничего не случилось
            sprintf(loggingOldMessage, "Не могу открыть конфигурационный файл!\n");
            write(journalFD, loggingOldMessage, strlen(loggingOldMessage));
        }
        if (statusOK) {
            configFD = fileno(config); // перезапишем дескриптор конфига
            char *bufferStr = (char *) malloc(10 * sizeof(char));
            fgets(bufferStr, 10, config); // считываем порт
            int newPORT = atoi(bufferStr); // считали порт
            if (!newPORT) {
                statusOK = 0;
                sprintf(loggingOldMessage, "Новый порт не был считан правильно!\n");
                write(journalFD, loggingOldMessage, strlen(loggingOldMessage));
            }
            else {
                PORT = newPORT; // когда считывание нового порта прошло успешно, перезапишем его
            }
            free(bufferStr);
            if (statusOK) {
                bufferStr = (char *) malloc(200 * sizeof(char));
                fgets(bufferStr, 200, config); // считываем путь нового файла журнала
                char loggingMessage[500];
                sprintf(loggingMessage, "journalNewName = %s\n", bufferStr);
                write(journalFD, loggingMessage, strlen(loggingMessage));
                int newJournalFD = open(bufferStr, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0666);
                if (newJournalFD == -1) {
                    sprintf(loggingMessage, "Ошибка при открытии файла журнала %d\n", errno);
                    write(journalFD, loggingMessage, strlen(loggingMessage));
                }
                else {
                    close(journalFD); // старый журнал закрываем
                    journalFD = newJournalFD; // дескриптор нового перезаписываем
                }
                free(bufferStr);
                bufferStr = (char *) malloc(100 * sizeof(char));
                fgets(bufferStr, 100, config);
                int newKey = atoi(bufferStr); // считали ключ для очереди сообщений
                free(bufferStr);
                bufferStr = NULL;
                if (!newKey) {
                    sprintf(loggingMessage, "Ключ для получения очереди сообщений не был считан правильно!\n");
                    write(journalFD, loggingMessage, strlen(loggingMessage));
                }
                else {
                    int newMqID = msgget(newKey, 0600 | IPC_CREAT | IPC_EXCL);
                    if (newMqID != -1) { // если такая очередь не существует
                        while (MQBlocked);
                        msgctl(mqID, IPC_RMID, 0); // удалим старую
                        mqID = newMqID; // дескриптор новой перезапишем
                    }
                }
            }
        }
    }
}


void finish(int signal) {
    if (signal == SIGTERM) {
        msgctl(mqID, IPC_RMID, 0);
        serverOn = 0;
        while (wait(0) > 0);
        char loggingMessage[150];
        sprintf(loggingMessage, "Закрываем все, завершаем работу сервера\n");
        write(journalFD, loggingMessage, strlen(loggingMessage));
        close(currentSockFD);
        close(journalFD);
        close(configFD);
        exit(EXIT_SUCCESS);
    }
}

int genRandom(int lower, int upper) {
    srand(time(0));
    return (rand() % (upper - lower + 1)) + lower;
}


int handlingMaster(int figure, int master, int w1, int w2, int w3) {
    if (getpid() == master) {
        MQBlocked = 1;
        char loggingMessage[300];
        struct message *forOtherProcess = (struct message *) malloc(sizeof(struct message));
        struct message *forMe = (struct message *) malloc(sizeof(struct message));
        forOtherProcess->type = w1;
        sprintf(forOtherProcess->text, "%d_%d_%d_%d", figure, w2, w3, master);
        sprintf(loggingMessage, "Записанный текст для отправления воркеру №1: %s\n", forOtherProcess->text);
        write(journalFD, loggingMessage, strlen(loggingMessage));
        sprintf(loggingMessage, "Здесь должен быть только родитель %d\n", getpid());
        write(journalFD, loggingMessage, strlen(loggingMessage));
        int code = msgsnd(mqID, forOtherProcess, 150, 0);
        sprintf(loggingMessage, "Отправилось воркеру с кодом = %d\n", code);
        write(journalFD, loggingMessage, strlen(loggingMessage));
        int codeReceived = msgrcv(mqID, forMe, 150, master, 0);
        sprintf(loggingMessage,"Сообщение от w3 принял, код %d, текст: %s\n", codeReceived, forMe->text);
        char * receivedFigure = strtok(forMe->text, "_");
        int newFigure = atoi(receivedFigure);
        free(forMe);
        free(forOtherProcess);
        forMe = NULL;
        forOtherProcess = NULL;
        MQBlocked = 0;
        return newFigure;
    }
}

void handlingWorker() {
    char loggingMessage[400];
    struct message *forOtherProcess = (struct message *) malloc(sizeof(struct message));
    struct message *forMe = (struct message *) malloc(sizeof(struct message));
    int code = msgrcv(mqID, forMe, 150, getpid(), 0);
    sprintf(loggingMessage, "Воркер %d сообщение принял, код %d\n", getpid(), code);
    write(journalFD, loggingMessage, strlen(loggingMessage));
    char * figure = strtok(forMe->text, "_");
    int receivedFigure = atoi(figure);
    char * nextProcess = strtok(NULL, "_");
    int nextProc = atoi(nextProcess);
    char * nextText = strtok(NULL, "");
    int newFigure = receivedFigure << 1;
    usleep(genRandom(1, 100) * 1000);
    forOtherProcess->type = nextProc;
    sprintf(forOtherProcess->text, "%d_%s", newFigure, nextText);
    sprintf(loggingMessage, "Рассчитанное воркером число %d, тип сообщения %ld, сообщение следующему %s\n", newFigure, forOtherProcess->type, forOtherProcess->text);
    write(journalFD, loggingMessage, strlen(loggingMessage));
    int codeSent = msgsnd(mqID, forOtherProcess, 150, 0);
    sprintf(loggingMessage, "Сообщение отправилось с кодом %d\n", codeSent);
    write(journalFD, loggingMessage, strlen(loggingMessage));
    free(forOtherProcess);
    free(forMe);
    forOtherProcess = NULL;
    forMe = NULL;
}


char * handlingString(char * forServer) {
    //printf("Передано сообщение от клиента в функцию: %s\n", forServer);
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
        char * toClient = (char *) malloc(MAXLINE * sizeof(char));
        char * rest = forServer;
        char * figure = strtok_r(forServer, "_", &rest);
        char loggingMessage[150];
        sprintf(loggingMessage, "Master: часть строки %s, rest=%s\n", figure, rest);
        write(journalFD, loggingMessage, strlen(loggingMessage));
        while (figure != NULL) {
            char miniBuf[20];
            sprintf(loggingMessage, "Master: часть строки: %s\n", figure);
            write(journalFD, loggingMessage, strlen(loggingMessage));
            int number = atoi(figure);
            int returned = handlingMaster(number, master, w1, w2, w3);
            sprintf(loggingMessage, "Master: возвращенное число после воркеров : %d\n", returned);
            sprintf(miniBuf, "%d", returned);
            strcat(toClient, miniBuf);
            strcat(toClient, " ");
            figure = strtok_r(NULL, "_", &rest);
            sprintf(loggingMessage, "Master: после разделения: %s, rest=%s\n", figure, rest);
            write(journalFD, loggingMessage, strlen(loggingMessage));
        }
        kill(w1, SIGINT);
        kill(w2, SIGINT);
        kill(w3, SIGINT);
        while (wait(0) > 0);
        sprintf(loggingMessage, "Воркеры %d, %d, %d отработали и были завершены\n", w1, w2, w3);
        write(journalFD, loggingMessage, strlen(loggingMessage));
        return toClient;
    }
    else {
        while (1) {
            handlingWorker();
        }
    }
}

void networkInteraction() {
    int sockFD;
    char buffer[MAXLINE];
    struct sockaddr_in serverAddress, clientAddress;
    // Creating socket file descriptor
    if ((sockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    currentSockFD = sockFD;
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));
    // Filling server information
    serverAddress.sin_family = AF_INET; // IPv4
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);
    // Bind the socket with the server address
    if (bind(sockFD, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    char loggingMessage[500];
    while (serverOn) {
        int len, n;
        len = sizeof(clientAddress);  //len is value/result
        n = recvfrom(sockFD, (char *) buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &clientAddress, &len);
        buffer[n] = '\0';
        sprintf(loggingMessage,"Прислано сообщение %s\n", buffer);
        write(journalFD, loggingMessage, strlen(loggingMessage));
        char * toClient = handlingString(buffer);
        sprintf(loggingMessage,"Сообщение для клиента: %s\n", toClient);
        write(journalFD, loggingMessage, strlen(loggingMessage));
        sendto(sockFD, (const char *) toClient, strlen(toClient), MSG_CONFIRM, (const struct sockaddr *) &clientAddress, len);
        free(toClient);
        toClient = NULL;
    }
}


int main() {
    FILE * config = fopen(CONFIG, "r");
    if (!config) {
        fprintf(stderr, "Не могу открыть конфигурационный файл!\n");
        exit(EXIT_FAILURE);
    }
    configFD = fileno(config);
    char * bufferStr = (char *) malloc(10 * sizeof(char));
    fgets(bufferStr, 10, config);
    PORT = atoi(bufferStr);
    if (!PORT) {
        fprintf(stderr, "Порт не был считан правильно!\n");
        exit(EXIT_FAILURE);
    }
    free(bufferStr);
    bufferStr = (char *) malloc (200 * sizeof(char));
    fgets(bufferStr, 200, config);
    char loggingMessage[500];
    sprintf(loggingMessage, "journalName = %s\n", bufferStr);
    write(journalFD, loggingMessage, strlen(loggingMessage));
    bufferStr[strlen(bufferStr) -1] = '\0';
    journalFD = open(bufferStr,O_WRONLY | O_CREAT, 0666);
    if (journalFD == -1) {
        perror("Ошибка при открытии файла журнала");
        exit(EXIT_FAILURE);
    }
    free(bufferStr);
    bufferStr = (char *) malloc(100 * sizeof(char));
    fgets(bufferStr, 100, config);
    int key = atoi(bufferStr);
    free(bufferStr);
    bufferStr = NULL;
    if (!key) {
        fprintf(stderr, "Ключ для получения очереди сообщений не был считан правильно!\n");
        exit(EXIT_FAILURE);
    }
    daemonize();
    mqID = msgget(key, 0600 | IPC_CREAT);
    sprintf(loggingMessage, "Из конфига получены : MQID = %d, PORT=%d, journalFD=%d\n", mqID, PORT, journalFD);
    write(journalFD, loggingMessage, strlen(loggingMessage));
    signal(SIGHUP, reread);
    signal(SIGTERM, finish);
    networkInteraction();
    return 0;
}