
#include "drv_timer.h"

#include <rtthread.h>

#include "exception.h"
#include "timer.h"

/**
 * This is the timer interrupt service routine.
 */
void rt_hw_timer_handler(int vector, void *param) {
  rt_interrupt_enter();

  Set_Timer_clear();

  /* increase a OS tick */
  rt_tick_increase();

  rt_interrupt_leave();
}

/**
 * This function will initial OS timer
 */
int rt_hw_timer_init(void) {
  Set_Timer_Init(LA_TIMER);
  rt_hw_interrupt_install(IRQ_TO_VECTOR(S_CSR_ESTAT_TI), rt_hw_timer_handler,
                          RT_NULL, "tick");
  rt_hw_interrupt_umask(S_CSR_ESTAT_TI);
  return 0;
}