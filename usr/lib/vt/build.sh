#! /bin/bash

gcc -Wextra -Wundef -Wpointer-arith -DVTTEST=1 -I.. -g -Wall -O -o test *.c ../ui/*.c ../ui/sys/zero.c ../ui/sys/xorg.c ../zero/unix.c -lX11

