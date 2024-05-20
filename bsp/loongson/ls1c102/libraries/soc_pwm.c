/*
 * Copyright (c) 2024, yunxiao
 * All rights reserved.
 *
 * Filename: pwm.c
 * Abstract:
 * pwm and servo.
 *
 * Current version£º1.1
 * Author: Lin Yihan
 * Completion date: 2024, 1, 2
 */

#include "soc_pwm.h"
#include "soc_ls1c102.h"

void pwm_init(void) {
    g_pwm->state = 0;
    g_pwm->div_freq = 500;
    g_pwm->comp = 250;
    g_pwm->state = 1;
}

/*
 * Function: set frequency and comparison value
 * Input: div_freq, comp
 *     div_freq: real frequency, range from 0 to 8M 
 *     comp: range from 0 to div_freq
 * Return: None
 */
void pwm_set(int div_freq, int comp) {
    g_pwm->div_freq = div_freq;
    g_pwm->comp = comp;
}

void pwm_steering_engine_init(void) {
    g_pwm->state = 0;
    g_pwm->div_freq = 160000;
    /*
		g_pwm->comp =  4000;// duty = 1/40 0 degree,
		g_pwm->comp =  8000;// duty = 2/40 45 degree,
		g_pwm->comp = 12000;// duty = 3/40 90 degree,
		g_pwm->comp = 16000;// duty = 4/40 145 degree,
		g_pwm->comp = 20000;// duty = 5/40 180 degree, actually, it turns back to 0 degrees.
    */
    g_pwm->comp = 4000;
    g_pwm->state = 1;
}

/*
 * Function: set the angle of servo
 * Input: angle
 *     angle: angle is a multiple of 9 degree
 * Return: None
 */
void pwm_steering_engine_set(int angle) {
    g_pwm->comp = 4000 + 800 * angle / 9;
}
    

