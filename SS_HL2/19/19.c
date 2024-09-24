/*
================================================
Author : Rajan Khade

Description :
Create a FIFO file by
	a. mknod command
	b. mkfifo command
	c. use strace command to find out, which command (mknod or mkfifo) is better.
	c. mknod system call
	d. mkfifo library function
================================================
*/

#include <stdio.h>
#include <sys/stat.h>

int main() {
	int result1 = mknod("my_fifo5", S_IFIFO | 0666, 0);
    if (result1 == -1) {
        perror("mknod");
        return 1;
    }

    int result2 = mkfifo("my_fifo6", 0666);
    if (result2 == -1) {
        perror("mkfifo");
        return 1;
    }
    return 0;
}
