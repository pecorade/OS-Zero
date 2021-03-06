#include <features.h>
#include <stddef.h>
#include <setjmp.h>
#include <signal.h>

#if defined(_POSIX_SOURCE)

#if (!SIG32BIT)
#if (PTHREAD)
#define _savesigmask(sp) pthread_sigmask(SIG_BLOCK, NULL, sp)
#define _loadsigmask(sp) pthread_sigmask(SIG_SETMASK, sp, NULL)
#else
#define _savesigmask(sp) sigprocmask(SIG_BLOCK, NULL, sp)
#define _loadsigmask(sp) sigprocmask(SIG_SETMASK, sp, NULL)
#endif
#else
#define _savesigmask(sp)
#define _loadsigmask(sp)
#endif

#elif defined(_BSD_SOURCE)

#if (SIG32BIT)
#define _savesigmask(sp) ((sp)->norm = sigblock(0))
#define _loadsigmask(sp) (sigsetmask((sp)->norm))
#else
#define _savesigmask(sp) (*(sp) = sigblock(0))
#define _loadsigmask(sp) (sigsetmask(*(sp)))
#endif

#else

#if (SIG32BIT)
#define _savesigmask(sp) ((sp)->norm = siggetmask())
#define _loadsigmask(sp) (sigsetmask((sp)->norm))
#else
#define _savesigmask(sp) (*(sp) = siggetmask())
#define _loadsigmask(sp) (sigsetmask(*(sp)))
#endif

#endif /* defined(_POSIX_SOURCE) */

ASMLINK NOINLINE
int
#if defined(__GNUC__)
__attribute__ ((returns_twice))
#endif
setjmp(jmp_buf env)
{
    m_setjmp(env);
#if !(USEOLDBSD)
    _savesigmask(&(env->sigmask));
#endif

    return 0;
}

ASMLINK NORETURN
void
longjmp(jmp_buf env, int val)
{
#if !(USEOLDBSD)
    _loadsigmask(&(env->sigmask));
#endif
    m_longjmp(env, val);

    /* NOTREACHED */
    for ( ; ; ) { ; };
}

ASMLINK NOINLINE
int
#if defined(__GNUC__)
__attribute__ ((returns_twice))
#endif
_setjmp(jmp_buf env)
{
    m_setjmp(env);

    return 0;
}

ASMLINK NORETURN
void
_longjmp(jmp_buf env,
         int val)
{
    m_longjmp(env, val);

    /* NOTREACHED */
    for ( ; ; ) { ; };
}

#if defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)

ASMLINK NOINLINE
int
#if defined(__GNUC__)
__attribute__ ((returns_twice))
#endif
sigsetjmp(sigjmp_buf env, int savesigs)
{
    m_setjmp(env);

    if (savesigs) {
        _savesigmask(&(env->sigmask));
        env->havesigs = 1;
    }

    return 0;
}

ASMLINK NORETURN
void
siglongjmp(sigjmp_buf env, int val)
{
    if (env->havesigs) {
        _loadsigmask(&(env->sigmask));
    }
    m_longjmp(env, val);

    /* NOTREACHED */
    for ( ; ; ) { ; };
}

#endif

