#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


void signalHandler(int someSignal) {
    printf("Сигнал SIGINT принят к исполнению\n");
}


int main() {
    struct sigaction action;
    action.sa_handler = signalHandler;
    action.sa_flags = SA_RESETHAND; // этот файл позволяет после кастомной обработки поставить дефолтную деспозицию
    sigaction(SIGINT, &action, NULL);
    for (;;);
}