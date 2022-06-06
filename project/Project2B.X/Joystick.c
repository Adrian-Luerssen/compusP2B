
#include <xc.h>
#include "Joystick.h"

static char send;
static char x,y;
static char moved;

void initJoystick(void){
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    ADCON0 = 0x03;
    ADCON1 = 0x0D;
    ADCON2 = 0x44;
    
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    moved = 0;
}


void joystickMotor(void){
    static char state = 0;
    
    switch (state){
        case 0:
            if (ADCON0bits.GO_DONE == 0){
                y = ADRESH;
                ADCON0bits.CHS0=0;
                state = 1;
                ADCON0bits.GO_DONE=1;
            }
            break;
        case 1:
            if (ADCON0bits.GO_DONE == 0){
                x = ADRESH;
                ADCON0bits.CHS0=1;
                state = 2;
            }
            break;
        case 2:
            state = 10;
            if (moved){
                if (x >= CENTRE_L && x <= CENTRE_H && y >= CENTRE_L && y <= CENTRE_H){
                    moved = 0;
                }
            }else {
                if (x <= LEFT){
                    send = 0x41;
                    moved = 1;
                    state = 4;
                } else if (x >= RIGHT){
                    send = 0x44;
                    moved = 1;
                    state = 4;
                } else if (y <= LEFT){
                    send = 83;
                    moved = 1;
                    state = 4;
                } else if (y >= RIGHT){
                    send = 87;
                    moved = 1;
                    state = 4;
                    
                }
                
            }
            
            break;

        case 4:
            if(SiIsAvailable()){
                SiSendChar(send);
                state = 0;
                ADCON0bits.GO_DONE=1;
            }
            break;
            
        case 10:

            state = 0;
            ADCON0bits.GO_DONE=1;
            break;         
    }
    
}


char JoMoved(void){
    return moved;
}

char JoDirection(void){
    return send;
}
