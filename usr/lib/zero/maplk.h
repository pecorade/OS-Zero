#ifndef __ZERO_MAPLK_H__
#define __ZERO_MAPLK_H__

#include <stdlib.h>
#include <zero/mtx.h>
#include <zero/trix.h>
#if (__KERNEL__)
#include <limits.h>
#include <zero/param.h>
#endif

#define ZEROMAPLKINIT { ZEROMTXINITVAL, 0, NULL }

typedef struct {
    volatile long  mtx;
    long           nbit;
    volatile long *bits;
} zeromaplk;

zeromaplk * maplkinit(zeromaplk *maplk, long n);

static __inline__ long
maplktrybit(zeromaplk *maplk, long ndx)
{
    long word = ndx >> LONGSIZELOG2;
    long id = word & (LONGSIZE * CHAR_BIT - 1);
    long val = 0L;
    long bit = 1L << id;
    long ret;

    mtxlk(&maplk->mtx);
    val = m_cmpsetbit(&maplk->bits[word], bit);
    mtxunlk(&maplk->mtx);
    ret = !val;

    return ret;
}

static __inline__ void
maplkbit(zeromaplk *maplk, long ndx)
{
    long word = ndx >> LONGSIZELOG2;
    long id = word & (LONGSIZE * CHAR_BIT - 1);
    long val = 0L;
    long bit = 1L << id;

    do {
        mtxlk(&maplk->mtx);
        val = m_cmpsetbit(&maplk->bits[word], bit);
        mtxunlk(&maplk->mtx);
    } while (val);

    return;
}

static __inline__ void
mapunlkbit(zeromaplk *maplk, long ndx)
{
    long word = ndx >> LONGSIZELOG2;
    long id = word & (LONGSIZE * CHAR_BIT - 1);
    long bit = 1L << id;
    long mask = ~bit;
    long tmp;

    mtxlk(&maplk->mtx);
    tmp = maplk->bits[word];
    tmp &= mask;
    maplk->bits[word] = tmp;
    mtxunlk(&maplk->mtx);

    return;
}

#endif /* __ZERO_MAPLK_H__ */
