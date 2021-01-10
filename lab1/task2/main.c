#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


extern int errno;
extern const char* const sys_errlist[];


int main(int argc, char * argv[]) {
    char fileName[30], buf[30], mode[10];
    char * rights;
    int p, f;
    if (argc < 3) {
        printf("Hello, World!\n");
        printf("Введите имя файла\n");
        scanf("%s", fileName);
        printf("Введите права доступа\n");
        scanf("%s", mode);
        p = strtol(mode, &rights, 8);
        printf("%s", rights);
        f = creat(fileName, p);
    }
    if (argc == 3) {
        printf("%s\n", argv[1]);
        printf("%s\n", argv[2]);
        p = strtol(argv[2], &rights, 8);
        f = creat(argv[1], p);
    }
    printf("descriptor: %d\n", f);
    for (int i = 0; i < 6; i++) {
        write(f, "Test\n", 6);
    }
    close(f);
    if (argc < 3) {
        f = open(fileName, O_RDONLY);
    }
    if (argc == 3) {
        f = open(argv[1], O_RDONLY);
    }
    printf("OPEN descriptor: %d\n", f);
    for (int i = 0; i < 6; i++) {
        read(f, buf, 6);
        printf("%s", buf);
    }
    close(f);
    if (argc < 3) {
        f = open(fileName, O_RDWR);
    }
    if (argc == 3) {
        f = open(argv[1], O_RDWR);
    }
    printf("OPEN READ_WRITE descriptor: %d\n", f);
    exit(0);
}

/* №2. в первом проходе все в порядке: хоть мы и создали файл с режимом только чтение, получили 1-ый свободный
 * дескриптор, это объясняется тем, что creat открывает файл на запись, соответственно, пока имеем право туда
 * записывать. Когда открыли на чтение, имели право по READONLY режиму.*/

/* №3. При повторном запуске программы при попытке открыть файл с правами ТОЛЬКО на чтение (т.е. перезаписать его, что вызов
 * creat бы сделал), получим -1. При этом в режиме на чтение дескриптор получим, а вот режим чтение + запись
 * не сработает, так как права только на чтение*/