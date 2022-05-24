#include "TEusart.h"

//post: configuring a baudrate of 9600 and FOSC of 10Mhz
void initSIO (void) {
	TRISCbits.6 = 1; 
	TRISCbits.7 = 1;
	TXSTA = 0x20; //b'00100000' SYNCH = 0 BRGH = 0
	RCSTA = 0x90; //b'10010000' RECIEVING ON
	BAUDCONbits.BRG16 = 0;
	SPBRG = 15;
}


//pre:SIO is available to send
//post: sense my byte with the eusart
void SiSendChar(char my_byte){
	TXREG = my_byte;
}


//pre:
//post: returns 1 if the sio is ready to send, returns 0 otherwise
char SiIsAvailable (void) {
	return (char)TXSTAbits.TRMT;
}