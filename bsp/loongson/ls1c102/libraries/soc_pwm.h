/*
 * Copyright (c) 2024, yunxiao
 * All rights reserved.
 *
 * Filename: soc_pwm.h
 * Abstract:
 * pwm and servo.
 *
 * Current version: 1.1
 * Author: Lin Yihan
 * Completion date: 2024, 1, 2
 */

#ifndef _SOC_PWM_H
#define _SOC_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

////////
////////////////
////////////////////////
#define LS1C102_PWM_BASE          0xBFEC0030    // the basic address of PWM register

typedef struct
{
    volatile unsigned int div_freq;             // 0xc0 pwm_div_freq
    volatile unsigned int comp;                 // 0xc4 pwm_comp
    volatile unsigned int state;                // 0xc8 pwm_state
} HW_PWM_t;

#define g_pwm ((HW_PWM_t *) LS1C102_PWM_BASE)

////////////////////////
////////////////
////////

void pwm_init(void);
void pwm_set(int div_freq, int comp);
void pwm_state(int div_freq, int comp);

void pwm_steering_engine_init(void);
void pwm_steering_engine_set(int angle);

#ifdef __cplusplus
}
#endif

#endif // _SOC_PWM_H

