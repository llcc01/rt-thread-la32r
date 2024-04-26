/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName: 	 ls1c102_irq.h
* Desc:
*
*
* Author: 	 zhuangwei
* Date: 	 2018-11-09
* Notes:
*
* -----------------------------------------------------------------
* Histroy: v1.0   2018-11-09, zhuangwei create this file
*
******************************************************************************/
#ifndef _LS1c102_IRQ_H_
#define _LS1c102_IRQ_H_


/*------------------------------- Includes ----------------------------------*/
#include "ls1c102.h"

/*----------------------------- Global Typedefs ------------------------------*/
enum
{
    IRQ_IP0 = 0,
    IRQ_IP1,
    IRQ_IP2,
    IRQ_IP3,
    IRQ_IP4,
    IRQ_IP5,
    IRQ_IP6,
    IRQ_IP7,
};

typedef enum
{
    IRQ_SOFT_INT = 0,
    IRQ_OS_TIMER,
    IRQ_MPU_WAKEUP,
    IRQ_TOUCH_KEY,
    IRQ_UART2,
    IRQ_BAT_FAIL,
    IRQ_C32K_FIAL,
    IRQ_C8M_FIAL,
    IRQ_RTC,
    IRQ_ADC,
    IRQ_TIMER,
    IRQ_I2C,
    IRQ_UART1,
    IRQ_UART0,
    IRQ_FLASH,
    IRQ_SPI,
    IRQ_VPWM,
    IRQ_DMA,
    IRQ_EXTI0,
    IRQ_EXTI1,
    IRQ_EXTI2,
    IRQ_EXTI3,
    IRQ_EXTI4,
    IRQ_EXTI5,
    IRQ_EXTI6,
    IRQ_EXTI7,
    IRQ_EXTI8,
    IRQ_EXTI9,
    IRQ_EXTI10,
    IRQ_EXTI11,
    IRQ_EXTI12,
    IRQ_EXTI13,
    IRQ_EXTI14,
    IRQ_EXTI15,
    IRQ_EXTI16,
    IRQ_EXTI17,
    IRQ_EXTI18,
    IRQ_EXTI19,
    IRQ_EXTI20,
    IRQ_EXTI21,
    IRQ_EXTI22,
    IRQ_EXTI23,
    IRQ_EXTI24,
    IRQ_EXTI25,
    IRQ_EXTI26,
    IRQ_EXTI27,
    IRQ_EXTI28,
    IRQ_EXTI29,
    IRQ_EXTI30,
    IRQ_EXTI31,
    IRQ_MAX,
}irq_num_t;

typedef void (*isr_handler_t)(int vector, void *param);

/*----------------------------- Global Defines -----------------------------*/


/*----------------------------- External Variables --------------------------*/


/*------------------------ Global Function Prototypes -----------------------*/
/******************************************************************************
* Name: 	 ls1c102_irq_init
*
* Desc: 	 中断初始化
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_irq_init(void);

/******************************************************************************
* Name: 	 ls1c102_irq_enable
*
* Desc: 	 开启CPU全局中断
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_irq_enable(void);

/******************************************************************************
* Name: 	 ls1c102_irq_disable
*
* Desc: 	 关闭CPU全局中断
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_irq_disable(void);

/******************************************************************************
* Name: 	 ls1c102_irq_mask
*
* Desc: 	 禁用中断
* Param:     vector - 中断号,见irq_num_t类型
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_irq_mask(int vector);

/******************************************************************************
* Name: 	 ls1c102_irq_unmask
*
* Desc: 	 使能中断
* Param:     vector - 中断号,见irq_num_t类型
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_irq_unmask(int vector);

/******************************************************************************
* Name: 	 ls1c102_irq_install
*
* Desc: 	 为中断源设置中断处理函数
* Param:     vector - 中断号,见irq_num_t类型
             isr_handler - 中断处理函数
			 param - 参数
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_irq_install(int vector, isr_handler_t isr_handler, void *param);

/******************************************************************************
* Name: 	 ls1c102_irq_dispatch
*
* Desc: 	 CPU中断源IP0-IP7处理函数
* Param:     无
* Return:    无
* Global:
* Note:      见LS1c102用户手册与CP0寄存器说明，在startup_gcc.S中调用
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_irq_dispatch(void);


#endif // _LS1c102_IRQ_H_
