/**
 * @file matrixCol.c
 * @author Hamilton Lee & Zoltan Csaki
 * @copyright All rights reserved, 2020
 *
 * This file contains function implementations for 
 * matrix Column structure used for columnwise animation
 * of LED matrix.
 * 
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include "matrixCol.h"
#include <stdlib.h>
#include <string.h>

/*Initializes a linked column list*/
matCol *createCol(char vals[8]){
    matCol *newCol = malloc(sizeof(matCol));
    /*Malloc Failed*/
    if(newCol == NULL){
        return NULL;
    }
    //Copy vals array to col attribute
    for(int i = 0; i < 8; i++)  {
        newCol->col[i] = vals[i];
    }
    newCol->next=NULL;
    return newCol;
}

/*Destorys columns at the end of the linked list*/
matCol *destroyCol(matCol	*matrixCol){
    /*If nothing is passed in*/
	if(matrixCol == NULL)	{
			return NULL;
	}
    /*If this is the only thing in the linked list*/
	if(matrixCol->next == NULL){
			free(matrixCol); //Free the memory
			return NULL;
	}
    /*Remove the last one and set the previous one's next to NULL*/
    matCol *curr = matrixCol;
    matCol *prev = NULL;
    while(curr->next!=NULL){
        prev = curr;
        curr = curr->next;
    }
    prev->next=NULL;
    free(curr); //free the memory
		return matrixCol;
}

/*Shift in a column to a linked list of columns*/
matCol *shiftColIn(char vals [8], matCol *head){
    //creates a new col based on vals
    matCol *newCol = createCol(vals);
    //If Malloc Failed
    if(newCol == NULL){
        return NULL;
    }
    //sets the previous head to be the next in linked list 
    newCol->next=head;
    return newCol;
}