
#include <xc.h>
#include "SIO.h"

void initSIO(void){
    SPBRG=64;
    TRISC |= 0xC0; //1100 0000, the 6 LSB are not modified.
	TXSTA = 0b00100100; // 0b00100000 BRG = 0; SYNC = 0;
	RCSTA = 0b10010000;
	BAUDCONbits.BRG16 = 0;
    TRISCbits.TRISC6=1;
    TRISCbits.TRISC7=1;
    
    /*TRISCbits.TRISC5 = 1;
    TRISCbits.TRISC6 = 1;
    TXSTA = 0x24;//BRG = 0; SYNC = 0;
	RCSTA = 0x90;//0b10010000;
	BAUDCONbits.BRG16 = 0;
    SPBRG = 0x40;*/
    
}

char SiIsAvailable(void){
    return TXSTAbits.TRMT;
}

void SiSendChar(char myByte){
    TXREG = myByte;
}
