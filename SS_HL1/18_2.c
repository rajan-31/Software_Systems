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

int rlock(int l);
int wlock(int l);

struct student {
    int roll_no;
    int marks;
};

int rlock(int l) {
    int fd=open("test_dir_02/test_file_07.txt", O_RDONLY);
    if(fd==-1) {
        perror("Error opening file");
        return 1;
    }

    struct student st;

    lseek(fd, (l - 1) * sizeof(struct student), SEEK_SET);

    read(fd, &st, sizeof(struct student));
    printf("Roll No: %d \t Marks : %d \n", st.roll_no, st.marks);

    struct flock lock;
    lock.l_type=F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start =  (l - 1 ) * sizeof(struct student);
    lock.l_len = sizeof(struct student);

    printf("Waiting to acquire lock on record %d \n", st.roll_no);
    fcntl(fd, F_SETLKW, &lock);

    printf("Acquired lock on record %d \n", st.roll_no);
    printf("Press return to exit\n");

    getchar();
    getchar();

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
    printf("Exited critical section\n");
}


int wlock(int l) {
    int fd=open("test_dir_02/test_file_07.txt", O_RDWR);
    if(fd==-1) {
        perror("Error opening file");
        return 1;
    }

    struct student st;

    lseek(fd, (l- 1) * sizeof(struct student), SEEK_SET);

    read(fd, &st, sizeof(struct student));
    printf("Roll No: %d \n", st.roll_no);
    printf("Marks : %d\n",st.marks);

    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(l- 1) * sizeof(struct student);
    lock.l_len = sizeof(struct student);

    printf("Waiting to acquire lock on record %d \n", st.roll_no);
    lock.l_type=F_WRLCK;
    int r=fcntl(fd, F_SETLKW, &lock);
    if(r < 0){
      perror("Error\n");
      return 1;
    }

    printf("Acquired lock on record %d \n", st.roll_no);
    printf("You selected to write on this record. \nEnter new marks: ");
    int marks;
    scanf("%d", &marks);
    st.marks=marks;

    lseek(fd, -1 * sizeof(struct student), SEEK_CUR);
    write(fd, &st, sizeof(struct student));

    printf("Roll No: %d \n", st.roll_no);
    printf("Marks : %d\n",st.marks);

    printf("Press return to exit\n");
    getchar();
    getchar();

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
    printf("Exited critical section\n");
}

int main() {
    int fd = open("test_dir_02/test_file_07.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if(fd==-1) {
        perror("Error opening file");
        return 1;
    }

    int num_records=2;
    printf("How many records do you want? ");
    scanf("%d", &num_records);
    srand(time(0));

    struct student st;
    for(int i=0; i<=num_records-1; i++) {
        st.roll_no=100+i, st.marks=rand()%101;
        write(fd, &st, sizeof(struct student));
    }

    /* ============================ */

    lseek(fd,0,SEEK_SET);

    for(int i=0; i<=num_records-1; i++) {
        read(fd, &st, sizeof(struct student));
        printf("Roll No.: %d\t", st.roll_no);
        printf("Marks: %d\n", st.marks);
    }

    /* ============================ */

    int l;
    int choice;

    lseek(fd, 0, SEEK_SET);

    printf("Select record you want to lock: ");
    scanf("%d", &l);
    if(l<=0 || l>num_records){
      printf("No such record found\n");
      return 0;
    }

    printf("Select type of lock: Read (1) Write (2) Choice: ");
    scanf("%d", &choice);
    if(choice == 1) {
        rlock(l);
        exit(1);
    }
    else if(choice == 2) {
        wlock(l);
        exit(1);
    }
    else {
        printf("Wrong choice!");
    }

    close(fd);

    return 0;
}
