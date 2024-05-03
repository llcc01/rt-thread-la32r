#include <rtthread.h>

#include "exception.h"

void Set_Timer_stop(void) { asm volatile("csrwr $r0, 0x41;\n"); }

void Set_Timer_clear(void) {
  __builtin_loongarch_csrwr(0x1, CSR_TICLR);
}

void Set_Timer_Init(uint32_t timer) {
  __builtin_loongarch_csrwr(timer | 0x3, CSR_TCFG);
}