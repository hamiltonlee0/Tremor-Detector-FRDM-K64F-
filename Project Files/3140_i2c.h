/**
 * @file 3140_i2c.h
 * @author Samuel DiPietro
 * @copyright All rights reserved, 2020
 *
 * This file holds function prototypes for concurrency functions
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

#ifndef __3140_i2c__
#define __3140_i2c__

/*----------------------------------------------------------------------------
  Accelerometer Register Definitions
 *----------------------------------------------------------------------------*/

#define FXOS8700CQ_ADDRESS 0x1D
#define FXOS8700CQ_STATUS 0x00
#define FXOS8700CQ_WHOAMI 0x0D
#define FXOS8700CQ_XYZ_DATA_CFG 0x0E
#define FXOS8700CQ_CTRL_REG1 0x2A
#define FXOS8700CQ_CTRL_REG2 0x2B
#define FXOS8700CQ_M_CTRL_REG1 0x5B
#define FXOS8700CQ_M_CTRL_REG2 0x5C
#define FXOS8700CQ_WHOAMI_VAL 0xC7

/*----------------------------------------------------------------------------
  I2C Register Definitions and other important values
 *----------------------------------------------------------------------------*/

#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24U
#define I2C_RELEASE_BUS_COUNT 100U
#define I2C_BAUDRATE 100000U
#define I2C_CHANNEL_BASE I2C0

#if   (CLOCK_SETUP == 0)
	#define BUS_CLOCK_FREQ 20485760u
#elif (CLOCK_SETUP == 1)
	#define BUS_CLOCK_FREQ 60000000u
#endif /* System Clock Setup > 1 */

extern i2c_master_handle_t g_m_handle;

volatile extern uint8_t nakFlag;		// I2C Transmission 
volatile extern uint8_t completionFlag;

/* Initializes the I2C channel for use with accelerometer or other I2C device */
int I2C_Init(void);

/* Initiates a read transaction between the NXP K64FN1xxx12 and i2c decive
 * Input Args[]		deviceAddr: The Target device i2c address
 *					regAddr: The Target Register to read (in multibyte read, this is the base addr)
 *					rxBuff: A pointer to the char, or char array for recieved bytes
 *					rxSize: Size of the array of recieved bytes (also number of bytes to recieve)
 * 
 * Output: Returns 1 on sucessful read, with the read bytes in the rxBuffer. Returns -1 on failure
 *			to read bytes
 */
int I2C_ReadRegs(uint8_t deviceAddr, uint8_t regAddr, uint8_t *rxBuff, uint32_t rxSize);

/* Initiates a read transaction between the NXP K64FN1xxx12 and i2c decive
 * Input Args[]		deviceAddr: The Target device i2c address
 *					regAddr: The Target Register to write
 *					value: The value to be written in a register
 *
 * Output: Returns 1 on sucessful write. Returns -1 on failure to write bytes
 */
int I2C_WriteReg( uint8_t deviceAddr, uint8_t  regAddr, uint8_t value );
int I2C_WriteReg1( uint8_t deviceAddr, uint8_t value );

/* I2C Callback Function. Called after a sucessful transfer 
 *	-Should never get called inline in your implementation
 */
void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);

/* Forces a bus release by siezing arbitration, then sending a stop signal */
void I2C_ReleaseBus(void);

#endif /* __3140_i2c__ */
