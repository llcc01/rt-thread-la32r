#include "drv_hdmi.h"

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

void hdmi_set_res(uint32_t res) { HDMI_RES = res; }

void hdmi_set_mode(uint32_t mode) { HDMI_MODE = mode; }

void hdmi_putc(uint8_t c) { HDMI_TERM = c; }

static int rt_hw_hdmi_putc(struct rt_serial_device *serial, char c) {
  RT_ASSERT(serial != RT_NULL);
  hdmi_putc(c);
  return 1;
}

static const struct rt_uart_ops hdmi_term_ops = {
    RT_NULL,
    RT_NULL,
    rt_hw_hdmi_putc,
    RT_NULL,
};

struct rt_serial_device hdmi_term;

int rt_hw_hdmi_term_init() {
  struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
  hdmi_term.ops = &hdmi_term_ops;
  hdmi_term.config = config;

  rt_hw_serial_register(&hdmi_term, "hdmi_term", RT_DEVICE_FLAG_WRONLY, NULL);

  return 0;
}