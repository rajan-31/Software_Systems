/*
================================================
Author : Rajan Khade

Description :
Write a program to execute ls -l | wc.
	c. use fcntl
================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        close(1);
        int x = fcntl(fd[1], F_DUPFD, 1);   // fcntl can't close a fd like dup2
        close(fd[0]);

        execlp("ls", "ls", "-l", NULL);
    } else {
        close(0);
        fcntl(fd[0], F_DUPFD, 0);	// 0 to get lowest FD
        close(fd[1]);

        execlp("wc", "wc", NULL);
    }

    return 0;
}
