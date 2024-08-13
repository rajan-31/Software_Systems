/*
================================================
Author : Rajan Khade

Description :
Write a program to find out the opening mode of a file. Use fcntl.
================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("./test_dir_01/test_file_03.txt", O_RDWR);
    if(fd == -1) {
		perror("Error opening file");
		return 1;
	}


	// F_GETFL: Return  (as  the function result) the file access mode
	// and the file status flags
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("Error getting file flags");
        return 1;
    }

    printf("flags: %d\n", flags);
    printf("O_ACCMODE: %d\n", O_ACCMODE);

    // extract specific bits | O_ACCMODE: 3, flags (based on mode):32770
    int accessMode = flags & O_ACCMODE;
    printf("accessMode: %d\n\n", accessMode);

    printf("File is opened in ");
    switch (accessMode) {
        case O_RDONLY:
            printf("read-only mode\n");
            break;
        case O_WRONLY:
            printf("write-only mode\n");
            break;
        case O_RDWR:
            printf("read-write mode\n");
            break;
        default:
            printf("unknown mode\n");
            break;
    }

    close(fd);
    return 0;

	return 0;
}
