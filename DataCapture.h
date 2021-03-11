/*
DataCapture.h
Uses Timer1 to periodically poll ADC
ADC result is rescaled to represent DUT voltage
Result is displayed on LCD
Rescaling descriptions are found in ID.h
LCD interface is found in LCD.h
*/

#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <plib.h>

#define kADCRef			2.50		//ADC reference voltage
#define kFullScaleCount 1000.00 	//ADC resistor divider is set so full scale voltage produces count of 1000

//Initializes ADC and timer interrupt, must call at beginning of program
char InitDataCapture(void);
//Runs a data capture and display, normally only used in timer interrupt
char DataCapture(void);
//Stop data capture interrupt - causes problems with frame sync
char DataCaptureStop(void);
//Start data capture interrupt
char DataCaptureStart(void);



