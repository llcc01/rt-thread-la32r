#include <rthw.h>
#include <rtthread.h>

#include "exception.h"

/**
 * @brief from thread used interrupt context switch
 *
 */
volatile rt_ubase_t  rt_interrupt_from_thread = 0;
/**
 * @brief to thread used interrupt context switch
 *
 */
volatile rt_ubase_t  rt_interrupt_to_thread   = 0;
/**
 * @brief flag to indicate context switch in interrupt or not
 *
 */
volatile rt_ubase_t rt_thread_switch_interrupt_flag = 0;


void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to, rt_thread_t from_thread, rt_thread_t to_thread)
{
    // rt_kprintf("rt_hw_context_switch_interrupt\n");
    // rt_kprintf("from: %p, to: %p\n", *(rt_ubase_t**)from, *(rt_ubase_t**)to);
    // dump_pt_regs(*(struct pt_regs **)to);

    if (rt_thread_switch_interrupt_flag == 0)
        rt_interrupt_from_thread = from;

    rt_interrupt_to_thread = to;
    rt_thread_switch_interrupt_flag = 1;

    return ;
}
