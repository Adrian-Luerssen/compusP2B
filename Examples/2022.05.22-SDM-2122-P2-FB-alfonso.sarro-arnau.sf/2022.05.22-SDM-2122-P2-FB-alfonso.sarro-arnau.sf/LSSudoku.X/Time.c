#include <xc.h>
#include "TTimer.h"
#include "Time.h"
#include "LcTLCD.h"
#include "Speaker.h"
#include "Logic.h"
#include "EUSART.h"

#define SEC 1000
#define SENDING 2
#define ENABLED 1
#define DISABLED 0

static char tTime;
static char sendIndex;
static char curSecs[2];
static char curMins[2];
static char status;
static char initSecs[2] = {0, 0};
static char initMins[2] = {0, 3};

void initTimeADT(void) {
    sendIndex = 0;
    tTime = TiGetTimer();
    status = DISABLED;
} 

void motorTime(void) {
    if(status == ENABLED) {
        if(TiGetTics(tTime) >= 1200) {
            TiResetTics(tTime);
            TimUpdateTime();
            LcGotoXY(0, 1);
            LcPutChar(curMins[1] + '0');
            LcPutChar(curMins[0] + '0');
            LcPutChar(':');
            LcPutChar(curSecs[1] + '0');
            LcPutChar(curSecs[0] + '0');
            if(status != DISABLED) status = SENDING;
            //sendTime()
        }
    }
    else if (status == SENDING) {
        switch (sendIndex) {
            case 0:
                if(EUIsAvailable()) {
                    EUSendChar(curMins[1] + '0');
                    sendIndex++;
                }
                break;
            case 1:
                if(EUIsAvailable()) {
                    EUSendChar(curMins[0] + '0');
                    sendIndex++;
                }
                break;
            case 2:
                if(EUIsAvailable()) {
                    EUSendChar(':');
                    sendIndex++;
                }
                break;
            case 3:
                if(EUIsAvailable()) {
                    EUSendChar(curSecs[1] + '0');
                    sendIndex++;
                }
                break;
            case 4:
                if(EUIsAvailable()) {
                    EUSendChar(curSecs[0] + '0');
                    sendIndex++;
                }
                break;
            case 5:
                if(EUIsAvailable()) {
                    EUSendChar('\n');
                    sendIndex = 0;
                    if(status != DISABLED) status = ENABLED;
                }
                break;
        }
    }
}

//ESTO SE PUEDE HACER + COOPERTIVO
void TimUpdateTime(void) {
    if(curSecs[0] == 0) {
        curSecs[0] = 9;
        if(curSecs[1] == 0) {
            curSecs[1] = 5;
            LoPlaySound();
            if(curMins[0] == 0) {
                curMins[0] = 9;
                if(curMins[1] == 0) {
                    curSecs[0] = 0;
                    curSecs[1] = 0;
                    curMins[0] = 0;
                    LoEndGame();
                    status = DISABLED;
                }
                else {
                    curMins[1]--;
                }
            }
            else {
                curMins[0]--;
            }
        }
        else {
            curSecs[1]--;
        }
    }
    else {
        curSecs[0]--;
        SPOneSec();
    }
    //aqui enviar variables a 
}

void TimEnableTime(void) {
    curSecs[0] = initSecs[0];
    curSecs[1] = initSecs[1];
    curMins[0] = initMins[0];
    curMins[1] = initMins[1];
    status = ENABLED;
    TiResetTics(tTime);
}

void TimDisableTime(void) {
    status = DISABLED;
}

void TimModifyTime(char newTime, char position) {
    if(position < 2) {
        initMins[!position] = newTime - '0';
        if(position == 1) LcPutChar(':');
    }
    else {
        initSecs[!(position - 2)] = newTime - '0';
    }
}

char TimGetEndTime(char endPosition) {
    if(endPosition < 2) {
        return (curMins[(endPosition == 0)] + '0');
    }
    return (curSecs[((endPosition - 2) == 0)] + '0');
}

void TimResetTime() {
    initSecs[0] = 0;
    initSecs[1] = 0;
    initMins[0] = 0;
    initMins[1] = 3;
}
