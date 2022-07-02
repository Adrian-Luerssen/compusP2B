
#include <xc.h>
#include "SIO.h"
//#include "LcTLCD.h"
static char stateRX = 0;
static char timerRX;
static char dataRX;
static char counterRX;

static char stateTX = 0;
static char timerTX;
static char dataTX;
static char counterTX;
static char allowed = 0;
//#define init() TRISCbits.TRISC7 = TRISCbits.TRISC6 = TXSTAbits.BRGH = TXSTAbits.TXEN = RCSTAbits.SPEN = RCSTAbits.CREN = 1;
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
            if (!PORTCbits.RC5){
                //start bit received
                TiResetTics(timerRX);
                dataRX =0;
                counterRX = 0;
                stateRX = 1;
            }
            break;
        case 1:
            if (TiGetTics(timerRX) >= 1){
                if(PORTCbits.RC5){ 
                    dataRX = dataRX | 0x80;
                }else {
                    dataRX = dataRX & 0x7F;
                }
                dataRX = (dataRX >>1);
                TiResetTics(timerRX);
                counterRX++;
                if (counterRX == 8){
                    stateRX = 2;
                }
            }
            break;
        case 2:
            if(TiGetTics(timerRX) >= 24){
                if (allowed && SiIsAvailable()){
                    SiSendChar(dataRX);
                    
                    //LcClear();
                    //LcPutChar(dataRX);
                    //btSendByte(dataRX);
                }
                stateRX=0;
                    
                    
            }
            break;
    }
    switch (stateTX){
        
        case 0:
            LATCbits.LATC4 = 1;// & 0x01;
            counterTX = 0;
            break;
        case 1:
            if (TiGetTics(timerTX) >= 1) {
                stateTX++;
                LATCbits.LATC4 = 0;
                TiResetTics(timerTX);
            }
            break;
        case 2:
            if (TiGetTics(timerTX) >= 1) {
                if (counterTX > 7){
                    stateTX++;
                }else{
                    TiResetTics(timerTX);
                    LATCbits.LATC4 = ((dataTX & 0x01 << counterTX) != 0);
                    counterTX++;
                }
                
            }
            break;
        case 3:
            
            TiResetTics(timerTX);
            LATCbits.LATC4 = 1;
            stateTX = 0;
            counterTX = 0;
            
            break;

    }
}

char btAvailable(void){
    return stateTX == 0;
}
void btSendByte(char byte){
    if (allowed){
        stateTX = 1;
        dataTX = byte;
        TiResetTics(timerTX);  
    }
    
}
void setResetTXRX(void){
    allowed = !allowed;
}