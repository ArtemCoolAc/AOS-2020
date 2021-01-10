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


int main(int argc, char * argv[]) {
    int pid, l = 1, stat;
    char buf[80];
    int fd[2];
    pipe(fd);
    pid = fork();
    if (pid) {
        close(fd[1]);
        while (l > 0) {
            l = read(fd[0], buf, sizeof(buf));
            write(1, buf, l);
        }
        close(fd[0]);
        wait(&stat);
        exit(0);
    }
    else {
        char arg1[10];
        char arg2[10];
        sprintf(arg1, "%d", fd[0]);
        sprintf(arg2, "%d", fd[1]);
        execl("/home/artemcoolac/CLionProjects/AOS_Labs/lab4/task4/child", "child", arg1, arg2, NULL);
//        close(fd[0]);
//        while ((l = read(0, buf, sizeof(buf))) > 0) {
//            write(fd[1], buf, l);
//        }
//        close(fd[1]);
//        exit(0);
    }
}
