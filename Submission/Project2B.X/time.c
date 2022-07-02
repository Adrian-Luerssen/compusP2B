
#include <xc.h>
#include "time.h"
static char seconds = 0;
static char minutes = 30;
static char second = 0;
static char minute = 30;
static char sysSecond;
static char sysMinute;
static char timer;
static char state = 0;
static char game = 0;
static char val = 0;
void initTime(void){
    timer = TiGetTimer();
    sysSecond = 0;
    sysMinute = 0;
    game = 0;
}

void timeMotor(void) {
    switch (state){
        case 0:
            state = game;
            if (TiGetTics(timer) >= TSECOND){
                TiResetTics(timer);
                updateSysTime();
            }
            break;
        case 1:
            if (TiGetTics(timer) >= TSECOND){
                TiResetTics(timer);
                state = 2;
                LcGotoXY(0,1);
                updateGameTime();
                updateSysTime();
            }
            break;
        case 2:
            val = minute/10;
            sendChar();
            
            break;
        case 3:
            val = minute%10;
            sendChar();
            break;
        case 4:
            val = 10;
            sendChar();
            break;
        case 5:
            val = second/10;
            sendChar();
            break;
        case 6:
            val = second%10;
            sendChar();
            break;
        case 7:
            if (btAvailable()){
                btSendByte(' ');
                state = 1;
            }
            break;
        case 20:
            LcPutChar(minute/10+'0');
            state = 21;
            break;
        case 21:
            LcPutChar(minute%10+'0');
            state = 22;
            break;
        case 22:
            LcPutChar(':');
            state = 23;
            break;
        case 23:
            LcPutChar(second/10+'0');
            state = 24;
            break;
        case 24:
            LcPutChar(second%10+'0');
            LcGotoXY(0,1);
            state = 0;
            if (game != 0){
                LcCursorOn();
            }
            
        case 25:
            state = 0;
            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
                minute = KeGetGenericValue()-'0';
                LcPutChar(KeGetGenericValue());
                game++;
            }
            break;
        case 26:
            state = 0;
            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
                minute = minute*10 +KeGetGenericValue()-'0';
                LcPutChar(KeGetGenericValue());
                LcPutChar(':');
                game++;
            }
            break;
        case 27:
            state = 0;
            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
                second = KeGetGenericValue()-'0';
                LcPutChar(KeGetGenericValue());
                game++;
            }
            break;
        case 28:
            state = 0;
            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
                second = second*10 +KeGetGenericValue()-'0';
                LcPutChar(KeGetGenericValue());
                game++;
            }
            break;
        case 29:
            state = 0;
            if (isPressed() && KeGetGenericValue() == '#'){
                seconds = second;
                minutes = minute;
                game = 0;
            }
            break;
        case 30:
            // send the time to the blue tooth
            LcPutChar(sysMinute/10+'0');
            state = 31;
            break;
        case 31:
            // send the time to the blue tooth
            LcPutChar(sysMinute%10+'0');
            state = 32;
            break;
        case 32:
            // send the time to the blue tooth
            LcPutChar(':');
            state = 33;
            break;
        case 33:
            // send the time to the blue tooth
            LcPutChar(sysSecond/10+'0');
            state = 34;
            break;
        case 34:
            // send the time to the blue tooth
            LcPutChar(sysSecond%10+'0');
            state = 35;
            break;
        case 35:
            if (TiGetTics(timer) >= TSECOND){
                TiResetTics(timer);
                updateSysTime();
                state = 30;
                LcGotoXY(0,1);
            }
            break;
    }
}

void startTimer(void){
    state = 2;
    LcGotoXY(0,1);
    TiResetTics(timer);
}
void stopTimer(void){
    state = 0;
}
void displayTimeRemaining(void){
    game = 0;
    LcGotoXY(11,0);
    state = 20;
}
void resetTimer(void){
    second = seconds;
    minute = minutes;
}

void modifyTime(void){
    state = 20;
    game = 25;
}

void updateGameTime(void){
   
    if (second == 0){
        if (minute == 0){
            state = 0;
        }else{
            second = 59;
            minute--; 
        }
        
    }else{
        second--;
    }
}
void updateSysTime(void){
    if (sysSecond == 59){
        sysSecond = 0;
        sysMinute++;
    } else {
        sysSecond++;
    }
}

void displaySysTime(void){
    state = 30;
    LcGotoXY(0,1);
}

char timerOver(void){
    return state == 0;
}

void sendChar(void){
    if (btAvailable()){
        LcPutChar(val+'0');
        btSendByte(val+'0');
        state++;
    }
    
}