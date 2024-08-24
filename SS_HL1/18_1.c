/*
================================================
Author : Rajan Khade

Description :
Write a program to perform Record locking.
    a. Implement write lock
    b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access to avoid race condition.
================================================
*/

#include <stdio.h>    // printf, perror, scanf, getchar
#include <fcntl.h>    // open, fcntl, O_RDONLY, O_RDWR, F_SETLKW, F_RDLCK, F_WRLCK, F_UNLCK, struct flock
#include <unistd.h>   // close, lseek, read, write, SEEK_SET


struct student {
    int roll_no=-1;
    int marks=-1;
};

void createRecord(int roll_no, int marks) {
    int fd = open("./test_dir_02/test_file_07.txt", O_RDWR | O_CREAT, 0666);
    if(fd == -1) {
        perror("Error opening file");
        return 1;
    }
}

int main()
{

    return 0;
}
