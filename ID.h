/* 
ID.h
Jumpers are set to three input DIO ports to define each PST unit
Primary utility is setting the voltage scaling and display format
All ports low (0) indicates jumpers are missing - invalid ID
1-7 are valid IDs
See Ports.h for port definitions
*/

#define kIDError " ERR_ID "

//deviceID is key to determining scaling and format

char deviceID;			//ID number of device	
double voltScaling;		//voltage scale of device	
char voltDisplay[10];	//format string for displaying voltage range

//Initializes variables above
//Should be called at beginning of program
void InitID(void);
