#include <rtthread.h>

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

void Set_Timer_stop(void) { asm volatile("csrwr $r0, 0x41;\n"); }

void Set_Timer_clear(void) {
  asm volatile(
      "li.w $r12, 0x1;\n"
      "csrwr $r12, 0x44;\n" ::
          : "$r12");
}

void Set_Timer_Init(uint32_t timer) {
  asm volatile(
      "csrrd    $r12, 0x41;"
      "ori      $r12, %0, 0x3;"
      "csrwr    $r12, 0x41;"
      :
      : "r"(timer)
      : "$r12");
}