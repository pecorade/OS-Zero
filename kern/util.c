#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <zero/param.h>
#include <zero/trix.h>
#include <kern/io/drv/pc/vga.h>

#define MAXPRINTFSTR 2048

#define isprint(c) ((c) >= 0x20 && (c) < 0x7f)

#define CACHEPREWARM 0
#if (LONGSIZE == 4)
#define LONGBUFSIZE 16
#else
#define LONGBUFSIZE 32
#endif

/*
 * NOTES
 * -----
 * - do not initialize stack variables at top of functions; do it explicitly in
 *   code to avoid stack problems, at least for linker constants
 */

const char _ltoxtab[]
= {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
};

void
bzero(void *adr, unsigned long len)
{
    unsigned long  nleft = len;
    long          *ptr = NULL;
    long          *next;
    long           val;
    long           incr;
#if (CACHEPREWARM)
    long           tmp;
#endif

    next = adr;
    val ^= val;
    incr = 8;
    /* set loop count */
    len >>= LONGSIZELOG2 + 3;
    nleft -= len << (LONGSIZELOG2 + 3);
    while (len) {
#if (CACHEPREWARM)
        tmp = *next;    // cache prewarm; fetch first byte of cacheline
#endif
        ptr = next;     // set pointer
        len--;          // adjust loop count
        next += incr;   // set next pointer
        /* zero memory */
        ptr[0] = val;
        ptr[1] = val;
        ptr[2] = val;
        ptr[3] = val;
        ptr[4] = val;
        ptr[5] = val;
        ptr[6] = val;
        ptr[7] = val;
    }
    while (nleft--) {
        *ptr++ = val;
    }

    return;
}

void
bcopy(void *dest, void *src, unsigned long len)
{
    unsigned long  nleft = len;
    long          *dptr = NULL;
    long          *sptr = NULL;
    long          *dnext;
    long          *snext;
    long           incr;
#if (CACHEPREWARM)
    long    tmp;
#endif

    dnext = dest;
    snext = src;
    incr = 8;
    /* set loop count */
    len >>= LONGSIZELOG2 + 3;
    nleft -= len << (LONGSIZELOG2 + 3);
    while (len) {
#if (CACHEPREWARM)
        tmp = *dnext;   // cache prewarm; fetch first byte of cacheline
        tmp = *snext;   // cache prewarm; fetch first byte of cacheline
#endif
        dptr = dnext;   // set pointers
        sptr = snext;
        len--;          // adjust loop count
        dnext += incr;  // set next pointers
        snext += incr;
        /* copy memory */
        dptr[0] = sptr[0];
        dptr[1] = sptr[1];
        dptr[2] = sptr[2];
        dptr[3] = sptr[3];
        dptr[4] = sptr[4];
        dptr[5] = sptr[5];
        dptr[6] = sptr[6];
        dptr[7] = sptr[7];
    }
    while (nleft--) {
        *dptr++ = *sptr++;
    }

    return;
}

void
bfill(void *adr, uint8_t byte, unsigned long len)
{
    unsigned long  nleft = len;
    long          *ptr = NULL;
    long          *next;
    long           val;
    long           incr;
#if (CACHEPREWARM)
    long           tmp;
#endif

    next = adr;
    val = byte;
    val |= val << 8;
    val |= val << 16;
    incr = 8;
    /* set loop count */
    len >>= LONGSIZELOG2 + 3;
#if (LONGSIZE == 8)
    val |= val << 32;
#endif
    nleft -= len << (LONGSIZELOG2 + 3);
    while (len) {
#if (CACHEPREWARM)
        tmp = *next;    // cache prewarm; fetch first byte of cacheline
#endif
        ptr = next;     // set pointer
        len--;          // adjust loop count
        next += incr;   // set next pointer
        /* fill memory */
        ptr[0] = val;
        ptr[1] = val;
        ptr[2] = val;
        ptr[3] = val;
        ptr[4] = val;
        ptr[5] = val;
        ptr[6] = val;
        ptr[7] = val;
    }
    while (nleft--) {
        *ptr++ = val;
    }

    return;
}

int
memcmp(const void *ptr1,
       const void *ptr2,
       unsigned long nb)
{
    unsigned char *ucptr1 = (unsigned char *)ptr1;
    unsigned char *ucptr2 = (unsigned char *)ptr2;
    int            retval = 0;

    if (nb) {
        while ((*ucptr1 == *ucptr2) && (nb--)) {
            ucptr1++;
            ucptr2++;
        }
        if (nb) {
            retval = (int)*ucptr1 - (int)*ucptr2;
        }
    }

    return retval;
}

int
strcmp(const char *str1,
       const char *str2)
{
    unsigned char *ucptr1 = (unsigned char *)str1;
    unsigned char *ucptr2 = (unsigned char *)str2;
    int            retval = 0;

    while ((*ucptr1) && *ucptr1 == *ucptr2) {
        ucptr1++;
        ucptr2++;
    }
    if (*ucptr1) {
        retval = (int)*ucptr1 - (int)*ucptr2;
    }

    return retval;
}

long
strncpy(char *dest, char *src, long len)
{
    long nb = 0;

    while ((*src) && len--) {
        *dest++ = *src++;
        nb++;
    }
    *dest = '\0';

    return nb;
}

static long
_ltoxn(long val, char *buf, unsigned long len)
{
    uint8_t u8;
    long    l;
    long    m;
    long    n;
    long    incr = 4;

    buf[len - 1] = '\0';
    m = len - 2;
    buf[m] = 0;
    n = 0;
    for (l = 0 ; l < 8 * LONGSIZE ; l += incr) {
        u8 = (val >> l) & 0xf;
        buf[m] = _ltoxtab[u8];
        if (++n == len) {

            break;
        }
        m--;
    }
    m++;
    m = min(m, len - 2);
    while (buf[m] == '0' && m < len - 2) {
        m++;
    }

    return m;
}

