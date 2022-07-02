
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
char getFullValue(char generic);
char getColumn (void);

char isPressed(void);
//PRE: -- 
//POST: returns 1 if a key has been pressed, returns 0 otherwise

char KeGetCharValue(void);
//PRE: isPressed()
//POST: returns the SMS character corresponding to the key pressed and number of times pressed

char getPresses(void);
//PRE: KeSetMode(1) and isPressed()
//POST: returns <= 1 if a new press is detected on the SMS keypad and >= 2 for a repeating press

char KeGetGenericValue(void);
//PRE: isPressed()
//POST: returns the generic character corresponding to the key pressed

void KeSetMode(char menuMode);
//PRE:  0 <= menuMode <= 1
//POST: mode 1 means the keypad acts as SMS
// 		mode 0 means the keypad acts as a number keypad
#endif	/* XC_HEADER_TEMPLATE_H */

