#ifndef JOYSTICK_H
#define	JOYSTICK_H

#include <xc.h>

#include "SIO.h"

static char positionJoystick;

void initJoystick();
void motorJoystick();
char getPositionJoystick();
void resetPositionJoystick();
char getCentreJoystick();

#endif	
