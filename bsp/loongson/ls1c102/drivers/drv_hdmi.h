#ifndef __DRV_HDMI_H__
#define __DRV_HDMI_H__

#include <rtthread.h>

void hdmi_set_res(rt_uint32_t res);
void hdmi_set_mode(rt_uint32_t mode);
void hdmi_set_input(rt_uint32_t hres, rt_uint32_t vres, rt_uint32_t hoffset, rt_uint32_t voffset);

// void hdmi_putc(rt_uint8_t c);
// int rt_hw_hdmi_term_init();

#endif