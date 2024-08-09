#ifndef __DRV_DDR_DMA_H__
#define __DRV_DDR_DMA_H__

#include <rtthread.h>

void ddr_dma_set_rd_ch_addr(rt_uint8_t ch, rt_uint32_t addr, rt_uint32_t len);
void ddr_dma_set_wr_ch_addr(rt_uint8_t ch, rt_uint32_t addr, rt_uint32_t len);

#endif
