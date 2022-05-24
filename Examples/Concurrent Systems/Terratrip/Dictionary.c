//DICTIONARY

char TiGetTimer();
//pre:--
//post: returns hander of a timer
// marks timer as busy
void TiResetTics(char handler);
//Pre: handlder has been reterned by TiGetTics
//Post Sets the tics of the handler to the universal tics.

int TiGetTics(char handler);
//Pre: handlder has been reterned by TiGetTics
//post:returns the number of tics on that handler
// 1 tic - 5ms

//INTERFACE 4
char* getInfo(void);
//pre:
//post:returns the first address 
//of the first position of the array of structs
//info[0].forward 	

//INTERFACE 5
int SeGetTicsSensor(void);
//pre: --
//post: returns the Tics received in the 
//sensor ADT and resets the tics variable

char SeNewTicsSensor(void);
//pre:--
//post: returns 1 if the sensor has recieved new tics.
// 0 otherwise. Distructive reading.


//INTERFACE 7
void SsUpdateValue (int value);
//Pre: value <= 9999.
//Post: displays them on the 7 segment displays


//INTERFACE 8
void LeChangeStatus(char status);
//pre: status == 0, ==1, ==2
//post:
	// if status == 1 --> LedForward ON and LedBackward OFF
	// if status == 2 --> LedForward OFF and LedBackward ON
	// if status == 3 --> LedForward ON and LedBackward ON


//INTERFACE 6 && 4
void StChangeChannel(char next_previous);
//Pre: next_previous == 1 or 0
//Post: increments or decrements circularly which_channel
// (channel shown on the display) depending on next_previous.

void StClearTrip(void);
//pre:--
//Post: resets the time and distance of current channel

void StTimeTrip(void);
//pre:--
//Post: toggles between time/distance shown on the 7segments;

void SsChangeDirection(void);
//pre:--
//post: it togges the direction between forward and backward;

void SsActivateOrDisactivate(void);
//pre:--
//Post: it toggles the active variable of 
//which_channel between active and disabled