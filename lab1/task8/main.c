#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    int fin, fout, l;
    char buf[4096];
    if (argc > 3) {
        fprintf(stderr, "Usage: %s filein fileout\n", argv[0]);
        exit(1);
    }
    fin = 0;
    fout = 1;
    if (argc > 2)
        if ((fout = creat(argv[2], 0777)) == -1) {
            perror(argv[2]);
            exit(1);
        }
    if (argc > 1)
        if ((fin = open(argv[1], O_RDONLY)) == -1) {
            perror(argv[1]);
            exit(1);
        }
    char name1[20], name2[20];
    if (argc == 1) {
        printf("Введите имя копируемого файла\n");
        scanf("%s", name1);
        printf("Введите имя файла, в который копируем\n");
        scanf("%s", name2);
        if ((fin = open(name1, O_RDONLY)) == -1) {
            perror(name1);
            exit(1);
        }
        if ((fout = creat(name2, 0777)) == -1) {
            perror(name2);
            exit(1);
        }
    }
    while ((l = read(fin, buf, 4096)) > 0)
        write(fout, buf, l);
    close(fin);
    close(fout);
    exit(0);
}
