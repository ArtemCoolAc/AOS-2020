#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


void signalHandler(int someSignal) {
    signal(SIGINT, SIG_DFL);
    printf("Сигнал SIGINT принят к исполнению\n");
}


int main() {
    signal(SIGINT, signalHandler);
    for (;;);
}

/* В первый раз вызовется наш кастомный обработчик, он установит
 * диспозицию по умолчанию, затем при следующем сигнале SIGINT программа завершится
 * */