#ifndef MENU_H
#define	MENU_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "LcTLCD.h"
#include "keypad.h"
#include "TTimer.h"

void initMenu(void);
void menuMotor(void);
void displayMenu (char menuMode,char row);
#endif

