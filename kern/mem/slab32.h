#ifndef __MEM_SLAB32_H__
#define __MEM_SLAB32_H__

#include <zero/param.h>
#include <zero/mtx.h>
#include <zero/trix.h>
#include <kern/task.h>

#define slabgetprev(hp, tab)                                            \
    (!(hp)                                                              \
     ? NULL                                                             \
     : (((hp)->link & 0x0000ffffL)                                      \
        ? ((tab) + ((hp)->link & 0x0000ffffL))                          \
        : NULL))
#define slabgetnext(hp, tab)                                            \
    (!(hp)                                                              \
     ? NULL                                                             \
     : (((hp)->link & 0xffff0000L)                                      \
        ? ((tab) + (((hp)->link & 0xffff0000L) >> 16))                  \
        : NULL))
#define slabclrprev(hp)                                                 \
    ((hp)->link &= 0xffff0000L)
#define slabclrnext(hp)                                                 \
    ((hp)->link &= 0x0000ffffL)
#define slabclrlink(hp)                                                 \
    ((hp)->link = 0L)
#define slabsetprev(hp, hdr, tab)                                       \
    (slabclrprev(hp), (hp)->link |= slabhdrnum(hdr, tab))
#define slabsetnext(hp, hdr, tab)                                       \
    (slabclrnext(hp), (hp)->link |= (slabhdrnum(hdr, tab) << 16))

struct slabhdr {
    unsigned long nfo;   // size shift count + free-bit
    unsigned long link;  // 16-bit prev and next header IDs
} PACK();

#endif /* __MEM_SLAB32_H__ */

