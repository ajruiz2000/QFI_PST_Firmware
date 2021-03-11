//Camera.c


#include <plib.h>
#include <p32xxxx.h>
#include "Utilities.h"
#include "DataCapture.h"
#include "Main.h"
#include "Ports.h"
#include "Camera.h"

//testing
//Setup camera triggering, main work is in interrupt
void InitCamera()
{
    //setup initial camera trigger port as input
    PORTSetPinsDigitalIn(kCamTrigPort);
    //setup camera trigger IO control line
    PORTSetPinsDigitalOut(kCamTrigDirPort);
    //setup device control line
    PORTSetPinsDigitalOut(kDUTLevelPort);
    PORTSetBits(kDUTLevelPort);	//Set-Enable needs DUT high to work 
    //setup user out line
    PORTSetPinsDigitalOut(kUserOutPort);
    //setup interrupt condition
    ConfigINT1(RISING_EDGE_INT);
    //setup priorities and clear flags
    INTSetVectorPriority( INT_EXTERNAL_1_VECTOR, INT_PRIORITY_LEVEL_2 );
    INTSetVectorSubPriority( INT_EXTERNAL_1_VECTOR, INT_SUB_PRIORITY_LEVEL_1 );
	INTClearFlag( INT_INT1 );
	//set initial framing conditions
	camCount = 0;
	camCountNum = 2;
	movieStartNum = 0;
	dutPulseOn = FALSE;
	movieOn = FALSE;
	//testing setup
	digitalModeOn = TRUE;
}

//Camera trigger control INT1
void __ISR( _EXTERNAL_1_VECTOR, ipl2 ) Int1InterruptServiceRoutine()
{
    int i;
	//wait needed for multi-bounce?
	INTClearFlag( INT_INT1 );
        //PORTToggleBits(kUserOutPort);
//        for(i=0; i<10; i++)
//        {i=i*i;}
//        PORTClearBits(kUserOutPort);

	if(dutPulseOn)	//flips DUT on/off at appropriate frame cycle
	{
	    if(camCount >= camCountNum)
	    {
		    camCount = 1;
		    PORTToggleBits(kDUTLevelPort);
		}
		else
		{camCount++;}
	}
	else if(movieOn) //flips DUT on and off at appropriate frames
	{
		camCount++;
		if(camCount == movieStartNum)
		{PORTToggleBits(kDUTLevelPort);}
		if(camCount >= camCountNum)
		{
			PORTClearBits(kDUTLevelPort);
			movieOn = FALSE;
			INTEnable( INT_INT1, INT_DISABLED );	//turns off interrupt at end
		}
	}
	else if(digitalModeOn)
	{
		camCount++;	//used for frame delay
		
	}
}


