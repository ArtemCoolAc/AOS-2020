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
        sleep(1);
        pause();
    }
    else {
        sprintf(sonStr2, "Процесс: %d, родительский процесс: %d, группа: %d\n", getpid(), getppid(), getpgrp());
        printf("%s", sonStr2);
        write(sonFile, sonStr2, strlen(sonStr2));
        setpgrp();
        sprintf(sonStr2, "Процесс: %d, родительский процесс: %d, группа: %d\n", getpid(), getppid(), getpgrp());
        printf("%s", sonStr2);
        write(sonFile, sonStr2, strlen(sonStr2));
        sleep(1);
        pause();
    }
    exit(0);
}

int main(int argc, char * argv[]) {
    who_earlier();
}

/* setpgrp работает как setpgid(pid=0, pgid=0)
 * pid - ID процесса, который нужно перенести в другую группу, если 0, то переводит самого себя
 * pgid - ID группы, в которую нужно перенести процесс, если 0, то создается группа, которая определяется
 * первым параметром. Если pid=0, pgid=0 - создается группа, лидером которой является вызывающий процесс
 * Когда нажимается клавиша прерывания, будут завершены все процессы группы родительского процесса, а
 * так как дочерний мы уже перевели в свою собственную группу, то он не завершится
 * ps -faj нормально выводит*/
