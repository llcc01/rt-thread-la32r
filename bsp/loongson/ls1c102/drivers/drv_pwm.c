#include "drv_pwm.h"

#include <drivers/rt_drv_pwm.h>
#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#include "soc_pwm.h"

#ifdef RT_USING_PWM

static rt_err_t control(struct rt_device_pwm *device, int cmd, void *arg) {
  rt_err_t result = RT_EOK;
  struct rt_pwm_configuration *configuration =
      (struct rt_pwm_configuration *)arg;

  if (cmd == PWM_CMD_ENABLE) {
  } else if (cmd == PWM_CMD_DISABLE) {
  } else if (cmd == PWM_CMD_SET) {
    pwm_set(configuration->period * 8 / 1000, configuration->pulse * 8 / 1000);
  } else if (cmd == PWM_CMD_GET) {
  }

  return result;
}

static struct rt_device_pwm pwm0;

static const struct rt_pwm_ops pwm_ops = {
    control,
};

int rt_hw_pwm_init(void) {
  pwm_init();

  return rt_device_pwm_register(&pwm0, "pwm0", &pwm_ops, RT_NULL);
}
INIT_DEVICE_EXPORT(rt_hw_pwm_init);

#endif /* RT_USING_PWM */