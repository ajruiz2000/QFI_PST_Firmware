// 
// Version 1.2  03/03/09  BPS : Fixed problem with boot_software_key_sec so it will work reliably with bootloader (forgot leading .)

#include <stdlib.h>
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "USB/usb_device.h"
#include "USB/usb.h"
#include "USB/usb_function_cdc.h"
#include "HardwareProfile.h"
#include "D32.h"
#include "Errors.h"
#include "Main.h"
#include "Utilities.h"


#define bitset(var,bitno) ((var) |= (1 << (bitno)))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))
#define bittst(var,bitno) (((var) & (1 << (bitno)))?1:0)
#define bitinvert(var,bitno) ((var) ^ (1 << (bitno)))

#define kTX_BUF_SIZE 			(64)			// In bytes
#define kRX_BUF_SIZE			(64)			// In bytes

#define kUSART_TX_BUF_SIZE		(64)			// In bytes
#define kUSART_RX_BUF_SIZE		(64)			// In bytes

#define kMAX_PORTS				(7)
#define k$A_TO					8000000			//sets a time out count for $A command


// Enum for extract_num() function parameter
typedef enum {
	 kCHAR
	,kUCHAR
	,kINT
	,kUINT
	,kASCII_CHAR
	,kUCASE_ASCII_CHAR
} ExtractType;

typedef enum {
	 kEXTRACT_OK = 0
	,kEXTRACT_PARAMETER_OUTSIDE_LIMIT
	,kEXTRACT_COMMA_MISSING
	,kEXTRACT_MISSING_PARAMETER
	,kEXTRACT_INVALID_TYPE
} ExtractReturnType;

#define kREQUIRED	FALSE
#define kOPTIONAL	TRUE

#define advance_RX_buf_out()						\
{ 													\
	g_RX_buf_out++;									\
	if (kRX_BUF_SIZE == g_RX_buf_out)				\
	{												\
		g_RX_buf_out = 0;							\
	}												\
}

#define kISR_FIFO_A_DEPTH		3
#define kISR_FIFO_D_DEPTH		3
#define kCR						0x0D
#define kLF						0x0A
#define kBS						0x08

// defines for the error_byte byte - each bit has a meaning
#define kERROR_BYTE_TX_BUF_OVERRUN			2
#define kERROR_BYTE_RX_BUFFER_OVERRUN		3
#define kERROR_BYTE_MISSING_PARAMETER		4
#define kERROR_BYTE_PRINTED_ERROR			5			// We've already printed out an error
#define kERROR_BYTE_PARAMETER_OUTSIDE_LIMIT	6
#define kERROR_BYTE_EXTRA_CHARACTERS 		7
#define kERROR_BYTE_UNKNOWN_COMMAND			8			// Part of command parser, not error handler

// Let compile time pre-processor calculate the CORE_TICK_PERIOD
//#define SYS_FREQ 				(80000000L)
//#define TOGGLES_PER_SEC			1000
//#define CORE_TICK_RATE	       (SYS_FREQ/2/TOGGLES_PER_SEC)

static char USB_In_Buffer[64];
void BlinkUSBStatus(void);

// Local variables for this file (statics)
static WORD old_swUser;
static WORD old_swProgram;

// This byte has each of its bits used as a seperate error flag
static BYTE error_byte;

// ROM strings
const char st_OK[] = {"OK\r\n"};
const char st_LFCR[] = {"\r\n"};
const char st_version[] = {"\nPST Firmware V1.00\r\n"};

const char ErrorStrings[8][40] = 
{
	"!0 \r\n",								// Unused as of yet
	"!1 \r\n",								// Unused as of yet
	"!2 Err: TX Buffer overrun\r\n",		// kERROR_BYTE_TX_BUF_OVERRUN
	"!3 Err: RX Buffer overrun\r\n",		// kERROR_BYTE_RX_BUFFER_OVERRUN
	"!4 Err: Missing parameter(s)\r\n",		// kERROR_BYTE_MISSING_PARAMETER
	"",										// kERROR_BYTE_PRINTED_ERROR (something already printed)
	"!6 Err: Invalid paramter value\r\n",	// kERROR_BYTE_PARAMETER_OUTSIDE_LIMIT
	"!7 Err: Extra parmater\r\n"			// kERROR_BYTE_EXTRA_CHARACTERS
};

