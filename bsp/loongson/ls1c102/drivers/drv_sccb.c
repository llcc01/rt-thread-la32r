#include "drv_i2c.h"

#include <drivers/i2c.h>
#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#include "drv_sccb.h"
#include "rttypes.h"
#include "soc_i2c.h"

#define DBG_LVL DBG_WARNING
#define DBG_TAG "drv/sccb"
#include <rtdbg.h>

#define ATK_MC5640_SCCB_ADDR 0x3C

static void atk_mc5640_write_reg(rt_uint16_t reg, rt_uint8_t dat) {
  soc_sccb_3_phase_write(ATK_MC5640_SCCB_ADDR, reg, dat);
}

rt_uint8_t atk_mc5640_set_output_size(rt_uint16_t width, rt_uint16_t height) {
  rt_uint8_t reg3808;
  rt_uint8_t reg3809;
  rt_uint8_t reg380A;
  rt_uint8_t reg380B;

  reg3808 = (rt_uint8_t)(width >> 8) & 0x0F;
  reg3809 = (rt_uint8_t)width & 0xFF;

  reg380A = (rt_uint8_t)(height >> 8) & 0x07;
  reg380B = (rt_uint8_t)height & 0xFF;

  atk_mc5640_write_reg(0x3212, 0x03);
  atk_mc5640_write_reg(0x3808, reg3808);
  atk_mc5640_write_reg(0x3809, reg3809);
  atk_mc5640_write_reg(0x380A, reg380A);
  atk_mc5640_write_reg(0x380B, reg380B);
  atk_mc5640_write_reg(0x3212, 0x13);
  atk_mc5640_write_reg(0x3212, 0xA3);

  return 0;
}

rt_uint8_t atk_mc5640_set_sensor_location(rt_uint16_t x, rt_uint16_t y) {
  rt_uint8_t reg3810;
  rt_uint8_t reg3811;
  rt_uint8_t reg3812;
  rt_uint8_t reg3813;

  reg3810 = (rt_uint8_t)(x >> 8) & 0x0F;
  reg3811 = (rt_uint8_t)x & 0xFF;

  reg3812 = (rt_uint8_t)(y >> 8) & 0x07;
  reg3813 = (rt_uint8_t)y & 0xFF;

  atk_mc5640_write_reg(0x3212, 0x03);
  atk_mc5640_write_reg(0x3810, reg3810);
  atk_mc5640_write_reg(0x3811, reg3811);
  atk_mc5640_write_reg(0x3812, reg3812);
  atk_mc5640_write_reg(0x3813, reg3813);
  atk_mc5640_write_reg(0x3212, 0x13);
  atk_mc5640_write_reg(0x3212, 0xA3);

  return 0;
}

rt_uint8_t atk_mc5640_set_output_format() {
  rt_uint32_t cfg_index;

  for (cfg_index = 0; cfg_index < sizeof(atk_mc5640_rgb565_cfg) /
                                      sizeof(atk_mc5640_rgb565_cfg[0]);
       cfg_index++) {
    atk_mc5640_write_reg(atk_mc5640_rgb565_cfg[cfg_index].reg,
                         atk_mc5640_rgb565_cfg[cfg_index].dat);
  }

  return 0;
}

/**
 * @brief       初始化ATK-MC5640寄存器配置
 * @param       无
 * @retval      无
 */
static void atk_mc5640_init_reg(void) {
  rt_uint32_t cfg_index;

  for (cfg_index = 0;
       cfg_index < sizeof(atk_mc5640_init_cfg) / sizeof(atk_mc5640_init_cfg[0]);
       cfg_index++) {
    atk_mc5640_write_reg(atk_mc5640_init_cfg[cfg_index].reg,
                         atk_mc5640_init_cfg[cfg_index].dat);
  }
}

void set_test_cfg() {
  rt_uint32_t cfg_index;

  for (cfg_index = 0;
       cfg_index < sizeof(test_cfg) / sizeof(test_cfg[0]);
       cfg_index++) {
    atk_mc5640_write_reg(test_cfg[cfg_index].reg,
                         test_cfg[cfg_index].dat);
  }
}

int rt_hw_sccb_init(void) {
  soc_sccb_set_config_done_flag(0);

  soc_sccb_wait_power_ready();

  atk_mc5640_init_reg();

  atk_mc5640_set_output_format();

  // ISP input 1920*1920
  atk_mc5640_set_sensor_location(465, 4);

  // ISP output 640*640
  atk_mc5640_set_output_size(640, 640);

  soc_sccb_set_config_done_flag(1);

  return 0;
}
INIT_BOARD_EXPORT(rt_hw_sccb_init);
