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
    close(fd[0]);
    while ((l = read(0, buf, sizeof(buf))) > 0) {
        write(fd[1], buf, l);
    }
    close(fd[1]);
    exit(0);
}