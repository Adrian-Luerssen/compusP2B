
#ifndef T_EUSART_H
#define	T_EUSART_H
#include <xc.h>
#include "LCD.h"
#include "TData.h"
#include "hour.h"
#include "speaker.h"
#include "TTimer.h"


char direction;
static const char HighScore[15]={"High score: \0"};

void EusInit();
void motorEusart();
void EusReset();
void ScoreDigits(char data, char col1, char col2,char col3,char row);


#endif 