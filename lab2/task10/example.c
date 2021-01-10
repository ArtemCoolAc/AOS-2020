#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int status;
    char command[100];
    if (argc > 5) {
        fprintf(stderr, "Слишком много аргументов");
    }
    if (strcmp(argv[1], "-system") && strcmp(argv[1], "-exec")) {
        fprintf(stderr, "Первым аргументом необходимо задать опцию в формате: -exec или -system\n");
        fprintf(stderr, "По '-exec' вызов через команды семейства exec, по '-system' выполнение через функцию system\n");
    }
    int pid = fork();
    if (!pid) {
        if (!strcmp(argv[1], "-exec")) {
            execl(argv[2], argv[3], argv[4], NULL);
        }
        else if (!strcmp(argv[1], "-system")) {
            sprintf(command, "%s %s", argv[2], argv[4]);
            system(command);
        }
    }
    else {
        wait(&status);
        exit(0);
    }
}

/* Режим system: ./example -system /bin/ls ls -l
 * Режим exec: ./example -exec /bin/ls ls -l
 * */
