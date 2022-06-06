
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef KEYPAD_H
#define	KEYPAD_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic18f4321.h>
#include "TTimer.h"
#include "LcTLCD.h"



void initKeypad(void);
void SMSMotor(void);
void KeypadMotor(void);
char isPressed(void);
char KeAvailable(void);
char KeGetCharValue(void);
char getFullValue(char generic);
char getColumn (void);
char getPresses(void);
char KeGetGenericValue(void);
void KeSetMode(char menuMode);
#endif	/* XC_HEADER_TEMPLATE_H */

