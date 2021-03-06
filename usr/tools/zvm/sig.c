#include <stdio.h>
#include <stdlib.h>
#include <zas/zas.h>
#include <zvm/zvm.h>
#include <zvm/mem.h>

/* unaligned memory access */
int8_t
zvmsigbus(zasmemadr_t adr, long size)
{
    fprintf(stderr, "SIGBUS: %llx (%ld)\n", (long long)adr, (long)size);

    abort();
}

/* unmapped memory access */
int8_t
zvmsigsegv(zasmemadr_t adr, long reason)
{
    if (reason == ZVMMEMREAD) {
        fprintf(stderr, "SIGSEGV: %llx (%s)\n", (long long)adr, "read");
    } else {
        fprintf(stderr, "SIGSEGV: %llx (%s)\n", (long long)adr, "write");
    }

    abort();
}
