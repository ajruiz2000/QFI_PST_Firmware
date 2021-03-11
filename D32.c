// D32.c handles buffering out USB data then parsing commands to set machine state


#include <string.h>
#include <ctype.h>
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "USB/usb_device.h"
#include "USB/usb.h"
#include "USB/usb_function_cdc.h"
#include "D32.h"
#include "Errors.h"
#include "Main.h"
#include "Utilities.h"
#include "Ports.h"
#include "Camera.h"
#include "ExtIO.h"
#include "DataCapture.h"
#include <stdio.h>
#include <stdlib.h>


#define bitset(var,bitno) ((var) |= (1 << (bitno)))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))
#define bittst(var,bitno) (((var) & (1 << (bitno)))?1:0)
#define bitinvert(var,bitno) ((var) ^ (1 << (bitno)))

#define kTX_BUF_SIZE 			(64)			// In bytes
#define kRX_BUF_SIZE			(64)			// In bytes

#define kUSART_TX_BUF_SIZE		(64)			// In bytes
#define kUSART_RX_BUF_SIZE		(64)			// In bytes


#define k$A_TO					8000000			//sets a time out count for $A command


#define kREQUIRED	FALSE
#define kOPTIONAL	TRUE

//single / is line continuation in #define
#define advance_RX_buf_out()						\
{ 													\
	g_RX_buf_out++;									\
	if (kRX_BUF_SIZE == g_RX_buf_out)				\
	{												\
		g_RX_buf_out = 0;							\
	}												\
}

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


static char USB_In_Buffer[64];
void BlinkUSBStatus(void);
int ExtractInt(char *string, char delim, char **endPt);
int ExtractUInt(char delim);

// This byte has each of its bits used as a seperate error flag
static BYTE error_byte;
// ROM strings
const char st_version[] = {"\nPST Firmware VX.00\r\n"};
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


//
unsigned int __attribute__((section(".boot_software_key_sec,\"aw\",@nobits#"))) SoftwareKey;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void parse_packet (void);		// Take a full packet and dispatch it to the right function
//ExtractReturnType extract_number(ExtractType Type, void * ReturnValue, unsigned char Required); 								// Pull a number paramter out of the packet
signed char extract_digit (signed long * acc, unsigned char digits); // Pull a character out of the packet
//void PrintErrors (void);		// Prints out any errors in error_byte

void parse_General_packet (void);
//void parse_ST_packet (void); //Turn into echo???
void parse_BL_packet (void);


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

//useful for having multiple initial command parsers
const CommandListType CommandList[] = 
{
	{'$','0',	&parse_General_packet},
	{'B','L',	&parse_BL_packet},
//	{'#','0',   &parse_Athena_packet},	//example of having a second parser
	{0x00,0x00,	NULL}
};




/** D E C L A R A T I O N S **************************************************/

/*************parse_XXX_packet functions************************/
/*************Please place in alphabetical order **************/


