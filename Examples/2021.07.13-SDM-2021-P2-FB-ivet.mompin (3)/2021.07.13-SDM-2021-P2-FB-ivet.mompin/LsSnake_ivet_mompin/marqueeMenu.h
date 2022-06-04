
#ifndef MARQUEE_MENU_H
#define	MARQUEE_MENU_H

#include <xc.h>
#include "TTimer.h"
#include "LCD.h"

static signed char j[2];
static char r[2];
static char i[2];
static char k[2];


static signed char option3,option4;
static char sub_opt;

void MenuDoMarquee(int timer, char option3, char option4,char sub_opt);
void marqueePlaceArrow(char sub_opt);
void InitMarquee();

#endif 
