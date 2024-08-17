#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include "asm.h"
#include "regdef.h"
#include "csrdef.h"
#include "stackframe.h"
#include "ptrace.h"


    .section ".exc_vectors", "ax"
    .globl rt_interrupt_enter
    .globl rt_interrupt_leave
    .globl interrupt_dispatch
    .globl la_irq_handle
    .global general_exception
general_exception:
    # BACKUP_T0T1
    SAVE_ALL


dispatch:
    move    a0, sp
    bl      interrupt_dispatch


check_switch:
    # /*
    # * if rt_thread_switch_interrupt_flag set, jump to
    # * rt_hw_context_switch_interrupt_do and do not return
    # */
    PTR_LA  s0, rt_thread_switch_interrupt_flag
    LONG_L  s1, s0, 0
    beqz    s1, spurious_interrupt

    LONG_S      zero, s0, 0

    # /*
    # * switch to the new thread
    # */
context_switch:
    PTR_LA  t0, rt_interrupt_from_thread
    LONG_L  a0, t0, 0
    LONG_S  sp, a0, 0                       /* store sp in preempted task TCB */

    PTR_LA  t0, rt_interrupt_to_thread
    LONG_L  a0, t0, 0
    LONG_L  sp, a0, 0                       /* get new task stack pointer */

spurious_interrupt:
    RESTORE_ALL_AND_RET 0
