#include "drv_hdmi.h"

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>
#include <stdlib.h>

#define HDMI_BASE 0xbfe80000
#define HDMI_RES (*(volatile rt_uint32_t *)(HDMI_BASE + 0x0))
#define HDMI_MODE (*(volatile rt_uint32_t *)(HDMI_BASE + 0x4))
// #define HDMI_TERM *(volatile int*)0xbfe80008
#define HDMI_IN_HOR_PIXELS (*(volatile rt_uint32_t *)(HDMI_BASE + 0x10))
#define HDMI_IN_VER_PIXELS (*(volatile rt_uint32_t *)(HDMI_BASE + 0x14))
#define HDMI_IN_HOR_OFFSET (*(volatile rt_uint32_t *)(HDMI_BASE + 0x18))
#define HDMI_IN_VER_OFFSET (*(volatile rt_uint32_t *)(HDMI_BASE + 0x1c))
#define HDMI_UPDATE (*(volatile rt_uint32_t *)(HDMI_BASE + 0x20))

void hdmi_set_res(rt_uint32_t res) { HDMI_RES = res; }

void hdmi_set_mode(rt_uint32_t mode) { HDMI_MODE = mode; }

// void hdmi_putc(rt_uint8_t c) { HDMI_TERM = c; }

// static int rt_hw_hdmi_putc(struct rt_serial_device *serial, char c) {
//   RT_ASSERT(serial != RT_NULL);
//   hdmi_putc(c);
//   return 1;
// }

// static const struct rt_uart_ops hdmi_term_ops = {
//     RT_NULL,
//     RT_NULL,
//     rt_hw_hdmi_putc,
//     RT_NULL,
// };

// struct rt_serial_device hdmi_term;

int rt_hw_hdmi_term_init() {
  // struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
  // hdmi_term.ops = &hdmi_term_ops;
  // hdmi_term.config = config;

  // rt_hw_serial_register(&hdmi_term, "hdmi_term", RT_DEVICE_FLAG_WRONLY,
  // NULL);

  return 0;
}

void hdmi_set_input(rt_uint32_t hres, rt_uint32_t vres, rt_uint32_t hoffset,
                    rt_uint32_t voffset) {
  HDMI_IN_HOR_PIXELS = hres;
  HDMI_IN_VER_PIXELS = vres;
  HDMI_IN_HOR_OFFSET = hoffset;
  HDMI_IN_VER_OFFSET = voffset;
  HDMI_UPDATE = 1;
}

static int cmd_hdmi(int argc, char *argv[]) {
  char *arg1;
  rt_uint8_t arg2;
  switch (argc) {
  case 3:
    arg1 = argv[1];
    arg2 = strtoul(argv[2], NULL, 0);
    if (strcmp(arg1, "res") == 0) {
      hdmi_set_res(arg2);
      break;
    }
    if (strcmp(arg1, "mode") == 0) {
      hdmi_set_mode(arg2);
      break;
    }
    // if (strcmp(arg1, "term") == 0) {
    //   char *p = argv[2];
    //   while (*p) {
    //     rt_hw_hdmi_putc(&hdmi_term, *p);
    //     p++;
    //   }
    //   rt_hw_hdmi_putc(&hdmi_term, '\n');

    //   break;
    // }
    break;
  case 6:
    arg1 = argv[1];
    if (strcmp(arg1, "input") == 0) {
      rt_uint32_t hres, vres, hoffset, voffset;
      hres = strtoul(argv[2], NULL, 0);
      vres = strtoul(argv[3], NULL, 0);
      hoffset = strtoul(argv[4], NULL, 0);
      voffset = strtoul(argv[5], NULL, 0);
      hdmi_set_input(hres, vres, hoffset, voffset);
      break;
    }
    break;
  default:
    rt_kprintf("usage : cmd_hdmi CMD VALUE\n  CMD: res, mode, input\n");
    break;
  }
  return 0;
}
MSH_CMD_EXPORT(cmd_hdmi, cmd_hdmi);