// USB Transmit buffer for packets (back to PC)
unsigned char g_TX_buf[kTX_BUF_SIZE];
// USB Receiving buffer for commands as they come from PC
unsigned char g_RX_buf[kRX_BUF_SIZE];

// USART Receiving buffer for data coming from the USART
unsigned char g_USART_RX_buf[kUSART_RX_BUF_SIZE];

// USART Transmit buffer for data going to the USART
unsigned char g_USART_TX_buf[kUSART_TX_BUF_SIZE];

// Pointers to USB transmit (back to PC) buffer
unsigned char g_TX_buf_in;
unsigned char g_TX_buf_out;
unsigned char g_TX_buf_length;

// Pointers to USB receive (from PC) buffer
unsigned char g_RX_buf_in;
unsigned char g_RX_buf_out;

// In and out pointers to our USART input buffer
unsigned char g_USART_RX_buf_in;
unsigned char g_USART_RX_buf_out;

// In and out pointers to our USART output buffer
unsigned char g_USART_TX_buf_in;
unsigned char g_USART_TX_buf_out;
char	numBytes;

// Normally set to TRUE. Able to set FALSE to not send "OK" message after packet recepetion
BOOL	g_ack_enable;

// Normally set to TRUE. Set to false to disable echoing of all data sent to UBW
BOOL	g_echo_enable = FALSE;

// Used in T1 command to time the LEDs. In ms.
//volatile unsigned int msTimer;

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

unsigned int __attribute__((section(".boot_software_key_sec,\"aw\",@nobits#"))) SoftwareKey;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void parse_packet (void);		// Take a full packet and dispatch it to the right function
ExtractReturnType extract_number(ExtractType Type, void * ReturnValue, unsigned char Required); 								// Pull a number paramter out of the packet
signed char extract_digit (signed long * acc, unsigned char digits); // Pull a character out of the packet
void PrintErrors (void);		// Prints out any errors in error_byte

//void parse_General_packet (void);		// V for printing version
//void parse_Athena_packet (void);		// A for Athena

void parse_ST_packet (void); //Turn into echo???


void check_and_send_TX_data (void); // See if there is any data to send to PC, and if so, do it
void PrintAck (void);			// Print "OK" after packet is parsed
void _mon_putc (char c);		// Our USB based stream character printer
unsigned char CheckLatchingInput (unsigned char PortIndex, unsigned char LatchingClearMask); // Handles the latching of inputs

typedef struct
{
	unsigned char Name1;
	unsigned char Name2;
	void (*CommandPtr)(void);
} CommandListType;

const CommandListType CommandList[] = 
{
	{'$','0',	&parse_General_packet},
//	{'#','0',   &parse_Athena_packet},
	{'S','T',	&parse_ST_packet}, // turn into echo??
	{0x00,0x00,	NULL}
};

char readStr[12];

//not sure how this is used???
//void __ISR(_CORE_TIMER_VECTOR, ipl2) CoreTimerHandler(void)
//{
//    // clear the interrupt flag
//    mCTClearIntFlag();
//	if (msTimer)
//	{msTimer--;}
//    // update the period
//    UpdateCoreTimer(CORE_TICK_RATE);
//}




/** D E C L A R A T I O N S **************************************************/

/*************parse_XXX_packet functions************************/
/*************Please place in alphabetical order **************/


void parse_ST_packet(void)
{
	// Print out the normal version string
	unsigned char Test; 

	if (kEXTRACT_OK == extract_number (kUCASE_ASCII_CHAR, (void*) &Test, kREQUIRED))
		{
		printf ((const char *)st_LFCR);
		printf ("You sent ");
		printf ("%c", Test);
		printf ((const char *)st_LFCR);
		//printf ((const char *)st_version);		
		}
}

