/* PWM.c */


#include <plib.h>

// Will use default osc mode with WDT off
// Other options are don't care for this example
//
//#pragma config FWDTEN = OFF //what does this do?

#define kTimerPeriod 0x190 //yeilds ~10kHz with current settings
#define kPWMFullFract 1000 //kTimer2Period * kPWMFullFract must not overflow int


//needs to be put into system memory
int PWM1Fract = 800;
int PWM2Fract = 600;
int PWM3Fract = 400;
int PWM4Fract = 200;
//int test;


char PWM1On(void) //check timer on - if not turn on then off
{
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   
			| OC Pin High , S Compare value, Compare value*/
	OpenOC1( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE 
			| OC_LOW_HIGH , kTimerPeriod, 0x100);//(kTimerPeriod*PWM1Fract)/kPWMFullFract );
	return 0;
}

char PWM2On(void)
{
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   
			| OC Pin High , S Compare value, Compare value*/
	OpenOC2( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE 
			| OC_LOW_HIGH , kTimerPeriod, (kTimerPeriod*PWM2Fract)/kPWMFullFract );
	return 0;
}

char PWM3On(void)
{
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   
			| OC Pin High , S Compare value, Compare value*/
	OpenOC3( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE 
			| OC_LOW_HIGH , kTimerPeriod, (kTimerPeriod*PWM3Fract)/kPWMFullFract );
	return 0;
}

char PWM4On(void)
{
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   
			| OC Pin High , S Compare value, Compare value*/
	OpenOC4( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE 
			| OC_LOW_HIGH , kTimerPeriod, (kTimerPeriod*PWM4Fract)/kPWMFullFract );
	return 0;
}

char PWM1Off(void)
{
	CloseOC1();
	return 0;
}

char PWM2Off(void)
{
	CloseOC2();
	return 0;
}

char PWM3Off(void)
{
	CloseOC3();
	return 0;
}

char PWM4Off(void)
{
	CloseOC4();
	return 0;
}

char PWM1Cal(int cal)
{

}

char PWM2Cal(int cal)
{

}

char PWM3Cal(int cal)
{

}

char PWM4Cal(int cal)
{

}

char PWMTimerOn(void)
{
	OpenTimer2(T2_ON, kTimerPeriod);
	return 0;
}
char PWMTimerOff(void)
{
	CloseTimer2();
	return 0;
}

char PWM_On(int unitNum)
{
	char error;
	
	switch(unitNum)
	{
		case 1:
			error = PWM1On();
			break;
		case 2:
			error = PWM2On();
			break;
		case 3:
			error = PWM3On();
			break;
		case 4:
			error = PWM4On();
			break;
		default:
		error = -1;
		}
	return error;
}

char PWM_Off(int unitNum)
{
	char error;
	
	switch(unitNum)
	{
		case 1:
			error = PWM1Off();
			break;
		case 2:
			error = PWM2Off();
			break;
		case 3:
			error = PWM3Off();
			break;
		case 4:
			error = PWM4Off();
			break;
		default:
		error = -1;
		}
	return error;
}



//int PWMOn(void)
//{
//	/* Open Timer2 with Period register value */
//	OpenTimer2(T2_ON, 0x550);
//
//	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value*/
//	OpenOC1( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , 0x550, 0x540 );
//	
//	/*
//	  The expected output looks like the diagram below with approximately 6% duty-cycle
//
//           ||      ||      ||
//	 ______||______||______||______
//
//	*/
//	
//	OpenOC2( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , 0x550, 0x100 );
//	OpenOC3( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , 0x550, 0x549 );
//	OpenOC4( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , 0x550, 0x200 );
//	while(1); /* can have user code here */
//
//	/* Close Output Compare */
//	CloseOC1();
//
//	return (0);
//}


