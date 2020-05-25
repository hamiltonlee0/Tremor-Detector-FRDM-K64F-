/**
 * @file ADC.h
 * @author Hamilton Lee & Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file holds fuctions declarations
 * and constants for ADC on an ADXL335 analog accelerometer
 * 
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#ifndef __ADC_H__
#define __ADC_H__

/*These are the pinas used for ADC in our project*/
#define PTB10 0b01110 /*Accelerometer X (ADC1_SE14)*/
#define PTB11 0b01111  	/*Accelerometer Y (ADC1_SE15)*/ 
#define ADC1_DM1 0b10100  /*Accelerometer Z (ADC1_DM1)*/

/*Offset values for X, Y, Z acceleration for the acclerometer*/
/*User needs to calibrate when placed in the desired static position*/
extern float xOffset;
extern float yOffset;
extern float zOffset;

void ADC_Init(void); /*Initializes ADC Module on the board*/
unsigned int ADC_read16b(char PIN); /*Read 16 bit ADC converted value*/
float readAccelZ(void);/*Reads and returns z axis acceleration*/
float readAccelY(void);/*Reads and return the y axis acceleration*/
float readAccelX(void);/*Reads and returns the x axis acceleration*/
float getXg(unsigned int val); /*Converts X ADC value to float in g */
float getYg(unsigned int val); /*Converts Y ADC value to float in g */
float getZg(unsigned int val); /*Converts Z ADC value to float in g */
void calibrateAccel(void); /*Calibrates the accelerometer on the user*/
float getMag(float X, float Y, float Z); /*Finds the magnitude of acceleration vector minus gravity*/
void checkDefaultAccel(void); /*For Developer use only. Used to check default acceleration values.*/
#endif