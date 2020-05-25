-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
IHEART-EMBEDDED: HAMILTON LEE AND ZOLTAN CSAKI
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
This file contains the entire code-base for
the iHeart-Embedded final project. Code is intended
to be run on Keil MircoVision IDE. Code organization
is pretty standard:

TremorDetector.c -> Contains main function and main
			loop for program logic.
TremorDetector.h -> Contains variable definitions used in
			main program display cycles
utils.c & utils.h -> Utility functions for the MK64F (same as
			the functions used in the previous labs)
3140_i2c.c, 3140_i2c.h, 3140_serial.c & 3140_serial.h -> Used for exactly 
			how they are named. Provided and refereneced in readme.md file
ADC.c & ADC.h -> The C file contains all code necessary fro ADC conversion
			for our accelerometer. The header file contains 
			pin definitions that are used to do the readings
			along with other constants/
LEDMat.c & LEDMat.h -> contains all definitions for pins and constants along
			with functions to interface with the ADAFruit LED
			Matrix.
matrixCol.c & matrixCol.h -> contains all functions relating to columnwise
			animation of the LED Matrix along with functions to
			convert data to a form readible by the LED matrix
			through I2C.
shared_structs.h -> contains struct definitions for accelerometer data and linked
			list for storing column data used for animating the matrix


You should be able to create a new project through the standard procedure on
microVision and import all these files. Note that when setting up the project,
you may have to include i2c, gpio, in the project setup window along
with all necessary definitions that are needed for i2c and gpio. After project is
created, you have to have a preprocessor definition of CPU_MK64FN1M0VLL12.
change the heap size to 1000. The startup_MK64F12.s file, all instances of
I2C0_IRQHandler need to be changed to I2C0_DriverIRQHandler. 

If those steps are followed, all that needs to be done is to import these files and
the program should run.