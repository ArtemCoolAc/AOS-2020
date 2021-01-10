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

#define QUANTITY 4

struct message {
    long type;
    char text[150];
};


int main(int argc, char * argv[]) {
    int qid;
    struct msqid_ds qstatus;
    if (argc != 2) {
        fprintf(stderr, "Необходим ID для генерации ключа\n");
    }
    int proj = atoi(argv[1]);
    if (!proj && strcmp(argv[1], "0")) {
        fprintf(stderr, "Не число передано\n");
        exit(1);
    }
    printf("proj=%d\n", proj);
    //int proj = atoi(argv[1]);
    key_t key = ftok(argv[0], proj);
    qid = msgget(key, IPC_CREAT | 0600);
    if (qid == -1) {
        perror("msgget failed");
        exit(1);
    }
    char* stTexts[] = {"AmwayDallas", "LebwaInspirer", "YushaVspishka", "NearGranny"};
    //char* myTexts[QUANTITY] = {NULL, NULL, NULL, NULL};
    struct message* messages[QUANTITY] = {NULL, NULL, NULL, NULL};
    for (int i = 0; i < QUANTITY; i++) {
        //myTexts[i] = (char *) malloc(sizeof(stTexts[i]));
        //sprintf(myTexts[i], "%s", stTexts[i]);
        messages[i] = (struct message *) malloc(sizeof(struct message));
        messages[i]->type = i + 1;
        strcpy(messages[i]->text, stTexts[i]);
        //messages[i]->text = myTexts[i];
        printf("Текст сообщения %d : %s\n", i, messages[i]->text);
        printf("Отладочное: size text = %lu\n", strlen(messages[i]->text));
        if (msgsnd(qid, messages[i], strlen(messages[i]->text), 0) == -1) {
            char * ptr = (char *) malloc(strlen("Сообщение n послано, но что-то пошло не так"));
            sprintf(ptr, "Сообщение %d послано, но что-то пошло не так", i);
            perror(ptr);
        }
    }

    if (msgctl(qid, IPC_STAT, &qstatus) < 0) {
        perror("msgctl failed");
        exit(1);
    }
    printf("Real user id of the queue creator: %d\n", qstatus.msg_perm.cuid);
    printf("Real group id of the queue creator: %d\n", qstatus.msg_perm.cgid);
    printf("Effective user id of the queue creator: %d\n", qstatus.msg_perm.uid);
    printf("Effective group id of the queue creator: %d\n", qstatus.msg_perm.gid);
    printf("Permissions: %d\n", qstatus.msg_perm.mode);
    printf("Message queue id: %d\n", qid);
    printf("%lu message(s) on queue\n", qstatus.msg_qnum);
    printf("Last message sent by process :%3d at %s \n", qstatus.msg_lspid, ctime(& (qstatus.msg_stime)));
    printf("Last message received by process :%3d at %s \n", qstatus.msg_lrpid, ctime(& (qstatus.msg_rtime)));
    printf("Current number of bytes on queue %lu\n", qstatus.msg_cbytes);
    printf("Maximum number of bytes allowed on the queue %lu\n", qstatus.msg_qbytes);
    for (int i = 0; i < QUANTITY; i++) {
        //free(myTexts[i]);
        free(messages[i]);
    }
    return 0;
}
