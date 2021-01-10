#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
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
            l = read(f, buf, sizeof(buf));
            write(1, buf, sizeof(buf));
        }
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