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
    int l;
    char buf[80];
    int fd[2];
    if (argc != 3) {
        fprintf(stderr, "Передаются 2 дескриптора\n");
    }
    fd[0] = atoi(argv[1]);
    fd[1] = atoi(argv[2]);
    printf("fd0=%d, fd1=%d\n",fd[0], fd[1]);
    int pid = fork();
    if (pid) {
        close(0); //CHILD CLOSING stdin
        dup(fd[0]); // copies the fd of read end of pipe into its fd i.e 0 (STDIN)
        printf("fd[0] после дап %d\n", fd[0]);
        close(fd[1]);
        while ((l = read(fd[0], buf, sizeof(buf))) > 0) {
        }
        execlp("/bin/wc", "wc", "-l", NULL);
        //system("wc -l");
        wait(NULL);
        exit(0);
    }
    else {
        //close(1);
        dup2(fd[1], 1);
        printf("fd[1] после дап %d\n", fd[1]);
        close(fd[0]);
        execlp("/bin/ls", "ls", "-l", NULL);
        //system("who");
        //execl("usr/bin/who", "who", "-s", NULL);
        exit(0);
    }
}