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
    printf("Я вообще вошел сюда?");
    printf("__________________________________________\n");
    printf("Опущенный процесс %d, родитель: %d, группа: %d\n", getpid(), getppid(), getgid());
    printf("Аргументы процесса %d:\n", getpid());
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Аргумент %d: %s\n", i, argv[i]);
    }
    printf("Окружение процесса %d:\n", getpid());
    for (int i = 0; envp[i]; i++) {
        printf("%d (%d) : %s\n", getpid(), getgid(), envp[i]);
    }
    char * mode = argv[1];
    char* symbol = !strcmp(mode, "1") ? "+": "-";
    for (int i = 0; i < 10; i++) {
        printf("%s ", symbol);
    }
    printf("\nПроцесс %d отработал (группа %d)!\n", getpid(), getgid());
    exit(0);
}
