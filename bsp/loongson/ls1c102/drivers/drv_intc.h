#ifndef __DRV_INTC_H__
#define __DRV_INTC_H__

#include <rtthread.h>

#define INTC_MAX_INTR 8

#define INTC_BASE 0xbfea0000  // Interrupt_Regs_Baseadd

#define INT_EN *(volatile unsigned char *)(INTC_BASE + 0x00)  // 中断使能寄存器
#define INT_CLR *(volatile unsigned char *)(INTC_BASE + 0x03)  // 中断清除寄存器
#define INT_OUT *(volatile unsigned char *)(INTC_BASE + 0x05)  // 中断输出寄存器

#define DMA_INT_EN 0x80
#define VPWM_INT_EN 0x40
#define SPI_INT_EN 0x20
#define FLASH_INT_EN 0x10
#define UART0_INT_EN 0x08
#define UART1_INT_EN 0x04
#define I2C_INT_EN 0x02
#define TIMER_INT_EN 0x01

#define DMA_INT_CLR 0x80
#define VPWM_INT_CLR 0x40
#define SPI_INT_CLR 0x20
#define FLASH_INT_CLR 0x10
#define UART0_INT_CLR 0x08
#define UART1_INT_CLR 0x04
#define I2C_INT_CLR 0x02
#define TIMER_INT_CLR 0x01

#define DMA_INT_OUT 0x80
#define VPWM_INT_OUT 0x40
#define SPI_INT_OUT 0x20
#define FLASH_INT_OUT 0x10
#define UART0_INT_OUT 0x08
#define UART1_INT_OUT 0x04
#define I2C_INT_OUT 0x02
#define TIMER_INT_OUT 0x01

#define DMA_INT_EDGE 0x80
#define VPWM_INT_EDGE 0x40
#define SPI_INT_EDGE 0x20
#define FLASH_INT_EDGE 0x10
#define UART0_INT_EDGE 0x08
#define UART1_INT_EDGE 0x04
#define I2C_INT_EDGE 0x02
#define TIMER_INT_EDGE 0x01

#define DMA_INT_POL 0x80
#define VPWM_INT_POL 0x40
#define SPI_INT_POL 0x20
#define FLASH_INT_POL 0x10
#define UART0_INT_POL 0x08
#define UART1_INT_POL 0x04
#define I2C_INT_POL 0x02
#define TIMER_INT_POL 0x01

#define DMA_INT_SET 0x80
#define VPWM_INT_SET 0x40
#define SPI_INT_SET 0x20
#define FLASH_INT_SET 0x10
#define UART0_INT_SET 0x08
#define UART1_INT_SET 0x04
#define I2C_INT_SET 0x02
#define TIMER_INT_SET 0x01

rt_isr_handler_t rt_hw_intc_interrupt_install(int vector,
                                              rt_isr_handler_t handler,
                                              void *param, const char *name);

int rt_hw_intc_init(void);

void rt_hw_intc_interrupt_umask(int irq);

void rt_hw_intc_interrupt_mask(int irq);

#endif