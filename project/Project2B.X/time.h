
#ifndef TIME_H
#define	TIME_H

#include <xc.h> 
#include "TTimer.h"
#include "LcTLCD.h"
void initTime(void);

void timeMotor(void);

void startTimer(void);

void stopTimer(void);

void resetTimer(void);
void displayTimeRemaining(void);

#endif	/* XC_HEADER_TEMPLATE_H */

