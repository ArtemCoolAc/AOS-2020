#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
extern int errno;
extern const char* const sys_errlist[];


int main() {
    printf("Hello, World!\n");
    int file = open("somefile", O_RDONLY);
    printf("%d", errno);
    for (int i=0; i<sys_nerr; i++) {
        printf("%d: %s \n", i, sys_errlist[i]);
    }
    return 0;
}
