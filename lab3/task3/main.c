#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

void signalHandler(int someSignal) {
    int pid, status;
    printf("Сигнал SIGCHILD принят к исполнению\n");
    pid = wait(&status);
    printf("Дочерний процесс %d завершился, статус %d\n", pid, status);
}


int main() {
    //char status[256];
    struct sigaction action;
    action.sa_handler = signalHandler;
    //action.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &action, NULL);
    int pid = fork();
    if (!pid) {
        exit(1);
    }
    else {
        sleep(1);
        //sprintf(status, "/proc/%d/status", pid);
    }

}