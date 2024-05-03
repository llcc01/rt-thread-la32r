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

#include "drv_pin.h"
#include "drv_uart.h"
#include "drv_timer.h"
#include "exception.h"
// #include "ls1x.h"
// #include "ls1x_common.h"
#include "soc_gpio.h"


extern unsigned int _system_heap;
extern unsigned int _system_heap_end;

/**
 * This function will initial board.
 */
void rt_hw_board_init(void) {
  // rt_hw_interrupt_enable(0);

  // gpio_init(0, LA_GPIO_OUTPUT);
  // gpio_init(1, LA_GPIO_OUTPUT);
  // gpio_init(2, LA_GPIO_OUTPUT);
  // gpio_init(3, LA_GPIO_OUTPUT);
  gpio_init(4, LA_GPIO_OUTPUT);

  // UART_FIFO_CTRL = 0x3;  // 115200

  // gpio_write(0, 1);
  // gpio_write(1, 1);
  // gpio_write(2, 1);
  // gpio_write(3, 1);
  gpio_write(4, 1);

#ifdef RT_USING_HEAP
  rt_system_heap_init((void *)&_system_heap, (void *)&_system_heap_end);
#endif

  rt_hw_interrupt_init();

  rt_hw_uart_init();

#ifdef RT_USING_CONSOLE
  /* set console device */
  rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

  rt_hw_pin_init();

  rt_hw_timer_init();

  // #ifdef RT_USING_COMPONENTS_INIT
  //   rt_components_board_init();
  // #endif
}

/*@}*/
