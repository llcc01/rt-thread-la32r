#ifndef __DRV_HDMI_H__
#define __DRV_HDMI_H__

#include <rtthread.h>

#define HDMI_RES *(volatile int*)0xbfe80000
#define HDMI_MODE *(volatile int*)0xbfe80004
#define HDMI_TERM *(volatile int*)0xbfe80008

void hdmi_set_res(uint32_t res);
void hdmi_set_mode(uint32_t mode);
void hdmi_put_char(uint8_t c);

#endif