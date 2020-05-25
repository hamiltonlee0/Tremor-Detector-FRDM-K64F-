/**
 * @file LEDMat.c
 * @author Hamilton Lee & Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file holds fuctions for a fully functional driver for
 * the Adafruit 8x16 Matrix ADAFRUIT 1.2' LED Matrix.
 * 
 * References: https://github.com/adafruit/Adafruit_LED_Backpack
 * 
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include "LEDMat.h"

/* Function to initialize LED matrix*/

/*PINS USED FOR i2C:
PTE24 is SCLK
PTE25 is SDA
*/

/*8x16 Matrix 
For ADAFRUIT 1.2' LED Matrix: https://www.adafruit.com/product/2035
Driver: HT16K33
VCC: 5V 
*/

/*Short delay*/
void displayDelay(void)	{
	int j;
	for(j=0; j<3000000; j++);
}

/*Initialize the LED matrix*/
int MatrixBegin(){
	/*Turn on the interal oscillator on the board*/
	if(I2C_WriteReg1(LED_addr, Start_Osc)!=1)	{ /*0x70 is the address of device, ,0x21 turns on the oscillator*/
		return -1; //I2C has failed
	}
	/*Set default blinking to NO*/
	if(blinkRate(0)!=1)	{
		return -1;
	}
	/*Set addressing to row addressing rather than keyscan*/
	if(I2C_WriteReg1(LED_addr, 0xA0)!=1){
		return -1;
	}
	/*Set default brightness of LED to low*/
	if(setBrightness(LOW_MINUS)!=1){
		return -1;
	}
	/*Clear the display*/
	if(clearDisplay()!=1){
		return -1;
	}
	return 1;
	
}

/* Function to set blink rate of LED matrix*/
int blinkRate(uint8_t b) {
	if(b > 3) b=0; /*Turn off if not sure*/
	/*Set the blink rate for the LED Matrix*/
	if(I2C_WriteReg1(LED_addr, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1)) !=1 ){
		return -1;
	}
	return 1;
}

/* Startup sequence for the LED Matrix*/
int startUpSequence(void)	{
	if(allOn() != 1)	{
		return -1;
	}
	displayDelay();
	if(clearDisplay() != 1)	{
		return -1;
	}
	displayDelay();
	for(int i = 0; i<16; i++)	{
		displayBuffer[i] = hello[i];
	}
	if(updateDisplay() != 1)	{
		return -1;
	}
	displayDelay();
	displayDelay();
	displayDelay();
	if(clearDisplay() != 1)	{
		return -1;
	}	
	return 1;
}

/*Set the brightness of the LED matrix*/
int setBrightness(uint8_t b) {
  if (b > 15) b = 15;

  if(I2C_WriteReg1(LED_addr, HT16K33_CMD_BRIGHTNESS | b)!=1){
	  return -1;
  }
  return 1; 
}

/*Clears LED Matrix Display*/
int clearDisplay(void){
	for (uint8_t i=0; i<16; i++) {
		/*i represents the row address, 0x00 writes all 8 LEDs in the row to 0*/
		/*i from even are the left LED backpack's rows*/
		/*i from odd are the right LED backpack's rows*/
		/*All are set to 0*/
		if(I2C_WriteReg(LED_addr, i, 0x00)!=1){
			return -1;
		}
  }
	return 1;
}

/*Turns on ALL LEDs on matrix*/
int allOn(void)	{
	/*0xFF means all LEDs in row are ON*/
	/*Write 0xFF to all rows*/
	for	(uint8_t i=0; i<16; i++)	{
		if(I2C_WriteReg(LED_addr, i, 0xFF)!=1){
			return -1;
		}
	}
	return 1;
}

/*Displays all the values in the current display buffer*/
int updateDisplay(void)	{
	for(uint8_t i=0; i<16; i++) {
		if(I2C_WriteReg(LED_addr, i, displayBuffer[i])!=1){
			return -1;
		}
	}
	return 1;
}

/*Clears the display buffer, all 0x00*/
void clearBuffer(void)	{
	for(int i = 0; i<16; i++)	{
		displayBuffer[i] = 0x00;
	}
}

/*Set a buffer value manually*/
/*Developer used for testing,
	Not for use!*/
void setBuffer(uint8_t val, int index){
	displayBuffer[index]=val;
}

/* 
*	Next two functions updates the current buffer based on tlist of columns. 
*
*	This function non-helper function clears the buffer then calls the helper function 
*	to recursively iterates through the input linked  matCol, and then sets the buffer 
*	according to the  elements of column. Uses bitshifting to convert values stored in 
*	columns to the memory structre of the matrix which is stored in rows.
*/
void toBuffer_help(matCol* matrixCol, char curr_col){
	//if the column is on the first LED matrix
	if(curr_col<=7){
		for(int i=0; i<8; i=i+1){
			displayBuffer[2*i] =  displayBuffer[2*i] + (matrixCol->col[i] << (curr_col));
		}
	}
	//if the column is on the second LED matrix
	else if(curr_col < 16){
		for(int i=0; i<8;i=i+1){
			displayBuffer[2*i+1] = displayBuffer[2*i+1] + (matrixCol->col[i] << (curr_col-8));
		}
	}
	/*Ignore all columns that are greater than or equal to 16*/
	else{
		return;
	}
	/*Base case if there are no more columns*/
	if(matrixCol->next==NULL){
		return;
	}
	/*Recursively iterate*/
	else{
		toBuffer_help(matrixCol->next, curr_col+1);
	}
}

/*Writes a linked list of columns into the proper buffer format using the above helper*/
void toBuffer(matCol* matrixCol){
	clearBuffer();
	if(matrixCol == NULL)	{
		return;
	}
	toBuffer_help(matrixCol, 0);
}


