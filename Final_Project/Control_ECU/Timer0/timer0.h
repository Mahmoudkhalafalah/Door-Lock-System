/******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer0.c
 *
 * Description: Source file for the AVR timer0 driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/


#ifndef TIMER0_TIMER0_H_
#define TIMER0_TIMER0_H_

#include "../std_types.h"
/*
 * Description:
 * Generate a PWM signal with frequency 500Hz
 * Timer0 will be used with pre-scaler F_CPU/8
 * F_PWM=(F_CPU)/(256*N) = (10^6)/(256*8) = 500Hz
 * Duty Cycle can be changed by updating the value
 * in The Compare Register
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* TIMER0_TIMER0_H_ */
