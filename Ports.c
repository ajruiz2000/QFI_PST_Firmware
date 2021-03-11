/* Ports.c*/


//#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "Ports.h"

#define kMAX_PORTS				(7)

volatile unsigned int * const ROM LATPtr[kMAX_PORTS] = 
{
	&LATA, 
	&LATB, 
	&LATC, 
	&LATD, 
	&LATE, 
	&LATF, 
	&LATG
};
volatile unsigned int * const ROM TRISPtr[kMAX_PORTS] = 
{
	&TRISA, 
	&TRISB, 
	&TRISC, 
	&TRISD, 
	&TRISE,
	&TRISF, 
	&TRISG
};
volatile unsigned int * const ROM ODCPtr[kMAX_PORTS] = 
{
	&ODCA, 
	&ODCB, 
	&ODCC, 
	&ODCD, 
	&ODCE, 
	&ODCF, 
	&ODCG
};


//Initializes all port to 'benign' state at start up
//Later modules will over ride these settings for ports accually used
//So call this init first....
void InitPorts()
{
	int i;
	// Loop through each I/O register, setting them all to digital inputs
	// and making none of them open drain and turning off all pullups and 
	// setting all of the latches to zero. We have PORTA through PORTG on
	// this chip. That's 7 total.

	for (i = 0; i < 7; i++)
	{
		*LATPtr[i] = 0x0000;
		*TRISPtr[i] = 0xFFFF;
		*ODCPtr[i] = 0x0000;
	}
	
	
}

