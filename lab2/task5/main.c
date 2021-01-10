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
    int finishedSonProcessStatus;
    printf("Статус процесса %d (его родитель %d) на текущий момент: %d\n", getpid(), getppid(), wait(&finishedSonProcessStatus));
    int pid = fork();
    printf("Процесс: %d, родительский процесс: %d, группа: %d\n", getpid(), getppid(), getpgrp());
    printf("fork показывает: %d\n", pid);
    if (pid) {
        //int new_pid = wait(&finishedSonProcessStatus);
        sleep(2);
        system("ps -fa");
        exit(0);
        //printf("Статус завершенного %d порожденного процесса: %d\n", new_pid, finishedSonProcessStatus);
    }
    else {
        printf("Я сын. Мой ID %d, отца ID %d\n", getpid(), getppid());
        exit(12);
    }
    //exit(0);
}

int main(int argc, char * argv[]){
    who_earlier();
}

/*
 * Когда дочерний процесс получает команду на завершение, он освобождает все используемые ресурсы
 * и продолжает как зомби существовать в системе с ранее присвоенным ему PID.
 * Далее с помощью сигнала SIGCHLD система уведомляет родительский процесс о завершении зомби процесса.
 * Если по какой-либо причине родительский процесс игнорирует этот сигнал,
 * то зомби процесс так и продолжает отображаться в системе.*/

