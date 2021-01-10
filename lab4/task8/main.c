#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

int flag = 1;

void handler(int signal) {
    if (signal == SIGINT) {
        int status;
        flag = 0;
        wait(&status);
        exit(0);
    }
}


int main(int argc, char * argv[]) {
    int pid;
    int f, l = 1;

    signal(SIGINT, handler);
    pid = fork();
    if (pid) {
        f = open(argv[1], O_RDWR);
        lseek(f,0, SEEK_SET);
        char buf[1];
        while (l) {
            flock(f, LOCK_EX);
            l = read(f, buf, sizeof(buf));
            flock(f, LOCK_UN);
            write(1, buf, sizeof(buf));
        }
        kill(getpid(), SIGINT);
    }
    else {
        f = open(argv[1], O_RDWR);
        lseek(f,0, SEEK_SET);
        while (flag) {
            char buf[2];
            sprintf(buf, "%s", "Z");
            l = write(f, buf, sizeof("Z"));
        }
        exit(0);
    }
}

/*При добавлении слабой блокировки для процесса чтения файл смог получше прочитаться, а потом пошла вся эта писанина*/