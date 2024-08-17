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

/*
 * void rt_hw_context_switch_to(rt_uint32 to)/*
 * a0 --> to
 */
    .globl rt_hw_context_switch_to
rt_hw_context_switch_to:

    REG_L      sp, a0, 0       /* get new task stack pointer */
    RESTORE_ALL_AND_RET 1 // restore all registers and CSR_PRMD, 

/*
 * void rt_hw_context_switch(rt_uint32 from, rt_uint32 to)
 * a0 --> from
 * a1 --> to
 */
    .globl rt_hw_context_switch
rt_hw_context_switch:
    # BACKUP_T0T1


    // fake an exception
    csrrd   t0, CSR_CRMD
    csrwr   t0, CSR_PRMD
    csrwr   ra, CSR_ERA

    SAVE_ALL

    REG_S      sp, a0, 0       /* store sp in preempted tasks TCB */
    REG_L      sp, a1, 0       /* get new task stack pointer */

    RESTORE_ALL_AND_RET 1

/*
 * void rt_hw_context_switch_interrupt_do(rt_base_t flag)
 */

.globl get_current_sp
get_current_sp:
    move    a0, sp
    jr      ra
