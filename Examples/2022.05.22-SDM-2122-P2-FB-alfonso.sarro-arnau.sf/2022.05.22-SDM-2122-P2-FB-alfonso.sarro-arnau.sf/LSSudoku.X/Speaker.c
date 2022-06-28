#include <xc.h>
#include "TTimer.h"
#include "Speaker.h"
#include "LcTLCD.h" //DEBUG

#define SPOUT LATDbits.LATD7
#define PLAYING 1
#define STANDBY 0
#define DISABLED 2
//char note[5] = {1, 2, 3, 4, 5};
static char status;
static char tSpeaker;
static char note;

void initSpeaker (void) {
    TRISDbits.TRISD7 = 0;
    status = DISABLED;
    tSpeaker = TiGetTimer();
}

void motorSpeaker (void) {
    if (status == PLAYING) {
        SPOUT = ((TiGetTics(tSpeaker) & 0x01 << note) != 0);
    }
}

//((TiGetTics(tSpeaker) & 0x01) != 0)
void SPPlay (void) {
    note = 0;
    if (status != DISABLED) status = PLAYING;
    TiResetTics(tSpeaker);
}

void SPOneSec (void) {
    if (note > 3) status = STANDBY;
    note++;
}

void SPEnable (void) {
    status = STANDBY;
}

void SPDisabled (void) {
    status = DISABLED;
}