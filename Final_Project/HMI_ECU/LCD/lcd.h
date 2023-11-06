/************************************************************************************
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the AVR LCD driver
 *
 * Author: Mahmoud Khalafallah
 *
 ************************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "../std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define LCD_CTRL_PORT PORTB_ID
#define LCD_DATA_PORT PORTA_ID
#define LCD_RS PIN0_ID
#define LCD_E PIN1_ID
#define LCD_Mode 8

#if (LCD_Mode==4)
#define LCD_D4_PIN PIN3_ID
#define LCD_D5_PIN PIN4_ID
#define LCD_D6_PIN PIN5_ID
#define LCD_D7_PIN PIN6_ID
#endif

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * LCD initialization
 * 1. Set RS and RW and Enable pin to be output
 * 2. Set Data port to be output
 * 3. function set
 * 4. Display control
 * 5. Clear display
 * 6. Entry mode set
 */
void LCD_Init();
/*
 * Description:
 * Sending command to LCD
 * Clear RS pin
 * Set enable pin
 * send command
 * clear enable pin
 */
void LCD_sendCommand(uint8 Instruction);
/*
 * Description:
 * print char on LCD
 * 1. Set RS pin
 * 2. Set enable pin
 * 3. send data
 * 4. clear enable pin
 */
void LCD_displayCharacter(uint8 data);
/*
 * Description:
 * Printing string on the LCD
 * Loop on the string
 * print each character with diplayCharacter function
 */
void LCD_displayString(uint8 * data);
/*
 * Description:
 * Clearing LCD
 */
void LCD_ClearDisplay();
/*
 * Description:
 * Set the cursor to start in a certain position
 */
void LCD_SetCursor(uint8 row ,uint8 col);
/*
 * Description:
 * print 4 bytes signed numbers
 */
void LCD_PrintSigned32BitNum(sint32 num);
/*
 * Description:
 * print 4 bytes float numbers
 */
void LCD_PrintNumFloat(float32 num);
/*
 * Description:
 * print 2 bytes unsigned numbers
 */
void LCD_PrintNum16Bit(uint16 num);
#endif /* LCD_H_ */
