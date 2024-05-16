/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-04     Jiaxun Yang  Initial version
 * 2024-04-26     llcc01       Adapted for Loongarch
 */

#include <rtthread.h>

#include "ptrace.h"
#include "regdef.h"
#include "csrdef.h"

rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit)
{
    rt_uint8_t *stk;

    struct pt_regs *pt;

    rt_uint32_t i;

    /* Get stack aligned */
    stk = (rt_uint8_t *)RT_ALIGN_DOWN((rt_ubase_t)stack_addr, 8);
    stk -= sizeof(struct pt_regs);
    pt =  (struct pt_regs*)stk;

    /* Fill Stack register numbers */
    for (i = 0; i < 32; ++i)
    {
        pt->regs[i] = 0xdeadbeef;
    }

    pt->regs[REG_SP] = (rt_ubase_t)stk;
    pt->regs[REG_TP] = (rt_ubase_t)stk;
    pt->regs[REG_A0] = (rt_ubase_t)parameter;
    pt->regs[REG_FP] = (rt_ubase_t)0x0;
    pt->regs[REG_RA] = (rt_ubase_t)texit;

    pt->csr_crmd       = 0x0;
    pt->csr_prmd       = __builtin_loongarch_csrrd(CSR_CRMD) | M_CSR_CRMD_IE;
    pt->csr_euen       = 0x0;
    pt->csr_ecfg       = 0x0;
    pt->csr_estat      = 0x0;
    
    pt->csr_era = (rt_ubase_t)tentry;
    pt->csr_badv    = 0x0;

    pt->last = 0x0;

    return stk;
}
