/*
================================================
Author : Rajan Khade

Description : 
Create the following types of a files using (i) shell command (ii) system call
    a. soft link (symlink system call)
    b. hard link (link system call)
    c. FIFO (mkfifo Library Function or mknod system call)
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    // source path needs to be relative to symlink location OR
    // absolute path
    int a = symlink("../test_dir_01/test_file_01.txt", "./test_dir_02/test_file_01_symlink1.txt");
    if(a < 0) {
        perror("Failed to create symlink!");
        return 1;
    }


    int b = link("./test_dir_01/test_file_01.txt", "./test_dir_02/test_file_01_hardlink1.txt");
    if(b < 0) {
        perror("Failed to create hardlink!");
        return 1;
    }


    // int mknod(const char *pathname, mode_t mode, dev_t dev);
    // dev = 0, device type not applicable
    int c = mknod("./test_dir_02/test_named_pipe_01", S_IFIFO | 0666, 0);


    // int mkfifo(const char *pathname, mode_t mode);
    // 0666: octal format, owner, group, others
    // int c = mkfifo("./test_dir_02/test_named_pipe_01", 0666);

    if(c < 0) {
        perror("Failed to create named pipe!");
        return 1;
    }
    return 0;
}
