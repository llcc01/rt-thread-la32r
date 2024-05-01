#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "csrdef.h"

#define RT_MAX_INTR 13
#define RT_MAX_EXC  64

#define IRQ_TO_VECTOR(x) ((x) + RT_MAX_EXC)
#define VECTOR_TO_IRQ(x) ((x) - RT_MAX_EXC)

void rt_hw_interrupt_mask(int irq);

#endif
