/*
================================================
Author : Rajan Khade

Description :
Write a program to execute ls -l | wc.
	a. use dup
================================================
*/

#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        close(1);   // so that dup doen't take this value, since it takes lowest
        dup(fd[1]); // assign pipes right end to stdout
        close(fd[0]);   // close read end

        execlp("ls", "ls", "-l", NULL);
    } else {
        close(0);   // so that dup doen't take this value, since it takes lowest
        dup(fd[0]); // assign pipes read end to stdin
        close(fd[1]);   // close write end

        execlp("wc", "wc", NULL);
    }

    return 0;
}
