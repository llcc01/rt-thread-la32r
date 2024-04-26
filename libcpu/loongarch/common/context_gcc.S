/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-04     Jiaxun Yang  Initial version
 * 2020-07-26     lizhirui     Fixed some problems
 * 2024-04-25     llcc01	   Add support for loongarch
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include "asm.h"
#include "regdef.h"
#include "csrdef.h"
#include "stackframe.h"
#include "ptrace.h"


	.align	5
/*
 * void rt_hw_context_switch(rt_uint32 from, rt_uint32 to)
 * a0 --> from
 * a1 --> to
 */
    .globl rt_hw_context_switch
rt_hw_context_switch:
    # MTC0    ra, CP0_EPC
    SAVE_ALL

    REG_S      sp, a0, 0       /* store sp in preempted tasks TCB */
    REG_L      sp, a1, 0       /* get new task stack pointer */

    RESTORE_ALL_AND_RET

/*
 * void rt_hw_context_switch_to(rt_uint32 to)/*
 * a0 --> to
 */
    .globl rt_hw_context_switch_to
rt_hw_context_switch_to:
    REG_L      sp, a0, 0       /* get new task stack pointer */
    RESTORE_ALL_AND_RET

/*
 * void rt_hw_context_switch_interrupt(rt_uint32 from, rt_uint32 to)/*
 */
    .globl rt_thread_switch_interrupt_flag
    .globl rt_interrupt_from_thread
    .globl rt_interrupt_to_thread
    .globl rt_hw_context_switch_interrupt
rt_hw_context_switch_interrupt:
    PTR_LA      t0, rt_thread_switch_interrupt_flag
    REG_L       t1, t0, 0
    nop
    bnez        t1, _reswitch
    nop
    li.w        t1, 0x01                       /* set rt_thread_switch_interrupt_flag to 1 */
    LONG_S      t1, t0, 0
    PTR_LA      t0, rt_interrupt_from_thread   /* set rt_interrupt_from_thread */
    LONG_S      a0, t0, 0
_reswitch:
    PTR_LA      t0, rt_interrupt_to_thread     /* set rt_interrupt_to_thread */
    LONG_S      a1, t0, 0
    jr      ra
    nop

/*
 * void rt_hw_context_switch_interrupt_do(rt_base_t flag)
 */
    .globl rt_interrupt_enter
    .globl rt_interrupt_leave
    .globl rt_general_exc_dispatch
    .globl mips_irq_handle
mips_irq_handle:
    SAVE_ALL

    /* let x0 keep the current context sp */
    move    x0, sp
    /* switch to kernel stack */
    PTR_LA  sp, _system_stack

    bl      rt_interrupt_enter
    nop
    /* Get Old SP from x0 as paremeter in a0 */
    move    a0, x0
    bl      rt_general_exc_dispatch
    nop
    bl      rt_interrupt_leave
    nop

    /* switch sp back to thread context */
    move    sp, x0

    /*
    * if rt_thread_switch_interrupt_flag set, jump to
    * rt_hw_context_switch_interrupt_do and do not return
    */
    PTR_LA  x0, rt_thread_switch_interrupt_flag
    LONG_L  x0, x0, 0
    beqz    x0, spurious_interrupt
    nop
    PTR_LA  x0, rt_thread_switch_interrupt_flag
    LONG_S  zero, x0, 0                     /* clear flag */
    nop

    /*
    * switch to the new thread
    */
    PTR_LA  x0, rt_interrupt_from_thread
    LONG_L  x0, x0, 0
    nop
    LONG_S  sp, x0, 0                       /* store sp in preempted task TCB */

    PTR_LA  x0, rt_interrupt_to_thread
    LONG_L  x0, x0, 0
    nop
    LONG_L  sp, x0, 0                       /* get new task stack pointer */
    bl      spurious_interrupt
    nop

spurious_interrupt:
    RESTORE_ALL_AND_RET
