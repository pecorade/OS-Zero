#ifndef __VT_VT_H__
#define __VT_VT_H__

#include <stdint.h>
#include <zero/cdecl.h>
#include <zero/param.h>
#include <ui/ui.h>

#define VTBUFSIZE    PAGESIZE

#define VTDEFMODE     0x00000000U
#define VTDEFFGCOLOR  0xffffffffU
#define VTDEFBGCOLOR  0xff000000U
#define VTDEFTEXTATR  0x00000000U

/* Unicode plans not to add characters beyond 0x10ffff */
#define VTUCSMASK     0x001fffffU
#define VTATRMASK     0xffe00000U

/* character attribute bits */
#define VTBRIGHT      0x80000000
#define VTDIM         0x40000000
#define VTUNDERSCORE  0x20000000
#define VTBLINK       0x10000000
#define VTREVERSE     0x08000000
#define VTHIDDEN      0x04000000
#define VTANTIALIAS   0x02000000 // antialiased text (interpolation)
#define VTDRAWBG      0x01000000 // draw text background
#define VTDRAWFG      0x00800000 // draw text foreground
#define VTFGMASK      0x000001ff // standard or 256-color xterm palette entry
#define VTBGMASK      0x0003fe00 // standard or 256-color xterm palette entry
#define VTFG256BIT    0x00000100 // foreground is xterm color
#define VTBG256BIT    0x00020000 // foreground is xterm color

#define VTXTERM256BIT 0x100 // xterm 256-color palette entry
struct vtrend {
    unsigned fgcolor : 9;       // standard or 256-color xterm palette entry
    unsigned bgcolor : 9;       // standard or 256-color xterm palette entry
    unsigned pad     : 7;       // extra room for later flags
    unsigned atr     : 7;       // specified text attributes
} PACK();

/* Unicode character plus attributes such as underline */
//typedef int32_t vtchar_t;
/* character and its 32-bit ARGB-color */
//typedef int64_t vttext_t;

#define RING_ITEM    int32_t
#define RING_INVAL   0x00000000
#include <zero/ring.h>

#define VTESC          '\033'
/* sequence postfixes after ESC */
#define VTCSI          '['
#define VTCHARSET      '('
#define VTHASH         '#'
/* sequences prefixed with ESC */
#define VTCURSORUP     'A'
#define VTCURSORDOWN   'B'
#define VTCURSORRIGHT  'C'
#define VTCURSORLEFT   'D'
#define VTSPECCHARSET  'F'
#define VTASCIICHARSET 'G'
#define VTCURSORHOME   'H'
#define VTREVLINEFEED  'I'
#define VTCLRTOSCREND  'J'
#define VTCLRTOLINEEND 'K'
#define VTCURSORADR    'Y'

struct vtatr {
    int   fd;           // master PTY file descriptor
    char *masterpath;   // master teletype device
    char *slavepath;    // slave teletype device
};

struct vtstate {
    long     col;       // current column
    long     row;       // current column
    long     nrow;      // # of screen rows
    long     ncol;      // # of screen columns
    long     w;         // width in characters
    long     h;         // height in characters
    uint64_t mode;      // private mode etc. mask
    uint64_t flags;     // modifier and button mask
    int32_t  fgcolor;   // foreground text color
    int32_t  bgcolor;   // background text color
    uint32_t textatr;   // current text attributes
};

struct vtdevbuf {
    struct ringbuf in;          // input ring-buffer
    struct ringbuf out;         // output ring-buffer
};

struct vttextbuf {
    long            nrow;       // # of buffer rows
//    long            ncol;       // # of colums in buffer rows
    int32_t       **data;       // Unicode text data
    struct vtrend **rend;       // rendition attribute data
};

struct vtcolormap {
    void *deftab;               // default colors
    void *xtermtab;             // xterm colors
};

struct vt {
    struct vtatr      atr;      // terminal attributes
    struct vtstate    state;    // terminal status
    struct vtdevbuf   devbuf;   // input and output buffers
    struct vttextbuf  textbuf;  // text and rendition buffers
    struct vttextbuf  scrbuf;   // current screen contents
    struct ui         ui;       // user interface
    struct uifont     font;     // default font
    struct vtcolormap colormap; // terminal colormaps
};

typedef long vtescfunc_t(struct vt *, long, long *);
struct vtesc {
    uint8_t      esccmdmap[32];
    uint8_t      csicmdmap[32];
    uint8_t      hashcmdmap[32];
    vtescfunc_t *escfunctab[256];
    vtescfunc_t *csifunctab[256];
    vtescfunc_t *hashfunctab[256];
};

#define vtdefcolor(i)     (((int32_t *)(vt->colormap.deftab))[(i)])
#define vtxtermcolor(i)   (((int32_t *)(vt->colormap.xtermtab))[(i)])
#define vtfgtodefcolor(i) ((i) - 30)
#define vtbgtodefcolor(i) ((i) - 40)

extern char *vtkeystrtab[128];

#endif /* __VT_VT_H__ */
