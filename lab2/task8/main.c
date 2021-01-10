#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char * argv[]) {
    int childProcessExitStatus;
    if (argc > 3) {
        fprintf(stderr, "Аргументов должно быть 1 или 2\n");
        exit(1);
    }
    char * mode = (argc == 3) ? argv[2] : "no";
    int file = open(argv[1], O_RDONLY);
    printf("Дескриптор файла = %d\n", file);
    if (file == -1) {
        fprintf(stderr, "Файл не открывается, что-то с ним не так: %s\n", sys_errlist[errno]);
        exit(1);
    }
    lseek(file, 0, SEEK_SET);
    int pid = fork();
    if (pid) {
        char firstName[] = "first";
        char buf1;
        int q1 = 1;
        int f1 = !strcmp(mode, "screen") ? 1 : creat(firstName, 0777);
        while (q1 > 0) {
            q1 = read(file, &buf1, 2);
            write(f1, &buf1, 2);

        }
        close(f1);
        wait(&childProcessExitStatus);
    }
    else {
        char secondName[] = "second";
        char buf2;
        int q2 = 1;
        int f2 = !strcmp(mode, "screen") ? 1 : creat(secondName, 0777);
        while (q2 > 0) {
            q2 = read(file, &buf2, 2);
            write(f2, &buf2, 2);
        }
        close(f2);
    }
    exit(0);
}
