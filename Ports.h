/*Ports.h	
All ports are defined with meaningful psesdo-names in this single document
to avoid conflicts in multiple sections of the code. Where pseudo-names will not work,
e.g. with the LCD ports, those ports are also listed so as to inicate they are used.
In essence, this file captures the port definition of the hardware system. 
*/
#pragma once

#include <plib.h>

#define kIDBits		IOPORT_D, BIT_1 | BIT_2 | BIT_3
#define kIDScalor	2 //only works if kIDBits are contiguous

#define kCamTrigPort	IOPORT_E, BIT_8		//is used as INT1
#define kUserInPort		IOPORT_E, BIT_9		//is used as INT2 in Slave mode
#define kUserOutPort	IOPORT_B, BIT_0		//use in Master mode to signal go
#define kDUTLevelPort	IOPORT_C, BIT_1		//controls DUT high/low
#define kCamTrigDirPort IOPORT_C, BIT_2		//contols in,out for camera triggering - may not use
#define kADCGndPort		IOPORT_A, BIT_9		//is used as VREF-
#define kADCRefPort		IOPORT_A, BIT_10	//is used as VREF+
#define kADCVoltPort	IOPORT_B, BIT_8		//is used as AN8
#define kTestPort		IOPORT_A, BIT_0		//TEST PORT 
#define kUSBBlinkPort	IOPORT_G, BIT_0		//blinks to indicate USB active
#define kVoltAD1CHS		0x00080000 			//correlates ADC port with AD1CHS nomemclature
/*
Parallel ports use for LCD
IOPORT_E, BIT_0
IOPORT_E, BIT_1
IOPORT_E, BIT_2
IOPORT_E, BIT_3
IOPORT_E, BIT_4
IOPORT_E, BIT_5
IOPORT_E, BIT_6
IOPORT_E, BIT_7
IOPORT_D, BIT_4
IOPORT_D, BIT_5
IOPORT_B, BIT_15
*/

/*
Ports used for USB
IOPORT_G, BIT_2
IOPORT_G, BIT_3
IOPORT_F, BIT_3
VBUS
*/

/*
Ports use for PICKit3 interface
IOPORT_B, BIT_6
IOPORT_B, BIT_7
MCLR
*/

//use to initailize all ports into nominal state, e.g. low power
//call this before all other Initialize functions, that will then
//override the specifc ports they use
void InitPorts(void);
