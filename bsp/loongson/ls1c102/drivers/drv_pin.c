/*
 * File      : drv_gpio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-30     zhuangwei    first version
 */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#include "soc_gpio.h"

#ifdef RT_USING_PIN

void ls1c102_pin_mode(struct rt_device *device, rt_base_t pin, rt_uint8_t mode) {
  if (mode == PIN_MODE_INPUT) {
    gpio_init((int)pin, LA_GPIO_INPUT);
  } else if (mode == PIN_MODE_OUTPUT) {
    gpio_init((int)pin, LA_GPIO_OUTPUT);
  }
  return;
}

void ls1c102_pin_write(struct rt_device *device, rt_base_t pin,
                       rt_uint8_t value) {
  gpio_write((int)pin, value);

  return;
}

rt_ssize_t ls1c102_pin_read(struct rt_device *device, rt_base_t pin) {
  return gpio_read((int)pin);
}

rt_err_t ls1c102_pin_attach_irq(struct rt_device *device, rt_base_t pin,
                                rt_uint8_t mode, void (*hdr)(void *args),
                                void *args) {
  /*
unsigned int gpio = pin;
char irq_name[10];

gpio_set_irq_type(gpio, mode);
rt_sprintf(irq_name, "PIN_%d", gpio);
rt_hw_interrupt_install(LS1C_GPIO_TO_IRQ(gpio), (rt_isr_handler_t)hdr, args,
irq_name);
  */

  return RT_EOK;
}

rt_err_t ls1c102_pin_detach_irq(struct rt_device *device, rt_base_t pin) {
  return RT_EOK;
}

rt_err_t ls1c102_pin_irq_enable(struct rt_device *device, rt_base_t pin,
                                rt_uint8_t enabled) {
  /*
unsigned int gpio = pin;

if (enabled)
  rt_hw_interrupt_umask(LS1C_GPIO_TO_IRQ(gpio));
else
  rt_hw_interrupt_mask(LS1C_GPIO_TO_IRQ(gpio));
  */
  return RT_EOK;
}

const static struct rt_pin_ops _ls1c102_pin_ops = {
    ls1c102_pin_mode,       ls1c102_pin_write,      ls1c102_pin_read,

    ls1c102_pin_attach_irq, ls1c102_pin_detach_irq, ls1c102_pin_irq_enable};

int rt_hw_pin_init(void) {
  int ret = RT_EOK;

  ret = rt_device_pin_register("pin", &_ls1c102_pin_ops, RT_NULL);

  return ret;
}
INIT_BOARD_EXPORT(rt_hw_pin_init);

#endif /*RT_USING_PIN */
