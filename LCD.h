/*
** LCD.h
Modified from microChip example
Requires Wait(xxx) from Utilities.h to get timings right
- uController is much too fast for most LCD displays
*/
#pragma once

#define HLCD    8      // LCD width = 8 characters 
#define VLCD    1       // LCD height = 1 rows

#define LCDDATA 1       // address of data register
#define LCDCMD  0       // address of command register

//Initializes LCD - must call before use
void InitLCD( void);
//Writes string to LCD
//Recongnizes \t \r \n
//excess charaters are truncated
void WriteStrLCD(char *s);



