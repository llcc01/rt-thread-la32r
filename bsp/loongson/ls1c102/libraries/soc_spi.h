#ifndef _SOC_SPI_H
#define _SOC_SPI_H

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#define SPI_BASE 0xbfe70000  // spi regs

//******************  SPI REGS  ******************/
typedef struct {
  volatile rt_uint8_t SPCR;   /* 控制寄存器 */
  volatile rt_uint8_t SPSR;   /* 状态寄存器 */
  volatile rt_uint8_t DATA;   /* 数据寄存器 */
  volatile rt_uint8_t SPER;   /* 外部寄存器 */
  volatile rt_uint8_t PARAM;  /* 参数控制寄存器 */
  volatile rt_uint8_t SOFTCS; /* 片选控制寄存器 */
  volatile rt_uint8_t TIMING; /* 时序控制寄存器 */
} SPI_TypeDef;

#define SPI ((SPI_TypeDef *)SPI_BASE)
#define SPI_SPCR *(volatile unsigned char *)(SPI_BASE + 0x00)  // 控制寄存器
#define SPI_SPSR *(volatile unsigned char *)(SPI_BASE + 0x01)  // 状态寄存器
#define SPI_TxFIFO *(volatile unsigned char *)(SPI_BASE + 0x02)  // 数据寄存器
#define SPI_RxFIFO *(volatile unsigned char *)(SPI_BASE + 0x02)  // 数据寄存器
#define SPI_SPER *(volatile unsigned char *)(SPI_BASE + 0x03)  // 外部寄存器
#define SPI_SFC_PARAM \
  *(volatile unsigned char *)(SPI_BASE + 0x04)  // 参数控制寄存器
#define SPI_SFC_SOFTCS \
  *(volatile unsigned char *)(SPI_BASE + 0x05)  // 片选控制寄存器
#define SPI_SFC_TIMING \
  *(volatile unsigned char *)(SPI_BASE + 0x06)  // 时序控制寄存器

#define SPI_INT_ENABLE (SPI->SPCR |= 0x80)      // SPI 控制器中断使能
#define SPI_INT_DISABLE (SPI->SPCR &= ~(0x80))  // SPI 控制器中断失能

#define SPI_ENABLE (SPI->SPCR |= 0x40)      // SPI 控制器使能
#define SPI_DISABLE (SPI->SPCR &= ~(0x40))  // SPI 控制器停止

#define SPI_MODE_MASTER (SPI->SPCR |= 0x10)
#define SPI_MODE_SLAVE (SPI->SPCR &= ~(0x10))

#define SPI_CPOL_HIGH (SPI->SPCR |= 0x08)
#define SPI_CPOL_LOW (SPI->SPCR &= ~(0x08))

#define SPI_CPHA_REV (SPI->SPCR |= 0x04)
#define SPI_CPHA_SAME (SPI->SPCR &= ~(0x04))

#define SPI_TXEMPTY (SPI->SPSR & 0x04)
#define SPI_RXEMPTY (SPI->SPSR & 0x01)
#define SPI_TXFULL (SPI->SPSR & 0x08)
#define SPI_RXFULL (SPI->SPSR & 0x02)

#define SPI_MODE_COMPATIBILIY (SPI->SPER |= 0x04)
#define SPI_MODE_STAND (SPI->SPER &= ~(0x04))

#define SPI_TRANST_1BYTE (SPI->SPER &= 0x3F)
#define SPI_TRANST_2BYTE (SPI->SPER |= (1 << 6))
#define SPI_TRANST_3BYTE (SPI->SPER |= (2 << 6))
#define SPI_TRANST_4BYTE (SPI->SPER |= (3 << 6))

#define SPI_ENABLE_CS(n) (SPI->SOFTCS |= (1 << n))
#define SPI_DISABLE_CS(n) (SPI->SOFTCS &= ~(1 << n))
#define SPI_LOW_CS(n) (SPI->SOFTCS &= ~(1 << (n + 4)))
#define SPI_HIGH_CS(n) (SPI->SOFTCS |= (1 << (n + 4)))

#define SPI_DIV_2 0b0000
#define SPI_DIV_4 0b0001
#define SPI_DIV_16 0b0010
#define SPI_DIV_32 0b0011
#define SPI_DIV_8 0b0100
#define SPI_DIV_64 0b0101
#define SPI_DIV_128 0b0110
#define SPI_DIV_256 0b0111
#define SPI_DIV_512 0b1000
#define SPI_DIV_1024 0b1001
#define SPI_DIV_2048 0b1010
#define SPI_DIV_4096 0b1011

void soc_Spi_FreqDiv(rt_uint8_t spr);
void soc_Spi_Init(rt_uint8_t spr);
void soc_Spi_Cs_Down(rt_uint8_t n);
void soc_Spi_Cs_Up(rt_uint8_t n);
void soc_Spi_Write_Read_1to4(const rt_uint8_t *buf_tx, rt_uint8_t *buf_rx, int n);
void soc_Spi_Write_Read(const rt_uint8_t *buf_tx, rt_uint8_t *buf_rx, int n);
// void soc_Spi_Send(uint8_t *buf, int n);

#endif  // _SOC_SPI_H
