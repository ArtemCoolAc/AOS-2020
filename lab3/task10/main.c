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

sigset_t myset;

void showPending(int mySignal) {
    sigset_t mask;
    sigpending(&mask); // получаем все заблокированные сигналы
    printf("\n2 3 4 5 6 7 8\n");
    for (int i = 2; i < 9; i++)
        printf("%d ", sigismember(&mask, i));
    printf("\n");
}

void clearPending(int mySignal) {
    struct sigaction action;
    action.sa_handler = SIG_IGN; // диспозиция на игронирование
    sigemptyset(&action.sa_mask); // убирает из структуры все сигналы
    action.sa_flags = 0;
    for (int i = 2; i < 9; i++) {
        sigaction(i, &action, NULL);
    }
    sigprocmask(SIG_UNBLOCK, &myset, NULL);
    sigprocmask(SIG_BLOCK, &myset, NULL);
    action.sa_handler = SIG_DFL;
    for (int i = 2; i < 9; i++) {
        sigaction(i, &action, NULL);
    }
}


int main(int argc, char * argv[]) {
    struct sigaction action;
    sigemptyset(&myset);
    for (int i = 2; i < 9; i++) {
        sigaddset(&myset, i);
    }
    action.sa_handler = showPending;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGUSR1, &action, NULL);
    action.sa_handler = clearPending;
    sigaction(SIGHUP, &action, NULL);
    sigprocmask(SIG_SETMASK, &myset, NULL);
    for (;;) pause();
    exit(0);
}
