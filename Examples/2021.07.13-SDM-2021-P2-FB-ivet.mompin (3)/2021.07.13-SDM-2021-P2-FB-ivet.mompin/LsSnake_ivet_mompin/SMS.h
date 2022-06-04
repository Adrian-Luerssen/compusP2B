#ifndef SMS_H
#define SMS_H
#include <xc.h>
#include "TTimer.h"
#include "TMatrix.h"
#include "LCD.h"
#include "TData.h"

void initSMS();
void motor_SMS();
char getKey(char key);
void SMS_PrintReset();
void SMS_Print(char chari);
void SMS_Reset();
#endif
