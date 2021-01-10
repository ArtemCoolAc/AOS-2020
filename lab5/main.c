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


int main() {
//    if (argc != 3) {
//        fprintf(stderr, "Формат команды: ID_очереди_сообщений тип_сообщения");
//        exit(0);
//    }
    char * argv[] = {"./main.c", "13", "1"};
    int qid = atoi(argv[1]);
    int messageType = atoi(argv[2]);
    struct message * tmp = (struct message * ) malloc(sizeof(struct message));
    int code;
    code = msgrcv(qid, tmp, 150, messageType, 0);
    if (code == - 1) {
        perror("Что случилось");
    }
    printf("Тип сообщения: %ld, сообщение: %s, код: %d\n", tmp->type, tmp->text, code);
    exit(0);
}