// All we do here is just print out our version number
void parse_General_packet(void)
{
	int i;
	uint32_t dataOffSet;
	int error;
	char command;
	char header[16];
	uint32_t index;
	uint32_t curIndex;
	char numFrames;
	int tOCount;
	int oldCamCount;
	int newCamCount;

	advance_RX_buf_out();
	command = toupper(g_RX_buf[g_RX_buf_out]);
//	command = 'C'; //for testing
		switch (command)
	{	
		case 'V':				//Get Version info
			printf ((const char *)st_version);
			break;

		case 'R':				//Software Reset
			printf("\nRESET\r\n");
			Wait(100);
			//Reset Firmware - just follow the yellow brick road :-)
			SYSKEY = 0xAA996655;
			SYSKEY = 0x556699AA;  //SYSTEMUnlock();
			RSWRSTSET = 1;
			volatile int* p = &RSWRST;
			*p;
			while(1); 
			break;
			
//			case 'P':	//pulse camera and DUT
//			
//			mPORTCSetPinsDigitalOut(BIT_1);
//			PWM2Off();
//			//mPORTCSetPinsDigitalOut(BIT_2);
//			advance_RX_buf_out();
//			numFrames = g_RX_buf[g_RX_buf_out];
//			
//			advance_RX_buf_out();
//			deviceOnOff = g_RX_buf[g_RX_buf_out];
//			
//			advance_RX_buf_out();
//			startDelay = g_RX_buf[g_RX_buf_out];
//			advance_RX_buf_out();
//			startDelay += (g_RX_buf[g_RX_buf_out])<<8;
//			
//			advance_RX_buf_out();
//			frameDelay = g_RX_buf[g_RX_buf_out];
//			advance_RX_buf_out();
//			frameDelay += (g_RX_buf[g_RX_buf_out])<<8;
//			//set device on/off
//			if(deviceOnOff)
//			{mPORTCSetBits(BIT_1);}
//			else
//			{mPORTCClearBits(BIT_1);}
//			Wait(startDelay);
//			PWM2On();
//			Wait(frameDelay);
//			PWM2Off();
////			for(i=0; i<numFrames; i++)
////			{
////				mPORTCClearBits(BIT_2); // make a pulse
////				mPORTCSetBits(BIT_2);
////				;;;;;;;;;;;;;;; //short wait
////				mPORTCClearBits(BIT_2);
////				Wait(frameDelay);
////			}
////			printf("$P\r\n");
//			break;

		case 'S': 							//start DUT pulsing
			advance_RX_buf_out();
			camCountNum = g_RX_buf[g_RX_buf_out];
			camCount = camCountNum-1;
			camPulseOn = TRUE;
			mPORTCClearBits(BIT_1);
			INTClearFlag( INT_INT1 );
			INTEnable( INT_INT1, INT_ENABLED );
			break;
		case 'T':							//terminate DUT pulsing
			camPulseOn = FALSE;
			INTEnable( INT_INT1, INT_DISABLED );
			mPORTCClearBits(BIT_1);
			break;
		case 'A':							//armed? - pulsing cycle just before reset to zero
			if(camPulseOn)
			{
				tOCount = 0;
				oldCamCount = camCount;
				while(mPORTCReadBits(BIT_1) || camCount != camCountNum)
				{
					if(camCount != oldCamCount)	//reset if 'ticking'
					{
						oldCamCount = camCount;
						tOCount = 0;
					}
					if(tOCount > k$A_TO)	//if timeout exceeded, break out of while loop
					{
						printf("Off\r\n");
						break;
					}
					tOCount++;
				} //loop till false
				printf("On\r\n");
			}
			else
			{printf("Off\r\n");}

			break;
		case 'E':						//ending - count just before armed
			if(mPORTCReadBits(BIT_1) || camCount != camCountNum-1)
			{printf("N\r\n");} //if bit on or not at next to last count - not ending
			else
			{printf("Y\r\n");}
			break;
		case 'M':						//starts movie mode
			movieOn = TRUE;
			advance_RX_buf_out();
			if(g_RX_buf[g_RX_buf_out])	//second bit indicates start condition
				{mPORTCSetBits(BIT_1);}
			else
				{mPORTCClearBits(BIT_1);}
			advance_RX_buf_out();
			camCountNum = g_RX_buf[g_RX_buf_out];
			advance_RX_buf_out();
			movieStartNum = g_RX_buf[g_RX_buf_out];
			camCount = -2;
			INTClearFlag( INT_INT1 );
			INTEnable( INT_INT1, INT_ENABLED );
			while(camCount == -2);	//wait till armed
			printf("Go\r\n");
			break;
		case 'O':					//sets device on/off	
			advance_RX_buf_out();
			if(g_RX_buf[g_RX_buf_out]) //second byte inticates on/off
				{mPORTCSetBits(BIT_1);}
			else
				{mPORTCClearBits(BIT_1);}
			printf("Set\r\n");
			break;
		case '*':
			i=0;
			while(g_RX_buf[g_RX_buf_out] != kLF)
			{
				advance_RX_buf_out();
				header[i] = g_RX_buf[g_RX_buf_out];
			}
			printf("Got: %s", header);
			
//			printf("%i", numBytes ); 
//			printf("%i", g_RX_buf[g_RX_buf_out]);
//			printf(" \r\n");
			
			break;
		default:
			printf("\nERROR_UC\r\n");
	}
	//flush buffer
//	for(i=0; i <kMaxWrite-1 ; i++)
//	{
//		if(g_RX_buf[g_RX_buf_out] == '\r')
//		{break;}
//		advance_RX_buf_out();
//	}

	//printf("V_Packet\r\n");
}

