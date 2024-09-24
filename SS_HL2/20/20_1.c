/*
================================================
Author : Rajan Khade

Description :
Write two programs so that both can communicate by FIFO -Use one way communication
================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
	int result = mkfifo("my_fifo", 0666);
    if (result == -1) {
        perror("mkfifo");
        return 1;
    }

    int fd = open("my_fifo", O_WRONLY);
    write(fd, "Hello\n", 6);
    close(fd);
    return 0;
}
