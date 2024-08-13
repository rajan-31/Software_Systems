/*
================================================
Author : Rajan Khade

Description :
Write a program to find the type of a file.
	a. Input should be taken from command line.
	b. program should be able to identify any type of a file.
================================================
*/

#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "One argument expected given %d\n", argc-1);
		return 1;
	}

	char const *filename = argv[1];
	struct stat fileStat;

	// Get file status
    if (stat(filename, &fileStat) == -1) {
        perror("Error");
        return 1;
    }

    printf("File type of '%s': ", filename);

    switch(fileStat.st_mode & S_IFMT) {
	    case S_IFREG:
	        printf("Regular file\n");
	    	break;
		case S_IFDIR:
	        printf("Directory\n");
	    	break;
		case S_IFLNK:
	        printf("Symbolic link\n");
	    	break;
		case S_IFCHR:
	        printf("Character device\n");
	    	break;
		case S_IFBLK:
	        printf("Block device\n");
	    	break;
		case S_IFIFO:
	        printf("FIFO/pipe\n");
	    	break;
		case S_IFSOCK:
	        printf("Socket\n");
	    	break;
	    default:
        	printf("Unknown file type\n");
    }

	return 0;
}
