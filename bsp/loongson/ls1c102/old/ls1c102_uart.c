/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName: 	 ls1c102_uart.c
* Desc:
*
*
* Author: 	 zhuangwei
* Date: 	 2018-12-16
* Notes:
*
* -----------------------------------------------------------------
* Histroy: v1.0   2018-12-16, zhuangwei create this file
*
******************************************************************************/


/*------------------------------- Includes ----------------------------------*/
#include "ls1c102.h"
#include "ls1c102_uart.h"
#include "ls1c102_clock.h"


/*------------------- Global Definitions and Declarations -------------------*/
typedef struct
{
    unsigned char bec:2;
    unsigned char sb:1;
    unsigned char pe:1;
    unsigned char eps:1;
    unsigned char spd:1;
    unsigned char bcb:1;
    unsigned char dlab:1;
}ls1c102_uart_LCR_t;


/*---------------------- Constant / Macro Definitions -----------------------*/
/* FCR */
#define UART_FCR_RXRST                      0x2
#define UART_FCR_TXRST                      0x4
#define UART_FCR_TRIGGER(x)                 ((unsigned char)x<<3)
#define UART_FCR_TRIGGER_MAX                16

/* LSR */
#define UART_LSR_DR                         0x01//接收FIFO有数据
#define UART_LSR_OE                         0x02//有数据溢出
#define UART_LSR_PE                         0x04//接收的数据奇偶校验错误
#define UART_LSR_FE                         0x08//帧错误表示位
#define UART_LSR_BI                         0x10//中断打断表示位
#define UART_LSR_TFE                        0x20//发送FIFO空
#define UART_LSR_TE                         0x40//没有数据发送
#define UART_LSR_ERR                        0x80//接收错误

/* IER */
#define UART_IER_IRE                        0x01//接收状态中断
#define UART_IER_ITE                        0x02//发送状态中断
#define UART_IER_ILE                        0x04//线路状态中断
#define UART_IER_IME                        0x08//MODEM状态中断

/* IIR */
#define UART_IIR_TXRDY                      0x02//发送FIFO空
#define UART_IIR_RXRDY                      0x04//接收数据量达到trigger
#define UART_IIR_RXL                        0x06//接收出错
#define UART_IIR_RXTIMEOUT                  0x0C//接收超时,FIFO有数据

/*----------------------- Type Declarations ---------------------------------*/


/*----------------------- Variable Declarations -----------------------------*/


/*----------------------- Function Prototype --------------------------------*/


/*----------------------- Function Implement --------------------------------*/
/******************************************************************************
* Name: 	 ls1c102_uart_get_base
*
* Desc: 	 获取UART的寄存器基址
* Param:     uart - UART号
* Return:    UART的寄存器基址
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/12/18, Create this function by zhuangwei
 ******************************************************************************/
static unsigned int ls1c102_uart_get_base(ls1c102_uart_t uart)
{
    unsigned int base = 0;

    switch (uart)
    {
    case LS1c102_UART0:
        base = UART0_BASEADDR;
        break;
    case LS1c102_UART1:
        base = UART1_BASEADDR;
        break;
    case LS1c102_UART2:
        base = UART2_BASEADDR;
        break;
    default:
        break;
    }

    return base;
}

/******************************************************************************
* Name: 	 ls1c102_uart_init
*
* Desc: 	 UART初始化
* Param:     uart_info - UART初始化参数
* Return:    0 - 成功
             -1 - 失败
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/12/18, Create this function by zhuangwei
 ******************************************************************************/
