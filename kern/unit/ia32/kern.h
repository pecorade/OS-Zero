#ifndef __UNIT_IA32_KERN_H__
#define __UNIT_IA32_KERN_H__

#include <stdint.h>

void trapinit(void);
void seginit(long core);
void vminit(void *pagetab, unsigned long nbphys);
void picinit(void);
void meminit(unsigned long nbphys);
void vgainitcon(int w, int h);
void kbdinit(void);
void mouseinit(void);
void logoprint(void);
void taskinit(void);
void tssinit(long id);

#endif /* __UNIT_IA32_KERN_H__ */

