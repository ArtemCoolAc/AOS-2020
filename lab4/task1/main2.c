#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char * argv[]) {
    int pid, n, c, p, k, nbread;
    char buf1[12], buf2[12];
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    pid = fork();
    if (pid)
    {
        close(fd1[1]); // канал на чтение в родительском
        close(fd2[0]); // канал на запись в родительском
        read(fd1[0], buf2, sizeof(buf2)); // читаем, что дочерний прислал
        n = atoi(buf2);
        printf("Parent read %d\n", n);
        for (int i = 0; i < n; i++)
        {
            printf("Parent dozes...\n");
            sleep(3); // поспим, в это время ребенок чего-то напишет (пока родитель ждет, ребенок нормально прочитает)
            printf("Parent wakes...\n");
            nbread = read(fd1[0], buf2, sizeof(buf2)); // читаем, что ответил ребенок (он отвечает чиселками из цикла)
            if (nbread == -1)
            {
                fprintf(stderr, "Parent exits after read failure\n");
                exit(1);
            }
            c = atoi(buf2);
            c = c * 2; // мы чиселку умножим на 2 и отправим
            sprintf(buf2, "%d", c);
            write(fd2[1], buf2, sizeof(buf2)); // обработанное число отправили ребенку
            printf("Parent wrote [%s]\n", buf2);
        }
        close(fd1[0]);
        close(fd2[1]);
        printf("Parent done\n");
        exit(0);
    }
    else
    {
        close(fd1[0]); // канал на запись в дочернем
        close(fd2[1]); // канал на чтение в дочернем
        printf("Enter integer: ");
        scanf("%d", &p); // из входного читаем наше число
        sprintf(buf1, "%d", p);
        write(fd1[1], buf1, sizeof(buf1)); // отправляем родительскому это число
        printf("Child wrote [%s]\n", buf1);
        printf("Child dozes...\n");
        sleep(3); // спим пока он обработает (читаем в родительском)
        printf("Child wakes...\n");
        for (int i = 0; i < p; i++) // родитель спит, а ему ему чего-нибудь напишем
        {
            sprintf(buf1, "%d", i);
            write(fd1[1], buf1, sizeof(buf1)); // отправляем родителю чиселку
            printf("Child wrote [%s]\n", buf1);
            read(fd2[0], buf2, sizeof(buf2)); // читаем то что родитель написал (в первый раз ему нечего читать)
            printf("number is: %s\n", buf2);
        }
        close(fd1[1]);
        close(fd2[0]);
        wait(NULL);
    }
    return 0;
}
