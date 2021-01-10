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


int main() {
    int fd[2];
    char *argv[3];
    int l;
    char buf[1000];
    argv[0] = "wc";
    argv[1] = "-l";
    pipe(fd);
    if( fork() == 0) {
//        close(0); //CHILD CLOSING stdin
//        dup(fd[0]); // copies the fd of read end of pipe into its fd i.e 0 (STDIN)
//        close(1);
//        while ((l = read(fd[0], buf, sizeof(buf))) > 0) {
//        }
//        close(fd[0]);
//        close(fd[1]);
//        execl("/bin/wc", argv[0], argv[1], NULL);
        wait(NULL);
        exit(0);
    }
    else {
        char arg1[10];
        char arg2[10];
        sprintf(arg1, "%d", fd[0]);
        sprintf(arg2, "%d", fd[1]);
        execlp("/home/artemcoolac/CLionProjects/AOS_Labs/lab4/task5/separate","separate", arg1, arg2, NULL);
//        close(1);
//        dup(fd[1]);
//        close(0);
//        execl("usr/bin/who", "who", NULL);
    }
}
