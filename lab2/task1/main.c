#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

extern int errno;
extern char ** environ;

int main(int argc, char * argv[], char * envp[]){
    int i;
    printf("ПЕРЕД добавлением переменных окружения\n");
    for (i=0; envp[i]; i++)
        printf("%s\n", envp[i]);
    printf("___________________________________\n");
    putenv("A=Somehing1");
    putenv("B=Anything2");
    printf("После добавления\n");
    for (i=0; envp[i]; i++)
        printf("%s\n", envp[i]);
    printf("После добавления переменных окружения\n");
    for (i=0; environ[i]; i++)
        printf("%s\n", environ[i]);
    exit(0);
}