/******************************************************************************
 * Function:        void UserInit(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        This routine should take care of all of the demo code
 *                  initialization that is required.
 * Note:            
 *****************************************************************************/
void UserInit(void)
{
	char i;
	char test;

	// Loop through each I/O register, setting them all to digital inputs
	// and making none of them open drain and turning off all pullups and 
	// setting all of the latches to zero. We have PORTA through PORTG on
	// this chip. That's 7 total.
	//desirable?????
	/*
	for (i = 0; i < 7; i++)
	{
		*LATPtr[i] = 0x0000;
		*TRISPtr[i] = 0xFFFF;
		*ODCPtr[i] = 0x0000;
	}

    //Initialize all of the LED pins
	mInitAllLEDs();
*/
	// Start off always using "OK" acknoledge.
	g_ack_enable = TRUE;

	// Start off always echoing all data sent to us
	g_echo_enable = FALSE;

    // Inialize USB TX and RX buffer management
    g_RX_buf_in = 0;
    g_RX_buf_out = 0;
	g_TX_buf_in = 0;
	g_TX_buf_out = 0;
	g_TX_buf_length = 0;

	// And the USART TX and RX buffer management
	g_USART_RX_buf_in = 0;
	g_USART_RX_buf_out = 0;
	g_USART_TX_buf_in = 0;
	g_USART_TX_buf_out = 0;

//	// Open up the core timer at our 1ms rate
//	OpenCoreTimer(CORE_TICK_RATE);
//
//    // set up the core timer interrupt with a prioirty of 2 and zero sub-priority
//	mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_2 | CT_INT_SUB_PRIOR_0));
//
    // enable multi-vector interrupts
	INTEnableSystemMultiVectoredInt();
	//test = UpDownInit();
//	if( NLInit())  //must init Neslabs first
//	{
//		ShutDown();
//		return;
//	}
//	if(AthenaInit())
//	{
//		ShutDown();
//		return;
//	}
}//end UserInit



/********************************************************************
 * Function:        void ProcessIO(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 * Note:            None
 *******************************************************************/
