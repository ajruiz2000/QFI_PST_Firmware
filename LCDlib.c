/*
** LCDlib.c
**
** 11/12/07 LDJ v1.5 PIC32 porting
*/

#include <p32xxxx.h>
#include <plib.h>
//#include <explore.h>
#include "Main.h"
#include "LCD.h"
#include "Utilities.h"

#define PMDATA  PMDIN
void writeLCD( int addr, char c);    
char readLCD( int addr);

#define putLCD( d)  writeLCD( LCDDATA, (d))
#define cmdLCD( c)  writeLCD( LCDCMD, (c))

#define clrLCD()    writeLCD( LCDCMD, 1); Wait(1)
#define homeLCD()   writeLCD( LCDCMD, 2)   

#define setLCDG( a) writeLCD( LCDCMD, (a & 0x3F) | 0x40)
#define setLCDC( a) writeLCD( LCDCMD, (a & 0x7F) | 0x80)

#define busyLCD() ( readLCD( LCDCMD) & 0x80)
#define addrLCD() ( readLCD( LCDCMD) & 0x7F)
#define getLCD()  readLCD( LCDDATA)

void putsLCD( char *s);

void InitLCD( void)
{
    // PMP initialization 
    mPMPOpen( PMP_ON | PMP_READ_WRITE_EN | 3,
              PMP_DATA_BUS_8 | PMP_MODE_MASTER1 | 
              PMP_WAIT_BEG_4 | PMP_WAIT_MID_15 | 
              PMP_WAIT_END_4,
              0x0001,           // only PMA0 enabled 
              PMP_INT_OFF);     // no interrupts used       
        
    // wait for >30ms
    Wait( 30);
    
    //initiate the HD44780 display 8-bit init sequence
    PMPSetAddress( LCDCMD);     // select command register
    PMPMasterWrite( 0x38);      // 8-bit int, 2 lines, 5x7
    Wait( 1);                // > 48 us 
    
    PMPMasterWrite( 0x0c);      // ON, no cursor, no blink 
    Wait( 1);                // > 48 us 
    
    PMPMasterWrite( 0x01);      // clear display
    Wait( 2);                // > 1.6ms   
    
    PMPMasterWrite( 0x06);      // increment cursor, no shift
    Wait( 2);                // > 1.6ms   
} // initLCD


char readLCD( int addr)
{
    PMPSetAddress( addr);       // select register
    mPMPMasterReadByte();       // initiate read sequence
    return mPMPMasterReadByte();// read actual data
} // readLCD


void writeLCD( int addr, char c)    
{
    while( busyLCD());
    PMPSetAddress( addr);       // select register
    PMPMasterWrite( c);         // initiate write sequence
} // writeLCD
    

void putsLCD( char *s)
{  
    char c;
    
    while( *s) 
    {
        switch (*s)
        {
        case '\n':          // point to second line
            setLCDC( 0x40);
            break;
        case '\r':          // home, point to first line
            setLCDC( 0);
            break;
        case '\t':          // advance next tab (8) positions
            c = addrLCD();
            while( c & 7)
            {
                putLCD( ' '); 
                c++;
            }
            if ( c > 15)   // if necessary move to second line
                setLCDC( 0x40);
            break;                
        default:            // print character
            putLCD( *s);
            break;
        } //switch
        s++;
        Wait( 1);//need to slow things down a bit
    } //while 
} //putsLCD

void WriteStrLCD(char *s)
{
	int i;
	clrLCD();
    for(i=0; i< 10000; i++)
    {;} //need a wait state
	putsLCD(s);
	}

