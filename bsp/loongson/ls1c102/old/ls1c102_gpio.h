/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName:      ls1c102_gpio.h
* Desc:
*
*
* Author:    zhuangwei
* Date:      2018/09/23
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
#ifndef _LS1c102_GPIO_H_
#define _LS1c102_GPIO_H_


/*------------------------------- Includes ----------------------------------*/
#include "ls1c102.h"


/*----------------------------- Global Typedefs -----------------------------*/
typedef enum
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
    GPIO_PIN_16,
    GPIO_PIN_17,
    GPIO_PIN_18,
    GPIO_PIN_19,
    GPIO_PIN_20,
    GPIO_PIN_21,
    GPIO_PIN_22,
    GPIO_PIN_23,
    GPIO_PIN_24,
    GPIO_PIN_25,
    GPIO_PIN_26,
    GPIO_PIN_27,
    GPIO_PIN_28,
    GPIO_PIN_29,
    GPIO_PIN_30,
    GPIO_PIN_31,
    GPIO_PIN_32,
    GPIO_PIN_33,
    GPIO_PIN_34,
    GPIO_PIN_35,
    GPIO_PIN_36,
    GPIO_PIN_37,
    GPIO_PIN_38,
    GPIO_PIN_39,
    GPIO_PIN_40,
    GPIO_PIN_41,
    GPIO_PIN_42,
    GPIO_PIN_43,
    GPIO_PIN_44,
    GPIO_PIN_45,
    GPIO_PIN_46,
    GPIO_PIN_47,
    GPIO_PIN_48,
    GPIO_PIN_49,
    GPIO_PIN_50,
    GPIO_PIN_51,
    GPIO_PIN_52,
    GPIO_PIN_53,
    GPIO_PIN_54,
    GPIO_PIN_55,
    GPIO_PIN_56,
    GPIO_PIN_57,
    GPIO_PIN_58,
    GPIO_PIN_59,
    GPIO_PIN_60,
    GPIO_PIN_61,
    GPIO_PIN_62,
    GPIO_PIN_63,
    GPIO_PIN_MAX,
}gpio_pin_t;

typedef enum
{
    GPIO_MODE_OUTPUT = 0,
    GPIO_MODE_INPUT,
}gpio_mode_t;

typedef enum
{
    GPIO_FUNC_GPIO = 0,
    GPIO_FUNC_MAIN,
    GPIO_FUNC_FIRST,
    GPIO_FUNC_SECOND,
    GPIO_FUNC_MAX,
}gpio_func_t;

/*----------------------------- Global Defines ------------------------------*/
#define GPIO_LOW				0x00
#define GPIO_HIGH				0x01

#define GPIO_IRQ_POL_HIGH       0x0
#define GPIO_IRQ_POL_LOW        0x1

#define GPIO_IRQ_MODE_LEVEL     0x0
#define GPIO_IRQ_MODE_EDGE      0x1


/*----------------------------- External Variables --------------------------*/




/*------------------------ Global Function Prototypes -----------------------*/
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
void ls1c102_gpio_write(int pin, unsigned int value);

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
int ls1c102_gpio_read(gpio_pin_t pin);

/******************************************************************************
* Name: 	 gpio_set_irq_mode
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
* Log: 	 2018/09/23, Create this function bgpio_func_ty zhuangwei
 ******************************************************************************/
void ls1c102_gpio_set_irq_mode(gpio_pin_t pin, unsigned int mode);

/******************************************************************************
* Name: 	 gpio_set_irq_pol
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
void ls1c102_gpio_set_irq_pol(gpio_pin_t pin, unsigned int pol);

/******************************************************************************
* Name: 	 gpio_remap
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
void ls1c102_gpio_remap(gpio_pin_t pin, gpio_func_t func);

#endif //_LS1c102_GPIO_H_
