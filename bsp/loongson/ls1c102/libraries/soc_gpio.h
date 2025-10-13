/*
 * Copyright (c) 2024, yunxiao
 * All rights reserved.
 *
 * Filename: user_gpio.h
 * Abstract:
 * gpio
 *
 * Current version: 1.1
 * Author: Lin Yihan
 * Completion date: 2024, 1, 2
 */

#ifndef _SOC_GPIO_H
#define _SOC_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#define LA_GPIO_INPUT 0
#define LA_GPIO_OUTPUT 1

void gpio_init(int gpio, int io);
void gpio_write(int gpio, int val);
int gpio_read(int gpio);

#ifdef __cplusplus
}
#endif

#endif // _SOC_GPIO_H

