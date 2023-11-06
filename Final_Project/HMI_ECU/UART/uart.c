/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the AVR UART driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#include "uart.h"
#include "../common_macros.h"
#include <avr/io.h>

/*
 * Description:
 * initializing uart module :
 * baud rate & frame:
 * 1-parity bit
 * 2-number of data bits
 * 3-stop bits"
 */
void UART_init(const UART_ConfigType *config_ptr) {
	uint16 UBRRValue = 0;

	/*Calculating the value of UBRR to obtain certain baud rate*/
	UBRRValue = (uint16) (((F_CPU / (config_ptr->baud_rate * 8UL))) - 1);
	/*
	 * Writing this bit to one to reduce the divisor of the baud rate divider
	 * from 16 to 8 to double the transfer rate.
	 */
	UCSRA = 1 << U2X;
	/*
	 * Writing RXEN bit and TXEN to one to enable the USART Receiver.
	 * The Receiver and transmitter will override normal port operation
	 * for the RxD and TxD pins when enabled.
	 * if 9 bit mode were chosen set UCZ2 to 1 "masking bit 2 in bit_data which is one when 9 bit mode is chosen"
	 */
	UCSRB = (1 << TXEN) | (1 << RXEN) | ((config_ptr->bit_data & 0x04) << 1);
	/*
	 * Writing URSEL to one to select register UCSRC to be written
	 * USMEL bit is zero to choose asynchronous mode
	 * select parity mode in UPM1:0 bits
	 * select data bits in UCSZ0 & UCSZ1 bits "masking first two bits only in bit_data"
	 * select stop bits in USBS bit
	 */
	UCSRC = (1 << URSEL) | (config_ptr->stop_bit << 3)
			| ((config_ptr->bit_data & 0x03) << 1)
			| ((config_ptr->parity) << 4);

	/*
	 * Writing URSEL to zero to select register UBBRH to be accessed
	 * determine the content of UBRRL register depending on the baud rate
	 * UBRR = (Fosc/(8*baudrate))-1
	 * put the Lower 8 bits in UBRRL register and 8:11 bits in UBRRH
	 */
	UBRRH = 0;
	UBRRH = UBRRValue >> 8;
	UBRRL = UBRRValue & 0x00FF;

}
/*
 * Description:
 * sending Data with UART
 */
void UART_sendData(uint8 data) {
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	LOOP_UNTIL_BIT_IS_SET(UCSRA, UDRE);

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;
}
/*
 * Description:
 * receiving Data with UART
 */
uint8 UART_receiveData(void) {
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	LOOP_UNTIL_BIT_IS_SET(UCSRA, RXC);
	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	return UDR;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str) {
	uint8 i = 0;

	/* Send the whole string */
	while (Str[i] != '\0') {
		UART_sendData(Str[i]);
		i++;
	}
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str) {
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_receiveData();

	/* Receive the whole string until the '#' */
	while (Str[i] != '#') {
		i++;
		Str[i] = UART_receiveData();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}

