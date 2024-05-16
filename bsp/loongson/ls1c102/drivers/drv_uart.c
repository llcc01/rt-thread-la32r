/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-08     zhuangwei    the first version
 */
#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>
// #include "ls1c102_irq.h"

#include "drv_intc.h"
#include "drv_uart.h"

struct rt_uart_ls1x {
  rt_uint32_t IRQ;
};

struct rt_uart_ls1x uart1 = {
    UART_IRQ,
};

void uart_putc(char c) {
  while (UART_FIFO_CTRL & 0x02) {
    ;
  }
  UART_FIFO = c;
}

char uart_getc(void) {
  char c;
  if ((UART_FIFO_CTRL & 0x1)) {
    return -1;
  }
  c = UART_FIFO;
  return c;
}

void uart_init(ls1x_uart_info_t *uart_info) {
  uint32_t uart_ctrl;
  switch (uart_info->baudrate) {
    case 9600:
      uart_ctrl = UART_CTRL_9600;
      break;
    case 19200:
      uart_ctrl = UART_CTRL_19200;
      break;
    case 38400:
      uart_ctrl = UART_CTRL_38400;
      break;
    case 115200:
      uart_ctrl = UART_CTRL_115200;
      break;
    default:
      uart_ctrl = UART_CTRL_115200;
      break;
  }
  if (uart_info->rx_enable) {
    ;
  }
  UART_FIFO_CTRL = uart_ctrl;
}

static rt_err_t ls1x_uart_configure(struct rt_serial_device *serial,
                                    struct serial_configure *cfg) {
  struct rt_uart_ls1x *uart_dev = RT_NULL;
  ls1x_uart_info_t uart_info = {0};

  RT_ASSERT(serial != RT_NULL);
  RT_ASSERT(cfg != RT_NULL);

  uart_dev = (struct rt_uart_ls1x *)serial->parent.user_data;

  // 初始化串口
  uart_info.baudrate = cfg->baud_rate;
  uart_info.rx_enable = 1;
  uart_init(&uart_info);

  return RT_EOK;
}

static rt_err_t ls1x_uart_control(struct rt_serial_device *serial, int cmd,
                                  void *arg) {
  struct rt_uart_ls1x *uart_dev = RT_NULL;

  RT_ASSERT(serial != RT_NULL);
  uart_dev = (struct rt_uart_ls1x *)serial->parent.user_data;

  switch (cmd) {
    case RT_DEVICE_CTRL_CLR_INT: /* disable rx irq */
      rt_hw_intc_interrupt_mask(uart_dev->IRQ);
      break;

    case RT_DEVICE_CTRL_SET_INT: /* enable rx irq */
      rt_hw_intc_interrupt_umask(uart_dev->IRQ);
      break;

    default:
      break;
  }

  return RT_EOK;
}

static int ls1x_uart_putc(struct rt_serial_device *serial, char c) {
  struct rt_uart_ls1x *uart_dev = RT_NULL;

  RT_ASSERT(serial != RT_NULL);

  uart_putc(c);

  return 1;
}

static int ls1x_uart_getc(struct rt_serial_device *serial) {
  struct rt_uart_ls1x *uart_dev = RT_NULL;

  RT_ASSERT(serial != RT_NULL);

  // uart_dev = (struct rt_uart_ls1x *)serial->parent.user_data;

  return uart_getc();

  // return -1;
}

/* UART interrupt handler */
static void uart_irq_handler(int vector, void *param) {
  struct rt_serial_device *serial = (struct rt_serial_device *)param;
  struct rt_uart_ls1x *uart_dev = RT_NULL;

  RT_ASSERT(serial != RT_NULL);

  uart_dev = (struct rt_uart_ls1x *)serial->parent.user_data;

  rt_interrupt_enter();
  rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
  rt_interrupt_leave();
}

static const struct rt_uart_ops ls1x_uart_ops = {
    ls1x_uart_configure,
    ls1x_uart_control,
    ls1x_uart_putc,
    ls1x_uart_getc,
};

struct rt_serial_device serial1;

int rt_hw_uart_init(void) {
  struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

  serial1.ops = &ls1x_uart_ops;
  serial1.config = config;

  rt_hw_intc_interrupt_install(UART_IRQ, uart_irq_handler, &serial1, "UART1");

  /* register UART1 device */
  rt_hw_serial_register(
      &serial1, "uart1",
      // RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_DMA_RX,
      RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, &uart1);
  return 0;
}
