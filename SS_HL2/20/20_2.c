/*
================================================
Author : Rajan Khade

Description :
Write two programs so that both can communicate by FIFO -Use one way communication
================================================
*/

#include <fcntl.h>
#include <unistd.h>

int main() {
    char buf[100];
    int fd = open("my_fifo", O_RDONLY);
    read(fd, buf, sizeof(buf));
    write(1, buf, 100); // Write to stdout
    close(fd);
    return 0;
}
