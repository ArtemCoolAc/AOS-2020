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
    char * endPtr;
    int sum = 0;
    printf("_________________________________\n");
    printf("Я процесс %d, мой родитель %d\n", getpid(), getppid());
    int outerCycle = strtol(argv[1], &endPtr, 10);
    int innerCycle = strtol(argv[2], &endPtr, 10);
    for (int i = 0; i < outerCycle; i++) {
        printf("%d \n", i);
        for (int j = 0; j < innerCycle; j++) {
            sum += i;
        }
    }
    printf("%d\n", sum);
    printf("_________________________________\n");
    exit(2);
}