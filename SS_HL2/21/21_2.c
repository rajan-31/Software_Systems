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

int main() {
    char buf[100];
    int fd_read = open("my_fifo1", O_RDONLY);
    int fd_write = open("my_fifo2", O_WRONLY);

    read(fd_read, buf, sizeof(buf));
    write(1, buf, 100); // Output to stdout
    write(fd_write, "Hello from prog 2\n", 18);

    close(fd_read);
    close(fd_write);
    return 0;
}
