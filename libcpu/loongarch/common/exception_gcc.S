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
    BACKUP_T0T1
    SAVE_ALL

    /* let s0 keep the current context sp */
    move    s0, sp

    /* switch to kernel stack */
    beqz    x0, dispatch
    move    sp, x0
    move    x0, zero

    // bl      rt_interrupt_enter

dispatch:
    /* Get Old SP from s0 as paremeter in a0 */
    move    a0, s0
    bl      interrupt_dispatch

    // bl      rt_interrupt_leave

    /* switch sp back to thread context */
    move    sp, s0

check_switch:
    # /*
    # * if rt_thread_switch_interrupt_flag set, jump to
    # * rt_hw_context_switch_interrupt_do and do not return
    # */
    PTR_LA  t0, rt_thread_switch_interrupt_flag
    LONG_L  t1, t0, 0
    beqz    t1, spurious_interrupt

    LONG_S      zero, t0, 0

    # /*
    # * switch to the new thread
    # */
context_switch:
    PTR_LA  t0, rt_interrupt_from_thread
    LONG_L  t1, t0, 0
    LONG_S  sp, t1, 0                       /* store sp in preempted task TCB */

    PTR_LA  t0, rt_interrupt_to_thread
    LONG_L  t1, t0, 0
    LONG_L  sp, t1, 0                       /* get new task stack pointer */

spurious_interrupt:
    RESTORE_ALL_AND_RET 0 1
