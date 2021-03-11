// ID.c

#include "Ports.h"	
#include "ID.h"
#include <string.h>

//Initializes deviceID with measured value
//Should be called at beginning of program
void InitID(void)
{
	PORTSetPinsDigitalIn(kIDBits);				//set DIO as input
	deviceID = PORTReadBits(kIDBits)/kIDScalor;	//read DIO and scale to 0-7

//Add cases here as they are defined
//voltScaling is nominally the full scale voltage range
//voltDisplay format is set to fit in 8 characters and
//reflect 1 part in 1000 precision, e.g. 1000 volt unit - "$6.0f V"
	switch (deviceID)				//deviceID is defined in ID.h
        {
        case 0:          	// errror - 0 indicates open key
            strcpy(voltDisplay, kIDError);
            break;
	case 1: 	// 40V Unit
            strcpy(voltDisplay, "%6.2f V");
            voltScaling = 40.0;
            break;
        case 2:	//20 volt unit
            strcpy(voltDisplay, "%6.2f V");
            voltScaling = 20.0;
            break;
        case 4:	//Added for Demo unit middle indicator is non functional
            strcpy(voltDisplay, "%6.2f V");
            voltScaling = 20.0;
            break;
        //add cases as new units are brought into production
        default:            // print character
            strcpy(voltDisplay, kIDError);
            break;
		}

}
	

