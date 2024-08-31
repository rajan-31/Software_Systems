/*
================================================
Author : Rajan Khade

Description :
Write a program to create a file and print the file descriptor value. Use creat () system call
================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = creat("./test_dir_02/test_file_02.txt", 0666);
	printf("File descriptor value : %d\n", fd);

	close(fd);
	return 0;
}
