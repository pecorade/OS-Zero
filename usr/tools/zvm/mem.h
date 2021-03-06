/* zero virtual machine memory interface */

#ifndef __ZVM_MEM_H__
#define __ZVM_MEM_H__

#include <zvm/conf.h>
#include <zero/cdefs.h>
#include <zero/param.h>

#if (ZVMVIRTMEM)

#define ZVMTEXTBASE     (2 * ZVMPAGESIZE)
#define ZVMNPAGE        (1UL << ZVMADRBITS - PAGESIZELOG2)
#if (ZVM32BIT)
#define ZVMADRBITS      32
#else
#define ZVMADRBITS      ADRBITS
#endif
#define ZVMPAGESIZE     PAGESIZE
#define ZVMPAGESIZELOG2 PAGESIZELOG2
#define ZVMPAGEPRES     0x01 // page is present i.e. mapped to physical page
#define ZVMPAGEREAD     0x02 // page read permission
#define ZVMPAGEWRITE    0x04 // page write permission
#define ZVMPAGEEXEC     0x08 // page execute permission

#else /* !ZVMVIRTMEM */

#define ZVMPAGESIZE     PAGESIZE
#define ZVMTEXTBASE     (2 * ZVMPAGESIZE)
#define ZVMMEMSIZE      (32U * 1024U * 1024U)

#endif

/* memory operation failure codes */
#define ZVMMEMREAD      0
#define ZVMMEMWRITE     1

/* memory fetch and store macros */
#define zvmgetmemt(adr, t)                                              \
    (((adr) & (sizeof(t) - 1))                                          \
     ? zvmsigbus(adr, sizeof(t))                                        \
     : _zvmreadmem(adr, t))
#define zvmputmemt(adr, t, val)                                         \
    (((adr) & (sizeof(t) - 1))                                          \
     ? zvmsigbus(adr, sizeof(t))                                        \
     : _zvmwritemem(adr, t, val))
/* memory read and write macros */
#if (ZVMVIRTMEM)
#define zvmadrtoptr(adr)                                                \
    (((!zvm.pagetab[zvmpagenum(ZVMTEXTBASE + adr)])                     \
      ? NULL                                                            \
      : &zvm.pagetab[zvmpagenum(ZVMTEXTBASE + adr)][zvmpageofs(adr)]))
#define _zvmpagenum(adr)                                                \
    ((adr) >> ZVMPAGESIZELOG2)
#define _zvmpageofs(adr)                                                \
    ((adr) & (ZVMPAGESIZE - 1))
#define _zvmreadmem(adr, t)                                             \
    (!(zvm.pagetab[zvmpagenum(adr)])                                    \
     ? zvmsigsegv(adr, ZVMMEMREAD)                                      \
     : (((zvmpageofs(adr) & (sizeof(t) - 1))                            \
         ? zvmsigbus(adr, t)                                            \
         : *(t *)zvm.pagetab[zvmpagenum(adr)][zvmpageofs(t)])))
#define _zvmwritemem(adr, t, val)                                       \
    (!(zvm.pagetab[zvmpagenum(adr)])                                    \
    ? zvmsigsegv(adr, ZVMMEMWRITE)                                      \
    : (((zvmpageofs(adr) & (sizeof(t) - 1))                             \
        ? zvmsigbus(adr, t)                                             \
        : *(t *)(zvm.pagetab[zvmpagenum(adr)][zvmpageofs(t)]) = (val)))
#else /* !ZVMVIRTMEM */
#define zvmadrtoptr(adr)                                                \
    (&zvm.physmem[(adr)])
#define _zvmreadmem(adr, t)                                             \
    (((adr) >= ZVMMEMSIZE)                                              \
     ? zvmsigsegv(adr, ZVMMEMREAD)                                      \
     : *(t *)&zvm.physmem[(adr)])
#define _zvmwritemem(adr, t, val)                                       \
    (((adr) >= ZVMMEMSIZE)                                              \
     ? zvmsigsegv(adr, ZVMMEMWRITE)                                     \
     : *(t *)&zvm.physmem[(adr)] = (val))
#endif

#endif /* __ZVM_MEM_H__ */

