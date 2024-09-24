/*
================================================
Author : Rajan Khade

Description :
Write two programs so that both can communicate by FIFO -Use two way communications
================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
	int result1 = mkfifo("my_fifo1", 0666);
    if (result1 == -1) {
        perror("mkfifo");
        return 1;
    }

    int result2 = mkfifo("my_fifo2", 0666);
    if (result2 == -1) {
        perror("mkfifo");
        return 1;
    }

    char buf[100];
    int fd_write = open("my_fifo1", O_WRONLY);
    int fd_read = open("my_fifo2", O_RDONLY);

    write(fd_write, "Hello from prog 1\n", 18);
    read(fd_read, buf, sizeof(buf));
    write(1, buf, 100); // Output to stdout

    close(fd_write);
    close(fd_read);
    return 0;
}
