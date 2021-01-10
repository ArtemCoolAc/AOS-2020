#define _GNU_SOURCE
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


int main(int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Аргумент должен быть ровно 1\n");
        exit(1);
    }
    if (strcmp(argv[1], "1") && strcmp(argv[1], "2")) {
        fprintf(stderr, "Нужно только '1' или '2'\n");
        exit(1);
    }
    int pid;
    int l, l2;
    char buf[80];
    char newBuf[80];
    int fromChildToParent[2];
    int fromParentToChild[2];
    pipe2(fromChildToParent, 0);
    pipe2(fromParentToChild, 0);
    pid = fork();
    printf("Процесс %d\n", getpid());
    if (pid) {
        close(fromChildToParent[1]); // закрыли дескриптор на запись
        close(fromParentToChild[0]); // закрыли дескриптор на чтение
        char * newStr = (char *) malloc(2 * sizeof(char));
        while ((l = read(fromChildToParent[0], buf, sizeof(char))) > 0) {
            printf("Зайдем ли мы сюда после ввода? Это родитель\n");
            sprintf(newStr, "%c%c", buf[0], buf[0]);
            printf("%s\n", newStr);
            write(fromParentToChild[1], newStr, 2 * l);
        }
        close(fromChildToParent[0]);
        close(fromParentToChild[1]);
        printf("Закрыли все каналы в родителе\n");
        wait(NULL);
        exit(0);
    }
    else {
        // l = 1;
        close(fromChildToParent[0]); // закрыли дескриптор на чтение, на запись оставили
        close(fromParentToChild[1]); // закрыли дескрпитор на запись, на чтение оставили
        while ((l = read(0, buf, strlen(buf))) > 0) {
            printf("Мы зашли к ребенку, строка: %s\n", buf);
            write(fromChildToParent[1], buf, l);
        }
        printf("А он сюда вообще заходит (ребенок)? %d\n", getpid());
        while ((l2 = read(fromParentToChild[0], newBuf, 2 * sizeof(char ))) > 0) {
            write(1, newBuf, l2);
        }
        close(fromChildToParent[1]);
        close(fromChildToParent[0]);
        printf("Все дескрипторы закрыты в сыне\n");
        exit(0);
    }
    exit(0);
}

/*Первый канал : родитель пишет, сын читает
 * Второй канал : родитель читает, сын пишет*/