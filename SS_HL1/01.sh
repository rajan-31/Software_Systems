# /*
# ================================================
# Author : Rajan Khade

# Description :
# Create the following types of a files using (i) shell command (ii) system call
#     a. soft link (symlink system call)
#     b. hard link (link system call)
#     c. FIFO (mkfifo Library Function or mknod system call)
# ================================================
# */

#/usr/bin/bash

# source path needs to be relative to symlink location OR
# absolute path
ln -s ../test_dir_01/test_file_01.txt ./test_dir_02/test_file_01_symlink2.txt

ln ./test_dir_01/test_file_01.txt ./test_dir_02/test_file_01_hardlink2.txt

mkfifo ./test_dir_02/test_named_pipe_02