void ProcessIO(void)
{   
	static BOOL in_cr = FALSE;
	static char last_fifo_size;
    unsigned char tst_char;
	unsigned char RXBufInTemp;
	BOOL	got_full_packet = FALSE;
	cdc_rx_len = 0;
	BYTE numBytesRead;
	int i;

    //Blink the LEDs according to the USB device status
    BlinkUSBStatus();

    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	// Pull in some new data if there is new data to pull in
	numBytes = 0;
//	i = 0;
//	while ((numBytesRead = getsUSBUSART(USB_In_Buffer,64)) != 0)
//	{
//		for(cdc_rx_len = 0; cdc_rx_len < numBytesRead; cdc_rx_len++)
//		{
//			readStr[i] = USB_In_Buffer[cdc_rx_len];
//			i++;
//		}
//	}
//	if (i != 0) {printf(readStr);}
//
	if(numBytesRead != 0)
	{
		// Copy data from USB buffer to our buffer
		for(cdc_rx_len = 0; cdc_rx_len < numBytesRead; cdc_rx_len++)
		{
			// Check to see if we are in a CR/LF situation
			tst_char = USB_In_Buffer[cdc_rx_len];
			if (g_echo_enable)
			{
				_mon_putc(tst_char);
				if (kCR == tst_char)
				{
					_mon_putc(kLF);
				}
				if (kBS == tst_char)
				{
					_mon_putc(' ');
					_mon_putc(kBS);
				}
			}
			if (!in_cr && (kCR == tst_char || kLF == tst_char))
			{
				in_cr = TRUE;
				g_RX_buf[g_RX_buf_in] = kCR;
				g_RX_buf_in++;
				// At this point, we know we have a full packet
				// of information from the PC to parse
				got_full_packet = TRUE;
			}
			else if (tst_char != kCR && tst_char != kLF)
			{
				in_cr = FALSE;
//				if (kBS == tst_char)
//				{
//					// Check to see that we're not already at the beginning
//					if (g_RX_buf_in != g_RX_buf_out)
//					{
//						// If we have backspace, then handle that here
//						// Then decriment the input pointer
//						if (g_RX_buf_in > 0)
//						{
//							g_RX_buf_in--;
//						}
//						else
//						{
//							g_RX_buf_in = kRX_BUF_SIZE - 1;
//						}
//					}
//					continue;
//				}
//				else
				{
					// Only add a byte if it is not a CR or LF or BS
					g_RX_buf[g_RX_buf_in] = tst_char;
					g_RX_buf_in++;
				}
			}
			else
			{
				continue;
			}
			// Check for buffer wraparound
			if (kRX_BUF_SIZE == g_RX_buf_in)
			{
				g_RX_buf_in = 0;
			}
			// If we hit the out pointer, then this is bad.
			if (g_RX_buf_in == g_RX_buf_out)
			{
				bitset (error_byte, kERROR_BYTE_RX_BUFFER_OVERRUN);
				break;
			}
			// Now, if we've gotten a full command (user send <CR>) then
			// go call the code that deals with that command, and then
			// keep parsing. (This allows multiple small commands per packet)
			if (got_full_packet)
			{
				parse_packet ();
				got_full_packet = FALSE;
			}
			//PrintErrors ();
		}		
	}

	//PrintErrors();

	// Go send any data that needs sending to PC

	check_and_send_TX_data ();
}

// This routine checks to see if any of the bits in error_byte
// are set, and if so, prints out the corresponding error string.
void PrintErrors (void)
{
	unsigned char Bit;

	// Check for any errors logged in error_byte that need to be sent out
	if (error_byte)
	{
		for (Bit = 0; Bit < 8; Bit++)
		{
			if (bittst (error_byte, Bit))
			{
				printf (ErrorStrings[Bit]);
			}
		}
		error_byte = 0;
	}
}

