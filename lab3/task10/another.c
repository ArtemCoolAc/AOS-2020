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

void handler(int mySignal) {
    sigset_t mask;
    printf("Начало обработчика сигнала %d", mySignal);
    for (int i = 0; i < 5000; i++) {
        for (int j = 0; j < 5000; j++);
    }
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    printf("Конец обработчика сигнала %d", mySignal);
}


int main() {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGUSR1);
    //sigprocmask(SIG_BLOCK, &action.sa_mask, NULL);
    action.sa_flags = SA_NODEFER;
    sigaction(SIGQUIT, &action, NULL);
    for (;;);
}