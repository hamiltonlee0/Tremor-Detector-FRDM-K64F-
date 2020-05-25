/**
 * @file TremorDetector.c
 * @author Hamilton Lee & Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file implements a tremor detector and display
 * using the MK64F, ADXL335, and 1.2' Adafruit 8x16 LED matrix.
 */

#include "TremorDetector.h"
#include <math.h>

/*display Values for our LED matrix*/
matCol *displayValues = NULL;

/*Count variable for while loop.
	Accelerometer values read every while loop iteration.
	Display updates every X while loop iterations.*/
	
volatile int count = 0;

/*Current Shape we want to pass into the display*/
char currentShape[8] = {};

/*Average magnitude over the last cycle of acceleration*/	
float avgMag = 0;

/*Initialize Accelerometer data to default values.*/
accelData *initAccelData(void)
{
    accelData *newData = malloc(sizeof(accelData));
    newData->min = 100; //Set to large value that should never me measured
    newData->max = -100;	//set to small value that should never be measured
    newData->avg=0;
    newData->number=0;
		return newData;
}

/*Structs to contain accelration data for X, Y, and Z directions.*/
struct accelData *Xaccel = NULL;
struct accelData *Yaccel = NULL;
struct accelData *Zaccel = NULL;

/*Updates read acceleration values for X direction:
	min, max, average, number all updated*/
void addAccelDataX(float newAccel){
    Xaccel->min = (newAccel < Xaccel->min ? newAccel:Xaccel->min);
    Xaccel->max = (newAccel > Xaccel->max ? newAccel:Xaccel->max);
    Xaccel->avg = (Xaccel->avg*((float)Xaccel->number)+newAccel)/((Xaccel->number)+1);
    Xaccel->number += 1;
}

/*Updates read acceleration values for Y direction:
	min, max, average, number all updated*/
void addAccelDataY(float newAccel){
    Yaccel->min = (newAccel < Yaccel->min ? newAccel:Yaccel->min);
    Yaccel->max = (newAccel > Yaccel->max ? newAccel:Yaccel->max);
    Yaccel->avg = (Yaccel->avg*((float)Yaccel->number)+newAccel)/((Yaccel->number)+1);
    Yaccel->number += 1;
}


/*Updates read acceleration values for Z direction:
	min, max, average, number all updated*/
void addAccelDataZ(float newAccel){
    Xaccel->min = (newAccel < Zaccel->min ? newAccel:Zaccel->min);
    Zaccel->max = (newAccel > Zaccel->max ? newAccel:Zaccel->max);
    Zaccel->avg = (Zaccel->avg*((float)Zaccel->number)+newAccel)/((Zaccel->number)+1);
    Zaccel->number += 1;
}

/*Reset the read acceration data for X, Y, and Z*/
void clearAccelData(){
    Xaccel->min = 100;
    Xaccel->max = -100;
    Xaccel->avg=0;
    Xaccel->number=0;

    Yaccel->min = 100;
    Yaccel->max = -100;
    Yaccel->avg=0;
    Yaccel->number=0;
    
    Zaccel->min = 100;
    Zaccel->max = -100;
    Zaccel->avg=0;
    Zaccel->number=0;
}

/* 
*	Helper method to copy matrix Column values from set templates
*	Necessary because arrays are immutable.
*/
void copyArraytoShape(char arr[8])  {
    for(int i=0; i<8; i++){
        currentShape[i] = arr[i];
    }
}


/*Sets the shape of the column that should be added to the waveform
*	based on recent readings. Quantization values should be set based
*	on application. */
void findShape(void){
		/* find the average magnitude of the readings from the last couple
			 of measurements.*/
	
		/* Decide the sign of the acceleration. Z is supposed to be
		*  the direction we are measuring in.*/
		float sign = (fabs(Zaccel->max) > fabs(Zaccel->min) ? Zaccel->max:Zaccel->min) > 0? 1.0 : -1.0;
		float mag = avgMag;
	
		/*Quantize the values for our LED matrix based on application.*/
    mag = sign*mag;
    if(mag>0.15){
        copyArraytoShape(POS4);
    }
    else if(mag>0.018){
        copyArraytoShape(POS3);
    }
    else if(mag>0.017){
        copyArraytoShape(POS2);
    }
    else if(mag>0.01){
        copyArraytoShape(POS1);
    }
    else if(mag<-0.018){
        copyArraytoShape(NEG3);
    }
    else if(mag<-0.017){
        copyArraytoShape(NEG2);
    }
    else if(mag<-0.01){
        copyArraytoShape(NEG1);
    }
    else{
        copyArraytoShape(Flatline);
    }
    
}

int main (void)
{
	/*Initialize all Acceleration Data structures*/
	Xaccel = initAccelData();
	Yaccel = initAccelData();
	Zaccel = initAccelData();
	
	/*Create a full flatline initial display, 16 columsn
		of flatline columns. */
	displayValues = createCol(Flatline);
	for(int i = 0; i < 15; i++) {
		displayValues = shiftColIn(Flatline, displayValues);
	}
  LED_Initialize();	//Initialize Board LEDs
	ADC_Init();				// Initialize ADC Module for ADC conversion
	checkDefaultAccel(); //Testing purposes... ignore
	I2C_ReleaseBus(); // Force Restart the bus, this is done via GPIO commands
										// and not via the onboard i2c module
	I2C_Init();				// Initialize I2C bus and Uart Serial Communications
	uart_init();
	MatrixBegin();  //Initialize the LED Matrix

  // Set default flatline signal and update the display
  toBuffer(displayValues);
  //Update Display
  updateDisplay();
	//Start Up sequence
	if(startUpSequence() != 1){
		return -1;
	}
  //Calibrate the Accelerometer
		//NOTE: Accelerometer should be still while this happens.
  calibrateAccel();

	while(1){
		//Update the number of counts
		count++;
		//Acceration data is updated every while loop
		float X = readAccelX();
		float Y = readAccelY();
		float Z = readAccelZ();
		addAccelDataX(X);
		addAccelDataY(Y);
		addAccelDataZ(Z);
		float newMag = getMag(X, Y, Z);
		/*If we get an unreasonable value, it's probably noise*/
		if(fabs(newMag)>3) newMag = 0;
		avgMag = (avgMag*((float)count-1.0)+newMag)/(float)count;
		//Every 500 while loop iterations, update display
			//This provides a reasonable frame-rate
		if(count==500)	{
			findShape(); //Update the current column that should be passed in
			//Shift in the new column
			displayValues = shiftColIn(currentShape, displayValues);
			//Destroy the last column to maintain 16 columns
			displayValues = destroyCol(displayValues);

			// Update the LED Matrix buffer values and display the new signal
			toBuffer(displayValues);
			if(updateDisplay()!=1)	{
				return -1;
			}

			//clears the accelerometer data for the next run
			clearAccelData();
			//reset the count value for the next run
			count = 0;
			avgMag = 0;
		}
	}
}