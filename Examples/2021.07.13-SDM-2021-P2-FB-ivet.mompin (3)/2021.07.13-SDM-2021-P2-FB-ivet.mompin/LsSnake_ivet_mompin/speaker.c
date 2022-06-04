#include "speaker.h"



void InitSpeaker(){
    timerSpeaker1=TiGetTimer();
    timerSpeaker2=TiGetTimer();
    TiResetTics(timerSpeaker1);
    TiResetTics(timerSpeaker2);
    TRISBbits.TRISB2=0;
    cycles=1;
    state_speaker=0;
}

void motorSpeaker(){
    
    
    switch(state_speaker){
        case 0:
            if(TiGetTics(timerSpeaker1)>=2000){
                TiResetTics(timerSpeaker1);
                if(cycles<10){
                    cycles=cycles+1;
                    LATBbits.LATB2=1;
                    state_speaker=0;
                }else if(cycles>=10){
                    cycles=1;
                    LATBbits.LATB2=1;
                    state_speaker=0;
                }
                
            }if(TiGetTics(timerSpeaker2)>=cycles){
                TiResetTics(timerSpeaker2);
                LATBbits.LATB2=0;
                state_speaker=1;
            }
            break;
        case 1:
            if(TiGetTics(timerSpeaker2)>=cycles){ 
                LATBbits.LATB2=1;
                TiResetTics(timerSpeaker2);
               state_speaker=0;
            }
            break;
    }
}