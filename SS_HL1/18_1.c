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
#include <stdlib.h>   // exit, rand, srand
#include <time.h>


struct student {
    int roll_no;
    int marks;
};

int main()
{
    int fd = open("test_dir_02/test_file_07.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if(fd==-1) {
        perror("Error opening file");
        return 1;
    }

    int num_records=5;
    printf("How many records do you want to generate (atleast 5)? ");
    scanf("%d", &num_records);

    if(num_records < 5)
        num_records = 5;

    srand(time(0));

    struct student st;
    for(int i=0; i<=num_records-1; i++) {
        st.roll_no=1+i, st.marks=rand()%101;
        write(fd, &st, sizeof(struct student));
    }

    printf("\nPrinting records from file: \n\n");

    lseek(fd,0,SEEK_SET);
    for(int i=0; i<=num_records-1; i++) {
        read(fd, &st, sizeof(struct student));
        printf("Roll No.: %d\t", st.roll_no);
        printf("Marks: %d\n", st.marks);
    }

    return 0;
}
