
#ifndef D32_H
#define D32_H

#define kNumDelim	'#'		//used to indicate next characters represent a number
extern void USBInit(void);
extern void ProcessIO(void);

#endif

/*
D32.C is a pared down version of the MicroChip D32.c USB example
The original example is serious bloatware - too much stuff in D32.c
and stuff in main.c that needed to be in D32.c
The USB data acquistion is a complex double-buffer arrangement. 
Read/write based on interrupts and flags - not completely syncronous
The our main code of interest is in the parser section
Suggest leaving other items alone
*/