// All the work of parsing our $xxxx\r commands is done here
void parse_General_packet(void)
{
    int i;
    int tempInt;
    unsigned int dataOffSet;
    int error;
    char command;
    char subCommand;
    char level;
    char *tempPt1;
    char *tempPt2;
    char header[16];
    unsigned int index;
    unsigned int curIndex;
    char numFrames;
    int tOCount;
    int oldCamCount;
    int newCamCount;
    int oldTrigCount;

    advance_RX_buf_out();
    command = toupper(g_RX_buf[g_RX_buf_out]);

    switch (command)
    {
        case 'A':					//_Armed? - pulsing cycle just before reset to zero
            if(dutPulseOn)
            {
            if (!alwaysArmed){
                tOCount = 0;
                oldCamCount = camCount;
                while(PORTReadBits(kDUTLevelPort) || camCount != camCountNum)
                {
                    if(camCount != oldCamCount)	//reset if 'ticking'
                    {
                            oldCamCount = camCount;
                            tOCount = 0;
                    }
                    if(tOCount > k$A_TO)	//if timeout exceeded, break out of while loop
                    {
                            stdout->_flag |= _IOBINARY;
                            printf("Off\r\n");
                            break;
                    }
                    tOCount++;
                } //loop till false
                /*for testing*/
                PORTToggleBits(kUserOutPort);
                stdout->_flag |= _IOBINARY;
                printf("On\r\n");
                }
            else{
              //do nothing if always arm mode is on...when arm command is sent 
                }
            }
            else{
            stdout->_flag |= _IOBINARY;
            printf("Off\r\n");
            }
            

        break;

        case 'B':   //camera captures image just before trigger
            if(dutPulseOn)
            {
                tOCount = 0;    //timeouts if not getting camera pulse
                oldCamCount = camCount;
                //get to first count on low - could already be there
                while(!PORTReadBits(kDUTLevelPort) || camCount != camCountNum)
                {
                    if(camCount != oldCamCount)	//reset if 'ticking'
                    {
                            oldCamCount = camCount;
                            tOCount = 0;
                    }
                    if(tOCount > k$A_TO)	//if timeout exceeded, return
                    {
                         stdout->_flag |= _IOBINARY;
                            printf("Off\r\n");
                            return;
                    }
                    tOCount++;
                } //loop till false
                
                tOCount = 0;
                oldCamCount = camCount;
                //trigger at low and second count
                while(PORTReadBits(kDUTLevelPort) || camCount != 1)
                {
                    if(camCount != oldCamCount)	//reset if 'ticking'
                    {
                            oldCamCount = camCount;
                            tOCount = 0;
                    }
                    if(tOCount > k$A_TO)	//if timeout exceeded, return
                    {
                            printf("Off\r\n");
                            return;
                    }
                    tOCount++;
                } //loop till false
                /*for testing*/
                PORTToggleBits(kUserOutPort);
                    //if(!PORTReadBits(kDUTLevelPort))
                 stdout->_flag |= _IOBINARY;
                printf("On\r\n");
                    //else
                    //printf("Opps\r\n");
            }
            else{
            stdout->_flag |= _IOBINARY;
            printf("Off\r\n");}

        break;

        case 'D': 					//Digital mode control

                advance_RX_buf_out();
                subCommand = toupper(g_RX_buf[g_RX_buf_out]);
                switch (subCommand)
                {
                case 'S':				//Set IO States
                                                                //Input as Level and Rising/Falling edge,
                                                                //Input as Pulse - edge ignored
                                                                //Output as Level/Pulse
                        advance_RX_buf_out();
                        userInCond = toupper(g_RX_buf[g_RX_buf_out]);
                        advance_RX_buf_out();
                        if(toupper(g_RX_buf[g_RX_buf_out]) == FALLING)
                        {userInEdge = USER_INT_FALLING;}
                        else
                        {userInEdge = USER_INT_RISING;}
                        ConfigINT2(userInEdge);
                        advance_RX_buf_out();
                        userOutCond = toupper(g_RX_buf[g_RX_buf_out]);
                        advance_RX_buf_out();
                         stdout->_flag |= _IOBINARY;
                        printf("Set\r\n");
                        break;

                case 'U':				//User Input/Output requests
                        advance_RX_buf_out();
                        subCommand = toupper(g_RX_buf[g_RX_buf_out]);
                        switch(subCommand)
                        {
                        case 'O':			//Output, change level or produce a pulse
                                advance_RX_buf_out();
                                 stdout->_flag |= _IOBINARY;
                                printf("Set\r\n");
                                if(userOutCond == LEVEL)
                                {

                                        if(toupper(g_RX_buf[g_RX_buf_out]) == 'H')
                                        {PORTSetBits(kUserOutPort);}
                                        else	//use default of low
                                        {PORTClearBits(kUserOutPort);}
                                }
                                else		//must be in pulse mode
                                {
                                        PORTToggleBits(kUserOutPort);
                                        for(i=0; i<8; i++); 	//delay
                                        PORTToggleBits(kUserOutPort);
                                }
                        //	printf("Set\r\n");
                                break;

                        case 'I':				//Inpute, start interrupt 2 to send "Ack\r\n" on interrupt
                                advance_RX_buf_out();
                                if(toupper(g_RX_buf[g_RX_buf_out]) == 'G')
                                {
                                        INTClearFlag( INT_INT2 );
                                        START_USER_INT;
                                }
                                else 		//must be Stop mode
                                {
                                        STOP_USER_INT;
                                         stdout->_flag |= _IOBINARY;
                                        printf("Set\r\n");
                                }
                                break;

                        default:	
                                 stdout->_flag |= _IOBINARY;//Send error if cannot parse
                                printf("Error\r\n");
                        }

                        break;

                default:	
                         stdout->_flag |= _IOBINARY;//Send error if cannot parse
                        printf("Error\r\n");
                }
        break;

        case 'E':					//_Ending - count just before armed
                if(PORTReadBits(kDUTLevelPort) || camCount != camCountNum-1)
                {
                     stdout->_flag |= _IOBINARY;
                    printf("N\r\n");} //if bit on or not at next to last count - not ending
                else
                {
                        /*for testing*/
                        PORTToggleBits(kTestPort);
                        PORTToggleBits(kTestPort);
                        /*for testing*/
                         stdout->_flag |= _IOBINARY;
                        printf("Y\r\n");
                }
        break;

        case 'M':						//starts _Movie mode
                movieOn = TRUE;
                advance_RX_buf_out();
                if(toupper(g_RX_buf[g_RX_buf_out]) == 'T')	//second bit indicates start condition
                        {PORTSetBits(kDUTLevelPort);}
                else
                        {PORTClearBits(kDUTLevelPort);}
                //advance_RX_buf_out();
                camCountNum = ExtractUInt(kNumDelim);//g_RX_buf[g_RX_buf_out];
                //advance_RX_buf_out();
                movieStartNum = ExtractUInt(kNumDelim);
                camCount = -2;
                INTClearFlag( INT_INT1 );
                INTEnable( INT_INT1, INT_ENABLED );
                while(camCount == -2);	//wait till armed
                /*for testing*/
                PORTToggleBits(kTestPort);
                PORTToggleBits(kTestPort);
                /*for testing*/
                 stdout->_flag |= _IOBINARY;
                printf("Go\r\n");
        break;

        case 'O':					//sets device _On/off
                advance_RX_buf_out();
                if(g_RX_buf[g_RX_buf_out] == 'H' || g_RX_buf[g_RX_buf_out] == 'h') //second byte inticates on/off
                        {
                                PORTSetBits(kDUTLevelPort);
                                PORTSetBits(kUserOutPort);
                                 stdout->_flag |= _IOBINARY;
                                printf("Set High\r\n");
                        }
                else
                        {
                                PORTClearBits(kDUTLevelPort);
                                PORTClearBits(kUserOutPort);
                                 stdout->_flag |= _IOBINARY;
                                printf("Set Low\r\n");
                        }
                break;

        case 'R':				//Software Reset
                 stdout->_flag |= _IOBINARY;
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

        case 'S': 							//_Start DUT pulsing
                advance_RX_buf_out();
                camCountNum = ExtractUInt(kNumDelim);
                //camCount = camCountNum-1;
                camCount = 0;
                dutPulseOn = TRUE;
                //ConfigIntTimer1(T1_INT_OFF | T1_INT_PRIOR_5);
                PORTClearBits(kDUTLevelPort);
                DataCaptureStop();
                INTClearFlag( INT_INT1 );
                INTEnable( INT_INT1, INT_ENABLED );
        break;

        case 'T':							//_Terminate DUT pulsing
                dutPulseOn = FALSE;
                INTEnable( INT_INT1, INT_DISABLED );
                PORTSetBits(kDUTLevelPort);	//Set-Enable needs DUT high to work
                DataCaptureStart();
        break;

        case 'V':				//Get _Version info
                stdout->_flag |= _IOBINARY;
                printf ((const char *)st_version);
        break;

    //testing case
        case '*':
            advance_RX_buf_out();
            if(g_RX_buf[g_RX_buf_out] == '1'){
                alwaysArmed = 1;
                stdout->_flag |= _IOBINARY;
                printf("\nArmed Mode\r\n");
            }
            else{
                stdout->_flag |= _IOBINARY;
                printf("\nNormal Mode\r\n");
              alwaysArmed = 0;  
            
            }
            break;
        default:
                 stdout->_flag |= _IOBINARY;
                printf("\nERROR_UC\r\n");
    }
    //flush buffer
//	for(i=0; i <kMaxWrite-1 ; i++)
//	{
//		if(g_RX_buf[g_RX_buf_out] == '\r')
//		{break;}
//		advance_RX_buf_out();
//	}

}

