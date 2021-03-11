//Camera.h
//


int camCount;		//current count of camera triggers - gets reset in some modes
int camCountNum;	//sets reset count in dutPulseOn mode or total frames in movie mode
int movieStartNum;	//sets frame number for switching dut on/off
char dutPulseOn;	//indicates operational mode
char movieOn;		//indicates operational mode
char digitalModeOn;	//indicates operational mode
extern char alwaysArmed;

//Initializes camera interface hardware - call at program start
//real action takes place in interrupt - settings in D32.c
void InitCamera(void);

