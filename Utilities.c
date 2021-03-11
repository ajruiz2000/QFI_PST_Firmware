//Utilities.c  

#include <plib.h>
#include "Utilities.h"
#include "Main.h"


#define TOGGLES_PER_SEC			1000						//sets Wait 'rate'
#define CORE_TICK_RATE	       (SYS_FREQ/2/TOGGLES_PER_SEC)	//sets core count rate SYS_FREQ defined in main.h
#define FOR_LOOP_COUNT			CORE_TICK_RATE/40			//20 core counts per for loop

//coupled with interrupt on CoreTimer
void Wait(int ms)
{
	int i;
	int count = ms*FOR_LOOP_COUNT;
	for(i=0; i<count; i++)
	{;}
}

