/******************************************************************************
 *
 * Module: EEPROM
 *
 * File Name: eeprom.h
 *
 * Description: Header file for the EEPROM driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#ifndef EEPROM_EEPROM_H_
#define EEPROM_EEPROM_H_

#include "../std_types.h"

/*******************************************************************************
 *                      		definitions			                           *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description:
 * A function to write a byte in the external EEPROM in a certain address
 */
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data);
/*
 * Description:
 * A function to write a string in the external EEPROM in a certain address
 */
void EEPROM_writeSting(uint16 u16startAddr, uint8 * u8data);
/*
 * Description:
 * A function to read a byte from the external EEPROM from a certain address
 */
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data);
/*
 * Description:
 * A function to read a String with a certain size from the external EEPROM from a certain address
 */
void EEPROM_readString(uint16 u16startAddr, uint8 * u8data, uint8 bytesNumber);

#endif /* EEPROM_EEPROM_H_ */
