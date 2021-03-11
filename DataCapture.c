/*DataCapture.c */

#include <plib.h>
#include <p32xxxx.h>
#include "DataCapture.h"
#include "LCD.h"
#include "Ports.h"
#include "ID.h"
#include <string.h>
#include <stdio.h>

float oldVolts = -1;  //stops screen update flicker


//User Timer1 interupt to update voltage display
void __ISR( _TIMER_1_VECTOR, ipl5 ) T1InterruptServiceRoutine()
{
	INTClearFlag( INT_T1 );
	DataCapture();			//on timer interrupt capture ADC and display
}

//Initializes ADC and timer interrupt, must call at beginning of program
char InitDataCapture(void)
{
	//Setup ADC
	CloseADC10();			// ensure the ADC is off before setting the configuration
	// configure and enable the ADC
	AD1PCFG = 0xFFFB;		//
	AD1CON1 = 0x0000;		//Sets into manual mode and UInt16 data format
	AD1CON2bits.VCFG = 0b011; //External Vref+
	AD1CHS = 0x00050000;	//Needed or relic?
	AD1CSSL = 0;			//No scanning allowed
	AD1CON3 = 0x0002;		//System clock with no divisor
	AD1CON1CLR = 0x8000;	//turn off A/D to change channel
	AD1CHS = kVoltAD1CHS;	// set ADC channel - defined in Ports.h
	
	//Turn on update timer
	OpenTimer1(T1_ON |T1_PS_1_256, 0xFFFF);
	ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_5);
	return 0;
}

//Runs a data capture and display, normally only used in timer interrupt
char DataCapture(void)
{
	char sVolts[HLCD+1];			//string for displaying voltage
	float newVolts;					//new voltage value
	int j;

	AD1CON1SET = 0x8000;			//turn on A/D
	AD1CON1SET = 0x0002;			//turn on sampling
	for(j=0; j<20; j++); 			//wait a while for settling
	AD1CON1CLR = 0x0002;			//turn off sampling - starts conversion
	while(!(AD1CON1 & 0x0001));		//wait for conversion finished
	newVolts = ((float)ADC1BUF0) / kFullScaleCount; //read data and scale
	
	if(strcmp(voltDisplay, kIDError) == 0)
	{WriteStrLCD(voltDisplay);}
	else
	{
        newVolts = newVolts * voltScaling;
        if (newVolts != oldVolts)		//reduces screen flicker
        {
            sprintf(sVolts, voltDisplay, newVolts); //format data
			WriteStrLCD(sVolts);						//write to LCD
			oldVolts = newVolts;
		}

	}
	return 0;
}

char DataCaptureStop(void)
{
    ConfigIntTimer1(T1_INT_OFF | T1_INT_PRIOR_5);
    WriteStrLCD("Paused");
}
//Stop data capture interrupt - causes problems with frame sync
char DataCaptureStart(void)
{
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_5);
    WriteStrLCD("Running");
}
	



