#ifndef MENU_H
#define	MENU_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "LcTLCD.h"
#include "keypad.h"
#include "TTimer.h"
#include "EEPROM.h"
#include "Joystick.h"
#include "time.h"
#include "Audio.h"

void initMenu(void);
void menuMotor(void);
void displayMenu (char menuMode,char row);
void resetDisplay(void);
void nextRow(void);
void displayLogOut(void);
void displayLogMenu(void);

void displayLogreg(void);
void mainMenu(void);
void playScreen(void);
void displayError(void);
void displayResults(void);
void displayScoresMarquee(void);
void displayString(char* string);
#endif

