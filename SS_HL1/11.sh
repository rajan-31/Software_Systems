#!/usr/bin/bash

# /*
# ================================================
# Author : Rajan Khade

# Description :
# Write a program to open a file, duplicate the file descriptor and append the file with both the
# descriptors and check whether the file is updated properly or not.
# 	a. use dup
# 	b. use dup2
# 	c. use fcntl
# ================================================
# */

cat ./test_dir_01/test_file_03.txt

od -c ./test_dir_01/test_file_03.txt
