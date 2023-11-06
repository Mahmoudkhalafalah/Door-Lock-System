/******************************************************************************
 *
 * Module: Motor
 *
 * File Name: motor.c
 *
 * Description: Source file for the Motor driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#include "../GPIO/gpio.h"
#include "motor.h"
#include "../Timer0/timer0.h"

/*
 * Description:
 * initializing motor pins to be output
 * stop the motor at the beginning
 */
void DCMotor_init(void) {
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN_1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN_2_ID, PIN_OUTPUT);

	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_1_ID, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_2_ID, LOGIC_LOW);
}
/*
 * Description:
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the
 * required speed value.
 */
void DCMotor_rotate(DCMotorState state, uint8 speed) {
	switch (state) {
	case STOP:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_1_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_2_ID, LOGIC_LOW);
		break;
	case CLOCK_WISE:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_1_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_2_ID, LOGIC_LOW);
		PWM_Timer0_Start(speed);
		break;
	case ANTI_CLOCK_WISE:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_1_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_2_ID, LOGIC_HIGH);
		PWM_Timer0_Start(speed);
	}

}
