#include "drv_jpeg.h"

#include <rtthread.h>
#include <stdlib.h>

#include "drv_ddr_dma.h"
#include "rttypes.h"

#define LOG_TAG "drv.jpeg"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>


#define JPEG_BASE 0xbfe50000

#define JPEG_REG_EN (*(volatile rt_uint32_t *)(JPEG_BASE + 0x0))
#define JPEG_REG_BLOCK_CNT (*(volatile rt_uint32_t *)(JPEG_BASE + 0x4))
#define JPEG_REG_SIZE (*(volatile rt_uint32_t *)(JPEG_BASE + 0xc))
#define JPEG_REG_INPUT_WIDTH (*(volatile rt_uint32_t *)(JPEG_BASE + 0x10))
#define JPEG_REG_INPUT_HEIGHT (*(volatile rt_uint32_t *)(JPEG_BASE + 0x14))
#define JPEG_REG_INPUT_LEFT (*(volatile rt_uint32_t *)(JPEG_BASE + 0x18))
#define JPEG_REG_INPUT_TOP (*(volatile rt_uint32_t *)(JPEG_BASE + 0x1c))
#define JPEG_REG_INPUT_UPDATE (*(volatile rt_uint32_t *)(JPEG_BASE + 0x20))

void jpeg_start() { JPEG_REG_EN = 1; }

void jpeg_stop() { JPEG_REG_EN = 0; }

rt_uint8_t jpeg_get_status() { return JPEG_REG_EN; }

rt_uint32_t jpeg_get_block_cnt() { return JPEG_REG_BLOCK_CNT; }

rt_uint32_t jpeg_get_size() { return JPEG_REG_SIZE; }

void jpeg_set_input(rt_uint32_t width, rt_uint32_t height, rt_uint32_t left,
                    rt_uint32_t top) {
  JPEG_REG_INPUT_WIDTH = width;
  JPEG_REG_INPUT_HEIGHT = height;
  JPEG_REG_INPUT_LEFT = left;
  JPEG_REG_INPUT_TOP = top;
  JPEG_REG_INPUT_UPDATE = 1;
}

void set_jpeg_src(rt_uint32_t addr, rt_uint32_t width, rt_uint32_t height) {
  rt_uint16_t jpeg_width, jpeg_height;
  jpeg_width = 1024;
  jpeg_height = 768;
  if (width > jpeg_width || height > jpeg_height) {
    rt_kprintf("Invalid input size\n");
    return;
  }
  ddr_dma_set_rd_ch_addr_immediate(1, addr, width * height);
  ddr_dma_set_rd_graph_width_immediate(1, width);

  jpeg_set_input(width, height, (jpeg_width - width) / 2,
                 (jpeg_height - height) / 2);
}

int jpeg_init() {
  set_jpeg_src(0, 640, 640);
  ddr_dma_set_wr_ch_addr(1, 640 * 640, 640 * 640);
  LOG_I("JPEG init done\n");
  return 0;
}
INIT_APP_EXPORT(jpeg_init);

static int cmd_jpeg(int argc, char *argv[]) {
  char *arg1;
  rt_uint8_t arg2;
  switch (argc) {
  case 2:
    arg1 = argv[1];
    // if (strcmp(arg1, "init") == 0) {
    //   jpeg_init();
    //   break;
    // }
    if (strcmp(arg1, "start") == 0) {
      jpeg_start();
      break;
    }
    if (strcmp(arg1, "stop") == 0) {
      // jpeg_stop();
      for (int i = 0; i < 128; i++) {
        JPEG_DATA_PTR[i] = 0;
        rt_kprintf("%08x ", JPEG_DATA_PTR[i]);
        if (i % 8 == 7) {
          rt_kprintf("\n");
        }
      }
      rt_kprintf("\n");
      break;
    }
    if (strcmp(arg1, "status") == 0) {
      rt_kprintf("JPEG status: %d\n", jpeg_get_status());
      break;
    }
    if (strcmp(arg1, "wait") == 0) {
      ddr_dma_reset_wr_ch(1);
      rt_thread_delay(1);
      jpeg_start();
      while (jpeg_get_status() == 1) {
        rt_kprintf("JPEG status: %d block_cnt: %d\n", jpeg_get_status(),
                   jpeg_get_block_cnt());
      }
      for (int i = 0; i < 128; i++) {
        rt_kprintf("%08x ", JPEG_DATA_PTR[i]);
        if (i % 8 == 7) {
          rt_kprintf("\n");
        }
      }
      rt_kprintf("\n");
      break;
    }
    break;
  case 5:
    arg1 = argv[1];
    if (strcmp(arg1, "src") == 0) {
      set_jpeg_src(strtol(argv[2], NULL, 0), strtol(argv[3], NULL, 0),
                   strtol(argv[4], NULL, 0));
      break;
    }
    break;
  default:
    rt_kprintf(
        "usage : cmd_jpeg CMD\n  CMD: init | start | stop | status | wait | "
        "input WIDTH HEIGHT LEFT TOP\n");
    break;
  }
  return 0;
}
MSH_CMD_EXPORT(cmd_jpeg, cmd_jpeg);
