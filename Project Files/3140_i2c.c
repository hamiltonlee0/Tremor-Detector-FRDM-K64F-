/**
 * @file 3140_i2c.c
 * @author Samuel DiPietro modified by Hamilton Lee and Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file holds function prototypes for I2C bus Read/Write functions
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 * Code base is the same as Sam DiPietro's version. Function written for
 * I2C write of 1 byte instead of 2 bytes. Necessary for LED Matrix.
 */

#include "3140_i2c.h"

i2c_master_handle_t g_m_handle;      // Handle Struct for transfer   

volatile uint8_t nakFlag		= 0; // No acknowledgement flag
volatile uint8_t completionFlag = 0; // Transfer Complete Flag


/* I2C Callback Function. Called after a sucessful transfer */
void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        completionFlag = true;
    }
    /* Signal transfer success when received success status. */
    if ((status == kStatus_I2C_Nak) || (status == kStatus_I2C_Addr_Nak))
    {
        nakFlag = true;
    }
}
/* I2C delay Function. Timed so as to not violate start/stop hold time */
static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

/* Initiates a read transaction between the NXP K64FN1xxx12 and i2c decive
 * Input Args[]		deviceAddr: The Target device i2c address
 *					regAddr: The Target Register to read (in multibyte read, this is the base addr)
 *					rxBuff: A pointer to the char, or char array for recieved bytes
 *					rxSize: Size of the array of recieved bytes (also number of bytes to recieve)
 * 
 * Output: Returns 1 on sucessful read, with the read bytes in the rxBuffer. Returns -1 on failure
 *			to read bytes
 */
int I2C_ReadRegs(uint8_t deviceAddr, uint8_t regAddr, uint8_t *rxBuff, uint32_t rxSize)
{
    i2c_master_transfer_t masterTransfer;
    memset(&masterTransfer, 0, sizeof(masterTransfer));
	
    masterTransfer.slaveAddress   = deviceAddr;
    masterTransfer.direction      = kI2C_Read;
    masterTransfer.subaddress     = regAddr;
    masterTransfer.subaddressSize = 1;
    masterTransfer.data           = rxBuff;
    masterTransfer.dataSize       = rxSize;
    masterTransfer.flags          = kI2C_TransferDefaultFlag;

    // Begin Byte(s) read operation 
    I2C_MasterTransferNonBlocking(I2C_CHANNEL_BASE, &g_m_handle, &masterTransfer);

    /*  wait for transfer completed. */
    while ((!nakFlag) && (!completionFlag))
    {
    }

    nakFlag = 0;

    if (completionFlag == true)
    {
        completionFlag = 0;
        return 1; // Transmission Suceeded
    }
    else
    {
        return -1; // Transmission Failed
    }

}


/* Initiates a read transaction between the NXP K64FN1xxx12 and i2c decive
 * Input Args[]		deviceAddr: The Target device i2c address
 *					regAddr: The Target Register to write
 *					value: The value to be written in a register
 *
 * Output: Returns 1 on sucessful write. Returns -1 on failure to write bytes
 */
int I2C_WriteReg( uint8_t deviceAddr, uint8_t  regAddr, uint8_t value )
{
    uint8_t buff[2] = {regAddr, value};
    
    i2c_master_transfer_t masterTransfer;                   // Make Transfer Struct
    memset(&masterTransfer,0,sizeof(masterTransfer));

    masterTransfer.slaveAddress     = deviceAddr;           // Init Transfer
    masterTransfer.direction        = kI2C_Write;            
    masterTransfer.subaddress       = 0;
    masterTransfer.subaddressSize   = 0;
    masterTransfer.data             = buff;
    masterTransfer.dataSize					= 2;
    masterTransfer.flags            = kI2C_TransferDefaultFlag;
    
    I2C_MasterTransferNonBlocking( I2C_CHANNEL_BASE, &g_m_handle, &masterTransfer);

    while ( (!nakFlag) && (!completionFlag) ); // Busy Wait for Signal

    nakFlag = 0;

    if (completionFlag == 1)
    {
        completionFlag = 0;
        return 1;
    }

    return -1; // Transmission failed 

}

/* Initiates a read transaction between the NXP K64FN1xxx12 and i2c decive
 * Input Args[]		deviceAddr: The Target device i2c address
 *					value: The value to be written in a register
 * Same as the above command, but only writes one byte of data
 * Output: Returns 1 on sucessful write. Returns -1 on failure to write bytes
 */
int I2C_WriteReg1( uint8_t deviceAddr, uint8_t value )
{
    
    i2c_master_transfer_t masterTransfer;                   // Make Transfer Struct
    memset(&masterTransfer,0,sizeof(masterTransfer));

    masterTransfer.slaveAddress     = deviceAddr;           // Init Transfer
    masterTransfer.direction        = kI2C_Write;            
    masterTransfer.subaddress       = 0;
    masterTransfer.subaddressSize   = 0;
    masterTransfer.data             = &value;
    masterTransfer.dataSize					= 1;
    masterTransfer.flags            = kI2C_TransferDefaultFlag;
    
    I2C_MasterTransferNonBlocking( I2C_CHANNEL_BASE, &g_m_handle, &masterTransfer);

    while ( (!nakFlag) && (!completionFlag) ); // Busy Wait for Signal

    nakFlag = 0;

    if (completionFlag == 1)
    {
        completionFlag = 0;
        return 1;
    }

    return -1; // Transmission failed 

}

/* Forces a bus release by siezing arbitration, then sending a stop signal */
void I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux        = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1U;
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

/* Initializes the I2C channel for use with accelerometer or other I2C device */
int I2C_Init(void)
{

		if (I2C_CHANNEL_BASE == I2C0) {
			SIM->SCGC5  |= SIM_SCGC5_PORTE_MASK;						// Turn on the Port Clock
			PORTE->PCR[24] |= PORT_PCR_MUX(0x05) | PORT_PCR_ODE_MASK; 	// Configure pins for i2c
			PORTE->PCR[25] |= PORT_PCR_MUX(0x05) | PORT_PCR_ODE_MASK;
			SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
		} else {
			/* Todo: Implement other I2C Channels */
			return -1;
		}
		I2C_MasterTransferCreateHandle(I2C_CHANNEL_BASE, &g_m_handle, i2c_master_callback, NULL);

		/* Get the default Configuration for the i2c bus */
		// 		-Baud Rate 100kBps
		//		-Interrupts Disbaled
		i2c_master_config_t masterConfig;
		I2C_MasterGetDefaultConfig(&masterConfig);
	
		
		masterConfig.baudRate_Bps = I2C_BAUDRATE;
		uint32_t sourceClock = BUS_CLOCK_FREQ;
	
		/* Open up i2c on I2C0 (The one that the accelerometer is on) */
		I2C_MasterInit(I2C_CHANNEL_BASE, &masterConfig, sourceClock);

		return 1;

}