int ls1c102_uart_init(ls1c102_uart_info_t uart_info)
{
    unsigned int div;
    unsigned char win_size, simple_ctrl, i;
    unsigned int clock_bus = ls1c102_clock_get();
	unsigned int uart_base = ls1c102_uart_get_base(uart_info.uart);
	unsigned char LCR_value;
	ls1c102_uart_LCR_t *LCR_ptr = (ls1c102_uart_LCR_t *)&LCR_value;

	if(uart_info.uart == LS1c102_UART2)
    {
        if(uart_info.baud_rate > UART_BAUD_RATE_9600) return -1;
        clock_bus = 32768;
    }

    /* 根据波特率计算分频系数与采样窗口长度 */
    for(win_size=16; win_size>=3; win_size--)
    {
        div = clock_bus/uart_info.baud_rate/win_size;
        if(div <= 0xffff) break;
    }

    if(win_size < 3) return -1;
    simple_ctrl = (win_size-1)<<4;
    simple_ctrl += (win_size==16)?0:win_size;
    UART_SAMPLE_CTRL(uart_base) = simple_ctrl;

    /* 计算分频值 */
    UART_LCR(uart_base) = 0x80;
    div = (clock_bus<<8)/uart_info.baud_rate/win_size;
    UART_DL_D(uart_base) = div & 0xff;
    div >>= 8;
    UART_DL_L(uart_base) = div & 0xff;
    div >>= 8;
    UART_DL_H(uart_base) = div & 0xff;

    /* 配置串口奇偶校验、停止位、数据位 */
    LCR_ptr->dlab = 0;
    LCR_ptr->bcb = 0;
    if(uart_info.parity)
    {
        LCR_ptr->spd = 1;
        LCR_ptr->eps = uart_info.parity - 1;
        LCR_ptr->pe =  1;
    }
    else
    {
        LCR_ptr->spd = 0;
        LCR_ptr->eps = 0;
        LCR_ptr->pe =  0;
    }
    LCR_ptr->sb = uart_info.stop_bits;
    LCR_ptr->bec = uart_info.data_bits;
    UART_LCR(uart_base) = LCR_value;

    if(uart_info.int_type)
    {
        if(uart_info.int_type > UART_INT_RXTX) return -1;
        if(uart_info.trigger > UART_FCR_TRIGGER_MAX) return -1;
        UART_FCR(uart_base) = UART_FCR_RXRST|UART_FCR_TXRST|UART_FCR_TRIGGER(uart_info.trigger);
        while(UART_STATUS(uart_base) & 0x40); //查询复位结束
        UART_IER(uart_base) = uart_info.int_type;//开启接收中断
    }
    else
    {
        UART_FCR(uart_base) = 0x0E;//FIFO_TRIGGER_1，清接收发送FIFO
        while(UART_STATUS(uart_base) & 0x40); //查询复位结束
        UART_IER(uart_base) = 0;//默认关闭所有UART中断
    }


    return 0;
}

/******************************************************************************
* Name: 	 ls1c102_uart_putc
*
* Desc: 	 UART发送一个字节
* Param:     uart - UART号
             ch - 要发送的字节数据
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/12/18, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_uart_putc(ls1c102_uart_t uart, unsigned char ch)
{
    unsigned int uart_base = ls1c102_uart_get_base(uart);

    while (!(UART_LSR(uart_base) & UART_LSR_TFE));//发送FIFO空
    UART_TxData(uart_base) = ch;
}

/******************************************************************************
* Name: 	 ls1c102_uart_getc
*
* Desc: 	 UART发送一个字节
* Param:     uart - UART号
* Return:    >=0 - 接收到的数据
             -1 - 没有收到数据
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/12/18, Create this function by zhuangwei
 ******************************************************************************/
int ls1c102_uart_getc(ls1c102_uart_t uart)
{
    unsigned int uart_base = ls1c102_uart_get_base(uart);

    if(UART_LSR(uart_base) & UART_LSR_DR)//接收FIFO有数据
    {
        return UART_RxData(uart_base);
    }

    return -1;
}

/******************************************************************************
* Name: 	 ls1c102_uart_rx_ready
*
* Desc: 	 UART发送一个字节
* Param:     uart - UART号
* Return:    0 - 接收缓冲区没有数据
             1 - 接收缓冲区有数据
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/12/18, Create this function by zhuangwei
 ******************************************************************************/
int ls1c102_uart_rx_ready(ls1c102_uart_t uart)
{
    unsigned int uart_base = ls1c102_uart_get_base(uart);
    unsigned char iir = UART_IIR(uart_base);

    return ((iir & UART_IIR_RXRDY) || (iir & UART_IIR_RXTIMEOUT));
}
/*---------------------------------------------------------------------------*/
