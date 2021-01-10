#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

void readStatus(int pid, int extra) {
    int f, n;
    char fileWriteName[40];
    sprintf(fileWriteName, "Status_%d_%d", pid, extra);
    int fw = creat(fileWriteName, 0777);
    char status[256], buf[4096];
    sprintf(status,  "/proc/%d/status", pid);
    f = open(status, O_RDONLY);
    if (f != -1) {
        n = read(f, buf, 4096);
        write(fw, buf, n);
        close(f);
    }
}


//void signalHandler(int someSignal) {
//    int pid, status;
//    printf("Процесс %d, родитель %d\n", getpid(), getppid());
//    printf("Сигнал SIGCHILD (%d) принят к исполнению\n", someSignal);
//    pid = wait(&status);
//    printf("Дочерний процесс %d завершился, статус %d\n", pid, status % 256);
//    readStatus(getpid(), 1);
//}


int main(int argc, char * argv[]) {
    int rc; int f; int n;

    if (argc != 4) {
        fprintf(stderr, "Аргументы должны быть: исполняемый файл, его 2 параметра\n");
        exit(1);
    }
    int pid = fork();
    printf("Процесс (до exec) %d\n", getpid());
    if (!pid) {
        execl(argv[1], argv[1], argv[2], argv[3], NULL);
    }
    else {
        readStatus(pid, 0);
        int new_pid = wait(&rc);
        printf("Статус завершения %d = %d\n", new_pid, rc);
        exit(1);
    }
}

/*Есть 16 бит статуса завершения процесса
 * Старший байт заполняются кодом из exit дочернего процесса
 * Если процесс был завершен по сигналу, только в младший бат записывается номер сигнала*/

