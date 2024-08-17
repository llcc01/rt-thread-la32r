#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>

#include "OLED.h"
#include "drv_ddr_dma.h"
#include "drv_hdmi.h"
#include "rttypes.h"

#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
#include "dfs_fs.h"
#endif

void set_hdmi_src(rt_uint8_t res, rt_uint32_t addr, rt_uint32_t width,
                  rt_uint32_t height) {
  rt_uint16_t hdmi_width, hdmi_height;
  if (res == 0) {
    hdmi_width = 640;
    hdmi_height = 480;
  } else if (res == 1) {
    hdmi_width = 1024;
    hdmi_height = 768;
  } else {
    rt_kprintf("Invalid resolution\n");
    return;
  }
  if (width > hdmi_width || height > hdmi_height) {
    rt_kprintf("Invalid input size\n");
    return;
  }
  ddr_dma_set_rd_ch_addr(0, addr, width * height);
  ddr_dma_set_rd_graph_width(0, width);

  hdmi_set_res(res);
  hdmi_set_input(width, height, (hdmi_width - width) / 2,
                 (hdmi_height - height) / 2);
}

#ifdef RT_USING_I2C
static void cmd_oled(int argc, char *argv[]) {
  char *arg1;
  uint8_t arg2;
  static uint8_t init_flag = 0;
  if (init_flag == 0) {
    rt_kprintf("\nOLED init\n");
    OLED_Init();
    OLED_CLS();
    init_flag = 1;
  }
  switch (argc) {
  case 4: {
    uint8_t x = strtoul(argv[1], NULL, 0);
    uint8_t y = strtoul(argv[2], NULL, 0);
    char *p = argv[3];
    OLED_P8x16Str(x, y, p);
    break;
  }
  default:
    rt_kprintf("\nusage : cmd_oled X Y STRING\n  X: 0-127\n  Y: 0-7\n");
    break;
  }
}
MSH_CMD_EXPORT(cmd_oled, print string on OLED);
#endif

#define PWM_DEV_NAME "pwm0"
#define PWM_DEV_CHANNEL 0

#ifdef RT_USING_PWM
static void cmd_pwm(int argc, char *argv[]) {
  uint32_t period, pulse;
  if (argc == 3) {
    /* Search the Device */
    struct rt_device_pwm *pwm_dev =
        (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL) {
      rt_kprintf("pwm sample run failed! can't find %s device!\n",
                 PWM_DEV_NAME);
      return;
    }

    period = strtoul(argv[1], NULL, 0);
    pulse = strtoul(argv[2], NULL, 0);
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
  } else {
    rt_kprintf("\nusage : cmd_pwm PERIOD PULSE\n  the unit is nanoseconds\n");
  }
}
MSH_CMD_EXPORT(cmd_pwm, set pwm period and pulse);
#endif /* RT_USING_PWM */

#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
static void cmd_sd(int argc, char *argv[]) {
  if (argc != 2) {
    rt_kprintf("\nusage : cmd_sd CMD\n  CMD: mount, format\n");
    return;
  }
  if (strcmp(argv[1], "mount") == 0) {
    if (dfs_mount("sd0", "/", "elm", 0, 0) == 0) {
      rt_kprintf("mount sd0 to / success\n");
    } else {
      rt_kprintf("mount sd0 to / failed\n");
    }
  } else if (strcmp(argv[1], "format") == 0) {
    if (dfs_mkfs("elm", "sd0") == 0) {
      rt_kprintf("format sd0 success\n");
    } else {
      rt_kprintf("format sd0 failed\n");
    }
  } else {
    rt_kprintf("\nusage : cmd_sd CMD\n  CMD: mount, format\n");
  }
}
MSH_CMD_EXPORT(cmd_sd, sd card test);

#endif /* defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT) */

static void cmd_ddr_dma(int argc, char *argv[]) {
  rt_uint32_t ch, addr, len;
  char *arg1 = argv[1];
  ch = strtoul(argv[2], NULL, 0);
  addr = strtoul(argv[3], NULL, 0);
  len = strtoul(argv[4], NULL, 0);
  if (argc != 1 + 4 || (strcmp(arg1, "r") != 0 && strcmp(arg1, "w") != 0) ||
      ch > 3) {
    rt_kprintf("usage : cmd_ddr_dma r/w ch b_addr len\nch: 0-3\n");
    return;
  }
  rt_kprintf("DDR DMA test\n");

  if (strcmp(arg1, "w") == 0) {
    rt_kprintf("DDR DMA write\n");
    rt_kprintf("ch: %d, addr: 0x%x, len: %d\n", ch, addr, len);
    ddr_dma_set_wr_ch_addr(ch, addr, len);
  } else {
    rt_kprintf("DDR DMA read\n");
    rt_kprintf("ch: %d, addr: 0x%x, len: %d\n", ch, addr, len);
    ddr_dma_set_rd_ch_addr(ch, addr, len);
  }
}
MSH_CMD_EXPORT(cmd_ddr_dma, test ddr dma);

static void cmd_reg(int argc, char *argv[]) {
  rt_uint32_t addr, val;
  if (argc < 2) {
    rt_kprintf("usage : cmd_reg addr1=val1 addr2=val2 ...\n");
    return;
  }
  for (int i = 1; i < argc; i++) {
    char *arg = argv[i];
    char *p = strchr(arg, '=');
    if (p == NULL) {
      rt_kprintf("usage : cmd_reg addr1=val1 addr2=val2 ...\n");
      return;
    }
    *p = '\0';
    addr = strtoul(arg, NULL, 0);
    val = strtoul(p + 1, NULL, 0);
    rt_kprintf("addr: 0x%x, val: 0x%x\n", addr, val);
    *(volatile rt_uint32_t *)addr = val;
  }

  rt_kprintf("reg test\n");
}
MSH_CMD_EXPORT(cmd_reg, test register);

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
    } else if (strcmp(arg1, "src") == 0) {
      rt_uint8_t res;
      rt_uint32_t addr, width, height;
      res = strtoul(argv[2], NULL, 0);
      addr = strtoul(argv[3], NULL, 0);
      width = strtoul(argv[4], NULL, 0);
      height = strtoul(argv[5], NULL, 0);
      set_hdmi_src(res, addr, width, height);
      break;
    }
    break;
  default:
    rt_kprintf("usage : cmd_hdmi CMD VALUE\n  CMD: res, mode, input, src\n");
    break;
  }
  return 0;
}
MSH_CMD_EXPORT(cmd_hdmi, cmd_hdmi);