// This is our replacement for the standard putc routine
// This enables printf() and all related functions to print to
// the USB output (i.e. to the PC) buffer
void _mon_putc (char c)
{
	// Only add chars to USB buffer if it's configured!
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1))
	{
		return;
	}

	// We need to check to see if adding this character will
	// cause us to become overfull.
	// We want to sit and just process USB tasks if our buffer
	// is full.
	if (g_TX_buf_length >= (kTX_BUF_SIZE - 2))
	{
		while (g_TX_buf_length > 0)
		{
			// In this case, we want to puase for a moment, send out these
			// characers to the PC over USB, and then clear out out buffer.
			check_and_send_TX_data();
		}
	}

	// Copy the character into the output buffer
	g_TX_buf[g_TX_buf_in] = c;
	g_TX_buf_in++;
	g_TX_buf_length++;

	// Check for wrap around
	if (kTX_BUF_SIZE == g_TX_buf_in)
	{
		g_TX_buf_in = 0;
	}
	
	// Also check to see if we bumped up against our output pointer
	if (g_TX_buf_in == g_TX_buf_out)
	{
		bitset (error_byte, kERROR_BYTE_TX_BUF_OVERRUN);
		g_TX_buf_in = 0;
		g_TX_buf_out = 0;
		g_TX_buf_length = 0;
	}
	return;
}

// In this function, we check to see it is OK to transmit. If so
// we see if there is any data to transmit to PC. If so, we schedule
// it for sending.
void check_and_send_TX_data (void)
{
	char temp;
	unsigned char i;
	char TempBuf[64];

	// Only send if there's something there to send
	if (g_TX_buf_length != 0)
	{
		// We're going to sit and spin and wait until
		// can transmit
		while (!mUSBUSARTIsTxTrfReady ())
		{
			USBDeviceTasks();
			CDCTxService();
		}

		// Now copy over all of the FIFO bytes into our temp buffer
		for (i = 0; i < g_TX_buf_length; i++)
		{
			TempBuf[i] = g_TX_buf[g_TX_buf_out];
			g_TX_buf_out++;
			if (g_TX_buf_out == kTX_BUF_SIZE)
			{
				g_TX_buf_out = 0;
			}
		}

		putUSBUSART (TempBuf, g_TX_buf_length);
		g_TX_buf_length = 0;
		g_TX_buf_out = g_TX_buf_in;

		USBDeviceTasks();
		CDCTxService();
	}
}


// Look at the new packet, see what command it is, and 
// route it appropriately. We come in knowing that
// our packet is in g_RX_buf[], and that the beginning
// of the packet is at g_RX_buf_out, and the end (CR) is at
// g_RX_buf_in. Note that because of buffer wrapping,
// g_RX_buf_in may be less than g_RX_buf_out.
void parse_packet(void)
{
	unsigned char	CommandNumber = 0;
	unsigned char	CmdName1 = 0;
	unsigned char	CmdName2 = 0;

	// Always grab the first character (which is the first byte of the command)
	CmdName1 = toupper (g_RX_buf[g_RX_buf_out]);
	if (kCR == CmdName1)
	{
		goto parse_packet_end;
	}

	// Now loop through the array of commands, trying to find
	// a match based upon the two (or one) first characters of the command
	while (CommandList[CommandNumber].CommandPtr != NULL && CommandNumber < 250)
	{
		// If the two name characters match, then call the command to do the work
		if (CmdName1 == CommandList[CommandNumber].Name1)
		{
			CommandList[CommandNumber].CommandPtr();
			goto parse_packet_end;
		}
		else
		{
			CommandNumber++;
		}
	}
	
	// If we get here then we did not find a match for our command characters
	if (0 == CmdName2)
	{
		// Send back 'unknown command' error
		printf ("\nERROR_UC\r\n");
	}

	bitset (error_byte, kERROR_BYTE_PRINTED_ERROR);

parse_packet_end:
	// Double check that our output pointer is now at the ending <CR>
	// If it is not, this indicates that there were extra characters that
	// the command parsing routine didn't eat. This would be an error and needs
	// to be reported. (Ignore for Reset command because FIFO pointers get cleared.)
	if (
		(g_RX_buf[g_RX_buf_out] != kCR)
		&& 
		(0 == error_byte)
		&&
		(
			('R' != CmdName1)
			||
			(0 != CmdName2)
		)
	)
	{
		bitset (error_byte, kERROR_BYTE_EXTRA_CHARACTERS);
	}

	// Clean up by skipping over any bytes we haven't eaten
	// This is safe since we parse each packet as we get a <CR>
	// (i.e. g_RX_buf_in doesn't move while we are in this routine)
	g_RX_buf_out = g_RX_buf_in;

	// Always try to print out an OK packet here. If there was an 
	// error, nothing will print out.
//	PrintAck();
}

