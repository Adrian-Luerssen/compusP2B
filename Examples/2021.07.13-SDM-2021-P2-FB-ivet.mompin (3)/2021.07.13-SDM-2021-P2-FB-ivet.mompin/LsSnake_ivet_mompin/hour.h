
#ifndef T_HOUR_H
#define	T_HOUR_H
#include <xc.h>
#include "TMatrix.h"
#include "TData.h"
#include "LCD.h"
#include "SMS.h"


void HourInit();
void motor_hour(char times_pressed);
void PrintCouples(char mode, char col1, char col2, char row, char character1, char character2, char yes);
#endif	
