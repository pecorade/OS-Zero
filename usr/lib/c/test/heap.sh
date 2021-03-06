#! /bin/sh

#CC=gcc
CC=gcc
#CCWARNFLAGS="-Wall"
CCWARNFLAGS="-D_FORTIFY_SOURCE=2 -Wall -Wundef -Wextra -Wpointer-arith -Wuninitialized -Winit-self -Wmissing-include-dirs -Wparentheses -Wswitch-default -Wswitch-bool -Wunused-parameter -Wstrict-aliasing -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wtrampolines -Wfloat-equal -Wshadow -Wstack-usage=16384 -Wunsafe-loop-optimizations -Wbad-function-cast -Wc99-c11-compat -Wcast-align -Wwrite-strings -Wdate-time -Wjump-misses-init -Wlogical-op -Waggregate-return -Wstrict-prototypes -Wnormalized -Wopenmp-simd -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wvector-operation-performance -Wvla -Wstack-protector -Woverlength-strings -Wno-designated-init -Wdeclaration-after-statement -Wformat-security -Wmissing-format-attribute"
#CC=clang

# As of October 24 2011, gcc -O2 produces slower code than -O // vendu
# - same thing with -finline-functions and -funroll-loops
$CC $CCWARNFLAGS -DZEROHASH=0 -DZEROFUTEX=0 -DZEROMALLOC=1 -DPTHREAD=1 -D_REENTRANT -g -Wall -fno-builtin -O -I. -o heap heap.c -pthread
$CC $CCWARNFLAGS -DX11VIS=0 -DZEROHASH=0 -DZEROFUTEX=0 -DZEROMALLOC=1 -DPTHREAD=1 -D_REENTRANT -g -Wall -fno-builtin -O -I../../../.. -I../../../lib -o zheap heap.c ../malloc.c -pthread
#$CC $CCWARNFLAGS -DX11VIS=0 -DMALLOCTRACE=1 -DZEROHASH=0 -DZEROFUTEX=0 -DZEROMALLOC=1 -DPTHREAD=1 -D_REENTRANT -g -Wall -fno-builtin -O -I../../../.. -I../../../lib -o tzheap heap.c ../execinfo.c ../malloc.c -pthread
$CC $CCWARNFLAGS -DZEROHASH=0 -DZEROFUTEX=0 -DZEROMALLOC=1 -DPTHREAD=1 -D_REENTRANT -DX11VIS=1 -g -Wall -fno-builtin -O -I../../../.. -I../../../lib -o xzheap heap.c ../malloc.c -Wl,-lX11 -pthread
$CC $CCWARNFLAGS -DZEROHASH=0 -DZEROFUTEX=0 -DZEROMALLOC=1 -DPTHREAD=1 -D_REENTRANT -DX11VIS=1 -g -Wall -fno-builtin -O -I../../../.. -I../../../lib -o t-test1 ../malloc.c t-test1.c -Wl,-lX11 -pthread
$CC $CCWARNFLAGS -DZEROHASH=0 -DZEROFUTEX=0 -DZEROMALLOC=1 -DPTHREAD=1 -D_REENTRANT -DX11VIS=1 -g -Wall -fno-builtin -O -I../../../.. -I../../../lib -o tmem ../malloc.c tmem.c -Wl,-lX11 -pthread
#$CC $CCWARNFLAGS -DPTHREAD=1 -DX11VIS=1 -D_REENTRANT -g -Wall -fno-builtin -O -I../../../.. -I../../../lib /../include -o xheap heap.c ../stdlib/../malloc.c -pthread -lX11
#$CC $CCWARNFLAGS -DPTHREAD=1 -D_REENTRANT -g -Wall -fno-builtin -O -I../../../.. -I../../../lib /../include -o jeheap heap.c libjemalloc.a -pthread -ldl
$CC $CCWARNFLAGS -DX11VIS=0 -DZEROHASH=0 -DZEROFUTEX=0 -DZEROMALLOC=1 -DPTHREAD=1 -D_REENTRANT -DDEVEL=0 -g -Wall -fno-builtin -shared -fPIC -O -I../../../.. -I../../../lib -fPIC -shared -o zmalloc.so ../malloc.c -pthread
#$CC $CCWARNFLAGS -DMALLOCMULTITAB=1 -DZEROHASH=0 -DZEROFUTEX=0 -DZEROMALLOC=1 -DPTHREAD=1 -D_REENTRANT -DX11VIS=1 -DEVEL=0 -g -Wall -fno-builtin -O -I../../../.. -I../../../lib -fPIC -shared -o xzmalloc.so ../malloc.c -lX11 -pthread
#$CC $CCWARNFLAGS -DPTHREAD=1 -DX11VIS=1 -D_REENTRANT -g -Wall -fno-builtin -O -I../../../.. -I../../../lib /../include -fPIC -shared -o xmalloc.so ../stdlib/../malloc.c -pthread


