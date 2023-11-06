/******************************************************************************
 *
 * Application
 *
 * File Name: main.c
 *
 * Description: Application of Human interface ECU
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/
#include "LCD/lcd.h"
#include "KEYPAD/keypad.h"
#include "UART/uart.h"
#include "util/delay.h"

/*******************************************************************************
 *                      		Global variables			                    *
 *******************************************************************************/

/*Strings for holding password and re-entered password*/
uint8 passwordArray[6], reEnteredPasswordArray[6];
/*
 * variable for holding the keypad input value
 * variable for counting the index to put input values in the array
 */
uint8 input, counter = 0;
/*Flag for controlling input in the password array or the re entered password array
 * True if the password was entered
 */
uint8 inputFlag = FALSE, matchingPasswordsFlag = FALSE;

/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
/*
 * Description:
 * Function to set or reset the password
 * the user enters two passwords and controller sends them the control ECU and waits for the result
 * if they are matched the program continues
 * else the whole process is repeated again
 */
void SetPassword() {
	LCD_ClearDisplay();
	LCD_displayString("plz enter pass:");
	LCD_SetCursor(1, 0);
	/*Enters a while loop until the user enters two matched passwords*/
	while (matchingPasswordsFlag != 'A') {
		/*taking the input from the keypad*/
		input = KEYPAD_getPressedKey();
		switch (input) {
		/*if the input is number*/
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 0:
			/*checks which password array to store in depending on the input flag*/
			if (inputFlag == FALSE && counter < 5) {
				/*converting the input value to ascii and storing it in the password array*/
				passwordArray[counter++] = input + '0';
				LCD_displayCharacter('*');
			} else if (inputFlag == TRUE && counter < 5) {
				/*converting the input value to ascii and storing it in the re entered password array*/
				reEnteredPasswordArray[counter++] = input + '0';
				LCD_displayCharacter('*');
			}
			break;
			/*if the user pressed enter*/
		case 13:
			/*
			 * depending on which array is filled:
			 * if it is the first array the user is asked to re enter the same password
			 */
			if (inputFlag == FALSE) {
				/*
				 * Prepare the program to take the re entered password by reseting the counter
				 * clearing display and print the message to re enter the same password
				 */
				inputFlag = TRUE;
				/*fill the last element by '# to prepare it for sending'*/
				passwordArray[counter] = '#';
				counter = 0;
				LCD_ClearDisplay();
				LCD_displayString("plz re-enter the ");
				LCD_SetCursor(1, 0);
				LCD_displayString("same pass: ");
				/*
				 *if it is the second array the program resets the flags and
				 *sends the passwords to the control ECU and waits for the results
				 */
			} else {
				/*fill the last element by '# to prepare it for sending'*/
				reEnteredPasswordArray[counter] = '#';
				counter = 0;
				inputFlag = FALSE;
				LCD_ClearDisplay();
				/*send 'C' to the control ECU to inform it that the passwords will be sent*/
				UART_sendData('C');
				/*send the first password*/
				UART_sendString(passwordArray);
				/*wait for the control ECU to send 'C' to send the second password*/
				while (UART_receiveData() != 'C')
					;
				/*send the second password*/
				UART_sendString(reEnteredPasswordArray);
				/*wait for the control ECU to send 'C' to receive the result*/
				while (UART_receiveData() != 'C')
					;
				matchingPasswordsFlag = UART_receiveData();
				/*if the two passwords are not matched result = 'B' the whole process is repeated again*/
				if (matchingPasswordsFlag == 'B') {
					LCD_ClearDisplay();
					LCD_displayString("plz enter pass:");
					LCD_SetCursor(1, 0);
				}
			}
			break;
		default:
			break;
		}
	}
	/*display a message to inform the user that the password has been saved*/
	LCD_displayString("Password Saved");
	LCD_SetCursor(1, 2);
	LCD_displayString("Successfully");
	_delay_ms(1000);
}

/*******************************************************************************
 *                      		 Main      			                           *
 *******************************************************************************/
