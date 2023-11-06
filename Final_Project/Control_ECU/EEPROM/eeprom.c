/******************************************************************************
 *
 * Module: EEPROM
 *
 * File Name: eeprom.c
 *
 * Description: Source file for the EEPROM driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/
#include "../TWI/twi.h"
#include "eeprom.h"
#include <util/delay.h>
/*
 * Description:
 * A function to write a byte in the external EEPROM in a certain address
 */
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data) {
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8) (0xA0 | ((u16addr & 0x0700) >> 7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8) (u16addr));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* write byte to eeprom */
	TWI_writeByte(u8data);
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}
/*
 * Description:
 * A function to read a byte from the external EEPROM from a certain address
 */
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data) {
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8) ((0xA0) | ((u16addr & 0x0700) >> 7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8) (u16addr));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8) ((0xA0) | ((u16addr & 0x0700) >> 7) | 1));
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;

	/* Read Byte from Memory without send ACK */
	*u8data = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}
/*
 * Description:
 * A function to write a string in the external EEPROM in a certain address
 */
void EEPROM_writeSting(uint16 u16startAddr, uint8 *u8data) {
	uint8 i = 0;
	while (u8data[i] != '\0') {
		EEPROM_writeByte(u16startAddr + i, u8data[i]);
		_delay_ms(10);
		i++;
	}
}
/*
 * Description:
 * A function to read a String with a certain size from the external EEPROM from a certain address
 */
void EEPROM_readString(uint16 u16startAddr, uint8 *u8data, uint8 bytesNumber) {
	uint8 i = 0;
	while (i < bytesNumber) {
		EEPROM_readByte(u16startAddr + i, &u8data[i]);
		_delay_ms(10);
		i++;
	}
	u8data[i] = '\0';
}
