#!/usr/bin/bash

# /*
# ================================================
# Author : Rajan Khade

# Description :
# Write a program to print the following information about a given file.
# 	a. inode
# 	b. number of hard links
# 	c. uid
# 	d. gid
# 	e. size
# 	f. block size
# 	g. number of blocks
# 	h. time of last access
# 	i. time of last modification
# 	j. time of last change
# ================================================
# */

filename=./test_dir_01/test_file_01.txt

# Extract the inode number
# inode_num=$(ls -i $filename | awk '{print $1}')
# echo -e "Inode: $inode_num"
echo -e "Inode: $(stat -c %i $filename)"
echo -e "Hard links count: $(stat -c %h $filename)"
echo -e "Uid: $(stat -c %u $filename) ($(stat -c %U $filename))"
echo -e "Gid: $(stat -c %g $filename) ($(stat -c %G $filename))"

echo -e "Size: $(stat -c %s $filename)"
echo -e "Block size: $(stat -c %B $filename)"
echo -e "Number of blocks: $(stat -c %b $filename)"

echo -e "Time of last access: $(stat -c %x $filename)"
echo -e "Time of last modification (content): $(stat -c %y $filename)"
echo -e "Time of last change (metadata): $(stat -c %z $filename)"
