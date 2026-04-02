#ifndef HDMI_H
#define HDMI_H

#include <rtthread.h>

#define H_NUM 1920
#define V_NUM 1080
#define VBUF_NUM 3

extern uint8_t vbuf[VBUF_NUM][V_NUM][H_NUM][3];

void hdmi_init();
void vdma_set_frame(size_t number);

#endif
