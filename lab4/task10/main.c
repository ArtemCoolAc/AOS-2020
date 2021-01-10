#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

int readFunc() {
    char buf[2]; int l;
        //printf("\nПроцесс %d читает:\n", getpid());
        while ((l = read(0, buf, 2)) > 0) {
            sleep(1);
            write(1, buf, 2);
            printf("PID=%d\n", getpid());
        }
        exit(0);
    }


int main() {
    int pid = fork();
    readFunc();
//    int pid = fork();
//    if (pid) {
//        //readFunc();
//    }
//    else {
//        int l; char buf[4000];
//        //l = read(0, buf, sizeof(buf));
//        //write(1, buf, sizeof(buf));
//        //readFunc();
//    }
//    exit(0);
}