//Dictionary

//interface 0
void SiSendChar (unsigned char my_byte);
// Pre: SIO is available for sending
// Post: sends my_byte using the EUSART

char SiIsAvailable (void);
// Pre: --
// Post: returns 1 if SIO is available
// returns 0 if SIO is unabailable

void SiRecievedByte(void);
// Pre: --
// Post: returns 1 if SIO has recieved character

char SiReadByte(void);
// Pre: SiRecievedByte();
// Post: returns the character recieved

// interface 1,2,3,4

char TiGetTimer(void);
//PRE: --
//POST returns the handler of a free timer 
//and marks it as busy. 
//Returns -1 if there are no free timers.

void TiResettics(char handler);
//Pre: 
//Post: sets the tics of the handlers timer 
//to the universal tics

unsigned int TiGetTics(char handler);
//pre: handler has been returned by the TiGetTimer();
//post: returns the number of tics of the handlers timer

//Interface 5
typedef struct{
	int humidity;
	int temperature;
	char humidity_bcd[4];
	char temperature_bcd[4];
} Element;

void StStoreTemperature(int temperature, char channel);
// Pre: 0<= channel < MAXSENSORS
// Post: stores the temperature in the RAM

void StStoreHumidity(int humidity, char channel);
// Pre: 0<= channel < MAXSENSORS
// Post: stores the humidity in the RAM

void StStoreTemperatureBCD(char* temperature, char channel);
// Pre: 0<= channel < MAXSENSORS
// Post: stores the temperature BCD in the RAM

void StStoreHumidityBCD(char* humidity, char channel);
// Pre: 0<= channel < MAXSENSORS
// Post: stores the humidity BCD in the RAM

//interface 6

char StGetTemperature(char channel, char digit);
// Pre: 0<= channel < MAXSENSORS, 0<= digit <=3
// Post: gets the digit of the temperature in BCD

char StGetHumidity(char channel, char digit);
// Pre: 0<= channel < MAXSENSORS, 0<= digit <=3
// Post: gets the digit of the humidity in BCD


//interface 7
int StGetTemperatureInt(char channel);
// Pre: 0<= channel < MAXSENSORS
// Post: gets the temperature as an integer

int StGetHumidityInt(char channel);
// Pre: 0<= channel < MAXSENSORS
// Post: gets the humidity as an integer

//interface 8
void SpSoundAlarm(char status,char freq);
//Pre: status is 0 or 1. 0 < freq < 20
//Post: makes the alarm sound or not

//humans can here from 20Hz to 16Khz
//1ms at 1, 1ms at 0 --> period 2ms --> 500hz
//2ms at 1, 2ms at 0 --> period 4ms --> 250hz
//3ms at 1, 3ms at 0 --> period 6ms --> 125hz
//period = 1/20 = 50ms --> 25ms at 1, 25ms at 0

//interface 9

void HbStartStop(char status);
//PRE: status is 1 or 0
//Post: starts or stops the heartbeat depending on the status

//interface 10

void CoSendHumidity(char ascii, char channel);
//pre: ascii is an ascii char
//post: converts ascii to integer value and stores in the store ADT

void CoSendTemperature(char ascii, char channel);
//pre: ascii is an ascii char
//post: converts ascii to integer value and stores in the store ADT

void CoDataIsCorrect(char correct, char channel);
//pre: correct is 1 or 0
//post: sends a 1 if the data is correct and can be stored or sends a 0 if the data was not correct nd should be discarded