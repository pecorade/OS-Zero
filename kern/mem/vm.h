#ifndef __KERN_MEM_VM_H__
#define __KERN_MEM_VM_H__

#if (defined(__i386__) || defined(__i486__)                             \
     || defined(__i586__) || defined(__i686__))                         \
    && !defined(__x86_64__) && !defined(__amd64__)
#include <kern/unit/ia32/vm.h>
#elif defined(__x86_64__) || defined(__amd64__)
#include <kern/unit/x86-64/vm.h>
#endif

#endif /* __KERN_MEM_VM_H__ */

