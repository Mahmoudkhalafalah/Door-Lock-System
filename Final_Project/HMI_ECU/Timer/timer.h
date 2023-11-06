/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR timer driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

#include "../std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum {
	NO_CLOCK, F_CPU_1, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
} Timer1_Prescaler;

typedef enum {
	NORMAL_MODE, CTC_MODE
} Timer1_Mode;

typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * Function to initialize Timer1:
 * takes configurations :
 * 1. initial value of the timer
 * 2. compare value for compare mode
 * 3. prescaler
 * 4. mode of the timer
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
/*
 * Description:
 * Function to disable timer 1
 */
void Timer1_deInit(void);
/*
 * Description:
 * Function to set the ISR for the timer
 */
void Timer1_setCallBack(void(*a_ptr)(void));
#endif /* TIMER_TIMER_H_ */
