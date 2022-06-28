#include "SIO.h"
#include <xc.h>

void initSIO(void){
	TXSTA = 0x20; // 0b00100000 BRG = 0; SYNC = 0;
	RCSTA = 0x90;
	BAUDCONbits.BRG16 = 1; //259
    SPBRGH = 0x01;
	SPBRG = 0x03; 
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
}

char TXbusy(){
    return TXSTAbits.TRMT;
}

char RXdata(){
    return PIR1bits.RCIF;
}

void setTX (char text){
    TXREG = text;
}

char getRX (){
    return RCREG;
}