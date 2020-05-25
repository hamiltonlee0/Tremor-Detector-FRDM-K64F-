/**
 * @file TremorDetector.h
 * @author Hamilton Lee & Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file contains constants for a tremor detector and display
 * using the MK64F, ADXL335, and 1.2' Adafruit 8x16 LED matrix.
 */

#include "matrixCol.h"
#include "LEDMat.h"
#include "3140_serial.h"
#include "shared_structs.h"

#include "utils.h"
#include "ADC.h"

#ifndef __TremorDetector__
#define __TremorDetector__

/*Column for a flatline detection*/
static char Flatline [8] = {0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00};
/*Column for a small positive detection*/
static char POS1 [8] = {0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00};
/*Column for a positive detection*/
static char POS2 [8] = {0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00};
/*Column for a large positive detection*/
static char POS3 [8] = {0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
/*Column for a very large positive detection*/
static char POS4 [8] = {0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
/*Column for a negative detection*/
static char NEG1 [8] = {0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00};
/*Column for a large negative detection*/
static char NEG2 [8] = {0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00};
/*Column for a very large negative detection*/
static char NEG3 [8]  = {0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01};

#endif