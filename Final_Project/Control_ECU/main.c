/******************************************************************************
 *
 * Application
 *
 * File Name: main.c
 *
 * Description: Application of Control ECU
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#include "UART/uart.h"
#include "Timer/timer.h"
#include "TWI/twi.h"
#include "EEPROM/eeprom.h"
#include <avr/io.h>
#include <util/delay.h>
#include "Buzzer/buzzer.h"
#include "Motor/motor.h"

/*******************************************************************************
 *                      		definitions			                           *
 *******************************************************************************/

#define PASSSTARTADDRESS 0x0311 /* The start address of the password in the external memory*/
/*
 * The address in the external memory that stores a flag
 * whether it the the first time for the user to use the program or not
 * if it is not the first time the address will hold a value of 'F'
 */
#define FIRSTTIMEFLAGADDRESS 0x0320

/*******************************************************************************
 *                      		Global variables			                    *
 *******************************************************************************/
/*
 * different timer configurations for different periods
 * 1. 7.5 seconds
 * 2. 3 seconds
 */

Timer1_ConfigType timerConfig_7_5_Seconds = { 0, 58590, F_CPU_1024, CTC_MODE };
Timer1_ConfigType timerConfig_3_Seconds = { 0, 23436, F_CPU_1024, CTC_MODE };

uint8 g_tick = 0;/* a variable for number of interrupts for the timer */

/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
/*
 * Description:
 * function for timer for closing the buzzer after one minute
 * will be set as call back function when the user enters a wrong password 3 times
 */
void wrongPassword() {
	g_tick++;
	if (g_tick == 6) {
		Buzzer_off();
		UART_sendData('D');
		Timer1_deInit();
		g_tick = 0;
	}
}
/*
 * Description:
 * function for timer for handling the logic after the user Enters the right password
 * will be set as call back function when the user enters the right password and wants to open the door
 */
void openDoor() {
	g_tick++;
	/*
	 * after 15 seconds:
	 * Stop the motor and send 'U' for the HMI ECU to inform it that the door has been opened and will hold
	 * and then set the timer to make an interrupt after 3 seconds
	 */
	if (g_tick == 2) {
		DCMotor_rotate(STOP, 0);
		UART_sendData('U');
		Timer1_init(&timerConfig_3_Seconds);
		/*
		 * after 3 more seconds:
		 * rotate the motor anti-clock wise and send 'H' for the HMI ECU to inform it that the door is closing
		 * and then set the timer to make an interrupt after 7.5 seconds
		 */
	} else if (g_tick == 3) {
		DCMotor_rotate(ANTI_CLOCK_WISE, MOTOR_FULL_SPEED);
		UART_sendData('H');
		Timer1_init(&timerConfig_7_5_Seconds);
		/*
		 * after 15 more seconds:
		 * stop the motor and send 'L' for the HMI ECU to inform it that the door has been closed
		 * and then stop the timer and reset the ticks variable
		 */
	} else if (g_tick == 5) {
		DCMotor_rotate(STOP, 0);
		UART_sendData('L');
		Timer1_deInit();
		g_tick = 0;
	}
}
/*
 * Description:
 * functions that checks the equality of two strings
 */
uint8 arePasswordsEqual(uint8 *firstPassword, uint8 *secondPassword) {
	uint8 i = 0;
	while (firstPassword[i] != '\0') {
		if (firstPassword[i] != secondPassword[i])
			return FALSE;
		i++;
	}
	return TRUE;
}
/*******************************************************************************
 *                      		 Main      			                           *
 *******************************************************************************/
