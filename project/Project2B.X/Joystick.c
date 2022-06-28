
#include <xc.h>
#include "Joystick.h"

static char send;
static char x,y;
static char moved;
static char stateJoy = 0;
static char joyMode = 0;
 
void initJoystick(void){
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    ADCON0 = 0x03;
    ADCON1 = 0x0C;
    ADCON2 = 0x44;
    
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    moved = 0;
    ADCON0bits.GO_DONE=1;
    ADCON0bits.CHS0=0;
    ADCON0bits.CHS1=1;
    stateJoy = 0;
}


void joystickMotor(void){
    switch (stateJoy){
        case 0:
            if (ADCON0bits.GO_DONE == 0){
                y = ADRESH;
                ADCON0bits.CHS0=1;
                ADCON0bits.CHS1=0;
                stateJoy = 1;
                ADCON0bits.GO_DONE=1;
            }
            break;
        case 1:
            if (ADCON0bits.GO_DONE == 0){
                x = ADRESH;
                ADCON0bits.CHS0=0;
                ADCON0bits.CHS1=1;
                stateJoy = 2;
            }
            break;
        case 2:
            stateJoy = 4;
            if (moved){
                if (x >= CENTRE_L && x <= CENTRE_H && y >= CENTRE_L && y <= CENTRE_H){
                    moved = 0;
                }
            }else {
                if (x <= LEFT){
                    send = 'A';
                    moved = 1;
                    stateJoy = 4+joyMode;
                } else if (x >= RIGHT){
                    send = 'D';
                    moved = 1;
                    stateJoy = 4+joyMode;
                } else if (y <= LEFT){
                    send = 'S';
                    moved = 1;
                    stateJoy = 4+joyMode;
                } else if (y >= RIGHT){
                    send = 'W';
                    moved = 1;
                    stateJoy = 4+joyMode;
                }
                
            }
            
            break;

        
            
        case 4:
            stateJoy = 0;
            ADCON0bits.GO_DONE=1;
            break; 
            
        case 5:
            if(SiIsAvailable()){
                SiSendChar(send);
                stateJoy = 0;
                ADCON0bits.GO_DONE=1;
            }
            break;
        default:
            stateJoy = 0;
            break;
    }
    
}


char JoMoved(void){
    return moved;
}

char JoDirection(void){
    return send;
}

void JoSetMode(char mode){
    joyMode = mode;
}
