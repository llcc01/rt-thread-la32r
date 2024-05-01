#include "exception.h"

#include <rthw.h>
#include <rtthread.h>

#include "csrdef.h"
#include "drv_uart.h"
#include "ptrace.h"
#include "soc_gpio.h"

extern rt_ubase_t __eentry_start;
struct rt_irq_desc irq_handle_table[RT_MAX_EXC + RT_MAX_INTR];

rt_ubase_t rt_interrupt_from_thread;
rt_ubase_t rt_interrupt_to_thread;
rt_ubase_t rt_thread_switch_interrupt_flag;

static void unhandled_interrupt_handler(int vector, void *param) {
  rt_uint32_t estat, exccode, excsubcode;

  estat = __builtin_loongarch_csrrd(CSR_ExStatus);
  exccode = (estat & M_CSR_ExStatus_Ecode) >> S_CSR_ExStatus_Ecode;
  excsubcode = (estat & M_CSR_ExStatus_EsubCode) >> S_CSR_ExStatus_EsubCode;
  if (exccode == 0) {
    rt_kprintf("Unhandled interrupt %d occured!\n", VECTOR_TO_IRQ(vector));
  } else {
    rt_kprintf("Unhandled exception %d(%d) occured!\n", exccode, excsubcode);
  }

  RT_ASSERT(0);
}

void rt_hw_interrupt_init(void) {
  int i;
  // rt_base_t eentry = (rt_base_t)&__eentry_start;

  // __builtin_loongarch_csrwr(0x0, CSR_ExConfig);
  // __builtin_loongarch_csrwr(eentry, CSR_EBase);

  // rt_hw_interrupt_enable(0);

  // uart_putc('a');

  for (i = 0; i < RT_MAX_EXC + RT_MAX_INTR; i++) {
    irq_handle_table[i].handler = unhandled_interrupt_handler;
    irq_handle_table[i].param = RT_NULL;
  }

  // uart_putc('b');

  rt_hw_interrupt_enable(0);
}

rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
                                         void *param, const char *name) {
  rt_isr_handler_t old_handler = RT_NULL;

  RT_ASSERT(vector >= 0 && vector < RT_MAX_EXC + RT_MAX_INTR);
  old_handler = irq_handle_table[vector].handler;

#ifdef RT_USING_INTERRUPT_INFO
  rt_strncpy(irq_handle_table[vector].name, name, RT_NAME_MAX);
#endif /* RT_USING_INTERRUPT_INFO */

  irq_handle_table[vector].handler = handler;
  irq_handle_table[vector].param = param;

  return old_handler;
}

static void handle_interrupt(rt_base_t exccode) {
  void *param;
  rt_isr_handler_t irq_func;

  RT_ASSERT(exccode < RT_MAX_EXC + RT_MAX_INTR);
  irq_func = irq_handle_table[exccode].handler;
  param = irq_handle_table[exccode].param;
  irq_func(exccode, param);
}

void interrupt_dispatch(void) {
  rt_uint32_t estat, exccode, mask, pending;

  rt_hw_interrupt_disable();

  estat = __builtin_loongarch_csrrd(CSR_ExStatus);
  exccode = (estat & M_CSR_ExStatus_Ecode) >> S_CSR_ExStatus_Ecode;

  if (exccode == 0) {
    gpio_write(1, 0);

    mask = __builtin_loongarch_csrrd(CSR_ExConfig) & M_CSR_ExConfig_IM;
    pending = (estat & M_CSR_ExStatus_IS & mask) >> S_CSR_ExStatus_IS;

    rt_kprintf("Pending: %x\n", pending);

    // if (pending & M_CSR_ExStatus_IPI)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_IPI));
    // if (pending & M_CSR_ExStatus_TI)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_TI));
    // if (pending & M_CSR_ExStatus_PCOV)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_PCOV));
    // if (pending & M_CSR_ExStatus_HW7)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_HW7));
    // if (pending & M_CSR_ExStatus_HW6)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_HW6));
    // if (pending & M_CSR_ExStatus_HW5)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_HW5));
    // if (pending & M_CSR_ExStatus_HW4)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_HW4));
    // if (pending & M_CSR_ExStatus_HW3)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_HW3));
    // if (pending & M_CSR_ExStatus_HW2)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_HW2));
    // if (pending & M_CSR_ExStatus_HW1)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_HW1));
    // if (pending & M_CSR_ExStatus_HW0)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_HW0));
    // if (pending & M_CSR_ExStatus_SW1)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_SW1));
    // if (pending & M_CSR_ExStatus_SW0)
    //   handle_interrupt(IRQ_TO_VECTOR(S_CSR_ExStatus_SW0));
  } else {
    gpio_write(4, 0);
    uint32_t esubcode =
        (estat & M_CSR_ExStatus_EsubCode) >> S_CSR_ExStatus_EsubCode;
    uint32_t badv = __builtin_loongarch_csrrd(CSR_BadVAddr);
    rt_kprintf("Exception %x(%x) at 0x%x\n", exccode, esubcode, badv);
    // handle_interrupt(exccode);
  }

  while (1) {
    ;
  }

  // rt_hw_interrupt_enable(0);
}

rt_base_t rt_hw_interrupt_disable(void) {
  __builtin_loongarch_csrxchg(0, M_CSR_CRMD_IE, CSR_CRMD);
  return 1;
}

void rt_hw_interrupt_enable(rt_base_t level) {
  if (!level) {
    __builtin_loongarch_csrxchg(M_CSR_CRMD_IE, M_CSR_CRMD_IE, CSR_CRMD);
  }
}

void rt_hw_interrupt_mask(int irq) {
  RT_ASSERT(irq >= 0 && irq < RT_MAX_INTR);
  __builtin_loongarch_csrxchg(0 << irq, 1 << irq, CSR_ExConfig);
}

void rt_hw_interrupt_umask(int irq) {
  RT_ASSERT(irq >= 0 && irq < RT_MAX_INTR);
  __builtin_loongarch_csrxchg(1 << irq, 1 << irq, CSR_ExConfig);
}