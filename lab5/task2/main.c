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
    if (argc < 3 || argc > 5) {
        fprintf(stderr, "Формат команды: ID_MQ MessageType [-nowait (не ждать)] [сколько байт]\n");
        exit(0);
    }
    int qid = atoi(argv[1]);
    int messageType = atoi(argv[2]);
    int flag = 0;
    if (argc > 3 && !strcmp(argv[3], "-nowait")) {
        flag = IPC_NOWAIT;
    }
    int nbytes = 150;
    if (argc == 5) {
        nbytes = atoi(argv[4]);
    }
    struct message * tmp = (struct message * ) malloc(sizeof(struct message));
    int bytesQuantity = msgrcv(qid, tmp, nbytes, messageType, flag);
    if (bytesQuantity == - 1) {
        perror("Что случилось");
    }
    printf("Тип сообщения: %ld, сообщение: %s, код: %d\n", tmp->type, tmp->text, bytesQuantity);
    exit(0);
}