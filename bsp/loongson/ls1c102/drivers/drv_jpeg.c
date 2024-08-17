#include "drv_jpeg.h"

#include <rtthread.h>

#include "drv_ddr_dma.h"

#define JPEG_BASE 0xbfe50000

#define JPEG_REG_EN (*(volatile rt_uint32_t *)(JPEG_BASE + 0x0))
#define JPEG_REG_BLOCK_CNT (*(volatile rt_uint32_t *)(JPEG_BASE + 0x4))

#define JPEG_DATA_ADDR ((rt_uint32_t *)(0x80000000 + 640 * 640 * 1 * 4))

void jpeg_start() { JPEG_REG_EN = 1; }

void jpeg_stop() { JPEG_REG_EN = 0; }

rt_uint8_t jpeg_get_status() { return JPEG_REG_EN; }

rt_uint32_t jpeg_get_block_cnt() { return JPEG_REG_BLOCK_CNT; }

static int cmd_jpeg(int argc, char *argv[]) {
  char *arg1;
  rt_uint8_t arg2;
  switch (argc) {
  case 2:
    arg1 = argv[1];
    if (strcmp(arg1, "start") == 0) {
      jpeg_start();
      break;
    }
    if (strcmp(arg1, "stop") == 0) {
      jpeg_stop();
      break;
    }
    if (strcmp(arg1, "status") == 0) {
      rt_kprintf("JPEG status: %d\n", jpeg_get_status());
      break;
    }
    if (strcmp(arg1, "wait") == 0) {
      ddr_dma_set_rd_ch_addr(1, 0, 640 * 640);
      ddr_dma_set_rd_graph_width(1, 640);
      ddr_dma_set_wr_ch_addr(1, 640 * 640, 640 * 640);
      jpeg_start();
      while (jpeg_get_status() == 1) {
        rt_kprintf("JPEG status: %d block_cnt: %d\n", jpeg_get_status(),
                   jpeg_get_block_cnt());
      }
      for (int i = 0; i < 16; i++) {
        rt_kprintf("%08x ", JPEG_DATA_ADDR[i]);
        if (i % 4 == 3) {
          rt_kprintf("\n");
        }
      }
      rt_kprintf("\n");
      break;
    }
    break;
  default:
    rt_kprintf("usage : cmd_jpeg CMD\n  CMD: start, stop, status, wait\n");
    break;
  }
  return 0;
}
MSH_CMD_EXPORT(cmd_jpeg, cmd_jpeg);
