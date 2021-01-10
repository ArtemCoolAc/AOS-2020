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
    char text[1];
};

void exclusiveRead(struct message * m1, struct message * m2, int qId, int * l, int file, int newFile) {
    printf("ТОЧКА ВХОДА %d\n", getpid());
    char buf[20];
    printf("ID очереди в функции равен %d, PID = %d\n", qId, getpid());
    m1->type = 1;
    int bytes = msgsnd(qId, m1, 1, 0);
    printf("Сообщение type=1 из %d послано\n", getpid());
    if (bytes == -1) {
        fprintf(stderr, "Считано %s, l = %d, qid = %d, m1=%p, m2=%p\n", buf, *l, qId, m1, m2);
        perror("Сообщение о занятости не отправилось");
        //exit(1);
    }
    *l = read(file, buf, 20);
    if (*l) {
        write(newFile, buf, *l);
        printf("Считано %s, l = %d, qid = %d, m1=%p, m2=%p\n", buf, *l, qId, m1, m2);
        m2->type = 2;
        bytes = msgsnd(qId, m2, 1, 0);
        printf("Сообщение type=2 из %d послано\n", getpid());
        if (bytes == -1) {
            perror("Сообщение об освобождении ресурса не отправилось");
            //exit(1);
        }
    }
    printf("ТОЧКА ВЫХОДА %d\n", getpid());
}


int main(int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Должно быть опция - название считываемого файла\n");
        exit(1);
    }
    int file = open(argv[1], 0666);
    lseek(file, 0, SEEK_SET);
    key_t key = ftok(argv[0], 666);
    int qId = msgget(key, IPC_CREAT | 0600);
    printf("ID очереди равен %d\n", qId);
    int newFile = creat("Rewritten_file.txt", 0666);
    int pid = fork();
    int l = 1;
    int nbytes;
    struct message * m1 = (struct message * ) malloc(sizeof(struct message));
    struct message * m2 = (struct message * ) malloc(sizeof(struct message));
    nbytes = msgrcv(qId, m1, 150, 1, IPC_NOWAIT);
    printf("Особая ситуация\n");
    if (nbytes == -1) {
        exclusiveRead(m1, m2, qId, &l, file, newFile);
    }
    else {
        if (m2) {
            msgrcv(qId, m2, 150, 2, 0);
            printf("Сообщение с type=2 в %d было принято\n", getpid());
            exclusiveRead(m1, m2, qId, &l, file, newFile);
        }
    }
    while (l) {
        msgrcv(qId, m1, 150, 1, 0);
        printf("Сообщение с type=1 в %d было принято\n", getpid());
        msgrcv(qId, m2, 150, 2, 0);
        printf("Сообщение с type=2 в %d было принято\n", getpid());
        exclusiveRead(m1, m2, qId, &l, file, newFile);
//        nbytes = msgrcv(qId, m1, 150, 1, IPC_NOWAIT);
//        if (nbytes == -1) {
//            exclusiveRead(m1, m2, qId, &l, file, newFile);
//        }
//        else {
//            if (m2) {
//                msgrcv(qId, m2, 150, 2, 0);
//                exclusiveRead(m1, m2, qId, &l, file, newFile);
//            }
//        }
    }
    if (m1) {
        free(m1);
        m1 = NULL;
    }
    if (m2) {
        free(m2);
        m2 = NULL;
    }
    if (msgctl(qId, IPC_RMID, 0) == -1) {
        perror("Не удалось удалить очередь");
    }
    close(file);
    close(newFile);
    printf("Процесс %d завершается\n", getpid());
    if (pid) {
        wait(0);
    }
    exit(0);
}

/*Идея программы следующая: до форка открывается файл.
 * Необходимо, чтобы процессы читали файл, но не мешали друг другу.
 * Для этого используется очередь сообщений.
 * В самом начале оба процесса пытаются получить сообщение типа 1 (обозначим, что файл занят) без ожидания
 * Процесс, который первым захватит файл, отправит в очередь сообщение типа 1.
 * В это время второй процесс получит сообщение типа 1 и начнет ждать сообщения типа 2
 * (это будет значить, что ресурс свободен).
 * Первый процесс работает с файлом (читает, пишет в другой файл) и отправляет сообщение типа 2.*/