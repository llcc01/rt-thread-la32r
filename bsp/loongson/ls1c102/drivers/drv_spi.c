#include "drv_spi.h"

#include <drivers/spi.h>
#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_SPI

#include "soc_spi.h"

struct spi_cs_t {
  unsigned char cs;
};

static rt_err_t configure(struct rt_spi_device *device,
                          struct rt_spi_configuration *configuration) {
  rt_uint8_t spr = SPI_DIV_2;
  soc_Spi_Init(spr);
  return RT_EOK;
}

static rt_ssize_t xfer(struct rt_spi_device *device,
                       struct rt_spi_message *message) {
  struct spi_cs_t *spi_cs = (struct spi_cs_t *)device->parent.user_data;
  if (message->cs_take) {
    soc_Spi_Cs_Down(spi_cs->cs);
  }

  const rt_uint8_t *send_ptr = message->send_buf;
  rt_uint8_t *recv_ptr = message->recv_buf;
  const rt_uint32_t size = message->length;
  soc_Spi_Write_Read(send_ptr, recv_ptr, size);

  if (message->cs_release) {
    soc_Spi_Cs_Up(spi_cs->cs);
  }

  return size;
}

static struct rt_spi_bus spi_bus0;
static struct rt_spi_device spi_device1;
static struct rt_spi_device spi_device2;
static struct spi_cs_t spi_cs1 = {.cs = 1};
static struct spi_cs_t spi_cs2 = {.cs = 2};
static struct rt_spi_ops spi_ops = {.configure = configure, .xfer = xfer};

int rt_hw_spi_init(void) {
  rt_spi_bus_register(&spi_bus0, "spi0", &spi_ops);

  rt_spi_bus_attach_device(&spi_device1, "spi01", "spi0", &spi_cs1);
  rt_spi_bus_attach_device(&spi_device2, "spi02", "spi0", &spi_cs2);

  return 0;
}
INIT_BOARD_EXPORT(rt_hw_spi_init);

#endif
