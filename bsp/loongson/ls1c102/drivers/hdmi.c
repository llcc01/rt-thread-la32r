
#include <rtthread.h>
#include <stddef.h>
#include <string.h>

#include "xgpio.h"

// #include "picset.h"

#include "hdmi.h"

#define VDAM_BASE 0xbf010000

#define VDMA_REG(offset) (*(volatile uint32_t *)(VDAM_BASE + offset))

#define VDMA_REG_MM2S_VDMACR        VDMA_REG(0)
#define VDMA_REG_MM2S_PTR           VDMA_REG(0x28)
#define VDMA_REG_MM2S_VSIZE         VDMA_REG(0x50)
#define VDMA_REG_MM2S_HSIZE         VDMA_REG(0x54)
#define VDMA_REG_MM2S_FRMDLY_STRIDE VDMA_REG(0x58)
#define VDMA_REG_MM2S_START_ADDR1   VDMA_REG(0x5c)
#define VDMA_REG_MM2S_START_ADDR2   VDMA_REG(0x60)
#define VDMA_REG_MM2S_START_ADDR3   VDMA_REG(0x64)

#define VDMA_FLAG_MM2S_VDMACR_GENLOCKSRC (1 << 7)
#define VDMA_FLAG_MM2S_VDMACR_GENLOCKEN  (1 << 3)
#define VDMA_FLAG_MM2S_VDMACR_CIRCULAR   (1 << 1)
#define VDMA_FLAG_MM2S_VDMACR_RUN        (1 << 0)


__attribute__((aligned(64))) uint8_t vbuf[VBUF_NUM][V_NUM][H_NUM][3] = {};

void vdma_init()
{
    VDMA_REG_MM2S_VDMACR = VDMA_FLAG_MM2S_VDMACR_GENLOCKSRC | VDMA_FLAG_MM2S_VDMACR_GENLOCKEN | VDMA_FLAG_MM2S_VDMACR_RUN;

    VDMA_REG_MM2S_START_ADDR1 = (size_t)&vbuf[0];
    VDMA_REG_MM2S_START_ADDR2 = (size_t)&vbuf[1];
    VDMA_REG_MM2S_START_ADDR3 = (size_t)&vbuf[2];

    VDMA_REG_MM2S_FRMDLY_STRIDE = H_NUM * 3;
    VDMA_REG_MM2S_HSIZE = H_NUM * 3;
    VDMA_REG_MM2S_VSIZE = V_NUM;
}

void vdma_set_frame(size_t number)
{
    VDMA_REG_MM2S_PTR = number & 0x1f;
}

void hdmi_init()
{
    xgpio_init();
    vdma_init();
    xgpio_set(0);
    xgpio_set(1);
    memset(vbuf[0], 0x7f, sizeof(vbuf[0]));
    // for (size_t c = 0; c < RESIZED_C; c++) {
    //     for (size_t y = 0; y < RESIZED_H; y++) {
    //         for (size_t x = 0; x < RESIZED_W; x++) {
    //             uint8_t v = ((float *)picset9_0000)[c * RESIZED_H * RESIZED_W + y * RESIZED_W + x] * 255;
    //             for (size_t y_offset = 0; y_offset < 4; y_offset++) {
    //                 for (size_t x_offset = 0; x_offset < 4; x_offset++) {
    //                     vbuf[0][y*4+y_offset][x*4+x_offset][2-c] = v;
    //                 }
    //             }
    //         }
    //     }
    // }
}
