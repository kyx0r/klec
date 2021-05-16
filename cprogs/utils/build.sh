#!/bin/sh

gcc -O3 -Wall -Wextra -std=c99 -o cembed cembed.c tinypgm.c 
gcc -O0 amal.c -o /usr/bin/amal -g
