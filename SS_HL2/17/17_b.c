/*
================================================
Author : Rajan Khade

Description :
Write a program to execute ls -l | wc.
	b. use dup2
================================================
*/

#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[0]);

        execlp("ls", "ls", "-l", NULL);
    } else {
        dup2(fd[0], 0);
        close(fd[1]);

        execlp("wc", "wc", NULL);
    }

    return 0;
}
