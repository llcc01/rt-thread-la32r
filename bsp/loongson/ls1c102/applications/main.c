/*
 * File      : main.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2008 - 2017, RT-Thread Development Team
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
 * 2018-05-10     zhuangwei    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_uart.h"


int main(int argc, char** argv) {
  rt_pin_mode(1, PIN_MODE_OUTPUT);

  rt_kprintf("Hello, RT-Thread!\n");

  while (1) {
    rt_pin_write(1, PIN_HIGH);
    my_delay_ms(500);
    rt_pin_write(1, PIN_LOW);
    my_delay_ms(500);
  }
  return 0;
}