int main(void) {
	/*
	 * Modules and devices Initializations
	 * Timer, UART and LCD
	 * 1. Set UART frame to 8 bit data, no parity bit and single stop bit
	 * Baud rate to 9600
	 */
	UART_ConfigType uartConfig = { EIGHT_BITS, NO_PARITY, ONE_BIT, RATE_9600 };
	UART_init(&uartConfig);
	/* 2. Initializing LCD*/
	LCD_Init();
	/*
	 * variables to store data through the program:
	 * option choice to store user's choice whether to open the door or to reset the password
	 * first time flag to check whether the user used the program before or not
	 * first input counter to mark the indices of the input password array
	 * check data to store the result whether the passwords are matched are not
	 * repeat flag to repeat the process when the user enters wrong password
	 */
	uint8 optionChoice, firstTimeFlag, inputCounter = 0, checkData, repeatFlag =
			FALSE, numberOfWrongPasswords = 0, truePassFlag = FALSE;
	/*String of the password user enters*/
	uint8 inputPassword[7];
	/*send 0xFF to inform control ECU that the initializations are done*/
	UART_sendData(0xFF);
	/*wait until control ECU sends 'Y' to receive the value of first time flag*/
	while (UART_receiveData() != 'Y')
		;
	firstTimeFlag = UART_receiveData();
	/*if it is the first time for the user asks him to set the password*/
	if (firstTimeFlag != 'F')
		SetPassword();
	/*Human interface code */
	while (1) {
		/*display a message with the choices*/
		LCD_ClearDisplay();
		LCD_displayString("+ : Open Door");
		LCD_SetCursor(1, 0);
		LCD_displayString("- : Change Pass");
		optionChoice = KEYPAD_getPressedKey();
		/*for either choice asks the user to enter the password*/
		LCD_ClearDisplay();
		LCD_displayString("plz enter pass:");
		LCD_SetCursor(1, 0);
		if (optionChoice == '+' || optionChoice == '-') {
			/*Enters a loop until the user enters the right password*/
			while (1) {
				input = KEYPAD_getPressedKey();
				switch (input) {
				/*if the input is number*/
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 0:
					/*stores the value in the password array*/
					inputPassword[inputCounter] = input + '0';
					inputCounter++;
					/*displaying * instead of the number to insure privacy */
					LCD_displayCharacter('*');
					break;
					/*if the users presses enter*/
				case 13:
					/*fill the last element with '#' to prepare the password for sending*/
					inputPassword[inputCounter] = '#';
					/*reset the counter*/
					inputCounter = 0;
					/*send 'M' to inform HMI that the password will be sent and will wait for the result*/
					UART_sendData('M');
					/*send the password*/
					UART_sendString(inputPassword);
					/*waits until the control ECU send 'Z' to receive the result*/
					while (UART_receiveData() != 'Z')
						;
					checkData = UART_receiveData();
					LCD_ClearDisplay();
					/*if the password is right result = 'A' the LCD displays a message and the program proceeds*/
					if (checkData == 'A') {
						LCD_ClearDisplay();
						LCD_SetCursor(0, 3);
						LCD_displayString("TRUE PASS");
						_delay_ms(1000);
						/*set the flag to repeat to proceed the code */
						repeatFlag = TRUE;
						break;
						/*
						 * if the password is not right result = 'B' the LCD displays a message
						 * and increases number of wrong passwords
						 */
					} else if (checkData == 'B') {
						/*if the user enters wrong password 3 times*/
						if (numberOfWrongPasswords == 2) {
							/*reset the number of wrong passwords*/
							numberOfWrongPasswords = 0;
							/*corrupting option choice by 'E' Error occurred to continue the code*/
							optionChoice = 'E';
							/*Send 'E' to control ECU to inform it that user entered a wrong password 3 times */
							UART_sendData('E');
							LCD_ClearDisplay();
							/*display error message*/
							LCD_SetCursor(0, 5);
							LCD_displayString("ERROR");
							LCD_SetCursor(1, 2);
							LCD_displayString("WRONG PASS");
							/*wait until the control ECU informs the HMI that the minute is finished*/
							while (UART_receiveData() != 'D')
								;
							LCD_ClearDisplay();
							/*set repeat flag to exit the loop*/
							repeatFlag = TRUE;
							break;
						}
						/*if the user enters the wrong password less than 3 times displays a message and continue the code */
						LCD_ClearDisplay();
						LCD_SetCursor(0, 3);
						LCD_displayString("Wrong Pass");
						_delay_ms(1000);
						LCD_ClearDisplay();
						numberOfWrongPasswords++;
						LCD_displayString("plz enter pass:");
						LCD_SetCursor(1, 0);
					}
				}
				/*break the loop if the user entered wrong password three times or the right password*/
				if (repeatFlag) {
					repeatFlag = FALSE;
					break;
				}
			}
		}
		/*if the user choice was to open the door*/
		if (optionChoice == '+') {
			/*sends 'T'to inform control ECU that the user wants to open the door*/
			UART_sendData('T');
			/*display message*/
			LCD_ClearDisplay();
			LCD_SetCursor(0, 4);
			LCD_displayString("Door is");
			LCD_SetCursor(1, 3);
			LCD_displayString("Unlocking");
			/*wait until the control ECU informs that 15 seconds are finished*/
			while (UART_receiveData() != 'U')
				;
			/*dispaly message*/
			LCD_ClearDisplay();
			LCD_SetCursor(0, 4);
			LCD_displayString("Welcome");
			/*wait until the control ECU informs that 3 seconds are finished*/
			while (UART_receiveData() != 'H')
				;
			/*dispaly message*/
			LCD_ClearDisplay();
			LCD_SetCursor(0, 4);
			LCD_displayString("Door is");
			LCD_SetCursor(1, 3);
			LCD_displayString("Locking");
			/*wait until the control ECU informs that 15 seconds are finished*/
			while (UART_receiveData() != 'L')
				;
			/*if the user choice was to reset the password*/
		} else if (optionChoice == '-') {
			SetPassword();
		}
	}
}