// if delim is ':' and data is "xxx:nnnyy" with n an ASCII number
//will advance to find ':' then extract nnn as uint, stoping at first y
int ExtractUInt(char delim)
{
	int num = 0;
	
	while(g_RX_buf[g_RX_buf_out] != delim)											//look for delimiter
	{		
		if (g_RX_buf[g_RX_buf_out] == kCR) 		//give up if hit carriage return
		{return num;}
		advance_RX_buf_out();		
	}
	
	advance_RX_buf_out();
	while(isdigit(g_RX_buf[g_RX_buf_out]))
	{
		num = 10*num + (g_RX_buf[g_RX_buf_out] - '0');
		advance_RX_buf_out();
	} 
	return num;
}

//looks for a delimitor in string and then extracts the following integer
//returns int, endPt is pointer to end point - i.e. next non-numeric character
int ExtractInt(char *string, char delim, char **endPt)
{
	int num = 0;
	char *tempPt;
	
	*endPt = NULL;
	
	if((tempPt = strchr(string, delim)) != NULL)
	{
		num = (int) strtol(tempPt+1, endPt, 10);
	}
	return num;
}

/******************************************************************************
 * Function:       Initializes buffer variables, etc. TestPort allows checking
 					on timing sequences using a scope
 * Note:            
 *****************************************************************************/
