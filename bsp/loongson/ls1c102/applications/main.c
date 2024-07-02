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

#include <rtdevice.h>
#include <rtthread.h>

#include "OLED.h"
#include "csrdef.h"
#include "drv_hdmi.h"
#include "drv_uart.h"

#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
#include "dfs_fs.h"
#endif

#ifdef RT_USING_USER_MAIN

#define HDMI_RES *(volatile int*)0xbfe80000

void thread2_entry(void* parameter) {
  // while (1) {
  //   rt_kprintf("tick: %d\n", rt_tick_get());
  //   OLED_Print_Num(0, 2, rt_tick_get());
  //   rt_thread_mdelay(50);
  // }
  while (1) {

    rt_pin_write(1, PIN_HIGH);
    // my_delay_ms(500);
    rt_thread_mdelay(100);
    rt_pin_write(1, PIN_LOW);
    // my_delay_ms(500);
    rt_thread_mdelay(100);
  }
}

int main(int argc, char** argv) {
  // I2C_InitTypeDef I2C_InitStruct0;
  // soc_I2C_StructInit(&I2C_InitStruct0);
  // soc_I2C_Init(&I2C_InitStruct0);

  // OLED_Init();

  // OLED_P8x16Str(0, 0, "RT-Thread!");

  rt_pin_mode(1, PIN_MODE_OUTPUT);

  rt_kprintf("Hello, RT-Thread!\n");

#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
  rt_kprintf("mount sd0 to / (elm)\n");
  if (dfs_mount("sd0", "/", "elm", 0, 0) == 0) {
    rt_kprintf("mount sd0 to / success\n");
  } else {
    rt_kprintf("mount sd0 to / failed\n");
  }
#endif

  // rt_thread_t id = rt_thread_create("uart", thread2_entry, RT_NULL, 1024,
  //                                   RT_MAIN_THREAD_PRIORITY, 10);
  // if (id != RT_NULL) {
  //   rt_thread_startup(id);
  // } else {
  //   rt_kprintf("Failed to create thread\n");
  // }

  // HDMI_MODE = 1;

  // // const char* term_name = "hdmi_term";
  // // char str[] = "Hello RT-Thread!\n";

  // // rt_device_t serial = rt_device_find(term_name);
  // // if (!serial) {
  // //   rt_kprintf("find %s failed!\n", term_name);
  // //   return RT_ERROR;
  // // }

  // // rt_err_t ret = rt_device_open(serial, RT_DEVICE_FLAG_WRONLY);
  // // if (ret != RT_EOK) {
  // //   rt_kprintf("open device failed\n");
  // //   return -RT_ERROR;
  // // }

  // // /* 发送字符串 */
  // // rt_size_t send_len = 0;
  // // send_len = rt_device_write(serial, 0, str, (sizeof(str) - 1));
  // // if (send_len != sizeof(str) - 1) {
  // //   rt_kprintf("send data failed\n");
  // //   return -RT_ERROR;
  // // }
  // // /* 关闭设备 */
  // // ret = rt_device_close(serial);
  // // if (ret != RT_EOK) {
  // //   rt_kprintf("close device failed\n");
  // //   return -RT_ERROR;
  // // }

  // // rt_kprintf("serial device test successful\n");

  // int i = 0;
  // int j = 0;
  // while (1) {
  //   // rt_kprintf("HDMI_RES: %d\n", i);
  //   // HDMI_RES = i;
  //   // rt_thread_mdelay(5000);
  //   // i++;
  //   // if (i >= 3) {
  //   //   i = 0;
  //   // }

  //   // rt_kprintf("HDMI_MODE: %d\n", j);
  //   // HDMI_MODE = j;
  //   // rt_thread_mdelay(5000);
  //   // j++;
  //   // if (j >= 2) {
  //   //   j = 0;
  //   // }

  //   rt_kprintf("tick: %d\n", rt_tick_get());

  //   rt_thread_mdelay(10);
  // }

  return 0;
}

#endif
