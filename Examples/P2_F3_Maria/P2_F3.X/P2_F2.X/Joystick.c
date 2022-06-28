#include "Joystick.h"
#include "Menu.h"
#include "Timer.h"
#include "TX.h"

static char state;
static char centre;
static char timerJoystick;


void initJoystick(void){
	ADCON0 = 1; 
	ADCON1 = 13;
    ADCON2 = 9;
    TRISAbits.TRISA0 = 1;
    timerJoystick = TiGetTimer();
	TiResetTics(timerJoystick);
}

void motorJoystick(){
    switch(state){
        case 0://MIREM SI ESTEM EN CHANNEL 0 O 1 I ELS ANEM ALTERNANT
            if(TiGetTics(timerJoystick) > BOUNCES){
              if(ADCON0bits.CHS0 == 0){ //ESTEM AL CHANNEL 0, MIREM VALORS DE LES X
                ADCON0bits.CHS0 = 1;
                state = 2;
                } else {
                    ADCON0bits.CHS0 = 0;
                    ++state;
                }
                ADCON0bits.DONE = 1;  
            }
            break;
        
        case 1:
            if (ADCON0bits.DONE == 0){//CHANNEL 0 -> MIREM VALORS X
                if(ADRESH > 180){
                    positionJoystick = 'D';//ESQUERRA
                    centre = 0;
                }else if(ADRESH < 70){
                    positionJoystick = 'A';//DRETA
                    centre = 0;
                }else{
                    //positionJoystick = 0;
                    centre = 1;
                }
                state = 3;
                ADCON0bits.DONE = 1;                
            }
            break;
            
        case 2:
            if (ADCON0bits.DONE == 0){//CHANNEL 1 -> MIREM VALORS Y
                if(ADRESH > 180){
                    positionJoystick = 'S';//ABAIX
                    centre = 0;
                }else if(ADRESH < 70){
                    positionJoystick = 'W';//ADALT
                    centre = 0;
                }else{
                    //positionJoystick = 0;
                    centre = 2;
                }
                state = 3;
                ADCON0bits.DONE = 1;                
            }
            break;

        case 3:
            if((positionJoystick == 'S' || positionJoystick == 'W') && centre == 2 || (positionJoystick == 'A' || positionJoystick == 'D') && centre == 1){
                if(TXbusy()==1 && getGame()==1){//PODEM ENVIAR
                    setTX(getPositionJoystick());
                    resetPositionJoystick();
                }
            }
            TiResetTics(timerJoystick);
            state = 0;
            break;
    }
}
char getPositionJoystick(){
    return positionJoystick;
}
void resetPositionJoystick(){
    positionJoystick = 0;
}
char getCentreJoystick(){
    return centre;
}



