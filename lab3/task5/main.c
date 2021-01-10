#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


int main(int argc, char * argv[]) {
    int rc; int f; int n;
    char * endPtr;

    if (argc < 4) {
        fprintf(stderr, "Аргументы должны быть: исполняемый файл, его 2 параметра, опционально номер сигнала\n");
        exit(1);
    }
    if (argc > 6 || strcmp(argv[4], "-signal")) {
        fprintf(stderr, "Формат: файл i_max j_max -signal number");
        exit(3);
    }
    int pid = fork();
    printf("Процесс (до exec) %d\n", getpid());
    if (!pid) {
        execl(argv[1], argv[1], argv[2], argv[3], NULL);
    }
    else {
        sleep(1);
        kill(pid, strtol(argv[5], &endPtr, 10));
        int new_pid = wait(&rc);
        printf("Статус завершения %d = %d\n", new_pid, rc);
        exit(1);
    }
}

/*Дефолтная диспозиция SIGUSR1 - завершить процесс, поэтому ни одной итерации и не выполнится
 * Выводится № SIGUSR1 (10)
 * При изменении сигнала, где диспозиция по умолчанию - завершение процесса, завершит, в других случаях что-то другое
 * будет делать.
 *
 * При использовании pause в параллельном процессе, функция будет ждать обработчик полученного сигнала.
 * Те сигналы, которые имеют диспозицию по умолчанию на завершение процесса - завершат его, остальные просто оставят
 * его спящим (проигнорируют по факту). Завершение процесс можно через Ctrl+C или послав сигнал с диспозицией
 * завершения процесса.
 *
 * Если в порожденный процесс поставить обработчик сигнала такой, что он изменяет внешний лимит цикла, то поведение
 * такое : если до отправки сигнала процесс успеет отработать, то все как обычно, а если не успеет, тогда будет
 * работать по переустановленному внешнему лимиту.
 *
 * Alarm без обработчика - диспозиция завершения по умолчанию.
 * ./t5 alarm_ex_no_handle 50000 150000 -signal 3
 *
 * Alarm с обработчиком работает нормально
 * ./t5 alarm_ex_with_handle 50000 150000 -signal 3

 *
 * ./t5 файл 5000 150000 -signal 10

 * */