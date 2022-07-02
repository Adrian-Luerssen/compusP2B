
#ifndef TIME_H
#define	TIME_H

#include <xc.h> 
#include "TTimer.h"
#include "LcTLCD.h"
#include "Audio.h"
#include "SIO.h"
#include "keypad.h"
void initTime(void);

void timeMotor(void);

void startTimer(void);

void stopTimer(void);

void resetTimer(void);
void displayTimeRemaining(void);
void modifyTime(void);
void updateSysTime(void);
void updateGameTime(void);
void displaySysTime(void);
void sendChar(void);
char timerOver(void);
#endif	/* XC_HEADER_TEMPLATE_H */

