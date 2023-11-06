/******************************************************************************
 *
 * Module: TWI
 *
 * File Name: TWI.c
 *
 * Description: Source file for the AVR TWI driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#include "twi.h"
#include "../common_macros.h"
#include <avr/io.h>

/*
 * Description:
 * Function to initialize I2C with :
 * 1. Baud Rate
 * 2. Device address
 */
void TWI_init(const TWI_ConfigType *Config_Ptr) {
	uint8 twbrValue = 0;
	/*
	 *Determine the content of TWBR register depending on baud rate "assuming no prescaler"
	 *TWSR No prescaler -> 00 "TWPS0,TWPS1"
	 *SCL Frequency = (F_CPU)/(16+2*(TWBR)*4^TWPS
	 *SCL Frequency = (F_CPU)/(16+2*(TWBR)*4^0
	 *TWBR = F_CPU / (2* SCL Frequency) - 8
	 */
	twbrValue = (uint8) ((uint16) F_CPU / (2 * Config_Ptr->bit_rate)) - 8;
	TWSR = 0;
	TWBR = twbrValue;
	/* Two Wire Bus address my address if any master device want to call me: 0x01 (used in case this MC is a slave device)
	 General Call Recognition: Off */
	TWAR = Config_Ptr->address << 1;
	/*
	 * Enabling TWI
	 */
	TWCR = 1 << TWEN;
}
/*
 * Description:
 * Function to Send Start bit
 */
void TWI_start(void) {
	/*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	/* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	LOOP_UNTIL_BIT_IS_SET(TWCR, TWINT);
}
/*
 * Description:
 * Function to Send Stop bit
 */
void TWI_stop(void) {
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
/*
 * Description:
 * function to put next data byte to be transmitted
 * and wait for it to be sent successfully
 */
void TWI_writeByte(uint8 data) {
	/* Put data On TWI data Register */
	TWDR = data;
	/*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	LOOP_UNTIL_BIT_IS_SET(TWCR, TWINT);
}
/*
 * Description:
 * Function to read byte and send acknowledge bit after receiving
 * Wait until the byte is received successfully
 */
uint8 TWI_readByteWithACK(void) {
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	LOOP_UNTIL_BIT_IS_SET(TWCR, TWINT);
	/* Read Data */
	return TWDR;
}
/*
 * Description:
 * Function to read byte without sending acknowledge bit after receiving
 * wait until the bit is received successfully
 */
uint8 TWI_readByteWithNACK(void) {
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	LOOP_UNTIL_BIT_IS_SET(TWCR, TWINT);
	/* Read Data */
	return TWDR;
}
/*
 * Description:
 * Function to get the status of the TWI logic and the Two-wire Serial Bus.
 */
uint8 TWI_getStatus(void) {
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = TWSR & 0xF8;
	return status;
}
