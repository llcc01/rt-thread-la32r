#include "soc_spi.h"
#include "rtthread.h"
#include "rttypes.h"

/*** SPI READ OPTIONS ***/
#define NORMAL 0x0
#define FAST 0x1
#define DUAL_IO 0x2

/**
 * @Description: SPI
 * 分频系数配置。分别配置到控制寄存器SPCR、外部寄存器SPER、参数控制寄存器PARAM
 * @Input: 分频系数
 * @Output: 无
 * @return {*}无
 * @param {uint8_t} spr 分频系数
 */
void soc_Spi_FreqDiv(rt_uint8_t spr) { // spr 低四位有效
  SPI->SPCR = (spr & 0x03);
  SPI->SPER = ((spr >> 2) & 0x03);
}

/**
 * @Description: spi init
 * @Input: spi 分频系数
 * SPI_DIV_2 SPI_DIV_4 SPI_DIV_16  SPI_DIV_32  SPI_DIV_8 SPI_DIV_64  SPI_DIV_128
 * SPI_DIV_256 SPI_DIV_512 SPI_DIV_1024 SPI_DIV_2048 SPI_DIV_4096
 * @Output: 无
 * @return {*}
 * @param {uint8_t} spr 分频系数。固定为以上的宏定义
 */
void soc_Spi_Init(rt_uint8_t spr) {
  soc_Spi_FreqDiv(spr);

  // 系统工作使能，master 模式，
  SPI->SPCR |= 0x50; // 0b0101_0000
                     // 系统工作使能，master 模式，

  while (!SPI_TXEMPTY)
    ; // wait till tx empty // 阻塞直到写寄存器为空
  while (!SPI_RXEMPTY)
    ; // read till rx empty // 阻塞直到读寄存器为空
}

void soc_Spi_Write_Read_1to4(const rt_uint8_t *buf_tx, rt_uint8_t *buf_rx,
                             int n) {
  rt_uint32_t i;
  for (i = 0; i < n; i++) {
    SPI->DATA = buf_tx[i]; // 向 DATA 写入最多四个字节的数据
    // rt_kprintf("buf_tx[%d]: %d\n", i, buf_tx[i]);
  }
  while (!SPI_TXEMPTY)
    ; // 阻塞直到写寄存器为空，即发送结束

  for (i = 0; i < n; i++) { // 从 DATA 读出数据
    buf_rx[i] = SPI->DATA;  // received high bits first, so placed in the
    // high bits of the array
    // rt_kprintf("buf_rx[%d]: %d\n", i, buf_rx[i]);
  }
}

void soc_Spi_Write_1to4(const rt_uint8_t *buf_tx, int n) {
  rt_uint32_t i;
  for (i = 0; i < n; i++) {
    SPI->DATA = buf_tx[i]; // 向 DATA 写入最多四个字节的数据
    // rt_kprintf("buf_tx[%d]: %d\n", i, buf_tx[i]);
  }
  while (!SPI_TXEMPTY)
    ; // 阻塞直到写寄存器为空，即发送结束

  for (i = 0; i < n; i++) {   // 从 DATA 读出数据
    rt_uint8_t t = SPI->DATA; // received high bits first, so placed in the
    // high bits of the array
    // rt_kprintf("buf_rx[%d]: %d\n", i, t);
  }
}

void soc_Spi_Write_Read(const rt_uint8_t *buf_tx, rt_uint8_t *buf_rx, int n) {
  rt_uint32_t i;
  // rt_kprintf("n: %d\n", n);
  const rt_uint32_t first_times = n / 4 * 4;
  if (buf_rx) {
    for (i = 0; i < first_times; i += 4) {
      soc_Spi_Write_Read_1to4(buf_tx + i, buf_rx + i, 4);
    }
    if (i < n) {
      soc_Spi_Write_Read_1to4(buf_tx + i, buf_rx + i, n - i);
    }
  } else {
    for (i = 0; i < first_times; i += 4) {
      soc_Spi_Write_1to4(buf_tx + i, 4);
    }
    if (i < n) {
      soc_Spi_Write_1to4(buf_tx + i, n - i);
    }
  }
}

/**
 * @Description: SPI 片选信号1使能并拉低
 * @Input: 无
 * @Output: 无
 * @return {*}
 */
void soc_Spi_Cs_Down(rt_uint8_t n) {
  SPI_ENABLE_CS(n); // 片选使能，高有效
  SPI_LOW_CS(n);    // 片选拉低，低有效
}

/**
 * @Description: SPI 片选信号1使能并拉高
 * @Input: 无
 * @Output: 无
 * @return {*}
 */
void soc_Spi_Cs_Up(rt_uint8_t n) {
  SPI_ENABLE_CS(n); // 片选使能，高有效
  SPI_HIGH_CS(n);   // 片选拉高，低有效
}

/**
 * @Description: SPI 发送数据
 * @Input: 数据缓冲；数据数量
 * @Output: 无
 * @return {*}
 * @param {uint8_t*} buf 数据缓冲
 * @param {int} n		数据量
 */
// void soc_Spi_Send(uint8_t* buf, int n) {  // 先发送高位，再发送低位
//   soc_Spi_Cs_Down();                      // 片选拉低，低有效
//   soc_Spi_Write_Read_1to4(buf, n);
//   soc_Spi_Cs_Up();
// }
