/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName: 	 ls1c102_uart.h
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
#ifndef _LS1c102_UART_H_
#define _LS1c102_UART_H_

/*------------------------------- Includes ----------------------------------*/


/*----------------------------- Global Typedefs ------------------------------*/
typedef enum
{
    LS1c102_UART0 = 0,
    LS1c102_UART1,
    LS1c102_UART2
}ls1c102_uart_t;

#define UART_BAUD_RATE_2400                 2400
#define UART_BAUD_RATE_4800                 4800
#define UART_BAUD_RATE_9600                 9600
#define UART_BAUD_RATE_19200                19200
#define UART_BAUD_RATE_38400                38400
#define UART_BAUD_RATE_57600                57600
#define UART_BAUD_RATE_115200               115200
#define UART_BAUD_RATE_230400               230400
#define UART_BAUD_RATE_460800               460800
#define UART_BAUD_RATE_921600               921600

/* 数据位 */
#define UART_DATA_BITS_5                    0
#define UART_DATA_BITS_6                    1
#define UART_DATA_BITS_7                    2
#define UART_DATA_BITS_8                    3

/* 停止位 */
#define UART_STOP_BITS_1                    0
#define UART_STOP_BITS_2                    1//数据位为5时为1.5位

/* 奇偶校验位 */
#define UART_PARITY_NONE                    0
#define UART_PARITY_ODD                     1
#define UART_PARITY_EVEN                    2

/* 中断类型 */
#define UART_INT_NONE                       0
#define UART_INT_RX                         1
#define UART_INT_TX                         2
#define UART_INT_RXTX                       3

typedef struct
{
    ls1c102_uart_t uart;
    unsigned int baud_rate;//波特率
    unsigned char data_bits;//数据位
    unsigned char stop_bits;//停止位
    unsigned char parity;//奇偶校验位
    unsigned char int_type;//中断使能
    unsigned char trigger;//触发接收中断的字节数
}ls1c102_uart_info_t;


/*----------------------------- Global Defines -----------------------------*/


/*----------------------------- External Variables --------------------------*/


/*------------------------ Global Function Prototypes -----------------------*/
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
int ls1c102_uart_init(ls1c102_uart_info_t uart_info);

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
void ls1c102_uart_putc(ls1c102_uart_t uart, unsigned char ch);

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
int ls1c102_uart_getc(ls1c102_uart_t uart);

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
int ls1c102_uart_rx_ready(ls1c102_uart_t uart);

#endif // _LS1c102_UART_H_
