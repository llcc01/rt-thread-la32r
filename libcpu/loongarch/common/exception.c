#include <rtthread.h>

uint8_t IsGlobalIntOpen(void) {
  unsigned int val = 0;
  asm volatile("csrrd  %0, 0x0" : "=r"(val));

  return val & 0x4;
}

void DisableInt(void) {
  asm volatile(
      "li.w $r12, 0x4;\n"
      "csrxchg $r0, $r12, 0x0;\n" ::
          : "$r12");
}

void EnableInt(void) {
  asm volatile(
      "li.w $r12, 0x4;\n"
      "csrxchg $r12, $r12, 0x0;\n" ::
          : "$r12");
}

inline rt_base_t rt_hw_interrupt_disable(void) { DisableInt(); }

inline void rt_hw_interrupt_enable(rt_base_t level) { EnableInt(); }