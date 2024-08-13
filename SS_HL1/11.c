/*
================================================
Author : Rajan Khade

Description :
Write a program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not.
	a. use dup
	b. use dup2
	c. use fcntl
================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int fd = open("./test_dir_01/test_file_03.txt", O_RDWR | O_CREAT, 0666);
	if(fd == -1) {
		perror("Error opening file");
		return 1;
	}

	int fd_dup = dup(fd);
	if(fd == -1) {
		perror("Error duplicating fd");
		return 1;
	}

	char buff1[] = "line1\n";
	int w1 = write(fd, buff1, strlen(buff1));
	if(w1 == -1) {
		perror("Error writing data using fd");
		return 1;
	}

	// lseek(fd, -1, SEEK_END);

	char buff2[] = "line2";
	int w2 = write(fd_dup, buff2, strlen(buff2));
	if(w2 == -1) {
		perror("Error writing data using fd_dup");
		return 1;
	}

	close(fd);
	close(fd_dup);

	return 0;
}
