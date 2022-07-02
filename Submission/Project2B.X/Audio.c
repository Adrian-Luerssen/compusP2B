/*
 * File:   Audio.c
 * Author: adril
 *
 * Created on 25 May 2022, 19:27
 */


#include <xc.h>
#include "Audio.h"



static char state= 0;
static char periodTimer;
static char timerSong;
static char period;
static char gameTime;

void initAudio(void) {
    TRISCbits.TRISC3 = 0;
    periodTimer = TiGetTimer();
    timerSong = TiGetTimer();
}

void audioMotor(void){
    switch(state){
        case 0:
            //song idle off
            period = 2;
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
            
            if (TiGetTics(timerSong) >= TSECOND){
                period++;
                TiResetTics(timerSong);
                gameTime = (gameTime+1)%60;
                
            } 
            if (gameTime > 6){
                state = 3;
                period = 2;
            }
            break;
        
    }
}

void startSong(void){
    TiResetTics(periodTimer);
    gameTime = 0;
    //period = 2;
    state = 1;
}
void stopSong(void){
    AUDIO_OFF();
    state = 0;
}
