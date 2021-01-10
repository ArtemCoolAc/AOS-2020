#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
extern int errno;
extern const char* const sys_errlist[];

int main(int argc, char * argv[]) {
    struct stat st;
    if (stat(argv[1], &st) == -1) {
        fprintf(stderr, "%s: %s\n", argv[1], sys_errlist[errno]);
        exit(0);
    }

    printf("Device=%lu Inode=%lu Mode=0%o\n", st.st_dev, st.st_ino, st.st_mode);
    printf("Links=%lu UID=%d GID=%d\n", st.st_nlink, st.st_uid, st.st_gid);
    printf("TypeDevice=0x%lx | %lu Size=%ld\n", st.st_rdev, st.st_rdev, st.st_size);
    printf("Size of block=%ld Count of blocks=%ld\n", st.st_blksize, st.st_blocks);
    printf("atime=%s", ctime(&st.st_atime));
    printf("mtime=%s", ctime(&st.st_mtime));
    printf("ctime=%s", ctime(&st.st_ctime));
    exit(0);
}

//struct stat {
//    dev_t     st_dev;         /* ID устройства с файлом */
//ino_t     st_ino;         /* номер inode */
//mode_t    st_mode;        /* права доступа */
//nlink_t   st_nlink;       /* кол-во жёстких ссылок */
//uid_t     st_uid;         /* ID пользователя-владельца */
//gid_t     st_gid;         /* ID группы-владельца */
//dev_t     st_rdev;        /* ID устройства (если это спец. файл) */
//off_t     st_size;        /* полный размер в байтах */
//blksize_t st_blksize;     /* размер блока ввода-вывода
//                                 файловой системы */
//blkcnt_t  st_blocks;      /* кол-во выделенных блоков по 512Б */
//struct timespec st_atim;  /* время последнего доступа */
//struct timespec st_mtim;  /* время последнего изменения */
//struct timespec st_ctim;  /* время последней смены состояния */
//#define st_atime st_atim.tv_sec      /* для обратной совместимости */
//#define st_mtime st_mtim.tv_sec
//#define st_ctime st_ctim.tv_sec
//};
