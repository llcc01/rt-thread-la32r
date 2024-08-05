#include "drv_intc.h"

#include <rtthread.h>

#include "exception.h"

struct rt_irq_desc intc_irq_handle_table[INTC_MAX_INTR];

static void intc_unhandled_interrupt_handler(int vector, void *param) {
  rt_kprintf("Unhandled intc interrupt %d occured!\n", vector);
}

rt_isr_handler_t rt_hw_intc_interrupt_install(int vector,
                                              rt_isr_handler_t handler,
                                              void *param, const char *name) {
  rt_isr_handler_t old_handler = RT_NULL;

  RT_ASSERT(vector >= 0 && vector < INTC_MAX_INTR);
  old_handler = intc_irq_handle_table[vector].handler;

  intc_irq_handle_table[vector].handler = handler;
  intc_irq_handle_table[vector].param = param;

  return old_handler;
}

static void intc_handle_interrupt(int vector) {
  void *param;
  rt_isr_handler_t irq_func;

  RT_ASSERT(vector < INTC_MAX_INTR);
  irq_func = intc_irq_handle_table[vector].handler;
  param = intc_irq_handle_table[vector].param;
  irq_func(vector, param);
}

void intc_irq_handler(int vector, void *param) {
  uint8_t int_out = INT_OUT;

  rt_interrupt_enter();
  for (int i = 0; i < INTC_MAX_INTR; i++) {
    if (int_out & (1 << i)) {
      intc_handle_interrupt(i);
    }
  }

  INT_CLR = 0xff;
  rt_interrupt_leave();
}

int rt_hw_intc_init(void) {
  int i;

  for (i = 0; i < INTC_MAX_INTR; i++) {
    intc_irq_handle_table[i].handler = intc_unhandled_interrupt_handler;
    intc_irq_handle_table[i].param = RT_NULL;
  }

  rt_hw_interrupt_install(IRQ_TO_VECTOR(S_CSR_ESTAT_HW4), intc_irq_handler,
                          RT_NULL, "INTC");
  return 0;
}

void rt_hw_intc_interrupt_umask(int irq) {
  RT_ASSERT(irq < INTC_MAX_INTR);
  INT_EN |= (1 << irq);
}

void rt_hw_intc_interrupt_mask(int irq) {
  RT_ASSERT(irq < INTC_MAX_INTR);
  INT_EN &= ~(1 << irq);
}
