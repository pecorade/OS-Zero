#include <stddef.h>
#include <stdint.h>
#include <zero/cdecl.h>
#include <zero/param.h>
#if defined(__i386__)
#include <kern/unit/ia32/link.h>
#include <kern/unit/ia32/vm.h>
#endif
#include <kern/mem/mag.h>
#include <kern/mem/slab32.h>

#if (MEMTEST)
#include <stdio.h>
extern uint32_t pagetab[NPDE][NPTE];
#endif

struct maghdr  _maghdrtab[1U << (PTRBITS - SLABMINLOG2)] ALIGNED(PAGESIZE);
struct maghdr *_freehdrtab[PTRBITS];
long           _freelktab[PTRBITS];

void *
memalloc(unsigned long nb, long flg)
{
    void          *ret = NULL;
    unsigned long  sz = 0;
    unsigned long  bkt;
    struct maghdr *mag;
    uint8_t       *u8ptr;
    long           l;
    long           n;
    long           incr;

    nb = max(MAGMIN, nb);
    if (nb > (SLABMIN >> 1)) {
#if (MEMTEST)
        ret = vmmapvirt((uint32_t *)&pagetab,
                        slaballoc(virtslabtab, virthdrtab, nb, 0), nb, flg);
#else
        ret = vmmapvirt((uint32_t *)&_pagetab,
                        slaballoc(virtslabtab, virthdrtab, nb, 0), nb, flg);
#endif
        mag = &_maghdrtab[maghdrnum(ret)];
        mag->n = mag->ndx = 0;
    } else {
        bkt = slabbkt(nb);
//        sz = 1UL << bkt;
        maglk(bkt);
        mag = _freehdrtab[bkt];
        if ((mag) && mag->ndx < mag->n) {
            ret = magpop(mag);
            if (magfull(mag)) {
                if (mag->next) {
                    mag->next->prev = NULL;
                }
                _freehdrtab[bkt] = mag->next;
            }
        } else {
            sz = 1UL << bkt;
            ret = u8ptr = slaballoc(virtslabtab, virthdrtab, sz, flg);
            n = 1UL << (SLABMINLOG2 - bkt);
            incr = 1L << bkt;
            mag = &_maghdrtab[maghdrnum(ret)];
            mag->n = n;
            mag->ndx = 0;
#if (MEMTEST)
            fprintf(stderr, "INIT: %ld items:", n);
#endif
            for (l = 0 ; l < n ; l++) {
                mag->ptab[l] = u8ptr;
#if (MEMTEST)
                fprintf(stderr, " %p", u8ptr);
#endif
                u8ptr += incr;
            }
#if (MEMTEST)
            fprintf(stderr, "\n");
#endif
            ret = magpop(mag);
            if (_freehdrtab[bkt]) {
                _freehdrtab[bkt]->prev = mag;
            }
            mag->next = _freehdrtab[bkt];
            _freehdrtab[bkt] = mag;
        }
        magunlk(bkt);
    }

    return ret;
}

void
kfree(void *ptr)
{
    struct maghdr  *mag = &_maghdrtab[maghdrnum(ptr)];
    struct slabhdr *slab;
    unsigned long   bkt;

    if (!ptr) {

        return;
    }
    if (!mag->n) {
        slabfree(virtslabtab, virthdrtab, ptr);
    } else {
#if (MEMTEST)
        fprintf(stderr, "PUSH:\n");
        magprint(mag);
#endif
        magpush(mag, ptr);
        if (magempty(mag)) {
            slabfree(virtslabtab, virthdrtab, ptr);
            slab = &virthdrtab[slabnum(ptr)];
            bkt = slabgetbkt(slab);
            maglk(bkt);
            if (mag->prev) {
                mag->prev->next = mag->next;
            } else {
                _freehdrtab[bkt] = mag->next;
            }
            if (mag->next) {
                mag->next->prev = mag->prev;
            }
            magunlk(bkt);
        }
    }

    return;
}

