#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char * argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Неправильный формат, передаем количество итераций i и j\n");
    }
    printf("_________________________________\n");
    printf("Я процесс %d, мой родитель %d\n", getpid(), getppid());
    alarm(3);
    while (1);
    exit(2);
}