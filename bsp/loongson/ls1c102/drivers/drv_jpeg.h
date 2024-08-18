#ifndef __DRV_JPEG_H__
#define __DRV_JPEG_H__

#include <rtthread.h>

#define JPEG_DATA_PTR ((volatile rt_uint32_t *)(0x80000000 + 640 * 640 * 1 * 4))

void jpeg_start();
void jpeg_stop();

rt_uint8_t jpeg_get_status();
rt_uint32_t jpeg_get_block_cnt();
rt_uint32_t jpeg_get_size();

#endif // __DRV_JPEG_H__
