/*
================================================
Author : Rajan Khade

Description :
Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        pid_t _pid = getpid(), _ppid = getppid();

        printf("Child  process: PID = %d, Parent PID = %d\n", _pid, _ppid);

        sleep(5);

        // Kill the parent process (send SIGKILL to the parent)
        printf("\nChild  process: sending SIGKILL to parent process (PID = %d)\n", _ppid);
        kill(_ppid, SIGKILL);


        while (getppid() == _ppid) {
            printf("Waiting for re-parenting, Current Parent PID = %d\n", getppid());
            sleep(1);
        }

        _ppid = getppid();

        // After parent is killed, this process becomes an orphan and is adopted by init
        printf("Child  process: After killing parent, new Parent PID = %d\n\n", _ppid);

        // Continue running as an orphan
        while (1) {
            printf("Child  process: PID = %d, Parent PID = %d, running as an orphan\n", _pid, _ppid);
            sleep(3);
        }
    } else {
        // This is the parent process
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);

        while (1) {
            printf("Parent process: running...\n");
            sleep(1);
        }
    }

    return 0;
}

/*
After killing the child new parent pid is pid of "systemd"

*/
