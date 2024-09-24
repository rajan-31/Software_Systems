/*
================================================
Author : Rajan Khade

Description :
Write a program to find out total number of directories on the pwd. execute ls -l | grep ^d | wc ? Use only dup2.
================================================
*/

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    if (fork() == 0) {
        dup2(pipe1[1], 1);

        close(pipe1[0]);
        close(pipe2[0]); close(pipe2[1]);

        execlp("ls", "ls", "-l", NULL);
    }

    if (fork() == 0) {
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);

        close(pipe1[1]);
        close(pipe2[0]);

        execlp("grep", "grep", "^d", NULL);
    }

    close(pipe1[1]);

    if (fork() == 0) {
        dup2(pipe2[0], 0);

        close(pipe2[1]);

        execlp("wc", "wc", "-l", NULL);
    }

    return 0;
}

