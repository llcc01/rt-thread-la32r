#ifndef __DRV_TIMER_H__
#define __DRV_TIMER_H__

#include <rtthread.h>

#define LA_TIMER (8 * 1000 * 1000 / RT_TICK_PER_SECOND)

void rt_hw_timer_handler(int vector, void *param);

int rt_hw_timer_init(void);

#endif