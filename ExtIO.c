/*
ExtIO.c
*/
#include <plib.h>
#include <p32xxxx.h>
#include "Utilities.h"
#include "DataCapture.h"
#include "Main.h"
#include "Ports.h"
#include "ExtIO.h"
#include <stdio.h>
#include <stdlib.h>



//Initalizes user IO - must call early in program
//Real work is in Interrput
void InitExtIO()
{
	PORTSetPinsDigitalIn(kUserInPort);
	PORTSetPinsDigitalOut(kUserOutPort);
	PORTClearBits(kUserOutPort);
	STOP_USER_INT;
	userInEdge = USER_INT_RISING;
    ConfigINT2(userInEdge);    
    INTSetVectorPriority( INT_EXTERNAL_2_VECTOR, INT_PRIORITY_LEVEL_3 );
    INTSetVectorSubPriority( INT_EXTERNAL_2_VECTOR, INT_SUB_PRIORITY_LEVEL_1 );
	INTClearFlag( INT_INT2 );
	//INTEnable( INT_INT2, INT_ENABLED );
	masterSlaveState = MASTER;
	simplexDuplexState = SIMPLEX;
	userInCond = LEVEL;
	userOutCond = LEVEL;
}

// User trigger control on INT2
//Simply sends out USB Ack when trigger occurs
void __ISR( _EXTERNAL_2_VECTOR, ipl3 ) Int2InterruptServiceRoutine()
{
	INTClearFlag( INT_INT2 );
	Wait(2);
	//turn off int - ignore extra pulses if any
	STOP_USER_INT;
	if(userInCond == LEVEL)
	{
		if(userInEdge == (USER_INT_RISING))
		{
		userInEdge = USER_INT_FALLING;
         stdout->_flag |= _IOBINARY;
		printf("AckR\r\n");
		}
		else
		{
		userInEdge = USER_INT_RISING;
         stdout->_flag |= _IOBINARY;
		printf("AckF\r\n");
		}
		ConfigINT2(userInEdge);
	}
	else
	{
        stdout->_flag |= _IOBINARY; 
        printf("Ack\r\n");}//testing
	START_USER_INT;
}


