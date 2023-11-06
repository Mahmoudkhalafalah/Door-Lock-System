/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR Timer driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/
#include <avr/io.h>
#include "timer.h"
#include "../common_macros.h"
#include <avr/interrupt.h>
/********************************************* Global functions **********************************************/
static volatile void (*timer1CallBack)(void);	//Call Back Function for Timer 1

uint16 G_initialValue; /*variable to save the initial value of TCNT1 Register to initialize it with each interrupt*/
/*
 * Description:
 * Function to initialize Timer1:
 * takes configurations :
 * 1. initial value of the timer
 * 2. compare value for compare mode
 * 3. prescaler
 * 4. mode of the timer
 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr) {
	/*
	 * Putting the initial value in TCNT1 Register
	 * Save the initial value in the global variable to initialize the register with each interrupt
	 */
	TCNT1 = Config_Ptr->initial_value;
	G_initialValue = Config_Ptr->initial_value;
	/*
	 * Sets FOC1A/FOC1B For Non-PWM Mode
	 */
	TCCR1A = (1 << FOC1A) | (1 << FOC1B);
	/*
	 * Enable Timer interrupt enable based on the chosen mode in register TIMSK
	 * select the requested mode pins "WGM10 ,WGM11, WGM12"
	 * normal Mode 000
	 * compare mode 100
	 * for compare mode put the compare value in OCR1A register
	 */
	switch (Config_Ptr->mode) {
	case NORMAL_MODE:
		TIMSK = 1 << TOIE1;
		break;
	case CTC_MODE:
		TIMSK = (1 << OCIE1A);
		TCCR1B |= (1 << WGM12);
		OCR1A = Config_Ptr->compare_value;
		break;
	}
	/*
	 * Selecting prescaler in register TCCR1B pins "CS10, CS11, CS12"
	 * No Clock 000
	 * F CPU / 1 -> 001
	 * F CPU / 8 -> 010
	 * F CPU / 64 -> 011
	 * F CPU / 256 -> 100
	 * F CPU / 1024 -> 101
	 */
	TCCR1B |= (Config_Ptr->prescaler & 0x07);
}
/*
 * Description:
 * Function to disable timer 1
 */
void Timer1_deInit(void) {
	//Clear Prescaler
	TCCR1B = TCCR1B & 0XF8;
	//Close Timer 1 Overflow Interrupt
	CLEAR_BIT(TIMSK, TOIE1);
	//Close Timer 1 Compare Interrupt For Channel A
	CLEAR_BIT(TIMSK, OCIE1A);
	//Close Timer 1 Compare Interrupt For Channel B
	CLEAR_BIT(TIMSK, OCIE1B);
}
/*
 * Description:
 * Function to set the Call Back function address
 */
void Timer1_setCallBack(void (*a_ptr)(void)) {
	timer1CallBack = a_ptr;
}

/*
 * ISR for Timer 1 Normal Mode
 */
ISR(TIMER1_OVF_vect) {
	TCNT1 = G_initialValue; /*initialize TCNT1 with the initial value*/
	if (timer1CallBack != NULL_PTR) {
		timer1CallBack();
	}
}
/*
 * ISR for Timer 1 compare Mode
 */
ISR(TIMER1_COMPA_vect) {
	TCNT1 = G_initialValue; /*initialize TCNT1 with the initial value*/
	if (timer1CallBack != NULL_PTR) {
		timer1CallBack();
	}
}
