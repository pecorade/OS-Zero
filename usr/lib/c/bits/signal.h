#ifndef __BITS_SIGNAL_H__
#define __BITS_SIGNAL_H__

#include <features.h>
#include <stdint.h>
#include <errno.h>
#include <zero/param.h>
#if (USEBSD) && (!USEPOSIX)
#include <ucontext.h>
#include <zero/types.h>
#endif
#if defined(PTHREAD)
#include <pthread.h>
#endif
#if defined(__x86_64__) || defined(__amd64__)
#include <x86-64/signal.h>
#elif (defined(__i386__) || defined(__i486__)                           \
       || defined(__i586__) || defined(__i686__))
#include <ia32/signal.h>
#elif defined(__arm__)
#include <arm/signal.h>
#endif
//#include <sys/types.h>
#if !defined(__pid_t_defined)
typedef long            pid_t;          // process ID
#define __pid_t_defined 1
#endif
#if !defined(__uid_t_defined)
typedef uint32_t        uid_t;
#define __uid_t_defined 1
#endif

/* internal. */
#define _sigvalid(sig)  ((sig) && (!((sig) & ~_SIGMASK)))
#define _sigrt(sig)     ((sig) && ((sig) & _SIGRTBIT))
#define _signorm(sig)   ((sig) && (!((sig) & _SIGRTBIT)))
#define _SIGNOCATCHBITS ((UINT64_C(1) << SIGKILL) | (UINT64_C(1) << SIGSTOP))

#if (defined(__x86_64__) || defined(__amd64__) || defined(___alpha__)   \
     || defined(__i386__) || defined(__i486__)                          \
     || defined(__i586__) || defined(__i686__))
#define SIG32BIT 0
#else
#define SIG32BIT 1
#endif

typedef void         (*__sighandler_t)(int);

/* special values; standard ones */
#define SIG_ERR      ((__sighandler_t)-1)
#define SIG_DFL      ((__sighandler_t)0)
#define SIG_IGN      ((__sighandler_t)1)
#define SIG_HOLD     ((__sighandler_t)2)
/* private values for signal actions */
#define _SIG_TERM    ((__sighandler_t)3)
#define _SIG_CORE    ((__sighandler_t)4)
#define _SIG_STOP    ((__sighandler_t)5)
#define _SIG_CONT    ((__sighandler_t)6)

/* commands for sigprocmask() */
#define SIG_BLOCK    0
#define SIG_UNBLOCK  1
#define SIG_SETMASK  2

#define SIGNO_MASK   0x000000ff
#define SIGDEFER     0x00000100
#define SIGHOLD      0x00000200
#define SIGRELSE     0x00000400
#define SIGIGNORE    0x00000800
#define SIGPAUSE     0x00001000

#if defined(SIG32BIT)
typedef struct {
    int32_t norm;
    int32_t rt;
} sigset_t;
#else
typedef int64_t sigset_t;
#endif
#if !defined(__pid_t_defined)
typedef long             pid_t;          // process ID
#define __pid_t_defined
#endif

#if (USEBSD) && (!USEPOSIX)
typedef void __bsdsig_t(int sig, int code, struct sigcontext *ctx, char *adr);
#endif
#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
typedef __sighandler_t sig_t;
#endif
#if defined(_GNU_SOURCE)
typedef __sighandler_t sighandler_t;
#endif

/* sigaction() definitions */ 
#define SA_NOCLDSTOP SIG_NOCLDSTOP
#define SA_NOCLDWAIT SIG_NOCLDWAIT
#define SA_NODEFER   SIG_NODEFER
#define SA_NOMASK    SA_NODEFER
#define SA_RESETHAND SIG_RESETHAND
#define SA_ONESHOT   SA_RESETHAND
#define SA_SIGINFO   SIG_SIGINFO
/* non-POSIX */
#if (!USEPOSIX) || (USEBSD) || (USEGNU)
#define SA_ONSTACK   SIG_ONSTACK
#define SA_RESTART   SIG_RESTART
#define SA_INTERRUPT SIG_FASTINTR
#define SA_RESTORER  SIG_TRAMPOLINE
#endif

union sigval {
    void *sival_ptr;
    int   sival_int;
#if (PTRBITS == 64)
    int   _pad;
#endif
};

/* values for sigev_notify */
#define SIGEV_NONE   0
#define SIGEV_SIGNAL 1
#define SIGEV_THREAD 2
struct sigevent {
    union sigval     sigev_value;
    void           (*sigev_notify_function)(union sigval);
#if defined(PTHREAD)
    pthread_attr_t  *sigev_notify_attributes;
#endif
    int              sigev_notify;
    int              sigev_signo;
    uint8_t          _res[CLSIZE - sizeof(union sigval) - sizeof(void *)
#if defined(PTHREAD)
                          - sizeof(pthread_attr_t *)
#endif
                          - 2 * sizeof(int)];
};

