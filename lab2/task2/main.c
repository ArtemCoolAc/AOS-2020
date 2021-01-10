#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



void who_earlier(int father, int son) {
    int pid;
    char* fileName = "TEST";
    int fd = creat(fileName, 0644);
    if( !(pid = fork())){ /* сын: порожденный процесс */
//        c = 'a';
//        write(fd, &c, 1);
        if (unlink(fileName) != -1) {
            write(son, "Сын\n", 8);
            //printf("Сын закончился, его pid=%d, ppid=%d\n", getpid(), getppid());
        }
        //printf("Сын %d окончен\n", getpid());
        exit(0);
    }
    /* else процесс-отец */
//    c = 'A';
//    write(fd, &c, 1);
    if (unlink(fileName) != -1) {
        write(father, "Отец\n", 10);
        //printf("Отец закончился, его pid=%d\n", getpid());
    }
    //printf("Родитель %d процесса %d окончен\n", getpid(), pid );
}

int who_earlier2() {
    char name[10];
    char parent[] = "Parent\n";
    char son[] = "Son\n";
    sprintf(name, "%d", getpid());
    int file = creat(name, 0777);
    close(file);
    int pid = fork();
    if (unlink(name) == -1) {
        write(1, pid? parent: son, pid? strlen(parent): strlen(son));
    }
    exit(0);
}


int main(int argc, char * argv[]){
//    int writingFather = creat("Father", 0777);
//    int writingSon = creat("Son", 0777);
//    for (int i = 0; i < 10000; ++i) {
//        //who_earlier(writingFather, writingSon)
//    }
//    exit(0);
    who_earlier2();

}
