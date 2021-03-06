#! /bin/sh

#clang -g -Wall -DZAS32BIT=1 -DZASZEROHASH=0 -DZVMDEBUG=1 -DZVMXORG=0 -DZVMTRACE=1 -DZVMMULTITAB=0 -DZASNEWHACKS=1 -DZVMVIRTMEM=0 -O0 -I../../lib -I../.. -I.. -I/usr/include/freetype2 -o zvm main.c zvm.c mem.c op.c io.c asm.c sig.c ../zas/zas.c ../zas/opt.c ../zas/io.c ../../lib/zero/hash.c -lX11 -lImlib2 -lfreetype
gcc -g -Wall -DZVMDEBUG=1 -DZAS32BIT=1 -DZASZEROHASH=0 -DZVMEFL=1 -DZVMTRACE=1 -DZVMMULTITAB=0 -DZASNEWHACKS=1 -DZVMVIRTMEM=0 -O -I../../lib -I../.. -I.. -I/usr/include/freetype2 -o ezvm main.c zvm.c mem.c op.c io.c asm.c sig.c efl.c ../zas/zas.c ../zas/opt.c ../zas/io.c ../../lib/zero/hash.c `pkg-config ecore ecore-evas edje evas --cflags --libs`

