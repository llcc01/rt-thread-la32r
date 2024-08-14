#ifndef __DRV_HDMI_H__
#define __DRV_HDMI_H__

#include <rtthread.h>

void hdmi_set_res(uint32_t res);
// void hdmi_set_mode(uint32_t mode);
// void hdmi_putc(uint8_t c);

int rt_hw_hdmi_term_init();

#endif