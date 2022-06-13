
#include <xc.h>
#include "SIO.h"

void initSIO(void){
    
    
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 1;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    
    BAUDCONbits.BRG16 = 0;
    
    SPBRG = 255;
    
}

char SiIsAvailable(void){
    return TXSTAbits.TRMT;
}

void SiSendChar(char myByte){
    TXREG = myByte;
}

char SiRecievedByte(void){
    return PIR1bits.RCIF;
}

char SiReadByte(void){
    return RCREG;
}
