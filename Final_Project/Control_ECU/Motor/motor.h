/******************************************************************************
 *
 * Module: Motor
 *
 * File Name: motor.h
 *
 * Description: Header file for the Motor driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include "../std_types.h"
#include "../common_macros.h"
#include "../GPIO/gpio.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MOTOR_PIN_1_ID PIN0_ID
#define MOTOR_PIN_2_ID PIN1_ID
#define MOTOR_PORT_ID PORTB_ID

#define MOTOR_QUARTER_SPEED 25
#define MOTOR_HALF_SPEED 50
#define MOTOR_THREE_QUARTERS_SPEED 75
#define MOTOR_FULL_SPEED 100


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum {
	STOP , CLOCK_WISE , ANTI_CLOCK_WISE
}DCMotorState;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * initializing motor pins to be output
 * stop the motor at the beginning
 */
void DCMotor_init(void);
/*
 * Description:
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the
 * required speed value.
 */
void DCMotor_rotate(DCMotorState state ,uint8 speed);
#endif /* MOTOR_MOTOR_H_ */