// Print out the positive acknoledgement that the packet was received
// if we have acks turned on.
void PrintAck(void)
{
	if (g_ack_enable && !error_byte)
	{
		printf ((const char *)st_OK);
	}
}


// Look at the string pointed to by ptr
// There should be a comma where ptr points to upon entry.
// If not, throw a comma error.
// If so, then look for up to three bytes after the
// comma for numbers, and put them all into one
// byte (0-255). If the number is greater than 255, then
// thow a range error.
// Advance the pointer to the byte after the last number
// and return.
ExtractReturnType extract_number(
	ExtractType Type, 
	void * ReturnValue, 
	unsigned char Required
)
{
	signed long Accumulator;
	unsigned char Negative = FALSE;

	// Check to see if we're already at the end
	if (kCR == g_RX_buf[g_RX_buf_out])
	{
		if (0 == Required)
		{
			bitset (error_byte, kERROR_BYTE_MISSING_PARAMETER);
		}
		return (kEXTRACT_MISSING_PARAMETER);
	}

	// Check for comma where ptr points
	if (g_RX_buf[g_RX_buf_out] != ',')
	{
		if (0 == Required)
		{
			printf ((const char *)"!5 Err: Need comma next, found: '%c'\r\n", g_RX_buf[g_RX_buf_out]);
			bitset (error_byte, kERROR_BYTE_PRINTED_ERROR);
		}
		return (kEXTRACT_COMMA_MISSING);
	}

	// Move to the next character
	advance_RX_buf_out ();

	// Check for end of command
	if (kCR == g_RX_buf[g_RX_buf_out])
	{
		if (0 == Required)
		{
			bitset (error_byte, kERROR_BYTE_MISSING_PARAMETER);
		}
		return (kEXTRACT_MISSING_PARAMETER);
	}
	
	// Now check for a sign character if we're not looking for ASCII chars
	if (
		('-' == g_RX_buf[g_RX_buf_out]) 
		&& 
		(
			(kASCII_CHAR != Type)
			&&
			(kUCASE_ASCII_CHAR != Type)
		)
	)
	{
		// It's an error if we see a negative sign on an unsigned value
		if (
			(kUCHAR == Type)
			||
			(kUINT == Type)
		)
		{
			bitset (error_byte, kERROR_BYTE_PARAMETER_OUTSIDE_LIMIT);
			return (kEXTRACT_PARAMETER_OUTSIDE_LIMIT);
		}
		else
		{
			Negative = TRUE;
			// Move to the next character
			advance_RX_buf_out ();
		}
	}

	// If we need to get a digit, go do that
	if (
		(kASCII_CHAR != Type)
		&&
		(kUCASE_ASCII_CHAR != Type)
	)
	{
		extract_digit(&Accumulator, 5);
	}
	else
	{
		// Otherwise just copy the byte
		Accumulator = g_RX_buf[g_RX_buf_out];
	
		// Force uppercase if that's what type we have
		if (kUCASE_ASCII_CHAR == Type)
		{
			Accumulator = toupper (Accumulator);
		}
		
		// Move to the next character
		advance_RX_buf_out ();
	}

	// Handle the negative sign
	if (Negative)
	{
		Accumulator = -Accumulator;
	}

	// Range check the new value
	if (
		(
			kCHAR == Type
			&&
			(
				(Accumulator > 127)
				||
				(Accumulator < -128)
			)
		)
		||
		(
			kUCHAR == Type
			&&
			(
				(Accumulator > 255)
				||
				(Accumulator < 0)
			)
		)
		||
		(
			kINT == Type
			&&
			(
				(Accumulator > 32767)
				||
				(Accumulator < -32768)
			)
		)
		||
		(
			kUINT == Type
			&&
			(
				(Accumulator > 65535)
				||
				(Accumulator < 0)
			)
		)
	)
	{
		bitset (error_byte, kERROR_BYTE_PARAMETER_OUTSIDE_LIMIT);
		return (kEXTRACT_PARAMETER_OUTSIDE_LIMIT);
	}

	// If all went well, then copy the result
	switch (Type)
	{	
		case kCHAR:
			*(signed char *)ReturnValue = (signed char)Accumulator;
			break;
		case kUCHAR:
		case kASCII_CHAR:
		case kUCASE_ASCII_CHAR:
			*(unsigned char *)ReturnValue = (unsigned char)Accumulator;
			break;
		case kINT:
			*(signed int *)ReturnValue = (signed int)Accumulator;
			break;
		case kUINT:
			*(unsigned int *)ReturnValue = (unsigned int)Accumulator;
			break;
		default:
			return (kEXTRACT_INVALID_TYPE);
	}	
	return(kEXTRACT_OK);	
}

