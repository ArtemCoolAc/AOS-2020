#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main() {
    char fileName[30], buf[30], mode[10];
    printf("Hello, World!\n");
    printf("Введите имя файла\n");
    char *rights;
    int p, f;
    scanf("%s", fileName);
    printf("Введите права доступа\n");
    scanf("%s", mode);
    p = strtol(mode, &rights, 8);
    printf("%s", rights);
    f = creat(fileName, p);
    if (f == -1) {
        perror(fileName);
    }
    char * my_str = "После дыры";
    lseek(f, 100, SEEK_END);
    write(f, my_str, strlen(my_str));
    my_str = "Промежуточный1";
    lseek(f, 30, SEEK_SET);
    write(f, my_str, strlen(my_str));
    lseek(f, 20, SEEK_CUR);
    my_str = "АЗАЗА";
    write(f, my_str, strlen(my_str));
    close(f);
    exit(0);
}

/* Сначала пишем за 100 символов после конца файла. Есть дыра длиной 100 (текст со 101 символа)
 * Затем перемещаем указатель на 30 символ после начала, пишем на 14 символов, а потом оказывается, что
 * что последний текст оказался на 101-14=87 символе, при этом позиционируя на 31+14+20=65 символе по CUR,
 * последнее сообщение пишется нормально, но промежуток без предпоследним и последним текстом сократился ещё на 4 единицы
 * Т.е. есть закономерность: промежуток между двумя последними текстами в разреженном файле сокращается на (l-1) единиц
 * */
