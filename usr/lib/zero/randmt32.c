/*
 * 32-bit implementation of the Mersenne Twister MT19937 algorithm
 */

/*
 * REFERENCE: https://en.wikipedia.org/wiki/Mersenne_twister
 */

/*
 * optimisations
 * -------------
 * - elimination of modulus calculations and in-loop branches by unrolling loops
 */

#include <stdint.h>
#include <stdlib.h>
#include <zero/param.h>
#include <zero/cdecl.h>

#define RANDMT32NBUFITEM   624            // # of buffer values
#define RANDMT32MAGIC      397
/* magic numbers */
#define RANDMT32MULTIPLIER 0x6c078965UL
#define RANDMT32MATRIX     0x9908b0dfUL
/* shift counts */
#define RANDMT32SHIFT      30
#define RANDMT32SHIFT1     11
#define RANDMT32SHIFT2     7
#define RANDMT32SHIFT3     15
#define RANDMT32SHIFT4     18
/* bitmasks */
#define RANDMT32MASK2      0x9d2c5680UL
#define RANDMT32MASK3      0xefc60000UL

static unsigned long randbuf32[RANDMT32NBUFITEM] ALIGNED(PAGESIZE);
static unsigned long randnext32[RANDMT32NBUFITEM] ALIGNED(PAGESIZE);
static unsigned long randndx = RANDMT32NBUFITEM + 1;

void
srandmt32(unsigned long seed)
{
    unsigned long val;
    unsigned long tmp;
    int           i;

    tmp = seed & 0xffffffffUL;
    randbuf32[0] = tmp;
    val = RANDMT32MULTIPLIER * (tmp ^ (tmp >> RANDMT32SHIFT)) + 1;
    randbuf32[1] = val & 0xffffffffUL;
    for (i = 2 ; i < RANDMT32NBUFITEM ; i++) {
        tmp = val & 0xffffffffUL;
        val = RANDMT32MULTIPLIER * (tmp ^ (tmp >> RANDMT32SHIFT)) +  i;
        randbuf32[i] = val & 0xffffffffUL;
    }
    randndx = i;

    return;
}

void
_randbuf32(void)
{
    unsigned long mask[2] = { 0UL, RANDMT32MATRIX };
    unsigned long x;
    unsigned long val1;
    unsigned long val2;
    int           i;

    if (randndx == RANDMT32NBUFITEM + 1) {
        srandmt32(5489UL);
    }
    for (i = 0 ; i < RANDMT32NBUFITEM - RANDMT32MAGIC ; i++) {
        val1 = i + 1;
        x = (randbuf32[i] & 0x80000000UL) | (randbuf32[val1] & 0x7fffffffUL);
        randbuf32[i] = randbuf32[i + RANDMT32MAGIC] ^ (x >> 1) ^ mask[x & 0x01];
    }
    for ( ; i < RANDMT32NBUFITEM - 1 ; i++) {
        val1 = i + 1;
        x = (randbuf32[i] & 0x80000000UL) | (randbuf32[val1] & 0x7fffffffUL);
        randbuf32[i] = randbuf32[i + RANDMT32MAGIC - RANDMT32NBUFITEM] ^ (x >> 1) ^ mask[x & 0x01];
    }
    x = (randbuf32[RANDMT32NBUFITEM - 1] & 0x80000000UL) | (randbuf32[0] & 0x7fffffffUL);
    randbuf32[RANDMT32NBUFITEM - 1] = randbuf32[RANDMT32MAGIC - 1] ^ (x >> 1) ^ RANDMT32MATRIX;
    randndx = 0;

    return;
}

unsigned long
randmt32(void)
{
    unsigned long x;

    if (randndx >= RANDMT32NBUFITEM) {
        _randbuf32();
    }
    x = randbuf32[randndx];
    x ^= x >> RANDMT32SHIFT1;
    x ^= (x << RANDMT32SHIFT2) & RANDMT32MASK2;
    x ^= (x << RANDMT32SHIFT3) & RANDMT32MASK3;
    x ^= x >> RANDMT32SHIFT4;
    randndx++;

    return x;
}

