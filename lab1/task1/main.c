#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
extern int errno;
extern const char* const sys_errlist[];


int main(int argc, char * argv[]) {
    int file = open(argv[1], O_RDONLY);
    printf("file descriptor: %d\n", file);
    if (file == -1) {
        printf("ERRNO = %d\n", errno);
        printf("Наша остановочка, ошибка sys_errlist: %s\n", sys_errlist[errno]);
        perror("Ошибочка вышла (PERROR):");
        exit(1);
    }
    exit(0);
}
