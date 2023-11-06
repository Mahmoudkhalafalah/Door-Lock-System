/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: header file for the AVR UART driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/
#ifndef UART_H_
#define UART_H_

#include "../std_types.h"
/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum {
	NO_PARITY, EVEN_PARITY = 2, ODD_PARITY
} UART_Parity;

typedef enum {
	ONE_BIT, TWO_BITS
} UART_StopBit;

typedef enum {
	FIVE_BITS, SIX_BITS, SEVEN_BITS, EIGHT_BITS, NINE_BITS
} UART_BitData;

typedef enum {
	RATE_2400 = 2400,
	RATE_4800 = 4800,
	RATE_9600 = 9600,
	RATE_14400 = 14400,
	RATE_19200 = 19200,
	RATE_28800 = 28800,
	RATE_38400 = 38400,
	RATE_57600 = 57600,
	RATE_76800 = 76800,
	RATE_115200 = 115200,
	RATE_230400 = 230400,
	RATE_250K = 250000,
	RATE_500K = 500000,
	RATE_1_M = 1000000
} UART_BaudRate;

typedef struct {
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
	UART_BaudRate baud_rate;
} UART_ConfigType;
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * initializing uart module :
 * baud rate & frame:
 * 1-parity bit
 * 2-number of data bits
 * 3-stop bits"
 */
void UART_init(const UART_ConfigType *config_ptr);
/*
 * Description:
 * sending Data with UART
 */
void UART_sendData(uint8 data);
/*
 * Description:
 * receiving Data with UART
 */
uint8 UART_receiveData(void);
/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);
/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str);

#endif /* UART_H_ */
