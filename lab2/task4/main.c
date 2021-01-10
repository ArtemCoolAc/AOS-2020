#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int who_earlier() {
    int pid = fork();
    printf("Процесс: %d, родительский процесс: %d, группа: %d\n", getpid(), getppid(), getpgrp());
    printf("fork показывает: %d\n", pid);
    if (pid) {
        sleep(1);
        exit(0);
    }
    else {
        printf("Я сын. Мой ID %d, отца ID %d\n", getpid(), getppid());
        sleep(3);
        printf("Я сын. Мой ID %d, отца ID %d\n", getpid(), getppid());
        //system("ps -fa");
        exit(0);
    }
}

int main(int argc, char * argv[]){
    who_earlier();
}

/*systemd — набор базовых строительных кирпичиков для системы Linux.
 * Он предоставляет диспетчер системы и служб, который выполняется с PID 1 и запускает остальную часть системы.
 * systemd обеспечивает возможности агрессивной параллелизации,
 * использует сокеты и активацию D-Bus для запускаемых служб, предлагает запуск демонов по необходимости,
 * отслеживает процессы при помощи контрольных групп Linux, поддерживает монтирование и точки монтирования,
 * а также внедряет основанную на зависимостях логику контроля процессов сложных транзакций.
 * systemd поддерживает сценарии инициализации SysV и LSB и является заменой sysvinit.*/

