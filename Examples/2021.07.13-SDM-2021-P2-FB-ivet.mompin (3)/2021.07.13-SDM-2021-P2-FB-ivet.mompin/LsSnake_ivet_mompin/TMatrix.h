#ifndef MATRIX_H
#define MATRIX_H
#include "TTimer.h"
#include "LCD.h"
#include "EUSART.h"
#include <xc.h>
#define PRESSED()  (PORTBbits.RB5==1||PORTBbits.RB4==1||PORTBbits.RB3==1)
static char charac;
char column;
char row;
void initMatrix(void);
void motorMatrix(void);
char selectChar(char column_1,char row_1);

#endif