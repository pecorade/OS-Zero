#ifndef __ZPC_ASM_H__
#define __ZPC_ASM_H__

#include <stdint.h>
#include <zero/param.h>
#include <zero/cdecl.h>

/* number of registers per unit */
#define ZPCNREG      16

/* instruction set */
#define ZPCASMILL    0x00
/* logical and bit operations */
#define ZPCASMNOT    0x01       // logical NOT
#define ZPCASMSHR    0x02       // logical right shift (fill with zero)
#define ZPCASMSHRA   0x03       // arithmetic right shift (fill with sign)
#define ZPCASMSHL    0x04       // logical shift left (fill with zero)
#define ZPCASMXOR    0x05       // logical exclusive OR
#define ZPCASMOR     0x06       // logical OR
#define ZPCASMAND    0x07       // logical AND
#define ZPCASMROR    0x08       // rotate right
#define ZPCASMROL    0x09       // rotate left
/* arithmetic operations */
#define ZPCASMINC    0x0a       // increment by one
#define ZPCASMDEC    0x0b       // decrement by one
#define ZPCASMADD    0x0c       // addition
#define ZPCASMSUB    0x0d       // subtraction
#define ZPCASMMUL    0x0e       // multiplication
#define ZPCASMDIV    0x0f       // division
#define ZPCASMMOD    0x10       // modulus
/* conditional branches */
#define ZPCASMBZ     0x11       // branch if zero/equal
#define ZPCASMBNZ    0x12       // branch if non-zero/not equal
#define ZPCASMBLT    0x13       // branch if less than
#define ZPCASMBLE    0x14       // branch if lower than or equal
#define ZPCASMBGT    0x15       // branch if greater than
#define ZPCASMBGE    0x15       // branch if greater than or equal
/* load-store instruction */
#define ZPCASMMOV    0x16       // load/store
/* function calls */
#define ZPCASMCALL   0x17       // trigger function call
#define ZPCASMRET    0x18       // return from a function
/* software interrupt */
#define ZPCASMTRAP   0x19       // trigger a software interrupt
#define ZPCNASMOP    32         // maximum number of operations

/* interrupt interface */
#define ZPCTRAPTMR   0x00       // timer interrupt
#define ZPCTRAPDIV   0x01       // division by zero
#define ZPCTRAPBRK   0x02       // breakpoint
#define ZPCTRAPOP    0x03       // invalid operation
#define ZPCTRAPPROT  0x04       // protection (read/write/execute)
#define ZPCTRAPIO    0x05       // I/O interrupt
#define ZPCNTRAP     16         // maximum number of traps

#define ZPCVPUILL    0x00       // illegal instruction
//#define ZPCVPUNOT    0x01     // logical NOT
#define ZPCVPUSHR    0x01       // logical right shift
#define ZPCVPUSHL    0x02       // logical left shift
#define ZPCVPUXOR    0x03       // logical OR
#define ZPCVPUOR     0x04       // logical OR
#define ZPCVPUAND    0x05       // logical AND
#define ZPCVPUADD    0x06       // arithmetic addition of subwords
#define ZPCVPUADDUS  0x07       // addition with unsigned saturation
#define ZPCVPUADDS   0x08       // addition with signed saturation
#define ZPCVPUSUB    0x09       // subtraction
#define ZPCVPUMUL    0x0a       // multiplication
#define ZPCVPUUNPCK  0x0b       // unpack subwords
#define ZPCVPUPCK    0x0c       // pack subwords
#define ZPCNVPUOP    16

/* argument types */
#define ZPCARGREG    0x00       // register
#define ZPCARGSTK    0x01       // stack register
#define ZPCARGI64    0x02       // signed 64-bit integral value
#define ZPCARGU64    0x03       // unsigned 64-bit integral value
#define ZPCARGFLOAT  0x04       // 32-bit IEEE floating point value
#define ZPCARGDOUBLE 0x05       // 64-bit IEEE floating point value
#define ZPCARGLD80   0x06       // 80-bit Intel/IEEE floating point value
#define ZPCARGLD128  0x07       // 128-bit floating point value
#define ZPCARGIMMED  0x08       // immediate 64-bit argument
#define ZPCARGINDIR  0x09       // indirect address (in a register)
#define ZPCARGINDEX  0x0a       // indexed indirect address (index + register)
/* unit IDs */
#define ZPCALU       0x00       // arithmetic-logical unit
#define ZPCVPU64     0x01       // 64-bit vector unit
#define ZPCVPU128    0x02       // 128-bit vector unit

/* opcode bitfield */
struct zpcasminst {
    unsigned op     : 4;        // operation ID
    unsigned arg1t  : 4;        // argument #1 type
    unsigned arg2t  : 4;        // argument #2 type
    unsigned reg1   : 4;        // register ID #1
    unsigned reg2   : 4;        // register ID #2
    unsigned unit   : 4;        // unit ID
    unsigned arg1sz : 4;        // argument sizes in octets/bytes
    unsigned arg2sz : 4;        // argument sizes in octets/bytes
    unsigned pad    : 32;       // pad to 64-bit boundary
    uint64_t args[EMPTY];
} PACK();

/* convenience macros */
#define zpcgetstkreg(num)                                               \
    (zpcstkregs[(num)]->type == ZPCUINT                                 \
     ? zpcstkregs[(num)]->data.ui64.u64                                 \
     : zpcstkregs[(num)]->data.ui64.i64)
#define getstkarg1(op)                                                  \
    (((op)->arg1t == ZPCARGREG                                          \
      ? zpcgetstkreg((op)->reg1])                                       \
      : (op)->args[0])))
#define getstkarg2(op)                                                  \
    (((op)->arg2t == ZPCARGREG                                          \
      ? zpcgetstkreg((op)->arg2)                                        \
      : (arg1t == ZPCARGREG                                             \
         ? (op)->args[1]                                                \
         : (op)->args[0])))
#define getintarg1(op)                                                  \
    (((op)->arg1t == ZPCARGREG                                          \
      ? zpcintregs[(op)->reg1]                                          \
      : (op)->args[0]))
#define getintarg2(op)                                                  \
    (((op)->arg2t == ZPCARGREG                                          \
      ? zpcintregs[(op)->arg2]                                          \
      : (arg1t == ZPCARGREG                                             \
         ? (op)->args[1]                                                \
         : (op)->args[0])))

#endif /* __ZPC_ASM_H__ */

