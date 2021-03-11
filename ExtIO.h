/*
ExtIO.h
defines and variables should be clear from names
values are set in D32 parser
*/


#define MASTER	'M'
#define SLAVE	'S'
#define	DUPLEX	'D'
#define SIMPLEX	'S'
#define LEVEL	'L'
#define PULSE	'P'
#define RISING	'R'
#define FALLING	'F'
#define USER_INT_RISING		EXT_INT_DISABLE | RISING_EDGE_INT |EXT_INT_PRI_3
#define USER_INT_FALLING 	EXT_INT_DISABLE | FALLING_EDGE_INT |EXT_INT_PRI_3
#define STOP_USER_INT 		INTEnable( INT_INT2, INT_DISABLED );
#define START_USER_INT		INTEnable( INT_INT2, INT_ENABLED );


char masterSlaveState;
char simplexDuplexState;
char userInCond;
unsigned int userInEdge;
char userOutCond;

//Initializes external IO - call early in program
//work takes place in interrupt
void InitExtIO(void);