int main(void) {
	/*
	 * Modules and devices Initializations
	 * Timer, TWI, UART, Motor and Buzzer
	 */
	/* Set TWI Speed to 400KHz and define controller address to 0x01 "when it is considered a slave" */
	TWI_ConfigType twiConfig = { 0x01, RATE_400K };
	TWI_init(&twiConfig);
	/*Set UART frame to 8 bit data, no parity bit and single stop bit
	 *Baud rate to 9600
	 */
	UART_ConfigType uartConfig = { EIGHT_BITS, NO_PARITY, ONE_BIT, RATE_9600 };
	UART_init(&uartConfig);
	/*
	 * Motor and buzzer initialization by setting their pins to output and turning them off initially
	 */
	Buzzer_init();
	DCMotor_init();
	/*
	 * Enable global interrupt
	 */
	SREG = 1 << 7;
	/*
	 * Arrays "Strings" to hold the input passwords by the user and retrieve password from memory
	 */
	uint8 firstPassword[6], secondPassword2[6];
	/* Flags for checking matching passwords and
	 * check if it is the first time for the user to use the program
	 * variable for holding UART received data
	 */
	uint8 matchingPasswordFlag = FALSE, firstTimeFlag, uartData;
	/*
	 * Holding until the HMI finishes initializations
	 */
	while (UART_receiveData() != 0xFF)
		;
	/*
	 * read the address that holds the first time flag
	 */
	EEPROM_readByte(FIRSTTIMEFLAGADDRESS, &firstTimeFlag);
	/* if it is the first time "the value stored in the address doesn't equal 'F'"
	 * Store 'F' in this location
	 */
	if (firstTimeFlag != 'F') {
		EEPROM_writeByte(FIRSTTIMEFLAGADDRESS, 'F');
	}
	/*
	 * send 'Y' to the HMI ECU to inform that it will receive the first time flag value
	 */
	UART_sendData('Y');
	UART_sendData(firstTimeFlag);

	/*control code */
	while (1) {
		/*receiving values from HMI to get informed by what action is happening*/
		uartData = UART_receiveData();
		switch (uartData) {
		/*
		 * if HMI sent 'C' that means it will start sending passwords for set or reset and
		 * will wait for the result
		 */
		case 'C':
			/*receiving first input password*/
			UART_receiveString(firstPassword);
			/*send 'C' to inform HMI to start sending the second password*/
			UART_sendData('C');
			/*receiving second input password*/
			UART_receiveString(secondPassword2);
			/*checks two passwords equality*/
			matchingPasswordFlag = arePasswordsEqual(firstPassword,
					secondPassword2);
			/*send 'C' to inform HMI that it will receive the result of the comparison*/
			UART_sendData('C');
			/*send 'A' if the two passwords are matched and 'B' it they aren't*/
			if (matchingPasswordFlag) {
				UART_sendData('A');
				EEPROM_writeSting(PASSSTARTADDRESS, firstPassword);
			} else
				UART_sendData('B');
			break;
			/*
			 * if HMI sent 'M' that means it will start sending password to
			 * check if it matches the stored password and
			 * will wait for the result
			 */
		case 'M':
			/*receiving input password*/
			UART_receiveString(firstPassword);
			/*retrieve password stored in memory*/
			EEPROM_readString(PASSSTARTADDRESS, secondPassword2, 5);
			secondPassword2[5] = '\0';
			/*check the input password*/
			matchingPasswordFlag = arePasswordsEqual(firstPassword,
					secondPassword2);
			/*send 'Z' to inform HMI that it will receive the result of the comparison*/
			UART_sendData('Z');
			/*send 'A' if the two passwords are matched and 'B' it they aren't*/
			if (matchingPasswordFlag) {
				UART_sendData('A');
			} else
				UART_sendData('B');
			break;
			/*
			 * if HMI sent 'E' that means that user entered a wrong password 3 times
			 * the control ECU starts the buzzer and initializes the timer
			 * set the call back to the wrong password function
			 */
		case 'E':
			Buzzer_on();
			Timer1_init(&timerConfig_7_5_Seconds);
			Timer1_setCallBack(wrongPassword);
			break;
			/*
			 * if HMI sent 'T' that means that user entered the right password
			 * the control ECU starts the motor and initializes the timer
			 * set the call back to the open door function
			 */
		case 'T':
			DCMotor_rotate(CLOCK_WISE, MOTOR_FULL_SPEED);
			Timer1_init(&timerConfig_7_5_Seconds);
			Timer1_setCallBack(openDoor);
		}
	}
}
