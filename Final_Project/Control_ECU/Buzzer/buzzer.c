/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the Buzzer driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/
#include "buzzer.h"
/*
 * Description:
 * function to initialize buzzer by :
 * Setting the pin to output
 * initially turning off the buzzer
 */
void Buzzer_init() {
	/*Defining Buzzer pin as output*/
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	/*Initially turning off the buzzer*/
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
/*
 * Description:
 * function to turn the buzzer on
 */
void Buzzer_on() {
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
/*
 * Description:
 * function to turn the buzzer off
 */
void Buzzer_off() {
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

