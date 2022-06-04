
#include "Joystick.h"
#define THR 100



static char x,y;



void JoyInit(void){
    ADCON0 = 0b00000101;
    ADCON1=0b00001101;
    ADCON2=0b01000100;
    TRISAbits.TRISA0=1;
    TRISAbits.TRISA1=1;
    x=0;
    y=0;
}

void motorJoyStick(void){
    static char state=0;
    switch(state){
        case 0:
            if(ADCON0bits.GO_DONE==0){
                ADCON0bits.CHS0=0;
                x=ADRESH;
                ADCON0bits.GO_DONE=1;
                state=1;
            }else{
                state=0;
            }
          break;   
        case 1:
            if(ADCON0bits.GO_DONE==0){
                ADCON0bits.CHS0=1;
                y=ADRESH;
                ADCON0bits.GO_DONE=1;
                state=2;
            }else{
                state=1;
            }
            break;
        case 2:
            if(calculate_dist(x)>calculate_dist(y)){
                state = 3;
            }else if(calculate_dist(x) < calculate_dist(y)){
                state = 4;
            }else{
                //donothing Reset values;
                state = 0;
            }
            break;
        case 3:
            if(x > 255-THR){ 
                //go right
               
                direction = 'R';
            }else if(x < THR){
                
                //go left
                
                direction = 'L';      
            }else{
                //stop
            }
            state = 0;
            break;
        case 4:
            if(y > 255-THR){ 
                
                //go up
                
                direction = 'U';
            }else if(y < THR){
                
                //go down
                
                direction = 'D';
            }else{
                //stop
            }
            state = 0;            
            break;
      }
}
char calculate_dist(char num){
    char dist = 0;
    if(num>128){
        dist=num - 128;
    }else if(num<128){
        dist=128-num;
    }
    return dist;
}