#if defined(_POSIX_SOURCE) && (USEPOSIX199309)

#if !defined(__ctid_t_defined)
typedef long ctid_t;
#define __ctid_t_defined 1
#endif
#if !defined(__zoneid_t_defined)
typedef long zoneid_t;
#define __zoneid_t_defined 1
#endif

/* si_code-member values */
/* SIGILL */
#define ILL_ILLOPC    1 // illegal opcode
#define ILL_ILLOPN    2 // illegal operand
#define ILL_ILLADR    3 // illegal addressing mode
#define ILL_ILLTRP    4 // illegal trap
#define ILL_PRVOPC    5 // privileged opcode
#define ILL_PRVREG    6 // privileged register
#define ILL_COPROC    7 // coprocessor error
#define ILL_BADSTK    8 // internal stack error
/* SIGFPE */
#define FPE_INTDIV    1 // integer division by zero
#define FPE_INTOVF    2 // integer overflow
#define FPE_FLTDIV    3 // floating-point division by zero
#define FPE_FLTOVF    4 // floating-point overflow
#define FPE_FLTUND    5 // floating-point underflow
#define FPE_FLTRES    6 // floating-point inexact result
#define FPE_FLTINV    7 // invalid floating-point operation
#define FPE_FLTSUB    8 // subscript out of range
/* SIGSEGV */
#define SEGV_MAPERR   1 // address not mapped to object
#define SEGV_ACCERR   2 // invalid permissions for mapped object
/* SIGBUS */
#define BUS_ADRALN    1 // invalid address alignment
#define BUS_ADRERR    2 // nonexistent physical address
#define BUS_OBJERR    3 // object-specific hardware error
/* SIGTRAP */
#define TRAP_BRKPT    1 // process breakpoint
#define TRAP_TRACE    2 // process trace trap
/* SIGCHLD */
#define CLD_EXITED    1 // child has exited
#define CLD_KILLED    2 // terminated abnormally, didn't dump core
#define CLD_DUMPED    3 // terminated abnormally, dumped core
#define CLD_TRAPPED   4 // traced child has trapped
#define CLD_STOPPED   5 // child has stopped
#define CLD_CONTINUED 6 // stopped child has continued
/* SIGPOLL */
#define POLL_IN       1 // data input available
#define POLL_OUT      2 // output buffers available
#define POLL_MSG      3 // input message available
#define POLL_ERR      4 // I/O error
#define POLL_PRI      5 // high priority input available
#define POLL_HUP      6 // device disconnected
/* any signal */
#define SI_USER       1 // signal sent by kill()
#define SI_KERNEL     2 // sent by the kernel
#define SI_QUEUE      3 // signal sent by sigqueue()
#define SI_TIMER      4 // timer set by timer_settime() expired
#define SI_ASYNCIO    5 // asynchronous I/O request completed
#define SI_MESGQ      6 // message arrived on empty message queue
/* linux has SI_SIGIO, SI_TKILL */
typedef struct {
    int           si_signo;     // signal number
    int           si_errno;     // errno-value or zero
    int           si_code;      // signal code
    int           si_trapno;    // hardware trap number
    int           si_status;    // exit value or signal
    int           si_int;       // POSIX.1b signal; sigqueue(), mq_notify()
    int           si_overrun;   // timer overrun count; POSIX.1b timers
    int           si_timerid;   // internal kernel timer ID; POSIX.1b timers
    int           si_fd;        // file descriptor; SIGPOLL/SIGIO
    int           si_syscall;   // system call number
    /* kill() and sigqueue() cause si_pid and si_uid to be set */
    pid_t         si_pid;       // sender process; kill(), sigqueue(),
                                // mq_notify()
    uid_t         si_uid;       // real user ID of sender process
    void         *si_ptr;       // POSIX.1b signal; sigqueue(), mq_notify()
    void         *si_addr;      // SIGILL, SIGFPE, SIGSEGV, SIGBUS
    long          si_band;      // band event for SIGPOLL/SIGIO
    union sigval  si_value;     // signal value
    ctid_t        si_ctid;
    zoneid_t      si_zoneid;
    clock_t       si_utime;     // user time consumed
    clock_t       si_stime;     // system time consumed
} siginfo_t;

#endif /* _POSIX_SOURCE && USEPOSIX199309 */

#if defined(_BSD_SOURCE) || defined(USEXOPENEXT)

struct sigstack {
    char *ss_sp;        // signal stack pointer
    int   ss_onstack;   // non-zero when signal-stack in use
#if (PTRBITS == 64)
    int   _pad;
#endif
};

#define SIGSTKSZ     (2 * PAGESIZE)
#define MINSIGSTKSZ  PAGESIZE
/* ss_flags bits */
#define SS_ONSTACK   0x00000001
#define SS_DISABLE   0x00000002
typedef struct {
    void   *ss_sp;      // stack base or pointer
    size_t  ss_size;    // stack size
    int     ss_flags;   // flags
} stack_t;

