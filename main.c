/********************************************************************
 FileName:     main.c
Modified from MicroChip USB example
********************************************************************/
/*********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Received Code from OptoMetrix added Bootloader - Initial release

  1.01   Added 2ms delay for digital modulation in - fixes debouncing

  1.02  Enabled A/D Vref = 2.5V - 9/15/14

********************************************************************/




/** INCLUDES *******************************************************/
#include <plib.h>
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "USB/usb_device.h"
#include "USB/usb.h"
#include "USB/usb_function_cdc.h"
//#include "HardwareProfile.h"
#include "D32.h"
#include "Main.h"
#include "LCD.h"
#include "Ports.h"
#include "Utilities.h"

/** CONFIGURATION **************************************************/

// NOTE THAT WHEN WE USE THE BOOTLOADER, NO CONFIGURATION IS NECESSARY
// THE BOOTLOADER PROJECT ACTUALLY CONTROLS ALL OF OUR CONFIG BITS

/** V A R I A B L E S ********************************************************/
#pragma udata

/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void USBDeviceTasks(void);	//must define here or USB does not work??

#pragma config FPLLODIV=DIV_1, UPLLEN=ON, UPLLIDIV=DIV_2, FPLLMUL=MUL_20, FPLLIDIV=DIV_2

#pragma config FWDTEN=OFF, WDTPS=PS1, FCKSM=CSDCMD, FPBDIV=DIV_1, OSCIOFNC=OFF, POSCMOD=HS, IESO=OFF, FSOSCEN=ON, FNOSC=PRIPLL

#pragma config CP=OFF, BWP=OFF, ICESEL=ICS_PGx2

/** DECLARATIONS ***************************************************/
#pragma code

/******************************************************************************
 * Function:        void main(void)
 *A simple IO polling loop - most the action takes place in interrupts
 *****************************************************************************/
int main(void)
{   

	Wait(1000); 			//let power voltages settle
    InitializeSystem();
   
    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif

    while(1)
    {
        #if defined(USB_POLLING)
		// Check bus status and service USB interrupts.
        USBDeviceTasks(); // Interrupt or polling method.  
        #endif
    				  
        ProcessIO();  //handles IO from USB      
    }//end while
}//end main



/********************************************************************
Initializes all modules
 *******************************************************************/
static void InitializeSystem(void)
{
    //Order is important here
    
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h - used to sense USB power - see full description
    #endif
    
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;	// See HardwareProfile.h - see full description
    #endif

	SYSTEMConfigWaitStatesAndPB(80000000L); // Configure the proper PB frequency and the number of wait states
	CheKseg0CacheOn();	// Enable the cache for the best performance
 	mJTAGPortEnable(0); //turns off JTAG comm
	PMCONbits.ON = 0; // not sure
   	InitPorts();
    USBInit();			//our USB variables
    INTEnableSystemMultiVectoredInt();
    InitLCD();
    WriteStrLCD("Test");
    InitID();
    InitDataCapture();
    InitCamera();
    InitExtIO();
    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    					//variables to known states.
}//end InitializeSystem


/******************************************************************************
 * Function:        void _USB1Interrupt(void)

 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
    #if !defined(self_powered)
        if(U1OTGIRbits.ACTVIF)
        {
            IEC5bits.USB1IE = 0;
            U1OTGIEbits.ACTVIE = 0;
            IFS5bits.USB1IF = 0;
        
            //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
            USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
            //USBSuspendControl = 0;
        }
    #endif
}
#endif

/** EOF main.c *************************************************/
