#! /bin/sh

#CC=gcc
CC=clang

#$CC -Wundef -Wextra -DNVALGRIND -DZERONEWMTX=0 -DZEROMALLOC=0 -DPTHREAD=1 -D_REENTRANT -g -Wall -O -I. -I.. -I../m -I../../.. -I../../../usr/lib -fPIC -nostdinc -fno-builtin -ffreestanding -shared -o zlibc.so bsearch.c malloc2.c qsort.c rand.c setjmp.c string.c strings.c systime.c unistd.c bits/*.c sys/*.c sys/zero/*.c -pthread
$CC -Wundef -Wextra -DNVALGRIND -DZERONEWMTX=0 -DZEROMALLOC=0 -DPTHREAD=1 -D_REENTRANT -g -Wall -O -I. -I.. -I../m -I../../.. -I../../../usr/lib -fPIC -fno-builtin -ffreestanding -shared -o zlibc.so bsearch.c malloc2.c qsort.c rand.c setjmp.c string.c strings.c systime.c unistd.c bits/*.c sys/*.c sys/zero/*.c -pthread

