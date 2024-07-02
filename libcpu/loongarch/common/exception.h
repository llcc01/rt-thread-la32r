#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "csrdef.h"
#include "ptrace.h"

#include <rtthread.h>

#define LA_MAX_INTR 13
#define RT_MAX_EXC  64

#define IRQ_TO_VECTOR(x) ((x) + RT_MAX_EXC)
#define VECTOR_TO_IRQ(x) ((x) - RT_MAX_EXC)

void rt_hw_interrupt_mask(int irq);

void dump_pt_regs(struct pt_regs *regs);

rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
                                         void *param, const char *name);

void Set_soft_int(void);
void Set_soft_stop(void);

#endif
