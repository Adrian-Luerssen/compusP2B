///*
// * File:   time.c
// * Author: adril
// *
// * Created on 13 June 2022, 18:10
// */
//
//
//#include <xc.h>
//#include "time.h"
//static char seconds = 0;
//static char minutes = 30;
//static char second[2] = {0,0};
//static char minute[2] = {3,0};
//static char sysSecond[2] = {0,0};
//static char sysMinute[2] = {0,0};
//static char timer;
//static char state = 0;
//static char game = 0;
//static char val = 0;
//void initTime(void){
//    timer = TiGetTimer();
//}
//
//void timeMotor(void) {
//    switch (state){
//        case 0:
//            if (TiGetTics(timer) >= TSECOND){
//                TiResetTics(timer);
//                updateSysTime();
//            }
//            break;
//        case 1:
//            if (TiGetTics(timer) >= TSECOND){
//                TiResetTics(timer);
//                state = 2;
//                LcGotoXY(0,1);
//                updateGameTime();
//                updateSysTime();
//            }
//            break;
//        case 2:
//            // send the time to the blue tooth
//            if (btAvailable()){
//                val = minute[0]+'0';
//                sendChar(1);
//                state = 3;
//            }
//            
//            break;
//        case 3:
//            // send the time to the blue tooth
//            if (btAvailable()){
//                val = minute[1]+'0';
//                sendChar(1);
//                state = 4;
//            }
//            break;
//        case 4:
//            // send the time to the blue tooth
//            if (btAvailable()){
//                val = ':';
//                sendChar(1);
//                state = 5;
//            }
//            break;
//        case 5:
//            // send the time to the blue tooth
//            if (btAvailable()){
//                val = second[0]+'0';
//                sendChar(1);
//                state = 6;
//            }
//            break;
//        case 6:
//            // send the time to the blue tooth
//            if (btAvailable()){
//                val = second[1]+'0';
//                sendChar(1);
//                state = 1;
//            }
//            break;
//        case 20:
//            // send the time to the blue tooth
//            LcPutChar(minute[0]+'0');
//            state = 21;
//            break;
//        case 21:
//            // send the time to the blue tooth
//            LcPutChar(minute[1]+'0');
//            state = 22;
//            break;
//        case 22:
//            // send the time to the blue tooth
//            LcPutChar(':');
//            state = 23;
//            break;
//        case 23:
//            // send the time to the blue tooth
//            LcPutChar(second[0]+'0');
//            state = 24;
//            break;
//        case 24:
//            // send the time to the blue tooth
//            LcPutChar(second[1]+'0');
//            LcCursorOn();
//            LcGotoXY(0,1);
//            if (game){
//                game = 0;
//                state = 0;
//            } else {
//                state = 25;
//            }
//            
//        case 25:
//            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
//                minute[0] = KeGetGenericValue()-'0';
//                LcPutChar(KeGetGenericValue());
//                state = 26;
//            }
//            break;
//        case 26:
//            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
//                minute[1] = KeGetGenericValue()-'0';
//                LcPutChar(KeGetGenericValue());
//                LcPutChar(':');
//                state = 27;
//            }
//            break;
//        case 27:
//            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
//                second[0] = KeGetGenericValue()-'0';
//                LcPutChar(KeGetGenericValue());
//                state = 28;
//            }
//            break;
//        case 28:
//            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
//                second[1] = KeGetGenericValue()-'0';
//                LcPutChar(KeGetGenericValue());
//                state = 29;
//            }
//            break;
//        case 29:
//            if (isPressed() && KeGetGenericValue() == '#'){
//                seconds = second[0]*10 + second[1];
//                minutes = minute[0]*10 + minute[1];
//                state = 0;
//            }
//            break;
//        case 30:
//            // send the time to the blue tooth
//            LcPutChar(sysMinute[0]+'0');
//            state = 31;
//            break;
//        case 31:
//            // send the time to the blue tooth
//            LcPutChar(sysMinute[1]+'0');
//            state = 32;
//            break;
//        case 32:
//            // send the time to the blue tooth
//            LcPutChar(':');
//            state = 33;
//            break;
//        case 33:
//            // send the time to the blue tooth
//            LcPutChar(sysSecond[0]+'0');
//            state = 34;
//            break;
//        case 34:
//            // send the time to the blue tooth
//            LcPutChar(sysSecond[1]+'0');
//            state = 35;
//            break;
//        case 35:
//            if (TiGetTics(timer) >= TSECOND){
//                TiResetTics(timer);
//                updateSysTime();
//                state = 30;
//                LcGotoXY(0,1);
//            }
//            break;
//    }
//}
//
//void startTimer(void){
//    state = 2;
//    TiResetTics(timer);
//}
//void stopTimer(void){
//    state = 0;
//}
//void displayTimeRemaining(void){
//    LcGotoXY(11,0);
//    LcPutChar(minute[0]+'0');
//    LcPutChar(minute[1]+'0');
//    LcPutChar(':');
//    LcPutChar(second[0]+'0');
//    LcPutChar(second[1]+'0');
//}
//void resetTimer(void){
//    second[0] = seconds/10;
//    second[1] = seconds%10;
//    minute[1] = minutes%10;
//    minute[0] = minutes/10;
//}
//
//void modifyTime(void){
//    state = 20;
//}
//
//void updateGameTime(void){
//    if (second[1] == 0){
//        second[1] = 9;
//        if (second[0] == 0){
//            second[0] = 5;
//            startSong();
//            if (minute[1] == 0){
//                minute[1] = 9;
//                if (minute[0] == 0){
//                    state = 0;
//                }else{
//                    minute[0]--;
//                }
//            }else{
//                minute[1]--;
//            }
//        }else {
//            second[0]--;
//        }
//    } else {
//        second[1]--;
//    }
//}
//void updateSysTime(void){
//    if (sysSecond[1] == 9){
//        sysSecond[1] = 0;
//        if (sysSecond[0] == 5){
//            sysSecond[0] = 0;
//            if (sysMinute[1] == 9){
//                sysMinute[1] = 0;
//                sysMinute[0]++;
//                
//            }else{
//                sysMinute[1]++;
//            }
//        }else {
//            sysSecond[0]++;
//        }
//    } else {
//        sysSecond[1]++;
//    }
//}
//
//void displaySysTime(void){
//    state = 30;
//    LcGotoXY(0,1);
//}
//void sendChar(char bt){
//    LcPutChar(val);
//    if (bt){
//        btSendByte(val);
//    }
//}
//char timerOver(void){
//    return state == 0;
//}
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
static char second = 0;
static char minute = 30;
//static char second[2] = {0,0};
//static char minute[2] = {3,0};
//static char sysSecond[2] = {0,0};
//static char sysMinute[2] = {0,0};
static char sysSecond = 0;
static char sysMinute = 0;
static char timer;
static char state = 0;
static char game = 0;
static char val = 0;
void initTime(void){
    timer = TiGetTimer();
}

