/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006-2012, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-06-25     Bernard      first version
 * 2011-08-08     lgnq            modified for Loongson LS1B
 * 2015-07-06     chinesebear  modified for Loongson LS1C
 */

#include "board.h"

#include <rthw.h>
#include <rtthread.h>

#include "ls1x.h"
#include "ls1x_common.h"
#include "drv_uart.h"
#include "timer.h"

extern unsigned char __bss_end;

/**
 * @addtogroup Loongson LS1B
 */

/*@{*/

/**
 * This is the timer interrupt service routine.
 */
void rt_hw_timer_handler(void) {
  Set_Timer_clear();
  Set_Timer_Init(LA_TIMER);  //  1.25ms
  /* increase a OS tick */
  rt_tick_increase();
}

/**
 * This function will initial OS timer
 */
void rt_hw_timer_init(void) {
  Set_Timer_Init(LA_TIMER);  //  1.25ms
}

/**
 * This function will initial sam7s64 board.
 */
void rt_hw_board_init(void) {
  /* init hardware interrupt */
  //   rt_hw_interrupt_init();
  EnableInt();  // 开总中断

#ifdef RT_USING_HEAP
  rt_system_heap_init((void*)&__bss_end, (void*)RT_HW_HEAP_END);
#endif

#ifdef RT_USING_SERIAL
  /* init hardware UART device */
  rt_hw_uart_init();
#endif

#ifdef RT_USING_CONSOLE
  /* set console device */
  rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
  /* init operating system timer */
  rt_hw_timer_init();

#ifdef RT_USING_COMPONENTS_INIT
  rt_components_board_init();
#endif
}

/*@}*/
