/*
 * File:   time.c
 * Author: adril
 *
 * Created on 13 June 2022, 18:10
 */


#include <xc.h>
#include "time.h"
static char seconds = 0;
static char minutes = 30;
static char second[2] = {0,0};
static char minute[2] = {3,0};
static char timer;
static char state = 0;
void initTime(void){
    timer = TiGetTimer();
}

void timeMotor(void) {
    switch (state){
        case 0:
            
            break;
        case 1:
            if (TiGetTics(timer) >= TSECOND){
                TiResetTics(timer);
                state = 2;
                LcGotoXY(0,1);
                if (second[1] == 0){
                    second[1] = 9;
                    if (second[0] == 0){
                        second[0] = 5;
                        if (minute[1] == 0){
                            minute[1] = 9;
                            if (minute[0] == 0){
                                state = 10;
                            }else{
                                minute[0]--;
                            }
                        }else{
                            minute[1]--;
                        }
                    }else {
                        second[0]--;
                    }
                } else {
                    second[1]--;
                }
            }
            break;
        case 2:
            // send the time to the blue tooth
            LcPutChar(minute[0]+'0');
            state = 3;
            break;
        case 3:
            // send the time to the blue tooth
            LcPutChar(minute[1]+'0');
            state = 4;
            break;
        case 4:
            // send the time to the blue tooth
            LcPutChar(':');
            state = 5;
            break;
        case 5:
            // send the time to the blue tooth
            LcPutChar(second[0]+'0');
            state = 6;
            break;
        case 6:
            // send the time to the blue tooth
            LcPutChar(second[1]+'0');
            state = 1;
            break;
        case 7:
            // send the time to the blue tooth
            LcPutChar(minute[0]+'0');
            state = 8;
            break;
        case 8:
            // send the time to the blue tooth
            LcPutChar(minute[1]+'0');
            state = 9;
            break;
        case 9:
            // send the time to the blue tooth
            LcPutChar(':');
            state = 10;
            break;
        case 10:
            // send the time to the blue tooth
            LcPutChar(second[0]+'0');
            state = 11;
            break;
        case 11:
            // send the time to the blue tooth
            LcPutChar(second[1]+'0');
            state = 0;
    }
}

void startTimer(void){
    state = 1;
    TiResetTics(timer);
}
void stopTimer(void){
    state = 0;
}
void displayTimeRemaining(void){
    state = 7;
    LcGotoXY(11,0);
}
void resetTimer(void){
    second[1] = seconds/10;
    second[0] = seconds%10;
    minute[0] = minutes%10;
    minute[1] = minutes/10;
}
