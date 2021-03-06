#ifndef __ZAS_IO_H__
#define __ZAS_IO_H__

void zasinitbuf(void);

#define ZASBUFSIZE 131072

/* zero assembler I/O facilities */

#if (ZASMMAP)
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#elif (ZASBUF)
#include <errno.h>
#include <fcntl.h>
#endif

#define ZASLINELEN     4096

#if (ZASMMAP)
struct zasmap {
    uint8_t *adr;
    uint8_t *cur;
    uint8_t *lim;
    size_t   sz;
};
#elif (ZASBUF)
struct readbuf {
    void    *data;
    uint8_t *cur;
    uint8_t *lim;
};
#endif

#if (ZASMMAP)
#define zasgetc(map) ((map)->cur < (map)->lim ? *(map)->cur++ : EOF)
#elif (ZASBUF)
static int zasgetc(int fd, int bufid);
#else

#endif

#endif /* __ZAS_IO_H__ */

