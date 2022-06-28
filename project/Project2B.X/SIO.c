
#include <xc.h>
#include "SIO.h"

static char stateRX = 0;
static char timerRX;
static char dataRX;
static char counterRX;

static char stateTX = 0;
static char timerTX;
static char dataTX;
static char counterTX;


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
    
    // NEW SERIAL PORT 1200 baud
    TRISCbits.TRISC5 = 1; // BTRX
    TRISCbits.TRISC4 = 0; // BTTX
    timerRX = TiGetTimer();
    timerTX = TiGetTimer();
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

void btMotor (void){
    switch (stateRX){
        case 0:
            if (PORTCbits.RC5 == 0){
                //start bit received
                TiResetTics(timerRX);
                dataRX =0;
                counterRX = 0;
                stateRX = 1;
            }
            break;
        case 1:
            if (counterRX >= 8){
                if (SiIsAvailable()){
                    SiSendChar(dataRX);
                }
                stateRX = 0;
            } else if (TiGetTics(timerRX) >= 1){
                if(PORTCbits.RC5){ 
                    dataRX = dataRX | 0x80;
                }else {
                    dataRX = dataRX & 0x7F;
                }
                dataRX = (dataRX >>1);
                TiResetTics(timerRX);
                counterRX++;
            }
            break;
        case 2:
            if(TiGetTics(timerRX) >= 24){
                stateRX=0; 
            }
            break;
    }
    switch (stateTX){
        case 0:
            LATCbits.LATC4 = 1 & 0x01;
            counterTX = 0;
            break;
          
        case 1:
            if(TiGetTics(timerTX) >= 1){
                if (counterTX < 8){
                    LATCbits.LATC4 = (dataTX & 0x01);
                    dataTX = (dataTX >> 1);
                    counterTX++;
                }
                else{ 
                    stateTX++;
                    LATCbits.LATC4 = 1 & 0x01;
                }
                TiResetTics(timerTX);
            }
            break;
            
        case 2:
            if(TiGetTics(timerTX) >= 1){
                LATCbits.LATC4 = 1 & 0x01;
                stateTX=0;
            }
            break; 

    }
}

char btAvailable(void){
    return stateTX == 0;
}
void btSendByte(char byte){
    stateTX = 1;
    dataTX = byte;
}