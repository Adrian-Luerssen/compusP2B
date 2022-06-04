/*
 * File:   Audio.c
 * Author: adril
 *
 * Created on 25 May 2022, 19:27
 */


#include <xc.h>
#include "Audio.h"



static char state;
static char timer;
static char period;
static char start;

void initAudio(void) {
    state = 0;
    timer = TiGetTimer();
}

void audioMotor(void){
    switch(state){
        case 0:
            //song idle off
            break;
        case 1:
            if (TiGetTics(timer)>= period){
                TiResetTics(timer);
                AUDIO_ON();
                state = 2;
            }
            break;
        case 2:
            if (TiGetTics(timer)>= period){
                TiResetTics(timer);
                AUDIO_OFF();
                state = 1;
            }
            break;
        
    }
}

void startSong(void){
    TiResetTics(timer);
    state = 1;
}
void stopSong(void){
    AUDIO_OFF();
    state = 0;
}
