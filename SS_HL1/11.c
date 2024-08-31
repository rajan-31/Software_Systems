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

	int fd_dup1 = dup(fd);
	if(fd_dup1 == -1) {
		perror("Error duplicating fd with dup()");
		return 1;
	}

	// dup2(int fd, int fd_num)
	int fd_dup2 = dup2(fd, 97);	// assuming fd. 97 is available
	if(fd_dup2 == -1) {
		perror("Error duplicating fd with dup2()");
		return 1;
	}

	/* ============================ */

	char buff1[] = "line1\n";
	int w1 = write(fd, buff1, strlen(buff1));
	if(w1 == -1) {
		perror("Error writing data using fd");
		return 1;
	}

	char buff2[] = "line2\n";
	int w2 = write(fd_dup1, buff2, strlen(buff2));
	if(w2 == -1) {
		perror("Error writing data using fd_dup1");
		return 1;
	}

	char buff3[] = "line3";
	int w3 = write(fd_dup2, buff3, strlen(buff3));
	if(w3 == -1) {
		perror("Error writing data using fd_dup2");
		return 1;
	}

	close(fd);
	close(fd_dup1);
	close(fd_dup2);

	return 0;
}
