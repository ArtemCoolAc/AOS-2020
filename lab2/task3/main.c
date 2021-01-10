#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int who_earlier() {
    int finishedSonProcessStatus;
    printf("Статус процесса %d на текущий момент: %d\n", getpid(), wait(&finishedSonProcessStatus));
    int pid = fork();
    printf("Процесс: %d, родительский процесс: %d, группа: %d\n", getpid(), getppid(), getpgrp());
    printf("fork показывает: %d\n", pid);
    if (pid) {
        int new_pid = wait(&finishedSonProcessStatus);
        printf("Статус завершенного %d порожденного процесса: %d\n", new_pid, finishedSonProcessStatus);
    }
    exit(0);
}

int main(int argc, char * argv[]){
    who_earlier();
}


/*
 Exit Code Number	Meaning	Example	Comments
1	Catchall for general errors	let "var1 = 1/0"	Miscellaneous errors, such as "divide by zero" and other impermissible operations
2	Misuse of shell builtins (according to Bash documentation)	empty_function() {}	Missing keyword or command, or permission problem (and diff return code on a failed binary file comparison).
126	Command invoked cannot execute	/dev/null	Permission problem or command is not an executable
127	"command not found"	illegal_command	Possible problem with $PATH or a typo
128	Invalid argument to exit	exit 3.14159	exit takes only integer args in the range 0 - 255 (see first footnote)
128+n	Fatal error signal "n"	kill -9 $PPID of script	$? returns 137 (128 + 9)
130	Script terminated by Control-C	Ctl-C	Control-C is fatal error signal 2, (130 = 128 + 2, see above)
255*	Exit status out of range	exit -1	exit takes only integer args in the range 0 - 255*/
