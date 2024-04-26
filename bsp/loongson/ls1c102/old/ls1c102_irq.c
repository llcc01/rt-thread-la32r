/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName: 	 interrupt.c
* Desc:
*
*
* Author: 	 zhuangwei
* Date: 	 2018/11/09
* Notes:
*
* -----------------------------------------------------------------
* Histroy: v1.0   2018/11/09, zhuangwei create this file
*
******************************************************************************/


/*------------------------------- Includes ----------------------------------*/
#include "ls1c102_irq.h"
#include "mipsregs.h"
#include <stdint.h>


/*------------------- Global Definitions and Declarations -------------------*/


/*---------------------- Constant / Macro Definitions -----------------------*/
struct isr_desc
{
    isr_handler_t handler;
    void          *param;
};
static struct isr_desc isr_handler_table[IRQ_MAX];


/*----------------------- Type Declarations ---------------------------------*/


/*----------------------- Variable Declarations -----------------------------*/


/*----------------------- Function Prototype --------------------------------*/


/*----------------------- Function Implement --------------------------------*/
/******************************************************************************
* Name: 	 ls1c102_irq_default_isr
*
* Desc: 	 默认的中断处理函数
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
static void ls1c102_irq_default_isr(int vector, void *param)
{

}
/******************************************************************************
* Name: 	 ls1c102_irq_pmu_evt_isr
*
* Desc: 	 PMU事件中断处理函数
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
static void ls1c102_irq_pmu_evt_isr(void)
{
    uint32_t reg_cmdsts=PMU_CmdSts;
    uint8_t pengding_pmu;
    uint8_t idx;

    pengding_pmu = ((uint8_t)(reg_cmdsts>>19)) & 0x1f;
    for(idx=0; idx<5; idx++)
    {
        if((pengding_pmu>>idx) & 0x1)
        {
            isr_handler_table[idx+IRQ_BAT_FAIL].handler(idx+IRQ_BAT_FAIL, isr_handler_table[idx+IRQ_BAT_FAIL].param);
            PMU_CommandW = (0x01<<(idx+19));//清中断
        }
    }
}

/******************************************************************************
* Name: 	 ls1c102_irq_intc_isr
*
* Desc: 	 中断控制器汇总的中断处理函数
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
static void ls1c102_irq_intc_isr(void)
{
    uint8_t reg_int_out = INT_OUT;
    uint8_t idx;

    for(idx=0; idx<8; idx++)
    {
        if((reg_int_out>>idx) & 0x1)
        {
            isr_handler_table[idx+IRQ_TIMER].handler(idx+IRQ_TIMER, isr_handler_table[idx+IRQ_TIMER].param);
            INT_CLR = (0x01<<idx);//清中断
        }
    }
}

/******************************************************************************
* Name: 	 ls1c102_irq_exti_isr
*
* Desc: 	 外部中断处理函数
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
static void ls1c102_irq_exti_isr(void)
{
    uint8_t reg_ExintSrc = PMU_ExintSrc;
    uint8_t idx;

    for(idx=0; idx<32; idx++)
    {
        if((reg_ExintSrc>>idx) & 0x1)
        {
            isr_handler_table[idx+IRQ_EXTI0].handler(idx+IRQ_EXTI0, isr_handler_table[idx+IRQ_EXTI0].param);
            PMU_ExintSrc = (0x01<<idx);//清中断
        }
    }
}

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
void ls1c102_irq_init(void)
{
    uint32_t status_val;
    uint32_t idx;

    /* 开启IP1-IP7中断 */
    status_val = read_c0_status();
    status_val |= 0xfe00;
    write_c0_status(status_val);

    for (idx = 0; idx < IRQ_MAX; idx++)
    {
        isr_handler_table[idx].handler = ls1c102_irq_default_isr;
    }
}

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
void ls1c102_irq_enable(void)
{
    uint32_t status_val;

    status_val = read_c0_status();
    status_val |= (uint32_t)(0x1);
    write_c0_status(status_val);
}

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
void ls1c102_irq_disable(void)
{
    uint32_t status_val;

    status_val = read_c0_status();
    status_val &= ~((uint32_t)(0x1));
    write_c0_status(status_val);
}

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
void ls1c102_irq_mask(int vector)
{
    if(vector <= IRQ_OS_TIMER)
    {
        uint32_t status_val;

        status_val = read_c0_status();
        status_val &= ~(((uint32_t)1)<<(8+vector));
        write_c0_status(status_val);
    }
    else if(vector <= IRQ_ADC)//见PMU_CmdSts(15:8)
    {
        PMU_CmdSts &= ~(((uint32_t)1)<<(vector+6));
    }
    else if(vector <= IRQ_DMA)//见INT_EN
    {
        INT_EN &= ~(((uint8_t)1)<<(vector-IRQ_TIMER));
    }
    else if(vector <= IRQ_EXTI31)//见PMU_ExintEn
    {
        PMU_ExintEn &= ~(((uint32_t)0x01)<<(vector-IRQ_EXTI0));
    }
}

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
void ls1c102_irq_unmask(int vector)
{
    if(vector <= IRQ_OS_TIMER)//IP0-IP1
    {
        uint32_t status_val;

        status_val = read_c0_status();
        status_val |= ((uint32_t)1)<<(8+vector);
        write_c0_status(status_val);
    }
    else if(vector <= IRQ_ADC)//见PMU_CmdSts(15:8)
    {
        PMU_CmdSts |= ((uint32_t)1)<<(vector+6);
    }
    else if(vector <= IRQ_DMA)//见INT_EN
    {
        INT_EN |= ((uint8_t)1)<<(vector-IRQ_TIMER);
    }
    else if(vector <= IRQ_EXTI31)//外部中断,见PMU_CmdSts与PMU_ExintEn
    {
        PMU_CmdSts |= ((uint32_t)1)<<25;//ExtIntEn
        PMU_ExintEn |= (((uint32_t)0x01)<<(vector-IRQ_EXTI0));
    }
}

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
void ls1c102_irq_install(int vector, isr_handler_t isr_handler, void *param)
{
    if(vector < IRQ_MAX)
	{
		isr_handler_table[vector].handler = isr_handler;
		isr_handler_table[vector].param = param;
	}
}

