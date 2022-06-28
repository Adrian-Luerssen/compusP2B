#ifndef KEYPAD_H
#define	KEYPAD_H

#include <xc.h>
#include "Timer.h"
#include "LcTLCD.h"
#include "SIO.h"


#define SetRowsOutput()         (TRISDbits.TRISD7 = TRISDbits.TRISD6 = TRISDbits.TRISD5 = TRISDbits.TRISD4 = 0)
#define SetColumnsInput()	    (TRISBbits.TRISB1 = TRISBbits.TRISB2 = TRISBbits.TRISB3 = 1)
#define ClrRows()               (LATDbits.LATD7 = LATDbits.LATD6 = LATDbits.LATD5 = LATDbits.LATD4 = 1)
#define Pressed()               !((PORTBbits.RB1) && (PORTBbits.RB2) && (PORTBbits.RB3))
#define COL0                    PORTBbits.RB2
#define COL1                    PORTBbits.RB1
#define COL2                    PORTBbits.RB3
#define ROW0                    LATDbits.LATD7
#define ROW1                    LATDbits.LATD4
#define ROW2                    LATDbits.LATD5
#define ROW3                    LATDbits.LATD6
#define BOUNCES                 24
#define ONESEC                  1200




void initKeypad(void);

void motorKey();

void KeyResetValue(void);

char KeyGetValue(void);

char KeyGetNumber(void);

char KeyGetSMS(void);

char isPressed();
char KeyGetPress();

#endif	
