#include "exception.h"

#include <rthw.h>
#include <rtthread.h>

#include "csrdef.h"
#include "drivers/pin.h"
#include "drv_timer.h"
#include "drv_uart.h"
#include "ptrace.h"
#include "regdef.h"

#include "soc_gpio.h"
#include "context.h"

extern rt_ubase_t __eentry_start;
struct rt_irq_desc irq_handle_table[RT_MAX_EXC + LA_MAX_INTR];

rt_ubase_t rt_interrupt_from_thread;
rt_ubase_t rt_interrupt_to_thread;
rt_ubase_t rt_thread_switch_interrupt_flag;

static void unhandled_interrupt_handler(int vector, void *param) {
  rt_uint32_t estat, exccode, excsubcode;

  estat = __builtin_loongarch_csrrd_w(CSR_ESTAT);
  exccode = (estat & M_CSR_ESTAT_Ecode) >> S_CSR_ESTAT_Ecode;
  excsubcode = (estat & M_CSR_ESTAT_EsubCode) >> S_CSR_ESTAT_EsubCode;
  if (exccode == 0) {
    rt_kprintf("Unhandled interrupt %d occured!\n", VECTOR_TO_IRQ(vector));
  } else {
    rt_kprintf("Unhandled exception %d(%d) occured!\n", exccode, excsubcode);
  }

  RT_ASSERT(0);
}

void rt_hw_interrupt_init(void) {
  int i;

  for (i = 0; i < RT_MAX_EXC + LA_MAX_INTR; i++) {
    irq_handle_table[i].handler = unhandled_interrupt_handler;
    irq_handle_table[i].param = RT_NULL;
  }

}

rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
                                         void *param, const char *name) {
  rt_isr_handler_t old_handler = RT_NULL;

  RT_ASSERT(vector >= 0 && vector < RT_MAX_EXC + LA_MAX_INTR);
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

  RT_ASSERT(exccode < RT_MAX_EXC + LA_MAX_INTR);
  irq_func = irq_handle_table[exccode].handler;
  param = irq_handle_table[exccode].param;
  irq_func(exccode, param);
}

void dump_pt_regs(struct pt_regs *regs) {
  rt_kprintf("Dumping registers:\n\n");

  rt_kprintf("RA(r%d) -> %p\t", REG_RA, regs->regs[REG_RA]);
  rt_kprintf("TP(r%d) -> %p\t", REG_TP, regs->regs[REG_TP]);
  rt_kprintf("SP(r%d) -> %p\n", REG_SP, regs->regs[REG_SP]);

  rt_kprintf("\n");

  rt_kprintf("A0(r%d) -> %p\t", REG_A0, regs->regs[REG_A0]);
  rt_kprintf("A1(r%d) -> %p\t", REG_A1, regs->regs[REG_A1]);
  rt_kprintf("A2(r%d) -> %p\t", REG_A2, regs->regs[REG_A2]);
  rt_kprintf("A3(r%d) -> %p\n", REG_A3, regs->regs[REG_A3]);

  rt_kprintf("A4(r%d) -> %p\t", REG_A4, regs->regs[REG_A4]);
  rt_kprintf("A5(r%d) -> %p\t", REG_A5, regs->regs[REG_A5]);
  rt_kprintf("A6(r%d) -> %p\t", REG_A6, regs->regs[REG_A6]);
  rt_kprintf("A7(r%d) -> %p\n", REG_A7, regs->regs[REG_A7]);

  rt_kprintf("\n");

  rt_kprintf("T0(r%d) -> %p\t", REG_T0, regs->regs[REG_T0]);
  rt_kprintf("T1(r%d) -> %p\t", REG_T1, regs->regs[REG_T1]);
  rt_kprintf("T2(r%d) -> %p\t", REG_T2, regs->regs[REG_T2]);
  rt_kprintf("T3(r%d) -> %p\n", REG_T3, regs->regs[REG_T3]);

  rt_kprintf("T4(r%d) -> %p\t", REG_T4, regs->regs[REG_T4]);
  rt_kprintf("T5(r%d) -> %p\t", REG_T5, regs->regs[REG_T5]);
  rt_kprintf("T6(r%d) -> %p\t", REG_T6, regs->regs[REG_T6]);
  rt_kprintf("T7(r%d) -> %p\n", REG_T7, regs->regs[REG_T7]);

  rt_kprintf("T8(r%d) -> %p\n", REG_T8, regs->regs[REG_T8]);

  rt_kprintf("\n");

  rt_kprintf("X0(r%d) -> %p\t", REG_X0, regs->regs[REG_X0]);
  rt_kprintf("FP(r%d) -> %p\n", REG_FP, regs->regs[REG_FP]);

  rt_kprintf("\n");

  rt_kprintf("S0(r%d) -> %p\t", REG_S0, regs->regs[REG_S0]);
  rt_kprintf("S1(r%d) -> %p\t", REG_S1, regs->regs[REG_S1]);
  rt_kprintf("S2(r%d) -> %p\t", REG_S2, regs->regs[REG_S2]);
  rt_kprintf("S3(r%d) -> %p\n", REG_S3, regs->regs[REG_S3]);

  rt_kprintf("S4(r%d) -> %p\t", REG_S4, regs->regs[REG_S4]);
  rt_kprintf("S5(r%d) -> %p\t", REG_S5, regs->regs[REG_S5]);
  rt_kprintf("S6(r%d) -> %p\t", REG_S6, regs->regs[REG_S6]);
  rt_kprintf("S7(r%d) -> %p\n", REG_S7, regs->regs[REG_S7]);

  rt_kprintf("S8(r%d) -> %p\n", REG_S8, regs->regs[REG_S8]);

  rt_kprintf("\n");

  rt_kprintf("CRMD -> %p\t", regs->csr_crmd);
  rt_kprintf("PRMD -> %p\t", regs->csr_prmd);
  rt_kprintf("EUEN -> %p\t", regs->csr_euen);
  rt_kprintf("ECFG -> %p\n", regs->csr_ecfg);

  rt_kprintf("ESTAT -> %p\t", regs->csr_estat);
  rt_kprintf("ERA -> %p\t", regs->csr_era);
  rt_kprintf("BADV -> %p\t", regs->csr_badv);
  rt_kprintf("LAST -> %p\n", regs->last);

  rt_kprintf("\n");

  rt_kprintf("irq_handle_table = %p\n", irq_handle_table);

}

