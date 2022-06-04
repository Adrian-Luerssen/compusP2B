#ifndef MENU_H
#define MENU_H
#include "TTimer.h"
#include "TMatrix.h"
#include "SMS.h"
#include "LCD.h"
#include "TData.h"
#include "marqueeMenu.h"
#include "EUSART.h"
#include "queueUser.h"


void initMenu(void);
void motorMenu();
void selectString();
void menuReset();

#endif