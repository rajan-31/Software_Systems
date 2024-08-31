/*
================================================
Author : Rajan Khade

Description :
Write a program to create five new files with infinite loop. Execute the program in the background and check the file descriptor table at /proc/pid/fd.
================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int fd[5];
	char filename[50];

	while(1) {
		for(int i=0; i<=4; i++) {
			snprintf(filename, sizeof(filename), "./test_dir_02/test_file_99%d.txt", i);

			fd[i] = open(filename, O_CREAT | O_WRONLY | O_EXCL, 0666);
			if(fd[i] == -1) {
				printf("%s", filename);
				perror("failed to open file!");
	        	return 1;
			}

			sleep(1);
		}
	}

	return 0;
}
