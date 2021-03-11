//Interrupts.c

#include <plib.h>
#include <p32xxxx.h>
#include "Interrupts.h"
#include "Utilities.h"
#include "DataCapture.h"
#include "Main.h"

void InitInterrupts()
{
	

	//Initialize Timer1 for DataCapture
	
//	//Initialize CoreTimer for millisecond 'clock'
//	// Open up the core timer at our 1ms rate
//	OpenCoreTimer(CORE_TICK_RATE);
//    // set up the core timer interrupt with a prioirty of 2 and zero sub-priority
//	mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_2 | CT_INT_SUB_PRIOR_0));
//	
		INTEnableSystemMultiVectoredInt();
}

//void __ISR(_CORE_TIMER_VECTOR, ipl2) CoreTimerHandler(void)
//{
//    // clear the interrupt flag
//    mCTClearIntFlag();
//	if (msTimer)
//	{msTimer--;}
//    // update the period
//    UpdateCoreTimer(CORE_TICK_RATE);
//}
//


