/*
 * Copyright (c) 2024, yunxiao
 * All rights reserved.
 *
 * Filename: soc_gpio.c
 * Abstract:
 * gpio
 *
 * Current version: 1.1
 * Author: Lin Yihan
 * Completion date: 2024, 1, 2
 */

#include "soc_gpio.h"
#include "soc_ls1c102.h"

/*
 * Function: enable gpio output
 * Input: gpio, io
 *     gpio: gpio number, range from 0 to 63
 * 	   io = 1: output mode, io = 0: input mode
 * Return: None
 */
void gpio_init(int gpio, int io) {
    int bit;
    if (gpio < 32) {
        bit = gpio;
        if(io == 0) g_pmu->GPIOA_OE &= ~(0x01<<bit);
        else g_pmu->GPIOA_OE |= (0x01<<bit);
    } else {
        bit = gpio - 32;
        if(io == 0) g_pmu->GPIOB_OE &= ~(0x01<<bit);
        else g_pmu->GPIOB_OE |= (0x01<<bit);
    }
}

/*
 * Function: set the output value of gpio
 * Input: gpio, val
 *     gpio: gpio number
 * 	   val = 1: output high, io = 0: output low
 * Return: None
 */
void gpio_write(int gpio, int val) {
    int bit;
    if (gpio < 32) {
        bit = gpio;
        if(val == 0) g_pmu->GPIOA_O &= ~(0x01<<bit);
        else g_pmu->GPIOA_O |= (0x01<<bit);
    } else {
        bit = gpio - 32;
        if(val == 0) g_pmu->GPIOB_O &= ~(0x01<<bit);
        else g_pmu->GPIOB_O |= (0x01<<bit);
    }
}

/*
 * Function: read the input value of gpio
 * Input: gpio
 *     gpio: gpio number
 * Return: None
 */
int gpio_read(int gpio) {
    int bit;
    if (gpio < 32) {
        bit = gpio;
        if((g_pmu->GPIOA_I & (0x01<<bit)) == (0x01<<bit))
            return 1;
        else
            return 0;
    } else {
        bit = gpio - 32;
        if((g_pmu->GPIOB_I & (0x01<<bit)) == (0x01<<bit))
            return 1;
        else
            return 0;
    }
}



