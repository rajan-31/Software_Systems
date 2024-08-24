/*
================================================
Author : Rajan Khade

Description :
Write a program to simulate online ticket reservation. Implement write lock
Write a program to open a file, store a ticket number and exit. Write a separate program, to open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.
================================================
*/

#include <stdio.h>    // perror
#include <stdlib.h>   // exit
#include <unistd.h>   // open, write, close
#include <fcntl.h>    // O_RDWR, O_CREAT, O_TRUNC

int main() {
  // O_TRUNC : Truncates the file to zero length if it already exists
  int fd = open("test_dir_02/test_file_06.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (fd == -1) {
    perror("Error creating/opening file");
    exit(EXIT_FAILURE);
  }

  /* ============================ */

  int ticket_no=0;
  printf("Enter ticket number: ");
  scanf("%d", &ticket_no);
  /* ============================ */

  // ssize_t write(int fd, const void *buf, size_t count);
  if (write(fd, &ticket_no, sizeof(int)) == -1) {
    perror("Error writing to file");
    exit(EXIT_FAILURE);
  }

  close(fd);

  return 0;
}
