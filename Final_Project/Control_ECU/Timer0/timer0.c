/******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer0.c
 *
 * Description: Source file for the AVR Timer0 driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/
#include "timer0.h"

#include "avr/io.h"
/*
 * Description:
 * Generate a PWM signal with frequency 500Hz
 * Timer0 will be used with pre-scaler F_CPU/64
 * F_PWM=(F_CPU)/(256*N) = (8*10^6)/(256*64) = 490Hz
 * Duty Cycle can be changed by updating the value
 * in The Compare Register
 */
void PWM_Timer0_Start(uint8 duty_cycle) {

	TCNT0 = 0; //Set Timer Initial value

	OCR0 = (duty_cycle / 100.0) * 255; // Set Compare Value

	DDRB |= (1 << PB3); //set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/64 CS00=1 CS01=1 CS02=0
	 */
	TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS01)
			| (1 << CS00);
}

