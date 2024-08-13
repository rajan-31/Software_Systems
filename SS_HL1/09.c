/*
================================================
Author : Rajan Khade

Description :
Write a program to print the following information about a given file.
	a. inode
	b. number of hard links
	c. uid
	d. gid
	e. size
	f. block size
	g. number of blocks
	h. time of last access
	i. time of last modification
	j. time of last change
================================================
*/

#include <stdio.h>	// fprintf, perror
#include <stdlib.h>	// EXIT_FAILURE
#include <sys/stat.h>	// stat
#include <pwd.h>	// getpwuid
#include <grp.h>	// getgrgid
#include <time.h>	// ctime


int main(int argc, char const *argv[])
{
	if(argc != 2) {
		fprintf(stderr, "Only 1 argument expected given %d, %s\n", argc-1, argv[0]);

		//in main when you want to return control to the calling process and possibly allow for cleanup after the return
		return EXIT_FAILURE;
	}

	const char *filename = argv[1];
	struct stat file_info;

	if(stat(filename, &file_info) == -1) {
		perror("Error");

		//if you want to terminate the program immediately and ensure that any cleanup functions are executed
		exit(EXIT_FAILURE);
	}


	printf("Inode: %ld\n", file_info.st_ino);
	printf("Hard links count: %ld\n", file_info.st_nlink);


	struct passwd *pw = getpwuid(file_info.st_uid);
	printf("Uid: %d (%s)\n", file_info.st_uid, pw ? pw->pw_name : "Name not available" );

	struct group *gr = getgrgid(file_info.st_gid);
	printf("Gid: %d (%s)\n", file_info.st_gid, gr ? gr->gr_name : "group name not available" );


	printf("Size: %ld bytes\n", file_info.st_size);
	printf("Block size: %ld bytes\n", file_info.st_blksize);
	printf("Number of blocks: %ld\n", file_info.st_blocks);

	printf("Time of last access: %s", ctime(&file_info.st_atime));
	printf("Time of last modification (content): %s", ctime(&file_info.st_mtime));
	printf("Time of last change (metadata): %s", ctime(&file_info.st_ctime));

	return 0;
}
