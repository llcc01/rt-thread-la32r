#ifndef __TIMER_H__
#define __TIMER_H__

#include <rtthread.h>

void Set_soft_int(void);
void Set_soft_stop(void);
void Set_Timer_stop(void);
void Set_Timer_clear(void);
void Set_Timer_Init(uint32_t timer);

#endif