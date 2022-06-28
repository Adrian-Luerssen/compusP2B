#ifndef SIO_H
#define SIO_H


#include "LcTLCD.h"
#include "Keypad.h"



void initSIO(void);


char TXbusy();
char RXdata();
void setTX (char text);
char getRX();

#endif