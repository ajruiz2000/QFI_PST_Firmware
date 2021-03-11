/*

*/
//Athena must be set to 9600-1N8, ID = 01

#define kDefaultAthenaTemp 	25.0
#define kAthenaOnDelay		5
#define kAID1 '0'	//sets Athena ID bytes
#define kAID2 '1'	//assumes always fixed and same


#define kMaxWrite 14//max length of write array
#define kMaxRead 18 //max length of read array
#define kTempStep 0.1 // Athena resolution

char AthenaInit(void);
void AthenaProcessExternal(char data[]);
char AthenaSetSetTemp (float value);
float AthenaGetSetTemp (void);
float AthenaGetTemp (void);
char AthenaGetStatus (void);

float oldAthenaSetTemp;
char SetNLPower(float);

