#include "drv_ddr_dma.h"

#include <rtthread.h>

#define DDR_DMA_BASE_ADDR 0xbfee0000

#define DDR_DMA_REG(offset)                                                    \
  (*(volatile rt_uint32_t *)((rt_uint8_t *)DDR_DMA_BASE_ADDR + (offset)))

#define DDR_DMA_REG_WR_B(ch) DDR_DMA_REG(0x0 + ch * 0x8)
#define DDR_DMA_REG_WR_E(ch) DDR_DMA_REG(0x4 + ch * 0x8)

#define DDR_DMA_REG_RD_B(ch) DDR_DMA_REG(0x100 + ch * 0x8)
#define DDR_DMA_REG_RD_E(ch) DDR_DMA_REG(0x104 + ch * 0x8)

#define DDR_DMA_REG_WR_UPDATE DDR_DMA_REG(0x0200)
#define DDR_DMA_REG_RD_UPDATE DDR_DMA_REG(0x0204)

#define DDR_DMA_REG_RD_GRAPH_WIDTH(ch) DDR_DMA_REG(0x0300 + ch * 0x4)

#define DDR_DMA_REG_WR_SOFT_RST(ch) DDR_DMA_REG(0x0400 + ch * 0x4)
#define DDR_DMA_REG_RD_SOFT_RST(ch) DDR_DMA_REG(0x0410 + ch * 0x4)

#define DDR_DMA_A 0
#define DDR_DMA_B 1
#define DDR_DMA_C 2
#define DDR_DMA_D 3

void ddr_dma_set_wr_ch_addr(rt_uint8_t ch, rt_uint32_t addr, rt_uint32_t len) {
  if (ch >= 4) {
    return;
  }
  DDR_DMA_REG_WR_B(ch) = addr;
  DDR_DMA_REG_WR_E(ch) = addr + len;
  DDR_DMA_REG_WR_UPDATE = 1 << ch;
}

void ddr_dma_reset_wr_ch(rt_uint8_t ch) {
  if (ch >= 4) {
    return;
  }
  DDR_DMA_REG_WR_SOFT_RST(ch) = 1;
  rt_thread_delay(1);
  DDR_DMA_REG_WR_SOFT_RST(ch) = 0;
}

void ddr_dma_set_wr_ch_addr_immediate(rt_uint8_t ch, rt_uint32_t addr,
                                      rt_uint32_t len) {
  if (ch >= 4) {
    return;
  }
  DDR_DMA_REG_WR_B(ch) = addr;
  DDR_DMA_REG_WR_E(ch) = addr + len;
  DDR_DMA_REG_WR_SOFT_RST(ch) = 1;
  rt_thread_delay(1);
  DDR_DMA_REG_WR_SOFT_RST(ch) = 0;
}

void ddr_dma_set_rd_ch_addr(rt_uint8_t ch, rt_uint32_t addr, rt_uint32_t len) {
  if (ch >= 4) {
    return;
  }
  DDR_DMA_REG_RD_B(ch) = addr;
  DDR_DMA_REG_RD_E(ch) = addr + len;
  DDR_DMA_REG_RD_UPDATE = 1 << ch;
}

void ddr_dma_reset_rd_ch(rt_uint8_t ch) {
  if (ch >= 4) {
    return;
  }
  DDR_DMA_REG_RD_SOFT_RST(ch) = 1;
  rt_thread_delay(1);
  DDR_DMA_REG_RD_SOFT_RST(ch) = 0;
}

void ddr_dma_set_rd_ch_addr_immediate(rt_uint8_t ch, rt_uint32_t addr,
                                      rt_uint32_t len) {
  if (ch >= 4) {
    return;
  }
  DDR_DMA_REG_RD_B(ch) = addr;
  DDR_DMA_REG_RD_E(ch) = addr + len;
  DDR_DMA_REG_RD_SOFT_RST(ch) = 1;
  rt_thread_delay(1);
  DDR_DMA_REG_RD_SOFT_RST(ch) = 0;
}

void ddr_dma_set_rd_graph_width(rt_uint8_t ch, rt_uint32_t width) {
  if (ch >= 4) {
    return;
  }
  DDR_DMA_REG_RD_GRAPH_WIDTH(ch) = width;
  DDR_DMA_REG_RD_UPDATE = 1 << ch;
}

void ddr_dma_set_rd_graph_width_immediate(rt_uint8_t ch, rt_uint32_t width) {
  if (ch >= 4) {
    return;
  }
  DDR_DMA_REG_RD_GRAPH_WIDTH(ch) = width;
  DDR_DMA_REG_RD_SOFT_RST(ch) = 1;
  rt_thread_delay(1);
  DDR_DMA_REG_RD_SOFT_RST(ch) = 0;
}
