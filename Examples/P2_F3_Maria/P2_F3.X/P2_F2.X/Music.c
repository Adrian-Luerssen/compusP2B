#include <xc.h>
#include "Menu.h"
#include "Music.h"
#include "Timer.h"
int timerMusic;
char timerNote;
char musicState;
char note;
static char pointer;

void initMusic(){
    TRISDbits.TRISD3 = 0;
    timerMusic = TiGetTimer();
    timerNote = TiGetTimer();
    musicState = 1;
    note = 1;
    pointer=0;
}
void motorMusic(){
    switch(musicState){
        case 0:
            if(pointer<5){
                if(TiGetTics(timerMusic) > 1200){
                    ++pointer;
                    TiResetTics(timerMusic);
                    note *=2;
                   if (note > 16){
                       note = 1;
                   }
                }
                playNote(note);
            }else{
                TiResetTics(timerMusic);
                musicState=2;
            }
            break;
        case 1: // no music playing
            pointer=0;
            LATDbits.LATD3 = 0;
            break;
            
        case 2:
            pointer=0;
            LATDbits.LATD3 = 0;
            if(TiGetTics(timerMusic) == 1200*15){
                musicState=0;
            }
    }
}
void startMusic(){
    note = 1;
    TiResetTics(timerMusic);
    TiResetTics(timerNote);
    musicState = 0;
}

void endMusic(){
    musicState = 1;
}

void playNote(char note){
    if(TiGetTics(timerNote) > note){
        LATDbits.LATD3 = 1 - LATDbits.LATD3;
        TiResetTics(timerNote);
    }
}

