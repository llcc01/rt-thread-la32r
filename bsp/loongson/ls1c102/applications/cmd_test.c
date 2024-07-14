#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>
#include <stdlib.h>

#include "OLED.h"

#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
#include "dfs_fs.h"
#endif

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
#endif

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

#endif