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

int main(int argc, char * argv[]) {
    struct msqid_ds qstatus;
    if (argc != 2) {
        fprintf(stderr, "Необходимо передать только ID_очереди");
    }
    int qid = atoi(argv[1]);
    if (qid == 0 && strcmp(argv[1], "0")) {
        fprintf(stderr, "Аргумент %s не явлется числом\n", argv[1]);
        exit(1);
    }
    int code = msgctl(qid, IPC_RMID, 0);
    printf("Что получилось: %s\n errno = %d\n", sys_errlist[errno], errno);
    printf("msgctl вернул %d\n", code);
    exit(0);
}