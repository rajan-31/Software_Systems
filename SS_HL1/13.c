/*
================================================
Author : Rajan Khade

Description :
Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to verify whether the data is available within 10 seconds or not (check in $man 2 select).
================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

int main() {
	/*
	"select" allows  a  program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible).   A

    file  descriptor is considered ready if it is possible to perform a corresponding I/O operation
	*/


	fd_set readfds;         // Declare a file descriptor "set" (collection) for monitoring read operations
    struct timeval timeout; // Declare a struct to specify the timeout for select()

    // Initialize the file descriptor "set" to be empty
    FD_ZERO(&readfds);

    // "Add" standard input (stdin) to the file descriptor "set"
    FD_SET(STDIN_FILENO, &readfds);

    // timeout value
    timeout.tv_sec = 10;    // Timeout in seconds

    printf("Waiting 10 seconds....\n");

    // Call select() to wait for data to become available on stdin or for the timeout to occur
    // int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
    int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select");
        return 1;
    } else if (result == 0) {
        // Timeout occurred without any data being available
        printf("No data within 10 seconds\n");
    } else {
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            // Check if stdin is still set in the file descriptor set
            printf("Data is available on stdin.\n");
        }
    }

	return 0;
}
