#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <zvm/zvm.h>

#if (ZVMVIRTMEM) && (ZAS32BIT)
size_t
zvminitmem(void)
{
    void *ptr = calloc(ZVMNPAGE, sizeof(uint32_t)); 

    if (!ptr) {

        return 0;
    }
    zvm.pagetab = ptr;

    return ZVMNPAGE * ZVMPAGESIZE;
}
#else
size_t
zvminitmem(void)
{
    size_t  len = ZVMMEMSIZE;
    void   *ptr = malloc(ZVMMEMSIZE);

    while (!ptr) {
        len >>= 1;
        ptr = malloc(len);
    }
    zvm.physmem = ptr;
    zvm.memsize = len;

    return len;
}
#endif

#if (ZVMADRBITS == 32)
void *
memmappage(uint32_t adr)
{
    uint32_t  num = adr >> PAGESIZELOG2;
    void     *ptr = malloc(ZVMPAGESIZE);

    if (!ptr) {

        return NULL;
    }
    zvm.pagetab[num] = ptr;

    return ptr;
}
#endif

