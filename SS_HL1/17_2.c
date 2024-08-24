/*
================================================
Author : Rajan Khade

Description :
Write a program to simulate online ticket reservation. Implement write lock Write a program to open a file, store a ticket number and exit. Write a separate program, to open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.
================================================
*/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main() {
  struct flock lock;
  int fd=open("test_dir_02/test_file_06.txt", O_RDWR);
  if(fd==-1){
    perror("Error opening file");
    return 1;
  }

  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;

  int ticket_no;

  /* ============================ */

  lock.l_type=F_RDLCK;
  printf("Read lock set\n");
  fcntl(fd, F_SETLKW, &lock);

  read(fd, &ticket_no, sizeof(int));

  lseek(fd, -1*sizeof(int), SEEK_CUR);

  printf("Current ticket no: %d\n\n", ticket_no);

  /* ============================ */

  ticket_no++;

  lock.l_type=F_WRLCK;
  fcntl(fd, F_SETLKW, &lock);
  printf("Write lock set\n");

  write(fd, &ticket_no, sizeof(int));

  lseek(fd, -1*sizeof(int), SEEK_CUR);

  read(fd, &ticket_no, sizeof(int));
  printf("New ticket no: %d\n\n", ticket_no);

  printf("Press Enter to release write lock...");
  getchar();

  lock.l_type=F_UNLCK;
  fcntl(fd, F_SETLKW, &lock);

  /* ============================ */

  close(fd);

  return 0;
}
