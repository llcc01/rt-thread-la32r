
#include "rtconfig.h"
#include "rtthread.h"
#include <lvgl.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <lv_display_private.h>

#include "hdmi.h"

#define DBG_TAG "LVGL_APP"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#ifndef BSP_USING_LVGL_DEMO

volatile uint8_t gui_init = 0;

void lv_user_gui_init(void)
{
    LOG_I("lv_user_gui_init");

    gui_init = 1;
}

#endif
