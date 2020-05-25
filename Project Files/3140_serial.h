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

#include "fsl_common.h"

#ifndef __3140_serial__
#define __3140_serial__

void uart_init(void);

void uart_putChar(char ch);

void uart_putString( char *ptr_str);

#endif /* __3140_serial */