static long
_ltodn(long val, char *buf, unsigned long len)
{
    uint8_t u8;
    long    l;
    long    n;

    buf[len - 1] = '\0';
    l = len - 2;
    n = 0;
    do {
        u8 = val % 10;
        val /= 10;
        buf[l] = _ltoxtab[u8];
        if (++n == len) {
            
            break;
        }
        l--;
    } while (val);
    l++;
    l = min(l, len - 2);
    while (buf[l] == '0' && l < len - 2) {
        l++;
    }

    return l;
}

static void *
_strtok(void *ptr, int ch)
{
    uint8_t *u8ptr = ptr;

    while ((u8ptr) && (*u8ptr) && *u8ptr != ch) {
        u8ptr++;
    }
    if ((u8ptr) && (*u8ptr)) {
        *u8ptr = '\0';
    } else {
        u8ptr = NULL;
    }

    return u8ptr;
}

/*
 * %x, %c, %h, %d, %ld, %uc, %uh, %ud, %ul, %lx, %x, %p
 */
void
kprintf(char *fmt, ...)
{
//    char    *str = fmt;
    char    *arg;
    char    *sptr;
    long     val;
    long     isch;
    long     isdec;
    long     ishex;
    long     l;
    long     len;
    va_list  al;
    char     buf[LONGBUFSIZE];
    char     str[MAXPRINTFSTR];
    
    va_start(al, fmt);
    len = MAXPRINTFSTR - 1;
    len = strncpy(str, fmt, len);
    sptr = str;
    while (*sptr) {
        isch = 0;
        isdec = 0;
        ishex = 0;
        val = 0;
        arg = _strtok(sptr, '%');
        if (arg) {
            vgaputs(sptr);
            arg++;
            if (*arg) {
                switch (*arg) {
                    case 'c':
                        isch = 1;
                        val = (char)va_arg(al, int);
                        arg++;
                        
                        break;
                    case 'h':
                        isdec = 1;
                        val = (short)va_arg(al, int);
                        arg++;
                        
                        break;
                    case 'd':
                        isdec = 1;
                        val = va_arg(al, int);
                        arg++;
                        
                        break;
                    case 'p':
                    case 'l':
                        isdec = 1;
                        if (*arg == 'p') {
                            ishex = 1;
                            val = (long)va_arg(al, void *);
                        } else {
                            val = va_arg(al, long);
                        }
                        arg++;
                        if (*arg) {
                            if (*arg == 'd') {
                                arg++;
                            } else if (*arg == 'x') {
                                arg++;
                                ishex = 1;
                            }
                        }
                        
                        break;
                    case 'x':
                        val = va_arg(al, int);
                        arg++;
                        ishex = 1;
                        
                        break;
                    case 'u':
                        arg++;
                        if (*arg) {
                            switch (*arg) {
                                case 'c':
                                    isch = 1;
                                    val = (char)va_arg(al, unsigned int);
                                    
                                    break;
                                case 'h':
                                    isdec = 1;
                                    val = (short)va_arg(al, unsigned int);
                                    
                                    break;
                                case 'd':
                                    isdec = 1;
                                    val = va_arg(al, unsigned int);
                                    
                                    break;
                                case 'l':
                                    isdec = 1;
                                    val = va_arg(al, unsigned long);
                                    
                                    break;
                                default:

                                    break;
                            }
                            arg++;
                        } else {
                            va_end(al);
                            
                            return;
                        }
                        
                        break;
                    default:
                        
                        break;
                }
                if (ishex) {
                    l = _ltoxn(val, buf, LONGBUFSIZE);
                    vgaputs(&buf[l]);
                } else if (isdec) {
                    l = _ltodn(val, buf, LONGBUFSIZE);
                    vgaputs(&buf[l]);
                } else if ((isch) && isprint(val)) {
                    vgaputchar((int)val);
                } else {
                    vgaputchar(' ');
                }
            } else {
                va_end(al);
                
                return;
            }
        } else {
            if (*sptr) {
                vgaputs(sptr);
            }
            va_end(al);
            
            return;
        }
        sptr = arg;
    }
    va_end(al);
    
    return;
}

/*
 * scan bitmap for first zero-bit past ofs
 * return -1 if not found, offset otherwise
 */
long
bfindzerol(unsigned long *bmap, long ofs, long nbit)
{
    long cnt = ofs & ((1UL << (LONGSIZELOG2 + 3)) - 1);
    long ndx = ofs >> (LONGSIZELOG2 + 3);
    long val;
    
    nbit -= ofs;
    if (nbit > 0) {
        val = bmap[ndx];
        val >>= cnt;
        if (val) {
            if (~val) {
                while (val & 0x01) {
                    val >>= 1;
                    ofs++;
                }
            } else {
                ndx++;
                nbit -= ofs + cnt;
                ofs += (1L << (LONGSIZELOG2 + 3)) - cnt;
                while (nbit) {
                    val = bmap[ndx];
                    if (!val) {
                        
                        break;
                    } else if (~val) {
                        while (val & 0x01) {
                            val >>= 1;
                            ofs++;
                        }

                        break;
                    } else {
                        val = 1L << (LONGSIZELOG2 + 3);
                        ndx++;
                        nbit -= val;
                        ofs += val;
                    }
                }
            }
        }
        if (nbit <= 0 || ofs >= nbit) {
            ofs = -1;
        }
    } else {
        ofs = -1;
    }
    
    return ofs;
}

