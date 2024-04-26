/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-08     zhuangwei    the first version
 */

#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include <rtthread.h>

#define UART_FIFO *(volatile int*)0xbfe88000
#define UART_FIFO_CTRL *(volatile int*)0xbfe88004

#define UART_CTRL_9600 0
#define UART_CTRL_19200 1
#define UART_CTRL_38400 2
#define UART_CTRL_115200 3
#define UART_CTRL_PARITY 4

typedef struct {
  unsigned int baudrate;
  uint8_t rx_enable;
} ls1x_uart_info_t;

void rt_hw_uart_init(void);

#endif