void USBInit(void)
{

	PORTSetPinsDigitalOut(kTestPort);	//setup test port
	PORTClearBits(kTestPort);
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

}//end USBInit



/********************************************************************
Checks for USB data then puts into buffer and then sends to parser
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

    //Blink the LEDs according to the USB device status
    BlinkUSBStatus();

    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	// Pull in some new data if there is new data to pull in
	numBytes += numBytesRead = getsUSBUSART(USB_In_Buffer,64);

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
		}		
	}

	// Go send any data that needs sending to PC
	check_and_send_TX_data ();
}


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

void parse_BL_packet(void)
{
    unsigned int dma_status;
    unsigned int int_status;

	// Kill USB so that we always re-enumerate when we hit the bootloader
	USBModuleDisable();

	// Set the software key
	SoftwareKey = 0x12345678;

	// TEMP : For reset testing
	/* The following code illustrates a software Reset */
	/* perform a system unlock sequence */
    mSYSTEMUnlock(int_status, dma_status);

	/* set SWRST bit to arm reset */
    RSWRSTSET = 1;

    /* read RSWRST register to trigger reset */
    volatile int* p = &RSWRST;
    *p;

    /* prevent any unwanted code execution until reset occurs*/
    while(1);
}


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
        {PORTToggleBits(kUSBBlinkPort);}
    }
}



/******************************************************************************
 * Function:        void _USB1Interrupt(void)
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