#elif (FAVORBSD)

struct sigaltstack {
    void *ss_base;
    int   ss_len;
    int   ss_onstack;
};

#elif (!USEPOSIX)

struct sigaltstack {
    void   *ss_sp;
    size_t  ss_size;
    int     ss_flags;
};

#endif

#if (USEBSD) && (!USEPOSIX)

/* bits for sv_flags */
#define SV_INTERRUPT SIG_RESTART        // opposite sense
#define SV_RESETHAND SIG_RESETHAND
#define SV_ONSTACK   SIG_ONSTACK
#define sv_onstack   sv_flags           // compatibility name
struct sigvec {
    void (*sv_handler)(int);            // signal disposition
    long   sv_mask;                     // signals to block while executing
    int    sv_flags;
};

struct sigcontext {
    sigset_t   sc_mask;         // sigstack state to restore
    mcontext_t sc_mcontext;
};

#endif /* USEBSD && !USEPOSIX */

#if defined(_POSIX_SOURCE)

#if defined(SIG32BIT)

/* POSIX */
#define sigemptyset(sp)                                                 \
    ((sp)->norm = (sp)->rt = 0)
#define sigfillset(sp)                                                  \
    (((sp)->norm = (sp)->rt = ~UINT32_C(0)), 0)
#define sigaddset(sp, sig)                                              \
    ((!_sigvalid(sig)                                                   \
      ? (__seterrno(EINVAL), -1L)                                       \
      : (_signorm(sig)                                                  \
         ? ((sp)->norm |= (1UL << (sig)))                               \
         : ((sp)->rt |= (1UL << ((sig) - SIGRTMIN))),                  \
         0)))
#define sigdelset(sp, sig)                                              \
    ((!_sigvalid(sig)                                                   \
      ? (__seterrno(EINVAL), -1L)                                       \
      : (_signorm(sig)                                                  \
         ? ((sp)->norm &= ~(1UL << (sig)))                              \
         : ((sp)->rt &= ~(1UL << ((sig) - SIGRTMIN))),                 \
         0)))
#define sigismember(sp, sig)                                            \
    ((!_sigvalid(sig)                                                   \
      ? (__seterrno(EINVAL), -1L)                                       \
      : (_signorm(sig)                                                  \
         ? (((sp)->norm >> (sig)) & 0x01)                               \
         : (((sp)->rt >> (sig - SIGRTMIN)) & 0x01))))
#define __sigisemptyset(sp) (!(sp)->norm | !(sp)->rt)

#else /* !SIG32BIT */

/* POSIX */
#define sigemptyset(sp)                                                 \
    (*(sp) = 0)
#define sigfillset(sp)                                                  \
    (*(sp) = ~0L)
#define sigaddset(sp, sig)                                              \
    ((!_sigvalid(sig)                                                   \
      ? (__seterrno(EINVAL), -1L)                                       \
      : ((sp) |= (1UL << (sig)),                                        \
         0)))
#define sigdelset(sp, sig)                                              \
    ((!_sigvalid(sig)                                                   \
      ? (__seterrno(EINVAL), -1L)                                       \
      : ((sp) &= ~(1UL << (sig)),                                       \
         0)))
#define sigismember(sp, sig)                                            \
    ((!_sigvalid(sig)                                                   \
      ? (__seterrno(EINVAL), -1)                                        \
      : ((*(sp) & (1UL << (sig)))                                       \
         ? 1                                                            \
         : 0)))
#if defined(_GNU_SOURCE)
#define __sigisemptyset(sp) (!*(sp))
#endif

#endif /* SIG32BIT */

#endif /* _POSIX_SOURCE */

#if defined(_GNU_SOURCE)
#define sigisemptyset(sp) __sigisemptyset(sp)
#endif

#if defined(_POSIX_SOURCE)

#define MAXSIG      SIGRTMAX

#define S_SIGNAL    1
#define S_SIGSET    2
#define S_SIGACTION 3
#define S_NONE      4

#endif /* _POSIX_SOURCE */

#if defined(_BSD_SOURCE)
#define BADSIG      SIG_ERR
#endif

#define MAXSIG      SIGRTMAX

#define S_SIGNAL    1
#define S_SIGSET    2
#define S_SIGACTION 3
#define S_NONE      4

#if defined(_ZERO_SOURCE)
#include <kern/signal.h>
#endif

#if (!_ZERO_SOURCE) && (USEPOSIX)
struct sigaction {
    void     (*sa_handler)(int);
    /* POSIX: the third argument may be cast to ucontext_t */
    void     (*sa_sigaction)(int, siginfo_t *, void *); // sa_flags & SA_SIGINFO
    void     (*sa_restorer)(void);
    sigset_t  sa_mask;
    int       sa_flags;
};
#endif

#endif /* __BITS_SIGNAL_H__ */

