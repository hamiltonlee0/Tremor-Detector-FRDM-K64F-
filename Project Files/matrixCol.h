/**
 * @file matrixCol.c
 * @author Hamilton Lee & Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file contains function declarations for matrix Column structure used for LED matrix
 * 
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include "shared_structs.h"

#ifndef __MatrixCol__
#define __MatrixCol__

matCol *createCol(char vals[8]); /*initializes a linked column list*/
matCol *destroyCol(matCol  *matrixCol); /*Free the memory for a matrix column and destroy last column*/
matCol *shiftColIn(char vals[8], matCol *head); /*Shift in a column to a linked list of columns*/
#endif

