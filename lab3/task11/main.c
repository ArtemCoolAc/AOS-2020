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


static pid_t pid;
static int counter = 0;
static void parentHandler(int unused) {
    counter += 2;
    printf("counter = %d\n", counter);
}
static void childHandler(int unused) {
    counter += 1;
    printf("counter = %d\n", counter);
    kill(getppid(), SIGUSR1);
}

int main(int argc, char *argv[]) {
    signal(SIGUSR1, parentHandler);
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    if ((pid = fork()) == 0) {
        signal(SIGUSR1, childHandler);
        sigemptyset(&mask);
        sigsuspend(&mask);
        return 0;
    }
    kill(pid, SIGUSR1);
    waitpid(pid, NULL, 0);
    //counter += 3;
    printf("counter = %d\n", counter);
    return 0;
}

/*3 Outputs:
Output 1. - The child prints 2, and both the child and parent deadlock.
Output 2. - The child prints 1, the parent prints 2, and both the child and parent deadlock.
Output 3. - The child prints 1, the parent prints 2, and then the parent prints 5. Both processes exit.

 • sigsuspend(&mask):
//ATOMICALLY:
 sigset_t old;
 sigprocmask(SIG_SETMASK, &mask, &old);
 sleep(); // wait for signal to wake us up
 sigprocmask(SIG_SETMASK, &old, NULL)
 * */