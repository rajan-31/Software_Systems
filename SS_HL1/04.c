/*
================================================
Author : Rajan Khade

Description :
Write a program to open an existing file with read write mode. Try O_EXCL flag also
================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	//  O_EXCL ensures that the file is created exclusively (i.e., it fails if the file already exists).
	int fd = open("./test_dir_02/test_file_03.txt", O_EXCL|O_CREAT|O_RDWR, 0666);
	if(fd==-1) {
		perror("Failed to open file!");
        return 1;
	}
	else
		printf("File opened successfully!\n");

	close(fd);

	return 0;
}
