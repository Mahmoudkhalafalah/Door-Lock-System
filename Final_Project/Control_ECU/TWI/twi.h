/******************************************************************************
 *
 * Module: TWI
 *
 * File Name: twi.h
 *
 * Description: Header file for the AVR TWI driver
 *
 * Author: Mahmoud Khalafallah
 *
 *******************************************************************************/

#ifndef TWI_TWI_H_
#define TWI_TWI_H_

#include "../std_types.h"


/*******************************************************************************
 *                              definitions                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum {
	RATE_100K = 100000,
	RATE_400K = 400000,
	RATE_1M = 1000000,
	RATE_3_4M = 3400000
}TWI_BaudRate;
typedef struct{
 uint8 address;
 TWI_BaudRate bit_rate;
}TWI_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * Function to initialize I2C with :
 * 1. Baud Rate
 * 2. Device address
 */
void TWI_init(const TWI_ConfigType * Config_Ptr);

/*
 * Description:
 * Function to Send Start bit
 */
void TWI_start(void);

/*
 * Description:
 * Function to Send Stop bit
 */
void TWI_stop(void);

/*
 * Description:
 * function to put next data byte to be transmitted
 * and wait for it to be sent successfully
 */
void TWI_writeByte(uint8 data);

/*
 * Description:
 * Function to read byte and send acknowledge bit after receiving
 * Wait until the byte is received successfully
 */
uint8 TWI_readByteWithACK(void);

/*
 * Description:
 * Function to read byte without sending acknowledge bit after receiving
 * wait until the bit is received successfully
 */
uint8 TWI_readByteWithNACK(void);

/*
 * Description:
 * Function to get the status of the TWI logic and the Two-wire Serial Bus.
 */
uint8 TWI_getStatus(void);


#endif /* TWI_TWI_H_ */
