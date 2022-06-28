#include <xc.h>
#include "Timer.h"
#include "TX.h"
#include "SIO.h"

static char timerTX;
static char timerRX;
static char stateTX;
static char toSend;
static char pointer;
static char pointerRX;
static char receive;
static char stateRX;

void initTX(){
    timerRX = TiGetTimer();
    timerTX = TiGetTimer();
    TiResetTics(timerRX);
    TiResetTics(timerTX);
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC4 = 1;
    stateTX = 0;
    stateRX = 0;
}

void motorTX() {
    switch (stateTX){
        case 0:
            LATCbits.LATC5 = 1 & 0x01;
            break;
          
        case 1:
            if(TiGetTics(timerTX) >= 1){
                TiResetTics(timerTX);
                if (pointer < 8){
                    LATCbits.LATC5 = (toSend & 0x01);
                    toSend = (toSend >> 1);
                    ++pointer;
                }
                else{ 
                    ++stateTX;
                    LATCbits.LATC5 = 1 & 0x01;
                    TiResetTics(timerTX);
                }
            }
            break;
            
        case 2:
            if(TiGetTics(timerTX) >= 1){
                LATCbits.LATC5 = 1 & 0x01;
                stateTX=0;
            }
            break;
            
    }
}
void motorRX() {
    switch (stateRX){
        case 0:
            if(PORTCbits.RC4 == 0){
                TiResetTics(timerRX);
                ++stateRX;
            }
            break;
        
        case 1:
            if (pointerRX < 8){
                if(TiGetTics(timerRX) >= 1){
                    if(PORTCbits.RC4) receive = receive | 0x80;
                    else receive = receive & 0x7F;
                    receive = (receive >> 1);
                    ++pointerRX;
                    TiResetTics(timerRX);
                }
            }
            else{
                if(TXbusy()==1){
                    setTX(receive);
                }
                ++stateRX;
                TiResetTics(timerRX);
            }
            break;
            
        case 2:
            if(TiGetTics(timerRX) >= BOUNCES){
                receive = 0;
                stateRX=0;
                pointerRX = 0; 
            }
             break;
    }
}


void sendMyTX(char send){
    toSend = send;
    LATCbits.LATC5 = 0 & 0x01; //STARTBIT
    TiResetTics(timerTX);
    stateTX = 1; 
    pointer = 0;
}

char TXgetState(){
    return stateTX;
}
