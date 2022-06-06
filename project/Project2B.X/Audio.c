/*
 * File:   Audio.c
 * Author: adril
 *
 * Created on 25 May 2022, 19:27
 */


#include <xc.h>
#include "Audio.h"



static char state;
static char periodTimer;
static char timerSong;
static char period;
static char start;

void initAudio(void) {
    TRISCbits.TRISC3 = 0;
    state = 0;
    periodTimer = TiGetTimer();
    timerSong = TiGetTimer();
    period = 1;
}

void audioMotor(void){
    switch(state){
        case 0:
            //song idle off
            break;
        case 1:
            if (TiGetTics(periodTimer)>= (period+1)){
                TiResetTics(periodTimer);
                AUDIO_ON();
                state = 2;
            }
            break;
        case 2:
            if (TiGetTics(periodTimer)>= (period+1)){
                TiResetTics(periodTimer);
                AUDIO_OFF();
                state = 3;
            }
            break;
            
        case 3:
            state = 1;
            if (TiGetTics(timerSong) >= 1000){
                period = (period+1)%5;
                TiResetTics(timerSong);
            }
            break;
        
    }
}

void startSong(void){
    TiResetTics(periodTimer);
    state = 1;
}
void stopSong(void){
    AUDIO_OFF();
    state = 0;
}
