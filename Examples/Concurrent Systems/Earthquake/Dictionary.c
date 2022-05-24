//INT0,1,2,3,4

char TiGetTimer();
//pre:--
//post: returns hander of a timer
// marks timer as busy
void TiResetTics(char handler);
//Pre: handlder has been reterned by TiGetTics
//Post: Sets the tics of the handler to the universal tics.

unsigned int TiGetTics(char handler);
//Pre: handlder has been reterned by TiGetTics
//post:returns the number of tics on that handler
// 1 tic - 0.5ms

//INT5
void HoGetHour(char* hour, char* min);
//pre: --
//Post: it fills the hour and mins with the current time

//INT6
void SiSendChar (unsigned char my_byte);
// Pre: SIO is available for sending
// Post: sends my_byte using the EUSART

char SiIsAvailable (void);
// Pre: --
// Post: returns 1 if SIO is available
//	 returns 0 if SIO is unabailable

char SiRecievedByte(void);
//PRE:--
//POST: returns 1 if eusart has recieved. otherwise 0.
char SiReadByte(void);
//pre: SiHasRecieved() == 1
//post returns the first received byte
//WARNING: destructive reading

//INT7
void StSetShaking(char shake);
//Pre: shake is 1 or 0

//Post: activates the shaking pin depending on the shake parameter
//so the PIC (ASM) knows that it has to store data to the external RAM

void StSendData(char data);
//pre: --
//post sends data to the Store ADT (4bits + 4bits)

//INT 8
void ShStartSMS (int value);
//Pre: value has at most 4 BCD digits
//Post: starts the SMS protocol transimission (AT commands)


//INT 9
//Pre: -- 
void AdWorkNow(void);
//Post calls the function ShStartSMS with the current converted converted
//value as perameter

//INT 9
char AdGetDuty(void);
//Pre: --
//Post: