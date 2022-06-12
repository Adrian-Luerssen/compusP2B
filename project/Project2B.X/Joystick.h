
#ifndef JOYSTICK_H
#define	JOYSTICK_H



#include <xc.h> // include processor files - each processor file is guarded.  
#include "SIO.h"

#define LEFT 10
#define RIGHT 240
#define CENTRE_H 150
#define CENTRE_L 100

void joystickMotor(void);
void initJoystick(void);

char JoMoved(void);

char JoDirection(void);
#endif	

