/*
================================================
Author : Rajan Khade

Description :
Write a program to copy file1 into file2 ($cp file1 file2).
================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int fd1 = open("test_dir_01/test_file_01.txt", O_RDONLY);
	int fd2 = open("test_dir_02/test_file_04.txt", O_EXCL | O_WRONLY | O_CREAT, 0666);

	if(fd1 == -1 || fd2 == -1) {
		perror("Failed to open/create files!");
		return 1;
	}

	int BUFF_SIZE = 5;
	char buff[BUFF_SIZE];
	int numberOfBytesRead = read(fd1, buff, BUFF_SIZE);

	while(numberOfBytesRead > 0) {
		if(write(fd2, buff, numberOfBytesRead) != numberOfBytesRead) {
			perror("Failed to write to file!");
			close(fd1);
			close(fd2);
			return 1;
		}
		numberOfBytesRead = read(fd1, buff, BUFF_SIZE);
	}

	if(numberOfBytesRead == -1)
		perror("Failed to read file!");

	close(fd1);
	close(fd2);

	return 0;
}
