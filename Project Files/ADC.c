/**
 * @file ADC.c
 * @author Hamilton Lee & Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file holds fuctions for ADC on an ADXL335 analog accelerometer
 * 
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include <MK64F12.h>
#include "ADC.h"
#include "Math.h"

/*Short delay Used for calibration*/
void shortDelay(void){
	int j;
	for(j=0; j<10000; j++);
}

/*Offsets for X, Y, and Z to zero the accelerometer
to the desired static position.*/
float xOffset = 0;
float yOffset = 0;
float zOffset = 0;

/*----------------------------------------------------------------------------
  Function that initializes ADC Module on K64f
 *----------------------------------------------------------------------------*/
void ADC_Init(void)	{
	SIM->SCGC5    |= (1 <<  10);  /* Enable Clock to Port B & C */ 
	SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK; /*Enable the ADC1 Clock*/
	ADC1_SC1A |= ADC_SC1_ADCH(0b11111); /*Module is Default set to OFF*/
	ADC1_CFG1 |= ADC_CFG1_MODE(0b11); /*Set Mode to 16 bit conversion single ended (diff = 0)*/
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; /*Enable the ADC1 Clock*/
}

/*----------------------------------------------------------------------------
Return the ADC value read in the input: PIN
 *----------------------------------------------------------------------------*/
unsigned int ADC_read16b(char PIN)	{
	ADC1_SC1A = ADC_SC1_ADCH(PIN) & ADC_SC1_ADCH_MASK; /*Set the pin for ADC1 to read from and start conversion*/
	while(ADC1_SC2 & ADC_SC2_ADACT_MASK); /*ADACT set to 1 means conversion is active so wait until it's over*/
	while(!(ADC1_SC1A & ADC_SC1_COCO_MASK)); /*Wait for Conversion to Finish*/
	return ADC1_RA; /*Return the Value in the register*/

}


/*----------------------------------------------------------------------------
Reads accelerometer in the Z direction in g
 *----------------------------------------------------------------------------*/
float readAccelZ(){
    float z = getZg(ADC_read16b(ADC1_DM1));
		//z = z-zOffset;
    return z;
}

/*----------------------------------------------------------------------------
Reads accelerometer in the Y direction in g
 *----------------------------------------------------------------------------*/
float readAccelY(){
  	float y = getYg(ADC_read16b(PTB11));
		//y=y-yOffset;
    return y;
}
/*----------------------------------------------------------------------------
Reads accelerometer in the X direction in g
 *----------------------------------------------------------------------------*/
float readAccelX(){
    float x = getXg(ADC_read16b(PTB10));
		//x = x-xOffset;
    return x;
}
/*----------------------------------------------------------------------------
Converts X ADC value to float in g
Values belows are manually calibrated.
 *----------------------------------------------------------------------------*/
float getXg(unsigned int val)	{
	float xg = ((float)val-(float)32813)/(float)6626;
	return -1.0*xg;
}

/*----------------------------------------------------------------------------
Converts Y ADC value to float in g
Values belows are manually calibrated.
 *----------------------------------------------------------------------------*/
float getYg(unsigned int val)	{
	float yg = ((float)val-(float)32278)/(float)6537;
	return -1.0*yg;
}

/*----------------------------------------------------------------------------
Converts Z ADC value to float in g
Values belows are manually calibrated.
 *----------------------------------------------------------------------------*/

float getZg(unsigned int val)	{
	float zg = ((float)val-(float)34374)/(float)6809;
	return -1.0*zg;
}

/*----------------------------------------------------------------------------
Find Magnitude of a acceleration vector normalized by gravity (ignoring gravity)
 *----------------------------------------------------------------------------*/
float getMag(float X, float Y, float Z){
	return sqrt(X*X+Y*Y+Z*Z)-1.0;
}

/*----------------------------------------------------------------------------
Find X, Y, and Z offsets after accelerometer is placed in a static position
that it will be used in.

Reads 1000 readings of X, Y, and Z and find the average.
 *----------------------------------------------------------------------------*/
void calibrateAccel(void)	{
	float tempX = 0;
	float tempY = 0;
	float tempZ = 0;
	for(int i=0; i<1000; i++){
		tempX= tempX+ readAccelX();
		tempY= tempY+ readAccelY();
		tempZ= tempZ+ readAccelZ();
	}
	xOffset= tempX/1000;
	yOffset= tempY/1000;
	zOffset= tempZ/1000;
}


/*----------------------------------------------------------------------------
For Developer use only. Used to check default acceleration values.
 *----------------------------------------------------------------------------*/

void checkDefaultAccel(void)	{
	int xAccel = 0;
	int yAccel = 0;
	int zAccel = 0;
	//Check debugger to see the default values
	for(int i = 0; i<1000; i++)	{
		xAccel+=ADC_read16b(PTB10);
		yAccel+=ADC_read16b(PTB11);
		zAccel+=ADC_read16b(ADC1_DM1);
	}
	xAccel = xAccel/1000;
	yAccel = yAccel/1000;
	zAccel = zAccel/1000;
	int k = 0;
}