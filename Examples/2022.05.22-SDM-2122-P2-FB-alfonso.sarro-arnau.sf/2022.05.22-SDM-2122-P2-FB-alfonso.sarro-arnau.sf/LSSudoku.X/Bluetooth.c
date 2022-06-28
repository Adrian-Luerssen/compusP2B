#include <xc.h>
#include "TTimer.h"
#include "Bluetooth.h"

void initBluetooth(void) {
    //Post: configuring a baudrate of 9.6k and FOSC = 40MHz
	TRISCbits.TRISC6 = 1;
	TRISCbits.TRISC7 = 1;
	TXSTA = 0x20; //SYNC = 0; BRGH = 0;
	RCSTA = 0x90; //RECEIVING ON
	BAUDCONbits.BRG16 = 0;
	SPBRG = 64;
}

void SiSendChar(char my_byte) {
// Pre: SIO is available for sending
// Post: Sends my_byte using the EUSART
	TXREG = my_byte; //MOVFF my_byte, TXREG
}

char SiIsAvailable(void){
// Pre: --
// Post: returns 1 if SIO is ready to send, otherwise returns 0
	return (char) TXSTAbits.TRMT;
}

char SiHasReceived(void) {
    return (char) PIR1bits.RCIF;
}

char SiGetChar(void){
    return RCREG;
}


