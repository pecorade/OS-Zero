#ifndef __ZAS_IO_H__
#define __ZAS_IO_H__

#define ZASLINELEN     4096

#if (ZASMMAP)
struct zasmap {
    uint8_t *adr;
    uint8_t *cur;
    uint8_t *lim;
    size_t   sz;
};
#endif

#endif /* __ZAS_IO_H__ */
