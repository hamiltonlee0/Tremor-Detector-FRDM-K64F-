/**
 * @file shared_structs.c
 * @author Hamilton Lee & Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file contains data structures for a tremor detector and display
 * using the MK64F, ADXL335, and 1.2' Adafruit 8x16 LED matrix.
 */

#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

/*
* This defines a linked list of columns that represents
* each column data in the LED matrix. Each column is length eight
* Each value in the column is either 0x00 to 0xFF representing different
* permutations of LEDs lighting up.
*/
typedef struct columnFrame {
  char col[8]; //Values of the Columns in the matrix
  struct columnFrame *next; //Next Column in the matrix
} matCol;

/*
*Using a Linked List for the accelerometer data
*/
typedef struct accelData {
  float min; //Minimum value of acceleration measured in the last timeframe
  float max; //Maximum value of acceleration measured in the last timeframe
  float avg; //Average value of acceleration measured in the last timeframe
  int number; //Number of measurements taken so far.
} accelData;

#endif
