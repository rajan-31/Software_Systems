/*
================================================
Author : Rajan Khade

Description :
Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
	a. check the return value of lseek
	b. open the file with od and check the empty spaces in between the data.
================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	int fd = open("./test_dir_01/test_file_02.txt", O_RDWR | O_CREAT, 0666);
	if(fd == -1) {
		perror("Error");
		return 1;
	}

	char buff[] = "a12345678a";

	int w = write(fd, buff, sizeof(buff));
	if(w == -1) {
		perror("Error writing data: ");
		return 1;
	}

	printf("Written \"%s\" in the file!\n", buff);

	int offset = 10;
	// move 10 bytes
	int l = lseek(fd, offset, SEEK_CUR);
	if(l == -1) {
		perror("Error writing data: ");
		return 1;
	}

	printf("After moving 10 bytes, value returned by \"lseek\": %d\n", l);

	char buff1[] = "b12345678b";

	w = write(fd, buff1, sizeof(buff1));
	if(w == -1) {
		perror("Error writing data 2nd time: ");
		return 1;
	}

	printf("Written \"%s\" in the file!\n", buff1);

	close(fd);

	return 0;
}