// Loop 'digits' number of times, looking at the
// byte in input_buffer index *ptr, and if it is
// a digit, adding it to acc. Take care of 
// powers of ten as well. If you hit a non-numerical
// char, then return FALSE, otherwise return TRUE.
// Store result as you go in *acc.
signed char extract_digit(signed long * acc,	unsigned char digits)
{
	unsigned char val;
	unsigned char digit_cnt;
	
	*acc = 0;

	for (digit_cnt = 0; digit_cnt < digits; digit_cnt++)
	{
		val = g_RX_buf[g_RX_buf_out];
		if ((val >= 48) && (val <= 57))
		{
			*acc = (*acc * 10) + (val - 48);
			// Move to the next character
			advance_RX_buf_out ();
		}
		else
		{
			return (FALSE);
		}
	}
	return (TRUE);
}

// For debugging, this command will spit out a bunch of values.
//void print_status(void)
//{
////	printf( 
////		(const char *)"Status=%i\r\n"
////		,ISR_D_FIFO_length
////	);
//}

/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        BlinkUSBStatus turns on and off LEDs 
 *                  corresponding to the USB device state.
 * Note:            mLED macros can be found in HardwareProfile.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/
void BlinkUSBStatus(void)
{
    static WORD led_count = 0;
    
    if(led_count == 0)
	{led_count = 50000U;}
    led_count--;

    if(USBDeviceState == CONFIGURED_STATE)
    {
        if(led_count == 0)
        {mLED_USB_Toggle();}
    }
}



/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 * Note:            None
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
    wakeUpCall = IORequest;
}
#endif


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckCDCRequest();
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    CDCInitEP();
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 * Note:            Interrupt vs. Polling
 *                 See Original for details
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    USBResumeControl = 1;                // Start RESUME signaling
    
    delay_count = 1800U;                // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}



/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 * PreCondition:    None
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 * Output:          None
 * Side Effects:    None
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 * Note:            See original for additional events not used here
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
//        case EVENT_SET_DESCRIPTOR:
//            USBCBStdSetDscHandler();
//            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
//        case EVENT_SOF:
//            USBCB_SOF_Handler();
//            break;
//        case EVENT_SUSPEND:
//            USBCBSuspend();
//            break;
//        case EVENT_RESUME:
//            USBCBWakeFromSuspend();
//            break;
//        case EVENT_BUS_ERROR:
//            USBCBErrorHandler();
//            break;
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }      
    return TRUE; 
}
