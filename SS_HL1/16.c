/*
================================================
Author : Rajan Khade

Description :
Write a program to perform mandatory locking.
 a. Implement write lock
 b. Implement read lock
================================================
*/


#include <stdio.h>    // printf, perror, getchar
#include <stdlib.h>   // exit, EXIT_FAILURE
#include <fcntl.h>    // open, fcntl, O_RDWR, O_CREAT, F_SETLKW, F_SETLK, struct flock
#include <unistd.h>   // close, SEEK_SET


int main() {

    int fd = open("test_dir_02/test_file_05.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error creating/opening file");
        exit(EXIT_FAILURE);
    }

    // used to define file locks, particularly
    struct flock lock;
    lock.l_whence = SEEK_SET;   // How to interpret l_start: SEEK_SET, SEEK_CUR, SEEK_END
    lock.l_start = 0;
    lock.l_len = 0; // Number of bytes to lock (0 means until EOF)

    /* ============================ */

    printf("Waiting to aquire write lock...\n");

    // F_RDLCK, F_WRLCK, F_UNLCK
    lock.l_type = F_WRLCK;
    // F_SETLKW : Set Lock, Wait if necessary
    // F_SETLK : Set Lock, immediately else fail
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error aquiring write lock");
        exit(EXIT_FAILURE);
    }
    printf("Write lock acquired. Press Enter to release it.\n");

    getchar();

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing write lock");
        exit(EXIT_FAILURE);
    }
    printf("Write lock released.\n\n\n");

    /* ============================ */

    printf("Waiting to aquire read lock...\n");

    lock.l_type = F_RDLCK;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error aquiring read lock");
        exit(EXIT_FAILURE);
    }
    printf("Read lock acquired. Press Enter to release it.\n");

    getchar();

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing read lock");
        exit(EXIT_FAILURE);
    }
    printf("Read lock released.\n");

    /* ============================ */

    close(fd);

    return 0;
}