void interrupt_dispatch(struct pt_regs *regs) {
  rt_uint32_t estat, exccode, mask, pending;

  estat = __builtin_loongarch_csrrd_w(CSR_ESTAT);
  exccode = (estat & M_CSR_ESTAT_Ecode) >> S_CSR_ESTAT_Ecode;

  if (exccode == 0) {
    mask = __builtin_loongarch_csrrd_w(CSR_ECFG) & M_CSR_ECFG_IM;
    pending = (estat & M_CSR_ESTAT_IS & mask) >> S_CSR_ESTAT_IS;

    // rt_kprintf("Pending: %x\n", pending);
    // gpio_write(4,  0);
    // rt_kprintf("sp: %p\n",get_current_sp());

    if (pending & M_CSR_ESTAT_IPI)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_IPI));
    if (pending & M_CSR_ESTAT_TI) {
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_TI));
    }
    if (pending & M_CSR_ESTAT_PCOV)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_PCOV));
    if (pending & M_CSR_ESTAT_HW7)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_HW7));
    if (pending & M_CSR_ESTAT_HW6)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_HW6));
    if (pending & M_CSR_ESTAT_HW5)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_HW5));
    if (pending & M_CSR_ESTAT_HW4)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_HW4));
    if (pending & M_CSR_ESTAT_HW3)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_HW3));
    if (pending & M_CSR_ESTAT_HW2)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_HW2));
    if (pending & M_CSR_ESTAT_HW1)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_HW1));
    if (pending & M_CSR_ESTAT_HW0)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_HW0));
    if (pending & M_CSR_ESTAT_SW1)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_SW1));
    if (pending & M_CSR_ESTAT_SW0)
      handle_interrupt(IRQ_TO_VECTOR(S_CSR_ESTAT_SW0));
  } else {
    rt_pin_write(0, PIN_HIGH);
    uint32_t esubcode = (estat & M_CSR_ESTAT_EsubCode) >> S_CSR_ESTAT_EsubCode;
    uint32_t badv = __builtin_loongarch_csrrd_w(CSR_BADV);
    uint32_t badi = __builtin_loongarch_csrrd_w(CSR_BADI);
    uint32_t era = __builtin_loongarch_csrrd_w(CSR_ERA);
    rt_kprintf("Exception 0x%x(0x%x), badv %p, badi %p, era %p\n",
               exccode, esubcode, badv, badi, era);
    dump_pt_regs(regs);
    // handle_interrupt(exccode);
    while (1) {
      ;
    }
  }

}

rt_base_t rt_hw_interrupt_disable(void) {
  return __builtin_loongarch_csrxchg_w(0, M_CSR_CRMD_IE, CSR_CRMD) &
         M_CSR_CRMD_IE;
}

void rt_hw_interrupt_enable(rt_base_t level) {
  __builtin_loongarch_csrxchg_w(level, M_CSR_CRMD_IE, CSR_CRMD);
}

void rt_hw_interrupt_mask(int irq) {
  RT_ASSERT(irq >= 0 && irq < LA_MAX_INTR);
  __builtin_loongarch_csrxchg_w(0 << irq, 1 << irq, CSR_ECFG);
}

void rt_hw_interrupt_umask(int irq) {
  RT_ASSERT(irq >= 0 && irq < LA_MAX_INTR);
  __builtin_loongarch_csrxchg_w(1 << irq, 1 << irq, CSR_ECFG);
}

void Set_soft_int(void) {
  asm volatile(
      "li.w $r12, 0x1;\n"
      "csrxchg $r12, $r12, 0x4;\n" ::
          : "$r12");
}

void Set_soft_stop(void) {
  asm volatile(
      "li.w $r12, 0x1;\n"
      "csrxchg $r0, $r12, 0x4;\n" ::
          : "$r12");
}