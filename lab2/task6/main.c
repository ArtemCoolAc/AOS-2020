#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int who_earlier() {
    int sonFile = creat("Son", 0777);
    int fatherFile = creat("Father", 0777);
    char sonStr2[100];
    char fatherStr2[100];
    int pid = fork();
    if (pid) {
        sprintf(fatherStr2, "Процесс: %d, родительский процесс: %d, группа: %d\n", getpid(), getppid(), getpgrp());
        printf("%s", fatherStr2);
        write(fatherFile, fatherStr2, strlen(fatherStr2));
        pause();
    }
    else {
        sprintf(sonStr2, "Процесс: %d, родительский процесс: %d, группа: %d\n", getpid(), getppid(), getpgrp());
        printf("%s", sonStr2);
        write(sonFile, sonStr2, strlen(sonStr2));
        pause();
    }
    exit(0);
}

int main(int argc, char * argv[]) {
    who_earlier();
}

/*В фоновом режиме программа оставит в фоне 2 процесса.
 * После убийства родительского процесса, дочерний процесс продолжит существование,
 * но его родительским процессом станет init.
 *В интерактивном режиме терминал будет ждать.
 * По нажатию Ctrl+C завершатся все процессы группы, а так как дочерний процесс
 * входит в группу с родительским, завершатся они оба
 * Если запустить в интерактивном режиме в одном терминале, а убить родительский процесс с другого,
 * то на первом терминале все завершится и достичь данного эффекта не получится, дочерний процесс
 * поменяет родительский процесс на init, который не входит в одну группу с предыдущим родительским*/