/******************************************************************************
* Name: 	 ls1c102_irq_dispatch
*
* Desc: 	 CPU中断源IP0-IP7处理函数
* Param:     无
* Return:    无
* Global:
* Note:      见LS1c102用户手册与CP0寄存器说明
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/11/11, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_irq_dispatch(void)
{
    uint32_t cause_im, status_im, pending_im;
    uint8_t idx;

    /*见CP0 STATUS和CAUSE寄存器说明*/
    cause_im = read_c0_cause() & ST0_IM;
    status_im = read_c0_status() & ST0_IM;
    pending_im = cause_im & status_im;

	if (pending_im & CAUSEF_IP0)//软件中断
    {
        isr_handler_table[IRQ_SOFT_INT].handler(IRQ_SOFT_INT, isr_handler_table[IRQ_SOFT_INT].param);
    }
	else if (pending_im & CAUSEF_IP1)//CPU定时器中断
    {
        isr_handler_table[IRQ_OS_TIMER].handler(IRQ_OS_TIMER, isr_handler_table[IRQ_OS_TIMER].param);
    }
    else if (pending_im & CAUSEF_IP2)//PMU唤醒中断
    {
        if(PMU_CmdSts & ((uint32_t)1)<<16)
		{
			isr_handler_table[IRQ_MPU_WAKEUP].handler(IRQ_MPU_WAKEUP, isr_handler_table[IRQ_MPU_WAKEUP].param);
			PMU_CommandW = (0x01<<16);//清中断
		}
    }
    else if (pending_im & CAUSEF_IP3)//触摸按键中断
    {
		if(PMU_CmdSts & ((uint32_t)1)<<17)
		{
			isr_handler_table[IRQ_TOUCH_KEY].handler(IRQ_TOUCH_KEY, isr_handler_table[IRQ_TOUCH_KEY].param);
			PMU_CommandW = (0x01<<17);//清中断
		}
	}
    else if (pending_im & CAUSEF_IP4)//串口2中断
    {
        if(PMU_CmdSts & ((uint32_t)1)<<18)
		{
			isr_handler_table[IRQ_UART2].handler(IRQ_UART2, isr_handler_table[IRQ_UART2].param);
			PMU_CommandW = (0x01<<18);//清中断
		}
    }
    else if (pending_im & CAUSEF_IP5)//PMU事件中断
    {
        ls1c102_irq_pmu_evt_isr();
    }
    else if (pending_im & CAUSEF_IP6)//中断控制器中断
    {
        ls1c102_irq_intc_isr();
    }
    else if (pending_im & CAUSEF_IP7)//外部中断
    {
        ls1c102_irq_exti_isr();
        PMU_CommandW = (0x01<<24);//清中断
    }
}
/*---------------------------------------------------------------------------*/

