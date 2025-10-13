#include "drv_i2c.h"

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>
#include <drivers/i2c.h>

#include "soc_i2c.h"

#define DBG_LVL DBG_WARNING
#define DBG_TAG "drv/i2c"
#include <rtdbg.h>

#ifdef RT_USING_I2C

static rt_ssize_t i2c_master_xfer(struct rt_i2c_bus_device *bus,
                                  struct rt_i2c_msg *msgs, rt_uint32_t num) {}

static const struct rt_i2c_bus_device_ops i2c_ops = {i2c_master_xfer, RT_NULL,
                                                     RT_NULL};

I2C_InitTypeDef I2C_InitStruct0;
static struct rt_i2c_bus_device i2c0 = {
    .ops = &i2c_ops,
};

int rt_hw_i2c_init(void) {
  soc_I2C_StructInit(&I2C_InitStruct0);
  soc_I2C_Init(&I2C_InitStruct0);

  if (rt_i2c_bus_device_register(&i2c0, "i2c0") != RT_EOK) {
    LOG_E("i2c bus register:%s failure\n", "i2c0");
    return -1;
  }

  return 0;
}
INIT_BOARD_EXPORT(rt_hw_i2c_init);

#endif /* RT_USING_I2C */
