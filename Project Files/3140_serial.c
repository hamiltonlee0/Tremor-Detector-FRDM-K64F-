/**
 * @file 3140_serial.c
 * @author Samuel DiPietro
 * @author Felipe Fortuna
 * @copyright All rights reserved, 2020
 *
 * This file holds function prototypes for serial using UART
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include "3140_serial.h"


/* Initiates the UART0 Channel for communication over a serial line (i.e usb)
 * 	-make sure that you have the mbed serial comms. driver installed from
 *	 https://os.mbed.com/docs/mbed-os/v5.15/tutorials/windows-serial-driver.html
 * 	 and make sure that you have putty installed from
 *	 
 */
void uart_init(void)
{
	SIM->SCGC4 |= 1<<10;
	SIM->SCGC5 |= 1<<10;
	
	PORTB->PCR[16] &= ~PORT_PCR_MUX(7);
	PORTB->PCR[16] |= PORT_PCR_MUX(3);
	PORTB->PCR[17] &= ~PORT_PCR_MUX(7);
	PORTB->PCR[17] |= PORT_PCR_MUX(3);
	
	uint16_t sbr = SystemCoreClock / (16 * 9600);
	
	UART0->C2 &= ~(0x0C);
	UART0->BDL = (uint8_t) (sbr & 0x00FF); // Set the baud Rate
	UART0->BDH &= 0xE0;
	UART0->BDH |= (uint8_t) ((sbr & 0x1F00) >> 8);
	
	UART0->C1 = 0;
	UART0->C2 |= 0x0C;
	
}

/* Initiates a single character send over UART
 * Input Args[]		ch: Input char that will be sent over UART
 */
void uart_putChar(char ch)
{
	while(!(UART0->S1 & 0x80));
	UART0->D = (uint8_t) ch;
}

/* Sends a string a characters over UART
 * Input Args[]		*ptr_str: String to be sent
 */
void uart_putString( char *ptr_str)
{
	while(*ptr_str){
		uart_putChar(*ptr_str++);
	}
}