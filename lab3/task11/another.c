#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

//int count = 0;
int f;
char buf[80];
int interrupted = 0;

void userChildHandler(int mySignal) {
    char * endPtr;
    f = open("resource", O_RDWR);
    read(f, buf, sizeof(buf));
    int count = strtol(buf, &endPtr, 10);
    count += 10;
    printf("Count = %d, процесс %d\n", count, getpid());
    close(f);
    f = open("resource", O_TRUNC);
    sprintf(buf, "%d", count);
    write(f, buf, sizeof(buf));
    close(f);
}

void userParentHandler(int mySignal) {
    char * endPtr;
    f = open("resource", O_RDWR);
    read(f, buf, sizeof(buf));
    int count = strtol(buf, &endPtr, 10);
    count = count * 2 / (float)3;
    printf("Count = %d, процесс %d\n", count, getpid());
    close(f);
    f = open("resource", O_TRUNC);
    sprintf(buf, "%d", count);
    write(f, buf, sizeof(buf));
    close(f);
}


int main() {
    int i = 1000;
    sigset_t mask, oldmask;



    /* Set up the mask of signals to temporarily block. */
    sigemptyset (&mask);
    sigaddset (&mask, SIGUSR1);

    int pid = fork();
    if (!pid) {
        signal(SIGUSR1, userChildHandler);
    }
    else {
        signal(SIGUSR1, userParentHandler);
    }

    /* Wait for a signal to arrive. */
    sigprocmask (SIG_BLOCK, &mask, &oldmask);
    while (!interrupted) {
        sigsuspend(&oldmask);
    }
    sigprocmask (SIG_UNBLOCK, &mask, NULL);
    exit(0);
}