void timeMotor(void) {
    switch (state){
        case 0:
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
            // send the time to the blue tooth
            if (btAvailable()){
                val = minute/10;
                sendChar(1);
                state = 3;
            }
            
            break;
        case 3:
            // send the time to the blue tooth
            if (btAvailable()){
                val = minute%10;
                sendChar(1);
                state = 4;
            }
            break;
        case 4:
            // send the time to the blue tooth
            if (btAvailable()){
                val = 10;
                sendChar(1);
                state = 5;
            }
            break;
        case 5:
            // send the time to the blue tooth
            if (btAvailable()){
                val = second/10;
                sendChar(1);
                state = 6;
            }
            break;
        case 6:
            // send the time to the blue tooth
            if (btAvailable()){
                val = second%10;
                sendChar(1);
                state = 1;
            }
            break;
        case 20:
            // send the time to the blue tooth
            LcPutChar(minute/10+'0');
            state = 21;
            break;
        case 21:
            // send the time to the blue tooth
            LcPutChar(minute%10+'0');
            state = 22;
            break;
        case 22:
            // send the time to the blue tooth
            LcPutChar(':');
            state = 23;
            break;
        case 23:
            // send the time to the blue tooth
            LcPutChar(second/10+'0');
            state = 24;
            break;
        case 24:
            // send the time to the blue tooth
            LcPutChar(second%10+'0');
            LcGotoXY(0,1);
            if (game){
                game = 0;
                state = 0;
            } else {
                LcCursorOn();
                state = 25;
            }
            
        case 25:
            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
                minute = (KeGetGenericValue()-'0')*10;
                LcPutChar(KeGetGenericValue());
                state = 26;
            }
            break;
        case 26:
            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
                minute +=KeGetGenericValue()-'0';
                LcPutChar(KeGetGenericValue());
                LcPutChar(':');
                state = 27;
            }
            break;
        case 27:
            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
                second = (KeGetGenericValue()-'0')*10;
                LcPutChar(KeGetGenericValue());
                state = 28;
            }
            break;
        case 28:
            if (isPressed() && KeGetGenericValue() != '*' && KeGetGenericValue() != '#' ){
                second += KeGetGenericValue()-'0';
                LcPutChar(KeGetGenericValue());
                state = 29;
            }
            break;
        case 29:
            if (isPressed() && KeGetGenericValue() == '#'){
                seconds = second;
                minutes = minute;
                state = 0;
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
    TiResetTics(timer);
}
void stopTimer(void){
    state = 0;
}
void displayTimeRemaining(void){
    game = 1;
    LcGotoXY(11,0);
    state = 20;
}
void resetTimer(void){
    second = seconds;
    minute = minutes;
}

void modifyTime(void){
    state = 20;
}

void updateGameTime(void){
    if (second == 0){
        second = 59;
        minute--;
        startSong();
        if (minute == 0){
            state = 0;
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

void sendChar(char bt){
    LcPutChar(val+'0');
    if (bt){
        btSendByte(val+'0');
    }
}