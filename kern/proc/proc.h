#ifndef __KERN_PROC_PROC_H__
#define __KERN_PROC_PROC_H__

/*
 * process startup
 * ---------------
 *
 * kernel world
 * ------------
 * - initialise segmentation
 * - initialise interrupt management
 * - initialise memory management
 * - acquire and initialise process and thread structures
 * - initialize [framebuffer] graphics
 * - initialize event interface
 * - initialize page daemon
 * - initialize [block I/O] buffer management
 * - execute init
 *
 * init
 * ----
 * - system startup
 *   - services; httpd, sshd, etc.
 *
 * user world
 * ----------
 * - allocate descriptor table
 *  - initialise <stdio.h> facilities
 * - stdin, stdout, stderr
 *   - keyboard and console drivers
 * - argc, argv, envp
 * - set up for crt0
 *   - initialise process structure
 *   - acquire process and thread IDs
 *   - define _text, _bss, _ebss, _start
 *   - set up user and system (ring 0) stacks at zero (the highest part of virtual address space)
 *   - map kernel segment at 3 gigabytes (32-bit systems) or -2 GB
 *   - load, link, and map program segments
 *   - configure default signal handlers
 *
 * userland (c standard library)
 * -----------------------------
 * - crt0
 *   - define _start to start of loaded .text section
 *   - program initialisation
 *     - load and runtime-link image
 *   - zero .bss section
 *   - call main()
 *   - call exit()
 */

#define __KERNEL__ 1
#include <stddef.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <zero/cdecl.h>
#include <zero/param.h>
#include <zero/types.h>
#include <kern/types.h>
#include <kern/cred.h>
#include <kern/syscall.h>
#include <kern/proc/task.h>
#if !defined(__arm__)
#include <kern/unit/x86/cpu.h>
#endif

/* system-assigned process IDs */
#define PROCKERN     0 // main kernel; context switches, system calls, ...
#define PROCINIT     1 // init process; the mother of all processes
#define PROCEVD      2 // event daemon; receive and dispatch events
#define PROCPAGED    3 // page daemon; page replacement; aging, LRU-queue
#define PROCBUFD     4 // buffer daemon; flush dirty buffers to disk
#define PROCIDLE     5 // idle process; zeroes memory etc.
#define TASKNPREDEF  6

long procinit(long id);
long procgetpid(void);
void procfreepid(long id);

/* process segments other than the stacks */
struct procseginfo {
    void   *trapvec;
    size_t  trapvecsize;
    void   *text;
    size_t  textsize;
    void   *rodata;
    size_t  rodatasize;
    void   *data;
    size_t  datasize;
    void   *bss;
    size_t  heapsize;
    void   *brk;
};

/* process */
struct proc {
    struct task         *task;
    long                 nthr;          // # of child threads
    struct task        **thrtab;        // child threads
    /* round-robin queue */
//    struct thrq       thrq;           // queue of ready threads
    /* memory attributes */
    pde_t               *pdetab;        // page directory address
//    size_t               npagetab;      // # of allocated page structures
//    struct virtpage     *pagetab;
    struct physpage     *pagelru;       // LRU-queue for in-core physical pages
    uint8_t             *brk;           // current heap-top
    struct procseginfo  *seginfo;       // process segment information
    /* process credentials */
    struct cred         *cred;
    struct cred         *realcred;
    /* descriptor tables */
    size_t               ndesctab;	// number of entries in descriptor table
    struct desc         *desctab;       // descriptor table
    /* current working directory */
    char                *cwd;
    /* current permission mask */
    mode_t               umask;
    /* signal dispositions */
    signalhandler_t     *sigvec[NSIG];
    /* runtime arguments */
    long                 argc;          // argument count
    char               **argv;          // argument vector
    char               **envp;          // environment strings
    /* memory management */
    struct slabhdr      *vmtab[PTRBITS];
    /* keyboard input buffer */
    void                *kbdbuf;
#if 0
    /* event queue */
    struct ev           *evq;
#endif
} ALIGNED(PAGESIZE);

#endif /* __KERN_PROC_PROC_H__ */

