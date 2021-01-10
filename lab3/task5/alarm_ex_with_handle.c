#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

int seconds = 5;
int flag = 1;


void handler(int mySignal) {
    printf("Куку\n");
    seconds -= 1;
    alarm(seconds);
    if (!seconds) {
        flag = 0;
    }
}


int main(int argc, char * argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Неправильный формат, передаем количество итераций i и j\n");
    }
    printf("_________________________________\n");
    printf("Я процесс %d, мой родитель %d\n", getpid(), getppid());
    signal(SIGALRM, handler);
    alarm(seconds);
    while (flag);
    exit(2);
}