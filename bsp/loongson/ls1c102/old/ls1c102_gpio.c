/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName: 	 ls1c102_gpio.c
* Desc:
*
*
* Author: 	 zhuangwei
* Date: 	 2018/09/23
* Notes:     外部中断只有32个，具体对应关系如下:
             GPIO0-GPIO7对应外部中断0-7
             GPIO16-GPIO23对应外部中断8-15
             GPIO32-GPIO39对应外部中断16-23
             GPIO48-GPIO55对应外部中断24-31
*
* -----------------------------------------------------------------
* Histroy: v1.0   2018/09/23, zhuangwei create this file
*
******************************************************************************/


/*------------------------------- Includes ----------------------------------*/
#include "ls1c102_gpio.h"

/*------------------- Global Definitions and Declarations -------------------*/


/*---------------------- Constant / Macro Definitions -----------------------*/


/*----------------------- Type Declarations ---------------------------------*/


/*----------------------- Variable Declarations -----------------------------*/


/*----------------------- Function Prototype --------------------------------*/


/*----------------------- Function Implement --------------------------------*/
/******************************************************************************
* Name: 	 ls1c102_gpio_write
*
* Desc: 	 GPIO输出指定值
* Param:     pin - GPIO号,见gpio_pin_t定义
             value - GPIO要输出的值
* Return:    无
* Global:
* Note:      mode值大于0则当作1
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/09/23, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_gpio_write(int pin, unsigned int value)
{
    if(pin >= GPIO_PIN_MAX) return;

	if(GPIO_HIGH == value)
	{
		PMU_GPIOBit(pin) = 0x03;
	}
	else
	{
		PMU_GPIOBit(pin) = 0x02;
	}
}

/******************************************************************************
* Name: 	 ls1c102_gpio_read
*
* Desc: 	 读取GPIO输入
* Param:     pin - GPIO号,见gpio_pin_t定义
* Return:    读取到的值(0或1),出错返回-1。
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/09/23, Create this function by zhuangwei
 ******************************************************************************/
int ls1c102_gpio_read(gpio_pin_t pin)
{
    if(pin >= GPIO_PIN_MAX) return -1;

    return PMU_GPIOBit(pin);
}

/******************************************************************************
* Name: 	 ls1c102_gpio_set_irq_mode
*
* Desc: 	 设置GPIO外部中断模式
* Param:     pin - GPIO号,见gpio_pin_t定义
             mode - 0:电平模式
                    1:边沿模式
* Return:
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/09/23, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_gpio_set_irq_mode(gpio_pin_t pin, unsigned int mode)
{
    unsigned int exti_num;

    if(pin >= GPIO_PIN_MAX) return;
    if(((pin/8)%2) != 0) return;//这些GPIO没有外部中断

    exti_num = pin%16 + pin/16*8;
    if(GPIO_IRQ_MODE_LEVEL == mode)
	{
		PMU_ExintEdge &= ~(((unsigned int)0x01)<<exti_num);
	}
	else
	{
		PMU_ExintEdge |= (((unsigned int)0x01)<<exti_num);
	}
}

/******************************************************************************
* Name: 	 ls1c102_gpio_set_irq_pol
*
* Desc: 	 设置GPIO外部中断极性
* Param:     pin - GPIO号,见gpio_pin_t定义
             pol - 0:低电平/下降沿有效
                   1:高电平/上升沿有效
* Return:
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/09/23, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_gpio_set_irq_pol(gpio_pin_t pin, unsigned int pol)
{
    unsigned int exti_num;

    if(pin >= GPIO_PIN_MAX) return;
    if(((pin/8)%2) != 0) return;//这些GPIO没有外部中断
    exti_num = pin%16 + pin/16*8;

	if(GPIO_IRQ_POL_HIGH == pol)
	{
		PMU_ExintPol &= ~(((unsigned int)0x01)<<exti_num);
	}
	else
	{
		PMU_ExintPol |= (((unsigned int)0x01)<<exti_num);
	}
}

/******************************************************************************
* Name: 	 ls1c102_gpio_remap
*
* Desc: 	 GPIO引脚复用
* Param:     pin - GPIO号,见gpio_pin_t定义
             func - 引脚复用功能选择,见gpio_iosel_t定义
* Return:
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/09/23, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_gpio_remap(gpio_pin_t pin, gpio_func_t func)
{
	unsigned int gpio_group = pin/16;
	unsigned int gpio_offset = (pin%16)*2;

	if(pin >= GPIO_PIN_MAX) return;
	if(func > GPIO_FUNC_MAX) return;

	switch(gpio_group)
	{
		case 0:
			PMU_IOSEL0 &= ~(0x03<<gpio_offset);
			PMU_IOSEL0 |= (func<<gpio_offset);
			break;
		case 1:
			PMU_IOSEL1 &= ~(0x03<<gpio_offset);
			PMU_IOSEL1 |= (func<<gpio_offset);
			break;
		case 2:
			PMU_IOSEL2 &= ~(0x03<<gpio_offset);
			PMU_IOSEL2 |= (func<<gpio_offset);
			break;
		case 3:
			PMU_IOSEL3 &= ~(0x03<<gpio_offset);
			PMU_IOSEL3 |= (func<<gpio_offset);
			break;
		default:
			break;

	}
}
/*---------------------------------------------------------------------------*/
