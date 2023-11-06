/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for Buzzer driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#ifndef BUZZER_BUZZER_H_
#define BUZZER_BUZZER_H_

#include "../GPIO/gpio.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT_ID PORTA_ID
#define BUZZER_PIN_ID PIN0_ID
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * function to initialize buzzer by :
 * Setting the pin to output
 * initially turning off the buzzer
 */
void Buzzer_init();
/*
 * Description:
 * function to turn the buzzer on
 */
void Buzzer_on();
/*
 * Description:
 * function to turn the buzzer off
 */
void Buzzer_off();

#endif /* BUZZER_BUZZER_H_ */
