#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char * argv[], char * envp[]) {
    int status;
    int pid = fork();
    if (pid) {
        printf("Процесс %d\n", getpid());
        for (int i = 0; i < argc; i++) {
            printf("Аргумент %d : %s\n", i, argv[i]);
        }
        printf("Окружение процесса %d:\n", getpid());
        for (int i = 0; envp[i]; i++) {
            printf("%s\n", envp[i]);
        }
        //exit(0);
        wait(&status);
    }
    else {
        execl(argv[1], argv[1], argv[2], argv[3], argv[4], argv[5], NULL);
    }
}

